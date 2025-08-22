#ifndef GRAPH_MANAGER_HPP
    #define GRAPH_MANAGER_HPP

    #include "module/AModule.hpp"
    #include <memory>
    #include <vector>

    class GraphManager {
    public:
        GraphManager(size_t bufferSize = 512);

        void addModule(std::shared_ptr<AModule> module);
        void removeModule(std::shared_ptr<AModule> module);

        void connect(std::shared_ptr<AModule> outputModule,
                      std::shared_ptr<AModule> inputModule, size_t destInputPort);

        void processBlock();
        void broadcastReset();
        
        void setNote(int note, float velocity);

        void triggerEnvelope(bool trigger, float velocity);

        const std::vector<float>& getOutputBuffer() const;
    private:
        void updateProcessingOrder();

        size_t _bufferSize;
        std::vector<std::shared_ptr<AModule>> _modules;
        std::vector<AModule*> _processingOrder;

        AModule* _outputModule; // The final output module in the graph
        };

#endif // GRAPH_MANAGER_HPP