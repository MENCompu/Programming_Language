#if 0
        s32 actionSpace = 40;
        Rect actionRect = {
            .leftBottom = s32v2_Add(clientRect.leftBottom, (s32v2){ padding, padding }),
            .rightTop   = s32v2_Add(clientRect.leftBottom, (s32v2){ 0, actionSpace }),
        };

        Rect dataRect = {
            .leftBottom = s32v2_Add(clientRect.leftBottom, (s32v2){ padding, padding + actionSpace}),
            .rightTop   = s32v2_Add(clientRect.rightTop  , (s32v2){ -padding, -padding }),
        };

        Rect buttonRect = Rect_Create_LD(actionRect.leftBottom, (s32v2){ 20, 20 });
        f32v4 colorRect = gDataMenu.backgroundColor;
        switch ( Select_Update(0, buttonRect, cursorSS, (u64)(void*)&gDataMenu, 88) ) {
            case UI_STATE_NOT_SELECTED: {
            } break;

            case UI_STATE_ABOUT_TO: { colorRect = YELLOW; } break;

            case UI_STATE_SELECTED_INIT: { } break;
            case UI_STATE_SELECTED: { colorRect = GREEN; } break;

            NO_DEFAULT
        }
        Renderer_Draw_Rect_With_Outline(image, buttonRect, colorRect, BLACK, 0, false);

        Rect textRect = {
            .leftBottom = { buttonRect.rightTop.x, buttonRect.leftBottom.y },
            .rightTop   = { clientRect.rightTop.x - padding, buttonRect.leftBottom.y + 20 },
        };

        UI_Button(textRect, cursorSS, (UI_ID){  })
        UI_Text_Editor_Begin(void* ID, String text, s32v2 pos);

        if ( Check_Text(&gDataMenu) ) {
            dataAsText = gUIState.textEditor.buffer;
        }
#endif

#if 0
    Array_Symbol_Data symbolDatas = gSymbolState.symbolDataPool;
    foreach (Symbol_Data, symbolData, symbolDatas) {
        if ( symbolData->edit.deleted ) { continue; }

        f32v4 rectColor = Color_Grey_Scale(0.9f);
        f32v4 outlineColor = BLACK;

        Local_Str(buffer, KiB(16));
        String dataAsText = { 0 };

        if ( Check_Text(symbolData) ) {
            dataAsText = gUIState.textEditor.buffer;

            if ( !gUIState.textEditor.using ) {
                switch ( symbolData->tag ) {
                    case SYMBOL_DATA_TAG_u32: {
                        Str_Parse_Fmt(dataAsText, "%u", (u32*)symbolData->data);
                    } break;

                    case SYMBOL_DATA_TAG_u8: {
                        Str_Parse_Fmt(dataAsText, "%hhu", (u8*)symbolData->data);
                    } break;

                    case SYMBOL_DATA_TAG_String: {
                        Str_Alloc_And_Copy(&gPermArena, (String*)symbolData->data, dataAsText);
                    } break;

                    case SYMBOL_DATA_TAG_COUNT:
                    NO_DEFAULT
                }
            }
        } else {
            switch ( symbolData->tag ) {
                case SYMBOL_DATA_TAG_u32: {
                    dataAsText = buffer;
                    Str_From_Fmt(&dataAsText, "%u", *(u32*)symbolData->data);
                } break;

                case SYMBOL_DATA_TAG_u8: {
                    dataAsText = buffer;
                    Str_From_Fmt(&dataAsText, "%hhu", *(u8*)symbolData->data);
                } break;

                case SYMBOL_DATA_TAG_String: {
                    dataAsText = *(String*)symbolData->data;
                } break;

                case SYMBOL_DATA_TAG_COUNT:
                NO_DEFAULT
            }
        }

        f32 fontScale = 0.5f;

        Font* font = Asset_Get(Font, S("font.fnt"));
        Rect textRect = Rederer_Get_Text_Rect(font, dataAsText, fontScale);
        s32v2 textDims = Rect_Get_Dims(textRect);

        s32 padding = 4;
        Rect rect = Rect_Create_CH(symbolData->edit.pos, (s32v2){ textDims.width / 2 + padding, textDims.height / 2 + padding});

        if ( Check_Text(symbolData) ) {
            gUIState.textEditor.pos = s32v2_Sub(symbolData->edit.pos, (s32v2){ textDims.width / 2, textDims.height / 2 });
        }

        switch ( Select_Update(MULTI_SELECTOR, rect, cursorWS, UI_OBJECT_TAG_SYMBOL_DATA, (u64)symbolData, 0) ) {
            case UI_STATE_NOT_SELECTED: {} break;

            case UI_STATE_ABOUT_TO: {
                rectColor = YELLOW;

                if ( Select_Exists(UI_OBJECT_TAG_CHILD_SYMBOL)  ) {
                    UI_Object* object = Select_Get(UI_OBJECT_TAG_CHILD_SYMBOL, 0, false);
                    *Select_Data(object, UI_Child)->childRef = Symbol_Data_Ptr(symbolData);
                }

                if ( Input_Button_Is_Press(gProgramState.input.mouse.button2) ) {
                    UI_Text_Editor_Begin(symbolData, dataAsText,
                                         s32v2_Sub(symbolData->edit.pos, (s32v2){ textDims.width / 2, textDims.height / 2 }));
                }
            } break;

            case UI_STATE_SELECTED_INIT: {} break;

            case UI_STATE_SELECTED: {
                rectColor = GREEN;

                if ( !gUIState.textEditor.using && Program_Input_Keyboard_Is_Press(II_Kbd_X) ) {
                    symbolData->edit.deleted = true;
                }

                if ( gUIState.pulsing ) { // Move
                    s32v2_AddEq(&symbolData->edit.pos, s32v2_Sub(cursorWS, prevCursorWS));
                }
            } break;

            NO_DEFAULT
        }

        Renderer_Draw_Rect_With_Outline(image, rect, rectColor, outlineColor, 0, relToCamera);
        (void)Rederer_Draw_Text(image, font, dataAsText, s32v2_Add(rect.leftBottom, (s32v2){ padding, padding }), fontScale, WHITE, relToCamera);
    }
#endif

#if 0
    tempID = (UI_ID){ .parent = UI_OBJECT_TAG_CHILD_SYMBOL, .state = UI_STATE_SELECTED };
    if ( Select_Exists(tempID) ) {
        UI_Child* data = Select_Data_ID(UI_Child, tempID);

        if ( *data->childRef ) {
            Render_Child_Reference(image, *data->childRef, data->childPos, data->symbolRect, data->symbol);
        } else {
            Renderer_Draw_Line(image, data->childPos, cursorWS, BLACK, 4, relToCamera);
        }
    }
#endif

#if 0
    if ( gUIState.isDataMenuActive ) {
        Font* font = Asset_Get(Font, S("font.fnt"));
        s32v2 textRectDims = { 200, font->lineHeight + 10 };
        s32v2 dataInfoPos = { 0, image->dims.height - textRectDims.height };

        foreach (Symbol_Data_Info, dataInfo, gSymbolDataInfos) {
            f32v4 rectColor = Color_Grey_Scale(0.7f);
            f32v4 textColor = BLACK;

            Rect rect = { .leftBottom = dataInfoPos, .rightTop = s32v2_Add(dataInfoPos, textRectDims) };

            if ( Rect_Is_Point_Inside(rect, cursorSS) ) {
                rectColor = YELLOW;

                if ( Has_Clicked() ) {
                    (void)Symbol_Data_Create(gUIState.tempDataPos, dataInfo->tag, 0);
                    gUIState.isDataMenuActive = false;
                }
            }

            Renderer_Draw_Rect_With_Outline(image, rect, rectColor, BLACK, 0, false);

            Rederer_Draw_Text(image, font, dataInfo->typeName, (s32v2){ dataInfoPos.x + 5, dataInfoPos.y + 5 }, 0.5f, textColor, false);
            dataInfoPos.y -= textRectDims.height;
        }
    }
#endif

