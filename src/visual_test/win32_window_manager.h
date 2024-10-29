typedef enum {
    EVENT_RESULT_OK,
    EVENT_RESULT_NOT_HANDLED,
} Event_Result;

typedef enum {
    EVENT_TAG_CLOSE  = WM_CLOSE,
    EVENT_TAG_INPUT  = WM_INPUT,
    EVENT_TAG_RESIZE = WM_SIZE,
    EVENT_TAG_REDRAW = WM_PAINT,
    EVENT_TAG_ACTIVE = WM_ACTIVATEAPP,
} Event_Tag;

typedef struct {
    Event_Tag tag;

    union {
        u32v2 newDims;
        b8 isActive;
        Input_Change input;
    };
} Event;

typedef struct Window Window;

#define Fn_Sig_Window_Event_CallBack(fnName) Event_Result fnName(Window* window, Event event)
typedef Fn_Sig_Window_Event_CallBack(Fn_Window_Event_CallBack);

typedef struct {
    HDC dc;
    HWND handle;
    BITMAPINFO bitmap;
    b8 isRawInputActive;
} Window_Platform;

typedef struct {
    Rect rect;
    Image* image;
} Window2;

typedef struct Window {
    Fn_Window_Event_CallBack* CallBack;

    //Rect rect;
    Image bitmap;

    u32 windowMaximizedFlags;
    Window_Platform platform;
} Window;
Array(Window);

typedef struct {
    Array_Window windowPool;
    Rect screenRect;
} Window_Manager_State;

Private Global Window_Manager_State gWindowManagerState;
