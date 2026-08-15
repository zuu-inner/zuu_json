/**
 * @file iterator.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief Brief description
 * @version 0.1.0
 * @date 2026-08-15
 *
 * @copyright Copyright (c) 2026
 */

#pragma once

#include <charconv>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <string_view>
#include <vector>

#include "zuu_json/error.hpp"

namespace zuu::json::ondemand::parser {

class Iterator {
  public:
    Iterator(const std::vector<uint32_t>& indices, std::string_view json, size_t idx = 0) noexcept
        : indices(indices), json(json), idx(idx) {}

  private:
    const std::vector<uint32_t>& indices;
    std::string_view json;
    size_t idx{};

    size_t skipWhitespace(size_t pos) const noexcept {
        while (pos < json.size() && json[pos] <= ' ') {
            pos++;
        }
        return pos;
    }

    std::expected<constants::JsonType, Error> type() const noexcept {
        if (idx >= indices.size()) {
            return std::unexpected{Error::EndOfFile};
        }

        const auto ch = json[indices[idx]];

        if (ch == '{') return constants::JsonType::Object;
        if (ch == '[') return constants::JsonType::Array;
        if (ch == '"') return constants::JsonType::String;
        
        if (ch == ':' || ch == ',') {
            size_t value_start = skipWhitespace(indices[idx] + 1);
            if (value_start < json.size()) {
                char next_c = json[value_start];
                if (next_c == '"') return constants::JsonType::String;
                if (next_c == '{') return constants::JsonType::Object;
                if (next_c == '[') return constants::JsonType::Array;
                if (next_c == 't' || next_c == 'f') return constants::JsonType::Boolean;
                if (next_c == 'n') return constants::JsonType::Null;
                if (next_c == '-' || (static_cast<uint32_t>(next_c - '0') < 10)) return constants::JsonType::Number;
            }
        }

        return std::unexpected{Error::InvalidType};
    }

    std::expected<std::string_view, Error> getString() noexcept {
        auto current_type = type();
        if (!current_type.has_value() || current_type.value() != constants::JsonType::String) {
            return std::unexpected{Error::InvalidType};
        }

        if (json[indices[idx]] == ':' || json[indices[idx]] == ',') {
            idx++;
        }

        uint32_t start_pos = indices[idx] + 1;

        idx++;
        if (idx >= indices.size() || json[indices[idx]] != '"') {
            return std::unexpected{Error::InvalidFormat};
        }

        uint32_t end_pos = indices[idx];
        idx++;

        return std::string_view(json.data() + start_pos, end_pos - start_pos);
    }

    std::expected<double, Error> getNumber() noexcept {
        auto current_type = type();
        if (!current_type.has_value() || current_type.value() != constants::JsonType::Number) {
            return std::unexpected{Error::InvalidType};
        }

        size_t start_pos = skipWhitespace(indices[idx] + 1);
        size_t end_pos = (idx + 1 < indices.size()) ? indices[idx + 1] : json.size();

        double value = 0.0;
        auto [ptr, ec] = std::from_chars(json.data() + start_pos, json.data() + end_pos, value);

        if (ec != std::errc()) {
            return std::unexpected{Error::InvalidFormat};
        }

        idx++;
        return value;
    }

    std::expected<Iterator, Error> findKey(std::string_view target_key) noexcept {
        auto current_type = type();
        if (!current_type.has_value() || current_type.value() != constants::JsonType::Object) {
            return std::unexpected{Error::InvalidType};
        }

        idx++;

        while (idx < indices.size()) {
            char c = json[indices[idx]];
            
            if (c == '}') break;
            if (c == ',') { 
                idx++; 
                continue; 
            }

            if (c == '"') {
                uint32_t start = indices[idx] + 1;
                idx++;
                
                if (idx >= indices.size()) return std::unexpected{Error::InvalidFormat};
                
                uint32_t end = indices[idx];
                std::string_view current_key(json.data() + start, end - start);
                idx++;
                
                if (current_key == target_key) {
                    return Iterator(indices, json, idx);
                } else {
                    skipCurrent(); 
                }
            } else {
                return std::unexpected{Error::InvalidFormat};
            }
        }
        return std::unexpected{Error::KeyNotFound};
    }

    void skipCurrent() noexcept {
        if (idx >= indices.size()) return;

        char c = json[indices[idx]];

        if (c == ':' || c == ',') {
            idx++;
            if (idx >= indices.size()) return;
            c = json[indices[idx]];
        }

        if (c == '{' || c == '[') {
            int depth = 1;
            idx++;
            while (depth > 0 && idx < indices.size()) {
                char curr = json[indices[idx]];
                if (curr == '{' || curr == '[') depth++;
                else if (curr == '}' || curr == ']') depth--;
                idx++;
            }
        } else if (c == '"') {
            idx += 2; 
        } else {
            idx++;
        }
    }
};

} // namespace zuu::json::ondemand::parser