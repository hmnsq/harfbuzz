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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-meta.hh"
#অন্তর্ভাব "hb-null.hh"
#অন্তর্ভাব "hb-number.hh"

#অন্তর্ভাব <algorithm>
#অন্তর্ভাব <initializer_list>
#অন্তর্ভাব <functional>
#অন্তর্ভাব <new>

স্থির HB_ALWAYS_INLINE শূন্য *
hb_malloc2 (size_t nmemb, size_t আকার_2)
{
  যদি (আকার_2 && nmemb > SIZE_MAX / আকার_2) ফেরত nullptr;
  ফেরত hb_malloc (nmemb * আকার_2);
}
স্থির HB_ALWAYS_INLINE শূন্য *
hb_realloc2 (শূন্য *ptr, size_t nmemb, size_t আকার_2)
{
  যদি (আকার_2 && nmemb > SIZE_MAX / আকার_2) ফেরত nullptr;
  ফেরত hb_realloc (ptr, nmemb * আকার_2);
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
	বাহ্যিক "C++" { \
	  স্থির অন্তঃরেখ constexpr T operator | (T l, T r) { ফেরত T ((unsigned) l | (unsigned) r); } \
	  স্থির অন্তঃরেখ constexpr T operator & (T l, T r) { ফেরত T ((unsigned) l & (unsigned) r); } \
	  স্থির অন্তঃরেখ constexpr T operator ^ (T l, T r) { ফেরত T ((unsigned) l ^ (unsigned) r); } \
	  স্থির অন্তঃরেখ constexpr unsigned operator ~ (T r) { ফেরত (~(unsigned) r); } \
	  স্থির অন্তঃরেখ T& operator |= (T &l, T r) { l = l | r; ফেরত l; } \
	  স্থির অন্তঃরেখ T& operator &= (T& l, T r) { l = l & r; ফেরত l; } \
	  স্থির অন্তঃরেখ T& operator ^= (T& l, T r) { l = l ^ r; ফেরত l; } \
	} \
	static_assert (সত্য, "")

/* Useful for set-operations on small enums.
 * For example, for testing "x ∈ {x1, x2, x3}" use:
 * (FLAG_UNSAFE(x) & (FLAG(x1) | FLAG(x2) | FLAG(x3)))
 */
#define FLAG(x) (static_assert_রাশিমালা ((unsigned)(x) < 32) + (((uint32_t) 1U) << (unsigned)(x)))
#define FLAG_UNSAFE(x) ((unsigned)(x) < 32 ? (((uint32_t) 1U) << (unsigned)(x)) : 0)
#define FLAG_RANGE(x,y) (static_assert_রাশিমালা ((x) < (y)) + FLAG(y+1) - FLAG(x))
#define FLAG64(x) (static_assert_রাশিমালা ((unsigned)(x) < 64) + (((uint64_t) 1ULL) << (unsigned)(x)))
#define FLAG64_UNSAFE(x) ((unsigned)(x) < 64 ? (((uint64_t) 1ULL) << (unsigned)(x)) : 0)


/*
 * Fixed-endian integers / floats.
 */


/* Endian swap, used in Windows related backends */
স্থির অন্তঃরেখ constexpr uint16_t hb_uint16_swap (uint16_t v)
{ ফেরত (v >> 8) | (v << 8); }
স্থির অন্তঃরেখ constexpr uint32_t hb_uint32_swap (uint32_t v)
{ ফেরত (hb_uint16_swap (v) << 16) | hb_uint16_swap (v >> 16); }

template <typename Type>
কাঠামো __attribute__((packed)) hb_packed_t
{
  hb_packed_t () = পূর্বনির্ধারিত;
  constexpr hb_packed_t (Type V) : v (V) {}
  operator Type () ধ্রুবক { ফেরত v; }
  hb_packed_t & operator = (Type V) { v = V; ফেরত *এই; }

  ব্যক্তিগত:
  Type v;
};

#ifndef HB_FAST_অঙ্ক_ACCESS

#যদি defined(__OPTIMIZE__) && \
    defined(__BYTE_ORDER) && \
    (__BYTE_ORDER == __BIG_ENDIAN || \
     (__BYTE_ORDER == __LITTLE_ENDIAN && \
      hb_has_builtin(__builtin_bswap16) && \
      hb_has_builtin(__builtin_bswap32) && \
      hb_has_builtin(__builtin_bswap64)))
#define HB_FAST_অঙ্ক_ACCESS 1
#অন্যথা
#define HB_FAST_অঙ্ক_ACCESS 0
#endif

// https://github.com/harfbuzz/harfbuzz/issues/5456
#যদি defined(__GNUC__) && !defined(__clang__) && (__GNUC__ <= 12)
#undef HB_FAST_অঙ্ক_ACCESS
#define HB_FAST_অঙ্ক_ACCESS 0
#endif

#endif

template <যৌক্তিক BE, typename Type, পূর্ণসংখ্যা Bytes = আকার (Type)>
কাঠামো HBInt;
template <যৌক্তিক BE, typename Type>
কাঠামো HBInt<BE, Type, 1>
{
  সর্বজনীন:
  HBInt () = পূর্বনির্ধারিত;
  constexpr HBInt (Type V) : v {uint8_t (V)} {}
  constexpr operator Type () ধ্রুবক { ফেরত v; }
  ব্যক্তিগত: uint8_t v;
};
template <যৌক্তিক BE, typename Type>
কাঠামো HBInt<BE, Type, 2>
{
  সর্বজনীন:
  HBInt () = পূর্বনির্ধারিত;

  HBInt (Type V)
#যদি HB_FAST_অঙ্ক_ACCESS
  {
    যদি (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint16_t> *) v) = V;
    অন্যথা
      *((hb_packed_t<uint16_t> *) v) = __builtin_bswap16 (V);
  }
#অন্যথা
    : v {BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >>  8) & 0xFF)} {}
#endif

  constexpr operator Type () ধ্রুবক
  {
#যদি HB_FAST_অঙ্ক_ACCESS
    ফেরত (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint16_t) *((ধ্রুবক hb_packed_t<uint16_t> *) v)
    :
      __builtin_bswap16 ((uint16_t) *((ধ্রুবক hb_packed_t<uint16_t> *) v))
    ;
#অন্যথা
    ফেরত (BE ? (v[0] <<  8) : (v[0]      ))
	 + (BE ? (v[1]      ) : (v[1] <<  8));
#endif
  }
  ব্যক্তিগত: uint8_t v[2];
};
template <যৌক্তিক BE, typename Type>
কাঠামো HBInt<BE, Type, 3>
{
  static_assert (!std::is_signed<Type>::value, "");
  সর্বজনীন:
  HBInt () = পূর্বনির্ধারিত;
  constexpr HBInt (Type V) : v {BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
				BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
				BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V      ) & 0xFF)} {}

  constexpr operator Type () ধ্রুবক { ফেরত (BE ? (v[0] << 16) : (v[0]      ))
					  + (BE ? (v[1] <<  8) : (v[1] <<  8))
					  + (BE ? (v[2]      ) : (v[2] << 16)); }
  ব্যক্তিগত: uint8_t v[3];
};
template <যৌক্তিক BE, typename Type>
কাঠামো HBInt<BE, Type, 4>
{
  template <যৌক্তিক, typename, পূর্ণসংখ্যা>
  friend কাঠামো HBFloat;

  সর্বজনীন:
  HBInt () = পূর্বনির্ধারিত;

  HBInt (Type V)
#যদি HB_FAST_অঙ্ক_ACCESS
  {
    যদি (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint32_t> *) v) = V;
    অন্যথা
      *((hb_packed_t<uint32_t> *) v) = __builtin_bswap32 (V);
  }
#অন্যথা
    : v {BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 24) & 0xFF)} {}
#endif

  constexpr operator Type () ধ্রুবক {
#যদি HB_FAST_অঙ্ক_ACCESS
    ফেরত (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint32_t) *((ধ্রুবক hb_packed_t<uint32_t> *) v)
    :
      __builtin_bswap32 ((uint32_t) *((ধ্রুবক hb_packed_t<uint32_t> *) v))
    ;
#অন্যথা
    ফেরত (BE ? (uint32_t (v[0]) << 24) : (uint32_t (v[0])      ))
	 + (BE ? (uint32_t (v[1]) << 16) : (uint32_t (v[1]) <<  8))
	 + (BE ? (uint32_t (v[2]) <<  8) : (uint32_t (v[2]) << 16))
	 + (BE ? (uint32_t (v[3])      ) : (uint32_t (v[3]) << 24));
#endif
  }
  ব্যক্তিগত: uint8_t v[4];
};
template <যৌক্তিক BE, typename Type>
কাঠামো HBInt<BE, Type, 8>
{
  template <যৌক্তিক, typename, পূর্ণসংখ্যা>
  friend কাঠামো HBFloat;

  সর্বজনীন:
  HBInt () = পূর্বনির্ধারিত;

  HBInt (Type V)
#যদি HB_FAST_অঙ্ক_ACCESS
  {
    যদি (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint64_t> *) v) = V;
    অন্যথা
      *((hb_packed_t<uint64_t> *) v) = __builtin_bswap64 (V);
  }
#অন্যথা
    : v {BE ? uint8_t ((V >> 56) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 48) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >> 40) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V >> 32) & 0xFF) : uint8_t ((V >> 24) & 0xFF),
	 BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V >> 32) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 40) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 48) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 56) & 0xFF)} {}
#endif

  constexpr operator Type () ধ্রুবক {
#যদি HB_FAST_অঙ্ক_ACCESS
    ফেরত (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint64_t) *((ধ্রুবক hb_packed_t<uint64_t> *) v)
    :
      __builtin_bswap64 ((uint64_t) *((ধ্রুবক hb_packed_t<uint64_t> *) v))
    ;
#অন্যথা
    ফেরত (BE ? (uint64_t (v[0]) << 56) : (uint64_t (v[0])      ))
	 + (BE ? (uint64_t (v[1]) << 48) : (uint64_t (v[1]) <<  8))
	 + (BE ? (uint64_t (v[2]) << 40) : (uint64_t (v[2]) << 16))
	 + (BE ? (uint64_t (v[3]) << 32) : (uint64_t (v[3]) << 24))
	 + (BE ? (uint64_t (v[4]) << 24) : (uint64_t (v[4]) << 32))
	 + (BE ? (uint64_t (v[5]) << 16) : (uint64_t (v[5]) << 40))
	 + (BE ? (uint64_t (v[6]) <<  8) : (uint64_t (v[6]) << 48))
	 + (BE ? (uint64_t (v[7])      ) : (uint64_t (v[7]) << 56));
#endif
  }
  ব্যক্তিগত: uint8_t v[8];
};

/* Floats. */

template <যৌক্তিক BE, typename Type, পূর্ণসংখ্যা Bytes>
কাঠামো HBFloat
{
  using IntType = typename std::conditional<Bytes == 4, uint32_t, uint64_t>::type;

  সর্বজনীন:
  HBFloat () = পূর্বনির্ধারিত;

  HBFloat (Type V)
  {
#যদি HB_FAST_অঙ্ক_ACCESS
    {
	      যদি (BE == (__BYTE_ORDER == __BIG_ENDIAN))
	      {
	        *((hb_packed_t<Type> *) v) = V;
	        ফেরত;
	      }
	    }
#endif

    সংঘ {
      hb_packed_t<Type> f;
      hb_packed_t<IntType> i;
    } u = {{V}};

    ধ্রুবক HBInt<BE, IntType> I = (IntType) u.i;
    জন্য (unsigned i = 0; i < Bytes; i++)
      v[i] = I.v[i];
  }

  /* c++14 constexpr */ operator Type () ধ্রুবক
  {
#যদি HB_FAST_অঙ্ক_ACCESS
	    {
	      যদি (BE == (__BYTE_ORDER == __BIG_ENDIAN))
		ফেরত (Type) *((ধ্রুবক hb_packed_t<Type> *) v);
	    }
#endif

    HBInt<BE, IntType> I;
    জন্য (unsigned i = 0; i < Bytes; i++)
      I.v[i] = v[i];

    সংঘ {
      hb_packed_t<IntType> i;
      hb_packed_t<Type> f;
    } u = {{I}};

    ফেরত (Type) u.f;
  }
  ব্যক্তিগত: uint8_t v[Bytes];
};


/* We want our rounding towards +infinity. */
স্থির অন্তঃরেখ দ্বিগুণ
_hb_roundf (দ্বিগুণ x) { ফেরত floor (x + .5); }

স্থির অন্তঃরেখ দশমিক
_hb_roundf (দশমিক x) { ফেরত floor ((দ্বিগুণ) x + .5); }

#define roundf(x) _hb_roundf(x)

স্থির অন্তঃরেখ শূন্য
hb_sincos (দশমিক rotation, দশমিক &s, দশমিক &c)
{
#ifdef HAVE_SINCOSF
  sincosf (rotation, &s, &c);
#অন্যথা
  c = cosf (rotation);
  s = sinf (rotation);
#endif
}
স্থির অন্তঃরেখ শূন্য
hb_sincos (দ্বিগুণ rotation, দ্বিগুণ &s, দ্বিগুণ &c)
{
#ifdef HAVE_SINCOS
  sincos (rotation, &s, &c);
#অন্যথা
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


কাঠামো
{
  /* Note.  This is dangerous in that if it's passed an rvalue, it returns rvalue-reference. */
  template <typename T> constexpr auto
  operator () (T&& v) ধ্রুবক HB_AUTO_RETURN ( std::forward<T> (v) )
}
HB_FUNCOBJ (hb_identity);
কাঠামো
{
  /* Like identity(), but only retains lvalue-references.  Rvalues are returned as rvalues. */
  template <typename T> constexpr T&
  operator () (T& v) ধ্রুবক { ফেরত v; }

  template <typename T> constexpr hb_remove_reference<T>
  operator () (T&& v) ধ্রুবক { ফেরত v; }
}
HB_FUNCOBJ (hb_lidentity);
কাঠামো
{
  /* Like identity(), but always returns rvalue. */
  template <typename T> constexpr hb_remove_reference<T>
  operator () (T&& v) ধ্রুবক { ফেরত v; }
}
HB_FUNCOBJ (hb_ridentity);

কাঠামো
{
  template <typename T> constexpr যৌক্তিক
  operator () (T&& v) ধ্রুবক { ফেরত যৌক্তিক (std::forward<T> (v)); }
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
			(শূন্য) ((h) ^= (h) >> 23),		\
			(শূন্য) ((h) *= 0x2127599bf4325c37ULL),	\
			(h) ^= (h) >> 47)

স্থির অন্তঃরেখ uint64_t fasthash64(ধ্রুবক শূন্য *buf, size_t len, uint64_t seed)
{
	কাঠামো __attribute__((packed)) packed_uint64_t { uint64_t v; };
	ধ্রুবক uint64_t    m = 0x880355f21e6d1965ULL;
	ধ্রুবক packed_uint64_t *pos = (ধ্রুবক packed_uint64_t *)buf;
	ধ্রুবক packed_uint64_t *end = pos + (len / 8);
	ধ্রুবক unsigned অক্ষর *pos2;
	uint64_t h = seed ^ (len * m);
	uint64_t v;

#ifndef HB_OPTIMIZE_আকার
	যদি (((uintptr_t) pos & 7) == 0)
	{
	  যখন (pos != end)
	  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
	    v  = * (ধ্রুবক uint64_t *) (pos++);
#pragma GCC diagnostic বের_করো
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}
	অন্যথা
#endif
	{
	  যখন (pos != end)
	  {
	    v  = pos++->v;
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}

	pos2 = (ধ্রুবক unsigned অক্ষর*)pos;
	v = 0;

	নির্বাচন (len & 7) {
	ক্ষেত্র 7: v ^= (uint64_t)pos2[6] << 48; HB_FALLTHROUGH;
	ক্ষেত্র 6: v ^= (uint64_t)pos2[5] << 40; HB_FALLTHROUGH;
	ক্ষেত্র 5: v ^= (uint64_t)pos2[4] << 32; HB_FALLTHROUGH;
	ক্ষেত্র 4: v ^= (uint64_t)pos2[3] << 24; HB_FALLTHROUGH;
	ক্ষেত্র 3: v ^= (uint64_t)pos2[2] << 16; HB_FALLTHROUGH;
	ক্ষেত্র 2: v ^= (uint64_t)pos2[1] <<  8; HB_FALLTHROUGH;
	ক্ষেত্র 1: v ^= (uint64_t)pos2[0];
		h ^= fasthash_mix(v);
		h *= m;
	}

	ফেরত fasthash_mix(h);
}

স্থির অন্তঃরেখ uint32_t fasthash32(ধ্রুবক শূন্য *buf, size_t len, uint32_t seed)
{
	// the following trick converts the 64-bit hashcode to Fermat
	// residue, which shall retain information from both the higher
	// and lower parts of hashcode.
        uint64_t h = fasthash64(buf, len, seed);
	ফেরত h - (h >> 32);
}

কাঠামো
{
  ব্যক্তিগত:

  template <typename T> constexpr auto
  impl (ধ্রুবক T& v, hb_priority<2>) ধ্রুবক HB_RETURN (uint32_t, hb_deref (v).hash ())

  // Horrible: std:hash() of integers seems to be identity in gcc / clang?!
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  //
  // For performance characteristics see:
  // https://github.com/harfbuzz/harfbuzz/pull/4228#issuecomment-1565079537
  template <typename T,
	    hb_enable_if (std::is_integral<T>::value && আকার (T) <= আকার (uint32_t))> constexpr auto
  impl (ধ্রুবক T& v, hb_priority<1>) ধ্রুবক HB_RETURN (uint32_t, (uint32_t) v * 2654435761u /* Knuh's multiplicative hash */)
  template <typename T,
	    hb_enable_if (std::is_integral<T>::value && আকার (T) > আকার (uint32_t))> constexpr auto
  impl (ধ্রুবক T& v, hb_priority<1>) ধ্রুবক HB_RETURN (uint32_t, (uint32_t) (v ^ (v >> 32)) * 2654435761u /* Knuth's multiplicative hash */)

  template <typename T,
	    hb_enable_if (std::is_floating_point<T>::value)> constexpr auto
  impl (ধ্রুবক T& v, hb_priority<1>) ধ্রুবক HB_RETURN (uint32_t, fasthash32 (std::addressof (v), আকার (T), 0xf437ffe6))

  template <typename T> constexpr auto
  impl (ধ্রুবক T& v, hb_priority<0>) ধ্রুবক HB_RETURN (uint32_t, std::hash<hb_decay<decltype (hb_deref (v))>>{} (hb_deref (v)))

  সর্বজনীন:

  template <typename T> constexpr auto
  operator () (ধ্রুবক T& v) ধ্রুবক HB_RETURN (uint32_t, impl (v, hb_prioritize))
}
HB_FUNCOBJ (hb_hash);


কাঠামো
{
  ব্যক্তিগত:

  /* Pointer-to-member-function. */
  template <typename Appl, typename T, typename ...Ts> auto
  impl (Appl&& a, hb_priority<2>, T &&v, Ts&&... ds) ধ্রুবক HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v)).*std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  /* Pointer-to-member. */
  template <typename Appl, typename T> auto
  impl (Appl&& a, hb_priority<1>, T &&v) ধ্রুবক HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v))).*std::forward<Appl> (a))

  /* Operator(). */
  template <typename Appl, typename ...Ts> auto
  impl (Appl&& a, hb_priority<0>, Ts&&... ds) ধ্রুবক HB_AUTO_RETURN
  (hb_deref (std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  সর্বজনীন:

  template <typename Appl, typename ...Ts> auto
  operator () (Appl&& a, Ts&&... ds) ধ্রুবক HB_AUTO_RETURN
  (
    impl (std::forward<Appl> (a),
	  hb_prioritize,
	  std::forward<Ts> (ds)...)
  )
}
HB_FUNCOBJ (hb_invoke);

template <unsigned Pos, typename Appl, typename V>
কাঠামো hb_partial_t
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
    ফেরত hb_invoke (std::forward<Appl> (a),
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
    ফেরত hb_invoke (std::forward<Appl> (a),
		      std::forward<T0> (d0),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }

  ব্যক্তিগত:
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
  decltype(auto) operator () (_T&& _v) ধ্রুবক \
  { ফেরত hb_partial<Pos> (এই, std::forward<_T> (_v)); } \
  static_assert (সত্য, "")
#অন্যথা
/* https://github.com/harfbuzz/harfbuzz/issues/1724 */
#define HB_PARTIALIZE(Pos) \
  template <typename _T> \
  auto operator () (_T&& _v) ধ্রুবক HB_AUTO_RETURN \
  (hb_partial<Pos> (+এই, std::forward<_T> (_v))) \
  static_assert (সত্য, "")
#endif


কাঠামো
{
  ব্যক্তিগত:

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<1>) ধ্রুবক HB_AUTO_RETURN
  (
    hb_deref (std::forward<Pred> (p)).has (std::forward<Val> (v))
  )

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<0>) ধ্রুবক HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Pred> (p),
	       std::forward<Val> (v))
  )

  সর্বজনীন:

  template <typename Pred, typename Val> auto
  operator () (Pred&& p, Val &&v) ধ্রুবক HB_RETURN (যৌক্তিক,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_has);

কাঠামো
{
  ব্যক্তিগত:

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<1>) ধ্রুবক HB_AUTO_RETURN
  (
    hb_has (std::forward<Pred> (p),
	    std::forward<Val> (v))
  )

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<0>) ধ্রুবক HB_AUTO_RETURN
  (
    std::forward<Pred> (p) == std::forward<Val> (v)
  )

  সর্বজনীন:

  template <typename Pred, typename Val> auto
  operator () (Pred&& p, Val &&v) ধ্রুবক HB_RETURN (যৌক্তিক,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_match);

কাঠামো
{
  ব্যক্তিগত:

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<2>) ধ্রুবক HB_AUTO_RETURN
  (
    hb_deref (std::forward<Proj> (f)).get (std::forward<Val> (v))
  )

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<1>) ধ্রুবক HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Proj> (f),
	       std::forward<Val> (v))
  )

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<0>) ধ্রুবক HB_AUTO_RETURN
  (
    std::forward<Proj> (f)[std::forward<Val> (v)]
  )

  সর্বজনীন:

  template <typename Proj, typename Val> auto
  operator () (Proj&& f, Val &&v) ধ্রুবক HB_AUTO_RETURN
  (
    impl (std::forward<Proj> (f),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_get);

কাঠামো
{
  ব্যক্তিগত:

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<3>) ধ্রুবক HB_AUTO_RETURN
  (
    std::forward<T2> (v2).cmp (std::forward<T1> (v1)) == 0
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<2>) ধ্রুবক HB_AUTO_RETURN
  (
    std::forward<T1> (v1).cmp (std::forward<T2> (v2)) == 0
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<1>) ধ্রুবক HB_AUTO_RETURN
  (
    std::forward<T1> (v1) == std::forward<T2> (v2)
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<0>) ধ্রুবক HB_AUTO_RETURN
  (
    std::forward<T2> (v2) == std::forward<T1> (v1)
  )

  সর্বজনীন:

  template <typename T1, typename T2> auto
  operator () (T1&& v1, T2 &&v2) ধ্রুবক HB_AUTO_RETURN
  (
    impl (std::forward<T1> (v1),
	  std::forward<T2> (v2),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_equal);

কাঠামো
{
  template <typename T> শূন্য
  operator () (T& a, T& b) ধ্রুবক
  {
    using std::swap; // allow ADL
    swap (a, b);
  }
}
HB_FUNCOBJ (hb_swap);


template <typename T1, typename T2>
কাঠামো hb_pair_t
{
  প্রকারলেখ T1 first_t;
  প্রকারলেখ T2 second_t;
  প্রকারলেখ hb_pair_t<T1, T2> pair_t;

  template <typename U1 = T1, typename U2 = T2,
	    hb_enable_if (std::is_default_constructible<U1>::value &&
			  std::is_default_constructible<U2>::value)>
  hb_pair_t () : first (), second () {}
  hb_pair_t (T1 a, T2 b) : first (std::forward<T1> (a)), second (std::forward<T2> (b)) {}

  template <typename Q1, typename Q2,
	    hb_enable_if (hb_is_convertible (T1, Q1) &&
			  hb_is_convertible (T2, Q2))>
  operator hb_pair_t<Q1, Q2> () { ফেরত hb_pair_t<Q1, Q2> (first, second); }

  hb_pair_t<T1, T2> reverse () ধ্রুবক
  { ফেরত hb_pair_t<T1, T2> (second, first); }

  যৌক্তিক operator == (ধ্রুবক pair_t& o) ধ্রুবক { ফেরত first == o.first && second == o.second; }
  যৌক্তিক operator != (ধ্রুবক pair_t& o) ধ্রুবক { ফেরত !(*এই == o); }
  যৌক্তিক operator < (ধ্রুবক pair_t& o) ধ্রুবক { ফেরত first < o.first || (first == o.first && second < o.second); }
  যৌক্তিক operator >= (ধ্রুবক pair_t& o) ধ্রুবক { ফেরত !(*এই < o); }
  যৌক্তিক operator > (ধ্রুবক pair_t& o) ধ্রুবক { ফেরত first > o.first || (first == o.first && second > o.second); }
  যৌক্তিক operator <= (ধ্রুবক pair_t& o) ধ্রুবক { ফেরত !(*এই > o); }

  স্থির পূর্ণসংখ্যা cmp (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb)
  {
    pair_t *a = (pair_t *) pa;
    pair_t *b = (pair_t *) pb;

    যদি (a->first < b->first) ফেরত -1;
    যদি (a->first > b->first) ফেরত +1;
    যদি (a->second < b->second) ফেরত -1;
    যদি (a->second > b->second) ফেরত +1;
    ফেরত 0;
  }

  friend শূন্য swap (hb_pair_t& a, hb_pair_t& b) noexcept
  {
    hb_swap (a.first, b.first);
    hb_swap (a.second, b.second);
  }


  T1 first;
  T2 second;
};
template <typename T1, typename T2> স্থির অন্তঃরেখ hb_pair_t<T1, T2>
hb_pair (T1&& a, T2&& b) { ফেরত hb_pair_t<T1, T2> (a, b); }

প্রকারলেখ hb_pair_t<hb_codepoint_t, hb_codepoint_t> hb_codepoint_pair_t;

কাঠামো
{
  template <typename Pair> constexpr typename Pair::first_t
  operator () (ধ্রুবক Pair& pair) ধ্রুবক { ফেরত pair.first; }
}
HB_FUNCOBJ (hb_first);

কাঠামো
{
  template <typename Pair> constexpr typename Pair::second_t
  operator () (ধ্রুবক Pair& pair) ধ্রুবক { ফেরত pair.second; }
}
HB_FUNCOBJ (hb_second);

/* Note.  In min/max impl, we can use hb_type_identity<T> for second argument.
 * However, that would silently convert between different-signedness integers.
 * Instead we accept two different types, such that compiler can err if
 * comparing integers of different signedness. */
কাঠামো
{
  template <typename T, typename T2> constexpr auto
  operator () (T&& a, T2&& b) ধ্রুবক HB_AUTO_RETURN
  (a <= b ? a : b)
}
HB_FUNCOBJ (hb_min);
কাঠামো
{
  template <typename T, typename T2> constexpr auto
  operator () (T&& a, T2&& b) ধ্রুবক HB_AUTO_RETURN
  (a >= b ? a : b)
}
HB_FUNCOBJ (hb_max);
কাঠামো
{
  template <typename T, typename T2, typename T3> constexpr auto
  operator () (T&& x, T2&& min, T3&& max) ধ্রুবক HB_AUTO_RETURN
  (hb_min (hb_max (std::forward<T> (x), std::forward<T2> (min)), std::forward<T3> (max)))
}
HB_FUNCOBJ (hb_clamp);

/* Signed saturating arithmetic. */
template <typename T>
স্থির HB_ALWAYS_INLINE T
hb_saturate_add (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#যদি hb_has_builtin(__builtin_add_overflow)
  T ফলাফল;
  যদি (likely (!__builtin_add_overflow (a, b, &ফলাফল)))
    ফেরত ফলাফল;
#অন্যথা
  যদি (b > 0)
  {
    যদি (likely (a <= hb_int_max (T) - b))
      ফেরত a + b;
  }
  অন্যথা যদি (likely (a >= hb_int_min (T) - b))
    ফেরত a + b;
#endif

  ফেরত a < 0 ? hb_int_min (T) : hb_int_max (T);
}

template <typename T>
স্থির HB_ALWAYS_INLINE T
hb_saturate_sub (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#যদি hb_has_builtin(__builtin_sub_overflow)
  T ফলাফল;
  যদি (likely (!__builtin_sub_overflow (a, b, &ফলাফল)))
    ফেরত ফলাফল;
#অন্যথা
  যদি (b > 0)
  {
    যদি (likely (a >= hb_int_min (T) + b))
      ফেরত a - b;
  }
  অন্যথা যদি (likely (a <= hb_int_max (T) + b))
    ফেরত a - b;
#endif

  ফেরত b < 0 ? hb_int_max (T) : hb_int_min (T);
}

template <typename T>
স্থির HB_ALWAYS_INLINE T
hb_saturate_neg (T a)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  ফেরত hb_saturate_sub ((T) 0, a);
}

/* Convert a floating-point value to integer type T, saturating to T's
 * range instead of relying on the undefined behavior of an out-of-range
 * float-to-int conversion.  NaN saturates to the minimum of T. */
template <typename T>
স্থির HB_ALWAYS_INLINE T
hb_clamp_to (দ্বিগুণ v)
{
  ফেরত (T) hb_clamp (v,
		       (দ্বিগুণ) hb_int_min (T),
		       (দ্বিগুণ) hb_int_max (T));
}

template <typename T>
স্থির HB_ALWAYS_INLINE T
hb_clamp_to (দশমিক v)
{
  /* Widen explicitly: float cannot represent all integer bounds exactly,
   * so the clamp itself is done in double. */
  ফেরত hb_clamp_to<T> ((দ্বিগুণ) v);
}

template <typename T>
স্থির HB_ALWAYS_INLINE T
hb_clamp_to (int64_t v)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  ফেরত (T) hb_clamp (v,
		       (int64_t) hb_int_min (T),
		       (int64_t) hb_int_max (T));
}

/*
 * Bithacks.
 */

/* Return the number of 1 bits in a uint8_t; faster than hb_popcount() */
স্থির অন্তঃরেখ unsigned
hb_popcount8 (uint8_t v)
{
  স্থির ধ্রুবক uint8_t popcount4[16] = {
    0, 1, 1, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 2, 3, 3, 4
  };
  ফেরত popcount4[v & 0xF] + popcount4[v >> 4];
}

/* Return the number of 1 bits in v. */
template <typename T>
স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা
hb_popcount (T v)
{
#যদি hb_has_builtin(__builtin_popcount)
  যদি (আকার (T) <= আকার (unsigned পূর্ণসংখ্যা))
    ফেরত __builtin_popcount (v);
#endif

#যদি hb_has_builtin(__builtin_popcountl)
  যদি (আকার (T) <= আকার (unsigned long))
    ফেরত __builtin_popcountl (v);
#endif

#যদি hb_has_builtin(__builtin_popcountll)
  যদি (আকার (T) <= আকার (unsigned long long))
    ফেরত __builtin_popcountll (v);
#endif

  যদি (আকার (T) <= 4)
  {
    /* "HACKMEM 169" */
    uint32_t y;
    y = (v >> 1) &033333333333;
    y = v - y - ((y >>1) & 033333333333);
    ফেরত (((y + (y >> 3)) & 030707070707) % 077);
  }

  যদি (আকার (T) == 8)
  {
    uint64_t y = (uint64_t) v;
    y -= ((y >> 1) & 0x5555555555555555ull);
    y = (y & 0x3333333333333333ull) + (y >> 2 & 0x3333333333333333ull);
    ফেরত ((y + (y >> 4)) & 0xf0f0f0f0f0f0f0full) * 0x101010101010101ull >> 56;
  }

  যদি (আকার (T) == 16)
  {
    unsigned পূর্ণসংখ্যা shift = 64;
    ফেরত hb_popcount<uint64_t> ((uint64_t) v) + hb_popcount ((uint64_t) (v >> shift));
  }

  প্রতিজ্ঞা (0);
  ফেরত 0; /* Shut up stupid compiler. */
}

/* Returns the number of bits needed to store number */
template <typename T>
স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা
hb_bit_storage (T v)
{
  যদি (unlikely (!v)) ফেরত 0;

#যদি hb_has_builtin(__builtin_clz)
  যদি (আকার (T) <= আকার (unsigned পূর্ণসংখ্যা))
    ফেরত আকার (unsigned পূর্ণসংখ্যা) * 8 - __builtin_clz (v);
#endif

#যদি hb_has_builtin(__builtin_clzl)
  যদি (আকার (T) <= আকার (unsigned long))
    ফেরত আকার (unsigned long) * 8 - __builtin_clzl (v);
#endif

#যদি hb_has_builtin(__builtin_clzll)
  যদি (আকার (T) <= আকার (unsigned long long))
    ফেরত আকার (unsigned long long) * 8 - __builtin_clzll (v);
#endif

#যদি (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  যদি (আকার (T) <= আকার (unsigned পূর্ণসংখ্যা))
  {
    unsigned long where;
    _BitScanReverse (&where, v);
    ফেরত 1 + where;
  }
# যদি defined(_WIN64)
  যদি (আকার (T) <= 8)
  {
    unsigned long where;
    _BitScanReverse64 (&where, v);
    ফেরত 1 + where;
  }
# endif
#endif

  যদি (আকার (T) <= 4)
  {
    /* "bithacks" */
    ধ্রুবক unsigned পূর্ণসংখ্যা b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    ধ্রুবক unsigned পূর্ণসংখ্যা S[] = {1, 2, 4, 8, 16};
    unsigned পূর্ণসংখ্যা r = 0;
    জন্য (পূর্ণসংখ্যা i = 4; i >= 0; i--)
      যদি (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    ফেরত r + 1;
  }
  যদি (আকার (T) <= 8)
  {
    /* "bithacks" */
    ধ্রুবক uint64_t b[] = {0x2ULL, 0xCULL, 0xF0ULL, 0xFF00ULL, 0xFFFF0000ULL, 0xFFFFFFFF00000000ULL};
    ধ্রুবক unsigned পূর্ণসংখ্যা S[] = {1, 2, 4, 8, 16, 32};
    unsigned পূর্ণসংখ্যা r = 0;
    জন্য (পূর্ণসংখ্যা i = 5; i >= 0; i--)
      যদি (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    ফেরত r + 1;
  }
  যদি (আকার (T) == 16)
  {
    unsigned পূর্ণসংখ্যা shift = 64;
    ফেরত (v >> shift) ? hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift :
			  hb_bit_storage<uint64_t> ((uint64_t) v);
  }

  প্রতিজ্ঞা (0);
  ফেরত 0; /* Shut up stupid compiler. */
}

/* Returns the number of zero bits in the least significant side of v */
template <typename T>
স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা
hb_ctz (T v)
{
  যদি (unlikely (!v)) ফেরত 8 * আকার (T);

#যদি hb_has_builtin(__builtin_ctz)
  যদি (আকার (T) <= আকার (unsigned পূর্ণসংখ্যা))
    ফেরত __builtin_ctz (v);
#endif

#যদি hb_has_builtin(__builtin_ctzl)
  যদি (আকার (T) <= আকার (unsigned long))
    ফেরত __builtin_ctzl (v);
#endif

#যদি hb_has_builtin(__builtin_ctzll)
  যদি (আকার (T) <= আকার (unsigned long long))
    ফেরত __builtin_ctzll (v);
#endif

#যদি (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  যদি (আকার (T) <= আকার (unsigned পূর্ণসংখ্যা))
  {
    unsigned long where;
    _BitScanForward (&where, v);
    ফেরত where;
  }
# যদি defined(_WIN64)
  যদি (আকার (T) <= 8)
  {
    unsigned long where;
    _BitScanForward64 (&where, v);
    ফেরত where;
  }
# endif
#endif

  যদি (আকার (T) <= 4)
  {
    /* "bithacks" */
    unsigned পূর্ণসংখ্যা c = 32;
    v &= - (int32_t) v;
    যদি (v) c--;
    যদি (v & 0x0000FFFF) c -= 16;
    যদি (v & 0x00FF00FF) c -= 8;
    যদি (v & 0x0F0F0F0F) c -= 4;
    যদি (v & 0x33333333) c -= 2;
    যদি (v & 0x55555555) c -= 1;
    ফেরত c;
  }
  যদি (আকার (T) <= 8)
  {
    /* "bithacks" */
    unsigned পূর্ণসংখ্যা c = 64;
    v &= - (int64_t) (v);
    যদি (v) c--;
    যদি (v & 0x00000000FFFFFFFFULL) c -= 32;
    যদি (v & 0x0000FFFF0000FFFFULL) c -= 16;
    যদি (v & 0x00FF00FF00FF00FFULL) c -= 8;
    যদি (v & 0x0F0F0F0F0F0F0F0FULL) c -= 4;
    যদি (v & 0x3333333333333333ULL) c -= 2;
    যদি (v & 0x5555555555555555ULL) c -= 1;
    ফেরত c;
  }
  যদি (আকার (T) == 16)
  {
    unsigned পূর্ণসংখ্যা shift = 64;
    ফেরত (uint64_t) v ? hb_bit_storage<uint64_t> ((uint64_t) v) :
			  hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift;
  }

  প্রতিজ্ঞা (0);
  ফেরত 0; /* Shut up stupid compiler. */
}


/*
 * Tiny stuff.
 */

/* ASCII tag/character handling */
স্থির অন্তঃরেখ যৌক্তিক ISALPHA (unsigned অক্ষর c)
{ ফেরত (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
স্থির অন্তঃরেখ যৌক্তিক ISALNUM (unsigned অক্ষর c)
{ ফেরত (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }
স্থির অন্তঃরেখ যৌক্তিক ISSPACE (unsigned অক্ষর c)
{ ফেরত c == ' ' || c =='\f'|| c =='\n'|| c =='\r'|| c =='\t'|| c =='\v'; }
স্থির অন্তঃরেখ unsigned অক্ষর TOUPPER (unsigned অক্ষর c)
{ ফেরত (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c; }
স্থির অন্তঃরেখ unsigned অক্ষর TOLOWER (unsigned অক্ষর c)
{ ফেরত (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }
স্থির অন্তঃরেখ যৌক্তিক ISHEX (unsigned অক্ষর c)
{ ফেরত (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
স্থির অন্তঃরেখ unsigned অক্ষর TOHEX (uint8_t c)
{ ফেরত (c & 0xF) <= 9 ? (c & 0xF) + '0' : (c & 0xF) + 'a' - 10; }
স্থির অন্তঃরেখ uint8_t FROMHEX (unsigned অক্ষর c)
{ ফেরত (c >= '0' && c <= '9') ? c - '0' : TOLOWER (c) - 'a' + 10; }

স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা DIV_CEIL (ধ্রুবক unsigned পূর্ণসংখ্যা a, unsigned পূর্ণসংখ্যা b)
{ ফেরত (a + (b - 1)) / b; }


#undef  ARRAY_দৈর্ঘ্য
template <typename Type, unsigned পূর্ণসংখ্যা n>
স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা ARRAY_দৈর্ঘ্য (ধ্রুবক Type (&)[n]) { ফেরত n; }
/* A const version, but does not detect erratically being called on pointers. */
#define ARRAY_দৈর্ঘ্য_CONST(__array) ((signed পূর্ণসংখ্যা) (আকার (__array) / আকার (__array[0])))


স্থির অন্তঃরেখ শূন্য *
hb_memcpy (শূন্য *__restrict dst, ধ্রুবক শূন্য *__restrict src, size_t len)
{
  /* It's illegal to pass 0 as size to memcpy. */
  যদি (unlikely (!len)) ফেরত dst;
  ফেরত memcpy (dst, src, len);
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
hb_memcmp (ধ্রুবক শূন্য *a, ধ্রুবক শূন্য *b, unsigned পূর্ণসংখ্যা len)
{
  /* It's illegal to pass NULL to memcmp(), even if len is zero.
   * So, wrap it.
   * https://sourceware.org/bugzilla/show_bug.cgi?id=23878 */
  যদি (unlikely (!len)) ফেরত 0;
  ফেরত memcmp (a, b, len);
}

স্থির অন্তঃরেখ শূন্য *
hb_memset (শূন্য *s, পূর্ণসংখ্যা c, unsigned পূর্ণসংখ্যা n)
{
  /* It's illegal to pass NULL to memset(), even if n is zero. */
  যদি (unlikely (!n)) ফেরত s;
  ফেরত memset (s, c, n);
}

স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা
hb_ceil_to_4 (unsigned পূর্ণসংখ্যা v)
{
  ফেরত ((v - 1) | 3) + 1;
}

template <typename T> স্থির অন্তঃরেখ যৌক্তিক
hb_in_range (T u, T lo, T hi)
{
  static_assert (!std::is_signed<T>::value, "");

  /* The casts below are important as if T is smaller than int,
   * the subtract results will become a signed int! */
  ফেরত (T)(u - lo) <= (T)(hi - lo);
}
template <typename T> স্থির অন্তঃরেখ যৌক্তিক
hb_in_ranges (T u, T lo1, T hi1)
{
  ফেরত hb_in_range (u, lo1, hi1);
}
template <typename T, typename ...Ts> স্থির অন্তঃরেখ যৌক্তিক
hb_in_ranges (T u, T lo1, T hi1, Ts... ds)
{
  ফেরত hb_in_range<T> (u, lo1, hi1) || hb_in_ranges<T> (u, ds...);
}


/*
 * Overflow checking.
 */

স্থির অন্তঃরেখ যৌক্তিক
hb_unsigned_mul_overflows (unsigned পূর্ণসংখ্যা গুনতি, unsigned পূর্ণসংখ্যা আকার_2, unsigned *ফলাফল = nullptr)
{
#যদি hb_has_builtin(__builtin_mul_overflow)
  unsigned স্তূপ_ফলাফল;
  যদি (!ফলাফল)
    ফলাফল = &স্তূপ_ফলাফল;
  ফেরত __builtin_mul_overflow (গুনতি, আকার_2, ফলাফল);
#endif

  যদি (ফলাফল)
    *ফলাফল = গুনতি * আকার_2;
  ফেরত (আকার_2 > 0) && (গুনতি >= ((unsigned পূর্ণসংখ্যা) -1) / আকার_2);
}

স্থির অন্তঃরেখ যৌক্তিক
hb_unsigned_add_overflows (unsigned পূর্ণসংখ্যা a, unsigned পূর্ণসংখ্যা b, unsigned *ফলাফল = nullptr)
{
#যদি hb_has_builtin(__builtin_add_overflow)
  unsigned স্তূপ_ফলাফল;
  যদি (!ফলাফল)
    ফলাফল = &স্তূপ_ফলাফল;
  ফেরত __builtin_add_overflow (a, b, ফলাফল);
#endif

  যদি (ফলাফল)
    *ফলাফল = a + b;
  ফেরত b > (unsigned পূর্ণসংখ্যা) -1 - a;
}

/* Saturating arithmetic on size_t.  On platforms where size_t is wider than
 * unsigned int (i.e. 64-bit), the inputs to get_size()-style computations
 * (counts and static_sizes which are at most 32-bit) cannot overflow size_t,
 * so these reduce to plain arithmetic.  On 32-bit platforms (size_t is
 * unsigned int), they saturate to SIZE_MAX so sanitize/serialize callers
 * naturally reject the resulting size as out-of-range. */

স্থির অন্তঃরেখ size_t
hb_unsigned_mul_saturate (size_t a, size_t b)
{
  যদি (আকার (size_t) > আকার (unsigned পূর্ণসংখ্যা))
    ফেরত a * b;
#যদি hb_has_builtin(__builtin_mul_overflow)
  size_t ফলাফল;
  যদি (__builtin_mul_overflow (a, b, &ফলাফল))
    ফেরত (size_t) -1;
  ফেরত ফলাফল;
#অন্যথা
  যদি (b > 0 && a > ((size_t) -1) / b) ফেরত (size_t) -1;
  ফেরত a * b;
#endif
}

স্থির অন্তঃরেখ size_t
hb_unsigned_add_saturate (size_t a, size_t b)
{
  যদি (আকার (size_t) > আকার (unsigned পূর্ণসংখ্যা))
    ফেরত a + b;
#যদি hb_has_builtin(__builtin_add_overflow)
  size_t ফলাফল;
  যদি (__builtin_add_overflow (a, b, &ফলাফল))
    ফেরত (size_t) -1;
  ফেরত ফলাফল;
#অন্যথা
  যদি (b > ((size_t) -1) - a) ফেরত (size_t) -1;
  ফেরত a + b;
#endif
}

/* Variadic forms: fold left across all arguments. */
template <typename ...Ts>
স্থির অন্তঃরেখ size_t
hb_unsigned_mul_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ ফেরত hb_unsigned_mul_saturate (hb_unsigned_mul_saturate (a, b), c, rest...); }

template <typename ...Ts>
স্থির অন্তঃরেখ size_t
hb_unsigned_add_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ ফেরত hb_unsigned_add_saturate (hb_unsigned_add_saturate (a, b), c, rest...); }

/* Saturating mul-add: a * b + c.  Covers the dominant get_size() pattern
 * `count * static_size + min_size`. */
স্থির অন্তঃরেখ size_t
hb_unsigned_mul_add_saturate (size_t a, size_t b, size_t c)
{ ফেরত hb_unsigned_add_saturate (hb_unsigned_mul_saturate (a, b), c); }


/*
 * Sort and search.
 */

template <typename K, typename V, typename ...Ts>
স্থির পূর্ণসংখ্যা
_hb_cmp_method (ধ্রুবক শূন্য *pkey, ধ্রুবক শূন্য *pval, Ts... ds)
{
  ধ্রুবক K& key = * (ধ্রুবক K*) pkey;
  ধ্রুবক V& val = * (ধ্রুবক V*) pval;

  ফেরত val.cmp (key, ds...);
}

template <typename K, typename V>
স্থির পূর্ণসংখ্যা
_hb_cmp_operator (ধ্রুবক শূন্য *pkey, ধ্রুবক শূন্য *pval)
{
  ধ্রুবক K& key = * (ধ্রুবক K*) pkey;
  ধ্রুবক V& val = * (ধ্রুবক V*) pval;

  যদি (key < val) ফেরত -1;
  যদি (key > val) ফেরত  1;
  ফেরত 0;
}

template <typename V, typename K, typename ...Ts>
HB_HOT
স্থির অন্তঃরেখ যৌক্তিক
hb_bsearch_impl (unsigned *pos, /* Out */
		 ধ্রুবক K& key,
		 V* base, size_t nmemb, size_t stride,
		 পূর্ণসংখ্যা (*compar)(ধ্রুবক শূন্য *চাবি_3, ধ্রুবক শূন্য *_item, Ts... _ds),
		 Ts... ds)
{
  /* This is our *only* bsearch implementation. */

  পূর্ণসংখ্যা min = 0, max = (পূর্ণসংখ্যা) nmemb - 1;
  যখন (min <= max)
  {
    পূর্ণসংখ্যা mid = ((unsigned পূর্ণসংখ্যা) min + (unsigned পূর্ণসংখ্যা) max) / 2;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    V* p = (V*) (((ধ্রুবক অক্ষর *) base) + (mid * stride));
#pragma GCC diagnostic বের_করো
    পূর্ণসংখ্যা c = compar ((ধ্রুবক শূন্য *) std::addressof (key), (ধ্রুবক শূন্য *) p, ds...);
    যদি (c < 0)
      max = mid - 1;
    অন্যথা যদি (c > 0)
      min = mid + 1;
    অন্যথা
    {
      *pos = mid;
      ফেরত সত্য;
    }
  }
  *pos = min;
  ফেরত মিথ্যা;
}

template <typename V, typename K>
স্থির অন্তঃরেখ V*
hb_bsearch (ধ্রুবক K& key, V* base,
	    size_t nmemb, size_t stride = আকার (V),
	    পূর্ণসংখ্যা (*compar)(ধ্রুবক শূন্য *চাবি_3, ধ্রুবক শূন্য *_item) = _hb_cmp_method<K, V>)
{
  unsigned pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  ফেরত hb_bsearch_impl (&pos, key, base, nmemb, stride, compar) ?
	 (V*) (((ধ্রুবক অক্ষর *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic বের_করো
}
template <typename V, typename K, typename ...Ts>
স্থির অন্তঃরেখ V*
hb_bsearch (ধ্রুবক K& key, V* base,
	    size_t nmemb, size_t stride,
	    পূর্ণসংখ্যা (*compar)(ধ্রুবক শূন্য *চাবি_3, ধ্রুবক শূন্য *_item, Ts... _ds),
	    Ts... ds)
{
  unsigned pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  ফেরত hb_bsearch_impl (&pos, key, base, nmemb, stride, compar, ds...) ?
	 (V*) (((ধ্রুবক অক্ষর *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic বের_করো
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
স্থির অন্তঃরেখ শূন্য
hb_qsort_loop (T *base, size_t nel, Compar compar)
{
  যখন (nel > 24)
  {
    T *last = base + nel - 1;
    T *mid = base + nel / 2;

    /* Median-of-three pivot, parked at last-1. */
    যদি (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    যদি (compar (*mid, *last) > 0)
    {
      hb_swap (*mid, *last);
      যদি (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    }
    hb_swap (*mid, *(last - 1));
    T &pivot = *(last - 1);

    /* Two-way Hoare partition.  Inner loops are unguarded:
     * median-of-three left *base <= pivot and *last >= pivot,
     * which act as sentinels. */
    T *i = base, *j = last - 1;
    যখন (সত্য)
    {
      যখন (compar (*++i, pivot) < 0) {}
      যখন (compar (*--j, pivot) > 0) {}
      যদি (i >= j) বিরতি;
      hb_swap (*i, *j);
    }
    hb_swap (*i, *(last - 1));

    /* Recurse on smaller side, loop on larger — bounds stack
     * depth at O(log n). */
    size_t left  = (size_t) (i - base);
    size_t ডান = nel - left - 1;
    যদি (left < ডান)
    {
      hb_qsort_loop (base, left, compar);
      base = i + 1;
      nel  = ডান;
    }
    অন্যথা
    {
      hb_qsort_loop (i + 1, ডান, compar);
      nel  = left;
    }
  }
}

template <typename T, typename Compar>
স্থির অন্তঃরেখ শূন্য
hb_qsort_inline (T *base, size_t nel, Compar compar)
{
  hb_qsort_loop (base, nel, compar);

  /* Single final insertion sort over the whole array.  After
   * the partitioning loop, every element is within the threshold
   * of its sorted position, so this pass is O(n * threshold). */
  T *end = base + nel;
  জন্য (T *pi = base + 1; pi < end; pi++)
    জন্য (T *pj = pi; pj > base && compar (pj[-1], pj[0]) > 0; pj--)
      hb_swap (pj[-1], pj[0]);
}

স্থির অন্তঃরেখ শূন্য
hb_qsort (শূন্য *base, size_t nel, size_t width,
	  পূর্ণসংখ্যা (*compar)(ধ্রুবক শূন্য *_a, ধ্রুবক শূন্য *_b))
{
  qsort (base, nel, width, compar);
}




template <typename T, typename T2, typename T3 = পূর্ণসংখ্যা> স্থির অন্তঃরেখ শূন্য
hb_stable_sort (T *array, unsigned পূর্ণসংখ্যা len, পূর্ণসংখ্যা(*compar)(ধ্রুবক T2 *, ধ্রুবক T2 *), T3 *array2 = nullptr)
{
  static_assert (hb_is_trivially_copy_assignable (T), "");
  static_assert (hb_is_trivially_copy_assignable (T3), "");

  জন্য (unsigned পূর্ণসংখ্যা i = 1; i < len; i++)
  {
    unsigned পূর্ণসংখ্যা j = i;
    যখন (j && compar (&array[j - 1], &array[i]) > 0)
      j--;
    যদি (i == j)
      চালু;
    /* Move item i to occupy place for item j, shift what's in between. */
    {
      T t = array[i];
      memmove (&array[j + 1], &array[j], (i - j) * আকার (T));
      array[j] = t;
    }
    যদি (array2)
    {
      T3 t = array2[i];
      memmove (&array2[j + 1], &array2[j], (i - j) * আকার (T3));
      array2[j] = t;
    }
  }
}

স্থির অন্তঃরেখ hb_bool_t
hb_codepoint_parse (ধ্রুবক অক্ষর *s, unsigned পূর্ণসংখ্যা len, পূর্ণসংখ্যা base, hb_codepoint_t *out)
{
  unsigned পূর্ণসংখ্যা v;
  ধ্রুবক অক্ষর *p = s;
  ধ্রুবক অক্ষর *end = p + len;
  যদি (unlikely (!hb_parse_uint (&p, end, &v, সত্য/* whole buffer */, base)))
    ফেরত মিথ্যা;

  *out = v;
  ফেরত সত্য;
}


/* Operators. */

কাঠামো
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ধ্রুবক T &a, ধ্রুবক T &b) ধ্রুবক HB_AUTO_RETURN (a & b)
}
HB_FUNCOBJ (hb_bitwise_and);
কাঠামো
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ধ্রুবক T &a, ধ্রুবক T &b) ধ্রুবক HB_AUTO_RETURN (a | b)
}
HB_FUNCOBJ (hb_bitwise_or);
কাঠামো
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ধ্রুবক T &a, ধ্রুবক T &b) ধ্রুবক HB_AUTO_RETURN (a ^ b)
}
HB_FUNCOBJ (hb_bitwise_xor);
কাঠামো
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ধ্রুবক T &a, ধ্রুবক T &b) ধ্রুবক HB_AUTO_RETURN (~a & b)
}
HB_FUNCOBJ (hb_bitwise_lt);
কাঠামো
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ধ্রুবক T &a, ধ্রুবক T &b) ধ্রুবক HB_AUTO_RETURN (a & ~b)
}
HB_FUNCOBJ (hb_bitwise_gt); // aka sub
কাঠামো
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ধ্রুবক T &a, ধ্রুবক T &b) ধ্রুবক HB_AUTO_RETURN (~a | b)
}
HB_FUNCOBJ (hb_bitwise_le);
কাঠামো
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ধ্রুবক T &a, ধ্রুবক T &b) ধ্রুবক HB_AUTO_RETURN (a | ~b)
}
HB_FUNCOBJ (hb_bitwise_ge);
কাঠামো
{
  template <typename T> constexpr auto
  operator () (ধ্রুবক T &a) ধ্রুবক HB_AUTO_RETURN (~a)
}
HB_FUNCOBJ (hb_bitwise_neg);

কাঠামো
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ধ্রুবক T &a, ধ্রুবক T2 &b) ধ্রুবক HB_AUTO_RETURN (a + b)
}
HB_FUNCOBJ (hb_add);
কাঠামো
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ধ্রুবক T &a, ধ্রুবক T2 &b) ধ্রুবক HB_AUTO_RETURN (a - b)
}
HB_FUNCOBJ (hb_sub);
কাঠামো
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ধ্রুবক T &a, ধ্রুবক T2 &b) ধ্রুবক HB_AUTO_RETURN (b - a)
}
HB_FUNCOBJ (hb_rsub);
কাঠামো
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ধ্রুবক T &a, ধ্রুবক T2 &b) ধ্রুবক HB_AUTO_RETURN (a * b)
}
HB_FUNCOBJ (hb_mul);
কাঠামো
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ধ্রুবক T &a, ধ্রুবক T2 &b) ধ্রুবক HB_AUTO_RETURN (a / b)
}
HB_FUNCOBJ (hb_div);
কাঠামো
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ধ্রুবক T &a, ধ্রুবক T2 &b) ধ্রুবক HB_AUTO_RETURN (a % b)
}
HB_FUNCOBJ (hb_mod);
কাঠামো
{
  template <typename T> constexpr auto
  operator () (ধ্রুবক T &a) ধ্রুবক HB_AUTO_RETURN (+a)
}
HB_FUNCOBJ (hb_pos);
কাঠামো
{
  template <typename T> constexpr auto
  operator () (ধ্রুবক T &a) ধ্রুবক HB_AUTO_RETURN (-a)
}
HB_FUNCOBJ (hb_neg);
কাঠামো
{
  template <typename T> constexpr auto
  operator () (T &a) ধ্রুবক HB_AUTO_RETURN (++a)
}
HB_FUNCOBJ (hb_inc);
কাঠামো
{
  template <typename T> constexpr auto
  operator () (T &a) ধ্রুবক HB_AUTO_RETURN (--a)
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
দ্বিগুণ solve_itp (func_t f,
		  দ্বিগুণ a, দ্বিগুণ b,
		  দ্বিগুণ epsilon,
		  দ্বিগুণ min_y, দ্বিগুণ max_y,
		  দ্বিগুণ &ya, দ্বিগুণ &yb, দ্বিগুণ &y)
{
  // Guard against degenerate interval
  যদি (b - a <= 0.0)
  {
    y = ya;
    ফেরত a;
  }

  unsigned n1_2 = (unsigned) (hb_max (ceil (log2 ((b - a) / epsilon)) - 1.0, 0.0));
  ধ্রুবক unsigned n0 = 1; // Hardwired
  ধ্রুবক দ্বিগুণ k1 = 0.2 / (b - a); // Hardwired.
  unsigned nmax = n0 + n1_2;
  দ্বিগুণ scaled_epsilon = epsilon * দ্বিগুণ (1llu << nmax);
  দ্বিগুণ _2_epsilon = 2.0 * epsilon;
  যখন (b - a > _2_epsilon)
  {
    দ্বিগুণ x1_2 = 0.5 * (a + b);
    দ্বিগুণ r = scaled_epsilon - 0.5 * (b - a);
    // Guard against yb == ya to prevent division by zero
    দ্বিগুণ xf = (yb != ya) ? (yb * a - ya * b) / (yb - ya) : x1_2;
    দ্বিগুণ sigma = x1_2 - xf;
    দ্বিগুণ b_a = b - a;
    // This has k2 = 2 hardwired for efficiency.
    দ্বিগুণ b_a_k2 = b_a * b_a;
    দ্বিগুণ delta = k1 * b_a_k2;
    পূর্ণসংখ্যা sigma_sign = sigma >= 0 ? +1 : -1;
    দ্বিগুণ xt = delta <= fabs (x1_2 - xf) ? xf + delta * sigma_sign : x1_2;
    দ্বিগুণ xitp = fabs (xt - x1_2) <= r ? xt : x1_2 - r * sigma_sign;
    দ্বিগুণ yitp = f (xitp);
    যদি (yitp > max_y)
    {
      b = xitp;
      yb = yitp;
    }
    অন্যথা যদি (yitp < min_y)
    {
      a = xitp;
      ya = yitp;
    }
    অন্যথা
    {
      y = yitp;
      ফেরত xitp;
    }
    scaled_epsilon *= 0.5;
  }
  ফেরত 0.5 * (a + b);
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
কাঠামো hb_scope_guard_t
{
  explicit hb_scope_guard_t (F &&f) : f (std::move (f)), active (সত্য) {}
  hb_scope_guard_t (hb_scope_guard_t &&o) noexcept
    : f (std::move (o.f)), active (o.active) { o.active = মিথ্যা; }
  hb_scope_guard_t (ধ্রুবক hb_scope_guard_t &) = delete;
  hb_scope_guard_t &operator= (ধ্রুবক hb_scope_guard_t &) = delete;
  hb_scope_guard_t &operator= (hb_scope_guard_t &&) = delete;
  ~hb_scope_guard_t () { যদি (active) f (); }

  /* Release: dismiss the guard so the cleanup does NOT run.  Use
   * when transferring ownership out of the scope. */
  শূন্য release () { active = মিথ্যা; }

  ব্যক্তিগত:
  F f;
  যৌক্তিক active;
};

template <typename F>
স্থির অন্তঃরেখ hb_scope_guard_t<F> hb_make_scope_guard (F &&f)
{ ফেরত hb_scope_guard_t<F> (std::forward<F> (f)); }

#define HB_SCOPE_GUARD_NAME_(সারি) hb_scope_guard_##line
#define HB_SCOPE_GUARD_NAME(সারি) HB_SCOPE_GUARD_NAME_(সারি)
#define HB_SCOPE_GUARD(stmt) \
  auto HB_SCOPE_GUARD_NAME(__LINE__) = \
    hb_make_scope_guard ([&]() { stmt; }); \
  (শূন্য) HB_SCOPE_GUARD_NAME(__LINE__)


#endif /* HB_ALGS_HH */
