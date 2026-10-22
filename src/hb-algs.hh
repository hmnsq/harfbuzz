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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-meta.hh"
#અન્તર્ભાવ "hb-null.hh"
#અન્તર્ભાવ "hb-number.hh"

#અન્તર્ભાવ <algorithm>
#અન્તર્ભાવ <initializer_list>
#અન્તર્ભાવ <functional>
#અન્તર્ભાવ <new>

સ્થિર HB_ALWAYS_INLINE શૂન્ય *
hb_malloc2 (size_t nmemb, size_t size)
{
  જો (size && nmemb > SIZE_MAX / size) પાછા nullptr;
  પાછા hb_malloc (nmemb * size);
}
સ્થિર HB_ALWAYS_INLINE શૂન્ય *
hb_realloc2 (શૂન્ય *ptr, size_t nmemb, size_t size)
{
  જો (size && nmemb > SIZE_MAX / size) પાછા nullptr;
  પાછા hb_realloc (ptr, nmemb * size);
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
	બાહ્ય "C++" { \
	  સ્થિર અંતઃરેખ constexpr T operator | (T l, T r) { પાછા T ((unsigned) l | (unsigned) r); } \
	  સ્થિર અંતઃરેખ constexpr T operator & (T l, T r) { પાછા T ((unsigned) l & (unsigned) r); } \
	  સ્થિર અંતઃરેખ constexpr T operator ^ (T l, T r) { પાછા T ((unsigned) l ^ (unsigned) r); } \
	  સ્થિર અંતઃરેખ constexpr unsigned operator ~ (T r) { પાછા (~(unsigned) r); } \
	  સ્થિર અંતઃરેખ T& operator |= (T &l, T r) { l = l | r; પાછા l; } \
	  સ્થિર અંતઃરેખ T& operator &= (T& l, T r) { l = l & r; પાછા l; } \
	  સ્થિર અંતઃરેખ T& operator ^= (T& l, T r) { l = l ^ r; પાછા l; } \
	} \
	static_assert (સાચું, "")

/* Useful for set-operations on small enums.
 * For example, for testing "x ∈ {x1, x2, x3}" use:
 * (FLAG_UNSAFE(x) & (FLAG(x1) | FLAG(x2) | FLAG(x3)))
 */
#define FLAG(x) (static_assert_પદાવલિ ((unsigned)(x) < 32) + (((uint32_t) 1U) << (unsigned)(x)))
#define FLAG_UNSAFE(x) ((unsigned)(x) < 32 ? (((uint32_t) 1U) << (unsigned)(x)) : 0)
#define FLAG_RANGE(x,y) (static_assert_પદાવલિ ((x) < (y)) + FLAG(y+1) - FLAG(x))
#define FLAG64(x) (static_assert_પદાવલિ ((unsigned)(x) < 64) + (((uint64_t) 1ULL) << (unsigned)(x)))
#define FLAG64_UNSAFE(x) ((unsigned)(x) < 64 ? (((uint64_t) 1ULL) << (unsigned)(x)) : 0)


/*
 * Fixed-endian integers / floats.
 */


/* Endian swap, used in Windows related backends */
સ્થિર અંતઃરેખ constexpr uint16_t hb_uint16_swap (uint16_t v)
{ પાછા (v >> 8) | (v << 8); }
સ્થિર અંતઃરેખ constexpr uint32_t hb_uint32_swap (uint32_t v)
{ પાછા (hb_uint16_swap (v) << 16) | hb_uint16_swap (v >> 16); }

template <typename Type>
સંરચના __attribute__((packed)) hb_packed_t
{
  hb_packed_t () = મૂળભૂત;
  constexpr hb_packed_t (Type V) : v (V) {}
  operator Type () અચલ { પાછા v; }
  hb_packed_t & operator = (Type V) { v = V; પાછા *આ; }

  ખાનગી:
  Type v;
};

#ifndef HB_FAST_અંક_ACCESS

#જો defined(__OPTIMIZE__) && \
    defined(__BYTE_ORDER) && \
    (__BYTE_ORDER == __BIG_ENDIAN || \
     (__BYTE_ORDER == __LITTLE_ENDIAN && \
      hb_has_builtin(__builtin_bswap16) && \
      hb_has_builtin(__builtin_bswap32) && \
      hb_has_builtin(__builtin_bswap64)))
#define HB_FAST_અંક_ACCESS 1
#નહીંતર
#define HB_FAST_અંક_ACCESS 0
#endif

// https://github.com/harfbuzz/harfbuzz/issues/5456
#જો defined(__GNUC__) && !defined(__clang__) && (__GNUC__ <= 12)
#undef HB_FAST_અંક_ACCESS
#define HB_FAST_અંક_ACCESS 0
#endif

#endif

template <તાર્કિક BE, typename Type, પૂર્ણાંક Bytes = કદ (Type)>
સંરચના HBInt;
template <તાર્કિક BE, typename Type>
સંરચના HBInt<BE, Type, 1>
{
  જાહેર:
  HBInt () = મૂળભૂત;
  constexpr HBInt (Type V) : v {uint8_t (V)} {}
  constexpr operator Type () અચલ { પાછા v; }
  ખાનગી: uint8_t v;
};
template <તાર્કિક BE, typename Type>
સંરચના HBInt<BE, Type, 2>
{
  જાહેર:
  HBInt () = મૂળભૂત;

  HBInt (Type V)
#જો HB_FAST_અંક_ACCESS
  {
    જો (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint16_t> *) v) = V;
    નહીંતર
      *((hb_packed_t<uint16_t> *) v) = __builtin_bswap16 (V);
  }
#નહીંતર
    : v {BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >>  8) & 0xFF)} {}
#endif

  constexpr operator Type () અચલ
  {
#જો HB_FAST_અંક_ACCESS
    પાછા (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint16_t) *((અચલ hb_packed_t<uint16_t> *) v)
    :
      __builtin_bswap16 ((uint16_t) *((અચલ hb_packed_t<uint16_t> *) v))
    ;
#નહીંતર
    પાછા (BE ? (v[0] <<  8) : (v[0]      ))
	 + (BE ? (v[1]      ) : (v[1] <<  8));
#endif
  }
  ખાનગી: uint8_t v[2];
};
template <તાર્કિક BE, typename Type>
સંરચના HBInt<BE, Type, 3>
{
  static_assert (!std::is_signed<Type>::value, "");
  જાહેર:
  HBInt () = મૂળભૂત;
  constexpr HBInt (Type V) : v {BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
				BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
				BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V      ) & 0xFF)} {}

  constexpr operator Type () અચલ { પાછા (BE ? (v[0] << 16) : (v[0]      ))
					  + (BE ? (v[1] <<  8) : (v[1] <<  8))
					  + (BE ? (v[2]      ) : (v[2] << 16)); }
  ખાનગી: uint8_t v[3];
};
template <તાર્કિક BE, typename Type>
સંરચના HBInt<BE, Type, 4>
{
  template <તાર્કિક, typename, પૂર્ણાંક>
  friend સંરચના HBFloat;

  જાહેર:
  HBInt () = મૂળભૂત;

  HBInt (Type V)
#જો HB_FAST_અંક_ACCESS
  {
    જો (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint32_t> *) v) = V;
    નહીંતર
      *((hb_packed_t<uint32_t> *) v) = __builtin_bswap32 (V);
  }
#નહીંતર
    : v {BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 24) & 0xFF)} {}
#endif

  constexpr operator Type () અચલ {
#જો HB_FAST_અંક_ACCESS
    પાછા (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint32_t) *((અચલ hb_packed_t<uint32_t> *) v)
    :
      __builtin_bswap32 ((uint32_t) *((અચલ hb_packed_t<uint32_t> *) v))
    ;
#નહીંતર
    પાછા (BE ? (uint32_t (v[0]) << 24) : (uint32_t (v[0])      ))
	 + (BE ? (uint32_t (v[1]) << 16) : (uint32_t (v[1]) <<  8))
	 + (BE ? (uint32_t (v[2]) <<  8) : (uint32_t (v[2]) << 16))
	 + (BE ? (uint32_t (v[3])      ) : (uint32_t (v[3]) << 24));
#endif
  }
  ખાનગી: uint8_t v[4];
};
template <તાર્કિક BE, typename Type>
સંરચના HBInt<BE, Type, 8>
{
  template <તાર્કિક, typename, પૂર્ણાંક>
  friend સંરચના HBFloat;

  જાહેર:
  HBInt () = મૂળભૂત;

  HBInt (Type V)
#જો HB_FAST_અંક_ACCESS
  {
    જો (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint64_t> *) v) = V;
    નહીંતર
      *((hb_packed_t<uint64_t> *) v) = __builtin_bswap64 (V);
  }
#નહીંતર
    : v {BE ? uint8_t ((V >> 56) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 48) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >> 40) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V >> 32) & 0xFF) : uint8_t ((V >> 24) & 0xFF),
	 BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V >> 32) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 40) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 48) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 56) & 0xFF)} {}
#endif

  constexpr operator Type () અચલ {
#જો HB_FAST_અંક_ACCESS
    પાછા (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint64_t) *((અચલ hb_packed_t<uint64_t> *) v)
    :
      __builtin_bswap64 ((uint64_t) *((અચલ hb_packed_t<uint64_t> *) v))
    ;
#નહીંતર
    પાછા (BE ? (uint64_t (v[0]) << 56) : (uint64_t (v[0])      ))
	 + (BE ? (uint64_t (v[1]) << 48) : (uint64_t (v[1]) <<  8))
	 + (BE ? (uint64_t (v[2]) << 40) : (uint64_t (v[2]) << 16))
	 + (BE ? (uint64_t (v[3]) << 32) : (uint64_t (v[3]) << 24))
	 + (BE ? (uint64_t (v[4]) << 24) : (uint64_t (v[4]) << 32))
	 + (BE ? (uint64_t (v[5]) << 16) : (uint64_t (v[5]) << 40))
	 + (BE ? (uint64_t (v[6]) <<  8) : (uint64_t (v[6]) << 48))
	 + (BE ? (uint64_t (v[7])      ) : (uint64_t (v[7]) << 56));
#endif
  }
  ખાનગી: uint8_t v[8];
};

/* Floats. */

template <તાર્કિક BE, typename Type, પૂર્ણાંક Bytes>
સંરચના HBFloat
{
  using IntType = typename std::conditional<Bytes == 4, uint32_t, uint64_t>::type;

  જાહેર:
  HBFloat () = મૂળભૂત;

  HBFloat (Type V)
  {
#જો HB_FAST_અંક_ACCESS
    {
	      જો (BE == (__BYTE_ORDER == __BIG_ENDIAN))
	      {
	        *((hb_packed_t<Type> *) v) = V;
	        પાછા;
	      }
	    }
#endif

    સંઘ {
      hb_packed_t<Type> f;
      hb_packed_t<IntType> i;
    } u = {{V}};

    અચલ HBInt<BE, IntType> I = (IntType) u.i;
    માટે (unsigned i = 0; i < Bytes; i++)
      v[i] = I.v[i];
  }

  /* c++14 constexpr */ operator Type () અચલ
  {
#જો HB_FAST_અંક_ACCESS
	    {
	      જો (BE == (__BYTE_ORDER == __BIG_ENDIAN))
		પાછા (Type) *((અચલ hb_packed_t<Type> *) v);
	    }
#endif

    HBInt<BE, IntType> I;
    માટે (unsigned i = 0; i < Bytes; i++)
      I.v[i] = v[i];

    સંઘ {
      hb_packed_t<IntType> i;
      hb_packed_t<Type> f;
    } u = {{I}};

    પાછા (Type) u.f;
  }
  ખાનગી: uint8_t v[Bytes];
};


/* We want our rounding towards +infinity. */
સ્થિર અંતઃરેખ દ્વિગુણ
_hb_roundf (દ્વિગુણ x) { પાછા floor (x + .5); }

સ્થિર અંતઃરેખ દશાંશ
_hb_roundf (દશાંશ x) { પાછા floor ((દ્વિગુણ) x + .5); }

#define roundf(x) _hb_roundf(x)

સ્થિર અંતઃરેખ શૂન્ય
hb_sincos (દશાંશ rotation, દશાંશ &s, દશાંશ &c)
{
#ifdef HAVE_SINCOSF
  sincosf (rotation, &s, &c);
#નહીંતર
  c = cosf (rotation);
  s = sinf (rotation);
#endif
}
સ્થિર અંતઃરેખ શૂન્ય
hb_sincos (દ્વિગુણ rotation, દ્વિગુણ &s, દ્વિગુણ &c)
{
#ifdef HAVE_SINCOS
  sincos (rotation, &s, &c);
#નહીંતર
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


સંરચના
{
  /* Note.  This is dangerous in that if it's passed an rvalue, it returns rvalue-reference. */
  template <typename T> constexpr auto
  operator () (T&& v) અચલ HB_AUTO_RETURN ( std::forward<T> (v) )
}
HB_FUNCOBJ (hb_identity);
સંરચના
{
  /* Like identity(), but only retains lvalue-references.  Rvalues are returned as rvalues. */
  template <typename T> constexpr T&
  operator () (T& v) અચલ { પાછા v; }

  template <typename T> constexpr hb_remove_reference<T>
  operator () (T&& v) અચલ { પાછા v; }
}
HB_FUNCOBJ (hb_lidentity);
સંરચના
{
  /* Like identity(), but always returns rvalue. */
  template <typename T> constexpr hb_remove_reference<T>
  operator () (T&& v) અચલ { પાછા v; }
}
HB_FUNCOBJ (hb_ridentity);

સંરચના
{
  template <typename T> constexpr તાર્કિક
  operator () (T&& v) અચલ { પાછા તાર્કિક (std::forward<T> (v)); }
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
			(શૂન્ય) ((h) ^= (h) >> 23),		\
			(શૂન્ય) ((h) *= 0x2127599bf4325c37ULL),	\
			(h) ^= (h) >> 47)

સ્થિર અંતઃરેખ uint64_t fasthash64(અચલ શૂન્ય *buf, size_t len, uint64_t seed)
{
	સંરચના __attribute__((packed)) packed_uint64_t { uint64_t v; };
	અચલ uint64_t    m = 0x880355f21e6d1965ULL;
	અચલ packed_uint64_t *pos = (અચલ packed_uint64_t *)buf;
	અચલ packed_uint64_t *end = pos + (len / 8);
	અચલ unsigned અક્ષર *pos2;
	uint64_t h = seed ^ (len * m);
	uint64_t v;

#ifndef HB_OPTIMIZE_SIZE
	જો (((uintptr_t) pos & 7) == 0)
	{
	  જ્યારે (pos != end)
	  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
	    v  = * (અચલ uint64_t *) (pos++);
#pragma GCC diagnostic કાઢો
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}
	નહીંતર
#endif
	{
	  જ્યારે (pos != end)
	  {
	    v  = pos++->v;
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}

	pos2 = (અચલ unsigned અક્ષર*)pos;
	v = 0;

	પસંદગી (len & 7) {
	સ્થિતિ 7: v ^= (uint64_t)pos2[6] << 48; HB_FALLTHROUGH;
	સ્થિતિ 6: v ^= (uint64_t)pos2[5] << 40; HB_FALLTHROUGH;
	સ્થિતિ 5: v ^= (uint64_t)pos2[4] << 32; HB_FALLTHROUGH;
	સ્થિતિ 4: v ^= (uint64_t)pos2[3] << 24; HB_FALLTHROUGH;
	સ્થિતિ 3: v ^= (uint64_t)pos2[2] << 16; HB_FALLTHROUGH;
	સ્થિતિ 2: v ^= (uint64_t)pos2[1] <<  8; HB_FALLTHROUGH;
	સ્થિતિ 1: v ^= (uint64_t)pos2[0];
		h ^= fasthash_mix(v);
		h *= m;
	}

	પાછા fasthash_mix(h);
}

સ્થિર અંતઃરેખ uint32_t fasthash32(અચલ શૂન્ય *buf, size_t len, uint32_t seed)
{
	// the following trick converts the 64-bit hashcode to Fermat
	// residue, which shall retain information from both the higher
	// and lower parts of hashcode.
        uint64_t h = fasthash64(buf, len, seed);
	પાછા h - (h >> 32);
}

સંરચના
{
  ખાનગી:

  template <typename T> constexpr auto
  impl (અચલ T& v, hb_priority<2>) અચલ HB_RETURN (uint32_t, hb_deref (v).hash ())

  // Horrible: std:hash() of integers seems to be identity in gcc / clang?!
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  //
  // For performance characteristics see:
  // https://github.com/harfbuzz/harfbuzz/pull/4228#issuecomment-1565079537
  template <typename T,
	    hb_enable_if (std::is_integral<T>::value && કદ (T) <= કદ (uint32_t))> constexpr auto
  impl (અચલ T& v, hb_priority<1>) અચલ HB_RETURN (uint32_t, (uint32_t) v * 2654435761u /* Knuh's multiplicative hash */)
  template <typename T,
	    hb_enable_if (std::is_integral<T>::value && કદ (T) > કદ (uint32_t))> constexpr auto
  impl (અચલ T& v, hb_priority<1>) અચલ HB_RETURN (uint32_t, (uint32_t) (v ^ (v >> 32)) * 2654435761u /* Knuth's multiplicative hash */)

  template <typename T,
	    hb_enable_if (std::is_floating_point<T>::value)> constexpr auto
  impl (અચલ T& v, hb_priority<1>) અચલ HB_RETURN (uint32_t, fasthash32 (std::addressof (v), કદ (T), 0xf437ffe6))

  template <typename T> constexpr auto
  impl (અચલ T& v, hb_priority<0>) અચલ HB_RETURN (uint32_t, std::hash<hb_decay<decltype (hb_deref (v))>>{} (hb_deref (v)))

  જાહેર:

  template <typename T> constexpr auto
  operator () (અચલ T& v) અચલ HB_RETURN (uint32_t, impl (v, hb_prioritize))
}
HB_FUNCOBJ (hb_hash);


સંરચના
{
  ખાનગી:

  /* Pointer-to-member-function. */
  template <typename Appl, typename T, typename ...Ts> auto
  impl (Appl&& a, hb_priority<2>, T &&v, Ts&&... ds) અચલ HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v)).*std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  /* Pointer-to-member. */
  template <typename Appl, typename T> auto
  impl (Appl&& a, hb_priority<1>, T &&v) અચલ HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v))).*std::forward<Appl> (a))

  /* Operator(). */
  template <typename Appl, typename ...Ts> auto
  impl (Appl&& a, hb_priority<0>, Ts&&... ds) અચલ HB_AUTO_RETURN
  (hb_deref (std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  જાહેર:

  template <typename Appl, typename ...Ts> auto
  operator () (Appl&& a, Ts&&... ds) અચલ HB_AUTO_RETURN
  (
    impl (std::forward<Appl> (a),
	  hb_prioritize,
	  std::forward<Ts> (ds)...)
  )
}
HB_FUNCOBJ (hb_invoke);

template <unsigned Pos, typename Appl, typename V>
સંરચના hb_partial_t
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
    પાછા hb_invoke (std::forward<Appl> (a),
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
    પાછા hb_invoke (std::forward<Appl> (a),
		      std::forward<T0> (d0),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }

  ખાનગી:
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
  decltype(auto) operator () (_T&& _v) અચલ \
  { પાછા hb_partial<Pos> (આ, std::forward<_T> (_v)); } \
  static_assert (સાચું, "")
#નહીંતર
/* https://github.com/harfbuzz/harfbuzz/issues/1724 */
#define HB_PARTIALIZE(Pos) \
  template <typename _T> \
  auto operator () (_T&& _v) અચલ HB_AUTO_RETURN \
  (hb_partial<Pos> (+આ, std::forward<_T> (_v))) \
  static_assert (સાચું, "")
#endif


સંરચના
{
  ખાનગી:

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<1>) અચલ HB_AUTO_RETURN
  (
    hb_deref (std::forward<Pred> (p)).has (std::forward<Val> (v))
  )

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<0>) અચલ HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Pred> (p),
	       std::forward<Val> (v))
  )

  જાહેર:

  template <typename Pred, typename Val> auto
  operator () (Pred&& p, Val &&v) અચલ HB_RETURN (તાર્કિક,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_has);

સંરચના
{
  ખાનગી:

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<1>) અચલ HB_AUTO_RETURN
  (
    hb_has (std::forward<Pred> (p),
	    std::forward<Val> (v))
  )

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<0>) અચલ HB_AUTO_RETURN
  (
    std::forward<Pred> (p) == std::forward<Val> (v)
  )

  જાહેર:

  template <typename Pred, typename Val> auto
  operator () (Pred&& p, Val &&v) અચલ HB_RETURN (તાર્કિક,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_match);

સંરચના
{
  ખાનગી:

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<2>) અચલ HB_AUTO_RETURN
  (
    hb_deref (std::forward<Proj> (f)).get (std::forward<Val> (v))
  )

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<1>) અચલ HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Proj> (f),
	       std::forward<Val> (v))
  )

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<0>) અચલ HB_AUTO_RETURN
  (
    std::forward<Proj> (f)[std::forward<Val> (v)]
  )

  જાહેર:

  template <typename Proj, typename Val> auto
  operator () (Proj&& f, Val &&v) અચલ HB_AUTO_RETURN
  (
    impl (std::forward<Proj> (f),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_get);

સંરચના
{
  ખાનગી:

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<3>) અચલ HB_AUTO_RETURN
  (
    std::forward<T2> (v2).cmp (std::forward<T1> (v1)) == 0
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<2>) અચલ HB_AUTO_RETURN
  (
    std::forward<T1> (v1).cmp (std::forward<T2> (v2)) == 0
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<1>) અચલ HB_AUTO_RETURN
  (
    std::forward<T1> (v1) == std::forward<T2> (v2)
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<0>) અચલ HB_AUTO_RETURN
  (
    std::forward<T2> (v2) == std::forward<T1> (v1)
  )

  જાહેર:

  template <typename T1, typename T2> auto
  operator () (T1&& v1, T2 &&v2) અચલ HB_AUTO_RETURN
  (
    impl (std::forward<T1> (v1),
	  std::forward<T2> (v2),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_equal);

સંરચના
{
  template <typename T> શૂન્ય
  operator () (T& a, T& b) અચલ
  {
    using std::swap; // allow ADL
    swap (a, b);
  }
}
HB_FUNCOBJ (hb_swap);


template <typename T1, typename T2>
સંરચના hb_pair_t
{
  પ્રકારવ્યાખ્યા T1 first_t;
  પ્રકારવ્યાખ્યા T2 second_t;
  પ્રકારવ્યાખ્યા hb_pair_t<T1, T2> pair_t;

  template <typename U1 = T1, typename U2 = T2,
	    hb_enable_if (std::is_default_constructible<U1>::value &&
			  std::is_default_constructible<U2>::value)>
  hb_pair_t () : first (), second () {}
  hb_pair_t (T1 a, T2 b) : first (std::forward<T1> (a)), second (std::forward<T2> (b)) {}

  template <typename Q1, typename Q2,
	    hb_enable_if (hb_is_convertible (T1, Q1) &&
			  hb_is_convertible (T2, Q2))>
  operator hb_pair_t<Q1, Q2> () { પાછા hb_pair_t<Q1, Q2> (first, second); }

  hb_pair_t<T1, T2> reverse () અચલ
  { પાછા hb_pair_t<T1, T2> (second, first); }

  તાર્કિક operator == (અચલ pair_t& o) અચલ { પાછા first == o.first && second == o.second; }
  તાર્કિક operator != (અચલ pair_t& o) અચલ { પાછા !(*આ == o); }
  તાર્કિક operator < (અચલ pair_t& o) અચલ { પાછા first < o.first || (first == o.first && second < o.second); }
  તાર્કિક operator >= (અચલ pair_t& o) અચલ { પાછા !(*આ < o); }
  તાર્કિક operator > (અચલ pair_t& o) અચલ { પાછા first > o.first || (first == o.first && second > o.second); }
  તાર્કિક operator <= (અચલ pair_t& o) અચલ { પાછા !(*આ > o); }

  સ્થિર પૂર્ણાંક cmp (અચલ શૂન્ય *pa, અચલ શૂન્ય *pb)
  {
    pair_t *a = (pair_t *) pa;
    pair_t *b = (pair_t *) pb;

    જો (a->first < b->first) પાછા -1;
    જો (a->first > b->first) પાછા +1;
    જો (a->second < b->second) પાછા -1;
    જો (a->second > b->second) પાછા +1;
    પાછા 0;
  }

  friend શૂન્ય swap (hb_pair_t& a, hb_pair_t& b) noexcept
  {
    hb_swap (a.first, b.first);
    hb_swap (a.second, b.second);
  }


  T1 first;
  T2 second;
};
template <typename T1, typename T2> સ્થિર અંતઃરેખ hb_pair_t<T1, T2>
hb_pair (T1&& a, T2&& b) { પાછા hb_pair_t<T1, T2> (a, b); }

પ્રકારવ્યાખ્યા hb_pair_t<hb_codepoint_t, hb_codepoint_t> hb_codepoint_pair_t;

સંરચના
{
  template <typename Pair> constexpr typename Pair::first_t
  operator () (અચલ Pair& pair) અચલ { પાછા pair.first; }
}
HB_FUNCOBJ (hb_first);

સંરચના
{
  template <typename Pair> constexpr typename Pair::second_t
  operator () (અચલ Pair& pair) અચલ { પાછા pair.second; }
}
HB_FUNCOBJ (hb_second);

/* Note.  In min/max impl, we can use hb_type_identity<T> for second argument.
 * However, that would silently convert between different-signedness integers.
 * Instead we accept two different types, such that compiler can err if
 * comparing integers of different signedness. */
સંરચના
{
  template <typename T, typename T2> constexpr auto
  operator () (T&& a, T2&& b) અચલ HB_AUTO_RETURN
  (a <= b ? a : b)
}
HB_FUNCOBJ (hb_min);
સંરચના
{
  template <typename T, typename T2> constexpr auto
  operator () (T&& a, T2&& b) અચલ HB_AUTO_RETURN
  (a >= b ? a : b)
}
HB_FUNCOBJ (hb_max);
સંરચના
{
  template <typename T, typename T2, typename T3> constexpr auto
  operator () (T&& x, T2&& min, T3&& max) અચલ HB_AUTO_RETURN
  (hb_min (hb_max (std::forward<T> (x), std::forward<T2> (min)), std::forward<T3> (max)))
}
HB_FUNCOBJ (hb_clamp);

/* Signed saturating arithmetic. */
template <typename T>
સ્થિર HB_ALWAYS_INLINE T
hb_saturate_add (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#જો hb_has_builtin(__builtin_add_overflow)
  T result;
  જો (likely (!__builtin_add_overflow (a, b, &result)))
    પાછા result;
#નહીંતર
  જો (b > 0)
  {
    જો (likely (a <= hb_int_max (T) - b))
      પાછા a + b;
  }
  નહીંતર જો (likely (a >= hb_int_min (T) - b))
    પાછા a + b;
#endif

  પાછા a < 0 ? hb_int_min (T) : hb_int_max (T);
}

template <typename T>
સ્થિર HB_ALWAYS_INLINE T
hb_saturate_sub (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#જો hb_has_builtin(__builtin_sub_overflow)
  T result;
  જો (likely (!__builtin_sub_overflow (a, b, &result)))
    પાછા result;
#નહીંતર
  જો (b > 0)
  {
    જો (likely (a >= hb_int_min (T) + b))
      પાછા a - b;
  }
  નહીંતર જો (likely (a <= hb_int_max (T) + b))
    પાછા a - b;
#endif

  પાછા b < 0 ? hb_int_max (T) : hb_int_min (T);
}

template <typename T>
સ્થિર HB_ALWAYS_INLINE T
hb_saturate_neg (T a)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  પાછા hb_saturate_sub ((T) 0, a);
}

/* Convert a floating-point value to integer type T, saturating to T's
 * range instead of relying on the undefined behavior of an out-of-range
 * float-to-int conversion.  NaN saturates to the minimum of T. */
template <typename T>
સ્થિર HB_ALWAYS_INLINE T
hb_clamp_to (દ્વિગુણ v)
{
  પાછા (T) hb_clamp (v,
		       (દ્વિગુણ) hb_int_min (T),
		       (દ્વિગુણ) hb_int_max (T));
}

template <typename T>
સ્થિર HB_ALWAYS_INLINE T
hb_clamp_to (દશાંશ v)
{
  /* Widen explicitly: float cannot represent all integer bounds exactly,
   * so the clamp itself is done in double. */
  પાછા hb_clamp_to<T> ((દ્વિગુણ) v);
}

template <typename T>
સ્થિર HB_ALWAYS_INLINE T
hb_clamp_to (int64_t v)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  પાછા (T) hb_clamp (v,
		       (int64_t) hb_int_min (T),
		       (int64_t) hb_int_max (T));
}

/*
 * Bithacks.
 */

/* Return the number of 1 bits in a uint8_t; faster than hb_popcount() */
સ્થિર અંતઃરેખ unsigned
hb_popcount8 (uint8_t v)
{
  સ્થિર અચલ uint8_t popcount4[16] = {
    0, 1, 1, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 2, 3, 3, 4
  };
  પાછા popcount4[v & 0xF] + popcount4[v >> 4];
}

/* Return the number of 1 bits in v. */
template <typename T>
સ્થિર અંતઃરેખ unsigned પૂર્ણાંક
hb_popcount (T v)
{
#જો hb_has_builtin(__builtin_popcount)
  જો (કદ (T) <= કદ (unsigned પૂર્ણાંક))
    પાછા __builtin_popcount (v);
#endif

#જો hb_has_builtin(__builtin_popcountl)
  જો (કદ (T) <= કદ (unsigned long))
    પાછા __builtin_popcountl (v);
#endif

#જો hb_has_builtin(__builtin_popcountll)
  જો (કદ (T) <= કદ (unsigned long long))
    પાછા __builtin_popcountll (v);
#endif

  જો (કદ (T) <= 4)
  {
    /* "HACKMEM 169" */
    uint32_t y;
    y = (v >> 1) &033333333333;
    y = v - y - ((y >>1) & 033333333333);
    પાછા (((y + (y >> 3)) & 030707070707) % 077);
  }

  જો (કદ (T) == 8)
  {
    uint64_t y = (uint64_t) v;
    y -= ((y >> 1) & 0x5555555555555555ull);
    y = (y & 0x3333333333333333ull) + (y >> 2 & 0x3333333333333333ull);
    પાછા ((y + (y >> 4)) & 0xf0f0f0f0f0f0f0full) * 0x101010101010101ull >> 56;
  }

  જો (કદ (T) == 16)
  {
    unsigned પૂર્ણાંક shift = 64;
    પાછા hb_popcount<uint64_t> ((uint64_t) v) + hb_popcount ((uint64_t) (v >> shift));
  }

  પ્રતિજ્ઞા (0);
  પાછા 0; /* Shut up stupid compiler. */
}

/* Returns the number of bits needed to store number */
template <typename T>
સ્થિર અંતઃરેખ unsigned પૂર્ણાંક
hb_bit_storage (T v)
{
  જો (unlikely (!v)) પાછા 0;

#જો hb_has_builtin(__builtin_clz)
  જો (કદ (T) <= કદ (unsigned પૂર્ણાંક))
    પાછા કદ (unsigned પૂર્ણાંક) * 8 - __builtin_clz (v);
#endif

#જો hb_has_builtin(__builtin_clzl)
  જો (કદ (T) <= કદ (unsigned long))
    પાછા કદ (unsigned long) * 8 - __builtin_clzl (v);
#endif

#જો hb_has_builtin(__builtin_clzll)
  જો (કદ (T) <= કદ (unsigned long long))
    પાછા કદ (unsigned long long) * 8 - __builtin_clzll (v);
#endif

#જો (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  જો (કદ (T) <= કદ (unsigned પૂર્ણાંક))
  {
    unsigned long where;
    _BitScanReverse (&where, v);
    પાછા 1 + where;
  }
# જો defined(_WIN64)
  જો (કદ (T) <= 8)
  {
    unsigned long where;
    _BitScanReverse64 (&where, v);
    પાછા 1 + where;
  }
# endif
#endif

  જો (કદ (T) <= 4)
  {
    /* "bithacks" */
    અચલ unsigned પૂર્ણાંક b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    અચલ unsigned પૂર્ણાંક S[] = {1, 2, 4, 8, 16};
    unsigned પૂર્ણાંક r = 0;
    માટે (પૂર્ણાંક i = 4; i >= 0; i--)
      જો (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    પાછા r + 1;
  }
  જો (કદ (T) <= 8)
  {
    /* "bithacks" */
    અચલ uint64_t b[] = {0x2ULL, 0xCULL, 0xF0ULL, 0xFF00ULL, 0xFFFF0000ULL, 0xFFFFFFFF00000000ULL};
    અચલ unsigned પૂર્ણાંક S[] = {1, 2, 4, 8, 16, 32};
    unsigned પૂર્ણાંક r = 0;
    માટે (પૂર્ણાંક i = 5; i >= 0; i--)
      જો (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    પાછા r + 1;
  }
  જો (કદ (T) == 16)
  {
    unsigned પૂર્ણાંક shift = 64;
    પાછા (v >> shift) ? hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift :
			  hb_bit_storage<uint64_t> ((uint64_t) v);
  }

  પ્રતિજ્ઞા (0);
  પાછા 0; /* Shut up stupid compiler. */
}

/* Returns the number of zero bits in the least significant side of v */
template <typename T>
સ્થિર અંતઃરેખ unsigned પૂર્ણાંક
hb_ctz (T v)
{
  જો (unlikely (!v)) પાછા 8 * કદ (T);

#જો hb_has_builtin(__builtin_ctz)
  જો (કદ (T) <= કદ (unsigned પૂર્ણાંક))
    પાછા __builtin_ctz (v);
#endif

#જો hb_has_builtin(__builtin_ctzl)
  જો (કદ (T) <= કદ (unsigned long))
    પાછા __builtin_ctzl (v);
#endif

#જો hb_has_builtin(__builtin_ctzll)
  જો (કદ (T) <= કદ (unsigned long long))
    પાછા __builtin_ctzll (v);
#endif

#જો (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  જો (કદ (T) <= કદ (unsigned પૂર્ણાંક))
  {
    unsigned long where;
    _BitScanForward (&where, v);
    પાછા where;
  }
# જો defined(_WIN64)
  જો (કદ (T) <= 8)
  {
    unsigned long where;
    _BitScanForward64 (&where, v);
    પાછા where;
  }
# endif
#endif

  જો (કદ (T) <= 4)
  {
    /* "bithacks" */
    unsigned પૂર્ણાંક c = 32;
    v &= - (int32_t) v;
    જો (v) c--;
    જો (v & 0x0000FFFF) c -= 16;
    જો (v & 0x00FF00FF) c -= 8;
    જો (v & 0x0F0F0F0F) c -= 4;
    જો (v & 0x33333333) c -= 2;
    જો (v & 0x55555555) c -= 1;
    પાછા c;
  }
  જો (કદ (T) <= 8)
  {
    /* "bithacks" */
    unsigned પૂર્ણાંક c = 64;
    v &= - (int64_t) (v);
    જો (v) c--;
    જો (v & 0x00000000FFFFFFFFULL) c -= 32;
    જો (v & 0x0000FFFF0000FFFFULL) c -= 16;
    જો (v & 0x00FF00FF00FF00FFULL) c -= 8;
    જો (v & 0x0F0F0F0F0F0F0F0FULL) c -= 4;
    જો (v & 0x3333333333333333ULL) c -= 2;
    જો (v & 0x5555555555555555ULL) c -= 1;
    પાછા c;
  }
  જો (કદ (T) == 16)
  {
    unsigned પૂર્ણાંક shift = 64;
    પાછા (uint64_t) v ? hb_bit_storage<uint64_t> ((uint64_t) v) :
			  hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift;
  }

  પ્રતિજ્ઞા (0);
  પાછા 0; /* Shut up stupid compiler. */
}


/*
 * Tiny stuff.
 */

/* ASCII tag/character handling */
સ્થિર અંતઃરેખ તાર્કિક ISALPHA (unsigned અક્ષર c)
{ પાછા (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
સ્થિર અંતઃરેખ તાર્કિક ISALNUM (unsigned અક્ષર c)
{ પાછા (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }
સ્થિર અંતઃરેખ તાર્કિક ISSPACE (unsigned અક્ષર c)
{ પાછા c == ' ' || c =='\f'|| c =='\n'|| c =='\r'|| c =='\t'|| c =='\v'; }
સ્થિર અંતઃરેખ unsigned અક્ષર TOUPPER (unsigned અક્ષર c)
{ પાછા (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c; }
સ્થિર અંતઃરેખ unsigned અક્ષર TOLOWER (unsigned અક્ષર c)
{ પાછા (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }
સ્થિર અંતઃરેખ તાર્કિક ISHEX (unsigned અક્ષર c)
{ પાછા (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
સ્થિર અંતઃરેખ unsigned અક્ષર TOHEX (uint8_t c)
{ પાછા (c & 0xF) <= 9 ? (c & 0xF) + '0' : (c & 0xF) + 'a' - 10; }
સ્થિર અંતઃરેખ uint8_t FROMHEX (unsigned અક્ષર c)
{ પાછા (c >= '0' && c <= '9') ? c - '0' : TOLOWER (c) - 'a' + 10; }

સ્થિર અંતઃરેખ unsigned પૂર્ણાંક DIV_CEIL (અચલ unsigned પૂર્ણાંક a, unsigned પૂર્ણાંક b)
{ પાછા (a + (b - 1)) / b; }


#undef  ARRAY_LENGTH
template <typename Type, unsigned પૂર્ણાંક n>
સ્થિર અંતઃરેખ unsigned પૂર્ણાંક ARRAY_LENGTH (અચલ Type (&)[n]) { પાછા n; }
/* A const version, but does not detect erratically being called on pointers. */
#define ARRAY_LENGTH_CONST(__array) ((signed પૂર્ણાંક) (કદ (__array) / કદ (__array[0])))


સ્થિર અંતઃરેખ શૂન્ય *
hb_memcpy (શૂન્ય *__restrict dst, અચલ શૂન્ય *__restrict src, size_t len)
{
  /* It's illegal to pass 0 as size to memcpy. */
  જો (unlikely (!len)) પાછા dst;
  પાછા memcpy (dst, src, len);
}

સ્થિર અંતઃરેખ પૂર્ણાંક
hb_memcmp (અચલ શૂન્ય *a, અચલ શૂન્ય *b, unsigned પૂર્ણાંક len)
{
  /* It's illegal to pass NULL to memcmp(), even if len is zero.
   * So, wrap it.
   * https://sourceware.org/bugzilla/show_bug.cgi?id=23878 */
  જો (unlikely (!len)) પાછા 0;
  પાછા memcmp (a, b, len);
}

સ્થિર અંતઃરેખ શૂન્ય *
hb_memset (શૂન્ય *s, પૂર્ણાંક c, unsigned પૂર્ણાંક n)
{
  /* It's illegal to pass NULL to memset(), even if n is zero. */
  જો (unlikely (!n)) પાછા s;
  પાછા memset (s, c, n);
}

સ્થિર અંતઃરેખ unsigned પૂર્ણાંક
hb_ceil_to_4 (unsigned પૂર્ણાંક v)
{
  પાછા ((v - 1) | 3) + 1;
}

template <typename T> સ્થિર અંતઃરેખ તાર્કિક
hb_in_range (T u, T lo, T hi)
{
  static_assert (!std::is_signed<T>::value, "");

  /* The casts below are important as if T is smaller than int,
   * the subtract results will become a signed int! */
  પાછા (T)(u - lo) <= (T)(hi - lo);
}
template <typename T> સ્થિર અંતઃરેખ તાર્કિક
hb_in_ranges (T u, T lo1, T hi1)
{
  પાછા hb_in_range (u, lo1, hi1);
}
template <typename T, typename ...Ts> સ્થિર અંતઃરેખ તાર્કિક
hb_in_ranges (T u, T lo1, T hi1, Ts... ds)
{
  પાછા hb_in_range<T> (u, lo1, hi1) || hb_in_ranges<T> (u, ds...);
}


/*
 * Overflow checking.
 */

સ્થિર અંતઃરેખ તાર્કિક
hb_unsigned_mul_overflows (unsigned પૂર્ણાંક ગણતરી, unsigned પૂર્ણાંક size, unsigned *result = nullptr)
{
#જો hb_has_builtin(__builtin_mul_overflow)
  unsigned ઢગલો_result;
  જો (!result)
    result = &ઢગલો_result;
  પાછા __builtin_mul_overflow (ગણતરી, size, result);
#endif

  જો (result)
    *result = ગણતરી * size;
  પાછા (size > 0) && (ગણતરી >= ((unsigned પૂર્ણાંક) -1) / size);
}

સ્થિર અંતઃરેખ તાર્કિક
hb_unsigned_add_overflows (unsigned પૂર્ણાંક a, unsigned પૂર્ણાંક b, unsigned *result = nullptr)
{
#જો hb_has_builtin(__builtin_add_overflow)
  unsigned ઢગલો_result;
  જો (!result)
    result = &ઢગલો_result;
  પાછા __builtin_add_overflow (a, b, result);
#endif

  જો (result)
    *result = a + b;
  પાછા b > (unsigned પૂર્ણાંક) -1 - a;
}

/* Saturating arithmetic on size_t.  On platforms where size_t is wider than
 * unsigned int (i.e. 64-bit), the inputs to get_size()-style computations
 * (counts and static_sizes which are at most 32-bit) cannot overflow size_t,
 * so these reduce to plain arithmetic.  On 32-bit platforms (size_t is
 * unsigned int), they saturate to SIZE_MAX so sanitize/serialize callers
 * naturally reject the resulting size as out-of-range. */

સ્થિર અંતઃરેખ size_t
hb_unsigned_mul_saturate (size_t a, size_t b)
{
  જો (કદ (size_t) > કદ (unsigned પૂર્ણાંક))
    પાછા a * b;
#જો hb_has_builtin(__builtin_mul_overflow)
  size_t result;
  જો (__builtin_mul_overflow (a, b, &result))
    પાછા (size_t) -1;
  પાછા result;
#નહીંતર
  જો (b > 0 && a > ((size_t) -1) / b) પાછા (size_t) -1;
  પાછા a * b;
#endif
}

સ્થિર અંતઃરેખ size_t
hb_unsigned_add_saturate (size_t a, size_t b)
{
  જો (કદ (size_t) > કદ (unsigned પૂર્ણાંક))
    પાછા a + b;
#જો hb_has_builtin(__builtin_add_overflow)
  size_t result;
  જો (__builtin_add_overflow (a, b, &result))
    પાછા (size_t) -1;
  પાછા result;
#નહીંતર
  જો (b > ((size_t) -1) - a) પાછા (size_t) -1;
  પાછા a + b;
#endif
}

/* Variadic forms: fold left across all arguments. */
template <typename ...Ts>
સ્થિર અંતઃરેખ size_t
hb_unsigned_mul_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ પાછા hb_unsigned_mul_saturate (hb_unsigned_mul_saturate (a, b), c, rest...); }

template <typename ...Ts>
સ્થિર અંતઃરેખ size_t
hb_unsigned_add_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ પાછા hb_unsigned_add_saturate (hb_unsigned_add_saturate (a, b), c, rest...); }

/* Saturating mul-add: a * b + c.  Covers the dominant get_size() pattern
 * `count * static_size + min_size`. */
સ્થિર અંતઃરેખ size_t
hb_unsigned_mul_add_saturate (size_t a, size_t b, size_t c)
{ પાછા hb_unsigned_add_saturate (hb_unsigned_mul_saturate (a, b), c); }


/*
 * Sort and search.
 */

template <typename K, typename V, typename ...Ts>
સ્થિર પૂર્ણાંક
_hb_cmp_method (અચલ શૂન્ય *pkey, અચલ શૂન્ય *pval, Ts... ds)
{
  અચલ K& key = * (અચલ K*) pkey;
  અચલ V& val = * (અચલ V*) pval;

  પાછા val.cmp (key, ds...);
}

template <typename K, typename V>
સ્થિર પૂર્ણાંક
_hb_cmp_operator (અચલ શૂન્ય *pkey, અચલ શૂન્ય *pval)
{
  અચલ K& key = * (અચલ K*) pkey;
  અચલ V& val = * (અચલ V*) pval;

  જો (key < val) પાછા -1;
  જો (key > val) પાછા  1;
  પાછા 0;
}

template <typename V, typename K, typename ...Ts>
HB_HOT
સ્થિર અંતઃરેખ તાર્કિક
hb_bsearch_impl (unsigned *pos, /* Out */
		 અચલ K& key,
		 V* base, size_t nmemb, size_t stride,
		 પૂર્ણાંક (*compar)(અચલ શૂન્ય *ચાવી_3, અચલ શૂન્ય *_item, Ts... _ds),
		 Ts... ds)
{
  /* This is our *only* bsearch implementation. */

  પૂર્ણાંક min = 0, max = (પૂર્ણાંક) nmemb - 1;
  જ્યારે (min <= max)
  {
    પૂર્ણાંક mid = ((unsigned પૂર્ણાંક) min + (unsigned પૂર્ણાંક) max) / 2;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    V* p = (V*) (((અચલ અક્ષર *) base) + (mid * stride));
#pragma GCC diagnostic કાઢો
    પૂર્ણાંક c = compar ((અચલ શૂન્ય *) std::addressof (key), (અચલ શૂન્ય *) p, ds...);
    જો (c < 0)
      max = mid - 1;
    નહીંતર જો (c > 0)
      min = mid + 1;
    નહીંતર
    {
      *pos = mid;
      પાછા સાચું;
    }
  }
  *pos = min;
  પાછા ખોટું;
}

template <typename V, typename K>
સ્થિર અંતઃરેખ V*
hb_bsearch (અચલ K& key, V* base,
	    size_t nmemb, size_t stride = કદ (V),
	    પૂર્ણાંક (*compar)(અચલ શૂન્ય *ચાવી_3, અચલ શૂન્ય *_item) = _hb_cmp_method<K, V>)
{
  unsigned pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  પાછા hb_bsearch_impl (&pos, key, base, nmemb, stride, compar) ?
	 (V*) (((અચલ અક્ષર *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic કાઢો
}
template <typename V, typename K, typename ...Ts>
સ્થિર અંતઃરેખ V*
hb_bsearch (અચલ K& key, V* base,
	    size_t nmemb, size_t stride,
	    પૂર્ણાંક (*compar)(અચલ શૂન્ય *ચાવી_3, અચલ શૂન્ય *_item, Ts... _ds),
	    Ts... ds)
{
  unsigned pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  પાછા hb_bsearch_impl (&pos, key, base, nmemb, stride, compar, ds...) ?
	 (V*) (((અચલ અક્ષર *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic કાઢો
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
સ્થિર અંતઃરેખ શૂન્ય
hb_qsort_loop (T *base, size_t nel, Compar compar)
{
  જ્યારે (nel > 24)
  {
    T *last = base + nel - 1;
    T *mid = base + nel / 2;

    /* Median-of-three pivot, parked at last-1. */
    જો (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    જો (compar (*mid, *last) > 0)
    {
      hb_swap (*mid, *last);
      જો (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    }
    hb_swap (*mid, *(last - 1));
    T &pivot = *(last - 1);

    /* Two-way Hoare partition.  Inner loops are unguarded:
     * median-of-three left *base <= pivot and *last >= pivot,
     * which act as sentinels. */
    T *i = base, *j = last - 1;
    જ્યારે (સાચું)
    {
      જ્યારે (compar (*++i, pivot) < 0) {}
      જ્યારે (compar (*--j, pivot) > 0) {}
      જો (i >= j) તોડો;
      hb_swap (*i, *j);
    }
    hb_swap (*i, *(last - 1));

    /* Recurse on smaller side, loop on larger — bounds stack
     * depth at O(log n). */
    size_t left  = (size_t) (i - base);
    size_t જમણું = nel - left - 1;
    જો (left < જમણું)
    {
      hb_qsort_loop (base, left, compar);
      base = i + 1;
      nel  = જમણું;
    }
    નહીંતર
    {
      hb_qsort_loop (i + 1, જમણું, compar);
      nel  = left;
    }
  }
}

template <typename T, typename Compar>
સ્થિર અંતઃરેખ શૂન્ય
hb_qsort_inline (T *base, size_t nel, Compar compar)
{
  hb_qsort_loop (base, nel, compar);

  /* Single final insertion sort over the whole array.  After
   * the partitioning loop, every element is within the threshold
   * of its sorted position, so this pass is O(n * threshold). */
  T *end = base + nel;
  માટે (T *pi = base + 1; pi < end; pi++)
    માટે (T *pj = pi; pj > base && compar (pj[-1], pj[0]) > 0; pj--)
      hb_swap (pj[-1], pj[0]);
}

સ્થિર અંતઃરેખ શૂન્ય
hb_qsort (શૂન્ય *base, size_t nel, size_t width,
	  પૂર્ણાંક (*compar)(અચલ શૂન્ય *_a, અચલ શૂન્ય *_b))
{
  qsort (base, nel, width, compar);
}




template <typename T, typename T2, typename T3 = પૂર્ણાંક> સ્થિર અંતઃરેખ શૂન્ય
hb_stable_sort (T *array, unsigned પૂર્ણાંક len, પૂર્ણાંક(*compar)(અચલ T2 *, અચલ T2 *), T3 *array2 = nullptr)
{
  static_assert (hb_is_trivially_copy_assignable (T), "");
  static_assert (hb_is_trivially_copy_assignable (T3), "");

  માટે (unsigned પૂર્ણાંક i = 1; i < len; i++)
  {
    unsigned પૂર્ણાંક j = i;
    જ્યારે (j && compar (&array[j - 1], &array[i]) > 0)
      j--;
    જો (i == j)
      ચાલુ;
    /* Move item i to occupy place for item j, shift what's in between. */
    {
      T t = array[i];
      memmove (&array[j + 1], &array[j], (i - j) * કદ (T));
      array[j] = t;
    }
    જો (array2)
    {
      T3 t = array2[i];
      memmove (&array2[j + 1], &array2[j], (i - j) * કદ (T3));
      array2[j] = t;
    }
  }
}

સ્થિર અંતઃરેખ hb_bool_t
hb_codepoint_parse (અચલ અક્ષર *s, unsigned પૂર્ણાંક len, પૂર્ણાંક base, hb_codepoint_t *out)
{
  unsigned પૂર્ણાંક v;
  અચલ અક્ષર *p = s;
  અચલ અક્ષર *end = p + len;
  જો (unlikely (!hb_parse_uint (&p, end, &v, સાચું/* whole buffer */, base)))
    પાછા ખોટું;

  *out = v;
  પાછા સાચું;
}


/* Operators. */

સંરચના
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (અચલ T &a, અચલ T &b) અચલ HB_AUTO_RETURN (a & b)
}
HB_FUNCOBJ (hb_bitwise_and);
સંરચના
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (અચલ T &a, અચલ T &b) અચલ HB_AUTO_RETURN (a | b)
}
HB_FUNCOBJ (hb_bitwise_or);
સંરચના
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (અચલ T &a, અચલ T &b) અચલ HB_AUTO_RETURN (a ^ b)
}
HB_FUNCOBJ (hb_bitwise_xor);
સંરચના
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (અચલ T &a, અચલ T &b) અચલ HB_AUTO_RETURN (~a & b)
}
HB_FUNCOBJ (hb_bitwise_lt);
સંરચના
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (અચલ T &a, અચલ T &b) અચલ HB_AUTO_RETURN (a & ~b)
}
HB_FUNCOBJ (hb_bitwise_gt); // aka sub
સંરચના
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (અચલ T &a, અચલ T &b) અચલ HB_AUTO_RETURN (~a | b)
}
HB_FUNCOBJ (hb_bitwise_le);
સંરચના
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (અચલ T &a, અચલ T &b) અચલ HB_AUTO_RETURN (a | ~b)
}
HB_FUNCOBJ (hb_bitwise_ge);
સંરચના
{
  template <typename T> constexpr auto
  operator () (અચલ T &a) અચલ HB_AUTO_RETURN (~a)
}
HB_FUNCOBJ (hb_bitwise_neg);

સંરચના
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (અચલ T &a, અચલ T2 &b) અચલ HB_AUTO_RETURN (a + b)
}
HB_FUNCOBJ (hb_add);
સંરચના
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (અચલ T &a, અચલ T2 &b) અચલ HB_AUTO_RETURN (a - b)
}
HB_FUNCOBJ (hb_sub);
સંરચના
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (અચલ T &a, અચલ T2 &b) અચલ HB_AUTO_RETURN (b - a)
}
HB_FUNCOBJ (hb_rsub);
સંરચના
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (અચલ T &a, અચલ T2 &b) અચલ HB_AUTO_RETURN (a * b)
}
HB_FUNCOBJ (hb_mul);
સંરચના
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (અચલ T &a, અચલ T2 &b) અચલ HB_AUTO_RETURN (a / b)
}
HB_FUNCOBJ (hb_div);
સંરચના
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (અચલ T &a, અચલ T2 &b) અચલ HB_AUTO_RETURN (a % b)
}
HB_FUNCOBJ (hb_mod);
સંરચના
{
  template <typename T> constexpr auto
  operator () (અચલ T &a) અચલ HB_AUTO_RETURN (+a)
}
HB_FUNCOBJ (hb_pos);
સંરચના
{
  template <typename T> constexpr auto
  operator () (અચલ T &a) અચલ HB_AUTO_RETURN (-a)
}
HB_FUNCOBJ (hb_neg);
સંરચના
{
  template <typename T> constexpr auto
  operator () (T &a) અચલ HB_AUTO_RETURN (++a)
}
HB_FUNCOBJ (hb_inc);
સંરચના
{
  template <typename T> constexpr auto
  operator () (T &a) અચલ HB_AUTO_RETURN (--a)
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
દ્વિગુણ solve_itp (func_t f,
		  દ્વિગુણ a, દ્વિગુણ b,
		  દ્વિગુણ epsilon,
		  દ્વિગુણ min_y, દ્વિગુણ max_y,
		  દ્વિગુણ &ya, દ્વિગુણ &yb, દ્વિગુણ &y)
{
  // Guard against degenerate interval
  જો (b - a <= 0.0)
  {
    y = ya;
    પાછા a;
  }

  unsigned n1_2 = (unsigned) (hb_max (ceil (log2 ((b - a) / epsilon)) - 1.0, 0.0));
  અચલ unsigned n0 = 1; // Hardwired
  અચલ દ્વિગુણ k1 = 0.2 / (b - a); // Hardwired.
  unsigned nmax = n0 + n1_2;
  દ્વિગુણ scaled_epsilon = epsilon * દ્વિગુણ (1llu << nmax);
  દ્વિગુણ _2_epsilon = 2.0 * epsilon;
  જ્યારે (b - a > _2_epsilon)
  {
    દ્વિગુણ x1_2 = 0.5 * (a + b);
    દ્વિગુણ r = scaled_epsilon - 0.5 * (b - a);
    // Guard against yb == ya to prevent division by zero
    દ્વિગુણ xf = (yb != ya) ? (yb * a - ya * b) / (yb - ya) : x1_2;
    દ્વિગુણ sigma = x1_2 - xf;
    દ્વિગુણ b_a = b - a;
    // This has k2 = 2 hardwired for efficiency.
    દ્વિગુણ b_a_k2 = b_a * b_a;
    દ્વિગુણ delta = k1 * b_a_k2;
    પૂર્ણાંક sigma_sign = sigma >= 0 ? +1 : -1;
    દ્વિગુણ xt = delta <= fabs (x1_2 - xf) ? xf + delta * sigma_sign : x1_2;
    દ્વિગુણ xitp = fabs (xt - x1_2) <= r ? xt : x1_2 - r * sigma_sign;
    દ્વિગુણ yitp = f (xitp);
    જો (yitp > max_y)
    {
      b = xitp;
      yb = yitp;
    }
    નહીંતર જો (yitp < min_y)
    {
      a = xitp;
      ya = yitp;
    }
    નહીંતર
    {
      y = yitp;
      પાછા xitp;
    }
    scaled_epsilon *= 0.5;
  }
  પાછા 0.5 * (a + b);
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
સંરચના hb_scope_guard_t
{
  explicit hb_scope_guard_t (F &&f) : f (std::move (f)), active (સાચું) {}
  hb_scope_guard_t (hb_scope_guard_t &&o) noexcept
    : f (std::move (o.f)), active (o.active) { o.active = ખોટું; }
  hb_scope_guard_t (અચલ hb_scope_guard_t &) = delete;
  hb_scope_guard_t &operator= (અચલ hb_scope_guard_t &) = delete;
  hb_scope_guard_t &operator= (hb_scope_guard_t &&) = delete;
  ~hb_scope_guard_t () { જો (active) f (); }

  /* Release: dismiss the guard so the cleanup does NOT run.  Use
   * when transferring ownership out of the scope. */
  શૂન્ય release () { active = ખોટું; }

  ખાનગી:
  F f;
  તાર્કિક active;
};

template <typename F>
સ્થિર અંતઃરેખ hb_scope_guard_t<F> hb_make_scope_guard (F &&f)
{ પાછા hb_scope_guard_t<F> (std::forward<F> (f)); }

#define HB_SCOPE_GUARD_NAME_(લીટી) hb_scope_guard_##line
#define HB_SCOPE_GUARD_NAME(લીટી) HB_SCOPE_GUARD_NAME_(લીટી)
#define HB_SCOPE_GUARD(stmt) \
  auto HB_SCOPE_GUARD_NAME(__LINE__) = \
    hb_make_scope_guard ([&]() { stmt; }); \
  (શૂન્ય) HB_SCOPE_GUARD_NAME(__LINE__)


#endif /* HB_ALGS_HH */
