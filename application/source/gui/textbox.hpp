#pragma once

#include "control.hpp"

class Textbox final : public Control {
    static inline const char* sClassName = "Edit";
public:
    Textbox(HWND parent, POINT position, SIZE size) noexcept;
    virtual ~Textbox() noexcept;

private:
    NODISCARD bool CreateControl(HWND) noexcept override;
    void DestroyControl() noexcept override;
};