#ifndef SYMBOL_H

typedef struct Enum_Val {
    String name;
    u32 val;
} Enum_Val;
Array(Enum_Val);

typedef enum {
    FN_FLAGS_MACRO,
    FN_FLAGS_RUNTIME,
    FN_FLAGS_COMPTIME,
} Fn_Flags;

typedef struct Fn Fn;
typedef struct Type_Fn_Table Type_Fn_Table;
typedef struct Unresolved_Fn_Call Unresolved_Fn_Call;

typedef struct Symbol Symbol;
Array(Symbol);
Array_Ref(Symbol);

#if 0
typedef struct {
    Symbol** old;
    Symbol*  new;
} Symbol_Change;
Array(Symbol_Change);
Array_Symbol_Change changes;
#endif

typedef struct {
    b8 deleted;
    s32v2 pos;
} Edit_Data;

// TODO(JENH): Put the base symbol inside the childs.
typedef struct Symbol {
    Edit_Data edit;

    Array_Symbol_P childs;
} Symbol;

#define SYMBOL_DATA_POINTER_MASK 0x8000000000000000

#define SYMBOL_DATA_LIST \
    X(u8) \
    X(u32) \
    X(String) \

typedef enum {
    //SYMBOL_DATA_TAG_bit,

#define X(type) \
    P(SYMBOL_DATA_TAG_, type),
SYMBOL_DATA_LIST
#undef X

    SYMBOL_DATA_TAG_COUNT,
} Symbol_Data_Tag;

typedef struct {
    Edit_Data edit;

    Symbol_Data_Tag tag;
    byte* data;
} Symbol_Data;
Array(Symbol_Data);

typedef struct {
    Symbol_Data_Tag tag;
    u32 size;
    String typeName;
} Symbol_Data_Info;
Slice(Symbol_Data_Info);

Private Global Symbol_Data_Info gSymbolDataInfosData[SYMBOL_DATA_TAG_COUNT];
Public  Global Slice_Symbol_Data_Info gSymbolDataInfos;

typedef struct {
    Symbol* root;

    Array_Symbol_Data symbolDataPool;
    Array_Symbol symbolPool;
    Memory_Arena symPtrArena;

    Memory_Arena dataArena;
} Symbol_State;

Private Global Symbol_State gSymbolState;

Public byte* Symbol_Data_Ptr_Get(Symbol* ptr) {
    return (byte*)( (u64)ptr & ~SYMBOL_DATA_POINTER_MASK );
}

Public b8 Is_Symbol_Data_Ptr(Symbol* ptr) {
    return (b8)( ( (u64)ptr & SYMBOL_DATA_POINTER_MASK ) != 0 );
}

Public Symbol* Symbol_Data_Ptr(void* symbolData) {
    return (Symbol*)( (u64)symbolData | SYMBOL_DATA_POINTER_MASK );
}

Public Symbol* Symbol_New(s32v2 pos) {
    Symbol* newSymbol = Array_Push(gSymbolState.symbolPool);
    DArray_Create(newSymbol->childs, &gSymbolState.symPtrArena);

    newSymbol->edit = (Edit_Data){ .deleted = false, .pos = pos };

    return newSymbol;
}

Public void Symbol_Data_Init(void) {
    gSymbolDataInfos = (Slice_Symbol_Data_Info){ .count = Array_Count(gSymbolDataInfosData), .E = gSymbolDataInfosData };

    u32 counter = 0;
    #define X(type) \
        gSymbolDataInfos.E[counter++] = (Symbol_Data_Info){ .tag = P(SYMBOL_DATA_TAG_, type), .size = sizeof(type), .typeName = S(#type) };
    SYMBOL_DATA_LIST
    #undef X
}

Public Symbol_Data* Symbol_Data_Create(s32v2 pos, Symbol_Data_Tag tag, void* data) {
    Symbol_Data* symbolData = Array_Push(gSymbolState.symbolDataPool);

    u32 size = gSymbolDataInfos.E[tag].size;
    symbolData->data = Arena_Alloc_Mem(&gSymbolState.dataArena, size);

    if ( data ) {
        Mem_Copy_Forward(symbolData->data, data, size);
    } else {
        Mem_Zero(symbolData->data, size);
    }

    symbolData->edit = (Edit_Data){ .deleted = false, .pos = pos };
    symbolData->tag = tag;

    return symbolData;
}

#define Symbol_Push_Child(symbol, child) DArray_Push_C(&gSymbolState.symPtrArena, &symbol->childs, child)

#if 0
#define AT(type, address) (type)Get_Pointer(address, S(#type))
Public void* Get_Pointer(Address address, String typeName) {
    Symbol symbol = Get_Symbol(address);
    Symbol typeSymbol = Get_Symbol(symbol.type);
    Assert( Str_Equal(typeSymbol.name, typeName) );
    return symbol.data;
}

Public Address Get_Child(Address parent, String childName) {
    Symbol parentSymbol = Get_Symbol(parent);
    return ((*AT(Type_Fn_Table**, parentSymbol.type))->Get_Child(parentSymbol, childName));
}

#define CN(parent, childName) Get_Child(parent, childName)
#define C(parent, childName) CN(parent, S(#childName))

#define CNR(childName) CN(gSymbolState.rootAddress, childName)
#define CR(childName) CNR(S(#childName))

#define DN(parent, childName) Get_Symbol(CN(parent, childName)).data
#define D(parent, childName) DN(parent, S(#childName))

#define DNR(childName) DN(gSymbolState.rootAddress, childName)
#define DR(childName)  DNR(S(#childName))

#define DTN(type, parent, childName) (type*)Symbol_Get_Child_Safe(S(#type), parent, childName)
#define DT(type, parent, childName) DTN(type, parent, S(#childName))

Public void* Symbol_Get_Child_Safe(String typeName, Address* parent, String childName) {
#if 0
    Address retSymbol = CN(parent, childName);
    Assert( (u8*)retSymbol.type == DNR(typeName) );
    return retSymbol.data;
#else
    return 0;
#endif
}

Public Address Type_Get(String name);

// Symbol Creation functions.
#define M_Str(str) #str
#define Symbol_Create_G(type, name, ...) \
    Symbol_Create_G_(S(#type), name, &(type){ __VA_ARGS__ });

Public Address Symbol_Create_G_(String typeName, String name, void* data);

Public Address Symbol_Create(u8* type, String name, void* data) {
#if 0
    Address symbol;

    symbol.name = name;
    symbol.type = (Type_Fn_Table**)type;
    symbol.data = Arena_Alloc_Mem(&gSymbolState.dataArena, Get_Size(type));
    Mem_Copy_Forward(symbol.data, data, Get_Size(type));

    return symbol;
#else
    return (Address){ 0 };
#endif
}

Public void Symbol_Create_Ptr(Address* symbol, u8* type, String name, void* data) {
    *symbol = Symbol_Create(type, name, data);
}

Public Unresolved_Fn_Call* Symbol_Create_Fn_Call(void) {
    Unresolved_Fn_Call* ret = Array_Push(gSymbolState.unresolvedCode);
    return ret;
}

#define Type_Symbol_Root_Get_By_Data(data) Type_Symbol_Get_By_Data(gSymbolState.root, data)
Public Symbol* Type_Symbol_Get_By_Data(Symbol* parent, Symbol* data);

// Get Functions
Public Symbol* Symbol_Get(Symbol* parent, Symbol* childType, String childName);

#define SGN(parent, childName) Symbol_Get_Name((Symbol*)parent, S(#childName))
#define Symbol_Get_Name(parent, name) Symbol_Get(parent, 0, name)

#define Symbol_Root_Get(type, name) Symbol_Get(gSymbolState.root, type, name)
#define Symbol_Root_Get_Name(name) Symbol_Get(gSymbolState.root, 0, name)
#endif

#endif // SYMBOL_H
