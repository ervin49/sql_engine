#pragma once
#include <iostream>

class StatusManager
{
    private:
        std::string msg;

    public:
        StatusManager();
        StatusManager(std::string msg);
        StatusManager(const StatusManager& statusManager);
        ~StatusManager();

        StatusManager& operator=(const StatusManager& statusManager);
        bool operator==(const StatusManager& other) const;
        friend std::ostream& operator<<(std::ostream& out, const StatusManager& sm);
        operator std::string() const;
        bool operator!() const;
        void operator+=(const std::string& text);

        enum Status { Note, Success, Error, Info };

        void print(const Status status, const std::string& err);

        void toString(Status status);
};
