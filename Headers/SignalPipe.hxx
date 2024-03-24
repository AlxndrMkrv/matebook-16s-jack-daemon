#pragma once

#include <exception>

import Posix;

class SignalPipe : public Pollable, public Pipe {
public:
    SignalPipe() : Pipe(Pipe::Type::READ) {}
    int fd() const override { return static_cast<int>(*this); }

    // Pollable interface implementation
    void onDataReady() override { throw std::runtime_error("Signal received"); }
};
