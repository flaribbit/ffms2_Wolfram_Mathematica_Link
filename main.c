#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "WolframImageLibrary.h"
#include "WolframLibrary.h"
#include "ffms.h"

DLLEXPORT int WolframLibrary_getVersion() {
    return WolframLibraryVersion;
}
DLLEXPORT int WolframLibrary_initialize(WolframLibraryData libData) {
    return LIBRARY_NO_ERROR;
}
DLLEXPORT void WolframLibrary_uninitialize(WolframLibraryData libData) {
    return;
}

extern DLLEXPORT int Init(WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res) {
    FFMS_Init(0, 0);
    return LIBRARY_NO_ERROR;
}

DLLEXPORT int OpenVideoSource(WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res) {
    //Get file name from mma
    char *filename = MArgument_getUTF8String(*Args);
    FFMS_ErrorInfo errinfo;
    errinfo.BufferSize = 1024;
    errinfo.Buffer = (char *)malloc(errinfo.BufferSize);
    //Get video source
    FFMS_Indexer *indexer = FFMS_CreateIndexer(filename, &errinfo);
    FFMS_Index *index = FFMS_DoIndexing2(indexer, FFMS_IEH_ABORT, &errinfo);
    int trackid = FFMS_GetFirstTrackOfType(index, FFMS_TYPE_VIDEO, &errinfo);
    FFMS_VideoSource *videosource = FFMS_CreateVideoSource(filename, trackid, index, 1, FFMS_SEEK_NORMAL, &errinfo);
    FFMS_DestroyIndex(index);
    //Set output pixel format
    int pixfmts[2] = {0};
    pixfmts[0] = FFMS_GetPixFmt("rgb24");
    pixfmts[1] = -1;
    const FFMS_Frame *frame = FFMS_GetFrame(videosource, 0, &errinfo);
    FFMS_SetOutputFormatV2(videosource, pixfmts, frame->EncodedWidth, frame->EncodedHeight, FFMS_RESIZER_BICUBIC, &errinfo);
    //Return video source to mma
    MArgument_setInteger(Res, (mint)videosource);
    return LIBRARY_NO_ERROR;
}

DLLEXPORT int CloseVideoSource(WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res) {
    FFMS_VideoSource *video = (FFMS_VideoSource *)MArgument_getInteger(*Args);
    FFMS_DestroyVideoSource(video);
    return LIBRARY_NO_ERROR;
}

DLLEXPORT int GetFrame(WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res) {
    WolframImageLibrary_Functions imageFunctions = libData->imageLibraryFunctions;
    FFMS_VideoSource *video = (FFMS_VideoSource *)MArgument_getInteger(Args[0]);
    mint n = MArgument_getInteger(Args[1]);
    mint pos[2] = {0};
    MImage image;
    FFMS_ErrorInfo errinfo;
    errinfo.BufferSize = 1024;
    errinfo.Buffer = (char *)malloc(errinfo.BufferSize);
    const FFMS_Frame *frame = FFMS_GetFrame(video, n, &errinfo);
    int w = frame->EncodedWidth;
    int h = frame->EncodedHeight;
    int err = imageFunctions->MImage_new2D(w, h, 3, MImage_Type_Bit8, MImage_CS_RGB, 1, &image);
    //const uint8_t const *pdata[4]=(const unsigned char (*)[4])frame->Data;
    uint8_t *pdata = (uint8_t *)frame->Data[0];
    for (pos[0] = 1; pos[0] <= h; pos[0]++) {
        for (pos[1] = 1; pos[1] <= w; pos[1]++) {
            imageFunctions->MImage_setByte(image, pos, 1, *pdata++);
            imageFunctions->MImage_setByte(image, pos, 2, *pdata++);
            imageFunctions->MImage_setByte(image, pos, 3, *pdata++);
        }
    }
    MArgument_setMImage(Res, image);
    return LIBRARY_NO_ERROR;
}
