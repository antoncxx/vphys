#pragma once

class IRenderer {
public:

    IRenderer() = default;
    virtual ~IRenderer() = default;

    virtual void Begin() = 0;
    virtual void End()  = 0;

    virtual void DrawRectangle(const D2D1_RECT_F& rectangle, D2D1::ColorF color) = 0;

};