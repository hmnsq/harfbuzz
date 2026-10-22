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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-meta.hh"
#அந்தர்பாவ "hb-null.hh"
#அந்தர்பாவ "hb-number.hh"

#அந்தர்பாவ <algorithm>
#அந்தர்பாவ <initializer_list>
#அந்தர்பாவ <functional>
#அந்தர்பாவ <new>

நிலையான HB_ALWAYS_INLINE வெறுமை *
hb_malloc2 (size_t nmemb, size_t size)
{
  எனில் (size && nmemb > SIZE_MAX / size) திரும்பு nullptr;
  திரும்பு hb_malloc (nmemb * size);
}
நிலையான HB_ALWAYS_INLINE வெறுமை *
hb_realloc2 (வெறுமை *ptr, size_t nmemb, size_t size)
{
  எனில் (size && nmemb > SIZE_MAX / size) திரும்பு nullptr;
  திரும்பு hb_realloc (ptr, nmemb * size);
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
	வெளி "C++" { \
	  நிலையான உள்வரி constexpr T operator | (T l, T r) { திரும்பு T ((unsigned) l | (unsigned) r); } \
	  நிலையான உள்வரி constexpr T operator & (T l, T r) { திரும்பு T ((unsigned) l & (unsigned) r); } \
	  நிலையான உள்வரி constexpr T operator ^ (T l, T r) { திரும்பு T ((unsigned) l ^ (unsigned) r); } \
	  நிலையான உள்வரி constexpr unsigned operator ~ (T r) { திரும்பு (~(unsigned) r); } \
	  நிலையான உள்வரி T& operator |= (T &l, T r) { l = l | r; திரும்பு l; } \
	  நிலையான உள்வரி T& operator &= (T& l, T r) { l = l & r; திரும்பு l; } \
	  நிலையான உள்வரி T& operator ^= (T& l, T r) { l = l ^ r; திரும்பு l; } \
	} \
	static_assert (உண்மை, "")

/* Useful for set-operations on small enums.
 * For example, for testing "x ∈ {x1, x2, x3}" use:
 * (FLAG_UNSAFE(x) & (FLAG(x1) | FLAG(x2) | FLAG(x3)))
 */
#define FLAG(x) (static_assert_கோவை ((unsigned)(x) < 32) + (((uint32_t) 1U) << (unsigned)(x)))
#define FLAG_UNSAFE(x) ((unsigned)(x) < 32 ? (((uint32_t) 1U) << (unsigned)(x)) : 0)
#define FLAG_RANGE(x,y) (static_assert_கோவை ((x) < (y)) + FLAG(y+1) - FLAG(x))
#define FLAG64(x) (static_assert_கோவை ((unsigned)(x) < 64) + (((uint64_t) 1ULL) << (unsigned)(x)))
#define FLAG64_UNSAFE(x) ((unsigned)(x) < 64 ? (((uint64_t) 1ULL) << (unsigned)(x)) : 0)


/*
 * Fixed-endian integers / floats.
 */


/* Endian swap, used in Windows related backends */
நிலையான உள்வரி constexpr uint16_t hb_uint16_swap (uint16_t v)
{ திரும்பு (v >> 8) | (v << 8); }
நிலையான உள்வரி constexpr uint32_t hb_uint32_swap (uint32_t v)
{ திரும்பு (hb_uint16_swap (v) << 16) | hb_uint16_swap (v >> 16); }

template <typename Type>
கட்டமைப்பு __attribute__((packed)) hb_packed_t
{
  hb_packed_t () = இயல்பு;
  constexpr hb_packed_t (Type V) : v (V) {}
  operator Type () மாறிலி { திரும்பு v; }
  hb_packed_t & operator = (Type V) { v = V; திரும்பு *இது; }

  தனிப்பட்ட:
  Type v;
};

#ifndef HB_FAST_இலக்கம்_ACCESS

#எனில் defined(__OPTIMIZE__) && \
    defined(__BYTE_ORDER) && \
    (__BYTE_ORDER == __BIG_ENDIAN || \
     (__BYTE_ORDER == __LITTLE_ENDIAN && \
      hb_has_builtin(__builtin_bswap16) && \
      hb_has_builtin(__builtin_bswap32) && \
      hb_has_builtin(__builtin_bswap64)))
#define HB_FAST_இலக்கம்_ACCESS 1
#இல்லையெனில்
#define HB_FAST_இலக்கம்_ACCESS 0
#endif

// https://github.com/harfbuzz/harfbuzz/issues/5456
#எனில் defined(__GNUC__) && !defined(__clang__) && (__GNUC__ <= 12)
#undef HB_FAST_இலக்கம்_ACCESS
#define HB_FAST_இலக்கம்_ACCESS 0
#endif

#endif

template <தருக்க BE, typename Type, முழு Bytes = அளவு (Type)>
கட்டமைப்பு HBInt;
template <தருக்க BE, typename Type>
கட்டமைப்பு HBInt<BE, Type, 1>
{
  பொது:
  HBInt () = இயல்பு;
  constexpr HBInt (Type V) : v {uint8_t (V)} {}
  constexpr operator Type () மாறிலி { திரும்பு v; }
  தனிப்பட்ட: uint8_t v;
};
template <தருக்க BE, typename Type>
கட்டமைப்பு HBInt<BE, Type, 2>
{
  பொது:
  HBInt () = இயல்பு;

  HBInt (Type V)
#எனில் HB_FAST_இலக்கம்_ACCESS
  {
    எனில் (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint16_t> *) v) = V;
    இல்லையெனில்
      *((hb_packed_t<uint16_t> *) v) = __builtin_bswap16 (V);
  }
#இல்லையெனில்
    : v {BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >>  8) & 0xFF)} {}
#endif

  constexpr operator Type () மாறிலி
  {
#எனில் HB_FAST_இலக்கம்_ACCESS
    திரும்பு (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint16_t) *((மாறிலி hb_packed_t<uint16_t> *) v)
    :
      __builtin_bswap16 ((uint16_t) *((மாறிலி hb_packed_t<uint16_t> *) v))
    ;
#இல்லையெனில்
    திரும்பு (BE ? (v[0] <<  8) : (v[0]      ))
	 + (BE ? (v[1]      ) : (v[1] <<  8));
#endif
  }
  தனிப்பட்ட: uint8_t v[2];
};
template <தருக்க BE, typename Type>
கட்டமைப்பு HBInt<BE, Type, 3>
{
  static_assert (!std::is_signed<Type>::value, "");
  பொது:
  HBInt () = இயல்பு;
  constexpr HBInt (Type V) : v {BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
				BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
				BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V      ) & 0xFF)} {}

  constexpr operator Type () மாறிலி { திரும்பு (BE ? (v[0] << 16) : (v[0]      ))
					  + (BE ? (v[1] <<  8) : (v[1] <<  8))
					  + (BE ? (v[2]      ) : (v[2] << 16)); }
  தனிப்பட்ட: uint8_t v[3];
};
template <தருக்க BE, typename Type>
கட்டமைப்பு HBInt<BE, Type, 4>
{
  template <தருக்க, typename, முழு>
  friend கட்டமைப்பு HBFloat;

  பொது:
  HBInt () = இயல்பு;

  HBInt (Type V)
#எனில் HB_FAST_இலக்கம்_ACCESS
  {
    எனில் (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint32_t> *) v) = V;
    இல்லையெனில்
      *((hb_packed_t<uint32_t> *) v) = __builtin_bswap32 (V);
  }
#இல்லையெனில்
    : v {BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 24) & 0xFF)} {}
#endif

  constexpr operator Type () மாறிலி {
#எனில் HB_FAST_இலக்கம்_ACCESS
    திரும்பு (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint32_t) *((மாறிலி hb_packed_t<uint32_t> *) v)
    :
      __builtin_bswap32 ((uint32_t) *((மாறிலி hb_packed_t<uint32_t> *) v))
    ;
#இல்லையெனில்
    திரும்பு (BE ? (uint32_t (v[0]) << 24) : (uint32_t (v[0])      ))
	 + (BE ? (uint32_t (v[1]) << 16) : (uint32_t (v[1]) <<  8))
	 + (BE ? (uint32_t (v[2]) <<  8) : (uint32_t (v[2]) << 16))
	 + (BE ? (uint32_t (v[3])      ) : (uint32_t (v[3]) << 24));
#endif
  }
  தனிப்பட்ட: uint8_t v[4];
};
template <தருக்க BE, typename Type>
கட்டமைப்பு HBInt<BE, Type, 8>
{
  template <தருக்க, typename, முழு>
  friend கட்டமைப்பு HBFloat;

  பொது:
  HBInt () = இயல்பு;

  HBInt (Type V)
#எனில் HB_FAST_இலக்கம்_ACCESS
  {
    எனில் (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint64_t> *) v) = V;
    இல்லையெனில்
      *((hb_packed_t<uint64_t> *) v) = __builtin_bswap64 (V);
  }
#இல்லையெனில்
    : v {BE ? uint8_t ((V >> 56) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 48) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >> 40) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V >> 32) & 0xFF) : uint8_t ((V >> 24) & 0xFF),
	 BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V >> 32) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 40) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 48) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 56) & 0xFF)} {}
#endif

  constexpr operator Type () மாறிலி {
#எனில் HB_FAST_இலக்கம்_ACCESS
    திரும்பு (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint64_t) *((மாறிலி hb_packed_t<uint64_t> *) v)
    :
      __builtin_bswap64 ((uint64_t) *((மாறிலி hb_packed_t<uint64_t> *) v))
    ;
#இல்லையெனில்
    திரும்பு (BE ? (uint64_t (v[0]) << 56) : (uint64_t (v[0])      ))
	 + (BE ? (uint64_t (v[1]) << 48) : (uint64_t (v[1]) <<  8))
	 + (BE ? (uint64_t (v[2]) << 40) : (uint64_t (v[2]) << 16))
	 + (BE ? (uint64_t (v[3]) << 32) : (uint64_t (v[3]) << 24))
	 + (BE ? (uint64_t (v[4]) << 24) : (uint64_t (v[4]) << 32))
	 + (BE ? (uint64_t (v[5]) << 16) : (uint64_t (v[5]) << 40))
	 + (BE ? (uint64_t (v[6]) <<  8) : (uint64_t (v[6]) << 48))
	 + (BE ? (uint64_t (v[7])      ) : (uint64_t (v[7]) << 56));
#endif
  }
  தனிப்பட்ட: uint8_t v[8];
};

/* Floats. */

template <தருக்க BE, typename Type, முழு Bytes>
கட்டமைப்பு HBFloat
{
  using IntType = typename std::conditional<Bytes == 4, uint32_t, uint64_t>::type;

  பொது:
  HBFloat () = இயல்பு;

  HBFloat (Type V)
  {
#எனில் HB_FAST_இலக்கம்_ACCESS
    {
	      எனில் (BE == (__BYTE_ORDER == __BIG_ENDIAN))
	      {
	        *((hb_packed_t<Type> *) v) = V;
	        திரும்பு;
	      }
	    }
#endif

    ஒன்றிப்பு {
      hb_packed_t<Type> f;
      hb_packed_t<IntType> i;
    } u = {{V}};

    மாறிலி HBInt<BE, IntType> I = (IntType) u.i;
    க்கு (unsigned i = 0; i < Bytes; i++)
      v[i] = I.v[i];
  }

  /* c++14 constexpr */ operator Type () மாறிலி
  {
#எனில் HB_FAST_இலக்கம்_ACCESS
	    {
	      எனில் (BE == (__BYTE_ORDER == __BIG_ENDIAN))
		திரும்பு (Type) *((மாறிலி hb_packed_t<Type> *) v);
	    }
#endif

    HBInt<BE, IntType> I;
    க்கு (unsigned i = 0; i < Bytes; i++)
      I.v[i] = v[i];

    ஒன்றிப்பு {
      hb_packed_t<IntType> i;
      hb_packed_t<Type> f;
    } u = {{I}};

    திரும்பு (Type) u.f;
  }
  தனிப்பட்ட: uint8_t v[Bytes];
};


/* We want our rounding towards +infinity. */
நிலையான உள்வரி இரட்டை
_hb_roundf (இரட்டை x) { திரும்பு floor (x + .5); }

நிலையான உள்வரி தசம
_hb_roundf (தசம x) { திரும்பு floor ((இரட்டை) x + .5); }

#define roundf(x) _hb_roundf(x)

நிலையான உள்வரி வெறுமை
hb_sincos (தசம rotation, தசம &s, தசம &c)
{
#ifdef HAVE_SINCOSF
  sincosf (rotation, &s, &c);
#இல்லையெனில்
  c = cosf (rotation);
  s = sinf (rotation);
#endif
}
நிலையான உள்வரி வெறுமை
hb_sincos (இரட்டை rotation, இரட்டை &s, இரட்டை &c)
{
#ifdef HAVE_SINCOS
  sincos (rotation, &s, &c);
#இல்லையெனில்
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


கட்டமைப்பு
{
  /* Note.  This is dangerous in that if it's passed an rvalue, it returns rvalue-reference. */
  template <typename T> constexpr auto
  operator () (T&& v) மாறிலி HB_AUTO_RETURN ( std::forward<T> (v) )
}
HB_FUNCOBJ (hb_identity);
கட்டமைப்பு
{
  /* Like identity(), but only retains lvalue-references.  Rvalues are returned as rvalues. */
  template <typename T> constexpr T&
  operator () (T& v) மாறிலி { திரும்பு v; }

  template <typename T> constexpr hb_remove_reference<T>
  operator () (T&& v) மாறிலி { திரும்பு v; }
}
HB_FUNCOBJ (hb_lidentity);
கட்டமைப்பு
{
  /* Like identity(), but always returns rvalue. */
  template <typename T> constexpr hb_remove_reference<T>
  operator () (T&& v) மாறிலி { திரும்பு v; }
}
HB_FUNCOBJ (hb_ridentity);

கட்டமைப்பு
{
  template <typename T> constexpr தருக்க
  operator () (T&& v) மாறிலி { திரும்பு தருக்க (std::forward<T> (v)); }
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
			(வெறுமை) ((h) ^= (h) >> 23),		\
			(வெறுமை) ((h) *= 0x2127599bf4325c37ULL),	\
			(h) ^= (h) >> 47)

நிலையான உள்வரி uint64_t fasthash64(மாறிலி வெறுமை *buf, size_t len, uint64_t seed)
{
	கட்டமைப்பு __attribute__((packed)) packed_uint64_t { uint64_t v; };
	மாறிலி uint64_t    m = 0x880355f21e6d1965ULL;
	மாறிலி packed_uint64_t *pos = (மாறிலி packed_uint64_t *)buf;
	மாறிலி packed_uint64_t *end = pos + (len / 8);
	மாறிலி unsigned எழுத்து *pos2;
	uint64_t h = seed ^ (len * m);
	uint64_t v;

#ifndef HB_OPTIMIZE_SIZE
	எனில் (((uintptr_t) pos & 7) == 0)
	{
	  போது (pos != end)
	  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
	    v  = * (மாறிலி uint64_t *) (pos++);
#pragma GCC diagnostic எடு
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}
	இல்லையெனில்
#endif
	{
	  போது (pos != end)
	  {
	    v  = pos++->v;
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}

	pos2 = (மாறிலி unsigned எழுத்து*)pos;
	v = 0;

	தேர்வு (len & 7) {
	நிலை 7: v ^= (uint64_t)pos2[6] << 48; HB_FALLTHROUGH;
	நிலை 6: v ^= (uint64_t)pos2[5] << 40; HB_FALLTHROUGH;
	நிலை 5: v ^= (uint64_t)pos2[4] << 32; HB_FALLTHROUGH;
	நிலை 4: v ^= (uint64_t)pos2[3] << 24; HB_FALLTHROUGH;
	நிலை 3: v ^= (uint64_t)pos2[2] << 16; HB_FALLTHROUGH;
	நிலை 2: v ^= (uint64_t)pos2[1] <<  8; HB_FALLTHROUGH;
	நிலை 1: v ^= (uint64_t)pos2[0];
		h ^= fasthash_mix(v);
		h *= m;
	}

	திரும்பு fasthash_mix(h);
}

நிலையான உள்வரி uint32_t fasthash32(மாறிலி வெறுமை *buf, size_t len, uint32_t seed)
{
	// the following trick converts the 64-bit hashcode to Fermat
	// residue, which shall retain information from both the higher
	// and lower parts of hashcode.
        uint64_t h = fasthash64(buf, len, seed);
	திரும்பு h - (h >> 32);
}

கட்டமைப்பு
{
  தனிப்பட்ட:

  template <typename T> constexpr auto
  impl (மாறிலி T& v, hb_priority<2>) மாறிலி HB_RETURN (uint32_t, hb_deref (v).hash ())

  // Horrible: std:hash() of integers seems to be identity in gcc / clang?!
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  //
  // For performance characteristics see:
  // https://github.com/harfbuzz/harfbuzz/pull/4228#issuecomment-1565079537
  template <typename T,
	    hb_enable_if (std::is_integral<T>::value && அளவு (T) <= அளவு (uint32_t))> constexpr auto
  impl (மாறிலி T& v, hb_priority<1>) மாறிலி HB_RETURN (uint32_t, (uint32_t) v * 2654435761u /* Knuh's multiplicative hash */)
  template <typename T,
	    hb_enable_if (std::is_integral<T>::value && அளவு (T) > அளவு (uint32_t))> constexpr auto
  impl (மாறிலி T& v, hb_priority<1>) மாறிலி HB_RETURN (uint32_t, (uint32_t) (v ^ (v >> 32)) * 2654435761u /* Knuth's multiplicative hash */)

  template <typename T,
	    hb_enable_if (std::is_floating_point<T>::value)> constexpr auto
  impl (மாறிலி T& v, hb_priority<1>) மாறிலி HB_RETURN (uint32_t, fasthash32 (std::addressof (v), அளவு (T), 0xf437ffe6))

  template <typename T> constexpr auto
  impl (மாறிலி T& v, hb_priority<0>) மாறிலி HB_RETURN (uint32_t, std::hash<hb_decay<decltype (hb_deref (v))>>{} (hb_deref (v)))

  பொது:

  template <typename T> constexpr auto
  operator () (மாறிலி T& v) மாறிலி HB_RETURN (uint32_t, impl (v, hb_prioritize))
}
HB_FUNCOBJ (hb_hash);


கட்டமைப்பு
{
  தனிப்பட்ட:

  /* Pointer-to-member-function. */
  template <typename Appl, typename T, typename ...Ts> auto
  impl (Appl&& a, hb_priority<2>, T &&v, Ts&&... ds) மாறிலி HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v)).*std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  /* Pointer-to-member. */
  template <typename Appl, typename T> auto
  impl (Appl&& a, hb_priority<1>, T &&v) மாறிலி HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v))).*std::forward<Appl> (a))

  /* Operator(). */
  template <typename Appl, typename ...Ts> auto
  impl (Appl&& a, hb_priority<0>, Ts&&... ds) மாறிலி HB_AUTO_RETURN
  (hb_deref (std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  பொது:

  template <typename Appl, typename ...Ts> auto
  operator () (Appl&& a, Ts&&... ds) மாறிலி HB_AUTO_RETURN
  (
    impl (std::forward<Appl> (a),
	  hb_prioritize,
	  std::forward<Ts> (ds)...)
  )
}
HB_FUNCOBJ (hb_invoke);

template <unsigned Pos, typename Appl, typename V>
கட்டமைப்பு hb_partial_t
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
    திரும்பு hb_invoke (std::forward<Appl> (a),
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
    திரும்பு hb_invoke (std::forward<Appl> (a),
		      std::forward<T0> (d0),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }

  தனிப்பட்ட:
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
  decltype(auto) operator () (_T&& _v) மாறிலி \
  { திரும்பு hb_partial<Pos> (இது, std::forward<_T> (_v)); } \
  static_assert (உண்மை, "")
#இல்லையெனில்
/* https://github.com/harfbuzz/harfbuzz/issues/1724 */
#define HB_PARTIALIZE(Pos) \
  template <typename _T> \
  auto operator () (_T&& _v) மாறிலி HB_AUTO_RETURN \
  (hb_partial<Pos> (+இது, std::forward<_T> (_v))) \
  static_assert (உண்மை, "")
#endif


கட்டமைப்பு
{
  தனிப்பட்ட:

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<1>) மாறிலி HB_AUTO_RETURN
  (
    hb_deref (std::forward<Pred> (p)).has (std::forward<Val> (v))
  )

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<0>) மாறிலி HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Pred> (p),
	       std::forward<Val> (v))
  )

  பொது:

  template <typename Pred, typename Val> auto
  operator () (Pred&& p, Val &&v) மாறிலி HB_RETURN (தருக்க,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_has);

கட்டமைப்பு
{
  தனிப்பட்ட:

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<1>) மாறிலி HB_AUTO_RETURN
  (
    hb_has (std::forward<Pred> (p),
	    std::forward<Val> (v))
  )

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<0>) மாறிலி HB_AUTO_RETURN
  (
    std::forward<Pred> (p) == std::forward<Val> (v)
  )

  பொது:

  template <typename Pred, typename Val> auto
  operator () (Pred&& p, Val &&v) மாறிலி HB_RETURN (தருக்க,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_match);

கட்டமைப்பு
{
  தனிப்பட்ட:

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<2>) மாறிலி HB_AUTO_RETURN
  (
    hb_deref (std::forward<Proj> (f)).get (std::forward<Val> (v))
  )

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<1>) மாறிலி HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Proj> (f),
	       std::forward<Val> (v))
  )

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<0>) மாறிலி HB_AUTO_RETURN
  (
    std::forward<Proj> (f)[std::forward<Val> (v)]
  )

  பொது:

  template <typename Proj, typename Val> auto
  operator () (Proj&& f, Val &&v) மாறிலி HB_AUTO_RETURN
  (
    impl (std::forward<Proj> (f),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_get);

கட்டமைப்பு
{
  தனிப்பட்ட:

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<3>) மாறிலி HB_AUTO_RETURN
  (
    std::forward<T2> (v2).cmp (std::forward<T1> (v1)) == 0
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<2>) மாறிலி HB_AUTO_RETURN
  (
    std::forward<T1> (v1).cmp (std::forward<T2> (v2)) == 0
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<1>) மாறிலி HB_AUTO_RETURN
  (
    std::forward<T1> (v1) == std::forward<T2> (v2)
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<0>) மாறிலி HB_AUTO_RETURN
  (
    std::forward<T2> (v2) == std::forward<T1> (v1)
  )

  பொது:

  template <typename T1, typename T2> auto
  operator () (T1&& v1, T2 &&v2) மாறிலி HB_AUTO_RETURN
  (
    impl (std::forward<T1> (v1),
	  std::forward<T2> (v2),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_equal);

கட்டமைப்பு
{
  template <typename T> வெறுமை
  operator () (T& a, T& b) மாறிலி
  {
    using std::swap; // allow ADL
    swap (a, b);
  }
}
HB_FUNCOBJ (hb_swap);


template <typename T1, typename T2>
கட்டமைப்பு hb_pair_t
{
  வகைவரை T1 first_t;
  வகைவரை T2 second_t;
  வகைவரை hb_pair_t<T1, T2> pair_t;

  template <typename U1 = T1, typename U2 = T2,
	    hb_enable_if (std::is_default_constructible<U1>::value &&
			  std::is_default_constructible<U2>::value)>
  hb_pair_t () : first (), second () {}
  hb_pair_t (T1 a, T2 b) : first (std::forward<T1> (a)), second (std::forward<T2> (b)) {}

  template <typename Q1, typename Q2,
	    hb_enable_if (hb_is_convertible (T1, Q1) &&
			  hb_is_convertible (T2, Q2))>
  operator hb_pair_t<Q1, Q2> () { திரும்பு hb_pair_t<Q1, Q2> (first, second); }

  hb_pair_t<T1, T2> reverse () மாறிலி
  { திரும்பு hb_pair_t<T1, T2> (second, first); }

  தருக்க operator == (மாறிலி pair_t& o) மாறிலி { திரும்பு first == o.first && second == o.second; }
  தருக்க operator != (மாறிலி pair_t& o) மாறிலி { திரும்பு !(*இது == o); }
  தருக்க operator < (மாறிலி pair_t& o) மாறிலி { திரும்பு first < o.first || (first == o.first && second < o.second); }
  தருக்க operator >= (மாறிலி pair_t& o) மாறிலி { திரும்பு !(*இது < o); }
  தருக்க operator > (மாறிலி pair_t& o) மாறிலி { திரும்பு first > o.first || (first == o.first && second > o.second); }
  தருக்க operator <= (மாறிலி pair_t& o) மாறிலி { திரும்பு !(*இது > o); }

  நிலையான முழு cmp (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb)
  {
    pair_t *a = (pair_t *) pa;
    pair_t *b = (pair_t *) pb;

    எனில் (a->first < b->first) திரும்பு -1;
    எனில் (a->first > b->first) திரும்பு +1;
    எனில் (a->second < b->second) திரும்பு -1;
    எனில் (a->second > b->second) திரும்பு +1;
    திரும்பு 0;
  }

  friend வெறுமை swap (hb_pair_t& a, hb_pair_t& b) noexcept
  {
    hb_swap (a.first, b.first);
    hb_swap (a.second, b.second);
  }


  T1 first;
  T2 second;
};
template <typename T1, typename T2> நிலையான உள்வரி hb_pair_t<T1, T2>
hb_pair (T1&& a, T2&& b) { திரும்பு hb_pair_t<T1, T2> (a, b); }

வகைவரை hb_pair_t<hb_codepoint_t, hb_codepoint_t> hb_codepoint_pair_t;

கட்டமைப்பு
{
  template <typename Pair> constexpr typename Pair::first_t
  operator () (மாறிலி Pair& pair) மாறிலி { திரும்பு pair.first; }
}
HB_FUNCOBJ (hb_first);

கட்டமைப்பு
{
  template <typename Pair> constexpr typename Pair::second_t
  operator () (மாறிலி Pair& pair) மாறிலி { திரும்பு pair.second; }
}
HB_FUNCOBJ (hb_second);

/* Note.  In min/max impl, we can use hb_type_identity<T> for second argument.
 * However, that would silently convert between different-signedness integers.
 * Instead we accept two different types, such that compiler can err if
 * comparing integers of different signedness. */
கட்டமைப்பு
{
  template <typename T, typename T2> constexpr auto
  operator () (T&& a, T2&& b) மாறிலி HB_AUTO_RETURN
  (a <= b ? a : b)
}
HB_FUNCOBJ (hb_min);
கட்டமைப்பு
{
  template <typename T, typename T2> constexpr auto
  operator () (T&& a, T2&& b) மாறிலி HB_AUTO_RETURN
  (a >= b ? a : b)
}
HB_FUNCOBJ (hb_max);
கட்டமைப்பு
{
  template <typename T, typename T2, typename T3> constexpr auto
  operator () (T&& x, T2&& min, T3&& max) மாறிலி HB_AUTO_RETURN
  (hb_min (hb_max (std::forward<T> (x), std::forward<T2> (min)), std::forward<T3> (max)))
}
HB_FUNCOBJ (hb_clamp);

/* Signed saturating arithmetic. */
template <typename T>
நிலையான HB_ALWAYS_INLINE T
hb_saturate_add (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#எனில் hb_has_builtin(__builtin_add_overflow)
  T result;
  எனில் (likely (!__builtin_add_overflow (a, b, &result)))
    திரும்பு result;
#இல்லையெனில்
  எனில் (b > 0)
  {
    எனில் (likely (a <= hb_int_max (T) - b))
      திரும்பு a + b;
  }
  இல்லையெனில் எனில் (likely (a >= hb_int_min (T) - b))
    திரும்பு a + b;
#endif

  திரும்பு a < 0 ? hb_int_min (T) : hb_int_max (T);
}

template <typename T>
நிலையான HB_ALWAYS_INLINE T
hb_saturate_sub (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#எனில் hb_has_builtin(__builtin_sub_overflow)
  T result;
  எனில் (likely (!__builtin_sub_overflow (a, b, &result)))
    திரும்பு result;
#இல்லையெனில்
  எனில் (b > 0)
  {
    எனில் (likely (a >= hb_int_min (T) + b))
      திரும்பு a - b;
  }
  இல்லையெனில் எனில் (likely (a <= hb_int_max (T) + b))
    திரும்பு a - b;
#endif

  திரும்பு b < 0 ? hb_int_max (T) : hb_int_min (T);
}

template <typename T>
நிலையான HB_ALWAYS_INLINE T
hb_saturate_neg (T a)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  திரும்பு hb_saturate_sub ((T) 0, a);
}

/* Convert a floating-point value to integer type T, saturating to T's
 * range instead of relying on the undefined behavior of an out-of-range
 * float-to-int conversion.  NaN saturates to the minimum of T. */
template <typename T>
நிலையான HB_ALWAYS_INLINE T
hb_clamp_to (இரட்டை v)
{
  திரும்பு (T) hb_clamp (v,
		       (இரட்டை) hb_int_min (T),
		       (இரட்டை) hb_int_max (T));
}

template <typename T>
நிலையான HB_ALWAYS_INLINE T
hb_clamp_to (தசம v)
{
  /* Widen explicitly: float cannot represent all integer bounds exactly,
   * so the clamp itself is done in double. */
  திரும்பு hb_clamp_to<T> ((இரட்டை) v);
}

template <typename T>
நிலையான HB_ALWAYS_INLINE T
hb_clamp_to (int64_t v)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  திரும்பு (T) hb_clamp (v,
		       (int64_t) hb_int_min (T),
		       (int64_t) hb_int_max (T));
}

/*
 * Bithacks.
 */

/* Return the number of 1 bits in a uint8_t; faster than hb_popcount() */
நிலையான உள்வரி unsigned
hb_popcount8 (uint8_t v)
{
  நிலையான மாறிலி uint8_t popcount4[16] = {
    0, 1, 1, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 2, 3, 3, 4
  };
  திரும்பு popcount4[v & 0xF] + popcount4[v >> 4];
}

/* Return the number of 1 bits in v. */
template <typename T>
நிலையான உள்வரி unsigned முழு
hb_popcount (T v)
{
#எனில் hb_has_builtin(__builtin_popcount)
  எனில் (அளவு (T) <= அளவு (unsigned முழு))
    திரும்பு __builtin_popcount (v);
#endif

#எனில் hb_has_builtin(__builtin_popcountl)
  எனில் (அளவு (T) <= அளவு (unsigned long))
    திரும்பு __builtin_popcountl (v);
#endif

#எனில் hb_has_builtin(__builtin_popcountll)
  எனில் (அளவு (T) <= அளவு (unsigned long long))
    திரும்பு __builtin_popcountll (v);
#endif

  எனில் (அளவு (T) <= 4)
  {
    /* "HACKMEM 169" */
    uint32_t y;
    y = (v >> 1) &033333333333;
    y = v - y - ((y >>1) & 033333333333);
    திரும்பு (((y + (y >> 3)) & 030707070707) % 077);
  }

  எனில் (அளவு (T) == 8)
  {
    uint64_t y = (uint64_t) v;
    y -= ((y >> 1) & 0x5555555555555555ull);
    y = (y & 0x3333333333333333ull) + (y >> 2 & 0x3333333333333333ull);
    திரும்பு ((y + (y >> 4)) & 0xf0f0f0f0f0f0f0full) * 0x101010101010101ull >> 56;
  }

  எனில் (அளவு (T) == 16)
  {
    unsigned முழு shift = 64;
    திரும்பு hb_popcount<uint64_t> ((uint64_t) v) + hb_popcount ((uint64_t) (v >> shift));
  }

  ப்ரதிஜ்ஞா (0);
  திரும்பு 0; /* Shut up stupid compiler. */
}

/* Returns the number of bits needed to store number */
template <typename T>
நிலையான உள்வரி unsigned முழு
hb_bit_storage (T v)
{
  எனில் (unlikely (!v)) திரும்பு 0;

#எனில் hb_has_builtin(__builtin_clz)
  எனில் (அளவு (T) <= அளவு (unsigned முழு))
    திரும்பு அளவு (unsigned முழு) * 8 - __builtin_clz (v);
#endif

#எனில் hb_has_builtin(__builtin_clzl)
  எனில் (அளவு (T) <= அளவு (unsigned long))
    திரும்பு அளவு (unsigned long) * 8 - __builtin_clzl (v);
#endif

#எனில் hb_has_builtin(__builtin_clzll)
  எனில் (அளவு (T) <= அளவு (unsigned long long))
    திரும்பு அளவு (unsigned long long) * 8 - __builtin_clzll (v);
#endif

#எனில் (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  எனில் (அளவு (T) <= அளவு (unsigned முழு))
  {
    unsigned long where;
    _BitScanReverse (&where, v);
    திரும்பு 1 + where;
  }
# எனில் defined(_WIN64)
  எனில் (அளவு (T) <= 8)
  {
    unsigned long where;
    _BitScanReverse64 (&where, v);
    திரும்பு 1 + where;
  }
# endif
#endif

  எனில் (அளவு (T) <= 4)
  {
    /* "bithacks" */
    மாறிலி unsigned முழு b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    மாறிலி unsigned முழு S[] = {1, 2, 4, 8, 16};
    unsigned முழு r = 0;
    க்கு (முழு i = 4; i >= 0; i--)
      எனில் (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    திரும்பு r + 1;
  }
  எனில் (அளவு (T) <= 8)
  {
    /* "bithacks" */
    மாறிலி uint64_t b[] = {0x2ULL, 0xCULL, 0xF0ULL, 0xFF00ULL, 0xFFFF0000ULL, 0xFFFFFFFF00000000ULL};
    மாறிலி unsigned முழு S[] = {1, 2, 4, 8, 16, 32};
    unsigned முழு r = 0;
    க்கு (முழு i = 5; i >= 0; i--)
      எனில் (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    திரும்பு r + 1;
  }
  எனில் (அளவு (T) == 16)
  {
    unsigned முழு shift = 64;
    திரும்பு (v >> shift) ? hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift :
			  hb_bit_storage<uint64_t> ((uint64_t) v);
  }

  ப்ரதிஜ்ஞா (0);
  திரும்பு 0; /* Shut up stupid compiler. */
}

/* Returns the number of zero bits in the least significant side of v */
template <typename T>
நிலையான உள்வரி unsigned முழு
hb_ctz (T v)
{
  எனில் (unlikely (!v)) திரும்பு 8 * அளவு (T);

#எனில் hb_has_builtin(__builtin_ctz)
  எனில் (அளவு (T) <= அளவு (unsigned முழு))
    திரும்பு __builtin_ctz (v);
#endif

#எனில் hb_has_builtin(__builtin_ctzl)
  எனில் (அளவு (T) <= அளவு (unsigned long))
    திரும்பு __builtin_ctzl (v);
#endif

#எனில் hb_has_builtin(__builtin_ctzll)
  எனில் (அளவு (T) <= அளவு (unsigned long long))
    திரும்பு __builtin_ctzll (v);
#endif

#எனில் (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  எனில் (அளவு (T) <= அளவு (unsigned முழு))
  {
    unsigned long where;
    _BitScanForward (&where, v);
    திரும்பு where;
  }
# எனில் defined(_WIN64)
  எனில் (அளவு (T) <= 8)
  {
    unsigned long where;
    _BitScanForward64 (&where, v);
    திரும்பு where;
  }
# endif
#endif

  எனில் (அளவு (T) <= 4)
  {
    /* "bithacks" */
    unsigned முழு c = 32;
    v &= - (int32_t) v;
    எனில் (v) c--;
    எனில் (v & 0x0000FFFF) c -= 16;
    எனில் (v & 0x00FF00FF) c -= 8;
    எனில் (v & 0x0F0F0F0F) c -= 4;
    எனில் (v & 0x33333333) c -= 2;
    எனில் (v & 0x55555555) c -= 1;
    திரும்பு c;
  }
  எனில் (அளவு (T) <= 8)
  {
    /* "bithacks" */
    unsigned முழு c = 64;
    v &= - (int64_t) (v);
    எனில் (v) c--;
    எனில் (v & 0x00000000FFFFFFFFULL) c -= 32;
    எனில் (v & 0x0000FFFF0000FFFFULL) c -= 16;
    எனில் (v & 0x00FF00FF00FF00FFULL) c -= 8;
    எனில் (v & 0x0F0F0F0F0F0F0F0FULL) c -= 4;
    எனில் (v & 0x3333333333333333ULL) c -= 2;
    எனில் (v & 0x5555555555555555ULL) c -= 1;
    திரும்பு c;
  }
  எனில் (அளவு (T) == 16)
  {
    unsigned முழு shift = 64;
    திரும்பு (uint64_t) v ? hb_bit_storage<uint64_t> ((uint64_t) v) :
			  hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift;
  }

  ப்ரதிஜ்ஞா (0);
  திரும்பு 0; /* Shut up stupid compiler. */
}


/*
 * Tiny stuff.
 */

/* ASCII tag/character handling */
நிலையான உள்வரி தருக்க ISALPHA (unsigned எழுத்து c)
{ திரும்பு (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
நிலையான உள்வரி தருக்க ISALNUM (unsigned எழுத்து c)
{ திரும்பு (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }
நிலையான உள்வரி தருக்க ISSPACE (unsigned எழுத்து c)
{ திரும்பு c == ' ' || c =='\f'|| c =='\n'|| c =='\r'|| c =='\t'|| c =='\v'; }
நிலையான உள்வரி unsigned எழுத்து TOUPPER (unsigned எழுத்து c)
{ திரும்பு (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c; }
நிலையான உள்வரி unsigned எழுத்து TOLOWER (unsigned எழுத்து c)
{ திரும்பு (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }
நிலையான உள்வரி தருக்க ISHEX (unsigned எழுத்து c)
{ திரும்பு (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
நிலையான உள்வரி unsigned எழுத்து TOHEX (uint8_t c)
{ திரும்பு (c & 0xF) <= 9 ? (c & 0xF) + '0' : (c & 0xF) + 'a' - 10; }
நிலையான உள்வரி uint8_t FROMHEX (unsigned எழுத்து c)
{ திரும்பு (c >= '0' && c <= '9') ? c - '0' : TOLOWER (c) - 'a' + 10; }

நிலையான உள்வரி unsigned முழு DIV_CEIL (மாறிலி unsigned முழு a, unsigned முழு b)
{ திரும்பு (a + (b - 1)) / b; }


#undef  ARRAY_LENGTH
template <typename Type, unsigned முழு n>
நிலையான உள்வரி unsigned முழு ARRAY_LENGTH (மாறிலி Type (&)[n]) { திரும்பு n; }
/* A const version, but does not detect erratically being called on pointers. */
#define ARRAY_LENGTH_CONST(__array) ((signed முழு) (அளவு (__array) / அளவு (__array[0])))


நிலையான உள்வரி வெறுமை *
hb_memcpy (வெறுமை *__restrict dst, மாறிலி வெறுமை *__restrict src, size_t len)
{
  /* It's illegal to pass 0 as size to memcpy. */
  எனில் (unlikely (!len)) திரும்பு dst;
  திரும்பு memcpy (dst, src, len);
}

நிலையான உள்வரி முழு
hb_memcmp (மாறிலி வெறுமை *a, மாறிலி வெறுமை *b, unsigned முழு len)
{
  /* It's illegal to pass NULL to memcmp(), even if len is zero.
   * So, wrap it.
   * https://sourceware.org/bugzilla/show_bug.cgi?id=23878 */
  எனில் (unlikely (!len)) திரும்பு 0;
  திரும்பு memcmp (a, b, len);
}

நிலையான உள்வரி வெறுமை *
hb_memset (வெறுமை *s, முழு c, unsigned முழு n)
{
  /* It's illegal to pass NULL to memset(), even if n is zero. */
  எனில் (unlikely (!n)) திரும்பு s;
  திரும்பு memset (s, c, n);
}

நிலையான உள்வரி unsigned முழு
hb_ceil_to_4 (unsigned முழு v)
{
  திரும்பு ((v - 1) | 3) + 1;
}

template <typename T> நிலையான உள்வரி தருக்க
hb_in_range (T u, T lo, T hi)
{
  static_assert (!std::is_signed<T>::value, "");

  /* The casts below are important as if T is smaller than int,
   * the subtract results will become a signed int! */
  திரும்பு (T)(u - lo) <= (T)(hi - lo);
}
template <typename T> நிலையான உள்வரி தருக்க
hb_in_ranges (T u, T lo1, T hi1)
{
  திரும்பு hb_in_range (u, lo1, hi1);
}
template <typename T, typename ...Ts> நிலையான உள்வரி தருக்க
hb_in_ranges (T u, T lo1, T hi1, Ts... ds)
{
  திரும்பு hb_in_range<T> (u, lo1, hi1) || hb_in_ranges<T> (u, ds...);
}


/*
 * Overflow checking.
 */

நிலையான உள்வரி தருக்க
hb_unsigned_mul_overflows (unsigned முழு மொத்தம், unsigned முழு size, unsigned *result = nullptr)
{
#எனில் hb_has_builtin(__builtin_mul_overflow)
  unsigned அடுக்கு_result;
  எனில் (!result)
    result = &அடுக்கு_result;
  திரும்பு __builtin_mul_overflow (மொத்தம், size, result);
#endif

  எனில் (result)
    *result = மொத்தம் * size;
  திரும்பு (size > 0) && (மொத்தம் >= ((unsigned முழு) -1) / size);
}

நிலையான உள்வரி தருக்க
hb_unsigned_add_overflows (unsigned முழு a, unsigned முழு b, unsigned *result = nullptr)
{
#எனில் hb_has_builtin(__builtin_add_overflow)
  unsigned அடுக்கு_result;
  எனில் (!result)
    result = &அடுக்கு_result;
  திரும்பு __builtin_add_overflow (a, b, result);
#endif

  எனில் (result)
    *result = a + b;
  திரும்பு b > (unsigned முழு) -1 - a;
}

/* Saturating arithmetic on size_t.  On platforms where size_t is wider than
 * unsigned int (i.e. 64-bit), the inputs to get_size()-style computations
 * (counts and static_sizes which are at most 32-bit) cannot overflow size_t,
 * so these reduce to plain arithmetic.  On 32-bit platforms (size_t is
 * unsigned int), they saturate to SIZE_MAX so sanitize/serialize callers
 * naturally reject the resulting size as out-of-range. */

நிலையான உள்வரி size_t
hb_unsigned_mul_saturate (size_t a, size_t b)
{
  எனில் (அளவு (size_t) > அளவு (unsigned முழு))
    திரும்பு a * b;
#எனில் hb_has_builtin(__builtin_mul_overflow)
  size_t result;
  எனில் (__builtin_mul_overflow (a, b, &result))
    திரும்பு (size_t) -1;
  திரும்பு result;
#இல்லையெனில்
  எனில் (b > 0 && a > ((size_t) -1) / b) திரும்பு (size_t) -1;
  திரும்பு a * b;
#endif
}

நிலையான உள்வரி size_t
hb_unsigned_add_saturate (size_t a, size_t b)
{
  எனில் (அளவு (size_t) > அளவு (unsigned முழு))
    திரும்பு a + b;
#எனில் hb_has_builtin(__builtin_add_overflow)
  size_t result;
  எனில் (__builtin_add_overflow (a, b, &result))
    திரும்பு (size_t) -1;
  திரும்பு result;
#இல்லையெனில்
  எனில் (b > ((size_t) -1) - a) திரும்பு (size_t) -1;
  திரும்பு a + b;
#endif
}

/* Variadic forms: fold left across all arguments. */
template <typename ...Ts>
நிலையான உள்வரி size_t
hb_unsigned_mul_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ திரும்பு hb_unsigned_mul_saturate (hb_unsigned_mul_saturate (a, b), c, rest...); }

template <typename ...Ts>
நிலையான உள்வரி size_t
hb_unsigned_add_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ திரும்பு hb_unsigned_add_saturate (hb_unsigned_add_saturate (a, b), c, rest...); }

/* Saturating mul-add: a * b + c.  Covers the dominant get_size() pattern
 * `count * static_size + min_size`. */
நிலையான உள்வரி size_t
hb_unsigned_mul_add_saturate (size_t a, size_t b, size_t c)
{ திரும்பு hb_unsigned_add_saturate (hb_unsigned_mul_saturate (a, b), c); }


/*
 * Sort and search.
 */

template <typename K, typename V, typename ...Ts>
நிலையான முழு
_hb_cmp_method (மாறிலி வெறுமை *pkey, மாறிலி வெறுமை *pval, Ts... ds)
{
  மாறிலி K& key = * (மாறிலி K*) pkey;
  மாறிலி V& val = * (மாறிலி V*) pval;

  திரும்பு val.cmp (key, ds...);
}

template <typename K, typename V>
நிலையான முழு
_hb_cmp_operator (மாறிலி வெறுமை *pkey, மாறிலி வெறுமை *pval)
{
  மாறிலி K& key = * (மாறிலி K*) pkey;
  மாறிலி V& val = * (மாறிலி V*) pval;

  எனில் (key < val) திரும்பு -1;
  எனில் (key > val) திரும்பு  1;
  திரும்பு 0;
}

template <typename V, typename K, typename ...Ts>
HB_HOT
நிலையான உள்வரி தருக்க
hb_bsearch_impl (unsigned *pos, /* Out */
		 மாறிலி K& key,
		 V* base, size_t nmemb, size_t stride,
		 முழு (*compar)(மாறிலி வெறுமை *சாவி_3, மாறிலி வெறுமை *_item, Ts... _ds),
		 Ts... ds)
{
  /* This is our *only* bsearch implementation. */

  முழு min = 0, max = (முழு) nmemb - 1;
  போது (min <= max)
  {
    முழு mid = ((unsigned முழு) min + (unsigned முழு) max) / 2;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    V* p = (V*) (((மாறிலி எழுத்து *) base) + (mid * stride));
#pragma GCC diagnostic எடு
    முழு c = compar ((மாறிலி வெறுமை *) std::addressof (key), (மாறிலி வெறுமை *) p, ds...);
    எனில் (c < 0)
      max = mid - 1;
    இல்லையெனில் எனில் (c > 0)
      min = mid + 1;
    இல்லையெனில்
    {
      *pos = mid;
      திரும்பு உண்மை;
    }
  }
  *pos = min;
  திரும்பு பொய்;
}

template <typename V, typename K>
நிலையான உள்வரி V*
hb_bsearch (மாறிலி K& key, V* base,
	    size_t nmemb, size_t stride = அளவு (V),
	    முழு (*compar)(மாறிலி வெறுமை *சாவி_3, மாறிலி வெறுமை *_item) = _hb_cmp_method<K, V>)
{
  unsigned pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  திரும்பு hb_bsearch_impl (&pos, key, base, nmemb, stride, compar) ?
	 (V*) (((மாறிலி எழுத்து *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic எடு
}
template <typename V, typename K, typename ...Ts>
நிலையான உள்வரி V*
hb_bsearch (மாறிலி K& key, V* base,
	    size_t nmemb, size_t stride,
	    முழு (*compar)(மாறிலி வெறுமை *சாவி_3, மாறிலி வெறுமை *_item, Ts... _ds),
	    Ts... ds)
{
  unsigned pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  திரும்பு hb_bsearch_impl (&pos, key, base, nmemb, stride, compar, ds...) ?
	 (V*) (((மாறிலி எழுத்து *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic எடு
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
நிலையான உள்வரி வெறுமை
hb_qsort_loop (T *base, size_t nel, Compar compar)
{
  போது (nel > 24)
  {
    T *last = base + nel - 1;
    T *mid = base + nel / 2;

    /* Median-of-three pivot, parked at last-1. */
    எனில் (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    எனில் (compar (*mid, *last) > 0)
    {
      hb_swap (*mid, *last);
      எனில் (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    }
    hb_swap (*mid, *(last - 1));
    T &pivot = *(last - 1);

    /* Two-way Hoare partition.  Inner loops are unguarded:
     * median-of-three left *base <= pivot and *last >= pivot,
     * which act as sentinels. */
    T *i = base, *j = last - 1;
    போது (உண்மை)
    {
      போது (compar (*++i, pivot) < 0) {}
      போது (compar (*--j, pivot) > 0) {}
      எனில் (i >= j) நிறுத்து;
      hb_swap (*i, *j);
    }
    hb_swap (*i, *(last - 1));

    /* Recurse on smaller side, loop on larger — bounds stack
     * depth at O(log n). */
    size_t left  = (size_t) (i - base);
    size_t வலது = nel - left - 1;
    எனில் (left < வலது)
    {
      hb_qsort_loop (base, left, compar);
      base = i + 1;
      nel  = வலது;
    }
    இல்லையெனில்
    {
      hb_qsort_loop (i + 1, வலது, compar);
      nel  = left;
    }
  }
}

template <typename T, typename Compar>
நிலையான உள்வரி வெறுமை
hb_qsort_inline (T *base, size_t nel, Compar compar)
{
  hb_qsort_loop (base, nel, compar);

  /* Single final insertion sort over the whole array.  After
   * the partitioning loop, every element is within the threshold
   * of its sorted position, so this pass is O(n * threshold). */
  T *end = base + nel;
  க்கு (T *pi = base + 1; pi < end; pi++)
    க்கு (T *pj = pi; pj > base && compar (pj[-1], pj[0]) > 0; pj--)
      hb_swap (pj[-1], pj[0]);
}

நிலையான உள்வரி வெறுமை
hb_qsort (வெறுமை *base, size_t nel, size_t width,
	  முழு (*compar)(மாறிலி வெறுமை *_a, மாறிலி வெறுமை *_b))
{
  qsort (base, nel, width, compar);
}




template <typename T, typename T2, typename T3 = முழு> நிலையான உள்வரி வெறுமை
hb_stable_sort (T *array, unsigned முழு len, முழு(*compar)(மாறிலி T2 *, மாறிலி T2 *), T3 *array2 = nullptr)
{
  static_assert (hb_is_trivially_copy_assignable (T), "");
  static_assert (hb_is_trivially_copy_assignable (T3), "");

  க்கு (unsigned முழு i = 1; i < len; i++)
  {
    unsigned முழு j = i;
    போது (j && compar (&array[j - 1], &array[i]) > 0)
      j--;
    எனில் (i == j)
      தொடர்;
    /* Move item i to occupy place for item j, shift what's in between. */
    {
      T t = array[i];
      memmove (&array[j + 1], &array[j], (i - j) * அளவு (T));
      array[j] = t;
    }
    எனில் (array2)
    {
      T3 t = array2[i];
      memmove (&array2[j + 1], &array2[j], (i - j) * அளவு (T3));
      array2[j] = t;
    }
  }
}

நிலையான உள்வரி hb_bool_t
hb_codepoint_parse (மாறிலி எழுத்து *s, unsigned முழு len, முழு base, hb_codepoint_t *out)
{
  unsigned முழு v;
  மாறிலி எழுத்து *p = s;
  மாறிலி எழுத்து *end = p + len;
  எனில் (unlikely (!hb_parse_uint (&p, end, &v, உண்மை/* whole buffer */, base)))
    திரும்பு பொய்;

  *out = v;
  திரும்பு உண்மை;
}


/* Operators. */

கட்டமைப்பு
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (மாறிலி T &a, மாறிலி T &b) மாறிலி HB_AUTO_RETURN (a & b)
}
HB_FUNCOBJ (hb_bitwise_and);
கட்டமைப்பு
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (மாறிலி T &a, மாறிலி T &b) மாறிலி HB_AUTO_RETURN (a | b)
}
HB_FUNCOBJ (hb_bitwise_or);
கட்டமைப்பு
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (மாறிலி T &a, மாறிலி T &b) மாறிலி HB_AUTO_RETURN (a ^ b)
}
HB_FUNCOBJ (hb_bitwise_xor);
கட்டமைப்பு
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (மாறிலி T &a, மாறிலி T &b) மாறிலி HB_AUTO_RETURN (~a & b)
}
HB_FUNCOBJ (hb_bitwise_lt);
கட்டமைப்பு
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (மாறிலி T &a, மாறிலி T &b) மாறிலி HB_AUTO_RETURN (a & ~b)
}
HB_FUNCOBJ (hb_bitwise_gt); // aka sub
கட்டமைப்பு
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (மாறிலி T &a, மாறிலி T &b) மாறிலி HB_AUTO_RETURN (~a | b)
}
HB_FUNCOBJ (hb_bitwise_le);
கட்டமைப்பு
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (மாறிலி T &a, மாறிலி T &b) மாறிலி HB_AUTO_RETURN (a | ~b)
}
HB_FUNCOBJ (hb_bitwise_ge);
கட்டமைப்பு
{
  template <typename T> constexpr auto
  operator () (மாறிலி T &a) மாறிலி HB_AUTO_RETURN (~a)
}
HB_FUNCOBJ (hb_bitwise_neg);

கட்டமைப்பு
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (மாறிலி T &a, மாறிலி T2 &b) மாறிலி HB_AUTO_RETURN (a + b)
}
HB_FUNCOBJ (hb_add);
கட்டமைப்பு
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (மாறிலி T &a, மாறிலி T2 &b) மாறிலி HB_AUTO_RETURN (a - b)
}
HB_FUNCOBJ (hb_sub);
கட்டமைப்பு
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (மாறிலி T &a, மாறிலி T2 &b) மாறிலி HB_AUTO_RETURN (b - a)
}
HB_FUNCOBJ (hb_rsub);
கட்டமைப்பு
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (மாறிலி T &a, மாறிலி T2 &b) மாறிலி HB_AUTO_RETURN (a * b)
}
HB_FUNCOBJ (hb_mul);
கட்டமைப்பு
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (மாறிலி T &a, மாறிலி T2 &b) மாறிலி HB_AUTO_RETURN (a / b)
}
HB_FUNCOBJ (hb_div);
கட்டமைப்பு
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (மாறிலி T &a, மாறிலி T2 &b) மாறிலி HB_AUTO_RETURN (a % b)
}
HB_FUNCOBJ (hb_mod);
கட்டமைப்பு
{
  template <typename T> constexpr auto
  operator () (மாறிலி T &a) மாறிலி HB_AUTO_RETURN (+a)
}
HB_FUNCOBJ (hb_pos);
கட்டமைப்பு
{
  template <typename T> constexpr auto
  operator () (மாறிலி T &a) மாறிலி HB_AUTO_RETURN (-a)
}
HB_FUNCOBJ (hb_neg);
கட்டமைப்பு
{
  template <typename T> constexpr auto
  operator () (T &a) மாறிலி HB_AUTO_RETURN (++a)
}
HB_FUNCOBJ (hb_inc);
கட்டமைப்பு
{
  template <typename T> constexpr auto
  operator () (T &a) மாறிலி HB_AUTO_RETURN (--a)
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
இரட்டை solve_itp (func_t f,
		  இரட்டை a, இரட்டை b,
		  இரட்டை epsilon,
		  இரட்டை min_y, இரட்டை max_y,
		  இரட்டை &ya, இரட்டை &yb, இரட்டை &y)
{
  // Guard against degenerate interval
  எனில் (b - a <= 0.0)
  {
    y = ya;
    திரும்பு a;
  }

  unsigned n1_2 = (unsigned) (hb_max (ceil (log2 ((b - a) / epsilon)) - 1.0, 0.0));
  மாறிலி unsigned n0 = 1; // Hardwired
  மாறிலி இரட்டை k1 = 0.2 / (b - a); // Hardwired.
  unsigned nmax = n0 + n1_2;
  இரட்டை scaled_epsilon = epsilon * இரட்டை (1llu << nmax);
  இரட்டை _2_epsilon = 2.0 * epsilon;
  போது (b - a > _2_epsilon)
  {
    இரட்டை x1_2 = 0.5 * (a + b);
    இரட்டை r = scaled_epsilon - 0.5 * (b - a);
    // Guard against yb == ya to prevent division by zero
    இரட்டை xf = (yb != ya) ? (yb * a - ya * b) / (yb - ya) : x1_2;
    இரட்டை sigma = x1_2 - xf;
    இரட்டை b_a = b - a;
    // This has k2 = 2 hardwired for efficiency.
    இரட்டை b_a_k2 = b_a * b_a;
    இரட்டை delta = k1 * b_a_k2;
    முழு sigma_sign = sigma >= 0 ? +1 : -1;
    இரட்டை xt = delta <= fabs (x1_2 - xf) ? xf + delta * sigma_sign : x1_2;
    இரட்டை xitp = fabs (xt - x1_2) <= r ? xt : x1_2 - r * sigma_sign;
    இரட்டை yitp = f (xitp);
    எனில் (yitp > max_y)
    {
      b = xitp;
      yb = yitp;
    }
    இல்லையெனில் எனில் (yitp < min_y)
    {
      a = xitp;
      ya = yitp;
    }
    இல்லையெனில்
    {
      y = yitp;
      திரும்பு xitp;
    }
    scaled_epsilon *= 0.5;
  }
  திரும்பு 0.5 * (a + b);
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
கட்டமைப்பு hb_scope_guard_t
{
  explicit hb_scope_guard_t (F &&f) : f (std::move (f)), active (உண்மை) {}
  hb_scope_guard_t (hb_scope_guard_t &&o) noexcept
    : f (std::move (o.f)), active (o.active) { o.active = பொய்; }
  hb_scope_guard_t (மாறிலி hb_scope_guard_t &) = delete;
  hb_scope_guard_t &operator= (மாறிலி hb_scope_guard_t &) = delete;
  hb_scope_guard_t &operator= (hb_scope_guard_t &&) = delete;
  ~hb_scope_guard_t () { எனில் (active) f (); }

  /* Release: dismiss the guard so the cleanup does NOT run.  Use
   * when transferring ownership out of the scope. */
  வெறுமை release () { active = பொய்; }

  தனிப்பட்ட:
  F f;
  தருக்க active;
};

template <typename F>
நிலையான உள்வரி hb_scope_guard_t<F> hb_make_scope_guard (F &&f)
{ திரும்பு hb_scope_guard_t<F> (std::forward<F> (f)); }

#define HB_SCOPE_GUARD_NAME_(வரி) hb_scope_guard_##line
#define HB_SCOPE_GUARD_NAME(வரி) HB_SCOPE_GUARD_NAME_(வரி)
#define HB_SCOPE_GUARD(stmt) \
  auto HB_SCOPE_GUARD_NAME(__LINE__) = \
    hb_make_scope_guard ([&]() { stmt; }); \
  (வெறுமை) HB_SCOPE_GUARD_NAME(__LINE__)


#endif /* HB_ALGS_HH */
