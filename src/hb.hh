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

#جيڪڏهن_وصف_نه_ٿيل HB_HH
#وصف_ڪريو HB_HH

#جيڪڏهن_وصف_نه_ٿيل HB_NO_PRAGMA_GCC_DIAGNOSTIC
#جيڪڏهن_وصف_ٿيل _MSC_VER
#هدايت warning( disable: 4068 ) /* Unknown pragma */
#جيڪڏهن_پورو
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
#جيڪڏهن_وصف_نه_ٿيل HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
#هدايت GCC diagnostic warning "-Wall"
#هدايت GCC diagnostic warning "-Wextra"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#هدايت GCC diagnostic ignored "-Wpragmas"
#هدايت GCC diagnostic ignored "-Wunknown-pragmas"
#هدايت GCC diagnostic ignored "-Wunknown-warning-option"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
//#pragma GCC diagnostic warning "-Weverything"
#جيڪڏهن_پورو

/* Error.  Should never happen. */
#جيڪڏهن_وصف_نه_ٿيل HB_NO_PRAGMA_GCC_DIAGNOSTIC_ERROR
#هدايت GCC diagnostic error   "-Wbitwise-instead-of-logical"
#هدايت GCC diagnostic error   "-Wcast-align"
#هدايت GCC diagnostic error   "-Wcast-function-type"
#هدايت GCC diagnostic error   "-Wcast-function-type-strict"
#هدايت GCC diagnostic error   "-Wconstant-conversion"
#هدايت GCC diagnostic error   "-Wcomma"
#هدايت GCC diagnostic error   "-Wdelete-non-virtual-dtor"
#هدايت GCC diagnostic error   "-Wembedded-directive"
#هدايت GCC diagnostic error   "-Wextra-semi-stmt"
#هدايت GCC diagnostic error   "-Wformat-security"
#هدايت GCC diagnostic error   "-Wimplicit-function-declaration"
#هدايت GCC diagnostic error   "-Winit-self"
#هدايت GCC diagnostic error   "-Winjected-class-name"
#هدايت GCC diagnostic error   "-Wmissing-braces"
#هدايت GCC diagnostic error   "-Wmissing-declarations"
#هدايت GCC diagnostic error   "-Wmissing-prototypes"
#هدايت GCC diagnostic error   "-Wnarrowing"
#هدايت GCC diagnostic error   "-Wnested-externs"
#هدايت GCC diagnostic error   "-Wold-style-definition"
#هدايت GCC diagnostic error   "-Wpointer-arith"
#هدايت GCC diagnostic error   "-Wredundant-decls"
#هدايت GCC diagnostic error   "-Wreorder"
#هدايت GCC diagnostic error   "-Wsign-compare"
#هدايت GCC diagnostic error   "-Wstrict-flex-arrays"
#هدايت GCC diagnostic error   "-Wstrict-prototypes"
#هدايت GCC diagnostic error   "-Wstring-conversion"
#هدايت GCC diagnostic error   "-Wswitch-enum"
#هدايت GCC diagnostic error   "-Wtautological-overlap-compare"
#هدايت GCC diagnostic error   "-Wunneeded-internal-declaration"
#هدايت GCC diagnostic error   "-Wunused"
#هدايت GCC diagnostic error   "-Wunused-local-typedefs"
#هدايت GCC diagnostic error   "-Wunused-value"
#هدايت GCC diagnostic error   "-Wunused-variable"
#هدايت GCC diagnostic error   "-Wvla"
#هدايت GCC diagnostic error   "-Wwrite-strings"
#جيڪڏهن_پورو

/* Warning.  To be investigated if happens. */
#جيڪڏهن_وصف_نه_ٿيل HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
#هدايت GCC diagnostic warning "-Wbuiltin-macro-redefined"
#هدايت GCC diagnostic warning "-Wdeprecated"
#هدايت GCC diagnostic warning "-Wdeprecated-declarations"
#هدايت GCC diagnostic warning "-Wdisabled-optimization"
#هدايت GCC diagnostic warning "-Wdouble-promotion"
#هدايت GCC diagnostic warning "-Wformat=2"
#هدايت GCC diagnostic warning "-Wformat-signedness"
#هدايت GCC diagnostic warning "-Wignored-pragma-optimize"
#هدايت GCC diagnostic warning "-Wlogical-op"
#هدايت GCC diagnostic warning "-Wmissing-format-attribute"
#هدايت GCC diagnostic warning "-Wpessimizing-move"
#هدايت GCC diagnostic warning "-Wundef"
#هدايت GCC diagnostic warning "-Wunsafe-loop-optimizations"
#هدايت GCC diagnostic warning "-Wunused-but-set-variable"
#جيڪڏهن_وصف_ٿيل __clang__
// The following are too buggy on gcc
// https://github.com/harfbuzz/harfbuzz/issues/5589
// https://github.com/harfbuzz/harfbuzz/pull/5367
#هدايت GCC diagnostic warning "-Wmaybe-uninitialized"
#هدايت GCC diagnostic warning "-Wuninitialized"
#وإلا
#هدايت GCC diagnostic ignored "-Wmaybe-uninitialized"
#هدايت GCC diagnostic ignored "-Wuninitialized"
#جيڪڏهن_پورو
#جيڪڏهن_پورو

/* Ignored currently, but should be fixed at some point. */
#جيڪڏهن_وصف_نه_ٿيل HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#هدايت GCC diagnostic ignored "-Wconversion"			// TODO fix
#هدايت GCC diagnostic ignored "-Wshadow"			// TODO fix
#هدايت GCC diagnostic ignored "-Wunused-parameter"		// TODO fix
#إذا defined(__GNUC__) && !defined(__clang__)
#هدايت GCC diagnostic ignored "-Wunused-result"		// TODO fix
#جيڪڏهن_پورو
#جيڪڏهن_پورو

/* Ignored intentionally. */
#جيڪڏهن_وصف_نه_ٿيل HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#هدايت GCC diagnostic ignored "-Warray-bounds" // https://github.com/harfbuzz/harfbuzz/issues/5738
#هدايت GCC diagnostic ignored "-Wclass-memaccess"
#هدايت GCC diagnostic ignored "-Wcast-function-type-strict" // https://github.com/harfbuzz/harfbuzz/pull/3859#issuecomment-1295409126
#هدايت GCC diagnostic ignored "-Wdangling-reference" // https://github.com/harfbuzz/harfbuzz/issues/4043
#هدايت GCC diagnostic ignored "-Wdangling-pointer" // Trigerred by hb_decycler_node_t().
#هدايت GCC diagnostic ignored "-Wformat-nonliteral"
#هدايت GCC diagnostic ignored "-Wformat-zero-length"
#هدايت GCC diagnostic ignored "-Wmissing-field-initializers"
#هدايت GCC diagnostic ignored "-Wold-style-cast"
#هدايت GCC diagnostic ignored "-Wpacked" // Erratic impl in clang
#هدايت GCC diagnostic ignored "-Wrange-loop-analysis" // https://github.com/harfbuzz/harfbuzz/issues/2834
#هدايت GCC diagnostic ignored "-Wstrict-aliasing"
#هدايت GCC diagnostic ignored "-Wtype-limits"
#هدايت GCC diagnostic ignored "-Wunused-template"
#هدايت GCC diagnostic ignored "-Wc++11-compat" // only gcc raises it
#جيڪڏهن_پورو

#جيڪڏهن_پورو
#جيڪڏهن_پورو


#ضمن "hb-config.hh"


/*
 * Following added based on what AC_USE_SYSTEM_EXTENSIONS adds to
 * config.h.in.  Copied here for the convenience of those embedding
 * HarfBuzz and not using our build system.
 */
/* Enable extensions on AIX 3, Interix.  */
#جيڪڏهن_وصف_نه_ٿيل ALL_ذريعو
# define ALL_ذريعو 1
#جيڪڏهن_پورو
/* Enable GNU extensions on systems that have them.  */
#جيڪڏهن_وصف_نه_ٿيل _GNU_SOURCE
# define _GNU_SOURCE 1
#جيڪڏهن_پورو
/* Enable threading extensions on Solaris.  */
#جيڪڏهن_وصف_نه_ٿيل _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#جيڪڏهن_پورو
/* Enable extensions on HP NonStop.  */
#جيڪڏهن_وصف_نه_ٿيل TANDEM_ذريعو
# define TANDEM_ذريعو 1
#جيڪڏهن_پورو
/* Enable general extensions on Solaris.  */
#جيڪڏهن_وصف_نه_ٿيل __EXTENSIONS__
# define __EXTENSIONS__ 1
#جيڪڏهن_پورو

#إذا defined (_MSC_VER) && defined (HB_DLL_EXPORT)
#وصف_ڪريو HB_EXTERN __declspec (dllexport) خارجي
#جيڪڏهن_پورو

// https://github.com/harfbuzz/harfbuzz/pull/4619
#جيڪڏهن_وصف_نه_ٿيل __STDC_FORMAT_MACROS
#وصف_ڪريو __STDC_FORMAT_MACROS 1
#جيڪڏهن_پورو

#ضمن "hb.h"
#وصف_ڪريو HB_H_اندريون
#ضمن "hb-ot.h"
#وصف_ڪريو HB_OT_H_اندريون
#ضمن "hb-aat.h"
#وصف_ڪريو HB_AAT_H_اندريون
#وصف_ڪريو HB_SUBSET_H_اندريون

#ضمن <cassert>
#ضمن <cfloat>
#ضمن <climits>
#إذا defined(_MSC_VER) && !defined(USE_MATH_مقرر_ڪري)
# define USE_MATH_مقرر_ڪري
#جيڪڏهن_پورو
#ضمن <cmath>
#ضمن <cstdarg>
#ضمن <cstddef>
#ضمن <cstdio>
#ضمن <cstdlib>
#ضمن <cstring>

#إذا (defined(_MSC_VER) && _MSC_VER >= 1500) || defined(__MINGW32__)
#جيڪڏهن_وصف_ٿيل __MINGW32_VERSION
#جيڪڏهن_وصف_نه_ٿيل WIN32_LEAN_AND_سراسري
#وصف_ڪريو WIN32_LEAN_AND_سراسري 1
#جيڪڏهن_پورو
#وإلا
#ضمن <intrin.h>
#جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل _WIN32
#ضمن <windows.h>
#ضمن <winapifamily.h>
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل PRId32
# define PRId32 "d"
# define PRIu32 "u"
# define PRIx32 "x"
#جيڪڏهن_پورو

#وصف_ڪريو HB_PASTE1(a,b) a##b
#وصف_ڪريو HB_PASTE(a,b) HB_PASTE1(a,b)


/*
 * Compiler attributes
 */

// gcc 10 has __has_builtin but not earlier versions. Sanction any gcc >= 5
// clang defines it so no need.
#جيڪڏهن_وصف_ٿيل __has_builtin
#وصف_ڪريو hb_has_builtin __has_builtin
#وإلاإذا defined(_MSC_VER)
#وصف_ڪريو hb_has_builtin(x) 0
#وإلا
#وصف_ڪريو hb_has_builtin(x) ((defined(__GNUC__) && __GNUC__ >= 5))
#جيڪڏهن_پورو

#إذا defined(__OPTIMIZE__) && hb_has_builtin(__builtin_expect)
#وصف_ڪريو likely(اظهار) __builtin_expect (منطقي(اظهار), 1)
#وصف_ڪريو unlikely(اظهار) __builtin_expect (منطقي(اظهار), 0)
#وإلا
#وصف_ڪريو likely(اظهار) (اظهار)
#وصف_ڪريو unlikely(اظهار) (اظهار)
#جيڪڏهن_پورو

#إذا !defined(__GNUC__) && !defined(__clang__)
#وصف_رد __attribute__
#وصف_ڪريو __attribute__(x)
#جيڪڏهن_پورو

#إذا defined(__MINGW32__) && (__GNUC__ >= 3) && !defined(__clang__)
#وصف_ڪريو HB_PRINTF_FUNC(format_idx, arg_idx) __attribute__((__format__ (gnu_printf, format_idx, arg_idx)))
#وإلاإذا defined(__GNUC__) && (__GNUC__ >= 3)
#وصف_ڪريو HB_PRINTF_FUNC(format_idx, arg_idx) __attribute__((__format__ (__printf__, format_idx, arg_idx)))
#وإلا
#وصف_ڪريو HB_PRINTF_FUNC(format_idx, arg_idx)
#جيڪڏهن_پورو
#إذا defined(__GNUC__) && (__GNUC__ >= 4) || (__clang__)
#وصف_ڪريو HB_اڻ_استعمال_ٿيل	__attribute__((اڻ_استعمال_ٿيل_2))
#وإلاإذا defined(_MSC_VER) /* https://github.com/harfbuzz/harfbuzz/issues/635 */
#وصف_ڪريو HB_اڻ_استعمال_ٿيل __pragma(warning(suppress: 4100 4101 4189))
#وإلا
#وصف_ڪريو HB_اڻ_استعمال_ٿيل
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_اندروني
# إذا !defined(HB_NO_VISIBILITY) && !defined(__MINGW32__) && !defined(__CYGWIN__) && !defined(_MSC_VER) && !defined(__SUNPRO_CC)
#  define HB_اندروني __attribute__((__visibility__("hidden")))
# elif defined(__MINGW32__)
   /* We use -export-symbols on mingw32, since it does not support visibility attributes. */
#  define HB_اندروني
# elif defined (_MSC_VER) && defined (HB_DLL_EXPORT)
   /* We do not try to export internal symbols on Visual Studio */
#  define HB_اندروني
#وإلا
#  define HB_اندروني
#  define HB_NO_VISIBILITY 1
# endif
#جيڪڏهن_پورو

/* https://github.com/harfbuzz/harfbuzz/issues/1651 */
#إذا defined(__clang__) && __clang_major__ < 10
#وصف_ڪريو static_const ساكن
#وإلا
#وصف_ڪريو static_const ساكن ثابت
#جيڪڏهن_پورو

#إذا defined(__GNUC__) && (__GNUC__ >= 3)
#وصف_ڪريو HB_FUNC __PRETTY_FUNCTION__
#وإلاإذا defined(_MSC_VER)
#وصف_ڪريو HB_FUNC __FUNCSIG__
#وإلا
#وصف_ڪريو HB_FUNC __func__
#جيڪڏهن_پورو

#إذا defined(__SUNPRO_CC) && (__SUNPRO_CC < 0x5140)
/* https://github.com/harfbuzz/harfbuzz/issues/630 */
#وصف_ڪريو __restrict
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_ALWAYS_INLINE
#إذا defined(_MSC_VER)
#وصف_ڪريو HB_ALWAYS_INLINE __forceinline
#وإلا
#وصف_ڪريو HB_ALWAYS_INLINE __attribute__((always_inline)) مضمن
#جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_NEVER_INLINE
#إذا defined(_MSC_VER)
#وصف_ڪريو HB_NEVER_INLINE __declspec(noinline)
#وإلا
#وصف_ڪريو HB_NEVER_INLINE __attribute__((noinline))
#جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_HOT
#وصف_ڪريو HB_HOT __attribute__((hot))
#جيڪڏهن_پورو

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
#جيڪڏهن_پورو

/* A tag to enforce use of return value for a function */
#إذا __cplusplus >= 201703L
#  define HB_NODISCARD [[nodiscard]]
#وإلاإذا defined(__GNUC__) || defined(__clang__)
#  define HB_NODISCARD __attribute__((warn_اڻ_استعمال_ٿيل_نتيجو))
#وإلاإذا defined(_MSC_VER)
#  define HB_NODISCARD _Check_return_
#وإلا
#  define HB_NODISCARD
#جيڪڏهن_پورو

/* Like HB_NODISCARD but can be used on structs */
#إذا __cplusplus >= 201703L
#  define HB_NODISCARD_STRUCT [[nodiscard]]
#وإلا
#  define HB_NODISCARD_STRUCT
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل _WIN32
   /* We need Windows Vista for both Uniscribe backend and for
    * MemoryBarrier.  We don't support compiling on Windows XP,
    * though we run on it fine. */
#  إذا defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0600
#    undef _WIN32_WINNT
#  endif
#  ifndef _WIN32_WINNT
#    إذا WINAPI_FAMILY_حصا_ڪريو(WINAPI_حصا_ڪريو_DESKTOP)
#      define _WIN32_WINNT 0x0600
#    endif
#  endif
#  ifndef WIN32_LEAN_AND_سراسري
#    define WIN32_LEAN_AND_سراسري 1
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
#  elif !WINAPI_FAMILY_حصا_ڪريو(WINAPI_حصا_ڪريو_DESKTOP)
#    ifndef HB_NO_GETENV
#      define HB_NO_GETENV
#    endif
#  endif
#  إذا defined(_MSC_VER) && _MSC_VER < 1900
#    define snprintf _snprintf
#  endif
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_NO_GETENV
#وصف_ڪريو getenv(Name) خالي_اشارو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_NO_ERRNO
#  include <cerrno>
#وإلا
ساكن صحيح HB_اڻ_استعمال_ٿيل _hb_errno = 0;
#  undef errno
#  define errno _hb_errno
#جيڪڏهن_پورو

#وصف_ڪريو HB_STMT_شروعات افعل
#وصف_ڪريو HB_STMT_پڇاڙي   طالما (0)

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
#جيڪڏهن_پورو /* defined(HAVE_ATEXIT) && !defined(HB_USE_ATEXIT) */
#جيڪڏهن_وصف_ٿيل HB_NO_ATEXIT
#  undef HB_USE_ATEXIT
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_USE_ATEXIT
#  define HB_USE_ATEXIT 0
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل hb_atexit
#إذا !HB_USE_ATEXIT
#  define hb_atexit(_) HB_STMT_شروعات { إذا (0) (_) (); } HB_STMT_پڇاڙي
#وإلا /* HB_USE_ATEXIT */
#  ifdef HAVE_ATEXIT
#    define hb_atexit atexit
#  وإلا
     سانچو <فراغ (*function) (فراغ)> بنية hb_atexit_t { ~hb_atexit_t () { function (); } };
#    define hb_atexit(f) ساكن hb_atexit_t<f> _hb_atexit_##__LINE__
#  endif
#جيڪڏهن_پورو
#جيڪڏهن_پورو


// Locale business

#إذا !defined(HB_NO_SETLOCALE) && (!defined(HAVE_NEWLOCALE) || !defined(HAVE_USELOCALE))
#وصف_ڪريو HB_NO_SETLOCALE 1
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_NO_SETLOCALE

#ضمن <locale.h>
#جيڪڏهن_وصف_ٿيل HAVE_XLOCALE_H
#ضمن <xlocale.h> // Needed on BSD/OS X for uselocale
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل WIN32
#وصف_ڪريو hb_locale_t _locale_t
#وإلا
#وصف_ڪريو hb_locale_t locale_t
#جيڪڏهن_پورو
#وصف_ڪريو hb_setlocale setlocale
#وصف_ڪريو hb_uselocale uselocale

#وإلا

#وصف_ڪريو hb_locale_t فراغ *
#وصف_ڪريو hb_setlocale(Category, Locale) "C"
#وصف_ڪريو hb_uselocale(Locale) ((hb_locale_t) 0)

#جيڪڏهن_پورو


/* Lets assert int types.  Saves trouble down the road. */
static_assert ((حجم (hb_codepoint_t) == 4), "");
static_assert ((حجم (hb_position_t) == 4), "");
static_assert ((حجم (hb_نقاب_t) == 4), "");
static_assert ((حجم (hb_var_int_t) == 4), "");


/* Pie time. */
// https://github.com/harfbuzz/harfbuzz/issues/4166
#وصف_ڪريو HB_PI 3.14159265358979323846f
#وصف_ڪريو HB_2_PI (2.f * HB_PI)

/* Compile-time custom allocator support. */

#إذا !defined(HB_CUSTOM_MALLOC) \
  && defined(hb_malloc_impl) \
  && defined(hb_calloc_impl) \
  && defined(hb_realloc_impl) \
  && defined(hb_free_impl)
#وصف_ڪريو HB_CUSTOM_MALLOC
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_CUSTOM_MALLOC
خارجي "C" فراغ* hb_malloc_impl(size_t سائيز);
خارجي "C" فراغ* hb_calloc_impl(size_t nmemb, size_t سائيز);
خارجي "C" فراغ* hb_realloc_impl(فراغ *ptr, size_t سائيز);
خارجي "C" فراغ  hb_free_impl(فراغ *ptr);
#وإلا
#وصف_ڪريو hb_malloc_impl يادگيري_ڏيو
#وصف_ڪريو hb_calloc_impl صفر_يادگيري_ڏيو
#وصف_ڪريو hb_realloc_impl realloc
#وصف_ڪريو hb_free_impl حرر
#جيڪڏهن_پورو



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
سانچو <قسم_نالو T> استعمال hb_unique_ptr_t = hb::unique_ptr<T>;


/* Our src/test-*.cc use hb_assert(), such that it's not compiled out under NDEBUG.
 * https://github.com/harfbuzz/harfbuzz/issues/5418 */
#وصف_ڪريو hb_always_assert(x) \
	HB_STMT_شروعات { \
	  إذا (!(x)) { fprintf(stderr, "Assertion failed: %s, at %s:%d\n", #x, __FILE__, __LINE__); abort(); } \
	} HB_STMT_پڇاڙي


#جيڪڏهن_پورو /* HB_HH */
