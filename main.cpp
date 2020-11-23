#include <Miner/miner.h>

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int cmdShow)
{
    if (!OverlappedWindow::RegisterClass())
    {
        MessageBox(
            NULL,
            L"Window Registration Failed!",
            L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    OverlappedWindow window = OverlappedWindow();
    window.changeHInstance(hInstance);

    if (!window.Create())
    {
        MessageBox(
            NULL,
            L"Window Creation Failed!",
            L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    window.Show(cmdShow);
    window.Update();

    MSG message;

    int temp = MessageBox(
        window.getHandle(),
        L"Would you like to play Easy Mode?",
        L"Init",
        MB_YESNO);

    if (temp == IDYES) 
    {
        height = 9;
        width = 9;
    }
    else {
        temp = MessageBox(
            window.getHandle(), 
            L"Would you like to play Medium Mode?",
            L"Init", 
            MB_YESNO);

        if (temp == IDYES) 
        {
            height = 16;
            width = 16;
        }
        else {
            temp = MessageBox(
                window.getHandle(), 
                L"Here Hard Mode goes!", 
                L"Init",
                MB_OK);
            height = 16;
            width = 30;
        }
    }

    window.Move();
    StartNewGame();

    while (GetMessage(&message, NULL, 0, 0) > 0) 
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return message.wParam;
}
