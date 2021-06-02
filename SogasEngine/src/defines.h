#pragma once

// Unsigned int types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

// Signed int types
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

// Floating point types
typedef float f32;
typedef double f64;

#define STATIC_ASSERT static_assert

// Ensure all types are of the correct type
STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 byte.");
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 byte.");
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 byte.");

STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 byte.");
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 byte.");
STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 byte.");

STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 byte.");
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 byte.");

STATIC_ASSERT(sizeof(bool) == 1, "Expected bool to be 8 byte.");

#define BIT(x) (1 << x)

// Pass function X with the event as parameter in arg position _1
#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)

#ifdef SGSEXPORT
#ifdef _MSC_VER
#define SGS __declspec(dllexport)
#endif // _MSC_VER
#else
#ifdef _MSC_VER
#define SGS __declspec(dllimport)
#endif // _MSC_VER

#endif // SGEXPORT