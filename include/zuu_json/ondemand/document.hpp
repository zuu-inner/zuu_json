/**
 * @file document.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief High level document for ondemand json parser
 * @version 0.1.0
 * @date 2026-08-16
 * 
 * @copyright Copyright (c) 2026
 */

#pragma once

#include <cstdint>
#include <expected>
#include <string_view>
#include <utility>
#include <vector>

#include "zuu_json/error.hpp"
#include "zuu_json/ondemand/value.hpp"
#include "ondemand/parser/scanner.hpp"
#include "ondemand/parser/iterator.hpp"

namespace zuu::json::ondemand {

class Document {
public:
    struct Location {
        size_t line{1};
        size_t column{1};
    };

    Document(std::string_view json) noexcept : json(json) {
        if (auto res = parser::Scanner(json); res.has_value()) {
            indices = std::move(res.value());
            err = Error::None;
        } else {
            err = res.error();
            err_offset = 0; // If scanner failed at start
        }
    }

    [[nodiscard]] std::expected<Value, Error> root() const noexcept {
        if (err != Error::None) {
            return std::unexpected{err};
        }
        if (indices.empty()) {
            return std::unexpected{Error::EmptyValue};
        }
        return Value(parser::Iterator(indices, json, 0, &err_offset));
    }

    std::expected<Value, Error> operator[](std::string_view key) const noexcept {
        auto r = root();
        if (!r.has_value()) {
            return std::unexpected{r.error()};
        }
        return r.value()[key];
    }

    [[nodiscard]] Error error() const noexcept {
        return err;
    }

    [[nodiscard]] size_t getErrorOffset() const noexcept {
        return err_offset;
    }

    [[nodiscard]] Location getErrorLocation() const noexcept {
        Location loc;
        if (err_offset == std::string::npos) return loc;
        
        for (size_t i = 0; i < err_offset && i < json.size(); ++i) {
            if (json[i] == '\n') {
                loc.line++;
                loc.column = 1;
            } else {
                loc.column++;
            }
        }
        return loc;
    }

private:
    std::vector<uint32_t> indices;
    std::string_view json;
    Error err{Error::None};
    mutable size_t err_offset{std::string::npos};
};

} // namespace zuu::json::ondemand
