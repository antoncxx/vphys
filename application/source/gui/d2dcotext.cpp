#include "d2dcotext.hpp"

D2Context::D2Context() noexcept 
    : factory(nullptr), renderTarget(nullptr) {
}

D2Context::~D2Context() noexcept {
}

bool D2Context::Initialize(HWND hwnd) noexcept {
    if (hwnd == nullptr) {
        return false;
    }

    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
    if (factory == nullptr) {
        return false;
    }

    factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(10, 10)), &renderTarget);

    if (renderTarget == nullptr) {
        return false;
    }

    renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &colorBrush);

    if (colorBrush == nullptr) {
        return false;
    }

    return true;
}

void D2Context::Finalize() noexcept {
    
    if (colorBrush != nullptr) {
        colorBrush->Release();
    }

    if (renderTarget != nullptr) {
        renderTarget->Release();
    }

    if (factory != nullptr) {
        factory->Release();
    }
}

void D2Context::SetBrushColor(const D2D1::ColorF& color) {
    colorBrush->SetColor(color);
}