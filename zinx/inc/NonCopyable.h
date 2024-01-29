#if !defined(ZINX_NON_COPYABLE_H)
#define ZINX_NON_COPYABLE_H

namespace zinx {
namespace base {

class NonCopyable {
protected:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

} // namespace base 
} // namespace zinx 

#endif // ZINX_NON_COPYABLE_H
