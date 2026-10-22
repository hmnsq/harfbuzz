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

#include "hb.hh"
#include "hb-meta.hh"
#include "hb-null.hh"
#include "hb-number.hh"

#include <algorithm>
#include <initializer_list>
#include <functional>
#include <new>

static HB_ALWAYS_INLINE void *
hb_malloc2 (size_t nmemb, size_t size)
{
  නම් (size && nmemb > SIZE_MAX / size) ආපසු nullptr;
  ආපසු hb_malloc (nmemb * size);
}
static HB_ALWAYS_INLINE void *
hb_realloc2 (void *ptr, size_t nmemb, size_t size)
{
  නම් (size && nmemb > SIZE_MAX / size) ආපසු nullptr;
  ආපසු hb_realloc (ptr, nmemb * size);
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
	extern "C++" { \
	  static anta_hstha constexpr T operator | (T l, T r) { ආපසු T ((unsigned) l | (unsigned) r); } \
	  static anta_hstha constexpr T operator & (T l, T r) { ආපසු T ((unsigned) l & (unsigned) r); } \
	  static anta_hstha constexpr T operator ^ (T l, T r) { ආපසු T ((unsigned) l ^ (unsigned) r); } \
	  static anta_hstha constexpr unsigned operator ~ (T r) { ආපසු (~(unsigned) r); } \
	  static anta_hstha T& operator |= (T &l, T r) { l = l | r; ආපසු l; } \
	  static anta_hstha T& operator &= (T& l, T r) { l = l & r; ආපසු l; } \
	  static anta_hstha T& operator ^= (T& l, T r) { l = l ^ r; ආපසු l; } \
	} \
	static_assert (true, "")

/* Useful for set-operations on small enums.
 * For example, for testing "x ∈ {x1, x2, x3}" use:
 * (FLAG_UNSAFE(x) & (FLAG(x1) | FLAG(x2) | FLAG(x3)))
 */
#define FLAG(x) (static_assert_expr ((unsigned)(x) < 32) + (((uint32_t) 1U) << (unsigned)(x)))
#define FLAG_UNSAFE(x) ((unsigned)(x) < 32 ? (((uint32_t) 1U) << (unsigned)(x)) : 0)
#define FLAG_RANGE(x,y) (static_assert_expr ((x) < (y)) + FLAG(y+1) - FLAG(x))
#define FLAG64(x) (static_assert_expr ((unsigned)(x) < 64) + (((uint64_t) 1ULL) << (unsigned)(x)))
#define FLAG64_UNSAFE(x) ((unsigned)(x) < 64 ? (((uint64_t) 1ULL) << (unsigned)(x)) : 0)


/*
 * Fixed-endian integers / floats.
 */


/* Endian swap, used in Windows related backends */
static anta_hstha constexpr uint16_t hb_uint16_swap (uint16_t v)
{ ආපසු (v >> 8) | (v << 8); }
static anta_hstha constexpr uint32_t hb_uint32_swap (uint32_t v)
{ ආපසු (hb_uint16_swap (v) << 16) | hb_uint16_swap (v >> 16); }

template <typename Type>
sa_nracanaa __attribute__((packed)) hb_packed_t
{
  hb_packed_t () = default;
  constexpr hb_packed_t (Type V) : v (V) {}
  operator Type () const { ආපසු v; }
  hb_packed_t & operator = (Type V) { v = V; ආපසු *this; }

  private:
  Type v;
};

#ifndef HB_FAST_NUM_ACCESS

#නම් defined(__OPTIMIZE__) && \
    defined(__BYTE_ORDER) && \
    (__BYTE_ORDER == __BIG_ENDIAN || \
     (__BYTE_ORDER == __LITTLE_ENDIAN && \
      hb_has_builtin(__builtin_bswap16) && \
      hb_has_builtin(__builtin_bswap32) && \
      hb_has_builtin(__builtin_bswap64)))
#define HB_FAST_NUM_ACCESS 1
#නැතහොත්
#define HB_FAST_NUM_ACCESS 0
#endif

// https://github.com/harfbuzz/harfbuzz/issues/5456
#නම් defined(__GNUC__) && !defined(__clang__) && (__GNUC__ <= 12)
#undef HB_FAST_NUM_ACCESS
#define HB_FAST_NUM_ACCESS 0
#endif

#endif

template <bool BE, typename Type, පූර්ණ_සංඛ්‍යා Bytes = parimaa_na (Type)>
sa_nracanaa HBInt;
template <bool BE, typename Type>
sa_nracanaa HBInt<BE, Type, 1>
{
  public:
  HBInt () = default;
  constexpr HBInt (Type V) : v {uint8_t (V)} {}
  constexpr operator Type () const { ආපසු v; }
  private: uint8_t v;
};
template <bool BE, typename Type>
sa_nracanaa HBInt<BE, Type, 2>
{
  public:
  HBInt () = default;

  HBInt (Type V)
#නම් HB_FAST_NUM_ACCESS
  {
    නම් (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint16_t> *) v) = V;
    නැතහොත්
      *((hb_packed_t<uint16_t> *) v) = __builtin_bswap16 (V);
  }
#නැතහොත්
    : v {BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >>  8) & 0xFF)} {}
#endif

  constexpr operator Type () const
  {
#නම් HB_FAST_NUM_ACCESS
    ආපසු (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint16_t) *((const hb_packed_t<uint16_t> *) v)
    :
      __builtin_bswap16 ((uint16_t) *((const hb_packed_t<uint16_t> *) v))
    ;
#නැතහොත්
    ආපසු (BE ? (v[0] <<  8) : (v[0]      ))
	 + (BE ? (v[1]      ) : (v[1] <<  8));
#endif
  }
  private: uint8_t v[2];
};
template <bool BE, typename Type>
sa_nracanaa HBInt<BE, Type, 3>
{
  static_assert (!std::is_signed<Type>::value, "");
  public:
  HBInt () = default;
  constexpr HBInt (Type V) : v {BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
				BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
				BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V      ) & 0xFF)} {}

  constexpr operator Type () const { ආපසු (BE ? (v[0] << 16) : (v[0]      ))
					  + (BE ? (v[1] <<  8) : (v[1] <<  8))
					  + (BE ? (v[2]      ) : (v[2] << 16)); }
  private: uint8_t v[3];
};
template <bool BE, typename Type>
sa_nracanaa HBInt<BE, Type, 4>
{
  template <bool, typename, පූර්ණ_සංඛ්‍යා>
  friend sa_nracanaa HBFloat;

  public:
  HBInt () = default;

  HBInt (Type V)
#නම් HB_FAST_NUM_ACCESS
  {
    නම් (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint32_t> *) v) = V;
    නැතහොත්
      *((hb_packed_t<uint32_t> *) v) = __builtin_bswap32 (V);
  }
#නැතහොත්
    : v {BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 24) & 0xFF)} {}
#endif

  constexpr operator Type () const {
#නම් HB_FAST_NUM_ACCESS
    ආපසු (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint32_t) *((const hb_packed_t<uint32_t> *) v)
    :
      __builtin_bswap32 ((uint32_t) *((const hb_packed_t<uint32_t> *) v))
    ;
#නැතහොත්
    ආපසු (BE ? (uint32_t (v[0]) << 24) : (uint32_t (v[0])      ))
	 + (BE ? (uint32_t (v[1]) << 16) : (uint32_t (v[1]) <<  8))
	 + (BE ? (uint32_t (v[2]) <<  8) : (uint32_t (v[2]) << 16))
	 + (BE ? (uint32_t (v[3])      ) : (uint32_t (v[3]) << 24));
#endif
  }
  private: uint8_t v[4];
};
template <bool BE, typename Type>
sa_nracanaa HBInt<BE, Type, 8>
{
  template <bool, typename, පූර්ණ_සංඛ්‍යා>
  friend sa_nracanaa HBFloat;

  public:
  HBInt () = default;

  HBInt (Type V)
#නම් HB_FAST_NUM_ACCESS
  {
    නම් (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint64_t> *) v) = V;
    නැතහොත්
      *((hb_packed_t<uint64_t> *) v) = __builtin_bswap64 (V);
  }
#නැතහොත්
    : v {BE ? uint8_t ((V >> 56) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 48) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >> 40) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V >> 32) & 0xFF) : uint8_t ((V >> 24) & 0xFF),
	 BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V >> 32) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 40) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 48) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 56) & 0xFF)} {}
#endif

  constexpr operator Type () const {
#නම් HB_FAST_NUM_ACCESS
    ආපසු (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint64_t) *((const hb_packed_t<uint64_t> *) v)
    :
      __builtin_bswap64 ((uint64_t) *((const hb_packed_t<uint64_t> *) v))
    ;
#නැතහොත්
    ආපසු (BE ? (uint64_t (v[0]) << 56) : (uint64_t (v[0])      ))
	 + (BE ? (uint64_t (v[1]) << 48) : (uint64_t (v[1]) <<  8))
	 + (BE ? (uint64_t (v[2]) << 40) : (uint64_t (v[2]) << 16))
	 + (BE ? (uint64_t (v[3]) << 32) : (uint64_t (v[3]) << 24))
	 + (BE ? (uint64_t (v[4]) << 24) : (uint64_t (v[4]) << 32))
	 + (BE ? (uint64_t (v[5]) << 16) : (uint64_t (v[5]) << 40))
	 + (BE ? (uint64_t (v[6]) <<  8) : (uint64_t (v[6]) << 48))
	 + (BE ? (uint64_t (v[7])      ) : (uint64_t (v[7]) << 56));
#endif
  }
  private: uint8_t v[8];
};

/* Floats. */

template <bool BE, typename Type, පූර්ණ_සංඛ්‍යා Bytes>
sa_nracanaa HBFloat
{
  using IntType = typename std::conditional<Bytes == 4, uint32_t, uint64_t>::type;

  public:
  HBFloat () = default;

  HBFloat (Type V)
  {
#නම් HB_FAST_NUM_ACCESS
    {
	      නම් (BE == (__BYTE_ORDER == __BIG_ENDIAN))
	      {
	        *((hb_packed_t<Type> *) v) = V;
	        ආපසු;
	      }
	    }
#endif

    sa_ngha {
      hb_packed_t<Type> f;
      hb_packed_t<IntType> i;
    } u = {{V}};

    const HBInt<BE, IntType> I = (IntType) u.i;
    සඳහා (unsigned i = 0; i < Bytes; i++)
      v[i] = I.v[i];
  }

  /* c++14 constexpr */ operator Type () const
  {
#නම් HB_FAST_NUM_ACCESS
	    {
	      නම් (BE == (__BYTE_ORDER == __BIG_ENDIAN))
		ආපසු (Type) *((const hb_packed_t<Type> *) v);
	    }
#endif

    HBInt<BE, IntType> I;
    සඳහා (unsigned i = 0; i < Bytes; i++)
      I.v[i] = v[i];

    sa_ngha {
      hb_packed_t<IntType> i;
      hb_packed_t<Type> f;
    } u = {{I}};

    ආපසු (Type) u.f;
  }
  private: uint8_t v[Bytes];
};


/* We want our rounding towards +infinity. */
static anta_hstha dviguu_na
_hb_roundf (dviguu_na x) { ආපසු floor (x + .5); }

static anta_hstha float
_hb_roundf (float x) { ආපසු floor ((dviguu_na) x + .5); }

#define roundf(x) _hb_roundf(x)

static anta_hstha void
hb_sincos (float rotation, float &s, float &c)
{
#ifdef HAVE_SINCOSF
  sincosf (rotation, &s, &c);
#නැතහොත්
  c = cosf (rotation);
  s = sinf (rotation);
#endif
}
static anta_hstha void
hb_sincos (dviguu_na rotation, dviguu_na &s, dviguu_na &c)
{
#ifdef HAVE_SINCOS
  sincos (rotation, &s, &c);
#නැතහොත්
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


sa_nracanaa
{
  /* Note.  This is dangerous in that if it's passed an rvalue, it returns rvalue-reference. */
  template <typename T> constexpr auto
  operator () (T&& v) const HB_AUTO_RETURN ( std::forward<T> (v) )
}
HB_FUNCOBJ (hb_identity);
sa_nracanaa
{
  /* Like identity(), but only retains lvalue-references.  Rvalues are returned as rvalues. */
  template <typename T> constexpr T&
  operator () (T& v) const { ආපසු v; }

  template <typename T> constexpr hb_remove_reference<T>
  operator () (T&& v) const { ආපසු v; }
}
HB_FUNCOBJ (hb_lidentity);
sa_nracanaa
{
  /* Like identity(), but always returns rvalue. */
  template <typename T> constexpr hb_remove_reference<T>
  operator () (T&& v) const { ආපසු v; }
}
HB_FUNCOBJ (hb_ridentity);

sa_nracanaa
{
  template <typename T> constexpr bool
  operator () (T&& v) const { ආපසු bool (std::forward<T> (v)); }
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
			(void) ((h) ^= (h) >> 23),		\
			(void) ((h) *= 0x2127599bf4325c37ULL),	\
			(h) ^= (h) >> 47)

static anta_hstha uint64_t fasthash64(const void *buf, size_t len, uint64_t seed)
{
	sa_nracanaa __attribute__((packed)) packed_uint64_t { uint64_t v; };
	const uint64_t    m = 0x880355f21e6d1965ULL;
	const packed_uint64_t *pos = (const packed_uint64_t *)buf;
	const packed_uint64_t *end = pos + (len / 8);
	const unsigned var_na *pos2;
	uint64_t h = seed ^ (len * m);
	uint64_t v;

#ifndef HB_OPTIMIZE_SIZE
	නම් (((uintptr_t) pos & 7) == 0)
	{
	  අතරතුර (pos != end)
	  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
	    v  = * (const uint64_t *) (pos++);
#pragma GCC diagnostic pop
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}
	නැතහොත්
#endif
	{
	  අතරතුර (pos != end)
	  {
	    v  = pos++->v;
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}

	pos2 = (const unsigned var_na*)pos;
	v = 0;

	switch (len & 7) {
	pak_sa 7: v ^= (uint64_t)pos2[6] << 48; HB_FALLTHROUGH;
	pak_sa 6: v ^= (uint64_t)pos2[5] << 40; HB_FALLTHROUGH;
	pak_sa 5: v ^= (uint64_t)pos2[4] << 32; HB_FALLTHROUGH;
	pak_sa 4: v ^= (uint64_t)pos2[3] << 24; HB_FALLTHROUGH;
	pak_sa 3: v ^= (uint64_t)pos2[2] << 16; HB_FALLTHROUGH;
	pak_sa 2: v ^= (uint64_t)pos2[1] <<  8; HB_FALLTHROUGH;
	pak_sa 1: v ^= (uint64_t)pos2[0];
		h ^= fasthash_mix(v);
		h *= m;
	}

	ආපසු fasthash_mix(h);
}

static anta_hstha uint32_t fasthash32(const void *buf, size_t len, uint32_t seed)
{
	// the following trick converts the 64-bit hashcode to Fermat
	// residue, which shall retain information from both the higher
	// and lower parts of hashcode.
        uint64_t h = fasthash64(buf, len, seed);
	ආපසු h - (h >> 32);
}

sa_nracanaa
{
  private:

  template <typename T> constexpr auto
  impl (const T& v, hb_priority<2>) const HB_RETURN (uint32_t, hb_deref (v).hash ())

  // Horrible: std:hash() of integers seems to be identity in gcc / clang?!
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  //
  // For performance characteristics see:
  // https://github.com/harfbuzz/harfbuzz/pull/4228#issuecomment-1565079537
  template <typename T,
	    hb_enable_if (std::is_integral<T>::value && parimaa_na (T) <= parimaa_na (uint32_t))> constexpr auto
  impl (const T& v, hb_priority<1>) const HB_RETURN (uint32_t, (uint32_t) v * 2654435761u /* Knuh's multiplicative hash */)
  template <typename T,
	    hb_enable_if (std::is_integral<T>::value && parimaa_na (T) > parimaa_na (uint32_t))> constexpr auto
  impl (const T& v, hb_priority<1>) const HB_RETURN (uint32_t, (uint32_t) (v ^ (v >> 32)) * 2654435761u /* Knuth's multiplicative hash */)

  template <typename T,
	    hb_enable_if (std::is_floating_point<T>::value)> constexpr auto
  impl (const T& v, hb_priority<1>) const HB_RETURN (uint32_t, fasthash32 (std::addressof (v), parimaa_na (T), 0xf437ffe6))

  template <typename T> constexpr auto
  impl (const T& v, hb_priority<0>) const HB_RETURN (uint32_t, std::hash<hb_decay<decltype (hb_deref (v))>>{} (hb_deref (v)))

  public:

  template <typename T> constexpr auto
  operator () (const T& v) const HB_RETURN (uint32_t, impl (v, hb_prioritize))
}
HB_FUNCOBJ (hb_hash);


sa_nracanaa
{
  private:

  /* Pointer-to-member-function. */
  template <typename Appl, typename T, typename ...Ts> auto
  impl (Appl&& a, hb_priority<2>, T &&v, Ts&&... ds) const HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v)).*std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  /* Pointer-to-member. */
  template <typename Appl, typename T> auto
  impl (Appl&& a, hb_priority<1>, T &&v) const HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v))).*std::forward<Appl> (a))

  /* Operator(). */
  template <typename Appl, typename ...Ts> auto
  impl (Appl&& a, hb_priority<0>, Ts&&... ds) const HB_AUTO_RETURN
  (hb_deref (std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  public:

  template <typename Appl, typename ...Ts> auto
  operator () (Appl&& a, Ts&&... ds) const HB_AUTO_RETURN
  (
    impl (std::forward<Appl> (a),
	  hb_prioritize,
	  std::forward<Ts> (ds)...)
  )
}
HB_FUNCOBJ (hb_invoke);

template <unsigned Pos, typename Appl, typename V>
sa_nracanaa hb_partial_t
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
    ආපසු hb_invoke (std::forward<Appl> (a),
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
    ආපසු hb_invoke (std::forward<Appl> (a),
		      std::forward<T0> (d0),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }

  private:
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
  decltype(auto) operator () (_T&& _v) const \
  { ආපසු hb_partial<Pos> (this, std::forward<_T> (_v)); } \
  static_assert (true, "")
#නැතහොත්
/* https://github.com/harfbuzz/harfbuzz/issues/1724 */
#define HB_PARTIALIZE(Pos) \
  template <typename _T> \
  auto operator () (_T&& _v) const HB_AUTO_RETURN \
  (hb_partial<Pos> (+this, std::forward<_T> (_v))) \
  static_assert (true, "")
#endif


sa_nracanaa
{
  private:

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<1>) const HB_AUTO_RETURN
  (
    hb_deref (std::forward<Pred> (p)).has (std::forward<Val> (v))
  )

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<0>) const HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Pred> (p),
	       std::forward<Val> (v))
  )

  public:

  template <typename Pred, typename Val> auto
  operator () (Pred&& p, Val &&v) const HB_RETURN (bool,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_has);

sa_nracanaa
{
  private:

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<1>) const HB_AUTO_RETURN
  (
    hb_has (std::forward<Pred> (p),
	    std::forward<Val> (v))
  )

  template <typename Pred, typename Val> auto
  impl (Pred&& p, Val &&v, hb_priority<0>) const HB_AUTO_RETURN
  (
    std::forward<Pred> (p) == std::forward<Val> (v)
  )

  public:

  template <typename Pred, typename Val> auto
  operator () (Pred&& p, Val &&v) const HB_RETURN (bool,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_match);

sa_nracanaa
{
  private:

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<2>) const HB_AUTO_RETURN
  (
    hb_deref (std::forward<Proj> (f)).get (std::forward<Val> (v))
  )

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<1>) const HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Proj> (f),
	       std::forward<Val> (v))
  )

  template <typename Proj, typename Val> auto
  impl (Proj&& f, Val &&v, hb_priority<0>) const HB_AUTO_RETURN
  (
    std::forward<Proj> (f)[std::forward<Val> (v)]
  )

  public:

  template <typename Proj, typename Val> auto
  operator () (Proj&& f, Val &&v) const HB_AUTO_RETURN
  (
    impl (std::forward<Proj> (f),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_get);

sa_nracanaa
{
  private:

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<3>) const HB_AUTO_RETURN
  (
    std::forward<T2> (v2).cmp (std::forward<T1> (v1)) == 0
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<2>) const HB_AUTO_RETURN
  (
    std::forward<T1> (v1).cmp (std::forward<T2> (v2)) == 0
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<1>) const HB_AUTO_RETURN
  (
    std::forward<T1> (v1) == std::forward<T2> (v2)
  )

  template <typename T1, typename T2> auto
  impl (T1&& v1, T2 &&v2, hb_priority<0>) const HB_AUTO_RETURN
  (
    std::forward<T2> (v2) == std::forward<T1> (v1)
  )

  public:

  template <typename T1, typename T2> auto
  operator () (T1&& v1, T2 &&v2) const HB_AUTO_RETURN
  (
    impl (std::forward<T1> (v1),
	  std::forward<T2> (v2),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_equal);

sa_nracanaa
{
  template <typename T> void
  operator () (T& a, T& b) const
  {
    using std::swap; // allow ADL
    swap (a, b);
  }
}
HB_FUNCOBJ (hb_swap);


template <typename T1, typename T2>
sa_nracanaa hb_pair_t
{
  typedef T1 first_t;
  typedef T2 second_t;
  typedef hb_pair_t<T1, T2> pair_t;

  template <typename U1 = T1, typename U2 = T2,
	    hb_enable_if (std::is_default_constructible<U1>::value &&
			  std::is_default_constructible<U2>::value)>
  hb_pair_t () : first (), second () {}
  hb_pair_t (T1 a, T2 b) : first (std::forward<T1> (a)), second (std::forward<T2> (b)) {}

  template <typename Q1, typename Q2,
	    hb_enable_if (hb_is_convertible (T1, Q1) &&
			  hb_is_convertible (T2, Q2))>
  operator hb_pair_t<Q1, Q2> () { ආපසු hb_pair_t<Q1, Q2> (first, second); }

  hb_pair_t<T1, T2> reverse () const
  { ආපසු hb_pair_t<T1, T2> (second, first); }

  bool operator == (const pair_t& o) const { ආපසු first == o.first && second == o.second; }
  bool operator != (const pair_t& o) const { ආපසු !(*this == o); }
  bool operator < (const pair_t& o) const { ආපසු first < o.first || (first == o.first && second < o.second); }
  bool operator >= (const pair_t& o) const { ආපසු !(*this < o); }
  bool operator > (const pair_t& o) const { ආපසු first > o.first || (first == o.first && second > o.second); }
  bool operator <= (const pair_t& o) const { ආපසු !(*this > o); }

  static පූර්ණ_සංඛ්‍යා cmp (const void *pa, const void *pb)
  {
    pair_t *a = (pair_t *) pa;
    pair_t *b = (pair_t *) pb;

    නම් (a->first < b->first) ආපසු -1;
    නම් (a->first > b->first) ආපසු +1;
    නම් (a->second < b->second) ආපසු -1;
    නම් (a->second > b->second) ආපසු +1;
    ආපසු 0;
  }

  friend void swap (hb_pair_t& a, hb_pair_t& b) noexcept
  {
    hb_swap (a.first, b.first);
    hb_swap (a.second, b.second);
  }


  T1 first;
  T2 second;
};
template <typename T1, typename T2> static anta_hstha hb_pair_t<T1, T2>
hb_pair (T1&& a, T2&& b) { ආපසු hb_pair_t<T1, T2> (a, b); }

typedef hb_pair_t<hb_codepoint_t, hb_codepoint_t> hb_codepoint_pair_t;

sa_nracanaa
{
  template <typename Pair> constexpr typename Pair::first_t
  operator () (const Pair& pair) const { ආපසු pair.first; }
}
HB_FUNCOBJ (hb_first);

sa_nracanaa
{
  template <typename Pair> constexpr typename Pair::second_t
  operator () (const Pair& pair) const { ආපසු pair.second; }
}
HB_FUNCOBJ (hb_second);

/* Note.  In min/max impl, we can use hb_type_identity<T> for second argument.
 * However, that would silently convert between different-signedness integers.
 * Instead we accept two different types, such that compiler can err if
 * comparing integers of different signedness. */
sa_nracanaa
{
  template <typename T, typename T2> constexpr auto
  operator () (T&& a, T2&& b) const HB_AUTO_RETURN
  (a <= b ? a : b)
}
HB_FUNCOBJ (hb_min);
sa_nracanaa
{
  template <typename T, typename T2> constexpr auto
  operator () (T&& a, T2&& b) const HB_AUTO_RETURN
  (a >= b ? a : b)
}
HB_FUNCOBJ (hb_max);
sa_nracanaa
{
  template <typename T, typename T2, typename T3> constexpr auto
  operator () (T&& x, T2&& min, T3&& max) const HB_AUTO_RETURN
  (hb_min (hb_max (std::forward<T> (x), std::forward<T2> (min)), std::forward<T3> (max)))
}
HB_FUNCOBJ (hb_clamp);

/* Signed saturating arithmetic. */
template <typename T>
static HB_ALWAYS_INLINE T
hb_saturate_add (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#නම් hb_has_builtin(__builtin_add_overflow)
  T result;
  නම් (likely (!__builtin_add_overflow (a, b, &result)))
    ආපසු result;
#නැතහොත්
  නම් (b > 0)
  {
    නම් (likely (a <= hb_int_max (T) - b))
      ආපසු a + b;
  }
  නැතහොත් නම් (likely (a >= hb_int_min (T) - b))
    ආපසු a + b;
#endif

  ආපසු a < 0 ? hb_int_min (T) : hb_int_max (T);
}

template <typename T>
static HB_ALWAYS_INLINE T
hb_saturate_sub (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#නම් hb_has_builtin(__builtin_sub_overflow)
  T result;
  නම් (likely (!__builtin_sub_overflow (a, b, &result)))
    ආපසු result;
#නැතහොත්
  නම් (b > 0)
  {
    නම් (likely (a >= hb_int_min (T) + b))
      ආපසු a - b;
  }
  නැතහොත් නම් (likely (a <= hb_int_max (T) + b))
    ආපසු a - b;
#endif

  ආපසු b < 0 ? hb_int_max (T) : hb_int_min (T);
}

template <typename T>
static HB_ALWAYS_INLINE T
hb_saturate_neg (T a)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  ආපසු hb_saturate_sub ((T) 0, a);
}

/* Convert a floating-point value to integer type T, saturating to T's
 * range instead of relying on the undefined behavior of an out-of-range
 * float-to-int conversion.  NaN saturates to the minimum of T. */
template <typename T>
static HB_ALWAYS_INLINE T
hb_clamp_to (dviguu_na v)
{
  ආපසු (T) hb_clamp (v,
		       (dviguu_na) hb_int_min (T),
		       (dviguu_na) hb_int_max (T));
}

template <typename T>
static HB_ALWAYS_INLINE T
hb_clamp_to (float v)
{
  /* Widen explicitly: float cannot represent all integer bounds exactly,
   * so the clamp itself is done in double. */
  ආපසු hb_clamp_to<T> ((dviguu_na) v);
}

template <typename T>
static HB_ALWAYS_INLINE T
hb_clamp_to (int64_t v)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  ආපසු (T) hb_clamp (v,
		       (int64_t) hb_int_min (T),
		       (int64_t) hb_int_max (T));
}

/*
 * Bithacks.
 */

/* Return the number of 1 bits in a uint8_t; faster than hb_popcount() */
static anta_hstha unsigned
hb_popcount8 (uint8_t v)
{
  static const uint8_t popcount4[16] = {
    0, 1, 1, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 2, 3, 3, 4
  };
  ආපසු popcount4[v & 0xF] + popcount4[v >> 4];
}

/* Return the number of 1 bits in v. */
template <typename T>
static anta_hstha unsigned පූර්ණ_සංඛ්‍යා
hb_popcount (T v)
{
#නම් hb_has_builtin(__builtin_popcount)
  නම් (parimaa_na (T) <= parimaa_na (unsigned පූර්ණ_සංඛ්‍යා))
    ආපසු __builtin_popcount (v);
#endif

#නම් hb_has_builtin(__builtin_popcountl)
  නම් (parimaa_na (T) <= parimaa_na (unsigned long))
    ආපසු __builtin_popcountl (v);
#endif

#නම් hb_has_builtin(__builtin_popcountll)
  නම් (parimaa_na (T) <= parimaa_na (unsigned long long))
    ආපසු __builtin_popcountll (v);
#endif

  නම් (parimaa_na (T) <= 4)
  {
    /* "HACKMEM 169" */
    uint32_t y;
    y = (v >> 1) &033333333333;
    y = v - y - ((y >>1) & 033333333333);
    ආපසු (((y + (y >> 3)) & 030707070707) % 077);
  }

  නම් (parimaa_na (T) == 8)
  {
    uint64_t y = (uint64_t) v;
    y -= ((y >> 1) & 0x5555555555555555ull);
    y = (y & 0x3333333333333333ull) + (y >> 2 & 0x3333333333333333ull);
    ආපසු ((y + (y >> 4)) & 0xf0f0f0f0f0f0f0full) * 0x101010101010101ull >> 56;
  }

  නම් (parimaa_na (T) == 16)
  {
    unsigned පූර්ණ_සංඛ්‍යා shift = 64;
    ආපසු hb_popcount<uint64_t> ((uint64_t) v) + hb_popcount ((uint64_t) (v >> shift));
  }

  assert (0);
  ආපසු 0; /* Shut up stupid compiler. */
}

/* Returns the number of bits needed to store number */
template <typename T>
static anta_hstha unsigned පූර්ණ_සංඛ්‍යා
hb_bit_storage (T v)
{
  නම් (unlikely (!v)) ආපසු 0;

#නම් hb_has_builtin(__builtin_clz)
  නම් (parimaa_na (T) <= parimaa_na (unsigned පූර්ණ_සංඛ්‍යා))
    ආපසු parimaa_na (unsigned පූර්ණ_සංඛ්‍යා) * 8 - __builtin_clz (v);
#endif

#නම් hb_has_builtin(__builtin_clzl)
  නම් (parimaa_na (T) <= parimaa_na (unsigned long))
    ආපසු parimaa_na (unsigned long) * 8 - __builtin_clzl (v);
#endif

#නම් hb_has_builtin(__builtin_clzll)
  නම් (parimaa_na (T) <= parimaa_na (unsigned long long))
    ආපසු parimaa_na (unsigned long long) * 8 - __builtin_clzll (v);
#endif

#නම් (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  නම් (parimaa_na (T) <= parimaa_na (unsigned පූර්ණ_සංඛ්‍යා))
  {
    unsigned long where;
    _BitScanReverse (&where, v);
    ආපසු 1 + where;
  }
# නම් defined(_WIN64)
  නම් (parimaa_na (T) <= 8)
  {
    unsigned long where;
    _BitScanReverse64 (&where, v);
    ආපසු 1 + where;
  }
# endif
#endif

  නම් (parimaa_na (T) <= 4)
  {
    /* "bithacks" */
    const unsigned පූර්ණ_සංඛ්‍යා b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    const unsigned පූර්ණ_සංඛ්‍යා S[] = {1, 2, 4, 8, 16};
    unsigned පූර්ණ_සංඛ්‍යා r = 0;
    සඳහා (පූර්ණ_සංඛ්‍යා i = 4; i >= 0; i--)
      නම් (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    ආපසු r + 1;
  }
  නම් (parimaa_na (T) <= 8)
  {
    /* "bithacks" */
    const uint64_t b[] = {0x2ULL, 0xCULL, 0xF0ULL, 0xFF00ULL, 0xFFFF0000ULL, 0xFFFFFFFF00000000ULL};
    const unsigned පූර්ණ_සංඛ්‍යා S[] = {1, 2, 4, 8, 16, 32};
    unsigned පූර්ණ_සංඛ්‍යා r = 0;
    සඳහා (පූර්ණ_සංඛ්‍යා i = 5; i >= 0; i--)
      නම් (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    ආපසු r + 1;
  }
  නම් (parimaa_na (T) == 16)
  {
    unsigned පූර්ණ_සංඛ්‍යා shift = 64;
    ආපසු (v >> shift) ? hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift :
			  hb_bit_storage<uint64_t> ((uint64_t) v);
  }

  assert (0);
  ආපසු 0; /* Shut up stupid compiler. */
}

/* Returns the number of zero bits in the least significant side of v */
template <typename T>
static anta_hstha unsigned පූර්ණ_සංඛ්‍යා
hb_ctz (T v)
{
  නම් (unlikely (!v)) ආපසු 8 * parimaa_na (T);

#නම් hb_has_builtin(__builtin_ctz)
  නම් (parimaa_na (T) <= parimaa_na (unsigned පූර්ණ_සංඛ්‍යා))
    ආපසු __builtin_ctz (v);
#endif

#නම් hb_has_builtin(__builtin_ctzl)
  නම් (parimaa_na (T) <= parimaa_na (unsigned long))
    ආපසු __builtin_ctzl (v);
#endif

#නම් hb_has_builtin(__builtin_ctzll)
  නම් (parimaa_na (T) <= parimaa_na (unsigned long long))
    ආපසු __builtin_ctzll (v);
#endif

#නම් (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  නම් (parimaa_na (T) <= parimaa_na (unsigned පූර්ණ_සංඛ්‍යා))
  {
    unsigned long where;
    _BitScanForward (&where, v);
    ආපසු where;
  }
# නම් defined(_WIN64)
  නම් (parimaa_na (T) <= 8)
  {
    unsigned long where;
    _BitScanForward64 (&where, v);
    ආපසු where;
  }
# endif
#endif

  නම් (parimaa_na (T) <= 4)
  {
    /* "bithacks" */
    unsigned පූර්ණ_සංඛ්‍යා c = 32;
    v &= - (int32_t) v;
    නම් (v) c--;
    නම් (v & 0x0000FFFF) c -= 16;
    නම් (v & 0x00FF00FF) c -= 8;
    නම් (v & 0x0F0F0F0F) c -= 4;
    නම් (v & 0x33333333) c -= 2;
    නම් (v & 0x55555555) c -= 1;
    ආපසු c;
  }
  නම් (parimaa_na (T) <= 8)
  {
    /* "bithacks" */
    unsigned පූර්ණ_සංඛ්‍යා c = 64;
    v &= - (int64_t) (v);
    නම් (v) c--;
    නම් (v & 0x00000000FFFFFFFFULL) c -= 32;
    නම් (v & 0x0000FFFF0000FFFFULL) c -= 16;
    නම් (v & 0x00FF00FF00FF00FFULL) c -= 8;
    නම් (v & 0x0F0F0F0F0F0F0F0FULL) c -= 4;
    නම් (v & 0x3333333333333333ULL) c -= 2;
    නම් (v & 0x5555555555555555ULL) c -= 1;
    ආපසු c;
  }
  නම් (parimaa_na (T) == 16)
  {
    unsigned පූර්ණ_සංඛ්‍යා shift = 64;
    ආපසු (uint64_t) v ? hb_bit_storage<uint64_t> ((uint64_t) v) :
			  hb_bit_storage<uint64_t> ((uint64_t) (v >> shift)) + shift;
  }

  assert (0);
  ආපසු 0; /* Shut up stupid compiler. */
}


/*
 * Tiny stuff.
 */

/* ASCII tag/character handling */
static anta_hstha bool ISALPHA (unsigned var_na c)
{ ආපසු (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
static anta_hstha bool ISALNUM (unsigned var_na c)
{ ආපසු (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }
static anta_hstha bool ISSPACE (unsigned var_na c)
{ ආපසු c == ' ' || c =='\f'|| c =='\n'|| c =='\r'|| c =='\t'|| c =='\v'; }
static anta_hstha unsigned var_na TOUPPER (unsigned var_na c)
{ ආපසු (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c; }
static anta_hstha unsigned var_na TOLOWER (unsigned var_na c)
{ ආපසු (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }
static anta_hstha bool ISHEX (unsigned var_na c)
{ ආපසු (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
static anta_hstha unsigned var_na TOHEX (uint8_t c)
{ ආපසු (c & 0xF) <= 9 ? (c & 0xF) + '0' : (c & 0xF) + 'a' - 10; }
static anta_hstha uint8_t FROMHEX (unsigned var_na c)
{ ආපසු (c >= '0' && c <= '9') ? c - '0' : TOLOWER (c) - 'a' + 10; }

static anta_hstha unsigned පූර්ණ_සංඛ්‍යා DIV_CEIL (const unsigned පූර්ණ_සංඛ්‍යා a, unsigned පූර්ණ_සංඛ්‍යා b)
{ ආපසු (a + (b - 1)) / b; }


#undef  ARRAY_LENGTH
template <typename Type, unsigned පූර්ණ_සංඛ්‍යා n>
static anta_hstha unsigned පූර්ණ_සංඛ්‍යා ARRAY_LENGTH (const Type (&)[n]) { ආපසු n; }
/* A const version, but does not detect erratically being called on pointers. */
#define ARRAY_LENGTH_CONST(__array) ((signed පූර්ණ_සංඛ්‍යා) (parimaa_na (__array) / parimaa_na (__array[0])))


static anta_hstha void *
hb_memcpy (void *__restrict dst, const void *__restrict src, size_t len)
{
  /* It's illegal to pass 0 as size to memcpy. */
  නම් (unlikely (!len)) ආපසු dst;
  ආපසු memcpy (dst, src, len);
}

static anta_hstha පූර්ණ_සංඛ්‍යා
hb_memcmp (const void *a, const void *b, unsigned පූර්ණ_සංඛ්‍යා len)
{
  /* It's illegal to pass NULL to memcmp(), even if len is zero.
   * So, wrap it.
   * https://sourceware.org/bugzilla/show_bug.cgi?id=23878 */
  නම් (unlikely (!len)) ආපසු 0;
  ආපසු memcmp (a, b, len);
}

static anta_hstha void *
hb_memset (void *s, පූර්ණ_සංඛ්‍යා c, unsigned පූර්ණ_සංඛ්‍යා n)
{
  /* It's illegal to pass NULL to memset(), even if n is zero. */
  නම් (unlikely (!n)) ආපසු s;
  ආපසු memset (s, c, n);
}

static anta_hstha unsigned පූර්ණ_සංඛ්‍යා
hb_ceil_to_4 (unsigned පූර්ණ_සංඛ්‍යා v)
{
  ආපසු ((v - 1) | 3) + 1;
}

template <typename T> static anta_hstha bool
hb_in_range (T u, T lo, T hi)
{
  static_assert (!std::is_signed<T>::value, "");

  /* The casts below are important as if T is smaller than int,
   * the subtract results will become a signed int! */
  ආපසු (T)(u - lo) <= (T)(hi - lo);
}
template <typename T> static anta_hstha bool
hb_in_ranges (T u, T lo1, T hi1)
{
  ආපසු hb_in_range (u, lo1, hi1);
}
template <typename T, typename ...Ts> static anta_hstha bool
hb_in_ranges (T u, T lo1, T hi1, Ts... ds)
{
  ආපසු hb_in_range<T> (u, lo1, hi1) || hb_in_ranges<T> (u, ds...);
}


/*
 * Overflow checking.
 */

static anta_hstha bool
hb_unsigned_mul_overflows (unsigned පූර්ණ_සංඛ්‍යා count, unsigned පූර්ණ_සංඛ්‍යා size, unsigned *result = nullptr)
{
#නම් hb_has_builtin(__builtin_mul_overflow)
  unsigned stack_result;
  නම් (!result)
    result = &stack_result;
  ආපසු __builtin_mul_overflow (count, size, result);
#endif

  නම් (result)
    *result = count * size;
  ආපසු (size > 0) && (count >= ((unsigned පූර්ණ_සංඛ්‍යා) -1) / size);
}

static anta_hstha bool
hb_unsigned_add_overflows (unsigned පූර්ණ_සංඛ්‍යා a, unsigned පූර්ණ_සංඛ්‍යා b, unsigned *result = nullptr)
{
#නම් hb_has_builtin(__builtin_add_overflow)
  unsigned stack_result;
  නම් (!result)
    result = &stack_result;
  ආපසු __builtin_add_overflow (a, b, result);
#endif

  නම් (result)
    *result = a + b;
  ආපසු b > (unsigned පූර්ණ_සංඛ්‍යා) -1 - a;
}

/* Saturating arithmetic on size_t.  On platforms where size_t is wider than
 * unsigned int (i.e. 64-bit), the inputs to get_size()-style computations
 * (counts and static_sizes which are at most 32-bit) cannot overflow size_t,
 * so these reduce to plain arithmetic.  On 32-bit platforms (size_t is
 * unsigned int), they saturate to SIZE_MAX so sanitize/serialize callers
 * naturally reject the resulting size as out-of-range. */

static anta_hstha size_t
hb_unsigned_mul_saturate (size_t a, size_t b)
{
  නම් (parimaa_na (size_t) > parimaa_na (unsigned පූර්ණ_සංඛ්‍යා))
    ආපසු a * b;
#නම් hb_has_builtin(__builtin_mul_overflow)
  size_t result;
  නම් (__builtin_mul_overflow (a, b, &result))
    ආපසු (size_t) -1;
  ආපසු result;
#නැතහොත්
  නම් (b > 0 && a > ((size_t) -1) / b) ආපසු (size_t) -1;
  ආපසු a * b;
#endif
}

static anta_hstha size_t
hb_unsigned_add_saturate (size_t a, size_t b)
{
  නම් (parimaa_na (size_t) > parimaa_na (unsigned පූර්ණ_සංඛ්‍යා))
    ආපසු a + b;
#නම් hb_has_builtin(__builtin_add_overflow)
  size_t result;
  නම් (__builtin_add_overflow (a, b, &result))
    ආපසු (size_t) -1;
  ආපසු result;
#නැතහොත්
  නම් (b > ((size_t) -1) - a) ආපසු (size_t) -1;
  ආපසු a + b;
#endif
}

/* Variadic forms: fold left across all arguments. */
template <typename ...Ts>
static anta_hstha size_t
hb_unsigned_mul_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ ආපසු hb_unsigned_mul_saturate (hb_unsigned_mul_saturate (a, b), c, rest...); }

template <typename ...Ts>
static anta_hstha size_t
hb_unsigned_add_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ ආපසු hb_unsigned_add_saturate (hb_unsigned_add_saturate (a, b), c, rest...); }

/* Saturating mul-add: a * b + c.  Covers the dominant get_size() pattern
 * `count * static_size + min_size`. */
static anta_hstha size_t
hb_unsigned_mul_add_saturate (size_t a, size_t b, size_t c)
{ ආපසු hb_unsigned_add_saturate (hb_unsigned_mul_saturate (a, b), c); }


/*
 * Sort and search.
 */

template <typename K, typename V, typename ...Ts>
static පූර්ණ_සංඛ්‍යා
_hb_cmp_method (const void *pkey, const void *pval, Ts... ds)
{
  const K& key = * (const K*) pkey;
  const V& val = * (const V*) pval;

  ආපසු val.cmp (key, ds...);
}

template <typename K, typename V>
static පූර්ණ_සංඛ්‍යා
_hb_cmp_operator (const void *pkey, const void *pval)
{
  const K& key = * (const K*) pkey;
  const V& val = * (const V*) pval;

  නම් (key < val) ආපසු -1;
  නම් (key > val) ආපසු  1;
  ආපසු 0;
}

template <typename V, typename K, typename ...Ts>
HB_HOT
static anta_hstha bool
hb_bsearch_impl (unsigned *pos, /* Out */
		 const K& key,
		 V* base, size_t nmemb, size_t stride,
		 පූර්ණ_සංඛ්‍යා (*compar)(const void *_key, const void *_item, Ts... _ds),
		 Ts... ds)
{
  /* This is our *only* bsearch implementation. */

  පූර්ණ_සංඛ්‍යා min = 0, max = (පූර්ණ_සංඛ්‍යා) nmemb - 1;
  අතරතුර (min <= max)
  {
    පූර්ණ_සංඛ්‍යා mid = ((unsigned පූර්ණ_සංඛ්‍යා) min + (unsigned පූර්ණ_සංඛ්‍යා) max) / 2;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    V* p = (V*) (((const var_na *) base) + (mid * stride));
#pragma GCC diagnostic pop
    පූර්ණ_සංඛ්‍යා c = compar ((const void *) std::addressof (key), (const void *) p, ds...);
    නම් (c < 0)
      max = mid - 1;
    නැතහොත් නම් (c > 0)
      min = mid + 1;
    නැතහොත්
    {
      *pos = mid;
      ආපසු true;
    }
  }
  *pos = min;
  ආපසු false;
}

template <typename V, typename K>
static anta_hstha V*
hb_bsearch (const K& key, V* base,
	    size_t nmemb, size_t stride = parimaa_na (V),
	    පූර්ණ_සංඛ්‍යා (*compar)(const void *_key, const void *_item) = _hb_cmp_method<K, V>)
{
  unsigned pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  ආපසු hb_bsearch_impl (&pos, key, base, nmemb, stride, compar) ?
	 (V*) (((const var_na *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic pop
}
template <typename V, typename K, typename ...Ts>
static anta_hstha V*
hb_bsearch (const K& key, V* base,
	    size_t nmemb, size_t stride,
	    පූර්ණ_සංඛ්‍යා (*compar)(const void *_key, const void *_item, Ts... _ds),
	    Ts... ds)
{
  unsigned pos;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  ආපසු hb_bsearch_impl (&pos, key, base, nmemb, stride, compar, ds...) ?
	 (V*) (((const var_na *) base) + (pos * stride)) : nullptr;
#pragma GCC diagnostic pop
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
static anta_hstha void
hb_qsort_loop (T *base, size_t nel, Compar compar)
{
  අතරතුර (nel > 24)
  {
    T *last = base + nel - 1;
    T *mid = base + nel / 2;

    /* Median-of-three pivot, parked at last-1. */
    නම් (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    නම් (compar (*mid, *last) > 0)
    {
      hb_swap (*mid, *last);
      නම් (compar (*base, *mid) > 0) hb_swap (*base, *mid);
    }
    hb_swap (*mid, *(last - 1));
    T &pivot = *(last - 1);

    /* Two-way Hoare partition.  Inner loops are unguarded:
     * median-of-three left *base <= pivot and *last >= pivot,
     * which act as sentinels. */
    T *i = base, *j = last - 1;
    අතරතුර (true)
    {
      අතරතුර (compar (*++i, pivot) < 0) {}
      අතරතුර (compar (*--j, pivot) > 0) {}
      නම් (i >= j) bha_nga;
      hb_swap (*i, *j);
    }
    hb_swap (*i, *(last - 1));

    /* Recurse on smaller side, loop on larger — bounds stack
     * depth at O(log n). */
    size_t left  = (size_t) (i - base);
    size_t right = nel - left - 1;
    නම් (left < right)
    {
      hb_qsort_loop (base, left, compar);
      base = i + 1;
      nel  = right;
    }
    නැතහොත්
    {
      hb_qsort_loop (i + 1, right, compar);
      nel  = left;
    }
  }
}

template <typename T, typename Compar>
static anta_hstha void
hb_qsort_inline (T *base, size_t nel, Compar compar)
{
  hb_qsort_loop (base, nel, compar);

  /* Single final insertion sort over the whole array.  After
   * the partitioning loop, every element is within the threshold
   * of its sorted position, so this pass is O(n * threshold). */
  T *end = base + nel;
  සඳහා (T *pi = base + 1; pi < end; pi++)
    සඳහා (T *pj = pi; pj > base && compar (pj[-1], pj[0]) > 0; pj--)
      hb_swap (pj[-1], pj[0]);
}

static anta_hstha void
hb_qsort (void *base, size_t nel, size_t width,
	  පූර්ණ_සංඛ්‍යා (*compar)(const void *_a, const void *_b))
{
  qsort (base, nel, width, compar);
}




template <typename T, typename T2, typename T3 = පූර්ණ_සංඛ්‍යා> static anta_hstha void
hb_stable_sort (T *array, unsigned පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා(*compar)(const T2 *, const T2 *), T3 *array2 = nullptr)
{
  static_assert (hb_is_trivially_copy_assignable (T), "");
  static_assert (hb_is_trivially_copy_assignable (T3), "");

  සඳහා (unsigned පූර්ණ_සංඛ්‍යා i = 1; i < len; i++)
  {
    unsigned පූර්ණ_සංඛ්‍යා j = i;
    අතරතුර (j && compar (&array[j - 1], &array[i]) > 0)
      j--;
    නම් (i == j)
      continue;
    /* Move item i to occupy place for item j, shift what's in between. */
    {
      T t = array[i];
      memmove (&array[j + 1], &array[j], (i - j) * parimaa_na (T));
      array[j] = t;
    }
    නම් (array2)
    {
      T3 t = array2[i];
      memmove (&array2[j + 1], &array2[j], (i - j) * parimaa_na (T3));
      array2[j] = t;
    }
  }
}

static anta_hstha hb_bool_t
hb_codepoint_parse (const var_na *s, unsigned පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා base, hb_codepoint_t *out)
{
  unsigned පූර්ණ_සංඛ්‍යා v;
  const var_na *p = s;
  const var_na *end = p + len;
  නම් (unlikely (!hb_parse_uint (&p, end, &v, true/* whole buffer */, base)))
    ආපසු false;

  *out = v;
  ආපසු true;
}


/* Operators. */

sa_nracanaa
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (const T &a, const T &b) const HB_AUTO_RETURN (a & b)
}
HB_FUNCOBJ (hb_bitwise_and);
sa_nracanaa
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (const T &a, const T &b) const HB_AUTO_RETURN (a | b)
}
HB_FUNCOBJ (hb_bitwise_or);
sa_nracanaa
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (const T &a, const T &b) const HB_AUTO_RETURN (a ^ b)
}
HB_FUNCOBJ (hb_bitwise_xor);
sa_nracanaa
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (const T &a, const T &b) const HB_AUTO_RETURN (~a & b)
}
HB_FUNCOBJ (hb_bitwise_lt);
sa_nracanaa
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (const T &a, const T &b) const HB_AUTO_RETURN (a & ~b)
}
HB_FUNCOBJ (hb_bitwise_gt); // aka sub
sa_nracanaa
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (const T &a, const T &b) const HB_AUTO_RETURN (~a | b)
}
HB_FUNCOBJ (hb_bitwise_le);
sa_nracanaa
{ HB_PARTIALIZE(2);
  template <typename T> constexpr auto
  operator () (const T &a, const T &b) const HB_AUTO_RETURN (a | ~b)
}
HB_FUNCOBJ (hb_bitwise_ge);
sa_nracanaa
{
  template <typename T> constexpr auto
  operator () (const T &a) const HB_AUTO_RETURN (~a)
}
HB_FUNCOBJ (hb_bitwise_neg);

sa_nracanaa
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (const T &a, const T2 &b) const HB_AUTO_RETURN (a + b)
}
HB_FUNCOBJ (hb_add);
sa_nracanaa
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (const T &a, const T2 &b) const HB_AUTO_RETURN (a - b)
}
HB_FUNCOBJ (hb_sub);
sa_nracanaa
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (const T &a, const T2 &b) const HB_AUTO_RETURN (b - a)
}
HB_FUNCOBJ (hb_rsub);
sa_nracanaa
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (const T &a, const T2 &b) const HB_AUTO_RETURN (a * b)
}
HB_FUNCOBJ (hb_mul);
sa_nracanaa
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (const T &a, const T2 &b) const HB_AUTO_RETURN (a / b)
}
HB_FUNCOBJ (hb_div);
sa_nracanaa
{ HB_PARTIALIZE(2);
  template <typename T, typename T2> constexpr auto
  operator () (const T &a, const T2 &b) const HB_AUTO_RETURN (a % b)
}
HB_FUNCOBJ (hb_mod);
sa_nracanaa
{
  template <typename T> constexpr auto
  operator () (const T &a) const HB_AUTO_RETURN (+a)
}
HB_FUNCOBJ (hb_pos);
sa_nracanaa
{
  template <typename T> constexpr auto
  operator () (const T &a) const HB_AUTO_RETURN (-a)
}
HB_FUNCOBJ (hb_neg);
sa_nracanaa
{
  template <typename T> constexpr auto
  operator () (T &a) const HB_AUTO_RETURN (++a)
}
HB_FUNCOBJ (hb_inc);
sa_nracanaa
{
  template <typename T> constexpr auto
  operator () (T &a) const HB_AUTO_RETURN (--a)
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
dviguu_na solve_itp (func_t f,
		  dviguu_na a, dviguu_na b,
		  dviguu_na epsilon,
		  dviguu_na min_y, dviguu_na max_y,
		  dviguu_na &ya, dviguu_na &yb, dviguu_na &y)
{
  // Guard against degenerate interval
  නම් (b - a <= 0.0)
  {
    y = ya;
    ආපසු a;
  }

  unsigned n1_2 = (unsigned) (hb_max (ceil (log2 ((b - a) / epsilon)) - 1.0, 0.0));
  const unsigned n0 = 1; // Hardwired
  const dviguu_na k1 = 0.2 / (b - a); // Hardwired.
  unsigned nmax = n0 + n1_2;
  dviguu_na scaled_epsilon = epsilon * dviguu_na (1llu << nmax);
  dviguu_na _2_epsilon = 2.0 * epsilon;
  අතරතුර (b - a > _2_epsilon)
  {
    dviguu_na x1_2 = 0.5 * (a + b);
    dviguu_na r = scaled_epsilon - 0.5 * (b - a);
    // Guard against yb == ya to prevent division by zero
    dviguu_na xf = (yb != ya) ? (yb * a - ya * b) / (yb - ya) : x1_2;
    dviguu_na sigma = x1_2 - xf;
    dviguu_na b_a = b - a;
    // This has k2 = 2 hardwired for efficiency.
    dviguu_na b_a_k2 = b_a * b_a;
    dviguu_na delta = k1 * b_a_k2;
    පූර්ණ_සංඛ්‍යා sigma_sign = sigma >= 0 ? +1 : -1;
    dviguu_na xt = delta <= fabs (x1_2 - xf) ? xf + delta * sigma_sign : x1_2;
    dviguu_na xitp = fabs (xt - x1_2) <= r ? xt : x1_2 - r * sigma_sign;
    dviguu_na yitp = f (xitp);
    නම් (yitp > max_y)
    {
      b = xitp;
      yb = yitp;
    }
    නැතහොත් නම් (yitp < min_y)
    {
      a = xitp;
      ya = yitp;
    }
    නැතහොත්
    {
      y = yitp;
      ආපසු xitp;
    }
    scaled_epsilon *= 0.5;
  }
  ආපසු 0.5 * (a + b);
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
sa_nracanaa hb_scope_guard_t
{
  explicit hb_scope_guard_t (F &&f) : f (std::move (f)), active (true) {}
  hb_scope_guard_t (hb_scope_guard_t &&o) noexcept
    : f (std::move (o.f)), active (o.active) { o.active = false; }
  hb_scope_guard_t (const hb_scope_guard_t &) = delete;
  hb_scope_guard_t &operator= (const hb_scope_guard_t &) = delete;
  hb_scope_guard_t &operator= (hb_scope_guard_t &&) = delete;
  ~hb_scope_guard_t () { නම් (active) f (); }

  /* Release: dismiss the guard so the cleanup does NOT run.  Use
   * when transferring ownership out of the scope. */
  void release () { active = false; }

  private:
  F f;
  bool active;
};

template <typename F>
static anta_hstha hb_scope_guard_t<F> hb_make_scope_guard (F &&f)
{ ආපසු hb_scope_guard_t<F> (std::forward<F> (f)); }

#define HB_SCOPE_GUARD_NAME_(line) hb_scope_guard_##line
#define HB_SCOPE_GUARD_NAME(line) HB_SCOPE_GUARD_NAME_(line)
#define HB_SCOPE_GUARD(stmt) \
  auto HB_SCOPE_GUARD_NAME(__LINE__) = \
    hb_make_scope_guard ([&]() { stmt; }); \
  (void) HB_SCOPE_GUARD_NAME(__LINE__)


#endif /* HB_ALGS_HH */
