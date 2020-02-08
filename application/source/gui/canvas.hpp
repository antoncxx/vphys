#pragma once

#include "../utils/utils.hpp"
#include "control.hpp"
#include "d2dcotext.hpp"
#include "irenderer.hpp"

class Canvas : public Control, public IRenderer {
    D2Context d2Context;

    static inline const char* sClassName = "CANVAS_CLASS";
    static LRESULT CALLBACK CanvasProc(HWND, UINT, WPARAM, LPARAM);

public:
    Canvas(HWND parent, POINT position, SIZE size) noexcept;
    virtual ~Canvas() noexcept;

    void Redraw() noexcept override;

private:
    NODISCARD bool RegisterCanvasClass() const noexcept;
    NODISCARD bool CreateControl(HWND) noexcept override;

    void UnregisterCanvasClass() noexcept;
    void DestroyControl() noexcept override;
};
