#pragma once

#include"main_window.hpp"

class Application {
public:
    NODISCARD static int Run(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow) noexcept;
};