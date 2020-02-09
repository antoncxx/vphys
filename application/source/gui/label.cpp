#include "label.hpp"

Label::Label(HWND parent, POINT position, SIZE size, const std::string& text) noexcept
    : Control(position, size), text(text) {

    isCreated = CreateControl(parent);
}

Label::~Label() noexcept {
    DestroyControl();
}


bool Label::CreateControl(HWND parent) noexcept {
    hwnd = CreateWindow(sClassName, text.c_str(), WS_CHILD | WS_VISIBLE | WS_TABSTOP , position.x, position.y, size.cx, size.cy, parent, NULL, reinterpret_cast<HINSTANCE>(GetWindowLong(parent, GWL_HINSTANCE)), NULL);
    return hwnd != NULL;
}

void Label::DestroyControl() noexcept {
    Control::DestroyControl();
}