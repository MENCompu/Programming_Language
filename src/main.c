/*
 * WEA ROADMAP.
 *  - [ ] Base language.
 *      - [?] Implement symbol mapped arrays.
 *      - [x] Allow the user to interact with the internal representation of the variable ( can modify the "Symbol" type ).
 *      - [-] Make "Go back" symbol action.
 *      - [ ] Change pointer symbol representation to be similar to static-addressed variables.
 *          - [x] Use the childs same representation.
 *          - [x] Make pointer to use "Go back" symbol action for the "Dereference" and "Get Address" operations.
 *          - [ ] Make the "data" symbol's field to already have the pointer type of its representation????
 *                i.e. for the var "u32 number", the "data" symbol's field should be "u32* data".
 *      - [ ] Implement simple generics, probably in the zig way.
 *      - [ ] Improve the interpreter.
 *          - [x] Update it to work with multiple codeBlocks
 *          - [?] Allow recursion.
 *          - [ ] Add labels to use with gotos utilising the Symbol_Unit's name of the elements of the Fn_Call array.
 *      - [ ] Fix Macros (functions that take code as arguments) by allowing to insert that stored code as any literal function call.
 *            i.e. of what the parser actually do
 *             (Array_Fn_Call_P){
 *                 .fnCall = [Output "some funtion call."]
 *                 .fnCall = [Let "Fn_Call*" "fnCallVar"]
 *                 .symbol = fnCallVar
 *             }
 *      - [ ] Implement methods.
 *  - [ ] '.wea' files visualization.
 *  - [ ] Wea Debugger.
 *      - [ ] Add steping to the next function call.
 *  - [ ] Compiler Engine.
 *  - [ ] Compile C to a .wea file.
*/

/*
 * // Variables should have different allocators. And there should be an allocator defined for a given codeBlock
 *    (including the global one, that is going to be different to the user defined function's codeBlock).
 *
 * bundle {
 *     static u32 hola;
 *     codeBlock.alloc u32 chao;
 * } Type;
 *
 * Type* hello1;
 * Type* hello2;
 * hello1 = hello2;
 *
 * fn My_Fn() {
 *    Type stackPene;
 * }
 *
 * // Variables can have references to symbol too.
 * var1;
 * var2;
 *
 * Create_Symbol(var1, Fn u32 hola(self) { return 0; });
 * ^var2 = ^var1;
 *
 * var1.hola(); // == 0
 * var2.hola(); // == 0
 *
 * Create_Symbol(var1, Fn u32 chao(self) { return 1; });
 * var1.chao(); // == 1
 * var2.chao(); // ERROR.
 *
 */

#include <third_party.h>

#include <meta_utils.h>

#define P2(a, b, c) a##b##c
Meta_Mark(My_Code,Begin)

Meta_Mark(Prim_Fn,Begin)

#pragma pack(push, 1)
#include <menc_libs.h>

#define BUILD_64
#include <file_format_utils.h>
#include <parse.h>

#include <menc_time.h>
#include <menc_time.c>

#include <file_system.h>
#include <file_system.c>

#include "symbol.h"
#include "wea_parser.h"
#include "base_parser.h"
#include "interpreter.h"

#include "introspection.h"
#include "introspection.c"

Public void Let(String typeName, String name, /*OPT NULL*/ Any_Type init);
Public void Let_C(String typeName, String name, Symbol* scope);

Public void Using(String name);

#include "types.c"
#include "symbol.c"

#include "wea_parser.c"
#include "base_parser.c"

#include "prim_fns.c"

Meta_Mark(Prim_Fn,End)

#define _Var(name, beginAt) \
    Local_Array(Symbol_P, name, KiB(4)); \
    do { \
        forrange (index, __args->childs.count - beginAt) { \
            Array_Push_C(name, __args->childs.E[beginAt].symbol); \
        } \
    } while( 0 );

#define _Any(name) Any_Type name = Symbol_Get(__args, S(#name))
#define _Out(type) type* c_out = D(type, Symbol_Get(__args, S("c_out")))
#define _Arg(type, name) type name = *D(type, Symbol_Get(__args, S(#name)))
#include "c_interpreter.c"
#undef _Arg
#undef _Out
#undef _Any
#undef _Var

#include "interpreter.c"

int main(int argc, char* argv[]) {
    cTypeInfo  = Slice_From_Raw_Array(Raw_Type, cTypeInfoBuffer);
    gFnsInfo   = Slice_From_Raw_Array(Raw_Fn, gFnInfoBuffer);
    gSymbolFns = Slice_From_Raw_Array(String, gSymbolFnsBuffer);

    Mem_Init();

    gPermArena = Arena_Create_From_Arena(&dyMem.arena, MiB(64));
    gTempArena = Arena_Create_From_Arena(&dyMem.arena, MiB(64));

    Array_Create(gInterState.codeStack, KiB(4), &gPermArena);

    Symbol_Init();

    Module_Create_By_File(S("P:\\src\\main.lisp.wea"));

    Arena_Clear(&gTempArena);

    //Wea_Save(S("..\\source.wea"));
    //Wea_Load(S("..\\source.wea"));

    return 0;
}
#pragma pack(pop)

Meta_Mark(My_Code,End)
