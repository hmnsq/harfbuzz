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
  यदि (size && nmemb > SIZE_MAX / size) फर्काऊ nullptr;
  फर्काऊ hb_malloc (nmemb * size);
}
स्थिर HB_ALWAYS_INLINE शून्य *
hb_realloc2 (शून्य *ptr, माप_प्रकार nmemb, माप_प्रकार size)
{
  यदि (size && nmemb > SIZE_MAX / size) फर्काऊ nullptr;
  फर्काऊ hb_realloc (ptr, nmemb * size);
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
	  स्थिर अन्तःस्थ constexpr T चालक | (T l, T r) { फर्काऊ T ((अचिन्हित) l | (अचिन्हित) r); } \
	  स्थिर अन्तःस्थ constexpr T चालक & (T l, T r) { फर्काऊ T ((अचिन्हित) l & (अचिन्हित) r); } \
	  स्थिर अन्तःस्थ constexpr T चालक ^ (T l, T r) { फर्काऊ T ((अचिन्हित) l ^ (अचिन्हित) r); } \
	  स्थिर अन्तःस्थ constexpr अचिन्हित चालक ~ (T r) { फर्काऊ (~(अचिन्हित) r); } \
	  स्थिर अन्तःस्थ T& चालक |= (T &l, T r) { l = l | r; फर्काऊ l; } \
	  स्थिर अन्तःस्थ T& चालक &= (T& l, T r) { l = l & r; फर्काऊ l; } \
	  स्थिर अन्तःस्थ T& चालक ^= (T& l, T r) { l = l ^ r; फर्काऊ l; } \
	} \
	static_assert (सत्य, "")

/* Useful for set-operations on small enums.
 * For example, for testing "x ∈ {x1, x2, x3}" use:
 * (FLAG_UNSAFE(x) & (FLAG(x1) | FLAG(x2) | FLAG(x3)))
 */
#define FLAG(x) (static_assert_अभिव्यक्ति ((अचिन्हित)(x) < 32) + (((uint32_t) 1U) << (अचिन्हित)(x)))
#define FLAG_UNSAFE(x) ((अचिन्हित)(x) < 32 ? (((uint32_t) 1U) << (अचिन्हित)(x)) : 0)
#define FLAG_RANGE(x,y) (static_assert_अभिव्यक्ति ((x) < (y)) + FLAG(y+1) - FLAG(x))
#define FLAG64(x) (static_assert_अभिव्यक्ति ((अचिन्हित)(x) < 64) + (((uint64_t) 1ULL) << (अचिन्हित)(x)))
#define FLAG64_UNSAFE(x) ((अचिन्हित)(x) < 64 ? (((uint64_t) 1ULL) << (अचिन्हित)(x)) : 0)


/*
 * Fixed-endian integers / floats.
 */


/* Endian swap, used in Windows related backends */
स्थिर अन्तःस्थ constexpr uint16_t hb_uint16_swap (uint16_t v)
{ फर्काऊ (v >> 8) | (v << 8); }
स्थिर अन्तःस्थ constexpr uint32_t hb_uint32_swap (uint32_t v)
{ फर्काऊ (hb_uint16_swap (v) << 16) | hb_uint16_swap (v >> 16); }

ढाँचा <typename Type>
संरचना __attribute__((packed)) hb_packed_t
{
  hb_packed_t () = स्वभाव;
  constexpr hb_packed_t (Type V) : v (V) {}
  चालक Type () अचर { फर्काऊ v; }
  hb_packed_t & चालक = (Type V) { v = V; फर्काऊ *एतत्; }

  गुप्त:
  Type v;
};

#ifndef HB_FAST_अङ्क_ACCESS

#यदि defined(__OPTIMIZE__) && \
    defined(__BYTE_ORDER) && \
    (__BYTE_ORDER == __BIG_ENDIAN || \
     (__BYTE_ORDER == __LITTLE_ENDIAN && \
      hb_has_builtin(__builtin_bswap16) && \
      hb_has_builtin(__builtin_bswap32) && \
      hb_has_builtin(__builtin_bswap64)))
#define HB_FAST_अङ्क_ACCESS 1
#अन्यथा
#define HB_FAST_अङ्क_ACCESS 0
#endif

// https://github.com/harfbuzz/harfbuzz/issues/5456
#यदि defined(__GNUC__) && !defined(__clang__) && (__GNUC__ <= 12)
#undef HB_FAST_अङ्क_ACCESS
#define HB_FAST_अङ्क_ACCESS 0
#endif

#endif

ढाँचा <तार्किक BE, typename Type, पूर्णाङ्क Bytes = परिमाण (Type)>
संरचना HBInt;
ढाँचा <तार्किक BE, typename Type>
संरचना HBInt<BE, Type, 1>
{
  सार्वजनिक:
  HBInt () = स्वभाव;
  constexpr HBInt (Type V) : v {uint8_t (V)} {}
  constexpr चालक Type () अचर { फर्काऊ v; }
  गुप्त: uint8_t v;
};
ढाँचा <तार्किक BE, typename Type>
संरचना HBInt<BE, Type, 2>
{
  सार्वजनिक:
  HBInt () = स्वभाव;

  HBInt (Type V)
#यदि HB_FAST_अङ्क_ACCESS
  {
    यदि (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint16_t> *) v) = V;
    अन्यथा
      *((hb_packed_t<uint16_t> *) v) = __builtin_bswap16 (V);
  }
#अन्यथा
    : v {BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >>  8) & 0xFF)} {}
#endif

  constexpr चालक Type () अचर
  {
#यदि HB_FAST_अङ्क_ACCESS
    फर्काऊ (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint16_t) *((अचर hb_packed_t<uint16_t> *) v)
    :
      __builtin_bswap16 ((uint16_t) *((अचर hb_packed_t<uint16_t> *) v))
    ;
#अन्यथा
    फर्काऊ (BE ? (v[0] <<  8) : (v[0]      ))
	 + (BE ? (v[1]      ) : (v[1] <<  8));
#endif
  }
  गुप्त: uint8_t v[2];
};
ढाँचा <तार्किक BE, typename Type>
संरचना HBInt<BE, Type, 3>
{
  static_assert (!std::is_signed<Type>::value, "");
  सार्वजनिक:
  HBInt () = स्वभाव;
  constexpr HBInt (Type V) : v {BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
				BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
				BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V      ) & 0xFF)} {}

  constexpr चालक Type () अचर { फर्काऊ (BE ? (v[0] << 16) : (v[0]      ))
					  + (BE ? (v[1] <<  8) : (v[1] <<  8))
					  + (BE ? (v[2]      ) : (v[2] << 16)); }
  गुप्त: uint8_t v[3];
};
ढाँचा <तार्किक BE, typename Type>
संरचना HBInt<BE, Type, 4>
{
  ढाँचा <तार्किक, typename, पूर्णाङ्क>
  मित्र संरचना HBFloat;

  सार्वजनिक:
  HBInt () = स्वभाव;

  HBInt (Type V)
#यदि HB_FAST_अङ्क_ACCESS
  {
    यदि (BE == (__BYTE_ORDER == __BIG_ENDIAN))
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

  constexpr चालक Type () अचर {
#यदि HB_FAST_अङ्क_ACCESS
    फर्काऊ (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint32_t) *((अचर hb_packed_t<uint32_t> *) v)
    :
      __builtin_bswap32 ((uint32_t) *((अचर hb_packed_t<uint32_t> *) v))
    ;
#अन्यथा
    फर्काऊ (BE ? (uint32_t (v[0]) << 24) : (uint32_t (v[0])      ))
	 + (BE ? (uint32_t (v[1]) << 16) : (uint32_t (v[1]) <<  8))
	 + (BE ? (uint32_t (v[2]) <<  8) : (uint32_t (v[2]) << 16))
	 + (BE ? (uint32_t (v[3])      ) : (uint32_t (v[3]) << 24));
#endif
  }
  गुप्त: uint8_t v[4];
};
ढाँचा <तार्किक BE, typename Type>
संरचना HBInt<BE, Type, 8>
{
  ढाँचा <तार्किक, typename, पूर्णाङ्क>
  मित्र संरचना HBFloat;

  सार्वजनिक:
  HBInt () = स्वभाव;

  HBInt (Type V)
#यदि HB_FAST_अङ्क_ACCESS
  {
    यदि (BE == (__BYTE_ORDER == __BIG_ENDIAN))
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

  constexpr चालक Type () अचर {
#यदि HB_FAST_अङ्क_ACCESS
    फर्काऊ (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint64_t) *((अचर hb_packed_t<uint64_t> *) v)
    :
      __builtin_bswap64 ((uint64_t) *((अचर hb_packed_t<uint64_t> *) v))
    ;
#अन्यथा
    फर्काऊ (BE ? (uint64_t (v[0]) << 56) : (uint64_t (v[0])      ))
	 + (BE ? (uint64_t (v[1]) << 48) : (uint64_t (v[1]) <<  8))
	 + (BE ? (uint64_t (v[2]) << 40) : (uint64_t (v[2]) << 16))
	 + (BE ? (uint64_t (v[3]) << 32) : (uint64_t (v[3]) << 24))
	 + (BE ? (uint64_t (v[4]) << 24) : (uint64_t (v[4]) << 32))
	 + (BE ? (uint64_t (v[5]) << 16) : (uint64_t (v[5]) << 40))
	 + (BE ? (uint64_t (v[6]) <<  8) : (uint64_t (v[6]) << 48))
	 + (BE ? (uint64_t (v[7])      ) : (uint64_t (v[7]) << 56));
#endif
  }
  गुप्त: uint8_t v[8];
};

/* Floats. */

ढाँचा <तार्किक BE, typename Type, पूर्णाङ्क Bytes>
संरचना HBFloat
{
  using IntType = typename std::conditional<Bytes == 4, uint32_t, uint64_t>::type;

  सार्वजनिक:
  HBFloat () = स्वभाव;

  HBFloat (Type V)
  {
#यदि HB_FAST_अङ्क_ACCESS
    {
	      यदि (BE == (__BYTE_ORDER == __BIG_ENDIAN))
	      {
	        *((hb_packed_t<Type> *) v) = V;
	        फर्काऊ;
	      }
	    }
#endif

    सङ्घ {
      hb_packed_t<Type> f;
      hb_packed_t<IntType> i;
    } u = {{V}};

    अचर HBInt<BE, IntType> I = (IntType) u.i;
    लागि (अचिन्हित i = 0; i < Bytes; i++)
      v[i] = I.v[i];
  }

  /* c++14 constexpr */ चालक Type () अचर
  {
#यदि HB_FAST_अङ्क_ACCESS
	    {
	      यदि (BE == (__BYTE_ORDER == __BIG_ENDIAN))
		फर्काऊ (Type) *((अचर hb_packed_t<Type> *) v);
	    }
#endif

    HBInt<BE, IntType> I;
    लागि (अचिन्हित i = 0; i < Bytes; i++)
      I.v[i] = v[i];

    सङ्घ {
      hb_packed_t<IntType> i;
      hb_packed_t<Type> f;
    } u = {{I}};

    फर्काऊ (Type) u.f;
  }
  गुप्त: uint8_t v[Bytes];
};


/* We want our rounding towards +infinity. */
स्थिर अन्तःस्थ द्विगुण
_hb_roundf (द्विगुण x) { फर्काऊ न्यूनमान (x + .5); }

स्थिर अन्तःस्थ भिन्न
_hb_roundf (भिन्न x) { फर्काऊ न्यूनमान ((द्विगुण) x + .5); }

#define roundf(x) _hb_roundf(x)

स्थिर अन्तःस्थ शून्य
hb_sincos (भिन्न rotation, भिन्न &s, भिन्न &c)
{
#ifdef HAVE_SINCOSF
  sincosf (rotation, &s, &c);
#अन्यथा
  c = cosf (rotation);
  s = sinf (rotation);
#endif
}
स्थिर अन्तःस्थ शून्य
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
  चालक () (T&& v) अचर HB_AUTO_RETURN ( std::forward<T> (v) )
}
HB_FUNCOBJ (hb_identity);
संरचना
{
  /* Like identity(), but only retains lvalue-references.  Rvalues are returned as rvalues. */
  ढाँचा <typename T> constexpr T&
  चालक () (T& v) अचर { फर्काऊ v; }

  ढाँचा <typename T> constexpr hb_remove_reference<T>
  चालक () (T&& v) अचर { फर्काऊ v; }
}
HB_FUNCOBJ (hb_lidentity);
संरचना
{
  /* Like identity(), but always returns rvalue. */
  ढाँचा <typename T> constexpr hb_remove_reference<T>
  चालक () (T&& v) अचर { फर्काऊ v; }
}
HB_FUNCOBJ (hb_ridentity);

संरचना
{
  ढाँचा <typename T> constexpr तार्किक
  चालक () (T&& v) अचर { फर्काऊ तार्किक (std::forward<T> (v)); }
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

स्थिर अन्तःस्थ uint64_t fasthash64(अचर शून्य *buf, माप_प्रकार len, uint64_t seed)
{
	संरचना __attribute__((packed)) packed_uint64_t { uint64_t v; };
	अचर uint64_t    m = 0x880355f21e6d1965ULL;
	अचर packed_uint64_t *pos = (अचर packed_uint64_t *)buf;
	अचर packed_uint64_t *end = pos + (len / 8);
	अचर अचिन्हित वर्ण *pos2;
	uint64_t h = seed ^ (len * m);
	uint64_t v;

#ifndef HB_OPTIMIZE_SIZE
	यदि (((uintptr_t) pos & 7) == 0)
	{
	  जबसम्म (pos != end)
	  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
	    v  = * (अचर uint64_t *) (pos++);
#pragma GCC diagnostic झिक
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}
	अन्यथा
#endif
	{
	  जबसम्म (pos != end)
	  {
	    v  = pos++->v;
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}

	pos2 = (अचर अचिन्हित वर्ण*)pos;
	v = 0;

	विकल्प (len & 7) {
	पक्ष 7: v ^= (uint64_t)pos2[6] << 48; HB_FALLTHROUGH;
	पक्ष 6: v ^= (uint64_t)pos2[5] << 40; HB_FALLTHROUGH;
	पक्ष 5: v ^= (uint64_t)pos2[4] << 32; HB_FALLTHROUGH;
	पक्ष 4: v ^= (uint64_t)pos2[3] << 24; HB_FALLTHROUGH;
	पक्ष 3: v ^= (uint64_t)pos2[2] << 16; HB_FALLTHROUGH;
	पक्ष 2: v ^= (uint64_t)pos2[1] <<  8; HB_FALLTHROUGH;
	पक्ष 1: v ^= (uint64_t)pos2[0];
		h ^= fasthash_mix(v);
		h *= m;
	}

	फर्काऊ fasthash_mix(h);
}

स्थिर अन्तःस्थ uint32_t fasthash32(अचर शून्य *buf, माप_प्रकार len, uint32_t seed)
{
	// the following trick converts the 64-bit hashcode to Fermat
	// residue, which shall retain information from both the higher
	// and lower parts of hashcode.
        uint64_t h = fasthash64(buf, len, seed);
	फर्काऊ h - (h >> 32);
}

संरचना
{
  गुप्त:

  ढाँचा <typename T> constexpr स्वतः
  impl (अचर T& v, hb_priority<2>) अचर HB_RETURN (uint32_t, hb_deref (v).hash ())

  // Horrible: std:hash() of integers seems to be identity in gcc / clang?!
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  //
  // For performance characteristics see:
  // https://github.com/harfbuzz/harfbuzz/pull/4228#issuecomment-1565079537
  ढाँचा <typename T,
	    hb_enable_if (std::is_integral<T>::value && परिमाण (T) <= परिमाण (uint32_t))> constexpr स्वतः
  impl (अचर T& v, hb_priority<1>) अचर HB_RETURN (uint32_t, (uint32_t) v * 2654435761u /* Knuh's multiplicative hash */)
  ढाँचा <typename T,
	    hb_enable_if (std::is_integral<T>::value && परिमाण (T) > परिमाण (uint32_t))> constexpr स्वतः
  impl (अचर T& v, hb_priority<1>) अचर HB_RETURN (uint32_t, (uint32_t) (v ^ (v >> 32)) * 2654435761u /* Knuth's multiplicative hash */)

  ढाँचा <typename T,
	    hb_enable_if (std::is_floating_point<T>::value)> constexpr स्वतः
  impl (अचर T& v, hb_priority<1>) अचर HB_RETURN (uint32_t, fasthash32 (std::addressof (v), परिमाण (T), 0xf437ffe6))

  ढाँचा <typename T> constexpr स्वतः
  impl (अचर T& v, hb_priority<0>) अचर HB_RETURN (uint32_t, std::hash<hb_decay<decltype (hb_deref (v))>>{} (hb_deref (v)))

  सार्वजनिक:

  ढाँचा <typename T> constexpr स्वतः
  चालक () (अचर T& v) अचर HB_RETURN (uint32_t, impl (v, hb_prioritize))
}
HB_FUNCOBJ (hb_hash);


संरचना
{
  गुप्त:

  /* Pointer-to-member-function. */
  ढाँचा <typename Appl, typename T, typename ...Ts> स्वतः
  impl (Appl&& a, hb_priority<2>, T &&v, Ts&&... ds) अचर HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v)).*std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  /* Pointer-to-member. */
  ढाँचा <typename Appl, typename T> स्वतः
  impl (Appl&& a, hb_priority<1>, T &&v) अचर HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v))).*std::forward<Appl> (a))

  /* Operator(). */
  ढाँचा <typename Appl, typename ...Ts> स्वतः
  impl (Appl&& a, hb_priority<0>, Ts&&... ds) अचर HB_AUTO_RETURN
  (hb_deref (std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  सार्वजनिक:

  ढाँचा <typename Appl, typename ...Ts> स्वतः
  चालक () (Appl&& a, Ts&&... ds) अचर HB_AUTO_RETURN
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
    फर्काऊ hb_invoke (std::forward<Appl> (a),
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
    फर्काऊ hb_invoke (std::forward<Appl> (a),
		      std::forward<T0> (d0),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }

  गुप्त:
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
  decltype(स्वतः) चालक () (_T&& _v) अचर \
  { फर्काऊ hb_partial<Pos> (एतत्, std::forward<_T> (_v)); } \
  static_assert (सत्य, "")
#अन्यथा
/* https://github.com/harfbuzz/harfbuzz/issues/1724 */
#define HB_PARTIALIZE(Pos) \
  ढाँचा <typename _T> \
  स्वतः चालक () (_T&& _v) अचर HB_AUTO_RETURN \
  (hb_partial<Pos> (+एतत्, std::forward<_T> (_v))) \
  static_assert (सत्य, "")
#endif


संरचना
{
  गुप्त:

  ढाँचा <typename Pred, typename Val> स्वतः
  impl (Pred&& p, Val &&v, hb_priority<1>) अचर HB_AUTO_RETURN
  (
    hb_deref (std::forward<Pred> (p)).has (std::forward<Val> (v))
  )

  ढाँचा <typename Pred, typename Val> स्वतः
  impl (Pred&& p, Val &&v, hb_priority<0>) अचर HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Pred> (p),
	       std::forward<Val> (v))
  )

  सार्वजनिक:

  ढाँचा <typename Pred, typename Val> स्वतः
  चालक () (Pred&& p, Val &&v) अचर HB_RETURN (तार्किक,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_has);

संरचना
{
  गुप्त:

  ढाँचा <typename Pred, typename Val> स्वतः
  impl (Pred&& p, Val &&v, hb_priority<1>) अचर HB_AUTO_RETURN
  (
    hb_has (std::forward<Pred> (p),
	    std::forward<Val> (v))
  )

  ढाँचा <typename Pred, typename Val> स्वतः
  impl (Pred&& p, Val &&v, hb_priority<0>) अचर HB_AUTO_RETURN
  (
    std::forward<Pred> (p) == std::forward<Val> (v)
  )

  सार्वजनिक:

  ढाँचा <typename Pred, typename Val> स्वतः
  चालक () (Pred&& p, Val &&v) अचर HB_RETURN (तार्किक,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_match);

संरचना
{
  गुप्त:

  ढाँचा <typename Proj, typename Val> स्वतः
  impl (Proj&& f, Val &&v, hb_priority<2>) अचर HB_AUTO_RETURN
  (
    hb_deref (std::forward<Proj> (f)).get (std::forward<Val> (v))
  )

  ढाँचा <typename Proj, typename Val> स्वतः
  impl (Proj&& f, Val &&v, hb_priority<1>) अचर HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Proj> (f),
	       std::forward<Val> (v))
  )

  ढाँचा <typename Proj, typename Val> स्वतः
  impl (Proj&& f, Val &&v, hb_priority<0>) अचर HB_AUTO_RETURN
  (
    std::forward<Proj> (f)[std::forward<Val> (v)]
  )

  सार्वजनिक:

  ढाँचा <typename Proj, typename Val> स्वतः
  चालक () (Proj&& f, Val &&v) अचर HB_AUTO_RETURN
  (
    impl (std::forward<Proj> (f),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_get);

संरचना
{
  गुप्त:

  ढाँचा <typename T1, typename T2> स्वतः
  impl (T1&& v1, T2 &&v2, hb_priority<3>) अचर HB_AUTO_RETURN
  (
    std::forward<T2> (v2).cmp (std::forward<T1> (v1)) == 0
  )

  ढाँचा <typename T1, typename T2> स्वतः
  impl (T1&& v1, T2 &&v2, hb_priority<2>) अचर HB_AUTO_RETURN
  (
    std::forward<T1> (v1).cmp (std::forward<T2> (v2)) == 0
  )

  ढाँचा <typename T1, typename T2> स्वतः
  impl (T1&& v1, T2 &&v2, hb_priority<1>) अचर HB_AUTO_RETURN
  (
    std::forward<T1> (v1) == std::forward<T2> (v2)
  )

  ढाँचा <typename T1, typename T2> स्वतः
  impl (T1&& v1, T2 &&v2, hb_priority<0>) अचर HB_AUTO_RETURN
  (
    std::forward<T2> (v2) == std::forward<T1> (v1)
  )

  सार्वजनिक:

  ढाँचा <typename T1, typename T2> स्वतः
  चालक () (T1&& v1, T2 &&v2) अचर HB_AUTO_RETURN
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
  चालक () (T& a, T& b) अचर
  {
    using std::swap; // allow ADL
    swap (a, b);
  }
}
HB_FUNCOBJ (hb_swap);


ढाँचा <typename T1, typename T2>
संरचना hb_pair_t
{
  प्रकारनिर्देश T1 first_t;
  प्रकारनिर्देश T2 second_t;
  प्रकारनिर्देश hb_pair_t<T1, T2> pair_t;

  ढाँचा <typename U1 = T1, typename U2 = T2,
	    hb_enable_if (std::is_default_constructible<U1>::value &&
			  std::is_default_constructible<U2>::value)>
  hb_pair_t () : first (), second () {}
  hb_pair_t (T1 a, T2 b) : first (std::forward<T1> (a)), second (std::forward<T2> (b)) {}

  ढाँचा <typename Q1, typename Q2,
	    hb_enable_if (hb_is_convertible (T1, Q1) &&
			  hb_is_convertible (T2, Q2))>
  चालक hb_pair_t<Q1, Q2> () { फर्काऊ hb_pair_t<Q1, Q2> (first, second); }

  hb_pair_t<T1, T2> reverse () अचर
  { फर्काऊ hb_pair_t<T1, T2> (second, first); }

  तार्किक चालक == (अचर pair_t& o) अचर { फर्काऊ first == o.first && second == o.second; }
  तार्किक चालक != (अचर pair_t& o) अचर { फर्काऊ !(*एतत् == o); }
  तार्किक चालक < (अचर pair_t& o) अचर { फर्काऊ first < o.first || (first == o.first && second < o.second); }
  तार्किक चालक >= (अचर pair_t& o) अचर { फर्काऊ !(*एतत् < o); }
  तार्किक चालक > (अचर pair_t& o) अचर { फर्काऊ first > o.first || (first == o.first && second > o.second); }
  तार्किक चालक <= (अचर pair_t& o) अचर { फर्काऊ !(*एतत् > o); }

  स्थिर पूर्णाङ्क cmp (अचर शून्य *pa, अचर शून्य *pb)
  {
    pair_t *a = (pair_t *) pa;
    pair_t *b = (pair_t *) pb;

    यदि (a->first < b->first) फर्काऊ -1;
    यदि (a->first > b->first) फर्काऊ +1;
    यदि (a->second < b->second) फर्काऊ -1;
    यदि (a->second > b->second) फर्काऊ +1;
    फर्काऊ 0;
  }

  मित्र शून्य swap (hb_pair_t& a, hb_pair_t& b) noexcept
  {
    hb_swap (a.first, b.first);
    hb_swap (a.second, b.second);
  }


  T1 first;
  T2 second;
};
ढाँचा <typename T1, typename T2> स्थिर अन्तःस्थ hb_pair_t<T1, T2>
hb_pair (T1&& a, T2&& b) { फर्काऊ hb_pair_t<T1, T2> (a, b); }

प्रकारनिर्देश hb_pair_t<hb_codepoint_t, hb_codepoint_t> hb_codepoint_pair_t;

संरचना
{
  ढाँचा <typename Pair> constexpr typename Pair::first_t
  चालक () (अचर Pair& pair) अचर { फर्काऊ pair.first; }
}
HB_FUNCOBJ (hb_first);

संरचना
{
  ढाँचा <typename Pair> constexpr typename Pair::second_t
  चालक () (अचर Pair& pair) अचर { फर्काऊ pair.second; }
}
HB_FUNCOBJ (hb_second);

/* Note.  In min/max impl, we can use hb_type_identity<T> for second argument.
 * However, that would silently convert between different-signedness integers.
 * Instead we accept two different types, such that compiler can err if
 * comparing integers of different signedness. */
संरचना
{
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (T&& a, T2&& b) अचर HB_AUTO_RETURN
  (a <= b ? a : b)
}
HB_FUNCOBJ (hb_min);
संरचना
{
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (T&& a, T2&& b) अचर HB_AUTO_RETURN
  (a >= b ? a : b)
}
HB_FUNCOBJ (hb_max);
संरचना
{
  ढाँचा <typename T, typename T2, typename T3> constexpr स्वतः
  चालक () (T&& x, T2&& min, T3&& max) अचर HB_AUTO_RETURN
  (hb_min (hb_max (std::forward<T> (x), std::forward<T2> (min)), std::forward<T3> (max)))
}
HB_FUNCOBJ (hb_clamp);

/* Signed saturating arithmetic. */
ढाँचा <typename T>
स्थिर HB_ALWAYS_INLINE T
hb_saturate_add (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#यदि hb_has_builtin(__builtin_add_overflow)
  T result;
  यदि (likely (!__builtin_add_overflow (a, b, &result)))
    फर्काऊ result;
#अन्यथा
  यदि (b > 0)
  {
    यदि (likely (a <= hb_int_max (T) - b))
      फर्काऊ a + b;
  }
  अन्यथा यदि (likely (a >= hb_int_min (T) - b))
    फर्काऊ a + b;
#endif

  फर्काऊ a < 0 ? hb_int_min (T) : hb_int_max (T);
}

ढाँचा <typename T>
स्थिर HB_ALWAYS_INLINE T
hb_saturate_sub (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#यदि hb_has_builtin(__builtin_sub_overflow)
  T result;
  यदि (likely (!__builtin_sub_overflow (a, b, &result)))
    फर्काऊ result;
#अन्यथा
  यदि (b > 0)
  {
    यदि (likely (a >= hb_int_min (T) + b))
      फर्काऊ a - b;
  }
  अन्यथा यदि (likely (a <= hb_int_max (T) + b))
    फर्काऊ a - b;
#endif

  फर्काऊ b < 0 ? hb_int_max (T) : hb_int_min (T);
}

ढाँचा <typename T>
स्थिर HB_ALWAYS_INLINE T
hb_saturate_neg (T a)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  फर्काऊ hb_saturate_sub ((T) 0, a);
}

/* Convert a floating-point value to integer type T, saturating to T's
 * range instead of relying on the undefined behavior of an out-of-range
 * float-to-int conversion.  NaN saturates to the minimum of T. */
ढाँचा <typename T>
स्थिर HB_ALWAYS_INLINE T
hb_clamp_to (द्विगुण v)
{
  फर्काऊ (T) hb_clamp (v,
		       (द्विगुण) hb_int_min (T),
		       (द्विगुण) hb_int_max (T));
}

ढाँचा <typename T>
स्थिर HB_ALWAYS_INLINE T
hb_clamp_to (भिन्न v)
{
  /* Widen explicitly: float cannot represent all integer bounds exactly,
   * so the clamp itself is done in double. */
  फर्काऊ hb_clamp_to<T> ((द्विगुण) v);
}

ढाँचा <typename T>
स्थिर HB_ALWAYS_INLINE T
hb_clamp_to (int64_t v)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  फर्काऊ (T) hb_clamp (v,
		       (int64_t) hb_int_min (T),
		       (int64_t) hb_int_max (T));
}

/*
 * Bithacks.
 */

/* Return the number of 1 bits in a uint8_t; faster than hb_popcount() */
स्थिर अन्तःस्थ अचिन्हित
hb_popcount8 (uint8_t v)
{
  स्थिर अचर uint8_t popcount4[16] = {
    0, 1, 1, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 2, 3, 3, 4
  };
  फर्काऊ popcount4[v & 0xF] + popcount4[v >> 4];
}

/* Return the number of 1 bits in v. */
ढाँचा <typename T>
स्थिर अन्तःस्थ अचिन्हित पूर्णाङ्क
hb_popcount (T v)
{
#यदि hb_has_builtin(__builtin_popcount)
  यदि (परिमाण (T) <= परिमाण (अचिन्हित पूर्णाङ्क))
    फर्काऊ __builtin_popcount (v);
#endif

#यदि hb_has_builtin(__builtin_popcountl)
  यदि (परिमाण (T) <= परिमाण (अचिन्हित दीर्घ))
    फर्काऊ __builtin_popcountl (v);
#endif

#यदि hb_has_builtin(__builtin_popcountll)
  यदि (परिमाण (T) <= परिमाण (अचिन्हित दीर्घ दीर्घ))
    फर्काऊ __builtin_popcountll (v);
#endif

  यदि (परिमाण (T) <= 4)
  {
    /* "HACKMEM 169" */
    uint32_t y;
    y = (v >> 1) &033333333333;
    y = v - y - ((y >>1) & 033333333333);
    फर्काऊ (((y + (y >> 3)) & 030707070707) % 077);
  }

  यदि (परिमाण (T) == 8)
  {
    uint64_t y = (uint64_t) v;
    y -= ((y >> 1) & 0x5555555555555555ull);
    y = (y & 0x3333333333333333ull) + (y >> 2 & 0x3333333333333333ull);
    फर्काऊ ((y + (y >> 4)) & 0xf0f0f0f0f0f0f0full) * 0x101010101010101ull >> 56;
  }

  यदि (परिमाण (T) == 16)
  {
    अचिन्हित पूर्णाङ्क shift = 64;
    फर्काऊ hb_popcount<uint64_t> ((uint64_t) v) + hb_popcount ((uint64_t) (v >> shift));
  }

  प्रतिज्ञा (0);
  फर्काऊ 0; /* Shut up stupid compiler. */
}

/* Returns the number of bits needed to store number */
ढाँचा <typename T>
स्थिर अन्तःस्थ अचिन्हित पूर्णाङ्क
hb_bit_storage (T v)
{
  यदि (unlikely (!v)) फर्काऊ 0;

#यदि hb_has_builtin(__builtin_clz)
  यदि (परिमाण (T) <= परिमाण (अचिन्हित पूर्णाङ्क))
    फर्काऊ परिमाण (अचिन्हित पूर्णाङ्क) * 8 - __builtin_clz (v);
#endif

#यदि hb_has_builtin(__builtin_clzl)
  यदि (परिमाण (T) <= परिमाण (अचिन्हित दीर्घ))
    फर्काऊ परिमाण (अचिन्हित दीर्घ) * 8 - __builtin_clzl (v);
#endif

#यदि hb_has_builtin(__builtin_clzll)
  यदि (परिमाण (T) <= परिमाण (अचिन्हित दीर्घ दीर्घ))
    फर्काऊ परिमाण (अचिन्हित दीर्घ दीर्घ) * 8 - __builtin_clzll (v);
#endif

#यदि (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  यदि (परिमाण (T) <= परिमाण (अचिन्हित पूर्णाङ्क))
  {
    अचिन्हित दीर्घ where;
    _BitScanReverse (&where, v);
    फर्काऊ 1 + where;
  }
# यदि defined(_WIN64)
  यदि (परिमाण (T) <= 8)
  {
    अचिन्हित दीर्घ where;
    _BitScanReverse64 (&where, v);
    फर्काऊ 1 + where;
  }
# endif
#endif

  यदि (परिमाण (T) <= 4)
  {
    /* "bithacks" */
    अचर अचिन्हित पूर्णाङ्क b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    अचर अचिन्हित पूर्णाङ्क S[] = {1, 2, 4, 8, 16};
    अचिन्हित पूर्णाङ्क r = 0;
    लागि (पूर्णाङ्क i = 4; i >= 0; i--)
      यदि (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    फर्काऊ r + 1;
  }
  यदि (परिमाण (T) <= 8)
  {
    /* "bithacks" */
    अचर uint64_t b[] = {0x2ULL, 0xCULL, 0xF0ULL, 0xFF00ULL, 0xFFFF0000ULL, 0xFFFFFFFF00000000ULL};
    अचर अचिन्हित पूर्णाङ्क S[] = {1, 2, 4, 8, 16, 32};
    अचिन्हित पूर्णाङ्क r = 0;
    लागि (पूर्णाङ्क i = 5; i >= 0; i--)
      यदि (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    फर्काऊ r + 1;
  }
  यदि (परिमाण (T) == 16)
  {
    अचिन्हित पूर्णाङ्क shift = 64;
    फर्काऊ (v >> shift) ? hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift :
			  hb_bit_storage<uint64_t> ((uint64_t) v);
  }

  प्रतिज्ञा (0);
  फर्काऊ 0; /* Shut up stupid compiler. */
}

/* Returns the number of zero bits in the least significant side of v */
ढाँचा <typename T>
स्थिर अन्तःस्थ अचिन्हित पूर्णाङ्क
hb_ctz (T v)
{
  यदि (unlikely (!v)) फर्काऊ 8 * परिमाण (T);

#यदि hb_has_builtin(__builtin_ctz)
  यदि (परिमाण (T) <= परिमाण (अचिन्हित पूर्णाङ्क))
    फर्काऊ __builtin_ctz (v);
#endif

#यदि hb_has_builtin(__builtin_ctzl)
  यदि (परिमाण (T) <= परिमाण (अचिन्हित दीर्घ))
    फर्काऊ __builtin_ctzl (v);
#endif

#यदि hb_has_builtin(__builtin_ctzll)
  यदि (परिमाण (T) <= परिमाण (अचिन्हित दीर्घ दीर्घ))
    फर्काऊ __builtin_ctzll (v);
#endif

#यदि (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  यदि (परिमाण (T) <= परिमाण (अचिन्हित पूर्णाङ्क))
  {
    अचिन्हित दीर्घ where;
    _BitScanForward (&where, v);
    फर्काऊ where;
  }
# यदि defined(_WIN64)
  यदि (परिमाण (T) <= 8)
  {
    अचिन्हित दीर्घ where;
    _BitScanForward64 (&where, v);
    फर्काऊ where;
  }
# endif
#endif

  यदि (परिमाण (T) <= 4)
  {
    /* "bithacks" */
    अचिन्हित पूर्णाङ्क c = 32;
    v &= - (int32_t) v;
    यदि (v) c--;
    यदि (v & 0x0000FFFF) c -= 16;
    यदि (v & 0x00FF00FF) c -= 8;
    यदि (v & 0x0F0F0F0F) c -= 4;
    यदि (v & 0x33333333) c -= 2;
    यदि (v & 0x55555555) c -= 1;
    फर्काऊ c;
  }
  यदि (परिमाण (T) <= 8)
  {
    /* "bithacks" */
    अचिन्हित पूर्णाङ्क c = 64;
    v &= - (int64_t) (v);
    यदि (v) c--;
    यदि (v & 0x00000000FFFFFFFFULL) c -= 32;
    यदि (v & 0x0000FFFF0000FFFFULL) c -= 16;
    यदि (v & 0x00FF00FF00FF00FFULL) c -= 8;
    यदि (v & 0x0F0F0F0F0F0F0F0FULL) c -= 4;
    यदि (v & 0x3333333333333333ULL) c -= 2;
    यदि (v & 0x5555555555555555ULL) c -= 1;
    फर्काऊ c;
  }
  यदि (परिमाण (T) == 16)
  {
    अचिन्हित पूर्णाङ्क shift = 64;
    फर्काऊ (uint64_t) v ? hb_bit_storage<uint64_t> ((uint64_t) v) :
			  hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift;
  }

  प्रतिज्ञा (0);
  फर्काऊ 0; /* Shut up stupid compiler. */
}


/*
 * Tiny stuff.
 */

/* ASCII tag/character handling */
स्थिर अन्तःस्थ तार्किक ISALPHA (अचिन्हित वर्ण c)
{ फर्काऊ (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
स्थिर अन्तःस्थ तार्किक ISALNUM (अचिन्हित वर्ण c)
{ फर्काऊ (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }
स्थिर अन्तःस्थ तार्किक ISSPACE (अचिन्हित वर्ण c)
{ फर्काऊ c == ' ' || c =='\f'|| c =='\n'|| c =='\r'|| c =='\t'|| c =='\v'; }
स्थिर अन्तःस्थ अचिन्हित वर्ण TOUPPER (अचिन्हित वर्ण c)
{ फर्काऊ (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c; }
स्थिर अन्तःस्थ अचिन्हित वर्ण TOLOWER (अचिन्हित वर्ण c)
{ फर्काऊ (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }
स्थिर अन्तःस्थ तार्किक ISHEX (अचिन्हित वर्ण c)
{ फर्काऊ (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
स्थिर अन्तःस्थ अचिन्हित वर्ण TOHEX (uint8_t c)
{ फर्काऊ (c & 0xF) <= 9 ? (c & 0xF) + '0' : (c & 0xF) + 'a' - 10; }
स्थिर अन्तःस्थ uint8_t FROMHEX (अचिन्हित वर्ण c)
{ फर्काऊ (c >= '0' && c <= '9') ? c - '0' : TOLOWER (c) - 'a' + 10; }

स्थिर अन्तःस्थ अचिन्हित पूर्णाङ्क DIV_CEIL (अचर अचिन्हित पूर्णाङ्क a, अचिन्हित पूर्णाङ्क b)
{ फर्काऊ (a + (b - 1)) / b; }


#undef  ARRAY_LENGTH
ढाँचा <typename Type, अचिन्हित पूर्णाङ्क n>
स्थिर अन्तःस्थ अचिन्हित पूर्णाङ्क ARRAY_LENGTH (अचर Type (&)[n]) { फर्काऊ n; }
/* A const version, but does not detect erratically being called on pointers. */
#define ARRAY_LENGTH_CONST(__array) ((चिन्हित पूर्णाङ्क) (परिमाण (__array) / परिमाण (__array[0])))


स्थिर अन्तःस्थ शून्य *
hb_memcpy (शून्य *__restrict dst, अचर शून्य *__restrict src, माप_प्रकार len)
{
  /* It's illegal to pass 0 as size to memcpy. */
  यदि (unlikely (!len)) फर्काऊ dst;
  फर्काऊ स_नकल (dst, src, len);
}

स्थिर अन्तःस्थ पूर्णाङ्क
hb_memcmp (अचर शून्य *a, अचर शून्य *b, अचिन्हित पूर्णाङ्क len)
{
  /* It's illegal to pass NULL to memcmp(), even if len is zero.
   * So, wrap it.
   * https://sourceware.org/bugzilla/show_bug.cgi?id=23878 */
  यदि (unlikely (!len)) फर्काऊ 0;
  फर्काऊ स_भेद (a, b, len);
}

स्थिर अन्तःस्थ शून्य *
hb_memset (शून्य *s, पूर्णाङ्क c, अचिन्हित पूर्णाङ्क n)
{
  /* It's illegal to pass NULL to memset(), even if n is zero. */
  यदि (unlikely (!n)) फर्काऊ s;
  फर्काऊ स_रखो (s, c, n);
}

स्थिर अन्तःस्थ अचिन्हित पूर्णाङ्क
hb_ceil_to_4 (अचिन्हित पूर्णाङ्क v)
{
  फर्काऊ ((v - 1) | 3) + 1;
}

ढाँचा <typename T> स्थिर अन्तःस्थ तार्किक
hb_in_range (T u, T lo, T hi)
{
  static_assert (!std::is_signed<T>::value, "");

  /* The casts below are important as if T is smaller than int,
   * the subtract results will become a signed int! */
  फर्काऊ (T)(u - lo) <= (T)(hi - lo);
}
ढाँचा <typename T> स्थिर अन्तःस्थ तार्किक
hb_in_ranges (T u, T lo1, T hi1)
{
  फर्काऊ hb_in_range (u, lo1, hi1);
}
ढाँचा <typename T, typename ...Ts> स्थिर अन्तःस्थ तार्किक
hb_in_ranges (T u, T lo1, T hi1, Ts... ds)
{
  फर्काऊ hb_in_range<T> (u, lo1, hi1) || hb_in_ranges<T> (u, ds...);
}


/*
 * Overflow checking.
 */

स्थिर अन्तःस्थ तार्किक
hb_unsigned_mul_overflows (अचिन्हित पूर्णाङ्क गन्ती, अचिन्हित पूर्णाङ्क size, अचिन्हित *result = nullptr)
{
#यदि hb_has_builtin(__builtin_mul_overflow)
  अचिन्हित थाक_result;
  यदि (!result)
    result = &थाक_result;
  फर्काऊ __builtin_mul_overflow (गन्ती, size, result);
#endif

  यदि (result)
    *result = गन्ती * size;
  फर्काऊ (size > 0) && (गन्ती >= ((अचिन्हित पूर्णाङ्क) -1) / size);
}

स्थिर अन्तःस्थ तार्किक
hb_unsigned_add_overflows (अचिन्हित पूर्णाङ्क a, अचिन्हित पूर्णाङ्क b, अचिन्हित *result = nullptr)
{
#यदि hb_has_builtin(__builtin_add_overflow)
  अचिन्हित थाक_result;
  यदि (!result)
    result = &थाक_result;
  फर्काऊ __builtin_add_overflow (a, b, result);
#endif

  यदि (result)
    *result = a + b;
  फर्काऊ b > (अचिन्हित पूर्णाङ्क) -1 - a;
}

/* Saturating arithmetic on size_t.  On platforms where size_t is wider than
 * unsigned int (i.e. 64-bit), the inputs to get_size()-style computations
 * (counts and static_sizes which are at most 32-bit) cannot overflow size_t,
 * so these reduce to plain arithmetic.  On 32-bit platforms (size_t is
 * unsigned int), they saturate to SIZE_MAX so sanitize/serialize callers
 * naturally reject the resulting size as out-of-range. */

स्थिर अन्तःस्थ माप_प्रकार
hb_unsigned_mul_saturate (माप_प्रकार a, माप_प्रकार b)
{
  यदि (परिमाण (माप_प्रकार) > परिमाण (अचिन्हित पूर्णाङ्क))
    फर्काऊ a * b;
#यदि hb_has_builtin(__builtin_mul_overflow)
  माप_प्रकार result;
  यदि (__builtin_mul_overflow (a, b, &result))
    फर्काऊ (माप_प्रकार) -1;
  फर्काऊ result;
#अन्यथा
  यदि (b > 0 && a > ((माप_प्रकार) -1) / b) फर्काऊ (माप_प्रकार) -1;
  फर्काऊ a * b;
#endif
}

स्थिर अन्तःस्थ माप_प्रकार
hb_unsigned_add_saturate (माप_प्रकार a, माप_प्रकार b)
{
  यदि (परिमाण (माप_प्रकार) > परिमाण (अचिन्हित पूर्णाङ्क))
    फर्काऊ a + b;
#यदि hb_has_builtin(__builtin_add_overflow)
  माप_प्रकार result;
  यदि (__builtin_add_overflow (a, b, &result))
    फर्काऊ (माप_प्रकार) -1;
  फर्काऊ result;
#अन्यथा
  यदि (b > ((माप_प्रकार) -1) - a) फर्काऊ (माप_प्रकार) -1;
  फर्काऊ a + b;
#endif
}

/* Variadic forms: fold left across all arguments. */
ढाँचा <typename ...Ts>
स्थिर अन्तःस्थ माप_प्रकार
hb_unsigned_mul_saturate (माप_प्रकार a, माप_प्रकार b, माप_प्रकार c, Ts... rest)
{ फर्काऊ hb_unsigned_mul_saturate (hb_unsigned_mul_saturate (a, b), c, rest...); }

ढाँचा <typename ...Ts>
स्थिर अन्तःस्थ माप_प्रकार
hb_unsigned_add_saturate (माप_प्रकार a, माप_प्रकार b, माप_प्रकार c, Ts... rest)
{ फर्काऊ hb_unsigned_add_saturate (hb_unsigned_add_saturate (a, b), c, rest...); }

/* Saturating mul-add: a * b + c.  Covers the dominant get_size() pattern
 * `count * static_size + min_size`. */
स्थिर अन्तःस्थ माप_प्रकार
hb_unsigned_mul_add_saturate (माप_प्रकार a, माप_प्रकार b, माप_प्रकार c)
{ फर्काऊ hb_unsigned_add_saturate (hb_unsigned_mul_saturate (a, b), c); }


/*
 * Sort and search.
 */

ढाँचा <typename K, typename V, typename ...Ts>
स्थिर पूर्णाङ्क
_hb_cmp_method (अचर शून्य *pkey, अचर शून्य *pval, Ts... ds)
{
  अचर K& key = * (अचर K*) pkey;
  अचर V& val = * (अचर V*) pval;

  फर्काऊ val.cmp (key, ds...);
}

ढाँचा <typename K, typename V>
स्थिर पूर्णाङ्क
_hb_cmp_operator (अचर शून्य *pkey, अचर शून्य *pval)
{
  अचर K& key = * (अचर K*) pkey;
  अचर V& val = * (अचर V*) pval;

  यदि (key < val) फर्काऊ -1;
  यदि (key > val) फर्काऊ  1;
  फर्काऊ 0;
}

ढाँचा <typename V, typename K, typename ...Ts>
HB_HOT
स्थिर अन्तःस्थ तार्किक
hb_bsearch_impl (अचिन्हित *pos, /* Out */
		 अचर K& key,
		 V* base, माप_प्रकार nmemb, माप_प्रकार stride,
		 पूर्णाङ्क (*compar)(अचर शून्य *साँचो_3, अचर शून्य *_item, Ts... _ds),
		 Ts... ds)
{
  /* This is our *only* bsearch implementation. */

  पूर्णाङ्क min = 0, max = (पूर्णाङ्क) nmemb - 1;
  जबसम्म (min <= max)
  {
    पूर्णाङ्क mid = ((अचिन्हित पूर्णाङ्क) min + (अचिन्हित पूर्णाङ्क) max) / 2;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    V* p = (V*) (((अचर वर्ण *) base) + (mid * stride));
#pragma GCC diagnostic झिक
    पूर्णाङ्क c = compar ((अचर शून्य *) std::addressof (key), (अचर शून्य *) p, ds...);
    यदि (c < 0)
      max = mid - 1;
    अन्यथा यदि (c > 0)
      min = mid + 1;
    अन्यथा
    {
      *pos = mid;
      फर्काऊ सत्य;
    }
  }
  *pos = min;
  फर्काऊ असत्य;
}

ढाँचा <typename V, typename K>
स्थिर अन्तःस्थ V*
hb_bsearch (अचर K& key, V* base,
	    माप_प्रकार nmemb, माप_प्रकार stride = परिमाण (V),
	    पूर्णाङ्क (*compar)(अचर शून्य *साँचो_3, अचर शून्य *_item) = _hb_cmp_method<K, V>)
{
  अचिन्हित pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  फर्काऊ hb_bsearch_impl (&pos, key, base, nmemb, stride, compar) ?
	 (V*) (((अचर वर्ण *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic झिक
}
ढाँचा <typename V, typename K, typename ...Ts>
स्थिर अन्तःस्थ V*
hb_bsearch (अचर K& key, V* base,
	    माप_प्रकार nmemb, माप_प्रकार stride,
	    पूर्णाङ्क (*compar)(अचर शून्य *साँचो_3, अचर शून्य *_item, Ts... _ds),
	    Ts... ds)
{
  अचिन्हित pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  फर्काऊ hb_bsearch_impl (&pos, key, base, nmemb, stride, compar, ds...) ?
	 (V*) (((अचर वर्ण *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic झिक
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
स्थिर अन्तःस्थ शून्य
hb_qsort_loop (T *base, माप_प्रकार nel, Compar compar)
{
  जबसम्म (nel > 24)
  {
    T *last = base + nel - 1;
    T *mid = base + nel / 2;

    /* Median-of-three pivot, parked at last-1. */
    यदि (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    यदि (compar (*mid, *last) > 0)
    {
      hb_swap (*mid, *last);
      यदि (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    }
    hb_swap (*mid, *(last - 1));
    T &pivot = *(last - 1);

    /* Two-way Hoare partition.  Inner loops are unguarded:
     * median-of-three left *base <= pivot and *last >= pivot,
     * which act as sentinels. */
    T *i = base, *j = last - 1;
    जबसम्म (सत्य)
    {
      जबसम्म (compar (*++i, pivot) < 0) {}
      जबसम्म (compar (*--j, pivot) > 0) {}
      यदि (i >= j) रोक;
      hb_swap (*i, *j);
    }
    hb_swap (*i, *(last - 1));

    /* Recurse on smaller side, loop on larger — bounds stack
     * depth at O(log n). */
    माप_प्रकार left  = (माप_प्रकार) (i - base);
    माप_प्रकार दायाँ = nel - left - 1;
    यदि (left < दायाँ)
    {
      hb_qsort_loop (base, left, compar);
      base = i + 1;
      nel  = दायाँ;
    }
    अन्यथा
    {
      hb_qsort_loop (i + 1, दायाँ, compar);
      nel  = left;
    }
  }
}

ढाँचा <typename T, typename Compar>
स्थिर अन्तःस्थ शून्य
hb_qsort_inline (T *base, माप_प्रकार nel, Compar compar)
{
  hb_qsort_loop (base, nel, compar);

  /* Single final insertion sort over the whole array.  After
   * the partitioning loop, every element is within the threshold
   * of its sorted position, so this pass is O(n * threshold). */
  T *end = base + nel;
  लागि (T *pi = base + 1; pi < end; pi++)
    लागि (T *pj = pi; pj > base && compar (pj[-1], pj[0]) > 0; pj--)
      hb_swap (pj[-1], pj[0]);
}

स्थिर अन्तःस्थ शून्य
hb_qsort (शून्य *base, माप_प्रकार nel, माप_प्रकार width,
	  पूर्णाङ्क (*compar)(अचर शून्य *_a, अचर शून्य *_b))
{
  क्विक (base, nel, width, compar);
}




ढाँचा <typename T, typename T2, typename T3 = पूर्णाङ्क> स्थिर अन्तःस्थ शून्य
hb_stable_sort (T *array, अचिन्हित पूर्णाङ्क len, पूर्णाङ्क(*compar)(अचर T2 *, अचर T2 *), T3 *array2 = nullptr)
{
  static_assert (hb_is_trivially_copy_assignable (T), "");
  static_assert (hb_is_trivially_copy_assignable (T3), "");

  लागि (अचिन्हित पूर्णाङ्क i = 1; i < len; i++)
  {
    अचिन्हित पूर्णाङ्क j = i;
    जबसम्म (j && compar (&array[j - 1], &array[i]) > 0)
      j--;
    यदि (i == j)
      जारी;
    /* Move item i to occupy place for item j, shift what's in between. */
    {
      T t = array[i];
      स_हटाओ (&array[j + 1], &array[j], (i - j) * परिमाण (T));
      array[j] = t;
    }
    यदि (array2)
    {
      T3 t = array2[i];
      स_हटाओ (&array2[j + 1], &array2[j], (i - j) * परिमाण (T3));
      array2[j] = t;
    }
  }
}

स्थिर अन्तःस्थ hb_bool_t
hb_codepoint_parse (अचर वर्ण *s, अचिन्हित पूर्णाङ्क len, पूर्णाङ्क base, hb_codepoint_t *out)
{
  अचिन्हित पूर्णाङ्क v;
  अचर वर्ण *p = s;
  अचर वर्ण *end = p + len;
  यदि (unlikely (!hb_parse_uint (&p, end, &v, सत्य/* whole buffer */, base)))
    फर्काऊ असत्य;

  *out = v;
  फर्काऊ सत्य;
}


/* Operators. */

संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (अचर T &a, अचर T &b) अचर HB_AUTO_RETURN (a & b)
}
HB_FUNCOBJ (hb_bitwise_and);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (अचर T &a, अचर T &b) अचर HB_AUTO_RETURN (a | b)
}
HB_FUNCOBJ (hb_bitwise_or);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (अचर T &a, अचर T &b) अचर HB_AUTO_RETURN (a ^ b)
}
HB_FUNCOBJ (hb_bitwise_xor);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (अचर T &a, अचर T &b) अचर HB_AUTO_RETURN (~a & b)
}
HB_FUNCOBJ (hb_bitwise_lt);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (अचर T &a, अचर T &b) अचर HB_AUTO_RETURN (a & ~b)
}
HB_FUNCOBJ (hb_bitwise_gt); // aka sub
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (अचर T &a, अचर T &b) अचर HB_AUTO_RETURN (~a | b)
}
HB_FUNCOBJ (hb_bitwise_le);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (अचर T &a, अचर T &b) अचर HB_AUTO_RETURN (a | ~b)
}
HB_FUNCOBJ (hb_bitwise_ge);
संरचना
{
  ढाँचा <typename T> constexpr स्वतः
  चालक () (अचर T &a) अचर HB_AUTO_RETURN (~a)
}
HB_FUNCOBJ (hb_bitwise_neg);

संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (अचर T &a, अचर T2 &b) अचर HB_AUTO_RETURN (a + b)
}
HB_FUNCOBJ (hb_add);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (अचर T &a, अचर T2 &b) अचर HB_AUTO_RETURN (a - b)
}
HB_FUNCOBJ (hb_sub);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (अचर T &a, अचर T2 &b) अचर HB_AUTO_RETURN (b - a)
}
HB_FUNCOBJ (hb_rsub);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (अचर T &a, अचर T2 &b) अचर HB_AUTO_RETURN (a * b)
}
HB_FUNCOBJ (hb_mul);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (अचर T &a, अचर T2 &b) अचर HB_AUTO_RETURN (a / b)
}
HB_FUNCOBJ (hb_div);
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (अचर T &a, अचर T2 &b) अचर HB_AUTO_RETURN (a % b)
}
HB_FUNCOBJ (hb_mod);
संरचना
{
  ढाँचा <typename T> constexpr स्वतः
  चालक () (अचर T &a) अचर HB_AUTO_RETURN (+a)
}
HB_FUNCOBJ (hb_pos);
संरचना
{
  ढाँचा <typename T> constexpr स्वतः
  चालक () (अचर T &a) अचर HB_AUTO_RETURN (-a)
}
HB_FUNCOBJ (hb_neg);
संरचना
{
  ढाँचा <typename T> constexpr स्वतः
  चालक () (T &a) अचर HB_AUTO_RETURN (++a)
}
HB_FUNCOBJ (hb_inc);
संरचना
{
  ढाँचा <typename T> constexpr स्वतः
  चालक () (T &a) अचर HB_AUTO_RETURN (--a)
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
  यदि (b - a <= 0.0)
  {
    y = ya;
    फर्काऊ a;
  }

  अचिन्हित n1_2 = (अचिन्हित) (hb_max (उच्चमान (log2 ((b - a) / epsilon)) - 1.0, 0.0));
  अचर अचिन्हित n0 = 1; // Hardwired
  अचर द्विगुण k1 = 0.2 / (b - a); // Hardwired.
  अचिन्हित nmax = n0 + n1_2;
  द्विगुण scaled_epsilon = epsilon * द्विगुण (1llu << nmax);
  द्विगुण _2_epsilon = 2.0 * epsilon;
  जबसम्म (b - a > _2_epsilon)
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
    पूर्णाङ्क sigma_sign = sigma >= 0 ? +1 : -1;
    द्विगुण xt = delta <= भ_असल (x1_2 - xf) ? xf + delta * sigma_sign : x1_2;
    द्विगुण xitp = भ_असल (xt - x1_2) <= r ? xt : x1_2 - r * sigma_sign;
    द्विगुण yitp = f (xitp);
    यदि (yitp > max_y)
    {
      b = xitp;
      yb = yitp;
    }
    अन्यथा यदि (yitp < min_y)
    {
      a = xitp;
      ya = yitp;
    }
    अन्यथा
    {
      y = yitp;
      फर्काऊ xitp;
    }
    scaled_epsilon *= 0.5;
  }
  फर्काऊ 0.5 * (a + b);
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
  hb_scope_guard_t (अचर hb_scope_guard_t &) = delete;
  hb_scope_guard_t &चालक= (अचर hb_scope_guard_t &) = delete;
  hb_scope_guard_t &चालक= (hb_scope_guard_t &&) = delete;
  ~hb_scope_guard_t () { यदि (active) f (); }

  /* Release: dismiss the guard so the cleanup does NOT run.  Use
   * when transferring ownership out of the scope. */
  शून्य release () { active = असत्य; }

  गुप्त:
  F f;
  तार्किक active;
};

ढाँचा <typename F>
स्थिर अन्तःस्थ hb_scope_guard_t<F> hb_make_scope_guard (F &&f)
{ फर्काऊ hb_scope_guard_t<F> (std::forward<F> (f)); }

#define HB_SCOPE_GUARD_NAME_(पङ्क्ति) hb_scope_guard_##line
#define HB_SCOPE_GUARD_NAME(पङ्क्ति) HB_SCOPE_GUARD_NAME_(पङ्क्ति)
#define HB_SCOPE_GUARD(stmt) \
  स्वतः HB_SCOPE_GUARD_NAME(__LINE__) = \
    hb_make_scope_guard ([&]() { stmt; }); \
  (शून्य) HB_SCOPE_GUARD_NAME(__LINE__)


#endif /* HB_ALGS_HH */
