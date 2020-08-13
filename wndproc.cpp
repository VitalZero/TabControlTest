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
    hwndTab = CreateTab(hwnd, 1100, 10, 10, 400, 300, true);

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
      COLORREF selColor;
      HDC hdc = GetDC(hwndTab);

      switch(dis->itemAction)
      {
        case ODA_FOCUS:
        if(dis->itemState & ODS_SELECTED)
        {
          DrawFocusRect(dis->hDC, &dis->rcItem);
        }
          break;

        case ODA_SELECT:
        {          
        }
        case ODA_DRAWENTIRE:
        {
          TEXTMETRIC tm;
          GetTextMetrics(hdc, &tm);
          int y = (dis->rcItem.bottom + dis->rcItem.top - tm.tmHeight) / 2;
          int x = dis->rcItem.left + 5;

          if(dis->itemState & ODS_SELECTED)
          {
            SetTextColor(hdc, GetSysColor(COLOR_HIGHLIGHT));
            x += 2;

            RECT rect = {dis->rcItem.left + 2, dis->rcItem.top, rect.left+2, dis->rcItem.bottom};
            FillRect(hdc, &rect, GetSysColorBrush(COLOR_HIGHLIGHT));
          }
          else
          {
            SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
          }
          
          SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
          
          TextOut(hdc, x, y, L"blah!", 5);
          //ExtTextOut(hdc, x, y, ETO_OPAQUE, &dis->rcItem, L"blah!", 5, 0);
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