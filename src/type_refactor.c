Public b8 Type_Name_Is_Integer(String typeName) {
    if ( Mem_Equal_Zero_Type(&typeName) ) { return false; }
    return (b8)( ( typeName.E[0] == 'u' || typeName.E[0] == 's' ) && Str_Is_Integer(Str_Skip_Begin(typeName, 1)) );
}

Public b8 Type_Name_Is_Raw_Data(String typeName) {
    return (b8)( typeName.E[0] == 'd' && Str_Is_Integer(Str_Skip_Begin(typeName, 1)) );
}

Public b8 Type_Name_Is_Pointer(String typeName) {
    return (b8)( typeName.E[typeName.count - 1] == '*' );
}

Public b8 Type_Name_Is_Array(String typeName) {
    return (b8)( Str_Begin_Equal(typeName, S("Array_")) );
}

Public b8 Type_Name_Is_Primitive(String typeName) {
    return Type_Name_Is_Array(typeName)   || Type_Name_Is_Pointer(typeName)  ||
           Type_Name_Is_Integer(typeName) || Type_Name_Is_Raw_Data(typeName);
}

Public Symbol_Unit* Symbol_Push_Child(Symbol* symbol) {
    Memory_Arena* arena = ( symbol->isPerm ) ? &gSymbolState.symbolUnitArena : &gTempArena;
    Symbol_Unit* res = DArray_Push(arena, &symbol->childs);
    return res;
}

Public Symbol_Unit* Symbol_Push_Child_C(Symbol* symbol, Symbol_Unit unit) {
    Memory_Arena* arena = ( symbol->isPerm ) ? &gSymbolState.symbolUnitArena : &gTempArena;

    Symbol_Unit* ret = DArray_Push_C(arena, &symbol->childs, unit);
    return ret;
}

Public Symbol* Symbol_Get_Unsafe(Symbol* parent, String name) {
    Symbol* symbol = 0;

    foreach (Symbol_Unit, unit, parent->childs) {
        if ( unit->name.count != 0 && Str_Equal(unit->name, name) ) {
            if ( symbol ) { LogFatal("There are collisions"); }
            symbol = unit->symbol;
        }
#if 0
        else if ( Str_Equal(unit->name, ANONIMUS_SYMBOl_NAME) ) {
            Symbol_Get_Unsafe(unit->symbol, name);
        }
#endif
    }

    return symbol;
}

Public Symbol* Symbol_Get(Symbol* parent, String name) {
    Symbol* symbol = Symbol_Get_Unsafe(parent, name);
    Assert( symbol );
    return symbol;
}

Public Symbol* Symbol_Root_Get(String name) {
    return Symbol_Get(gSymbolState.root, name);
}

Public b8 Symbol_Exists(Symbol* parent, String name) {
    return ( Symbol_Get_Unsafe(parent, name) != 0 );
}

Public b8 Symbol_Check_Type(Symbol* parent, String name, Wea_Type* type) {
    return ( Symbol_Get_Unsafe(parent, name)->type == type );
}

Public Symbol* Get_Sym(String name) {
    foreach (Symbol*, scopeRef, gSymbolScope) {
        Symbol* scope = *scopeRef;

        Symbol* symbol = Symbol_Get_Unsafe(scope, name);
        if ( symbol ) { return symbol; }
    }

    foreach (Symbol*, baseRef, gSymbolBases) {
        Symbol* base = *baseRef;

        Symbol* symbol = Symbol_Get_Unsafe(base, name);
        if ( symbol ) { return symbol; }
    }

    LogFatal("Couldn't find a symbol with the given name in any of the symbol bases");
    return 0;
}

Public b8 Symbol_Root_Check_Type(String name, Wea_Type* type) {
    return Symbol_Check_Type(gSymbolState.root, name, type);
}

#if 0
Public b8 Symbol_Is_Data(Symbol* symbol) {
    if ( Arena_Is_In_Bounds_Ptr(gSymbolState.dataArena, symbol) ) {
        return true;
    }

    Assert( Array_Is_In_Bounds_Ptr(gSymbolState.symbolPool, symbol) );
    return false;
}
#endif

Public b8 Type_Is_Primitive(Wea_Type* weaType) {
    Type* type = *D(Type*, weaType);
    return (b8)( type->tag == TAG_Integer || type->tag == TAG_Pointer || type->tag == TAG_Raw_Data || type->tag == TAG_Enum );
}

Public b8 Type_Is_Not_Primitive(Wea_Type* weaType) {
    Type* type = *D(Type*, weaType);
    return (b8)( type->tag == TAG_Bundle || type->tag == TAG_Array || type->tag == TAG_Union );
}

#define PTR_SIZE 8

Public u32 Type_Size(Wea_Type* weaType) {
    Type* type = *D(Type*, weaType);

    switch ( type->tag ) {
        case TAG_Bundle: {
            u32 counter = 0;

            foreach (Field, field, Poly(Bundle, type)->fields) {
                counter += Type_Size(field->type);
            }

            return counter;
        } break;

        case TAG_Union: {
            u32 maxSize = 0;

            foreach (Field, field, Poly(Union, type)->fields) {
                u32 size = Type_Size(field->type);

                if ( maxSize > size ) {
                    maxSize = size;
                }
            }

            return maxSize;
        } break;

        case TAG_Enum: {
            return Type_Size(Type_Get(S("u32")));
        } break;

        case TAG_Array: {
            return sizeof(Array_void);
        } break;

        case TAG_Signature: { } break;

        case TAG_Integer: {
            return Poly(Integer, type)->size;
        } break;

        case TAG_Pointer: {
            return PTR_SIZE;
        } break;

        case TAG_Raw_Data: {
            return Poly(Raw_Data, type)->size;
        } break;

        NO_DEFAULT
    }

    return 0;
}

Public b8 Type_Check(Wea_Type* weaA, Wea_Type* weaB) {
    Type* a = *D(Type*, weaA);
    Type* b = *D(Type*, weaB);

    if ( a->tag == TAG_Integer && b->tag == TAG_Integer ) {
        return true;
    } else if ( a->tag == TAG_Raw_Data ) {
        return ( Type_Size(weaA) >= Type_Size(weaB) );
    } else if ( b->tag == TAG_Raw_Data ) {
        return ( Type_Size(weaB) >= Type_Size(weaA) );
    } else {
        return ( a == b );
    }
}

Public u8* Symbol_Data_Create(u32 size) {
    return (u8*)Arena_Alloc_Mem(&gSymbolState.dataArena, size);
}

Public void* Symbol_Data_Get(Symbol* symbol) {
    Loop {
        if ( Type_Is_Primitive(symbol->type) ) {
            return symbol->data;
        } else if ( Type_Is_Not_Primitive(symbol->type) ) {
            symbol = symbol->childs.E[0].symbol;
        } NO_ELSE
    }
}

Public void* Symbol_Data_Get_And_Check(Wea_Type* weaType, Symbol* symbol) {
    Assert( Type_Check(symbol->type, weaType) );
    return Symbol_Data_Get(symbol);
}

#define Symbol_Root_New(name) Symbol_New_(gSymbolState.root, name, true)
#define Symbol_New(parent, name) Symbol_New_(parent, name, true)
Public Symbol* Symbol_New_(Symbol* parent, String name, b8 isPerm) {
    Symbol* symbol = 0;

    String copyName;
    if ( isPerm ) {
        copyName = Str_Alloc_And_Copy(&gSymbolState.stringArena, name, true);
    } else {
        copyName = Str_Alloc_And_Copy(&gTempArena, name, true);
    }

    if ( isPerm ) {
        symbol = Array_Push(gSymbolState.symbolPool);
    } else {
        symbol = Arena_Alloc_Type(&gTempArena, Symbol);
    }

    *symbol = (Symbol){
        .name = copyName,
        .type = Type_Get(S("Any_Type")),
        .isPerm = isPerm,
    };

    if ( isPerm ) {
        DArray_Create(symbol->childs, &gSymbolState.symbolUnitArena);
    } else {
        DArray_Create(symbol->childs, &gTempArena);
    }

    if ( parent ) {
        (void)Symbol_Push_Child_C(parent, (Symbol_Unit){ .name = copyName, .symbol = symbol });
    }

    return symbol;
}

Public void Symbol_Set_Mem(Symbol* symbol, u8* mem);
Public void Symbol_Create_From_Type(Symbol* symbol, Wea_Type* type);

Public Symbol* Symbol_Set(Symbol* symbol, Wea_Type* type, b8 allocNew, void* mem) {
    if ( type == Type_Get(S("Any_Type")) ) { return symbol; }

    Symbol_Create_From_Type(symbol, type);

    u8* symbolMem = mem;

    if ( allocNew ) {
        if ( symbol->isPerm ) {
            symbolMem = Symbol_Data_Create(Type_Size(type));
        } else {
            symbolMem = Arena_Alloc_Mem(&gTempArena, Type_Size(type));
        }

        if ( mem ) {
            Mem_Copy_Forward(symbolMem, mem, Type_Size(type));
        }
    }

    Symbol_Set_Mem(symbol, symbolMem);

    return symbol;
}

#define Symbol_Root_Create(name, type) Symbol_Create(gSymbolState.root, name, type)
Public Symbol* Symbol_Create(Symbol* parent, String name, Wea_Type* type) {
    return Symbol_Set(Symbol_New(parent, name), type, true, 0);
}

Public void Symbol_Create_From_Type(Symbol* symbol, Wea_Type* weaType) {
    symbol->type = weaType;
    Type* type = *D(Type*, weaType);

    if ( type ) {
        switch ( type->tag ) {
            case TAG_Bundle: {
                Assert( Poly(Bundle, type)->fields.count != 0 );

                foreach (Field, field, Poly(Bundle, type)->fields) {
                    Symbol_Create_From_Type(Symbol_New(symbol, field->name), field->type);
                }
            } break;

            case TAG_Union: {
                Assert( Poly(Union, type)->fields.count != 0 );

                foreach (Field, field, Poly(Union, type)->fields) {
                    Symbol_Create_From_Type(Symbol_New(symbol, field->name), field->type);
                }
            } break;

            case TAG_Array: {
                Local_Str(elemPtrTypeName, KiB(4));
                Str_From_Fmt(&elemPtrTypeName, "%s*", Poly(Array, type)->elemType->name.E);

                Slice_Field fields = Slice_From_Raw_Array(Field, ((Field[]){
                    { .name = S("capacity"), .type = Type_Get(S("u32")) },
                    { .name = S("count"), .type = Type_Get(S("u32")) },
                    { .name = S("E"), .type = Type_Get(elemPtrTypeName) },
                }));

                foreach (Field, field, fields) {
                    Symbol_Create_From_Type(Symbol_New(symbol, field->name), field->type);
                }
            } break;

            case TAG_Pointer: case TAG_Integer: case TAG_Raw_Data: case TAG_Enum:
            case TAG_Signature: { } break;

            NO_DEFAULT
        }
    }
}

Public void Symbol_Set_Mem(Symbol* symbol, u8* mem) {
    Type* type = *D(Type*, symbol->type);

    switch ( type->tag ) {
        case TAG_Pointer: case TAG_Integer: case TAG_Raw_Data: case TAG_Enum: {
            symbol->data = mem;
        } break;

        case TAG_Bundle: {
            Assert( Poly(Bundle, type)->fields.count != 0 );

            foreach (Symbol_Unit, unit, symbol->childs) {
                Symbol_Set_Mem(unit->symbol, mem);
                mem += Type_Size(unit->symbol->type);
            }
        } break;

        case TAG_Union: {
            Assert( Poly(Union, type)->fields.count != 0 );

            foreach (Symbol_Unit, unit, symbol->childs) {
                Symbol_Set_Mem(unit->symbol, mem);
            }
        } break;

        case TAG_Array: {
            Assert( symbol->childs.count == 3 );

            foreach (Symbol_Unit, unit, symbol->childs) {
                Symbol_Set_Mem(unit->symbol, mem);
                mem += Type_Size(unit->symbol->type);
            }
        } break;

        case TAG_Signature: { } break;

        NO_DEFAULT
    }
}

Public b8 Has_Optional(Array_Shared sharedList) {
    forrange (index, sharedList.count) {
        Shared* shared = &sharedList.E[index];

        if ( shared->tag.isOptional ) {
            Assert( ( index == ( sharedList.count - 1 ) ), "Optionals should be at the last argument of the function" );
            return true;
        }
    }

    return false;
}

Public b8 Has_Variadic(Array_Shared sharedList) {
    forrange (index, sharedList.count) {
        Shared* shared = &sharedList.E[index];

        if ( shared->type == Type_Get(S("Variadic")) ) {
            Assert( ( index == ( sharedList.count - 1 ) ), "Variadic type should be the last argument of the function" );
            return true;
        }
    }

    return false;
}

Public Type* Type_Create(String name) {
    return D(Type, Symbol_Root_Create(name, Type_Get(S("Type"))));
}

Public void Fn_Create(String name, Param ret, Array_Param params, Array_Fn_Call_P code) {
    Local_Str(sigName, KiB(4));
    Str_From_Fmt(&sigName, "%s_Sig", name.E);

    Type* sigType = Type_Create(sigName);
    sigType->tag = TAG_Signature;
    Signature* sig = Poly(Signature, sigType);

    Array_Create(sig->shared, params.count + ( ( !Mem_Equal_Zero_Type(&ret) ) ? 1 : 0 ), &gSymbolState.stateArena);

    if ( !Mem_Equal_Zero_Type(&ret) ) {
        Array_Push_C(sig->shared, (Shared){ .tag = SHARED_TAG_OUT, .type = Type_Get(ret.type), .name = ret.name });
    }

    foreach (Param, param, params) {
        Array_Push_C(sig->shared, (Shared){ .tag = SHARED_TAG_IN, .type = Type_Get(param->type), .name = param->name });
    }

    // NOTE(JENH): This function is used to Check that the variadic, if there is one, fulfills all its restrictions.
    (void)Has_Variadic(sig->shared);

    Symbol* fnSymbol = Symbol_Root_Create_And_Init(name, Type_Get(S("Fn")), true, (&(Fn){
        .name = name,
        .sig  = sigType,
        .code = code,
        //.locals = Symbol_Root_Create((String){ 0 }, Type_Get(S("Any_Type"))),
    }));

#if 0
    Fn* fn = D(Fn, fnSymbol);

    foreach (Shared, shared, sig->shared) {
        Symbol_Push_Child_C(fn->locals, (Symbol_Unit){ .name = shared->name });
    }
#endif
}

Public void Enum_Init(Wea_Type* type, Array_String names) {
    type->tag = TAG_Enum;

    Symbol* values = Symbol_Root_Create(S(""), Type_Get(S("Any_Type")));

    u32 counter = 0;
    foreach (String, name, names) {
        Symbol_Create_And_Init(values, *name, Type_Get(S("u32")), true, &counter);
        ++counter;
    }

    Poly(Enum, type)->values = values;

    // This "values" symbol should be pushed inside the root, instead of alongside "root".
    Array_Push_C(gSymbolBases, values);
}

Public void Enum_Create(String name, Array_String names) {
    Enum_Init(Type_Create(name), names);
}

Public void Union_Init(Wea_Type* type, Array_Raw_Field rawFields) {
    type->tag = TAG_Union;

    Array_Field fields;
    Array_Create(fields, rawFields.count, &gSymbolState.stateArena);

    foreach (Raw_Field, rawField, rawFields) {
        Array_Push_C(fields, (Field){
            .type = Type_Get(rawField->type),
            .name = rawField->name,
        });
    }

    Poly(Union, type)->fields = fields;
}

Public void Union_Create(String name, Array_Raw_Field rawFields) {
    Union_Init(Type_Create(name), rawFields);
}

Public void Bundle_Init(Wea_Type* type, Array_Raw_Field rawFields) {
    type->tag = TAG_Bundle;

    Array_Field fields;
    Array_Create(fields, rawFields.count, &gSymbolState.stateArena);

    foreach (Raw_Field, rawField, rawFields) {
        Array_Push_C(fields, (Field){
            .type = Type_Get(rawField->type),
            .name = rawField->name,
        });
    }

    Poly(Bundle, type)->fields = fields;
}

Public void Bundle_Create(String name, Array_Raw_Field rawFields) {
    Bundle_Init(Type_Create(name), rawFields);
}

Public Wea_Type* Type_Get_Unsafe(String name) {
    foreach (Wea_Type, type, gSymbolState.typePool) {
        if ( Str_Equal(type->name, name) ) {
            return type;
        }
    }

    Wea_Type* type = 0;
    if ( Type_Name_Is_Pointer(name) ) {
        Wea_Type* pointee = Type_Get_Unsafe(Str_Skip_End(name, 1));
        if ( !pointee ) { return 0; }

        type = Type_Create(name);
        type->tag = TAG_Pointer;
        *Poly(Pointer, type) = (Pointer){
            .type = pointee,
        };
    } else if ( Type_Name_Is_Integer(name) ) {
        type = Type_Create(name);
        type->tag = TAG_Integer;
        *Poly(Integer, type) = (Integer){
            .size = (u32)Str_To_S64(Str_Skip_Begin(name, 1)) / BYTE_SIZE,
            .sign = ( name.E[0] == 's' ),
        };
    } else if ( Type_Name_Is_Raw_Data(name) ) {
        type = Type_Create(name);
        type->tag = TAG_Raw_Data;
        *Poly(Raw_Data, type) = (Raw_Data){
            .size = (u32)Str_To_S64(Str_Skip_Begin(name, 1)) / BYTE_SIZE,
        };
    } else if ( Type_Name_Is_Array(name) ) {
        Local_Str(elemTypeName, KiB(4));
        Str_Copy(&elemTypeName, Str_Skip_Begin(name, S("Array_").count));

        if ( Str_End_Equal(elemTypeName, S("_P")) ) {
            elemTypeName = Str_Skip_End(elemTypeName, S("_P").count);
            elemTypeName.E[(++elemTypeName.count) - 1] = '*';
            Str_Null_Terminate(&elemTypeName);
        }

        Wea_Type* elemType = Type_Get_Unsafe(elemTypeName);
        if ( !elemType ) { return 0; }

        type = Type_Create(name);
        type->tag = TAG_Array;
        *Poly(Array, type) = (Array){
            .elemType = elemType,
        };
    }

    return type;
}

Public Wea_Type* Type_Get(String name) {
    Wea_Type* type = Type_Get_Unsafe(name);
    Assert( type );
    return type;
}

Public Wea_Type* Type_Ptr_Get(Wea_Type* type) {
    Local_Str(typePtrName, KiB(4));
    Str_From_Fmt(&typePtrName, "%s*", type->name.E);

    return Type_Get(typePtrName);
}

#if 0
Public b8 Exists_Enum_Val(Enum* _enum, Symbol* valueSymbol) {
    u32 value = *(u32*)Symbol_Data_Get(valueSymbol);
    foreach (Symbol_Unit, unit, _enum->values->childs) {
        if ( *D(u32, unit->symbol) == value ) { return true; }
    }

    return false;
}
#else
Public b8 Exists_Enum_Val(Enum* _enum, Symbol* valueSymbol) {
    foreach (Symbol_Unit, unit, _enum->values->childs) {
        if ( unit->symbol == valueSymbol ) { return true; }
    }

    return false;
}
#endif

Public Symbol* Symbol_Create_And_Init_(Symbol* parent, String name, b8 isPerm, Wea_Type* type, b8 allocNew, void* mem) {
    Symbol* symbol = Symbol_Set(Symbol_New_(parent, name, isPerm), type, allocNew, mem);
    return symbol;
}

// C introspection stuff.
Public u32 Get_Fn_Index(String name) {
    forrange (index, gFnsInfo.count) {
        Raw_Fn* fnInfo = &gFnsInfo.E[index];
        if ( Str_Equal(fnInfo->name, name) ) {
            return index;
        }
    }

    LogFatal("Failed to find a c function with the given name");
    return 0;
}

Public Raw_Fn* Get_C_Fn(String name) {
    foreach (Raw_Fn, fnInfo, gFnsInfo) {
        if ( Str_Equal(fnInfo->name, name) ) {
            return fnInfo;
        }
    }

    LogFatal("Failed to find a c function with the given name");
    return 0;
}

Public Wea_Type* Raw_Type_Get(String* name) {
    if ( Str_Equal(*name, S("b8")) ) {
        *name = S("u8");
    } else if ( Str_Equal(*name, S("void*")) ) {
        *name = S("u8*");
    }

    return Type_Get_Unsafe(*name);
}
//

Public void Symbol_Init(void) {
    /* Init global state */ {
        gSymbolState.stateArena = Arena_Create_From_Arena(&gPermArena, MiB(16));

        Array_Create(gSymbolState.symbolPool, KiB(32), &gSymbolState.stateArena);
        Array_Create(gSymbolState.typePool, KiB(16), &gSymbolState.stateArena);
        Array_Create(gSymbolState.fnCallPool, KiB(16), &gSymbolState.stateArena);

        gSymbolState.symbolUnitArena = Arena_Create_From_Arena(&gSymbolState.stateArena, MiB(1));
        gSymbolState.dataArena = Arena_Create_From_Arena(&gSymbolState.stateArena, MiB(4));
        gSymbolState.stringArena = Arena_Create_From_Arena(&gSymbolState.stateArena, MiB(2));
        gSymbolState.argPool = Arena_Create_From_Arena(&gSymbolState.stateArena, MiB(2));

        gSymbolState.root = Array_Push(gSymbolState.symbolPool);
        gSymbolState.root->name = S("__ROOT__");

        DArray_Create(gSymbolState.root->childs, &gSymbolState.symbolUnitArena);
        Array_Create(gSymbolState.rootCode, KiB(4), &gSymbolState.dataArena);

        Array_Create(gSymbolBases, KiB(8), &gSymbolState.stateArena);

        Array_Create(gSymbolScope, KiB(4), &gSymbolState.stateArena);
        Array_Push_C(gSymbolScope, gSymbolState.root);
    }

    /* Create types from c */ {
        Array_String filters = Array_From_Raw_Array(String, ((String[]){ S_Const("Any_Type"), S_Const("Variadic"), }));

        foreach (Raw_Type, typeInfo, cTypeInfo) {
            if ( Type_Name_Is_Primitive(typeInfo->name) ) { continue; }

            (void)Type_Create(typeInfo->name);
        }

        foreach (Raw_Type, typeInfo, cTypeInfo) {
            if ( Array_String_Exists(filters, typeInfo->name) || Type_Name_Is_Primitive(typeInfo->name) ) { continue; }

            Wea_Type* type = Type_Get(typeInfo->name);
            if ( typeInfo->tag == TAG_Bundle ) {
                foreach (Raw_Field, field, typeInfo->fields) {
                    Wea_Type* type = Raw_Type_Get(&field->type);
                    if ( !type ) { goto skip_type; }
                }

                Bundle_Init(type, Array_From_Array_Static(Raw_Field, typeInfo->fields));
            } else if ( typeInfo->tag == TAG_Enum ) {
                Array_String names;
                Array_Create(names, typeInfo->values.count, &gTempArena);

                foreach (Raw_Enum_Value, value, typeInfo->values) {
                    Array_Push_C(names, value->name);
                }

                Enum_Init(type, names);
            }
            skip_type:;
        }
    }

    /* Create default global variables. */ {
        u32 voidDataTypeSize = KiB(32);
        Local_Str(voidDataTypeName, KiB(4));
        Str_From_Fmt(&voidDataTypeName, "d%u", voidDataTypeSize);
        Wea_Type* voidDataType = Type_Get(voidDataTypeName);

        Symbol_Root_Create(S("_"), voidDataType);

        Symbol_Root_Create(S("NULL"), voidDataType);
        Mem_Zero(Symbol_Data_Get(Symbol_Root_Get(S("NULL"))), voidDataTypeSize);

        void* somePtr;

#define Cross_Global(type, name) \
        somePtr = &name; \
        Symbol_Root_Create_And_Init(S(#name), Type_Get(S(#type"*")), true, (type**)&somePtr);

        Cross_Global(Memory_Arena, gTempArena);
        Cross_Global(Symbol_State, gSymbolState);

#undef Cross_Global
    }

    /* Create fns from c */ {
        foreach (Raw_Fn, fnInfo, gFnsInfo) {
            Local_Str(sigName, KiB(4));
            Str_From_Fmt(&sigName, "%s_Sig", fnInfo->name.E);

            Wea_Type* sigType = Type_Create(sigName);
            sigType->tag = TAG_Signature;
            Signature* sig = Poly(Signature, sigType);

            Array_Create(sig->shared, fnInfo->sharedList.count + ( Str_Equal(fnInfo->retType, S("void")) ? 0 : 1 ), &gSymbolState.stateArena);

            if ( !Str_Equal(fnInfo->retType, S("void")) ) {
                Wea_Type* type = Raw_Type_Get(&fnInfo->retType);
                if ( !type ) { goto skip_fn; }
                Array_Push_C(sig->shared, (Shared){ .tag = { .dir = SHARED_TAG_OUT, .isOptional = false }, .name = S("c_out"), .type = type, });
            }

            forrange (index, fnInfo->sharedList.count) {
                Raw_Shared* shared = &fnInfo->sharedList.E[index];
                Wea_Type* type = Raw_Type_Get(&shared->type);

                if ( !type ) { goto skip_fn; }
                Array_Push_C(sig->shared, (Shared){
                    .tag = { .dir = (u32)shared->tag.dir, .isOptional = shared->tag.isOptional },
                    .name = shared->name,
                    .type = type,
                    .optSymbolName = shared->optSymbolName,
                });
            }

            (void)Has_Variadic(sig->shared); // Check that variadic is at the end of the arguments if there is one.

            (void)Symbol_Root_Create_And_Init(fnInfo->name, Type_Get(S("Fn")), true, (&(Fn){
                .name = fnInfo->name,
                .sig  = sigType,
                .code = { 0 },
            }));
            skip_fn:;
        }
    }
}

#if 0
Public void Symbol_Data_Get_Impl(Symbol* symbol, byte** data) {
    if ( Type_Is_Primitive(symbol->type) ) {
        Symbol* dataSymbol = symbol->childs.E[0].symbol;
        Assert( Symbol_Is_Data(dataSymbol) );

        u32 size = Type_Size(symbol->type);

        Mem_Copy_Forward(*data, (byte*)dataSymbol, size);
        *data += size;
    } else {
        foreach (Symbol_Unit, unit, symbol->childs) {
            Symbol_Data_Get_Impl(unit->symbol, data);
        }
    }
}

Public void Symbol_Data_Set_Impl(Symbol* symbol, byte** data, u32* totalSize) {
    if ( Type_Is_Primitive(symbol->type) ) {
        // Symbol_Get(symbol, S("data"));
        Symbol* dataSymbol = symbol->childs.E[0].symbol;
        Assert( Symbol_Is_Data(dataSymbol) );

        u32 size = Type_Size(symbol->type);

        Mem_Copy_Forward((byte*)dataSymbol, *data, size);
        *data += size;
        *totalSize -= size;
    } else {
        foreach (Symbol_Unit, unit, symbol->childs) {
            Symbol_C_Set_Impl(unit->symbol, data, totalSize);
        }
    }
}
#endif
