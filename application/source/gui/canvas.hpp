#pragma once

#include "../utils/utils.hpp"
#include "control.hpp"

#include <d3d9.h>


class Canvas : public Control {

    static inline const char* sClassName = "CANVAS_CLASS";
    static LRESULT CALLBACK CanvasProc(HWND, UINT, WPARAM, LPARAM);

public:
    Canvas(HWND parent, POINT position, SIZE size) noexcept;
    virtual ~Canvas() noexcept;

private:
    NODISCARD bool RegisterCanvasClass() const noexcept;
    NODISCARD bool CreateControl(HWND) noexcept override;

    void UnregisterCanvasClass() noexcept;
    void DestroyControl() noexcept override;
};
