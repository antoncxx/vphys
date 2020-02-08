#pragma once

#include "utils/utils.hpp"
#include "gui/canvas.hpp"
#include "gui/button.hpp"

#include <windows.h>
#include <windowsx.h>

#include <string>

class MainWindow : public NonCopybale, public NonMovable { 
    HINSTANCE   handle;
    HWND        hwnd;
    int         width;
    int         height;

    bool isRegistered{ false };
    bool isCreated{ false };

    Control* canvas;
    Control* button;

    static inline const char* sClassName = "MAIN_WINDOW_CLASS";
    static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
public:
    explicit MainWindow(HINSTANCE handle, int width, int height) noexcept;
    ~MainWindow() noexcept;

    NODISCARD inline bool IsRegistered() const noexcept { return isRegistered; }
    NODISCARD inline bool IsCreated()    const noexcept { return isCreated; }
   
    NODISCARD inline int Width()  const noexcept { return width; }
    NODISCARD inline int Height() const noexcept { return height; }
private:
    NODISCARD bool RegisterMainClass() const noexcept;
    NODISCARD bool CreateMainWindow() noexcept;

    void UnregisterMainClass() noexcept;
    void DestroyMainWindow() noexcept;

    void CreateControls() noexcept;
    void DestroyControls() noexcept;
};