/**
 * @file iterator.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief Brief description
 * @version 0.3.0
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

#include "parse_literal.hpp"
#include "parse_number.hpp"
#include "parse_string.hpp"
#include "zuu_json/error.hpp"
#include "constants/json_type.hpp"

namespace zuu::json::ondemand::parser {

class Iterator {
  public:
    Iterator(const std::vector<uint32_t>& indices, std::string_view json, size_t idx = 0, size_t* err_offset = nullptr) noexcept
        : indices(indices), json(json), idx(idx), err_offset(err_offset) {}

  private:
    const std::vector<uint32_t>& indices;
    std::string_view json;
    size_t idx{};
    size_t* err_offset{};

    std::unexpected<Error> fail(Error e) const noexcept {
        if (err_offset && *err_offset == std::string::npos && e != Error::None) {
            *err_offset = (idx < indices.size()) ? indices[idx] : json.size();
        }
        return std::unexpected{e};
    }

    size_t skipWhitespace(size_t pos) const noexcept {
        while (pos < json.size() && json[pos] <= ' ') {
            pos++;
        }
        return pos;
    }

  public:
    std::expected<constants::JsonType, Error> type() const noexcept {
        if (idx >= indices.size()) {
            return fail(Error::EndOfFile);
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
                if (next_c == 't' || next_c == 'f') return constants::JsonType::Bool;
                if (next_c == 'n') return constants::JsonType::Null;
                if (next_c == '-' || (static_cast<uint32_t>(next_c - '0') < 10)) return constants::JsonType::Float;
            }
        }

        return fail(Error::InvalidType);
    }

    std::expected<std::string_view, Error> getString() noexcept {
        auto current_type = type();
        if (!current_type.has_value() || current_type.value() != constants::JsonType::String) {
            return fail(Error::InvalidType);
        }

        if (json[indices[idx]] == ':' || json[indices[idx]] == ',') {
            idx++;
        }

        uint32_t start_pos = indices[idx] + 1;

        idx++;
        if (idx >= indices.size() || json[indices[idx]] != '"') {
            return fail(Error::InvalidFormat);
        }

        uint32_t end_pos = indices[idx];
        idx++;

        std::string_view result_str(json.data() + start_pos, end_pos - start_pos);
        if (auto valid = ValidateUtf8(result_str); !valid.has_value()) {
            return fail(valid.error());
        }
        
        return result_str;
    }

    std::expected<std::string, Error> getUnescapedString() noexcept {
        auto str_res = getString();
        if (!str_res.has_value()) return fail(str_res.error());

        if (!HasEscape(str_res.value())) {
            return std::string(str_res.value());
        }

        return UnescapeString(str_res.value());
    }

    std::expected<double, Error> getNumber() noexcept {
        auto current_type = type();
        if (!current_type.has_value() || current_type.value() != constants::JsonType::Float) {
            return fail(Error::InvalidType);
        }

        size_t start_pos = skipWhitespace(indices[idx] + 1);
        size_t end_pos = (idx + 1 < indices.size()) ? indices[idx + 1] : json.size();

        std::string_view num_str(json.data() + start_pos, end_pos - start_pos);
        auto res = ParseDoubleFast(num_str);
        if (!res.has_value()) {
            return fail(res.error());
        }

        idx++;
        return res.value();
    }

	std::expected<std::nullptr_t, Error> getNull() noexcept {
		auto current_type = type();
		if (!current_type.has_value() || current_type.value() != constants::JsonType::Null) {
			return fail(Error::InvalidType);
		}

		size_t start_pos = skipWhitespace(indices[idx] + 1);
		size_t end_pos = (idx + 1 < indices.size()) ? indices[idx + 1] : json.size();

		auto res = ParseNull(std::string_view(json.data() + start_pos, end_pos - start_pos));
		if (res.has_value()) {
			idx++;
		}
		return res;
	}

	std::expected<bool, Error> getBool() noexcept {
		auto current_type = type();
		if (!current_type.has_value() || current_type.value() != constants::JsonType::Bool) {
			return fail(Error::InvalidType);
		}

		size_t start_pos = skipWhitespace(indices[idx] + 1);
		size_t end_pos = (idx + 1 < indices.size()) ? indices[idx + 1] : json.size();

		auto res = ParseBool(std::string_view(json.data() + start_pos, end_pos - start_pos));
		if (res.has_value()) {
			idx++;
		}
		return res;
	}

    std::expected<Iterator, Error> findKey(std::string_view target_key) noexcept {
        auto current_type = type();
        if (!current_type.has_value() || current_type.value() != constants::JsonType::Object) {
            return fail(Error::InvalidType);
        }

        if (idx < indices.size() && (json[indices[idx]] == ':' || json[indices[idx]] == ',')) {
            idx++;
        }

        idx++; // Advance past '{'

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
                
                if (idx >= indices.size()) return fail(Error::InvalidFormat);
                
                uint32_t end = indices[idx];
                std::string_view current_key(json.data() + start, end - start);
                idx++;
                
                if (current_key == target_key) {
                    return Iterator(indices, json, idx, err_offset);
                } else {
                    skipCurrent(); 
                }
            } else {
                return fail(Error::InvalidFormat);
            }
        }
        return fail(Error::KeyNotFound);
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