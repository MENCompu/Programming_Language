Public void Renderer_Clear(Image* dst, f32v4 color) {
    u32 packedColor = Color_Pack(color);

    u32 size = dst->dims.width * dst->dims.height * Image_Get_Byte_Per_Pixel(dst) / sizeof(u32);

    u32* scan = (u32*)dst->data;
    for (u32 i = 0; i < size; ++i) {
        *scan++ = packedColor;
    }
}

Public void Renderer_Draw_Rect(Image* dst, Rect rect, f32v4 color, b8 relToCamera) {
    if ( relToCamera ) {
        rect = Rect_Offset(rect, f32v2_To_s32v2(gProgramState.camera.pos));
    }

    Rect cliped = Rect_Clip(Rect_From_Image(dst), rect);

    u32* dstScan = (u32*)Image_Data_At(dst, s32v2_To_u32v2(cliped.leftBottom));

    for (s32 y = cliped.leftBottom.y; y <= cliped.rightTop.y; ++y) {
        byte* row = (byte*)dstScan;

        for (s32 x = cliped.leftBottom.x; x < cliped.rightTop.x; ++x) {
            Color_Alpha_Blend(dstScan++, color);
        }

        dstScan = (u32*)( row + dst->stride );
    }
}

Public void Renderer_Draw_Triangle(Image* dst, f32v2 a, f32v2 b, f32v2 c, f32v4 color) {
    u32 pakedColor = Color_Pack(color);

    Rect testRect = {
        .leftBottom = { .x = (s32)Min(Min(a.x, b.x), c.x),
                        .y = (s32)Min(Min(a.y, b.y), c.y) },
        .rightTop   = { .x = Ceil_F32_To_S32(Max(Max(a.x, b.x), c.x)),
                        .y = Ceil_F32_To_S32(Max(Max(a.y, b.y), c.y)) },
    };

    Rect clipedRect = Rect_Clip(Rect_From_Image(dst), testRect);

    f32v2 perpAB = CCW_Perp(f32v2_Sub(b, a));
    f32v2 perpBC = CCW_Perp(f32v2_Sub(c, b));
    f32v2 perpCA = CCW_Perp(f32v2_Sub(a, c));

    u32v2 point;
    for (point.y = clipedRect.leftBottom.y; point.y < (u32)clipedRect.rightTop.y; ++point.y) {
        for (point.x = clipedRect.leftBottom.x; point.x < (u32)clipedRect.rightTop.x; ++point.x) {
            if ( f32v2_Dot(perpAB, f32v2_Sub((f32v2){ (f32)point.x, (f32)point.y }, a)) >= 0.0f &&
                 f32v2_Dot(perpBC, f32v2_Sub((f32v2){ (f32)point.x, (f32)point.y }, b)) >= 0.0f &&
                 f32v2_Dot(perpCA, f32v2_Sub((f32v2){ (f32)point.x, (f32)point.y }, c)) >= 0.0f ) {
                *(u32*)Image_Data_At(dst, point) = pakedColor;
            }
        }
    }
}

Public void Renderer_Draw_Mesh(Image* dst, Mesh* mesh, f32m4x4 PVM, f32v4 color) {
    Slice_u32v3 faceIndices = {
        .count = mesh->indices.count / 3,
        .E = (u32v3*)mesh->indices.E,
    };

    foreach (u32v3, indices, faceIndices) {
        f32v4 pointsV4[3] = {
            (f32v4){ mesh->vertices.E[indices->E[0]].x, mesh->vertices.E[indices->E[0]].y, mesh->vertices.E[indices->E[0]].z, 10.0f },
            (f32v4){ mesh->vertices.E[indices->E[1]].x, mesh->vertices.E[indices->E[1]].y, mesh->vertices.E[indices->E[1]].z, 10.0f },
            (f32v4){ mesh->vertices.E[indices->E[2]].x, mesh->vertices.E[indices->E[2]].y, mesh->vertices.E[indices->E[2]].z, 10.0f },
        };

        f32v4 points[3] = {
            f32m4x4_Mul_Vec(PVM, pointsV4[0]),
            f32m4x4_Mul_Vec(PVM, pointsV4[1]),
            f32m4x4_Mul_Vec(PVM, pointsV4[2]),
        };

        if ( !( F32_Abs(points[0].x) <= points[0].w && F32_Abs(points[0].y) <= points[0].w && F32_Abs(points[0].z) <= points[0].w) &&
             !( F32_Abs(points[1].x) <= points[1].w && F32_Abs(points[1].y) <= points[1].w && F32_Abs(points[1].z) <= points[1].w) &&
             !( F32_Abs(points[2].x) <= points[2].w && F32_Abs(points[2].y) <= points[2].w && F32_Abs(points[2].z) <= points[2].w) ) {
            continue;
        }

        points[0] = f32v4_Mul(points[0], 1.0f / points[0].w);
        points[1] = f32v4_Mul(points[1], 1.0f / points[1].w);
        points[2] = f32v4_Mul(points[2], 1.0f / points[2].w);

        f32v2 dstHalfDims = { .width = (f32)dst->dims.width / 2.0f, .height = (f32)dst->dims.height / 2.0f };

        f32v2 screenPoints[3] = {
            f32v2_Add(f32v2_ElemWise_Mul(dstHalfDims, points[0].xy), dstHalfDims),
            f32v2_Add(f32v2_ElemWise_Mul(dstHalfDims, points[1].xy), dstHalfDims),
            f32v2_Add(f32v2_ElemWise_Mul(dstHalfDims, points[2].xy), dstHalfDims),
        };

        Renderer_Draw_Triangle(dst, screenPoints[0], screenPoints[1], screenPoints[2], color);
    }
}

Private f32 Evaluate_Point_In_Line(s32v2 begin, s32v2 end, s32v2 point) {
    f32v2 d = { (f32)( end.x - begin.x ), (f32)( end.y - begin.y ) };
    f32 yIntersect = begin.y - ( ( d.y / d.x ) * begin.x );
    f32 ret = d.y*point.x - d.x*point.y + d.x*yIntersect;
    return ret;
}

typedef struct {
    f32v2 begin;
    f32v2 end;
} Line;
Slice(Line);

Public Line Clip_Line(f32v2 begin, f32v2 end, Rect rect) {
    Line ret = { begin, end };

    f32v2 tempRectLines[] = {
        s32v2_To_f32v2(rect.leftBottom),
        s32v2_To_f32v2((s32v2){ rect.leftBottom.x, rect.rightTop.y - 1}),
        s32v2_To_f32v2(s32v2_Sub(rect.rightTop, (s32v2){ 1, 1 })),
        s32v2_To_f32v2((s32v2){ rect.rightTop.x - 1, rect.leftBottom.y }),
        s32v2_To_f32v2(rect.leftBottom),
    };

    Slice_f32v2 rectLines = Slice_From_Raw_Array(f32v2, tempRectLines);

    f32v2 tBounds = { 0.0f, 1.0f };

    for (u32 i = 1; i < rectLines.count; ++i) {
        Line rectLine = { .begin = rectLines.E[i-1], .end = rectLines.E[i] };

        f32v2 normal = CW_Perp(f32v2_Sub(rectLine.end, rectLine.begin));

        f32 beginSignDistance = f32v2_Dot(f32v2_Sub(begin, rectLine.begin), normal);
        f32 endSignDistance = f32v2_Dot(f32v2_Sub(end, rectLine.begin), normal);

        if ( beginSignDistance < 0.0f && endSignDistance < 0.0f ) {
            return (Line){ { 0 }, { 0 } }; // reject line.
        } else if ( beginSignDistance >= 0.0f && endSignDistance >= 0.0f ) {
            continue; // accept the line for this iteration.
        } else {
            f32 t = f32v2_Dot(f32v2_Sub(rectLine.begin, begin), normal) /
                    f32v2_Dot(f32v2_Sub(end, begin), normal);

            if ( beginSignDistance < 0.0f ) {
                f32v2 temp = f32v2_Lerp(t, begin, end);
                (void)temp;

                if ( t > tBounds.x ) {
                    ret.begin = f32v2_Lerp(t, begin, end);
                    tBounds.x = t;
                }
            } else {
                f32v2 temp = f32v2_Lerp(t, begin, end);
                (void)temp;

                if ( t < tBounds.y ) {
                    ret.end = f32v2_Lerp(t, begin, end);
                    tBounds.y = t;
                }
            }
        }
    }

    return ret;
}

Public void Renderer_Draw_Line(Image* dst, s32v2 begin, s32v2 end, f32v4 color, u32 segmentSize, b8 relToCamera) {
    if ( relToCamera ) {
        s32v2_AddEq(&begin, f32v2_To_s32v2(gProgramState.camera.pos));
        s32v2_AddEq(&end, f32v2_To_s32v2(gProgramState.camera.pos));
    }

    if ( segmentSize == 0 ) {
        segmentSize = 0xffffffff;
    }

    u32 packedColor = Color_Pack(color);

    f32v2 d = { (f32)( end.x - begin.x ), (f32)( end.y - begin.y ) };
    f32 lenght = F32_Sqrt(F32_Sq(d.x)) + F32_Sqrt(F32_Sq(d.y));

    f32v2 direction = f32v2_Mul(d, 1.0f / lenght);

    // Should flip points.
    if ( ( direction.x < 0.0f && direction.y < 0.5f ) ||
         ( direction.y < 0.0f && direction.x < 0.5f ) ) {
        direction = (f32v2){ -direction.x, -direction.y };
        Swap(&begin, &end, s32v2);
    }

    /* clip line in image's rect */ {
        Line line = Clip_Line(s32v2_To_f32v2(begin), s32v2_To_f32v2(end), Rect_From_Image(dst));

        if ( Mem_Equal_Zero_Type(&line) ) { return; } // line is completely out of the image.

        begin = f32v2_To_s32v2(line.begin);
        end = f32v2_To_s32v2(line.end);
    }

    b8 shouldDraw = true;
    u32 segmentCounter = segmentSize;

    if ( F32_Abs(direction.x) >= F32_Abs(direction.y) ) { // Increase x.
        s32 sign = ( direction.y <= 0.0f ) ? -1 : +1;

        for (s32v2 point = begin; point.x <= end.x; ++point.x) {
            f32 v1 = Evaluate_Point_In_Line(begin, end, point);
            f32 v2 = Evaluate_Point_In_Line(begin, end, (s32v2){ .x = point.x, .y = ( point.y + 1*sign ) });

            if ( F32_Abs(v1) > F32_Abs(v2) ) {
                point.y = point.y + 1*sign;
            }

            if ( shouldDraw ) {
                byte* data = Image_Data_At(dst, (u32v2){ (u32)point.x, (u32)point.y });
                *(u32*)data = packedColor;
            }

            if ( --segmentCounter == 0 ) {
                segmentCounter = segmentSize;
                shouldDraw = !shouldDraw;
            }
        }
    } else { // Increase y.
        s32 sign = ( direction.x <= 0.0f ) ? -1 : +1;

        for (s32v2 point = begin; point.y <= end.y; ++point.y) {
            f32 v1 = Evaluate_Point_In_Line(begin, end, point);
            f32 v2 = Evaluate_Point_In_Line(begin, end, (s32v2){ .x = ( point.x + 1*sign ), .y = point.y });

            if ( F32_Abs(v1) > F32_Abs(v2) ) {
                point.x = point.x + 1*sign;
            }

            if ( shouldDraw ) {
                byte* data = Image_Data_At(dst, (u32v2){ (u32)point.x, (u32)point.y });
                *(u32*)data = packedColor;
            }

            if ( --segmentCounter == 0 ) {
                segmentCounter = segmentSize;
                shouldDraw = !shouldDraw;
            }
        }
    }
}

Public void Renderer_Draw_Points_As_Lines(Image* image, Slice_s32v2 points, f32v4 color, u32 segmentSize, b8 retToCamera) {
    for (u32 i = 1; i < points.count; ++i) {
        Renderer_Draw_Line(image, points.E[i-1], points.E[i], BLACK, segmentSize, true);
    }
}

Public void Renderer_Draw_Rect_With_Outline(Image* dst, Rect rect, f32v4 rectColor, f32v4 outLineColor, u32 segmentSize, b8 relToCamera) {
    Renderer_Draw_Rect(dst, rect, rectColor, relToCamera);

    s32v2 leftBottom = rect.leftBottom;
    s32v2 rightTop = rect.rightTop;
    s32v2 leftTop = (s32v2){ leftBottom.x, rightTop.y };
    s32v2 rightBottom = (s32v2){ rightTop.x, leftBottom.y };

    Renderer_Draw_Line(dst, leftBottom, leftTop, outLineColor, segmentSize, relToCamera);
    Renderer_Draw_Line(dst, leftTop, rightTop, outLineColor, segmentSize, relToCamera);
    Renderer_Draw_Line(dst, rightTop, rightBottom, outLineColor, segmentSize, relToCamera);
    Renderer_Draw_Line(dst, rightBottom, leftBottom, outLineColor, segmentSize, relToCamera);
}

#if 1
Public void Renderer_Draw_Image(Image* dst, Rect dstRect, Image* src, Rect srcRect, f32v4 color) {
    u32 packedColor = Color_Pack(color);

    Rect clipedSrcRect = Rect_Clip(Rect_From_Image(src), srcRect);
    Rect clipedDstRect = Rect_Clip(Rect_From_Image(dst), dstRect);

    s32v2 srcOffset = clipedSrcRect.leftBottom;
    s32v2 dstOffset = clipedDstRect.leftBottom;

    f32v2 dstSrcRatio = f32v2_ElemWise_Div(s32v2_To_f32v2(Rect_Get_Dims(srcRect)), s32v2_To_f32v2(Rect_Get_Dims(dstRect)));

    s32v2 srcNewDims = {
        .width  = (s32)( Rect_Get_Dims(clipedDstRect).width  * dstSrcRatio.width  ),
        .height = (s32)( Rect_Get_Dims(clipedDstRect).height * dstSrcRatio.height ),
    };

    clipedSrcRect = Rect_Offset(Rect_Create_LD(dstRect.leftBottom, srcNewDims), s32v2_Sub(srcOffset, dstOffset));

    u32* dstScan = (u32*)Image_Data_At(dst, (u32v2){ (u32)clipedDstRect.leftBottom.x, (u32)clipedDstRect.leftBottom.y });
    u32* srcScan = (u32*)Image_Data_At(src, (u32v2){ (u32)clipedSrcRect.leftBottom.x, (u32)clipedSrcRect.leftBottom.y });

    s32v2 dstPoint = { 0 };
    s32v2 srcPoint = { 0 };

    dstPoint.y = clipedDstRect.leftBottom.y;
    srcPoint.y = clipedSrcRect.leftBottom.y;
    while ( dstPoint.y < clipedDstRect.rightTop.y ) {
        byte* dstRow = (byte*)dstScan;
        byte* srcRow = (byte*)srcScan;

        dstPoint.x = clipedDstRect.leftBottom.x;
        srcPoint.x = clipedSrcRect.leftBottom.x;
        while ( dstPoint.x <= clipedDstRect.rightTop.x ) {
            u32 srcScan = *(u32*)Image_Data_At(src, (u32v2){ (u32)srcPoint.x, (u32)srcPoint.y });
            Color_Alpha_Blend(dstScan++, f32v4_ElemWise_Mul(color, Color_Unpack(srcScan)));

            ++dstPoint.x;
            srcPoint.x = clipedSrcRect.leftBottom.x + (s32)( (f32)( dstPoint.x - clipedDstRect.leftBottom.x ) * dstSrcRatio.width );
        }

        dstScan = (u32*)( dstRow + dst->stride );
        srcScan = (u32*)( srcRow + src->stride );

        ++dstPoint.y;
        srcPoint.y = clipedSrcRect.leftBottom.y + (s32)( (f32)( dstPoint.y - clipedDstRect.leftBottom.y ) * dstSrcRatio.height );
    }
}
#else
Public void Renderer_Draw_Image(Window2* dst, Rect dstRect, Image* src, Rect srcRect, f32v4 color) {
    u32 packedColor = Color_Pack(color);

    Rect clipedSrcRect = Rect_Clip(Rect_From_Image(src), srcRect);
#if 1
    Rect clipedDstRect = Rect_Clip(Rect_From_Image(dst->image), dstRect);
#else
    Rect clipedDstRect = Rect_Clip(Rect_From_Image(dst->image),
                                   Rect_Offset(Rect_Clip(Rect_At_Origin(dst->rect), dstRect), dst->rect.leftBottom));
#endif

    s32v2 srcOffset = clipedSrcRect.leftBottom;
    s32v2 dstOffset = clipedDstRect.leftBottom;

    f32v2 srcDstRatio = f32v2_ElemWise_Div(s32v2_To_f32v2(Rect_Get_Dims(srcRect)), s32v2_To_f32v2(Rect_Get_Dims(dstRect)));

    s32v2 srcNewDims = {
        .width  = (s32)( Rect_Get_Dims(clipedDstRect).width  * srcDstRatio.width  ),
        .height = (s32)( Rect_Get_Dims(clipedDstRect).height * srcDstRatio.height ),
    };

    clipedSrcRect = Rect_Offset(Rect_Create_LD(dstRect.leftBottom, srcNewDims), s32v2_Sub(srcOffset, dstOffset));

    u32* dstScan = (u32*)Image_Data_At(dst->image, s32v2_To_u32v2(clipedDstRect.leftBottom));
    s32v2 dstPoint = { 0 };
    dstPoint.y = clipedDstRect.leftBottom.y;

    u32* srcScan = (u32*)Image_Data_At(src, (u32v2){ (u32)clipedSrcRect.leftBottom.x, (u32)clipedSrcRect.leftBottom.y });
    f32v2 srcPoint = { 0 };
    srcPoint.y = (f32)clipedSrcRect.leftBottom.y;

    for (dstPoint.y = clipedDstRect.leftBottom.y; dstPoint.y <= clipedDstRect.rightTop.y; ++dstPoint.y) {
        byte* dstRow = (byte*)dstScan;
        byte* srcRow = (byte*)srcScan;

        srcPoint.x = (f32)clipedSrcRect.leftBottom.x;
        for (dstPoint.x = clipedDstRect.leftBottom.x; dstPoint.x <= clipedDstRect.rightTop.x; ++dstPoint.x) {
            u32 srcScan = *(u32*)Image_Data_At(src, f32v2_To_u32v2(srcPoint));
            Color_Alpha_Blend(dstScan++, f32v4_ElemWise_Mul(color, Color_Unpack(srcScan)));

            srcPoint.x += srcDstRatio.width;
        }

        dstScan = (u32*)( dstRow + dst->image->stride );
        srcScan = (u32*)( srcRow + src->stride );

        srcPoint.y += srcDstRatio.height;
    }
}
#endif

Public s32 Renderer_Get_Last_Char_Pos(Font* font, String text) {
    s32 pos = 0;

    foreach (char, codePoint, text) {
        Code_Point_Data* data = Font_Get_Code_Point(font, *codePoint);
        pos += data->xAdvance;
    }

    return pos;
}

Public Rect Rederer_Get_Text_Rect(Font* font, String text, f32 fontScale) {
    Rect textRect = { 0 };
    s32v2 offset = { 0 };

    foreach (char, codePoint, text) {
        Code_Point_Data* data = Font_Get_Code_Point(font, *codePoint);
        Rect codePointRect = Rect_Offset(
            Rect_Create_LD((s32v2){ (s32)( (f32)data->offset.x * fontScale ), (s32)((f32)data->offset.y * fontScale ) },
            (s32v2){ (s32)( (f32)data->xAdvance * fontScale ), (s32)( (f32)data->dims.height * fontScale ) }), offset);

        textRect = Rect_Enclosing(textRect, codePointRect);
        offset.x += (s32)( (f32)data->xAdvance * fontScale );
    }

    return textRect;
}

Public Rect Rederer_Draw_Text(Image* image, Font* font, String text, s32v2 pos, f32 fontScale, f32v4 color, b8 relToCamera) {
    if ( relToCamera ) {
        s32v2_AddEq(&pos, f32v2_To_s32v2(gProgramState.camera.pos));
    }

    Rect textRect = { .leftBottom = pos, .rightTop = pos };

    foreach (char, codePoint, text) {
        Code_Point_Data* data = Font_Get_Code_Point(font, *codePoint);

        Rect codePointRect = Rect_Create_LD((s32v2){ (s32)data->pos.x, (s32)data->pos.y },
                                            (s32v2){ (s32)data->dims.width, (s32)data->dims.height });

        Rect dstRect = Rect_Create_LD(s32v2_Add(pos, (s32v2){ (s32)( (f32)data->offset.x * fontScale ), (s32)( (f32)data->offset.y * fontScale ) }),
                                      (s32v2){ (s32)( (f32)data->dims.width * fontScale ), (s32)( (f32)data->dims.height * fontScale ) });

        Renderer_Draw_Image(image, dstRect, font->bitmap, codePointRect, color);

        pos.x += (s32)( (f32)data->xAdvance * fontScale );

        textRect.rightTop.y = Max(textRect.rightTop.y, (s32)( (f32)data->dims.height * fontScale ));
        textRect.rightTop.x += (s32)( (f32)data->xAdvance * fontScale );
    }

    return textRect;
}
