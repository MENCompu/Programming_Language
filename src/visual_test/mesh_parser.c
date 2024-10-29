Public b8 Mesh_Converter_OBJ(Memory_Arena* arena, Mesh* mesh, String mem) {
    // TODO(JENH): Temporary.
    // TODO(JENH): This is bad, better memory handling.
#if 0
    u32 materialCount = 0;
    RCAF_Material materials[256];
    Mem_Zero_Array(materials);
    String materialNames[256];
#endif

    // TODO(JENH): This only handles position data.

    String firstFace = Str_End((String){ .E=mem.E, .count=mem.count }, Str_Find_Char_Backward(mem, 'f'));

    #define OBJ_PARSER_ERROR_CONSTANT 1.3f
    Array_f32v3 posData;
    Str_Parse_Fmt(firstFace, " %u", &posData.capacity);
    posData.capacity = (u32)( (f32)posData.capacity * OBJ_PARSER_ERROR_CONSTANT );
    posData.count = 1; // NOTE(JENH): This is because obj format indices starts at 1.

    posData.E = OS_Alloc_Array(f32v3, posData.capacity);

    b8 isFirstMesh = JENH_TRUE;

#if 0
    Array_Mesh meshes;
    Array_Create(meshes, KiB(1), &gTempArena);

    Mesh* mesh = Array_Push(meshes);
#endif

    *mesh = (Mesh){ 0 };

    mesh->indexSize = sizeof(u32);
    mesh->vertexSize = sizeof(f32v3);

    // TODO(JENH): This is bad, better memory handling.
    Array_u32v3 faceIndexData;
    faceIndexData.capacity = mem.count / sizeof(u32v3);
    faceIndexData.count = 0;
    faceIndexData.E = OS_Alloc_Array(u32v3, faceIndexData.capacity);

    b8 mtllibIsFirstFound = true;

    u32 lineCount = 1;

    for (String_Scan fileScan = Str_Scan_From_Str(mem); !StrScanEOS(fileScan);) {
        StrScanAdvance(&fileScan, FindAnyCharForward(fileScan.scan, fileScan.end, S("\r\n")));
        String_Scan line = Str_Scan_From_Str(fileScan.data);

        if ( fileScan.scan[0] == '\n' ) { ++fileScan.scan; }

        StrScanAdvance(&line, FindCharForward(line.scan, line.end, ' '));

        if ( StrScanEOS(line) ) { continue; }

        if ( Str_Equal(line.data, S("#")) ) {
            continue;
#if 0
        } else if ( Str_Equal(line.data, S("mtllib")) ) {
            mtllibIsFirstFound = JENH_TRUE;

            StrScanAdvance(&line, FindAnyCharForward(line.scan, line.end, S("\r\n")));

            String materialName = Str_Get_File_Name_In_Path(line.data);

            Local_Str(name, 256);
            Mem_Zero(name.str, 256);
            Mem_Copy_Forward(name.str, materialName.str, materialName.size);
            name.size = materialName.size;

            materialCount += Asset_Loader_Load_Material_MTL(inDirPath, name, inDstFile, materialNames);

            dstFileSize += (materialCount * sizeof(RCAF_Material));
        } else if (Str_Equal(line.data, S("usemtl"))) {
            StrScanAdvance(&line, FindAnyCharForward(line.scan, line.end, S("\r\n")));

            for (u32 i = 0; i < materialCount; ++i) {
                if ( Str_Equal(materialNames[i], line.data) ) {
                    mesh->materialID = i;

                    //if ( mesh->material.diffuseTex.assetID  == 0 ) { DEBUG_Breakpoint; }
                    //if ( mesh->material.specularTex.assetID == 0 ) { DEBUG_Breakpoint; }
                    //if ( mesh->material.normalTex.assetID   == 0 ) { DEBUG_Breakpoint; }

                    break;
                }
            }
#endif
        } else if ( Str_Equal(line.data, S("v")) ) {
            //Assert ( mtllibIsFirstFound );

            f32v3* pos = Array_Push(posData);
            Check( Str_Parse_Fmt(line.data, "v %f %f %f", &pos->x, &pos->y, &pos->z), > 0 );

#if 0
        } else if (Str_Equal(line.data, S("vn"))) {
            Assert ( mtllibIsFirstFound );

            f32x3* normal = &normalData[normalCount++];

            StrScanAdvance(&line, FindCharForward(line.scan, line.end, ' '));
            normal->x = StrToF32(line.data);

            StrScanAdvance(&line, FindCharForward(line.scan, line.end, ' '));
            normal->y = StrToF32(line.data);

            StrScanAdvance(&line, FindAnyCharForward(line.scan, line.end, S("\r\n")));
            normal->z = StrToF32(line.data);
        } else if (Str_Equal(line.data, S("vt"))) {
            Assert ( mtllibIsFirstFound );

            f32x2* texCoord = &texCoordsData[texCoordsCount++];

            StrScanAdvance(&line, FindCharForward(line.scan, line.end, ' '));
            texCoord->x = StrToF32(line.data);

            StrScanAdvance(&line, FindAnyCharForward(line.scan, line.end, S(" \r\n")));
            texCoord->y = StrToF32(line.data);
#endif
        } else if (Str_Equal(line.data, S("f"))) {
            Assert ( mtllibIsFirstFound );

            u32v3* faceindices = Array_Push(faceIndexData);
            Check( Str_Parse_Fmt(line.data, "f %u %u %u", &faceindices->E[0], &faceindices->E[1], &faceindices->E[2]), > 0 );

#if 0
            for (u32 i = 0; i < 3; ++i) {
                Tri_Indices *indices = &mulIndexData[mesh->indexCount++];

                StrScanAdvance(&line, FindCharForward(line.scan, line.end, '/'));
                indices->i1 = (u32)Str_To_S32(line.data);

                StrScanAdvance(&line, FindCharForward(line.scan, line.end, '/'));
                indices->i2 = (u32)Str_To_S32(line.data);

                StrScanAdvance(&line, FindAnyCharForward(line.scan, line.end, S(" \r\n")));
                indices->i3 = (u32)Str_To_S32(line.data);
            }
        } else if (Str_Equal(line.data, S("g"))) {
            Assert ( mtllibIsFirstFound );

            if ( !isFirstMesh ) {
                Vertex_Data* vertices = ArenaPushArray(arena, Vertex_Data, mesh->indexCount);
                u32* indices = ArenaPushArray(arena, u32, mesh->indexCount * 3);

                mesh->vertexCount = 0;

                f32x3 minPoint = F32x3(MAX_F32, MAX_F32, MAX_F32);
                f32x3 maxPoint = F32x3(-MAX_F32, -MAX_F32, -MAX_F32);

                for (u32 i = 0; i < mesh->indexCount; ++i) {
                    Tri_Indices* mulIndex = &mulIndexData[i];

                    Vertex_Data vertex;
                    vertex.pos = positionData[mulIndex->i1];
                    vertex.tex = texCoordsData[mulIndex->i2];
                    vertex.normal = normalData[mulIndex->i3];

                    u32* index = &indices[i];

                    for (u32 j = 0; j < mesh->vertexCount; ++j) {
                        Vertex_Data* savedVertex = &vertices[j];
                        if ( Mem_Type_Equal(&vertex, savedVertex) ) {
                            *index = j;
                            goto duplicate_found;
                        }
                    }

                    minPoint.x = Min(minPoint.x, vertex.pos.x);
                    minPoint.y = Min(minPoint.y, vertex.pos.y);
                    minPoint.z = Min(minPoint.z, vertex.pos.z);

                    maxPoint.x = Max(maxPoint.x, vertex.pos.x);
                    maxPoint.y = Max(maxPoint.y, vertex.pos.y);
                    maxPoint.z = Max(maxPoint.z, vertex.pos.z);

                    *index = mesh->vertexCount;
                    vertices[mesh->vertexCount++] = vertex;

                    duplicate_found:;
                }

                mesh->halfDim.x = (maxPoint.x - minPoint.x) * 0.5f;
                mesh->halfDim.y = (maxPoint.y - minPoint.y) * 0.5f;
                mesh->halfDim.z = (maxPoint.z - minPoint.z) * 0.5f;

                mesh->center = minPoint + mesh->halfDim;

                mesh->vertexOffset = dstFileSize;
                Assert( File_Write_At(inDstFile, mesh->vertexOffset, mesh->vertexCount * mesh->vertexSize, vertices) );
                dstFileSize += mesh->vertexCount * mesh->vertexSize;

                mesh->indexOffset = dstFileSize;
                Assert( File_Write_At(inDstFile, mesh->indexOffset, mesh->indexCount * mesh->indexSize, indices) );
                dstFileSize += mesh->indexCount * mesh->indexSize;

                Arena_Clear(arena);

                mesh = &meshes[meshCount++];

                mesh->indexSize = sizeof(u32);
                mesh->vertexSize = sizeof(Vertex_Data);
                mesh->indexCount = 0;
                mesh->vertexCount = 0;
            }

            isFirstMesh = JENH_FALSE;
#endif
        }

        ++lineCount;
    }

    /* The same that the 'g' input. */ {
        f32v3 AABBminPoint = {  MAX_F32,  MAX_F32,  MAX_F32 };
        f32v3 AABBmaxPoint = { -MAX_F32, -MAX_F32, -MAX_F32 };

        Slice_u32 indexData = {
            .count = faceIndexData.count * 3,
            .E = (u32*)faceIndexData.E,
        };

        Array_u32 indices;
        Array_Create(indices, faceIndexData.count * 3, arena);

        Array_f32v3 positions;
        Array_Create(positions, posData.count, arena);

        foreach (u32, rawIndex, indexData) {
            f32v3 pos = posData.E[*rawIndex];

            // check if vertex is duplicated.
            for (u32 posIndex = 0; posIndex < positions.count; ++posIndex) {
                if ( Mem_Type_Equal(&positions.E[posIndex], &pos) ) {
                    Array_Push_C(indices, posIndex);
                    goto is_duplicated;
                }
            }

            /* Is a new vertex */ {
                Array_Push_C(indices, positions.count);
                Array_Push_C(positions, pos);

                /* Check bound points */ {
                    AABBminPoint = (f32v3){
                        .x = Min(AABBminPoint.x, pos.x),
                        .y = Min(AABBminPoint.y, pos.y),
                        .z = Min(AABBminPoint.z, pos.z),
                    };

                    AABBmaxPoint = (f32v3){
                        .x = Max(AABBmaxPoint.x, pos.x),
                        .y = Max(AABBmaxPoint.y, pos.y),
                        .z = Max(AABBmaxPoint.z, pos.z),
                    };
                }

                is_duplicated:;
            }
        }

        mesh->vertices = Array_To_Slice(f32v3, positions);
        mesh->indices  = Array_To_Slice(u32, indices);

        mesh->AABB.halfDim = (f32v3){
            .x = (AABBmaxPoint.x - AABBminPoint.x) * 0.5f,
            .y = (AABBmaxPoint.y - AABBminPoint.y) * 0.5f,
            .z = (AABBmaxPoint.z - AABBminPoint.z) * 0.5f,
        };

        mesh->AABB.center = f32v3_Add(AABBminPoint, mesh->AABB.halfDim);

#if 0
        // NOTE(JENH): copy data to file.

        mesh->vertexOffset = dstFileSize;
        Assert( File_Write_At(inDstFile, mesh->vertexOffset, mesh->vertexCount * mesh->vertexSize, vertices) );
        dstFileSize += mesh->vertexCount * mesh->vertexSize;

        mesh->indexOffset = dstFileSize;
        Assert( File_Write_At(inDstFile, mesh->indexOffset, mesh->indexCount * mesh->indexSize, indices) );
        dstFileSize += mesh->indexCount * mesh->indexSize;
#endif
    }

#if 0
    RCAF_Model model;
    model.meshCount = meshCount;
    model.meshesOffset = dstFileSize;
    model.materialCount = materialCount;

    File_Write_At(inDstFile, 0, sizeof(RCAF_Model), &model);
    File_Write_At(inDstFile, model.meshesOffset, model.meshCount * sizeof(RCAF_Mesh), meshes);
#endif

    OS_Free_Mem(posData.E);
    OS_Free_Mem(faceIndexData.E);

    return true;
}
