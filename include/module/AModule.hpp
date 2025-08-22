#ifndef AMODULE_HPP
#define AMODULE_HPP

    #include "module/IModule.hpp"
    #include <memory>

    class AModule : public IModule {
    public:
        virtual ~AModule() = default;
        virtual void process(int bufferSize) override = 0;
        virtual void setInput(std::shared_ptr<AModule> inputModule, size_t port = 0) {
            if (_input.size() <= port) {
                _input.resize(port + 1);
            }
            _input[port] = inputModule;
        }

        const std::vector<std::shared_ptr<AModule>>& getInput() const {
            return _input;
        }

        std::vector<float>& getInternalBuffer() {
            return _internalBuffer;
        }

    protected:
        std::vector<std::shared_ptr<AModule>> _input;
        std::vector<float> _internalBuffer;
    };
#endif // AMODULE_HPP
