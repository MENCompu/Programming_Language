#define KEYWORD_LIST \
    Keyword_Def(typedef, TYPEDEF) \
    Keyword_Def(struct , STRUCT ) \
    Keyword_Def(union  , UNION  ) \
    Keyword_Def(enum   , ENUM   ) \

typedef enum {
    C_TOKEN_TAG_KEYWORD_BEGIN,

    #define Keyword_Def(name, tag) \
        P(C_TOKEN_TAG_, tag),
    KEYWORD_LIST
    #undef Keyword_Def

    C_TOKEN_TAG_META_KEYWORD_BEGIN,

    C_TOKEN_TAG_OUT,
    C_TOKEN_TAG_OPT,

    C_TOKEN_TAG_NAME,
    C_TOKEN_TAG_LITERAL,
    C_TOKEN_TAG_ELLIPSIS,
} C_Token_Tag;

typedef enum {
    TAG_u64,
    TAG_s64,
    TAG_f32,
    TAG_f64,
    TAG_String,
} Literal_Tag;

typedef struct {
    Literal_Tag tag;
    union {
        u64 varu64;
        s64 vars64;
        f32 varf32;
        f64 varf64;
        String varString;
    };
} Literal;

typedef struct {
    C_Token_Tag tag;

    u32 line;
    u32 column;
    u8* scan;

    union {
        String name;
        String optSymbolName;
        Literal lit;
    };
} C_Token;
Array(C_Token);

typedef struct {
    u32 line;
    u8* beginOfLine;

    String text;
    u8* scan;
    u8* prevScan;

    C_Token* token;
    C_Token* nextToken;

    Array_C_Token tokens;
} C_Parser_State;

Private Global String keywordsBuffer[] = {
    #define Keyword_Def(name, tag) \
        S_Const(#name),
    KEYWORD_LIST
    #undef Keyword_Def
};

Private Global C_Parser_State gParser;

Public void C_Lexer_Parse(C_Parser_State* state);

Public C_Token* C_Lexer_Advance(C_Parser_State* state) {
    if ( !state->nextToken ) { return 0; }

    state->token = state->nextToken;

    C_Lexer_Parse(state);

    if ( state->token->tag == C_TOKEN_TAG_NAME ) {
        state->token->name.E[state->token->name.count] = '\0';
    }

    return state->token;
}

Public C_Token* C_Lexer_Advance_And_Check(C_Parser_State* state, C_Token_Tag tag) {
    C_Token* token = C_Lexer_Advance(state);
    Assert( token->tag == tag );
    return token;
}

Public void C_Lexer_Update_Parser_Line(C_Parser_State* state) {
    if ( state->scan[0] == '\r' && state->scan[1] == '\n' ) { ++state->scan; }

    ++state->line;
    state->beginOfLine = state->scan;
}

Public C_Token* C_Token_New(C_Parser_State* state) {
    C_Token* token = state->nextToken = Array_Push_C(state->tokens, (C_Token){
        .line = state->line,
        .column = (u32)( state->scan - state->beginOfLine ),
    });

    return token;
}

Public b8 C_Lexer_Consume_White_Space(C_Parser_State* state) {
    b8 isKeyword = false;

    Loop {
        switch ( *state->scan ) {
            case '\r':
            case '\n': {
                C_Lexer_Update_Parser_Line(state);
            } break;

            case ' ':  { } break;
            case '\t': { } break;

            case '#': {
                while ( *state->scan != '#' ) {
                    if ( state->scan[0] == '\r' || state->scan[0] == '\n' ) {
                        C_Lexer_Update_Parser_Line(state);
                    }
                    ++state->scan;
                }
                ++state->scan;
                break;
            } break;

            case '/': {
                if ( state->scan[1] == '/' ) {
                    while ( *state->scan != '\r' && *state->scan != '\n' ) { ++state->scan; }
                    C_Lexer_Update_Parser_Line(state);
                } else if ( state->scan[1] == '*' ) {
                    u8* begin = state->scan + 2;

                    while ( !( state->scan[0] == '*' && state->scan[1] == '/' ) ) {
                        if ( state->scan[0] == '\r' || state->scan[0] == '\n' ) {
                            C_Lexer_Update_Parser_Line(state);
                        }
                        ++state->scan;
                    }

                    String metaSource = { .E = begin, .count = (u32)( state->scan - begin) };

                    String buffer[] = { S("OUT"), S("OPT") };
                    Slice_String metaKeywords = Slice_From_Raw_Array(String, buffer);

                    forrange (index, metaKeywords.count) {
                        String keyword = metaKeywords.E[index];

                        if ( Chars_Equal(metaSource.E, keyword.E, keyword.count) ) {
                            C_Token* newToken = C_Token_New(state);
                            newToken->tag = C_TOKEN_TAG_META_KEYWORD_BEGIN + 1 + index;
                            isKeyword = true;

                            if ( newToken->tag == C_TOKEN_TAG_OPT ) {
                                metaSource = Str_Skip_Begin(metaSource, keyword.count);

                                newToken->optSymbolName = Str_Skip_Begin(metaSource, Str_Find_Diff_Char_Forward(metaSource, ' '));
                                //.E = (String){ metaScan, ( &metaSource.E[metaSource.count] - metaScan ) };
                            }

                            break;
                        }
                    }
                    ++state->scan;
                    break;
                } else {
                    INVALID_PATH("");
                }
            } break;

            default: { return isKeyword; } break;
        }

        ++state->scan;
    }
}

Public String C_Parser_Parse_String(C_Parser_State* state) {
    u8* base = (u8*)state->scan++;

    Loop {
        switch ( *state->scan ) {
            // case ']': case '[':
            case ' ': case '\n': case '\r': case '\t':
            case '(':  case ')': case '{': case '}':
            case ';':  case '.': case ',': case '=': {
                return (String){ .E = base, .count = (u32)( (u8*)state->scan - base ) };
            } break;
        }

        ++state->scan;
    }
}

Public void C_Lexer_Parse(C_Parser_State* state) {
    if ( C_Lexer_Consume_White_Space(state) ) { return; }

    if ( (byte*)state->scan >= (byte*)( &state->text.E[state->text.count] ) ) {
        state->nextToken = 0;
        return;
    }

    state->prevScan = state->scan;

    C_Token* newToken = C_Token_New(state);
    newToken->scan = state->scan;

    switch ( *state->scan ) {
        case '(':  case ')': case '{': case '}':
        case ';':  case ',': case '=': {
            newToken->tag = *state->scan++;
        } break;

        case '.': {
            if ( Chars_Equal(state->scan, (u8*)"...", 3) ) {
                newToken->tag = C_TOKEN_TAG_ELLIPSIS;
                state->scan += 3;
            } else {
                newToken->tag = '.';
                ++state->scan;
            }
        } break;

        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        case '8': case '9': {
            newToken->tag = C_TOKEN_TAG_LITERAL;
            newToken->lit.tag = TAG_s64;
            *Poly(s64, &newToken->lit) = Str_To_S64(C_Parser_Parse_String(state));
        } break;

        default: {
            String name = C_Parser_Parse_String(state);

            Slice_String keywords = Slice_From_Raw_Array(String, keywordsBuffer);
            for (u32 i = 0; i < keywords.count; ++i) {
                if ( Str_Equal(keywords.E[i], name) ) {
                    newToken->tag = C_TOKEN_TAG_KEYWORD_BEGIN + 1 + i;
                    goto is_a_keyword;
                }
            } /* if is not a keyword */ {
                newToken->tag = C_TOKEN_TAG_NAME;
                newToken->name = name;
            }
            is_a_keyword:;
        } break;
    }
}

Public C_Token* C_Parser_Look_Back(C_Parser_State* state, u32 index) {
    return &state->tokens.E[state->tokens.count - 1 - 1 - index];
}

Public u8* C_Parser_Scan(C_Parser_State* state) {
    return state->prevScan;
}

Public C_Parser_State C_Parser_Init(Memory_Arena* arena, String text) {
    C_Parser_State parser;

    Array_Create(parser.tokens, (text.count / 64) + 5, arena);

    parser.text = text;
    parser.scan = text.E;

    parser.line = 1;
    parser.beginOfLine = ( parser.scan - 1 );

    parser.token = &parser.tokens.E[0];

    //C_Lexer_Consume_White_Space(&parser);
    C_Lexer_Parse(&parser);

    return parser;
}

Public void C_Parser_Reposition(C_Parser_State* state, String text) {
    Array_Clear(&state->tokens);

    state->text = text;
    state->scan = text.E;
    state->prevScan = state->scan;

    state->line = 1;
    state->beginOfLine = ( state->scan - 1 );

    //C_Lexer_Consume_White_Space(state);
    C_Lexer_Parse(state);
}
