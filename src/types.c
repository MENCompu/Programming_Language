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

Public b8 Type_Name_Is_WArray(String typeName) {
    return (b8)( Str_Begin_Equal(typeName, S("WArray_")) );
}

Public b8 Type_Name_Is_Sym(String typeName) {
    return (b8)( Str_Begin_Equal(typeName, S("Sym_")) );
}

Public b8 Type_Name_Is_Primitive(String typeName) {
    return Type_Name_Is_Array(typeName)   || Type_Name_Is_Pointer(typeName)  ||
           Type_Name_Is_Integer(typeName) || Type_Name_Is_Raw_Data(typeName);
}

Public b8 Type_Is_Primitive(Type* type) {
    return (b8)( type->tag == TAG_Integer || type->tag == TAG_Pointer || type->tag == TAG_Raw_Data || type->tag == TAG_Enum );
}

Public b8 Type_Is_Not_Primitive(Type* type) {
    return (b8)( type->tag == TAG_Bundle || type->tag == TAG_Array || type->tag == TAG_Union );
}

Public Type* Type_Create(Symbol* parent, String name) {
    Symbol* weaType = Symbol_Create(gSymbolState.types, name, Type_Get(S("Type")));
    Symbol_Unit* unit = Symbol_Unit_Get(gSymbolState.types, name);

    if ( parent != gSymbolState.types ) {
        (void)Symbol_Push_Child_C(parent, name, weaType);
    }

    Type* type = D(Type, weaType);
    type->name = unit->name;

    return type;
}

#define PTR_SIZE 8

Public u32 Type_Size(Type* type) {
    if ( Type_Name_Is_Sym(type->name) || Str_Equal(type->name, S("Enum")) ) {
        return sizeof(Symbol*);
    }

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
            return Type_Size(Type_All_Get(S("u32")));
        } break;

        case TAG_Array: {
            return sizeof(Array_void);
        } break;

        case TAG_WArray: {
            return 0; // NOTE(JENH): Its size should be dynamic.
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

        case TAG_Null:
        NO_DEFAULT
    }

    return 0;
}

Public b8 Type_Check(Type* a, Type* b) {
    if ( a == Type_All_Get(S("Any_Type")) || b == Type_All_Get(S("Any_Type")) ) { return true; }

    if ( ( b->tag == TAG_Enum && Str_Equal(a->name, S("u32")) ) || b->tag == TAG_Raw_Data ) {
        Swap(&a, &b, Type*);
    }

    if ( a->tag == TAG_Enum && Str_Equal(b->name, S("u32")) ) {
        return true;
    } else if ( a->tag == TAG_Integer && b->tag == TAG_Integer ) {
        return true;
    } else if ( a->tag == TAG_Raw_Data ) {
        return ( Type_Size(a) >= Type_Size(b) );
    } else {
        return ( Str_Equal(a->name, b->name) );
    }
}

Public void Bundle_Init(Type* type, Array_Raw_Field rawFields) {
    type->tag = TAG_Bundle;

    Array_Field fields;
    Array_Create(fields, rawFields.count, &gSymbolState.stateArena);

    foreach (Raw_Field, rawField, rawFields) {
        Array_Push_C(fields, (Field){
            .type = Type_Get(rawField->type),
            .name = Str_Alloc_And_Copy(&gSymbolState.stringArena, rawField->name, true),
        });
    }

    Poly(Bundle, type)->fields = fields;
}

Public Type* Bundle_Create(String name, Array_Raw_Field rawFields) {
    Type* type = Type_Create(Module_Top(), name);
    Bundle_Init(type, rawFields);
    return type;
}

#if 0
Public String Type_Name_To_WArray(String typeName) {
    String out = Str_Alloc(&gSymbolState.stringArena, KiB(4));
    Str_From_Fmt(&out, "Array_%s", typeName.E);
    return out;
}
#endif

#if 0
Public void GBundle_Create(Array_String typeNames, String bundleName, Fn_Call* childsDefinition) {
    Local_Array(Param, params, KiB(4));

    foreach (String, typeName, typeNames) {
        Array_Push_C(params, (Param){ S("String"), *typeName });
    }

    Local_Str(codeText, KiB(4));
    Str_From_Fmt(&codeText,
        "(Let \"String\" \"typeName\" (WStr_From_Fmt \"%s_%%\" T));"
        "(Let \"Type*\"  \"type\");"
        ""
        "(If (Type_Exists typeName) {"
        "   (Type_Get type typeName);"
        "} {"
        "   (Bundle_Create type typeName);"
        "});"
        ""
        "(Transfer out (Type_Get_Name type));"
    , bundleName.E);

    Array_Fn_Call_P code = Parse_Text(codeText, WEA_FILE_TAG_LISP);

    foreach (Fn_Call*, fnCallRef, code) {
        Fn_Call_Resolve_Rec(*fnCallRef, 0);
    }

    DArray_Push_C(&gSymbolState.symbolUnitArena,
                  &D(Array_Fn_Call_P, code.E[2]->args->childs.E[2].symbol)->E[0]->args->childs,
                  (Symbol_Unit){ .fnCall = childsDefinition });

    Fn_Create(bundleName, (Param){ S("String"), S("out") }, params, code);
};
#endif

Public void Union_Init(Type* type, Array_Raw_Field rawFields) {
    type->tag = TAG_Union;

    Array_Field fields;
    Array_Create(fields, rawFields.count, &gSymbolState.stateArena);

    foreach (Raw_Field, rawField, rawFields) {
        Array_Push_C(fields, (Field){
            .type = Type_Get(rawField->type),
            .name = Str_Alloc_And_Copy(&gSymbolState.stringArena, rawField->name, true),
        });
    }

    Poly(Union, type)->fields = fields;
}

Public void Union_Create(String name, Array_Raw_Field rawFields) {
    Union_Init(Type_Create(Module_Top(), name), rawFields);
}

Public Symbol* Symbol_Enum_Values(Type* type) {
    return Symbol_Get(Symbol_Scope_Get(type->name), S("varEnum"));
}

Public void Enum_Init(Type* type, Array_String names) {
    type->tag = TAG_Enum;

    Symbol* values = Symbol_Enum_Values(type);

    u32 counter = 0;
    foreach (String, name, names) {
        Symbol_Create_And_Init(values, *name, Type_Get(S("u32")), true, &counter);
        ++counter;
    }

    Poly(Enum, type)->values = values;
}

Public void Enum_Create(String name, Array_String names) {
    Enum_Init(Type_Create(Module_Top(), name), names);
}

Public String Pointee_Name(String ptrName) {
    return Str_Alloc_And_Copy(&gTempArena, Str_Skip_End(ptrName, 1), true);
}

Public void Pointer_Init(Type* type, Type* pointee) {
    type->tag = TAG_Pointer;
    *Poly(Pointer, type) = (Pointer){
        .type = pointee,
    };
}

Public void Integer_Init_By_Name(Type* type, String name) {
    type->tag = TAG_Integer;
    *Poly(Integer, type) = (Integer){
        .size = (u32)Str_To_S64(Str_Skip_Begin(name, 1)) / BYTE_SIZE,
        .sign = ( name.E[0] == 's' ),
    };
}

Public void Raw_Data_Init_By_Name(Type* type, String name) {
    type->tag = TAG_Raw_Data;
    *Poly(Raw_Data, type) = (Raw_Data){
        .size = (u32)Str_To_S64(Str_Skip_Begin(name, 1)) / BYTE_SIZE,
    };
}

Public void Array_Init(Type* type, Type* elemType) {
    type->tag = TAG_Array;

    *Poly(Array, type) = (Array){
        .elemType = elemType,
    };
}

Public void WArray_Init(Type* type, Type* elemType) {
    type->tag = TAG_WArray;

    *Poly(WArray, type) = (WArray){
        .elemType = elemType,
    };
}

Public String Elem_Name(String name) {
    String out = Str_Alloc(&gTempArena, KiB(4));
    Str_Copy(&out, Str_Skip_Begin(name, S("Array_").count));

    if ( Str_End_Equal(out, S("_P")) ) {
        out = Str_Skip_End(out, S("_P").count);
        out.E[(++out.count) - 1] = '*';
        Str_Null_Terminate(&out);
    }

    return out;
}

Public String Handle_Pointer(String name) {
    Local_Str(out, KiB(4));

    foreach (u8, ch, name) {
        if ( Chars_Ptr_Equal(ch, S("_P")) ) {
            Str_Push_Char(&out, '*');
            ch += S("_P").count;
        }
    }

    return out;
}

Public String WElem_Name(String name) {
    return Handle_Pointer(Str_Skip_Begin(name, S("WArray_").count));
}

Public Type* Type_Primitive_Create_By_Name(Symbol* parent, String name) {
    Type* type = 0;

    if ( Type_Name_Is_Pointer(name) ) {
        String pointeeName = Pointee_Name(name);
        //if ( Type_Name_Is_Pointer(pointeeName) ) { return 0; }
        Type* pointee = Type_Get_Unsafe(parent, pointeeName);
        if ( !pointee ) { return 0; }
        type = Type_Create(parent, name);
        Pointer_Init(type, pointee);
    } else if ( Type_Name_Is_Integer(name) ) {
        type = Type_Create(parent, name);
        Integer_Init_By_Name(type, name);
    } else if ( Type_Name_Is_Raw_Data(name) ) {
        type = Type_Create(parent, name);
        Raw_Data_Init_By_Name(type, name);
    } else if ( Type_Name_Is_Array(name) ) {
        Type* elemType = Type_Get_Unsafe(parent, Elem_Name(name));
        if ( !elemType ) { return 0; }
        type = Type_Create(parent, name);
        Array_Init(type, elemType);
    } else if ( Type_Name_Is_WArray(name) ) {
        Type* elemType = Type_Get_Unsafe(parent, WElem_Name(name));
        if ( !elemType ) { return 0; }
        type = Type_Create(parent, name);
        WArray_Init(type, elemType);
    }

    return type;
}

Public Type* Type_Get_Unsafe(Symbol* parent, String name) {
    Symbol* weaType = Symbol_Get_Unsafe(parent, name);

    if ( weaType ) {
        Assert( Str_Equal(weaType->type->name, S("Type")) );
        return (Type*)Symbol_Data_Get(weaType);
    }

    return Type_Primitive_Create_By_Name(parent, name);
}

Public Type* Type_Get(String name) {
    Scope_Iter (scope) {
        Type* type = Type_Get_Unsafe(scope, name);
        if ( type ) { return type; }
    }

    LogFatal("Couldn't find the type");
    return 0;
}

Public Type* Type_All_Get(String name) {
    //Dbg_If( Str_Equal(name, S("Fn_Call**")) );
    Type* type = Type_Get_Unsafe(gSymbolState.types, name);
    Assert( type );
    return type;
}

Public b8 Type_Exists(String name) {
    Scope_Iter (scope) {
        if ( Type_Get_Unsafe(scope, name) ) { return true; }
    }

    return false;
}

Public Type* Type_Ptr_Get(Type* type) {
    Local_Str(typePtrName, KiB(4));
    Str_From_Fmt(&typePtrName, "%s*", type->name.E);

    return Type_Get(typePtrName);
}

#if 1
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

// Temp types for the creation of the "Type" type.
Public Type* Type_Create_Temp(String name) {
    Symbol* symbol = Array_Push(gSymbolState.symbolPool);

    symbol->isPerm = true;
    DArray_Create(symbol->childs, &gSymbolState.symbolUnitArena);

    Symbol_Unit* unit = Symbol_Push_Child_C(gSymbolState.types, name, symbol);

    if ( Scope_Top(0) != gSymbolState.types ) {
        (void)Symbol_Push_Child_C(Scope_Top(0), name, symbol);
    }

    symbol->data = Symbol_Data_Create(sizeof(Type));
    Type* type = (Type*)symbol->data;
    type->name = unit->name;

    symbol->type = ( Str_Equal(name, S("Type")) ) ? type : Type_Get((S("Type")));

    return (Type*)symbol->data;
}

Public Type* Type_Get_Temp(String name);

Public void Type_Init_Temp(Type* type, String name) {
    if ( Type_Name_Is_Primitive(name) ) {
        if ( Type_Name_Is_Pointer(name) ) {
            Pointer_Init(type, Type_Get_Temp(Pointee_Name(name)));
        } else if ( Type_Name_Is_Integer(name) ) {
            Integer_Init_By_Name(type, name);
        } else if ( Type_Name_Is_Raw_Data(name) ) {
            Raw_Data_Init_By_Name(type, name);
        } else if ( Type_Name_Is_Array(name) ) {
            Type* elemType = Type_Get_Temp(Elem_Name(name));

            Local_Str(elemTypePtrName, KiB(4));
            Str_From_Fmt(&elemTypePtrName, "%s*", elemType->name.E);
            (void)Type_Get_Temp(elemTypePtrName);

            Array_Init(type, elemType);
        }
    } else {
        Raw_Type* rawType = Raw_Type_Get(name);

        if (0) {
        } else if ( rawType->tag == TAG_Bundle || rawType->tag == TAG_Union ) {
            foreach (Raw_Field, rawField, rawType->fields) {
                (void)Type_Get_Temp(rawField->type);
            }

            Bundle_Init(type, Array_From_Array_Static(Raw_Field, rawType->fields));
        } else if ( rawType->tag == TAG_Enum ) {
            (void)Type_Get_Temp(S("u32"));

            type->tag = TAG_Enum;
        }
    }
}

Public Type* Type_Get_Temp(String name) {
    Symbol* symbol = Symbol_Get_Unsafe(Scope_Top(0), name);
    if ( symbol ) { return (Type*)symbol->data; }

    Type* type = Type_Create_Temp(name);
    Type_Init_Temp(type, name);

    return type;
}
//
