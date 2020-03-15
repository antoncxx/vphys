#pragma once

#include "../utils/utils.hpp"

#include <d2d1.h>

class D2Context {
    ID2D1Factory          *factory;
    ID2D1HwndRenderTarget *renderTarget;
    ID2D1SolidColorBrush  *colorBrush;
public:
    D2Context() noexcept;
    ~D2Context() noexcept;

    bool Initialize(HWND hwnd) noexcept;
    void Finalize() noexcept;

    void SetBrushColor(const D2D1::ColorF& color);

    inline ID2D1HwndRenderTarget* GetRenderTarget() const noexcept { return renderTarget; }
    inline ID2D1SolidColorBrush* GetBrush() const noexcept { return colorBrush; }
};