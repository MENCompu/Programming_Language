Public Symbol* Symbol_Scope_Exists(String name, Symbol** parent) {
    Scope_Iter(scope) {
        Symbol* symbol = Symbol_Get_Unsafe(scope, name);

        if ( symbol ) {
            *parent = scope;
            return symbol;
        }
    }

    return 0;
}

Public void Let_(String typeName, String name, Any_Type init, Symbol* scope) {
    Type* type = Type_Get(typeName);

    if ( init->type == Type_All_Get(S("Any_Type")) ) {
        Assert( Are_Symbol_And_Type_Compatible(init, type) );
        init->type = type;
        Symbol_Resolve(init);
    }

    Assert( Type_Check(init->type, type) );

    /* Check if there is collision between symbol bases. */ {
        Symbol* parent = 0;
        Assert( Symbol_Scope_Exists(name, &parent) == 0 );
    }

    //Symbol* scope = ( gSymbolInit.using ) ? Scope_Top(0) : Scope_Top(1); // TODO(JENH): The "scope" should be passed as a parameter instead???.

    b8 isPerm = false;
    foreach (Symbol_Unit, moduleUnit, gSymbolState.root->childs) {
        if ( moduleUnit->symbol == scope ) {
            isPerm = true;
            break;
        }
    }

    if ( type->tag != TAG_WArray ) {
        (void)Symbol_Create_And_Init_(scope, name, isPerm, type, true, Symbol_Data_Get(init));
    } else {
        (void)Symbol_Set(Symbol_New_(scope, name, isPerm), type, false, 0);
    }
}

Public void Let(String typeName, String name, /*OPT NULL*/ Any_Type init) {
    Let_(typeName, name, init, Scope_Top(1));
}

Public void Let_C(String typeName, String name, Symbol* scope) {
    Let_(typeName, name, Symbol_Get(Symbol_Core(), S("NULL")), scope);
}

Public void View(String name, Symbol* symbol) {
    Symbol_Push_Child_C(Scope_Top(1), name, symbol);
}

Public void If(b8 boolean, Array_Fn_Call_P thenBlock, /*OPT NULL*/ Array_Fn_Call_P elseBlock) {
    if ( boolean ) {
        Interpret_Code(thenBlock);
    } else if ( elseBlock.count != 0 ) {
        Interpret_Code(elseBlock);
    }
}

Public b8 Not(b8 boolean) {
    return !boolean;
}

Public void While(Fn_Call* condition, Array_Fn_Call_P body) {
    if ( !condition->isResolved ) {
        Fn_Call_Resolve(Get_Fn_Call_Symbol(condition), &(Symbol_Unit){ .symbol = Symbol_Scope_Create((String){ 0 }, Type_Get(S("b8"))) });
    }

    Loop {
        if ( !( *D(b8, Interpret_Fn_Call(Get_Fn_Call_Symbol(condition))) ) ) { break; }
        Interpret_Code(body);
    }
}

typedef struct {
    b8 shouldContinue;
    Array_u8 array;
    u8* scan;
    Symbol* iter;
} Foreach_State;

Public void Foreach_Begin_(String iterName, Any_Type weaArray, Foreach_State* state) {
    Assert( weaArray->type->tag == TAG_Array );

    state->array = *(Array_u8*)Symbol_Data_Get(weaArray);

    Interpret_Begin();

    Let_C(Poly(Array, weaArray->type)->elemType->name, iterName, Scope_Top(0));
    state->iter = Get_Sym(iterName);

    state->scan = state->array.E;

    Symbol_Set_Mem(state->iter, state->scan);
    state->shouldContinue = ( state->scan < &state->array.E[state->array.count * Type_Size(state->iter->type)] );
}

Public void Foreach_Update(Foreach_State* state) {
    state->scan += Type_Size(state->iter->type);
    Symbol_Set_Mem(state->iter, state->scan);
    state->shouldContinue = ( state->scan < &state->array.E[state->array.count * Type_Size(state->iter->type)] );
}

#define Foreach_Begin(iter, iterName, weaArray) \
    Foreach_State P(state, __LINE__) = { 0 }; \
    Foreach_Begin_(iterName, weaArray, &P(state, __LINE__)); \
    Symbol* iter = P(state, __LINE__).iter; \
    goto P(foreach_begin_, __LINE__); \
    P(foreach_update_, __LINE__): \
    Foreach_Update(&P(state, __LINE__)); \
    P(foreach_begin_, __LINE__): \
    if ( !( (P(state, __LINE__)).shouldContinue ) ) { goto P(foreach_end_, __LINE__); }

#define Foreach_End() \
    goto P(foreach_update_, __LINE__); \
    P(foreach_end_, __LINE__): \
    Interpret_End();

Public void Foreach(String iterName, Any_Type array, Array_Fn_Call_P body) {
    //INVALID_PATH("This doesn't work currently, because we create a new scope in \"Foreach_Begin\" and \"Interpret_Code_Block\"");

    if ( array->type->tag == TAG_Array ) {
#if 0
        Foreach_Begin(iter, iterName, array);
            Interpret_Code_Block((Code_Block){ .code = body, });
        Foreach_End();
#endif
    } else if ( array->type->tag == TAG_WArray ) {
        (void)Interpret_Begin();

        Let_C(Poly(WArray, array->type)->elemType->name, iterName, Scope_Top(1));
        Symbol_Unit* unit = Symbol_Push_Child_C(Scope_Top(0), .name = iterName);

        foreach (Symbol_Unit, iter, array->childs) {
            unit->symbol = iter->symbol;

            Interpret_Code(body);
        }

        Interpret_End();
    } NO_ELSE
}

Public void Cast(/*OUT*/ Any_Type out, String typeName, Any_Type src) {
    INVALID_PATH("");
    Type* type = Type_Get(typeName);

    if ( ( src->type->tag == TAG_Pointer || src->type->tag == TAG_Integer ) &&
         ( type->tag == TAG_Pointer      || type->tag == TAG_Integer ) ) {
        Symbol_Set(out, type, true, 0);
        Mem_Copy_Forward(Symbol_Data_Get(out), Symbol_Data_Get(src), Min(Type_Size(type), Type_Size(src->type)));
        return;
    }
}

Public void WArray_Push(Any_Type wArray, Any_Type elem) {
    Type* elemType = Poly(WArray, wArray->type)->elemType;

    Assert( elemType == elem->type );

    Symbol_Create_And_Init_(wArray, (String){ 0 }, true, elemType, true, Symbol_Data_Get(elem));
}

Public void WArray_Index(/*OUT*/ Any_Type out, Any_Type wArray, u32 index) {
    Mem_Copy_Forward(Symbol_Data_Get(out), Symbol_Data_Get(wArray->childs.E[index].symbol), Type_Size(Poly(WArray, wArray->type)->elemType));
}

#if 0
Public void WArray_Push(Any_Type wArray, Any_Type elem) {
    Assert( wArray->type->tag == TAG_WArray );
    Assert( Poly(WArray, wArray->type)->elemType == elem->type );

    Type* elemType = Poly(WArray, wArray->type)->elemType;
    u32 elemSize = Type_Size(elemType);

    if ( wArray->data == 0 ) {
        array->capacity = 1;
        array->E = Arena_Alloc_Mem(&gSymbolState.dataArena, elemSize);
    }

    if ( array->count == array->capacity ) {
        Array_void copy = *array;

        array->capacity *= 2;
        array->E = Arena_Alloc_Mem(arena, elemSize * array->capacity);
        Mem_Copy_Forward(array->E, copy.E, elemSize * array->count);

        Mem_Zero(copy.E, elemSize * copy.capacity );

        forrange (index, wArray->childs.count) {
            Symbol_Set_Mem(wArray->childs.E[index]->symbol, &array->E[elemSize * index]);
        }
    }

    //Symbol* symbol =
    (void)Symbol_Create_And_Init_(wArray, (String){ 0 }, true, elemType, false, &array->E[elemSize * array->count]);
}
#endif

Public String Type_Get_Name(Type* type) {
    return type->name;
}

Public u32 Mul(u32 a, u32 b) {
    return a * b;
}

Public u32 Add(u32 a, u32 b) {
    return a + b;
}

Public u32 Sub(u32 a, u32 b) {
    return a - b;
}

Public b8 Eq(Any_Type a, Any_Type b) {
    //Assert( ( a->type->tag == TAG_Integer || a->type->tag == TAG_Enum ) &&
    //        ( b->type->tag == TAG_Integer || b->type->tag == TAG_Enum ) );

    return Mem_Equal(Symbol_Data_Get(a), Symbol_Data_Get(b), Type_Size(a->type));
}

Public b8 Less_Than(u32 a, u32 b) {
    return ( a < b );
}

Public void Return(void) {
    Code* code = &Array_Index_Back(gInterState.codeStack, 0);
    code->pc = code->codeBlock->code.count;
}

Public void Import(String path, String alias, b8 using) {
    Wea_File_Info fileInfo = Parse_Wea_File_Info(path);
    Symbol* moduleLocals;

    if ( Symbol_Exists(gSymbolState.root, fileInfo.name) ) {
        moduleLocals = D(Fn, Symbol_Get(gSymbolState.root, fileInfo.name))->codeBlock.locals;
    } else {
        // TODO(JENH): When creating the module lazily you should clean the interpreter state.
        moduleLocals = Module_Create_By_File(path)->codeBlock.locals;
    }

    (void)Symbol_Push_Child_C(Module_Top(), Str_Alloc_And_Copy(&gSymbolState.stringArena, alias, true), moduleLocals, using);
}

Public String Input(void) {
    Local_Str(localOut, KiB(4));

    scanf_s("%[^\n]", localOut.E);
    localOut.count = CStr_Len(localOut.E);

    char c;
    while ( ( c = getchar() ) != '\n' );

    return Str_Alloc_And_Copy(&gPermArena, localOut, true);
}

// String text
Public void Output(Variadic args) {
    foreach (Symbol*, argRef, args) {
        Symbol* arg = *argRef;

        if ( 0 ) {
        } else if ( Str_Equal(arg->type->name, S("u8")) ) {
            printf("%hhu", *D(u8, arg));
        } else if ( Str_Equal(arg->type->name, S("u16")) ) {
            printf("%hu", *D(u16, arg));
        } else if ( Str_Equal(arg->type->name, S("u32")) ) {
            printf("%u", *D(u32, arg));
        } else if ( Str_Equal(arg->type->name, S("u64")) ) {
            printf("%llu", *D(u64, arg));
        } else if ( Str_Equal(arg->type->name, S("String")) ) {
            String text = *D(String, arg);
            printf("%.*s", text.count, text.E);
        } else if ( Str_Equal(arg->type->name, S("Symbol")) ) {
            printf("soy un symbolo mamaguebo");
        } else if ( arg->type->tag == TAG_Pointer ) {
            printf("0x%016llx", *(u64*)Symbol_Data_Get(arg));
        } else if ( arg->type->tag == TAG_Enum ) {
            u32 value = *(u32*)Symbol_Data_Get(arg);
            foreach (Symbol_Unit, unit, Symbol_Enum_Values(arg->type)->childs) {
                if ( *D(u32, unit->symbol) == value ) {
                    printf("%s", unit->name.E);
                    return;
                }
            }

            LogFatal("The value is not type correct");
        }
    }
}

Public String Str_From_U32(u32 value) {
#if 0
    Local_Str(temp, KiB(4));
    U32_To_Str(value, &temp);
    return Str_Alloc_And_Copy(&gPermArena, temp, true);
#endif
    return (String){ 0 };
}

Public String WStr_From_Fmt(String fmt, Variadic args) {
    /* Check args count match with the fmt string. */ {
        u32 placeholderCount = 0;
        foreach (u8, scan, fmt) {
            if ( *scan == '%' ) { ++placeholderCount; }
        }
        Assert( placeholderCount == args.count );
    }

    String out = Str_Alloc(&gSymbolState.stringArena, KiB(4));
    Symbol* arg = args.E[0];

    foreach (u8, scan, fmt) {
        if ( *scan == '%' ) {
            if ( 0 ) {
            } else if ( arg->type->tag == TAG_Integer ) {
                u32 integer = *(u32*)Symbol_Data_Get(arg);
                Str_Push(&out, U32_To_Str(integer));
            } else if ( Str_Equal(arg->type->name, S("String")) ) {
                Str_Push(&out, *D(String, arg));
            } NO_ELSE
            //} else if ( arg->type->tag == TAG_Pointer ) {

            ++arg;
        } else {
            Str_Push_Char(&out, *scan);
        }
    }

    return out;
}

Public void Using(String name) {
    Scope_Iter (scope) {
        Symbol_Unit* unit = Symbol_Unit_Get_Unsafe(scope, name);

        if ( unit ) {
            unit->using = true;
            return;
        }
    }
}

Public void Transfer(/*OUT*/ Any_Type dst, Any_Type src) {
    if ( !( dst->type->tag == TAG_Pointer && src->type->tag == TAG_Integer ) ) {
        Assert( Type_Check(dst->type, src->type) );
    }

    if ( dst->type->tag == TAG_Enum ) {
        Assert( Exists_Enum_Val(Poly(Enum, dst->type), src) );
    }

    Mem_Copy_Forward(Symbol_Data_Get(dst), Symbol_Data_Get(src), Min(Type_Size(dst->type), Type_Size(src->type)));
}

#if 0
Public u32 Symbol_Size(Symbol* symbol) {
    u32 size;
    foreach (Symbol_Unit, unit, symbol->childs) {
        if ( unit->symbol->type == Type_All_Get(S("Any_Type")) ) {
            Symbol_Size();
        }
        size += unit
    }
    return size;
}
#endif

Public void Lit(/*OUT*/ Any_Type out, Variadic args) {
    if ( 0 ) {
    } else if ( out->type == Type_All_Get(S("Any_Type")) ) {
#if 0
        u32 size = 0;
        foreach (Symbol_Unit, arg, args) {
            Assert( arg->type );
            size +=
        }
#endif

        foreach (Symbol*, argRef, args) {
            Symbol* arg = *argRef;
            Symbol* child = Symbol_Create_And_Init(out, (String){ 0 }, arg->type, true, Symbol_Data_Get(arg));

            if ( !out->data ) {
                out->data = child->data;
            }
        }
    } else if ( out->type->tag == TAG_Array ) {
        Array_u8* array = (Array_u8*)Symbol_Data_Get(out);
        Type* elemType = Poly(Array, out->type)->elemType;
        u32 elemSize = Type_Size(elemType);

        *array = (Array_u8){
            .capacity = args.count,
            .count = args.count,
            .E = Arena_Alloc_Mem(&gSymbolState.dataArena, elemSize * args.count),
        };

        forrange (index, args.count) {
            Symbol* arg = args.E[index];

            Mem_Copy_Forward(&array->E[elemSize * index], Symbol_Data_Get_And_Check(elemType, arg), elemSize);
        }
    } else if ( out->type->tag == TAG_Bundle ) {
        Bundle* bundle = Poly(Bundle, out->type);
        Assert( args.count == bundle->fields.count );

        forrange (index, args.count) {
            Symbol* arg = args.E[index];
            Field* field = &bundle->fields.E[index];

            void* dst = Symbol_Data_Get_And_Check(field->type, Symbol_Get((Symbol*)out, field->name));
            void* src = Symbol_Data_Get_And_Check(field->type, arg);

            Mem_Copy_Forward(dst, src, Type_Size(field->type));
        }
    }
}

Public Fn_Call* Fn_Call_Create(Variadic inArgs) {
    Local_Array(Symbol_Unit, args, KiB(1));

    Symbol_Unit fn = *D(Symbol_Unit, inArgs.E[0]);

    inArgs = (Variadic){ .count = inArgs.count - 1, .E = &inArgs.E[1] };

    foreach (Symbol*, argRef, inArgs) {
        Symbol* arg = *argRef;
        Assert( arg->type == Type_All_Get(S("Symbol_Unit")) );

        Array_Push_C(args, *D(Symbol_Unit, arg));
    }

    return *D(Fn_Call*, F_(fn, args));
}

Public b8 Type_Has_Pointer_Rec(Type* type) {
    switch ( type->tag ) {
        case TAG_Pointer: { return true; } break;
        case TAG_Integer: case TAG_Raw_Data: case TAG_Signature: case TAG_Enum: { return false; } break;

        case TAG_Bundle: {
            foreach (Field, field, Poly(Bundle, type)->fields) {
                if ( Type_Has_Pointer_Rec(field->type) ) { return true; }
            }
        } break;

        case TAG_Union: {
            foreach (Field, field, Poly(Union, type)->fields) {
                if ( Type_Has_Pointer_Rec(field->type) ) { return true; }
            }
        } break;

        case TAG_WArray: {
            if ( Type_Has_Pointer_Rec(Poly(WArray, type)->elemType) ) { return true; }
        } break;

        case TAG_Array: {
            if ( Type_Has_Pointer_Rec(Poly(Array, type)->elemType) ) { return true; }
        } break;

        case TAG_Null:
        NO_DEFAULT
    }

    return false;
}

#define Print_New_Line(fmt, ...) printf("\n%*s"fmt, depthLevel*2, "", __VA_ARGS__)

#if 0
Public void Print_Var(Any_Type var, b8 isArrayElem, u32 depthLevel, u32* counter) {
    ++(*counter);

    if ( !isArrayElem ) {
        Print_New_Line("%s %s", var->type->name.E, var->name.E);
    }

    if ( Str_Equal(var->type->name, S("String")) ) {
        printf(": [\"");
        Output((Variadic)Array_From_Raw_Array(Arg, (Arg[]){ { .symbol = var } }));
        printf("\"]");
    }

    if ( !Str_Equal(var->type->name, S("Any_Type")) ) {
        switch ( var->type->tag ) {
            case TAG_Integer: case TAG_Pointer: case TAG_Enum: case TAG_Raw_Data: {
                printf(": ");

                Output((Variadic)Array_From_Raw_Array(Arg, (Arg[]){ { .symbol = var } }));

                printf(" <0x%016llx>", (u64)var->data);
            } break;

            case TAG_Union: case TAG_Signature: {
                INVALID_PATH("");
            } break;

            case TAG_Bundle: { } break;
            case TAG_WArray: { } break;
            case TAG_Array: { } break;

            case TAG_Null:
            NO_DEFAULT
        }
    }

    //if ( var->isPerm ) { return; }

    if ( var->childs.count != 0 ) {
        if ( !isArrayElem ) {
            printf(" = {");
        } else {
            Print_New_Line("{");
        }

        foreach (Symbol_Unit, unit, var->childs) {
            Print_Var(unit->symbol, false, depthLevel+1, counter);
        }

        Print_New_Line("}");
    }
}
#endif

typedef struct {
    Slice_u8 from;
    Slice_u8 to;
} Realloc_State;
Public Global Realloc_State gReallocState;

typedef enum {
    REALLOC_ERROR_IN_NEADER_FROM_OR_TO = 0,
    REALLOC_ERROR_ALREADY_IN_TO = 1,
    REALLOC_SUCCESS = 2,
} Realloc_Error;

Public Realloc_Error Realloc(void* inAddress, Realloc_State* state) {
    u64* address = (u64*)inAddress;

    if ( !(*address) ) { return REALLOC_ERROR_ALREADY_IN_TO; }

    if ( !Array_Is_In_Bounds_Ptr(state->to, *(void**)address) ) {
        if ( !Array_Is_In_Bounds_Ptr(state->from, *(void**)address) ) {
            return REALLOC_ERROR_IN_NEADER_FROM_OR_TO;
        }

        *address = (u64)state->to.E + ( *address - (u64)state->from.E );

        return REALLOC_SUCCESS;
    }

    return REALLOC_ERROR_ALREADY_IN_TO;
}

Private Global Memory_Arena output;

#define printf(...)
#define Output(...)

#define Serialize(var, recursive, stringBefore) Serialize_Rec(var, recursive, false, stringBefore, 0)
Public void Serialize_Rec(Any_Type var, b8 recursive, b8 isArrayElem, b8 stringBefore, u32 depthLevel) {
    if ( !( var->type->tag == TAG_Pointer || var->type->tag == TAG_Array || var->type->tag == TAG_Bundle ) ) { return; }

    //u8* varAddress = Symbol_Data_Get(var);

    if ( !isArrayElem ) {
        Print_New_Line("%s %s", var->type->name.E, var->name.E);
    }

    if ( Str_Equal(var->type->name, S("Any_Type")) ) { return; }

#if 1
    if ( stringBefore && Str_Equal(var->type->name, S("String")) ) {
        printf(": [\"");
        Output((Variadic)Array_From_Raw_Array(Arg, (Arg[]){ { .symbol = var } }));
        printf("\"]");
    }
#endif

    switch ( var->type->tag ) {
        case TAG_Pointer: {
            if ( var->type->tag == TAG_Pointer ) {
                printf(": ");

                Output((Variadic)Array_From_Raw_Array(Arg, (Arg[]){ { .symbol = var } }));
#if 1
                if ( Realloc(Symbol_Data_Get(var), &gReallocState) == REALLOC_SUCCESS ) {
                    printf(" -> ");
                    Output((Variadic)Array_From_Raw_Array(Arg, (Arg[]){ { .symbol = var } }));
                }
#endif

                printf(" <0x%016llx>", (u64)var->data);
            }
        } break;

        case TAG_Array: {
            printf(": [");

            Local_Str(iterName, KiB(4));
            Str_From_Fmt(&iterName, "iter_%u", depthLevel);

            Array_u8* array = (Array_u8*)Symbol_Data_Get(var);
            Assert( Realloc(&array->E, &(Realloc_State){ .from = gReallocState.from, .to = Slice_From_Array(u8, output), }) !=
                    REALLOC_ERROR_IN_NEADER_FROM_OR_TO);

            Foreach_Begin(iter, iterName, var);
                if ( iter->type->tag == TAG_Pointer || Type_Has_Pointer_Rec(iter->type) ) {
                    Serialize_Rec(iter, recursive, true, stringBefore, depthLevel+1);
                }
            Foreach_End();

            Realloc(&array->E, &(Realloc_State){ .from = Slice_From_Array(u8, output), .to = gReallocState.from, });
            //Realloc(&array->E, &(Realloc_State){ .from = Slice_From_Array(u8, output), .to = Slice_From_Array(u8, gSymbolState.stateArena), });

            Print_New_Line("]");
        } break;

        case TAG_Integer: case TAG_Enum: case TAG_Raw_Data:
        case TAG_Union: case TAG_Signature: {
            INVALID_PATH("");
        } break;

        case TAG_WArray: { } break;
        case TAG_Bundle: { } break;

        case TAG_Null:
        NO_DEFAULT
    }

    // !var->isPerm || var->type->tag == TAG_Array
    if ( recursive && var->childs.count != 0 ) {
        if ( !isArrayElem ) {
            printf(" = {");
        } else {
            Print_New_Line("{");
        }

        foreach (Symbol_Unit, unit, var->childs) {
            if ( unit->symbol->type->tag == TAG_Pointer || Type_Has_Pointer_Rec(unit->symbol->type) ) {
                Serialize_Rec(unit->symbol, recursive, false, stringBefore, depthLevel+1);
            }
        }

        Print_New_Line("}");
    }

    if ( !stringBefore && Str_Equal(var->type->name, S("String")) ) {
        printf(": [\"");
        Output((Variadic)Array_From_Raw_Array(Arg, (Arg[]){ { .symbol = var } }));
        printf("\"]");
    }
}

#define Iter(type, ptr) Util_Iter(&iter, Type_Get(S(#type)), ptr)
Public Symbol* Util_Iter(Symbol** iter, Type* type, void* ptr) {
    Symbol* scope = Scope_Top(0);

    if ( !*iter ) {
        Assert( scope != Symbol_Core() );
        *iter = Symbol_Create_And_Init_(scope, S("iter"), false, type, false, ptr);
    } else {
        if ( !Str_Equal((*iter)->type->name, type->name) ) {
            DArray_Create((*iter)->childs, &gTempArena);
            Symbol_Set(*iter, type, false, ptr);
        } else {
            Symbol_Set_Mem(*iter, ptr);
        }
    }

    return *iter;
}

Public void Wea_Save(String path) {
    output = Arena_Create_From_Arena(&gTempArena, MiB(32));
    Arena_Push_Arena(&output, gSymbolState.stateArena);

    Array_Clear(&gInterState.codeStack);

    Interpret_Begin();

    foreach (Symbol_Unit, moduleUnit, gSymbolState.root->childs) {
        (void)Symbol_Push_Child_C(Scope_Top(0), moduleUnit->name, moduleUnit->symbol, true);
    }

    Symbol* iter = 0;

    Symbol_State* state = (Symbol_State*)output.E;
    *state = gSymbolState;

    gReallocState = (Realloc_State){ .from = Slice_From_Array(u8, gSymbolState.stateArena), .to = { 0 }, };
    gReallocState = (Realloc_State){
        .from = Slice_From_Array(u8, gSymbolState.stateArena),
        .to   = { .count = gSymbolState.stateArena.count, .E = 0, },
    };

    foreach (Symbol, symbol, gSymbolState.symbolPool) {
        if ( symbol->type->tag == TAG_Pointer || ( symbol->type->tag == TAG_Array && Type_Has_Pointer_Rec(symbol->type) ) ) {
            u8* address = Symbol_Data_Get(symbol);
            Realloc(&address, &(Realloc_State){ .from = gReallocState.from, .to = Slice_From_Array(u8, output), });
            Serialize(Util_Iter(&iter, symbol->type, address), true, true);
        } else if ( Str_Equal(symbol->type->name, S("String")) ) {
            printf("\n: [\"");
            Output((Variadic)Array_From_Raw_Array(Arg, (Arg[]){ { .symbol = symbol } }));
            printf("\"]");
        }
    }

    Serialize(Iter(Symbol_State, state), true, true);

    printf("\n\n\n\n");

    Interpret_End();

    File_Fill_With_Contents_Safe(path, output.E, output.count);

    Arena_Clear(&gTempArena);
}

Public void Wea_Load(String path) {
    output = Arena_Create_From_Arena(&gTempArena, MiB(32));
    File_Read_All(path, &output, 0);

    Array_Clear(&gInterState.codeStack);

    Interpret_Begin();

    foreach (Symbol_Unit, moduleUnit, gSymbolState.root->childs) {
        (void)Symbol_Push_Child_C(Scope_Top(0), moduleUnit->name, moduleUnit->symbol, true);
    }

    Symbol* iter = 0;

    Symbol_State* state = (Symbol_State*)output.E;

    gReallocState = (Realloc_State){
        .from = { .count = gSymbolState.stateArena.count, .E = 0, },
        .to   = Slice_From_Array(u8, gSymbolState.stateArena),
    };

    Serialize(Iter(Symbol_State, state), true, false);

    foreach (Symbol, symbol, state->symbolPool) {
        if ( symbol->type->tag == TAG_Pointer || ( symbol->type->tag == TAG_Array && Type_Has_Pointer_Rec(symbol->type) ) ) {
            u8* address = Symbol_Data_Get(symbol);
            Realloc(&address, &(Realloc_State){ .from = Slice_From_Array(u8, gSymbolState.stateArena), .to = Slice_From_Array(u8, output), });
            Serialize(Util_Iter(&iter, symbol->type, address), true, false);
        }
    }

    Interpret_End();

    Arena_Zero(&gSymbolState.stateArena);

    state->stateArena = gSymbolState.stateArena;
    Arena_Push_Arena(&gSymbolState.stateArena, output);
    gSymbolState = *state;

    Arena_Clear(&gTempArena);
}

#undef printf
#undef Output
