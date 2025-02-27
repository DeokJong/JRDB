#include "FileException.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "Util.h"

#ifdef _WIN32
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#else
#include <execinfo.h>
#endif

FileException::FileException(const std::string &msg) : message(msg)
{
    captureStackTrace();
}

const char *FileException::what() const noexcept
{
    return message.c_str();
}

void FileException::captureStackTrace()
{
#ifdef _WIN32
    void *stack[64];
    unsigned short frames;
    SYMBOL_INFO *symbol;
    HANDLE process = GetCurrentProcess();

    SymInitialize(process, NULL, TRUE);
    frames = CaptureStackBackTrace(0, 64, stack, NULL);

    symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    for (unsigned short i = 0; i < frames; i++)
    {
        SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
        stackTrace.push_back(symbol->Name);
    }
    free(symbol);

#else
    void *buffer[64];
    int frames = backtrace(buffer, 64);
    char **symbols = backtrace_symbols(buffer, frames);

    for (int i = 0; i < frames; i++)
    {
        stackTrace.push_back(symbols[i]);
    }
    free(symbols);
#endif
}

std::vector<std::string> FileException::getStackTrace() const
{
    return stackTrace;
}

void FileException::printStackTrace() const
{
    std::cerr << "EXCEPTION: " << message << std::endl;
    std::cerr << "Stack Trace:" << std::endl;
    for (const auto &frame : stackTrace)
    {
        std::cerr << "\t" << frame << std::endl;
    }
    std::cerr << "----------------------------------" << std::endl;
    
    this->saveLogFile();
}

void FileException::saveLogFile() const
{
    std::string fileName = "error-" + Util::getCurrentTimeString() + ".log";
    std::ofstream logFile(fileName, std::ios::app);
    if (logFile.is_open())
    {
        logFile << "EXCEPTION: " << message << std::endl;
        logFile << "Stack Trace:" << std::endl;
        for (const auto &frame : stackTrace)
        {
            logFile << "\t" << frame << std::endl;
        }
        logFile << "----------------------------------" << std::endl;
        logFile.close();
    }
}