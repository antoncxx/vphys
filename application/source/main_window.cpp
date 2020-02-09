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
    this->renderer = new Canvas(hwnd, { 10,10 }, { 200,200 });
    buttonStart = new Button(hwnd, { 300,50 }, { 100,40 }, 145, "Start");
    buttonStart->SetCallback(std::bind(&MainWindow::OnButtonStartPushed, this));

    buttonStop = new Button(hwnd, { 300,150 }, { 100,40 }, 168, "Stop");
    buttonStop->SetCallback(std::bind(&MainWindow::OnButtonStopPushed, this));
}

void MainWindow::DestroyControls() noexcept {
    delete renderer;
    delete buttonStart;
    delete buttonStop;
}

bool MainWindow::CheckControls() const noexcept {

    return renderer != 0;
}

void MainWindow::Update() noexcept {
    renderer->Redraw();
}

void MainWindow::OnButtonStartPushed() {
    MessageBox(0, "Start", 0, 0);
}

void MainWindow::OnButtonStopPushed() {
    MessageBox(0, "Stop", 0, 0);
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

    case WM_COMMAND: {
        return SendMessage(reinterpret_cast<HWND>(lParam), WM_COMMAND, wParam, lParam);
    }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return TRUE;
}
