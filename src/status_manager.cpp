#include <iostream>
#include "status_manager.hpp"

StatusManager::StatusManager() = default;

StatusManager::StatusManager(std::string msg)
{
    this->msg = msg;
}

StatusManager::StatusManager(const StatusManager& statusManager)
{
    this->msg = statusManager.msg;
}

StatusManager::~StatusManager(){}

StatusManager& StatusManager::operator=(const StatusManager& statusManager)
{
    if (this != &statusManager)
    {
        this->msg = statusManager.msg;
    }
    return *this;
}

bool StatusManager::operator==(const StatusManager& other) const
{
    return this->msg == other.msg;
}

std::ostream& operator<<(std::ostream& out, const StatusManager& sm)
{
    out << sm.msg;
    return out;
}

StatusManager::operator std::string() const
{
    return this->msg;
}

bool StatusManager::operator!() const
{
    return msg.empty();
}

void StatusManager::operator+=(const std::string& text)
{
    this->msg += text;
}

void StatusManager::print(const Status status, const std::string& err)
{
    toString(status);
    if (err.empty() == false)
    {
        msg += err;
    }
    std::cout << msg << std::endl;
}

void StatusManager::toString(Status status)
{
    switch (status)
    {
    case(Note):
        msg = "Note: ";
        break;
    case(Success):
        msg = "Success: ";
        break;
    case(Error):
        msg = "Error: ";
        break;
    default:
        break;
    }
}
