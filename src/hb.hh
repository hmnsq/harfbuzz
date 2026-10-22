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

#جے_بنا_تعریف HB_HH
#تعریف_کرو HB_HH

#جے_بنا_تعریف HB_NO_PRAGMA_GCC_DIAGNOSTIC
#جے_تعریف_شدہ _MSC_VER
#ہدایت warning( disable: 4068 ) /* Unknown pragma */
#جے_انت
#إذا defined(__GNUC__) || defined(__clang__)
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
#جے_بنا_تعریف HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
#ہدایت GCC diagnostic warning "-Wall"
#ہدایت GCC diagnostic warning "-Wextra"
#جے_انت
#جے_بنا_تعریف HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#ہدایت GCC diagnostic ignored "-Wpragmas"
#ہدایت GCC diagnostic ignored "-Wunknown-pragmas"
#ہدایت GCC diagnostic ignored "-Wunknown-warning-option"
#جے_انت
#جے_بنا_تعریف HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
//#pragma GCC diagnostic warning "-Weverything"
#جے_انت

/* Error.  Should never happen. */
#جے_بنا_تعریف HB_NO_PRAGMA_GCC_DIAGNOSTIC_ERROR
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
#جے_انت

/* Warning.  To be investigated if happens. */
#جے_بنا_تعریف HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
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
#جے_تعریف_شدہ __clang__
// The following are too buggy on gcc
// https://github.com/harfbuzz/harfbuzz/issues/5589
// https://github.com/harfbuzz/harfbuzz/pull/5367
#ہدایت GCC diagnostic warning "-Wmaybe-uninitialized"
#ہدایت GCC diagnostic warning "-Wuninitialized"
#وإلا
#ہدایت GCC diagnostic ignored "-Wmaybe-uninitialized"
#ہدایت GCC diagnostic ignored "-Wuninitialized"
#جے_انت
#جے_انت

/* Ignored currently, but should be fixed at some point. */
#جے_بنا_تعریف HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#ہدایت GCC diagnostic ignored "-Wconversion"			// TODO fix
#ہدایت GCC diagnostic ignored "-Wshadow"			// TODO fix
#ہدایت GCC diagnostic ignored "-Wunused-parameter"		// TODO fix
#إذا defined(__GNUC__) && !defined(__clang__)
#ہدایت GCC diagnostic ignored "-Wunused-result"		// TODO fix
#جے_انت
#جے_انت

/* Ignored intentionally. */
#جے_بنا_تعریف HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
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
#جے_انت

#جے_انت
#جے_انت


#ضمن "hb-config.hh"


/*
 * Following added based on what AC_USE_SYSTEM_EXTENSIONS adds to
 * config.h.in.  Copied here for the convenience of those embedding
 * HarfBuzz and not using our build system.
 */
/* Enable extensions on AIX 3, Interix.  */
#جے_بنا_تعریف ALL_سروت
# define ALL_سروت 1
#جے_انت
/* Enable GNU extensions on systems that have them.  */
#جے_بنا_تعریف _GNU_SOURCE
# define _GNU_SOURCE 1
#جے_انت
/* Enable threading extensions on Solaris.  */
#جے_بنا_تعریف _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#جے_انت
/* Enable extensions on HP NonStop.  */
#جے_بنا_تعریف TANDEM_سروت
# define TANDEM_سروت 1
#جے_انت
/* Enable general extensions on Solaris.  */
#جے_بنا_تعریف __EXTENSIONS__
# define __EXTENSIONS__ 1
#جے_انت

#إذا defined (_MSC_VER) && defined (HB_DLL_EXPORT)
#تعریف_کرو HB_EXTERN __declspec (dllexport) خارجي
#جے_انت

// https://github.com/harfbuzz/harfbuzz/pull/4619
#جے_بنا_تعریف __STDC_FORMAT_MACROS
#تعریف_کرو __STDC_FORMAT_MACROS 1
#جے_انت

#ضمن "hb.h"
#تعریف_کرو HB_H_وچ
#ضمن "hb-ot.h"
#تعریف_کرو HB_OT_H_وچ
#ضمن "hb-aat.h"
#تعریف_کرو HB_AAT_H_وچ
#تعریف_کرو HB_SUBSET_H_وچ

#ضمن <cassert>
#ضمن <cfloat>
#ضمن <climits>
#إذا defined(_MSC_VER) && !defined(USE_MATH_مقرر_کردا)
# define USE_MATH_مقرر_کردا
#جے_انت
#ضمن <cmath>
#ضمن <cstdarg>
#ضمن <cstddef>
#ضمن <cstdio>
#ضمن <cstdlib>
#ضمن <cstring>

#إذا (defined(_MSC_VER) && _MSC_VER >= 1500) || defined(__MINGW32__)
#جے_تعریف_شدہ __MINGW32_VERSION
#جے_بنا_تعریف WIN32_LEAN_AND_اوسط
#تعریف_کرو WIN32_LEAN_AND_اوسط 1
#جے_انت
#وإلا
#ضمن <intrin.h>
#جے_انت
#جے_انت

#جے_تعریف_شدہ _WIN32
#ضمن <windows.h>
#ضمن <winapifamily.h>
#جے_انت

#جے_بنا_تعریف PRId32
# define PRId32 "d"
# define PRIu32 "u"
# define PRIx32 "x"
#جے_انت

#تعریف_کرو HB_PASTE1(a,b) a##b
#تعریف_کرو HB_PASTE(a,b) HB_PASTE1(a,b)


/*
 * Compiler attributes
 */

// gcc 10 has __has_builtin but not earlier versions. Sanction any gcc >= 5
// clang defines it so no need.
#جے_تعریف_شدہ __has_builtin
#تعریف_کرو hb_has_builtin __has_builtin
#وإلاإذا defined(_MSC_VER)
#تعریف_کرو hb_has_builtin(x) 0
#وإلا
#تعریف_کرو hb_has_builtin(x) ((defined(__GNUC__) && __GNUC__ >= 5))
#جے_انت

#إذا defined(__OPTIMIZE__) && hb_has_builtin(__builtin_expect)
#تعریف_کرو likely(اظہاریہ) __builtin_expect (منطقي(اظہاریہ), 1)
#تعریف_کرو unlikely(اظہاریہ) __builtin_expect (منطقي(اظہاریہ), 0)
#وإلا
#تعریف_کرو likely(اظہاریہ) (اظہاریہ)
#تعریف_کرو unlikely(اظہاریہ) (اظہاریہ)
#جے_انت

#إذا !defined(__GNUC__) && !defined(__clang__)
#تعریف_رد __attribute__
#تعریف_کرو __attribute__(x)
#جے_انت

#إذا defined(__MINGW32__) && (__GNUC__ >= 3) && !defined(__clang__)
#تعریف_کرو HB_PRINTF_FUNC(format_idx, arg_idx) __attribute__((__format__ (gnu_printf, format_idx, arg_idx)))
#وإلاإذا defined(__GNUC__) && (__GNUC__ >= 3)
#تعریف_کرو HB_PRINTF_FUNC(format_idx, arg_idx) __attribute__((__format__ (__printf__, format_idx, arg_idx)))
#وإلا
#تعریف_کرو HB_PRINTF_FUNC(format_idx, arg_idx)
#جے_انت
#إذا defined(__GNUC__) && (__GNUC__ >= 4) || (__clang__)
#تعریف_کرو HB_بن_ورتیا	__attribute__((بن_ورتیا_2))
#وإلاإذا defined(_MSC_VER) /* https://github.com/harfbuzz/harfbuzz/issues/635 */
#تعریف_کرو HB_بن_ورتیا __pragma(warning(suppress: 4100 4101 4189))
#وإلا
#تعریف_کرو HB_بن_ورتیا
#جے_انت

#جے_بنا_تعریف HB_اندرونی
# إذا !defined(HB_NO_VISIBILITY) && !defined(__MINGW32__) && !defined(__CYGWIN__) && !defined(_MSC_VER) && !defined(__SUNPRO_CC)
#  define HB_اندرونی __attribute__((__visibility__("hidden")))
# elif defined(__MINGW32__)
   /* We use -export-symbols on mingw32, since it does not support visibility attributes. */
#  define HB_اندرونی
# elif defined (_MSC_VER) && defined (HB_DLL_EXPORT)
   /* We do not try to export internal symbols on Visual Studio */
#  define HB_اندرونی
#وإلا
#  define HB_اندرونی
#  define HB_NO_VISIBILITY 1
# endif
#جے_انت

/* https://github.com/harfbuzz/harfbuzz/issues/1651 */
#إذا defined(__clang__) && __clang_major__ < 10
#تعریف_کرو static_const ساكن
#وإلا
#تعریف_کرو static_const ساكن ثابت
#جے_انت

#إذا defined(__GNUC__) && (__GNUC__ >= 3)
#تعریف_کرو HB_FUNC __PRETTY_FUNCTION__
#وإلاإذا defined(_MSC_VER)
#تعریف_کرو HB_FUNC __FUNCSIG__
#وإلا
#تعریف_کرو HB_FUNC __func__
#جے_انت

#إذا defined(__SUNPRO_CC) && (__SUNPRO_CC < 0x5140)
/* https://github.com/harfbuzz/harfbuzz/issues/630 */
#تعریف_کرو __restrict
#جے_انت

#جے_بنا_تعریف HB_ALWAYS_INLINE
#إذا defined(_MSC_VER)
#تعریف_کرو HB_ALWAYS_INLINE __forceinline
#وإلا
#تعریف_کرو HB_ALWAYS_INLINE __attribute__((always_inline)) مضمن
#جے_انت
#جے_انت

#جے_بنا_تعریف HB_NEVER_INLINE
#إذا defined(_MSC_VER)
#تعریف_کرو HB_NEVER_INLINE __declspec(noinline)
#وإلا
#تعریف_کرو HB_NEVER_INLINE __attribute__((noinline))
#جے_انت
#جے_انت

#جے_بنا_تعریف HB_HOT
#تعریف_کرو HB_HOT __attribute__((hot))
#جے_انت

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
#إذا defined(__clang__) && __cplusplus >= 201103L
   /* clang's fallthrough annotations are only available starting in C++11. */
#  define HB_FALLTHROUGH [[clang::fallthrough]]
#وإلاإذا defined(__GNUC__) && (__GNUC__ >= 7)
   /* GNU fallthrough attribute is available from GCC7 */
#  define HB_FALLTHROUGH __attribute__((fallthrough))
#وإلاإذا defined(_MSC_VER)
   /*
    * MSVC's __fallthrough annotations are checked by /analyze (Code Analysis):
    * https://msdn.microsoft.com/en-us/library/ms235402%28VS.80%29.aspx
    */
#  include <sal.h>
#  define HB_FALLTHROUGH __fallthrough
#وإلا
#  define HB_FALLTHROUGH /* FALLTHROUGH */
#جے_انت

/* A tag to enforce use of return value for a function */
#إذا __cplusplus >= 201703L
#  define HB_NODISCARD [[nodiscard]]
#وإلاإذا defined(__GNUC__) || defined(__clang__)
#  define HB_NODISCARD __attribute__((warn_بن_ورتیا_نتیجہ))
#وإلاإذا defined(_MSC_VER)
#  define HB_NODISCARD _Check_return_
#وإلا
#  define HB_NODISCARD
#جے_انت

/* Like HB_NODISCARD but can be used on structs */
#إذا __cplusplus >= 201703L
#  define HB_NODISCARD_STRUCT [[nodiscard]]
#وإلا
#  define HB_NODISCARD_STRUCT
#جے_انت

#جے_تعریف_شدہ _WIN32
   /* We need Windows Vista for both Uniscribe backend and for
    * MemoryBarrier.  We don't support compiling on Windows XP,
    * though we run on it fine. */
#  إذا defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0600
#    undef _WIN32_WINNT
#  endif
#  ifndef _WIN32_WINNT
#    إذا WINAPI_FAMILY_حصے_کرو(WINAPI_حصے_کرو_DESKTOP)
#      define _WIN32_WINNT 0x0600
#    endif
#  endif
#  ifndef WIN32_LEAN_AND_اوسط
#    define WIN32_LEAN_AND_اوسط 1
#  endif
#  ifndef STRICT
#    define STRICT 1
#  endif

#  إذا defined(_WIN32_WCE)
     /* Some things not defined on Windows CE. */
#    define vsnprintf _vsnprintf
#    ifndef HB_NO_GETENV
#      define HB_NO_GETENV
#    endif
#    إذا _WIN32_WCE < 0x800
#      define HB_NO_SETLOCALE
#      define HB_NO_ERRNO
#    endif
#  elif !WINAPI_FAMILY_حصے_کرو(WINAPI_حصے_کرو_DESKTOP)
#    ifndef HB_NO_GETENV
#      define HB_NO_GETENV
#    endif
#  endif
#  إذا defined(_MSC_VER) && _MSC_VER < 1900
#    define snprintf _snprintf
#  endif
#جے_انت

#جے_تعریف_شدہ HB_NO_GETENV
#تعریف_کرو getenv(Name) خالی_سوچک
#جے_انت

#جے_بنا_تعریف HB_NO_ERRNO
#  include <cerrno>
#وإلا
ساكن صحيح HB_بن_ورتیا _hb_errno = 0;
#  undef errno
#  define errno _hb_errno
#جے_انت

#تعریف_کرو HB_STMT_شروع افعل
#تعریف_کرو HB_STMT_سرا   طالما (0)

#إذا defined(HAVE_ATEXIT) && !defined(HB_USE_ATEXIT)
/* atexit() is only safe to be called from shared libraries on certain
 * platforms.  Whitelist.
 * https://bugs.freedesktop.org/show_bug.cgi?id=82246 */
#  إذا defined(__linux) && defined(__GLIBC_PREREQ)
#    إذا __GLIBC_PREREQ(2,3)
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
#جے_انت /* defined(HAVE_ATEXIT) && !defined(HB_USE_ATEXIT) */
#جے_تعریف_شدہ HB_NO_ATEXIT
#  undef HB_USE_ATEXIT
#جے_انت
#جے_بنا_تعریف HB_USE_ATEXIT
#  define HB_USE_ATEXIT 0
#جے_انت
#جے_بنا_تعریف hb_atexit
#إذا !HB_USE_ATEXIT
#  define hb_atexit(_) HB_STMT_شروع { إذا (0) (_) (); } HB_STMT_سرا
#وإلا /* HB_USE_ATEXIT */
#  ifdef HAVE_ATEXIT
#    define hb_atexit atexit
#  وإلا
     سانچا <فراغ (*function) (فراغ)> بنية hb_atexit_t { ~hb_atexit_t () { function (); } };
#    define hb_atexit(f) ساكن hb_atexit_t<f> _hb_atexit_##__LINE__
#  endif
#جے_انت
#جے_انت


// Locale business

#إذا !defined(HB_NO_SETLOCALE) && (!defined(HAVE_NEWLOCALE) || !defined(HAVE_USELOCALE))
#تعریف_کرو HB_NO_SETLOCALE 1
#جے_انت

#جے_بنا_تعریف HB_NO_SETLOCALE

#ضمن <locale.h>
#جے_تعریف_شدہ HAVE_XLOCALE_H
#ضمن <xlocale.h> // Needed on BSD/OS X for uselocale
#جے_انت

#جے_تعریف_شدہ WIN32
#تعریف_کرو hb_locale_t _locale_t
#وإلا
#تعریف_کرو hb_locale_t locale_t
#جے_انت
#تعریف_کرو hb_setlocale setlocale
#تعریف_کرو hb_uselocale uselocale

#وإلا

#تعریف_کرو hb_locale_t فراغ *
#تعریف_کرو hb_setlocale(Category, Locale) "C"
#تعریف_کرو hb_uselocale(Locale) ((hb_locale_t) 0)

#جے_انت


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

#إذا !defined(HB_CUSTOM_MALLOC) \
  && defined(hb_malloc_impl) \
  && defined(hb_calloc_impl) \
  && defined(hb_realloc_impl) \
  && defined(hb_free_impl)
#تعریف_کرو HB_CUSTOM_MALLOC
#جے_انت

#جے_تعریف_شدہ HB_CUSTOM_MALLOC
خارجي "C" فراغ* hb_malloc_impl(size_t ناپ);
خارجي "C" فراغ* hb_calloc_impl(size_t nmemb, size_t ناپ);
خارجي "C" فراغ* hb_realloc_impl(فراغ *ptr, size_t ناپ);
خارجي "C" فراغ  hb_free_impl(فراغ *ptr);
#وإلا
#تعریف_کرو hb_malloc_impl حافظہ_دیو
#تعریف_کرو hb_calloc_impl صفر_حافظہ_دیو
#تعریف_کرو hb_realloc_impl realloc
#تعریف_کرو hb_free_impl حرر
#جے_انت



/* Headers we include for everyone.  Keep topologically sorted by dependency.
 * They express dependency amongst themselves, but no other file should include
 * them directly.*/
#ضمن "hb-limits.hh"
#ضمن "hb-cplusplus.hh"
#ضمن "hb-meta.hh"
#ضمن "hb-mutex.hh"
#ضمن "hb-number.hh"
#ضمن "hb-atomic.hh"	// Requires: hb-meta
#ضمن "hb-null.hh"	// Requires: hb-meta
#ضمن "hb-algs.hh"	// Requires: hb-meta hb-null hb-number
#ضمن "hb-iter.hh"	// Requires: hb-algs hb-meta
#ضمن "hb-debug.hh"	// Requires: hb-algs hb-atomic
#ضمن "hb-array.hh"	// Requires: hb-algs hb-iter hb-null
#ضمن "hb-vector.hh"	// Requires: hb-array hb-null
#ضمن "hb-object.hh"	// Requires: hb-atomic hb-mutex hb-vector


/* Library-internal alias for hb::unique_ptr defined in hb-cplusplus.hh.
 * Unique-ownership RAII wrapper around an HB object; destroys on scope exit
 * via the type's hb_*_destroy() function.  Usage: hb_unique_ptr_t<hb_blob_t>.
 * Matches the hb_*_t naming used elsewhere in internal code. */
سانچا <قسم_ناں T> ورتوں hb_unique_ptr_t = hb::unique_ptr<T>;


/* Our src/test-*.cc use hb_assert(), such that it's not compiled out under NDEBUG.
 * https://github.com/harfbuzz/harfbuzz/issues/5418 */
#تعریف_کرو hb_always_assert(x) \
	HB_STMT_شروع { \
	  إذا (!(x)) { fprintf(stderr, "Assertion failed: %s, at %s:%d\n", #x, __FILE__, __LINE__); abort(); } \
	} HB_STMT_سرا


#جے_انت /* HB_HH */
