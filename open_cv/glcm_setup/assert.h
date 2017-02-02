#ifndef ASSERT_H
#define ASSERT_H

/**
 * A macro to define assertions with dinamic messagies.
 */
#ifndef NDEBUG
#   define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << std::endl; \
            std::cerr << "===================================================" << std::endl; \
            std::cerr << "Assertion `" #condition << std::endl; \
            std::cerr << "` failed in " << __FILE__ << std::endl; \
            std::cerr << " line " << __LINE__ << std::endl; \
            std::cerr << " error description: " << message << std::endl; \
            std::cerr << "===================================================" << std::endl; \
            std::cerr << std::endl; \
            std::terminate(); \
        } \
    } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

#endif // ASSERT_H
