/**
 * @file value.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief High level value wrapper for ondemand json parser
 * @version 0.1.0
 * @date 2026-08-16
 * 
 * @copyright Copyright (c) 2026
 */

#pragma once

#include <expected>
#include <string_view>

#include "zuu_json/error.hpp"
#include "ondemand/parser/iterator.hpp"

namespace zuu::json::ondemand {

class Value {
public:
    Value(parser::Iterator iter) noexcept : iter(std::move(iter)) {}

    std::expected<std::string_view, Error> getString() noexcept {
        return iter.getString();
    }

    std::expected<double, Error> getNumber() noexcept {
        return iter.getNumber();
    }

    std::expected<Value, Error> operator[](std::string_view key) noexcept {
        auto child_iter = iter.findKey(key);
        if (!child_iter.has_value()) {
            return std::unexpected{child_iter.error()};
        }
        return Value(child_iter.value());
    }

private:
    parser::Iterator iter;
};

} // namespace zuu::json::ondemand
