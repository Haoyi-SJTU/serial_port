#include <windows.h>
#include <string>
#include <vector>

#define ID_COMBO_PORT 101
#define ID_BTN_CONNECT 102
#define ID_BTN_ON 103
#define ID_BTN_OFF 104
#define ID_CHK_TIMER 105
#define ID_EDIT_MS 106
#define ID_SYSLINK_AUTHOR 107
#define ID_TIMER_AUTO_OFF 1

HANDLE hSerial = INVALID_HANDLE_VALUE;
HWND hComboPort, hBtnConnect, hBtnOn, hBtnOff, hChkTimer, hEditMs, hLinkAuthor;
bool isConnected = false;

// 串口指令
BYTE dataOn[] = {0xA0, 0x01, 0x01, 0xA2};
BYTE dataOff[] = {0xA0, 0x01, 0x00, 0xA1};

// 发送数据
void SendSerialData(const BYTE *data, DWORD size)
{
    if (hSerial == INVALID_HANDLE_VALUE)
        return;
    DWORD bytesWritten;
    WriteFile(hSerial, data, size, &bytesWritten, NULL);
    FlushFileBuffers(hSerial);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        CreateWindowA("STATIC", "选择串口:", WS_VISIBLE | WS_CHILD, 20, 20, 80, 20, hwnd, NULL, NULL, NULL);
        hComboPort = CreateWindowA("COMBOBOX", "", CBS_DROPDOWNLIST | WS_VISIBLE | WS_CHILD, 100, 18, 100, 200, hwnd, (HMENU)ID_COMBO_PORT, NULL, NULL);
        for (int i = 1; i <= 10; ++i)
        {
            std::string portName = "COM" + std::to_string(i);
            SendMessageA(hComboPort, CB_ADDSTRING, 0, (LPARAM)portName.c_str());
        }
        SendMessageA(hComboPort, CB_SETCURSEL, 9, 0);

        hBtnConnect = CreateWindowA("BUTTON", "连接", WS_VISIBLE | WS_CHILD, 210, 18, 80, 24, hwnd, (HMENU)ID_BTN_CONNECT, NULL, NULL);

        hBtnOn = CreateWindowA("BUTTON", "开启", WS_VISIBLE | WS_CHILD | WS_DISABLED, 20, 70, 120, 40, hwnd, (HMENU)ID_BTN_ON, NULL, NULL);
        hBtnOff = CreateWindowA("BUTTON", "关闭", WS_VISIBLE | WS_CHILD | WS_DISABLED, 170, 70, 120, 40, hwnd, (HMENU)ID_BTN_OFF, NULL, NULL);

        hChkTimer = CreateWindowA("BUTTON", "定时自动关闭", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 20, 130, 120, 20, hwnd, (HMENU)ID_CHK_TIMER, NULL, NULL);
        CreateWindowA("STATIC", "时长(ms):", WS_VISIBLE | WS_CHILD, 150, 132, 80, 20, hwnd, NULL, NULL, NULL);
        hEditMs = CreateWindowA("EDIT", "1500", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 220, 130, 70, 20, hwnd, (HMENU)ID_EDIT_MS, NULL, NULL);

        hLinkAuthor = CreateWindowExA(0, "STATIC",
                                      "软件作者：Haoyi",
                                      WS_VISIBLE | WS_CHILD | SS_NOTIFY | SS_CENTER,
                                      60, 180, 200, 20, // 居中放置
                                      hwnd, (HMENU)ID_SYSLINK_AUTHOR, NULL, NULL);
        break;
    }

    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        HWND hwndStatic = (HWND)lParam;
        if (hwndStatic == hLinkAuthor)
        {
            SetTextColor(hdcStatic, RGB(0, 0, 255)); // 蓝色字体
            SetBkMode(hdcStatic, TRANSPARENT);       // 背景透明
            return (LRESULT)GetStockObject(NULL_BRUSH);
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    case WM_SETCURSOR:
    {
        if ((HWND)wParam == hLinkAuthor)
        {
            SetCursor(LoadCursor(NULL, IDC_HAND)); // 设置为手型光标
            return TRUE;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        if (wmId == ID_SYSLINK_AUTHOR && HIWORD(wParam) == STN_CLICKED)
        {
            ShellExecuteA(NULL, "open", "https://github.com/Haoyi-SJTU", NULL, NULL, SW_SHOWNORMAL);
        }

        if (wmId == ID_BTN_CONNECT)
        {
            if (!isConnected)
            {
                char portName[20];
                GetWindowTextA(hComboPort, portName, 20);
                std::string fullPort = "\\\\.\\" + std::string(portName);

                hSerial = CreateFileA(fullPort.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                if (hSerial == INVALID_HANDLE_VALUE)
                {
                    MessageBoxA(hwnd, "无法打开串口！请检查是否被占用或未连接。", "错误", MB_ICONERROR);
                    return 0;
                }

                DCB dcb = {0};
                dcb.DCBlength = sizeof(dcb);
                GetCommState(hSerial, &dcb);
                dcb.BaudRate = CBR_9600;
                dcb.ByteSize = 8;
                dcb.StopBits = ONESTOPBIT;
                dcb.Parity = NOPARITY;
                SetCommState(hSerial, &dcb);

                isConnected = true;
                SetWindowTextA(hBtnConnect, "断开");
                EnableWindow(hBtnOn, TRUE);
                EnableWindow(hBtnOff, TRUE);
                EnableWindow(hComboPort, FALSE);
            }
            else
            {
                CloseHandle(hSerial);
                hSerial = INVALID_HANDLE_VALUE;
                isConnected = false;
                SetWindowTextA(hBtnConnect, "连接");
                EnableWindow(hBtnOn, FALSE);
                EnableWindow(hBtnOff, FALSE);
                EnableWindow(hComboPort, TRUE);
            }
        }
        else if (wmId == ID_BTN_ON)
        {
            SendSerialData(dataOn, sizeof(dataOn));
            if (SendMessage(hChkTimer, BM_GETCHECK, 0, 0) == BST_CHECKED)
            {
                char msText[20];
                GetWindowTextA(hEditMs, msText, 20);
                int ms = std::stoi(msText);
                if (ms > 0)
                {
                    SetTimer(hwnd, ID_TIMER_AUTO_OFF, ms, NULL);
                }
            }
        }
        else if (wmId == ID_BTN_OFF)
        {
            SendSerialData(dataOff, sizeof(dataOff));
            KillTimer(hwnd, ID_TIMER_AUTO_OFF);
        }
        break;
    }

    case WM_TIMER:
    {
        if (wParam == ID_TIMER_AUTO_OFF)
        {
            KillTimer(hwnd, ID_TIMER_AUTO_OFF);
            SendSerialData(dataOff, sizeof(dataOff));
        }
        break;
    }

    case WM_DESTROY:
    {
        if (hSerial != INVALID_HANDLE_VALUE)
            CloseHandle(hSerial);
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// 主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const char CLASS_NAME[] = "SerialControlApp";
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HWND hwnd = CreateWindowExA(0, CLASS_NAME, "LCUS-1 USB继电器控制面板", WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
                                CW_USEDEFAULT, CW_USEDEFAULT, 340, 250, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
        return 0;
    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}