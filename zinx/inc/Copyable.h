#if !defined(ZINX_COPYABLE_H)
#define ZINX_COPYABLE_H

namespace zinx {

class Copyable {
public:
    Copyable() = default;
    Copyable(const Copyable&) = default;
    Copyable& operator=(const Copyable&) = default;
};

} // namespace zinx 

#endif // ZINX_COPYABLE_H
