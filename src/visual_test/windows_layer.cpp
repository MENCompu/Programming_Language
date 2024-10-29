Private Global OS_State gStateOS;

typedef struct {
    u32 volatile size;
    u32 volatile index;

    Job_Entry Q[256];
} Job_Queue;

Private Global HANDLE dirThread;
Private Global HANDLE semaphoreDir;

Private Global HANDLE threadOS;
Private Global HANDLE semaphoreOS;
Private Global Job_Queue queueOS;

Public u32 OS_Get_Page_Size() {
    SYSTEM_INFO systemInfo;

    GetSystemInfo(&systemInfo);

    u32 pageSize = (u32)systemInfo.dwPageSize;

    return pageSize;
}

#if 0
// TODO(JENH): Should not crash when failing to register devices.
Private void Win32_Init_Raw_Input(HWND window) {
    RAWINPUTDEVICE devices[3];
    RAWINPUTDEVICE *keyboard = &devices[0];
    RAWINPUTDEVICE *mouse = &devices[1];
    RAWINPUTDEVICE *controller = &devices[2];

    // NOTE(JENH): HID Clients Supported in Windows.
    keyboard->usUsagePage = 0x0001;
    keyboard->usUsage = 0x0006;
    keyboard->dwFlags = RIDEV_NOLEGACY;
    keyboard->hwndTarget = window;

#if 1
    mouse->usUsagePage = 0x0001;
    mouse->usUsage = 0x0002;
    mouse->dwFlags = RIDEV_NOLEGACY;
    mouse->hwndTarget = window;

    controller->usUsagePage = 0x0001;
    controller->usUsage = 0x0005;
    controller->dwFlags = 0;
    controller->hwndTarget = window;

    s32 displayCounter = ShowCursor(JENH_FALSE);
    Assert(displayCounter < 0);
#endif

    Win32_Check( RegisterRawInputDevices(devices, ArrayCount(devices), sizeof(RAWINPUTDEVICE)), > 0 );
}

Fn_Job(TSetMouseRelative) {
    // TODO(JENH): Shuold set mouse position to the center of the screen for absolute movement of mouse in raw input.
    RAWINPUTDEVICE mouse;

    s32 displayCounter;
    while((displayCounter = ShowCursor(JENH_FALSE)) >= 0);
    Assert(displayCounter < 0);

    mouse.usUsagePage = 0x0001;
    mouse.usUsage = 0x0002;
    mouse.dwFlags = RIDEV_NOLEGACY;
    mouse.hwndTarget = gStateOS.window;

    Win32_Check( RegisterRawInputDevices(&mouse, 1, sizeof(RAWINPUTDEVICE)), > 0 );
}

Fn_Job(TSetMouseAbsolute) {
    RAWINPUTDEVICE mouse;

    while(ShowCursor(JENH_TRUE) < 0);
    //Assert(displayCounter >= 0);

    mouse.usUsagePage = 0x0001;
    mouse.usUsage = 0x0002;
    mouse.dwFlags = RIDEV_REMOVE;
    mouse.hwndTarget = 0;

    Win32_Check( RegisterRawInputDevices(&mouse, 1, sizeof(RAWINPUTDEVICE)), > 0 );
}
#endif

Private b8 Controller_Button_Change(u8 inButton, Input_Index inInputIndex, Input_Index* outInputIndex, b8* outIsPress) {
    Input_Mapping* mapping = Input_Get_Mappings();
    Bit_Flags_Inputs_State *inputsState = &mapping->inputsState;

    if ( inButton && !Bit_Flags_Is_Set(inputsState, inInputIndex) ) {
        *outInputIndex = inInputIndex;
        *outIsPress = JENH_TRUE;
        return JENH_TRUE;
    } else if ( !inButton && Bit_Flags_Is_Set(inputsState, inInputIndex) ) {
        *outInputIndex = inInputIndex;
        *outIsPress = JENH_FALSE;
        return JENH_TRUE;
    }

    return JENH_FALSE;
}

#if 0
Intern inline void WIN32_ProcessPendingEvents(MSG *event, u32* outSleepTime) {
	switch (event->message) {
	    case WM_QUIT: {
            Program_Close();
	    } break;

        case WM_MOUSEMOVE: {
            s32 x = (s32)LOWORD(event->lParam);
            s32 y = (s32)HIWORD(event->lParam);

            gInputSystem.mouseCooldown = 10;
            *outSleepTime = gInputSystem.mouseCooldown;

            Input_Mapping* mapping = Input_Get_Mappings();

            Extended_Input* extendedInput = &mapping->extendedInputs[ExtendedInput(II_Ms_Movement)];
            extendedInput->data.moveX = x;
            extendedInput->data.moveY = y;

            Input_Update_Actions(II_Ms_Movement, JENH_TRUE, 0.0f);

            //extendedInput->actionIDAttached = A_H_UI_Cursor;
        } break;

        case WM_LBUTTONDOWN: {
            Input_Mapping* mapping = Input_Get_Mappings();
            Bit_Flags_Inputs_State *inputsState = &mapping->inputsState;

            Assert(!Bit_Flags_Is_Set(inputsState, II_Ms_Button1));

            Input_Update_Actions(II_Ms_Button1, JENH_TRUE, 0.0f);
        } break;

        case WM_LBUTTONUP: {
            Input_Mapping* mapping = Input_Get_Mappings();
            Bit_Flags_Inputs_State *inputsState = &mapping->inputsState;

            Input_Update_Actions(II_Ms_Button1, JENH_FALSE, 0.0f);
        } break;

	    default: {
		    DispatchMessageA(event);
	    } break;
	}
}
#endif
