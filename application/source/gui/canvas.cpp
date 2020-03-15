#include "canvas.hpp"

Canvas::Canvas(HWND parent, POINT position, SIZE size) noexcept 
    : Control(position, size) {
    isCreated = CreateControl(parent);
    d2Context.Initialize(hwnd);
}

Canvas::~Canvas() noexcept {
    d2Context.Finalize();
    DestroyControl();
    UnregisterCanvasClass();
}

bool Canvas::RegisterCanvasClass() const noexcept {
    WNDCLASS wc;
    memset(&wc, 0, sizeof(wc));

    if (!GetClassInfo(GetModuleHandle(nullptr), sClassName, &wc)) {
        wc.style         = 0;
        wc.hInstance     = GetModuleHandle(nullptr);
        wc.lpszClassName = sClassName;
        wc.lpfnWndProc   = CanvasProc;

        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = sizeof(LONG_PTR);
        wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

        wc.hIcon         = nullptr;
        wc.hCursor       = nullptr;
        wc.lpszMenuName  = nullptr;
    }

    return RegisterClass(&wc) != 0;
}

bool Canvas::CreateControl(HWND parent) noexcept {
    if (RegisterCanvasClass()) {
        hwnd = CreateWindow(sClassName, nullptr, WS_VISIBLE | WS_CHILD, position.x, position.y, size.cx, size.cy, parent, nullptr, reinterpret_cast<HINSTANCE>(GetWindowLongPtr(parent, GWLP_HINSTANCE)), this);
    }

    return hwnd != nullptr;
}

void Canvas::UnregisterCanvasClass() noexcept {
    UnregisterClass(sClassName, GetModuleHandle(nullptr));
}

void Canvas::DestroyControl() noexcept {
    Control::DestroyControl();
}

void Canvas::Begin() {
    auto renderTarget = d2Context.GetRenderTarget();

    renderTarget->BeginDraw();
    renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Yellow));
}

void Canvas::End() {
    auto renderTarget = d2Context.GetRenderTarget();

    renderTarget->EndDraw();
}

void Canvas::DrawRectangle(const D2D1_RECT_F& rectangle, D2D1::ColorF color) {
    d2Context.SetBrushColor(color);
    d2Context.GetRenderTarget()->FillRectangle(rectangle, d2Context.GetBrush());
}


LRESULT CALLBACK Canvas::CanvasProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_NCCREATE) {
        Canvas* canvas = reinterpret_cast<Canvas*>(lParam);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(canvas));
        return TRUE;
    }

    auto canvas = reinterpret_cast<Canvas*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (canvas == nullptr) {
        return FALSE;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}