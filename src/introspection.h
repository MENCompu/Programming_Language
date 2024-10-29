// Type Name
#define Type_Name(expr) S(_Generic(expr, \
    s8: "s8", \
    s16: "s16", \
    s32: "s32", \
    s64: "s64", \
    u8: "u8", \
    u16: "u16", \
    u32: "u32", \
    u64: "u64", \
    JENH_Result: "JENH_Result", \
    f32: "f32", \
    f64: "f64", \
    b8: "b8", \
    Array_void: "Array_void", \
    Array_void_P: "Array_void_P", \
    Array_s8: "Array_s8", \
    Array_s16: "Array_s16", \
    Array_s32: "Array_s32", \
    Array_s64: "Array_s64", \
    Array_u8: "Array_u8", \
    Array_u16: "Array_u16", \
    Array_u32: "Array_u32", \
    Array_u64: "Array_u64", \
    Array_b8: "Array_b8", \
    Array_char: "Array_char", \
    Slice_void_P: "Slice_void_P", \
    Slice_s8: "Slice_s8", \
    Slice_s16: "Slice_s16", \
    Slice_s32: "Slice_s32", \
    Slice_s64: "Slice_s64", \
    Slice_u8: "Slice_u8", \
    Slice_u16: "Slice_u16", \
    Slice_u32: "Slice_u32", \
    Slice_u64: "Slice_u64", \
    Slice_b8: "Slice_b8", \
    Slice_char: "Slice_char", \
    Ring_Buffer_void: "Ring_Buffer_void", \
    String: "String", \
    Array_String: "Array_String", \
    Slice_String: "Slice_String", \
    CString: "CString", \
    Array_CString: "Array_CString", \
    Memory_Arena: "Memory_Arena", \
    Dynamic_Memory: "Dynamic_Memory", \
    Log_Type: "Log_Type", \
    Log_Type_Info: "Log_Type_Info", \
    File_String: "File_String", \
    String_Scan: "String_Scan", \
    Time_OS: "Time_OS", \
    File_Handle: "File_Handle", \
    File: "File", \
    File_Info: "File_Info", \
    File_Tag: "File_Tag", \
    Dir_Stream: "Dir_Stream", \
    Dir_Change_Type: "Dir_Change_Type", \
    Dir_Change: "Dir_Change", \
    Dir_Monitor: "Dir_Monitor", \
    Array_Symbol_Unit: "Array_Symbol_Unit", \
    Array_Symbol_P: "Array_Symbol_P", \
    Variadic: "Variadic", \
    Any_Type: "Any_Type", \
    Sym_Sym: "Sym_Sym", \
    Sym_u8_P: "Sym_u8_P", \
    Symbol: "Symbol", \
    Array_Symbol: "Array_Symbol", \
    Symbol_P: "Symbol_P", \
    Symbol_Unit: "Symbol_Unit", \
    Type_Tag: "Type_Tag", \
    Shared_Direction: "Shared_Direction", \
    Shared_Tag: "Shared_Tag", \
    Shared: "Shared", \
    Array_Shared: "Array_Shared", \
    Field: "Field", \
    Array_Field: "Array_Field", \
    Slice_Field: "Slice_Field", \
    Signature: "Signature", \
    Integer: "Integer", \
    Pointer: "Pointer", \
    Raw_Data: "Raw_Data", \
    Array: "Array", \
    WArray: "WArray", \
    Bundle: "Bundle", \
    Union: "Union", \
    Enum: "Enum", \
    Type: "Type", \
    Array_Type: "Array_Type", \
    Raw_Shared: "Raw_Shared", \
    Array_Raw_Shared: "Array_Raw_Shared", \
    Array_Static_Raw_Shared: "Array_Static_Raw_Shared", \
    Raw_Field: "Raw_Field", \
    Array_Raw_Field: "Array_Raw_Field", \
    Array_Static_Raw_Field: "Array_Static_Raw_Field", \
    Slice_Raw_Field: "Slice_Raw_Field", \
    Param: "Param", \
    Array_Param: "Array_Param", \
    Raw_Enum_Value: "Raw_Enum_Value", \
    Array_Raw_Enum_Value: "Array_Raw_Enum_Value", \
    Array_Static_Raw_Enum_Value: "Array_Static_Raw_Enum_Value", \
    Array_Static_u32: "Array_Static_u32", \
    Raw_Type: "Raw_Type", \
    Array_Raw_Type: "Array_Raw_Type", \
    Slice_Raw_Type: "Slice_Raw_Type", \
    Raw_Fn: "Raw_Fn", \
    Array_Raw_Fn: "Array_Raw_Fn", \
    Slice_Raw_Fn: "Slice_Raw_Fn", \
    Fn_Call: "Fn_Call", \
    Array_Fn_Call: "Array_Fn_Call", \
    Array_Fn_Call_P: "Array_Fn_Call_P", \
    Fn_Call_P: "Fn_Call_P", \
    Code_Block: "Code_Block", \
    Array_Code_Block: "Array_Code_Block", \
    Fn: "Fn", \
    Array_Fn: "Array_Fn", \
    Symbol_State: "Symbol_State", \
    Token_Tag: "Token_Tag", \
    Symbol_Unit_Field: "Symbol_Unit_Field", \
    Literal_Tag: "Literal_Tag", \
    Literal: "Literal", \
    Token: "Token", \
    Array_Token: "Array_Token", \
    Parser_State: "Parser_State", \
    Wea_File_Tag: "Wea_File_Tag", \
    Wea_File_Info: "Wea_File_Info", \
    Code: "Code", \
    Array_Code: "Array_Code", \
    Interpreter_State: "Interpreter_State", \
    Symbol_Init_State: "Symbol_Init_State", \
    Foreach_State: "Foreach_State", \
    Realloc_State: "Realloc_State", \
    Realloc_Error: "Realloc_Error" \
))

// c type info.
static Raw_Type cTypeInfoBuffer[] = {
    { .tag = TAG_Raw_Data, .name = S_Const("s8"), },
    { .tag = TAG_Raw_Data, .name = S_Const("s16"), },
    { .tag = TAG_Raw_Data, .name = S_Const("s32"), },
    { .tag = TAG_Raw_Data, .name = S_Const("s64"), },
    { .tag = TAG_Raw_Data, .name = S_Const("u8"), },
    { .tag = TAG_Raw_Data, .name = S_Const("u16"), },
    { .tag = TAG_Raw_Data, .name = S_Const("u32"), },
    { .tag = TAG_Raw_Data, .name = S_Const("u64"), },
    { .tag = TAG_Enum, .name = S_Const("JENH_Result"),.values = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("JENH_Result_Success"), .value = 0 },
        { .name = S_Const("JENH_Result_Failure"), .value = 1 },
    } } },
    { .tag = TAG_Raw_Data, .name = S_Const("f32"), },
    { .tag = TAG_Raw_Data, .name = S_Const("f64"), },
    { .tag = TAG_Enum, .name = S_Const("b8"),.values = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("false"), .value = 0 },
        { .name = S_Const("true"), .value = 1 },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_void"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("u8*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_void_P"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("void**") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_s8"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("s8*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_s16"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("s16*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_s32"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("s32*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_s64"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("s64*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_u8"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("u8*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_u16"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("u16*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_u32"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("u32*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_u64"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("u64*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_b8"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("b8*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_char"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("char*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Slice_void_P"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("void**") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Slice_s8"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("s8*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Slice_s16"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("s16*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Slice_s32"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("s32*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Slice_s64"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("s64*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Slice_u8"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("u8*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Slice_u16"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("u16*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Slice_u32"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("u32*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Slice_u64"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("u64*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Slice_b8"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("b8*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Slice_char"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("char*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Ring_Buffer_void"),.fields = { .capacity = 256, .count = 4, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("begin"), .type = S_Const("u32") },
        { .name = S_Const("end"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("u8*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("String"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("u8*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_String"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("String*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Slice_String"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("String*") },
    } } },
    { .tag = TAG_Raw_Data, .name = S_Const("CString"), },
    { .tag = TAG_Bundle, .name = S_Const("Array_CString"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("CString*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Memory_Arena"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("u8*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Dynamic_Memory"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("mem"), .type = S_Const("u8*") },
        { .name = S_Const("arena"), .type = S_Const("Memory_Arena") },
    } } },
    { .tag = TAG_Enum, .name = S_Const("Log_Type"),.values = { .capacity = 256, .count = 6, .E = {
        { .name = S_Const("LT_Debug"), .value = 0 },
        { .name = S_Const("LT_Info"), .value = 1 },
        { .name = S_Const("LT_Warn"), .value = 2 },
        { .name = S_Const("LT_Error"), .value = 3 },
        { .name = S_Const("LT_Fatal"), .value = 4 },
        { .name = S_Const("LOG_TYPE_COUNT"), .value = 5 },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Log_Type_Info"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("name"), .type = S_Const("CString") },
        { .name = S_Const("color"), .type = S_Const("CString") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("anon_0"),.fields = { .capacity = 256, .count = 1, .E = {
        { .name = S_Const("E"), .type = S_Const("u8*") },
    } } },
    { .tag = TAG_Union, .name = S_Const("anon_1"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("offset"), .type = S_Const("u64") },
        { .name = S_Const("__anon__"), .type = S_Const("anon_0") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("File_String"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("__anon__"), .type = S_Const("anon_1") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("String_Scan"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("data"), .type = S_Const("String") },
        { .name = S_Const("scan"), .type = S_Const("u8*") },
        { .name = S_Const("end"), .type = S_Const("u8*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Time_OS"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("sleepIsGranular"), .type = S_Const("b8") },
        { .name = S_Const("schedulerMs"), .type = S_Const("u32") },
        { .name = S_Const("counterFrecuency"), .type = S_Const("s64") },
    } } },
    { .tag = TAG_Raw_Data, .name = S_Const("File_Handle"), },
    { .tag = TAG_Bundle, .name = S_Const("File"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("size"), .type = S_Const("u32") },
        { .name = S_Const("data"), .type = S_Const("uint8_t*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("File_Info"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("name[256]"), .type = S_Const("char") },
        { .name = S_Const("writeTime"), .type = S_Const("u64") },
    } } },
    { .tag = TAG_Enum, .name = S_Const("File_Tag"),.values = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("FILE_TAG_NIL"), .value = 0 },
        { .name = S_Const("FILE_TAG_FILE"), .value = 1 },
        { .name = S_Const("FILE_TAG_DIRECTORY"), .value = 2 },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Dir_Stream"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("handle"), .type = S_Const("HANDLE") },
        { .name = S_Const("fileData"), .type = S_Const("WIN32_FIND_DATA") },
    } } },
    { .tag = TAG_Enum, .name = S_Const("Dir_Change_Type"),.values = { .capacity = 256, .count = 4, .E = {
        { .name = S_Const("DCT_Rename"), .value = 0 },
        { .name = S_Const("DCT_Crate"), .value = 1 },
        { .name = S_Const("DCT_Delete"), .value = 2 },
        { .name = S_Const("DCT_Write"), .value = 3 },
    } } },
    { .tag = TAG_Union, .name = S_Const("anon_2"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("fileName"), .type = S_Const("String") },
        { .name = S_Const("oldFileName"), .type = S_Const("String") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Dir_Change"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("type"), .type = S_Const("Dir_Change_Type") },
        { .name = S_Const("__anon__"), .type = S_Const("anon_2") },
        { .name = S_Const("newFileName"), .type = S_Const("String") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Dir_Monitor"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("dir"), .type = S_Const("File_Handle") },
        { .name = S_Const("buffer[1024]"), .type = S_Const("char") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Symbol_Unit"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Symbol_Unit*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Symbol_P"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Symbol**") },
    } } },
    { .tag = TAG_Raw_Data, .name = S_Const("Variadic"), },
    { .tag = TAG_Raw_Data, .name = S_Const("Any_Type"), },
    { .tag = TAG_Bundle, .name = S_Const("Sym_Sym"),.fields = { .capacity = 256, .count = 1, .E = {
        { .name = S_Const("sym"), .type = S_Const("Symbol*") },
    } } },
    { .tag = TAG_Raw_Data, .name = S_Const("Sym_u8_P"), },
    { .tag = TAG_Bundle, .name = S_Const("Symbol"),.fields = { .capacity = 256, .count = 10, .E = {
        { .name = S_Const("outer"), .type = S_Const("Symbol*") },
        { .name = S_Const("inner"), .type = S_Const("Symbol*") },
        { .name = S_Const("parent"), .type = S_Const("Symbol*") },
        { .name = S_Const("type"), .type = S_Const("Type*") },
        { .name = S_Const("isPerm"), .type = S_Const("b8") },
        { .name = S_Const("isModule"), .type = S_Const("b8") },
        { .name = S_Const("name"), .type = S_Const("String") },
        { .name = S_Const("data"), .type = S_Const("u8*") },
        { .name = S_Const("lazyCreation"), .type = S_Const("b8") },
        { .name = S_Const("childs"), .type = S_Const("Array_Symbol_Unit") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Symbol"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Symbol*") },
    } } },
    { .tag = TAG_Raw_Data, .name = S_Const("Symbol_P"), },
    { .tag = TAG_Bundle, .name = S_Const("Symbol_Unit"),.fields = { .capacity = 256, .count = 6, .E = {
        { .name = S_Const("name"), .type = S_Const("String") },
        { .name = S_Const("symbol"), .type = S_Const("Symbol*") },
        { .name = S_Const("using"), .type = S_Const("b8") },
        { .name = S_Const("prevUnit"), .type = S_Const("Symbol_Unit*") },
        { .name = S_Const("fnCall"), .type = S_Const("Symbol*") },
        { .name = S_Const("symbolFnCall"), .type = S_Const("Symbol*") },
    } } },
    { .tag = TAG_Enum, .name = S_Const("Type_Tag"),.values = { .capacity = 256, .count = 10, .E = {
        { .name = S_Const("TAG_Null"), .value = 0 },
        { .name = S_Const("TAG_Signature"), .value = 1 },
        { .name = S_Const("TAG_Integer"), .value = 2 },
        { .name = S_Const("TAG_Pointer"), .value = 3 },
        { .name = S_Const("TAG_Raw_Data"), .value = 4 },
        { .name = S_Const("TAG_Array"), .value = 5 },
        { .name = S_Const("TAG_WArray"), .value = 6 },
        { .name = S_Const("TAG_Bundle"), .value = 7 },
        { .name = S_Const("TAG_Union"), .value = 8 },
        { .name = S_Const("TAG_Enum"), .value = 9 },
    } } },
    { .tag = TAG_Enum, .name = S_Const("Shared_Direction"),.values = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("SHARED_TAG_IN"), .value = 0 },
        { .name = S_Const("SHARED_TAG_OUT"), .value = 1 },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Shared_Tag"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("dir"), .type = S_Const("Shared_Direction") },
        { .name = S_Const("isOptional"), .type = S_Const("u8") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Shared"),.fields = { .capacity = 256, .count = 4, .E = {
        { .name = S_Const("tag"), .type = S_Const("Shared_Tag") },
        { .name = S_Const("name"), .type = S_Const("String") },
        { .name = S_Const("type"), .type = S_Const("Type*") },
        { .name = S_Const("optSymbolName"), .type = S_Const("String") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Shared"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Shared*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Field"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("type"), .type = S_Const("Type*") },
        { .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Field"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Field*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Slice_Field"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Field*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Signature"),.fields = { .capacity = 256, .count = 1, .E = {
        { .name = S_Const("shared"), .type = S_Const("Array_Shared") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Integer"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("size"), .type = S_Const("u32") },
        { .name = S_Const("sign"), .type = S_Const("b8") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Pointer"),.fields = { .capacity = 256, .count = 1, .E = {
        { .name = S_Const("type"), .type = S_Const("Type*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Raw_Data"),.fields = { .capacity = 256, .count = 1, .E = {
        { .name = S_Const("size"), .type = S_Const("u32") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array"),.fields = { .capacity = 256, .count = 1, .E = {
        { .name = S_Const("elemType"), .type = S_Const("Type*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("WArray"),.fields = { .capacity = 256, .count = 1, .E = {
        { .name = S_Const("elemType"), .type = S_Const("Type*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Bundle"),.fields = { .capacity = 256, .count = 1, .E = {
        { .name = S_Const("fields"), .type = S_Const("Array_Field") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Union"),.fields = { .capacity = 256, .count = 1, .E = {
        { .name = S_Const("fields"), .type = S_Const("Array_Field") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Enum"),.fields = { .capacity = 256, .count = 1, .E = {
        { .name = S_Const("values"), .type = S_Const("Symbol*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Type"),.fields = { .capacity = 256, .count = 11, .E = {
        { .name = S_Const("name"), .type = S_Const("String") },
        { .name = S_Const("tag"), .type = S_Const("Type_Tag") },
        { .name = S_Const("varInteger"), .type = S_Const("Integer") },
        { .name = S_Const("varPointer"), .type = S_Const("Pointer") },
        { .name = S_Const("varBundle"), .type = S_Const("Bundle") },
        { .name = S_Const("varUnion"), .type = S_Const("Union") },
        { .name = S_Const("varArray"), .type = S_Const("Array") },
        { .name = S_Const("varWArray"), .type = S_Const("WArray") },
        { .name = S_Const("varSignature"), .type = S_Const("Signature") },
        { .name = S_Const("varRaw_Data"), .type = S_Const("Raw_Data") },
        { .name = S_Const("varEnum"), .type = S_Const("Enum") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Type"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Type*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Raw_Shared"),.fields = { .capacity = 256, .count = 4, .E = {
        { .name = S_Const("tag"), .type = S_Const("Shared_Tag") },
        { .name = S_Const("name"), .type = S_Const("String") },
        { .name = S_Const("type"), .type = S_Const("String") },
        { .name = S_Const("optSymbolName"), .type = S_Const("String") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Raw_Shared"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Raw_Shared*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Static_Raw_Shared"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E[256]"), .type = S_Const("Raw_Shared") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Raw_Field"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("type"), .type = S_Const("String") },
        { .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Raw_Field"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Raw_Field*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Static_Raw_Field"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E[256]"), .type = S_Const("Raw_Field") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Slice_Raw_Field"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Raw_Field*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Param"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("type"), .type = S_Const("String") },
        { .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Param"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Param*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Raw_Enum_Value"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("name"), .type = S_Const("String") },
        { .name = S_Const("value"), .type = S_Const("u32") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Raw_Enum_Value"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Raw_Enum_Value*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Static_Raw_Enum_Value"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E[256]"), .type = S_Const("Raw_Enum_Value") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Static_u32"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E[256]"), .type = S_Const("u32") },
    } } },
    { .tag = TAG_Union, .name = S_Const("anon_3"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("fields"), .type = S_Const("Array_Static_Raw_Field") },
        { .name = S_Const("values"), .type = S_Const("Array_Static_Raw_Enum_Value") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Raw_Type"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("name"), .type = S_Const("String") },
        { .name = S_Const("tag"), .type = S_Const("Type_Tag") },
        { .name = S_Const("__anon__"), .type = S_Const("anon_3") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Raw_Type"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Raw_Type*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Slice_Raw_Type"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Raw_Type*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Raw_Fn"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("name"), .type = S_Const("String") },
        { .name = S_Const("retType"), .type = S_Const("String") },
        { .name = S_Const("sharedList"), .type = S_Const("Array_Static_Raw_Shared") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Raw_Fn"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Raw_Fn*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Slice_Raw_Fn"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Raw_Fn*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Fn_Call"),.fields = { .capacity = 256, .count = 4, .E = {
        { .name = S_Const("fnFnCall"), .type = S_Const("Symbol*") },
        { .name = S_Const("fn"), .type = S_Const("Fn*") },
        { .name = S_Const("args"), .type = S_Const("Symbol*") },
        { .name = S_Const("isResolved"), .type = S_Const("b8") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Fn_Call"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Fn_Call*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Fn_Call_P"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Fn_Call**") },
    } } },
    { .tag = TAG_Raw_Data, .name = S_Const("Fn_Call_P"), },
    { .tag = TAG_Bundle, .name = S_Const("Code_Block"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("locals"), .type = S_Const("Symbol*") },
        { .name = S_Const("code"), .type = S_Const("Array_Fn_Call_P") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Code_Block"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Code_Block*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Fn"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("name"), .type = S_Const("String") },
        { .name = S_Const("sig"), .type = S_Const("Type*") },
        { .name = S_Const("codeBlock"), .type = S_Const("Code_Block") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Fn"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Fn*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Symbol_State"),.fields = { .capacity = 256, .count = 9, .E = {
        { .name = S_Const("root"), .type = S_Const("Symbol*") },
        { .name = S_Const("lits"), .type = S_Const("Symbol*") },
        { .name = S_Const("types"), .type = S_Const("Symbol*") },
        { .name = S_Const("fnCalls"), .type = S_Const("Symbol*") },
        { .name = S_Const("symbolPool"), .type = S_Const("Array_Symbol") },
        { .name = S_Const("symbolUnitArena"), .type = S_Const("Memory_Arena") },
        { .name = S_Const("dataArena"), .type = S_Const("Memory_Arena") },
        { .name = S_Const("stringArena"), .type = S_Const("Memory_Arena") },
        { .name = S_Const("stateArena"), .type = S_Const("Memory_Arena") },
    } } },
    { .tag = TAG_Enum, .name = S_Const("Token_Tag"),.values = { .capacity = 256, .count = 19, .E = {
        { .name = S_Const("TOKEN_TAG_NAME"), .value = 0 },
        { .name = S_Const("TOKEN_TAG_LITERAL"), .value = 1 },
        { .name = S_Const("TOKEN_TAG_SU_FIELD"), .value = 2 },
        { .name = S_Const("TOKEN_TAG_QUOTE"), .value = 3 },
        { .name = S_Const("TOKEN_TAG_FWD"), .value = 4 },
        { .name = S_Const("TOKEN_TAG_BWD"), .value = 5 },
        { .name = S_Const("TOKEN_TAG_CODE_BEGIN"), .value = 6 },
        { .name = S_Const("TOKEN_TAG_CODE_END"), .value = 7 },
        { .name = S_Const("TOKEN_TAG_LIST_BEGIN"), .value = 8 },
        { .name = S_Const("TOKEN_TAG_LIST_END"), .value = 9 },
        { .name = S_Const("TOKEN_TAG_DATA_BEGIN"), .value = 10 },
        { .name = S_Const("TOKEN_TAG_DATA_END"), .value = 11 },
        { .name = S_Const("TOKEN_TAG_GROUP_BEGIN"), .value = 12 },
        { .name = S_Const("TOKEN_TAG_GROUP_END"), .value = 13 },
        { .name = S_Const("TOKEN_TAG_AMPERSAND"), .value = 14 },
        { .name = S_Const("TOKEN_TAG_ASTERISK"), .value = 15 },
        { .name = S_Const("TOKEN_TAG_INNER"), .value = 16 },
        { .name = S_Const("TOKEN_TAG_OUTER"), .value = 17 },
        { .name = S_Const("TOKEN_TAG_COMMA"), .value = 18 },
    } } },
    { .tag = TAG_Enum, .name = S_Const("Symbol_Unit_Field"),.values = { .capacity = 256, .count = 4, .E = {
        { .name = S_Const("SYMBOL_UNIT_FIELD_NULL"), .value = 0 },
        { .name = S_Const("SYMBOL_UNIT_FIELD_SYMBOL"), .value = 1 },
        { .name = S_Const("SYMBOL_UNIT_FIELD_FN_CALL"), .value = 2 },
        { .name = S_Const("SYMBOL_UNIT_FIELD_SYMBOL_FN_CALL"), .value = 3 },
    } } },
    { .tag = TAG_Enum, .name = S_Const("Literal_Tag"),.values = { .capacity = 256, .count = 5, .E = {
        { .name = S_Const("LITERAL_TAG_U64"), .value = 0 },
        { .name = S_Const("LITERAL_TAG_S64"), .value = 1 },
        { .name = S_Const("LITERAL_TAG_F32"), .value = 2 },
        { .name = S_Const("LITERAL_TAG_F64"), .value = 3 },
        { .name = S_Const("LITERAL_TAG_STR"), .value = 4 },
    } } },
    { .tag = TAG_Union, .name = S_Const("anon_4"),.fields = { .capacity = 256, .count = 5, .E = {
        { .name = S_Const("U"), .type = S_Const("u64") },
        { .name = S_Const("S"), .type = S_Const("s64") },
        { .name = S_Const("F32"), .type = S_Const("f32") },
        { .name = S_Const("F64"), .type = S_Const("f64") },
        { .name = S_Const("string"), .type = S_Const("String") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Literal"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("tag"), .type = S_Const("Literal_Tag") },
        { .name = S_Const("__anon__"), .type = S_Const("anon_4") },
    } } },
    { .tag = TAG_Union, .name = S_Const("anon_5"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("name"), .type = S_Const("String") },
        { .name = S_Const("lit"), .type = S_Const("Literal") },
        { .name = S_Const("suField"), .type = S_Const("Symbol_Unit_Field") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Token"),.fields = { .capacity = 256, .count = 4, .E = {
        { .name = S_Const("tag"), .type = S_Const("Token_Tag") },
        { .name = S_Const("__anon__"), .type = S_Const("anon_5") },
        { .name = S_Const("line"), .type = S_Const("u32") },
        { .name = S_Const("column"), .type = S_Const("u32") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Token"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Token*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Parser_State"),.fields = { .capacity = 256, .count = 7, .E = {
        { .name = S_Const("line"), .type = S_Const("u32") },
        { .name = S_Const("beginOfLine"), .type = S_Const("u8*") },
        { .name = S_Const("text"), .type = S_Const("String") },
        { .name = S_Const("scan"), .type = S_Const("u8*") },
        { .name = S_Const("left"), .type = S_Const("Symbol*") },
        { .name = S_Const("tokenID"), .type = S_Const("u32") },
        { .name = S_Const("tokens"), .type = S_Const("Array_Token") },
    } } },
    { .tag = TAG_Enum, .name = S_Const("Wea_File_Tag"),.values = { .capacity = 256, .count = 1, .E = {
        { .name = S_Const("WEA_FILE_TAG_LISP"), .value = 1 },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Wea_File_Info"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("tag"), .type = S_Const("Wea_File_Tag") },
        { .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Code"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("wFnCall"), .type = S_Const("Symbol*") },
        { .name = S_Const("codeBlock"), .type = S_Const("Code_Block*") },
        { .name = S_Const("pc"), .type = S_Const("u32") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Array_Code"),.fields = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("capacity"), .type = S_Const("u32") },
        { .name = S_Const("count"), .type = S_Const("u32") },
        { .name = S_Const("E"), .type = S_Const("Code*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Interpreter_State"),.fields = { .capacity = 256, .count = 1, .E = {
        { .name = S_Const("codeStack"), .type = S_Const("Array_Code") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Symbol_Init_State"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("using"), .type = S_Const("b8") },
        { .name = S_Const("core"), .type = S_Const("Code_Block") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Foreach_State"),.fields = { .capacity = 256, .count = 4, .E = {
        { .name = S_Const("shouldContinue"), .type = S_Const("b8") },
        { .name = S_Const("array"), .type = S_Const("Array_u8") },
        { .name = S_Const("scan"), .type = S_Const("u8*") },
        { .name = S_Const("iter"), .type = S_Const("Symbol*") },
    } } },
    { .tag = TAG_Bundle, .name = S_Const("Realloc_State"),.fields = { .capacity = 256, .count = 2, .E = {
        { .name = S_Const("from"), .type = S_Const("Slice_u8") },
        { .name = S_Const("to"), .type = S_Const("Slice_u8") },
    } } },
    { .tag = TAG_Enum, .name = S_Const("Realloc_Error"),.values = { .capacity = 256, .count = 3, .E = {
        { .name = S_Const("REALLOC_ERROR_IN_NEADER_FROM_OR_TO"), .value = 0 },
        { .name = S_Const("REALLOC_ERROR_ALREADY_IN_TO"), .value = 1 },
        { .name = S_Const("REALLOC_SUCCESS"), .value = 2 },
    } } },
};

static Slice_Raw_Type cTypeInfo;

// c fn info.
static Raw_Fn gFnInfoBuffer[] = {
    { .name = S_Const("Toggle"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("boolean"), .type = S_Const("b8*") },
    } } },
    { .name = S_Const("Is_In_Bounds"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("min"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("x"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("max"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("Mem_Copy_Backward"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("dest"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("src"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("size"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Mem_Copy_Forward"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("dest"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("src"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("size"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("FindAnyDiffByteForward"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 5, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("size"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("bytesToFind"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("sizeBytes"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("timesToIgnore"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("FindAnyDiffByteBackward"), .retType = S_Const("u8*"), .sharedList = { .capacity = 256, .count = 5, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("end"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("bytesToFind"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("sizeBytes"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("timesToIgnore"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("FindAnyByteForwardTimes"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 5, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("count"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("bytesToFind"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("byteCount"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("timesToIgnore"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("FindAnyByteBackward"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 5, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("end"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("bytesToFind"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("sizeBytes"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("timesToIgnore"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("FindByteForwardTimes"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 4, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("end"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("byteToFind"), .type = S_Const("uint8_t") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("timesToIgnore"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("FindByteBackwardsTimes"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 4, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("size"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("byteToFind"), .type = S_Const("uint8_t") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("timesToIgnore"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Mem_Fill_With_Byte"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("size"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("value"), .type = S_Const("uint8_t") },
    } } },
    { .name = S_Const("Mem_Comp"), .retType = S_Const("s32"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem1"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem2"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("size"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Mem_Equal"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inMem1"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inMem2"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inSize"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Mem_Equal_Zero"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("size"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Mem_Init"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 0 } },
    { .name = S_Const("Arena_Create"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("arena"), .type = S_Const("Memory_Arena*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("base"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("capacity"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Arena_Create_From_Arena"), .retType = S_Const("Memory_Arena"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("parent"), .type = S_Const("Memory_Arena*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("capacity"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Arena_Clear"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("arena"), .type = S_Const("Memory_Arena*") },
    } } },
    { .name = S_Const("Arena_Alloc"), .retType = S_Const("u8*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("arena"), .type = S_Const("Memory_Arena*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("size"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Arena_Push_Arena"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("arena"), .type = S_Const("Memory_Arena*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("src"), .type = S_Const("Memory_Arena") },
    } } },
    { .name = S_Const("Log"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Log_Type") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("fmt"), .type = S_Const("CString") },
    } } },
    { .name = S_Const("Str_Alloc"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("arena"), .type = S_Const("Memory_Arena*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("count"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Str_Find_Any_Char_Backward"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("string"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("chars"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Str_Find_Diff_Char_Forward"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("character"), .type = S_Const("u8") },
    } } },
    { .name = S_Const("Str_Find_Any_Diff_Char_Forward"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("chars"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Str_Find_Char_Forward"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inChar"), .type = S_Const("u8") },
    } } },
    { .name = S_Const("Str_Copy"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("dst"), .type = S_Const("String*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("src"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Str_Null_Terminate"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str"), .type = S_Const("String*") },
    } } },
    { .name = S_Const("Str_Alloc_And_Copy"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("arena"), .type = S_Const("Memory_Arena*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("src"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("shouldPutNullTerminator"), .type = S_Const("b8") },
    } } },
    { .name = S_Const("Str_Find_Char_Backward_Times"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("string"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("charToFind"), .type = S_Const("u8") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("timesToIgnore"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Str_Find_Char_Backward"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("string"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("charToFind"), .type = S_Const("u8") },
    } } },
    { .name = S_Const("CStr_Size_Equal"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str1"), .type = S_Const("CString") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str2"), .type = S_Const("CString") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("count"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Str_Begin"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("string"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("count"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Str_End"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("string"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("count"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Str_Skip_Begin"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("string"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("skip"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Str_Skip_End"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("string"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("skip"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Check_Bounds"), .retType = S_Const("u8*"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("min"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("x"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("max"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("Str_Begin_Ptr"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("string"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("ptr"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("Chars_Equal"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str1"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str2"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("count"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Chars_Ptr_Equal"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("ptr"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str"), .type = S_Const("String") },
    } } },
    { .name = S_Const("CStr_Comp"), .retType = S_Const("s32"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str1"), .type = S_Const("CString") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str2"), .type = S_Const("CString") },
    } } },
    { .name = S_Const("CStr_Equal"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str1"), .type = S_Const("CString") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str2"), .type = S_Const("CString") },
    } } },
    { .name = S_Const("CStr_Len"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str"), .type = S_Const("CString") },
    } } },
    { .name = S_Const("CStr_To_Str"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str"), .type = S_Const("CString") },
    } } },
    { .name = S_Const("Str_Comp"), .retType = S_Const("s32"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str1"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str2"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Str_Equal"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str1"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str2"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Str_Begin_Equal"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("a"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("b"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Str_End_Equal"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("a"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("b"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Str_Find_Str_Forward"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("src"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("match"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Str_Consume"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("src"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("match"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("shouldAdvanceMatch"), .type = S_Const("b8") },
    } } },
    { .name = S_Const("CatStr"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("strOut"), .type = S_Const("String*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str1"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str2"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Str_Get_File_Name_In_Path"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("path"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Str_16_To_8"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inStr16"), .type = S_Const("u16*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inSize"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("outStr8"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("Str_Push"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("dst"), .type = S_Const("String*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("src"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Str_Push_Char"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("dst"), .type = S_Const("String*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("ch"), .type = S_Const("u8") },
    } } },
    { .name = S_Const("Array_String_Exists"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("strs"), .type = S_Const("Array_String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Log2"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("value"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("isExact"), .type = S_Const("b8*") },
    } } },
    { .name = S_Const("Radians"), .retType = S_Const("f32"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("degrees"), .type = S_Const("f32") },
    } } },
    { .name = S_Const("ConsoleWrite"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("stdhandleToGet"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("string"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("color"), .type = S_Const("u8") },
    } } },
    { .name = S_Const("Win32_Log"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("logType"), .type = S_Const("Log_Type") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("fmt"), .type = S_Const("CString") },
    } } },
    { .name = S_Const("OS_Alloc_Mem"), .retType = S_Const("u8*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inSize"), .type = S_Const("size_t") },
    } } },
    { .name = S_Const("OS_Alloc_Mem_At"), .retType = S_Const("u8*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inSize"), .type = S_Const("size_t") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inAddress"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("OS_Free_Mem"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inMem"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("Poly_"), .retType = S_Const("u8*"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("dataTag"), .type = S_Const("u32*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("tag"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("retVar"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("DArray_Expand"), .retType = S_Const("u8*"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("arena"), .type = S_Const("Memory_Arena*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("array"), .type = S_Const("Array_void*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("elemSize"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Ring_Buffer_Push_Impl"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("ringBuffer"), .type = S_Const("Ring_Buffer_void*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("elem"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("elemSize"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Ring_Buffer_Pop_Impl"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("ringBuffer"), .type = S_Const("Ring_Buffer_void*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("outElem"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("elemSize"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Consume"), .retType = S_Const("u8*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("ioPtr"), .type = S_Const("void**") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inSize"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Consume_Offset"), .retType = S_Const("u8*"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inBase"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inSize"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("ioOffset"), .type = S_Const("u32*") },
    } } },
    { .name = S_Const("StrScan"), .retType = S_Const("String_Scan"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("scan"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("end"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("Str_Scan_From_Str"), .retType = S_Const("String_Scan"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("string"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Str_Parse_And_Consume_Fmt_Impl"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("src"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("fmt"), .type = S_Const("String") },
    } } },
    { .name = S_Const("StrScanAdvance"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("strScan"), .type = S_Const("String_Scan*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("charCount"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("StrScanEOS"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("strScan"), .type = S_Const("String_Scan") },
    } } },
    { .name = S_Const("Get_Digit_Count"), .retType = S_Const("u16"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inValue"), .type = S_Const("u64") },
    } } },
    { .name = S_Const("Str_Is_Integer"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str"), .type = S_Const("String") },
    } } },
    { .name = S_Const("U32_To_Str"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("value"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Str_To_S64"), .retType = S_Const("s64"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("string"), .type = S_Const("String") },
    } } },
    { .name = S_Const("StrToF32"), .retType = S_Const("f32"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("string"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Abs_To_Rel_Ptr"), .retType = S_Const("u8*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("base"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("ptr"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("Rel_To_Abs_Ptr"), .retType = S_Const("u8*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("base"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("ptr"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("Str_From_File_Str"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inFileString"), .type = S_Const("File_String") },
    } } },
    { .name = S_Const("File_String_From_Str"), .retType = S_Const("File_String"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Time_OS_Init"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 0 } },
    { .name = S_Const("Time_OS_Cleanup"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 0 } },
    { .name = S_Const("Time_Get"), .retType = S_Const("u64"), .sharedList = { .capacity = 256, .count = 0 } },
    { .name = S_Const("Time_CPU_Counter"), .retType = S_Const("u64"), .sharedList = { .capacity = 256, .count = 0 } },
    { .name = S_Const("Time_OS_Counter"), .retType = S_Const("s64"), .sharedList = { .capacity = 256, .count = 0 } },
    { .name = S_Const("Time_OS_Seconds_Elapsed"), .retType = S_Const("f32"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("begin"), .type = S_Const("s64") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("end"), .type = S_Const("s64") },
    } } },
    { .name = S_Const("Time_Sec_To_Ms"), .retType = S_Const("f32"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("secs"), .type = S_Const("f32") },
    } } },
    { .name = S_Const("OS_FILETIME_To_Time"), .retType = S_Const("u64"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inFileTime"), .type = S_Const("FILETIME") },
    } } },
    { .name = S_Const("Time_Mask_Days"), .retType = S_Const("u64"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("time"), .type = S_Const("u64") },
    } } },
    { .name = S_Const("Time_Mask_Hrs"), .retType = S_Const("u64"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("time"), .type = S_Const("u64") },
    } } },
    { .name = S_Const("Time_Mask_Mins"), .retType = S_Const("u64"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("time"), .type = S_Const("u64") },
    } } },
    { .name = S_Const("Time_Mask_Secs"), .retType = S_Const("u64"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("time"), .type = S_Const("u64") },
    } } },
    { .name = S_Const("Time_Mask_Mils"), .retType = S_Const("u64"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("time"), .type = S_Const("u64") },
    } } },
    { .name = S_Const("File_Exists"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("dirPath"), .type = S_Const("CString") },
    } } },
    { .name = S_Const("File_Create"), .retType = S_Const("File_Handle"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("path"), .type = S_Const("CString") },
    } } },
    { .name = S_Const("File_Open"), .retType = S_Const("File_Handle"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("path"), .type = S_Const("CString") },
    } } },
    { .name = S_Const("File_Handle_Is_Valid"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inHandle"), .type = S_Const("File_Handle") },
    } } },
    { .name = S_Const("File_Close"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("handle"), .type = S_Const("File_Handle") },
    } } },
    { .name = S_Const("File_Delete"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("path"), .type = S_Const("CString") },
    } } },
    { .name = S_Const("File_Get_Size"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("retHandle"), .type = S_Const("File_Handle") },
    } } },
    { .name = S_Const("File_Get_Write_Time"), .retType = S_Const("u64"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inPath"), .type = S_Const("CString") },
    } } },
    { .name = S_Const("File_Read"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("handle"), .type = S_Const("File_Handle") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("bytesToRead"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("File_Read_At"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 4, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("handle"), .type = S_Const("File_Handle") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("offset"), .type = S_Const("size_t") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("bytesToRead"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("File_Read_All"), .retType = S_Const("u8*"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("path"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("arena"), .type = S_Const("Memory_Arena*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("size"), .type = S_Const("u32*") },
    } } },
    { .name = S_Const("File_Write"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("handle"), .type = S_Const("File_Handle") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("bytesToWrite"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("File_Write_At"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 4, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("handle"), .type = S_Const("File_Handle") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("offset"), .type = S_Const("size_t") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("bytesToWrite"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("File_Clear"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("handle"), .type = S_Const("File_Handle") },
    } } },
    { .name = S_Const("File_Size_Set"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("handle"), .type = S_Const("File_Handle") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("size"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("File_Create_With_Contents"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("path"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("data"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("size"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("File_Replace_Contents"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("path"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("data"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("size"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Dir_Exists"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("dirPath"), .type = S_Const("CString") },
    } } },
    { .name = S_Const("Dir_Create"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("dirPath"), .type = S_Const("CString") },
    } } },
    { .name = S_Const("Dir_Recursive_Delete"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inDirPath"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Dir_Get_All_Files_Write_Times"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("dirPath"), .type = S_Const("CString") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("infos"), .type = S_Const("File_Info*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("DEBUGCount"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("File_Get_Tag"), .retType = S_Const("File_Tag"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("path"), .type = S_Const("CString") },
    } } },
    { .name = S_Const("Dir_Stream_Open"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inDirPath"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("outDirStream"), .type = S_Const("Dir_Stream*") },
    } } },
    { .name = S_Const("Dir_Stream_Read"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inDirStream"), .type = S_Const("Dir_Stream*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("outFileName"), .type = S_Const("String*") },
    } } },
    { .name = S_Const("Dir_Stream_Close"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inDirStream"), .type = S_Const("Dir_Stream*") },
    } } },
    { .name = S_Const("Dir_Monitor_Init"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inDirPath"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("outMonitor"), .type = S_Const("Dir_Monitor*") },
    } } },
    { .name = S_Const("Dir_Monitor_Wait_Till_Change"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inMonitor"), .type = S_Const("Dir_Monitor*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("reloadSemaphore"), .type = S_Const("HANDLE") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("outDirChange"), .type = S_Const("Dir_Change*") },
    } } },
    { .name = S_Const("Dir_Monitor_End"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inMonitor"), .type = S_Const("Dir_Monitor*") },
    } } },
    { .name = S_Const("File_Fill_With_Contents_Safe"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("path"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("data"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("size"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("File_Append"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("handle"), .type = S_Const("File_Handle") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("bytesToAppend"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("Dir_Open"), .retType = S_Const("File_Handle"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("path"), .type = S_Const("CString") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("async"), .type = S_Const("b8") },
    } } },
    { .name = S_Const("Dir_File_Count"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("dirPath"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Dir_Find_File"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("dirPath"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("fileName"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Type_Get_Unsafe"), .retType = S_Const("Type*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("parent"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Type_Get"), .retType = S_Const("Type*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Type_All_Get"), .retType = S_Const("Type*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Symbol_Create"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("parent"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
    } } },
    { .name = S_Const("Symbol_Scope_Get"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Symbol_Get"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("parent"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Symbol_Get_Unsafe"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("parent"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Addr"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("symbol"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("Symbol_Set"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 4, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("symbol"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("allocNew"), .type = S_Const("b8") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("Symbol_Unit_Get_Unsafe_"), .retType = S_Const("Symbol_Unit*"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("parent"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inExternLocals"), .type = S_Const("b8") },
    } } },
    { .name = S_Const("Symbol_Unit_Get"), .retType = S_Const("Symbol_Unit*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("parent"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Symbol_Push_Child_C_"), .retType = S_Const("Symbol_Unit*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("symbol"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("unit"), .type = S_Const("Symbol_Unit") },
    } } },
    { .name = S_Const("Scope_Top"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("index"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Module_Top"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 0 } },
    { .name = S_Const("Symbol_Data_Create"), .retType = S_Const("u8*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("size"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Symbol_New_"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("parent"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("isPerm"), .type = S_Const("b8") },
    } } },
    { .name = S_Const("Symbol_Set_Mem"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("symbol"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("Symbol_Create_From_Type"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("symbol"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
    } } },
    { .name = S_Const("Symbol_Create_And_Init_"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 6, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("parent"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("isPerm"), .type = S_Const("b8") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("allocNew"), .type = S_Const("b8") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("mem"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("Symbol_Data_Get"), .retType = S_Const("u8*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("symbol"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("Symbol_Data_Get_And_Check"), .retType = S_Const("u8*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("symbol"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("Fn_Create"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 4, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("ret"), .type = S_Const("Param") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("params"), .type = S_Const("Array_Param") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("code"), .type = S_Const("Array_Fn_Call_P") },
    } } },
    { .name = S_Const("Parser_Init"), .retType = S_Const("Parser_State"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("arena"), .type = S_Const("Memory_Arena*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("text"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Parser_Init_From_File"), .retType = S_Const("Parser_State"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("path"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("arena"), .type = S_Const("Memory_Arena*") },
    } } },
    { .name = S_Const("Parse_Symbol"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Parser_State*") },
    } } },
    { .name = S_Const("Wea_File_Tag_Get_From_String"), .retType = S_Const("Wea_File_Tag"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Parse_Wea_File_Info"), .retType = S_Const("Wea_File_Info"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("path"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Parse"), .retType = S_Const("Array_Fn_Call_P"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Parser_State*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("tag"), .type = S_Const("Wea_File_Tag") },
    } } },
    { .name = S_Const("Parse_Text"), .retType = S_Const("Array_Fn_Call_P"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("text"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("tag"), .type = S_Const("Wea_File_Tag") },
    } } },
    { .name = S_Const("Parse_File"), .retType = S_Const("Array_Fn_Call_P"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("path"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Module_Create_By_File"), .retType = S_Const("Fn*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("path"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Scope_Iter_Advance"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("code"), .type = S_Const("Code*") },
    } } },
    { .name = S_Const("Interpret_Begin"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 0 } },
    { .name = S_Const("Interpret_End"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 0 } },
    { .name = S_Const("Get_Out_Symbol"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("sharedList"), .type = S_Const("Array_Shared") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("args"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("Get_Out_Symbol_Fn_Call"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("fnCall"), .type = S_Const("Fn_Call*") },
    } } },
    { .name = S_Const("Fn_Call_Resolve"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("wFnCall"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("outArg"), .type = S_Const("Symbol_Unit*") },
    } } },
    { .name = S_Const("Fn_Call_Resolve_Rec"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("wFnCall"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("outArg"), .type = S_Const("Symbol_Unit*") },
    } } },
    { .name = S_Const("Interpret"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("code"), .type = S_Const("Code") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("outArg"), .type = S_Const("Symbol_Unit*") },
    } } },
    { .name = S_Const("Interpret_Code"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("code"), .type = S_Const("Array_Fn_Call_P") },
    } } },
    { .name = S_Const("Interpret_Code_Block"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("codeBlock"), .type = S_Const("Code_Block*") },
    } } },
    { .name = S_Const("Interpret_Fn_Call"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("wFnCall"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("Get_Fn_Index"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Get_C_Fn"), .retType = S_Const("Raw_Fn*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Raw_Type_Get"), .retType = S_Const("Raw_Type*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Let"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("typeName"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = true }, .name = S_Const("init"), .type = S_Const("Any_Type"), .optSymbolName = S_Const("NULL") },
    } } },
    { .name = S_Const("Let_C"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("typeName"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("scope"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("Using"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Type_Name_Is_Integer"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("typeName"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Type_Name_Is_Raw_Data"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("typeName"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Type_Name_Is_Pointer"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("typeName"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Type_Name_Is_Array"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("typeName"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Type_Name_Is_WArray"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("typeName"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Type_Name_Is_Sym"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("typeName"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Type_Name_Is_Primitive"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("typeName"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Type_Is_Primitive"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
    } } },
    { .name = S_Const("Type_Is_Not_Primitive"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
    } } },
    { .name = S_Const("Type_Create"), .retType = S_Const("Type*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("parent"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Type_Size"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
    } } },
    { .name = S_Const("Type_Check"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("a"), .type = S_Const("Type*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("b"), .type = S_Const("Type*") },
    } } },
    { .name = S_Const("Bundle_Init"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("rawFields"), .type = S_Const("Array_Raw_Field") },
    } } },
    { .name = S_Const("Bundle_Create"), .retType = S_Const("Type*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("rawFields"), .type = S_Const("Array_Raw_Field") },
    } } },
    { .name = S_Const("Union_Init"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("rawFields"), .type = S_Const("Array_Raw_Field") },
    } } },
    { .name = S_Const("Union_Create"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("rawFields"), .type = S_Const("Array_Raw_Field") },
    } } },
    { .name = S_Const("Symbol_Enum_Values"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
    } } },
    { .name = S_Const("Enum_Init"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("names"), .type = S_Const("Array_String") },
    } } },
    { .name = S_Const("Enum_Create"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("names"), .type = S_Const("Array_String") },
    } } },
    { .name = S_Const("Pointee_Name"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("ptrName"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Pointer_Init"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("pointee"), .type = S_Const("Type*") },
    } } },
    { .name = S_Const("Integer_Init_By_Name"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Raw_Data_Init_By_Name"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Array_Init"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("elemType"), .type = S_Const("Type*") },
    } } },
    { .name = S_Const("WArray_Init"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("elemType"), .type = S_Const("Type*") },
    } } },
    { .name = S_Const("Elem_Name"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Handle_Pointer"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("WElem_Name"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Type_Primitive_Create_By_Name"), .retType = S_Const("Type*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("parent"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Type_Exists"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Type_Ptr_Get"), .retType = S_Const("Type*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
    } } },
    { .name = S_Const("Exists_Enum_Val"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("_enum"), .type = S_Const("Enum*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("valueSymbol"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("Has_Optional"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("sharedList"), .type = S_Const("Array_Shared") },
    } } },
    { .name = S_Const("Has_Variadic"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("sharedList"), .type = S_Const("Array_Shared") },
    } } },
    { .name = S_Const("Type_Create_Temp"), .retType = S_Const("Type*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Type_Get_Temp"), .retType = S_Const("Type*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Type_Init_Temp"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Symbol_Get_Module"), .retType = S_Const("Fn*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Symbol_Core"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 0 } },
    { .name = S_Const("Symbol_Unit_Scope_Get"), .retType = S_Const("Symbol_Unit*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Symbol_Exists"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("parent"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Symbol_Check_Type"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("parent"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
    } } },
    { .name = S_Const("Get_Sym"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Get_Sym_Unsafe"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Symbol_Forward"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("parent"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Symbol_Backward"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("child"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("Symbol_Inner"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("symbol"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("Symbol_Outer"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("symbol"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("Deref"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("symbol"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("Symbol_Data_Is_In_Data_Arena"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("symbol"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("Symbol_Lit_Create"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("data"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("Wea_Str_Lit"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("str"), .type = S_Const("String") },
    } } },
    { .name = S_Const("WLit"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("symbol"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("Symbol_Create_From_Fields"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("symbol"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("fields"), .type = S_Const("Array_Field") },
    } } },
    { .name = S_Const("Symbol_Resolve"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("symbol"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("Are_Symbol_And_Type_Compatible"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("symbol"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
    } } },
    { .name = S_Const("Fn_Create_"), .retType = S_Const("Fn*"), .sharedList = { .capacity = 256, .count = 6, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("parent"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("ret"), .type = S_Const("Param") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("params"), .type = S_Const("Array_Param") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("code"), .type = S_Const("Array_Fn_Call_P") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("shouldCreateLocals"), .type = S_Const("b8") },
    } } },
    { .name = S_Const("Fn_Create_With_Locals"), .retType = S_Const("Fn*"), .sharedList = { .capacity = 256, .count = 5, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("parent"), .type = S_Const("Symbol*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("ret"), .type = S_Const("Param") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("params"), .type = S_Const("Array_Param") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("code"), .type = S_Const("Array_Fn_Call_P") },
    } } },
    { .name = S_Const("Fn_Create_Scope"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 5, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("scope"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("ret"), .type = S_Const("Param") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("params"), .type = S_Const("Array_Param") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("code"), .type = S_Const("Array_Fn_Call_P") },
    } } },
    { .name = S_Const("Module_Create"), .retType = S_Const("Fn*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("code"), .type = S_Const("Array_Fn_Call_P") },
    } } },
    { .name = S_Const("Get_Fn_Call_Symbol"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("fnCall"), .type = S_Const("Fn_Call*") },
    } } },
    { .name = S_Const("F_"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("fn"), .type = S_Const("Symbol_Unit") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inUnits"), .type = S_Const("Array_Symbol_Unit") },
    } } },
    { .name = S_Const("F_By_Name"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("fnName"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inUnits"), .type = S_Const("Array_Symbol_Unit") },
    } } },
    { .name = S_Const("Symbol_Init"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 0 } },
    { .name = S_Const("Lexer_Parse"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Parser_State*") },
    } } },
    { .name = S_Const("Parser_Is_EoF"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Parser_State*") },
    } } },
    { .name = S_Const("Lexer_Advance_To_Index"), .retType = S_Const("Token*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Parser_State*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("tokenID"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Lexer_Advance"), .retType = S_Const("Token*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Parser_State*") },
    } } },
    { .name = S_Const("Lexer_Advance_And_Check"), .retType = S_Const("Token*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Parser_State*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("tag"), .type = S_Const("Token_Tag") },
    } } },
    { .name = S_Const("Lexer_Update_Parser_Line"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Parser_State*") },
    } } },
    { .name = S_Const("Lexer_Look_Back"), .retType = S_Const("Token*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Parser_State*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("index"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Lexer_Look_Ahead"), .retType = S_Const("Token*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Parser_State*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("index"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Lexer_Consume_White_Space"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Parser_State*") },
    } } },
    { .name = S_Const("Parser_Parse_String"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Parser_State*") },
    } } },
    { .name = S_Const("Is_Symbol_Fn"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("argSymbol"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("Set_Default_Arg"), .retType = S_Const("Symbol_Unit"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("argSymbol"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("Parse_Symbol_L"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Parser_State*") },
    } } },
    { .name = S_Const("Parse_Arg"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Parser_State*") },
    } } },
    { .name = S_Const("Parse_Arg_Unit"), .retType = S_Const("Symbol_Unit"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Parser_State*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("outSymbol"), .type = S_Const("Symbol**") },
    } } },
    { .name = S_Const("Symbol_Scope_Exists"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("parent"), .type = S_Const("Symbol**") },
    } } },
    { .name = S_Const("Let_"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 4, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("typeName"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("init"), .type = S_Const("Any_Type") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("scope"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("View"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("name"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("symbol"), .type = S_Const("Symbol*") },
    } } },
    { .name = S_Const("If"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("boolean"), .type = S_Const("b8") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("thenBlock"), .type = S_Const("Array_Fn_Call_P") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = true }, .name = S_Const("elseBlock"), .type = S_Const("Array_Fn_Call_P"), .optSymbolName = S_Const("NULL") },
    } } },
    { .name = S_Const("Not"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("boolean"), .type = S_Const("b8") },
    } } },
    { .name = S_Const("While"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("condition"), .type = S_Const("Fn_Call*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("body"), .type = S_Const("Array_Fn_Call_P") },
    } } },
    { .name = S_Const("Foreach_Begin_"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("iterName"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("weaArray"), .type = S_Const("Any_Type") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Foreach_State*") },
    } } },
    { .name = S_Const("Foreach_Update"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Foreach_State*") },
    } } },
    { .name = S_Const("Foreach"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("iterName"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("array"), .type = S_Const("Any_Type") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("body"), .type = S_Const("Array_Fn_Call_P") },
    } } },
    { .name = S_Const("Cast"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_OUT, .isOptional = false }, .name = S_Const("out"), .type = S_Const("Any_Type") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("typeName"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("src"), .type = S_Const("Any_Type") },
    } } },
    { .name = S_Const("WArray_Push"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("wArray"), .type = S_Const("Any_Type") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("elem"), .type = S_Const("Any_Type") },
    } } },
    { .name = S_Const("WArray_Index"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_OUT, .isOptional = false }, .name = S_Const("out"), .type = S_Const("Any_Type") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("wArray"), .type = S_Const("Any_Type") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("index"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Type_Get_Name"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
    } } },
    { .name = S_Const("Mul"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("a"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("b"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Add"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("a"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("b"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Sub"), .retType = S_Const("u32"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("a"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("b"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Eq"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("a"), .type = S_Const("Any_Type") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("b"), .type = S_Const("Any_Type") },
    } } },
    { .name = S_Const("Less_Than"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("a"), .type = S_Const("u32") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("b"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Return"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 0 } },
    { .name = S_Const("Import"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("path"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("alias"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("using"), .type = S_Const("b8") },
    } } },
    { .name = S_Const("Input"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 0 } },
    { .name = S_Const("Output"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("args"), .type = S_Const("Variadic") },
    } } },
    { .name = S_Const("Str_From_U32"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("value"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("WStr_From_Fmt"), .retType = S_Const("String"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("fmt"), .type = S_Const("String") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("args"), .type = S_Const("Variadic") },
    } } },
    { .name = S_Const("Transfer"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_OUT, .isOptional = false }, .name = S_Const("dst"), .type = S_Const("Any_Type") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("src"), .type = S_Const("Any_Type") },
    } } },
    { .name = S_Const("Lit"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_OUT, .isOptional = false }, .name = S_Const("out"), .type = S_Const("Any_Type") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("args"), .type = S_Const("Variadic") },
    } } },
    { .name = S_Const("Fn_Call_Create"), .retType = S_Const("Fn_Call*"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inArgs"), .type = S_Const("Variadic") },
    } } },
    { .name = S_Const("Type_Has_Pointer_Rec"), .retType = S_Const("b8"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
    } } },
    { .name = S_Const("Realloc"), .retType = S_Const("Realloc_Error"), .sharedList = { .capacity = 256, .count = 2, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("inAddress"), .type = S_Const("u8*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("state"), .type = S_Const("Realloc_State*") },
    } } },
    { .name = S_Const("Serialize_Rec"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 5, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("var"), .type = S_Const("Any_Type") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("recursive"), .type = S_Const("b8") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("isArrayElem"), .type = S_Const("b8") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("stringBefore"), .type = S_Const("b8") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("depthLevel"), .type = S_Const("u32") },
    } } },
    { .name = S_Const("Util_Iter"), .retType = S_Const("Symbol*"), .sharedList = { .capacity = 256, .count = 3, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("iter"), .type = S_Const("Symbol**") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("type"), .type = S_Const("Type*") },
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("ptr"), .type = S_Const("u8*") },
    } } },
    { .name = S_Const("Wea_Save"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("path"), .type = S_Const("String") },
    } } },
    { .name = S_Const("Wea_Load"), .retType = S_Const("void"), .sharedList = { .capacity = 256, .count = 1, .E = {
        { .tag = { .dir = SHARED_TAG_IN, .isOptional = false }, .name = S_Const("path"), .type = S_Const("String") },
    } } },
};

static Slice_Raw_Fn gFnsInfo;

