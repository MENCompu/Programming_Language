#ifndef SYMBOL_H

typedef struct Symbol_Unit Symbol_Unit;
Array(Symbol_Unit);

typedef struct Symbol Symbol;
Array_Ref(Symbol);

typedef struct Type Type;
typedef struct Fn_Call Fn_Call;
typedef struct Fn Fn;

#define Run_Time_Var(type, name) \
    Fn_Call* P(name,FnCall); \
    type name

typedef Array_Symbol_P Variadic;
typedef Symbol* Any_Type;

typedef struct {
    Symbol* sym;
} Sym_Sym;
typedef Sym_Sym Sym_u8_P;

// TODO(JENH): Mix "Symbol" and "Symbol_Unit" types into one "Symbol" type and
//             try to separate thier data using the "data" pointer field of the symbol's symbol.
typedef struct Symbol {
    // moving data.
    Symbol* outer;
    Symbol* inner;

    Symbol* parent;
    //

    Type* type;
    b8 isPerm;

    b8 isModule;
    String name;

    u8* data;

    b8 lazyCreation;
    Array_Symbol_Unit childs;
} Symbol;
Array(Symbol);
typedef Symbol* Symbol_P;

typedef struct Symbol_Unit {
    String name;
    Symbol* symbol;
    b8 using;

    Symbol_Unit* prevUnit;

    Symbol* fnCall;
    Symbol* symbolFnCall;
} Symbol_Unit;

#define WEA_TYPES
#include "types.h"

typedef struct Fn_Call {
    Symbol* fnFnCall;
    Fn* fn;

    Symbol* args;

    b8 isResolved;
} Fn_Call;
Array(Fn_Call);
Array_Ref(Fn_Call);
typedef Fn_Call* Fn_Call_P;

typedef struct {
    Symbol* locals;
    Array_Fn_Call_P code;
} Code_Block;
Array(Code_Block);

typedef struct Fn {
    String name;
    Type* sig;
    Code_Block codeBlock;
} Fn;
Array(Fn);

typedef struct {
    Symbol* root;
    Symbol* lits;
    Symbol* types;
    Symbol* fnCalls;

    Array_Symbol symbolPool;
    //Array_Fn_Call fnCallPool;

    Memory_Arena symbolUnitArena;
    Memory_Arena dataArena;
    Memory_Arena stringArena;

    Memory_Arena stateArena;
} Symbol_State;

Private Global Symbol_State gSymbolState;

// Functions
#define Symbol_Scope_Create(name, type) Symbol_Create(Scope_Top(0), name, type)
Public Symbol* Symbol_Create(Symbol* parent, String name, Type* type);

Public Symbol* Symbol_Scope_Get(String name);
Public Symbol* Symbol_Get(Symbol* parent, String name);
Public Symbol* Symbol_Get_Unsafe(Symbol* parent, String name);

Public Symbol* Addr(Symbol* symbol);

Public Symbol* Symbol_Set(Symbol* symbol, Type* type, b8 allocNew, void* mem);

#define Symbol_Unit_Get_Unsafe(parent, name) Symbol_Unit_Get_Unsafe_(parent, name, false)
Public Symbol_Unit* Symbol_Unit_Get_Unsafe_(Symbol* parent, String name, b8 inExternLocals);
Public Symbol_Unit* Symbol_Unit_Get(Symbol* parent, String name);

#define Symbol_Push_Child_C(symbol, ...) Symbol_Push_Child_C_(symbol, (Symbol_Unit){ __VA_ARGS__ })
Public Symbol_Unit* Symbol_Push_Child_C_(Symbol* symbol, Symbol_Unit unit);

Public Symbol* Scope_Top(u32 index);
Public Symbol* Module_Top(void);

Public u8* Symbol_Data_Create(u32 size);

#define Symbol_New(parent, name) Symbol_New_(parent, name, true)
Public Symbol* Symbol_New_(Symbol* parent, String name, b8 isPerm);

Public void Symbol_Set_Mem(Symbol* symbol, u8* mem);
Public void Symbol_Create_From_Type(Symbol* symbol, Type* type);

//#define Symbol_Root_Create_And_Init(name, type, allocNew, mem) Symbol_Create_And_Init_(gSymbolState.root, name, true, type, allocNew, mem)
#define Symbol_Scope_Create_And_Init(name, type, allocNew, mem) Symbol_Create_And_Init_(Scope_Top(0), name, true, type, allocNew, mem)
#define Symbol_Create_And_Init(parent, name, type, allocNew, mem) Symbol_Create_And_Init_(parent, name, true, type, allocNew, mem)
Public Symbol* Symbol_Create_And_Init_(Symbol* parent, String name, b8 isPerm, Type* type, b8 allocNew, void* mem);

#define D(type, symbol) ((type*)Symbol_Data_Get_And_Check(Type_All_Get(S(#type)), symbol))
#define W(type, symbol) D(type, (symbol).sym)

Public void* Symbol_Data_Get(Symbol* symbol);
Public void* Symbol_Data_Get_And_Check(Type* type, Symbol* symbol);

Public void Fn_Create(String name, Param ret, Array_Param params, Array_Fn_Call_P code);

#endif // SYMBOL_H
