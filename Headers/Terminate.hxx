#pragma once

#include <iostream>
#include <string>

void terminate(const std::string & s)
{
    std::cout << ProgramName << " error: " << s << std::endl;
    exit(1);
}
