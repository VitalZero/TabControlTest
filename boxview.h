#include "includes.h"
#include <tchar.h>

// boxview window class
static const TCHAR* BOXVIEW_WC = _T("BoxView");

// boxview data
struct BoxviewData
{
  COLORREF c = 0;
  TCHAR* text = nullptr;
};

// boxview messages
static const DWORD BVM_SETCOLOR = 0x0001;
static const DWORD BVM_SETTEXT = 0x0010;

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
  wc.cbWndExtra = sizeof(BoxviewData*);

  RegisterClassEx(&wc);
}

void CustomUnregister()
{
  UnregisterClass(BOXVIEW_WC, nullptr);
}

static void CustomPaint(HWND hwnd)
{
  RECT rc;
  GetClientRect(hwnd, &rc);

  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(hwnd, &ps);

  SetBkMode(hdc, TRANSPARENT);
  TCHAR buffer[MAX_PATH];
  int len = GetWindowTextLength(hwnd);
  GetWindowText(hwnd, buffer, len + 1);
  DrawText(hdc, buffer, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

  EndPaint(hwnd, &ps);
}

HWND CreateBoxView(HWND parent, LPCWSTR text, int x, int y, int width, int height, COLORREF color)
{
  CustomRegister();

  return CreateWindow(
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
  BoxviewData* pData = (BoxviewData*)GetWindowLongPtr(hwnd, 0);

  switch(msg)
  {
    case WM_NCCREATE:
    {
      pData = new BoxviewData;
      if(!pData)
      {
        return FALSE;
      }
      SetWindowLongPtr(hwnd, 0, (LONG_PTR)pData);

      return TRUE;
    }
    break;

    case WM_CREATE:
    {
      return 0;
    }
    break;

    case WM_PAINT:
    {
      CustomPaint(hwnd);
      return 0;
    }

    case WM_NCDESTROY:
      if(pData)
      {
        delete pData;
        pData = nullptr;
      }
      CustomUnregister();
    break;
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}
