#ifndef APRESET_HPP
#define APRESET_HPP

#include "preset/IPreset.hpp"
#include "core/GraphManager.hpp"

class APreset : public IPreset {
public:
    APreset();
    virtual ~APreset() = default;
    void load(bool debug = false) override = 0;
    GraphManager& getGraphManager() override {
        return _graphManager;
    }
protected:
    GraphManager _graphManager;
};

#endif // APRESET_HPP