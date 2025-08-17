#pragma once

#include <stdexcept>
#include <string>

import Posix;

class OutPipeEnd : public matebook::posix::Pollable,
                   public matebook::posix::Pipe {
public:
    OutPipeEnd() : Pipe(matebook::posix::Pipe::Type::READ) {}
    int fd() const override { return static_cast<int>(*this); }

    // Pollable interface implementation
    void onDataReady() override
    {
        std::string s;
        read(s);
        throw std::runtime_error(s);
    }
};
