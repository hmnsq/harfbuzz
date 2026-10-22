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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-meta.hh"
#ଅନ୍ତର୍ଭାଵ "hb-null.hh"
#ଅନ୍ତର୍ଭାଵ "hb-number.hh"

#ଅନ୍ତର୍ଭାଵ <algorithm>
#ଅନ୍ତର୍ଭାଵ <initializer_list>
#ଅନ୍ତର୍ଭାଵ <functional>
#ଅନ୍ତର୍ଭାଵ <new>

ସ୍ଥିର HB_ALWAYS_INLINE ଶୂନ୍ୟ *
hb_malloc2 (size_t nmemb, size_t size)
{
  ଯଦି (size && nmemb > SIZE_MAX / size) ଫେରନ୍ତୁ nullptr;
  ଫେରନ୍ତୁ hb_malloc (nmemb * size);
}
ସ୍ଥିର HB_ALWAYS_INLINE ଶୂନ୍ୟ *
hb_realloc2 (ଶୂନ୍ୟ *ptr, size_t nmemb, size_t size)
{
  ଯଦି (size && nmemb > SIZE_MAX / size) ଫେରନ୍ତୁ nullptr;
  ଫେରନ୍ତୁ hb_realloc (ptr, nmemb * size);
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
	ବାହ୍ୟ "C++" { \
	  ସ୍ଥିର ଧାଡ଼ିଭିତରେ constexpr T operator | (T l, T r) { ଫେରନ୍ତୁ T ((unsigned) l | (unsigned) r); } \
	  ସ୍ଥିର ଧାଡ଼ିଭିତରେ constexpr T operator & (T l, T r) { ଫେରନ୍ତୁ T ((unsigned) l & (unsigned) r); } \
	  ସ୍ଥିର ଧାଡ଼ିଭିତରେ constexpr T operator ^ (T l, T r) { ଫେରନ୍ତୁ T ((unsigned) l ^ (unsigned) r); } \
	  ସ୍ଥିର ଧାଡ଼ିଭିତରେ constexpr unsigned operator ~ (T r) { ଫେରନ୍ତୁ (~(unsigned) r); } \
	  ସ୍ଥିର ଧାଡ଼ିଭିତରେ T& operator |= (T &l, T r) { l = l | r; ଫେରନ୍ତୁ l; } \
	  ସ୍ଥିର ଧାଡ଼ିଭିତରେ T& operator &= (T& l, T r) { l = l & r; ଫେରନ୍ତୁ l; } \
	  ସ୍ଥିର ଧାଡ଼ିଭିତରେ T& operator ^= (T& l, T r) { l = l ^ r; ଫେରନ୍ତୁ l; } \
	} \
	static_assert (ସତ୍ୟ, "")

/* Useful for set-operations on small enums.
 * For example, for testing "x ∈ {x1, x2, x3}" use:
 * (FLAG_UNSAFE(x) & (FLAG(x1) | FLAG(x2) | FLAG(x3)))
 */
#define FLAG(x) (static_assert_ପଦସମଷ୍ଟି ((unsigned)(x) < 32) + (((uint32_t) 1U) << (unsigned)(x)))
#define FLAG_UNSAFE(x) ((unsigned)(x) < 32 ? (((uint32_t) 1U) << (unsigned)(x)) : 0)
#define FLAG_RANGE(x,y) (static_assert_ପଦସମଷ୍ଟି ((x) < (y)) + FLAG(y+1) - FLAG(x))
#define FLAG64(x) (static_assert_ପଦସମଷ୍ଟି ((unsigned)(x) < 64) + (((uint64_t) 1ULL) << (unsigned)(x)))
#define FLAG64_UNSAFE(x) ((unsigned)(x) < 64 ? (((uint64_t) 1ULL) << (unsigned)(x)) : 0)


/*
 * Fixed-endian integers / floats.
 */


/* Endian swap, used in Windows related backends */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ constexpr uint16_t hb_uint16_swap (uint16_t v)
{ ଫେରନ୍ତୁ (v >> 8) | (v << 8); }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ constexpr uint32_t hb_uint32_swap (uint32_t v)
{ ଫେରନ୍ତୁ (hb_uint16_swap (v) << 16) | hb_uint16_swap (v >> 16); }

template <typename Type>
ସଂରଚନା __attribute__((packed)) hb_packed_t
{
  hb_packed_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  constexpr hb_packed_t (Type V) : v (V) {}
  operator Type () ଅଚଳ { ଫେରନ୍ତୁ v; }
  hb_packed_t & operator = (Type V) { v = V; ଫେରନ୍ତୁ *ଏହି; }

  ବ୍ୟକ୍ତିଗତ:
  Type v;
};

#ifndef HB_FAST_ଅଙ୍କ_ACCESS

#ଯଦି defined(__OPTIMIZE__) && \
    defined(__BYTE_ORDER) && \
    (__BYTE_ORDER == __BIG_ENDIAN || \
     (__BYTE_ORDER == __LITTLE_ENDIAN && \
      hb_has_builtin(__builtin_bswap16) && \
      hb_has_builtin(__builtin_bswap32) && \
      hb_has_builtin(__builtin_bswap64)))
#define HB_FAST_ଅଙ୍କ_ACCESS 1
#ଅନ୍ୟଥା
#define HB_FAST_ଅଙ୍କ_ACCESS 0
#endif

// https://github.com/harfbuzz/harfbuzz/issues/5456
#ଯଦି defined(__GNUC__) && !defined(__clang__) && (__GNUC__ <= 12)
#undef HB_FAST_ଅଙ୍କ_ACCESS
#define HB_FAST_ଅଙ୍କ_ACCESS 0
#endif

#endif

template <ତାର୍କିକ BE, typename Type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା Bytes = ଆକାର (Type)>
ସଂରଚନା HBInt;
template <ତାର୍କିକ BE, typename Type>
ସଂରଚନା HBInt<BE, Type, 1>
{
  ସର୍ବସାଧାରଣ:
  HBInt () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  constexpr HBInt (Type V) : v {uint8_t (V)} {}
  constexpr operator Type () ଅଚଳ { ଫେରନ୍ତୁ v; }
  ବ୍ୟକ୍ତିଗତ: uint8_t v;
};
template <ତାର୍କିକ BE, typename Type>
ସଂରଚନା HBInt<BE, Type, 2>
{
  ସର୍ବସାଧାରଣ:
  HBInt () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

  HBInt (Type V)
#ଯଦି HB_FAST_ଅଙ୍କ_ACCESS
  {
    ଯଦି (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint16_t> *) v) = V;
    ଅନ୍ୟଥା
      *((hb_packed_t<uint16_t> *) v) = __builtin_bswap16 (V);
  }
#ଅନ୍ୟଥା
    : v {BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >>  8) & 0xFF)} {}
#endif

  constexpr operator Type () ଅଚଳ
  {
#ଯଦି HB_FAST_ଅଙ୍କ_ACCESS
    ଫେରନ୍ତୁ (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint16_t) *((ଅଚଳ hb_packed_t<uint16_t> *) v)
    :
      __builtin_bswap16 ((uint16_t) *((ଅଚଳ hb_packed_t<uint16_t> *) v))
    ;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ (BE ? (v[0] <<  8) : (v[0]      ))
	 + (BE ? (v[1]      ) : (v[1] <<  8));
#endif
  }
  ବ୍ୟକ୍ତିଗତ: uint8_t v[2];
};
template <ତାର୍କିକ BE, typename Type>
ସଂରଚନା HBInt<BE, Type, 3>
{
  static_assert (!std::is_signed<Type>::value, "");
  ସର୍ବସାଧାରଣ:
  HBInt () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  constexpr HBInt (Type V) : v {BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
				BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
				BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V      ) & 0xFF)} {}

  constexpr operator Type () ଅଚଳ { ଫେରନ୍ତୁ (BE ? (v[0] << 16) : (v[0]      ))
					  + (BE ? (v[1] <<  8) : (v[1] <<  8))
					  + (BE ? (v[2]      ) : (v[2] << 16)); }
  ବ୍ୟକ୍ତିଗତ: uint8_t v[3];
};
template <ତାର୍କିକ BE, typename Type>
ସଂରଚନା HBInt<BE, Type, 4>
{
  template <ତାର୍କିକ, typename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>
  friend ସଂରଚନା HBFloat;

  ସର୍ବସାଧାରଣ:
  HBInt () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

  HBInt (Type V)
#ଯଦି HB_FAST_ଅଙ୍କ_ACCESS
  {
    ଯଦି (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint32_t> *) v) = V;
    ଅନ୍ୟଥା
      *((hb_packed_t<uint32_t> *) v) = __builtin_bswap32 (V);
  }
#ଅନ୍ୟଥା
    : v {BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 24) & 0xFF)} {}
#endif

  constexpr operator Type () ଅଚଳ {
#ଯଦି HB_FAST_ଅଙ୍କ_ACCESS
    ଫେରନ୍ତୁ (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint32_t) *((ଅଚଳ hb_packed_t<uint32_t> *) v)
    :
      __builtin_bswap32 ((uint32_t) *((ଅଚଳ hb_packed_t<uint32_t> *) v))
    ;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ (BE ? (uint32_t (v[0]) << 24) : (uint32_t (v[0])      ))
	 + (BE ? (uint32_t (v[1]) << 16) : (uint32_t (v[1]) <<  8))
	 + (BE ? (uint32_t (v[2]) <<  8) : (uint32_t (v[2]) << 16))
	 + (BE ? (uint32_t (v[3])      ) : (uint32_t (v[3]) << 24));
#endif
  }
  ବ୍ୟକ୍ତିଗତ: uint8_t v[4];
};
template <ତାର୍କିକ BE, typename Type>
ସଂରଚନା HBInt<BE, Type, 8>
{
  template <ତାର୍କିକ, typename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>
  friend ସଂରଚନା HBFloat;

  ସର୍ବସାଧାରଣ:
  HBInt () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

  HBInt (Type V)
#ଯଦି HB_FAST_ଅଙ୍କ_ACCESS
  {
    ଯଦି (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint64_t> *) v) = V;
    ଅନ୍ୟଥା
      *((hb_packed_t<uint64_t> *) v) = __builtin_bswap64 (V);
  }
#ଅନ୍ୟଥା
    : v {BE ? uint8_t ((V >> 56) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 48) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >> 40) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V >> 32) & 0xFF) : uint8_t ((V >> 24) & 0xFF),
	 BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V >> 32) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 40) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 48) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 56) & 0xFF)} {}
#endif

  constexpr operator Type () ଅଚଳ {
#ଯଦି HB_FAST_ଅଙ୍କ_ACCESS
    ଫେରନ୍ତୁ (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint64_t) *((ଅଚଳ hb_packed_t<uint64_t> *) v)
    :
      __builtin_bswap64 ((uint64_t) *((ଅଚଳ hb_packed_t<uint64_t> *) v))
    ;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ (BE ? (uint64_t (v[0]) << 56) : (uint64_t (v[0])      ))
	 + (BE ? (uint64_t (v[1]) << 48) : (uint64_t (v[1]) <<  8))
	 + (BE ? (uint64_t (v[2]) << 40) : (uint64_t (v[2]) << 16))
	 + (BE ? (uint64_t (v[3]) << 32) : (uint64_t (v[3]) << 24))
	 + (BE ? (uint64_t (v[4]) << 24) : (uint64_t (v[4]) << 32))
	 + (BE ? (uint64_t (v[5]) << 16) : (uint64_t (v[5]) << 40))
	 + (BE ? (uint64_t (v[6]) <<  8) : (uint64_t (v[6]) << 48))
	 + (BE ? (uint64_t (v[7])      ) : (uint64_t (v[7]) << 56));
#endif
  }
  ବ୍ୟକ୍ତିଗତ: uint8_t v[8];
};

/* Floats. */

template <ତାର୍କିକ BE, typename Type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା Bytes>
ସଂରଚନା HBFloat
{
  using IntType = typename std::conditional<Bytes == 4, uint32_t, uint64_t>::type;

  ସର୍ବସାଧାରଣ:
  HBFloat () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

  HBFloat (Type V)
  {
#ଯଦି HB_FAST_ଅଙ୍କ_ACCESS
    {
	      ଯଦି (BE == (__BYTE_ORDER == __BIG_ENDIAN))
	      {
	        *((hb_packed_t<Type> *) v) = V;
	        ଫେରନ୍ତୁ;
	      }
	    }
#endif

    ସଂଘ {
      hb_packed_t<Type> f;
      hb_packed_t<IntType> i;
    } u = {{V}};

    ଅଚଳ HBInt<BE, IntType> I = (IntType) u.i;
    ପାଇଁ (unsigned i = 0; i < Bytes; i++)
      v[i] = I.v[i];
  }

  /* c++14 constexpr */ operator Type () ଅଚଳ
  {
#ଯଦି HB_FAST_ଅଙ୍କ_ACCESS
	    {
	      ଯଦି (BE == (__BYTE_ORDER == __BIG_ENDIAN))
		ଫେରନ୍ତୁ (Type) *((ଅଚଳ hb_packed_t<Type> *) v);
	    }
#endif

    HBInt<BE, IntType> I;
    ପାଇଁ (unsigned i = 0; i < Bytes; i++)
      I.v[i] = v[i];

    ସଂଘ {
      hb_packed_t<IntType> i;
      hb_packed_t<Type> f;
    } u = {{I}};

    ଫେରନ୍ତୁ (Type) u.f;
  }
  ବ୍ୟକ୍ତିଗତ: uint8_t v[Bytes];
};


/* We want our rounding towards +infinity. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ
_hb_roundf (ଦ୍ୱିଗୁଣ x) { ଫେରନ୍ତୁ floor (x + .5); }

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ
_hb_roundf (ଦଶମିକ x) { ଫେରନ୍ତୁ floor ((ଦ୍ୱିଗୁଣ) x + .5); }

#define roundf(x) _hb_roundf(x)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_sincos (ଦଶମିକ rotation, ଦଶମିକ &s, ଦଶମିକ &c)
{
#ifdef HAVE_SINCOSF
  sincosf (rotation, &s, &c);
#ଅନ୍ୟଥା
  c = cosf (rotation);
  s = sinf (rotation);
#endif
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_sincos (ଦ୍ୱିଗୁଣ rotation, ଦ୍ୱିଗୁଣ &s, ଦ୍ୱିଗୁଣ &c)
{
#ifdef HAVE_SINCOS
  sincos (rotation, &s, &c);
#ଅନ୍ୟଥା
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


ସଂରଚନା
{
  /* Note.  This is dangerous in that if it's passed an rvalue, it returns rvalue-reference. */
  template <typename T> constexpr auto
  operator () (T&& v) ଅଚଳ HB_AUTO_RETURN ( std::forward<T> (v) )
}
HB_FUNCOBJ (hb_identity);
ସଂରଚନା
{
  /* Like identity(), but only retains lvalue-references.  Rvalues are returned as rvalues. */
  template <typename T> constexpr T&
  operator () (T& v) ଅଚଳ { ଫେରନ୍ତୁ v; }

  template <typename T> constexpr hb_remove_reference<T>
  operator () (T&& v) ଅଚଳ { ଫେରନ୍ତୁ v; }
}
HB_FUNCOBJ (hb_lidentity);
ସଂରଚନା
{
  /* Like identity(), but always returns rvalue. */
  template <typename T> constexpr hb_remove_reference<T>
  operator () (T&& v) ଅଚଳ { ଫେରନ୍ତୁ v; }
}
HB_FUNCOBJ (hb_ridentity);

ସଂରଚନା
{
  template <typename T> constexpr ତାର୍କିକ
  operator () (T&& v) ଅଚଳ { ଫେରନ୍ତୁ ତାର୍କିକ (std::forward<T> (v)); }
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
			(ଶୂନ୍ୟ) ((h) ^= (h) >> 23),		\
			(ଶୂନ୍ୟ) ((h) *= 0x2127599bf4325c37ULL),	\
			(h) ^= (h) >> 47)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ uint64_t fasthash64(ଅଚଳ ଶୂନ୍ୟ *buf, size_t len, uint64_t seed)
{
	ସଂରଚନା __attribute__((packed)) packed_uint64_t { uint64_t v; };
	ଅଚଳ uint64_t    m = 0x880355f21e6d1965ULL;
	ଅଚଳ packed_uint64_t *pos = (ଅଚଳ packed_uint64_t *)buf;
	ଅଚଳ packed_uint64_t *end = pos + (len / 8);
	ଅଚଳ unsigned ଅକ୍ଷର *pos2;
	uint64_t h = seed ^ (len * m);
	uint64_t v;

#ifndef HB_OPTIMIZE_SIZE
	ଯଦି (((uintptr_t) pos & 7) == 0)
	{
	  ଯେତେବେଳେ (pos != end)
	  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
	    v  = * (ଅଚଳ uint64_t *) (pos++);
#pragma GCC diagnostic କାଢ଼
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}
	ଅନ୍ୟଥା
#endif
	{
	  ଯେତେବେଳେ (pos != end)
	  {
	    v  = pos++->v;
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}

	pos2 = (ଅଚଳ unsigned ଅକ୍ଷର*)pos;
	v = 0;

	ବାଛ (len & 7) {
	ସ୍ଥିତି 7: v ^= (uint64_t)pos2[6] << 48; HB_FALLTHROUGH;
	ସ୍ଥିତି 6: v ^= (uint64_t)pos2[5] << 40; HB_FALLTHROUGH;
	ସ୍ଥିତି 5: v ^= (uint64_t)pos2[4] << 32; HB_FALLTHROUGH;
	ସ୍ଥିତି 4: v ^= (uint64_t)pos2[3] << 24; HB_FALLTHROUGH;
	ସ୍ଥିତି 3: v ^= (uint64_t)pos2[2] << 16; HB_FALLTHROUGH;
	ସ୍ଥିତି 2: v ^= (uint64_t)pos2[1] <<  8; HB_FALLTHROUGH;
	ସ୍ଥିତି 1: v ^= (uint64_t)pos2[0];
		h ^= fasthash_mix(v);
		h *= m;
	}

	ଫେରନ୍ତୁ fasthash_mix(h);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ uint32_t fasthash32(ଅଚଳ ଶୂନ୍ୟ *buf, size_t len, uint32_t seed)
{
	// the following trick converts the 64-bit hashcode to Fermat
	// residue, which shall retain information from both the higher
	// and lower parts of hashcode.
        uint64_t h = fasthash64(buf, len, seed);
	ଫେରନ୍ତୁ h - (h >> 32);
}

ସଂରଚନା
{
  ବ୍ୟକ୍ତିଗତ:

  template <typename T> constexpr auto
  impl (ଅଚଳ T& v, hb_priority<2>) ଅଚଳ HB_RETURN (uint32_t, hb_deref (v).hash ())

  // Horrible: std:hash() of integers seems to be identity in gcc / clang?!
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  //
  // For performance characteristics see:
  // https://github.com/harfbuzz/harfbuzz/pull/4228#issuecomment-1565079537
  template <typename T,
	    hb_enable_if (std::is_integral<T>::value && ଆକାର (T) <= ଆକାର (uint32_t))> constexpr auto
  impl (ଅଚଳ T& v, hb_priority<1>) ଅଚଳ HB_RETURN (uint32_t, (uint32_t) v * 2654435761u /* Knuh's multiplicative hash */)
  template <typename T,
	    hb_enable_if (std::is_integral<T>::value && ଆକାର (T) > ଆକାର (uint32_t))> constexpr auto
  impl (ଅଚଳ T& v, hb_priority<1>) ଅଚଳ HB_RETURN (uint32_t, (uint32_t) (v ^ (v >> 32)) * 2654435761u /* Knuth's multiplicative hash */)

  template <typename T,
	    hb_enable_if (std::is_floating_point<T>::value)> constexpr auto
  impl (ଅଚଳ T& v, hb_priority<1>) ଅଚଳ HB_RETURN (uint32_t, fasthash32 (std::addressof (v), ଆକାର (T), 0xf437ffe6))

  template <typename T> constexpr auto
  impl (ଅଚଳ T& v, hb_priority<0>) ଅଚଳ HB_RETURN (uint32_t, std::hash<hb_decay<decltype (hb_deref (v))>>{} (hb_deref (v)))

  ସର୍ବସାଧାରଣ:

  template <typename T> constexpr auto
  operator () (ଅଚଳ T& v) ଅଚଳ HB_RETURN (uint32_t, impl (v, hb_prioritize))
}
HB_FUNCOBJ (hb_hash);


ସଂରଚନା
{
  ବ୍ୟକ୍ତିଗତ:

  /* Pointer-to-member-function. */
  template <typename Appl, typename T, typename ...Ts> auto
  impl (Appl&& a, hb_priority<2>, T &&v, Ts&&... ds) ଅଚଳ HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v)).*std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  /* Pointer-to-member. */
  template <typename Appl, typename T> auto
  impl (Appl&& a, hb_priority<1>, T &&v) ଅଚଳ HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v))).*std::forward<Appl> (a))

  /* Operator(). */
  template <typename Appl, typename ...Ts> auto
  impl (Appl&& a, hb_priority<0>, Ts&&... ds) ଅଚଳ HB_AUTO_RETURN
  (hb_deref (std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  ସର୍ବସାଧାରଣ:

  template <typename Appl, typename ...Ts> auto
  operator () (Appl&& a, Ts&&... ds) ଅଚଳ HB_AUTO_RETURN
  (
    impl (std::forward<Appl> (a),
	  hb_prioritize,
	  std::forward<Ts> (ds)...)
  )
}
HB_FUNCOBJ (hb_invoke);

template <unsigned Pos, typename Appl, typename V>
ସଂରଚନା hb_partial_t
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
    ଫେରନ୍ତୁ hb_invoke (std::forward<Appl> (a),
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
    ଫେରନ୍ତୁ hb_invoke (std::forward<Appl> (a),
		      std::forward<T0> (d0),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }

  ବ୍ୟକ୍ତିଗତ:
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
  decltype(auto) operator () (_T&& _v) ଅଚଳ \
  { ଫେରନ୍ତୁ hb_partial<Pos> (ଏହି, std::forward<_T> (_v)); } \
  static_assert (ସତ୍ୟ, "")
#ଅନ୍ୟଥା
/* https://github.com/harfbuzz/harfbuzz/issues/1724 */
#define HB_PARTIALIZE(Pos) \
  template <typename _T> \
  auto operator () (_T&& _v) ଅଚଳ HB_AUTO_RETURN \
  (hb_partial<Pos> (+ଏହି, std::forward<_T> (_v))) \
  static_assert (ସତ୍ୟ, "")
#endif


ସଂରଚନା
{
  ବ୍ୟକ୍ତିଗତ:

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<1>) ଅଚଳ HB_AUTO_RETURN
  (
    hb_deref (std::forward<Pred> (p)).has (std::forward<Val> (v))
  )

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<0>) ଅଚଳ HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Pred> (p),
	       std::forward<Val> (v))
  )

  ସର୍ବସାଧାରଣ:

  template <typename Pred, typename Val> auto
  operator () (Pred&& p, Val &&v) ଅଚଳ HB_RETURN (ତାର୍କିକ,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_has);

ସଂରଚନା
{
  ବ୍ୟକ୍ତିଗତ:

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<1>) ଅଚଳ HB_AUTO_RETURN
  (
    hb_has (std::forward<Pred> (p),
	    std::forward<Val> (v))
  )

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<0>) ଅଚଳ HB_AUTO_RETURN
  (
    std::forward<Pred> (p) == std::forward<Val> (v)
  )

  ସର୍ବସାଧାରଣ:

  template <typename Pred, typename Val> auto
  operator () (Pred&& p, Val &&v) ଅଚଳ HB_RETURN (ତାର୍କିକ,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_match);

ସଂରଚନା
{
  ବ୍ୟକ୍ତିଗତ:

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<2>) ଅଚଳ HB_AUTO_RETURN
  (
    hb_deref (std::forward<Proj> (f)).get (std::forward<Val> (v))
  )

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<1>) ଅଚଳ HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Proj> (f),
	       std::forward<Val> (v))
  )

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<0>) ଅଚଳ HB_AUTO_RETURN
  (
    std::forward<Proj> (f)[std::forward<Val> (v)]
  )

  ସର୍ବସାଧାରଣ:

  template <typename Proj, typename Val> auto
  operator () (Proj&& f, Val &&v) ଅଚଳ HB_AUTO_RETURN
  (
    impl (std::forward<Proj> (f),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_get);

ସଂରଚନା
{
  ବ୍ୟକ୍ତିଗତ:

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<3>) ଅଚଳ HB_AUTO_RETURN
  (
    std::forward<T2> (v2).cmp (std::forward<T1> (v1)) == 0
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<2>) ଅଚଳ HB_AUTO_RETURN
  (
    std::forward<T1> (v1).cmp (std::forward<T2> (v2)) == 0
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<1>) ଅଚଳ HB_AUTO_RETURN
  (
    std::forward<T1> (v1) == std::forward<T2> (v2)
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<0>) ଅଚଳ HB_AUTO_RETURN
  (
    std::forward<T2> (v2) == std::forward<T1> (v1)
  )

  ସର୍ବସାଧାରଣ:

  template <typename T1, typename T2> auto
  operator () (T1&& v1, T2 &&v2) ଅଚଳ HB_AUTO_RETURN
  (
    impl (std::forward<T1> (v1),
	  std::forward<T2> (v2),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_equal);

ସଂରଚନା
{
  template <typename T> ଶୂନ୍ୟ
  operator () (T& a, T& b) ଅଚଳ
  {
    using std::swap; // allow ADL
    swap (a, b);
  }
}
HB_FUNCOBJ (hb_swap);


template <typename T1, typename T2>
ସଂରଚନା hb_pair_t
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା T1 first_t;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା T2 second_t;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_pair_t<T1, T2> pair_t;

  template <typename U1 = T1, typename U2 = T2,
	    hb_enable_if (std::is_default_constructible<U1>::value &&
			  std::is_default_constructible<U2>::value)>
  hb_pair_t () : first (), second () {}
  hb_pair_t (T1 a, T2 b) : first (std::forward<T1> (a)), second (std::forward<T2> (b)) {}

  template <typename Q1, typename Q2,
	    hb_enable_if (hb_is_convertible (T1, Q1) &&
			  hb_is_convertible (T2, Q2))>
  operator hb_pair_t<Q1, Q2> () { ଫେରନ୍ତୁ hb_pair_t<Q1, Q2> (first, second); }

  hb_pair_t<T1, T2> reverse () ଅଚଳ
  { ଫେରନ୍ତୁ hb_pair_t<T1, T2> (second, first); }

  ତାର୍କିକ operator == (ଅଚଳ pair_t& o) ଅଚଳ { ଫେରନ୍ତୁ first == o.first && second == o.second; }
  ତାର୍କିକ operator != (ଅଚଳ pair_t& o) ଅଚଳ { ଫେରନ୍ତୁ !(*ଏହି == o); }
  ତାର୍କିକ operator < (ଅଚଳ pair_t& o) ଅଚଳ { ଫେରନ୍ତୁ first < o.first || (first == o.first && second < o.second); }
  ତାର୍କିକ operator >= (ଅଚଳ pair_t& o) ଅଚଳ { ଫେରନ୍ତୁ !(*ଏହି < o); }
  ତାର୍କିକ operator > (ଅଚଳ pair_t& o) ଅଚଳ { ଫେରନ୍ତୁ first > o.first || (first == o.first && second > o.second); }
  ତାର୍କିକ operator <= (ଅଚଳ pair_t& o) ଅଚଳ { ଫେରନ୍ତୁ !(*ଏହି > o); }

  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb)
  {
    pair_t *a = (pair_t *) pa;
    pair_t *b = (pair_t *) pb;

    ଯଦି (a->first < b->first) ଫେରନ୍ତୁ -1;
    ଯଦି (a->first > b->first) ଫେରନ୍ତୁ +1;
    ଯଦି (a->second < b->second) ଫେରନ୍ତୁ -1;
    ଯଦି (a->second > b->second) ଫେରନ୍ତୁ +1;
    ଫେରନ୍ତୁ 0;
  }

  friend ଶୂନ୍ୟ swap (hb_pair_t& a, hb_pair_t& b) noexcept
  {
    hb_swap (a.first, b.first);
    hb_swap (a.second, b.second);
  }


  T1 first;
  T2 second;
};
template <typename T1, typename T2> ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_pair_t<T1, T2>
hb_pair (T1&& a, T2&& b) { ଫେରନ୍ତୁ hb_pair_t<T1, T2> (a, b); }

ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_pair_t<hb_codepoint_t, hb_codepoint_t> hb_codepoint_pair_t;

ସଂରଚନା
{
  template <typename Pair> constexpr typename Pair::first_t
  operator () (ଅଚଳ Pair& pair) ଅଚଳ { ଫେରନ୍ତୁ pair.first; }
}
HB_FUNCOBJ (hb_first);

ସଂରଚନା
{
  template <typename Pair> constexpr typename Pair::second_t
  operator () (ଅଚଳ Pair& pair) ଅଚଳ { ଫେରନ୍ତୁ pair.second; }
}
HB_FUNCOBJ (hb_second);

/* Note.  In min/max impl, we can use hb_type_identity<T> for second argument.
 * However, that would silently convert between different-signedness integers.
 * Instead we accept two different types, such that compiler can err if
 * comparing integers of different signedness. */
ସଂରଚନା
{
  template <typename T, typename T2> constexpr auto
  operator () (T&& a, T2&& b) ଅଚଳ HB_AUTO_RETURN
  (a <= b ? a : b)
}
HB_FUNCOBJ (hb_min);
ସଂରଚନା
{
  template <typename T, typename T2> constexpr auto
  operator () (T&& a, T2&& b) ଅଚଳ HB_AUTO_RETURN
  (a >= b ? a : b)
}
HB_FUNCOBJ (hb_max);
ସଂରଚନା
{
  template <typename T, typename T2, typename T3> constexpr auto
  operator () (T&& x, T2&& min, T3&& max) ଅଚଳ HB_AUTO_RETURN
  (hb_min (hb_max (std::forward<T> (x), std::forward<T2> (min)), std::forward<T3> (max)))
}
HB_FUNCOBJ (hb_clamp);

/* Signed saturating arithmetic. */
template <typename T>
ସ୍ଥିର HB_ALWAYS_INLINE T
hb_saturate_add (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#ଯଦି hb_has_builtin(__builtin_add_overflow)
  T result;
  ଯଦି (likely (!__builtin_add_overflow (a, b, &result)))
    ଫେରନ୍ତୁ result;
#ଅନ୍ୟଥା
  ଯଦି (b > 0)
  {
    ଯଦି (likely (a <= hb_int_max (T) - b))
      ଫେରନ୍ତୁ a + b;
  }
  ଅନ୍ୟଥା ଯଦି (likely (a >= hb_int_min (T) - b))
    ଫେରନ୍ତୁ a + b;
#endif

  ଫେରନ୍ତୁ a < 0 ? hb_int_min (T) : hb_int_max (T);
}

template <typename T>
ସ୍ଥିର HB_ALWAYS_INLINE T
hb_saturate_sub (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#ଯଦି hb_has_builtin(__builtin_sub_overflow)
  T result;
  ଯଦି (likely (!__builtin_sub_overflow (a, b, &result)))
    ଫେରନ୍ତୁ result;
#ଅନ୍ୟଥା
  ଯଦି (b > 0)
  {
    ଯଦି (likely (a >= hb_int_min (T) + b))
      ଫେରନ୍ତୁ a - b;
  }
  ଅନ୍ୟଥା ଯଦି (likely (a <= hb_int_max (T) + b))
    ଫେରନ୍ତୁ a - b;
#endif

  ଫେରନ୍ତୁ b < 0 ? hb_int_max (T) : hb_int_min (T);
}

template <typename T>
ସ୍ଥିର HB_ALWAYS_INLINE T
hb_saturate_neg (T a)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  ଫେରନ୍ତୁ hb_saturate_sub ((T) 0, a);
}

/* Convert a floating-point value to integer type T, saturating to T's
 * range instead of relying on the undefined behavior of an out-of-range
 * float-to-int conversion.  NaN saturates to the minimum of T. */
template <typename T>
ସ୍ଥିର HB_ALWAYS_INLINE T
hb_clamp_to (ଦ୍ୱିଗୁଣ v)
{
  ଫେରନ୍ତୁ (T) hb_clamp (v,
		       (ଦ୍ୱିଗୁଣ) hb_int_min (T),
		       (ଦ୍ୱିଗୁଣ) hb_int_max (T));
}

template <typename T>
ସ୍ଥିର HB_ALWAYS_INLINE T
hb_clamp_to (ଦଶମିକ v)
{
  /* Widen explicitly: float cannot represent all integer bounds exactly,
   * so the clamp itself is done in double. */
  ଫେରନ୍ତୁ hb_clamp_to<T> ((ଦ୍ୱିଗୁଣ) v);
}

template <typename T>
ସ୍ଥିର HB_ALWAYS_INLINE T
hb_clamp_to (int64_t v)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  ଫେରନ୍ତୁ (T) hb_clamp (v,
		       (int64_t) hb_int_min (T),
		       (int64_t) hb_int_max (T));
}

/*
 * Bithacks.
 */

/* Return the number of 1 bits in a uint8_t; faster than hb_popcount() */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned
hb_popcount8 (uint8_t v)
{
  ସ୍ଥିର ଅଚଳ uint8_t popcount4[16] = {
    0, 1, 1, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 2, 3, 3, 4
  };
  ଫେରନ୍ତୁ popcount4[v & 0xF] + popcount4[v >> 4];
}

/* Return the number of 1 bits in v. */
template <typename T>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_popcount (T v)
{
#ଯଦି hb_has_builtin(__builtin_popcount)
  ଯଦି (ଆକାର (T) <= ଆକାର (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
    ଫେରନ୍ତୁ __builtin_popcount (v);
#endif

#ଯଦି hb_has_builtin(__builtin_popcountl)
  ଯଦି (ଆକାର (T) <= ଆକାର (unsigned long))
    ଫେରନ୍ତୁ __builtin_popcountl (v);
#endif

#ଯଦି hb_has_builtin(__builtin_popcountll)
  ଯଦି (ଆକାର (T) <= ଆକାର (unsigned long long))
    ଫେରନ୍ତୁ __builtin_popcountll (v);
#endif

  ଯଦି (ଆକାର (T) <= 4)
  {
    /* "HACKMEM 169" */
    uint32_t y;
    y = (v >> 1) &033333333333;
    y = v - y - ((y >>1) & 033333333333);
    ଫେରନ୍ତୁ (((y + (y >> 3)) & 030707070707) % 077);
  }

  ଯଦି (ଆକାର (T) == 8)
  {
    uint64_t y = (uint64_t) v;
    y -= ((y >> 1) & 0x5555555555555555ull);
    y = (y & 0x3333333333333333ull) + (y >> 2 & 0x3333333333333333ull);
    ଫେରନ୍ତୁ ((y + (y >> 4)) & 0xf0f0f0f0f0f0f0full) * 0x101010101010101ull >> 56;
  }

  ଯଦି (ଆକାର (T) == 16)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା shift = 64;
    ଫେରନ୍ତୁ hb_popcount<uint64_t> ((uint64_t) v) + hb_popcount ((uint64_t) (v >> shift));
  }

  ପ୍ରତିଜ୍ଞା (0);
  ଫେରନ୍ତୁ 0; /* Shut up stupid compiler. */
}

/* Returns the number of bits needed to store number */
template <typename T>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_bit_storage (T v)
{
  ଯଦି (unlikely (!v)) ଫେରନ୍ତୁ 0;

#ଯଦି hb_has_builtin(__builtin_clz)
  ଯଦି (ଆକାର (T) <= ଆକାର (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
    ଫେରନ୍ତୁ ଆକାର (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) * 8 - __builtin_clz (v);
#endif

#ଯଦି hb_has_builtin(__builtin_clzl)
  ଯଦି (ଆକାର (T) <= ଆକାର (unsigned long))
    ଫେରନ୍ତୁ ଆକାର (unsigned long) * 8 - __builtin_clzl (v);
#endif

#ଯଦି hb_has_builtin(__builtin_clzll)
  ଯଦି (ଆକାର (T) <= ଆକାର (unsigned long long))
    ଫେରନ୍ତୁ ଆକାର (unsigned long long) * 8 - __builtin_clzll (v);
#endif

#ଯଦି (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  ଯଦି (ଆକାର (T) <= ଆକାର (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
  {
    unsigned long where;
    _BitScanReverse (&where, v);
    ଫେରନ୍ତୁ 1 + where;
  }
# ଯଦି defined(_WIN64)
  ଯଦି (ଆକାର (T) <= 8)
  {
    unsigned long where;
    _BitScanReverse64 (&where, v);
    ଫେରନ୍ତୁ 1 + where;
  }
# endif
#endif

  ଯଦି (ଆକାର (T) <= 4)
  {
    /* "bithacks" */
    ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା S[] = {1, 2, 4, 8, 16};
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 4; i >= 0; i--)
      ଯଦି (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    ଫେରନ୍ତୁ r + 1;
  }
  ଯଦି (ଆକାର (T) <= 8)
  {
    /* "bithacks" */
    ଅଚଳ uint64_t b[] = {0x2ULL, 0xCULL, 0xF0ULL, 0xFF00ULL, 0xFFFF0000ULL, 0xFFFFFFFF00000000ULL};
    ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା S[] = {1, 2, 4, 8, 16, 32};
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 5; i >= 0; i--)
      ଯଦି (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    ଫେରନ୍ତୁ r + 1;
  }
  ଯଦି (ଆକାର (T) == 16)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା shift = 64;
    ଫେରନ୍ତୁ (v >> shift) ? hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift :
			  hb_bit_storage<uint64_t> ((uint64_t) v);
  }

  ପ୍ରତିଜ୍ଞା (0);
  ଫେରନ୍ତୁ 0; /* Shut up stupid compiler. */
}

/* Returns the number of zero bits in the least significant side of v */
template <typename T>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_ctz (T v)
{
  ଯଦି (unlikely (!v)) ଫେରନ୍ତୁ 8 * ଆକାର (T);

#ଯଦି hb_has_builtin(__builtin_ctz)
  ଯଦି (ଆକାର (T) <= ଆକାର (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
    ଫେରନ୍ତୁ __builtin_ctz (v);
#endif

#ଯଦି hb_has_builtin(__builtin_ctzl)
  ଯଦି (ଆକାର (T) <= ଆକାର (unsigned long))
    ଫେରନ୍ତୁ __builtin_ctzl (v);
#endif

#ଯଦି hb_has_builtin(__builtin_ctzll)
  ଯଦି (ଆକାର (T) <= ଆକାର (unsigned long long))
    ଫେରନ୍ତୁ __builtin_ctzll (v);
#endif

#ଯଦି (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  ଯଦି (ଆକାର (T) <= ଆକାର (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
  {
    unsigned long where;
    _BitScanForward (&where, v);
    ଫେରନ୍ତୁ where;
  }
# ଯଦି defined(_WIN64)
  ଯଦି (ଆକାର (T) <= 8)
  {
    unsigned long where;
    _BitScanForward64 (&where, v);
    ଫେରନ୍ତୁ where;
  }
# endif
#endif

  ଯଦି (ଆକାର (T) <= 4)
  {
    /* "bithacks" */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = 32;
    v &= - (int32_t) v;
    ଯଦି (v) c--;
    ଯଦି (v & 0x0000FFFF) c -= 16;
    ଯଦି (v & 0x00FF00FF) c -= 8;
    ଯଦି (v & 0x0F0F0F0F) c -= 4;
    ଯଦି (v & 0x33333333) c -= 2;
    ଯଦି (v & 0x55555555) c -= 1;
    ଫେରନ୍ତୁ c;
  }
  ଯଦି (ଆକାର (T) <= 8)
  {
    /* "bithacks" */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = 64;
    v &= - (int64_t) (v);
    ଯଦି (v) c--;
    ଯଦି (v & 0x00000000FFFFFFFFULL) c -= 32;
    ଯଦି (v & 0x0000FFFF0000FFFFULL) c -= 16;
    ଯଦି (v & 0x00FF00FF00FF00FFULL) c -= 8;
    ଯଦି (v & 0x0F0F0F0F0F0F0F0FULL) c -= 4;
    ଯଦି (v & 0x3333333333333333ULL) c -= 2;
    ଯଦି (v & 0x5555555555555555ULL) c -= 1;
    ଫେରନ୍ତୁ c;
  }
  ଯଦି (ଆକାର (T) == 16)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା shift = 64;
    ଫେରନ୍ତୁ (uint64_t) v ? hb_bit_storage<uint64_t> ((uint64_t) v) :
			  hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift;
  }

  ପ୍ରତିଜ୍ଞା (0);
  ଫେରନ୍ତୁ 0; /* Shut up stupid compiler. */
}


/*
 * Tiny stuff.
 */

/* ASCII tag/character handling */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ ISALPHA (unsigned ଅକ୍ଷର c)
{ ଫେରନ୍ତୁ (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ ISALNUM (unsigned ଅକ୍ଷର c)
{ ଫେରନ୍ତୁ (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ ISSPACE (unsigned ଅକ୍ଷର c)
{ ଫେରନ୍ତୁ c == ' ' || c =='\f'|| c =='\n'|| c =='\r'|| c =='\t'|| c =='\v'; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ଅକ୍ଷର TOUPPER (unsigned ଅକ୍ଷର c)
{ ଫେରନ୍ତୁ (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ଅକ୍ଷର TOLOWER (unsigned ଅକ୍ଷର c)
{ ଫେରନ୍ତୁ (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ ISHEX (unsigned ଅକ୍ଷର c)
{ ଫେରନ୍ତୁ (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ଅକ୍ଷର TOHEX (uint8_t c)
{ ଫେରନ୍ତୁ (c & 0xF) <= 9 ? (c & 0xF) + '0' : (c & 0xF) + 'a' - 10; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ uint8_t FROMHEX (unsigned ଅକ୍ଷର c)
{ ଫେରନ୍ତୁ (c >= '0' && c <= '9') ? c - '0' : TOLOWER (c) - 'a' + 10; }

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା DIV_CEIL (ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା a, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା b)
{ ଫେରନ୍ତୁ (a + (b - 1)) / b; }


#undef  ARRAY_LENGTH
template <typename Type, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ARRAY_LENGTH (ଅଚଳ Type (&)[n]) { ଫେରନ୍ତୁ n; }
/* A const version, but does not detect erratically being called on pointers. */
#define ARRAY_LENGTH_CONST(__array) ((signed ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (ଆକାର (__array) / ଆକାର (__array[0])))


ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ *
hb_memcpy (ଶୂନ୍ୟ *__restrict dst, ଅଚଳ ଶୂନ୍ୟ *__restrict src, size_t len)
{
  /* It's illegal to pass 0 as size to memcpy. */
  ଯଦି (unlikely (!len)) ଫେରନ୍ତୁ dst;
  ଫେରନ୍ତୁ memcpy (dst, src, len);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_memcmp (ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
  /* It's illegal to pass NULL to memcmp(), even if len is zero.
   * So, wrap it.
   * https://sourceware.org/bugzilla/show_bug.cgi?id=23878 */
  ଯଦି (unlikely (!len)) ଫେରନ୍ତୁ 0;
  ଫେରନ୍ତୁ memcmp (a, b, len);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ *
hb_memset (ଶୂନ୍ୟ *s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
  /* It's illegal to pass NULL to memset(), even if n is zero. */
  ଯଦି (unlikely (!n)) ଫେରନ୍ତୁ s;
  ଫେରନ୍ତୁ memset (s, c, n);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_ceil_to_4 (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା v)
{
  ଫେରନ୍ତୁ ((v - 1) | 3) + 1;
}

template <typename T> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
hb_in_range (T u, T lo, T hi)
{
  static_assert (!std::is_signed<T>::value, "");

  /* The casts below are important as if T is smaller than int,
   * the subtract results will become a signed int! */
  ଫେରନ୍ତୁ (T)(u - lo) <= (T)(hi - lo);
}
template <typename T> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
hb_in_ranges (T u, T lo1, T hi1)
{
  ଫେରନ୍ତୁ hb_in_range (u, lo1, hi1);
}
template <typename T, typename ...Ts> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
hb_in_ranges (T u, T lo1, T hi1, Ts... ds)
{
  ଫେରନ୍ତୁ hb_in_range<T> (u, lo1, hi1) || hb_in_ranges<T> (u, ds...);
}


/*
 * Overflow checking.
 */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
hb_unsigned_mul_overflows (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, unsigned *result = nullptr)
{
#ଯଦି hb_has_builtin(__builtin_mul_overflow)
  unsigned ଗଦା_result;
  ଯଦି (!result)
    result = &ଗଦା_result;
  ଫେରନ୍ତୁ __builtin_mul_overflow (ଗଣନା_2, size, result);
#endif

  ଯଦି (result)
    *result = ଗଣନା_2 * size;
  ଫେରନ୍ତୁ (size > 0) && (ଗଣନା_2 >= ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) -1) / size);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
hb_unsigned_add_overflows (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା a, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା b, unsigned *result = nullptr)
{
#ଯଦି hb_has_builtin(__builtin_add_overflow)
  unsigned ଗଦା_result;
  ଯଦି (!result)
    result = &ଗଦା_result;
  ଫେରନ୍ତୁ __builtin_add_overflow (a, b, result);
#endif

  ଯଦି (result)
    *result = a + b;
  ଫେରନ୍ତୁ b > (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) -1 - a;
}

/* Saturating arithmetic on size_t.  On platforms where size_t is wider than
 * unsigned int (i.e. 64-bit), the inputs to get_size()-style computations
 * (counts and static_sizes which are at most 32-bit) cannot overflow size_t,
 * so these reduce to plain arithmetic.  On 32-bit platforms (size_t is
 * unsigned int), they saturate to SIZE_MAX so sanitize/serialize callers
 * naturally reject the resulting size as out-of-range. */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ size_t
hb_unsigned_mul_saturate (size_t a, size_t b)
{
  ଯଦି (ଆକାର (size_t) > ଆକାର (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
    ଫେରନ୍ତୁ a * b;
#ଯଦି hb_has_builtin(__builtin_mul_overflow)
  size_t result;
  ଯଦି (__builtin_mul_overflow (a, b, &result))
    ଫେରନ୍ତୁ (size_t) -1;
  ଫେରନ୍ତୁ result;
#ଅନ୍ୟଥା
  ଯଦି (b > 0 && a > ((size_t) -1) / b) ଫେରନ୍ତୁ (size_t) -1;
  ଫେରନ୍ତୁ a * b;
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ size_t
hb_unsigned_add_saturate (size_t a, size_t b)
{
  ଯଦି (ଆକାର (size_t) > ଆକାର (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା))
    ଫେରନ୍ତୁ a + b;
#ଯଦି hb_has_builtin(__builtin_add_overflow)
  size_t result;
  ଯଦି (__builtin_add_overflow (a, b, &result))
    ଫେରନ୍ତୁ (size_t) -1;
  ଫେରନ୍ତୁ result;
#ଅନ୍ୟଥା
  ଯଦି (b > ((size_t) -1) - a) ଫେରନ୍ତୁ (size_t) -1;
  ଫେରନ୍ତୁ a + b;
#endif
}

/* Variadic forms: fold left across all arguments. */
template <typename ...Ts>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ size_t
hb_unsigned_mul_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ ଫେରନ୍ତୁ hb_unsigned_mul_saturate (hb_unsigned_mul_saturate (a, b), c, rest...); }

template <typename ...Ts>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ size_t
hb_unsigned_add_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ ଫେରନ୍ତୁ hb_unsigned_add_saturate (hb_unsigned_add_saturate (a, b), c, rest...); }

/* Saturating mul-add: a * b + c.  Covers the dominant get_size() pattern
 * `count * static_size + min_size`. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ size_t
hb_unsigned_mul_add_saturate (size_t a, size_t b, size_t c)
{ ଫେରନ୍ତୁ hb_unsigned_add_saturate (hb_unsigned_mul_saturate (a, b), c); }


/*
 * Sort and search.
 */

template <typename K, typename V, typename ...Ts>
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
_hb_cmp_method (ଅଚଳ ଶୂନ୍ୟ *pkey, ଅଚଳ ଶୂନ୍ୟ *pval, Ts... ds)
{
  ଅଚଳ K& key = * (ଅଚଳ K*) pkey;
  ଅଚଳ V& val = * (ଅଚଳ V*) pval;

  ଫେରନ୍ତୁ val.cmp (key, ds...);
}

template <typename K, typename V>
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
_hb_cmp_operator (ଅଚଳ ଶୂନ୍ୟ *pkey, ଅଚଳ ଶୂନ୍ୟ *pval)
{
  ଅଚଳ K& key = * (ଅଚଳ K*) pkey;
  ଅଚଳ V& val = * (ଅଚଳ V*) pval;

  ଯଦି (key < val) ଫେରନ୍ତୁ -1;
  ଯଦି (key > val) ଫେରନ୍ତୁ  1;
  ଫେରନ୍ତୁ 0;
}

template <typename V, typename K, typename ...Ts>
HB_HOT
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
hb_bsearch_impl (unsigned *pos, /* Out */
		 ଅଚଳ K& key,
		 V* base, size_t nmemb, size_t stride,
		 ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*compar)(ଅଚଳ ଶୂନ୍ୟ *ଚାବି_3, ଅଚଳ ଶୂନ୍ୟ *_item, Ts... _ds),
		 Ts... ds)
{
  /* This is our *only* bsearch implementation. */

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା min = 0, max = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) nmemb - 1;
  ଯେତେବେଳେ (min <= max)
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା mid = ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) min + (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) max) / 2;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    V* p = (V*) (((ଅଚଳ ଅକ୍ଷର *) base) + (mid * stride));
#pragma GCC diagnostic କାଢ଼
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = compar ((ଅଚଳ ଶୂନ୍ୟ *) std::addressof (key), (ଅଚଳ ଶୂନ୍ୟ *) p, ds...);
    ଯଦି (c < 0)
      max = mid - 1;
    ଅନ୍ୟଥା ଯଦି (c > 0)
      min = mid + 1;
    ଅନ୍ୟଥା
    {
      *pos = mid;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
  }
  *pos = min;
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

template <typename V, typename K>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ V*
hb_bsearch (ଅଚଳ K& key, V* base,
	    size_t nmemb, size_t stride = ଆକାର (V),
	    ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*compar)(ଅଚଳ ଶୂନ୍ୟ *ଚାବି_3, ଅଚଳ ଶୂନ୍ୟ *_item) = _hb_cmp_method<K, V>)
{
  unsigned pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  ଫେରନ୍ତୁ hb_bsearch_impl (&pos, key, base, nmemb, stride, compar) ?
	 (V*) (((ଅଚଳ ଅକ୍ଷର *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic କାଢ଼
}
template <typename V, typename K, typename ...Ts>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ V*
hb_bsearch (ଅଚଳ K& key, V* base,
	    size_t nmemb, size_t stride,
	    ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*compar)(ଅଚଳ ଶୂନ୍ୟ *ଚାବି_3, ଅଚଳ ଶୂନ୍ୟ *_item, Ts... _ds),
	    Ts... ds)
{
  unsigned pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  ଫେରନ୍ତୁ hb_bsearch_impl (&pos, key, base, nmemb, stride, compar, ds...) ?
	 (V*) (((ଅଚଳ ଅକ୍ଷର *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic କାଢ଼
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_qsort_loop (T *base, size_t nel, Compar compar)
{
  ଯେତେବେଳେ (nel > 24)
  {
    T *last = base + nel - 1;
    T *mid = base + nel / 2;

    /* Median-of-three pivot, parked at last-1. */
    ଯଦି (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    ଯଦି (compar (*mid, *last) > 0)
    {
      hb_swap (*mid, *last);
      ଯଦି (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    }
    hb_swap (*mid, *(last - 1));
    T &pivot = *(last - 1);

    /* Two-way Hoare partition.  Inner loops are unguarded:
     * median-of-three left *base <= pivot and *last >= pivot,
     * which act as sentinels. */
    T *i = base, *j = last - 1;
    ଯେତେବେଳେ (ସତ୍ୟ)
    {
      ଯେତେବେଳେ (compar (*++i, pivot) < 0) {}
      ଯେତେବେଳେ (compar (*--j, pivot) > 0) {}
      ଯଦି (i >= j) ଭାଙ୍ଗ;
      hb_swap (*i, *j);
    }
    hb_swap (*i, *(last - 1));

    /* Recurse on smaller side, loop on larger — bounds stack
     * depth at O(log n). */
    size_t left  = (size_t) (i - base);
    size_t ଡାହାଣ = nel - left - 1;
    ଯଦି (left < ଡାହାଣ)
    {
      hb_qsort_loop (base, left, compar);
      base = i + 1;
      nel  = ଡାହାଣ;
    }
    ଅନ୍ୟଥା
    {
      hb_qsort_loop (i + 1, ଡାହାଣ, compar);
      nel  = left;
    }
  }
}

template <typename T, typename Compar>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_qsort_inline (T *base, size_t nel, Compar compar)
{
  hb_qsort_loop (base, nel, compar);

  /* Single final insertion sort over the whole array.  After
   * the partitioning loop, every element is within the threshold
   * of its sorted position, so this pass is O(n * threshold). */
  T *end = base + nel;
  ପାଇଁ (T *pi = base + 1; pi < end; pi++)
    ପାଇଁ (T *pj = pi; pj > base && compar (pj[-1], pj[0]) > 0; pj--)
      hb_swap (pj[-1], pj[0]);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_qsort (ଶୂନ୍ୟ *base, size_t nel, size_t width,
	  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*compar)(ଅଚଳ ଶୂନ୍ୟ *_a, ଅଚଳ ଶୂନ୍ୟ *_b))
{
  qsort (base, nel, width, compar);
}




template <typename T, typename T2, typename T3 = ପୂର୍ଣ୍ଣସଂଖ୍ୟା> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_stable_sort (T *array, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା(*compar)(ଅଚଳ T2 *, ଅଚଳ T2 *), T3 *array2 = nullptr)
{
  static_assert (hb_is_trivially_copy_assignable (T), "");
  static_assert (hb_is_trivially_copy_assignable (T3), "");

  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < len; i++)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = i;
    ଯେତେବେଳେ (j && compar (&array[j - 1], &array[i]) > 0)
      j--;
    ଯଦି (i == j)
      ଜାରି;
    /* Move item i to occupy place for item j, shift what's in between. */
    {
      T t = array[i];
      memmove (&array[j + 1], &array[j], (i - j) * ଆକାର (T));
      array[j] = t;
    }
    ଯଦି (array2)
    {
      T3 t = array2[i];
      memmove (&array2[j + 1], &array2[j], (i - j) * ଆକାର (T3));
      array2[j] = t;
    }
  }
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_bool_t
hb_codepoint_parse (ଅଚଳ ଅକ୍ଷର *s, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା base, hb_codepoint_t *out)
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା v;
  ଅଚଳ ଅକ୍ଷର *p = s;
  ଅଚଳ ଅକ୍ଷର *end = p + len;
  ଯଦି (unlikely (!hb_parse_uint (&p, end, &v, ସତ୍ୟ/* whole buffer */, base)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  *out = v;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}


/* Operators. */

ସଂରଚନା
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ଅଚଳ T &a, ଅଚଳ T &b) ଅଚଳ HB_AUTO_RETURN (a & b)
}
HB_FUNCOBJ (hb_bitwise_and);
ସଂରଚନା
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ଅଚଳ T &a, ଅଚଳ T &b) ଅଚଳ HB_AUTO_RETURN (a | b)
}
HB_FUNCOBJ (hb_bitwise_or);
ସଂରଚନା
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ଅଚଳ T &a, ଅଚଳ T &b) ଅଚଳ HB_AUTO_RETURN (a ^ b)
}
HB_FUNCOBJ (hb_bitwise_xor);
ସଂରଚନା
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ଅଚଳ T &a, ଅଚଳ T &b) ଅଚଳ HB_AUTO_RETURN (~a & b)
}
HB_FUNCOBJ (hb_bitwise_lt);
ସଂରଚନା
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ଅଚଳ T &a, ଅଚଳ T &b) ଅଚଳ HB_AUTO_RETURN (a & ~b)
}
HB_FUNCOBJ (hb_bitwise_gt); // aka sub
ସଂରଚନା
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ଅଚଳ T &a, ଅଚଳ T &b) ଅଚଳ HB_AUTO_RETURN (~a | b)
}
HB_FUNCOBJ (hb_bitwise_le);
ସଂରଚନା
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (ଅଚଳ T &a, ଅଚଳ T &b) ଅଚଳ HB_AUTO_RETURN (a | ~b)
}
HB_FUNCOBJ (hb_bitwise_ge);
ସଂରଚନା
{
  template <typename T> constexpr auto
  operator () (ଅଚଳ T &a) ଅଚଳ HB_AUTO_RETURN (~a)
}
HB_FUNCOBJ (hb_bitwise_neg);

ସଂରଚନା
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ଅଚଳ T &a, ଅଚଳ T2 &b) ଅଚଳ HB_AUTO_RETURN (a + b)
}
HB_FUNCOBJ (hb_add);
ସଂରଚନା
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ଅଚଳ T &a, ଅଚଳ T2 &b) ଅଚଳ HB_AUTO_RETURN (a - b)
}
HB_FUNCOBJ (hb_sub);
ସଂରଚନା
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ଅଚଳ T &a, ଅଚଳ T2 &b) ଅଚଳ HB_AUTO_RETURN (b - a)
}
HB_FUNCOBJ (hb_rsub);
ସଂରଚନା
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ଅଚଳ T &a, ଅଚଳ T2 &b) ଅଚଳ HB_AUTO_RETURN (a * b)
}
HB_FUNCOBJ (hb_mul);
ସଂରଚନା
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ଅଚଳ T &a, ଅଚଳ T2 &b) ଅଚଳ HB_AUTO_RETURN (a / b)
}
HB_FUNCOBJ (hb_div);
ସଂରଚନା
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (ଅଚଳ T &a, ଅଚଳ T2 &b) ଅଚଳ HB_AUTO_RETURN (a % b)
}
HB_FUNCOBJ (hb_mod);
ସଂରଚନା
{
  template <typename T> constexpr auto
  operator () (ଅଚଳ T &a) ଅଚଳ HB_AUTO_RETURN (+a)
}
HB_FUNCOBJ (hb_pos);
ସଂରଚନା
{
  template <typename T> constexpr auto
  operator () (ଅଚଳ T &a) ଅଚଳ HB_AUTO_RETURN (-a)
}
HB_FUNCOBJ (hb_neg);
ସଂରଚନା
{
  template <typename T> constexpr auto
  operator () (T &a) ଅଚଳ HB_AUTO_RETURN (++a)
}
HB_FUNCOBJ (hb_inc);
ସଂରଚନା
{
  template <typename T> constexpr auto
  operator () (T &a) ଅଚଳ HB_AUTO_RETURN (--a)
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
ଦ୍ୱିଗୁଣ solve_itp (func_t f,
		  ଦ୍ୱିଗୁଣ a, ଦ୍ୱିଗୁଣ b,
		  ଦ୍ୱିଗୁଣ epsilon,
		  ଦ୍ୱିଗୁଣ min_y, ଦ୍ୱିଗୁଣ max_y,
		  ଦ୍ୱିଗୁଣ &ya, ଦ୍ୱିଗୁଣ &yb, ଦ୍ୱିଗୁଣ &y)
{
  // Guard against degenerate interval
  ଯଦି (b - a <= 0.0)
  {
    y = ya;
    ଫେରନ୍ତୁ a;
  }

  unsigned n1_2 = (unsigned) (hb_max (ceil (log2 ((b - a) / epsilon)) - 1.0, 0.0));
  ଅଚଳ unsigned n0 = 1; // Hardwired
  ଅଚଳ ଦ୍ୱିଗୁଣ k1 = 0.2 / (b - a); // Hardwired.
  unsigned nmax = n0 + n1_2;
  ଦ୍ୱିଗୁଣ scaled_epsilon = epsilon * ଦ୍ୱିଗୁଣ (1llu << nmax);
  ଦ୍ୱିଗୁଣ _2_epsilon = 2.0 * epsilon;
  ଯେତେବେଳେ (b - a > _2_epsilon)
  {
    ଦ୍ୱିଗୁଣ x1_2 = 0.5 * (a + b);
    ଦ୍ୱିଗୁଣ r = scaled_epsilon - 0.5 * (b - a);
    // Guard against yb == ya to prevent division by zero
    ଦ୍ୱିଗୁଣ xf = (yb != ya) ? (yb * a - ya * b) / (yb - ya) : x1_2;
    ଦ୍ୱିଗୁଣ sigma = x1_2 - xf;
    ଦ୍ୱିଗୁଣ b_a = b - a;
    // This has k2 = 2 hardwired for efficiency.
    ଦ୍ୱିଗୁଣ b_a_k2 = b_a * b_a;
    ଦ୍ୱିଗୁଣ delta = k1 * b_a_k2;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sigma_sign = sigma >= 0 ? +1 : -1;
    ଦ୍ୱିଗୁଣ xt = delta <= fabs (x1_2 - xf) ? xf + delta * sigma_sign : x1_2;
    ଦ୍ୱିଗୁଣ xitp = fabs (xt - x1_2) <= r ? xt : x1_2 - r * sigma_sign;
    ଦ୍ୱିଗୁଣ yitp = f (xitp);
    ଯଦି (yitp > max_y)
    {
      b = xitp;
      yb = yitp;
    }
    ଅନ୍ୟଥା ଯଦି (yitp < min_y)
    {
      a = xitp;
      ya = yitp;
    }
    ଅନ୍ୟଥା
    {
      y = yitp;
      ଫେରନ୍ତୁ xitp;
    }
    scaled_epsilon *= 0.5;
  }
  ଫେରନ୍ତୁ 0.5 * (a + b);
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
ସଂରଚନା hb_scope_guard_t
{
  explicit hb_scope_guard_t (F &&f) : f (std::move (f)), active (ସତ୍ୟ) {}
  hb_scope_guard_t (hb_scope_guard_t &&o) noexcept
    : f (std::move (o.f)), active (o.active) { o.active = ମିଥ୍ୟା; }
  hb_scope_guard_t (ଅଚଳ hb_scope_guard_t &) = delete;
  hb_scope_guard_t &operator= (ଅଚଳ hb_scope_guard_t &) = delete;
  hb_scope_guard_t &operator= (hb_scope_guard_t &&) = delete;
  ~hb_scope_guard_t () { ଯଦି (active) f (); }

  /* Release: dismiss the guard so the cleanup does NOT run.  Use
   * when transferring ownership out of the scope. */
  ଶୂନ୍ୟ release () { active = ମିଥ୍ୟା; }

  ବ୍ୟକ୍ତିଗତ:
  F f;
  ତାର୍କିକ active;
};

template <typename F>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_scope_guard_t<F> hb_make_scope_guard (F &&f)
{ ଫେରନ୍ତୁ hb_scope_guard_t<F> (std::forward<F> (f)); }

#define HB_SCOPE_GUARD_NAME_(ଧାଡ଼ି) hb_scope_guard_##line
#define HB_SCOPE_GUARD_NAME(ଧାଡ଼ି) HB_SCOPE_GUARD_NAME_(ଧାଡ଼ି)
#define HB_SCOPE_GUARD(stmt) \
  auto HB_SCOPE_GUARD_NAME(__LINE__) = \
    hb_make_scope_guard ([&]() { stmt; }); \
  (ଶୂନ୍ୟ) HB_SCOPE_GUARD_NAME(__LINE__)


#endif /* HB_ALGS_HH */
