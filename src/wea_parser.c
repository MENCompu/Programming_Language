// LEXER

Public void Lexer_Parse(Parser_State* state);

Public b8 Parser_Is_EoF(Parser_State* state) {
    return ( state->scan == 0 );
}

Public Token* Lexer_Advance_To_Index(Parser_State* state, u32 tokenID) {
    if ( !state->scan ) { return 0; }

    forrange(counter, ( tokenID - ( (s32)state->tokens.count - 2 ) )) {
        Lexer_Parse(state);

        Token* prevToken = &state->tokens.E[state->tokens.count - 2];

        if ( prevToken->tag == TOKEN_TAG_NAME ) {
            Str_Null_Terminate(&prevToken->name);
        }
    }

    return &state->tokens.E[tokenID];
}

Public Token* Lexer_Advance(Parser_State* state) {
    return Lexer_Advance_To_Index(state, ++state->tokenID);
}

Public Token* Lexer_Advance_And_Check(Parser_State* state, Token_Tag tag) {
    Token* token = Lexer_Advance(state);
    Assert( token->tag == tag );
    return token;
}

Public void Lexer_Update_Parser_Line(Parser_State* state) {
    if ( state->scan[0] == '\r' && state->scan[1] == '\n' ) { ++state->scan; }

    ++state->line;
    state->beginOfLine = state->scan;
}

Public Token* Lexer_Look_Back(Parser_State* state, u32 index) {
    return &state->tokens.E[state->tokenID - index];
}

Public Token* Lexer_Look_Ahead(Parser_State* state, u32 index) {
    return &state->tokens.E[state->tokenID + index];
}

#if 0
typedef struct {
    u32 tokenID;
} Parser_Look_Ahead_State;

Public Token* Lexer_Look_Ahead(Parser_State* state, Parser_Look_Ahead_State* laState) {
    if ( Mem_Equal_Zero_Type(laState) ) {
        laState->tokenID = state->tokenID;
    }

    return Lexer_Advance_To_Index(state, ++laState->tokenID);
}

Public void Lexer_Consolidate_Look_Ahead(Parser_State* state, Parser_Look_Ahead_State* laState) {
    state->tokenID = laState->tokenID;
}
#endif

Public void Lexer_Consume_White_Space(Parser_State* state) {
    Loop {
        switch ( *state->scan ) {
            case '\r': case '\n': {
                Lexer_Update_Parser_Line(state);
            } break;

            case ';':  { } break;
            case ' ':  { } break;
            case '\t': { } break;

            case '/': {
                if ( state->scan[1] == '/' ) {
                    while ( *state->scan != '\r' && *state->scan != '\n' ) { ++state->scan; }
                    Lexer_Update_Parser_Line(state);
                } else if ( state->scan[1] == '*' ) {
                    while ( state->scan[0] != '*' || state->scan[1] != '/' ) {
                        if ( state->scan[0] == '\r' || state->scan[0] == '\n' ) {
                            Lexer_Update_Parser_Line(state);
                        }
                        ++state->scan;
                    }
                    ++state->scan;
                    break;
                } else {
                    INVALID_PATH("");
                    return;
                }
            } break;

            default: { return; }
        }

        ++state->scan;
    }
}

Public String Parser_Parse_String(Parser_State* state) {
    u8* base = (u8*)state->scan++;

    Loop {
        switch ( *state->scan ) {
            #define X(ch, tag) \
                case ch:
            OPERATOR_LIST
            #undef X
            case ' ': case '\n': case '\r': case '\t': {
                return (String){ .E = base, .count = (u32)( (u8*)state->scan - base ) };
            } break;

            case ';': { INVALID_PATH(""); } break;
        }

        ++state->scan;
    }

    return (String){ 0 };
}

Public void Lexer_Parse(Parser_State* state) {
    if ( state->scan >= &state->text.E[state->text.count-1] ) {
        state->scan = 0;
        return;
    }

    Token* newToken = Array_Push(state->tokens);

    newToken->line = state->line;
    newToken->column = (u32)(state->scan - state->beginOfLine);

    switch ( *state->scan ) {
        #define X(ch, inTag) \
            case ch: { newToken->tag = P(TOKEN_TAG_,inTag); ++state->scan; } break;
        OPERATOR_LIST
        #undef X

        case '\"': {
            u8* begin = ++state->scan;

            Loop {
                if ( *state->scan == '\\' ) {
                    ++state->scan;
                }

                ++state->scan;

                if ( *state->scan == '\"' ) { break; }
            }
            ++state->scan;

            String rawStrLit = { .E = begin, .count = (u32)( ( state->scan - 1 ) - begin ) };

            Local_Str(tempStrLit, rawStrLit.count);

            u8* scanRaw = rawStrLit.E;
            u8* scan = tempStrLit.E;

            while ( scanRaw <= &rawStrLit.E[rawStrLit.count - 1] ) {
                if ( *scanRaw == '\\' ) {
                    ++scanRaw;

                    switch ( *scanRaw ) {
                        case 'n':  { *scan = '\n'; } break;
                        case '\\': { *scan = '\\'; } break;
                        case '"':  { *scan = '"';  } break;
                        case '\'': { *scan = '\''; } break;
                    }
                } else {
                    *scan = *scanRaw;
                }

                ++scanRaw;
                ++scan;
            }

            tempStrLit.count = (u32)( scan - tempStrLit.E );

            newToken->tag = TOKEN_TAG_LITERAL;

            newToken->lit.tag = LITERAL_TAG_STR;
            newToken->lit.string = Str_Alloc_And_Copy(&gTempArena, tempStrLit, true);
            Str_Null_Terminate(&newToken->lit.string);
        } break;

        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        case '8': case '9': {
            newToken->tag = TOKEN_TAG_LITERAL;
            newToken->lit.tag = LITERAL_TAG_S64;
            newToken->lit.S = Str_To_S64(Parser_Parse_String(state));
        } break;

        case '~': {
            newToken->tag = TOKEN_TAG_SU_FIELD;
            ++state->scan;
            String fieldName = Parser_Parse_String(state);

            if ( 0 ) {
            } else if ( Str_Equal(fieldName, S("s")) ) {
                newToken->suField = SYMBOL_UNIT_FIELD_SYMBOL;
            } else if ( Str_Equal(fieldName, S("f")) ) {
                newToken->suField = SYMBOL_UNIT_FIELD_FN_CALL;
            } else if ( Str_Equal(fieldName, S("sf")) ) {
                newToken->suField = SYMBOL_UNIT_FIELD_SYMBOL_FN_CALL;
            } NO_ELSE
        } break;

        default: {
            newToken->tag = TOKEN_TAG_NAME;
            newToken->name = Parser_Parse_String(state);
        } break;
    }

    Lexer_Consume_White_Space(state);
}

#if 0
Public void Parser_Print_Tokens(Parser_State* state) {
    foreach (Token, token, state->tokens) {
        printf("line: %u | column: %u | value: ", token->line, token->column);

        switch ( token->tag ) {
            case TOKEN_TAG_QUOTE: {
                printf("quote");
            } break;

            case TOKEN_TAG_NAME: {
                printf("name(%.*s)", token->name.count, token->name.E);
            } break;

            case TOKEN_TAG_LITERAL: {
                switch ( token->lit.tag ) {
                    case LITERAL_TAG_U64: {
                        printf("litU(%llu)", token->lit.U);
                    } break;

                    case LITERAL_TAG_S64: {
                        printf("litS(%lld)", token->lit.S);
                    } break;

                    case LITERAL_TAG_F32: {
                        printf("litS(%f)", token->lit.F32);
                    } break;

                    case LITERAL_TAG_F64: {
                        printf("litS(%f)", token->lit.F64);
                    } break;

                    case LITERAL_TAG_STR: {
                        printf("litString(%.*s)", token->lit.string.count, token->lit.string.E);
                    } break;

                    NO_DEFAULT
                }
            } break;

            case TOKEN_TAG_CODE_BEGIN: {
                printf("{");
            } break;

            case TOKEN_TAG_CODE_END: {
                printf("}");
            } break;

            case TOKEN_TAG_LIST_BEGIN: {
                printf("(");
            } break;

            case TOKEN_TAG_LIST_END: {
                printf(")");
            } break;

            NO_DEFAULT
        }

        printf("\n");
    }
}
#endif

Public Parser_State Parser_Init(Memory_Arena* arena, String text) {
    Parser_State parser;

    Array_Create(parser.tokens, ( text.count / 2 ) + 5, arena);

    parser.text = text;
    parser.scan = text.E;

    parser.line = 1;
    parser.beginOfLine = ( parser.scan - 1 );

    parser.tokenID = (u32)( -1 );

    Lexer_Consume_White_Space(&parser);
    Lexer_Parse(&parser);
    //Lexer_Advance_To_Index(&parser, 0);

    return parser;
}

Public Parser_State Parser_Init_From_File(String path, Memory_Arena* arena) {
    String data;
    data.E = File_Read_All(path, arena, &data.count);
    return Parser_Init(arena, data);
}

Private Global String gSymbolFnsBuffer[] = {
    S_Const("Symbol_Forward"), S_Const("Symbol_Backward"), S_Const("Get_Sym"),
    S_Const("Addr"), S_Const("Deref"), S_Const("Symbol_Inner"), S_Const("Symbol_Outer"),
};

Private Global Slice_String gSymbolFns;

Public b8 Is_Symbol_Fn(Symbol* argSymbol) {
    /* Check that symbol fn name exists as an actual function. */ {
        foreach (String, symbolFn, gSymbolFns) {
            foreach (Raw_Fn, rawFn, gFnsInfo) {
                if ( Str_Equal(rawFn->name, *symbolFn) ) {
                    goto symbol_fn_exists;
                }
            } /* IF NOT symbol_fn_exists */ {
                INVALID_PATH("symbol fn doesn't exists.");
            }

            symbol_fn_exists:;
        }
    }

    Symbol* fnNameSymbol = (*D(Fn_Call*, (*D(Fn_Call*, argSymbol))->fnFnCall))->args->childs.E[0].symbol;
    Assert( fnNameSymbol->type == Type_All_Get(S("String")) );
    String fnName = *D(String, fnNameSymbol);

    foreach (String, symbolFn, gSymbolFns) {
        if ( Str_Equal(fnName, *symbolFn) ) {
            return true;
        }
    }

    return false;
}

Public Symbol_Unit Set_Default_Arg(Symbol* argSymbol) {
    Symbol_Unit arg = { 0 };

    if ( argSymbol->type == Type_All_Get(S("Fn_Call*")) ) {
        if ( Is_Symbol_Fn(argSymbol) ) {
            arg.symbolFnCall = argSymbol;
        } else {
            arg.fnCall = argSymbol;
        }
    } else {
        arg.symbol = argSymbol;
    }

    return arg;
}

Public Symbol* Parse_Symbol_L(Parser_State* state);

Public Symbol* Parse_Arg(Parser_State* state) {
    while ( Lexer_Look_Ahead(state, 1)->tag != TOKEN_TAG_COMMA    &&
            Lexer_Look_Ahead(state, 1)->tag != TOKEN_TAG_DATA_END &&
            Lexer_Look_Ahead(state, 1)->tag != TOKEN_TAG_LIST_END &&
            Lexer_Look_Ahead(state, 1)->tag != TOKEN_TAG_CODE_END &&
            Lexer_Look_Ahead(state, 1)->tag != TOKEN_TAG_GROUP_END ) {
        Parse_Symbol(state);
    }

    Symbol* out = state->left;
    state->left = 0;
    return out;
}

Public Symbol_Unit Parse_Arg_Unit(Parser_State* state, Symbol** outSymbol) {
    Symbol_Unit out = { 0 };

    Symbol* dummy = 0;
    if ( !outSymbol ) { outSymbol = &dummy; }

    Symbol_Unit_Field suField = SYMBOL_UNIT_FIELD_NULL;

    if ( Lexer_Look_Ahead(state, 1)->tag == TOKEN_TAG_SU_FIELD ) {
        suField = Lexer_Advance(state)->suField;
    }

    Symbol* argSymbol = Parse_Arg(state);

    if ( suField != SYMBOL_UNIT_FIELD_NULL ) {
        switch ( suField ) {
            case SYMBOL_UNIT_FIELD_SYMBOL: { out.symbol = argSymbol; } break;
            case SYMBOL_UNIT_FIELD_FN_CALL: { out.fnCall = argSymbol; } break;
            case SYMBOL_UNIT_FIELD_SYMBOL_FN_CALL: { out.symbolFnCall = argSymbol; } break;

            case SYMBOL_UNIT_FIELD_NULL:
            NO_DEFAULT
        }

        *outSymbol = 0;
    } else {
        *outSymbol = argSymbol;
    }

    return out;
}

/////////
// PARSER
Public void Parse_Symbol(Parser_State* state) {
    Symbol* out = 0;
    Token* token;

    if ( ( token = Lexer_Advance(state) ) == 0 ) { return; }

    switch ( token->tag ) {
        case TOKEN_TAG_COMMA: { INVALID_PATH(""); } break;

        case TOKEN_TAG_DATA_BEGIN: {
            Local_Array(Symbol_Unit, args, KiB(1));

            while ( Lexer_Look_Ahead(state, 0)->tag != TOKEN_TAG_DATA_END ) {
                Symbol* argSymbol = 0;
                Symbol_Unit arg = Parse_Arg_Unit(state, &argSymbol);
                Array_Push_C(args, ( !argSymbol ) ? arg : Set_Default_Arg(argSymbol));

                Lexer_Advance(state); // Consume ',' or ']'.
            }

            out = PF_By_Name(S("Lit"), args);
        } break;

        case TOKEN_TAG_DATA_END: { INVALID_PATH(""); } break;

        case TOKEN_TAG_LIST_BEGIN: {
            Local_Array(Symbol_Unit, args, KiB(1));

            while ( Lexer_Look_Ahead(state, 0)->tag != TOKEN_TAG_LIST_END ) {
                Symbol* argSymbol = 0;
                Symbol_Unit arg = Parse_Arg_Unit(state, &argSymbol);
                Array_Push_C(args, ( !argSymbol ) ? arg : Set_Default_Arg(argSymbol));

                Lexer_Advance(state); // Consume ',' or ')'.
            }

            out = PF_By_Name(S("Fn_Call_Create"), args);
        } break;

        case TOKEN_TAG_LIST_END: { INVALID_PATH(""); } break;

        case TOKEN_TAG_CODE_BEGIN: {
#if 1
            Local_Array(Symbol_Unit, code, KiB(1));

            while ( Lexer_Look_Ahead(state, 0)->tag != TOKEN_TAG_CODE_END ) {
                Symbol* argSymbol = 0;
                Symbol_Unit arg = Parse_Arg_Unit(state, &argSymbol);
                if ( argSymbol ) {
                    if ( argSymbol->type == Type_All_Get(S("Fn_Call*")) ) {
                        if ( Is_Symbol_Fn(argSymbol) ) {
                            arg.symbolFnCall = argSymbol;
                        } else {
                            arg.symbol = argSymbol;
                        }
                    } NO_ELSE
                }

                Array_Push_C(code, arg);

                Lexer_Advance(state); // Consume ',' or '}'.
            }

            out = PF_By_Name(S("Lit"), code);
#else
            Local_Array(Fn_Call_P, tempCode, KiB(1));

            Loop {
                Symbol_Unit arg = Parse_Symbol_L(state);
                if ( Mem_Equal_Zero_Type(&arg) ) { break; }

                Assert( arg.fnCall );
                Array_Push_C(tempCode, arg.fnCall);
            }

            Array_Fn_Call_P code;
            Array_Alloc_And_Copy(&code, tempCode, &gSymbolState.dataArena);

            out.symbol = L(Array_Fn_Call_P, code);
#endif
        } break;

        case TOKEN_TAG_CODE_END: { INVALID_PATH(""); } break;

        case TOKEN_TAG_GROUP_BEGIN: {
            out = Parse_Arg(state);
            Lexer_Advance(state); // Consume '>'.
        } break;

        case TOKEN_TAG_GROUP_END: { INVALID_PATH(""); } break;

        case TOKEN_TAG_QUOTE: {
            Symbol* argSymbol = Parse_Symbol_L(state);
            Assert( argSymbol->type == Type_All_Get(S("Fn_Call*")) );

            out = argSymbol;
        } break;

        case TOKEN_TAG_NAME: {
            out = PF(Get_Sym, { .symbol = Wea_Str_Lit(token->name), } );
        } break;

        case TOKEN_TAG_LITERAL: {
            Assert( state->left == 0 ); // You're probablly missing a comma.

            switch ( token->lit.tag ) {
                case LITERAL_TAG_STR: {
                    out = Wea_Str_Lit(token->lit.string);
                } break;

                case LITERAL_TAG_S64: {
                    out = L(u32, token->lit.U);
                } break;

                case LITERAL_TAG_U64: { INVALID_PATH(""); } break;
                case LITERAL_TAG_F32: { INVALID_PATH(""); } break;
                case LITERAL_TAG_F64: { INVALID_PATH(""); } break;
            }
        } break;

        case TOKEN_TAG_AMPERSAND: {
            //Assert( state->left == 0 ); // You're probablly missing a comma.
            Symbol* argSymbol = Parse_Arg(state);

            Assert ( Is_Symbol_Fn(argSymbol) );

            out = PF(Addr, (Symbol_Unit){ .fnCall = argSymbol });
            //PF(Symbol_Forward, (Symbol_Unit){ .fnCall = state->left, }, (Symbol_Unit){ .symbol = Wea_Str_Lit(fieldName), } );
        } break;

        case TOKEN_TAG_ASTERISK: {
            //Assert( state->left == 0 ); // You're probablly missing a comma.
            Symbol* argSymbol = Parse_Arg(state);

            Assert ( Is_Symbol_Fn(argSymbol) );

            out = PF(Deref, (Symbol_Unit){ .fnCall = argSymbol });
        } break;

        case TOKEN_TAG_INNER: {
            //Assert( state->left == 0 ); // You're probablly missing a comma.
            Symbol* argSymbol = Parse_Arg(state);

            // argSymbol->type != Type_All_Get(S("Fn_Call")) ||
            Assert( Is_Symbol_Fn(argSymbol) );
            out = PF(Symbol_Inner, (Symbol_Unit){ .fnCall = argSymbol, });
        } break;

        case TOKEN_TAG_OUTER: {
            //Assert( state->left == 0 ); // You're probablly missing a comma.
            Symbol* argSymbol = Parse_Arg(state);

            // argSymbol->type != Type_All_Get(S("Fn_Call")) ||
            Assert( Is_Symbol_Fn(argSymbol) );

            out = PF(Symbol_Outer, (Symbol_Unit){ .fnCall = argSymbol, });
        } break;

        case TOKEN_TAG_FWD: {
            String fieldName = Lexer_Advance_And_Check(state, TOKEN_TAG_NAME)->name;
            out = PF(Symbol_Forward, (Symbol_Unit){ .fnCall = state->left, }, (Symbol_Unit){ .symbol = Wea_Str_Lit(fieldName), } );
        } break;

        case TOKEN_TAG_BWD: {
            out = PF(Symbol_Backward, (Symbol_Unit){ .fnCall = state->left, } );
        } break;

        case TOKEN_TAG_SU_FIELD: { INVALID_PATH(""); }

        NO_DEFAULT
    }

    state->left = out;
}

Public Symbol* Parse_Symbol_L(Parser_State* state) {
    Parse_Symbol(state);
    return state->left;
}
