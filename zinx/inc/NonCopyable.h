#if !defined(ZINX_NON_COPYABLE_H)
#define ZINX_NON_COPYABLE_H

namespace zinx {

class NonCopyable {
private:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};
} // namespace zinx 

#endif // ZINX_NON_COPYABLE_H
