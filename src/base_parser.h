typedef enum {
    WEA_FILE_TAG_LISP = 1,
} Wea_File_Tag;

typedef struct {
    Wea_File_Tag tag;
    String name;
} Wea_File_Info;

typedef struct Array_Fn_Call_P Array_Fn_Call_P;

// Functions.
Public Wea_File_Tag Wea_File_Tag_Get_From_String(String name);
Public Wea_File_Info Parse_Wea_File_Info(String path);
Public Array_Fn_Call_P Parse(Parser_State* state, Wea_File_Tag tag);
Public Array_Fn_Call_P Parse_Text(String text, Wea_File_Tag tag);
Public Array_Fn_Call_P Parse_File(String path);
Public Fn* Module_Create_By_File(String path);
