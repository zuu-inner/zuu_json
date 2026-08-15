/**
 * @file token_type.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief Brief description
 * @version 0.3.0
 * @date 2026-08-12
 * 
 * @copyright Copyright (c) 2026
 */

#pragma once

#include <cstdint>

namespace zuu::json::constants {

enum class TokenType : uint8_t {
	// structural
	LBrace,
	RBrace,
	LBracket,
	RBracket,
	Colon,
	Comma,

	// value
	Quote = 129,
	Literal,
	Number,

	// eof
	EndOfFile
};

} // namespace zuu::json::constants