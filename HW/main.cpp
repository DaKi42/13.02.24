#include <Windows.h>
#include "resource.h"
#include <string>
using namespace std;

#define IDC_CHECK3 (WM_USER + 1)
#define IDC_BUTTON_START_NEW_GAME (WM_USER + 2)
#define IDC_BUTTON_GAME_FIELD_START (WM_USER + 100)

HINSTANCE hInstance;
HWND hGameButtons[3][3];
bool playerTurn = true; 
bool gameActive = false;
bool gameBoard[3][3] = { {false,false,false}, {false,false,false}, {false,false,false} };


INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void ResetGame(HWND hwnd) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            SetWindowText(hGameButtons[i][j], L""); 
            EnableWindow(hGameButtons[i][j], TRUE); 
            gameBoard[i][j] = false; 
        }
    }
    playerTurn = true;
    gameActive = true; 
}

bool CheckWin() {
    
    for (int i = 0; i < 3; i++) {
        if (gameBoard[i][0] == gameBoard[i][1] && gameBoard[i][1] == gameBoard[i][2] && gameBoard[i][0] != 0 ||
            gameBoard[0][i] == gameBoard[1][i] && gameBoard[1][i] == gameBoard[2][i] && gameBoard[0][i] != 0)
            return true;
    }
    if (gameBoard[0][0] == gameBoard[1][1] && gameBoard[1][1] == gameBoard[2][2] && gameBoard[0][0] != 0 ||
        gameBoard[0][2] == gameBoard[1][1] && gameBoard[1][1] == gameBoard[2][0] && gameBoard[0][2] != 0)
        return true;

    return false;
}

bool CheckDraw() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (gameBoard[i][j] == 0)
                return false;
    return true;
}

void EndGame(HWND hwnd, bool win) {
    if (win) {
        MessageBox(hwnd, playerTurn ? L"Крестики выиграли!" : L"Нолики выиграли!", L"Победа", MB_OK);
    }
    else {
        MessageBox(hwnd, L"Ничья!", L"Игра окончена", MB_OK);
    }
    gameActive = false;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    ::hInstance = hInstance;
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                hGameButtons[i][j] = CreateWindow(L"BUTTON", L"", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    10 + j * 100, 10 + i * 100, 95, 95, hwnd,
                    (HMENU)(IDC_BUTTON_GAME_FIELD_START + i * 3 + j), hInstance, NULL);
            }
        }

        CreateWindow(L"BUTTON", L"Начать новую игру", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            10, 320, 150, 30, hwnd, (HMENU)IDC_BUTTON_START_NEW_GAME, hInstance, NULL);

        return TRUE;
    }

    case WM_COMMAND:
    {
        if (LOWORD(wParam) >= IDC_BUTTON_GAME_FIELD_START && LOWORD(wParam) < IDC_BUTTON_GAME_FIELD_START + 9) {
            if (HIWORD(wParam) == BN_CLICKED && gameActive) {
                HWND hButton = (HWND)lParam;
                int buttonId = LOWORD(wParam) - IDC_BUTTON_GAME_FIELD_START;
                int row = buttonId / 3;
                int col = buttonId % 3;

                if (row >= 0 && row < 3 && col >= 0 && col < 3) {
                    if (gameBoard[row][col] == 0) { 
                        gameBoard[row][col] = playerTurn ? 1 : 2; 

                       
                        HBITMAP hBitmap = LoadBitmap(hInstance, playerTurn ? MAKEINTRESOURCE(IDB_BITMAP_X) : MAKEINTRESOURCE(IDB_BITMAP_O));
                        SendMessage(hButton, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);

                        EnableWindow(hButton, FALSE);

                        if (CheckWin()) {
                            EndGame(hwnd, true); // Обработка победы
                        }
                        else if (CheckDraw()) {
                            EndGame(hwnd, false); 
                        }
                        else {
                            playerTurn = !playerTurn;
                        }
                    }
                }
                else {
                    
                    MessageBox(hwnd, L"Неверные координаты клетки", L"Ошибка", MB_OK | MB_ICONERROR);
                }
            }
        }
        else if (LOWORD(wParam) == IDC_BUTTON_START_NEW_GAME) {
            ResetGame(hwnd);
        }
        break;
    }

    case WM_CLOSE:
        EndDialog(hwnd, 0);
        return TRUE;
    }

    return FALSE;
}