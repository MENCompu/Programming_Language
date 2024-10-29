#include <third_party.h>

#include <meta_utils.h>

#include <menc_libs.h>

#define BUILD_64
#include <file_format_utils.h>
#include <parse.h>

#include <file_system.h>
#include <file_system.c>

#include "c_parser.c"

#include <meta_utils.h>
#include <meta_utils.c>

#include "P:\\src\\types.h"

Private Global Memory_Arena gOutput;

#define Str_B8(boolean) ( ( boolean ) ? "true" : "false" )

Public CString Str_Lit(String str) {
    String fmt = S("S_Const(\"%s\")");
    Str_Null_Terminate(&str);

    String output = Str_Alloc(&gTempArena, fmt.count + str.count); // 20 for the string count.
    Str_From_Fmt(&output, (char*)fmt.E, str.E);

    return output.E;
}

Public String Wea_Type_Name(String name) {
    if ( Str_Equal(name, S("void*")) ) {
        return S("u8*");
    }

    return name;
}

#define Write(fmt, ...) \
    do { \
        Local_Str(buffer, KiB(4)); \
        Str_From_Fmt(&buffer, fmt, __VA_ARGS__); \
        (void)Str_Alloc_And_Copy(&gOutput, buffer, false); \
    } while (0)

Public String Meta_Range(String src, String mark, b8 isPreprocessed) {
    String scan;

    Local_Str(mark2, KiB(4));
    Meta_Mark_As_String(&mark2, mark, true, isPreprocessed);

    scan = Str_Consume(src, mark2, true);

    Meta_Mark_As_String(&mark2, mark, false, isPreprocessed);
    scan = Str_Begin(scan, Str_Find_Str_Forward(scan, mark2));
    Str_Null_Terminate(&scan);

    return scan;
}

Public void Save(String path, String mark, b8 isPreprocessed) {
    if ( File_Exists(path.E) ) {
        if ( !Mem_Equal_Zero_Type(&mark) ) {
            String fileContents;
            fileContents.E = File_Read_All(path, &gTempArena, &fileContents.count);

            Local_Str(mark2, KiB(4));

            Meta_Mark_As_String(&mark2, mark, true, isPreprocessed);
            String begin = Str_Begin(fileContents, Str_Find_Str_Forward(fileContents, mark2) + mark2.count);
            //Str_Null_Terminate(&begin);

            Meta_Mark_As_String(&mark2, mark, false, isPreprocessed);
            String end = Str_Skip_Begin(fileContents, Str_Find_Str_Forward(fileContents, mark2));
            //Str_Null_Terminate(&end);

            Memory_Arena arena = Arena_Create_From_Arena(&gTempArena, begin.count + gOutput.count + end.count);

            (void)Str_Alloc_And_Copy(&arena, begin, false);

            String output = { .E = gOutput.E, .count = gOutput.count };
            (void)Str_Alloc_And_Copy(&arena, output, false);
            (void)Str_Alloc_And_Copy(&arena, end, false);

            File_Replace_Contents(path, arena.E, (u32)arena.count);
        } else {
            (void)File_Replace_Contents(path, gOutput.E, (u32)gOutput.count);
        }
    } else {
        File_Create_With_Contents(path, gOutput.E, (u32)gOutput.count);
    }

    Arena_Clear(&gOutput);
}

// Creating type info.
Public Raw_Type* Raw_Type_Get(Array_Raw_Type types, String name) {
    foreach (Raw_Type, type, types) {
        if ( Str_Equal(type->name, name) ) {
            return type;
        }
    }

    return 0;
}

Public b8 Raw_Type_Exists(Array_Raw_Type types, String name) {
    return ( Raw_Type_Get(types, name) != 0 );
}

Private Global u32 anonNum = 0;

Public Raw_Type* Parse_Raw_Type_Def(C_Parser_State* state, Array_Raw_Type* types, C_Token* kind) {
    Raw_Type tempType = { 0 };

    if ( kind->tag == C_TOKEN_TAG_STRUCT || kind->tag == C_TOKEN_TAG_UNION ) {
        C_Token* nameOrDef = C_Lexer_Advance(state);

        if ( nameOrDef->tag == C_TOKEN_TAG_NAME ) { nameOrDef = C_Lexer_Advance(state); } // Ignore name if exists.

        if ( nameOrDef->tag == '{' ) {
            if ( kind->tag == C_TOKEN_TAG_STRUCT ) {
                tempType.tag = TAG_Bundle;
            } else if ( kind->tag == C_TOKEN_TAG_UNION ) {
                tempType.tag = TAG_Union;
            } NO_ELSE

            Array_Static_Create(tempType.fields);

            for (C_Token* token = C_Lexer_Advance(state); token->tag != '}'; token = C_Lexer_Advance(state)) {
                switch ( (u32)token->tag ) {
                    case C_TOKEN_TAG_STRUCT:
                    case C_TOKEN_TAG_UNION: {
                        Raw_Type* innerType = Parse_Raw_Type_Def(state, types, token);

                        Array_Push_C(tempType.fields, (Raw_Field){ .name = ANONIMUS_SYMBOl_NAME, .type = innerType->name });
                    } break;

                    case C_TOKEN_TAG_NAME: {
                        Assert( token->tag == C_TOKEN_TAG_NAME );
                        String fieldName = C_Lexer_Advance_And_Check(state, C_TOKEN_TAG_NAME)->name;
                        (void)C_Lexer_Advance_And_Check(state, ';');

                        Array_Push_C(tempType.fields, (Raw_Field){ .name = fieldName, .type = token->name });
                    } break;
                }
            }
        } else {
            return 0;
        }
    } else if ( kind->tag == C_TOKEN_TAG_ENUM ) {
        C_Token* nameOrDef = C_Lexer_Advance(state);

        if ( nameOrDef->tag == C_TOKEN_TAG_NAME ) { nameOrDef = C_Lexer_Advance(state); } // Ignore name if exists.

        if ( nameOrDef->tag == '{' ) {
            tempType.tag = TAG_Enum;
            Array_Static_Create(tempType.values);

            for (C_Token* token = C_Lexer_Advance(state); token->tag != '}';) {
                Assert( token->tag == C_TOKEN_TAG_NAME );

                String name = token->name;
                u32 value = 0;

                token = C_Lexer_Advance(state);
                if ( token->tag == '=' ) {
                    C_Token* literal = C_Lexer_Advance_And_Check(state, C_TOKEN_TAG_LITERAL);
                    value = (u32)*Poly(s64, &literal->lit);
                    token = C_Lexer_Advance(state);
                }

                if ( token->tag == ',' ) { token = C_Lexer_Advance(state); }

                Array_Push_C(tempType.values, (Raw_Enum_Value){ .name = name, .value = value });
            }
        }
    } else {
        tempType.tag = TAG_Raw_Data;
    }

    C_Token* nameOrEnd = C_Lexer_Advance(state);

    if ( nameOrEnd->tag == C_TOKEN_TAG_NAME ) {
        tempType.name = nameOrEnd->name;
        (void)C_Lexer_Advance_And_Check(state, ';');
    } else {
        tempType.name = Str_Alloc(&gTempArena, 256);
        Str_From_Fmt(&tempType.name, "anon_%u", anonNum++);
    }

    Raw_Type* type;
    if ( !Raw_Type_Exists(*types, tempType.name) ) {
        type = Array_Push_C(*types, tempType);
    } else {
        type = Raw_Type_Get(*types, tempType.name);
    }

    return type;
}

Public void Create_Type_Info(String path, String mark, b8 isPreprocessed, Array_Raw_Type* types) {
    String file;
    file.E = File_Read_All(path, &gTempArena, &file.count);

    String scan = Meta_Range(file, mark, isPreprocessed);

    C_Parser_State parser = C_Parser_Init(&gTempArena, file);
    for (scan = Str_Consume(scan, S("typedef"), false);
         !Mem_Equal_Zero_Type(&scan);
         scan = Str_Consume(scan = Str_Begin_Ptr(file, C_Parser_Scan(&parser)), S("typedef"), false)) {
        C_Parser_Reposition(&parser, scan);

        (void)C_Lexer_Advance_And_Check(&parser, C_TOKEN_TAG_TYPEDEF);

        C_Token* kind = C_Lexer_Advance(&parser);
        //if ( kind->tag != C_TOKEN_TAG_ENUM && kind->tag != C_TOKEN_TAG_UNION ) {
        Parse_Raw_Type_Def(&parser, types, kind);
        //}
    }
}

Public b8 Raw_Fn_Exists(Array_Raw_Fn fns, String name) {
    foreach (Raw_Fn, fn, fns) {
        if ( Str_Equal(fn->name, name) ) {
            return true;
        }
    }

    return false;
}

Public Array_Raw_Fn Create_Prim_Raw_Fns(String path, String mark, b8 isPreprocessed) {
    Array_Raw_Fn fns;
    Array_Create(fns, KiB(4), &gPermArena);

    String file;
    file.E = File_Read_All(path, &gTempArena, &file.count);

    String scan = Meta_Range(file, mark, isPreprocessed);

    C_Parser_State parser = C_Parser_Init(&gTempArena, file);

    String match = S("static");
    for (scan = Str_Consume(scan, match, true);
         !Mem_Equal_Zero_Type(&scan);
         scan = Str_Consume(scan = Str_Begin_Ptr(scan, C_Parser_Scan(&parser)), match, true)) {
        C_Parser_Reposition(&parser, scan);

        String retType = C_Lexer_Advance_And_Check(&parser, C_TOKEN_TAG_NAME)->name;
        String fnName  = C_Lexer_Advance_And_Check(&parser, C_TOKEN_TAG_NAME)->name;

        if ( Raw_Fn_Exists(fns, fnName) || C_Lexer_Advance(&parser)->tag != '(' ) { continue; }

        Raw_Fn* fn = Array_Push(fns);
        fn->retType = retType;
        fn->name = fnName;

        Array_Static_Create(fn->sharedList);

        C_Token* token = C_Lexer_Advance(&parser);

        if ( !Str_Equal(token->name, S("void")) ) {
            Loop {
                Raw_Shared shared = { .tag = { .dir = SHARED_TAG_IN, .isOptional = 0 }, };

                if ( token->tag == C_TOKEN_TAG_OUT ) {
                    shared.tag.dir = SHARED_TAG_OUT;
                    token = C_Lexer_Advance_And_Check(&parser, C_TOKEN_TAG_NAME);
                } else if ( token->tag == C_TOKEN_TAG_OPT ) {
                    shared.tag.isOptional = true;
                    shared.optSymbolName = token->optSymbolName;
                    token = C_Lexer_Advance_And_Check(&parser, C_TOKEN_TAG_NAME);
                } else if ( token->tag == C_TOKEN_TAG_ELLIPSIS ) {
                    C_Lexer_Advance_And_Check(&parser, ')');
                    break;
                } else {
                    Assert( token->tag == C_TOKEN_TAG_NAME );
                }

                shared.type = token->name,
                shared.name = C_Lexer_Advance_And_Check(&parser, C_TOKEN_TAG_NAME)->name,

                DArray_Push_C(&gPermArena, &fn->sharedList, shared);

                C_Token* commaOrEnd = C_Lexer_Advance(&parser);
                if ( commaOrEnd->tag == ')' ) { break; }

                Assert( commaOrEnd->tag == ',' );

                token = C_Lexer_Advance(&parser);
            }
        }
    }

    return fns;
}

Public void Save_C_Introspection(Array_Raw_Type types, Array_Raw_Fn fns) {
    /* Raw_Type_Name macro definition */ {
        Write("// Type Name\n");
        Write("#define Type_Name(expr) S(_Generic(expr");

        foreach (Raw_Type, type, types) {
            if ( !Str_Equal(Str_Begin(type->name, S("anon").count), S("anon")) ) {
                Write(", \\\n    %s: \"%s\"", type->name.E, type->name.E);
            }
        }

        Write(" \\\n))\n\n");
    }

    /* Type data introspection. */ {
        Write("// c type info.\n");
        Write("static Raw_Type cTypeInfoBuffer[] = {\n");

        foreach (Raw_Type, type, types) {
            String tag = { 0 };
            switch ( type->tag ) {
                case TAG_Raw_Data: { tag = S("TAG_Raw_Data"); } break;
                case TAG_Bundle:   { tag = S("TAG_Bundle");   } break;
                case TAG_Union:    { tag = S("TAG_Union");    } break;
                case TAG_Enum:     { tag = S("TAG_Enum");     } break;

                case TAG_Array: case TAG_WArray: case TAG_Signature: case TAG_Integer: case TAG_Pointer:
                case TAG_Null:
                NO_DEFAULT
            }

            Write("    { .tag = %s, .name = %s,", tag.E, Str_Lit(type->name));

            if ( type->tag == TAG_Bundle || type->tag == TAG_Union ) {
                if ( type->fields.count != 0 ) {
                    Write(".fields = { .capacity = 256, .count = %d, .E = {\n", type->fields.count);

                    foreach (Raw_Field, field, type->fields) {
                        Write("        { .name = %s, .type = %s },\n", Str_Lit(field->name), Str_Lit(Wea_Type_Name(field->type)));
                    }

                    Write("    } }");
                }
            } else if ( type->tag == TAG_Enum ) {
                if ( type->values.count != 0 ) {
                    Write(".values = { .capacity = 256, .count = %d, .E = {\n", type->values.count);

                    u32 counter = 0;
                    foreach (Raw_Enum_Value, enumValue, type->values) {
                        if ( enumValue->value != 0 ) { counter = enumValue->value; }
                        Write("        { .name = %s, .value = %u },\n", Str_Lit(enumValue->name), counter);
                        ++counter;
                    }

                    Write("    } }");
                }
            }

            Write(" },\n");
        }

        Write("};\n\n");
        Write("static Slice_Raw_Type cTypeInfo;\n\n");
    }

    /* create prim fns */ {
        Write("// c fn info.\n");
        Write("static Raw_Fn gFnInfoBuffer[] = {\n");

        foreach (Raw_Fn, fn, fns) {
            Write("    { .name = %s, .retType = %s, .sharedList = { .capacity = 256, .count = %u",
                  Str_Lit(fn->name), Str_Lit(Wea_Type_Name(fn->retType)), fn->sharedList.count);

            if ( fn->sharedList.count != 0 ) {
                Write(", .E = {\n");

                foreach (Raw_Shared, shared, fn->sharedList) {
                    String tagDir = { 0 };
                    switch ( shared->tag.dir ) {
                        case SHARED_TAG_IN:  { tagDir = S("SHARED_TAG_IN"); } break;
                        case SHARED_TAG_OUT: { tagDir = S("SHARED_TAG_OUT"); } break;
                        NO_DEFAULT
                    }

                    Write("        { .tag = { .dir = %s, .isOptional = %s }, .name = %s, .type = %s",
                          tagDir.E, Str_B8(shared->tag.isOptional), Str_Lit(shared->name), Str_Lit(Wea_Type_Name(shared->type)));

                    if ( shared->tag.isOptional ) {
                        Write(", .optSymbolName = %s", Str_Lit(shared->optSymbolName));
                    }

                    Write(" },\n");
                }

                Write("    }");
            }

            Write(" } },\n");
        }

        Write("};\n\n");
        Write("static Slice_Raw_Fn gFnsInfo;\n\n");

        //Save(S("P:\\src\\ast.h"), S("Prim_Raw_Fn_Info"), false);
    }

    Save(S("P:\\src\\introspection.h"), (String){ 0 }, true);

    /* interpreter */ {
        Write("// interpreter.\n");
        Write("Public void Interpret_C(String __fnName, Symbol* __args) {\n");
        Write("    switch ( Get_Fn_Index(__fnName) ) {\n");

        forrange(index, fns.count) {
            Raw_Fn* fn = &fns.E[index];

            Write("        case %u: {", index);

            u32 weaSharedIndex = 0;

            if ( !Str_Equal(fn->retType, S("void")) ) {
                Write(" _Out(%s);", Wea_Type_Name(fn->retType).E);
                ++weaSharedIndex;
            }

            foreach (Raw_Shared, shared, fn->sharedList) {
                String type = Wea_Type_Name(shared->type);

                if ( Str_Equal(type, S("Variadic")) ) {
                    Write(" _Var(%s, %u);", shared->name.E, weaSharedIndex);
                    break;
                } else if ( Str_Equal(type, S("Any_Type")) ) {
                    Write(" _Any(%s);", shared->name.E);
                } else {
                    Write(" _Arg(%s,%s);", type.E, shared->name.E);
                }

                ++weaSharedIndex;
            }

            Write("\n            ");

            /* Wea symbols to c variables. */ {
                if ( !Str_Equal(fn->retType, S("void")) ) {
                    Write("*c_out = ");
                }

                Write("%s(", fn->name.E);

                forrange(sharedIndex, fn->sharedList.count) {
                    Raw_Shared* shared = &fn->sharedList.E[sharedIndex];

                    if ( sharedIndex != 0 ) { Write(", "); }
                    Write("%s", shared->name.E);
                }
            }

            Write(");\n");
            Write("        } break;\n");
        }

        Write("\n");
        Write("        NO_DEFAULT\n");
        Write("    }\n");
        Write("}\n");
        Write("\n");

        //Save(S("P:\\src\\interpreter.c"), S("Prim_Raw_Fn_Exec"), false);
    }

    Save(S("P:\\src\\c_interpreter.c"), (String){ 0 }, false);

    Arena_Clear(&gTempArena);
}

int main(int argc, char* argv[]) {
    Mem_Init();

    gPermArena = Arena_Create_From_Arena(&dyMem.arena, MiB(64));
    gTempArena = Arena_Create_From_Arena(&dyMem.arena, MiB(64));

    gOutput = Arena_Create_From_Arena(&gPermArena, MiB(4));

    Array_Raw_Type types;
    Array_Create(types, KiB(2), &gTempArena);
    Create_Type_Info(S("P:\\src\\pre_lang.c"), S("My_Code"), true, &types);

    Array_Raw_Fn fns = Create_Prim_Raw_Fns(S("P:\\src\\pre_lang.c"), S("Prim_Fn"), true);

    Save_C_Introspection(types, fns);

    return 0;
}

#if 0
Meta_Mark(Prim_Raw_Fn,Begin)

Public Symbol* Get_Sym(String name) {
    return Symbol_Root_Get(name);
}

Public void Type_Def(String name) {
    INVALID_PATH("");
}

Meta_Mark(Prim_Raw_Fn,End)

Meta_Mark(Prim_Raw_Fn_Info,Begin)

Meta_Mark(Prim_Raw_Fn_Info,End)

Public u32 Get_Raw_Fn_Index(fnCall->fn->name) {
    forrange (index, gRaw_FnsInfo.count) {
        Raw_Fn_Info_C* fnInfo = &gRaw_FnsInfo.E[index];
        if ( Str_Equal(fnInfo->name, name) ) {
            return index;
        }
    }

    LogFatal("Failed to find a c function with the given name");
    return 0;
}

Public void Symbol_Raw_Fn_Prim_Init(void) {
    foreach (Raw_Fn_Info_C, fnInfo, gRaw_FnsInfo) {
        Local_Str(sigName, KiB(4));
        Str_From_Fmt(&sigName, "%s_Sig", fnInfo->name.E);

        Type* sigType = Type_Create((Type){
            .name = Str_Alloc_And_Copy(&gPermArena, sigName, true),
            .tag  = TAG_Signature,
            .sig  = {
                .type = Type_Get(Str_Skip_End(name, 1)),
            },
        });
        Signature* sig = Poly(Signature, sigType);

        u32 count = fnInfo->args.count;
        if ( !Str_Equal(fnInfo->retType, S("void")) ) { ++count; }

        Array_Create(sig->shared, count, &gPermArena);

        foreach (C_Arg_Info_C, arg, fnInfo->args) {
            Array_Push(sig->shared, (Shared){ .tag = SHARED_TAG_IN, .name = arg->name, .type = Type_Get(arg->type) });
        }

        if ( !Str_Equal(fnInfo->retType, S("void")) ) {
            Array_Push(sig->shared, (Shared){ .tag = SHARED_TAG_OUT, .name = fnInfo->retType.name, .name = Type_Get(fnInfo->retType.type), });
        }

        (void)Symbol_Root_Create_And_Init(fnInfo->name, &(Raw_Fn){ .name = fnInfo->name, .sig = sigType, .code = { 0 } }, Type_Get(S("Raw_Fn")));
    }
}

    Lit((Variadic){ .capacity = ( args.count - 0 - 1 ), .count = ( args.count - 0 - 1 ), .E = &args.E[0], }, args.E[args.count - 1].symbol);
#endif

#if 0
#if 1
typedef struct {
    String name;
    String type;
} Raw_Field;
Array(Raw_Field);
#endif

typedef enum {
    TYPE_TAG_PRIMITIVE,
    TYPE_TAG_STRUCT,
    TYPE_TAG_UNION,
} Raw_Type_Tag;

typedef struct {
    String name;

    Raw_Type_Tag tag;
    Array_Raw_Field fields;
} Raw_Type;
Array(Raw_Type);
#endif

