#include "wndproc.h"
#include "tabcontrol.h"

HWND hwndTab = nullptr;

LRESULT CALLBACK WindowProc(
  HWND   hwnd,
  UINT   msg,
  WPARAM wparam,
  LPARAM lparam)
{
  MEASUREITEMSTRUCT* mis;

  switch(msg)
  {
  case WM_CREATE:
  {
    hwndTab = CreateTab(hwnd, 1100, 10, 10, 400, 300);

    TCITEM item;

    item.mask = TCIF_IMAGE | TCIF_TEXT;
    item.iImage = -1;

    wchar_t tabName[MAX_PATH] = L"blah!";
    item.pszText = tabName;
    InsertTab(hwndTab, 0, &item);
    wcscpy(tabName, L"blah!");
    InsertTab(hwndTab, 1, &item);


    int w = 25;
    int h = 100;    
    SendMessage(hwndTab, TCM_SETITEMSIZE, 0, (LPARAM)MAKELPARAM(w, h));

    return 1;
  }
    break;

  case WM_DRAWITEM:
  {
    DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*)lparam;
    if(dis->itemID != -1)
    {
      switch(dis->itemAction)
      {
        case ODA_FOCUS:
        if(dis->itemState & ODS_SELECTED)
        {
          DrawFocusRect(dis->hDC, &dis->rcItem);
        }
          break;

        case ODA_SELECT:
        case ODA_DRAWENTIRE:
        {
          HDC hdc = GetDC(hwndTab);
          TEXTMETRIC tm;
          GetTextMetrics(hdc, &tm);
          int y = (dis->rcItem.bottom + dis->rcItem.top - tm.tmHeight) / 2;
          SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
          ExtTextOut(hdc, dis->rcItem.left + 10, y, ETO_OPAQUE, &dis->rcItem, L"blah!", 5, 0);
          ReleaseDC(hwndTab, hdc);
        }
        break;
      }
    }
    return TRUE;
  }
    break;

  case WM_CTLCOLORSTATIC:
    break;

  case WM_PAINT:
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
    break;

  default:
    return DefWindowProc(hwnd, msg, wparam, lparam);
  }

  return 0;
}