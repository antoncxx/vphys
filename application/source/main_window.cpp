#include "main_window.hpp"

MainWindow::MainWindow(HINSTANCE handle, int width, int height, const std::string& title) noexcept 
    : width(width), height(height), title(title), handle(handle) {

    isRegistered = RegisterMainClass();
    isCreated    = CreateMainWindow();
}

MainWindow::~MainWindow() noexcept {
    DestroyMainWindow();
    UnregisterMainClass();
}

bool MainWindow::RegisterMainClass() const noexcept {
    WNDCLASSA ws;
    memset(&ws, 0, sizeof(WNDCLASSA));

    if (!GetClassInfoA(handle, sClassName, &ws)) {
        ws.style         = 0;
        ws.hInstance     = handle;
        ws.lpszClassName = sClassName;
        ws.lpfnWndProc   = WindowProc;

        ws.cbClsExtra    = 0;
        ws.cbWndExtra    = sizeof(LONG_PTR);
        ws.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

        ws.hIcon         = NULL;
        ws.hCursor       = NULL;
        ws.lpszMenuName  = NULL;
    }

    return RegisterClass(&ws) != 0;
}

bool MainWindow::CreateMainWindow() noexcept {
    if (IsRegistered()) {
        hwnd = CreateWindow(sClassName, title.c_str(), WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, handle, this);
    }
    return hwnd != NULL;
}

void MainWindow::UnregisterMainClass() noexcept {
    UnregisterClass(sClassName, handle);
    isRegistered = false;
}

void MainWindow::DestroyMainWindow() noexcept {
    if (hwnd) {
        DestroyWindow(hwnd);
        hwnd = NULL;
    }

    isCreated = false;
}

LRESULT CALLBACK MainWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_NCCREATE) {
        MainWindow* window = reinterpret_cast<MainWindow*>(lParam);
        SetWindowLongPtr(hwnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(window));
        return TRUE;
    }

    auto window = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hwnd, GWL_USERDATA));
    if (window == nullptr) { // To prevent WM_SIZE before WM_NCCREATE
        return FALSE; 
    }

    switch (uMsg) {
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return TRUE;
}
