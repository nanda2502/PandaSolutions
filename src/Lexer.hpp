#ifndef LEXER_HPP
#define LEXER_HPP

#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

enum class TokenType {
    END = 0, INVALID, SYMBOL, OPEN_BRACKET, CLOSE_BRACKET, COLON, COMMENT, STRING, KEYWORD, INDENT, DEDENT
};

struct Position {
    size_t line;
    size_t col;
};

struct Token {
    TokenType type;
    const char* text;
    size_t text_len;
    Position position;
};

const char* token_type_to_string(TokenType type);

class LexerError : public std::runtime_error {
public:
    LexerError(const std::string& message, size_t line_number, const std::string& line_text, 
               size_t error_position_start, size_t error_position_end);

private:
    static std::string format_error(const std::string& message, size_t line_number, const std::string& line_text, 
                                    size_t error_position_start, size_t error_position_end);
};

struct Lexer {
    const char* input;
    std::vector<size_t> indent_stack;
    size_t input_len;
    size_t cursor;
    size_t line;
    size_t line_start;
    size_t col;

    Lexer(const char* in, size_t len);
};

void lexer_consume_char(Lexer& l);
size_t lexer_trim_left(Lexer& l);
Token lexer_next(Lexer& l);

#endif