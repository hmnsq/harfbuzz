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

#अगर_अघोषित HB_HH
#घोषणा HB_HH

#अगर_अघोषित HB_NO_PRAGMA_GCC_DIAGNOSTIC
#अगर_घोषित _MSC_VER
#आशय warning( disable: 4068 ) /* Unknown pragma */
#पूर्ण_अगर
#अगर defined(__GNUC__) || defined(__clang__)
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
#अगर_अघोषित HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
#आशय GCC diagnostic warning "-Wall"
#आशय GCC diagnostic warning "-Wextra"
#पूर्ण_अगर
#अगर_अघोषित HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#आशय GCC diagnostic ignored "-Wpragmas"
#आशय GCC diagnostic ignored "-Wunknown-pragmas"
#आशय GCC diagnostic ignored "-Wunknown-warning-option"
#पूर्ण_अगर
#अगर_अघोषित HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
//#pragma GCC diagnostic warning "-Weverything"
#पूर्ण_अगर

/* Error.  Should never happen. */
#अगर_अघोषित HB_NO_PRAGMA_GCC_DIAGNOSTIC_ERROR
#आशय GCC diagnostic error   "-Wbitwise-instead-of-logical"
#आशय GCC diagnostic error   "-Wcast-align"
#आशय GCC diagnostic error   "-Wcast-function-type"
#आशय GCC diagnostic error   "-Wcast-function-type-strict"
#आशय GCC diagnostic error   "-Wconstant-conversion"
#आशय GCC diagnostic error   "-Wcomma"
#आशय GCC diagnostic error   "-Wdelete-non-virtual-dtor"
#आशय GCC diagnostic error   "-Wembedded-directive"
#आशय GCC diagnostic error   "-Wextra-semi-stmt"
#आशय GCC diagnostic error   "-Wformat-security"
#आशय GCC diagnostic error   "-Wimplicit-function-declaration"
#आशय GCC diagnostic error   "-Winit-self"
#आशय GCC diagnostic error   "-Winjected-class-name"
#आशय GCC diagnostic error   "-Wmissing-braces"
#आशय GCC diagnostic error   "-Wmissing-declarations"
#आशय GCC diagnostic error   "-Wmissing-prototypes"
#आशय GCC diagnostic error   "-Wnarrowing"
#आशय GCC diagnostic error   "-Wnested-externs"
#आशय GCC diagnostic error   "-Wold-style-definition"
#आशय GCC diagnostic error   "-Wpointer-arith"
#आशय GCC diagnostic error   "-Wredundant-decls"
#आशय GCC diagnostic error   "-Wreorder"
#आशय GCC diagnostic error   "-Wsign-compare"
#आशय GCC diagnostic error   "-Wstrict-flex-arrays"
#आशय GCC diagnostic error   "-Wstrict-prototypes"
#आशय GCC diagnostic error   "-Wstring-conversion"
#आशय GCC diagnostic error   "-Wswitch-enum"
#आशय GCC diagnostic error   "-Wtautological-overlap-compare"
#आशय GCC diagnostic error   "-Wunneeded-internal-declaration"
#आशय GCC diagnostic error   "-Wunused"
#आशय GCC diagnostic error   "-Wunused-local-typedefs"
#आशय GCC diagnostic error   "-Wunused-value"
#आशय GCC diagnostic error   "-Wunused-variable"
#आशय GCC diagnostic error   "-Wvla"
#आशय GCC diagnostic error   "-Wwrite-strings"
#पूर्ण_अगर

/* Warning.  To be investigated if happens. */
#अगर_अघोषित HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
#आशय GCC diagnostic warning "-Wbuiltin-macro-redefined"
#आशय GCC diagnostic warning "-Wdeprecated"
#आशय GCC diagnostic warning "-Wdeprecated-declarations"
#आशय GCC diagnostic warning "-Wdisabled-optimization"
#आशय GCC diagnostic warning "-Wdouble-promotion"
#आशय GCC diagnostic warning "-Wformat=2"
#आशय GCC diagnostic warning "-Wformat-signedness"
#आशय GCC diagnostic warning "-Wignored-pragma-optimize"
#आशय GCC diagnostic warning "-Wlogical-op"
#आशय GCC diagnostic warning "-Wmissing-format-attribute"
#आशय GCC diagnostic warning "-Wpessimizing-move"
#आशय GCC diagnostic warning "-Wundef"
#आशय GCC diagnostic warning "-Wunsafe-loop-optimizations"
#आशय GCC diagnostic warning "-Wunused-but-set-variable"
#अगर_घोषित __clang__
// The following are too buggy on gcc
// https://github.com/harfbuzz/harfbuzz/issues/5589
// https://github.com/harfbuzz/harfbuzz/pull/5367
#आशय GCC diagnostic warning "-Wmaybe-uninitialized"
#आशय GCC diagnostic warning "-Wuninitialized"
#अन्यथा
#आशय GCC diagnostic ignored "-Wmaybe-uninitialized"
#आशय GCC diagnostic ignored "-Wuninitialized"
#पूर्ण_अगर
#पूर्ण_अगर

/* Ignored currently, but should be fixed at some point. */
#अगर_अघोषित HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#आशय GCC diagnostic ignored "-Wconversion"			// TODO fix
#आशय GCC diagnostic ignored "-Wshadow"			// TODO fix
#आशय GCC diagnostic ignored "-Wunused-parameter"		// TODO fix
#अगर defined(__GNUC__) && !defined(__clang__)
#आशय GCC diagnostic ignored "-Wunused-result"		// TODO fix
#पूर्ण_अगर
#पूर्ण_अगर

/* Ignored intentionally. */
#अगर_अघोषित HB_NO_PRAGMA_GCC_DIAGNOSTIC_IGNORED
#आशय GCC diagnostic ignored "-Warray-bounds" // https://github.com/harfbuzz/harfbuzz/issues/5738
#आशय GCC diagnostic ignored "-Wclass-memaccess"
#आशय GCC diagnostic ignored "-Wcast-function-type-strict" // https://github.com/harfbuzz/harfbuzz/pull/3859#issuecomment-1295409126
#आशय GCC diagnostic ignored "-Wdangling-reference" // https://github.com/harfbuzz/harfbuzz/issues/4043
#आशय GCC diagnostic ignored "-Wdangling-pointer" // Trigerred by hb_decycler_node_t().
#आशय GCC diagnostic ignored "-Wformat-nonliteral"
#आशय GCC diagnostic ignored "-Wformat-zero-length"
#आशय GCC diagnostic ignored "-Wmissing-field-initializers"
#आशय GCC diagnostic ignored "-Wold-style-cast"
#आशय GCC diagnostic ignored "-Wpacked" // Erratic impl in clang
#आशय GCC diagnostic ignored "-Wrange-loop-analysis" // https://github.com/harfbuzz/harfbuzz/issues/2834
#आशय GCC diagnostic ignored "-Wstrict-aliasing"
#आशय GCC diagnostic ignored "-Wtype-limits"
#आशय GCC diagnostic ignored "-Wunused-template"
#आशय GCC diagnostic ignored "-Wc++11-compat" // only gcc raises it
#पूर्ण_अगर

#पूर्ण_अगर
#पूर्ण_अगर


#समावेश "hb-config.hh"


/*
 * Following added based on what AC_USE_SYSTEM_EXTENSIONS adds to
 * config.h.in.  Copied here for the convenience of those embedding
 * HarfBuzz and not using our build system.
 */
/* Enable extensions on AIX 3, Interix.  */
#अगर_अघोषित _ALL_SOURCE
# define _ALL_SOURCE 1
#पूर्ण_अगर
/* Enable GNU extensions on systems that have them.  */
#अगर_अघोषित _GNU_SOURCE
# define _GNU_SOURCE 1
#पूर्ण_अगर
/* Enable threading extensions on Solaris.  */
#अगर_अघोषित _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#पूर्ण_अगर
/* Enable extensions on HP NonStop.  */
#अगर_अघोषित _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#पूर्ण_अगर
/* Enable general extensions on Solaris.  */
#अगर_अघोषित __EXTENSIONS__
# define __EXTENSIONS__ 1
#पूर्ण_अगर

#अगर defined (_MSC_VER) && defined (HB_DLL_EXPORT)
#घोषणा HB_EXTERN __declspec (dllexport) बाह्य
#पूर्ण_अगर

// https://github.com/harfbuzz/harfbuzz/pull/4619
#अगर_अघोषित __STDC_FORMAT_MACROS
#घोषणा __STDC_FORMAT_MACROS 1
#पूर्ण_अगर

#समावेश "hb.h"
#घोषणा HB_H_IN
#समावेश "hb-ot.h"
#घोषणा HB_OT_H_IN
#समावेश "hb-aat.h"
#घोषणा HB_AAT_H_IN
#घोषणा HB_SUBSET_H_IN

#समावेश <cassert>
#समावेश <cfloat>
#समावेश <climits>
#अगर defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
# define _USE_MATH_DEFINES
#पूर्ण_अगर
#समावेश <cmath>
#समावेश <cstdarg>
#समावेश <cstddef>
#समावेश <cstdio>
#समावेश <cstdlib>
#समावेश <cstring>

#अगर (defined(_MSC_VER) && _MSC_VER >= 1500) || defined(__MINGW32__)
#अगर_घोषित __MINGW32_VERSION
#अगर_अघोषित WIN32_LEAN_AND_MEAN
#घोषणा WIN32_LEAN_AND_MEAN 1
#पूर्ण_अगर
#अन्यथा
#समावेश <intrin.h>
#पूर्ण_अगर
#पूर्ण_अगर

#अगर_घोषित _WIN32
#समावेश <windows.h>
#समावेश <winapifamily.h>
#पूर्ण_अगर

#अगर_अघोषित PRId32
# define PRId32 "d"
# define PRIu32 "u"
# define PRIx32 "x"
#पूर्ण_अगर

#घोषणा HB_PASTE1(a,b) a##b
#घोषणा HB_PASTE(a,b) HB_PASTE1(a,b)


/*
 * Compiler attributes
 */

// gcc 10 has __has_builtin but not earlier versions. Sanction any gcc >= 5
// clang defines it so no need.
#अगर_घोषित __has_builtin
#घोषणा hb_has_builtin __has_builtin
#या_अगर defined(_MSC_VER)
#घोषणा hb_has_builtin(x) 0
#अन्यथा
#घोषणा hb_has_builtin(x) ((defined(__GNUC__) && __GNUC__ >= 5))
#पूर्ण_अगर

#अगर defined(__OPTIMIZE__) && hb_has_builtin(__builtin_expect)
#घोषणा likely(व्यंजक) __builtin_expect (bool(व्यंजक), 1)
#घोषणा unlikely(व्यंजक) __builtin_expect (bool(व्यंजक), 0)
#अन्यथा
#घोषणा likely(व्यंजक) (व्यंजक)
#घोषणा unlikely(व्यंजक) (व्यंजक)
#पूर्ण_अगर

#अगर !defined(__GNUC__) && !defined(__clang__)
#अघोषित __attribute__
#घोषणा __attribute__(x)
#पूर्ण_अगर

#अगर defined(__MINGW32__) && (__GNUC__ >= 3) && !defined(__clang__)
#घोषणा HB_PRINTF_FUNC(format_idx, arg_idx) __attribute__((__format__ (gnu_printf, format_idx, arg_idx)))
#या_अगर defined(__GNUC__) && (__GNUC__ >= 3)
#घोषणा HB_PRINTF_FUNC(format_idx, arg_idx) __attribute__((__format__ (__printf__, format_idx, arg_idx)))
#अन्यथा
#घोषणा HB_PRINTF_FUNC(format_idx, arg_idx)
#पूर्ण_अगर
#अगर defined(__GNUC__) && (__GNUC__ >= 4) || (__clang__)
#घोषणा HB_UNUSED	__attribute__((unused))
#या_अगर defined(_MSC_VER) /* https://github.com/harfbuzz/harfbuzz/issues/635 */
#घोषणा HB_UNUSED __pragma(warning(suppress: 4100 4101 4189))
#अन्यथा
#घोषणा HB_UNUSED
#पूर्ण_अगर

#अगर_अघोषित HB_INTERNAL
# अगर !defined(HB_NO_VISIBILITY) && !defined(__MINGW32__) && !defined(__CYGWIN__) && !defined(_MSC_VER) && !defined(__SUNPRO_CC)
#  define HB_INTERNAL __attribute__((__visibility__("hidden")))
# elif defined(__MINGW32__)
   /* We use -export-symbols on mingw32, since it does not support visibility attributes. */
#  define HB_INTERNAL
# elif defined (_MSC_VER) && defined (HB_DLL_EXPORT)
   /* We do not try to export internal symbols on Visual Studio */
#  define HB_INTERNAL
#अन्यथा
#  define HB_INTERNAL
#  define HB_NO_VISIBILITY 1
# endif
#पूर्ण_अगर

/* https://github.com/harfbuzz/harfbuzz/issues/1651 */
#अगर defined(__clang__) && __clang_major__ < 10
#घोषणा static_const अटल
#अन्यथा
#घोषणा static_const अटल स्थिर
#पूर्ण_अगर

#अगर defined(__GNUC__) && (__GNUC__ >= 3)
#घोषणा HB_FUNC __PRETTY_FUNCTION__
#या_अगर defined(_MSC_VER)
#घोषणा HB_FUNC __FUNCSIG__
#अन्यथा
#घोषणा HB_FUNC __func__
#पूर्ण_अगर

#अगर defined(__SUNPRO_CC) && (__SUNPRO_CC < 0x5140)
/* https://github.com/harfbuzz/harfbuzz/issues/630 */
#घोषणा __restrict
#पूर्ण_अगर

#अगर_अघोषित HB_ALWAYS_INLINE
#अगर defined(_MSC_VER)
#घोषणा HB_ALWAYS_INLINE __forceinline
#अन्यथा
#घोषणा HB_ALWAYS_INLINE __attribute__((always_inline)) अंतरभूत
#पूर्ण_अगर
#पूर्ण_अगर

#अगर_अघोषित HB_NEVER_INLINE
#अगर defined(_MSC_VER)
#घोषणा HB_NEVER_INLINE __declspec(noinline)
#अन्यथा
#घोषणा HB_NEVER_INLINE __attribute__((noinline))
#पूर्ण_अगर
#पूर्ण_अगर

#अगर_अघोषित HB_HOT
#घोषणा HB_HOT __attribute__((hot))
#पूर्ण_अगर

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
#अगर defined(__clang__) && __cplusplus >= 201103L
   /* clang's fallthrough annotations are only available starting in C++11. */
#  define HB_FALLTHROUGH [[clang::fallthrough]]
#या_अगर defined(__GNUC__) && (__GNUC__ >= 7)
   /* GNU fallthrough attribute is available from GCC7 */
#  define HB_FALLTHROUGH __attribute__((fallthrough))
#या_अगर defined(_MSC_VER)
   /*
    * MSVC's __fallthrough annotations are checked by /analyze (Code Analysis):
    * https://msdn.microsoft.com/en-us/library/ms235402%28VS.80%29.aspx
    */
#  include <sal.h>
#  define HB_FALLTHROUGH __fallthrough
#अन्यथा
#  define HB_FALLTHROUGH /* FALLTHROUGH */
#पूर्ण_अगर

/* A tag to enforce use of return value for a function */
#अगर __cplusplus >= 201703L
#  define HB_NODISCARD [[nodiscard]]
#या_अगर defined(__GNUC__) || defined(__clang__)
#  define HB_NODISCARD __attribute__((warn_unused_परिणाम))
#या_अगर defined(_MSC_VER)
#  define HB_NODISCARD _Check_return_
#अन्यथा
#  define HB_NODISCARD
#पूर्ण_अगर

/* Like HB_NODISCARD but can be used on structs */
#अगर __cplusplus >= 201703L
#  define HB_NODISCARD_STRUCT [[nodiscard]]
#अन्यथा
#  define HB_NODISCARD_STRUCT
#पूर्ण_अगर

#अगर_घोषित _WIN32
   /* We need Windows Vista for both Uniscribe backend and for
    * MemoryBarrier.  We don't support compiling on Windows XP,
    * though we run on it fine. */
#  अगर defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0600
#    undef _WIN32_WINNT
#  endif
#  ifndef _WIN32_WINNT
#    अगर WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#      define _WIN32_WINNT 0x0600
#    endif
#  endif
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN 1
#  endif
#  ifndef STRICT
#    define STRICT 1
#  endif

#  अगर defined(_WIN32_WCE)
     /* Some things not defined on Windows CE. */
#    define vsnprintf _vsnprintf
#    ifndef HB_NO_GETENV
#      define HB_NO_GETENV
#    endif
#    अगर _WIN32_WCE < 0x800
#      define HB_NO_SETLOCALE
#      define HB_NO_ERRNO
#    endif
#  elif !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#    ifndef HB_NO_GETENV
#      define HB_NO_GETENV
#    endif
#  endif
#  अगर defined(_MSC_VER) && _MSC_VER < 1900
#    define snprintf _snprintf
#  endif
#पूर्ण_अगर

#अगर_घोषित HB_NO_GETENV
#घोषणा दो_पर्या(Name) nullptr
#पूर्ण_अगर

#अगर_अघोषित HB_NO_ERRNO
#  include <cerrno>
#अन्यथा
अटल पूर्णांक HB_UNUSED _hb_errno = 0;
#  undef त्रुटि_सं
#  define त्रुटि_सं _hb_errno
#पूर्ण_अगर

#घोषणा HB_STMT_START करो
#घोषणा HB_STMT_END   जबतक (0)

#अगर defined(HAVE_ATEXIT) && !defined(HB_USE_ATEXIT)
/* atexit() is only safe to be called from shared libraries on certain
 * platforms.  Whitelist.
 * https://bugs.freedesktop.org/show_bug.cgi?id=82246 */
#  अगर defined(__linux) && defined(__GLIBC_PREREQ)
#    अगर __GLIBC_PREREQ(2,3)
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
#पूर्ण_अगर /* defined(HAVE_ATEXIT) && !defined(HB_USE_ATEXIT) */
#अगर_घोषित HB_NO_ATEXIT
#  undef HB_USE_ATEXIT
#पूर्ण_अगर
#अगर_अघोषित HB_USE_ATEXIT
#  define HB_USE_ATEXIT 0
#पूर्ण_अगर
#अगर_अघोषित hb_atexit
#अगर !HB_USE_ATEXIT
#  define hb_atexit(_) HB_STMT_START { अगर (0) (_) (); } HB_STMT_END
#अन्यथा /* HB_USE_ATEXIT */
#  ifdef HAVE_ATEXIT
#    define hb_atexit निकास_पर
#  अन्यथा
     ढाँचा <व्योम (*function) (व्योम)> काष्ठा hb_atexit_t { ~hb_atexit_t () { function (); } };
#    define hb_atexit(f) अटल hb_atexit_t<f> _hb_atexit_##__LINE__
#  endif
#पूर्ण_अगर
#पूर्ण_अगर


// Locale business

#अगर !defined(HB_NO_SETLOCALE) && (!defined(HAVE_NEWLOCALE) || !defined(HAVE_USELOCALE))
#घोषणा HB_NO_SETLOCALE 1
#पूर्ण_अगर

#अगर_अघोषित HB_NO_SETLOCALE

#समावेश <locale.h>
#अगर_घोषित HAVE_XLOCALE_H
#समावेश <xlocale.h> // Needed on BSD/OS X for uselocale
#पूर्ण_अगर

#अगर_घोषित WIN32
#घोषणा hb_locale_t _locale_t
#अन्यथा
#घोषणा hb_locale_t locale_t
#पूर्ण_अगर
#घोषणा hb_setlocale रखो_क्षेत्र
#घोषणा hb_uselocale uselocale

#अन्यथा

#घोषणा hb_locale_t व्योम *
#घोषणा hb_setlocale(Category, Locale) "C"
#घोषणा hb_uselocale(Locale) ((hb_locale_t) 0)

#पूर्ण_अगर


/* Lets assert int types.  Saves trouble down the road. */
static_assert ((माप (hb_codepoint_t) == 4), "");
static_assert ((माप (hb_position_t) == 4), "");
static_assert ((माप (hb_mask_t) == 4), "");
static_assert ((माप (hb_var_int_t) == 4), "");


/* Pie time. */
// https://github.com/harfbuzz/harfbuzz/issues/4166
#घोषणा HB_PI 3.14159265358979323846f
#घोषणा HB_2_PI (2.f * HB_PI)

/* Compile-time custom allocator support. */

#अगर !defined(HB_CUSTOM_MALLOC) \
  && defined(hb_malloc_impl) \
  && defined(hb_calloc_impl) \
  && defined(hb_realloc_impl) \
  && defined(hb_free_impl)
#घोषणा HB_CUSTOM_MALLOC
#पूर्ण_अगर

#अगर_घोषित HB_CUSTOM_MALLOC
बाह्य "C" व्योम* hb_malloc_impl(माप_प्रकार आकार);
बाह्य "C" व्योम* hb_calloc_impl(माप_प्रकार nmemb, माप_प्रकार आकार);
बाह्य "C" व्योम* hb_realloc_impl(व्योम *ptr, माप_प्रकार आकार);
बाह्य "C" व्योम  hb_free_impl(व्योम *ptr);
#अन्यथा
#घोषणा hb_malloc_impl दो_स्मृति
#घोषणा hb_calloc_impl सुस्मृति
#घोषणा hb_realloc_impl पुनः_स्मृति
#घोषणा hb_free_impl मुक्त
#पूर्ण_अगर



/* Headers we include for everyone.  Keep topologically sorted by dependency.
 * They express dependency amongst themselves, but no other file should include
 * them directly.*/
#समावेश "hb-limits.hh"
#समावेश "hb-cplusplus.hh"
#समावेश "hb-meta.hh"
#समावेश "hb-mutex.hh"
#समावेश "hb-number.hh"
#समावेश "hb-atomic.hh"	// Requires: hb-meta
#समावेश "hb-null.hh"	// Requires: hb-meta
#समावेश "hb-algs.hh"	// Requires: hb-meta hb-null hb-number
#समावेश "hb-iter.hh"	// Requires: hb-algs hb-meta
#समावेश "hb-debug.hh"	// Requires: hb-algs hb-atomic
#समावेश "hb-array.hh"	// Requires: hb-algs hb-iter hb-null
#समावेश "hb-vector.hh"	// Requires: hb-array hb-null
#समावेश "hb-object.hh"	// Requires: hb-atomic hb-mutex hb-vector


/* Library-internal alias for hb::unique_ptr defined in hb-cplusplus.hh.
 * Unique-ownership RAII wrapper around an HB object; destroys on scope exit
 * via the type's hb_*_destroy() function.  Usage: hb_unique_ptr_t<hb_blob_t>.
 * Matches the hb_*_t naming used elsewhere in internal code. */
ढाँचा <typename T> using hb_unique_ptr_t = hb::unique_ptr<T>;


/* Our src/test-*.cc use hb_assert(), such that it's not compiled out under NDEBUG.
 * https://github.com/harfbuzz/harfbuzz/issues/5418 */
#घोषणा hb_always_assert(x) \
	HB_STMT_START { \
	  अगर (!(x)) { ख_लिखो(मानक_त्रुटि, "Assertion failed: %s, at %s:%d\n", #x, __खाता__, __LINE__); पात(); } \
	} HB_STMT_END


#पूर्ण_अगर /* HB_HH */
