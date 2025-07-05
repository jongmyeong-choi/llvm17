#ifndef LEXER_H
#define LEXER_H

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/MemoryBuffer.h"
// MemoryBuffer read only memory block 으로, 파일을 읽어올 때 쓸 수 있다.
// StringRef 는 C string 을 표현하지만 length 를 갖는다. 그래서 null terminated 가 필요가 없다.
// 이런 속성 때문에 MemoryBuffer 를 가리킬 수 있다.

class Lexer;

class Token {
    friend class Lexer;

public:
    enum TokenKind : unsigned short {
        eoi /*end of input*/, unknown/*error handling*/, ident, number, comma, colon, plus,
        minus, star, slash, l_paren, r_paren, KW_with
    };

private:
    TokenKind Kind;
    llvm::StringRef Text;

public:
    TokenKind getKind() const { return Kind; }
    llvm::StringRef getText() const { return Text; }

    bool is(TokenKind K) const { return Kind == K; }
    bool isOneOf(TokenKind K1, TokenKind K2) const {
        return is(K1) || is(K2);
    }
    template <typename... Ts>
    bool isOneOf(TokenKind K1, TokenKind K2, Ts... Ks) const {
        return is(K1) || isOneOf(K2, Ks...);
    }
};

class Lexer {
    const char* BufferStart;
    const char* BufferPtr; // null terminated 된다는 걸 기억할 것

public:
    Lexer(const llvm::StringRef& Buffer) {
        BufferStart = Buffer.begin();
        BufferPtr = BufferStart;
    }

    void next(Token &token);

private:
    void formToken(Token &Result, const char *TokEnd,
                   Token::TokenKind Kind);
};

#endif
