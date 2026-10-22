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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-meta.hh"
#അന്തര്ഭാവ "hb-null.hh"
#അന്തര്ഭാവ "hb-number.hh"

#അന്തര്ഭാവ <algorithm>
#അന്തര്ഭാവ <initializer_list>
#അന്തര്ഭാവ <functional>
#അന്തര്ഭാവ <new>

സ്ഥിതം HB_ALWAYS_INLINE ശൂന്യം *
hb_malloc2 (size_t nmemb, size_t size)
{
  എങ്കിൽ (size && nmemb > SIZE_MAX / size) തിരികെ nullptr;
  തിരികെ hb_malloc (nmemb * size);
}
സ്ഥിതം HB_ALWAYS_INLINE ശൂന്യം *
hb_realloc2 (ശൂന്യം *ptr, size_t nmemb, size_t size)
{
  എങ്കിൽ (size && nmemb > SIZE_MAX / size) തിരികെ nullptr;
  തിരികെ hb_realloc (ptr, nmemb * size);
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
	ബാഹ്യ "C++" { \
	  സ്ഥിതം വരിയിൽ constexpr T operator | (T l, T r) { തിരികെ T ((unsigned) l | (unsigned) r); } \
	  സ്ഥിതം വരിയിൽ constexpr T operator & (T l, T r) { തിരികെ T ((unsigned) l & (unsigned) r); } \
	  സ്ഥിതം വരിയിൽ constexpr T operator ^ (T l, T r) { തിരികെ T ((unsigned) l ^ (unsigned) r); } \
	  സ്ഥിതം വരിയിൽ constexpr unsigned operator ~ (T r) { തിരികെ (~(unsigned) r); } \
	  സ്ഥിതം വരിയിൽ T& operator |= (T &l, T r) { l = l | r; തിരികെ l; } \
	  സ്ഥിതം വരിയിൽ T& operator &= (T& l, T r) { l = l & r; തിരികെ l; } \
	  സ്ഥിതം വരിയിൽ T& operator ^= (T& l, T r) { l = l ^ r; തിരികെ l; } \
	} \
	static_assert (സത്യം, "")

/* Useful for set-operations on small enums.
 * For example, for testing "x ∈ {x1, x2, x3}" use:
 * (FLAG_UNSAFE(x) & (FLAG(x1) | FLAG(x2) | FLAG(x3)))
 */
#define FLAG(x) (static_assert_വ്യഞ്ജകം ((unsigned)(x) < 32) + (((uint32_t) 1U) << (unsigned)(x)))
#define FLAG_UNSAFE(x) ((unsigned)(x) < 32 ? (((uint32_t) 1U) << (unsigned)(x)) : 0)
#define FLAG_RANGE(x,y) (static_assert_വ്യഞ്ജകം ((x) < (y)) + FLAG(y+1) - FLAG(x))
#define FLAG64(x) (static_assert_വ്യഞ്ജകം ((unsigned)(x) < 64) + (((uint64_t) 1ULL) << (unsigned)(x)))
#define FLAG64_UNSAFE(x) ((unsigned)(x) < 64 ? (((uint64_t) 1ULL) << (unsigned)(x)) : 0)


/*
 * Fixed-endian integers / floats.
 */


/* Endian swap, used in Windows related backends */
സ്ഥിതം വരിയിൽ constexpr uint16_t hb_uint16_swap (uint16_t v)
{ തിരികെ (v >> 8) | (v << 8); }
സ്ഥിതം വരിയിൽ constexpr uint32_t hb_uint32_swap (uint32_t v)
{ തിരികെ (hb_uint16_swap (v) << 16) | hb_uint16_swap (v >> 16); }

template <typename Type>
ഘടന __attribute__((packed)) hb_packed_t
{
  hb_packed_t () = സ്ഥിരസ്ഥിതി;
  constexpr hb_packed_t (Type V) : v (V) {}
  operator Type () സ്ഥിരം { തിരികെ v; }
  hb_packed_t & operator = (Type V) { v = V; തിരികെ *ഇത്; }

  സ്വകാര്യ:
  Type v;
};

#ifndef HB_FAST_അക്കം_ACCESS

#എങ്കിൽ defined(__OPTIMIZE__) && \
    defined(__BYTE_ORDER) && \
    (__BYTE_ORDER == __BIG_ENDIAN || \
     (__BYTE_ORDER == __LITTLE_ENDIAN && \
      hb_has_builtin(__builtin_bswap16) && \
      hb_has_builtin(__builtin_bswap32) && \
      hb_has_builtin(__builtin_bswap64)))
#define HB_FAST_അക്കം_ACCESS 1
#അല്ലെങ്കിൽ
#define HB_FAST_അക്കം_ACCESS 0
#endif

// https://github.com/harfbuzz/harfbuzz/issues/5456
#എങ്കിൽ defined(__GNUC__) && !defined(__clang__) && (__GNUC__ <= 12)
#undef HB_FAST_അക്കം_ACCESS
#define HB_FAST_അക്കം_ACCESS 0
#endif

#endif

template <തർക്കം BE, typename Type, പൂർണ്ണസംഖ്യ Bytes = വലുപ്പം (Type)>
ഘടന HBInt;
template <തർക്കം BE, typename Type>
ഘടന HBInt<BE, Type, 1>
{
  പൊതു:
  HBInt () = സ്ഥിരസ്ഥിതി;
  constexpr HBInt (Type V) : v {uint8_t (V)} {}
  constexpr operator Type () സ്ഥിരം { തിരികെ v; }
  സ്വകാര്യ: uint8_t v;
};
template <തർക്കം BE, typename Type>
ഘടന HBInt<BE, Type, 2>
{
  പൊതു:
  HBInt () = സ്ഥിരസ്ഥിതി;

  HBInt (Type V)
#എങ്കിൽ HB_FAST_അക്കം_ACCESS
  {
    എങ്കിൽ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint16_t> *) v) = V;
    അല്ലെങ്കിൽ
      *((hb_packed_t<uint16_t> *) v) = __builtin_bswap16 (V);
  }
#അല്ലെങ്കിൽ
    : v {BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >>  8) & 0xFF)} {}
#endif

  constexpr operator Type () സ്ഥിരം
  {
#എങ്കിൽ HB_FAST_അക്കം_ACCESS
    തിരികെ (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint16_t) *((സ്ഥിരം hb_packed_t<uint16_t> *) v)
    :
      __builtin_bswap16 ((uint16_t) *((സ്ഥിരം hb_packed_t<uint16_t> *) v))
    ;
#അല്ലെങ്കിൽ
    തിരികെ (BE ? (v[0] <<  8) : (v[0]      ))
	 + (BE ? (v[1]      ) : (v[1] <<  8));
#endif
  }
  സ്വകാര്യ: uint8_t v[2];
};
template <തർക്കം BE, typename Type>
ഘടന HBInt<BE, Type, 3>
{
  static_assert (!std::is_signed<Type>::value, "");
  പൊതു:
  HBInt () = സ്ഥിരസ്ഥിതി;
  constexpr HBInt (Type V) : v {BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
				BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
				BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V      ) & 0xFF)} {}

  constexpr operator Type () സ്ഥിരം { തിരികെ (BE ? (v[0] << 16) : (v[0]      ))
					  + (BE ? (v[1] <<  8) : (v[1] <<  8))
					  + (BE ? (v[2]      ) : (v[2] << 16)); }
  സ്വകാര്യ: uint8_t v[3];
};
template <തർക്കം BE, typename Type>
ഘടന HBInt<BE, Type, 4>
{
  template <തർക്കം, typename, പൂർണ്ണസംഖ്യ>
  friend ഘടന HBFloat;

  പൊതു:
  HBInt () = സ്ഥിരസ്ഥിതി;

  HBInt (Type V)
#എങ്കിൽ HB_FAST_അക്കം_ACCESS
  {
    എങ്കിൽ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint32_t> *) v) = V;
    അല്ലെങ്കിൽ
      *((hb_packed_t<uint32_t> *) v) = __builtin_bswap32 (V);
  }
#അല്ലെങ്കിൽ
    : v {BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 24) & 0xFF)} {}
#endif

  constexpr operator Type () സ്ഥിരം {
#എങ്കിൽ HB_FAST_അക്കം_ACCESS
    തിരികെ (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint32_t) *((സ്ഥിരം hb_packed_t<uint32_t> *) v)
    :
      __builtin_bswap32 ((uint32_t) *((സ്ഥിരം hb_packed_t<uint32_t> *) v))
    ;
#അല്ലെങ്കിൽ
    തിരികെ (BE ? (uint32_t (v[0]) << 24) : (uint32_t (v[0])      ))
	 + (BE ? (uint32_t (v[1]) << 16) : (uint32_t (v[1]) <<  8))
	 + (BE ? (uint32_t (v[2]) <<  8) : (uint32_t (v[2]) << 16))
	 + (BE ? (uint32_t (v[3])      ) : (uint32_t (v[3]) << 24));
#endif
  }
  സ്വകാര്യ: uint8_t v[4];
};
template <തർക്കം BE, typename Type>
ഘടന HBInt<BE, Type, 8>
{
  template <തർക്കം, typename, പൂർണ്ണസംഖ്യ>
  friend ഘടന HBFloat;

  പൊതു:
  HBInt () = സ്ഥിരസ്ഥിതി;

  HBInt (Type V)
#എങ്കിൽ HB_FAST_അക്കം_ACCESS
  {
    എങ്കിൽ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint64_t> *) v) = V;
    അല്ലെങ്കിൽ
      *((hb_packed_t<uint64_t> *) v) = __builtin_bswap64 (V);
  }
#അല്ലെങ്കിൽ
    : v {BE ? uint8_t ((V >> 56) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 48) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >> 40) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V >> 32) & 0xFF) : uint8_t ((V >> 24) & 0xFF),
	 BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V >> 32) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 40) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 48) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 56) & 0xFF)} {}
#endif

  constexpr operator Type () സ്ഥിരം {
#എങ്കിൽ HB_FAST_അക്കം_ACCESS
    തിരികെ (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint64_t) *((സ്ഥിരം hb_packed_t<uint64_t> *) v)
    :
      __builtin_bswap64 ((uint64_t) *((സ്ഥിരം hb_packed_t<uint64_t> *) v))
    ;
#അല്ലെങ്കിൽ
    തിരികെ (BE ? (uint64_t (v[0]) << 56) : (uint64_t (v[0])      ))
	 + (BE ? (uint64_t (v[1]) << 48) : (uint64_t (v[1]) <<  8))
	 + (BE ? (uint64_t (v[2]) << 40) : (uint64_t (v[2]) << 16))
	 + (BE ? (uint64_t (v[3]) << 32) : (uint64_t (v[3]) << 24))
	 + (BE ? (uint64_t (v[4]) << 24) : (uint64_t (v[4]) << 32))
	 + (BE ? (uint64_t (v[5]) << 16) : (uint64_t (v[5]) << 40))
	 + (BE ? (uint64_t (v[6]) <<  8) : (uint64_t (v[6]) << 48))
	 + (BE ? (uint64_t (v[7])      ) : (uint64_t (v[7]) << 56));
#endif
  }
  സ്വകാര്യ: uint8_t v[8];
};

/* Floats. */

template <തർക്കം BE, typename Type, പൂർണ്ണസംഖ്യ Bytes>
ഘടന HBFloat
{
  using IntType = typename std::conditional<Bytes == 4, uint32_t, uint64_t>::type;

  പൊതു:
  HBFloat () = സ്ഥിരസ്ഥിതി;

  HBFloat (Type V)
  {
#എങ്കിൽ HB_FAST_അക്കം_ACCESS
    {
	      എങ്കിൽ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
	      {
	        *((hb_packed_t<Type> *) v) = V;
	        തിരികെ;
	      }
	    }
#endif

    സംഘം {
      hb_packed_t<Type> f;
      hb_packed_t<IntType> i;
    } u = {{V}};

    സ്ഥിരം HBInt<BE, IntType> I = (IntType) u.i;
    വേണ്ടി (unsigned i = 0; i < Bytes; i++)
      v[i] = I.v[i];
  }

  /* c++14 constexpr */ operator Type () സ്ഥിരം
  {
#എങ്കിൽ HB_FAST_അക്കം_ACCESS
	    {
	      എങ്കിൽ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
		തിരികെ (Type) *((സ്ഥിരം hb_packed_t<Type> *) v);
	    }
#endif

    HBInt<BE, IntType> I;
    വേണ്ടി (unsigned i = 0; i < Bytes; i++)
      I.v[i] = v[i];

    സംഘം {
      hb_packed_t<IntType> i;
      hb_packed_t<Type> f;
    } u = {{I}};

    തിരികെ (Type) u.f;
  }
  സ്വകാര്യ: uint8_t v[Bytes];
};


/* We want our rounding towards +infinity. */
സ്ഥിതം വരിയിൽ ഇരട്ട
_hb_roundf (ഇരട്ട x) { തിരികെ floor (x + .5); }

സ്ഥിതം വരിയിൽ ദശാംശം
_hb_roundf (ദശാംശം x) { തിരികെ floor ((ഇരട്ട) x + .5); }

#define roundf(x) _hb_roundf(x)

സ്ഥിതം വരിയിൽ ശൂന്യം
hb_sincos (ദശാംശം rotation, ദശാംശം &s, ദശാംശം &c)
{
#ifdef HAVE_SINCOSF
  sincosf (rotation, &s, &c);
#അല്ലെങ്കിൽ
  c = cosf (rotation);
  s = sinf (rotation);
#endif
}
സ്ഥിതം വരിയിൽ ശൂന്യം
hb_sincos (ഇരട്ട rotation, ഇരട്ട &s, ഇരട്ട &c)
{
#ifdef HAVE_SINCOS
  sincos (rotation, &s, &c);
#അല്ലെങ്കിൽ
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


ഘടന
{
  /* Note.  This is dangerous in that if it's passed an rvalue, it returns rvalue-reference. */
  template <typename T> constexpr auto
  operator () (T&& v) സ്ഥിരം HB_AUTO_RETURN ( std::forward<T> (v) )
}
HB_FUNCOBJ (hb_identity);
ഘടന
{
  /* Like identity(), but only retains lvalue-references.  Rvalues are returned as rvalues. */
  template <typename T> constexpr T&
  operator () (T& v) സ്ഥിരം { തിരികെ v; }

  template <typename T> constexpr hb_remove_reference<T>
  operator () (T&& v) സ്ഥിരം { തിരികെ v; }
}
HB_FUNCOBJ (hb_lidentity);
ഘടന
{
  /* Like identity(), but always returns rvalue. */
  template <typename T> constexpr hb_remove_reference<T>
  operator () (T&& v) സ്ഥിരം { തിരികെ v; }
}
HB_FUNCOBJ (hb_ridentity);

ഘടന
{
  template <typename T> constexpr തർക്കം
  operator () (T&& v) സ്ഥിരം { തിരികെ തർക്കം (std::forward<T> (v)); }
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
			(ശൂന്യം) ((h) ^= (h) >> 23),		\
			(ശൂന്യം) ((h) *= 0x2127599bf4325c37ULL),	\
			(h) ^= (h) >> 47)

സ്ഥിതം വരിയിൽ uint64_t fasthash64(സ്ഥിരം ശൂന്യം *buf, size_t len, uint64_t seed)
{
	ഘടന __attribute__((packed)) packed_uint64_t { uint64_t v; };
	സ്ഥിരം uint64_t    m = 0x880355f21e6d1965ULL;
	സ്ഥിരം packed_uint64_t *pos = (സ്ഥിരം packed_uint64_t *)buf;
	സ്ഥിരം packed_uint64_t *end = pos + (len / 8);
	സ്ഥിരം unsigned അക്ഷരം *pos2;
	uint64_t h = seed ^ (len * m);
	uint64_t v;

#ifndef HB_OPTIMIZE_SIZE
	എങ്കിൽ (((uintptr_t) pos & 7) == 0)
	{
	  ആകുമ്പോൾ (pos != end)
	  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
	    v  = * (സ്ഥിരം uint64_t *) (pos++);
#pragma GCC diagnostic എടുക്കുക
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}
	അല്ലെങ്കിൽ
#endif
	{
	  ആകുമ്പോൾ (pos != end)
	  {
	    v  = pos++->v;
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}

	pos2 = (സ്ഥിരം unsigned അക്ഷരം*)pos;
	v = 0;

	തിരഞ്ഞെടുപ്പ് (len & 7) {
	അവസ്ഥ 7: v ^= (uint64_t)pos2[6] << 48; HB_FALLTHROUGH;
	അവസ്ഥ 6: v ^= (uint64_t)pos2[5] << 40; HB_FALLTHROUGH;
	അവസ്ഥ 5: v ^= (uint64_t)pos2[4] << 32; HB_FALLTHROUGH;
	അവസ്ഥ 4: v ^= (uint64_t)pos2[3] << 24; HB_FALLTHROUGH;
	അവസ്ഥ 3: v ^= (uint64_t)pos2[2] << 16; HB_FALLTHROUGH;
	അവസ്ഥ 2: v ^= (uint64_t)pos2[1] <<  8; HB_FALLTHROUGH;
	അവസ്ഥ 1: v ^= (uint64_t)pos2[0];
		h ^= fasthash_mix(v);
		h *= m;
	}

	തിരികെ fasthash_mix(h);
}

സ്ഥിതം വരിയിൽ uint32_t fasthash32(സ്ഥിരം ശൂന്യം *buf, size_t len, uint32_t seed)
{
	// the following trick converts the 64-bit hashcode to Fermat
	// residue, which shall retain information from both the higher
	// and lower parts of hashcode.
        uint64_t h = fasthash64(buf, len, seed);
	തിരികെ h - (h >> 32);
}

ഘടന
{
  സ്വകാര്യ:

  template <typename T> constexpr auto
  impl (സ്ഥിരം T& v, hb_priority<2>) സ്ഥിരം HB_RETURN (uint32_t, hb_deref (v).hash ())

  // Horrible: std:hash() of integers seems to be identity in gcc / clang?!
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  //
  // For performance characteristics see:
  // https://github.com/harfbuzz/harfbuzz/pull/4228#issuecomment-1565079537
  template <typename T,
	    hb_enable_if (std::is_integral<T>::value && വലുപ്പം (T) <= വലുപ്പം (uint32_t))> constexpr auto
  impl (സ്ഥിരം T& v, hb_priority<1>) സ്ഥിരം HB_RETURN (uint32_t, (uint32_t) v * 2654435761u /* Knuh's multiplicative hash */)
  template <typename T,
	    hb_enable_if (std::is_integral<T>::value && വലുപ്പം (T) > വലുപ്പം (uint32_t))> constexpr auto
  impl (സ്ഥിരം T& v, hb_priority<1>) സ്ഥിരം HB_RETURN (uint32_t, (uint32_t) (v ^ (v >> 32)) * 2654435761u /* Knuth's multiplicative hash */)

  template <typename T,
	    hb_enable_if (std::is_floating_point<T>::value)> constexpr auto
  impl (സ്ഥിരം T& v, hb_priority<1>) സ്ഥിരം HB_RETURN (uint32_t, fasthash32 (std::addressof (v), വലുപ്പം (T), 0xf437ffe6))

  template <typename T> constexpr auto
  impl (സ്ഥിരം T& v, hb_priority<0>) സ്ഥിരം HB_RETURN (uint32_t, std::hash<hb_decay<decltype (hb_deref (v))>>{} (hb_deref (v)))

  പൊതു:

  template <typename T> constexpr auto
  operator () (സ്ഥിരം T& v) സ്ഥിരം HB_RETURN (uint32_t, impl (v, hb_prioritize))
}
HB_FUNCOBJ (hb_hash);


ഘടന
{
  സ്വകാര്യ:

  /* Pointer-to-member-function. */
  template <typename Appl, typename T, typename ...Ts> auto
  impl (Appl&& a, hb_priority<2>, T &&v, Ts&&... ds) സ്ഥിരം HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v)).*std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  /* Pointer-to-member. */
  template <typename Appl, typename T> auto
  impl (Appl&& a, hb_priority<1>, T &&v) സ്ഥിരം HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v))).*std::forward<Appl> (a))

  /* Operator(). */
  template <typename Appl, typename ...Ts> auto
  impl (Appl&& a, hb_priority<0>, Ts&&... ds) സ്ഥിരം HB_AUTO_RETURN
  (hb_deref (std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  പൊതു:

  template <typename Appl, typename ...Ts> auto
  operator () (Appl&& a, Ts&&... ds) സ്ഥിരം HB_AUTO_RETURN
  (
    impl (std::forward<Appl> (a),
	  hb_prioritize,
	  std::forward<Ts> (ds)...)
  )
}
HB_FUNCOBJ (hb_invoke);

template <unsigned Pos, typename Appl, typename V>
ഘടന hb_partial_t
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
    തിരികെ hb_invoke (std::forward<Appl> (a),
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
    തിരികെ hb_invoke (std::forward<Appl> (a),
		      std::forward<T0> (d0),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }

  സ്വകാര്യ:
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
  decltype(auto) operator () (_T&& _v) സ്ഥിരം \
  { തിരികെ hb_partial<Pos> (ഇത്, std::forward<_T> (_v)); } \
  static_assert (സത്യം, "")
#അല്ലെങ്കിൽ
/* https://github.com/harfbuzz/harfbuzz/issues/1724 */
#define HB_PARTIALIZE(Pos) \
  template <typename _T> \
  auto operator () (_T&& _v) സ്ഥിരം HB_AUTO_RETURN \
  (hb_partial<Pos> (+ഇത്, std::forward<_T> (_v))) \
  static_assert (സത്യം, "")
#endif


ഘടന
{
  സ്വകാര്യ:

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<1>) സ്ഥിരം HB_AUTO_RETURN
  (
    hb_deref (std::forward<Pred> (p)).has (std::forward<Val> (v))
  )

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<0>) സ്ഥിരം HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Pred> (p),
	       std::forward<Val> (v))
  )

  പൊതു:

  template <typename Pred, typename Val> auto
  operator () (Pred&& p, Val &&v) സ്ഥിരം HB_RETURN (തർക്കം,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_has);

ഘടന
{
  സ്വകാര്യ:

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<1>) സ്ഥിരം HB_AUTO_RETURN
  (
    hb_has (std::forward<Pred> (p),
	    std::forward<Val> (v))
  )

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<0>) സ്ഥിരം HB_AUTO_RETURN
  (
    std::forward<Pred> (p) == std::forward<Val> (v)
  )

  പൊതു:

  template <typename Pred, typename Val> auto
  operator () (Pred&& p, Val &&v) സ്ഥിരം HB_RETURN (തർക്കം,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_match);

ഘടന
{
  സ്വകാര്യ:

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<2>) സ്ഥിരം HB_AUTO_RETURN
  (
    hb_deref (std::forward<Proj> (f)).get (std::forward<Val> (v))
  )

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<1>) സ്ഥിരം HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Proj> (f),
	       std::forward<Val> (v))
  )

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<0>) സ്ഥിരം HB_AUTO_RETURN
  (
    std::forward<Proj> (f)[std::forward<Val> (v)]
  )

  പൊതു:

  template <typename Proj, typename Val> auto
  operator () (Proj&& f, Val &&v) സ്ഥിരം HB_AUTO_RETURN
  (
    impl (std::forward<Proj> (f),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_get);

ഘടന
{
  സ്വകാര്യ:

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<3>) സ്ഥിരം HB_AUTO_RETURN
  (
    std::forward<T2> (v2).cmp (std::forward<T1> (v1)) == 0
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<2>) സ്ഥിരം HB_AUTO_RETURN
  (
    std::forward<T1> (v1).cmp (std::forward<T2> (v2)) == 0
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<1>) സ്ഥിരം HB_AUTO_RETURN
  (
    std::forward<T1> (v1) == std::forward<T2> (v2)
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<0>) സ്ഥിരം HB_AUTO_RETURN
  (
    std::forward<T2> (v2) == std::forward<T1> (v1)
  )

  പൊതു:

  template <typename T1, typename T2> auto
  operator () (T1&& v1, T2 &&v2) സ്ഥിരം HB_AUTO_RETURN
  (
    impl (std::forward<T1> (v1),
	  std::forward<T2> (v2),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_equal);

ഘടന
{
  template <typename T> ശൂന്യം
  operator () (T& a, T& b) സ്ഥിരം
  {
    using std::swap; // allow ADL
    swap (a, b);
  }
}
HB_FUNCOBJ (hb_swap);


template <typename T1, typename T2>
ഘടന hb_pair_t
{
  തരനിർവചനം T1 first_t;
  തരനിർവചനം T2 second_t;
  തരനിർവചനം hb_pair_t<T1, T2> pair_t;

  template <typename U1 = T1, typename U2 = T2,
	    hb_enable_if (std::is_default_constructible<U1>::value &&
			  std::is_default_constructible<U2>::value)>
  hb_pair_t () : first (), second () {}
  hb_pair_t (T1 a, T2 b) : first (std::forward<T1> (a)), second (std::forward<T2> (b)) {}

  template <typename Q1, typename Q2,
	    hb_enable_if (hb_is_convertible (T1, Q1) &&
			  hb_is_convertible (T2, Q2))>
  operator hb_pair_t<Q1, Q2> () { തിരികെ hb_pair_t<Q1, Q2> (first, second); }

  hb_pair_t<T1, T2> reverse () സ്ഥിരം
  { തിരികെ hb_pair_t<T1, T2> (second, first); }

  തർക്കം operator == (സ്ഥിരം pair_t& o) സ്ഥിരം { തിരികെ first == o.first && second == o.second; }
  തർക്കം operator != (സ്ഥിരം pair_t& o) സ്ഥിരം { തിരികെ !(*ഇത് == o); }
  തർക്കം operator < (സ്ഥിരം pair_t& o) സ്ഥിരം { തിരികെ first < o.first || (first == o.first && second < o.second); }
  തർക്കം operator >= (സ്ഥിരം pair_t& o) സ്ഥിരം { തിരികെ !(*ഇത് < o); }
  തർക്കം operator > (സ്ഥിരം pair_t& o) സ്ഥിരം { തിരികെ first > o.first || (first == o.first && second > o.second); }
  തർക്കം operator <= (സ്ഥിരം pair_t& o) സ്ഥിരം { തിരികെ !(*ഇത് > o); }

  സ്ഥിതം പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb)
  {
    pair_t *a = (pair_t *) pa;
    pair_t *b = (pair_t *) pb;

    എങ്കിൽ (a->first < b->first) തിരികെ -1;
    എങ്കിൽ (a->first > b->first) തിരികെ +1;
    എങ്കിൽ (a->second < b->second) തിരികെ -1;
    എങ്കിൽ (a->second > b->second) തിരികെ +1;
    തിരികെ 0;
  }

  friend ശൂന്യം swap (hb_pair_t& a, hb_pair_t& b) noexcept
  {
    hb_swap (a.first, b.first);
    hb_swap (a.second, b.second);
  }


  T1 first;
  T2 second;
};
template <typename T1, typename T2> സ്ഥിതം വരിയിൽ hb_pair_t<T1, T2>
hb_pair (T1&& a, T2&& b) { തിരികെ hb_pair_t<T1, T2> (a, b); }

തരനിർവചനം hb_pair_t<hb_codepoint_t, hb_codepoint_t> hb_codepoint_pair_t;

ഘടന
{
  template <typename Pair> constexpr typename Pair::first_t
  operator () (സ്ഥിരം Pair& pair) സ്ഥിരം { തിരികെ pair.first; }
}
HB_FUNCOBJ (hb_first);

ഘടന
{
  template <typename Pair> constexpr typename Pair::second_t
  operator () (സ്ഥിരം Pair& pair) സ്ഥിരം { തിരികെ pair.second; }
}
HB_FUNCOBJ (hb_second);

/* Note.  In min/max impl, we can use hb_type_identity<T> for second argument.
 * However, that would silently convert between different-signedness integers.
 * Instead we accept two different types, such that compiler can err if
 * comparing integers of different signedness. */
ഘടന
{
  template <typename T, typename T2> constexpr auto
  operator () (T&& a, T2&& b) സ്ഥിരം HB_AUTO_RETURN
  (a <= b ? a : b)
}
HB_FUNCOBJ (hb_min);
ഘടന
{
  template <typename T, typename T2> constexpr auto
  operator () (T&& a, T2&& b) സ്ഥിരം HB_AUTO_RETURN
  (a >= b ? a : b)
}
HB_FUNCOBJ (hb_max);
ഘടന
{
  template <typename T, typename T2, typename T3> constexpr auto
  operator () (T&& x, T2&& min, T3&& max) സ്ഥിരം HB_AUTO_RETURN
  (hb_min (hb_max (std::forward<T> (x), std::forward<T2> (min)), std::forward<T3> (max)))
}
HB_FUNCOBJ (hb_clamp);

/* Signed saturating arithmetic. */
template <typename T>
സ്ഥിതം HB_ALWAYS_INLINE T
hb_saturate_add (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#എങ്കിൽ hb_has_builtin(__builtin_add_overflow)
  T result;
  എങ്കിൽ (likely (!__builtin_add_overflow (a, b, &result)))
    തിരികെ result;
#അല്ലെങ്കിൽ
  എങ്കിൽ (b > 0)
  {
    എങ്കിൽ (likely (a <= hb_int_max (T) - b))
      തിരികെ a + b;
  }
  അല്ലെങ്കിൽ എങ്കിൽ (likely (a >= hb_int_min (T) - b))
    തിരികെ a + b;
#endif

  തിരികെ a < 0 ? hb_int_min (T) : hb_int_max (T);
}

template <typename T>
സ്ഥിതം HB_ALWAYS_INLINE T
hb_saturate_sub (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#എങ്കിൽ hb_has_builtin(__builtin_sub_overflow)
  T result;
  എങ്കിൽ (likely (!__builtin_sub_overflow (a, b, &result)))
    തിരികെ result;
#അല്ലെങ്കിൽ
  എങ്കിൽ (b > 0)
  {
    എങ്കിൽ (likely (a >= hb_int_min (T) + b))
      തിരികെ a - b;
  }
  അല്ലെങ്കിൽ എങ്കിൽ (likely (a <= hb_int_max (T) + b))
    തിരികെ a - b;
#endif

  തിരികെ b < 0 ? hb_int_max (T) : hb_int_min (T);
}

template <typename T>
സ്ഥിതം HB_ALWAYS_INLINE T
hb_saturate_neg (T a)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  തിരികെ hb_saturate_sub ((T) 0, a);
}

/* Convert a floating-point value to integer type T, saturating to T's
 * range instead of relying on the undefined behavior of an out-of-range
 * float-to-int conversion.  NaN saturates to the minimum of T. */
template <typename T>
സ്ഥിതം HB_ALWAYS_INLINE T
hb_clamp_to (ഇരട്ട v)
{
  തിരികെ (T) hb_clamp (v,
		       (ഇരട്ട) hb_int_min (T),
		       (ഇരട്ട) hb_int_max (T));
}

template <typename T>
സ്ഥിതം HB_ALWAYS_INLINE T
hb_clamp_to (ദശാംശം v)
{
  /* Widen explicitly: float cannot represent all integer bounds exactly,
   * so the clamp itself is done in double. */
  തിരികെ hb_clamp_to<T> ((ഇരട്ട) v);
}

template <typename T>
സ്ഥിതം HB_ALWAYS_INLINE T
hb_clamp_to (int64_t v)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  തിരികെ (T) hb_clamp (v,
		       (int64_t) hb_int_min (T),
		       (int64_t) hb_int_max (T));
}

/*
 * Bithacks.
 */

/* Return the number of 1 bits in a uint8_t; faster than hb_popcount() */
സ്ഥിതം വരിയിൽ unsigned
hb_popcount8 (uint8_t v)
{
  സ്ഥിതം സ്ഥിരം uint8_t popcount4[16] = {
    0, 1, 1, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 2, 3, 3, 4
  };
  തിരികെ popcount4[v & 0xF] + popcount4[v >> 4];
}

/* Return the number of 1 bits in v. */
template <typename T>
സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
hb_popcount (T v)
{
#എങ്കിൽ hb_has_builtin(__builtin_popcount)
  എങ്കിൽ (വലുപ്പം (T) <= വലുപ്പം (unsigned പൂർണ്ണസംഖ്യ))
    തിരികെ __builtin_popcount (v);
#endif

#എങ്കിൽ hb_has_builtin(__builtin_popcountl)
  എങ്കിൽ (വലുപ്പം (T) <= വലുപ്പം (unsigned long))
    തിരികെ __builtin_popcountl (v);
#endif

#എങ്കിൽ hb_has_builtin(__builtin_popcountll)
  എങ്കിൽ (വലുപ്പം (T) <= വലുപ്പം (unsigned long long))
    തിരികെ __builtin_popcountll (v);
#endif

  എങ്കിൽ (വലുപ്പം (T) <= 4)
  {
    /* "HACKMEM 169" */
    uint32_t y;
    y = (v >> 1) &033333333333;
    y = v - y - ((y >>1) & 033333333333);
    തിരികെ (((y + (y >> 3)) & 030707070707) % 077);
  }

  എങ്കിൽ (വലുപ്പം (T) == 8)
  {
    uint64_t y = (uint64_t) v;
    y -= ((y >> 1) & 0x5555555555555555ull);
    y = (y & 0x3333333333333333ull) + (y >> 2 & 0x3333333333333333ull);
    തിരികെ ((y + (y >> 4)) & 0xf0f0f0f0f0f0f0full) * 0x101010101010101ull >> 56;
  }

  എങ്കിൽ (വലുപ്പം (T) == 16)
  {
    unsigned പൂർണ്ണസംഖ്യ shift = 64;
    തിരികെ hb_popcount<uint64_t> ((uint64_t) v) + hb_popcount ((uint64_t) (v >> shift));
  }

  പ്രതിജ്ഞാ (0);
  തിരികെ 0; /* Shut up stupid compiler. */
}

/* Returns the number of bits needed to store number */
template <typename T>
സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
hb_bit_storage (T v)
{
  എങ്കിൽ (unlikely (!v)) തിരികെ 0;

#എങ്കിൽ hb_has_builtin(__builtin_clz)
  എങ്കിൽ (വലുപ്പം (T) <= വലുപ്പം (unsigned പൂർണ്ണസംഖ്യ))
    തിരികെ വലുപ്പം (unsigned പൂർണ്ണസംഖ്യ) * 8 - __builtin_clz (v);
#endif

#എങ്കിൽ hb_has_builtin(__builtin_clzl)
  എങ്കിൽ (വലുപ്പം (T) <= വലുപ്പം (unsigned long))
    തിരികെ വലുപ്പം (unsigned long) * 8 - __builtin_clzl (v);
#endif

#എങ്കിൽ hb_has_builtin(__builtin_clzll)
  എങ്കിൽ (വലുപ്പം (T) <= വലുപ്പം (unsigned long long))
    തിരികെ വലുപ്പം (unsigned long long) * 8 - __builtin_clzll (v);
#endif

#എങ്കിൽ (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  എങ്കിൽ (വലുപ്പം (T) <= വലുപ്പം (unsigned പൂർണ്ണസംഖ്യ))
  {
    unsigned long where;
    _BitScanReverse (&where, v);
    തിരികെ 1 + where;
  }
# എങ്കിൽ defined(_WIN64)
  എങ്കിൽ (വലുപ്പം (T) <= 8)
  {
    unsigned long where;
    _BitScanReverse64 (&where, v);
    തിരികെ 1 + where;
  }
# endif
#endif

  എങ്കിൽ (വലുപ്പം (T) <= 4)
  {
    /* "bithacks" */
    സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ S[] = {1, 2, 4, 8, 16};
    unsigned പൂർണ്ണസംഖ്യ r = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 4; i >= 0; i--)
      എങ്കിൽ (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    തിരികെ r + 1;
  }
  എങ്കിൽ (വലുപ്പം (T) <= 8)
  {
    /* "bithacks" */
    സ്ഥിരം uint64_t b[] = {0x2ULL, 0xCULL, 0xF0ULL, 0xFF00ULL, 0xFFFF0000ULL, 0xFFFFFFFF00000000ULL};
    സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ S[] = {1, 2, 4, 8, 16, 32};
    unsigned പൂർണ്ണസംഖ്യ r = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 5; i >= 0; i--)
      എങ്കിൽ (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    തിരികെ r + 1;
  }
  എങ്കിൽ (വലുപ്പം (T) == 16)
  {
    unsigned പൂർണ്ണസംഖ്യ shift = 64;
    തിരികെ (v >> shift) ? hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift :
			  hb_bit_storage<uint64_t> ((uint64_t) v);
  }

  പ്രതിജ്ഞാ (0);
  തിരികെ 0; /* Shut up stupid compiler. */
}

/* Returns the number of zero bits in the least significant side of v */
template <typename T>
സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
hb_ctz (T v)
{
  എങ്കിൽ (unlikely (!v)) തിരികെ 8 * വലുപ്പം (T);

#എങ്കിൽ hb_has_builtin(__builtin_ctz)
  എങ്കിൽ (വലുപ്പം (T) <= വലുപ്പം (unsigned പൂർണ്ണസംഖ്യ))
    തിരികെ __builtin_ctz (v);
#endif

#എങ്കിൽ hb_has_builtin(__builtin_ctzl)
  എങ്കിൽ (വലുപ്പം (T) <= വലുപ്പം (unsigned long))
    തിരികെ __builtin_ctzl (v);
#endif

#എങ്കിൽ hb_has_builtin(__builtin_ctzll)
  എങ്കിൽ (വലുപ്പം (T) <= വലുപ്പം (unsigned long long))
    തിരികെ __builtin_ctzll (v);
#endif

#എങ്കിൽ (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  എങ്കിൽ (വലുപ്പം (T) <= വലുപ്പം (unsigned പൂർണ്ണസംഖ്യ))
  {
    unsigned long where;
    _BitScanForward (&where, v);
    തിരികെ where;
  }
# എങ്കിൽ defined(_WIN64)
  എങ്കിൽ (വലുപ്പം (T) <= 8)
  {
    unsigned long where;
    _BitScanForward64 (&where, v);
    തിരികെ where;
  }
# endif
#endif

  എങ്കിൽ (വലുപ്പം (T) <= 4)
  {
    /* "bithacks" */
    unsigned പൂർണ്ണസംഖ്യ c = 32;
    v &= - (int32_t) v;
    എങ്കിൽ (v) c--;
    എങ്കിൽ (v & 0x0000FFFF) c -= 16;
    എങ്കിൽ (v & 0x00FF00FF) c -= 8;
    എങ്കിൽ (v & 0x0F0F0F0F) c -= 4;
    എങ്കിൽ (v & 0x33333333) c -= 2;
    എങ്കിൽ (v & 0x55555555) c -= 1;
    തിരികെ c;
  }
  എങ്കിൽ (വലുപ്പം (T) <= 8)
  {
    /* "bithacks" */
    unsigned പൂർണ്ണസംഖ്യ c = 64;
    v &= - (int64_t) (v);
    എങ്കിൽ (v) c--;
    എങ്കിൽ (v & 0x00000000FFFFFFFFULL) c -= 32;
    എങ്കിൽ (v & 0x0000FFFF0000FFFFULL) c -= 16;
    എങ്കിൽ (v & 0x00FF00FF00FF00FFULL) c -= 8;
    എങ്കിൽ (v & 0x0F0F0F0F0F0F0F0FULL) c -= 4;
    എങ്കിൽ (v & 0x3333333333333333ULL) c -= 2;
    എങ്കിൽ (v & 0x5555555555555555ULL) c -= 1;
    തിരികെ c;
  }
  എങ്കിൽ (വലുപ്പം (T) == 16)
  {
    unsigned പൂർണ്ണസംഖ്യ shift = 64;
    തിരികെ (uint64_t) v ? hb_bit_storage<uint64_t> ((uint64_t) v) :
			  hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift;
  }

  പ്രതിജ്ഞാ (0);
  തിരികെ 0; /* Shut up stupid compiler. */
}


/*
 * Tiny stuff.
 */

/* ASCII tag/character handling */
സ്ഥിതം വരിയിൽ തർക്കം ISALPHA (unsigned അക്ഷരം c)
{ തിരികെ (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
സ്ഥിതം വരിയിൽ തർക്കം ISALNUM (unsigned അക്ഷരം c)
{ തിരികെ (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }
സ്ഥിതം വരിയിൽ തർക്കം ISSPACE (unsigned അക്ഷരം c)
{ തിരികെ c == ' ' || c =='\f'|| c =='\n'|| c =='\r'|| c =='\t'|| c =='\v'; }
സ്ഥിതം വരിയിൽ unsigned അക്ഷരം TOUPPER (unsigned അക്ഷരം c)
{ തിരികെ (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c; }
സ്ഥിതം വരിയിൽ unsigned അക്ഷരം TOLOWER (unsigned അക്ഷരം c)
{ തിരികെ (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }
സ്ഥിതം വരിയിൽ തർക്കം ISHEX (unsigned അക്ഷരം c)
{ തിരികെ (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
സ്ഥിതം വരിയിൽ unsigned അക്ഷരം TOHEX (uint8_t c)
{ തിരികെ (c & 0xF) <= 9 ? (c & 0xF) + '0' : (c & 0xF) + 'a' - 10; }
സ്ഥിതം വരിയിൽ uint8_t FROMHEX (unsigned അക്ഷരം c)
{ തിരികെ (c >= '0' && c <= '9') ? c - '0' : TOLOWER (c) - 'a' + 10; }

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ DIV_CEIL (സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ a, unsigned പൂർണ്ണസംഖ്യ b)
{ തിരികെ (a + (b - 1)) / b; }


#undef  ARRAY_LENGTH
template <typename Type, unsigned പൂർണ്ണസംഖ്യ n>
സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ ARRAY_LENGTH (സ്ഥിരം Type (&)[n]) { തിരികെ n; }
/* A const version, but does not detect erratically being called on pointers. */
#define ARRAY_LENGTH_CONST(__array) ((signed പൂർണ്ണസംഖ്യ) (വലുപ്പം (__array) / വലുപ്പം (__array[0])))


സ്ഥിതം വരിയിൽ ശൂന്യം *
hb_memcpy (ശൂന്യം *__restrict dst, സ്ഥിരം ശൂന്യം *__restrict src, size_t len)
{
  /* It's illegal to pass 0 as size to memcpy. */
  എങ്കിൽ (unlikely (!len)) തിരികെ dst;
  തിരികെ memcpy (dst, src, len);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
hb_memcmp (സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b, unsigned പൂർണ്ണസംഖ്യ len)
{
  /* It's illegal to pass NULL to memcmp(), even if len is zero.
   * So, wrap it.
   * https://sourceware.org/bugzilla/show_bug.cgi?id=23878 */
  എങ്കിൽ (unlikely (!len)) തിരികെ 0;
  തിരികെ memcmp (a, b, len);
}

സ്ഥിതം വരിയിൽ ശൂന്യം *
hb_memset (ശൂന്യം *s, പൂർണ്ണസംഖ്യ c, unsigned പൂർണ്ണസംഖ്യ n)
{
  /* It's illegal to pass NULL to memset(), even if n is zero. */
  എങ്കിൽ (unlikely (!n)) തിരികെ s;
  തിരികെ memset (s, c, n);
}

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
hb_ceil_to_4 (unsigned പൂർണ്ണസംഖ്യ v)
{
  തിരികെ ((v - 1) | 3) + 1;
}

template <typename T> സ്ഥിതം വരിയിൽ തർക്കം
hb_in_range (T u, T lo, T hi)
{
  static_assert (!std::is_signed<T>::value, "");

  /* The casts below are important as if T is smaller than int,
   * the subtract results will become a signed int! */
  തിരികെ (T)(u - lo) <= (T)(hi - lo);
}
template <typename T> സ്ഥിതം വരിയിൽ തർക്കം
hb_in_ranges (T u, T lo1, T hi1)
{
  തിരികെ hb_in_range (u, lo1, hi1);
}
template <typename T, typename ...Ts> സ്ഥിതം വരിയിൽ തർക്കം
hb_in_ranges (T u, T lo1, T hi1, Ts... ds)
{
  തിരികെ hb_in_range<T> (u, lo1, hi1) || hb_in_ranges<T> (u, ds...);
}


/*
 * Overflow checking.
 */

സ്ഥിതം വരിയിൽ തർക്കം
hb_unsigned_mul_overflows (unsigned പൂർണ്ണസംഖ്യ എണ്ണം, unsigned പൂർണ്ണസംഖ്യ size, unsigned *result = nullptr)
{
#എങ്കിൽ hb_has_builtin(__builtin_mul_overflow)
  unsigned അട്ടി_result;
  എങ്കിൽ (!result)
    result = &അട്ടി_result;
  തിരികെ __builtin_mul_overflow (എണ്ണം, size, result);
#endif

  എങ്കിൽ (result)
    *result = എണ്ണം * size;
  തിരികെ (size > 0) && (എണ്ണം >= ((unsigned പൂർണ്ണസംഖ്യ) -1) / size);
}

സ്ഥിതം വരിയിൽ തർക്കം
hb_unsigned_add_overflows (unsigned പൂർണ്ണസംഖ്യ a, unsigned പൂർണ്ണസംഖ്യ b, unsigned *result = nullptr)
{
#എങ്കിൽ hb_has_builtin(__builtin_add_overflow)
  unsigned അട്ടി_result;
  എങ്കിൽ (!result)
    result = &അട്ടി_result;
  തിരികെ __builtin_add_overflow (a, b, result);
#endif

  എങ്കിൽ (result)
    *result = a + b;
  തിരികെ b > (unsigned പൂർണ്ണസംഖ്യ) -1 - a;
}

/* Saturating arithmetic on size_t.  On platforms where size_t is wider than
 * unsigned int (i.e. 64-bit), the inputs to get_size()-style computations
 * (counts and static_sizes which are at most 32-bit) cannot overflow size_t,
 * so these reduce to plain arithmetic.  On 32-bit platforms (size_t is
 * unsigned int), they saturate to SIZE_MAX so sanitize/serialize callers
 * naturally reject the resulting size as out-of-range. */

സ്ഥിതം വരിയിൽ size_t
hb_unsigned_mul_saturate (size_t a, size_t b)
{
  എങ്കിൽ (വലുപ്പം (size_t) > വലുപ്പം (unsigned പൂർണ്ണസംഖ്യ))
    തിരികെ a * b;
#എങ്കിൽ hb_has_builtin(__builtin_mul_overflow)
  size_t result;
  എങ്കിൽ (__builtin_mul_overflow (a, b, &result))
    തിരികെ (size_t) -1;
  തിരികെ result;
#അല്ലെങ്കിൽ
  എങ്കിൽ (b > 0 && a > ((size_t) -1) / b) തിരികെ (size_t) -1;
  തിരികെ a * b;
#endif
}

സ്ഥിതം വരിയിൽ size_t
hb_unsigned_add_saturate (size_t a, size_t b)
{
  എങ്കിൽ (വലുപ്പം (size_t) > വലുപ്പം (unsigned പൂർണ്ണസംഖ്യ))
    തിരികെ a + b;
#എങ്കിൽ hb_has_builtin(__builtin_add_overflow)
  size_t result;
  എങ്കിൽ (__builtin_add_overflow (a, b, &result))
    തിരികെ (size_t) -1;
  തിരികെ result;
#അല്ലെങ്കിൽ
  എങ്കിൽ (b > ((size_t) -1) - a) തിരികെ (size_t) -1;
  തിരികെ a + b;
#endif
}

/* Variadic forms: fold left across all arguments. */
template <typename ...Ts>
സ്ഥിതം വരിയിൽ size_t
hb_unsigned_mul_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ തിരികെ hb_unsigned_mul_saturate (hb_unsigned_mul_saturate (a, b), c, rest...); }

template <typename ...Ts>
സ്ഥിതം വരിയിൽ size_t
hb_unsigned_add_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ തിരികെ hb_unsigned_add_saturate (hb_unsigned_add_saturate (a, b), c, rest...); }

/* Saturating mul-add: a * b + c.  Covers the dominant get_size() pattern
 * `count * static_size + min_size`. */
സ്ഥിതം വരിയിൽ size_t
hb_unsigned_mul_add_saturate (size_t a, size_t b, size_t c)
{ തിരികെ hb_unsigned_add_saturate (hb_unsigned_mul_saturate (a, b), c); }


/*
 * Sort and search.
 */

template <typename K, typename V, typename ...Ts>
സ്ഥിതം പൂർണ്ണസംഖ്യ
_hb_cmp_method (സ്ഥിരം ശൂന്യം *pkey, സ്ഥിരം ശൂന്യം *pval, Ts... ds)
{
  സ്ഥിരം K& key = * (സ്ഥിരം K*) pkey;
  സ്ഥിരം V& val = * (സ്ഥിരം V*) pval;

  തിരികെ val.cmp (key, ds...);
}

template <typename K, typename V>
സ്ഥിതം പൂർണ്ണസംഖ്യ
_hb_cmp_operator (സ്ഥിരം ശൂന്യം *pkey, സ്ഥിരം ശൂന്യം *pval)
{
  സ്ഥിരം K& key = * (സ്ഥിരം K*) pkey;
  സ്ഥിരം V& val = * (സ്ഥിരം V*) pval;

  എങ്കിൽ (key < val) തിരികെ -1;
  എങ്കിൽ (key > val) തിരികെ  1;
  തിരികെ 0;
}

template <typename V, typename K, typename ...Ts>
HB_HOT
സ്ഥിതം വരിയിൽ തർക്കം
hb_bsearch_impl (unsigned *pos, /* Out */
		 സ്ഥിരം K& key,
		 V* base, size_t nmemb, size_t stride,
		 പൂർണ്ണസംഖ്യ (*compar)(സ്ഥിരം ശൂന്യം *താക്കോൽ_3, സ്ഥിരം ശൂന്യം *_item, Ts... _ds),
		 Ts... ds)
{
  /* This is our *only* bsearch implementation. */

  പൂർണ്ണസംഖ്യ min = 0, max = (പൂർണ്ണസംഖ്യ) nmemb - 1;
  ആകുമ്പോൾ (min <= max)
  {
    പൂർണ്ണസംഖ്യ mid = ((unsigned പൂർണ്ണസംഖ്യ) min + (unsigned പൂർണ്ണസംഖ്യ) max) / 2;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    V* p = (V*) (((സ്ഥിരം അക്ഷരം *) base) + (mid * stride));
#pragma GCC diagnostic എടുക്കുക
    പൂർണ്ണസംഖ്യ c = compar ((സ്ഥിരം ശൂന്യം *) std::addressof (key), (സ്ഥിരം ശൂന്യം *) p, ds...);
    എങ്കിൽ (c < 0)
      max = mid - 1;
    അല്ലെങ്കിൽ എങ്കിൽ (c > 0)
      min = mid + 1;
    അല്ലെങ്കിൽ
    {
      *pos = mid;
      തിരികെ സത്യം;
    }
  }
  *pos = min;
  തിരികെ അസത്യം;
}

template <typename V, typename K>
സ്ഥിതം വരിയിൽ V*
hb_bsearch (സ്ഥിരം K& key, V* base,
	    size_t nmemb, size_t stride = വലുപ്പം (V),
	    പൂർണ്ണസംഖ്യ (*compar)(സ്ഥിരം ശൂന്യം *താക്കോൽ_3, സ്ഥിരം ശൂന്യം *_item) = _hb_cmp_method<K, V>)
{
  unsigned pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  തിരികെ hb_bsearch_impl (&pos, key, base, nmemb, stride, compar) ?
	 (V*) (((സ്ഥിരം അക്ഷരം *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic എടുക്കുക
}
template <typename V, typename K, typename ...Ts>
സ്ഥിതം വരിയിൽ V*
hb_bsearch (സ്ഥിരം K& key, V* base,
	    size_t nmemb, size_t stride,
	    പൂർണ്ണസംഖ്യ (*compar)(സ്ഥിരം ശൂന്യം *താക്കോൽ_3, സ്ഥിരം ശൂന്യം *_item, Ts... _ds),
	    Ts... ds)
{
  unsigned pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  തിരികെ hb_bsearch_impl (&pos, key, base, nmemb, stride, compar, ds...) ?
	 (V*) (((സ്ഥിരം അക്ഷരം *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic എടുക്കുക
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
സ്ഥിതം വരിയിൽ ശൂന്യം
hb_qsort_loop (T *base, size_t nel, Compar compar)
{
  ആകുമ്പോൾ (nel > 24)
  {
    T *last = base + nel - 1;
    T *mid = base + nel / 2;

    /* Median-of-three pivot, parked at last-1. */
    എങ്കിൽ (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    എങ്കിൽ (compar (*mid, *last) > 0)
    {
      hb_swap (*mid, *last);
      എങ്കിൽ (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    }
    hb_swap (*mid, *(last - 1));
    T &pivot = *(last - 1);

    /* Two-way Hoare partition.  Inner loops are unguarded:
     * median-of-three left *base <= pivot and *last >= pivot,
     * which act as sentinels. */
    T *i = base, *j = last - 1;
    ആകുമ്പോൾ (സത്യം)
    {
      ആകുമ്പോൾ (compar (*++i, pivot) < 0) {}
      ആകുമ്പോൾ (compar (*--j, pivot) > 0) {}
      എങ്കിൽ (i >= j) നിർത്തുക;
      hb_swap (*i, *j);
    }
    hb_swap (*i, *(last - 1));

    /* Recurse on smaller side, loop on larger — bounds stack
     * depth at O(log n). */
    size_t left  = (size_t) (i - base);
    size_t വലത് = nel - left - 1;
    എങ്കിൽ (left < വലത്)
    {
      hb_qsort_loop (base, left, compar);
      base = i + 1;
      nel  = വലത്;
    }
    അല്ലെങ്കിൽ
    {
      hb_qsort_loop (i + 1, വലത്, compar);
      nel  = left;
    }
  }
}

template <typename T, typename Compar>
സ്ഥിതം വരിയിൽ ശൂന്യം
hb_qsort_inline (T *base, size_t nel, Compar compar)
{
  hb_qsort_loop (base, nel, compar);

  /* Single final insertion sort over the whole array.  After
   * the partitioning loop, every element is within the threshold
   * of its sorted position, so this pass is O(n * threshold). */
  T *end = base + nel;
  വേണ്ടി (T *pi = base + 1; pi < end; pi++)
    വേണ്ടി (T *pj = pi; pj > base && compar (pj[-1], pj[0]) > 0; pj--)
      hb_swap (pj[-1], pj[0]);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
hb_qsort (ശൂന്യം *base, size_t nel, size_t width,
	  പൂർണ്ണസംഖ്യ (*compar)(സ്ഥിരം ശൂന്യം *_a, സ്ഥിരം ശൂന്യം *_b))
{
  qsort (base, nel, width, compar);
}




template <typename T, typename T2, typename T3 = പൂർണ്ണസംഖ്യ> സ്ഥിതം വരിയിൽ ശൂന്യം
hb_stable_sort (T *array, unsigned പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ(*compar)(സ്ഥിരം T2 *, സ്ഥിരം T2 *), T3 *array2 = nullptr)
{
  static_assert (hb_is_trivially_copy_assignable (T), "");
  static_assert (hb_is_trivially_copy_assignable (T3), "");

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < len; i++)
  {
    unsigned പൂർണ്ണസംഖ്യ j = i;
    ആകുമ്പോൾ (j && compar (&array[j - 1], &array[i]) > 0)
      j--;
    എങ്കിൽ (i == j)
      തുടരുക;
    /* Move item i to occupy place for item j, shift what's in between. */
    {
      T t = array[i];
      memmove (&array[j + 1], &array[j], (i - j) * വലുപ്പം (T));
      array[j] = t;
    }
    എങ്കിൽ (array2)
    {
      T3 t = array2[i];
      memmove (&array2[j + 1], &array2[j], (i - j) * വലുപ്പം (T3));
      array2[j] = t;
    }
  }
}

സ്ഥിതം വരിയിൽ hb_bool_t
hb_codepoint_parse (സ്ഥിരം അക്ഷരം *s, unsigned പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ base, hb_codepoint_t *out)
{
  unsigned പൂർണ്ണസംഖ്യ v;
  സ്ഥിരം അക്ഷരം *p = s;
  സ്ഥിരം അക്ഷരം *end = p + len;
  എങ്കിൽ (unlikely (!hb_parse_uint (&p, end, &v, സത്യം/* whole buffer */, base)))
    തിരികെ അസത്യം;

  *out = v;
  തിരികെ സത്യം;
}


/* Operators. */

ഘടന
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (സ്ഥിരം T &a, സ്ഥിരം T &b) സ്ഥിരം HB_AUTO_RETURN (a & b)
}
HB_FUNCOBJ (hb_bitwise_and);
ഘടന
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (സ്ഥിരം T &a, സ്ഥിരം T &b) സ്ഥിരം HB_AUTO_RETURN (a | b)
}
HB_FUNCOBJ (hb_bitwise_or);
ഘടന
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (സ്ഥിരം T &a, സ്ഥിരം T &b) സ്ഥിരം HB_AUTO_RETURN (a ^ b)
}
HB_FUNCOBJ (hb_bitwise_xor);
ഘടന
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (സ്ഥിരം T &a, സ്ഥിരം T &b) സ്ഥിരം HB_AUTO_RETURN (~a & b)
}
HB_FUNCOBJ (hb_bitwise_lt);
ഘടന
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (സ്ഥിരം T &a, സ്ഥിരം T &b) സ്ഥിരം HB_AUTO_RETURN (a & ~b)
}
HB_FUNCOBJ (hb_bitwise_gt); // aka sub
ഘടന
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (സ്ഥിരം T &a, സ്ഥിരം T &b) സ്ഥിരം HB_AUTO_RETURN (~a | b)
}
HB_FUNCOBJ (hb_bitwise_le);
ഘടന
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (സ്ഥിരം T &a, സ്ഥിരം T &b) സ്ഥിരം HB_AUTO_RETURN (a | ~b)
}
HB_FUNCOBJ (hb_bitwise_ge);
ഘടന
{
  template <typename T> constexpr auto
  operator () (സ്ഥിരം T &a) സ്ഥിരം HB_AUTO_RETURN (~a)
}
HB_FUNCOBJ (hb_bitwise_neg);

ഘടന
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (സ്ഥിരം T &a, സ്ഥിരം T2 &b) സ്ഥിരം HB_AUTO_RETURN (a + b)
}
HB_FUNCOBJ (hb_add);
ഘടന
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (സ്ഥിരം T &a, സ്ഥിരം T2 &b) സ്ഥിരം HB_AUTO_RETURN (a - b)
}
HB_FUNCOBJ (hb_sub);
ഘടന
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (സ്ഥിരം T &a, സ്ഥിരം T2 &b) സ്ഥിരം HB_AUTO_RETURN (b - a)
}
HB_FUNCOBJ (hb_rsub);
ഘടന
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (സ്ഥിരം T &a, സ്ഥിരം T2 &b) സ്ഥിരം HB_AUTO_RETURN (a * b)
}
HB_FUNCOBJ (hb_mul);
ഘടന
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (സ്ഥിരം T &a, സ്ഥിരം T2 &b) സ്ഥിരം HB_AUTO_RETURN (a / b)
}
HB_FUNCOBJ (hb_div);
ഘടന
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (സ്ഥിരം T &a, സ്ഥിരം T2 &b) സ്ഥിരം HB_AUTO_RETURN (a % b)
}
HB_FUNCOBJ (hb_mod);
ഘടന
{
  template <typename T> constexpr auto
  operator () (സ്ഥിരം T &a) സ്ഥിരം HB_AUTO_RETURN (+a)
}
HB_FUNCOBJ (hb_pos);
ഘടന
{
  template <typename T> constexpr auto
  operator () (സ്ഥിരം T &a) സ്ഥിരം HB_AUTO_RETURN (-a)
}
HB_FUNCOBJ (hb_neg);
ഘടന
{
  template <typename T> constexpr auto
  operator () (T &a) സ്ഥിരം HB_AUTO_RETURN (++a)
}
HB_FUNCOBJ (hb_inc);
ഘടന
{
  template <typename T> constexpr auto
  operator () (T &a) സ്ഥിരം HB_AUTO_RETURN (--a)
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
ഇരട്ട solve_itp (func_t f,
		  ഇരട്ട a, ഇരട്ട b,
		  ഇരട്ട epsilon,
		  ഇരട്ട min_y, ഇരട്ട max_y,
		  ഇരട്ട &ya, ഇരട്ട &yb, ഇരട്ട &y)
{
  // Guard against degenerate interval
  എങ്കിൽ (b - a <= 0.0)
  {
    y = ya;
    തിരികെ a;
  }

  unsigned n1_2 = (unsigned) (hb_max (ceil (log2 ((b - a) / epsilon)) - 1.0, 0.0));
  സ്ഥിരം unsigned n0 = 1; // Hardwired
  സ്ഥിരം ഇരട്ട k1 = 0.2 / (b - a); // Hardwired.
  unsigned nmax = n0 + n1_2;
  ഇരട്ട scaled_epsilon = epsilon * ഇരട്ട (1llu << nmax);
  ഇരട്ട _2_epsilon = 2.0 * epsilon;
  ആകുമ്പോൾ (b - a > _2_epsilon)
  {
    ഇരട്ട x1_2 = 0.5 * (a + b);
    ഇരട്ട r = scaled_epsilon - 0.5 * (b - a);
    // Guard against yb == ya to prevent division by zero
    ഇരട്ട xf = (yb != ya) ? (yb * a - ya * b) / (yb - ya) : x1_2;
    ഇരട്ട sigma = x1_2 - xf;
    ഇരട്ട b_a = b - a;
    // This has k2 = 2 hardwired for efficiency.
    ഇരട്ട b_a_k2 = b_a * b_a;
    ഇരട്ട delta = k1 * b_a_k2;
    പൂർണ്ണസംഖ്യ sigma_sign = sigma >= 0 ? +1 : -1;
    ഇരട്ട xt = delta <= fabs (x1_2 - xf) ? xf + delta * sigma_sign : x1_2;
    ഇരട്ട xitp = fabs (xt - x1_2) <= r ? xt : x1_2 - r * sigma_sign;
    ഇരട്ട yitp = f (xitp);
    എങ്കിൽ (yitp > max_y)
    {
      b = xitp;
      yb = yitp;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (yitp < min_y)
    {
      a = xitp;
      ya = yitp;
    }
    അല്ലെങ്കിൽ
    {
      y = yitp;
      തിരികെ xitp;
    }
    scaled_epsilon *= 0.5;
  }
  തിരികെ 0.5 * (a + b);
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
ഘടന hb_scope_guard_t
{
  explicit hb_scope_guard_t (F &&f) : f (std::move (f)), active (സത്യം) {}
  hb_scope_guard_t (hb_scope_guard_t &&o) noexcept
    : f (std::move (o.f)), active (o.active) { o.active = അസത്യം; }
  hb_scope_guard_t (സ്ഥിരം hb_scope_guard_t &) = delete;
  hb_scope_guard_t &operator= (സ്ഥിരം hb_scope_guard_t &) = delete;
  hb_scope_guard_t &operator= (hb_scope_guard_t &&) = delete;
  ~hb_scope_guard_t () { എങ്കിൽ (active) f (); }

  /* Release: dismiss the guard so the cleanup does NOT run.  Use
   * when transferring ownership out of the scope. */
  ശൂന്യം release () { active = അസത്യം; }

  സ്വകാര്യ:
  F f;
  തർക്കം active;
};

template <typename F>
സ്ഥിതം വരിയിൽ hb_scope_guard_t<F> hb_make_scope_guard (F &&f)
{ തിരികെ hb_scope_guard_t<F> (std::forward<F> (f)); }

#define HB_SCOPE_GUARD_NAME_(വരി) hb_scope_guard_##line
#define HB_SCOPE_GUARD_NAME(വരി) HB_SCOPE_GUARD_NAME_(വരി)
#define HB_SCOPE_GUARD(stmt) \
  auto HB_SCOPE_GUARD_NAME(__LINE__) = \
    hb_make_scope_guard ([&]() { stmt; }); \
  (ശൂന്യം) HB_SCOPE_GUARD_NAME(__LINE__)


#endif /* HB_ALGS_HH */
