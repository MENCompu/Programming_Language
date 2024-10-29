#define CORE_SYMBOL_NAME S("Core")

typedef struct {
    b8 using;

    Code_Block core;
} Symbol_Init_State;

Private Global Symbol_Init_State gSymbolInit;

Public Symbol_Unit* Symbol_Push_Child_C_(Symbol* parent, Symbol_Unit unit) {
    if ( !Mem_Equal_Zero_Type(&unit.name) ) {
        if ( parent->isPerm ) {
            unit.name = Str_Alloc_And_Copy(&gSymbolState.stringArena, unit.name, true);
        } else {
            unit.name = Str_Alloc_And_Copy(&gTempArena, unit.name, true);
        }
    }

    Memory_Arena* arena = ( parent->isPerm ) ? &gSymbolState.symbolUnitArena : &gTempArena;
    Symbol_Unit* ret = DArray_Push_C(arena, &parent->childs, unit);
    return ret;
}

Public Fn* Symbol_Get_Module(String name) {
    Symbol* module = Symbol_Get(gSymbolState.root, name);
    Assert( module->type == Type_Get(S("Fn")) );
    return D(Fn, module);
}

Public Symbol* Symbol_Core(void) {
    Symbol* core;

    if ( gSymbolInit.using ) {
        core = gSymbolInit.core.locals;
    } else {
        Fn* module = D(Fn, Symbol_Get(gSymbolState.root, CORE_SYMBOL_NAME));
        core = module->codeBlock.locals;
    }

    return core;
}

Public Symbol* Scope_Top(u32 index) {
    return Array_Index_Back(gInterState.codeStack, index).codeBlock->locals;
}

Public Symbol* Module_Top(void) {
    Scope_Iter(scope) {
        if ( scope->isPerm ) {
            return scope;
        }
    }

    LogFatal("There isn't a current module");
    return 0;
}

Public Symbol_Unit* Symbol_Unit_Get_Unsafe_(Symbol* parent, String name, b8 inExternLocals) {
    Symbol_Unit* ret = 0;

    foreach (Symbol_Unit, unit, parent->childs) {
        Symbol_Unit* temp = 0;

        if ( unit->name.count != 0 && Str_Equal(unit->name, name) ) {
            temp = unit;
        } else if ( unit->using ) {
            b8 isExternLocals = unit->symbol->isModule;

            if ( !( isExternLocals && inExternLocals ) ) {
                temp = Symbol_Unit_Get_Unsafe_(unit->symbol, name, isExternLocals);
            }
        }

        if ( temp ) {
            if ( ret ) {
                LogFatal("There are collisions");
            } else {
                ret = temp;
            }
        }
    }

    return ret;
}

Public Symbol_Unit* Symbol_Unit_Get(Symbol* parent, String name) {
    Symbol_Unit* unit = Symbol_Unit_Get_Unsafe(parent, name);
    Assert( unit );
    return unit;
}

Public Symbol_Unit* Symbol_Unit_Scope_Get(String name) {
    return Symbol_Unit_Get(Scope_Top(0), name);
}

Public Symbol* Symbol_Get_Unsafe(Symbol* parent, String name) {
    Symbol_Unit* unit = Symbol_Unit_Get_Unsafe(parent, name);
    return ( unit ) ? unit->symbol : 0;
}

Public Symbol* Symbol_Get(Symbol* parent, String name) {
    Symbol* symbol = Symbol_Get_Unsafe(parent, name);
    Assert( symbol );
    return symbol;
}

Public Symbol* Symbol_Scope_Get(String name) {
    return Symbol_Get(Scope_Top(0), name);
}

Public b8 Symbol_Exists(Symbol* parent, String name) {
    return ( Symbol_Get_Unsafe(parent, name) != 0 );
}

Public b8 Symbol_Check_Type(Symbol* parent, String name, Type* type) {
    return ( Symbol_Get_Unsafe(parent, name)->type == type );
}

Public Symbol* Get_Sym(String name);

Public Symbol* Get_Sym_Unsafe(String name) {
    Scope_Iter(scope) {
        Symbol* symbol = Symbol_Get_Unsafe(scope, name);
        if ( symbol ) { return symbol; }
    }

    foreach (Symbol_Unit, weaTypeUnit, gSymbolState.types->childs) {
        Type* type = D(Type, weaTypeUnit->symbol);

        if ( ( type->tag == TAG_Enum ) && Symbol_Exists(Poly(Enum, type)->values, name) ) {
            return Symbol_Get(Poly(Enum, type)->values, name);
        }
    }

    if ( !Type_Primitive_Create_By_Name(Scope_Top(0), name) ) { return 0; }
    return Get_Sym(name);
}

Public Symbol* Get_Sym(String name) {
    Symbol* symbol = Get_Sym_Unsafe(name);
    if ( !symbol ) {
        LogFatal("Couldn't find a symbol with the given name in any of the symbol bases");
    }
    return symbol;
}

Public Symbol* Symbol_Forward(Symbol* parent, String name) {
    return Symbol_Get(parent, name);
}

Public Symbol* Symbol_Backward(Symbol* child) {
    if ( !child->parent ) {
        Assert( child->type->tag == TAG_Pointer );

        child->parent = Symbol_New(0, S("__symbol__"));
        Symbol_Create_From_Type(child->parent, Type_All_Get(S("Symbol")));
    }

    return child->parent;
}

Public Symbol* Symbol_Inner(Symbol* symbol) {
    if ( !symbol->inner ) {
        symbol->inner = Symbol_Create_And_Init(0, S("__symbol__"), Type_All_Get(S("Symbol")), false, symbol);
        symbol->inner->outer = symbol;
    }

    return symbol->inner;
}

Public Symbol* Symbol_Outer(Symbol* symbol) {
    Assert( symbol->outer );
    return symbol->outer;
}

#if 1
#if 0
Public void Addr(/*OUT*/ Any_Type out, Any_Type var) {
    Assert( Poly(Pointer, out->type)->type == var->type );

    *(u8**)Symbol_Data_Get(out) = var->data;
}

Public void Deref(/*OUT*/ Any_Type out, Any_Type ptr) {
    if ( out->type == Type_All_Get(S("Any_Type")) ) {
        Symbol_Set(out, Poly(Pointer, ptr->type)->type, true, 0);
    }

    Assert( Poly(Pointer, ptr->type)->type == out->type );

    Symbol_Set_Mem(out, *(u8**)Symbol_Data_Get(ptr));
}
#else
Public Symbol* Addr(Symbol* symbol) {
    u8* data = Symbol_Data_Get(symbol);
    return Symbol_Create_And_Init(0, (String){ 0 }, Type_Ptr_Get(symbol->type), true, &data);
}

Public Symbol* Deref(Symbol* symbol) {
    Assert( symbol->type->tag == TAG_Pointer );

    return Symbol_Create_And_Init(0, (String){ 0 }, Poly(Pointer, symbol->type)->type, false, *(u8**)Symbol_Data_Get(symbol));
}
#endif

#else
Public Symbol* Addr(Symbol* symbol) {
    return Symbol_Forward(Symbol_Inner(symbol), S("data"));
}

Public Symbol* Deref(Symbol* symbol) {
    return Symbol_Outer(Symbol_Backward(symbol));
}
#endif

Public b8 Symbol_Data_Is_In_Data_Arena(Symbol* symbol) {
    return ( Array_Is_In_Bounds_Ptr(gSymbolState.dataArena, symbol) );
}

Public u8* Symbol_Data_Create(u32 size) {
    return (u8*)Arena_Alloc_Mem(&gSymbolState.dataArena, size);
}

Public void* Symbol_Data_Get(Symbol* symbol) {
    Assert( symbol->data );
    return symbol->data;
#if 0
    Loop {
        if ( Type_Is_Primitive(symbol->type) ) {
            return symbol->data;
        } else if ( Type_Is_Not_Primitive(symbol->type) ) {
            if ( gSymbolInit.using && symbol->data ) { return symbol->data; }
            symbol = symbol->childs.E[0].symbol;
        } NO_ELSE
    }
#endif
}

Public void* Symbol_Data_Get_And_Check(Type* type, Symbol* symbol) {
    Assert( Type_Check(symbol->type, type) );
    return Symbol_Data_Get(symbol);
}

Public Symbol* Symbol_New_(Symbol* parent, String name, b8 isPerm) {
    Symbol* symbol = 0;

    String copyName = { 0 };
    if ( !Mem_Equal_Zero_Type(&name) ) {
        if ( isPerm ) {
            copyName = Str_Alloc_And_Copy(&gSymbolState.stringArena, name, true);
        } else {
            copyName = Str_Alloc_And_Copy(&gTempArena, name, true);
        }
    }

    if ( isPerm ) {
        symbol = Array_Push(gSymbolState.symbolPool);
    } else {
        symbol = Arena_Alloc_Type(&gTempArena, Symbol);
    }

    *symbol = (Symbol){
        .type   = Type_All_Get(S("Any_Type")),
        .isPerm = isPerm,
        .parent = parent,
    };

    symbol->name = copyName;

    if ( isPerm ) {
        DArray_Create(symbol->childs, &gSymbolState.symbolUnitArena);
    } else {
        DArray_Create(symbol->childs, &gTempArena);
    }

    if ( parent ) {
        (void)Symbol_Push_Child_C(parent, copyName, symbol);
    }

    return symbol;
}

Public Symbol* Symbol_Set(Symbol* symbol, Type* type, b8 allocNew, void* mem) {
    if ( type == Type_All_Get(S("Any_Type")) ) {
        symbol->lazyCreation = false;
        return symbol;
    }

    Symbol_Create_From_Type(symbol, type);

    if ( type->tag == TAG_WArray ) { return symbol; }

    u8* symbolMem = mem;

    if ( allocNew ) {
        if ( symbol->isPerm ) {
            symbolMem = Symbol_Data_Create(Type_Size(type));
        } else {
            symbolMem = Arena_Alloc_Mem(&gTempArena, Type_Size(type));
        }

        Mem_Zero(symbolMem, Type_Size(type));

        if ( mem ) {
            Mem_Copy_Forward(symbolMem, mem, Type_Size(type));
        }
    }

    Symbol_Set_Mem(symbol, symbolMem);

    return symbol;
}

Public Symbol* Symbol_Create(Symbol* parent, String name, Type* type) {
    return Symbol_Set(Symbol_New(parent, name), type, true, 0);
}

Public Symbol* Symbol_Create_And_Init_(Symbol* parent, String name, b8 isPerm, Type* type, b8 allocNew, void* mem) {
    Symbol* symbol = Symbol_Set(Symbol_New_(parent, name, isPerm), type, allocNew, mem);
    return symbol;
}

#define L(type, data) Symbol_Lit_Create(Type_Get(S(#type)), &(data))
Public Symbol* Symbol_Lit_Create(Type* type, void* data) {
    Local_Str(name, KiB(4));

    if ( type == Type_All_Get(S("String")) ) {
        Str_Copy(&name, *(String*)data);
    } else if ( type == Type_All_Get(S("u32")) ) {
        Str_From_Fmt(&name, "%u", *(u32*)data);
    }

    return Symbol_Create_And_Init(gSymbolState.lits, name, type, true, data);
}

Public Symbol* Wea_Str_Lit(String str) {
    String copy = Str_Alloc_And_Copy(&gSymbolState.stringArena, str, true);
    return Symbol_Lit_Create(Type_Get(S("String")), &copy);
}

Public Symbol* WLit(Symbol* symbol) {
    return Symbol_Create_And_Init(gSymbolState.lits, symbol->name, symbol->type, true, Symbol_Data_Get(symbol));
}

#if 0
Public Symbol* Symbol_Create_From_Pointer(Symbol* parent, Type* type, String name) {
    Local_Str(ogInstance, KiB(4));
    Str_From_Fmt(&ogInstance, "%s_Impl", name.E);

    Symbol* instance = Symbol_New(0, ogInstance);
    instance->lazyCreation = true;

    Symbol* ptr = Addr(instance);
    Symbol_Push_Child_C(parent, name, ptr);

    return ptr;
}
#endif

Public void Symbol_Create_From_Fields(Symbol* symbol, Array_Field fields) {
    foreach (Field, field, fields) {
        Symbol_Create_From_Type(Symbol_New_(symbol, field->name, symbol->isPerm), field->type);
    }
}

Public void Symbol_Create_From_Type(Symbol* symbol, Type* type) {
    symbol->type = type;

    if ( Str_Equal(type->name, S("Enum")) ) { return; }

    if ( type ) {
        switch ( type->tag ) {
            case TAG_Bundle: {
                Assert( Poly(Bundle, type)->fields.count != 0 );
                Symbol_Create_From_Fields(symbol, Poly(Bundle, type)->fields);
            } break;

            case TAG_Union: {
                Assert( Poly(Union, type)->fields.count != 0 );

                Symbol_Create_From_Fields(symbol, Poly(Union, type)->fields);
            } break;

            case TAG_Array: {
                Local_Str(elemPtrTypeName, KiB(4));
                Str_From_Fmt(&elemPtrTypeName, "%s*", Poly(Array, type)->elemType->name.E);

                Symbol_Create_From_Fields(symbol, Array_From_Raw_Array(Field, ((Field[]){
                    { .name = S("capacity"), .type = Type_All_Get(S("u32")) },
                    { .name = S("count"), .type = Type_All_Get(S("u32")) },
                    { .name = S("E"), .type = Type_All_Get(elemPtrTypeName) },
                })));
            } break;

            case TAG_WArray: { } break;
            case TAG_Pointer: case TAG_Integer: case TAG_Raw_Data: case TAG_Enum:
            case TAG_Signature: { } break;

            case TAG_Null:
            NO_DEFAULT
        }
    }
}

Public void Symbol_Set_Mem(Symbol* symbol, u8* mem) {
    if ( Str_Equal(symbol->type->name, S("Enum")) ) { return; }

    switch ( symbol->type->tag ) {
        case TAG_Pointer: {
            symbol->data = mem;
        } break;

        case TAG_Integer: case TAG_Raw_Data: case TAG_Enum: {
            symbol->data = mem;
        } break;

        case TAG_Bundle: {
            Assert( Poly(Bundle, symbol->type)->fields.count != 0 );
            symbol->data = mem;

            foreach (Symbol_Unit, unit, symbol->childs) {
                Symbol_Set_Mem(unit->symbol, mem);
                mem += Type_Size(unit->symbol->type);
            }
        } break;

        case TAG_Union: {
            Assert( Poly(Union, symbol->type)->fields.count != 0 );
            symbol->data = mem;

            foreach (Symbol_Unit, unit, symbol->childs) {
                Symbol_Set_Mem(unit->symbol, mem);
            }
        } break;

        case TAG_Array: {
            Assert( symbol->childs.count == 3 );
            symbol->data = mem;

            foreach (Symbol_Unit, unit, symbol->childs) {
                Symbol_Set_Mem(unit->symbol, mem);
                mem += Type_Size(unit->symbol->type);
            }
        } break;

        case TAG_WArray: { } break;
        case TAG_Signature: { } break;

        case TAG_Null:
        NO_DEFAULT
    }
}

Public void Symbol_Resolve(Symbol* symbol) {
    Assert( symbol->type != Type_All_Get(S("Any_Type")) );

    switch ( symbol->type->tag ) {
        case TAG_Bundle: case TAG_Union: {
            Array_Field fields = ( symbol->type->tag == TAG_Bundle ) ? Poly(Bundle, symbol->type)->fields : Poly(Union, symbol->type)->fields;

            Assert( fields.count == symbol->childs.count );

            forrange (index, fields.count) {
                Symbol_Unit* unit = &symbol->childs.E[index];

                unit->name = Str_Alloc_And_Copy(&gSymbolState.stringArena, fields.E[index].name, true);
                unit->symbol->name = unit->name;
            }
        } break;

        case TAG_Array: case TAG_WArray: {
            //Type* elemType = ( type->tag == TAG_Array ) ? Poly(Array, type)->elemType : Poly(WArray, type)->elemType;
        } break;

        case TAG_Null:
        case TAG_Enum:
        case TAG_Signature:
        case TAG_Integer:
        case TAG_Pointer:
        case TAG_Raw_Data: { INVALID_PATH(""); }
    }
}

Public b8 Are_Symbol_And_Type_Compatible(Symbol* symbol, Type* type) {
    switch ( type->tag ) {
        case TAG_Bundle: case TAG_Union: {
            Array_Field fields = ( type->tag == TAG_Bundle ) ? Poly(Bundle, type)->fields : Poly(Union, type)->fields;

            if ( fields.count != symbol->childs.count ) { return false; }

            forrange (index, fields.count) {
                if ( fields.E[index].type != symbol->childs.E[index].symbol->type) {
                    return false;
                }
            }
        } break;

        case TAG_Array: case TAG_WArray: {
            Type* elemType = ( type->tag == TAG_Array ) ? Poly(Array, type)->elemType : Poly(WArray, type)->elemType;

            foreach (Symbol_Unit, unit, symbol->childs) {
                if ( unit->symbol->type != elemType ) {
                    return false;
                }
            }
        } break;

        case TAG_Null:
        case TAG_Enum:
        case TAG_Signature:
        case TAG_Integer:
        case TAG_Pointer:
        case TAG_Raw_Data: { INVALID_PATH(""); }
    }

    return true;
}

Public Fn* Fn_Create_(Symbol* parent, String name, Param ret, Array_Param params, Array_Fn_Call_P code, b8 shouldCreateLocals) {
    Local_Str(sigName, KiB(4));
    Str_From_Fmt(&sigName, "%s_Sig", name.E);

    Type* sigType = Type_Create(parent, sigName);
    sigType->tag = TAG_Signature;
    Signature* sig = Poly(Signature, sigType);

    u32 count = params.count + ( ( !Mem_Equal_Zero_Type(&ret) ) ? 1 : 0 );

    if ( count != 0 ) {
        Array_Create(sig->shared, count, &gSymbolState.stateArena);

        if ( !Mem_Equal_Zero_Type(&ret) ) {
            Array_Push_C(sig->shared, (Shared){
                .tag = { .dir = SHARED_TAG_OUT, .isOptional = false },
                .type = Type_Get(ret.type),
                .name = ret.name
            });
        }

        foreach (Param, param, params) {
            Array_Push_C(sig->shared, (Shared){
                .tag = { .dir = SHARED_TAG_IN, .isOptional = false },
                .type = Type_Get(param->type),
                .name = param->name,
            });
        }

        // NOTE(JENH): This function is used to Check that the variadic, if there is one, fulfills all its restrictions.
        (void)Has_Variadic(sig->shared);
    }

    Symbol* fnSymbol = Symbol_Create_And_Init(parent, name, Type_Get(S("Fn")), true, (&(Fn){
        .name = Str_Alloc_And_Copy(&gSymbolState.stringArena, name, true),
        .sig  = sigType,
        //.codeBlock = Symbol_Create(fnSymbol, ),
        .codeBlock = { .code = code, },
    }));

    Fn* fn = D(Fn, fnSymbol);

    if ( shouldCreateLocals ) {
        fn->codeBlock.locals = Symbol_Create(fnSymbol, S("__locals__"), Type_Get(S("Any_Type")));
    }

    return fn;
}

Public Fn* Fn_Create_With_Locals(Symbol* parent, String name, Param ret, Array_Param params, Array_Fn_Call_P code) {
    return Fn_Create_(parent, name, ret, params, code, true);
}

Public void Fn_Create(String name, Param ret, Array_Param params, Array_Fn_Call_P code) {
    Fn_Create_(Scope_Top(1), name, ret, params, code, false);
}

Public void Fn_Create_Scope(u32 scope, String name, Param ret, Array_Param params, Array_Fn_Call_P code) {
    Fn_Create_(Scope_Top(scope+1), name, ret, params, code, false);
}

Public Fn* Module_Create(String name, Array_Fn_Call_P code) {
    Fn* module = Fn_Create_With_Locals(gSymbolState.root, name, (Param){ 0 }, (Array_Param){ 0 }, code);
    module->codeBlock.locals->isModule = true;

    (void)Symbol_Push_Child_C(module->codeBlock.locals, CORE_SYMBOL_NAME, .symbol = Symbol_Core(), .using = true);

    if ( Str_Equal(name, S("Core")) ) { return module; }

    Symbol* moduleFnCall = Symbol_Create_And_Init(gSymbolState.fnCalls, (String){ 0 }, Type_All_Get(S("Fn_Call")), true, (&(Fn_Call){
        .fn = module,
        .isResolved = true,
        .args = Symbol_Create(0, S("__args__"), Type_Get(S("Any_Type")))
    }));

    Interpret_Fn_Call(Addr(moduleFnCall));

#if 0
    foreach (Fn_Call*, fnCallRef, module->codeBlock.code) {
        Fn_Call* fnCall = *fnCallRef;

        Fn_Call_Resolve_Rec(fnCall, 0);
        printf("%s;\n", Fn_Call_To_Text(*fnCallRef).E);
    }
#endif

    return module;
}

Public Symbol* Get_Fn_Call_Symbol(Fn_Call* fnCall) {
    foreach (Symbol_Unit, fnCallUnit, gSymbolState.fnCalls->childs) {
        if ( fnCall == *D(Fn_Call*, fnCallUnit->symbol) ) {
            return fnCallUnit->symbol;
        }
    }

    LogFatal("Couldn't find a fn call symbol that matches the given pointer");
    return 0;
}

#define PF(fnName, ...) F_By_Name(S(#fnName), Array_From_Raw_Array(Symbol_Unit, ( (Symbol_Unit[]){ __VA_ARGS__ } ) ) )
#define F(fnName, ...) \
    F_By_Name(S(#fnName), Array_From_Raw_Array(Symbol_Unit, ( (Symbol_Unit[]){ __VA_ARGS__ } ) ) )

Public Symbol* F_(Symbol_Unit fn, Array_Symbol_Unit inUnits) {
    Assert( fn.symbolFnCall );

    Symbol* args = Symbol_Create(0, S("__args__"), Type_Get(S("Any_Type")));
    Array_Alloc_And_Copy(&args->childs, inUnits, &gSymbolState.symbolUnitArena);

    Symbol* fnCallSymbol = Symbol_Create_And_Init(0, (String){ 0 }, Type_All_Get(S("Fn_Call")), true, (&(Fn_Call){
        .fnFnCall = fn.symbolFnCall,
        .args = args,
    }));

    Symbol* wFnCall = Addr(fnCallSymbol);
    Symbol_Push_Child_C(gSymbolState.fnCalls, (String){ 0 }, wFnCall);

    return wFnCall;
}

#define PF_By_Name(fnName, inUnits) F_By_Name(fnName, inUnits)
Public Symbol* F_By_Name(String fnName, Array_Symbol_Unit inUnits) {
    Symbol* getSymArgs = Symbol_Create(0, S("__args__"), Type_Get(S("Any_Type")));

    DArray_Create(getSymArgs->childs, &gSymbolState.symbolUnitArena);
    DArray_Push_C(&gSymbolState.symbolUnitArena, &getSymArgs->childs, (Symbol_Unit){ .symbol = Wea_Str_Lit(fnName), });

    Symbol* getSym = Symbol_Create_And_Init(0, (String){ 0 }, Type_All_Get(S("Fn_Call")), true, (&(Fn_Call){
        .fn = D(Fn, Symbol_Get(Symbol_Core(), S("Get_Sym_Unsafe"))),
        .args = getSymArgs,
    }));

    Symbol* wGetSym = Addr(getSym);
    Symbol_Push_Child_C(gSymbolState.fnCalls, (String){ 0 }, wGetSym);

    return F_((Symbol_Unit){ .symbolFnCall = wGetSym, }, inUnits);
}

Public void Symbol_Init(void) {
    gSymbolInit.using = true;
    Symbol tempCodeSymbol = { 0 };

    /* Init global state */ {
        gSymbolState.stateArena = Arena_Create_From_Arena(&gPermArena, MiB(16));
        (void)Arena_Alloc_Type(&gSymbolState.stateArena, Symbol_State);

        Array_Create(gSymbolState.symbolPool, KiB(32), &gSymbolState.stateArena);

        gSymbolState.symbolUnitArena = Arena_Create_From_Arena(&gSymbolState.stateArena, MiB(4));
        gSymbolState.dataArena = Arena_Create_From_Arena(&gSymbolState.stateArena, MiB(4));
        gSymbolState.stringArena = Arena_Create_From_Arena(&gSymbolState.stateArena, MiB(2));

        // create
        gSymbolState.root = Array_Push(gSymbolState.symbolPool);
        //gSymbolState.root->name = Str_Alloc_And_Copy(&gSymbolState.stringArena, S("__ROOT__"), true);
        DArray_Create(gSymbolState.root->childs, &gSymbolState.symbolUnitArena);
        gSymbolState.root->isPerm = true;

        /* Create core module. */ {
            gSymbolInit.core = (Code_Block){ .locals = &tempCodeSymbol };
            gSymbolInit.core.locals->isPerm = true;
            DArray_Create(gSymbolInit.core.locals->childs, &gSymbolState.symbolUnitArena);
        }

        /* Create literals symbol. */ {
            gSymbolState.lits = Array_Push(gSymbolState.symbolPool);
            //gSymbolState.lits->name = Str_Alloc_And_Copy(&gSymbolState.stringArena, S("__LITS__"), true);
            gSymbolState.lits->isPerm = true;
            DArray_Create(gSymbolState.lits->childs, &gSymbolState.symbolUnitArena);
        }

        /* Create types symbol. */ {
            gSymbolState.types = Array_Push(gSymbolState.symbolPool);
            //gSymbolState.types->name = Str_Alloc_And_Copy(&gSymbolState.stringArena, S("__TYPES__"), true);
            gSymbolState.types->isPerm = true;
            DArray_Create(gSymbolState.types->childs, &gSymbolState.symbolUnitArena);
        }

        /* Create fnCalls symbol. */ {
            gSymbolState.fnCalls = Array_Push(gSymbolState.symbolPool);
            //gSymbolState.fnCalls->name = Str_Alloc_And_Copy(&gSymbolState.stringArena, S("__FNCALLS__"), true);
            gSymbolState.fnCalls->isPerm = true;
            DArray_Create(gSymbolState.fnCalls->childs, &gSymbolState.symbolUnitArena);
        }

        Array_Push_C(gInterState.codeStack, (Code){ .codeBlock = &gSymbolInit.core, });
    }

    /* Create types from c */ {
        /* Create "Type" and "Any_Tyoe" type. Can't be created in the "normal" path, beacuse they are used in there. */ {
            Type* typeType = Type_Create_Temp(S("Type"));
            typeType->tag = TAG_Bundle;
            (void)Type_Create_Temp(S("Any_Type"));
            Type_Init_Temp(typeType, S("Type"));

            foreach (Symbol_Unit, unit, Scope_Top(0)->childs) {
                if ( unit->symbol->type == Type_Get(S("Type")) ) {
                    (void)Symbol_Set(unit->symbol, Type_Get(S("Type")), 0, unit->symbol->data);

                    Type* type = D(Type, unit->symbol);
                    if ( type->tag == TAG_Enum ) {
                        Raw_Type* rawType = Raw_Type_Get(unit->name);

                        Array_String names;
                        Array_Create(names, rawType->values.count, &gTempArena);

                        foreach (Raw_Enum_Value, value, rawType->values) {
                            Array_Push_C(names, value->name);
                        }

                        Enum_Init(type, names);
                    }
                }
            }
        }

        Array_String filters = Array_From_Raw_Array(String, ((String[]){
            S_Const("Any_Type"), S_Const("Variadic"), S_Const("Type"), S_Const("Type"), S_Const("b8"),
        }));

        (void)Type_Create(Symbol_Core(), S("Variadic"));

        #define Should_Skip(name) \
            ( Array_String_Exists(filters, name) || Type_Name_Is_Primitive(name) )

        foreach (Raw_Type, rawType, cTypeInfo) {
            if ( Should_Skip(rawType->name) || Type_Exists(rawType->name) ) { continue; }

            if ( rawType->tag == TAG_Bundle || rawType->tag == TAG_Enum ) {
                (void)Type_Create(Symbol_Core(), rawType->name);
            }
        }

        foreach (Raw_Type, rawType, cTypeInfo) {
            if ( Should_Skip(rawType->name) || !Type_Exists(rawType->name) || Type_Get(rawType->name)->tag != TAG_Null ) { continue; }

            if ( rawType->tag == TAG_Bundle || rawType->tag == TAG_Enum ) {
                Type* type = Type_Get(rawType->name);

                if ( rawType->tag == TAG_Bundle ) {
                    foreach (Raw_Field, field, rawType->fields) {
                        if ( !Type_Exists(field->type) ) { goto skip_type; }
                    }

                    Bundle_Init(type, Array_From_Array_Static(Raw_Field, rawType->fields));
                } else if ( rawType->tag == TAG_Enum ) {
                    Array_String names;
                    Array_Create(names, rawType->values.count, &gTempArena);

                    foreach (Raw_Enum_Value, value, rawType->values) {
                        Array_Push_C(names, value->name);
                    }

                    Enum_Init(type, names);
                }
            }
            skip_type:;
        }
    }

    /* Create default core global variables. */ {
        u32 voidDataTypeSize = KiB(32);
        Local_Str(voidDataTypeName, KiB(4));
        Str_From_Fmt(&voidDataTypeName, "d%u", voidDataTypeSize);
        Type* voidDataType = Type_Get(voidDataTypeName);

        Symbol_Scope_Create(S("NULL"), voidDataType);
        Mem_Zero(Symbol_Data_Get(Symbol_Scope_Get(S("NULL"))), voidDataTypeSize);

        Let_C(voidDataTypeName, S("_"), Scope_Top(0));
    }

    /* Create fns from c */ {
        foreach (Raw_Fn, fnInfo, gFnsInfo) {
            Local_Str(sigName, KiB(4));
            Str_From_Fmt(&sigName, "%s_Sig", fnInfo->name.E);

            Array_Shared sharedList;
            Array_Create(sharedList, fnInfo->sharedList.count + ( Str_Equal(fnInfo->retType, S("void")) ? 0 : 1 ), &gSymbolState.stateArena);

#if 1
            if ( !Str_Equal(fnInfo->retType, S("void")) ) {
                if ( !Type_Exists(fnInfo->retType) ) { goto skip_fn; }

                Array_Push_C(sharedList, (Shared){
                    .tag = { .dir = SHARED_TAG_OUT, .isOptional = false },
                    .name = Str_Alloc_And_Copy(&gSymbolState.stringArena, S("c_out"), true),
                    .type = Type_Get(fnInfo->retType),
                });
            }
#endif

            foreach (Raw_Shared, rawShared, fnInfo->sharedList) {
                if ( !Type_Exists(rawShared->type) ) { goto skip_fn; }

                Shared shared = {
                    .tag = { .dir = (u32)rawShared->tag.dir, .isOptional = rawShared->tag.isOptional },
                    .name = Str_Alloc_And_Copy(&gSymbolState.stringArena, rawShared->name, true),
                    .type = Type_Get(rawShared->type),
                };

                if ( !Mem_Equal_Zero_Type(&rawShared->optSymbolName) ) {
                    shared.optSymbolName = Str_Alloc_And_Copy(&gSymbolState.stringArena, rawShared->optSymbolName, true);
                }

                Array_Push_C(sharedList, shared);

            }

            (void)Has_Variadic(sharedList); // Check that variadic is at the end of the arguments if there is one.

            Type* sigType = Type_Create(Symbol_Core(), sigName);
            sigType->tag = TAG_Signature;

            Array_Alloc_And_Copy(&Poly(Signature, sigType)->shared, sharedList, &gSymbolState.stateArena);

            (void)Symbol_Scope_Create_And_Init(fnInfo->name, Type_Get(S("Fn")), true, (&(Fn){
                .name = Str_Alloc_And_Copy(&gSymbolState.stringArena, fnInfo->name, true),
                .sig  = sigType,
                .codeBlock = { 0 },
            }));
            skip_fn:;
        }
    }

    gSymbolState.root->type  = Type_Get(S("Any_Type"));
    gSymbolState.lits->type  = Type_Get(S("Any_Type"));
    gSymbolState.types->type = Type_Get(S("Any_Type"));

    Fn* coreModule = Module_Create(CORE_SYMBOL_NAME, (Array_Fn_Call_P){ 0 });
    coreModule->codeBlock.locals->childs = gSymbolInit.core.locals->childs;

    Array_Pop(gInterState.codeStack);
    Array_Push_C(gInterState.codeStack, (Code){ .codeBlock = &coreModule->codeBlock });

    gSymbolInit = (Symbol_Init_State){ 0 };
    gSymbolInit.using = false;
}
