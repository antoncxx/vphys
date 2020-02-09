#pragma once

#include "control.hpp"

class Label final : public Control {
    std::string text;

    static inline const char* sClassName = "Static";
public:
    Label(HWND parent, POINT position, SIZE size, const std::string& text) noexcept;
    virtual ~Label() noexcept;

private:
    NODISCARD bool CreateControl(HWND) noexcept override;
    void DestroyControl() noexcept override;
};