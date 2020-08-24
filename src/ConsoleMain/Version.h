// update these
#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_PATCH 0

///////////////////////////////////////////////////////////////////////////////

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)


#define VER_FILE_VERSION VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH
#define VER_FILE_VERSION_STR STRINGIFY(VERSION_MAJOR) "." \
                             STRINGIFY(VERSION_MINOR) "." \
                             STRINGIFY(VERSION_PATCH)
#define VER_PRODUCT_VERSION VER_FILE_VERSION
#define VER_PRODUCT_VERSION_STR VER_FILE_VERSION_STR

#define VER_PRODUCT_NAME_STR "CppBuildAnalyzer"
#define VER_ORIGINAL_FILENAME_STR VER_PRODUCT_NAME_STR ".exe"
#define VER_INTERNAL_NAME_STR VER_ORIGINAL_FILENAME_STR

#ifdef _DEBUG
#define VER_DEBUG_FLAGS VS_FF_DEBUG
#else
#define VER_DEBUG_FLAGS 0
#endif

#define VER_FILE_FLAGS VER_DEBUG_FLAGS
#define VER_FILE_OS VOS_NT_WINDOWS32
#define VER_FILE_TYPE VFT_APP

///////////////////////////////////////////////////////////////////////////////
