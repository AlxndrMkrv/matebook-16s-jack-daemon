#pragma once

#include <exception>
#include <string>

import Posix;

class OutPipeEnd : public Pollable, public Pipe {
public:
    OutPipeEnd() : Pipe(Pipe::Type::READ) {}
    int fd() const override { return static_cast<int>(*this); }

    // Pollable interface implementation
    void onDataReady() override
    {
        std::string s;
        read(s);
        throw std::runtime_error(s);
    }
};
