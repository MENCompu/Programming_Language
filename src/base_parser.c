Public Wea_File_Tag Wea_File_Tag_Get_From_String(String name) {
    if ( 0 ) {
    } else if ( Str_Equal(name, S("lisp")) ) {
        return WEA_FILE_TAG_LISP;
    } NO_ELSE

    return (Wea_File_Tag)0;
}

Public Wea_File_Info Parse_Wea_File_Info(String path) {
    Wea_File_Info ret = { 0 };

    Assert( Str_Equal(Str_End(path, Str_Find_Char_Backward(path, '.')), S("wea")) );

    u32 dirIndex = Str_Find_Char_Backward(path, '\\');

    ret.name = path;

    if ( dirIndex != MAX_U32 ) {
        ret.name = Str_End(path, dirIndex);
    }

    String extension = Str_Skip_Begin(ret.name, Str_Find_Char_Forward(ret.name, '.') + 1);

    ret.tag  = Wea_File_Tag_Get_From_String(Str_Begin(extension, Str_Find_Char_Forward(extension, '.')));
    ret.name = Str_Alloc_And_Copy(&gTempArena, Str_Begin(ret.name, Str_Find_Char_Forward(ret.name, '.')), true);

    return ret;
}

Public Array_Fn_Call_P Parse(Parser_State* state, Wea_File_Tag tag) {
    //Local_Array(Symbol_P, tempCode, KiB(4));
    Local_Array(Fn_Call_P, tempCode, KiB(4));

    while ( !Parser_Is_EoF(state) ) {
        Symbol* argSymbol = 0;

        switch ( tag ) {
            case WEA_FILE_TAG_LISP: { argSymbol = Parse_Symbol_L(state); } break;
        }

        Array_Push_C(tempCode, *D(Fn_Call*, argSymbol));
    }

    Array_Fn_Call_P code;
    Array_Alloc_And_Copy(&code, tempCode, &gSymbolState.dataArena);

    return code;
}

Public Array_Fn_Call_P Parse_Text(String text, Wea_File_Tag tag) {
    Parser_State parser = Parser_Init(&gPermArena, text);
    return Parse(&parser, tag);
}

Public Array_Fn_Call_P Parse_File(String path) {
    Wea_File_Info fileInfo = Parse_Wea_File_Info(path);
    Parser_State parser = Parser_Init_From_File(path, &gPermArena);
    return Parse(&parser, fileInfo.tag);
}

Public Fn* Module_Create_By_File(String path) {
    Wea_File_Info fileInfo = Parse_Wea_File_Info(path);
    return Module_Create(fileInfo.name, Parse_File(path));
}
