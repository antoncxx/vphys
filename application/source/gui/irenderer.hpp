#pragma once

class IRenderer {
public:

    IRenderer() = default;
    virtual ~IRenderer() = default;

    virtual void Redraw() noexcept = 0;
};