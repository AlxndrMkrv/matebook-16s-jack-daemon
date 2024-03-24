#include <string>

import Posix;

int main()
{
    Pipe in{Pipe::Type::WRITE};
    Pipe out{Pipe::Type::READ};

    Pipe::Connect(in, out);

    in.write("message");

    std::string s;
    out.read(s);

    return s == "message" ? 0 : 1;
}
