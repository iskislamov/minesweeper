#pragma once

#include "resource.h"
#include <iostream>
#include <time.h>
#include <vector>
#include <Windows.h>

extern size_t height;
extern size_t width;

void StartNewGame();

class OverlappedWindow 
{
public:
    OverlappedWindow();
    ~OverlappedWindow();

    static bool RegisterClass();

    bool Create();
    void Show(int cmdShow);
    void Update();
    void Move();

    HWND getHandle();
    void changeHInstance(HINSTANCE hInst);

protected:
    void OnNCCreate(HWND& handle);
    void OnCreate();
    void OnClose();
    void OnPaint();
    void OnTime();
    void OnLDown();
    void OnLUp(LPARAM lParam);
    void OnRUp(LPARAM lParam);
    void OnDestroy();

private:
    HWND handle_;
    HINSTANCE hInst_;
    HDC hdcBuffer_;
    HBITMAP hbmBuffer_;

    static LRESULT __stdcall windowProc(
        HWND handle,
        UINT message,
        WPARAM wParam,
        LPARAM lParam);
};
