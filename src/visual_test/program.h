#ifndef PROGARM_H
#define PROGARM_H

typedef struct {
    f32 target;
    f32 delta;

    s64 lastFrameOS;
    u64 lastFrameCPU;

    f32 startup;

    s32 refreshRateHz;
    s32 gameUpdateRateHz;
} Program_Time;

typedef struct {
    Array_b8 heldButtons;
    Input_Keyboard kbd;
    Input_Mouse mouse;
    //Input_Xbox_360 xbox360;
} Program_Input;

typedef struct {
    b8 isActive;

    Array_f32v3 vertices;
    Array_u32 indices;
} Geometry_Builder;

typedef struct {
    f32v2 pos;
    f32v2 targetPos;
} Camera;

typedef struct {
    b8 isRunning;

    b8 isFullScreen;
    Window* window;

    f32v2 thingPos;

    Program_Time  time;
    Program_Input input;

    Camera camera;
} Program_State;

Private Global Program_State gProgramState;

Public void Program_Input_Keyboard_Add_Press(u32 key, b8 isPress);

#endif // PROGARM_H
