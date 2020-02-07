#pragma once

#include "../utils/utils.hpp"

#include <d3d9.h>


class Canvas {
    HWND        hwnd;

    POINT position;
    SIZE  size;

    bool isRegistered{ false };
    bool isCreated{ false };

    static inline const char* sClassName = "CANVAS_CLASS";
    static LRESULT CALLBACK CanvasProc(HWND, UINT, WPARAM, LPARAM);

public:
    Canvas(HWND parent, POINT position, SIZE size) noexcept;
    ~Canvas() noexcept;

    NODISCARD inline bool IsRegistered() const noexcept { return isRegistered; }
    NODISCARD inline bool IsCreated()    const noexcept { return isCreated; }

private:
    NODISCARD bool RegisterCanvasClass() const noexcept;
    NODISCARD bool CreateCanvasWindow(HWND) noexcept;

    void UnregisterCanvasClass() noexcept;
    void DestroyCanvasWindow() noexcept;
};
