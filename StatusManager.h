#pragma once
#include <iostream>

class StatusManager {
private:
    std::string msg;

public:

    StatusManager() {}
    StatusManager(std::string msg) {
        this->msg = msg;
    }
    StatusManager(const StatusManager& statusManager) {
        this->msg = statusManager.msg;
    }
    ~StatusManager() {}
    StatusManager& operator=(const StatusManager& statusManager) {
        if (this != &statusManager) {
            this->msg = statusManager.msg;
        }
        return *this;
    }

    bool operator==(const StatusManager& other) const {
        return this->msg == other.msg;
    }

    friend std::ostream& operator<<(std::ostream& out, const StatusManager& sm) {
        out << sm.msg;
        return out;
    }

    operator std::string() const {
        return this->msg;
    }

    bool operator!() const {
        return msg.empty();
    }

    void operator+=(const std::string& text) {
        this->msg += text;
    }

    enum Status { Note, Success, Error };

    void print(const Status status, const std::string &err) {
        toString(status);
        if (err.empty() == false) {
            msg += err;
        }
        std::cout << msg << std::endl;
    }

    void toString(Status status) {
        switch (status) {
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
};
