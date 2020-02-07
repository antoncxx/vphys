#include "application.hpp"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return Application::Run(hInstance, lpCmdLine, nCmdShow);
}