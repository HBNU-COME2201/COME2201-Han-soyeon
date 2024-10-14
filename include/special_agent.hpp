#pragma once

#include "agent.hpp"

#include <sstream>

class CSpecialAgent: public CAgent
{
public:
    CSpecialAgent(double x, double y, double heading, 
                 double speed, double drange)
    :CAgent(x, y, heading, speed, drange)
    {

    }

public:
    std::string to_string() const
    {
        std::stringstream os;
        os << "[Special Agent]";
        os << CAgent::to_string();
        return os.str();
    }
};