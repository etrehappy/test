#pragma once
#include <string>
#include <functional>

class IInputDataProvider 
{
public:
    virtual ~IInputDataProvider() = default;
    virtual std::string GetString(std::function<void()>) = 0;
    virtual uint16_t GetInt(std::function<void()>, uint16_t) = 0;
};
