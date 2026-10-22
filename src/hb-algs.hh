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

#अगर_अघोषित HB_ALGS_HH
#घोषणा HB_ALGS_HH

#समावेश "hb.hh"
#समावेश "hb-meta.hh"
#समावेश "hb-null.hh"
#समावेश "hb-number.hh"

#समावेश <algorithm>
#समावेश <initializer_list>
#समावेश <functional>
#समावेश <new>

अटल HB_ALWAYS_INLINE व्योम *
hb_malloc2 (माप_प्रकार nmemb, माप_प्रकार आकार)
{
  अगर (आकार && nmemb > SIZE_MAX / आकार) वापस nullptr;
  वापस hb_malloc (nmemb * आकार);
}
अटल HB_ALWAYS_INLINE व्योम *
hb_realloc2 (व्योम *ptr, माप_प्रकार nmemb, माप_प्रकार आकार)
{
  अगर (आकार && nmemb > SIZE_MAX / आकार) वापस nullptr;
  वापस hb_realloc (ptr, nmemb * आकार);
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
#अगर_घोषित _MSC_VER
# pragma warning(disable:4200)
# pragma warning(disable:4800)
#पूर्ण_अगर
#घोषणा HB_MARK_AS_FLAG_T(T) \
	बाह्य "C++" { \
	  अटल अंतरभूत constexpr T चालक | (T l, T r) { वापस T ((अचिन्हित) l | (अचिन्हित) r); } \
	  अटल अंतरभूत constexpr T चालक & (T l, T r) { वापस T ((अचिन्हित) l & (अचिन्हित) r); } \
	  अटल अंतरभूत constexpr T चालक ^ (T l, T r) { वापस T ((अचिन्हित) l ^ (अचिन्हित) r); } \
	  अटल अंतरभूत constexpr अचिन्हित चालक ~ (T r) { वापस (~(अचिन्हित) r); } \
	  अटल अंतरभूत T& चालक |= (T &l, T r) { l = l | r; वापस l; } \
	  अटल अंतरभूत T& चालक &= (T& l, T r) { l = l & r; वापस l; } \
	  अटल अंतरभूत T& चालक ^= (T& l, T r) { l = l ^ r; वापस l; } \
	} \
	static_assert (true, "")

/* Useful for set-operations on small enums.
 * For example, for testing "x ∈ {x1, x2, x3}" use:
 * (FLAG_UNSAFE(x) & (FLAG(x1) | FLAG(x2) | FLAG(x3)))
 */
#घोषणा FLAG(x) (static_assert_व्यंजक ((अचिन्हित)(x) < 32) + (((uint32_t) 1U) << (अचिन्हित)(x)))
#घोषणा FLAG_UNSAFE(x) ((अचिन्हित)(x) < 32 ? (((uint32_t) 1U) << (अचिन्हित)(x)) : 0)
#घोषणा FLAG_RANGE(x,y) (static_assert_व्यंजक ((x) < (y)) + FLAG(y+1) - FLAG(x))
#घोषणा FLAG64(x) (static_assert_व्यंजक ((अचिन्हित)(x) < 64) + (((uint64_t) 1ULL) << (अचिन्हित)(x)))
#घोषणा FLAG64_UNSAFE(x) ((अचिन्हित)(x) < 64 ? (((uint64_t) 1ULL) << (अचिन्हित)(x)) : 0)


/*
 * Fixed-endian integers / floats.
 */


/* Endian swap, used in Windows related backends */
अटल अंतरभूत constexpr uint16_t hb_uint16_swap (uint16_t v)
{ वापस (v >> 8) | (v << 8); }
अटल अंतरभूत constexpr uint32_t hb_uint32_swap (uint32_t v)
{ वापस (hb_uint16_swap (v) << 16) | hb_uint16_swap (v >> 16); }

ढाँचा <typename Type>
काष्ठा __attribute__((packed)) hb_packed_t
{
  hb_packed_t () = शेष;
  constexpr hb_packed_t (Type V) : v (V) {}
  चालक Type () स्थिर { वापस v; }
  hb_packed_t & चालक = (Type V) { v = V; वापस *यह; }

  निजी:
  Type v;
};

#अगर_अघोषित HB_FAST_अंक_ACCESS

#अगर defined(__OPTIMIZE__) && \
    defined(__BYTE_ORDER) && \
    (__BYTE_ORDER == __BIG_ENDIAN || \
     (__BYTE_ORDER == __LITTLE_ENDIAN && \
      hb_has_builtin(__builtin_bswap16) && \
      hb_has_builtin(__builtin_bswap32) && \
      hb_has_builtin(__builtin_bswap64)))
#घोषणा HB_FAST_अंक_ACCESS 1
#अन्यथा
#घोषणा HB_FAST_अंक_ACCESS 0
#पूर्ण_अगर

// https://github.com/harfbuzz/harfbuzz/issues/5456
#अगर defined(__GNUC__) && !defined(__clang__) && (__GNUC__ <= 12)
#अघोषित HB_FAST_अंक_ACCESS
#घोषणा HB_FAST_अंक_ACCESS 0
#पूर्ण_अगर

#पूर्ण_अगर

ढाँचा <bool BE, typename Type, पूर्णांक Bytes = माप (Type)>
काष्ठा HBInt;
ढाँचा <bool BE, typename Type>
काष्ठा HBInt<BE, Type, 1>
{
  खुला:
  HBInt () = शेष;
  constexpr HBInt (Type V) : v {uint8_t (V)} {}
  constexpr चालक Type () स्थिर { वापस v; }
  निजी: uint8_t v;
};
ढाँचा <bool BE, typename Type>
काष्ठा HBInt<BE, Type, 2>
{
  खुला:
  HBInt () = शेष;

  HBInt (Type V)
#अगर HB_FAST_अंक_ACCESS
  {
    अगर (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint16_t> *) v) = V;
    अन्यथा
      *((hb_packed_t<uint16_t> *) v) = __builtin_bswap16 (V);
  }
#अन्यथा
    : v {BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >>  8) & 0xFF)} {}
#पूर्ण_अगर

  constexpr चालक Type () स्थिर
  {
#अगर HB_FAST_अंक_ACCESS
    वापस (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint16_t) *((स्थिर hb_packed_t<uint16_t> *) v)
    :
      __builtin_bswap16 ((uint16_t) *((स्थिर hb_packed_t<uint16_t> *) v))
    ;
#अन्यथा
    वापस (BE ? (v[0] <<  8) : (v[0]      ))
	 + (BE ? (v[1]      ) : (v[1] <<  8));
#पूर्ण_अगर
  }
  निजी: uint8_t v[2];
};
ढाँचा <bool BE, typename Type>
काष्ठा HBInt<BE, Type, 3>
{
  static_assert (!std::is_signed<Type>::value, "");
  खुला:
  HBInt () = शेष;
  constexpr HBInt (Type V) : v {BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
				BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
				BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V      ) & 0xFF)} {}

  constexpr चालक Type () स्थिर { वापस (BE ? (v[0] << 16) : (v[0]      ))
					  + (BE ? (v[1] <<  8) : (v[1] <<  8))
					  + (BE ? (v[2]      ) : (v[2] << 16)); }
  निजी: uint8_t v[3];
};
ढाँचा <bool BE, typename Type>
काष्ठा HBInt<BE, Type, 4>
{
  ढाँचा <bool, typename, पूर्णांक>
  मित्र काष्ठा HBFloat;

  खुला:
  HBInt () = शेष;

  HBInt (Type V)
#अगर HB_FAST_अंक_ACCESS
  {
    अगर (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint32_t> *) v) = V;
    अन्यथा
      *((hb_packed_t<uint32_t> *) v) = __builtin_bswap32 (V);
  }
#अन्यथा
    : v {BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 24) & 0xFF)} {}
#पूर्ण_अगर

  constexpr चालक Type () स्थिर {
#अगर HB_FAST_अंक_ACCESS
    वापस (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint32_t) *((स्थिर hb_packed_t<uint32_t> *) v)
    :
      __builtin_bswap32 ((uint32_t) *((स्थिर hb_packed_t<uint32_t> *) v))
    ;
#अन्यथा
    वापस (BE ? (uint32_t (v[0]) << 24) : (uint32_t (v[0])      ))
	 + (BE ? (uint32_t (v[1]) << 16) : (uint32_t (v[1]) <<  8))
	 + (BE ? (uint32_t (v[2]) <<  8) : (uint32_t (v[2]) << 16))
	 + (BE ? (uint32_t (v[3])      ) : (uint32_t (v[3]) << 24));
#पूर्ण_अगर
  }
  निजी: uint8_t v[4];
};
ढाँचा <bool BE, typename Type>
काष्ठा HBInt<BE, Type, 8>
{
  ढाँचा <bool, typename, पूर्णांक>
  मित्र काष्ठा HBFloat;

  खुला:
  HBInt () = शेष;

  HBInt (Type V)
#अगर HB_FAST_अंक_ACCESS
  {
    अगर (BE == (__BYTE_ORDER == __BIG_ENDIAN))
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
#पूर्ण_अगर

  constexpr चालक Type () स्थिर {
#अगर HB_FAST_अंक_ACCESS
    वापस (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint64_t) *((स्थिर hb_packed_t<uint64_t> *) v)
    :
      __builtin_bswap64 ((uint64_t) *((स्थिर hb_packed_t<uint64_t> *) v))
    ;
#अन्यथा
    वापस (BE ? (uint64_t (v[0]) << 56) : (uint64_t (v[0])      ))
	 + (BE ? (uint64_t (v[1]) << 48) : (uint64_t (v[1]) <<  8))
	 + (BE ? (uint64_t (v[2]) << 40) : (uint64_t (v[2]) << 16))
	 + (BE ? (uint64_t (v[3]) << 32) : (uint64_t (v[3]) << 24))
	 + (BE ? (uint64_t (v[4]) << 24) : (uint64_t (v[4]) << 32))
	 + (BE ? (uint64_t (v[5]) << 16) : (uint64_t (v[5]) << 40))
	 + (BE ? (uint64_t (v[6]) <<  8) : (uint64_t (v[6]) << 48))
	 + (BE ? (uint64_t (v[7])      ) : (uint64_t (v[7]) << 56));
#पूर्ण_अगर
  }
  निजी: uint8_t v[8];
};

/* Floats. */

ढाँचा <bool BE, typename Type, पूर्णांक Bytes>
काष्ठा HBFloat
{
  using IntType = typename std::conditional<Bytes == 4, uint32_t, uint64_t>::type;

  खुला:
  HBFloat () = शेष;

  HBFloat (Type V)
  {
#अगर HB_FAST_अंक_ACCESS
    {
	      अगर (BE == (__BYTE_ORDER == __BIG_ENDIAN))
	      {
	        *((hb_packed_t<Type> *) v) = V;
	        वापस;
	      }
	    }
#पूर्ण_अगर

    जोड़ {
      hb_packed_t<Type> f;
      hb_packed_t<IntType> i;
    } u = {{V}};

    स्थिर HBInt<BE, IntType> I = (IntType) u.i;
    क्रम (अचिन्हित i = 0; i < Bytes; i++)
      v[i] = I.v[i];
  }

  /* c++14 constexpr */ चालक Type () स्थिर
  {
#अगर HB_FAST_अंक_ACCESS
	    {
	      अगर (BE == (__BYTE_ORDER == __BIG_ENDIAN))
		वापस (Type) *((स्थिर hb_packed_t<Type> *) v);
	    }
#पूर्ण_अगर

    HBInt<BE, IntType> I;
    क्रम (अचिन्हित i = 0; i < Bytes; i++)
      I.v[i] = v[i];

    जोड़ {
      hb_packed_t<IntType> i;
      hb_packed_t<Type> f;
    } u = {{I}};

    वापस (Type) u.f;
  }
  निजी: uint8_t v[Bytes];
};


/* We want our rounding towards +infinity. */
अटल अंतरभूत द्विगुन
_hb_roundf (द्विगुन x) { वापस न्यूनमान (x + .5); }

अटल अंतरभूत भग्न
_hb_roundf (भग्न x) { वापस न्यूनमान ((द्विगुन) x + .5); }

#घोषणा roundf(x) _hb_roundf(x)

अटल अंतरभूत व्योम
hb_sincos (भग्न rotation, भग्न &s, भग्न &c)
{
#अगर_घोषित HAVE_SINCOSF
  sincosf (rotation, &s, &c);
#अन्यथा
  c = cosf (rotation);
  s = sinf (rotation);
#पूर्ण_अगर
}
अटल अंतरभूत व्योम
hb_sincos (द्विगुन rotation, द्विगुन &s, द्विगुन &c)
{
#अगर_घोषित HAVE_SINCOS
  sincos (rotation, &s, &c);
#अन्यथा
  c = cos (rotation);
  s = sin (rotation);
#पूर्ण_अगर
}


/* Encodes three unsigned integers in one 64-bit number.  If the inputs have more than 21 bits,
 * values will be truncated / overlap, and might not decode exactly. */
#घोषणा HB_CODEPOINT_ENCODE3(x,y,z) (((uint64_t) (x) << 42) | ((uint64_t) (y) << 21) | (uint64_t) (z))
#घोषणा HB_CODEPOINT_DECODE3_1(v) ((hb_codepoint_t) ((v) >> 42))
#घोषणा HB_CODEPOINT_DECODE3_2(v) ((hb_codepoint_t) ((v) >> 21) & 0x1FFFFFu)
#घोषणा HB_CODEPOINT_DECODE3_3(v) ((hb_codepoint_t) (v) & 0x1FFFFFu)

/* Custom encoding used by hb-ucd. */
#घोषणा HB_CODEPOINT_ENCODE3_11_7_14(x,y,z) (((uint32_t) ((x) & 0x07FFu) << 21) | (((uint32_t) (y) & 0x007Fu) << 14) | (uint32_t) ((z) & 0x3FFFu))
#घोषणा HB_CODEPOINT_DECODE3_11_7_14_1(v) ((hb_codepoint_t) ((v) >> 21))
#घोषणा HB_CODEPOINT_DECODE3_11_7_14_2(v) ((hb_codepoint_t) (((v) >> 14) & 0x007Fu) | 0x0300)
#घोषणा HB_CODEPOINT_DECODE3_11_7_14_3(v) ((hb_codepoint_t) (v) & 0x3FFFu)


काष्ठा
{
  /* Note.  This is dangerous in that if it's passed an rvalue, it returns rvalue-reference. */
  ढाँचा <typename T> constexpr स्वतः
  चालक () (T&& v) स्थिर HB_AUTO_RETURN ( std::forward<T> (v) )
}
HB_FUNCOBJ (hb_identity);
काष्ठा
{
  /* Like identity(), but only retains lvalue-references.  Rvalues are returned as rvalues. */
  ढाँचा <typename T> constexpr T&
  चालक () (T& v) स्थिर { वापस v; }

  ढाँचा <typename T> constexpr hb_remove_reference<T>
  चालक () (T&& v) स्थिर { वापस v; }
}
HB_FUNCOBJ (hb_lidentity);
काष्ठा
{
  /* Like identity(), but always returns rvalue. */
  ढाँचा <typename T> constexpr hb_remove_reference<T>
  चालक () (T&& v) स्थिर { वापस v; }
}
HB_FUNCOBJ (hb_ridentity);

काष्ठा
{
  ढाँचा <typename T> constexpr bool
  चालक () (T&& v) स्थिर { वापस bool (std::forward<T> (v)); }
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
#घोषणा fasthash_mix(h) (					\
			(व्योम) ((h) ^= (h) >> 23),		\
			(व्योम) ((h) *= 0x2127599bf4325c37ULL),	\
			(h) ^= (h) >> 47)

अटल अंतरभूत uint64_t fasthash64(स्थिर व्योम *buf, माप_प्रकार len, uint64_t seed)
{
	काष्ठा __attribute__((packed)) packed_uint64_t { uint64_t v; };
	स्थिर uint64_t    m = 0x880355f21e6d1965ULL;
	स्थिर packed_uint64_t *pos = (स्थिर packed_uint64_t *)buf;
	स्थिर packed_uint64_t *end = pos + (len / 8);
	स्थिर अचिन्हित अक्षर *pos2;
	uint64_t h = seed ^ (len * m);
	uint64_t v;

#अगर_अघोषित HB_OPTIMIZE_आकार
	अगर (((uintptr_t) pos & 7) == 0)
	{
	  जबतक (pos != end)
	  {
#आशय GCC diagnostic push
#आशय GCC diagnostic ignored "-Wcast-align"
	    v  = * (स्थिर uint64_t *) (pos++);
#आशय GCC diagnostic निकालो
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}
	अन्यथा
#पूर्ण_अगर
	{
	  जबतक (pos != end)
	  {
	    v  = pos++->v;
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}

	pos2 = (स्थिर अचिन्हित अक्षर*)pos;
	v = 0;

	चयन (len & 7) {
	हाल 7: v ^= (uint64_t)pos2[6] << 48; HB_FALLTHROUGH;
	हाल 6: v ^= (uint64_t)pos2[5] << 40; HB_FALLTHROUGH;
	हाल 5: v ^= (uint64_t)pos2[4] << 32; HB_FALLTHROUGH;
	हाल 4: v ^= (uint64_t)pos2[3] << 24; HB_FALLTHROUGH;
	हाल 3: v ^= (uint64_t)pos2[2] << 16; HB_FALLTHROUGH;
	हाल 2: v ^= (uint64_t)pos2[1] <<  8; HB_FALLTHROUGH;
	हाल 1: v ^= (uint64_t)pos2[0];
		h ^= fasthash_mix(v);
		h *= m;
	}

	वापस fasthash_mix(h);
}

अटल अंतरभूत uint32_t fasthash32(स्थिर व्योम *buf, माप_प्रकार len, uint32_t seed)
{
	// the following trick converts the 64-bit hashcode to Fermat
	// residue, which shall retain information from both the higher
	// and lower parts of hashcode.
        uint64_t h = fasthash64(buf, len, seed);
	वापस h - (h >> 32);
}

काष्ठा
{
  निजी:

  ढाँचा <typename T> constexpr स्वतः
  impl (स्थिर T& v, hb_priority<2>) स्थिर HB_RETURN (uint32_t, hb_deref (v).hash ())

  // Horrible: std:hash() of integers seems to be identity in gcc / clang?!
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  //
  // For performance characteristics see:
  // https://github.com/harfbuzz/harfbuzz/pull/4228#issuecomment-1565079537
  ढाँचा <typename T,
	    hb_enable_if (std::is_integral<T>::value && माप (T) <= माप (uint32_t))> constexpr स्वतः
  impl (स्थिर T& v, hb_priority<1>) स्थिर HB_RETURN (uint32_t, (uint32_t) v * 2654435761u /* Knuh's multiplicative hash */)
  ढाँचा <typename T,
	    hb_enable_if (std::is_integral<T>::value && माप (T) > माप (uint32_t))> constexpr स्वतः
  impl (स्थिर T& v, hb_priority<1>) स्थिर HB_RETURN (uint32_t, (uint32_t) (v ^ (v >> 32)) * 2654435761u /* Knuth's multiplicative hash */)

  ढाँचा <typename T,
	    hb_enable_if (std::is_floating_point<T>::value)> constexpr स्वतः
  impl (स्थिर T& v, hb_priority<1>) स्थिर HB_RETURN (uint32_t, fasthash32 (std::addressof (v), माप (T), 0xf437ffe6))

  ढाँचा <typename T> constexpr स्वतः
  impl (स्थिर T& v, hb_priority<0>) स्थिर HB_RETURN (uint32_t, std::hash<hb_decay<decltype (hb_deref (v))>>{} (hb_deref (v)))

  खुला:

  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिर T& v) स्थिर HB_RETURN (uint32_t, impl (v, hb_prioritize))
}
HB_FUNCOBJ (hb_hash);


काष्ठा
{
  निजी:

  /* Pointer-to-member-function. */
  ढाँचा <typename Appl, typename T, typename ...Ts> स्वतः
  impl (Appl&& a, hb_priority<2>, T &&v, Ts&&... ds) स्थिर HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v)).*std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  /* Pointer-to-member. */
  ढाँचा <typename Appl, typename T> स्वतः
  impl (Appl&& a, hb_priority<1>, T &&v) स्थिर HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v))).*std::forward<Appl> (a))

  /* Operator(). */
  ढाँचा <typename Appl, typename ...Ts> स्वतः
  impl (Appl&& a, hb_priority<0>, Ts&&... ds) स्थिर HB_AUTO_RETURN
  (hb_deref (std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  खुला:

  ढाँचा <typename Appl, typename ...Ts> स्वतः
  चालक () (Appl&& a, Ts&&... ds) स्थिर HB_AUTO_RETURN
  (
    impl (std::forward<Appl> (a),
	  hb_prioritize,
	  std::forward<Ts> (ds)...)
  )
}
HB_FUNCOBJ (hb_invoke);

ढाँचा <अचिन्हित Pos, typename Appl, typename V>
काष्ठा hb_partial_t
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
    वापस hb_invoke (std::forward<Appl> (a),
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
    वापस hb_invoke (std::forward<Appl> (a),
		      std::forward<T0> (d0),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }

  निजी:
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
#अगर_घोषित _MSC_VER
/* https://github.com/harfbuzz/harfbuzz/issues/1730 */ \
#घोषणा HB_PARTIALIZE(Pos) \
  ढाँचा <typename _T> \
  decltype(स्वतः) चालक () (_T&& _v) स्थिर \
  { वापस hb_partial<Pos> (यह, std::forward<_T> (_v)); } \
  static_assert (true, "")
#अन्यथा
/* https://github.com/harfbuzz/harfbuzz/issues/1724 */
#घोषणा HB_PARTIALIZE(Pos) \
  ढाँचा <typename _T> \
  स्वतः चालक () (_T&& _v) स्थिर HB_AUTO_RETURN \
  (hb_partial<Pos> (+यह, std::forward<_T> (_v))) \
  static_assert (true, "")
#पूर्ण_अगर


काष्ठा
{
  निजी:

  ढाँचा <typename Pred, typename Val> स्वतः
  impl (Pred&& p, Val &&v, hb_priority<1>) स्थिर HB_AUTO_RETURN
  (
    hb_deref (std::forward<Pred> (p)).has (std::forward<Val> (v))
  )

  ढाँचा <typename Pred, typename Val> स्वतः
  impl (Pred&& p, Val &&v, hb_priority<0>) स्थिर HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Pred> (p),
	       std::forward<Val> (v))
  )

  खुला:

  ढाँचा <typename Pred, typename Val> स्वतः
  चालक () (Pred&& p, Val &&v) स्थिर HB_RETURN (bool,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_has);

काष्ठा
{
  निजी:

  ढाँचा <typename Pred, typename Val> स्वतः
  impl (Pred&& p, Val &&v, hb_priority<1>) स्थिर HB_AUTO_RETURN
  (
    hb_has (std::forward<Pred> (p),
	    std::forward<Val> (v))
  )

  ढाँचा <typename Pred, typename Val> स्वतः
  impl (Pred&& p, Val &&v, hb_priority<0>) स्थिर HB_AUTO_RETURN
  (
    std::forward<Pred> (p) == std::forward<Val> (v)
  )

  खुला:

  ढाँचा <typename Pred, typename Val> स्वतः
  चालक () (Pred&& p, Val &&v) स्थिर HB_RETURN (bool,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_match);

काष्ठा
{
  निजी:

  ढाँचा <typename Proj, typename Val> स्वतः
  impl (Proj&& f, Val &&v, hb_priority<2>) स्थिर HB_AUTO_RETURN
  (
    hb_deref (std::forward<Proj> (f)).लो (std::forward<Val> (v))
  )

  ढाँचा <typename Proj, typename Val> स्वतः
  impl (Proj&& f, Val &&v, hb_priority<1>) स्थिर HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Proj> (f),
	       std::forward<Val> (v))
  )

  ढाँचा <typename Proj, typename Val> स्वतः
  impl (Proj&& f, Val &&v, hb_priority<0>) स्थिर HB_AUTO_RETURN
  (
    std::forward<Proj> (f)[std::forward<Val> (v)]
  )

  खुला:

  ढाँचा <typename Proj, typename Val> स्वतः
  चालक () (Proj&& f, Val &&v) स्थिर HB_AUTO_RETURN
  (
    impl (std::forward<Proj> (f),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_get);

काष्ठा
{
  निजी:

  ढाँचा <typename T1, typename T2> स्वतः
  impl (T1&& v1, T2 &&v2, hb_priority<3>) स्थिर HB_AUTO_RETURN
  (
    std::forward<T2> (v2).cmp (std::forward<T1> (v1)) == 0
  )

  ढाँचा <typename T1, typename T2> स्वतः
  impl (T1&& v1, T2 &&v2, hb_priority<2>) स्थिर HB_AUTO_RETURN
  (
    std::forward<T1> (v1).cmp (std::forward<T2> (v2)) == 0
  )

  ढाँचा <typename T1, typename T2> स्वतः
  impl (T1&& v1, T2 &&v2, hb_priority<1>) स्थिर HB_AUTO_RETURN
  (
    std::forward<T1> (v1) == std::forward<T2> (v2)
  )

  ढाँचा <typename T1, typename T2> स्वतः
  impl (T1&& v1, T2 &&v2, hb_priority<0>) स्थिर HB_AUTO_RETURN
  (
    std::forward<T2> (v2) == std::forward<T1> (v1)
  )

  खुला:

  ढाँचा <typename T1, typename T2> स्वतः
  चालक () (T1&& v1, T2 &&v2) स्थिर HB_AUTO_RETURN
  (
    impl (std::forward<T1> (v1),
	  std::forward<T2> (v2),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_equal);

काष्ठा
{
  ढाँचा <typename T> व्योम
  चालक () (T& a, T& b) स्थिर
  {
    using std::swap; // allow ADL
    swap (a, b);
  }
}
HB_FUNCOBJ (hb_swap);


ढाँचा <typename T1, typename T2>
काष्ठा hb_pair_t
{
  प्रकार T1 first_t;
  प्रकार T2 second_t;
  प्रकार hb_pair_t<T1, T2> pair_t;

  ढाँचा <typename U1 = T1, typename U2 = T2,
	    hb_enable_if (std::is_default_constructible<U1>::value &&
			  std::is_default_constructible<U2>::value)>
  hb_pair_t () : first (), second () {}
  hb_pair_t (T1 a, T2 b) : first (std::forward<T1> (a)), second (std::forward<T2> (b)) {}

  ढाँचा <typename Q1, typename Q2,
	    hb_enable_if (hb_is_convertible (T1, Q1) &&
			  hb_is_convertible (T2, Q2))>
  चालक hb_pair_t<Q1, Q2> () { वापस hb_pair_t<Q1, Q2> (first, second); }

  hb_pair_t<T1, T2> reverse () स्थिर
  { वापस hb_pair_t<T1, T2> (second, first); }

  bool चालक == (स्थिर pair_t& o) स्थिर { वापस first == o.first && second == o.second; }
  bool चालक != (स्थिर pair_t& o) स्थिर { वापस !(*यह == o); }
  bool चालक < (स्थिर pair_t& o) स्थिर { वापस first < o.first || (first == o.first && second < o.second); }
  bool चालक >= (स्थिर pair_t& o) स्थिर { वापस !(*यह < o); }
  bool चालक > (स्थिर pair_t& o) स्थिर { वापस first > o.first || (first == o.first && second > o.second); }
  bool चालक <= (स्थिर pair_t& o) स्थिर { वापस !(*यह > o); }

  अटल पूर्णांक cmp (स्थिर व्योम *pa, स्थिर व्योम *pb)
  {
    pair_t *a = (pair_t *) pa;
    pair_t *b = (pair_t *) pb;

    अगर (a->first < b->first) वापस -1;
    अगर (a->first > b->first) वापस +1;
    अगर (a->second < b->second) वापस -1;
    अगर (a->second > b->second) वापस +1;
    वापस 0;
  }

  मित्र व्योम swap (hb_pair_t& a, hb_pair_t& b) noexcept
  {
    hb_swap (a.first, b.first);
    hb_swap (a.second, b.second);
  }


  T1 first;
  T2 second;
};
ढाँचा <typename T1, typename T2> अटल अंतरभूत hb_pair_t<T1, T2>
hb_pair (T1&& a, T2&& b) { वापस hb_pair_t<T1, T2> (a, b); }

प्रकार hb_pair_t<hb_codepoint_t, hb_codepoint_t> hb_codepoint_pair_t;

काष्ठा
{
  ढाँचा <typename Pair> constexpr typename Pair::first_t
  चालक () (स्थिर Pair& pair) स्थिर { वापस pair.first; }
}
HB_FUNCOBJ (hb_first);

काष्ठा
{
  ढाँचा <typename Pair> constexpr typename Pair::second_t
  चालक () (स्थिर Pair& pair) स्थिर { वापस pair.second; }
}
HB_FUNCOBJ (hb_second);

/* Note.  In min/max impl, we can use hb_type_identity<T> for second argument.
 * However, that would silently convert between different-signedness integers.
 * Instead we accept two different types, such that compiler can err if
 * comparing integers of different signedness. */
काष्ठा
{
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (T&& a, T2&& b) स्थिर HB_AUTO_RETURN
  (a <= b ? a : b)
}
HB_FUNCOBJ (hb_min);
काष्ठा
{
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (T&& a, T2&& b) स्थिर HB_AUTO_RETURN
  (a >= b ? a : b)
}
HB_FUNCOBJ (hb_max);
काष्ठा
{
  ढाँचा <typename T, typename T2, typename T3> constexpr स्वतः
  चालक () (T&& x, T2&& min, T3&& max) स्थिर HB_AUTO_RETURN
  (hb_min (hb_max (std::forward<T> (x), std::forward<T2> (min)), std::forward<T3> (max)))
}
HB_FUNCOBJ (hb_clamp);

/* Signed saturating arithmetic. */
ढाँचा <typename T>
अटल HB_ALWAYS_INLINE T
hb_saturate_add (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#अगर hb_has_builtin(__builtin_add_overflow)
  T परिणाम;
  अगर (likely (!__builtin_add_overflow (a, b, &परिणाम)))
    वापस परिणाम;
#अन्यथा
  अगर (b > 0)
  {
    अगर (likely (a <= hb_int_max (T) - b))
      वापस a + b;
  }
  अन्यथा अगर (likely (a >= hb_int_min (T) - b))
    वापस a + b;
#पूर्ण_अगर

  वापस a < 0 ? hb_int_min (T) : hb_int_max (T);
}

ढाँचा <typename T>
अटल HB_ALWAYS_INLINE T
hb_saturate_sub (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#अगर hb_has_builtin(__builtin_sub_overflow)
  T परिणाम;
  अगर (likely (!__builtin_sub_overflow (a, b, &परिणाम)))
    वापस परिणाम;
#अन्यथा
  अगर (b > 0)
  {
    अगर (likely (a >= hb_int_min (T) + b))
      वापस a - b;
  }
  अन्यथा अगर (likely (a <= hb_int_max (T) + b))
    वापस a - b;
#पूर्ण_अगर

  वापस b < 0 ? hb_int_max (T) : hb_int_min (T);
}

ढाँचा <typename T>
अटल HB_ALWAYS_INLINE T
hb_saturate_neg (T a)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  वापस hb_saturate_sub ((T) 0, a);
}

/* Convert a floating-point value to integer type T, saturating to T's
 * range instead of relying on the undefined behavior of an out-of-range
 * float-to-int conversion.  NaN saturates to the minimum of T. */
ढाँचा <typename T>
अटल HB_ALWAYS_INLINE T
hb_clamp_to (द्विगुन v)
{
  वापस (T) hb_clamp (v,
		       (द्विगुन) hb_int_min (T),
		       (द्विगुन) hb_int_max (T));
}

ढाँचा <typename T>
अटल HB_ALWAYS_INLINE T
hb_clamp_to (भग्न v)
{
  /* Widen explicitly: float cannot represent all integer bounds exactly,
   * so the clamp itself is done in double. */
  वापस hb_clamp_to<T> ((द्विगुन) v);
}

ढाँचा <typename T>
अटल HB_ALWAYS_INLINE T
hb_clamp_to (int64_t v)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  वापस (T) hb_clamp (v,
		       (int64_t) hb_int_min (T),
		       (int64_t) hb_int_max (T));
}

/*
 * Bithacks.
 */

/* Return the number of 1 bits in a uint8_t; faster than hb_popcount() */
अटल अंतरभूत अचिन्हित
hb_popcount8 (uint8_t v)
{
  अटल स्थिर uint8_t popcount4[16] = {
    0, 1, 1, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 2, 3, 3, 4
  };
  वापस popcount4[v & 0xF] + popcount4[v >> 4];
}

/* Return the number of 1 bits in v. */
ढाँचा <typename T>
अटल अंतरभूत अचिन्हित पूर्णांक
hb_popcount (T v)
{
#अगर hb_has_builtin(__builtin_popcount)
  अगर (माप (T) <= माप (अचिन्हित पूर्णांक))
    वापस __builtin_popcount (v);
#पूर्ण_अगर

#अगर hb_has_builtin(__builtin_popcountl)
  अगर (माप (T) <= माप (अचिन्हित दीर्घ))
    वापस __builtin_popcountl (v);
#पूर्ण_अगर

#अगर hb_has_builtin(__builtin_popcountll)
  अगर (माप (T) <= माप (अचिन्हित दीर्घ दीर्घ))
    वापस __builtin_popcountll (v);
#पूर्ण_अगर

  अगर (माप (T) <= 4)
  {
    /* "HACKMEM 169" */
    uint32_t y;
    y = (v >> 1) &033333333333;
    y = v - y - ((y >>1) & 033333333333);
    वापस (((y + (y >> 3)) & 030707070707) % 077);
  }

  अगर (माप (T) == 8)
  {
    uint64_t y = (uint64_t) v;
    y -= ((y >> 1) & 0x5555555555555555ull);
    y = (y & 0x3333333333333333ull) + (y >> 2 & 0x3333333333333333ull);
    वापस ((y + (y >> 4)) & 0xf0f0f0f0f0f0f0full) * 0x101010101010101ull >> 56;
  }

  अगर (माप (T) == 16)
  {
    अचिन्हित पूर्णांक shift = 64;
    वापस hb_popcount<uint64_t> ((uint64_t) v) + hb_popcount ((uint64_t) (v >> shift));
  }

  निश्चित (0);
  वापस 0; /* Shut up stupid compiler. */
}

/* Returns the number of bits needed to store number */
ढाँचा <typename T>
अटल अंतरभूत अचिन्हित पूर्णांक
hb_bit_storage (T v)
{
  अगर (unlikely (!v)) वापस 0;

#अगर hb_has_builtin(__builtin_clz)
  अगर (माप (T) <= माप (अचिन्हित पूर्णांक))
    वापस माप (अचिन्हित पूर्णांक) * 8 - __builtin_clz (v);
#पूर्ण_अगर

#अगर hb_has_builtin(__builtin_clzl)
  अगर (माप (T) <= माप (अचिन्हित दीर्घ))
    वापस माप (अचिन्हित दीर्घ) * 8 - __builtin_clzl (v);
#पूर्ण_अगर

#अगर hb_has_builtin(__builtin_clzll)
  अगर (माप (T) <= माप (अचिन्हित दीर्घ दीर्घ))
    वापस माप (अचिन्हित दीर्घ दीर्घ) * 8 - __builtin_clzll (v);
#पूर्ण_अगर

#अगर (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  अगर (माप (T) <= माप (अचिन्हित पूर्णांक))
  {
    अचिन्हित दीर्घ where;
    _BitScanReverse (&where, v);
    वापस 1 + where;
  }
# अगर defined(_WIN64)
  अगर (माप (T) <= 8)
  {
    अचिन्हित दीर्घ where;
    _BitScanReverse64 (&where, v);
    वापस 1 + where;
  }
# endif
#पूर्ण_अगर

  अगर (माप (T) <= 4)
  {
    /* "bithacks" */
    स्थिर अचिन्हित पूर्णांक b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    स्थिर अचिन्हित पूर्णांक S[] = {1, 2, 4, 8, 16};
    अचिन्हित पूर्णांक r = 0;
    क्रम (पूर्णांक i = 4; i >= 0; i--)
      अगर (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    वापस r + 1;
  }
  अगर (माप (T) <= 8)
  {
    /* "bithacks" */
    स्थिर uint64_t b[] = {0x2ULL, 0xCULL, 0xF0ULL, 0xFF00ULL, 0xFFFF0000ULL, 0xFFFFFFFF00000000ULL};
    स्थिर अचिन्हित पूर्णांक S[] = {1, 2, 4, 8, 16, 32};
    अचिन्हित पूर्णांक r = 0;
    क्रम (पूर्णांक i = 5; i >= 0; i--)
      अगर (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    वापस r + 1;
  }
  अगर (माप (T) == 16)
  {
    अचिन्हित पूर्णांक shift = 64;
    वापस (v >> shift) ? hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift :
			  hb_bit_storage<uint64_t> ((uint64_t) v);
  }

  निश्चित (0);
  वापस 0; /* Shut up stupid compiler. */
}

/* Returns the number of zero bits in the least significant side of v */
ढाँचा <typename T>
अटल अंतरभूत अचिन्हित पूर्णांक
hb_ctz (T v)
{
  अगर (unlikely (!v)) वापस 8 * माप (T);

#अगर hb_has_builtin(__builtin_ctz)
  अगर (माप (T) <= माप (अचिन्हित पूर्णांक))
    वापस __builtin_ctz (v);
#पूर्ण_अगर

#अगर hb_has_builtin(__builtin_ctzl)
  अगर (माप (T) <= माप (अचिन्हित दीर्घ))
    वापस __builtin_ctzl (v);
#पूर्ण_अगर

#अगर hb_has_builtin(__builtin_ctzll)
  अगर (माप (T) <= माप (अचिन्हित दीर्घ दीर्घ))
    वापस __builtin_ctzll (v);
#पूर्ण_अगर

#अगर (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  अगर (माप (T) <= माप (अचिन्हित पूर्णांक))
  {
    अचिन्हित दीर्घ where;
    _BitScanForward (&where, v);
    वापस where;
  }
# अगर defined(_WIN64)
  अगर (माप (T) <= 8)
  {
    अचिन्हित दीर्घ where;
    _BitScanForward64 (&where, v);
    वापस where;
  }
# endif
#पूर्ण_अगर

  अगर (माप (T) <= 4)
  {
    /* "bithacks" */
    अचिन्हित पूर्णांक c = 32;
    v &= - (int32_t) v;
    अगर (v) c--;
    अगर (v & 0x0000FFFF) c -= 16;
    अगर (v & 0x00FF00FF) c -= 8;
    अगर (v & 0x0F0F0F0F) c -= 4;
    अगर (v & 0x33333333) c -= 2;
    अगर (v & 0x55555555) c -= 1;
    वापस c;
  }
  अगर (माप (T) <= 8)
  {
    /* "bithacks" */
    अचिन्हित पूर्णांक c = 64;
    v &= - (int64_t) (v);
    अगर (v) c--;
    अगर (v & 0x00000000FFFFFFFFULL) c -= 32;
    अगर (v & 0x0000FFFF0000FFFFULL) c -= 16;
    अगर (v & 0x00FF00FF00FF00FFULL) c -= 8;
    अगर (v & 0x0F0F0F0F0F0F0F0FULL) c -= 4;
    अगर (v & 0x3333333333333333ULL) c -= 2;
    अगर (v & 0x5555555555555555ULL) c -= 1;
    वापस c;
  }
  अगर (माप (T) == 16)
  {
    अचिन्हित पूर्णांक shift = 64;
    वापस (uint64_t) v ? hb_bit_storage<uint64_t> ((uint64_t) v) :
			  hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift;
  }

  निश्चित (0);
  वापस 0; /* Shut up stupid compiler. */
}


/*
 * Tiny stuff.
 */

/* ASCII tag/character handling */
अटल अंतरभूत bool ISALPHA (अचिन्हित अक्षर c)
{ वापस (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
अटल अंतरभूत bool ISALNUM (अचिन्हित अक्षर c)
{ वापस (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }
अटल अंतरभूत bool ISSPACE (अचिन्हित अक्षर c)
{ वापस c == ' ' || c =='\f'|| c =='\n'|| c =='\r'|| c =='\t'|| c =='\v'; }
अटल अंतरभूत अचिन्हित अक्षर TOUPPER (अचिन्हित अक्षर c)
{ वापस (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c; }
अटल अंतरभूत अचिन्हित अक्षर TOLOWER (अचिन्हित अक्षर c)
{ वापस (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }
अटल अंतरभूत bool ISHEX (अचिन्हित अक्षर c)
{ वापस (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
अटल अंतरभूत अचिन्हित अक्षर TOHEX (uint8_t c)
{ वापस (c & 0xF) <= 9 ? (c & 0xF) + '0' : (c & 0xF) + 'a' - 10; }
अटल अंतरभूत uint8_t FROMHEX (अचिन्हित अक्षर c)
{ वापस (c >= '0' && c <= '9') ? c - '0' : TOLOWER (c) - 'a' + 10; }

अटल अंतरभूत अचिन्हित पूर्णांक DIV_CEIL (स्थिर अचिन्हित पूर्णांक a, अचिन्हित पूर्णांक b)
{ वापस (a + (b - 1)) / b; }


#अघोषित  ARRAY_लंबाई
ढाँचा <typename Type, अचिन्हित पूर्णांक n>
अटल अंतरभूत अचिन्हित पूर्णांक ARRAY_लंबाई (स्थिर Type (&)[n]) { वापस n; }
/* A const version, but does not detect erratically being called on pointers. */
#घोषणा ARRAY_लंबाई_CONST(__array) ((चिन्हित पूर्णांक) (माप (__array) / माप (__array[0])))


अटल अंतरभूत व्योम *
hb_memcpy (व्योम *__restrict dst, स्थिर व्योम *__restrict src, माप_प्रकार len)
{
  /* It's illegal to pass 0 as size to memcpy. */
  अगर (unlikely (!len)) वापस dst;
  वापस स_नकल (dst, src, len);
}

अटल अंतरभूत पूर्णांक
hb_memcmp (स्थिर व्योम *a, स्थिर व्योम *b, अचिन्हित पूर्णांक len)
{
  /* It's illegal to pass NULL to memcmp(), even if len is zero.
   * So, wrap it.
   * https://sourceware.org/bugzilla/show_bug.cgi?id=23878 */
  अगर (unlikely (!len)) वापस 0;
  वापस स_भेद (a, b, len);
}

अटल अंतरभूत व्योम *
hb_memset (व्योम *s, पूर्णांक c, अचिन्हित पूर्णांक n)
{
  /* It's illegal to pass NULL to memset(), even if n is zero. */
  अगर (unlikely (!n)) वापस s;
  वापस स_रखो (s, c, n);
}

अटल अंतरभूत अचिन्हित पूर्णांक
hb_ceil_to_4 (अचिन्हित पूर्णांक v)
{
  वापस ((v - 1) | 3) + 1;
}

ढाँचा <typename T> अटल अंतरभूत bool
hb_in_range (T u, T lo, T hi)
{
  static_assert (!std::is_signed<T>::value, "");

  /* The casts below are important as if T is smaller than int,
   * the subtract results will become a signed int! */
  वापस (T)(u - lo) <= (T)(hi - lo);
}
ढाँचा <typename T> अटल अंतरभूत bool
hb_in_ranges (T u, T lo1, T hi1)
{
  वापस hb_in_range (u, lo1, hi1);
}
ढाँचा <typename T, typename ...Ts> अटल अंतरभूत bool
hb_in_ranges (T u, T lo1, T hi1, Ts... ds)
{
  वापस hb_in_range<T> (u, lo1, hi1) || hb_in_ranges<T> (u, ds...);
}


/*
 * Overflow checking.
 */

अटल अंतरभूत bool
hb_unsigned_mul_overflows (अचिन्हित पूर्णांक गिनती, अचिन्हित पूर्णांक आकार, अचिन्हित *परिणाम = nullptr)
{
#अगर hb_has_builtin(__builtin_mul_overflow)
  अचिन्हित ढेर_परिणाम;
  अगर (!परिणाम)
    परिणाम = &ढेर_परिणाम;
  वापस __builtin_mul_overflow (गिनती, आकार, परिणाम);
#पूर्ण_अगर

  अगर (परिणाम)
    *परिणाम = गिनती * आकार;
  वापस (आकार > 0) && (गिनती >= ((अचिन्हित पूर्णांक) -1) / आकार);
}

अटल अंतरभूत bool
hb_unsigned_add_overflows (अचिन्हित पूर्णांक a, अचिन्हित पूर्णांक b, अचिन्हित *परिणाम = nullptr)
{
#अगर hb_has_builtin(__builtin_add_overflow)
  अचिन्हित ढेर_परिणाम;
  अगर (!परिणाम)
    परिणाम = &ढेर_परिणाम;
  वापस __builtin_add_overflow (a, b, परिणाम);
#पूर्ण_अगर

  अगर (परिणाम)
    *परिणाम = a + b;
  वापस b > (अचिन्हित पूर्णांक) -1 - a;
}

/* Saturating arithmetic on size_t.  On platforms where size_t is wider than
 * unsigned int (i.e. 64-bit), the inputs to get_size()-style computations
 * (counts and static_sizes which are at most 32-bit) cannot overflow size_t,
 * so these reduce to plain arithmetic.  On 32-bit platforms (size_t is
 * unsigned int), they saturate to SIZE_MAX so sanitize/serialize callers
 * naturally reject the resulting size as out-of-range. */

अटल अंतरभूत माप_प्रकार
hb_unsigned_mul_saturate (माप_प्रकार a, माप_प्रकार b)
{
  अगर (माप (माप_प्रकार) > माप (अचिन्हित पूर्णांक))
    वापस a * b;
#अगर hb_has_builtin(__builtin_mul_overflow)
  माप_प्रकार परिणाम;
  अगर (__builtin_mul_overflow (a, b, &परिणाम))
    वापस (माप_प्रकार) -1;
  वापस परिणाम;
#अन्यथा
  अगर (b > 0 && a > ((माप_प्रकार) -1) / b) वापस (माप_प्रकार) -1;
  वापस a * b;
#पूर्ण_अगर
}

अटल अंतरभूत माप_प्रकार
hb_unsigned_add_saturate (माप_प्रकार a, माप_प्रकार b)
{
  अगर (माप (माप_प्रकार) > माप (अचिन्हित पूर्णांक))
    वापस a + b;
#अगर hb_has_builtin(__builtin_add_overflow)
  माप_प्रकार परिणाम;
  अगर (__builtin_add_overflow (a, b, &परिणाम))
    वापस (माप_प्रकार) -1;
  वापस परिणाम;
#अन्यथा
  अगर (b > ((माप_प्रकार) -1) - a) वापस (माप_प्रकार) -1;
  वापस a + b;
#पूर्ण_अगर
}

/* Variadic forms: fold left across all arguments. */
ढाँचा <typename ...Ts>
अटल अंतरभूत माप_प्रकार
hb_unsigned_mul_saturate (माप_प्रकार a, माप_प्रकार b, माप_प्रकार c, Ts... rest)
{ वापस hb_unsigned_mul_saturate (hb_unsigned_mul_saturate (a, b), c, rest...); }

ढाँचा <typename ...Ts>
अटल अंतरभूत माप_प्रकार
hb_unsigned_add_saturate (माप_प्रकार a, माप_प्रकार b, माप_प्रकार c, Ts... rest)
{ वापस hb_unsigned_add_saturate (hb_unsigned_add_saturate (a, b), c, rest...); }

/* Saturating mul-add: a * b + c.  Covers the dominant get_size() pattern
 * `count * static_size + min_size`. */
अटल अंतरभूत माप_प्रकार
hb_unsigned_mul_add_saturate (माप_प्रकार a, माप_प्रकार b, माप_प्रकार c)
{ वापस hb_unsigned_add_saturate (hb_unsigned_mul_saturate (a, b), c); }


/*
 * Sort and search.
 */

ढाँचा <typename K, typename V, typename ...Ts>
अटल पूर्णांक
_hb_cmp_method (स्थिर व्योम *pkey, स्थिर व्योम *pval, Ts... ds)
{
  स्थिर K& key = * (स्थिर K*) pkey;
  स्थिर V& val = * (स्थिर V*) pval;

  वापस val.cmp (key, ds...);
}

ढाँचा <typename K, typename V>
अटल पूर्णांक
_hb_cmp_operator (स्थिर व्योम *pkey, स्थिर व्योम *pval)
{
  स्थिर K& key = * (स्थिर K*) pkey;
  स्थिर V& val = * (स्थिर V*) pval;

  अगर (key < val) वापस -1;
  अगर (key > val) वापस  1;
  वापस 0;
}

ढाँचा <typename V, typename K, typename ...Ts>
HB_HOT
अटल अंतरभूत bool
hb_bsearch_impl (अचिन्हित *pos, /* Out */
		 स्थिर K& key,
		 V* base, माप_प्रकार nmemb, माप_प्रकार stride,
		 पूर्णांक (*compar)(स्थिर व्योम *कुंजी_3, स्थिर व्योम *_item, Ts... _ds),
		 Ts... ds)
{
  /* This is our *only* bsearch implementation. */

  पूर्णांक min = 0, max = (पूर्णांक) nmemb - 1;
  जबतक (min <= max)
  {
    पूर्णांक mid = ((अचिन्हित पूर्णांक) min + (अचिन्हित पूर्णांक) max) / 2;
#आशय GCC diagnostic push
#आशय GCC diagnostic ignored "-Wcast-align"
    V* p = (V*) (((स्थिर अक्षर *) base) + (mid * stride));
#आशय GCC diagnostic निकालो
    पूर्णांक c = compar ((स्थिर व्योम *) std::addressof (key), (स्थिर व्योम *) p, ds...);
    अगर (c < 0)
      max = mid - 1;
    अन्यथा अगर (c > 0)
      min = mid + 1;
    अन्यथा
    {
      *pos = mid;
      वापस true;
    }
  }
  *pos = min;
  वापस false;
}

ढाँचा <typename V, typename K>
अटल अंतरभूत V*
hb_bsearch (स्थिर K& key, V* base,
	    माप_प्रकार nmemb, माप_प्रकार stride = माप (V),
	    पूर्णांक (*compar)(स्थिर व्योम *कुंजी_3, स्थिर व्योम *_item) = _hb_cmp_method<K, V>)
{
  अचिन्हित pos;
#आशय GCC diagnostic push
#आशय GCC diagnostic ignored "-Wcast-align"
  वापस hb_bsearch_impl (&pos, key, base, nmemb, stride, compar) ?
	 (V*) (((स्थिर अक्षर *) base) + (pos * stride)) : nullptr;
#आशय GCC diagnostic निकालो
}
ढाँचा <typename V, typename K, typename ...Ts>
अटल अंतरभूत V*
hb_bsearch (स्थिर K& key, V* base,
	    माप_प्रकार nmemb, माप_प्रकार stride,
	    पूर्णांक (*compar)(स्थिर व्योम *कुंजी_3, स्थिर व्योम *_item, Ts... _ds),
	    Ts... ds)
{
  अचिन्हित pos;
#आशय GCC diagnostic push
#आशय GCC diagnostic ignored "-Wcast-align"
  वापस hb_bsearch_impl (&pos, key, base, nmemb, stride, compar, ds...) ?
	 (V*) (((स्थिर अक्षर *) base) + (pos * stride)) : nullptr;
#आशय GCC diagnostic निकालो
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
अटल अंतरभूत व्योम
hb_qsort_loop (T *base, माप_प्रकार nel, Compar compar)
{
  जबतक (nel > 24)
  {
    T *last = base + nel - 1;
    T *mid = base + nel / 2;

    /* Median-of-three pivot, parked at last-1. */
    अगर (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    अगर (compar (*mid, *last) > 0)
    {
      hb_swap (*mid, *last);
      अगर (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    }
    hb_swap (*mid, *(last - 1));
    T &pivot = *(last - 1);

    /* Two-way Hoare partition.  Inner loops are unguarded:
     * median-of-three left *base <= pivot and *last >= pivot,
     * which act as sentinels. */
    T *i = base, *j = last - 1;
    जबतक (true)
    {
      जबतक (compar (*++i, pivot) < 0) {}
      जबतक (compar (*--j, pivot) > 0) {}
      अगर (i >= j) अवरोध;
      hb_swap (*i, *j);
    }
    hb_swap (*i, *(last - 1));

    /* Recurse on smaller side, loop on larger — bounds stack
     * depth at O(log n). */
    माप_प्रकार बाँए  = (माप_प्रकार) (i - base);
    माप_प्रकार दाँए = nel - बाँए - 1;
    अगर (बाँए < दाँए)
    {
      hb_qsort_loop (base, बाँए, compar);
      base = i + 1;
      nel  = दाँए;
    }
    अन्यथा
    {
      hb_qsort_loop (i + 1, दाँए, compar);
      nel  = बाँए;
    }
  }
}

ढाँचा <typename T, typename Compar>
अटल अंतरभूत व्योम
hb_qsort_inline (T *base, माप_प्रकार nel, Compar compar)
{
  hb_qsort_loop (base, nel, compar);

  /* Single final insertion sort over the whole array.  After
   * the partitioning loop, every element is within the threshold
   * of its sorted position, so this pass is O(n * threshold). */
  T *end = base + nel;
  क्रम (T *pi = base + 1; pi < end; pi++)
    क्रम (T *pj = pi; pj > base && compar (pj[-1], pj[0]) > 0; pj--)
      hb_swap (pj[-1], pj[0]);
}

अटल अंतरभूत व्योम
hb_qsort (व्योम *base, माप_प्रकार nel, माप_प्रकार width,
	  पूर्णांक (*compar)(स्थिर व्योम *_a, स्थिर व्योम *_b))
{
  क्विक (base, nel, width, compar);
}




ढाँचा <typename T, typename T2, typename T3 = पूर्णांक> अटल अंतरभूत व्योम
hb_stable_sort (T *array, अचिन्हित पूर्णांक len, पूर्णांक(*compar)(स्थिर T2 *, स्थिर T2 *), T3 *array2 = nullptr)
{
  static_assert (hb_is_trivially_copy_assignable (T), "");
  static_assert (hb_is_trivially_copy_assignable (T3), "");

  क्रम (अचिन्हित पूर्णांक i = 1; i < len; i++)
  {
    अचिन्हित पूर्णांक j = i;
    जबतक (j && compar (&array[j - 1], &array[i]) > 0)
      j--;
    अगर (i == j)
      जारी;
    /* Move item i to occupy place for item j, shift what's in between. */
    {
      T t = array[i];
      स_हटाओ (&array[j + 1], &array[j], (i - j) * माप (T));
      array[j] = t;
    }
    अगर (array2)
    {
      T3 t = array2[i];
      स_हटाओ (&array2[j + 1], &array2[j], (i - j) * माप (T3));
      array2[j] = t;
    }
  }
}

अटल अंतरभूत hb_bool_t
hb_codepoint_parse (स्थिर अक्षर *s, अचिन्हित पूर्णांक len, पूर्णांक base, hb_codepoint_t *out)
{
  अचिन्हित पूर्णांक v;
  स्थिर अक्षर *p = s;
  स्थिर अक्षर *end = p + len;
  अगर (unlikely (!hb_parse_uint (&p, end, &v, true/* whole buffer */, base)))
    वापस false;

  *out = v;
  वापस true;
}


/* Operators. */

काष्ठा
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिर T &a, स्थिर T &b) स्थिर HB_AUTO_RETURN (a & b)
}
HB_FUNCOBJ (hb_bitwise_and);
काष्ठा
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिर T &a, स्थिर T &b) स्थिर HB_AUTO_RETURN (a | b)
}
HB_FUNCOBJ (hb_bitwise_or);
काष्ठा
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिर T &a, स्थिर T &b) स्थिर HB_AUTO_RETURN (a ^ b)
}
HB_FUNCOBJ (hb_bitwise_xor);
काष्ठा
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिर T &a, स्थिर T &b) स्थिर HB_AUTO_RETURN (~a & b)
}
HB_FUNCOBJ (hb_bitwise_lt);
काष्ठा
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिर T &a, स्थिर T &b) स्थिर HB_AUTO_RETURN (a & ~b)
}
HB_FUNCOBJ (hb_bitwise_gt); // aka sub
काष्ठा
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिर T &a, स्थिर T &b) स्थिर HB_AUTO_RETURN (~a | b)
}
HB_FUNCOBJ (hb_bitwise_le);
काष्ठा
{ HB_PARTIALIZE(2);
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिर T &a, स्थिर T &b) स्थिर HB_AUTO_RETURN (a | ~b)
}
HB_FUNCOBJ (hb_bitwise_ge);
काष्ठा
{
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिर T &a) स्थिर HB_AUTO_RETURN (~a)
}
HB_FUNCOBJ (hb_bitwise_neg);

काष्ठा
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (स्थिर T &a, स्थिर T2 &b) स्थिर HB_AUTO_RETURN (a + b)
}
HB_FUNCOBJ (hb_add);
काष्ठा
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (स्थिर T &a, स्थिर T2 &b) स्थिर HB_AUTO_RETURN (a - b)
}
HB_FUNCOBJ (hb_sub);
काष्ठा
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (स्थिर T &a, स्थिर T2 &b) स्थिर HB_AUTO_RETURN (b - a)
}
HB_FUNCOBJ (hb_rsub);
काष्ठा
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (स्थिर T &a, स्थिर T2 &b) स्थिर HB_AUTO_RETURN (a * b)
}
HB_FUNCOBJ (hb_mul);
काष्ठा
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (स्थिर T &a, स्थिर T2 &b) स्थिर HB_AUTO_RETURN (a / b)
}
HB_FUNCOBJ (hb_div);
काष्ठा
{ HB_PARTIALIZE(2);
  ढाँचा <typename T, typename T2> constexpr स्वतः
  चालक () (स्थिर T &a, स्थिर T2 &b) स्थिर HB_AUTO_RETURN (a % b)
}
HB_FUNCOBJ (hb_mod);
काष्ठा
{
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिर T &a) स्थिर HB_AUTO_RETURN (+a)
}
HB_FUNCOBJ (hb_pos);
काष्ठा
{
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिर T &a) स्थिर HB_AUTO_RETURN (-a)
}
HB_FUNCOBJ (hb_neg);
काष्ठा
{
  ढाँचा <typename T> constexpr स्वतः
  चालक () (T &a) स्थिर HB_AUTO_RETURN (++a)
}
HB_FUNCOBJ (hb_inc);
काष्ठा
{
  ढाँचा <typename T> constexpr स्वतः
  चालक () (T &a) स्थिर HB_AUTO_RETURN (--a)
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
द्विगुन solve_itp (func_t f,
		  द्विगुन a, द्विगुन b,
		  द्विगुन epsilon,
		  द्विगुन min_y, द्विगुन max_y,
		  द्विगुन &ya, द्विगुन &yb, द्विगुन &y)
{
  // Guard against degenerate interval
  अगर (b - a <= 0.0)
  {
    y = ya;
    वापस a;
  }

  अचिन्हित n1_2 = (अचिन्हित) (hb_max (उच्चमान (log2 ((b - a) / epsilon)) - 1.0, 0.0));
  स्थिर अचिन्हित n0 = 1; // Hardwired
  स्थिर द्विगुन k1 = 0.2 / (b - a); // Hardwired.
  अचिन्हित nmax = n0 + n1_2;
  द्विगुन scaled_epsilon = epsilon * द्विगुन (1llu << nmax);
  द्विगुन _2_epsilon = 2.0 * epsilon;
  जबतक (b - a > _2_epsilon)
  {
    द्विगुन x1_2 = 0.5 * (a + b);
    द्विगुन r = scaled_epsilon - 0.5 * (b - a);
    // Guard against yb == ya to prevent division by zero
    द्विगुन xf = (yb != ya) ? (yb * a - ya * b) / (yb - ya) : x1_2;
    द्विगुन sigma = x1_2 - xf;
    द्विगुन b_a = b - a;
    // This has k2 = 2 hardwired for efficiency.
    द्विगुन b_a_k2 = b_a * b_a;
    द्विगुन delta = k1 * b_a_k2;
    पूर्णांक sigma_sign = sigma >= 0 ? +1 : -1;
    द्विगुन xt = delta <= भ_असल (x1_2 - xf) ? xf + delta * sigma_sign : x1_2;
    द्विगुन xitp = भ_असल (xt - x1_2) <= r ? xt : x1_2 - r * sigma_sign;
    द्विगुन yitp = f (xitp);
    अगर (yitp > max_y)
    {
      b = xitp;
      yb = yitp;
    }
    अन्यथा अगर (yitp < min_y)
    {
      a = xitp;
      ya = yitp;
    }
    अन्यथा
    {
      y = yitp;
      वापस xitp;
    }
    scaled_epsilon *= 0.5;
  }
  वापस 0.5 * (a + b);
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
काष्ठा hb_scope_guard_t
{
  explicit hb_scope_guard_t (F &&f) : f (std::move (f)), active (true) {}
  hb_scope_guard_t (hb_scope_guard_t &&o) noexcept
    : f (std::move (o.f)), active (o.active) { o.active = false; }
  hb_scope_guard_t (स्थिर hb_scope_guard_t &) = मिटाओ;
  hb_scope_guard_t &चालक= (स्थिर hb_scope_guard_t &) = मिटाओ;
  hb_scope_guard_t &चालक= (hb_scope_guard_t &&) = मिटाओ;
  ~hb_scope_guard_t () { अगर (active) f (); }

  /* Release: dismiss the guard so the cleanup does NOT run.  Use
   * when transferring ownership out of the scope. */
  व्योम release () { active = false; }

  निजी:
  F f;
  bool active;
};

ढाँचा <typename F>
अटल अंतरभूत hb_scope_guard_t<F> hb_make_scope_guard (F &&f)
{ वापस hb_scope_guard_t<F> (std::forward<F> (f)); }

#घोषणा HB_SCOPE_GUARD_NAME_(पंक्ति) hb_scope_guard_##पंक्ति
#घोषणा HB_SCOPE_GUARD_NAME(पंक्ति) HB_SCOPE_GUARD_NAME_(पंक्ति)
#घोषणा HB_SCOPE_GUARD(stmt) \
  स्वतः HB_SCOPE_GUARD_NAME(__LINE__) = \
    hb_make_scope_guard ([&]() { stmt; }); \
  (व्योम) HB_SCOPE_GUARD_NAME(__LINE__)


#पूर्ण_अगर /* HB_ALGS_HH */
