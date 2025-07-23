#ifndef TINYLANG_LEXER_LEXER_H
#define TINYLANG_LEXER_LEXER_H

#include "tinylang/Basic/Diagnostics.h"
#include "tinylang/Basic/LLVM.h"
#include "tinylang/Lexer/Token.h"

#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/SourceMgr.h"

namespace tinylang {

class KeywordFilter {
    llvm::StringMap<tok::TokenKind> HashTable;

    void addKeyword(StringRef Keyword,
                    tok::TokenKind TokenCode);
                
public:
    void addKeywords();

    tok::TokenKind getKeyword(
        StringRef Name,
        tok::TokenKind DefaultTokenCode = tok::unknown
    ) {
        auto Result = HashTable.find(Name);
        if (Result != HashTable.end())
            return Result->second;
        return DefaultTokenCode;
    }
};

class Lexer {
    SourceMgr &SrcMgr;
    DiagnoticsEngine &Diags;

    const char *CurPtr;
    StringRef CurBuf;

    /// CurBuffer - 현재 버퍼 인덱스로, SourceMgr 객체에서 관리됩니다.
    unsigned CurBuffer = 0;

    KeywordFilter Keywords;

public:
    Lexer(SourceMgr &SrcMgr, DiagnoticsEngine &Diags)
        : SrcMgr(SrcMgr), Diags(Diags) {
        CurBuffer = SrcMgr.getMainFilId();
        CurBuf = SrcMgr.getMemoryBuffer(CurrBuffer)->getBuffer();
        CurPtr = CurBuf.begin();
        Keywords.addKeywords();
    }

    DiagnoticsEngine &getDiagnostics() { return Diags; }

    void next(Token& Result);

    StringRef getBuffer() { return CurBuf; }

private:
    void identifier(Token& Result);
    void number(Token& Result);
    void string(Token& Result);
    void comment();

    SMLoc getLoc() { return SMLoc::getFromPointer(CurPtr); }

    void fromToken(Token &Result, const char *TokEnd,
        tok::TokenKind Kind);
}
} // namespace tinylang
#endif