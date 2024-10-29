typedef struct {
    f32v3 center;
    f32v3 halfDim;
} Box_3D;

typedef struct {
    s32v2 leftBottom;
    s32v2 rightTop;
} Rect;
Array(Rect);

typedef struct {
    f32v2 leftBottom;
    f32v2 rightTop;
} Rect_f32;
Array(Rect_f32);

typedef struct Mesh {
    u8 vertexSize;
    Slice_f32v3 vertices;

    u8 indexSize;
    Slice_u32 indices;

    Box_3D AABB;
} Mesh;
Array(Mesh);

typedef struct {
    u32v2 dims;
    u32 stride;
    b8 hasTransparency;
    u8 channelCount;
    u8 channelSizes[4];
    void* data;
} Image;

typedef struct RCAF_Code_Point_Data {
    char id;
    u16v2 pos;
    u16v2 dims;
    s16v2 offset;
    u16 xAdvance;
} Code_Point_Data;
Array(Code_Point_Data);
Slice(Code_Point_Data);

typedef struct {
    Image* bitmap;
    u16v2 bitmapDims;

    u16 lineHeight;

    Slice_Code_Point_Data table;
} Font;

typedef enum {
    ASSET_TAG_Mesh,
    ASSET_TAG_Image,
    ASSET_TAG_Font,
} Asset_Tag;

typedef struct {
    String name;

    Asset_Tag tag;
    union {
        byte data[];
        Mesh  mesh;
        Image image;
        Font  font;
    };
} Asset;
Array(Asset);

Private Global Array_Asset gAssets;

Public b8 Font_Code_Point_Exists(Font* font, char id) {
    foreach (Code_Point_Data, data, font->table) {
        if ( data->id == id ) { return true; }
    }

    return false;
}

Public Code_Point_Data* Font_Get_Code_Point(Font* font, char id) {
    foreach (Code_Point_Data, data, font->table) {
        if ( data->id == id ) {
            return data;
        }
    }

    LogError("Couldn't find the code point id");
    return 0;
}

#define ASSET_DIR_NAME "P:\\src\\visual_test\\assets"

Public Slice_byte Asset_Read_File(String name) {
    Slice_byte retData;

    Local_Str(path, KiB(4));
    Str_From_Fmt(&path, "%s\\%s", ASSET_DIR_NAME, name.E);

    File_Handle file = File_Open(path.E);

    retData.E = (byte*)File_Read_All(path, &gTempArena, &retData.count);

    File_Close(file);

    return retData;
}

Public b8 Mesh_Converter_OBJ(Memory_Arena* arena, Mesh* mesh, String mem);
Public b8 Image_Converter_TGA(Memory_Arena* arena, Image* image, Slice_byte mem);
Public b8 Font_Converter_Fnt(Memory_Arena* arena, Font* font, String mem);

#define Asset_Get(type, name) (type*)Asset_Get_(name, P(ASSET_TAG_, type))
Public byte* Asset_Get_(String name, Asset_Tag tag) {
    foreach (Asset, asset, gAssets) {
        if ( Str_Equal(asset->name, name) && asset->tag == tag ) {
            return asset->data;
        }
    }

    Asset* newAsset = Array_Push(gAssets);
    newAsset->tag = tag;
    newAsset->name = name;

    /* Load Asset */ {
        Slice_byte data = Asset_Read_File(name);

        switch ( newAsset->tag ) {
            case ASSET_TAG_Mesh:  { (void)Mesh_Converter_OBJ(&gPermArena, &newAsset->mesh, Str((char*)data.E, data.count)); } break;
            case ASSET_TAG_Image: { (void)Image_Converter_TGA(&gPermArena, &newAsset->image, data); } break;
            case ASSET_TAG_Font:  { (void)Font_Converter_Fnt(&gPermArena, &newAsset->font, Str((char*)data.E, data.count)); } break;
        }
    }

    return newAsset->data;
}
