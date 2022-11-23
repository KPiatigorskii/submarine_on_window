#pragma once
#include <windows.h>
#include <windowsx.h>

#define FILE_MENU_NEW 1
#define TEST 2
#define FILE_MENU_EXIT 3
#define NEW_GAME 4
#define NEW_GAME_WITH_PC 5
#define NEW_GAME_PC_VS_PC 8
#define GENERATE_SHIPS 6
#define START_GAME 7

HMENU hMenu;
HWND hGenerateButton, hStartGameButton;
static  HWND hStaticLabel;

void AddMenus(HWND hWnd);

void AddGameControls(HWND hWnd);

void RemoveGameControlsBeforeStart();

void RemoveGameControls();