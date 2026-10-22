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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-meta.hh"
#अन्तर्भाव "hb-null.hh"
#अन्तर्भाव "hb-number.hh"

#अन्तर्भाव <algorithm>
#अन्तर्भाव <initializer_list>
#अन्तर्भाव <functional>
#अन्तर्भाव <new>

स्थिर HB_ALWAYS_INLINE शून्य *
hb_malloc2 (माप_प्रकार nmemb, माप_प्रकार size)
{
  जर (size && nmemb > SIZE_MAX / size) परत nullptr;
  परत hb_malloc (nmemb * size);
}
स्थिर HB_ALWAYS_INLINE शून्य *
hb_realloc2 (शून्य *ptr, माप_प्रकार nmemb, माप_प्रकार size)
{
  जर (size && nmemb > SIZE_MAX / size) परत nullptr;
  परत hb_realloc (ptr, nmemb * size);
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
	बाह्य "C++" { \
	  स्थिर ओळीत constexpr T चालक | (T l, T r) { परत T ((अचिन्हित) l | (अचिन्हित) r); } \
	  स्थिर ओळीत constexpr T चालक & (T l, T r) { परत T ((अचिन्हित) l & (अचिन्हित) r); } \
	  स्थिर ओळीत constexpr T चालक ^ (T l, T r) { परत T ((अचिन्हित) l ^ (अचिन्हित) r); } \
	  स्थिर ओळीत constexpr अचिन्हित चालक ~ (T r) { परत (~(अचिन्हित) r); } \
	  स्थिर ओळीत T& चालक |= (T &l, T r) { l = l | r; परत l; } \
	  स्थिर ओळीत T& चालक &= (T& l, T r) { l = l & r; परत l; } \
	  स्थिर ओळीत T& चालक ^= (T& l, T r) { l = l ^ r; परत l; } \
	} \
	static_assert (सत्य, "")

/* Useful for set-operations on small enums.
 * For example, for testing "x ∈ {x1, x2, x3}" use:
 * (FLAG_UNSAFE(x) & (FLAG(x1) | FLAG(x2) | FLAG(x3)))
 */
#define FLAG(x) (static_assert_अभिव्यक्ती ((अचिन्हित)(x) < 32) + (((uint32_t) 1U) << (अचिन्हित)(x)))
#define FLAG_UNSAFE(x) ((अचिन्हित)(x) < 32 ? (((uint32_t) 1U) << (अचिन्हित)(x)) : 0)
#define FLAG_RANGE(x,y) (static_assert_अभिव्यक्ती ((x) < (y)) + FLAG(y+1) - FLAG(x))
#define FLAG64(x) (static_assert_अभिव्यक्ती ((अचिन्हित)(x) < 64) + (((uint64_t) 1ULL) << (अचिन्हित)(x)))
#define FLAG64_UNSAFE(x) ((अचिन्हित)(x) < 64 ? (((uint64_t) 1ULL) << (अचिन्हित)(x)) : 0)


/*
 * Fixed-endian integers / floats.
 */


/* Endian swap, used in Windows related backends */
स्थिर ओळीत constexpr uint16_t hb_uint16_swap (uint16_t v)
{ परत (v >> 8) | (v << 8); }
स्थिर ओळीत constexpr uint32_t hb_uint32_swap (uint32_t v)
{ परत (hb_uint16_swap (v) << 16) | hb_uint16_swap (v >> 16); }

ढाँचा <typename Type>
संरचना __attribute__((packed)) hb_packed_t
{
  hb_packed_t () = पूर्वनिर्धारित;
  constexpr hb_packed_t (Type V) : v (V) {}
  चालक Type () स्थिरांक { परत v; }
  hb_packed_t & चालक = (Type V) { v = V; परत *हे; }

  खाजगी:
  Type v;
};

#ifndef HB_FAST_अंक_ACCESS

#जर defined(__OPTIMIZE__) && \
    defined(__BYTE_ORDER) && \
    (__BYTE_ORDER == __BIG_ENDIAN || \
     (__BYTE_ORDER == __LITTLE_ENDIAN && \
      hb_has_builtin(__builtin_bswap16) && \
      hb_has_builtin(__builtin_bswap32) && \
      hb_has_builtin(__builtin_bswap64)))
#define HB_FAST_अंक_ACCESS 1
#अन्यथा
#define HB_FAST_अंक_ACCESS 0
#endif

// https://github.com/harfbuzz/harfbuzz/issues/5456
#जर defined(__GNUC__) && !defined(__clang__) && (__GNUC__ <= 12)
#undef HB_FAST_अंक_ACCESS
#define HB_FAST_अंक_ACCESS 0
#endif

#endif

ढाँचा <तार्किक BE, typename Type, पूर्णांक Bytes = आकार (Type)>
संरचना HBInt;
ढाँचा <तार्किक BE, typename Type>
संरचना HBInt<BE, Type, 1>
{
  सार्वजनिक:
  HBInt () = पूर्वनिर्धारित;
  constexpr HBInt (Type V) : v {uint8_t (V)} {}
  constexpr चालक Type () स्थिरांक { परत v; }
  खाजगी: uint8_t v;
};
ढाँचा <तार्किक BE, typename Type>
संरचना HBInt<BE, Type, 2>
{
  सार्वजनिक:
  HBInt () = पूर्वनिर्धारित;

  HBInt (Type V)
#जर HB_FAST_अंक_ACCESS
  {
    जर (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint16_t> *) v) = V;
    अन्यथा
      *((hb_packed_t<uint16_t> *) v) = __builtin_bswap16 (V);
  }
#अन्यथा
    : v {BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >>  8) & 0xFF)} {}
#endif

  constexpr चालक Type () स्थिरांक
  {
#जर HB_FAST_अंक_ACCESS
    परत (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint16_t) *((स्थिरांक hb_packed_t<uint16_t> *) v)
    :
      __builtin_bswap16 ((uint16_t) *((स्थिरांक hb_packed_t<uint16_t> *) v))
    ;
#अन्यथा
    परत (BE ? (v[0] <<  8) : (v[0]      ))
	 + (BE ? (v[1]      ) : (v[1] <<  8));
#endif
  }
  खाजगी: uint8_t v[2];
};
ढाँचा <तार्किक BE, typename Type>
संरचना HBInt<BE, Type, 3>
{
  static_assert (!std::is_signed<Type>::value, "");
  सार्वजनिक:
  HBInt () = पूर्वनिर्धारित;
  constexpr HBInt (Type V) : v {BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
				BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
				BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V      ) & 0xFF)} {}

  constexpr चालक Type () स्थिरांक { परत (BE ? (v[0] << 16) : (v[0]      ))
					  + (BE ? (v[1] <<  8) : (v[1] <<  8))
					  + (BE ? (v[2]      ) : (v[2] << 16)); }
  खाजगी: uint8_t v[3];
};
ढाँचा <तार्किक BE, typename Type>
संरचना HBInt<BE, Type, 4>
{
  ढाँचा <तार्किक, typename, पूर्णांक>
  मित्र संरचना HBFloat;

  सार्वजनिक:
  HBInt () = पूर्वनिर्धारित;

  HBInt (Type V)
#जर HB_FAST_अंक_ACCESS
  {
    जर (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint32_t> *) v) = V;
    अन्यथा
      *((hb_packed_t<uint32_t> *) v) = __builtin_bswap32 (V);
  }
#अन्यथा
    : v {BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 24) & 0xFF)} {}
#endif

  constexpr चालक Type () स्थिरांक {
#जर HB_FAST_अंक_ACCESS
    परत (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint32_t) *((स्थिरांक hb_packed_t<uint32_t> *) v)
    :
      __builtin_bswap32 ((uint32_t) *((स्थिरांक hb_packed_t<uint32_t> *) v))
    ;
#अन्यथा
    परत (BE ? (uint32_t (v[0]) << 24) : (uint32_t (v[0])      ))
	 + (BE ? (uint32_t (v[1]) << 16) : (uint32_t (v[1]) <<  8))
	 + (BE ? (uint32_t (v[2]) <<  8) : (uint32_t (v[2]) << 16))
	 + (BE ? (uint32_t (v[3])      ) : (uint32_t (v[3]) << 24));
#endif
  }
  खाजगी: uint8_t v[4];
};
ढाँचा <तार्किक BE, typename Type>
संरचना HBInt<BE, Type, 8>
{
  ढाँचा <तार्किक, typename, पूर्णांक>
  मित्र संरचना HBFloat;

  सार्वजनिक:
  HBInt () = पूर्वनिर्धारित;

  HBInt (Type V)
#जर HB_FAST_अंक_ACCESS
  {
    जर (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint64_t> *) v) = V;
    अन्यथा
      *((hb_packed_t<uint64_t> *) v) = __builtin_bswap64 (V);
  }
#अन्यथा
    : v {BE ? uint8_t ((V >> 56) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 48) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >> 40) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V >> 32) & 0xFF) : uint8_t ((V >> 24) & 0xFF),
	 BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V >> 32) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 40) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 48) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 56) & 0xFF)} {}
#endif

  constexpr चालक Type () स्थिरांक {
#जर HB_FAST_अंक_ACCESS
    परत (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint64_t) *((स्थिरांक hb_packed_t<uint64_t> *) v)
    :
      __builtin_bswap64 ((uint64_t) *((स्थिरांक hb_packed_t<uint64_t> *) v))
    ;
#अन्यथा
    परत (BE ? (uint64_t (v[0]) << 56) : (uint64_t (v[0])      ))
	 + (BE ? (uint64_t (v[1]) << 48) : (uint64_t (v[1]) <<  8))
	 + (BE ? (uint64_t (v[2]) << 40) : (uint64_t (v[2]) << 16))
	 + (BE ? (uint64_t (v[3]) << 32) : (uint64_t (v[3]) << 24))
	 + (BE ? (uint64_t (v[4]) << 24) : (uint64_t (v[4]) << 32))
	 + (BE ? (uint64_t (v[5]) << 16) : (uint64_t (v[5]) << 40))
	 + (BE ? (uint64_t (v[6]) <<  8) : (uint64_t (v[6]) << 48))
	 + (BE ? (uint64_t (v[7])      ) : (uint64_t (v[7]) << 56));
#endif
  }
  खाजगी: uint8_t v[8];
};

/* Floats. */

ढाँचा <तार्किक BE, typename Type, पूर्णांक Bytes>
संरचना HBFloat
{
  using IntType = typename std::conditional<Bytes == 4, uint32_t, uint64_t>::type;

  सार्वजनिक:
  HBFloat () = पूर्वनिर्धारित;

  HBFloat (Type V)
  {
#जर HB_FAST_अंक_ACCESS
    {
	      जर (BE == (__BYTE_ORDER == __BIG_ENDIAN))
	      {
	        *((hb_packed_t<Type> *) v) = V;
	        परत;
	      }
	    }
#endif

    संघ {
      hb_packed_t<Type> f;
      hb_packed_t<IntType> i;
    } u = {{V}};

    स्थिरांक HBInt<BE, IntType> I = (IntType) u.i;
    साठी (अचिन्हित i = 0; i < Bytes; i++)
      v[i] = I.v[i];
  }

  /* c++14 constexpr */ चालक Type () स्थिरांक
  {
#जर HB_FAST_अंक_ACCESS
	    {
	      जर (BE == (__BYTE_ORDER == __BIG_ENDIAN))
		परत (Type) *((स्थिरांक hb_packed_t<Type> *) v);
	    }
#endif

    HBInt<BE, IntType> I;
    साठी (अचिन्हित i = 0; i < Bytes; i++)
      I.v[i] = v[i];

    संघ {
      hb_packed_t<IntType> i;
      hb_packed_t<Type> f;
    } u = {{I}};

    परत (Type) u.f;
  }
  खाजगी: uint8_t v[Bytes];
};


/* We want our rounding towards +infinity. */
स्थिर ओळीत द्विगुण
_hb_roundf (द्विगुण x) { परत न्यूनमान (x + .5); }

स्थिर ओळीत दशांश
_hb_roundf (दशांश x) { परत न्यूनमान ((द्विगुण) x + .5); }

#define roundf(x) _hb_roundf(x)

स्थिर ओळीत शून्य
hb_sincos (दशांश rotation, दशांश &s, दशांश &c)
{
#ifdef HAVE_SINCOSF
  sincosf (rotation, &s, &c);
#अन्यथा
  c = cosf (rotation);
  s = sinf (rotation);
#endif
}
स्थिर ओळीत शून्य
hb_sincos (द्विगुण rotation, द्विगुण &s, द्विगुण &c)
{
#ifdef HAVE_SINCOS
  sincos (rotation, &s, &c);
#अन्यथा
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


संरचना
{
  /* Note.  This is dangerous in that if it's passed an rvalue, it returns rvalue-reference. */
  ढाँचा <typename T> constexpr स्वतः
  चालक () (T&& v) स्थिरांक HB_AUTO_RETURN ( std::forward<T> (v) )
}
HB_FUNCOBJ (hb_identity);
संरचना
{
  /* Like identity(), but only retains lvalue-references.  Rvalues are returned as rvalues. */
  ढाँचा <typename T> constexpr T&
  चालक () (T& v) स्थिरांक { परत v; }

  ढाँचा <typename T> constexpr hb_remove_reference<T>
  चालक () (T&& v) स्थिरांक { परत v; }
}
HB_FUNCOBJ (hb_lidentity);
संरचना
{
  /* Like identity(), but always returns rvalue. */
  ढाँचा <typename T> constexpr hb_remove_reference<T>
  चालक () (T&& v) स्थिरांक { परत v; }
}
HB_FUNCOBJ (hb_ridentity);

संरचना
{
  ढाँचा <typename T> constexpr तार्किक
  चालक () (T&& v) स्थिरांक { परत तार्किक (std::forward<T> (v)); }
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
			(शून्य) ((h) ^= (h) >> 23),		\
			(शून्य) ((h) *= 0x2127599bf4325c37ULL),	\
			(h) ^= (h) >> 47)

स्थिर ओळीत uint64_t fasthash64(स्थिरांक शून्य *buf, माप_प्रकार len, uint64_t seed)
{
	संरचना __attribute__((packed)) packed_uint64_t { uint64_t v; };
	स्थिरांक uint64_t    m = 0x880355f21e6d1965ULL;
	स्थिरांक packed_uint64_t *pos = (स्थिरांक packed_uint64_t *)buf;
	स्थिरांक packed_uint64_t *end = pos + (len / 8);
	स्थिरांक अचिन्हित अक्षर *pos2;
	uint64_t h = seed ^ (len * m);
	uint64_t v;

#ifndef HB_OPTIMIZE_SIZE
	जर (((uintptr_t) pos & 7) == 0)
	{
	  पर्यंत (pos != end)
	  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
	    v  = * (स्थिरांक uint64_t *) (pos++);
#pragma GCC diagnostic काढा
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}
	अन्यथा
#endif
	{
	  पर्यंत (pos != end)
	  {
	    v  = pos++->v;
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}

	pos2 = (स्थिरांक अचिन्हित अक्षर*)pos;
	v = 0;

	निवड (len & 7) {
	स्थिती 7: v ^= (uint64_t)pos2[6] << 48; HB_FALLTHROUGH;
	स्थिती 6: v ^= (uint64_t)pos2[5] << 40; HB_FALLTHROUGH;
	स्थिती 5: v ^= (uint64_t)pos2[4] << 32; HB_FALLTHROUGH;
	स्थिती 4: v ^= (uint64_t)pos2[3] << 24; HB_FALLTHROUGH;
	स्थिती 3: v ^= (uint64_t)pos2[2] << 16; HB_FALLTHROUGH;
	स्थिती 2: v ^= (uint64_t)pos2[1] <<  8; HB_FALLTHROUGH;
	स्थिती 1: v ^= (uint64_t)pos2[0];
		h ^= fasthash_mix(v);
		h *= m;
	}

	परत fasthash_mix(h);
}

स्थिर ओळीत uint32_t fasthash32(स्थिरांक शून्य *buf, माप_प्रकार len, uint32_t seed)
{
	// the following trick converts the 64-bit hashcode to Fermat
	// residue, which shall retain information from both the higher
	// and lower parts of hashcode.
        uint64_t h = fasthash64(buf, len, seed);
	परत h - (h >> 32);
}

संरचना
{
  खाजगी:

  ढाँचा <typename T> constexpr स्वतः
  impl (स्थिरांक T& v, hb_priority<2>) स्थिरांक HB_RETURN (uint32_t, hb_deref (v).hash ())

  // Horrible: std:hash() of integers seems to be identity in gcc / clang?!
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  //
  // For performance characteristics see:
  // https://github.com/harfbuzz/harfbuzz/pull/4228#issuecomment-1565079537
  ढाँचा <typename T,
	    hb_enable_if (std::is_integral<T>::value && आकार (T) <= आकार (uint32_t))> constexpr स्वतः
  impl (स्थिरांक T& v, hb_priority<1>) स्थिरांक HB_RETURN (uint32_t, (uint32_t) v * 2654435761u /* Knuh's multiplicative hash */)
  ढाँचा <typename T,
	    hb_enable_if (std::is_integral<T>::value && आकार (T) > आकार (uint32_t))> constexpr स्वतः
  impl (स्थिरांक T& v, hb_priority<1>) स्थिरांक HB_RETURN (uint32_t, (uint32_t) (v ^ (v >> 32)) * 2654435761u /* Knuth's multiplicative hash */)

  ढाँचा <typename T,
	    hb_enable_if (std::is_floating_point<T>::value)> constexpr स्वतः
  impl (स्थिरांक T& v, hb_priority<1>) स्थिरांक HB_RETURN (uint32_t, fasthash32 (std::addressof (v), आकार (T), 0xf437ffe6))

  ढाँचा <typename T> constexpr स्वतः
  impl (स्थिरांक T& v, hb_priority<0>) स्थिरांक HB_RETURN (uint32_t, std::hash<hb_decay<decltype (hb_deref (v))>>{} (hb_deref (v)))

  सार्वजनिक:

  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिरांक T& v) स्थिरांक HB_RETURN (uint32_t, impl (v, hb_prioritize))
}
HB_FUNCOBJ (hb_hash);


संरचना
{
  खाजगी:

  /* Pointer-to-member-function. */
  ढाँचा <typename Appl, typename T, typename ...Ts> स्वतः
  impl (Appl&& a, hb_priority<2>, T &&v, Ts&&... ds) स्थिरांक HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v)).*std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  /* Pointer-to-member. */
  ढाँचा <typename Appl, typename T> स्वतः
  impl (Appl&& a, hb_priority<1>, T &&v) स्थिरांक HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v))).*std::forward<Appl> (a))

  /* Operator(). */
  ढाँचा <typename Appl, typename ...Ts> स्वतः
  impl (Appl&& a, hb_priority<0>, Ts&&... ds) स्थिरांक HB_AUTO_RETURN
  (hb_deref (std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  सार्वजनिक:

  ढाँचा <typename Appl, typename ...Ts> स्वतः
  चालक () (Appl&& a, Ts&&... ds) स्थिरांक HB_AUTO_RETURN
  (
    impl (std::forward<Appl> (a),
	  hb_prioritize,
	  std::forward<Ts> (ds)...)
  )
}
HB_FUNCOBJ (hb_invoke);

ढाँचा <अचिन्हित Pos, typename Appl, typename V>
संरचना hb_partial_t
{
  hb_partial_t (Appl a, V v) : a (a), v (v) {}

  static_assert (Pos > 0, "");

  ढाँचा <typename ...Ts,
	    अचिन्हित P = Pos,
	    hb_enable_if (P == 1)> स्वतः
  चालक () (Ts&& ...ds) -> decltype (hb_invoke (hb_declval (Appl),
						   hb_declval (V),
						   hb_declval (Ts)...))
  {
    परत hb_invoke (std::forward<Appl> (a),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }
  ढाँचा <typename T0, typename ...Ts,
	    अचिन्हित P = Pos,
	    hb_enable_if (P == 2)> स्वतः
  चालक () (T0&& d0, Ts&& ...ds) -> decltype (hb_invoke (hb_declval (Appl),
							    hb_declval (T0),
							    hb_declval (V),
							    hb_declval (Ts)...))
  {
    परत hb_invoke (std::forward<Appl> (a),
		      std::forward<T0> (d0),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }

  खाजगी:
  hb_reference_wrapper<Appl> a;
  V v;
};
ढाँचा <अचिन्हित Pos=1, typename Appl, typename V>
स्वतः hb_partial (Appl&& a, V&& v) HB_AUTO_RETURN
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
  ढाँचा <typename _T> \
  decltype(स्वतः) चालक () (_T&& _v) स्थिरांक \
  { परत hb_partial<Pos> (हे, std::forward<_T> (_v)); } \
  static_assert (सत्य, "")
#अन्यथा
/* https://github.com/harfbuzz/harfbuzz/issues/1724 */
#define HB_PARTIALIZE(Pos) \
  ढाँचा <typename _T> \
  स्वतः चालक () (_T&& _v) स्थिरांक HB_AUTO_RETURN \
  (hb_partial<Pos> (+हे, std::forward<_T> (_v))) \
  static_assert (सत्य, "")
#endif


संरचना
{
  खाजगी:

  ढाँचा <typename Pred, typename Val> स्वतः
  impl (Pred&& p, Val &&v, hb_priority<1>) स्थिरांक HB_AUTO_RETURN
  (
    hb_deref (std::forward<Pred> (p)).has (std::forward<Val> (v))
  )

  ढाँचा <typename Pred, typename Val> स्वतः
  impl (Pred&& p, Val &&v, hb_priority<0>) स्थिरांक HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Pred> (p),
	       std::forward<Val> (v))
  )

  सार्वजनिक:

  ढाँचा <typename Pred, typename Val> स्वतः
  चालक () (Pred&& p, Val &&v) स्थिरांक HB_RETURN (तार्किक,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_has);

संरचना
{
  खाजगी:

  ढाँचा <typename Pred, typename Val> स्वतः
  impl (Pred&& p, Val &&v, hb_priority<1>) स्थिरांक HB_AUTO_RETURN
  (
    hb_has (std::forward<Pred> (p),
	    std::forward<Val> (v))
  )

  ढाँचा <typename Pred, typename Val> स्वतः
  impl (Pred&& p, Val &&v, hb_priority<0>) स्थिरांक HB_AUTO_RETURN
  (
    std::forward<Pred> (p) == std::forward<Val> (v)
  )

  सार्वजनिक:

  ढाँचा <typename Pred, typename Val> स्वतः
  चालक () (Pred&& p, Val &&v) स्थिरांक HB_RETURN (तार्किक,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_match);

संरचना
{
  खाजगी:

  ढाँचा <typename Proj, typename Val> स्वतः
  impl (Proj&& f, Val &&v, hb_priority<2>) स्थिरांक HB_AUTO_RETURN
  (
    hb_deref (std::forward<Proj> (f)).get (std::forward<Val> (v))
  )

  ढाँचा <typename Proj, typename Val> स्वतः
  impl (Proj&& f, Val &&v, hb_priority<1>) स्थिरांक HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Proj> (f),
	       std::forward<Val> (v))
  )

  ढाँचा <typename Proj, typename Val> स्वतः
  impl (Proj&& f, Val &&v, hb_priority<0>) स्थिरांक HB_AUTO_RETURN
  (
    std::forward<Proj> (f)[std::forward<Val> (v)]
  )

  सार्वजनिक:

  ढाँचा <typename Proj, typename Val> स्वतः
  चालक () (Proj&& f, Val &&v) स्थिरांक HB_AUTO_RETURN
  (
    impl (std::forward<Proj> (f),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_get);

संरचना
{
  खाजगी:

  ढाँचा <typename T1, typename T2> स्वतः
  impl (T1&& v1, T2 &&v2, hb_priority<3>) स्थिरांक HB_AUTO_RETURN
  (
    std::forward<T2> (v2).cmp (std::forward<T1> (v1)) == 0
  )

  ढाँचा <typename T1, typename T2> स्वतः
  impl (T1&& v1, T2 &&v2, hb_priority<2>) स्थिरांक HB_AUTO_RETURN
  (
    std::forward<T1> (v1).cmp (std::forward<T2> (v2)) == 0
  )

  ढाँचा <typename T1, typename T2> स्वतः
  impl (T1&& v1, T2 &&v2, hb_priority<1>) स्थिरांक HB_AUTO_RETURN
  (
    std::forward<T1> (v1) == std::forward<T2> (v2)
  )

  ढाँचा <typename T1, typename T2> स्वतः
  impl (T1&& v1, T2 &&v2, hb_priority<0>) स्थिरांक HB_AUTO_RETURN
  (
    std::forward<T2> (v2) == std::forward<T1> (v1)
  )

  सार्वजनिक:

  ढाँचा <typename T1, typename T2> स्वतः
  चालक () (T1&& v1, T2 &&v2) स्थिरांक HB_AUTO_RETURN
  (
    impl (std::forward<T1> (v1),
	  std::forward<T2> (v2),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_equal);

संरचना
{
  ढाँचा <typename T> शून्य
  चालक () (T& a, T& b) स्थिरांक
  {
    using std::swap; // allow ADL
    swap (a, b);
  }
}
HB_FUNCOBJ (hb_swap);


ढाँचा <typename T1, typename T2>
संरचना hb_pair_t
{
  प्रकारव्याख्या T1 first_t;
  प्रकारव्याख्या T2 second_t;
  प्रकारव्याख्या hb_pair_t<T1, T2> pair_t;

  ढाँचा <typename U1 = T1, typename U2 = T2,
	    hb_enable_if (std::is_default_constructible<U1>::value &&
			  std::is_default_constructible<U2>::value)>
  hb_pair_t () : first (), second () {}
  hb_pair_t (T1 a, T2 b) : first (std::forward<T1> (a)), second (std::forward<T2> (b)) {}

  ढाँचा <typename Q1, typename Q2,
	    hb_enable_if (hb_is_convertible (T1, Q1) &&
			  hb_is_convertible (T2, Q2))>
  चालक hb_pair_t<Q1, Q2> () { परत hb_pair_t<Q1, Q2> (first, second); }

  hb_pair_t<T1, T2> reverse () स्थिरांक
  { परत hb_pair_t<T1, T2> (second, first); }

  तार्किक चालक == (स्थिरांक pair_t& o) स्थिरांक { परत first == o.first && second == o.second; }
  तार्किक चालक != (स्थिरांक pair_t& o) स्थिरांक { परत !(*हे == o); }
  तार्किक चालक < (स्थिरांक pair_t& o) स्थिरांक { परत first < o.first || (first == o.first && second < o.second); }
  तार्किक चालक >= (स्थिरांक pair_t& o) स्थिरांक { परत !(*हे < o); }
  तार्किक चालक > (स्थिरांक pair_t& o) स्थिरांक { परत first > o.first || (first == o.first && second > o.second); }
  तार्किक चालक <= (स्थिरांक pair_t& o) स्थिरांक { परत !(*हे > o); }

  स्थिर पूर्णांक cmp (स्थिरांक शून्य *pa, स्थिरांक शून्य *pb)
  {
    pair_t *a = (pair_t *) pa;
    pair_t *b = (pair_t *) pb;

    जर (a->first < b->first) परत -1;
    जर (a->first > b->first) परत +1;
    जर (a->second < b->second) परत -1;
    जर (a->second > b->second) परत +1;
    परत 0;
  }

  मित्र शून्य swap (hb_pair_t& a, hb_pair_t& b) noexcept
  {
    hb_swap (a.first, b.first);
    hb_swap (a.second, b.second);
  }


  T1 first;
  T2 second;
};
ढाँचा <typename T1, typename T2> स्थिर ओळीत hb_pair_t<T1, T2>
hb_pair (T1&& a, T2&& b) { परत hb_pair_t<T1, T2> (a, b); }

प्रकारव्याख्या hb_pair_t<hb_codepoint_t, hb_codepoint_t> hb_codepoint_pair_t;

संरचना
{
  ढाँचा <typename Pair> constexpr typename Pair::first_t
  चालक () (स्थिरांक Pair& pair) स्थिरांक { परत pair.first; }
}
HB_FUNCOBJ (hb_first);

संरचना
{
  ढाँचा <typename Pair> constexpr typename Pair::second_t
  चालक () (स्थिरांक Pair& pair) स्थिरांक { परत pair.second; }
}
HB_FUNCOBJ (hb_second);

/* Note.  In min/max impl, we can use hb_type_identity<T> for second argument.
 * However, that would silently convert between different-signedness integers.
 * Instead we accept two different types, such that compiler can err if
 * comparing integers of different signedness. */
संरचना
{
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (T&& a, T2&& b) स्थिरांक HB_AUTO_RETURN
  (a <= b ? a : b)
}
HB_FUNCOBJ (hb_min);
संरचना
{
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (T&& a, T2&& b) स्थिरांक HB_AUTO_RETURN
  (a >= b ? a : b)
}
HB_FUNCOBJ (hb_max);
संरचना
{
  ढाँचा <typename T, typename T2, typename T3> constexpr स्वतः
  चालक () (T&& x, T2&& min, T3&& max) स्थिरांक HB_AUTO_RETURN
  (hb_min (hb_max (std::forward<T> (x), std::forward<T2> (min)), std::forward<T3> (max)))
}
HB_FUNCOBJ (hb_clamp);

/* Signed saturating arithmetic. */
ढाँचा <typename T>
स्थिर HB_ALWAYS_INLINE T
hb_saturate_add (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#जर hb_has_builtin(__builtin_add_overflow)
  T result;
  जर (likely (!__builtin_add_overflow (a, b, &result)))
    परत result;
#अन्यथा
  जर (b > 0)
  {
    जर (likely (a <= hb_int_max (T) - b))
      परत a + b;
  }
  अन्यथा जर (likely (a >= hb_int_min (T) - b))
    परत a + b;
#endif

  परत a < 0 ? hb_int_min (T) : hb_int_max (T);
}

ढाँचा <typename T>
स्थिर HB_ALWAYS_INLINE T
hb_saturate_sub (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#जर hb_has_builtin(__builtin_sub_overflow)
  T result;
  जर (likely (!__builtin_sub_overflow (a, b, &result)))
    परत result;
#अन्यथा
  जर (b > 0)
  {
    जर (likely (a >= hb_int_min (T) + b))
      परत a - b;
  }
  अन्यथा जर (likely (a <= hb_int_max (T) + b))
    परत a - b;
#endif

  परत b < 0 ? hb_int_max (T) : hb_int_min (T);
}

ढाँचा <typename T>
स्थिर HB_ALWAYS_INLINE T
hb_saturate_neg (T a)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  परत hb_saturate_sub ((T) 0, a);
}

/* Convert a floating-point value to integer type T, saturating to T's
 * range instead of relying on the undefined behavior of an out-of-range
 * float-to-int conversion.  NaN saturates to the minimum of T. */
ढाँचा <typename T>
स्थिर HB_ALWAYS_INLINE T
hb_clamp_to (द्विगुण v)
{
  परत (T) hb_clamp (v,
		       (द्विगुण) hb_int_min (T),
		       (द्विगुण) hb_int_max (T));
}

ढाँचा <typename T>
स्थिर HB_ALWAYS_INLINE T
hb_clamp_to (दशांश v)
{
  /* Widen explicitly: float cannot represent all integer bounds exactly,
   * so the clamp itself is done in double. */
  परत hb_clamp_to<T> ((द्विगुण) v);
}

ढाँचा <typename T>
स्थिर HB_ALWAYS_INLINE T
hb_clamp_to (int64_t v)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  परत (T) hb_clamp (v,
		       (int64_t) hb_int_min (T),
		       (int64_t) hb_int_max (T));
}

/*
 * Bithacks.
 */

/* Return the number of 1 bits in a uint8_t; faster than hb_popcount() */
स्थिर ओळीत अचिन्हित
hb_popcount8 (uint8_t v)
{
  स्थिर स्थिरांक uint8_t popcount4[16] = {
    0, 1, 1, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 2, 3, 3, 4
  };
  परत popcount4[v & 0xF] + popcount4[v >> 4];
}

/* Return the number of 1 bits in v. */
ढाँचा <typename T>
स्थिर ओळीत अचिन्हित पूर्णांक
hb_popcount (T v)
{
#जर hb_has_builtin(__builtin_popcount)
  जर (आकार (T) <= आकार (अचिन्हित पूर्णांक))
    परत __builtin_popcount (v);
#endif

#जर hb_has_builtin(__builtin_popcountl)
  जर (आकार (T) <= आकार (अचिन्हित दीर्घ))
    परत __builtin_popcountl (v);
#endif

#जर hb_has_builtin(__builtin_popcountll)
  जर (आकार (T) <= आकार (अचिन्हित दीर्घ दीर्घ))
    परत __builtin_popcountll (v);
#endif

  जर (आकार (T) <= 4)
  {
    /* "HACKMEM 169" */
    uint32_t y;
    y = (v >> 1) &033333333333;
    y = v - y - ((y >>1) & 033333333333);
    परत (((y + (y >> 3)) & 030707070707) % 077);
  }

  जर (आकार (T) == 8)
  {
    uint64_t y = (uint64_t) v;
    y -= ((y >> 1) & 0x5555555555555555ull);
    y = (y & 0x3333333333333333ull) + (y >> 2 & 0x3333333333333333ull);
    परत ((y + (y >> 4)) & 0xf0f0f0f0f0f0f0full) * 0x101010101010101ull >> 56;
  }

  जर (आकार (T) == 16)
  {
    अचिन्हित पूर्णांक shift = 64;
    परत hb_popcount<uint64_t> ((uint64_t) v) + hb_popcount ((uint64_t) (v >> shift));
  }

  प्रतिज्ञा (0);
  परत 0; /* Shut up stupid compiler. */
}

/* Returns the number of bits needed to store number */
ढाँचा <typename T>
स्थिर ओळीत अचिन्हित पूर्णांक
hb_bit_storage (T v)
{
  जर (unlikely (!v)) परत 0;

#जर hb_has_builtin(__builtin_clz)
  जर (आकार (T) <= आकार (अचिन्हित पूर्णांक))
    परत आकार (अचिन्हित पूर्णांक) * 8 - __builtin_clz (v);
#endif

#जर hb_has_builtin(__builtin_clzl)
  जर (आकार (T) <= आकार (अचिन्हित दीर्घ))
    परत आकार (अचिन्हित दीर्घ) * 8 - __builtin_clzl (v);
#endif

#जर hb_has_builtin(__builtin_clzll)
  जर (आकार (T) <= आकार (अचिन्हित दीर्घ दीर्घ))
    परत आकार (अचिन्हित दीर्घ दीर्घ) * 8 - __builtin_clzll (v);
#endif

#जर (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  जर (आकार (T) <= आकार (अचिन्हित पूर्णांक))
  {
    अचिन्हित दीर्घ where;
    _BitScanReverse (&where, v);
    परत 1 + where;
  }
# जर defined(_WIN64)
  जर (आकार (T) <= 8)
  {
    अचिन्हित दीर्घ where;
    _BitScanReverse64 (&where, v);
    परत 1 + where;
  }
# endif
#endif

  जर (आकार (T) <= 4)
  {
    /* "bithacks" */
    स्थिरांक अचिन्हित पूर्णांक b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    स्थिरांक अचिन्हित पूर्णांक S[] = {1, 2, 4, 8, 16};
    अचिन्हित पूर्णांक r = 0;
    साठी (पूर्णांक i = 4; i >= 0; i--)
      जर (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    परत r + 1;
  }
  जर (आकार (T) <= 8)
  {
    /* "bithacks" */
    स्थिरांक uint64_t b[] = {0x2ULL, 0xCULL, 0xF0ULL, 0xFF00ULL, 0xFFFF0000ULL, 0xFFFFFFFF00000000ULL};
    स्थिरांक अचिन्हित पूर्णांक S[] = {1, 2, 4, 8, 16, 32};
    अचिन्हित पूर्णांक r = 0;
    साठी (पूर्णांक i = 5; i >= 0; i--)
      जर (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    परत r + 1;
  }
  जर (आकार (T) == 16)
  {
    अचिन्हित पूर्णांक shift = 64;
    परत (v >> shift) ? hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift :
			  hb_bit_storage<uint64_t> ((uint64_t) v);
  }

  प्रतिज्ञा (0);
  परत 0; /* Shut up stupid compiler. */
}

/* Returns the number of zero bits in the least significant side of v */
ढाँचा <typename T>
स्थिर ओळीत अचिन्हित पूर्णांक
hb_ctz (T v)
{
  जर (unlikely (!v)) परत 8 * आकार (T);

#जर hb_has_builtin(__builtin_ctz)
  जर (आकार (T) <= आकार (अचिन्हित पूर्णांक))
    परत __builtin_ctz (v);
#endif

#जर hb_has_builtin(__builtin_ctzl)
  जर (आकार (T) <= आकार (अचिन्हित दीर्घ))
    परत __builtin_ctzl (v);
#endif

#जर hb_has_builtin(__builtin_ctzll)
  जर (आकार (T) <= आकार (अचिन्हित दीर्घ दीर्घ))
    परत __builtin_ctzll (v);
#endif

#जर (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  जर (आकार (T) <= आकार (अचिन्हित पूर्णांक))
  {
    अचिन्हित दीर्घ where;
    _BitScanForward (&where, v);
    परत where;
  }
# जर defined(_WIN64)
  जर (आकार (T) <= 8)
  {
    अचिन्हित दीर्घ where;
    _BitScanForward64 (&where, v);
    परत where;
  }
# endif
#endif

  जर (आकार (T) <= 4)
  {
    /* "bithacks" */
    अचिन्हित पूर्णांक c = 32;
    v &= - (int32_t) v;
    जर (v) c--;
    जर (v & 0x0000FFFF) c -= 16;
    जर (v & 0x00FF00FF) c -= 8;
    जर (v & 0x0F0F0F0F) c -= 4;
    जर (v & 0x33333333) c -= 2;
    जर (v & 0x55555555) c -= 1;
    परत c;
  }
  जर (आकार (T) <= 8)
  {
    /* "bithacks" */
    अचिन्हित पूर्णांक c = 64;
    v &= - (int64_t) (v);
    जर (v) c--;
    जर (v & 0x00000000FFFFFFFFULL) c -= 32;
    जर (v & 0x0000FFFF0000FFFFULL) c -= 16;
    जर (v & 0x00FF00FF00FF00FFULL) c -= 8;
    जर (v & 0x0F0F0F0F0F0F0F0FULL) c -= 4;
    जर (v & 0x3333333333333333ULL) c -= 2;
    जर (v & 0x5555555555555555ULL) c -= 1;
    परत c;
  }
  जर (आकार (T) == 16)
  {
    अचिन्हित पूर्णांक shift = 64;
    परत (uint64_t) v ? hb_bit_storage<uint64_t> ((uint64_t) v) :
			  hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift;
  }

  प्रतिज्ञा (0);
  परत 0; /* Shut up stupid compiler. */
}


/*
 * Tiny stuff.
 */

/* ASCII tag/character handling */
स्थिर ओळीत तार्किक ISALPHA (अचिन्हित अक्षर c)
{ परत (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
स्थिर ओळीत तार्किक ISALNUM (अचिन्हित अक्षर c)
{ परत (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }
स्थिर ओळीत तार्किक ISSPACE (अचिन्हित अक्षर c)
{ परत c == ' ' || c =='\f'|| c =='\n'|| c =='\r'|| c =='\t'|| c =='\v'; }
स्थिर ओळीत अचिन्हित अक्षर TOUPPER (अचिन्हित अक्षर c)
{ परत (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c; }
स्थिर ओळीत अचिन्हित अक्षर TOLOWER (अचिन्हित अक्षर c)
{ परत (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }
स्थिर ओळीत तार्किक ISHEX (अचिन्हित अक्षर c)
{ परत (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
स्थिर ओळीत अचिन्हित अक्षर TOHEX (uint8_t c)
{ परत (c & 0xF) <= 9 ? (c & 0xF) + '0' : (c & 0xF) + 'a' - 10; }
स्थिर ओळीत uint8_t FROMHEX (अचिन्हित अक्षर c)
{ परत (c >= '0' && c <= '9') ? c - '0' : TOLOWER (c) - 'a' + 10; }

स्थिर ओळीत अचिन्हित पूर्णांक DIV_CEIL (स्थिरांक अचिन्हित पूर्णांक a, अचिन्हित पूर्णांक b)
{ परत (a + (b - 1)) / b; }


#undef  ARRAY_LENGTH
ढाँचा <typename Type, अचिन्हित पूर्णांक n>
स्थिर ओळीत अचिन्हित पूर्णांक ARRAY_LENGTH (स्थिरांक Type (&)[n]) { परत n; }
/* A const version, but does not detect erratically being called on pointers. */
#define ARRAY_LENGTH_CONST(__array) ((चिन्हित पूर्णांक) (आकार (__array) / आकार (__array[0])))


स्थिर ओळीत शून्य *
hb_memcpy (शून्य *__restrict dst, स्थिरांक शून्य *__restrict src, माप_प्रकार len)
{
  /* It's illegal to pass 0 as size to memcpy. */
  जर (unlikely (!len)) परत dst;
  परत स_नकल (dst, src, len);
}

स्थिर ओळीत पूर्णांक
hb_memcmp (स्थिरांक शून्य *a, स्थिरांक शून्य *b, अचिन्हित पूर्णांक len)
{
  /* It's illegal to pass NULL to memcmp(), even if len is zero.
   * So, wrap it.
   * https://sourceware.org/bugzilla/show_bug.cgi?id=23878 */
  जर (unlikely (!len)) परत 0;
  परत स_भेद (a, b, len);
}

स्थिर ओळीत शून्य *
hb_memset (शून्य *s, पूर्णांक c, अचिन्हित पूर्णांक n)
{
  /* It's illegal to pass NULL to memset(), even if n is zero. */
  जर (unlikely (!n)) परत s;
  परत स_रखो (s, c, n);
}

स्थिर ओळीत अचिन्हित पूर्णांक
hb_ceil_to_4 (अचिन्हित पूर्णांक v)
{
  परत ((v - 1) | 3) + 1;
}

ढाँचा <typename T> स्थिर ओळीत तार्किक
hb_in_range (T u, T lo, T hi)
{
  static_assert (!std::is_signed<T>::value, "");

  /* The casts below are important as if T is smaller than int,
   * the subtract results will become a signed int! */
  परत (T)(u - lo) <= (T)(hi - lo);
}
ढाँचा <typename T> स्थिर ओळीत तार्किक
hb_in_ranges (T u, T lo1, T hi1)
{
  परत hb_in_range (u, lo1, hi1);
}
ढाँचा <typename T, typename ...Ts> स्थिर ओळीत तार्किक
hb_in_ranges (T u, T lo1, T hi1, Ts... ds)
{
  परत hb_in_range<T> (u, lo1, hi1) || hb_in_ranges<T> (u, ds...);
}


/*
 * Overflow checking.
 */

स्थिर ओळीत तार्किक
hb_unsigned_mul_overflows (अचिन्हित पूर्णांक मोजणी, अचिन्हित पूर्णांक size, अचिन्हित *result = nullptr)
{
#जर hb_has_builtin(__builtin_mul_overflow)
  अचिन्हित चळत_result;
  जर (!result)
    result = &चळत_result;
  परत __builtin_mul_overflow (मोजणी, size, result);
#endif

  जर (result)
    *result = मोजणी * size;
  परत (size > 0) && (मोजणी >= ((अचिन्हित पूर्णांक) -1) / size);
}

स्थिर ओळीत तार्किक
hb_unsigned_add_overflows (अचिन्हित पूर्णांक a, अचिन्हित पूर्णांक b, अचिन्हित *result = nullptr)
{
#जर hb_has_builtin(__builtin_add_overflow)
  अचिन्हित चळत_result;
  जर (!result)
    result = &चळत_result;
  परत __builtin_add_overflow (a, b, result);
#endif

  जर (result)
    *result = a + b;
  परत b > (अचिन्हित पूर्णांक) -1 - a;
}

/* Saturating arithmetic on size_t.  On platforms where size_t is wider than
 * unsigned int (i.e. 64-bit), the inputs to get_size()-style computations
 * (counts and static_sizes which are at most 32-bit) cannot overflow size_t,
 * so these reduce to plain arithmetic.  On 32-bit platforms (size_t is
 * unsigned int), they saturate to SIZE_MAX so sanitize/serialize callers
 * naturally reject the resulting size as out-of-range. */

स्थिर ओळीत माप_प्रकार
hb_unsigned_mul_saturate (माप_प्रकार a, माप_प्रकार b)
{
  जर (आकार (माप_प्रकार) > आकार (अचिन्हित पूर्णांक))
    परत a * b;
#जर hb_has_builtin(__builtin_mul_overflow)
  माप_प्रकार result;
  जर (__builtin_mul_overflow (a, b, &result))
    परत (माप_प्रकार) -1;
  परत result;
#अन्यथा
  जर (b > 0 && a > ((माप_प्रकार) -1) / b) परत (माप_प्रकार) -1;
  परत a * b;
#endif
}

स्थिर ओळीत माप_प्रकार
hb_unsigned_add_saturate (माप_प्रकार a, माप_प्रकार b)
{
  जर (आकार (माप_प्रकार) > आकार (अचिन्हित पूर्णांक))
    परत a + b;
#जर hb_has_builtin(__builtin_add_overflow)
  माप_प्रकार result;
  जर (__builtin_add_overflow (a, b, &result))
    परत (माप_प्रकार) -1;
  परत result;
#अन्यथा
  जर (b > ((माप_प्रकार) -1) - a) परत (माप_प्रकार) -1;
  परत a + b;
#endif
}

/* Variadic forms: fold left across all arguments. */
ढाँचा <typename ...Ts>
स्थिर ओळीत माप_प्रकार
hb_unsigned_mul_saturate (माप_प्रकार a, माप_प्रकार b, माप_प्रकार c, Ts... rest)
{ परत hb_unsigned_mul_saturate (hb_unsigned_mul_saturate (a, b), c, rest...); }

ढाँचा <typename ...Ts>
स्थिर ओळीत माप_प्रकार
hb_unsigned_add_saturate (माप_प्रकार a, माप_प्रकार b, माप_प्रकार c, Ts... rest)
{ परत hb_unsigned_add_saturate (hb_unsigned_add_saturate (a, b), c, rest...); }

/* Saturating mul-add: a * b + c.  Covers the dominant get_size() pattern
 * `count * static_size + min_size`. */
स्थिर ओळीत माप_प्रकार
hb_unsigned_mul_add_saturate (माप_प्रकार a, माप_प्रकार b, माप_प्रकार c)
{ परत hb_unsigned_add_saturate (hb_unsigned_mul_saturate (a, b), c); }


/*
 * Sort and search.
 */

ढाँचा <typename K, typename V, typename ...Ts>
स्थिर पूर्णांक
_hb_cmp_method (स्थिरांक शून्य *pkey, स्थिरांक शून्य *pval, Ts... ds)
{
  स्थिरांक K& key = * (स्थिरांक K*) pkey;
  स्थिरांक V& val = * (स्थिरांक V*) pval;

  परत val.cmp (key, ds...);
}

ढाँचा <typename K, typename V>
स्थिर पूर्णांक
_hb_cmp_operator (स्थिरांक शून्य *pkey, स्थिरांक शून्य *pval)
{
  स्थिरांक K& key = * (स्थिरांक K*) pkey;
  स्थिरांक V& val = * (स्थिरांक V*) pval;

  जर (key < val) परत -1;
  जर (key > val) परत  1;
  परत 0;
}

ढाँचा <typename V, typename K, typename ...Ts>
HB_HOT
स्थिर ओळीत तार्किक
hb_bsearch_impl (अचिन्हित *pos, /* Out */
		 स्थिरांक K& key,
		 V* base, माप_प्रकार nmemb, माप_प्रकार stride,
		 पूर्णांक (*compar)(स्थिरांक शून्य *किल्ली_3, स्थिरांक शून्य *_item, Ts... _ds),
		 Ts... ds)
{
  /* This is our *only* bsearch implementation. */

  पूर्णांक min = 0, max = (पूर्णांक) nmemb - 1;
  पर्यंत (min <= max)
  {
    पूर्णांक mid = ((अचिन्हित पूर्णांक) min + (अचिन्हित पूर्णांक) max) / 2;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    V* p = (V*) (((स्थिरांक अक्षर *) base) + (mid * stride));
#pragma GCC diagnostic काढा
    पूर्णांक c = compar ((स्थिरांक शून्य *) std::addressof (key), (स्थिरांक शून्य *) p, ds...);
    जर (c < 0)
      max = mid - 1;
    अन्यथा जर (c > 0)
      min = mid + 1;
    अन्यथा
    {
      *pos = mid;
      परत सत्य;
    }
  }
  *pos = min;
  परत असत्य;
}

ढाँचा <typename V, typename K>
स्थिर ओळीत V*
hb_bsearch (स्थिरांक K& key, V* base,
	    माप_प्रकार nmemb, माप_प्रकार stride = आकार (V),
	    पूर्णांक (*compar)(स्थिरांक शून्य *किल्ली_3, स्थिरांक शून्य *_item) = _hb_cmp_method<K, V>)
{
  अचिन्हित pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  परत hb_bsearch_impl (&pos, key, base, nmemb, stride, compar) ?
	 (V*) (((स्थिरांक अक्षर *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic काढा
}
ढाँचा <typename V, typename K, typename ...Ts>
स्थिर ओळीत V*
hb_bsearch (स्थिरांक K& key, V* base,
	    माप_प्रकार nmemb, माप_प्रकार stride,
	    पूर्णांक (*compar)(स्थिरांक शून्य *किल्ली_3, स्थिरांक शून्य *_item, Ts... _ds),
	    Ts... ds)
{
  अचिन्हित pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  परत hb_bsearch_impl (&pos, key, base, nmemb, stride, compar, ds...) ?
	 (V*) (((स्थिरांक अक्षर *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic काढा
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
ढाँचा <typename T, typename Compar>
स्थिर ओळीत शून्य
hb_qsort_loop (T *base, माप_प्रकार nel, Compar compar)
{
  पर्यंत (nel > 24)
  {
    T *last = base + nel - 1;
    T *mid = base + nel / 2;

    /* Median-of-three pivot, parked at last-1. */
    जर (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    जर (compar (*mid, *last) > 0)
    {
      hb_swap (*mid, *last);
      जर (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    }
    hb_swap (*mid, *(last - 1));
    T &pivot = *(last - 1);

    /* Two-way Hoare partition.  Inner loops are unguarded:
     * median-of-three left *base <= pivot and *last >= pivot,
     * which act as sentinels. */
    T *i = base, *j = last - 1;
    पर्यंत (सत्य)
    {
      पर्यंत (compar (*++i, pivot) < 0) {}
      पर्यंत (compar (*--j, pivot) > 0) {}
      जर (i >= j) थांबा;
      hb_swap (*i, *j);
    }
    hb_swap (*i, *(last - 1));

    /* Recurse on smaller side, loop on larger — bounds stack
     * depth at O(log n). */
    माप_प्रकार left  = (माप_प्रकार) (i - base);
    माप_प्रकार उजवा = nel - left - 1;
    जर (left < उजवा)
    {
      hb_qsort_loop (base, left, compar);
      base = i + 1;
      nel  = उजवा;
    }
    अन्यथा
    {
      hb_qsort_loop (i + 1, उजवा, compar);
      nel  = left;
    }
  }
}

ढाँचा <typename T, typename Compar>
स्थिर ओळीत शून्य
hb_qsort_inline (T *base, माप_प्रकार nel, Compar compar)
{
  hb_qsort_loop (base, nel, compar);

  /* Single final insertion sort over the whole array.  After
   * the partitioning loop, every element is within the threshold
   * of its sorted position, so this pass is O(n * threshold). */
  T *end = base + nel;
  साठी (T *pi = base + 1; pi < end; pi++)
    साठी (T *pj = pi; pj > base && compar (pj[-1], pj[0]) > 0; pj--)
      hb_swap (pj[-1], pj[0]);
}

स्थिर ओळीत शून्य
hb_qsort (शून्य *base, माप_प्रकार nel, माप_प्रकार width,
	  पूर्णांक (*compar)(स्थिरांक शून्य *_a, स्थिरांक शून्य *_b))
{
  क्विक (base, nel, width, compar);
}




ढाँचा <typename T, typename T2, typename T3 = पूर्णांक> स्थिर ओळीत शून्य
hb_stable_sort (T *array, अचिन्हित पूर्णांक len, पूर्णांक(*compar)(स्थिरांक T2 *, स्थिरांक T2 *), T3 *array2 = nullptr)
{
  static_assert (hb_is_trivially_copy_assignable (T), "");
  static_assert (hb_is_trivially_copy_assignable (T3), "");

  साठी (अचिन्हित पूर्णांक i = 1; i < len; i++)
  {
    अचिन्हित पूर्णांक j = i;
    पर्यंत (j && compar (&array[j - 1], &array[i]) > 0)
      j--;
    जर (i == j)
      सुरूठेवा;
    /* Move item i to occupy place for item j, shift what's in between. */
    {
      T t = array[i];
      स_हटाओ (&array[j + 1], &array[j], (i - j) * आकार (T));
      array[j] = t;
    }
    जर (array2)
    {
      T3 t = array2[i];
      स_हटाओ (&array2[j + 1], &array2[j], (i - j) * आकार (T3));
      array2[j] = t;
    }
  }
}

स्थिर ओळीत hb_bool_t
hb_codepoint_parse (स्थिरांक अक्षर *s, अचिन्हित पूर्णांक len, पूर्णांक base, hb_codepoint_t *out)
{
  अचिन्हित पूर्णांक v;
  स्थिरांक अक्षर *p = s;
  स्थिरांक अक्षर *end = p + len;
  जर (unlikely (!hb_parse_uint (&p, end, &v, सत्य/* whole buffer */, base)))
    परत असत्य;

  *out = v;
  परत सत्य;
}


/* Operators. */

संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिरांक T &a, स्थिरांक T &b) स्थिरांक HB_AUTO_RETURN (a & b)
}
HB_FUNCOBJ (hb_bitwise_and);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिरांक T &a, स्थिरांक T &b) स्थिरांक HB_AUTO_RETURN (a | b)
}
HB_FUNCOBJ (hb_bitwise_or);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिरांक T &a, स्थिरांक T &b) स्थिरांक HB_AUTO_RETURN (a ^ b)
}
HB_FUNCOBJ (hb_bitwise_xor);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिरांक T &a, स्थिरांक T &b) स्थिरांक HB_AUTO_RETURN (~a & b)
}
HB_FUNCOBJ (hb_bitwise_lt);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिरांक T &a, स्थिरांक T &b) स्थिरांक HB_AUTO_RETURN (a & ~b)
}
HB_FUNCOBJ (hb_bitwise_gt); // aka sub
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिरांक T &a, स्थिरांक T &b) स्थिरांक HB_AUTO_RETURN (~a | b)
}
HB_FUNCOBJ (hb_bitwise_le);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिरांक T &a, स्थिरांक T &b) स्थिरांक HB_AUTO_RETURN (a | ~b)
}
HB_FUNCOBJ (hb_bitwise_ge);
संरचना
{
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिरांक T &a) स्थिरांक HB_AUTO_RETURN (~a)
}
HB_FUNCOBJ (hb_bitwise_neg);

संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (स्थिरांक T &a, स्थिरांक T2 &b) स्थिरांक HB_AUTO_RETURN (a + b)
}
HB_FUNCOBJ (hb_add);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (स्थिरांक T &a, स्थिरांक T2 &b) स्थिरांक HB_AUTO_RETURN (a - b)
}
HB_FUNCOBJ (hb_sub);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (स्थिरांक T &a, स्थिरांक T2 &b) स्थिरांक HB_AUTO_RETURN (b - a)
}
HB_FUNCOBJ (hb_rsub);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (स्थिरांक T &a, स्थिरांक T2 &b) स्थिरांक HB_AUTO_RETURN (a * b)
}
HB_FUNCOBJ (hb_mul);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (स्थिरांक T &a, स्थिरांक T2 &b) स्थिरांक HB_AUTO_RETURN (a / b)
}
HB_FUNCOBJ (hb_div);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (स्थिरांक T &a, स्थिरांक T2 &b) स्थिरांक HB_AUTO_RETURN (a % b)
}
HB_FUNCOBJ (hb_mod);
संरचना
{
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिरांक T &a) स्थिरांक HB_AUTO_RETURN (+a)
}
HB_FUNCOBJ (hb_pos);
संरचना
{
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिरांक T &a) स्थिरांक HB_AUTO_RETURN (-a)
}
HB_FUNCOBJ (hb_neg);
संरचना
{
  ढाँचा <typename T> constexpr स्वतः
  चालक () (T &a) स्थिरांक HB_AUTO_RETURN (++a)
}
HB_FUNCOBJ (hb_inc);
संरचना
{
  ढाँचा <typename T> constexpr स्वतः
  चालक () (T &a) स्थिरांक HB_AUTO_RETURN (--a)
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
ढाँचा <typename func_t>
द्विगुण solve_itp (func_t f,
		  द्विगुण a, द्विगुण b,
		  द्विगुण epsilon,
		  द्विगुण min_y, द्विगुण max_y,
		  द्विगुण &ya, द्विगुण &yb, द्विगुण &y)
{
  // Guard against degenerate interval
  जर (b - a <= 0.0)
  {
    y = ya;
    परत a;
  }

  अचिन्हित n1_2 = (अचिन्हित) (hb_max (उच्चमान (log2 ((b - a) / epsilon)) - 1.0, 0.0));
  स्थिरांक अचिन्हित n0 = 1; // Hardwired
  स्थिरांक द्विगुण k1 = 0.2 / (b - a); // Hardwired.
  अचिन्हित nmax = n0 + n1_2;
  द्विगुण scaled_epsilon = epsilon * द्विगुण (1llu << nmax);
  द्विगुण _2_epsilon = 2.0 * epsilon;
  पर्यंत (b - a > _2_epsilon)
  {
    द्विगुण x1_2 = 0.5 * (a + b);
    द्विगुण r = scaled_epsilon - 0.5 * (b - a);
    // Guard against yb == ya to prevent division by zero
    द्विगुण xf = (yb != ya) ? (yb * a - ya * b) / (yb - ya) : x1_2;
    द्विगुण sigma = x1_2 - xf;
    द्विगुण b_a = b - a;
    // This has k2 = 2 hardwired for efficiency.
    द्विगुण b_a_k2 = b_a * b_a;
    द्विगुण delta = k1 * b_a_k2;
    पूर्णांक sigma_sign = sigma >= 0 ? +1 : -1;
    द्विगुण xt = delta <= भ_असल (x1_2 - xf) ? xf + delta * sigma_sign : x1_2;
    द्विगुण xitp = भ_असल (xt - x1_2) <= r ? xt : x1_2 - r * sigma_sign;
    द्विगुण yitp = f (xitp);
    जर (yitp > max_y)
    {
      b = xitp;
      yb = yitp;
    }
    अन्यथा जर (yitp < min_y)
    {
      a = xitp;
      ya = yitp;
    }
    अन्यथा
    {
      y = yitp;
      परत xitp;
    }
    scaled_epsilon *= 0.5;
  }
  परत 0.5 * (a + b);
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
ढाँचा <typename F>
संरचना hb_scope_guard_t
{
  explicit hb_scope_guard_t (F &&f) : f (std::move (f)), active (सत्य) {}
  hb_scope_guard_t (hb_scope_guard_t &&o) noexcept
    : f (std::move (o.f)), active (o.active) { o.active = असत्य; }
  hb_scope_guard_t (स्थिरांक hb_scope_guard_t &) = delete;
  hb_scope_guard_t &चालक= (स्थिरांक hb_scope_guard_t &) = delete;
  hb_scope_guard_t &चालक= (hb_scope_guard_t &&) = delete;
  ~hb_scope_guard_t () { जर (active) f (); }

  /* Release: dismiss the guard so the cleanup does NOT run.  Use
   * when transferring ownership out of the scope. */
  शून्य release () { active = असत्य; }

  खाजगी:
  F f;
  तार्किक active;
};

ढाँचा <typename F>
स्थिर ओळीत hb_scope_guard_t<F> hb_make_scope_guard (F &&f)
{ परत hb_scope_guard_t<F> (std::forward<F> (f)); }

#define HB_SCOPE_GUARD_NAME_(ओळ) hb_scope_guard_##line
#define HB_SCOPE_GUARD_NAME(ओळ) HB_SCOPE_GUARD_NAME_(ओळ)
#define HB_SCOPE_GUARD(stmt) \
  स्वतः HB_SCOPE_GUARD_NAME(__LINE__) = \
    hb_make_scope_guard ([&]() { stmt; }); \
  (शून्य) HB_SCOPE_GUARD_NAME(__LINE__)


#endif /* HB_ALGS_HH */
