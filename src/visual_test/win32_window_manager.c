Public s32v2 Window_Get_Position(Window* window) {
    POINT point = { 0, window->bitmap.dims.y };

    Win32_Check( ClientToScreen(window->platform.handle, &point), > 0 );

    s32v2 screenDims = Rect_Get_Dims(gWindowManagerState.screenRect);

    return (s32v2){ point.x, screenDims.height - point.y };
}

Public Rect Window_Get_Client_Rect(Window* window) {
    return (Rect){
        .leftBottom = Window_Get_Position(window),
        .rightTop   = s32v2_Add(Window_Get_Position(window), (s32v2){ window->bitmap.dims.width, window->bitmap.dims.height }),
    };
}

Public Rect OS_Get_Window_Rect(Window* window) {
    RECT rectOS;
    Win32_Check( GetWindowRect(window->platform.handle, &rectOS), > 0 );

    s32v2 screenDims = Rect_Get_Dims(gWindowManagerState.screenRect);

    return (Rect){
        .leftBottom = { rectOS.left , screenDims.height - rectOS.bottom },
        .rightTop   = { rectOS.right, screenDims.height - rectOS.top    },
    };
}

Public u32v2 Window_Manager_Get_Cursor_Pos(void) {
    POINT cursorPoint;
    Win32_Check( GetCursorPos(&cursorPoint), > 0 );

    s32v2 screenDims = Rect_Get_Dims(gWindowManagerState.screenRect);

    return (u32v2){
        (u32)cursorPoint.x,
        (u32)( screenDims.height - cursorPoint.y )
    };
}

Public s32v2 Window_Manager_Get_Cursor_Rel_To_Window(Window* window) {
    u32v2 absCursor = Window_Manager_Get_Cursor_Pos();

    Rect windowRect = Window_Get_Client_Rect(window);

    return s32v2_Sub((s32v2){ .x = absCursor.x, .y = absCursor.y }, windowRect.leftBottom);
}

Public void Window_Manager_Init(Memory_Arena* arena) {
    gWindowManagerState.screenRect = (Rect){
        .leftBottom = { 0, 0 },
        .rightTop   = { (s32)GetSystemMetrics(SM_CXSCREEN), (s32)GetSystemMetrics(SM_CYSCREEN) },
    };

    Array_Create(gWindowManagerState.windowPool, KiB(4), arena);
}

Public void Window_Manager_Show_Cursor(b8 shouldShowIt) {

    if ( shouldShowIt ) {
        while( ShowCursor(true) < 0 );
        //Assert(displayCounter >= 0);
    } else {
        //s32 displayCounter;
        while( ShowCursor(false) >= 0);
        //Assert( displayCounter < 0 );
    }
}

Private void Win32_Init_Raw_Input(Window* window);
LRESULT CALLBACK Win32_Window_Event_CallBack(HWND platform, UINT tag, WPARAM wParam, LPARAM lParam);

Public Window* Window_Open(Memory_Arena* arena, String title, u32v2 dims, Fn_Window_Event_CallBack* eventCallBack) {
    Window* retWindow = Array_Push(gWindowManagerState.windowPool);
    retWindow->CallBack = eventCallBack;

    RECT clientRect = { 0, 0, dims.width, dims.height };

    AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, JENH_FALSE);

    u32v2 dimsFixed = { .x=(u32)(clientRect.right + -clientRect.left), .y=(u32)(clientRect.bottom += -clientRect.top) };

    /* initialize bitmap */ {
        s32v2 screenDims = Rect_Get_Dims(gWindowManagerState.screenRect);

        retWindow->bitmap = (Image){
            .dims = dims,
            .hasTransparency = false,
            .channelCount = 4,
            .channelSizes = {8, 8, 8, 8},
        };

        // NOTE(JENH): It seems that the plain monitor dimensions aren't enough. For that reason should add one row.
        retWindow->bitmap.data = OS_Alloc_Mem( screenDims.width * ( screenDims.height + 1 ) * Image_Get_Byte_Per_Pixel(&retWindow->bitmap)),
        retWindow->bitmap.stride = retWindow->bitmap.dims.width * Image_Get_Byte_Per_Pixel(&retWindow->bitmap);

        retWindow->platform.bitmap.bmiHeader = (BITMAPINFOHEADER){
            .biSize = sizeof(retWindow->platform.bitmap.bmiHeader),
            .biWidth = retWindow->bitmap.dims.width,
            .biHeight = retWindow->bitmap.dims.height,
            .biPlanes = 1,
            .biBitCount = (WORD)( Image_Get_Byte_Per_Pixel(&retWindow->bitmap) * BYTE_SIZE ),
            .biCompression = BI_RGB,
        };
    }

    WNDCLASSA windowClass = { 0 };

    windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
    windowClass.lpfnWndProc = Win32_Window_Event_CallBack;
    windowClass.hInstance = (HINSTANCE)GetModuleHandleA(0);
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    // windowClass.hIcon = ;

    windowClass.lpszClassName = title.E;

    if ( !RegisterClassA(&windowClass) ) {
        LogError("The window class did not register successfully");
    }

    retWindow->platform.handle = CreateWindowExA(0, windowClass.lpszClassName, title.E, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                                                 CW_USEDEFAULT, dimsFixed.width, dimsFixed.height, 0, 0, windowClass.hInstance, 0);

    if ( !retWindow->platform.handle ) {
        LogError("Failed at creating window");
    }

    retWindow->platform.dc = GetDC(retWindow->platform.handle);

    Win32_Init_Raw_Input(retWindow);

    return retWindow;
}

Public void Window_Close(Window* window) {
    CloseWindow(window->platform.handle);
}

Public void Window_Update(Window* window) {
    (void)StretchDIBits(window->platform.dc, 0, 0, window->bitmap.dims.width, window->bitmap.dims.height, 0, 0,
		  window->bitmap.dims.width, window->bitmap.dims.height, window->bitmap.data, &window->platform.bitmap,
		  DIB_RGB_COLORS, SRCCOPY);
}

Public LRESULT Window_Process_Event(Window* window, UINT tag, WPARAM wParam, LPARAM lParam) {
    LRESULT ret = 0;

    Event event = { 0 };
    event.tag = (Event_Tag)tag;

    switch ( event.tag ) {
        case WM_SIZE: {
            event.newDims = (u32v2){ .width=LOWORD(lParam), .height=HIWORD(lParam) };
        } break;

#if 1
        case WM_SETCURSOR: {
            (void)SetCursor(LoadCursor(0, IDC_ARROW));
        } break;
#endif

        case WM_ACTIVATEAPP: {
            event.isActive = (b8)wParam;
        } break;

        case WM_MOUSEMOVE: {
            u32v2 absCursor = Window_Manager_Get_Cursor_Pos();

            if ( Rect_Is_Point_Inside(Window_Get_Client_Rect(window), (s32v2){ (s32)absCursor.x , (s32)absCursor.y  }) ) {
                //LogInfo("relative");

                // TODO(JENH): Shuold set mouse position to the center of the screen for absolute movement of mouse in raw input.
                RAWINPUTDEVICE mouse;

                mouse.usUsagePage = 0x0001;
                mouse.usUsage = 0x0002;
                mouse.dwFlags = RIDEV_NOLEGACY;
                mouse.hwndTarget = window->platform.handle;

                Win32_Check( RegisterRawInputDevices(&mouse, 1, sizeof(RAWINPUTDEVICE)), > 0 );

                //Window_Manager_Show_Cursor(false);
            }
        } break;

        case WM_INPUT: {
            RAWINPUT* rawInput;
            UINT dataSize;
            Win32_Check( GetRawInputData((HRAWINPUT)lParam, RID_INPUT, 0, &dataSize, sizeof(RAWINPUTHEADER)), != (UINT)-1 );

            rawInput = (RAWINPUT*)Arena_Alloc_Mem(&gTempArena, (u32)dataSize);

            u32 bytesWritten;
            Win32_Check( (bytesWritten = GetRawInputData((HRAWINPUT)lParam, RID_INPUT, rawInput,
                                                          &dataSize, sizeof(RAWINPUTHEADER))), != (UINT)-1 && bytesWritten == dataSize );

            // TODO(JENH): Handle error scan codes (0x00, 0xff) and other protocol scan codes.
            //             https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html

            switch ( (u8)rawInput->header.dwType ) {
                case RIM_TYPEMOUSE: {
                    event.input.device = INPUT_DEVICE_TAG_MOUSE;
                    RAWMOUSE* rawMouse = &rawInput->data.mouse;

                    u32 buttonFlags = (u32)rawMouse->usButtonFlags;
                    u32 flags = (u32)rawMouse->usFlags;

                    Assert( !Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_1_DOWN|RI_MOUSE_BUTTON_1_UP) &&
                            !Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_2_DOWN|RI_MOUSE_BUTTON_2_UP) &&
                            !Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_3_DOWN|RI_MOUSE_BUTTON_3_UP) &&
                            !Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_4_DOWN|RI_MOUSE_BUTTON_4_UP) &&
                            !Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_5_DOWN|RI_MOUSE_BUTTON_5_UP) );

                    //MOUSE_MOVE_ABSOLUTE
                    Assert( Flags_Has_All(rawMouse->usFlags, MOUSE_MOVE_RELATIVE) );

#if 1
                    if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_1_DOWN) ) {
                        gProgramState.input.mouse.button1.hasBeenPress = true;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_1_UP) ) {
                        gProgramState.input.mouse.button1.hasBeenPress = false;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_2_DOWN) ) {
                        gProgramState.input.mouse.button2.hasBeenPress = true;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_2_UP) ) {
                        gProgramState.input.mouse.button2.hasBeenPress = false;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_3_DOWN) ) {
                        gProgramState.input.mouse.button3.hasBeenPress = true;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_3_UP) ) {
                        gProgramState.input.mouse.button3.hasBeenPress = false;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_4_DOWN) ) {
                        gProgramState.input.mouse.button4.hasBeenPress = true;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_4_UP) ) {
                        gProgramState.input.mouse.button4.hasBeenPress = false;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_5_DOWN) ) {
                        gProgramState.input.mouse.button5.hasBeenPress = true;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_5_UP) ) {
                        gProgramState.input.mouse.button5.hasBeenPress = false;
                    } else if ( !Mem_Equal_Zero_Type(&rawMouse->usButtonData) ) {
                        u16v2_AddEq(&gProgramState.input.mouse.wheel, (u16v2){ 0, rawMouse->usButtonData });
                    } else if ( rawMouse->lLastX != 0 && rawMouse->lLastY != 0 ) {
                        s16v2_AddEq(&gProgramState.input.mouse.move, (s16v2){ (s16)rawMouse->lLastX, -(s16)rawMouse->lLastY });
                    }
#else
                    if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_1_DOWN) ) {
                        event.inputChange.ID = INPUT_FIELD_ID_Mouse_button1;
                        event.inputChange.bit = true;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_1_UP) ) {
                        event.inputChange.ID = INPUT_FIELD_ID_Mouse_button1;
                        event.inputChange.bit = false;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_2_DOWN) ) {
                        event.inputChange.ID = INPUT_FIELD_ID_Mouse_button2;
                        event.inputChange.bit = true;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_2_UP) ) {
                        event.inputChange.ID = INPUT_FIELD_ID_Mouse_button2;
                        event.inputChange.bit = false;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_3_DOWN) ) {
                        event.inputChange.ID = INPUT_FIELD_ID_Mouse_button3;
                        event.inputChange.bit = true;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_3_UP) ) {
                        event.inputChange.ID = INPUT_FIELD_ID_Mouse_button3;
                        event.inputChange.bit = false;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_4_DOWN) ) {
                        event.inputChange.ID = INPUT_FIELD_ID_Mouse_button4;
                        event.inputChange.bit = true;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_4_UP) ) {
                        event.inputChange.ID = INPUT_FIELD_ID_Mouse_button4;
                        event.inputChange.bit = false;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_5_DOWN) ) {
                        event.inputChange.ID = INPUT_FIELD_ID_Mouse_button5;
                        event.inputChange.bit = true;
                    } else if ( Flags_Has_All(buttonFlags, RI_MOUSE_BUTTON_5_UP) ) {
                        event.inputChange.ID = INPUT_FIELD_ID_Mouse_button5;
                        event.inputChange.bit = false;
                    } else if ( !Mem_Equal_Zero_Type(&rawMouse->usButtonData) ) {
                        event.inputChange.ID = INPUT_FIELD_ID_Mouse_wheel;
                        event.inputChange.vec = (u16v2){ 0, rawMouse->usButtonData };
                    } else if ( rawMouse->lLastX != 0 && rawMouse->lLastY != 0 ) {
                        event.inputChange.ID = INPUT_FIELD_ID_Mouse_move;
                        event.inputChange.vec = (u16v2){ (u16)rawMouse->lLastX, (u16)rawMouse->lLastY };
                    }
#endif

#if 1
                    /* handle cursor outside of window */ {
                        u32v2 absCursor = Window_Manager_Get_Cursor_Pos();

                        if ( !Rect_Is_Point_Inside(Window_Get_Client_Rect(window), (s32v2){ (s32)absCursor.x , (s32)absCursor.y }) ) {
                            if ( Mem_Equal_Zero_Array(gProgramState.input.mouse.buttons) ) {
                                //LogInfo("absolute");

                                RAWINPUTDEVICE mouse;

                                mouse.usUsagePage = 0x0001;
                                mouse.usUsage = 0x0002;
                                mouse.dwFlags = RIDEV_REMOVE;
                                mouse.hwndTarget = 0;

                                Win32_Check( RegisterRawInputDevices(&mouse, 1, sizeof(RAWINPUTDEVICE)), > 0 );

                                //Window_Manager_Show_Cursor(false);
                            }
                        }
                    }
#endif
                } break;

                case RIM_TYPEKEYBOARD: {
                    event.input.device = INPUT_DEVICE_TAG_KEYBOARD;

                    RAWKEYBOARD* rawKbd = &rawInput->data.keyboard;

                    u32 scanCode = (u32)rawKbd->MakeCode;
                    Assert( scanCode < 0xff );

                    b8 isPress = 0;
                    u32 key = scanCode;

                    if ( Flags_Has_Any(rawKbd->Flags, RI_KEY_E0|RI_KEY_E1) ) {
                        key |= KBD_EXT_INDEX_MASK;
                    }

                    if ( Flags_Has_All(rawKbd->Flags, RI_KEY_BREAK) ) {
                        isPress = false;
                    } else if ( Flags_Has_All(rawKbd->Flags, RI_KEY_MAKE) ) {
                        isPress = true;
                    } NO_ELSE

                    Program_Input_Keyboard_Add_Press(key, isPress);
                } break;

                case RIM_TYPEHID: {
#if 0
                    event.input.device = INPUT_DEVICE_TAG_XBOX_360;
                    event.input.xbox360 = *(Input_Xbox_360*)rawInput->data.hid.bRawData;
#endif
                } break;

                NO_DEFAULT
            }

        } break;

        case WM_CLOSE:
        case WM_PAINT: break;
    }

    Event_Result result = window->CallBack(window, event);

    if ( result == EVENT_RESULT_NOT_HANDLED ) {
        ret = DefWindowProcA(window->platform.handle, tag, wParam, lParam);
    }

    return ret;
}

Public void Window_Process_Pool_Events(Window* window) {
    MSG msg;

    while ( PeekMessageA(&msg, 0, 0, 0, PM_REMOVE) ) {
        Window_Process_Event(window, msg.message, msg.wParam, msg.lParam);
    }
}

LRESULT CALLBACK Win32_Window_Event_CallBack(HWND platformHandle, UINT tag, WPARAM wParam, LPARAM lParam) {
    Window* window = 0;
    foreach (Window, iterWindow, gWindowManagerState.windowPool) {
        if ( iterWindow->platform.handle == platformHandle ) {
            window = iterWindow;
            goto found;
        } else if ( !iterWindow->platform.handle ) {
            iterWindow->platform.handle = platformHandle;
            window = iterWindow;
            goto found;
        }
    }

    /* IF NOT found */ {
        LogFatal("Failed to find window platform handle");
    }
found:

    return Window_Process_Event(window, tag, wParam, lParam);
}

Public s32 Window_Get_Refresh_Rate(Window* window) {
    HDC refreshDC = GetDC(window->platform.handle);
    s32 actualRefreshRate = GetDeviceCaps(refreshDC, VREFRESH);
    ReleaseDC(window->platform.handle, refreshDC);

    return ( actualRefreshRate > 1 ) ? actualRefreshRate : 60;
}

Public void Window_Update_Dims(Window* window, u32v2 newDims) {
    window->bitmap.dims = newDims;

    window->platform.bitmap.bmiHeader.biWidth  = newDims.width;
    window->platform.bitmap.bmiHeader.biHeight = newDims.height;
}

Public u32v2 OS_Window_Get_Dims(Window* window) {
    u32v2 ret = {0};

    RECT rect;
    GetClientRect(window->platform.handle, &rect);

    ret.width  = (u32)( rect.right - rect.left );
    ret.height = (u32)( rect.bottom - rect.top );

    return ret;
}

Public void Window_FullScreen_Size(Window* window) {
    // HWND_TOPMOST
    LONG windowStyle = GetWindowLong(window->platform.handle, GWL_STYLE);
    window->windowMaximizedFlags = (u32)( windowStyle & WS_MAXIMIZE );
    SetWindowLongPtrA(window->platform.handle, GWL_STYLE, windowStyle & ~(WS_OVERLAPPEDWINDOW | window->windowMaximizedFlags));
    SetWindowPos(window->platform.handle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
    ShowWindow(window->platform.handle, SW_SHOWMAXIMIZED);
}

Public void Window_Normal_Size(Window* window) {
    LONG windowStyle = GetWindowLong(window->platform.handle, GWL_STYLE);
    LONG newStyle = windowStyle|WS_OVERLAPPEDWINDOW;
    SetWindowLongPtrA(window->platform.handle, GWL_STYLE, newStyle);
    SetWindowPos(window->platform.handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
    ShowWindow(window->platform.handle, SW_RESTORE);
    SetWindowLongPtrA(window->platform.handle, GWL_STYLE, (newStyle & ~WS_MAXIMIZE) | window->windowMaximizedFlags);
}

Public char OS_Keyboard_Key_To_Letter(Input_Keyboard_ID key) {
    if ( key & KBD_EXT_INDEX_MASK ) {
        return (char)0;
    }

    BYTE rawKbdState[256];

    Win32_Check( GetKeyboardState(rawKbdState), > 0 );

    WORD keyWord;
    if ( !ToAscii(MapVirtualKeyA(key, MAPVK_VSC_TO_VK), key, rawKbdState, &keyWord, false) ) { return '\0'; }

    return (char)keyWord;
}

Private void Win32_Init_Raw_Input(Window* window) {
    RAWINPUTDEVICE devices[] = {
        /* keyboard */ {
            .usUsagePage = 0x0001, // NOTE(JENH): HID Clients Supported in Windows.
            .usUsage = 0x0006,
            .dwFlags = RIDEV_NOLEGACY,
            .hwndTarget = window->platform.handle,
        },

#if 0
        /* mouse */ {
            .usUsagePage = 0x0001,
            .usUsage = 0x0002,
            .dwFlags = RIDEV_NOLEGACY,
            .hwndTarget = window->platform.handle,
        },
#endif

        /* controller */ {
            .usUsagePage = 0x0001,
            .usUsage = 0x0005,
            .dwFlags = 0,
            .hwndTarget = window->platform.handle,
        },
    };

    Win32_Check( RegisterRawInputDevices(devices, Array_Count(devices), sizeof(RAWINPUTDEVICE)), > 0 );

    window->platform.isRawInputActive = true;
}
