Public b8 Font_Converter_Fnt(Memory_Arena* arena, Font* font, String mem) {
    Array_Code_Point_Data tempTable = { 0 };

    u32 lineNumber = 0;

    for (String_Scan fileScan = StrScan(mem.E, mem.E + mem.count); !StrScanEOS(fileScan);) {
        StrScanAdvance(&fileScan, FindAnyCharForward(fileScan.scan, fileScan.end, S("\r\n")));
        String_Scan line = Str_Scan_From_Str(fileScan.data);

        if (fileScan.scan[0] == '\n') { ++fileScan.scan; }

        StrScanAdvance(&line, FindCharForward(line.scan, line.end, ' '));
        ++lineNumber;

        if ( StrScanEOS(line) ) { continue; }

        if ( Str_Equal(line.data, S("common")) ) {
            //  pages=%d packed=%d alphaChnl=%d
            Str_Parse_Fmt(line.data, "common lineHeight=%hu base=%*d scaleW=%hu scaleH=%hu",
                          &font->lineHeight, &font->bitmapDims.width, &font->bitmapDims.height);
        } else if ( Str_Equal(line.data, S("page")) ) {
            Local_Str(bitmapName, KiB(4));

            Str_Parse_Fmt(line.data, "page id=%*d file=\"%[^\"]\"", bitmapName.E);
            bitmapName.count = CStr_Len(bitmapName.E);

            font->bitmap = Asset_Get(Image, bitmapName);
        } else if ( Str_Equal(line.data, S("chars")) ) {
            Str_Parse_Fmt(line.data, "chars count=%u", &tempTable.capacity);

            Array_Create(tempTable, tempTable.capacity, arena);
        } else if ( Str_Equal(line.data, S("char")) ) {
            Code_Point_Data data = { 0 };

            // page=0 chnl=15
            Str_Parse_Fmt(line.data, "char id=%hhd x=%hu y=%hu width=%hu height=%hu xoffset=%hd yoffset=%hd xadvance=%hu",
                          &data.id, &data.pos.x, &data.pos.y, &data.dims.width, &data.dims.height, &data.offset.x, &data.offset.y, &data.xAdvance);

            // NOTE(JENH): As the images in the code are handled from bottom to top, this should be inverted.
            data.pos.y = (u16)font->bitmap->dims.height - data.pos.y - data.dims.height;
            data.offset.y = font->lineHeight - ( data.dims.height + data.offset.y );

            Array_Push_C(tempTable, data);
        }
    }

    font->table = Array_To_Slice(Code_Point_Data, tempTable);

    return true;
}
