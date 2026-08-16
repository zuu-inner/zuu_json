/**
 * @file scanner.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief Brief description
 * @version 0.3.0
 * @date 2026-08-13
 *
 * @copyright Copyright (c) 2026
 */

#pragma once

#include <bit>
#include <cstdint>
#include <cstring>
#include <expected>
#include <string_view>
#include <vector>

#include "zuu_json/error.hpp"

#define kBlockSize 8

namespace zuu::json::ondemand::parser {

inline std::expected<std::vector<uint32_t>, Error> Scanner(std::string_view json) noexcept {
    if (json.empty()) {
        return std::unexpected{Error::EmptyValue};
    }

    auto ptr = json.data();
    const auto beg = json.data();
    const auto end = json.data() + json.size();

    std::vector<uint32_t> indices;
    indices.reserve(json.size() / 3);

    bool in_string = false;

    // fast path
    while (ptr + kBlockSize <= end) {
        uint64_t block;
        std::memcpy(&block, ptr, kBlockSize);

        constexpr uint64_t L0 = 0x0101010101010101ULL;
        constexpr uint64_t L8 = 0x8080808080808080ULL;

        auto match = [&](char c) noexcept {
            uint64_t diff = block ^ (static_cast<uint8_t>(c) * L0);
            return (diff - L0) & ~diff & L8;
        };

        uint64_t quotes = match('\"');
        uint64_t backslashes = match('\\');

        if (quotes == 0 && backslashes == 0) {
            if (!in_string) {
                uint64_t structs =
                    match('{') | match('}') | match('[') | match(']') | match(':') | match(',');

                while (structs) {
                    int tz = std::countr_zero(structs);
                    indices.push_back(static_cast<uint32_t>((ptr - beg) + (tz / 8)));
                    structs &= structs - 1;
                }
            }
            ptr += kBlockSize;
        } else {
            for (int i = 0; i < kBlockSize; ++i) {
                const auto ch = ptr[i];
                if (ch == '\"') {
                    if ((ptr + i) == beg || *(ptr + i - 1) != '\\') {
                        in_string = !in_string;
                        indices.push_back(static_cast<uint32_t>((ptr + i) - beg));
                    }
                } else if (!in_string) {
                    switch (ch) {
                    case '{':
                    case '}':
                    case '[':
                    case ']':
                    case ':':
                    case ',':
                        indices.push_back(static_cast<uint32_t>((ptr + i) - beg));
                        break;
                    default:
                        break;
                    }
                }
            }
            ptr += kBlockSize;
        }
    }

    // slow path (Sisa byte di ujung buffer yang kurang dari 8)
    while (ptr < end) {
        const auto ch = *ptr;
        if (ch == '\"') {
            if (ptr == beg || *(ptr - 1) != '\\') {
                in_string = !in_string;
                indices.push_back(static_cast<uint32_t>(ptr - beg));
            }
        } else if (!in_string) {
            switch (ch) {
            case '{':
            case '}':
            case '[':
            case ']':
            case ':':
            case ',':
                indices.push_back(static_cast<uint32_t>(ptr - beg));
                break;
            default:
                break;
            }
        }
        ptr++;
    }

    return indices;
}

} // namespace zuu::json::ondemand::parser