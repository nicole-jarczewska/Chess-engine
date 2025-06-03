#ifdef _MSC_VER  // MSVC compiler
#include <intrin.h>
#pragma intrinsic(_tzcnt_u64)
#pragma intrinsic(__popcnt64)

inline int ctzll(uint64_t x) {
    return (x == 0) ? 64 : (int)_tzcnt_u64(x);
}

inline int popcountll(uint64_t x) {
    return __popcnt64(x);
}

#else  // GCC/Clang
inline int ctzll(uint64_t x) {
    return __builtin_ctzll(x);
}

inline int popcountll(uint64_t x) {
    return __builtin_popcountll(x);
}
#endif
