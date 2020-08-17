#include "includes.h"
#include <tchar.h>

// boxview window class
static const TCHAR* BOXVIEW_WC = _T("BoxView");

// boxview data
struct BOXVIEWDATA
{
  HWND hwnd = nullptr;
  COLORREF fgColor = 0;
  COLORREF bgColor = 0;
  HFONT font = nullptr;
};

// boxview messages
static const DWORD BVM_SETBGCOLOR = WM_USER + 1;
static const DWORD BVM_SETTEXT = WM_USER + 2;

//boxview function declarations
void CustomRegister();
void CustomUnregister();
static void CustomPaint(HWND hwnd);
HWND CreateBoxView(HWND parent, LPCWSTR text, int x, int y, int width, int height, COLORREF color);
LRESULT CALLBACK BoxViewWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

//boxview function implementations
void CustomRegister()
{
  WNDCLASSEX wc = {0};
  wc.cbSize = sizeof(wc);
  wc.lpszClassName = BOXVIEW_WC;
  wc.lpfnWndProc = BoxViewWndProc;
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS;
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wc.cbWndExtra = sizeof(BOXVIEWDATA*);

  RegisterClassEx(&wc);
}

void CustomUnregister()
{
  UnregisterClass(BOXVIEW_WC, nullptr);
}

HWND CreateBoxView(HWND parent, LPCWSTR text, int x, int y, int width, int height, COLORREF color)
{
  CustomRegister();

  return CreateWindowEx(
    0,
    BOXVIEW_WC,
    text,
    WS_CHILD | WS_VISIBLE,
    x,
    y, 
    width, 
    height, 
    parent,
    (HMENU)-1,
    GetModuleHandle(nullptr),
    0);
}

LRESULT CALLBACK BoxViewWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  BOXVIEWDATA* pbvd = (BOXVIEWDATA*)GetWindowLongPtr(hwnd, 0);

  switch(msg)
  {
    case WM_NCCREATE:
    {
      pbvd = new BOXVIEWDATA;
      if(!pbvd)
      {
        return FALSE;
      }

      pbvd->hwnd = hwnd;
      pbvd->fgColor = GetSysColor(COLOR_WINDOWTEXT);
      pbvd->bgColor = GetSysColor(COLOR_HIGHLIGHT);
      SetWindowText(hwnd, ((CREATESTRUCT*)lparam)->lpszName);

      SetWindowLongPtr(hwnd, 0, (LONG_PTR)pbvd);

      return TRUE;
    }
    break;

    case WM_CREATE:
    {
      return 1;
    }
    break;

    case WM_PAINT:
    {
      CustomPaint(hwnd);
      return 0;
    }
    
    case BVM_SETBGCOLOR:
    {
      BOXVIEWDATA* pbvd = (BOXVIEWDATA*)GetWindowLongPtr(hwnd, 0);

      if(!pbvd)
      {
        MessageBox(hwnd, L"No se pudo cambiar el color", L"Error", MB_ICONERROR);
        return -1;
      }

      pbvd->bgColor = (COLORREF)wparam;

      InvalidateRect(hwnd, nullptr, FALSE);
      UpdateWindow(hwnd);
      return 0;
    }
      break;

    case WM_NCDESTROY:
      if(pbvd)
      {
        delete pbvd;
        pbvd = nullptr;
      }
      CustomUnregister();
    break;
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

static void CustomPaint(HWND hwnd)
{
  BOXVIEWDATA* pbvd = (BOXVIEWDATA*)GetWindowLongPtr(hwnd, 0);
  RECT rc;
  GetClientRect(hwnd, &rc);

  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(hwnd, &ps);

  SetBkMode(hdc, TRANSPARENT);
  TCHAR buffer[MAX_PATH] = {0};
  int len = GetWindowTextLength(hwnd);
  GetWindowText(hwnd, buffer, len + 1);
  if(pbvd->bgColor != 0)
  {
    HBRUSH bgBrush = CreateSolidBrush(pbvd->bgColor);
    FillRect(hdc, &rc, bgBrush);
    DeleteObject(bgBrush);
  }
  DrawText(hdc, buffer, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);

  EndPaint(hwnd, &ps);
}