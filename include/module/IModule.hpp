#ifndef IMODULE_HPP
    #define IMODULE_HPP

    #include <vector>

    class IModule {
    public:
        virtual ~IModule() = default;
        virtual void process(int bufferSize) = 0;
    };
#endif // IMODULE_HPP