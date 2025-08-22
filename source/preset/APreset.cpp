#include "preset/APreset.hpp"

APreset::APreset() 
{
    GraphManager g = GraphManager(512);
    _graphManager = g;
}

void APreset::load(bool debug) 
{
    // Pure virtual function, must be implemented by derived classes
}
