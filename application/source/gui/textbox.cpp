#include "textbox.hpp"

Textbox::Textbox(HWND parent, POINT position, SIZE size) noexcept
    : Control(position, size) {

    isCreated = CreateControl(parent);
}

Textbox::~Textbox() noexcept {
    DestroyControl();
}


bool Textbox::CreateControl(HWND parent) noexcept {
    hwnd = CreateWindow(sClassName, nullptr, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, position.x, position.y, size.cx, size.cy, parent, nullptr, reinterpret_cast<HINSTANCE>(GetWindowLongPtr(parent, GWLP_HINSTANCE)), nullptr);
    return hwnd != nullptr;
}

void Textbox::DestroyControl() noexcept {
    Control::DestroyControl();
}