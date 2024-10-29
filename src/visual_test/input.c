Public void Input_Init(void) {
    Slice_Fn_Info_C gFnsInfo = Slice_From_Raw_Array(Fn_Info_C, gFnInfoBuffer);

    Slice_Create(gInputMetadata.deivces, INPUT_DEVICE_TAG_COUNT, &gPermArena);

    foreach (Input_Device_Metadata, device, gInputMetadata.deivces) {
        foreach (Input_Unit_Metadata, unit, device->units) {
            unit->offset
        }
    }

    Slice_Create(gInputMetadata, INPUT_DEVICE_TAG_COUNT, &gPermArena);
}
