#include "control.hpp"

Control::Control(POINT position, SIZE size) noexcept 
    : position(position), size(size) {

}

void Control::DestroyControl() noexcept {
    if (hwnd) {
        DestroyWindow(hwnd);
        hwnd = NULL;
    }

    isCreated = false;
}