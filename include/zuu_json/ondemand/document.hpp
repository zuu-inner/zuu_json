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
    Document(std::string_view json) noexcept : json(json) {
        if (auto res = parser::Scanner(json); res.has_value()) {
            indices = std::move(res.value());
            err = Error::None;
        } else {
            err = res.error();
        }
    }

    [[nodiscard]] std::expected<Value, Error> root() const noexcept {
        if (err != Error::None) {
            return std::unexpected{err};
        }
        if (indices.empty()) {
            return std::unexpected{Error::EmptyValue};
        }
        return Value(parser::Iterator(indices, json));
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

private:
    std::vector<uint32_t> indices;
    std::string_view json;
    Error err{Error::None};
};

} // namespace zuu::json::ondemand
