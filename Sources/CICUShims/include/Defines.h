#ifndef Defines_h
#define Defines_h

#ifndef CICUNullable
//#if defined(_Nullable)
#define CICUNullable _Nullable
//#else
//#define CICUNullable
//#endif
#endif

#ifndef CICUNonnull
//#if defined(_Nonnull)
#define CICUNonnull _Nonnull
//#else
//#define CICUNonnull
//#endif
#endif

#ifndef CICUNull_unspecified
//#if defined(_Null_unspecified)
#define CICUNull_unspecified _Null_unspecified
//#else
//#define CICUNull_unspecified
//#endif
#endif

#ifndef CICUNullable_result
//#if defined(_Nullable_result)
#define CICUNullable_result _Nullable_result
//#else
//#define CICUNullable_result
//#endif
#endif

#ifndef cicu_nullable
#if __has_attribute(__nullable__)
#define cicu_nullable __attribute__((__nullable__))
#else
#define cicu_nullable
#endif
#endif

#ifndef cicu_nonnull
#if __has_attribute(__nonnull__)
#define cicu_nonnull __attribute__((__nonnull__))
#else
#define cicu_nonnull
#endif
#endif

#ifndef cicu_returns_nonnull
#if __has_attribute(__returns_nonnull__)
#define cicu_returns_nonnull __attribute__((__returns_nonnull__))
#else
#define cicu_returns_nonnull
#endif
#endif

#ifndef cicu_enum_extensibility
#if __has_attribute(__enum_extensibility__)
#define cicu_enum_extensibility(extensibility) __attribute__((__enum_extensibility__(extensibility)))
#define cicu_flag_enum_extensibility(extensibility) __attribute__((__enum_extensibility__(extensibility),flag_enum))
#else
#define cicu_enum_extensibility(extensibility)
#define cicu_flag_enum_extensibility(extensibility)
#endif
#endif

#ifndef cicu_swift_newtype
#if __has_attribute(__swift_newtype__) && !defined(CICU_NO_SWIFT_ANNOTATIONS)
#define cicu_swift_newtype(type) __attribute__((__swift_newtype__(type)))
#else
#define cicu_swift_newtype(type)
#endif
#endif

#ifndef cicu_swift_name
#if __has_attribute(__swift_name__) && !defined(CICU_NO_SWIFT_ANNOTATIONS)
#define cicu_swift_name(name) __attribute__((__swift_name__(name)))
#else
#define cicu_swift_name(name)
#endif
#endif

#ifndef cicu_packed
#if __has_attribute(packed)
#define cicu_packed __attribute__((packed))
#else
#define cicu_packed
#endif
#endif

#ifndef cicu_static_assert
#ifdef __cplusplus
#define cicu_static_assert static_assert
#else
#define cicu_static_assert _Static_assert
#endif
#endif

#endif /* Defines_h */
