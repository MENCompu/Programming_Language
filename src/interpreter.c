#if 0
typedef struct {
    String name;
    u32 id;
} Some_Type;

typedef struct {
    String name;
    u32 id;
} Other_Different_Type;

Public void Some_Fn(Type hola, u32 number) {
    Other_Different_Type a;
    Some_Type b;

    a = b;

    a.name = hola.name;
    a.id = number;
}
#endif

Public Symbol* Get_Out_Symbol(Array_Shared sharedList, Symbol* args) {
    forrange (index, sharedList.count) {
        Shared* shared = &sharedList.E[index];
        Symbol_Unit* arg = &args->childs.E[index];

        if ( shared->tag.dir == SHARED_TAG_OUT ) {
            return arg->symbol;
        }
    }

    return 0;
}

Public Symbol* Get_Out_Symbol_Fn_Call(Fn_Call* fnCall) {
    return Get_Out_Symbol(Poly(Signature, fnCall->fn->sig)->shared, fnCall->args);
}

#if 0
Public Shared* Shared_By_Name(Array_Shared sharedList, String name) {
    foreach (Shared, shared, sharedList) {
        if ( Str_Equal(shared->name, name) ) {
            return shared;
        }
    }

    LogFatal("Failed to find the shared object by name");
    return 0;
}

Public Symbol* Get_Arg_Symbol(Symbol_Unit* arg) {
    if ( arg->symbol ) { return arg->symbol; }

    Assert( arg->symbolFnCall );

    if ( !D(Fn_Call, arg->symbolFnCall)->isResolved ) {
        Fn_Call_Resolve(arg->symbolFnCall, &(Symbol_Unit){ .symbol = Symbol_Scope_Create((String){ 0 }, Type_Get(S("Symbol*"))) });
    }

    return *D(Symbol*, Interpret_Fn_Call(arg->symbolFnCall));
}
#endif

#define Out_Shared(sharedList) \
    &(sharedList).E[0]; \
    Assert( (sharedList).E[0].tag.dir == SHARED_TAG_OUT );

Public b8 Fn_Call_Fn_Resolve(Symbol* wFnCall) {
    Fn_Call* fnCall = *D(Fn_Call*, wFnCall);

    if ( fnCall->fn ) { return true; }

    Assert( fnCall->fnFnCall );

    if ( !(*D(Fn_Call*, fnCall->fnFnCall))->isResolved ) {
        Fn_Call_Resolve(fnCall->fnFnCall, &(Symbol_Unit){ .symbol = Symbol_Scope_Create((String){ 0 }, Type_Get(S("Symbol*"))) });
    }

    Symbol* out = *D(Symbol*, Interpret_Fn_Call(fnCall->fnFnCall));

    if ( out ) {
        fnCall->fn = D(Fn, out);
        return true;
    } else {
        return false;
    }
}

/* TODO(JENH): Try to standarize the fn call resolving with general type instance resolving.
 * Type var;
 * var.field1 = 2;
 * var.field2 = "name";
 * var.field3 = false;
 *
 * then ...
 *
 * bundle {
 *     u32 age;
 *     String name;
 *     b8 isGay;
 * } Type;
*/

Public void Fn_Call_Resolve(Symbol* wFnCall, Symbol_Unit* outArg) {
    if ( !Fn_Call_Fn_Resolve(wFnCall) ) { return; }
    Fn_Call* fnCall = *D(Fn_Call*, wFnCall);

    Array_Shared sharedList = Poly(Signature, fnCall->fn->sig)->shared;

    // TODO(JENH): This is assuming that there is only one output.
    if ( outArg && ( fnCall->args->childs.E[0].symbol != outArg->symbol ) ) {
        DArray_Push_Front_C(&gSymbolState.symbolUnitArena, &fnCall->args->childs, (Symbol_Unit){
            .symbol = outArg->symbol,
        });
    }

    Assert( Has_Optional(sharedList) || Has_Variadic(sharedList) || ( fnCall->args->childs.count == sharedList.count ) );

#if 0
    Local_Array(Shared, tempSharedList, KiB(1));

    if ( Str_Equal(fnCall->fn->name, S("Lit")) ) {
        Symbol* out = Get_Out_Symbol(sharedList, fnCall->args);
        Shared* outShared = Out_Shared(sharedList);

        sharedList = tempSharedList;

        Array_Push_C(sharedList, (Shared){ .type = out->type, .name = outShared->name });

        if ( out->type->tag == TAG_Array || out->type->tag == TAG_WArray ) {
            Type* elemType = ( out->type->tag == TAG_Array ) ? Poly(Array, out->type)->elemType : Poly(WArray, out->type)->elemType;
            forrange (dummy, fnCall->args->childs.count - 1) {
                Array_Push_C(sharedList, (Shared){ .type = elemType, .name = (String){ 0 } });
            }
        } else if ( out->type->tag == TAG_Bundle ) {
            Array_Field fields = Poly(Bundle, out->type)->fields;
            foreach (Field, field, fields) {
                Array_Push_C(sharedList, (Shared){ .type = field->type, .name = field->name });
            }
        } NO_ELSE
    } else if ( Str_Equal(fnCall->fn->name, S("Let")) ) {
        Array_Copy_All(tempSharedList, sharedList);
        sharedList = tempSharedList;

        Symbol_Unit* argType = &fnCall->args->childs.E[0];

        String typeName = { 0 };

        if ( argType->fnCall ) {
            typeName = *D(String, Interpret((Code){ .wFnCall = argType->fnCall, }, argType));
        } else if ( argType->symbol ) {
            typeName = *D(String, argType->symbol);
        }

        Shared_By_Name(sharedList, S("init"))->type = Type_Get(typeName);
    } else if ( Str_Equal(fnCall->fn->name, S("WArray_Push")) ) {
        Array_Copy_All(tempSharedList, sharedList);
        sharedList = tempSharedList;

        Symbol* wArray = Get_Arg_Symbol(&fnCall->args->childs.E[0]);
        Assert( wArray->type->tag == TAG_WArray );

        Shared_By_Name(sharedList, S("wArray"))->type = wArray->type;
        Shared_By_Name(sharedList, S("elem"))->type   = Poly(WArray, wArray->type)->elemType;
    } else if ( Str_Equal(fnCall->fn->name, S("WArray_Index")) ) {
        Array_Copy_All(tempSharedList, sharedList);
        sharedList = tempSharedList;

        Symbol* wArray = Get_Arg_Symbol(&fnCall->args->childs.E[1]);
        Assert( wArray->type->tag == TAG_WArray );

        Shared_By_Name(sharedList, S("wArray"))->type = wArray->type;
        Shared_By_Name(sharedList, S("out"))->type    = Poly(WArray, wArray->type)->elemType;
    }
#endif

    forrange (index, sharedList.count) {
        Shared* shared = &sharedList.E[index];

        if ( shared->type == Type_All_Get(S("Variadic")) ) {
            for (; index < fnCall->args->childs.count; ++index) {
                Symbol_Unit* arg = &fnCall->args->childs.E[index];

                if ( !arg->symbol ) {
                    arg->symbol = Symbol_Create(0, (String){ 0 }, Type_Get(S("Any_Type")));
                }

                arg->name = (String){ 0 };
            }
        } else if ( shared->tag.isOptional && ( index == fnCall->args->childs.count ) ) {
            Symbol_Unit* arg = DArray_Push_C(&gSymbolState.symbolUnitArena, &fnCall->args->childs, (Symbol_Unit){ .name = shared->name, });

            arg->symbolFnCall = F(Get_Sym, (Symbol_Unit){ .symbol = Wea_Str_Lit(shared->optSymbolName) });
        } else {
            Symbol_Unit* arg = &fnCall->args->childs.E[index];

            if ( !arg->symbol ) {
                arg->symbol = Symbol_Create(0, (String){ 0 }, shared->type);
            }

            if ( arg->symbol->type == Type_Get(S("Any_Type")) && shared->type != Type_Get(S("Any_Type")) ) {
                Symbol_Set(arg->symbol, shared->type, true, 0);
            }

            arg->name = shared->name;
        }
    }

    fnCall->isResolved = true;
}

Public void Fn_Call_Resolve_Rec(Symbol* wFnCall, Symbol_Unit* outArg) {
    Fn_Call_Resolve(wFnCall, outArg);

    Fn_Call* fnCall = *D(Fn_Call*, wFnCall);

    foreach (Symbol_Unit, arg, fnCall->args->childs) {
        if ( arg->fnCall && !(*D(Fn_Call*, arg->fnCall))->isResolved ) {
            Fn_Call_Resolve_Rec(arg->fnCall, arg);
        } else if ( arg->symbolFnCall && !(*D(Fn_Call*, arg->symbolFnCall))->isResolved ) {
            Fn_Call_Resolve_Rec(arg->symbolFnCall, &(Symbol_Unit){ .symbol = Symbol_Scope_Create((String){ 0 }, Type_Get(S("Symbol*"))), });
        } else if ( arg->symbol->type == Type_All_Get(S("Fn_Call*")) && !(*D(Fn_Call*, arg->symbol))->isResolved ) {
            Fn_Call_Resolve_Rec(Get_Fn_Call_Symbol(*D(Fn_Call*, arg->symbol)), 0);
        }
    }
}

#define INTERPRET_FAILED (void*)0xffffffffffffffff

Public void Interpret_(Code* code) {
    Array_Fn_Call_P fnCalls = code->codeBlock->code;

    for (code->pc = 0; code->pc < fnCalls.count; ++code->pc) {
        Interpret_Fn_Call(Get_Fn_Call_Symbol(fnCalls.E[code->pc]));
    }
}

Public Symbol* Interpret(Code inCode, Symbol_Unit* outArg) {
    Symbol* out = 0;

    if ( inCode.wFnCall ) {
        Fn_Call* fnCall = *D(Fn_Call*, inCode.wFnCall);

        if ( !fnCall->isResolved ) {
            Fn_Call_Resolve(inCode.wFnCall, outArg);

            if ( !fnCall->fn ) { return INTERPRET_FAILED; }
        }

        Assert( fnCall->fn );

        inCode.codeBlock = &fnCall->fn->codeBlock;
    }

    Code* code = Array_Push_C(gInterState.codeStack, inCode);
    b8 isScopeTemp = false;

    if ( !code->codeBlock->locals ) {
        code->codeBlock->locals = Symbol_New_(0, (String){ 0 }, false);
        isScopeTemp = true;
    }

    if ( code->wFnCall ) {
        Fn_Call* fnCall = *D(Fn_Call*, code->wFnCall);

        foreach (Symbol_Unit, arg, fnCall->args->childs) {
            if ( arg->fnCall ) {
                Interpret((Code){ .wFnCall = arg->fnCall }, arg);
            } else if ( arg->symbolFnCall ) { // && !arg->symbol
                Symbol_Unit  temp = { 0 };
                Symbol_Unit* symbolFnCallOut = 0;

                if ( !(*D(Fn_Call*, arg->symbolFnCall))->isResolved ) {
                    temp = (Symbol_Unit){ .symbol = Symbol_Scope_Create((String){ 0 }, Type_Get(S("Symbol*"))), };
                    symbolFnCallOut = &temp;
                }

                arg->symbol = *D(Symbol*, Interpret((Code){ .wFnCall = arg->symbolFnCall }, symbolFnCallOut));
            } else if ( arg->symbol->type == Type_All_Get(S("Fn_Call*")) && !(*D(Fn_Call*, arg->symbol))->isResolved ) {
                Fn_Call_Resolve_Rec(Get_Fn_Call_Symbol(*D(Fn_Call*, arg->symbol)), 0);
            }
        }

        Array_Shared sharedList = Poly(Signature, fnCall->fn->sig)->shared;

        if ( !Has_Variadic(sharedList) && !Has_Optional(sharedList) ) { /* Type check fn call */
            Assert( fnCall->args->childs.count == sharedList.count );

            foreach (Shared, shared, sharedList) {
                if ( shared->type == Type_All_Get(S("Any_Type")) ) {
                    goto skip;
                }
            }

            forrange (index, fnCall->args->childs.count) {
                Shared* shared = &sharedList.E[index];
                Symbol_Unit* arg = &fnCall->args->childs.E[index];

                Assert( Type_Check(shared->type, arg->symbol->type) );
            }

            skip:;
        }

        if ( code->codeBlock->code.count != 0 ) {
            Array_Shared sharedList = Poly(Signature, fnCall->fn->sig)->shared;

            forrange (index, fnCall->args->childs.count) {
                Shared* shared = &sharedList.E[index];
                Symbol_Unit* arg = &fnCall->args->childs.E[index];

                Symbol_Push_Child_C(code->codeBlock->locals, shared->name, arg->symbol);
            }
        }

        out = Get_Out_Symbol_Fn_Call(fnCall);
    }

    if ( code->codeBlock->code.count != 0 ) {
        Interpret_(code);
    } else {
        Assert( code->wFnCall );
        Fn_Call* fnCall = *D(Fn_Call*, code->wFnCall);
        Interpret_C(fnCall->fn->name, fnCall->args);
    }

    Array_Pop(gInterState.codeStack);

    if ( isScopeTemp ) {
        code->codeBlock->locals = 0;
    }

    return out;
}

Public Symbol* Interpret_Code(Array_Fn_Call_P code) {
    return Interpret((Code){ .codeBlock = &(Code_Block){ .code = code } }, 0);
}

Public Symbol* Interpret_Code_Block(Code_Block* codeBlock) {
    return Interpret((Code){ .codeBlock = codeBlock }, 0);
}

Public Symbol* Interpret_Fn_Call(Symbol* wFnCall) {
    return Interpret((Code){ .wFnCall = wFnCall, }, 0);
}

Public void Interpret_Begin(void) {
    Code_Block* codeBlock = Arena_Alloc_Type(&gTempArena, Code_Block);
    *codeBlock = (Code_Block){ .locals = Symbol_New_(0, (String){ 0 }, false), };
    Array_Push_C(gInterState.codeStack, (Code){ .codeBlock = codeBlock, });
}

Public void Interpret_End(void) {
    Array_Pop(gInterState.codeStack);
}

#if 0
Public b8 Symbol_Is_Lit(Symbol* symbol) {
    foreach (Symbol_Unit, litUnit, gSymbolState.lits->childs) {
        if ( litUnit->symbol == symbol ) {
            return true;
        }
    }

    return false;
}

Public String Arg_To_Text(Symbol_Unit* arg) {
    Local_Str(text, KiB(1));

    if ( arg->fnCall ) {
        Str_Push(&text, Fn_Call_To_Text(arg->fnCall));
    } else if ( arg->symbolFnCall ) {
        Str_Push(&text, Fn_Call_To_Text(arg->symbolFnCall));
    } else if ( Symbol_Is_Lit(arg->symbol) ) {
        if ( Str_Equal(arg->symbol->type->name, S("String")) ) {
            Local_Str(strLit, KiB(4));
            Str_From_Fmt(&strLit, "\"%s\"", D(String, arg->symbol)->E);
            Str_Push(&text, strLit);
        } else if ( Str_Equal(arg->symbol->type->name, S("u32")) ) {
            Str_Push(&text, U32_To_Str(*D(u32, arg->symbol)));
        }
    } else if ( arg->isProducedByParser && arg->symbol->type == Type_All_Get(S("Fn_Call*"))  ) {
        Str_From_Fmt(&text, "'%s", Fn_Call_To_Text(*D(Fn_Call*, arg->symbol)).E);
    } else {
        Assert( arg->isProducedByParser );
        return (String){ 0 };
    }

    return text;
}

Public String Args_To_Text(Fn_Call* fnCall) {
    Local_Str(text, KiB(1));

    u32 index = 0;
    foreach (Symbol_Unit, arg, fnCall->args->childs) {
        if ( index != 0 ) { Str_Push_Char(&text, ' '); }

        Loop {
            String argText = Arg_To_Text(arg);
            if ( !Mem_Equal_Zero_Type(&argText) ) { break; }
            ++arg;
        }

        Str_Push(&text, Arg_To_Text(arg));
        ++index;
    }

    return text;
}

Public String Fn_Call_To_Text(Fn_Call* fnCall) {
    Local_Str(text, KiB(4));

    if ( fnCall->isProducedByParser ) {
        if ( Str_Equal(fnCall->fn->name, S("Get_Sym")) ) {
            //Str_Push(&text, *D(String, fnCall->args->childs.E[1].symbol);
            Str_Push(&text, *D(String, Symbol_Get(fnCall->args, S("name"))));
        } else if ( Str_Equal(fnCall->fn->name, S("Lit")) ) {
            if ( Symbol_Get(fnCall->args, S("out"))->type == Type_All_Get(S("Array_Fn_Call_P")) ) {
                Str_Push(&text, S("{\n"));

                u32 index = 0;
                foreach (Symbol_Unit, arg, fnCall->args->childs) {
                    if ( index != 0 ) { Str_Push_Char(&text, ' '); }

                    if ( arg->isProducedByParser && arg->symbol->type != Type_All_Get(S("Fn_Call*")) ) { arg++; }

                    Str_Push(&text, Fn_Call_To_Text(*D(Fn_Call*, arg->symbol)));
                    Str_Push(&text, S(";\n"));
                    ++index;
                }

                Str_Push_Char(&text, '}');
            } else {
                Str_Push_Char(&text, '[');
                Str_Push(&text, Args_To_Text(fnCall));
                Str_Push_Char(&text, ']');
            }
        } else if ( Str_Equal(fnCall->fn->name, S("Dot")) ) {
            Assert( Symbol_Unit_Get(fnCall->args, S("parent"))->fnCall );
            Assert( Symbol_Is_Lit(Symbol_Unit_Get(fnCall->args, S("name"))->symbol) );

            Str_Push(&text, Fn_Call_To_Text(Symbol_Unit_Get(fnCall->args, S("parent"))->fnCall));
            Str_Push_Char(&text, '.');
            Str_Push(&text, *D(String, Symbol_Get(fnCall->args, S("name"))));
        }
    } else {
        Assert( fnCall->fnFnCall );

        Str_Push_Char(&text, '(');
        Str_Push(&text, Fn_Call_To_Text(fnCall->fnFnCall));

        Str_Push_Char(&text, ' ');

        Str_Push(&text, Args_To_Text(fnCall));

        Str_Push_Char(&text, ')');
    }

    return text;
}

Public void Render_Text_Fn_Call(Fn_Call* fnCall) {
    printf("%s;", Fn_Call_To_Text(fnCall).E);
}
#endif

