#pragma once

#include "defines.h"

// Disable assertions by commenting the below line
#define SGSASSERTIONS_ENABLED

#ifdef SGSASSERTIONS_ENABLED
#if _MSC_VER
#define debugBreak() __debugbreak()
#endif // _MSC_VER

SGS void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line);

#define SGSASSERT(expr)                                                \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            report_assertion_failure(#expr, "", __FILE__, __LINE__); \
            debugBreak();                                            \
        }                                                            \
    }

#define SGSASSERT_MSG(expr, message)                                            \
    {                                                                           \
        if (expr) {                                                             \
        }                                                                       \
        else {                                                                  \
            report_assertion_failure(#expr, message, __FILE__, __LINE__);       \
            debugBreak();                                                       \
        }                                                                       \
    }

#ifdef _DEBUG
#define SGSASSERT_DEBUG(expr)                                                   \
{                                                                               \
    if (expr) {}                                                                \
    else {                                                                      \
        report_assertion_failure(#expr, "", __FILE__, __LINE__);                \
        debugBreak();                                                           \
    }                                                                           \
}
#else
#define SGSASSERT_DEBUG(expr) // Does nothing at all
#endif

#else
#define SGSASSERT(expr)
#define SGSASSERT_MSG(expr)
#define SGSASSERT_DEBUG(expr)

#endif // SGSASSERTIONS_ENABLED