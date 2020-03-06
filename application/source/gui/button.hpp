#pragma once

#include "../utils/utils.hpp"
#include "control.hpp"

#include <Windows.h>

class Button final : public Control {
    std::string  text;
    std::int64_t id;

    std::function<void(void)> callback{};

    static inline const char* sClassName = "Button";
    static LRESULT CALLBACK ButtonSubclassProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
public:
    Button(HWND parent, POINT position, SIZE size, int id, const std::string& text) noexcept;
    virtual ~Button() noexcept;

    inline void SetCallback(const std::function<void(void)>& callback) noexcept { this->callback = callback; }
    NODISCARD inline std::int64_t GetId() const noexcept { return id; }
private:
    NODISCARD bool CreateControl(HWND) noexcept override;
    void DestroyControl() noexcept override;
    void Invoke() const noexcept;
};