#pragma once

#define NODISCARD    [[nodiscard]]
#define FALLTHROUGH  [[fallthrough]]

#define UNUSEDPARAM(P) (void)(P)

class NonCopyable {
protected:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

    NonCopyable() = default;
    ~NonCopyable() = default;
};

class NonMovable {
protected:
    NonMovable(NonMovable&&) = delete;
    NonMovable& operator=(NonMovable&&) = delete;

    NonMovable() = default;
    ~NonMovable() = default;
};