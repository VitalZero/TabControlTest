#pragma once
#include "includes.h"
#include <CommCtrl.h>

HWND CreateTab(
  HWND parent,
  int id,
  int x,
  int y,
  int width,
  int height,
  bool ownerDrawn = false);

  int InsertTab(HWND hwnd, int idx, TCITEM* item);