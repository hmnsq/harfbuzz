/*
 * Copyright © 2017  Google, Inc.
 * Copyright © 2019  Facebook, Inc.
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
 * Google Author(s): Behdad Esfahbod
 * Facebook Author(s): Behdad Esfahbod
 */

#ifndef HB_ALGS_HH
#define HB_ALGS_HH

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-meta.hh"
#ಅನ್ತರ್ಭಾವ "hb-null.hh"
#ಅನ್ತರ್ಭಾವ "hb-number.hh"

#ಅನ್ತರ್ಭಾವ <algorithm>
#ಅನ್ತರ್ಭಾವ <initializer_list>
#ಅನ್ತರ್ಭಾವ <functional>
#ಅನ್ತರ್ಭಾವ <new>

ಸ್ಥಿರ HB_ALWAYS_INLINE ಶೂನ್ಯ *
hb_malloc2 (size_t nmemb, size_t size)
{
  ಯದಿ (size && nmemb > SIZE_MAX / size) ಪ್ರತ್ಯಾಗಮ nullptr;
  ಪ್ರತ್ಯಾಗಮ hb_malloc (nmemb * size);
}
ಸ್ಥಿರ HB_ALWAYS_INLINE ಶೂನ್ಯ *
hb_realloc2 (ಶೂನ್ಯ *ptr, size_t nmemb, size_t size)
{
  ಯದಿ (size && nmemb > SIZE_MAX / size) ಪ್ರತ್ಯಾಗಮ nullptr;
  ಪ್ರತ್ಯಾಗಮ hb_realloc (ptr, nmemb * size);
}

/*
 * Flags
 */

/* Enable bitwise ops on enums marked as flags_t */
/* To my surprise, looks like the function resolver is happy to silently cast
 * one enum to another...  So this doesn't provide the type-checking that I
 * originally had in mind... :(.
 *
 * For MSVC warnings, see: https://github.com/harfbuzz/harfbuzz/pull/163
 */
#ifdef _MSC_VER
# pragma warning(disable:4200)
# pragma warning(disable:4800)
#endif
#define HB_MARK_AS_FLAG_T(T) \
	ಬಾಹ್ಯ "C++" { \
	  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ constexpr T operator | (T l, T r) { ಪ್ರತ್ಯಾಗಮ T ((unsigned) l | (unsigned) r); } \
	  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ constexpr T operator & (T l, T r) { ಪ್ರತ್ಯಾಗಮ T ((unsigned) l & (unsigned) r); } \
	  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ constexpr T operator ^ (T l, T r) { ಪ್ರತ್ಯಾಗಮ T ((unsigned) l ^ (unsigned) r); } \
	  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ constexpr unsigned operator ~ (T r) { ಪ್ರತ್ಯಾಗಮ (~(unsigned) r); } \
	  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ T& operator |= (T &l, T r) { l = l | r; ಪ್ರತ್ಯಾಗಮ l; } \
	  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ T& operator &= (T& l, T r) { l = l & r; ಪ್ರತ್ಯಾಗಮ l; } \
	  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ T& operator ^= (T& l, T r) { l = l ^ r; ಪ್ರತ್ಯಾಗಮ l; } \
	} \
	static_assert (ಸತ್ಯ, "")

/* Useful for set-operations on small enums.
 * For example, for testing "x ∈ {x1, x2, x3}" use:
 * (FLAG_UNSAFE(x) & (FLAG(x1) | FLAG(x2) | FLAG(x3)))
 */
#define FLAG(x) (static_assert_ಅಭಿವ್ಯಕ್ತಿ ((unsigned)(x) < 32) + (((uint32_t) 1U) << (unsigned)(x)))
#define FLAG_UNSAFE(x) ((unsigned)(x) < 32 ? (((uint32_t) 1U) << (unsigned)(x)) : 0)
#define FLAG_RANGE(x,y) (static_assert_ಅಭಿವ್ಯಕ್ತಿ ((x) < (y)) + FLAG(y+1) - FLAG(x))
#define FLAG64(x) (static_assert_ಅಭಿವ್ಯಕ್ತಿ ((unsigned)(x) < 64) + (((uint64_t) 1ULL) << (unsigned)(x)))
#define FLAG64_UNSAFE(x) ((unsigned)(x) < 64 ? (((uint64_t) 1ULL) << (unsigned)(x)) : 0)


/*
 * Fixed-endian integers / floats.
 */


/* Endian swap, used in Windows related backends */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ constexpr uint16_t hb_uint16_swap (uint16_t v)
{ ಪ್ರತ್ಯಾಗಮ (v >> 8) | (v << 8); }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ constexpr uint32_t hb_uint32_swap (uint32_t v)
{ ಪ್ರತ್ಯಾಗಮ (hb_uint16_swap (v) << 16) | hb_uint16_swap (v >> 16); }

template <typename Type>
ಸಂರಚನಾ __attribute__((packed)) hb_packed_t
{
  hb_packed_t () = ಸ್ವಭಾವ;
  constexpr hb_packed_t (Type V) : v (V) {}
  operator Type () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v; }
  hb_packed_t & operator = (Type V) { v = V; ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

  ಗುಪ್ತ:
  Type v;
};

#ifndef HB_FAST_ಅಂಕೆ_ACCESS

#ಯದಿ defined(__OPTIMIZE__) && \
    defined(__BYTE_ORDER) && \
    (__BYTE_ORDER == __BIG_ENDIAN || \
     (__BYTE_ORDER == __LITTLE_ENDIAN && \
      hb_has_builtin(__builtin_bswap16) && \
      hb_has_builtin(__builtin_bswap32) && \
      hb_has_builtin(__builtin_bswap64)))
#define HB_FAST_ಅಂಕೆ_ACCESS 1
#ಅನ್ಯಥಾ
#define HB_FAST_ಅಂಕೆ_ACCESS 0
#endif

// https://github.com/harfbuzz/harfbuzz/issues/5456
#ಯದಿ defined(__GNUC__) && !defined(__clang__) && (__GNUC__ <= 12)
#undef HB_FAST_ಅಂಕೆ_ACCESS
#define HB_FAST_ಅಂಕೆ_ACCESS 0
#endif

#endif

template <ತಾರ್ಕಿಕ BE, typename Type, ಪೂರ್ಣಾಙ್ಕ Bytes = ಪರಿಮಾಣ (Type)>
ಸಂರಚನಾ HBInt;
template <ತಾರ್ಕಿಕ BE, typename Type>
ಸಂರಚನಾ HBInt<BE, Type, 1>
{
  ಸಾರ್ವಜನಿಕ:
  HBInt () = ಸ್ವಭಾವ;
  constexpr HBInt (Type V) : v {uint8_t (V)} {}
  constexpr operator Type () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v; }
  ಗುಪ್ತ: uint8_t v;
};
template <ತಾರ್ಕಿಕ BE, typename Type>
ಸಂರಚನಾ HBInt<BE, Type, 2>
{
  ಸಾರ್ವಜನಿಕ:
  HBInt () = ಸ್ವಭಾವ;

  HBInt (Type V)
#ಯದಿ HB_FAST_ಅಂಕೆ_ACCESS
  {
    ಯದಿ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint16_t> *) v) = V;
    ಅನ್ಯಥಾ
      *((hb_packed_t<uint16_t> *) v) = __builtin_bswap16 (V);
  }
#ಅನ್ಯಥಾ
    : v {BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >>  8) & 0xFF)} {}
#endif

  constexpr operator Type () ಧ್ರುವ
  {
#ಯದಿ HB_FAST_ಅಂಕೆ_ACCESS
    ಪ್ರತ್ಯಾಗಮ (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint16_t) *((ಧ್ರುವ hb_packed_t<uint16_t> *) v)
    :
      __builtin_bswap16 ((uint16_t) *((ಧ್ರುವ hb_packed_t<uint16_t> *) v))
    ;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ (BE ? (v[0] <<  8) : (v[0]      ))
	 + (BE ? (v[1]      ) : (v[1] <<  8));
#endif
  }
  ಗುಪ್ತ: uint8_t v[2];
};
template <ತಾರ್ಕಿಕ BE, typename Type>
ಸಂರಚನಾ HBInt<BE, Type, 3>
{
  static_assert (!std::is_signed<Type>::value, "");
  ಸಾರ್ವಜನಿಕ:
  HBInt () = ಸ್ವಭಾವ;
  constexpr HBInt (Type V) : v {BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
				BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
				BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V      ) & 0xFF)} {}

  constexpr operator Type () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (BE ? (v[0] << 16) : (v[0]      ))
					  + (BE ? (v[1] <<  8) : (v[1] <<  8))
					  + (BE ? (v[2]      ) : (v[2] << 16)); }
  ಗುಪ್ತ: uint8_t v[3];
};
template <ತಾರ್ಕಿಕ BE, typename Type>
ಸಂರಚನಾ HBInt<BE, Type, 4>
{
  template <ತಾರ್ಕಿಕ, typename, ಪೂರ್ಣಾಙ್ಕ>
  friend ಸಂರಚನಾ HBFloat;

  ಸಾರ್ವಜನಿಕ:
  HBInt () = ಸ್ವಭಾವ;

  HBInt (Type V)
#ಯದಿ HB_FAST_ಅಂಕೆ_ACCESS
  {
    ಯದಿ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint32_t> *) v) = V;
    ಅನ್ಯಥಾ
      *((hb_packed_t<uint32_t> *) v) = __builtin_bswap32 (V);
  }
#ಅನ್ಯಥಾ
    : v {BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 24) & 0xFF)} {}
#endif

  constexpr operator Type () ಧ್ರುವ {
#ಯದಿ HB_FAST_ಅಂಕೆ_ACCESS
    ಪ್ರತ್ಯಾಗಮ (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint32_t) *((ಧ್ರುವ hb_packed_t<uint32_t> *) v)
    :
      __builtin_bswap32 ((uint32_t) *((ಧ್ರುವ hb_packed_t<uint32_t> *) v))
    ;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ (BE ? (uint32_t (v[0]) << 24) : (uint32_t (v[0])      ))
	 + (BE ? (uint32_t (v[1]) << 16) : (uint32_t (v[1]) <<  8))
	 + (BE ? (uint32_t (v[2]) <<  8) : (uint32_t (v[2]) << 16))
	 + (BE ? (uint32_t (v[3])      ) : (uint32_t (v[3]) << 24));
#endif
  }
  ಗುಪ್ತ: uint8_t v[4];
};
template <ತಾರ್ಕಿಕ BE, typename Type>
ಸಂರಚನಾ HBInt<BE, Type, 8>
{
  template <ತಾರ್ಕಿಕ, typename, ಪೂರ್ಣಾಙ್ಕ>
  friend ಸಂರಚನಾ HBFloat;

  ಸಾರ್ವಜನಿಕ:
  HBInt () = ಸ್ವಭಾವ;

  HBInt (Type V)
#ಯದಿ HB_FAST_ಅಂಕೆ_ACCESS
  {
    ಯದಿ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint64_t> *) v) = V;
    ಅನ್ಯಥಾ
      *((hb_packed_t<uint64_t> *) v) = __builtin_bswap64 (V);
  }
#ಅನ್ಯಥಾ
    : v {BE ? uint8_t ((V >> 56) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 48) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >> 40) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V >> 32) & 0xFF) : uint8_t ((V >> 24) & 0xFF),
	 BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V >> 32) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 40) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 48) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 56) & 0xFF)} {}
#endif

  constexpr operator Type () ಧ್ರುವ {
#ಯದಿ HB_FAST_ಅಂಕೆ_ACCESS
    ಪ್ರತ್ಯಾಗಮ (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint64_t) *((ಧ್ರುವ hb_packed_t<uint64_t> *) v)
    :
      __builtin_bswap64 ((uint64_t) *((ಧ್ರುವ hb_packed_t<uint64_t> *) v))
    ;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ (BE ? (uint64_t (v[0]) << 56) : (uint64_t (v[0])      ))
	 + (BE ? (uint64_t (v[1]) << 48) : (uint64_t (v[1]) <<  8))
	 + (BE ? (uint64_t (v[2]) << 40) : (uint64_t (v[2]) << 16))
	 + (BE ? (uint64_t (v[3]) << 32) : (uint64_t (v[3]) << 24))
	 + (BE ? (uint64_t (v[4]) << 24) : (uint64_t (v[4]) << 32))
	 + (BE ? (uint64_t (v[5]) << 16) : (uint64_t (v[5]) << 40))
	 + (BE ? (uint64_t (v[6]) <<  8) : (uint64_t (v[6]) << 48))
	 + (BE ? (uint64_t (v[7])      ) : (uint64_t (v[7]) << 56));
#endif
  }
  ಗುಪ್ತ: uint8_t v[8];
};

/* Floats. */

template <ತಾರ್ಕಿಕ BE, typename Type, ಪೂರ್ಣಾಙ್ಕ Bytes>
ಸಂರಚನಾ HBFloat
{
  using IntType = typename std::conditional<Bytes == 4, uint32_t, uint64_t>::type;

  ಸಾರ್ವಜನಿಕ:
  HBFloat () = ಸ್ವಭಾವ;

  HBFloat (Type V)
  {
#ಯದಿ HB_FAST_ಅಂಕೆ_ACCESS
    {
	      ಯದಿ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
	      {
	        *((hb_packed_t<Type> *) v) = V;
	        ಪ್ರತ್ಯಾಗಮ;
	      }
	    }
#endif

    ಸಙ್ಘ {
      hb_packed_t<Type> f;
      hb_packed_t<IntType> i;
    } u = {{V}};

    ಧ್ರುವ HBInt<BE, IntType> I = (IntType) u.i;
    ಪ್ರತಿ (unsigned i = 0; i < Bytes; i++)
      v[i] = I.v[i];
  }

  /* c++14 constexpr */ operator Type () ಧ್ರುವ
  {
#ಯದಿ HB_FAST_ಅಂಕೆ_ACCESS
	    {
	      ಯದಿ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
		ಪ್ರತ್ಯಾಗಮ (Type) *((ಧ್ರುವ hb_packed_t<Type> *) v);
	    }
#endif

    HBInt<BE, IntType> I;
    ಪ್ರತಿ (unsigned i = 0; i < Bytes; i++)
      I.v[i] = v[i];

    ಸಙ್ಘ {
      hb_packed_t<IntType> i;
      hb_packed_t<Type> f;
    } u = {{I}};

    ಪ್ರತ್ಯಾಗಮ (Type) u.f;
  }
  ಗುಪ್ತ: uint8_t v[Bytes];
};


/* We want our rounding towards +infinity. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ
_hb_roundf (ದ್ವಿಗುಣ x) { ಪ್ರತ್ಯಾಗಮ floor (x + .5); }

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಭಿನ್ನ
_hb_roundf (ಭಿನ್ನ x) { ಪ್ರತ್ಯಾಗಮ floor ((ದ್ವಿಗುಣ) x + .5); }

#define roundf(x) _hb_roundf(x)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_sincos (ಭಿನ್ನ rotation, ಭಿನ್ನ &s, ಭಿನ್ನ &c)
{
#ifdef HAVE_SINCOSF
  sincosf (rotation, &s, &c);
#ಅನ್ಯಥಾ
  c = cosf (rotation);
  s = sinf (rotation);
#endif
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_sincos (ದ್ವಿಗುಣ rotation, ದ್ವಿಗುಣ &s, ದ್ವಿಗುಣ &c)
{
#ifdef HAVE_SINCOS
  sincos (rotation, &s, &c);
#ಅನ್ಯಥಾ
  c = cos (rotation);
  s = sin (rotation);
#endif
}


/* Encodes three unsigned integers in one 64-bit number.  If the inputs have more than 21 bits,
 * values will be truncated / overlap, and might not decode exactly. */
#define HB_CODEPOINT_ENCODE3(x,y,z) (((uint64_t) (x) << 42) | ((uint64_t) (y) << 21) | (uint64_t) (z))
#define HB_CODEPOINT_DECODE3_1(v) ((hb_codepoint_t) ((v) >> 42))
#define HB_CODEPOINT_DECODE3_2(v) ((hb_codepoint_t) ((v) >> 21) & 0x1FFFFFu)
#define HB_CODEPOINT_DECODE3_3(v) ((hb_codepoint_t) (v) & 0x1FFFFFu)

/* Custom encoding used by hb-ucd. */
#define HB_CODEPOINT_ENCODE3_11_7_14(x,y,z) (((uint32_t) ((x) & 0x07FFu) << 21) | (((uint32_t) (y) & 0x007Fu) << 14) | (uint32_t) ((z) & 0x3FFFu))
#define HB_CODEPOINT_DECODE3_11_7_14_1(v) ((hb_codepoint_t) ((v) >> 21))
#define HB_CODEPOINT_DECODE3_11_7_14_2(v) ((hb_codepoint_t) (((v) >> 14) & 0x007Fu) | 0x0300)
#define HB_CODEPOINT_DECODE3_11_7_14_3(v) ((hb_codepoint_t) (v) & 0x3FFFu)


ಸಂರಚನಾ
{
  /* Note.  This is dangerous in that if it's passed an rvalue, it returns rvalue-reference. */
  template <typename T> constexpr auto
  operator () (T&& v) ಧ್ರುವ HB_AUTO_RETURN ( std::forward<T> (v) )
}
HB_FUNCOBJ (hb_identity);
ಸಂರಚನಾ
{
  /* Like identity(), but only retains lvalue-references.  Rvalues are returned as rvalues. */
  template <typename T> constexpr T&
  operator () (T& v) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v; }

  template <typename T> constexpr hb_remove_reference<T>
  operator () (T&& v) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v; }
}
HB_FUNCOBJ (hb_lidentity);
ಸಂರಚನಾ
{
  /* Like identity(), but always returns rvalue. */
  template <typename T> constexpr hb_remove_reference<T>
  operator () (T&& v) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v; }
}
HB_FUNCOBJ (hb_ridentity);

ಸಂರಚನಾ
{
  template <typename T> constexpr ತಾರ್ಕಿಕ
  operator () (T&& v) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ತಾರ್ಕಿಕ (std::forward<T> (v)); }
}
HB_FUNCOBJ (hb_bool);


/* The MIT License

   Copyright (C) 2012 Zilong Tan (eric.zltan@gmail.com)

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without
   restriction, including without limitation the rights to use, copy,
   modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/


// Compression function for Merkle-Damgard construction.
// This function is generated using the framework provided.
#define fasthash_mix(h) (					\
			(ಶೂನ್ಯ) ((h) ^= (h) >> 23),		\
			(ಶೂನ್ಯ) ((h) *= 0x2127599bf4325c37ULL),	\
			(h) ^= (h) >> 47)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint64_t fasthash64(ಧ್ರುವ ಶೂನ್ಯ *buf, size_t len, uint64_t seed)
{
	ಸಂರಚನಾ __attribute__((packed)) packed_uint64_t { uint64_t v; };
	ಧ್ರುವ uint64_t    m = 0x880355f21e6d1965ULL;
	ಧ್ರುವ packed_uint64_t *pos = (ಧ್ರುವ packed_uint64_t *)buf;
	ಧ್ರುವ packed_uint64_t *end = pos + (len / 8);
	ಧ್ರುವ unsigned ವರ್ಣ *pos2;
	uint64_t h = seed ^ (len * m);
	uint64_t v;

#ifndef HB_OPTIMIZE_SIZE
	ಯದಿ (((uintptr_t) pos & 7) == 0)
	{
	  ಯಾವತ್ (pos != end)
	  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
	    v  = * (ಧ್ರುವ uint64_t *) (pos++);
#pragma GCC diagnostic ತೆಗೆ
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}
	ಅನ್ಯಥಾ
#endif
	{
	  ಯಾವತ್ (pos != end)
	  {
	    v  = pos++->v;
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}

	pos2 = (ಧ್ರುವ unsigned ವರ್ಣ*)pos;
	v = 0;

	ವಿಕಲ್ಪ (len & 7) {
	ಪಕ್ಷ 7: v ^= (uint64_t)pos2[6] << 48; HB_FALLTHROUGH;
	ಪಕ್ಷ 6: v ^= (uint64_t)pos2[5] << 40; HB_FALLTHROUGH;
	ಪಕ್ಷ 5: v ^= (uint64_t)pos2[4] << 32; HB_FALLTHROUGH;
	ಪಕ್ಷ 4: v ^= (uint64_t)pos2[3] << 24; HB_FALLTHROUGH;
	ಪಕ್ಷ 3: v ^= (uint64_t)pos2[2] << 16; HB_FALLTHROUGH;
	ಪಕ್ಷ 2: v ^= (uint64_t)pos2[1] <<  8; HB_FALLTHROUGH;
	ಪಕ್ಷ 1: v ^= (uint64_t)pos2[0];
		h ^= fasthash_mix(v);
		h *= m;
	}

	ಪ್ರತ್ಯಾಗಮ fasthash_mix(h);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint32_t fasthash32(ಧ್ರುವ ಶೂನ್ಯ *buf, size_t len, uint32_t seed)
{
	// the following trick converts the 64-bit hashcode to Fermat
	// residue, which shall retain information from both the higher
	// and lower parts of hashcode.
        uint64_t h = fasthash64(buf, len, seed);
	ಪ್ರತ್ಯಾಗಮ h - (h >> 32);
}

ಸಂರಚನಾ
{
  ಗುಪ್ತ:

  template <typename T> constexpr auto
  impl (ಧ್ರುವ T& v, hb_priority<2>) ಧ್ರುವ HB_RETURN (uint32_t, hb_deref (v).hash ())

  // Horrible: std:hash() of integers seems to be identity in gcc / clang?!
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  //
  // For performance characteristics see:
  // https://github.com/harfbuzz/harfbuzz/pull/4228#issuecomment-1565079537
  template <typename T,
	    hb_enable_if (std::is_integral<T>::value && ಪರಿಮಾಣ (T) <= ಪರಿಮಾಣ (uint32_t))> constexpr auto
  impl (ಧ್ರುವ T& v, hb_priority<1>) ಧ್ರುವ HB_RETURN (uint32_t, (uint32_t) v * 2654435761u /* Knuh's multiplicative hash */)
  template <typename T,
	    hb_enable_if (std::is_integral<T>::value && ಪರಿಮಾಣ (T) > ಪರಿಮಾಣ (uint32_t))> constexpr auto
  impl (ಧ್ರುವ T& v, hb_priority<1>) ಧ್ರುವ HB_RETURN (uint32_t, (uint32_t) (v ^ (v >> 32)) * 2654435761u /* Knuth's multiplicative hash */)

  template <typename T,
	    hb_enable_if (std::is_floating_point<T>::value)> constexpr auto
  impl (ಧ್ರುವ T& v, hb_priority<1>) ಧ್ರುವ HB_RETURN (uint32_t, fasthash32 (std::addressof (v), ಪರಿಮಾಣ (T), 0xf437ffe6))

  template <typename T> constexpr auto
  impl (ಧ್ರುವ T& v, hb_priority<0>) ಧ್ರುವ HB_RETURN (uint32_t, std::hash<hb_decay<decltype (hb_deref (v))>>{} (hb_deref (v)))

  ಸಾರ್ವಜನಿಕ:

  template <typename T> constexpr auto
  operator () (ಧ್ರುವ T& v) ಧ್ರುವ HB_RETURN (uint32_t, impl (v, hb_prioritize))
}
HB_FUNCOBJ (hb_hash);


ಸಂರಚನಾ
{
  ಗುಪ್ತ:

  /* Pointer-to-member-function. */
  template <typename Appl, typename T, typename ...Ts> auto
  impl (Appl&& a, hb_priority<2>, T &&v, Ts&&... ds) ಧ್ರುವ HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v)).*std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  /* Pointer-to-member. */
  template <typename Appl, typename T> auto
  impl (Appl&& a, hb_priority<1>, T &&v) ಧ್ರುವ HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v))).*std::forward<Appl> (a))

  /* Operator(). */
  template <typename Appl, typename ...Ts> auto
  impl (Appl&& a, hb_priority<0>, Ts&&... ds) ಧ್ರುವ HB_AUTO_RETURN
  (hb_deref (std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  ಸಾರ್ವಜನಿಕ:

  template <typename Appl, typename ...Ts> auto
  operator () (Appl&& a, Ts&&... ds) ಧ್ರುವ HB_AUTO_RETURN
  (
    impl (std::forward<Appl> (a),
	  hb_prioritize,
	  std::forward<Ts> (ds)...)
  )
}
HB_FUNCOBJ (hb_invoke);

template <unsigned Pos, typename Appl, typename V>
ಸಂರಚನಾ hb_partial_t
{
  hb_partial_t (Appl a, V v) : a (a), v (v) {}

  static_assert (Pos > 0, "");

  template <typename ...Ts,
	    unsigned P = Pos,
	    hb_enable_if (P == 1)> auto
  operator () (Ts&& ...ds) -> decltype (hb_invoke (hb_declval (Appl),
						   hb_declval (V),
						   hb_declval (Ts)...))
  {
    ಪ್ರತ್ಯಾಗಮ hb_invoke (std::forward<Appl> (a),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }
  template <typename T0, typename ...Ts,
	    unsigned P = Pos,
	    hb_enable_if (P == 2)> auto
  operator () (T0&& d0, Ts&& ...ds) -> decltype (hb_invoke (hb_declval (Appl),
							    hb_declval (T0),
							    hb_declval (V),
							    hb_declval (Ts)...))
  {
    ಪ್ರತ್ಯಾಗಮ hb_invoke (std::forward<Appl> (a),
		      std::forward<T0> (d0),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }

  ಗುಪ್ತ:
  hb_reference_wrapper<Appl> a;
  V v;
};
template <unsigned Pos=1, typename Appl, typename V>
auto hb_partial (Appl&& a, V&& v) HB_AUTO_RETURN
(( hb_partial_t<Pos, Appl, V> (a, v) ))

/* The following, HB_PARTIALIZE, macro uses a particular corner-case
 * of C++11 that is not particularly well-supported by all compilers.
 * What's happening is that it's using "this" in a trailing return-type
 * via decltype().  Broken compilers deduce the type of "this" pointer
 * in that context differently from what it resolves to in the body
 * of the function.
 *
 * One probable cause of this is that at the time of trailing return
 * type declaration, "this" points to an incomplete type, whereas in
 * the function body the type is complete.  That doesn't justify the
 * error in any way, but is probably what's happening.
 *
 * In the case of MSVC, we get around this by using C++14 "decltype(auto)"
 * which deduces the type from the actual return statement.  For gcc 4.8
 * we use "+this" instead of "this" which produces an rvalue that seems
 * to be deduced as the same type with this particular compiler, and seem
 * to be fine as default code path as well.
 */
#ifdef _MSC_VER
/* https://github.com/harfbuzz/harfbuzz/issues/1730 */ \
#define HB_PARTIALIZE(Pos) \
  template <typename _T> \
  decltype(auto) operator () (_T&& _v) ಧ್ರುವ \
  { ಪ್ರತ್ಯಾಗಮ hb_partial<Pos> (ಏತತ್, std::forward<_T> (_v)); } \
  static_assert (ಸತ್ಯ, "")
#ಅನ್ಯಥಾ
/* https://github.com/harfbuzz/harfbuzz/issues/1724 */
#define HB_PARTIALIZE(Pos) \
  template <typename _T> \
  auto operator () (_T&& _v) ಧ್ರುವ HB_AUTO_RETURN \
  (hb_partial<Pos> (+ಏತತ್, std::forward<_T> (_v))) \
  static_assert (ಸತ್ಯ, "")
#endif


ಸಂರಚನಾ
{
  ಗುಪ್ತ:

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<1>) ಧ್ರುವ HB_AUTO_RETURN
  (
    hb_deref (std::forward<Pred> (p)).has (std::forward<Val> (v))
  )

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<0>) ಧ್ರುವ HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Pred> (p),
	       std::forward<Val> (v))
  )

  ಸಾರ್ವಜನಿಕ:

  template <typename Pred, typename Val> auto
  operator () (Pred&& p, Val &&v) ಧ್ರುವ HB_RETURN (ತಾರ್ಕಿಕ,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_has);

ಸಂರಚನಾ
{
  ಗುಪ್ತ:

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<1>) ಧ್ರುವ HB_AUTO_RETURN
  (
    hb_has (std::forward<Pred> (p),
	    std::forward<Val> (v))
  )

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<0>) ಧ್ರುವ HB_AUTO_RETURN
  (
    std::forward<Pred> (p) == std::forward<Val> (v)
  )

  ಸಾರ್ವಜನಿಕ:

  template <typename Pred, typename Val> auto
  operator () (Pred&& p, Val &&v) ಧ್ರುವ HB_RETURN (ತಾರ್ಕಿಕ,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_match);

ಸಂರಚನಾ
{
  ಗುಪ್ತ:

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<2>) ಧ್ರುವ HB_AUTO_RETURN
  (
    hb_deref (std::forward<Proj> (f)).get (std::forward<Val> (v))
  )

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<1>) ಧ್ರುವ HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Proj> (f),
	       std::forward<Val> (v))
  )

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<0>) ಧ್ರುವ HB_AUTO_RETURN
  (
    std::forward<Proj> (f)[std::forward<Val> (v)]
  )

  ಸಾರ್ವಜನಿಕ:

  template <typename Proj, typename Val> auto
  operator () (Proj&& f, Val &&v) ಧ್ರುವ HB_AUTO_RETURN
  (
    impl (std::forward<Proj> (f),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_get);

ಸಂರಚನಾ
{
  ಗುಪ್ತ:

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<3>) ಧ್ರುವ HB_AUTO_RETURN
  (
    std::forward<T2> (v2).cmp (std::forward<T1> (v1)) == 0
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<2>) ಧ್ರುವ HB_AUTO_RETURN
  (
    std::forward<T1> (v1).cmp (std::forward<T2> (v2)) == 0
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<1>) ಧ್ರುವ HB_AUTO_RETURN
  (
    std::forward<T1> (v1) == std::forward<T2> (v2)
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<0>) ಧ್ರುವ HB_AUTO_RETURN
  (
    std::forward<T2> (v2) == std::forward<T1> (v1)
  )

  ಸಾರ್ವಜನಿಕ:

  template <typename T1, typename T2> auto
  operator () (T1&& v1, T2 &&v2) ಧ್ರುವ HB_AUTO_RETURN
  (
    impl (std::forward<T1> (v1),
	  std::forward<T2> (v2),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_equal);

ಸಂರಚನಾ
{
  template <typename T> ಶೂನ್ಯ
  operator () (T& a, T& b) ಧ್ರುವ
  {
    using std::swap; // allow ADL
    swap (a, b);
  }
}
HB_FUNCOBJ (hb_swap);


template <typename T1, typename T2>
ಸಂರಚನಾ hb_pair_t
{
  ಪ್ರಕಾರನಿರ್ದೇಶ T1 first_t;
  ಪ್ರಕಾರನಿರ್ದೇಶ T2 second_t;
  ಪ್ರಕಾರನಿರ್ದೇಶ hb_pair_t<T1, T2> pair_t;

  template <typename U1 = T1, typename U2 = T2,
	    hb_enable_if (std::is_default_constructible<U1>::value &&
			  std::is_default_constructible<U2>::value)>
  hb_pair_t () : first (), second () {}
  hb_pair_t (T1 a, T2 b) : first (std::forward<T1> (a)), second (std::forward<T2> (b)) {}

  template <typename Q1, typename Q2,
	    hb_enable_if (hb_is_convertible (T1, Q1) &&
			  hb_is_convertible (T2, Q2))>
  operator hb_pair_t<Q1, Q2> () { ಪ್ರತ್ಯಾಗಮ hb_pair_t<Q1, Q2> (first, second); }

  hb_pair_t<T1, T2> reverse () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_pair_t<T1, T2> (second, first); }

  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ pair_t& o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ first == o.first && second == o.second; }
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ pair_t& o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !(*ಏತತ್ == o); }
  ತಾರ್ಕಿಕ operator < (ಧ್ರುವ pair_t& o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ first < o.first || (first == o.first && second < o.second); }
  ತಾರ್ಕಿಕ operator >= (ಧ್ರುವ pair_t& o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !(*ಏತತ್ < o); }
  ತಾರ್ಕಿಕ operator > (ಧ್ರುವ pair_t& o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ first > o.first || (first == o.first && second > o.second); }
  ತಾರ್ಕಿಕ operator <= (ಧ್ರುವ pair_t& o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !(*ಏತತ್ > o); }

  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb)
  {
    pair_t *a = (pair_t *) pa;
    pair_t *b = (pair_t *) pb;

    ಯದಿ (a->first < b->first) ಪ್ರತ್ಯಾಗಮ -1;
    ಯದಿ (a->first > b->first) ಪ್ರತ್ಯಾಗಮ +1;
    ಯದಿ (a->second < b->second) ಪ್ರತ್ಯಾಗಮ -1;
    ಯದಿ (a->second > b->second) ಪ್ರತ್ಯಾಗಮ +1;
    ಪ್ರತ್ಯಾಗಮ 0;
  }

  friend ಶೂನ್ಯ swap (hb_pair_t& a, hb_pair_t& b) noexcept
  {
    hb_swap (a.first, b.first);
    hb_swap (a.second, b.second);
  }


  T1 first;
  T2 second;
};
template <typename T1, typename T2> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ hb_pair_t<T1, T2>
hb_pair (T1&& a, T2&& b) { ಪ್ರತ್ಯಾಗಮ hb_pair_t<T1, T2> (a, b); }

ಪ್ರಕಾರನಿರ್ದೇಶ hb_pair_t<hb_codepoint_t, hb_codepoint_t> hb_codepoint_pair_t;

ಸಂರಚನಾ
{
  template <typename Pair> constexpr typename Pair::first_t
  operator () (ಧ್ರುವ Pair& pair) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ pair.first; }
}
HB_FUNCOBJ (hb_first);

ಸಂರಚನಾ
{
  template <typename Pair> constexpr typename Pair::second_t
  operator () (ಧ್ರುವ Pair& pair) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ pair.second; }
}
HB_FUNCOBJ (hb_second);

/* Note.  In min/max impl, we can use hb_type_identity<T> for second argument.
 * However, that would silently convert between different-signedness integers.
 * Instead we accept two different types, such that compiler can err if
 * comparing integers of different signedness. */
ಸಂರಚನಾ
{
  template <typename T, typename T2> constexpr auto
  operator () (T&& a, T2&& b) ಧ್ರುವ HB_AUTO_RETURN
  (a <= b ? a : b)
}
HB_FUNCOBJ (hb_min);
ಸಂರಚನಾ
{
  template <typename T, typename T2> constexpr auto
  operator () (T&& a, T2&& b) ಧ್ರುವ HB_AUTO_RETURN
  (a >= b ? a : b)
}
HB_FUNCOBJ (hb_max);
ಸಂರಚನಾ
{
  template <typename T, typename T2, typename T3> constexpr auto
  operator () (T&& x, T2&& min, T3&& max) ಧ್ರುವ HB_AUTO_RETURN
  (hb_min (hb_max (std::forward<T> (x), std::forward<T2> (min)), std::forward<T3> (max)))
}
HB_FUNCOBJ (hb_clamp);

/* Signed saturating arithmetic. */
template <typename T>
ಸ್ಥಿರ HB_ALWAYS_INLINE T
hb_saturate_add (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#ಯದಿ hb_has_builtin(__builtin_add_overflow)
  T result;
  ಯದಿ (likely (!__builtin_add_overflow (a, b, &result)))
    ಪ್ರತ್ಯಾಗಮ result;
#ಅನ್ಯಥಾ
  ಯದಿ (b > 0)
  {
    ಯದಿ (likely (a <= hb_int_max (T) - b))
      ಪ್ರತ್ಯಾಗಮ a + b;
  }
  ಅನ್ಯಥಾ ಯದಿ (likely (a >= hb_int_min (T) - b))
    ಪ್ರತ್ಯಾಗಮ a + b;
#endif

  ಪ್ರತ್ಯಾಗಮ a < 0 ? hb_int_min (T) : hb_int_max (T);
}

template <typename T>
ಸ್ಥಿರ HB_ALWAYS_INLINE T
hb_saturate_sub (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#ಯದಿ hb_has_builtin(__builtin_sub_overflow)
  T result;
  ಯದಿ (likely (!__builtin_sub_overflow (a, b, &result)))
    ಪ್ರತ್ಯಾಗಮ result;
#ಅನ್ಯಥಾ
  ಯದಿ (b > 0)
  {
    ಯದಿ (likely (a >= hb_int_min (T) + b))
      ಪ್ರತ್ಯಾಗಮ a - b;
  }
  ಅನ್ಯಥಾ ಯದಿ (likely (a <= hb_int_max (T) + b))
    ಪ್ರತ್ಯಾಗಮ a - b;
#endif

  ಪ್ರತ್ಯಾಗಮ b < 0 ? hb_int_max (T) : hb_int_min (T);
}

template <typename T>
ಸ್ಥಿರ HB_ALWAYS_INLINE T
hb_saturate_neg (T a)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  ಪ್ರತ್ಯಾಗಮ hb_saturate_sub ((T) 0, a);
}

/* Convert a floating-point value to integer type T, saturating to T's
 * range instead of relying on the undefined behavior of an out-of-range
 * float-to-int conversion.  NaN saturates to the minimum of T. */
template <typename T>
ಸ್ಥಿರ HB_ALWAYS_INLINE T
hb_clamp_to (ದ್ವಿಗುಣ v)
{
  ಪ್ರತ್ಯಾಗಮ (T) hb_clamp (v,
		       (ದ್ವಿಗುಣ) hb_int_min (T),
		       (ದ್ವಿಗುಣ) hb_int_max (T));
}

template <typename T>
ಸ್ಥಿರ HB_ALWAYS_INLINE T
hb_clamp_to (ಭಿನ್ನ v)
{
  /* Widen explicitly: float cannot represent all integer bounds exactly,
   * so the clamp itself is done in double. */
  ಪ್ರತ್ಯಾಗಮ hb_clamp_to<T> ((ದ್ವಿಗುಣ) v);
}

template <typename T>
ಸ್ಥಿರ HB_ALWAYS_INLINE T
hb_clamp_to (int64_t v)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  ಪ್ರತ್ಯಾಗಮ (T) hb_clamp (v,
		       (int64_t) hb_int_min (T),
		       (int64_t) hb_int_max (T));
}

/*
 * Bithacks.
 */

/* Return the number of 1 bits in a uint8_t; faster than hb_popcount() */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned
hb_popcount8 (uint8_t v)
{
  ಸ್ಥಿರ ಧ್ರುವ uint8_t popcount4[16] = {
    0, 1, 1, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 2, 3, 3, 4
  };
  ಪ್ರತ್ಯಾಗಮ popcount4[v & 0xF] + popcount4[v >> 4];
}

/* Return the number of 1 bits in v. */
template <typename T>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ
hb_popcount (T v)
{
#ಯದಿ hb_has_builtin(__builtin_popcount)
  ಯದಿ (ಪರಿಮಾಣ (T) <= ಪರಿಮಾಣ (unsigned ಪೂರ್ಣಾಙ್ಕ))
    ಪ್ರತ್ಯಾಗಮ __builtin_popcount (v);
#endif

#ಯದಿ hb_has_builtin(__builtin_popcountl)
  ಯದಿ (ಪರಿಮಾಣ (T) <= ಪರಿಮಾಣ (unsigned long))
    ಪ್ರತ್ಯಾಗಮ __builtin_popcountl (v);
#endif

#ಯದಿ hb_has_builtin(__builtin_popcountll)
  ಯದಿ (ಪರಿಮಾಣ (T) <= ಪರಿಮಾಣ (unsigned long long))
    ಪ್ರತ್ಯಾಗಮ __builtin_popcountll (v);
#endif

  ಯದಿ (ಪರಿಮಾಣ (T) <= 4)
  {
    /* "HACKMEM 169" */
    uint32_t y;
    y = (v >> 1) &033333333333;
    y = v - y - ((y >>1) & 033333333333);
    ಪ್ರತ್ಯಾಗಮ (((y + (y >> 3)) & 030707070707) % 077);
  }

  ಯದಿ (ಪರಿಮಾಣ (T) == 8)
  {
    uint64_t y = (uint64_t) v;
    y -= ((y >> 1) & 0x5555555555555555ull);
    y = (y & 0x3333333333333333ull) + (y >> 2 & 0x3333333333333333ull);
    ಪ್ರತ್ಯಾಗಮ ((y + (y >> 4)) & 0xf0f0f0f0f0f0f0full) * 0x101010101010101ull >> 56;
  }

  ಯದಿ (ಪರಿಮಾಣ (T) == 16)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ shift = 64;
    ಪ್ರತ್ಯಾಗಮ hb_popcount<uint64_t> ((uint64_t) v) + hb_popcount ((uint64_t) (v >> shift));
  }

  ಪ್ರತಿಜ್ಞಾ (0);
  ಪ್ರತ್ಯಾಗಮ 0; /* Shut up stupid compiler. */
}

/* Returns the number of bits needed to store number */
template <typename T>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ
hb_bit_storage (T v)
{
  ಯದಿ (unlikely (!v)) ಪ್ರತ್ಯಾಗಮ 0;

#ಯದಿ hb_has_builtin(__builtin_clz)
  ಯದಿ (ಪರಿಮಾಣ (T) <= ಪರಿಮಾಣ (unsigned ಪೂರ್ಣಾಙ್ಕ))
    ಪ್ರತ್ಯಾಗಮ ಪರಿಮಾಣ (unsigned ಪೂರ್ಣಾಙ್ಕ) * 8 - __builtin_clz (v);
#endif

#ಯದಿ hb_has_builtin(__builtin_clzl)
  ಯದಿ (ಪರಿಮಾಣ (T) <= ಪರಿಮಾಣ (unsigned long))
    ಪ್ರತ್ಯಾಗಮ ಪರಿಮಾಣ (unsigned long) * 8 - __builtin_clzl (v);
#endif

#ಯದಿ hb_has_builtin(__builtin_clzll)
  ಯದಿ (ಪರಿಮಾಣ (T) <= ಪರಿಮಾಣ (unsigned long long))
    ಪ್ರತ್ಯಾಗಮ ಪರಿಮಾಣ (unsigned long long) * 8 - __builtin_clzll (v);
#endif

#ಯದಿ (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  ಯದಿ (ಪರಿಮಾಣ (T) <= ಪರಿಮಾಣ (unsigned ಪೂರ್ಣಾಙ್ಕ))
  {
    unsigned long where;
    _BitScanReverse (&where, v);
    ಪ್ರತ್ಯಾಗಮ 1 + where;
  }
# ಯದಿ defined(_WIN64)
  ಯದಿ (ಪರಿಮಾಣ (T) <= 8)
  {
    unsigned long where;
    _BitScanReverse64 (&where, v);
    ಪ್ರತ್ಯಾಗಮ 1 + where;
  }
# endif
#endif

  ಯದಿ (ಪರಿಮಾಣ (T) <= 4)
  {
    /* "bithacks" */
    ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ S[] = {1, 2, 4, 8, 16};
    unsigned ಪೂರ್ಣಾಙ್ಕ r = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 4; i >= 0; i--)
      ಯದಿ (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    ಪ್ರತ್ಯಾಗಮ r + 1;
  }
  ಯದಿ (ಪರಿಮಾಣ (T) <= 8)
  {
    /* "bithacks" */
    ಧ್ರುವ uint64_t b[] = {0x2ULL, 0xCULL, 0xF0ULL, 0xFF00ULL, 0xFFFF0000ULL, 0xFFFFFFFF00000000ULL};
    ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ S[] = {1, 2, 4, 8, 16, 32};
    unsigned ಪೂರ್ಣಾಙ್ಕ r = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 5; i >= 0; i--)
      ಯದಿ (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    ಪ್ರತ್ಯಾಗಮ r + 1;
  }
  ಯದಿ (ಪರಿಮಾಣ (T) == 16)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ shift = 64;
    ಪ್ರತ್ಯಾಗಮ (v >> shift) ? hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift :
			  hb_bit_storage<uint64_t> ((uint64_t) v);
  }

  ಪ್ರತಿಜ್ಞಾ (0);
  ಪ್ರತ್ಯಾಗಮ 0; /* Shut up stupid compiler. */
}

/* Returns the number of zero bits in the least significant side of v */
template <typename T>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ
hb_ctz (T v)
{
  ಯದಿ (unlikely (!v)) ಪ್ರತ್ಯಾಗಮ 8 * ಪರಿಮಾಣ (T);

#ಯದಿ hb_has_builtin(__builtin_ctz)
  ಯದಿ (ಪರಿಮಾಣ (T) <= ಪರಿಮಾಣ (unsigned ಪೂರ್ಣಾಙ್ಕ))
    ಪ್ರತ್ಯಾಗಮ __builtin_ctz (v);
#endif

#ಯದಿ hb_has_builtin(__builtin_ctzl)
  ಯದಿ (ಪರಿಮಾಣ (T) <= ಪರಿಮಾಣ (unsigned long))
    ಪ್ರತ್ಯಾಗಮ __builtin_ctzl (v);
#endif

#ಯದಿ hb_has_builtin(__builtin_ctzll)
  ಯದಿ (ಪರಿಮಾಣ (T) <= ಪರಿಮಾಣ (unsigned long long))
    ಪ್ರತ್ಯಾಗಮ __builtin_ctzll (v);
#endif

#ಯದಿ (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  ಯದಿ (ಪರಿಮಾಣ (T) <= ಪರಿಮಾಣ (unsigned ಪೂರ್ಣಾಙ್ಕ))
  {
    unsigned long where;
    _BitScanForward (&where, v);
    ಪ್ರತ್ಯಾಗಮ where;
  }
# ಯದಿ defined(_WIN64)
  ಯದಿ (ಪರಿಮಾಣ (T) <= 8)
  {
    unsigned long where;
    _BitScanForward64 (&where, v);
    ಪ್ರತ್ಯಾಗಮ where;
  }
# endif
#endif

  ಯದಿ (ಪರಿಮಾಣ (T) <= 4)
  {
    /* "bithacks" */
    unsigned ಪೂರ್ಣಾಙ್ಕ c = 32;
    v &= - (int32_t) v;
    ಯದಿ (v) c--;
    ಯದಿ (v & 0x0000FFFF) c -= 16;
    ಯದಿ (v & 0x00FF00FF) c -= 8;
    ಯದಿ (v & 0x0F0F0F0F) c -= 4;
    ಯದಿ (v & 0x33333333) c -= 2;
    ಯದಿ (v & 0x55555555) c -= 1;
    ಪ್ರತ್ಯಾಗಮ c;
  }
  ಯದಿ (ಪರಿಮಾಣ (T) <= 8)
  {
    /* "bithacks" */
    unsigned ಪೂರ್ಣಾಙ್ಕ c = 64;
    v &= - (int64_t) (v);
    ಯದಿ (v) c--;
    ಯದಿ (v & 0x00000000FFFFFFFFULL) c -= 32;
    ಯದಿ (v & 0x0000FFFF0000FFFFULL) c -= 16;
    ಯದಿ (v & 0x00FF00FF00FF00FFULL) c -= 8;
    ಯದಿ (v & 0x0F0F0F0F0F0F0F0FULL) c -= 4;
    ಯದಿ (v & 0x3333333333333333ULL) c -= 2;
    ಯದಿ (v & 0x5555555555555555ULL) c -= 1;
    ಪ್ರತ್ಯಾಗಮ c;
  }
  ಯದಿ (ಪರಿಮಾಣ (T) == 16)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ shift = 64;
    ಪ್ರತ್ಯಾಗಮ (uint64_t) v ? hb_bit_storage<uint64_t> ((uint64_t) v) :
			  hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift;
  }

  ಪ್ರತಿಜ್ಞಾ (0);
  ಪ್ರತ್ಯಾಗಮ 0; /* Shut up stupid compiler. */
}


/*
 * Tiny stuff.
 */

/* ASCII tag/character handling */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ ISALPHA (unsigned ವರ್ಣ c)
{ ಪ್ರತ್ಯಾಗಮ (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ ISALNUM (unsigned ವರ್ಣ c)
{ ಪ್ರತ್ಯಾಗಮ (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ ISSPACE (unsigned ವರ್ಣ c)
{ ಪ್ರತ್ಯಾಗಮ c == ' ' || c =='\f'|| c =='\n'|| c =='\r'|| c =='\t'|| c =='\v'; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ವರ್ಣ TOUPPER (unsigned ವರ್ಣ c)
{ ಪ್ರತ್ಯಾಗಮ (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ವರ್ಣ TOLOWER (unsigned ವರ್ಣ c)
{ ಪ್ರತ್ಯಾಗಮ (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ ISHEX (unsigned ವರ್ಣ c)
{ ಪ್ರತ್ಯಾಗಮ (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ವರ್ಣ TOHEX (uint8_t c)
{ ಪ್ರತ್ಯಾಗಮ (c & 0xF) <= 9 ? (c & 0xF) + '0' : (c & 0xF) + 'a' - 10; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint8_t FROMHEX (unsigned ವರ್ಣ c)
{ ಪ್ರತ್ಯಾಗಮ (c >= '0' && c <= '9') ? c - '0' : TOLOWER (c) - 'a' + 10; }

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ DIV_CEIL (ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ a, unsigned ಪೂರ್ಣಾಙ್ಕ b)
{ ಪ್ರತ್ಯಾಗಮ (a + (b - 1)) / b; }


#undef  ARRAY_LENGTH
template <typename Type, unsigned ಪೂರ್ಣಾಙ್ಕ n>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ ARRAY_LENGTH (ಧ್ರುವ Type (&)[n]) { ಪ್ರತ್ಯಾಗಮ n; }
/* A const version, but does not detect erratically being called on pointers. */
#define ARRAY_LENGTH_CONST(__array) ((signed ಪೂರ್ಣಾಙ್ಕ) (ಪರಿಮಾಣ (__array) / ಪರಿಮಾಣ (__array[0])))


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ *
hb_memcpy (ಶೂನ್ಯ *__restrict dst, ಧ್ರುವ ಶೂನ್ಯ *__restrict src, size_t len)
{
  /* It's illegal to pass 0 as size to memcpy. */
  ಯದಿ (unlikely (!len)) ಪ್ರತ್ಯಾಗಮ dst;
  ಪ್ರತ್ಯಾಗಮ memcpy (dst, src, len);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
hb_memcmp (ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b, unsigned ಪೂರ್ಣಾಙ್ಕ len)
{
  /* It's illegal to pass NULL to memcmp(), even if len is zero.
   * So, wrap it.
   * https://sourceware.org/bugzilla/show_bug.cgi?id=23878 */
  ಯದಿ (unlikely (!len)) ಪ್ರತ್ಯಾಗಮ 0;
  ಪ್ರತ್ಯಾಗಮ memcmp (a, b, len);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ *
hb_memset (ಶೂನ್ಯ *s, ಪೂರ್ಣಾಙ್ಕ c, unsigned ಪೂರ್ಣಾಙ್ಕ n)
{
  /* It's illegal to pass NULL to memset(), even if n is zero. */
  ಯದಿ (unlikely (!n)) ಪ್ರತ್ಯಾಗಮ s;
  ಪ್ರತ್ಯಾಗಮ memset (s, c, n);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ
hb_ceil_to_4 (unsigned ಪೂರ್ಣಾಙ್ಕ v)
{
  ಪ್ರತ್ಯಾಗಮ ((v - 1) | 3) + 1;
}

template <typename T> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
hb_in_range (T u, T lo, T hi)
{
  static_assert (!std::is_signed<T>::value, "");

  /* The casts below are important as if T is smaller than int,
   * the subtract results will become a signed int! */
  ಪ್ರತ್ಯಾಗಮ (T)(u - lo) <= (T)(hi - lo);
}
template <typename T> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
hb_in_ranges (T u, T lo1, T hi1)
{
  ಪ್ರತ್ಯಾಗಮ hb_in_range (u, lo1, hi1);
}
template <typename T, typename ...Ts> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
hb_in_ranges (T u, T lo1, T hi1, Ts... ds)
{
  ಪ್ರತ್ಯಾಗಮ hb_in_range<T> (u, lo1, hi1) || hb_in_ranges<T> (u, ds...);
}


/*
 * Overflow checking.
 */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
hb_unsigned_mul_overflows (unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, unsigned ಪೂರ್ಣಾಙ್ಕ size, unsigned *result = nullptr)
{
#ಯದಿ hb_has_builtin(__builtin_mul_overflow)
  unsigned ರಾಶಿ_result;
  ಯದಿ (!result)
    result = &ರಾಶಿ_result;
  ಪ್ರತ್ಯಾಗಮ __builtin_mul_overflow (ಎಣಿಕೆ, size, result);
#endif

  ಯದಿ (result)
    *result = ಎಣಿಕೆ * size;
  ಪ್ರತ್ಯಾಗಮ (size > 0) && (ಎಣಿಕೆ >= ((unsigned ಪೂರ್ಣಾಙ್ಕ) -1) / size);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
hb_unsigned_add_overflows (unsigned ಪೂರ್ಣಾಙ್ಕ a, unsigned ಪೂರ್ಣಾಙ್ಕ b, unsigned *result = nullptr)
{
#ಯದಿ hb_has_builtin(__builtin_add_overflow)
  unsigned ರಾಶಿ_result;
  ಯದಿ (!result)
    result = &ರಾಶಿ_result;
  ಪ್ರತ್ಯಾಗಮ __builtin_add_overflow (a, b, result);
#endif

  ಯದಿ (result)
    *result = a + b;
  ಪ್ರತ್ಯಾಗಮ b > (unsigned ಪೂರ್ಣಾಙ್ಕ) -1 - a;
}

/* Saturating arithmetic on size_t.  On platforms where size_t is wider than
 * unsigned int (i.e. 64-bit), the inputs to get_size()-style computations
 * (counts and static_sizes which are at most 32-bit) cannot overflow size_t,
 * so these reduce to plain arithmetic.  On 32-bit platforms (size_t is
 * unsigned int), they saturate to SIZE_MAX so sanitize/serialize callers
 * naturally reject the resulting size as out-of-range. */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ size_t
hb_unsigned_mul_saturate (size_t a, size_t b)
{
  ಯದಿ (ಪರಿಮಾಣ (size_t) > ಪರಿಮಾಣ (unsigned ಪೂರ್ಣಾಙ್ಕ))
    ಪ್ರತ್ಯಾಗಮ a * b;
#ಯದಿ hb_has_builtin(__builtin_mul_overflow)
  size_t result;
  ಯದಿ (__builtin_mul_overflow (a, b, &result))
    ಪ್ರತ್ಯಾಗಮ (size_t) -1;
  ಪ್ರತ್ಯಾಗಮ result;
#ಅನ್ಯಥಾ
  ಯದಿ (b > 0 && a > ((size_t) -1) / b) ಪ್ರತ್ಯಾಗಮ (size_t) -1;
  ಪ್ರತ್ಯಾಗಮ a * b;
#endif
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ size_t
hb_unsigned_add_saturate (size_t a, size_t b)
{
  ಯದಿ (ಪರಿಮಾಣ (size_t) > ಪರಿಮಾಣ (unsigned ಪೂರ್ಣಾಙ್ಕ))
    ಪ್ರತ್ಯಾಗಮ a + b;
#ಯದಿ hb_has_builtin(__builtin_add_overflow)
  size_t result;
  ಯದಿ (__builtin_add_overflow (a, b, &result))
    ಪ್ರತ್ಯಾಗಮ (size_t) -1;
  ಪ್ರತ್ಯಾಗಮ result;
#ಅನ್ಯಥಾ
  ಯದಿ (b > ((size_t) -1) - a) ಪ್ರತ್ಯಾಗಮ (size_t) -1;
  ಪ್ರತ್ಯಾಗಮ a + b;
#endif
}

/* Variadic forms: fold left across all arguments. */
template <typename ...Ts>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ size_t
hb_unsigned_mul_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ ಪ್ರತ್ಯಾಗಮ hb_unsigned_mul_saturate (hb_unsigned_mul_saturate (a, b), c, rest...); }

template <typename ...Ts>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ size_t
hb_unsigned_add_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ ಪ್ರತ್ಯಾಗಮ hb_unsigned_add_saturate (hb_unsigned_add_saturate (a, b), c, rest...); }

/* Saturating mul-add: a * b + c.  Covers the dominant get_size() pattern
 * `count * static_size + min_size`. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ size_t
hb_unsigned_mul_add_saturate (size_t a, size_t b, size_t c)
{ ಪ್ರತ್ಯಾಗಮ hb_unsigned_add_saturate (hb_unsigned_mul_saturate (a, b), c); }


/*
 * Sort and search.
 */

template <typename K, typename V, typename ...Ts>
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
_hb_cmp_method (ಧ್ರುವ ಶೂನ್ಯ *pkey, ಧ್ರುವ ಶೂನ್ಯ *pval, Ts... ds)
{
  ಧ್ರುವ K& key = * (ಧ್ರುವ K*) pkey;
  ಧ್ರುವ V& val = * (ಧ್ರುವ V*) pval;

  ಪ್ರತ್ಯಾಗಮ val.cmp (key, ds...);
}

template <typename K, typename V>
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
_hb_cmp_operator (ಧ್ರುವ ಶೂನ್ಯ *pkey, ಧ್ರುವ ಶೂನ್ಯ *pval)
{
  ಧ್ರುವ K& key = * (ಧ್ರುವ K*) pkey;
  ಧ್ರುವ V& val = * (ಧ್ರುವ V*) pval;

  ಯದಿ (key < val) ಪ್ರತ್ಯಾಗಮ -1;
  ಯದಿ (key > val) ಪ್ರತ್ಯಾಗಮ  1;
  ಪ್ರತ್ಯಾಗಮ 0;
}

template <typename V, typename K, typename ...Ts>
HB_HOT
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
hb_bsearch_impl (unsigned *pos, /* Out */
		 ಧ್ರುವ K& key,
		 V* base, size_t nmemb, size_t stride,
		 ಪೂರ್ಣಾಙ್ಕ (*compar)(ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ_3, ಧ್ರುವ ಶೂನ್ಯ *_item, Ts... _ds),
		 Ts... ds)
{
  /* This is our *only* bsearch implementation. */

  ಪೂರ್ಣಾಙ್ಕ min = 0, max = (ಪೂರ್ಣಾಙ್ಕ) nmemb - 1;
  ಯಾವತ್ (min <= max)
  {
    ಪೂರ್ಣಾಙ್ಕ mid = ((unsigned ಪೂರ್ಣಾಙ್ಕ) min + (unsigned ಪೂರ್ಣಾಙ್ಕ) max) / 2;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    V* p = (V*) (((ಧ್ರುವ ವರ್ಣ *) base) + (mid * stride));
#pragma GCC diagnostic ತೆಗೆ
    ಪೂರ್ಣಾಙ್ಕ c = compar ((ಧ್ರುವ ಶೂನ್ಯ *) std::addressof (key), (ಧ್ರುವ ಶೂನ್ಯ *) p, ds...);
    ಯದಿ (c < 0)
      max = mid - 1;
    ಅನ್ಯಥಾ ಯದಿ (c > 0)
      min = mid + 1;
    ಅನ್ಯಥಾ
    {
      *pos = mid;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
  }
  *pos = min;
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

template <typename V, typename K>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ V*
hb_bsearch (ಧ್ರುವ K& key, V* base,
	    size_t nmemb, size_t stride = ಪರಿಮಾಣ (V),
	    ಪೂರ್ಣಾಙ್ಕ (*compar)(ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ_3, ಧ್ರುವ ಶೂನ್ಯ *_item) = _hb_cmp_method<K, V>)
{
  unsigned pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  ಪ್ರತ್ಯಾಗಮ hb_bsearch_impl (&pos, key, base, nmemb, stride, compar) ?
	 (V*) (((ಧ್ರುವ ವರ್ಣ *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic ತೆಗೆ
}
template <typename V, typename K, typename ...Ts>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ V*
hb_bsearch (ಧ್ರುವ K& key, V* base,
	    size_t nmemb, size_t stride,
	    ಪೂರ್ಣಾಙ್ಕ (*compar)(ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ_3, ಧ್ರುವ ಶೂನ್ಯ *_item, Ts... _ds),
	    Ts... ds)
{
  unsigned pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  ಪ್ರತ್ಯಾಗಮ hb_bsearch_impl (&pos, key, base, nmemb, stride, compar, ds...) ?
	 (V*) (((ಧ್ರುವ ವರ್ಣ *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic ತೆಗೆ
}


/* Quicksort partitioning loop: median-of-three pivot, two-way
 * Hoare partition, tail-call elimination on the larger side.
 * Stops partitioning when subranges shrink below the threshold;
 * a single insertion-sort pass over the whole array (run by the
 * caller) finishes the job.  Same structure libstdc++ uses for
 * std::sort.
 *
 * Not stable; equivalent values may be swapped. */
/* Quicksort partitioning loop: median-of-three pivot, two-way
 * Hoare partition, tail-call elimination on the larger side.
 * Stops partitioning when subranges shrink below the threshold;
 * a single insertion-sort pass over the whole array (run by
 * hb_qsort_inline below) finishes the job.  Same shape libstdc++
 * uses for std::sort.
 *
 * Not stable; equivalent values may be swapped. */
template <typename T, typename Compar>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_qsort_loop (T *base, size_t nel, Compar compar)
{
  ಯಾವತ್ (nel > 24)
  {
    T *last = base + nel - 1;
    T *mid = base + nel / 2;

    /* Median-of-three pivot, parked at last-1. */
    ಯದಿ (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    ಯದಿ (compar (*mid, *last) > 0)
    {
      hb_swap (*mid, *last);
      ಯದಿ (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    }
    hb_swap (*mid, *(last - 1));
    T &pivot = *(last - 1);

    /* Two-way Hoare partition.  Inner loops are unguarded:
     * median-of-three left *base <= pivot and *last >= pivot,
     * which act as sentinels. */
    T *i = base, *j = last - 1;
    ಯಾವತ್ (ಸತ್ಯ)
    {
      ಯಾವತ್ (compar (*++i, pivot) < 0) {}
      ಯಾವತ್ (compar (*--j, pivot) > 0) {}
      ಯದಿ (i >= j) ಭಙ್ಗ;
      hb_swap (*i, *j);
    }
    hb_swap (*i, *(last - 1));

    /* Recurse on smaller side, loop on larger — bounds stack
     * depth at O(log n). */
    size_t left  = (size_t) (i - base);
    size_t ಬಲ = nel - left - 1;
    ಯದಿ (left < ಬಲ)
    {
      hb_qsort_loop (base, left, compar);
      base = i + 1;
      nel  = ಬಲ;
    }
    ಅನ್ಯಥಾ
    {
      hb_qsort_loop (i + 1, ಬಲ, compar);
      nel  = left;
    }
  }
}

template <typename T, typename Compar>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_qsort_inline (T *base, size_t nel, Compar compar)
{
  hb_qsort_loop (base, nel, compar);

  /* Single final insertion sort over the whole array.  After
   * the partitioning loop, every element is within the threshold
   * of its sorted position, so this pass is O(n * threshold). */
  T *end = base + nel;
  ಪ್ರತಿ (T *pi = base + 1; pi < end; pi++)
    ಪ್ರತಿ (T *pj = pi; pj > base && compar (pj[-1], pj[0]) > 0; pj--)
      hb_swap (pj[-1], pj[0]);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_qsort (ಶೂನ್ಯ *base, size_t nel, size_t width,
	  ಪೂರ್ಣಾಙ್ಕ (*compar)(ಧ್ರುವ ಶೂನ್ಯ *_a, ಧ್ರುವ ಶೂನ್ಯ *_b))
{
  qsort (base, nel, width, compar);
}




template <typename T, typename T2, typename T3 = ಪೂರ್ಣಾಙ್ಕ> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_stable_sort (T *array, unsigned ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ(*compar)(ಧ್ರುವ T2 *, ಧ್ರುವ T2 *), T3 *array2 = nullptr)
{
  static_assert (hb_is_trivially_copy_assignable (T), "");
  static_assert (hb_is_trivially_copy_assignable (T3), "");

  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < len; i++)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ j = i;
    ಯಾವತ್ (j && compar (&array[j - 1], &array[i]) > 0)
      j--;
    ಯದಿ (i == j)
      ಅನುವರ್ತ;
    /* Move item i to occupy place for item j, shift what's in between. */
    {
      T t = array[i];
      memmove (&array[j + 1], &array[j], (i - j) * ಪರಿಮಾಣ (T));
      array[j] = t;
    }
    ಯದಿ (array2)
    {
      T3 t = array2[i];
      memmove (&array2[j + 1], &array2[j], (i - j) * ಪರಿಮಾಣ (T3));
      array2[j] = t;
    }
  }
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ hb_bool_t
hb_codepoint_parse (ಧ್ರುವ ವರ್ಣ *s, unsigned ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ base, hb_codepoint_t *out)
{
  unsigned ಪೂರ್ಣಾಙ್ಕ v;
  ಧ್ರುವ ವರ್ಣ *p = s;
  ಧ್ರುವ ವರ್ಣ *end = p + len;
  ಯದಿ (unlikely (!hb_parse_uint (&p, end, &v, ಸತ್ಯ/* whole buffer */, base)))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  *out = v;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}


/* Operators. */

ಸಂರಚನಾ
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ಧ್ರುವ T &a, ಧ್ರುವ T &b) ಧ್ರುವ HB_AUTO_RETURN (a & b)
}
HB_FUNCOBJ (hb_bitwise_and);
ಸಂರಚನಾ
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ಧ್ರುವ T &a, ಧ್ರುವ T &b) ಧ್ರುವ HB_AUTO_RETURN (a | b)
}
HB_FUNCOBJ (hb_bitwise_or);
ಸಂರಚನಾ
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ಧ್ರುವ T &a, ಧ್ರುವ T &b) ಧ್ರುವ HB_AUTO_RETURN (a ^ b)
}
HB_FUNCOBJ (hb_bitwise_xor);
ಸಂರಚನಾ
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ಧ್ರುವ T &a, ಧ್ರುವ T &b) ಧ್ರುವ HB_AUTO_RETURN (~a & b)
}
HB_FUNCOBJ (hb_bitwise_lt);
ಸಂರಚನಾ
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ಧ್ರುವ T &a, ಧ್ರುವ T &b) ಧ್ರುವ HB_AUTO_RETURN (a & ~b)
}
HB_FUNCOBJ (hb_bitwise_gt); // aka sub
ಸಂರಚನಾ
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ಧ್ರುವ T &a, ಧ್ರುವ T &b) ಧ್ರುವ HB_AUTO_RETURN (~a | b)
}
HB_FUNCOBJ (hb_bitwise_le);
ಸಂರಚನಾ
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ಧ್ರುವ T &a, ಧ್ರುವ T &b) ಧ್ರುವ HB_AUTO_RETURN (a | ~b)
}
HB_FUNCOBJ (hb_bitwise_ge);
ಸಂರಚನಾ
{
  template <typename T> constexpr auto
  operator () (ಧ್ರುವ T &a) ಧ್ರುವ HB_AUTO_RETURN (~a)
}
HB_FUNCOBJ (hb_bitwise_neg);

ಸಂರಚನಾ
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ಧ್ರುವ T &a, ಧ್ರುವ T2 &b) ಧ್ರುವ HB_AUTO_RETURN (a + b)
}
HB_FUNCOBJ (hb_add);
ಸಂರಚನಾ
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ಧ್ರುವ T &a, ಧ್ರುವ T2 &b) ಧ್ರುವ HB_AUTO_RETURN (a - b)
}
HB_FUNCOBJ (hb_sub);
ಸಂರಚನಾ
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ಧ್ರುವ T &a, ಧ್ರುವ T2 &b) ಧ್ರುವ HB_AUTO_RETURN (b - a)
}
HB_FUNCOBJ (hb_rsub);
ಸಂರಚನಾ
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ಧ್ರುವ T &a, ಧ್ರುವ T2 &b) ಧ್ರುವ HB_AUTO_RETURN (a * b)
}
HB_FUNCOBJ (hb_mul);
ಸಂರಚನಾ
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ಧ್ರುವ T &a, ಧ್ರುವ T2 &b) ಧ್ರುವ HB_AUTO_RETURN (a / b)
}
HB_FUNCOBJ (hb_div);
ಸಂರಚನಾ
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ಧ್ರುವ T &a, ಧ್ರುವ T2 &b) ಧ್ರುವ HB_AUTO_RETURN (a % b)
}
HB_FUNCOBJ (hb_mod);
ಸಂರಚನಾ
{
  template <typename T> constexpr auto
  operator () (ಧ್ರುವ T &a) ಧ್ರುವ HB_AUTO_RETURN (+a)
}
HB_FUNCOBJ (hb_pos);
ಸಂರಚನಾ
{
  template <typename T> constexpr auto
  operator () (ಧ್ರುವ T &a) ಧ್ರುವ HB_AUTO_RETURN (-a)
}
HB_FUNCOBJ (hb_neg);
ಸಂರಚನಾ
{
  template <typename T> constexpr auto
  operator () (T &a) ಧ್ರುವ HB_AUTO_RETURN (++a)
}
HB_FUNCOBJ (hb_inc);
ಸಂರಚನಾ
{
  template <typename T> constexpr auto
  operator () (T &a) ಧ್ರುವ HB_AUTO_RETURN (--a)
}
HB_FUNCOBJ (hb_dec);


/* Adapted from kurbo implementation with extra parameters added,
 * and finding for a particular range instead of 0.
 *
 * For documentation and implementation see:
 *
 * [ITP method]: https://en.wikipedia.org/wiki/ITP_Method
 * [An Enhancement of the Bisection Method Average Performance Preserving Minmax Optimality]: https://dl.acm.org/doi/10.1145/3423597
 * https://docs.rs/kurbo/0.8.1/kurbo/common/fn.solve_itp.html
 * https://github.com/linebender/kurbo/blob/fd839c25ea0c98576c7ce5789305822675a89938/src/common.rs#L162-L248
 */
template <typename func_t>
ದ್ವಿಗುಣ solve_itp (func_t f,
		  ದ್ವಿಗುಣ a, ದ್ವಿಗುಣ b,
		  ದ್ವಿಗುಣ epsilon,
		  ದ್ವಿಗುಣ min_y, ದ್ವಿಗುಣ max_y,
		  ದ್ವಿಗುಣ &ya, ದ್ವಿಗುಣ &yb, ದ್ವಿಗುಣ &y)
{
  // Guard against degenerate interval
  ಯದಿ (b - a <= 0.0)
  {
    y = ya;
    ಪ್ರತ್ಯಾಗಮ a;
  }

  unsigned n1_2 = (unsigned) (hb_max (ceil (log2 ((b - a) / epsilon)) - 1.0, 0.0));
  ಧ್ರುವ unsigned n0 = 1; // Hardwired
  ಧ್ರುವ ದ್ವಿಗುಣ k1 = 0.2 / (b - a); // Hardwired.
  unsigned nmax = n0 + n1_2;
  ದ್ವಿಗುಣ scaled_epsilon = epsilon * ದ್ವಿಗುಣ (1llu << nmax);
  ದ್ವಿಗುಣ _2_epsilon = 2.0 * epsilon;
  ಯಾವತ್ (b - a > _2_epsilon)
  {
    ದ್ವಿಗುಣ x1_2 = 0.5 * (a + b);
    ದ್ವಿಗುಣ r = scaled_epsilon - 0.5 * (b - a);
    // Guard against yb == ya to prevent division by zero
    ದ್ವಿಗುಣ xf = (yb != ya) ? (yb * a - ya * b) / (yb - ya) : x1_2;
    ದ್ವಿಗುಣ sigma = x1_2 - xf;
    ದ್ವಿಗುಣ b_a = b - a;
    // This has k2 = 2 hardwired for efficiency.
    ದ್ವಿಗುಣ b_a_k2 = b_a * b_a;
    ದ್ವಿಗುಣ delta = k1 * b_a_k2;
    ಪೂರ್ಣಾಙ್ಕ sigma_sign = sigma >= 0 ? +1 : -1;
    ದ್ವಿಗುಣ xt = delta <= fabs (x1_2 - xf) ? xf + delta * sigma_sign : x1_2;
    ದ್ವಿಗುಣ xitp = fabs (xt - x1_2) <= r ? xt : x1_2 - r * sigma_sign;
    ದ್ವಿಗುಣ yitp = f (xitp);
    ಯದಿ (yitp > max_y)
    {
      b = xitp;
      yb = yitp;
    }
    ಅನ್ಯಥಾ ಯದಿ (yitp < min_y)
    {
      a = xitp;
      ya = yitp;
    }
    ಅನ್ಯಥಾ
    {
      y = yitp;
      ಪ್ರತ್ಯಾಗಮ xitp;
    }
    scaled_epsilon *= 0.5;
  }
  ಪ್ರತ್ಯಾಗಮ 0.5 * (a + b);
}


/*
 * Scope guard: runs a callable at scope exit (RAII cleanup for
 * non-HB-type resources — raw malloc'd buffers, paired init/end
 * calls like inflateInit/inflateEnd, FT_Done_* handles, etc.).
 *
 * Prefer hb_unique_ptr_t<hb_blob_t> etc. for HB types; this is
 * for the long tail of cleanup that those wrappers don't cover.
 *
 * Usage:
 *   void *buf = hb_malloc (len);
 *   if (!buf) return false;
 *   HB_SCOPE_GUARD (hb_free (buf));
 *   ... multiple fallible operations ...
 *   return true;  // buf freed automatically on any return path
 */
template <typename F>
ಸಂರಚನಾ hb_scope_guard_t
{
  explicit hb_scope_guard_t (F &&f) : f (std::move (f)), active (ಸತ್ಯ) {}
  hb_scope_guard_t (hb_scope_guard_t &&o) noexcept
    : f (std::move (o.f)), active (o.active) { o.active = ಅಸತ್ಯ; }
  hb_scope_guard_t (ಧ್ರುವ hb_scope_guard_t &) = delete;
  hb_scope_guard_t &operator= (ಧ್ರುವ hb_scope_guard_t &) = delete;
  hb_scope_guard_t &operator= (hb_scope_guard_t &&) = delete;
  ~hb_scope_guard_t () { ಯದಿ (active) f (); }

  /* Release: dismiss the guard so the cleanup does NOT run.  Use
   * when transferring ownership out of the scope. */
  ಶೂನ್ಯ release () { active = ಅಸತ್ಯ; }

  ಗುಪ್ತ:
  F f;
  ತಾರ್ಕಿಕ active;
};

template <typename F>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ hb_scope_guard_t<F> hb_make_scope_guard (F &&f)
{ ಪ್ರತ್ಯಾಗಮ hb_scope_guard_t<F> (std::forward<F> (f)); }

#define HB_SCOPE_GUARD_NAME_(ಸಾಲು) hb_scope_guard_##line
#define HB_SCOPE_GUARD_NAME(ಸಾಲು) HB_SCOPE_GUARD_NAME_(ಸಾಲು)
#define HB_SCOPE_GUARD(stmt) \
  auto HB_SCOPE_GUARD_NAME(__LINE__) = \
    hb_make_scope_guard ([&]() { stmt; }); \
  (ಶೂನ್ಯ) HB_SCOPE_GUARD_NAME(__LINE__)


#endif /* HB_ALGS_HH */
