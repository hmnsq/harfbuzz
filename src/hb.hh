/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2011,2012  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#اگر_غیر_تعریف_شدہ HB_HH
#تعریف_کرو HB_HH

#اگر_غیر_تعریف_شدہ HB_NO_PRAGMA_GCC_DIAGNOSTIC
#اگر_تعریف_شدہ _MSC_VER
#ہدایت warning( disable: 4068 ) /* Unknown pragma */
#اگر_ختم
#اگر defined(__GNUC__) || defined(__clang__)
/* Rules:
 *
 * - All pragmas are declared GCC even if they are clang ones.  Otherwise GCC
 *   nags, even though we instruct it to ignore -Wunknown-pragmas. ¯\_(ツ)_/¯
 *
 * - Within each category, keep sorted.
 *
 * - Warnings whose scope can be expanded in future compiler versions shall
 *   be declared as "warning".  Otherwise, either ignored or error.
 */

/* Setup.  Don't sort order within this category. */
#اگر_غیر_تعریف_شدہ HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
#ہدایت GCC diagnostic warning "-Wall"
#ہدایت GCC diagnostic warning "-Wextra"
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#ہدایت GCC diagnostic ignored "-Wpragmas"
#ہدایت GCC diagnostic ignored "-Wunknown-pragmas"
#ہدایت GCC diagnostic ignored "-Wunknown-warning-option"
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
//#pragma GCC diagnostic warning "-Weverything"
#اگر_ختم

/* Error.  Should never happen. */
#اگر_غیر_تعریف_شدہ HB_NO_PRAGMA_GCC_DIAGNOSTIC_ERROR
#ہدایت GCC diagnostic error   "-Wbitwise-instead-of-logical"
#ہدایت GCC diagnostic error   "-Wcast-align"
#ہدایت GCC diagnostic error   "-Wcast-function-type"
#ہدایت GCC diagnostic error   "-Wcast-function-type-strict"
#ہدایت GCC diagnostic error   "-Wconstant-conversion"
#ہدایت GCC diagnostic error   "-Wcomma"
#ہدایت GCC diagnostic error   "-Wdelete-non-virtual-dtor"
#ہدایت GCC diagnostic error   "-Wembedded-directive"
#ہدایت GCC diagnostic error   "-Wextra-semi-stmt"
#ہدایت GCC diagnostic error   "-Wformat-security"
#ہدایت GCC diagnostic error   "-Wimplicit-function-declaration"
#ہدایت GCC diagnostic error   "-Winit-self"
#ہدایت GCC diagnostic error   "-Winjected-class-name"
#ہدایت GCC diagnostic error   "-Wmissing-braces"
#ہدایت GCC diagnostic error   "-Wmissing-declarations"
#ہدایت GCC diagnostic error   "-Wmissing-prototypes"
#ہدایت GCC diagnostic error   "-Wnarrowing"
#ہدایت GCC diagnostic error   "-Wnested-externs"
#ہدایت GCC diagnostic error   "-Wold-style-definition"
#ہدایت GCC diagnostic error   "-Wpointer-arith"
#ہدایت GCC diagnostic error   "-Wredundant-decls"
#ہدایت GCC diagnostic error   "-Wreorder"
#ہدایت GCC diagnostic error   "-Wsign-compare"
#ہدایت GCC diagnostic error   "-Wstrict-flex-arrays"
#ہدایت GCC diagnostic error   "-Wstrict-prototypes"
#ہدایت GCC diagnostic error   "-Wstring-conversion"
#ہدایت GCC diagnostic error   "-Wswitch-enum"
#ہدایت GCC diagnostic error   "-Wtautological-overlap-compare"
#ہدایت GCC diagnostic error   "-Wunneeded-internal-declaration"
#ہدایت GCC diagnostic error   "-Wunused"
#ہدایت GCC diagnostic error   "-Wunused-local-typedefs"
#ہدایت GCC diagnostic error   "-Wunused-value"
#ہدایت GCC diagnostic error   "-Wunused-variable"
#ہدایت GCC diagnostic error   "-Wvla"
#ہدایت GCC diagnostic error   "-Wwrite-strings"
#اگر_ختم

/* Warning.  To be investigated if happens. */
#اگر_غیر_تعریف_شدہ HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
#ہدایت GCC diagnostic warning "-Wbuiltin-macro-redefined"
#ہدایت GCC diagnostic warning "-Wdeprecated"
#ہدایت GCC diagnostic warning "-Wdeprecated-declarations"
#ہدایت GCC diagnostic warning "-Wdisabled-optimization"
#ہدایت GCC diagnostic warning "-Wdouble-promotion"
#ہدایت GCC diagnostic warning "-Wformat=2"
#ہدایت GCC diagnostic warning "-Wformat-signedness"
#ہدایت GCC diagnostic warning "-Wignored-pragma-optimize"
#ہدایت GCC diagnostic warning "-Wlogical-op"
#ہدایت GCC diagnostic warning "-Wmissing-format-attribute"
#ہدایت GCC diagnostic warning "-Wpessimizing-move"
#ہدایت GCC diagnostic warning "-Wundef"
#ہدایت GCC diagnostic warning "-Wunsafe-loop-optimizations"
#ہدایت GCC diagnostic warning "-Wunused-but-set-variable"
#اگر_تعریف_شدہ __clang__
// The following are too buggy on gcc
// https://github.com/harfbuzz/harfbuzz/issues/5589
// https://github.com/harfbuzz/harfbuzz/pull/5367
#ہدایت GCC diagnostic warning "-Wmaybe-uninitialized"
#ہدایت GCC diagnostic warning "-Wuninitialized"
#ورنہ
#ہدایت GCC diagnostic ignored "-Wmaybe-uninitialized"
#ہدایت GCC diagnostic ignored "-Wuninitialized"
#اگر_ختم
#اگر_ختم

/* Ignored currently, but should be fixed at some point. */
#اگر_غیر_تعریف_شدہ HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#ہدایت GCC diagnostic ignored "-Wconversion"			// TODO fix
#ہدایت GCC diagnostic ignored "-Wshadow"			// TODO fix
#ہدایت GCC diagnostic ignored "-Wunused-parameter"		// TODO fix
#اگر defined(__GNUC__) && !defined(__clang__)
#ہدایت GCC diagnostic ignored "-Wunused-result"		// TODO fix
#اگر_ختم
#اگر_ختم

/* Ignored intentionally. */
#اگر_غیر_تعریف_شدہ HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#ہدایت GCC diagnostic ignored "-Warray-bounds" // https://github.com/harfbuzz/harfbuzz/issues/5738
#ہدایت GCC diagnostic ignored "-Wclass-memaccess"
#ہدایت GCC diagnostic ignored "-Wcast-function-type-strict" // https://github.com/harfbuzz/harfbuzz/pull/3859#issuecomment-1295409126
#ہدایت GCC diagnostic ignored "-Wdangling-reference" // https://github.com/harfbuzz/harfbuzz/issues/4043
#ہدایت GCC diagnostic ignored "-Wdangling-pointer" // Trigerred by hb_decycler_node_t().
#ہدایت GCC diagnostic ignored "-Wformat-nonliteral"
#ہدایت GCC diagnostic ignored "-Wformat-zero-length"
#ہدایت GCC diagnostic ignored "-Wmissing-field-initializers"
#ہدایت GCC diagnostic ignored "-Wold-style-cast"
#ہدایت GCC diagnostic ignored "-Wpacked" // Erratic impl in clang
#ہدایت GCC diagnostic ignored "-Wrange-loop-analysis" // https://github.com/harfbuzz/harfbuzz/issues/2834
#ہدایت GCC diagnostic ignored "-Wstrict-aliasing"
#ہدایت GCC diagnostic ignored "-Wtype-limits"
#ہدایت GCC diagnostic ignored "-Wunused-template"
#ہدایت GCC diagnostic ignored "-Wc++11-compat" // only gcc raises it
#اگر_ختم

#اگر_ختم
#اگر_ختم


#شامل "hb-config.hh"


/*
 * Following added based on what AC_USE_SYSTEM_EXTENSIONS adds to
 * config.h.in.  Copied here for the convenience of those embedding
 * HarfBuzz and not using our build system.
 */
/* Enable extensions on AIX 3, Interix.  */
#اگر_غیر_تعریف_شدہ ALL_ماخذ
# define ALL_ماخذ 1
#اگر_ختم
/* Enable GNU extensions on systems that have them.  */
#اگر_غیر_تعریف_شدہ _GNU_SOURCE
# define _GNU_SOURCE 1
#اگر_ختم
/* Enable threading extensions on Solaris.  */
#اگر_غیر_تعریف_شدہ _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#اگر_ختم
/* Enable extensions on HP NonStop.  */
#اگر_غیر_تعریف_شدہ TANDEM_ماخذ
# define TANDEM_ماخذ 1
#اگر_ختم
/* Enable general extensions on Solaris.  */
#اگر_غیر_تعریف_شدہ __EXTENSIONS__
# define __EXTENSIONS__ 1
#اگر_ختم

#اگر defined (_MSC_VER) && defined (HB_DLL_EXPORT)
#تعریف_کرو HB_EXTERN __declspec (dllexport) خارجی
#اگر_ختم

// https://github.com/harfbuzz/harfbuzz/pull/4619
#اگر_غیر_تعریف_شدہ __STDC_FORMAT_MACROS
#تعریف_کرو __STDC_FORMAT_MACROS 1
#اگر_ختم

#شامل "hb.h"
#تعریف_کرو HB_H_اندر
#شامل "hb-ot.h"
#تعریف_کرو HB_OT_H_اندر
#شامل "hb-aat.h"
#تعریف_کرو HB_AAT_H_اندر
#تعریف_کرو HB_SUBSET_H_اندر

#شامل <cassert>
#شامل <cfloat>
#شامل <climits>
#اگر defined(_MSC_VER) && !defined(USE_MATH_متعین_کرتا)
# define USE_MATH_متعین_کرتا
#اگر_ختم
#شامل <cmath>
#شامل <cstdarg>
#شامل <cstddef>
#شامل <cstdio>
#شامل <cstdlib>
#شامل <cstring>

#اگر (defined(_MSC_VER) && _MSC_VER >= 1500) || defined(__MINGW32__)
#اگر_تعریف_شدہ __MINGW32_VERSION
#اگر_غیر_تعریف_شدہ WIN32_LEAN_AND_اوسط
#تعریف_کرو WIN32_LEAN_AND_اوسط 1
#اگر_ختم
#ورنہ
#شامل <intrin.h>
#اگر_ختم
#اگر_ختم

#اگر_تعریف_شدہ _WIN32
#شامل <windows.h>
#شامل <winapifamily.h>
#اگر_ختم

#اگر_غیر_تعریف_شدہ PRId32
# define PRId32 "d"
# define PRIu32 "u"
# define PRIx32 "x"
#اگر_ختم

#تعریف_کرو HB_PASTE1(a,b) a##b
#تعریف_کرو HB_PASTE(a,b) HB_PASTE1(a,b)


/*
 * Compiler attributes
 */

// gcc 10 has __has_builtin but not earlier versions. Sanction any gcc >= 5
// clang defines it so no need.
#اگر_تعریف_شدہ __has_builtin
#تعریف_کرو hb_has_builtin __has_builtin
#وگرنہ_اگر defined(_MSC_VER)
#تعریف_کرو hb_has_builtin(x) 0
#ورنہ
#تعریف_کرو hb_has_builtin(x) ((defined(__GNUC__) && __GNUC__ >= 5))
#اگر_ختم

#اگر defined(__OPTIMIZE__) && hb_has_builtin(__builtin_expect)
#تعریف_کرو likely(اظہاریہ) __builtin_expect (منطقی(اظہاریہ), 1)
#تعریف_کرو unlikely(اظہاریہ) __builtin_expect (منطقی(اظہاریہ), 0)
#ورنہ
#تعریف_کرو likely(اظہاریہ) (اظہاریہ)
#تعریف_کرو unlikely(اظہاریہ) (اظہاریہ)
#اگر_ختم

#اگر !defined(__GNUC__) && !defined(__clang__)
#تعریف_منسوخ __attribute__
#تعریف_کرو __attribute__(x)
#اگر_ختم

#اگر defined(__MINGW32__) && (__GNUC__ >= 3) && !defined(__clang__)
#تعریف_کرو HB_PRINTF_FUNC(format_idx, arg_idx) __attribute__((__format__ (gnu_printf, format_idx, arg_idx)))
#وگرنہ_اگر defined(__GNUC__) && (__GNUC__ >= 3)
#تعریف_کرو HB_PRINTF_FUNC(format_idx, arg_idx) __attribute__((__format__ (__printf__, format_idx, arg_idx)))
#ورنہ
#تعریف_کرو HB_PRINTF_FUNC(format_idx, arg_idx)
#اگر_ختم
#اگر defined(__GNUC__) && (__GNUC__ >= 4) || (__clang__)
#تعریف_کرو HB_غیر_مستعمل	__attribute__((غیر_مستعمل_2))
#وگرنہ_اگر defined(_MSC_VER) /* https://github.com/harfbuzz/harfbuzz/issues/635 */
#تعریف_کرو HB_غیر_مستعمل __pragma(warning(suppress: 4100 4101 4189))
#ورنہ
#تعریف_کرو HB_غیر_مستعمل
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_اندرونی
# اگر !defined(HB_NO_VISIBILITY) && !defined(__MINGW32__) && !defined(__CYGWIN__) && !defined(_MSC_VER) && !defined(__SUNPRO_CC)
#  define HB_اندرونی __attribute__((__visibility__("hidden")))
# elif defined(__MINGW32__)
   /* We use -export-symbols on mingw32, since it does not support visibility attributes. */
#  define HB_اندرونی
# elif defined (_MSC_VER) && defined (HB_DLL_EXPORT)
   /* We do not try to export internal symbols on Visual Studio */
#  define HB_اندرونی
#ورنہ
#  define HB_اندرونی
#  define HB_NO_VISIBILITY 1
# endif
#اگر_ختم

/* https://github.com/harfbuzz/harfbuzz/issues/1651 */
#اگر defined(__clang__) && __clang_major__ < 10
#تعریف_کرو static_const ساکن
#ورنہ
#تعریف_کرو static_const ساکن ثابت
#اگر_ختم

#اگر defined(__GNUC__) && (__GNUC__ >= 3)
#تعریف_کرو HB_FUNC __PRETTY_FUNCTION__
#وگرنہ_اگر defined(_MSC_VER)
#تعریف_کرو HB_FUNC __FUNCSIG__
#ورنہ
#تعریف_کرو HB_FUNC __func__
#اگر_ختم

#اگر defined(__SUNPRO_CC) && (__SUNPRO_CC < 0x5140)
/* https://github.com/harfbuzz/harfbuzz/issues/630 */
#تعریف_کرو __restrict
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_ALWAYS_INLINE
#اگر defined(_MSC_VER)
#تعریف_کرو HB_ALWAYS_INLINE __forceinline
#ورنہ
#تعریف_کرو HB_ALWAYS_INLINE __attribute__((always_inline)) اندرونی
#اگر_ختم
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NEVER_INLINE
#اگر defined(_MSC_VER)
#تعریف_کرو HB_NEVER_INLINE __declspec(noinline)
#ورنہ
#تعریف_کرو HB_NEVER_INLINE __attribute__((noinline))
#اگر_ختم
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_HOT
#تعریف_کرو HB_HOT __attribute__((hot))
#اگر_ختم

/*
 * Borrowed from https://bugzilla.mozilla.org/show_bug.cgi?id=1215411
 * HB_FALLTHROUGH is an annotation to suppress compiler warnings about switch
 * cases that fall through without a break or return statement. HB_FALLTHROUGH
 * is only needed on cases that have code:
 *
 * switch (foo) {
 *   case 1: // These cases have no code. No fallthrough annotations are needed.
 *   case 2:
 *   case 3:
 *     foo = 4; // This case has code, so a fallthrough annotation is needed:
 *     HB_FALLTHROUGH;
 *   default:
 *     return foo;
 * }
 */
#اگر defined(__clang__) && __cplusplus >= 201103L
   /* clang's fallthrough annotations are only available starting in C++11. */
#  define HB_FALLTHROUGH [[clang::fallthrough]]
#وگرنہ_اگر defined(__GNUC__) && (__GNUC__ >= 7)
   /* GNU fallthrough attribute is available from GCC7 */
#  define HB_FALLTHROUGH __attribute__((fallthrough))
#وگرنہ_اگر defined(_MSC_VER)
   /*
    * MSVC's __fallthrough annotations are checked by /analyze (Code Analysis):
    * https://msdn.microsoft.com/en-us/library/ms235402%28VS.80%29.aspx
    */
#  include <sal.h>
#  define HB_FALLTHROUGH __fallthrough
#ورنہ
#  define HB_FALLTHROUGH /* FALLTHROUGH */
#اگر_ختم

/* A tag to enforce use of return value for a function */
#اگر __cplusplus >= 201703L
#  define HB_NODISCARD [[nodiscard]]
#وگرنہ_اگر defined(__GNUC__) || defined(__clang__)
#  define HB_NODISCARD __attribute__((warn_غیر_مستعمل_نتیجہ))
#وگرنہ_اگر defined(_MSC_VER)
#  define HB_NODISCARD _Check_return_
#ورنہ
#  define HB_NODISCARD
#اگر_ختم

/* Like HB_NODISCARD but can be used on structs */
#اگر __cplusplus >= 201703L
#  define HB_NODISCARD_STRUCT [[nodiscard]]
#ورنہ
#  define HB_NODISCARD_STRUCT
#اگر_ختم

#اگر_تعریف_شدہ _WIN32
   /* We need Windows Vista for both Uniscribe backend and for
    * MemoryBarrier.  We don't support compiling on Windows XP,
    * though we run on it fine. */
#  اگر defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0600
#    undef _WIN32_WINNT
#  endif
#  ifndef _WIN32_WINNT
#    اگر WINAPI_FAMILY_تقسیم(WINAPI_تقسیم_DESKTOP)
#      define _WIN32_WINNT 0x0600
#    endif
#  endif
#  ifndef WIN32_LEAN_AND_اوسط
#    define WIN32_LEAN_AND_اوسط 1
#  endif
#  ifndef STRICT
#    define STRICT 1
#  endif

#  اگر defined(_WIN32_WCE)
     /* Some things not defined on Windows CE. */
#    define vsnprintf _vsnprintf
#    ifndef HB_NO_GETENV
#      define HB_NO_GETENV
#    endif
#    اگر _WIN32_WCE < 0x800
#      define HB_NO_SETLOCALE
#      define HB_NO_ERRNO
#    endif
#  elif !WINAPI_FAMILY_تقسیم(WINAPI_تقسیم_DESKTOP)
#    ifndef HB_NO_GETENV
#      define HB_NO_GETENV
#    endif
#  endif
#  اگر defined(_MSC_VER) && _MSC_VER < 1900
#    define snprintf _snprintf
#  endif
#اگر_ختم

#اگر_تعریف_شدہ HB_NO_GETENV
#تعریف_کرو getenv(Name) خالی_اشاریہ
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_ERRNO
#  include <cerrno>
#ورنہ
ساکن صحیح HB_غیر_مستعمل _hb_errno = 0;
#  undef errno
#  define errno _hb_errno
#اگر_ختم

#تعریف_کرو HB_STMT_آغاز کرو
#تعریف_کرو HB_STMT_انتہا   جب (0)

#اگر defined(HAVE_ATEXIT) && !defined(HB_USE_ATEXIT)
/* atexit() is only safe to be called from shared libraries on certain
 * platforms.  Whitelist.
 * https://bugs.freedesktop.org/show_bug.cgi?id=82246 */
#  اگر defined(__linux) && defined(__GLIBC_PREREQ)
#    اگر __GLIBC_PREREQ(2,3)
/* From atexit() manpage, it's safe with glibc 2.2.3 on Linux. */
#      define HB_USE_ATEXIT 1
#    endif
#  elif defined(_MSC_VER) || defined(__MINGW32__)
/* For MSVC:
 * https://msdn.microsoft.com/en-us/library/tze57ck3.aspx
 * https://msdn.microsoft.com/en-us/library/zk17ww08.aspx
 * mingw32 headers say atexit is safe to use in shared libraries.
 */
#    define HB_USE_ATEXIT 1
#  elif defined(__ANDROID__)
/* This is available since Android NKD r8 or r8b:
 * https://issuetracker.google.com/code/p/android/issues/detail?id=6455
 */
#    define HB_USE_ATEXIT 1
#  elif defined(__APPLE__)
/* For macOS and related platforms, the atexit man page indicates
 * that it will be invoked when the library is unloaded, not only
 * at application exit.
 */
#    define HB_USE_ATEXIT 1
#  endif
#اگر_ختم /* defined(HAVE_ATEXIT) && !defined(HB_USE_ATEXIT) */
#اگر_تعریف_شدہ HB_NO_ATEXIT
#  undef HB_USE_ATEXIT
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_USE_ATEXIT
#  define HB_USE_ATEXIT 0
#اگر_ختم
#اگر_غیر_تعریف_شدہ hb_atexit
#اگر !HB_USE_ATEXIT
#  define hb_atexit(_) HB_STMT_آغاز { اگر (0) (_) (); } HB_STMT_انتہا
#ورنہ /* HB_USE_ATEXIT */
#  ifdef HAVE_ATEXIT
#    define hb_atexit atexit
#  ورنہ
     سانچہ <خالی (*function) (خالی)> ساخت hb_atexit_t { ~hb_atexit_t () { function (); } };
#    define hb_atexit(f) ساکن hb_atexit_t<f> _hb_atexit_##__LINE__
#  endif
#اگر_ختم
#اگر_ختم


// Locale business

#اگر !defined(HB_NO_SETLOCALE) && (!defined(HAVE_NEWLOCALE) || !defined(HAVE_USELOCALE))
#تعریف_کرو HB_NO_SETLOCALE 1
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_SETLOCALE

#شامل <locale.h>
#اگر_تعریف_شدہ HAVE_XLOCALE_H
#شامل <xlocale.h> // Needed on BSD/OS X for uselocale
#اگر_ختم

#اگر_تعریف_شدہ WIN32
#تعریف_کرو hb_locale_t _locale_t
#ورنہ
#تعریف_کرو hb_locale_t locale_t
#اگر_ختم
#تعریف_کرو hb_setlocale setlocale
#تعریف_کرو hb_uselocale uselocale

#ورنہ

#تعریف_کرو hb_locale_t خالی *
#تعریف_کرو hb_setlocale(Category, Locale) "C"
#تعریف_کرو hb_uselocale(Locale) ((hb_locale_t) 0)

#اگر_ختم


/* Lets assert int types.  Saves trouble down the road. */
static_assert ((حجم (hb_codepoint_t) == 4), "");
static_assert ((حجم (hb_position_t) == 4), "");
static_assert ((حجم (hb_نقاب_t) == 4), "");
static_assert ((حجم (hb_var_int_t) == 4), "");


/* Pie time. */
// https://github.com/harfbuzz/harfbuzz/issues/4166
#تعریف_کرو HB_PI 3.14159265358979323846f
#تعریف_کرو HB_2_PI (2.f * HB_PI)

/* Compile-time custom allocator support. */

#اگر !defined(HB_CUSTOM_MALLOC) \
  && defined(hb_malloc_impl) \
  && defined(hb_calloc_impl) \
  && defined(hb_realloc_impl) \
  && defined(hb_free_impl)
#تعریف_کرو HB_CUSTOM_MALLOC
#اگر_ختم

#اگر_تعریف_شدہ HB_CUSTOM_MALLOC
خارجی "C" خالی* hb_malloc_impl(size_t سائز);
خارجی "C" خالی* hb_calloc_impl(size_t nmemb, size_t سائز);
خارجی "C" خالی* hb_realloc_impl(خالی *ptr, size_t سائز);
خارجی "C" خالی  hb_free_impl(خالی *ptr);
#ورنہ
#تعریف_کرو hb_malloc_impl حافظہ_دو
#تعریف_کرو hb_calloc_impl صفر_حافظہ_دو
#تعریف_کرو hb_realloc_impl realloc
#تعریف_کرو hb_free_impl آزاد
#اگر_ختم



/* Headers we include for everyone.  Keep topologically sorted by dependency.
 * They express dependency amongst themselves, but no other file should include
 * them directly.*/
#شامل "hb-limits.hh"
#شامل "hb-cplusplus.hh"
#شامل "hb-meta.hh"
#شامل "hb-mutex.hh"
#شامل "hb-number.hh"
#شامل "hb-atomic.hh"	// Requires: hb-meta
#شامل "hb-null.hh"	// Requires: hb-meta
#شامل "hb-algs.hh"	// Requires: hb-meta hb-null hb-number
#شامل "hb-iter.hh"	// Requires: hb-algs hb-meta
#شامل "hb-debug.hh"	// Requires: hb-algs hb-atomic
#شامل "hb-array.hh"	// Requires: hb-algs hb-iter hb-null
#شامل "hb-vector.hh"	// Requires: hb-array hb-null
#شامل "hb-object.hh"	// Requires: hb-atomic hb-mutex hb-vector


/* Library-internal alias for hb::unique_ptr defined in hb-cplusplus.hh.
 * Unique-ownership RAII wrapper around an HB object; destroys on scope exit
 * via the type's hb_*_destroy() function.  Usage: hb_unique_ptr_t<hb_blob_t>.
 * Matches the hb_*_t naming used elsewhere in internal code. */
سانچہ <قسم_نام T> استعمال hb_unique_ptr_t = hb::unique_ptr<T>;


/* Our src/test-*.cc use hb_assert(), such that it's not compiled out under NDEBUG.
 * https://github.com/harfbuzz/harfbuzz/issues/5418 */
#تعریف_کرو hb_always_assert(x) \
	HB_STMT_آغاز { \
	  اگر (!(x)) { fprintf(stderr, "Assertion failed: %s, at %s:%d\n", #x, __FILE__, __LINE__); abort(); } \
	} HB_STMT_انتہا


#اگر_ختم /* HB_HH */
