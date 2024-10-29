#ifndef WEALISP_PARSER_H
#define WEALISP_PARSER_H

typedef enum {
    // keywords
    TOKEN_TYPE_BEGIN_KEY_WORDS = 256,
    TOKEN_TYPE_NULL = TOKEN_TYPE_BEGIN_KEY_WORDS,

    TOKEN_TYPE_NAME,
    TOKEN_TYPE_LITERAL,

    TOKEN_TYPE_PRIM_FN_CALL,

    TOKEN_TYPE_RET_TYPE_SPEC,

    TOKEN_TYPE_LIST_BEGIN,
    TOKEN_TYPE_LIST_END,

    TOKEN_TYPE_NODE_ARRAY_BEGIN,
    TOKEN_TYPE_NODE_ARRAY_END = TOKEN_TYPE_LIST_END,
} Token_Type;

typedef struct {
    u16 fileID;
    u32 offset;
    u32 line;
    u32 column;
} Token_Position;

typedef enum {
    LITERAL_TAG_U64,
    LITERAL_TAG_S64,
    LITERAL_TAG_F32,
    LITERAL_TAG_F64,
    LITERAL_TAG_STR,
    LITERAL_TAG_TYPE,
} Literal_Tag;

typedef union {
    u64 U;
    s64 S;
    f32 F32;
    f64 F64;
    String string;
} Literal_Data;

typedef struct Token {
    Token_Type type;

    Token_Position pos;
    u32 line;
    u32 column;

    union {
        String name;
        struct {
            Literal_Tag  litTag;
            Literal_Data data;
        };
    };
} Token;

typedef struct {
    u32 capacity;
    u32 count;
    struct Token* E;
} Array_Token;

#endif // WEALISP_PARSER_H
