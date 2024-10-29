Private Global f32v2 linePos = { 400, 300 };
Private Global f32 angle;

Public void Manual_Test_Line(Image* image) {
    f32 speed = 100.0f;

    if ( Program_Input_Keyboard_Is_Held(II_Kbd_W) ) {
        linePos.y += gProgramState.time.delta * speed;
    } if ( Program_Input_Keyboard_Is_Held(II_Kbd_S) ) {
        linePos.y -= gProgramState.time.delta * speed;
    } if ( Program_Input_Keyboard_Is_Held(II_Kbd_D) ) {
        linePos.x += gProgramState.time.delta * speed;
    } if ( Program_Input_Keyboard_Is_Held(II_Kbd_A) ) {
        linePos.x -= gProgramState.time.delta * speed;
    } if ( Program_Input_Keyboard_Is_Held(II_Kbd_Left) ) {
        angle += 2.0f*JENH_PI * gProgramState.time.delta;
    } if ( Program_Input_Keyboard_Is_Held(II_Kbd_Right) ) {
        angle -= 2.0f*JENH_PI * gProgramState.time.delta;
    }

    Renderer_Clear(image, (f32v4){ 1.0f, 1.0f, 1.0f, 1.0f });

    f32 scale = 200.0f;

    s32v2 begin = f32v2_To_s32v2(linePos);
    s32v2 end   = f32v2_To_s32v2(f32v2_Add(linePos, (f32v2){ F32_Cos(angle) * scale, F32_Sin(angle) * scale }));

    Renderer_Draw_Line(image, begin, end, BLACK, 0, false);
}

Public void Manual_Test_Text(Image* image) {
    s32v2 pos = { 200, 200 };
    f32v4 color = BLACK;
    f32 scale = 0.5f;

    Font* font = Asset_Get(Font, S("font.fnt"));

    Code_Point_Data* data = Font_Get_Code_Point(font, 'C');

    Rect codePointRect = Rect_Create_LD((s32v2){ (s32)data->pos.x, (s32)data->pos.y },
                                        (s32v2){ (s32)data->dims.width, (s32)data->dims.height });

    Rect dstRect = Rect_Create_LD(s32v2_Add(pos, (s32v2){ (s32)data->offset.x, (s32)data->offset.y }),
                                  (s32v2){ (s32)( (f32)data->dims.width * scale ), (s32)( (f32)data->dims.height * scale ) });

#if 0
    Window2 window = {
        .image = image,
        .rect  = Rect_From_Image(image),
    };
#endif

    Renderer_Draw_Image(image, dstRect, font->bitmap, codePointRect, color);
}

#if 0
        /* movement button */ {
            s32v2 buttonHalfDims = { 4, 4 };
            s32v2 buttonCenter = s32v2_Add(symbol->pos, (s32v2){ 0, symbolHalfDims.height + buttonHalfDims.height });
            Rect buttonRect = Rect_Create_CH(buttonCenter, buttonHalfDims);
            f32v4 buttonColor = Color_Grey_Scale(0.5f);

            if ( Rect_Is_Point_Inside(buttonRect, cursorWS) ) {
                buttonColor = YELLOW;

                if ( Has_Clicked() ) {
                    UIState.hotObject = (UI_Object){ .held = &gProgramState.input.mouse.button1, .ID = UI_OBJECT_TAG_MOVE_SYMBOL, .extID = symbol };
                }
            }

            if ( Check_Hot(UI_OBJECT_TAG_MOVE_SYMBOL, symbol) ) {
                buttonColor = GREEN;
                s32v2 newPos = cursorWS;
                newPos.y -= symbolHalfDims.height + buttonHalfDims.height;
                symbol->pos = newPos;
            }

            Renderer_Draw_Rect_With_Outline(image, buttonRect, buttonColor, outlineColor, 0, relToCamera);
        }
#endif
