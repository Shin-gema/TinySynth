#ifndef IRESETABLE_HPP
#define IRESETABLE_HPP

class IResetable {
public:
    virtual ~IResetable() = default;
    virtual void reset() = 0;
};

#endif // IRESETABLE_HPP