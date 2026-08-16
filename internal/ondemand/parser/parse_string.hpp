/**
 * @file parse_string.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief UTF-8 validation
 * @version 0.1.0
 * @date 2026-08-16
 * 
 * @copyright Copyright (c) 2026
 */
#pragma once
#include <string_view>
#include <cstdint>
#include <cstring>
#include <expected>
#include <string>
#include "zuu_json/error.hpp"

namespace zuu::json::ondemand::parser {

// SWAR approach: fast path for pure ASCII strings.
// A block of 8 bytes is ASCII if no bytes have their MSB (most significant bit) set.
inline std::expected<void, Error> ValidateUtf8(std::string_view str) noexcept {
    const uint8_t* data = reinterpret_cast<const uint8_t*>(str.data());
    size_t len = str.size();
    size_t i = 0;

    while (i < len) {
        // Fast path: SWAR for 8 bytes at a time
        while (i + 8 <= len) {
            uint64_t block;
            std::memcpy(&block, data + i, sizeof(uint64_t));
            
            // If any MSB is set, it's not purely ASCII. Fall back to byte-by-byte for this block.
            if ((block & 0x8080808080808080ULL) != 0) {
                break; 
            }
            i += 8;
        }

        if (i >= len) break;

        // Slow path: validate remaining bytes (including any non-ASCII blocks)
        uint8_t c = data[i];
        if (c < 0x80) { // 1-byte (ASCII)
            i++;
        } else if ((c >> 5) == 0x06) { // 2-byte sequence
            if (i + 1 >= len) return std::unexpected{Error::InvalidUnicode};
            uint8_t c2 = data[i + 1];
            if ((c2 >> 6) != 0x02) return std::unexpected{Error::InvalidUnicode};
            if (c < 0xC2) return std::unexpected{Error::InvalidUnicode}; // Overlong encoding
            i += 2;
        } else if ((c >> 4) == 0x0E) { // 3-byte sequence
            if (i + 2 >= len) return std::unexpected{Error::InvalidUnicode};
            uint8_t c2 = data[i + 1];
            uint8_t c3 = data[i + 2];
            if ((c2 >> 6) != 0x02 || (c3 >> 6) != 0x02) return std::unexpected{Error::InvalidUnicode};
            if (c == 0xE0 && c2 < 0xA0) return std::unexpected{Error::InvalidUnicode}; // Overlong encoding
            if (c == 0xED && c2 >= 0xA0) return std::unexpected{Error::InvalidUnicode}; // Surrogate halves
            i += 3;
        } else if ((c >> 3) == 0x1E) { // 4-byte sequence
            if (i + 3 >= len) return std::unexpected{Error::InvalidUnicode};
            uint8_t c2 = data[i + 1];
            uint8_t c3 = data[i + 2];
            uint8_t c4 = data[i + 3];
            if ((c2 >> 6) != 0x02 || (c3 >> 6) != 0x02 || (c4 >> 6) != 0x02) return std::unexpected{Error::InvalidUnicode};
            if (c == 0xF0 && c2 < 0x90) return std::unexpected{Error::InvalidUnicode}; // Overlong encoding
            if (c == 0xF4 && c2 >= 0x90) return std::unexpected{Error::InvalidUnicode}; // Out of bounds (> U+10FFFF)
            i += 4;
        } else {
            return std::unexpected{Error::InvalidUnicode}; // Invalid start byte
        }
    }
    
    return {}; // Success
}

inline bool HasEscape(std::string_view str) noexcept {
    return std::memchr(str.data(), '\\', str.size()) != nullptr;
}

inline std::expected<std::string, Error> UnescapeString(std::string_view str) noexcept {
    std::string result;
    result.reserve(str.size()); // At most the same size

    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '\\') {
            if (i + 1 >= str.size()) return std::unexpected{Error::InvalidFormat};
            i++;
            switch (str[i]) {
                case '"':  result.push_back('"'); break;
                case '\\': result.push_back('\\'); break;
                case '/':  result.push_back('/'); break;
                case 'b':  result.push_back('\b'); break;
                case 'f':  result.push_back('\f'); break;
                case 'n':  result.push_back('\n'); break;
                case 'r':  result.push_back('\r'); break;
                case 't':  result.push_back('\t'); break;
                case 'u': {
                    if (i + 4 >= str.size()) return std::unexpected{Error::InvalidUnicode};
                    
                    auto parse_hex = [](char c) -> int {
                        if (c >= '0' && c <= '9') return c - '0';
                        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
                        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
                        return -1;
                    };

                    uint32_t codepoint = 0;
                    for (size_t j = 1; j <= 4; ++j) {
                        int h = parse_hex(str[i + j]);
                        if (h == -1) return std::unexpected{Error::InvalidUnicode};
                        codepoint = (codepoint << 4) | static_cast<uint32_t>(h);
                    }
                    i += 4;

                    // Handle UTF-16 surrogates
                    if (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                        // High surrogate, need low surrogate
                        if (i + 6 >= str.size() || str[i + 1] != '\\' || str[i + 2] != 'u') {
                            return std::unexpected{Error::InvalidSurrogate};
                        }
                        uint32_t low_surrogate = 0;
                        for (size_t j = 3; j <= 6; ++j) {
                            int h = parse_hex(str[i + j]);
                            if (h == -1) return std::unexpected{Error::InvalidUnicode};
                            low_surrogate = (low_surrogate << 4) | static_cast<uint32_t>(h);
                        }
                        if (low_surrogate < 0xDC00 || low_surrogate > 0xDFFF) {
                            return std::unexpected{Error::InvalidSurrogate};
                        }
                        
                        codepoint = 0x10000 + (((codepoint - 0xD800) << 10) | (low_surrogate - 0xDC00));
                        i += 6;
                    } else if (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                        return std::unexpected{Error::InvalidSurrogate};
                    }

                    // Encode codepoint to UTF-8
                    if (codepoint <= 0x7F) {
                        result.push_back(static_cast<char>(codepoint));
                    } else if (codepoint <= 0x7FF) {
                        result.push_back(static_cast<char>(0xC0 | (codepoint >> 6)));
                        result.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
                    } else if (codepoint <= 0xFFFF) {
                        result.push_back(static_cast<char>(0xE0 | (codepoint >> 12)));
                        result.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
                        result.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
                    } else if (codepoint <= 0x10FFFF) {
                        result.push_back(static_cast<char>(0xF0 | (codepoint >> 18)));
                        result.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
                        result.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
                        result.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
                    } else {
                        return std::unexpected{Error::InvalidUnicode};
                    }
                    break;
                }
                default:
                    return std::unexpected{Error::InvalidFormat};
            }
        } else {
            result.push_back(str[i]);
        }
    }
    return result;
}

} // namespace zuu::json::ondemand::parser
