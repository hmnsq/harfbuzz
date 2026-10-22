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

#که_نه_تعریف_شوی HB_HH
#تعریف_کړه HB_HH

#که_نه_تعریف_شوی HB_NO_PRAGMA_GCC_DIAGNOSTIC
#که_تعریف_شوی _MSC_VER
#لارښوونه warning( disable: 4068 ) /* Unknown pragma */
#د_که_پای
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
#که_نه_تعریف_شوی HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
#لارښوونه GCC diagnostic warning "-Wall"
#لارښوونه GCC diagnostic warning "-Wextra"
#د_که_پای
#که_نه_تعریف_شوی HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#لارښوونه GCC diagnostic ignored "-Wpragmas"
#لارښوونه GCC diagnostic ignored "-Wunknown-pragmas"
#لارښوونه GCC diagnostic ignored "-Wunknown-warning-option"
#د_که_پای
#که_نه_تعریف_شوی HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
//#pragma GCC diagnostic warning "-Weverything"
#د_که_پای

/* Error.  Should never happen. */
#که_نه_تعریف_شوی HB_NO_PRAGMA_GCC_DIAGNOSTIC_ERROR
#لارښوونه GCC diagnostic error   "-Wbitwise-instead-of-logical"
#لارښوونه GCC diagnostic error   "-Wcast-align"
#لارښوونه GCC diagnostic error   "-Wcast-function-type"
#لارښوونه GCC diagnostic error   "-Wcast-function-type-strict"
#لارښوونه GCC diagnostic error   "-Wconstant-conversion"
#لارښوونه GCC diagnostic error   "-Wcomma"
#لارښوونه GCC diagnostic error   "-Wdelete-non-virtual-dtor"
#لارښوونه GCC diagnostic error   "-Wembedded-directive"
#لارښوونه GCC diagnostic error   "-Wextra-semi-stmt"
#لارښوونه GCC diagnostic error   "-Wformat-security"
#لارښوونه GCC diagnostic error   "-Wimplicit-function-declaration"
#لارښوونه GCC diagnostic error   "-Winit-self"
#لارښوونه GCC diagnostic error   "-Winjected-class-name"
#لارښوونه GCC diagnostic error   "-Wmissing-braces"
#لارښوونه GCC diagnostic error   "-Wmissing-declarations"
#لارښوونه GCC diagnostic error   "-Wmissing-prototypes"
#لارښوونه GCC diagnostic error   "-Wnarrowing"
#لارښوونه GCC diagnostic error   "-Wnested-externs"
#لارښوونه GCC diagnostic error   "-Wold-style-definition"
#لارښوونه GCC diagnostic error   "-Wpointer-arith"
#لارښوونه GCC diagnostic error   "-Wredundant-decls"
#لارښوونه GCC diagnostic error   "-Wreorder"
#لارښوونه GCC diagnostic error   "-Wsign-compare"
#لارښوونه GCC diagnostic error   "-Wstrict-flex-arrays"
#لارښوونه GCC diagnostic error   "-Wstrict-prototypes"
#لارښوونه GCC diagnostic error   "-Wstring-conversion"
#لارښوونه GCC diagnostic error   "-Wswitch-enum"
#لارښوونه GCC diagnostic error   "-Wtautological-overlap-compare"
#لارښوونه GCC diagnostic error   "-Wunneeded-internal-declaration"
#لارښوونه GCC diagnostic error   "-Wunused"
#لارښوونه GCC diagnostic error   "-Wunused-local-typedefs"
#لارښوونه GCC diagnostic error   "-Wunused-value"
#لارښوونه GCC diagnostic error   "-Wunused-variable"
#لارښوونه GCC diagnostic error   "-Wvla"
#لارښوونه GCC diagnostic error   "-Wwrite-strings"
#د_که_پای

/* Warning.  To be investigated if happens. */
#که_نه_تعریف_شوی HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
#لارښوونه GCC diagnostic warning "-Wbuiltin-macro-redefined"
#لارښوونه GCC diagnostic warning "-Wdeprecated"
#لارښوونه GCC diagnostic warning "-Wdeprecated-declarations"
#لارښوونه GCC diagnostic warning "-Wdisabled-optimization"
#لارښوونه GCC diagnostic warning "-Wdouble-promotion"
#لارښوونه GCC diagnostic warning "-Wformat=2"
#لارښوونه GCC diagnostic warning "-Wformat-signedness"
#لارښوونه GCC diagnostic warning "-Wignored-pragma-optimize"
#لارښوونه GCC diagnostic warning "-Wlogical-op"
#لارښوونه GCC diagnostic warning "-Wmissing-format-attribute"
#لارښوونه GCC diagnostic warning "-Wpessimizing-move"
#لارښوونه GCC diagnostic warning "-Wundef"
#لارښوونه GCC diagnostic warning "-Wunsafe-loop-optimizations"
#لارښوونه GCC diagnostic warning "-Wunused-but-set-variable"
#که_تعریف_شوی __clang__
// The following are too buggy on gcc
// https://github.com/harfbuzz/harfbuzz/issues/5589
// https://github.com/harfbuzz/harfbuzz/pull/5367
#لارښوونه GCC diagnostic warning "-Wmaybe-uninitialized"
#لارښوونه GCC diagnostic warning "-Wuninitialized"
#وإلا
#لارښوونه GCC diagnostic ignored "-Wmaybe-uninitialized"
#لارښوونه GCC diagnostic ignored "-Wuninitialized"
#د_که_پای
#د_که_پای

/* Ignored currently, but should be fixed at some point. */
#که_نه_تعریف_شوی HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#لارښوونه GCC diagnostic ignored "-Wconversion"			// TODO fix
#لارښوونه GCC diagnostic ignored "-Wshadow"			// TODO fix
#لارښوونه GCC diagnostic ignored "-Wunused-parameter"		// TODO fix
#إذا defined(__GNUC__) && !defined(__clang__)
#لارښوونه GCC diagnostic ignored "-Wunused-result"		// TODO fix
#د_که_پای
#د_که_پای

/* Ignored intentionally. */
#که_نه_تعریف_شوی HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#لارښوونه GCC diagnostic ignored "-Warray-bounds" // https://github.com/harfbuzz/harfbuzz/issues/5738
#لارښوونه GCC diagnostic ignored "-Wclass-memaccess"
#لارښوونه GCC diagnostic ignored "-Wcast-function-type-strict" // https://github.com/harfbuzz/harfbuzz/pull/3859#issuecomment-1295409126
#لارښوونه GCC diagnostic ignored "-Wdangling-reference" // https://github.com/harfbuzz/harfbuzz/issues/4043
#لارښوونه GCC diagnostic ignored "-Wdangling-pointer" // Trigerred by hb_decycler_node_t().
#لارښوونه GCC diagnostic ignored "-Wformat-nonliteral"
#لارښوونه GCC diagnostic ignored "-Wformat-zero-length"
#لارښوونه GCC diagnostic ignored "-Wmissing-field-initializers"
#لارښوونه GCC diagnostic ignored "-Wold-style-cast"
#لارښوونه GCC diagnostic ignored "-Wpacked" // Erratic impl in clang
#لارښوونه GCC diagnostic ignored "-Wrange-loop-analysis" // https://github.com/harfbuzz/harfbuzz/issues/2834
#لارښوونه GCC diagnostic ignored "-Wstrict-aliasing"
#لارښوونه GCC diagnostic ignored "-Wtype-limits"
#لارښوونه GCC diagnostic ignored "-Wunused-template"
#لارښوونه GCC diagnostic ignored "-Wc++11-compat" // only gcc raises it
#د_که_پای

#د_که_پای
#د_که_پای


#ضمن "hb-config.hh"


/*
 * Following added based on what AC_USE_SYSTEM_EXTENSIONS adds to
 * config.h.in.  Copied here for the convenience of those embedding
 * HarfBuzz and not using our build system.
 */
/* Enable extensions on AIX 3, Interix.  */
#که_نه_تعریف_شوی ALL_سرچینه
# define ALL_سرچینه 1
#د_که_پای
/* Enable GNU extensions on systems that have them.  */
#که_نه_تعریف_شوی _GNU_SOURCE
# define _GNU_SOURCE 1
#د_که_پای
/* Enable threading extensions on Solaris.  */
#که_نه_تعریف_شوی _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#د_که_پای
/* Enable extensions on HP NonStop.  */
#که_نه_تعریف_شوی TANDEM_سرچینه
# define TANDEM_سرچینه 1
#د_که_پای
/* Enable general extensions on Solaris.  */
#که_نه_تعریف_شوی __EXTENSIONS__
# define __EXTENSIONS__ 1
#د_که_پای

#إذا defined (_MSC_VER) && defined (HB_DLL_EXPORT)
#تعریف_کړه HB_EXTERN __declspec (dllexport) خارجي
#د_که_پای

// https://github.com/harfbuzz/harfbuzz/pull/4619
#که_نه_تعریف_شوی __STDC_FORMAT_MACROS
#تعریف_کړه __STDC_FORMAT_MACROS 1
#د_که_پای

#ضمن "hb.h"
#تعریف_کړه HB_H_دننه
#ضمن "hb-ot.h"
#تعریف_کړه HB_OT_H_دننه
#ضمن "hb-aat.h"
#تعریف_کړه HB_AAT_H_دننه
#تعریف_کړه HB_SUBSET_H_دننه

#ضمن <cassert>
#ضمن <cfloat>
#ضمن <climits>
#إذا defined(_MSC_VER) && !defined(USE_MATH_تعریفوي)
# define USE_MATH_تعریفوي
#د_که_پای
#ضمن <cmath>
#ضمن <cstdarg>
#ضمن <cstddef>
#ضمن <cstdio>
#ضمن <cstdlib>
#ضمن <cstring>

#إذا (defined(_MSC_VER) && _MSC_VER >= 1500) || defined(__MINGW32__)
#که_تعریف_شوی __MINGW32_VERSION
#که_نه_تعریف_شوی WIN32_LEAN_AND_منځنۍ
#تعریف_کړه WIN32_LEAN_AND_منځنۍ 1
#د_که_پای
#وإلا
#ضمن <intrin.h>
#د_که_پای
#د_که_پای

#که_تعریف_شوی _WIN32
#ضمن <windows.h>
#ضمن <winapifamily.h>
#د_که_پای

#که_نه_تعریف_شوی PRId32
# define PRId32 "d"
# define PRIu32 "u"
# define PRIx32 "x"
#د_که_پای

#تعریف_کړه HB_PASTE1(a,b) a##b
#تعریف_کړه HB_PASTE(a,b) HB_PASTE1(a,b)


/*
 * Compiler attributes
 */

// gcc 10 has __has_builtin but not earlier versions. Sanction any gcc >= 5
// clang defines it so no need.
#که_تعریف_شوی __has_builtin
#تعریف_کړه hb_has_builtin __has_builtin
#وإلاإذا defined(_MSC_VER)
#تعریف_کړه hb_has_builtin(x) 0
#وإلا
#تعریف_کړه hb_has_builtin(x) ((defined(__GNUC__) && __GNUC__ >= 5))
#د_که_پای

#إذا defined(__OPTIMIZE__) && hb_has_builtin(__builtin_expect)
#تعریف_کړه likely(بیان) __builtin_expect (منطقي(بیان), 1)
#تعریف_کړه unlikely(بیان) __builtin_expect (منطقي(بیان), 0)
#وإلا
#تعریف_کړه likely(بیان) (بیان)
#تعریف_کړه unlikely(بیان) (بیان)
#د_که_پای

#إذا !defined(__GNUC__) && !defined(__clang__)
#تعریف_لغوه __attribute__
#تعریف_کړه __attribute__(x)
#د_که_پای

#إذا defined(__MINGW32__) && (__GNUC__ >= 3) && !defined(__clang__)
#تعریف_کړه HB_PRINTF_FUNC(format_idx, arg_idx) __attribute__((__format__ (gnu_printf, format_idx, arg_idx)))
#وإلاإذا defined(__GNUC__) && (__GNUC__ >= 3)
#تعریف_کړه HB_PRINTF_FUNC(format_idx, arg_idx) __attribute__((__format__ (__printf__, format_idx, arg_idx)))
#وإلا
#تعریف_کړه HB_PRINTF_FUNC(format_idx, arg_idx)
#د_که_پای
#إذا defined(__GNUC__) && (__GNUC__ >= 4) || (__clang__)
#تعریف_کړه HB_ناکارول_شوی	__attribute__((ناکارول_شوی_2))
#وإلاإذا defined(_MSC_VER) /* https://github.com/harfbuzz/harfbuzz/issues/635 */
#تعریف_کړه HB_ناکارول_شوی __pragma(warning(suppress: 4100 4101 4189))
#وإلا
#تعریف_کړه HB_ناکارول_شوی
#د_که_پای

#که_نه_تعریف_شوی HB_دننی
# إذا !defined(HB_NO_VISIBILITY) && !defined(__MINGW32__) && !defined(__CYGWIN__) && !defined(_MSC_VER) && !defined(__SUNPRO_CC)
#  define HB_دننی __attribute__((__visibility__("hidden")))
# elif defined(__MINGW32__)
   /* We use -export-symbols on mingw32, since it does not support visibility attributes. */
#  define HB_دننی
# elif defined (_MSC_VER) && defined (HB_DLL_EXPORT)
   /* We do not try to export internal symbols on Visual Studio */
#  define HB_دننی
#وإلا
#  define HB_دننی
#  define HB_NO_VISIBILITY 1
# endif
#د_که_پای

/* https://github.com/harfbuzz/harfbuzz/issues/1651 */
#إذا defined(__clang__) && __clang_major__ < 10
#تعریف_کړه static_const ساكن
#وإلا
#تعریف_کړه static_const ساكن ثابت
#د_که_پای

#إذا defined(__GNUC__) && (__GNUC__ >= 3)
#تعریف_کړه HB_FUNC __PRETTY_FUNCTION__
#وإلاإذا defined(_MSC_VER)
#تعریف_کړه HB_FUNC __FUNCSIG__
#وإلا
#تعریف_کړه HB_FUNC __func__
#د_که_پای

#إذا defined(__SUNPRO_CC) && (__SUNPRO_CC < 0x5140)
/* https://github.com/harfbuzz/harfbuzz/issues/630 */
#تعریف_کړه __restrict
#د_که_پای

#که_نه_تعریف_شوی HB_ALWAYS_INLINE
#إذا defined(_MSC_VER)
#تعریف_کړه HB_ALWAYS_INLINE __forceinline
#وإلا
#تعریف_کړه HB_ALWAYS_INLINE __attribute__((always_inline)) مضمن
#د_که_پای
#د_که_پای

#که_نه_تعریف_شوی HB_NEVER_INLINE
#إذا defined(_MSC_VER)
#تعریف_کړه HB_NEVER_INLINE __declspec(noinline)
#وإلا
#تعریف_کړه HB_NEVER_INLINE __attribute__((noinline))
#د_که_پای
#د_که_پای

#که_نه_تعریف_شوی HB_HOT
#تعریف_کړه HB_HOT __attribute__((hot))
#د_که_پای

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
#د_که_پای

/* A tag to enforce use of return value for a function */
#إذا __cplusplus >= 201703L
#  define HB_NODISCARD [[nodiscard]]
#وإلاإذا defined(__GNUC__) || defined(__clang__)
#  define HB_NODISCARD __attribute__((warn_ناکارول_شوی_پایله))
#وإلاإذا defined(_MSC_VER)
#  define HB_NODISCARD _Check_return_
#وإلا
#  define HB_NODISCARD
#د_که_پای

/* Like HB_NODISCARD but can be used on structs */
#إذا __cplusplus >= 201703L
#  define HB_NODISCARD_STRUCT [[nodiscard]]
#وإلا
#  define HB_NODISCARD_STRUCT
#د_که_پای

#که_تعریف_شوی _WIN32
   /* We need Windows Vista for both Uniscribe backend and for
    * MemoryBarrier.  We don't support compiling on Windows XP,
    * though we run on it fine. */
#  إذا defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0600
#    undef _WIN32_WINNT
#  endif
#  ifndef _WIN32_WINNT
#    إذا WINAPI_FAMILY_برخه_کول(WINAPI_برخه_کول_DESKTOP)
#      define _WIN32_WINNT 0x0600
#    endif
#  endif
#  ifndef WIN32_LEAN_AND_منځنۍ
#    define WIN32_LEAN_AND_منځنۍ 1
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
#  elif !WINAPI_FAMILY_برخه_کول(WINAPI_برخه_کول_DESKTOP)
#    ifndef HB_NO_GETENV
#      define HB_NO_GETENV
#    endif
#  endif
#  إذا defined(_MSC_VER) && _MSC_VER < 1900
#    define snprintf _snprintf
#  endif
#د_که_پای

#که_تعریف_شوی HB_NO_GETENV
#تعریف_کړه getenv(Name) تش_اشاره
#د_که_پای

#که_نه_تعریف_شوی HB_NO_ERRNO
#  include <cerrno>
#وإلا
ساكن صحيح HB_ناکارول_شوی _hb_errno = 0;
#  undef errno
#  define errno _hb_errno
#د_که_پای

#تعریف_کړه HB_STMT_پیل افعل
#تعریف_کړه HB_STMT_پای_ټکی   طالما (0)

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
#د_که_پای /* defined(HAVE_ATEXIT) && !defined(HB_USE_ATEXIT) */
#که_تعریف_شوی HB_NO_ATEXIT
#  undef HB_USE_ATEXIT
#د_که_پای
#که_نه_تعریف_شوی HB_USE_ATEXIT
#  define HB_USE_ATEXIT 0
#د_که_پای
#که_نه_تعریف_شوی hb_atexit
#إذا !HB_USE_ATEXIT
#  define hb_atexit(_) HB_STMT_پیل { إذا (0) (_) (); } HB_STMT_پای_ټکی
#وإلا /* HB_USE_ATEXIT */
#  ifdef HAVE_ATEXIT
#    define hb_atexit atexit
#  وإلا
     کينډۍ <فراغ (*function) (فراغ)> بنية hb_atexit_t { ~hb_atexit_t () { function (); } };
#    define hb_atexit(f) ساكن hb_atexit_t<f> _hb_atexit_##__LINE__
#  endif
#د_که_پای
#د_که_پای


// Locale business

#إذا !defined(HB_NO_SETLOCALE) && (!defined(HAVE_NEWLOCALE) || !defined(HAVE_USELOCALE))
#تعریف_کړه HB_NO_SETLOCALE 1
#د_که_پای

#که_نه_تعریف_شوی HB_NO_SETLOCALE

#ضمن <locale.h>
#که_تعریف_شوی HAVE_XLOCALE_H
#ضمن <xlocale.h> // Needed on BSD/OS X for uselocale
#د_که_پای

#که_تعریف_شوی WIN32
#تعریف_کړه hb_locale_t _locale_t
#وإلا
#تعریف_کړه hb_locale_t locale_t
#د_که_پای
#تعریف_کړه hb_setlocale setlocale
#تعریف_کړه hb_uselocale uselocale

#وإلا

#تعریف_کړه hb_locale_t فراغ *
#تعریف_کړه hb_setlocale(Category, Locale) "C"
#تعریف_کړه hb_uselocale(Locale) ((hb_locale_t) 0)

#د_که_پای


/* Lets assert int types.  Saves trouble down the road. */
static_assert ((حجم (hb_codepoint_t) == 4), "");
static_assert ((حجم (hb_position_t) == 4), "");
static_assert ((حجم (hb_ماسک_t) == 4), "");
static_assert ((حجم (hb_var_int_t) == 4), "");


/* Pie time. */
// https://github.com/harfbuzz/harfbuzz/issues/4166
#تعریف_کړه HB_PI 3.14159265358979323846f
#تعریف_کړه HB_2_PI (2.f * HB_PI)

/* Compile-time custom allocator support. */

#إذا !defined(HB_CUSTOM_MALLOC) \
  && defined(hb_malloc_impl) \
  && defined(hb_calloc_impl) \
  && defined(hb_realloc_impl) \
  && defined(hb_free_impl)
#تعریف_کړه HB_CUSTOM_MALLOC
#د_که_پای

#که_تعریف_شوی HB_CUSTOM_MALLOC
خارجي "C" فراغ* hb_malloc_impl(size_t اندازه);
خارجي "C" فراغ* hb_calloc_impl(size_t nmemb, size_t اندازه);
خارجي "C" فراغ* hb_realloc_impl(فراغ *ptr, size_t اندازه);
خارجي "C" فراغ  hb_free_impl(فراغ *ptr);
#وإلا
#تعریف_کړه hb_malloc_impl حافظه_ورکړه
#تعریف_کړه hb_calloc_impl حافظه_صفر_ورکړه
#تعریف_کړه hb_realloc_impl realloc
#تعریف_کړه hb_free_impl حرر
#د_که_پای



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
کينډۍ <ډول_نوم T> کارول hb_unique_ptr_t = hb::unique_ptr<T>;


/* Our src/test-*.cc use hb_assert(), such that it's not compiled out under NDEBUG.
 * https://github.com/harfbuzz/harfbuzz/issues/5418 */
#تعریف_کړه hb_always_assert(x) \
	HB_STMT_پیل { \
	  إذا (!(x)) { fprintf(stderr, "Assertion failed: %s, at %s:%d\n", #x, __FILE__, __LINE__); abort(); } \
	} HB_STMT_پای_ټکی


#د_که_پای /* HB_HH */
