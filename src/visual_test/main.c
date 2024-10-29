#pragma warning(push, 1)
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <stdio.h>
#pragma warning(pop)

#include <entry.h>

#include <types.h>
#include <assert.h>
#include <utils.h>
#include <Memory_Utils.h>
#include <data_structures.h>
#include <memory_managment.h>
#include <jenh_string.h>
#include <logger.h>
#include <jenh_math.h>
#include <logger.c>
#include <virtual_mem_allocator.h>
#include <jenh_string.c>
#include <memory_managment.c>
#include <data_structures.c>
#include <vectors_c.h>
#include <matrices.c>

#define BUILD_64
#include <file_format_utils.h>
#include <parse.h>

#include <time.h>
#include <time.c>

#include <file_system.h>
#include <file_system.c>

#include "assets.c"
#include "mesh_parser.c"
#include "image.c"
#include "image_parser.c"
#include "font_parser.c"

#include "input.h"
#include "win32_window_manager.h"
#include "program.h"
#include "renderer.c"
#include "win32_window_manager.c"

#include "program.c"

#include "symbol.h"

#include "test.c"

#if 0
Private Global Memory_Arena gGeometryBuilderArena;

Public void Geometry_Builder_Begin(Geometry_Builder* geoBuilder) {
    DArray_Create(geoBuilder->vertices, &gGeometryBuilderArena);
    DArray_Create(geoBuilder->indices , &gGeometryBuilderArena);
}

Public void Geometry_Builder_End(Geometry_Builder* geoBuilder) {
    Arena_Clear(&gGeometryBuilderArena);
}
#endif

Public f32v3 Screen_Point_To_World(f32v2 winDims, f32v2 screenPoint, Frustum projFrustum) {
    f32v2 NDCPoint = f32v2_Sub(f32v2_Mul(f32v2_ElemWise_Div(screenPoint, winDims), 2.0f), (f32v2){ 1.0f, 1.0f });

    f32v2 halfNearDims = {
        .width  = ( projFrustum.right - projFrustum.left ) / 2.0f,
        .height = ( projFrustum.top - projFrustum.bottom ) / 2.0f,
    };

    f32v2 nearPoint2D = f32v2_ElemWise_Mul(halfNearDims, NDCPoint);
    return f32v3_Normalize((f32v3){nearPoint2D.x, nearPoint2D.y, projFrustum.nearPlane});
}

Public s32v2 Get_Cursor_World_Space(Window* window) {
    s32v2 cursor = Window_Manager_Get_Cursor_Rel_To_Window(window);
    s32v2_SubEq(&cursor, f32v2_To_s32v2(gProgramState.camera.pos));
    return cursor;
}

typedef struct {
    b8 using;

    void* ID;
    s32v2 pos;

    u32 offset;
    String buffer;

    f32 keyCoolDown;
} UI_Text_Editor;

typedef enum {
    UI_STATE_NOT_SELECTED = 1,
    UI_STATE_ABOUT_TO = 2,
    UI_STATE_SELECTED_INIT = 3,
    UI_STATE_SELECTED = 4,
} UI_State;

typedef enum {
    UI_OBJECT_TAG_NIL,
    UI_OBJECT_TAG_CHILD_SYMBOL,
    UI_OBJECT_TAG_SYMBOL,
    UI_OBJECT_TAG_SYMBOL_DATA,
    UI_OBJECT_TAG_WINDOW,
} UI_Object_Tag;

typedef struct {
    u64 parent;
    u64 item;

    UI_State state;
} UI_ID;

typedef struct {
    UI_ID ID;
    b8 held;

    byte data[KiB(4)];
} UI_Object;
Array(UI_Object);
Array_Ref(UI_Object);

typedef struct {
    b8 using;
    Input_Button* pulsing;

    Rect rect;
    s32v2 startingPos;
} UI_Select;

typedef struct {
    Rect symbolRect;
    Symbol* symbol;
    Symbol** childRef;
    s32v2 childPos;
    UI_ID refID;
} UI_Child;

typedef struct {
    Input_Button* pulsing;

    b8 clickHasCollided;

    UI_Select select;

    UI_Text_Editor textEditor;

    b8 isDataMenuActive;
    s32v2 tempDataPos;

    Array_UI_Object objects;
} UI_State_Data;

Private Global UI_State_Data gUIState;

typedef struct {
    b8 isActive;

    f32v4 backgroundColor;
    f32v4 outlineColor;

    Rect rect;
} UI_Window;

Private Global UI_Window gDataMenu;

typedef struct {
    void* old;
    void* new;
} Copy_Data;
Array(Copy_Data);

#define Check_Text(inID) \
    ( gUIState.textEditor.ID == inID )

Public void UI_Text_Editor_Begin(void* ID, String text, s32v2 pos) {
    gUIState.textEditor = (UI_Text_Editor){
        .using = true,
        .ID = ID,
        .pos = pos,
        .offset = 0,
        .keyCoolDown = 0.0f,
        .buffer = gUIState.textEditor.buffer,
    };

    Str_Copy(&gUIState.textEditor.buffer, text);
}

Public void Text_Editor_Update(UI_Text_Editor* textEditor, Image* image, b8 relToCamera) {
    Font* font = Asset_Get(Font, S("font.fnt"));

    /* Draw cursor. */ {
        u32 offset;
        if ( textEditor->offset != 0 ) {
            offset = textEditor->offset - 1;
        } else {
            offset = 0;
        }

        s32v2 begin = textEditor->pos;
        s32v2 end = { begin.x, begin.y + font->lineHeight };

        if ( textEditor->buffer.count != 0 ) {
            Code_Point_Data* cursorCodePointData = Font_Get_Code_Point(font, textEditor->buffer.E[offset]);

            begin.x += Renderer_Get_Last_Char_Pos(font, Str_Begin(textEditor->buffer, textEditor->offset)) + cursorCodePointData->offset.x;
            begin.y += cursorCodePointData->offset.y;

            s32v2 end = { begin.x, begin.y + cursorCodePointData->dims.height };

            Renderer_Draw_Line(image, begin, end, BLACK, 0, relToCamera);
        }
    }

    /* Process keyboard inputs */ {
        textEditor->keyCoolDown -= gProgramState.time.delta;

        Input_Button keyState;
        Input_Keyboard_ID newKeyPress = Input_Keyboard_Get_Press(&keyState);
        if ( newKeyPress == 0 ) { return; }

        if ( !Input_Button_Is_Press(keyState) ) { return; }

#if 0
        if ( textEditor->keyCoolDown > 0.0f ) { return; }

        if ( Input_Button_Is_Press(keyState) ) {
            textEditor->keyCoolDown = 0.5f;
        } else {
            textEditor->keyCoolDon = 0.05f;
        }
#endif

        switch ( (u32)newKeyPress ) {
            case II_Kbd_Enter: {
                textEditor->using = false;
            } break;

            case II_Kbd_BackSpace: {
                if ( textEditor->offset != 0 ) {
                    if ( textEditor->offset <= ( textEditor->buffer.count - 1 ) ) {
                        Mem_Shift_Left_Array(textEditor->buffer.E, textEditor->offset, textEditor->buffer.count-1, 1);
                    }

                    --textEditor->offset;
                    --textEditor->buffer.count;
                    textEditor->buffer.E[textEditor->buffer.count] = '\0';
                }
            } break;

            case II_Kbd_Left: {
                if ( textEditor->offset != 0 ) {
                    --textEditor->offset;
                }
            } break;

            case II_Kbd_Right: {
                textEditor->offset = CapTop(textEditor->buffer.count, textEditor->offset + 1);
            } break;

            default: {
                char newChar = OS_Keyboard_Key_To_Letter(newKeyPress);

                if ( newChar == 0 || !Font_Code_Point_Exists(font, newChar) ) { return; }

                if ( textEditor->offset != textEditor->buffer.count ) {
                    Mem_Shift_Right_Array(textEditor->buffer.E, textEditor->offset, textEditor->buffer.count-1, 1);
                }

                textEditor->buffer.E[textEditor->offset++] = newChar;
                ++textEditor->buffer.count;

                textEditor->buffer.E[textEditor->buffer.count] = '\0';
            } break;
        }
    }
}

Private Global s32v2 prevCursorWS;
Private Global s32v2 prevCursorSS;

Public b8 Has_Clicked(void) {
    b8 hasClicked = ( !gUIState.select.using && Input_Button_Is_Press(gProgramState.input.mouse.button1) );

    if ( hasClicked ) {
        gUIState.clickHasCollided = true;
    }

    return hasClicked;
}

#define SYMBOL_CHILD_HALF_DIMS (s32v2){ 10, 10 }

Public void Render_Child_Reference(Image* image, Symbol* child, s32v2 childPos, Rect symbolRect, Symbol* symbol) {
    u32 segmentSize = 4;

    if ( child == symbol ) {
        s32 distance = 5;
        s32v2 pointsData[] = {
            childPos,
            { pointsData[0].x, symbolRect.rightTop.y + distance },
            { symbolRect.leftBottom.x - distance, pointsData[1].y },
            { pointsData[2].x, symbolRect.leftBottom.y - ( SYMBOL_CHILD_HALF_DIMS.height * 2 ) - distance },
            { symbol->edit.pos.x, pointsData[3].y },
            { pointsData[4].x, pointsData[4].y + distance },
        };

        Slice_s32v2 points = Slice_From_Raw_Array(s32v2, pointsData);
        Renderer_Draw_Points_As_Lines(image, points, BLACK, segmentSize, true);
    } else  {
        if ( !Is_Symbol_Data_Ptr(child) ) {
            Renderer_Draw_Line(image, childPos, child->edit.pos, BLACK, segmentSize, true);
        }
    }
}

// Select.
#define Check_Optional(a, b, opt) ( ( (a) == opt ) || ( (b) == opt ) || ( (a) == (b) ) )
#define Check_UI_Object_ID(a, b) ( Check_Optional((a).parent, (b).parent, 0) && Check_Optional((a).item, (b).item, 0) )

Public void Select_Clear(void) {
    gUIState.objects.count = 0;
}

Public UI_Object* Select_Add(UI_ID ID) {
    DArray_Push_C(&gPermArena, &gUIState.objects, (UI_Object){ .ID = ID });
    return &gUIState.objects.E[gUIState.objects.count-1];
}

Public UI_Object* Select_Single(UI_ID ID) {
    Select_Clear();
    return Select_Add(ID);
}

#define DONT_CARE 0

#define UI_ID_Equal(a, b) ( Check_Optional(object->ID.parent, ID.parent, DONT_CARE) && \
                            Check_Optional(object->ID.item, ID.item, DONT_CARE) && \
                            Check_Optional(object->ID.state, ID.state, DONT_CARE) )

Public UI_Object* Select_Get(UI_ID ID) {
    foreach (UI_Object, object, gUIState.objects) {
        if ( UI_ID_Equal(object->ID, ID) ) {
            return object;
        }
    }

    return 0;
}

Public b8 Select_Exists(UI_ID ID) {
    return (b8)( Select_Get(ID) != 0 );
}

Public void Select_Delete(UI_ID ID) {
    Swap(Select_Get(ID), &gUIState.objects.E[gUIState.objects.count-1], UI_Object);

    --gUIState.objects.count;
}

#define Select_Data_ID(type, ID) ( (type*)Select_Data_(sizeof(type), ID) )
#define Select_Data(type, ...) ( (type*)Select_Data_(sizeof(type), (UI_ID){ __VA_ARGS__ }) )
Public void* Select_Data_(u32 size, UI_ID ID) {
    UI_Object* object = Select_Get(ID);
    Assert( size <= Array_Count(object->data) );
    return object->data;
}

typedef enum {
    MULTI_SELECTOR = Bit(1),
    MULTI = Bit(2),
    HELD  = Bit(3),
    PARENT_MULTI = Bit(4),
    TOGGLE = Bit(5),
} Select_Flags;

#define Select_Update(flags, rect, cursor, ...) Select_Update_ID(flags, rect, cursor, (UI_ID){ __VA_ARGS__ })
Public UI_State Select_Update_ID(Select_Flags flags, Rect rect, s32v2 cursor, UI_ID ID) {
    UI_State state = UI_STATE_NOT_SELECTED;

    if ( Select_Exists(ID) ) {
        state = Select_Get(ID)->ID.state;
    }

    switch ( state ) {
        case UI_STATE_NOT_SELECTED: {
            if ( gUIState.select.using ) {
                if ( Flags_Has_All(flags, MULTI_SELECTOR) && Rect_Is_Rect_Colliding(gUIState.select.rect, rect) ) {
                    ID.state = UI_STATE_ABOUT_TO;
                    (void)Select_Add(ID);
                }
            } else {
                if ( Rect_Is_Point_Inside(rect, cursor) ) {
                    ID.state = UI_STATE_ABOUT_TO;
                    (void)Select_Add(ID);
                }
            }
        } break;

        case UI_STATE_ABOUT_TO: {
            if ( gUIState.select.using ) {
                if ( Flags_Has_All(flags, MULTI_SELECTOR) && !Rect_Is_Rect_Colliding(gUIState.select.rect, rect) ) {
                    Select_Delete(ID);
                }
            } else {
                if ( Rect_Is_Point_Inside(rect, cursor) ) {
                    if ( Has_Clicked() ) {
                        if ( Flags_Has_All(flags, PARENT_MULTI) || Flags_Has_All(flags, MULTI) || Program_Input_Keyboard_Is_Held(II_Kbd_Ctrl_L) ) {
                            if ( Flags_Has_All(flags, PARENT_MULTI) ) {
                                foreach (UI_Object, iter, gUIState.objects) {
                                    if ( iter->ID.parent != ID.parent ) {
                                        Select_Delete(iter->ID);
                                    }
                                }
                            }

                            if ( Program_Input_Keyboard_Is_Held(II_Kbd_Ctrl_L) ) {
                                Select_Get(ID)->held = true;
                            }

                            Select_Get(ID)->ID.state = UI_STATE_SELECTED_INIT;
                        } else {
                            foreach (UI_Object, iter, gUIState.objects) {
                                if ( iter->ID.state == UI_STATE_SELECTED ) {
                                    Select_Delete(iter->ID);
                                }
                            }

                            ID.state = UI_STATE_SELECTED_INIT;
                            (void)Select_Add(ID);
                        }
                    }
                } else {
                    Select_Delete(ID);
                }
            }
        } break;

        case UI_STATE_SELECTED_INIT: {
            Select_Get(ID)->ID.state = UI_STATE_SELECTED;
            if ( Rect_Is_Point_Inside(rect, cursor) ) {
                Has_Clicked();
            }
        } break;

        case UI_STATE_SELECTED: {
            if ( Flags_Has_All(flags, HELD) && !Select_Get(ID)->held && !gProgramState.input.mouse.button1.isHeld ) {
                Select_Delete(ID);
            }

            if ( Rect_Is_Point_Inside(rect, cursor) ) {
                // Flags_Has_All(flags, TOGGLE)
                if ( Has_Clicked() && Program_Input_Keyboard_Is_Held(II_Kbd_Ctrl_L) ) {
                    Select_Delete(ID);
                }
            }
        } break;

        NO_DEFAULT
    }

    return ( Select_Exists(ID) ) ? Select_Get(ID)->ID.state : UI_STATE_NOT_SELECTED;
}

Public b8 UI_Button(Rect rect, s32v2 cursor, UI_ID ID) {
    return ( Select_Update_ID(0, rect, cursor, ID) == UI_STATE_SELECTED );
}

// Copy
#if 0
Public Copy_Data* Copy_Get(Array_Copy_Data copies, void* oldID) {
    foreach (Copy_Data, copyData, copies) {
        if ( copyData->old == oldID ) {
            return copyData;
        }
    }

    return 0;
}

Public b8 Is_Already_Copied(Array_Copy_Data copies, void* oldID) {
    return (b8)( Copy_Get(copies, oldID)->new != 0 );
}

Public void* UI_Copy(Array_Copy_Data* copies, UI_Object_Tag tag, void* oldID) {
    if ( Is_Already_Copied(*copies, oldID) ) {
        return Copy_Get(*copies, oldID)->new;
    }

    s32v2 copyOffset = (s32v2){ 10, 10 };

    Copy_Data* copyData = Copy_Get(*copies, oldID);

    if ( tag == UI_OBJECT_TAG_SYMBOL ) {
        Symbol* symbol = (Symbol*)oldID;
        Symbol* symbolCopy = Symbol_New(s32v2_Add(symbol->edit.pos, copyOffset));

        copyData->new = symbolCopy;
        (void)Select_Add(UI_OBJECT_TAG_SYMBOL, symbolCopy, UI_STATE_SELECTED);

        foreach (Symbol*, childRef, symbol->childs) {
            void* child = Is_Symbol_Data_Ptr(*childRef) ? (void*)Symbol_Data_Ptr_Get(*childRef) : (void*)*childRef;

            DArray_Push(&gPermArena, &symbolCopy->childs);
            Symbol** childCopyRef = &symbolCopy->childs.E[symbolCopy->childs.count-1];

            Copy_Data* childCopyData = Copy_Get(*copies, child);
            if ( child && childCopyData ) {
                if ( !Is_Symbol_Data_Ptr(*childRef) ) {
                    *childCopyRef = (Symbol*)UI_Copy(copies, UI_OBJECT_TAG_SYMBOL, child);
                } else {
                    *childCopyRef = Symbol_Data_Ptr((Symbol_Data*)UI_Copy(copies, UI_OBJECT_TAG_SYMBOL_DATA, (Symbol_Data*)child));
                }
            } else {
                *childCopyRef = child;
            }
        }
    } else if ( tag == UI_OBJECT_TAG_SYMBOL_DATA ) {
        Symbol_Data* symbolData = (Symbol_Data*)oldID;

        Symbol_Data* symbolDataCopy = Symbol_Data_Create(s32v2_Add(symbolData->edit.pos, copyOffset),
                                                         symbolData->tag, symbolData->data);
        copyData->new = symbolDataCopy;
        (void)Select_Add(UI_OBJECT_TAG_SYMBOL_DATA, symbolDataCopy, UI_STATE_SELECTED);
    } NO_ELSE

    return copyData->new;
}
#endif
//

Public b8 UI_Button_Update(Rect rect, s32v2 cursor) {
    if ( Rect_Is_Point_Inside(rect, cursor) && Has_Clicked() ) {
        gUIState.pulsing = &gProgramState.input.mouse.button1;
        return true;
    }

    return false;
}

Public b8 Parent_Is_Symbol(u64 parent) {
    return (b8)( (byte*)gSymbolState.symbolPool.E <= (byte*)parent &&
                 (byte*)parent <= (byte*)&gSymbolState.symbolPool.E[gSymbolState.symbolPool.count] );
}

Public void Render_Symbols(Image* image) {
    gUIState.clickHasCollided = false;
    b8 relToCamera = true;

    // NOTE(JENH): WS stands for "World space" and SS stands for "Screen space".
    s32v2 cursorWS = Get_Cursor_World_Space(gProgramState.window);
    s32v2 cursorSS = Window_Manager_Get_Cursor_Rel_To_Window(gProgramState.window);

    Array_Symbol symbols = gSymbolState.symbolPool;
    foreach (Symbol, symbol, symbols) {
        if ( symbol->edit.deleted ) { continue; }

        s32v2 symbolHalfDims = { 5, 5 };
        Rect symbolRect = Rect_Create_CH(symbol->edit.pos, symbolHalfDims);
        f32v4 symbolColor = Color_Grey_Scale(0.5f);
        f32v4 outlineColor = BLACK;

        if ( symbol->childs.count > 0 ) {
            symbolRect = Rect_Create_CH(symbol->edit.pos,
                                        (s32v2){ ( SYMBOL_CHILD_HALF_DIMS.width * symbol->childs.count ),
                                        symbolHalfDims.height });
        }

        UI_ID symbolID = { UI_OBJECT_TAG_SYMBOL, (u64)symbol };
        switch ( Select_Update_ID(MULTI_SELECTOR, symbolRect, cursorWS, symbolID) ) {
            case UI_STATE_NOT_SELECTED: {} break;

            case UI_STATE_ABOUT_TO: {
                symbolColor = YELLOW;

                UI_ID ID = (UI_ID){ .parent = UI_OBJECT_TAG_CHILD_SYMBOL, .state = UI_STATE_SELECTED };
                if ( Select_Exists(ID) ) {
                    UI_Child* data = Select_Data_ID(UI_Child, ID);
                    *data->childRef = symbol;
                    data->refID = symbolID;
                }
            } break;

            case UI_STATE_SELECTED_INIT: {} break;

            case UI_STATE_SELECTED: {
                symbolColor = GREEN;

                if ( !gUIState.textEditor.using && Program_Input_Keyboard_Is_Press(II_Kbd_X) ) {
                    symbol->edit.deleted = true;
                }

                if ( gProgramState.input.mouse.button1.isHeld ) { // Move
                    s32v2_AddEq(&symbol->edit.pos, s32v2_Sub(cursorWS, prevCursorWS));
                }

                if ( !gUIState.textEditor.using && Program_Input_Keyboard_Is_Press(II_Kbd_Up) ) {
                    DArray_Push(&gSymbolState.symPtrArena, &symbol->childs);
                }

                if ( !gUIState.textEditor.using && Program_Input_Keyboard_Is_Press(II_Kbd_Down) ) {
                    if ( symbol->childs.count != 0 ) {
                        --symbol->childs.count;
                    }
                }
            } break;

            NO_DEFAULT
        }

        Renderer_Draw_Rect_With_Outline(image, symbolRect, symbolColor, outlineColor, 0, relToCamera);

        s32v2 childPos = s32v2_Add(symbolRect.leftBottom, (s32v2){ SYMBOL_CHILD_HALF_DIMS.width, -SYMBOL_CHILD_HALF_DIMS.height });
        foreach (Symbol*, childRef, symbol->childs) {
            Symbol* child = *childRef;

            if ( child ) { // Update deleted references.
                if ( !Is_Symbol_Data_Ptr(child) ) {
                    if ( &child->edit.deleted ) {
                        *childRef = 0;
                    }
                }
            }

            Rect childRect = Rect_Create_CH(childPos, SYMBOL_CHILD_HALF_DIMS);
            f32v4 childColor = Color_Grey_Scale(0.5f);

            UI_ID childID = { UI_OBJECT_TAG_CHILD_SYMBOL, (u64)childRef };
            switch ( Select_Update_ID(HELD, childRect, cursorWS, childID) ) {
                case UI_STATE_NOT_SELECTED: {
                    if ( child ) {
                        if ( !Is_Symbol_Data_Ptr(child) ) {
                            Render_Child_Reference(image, child, childPos, symbolRect, symbol);
                        } else {
                            childColor = Color_Grey_Scale(0.8f);
                        }
                    }
                } break;

                case UI_STATE_ABOUT_TO: {
                    childColor = YELLOW;

                    if ( child && !Is_Symbol_Data_Ptr(child) ) {
                        Render_Child_Reference(image, child, childPos, symbolRect, symbol);
                    }

                    UI_ID ID = (UI_ID){ .parent = UI_OBJECT_TAG_CHILD_SYMBOL, .state = UI_STATE_SELECTED };
                    if ( Select_Exists(ID) ) {
                        UI_Child* data = Select_Data_ID(UI_Child, ID);
                        *data->childRef = symbol;
                        data->refID = childID;
                    }
                } break;

                case UI_STATE_SELECTED_INIT: {
                    *Select_Data_ID(UI_Child, childID) = (UI_Child){ symbolRect, symbol, childRef, childPos };
                } break;

                case UI_STATE_SELECTED: {
                    childColor = GREEN;

                    if ( gProgramState.input.mouse.button1.isHeld ) {
                        if ( child ) {
                            UI_Child* data = Select_Data_ID(UI_Child, childID);
                            if ( Select_Exists(data->refID) ) {
                                Render_Child_Reference(image, child, data->childPos, data->symbolRect, data->symbol);
                            } else {
                                *childRef = 0;
                            }
                        } else {
                            Renderer_Draw_Line(image, childPos, cursorWS, BLACK, 4, relToCamera);
                        }
                    } else {
                    }
                } break;

                NO_DEFAULT
            }

            Renderer_Draw_Rect_With_Outline(image, childRect, childColor, outlineColor, 0, relToCamera);

            childPos.x += SYMBOL_CHILD_HALF_DIMS.width * 2;
        }
    }

    UI_ID tempID = (UI_ID){ .parent = UI_OBJECT_TAG_CHILD_SYMBOL, .state = UI_STATE_SELECTED };
    if ( Select_Exists(tempID) ) {
        if ( Program_Input_Keyboard_Is_Press(II_Kbd_A) ) {
            u32 count = 0;
            Symbol** childBuffer[KiB(4)];

            Symbol* symbol = 0;

            foreach (UI_Object, iter, gUIState.objects) {
                UI_Child* iterData = Select_Data_ID(UI_Child, iter->ID);

                if ( iter->ID.parent == UI_OBJECT_TAG_CHILD_SYMBOL && iter->ID.state == UI_STATE_SELECTED ) {
                    if ( symbol ) {
                        Assert( symbol == iterData->symbol );
                    } else {
                        symbol = iterData->symbol;
                    }

                    childBuffer[count++] = iterData->childRef;
                }
            }

            void* rawData = Arena_Alloc_Mem(&gSymbolState.dataArena, count);

            for (u32 i = 0; i < count; ++i) {
                *childBuffer[i] = Symbol_Data_Ptr((byte*)rawData + i);
            }
        }
    }

    if ( gDataMenu.isActive ) {
        (void)cursorSS;

        s32 topSize = 20;

        Rect titleRect = {
            .leftBottom = { gDataMenu.rect.leftBottom.x, gDataMenu.rect.rightTop.y - topSize },
            .rightTop   =  gDataMenu.rect.rightTop,
        };

        f32v4 backgroundColor = gDataMenu.backgroundColor;

        switch ( Select_Update(HELD, titleRect, cursorSS, UI_OBJECT_TAG_WINDOW, (u64)&gDataMenu) ) {
            case UI_STATE_NOT_SELECTED: {
            } break;

            case UI_STATE_ABOUT_TO: { backgroundColor = YELLOW; } break;

            case UI_STATE_SELECTED_INIT: { } break;
            case UI_STATE_SELECTED: {
                backgroundColor = GREEN;

                if ( gProgramState.input.mouse.button1.isHeld ) { // Move
                    gDataMenu.rect = Rect_Offset(gDataMenu.rect, s32v2_Sub(cursorSS, prevCursorSS));
                }
            } break;

            NO_DEFAULT
        }

        Renderer_Draw_Rect_With_Outline(image, titleRect, backgroundColor, gDataMenu.outlineColor, 0, false);

        Rect clientRect = {
            .leftBottom = gDataMenu.rect.leftBottom,
            .rightTop   = { gDataMenu.rect.rightTop.x, gDataMenu.rect.rightTop.y - topSize },
        };

        Renderer_Draw_Rect_With_Outline(image, clientRect, gDataMenu.backgroundColor, gDataMenu.outlineColor, 0, false);

        Rect clipedClientRect = Rect_Clip(clientRect, Rect_From_Image(image));

        Image clientImage = {
            .dims = s32v2_To_u32v2(Rect_Get_Dims(clipedClientRect)),
            .hasTransparency = false,
            .channelCount = 4,
            .channelSizes = { 8, 8, 8, 8 },
            .data = Image_Data_At(image, s32v2_To_u32v2(clipedClientRect.leftBottom)),
            .stride = image->stride,
        };

        s32v2 bytesDisplayDims = { 10, 5 };
        s32 padding = 2;

        f32 fontScale = 0.5f;

        byte* bytesScan = gSymbolState.dataArena.base;
        ++(*bytesScan);
        s32v2 byteRectDims = { 15, 15 };
        s32v2 pos;

        pos.y = clientRect.rightTop.y - byteRectDims.height - padding;
        for (;( pos.y - byteRectDims.height) > clientRect.leftBottom.y; pos.y -= byteRectDims.width) {
            pos.x = clientRect.leftBottom.x + padding;

            Local_Str(address, KiB(4));
            Str_From_Fmt(&address, "0x%p", bytesScan);

            (void)Rederer_Draw_Text(image, Asset_Get(Font, S("font.fnt")), address, pos, fontScale, BLACK, false);

            pos.x += byteRectDims.width * 9;
            for (; ( pos.x + byteRectDims.width) < clientRect.rightTop.x; pos.x += byteRectDims.width) {
                Rect byteRect = Rect_Create_LD(pos, byteRectDims);
                f32v4 colorRect = gDataMenu.backgroundColor;

                switch ( Select_Update(0, byteRect, cursorSS, (u64)(void*)&gSymbolState.dataArena, (u64)bytesScan) ) {
                    case UI_STATE_NOT_SELECTED: {
                    } break;

                    case UI_STATE_ABOUT_TO: { colorRect = YELLOW; } break;

                    case UI_STATE_SELECTED_INIT: { } break;
                    case UI_STATE_SELECTED: { colorRect = GREEN; } break;

                    NO_DEFAULT
                }

                foreach (UI_Object, iter, gUIState.objects) {
                    if ( iter->ID.parent == UI_OBJECT_TAG_CHILD_SYMBOL && iter->ID.state == UI_STATE_ABOUT_TO ) {
                        Symbol** childRef = (Symbol**)iter->ID.item;
                        if ( Is_Symbol_Data_Ptr(*childRef) &&
                             Symbol_Data_Ptr_Get(*childRef) == bytesScan ) {
                            colorRect = YELLOW;
                        }
                    }
                }

                Renderer_Draw_Rect(image, byteRect, colorRect, false);

                Local_Str(text, KiB(4));
                Str_From_Fmt(&text, "%02hhx", *bytesScan++);

                (void)Rederer_Draw_Text(image, Asset_Get(Font, S("font.fnt")), text, pos, fontScale, BLACK, false);
            }
        }

        /* handle edges. */ {
            s32 edgeWidth = 4;

            s32v2 corner1 = gDataMenu.rect.leftBottom;
            s32v2 corner2 = gDataMenu.rect.rightTop;

            s32v2 edgePoints[] = {
                corner1,
                { corner1.x, corner2.y },
                corner2,
                { corner2.x, corner1.y },
            };

            for (u32 i = 0; i < Array_Count(edgePoints); ++i) {
                s32v2 *point1 = &edgePoints[i];
                s32v2 *point2 = ( i == ( Array_Count(edgePoints) - 1 ) ) ? &edgePoints[0] : &edgePoints[i+1];

                s32v2 tan  = s32v2_Normalize(s32v2_Sub(*point2, *point1));
                s32v2 norm = s32v2_Normalize((s32v2)CW_Perp(tan));

                s32v2 rectCroner1 = s32v2_Sub(*point1, s32v2_Mul(norm, edgeWidth));
                rectCroner1 = s32v2_Sub(rectCroner1, s32v2_Mul(tan, edgeWidth));
                s32v2 rectCroner2 = s32v2_Add(*point2, s32v2_Mul(norm, edgeWidth));
                rectCroner2 = s32v2_Add(rectCroner2, s32v2_Mul(tan, edgeWidth));

                Rect edgeRect = Rect_Create_PP(rectCroner1, rectCroner2);
                f32v4 color = (f32v4){ 1.0f, 0.0f, 0.0f, 0.4f };

                switch ( Select_Update(MULTI|HELD|PARENT_MULTI, edgeRect, cursorSS, (u64)&gDataMenu.rect, (u64)( (byte*)&gDataMenu.rect + i )) ) {
                    case UI_STATE_NOT_SELECTED: {
                    } break;

                    case UI_STATE_ABOUT_TO: { color = (f32v4){ 1.0f, 1.0f, 0.0f, 0.4f }; } break;

                    case UI_STATE_SELECTED_INIT: { } break;
                    case UI_STATE_SELECTED: {
                        color = (f32v4){ 0.0f, 1.0f, 0.0f, 0.4f };

                        if ( gProgramState.input.mouse.button1.isHeld ) { // Move
                            s32 displacement = (s32)f32v2_Dot(s32v2_To_f32v2(norm), s32v2_To_f32v2(s32v2_Sub(cursorSS, prevCursorSS)));
                            s32v2_AddEq(point1, s32v2_Mul(norm, displacement));
                            s32v2_AddEq(point2, s32v2_Mul(norm, displacement));
                        }
                    } break;

                    NO_DEFAULT
                }

                Renderer_Draw_Rect_With_Outline(image, edgeRect, color, RED, 3, false);
            }

            gDataMenu.rect = Rect_Create_PP(edgePoints[0], edgePoints[2]);
        }
    }

    if ( !gUIState.textEditor.using && Program_Input_Keyboard_Is_Press(II_Kbd_D) ) {
        if ( Toggle(&gDataMenu.isActive) ) {
            gUIState.tempDataPos = cursorWS;
        } else {
            gUIState.tempDataPos = (s32v2){ 0 };
        }
    }

    if ( !gUIState.textEditor.using && Program_Input_Keyboard_Is_Press(II_Kbd_C) ) { // Copy
        Array_UI_Object objects;
        Array_Alloc_And_Copy(&objects, gUIState.objects, &gTempArena);

        Select_Clear();

        Array_Copy_Data copies;
        Array_Create(copies, objects.count, &gTempArena);

#if 0
        foreach (UI_Object, object, objects) {
            Array_Push_C(copies, (Copy_Data){ .old = object->ID, .new = 0 });
        }

        foreach (UI_Object, object, objects) {
            UI_Copy(&copies, object->tag, object->ID);
        }
#endif
    }

    if ( gUIState.textEditor.using ) {
        Text_Editor_Update(&gUIState.textEditor, image, relToCamera);
    } else {
        gUIState.textEditor.ID = 0;
    }

    if ( gUIState.select.using ) {
        if ( gProgramState.input.mouse.button1.isHeld ) {
            gUIState.select.rect = Rect_Create_PP(gUIState.select.startingPos, cursorWS);

            Renderer_Draw_Rect_With_Outline(image, gUIState.select.rect, (f32v4){ 0.0f, 0.0f, 1.0f, 0.2f }, BLUE, 5, true);

            if ( !gUIState.select.pulsing->hasBeenPress ) {
                foreach (UI_Object, object, gUIState.objects) {
                    if ( object->ID.state == UI_STATE_ABOUT_TO ) {
                        object->ID.state = UI_STATE_SELECTED;
                    }
                }
            }
        } else {
            gUIState.select.using = false;
        }
    }

    if ( !gUIState.clickHasCollided && Input_Button_Is_Press(gProgramState.input.mouse.button1) ) {
        gUIState.select.using = true;
        gUIState.select.pulsing = &gProgramState.input.mouse.button1;
        gUIState.select.startingPos = cursorWS;

        if ( !Program_Input_Keyboard_Is_Held(II_Kbd_Ctrl_L) ) {
            Select_Clear();
        }
    }

    if ( !gUIState.textEditor.using && Program_Input_Keyboard_Is_Press(II_Kbd_N) ) {
        Symbol* newSymbol = Symbol_New(cursorWS);
        Select_Single((UI_ID){ UI_OBJECT_TAG_SYMBOL, (u64)newSymbol, UI_STATE_SELECTED });
    }

    prevCursorSS = cursorSS;
    prevCursorWS = cursorWS;
}

Public void Render(Image* image) {
    f32 speed = 100.0f;

#if 0
    if ( Program_Input_Keyboard_Is_Held(II_Kbd_W) ) {
        gProgramState.camera.targetPos.y -= gProgramState.time.delta * speed;
    } if ( Program_Input_Keyboard_Is_Held(II_Kbd_S) ) {
        gProgramState.camera.targetPos.y += gProgramState.time.delta * speed;
    } if ( Program_Input_Keyboard_Is_Held(II_Kbd_D) ) {
        gProgramState.camera.targetPos.x -= gProgramState.time.delta * speed;
    } if ( Program_Input_Keyboard_Is_Held(II_Kbd_A) ) {
        gProgramState.camera.targetPos.x += gProgramState.time.delta * speed;
    }
#endif

    if ( !gUIState.textEditor.using ) {
        if ( Program_Input_Keyboard_Is_Press(II_Kbd_Esc) ) {
            Program_Close();
        }

        if ( Program_Input_Keyboard_Is_Press(II_Kbd_F) ) {
            gProgramState.isFullScreen = !gProgramState.isFullScreen;

            if ( gProgramState.isFullScreen ) {
                Window_FullScreen_Size(gProgramState.window);
            } else {
                Window_Normal_Size(gProgramState.window);
            }
        }

        if ( gProgramState.input.mouse.button3.isHeld ) {
            s32v2 offset = (s32v2){ (s32)gProgramState.input.mouse.move.x, (s32)gProgramState.input.mouse.move.y };
            f32v2_AddEq(&gProgramState.camera.targetPos, (f32v2){ (f32)offset.x, (f32)offset.y });
        }
    }

    Renderer_Clear(image, WHITE);

    Render_Symbols(image);

#if 0
    Renderer_Draw_Image(image, Rect_Create_LD((s32v2){ 0, 0 }, (s32v2){ 256, 256 }), &sample,
                        Rect_Create_LD((s32v2){ 0, 0 }, (s32v2){ 256, 256 }), RED);
#endif

#if 0
    Mesh* mesh = Asset_Get(Mesh, S("teapot.obj"));
    f32m4x4 matrix = f32m4x4_IDENTITY;
    Renderer_Draw_Mesh(image, mesh, matrix, BLACK);
#endif

    /* Draw cursor. */ {
        s32 cursorHalfDims = 2;
        s32v2 cursor = Window_Manager_Get_Cursor_Rel_To_Window(gProgramState.window);
        Renderer_Draw_Rect(image, Rect_Create_CH(cursor, (s32v2){ cursorHalfDims, cursorHalfDims }), RED, false);
    }

    /* Update cursor position. */ {
        gProgramState.camera.pos = f32v2_Lerp(gProgramState.time.delta * 0.95f, gProgramState.camera.pos, gProgramState.camera.targetPos);

        f32 epsilon = 0.01f;
        if ( f32v2_Length(f32v2_Sub(gProgramState.camera.pos, gProgramState.camera.targetPos)) <= epsilon) {
            gProgramState.camera.pos = gProgramState.camera.targetPos;
        }
    }
}

Public void Copy_Diff_Bytes(void* dst, void* src, u32 size) {
    byte* dstScan = dst;
    byte* srcScan = src;

    forCount (i, size) {
        if ( *dstScan != *srcScan ) {
            *dstScan = *srcScan;
        }

        ++dstScan;
        ++srcScan;
    }
}

Fn_Sig_Window_Event_CallBack(Process_Window_Event) {
    Event_Result result = EVENT_RESULT_OK;

    switch ( event.tag ) {
        case EVENT_TAG_CLOSE: {
            Program_Close();
        } break;

        case EVENT_TAG_RESIZE: {
            Window_Update_Dims(window, event.newDims);
            Window_Update(window);
        } break;

        case EVENT_TAG_ACTIVE: {
            b8 isBeingActivated = (b32)event.isActive;
            (void)isBeingActivated;
        } break;

        case EVENT_TAG_REDRAW: {
            // TODO(JENH): Find a way to get rid of this in the platform independent code,
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(window->platform.handle, &paint);
            //

            Window_Update_Dims(window, OS_Window_Get_Dims(window));

            Render(&window->bitmap);
            Window_Update(window);

            // TODO(JENH): Find a way to get rid of this in the platform independent code,
            EndPaint(window->platform.handle, &paint);
        } break;

        case EVENT_TAG_INPUT: {
#if 0
            switch ( event.input.device ) {
                case INPUT_DEVICE_TAG_MOUSE: {
                    Mem_Copy_Type(gProgramState.input.mouse.buttons, event.input.mouse.buttons);

                    u16v2_AddEq(&gProgramState.input.mouse.wheel, event.input.mouse.wheel);
                    u16v2_AddEq(&gProgramState.input.mouse.move, event.input.mouse.move);
                } break;

                case INPUT_DEVICE_TAG_KEYBOARD: {
                    Program_Input_Keyboard_Add_Press(event.input.kbdKeyPress);

                    if ( Program_Input_Keyboard_Is_Press(II_Kbd_Esc) ) {
                        Program_Close();
                        break;
                    }
                } break;

                case INPUT_DEVICE_TAG_XBOX_360: {
                    gProgramState.input.xbox360 = event.input.xbox360;
                } break;

                NO_DEFAULT
            }
#endif
        } break;

        default: {
            result = EVENT_RESULT_NOT_HANDLED;
        } break;
    }

    return result;
}

Public void Symbol_Init(void) {
    Arena_Create_From_Arena(&gSymbolState.dataArena, &dyMem.arena, MiB(32));
    Arena_Create_From_Arena(&gSymbolState.symPtrArena, &dyMem.arena, MiB(1));
    Array_Create(gSymbolState.symbolPool, KiB(256), &gPermArena);
    Array_Create(gSymbolState.symbolDataPool, KiB(32), &gPermArena);

    gSymbolState.root = Symbol_New(Rect_Get_Center(Rect_From_Image(&gProgramState.window->bitmap)));
    Select_Single((UI_ID){ UI_OBJECT_TAG_SYMBOL, (u64)gSymbolState.root, UI_STATE_SELECTED });

    Symbol_Data_Init();
}

Public void UI_Init(void) {
    gUIState.textEditor.buffer = Str_Alloc(&gPermArena, KiB(16));
    DArray_Create(gUIState.objects, &gPermArena);

    s32v2 halfDims = (s32v2){ (s32)( (f32)gProgramState.window->bitmap.dims.width  * 0.15f ),
                              (s32)( (f32)gProgramState.window->bitmap.dims.height * 0.15f ) };
    gDataMenu.rect = Rect_Create_CH(Rect_Get_Center(Rect_From_Image(&gProgramState.window->bitmap)), halfDims);
    gDataMenu.backgroundColor = Color_Grey_Scale(0.5f);
}

void Entry(void) {
    Mem_Init();

    Arena_Create_From_Arena(&gPermArena, &dyMem.arena, MiB(64));
    Arena_Create_From_Arena(&gTempArena, &dyMem.arena, MiB(64));
    //Arena_Create_From_Arena(&gGeometryBuilderArena, &dyMem.arena, MiB(1));

    gUIState.textEditor.buffer.E = Arena_Alloc_Array(&gPermArena, char, KiB(16));

    Window_Manager_Init(&gPermArena);
    Array_Create(gAssets, KiB(256), &gPermArena);

    Program_Init();
    gProgramState.thingPos   = (f32v2){ 400.0f, 300.0f };
    gProgramState.camera.pos = (f32v2){ 0.0f, 0.0f };
    gProgramState.window = Window_Open(&gPermArena, S("Window Test"), (u32v2){ 960, 540 }, Process_Window_Event);

    UI_Init();
    Symbol_Init();
    Image_Create_Sample();

    while ( gProgramState.isRunning ) {
        Program_Time_Update();
        Window_Process_Pool_Events(gProgramState.window);

        Render(&gProgramState.window->bitmap);

        Window_Update(gProgramState.window);
        Arena_Clear(&gTempArena);

        Program_Input_Update();
    }

    Window_Close(gProgramState.window);

    return;
}
