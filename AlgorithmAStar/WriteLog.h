////////////////////////////////////////////////////////////////////
// File: WriteLog.h
// Desc: debug utility functions
//       programmed by seojt@kogsoft.com on November 16, 2000
//       modified on November 16, 2000
//       modified on 3 April, 2001
//       last modified on 20 April, 2001
//

#include <windows.h>

extern const char  g_logFilename[];
extern BOOL        g_bLogEnabled;

void Log_WriteString(const char* lpszMessage);
void DebugOut(const char *msg, ...) ;
BOOL Log_WriteVString(const char *lpszText, ... );
