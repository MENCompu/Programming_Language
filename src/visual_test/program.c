Public void Program_Init(void) {
    /* init time */ {
        Time_OS_Init();

        // TODO(JENH): This should be computed better.
        gProgramState.time.startup = 0.0f;

        gProgramState.time.target = 1.0f / (f32)gProgramState.time.gameUpdateRateHz;

        gProgramState.time.lastFrameOS  = Time_OS_Counter();
        gProgramState.time.lastFrameCPU = Time_CPU_Counter();

        //gTimeState.refreshRateHz = OS_Get_Refresh_Rate();
        //gTimeState.gameUpdateRateHz = gTimeState.refreshRateHz / 2;
    }

    gProgramState.isRunning = true;
}

Public void Program_Cleanup(void) {
    Time_OS_Cleanup();
}

Public void Program_Close(void) {
    gProgramState.isRunning = false;
}

Public void Program_Time_Update(void) {
    s64 newCounterOS = Time_OS_Counter();
    gProgramState.time.lastFrameCPU = Time_CPU_Counter();

    gProgramState.time.delta = Time_OS_Seconds_Elapsed(gProgramState.time.lastFrameOS, newCounterOS);
    gProgramState.time.startup += gProgramState.time.delta;

    gProgramState.time.lastFrameOS = newCounterOS;
}

Public void Program_Input_Keyboard_Add_Press(u32 key, b8 isPress) {
    Input_Button* keyState = &gProgramState.input.kbd[key];

    keyState->hasBeenPress = isPress;

    if ( !keyState->hasBeenPress ) {
        keyState->isHeld = false;
    }
}

Public Input_Button* Input_Keyboard_Get_Key(Input_Keyboard_ID keyID) {
    return &gProgramState.input.kbd[keyID];
}

Public b8 Input_Button_Is_Press(Input_Button button) {
    return (b8)( button.hasBeenPress && !button.isHeld );
}

Public b8 Program_Input_Keyboard_Is_Press(Input_Keyboard_ID key) {
    Input_Button* keyState = &gProgramState.input.kbd[key];
    return ( keyState->hasBeenPress && !keyState->isHeld );
}

Public b8 Program_Input_Keyboard_Is_Held(Input_Keyboard_ID key) {
    Input_Button* keyState = &gProgramState.input.kbd[key];
    return keyState->isHeld;
}

Public void Input_Button_Update(Input_Button* button) {
    button->isHeld = false;
    if ( button->hasBeenPress ) { button->isHeld = true; }
}

Public Input_Keyboard_ID Input_Keyboard_Get_Press(Input_Button* keyState) {
    for (u32 i = 0; i < Array_Count(gProgramState.input.kbd); ++i) {
        Input_Button* key = &gProgramState.input.kbd[i];

        if ( Input_Button_Is_Press(*key) ) {
            *keyState = *key;
            return (Input_Keyboard_ID)i;
        }
    }

    *keyState = (Input_Button){ 0 };
    return 0;
}

Public void Program_Input_Update(void) {
#if 0
    for (u32 deviceIndex = 0; deviceIndex < gInputMetadata.devices.count; ++deviceIndex) {
        for (u32 i = 0; i < gInputMetadata.units.count; ++i) {
            Input_Unit_Metadata* unit = &gInputMetadata.units.E[i];

            switch ( unit->type ) {
                case INPUT_TYPE_u8: {
                    if (  ) {
                        u8* gProgramState.input.heldButtons.E[i];
                        key->isHeld = true;
                    }
                } break;

                case INPUT_TYPE_U16V2: {
                } break;

                NO_DEFAULT
            }
        }
    }
#endif

    Slice_Input_Button kbd = { Array_Count(gProgramState.input.kbd), gProgramState.input.kbd };
    foreach (Input_Button, key, kbd) {
        Input_Button_Update(key);
    }

    Input_Mouse* mouse = &gProgramState.input.mouse;

    Input_Button_Update(&mouse->button1);
    Input_Button_Update(&mouse->button2);
    Input_Button_Update(&mouse->button3);
    Input_Button_Update(&mouse->button4);
    Input_Button_Update(&mouse->button1);

    mouse->wheel = (u16v2){ 0 };
    mouse->move  = (s16v2){ 0 };
}
