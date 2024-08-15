#include "Lexer.hpp"
#include <sstream>
#include <algorithm>


const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TokenType::END: return "end of input";
        case TokenType::INVALID: return "invalid token";
        case TokenType::SYMBOL: return "symbol";
        case TokenType::OPEN_BRACKET: return "open bracket";
        case TokenType::CLOSE_BRACKET: return "close bracket";
        case TokenType::COLON: return "colon symbol";
        case TokenType::COMMENT: return "comment";
        case TokenType::STRING: return "string";
        case TokenType::KEYWORD: return "keyword";
        case TokenType::INDENT: return "indent";
        case TokenType::DEDENT: return "dedent";
        default: return "unknown token type";
    }
}

LexerError::LexerError(const std::string& message, size_t line_number, const std::string& line_text, 
                       size_t error_position_start, size_t error_position_end)
    : std::runtime_error(format_error(message, line_number, line_text, error_position_start, error_position_end)) {}

std::string LexerError::format_error(const std::string& message, size_t line_number, const std::string& line_text, 
                                     size_t error_position_start, size_t error_position_end) {
    std::ostringstream oss;
    oss << message << " (line " << line_number << "):\n";
    oss << line_text << '\n';
    
    if (error_position_start < line_text.length()) {
        oss << std::string(error_position_start, ' ');
        size_t highlight_length = std::min(error_position_end, line_text.length()) - error_position_start;
        oss << "\033[1;31m" << std::string(highlight_length, '^') << "\033[0m";
    }
    
    return oss.str();
}

Lexer::Lexer(const char* in, size_t len)
    : input(in), indent_stack({}), input_len(len), cursor(0), line(1), line_start(0), col(0) {}

void lexer_consume_char(Lexer& l) {
    char x = l.input[l.cursor];
    l.cursor += 1;
    if (x == '\n') {
        l.line += 1;
        l.line_start = l.cursor;
        l.col = 0;
    } else {
        l.col += 1;
    }
}

size_t lexer_trim_left(Lexer& l) {
    size_t line_indent = 0;
    while (l.cursor < l.input_len && (std::isspace(l.input[l.cursor]) != 0) && line_indent < 4) {
        l.input[l.cursor] == '\t' ? line_indent++ : line_indent += 4;
        lexer_consume_char(l);
    }
    if (line_indent % 4 != 0) {
        std::ostringstream error_msg;
        error_msg << "Line indent must be a tab or multiple of 4 spaces. ";
        error_msg << "Current indent: " << line_indent << " spaces. ";
        throw LexerError(error_msg.str(), l.line, std::string(l.input + l.line_start, l.cursor - l.line_start), 0, line_indent);
    } 
    return line_indent / 4;
}

Token lexer_next(Lexer& l) {
    size_t new_indent = lexer_trim_left(l);

    if (l.indent_stack.empty() || new_indent > l.indent_stack.back()) {
        l.indent_stack.push_back(new_indent);
        return {TokenType::INDENT, l.input + l.cursor, 0, {l.line, l.col}};
    }

    if (new_indent < l.indent_stack.back()) {
        l.indent_stack.pop_back();
        return {TokenType::DEDENT, l.input + l.cursor, 0, {l.line, l.col}};
    }

    Token token = {
        .text = &l.input[l.cursor],
        .position = {l.line, l.col}
    };

    if (l.input[l.cursor] == '"') {
        token.type = TokenType::STRING;
        lexer_consume_char(l);
        while (l.cursor < l.input_len && l.input[l.cursor] != '"' && l.input[l.cursor] != '\n') {
            lexer_consume_char(l);
        }

        if (l.input[l.cursor] == '"') {
            token.text_len = &l.input[l.cursor] - token.text;
            lexer_consume_char(l);
            return token;
        }              
        throw LexerError("Unterminated string", l.line, std::string(token.text, &l.input[l.cursor] - token.text), token.text - l.input, l.cursor);
       
    }



}



