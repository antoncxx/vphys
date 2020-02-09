#include "button.hpp"

Button::Button(HWND parent, POINT position, SIZE size, int id, const std::string& text) noexcept
    : Control(position, size), id(id), text(text) {

    isCreated = CreateControl(parent);
    SetWindowSubclass(hwnd, &Button::ButtonSubclassProc, id, reinterpret_cast<DWORD_PTR>(this));
}

Button::~Button() noexcept {
    RemoveWindowSubclass(hwnd, &Button::ButtonSubclassProc, id);
    DestroyControl();
}

NODISCARD bool Button::CreateControl(HWND parent) noexcept {
    hwnd = CreateWindow(sClassName, text.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, position.x, position.y, size.cx, size.cy, parent, reinterpret_cast<HMENU>(id), reinterpret_cast<HINSTANCE>(GetWindowLong(parent, GWL_HINSTANCE)), NULL);
    return hwnd != NULL;
}

void Button::DestroyControl() noexcept {
    Control::DestroyControl();
}

void Button::Invoke() const noexcept {
    if (callback) {
        callback();
    }
}

LRESULT CALLBACK Button::ButtonSubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR id, DWORD_PTR refData) {

    auto button = reinterpret_cast<Button*>(refData);

    if (uMsg == WM_COMMAND) {    
        button->Invoke();
        return TRUE;
    }

    return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}
