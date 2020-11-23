#include "miner.h"

size_t height;
size_t width;

namespace {

const wchar_t g_szClassName[] = L"MinesweeperClass";
const int ID_TIMER = 1;

const size_t MAX_HEIGHT = 16;
const size_t MAX_WIDTH = 30;
const size_t CELLS_PER_MINE = 8;

std::vector<std::vector<char>> field(
    MAX_WIDTH + 1,
    std::vector<char>(MAX_HEIGHT + 1, '?'));
std::vector<std::vector<bool>> mineField(
    MAX_WIDTH + 1,
    std::vector<bool>(
        MAX_HEIGHT + 1,
        false));

std::vector<std::vector<size_t>> openedCells(1000, std::vector<size_t>(2, 0));

enum class FaceState
{
    Smiling,
    Excited,
    Dead
};

FaceState face{ FaceState::Smiling };
int timer = 0;
size_t opened = 0;
size_t lmx, lmy;

bool md = false;
size_t mineNumber = 0;
size_t mineLaid = 0;

HBITMAP mine;

bool IsOpened(size_t x, size_t y) 
{
    for (size_t i = 0; i < opened; ++i) 
    {
        if ((openedCells[i][0] == x) && (openedCells[i][1] == y))
            return true;
    }
    return false;
}

size_t GetAmountMinesAround(size_t x, size_t y) {
    size_t count = 0;

    if (x >= 1 && y >= 1) 
    {
        count += mineField[x - 1][y - 1];
    }
    if (y >= 1) 
    {
        count += mineField[x][y - 1];
    }
    if (x + 1 <= (width - 1) && y >= 1) 
    {
        count += mineField[x + 1][y - 1];
    }
    if (x >= 1) 
    {
        count += mineField[x - 1][y];
    }
    if (x + 1 <= (width - 1)) 
    {
        count += mineField[x + 1][y];
    }
    if (x >= 1 && y + 1 <= (height - 1)) 
    {
        count += mineField[x - 1][y + 1];
    }
    if (y + 1 <= (height - 1)) 
    {
        count += mineField[x][y + 1];
    }
    if (y + 1 <= (height - 1) && x + 1 <= (width - 1)) 
    {
        count += mineField[x + 1][y + 1];
    }
    return count;
}

void OpenCell(size_t x, size_t y) {
    if (IsOpened(x, y))
        return;

    openedCells[opened][0] = x;
    openedCells[opened][1] = y;
    ++opened;

    if (field[x][y] == 'M') 
    {
        --mineLaid;
    }

    size_t minesAroundCount = GetAmountMinesAround(x, y);
    field[x][y] = '0' + minesAroundCount;
    if (minesAroundCount == 0) 
    {
        if ((y >= 1) && 
            (x >= 1) &&
            (y <= height) &&
            (x <= width)) 
        {
            OpenCell(x - 1, y - 1);
        }

        if ((y + 1 >= 0) && 
            (x >= 1) && 
            (y + 1 <= height - 1) &&
            (x <= width)) 
        {
            OpenCell(x - 1, y + 1);
        }

        if ((y >= 1) &&
            (x + 1 >= 0) &&
            (y <= height) &&
            (x + 1 <= width - 1)) 
        {
            OpenCell(x + 1, y - 1);
        }

        if ((y + 1 >= 0) && 
            (x + 1 >= 0) &&
            (y + 1 <= height - 1) &&
            (x + 1 <= width - 1)) 
        {
            OpenCell(x + 1, y + 1);
        }

        if (y >= 1) 
        {
            OpenCell(x, y - 1);
        }

        if (x >= 1) 
        {
            OpenCell(x - 1, y);
        }

        if (x + 1 <= width - 1) 
        {
            OpenCell(x + 1, y);
        }

        if (y + 1 <= height - 1) 
        {
            OpenCell(x, y + 1);
        }
    }
}

void DrawScreen(HDC hdc, RECT* prc)
{
    HDC hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, mine);

    if (face == FaceState::Smiling)
    {
        for (size_t x = 0; x < width; ++x)
        {
            for (size_t y = 0; y < height; ++y)
            {
                if (field[x][y] == '?')
                {
                    BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem, 0, 0, SRCCOPY);
                }
                if (field[x][y] == 'M')
                {
                    BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem, 16, 0, SRCCOPY);
                }
                if ((field[x][y] >= '0') && (field[x][y] <= '9'))
                {
                    BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem,
                        (field[x][y] - '0') * 16, 16, SRCCOPY);
                }
            }
        }
    }

    if (face == FaceState::Dead)
    {
        for (size_t x = 0; x < width; ++x)
        {
            for (size_t y = 0; y < height; ++y)
            {
                if (field[x][y] == '?')
                {
                    BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem, 0, 0, SRCCOPY);
                }

                if ((field[x][y] >= '0') && (field[x][y] <= '9'))
                {
                    BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem,
                        (field[x][y] - '0') * 16, 16, SRCCOPY);
                }

                if (field[x][y] == 'M')
                {
                    BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem, 32, 32, SRCCOPY);
                }

                if (mineField[x][y] == true) {
                    if (field[x][y] == 'M')
                    {
                        BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem, 16, 0, SRCCOPY);
                    }
                    else
                    {
                        BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem, 0, 32, SRCCOPY);
                    }
                }
            }
        }
        BitBlt(hdc, lmx * 16, lmy * 16, 16, 16, hdcMem, 16, 32, SRCCOPY);
    }

    if (face == FaceState::Excited)
    {
        for (size_t x = 0; x < width; ++x)
        {
            for (size_t y = 0; y < height; ++y)
            {
                BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem,
                    GetAmountMinesAround(x, y) * 16, 16, SRCCOPY);

                if (mineField[x][y] == true)
                {
                    BitBlt(hdc, x * 16, y * 16, 16, 16, hdcMem, 16, 0, SRCCOPY);
                }
            }
        }
    }

    BitBlt(hdc, 0, 16 * height, 13, 26, hdcMem,
        ((mineNumber - mineLaid) / 100) * 13, 16 * 3, SRCCOPY);
    BitBlt(hdc, 13, 16 * height, 13, 26, hdcMem,
        ((mineNumber - mineLaid) % 100 / 10) * 13, 16 * 3, SRCCOPY);
    BitBlt(hdc, 26, 16 * height, 13, 26, hdcMem,
        ((mineNumber - mineLaid) % 100 % 10) * 13, 16 * 3, SRCCOPY);
    BitBlt(hdc, 13 * 5 - 2, 16 * height, 13, 26, hdcMem,
        (timer / 100) * 13, 16 * 3, SRCCOPY);
    BitBlt(hdc, 13 * 6 - 2, 16 * height, 13, 26, hdcMem,
        (timer % 100 / 10) * 13, 16 * 3, SRCCOPY);
    BitBlt(hdc, 13 * 7 - 2, 16 * height, 13, 26, hdcMem,
        (timer % 100 % 10) * 13, 16 * 3, SRCCOPY);

    if (face != FaceState::Dead)
    {
        BitBlt(hdc, 13 * 3, 16 * height, 25, 23, hdcMem, 0, 23 + 16 * 3, SRCCOPY);
    }
    else
    {
        BitBlt(hdc, 13 * 3, 16 * height, 25, 23, hdcMem, 25, 23 + 16 * 3, SRCCOPY);
    }
    if (md == true)
    {
        BitBlt(hdc, 13 * 3, 16 * height, 25, 23, hdcMem, 50, 23 + 16 * 3, SRCCOPY);
    }

    DeleteDC(hdcMem);
}

} // namespace

void StartNewGame() {
    srand(int(time(NULL)));
    timer = 0;

    for (size_t y = 0; y < MAX_HEIGHT; ++y) 
    {
        for (size_t x = 0; x < MAX_WIDTH; ++x) 
        {
            field[x][y] = '?';
            mineField[x][y] = false;
        }
    }

    opened = 0;
    mineNumber = 0;
    mineLaid = 0;
    face = FaceState::Smiling;

    for (size_t i = 0; i < (height * width) / CELLS_PER_MINE; i++) 
    {
        size_t x = rand() % width;
        size_t y = rand() % height;

        if (mineField[x][y] != true) 
        {
            ++mineNumber;
        }

        mineField[x][y] = true;
    }
}

OverlappedWindow::OverlappedWindow() 
{
    handle_ = 0;
    HDC hdcBuffer_ = 0;
    HBITMAP hbmBuffer_ = 0;
}

OverlappedWindow::~OverlappedWindow() {}

HWND OverlappedWindow::getHandle() 
{
    return handle_;
}

void OverlappedWindow::changeHInstance(HINSTANCE hInst) 
{
	hInst_ = hInst;
}

void OverlappedWindow::Move() 
{
	MoveWindow(handle_, 0, 0, width * 16 + 20, height * 16 + 65, true);
}

bool OverlappedWindow::Create() 
{
    handle_ = CreateWindowEx(
        WS_EX_CLIENTEDGE, 
        g_szClassName, 
        L"Minesweeper",
        WS_SYSMENU, 
        CW_USEDEFAULT, 
        CW_USEDEFAULT,
        width * 16 + 20, 
        height * 16 + 65, 
        NULL, 
        NULL,
        GetModuleHandle(0), 
        this);
    return handle_ != NULL;
}

void OverlappedWindow::Show(int cmdShow) 
{ 
    ShowWindow(handle_, cmdShow); 
}

void OverlappedWindow::Update() 
{
    UpdateWindow(handle_); 
}

bool OverlappedWindow::RegisterClass() 
{
    WNDCLASSEX window;

    window.cbSize = sizeof(WNDCLASSEX);
    window.style = 0;
    window.lpfnWndProc = windowProc;
    window.cbClsExtra = 0;
    window.cbWndExtra = 0;
    window.hInstance = GetModuleHandle(0);
    window.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    window.hCursor = LoadCursor(NULL, IDC_ARROW);
    window.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    window.lpszMenuName = NULL;
    window.lpszClassName = g_szClassName;
    window.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    return (::RegisterClassEx(&window) != 0);
}

void OverlappedWindow::OnNCCreate(HWND& handle) 
{
	handle_ = handle;
}

void OverlappedWindow::OnCreate() 
{
    hdcBuffer_ = CreateCompatibleDC(GetDC(handle_));
    hbmBuffer_ = CreateCompatibleBitmap(
        GetDC(handle_), 
        16 * 10 + 10, 
        16 * 10 + 20);
    SetTimer(handle_, ID_TIMER, 1000, NULL);
    mine = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_MINE));
}

void OverlappedWindow::OnClose() 
{ 
    DestroyWindow(handle_); 
}

void OverlappedWindow::OnPaint() 
{
    RECT rcClient;
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(handle_, &ps);

    GetClientRect(handle_, &rcClient);
    DrawScreen(hdc, &rcClient);

    EndPaint(handle_, &ps);
}

void OverlappedWindow::OnTime() 
{
    if (face != FaceState::Dead) 
    {
        ++timer;
    }

    InvalidateRect(handle_, NULL, false);
}

void OverlappedWindow::OnLDown() 
{
    if (face == FaceState::Smiling) 
    {
        md = true;
    }
    InvalidateRect(handle_, NULL, false);
}

void OverlappedWindow::OnLUp(LPARAM lParam) 
{
    size_t xPos = LOWORD(lParam) / 16;
    size_t yPos = HIWORD(lParam) / 16;

    if ((face == FaceState::Smiling) &&
        (xPos < width) &&
        (yPos < height)) 
    {
        if (mineField[xPos][yPos] == false) 
        {
            OpenCell(xPos, yPos);
        } 
        else 
        {
            face = FaceState::Dead;
        }

        lmx = xPos;
        lmy = yPos;

        if (face == FaceState::Smiling) 
        {
            bool flag = true;
            for (size_t i = 0; i < width; ++i) 
            {
                for (size_t j = 0; j < height; ++j) 
                {
                    if ((field[i][j] == '?') && (mineField[i][j] == false)) 
                    {
                        flag = false;
                    }
                }
            }

            if (flag == true) 
            {
                face = FaceState::Excited;
            }
        }
    }

  // Mine
  if ((yPos > height - 1) && (xPos > 2) && (xPos < 4)) 
  {
    StartNewGame();
  }

  md = false;
  InvalidateRect(handle_, NULL, true);
}

void OverlappedWindow::OnRUp(LPARAM lParam) 
{
    if (face == FaceState::Smiling) 
    {
        size_t xPos = LOWORD(lParam) / 16;
        size_t yPos = HIWORD(lParam) / 16;

        if ((xPos < width) &&
            (yPos < height) &&
            ((field[xPos][yPos] == '?') 
                ? (mineNumber > mineLaid) : true)) 
        {
            if (field[xPos][yPos] == '?') 
            {
                field[xPos][yPos] = 'M';
                ++mineLaid;
            } 
            else if (field[xPos][yPos] == 'M') 
            {
                field[xPos][yPos] = '?';
                --mineLaid;
            }
        }
    }
    InvalidateRect(handle_, NULL, true);
}

void OverlappedWindow::OnDestroy() 
{
    KillTimer(handle_, ID_TIMER);
    DeleteDC(hdcBuffer_);
    DeleteObject(hbmBuffer_);
    DeleteObject(mine);
}

LRESULT OverlappedWindow::windowProc(
    HWND handle, 
    UINT message, 
    WPARAM wParam,
    LPARAM lParam) 
{
    switch (message) 
    {
        case WM_NCCREATE:
        {
	        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
	        OverlappedWindow* windowPtr = reinterpret_cast<OverlappedWindow*>(pCreate->lpCreateParams);
	        SetWindowLongPtr(handle, GWLP_HINSTANCE, reinterpret_cast<LONG_PTR>(windowPtr));
	        windowPtr->OnNCCreate(handle);
	        return true;
        }
        case WM_CREATE: 
        {
            OverlappedWindow* windowPtr = reinterpret_cast<OverlappedWindow*>(
                GetWindowLongPtr(handle, GWLP_HINSTANCE));
            windowPtr->OnCreate();
            break;
        }
        case WM_CLOSE: 
        {
            OverlappedWindow* windowPtr = reinterpret_cast<OverlappedWindow*>(
                GetWindowLongPtr(handle, GWLP_HINSTANCE));
            windowPtr->OnClose();
            break;
        }
        case WM_PAINT: 
        {
            OverlappedWindow* windowPtr = reinterpret_cast<OverlappedWindow*>(
                GetWindowLongPtr(handle, GWLP_HINSTANCE));
            windowPtr->OnPaint();
            break;
        }
        case WM_TIMER: 
        {
            OverlappedWindow* windowPtr = reinterpret_cast<OverlappedWindow*>(
                GetWindowLongPtr(handle, GWLP_HINSTANCE));
            windowPtr->OnTime();
            break;
        }
        case WM_LBUTTONDOWN: 
        {
            OverlappedWindow* windowPtr = reinterpret_cast<OverlappedWindow*>(
                GetWindowLongPtr(handle, GWLP_HINSTANCE));
            windowPtr->OnLDown();
            break;
        }
        case WM_LBUTTONUP: 
        {
            OverlappedWindow* windowPtr = reinterpret_cast<OverlappedWindow*>(
                GetWindowLongPtr(handle, GWLP_HINSTANCE));
            windowPtr->OnLUp(lParam);
            break;
        }
        case WM_RBUTTONUP: 
        {
            OverlappedWindow* windowPtr = reinterpret_cast<OverlappedWindow*>(
                GetWindowLongPtr(handle, GWLP_HINSTANCE));
            windowPtr->OnRUp(lParam);
            break;
        }
        case WM_DESTROY: 
        {
            OverlappedWindow* windowPtr = reinterpret_cast<OverlappedWindow*>(
                GetWindowLongPtr(handle, GWLP_HINSTANCE));
            windowPtr->OnDestroy();
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(handle, message, wParam, lParam);
    }
    return false;
}
