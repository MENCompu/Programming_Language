Public u32 Get_Fn_Index(String name) {
    forrange (index, gFnsInfo.count) {
        Raw_Fn* fnInfo = &gFnsInfo.E[index];
        if ( Str_Equal(fnInfo->name, name) ) {
            return index;
        }
    }

    LogFatal("Failed to find a c function with the given name");
    return 0;
}

Public Raw_Fn* Get_C_Fn(String name) {
    foreach (Raw_Fn, fnInfo, gFnsInfo) {
        if ( Str_Equal(fnInfo->name, name) ) {
            return fnInfo;
        }
    }

    LogFatal("Failed to find a c function with the given name");
    return 0;
}

Public Raw_Type* Raw_Type_Get(String name) {
    foreach (Raw_Type, typeInfo, cTypeInfo) {
        if ( Str_Equal(typeInfo->name, name) ) {
            return typeInfo;
        }
    }

    LogFatal("Failed to find a c type with the given name");
    return 0;
}
