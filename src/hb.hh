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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_HH

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PRAGMA_GCC_DIAGNOSTIC
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
#ਹਦਾਇਤ warning( disable: 4068 ) /* Unknown pragma */
#ਜੇ_ਅੰਤ
#ਜੇ defined(__GNUC__) || defined(__clang__)
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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
#ਹਦਾਇਤ GCC diagnostic warning "-Wall"
#ਹਦਾਇਤ GCC diagnostic warning "-Wextra"
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#ਹਦਾਇਤ GCC diagnostic ignored "-Wpragmas"
#ਹਦਾਇਤ GCC diagnostic ignored "-Wunknown-pragmas"
#ਹਦਾਇਤ GCC diagnostic ignored "-Wunknown-warning-option"
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
//#pragma GCC diagnostic warning "-Weverything"
#ਜੇ_ਅੰਤ

/* Error.  Should never happen. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PRAGMA_GCC_DIAGNOSTIC_ERROR
#ਹਦਾਇਤ GCC diagnostic error   "-Wbitwise-instead-of-logical"
#ਹਦਾਇਤ GCC diagnostic error   "-Wcast-align"
#ਹਦਾਇਤ GCC diagnostic error   "-Wcast-function-type"
#ਹਦਾਇਤ GCC diagnostic error   "-Wcast-function-type-strict"
#ਹਦਾਇਤ GCC diagnostic error   "-Wconstant-conversion"
#ਹਦਾਇਤ GCC diagnostic error   "-Wcomma"
#ਹਦਾਇਤ GCC diagnostic error   "-Wdelete-non-virtual-dtor"
#ਹਦਾਇਤ GCC diagnostic error   "-Wembedded-directive"
#ਹਦਾਇਤ GCC diagnostic error   "-Wextra-semi-stmt"
#ਹਦਾਇਤ GCC diagnostic error   "-Wformat-security"
#ਹਦਾਇਤ GCC diagnostic error   "-Wimplicit-function-declaration"
#ਹਦਾਇਤ GCC diagnostic error   "-Winit-self"
#ਹਦਾਇਤ GCC diagnostic error   "-Winjected-class-name"
#ਹਦਾਇਤ GCC diagnostic error   "-Wmissing-braces"
#ਹਦਾਇਤ GCC diagnostic error   "-Wmissing-declarations"
#ਹਦਾਇਤ GCC diagnostic error   "-Wmissing-prototypes"
#ਹਦਾਇਤ GCC diagnostic error   "-Wnarrowing"
#ਹਦਾਇਤ GCC diagnostic error   "-Wnested-externs"
#ਹਦਾਇਤ GCC diagnostic error   "-Wold-style-definition"
#ਹਦਾਇਤ GCC diagnostic error   "-Wpointer-arith"
#ਹਦਾਇਤ GCC diagnostic error   "-Wredundant-decls"
#ਹਦਾਇਤ GCC diagnostic error   "-Wreorder"
#ਹਦਾਇਤ GCC diagnostic error   "-Wsign-compare"
#ਹਦਾਇਤ GCC diagnostic error   "-Wstrict-flex-arrays"
#ਹਦਾਇਤ GCC diagnostic error   "-Wstrict-prototypes"
#ਹਦਾਇਤ GCC diagnostic error   "-Wstring-conversion"
#ਹਦਾਇਤ GCC diagnostic error   "-Wswitch-enum"
#ਹਦਾਇਤ GCC diagnostic error   "-Wtautological-overlap-compare"
#ਹਦਾਇਤ GCC diagnostic error   "-Wunneeded-internal-declaration"
#ਹਦਾਇਤ GCC diagnostic error   "-Wunused"
#ਹਦਾਇਤ GCC diagnostic error   "-Wunused-local-typedefs"
#ਹਦਾਇਤ GCC diagnostic error   "-Wunused-value"
#ਹਦਾਇਤ GCC diagnostic error   "-Wunused-variable"
#ਹਦਾਇਤ GCC diagnostic error   "-Wvla"
#ਹਦਾਇਤ GCC diagnostic error   "-Wwrite-strings"
#ਜੇ_ਅੰਤ

/* Warning.  To be investigated if happens. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
#ਹਦਾਇਤ GCC diagnostic warning "-Wbuiltin-macro-redefined"
#ਹਦਾਇਤ GCC diagnostic warning "-Wdeprecated"
#ਹਦਾਇਤ GCC diagnostic warning "-Wdeprecated-declarations"
#ਹਦਾਇਤ GCC diagnostic warning "-Wdisabled-optimization"
#ਹਦਾਇਤ GCC diagnostic warning "-Wdouble-promotion"
#ਹਦਾਇਤ GCC diagnostic warning "-Wformat=2"
#ਹਦਾਇਤ GCC diagnostic warning "-Wformat-signedness"
#ਹਦਾਇਤ GCC diagnostic warning "-Wignored-pragma-optimize"
#ਹਦਾਇਤ GCC diagnostic warning "-Wlogical-op"
#ਹਦਾਇਤ GCC diagnostic warning "-Wmissing-format-attribute"
#ਹਦਾਇਤ GCC diagnostic warning "-Wpessimizing-move"
#ਹਦਾਇਤ GCC diagnostic warning "-Wundef"
#ਹਦਾਇਤ GCC diagnostic warning "-Wunsafe-loop-optimizations"
#ਹਦਾਇਤ GCC diagnostic warning "-Wunused-but-set-variable"
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __clang__
// The following are too buggy on gcc
// https://github.com/harfbuzz/harfbuzz/issues/5589
// https://github.com/harfbuzz/harfbuzz/pull/5367
#ਹਦਾਇਤ GCC diagnostic warning "-Wmaybe-uninitialized"
#ਹਦਾਇਤ GCC diagnostic warning "-Wuninitialized"
#ਨਹੀਂਤਾਂ
#ਹਦਾਇਤ GCC diagnostic ignored "-Wmaybe-uninitialized"
#ਹਦਾਇਤ GCC diagnostic ignored "-Wuninitialized"
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

/* Ignored currently, but should be fixed at some point. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#ਹਦਾਇਤ GCC diagnostic ignored "-Wconversion"			// TODO fix
#ਹਦਾਇਤ GCC diagnostic ignored "-Wshadow"			// TODO fix
#ਹਦਾਇਤ GCC diagnostic ignored "-Wunused-parameter"		// TODO fix
#ਜੇ defined(__GNUC__) && !defined(__clang__)
#ਹਦਾਇਤ GCC diagnostic ignored "-Wunused-result"		// TODO fix
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

/* Ignored intentionally. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#ਹਦਾਇਤ GCC diagnostic ignored "-Warray-bounds" // https://github.com/harfbuzz/harfbuzz/issues/5738
#ਹਦਾਇਤ GCC diagnostic ignored "-Wclass-memaccess"
#ਹਦਾਇਤ GCC diagnostic ignored "-Wcast-function-type-strict" // https://github.com/harfbuzz/harfbuzz/pull/3859#issuecomment-1295409126
#ਹਦਾਇਤ GCC diagnostic ignored "-Wdangling-reference" // https://github.com/harfbuzz/harfbuzz/issues/4043
#ਹਦਾਇਤ GCC diagnostic ignored "-Wdangling-pointer" // Trigerred by hb_decycler_node_t().
#ਹਦਾਇਤ GCC diagnostic ignored "-Wformat-nonliteral"
#ਹਦਾਇਤ GCC diagnostic ignored "-Wformat-zero-length"
#ਹਦਾਇਤ GCC diagnostic ignored "-Wmissing-field-initializers"
#ਹਦਾਇਤ GCC diagnostic ignored "-Wold-style-cast"
#ਹਦਾਇਤ GCC diagnostic ignored "-Wpacked" // Erratic impl in clang
#ਹਦਾਇਤ GCC diagnostic ignored "-Wrange-loop-analysis" // https://github.com/harfbuzz/harfbuzz/issues/2834
#ਹਦਾਇਤ GCC diagnostic ignored "-Wstrict-aliasing"
#ਹਦਾਇਤ GCC diagnostic ignored "-Wtype-limits"
#ਹਦਾਇਤ GCC diagnostic ignored "-Wunused-template"
#ਹਦਾਇਤ GCC diagnostic ignored "-Wc++11-compat" // only gcc raises it
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ


#ਅਨ੍ਤਰ੍ਭਾਵ "hb-config.hh"


/*
 * Following added based on what AC_USE_SYSTEM_EXTENSIONS adds to
 * config.h.in.  Copied here for the convenience of those embedding
 * HarfBuzz and not using our build system.
 */
/* Enable extensions on AIX 3, Interix.  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ALL_ਸਰੋਤ
# define ALL_ਸਰੋਤ 1
#ਜੇ_ਅੰਤ
/* Enable GNU extensions on systems that have them.  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _GNU_SOURCE
# define _GNU_SOURCE 1
#ਜੇ_ਅੰਤ
/* Enable threading extensions on Solaris.  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#ਜੇ_ਅੰਤ
/* Enable extensions on HP NonStop.  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ TANDEM_ਸਰੋਤ
# define TANDEM_ਸਰੋਤ 1
#ਜੇ_ਅੰਤ
/* Enable general extensions on Solaris.  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __EXTENSIONS__
# define __EXTENSIONS__ 1
#ਜੇ_ਅੰਤ

#ਜੇ defined (_MSC_VER) && defined (HB_DLL_EXPORT)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_EXTERN __declspec (dllexport) ਬਾਹਰੀ
#ਜੇ_ਅੰਤ

// https://github.com/harfbuzz/harfbuzz/pull/4619
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __STDC_FORMAT_MACROS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __STDC_FORMAT_MACROS 1
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.h"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_H_ਵਿੱਚ
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot.h"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_H_ਵਿੱਚ
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat.h"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_H_ਵਿੱਚ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SUBSET_H_ਵਿੱਚ

#ਅਨ੍ਤਰ੍ਭਾਵ <cassert>
#ਅਨ੍ਤਰ੍ਭਾਵ <cfloat>
#ਅਨ੍ਤਰ੍ਭਾਵ <climits>
#ਜੇ defined(_MSC_VER) && !defined(USE_MATH_ਨਿਰਧਾਰਤ_ਕਰਦਾ)
# define USE_MATH_ਨਿਰਧਾਰਤ_ਕਰਦਾ
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <cmath>
#ਅਨ੍ਤਰ੍ਭਾਵ <cstdarg>
#ਅਨ੍ਤਰ੍ਭਾਵ <cstddef>
#ਅਨ੍ਤਰ੍ਭਾਵ <cstdio>
#ਅਨ੍ਤਰ੍ਭਾਵ <cstdlib>
#ਅਨ੍ਤਰ੍ਭਾਵ <cstring>

#ਜੇ (defined(_MSC_VER) && _MSC_VER >= 1500) || defined(__MINGW32__)
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __MINGW32_VERSION
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WIN32_LEAN_AND_ਔਸਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WIN32_LEAN_AND_ਔਸਤ 1
#ਜੇ_ਅੰਤ
#ਨਹੀਂਤਾਂ
#ਅਨ੍ਤਰ੍ਭਾਵ <intrin.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
#ਅਨ੍ਤਰ੍ਭਾਵ <windows.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <winapifamily.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PRId32
# define PRId32 "d"
# define PRIu32 "u"
# define PRIx32 "x"
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PASTE1(a,b) a##b
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PASTE(a,b) HB_PASTE1(a,b)


/*
 * Compiler attributes
 */

// gcc 10 has __has_builtin but not earlier versions. Sanction any gcc >= 5
// clang defines it so no need.
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __has_builtin
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_has_builtin __has_builtin
#ਜਾਂਜੇ defined(_MSC_VER)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_has_builtin(x) 0
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_has_builtin(x) ((defined(__GNUC__) && __GNUC__ >= 5))
#ਜੇ_ਅੰਤ

#ਜੇ defined(__OPTIMIZE__) && hb_has_builtin(__builtin_expect)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ likely(ਵਿਅੰਜਕ) __builtin_expect (ਤਾਰਕਿਕ(ਵਿਅੰਜਕ), 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ unlikely(ਵਿਅੰਜਕ) __builtin_expect (ਤਾਰਕਿਕ(ਵਿਅੰਜਕ), 0)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ likely(ਵਿਅੰਜਕ) (ਵਿਅੰਜਕ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ unlikely(ਵਿਅੰਜਕ) (ਵਿਅੰਜਕ)
#ਜੇ_ਅੰਤ

#ਜੇ !defined(__GNUC__) && !defined(__clang__)
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ __attribute__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __attribute__(x)
#ਜੇ_ਅੰਤ

#ਜੇ defined(__MINGW32__) && (__GNUC__ >= 3) && !defined(__clang__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PRINTF_FUNC(format_idx, arg_idx) __attribute__((__format__ (gnu_printf, format_idx, arg_idx)))
#ਜਾਂਜੇ defined(__GNUC__) && (__GNUC__ >= 3)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PRINTF_FUNC(format_idx, arg_idx) __attribute__((__format__ (__printf__, format_idx, arg_idx)))
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PRINTF_FUNC(format_idx, arg_idx)
#ਜੇ_ਅੰਤ
#ਜੇ defined(__GNUC__) && (__GNUC__ >= 4) || (__clang__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਅਣਵਰਤਿਆ	__attribute__((ਅਣਵਰਤਿਆ_2))
#ਜਾਂਜੇ defined(_MSC_VER) /* https://github.com/harfbuzz/harfbuzz/issues/635 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਅਣਵਰਤਿਆ __pragma(warning(suppress: 4100 4101 4189))
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਅਣਵਰਤਿਆ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਅੰਦਰੂਨੀ
# ਜੇ !defined(HB_NO_VISIBILITY) && !defined(__MINGW32__) && !defined(__CYGWIN__) && !defined(_MSC_VER) && !defined(__SUNPRO_CC)
#  define HB_ਅੰਦਰੂਨੀ __attribute__((__visibility__("hidden")))
# elif defined(__MINGW32__)
   /* We use -export-symbols on mingw32, since it does not support visibility attributes. */
#  define HB_ਅੰਦਰੂਨੀ
# elif defined (_MSC_VER) && defined (HB_DLL_EXPORT)
   /* We do not try to export internal symbols on Visual Studio */
#  define HB_ਅੰਦਰੂਨੀ
#ਨਹੀਂਤਾਂ
#  define HB_ਅੰਦਰੂਨੀ
#  define HB_NO_VISIBILITY 1
# endif
#ਜੇ_ਅੰਤ

/* https://github.com/harfbuzz/harfbuzz/issues/1651 */
#ਜੇ defined(__clang__) && __clang_major__ < 10
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ static_const ਸਥਿਰ
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ static_const ਸਥਿਰ ਅਚਲ
#ਜੇ_ਅੰਤ

#ਜੇ defined(__GNUC__) && (__GNUC__ >= 3)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FUNC __PRETTY_FUNCTION__
#ਜਾਂਜੇ defined(_MSC_VER)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FUNC __FUNCSIG__
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FUNC __func__
#ਜੇ_ਅੰਤ

#ਜੇ defined(__SUNPRO_CC) && (__SUNPRO_CC < 0x5140)
/* https://github.com/harfbuzz/harfbuzz/issues/630 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __restrict
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ALWAYS_INLINE
#ਜੇ defined(_MSC_VER)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ALWAYS_INLINE __forceinline
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ALWAYS_INLINE __attribute__((always_inline)) ਸਤਰਅੰਦਰ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NEVER_INLINE
#ਜੇ defined(_MSC_VER)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NEVER_INLINE __declspec(noinline)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NEVER_INLINE __attribute__((noinline))
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_HOT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_HOT __attribute__((hot))
#ਜੇ_ਅੰਤ

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
#ਜੇ defined(__clang__) && __cplusplus >= 201103L
   /* clang's fallthrough annotations are only available starting in C++11. */
#  define HB_FALLTHROUGH [[clang::fallthrough]]
#ਜਾਂਜੇ defined(__GNUC__) && (__GNUC__ >= 7)
   /* GNU fallthrough attribute is available from GCC7 */
#  define HB_FALLTHROUGH __attribute__((fallthrough))
#ਜਾਂਜੇ defined(_MSC_VER)
   /*
    * MSVC's __fallthrough annotations are checked by /analyze (Code Analysis):
    * https://msdn.microsoft.com/en-us/library/ms235402%28VS.80%29.aspx
    */
#  include <sal.h>
#  define HB_FALLTHROUGH __fallthrough
#ਨਹੀਂਤਾਂ
#  define HB_FALLTHROUGH /* FALLTHROUGH */
#ਜੇ_ਅੰਤ

/* A tag to enforce use of return value for a function */
#ਜੇ __cplusplus >= 201703L
#  define HB_NODISCARD [[nodiscard]]
#ਜਾਂਜੇ defined(__GNUC__) || defined(__clang__)
#  define HB_NODISCARD __attribute__((warn_ਅਣਵਰਤਿਆ_ਨਤੀਜਾ))
#ਜਾਂਜੇ defined(_MSC_VER)
#  define HB_NODISCARD _Check_return_
#ਨਹੀਂਤਾਂ
#  define HB_NODISCARD
#ਜੇ_ਅੰਤ

/* Like HB_NODISCARD but can be used on structs */
#ਜੇ __cplusplus >= 201703L
#  define HB_NODISCARD_STRUCT [[nodiscard]]
#ਨਹੀਂਤਾਂ
#  define HB_NODISCARD_STRUCT
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
   /* We need Windows Vista for both Uniscribe backend and for
    * MemoryBarrier.  We don't support compiling on Windows XP,
    * though we run on it fine. */
#  ਜੇ defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0600
#    undef _WIN32_WINNT
#  endif
#  ifndef _WIN32_WINNT
#    ਜੇ WINAPI_FAMILY_ਹਿੱਸੇ_ਕਰੋ(WINAPI_ਹਿੱਸੇ_ਕਰੋ_DESKTOP)
#      define _WIN32_WINNT 0x0600
#    endif
#  endif
#  ifndef WIN32_LEAN_AND_ਔਸਤ
#    define WIN32_LEAN_AND_ਔਸਤ 1
#  endif
#  ifndef STRICT
#    define STRICT 1
#  endif

#  ਜੇ defined(_WIN32_WCE)
     /* Some things not defined on Windows CE. */
#    define vsnprintf _vsnprintf
#    ifndef HB_NO_GETENV
#      define HB_NO_GETENV
#    endif
#    ਜੇ _WIN32_WCE < 0x800
#      define HB_NO_SETLOCALE
#      define HB_NO_ERRNO
#    endif
#  elif !WINAPI_FAMILY_ਹਿੱਸੇ_ਕਰੋ(WINAPI_ਹਿੱਸੇ_ਕਰੋ_DESKTOP)
#    ifndef HB_NO_GETENV
#      define HB_NO_GETENV
#    endif
#  endif
#  ਜੇ defined(_MSC_VER) && _MSC_VER < 1900
#    define snprintf _snprintf
#  endif
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_GETENV
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ getenv(Name) ਖਾਲੀ_ਸੂਚਕ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ERRNO
#  include <cerrno>
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਪੂਰਨ HB_ਅਣਵਰਤਿਆ _hb_errno = 0;
#  undef errno
#  define errno _hb_errno
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_STMT_ਸ਼ੁਰੂ ਕਰੋ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_STMT_ਸਿਰਾ   ਜਦੋਂ (0)

#ਜੇ defined(HAVE_ATEXIT) && !defined(HB_USE_ATEXIT)
/* atexit() is only safe to be called from shared libraries on certain
 * platforms.  Whitelist.
 * https://bugs.freedesktop.org/show_bug.cgi?id=82246 */
#  ਜੇ defined(__linux) && defined(__GLIBC_PREREQ)
#    ਜੇ __GLIBC_PREREQ(2,3)
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
#ਜੇ_ਅੰਤ /* defined(HAVE_ATEXIT) && !defined(HB_USE_ATEXIT) */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_ATEXIT
#  undef HB_USE_ATEXIT
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_USE_ATEXIT
#  define HB_USE_ATEXIT 0
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ hb_atexit
#ਜੇ !HB_USE_ATEXIT
#  define hb_atexit(_) HB_STMT_ਸ਼ੁਰੂ { ਜੇ (0) (_) (); } HB_STMT_ਸਿਰਾ
#ਨਹੀਂਤਾਂ /* HB_USE_ATEXIT */
#  ifdef HAVE_ATEXIT
#    define hb_atexit atexit
#  ਨਹੀਂਤਾਂ
     ਸਾਂਚਾ <ਖਾਲੀ (*function) (ਖਾਲੀ)> ਬਣਤਰ hb_atexit_t { ~hb_atexit_t () { function (); } };
#    define hb_atexit(f) ਸਥਿਰ hb_atexit_t<f> _hb_atexit_##__LINE__
#  endif
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ


// Locale business

#ਜੇ !defined(HB_NO_SETLOCALE) && (!defined(HAVE_NEWLOCALE) || !defined(HAVE_USELOCALE))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_SETLOCALE 1
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SETLOCALE

#ਅਨ੍ਤਰ੍ਭਾਵ <locale.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_XLOCALE_H
#ਅਨ੍ਤਰ੍ਭਾਵ <xlocale.h> // Needed on BSD/OS X for uselocale
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ WIN32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_locale_t _locale_t
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_locale_t locale_t
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_setlocale setlocale
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_uselocale uselocale

#ਨਹੀਂਤਾਂ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_locale_t ਖਾਲੀ *
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_setlocale(Category, Locale) "C"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_uselocale(Locale) ((hb_locale_t) 0)

#ਜੇ_ਅੰਤ


/* Lets assert int types.  Saves trouble down the road. */
static_assert ((ਆਕਾਰ (hb_codepoint_t) == 4), "");
static_assert ((ਆਕਾਰ (hb_position_t) == 4), "");
static_assert ((ਆਕਾਰ (hb_ਨਕਾਬ_t) == 4), "");
static_assert ((ਆਕਾਰ (hb_var_int_t) == 4), "");


/* Pie time. */
// https://github.com/harfbuzz/harfbuzz/issues/4166
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PI 3.14159265358979323846f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_2_PI (2.f * HB_PI)

/* Compile-time custom allocator support. */

#ਜੇ !defined(HB_CUSTOM_MALLOC) \
  && defined(hb_malloc_impl) \
  && defined(hb_calloc_impl) \
  && defined(hb_realloc_impl) \
  && defined(hb_free_impl)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CUSTOM_MALLOC
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_CUSTOM_MALLOC
ਬਾਹਰੀ "C" ਖਾਲੀ* hb_malloc_impl(size_t ਮਾਪ);
ਬਾਹਰੀ "C" ਖਾਲੀ* hb_calloc_impl(size_t nmemb, size_t ਮਾਪ);
ਬਾਹਰੀ "C" ਖਾਲੀ* hb_realloc_impl(ਖਾਲੀ *ptr, size_t ਮਾਪ);
ਬਾਹਰੀ "C" ਖਾਲੀ  hb_free_impl(ਖਾਲੀ *ptr);
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_malloc_impl ਸਮ੍ਰਿਤੀ_ਦਿਓ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_calloc_impl ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_realloc_impl realloc
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_free_impl ਮੁਕਤ
#ਜੇ_ਅੰਤ



/* Headers we include for everyone.  Keep topologically sorted by dependency.
 * They express dependency amongst themselves, but no other file should include
 * them directly.*/
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-limits.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cplusplus.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-meta.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-mutex.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-number.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-atomic.hh"	// Requires: hb-meta
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-null.hh"	// Requires: hb-meta
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-algs.hh"	// Requires: hb-meta hb-null hb-number
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-iter.hh"	// Requires: hb-algs hb-meta
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-debug.hh"	// Requires: hb-algs hb-atomic
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-array.hh"	// Requires: hb-algs hb-iter hb-null
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector.hh"	// Requires: hb-array hb-null
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-object.hh"	// Requires: hb-atomic hb-mutex hb-vector


/* Library-internal alias for hb::unique_ptr defined in hb-cplusplus.hh.
 * Unique-ownership RAII wrapper around an HB object; destroys on scope exit
 * via the type's hb_*_destroy() function.  Usage: hb_unique_ptr_t<hb_blob_t>.
 * Matches the hb_*_t naming used elsewhere in internal code. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਵਰਤੋਂ hb_unique_ptr_t = hb::unique_ptr<T>;


/* Our src/test-*.cc use hb_assert(), such that it's not compiled out under NDEBUG.
 * https://github.com/harfbuzz/harfbuzz/issues/5418 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_always_assert(x) \
	HB_STMT_ਸ਼ੁਰੂ { \
	  ਜੇ (!(x)) { fprintf(stderr, "Assertion failed: %s, at %s:%d\n", #x, __FILE__, __LINE__); abort(); } \
	} HB_STMT_ਸਿਰਾ


#ਜੇ_ਅੰਤ /* HB_HH */
