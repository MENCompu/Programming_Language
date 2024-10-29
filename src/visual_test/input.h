#ifndef INPUT_H
#define INPUT_H

// NOTE(JENH): See keyboards scan codes
//             https://www.plantation-productions.com/Webster/www.artofasm.com/DOS/pdf/apndxc.pdf
//             Names are from QWERTY keyboard layout.
#define KBD_EXT_INDEX_MASK 0b10000000

typedef enum {
    II_Nul                = 0x00,
    II_Kbd_Esc            = 0x01,
    II_Kbd_1              = 0x02,
    II_Kbd_2              = 0x03,
    II_Kbd_3              = 0x04,
    II_Kbd_4              = 0x05,
    II_Kbd_5              = 0x06,
    II_Kbd_6              = 0x07,
    II_Kbd_7              = 0x08,
    II_Kbd_8              = 0x09,
    II_Kbd_9              = 0x0a,
    II_Kbd_0              = 0x0b,
    II_Kbd_Hyphen         = 0x0c,
    II_Kbd_Equal          = 0x0d,
    II_Kbd_BackSpace      = 0x0e,
    II_Kbd_Tab            = 0x0f,
    II_Kbd_Q              = 0x10,
    II_Kbd_W              = 0x11,
    II_Kbd_E              = 0x12,
    II_Kbd_R              = 0x13,
    II_Kbd_T              = 0x14,
    II_Kbd_Y              = 0x15,
    II_Kbd_U              = 0x16,
    II_Kbd_I              = 0x17,
    II_Kbd_O              = 0x18,
    II_Kbd_P              = 0x19,
    II_Kbd_Sqr_Bcks_Open  = 0x1a,
    II_Kbd_Sqr_Bcks_Close = 0x1b,
    II_Kbd_Enter          = 0x1c,
    II_Kbd_Ctrl_L         = 0x1d,
    II_Kbd_A              = 0x1e,
    II_Kbd_S              = 0x1f,
    II_Kbd_D              = 0x20,
    II_Kbd_F              = 0x21,
    II_Kbd_G              = 0x22,
    II_Kbd_H              = 0x23,
    II_Kbd_J              = 0x24,
    II_Kbd_K              = 0x25,
    II_Kbd_L              = 0x26,
    II_Kbd_Semicolon      = 0x27,
    II_Kbd_Quotation      = 0x28,
    II_Kbd_Apostrophe     = 0x29,
    II_Kbd_Shift_L        = 0x2a,
    II_Kbd_Backslash      = 0x2b,
    II_Kbd_Z              = 0x2c,
    II_Kbd_X              = 0x2d,
    II_Kbd_C              = 0x2e,
    II_Kbd_V              = 0x2f,
    II_Kbd_B              = 0x30,
    II_Kbd_N              = 0x31,
    II_Kbd_M              = 0x32,
    II_Kbd_Comma          = 0x33,
    II_Kbd_Dot            = 0x34,
    II_Kbd_Slash          = 0x35,
    II_Kbd_Shift_R        = 0x36,
    II_Kbd_Asterisk       = 0x37,
    II_Kbd_Alt_L          = 0x38,
    II_Kbd_Space          = 0x39,
    II_Kbd_Caps           = 0x3a,
    II_Kbd_F1             = 0x3b,
    II_Kbd_F2             = 0x3c,
    II_Kbd_F3             = 0x3d,
    II_Kbd_F4             = 0x3e,
    II_Kbd_F5             = 0x3f,
    II_Kbd_F6             = 0x40,
    II_Kbd_F7             = 0x41,
    II_Kbd_F8             = 0x42,
    II_Kbd_F9             = 0x43,
    II_Kbd_F10            = 0x44,
    II_Kbd_NumLock        = 0x45,
    II_Kbd_rollLock       = 0x46,
    II_Kbd_Numpad_7       = 0x47,
    II_Kbd_Numpad_8       = 0x48,
    II_Kbd_Numpad_9       = 0x49,
    II_Kbd_Numpad_Minus   = 0x4a,
    II_Kbd_Numpad_4       = 0x4b,
    II_Kbd_Numpad_5       = 0x4c,
    II_Kbd_Numpad_6       = 0x4d,
    II_Kbd_Numpad_Plus    = 0x4e,
    II_Kbd_Numpad_1       = 0x4f,
    II_Kbd_Numpad_2       = 0x50,
    II_Kbd_Numpad_3       = 0x51,
    II_Kbd_Numpad_0       = 0x52,
    II_Kbd_Del            = 0x53,
    II_Kbd_F11            = 0x57,
    II_Kbd_F12            = 0x58,
    //II_Kbd_F12            = 0x59,
    II_Kbd_Slash_2        = KBD_EXT_INDEX_MASK|0x35,
    II_Kbd_Enter_2        = KBD_EXT_INDEX_MASK|0x1c,
    II_Kbd_Ins_2          = KBD_EXT_INDEX_MASK|0x52,
    II_Kbd_Del_2          = KBD_EXT_INDEX_MASK|0x53,
    II_Kbd_Home_2         = KBD_EXT_INDEX_MASK|0x47,
    II_Kbd_End_2          = KBD_EXT_INDEX_MASK|0x4f,
    II_Kbd_Pageup_2       = KBD_EXT_INDEX_MASK|0x49,
    II_Kbd_Pagedown_2     = KBD_EXT_INDEX_MASK|0x51,
    II_Kbd_Left           = KBD_EXT_INDEX_MASK|0x4b,
    II_Kbd_Right          = KBD_EXT_INDEX_MASK|0x4d,
    II_Kbd_Up             = KBD_EXT_INDEX_MASK|0x48,
    II_Kbd_Down           = KBD_EXT_INDEX_MASK|0x50,
    II_Kbd_Alt_R          = KBD_EXT_INDEX_MASK|0x38,
    II_Kbd_Ctrl_R         = KBD_EXT_INDEX_MASK|0x1d,
    II_Kbd_Pause          = KBD_EXT_INDEX_MASK|0x7f,

    INPUT_INDEX_KBD_MAX = 0b111111111,
} Input_Keyboard_ID;

typedef struct {
    b8 isPress;
    Input_Keyboard_ID key;
} Input_Keyboard_Key_Press;

typedef struct {
    b8 hasBeenPress;
    b8 isHeld;
} Input_Button;
Slice(Input_Button);

typedef Input_Button Input_Keyboard[INPUT_INDEX_KBD_MAX+1];

typedef enum {
    INPUT_MOUSE_ID_BUTTON1,
    INPUT_MOUSE_ID_BUTTON2,
    INPUT_MOUSE_ID_BUTTON3,
    INPUT_MOUSE_ID_BUTTON4,
    INPUT_MOUSE_ID_BUTTON5,

    INPUT_MOUSE_ID_WHEEL,
    INPUT_MOUSE_ID_MOVEMENT,
} Input_Mouse_ID;

typedef struct {
    union {
        Input_Button buttons[5];
        struct {
            Input_Button button1;
            Input_Button button2;
            Input_Button button3;
            Input_Button button4;
            Input_Button button5;
        };
    };

    u16v2 wheel;
    union {
        s16v2 move;
        u16v2 pos;
    };
} Input_Mouse;

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

typedef enum {
    INPUT_DEVICE_TAG_MOUSE,
    INPUT_DEVICE_TAG_KEYBOARD,
    INPUT_DEVICE_TAG_XBOX_360,
} Input_Device_Tag;

typedef struct {
    Input_Device_Tag device;

    union {
        Input_Keyboard_Key_Press kbdKeyPress;
        Input_Mouse mouse;
        Input_Xbox_360 xbox360;
    };
} Input;

typedef struct {
    Input_Device_Tag device;

    union {
        Input_Keyboard_Key_Press kbdKeyPress;
        Input_Mouse mouse;
        Input_Xbox_360 xbox360;
    };
} Input_Change;

typedef struct {
    Input_Keyboard_Key_Press kbdKeyPress;
    Input_Mouse mouse;
    Input_Xbox_360 xbox360;
} Input_State;

typedef struct {
    Input_State* current;
    Input_State* next;

    Input_State states[2];
} Input_System;

#endif // INPUT_H
