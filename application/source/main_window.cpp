#include "main_window.hpp"

MainWindow::MainWindow(HINSTANCE handle, int width, int height) noexcept 
    : width(width), height(height), handle(handle) {

    isRegistered = RegisterMainClass();
    isCreated    = CreateMainWindow();

    CreateControls();
}

MainWindow::~MainWindow() noexcept {
    DestroyControls();
    
    DestroyMainWindow();
    UnregisterMainClass();
}

bool MainWindow::RegisterMainClass() const noexcept {
    WNDCLASS wc;
    memset(&wc, 0, sizeof(WNDCLASSA));

    if (!GetClassInfo(handle, sClassName, &wc)) {
        wc.style         = 0;
        wc.hInstance     = handle;
        wc.lpszClassName = sClassName;
        wc.lpfnWndProc   = WindowProc;

        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = sizeof(LONG_PTR);
        wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

        wc.hIcon         = NULL;
        wc.hCursor       = NULL;
        wc.lpszMenuName  = NULL;
    }

    return RegisterClass(&wc) != 0;
}

bool MainWindow::CreateMainWindow() noexcept {
    if (IsRegistered()) {
        hwnd = CreateWindow(sClassName, NULL, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, handle, this);
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

void MainWindow::CreateControls() noexcept {
    this->renderer = new Canvas(hwnd, { 10,10 }, { 100,100 });
    button = new Button(hwnd, { 150,50 }, { 100,40 });
}

void MainWindow::DestroyControls() noexcept {
    delete renderer;
    delete button;
}

bool MainWindow::CheckControls() const noexcept {

    return renderer != 0;
}

void MainWindow::Update() noexcept {
    renderer->Redraw();
}

LRESULT CALLBACK MainWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_NCCREATE) {
        MainWindow* window = reinterpret_cast<MainWindow*>(lParam);
        SetWindowLongPtr(hwnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(window));
        return TRUE;
    }

    auto window = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hwnd, GWL_USERDATA));
    if (window == nullptr) { 
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
