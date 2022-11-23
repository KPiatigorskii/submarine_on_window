#pragma once
#include "controls.h"

void AddMenus(HWND hWnd)
{
    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();

    HMENU hSubMenu = CreateMenu();

    AppendMenu(hSubMenu, MF_STRING, (UINT_PTR)NULL, TEXT("SubMenu"));
    AppendMenu(hFileMenu, MF_STRING, NEW_GAME_WITH_PC, TEXT("New game with PC")); 
    AppendMenu(hFileMenu, MF_STRING, NEW_GAME_PC_VS_PC, TEXT("New game PC vs PC"));
    AppendMenu(hFileMenu, MF_SEPARATOR, (UINT_PTR)NULL, (UINT_PTR)NULL);
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, TEXT("Exit"));


    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, TEXT("File"));
    //AppendMenu(hMenu, MF_STRING, 2, TEXT("Help");
    SetMenu(hWnd, hMenu);
}

void AddGameControls(HWND hWnd)
{
    hGenerateButton = CreateWindowW(TEXT("Button"), TEXT("Generate ships"), WS_VISIBLE | WS_CHILD, 395, 480, 140, 20, hWnd, (HMENU)GENERATE_SHIPS, (UINT_PTR)NULL, (UINT_PTR)NULL);
    hStartGameButton = CreateWindowW(TEXT("Button"), TEXT("Start game"), WS_VISIBLE | WS_CHILD, 655, 480, 140, 20, hWnd, (HMENU)START_GAME, NULL, NULL);
}

void RemoveGameControlsBeforeStart()
{
    DestroyWindow(hGenerateButton);
    DestroyWindow(hStartGameButton);
}

void RemoveGameControls()
{
    DestroyWindow(hGenerateButton);
    DestroyWindow(hStartGameButton);
    DestroyWindow(hStaticLabel);
}