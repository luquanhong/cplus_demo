// -------------------------------------------------------------------------------------------
// atomic.h
//
// -------------------------------------------------------------------------------------------
#ifndef __TURBO_ATOMIC_H__
#define __TURBO_ATOMIC_H__

#include <stdint.h>

#ifdef __APPLE__
#include <libkern/OSAtomic.h>
typedef int32_t atomic_int32_t;
#endif

#if defined(__ANDROID__)
#include <android/api-level.h>
#if __ANDROID_API__ > 19
#include <stdatomic.h>
typedef atomic_int_least32_t atomic_int32_t;
#else
#include <sys/atomics.h> // It is in NDK's include, part of bionic (libc)
typedef int32_t atomic_int32_t;
#endif
#endif

namespace turbo {

#undef __USE_GNUC_BUILTIN__

#ifdef __APPLE__

inline void atomicInit32(volatile atomic_int32_t* pVariable, int32_t value)
{
    *pVariable = value;
}

inline int32_t atomicIncrement32(volatile /*int32_t*/ atomic_int32_t* pValue)
{
    return OSAtomicIncrement32Barrier(pValue);
}

inline int32_t atomicDecrement32(volatile /*int32_t*/ atomic_int32_t* pValue)
{
    return OSAtomicDecrement32Barrier(pValue);
}

/**
 *  if (*pValue == oldValue) {
 *     *pValue = newValue;
 *     return true;
 *  }
 *
 *  return false;
 */
inline bool atomicCompareAndSwap32(int32_t oldValue, int32_t newValue, volatile /*int32_t*/ atomic_int32_t* pValue)
{
    return OSAtomicCompareAndSwap32Barrier(oldValue, newValue, pValue);
}

#endif // __APPLE__

#if defined(__ANDROID__)

inline void atomicInit32(volatile atomic_int32_t* pVariable, int32_t value)
{
#if __ANDROID_API_LEVEL__ > 19
    atomic_init(pValue, value);
#else
    *pVariable = value;
#endif
}

inline int32_t atomicIncrement32(volatile /*int32_t*/ atomic_int32_t* pValue)
{
#if __ANDROID_API_LEVEL__ > 19
    return atomic_fetch_add(pValue, 1) + 1;
#else
    return __atomic_inc(pValue) + 1;
#endif
}

inline int32_t atomicDecrement32(volatile /*int32_t*/ atomic_int32_t* pValue)
{
#if __ANDROID_API_LEVEL__ > 19
    return atomic_fetch_sub(pValue, 1) - 1;
#else
    return __atomic_dec(pValue) - 1;
#endif
}

inline bool atomicCompareAndSwap32(int32_t oldValue, int32_t newValue, volatile /*int32_t*/ atomic_int32_t* pValue)
{
#if __ANDROID_API_LEVEL__ > 19
    return atomic_compare_exchange_strong(pValue, &oldValue, newValue);
#else
    // This returns zero if success
    return !__atomic_cmpxchg(oldValue, newValue, pValue);
#endif
}
#endif // __ANDROID__

// gcc fallback
#if defined(__USE_GNUC_BUILTIN__) && defined(__GNUC__)

inline int32_t atomicIncrement32(volatile int32_t* pValue)
{
    return __sync_add_and_fetch(pValue, 1);
}

inline int32_t atomicDecrement32(volatile int32_t* pValue)
{
    return __sync_sub_and_fetch(pValue, 1);
}

inline bool atomicCompareAndSwap32(int32_t oldValue, int32_t newValue, volatile int32_t* pValue)
{
    // This returns zero if success
    return __sync_bool_compare_and_swap(pValue, oldValue, newValue);
}

#endif // __USE_GNUC_BUILTIN__ && __GNUC__

} // namespace

#endif
