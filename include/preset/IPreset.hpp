#ifndef IPRESET_HPP
#define IPRESET_HPP

#include <string>
#include <vector>
#include "core/GraphManager.hpp"

class IPreset {
public:
    virtual ~IPreset() = default;
    virtual void load(bool debug = false) = 0;
    virtual GraphManager& getGraphManager() = 0;

};
#endif // IPRESET_HPP