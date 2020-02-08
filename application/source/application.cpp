#include "application.hpp"

int Application::Run(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow) noexcept {

    MainWindow mainWindow(hInstance, 400, 300);

    if (!mainWindow.IsRegistered()) {
        MessageBox(NULL, "Failed to register main window class.", "Error", MB_OK);
        return EXIT_FAILURE;
    }

    if (!mainWindow.IsCreated()) {
        MessageBox(NULL, "Failed to create main window.", "Error", MB_OK);
        return EXIT_FAILURE;
    }

    MSG message;
    memset(&message, 0, sizeof(MSG));

    while (GetMessage(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return static_cast<int>(message.wParam);
}