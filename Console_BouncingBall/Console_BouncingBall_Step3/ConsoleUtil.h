#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

extern HANDLE g_hConsole;

void InitializeConsole();
void FinalizeConsole();
void SetCursorPosition( int x, int y );
