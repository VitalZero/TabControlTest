#pragma once
#include "includes.h"

// Definitinons
ATOM RegisterWC(const wchar_t* className, WNDPROC WindowProc);
BOOL CALLBACK ChangeChildrenFontProc(HWND hwnd, LPARAM lparam);


// Declarations
ATOM RegisterWC(const wchar_t* className, WNDPROC WindowProc)
{
  WNDCLASS wc = {0};

  wc.lpfnWndProc = WindowProc;
  wc.hInstance = GetModuleHandle(nullptr);
  wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//(HBRUSH)GetStockObject(BLACK_BRUSH);
  wc.lpszClassName = className;

  return RegisterClass(&wc);
}

// BOOL CALLBACK ChangeChildrenFontProc(HWND hwnd, LPARAM lparam)
// {
//   SendMessage(hwnd, WM_SETFONT, (WPARAM)lparam, (LPARAM)TRUE);
//   return TRUE;
// }
