#include "core/GraphManager.hpp"
#include <stdexcept>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include "core/IResetable.hpp"

#include "module/generator/Oscillator.hpp"
#include "module/utility/ADSRModule.hpp"


GraphManager::GraphManager(size_t bufferSize)
    : _bufferSize(bufferSize) {
}

void GraphManager::addModule(std::shared_ptr<AModule> module) {
    if (!module) {
        std::cerr << "Error: Attempted to add a null module." << std::endl;
    }
    module->getInternalBuffer().resize(_bufferSize, 0.0f);
    _modules.push_back(module);
}

void GraphManager::removeModule(std::shared_ptr<AModule> module) {
    auto it = std::remove(_modules.begin(), _modules.end(), module);
    if (it != _modules.end()) {
        _modules.erase(it, _modules.end());
    } else {
        std::cerr << "Error: Attempted to remove a module that does not exist." << std::endl;
    }
}

void GraphManager::connect(std::shared_ptr<AModule> outputModule,
                           std::shared_ptr<AModule> inputModule, size_t destInputPort) {
    if (!outputModule || !inputModule) {
        std::cerr << "Error: Attempted to connect null modules." << std::endl;
        return;
    }
    outputModule->setInput(inputModule, destInputPort);
    updateProcessingOrder();
}

void GraphManager::processBlock() {
    if (_processingOrder.empty()) {
        std::cerr << "Error: No modules to process." << std::endl;
        return;
    }

    _outputModule = _processingOrder.back();
    _outputModule->getInternalBuffer().resize(_bufferSize, 0.0f);

    for (auto& module : _processingOrder) {
        module->process(_bufferSize);
    }
}

const std::vector<float>& GraphManager::getOutputBuffer() const {
    if (!_outputModule) {
        std::cerr << "Error: No output module set." << std::endl;
    }
    return _outputModule->getInternalBuffer();
}

void GraphManager::broadcastReset() {
    for (const auto& module : _modules) {
        if (auto resetable = std::dynamic_pointer_cast<IResetable>(module)) {
            resetable->reset();
        }
    }
    std::cout << "All resettable modules have been reset." << std::endl;
}

// tri topology
void GraphManager::updateProcessingOrder() {
    _processingOrder.clear();
    if (_modules.empty()) {
        std::cerr << "Error: No modules available to process." << std::endl;
        return;
    }
    std::unordered_map<AModule*, int> inDegree;
    std::unordered_map<AModule*, std::vector<AModule*>> adjacencyList;

    for (const auto& module : _modules) {
        AModule* modPtr = module.get();
        inDegree[modPtr] = 0;
        adjacencyList[modPtr] = {};
    }

    for (const auto& module : _modules) {
        AModule* modPtr = module.get();
        for (const auto& input : modPtr->getInput()) {
            if (input) {
                AModule* inputPtr = input.get();
                adjacencyList[inputPtr].push_back(modPtr);
                inDegree[modPtr]++;
            }
        }
    }

    std::vector<AModule*> queue;
    for (const auto& module : _modules) {
        AModule* modPtr = module.get();
        if (inDegree[modPtr] == 0) {
            queue.push_back(modPtr);
        }
    }

    while (!queue.empty()) {
        AModule* current = queue.front();
        queue.erase(queue.begin());
        _processingOrder.push_back(current);

        for (AModule* neighbor : adjacencyList[current]) {
            inDegree[neighbor]--;
            if (inDegree[neighbor] == 0) {
                queue.push_back(neighbor);
            }
        }
    }

    if (_processingOrder.size() != _modules.size()) {
        std::cerr << "Error: Cycle detected in the module graph." << std::endl;
        _processingOrder.clear();
        return;
    }

    std::cout << "Processing order updated. Total modules: " << _processingOrder.size() << std::endl;

}

void GraphManager::setNote(int note, float velocity) {
    for (const auto& module : _modules) {
        if (auto oscillator = std::dynamic_pointer_cast<Oscillator>(module)) {
            std::cout << "Setting note " << note << " with velocity " << velocity << " on Oscillator." << std::endl;
            oscillator->setNote(note);
        }
    }
}

void GraphManager::triggerEnvelope(bool trigger, float velocity) {
    for (const auto& module : _modules) {
        if (auto adsrModule = std::dynamic_pointer_cast<ADSRModule>(module)) {
            std::cout << "Triggering ADSR envelope with velocity: " << static_cast<int>(velocity) << std::endl;
            adsrModule->gate(trigger, velocity);
        }
    }
}