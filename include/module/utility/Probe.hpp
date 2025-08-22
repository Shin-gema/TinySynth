#ifndef PROBE_HPP
    #define PROBE_HPP

#include "module/AModule.hpp"
#include <fstream>
#include <vector>
#include <string>

class Probe : public AModule {
public:
    Probe(const std::string& name, const std::string& filePath);
    ~Probe();

    void process(int bufferSize) override;

private:
    void writeData(const std::vector<float>& buffer);

    std::string _name;
    std::ofstream _file;
};

#endif // PROBE_HPP