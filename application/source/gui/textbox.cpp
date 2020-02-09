#include "textbox.hpp"

Textbox::Textbox(HWND parent, POINT position, SIZE size) noexcept
    : Control(position, size) {

    isCreated = CreateControl(parent);
}

Textbox::~Textbox() noexcept {
    DestroyControl();
}


bool Textbox::CreateControl(HWND parent) noexcept {
    hwnd = CreateWindow(sClassName, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, position.x, position.y, size.cx, size.cy, parent, NULL, reinterpret_cast<HINSTANCE>(GetWindowLong(parent, GWL_HINSTANCE)), NULL);
    return hwnd != NULL;
}

void Textbox::DestroyControl() noexcept {
    Control::DestroyControl();
}