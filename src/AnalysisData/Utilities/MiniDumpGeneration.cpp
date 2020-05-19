#include "MiniDumpGeneration.h"

namespace
{
    const char* REPORT_MESSAGE =
        "If you wish to report this crash please go to\n"
        "https://github.com/MetanoKid/cpp-build-analyzer/issues\n"
        "and create a new issue.\n"
        "Please mention the commit you're working on and attach the\n"
        "generated .dmp file to help us investigate.\n"
        "\n"
        "============================= WARNING =============================\n"
        "The .dmp file includes data that can be considered sensitive\n"
        "(i.e. variable's values like the name of the analyzed .etl file).\n"
        "===================================================================\n"
        "\n"
        "Thank you!";

    const char* EXECUTABLE_EXTENSION = ".exe";
    const size_t EXECUTABLE_EXTENSION_LENGTH = strlen(EXECUTABLE_EXTENSION);

    // http://www.debuginfo.com/articles/effminidumps.html
    const MINIDUMP_TYPE MINIDUMP_FLAGS = MINIDUMP_TYPE(MiniDumpNormal |
                                                       MiniDumpWithIndirectlyReferencedMemory |
                                                       MiniDumpScanMemory);

    void BuildFullFilePath(wchar_t* buffer, int maxSize)
    {
        // full path to executable (includes extension)
        DWORD written = GetModuleFileName(GetModuleHandle(0), buffer, maxSize);

        SYSTEMTIME timestamp;
        GetSystemTime(&timestamp);

        // compose path with timestamp
        wsprintf(buffer + written - EXECUTABLE_EXTENSION_LENGTH,  // remember to remove extension
                 L"_%4d%02d%02d_%02d%02d%02d.dmp",  // YYYYMMDD_HHmmSS
                 timestamp.wYear, timestamp.wMonth, timestamp.wDay,
                 timestamp.wHour, timestamp.wMinute, timestamp.wSecond);
    }
}

// https://docs.microsoft.com/windows/win32/dxtecharts/crash-dump-analysis
LONG WINAPI GenerateMiniDump(PEXCEPTION_POINTERS exceptionPointers)
{
    printf("\n");
    printf("---------------------------------------\n");
    printf("Program crashed. Generating minidump...\n");

    wchar_t fullDumpFilePath[MAX_PATH];
    BuildFullFilePath(fullDumpFilePath, MAX_PATH);

    HANDLE fileHandle = CreateFile(fullDumpFilePath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL,
                                   CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (fileHandle != NULL && fileHandle != INVALID_HANDLE_VALUE)
    {
        MINIDUMP_EXCEPTION_INFORMATION exceptionInformation;
        exceptionInformation.ThreadId = GetCurrentThreadId();
        exceptionInformation.ExceptionPointers = exceptionPointers;
        exceptionInformation.ClientPointers = FALSE;

        BOOL dumped = MiniDumpWriteDump(GetCurrentProcess(),
                                        GetCurrentProcessId(),
                                        fileHandle,
                                        MINIDUMP_FLAGS,
                                        exceptionPointers != NULL ? &exceptionInformation : NULL,
                                        NULL,
                                        NULL);
        CloseHandle(fileHandle);

        if (dumped)
        {
            printf("Minidump written successfully to:\n");
            printf("%ws\n\n", fullDumpFilePath);
            printf("%s\n", REPORT_MESSAGE);
        }
        else
        {
            DeleteFile(fullDumpFilePath);
            printf("Couldn't write minidump. Error: %d\n", GetLastError());
        }
    }
    else
    {
        printf("Couldn't create file for minidump. Error: %d\n", GetLastError());
    }

    printf("---------------------------------------\n");

    return EXCEPTION_CONTINUE_SEARCH;
}