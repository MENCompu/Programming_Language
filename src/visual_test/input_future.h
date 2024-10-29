#ifndef INPUT_H
#define INPUT_H

// NOTE(JENH): See keyboards scan codes
//             https://www.plantation-productions.com/Webster/www.artofasm.com/DOS/pdf/apndxc.pdf
//             Names are from QWERTY keyboard layout.
#define KBD_EXT_INDEX_MASK 0b10000000

#undef INPUT_KEYBOARD
#define INPUT_KEYBOARD \
    X(u8, Nul, 0x00) \
    X(u8, esc, 0x01) \
    X(u8, key1, 0x02) \
    X(u8, key2, 0x03) \
    X(u8, key3, 0x04) \
    X(u8, key4, 0x05) \
    X(u8, key5, 0x06) \
    X(u8, key6, 0x07) \
    X(u8, key7, 0x08) \
    X(u8, key8, 0x09) \
    X(u8, key9, 0x0a) \
    X(u8, key0, 0x0a) \
    X(u8, hyphen, 0x0c) \
    X(u8, equal, 0x0d) \
    X(u8, tab, 0x0f) \
    X(u8, q, 0x10) \
    X(u8, w, 0x11) \
    X(u8, e, 0x12) \
    X(u8, r, 0x13) \
    X(u8, t, 0x14) \
    X(u8, y, 0x15) \
    X(u8, u, 0x16) \
    X(u8, i, 0x17) \
    X(u8, o, 0x18) \
    X(u8, p, 0x19) \
    X(u8, sqrBcksOpen, 0x1a) \
    X(u8, sqrBcksClose, 0x1b) \
    X(u8, enter, 0x1c) \
    X(u8, ctrlL, 0x1d) \
    X(u8, a, 0x1e) \
    X(u8, s, 0x1f) \
    X(u8, d, 0x20) \
    X(u8, f, 0x21) \
    X(u8, g, 0x22) \
    X(u8, h, 0x23) \
    X(u8, j, 0x24) \
    X(u8, k, 0x25) \
    X(u8, l, 0x26) \
    X(u8, semicolon, 0x27) \
    X(u8, quotation, 0x28) \
    X(u8, apostrophe, 0x29) \
    X(u8, shiftL, 0x2a) \
    X(u8, backslash, 0x2b) \
    X(u8, z, 0x2c) \
    X(u8, x, 0x2d) \
    X(u8, c, 0x2e) \
    X(u8, v, 0x2f) \
    X(u8, b, 0x30) \
    X(u8, n, 0x31) \
    X(u8, m, 0x32) \
    X(u8, comma, 0x33) \
    X(u8, dot, 0x34) \
    X(u8, slash, 0x35) \
    X(u8, shiftR, 0x36) \
    X(u8, asterisk, 0x37) \
    X(u8, altL, 0x38) \
    X(u8, space, 0x39) \
    X(u8, caps, 0x3a) \
    X(u8, f1, 0x3b) \
    X(u8, f2, 0x3c) \
    X(u8, f3, 0x3d) \
    X(u8, f4, 0x3e) \
    X(u8, f5, 0x3f) \
    X(u8, f6, 0x40) \
    X(u8, f7, 0x41) \
    X(u8, f8, 0x42) \
    X(u8, f9, 0x43) \
    X(u8, f10, 0x44) \
    X(u8, numLock, 0x45) \
    X(u8, rollLock, 0x46) \
    X(u8, numpad7, 0x47) \
    X(u8, numpad8, 0x48) \
    X(u8, numpad9, 0x49) \
    X(u8, numpadMinus, 0x4a) \
    X(u8, numpad4, 0x4b) \
    X(u8, numpad5, 0x4c) \
    X(u8, numpad6, 0x4d) \
    X(u8, numpadPlus, 0x4e) \
    X(u8, numpad1, 0x4f) \
    X(u8, numpad2, 0x50) \
    X(u8, numpad3, 0x51) \
    X(u8, numpad0, 0x52) \
    X(u8, del, 0x53) \
    X(u8, f11, 0x57) \
    X(u8, f12, 0x58) \
    X(u8, slash2, KBD_EXT_INDEX_MASK|0x35) \
    X(u8, enter2, KBD_EXT_INDEX_MASK|0x1c) \
    X(u8, ins2, KBD_EXT_INDEX_MASK|0x52) \
    X(u8, del2, KBD_EXT_INDEX_MASK|0x53) \
    X(u8, home2, KBD_EXT_INDEX_MASK|0x47) \
    X(u8, end2, KBD_EXT_INDEX_MASK|0x4f) \
    X(u8, pageup2, KBD_EXT_INDEX_MASK|0x49) \
    X(u8, pagedown2, KBD_EXT_INDEX_MASK|0x51) \
    X(u8, left, KBD_EXT_INDEX_MASK|0x4b) \
    X(u8, right, KBD_EXT_INDEX_MASK|0x4d) \
    X(u8, up, KBD_EXT_INDEX_MASK|0x48) \
    X(u8, down, KBD_EXT_INDEX_MASK|0x50) \
    X(u8, altR, KBD_EXT_INDEX_MASK|0x38) \
    X(u8, ctrlR, KBD_EXT_INDEX_MASK|0x1d) \
    X(u8, pause, KBD_EXT_INDEX_MASK|0x7f)

#define KEYBOARD_MAX_KEY_COUNT 0b11111111
typedef u8 Input_Keyboard[KEYBOARD_MAX_KEY_COUNT+1];

#undef INPUT_MOUSE
#define INPUT_MOUSE_DEF \
    IN_FIELD(u8, button1) \
    IN_FIELD(u8, button2) \
    IN_FIELD(u8, button3) \
    IN_FIELD(u8, button4) \
    IN_FIELD(u8, button5) \
    IN_FIELD(u16v2, wheel) \
    IN_FIELD(u16v2, move)

typedef enum {
    #define IN_FIELD(type, name) \
        P(INPUT_FIELD_ID_Mouse_, name),
    INPUT_MOUSE_DEF
    #undef IN_FIELD
} Input_Field_ID_Mouse;

typedef struct {
    #define IN_FIELD(type, name) \
        type name;
    INPUT_MOUSE_DEF
    #undef IN_FIELD
} Input_Mouse;

#if 0
typedef enum {
    INPUT_XBOX_360_ID_A,
    INPUT_XBOX_360_ID_B,
    INPUT_XBOX_360_ID_X,
    INPUT_XBOX_360_ID_Y,

    INPUT_XBOX_360_ID_LB,
    INPUT_XBOX_360_ID_RB,

    INPUT_XBOX_360_ID_Back,
    INPUT_XBOX_360_ID_Start,

    INPUT_XBOX_360_ID_LSB,
    INPUT_XBOX_360_ID_RSB,

    INPUT_XBOX_360_ID_LS,
    INPUT_XBOX_360_ID_RS,

    INPUT_XBOX_360_ID_LT,
    INPUT_XBOX_360_ID_RT,
} Input_Xbox_360_ID;

#pragma pack(push, 1)
typedef struct {
    u8 pad; // ???

    u16v2 LS;
    u16v2 RS;

    u8  LT;
    u8  RT;

    u8  A : 1;
    u8  B : 1;
    u8  X : 1;
    u8  Y : 1;

    u8  LB : 1;
    u8  RB : 1;

    u8  back  : 1;
    u8  start : 1;

    u8  LSB : 1;
    u8  RSB : 1;

    u8  DPad : 4;
} Input_Xbox_360;
#pragma pack(pop)
#endif

typedef enum {
    INPUT_DEVICE_TAG_MOUSE,
    INPUT_DEVICE_TAG_KEYBOARD,
    INPUT_DEVICE_TAG_XBOX_360,

    INPUT_DEVICE_TAG_COUNT,
} Input_Device_Tag;

typedef enum {
    INPUT_TYPE_u8,
    INPUT_TYPE_U16V2,
} Input_Type;

typedef struct {
    Input_Device_Tag device;
    u32 ID;

    Input_Type type;
    union {
        u8 bit;
        u16v2 vec;
    };
} Input_Change;

typedef struct {
    Input_Type type;
} Input_Unit_Metadata;
Slice(Input_Unit_Metadata);

typedef Slice_Input_Unit_Metadata Input_Device_Metadata;
Slice(Input_Device_Metadata);

typedef struct {
    Slice_Input_Unit_Metadata units;
    Slice_Input_Device_Metadata deivces;
} Input_Metadata;

Private Global Input_Metadata gInputMetadata;

#endif // INPUT_H
