#pragma once

#include "../utils/utils.hpp"

#include <d2d1.h>

class D2Context {
    ID2D1Factory          *factory;
    ID2D1HwndRenderTarget *renderTarget;
public:
    D2Context() noexcept;
    ~D2Context() noexcept;

    bool Initialize(HWND hwnd) noexcept;
    void Finalize() noexcept;

    inline ID2D1HwndRenderTarget* GetRenderTarget() const noexcept { return renderTarget; }
};