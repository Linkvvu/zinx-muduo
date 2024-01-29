#if !defined(ZINX_COPYABLE_H)
#define ZINX_COPYABLE_H

namespace zinx {
namespace base {

class Copyable {
public:
    Copyable() = default;
    Copyable(const Copyable&) = default;
    Copyable& operator=(const Copyable&) = default;
};

} // namespace base 
} // namespace zinx 

#endif // ZINX_COPYABLE_H
