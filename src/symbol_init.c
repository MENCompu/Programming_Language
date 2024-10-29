typedef struct Temp_Symbol Temp_Symbol;
Array(Temp_Symbol);
Array_Ref(Temp_Symbol);

typedef struct Temp_Symbol {
    String name;

    b8 isInstance;
    union {
        struct {
            Temp_Symbol* base;
            void* data;
        };
        Array_Temp_Symbol_P childs;
    };

    Symbol* symbol;
} Temp_Symbol;

Private Global Array_Temp_Symbol tempSymbols;
Private Global Temp_Symbol* tempSymbolRoot;

#define UNDEF_SYMBOL_DATA 0

Public Temp_Symbol* Temp_Symbol_Create(String name, b8 isInstance) {
    Temp_Symbol* retSymbol = Array_Push(tempSymbols);

    retSymbol->name = name;
    retSymbol->data = UNDEF_SYMBOL_DATA;
    retSymbol->isInstance = isInstance;

    Array_Create(retSymbol->symbol->childs, KiB(2), &gTempArena);

    retSymbol->symbol = Symbol_New();
    if ( !retSymbol->isInstance ) {
        Symbol_Push_Child(retSymbol->symbol, Symbol_New()); // symName.
        Symbol_Push_Child(retSymbol->symbol, Symbol_New()); // symData.
    }

    return retSymbol;
}

Public Temp_Symbol* Temp_Symbol_Get(String name) {
    foreach (Temp_Symbol, entry, tempSymbols) {
        if ( Str_Equal(entry->name, name) ) {
            return entry;
        }
    }

    b8 pointerOrInt = ( ( name.E[name.count-1] == '*' ) ||
                        ( ( name.E[0] == 's' || name.E[0] == 'u' ) && Str_Is_Integer(Str_Skip_Begin(name, 1) ) ) );

    return Temp_Symbol_Create(name, pointerOrInt);
}

typedef struct {
    String typeName;
    String name;
} Temp_Field;
Array(Temp_Field);

Public void Temp_Symbol_Init(Temp_Symbol* tempSymbol, Array_Temp_Field fields) {
    foreach (Temp_Field, fieldData, fields) {
        Temp_Symbol* field = Temp_Symbol_Create(fieldData->name, true);
        field->base = Temp_Symbol_Get(fieldData->typeName);

        Array_Push_C(tempSymbol->childs, field);
    }
}

#if 0
Public void Symbol_Add_Name_Child(Symbol* symbol, String name) {
    Symbol* symName = Symbol_New();

    Symbol_Push_Child(symNameSym, symNameSym);
    Symbol_Push_Child(symNameSym, Symbol_Alloc_Data(sizeof(String)));

    Symbol_Push_Child(symbol, symName);
}
#endif

#if 0
Public Symbol* Symbol_Data(void) {
    return (Symbol*)( SYMBOL_DATA_POINTER_MASK );
}
#endif

Public Symbol* Symbol_Alloc_Data(u32 size) {
    return (Symbol*)( SYMBOL_DATA_POINTER_MASK | (u64)Arena_Alloc_Mem(&gSymbolState.dataArena, size) );
}

Public Symbol* Symbol_Copy(Symbol* symbol) {
    Symbol* retSymbol = Symbol_New();

    foreach (Symbol*, entryRef, symbol->childs) {
        Symbol_Push_Child(retSymbol, Symbol_Copy(*entryRef));
    }

    return retSymbol;
}

Public void Symbol_Init(void) {
    Arena_Create_From_Arena(&gSymbolState.dataArena, &dyMem.arena, MiB(32));
    Arena_Create_From_Arena(&gSymbolState.symPtrArena, &dyMem.arena, MiB(1));

    Array_Create(tempSymbols, KiB(4), &gTempArena);
    tempSymbolRoot = Temp_Symbol_Create(S("__ROOT__"), false);

    /* initialize temp symbols. */ {
        Array_Temp_Field fields;
        Array_Create(fields, KiB(16), &gTempArena);

        #define S_Field(type, name) \
            Array_Push_C(fields, (Temp_Field){ .typeName=S(#type), .name=S(#name) });

        #define S_Prim_Init(symbolName, ...) \
            Temp_Symbol_Init(Temp_Symbol_Get(S(#symbolName)), fields);
        SYMBOL_LIST
        #undef S_Prim_Init
    }

    /* create integer and pointer symbols. */ {
        foreach (Temp_Symbol, entry, tempSymbols) {
            String name = entry->name;
            if ( name.E[name.count-1] == '*') {
                entry->base = Temp_Symbol_Get(S("Pointer"));

                Symbol** pointeeType = Arena_Alloc_Type(&gTempArena, Symbol*);
                (*pointeeType) = Temp_Symbol_Get(Str_Skip_End(name, 1))->symbol;

                entry->data = pointeeType;
            } else if ( ( name.E[0] == 's' || name.E[0] == 'u' ) && Str_Is_Integer(Str_Skip_Begin(name, 1)) ) {
                entry->base = Temp_Symbol_Get(S("Integer"));

                Integer* integer = Arena_Alloc_Type(&gTempArena, Integer);

                integer->size = (u32)Str_To_S64(Str_Skip_Begin(name, 1));
                integer->isSigned = ( name.E[0] == 's' );

                entry->data = integer;
            }
        }
    }

    Assert( gSymbolState.symbolPool.count == tempSymbols.count );

    Symbol* symData = Symbol_New();
    Symbol* symName = Symbol_New();

    /* temp child symbols to perm */ {
        foreach (Temp_Symbol, temp, tempSymbols) {
            foreach (Temp_Symbol*, tempRef, temp->childs) {
                Symbol_Push_Child(temp->symbol, (*tempRef)->symbol);
            }
        }
    }

#if 0
    /* Create symName symbol */ {
        Symbol_Push_Child(symName, symName);
        Symbol_Push_Child(symData, symName);


        //Symbol_Push_Child(symName, Symbol_Data());
    }
#endif

    /* Create symName symbol */ {
        Symbol_Push_Child(symName, symName);
        // Symbol_Alloc_Data(sizeof(String))

        Symbol* count = Symbol_New();
        Symbol_Push_Child(symName, count);

        Symbol* E = Symbol_New();
        Symbol_Push_Child(symName, E);

        Temp_Symbol* stringSym = Temp_Symbol_Get(S("String"));
    }
}
