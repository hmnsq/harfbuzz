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

#నిర్వచితం_కాకపోతే HB_HH
#నిర్వచించు HB_HH

#నిర్వచితం_కాకపోతే HB_NO_PRAGMA_GCC_DIAGNOSTIC
#నిర్వచితమైతే _MSC_VER
#సూచన warning( disable: 4068 ) /* Unknown pragma */
#అయితే_ముగింపు
#ఒకవేళ defined(__GNUC__) || defined(__clang__)
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
#నిర్వచితం_కాకపోతే HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
#సూచన GCC diagnostic warning "-Wall"
#సూచన GCC diagnostic warning "-Wextra"
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#సూచన GCC diagnostic ignored "-Wpragmas"
#సూచన GCC diagnostic ignored "-Wunknown-pragmas"
#సూచన GCC diagnostic ignored "-Wunknown-warning-option"
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
//#pragma GCC diagnostic warning "-Weverything"
#అయితే_ముగింపు

/* Error.  Should never happen. */
#నిర్వచితం_కాకపోతే HB_NO_PRAGMA_GCC_DIAGNOSTIC_ERROR
#సూచన GCC diagnostic error   "-Wbitwise-instead-of-logical"
#సూచన GCC diagnostic error   "-Wcast-align"
#సూచన GCC diagnostic error   "-Wcast-function-type"
#సూచన GCC diagnostic error   "-Wcast-function-type-strict"
#సూచన GCC diagnostic error   "-Wconstant-conversion"
#సూచన GCC diagnostic error   "-Wcomma"
#సూచన GCC diagnostic error   "-Wdelete-non-virtual-dtor"
#సూచన GCC diagnostic error   "-Wembedded-directive"
#సూచన GCC diagnostic error   "-Wextra-semi-stmt"
#సూచన GCC diagnostic error   "-Wformat-security"
#సూచన GCC diagnostic error   "-Wimplicit-function-declaration"
#సూచన GCC diagnostic error   "-Winit-self"
#సూచన GCC diagnostic error   "-Winjected-class-name"
#సూచన GCC diagnostic error   "-Wmissing-braces"
#సూచన GCC diagnostic error   "-Wmissing-declarations"
#సూచన GCC diagnostic error   "-Wmissing-prototypes"
#సూచన GCC diagnostic error   "-Wnarrowing"
#సూచన GCC diagnostic error   "-Wnested-externs"
#సూచన GCC diagnostic error   "-Wold-style-definition"
#సూచన GCC diagnostic error   "-Wpointer-arith"
#సూచన GCC diagnostic error   "-Wredundant-decls"
#సూచన GCC diagnostic error   "-Wreorder"
#సూచన GCC diagnostic error   "-Wsign-compare"
#సూచన GCC diagnostic error   "-Wstrict-flex-arrays"
#సూచన GCC diagnostic error   "-Wstrict-prototypes"
#సూచన GCC diagnostic error   "-Wstring-conversion"
#సూచన GCC diagnostic error   "-Wswitch-enum"
#సూచన GCC diagnostic error   "-Wtautological-overlap-compare"
#సూచన GCC diagnostic error   "-Wunneeded-internal-declaration"
#సూచన GCC diagnostic error   "-Wunused"
#సూచన GCC diagnostic error   "-Wunused-local-typedefs"
#సూచన GCC diagnostic error   "-Wunused-value"
#సూచన GCC diagnostic error   "-Wunused-variable"
#సూచన GCC diagnostic error   "-Wvla"
#సూచన GCC diagnostic error   "-Wwrite-strings"
#అయితే_ముగింపు

/* Warning.  To be investigated if happens. */
#నిర్వచితం_కాకపోతే HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
#సూచన GCC diagnostic warning "-Wbuiltin-macro-redefined"
#సూచన GCC diagnostic warning "-Wdeprecated"
#సూచన GCC diagnostic warning "-Wdeprecated-declarations"
#సూచన GCC diagnostic warning "-Wdisabled-optimization"
#సూచన GCC diagnostic warning "-Wdouble-promotion"
#సూచన GCC diagnostic warning "-Wformat=2"
#సూచన GCC diagnostic warning "-Wformat-signedness"
#సూచన GCC diagnostic warning "-Wignored-pragma-optimize"
#సూచన GCC diagnostic warning "-Wlogical-op"
#సూచన GCC diagnostic warning "-Wmissing-format-attribute"
#సూచన GCC diagnostic warning "-Wpessimizing-move"
#సూచన GCC diagnostic warning "-Wundef"
#సూచన GCC diagnostic warning "-Wunsafe-loop-optimizations"
#సూచన GCC diagnostic warning "-Wunused-but-set-variable"
#నిర్వచితమైతే __clang__
// The following are too buggy on gcc
// https://github.com/harfbuzz/harfbuzz/issues/5589
// https://github.com/harfbuzz/harfbuzz/pull/5367
#సూచన GCC diagnostic warning "-Wmaybe-uninitialized"
#సూచన GCC diagnostic warning "-Wuninitialized"
#లేకపోతే
#సూచన GCC diagnostic ignored "-Wmaybe-uninitialized"
#సూచన GCC diagnostic ignored "-Wuninitialized"
#అయితే_ముగింపు
#అయితే_ముగింపు

/* Ignored currently, but should be fixed at some point. */
#నిర్వచితం_కాకపోతే HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#సూచన GCC diagnostic ignored "-Wconversion"			// TODO fix
#సూచన GCC diagnostic ignored "-Wshadow"			// TODO fix
#సూచన GCC diagnostic ignored "-Wunused-parameter"		// TODO fix
#ఒకవేళ defined(__GNUC__) && !defined(__clang__)
#సూచన GCC diagnostic ignored "-Wunused-result"		// TODO fix
#అయితే_ముగింపు
#అయితే_ముగింపు

/* Ignored intentionally. */
#నిర్వచితం_కాకపోతే HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#సూచన GCC diagnostic ignored "-Warray-bounds" // https://github.com/harfbuzz/harfbuzz/issues/5738
#సూచన GCC diagnostic ignored "-Wclass-memaccess"
#సూచన GCC diagnostic ignored "-Wcast-function-type-strict" // https://github.com/harfbuzz/harfbuzz/pull/3859#issuecomment-1295409126
#సూచన GCC diagnostic ignored "-Wdangling-reference" // https://github.com/harfbuzz/harfbuzz/issues/4043
#సూచన GCC diagnostic ignored "-Wdangling-pointer" // Trigerred by hb_decycler_node_t().
#సూచన GCC diagnostic ignored "-Wformat-nonliteral"
#సూచన GCC diagnostic ignored "-Wformat-zero-length"
#సూచన GCC diagnostic ignored "-Wmissing-field-initializers"
#సూచన GCC diagnostic ignored "-Wold-style-cast"
#సూచన GCC diagnostic ignored "-Wpacked" // Erratic impl in clang
#సూచన GCC diagnostic ignored "-Wrange-loop-analysis" // https://github.com/harfbuzz/harfbuzz/issues/2834
#సూచన GCC diagnostic ignored "-Wstrict-aliasing"
#సూచన GCC diagnostic ignored "-Wtype-limits"
#సూచన GCC diagnostic ignored "-Wunused-template"
#సూచన GCC diagnostic ignored "-Wc++11-compat" // only gcc raises it
#అయితే_ముగింపు

#అయితే_ముగింపు
#అయితే_ముగింపు


#అన్తర్భావ "hb-config.hh"


/*
 * Following added based on what AC_USE_SYSTEM_EXTENSIONS adds to
 * config.h.in.  Copied here for the convenience of those embedding
 * HarfBuzz and not using our build system.
 */
/* Enable extensions on AIX 3, Interix.  */
#నిర్వచితం_కాకపోతే ALL_ఆధారం
# define ALL_ఆధారం 1
#అయితే_ముగింపు
/* Enable GNU extensions on systems that have them.  */
#నిర్వచితం_కాకపోతే _GNU_SOURCE
# define _GNU_SOURCE 1
#అయితే_ముగింపు
/* Enable threading extensions on Solaris.  */
#నిర్వచితం_కాకపోతే _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#అయితే_ముగింపు
/* Enable extensions on HP NonStop.  */
#నిర్వచితం_కాకపోతే TANDEM_ఆధారం
# define TANDEM_ఆధారం 1
#అయితే_ముగింపు
/* Enable general extensions on Solaris.  */
#నిర్వచితం_కాకపోతే __EXTENSIONS__
# define __EXTENSIONS__ 1
#అయితే_ముగింపు

#ఒకవేళ defined (_MSC_VER) && defined (HB_DLL_EXPORT)
#నిర్వచించు HB_EXTERN __declspec (dllexport) బాహ్య
#అయితే_ముగింపు

// https://github.com/harfbuzz/harfbuzz/pull/4619
#నిర్వచితం_కాకపోతే __STDC_FORMAT_MACROS
#నిర్వచించు __STDC_FORMAT_MACROS 1
#అయితే_ముగింపు

#అన్తర్భావ "hb.h"
#నిర్వచించు HB_H_లోపల
#అన్తర్భావ "hb-ot.h"
#నిర్వచించు HB_OT_H_లోపల
#అన్తర్భావ "hb-aat.h"
#నిర్వచించు HB_AAT_H_లోపల
#నిర్వచించు HB_SUBSET_H_లోపల

#అన్తర్భావ <cassert>
#అన్తర్భావ <cfloat>
#అన్తర్భావ <climits>
#ఒకవేళ defined(_MSC_VER) && !defined(USE_MATH_నిర్వచిస్తుంది)
# define USE_MATH_నిర్వచిస్తుంది
#అయితే_ముగింపు
#అన్తర్భావ <cmath>
#అన్తర్భావ <cstdarg>
#అన్తర్భావ <cstddef>
#అన్తర్భావ <cstdio>
#అన్తర్భావ <cstdlib>
#అన్తర్భావ <cstring>

#ఒకవేళ (defined(_MSC_VER) && _MSC_VER >= 1500) || defined(__MINGW32__)
#నిర్వచితమైతే __MINGW32_VERSION
#నిర్వచితం_కాకపోతే WIN32_LEAN_AND_సగటు
#నిర్వచించు WIN32_LEAN_AND_సగటు 1
#అయితే_ముగింపు
#లేకపోతే
#అన్తర్భావ <intrin.h>
#అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితమైతే _WIN32
#అన్తర్భావ <windows.h>
#అన్తర్భావ <winapifamily.h>
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే PRId32
# define PRId32 "d"
# define PRIu32 "u"
# define PRIx32 "x"
#అయితే_ముగింపు

#నిర్వచించు HB_PASTE1(a,b) a##b
#నిర్వచించు HB_PASTE(a,b) HB_PASTE1(a,b)


/*
 * Compiler attributes
 */

// gcc 10 has __has_builtin but not earlier versions. Sanction any gcc >= 5
// clang defines it so no need.
#నిర్వచితమైతే __has_builtin
#నిర్వచించు hb_has_builtin __has_builtin
#లేదావేళ defined(_MSC_VER)
#నిర్వచించు hb_has_builtin(x) 0
#లేకపోతే
#నిర్వచించు hb_has_builtin(x) ((defined(__GNUC__) && __GNUC__ >= 5))
#అయితే_ముగింపు

#ఒకవేళ defined(__OPTIMIZE__) && hb_has_builtin(__builtin_expect)
#నిర్వచించు likely(సమాసం) __builtin_expect (తార్కిక(సమాసం), 1)
#నిర్వచించు unlikely(సమాసం) __builtin_expect (తార్కిక(సమాసం), 0)
#లేకపోతే
#నిర్వచించు likely(సమాసం) (సమాసం)
#నిర్వచించు unlikely(సమాసం) (సమాసం)
#అయితే_ముగింపు

#ఒకవేళ !defined(__GNUC__) && !defined(__clang__)
#నిర్వచనం_రద్దు __attribute__
#నిర్వచించు __attribute__(x)
#అయితే_ముగింపు

#ఒకవేళ defined(__MINGW32__) && (__GNUC__ >= 3) && !defined(__clang__)
#నిర్వచించు HB_PRINTF_FUNC(format_idx, arg_idx) __attribute__((__format__ (gnu_printf, format_idx, arg_idx)))
#లేదావేళ defined(__GNUC__) && (__GNUC__ >= 3)
#నిర్వచించు HB_PRINTF_FUNC(format_idx, arg_idx) __attribute__((__format__ (__printf__, format_idx, arg_idx)))
#లేకపోతే
#నిర్వచించు HB_PRINTF_FUNC(format_idx, arg_idx)
#అయితే_ముగింపు
#ఒకవేళ defined(__GNUC__) && (__GNUC__ >= 4) || (__clang__)
#నిర్వచించు HB_వాడని	__attribute__((వాడని_2))
#లేదావేళ defined(_MSC_VER) /* https://github.com/harfbuzz/harfbuzz/issues/635 */
#నిర్వచించు HB_వాడని __pragma(warning(suppress: 4100 4101 4189))
#లేకపోతే
#నిర్వచించు HB_వాడని
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_అంతర్గత
# ఒకవేళ !defined(HB_NO_VISIBILITY) && !defined(__MINGW32__) && !defined(__CYGWIN__) && !defined(_MSC_VER) && !defined(__SUNPRO_CC)
#  define HB_అంతర్గత __attribute__((__visibility__("hidden")))
# elif defined(__MINGW32__)
   /* We use -export-symbols on mingw32, since it does not support visibility attributes. */
#  define HB_అంతర్గత
# elif defined (_MSC_VER) && defined (HB_DLL_EXPORT)
   /* We do not try to export internal symbols on Visual Studio */
#  define HB_అంతర్గత
#లేకపోతే
#  define HB_అంతర్గత
#  define HB_NO_VISIBILITY 1
# endif
#అయితే_ముగింపు

/* https://github.com/harfbuzz/harfbuzz/issues/1651 */
#ఒకవేళ defined(__clang__) && __clang_major__ < 10
#నిర్వచించు static_const స్థిర
#లేకపోతే
#నిర్వచించు static_const స్థిర స్థిరరాశి
#అయితే_ముగింపు

#ఒకవేళ defined(__GNUC__) && (__GNUC__ >= 3)
#నిర్వచించు HB_FUNC __PRETTY_FUNCTION__
#లేదావేళ defined(_MSC_VER)
#నిర్వచించు HB_FUNC __FUNCSIG__
#లేకపోతే
#నిర్వచించు HB_FUNC __func__
#అయితే_ముగింపు

#ఒకవేళ defined(__SUNPRO_CC) && (__SUNPRO_CC < 0x5140)
/* https://github.com/harfbuzz/harfbuzz/issues/630 */
#నిర్వచించు __restrict
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_ALWAYS_INLINE
#ఒకవేళ defined(_MSC_VER)
#నిర్వచించు HB_ALWAYS_INLINE __forceinline
#లేకపోతే
#నిర్వచించు HB_ALWAYS_INLINE __attribute__((always_inline)) అంతఃవరుస
#అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NEVER_INLINE
#ఒకవేళ defined(_MSC_VER)
#నిర్వచించు HB_NEVER_INLINE __declspec(noinline)
#లేకపోతే
#నిర్వచించు HB_NEVER_INLINE __attribute__((noinline))
#అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_HOT
#నిర్వచించు HB_HOT __attribute__((hot))
#అయితే_ముగింపు

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
#ఒకవేళ defined(__clang__) && __cplusplus >= 201103L
   /* clang's fallthrough annotations are only available starting in C++11. */
#  define HB_FALLTHROUGH [[clang::fallthrough]]
#లేదావేళ defined(__GNUC__) && (__GNUC__ >= 7)
   /* GNU fallthrough attribute is available from GCC7 */
#  define HB_FALLTHROUGH __attribute__((fallthrough))
#లేదావేళ defined(_MSC_VER)
   /*
    * MSVC's __fallthrough annotations are checked by /analyze (Code Analysis):
    * https://msdn.microsoft.com/en-us/library/ms235402%28VS.80%29.aspx
    */
#  include <sal.h>
#  define HB_FALLTHROUGH __fallthrough
#లేకపోతే
#  define HB_FALLTHROUGH /* FALLTHROUGH */
#అయితే_ముగింపు

/* A tag to enforce use of return value for a function */
#ఒకవేళ __cplusplus >= 201703L
#  define HB_NODISCARD [[nodiscard]]
#లేదావేళ defined(__GNUC__) || defined(__clang__)
#  define HB_NODISCARD __attribute__((warn_వాడని_ఫలితం))
#లేదావేళ defined(_MSC_VER)
#  define HB_NODISCARD _Check_return_
#లేకపోతే
#  define HB_NODISCARD
#అయితే_ముగింపు

/* Like HB_NODISCARD but can be used on structs */
#ఒకవేళ __cplusplus >= 201703L
#  define HB_NODISCARD_STRUCT [[nodiscard]]
#లేకపోతే
#  define HB_NODISCARD_STRUCT
#అయితే_ముగింపు

#నిర్వచితమైతే _WIN32
   /* We need Windows Vista for both Uniscribe backend and for
    * MemoryBarrier.  We don't support compiling on Windows XP,
    * though we run on it fine. */
#  ఒకవేళ defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0600
#    undef _WIN32_WINNT
#  endif
#  ifndef _WIN32_WINNT
#    ఒకవేళ WINAPI_FAMILY_విభజన(WINAPI_విభజన_DESKTOP)
#      define _WIN32_WINNT 0x0600
#    endif
#  endif
#  ifndef WIN32_LEAN_AND_సగటు
#    define WIN32_LEAN_AND_సగటు 1
#  endif
#  ifndef STRICT
#    define STRICT 1
#  endif

#  ఒకవేళ defined(_WIN32_WCE)
     /* Some things not defined on Windows CE. */
#    define vsnprintf _vsnprintf
#    ifndef HB_NO_GETENV
#      define HB_NO_GETENV
#    endif
#    ఒకవేళ _WIN32_WCE < 0x800
#      define HB_NO_SETLOCALE
#      define HB_NO_ERRNO
#    endif
#  elif !WINAPI_FAMILY_విభజన(WINAPI_విభజన_DESKTOP)
#    ifndef HB_NO_GETENV
#      define HB_NO_GETENV
#    endif
#  endif
#  ఒకవేళ defined(_MSC_VER) && _MSC_VER < 1900
#    define snprintf _snprintf
#  endif
#అయితే_ముగింపు

#నిర్వచితమైతే HB_NO_GETENV
#నిర్వచించు getenv(Name) శూన్యసూచిక
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_ERRNO
#  include <cerrno>
#లేకపోతే
స్థిర పూర్ణాంకం HB_వాడని _hb_errno = 0;
#  undef errno
#  define errno _hb_errno
#అయితే_ముగింపు

#నిర్వచించు HB_STMT_ప్రారంభం చేయి
#నిర్వచించు HB_STMT_చివర   అయితే (0)

#ఒకవేళ defined(HAVE_ATEXIT) && !defined(HB_USE_ATEXIT)
/* atexit() is only safe to be called from shared libraries on certain
 * platforms.  Whitelist.
 * https://bugs.freedesktop.org/show_bug.cgi?id=82246 */
#  ఒకవేళ defined(__linux) && defined(__GLIBC_PREREQ)
#    ఒకవేళ __GLIBC_PREREQ(2,3)
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
#అయితే_ముగింపు /* defined(HAVE_ATEXIT) && !defined(HB_USE_ATEXIT) */
#నిర్వచితమైతే HB_NO_ATEXIT
#  undef HB_USE_ATEXIT
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_USE_ATEXIT
#  define HB_USE_ATEXIT 0
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే hb_atexit
#ఒకవేళ !HB_USE_ATEXIT
#  define hb_atexit(_) HB_STMT_ప్రారంభం { ఒకవేళ (0) (_) (); } HB_STMT_చివర
#లేకపోతే /* HB_USE_ATEXIT */
#  ifdef HAVE_ATEXIT
#    define hb_atexit atexit
#  లేకపోతే
     మూస <శూన్య (*function) (శూన్య)> నిర్మాణం hb_atexit_t { ~hb_atexit_t () { function (); } };
#    define hb_atexit(f) స్థిర hb_atexit_t<f> _hb_atexit_##__LINE__
#  endif
#అయితే_ముగింపు
#అయితే_ముగింపు


// Locale business

#ఒకవేళ !defined(HB_NO_SETLOCALE) && (!defined(HAVE_NEWLOCALE) || !defined(HAVE_USELOCALE))
#నిర్వచించు HB_NO_SETLOCALE 1
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_SETLOCALE

#అన్తర్భావ <locale.h>
#నిర్వచితమైతే HAVE_XLOCALE_H
#అన్తర్భావ <xlocale.h> // Needed on BSD/OS X for uselocale
#అయితే_ముగింపు

#నిర్వచితమైతే WIN32
#నిర్వచించు hb_locale_t _locale_t
#లేకపోతే
#నిర్వచించు hb_locale_t locale_t
#అయితే_ముగింపు
#నిర్వచించు hb_setlocale setlocale
#నిర్వచించు hb_uselocale uselocale

#లేకపోతే

#నిర్వచించు hb_locale_t శూన్య *
#నిర్వచించు hb_setlocale(Category, Locale) "C"
#నిర్వచించు hb_uselocale(Locale) ((hb_locale_t) 0)

#అయితే_ముగింపు


/* Lets assert int types.  Saves trouble down the road. */
static_assert ((పరిమాణం (hb_codepoint_t) == 4), "");
static_assert ((పరిమాణం (hb_position_t) == 4), "");
static_assert ((పరిమాణం (hb_ముసుగు_t) == 4), "");
static_assert ((పరిమాణం (hb_var_int_t) == 4), "");


/* Pie time. */
// https://github.com/harfbuzz/harfbuzz/issues/4166
#నిర్వచించు HB_PI 3.14159265358979323846f
#నిర్వచించు HB_2_PI (2.f * HB_PI)

/* Compile-time custom allocator support. */

#ఒకవేళ !defined(HB_CUSTOM_MALLOC) \
  && defined(hb_malloc_impl) \
  && defined(hb_calloc_impl) \
  && defined(hb_realloc_impl) \
  && defined(hb_free_impl)
#నిర్వచించు HB_CUSTOM_MALLOC
#అయితే_ముగింపు

#నిర్వచితమైతే HB_CUSTOM_MALLOC
బాహ్య "C" శూన్య* hb_malloc_impl(size_t కొలత);
బాహ్య "C" శూన్య* hb_calloc_impl(size_t nmemb, size_t కొలత);
బాహ్య "C" శూన్య* hb_realloc_impl(శూన్య *ptr, size_t కొలత);
బాహ్య "C" శూన్య  hb_free_impl(శూన్య *ptr);
#లేకపోతే
#నిర్వచించు hb_malloc_impl జ్ఞాపకం_ఇవ్వు
#నిర్వచించు hb_calloc_impl సున్నా_జ్ఞాపకం_ఇవ్వు
#నిర్వచించు hb_realloc_impl realloc
#నిర్వచించు hb_free_impl విడుదల
#అయితే_ముగింపు



/* Headers we include for everyone.  Keep topologically sorted by dependency.
 * They express dependency amongst themselves, but no other file should include
 * them directly.*/
#అన్తర్భావ "hb-limits.hh"
#అన్తర్భావ "hb-cplusplus.hh"
#అన్తర్భావ "hb-meta.hh"
#అన్తర్భావ "hb-mutex.hh"
#అన్తర్భావ "hb-number.hh"
#అన్తర్భావ "hb-atomic.hh"	// Requires: hb-meta
#అన్తర్భావ "hb-null.hh"	// Requires: hb-meta
#అన్తర్భావ "hb-algs.hh"	// Requires: hb-meta hb-null hb-number
#అన్తర్భావ "hb-iter.hh"	// Requires: hb-algs hb-meta
#అన్తర్భావ "hb-debug.hh"	// Requires: hb-algs hb-atomic
#అన్తర్భావ "hb-array.hh"	// Requires: hb-algs hb-iter hb-null
#అన్తర్భావ "hb-vector.hh"	// Requires: hb-array hb-null
#అన్తర్భావ "hb-object.hh"	// Requires: hb-atomic hb-mutex hb-vector


/* Library-internal alias for hb::unique_ptr defined in hb-cplusplus.hh.
 * Unique-ownership RAII wrapper around an HB object; destroys on scope exit
 * via the type's hb_*_destroy() function.  Usage: hb_unique_ptr_t<hb_blob_t>.
 * Matches the hb_*_t naming used elsewhere in internal code. */
మూస <రకం_పేరు T> ఉపయోగించి hb_unique_ptr_t = hb::unique_ptr<T>;


/* Our src/test-*.cc use hb_assert(), such that it's not compiled out under NDEBUG.
 * https://github.com/harfbuzz/harfbuzz/issues/5418 */
#నిర్వచించు hb_always_assert(x) \
	HB_STMT_ప్రారంభం { \
	  ఒకవేళ (!(x)) { fprintf(stderr, "Assertion failed: %s, at %s:%d\n", #x, __FILE__, __LINE__); abort(); } \
	} HB_STMT_చివర


#అయితే_ముగింపు /* HB_HH */
