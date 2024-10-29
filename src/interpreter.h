typedef struct {
    Symbol* wFnCall;
    Code_Block* codeBlock;

    u32 pc;
} Code;
Array(Code);

typedef struct {
    Array_Code codeStack;
} Interpreter_State;

Private Global Interpreter_State gInterState;

Public Symbol* Scope_Iter_Advance(Code* code) {
    if ( code->codeBlock ) {
        return code->codeBlock->locals;
    } else {
        return 0;
    }
}

#define Scope_Iter(name) \
    Code* __code = &gInterState.codeStack.E[0]; \
    for (Symbol* name = Scope_Iter_Advance(__code); __code <= &Array_Index_Back(gInterState.codeStack, 0); ++__code, name = Scope_Iter_Advance(__code))

// Functions
Public void Interpret_Begin(void);
Public void Interpret_End(void);

Public Symbol* Get_Out_Symbol(Array_Shared sharedList, Symbol* args);
Public Symbol* Get_Out_Symbol_Fn_Call(Fn_Call* fnCall);

Public void Fn_Call_Resolve(Symbol* wFnCall, Symbol_Unit* outArg);
Public void Fn_Call_Resolve_Rec(Symbol* wFnCall, Symbol_Unit* outArg);

#if 0
Public void Render_Text_Fn_Call(Fn_Call* fnCall);
Public String Fn_Call_To_Text(Fn_Call* fnCall);
//Public String Args_To_Text(Fn_Call* fnCall);
#endif

Public Symbol* Interpret(Code code, Symbol_Unit* outArg);
Public Symbol* Interpret_Code(Array_Fn_Call_P code);
Public Symbol* Interpret_Code_Block(Code_Block* codeBlock);
Public Symbol* Interpret_Fn_Call(Symbol* wFnCall);
