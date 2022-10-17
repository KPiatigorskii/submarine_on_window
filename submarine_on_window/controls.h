#define FILE_MENU_NEW 1
#define TEST 2
#define FILE_MENU_EXIT 3
#define NEW_GAME 4
#define NEW_GAME_WITH_PC 5
#define GENERATE_SHIPS 6
#define START_GAME 7

HWND hMenu, hGenerateButton, hStartGameButton;
static  HWND hStaticLabel;

void AddMenus(HWND hWnd)
{
    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();

    HMENU hSubMenu = CreateMenu();

    AppendMenu(hSubMenu, MF_STRING, NULL, L"SubMenu");
    AppendMenu(hFileMenu, MF_STRING, NEW_GAME_WITH_PC, L"New game with PC");
    AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"Exit");


    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    AppendMenu(hMenu, MF_STRING, 2, L"Help");
    SetMenu(hWnd, hMenu);
}

void AddGameControls(HWND hWnd)
{
    hGenerateButton = CreateWindowW(L"Button", L"Generate ships", WS_VISIBLE | WS_CHILD, 200, 480, 120, 20, hWnd, (HMENU)GENERATE_SHIPS, NULL, NULL);
    hStartGameButton = CreateWindowW(L"Button", L"Start game", WS_VISIBLE | WS_CHILD, 400, 480, 120, 20, hWnd, (HMENU)START_GAME, NULL, NULL);

}

void RemoveGameControls() 
{
    DestroyWindow(hGenerateButton);
    DestroyWindow(hStartGameButton); 
    DestroyWindow(hStaticLabel);
}