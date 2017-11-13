//---------------------------------------------------------------------
// Name: WriteLog.cpp
// Desc: debug Utility functions 
//       programmed by seojt@kogsoft.com on November 16, 2000
//
#define D3D_OVERLOADS

#include "kogGen.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

const char  g_logFilename[10] = "log.txt";
BOOL        g_bLogEnabled = TRUE;

void Log_WriteString(const char* lpszMessage)
{
    if (!g_bLogEnabled) return;
    
    
    FILE* fp;
    
    fopen_s( &fp, g_logFilename, "a");
    if (fp == NULL) return;
    
    fprintf(fp, lpszMessage);
    fclose(fp);
}

void DebugOut(const char *msg, ...) 
{
#ifndef _DEBUG
    MessageBoxA( NULL, "You used DebugOut() in Release mode", "Error", MB_OK );
#endif // _DEBUG

    va_list argList;
    char buffer[512] = {0,};   
    
    va_start(argList, msg);        
    vsprintf_s(buffer, msg, argList);    
    va_end(argList);
    
    OutputDebugStringA(buffer);
    //OutputDebugString("\n");
}

BOOL Log_WriteVString(const char *lpszText, ... )
{
    // return when log isn't enabled
    if (!g_bLogEnabled) return FALSE;
    
    
    va_list argList;
    FILE*   pFile;
    
    // initialize variable argument list
    va_start(argList, lpszText);
    
    // open the log file for append
    fopen_s( &pFile, g_logFilename, "a" );
    if( pFile == NULL )
        return(FALSE);
    
    // write the text and a newline
    vfprintf(pFile, lpszText, argList);
    //putc('\n', pFile);
    
    // close the file
    fclose(pFile);
    va_end(argList);
    
    // return success
    return TRUE;
}

