#pragma pack(push, 1)
typedef struct {
    u8 b;
    u8 g;
    u8 r;
    u8 a;
} Packed_Color_BGRA;
#pragma pack(pop)

Public f32v4 Color_Unpack(u32 packedColor) {
    f32v4 ret = {
        .b = (f32)( ( packedColor & 0x000000ff ) >> 0  ) / 255.0f,
        .g = (f32)( ( packedColor & 0x0000ff00 ) >> 8  ) / 255.0f,
        .r = (f32)( ( packedColor & 0x00ff0000 ) >> 16 ) / 255.0f,
        .a = (f32)( ( packedColor & 0xff000000 ) >> 24 ) / 255.0f,
    };
    return ret;
}

Public u32 Color_Pack(f32v4 color) {
    // TODO(JENH): Handle all kinds of color channels combinations. This one is BGRA.
#if 0
    Packed_Color_BGRA ret = {
        .b = (u8)( 255.0f * color.b ),
        .g = (u8)( 255.0f * color.g ),
        .r = (u8)( 255.0f * color.r ),
        .a = (u8)( 255.0f * color.a ),
    };
#else
    return ( ( (u32)(255.0f*color.b) << 0  ) |
             ( (u32)(255.0f*color.g) << 8  ) |
             ( (u32)(255.0f*color.r) << 16 ) |
             ( (u32)(255.0f*color.a) << 24 ) );
#endif
}

#define RED    (f32v4){ 1.0f, 0.0f, 0.0f, 1.0f }
#define GREEN  (f32v4){ 0.0f, 1.0f, 0.0f, 1.0f }
#define BLUE   (f32v4){ 0.0f, 0.0f, 1.0f, 1.0f }
#define YELLOW (f32v4){ 1.0f, 1.0f, 0.0f, 1.0f }
#define BLACK  (f32v4){ 0.0f, 0.0f, 0.0f, 1.0f }
#define WHITE  (f32v4){ 1.0f, 1.0f, 1.0f, 1.0f }

Public f32v4 Color_Grey_Scale(f32 grey) {
    return (f32v4){ grey, grey, grey, 1.0f };
}

Public void Color_Alpha_Blend(u32* dst, f32v4 color) {
    f32v4 dstColor;
    dstColor.rgb = f32v3_Add(f32v3_Mul(Color_Unpack(*dst).rgb, 1.0f - color.a), f32v3_Mul(color.rgb, color.a));
    dstColor.a = 1.0f;

    *dst = Color_Pack(dstColor);
}

Public Rect Rect_Offset(Rect rect, s32v2 offset) {
    return (Rect){
        .leftBottom = s32v2_Add(rect.leftBottom, offset),
        .rightTop   = s32v2_Add(rect.rightTop  , offset),
    };
}

Public Rect Rect_At_Origin(Rect rect) {
    return Rect_Offset(rect, (s32v2){ -rect.leftBottom.x, -rect.leftBottom.y });
}

Public s32v2 Rect_Get_Dims(Rect rect) {
    return (s32v2){ .width = ( rect.rightTop.x - rect.leftBottom.x ), .height = ( rect.rightTop.y - rect.leftBottom.y ) };
}

Public s32v2 Rect_Get_Center(Rect rect) {
    s32v2 dims = Rect_Get_Dims(rect);
    return s32v2_Add(rect.leftBottom, (s32v2){ dims.width / 2, dims.height / 2 });
}

Public b8 Rect_Is_Point_Inside(Rect rect, s32v2 point) {
    return (b8)( rect.leftBottom.x <= point.x && point.x <= rect.rightTop.x &&
                 rect.leftBottom.y <= point.y && point.y <= rect.rightTop.y );
}

Public b8 Rect_Is_Rect_Colliding(Rect rect1, Rect rect2) {
    s32v2 rect2HalfDims = { Rect_Get_Dims(rect2).width / 2, Rect_Get_Dims(rect2).height / 2 };

    Rect newRect1 = {
        .leftBottom = s32v2_Sub(rect1.leftBottom, rect2HalfDims),
        .rightTop   = s32v2_Add(rect1.rightTop  , rect2HalfDims),
    };

    return Rect_Is_Point_Inside(newRect1, Rect_Get_Center(rect2));
}

Public Rect Rect_Create_PP(s32v2 point1, s32v2 point2) {
    return (Rect){
        .leftBottom = { .x = Min(point1.x, point2.x), .y = Min(point1.y, point2.y) },
        .rightTop   = { .x = Max(point1.x, point2.x), .y = Max(point1.y, point2.y) },
    };
}

Public Rect Rect_Create_LD(s32v2 leftBottom, s32v2 dims) {
    return (Rect){
        .leftBottom = leftBottom,
        .rightTop   = s32v2_Add(leftBottom, dims),
    };
}

Public Rect Rect_f32_To_Rect(Rect_f32 rect) {
    return (Rect){
        .leftBottom = f32v2_To_s32v2(rect.leftBottom),
        .rightTop   = f32v2_To_s32v2(rect.rightTop),
    };
}

Public Rect_f32 Rect_f32_Create_LD(f32v2 leftBottom, f32v2 dims) {
    return (Rect_f32){
        .leftBottom = leftBottom,
        .rightTop   = f32v2_Add(leftBottom, dims),
    };
}

Public Rect Rect_Create_CH(s32v2 center, s32v2 halfDims) {
    return (Rect){
        .leftBottom = s32v2_Sub(center, halfDims),
        .rightTop   = s32v2_Add(center, halfDims),
    };
}

Public Rect Rect_From_Image(Image* image) {
    return (Rect){
        .leftBottom = { 0, 0 },
        .rightTop   = (s32v2){ (s32)image->dims.width, (s32)image->dims.height, },
    };
}

Public u32 Image_Get_Byte_Per_Pixel(Image* image) {
    u32 bytesPerPixel = 0;
    for (u32 i = 0; i < image->channelCount; ++i) {
        bytesPerPixel += image->channelSizes[i];
    }

    return bytesPerPixel / BYTE_SIZE;
}

Public u32 Image_Pixel_Count(Image* image) {
    return image->dims.width * image->dims.height;
}

Public u32 Image_Size(Image* image) {
    return Image_Pixel_Count(image) * Image_Get_Byte_Per_Pixel(image);
}

Public Rect Rect_Enclosing(Rect parent, Rect child) {
    return (Rect){
        .leftBottom = { CapTop(parent.leftBottom.x, child.leftBottom.x), CapTop(parent.leftBottom.y, child.leftBottom.y) },
        .rightTop   = { CapBtm(parent.rightTop.x, child.rightTop.x), CapBtm(parent.rightTop.y, child.rightTop.y) },
    };
}

Public Rect Rect_Clip(Rect parent, Rect child) {
    Rect cliped = {
        .leftBottom = { CapBtm(parent.leftBottom.x, child.leftBottom.x), CapBtm(parent.leftBottom.y, child.leftBottom.y) },
        .rightTop   = { CapTop(parent.rightTop.x, child.rightTop.x), CapTop(parent.rightTop.y, child.rightTop.y) },
    };

    return cliped;
}

Public byte* Image_Data_At(Image* image, u32v2 pos) {
    return (byte*)image->data + ( Image_Get_Byte_Per_Pixel(image) * pos.x ) + ( image->stride * pos.y );
}

Public void Image_Copy(Image* dst, Image* src) {
    Assert(
        dst->channelCount == src->channelCount &&
        Mem_Equal(dst->channelSizes, src->channelSizes, sizeof(src->channelSizes))
    );

    Rect cliped = Rect_Clip(Rect_From_Image(dst), Rect_From_Image(src));

    byte* dstScan = Image_Data_At(dst, (u32v2){ 0, ( dst->dims.height - 1 ) });
    byte* srcScan = Image_Data_At(src, (u32v2){ cliped.leftBottom.x, ( cliped.leftBottom.y + src->dims.height - 1 ) });

    for (s32 y = cliped.leftBottom.y; y < cliped.rightTop.y; ++y) {
        Mem_Copy_Forward(dstScan, srcScan, ( cliped.rightTop.x - cliped.leftBottom.x ) * Image_Get_Byte_Per_Pixel(src));

        dstScan -= dst->stride;
        srcScan -= src->stride;
    }
}

Private Global Image sample;
Public Image Image_Create_Sample(void) {
    u32v2 dims = { .width=256, .height=256 };
    Image image = {
        .dims = dims,
        .hasTransparency = false,
        .channelCount = 4,
        .channelSizes = {8, 8, 8, 8},
        .data = Arena_Alloc_Array(&gPermArena, u32, dims.width * dims.height),
    };

    image.stride = image.dims.width * Image_Get_Byte_Per_Pixel(&image);

    u32* texels = (u32*)image.data;
    u32 texelIndex = 0;
    for (u32 row = 0; row < image.dims.width; ++row) {
        for (u32 col = 0; col < image.dims.height; ++col) {
            if ((col & (0b00010000)) != 0) {
                if ((row & (0b00010000)) != 0) {
                    texels[texelIndex] = 0xffa0a0a0;
                } else {
                    texels[texelIndex] = 0x00000000;
                }
            } else {
                if ((row & (0b00010000)) != 0) {
                    texels[texelIndex] = 0x00000000;
                } else {
                    texels[texelIndex] = 0xffa0a0a0;
                }
            }

            ++texelIndex;
        }
    }

    sample = image;
    return image;
}
