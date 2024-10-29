#ifndef WEALISP_PARSER_H
#define WEALISP_PARSER_H

#define OPERATOR_LIST   \
    X('\'', QUOTE)      \
    X('.' , FWD)        \
    X(':' , BWD)        \
    X('{' , CODE_BEGIN) \
    X('}' , CODE_END)   \
    X('(' , LIST_BEGIN) \
    X(')' , LIST_END)   \
    X('[' , DATA_BEGIN) \
    X(']' , DATA_END)   \
    X('<' , GROUP_BEGIN) \
    X('>' , GROUP_END)   \
    X('&' , AMPERSAND)  \
    X('*' , ASTERISK)   \
    X('-' , INNER)      \
    X('+' , OUTER)      \
    X(',' , COMMA)      \

typedef enum {
    TOKEN_TAG_NAME,
    TOKEN_TAG_LITERAL,

    TOKEN_TAG_SU_FIELD,

    #define X(ch, tag) \
        P(TOKEN_TAG_,tag),
    OPERATOR_LIST
    #undef X
} Token_Tag;

typedef enum {
    SYMBOL_UNIT_FIELD_NULL,
    SYMBOL_UNIT_FIELD_SYMBOL,
    SYMBOL_UNIT_FIELD_FN_CALL,
    SYMBOL_UNIT_FIELD_SYMBOL_FN_CALL,
} Symbol_Unit_Field;

#if 0
typedef struct {
    u16 fileID;
    u32 offset;
    u32 line;
    u32 column;
} Token_Position;
#endif

typedef enum {
    LITERAL_TAG_U64,
    LITERAL_TAG_S64,
    LITERAL_TAG_F32,
    LITERAL_TAG_F64,
    LITERAL_TAG_STR,
} Literal_Tag;

typedef struct {
    Literal_Tag tag;
    union {
        u64 U;
        s64 S;
        f32 F32;
        f64 F64;
        String string;
    };
} Literal;

typedef struct Token {
    Token_Tag tag;
    union {
        String name;
        Literal lit;
        Symbol_Unit_Field suField;
    };

    //Token_Position pos;
    u32 line;
    u32 column;
} Token;
Array(Token);

typedef struct {
    u32 line;
    u8* beginOfLine;

    String text;
    u8* scan;

    Symbol* left;

    u32 tokenID;
    Array_Token tokens;
} Parser_State;

Public Parser_State Parser_Init(Memory_Arena* arena, String text);
Public Parser_State Parser_Init_From_File(String path, Memory_Arena* arena);

Public void Parse_Symbol(Parser_State* state);

#endif // WEALISP_PARSER_H
