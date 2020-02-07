#pragma once

#define NODISCARD    [[nodiscard]]
#define FALLTHROUGH  [[fallthrough]]

class NonCopybale {
protected:
    NonCopybale(const NonCopybale&) = delete;
    NonCopybale& operator=(const NonCopybale&) = delete;

    NonCopybale() = default;
    ~NonCopybale() = default;
};

class NonMovable {
protected:
    NonMovable(NonMovable&&) = delete;
    NonMovable& operator=(NonMovable&&) = delete;

    NonMovable() = default;
    ~NonMovable() = default;
};