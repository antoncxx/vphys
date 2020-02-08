#pragma once

#include "../utils/utils.hpp"
#include "control.hpp"

#include <Windows.h>

class Button : public Control {
    static inline const char* sClassName = "Button";
public:
    Button(HWND parent, POINT position, SIZE size) noexcept;
    virtual ~Button() noexcept;

private:
    NODISCARD bool CreateControl(HWND) noexcept override;
    void DestroyControl() noexcept override;
};