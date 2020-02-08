#include "canvas.hpp"

Canvas::Canvas(HWND parent, POINT position, SIZE size) noexcept 
    : Control(position, size) {

    isRegistered = RegisterCanvasClass();
    isCreated    = CreateControl(parent);
}

Canvas::~Canvas() noexcept {
    DestroyControl();
    UnregisterCanvasClass();
}

bool Canvas::RegisterCanvasClass() const noexcept {
    WNDCLASS wc;
    memset(&wc, 0, sizeof(wc));

    if (!GetClassInfo(GetModuleHandle(NULL), sClassName, &wc)) {
        wc.style         = 0;
        wc.hInstance     = GetModuleHandle(NULL);
        wc.lpszClassName = sClassName;
        wc.lpfnWndProc   = CanvasProc;

        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = sizeof(LONG_PTR);
        wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 2);

        wc.hIcon         = NULL;
        wc.hCursor       = NULL;
        wc.lpszMenuName  = NULL;
    }

    return RegisterClass(&wc) != 0;
}

bool Canvas::CreateControl(HWND parent) noexcept {
    if (IsRegistered()) {
        hwnd = CreateWindow(sClassName, NULL, WS_VISIBLE | WS_CHILD, position.x, position.y, size.cx, size.cy, parent, NULL, GetModuleHandle(NULL), this);
    }

    return hwnd != NULL;
}

void Canvas::UnregisterCanvasClass() noexcept {
    UnregisterClass(sClassName, GetModuleHandle(NULL));
    isRegistered = false;
}

void Canvas::DestroyControl() noexcept {
    Control::DestroyControl();
}

LRESULT CALLBACK Canvas::CanvasProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_NCCREATE) {
        Canvas* canvas = reinterpret_cast<Canvas*>(lParam);
        SetWindowLongPtr(hwnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(canvas));
        return TRUE;
    }

    auto canvas = reinterpret_cast<Canvas*>(GetWindowLongPtr(hwnd, GWL_USERDATA));
    if (canvas == nullptr) {
        return FALSE;
    }

    switch (uMsg) {

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return TRUE;
}