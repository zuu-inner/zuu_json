/**
 * @file error.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief Brief description
 * @version 0.1.0
 * @date 2026-08-12
 *
 * @copyright Copyright (c) 2026
 */

#pragma once

#include <cstdint>

namespace zuu::json {

enum class Error : uint8_t {
    None,

    CommentNotAllowed,
    DepthLimitExceeded,
    EmptyValue,
    InvalidBooleanLiteral,
    InvalidFormat,
    InvalidNullLiteral,
    InvalidSurrogate,
    InvalidType,
    InvalidUnicode,
    InvalidValue,
    IsNotArray,
    IsNotObject,
    LeadingZero,
    MissingComma,
    OutOfBound,
    RootNotArrayType,
    RootNotObjectType,
    SingleQuotedString,
    TrailingComma,
    UnescapedCharacter,
    UnquotedKey,

    Unknown
};

[[nodiscard]] constexpr const char*
    ResolveError(Error err) noexcept {
    switch (err) {
        case Error::None:
            return "No error";
        case Error::CommentNotAllowed:
            return "Comments are not allowed in standard JSON";
        case Error::DepthLimitExceeded:
            return "Maximum nesting depth limit exceeded";
        case Error::EmptyValue:
            return "Empty JSON input or value";
        case Error::InvalidBooleanLiteral:
            return "Invalid boolean literal";
        case Error::InvalidFormat:
            return "Invalid JSON format";
        case Error::InvalidNullLiteral:
            return "Invalid null literal";
        case Error::InvalidSurrogate:
            return "Invalid UTF-16 surrogate pair";
        case Error::InvalidType:
            return "Invalid or unexpected JSON type";
        case Error::InvalidUnicode:
            return "Invalid UTF-8/Unicode sequence";
        case Error::InvalidValue:
            return "Invalid JSON value";
        case Error::IsNotArray:
            return "JSON element is not an array";
        case Error::IsNotObject:
            return "JSON element is not an object";
        case Error::LeadingZero:
            return "Numbers cannot have leading zeros";
        case Error::MissingComma:
            return "Missing expected comma separator";
        case Error::OutOfBound:
            return "Index or value out of bounds";
        case Error::RootNotArrayType:
            return "Root JSON element is not an array";
        case Error::RootNotObjectType:
            return "Root JSON element is not an object";
        case Error::SingleQuotedString:
            return "Strings must use double quotes";
        case Error::TrailingComma:
            return "Trailing commas are not allowed";
        case Error::UnescapedCharacter:
            return "Unescaped control character in string";
        case Error::UnquotedKey:
            return "Object keys must be double-quoted strings";
        case Error::Unknown:
        default:
            return "Unknown JSON error";
    }
}

} // namespace zuu::json