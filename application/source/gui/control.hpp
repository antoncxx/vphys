#pragma once

#include "../utils/utils.hpp"

#include <Windows.h>
#include <CommCtrl.h>

class Control {
protected:
    HWND        hwnd;
    POINT       position;
    SIZE        size;
    bool        isCreated{ false };
    bool        isRegistered{ false };
public:
    Control(POINT position, SIZE size) noexcept;
    virtual ~Control() = default;

    NODISCARD inline bool IsCreated() const noexcept    { return isCreated; }
    NODISCARD inline bool IsRegistered() const noexcept { return isRegistered; }

protected:
    NODISCARD virtual bool CreateControl(HWND) noexcept = 0;
    virtual void DestroyControl() noexcept;
};