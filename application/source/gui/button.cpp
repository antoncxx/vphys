#include "button.hpp"

Button::Button(HWND parent, POINT position, SIZE size) noexcept 
    : Control(position, size) {

    isRegistered = CheckRegistration();
    isCreated    = CreateControl(parent);
}

Button::~Button() noexcept {
    DestroyControl();
}

NODISCARD bool Button::CreateControl(HWND parent) noexcept {
    if (IsRegistered()) {
        hwnd = CreateWindow(sClassName, "okay", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, position.x, position.y, size.cx, size.cy, parent, NULL, (HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE), NULL);
    }

    return hwnd != NULL;
}

void Button::DestroyControl() noexcept {
    Control::DestroyControl();
}

NODISCARD bool Button::CheckRegistration() const noexcept {
    WNDCLASS wc;
    memset(&wc, 0, sizeof(wc));

    return GetClassInfo(GetModuleHandle(NULL), sClassName, &wc) != 0;
}