#define ANONIMUS_SYMBOl_NAME S("__anon__")

#define TYPES_LIST \
    X_Type(Signature, struct { \
        Array_Shared shared; \
    }) \
    X_Type(Integer, struct { \
        u32 size; \
        b8  sign; \
    }) \
    X_Type(Pointer, struct { \
        Type* type; \
    }) \
    X_Type(Raw_Data, struct { \
        u32 size; \
    }) \
    X_Type(Array, struct { \
        Type* elemType; \
    }) \
    X_Type(WArray, struct { \
        Type* elemType; \
    }) \
    X_Type(Bundle, struct { \
        Array_Field fields; \
    }) \
    X_Type(Union, struct { \
        Array_Field fields; \
    }) \
    X_Type(Enum, struct { \
        Symbol* values; \
    }) \
//END

typedef enum {
    TAG_Null,

#define X_Type(typeName, ...) \
    P(TAG_, typeName),
TYPES_LIST
#undef X_Type

} Type_Tag;

typedef enum {
    SHARED_TAG_IN,
    SHARED_TAG_OUT,
} Shared_Direction;

typedef struct {
    Shared_Direction dir;
    u8 isOptional;
} Shared_Tag;

#ifdef WEA_TYPES
    typedef struct Fn Fn;
    typedef struct Fn_Call Fn_Call;

    typedef struct {
        Shared_Tag tag;
        String name;
        Type* type;
        String optSymbolName;
    } Shared;
    Array(Shared);

    typedef struct {
        Type*  type;
        String name;
    } Field;
    Array(Field);
    Slice(Field);

    #define X_Type(typeName, structDef) \
        typedef structDef typeName;
    TYPES_LIST
    #undef X_Type

    #define Poly_Name(type) P(var, type)

    #define Poly_Def(type) \
        type Poly_Name(type)

    typedef struct Type {
        String name;

        Type_Tag tag;

        Poly_Def(Integer);
        Poly_Def(Pointer);
        Poly_Def(Bundle);
        Poly_Def(Union);
        Poly_Def(Array);
        Poly_Def(WArray);
        Poly_Def(Signature);
        Poly_Def(Raw_Data);
        Poly_Def(Enum);
    } Type;
    Array(Type);

    // Functions

    Public Type* Type_Get_Unsafe(Symbol* parent, String name);
    Public Type* Type_Get(String name);
    Public Type* Type_All_Get(String name);
#endif

typedef struct {
    Shared_Tag tag;

    String name;
    String type;
    String optSymbolName;
} Raw_Shared;
Array(Raw_Shared);
Array_Static(Raw_Shared, 256);

typedef struct {
    String type;
    String name;
} Raw_Field;
Array(Raw_Field);
Array_Static(Raw_Field, 256);
Slice(Raw_Field);

typedef struct {
    String type;
    String name;
} Param;
Array(Param);

typedef struct {
    String name;
    u32 value;
} Raw_Enum_Value;
Array(Raw_Enum_Value);
Array_Static(Raw_Enum_Value, 256);

Array_Static(u32, 256);
typedef struct Raw_Type {
    String name;

    Type_Tag tag;
    union {
        Array_Static_Raw_Field fields;
        Array_Static_Raw_Enum_Value values;
    };
} Raw_Type;
Array(Raw_Type);
Slice(Raw_Type);
// Raw Types.

typedef struct {
    String name;
    String retType;
    Array_Static_Raw_Shared sharedList;
} Raw_Fn;
Array(Raw_Fn);
Slice(Raw_Fn);
