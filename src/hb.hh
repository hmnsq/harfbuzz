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

#اگر_تعریف_نشده HB_HH
#تعریف_کن HB_HH

#اگر_تعریف_نشده HB_NO_PRAGMA_GCC_DIAGNOSTIC
#اگر_تعریف_شده _MSC_VER
#رهنمود warning( disable: 4068 ) /* Unknown pragma */
#پایان_اگر
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
#اگر_تعریف_نشده HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
#رهنمود GCC diagnostic warning "-Wall"
#رهنمود GCC diagnostic warning "-Wextra"
#پایان_اگر
#اگر_تعریف_نشده HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#رهنمود GCC diagnostic ignored "-Wpragmas"
#رهنمود GCC diagnostic ignored "-Wunknown-pragmas"
#رهنمود GCC diagnostic ignored "-Wunknown-warning-option"
#پایان_اگر
#اگر_تعریف_نشده HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
//#pragma GCC diagnostic warning "-Weverything"
#پایان_اگر

/* Error.  Should never happen. */
#اگر_تعریف_نشده HB_NO_PRAGMA_GCC_DIAGNOSTIC_ERROR
#رهنمود GCC diagnostic error   "-Wbitwise-instead-of-logical"
#رهنمود GCC diagnostic error   "-Wcast-align"
#رهنمود GCC diagnostic error   "-Wcast-function-type"
#رهنمود GCC diagnostic error   "-Wcast-function-type-strict"
#رهنمود GCC diagnostic error   "-Wconstant-conversion"
#رهنمود GCC diagnostic error   "-Wcomma"
#رهنمود GCC diagnostic error   "-Wdelete-non-virtual-dtor"
#رهنمود GCC diagnostic error   "-Wembedded-directive"
#رهنمود GCC diagnostic error   "-Wextra-semi-stmt"
#رهنمود GCC diagnostic error   "-Wformat-security"
#رهنمود GCC diagnostic error   "-Wimplicit-function-declaration"
#رهنمود GCC diagnostic error   "-Winit-self"
#رهنمود GCC diagnostic error   "-Winjected-class-name"
#رهنمود GCC diagnostic error   "-Wmissing-braces"
#رهنمود GCC diagnostic error   "-Wmissing-declarations"
#رهنمود GCC diagnostic error   "-Wmissing-prototypes"
#رهنمود GCC diagnostic error   "-Wnarrowing"
#رهنمود GCC diagnostic error   "-Wnested-externs"
#رهنمود GCC diagnostic error   "-Wold-style-definition"
#رهنمود GCC diagnostic error   "-Wpointer-arith"
#رهنمود GCC diagnostic error   "-Wredundant-decls"
#رهنمود GCC diagnostic error   "-Wreorder"
#رهنمود GCC diagnostic error   "-Wsign-compare"
#رهنمود GCC diagnostic error   "-Wstrict-flex-arrays"
#رهنمود GCC diagnostic error   "-Wstrict-prototypes"
#رهنمود GCC diagnostic error   "-Wstring-conversion"
#رهنمود GCC diagnostic error   "-Wswitch-enum"
#رهنمود GCC diagnostic error   "-Wtautological-overlap-compare"
#رهنمود GCC diagnostic error   "-Wunneeded-internal-declaration"
#رهنمود GCC diagnostic error   "-Wunused"
#رهنمود GCC diagnostic error   "-Wunused-local-typedefs"
#رهنمود GCC diagnostic error   "-Wunused-value"
#رهنمود GCC diagnostic error   "-Wunused-variable"
#رهنمود GCC diagnostic error   "-Wvla"
#رهنمود GCC diagnostic error   "-Wwrite-strings"
#پایان_اگر

/* Warning.  To be investigated if happens. */
#اگر_تعریف_نشده HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
#رهنمود GCC diagnostic warning "-Wbuiltin-macro-redefined"
#رهنمود GCC diagnostic warning "-Wdeprecated"
#رهنمود GCC diagnostic warning "-Wdeprecated-declarations"
#رهنمود GCC diagnostic warning "-Wdisabled-optimization"
#رهنمود GCC diagnostic warning "-Wdouble-promotion"
#رهنمود GCC diagnostic warning "-Wformat=2"
#رهنمود GCC diagnostic warning "-Wformat-signedness"
#رهنمود GCC diagnostic warning "-Wignored-pragma-optimize"
#رهنمود GCC diagnostic warning "-Wlogical-op"
#رهنمود GCC diagnostic warning "-Wmissing-format-attribute"
#رهنمود GCC diagnostic warning "-Wpessimizing-move"
#رهنمود GCC diagnostic warning "-Wundef"
#رهنمود GCC diagnostic warning "-Wunsafe-loop-optimizations"
#رهنمود GCC diagnostic warning "-Wunused-but-set-variable"
#اگر_تعریف_شده __clang__
// The following are too buggy on gcc
// https://github.com/harfbuzz/harfbuzz/issues/5589
// https://github.com/harfbuzz/harfbuzz/pull/5367
#رهنمود GCC diagnostic warning "-Wmaybe-uninitialized"
#رهنمود GCC diagnostic warning "-Wuninitialized"
#وإلا
#رهنمود GCC diagnostic ignored "-Wmaybe-uninitialized"
#رهنمود GCC diagnostic ignored "-Wuninitialized"
#پایان_اگر
#پایان_اگر

/* Ignored currently, but should be fixed at some point. */
#اگر_تعریف_نشده HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#رهنمود GCC diagnostic ignored "-Wconversion"			// TODO fix
#رهنمود GCC diagnostic ignored "-Wshadow"			// TODO fix
#رهنمود GCC diagnostic ignored "-Wunused-parameter"		// TODO fix
#إذا defined(__GNUC__) && !defined(__clang__)
#رهنمود GCC diagnostic ignored "-Wunused-result"		// TODO fix
#پایان_اگر
#پایان_اگر

/* Ignored intentionally. */
#اگر_تعریف_نشده HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#رهنمود GCC diagnostic ignored "-Warray-bounds" // https://github.com/harfbuzz/harfbuzz/issues/5738
#رهنمود GCC diagnostic ignored "-Wclass-memaccess"
#رهنمود GCC diagnostic ignored "-Wcast-function-type-strict" // https://github.com/harfbuzz/harfbuzz/pull/3859#issuecomment-1295409126
#رهنمود GCC diagnostic ignored "-Wdangling-reference" // https://github.com/harfbuzz/harfbuzz/issues/4043
#رهنمود GCC diagnostic ignored "-Wdangling-pointer" // Trigerred by hb_decycler_node_t().
#رهنمود GCC diagnostic ignored "-Wformat-nonliteral"
#رهنمود GCC diagnostic ignored "-Wformat-zero-length"
#رهنمود GCC diagnostic ignored "-Wmissing-field-initializers"
#رهنمود GCC diagnostic ignored "-Wold-style-cast"
#رهنمود GCC diagnostic ignored "-Wpacked" // Erratic impl in clang
#رهنمود GCC diagnostic ignored "-Wrange-loop-analysis" // https://github.com/harfbuzz/harfbuzz/issues/2834
#رهنمود GCC diagnostic ignored "-Wstrict-aliasing"
#رهنمود GCC diagnostic ignored "-Wtype-limits"
#رهنمود GCC diagnostic ignored "-Wunused-template"
#رهنمود GCC diagnostic ignored "-Wc++11-compat" // only gcc raises it
#پایان_اگر

#پایان_اگر
#پایان_اگر


#ضمن "hb-config.hh"


/*
 * Following added based on what AC_USE_SYSTEM_EXTENSIONS adds to
 * config.h.in.  Copied here for the convenience of those embedding
 * HarfBuzz and not using our build system.
 */
/* Enable extensions on AIX 3, Interix.  */
#اگر_تعریف_نشده ALL_منبع
# define ALL_منبع 1
#پایان_اگر
/* Enable GNU extensions on systems that have them.  */
#اگر_تعریف_نشده _GNU_SOURCE
# define _GNU_SOURCE 1
#پایان_اگر
/* Enable threading extensions on Solaris.  */
#اگر_تعریف_نشده _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#پایان_اگر
/* Enable extensions on HP NonStop.  */
#اگر_تعریف_نشده TANDEM_منبع
# define TANDEM_منبع 1
#پایان_اگر
/* Enable general extensions on Solaris.  */
#اگر_تعریف_نشده __EXTENSIONS__
# define __EXTENSIONS__ 1
#پایان_اگر

#إذا defined (_MSC_VER) && defined (HB_DLL_EXPORT)
#تعریف_کن HB_EXTERN __declspec (dllexport) خارجي
#پایان_اگر

// https://github.com/harfbuzz/harfbuzz/pull/4619
#اگر_تعریف_نشده __STDC_FORMAT_MACROS
#تعریف_کن __STDC_FORMAT_MACROS 1
#پایان_اگر

#ضمن "hb.h"
#تعریف_کن HB_H_درون
#ضمن "hb-ot.h"
#تعریف_کن HB_OT_H_درون
#ضمن "hb-aat.h"
#تعریف_کن HB_AAT_H_درون
#تعریف_کن HB_SUBSET_H_درون

#ضمن <cassert>
#ضمن <cfloat>
#ضمن <climits>
#إذا defined(_MSC_VER) && !defined(USE_MATH_تعریف_میکند)
# define USE_MATH_تعریف_میکند
#پایان_اگر
#ضمن <cmath>
#ضمن <cstdarg>
#ضمن <cstddef>
#ضمن <cstdio>
#ضمن <cstdlib>
#ضمن <cstring>

#إذا (defined(_MSC_VER) && _MSC_VER >= 1500) || defined(__MINGW32__)
#اگر_تعریف_شده __MINGW32_VERSION
#اگر_تعریف_نشده WIN32_LEAN_AND_میانگین
#تعریف_کن WIN32_LEAN_AND_میانگین 1
#پایان_اگر
#وإلا
#ضمن <intrin.h>
#پایان_اگر
#پایان_اگر

#اگر_تعریف_شده _WIN32
#ضمن <windows.h>
#ضمن <winapifamily.h>
#پایان_اگر

#اگر_تعریف_نشده PRId32
# define PRId32 "d"
# define PRIu32 "u"
# define PRIx32 "x"
#پایان_اگر

#تعریف_کن HB_PASTE1(a,b) a##b
#تعریف_کن HB_PASTE(a,b) HB_PASTE1(a,b)


/*
 * Compiler attributes
 */

// gcc 10 has __has_builtin but not earlier versions. Sanction any gcc >= 5
// clang defines it so no need.
#اگر_تعریف_شده __has_builtin
#تعریف_کن hb_has_builtin __has_builtin
#وإلاإذا defined(_MSC_VER)
#تعریف_کن hb_has_builtin(x) 0
#وإلا
#تعریف_کن hb_has_builtin(x) ((defined(__GNUC__) && __GNUC__ >= 5))
#پایان_اگر

#إذا defined(__OPTIMIZE__) && hb_has_builtin(__builtin_expect)
#تعریف_کن likely(عبارت) __builtin_expect (منطقي(عبارت), 1)
#تعریف_کن unlikely(عبارت) __builtin_expect (منطقي(عبارت), 0)
#وإلا
#تعریف_کن likely(عبارت) (عبارت)
#تعریف_کن unlikely(عبارت) (عبارت)
#پایان_اگر

#إذا !defined(__GNUC__) && !defined(__clang__)
#لغو_تعریف __attribute__
#تعریف_کن __attribute__(x)
#پایان_اگر

#إذا defined(__MINGW32__) && (__GNUC__ >= 3) && !defined(__clang__)
#تعریف_کن HB_PRINTF_FUNC(format_idx, arg_idx) __attribute__((__format__ (gnu_printf, format_idx, arg_idx)))
#وإلاإذا defined(__GNUC__) && (__GNUC__ >= 3)
#تعریف_کن HB_PRINTF_FUNC(format_idx, arg_idx) __attribute__((__format__ (__printf__, format_idx, arg_idx)))
#وإلا
#تعریف_کن HB_PRINTF_FUNC(format_idx, arg_idx)
#پایان_اگر
#إذا defined(__GNUC__) && (__GNUC__ >= 4) || (__clang__)
#تعریف_کن HB_استفادهنشده	__attribute__((استفادهنشده_2))
#وإلاإذا defined(_MSC_VER) /* https://github.com/harfbuzz/harfbuzz/issues/635 */
#تعریف_کن HB_استفادهنشده __pragma(warning(suppress: 4100 4101 4189))
#وإلا
#تعریف_کن HB_استفادهنشده
#پایان_اگر

#اگر_تعریف_نشده HB_داخلی
# إذا !defined(HB_NO_VISIBILITY) && !defined(__MINGW32__) && !defined(__CYGWIN__) && !defined(_MSC_VER) && !defined(__SUNPRO_CC)
#  define HB_داخلی __attribute__((__visibility__("hidden")))
# elif defined(__MINGW32__)
   /* We use -export-symbols on mingw32, since it does not support visibility attributes. */
#  define HB_داخلی
# elif defined (_MSC_VER) && defined (HB_DLL_EXPORT)
   /* We do not try to export internal symbols on Visual Studio */
#  define HB_داخلی
#وإلا
#  define HB_داخلی
#  define HB_NO_VISIBILITY 1
# endif
#پایان_اگر

/* https://github.com/harfbuzz/harfbuzz/issues/1651 */
#إذا defined(__clang__) && __clang_major__ < 10
#تعریف_کن static_const ساكن
#وإلا
#تعریف_کن static_const ساكن ثابت
#پایان_اگر

#إذا defined(__GNUC__) && (__GNUC__ >= 3)
#تعریف_کن HB_FUNC __PRETTY_FUNCTION__
#وإلاإذا defined(_MSC_VER)
#تعریف_کن HB_FUNC __FUNCSIG__
#وإلا
#تعریف_کن HB_FUNC __func__
#پایان_اگر

#إذا defined(__SUNPRO_CC) && (__SUNPRO_CC < 0x5140)
/* https://github.com/harfbuzz/harfbuzz/issues/630 */
#تعریف_کن __restrict
#پایان_اگر

#اگر_تعریف_نشده HB_ALWAYS_INLINE
#إذا defined(_MSC_VER)
#تعریف_کن HB_ALWAYS_INLINE __forceinline
#وإلا
#تعریف_کن HB_ALWAYS_INLINE __attribute__((always_inline)) مضمن
#پایان_اگر
#پایان_اگر

#اگر_تعریف_نشده HB_NEVER_INLINE
#إذا defined(_MSC_VER)
#تعریف_کن HB_NEVER_INLINE __declspec(noinline)
#وإلا
#تعریف_کن HB_NEVER_INLINE __attribute__((noinline))
#پایان_اگر
#پایان_اگر

#اگر_تعریف_نشده HB_HOT
#تعریف_کن HB_HOT __attribute__((hot))
#پایان_اگر

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
#پایان_اگر

/* A tag to enforce use of return value for a function */
#إذا __cplusplus >= 201703L
#  define HB_NODISCARD [[nodiscard]]
#وإلاإذا defined(__GNUC__) || defined(__clang__)
#  define HB_NODISCARD __attribute__((warn_استفادهنشده_نتیجه))
#وإلاإذا defined(_MSC_VER)
#  define HB_NODISCARD _Check_return_
#وإلا
#  define HB_NODISCARD
#پایان_اگر

/* Like HB_NODISCARD but can be used on structs */
#إذا __cplusplus >= 201703L
#  define HB_NODISCARD_STRUCT [[nodiscard]]
#وإلا
#  define HB_NODISCARD_STRUCT
#پایان_اگر

#اگر_تعریف_شده _WIN32
   /* We need Windows Vista for both Uniscribe backend and for
    * MemoryBarrier.  We don't support compiling on Windows XP,
    * though we run on it fine. */
#  إذا defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0600
#    undef _WIN32_WINNT
#  endif
#  ifndef _WIN32_WINNT
#    إذا WINAPI_FAMILY_افراز(WINAPI_افراز_DESKTOP)
#      define _WIN32_WINNT 0x0600
#    endif
#  endif
#  ifndef WIN32_LEAN_AND_میانگین
#    define WIN32_LEAN_AND_میانگین 1
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
#  elif !WINAPI_FAMILY_افراز(WINAPI_افراز_DESKTOP)
#    ifndef HB_NO_GETENV
#      define HB_NO_GETENV
#    endif
#  endif
#  إذا defined(_MSC_VER) && _MSC_VER < 1900
#    define snprintf _snprintf
#  endif
#پایان_اگر

#اگر_تعریف_شده HB_NO_GETENV
#تعریف_کن getenv(Name) اشاره_تهی
#پایان_اگر

#اگر_تعریف_نشده HB_NO_ERRNO
#  include <cerrno>
#وإلا
ساكن صحيح HB_استفادهنشده _hb_errno = 0;
#  undef errno
#  define errno _hb_errno
#پایان_اگر

#تعریف_کن HB_STMT_شروع افعل
#تعریف_کن HB_STMT_انتها   طالما (0)

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
#پایان_اگر /* defined(HAVE_ATEXIT) && !defined(HB_USE_ATEXIT) */
#اگر_تعریف_شده HB_NO_ATEXIT
#  undef HB_USE_ATEXIT
#پایان_اگر
#اگر_تعریف_نشده HB_USE_ATEXIT
#  define HB_USE_ATEXIT 0
#پایان_اگر
#اگر_تعریف_نشده hb_atexit
#إذا !HB_USE_ATEXIT
#  define hb_atexit(_) HB_STMT_شروع { إذا (0) (_) (); } HB_STMT_انتها
#وإلا /* HB_USE_ATEXIT */
#  ifdef HAVE_ATEXIT
#    define hb_atexit atexit
#  وإلا
     قالب <فراغ (*function) (فراغ)> بنية hb_atexit_t { ~hb_atexit_t () { function (); } };
#    define hb_atexit(f) ساكن hb_atexit_t<f> _hb_atexit_##__LINE__
#  endif
#پایان_اگر
#پایان_اگر


// Locale business

#إذا !defined(HB_NO_SETLOCALE) && (!defined(HAVE_NEWLOCALE) || !defined(HAVE_USELOCALE))
#تعریف_کن HB_NO_SETLOCALE 1
#پایان_اگر

#اگر_تعریف_نشده HB_NO_SETLOCALE

#ضمن <locale.h>
#اگر_تعریف_شده HAVE_XLOCALE_H
#ضمن <xlocale.h> // Needed on BSD/OS X for uselocale
#پایان_اگر

#اگر_تعریف_شده WIN32
#تعریف_کن hb_locale_t _locale_t
#وإلا
#تعریف_کن hb_locale_t locale_t
#پایان_اگر
#تعریف_کن hb_setlocale setlocale
#تعریف_کن hb_uselocale uselocale

#وإلا

#تعریف_کن hb_locale_t فراغ *
#تعریف_کن hb_setlocale(Category, Locale) "C"
#تعریف_کن hb_uselocale(Locale) ((hb_locale_t) 0)

#پایان_اگر


/* Lets assert int types.  Saves trouble down the road. */
static_assert ((حجم (hb_codepoint_t) == 4), "");
static_assert ((حجم (hb_position_t) == 4), "");
static_assert ((حجم (hb_ماسک_t) == 4), "");
static_assert ((حجم (hb_var_int_t) == 4), "");


/* Pie time. */
// https://github.com/harfbuzz/harfbuzz/issues/4166
#تعریف_کن HB_PI 3.14159265358979323846f
#تعریف_کن HB_2_PI (2.f * HB_PI)

/* Compile-time custom allocator support. */

#إذا !defined(HB_CUSTOM_MALLOC) \
  && defined(hb_malloc_impl) \
  && defined(hb_calloc_impl) \
  && defined(hb_realloc_impl) \
  && defined(hb_free_impl)
#تعریف_کن HB_CUSTOM_MALLOC
#پایان_اگر

#اگر_تعریف_شده HB_CUSTOM_MALLOC
خارجي "C" فراغ* hb_malloc_impl(size_t اندازه);
خارجي "C" فراغ* hb_calloc_impl(size_t nmemb, size_t اندازه);
خارجي "C" فراغ* hb_realloc_impl(فراغ *ptr, size_t اندازه);
خارجي "C" فراغ  hb_free_impl(فراغ *ptr);
#وإلا
#تعریف_کن hb_malloc_impl حافظه_بده
#تعریف_کن hb_calloc_impl حافظه_صفر_بده
#تعریف_کن hb_realloc_impl realloc
#تعریف_کن hb_free_impl حرر
#پایان_اگر



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
قالب <نام_نوع T> استفاده hb_unique_ptr_t = hb::unique_ptr<T>;


/* Our src/test-*.cc use hb_assert(), such that it's not compiled out under NDEBUG.
 * https://github.com/harfbuzz/harfbuzz/issues/5418 */
#تعریف_کن hb_always_assert(x) \
	HB_STMT_شروع { \
	  إذا (!(x)) { fprintf(stderr, "Assertion failed: %s, at %s:%d\n", #x, __FILE__, __LINE__); abort(); } \
	} HB_STMT_انتها


#پایان_اگر /* HB_HH */
