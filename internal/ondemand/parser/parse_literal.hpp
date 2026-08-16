/**
 * @file parse_literal.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief Brief description
 * @version 0.1.0
 * @date 2026-08-16
 * 
 * @copyright Copyright (c) 2026
 */

#pragma once

#include "zuu_json/error.hpp"

#include <expected>
#include <string_view>

namespace zuu::json::ondemand::parser {

inline std::expected<bool, Error> ParseBool(std::string_view str) noexcept {
	if (str.size() >= 3 &&
		str[1] == 'r' &&
		str[2] == 'u' &&
		str[3] == 'e'
	) {
		return true;
	} else if (str.size() >= 4 &&
		str[1] == 'a' &&
		str[2] == 'l' &&
		str[3] == 's' &&
		str[4] == 'e'
	) {
		return false;
	}

	return std::unexpected{Error::InvalidBooleanLiteral};
}

inline std::expected<std::nullptr_t, Error> ParseNull(std::string_view str) noexcept {
	if (str.size() >= 3 &&
		str[1] == 'u' &&
		str[2] == 'l' &&
		str[3] == 'l'
	) {
		return nullptr;
	}

	return std::unexpected{Error::InvalidNullLiteral};
}

} // namespace zuu::json::ondemand::parser