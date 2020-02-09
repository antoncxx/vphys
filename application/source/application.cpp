#include "application.hpp"

int Application::Run(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow) noexcept {

    UNUSEDPARAM(lpCmdLine);
    UNUSEDPARAM(nCmdShow);

    MainWindow mainWindow(hInstance, 400, 300);

    if (!mainWindow.IsRegistered()) {
        MessageBox(NULL, "Failed to register main window class.", "Error", MB_OK);
        return EXIT_FAILURE;
    }

    if (!mainWindow.IsCreated()) {
        MessageBox(NULL, "Failed to create main window.", "Error", MB_OK);
        return EXIT_FAILURE;
    }

    if (!mainWindow.CheckControls()) {
        MessageBox(NULL, "Failed to create main window's controls.", "Error", MB_OK);
        return EXIT_FAILURE;
    }

    MSG message;
    memset(&message, 0, sizeof(MSG));

    while (true) {
        if (PeekMessage(&message, 0, 0, 0, PM_REMOVE) != 0) {
            if (message.message == WM_QUIT) {
                break;
            }

            TranslateMessage(&message);
            DispatchMessage(&message);
        } else {
            // TODO: update window according to timer
            mainWindow.Update();
        }
    }

    return static_cast<int>(message.wParam);
}