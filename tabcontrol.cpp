#include "tabcontrol.h"

HWND CreateTab(
  HWND parent,
  int id,
  int x,
  int y,
  int width,
  int height,
  bool ownerDrawn)
{
  INITCOMMONCONTROLSEX icex;
  icex.dwICC = ICC_TAB_CLASSES;
  icex.dwSize = sizeof(icex);
  InitCommonControlsEx(&icex);

  DWORD styles = WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TCS_HOTTRACK | TCS_VERTICAL | TCS_FIXEDWIDTH;
  if(ownerDrawn)
  {
    styles |= TCS_OWNERDRAWFIXED;
  }
  
  return CreateWindowEx(
    WS_EX_CONTROLPARENT,
    WC_TABCONTROL,
    L"",
    styles,
    x,
    y,
    width,
    height,
    parent,
    (HMENU)id,
    GetModuleHandle(nullptr),
    nullptr);
}

int InsertTab(HWND hwnd, int idx, TCITEM* item)
{
  return SendMessage(hwnd, TCM_INSERTITEM, (WPARAM)idx, (LPARAM)item);
}