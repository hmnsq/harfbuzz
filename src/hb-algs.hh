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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ALGS_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ALGS_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-meta.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-null.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-number.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ <algorithm>
#ਅਨ੍ਤਰ੍ਭਾਵ <initializer_list>
#ਅਨ੍ਤਰ੍ਭਾਵ <functional>
#ਅਨ੍ਤਰ੍ਭਾਵ <new>

ਸਥਿਰ HB_ALWAYS_INLINE ਖਾਲੀ *
hb_malloc2 (size_t nmemb, size_t ਮਾਪ)
{
  ਜੇ (ਮਾਪ && nmemb > SIZE_MAX / ਮਾਪ) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  ਵਾਪਸ hb_malloc (nmemb * ਮਾਪ);
}
ਸਥਿਰ HB_ALWAYS_INLINE ਖਾਲੀ *
hb_realloc2 (ਖਾਲੀ *ptr, size_t nmemb, size_t ਮਾਪ)
{
  ਜੇ (ਮਾਪ && nmemb > SIZE_MAX / ਮਾਪ) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  ਵਾਪਸ hb_realloc (ptr, nmemb * ਮਾਪ);
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
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
# pragma warning(disable:4200)
# pragma warning(disable:4800)
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MARK_AS_FLAG_T(T) \
	ਬਾਹਰੀ "C++" { \
	  ਸਥਿਰ ਸਤਰਅੰਦਰ constexpr T ਸੰਚਾਲਕ | (T l, T r) { ਵਾਪਸ T ((ਬਿਨਾਨਿਸ਼ਾਨੀ) l | (ਬਿਨਾਨਿਸ਼ਾਨੀ) r); } \
	  ਸਥਿਰ ਸਤਰਅੰਦਰ constexpr T ਸੰਚਾਲਕ & (T l, T r) { ਵਾਪਸ T ((ਬਿਨਾਨਿਸ਼ਾਨੀ) l & (ਬਿਨਾਨਿਸ਼ਾਨੀ) r); } \
	  ਸਥਿਰ ਸਤਰਅੰਦਰ constexpr T ਸੰਚਾਲਕ ^ (T l, T r) { ਵਾਪਸ T ((ਬਿਨਾਨਿਸ਼ਾਨੀ) l ^ (ਬਿਨਾਨਿਸ਼ਾਨੀ) r); } \
	  ਸਥਿਰ ਸਤਰਅੰਦਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੰਚਾਲਕ ~ (T r) { ਵਾਪਸ (~(ਬਿਨਾਨਿਸ਼ਾਨੀ) r); } \
	  ਸਥਿਰ ਸਤਰਅੰਦਰ T& ਸੰਚਾਲਕ |= (T &l, T r) { l = l | r; ਵਾਪਸ l; } \
	  ਸਥਿਰ ਸਤਰਅੰਦਰ T& ਸੰਚਾਲਕ &= (T& l, T r) { l = l & r; ਵਾਪਸ l; } \
	  ਸਥਿਰ ਸਤਰਅੰਦਰ T& ਸੰਚਾਲਕ ^= (T& l, T r) { l = l ^ r; ਵਾਪਸ l; } \
	} \
	static_assert (ਸੱਚ, "")

/* Useful for set-operations on small enums.
 * For example, for testing "x ∈ {x1, x2, x3}" use:
 * (FLAG_UNSAFE(x) & (FLAG(x1) | FLAG(x2) | FLAG(x3)))
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FLAG(x) (static_assert_ਵਿਅੰਜਕ ((ਬਿਨਾਨਿਸ਼ਾਨੀ)(x) < 32) + (((uint32_t) 1U) << (ਬਿਨਾਨਿਸ਼ਾਨੀ)(x)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FLAG_UNSAFE(x) ((ਬਿਨਾਨਿਸ਼ਾਨੀ)(x) < 32 ? (((uint32_t) 1U) << (ਬਿਨਾਨਿਸ਼ਾਨੀ)(x)) : 0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FLAG_RANGE(x,y) (static_assert_ਵਿਅੰਜਕ ((x) < (y)) + FLAG(y+1) - FLAG(x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FLAG64(x) (static_assert_ਵਿਅੰਜਕ ((ਬਿਨਾਨਿਸ਼ਾਨੀ)(x) < 64) + (((uint64_t) 1ULL) << (ਬਿਨਾਨਿਸ਼ਾਨੀ)(x)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FLAG64_UNSAFE(x) ((ਬਿਨਾਨਿਸ਼ਾਨੀ)(x) < 64 ? (((uint64_t) 1ULL) << (ਬਿਨਾਨਿਸ਼ਾਨੀ)(x)) : 0)


/*
 * Fixed-endian integers / floats.
 */


/* Endian swap, used in Windows related backends */
ਸਥਿਰ ਸਤਰਅੰਦਰ constexpr uint16_t hb_uint16_ਅਦਲਾ_ਬਦਲੀ (uint16_t v)
{ ਵਾਪਸ (v >> 8) | (v << 8); }
ਸਥਿਰ ਸਤਰਅੰਦਰ constexpr uint32_t hb_uint32_ਅਦਲਾ_ਬਦਲੀ (uint32_t v)
{ ਵਾਪਸ (hb_uint16_ਅਦਲਾ_ਬਦਲੀ (v) << 16) | hb_uint16_ਅਦਲਾ_ਬਦਲੀ (v >> 16); }

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ __attribute__((packed)) hb_packed_t
{
  hb_packed_t () = ਮੂਲ;
  constexpr hb_packed_t (Type V) : v (V) {}
  ਸੰਚਾਲਕ Type () ਅਚਲ { ਵਾਪਸ v; }
  hb_packed_t & ਸੰਚਾਲਕ = (Type V) { v = V; ਵਾਪਸ *ਇਹ; }

  ਨਿਜੀ:
  Type v;
};

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_FAST_ਅੰਕ_ACCESS

#ਜੇ defined(__OPTIMIZE__) && \
    defined(__BYTE_ORDER) && \
    (__BYTE_ORDER == __BIG_ENDIAN || \
     (__BYTE_ORDER == __LITTLE_ENDIAN && \
      hb_has_builtin(__builtin_bswap16) && \
      hb_has_builtin(__builtin_bswap32) && \
      hb_has_builtin(__builtin_bswap64)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FAST_ਅੰਕ_ACCESS 1
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FAST_ਅੰਕ_ACCESS 0
#ਜੇ_ਅੰਤ

// https://github.com/harfbuzz/harfbuzz/issues/5456
#ਜੇ defined(__GNUC__) && !defined(__clang__) && (__GNUC__ <= 12)
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_FAST_ਅੰਕ_ACCESS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FAST_ਅੰਕ_ACCESS 0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

ਸਾਂਚਾ <ਤਾਰਕਿਕ BE, ਕਿਸਮ_ਨਾਂ Type, ਪੂਰਨ ਬਾਈਟਾਂ_3 = ਆਕਾਰ (Type)>
ਬਣਤਰ HBInt;
ਸਾਂਚਾ <ਤਾਰਕਿਕ BE, ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ HBInt<BE, Type, 1>
{
  ਜਨਤਕ:
  HBInt () = ਮੂਲ;
  constexpr HBInt (Type V) : v {uint8_t (V)} {}
  constexpr ਸੰਚਾਲਕ Type () ਅਚਲ { ਵਾਪਸ v; }
  ਨਿਜੀ: uint8_t v;
};
ਸਾਂਚਾ <ਤਾਰਕਿਕ BE, ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ HBInt<BE, Type, 2>
{
  ਜਨਤਕ:
  HBInt () = ਮੂਲ;

  HBInt (Type V)
#ਜੇ HB_FAST_ਅੰਕ_ACCESS
  {
    ਜੇ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint16_t> *) v) = V;
    ਨਹੀਂਤਾਂ
      *((hb_packed_t<uint16_t> *) v) = __builtin_bswap16 (V);
  }
#ਨਹੀਂਤਾਂ
    : v {BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >>  8) & 0xFF)} {}
#ਜੇ_ਅੰਤ

  constexpr ਸੰਚਾਲਕ Type () ਅਚਲ
  {
#ਜੇ HB_FAST_ਅੰਕ_ACCESS
    ਵਾਪਸ (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint16_t) *((ਅਚਲ hb_packed_t<uint16_t> *) v)
    :
      __builtin_bswap16 ((uint16_t) *((ਅਚਲ hb_packed_t<uint16_t> *) v))
    ;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ (BE ? (v[0] <<  8) : (v[0]      ))
	 + (BE ? (v[1]      ) : (v[1] <<  8));
#ਜੇ_ਅੰਤ
  }
  ਨਿਜੀ: uint8_t v[2];
};
ਸਾਂਚਾ <ਤਾਰਕਿਕ BE, ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ HBInt<BE, Type, 3>
{
  static_assert (!std::is_signed<Type>::value, "");
  ਜਨਤਕ:
  HBInt () = ਮੂਲ;
  constexpr HBInt (Type V) : v {BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
				BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
				BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V      ) & 0xFF)} {}

  constexpr ਸੰਚਾਲਕ Type () ਅਚਲ { ਵਾਪਸ (BE ? (v[0] << 16) : (v[0]      ))
					  + (BE ? (v[1] <<  8) : (v[1] <<  8))
					  + (BE ? (v[2]      ) : (v[2] << 16)); }
  ਨਿਜੀ: uint8_t v[3];
};
ਸਾਂਚਾ <ਤਾਰਕਿਕ BE, ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ HBInt<BE, Type, 4>
{
  ਸਾਂਚਾ <ਤਾਰਕਿਕ, ਕਿਸਮ_ਨਾਂ, ਪੂਰਨ>
  ਮਿੱਤਰ ਬਣਤਰ HBFloat;

  ਜਨਤਕ:
  HBInt () = ਮੂਲ;

  HBInt (Type V)
#ਜੇ HB_FAST_ਅੰਕ_ACCESS
  {
    ਜੇ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint32_t> *) v) = V;
    ਨਹੀਂਤਾਂ
      *((hb_packed_t<uint32_t> *) v) = __builtin_bswap32 (V);
  }
#ਨਹੀਂਤਾਂ
    : v {BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 24) & 0xFF)} {}
#ਜੇ_ਅੰਤ

  constexpr ਸੰਚਾਲਕ Type () ਅਚਲ {
#ਜੇ HB_FAST_ਅੰਕ_ACCESS
    ਵਾਪਸ (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint32_t) *((ਅਚਲ hb_packed_t<uint32_t> *) v)
    :
      __builtin_bswap32 ((uint32_t) *((ਅਚਲ hb_packed_t<uint32_t> *) v))
    ;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ (BE ? (uint32_t (v[0]) << 24) : (uint32_t (v[0])      ))
	 + (BE ? (uint32_t (v[1]) << 16) : (uint32_t (v[1]) <<  8))
	 + (BE ? (uint32_t (v[2]) <<  8) : (uint32_t (v[2]) << 16))
	 + (BE ? (uint32_t (v[3])      ) : (uint32_t (v[3]) << 24));
#ਜੇ_ਅੰਤ
  }
  ਨਿਜੀ: uint8_t v[4];
};
ਸਾਂਚਾ <ਤਾਰਕਿਕ BE, ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ HBInt<BE, Type, 8>
{
  ਸਾਂਚਾ <ਤਾਰਕਿਕ, ਕਿਸਮ_ਨਾਂ, ਪੂਰਨ>
  ਮਿੱਤਰ ਬਣਤਰ HBFloat;

  ਜਨਤਕ:
  HBInt () = ਮੂਲ;

  HBInt (Type V)
#ਜੇ HB_FAST_ਅੰਕ_ACCESS
  {
    ਜੇ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint64_t> *) v) = V;
    ਨਹੀਂਤਾਂ
      *((hb_packed_t<uint64_t> *) v) = __builtin_bswap64 (V);
  }
#ਨਹੀਂਤਾਂ
    : v {BE ? uint8_t ((V >> 56) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 48) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >> 40) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V >> 32) & 0xFF) : uint8_t ((V >> 24) & 0xFF),
	 BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V >> 32) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 40) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 48) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 56) & 0xFF)} {}
#ਜੇ_ਅੰਤ

  constexpr ਸੰਚਾਲਕ Type () ਅਚਲ {
#ਜੇ HB_FAST_ਅੰਕ_ACCESS
    ਵਾਪਸ (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint64_t) *((ਅਚਲ hb_packed_t<uint64_t> *) v)
    :
      __builtin_bswap64 ((uint64_t) *((ਅਚਲ hb_packed_t<uint64_t> *) v))
    ;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ (BE ? (uint64_t (v[0]) << 56) : (uint64_t (v[0])      ))
	 + (BE ? (uint64_t (v[1]) << 48) : (uint64_t (v[1]) <<  8))
	 + (BE ? (uint64_t (v[2]) << 40) : (uint64_t (v[2]) << 16))
	 + (BE ? (uint64_t (v[3]) << 32) : (uint64_t (v[3]) << 24))
	 + (BE ? (uint64_t (v[4]) << 24) : (uint64_t (v[4]) << 32))
	 + (BE ? (uint64_t (v[5]) << 16) : (uint64_t (v[5]) << 40))
	 + (BE ? (uint64_t (v[6]) <<  8) : (uint64_t (v[6]) << 48))
	 + (BE ? (uint64_t (v[7])      ) : (uint64_t (v[7]) << 56));
#ਜੇ_ਅੰਤ
  }
  ਨਿਜੀ: uint8_t v[8];
};

/* Floats. */

ਸਾਂਚਾ <ਤਾਰਕਿਕ BE, ਕਿਸਮ_ਨਾਂ Type, ਪੂਰਨ ਬਾਈਟਾਂ_3>
ਬਣਤਰ HBFloat
{
  ਵਰਤੋਂ IntType = ਕਿਸਮ_ਨਾਂ std::conditional<ਬਾਈਟਾਂ_3 == 4, uint32_t, uint64_t>::type;

  ਜਨਤਕ:
  HBFloat () = ਮੂਲ;

  HBFloat (Type V)
  {
#ਜੇ HB_FAST_ਅੰਕ_ACCESS
    {
	      ਜੇ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
	      {
	        *((hb_packed_t<Type> *) v) = V;
	        ਵਾਪਸ;
	      }
	    }
#ਜੇ_ਅੰਤ

    ਸੰਘ {
      hb_packed_t<Type> f;
      hb_packed_t<IntType> i;
    } u = {{V}};

    ਅਚਲ HBInt<BE, IntType> I = (IntType) u.i;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਬਾਈਟਾਂ_3; i++)
      v[i] = I.v[i];
  }

  /* c++14 constexpr */ ਸੰਚਾਲਕ Type () ਅਚਲ
  {
#ਜੇ HB_FAST_ਅੰਕ_ACCESS
	    {
	      ਜੇ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
		ਵਾਪਸ (Type) *((ਅਚਲ hb_packed_t<Type> *) v);
	    }
#ਜੇ_ਅੰਤ

    HBInt<BE, IntType> I;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਬਾਈਟਾਂ_3; i++)
      I.v[i] = v[i];

    ਸੰਘ {
      hb_packed_t<IntType> i;
      hb_packed_t<Type> f;
    } u = {{I}};

    ਵਾਪਸ (Type) u.f;
  }
  ਨਿਜੀ: uint8_t v[ਬਾਈਟਾਂ_3];
};


/* We want our rounding towards +infinity. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ
_hb_roundf (ਦੁੱਗਣਾ x) { ਵਾਪਸ floor (x + .5); }

ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
_hb_roundf (ਦਸ਼ਮਲਵ x) { ਵਾਪਸ floor ((ਦੁੱਗਣਾ) x + .5); }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ roundf(x) _hb_roundf(x)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_sincos (ਦਸ਼ਮਲਵ rotation, ਦਸ਼ਮਲਵ &s, ਦਸ਼ਮਲਵ &c)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SINCOSF
  sincosf (rotation, &s, &c);
#ਨਹੀਂਤਾਂ
  c = cosf (rotation);
  s = sinf (rotation);
#ਜੇ_ਅੰਤ
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_sincos (ਦੁੱਗਣਾ rotation, ਦੁੱਗਣਾ &s, ਦੁੱਗਣਾ &c)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SINCOS
  sincos (rotation, &s, &c);
#ਨਹੀਂਤਾਂ
  c = ਕੋਜੀਵਾ (rotation);
  s = ਜੀਵਾ (rotation);
#ਜੇ_ਅੰਤ
}


/* Encodes three unsigned integers in one 64-bit number.  If the inputs have more than 21 bits,
 * values will be truncated / overlap, and might not decode exactly. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CODEPOINT_ENCODE3(x,y,z) (((uint64_t) (x) << 42) | ((uint64_t) (y) << 21) | (uint64_t) (z))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CODEPOINT_DECODE3_1(v) ((hb_codepoint_t) ((v) >> 42))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CODEPOINT_DECODE3_2(v) ((hb_codepoint_t) ((v) >> 21) & 0x1FFFFFu)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CODEPOINT_DECODE3_3(v) ((hb_codepoint_t) (v) & 0x1FFFFFu)

/* Custom encoding used by hb-ucd. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CODEPOINT_ENCODE3_11_7_14(x,y,z) (((uint32_t) ((x) & 0x07FFu) << 21) | (((uint32_t) (y) & 0x007Fu) << 14) | (uint32_t) ((z) & 0x3FFFu))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CODEPOINT_DECODE3_11_7_14_1(v) ((hb_codepoint_t) ((v) >> 21))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CODEPOINT_DECODE3_11_7_14_2(v) ((hb_codepoint_t) (((v) >> 14) & 0x007Fu) | 0x0300)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CODEPOINT_DECODE3_11_7_14_3(v) ((hb_codepoint_t) (v) & 0x3FFFu)


ਬਣਤਰ
{
  /* Note.  This is dangerous in that if it's passed an rvalue, it returns rvalue-reference. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (T&& v) ਅਚਲ HB_AUTO_RETURN ( std::forward<T> (v) )
}
HB_FUNCOBJ (hb_ਪਛਾਣ_ਤੱਤ);
ਬਣਤਰ
{
  /* Like identity(), but only retains lvalue-references.  Rvalues are returned as rvalues. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr T&
  ਸੰਚਾਲਕ () (T& v) ਅਚਲ { ਵਾਪਸ v; }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr hb_remove_reference<T>
  ਸੰਚਾਲਕ () (T&& v) ਅਚਲ { ਵਾਪਸ v; }
}
HB_FUNCOBJ (hb_lidentity);
ਬਣਤਰ
{
  /* Like identity(), but always returns rvalue. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr hb_remove_reference<T>
  ਸੰਚਾਲਕ () (T&& v) ਅਚਲ { ਵਾਪਸ v; }
}
HB_FUNCOBJ (hb_ridentity);

ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਤਾਰਕਿਕ
  ਸੰਚਾਲਕ () (T&& v) ਅਚਲ { ਵਾਪਸ ਤਾਰਕਿਕ (std::forward<T> (v)); }
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ fasthash_mix(h) (					\
			(ਖਾਲੀ) ((h) ^= (h) >> 23),		\
			(ਖਾਲੀ) ((h) *= 0x2127599bf4325c37ULL),	\
			(h) ^= (h) >> 47)

ਸਥਿਰ ਸਤਰਅੰਦਰ uint64_t fasthash64(ਅਚਲ ਖਾਲੀ *buf, size_t len, uint64_t ਬੀਜ)
{
	ਬਣਤਰ __attribute__((packed)) packed_uint64_t { uint64_t v; };
	ਅਚਲ uint64_t    m = 0x880355f21e6d1965ULL;
	ਅਚਲ packed_uint64_t *pos = (ਅਚਲ packed_uint64_t *)buf;
	ਅਚਲ packed_uint64_t *ਸਿਰਾ = pos + (len / 8);
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *pos2;
	uint64_t h = ਬੀਜ ^ (len * m);
	uint64_t v;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
	ਜੇ (((uintptr_t) pos & 7) == 0)
	{
	  ਜਦੋਂ (pos != ਸਿਰਾ)
	  {
#ਹਦਾਇਤ GCC diagnostic push
#ਹਦਾਇਤ GCC diagnostic ignored "-Wcast-align"
	    v  = * (ਅਚਲ uint64_t *) (pos++);
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}
	ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
	{
	  ਜਦੋਂ (pos != ਸਿਰਾ)
	  {
	    v  = pos++->v;
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}

	pos2 = (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*)pos;
	v = 0;

	ਚੋਣ (len & 7) {
	ਹਾਲਤ 7: v ^= (uint64_t)pos2[6] << 48; HB_FALLTHROUGH;
	ਹਾਲਤ 6: v ^= (uint64_t)pos2[5] << 40; HB_FALLTHROUGH;
	ਹਾਲਤ 5: v ^= (uint64_t)pos2[4] << 32; HB_FALLTHROUGH;
	ਹਾਲਤ 4: v ^= (uint64_t)pos2[3] << 24; HB_FALLTHROUGH;
	ਹਾਲਤ 3: v ^= (uint64_t)pos2[2] << 16; HB_FALLTHROUGH;
	ਹਾਲਤ 2: v ^= (uint64_t)pos2[1] <<  8; HB_FALLTHROUGH;
	ਹਾਲਤ 1: v ^= (uint64_t)pos2[0];
		h ^= fasthash_mix(v);
		h *= m;
	}

	ਵਾਪਸ fasthash_mix(h);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ uint32_t fasthash32(ਅਚਲ ਖਾਲੀ *buf, size_t len, uint32_t ਬੀਜ)
{
	// the following trick converts the 64-bit hashcode to Fermat
	// residue, which shall retain information from both the higher
	// and lower parts of hashcode.
        uint64_t h = fasthash64(buf, len, ਬੀਜ);
	ਵਾਪਸ h - (h >> 32);
}

ਬਣਤਰ
{
  ਨਿਜੀ:

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  impl (ਅਚਲ T& v, hb_ਤਰਜੀਹ<2>) ਅਚਲ HB_RETURN (uint32_t, hb_deref (v).hash ())

  // Horrible: std:hash() of integers seems to be identity in gcc / clang?!
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  //
  // For performance characteristics see:
  // https://github.com/harfbuzz/harfbuzz/pull/4228#issuecomment-1565079537
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T,
	    hb_enable_if (std::is_integral<T>::value && ਆਕਾਰ (T) <= ਆਕਾਰ (uint32_t))> constexpr ਸਵੈਚਾਲਿਤ
  impl (ਅਚਲ T& v, hb_ਤਰਜੀਹ<1>) ਅਚਲ HB_RETURN (uint32_t, (uint32_t) v * 2654435761u /* Knuh's multiplicative hash */)
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T,
	    hb_enable_if (std::is_integral<T>::value && ਆਕਾਰ (T) > ਆਕਾਰ (uint32_t))> constexpr ਸਵੈਚਾਲਿਤ
  impl (ਅਚਲ T& v, hb_ਤਰਜੀਹ<1>) ਅਚਲ HB_RETURN (uint32_t, (uint32_t) (v ^ (v >> 32)) * 2654435761u /* Knuth's multiplicative hash */)

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T,
	    hb_enable_if (std::is_floating_point<T>::value)> constexpr ਸਵੈਚਾਲਿਤ
  impl (ਅਚਲ T& v, hb_ਤਰਜੀਹ<1>) ਅਚਲ HB_RETURN (uint32_t, fasthash32 (std::addressof (v), ਆਕਾਰ (T), 0xf437ffe6))

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  impl (ਅਚਲ T& v, hb_ਤਰਜੀਹ<0>) ਅਚਲ HB_RETURN (uint32_t, std::hash<hb_decay<decltype (hb_deref (v))>>{} (hb_deref (v)))

  ਜਨਤਕ:

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ T& v) ਅਚਲ HB_RETURN (uint32_t, impl (v, hb_prioritize))
}
HB_FUNCOBJ (hb_hash);


ਬਣਤਰ
{
  ਨਿਜੀ:

  /* Pointer-to-member-function. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Appl, ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  impl (Appl&& a, hb_ਤਰਜੀਹ<2>, T &&v, Ts&&... ds) ਅਚਲ HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v)).*std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  /* Pointer-to-member. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Appl, ਕਿਸਮ_ਨਾਂ T> ਸਵੈਚਾਲਿਤ
  impl (Appl&& a, hb_ਤਰਜੀਹ<1>, T &&v) ਅਚਲ HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v))).*std::forward<Appl> (a))

  /* Operator(). */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Appl, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  impl (Appl&& a, hb_ਤਰਜੀਹ<0>, Ts&&... ds) ਅਚਲ HB_AUTO_RETURN
  (hb_deref (std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  ਜਨਤਕ:

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Appl, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (Appl&& a, Ts&&... ds) ਅਚਲ HB_AUTO_RETURN
  (
    impl (std::forward<Appl> (a),
	  hb_prioritize,
	  std::forward<Ts> (ds)...)
  )
}
HB_FUNCOBJ (hb_invoke);

ਸਾਂਚਾ <ਬਿਨਾਨਿਸ਼ਾਨੀ Pos, ਕਿਸਮ_ਨਾਂ Appl, ਕਿਸਮ_ਨਾਂ V>
ਬਣਤਰ hb_partial_t
{
  hb_partial_t (Appl a, V v) : a (a), v (v) {}

  static_assert (Pos > 0, "");

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ...Ts,
	    ਬਿਨਾਨਿਸ਼ਾਨੀ P = Pos,
	    hb_enable_if (P == 1)> ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (Ts&& ...ds) -> decltype (hb_invoke (hb_declval (Appl),
						   hb_declval (V),
						   hb_declval (Ts)...))
  {
    ਵਾਪਸ hb_invoke (std::forward<Appl> (a),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T0, ਕਿਸਮ_ਨਾਂ ...Ts,
	    ਬਿਨਾਨਿਸ਼ਾਨੀ P = Pos,
	    hb_enable_if (P == 2)> ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (T0&& d0, Ts&& ...ds) -> decltype (hb_invoke (hb_declval (Appl),
							    hb_declval (T0),
							    hb_declval (V),
							    hb_declval (Ts)...))
  {
    ਵਾਪਸ hb_invoke (std::forward<Appl> (a),
		      std::forward<T0> (d0),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }

  ਨਿਜੀ:
  hb_reference_wrapper<Appl> a;
  V v;
};
ਸਾਂਚਾ <ਬਿਨਾਨਿਸ਼ਾਨੀ Pos=1, ਕਿਸਮ_ਨਾਂ Appl, ਕਿਸਮ_ਨਾਂ V>
ਸਵੈਚਾਲਿਤ hb_partial (Appl&& a, V&& v) HB_AUTO_RETURN
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
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
/* https://github.com/harfbuzz/harfbuzz/issues/1730 */ \
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PARTIALIZE(Pos) \
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ _T> \
  decltype(ਸਵੈਚਾਲਿਤ) ਸੰਚਾਲਕ () (_T&& _v) ਅਚਲ \
  { ਵਾਪਸ hb_partial<Pos> (ਇਹ, std::forward<_T> (_v)); } \
  static_assert (ਸੱਚ, "")
#ਨਹੀਂਤਾਂ
/* https://github.com/harfbuzz/harfbuzz/issues/1724 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PARTIALIZE(Pos) \
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ _T> \
  ਸਵੈਚਾਲਿਤ ਸੰਚਾਲਕ () (_T&& _v) ਅਚਲ HB_AUTO_RETURN \
  (hb_partial<Pos> (+ਇਹ, std::forward<_T> (_v))) \
  static_assert (ਸੱਚ, "")
#ਜੇ_ਅੰਤ


ਬਣਤਰ
{
  ਨਿਜੀ:

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Pred, ਕਿਸਮ_ਨਾਂ Val> ਸਵੈਚਾਲਿਤ
  impl (Pred&& p, Val &&v, hb_ਤਰਜੀਹ<1>) ਅਚਲ HB_AUTO_RETURN
  (
    hb_deref (std::forward<Pred> (p)).has (std::forward<Val> (v))
  )

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Pred, ਕਿਸਮ_ਨਾਂ Val> ਸਵੈਚਾਲਿਤ
  impl (Pred&& p, Val &&v, hb_ਤਰਜੀਹ<0>) ਅਚਲ HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Pred> (p),
	       std::forward<Val> (v))
  )

  ਜਨਤਕ:

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Pred, ਕਿਸਮ_ਨਾਂ Val> ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (Pred&& p, Val &&v) ਅਚਲ HB_RETURN (ਤਾਰਕਿਕ,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_has);

ਬਣਤਰ
{
  ਨਿਜੀ:

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Pred, ਕਿਸਮ_ਨਾਂ Val> ਸਵੈਚਾਲਿਤ
  impl (Pred&& p, Val &&v, hb_ਤਰਜੀਹ<1>) ਅਚਲ HB_AUTO_RETURN
  (
    hb_has (std::forward<Pred> (p),
	    std::forward<Val> (v))
  )

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Pred, ਕਿਸਮ_ਨਾਂ Val> ਸਵੈਚਾਲਿਤ
  impl (Pred&& p, Val &&v, hb_ਤਰਜੀਹ<0>) ਅਚਲ HB_AUTO_RETURN
  (
    std::forward<Pred> (p) == std::forward<Val> (v)
  )

  ਜਨਤਕ:

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Pred, ਕਿਸਮ_ਨਾਂ Val> ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (Pred&& p, Val &&v) ਅਚਲ HB_RETURN (ਤਾਰਕਿਕ,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_ਮੇਲ);

ਬਣਤਰ
{
  ਨਿਜੀ:

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Proj, ਕਿਸਮ_ਨਾਂ Val> ਸਵੈਚਾਲਿਤ
  impl (Proj&& f, Val &&v, hb_ਤਰਜੀਹ<2>) ਅਚਲ HB_AUTO_RETURN
  (
    hb_deref (std::forward<Proj> (f)).get (std::forward<Val> (v))
  )

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Proj, ਕਿਸਮ_ਨਾਂ Val> ਸਵੈਚਾਲਿਤ
  impl (Proj&& f, Val &&v, hb_ਤਰਜੀਹ<1>) ਅਚਲ HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Proj> (f),
	       std::forward<Val> (v))
  )

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Proj, ਕਿਸਮ_ਨਾਂ Val> ਸਵੈਚਾਲਿਤ
  impl (Proj&& f, Val &&v, hb_ਤਰਜੀਹ<0>) ਅਚਲ HB_AUTO_RETURN
  (
    std::forward<Proj> (f)[std::forward<Val> (v)]
  )

  ਜਨਤਕ:

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Proj, ਕਿਸਮ_ਨਾਂ Val> ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (Proj&& f, Val &&v) ਅਚਲ HB_AUTO_RETURN
  (
    impl (std::forward<Proj> (f),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_get);

ਬਣਤਰ
{
  ਨਿਜੀ:

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T1, ਕਿਸਮ_ਨਾਂ T2> ਸਵੈਚਾਲਿਤ
  impl (T1&& v1, T2 &&v2, hb_ਤਰਜੀਹ<3>) ਅਚਲ HB_AUTO_RETURN
  (
    std::forward<T2> (v2).cmp (std::forward<T1> (v1)) == 0
  )

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T1, ਕਿਸਮ_ਨਾਂ T2> ਸਵੈਚਾਲਿਤ
  impl (T1&& v1, T2 &&v2, hb_ਤਰਜੀਹ<2>) ਅਚਲ HB_AUTO_RETURN
  (
    std::forward<T1> (v1).cmp (std::forward<T2> (v2)) == 0
  )

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T1, ਕਿਸਮ_ਨਾਂ T2> ਸਵੈਚਾਲਿਤ
  impl (T1&& v1, T2 &&v2, hb_ਤਰਜੀਹ<1>) ਅਚਲ HB_AUTO_RETURN
  (
    std::forward<T1> (v1) == std::forward<T2> (v2)
  )

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T1, ਕਿਸਮ_ਨਾਂ T2> ਸਵੈਚਾਲਿਤ
  impl (T1&& v1, T2 &&v2, hb_ਤਰਜੀਹ<0>) ਅਚਲ HB_AUTO_RETURN
  (
    std::forward<T2> (v2) == std::forward<T1> (v1)
  )

  ਜਨਤਕ:

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T1, ਕਿਸਮ_ਨਾਂ T2> ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (T1&& v1, T2 &&v2) ਅਚਲ HB_AUTO_RETURN
  (
    impl (std::forward<T1> (v1),
	  std::forward<T2> (v2),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_equal);

ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਖਾਲੀ
  ਸੰਚਾਲਕ () (T& a, T& b) ਅਚਲ
  {
    ਵਰਤੋਂ std::swap; // allow ADL
    swap (a, b);
  }
}
HB_FUNCOBJ (hb_ਅਦਲਾ_ਬਦਲੀ);


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T1, ਕਿਸਮ_ਨਾਂ T2>
ਬਣਤਰ hb_pair_t
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ T1 ਪਹਿਲਾ_t;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ T2 ਦੂਜਾ_t;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_pair_t<T1, T2> pair_t;

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U1 = T1, ਕਿਸਮ_ਨਾਂ U2 = T2,
	    hb_enable_if (std::is_default_constructible<U1>::value &&
			  std::is_default_constructible<U2>::value)>
  hb_pair_t () : ਪਹਿਲਾ (), second () {}
  hb_pair_t (T1 a, T2 b) : ਪਹਿਲਾ (std::forward<T1> (a)), second (std::forward<T2> (b)) {}

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Q1, ਕਿਸਮ_ਨਾਂ Q2,
	    hb_enable_if (hb_is_convertible (T1, Q1) &&
			  hb_is_convertible (T2, Q2))>
  ਸੰਚਾਲਕ hb_pair_t<Q1, Q2> () { ਵਾਪਸ hb_pair_t<Q1, Q2> (ਪਹਿਲਾ, second); }

  hb_pair_t<T1, T2> reverse () ਅਚਲ
  { ਵਾਪਸ hb_pair_t<T1, T2> (second, ਪਹਿਲਾ); }

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ pair_t& o) ਅਚਲ { ਵਾਪਸ ਪਹਿਲਾ == o.ਪਹਿਲਾ && second == o.second; }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ pair_t& o) ਅਚਲ { ਵਾਪਸ !(*ਇਹ == o); }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ < (ਅਚਲ pair_t& o) ਅਚਲ { ਵਾਪਸ ਪਹਿਲਾ < o.ਪਹਿਲਾ || (ਪਹਿਲਾ == o.ਪਹਿਲਾ && second < o.second); }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ >= (ਅਚਲ pair_t& o) ਅਚਲ { ਵਾਪਸ !(*ਇਹ < o); }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ > (ਅਚਲ pair_t& o) ਅਚਲ { ਵਾਪਸ ਪਹਿਲਾ > o.ਪਹਿਲਾ || (ਪਹਿਲਾ == o.ਪਹਿਲਾ && second > o.second); }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ <= (ਅਚਲ pair_t& o) ਅਚਲ { ਵਾਪਸ !(*ਇਹ > o); }

  ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb)
  {
    pair_t *a = (pair_t *) pa;
    pair_t *b = (pair_t *) pb;

    ਜੇ (a->ਪਹਿਲਾ < b->ਪਹਿਲਾ) ਵਾਪਸ -1;
    ਜੇ (a->ਪਹਿਲਾ > b->ਪਹਿਲਾ) ਵਾਪਸ +1;
    ਜੇ (a->second < b->second) ਵਾਪਸ -1;
    ਜੇ (a->second > b->second) ਵਾਪਸ +1;
    ਵਾਪਸ 0;
  }

  ਮਿੱਤਰ ਖਾਲੀ swap (hb_pair_t& a, hb_pair_t& b) noexcept
  {
    hb_ਅਦਲਾ_ਬਦਲੀ (a.ਪਹਿਲਾ, b.ਪਹਿਲਾ);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.second, b.second);
  }


  T1 ਪਹਿਲਾ;
  T2 second;
};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T1, ਕਿਸਮ_ਨਾਂ T2> ਸਥਿਰ ਸਤਰਅੰਦਰ hb_pair_t<T1, T2>
hb_pair (T1&& a, T2&& b) { ਵਾਪਸ hb_pair_t<T1, T2> (a, b); }

ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_pair_t<hb_codepoint_t, hb_codepoint_t> hb_codepoint_pair_t;

ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Pair> constexpr ਕਿਸਮ_ਨਾਂ Pair::ਪਹਿਲਾ_t
  ਸੰਚਾਲਕ () (ਅਚਲ Pair& pair) ਅਚਲ { ਵਾਪਸ pair.ਪਹਿਲਾ; }
}
HB_FUNCOBJ (hb_ਪਹਿਲਾ);

ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Pair> constexpr ਕਿਸਮ_ਨਾਂ Pair::ਦੂਜਾ_t
  ਸੰਚਾਲਕ () (ਅਚਲ Pair& pair) ਅਚਲ { ਵਾਪਸ pair.second; }
}
HB_FUNCOBJ (hb_ਦੂਜਾ);

/* Note.  In min/max impl, we can use hb_type_identity<T> for second argument.
 * However, that would silently convert between different-signedness integers.
 * Instead we accept two different types, such that compiler can err if
 * comparing integers of different signedness. */
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ T2> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (T&& a, T2&& b) ਅਚਲ HB_AUTO_RETURN
  (a <= b ? a : b)
}
HB_FUNCOBJ (hb_min);
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ T2> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (T&& a, T2&& b) ਅਚਲ HB_AUTO_RETURN
  (a >= b ? a : b)
}
HB_FUNCOBJ (hb_max);
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ T2, ਕਿਸਮ_ਨਾਂ T3> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (T&& x, T2&& min, T3&& max) ਅਚਲ HB_AUTO_RETURN
  (hb_min (hb_max (std::forward<T> (x), std::forward<T2> (min)), std::forward<T3> (max)))
}
HB_FUNCOBJ (hb_clamp);

/* Signed saturating arithmetic. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ HB_ALWAYS_INLINE T
hb_saturate_ਜੋੜੋ (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#ਜੇ hb_has_builtin(__builtin_add_overflow)
  T ਨਤੀਜਾ;
  ਜੇ (likely (!__builtin_add_overflow (a, b, &ਨਤੀਜਾ)))
    ਵਾਪਸ ਨਤੀਜਾ;
#ਨਹੀਂਤਾਂ
  ਜੇ (b > 0)
  {
    ਜੇ (likely (a <= hb_int_max (T) - b))
      ਵਾਪਸ a + b;
  }
  ਨਹੀਂਤਾਂ ਜੇ (likely (a >= hb_int_min (T) - b))
    ਵਾਪਸ a + b;
#ਜੇ_ਅੰਤ

  ਵਾਪਸ a < 0 ? hb_int_min (T) : hb_int_max (T);
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ HB_ALWAYS_INLINE T
hb_saturate_sub (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#ਜੇ hb_has_builtin(__builtin_sub_overflow)
  T ਨਤੀਜਾ;
  ਜੇ (likely (!__builtin_sub_overflow (a, b, &ਨਤੀਜਾ)))
    ਵਾਪਸ ਨਤੀਜਾ;
#ਨਹੀਂਤਾਂ
  ਜੇ (b > 0)
  {
    ਜੇ (likely (a >= hb_int_min (T) + b))
      ਵਾਪਸ a - b;
  }
  ਨਹੀਂਤਾਂ ਜੇ (likely (a <= hb_int_max (T) + b))
    ਵਾਪਸ a - b;
#ਜੇ_ਅੰਤ

  ਵਾਪਸ b < 0 ? hb_int_max (T) : hb_int_min (T);
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ HB_ALWAYS_INLINE T
hb_saturate_neg (T a)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  ਵਾਪਸ hb_saturate_sub ((T) 0, a);
}

/* Convert a floating-point value to integer type T, saturating to T's
 * range instead of relying on the undefined behavior of an out-of-range
 * float-to-int conversion.  NaN saturates to the minimum of T. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ HB_ALWAYS_INLINE T
hb_clamp_ਮੰਜ਼ਿਲ (ਦੁੱਗਣਾ v)
{
  ਵਾਪਸ (T) hb_clamp (v,
		       (ਦੁੱਗਣਾ) hb_int_min (T),
		       (ਦੁੱਗਣਾ) hb_int_max (T));
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ HB_ALWAYS_INLINE T
hb_clamp_ਮੰਜ਼ਿਲ (ਦਸ਼ਮਲਵ v)
{
  /* Widen explicitly: float cannot represent all integer bounds exactly,
   * so the clamp itself is done in double. */
  ਵਾਪਸ hb_clamp_ਮੰਜ਼ਿਲ<T> ((ਦੁੱਗਣਾ) v);
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ HB_ALWAYS_INLINE T
hb_clamp_ਮੰਜ਼ਿਲ (int64_t v)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  ਵਾਪਸ (T) hb_clamp (v,
		       (int64_t) hb_int_min (T),
		       (int64_t) hb_int_max (T));
}

/*
 * Bithacks.
 */

/* Return the number of 1 bits in a uint8_t; faster than hb_popcount() */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_popcount8 (uint8_t v)
{
  ਸਥਿਰ ਅਚਲ uint8_t popcount4[16] = {
    0, 1, 1, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 2, 3, 3, 4
  };
  ਵਾਪਸ popcount4[v & 0xF] + popcount4[v >> 4];
}

/* Return the number of 1 bits in v. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_popcount (T v)
{
#ਜੇ hb_has_builtin(__builtin_popcount)
  ਜੇ (ਆਕਾਰ (T) <= ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ))
    ਵਾਪਸ __builtin_popcount (v);
#ਜੇ_ਅੰਤ

#ਜੇ hb_has_builtin(__builtin_popcountl)
  ਜੇ (ਆਕਾਰ (T) <= ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ))
    ਵਾਪਸ __builtin_popcountl (v);
#ਜੇ_ਅੰਤ

#ਜੇ hb_has_builtin(__builtin_popcountll)
  ਜੇ (ਆਕਾਰ (T) <= ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ))
    ਵਾਪਸ __builtin_popcountll (v);
#ਜੇ_ਅੰਤ

  ਜੇ (ਆਕਾਰ (T) <= 4)
  {
    /* "HACKMEM 169" */
    uint32_t y;
    y = (v >> 1) &033333333333;
    y = v - y - ((y >>1) & 033333333333);
    ਵਾਪਸ (((y + (y >> 3)) & 030707070707) % 077);
  }

  ਜੇ (ਆਕਾਰ (T) == 8)
  {
    uint64_t y = (uint64_t) v;
    y -= ((y >> 1) & 0x5555555555555555ull);
    y = (y & 0x3333333333333333ull) + (y >> 2 & 0x3333333333333333ull);
    ਵਾਪਸ ((y + (y >> 4)) & 0xf0f0f0f0f0f0f0full) * 0x101010101010101ull >> 56;
  }

  ਜੇ (ਆਕਾਰ (T) == 16)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖਿਸਕਾਓ = 64;
    ਵਾਪਸ hb_popcount<uint64_t> ((uint64_t) v) + hb_popcount ((uint64_t) (v >> ਖਿਸਕਾਓ));
  }

  ਪ੍ਰਤਿਜ੍ਞਾ (0);
  ਵਾਪਸ 0; /* Shut up stupid compiler. */
}

/* Returns the number of bits needed to store number */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (T v)
{
  ਜੇ (unlikely (!v)) ਵਾਪਸ 0;

#ਜੇ hb_has_builtin(__builtin_clz)
  ਜੇ (ਆਕਾਰ (T) <= ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ))
    ਵਾਪਸ ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) * 8 - __builtin_clz (v);
#ਜੇ_ਅੰਤ

#ਜੇ hb_has_builtin(__builtin_clzl)
  ਜੇ (ਆਕਾਰ (T) <= ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ))
    ਵਾਪਸ ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) * 8 - __builtin_clzl (v);
#ਜੇ_ਅੰਤ

#ਜੇ hb_has_builtin(__builtin_clzll)
  ਜੇ (ਆਕਾਰ (T) <= ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ))
    ਵਾਪਸ ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) * 8 - __builtin_clzll (v);
#ਜੇ_ਅੰਤ

#ਜੇ (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  ਜੇ (ਆਕਾਰ (T) <= ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ))
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ where;
    ਦੋਆਧਾਰੀ_ਅੰਕ_ਜਾਂਚ_ਉਲਟ (&where, v);
    ਵਾਪਸ 1 + where;
  }
# ਜੇ defined(_WIN64)
  ਜੇ (ਆਕਾਰ (T) <= 8)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ where;
    ਦੋਆਧਾਰੀ_ਅੰਕ_ਜਾਂਚ_Reverse64 (&where, v);
    ਵਾਪਸ 1 + where;
  }
# endif
#ਜੇ_ਅੰਤ

  ਜੇ (ਆਕਾਰ (T) <= 4)
  {
    /* "bithacks" */
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ S[] = {1, 2, 4, 8, 16};
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ r = 0;
    ਲਈ (ਪੂਰਨ i = 4; i >= 0; i--)
      ਜੇ (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    ਵਾਪਸ r + 1;
  }
  ਜੇ (ਆਕਾਰ (T) <= 8)
  {
    /* "bithacks" */
    ਅਚਲ uint64_t b[] = {0x2ULL, 0xCULL, 0xF0ULL, 0xFF00ULL, 0xFFFF0000ULL, 0xFFFFFFFF00000000ULL};
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ S[] = {1, 2, 4, 8, 16, 32};
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ r = 0;
    ਲਈ (ਪੂਰਨ i = 5; i >= 0; i--)
      ਜੇ (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    ਵਾਪਸ r + 1;
  }
  ਜੇ (ਆਕਾਰ (T) == 16)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖਿਸਕਾਓ = 64;
    ਵਾਪਸ (v >> ਖਿਸਕਾਓ) ? hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage<uint64_t> ((uint64_t) (v >> ਖਿਸਕਾਓ)) + ਖਿਸਕਾਓ :
			  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage<uint64_t> ((uint64_t) v);
  }

  ਪ੍ਰਤਿਜ੍ਞਾ (0);
  ਵਾਪਸ 0; /* Shut up stupid compiler. */
}

/* Returns the number of zero bits in the least significant side of v */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ctz (T v)
{
  ਜੇ (unlikely (!v)) ਵਾਪਸ 8 * ਆਕਾਰ (T);

#ਜੇ hb_has_builtin(__builtin_ctz)
  ਜੇ (ਆਕਾਰ (T) <= ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ))
    ਵਾਪਸ __builtin_ctz (v);
#ਜੇ_ਅੰਤ

#ਜੇ hb_has_builtin(__builtin_ctzl)
  ਜੇ (ਆਕਾਰ (T) <= ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ))
    ਵਾਪਸ __builtin_ctzl (v);
#ਜੇ_ਅੰਤ

#ਜੇ hb_has_builtin(__builtin_ctzll)
  ਜੇ (ਆਕਾਰ (T) <= ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ))
    ਵਾਪਸ __builtin_ctzll (v);
#ਜੇ_ਅੰਤ

#ਜੇ (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  ਜੇ (ਆਕਾਰ (T) <= ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ))
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ where;
    ਦੋਆਧਾਰੀ_ਅੰਕ_ਜਾਂਚ_Forward (&where, v);
    ਵਾਪਸ where;
  }
# ਜੇ defined(_WIN64)
  ਜੇ (ਆਕਾਰ (T) <= 8)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ where;
    ਦੋਆਧਾਰੀ_ਅੰਕ_ਜਾਂਚ_Forward64 (&where, v);
    ਵਾਪਸ where;
  }
# endif
#ਜੇ_ਅੰਤ

  ਜੇ (ਆਕਾਰ (T) <= 4)
  {
    /* "bithacks" */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ c = 32;
    v &= - (int32_t) v;
    ਜੇ (v) c--;
    ਜੇ (v & 0x0000FFFF) c -= 16;
    ਜੇ (v & 0x00FF00FF) c -= 8;
    ਜੇ (v & 0x0F0F0F0F) c -= 4;
    ਜੇ (v & 0x33333333) c -= 2;
    ਜੇ (v & 0x55555555) c -= 1;
    ਵਾਪਸ c;
  }
  ਜੇ (ਆਕਾਰ (T) <= 8)
  {
    /* "bithacks" */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ c = 64;
    v &= - (int64_t) (v);
    ਜੇ (v) c--;
    ਜੇ (v & 0x00000000FFFFFFFFULL) c -= 32;
    ਜੇ (v & 0x0000FFFF0000FFFFULL) c -= 16;
    ਜੇ (v & 0x00FF00FF00FF00FFULL) c -= 8;
    ਜੇ (v & 0x0F0F0F0F0F0F0F0FULL) c -= 4;
    ਜੇ (v & 0x3333333333333333ULL) c -= 2;
    ਜੇ (v & 0x5555555555555555ULL) c -= 1;
    ਵਾਪਸ c;
  }
  ਜੇ (ਆਕਾਰ (T) == 16)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖਿਸਕਾਓ = 64;
    ਵਾਪਸ (uint64_t) v ? hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage<uint64_t> ((uint64_t) v) :
			  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage<uint64_t> ((uint64_t) (v >> ਖਿਸਕਾਓ)) + ਖਿਸਕਾਓ;
  }

  ਪ੍ਰਤਿਜ੍ਞਾ (0);
  ਵਾਪਸ 0; /* Shut up stupid compiler. */
}


/*
 * Tiny stuff.
 */

/* ASCII tag/character handling */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ISALPHA (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c)
{ ਵਾਪਸ (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ISALNUM (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c)
{ ਵਾਪਸ (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ISSPACE (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c)
{ ਵਾਪਸ c == ' ' || c =='\f'|| c =='\n'|| c =='\r'|| c =='\t'|| c =='\v'; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ TOUPPER (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c)
{ ਵਾਪਸ (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ TOLOWER (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c)
{ ਵਾਪਸ (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ISHEX (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c)
{ ਵਾਪਸ (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ TOHEX (uint8_t c)
{ ਵਾਪਸ (c & 0xF) <= 9 ? (c & 0xF) + '0' : (c & 0xF) + 'a' - 10; }
ਸਥਿਰ ਸਤਰਅੰਦਰ uint8_t FROMHEX (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c)
{ ਵਾਪਸ (c >= '0' && c <= '9') ? c - '0' : TOLOWER (c) - 'a' + 10; }

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ DIV_CEIL (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ a, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ b)
{ ਵਾਪਸ (a + (b - 1)) / b; }


#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  ARRAY_ਲੰਮਾਈ
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ARRAY_ਲੰਮਾਈ (ਅਚਲ Type (&)[n]) { ਵਾਪਸ n; }
/* A const version, but does not detect erratically being called on pointers. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ARRAY_ਲੰਮਾਈ_CONST(__array) ((ਨਿਸ਼ਾਨੀਵਾਲਾ ਪੂਰਨ) (ਆਕਾਰ (__array) / ਆਕਾਰ (__array[0])))


ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ *
hb_memcpy (ਖਾਲੀ *__restrict dst, ਅਚਲ ਖਾਲੀ *__restrict src, size_t len)
{
  /* It's illegal to pass 0 as size to memcpy. */
  ਜੇ (unlikely (!len)) ਵਾਪਸ dst;
  ਵਾਪਸ ਸਮ੍ਰਿਤੀ_ਨਕਲ (dst, src, len);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
hb_memcmp (ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len)
{
  /* It's illegal to pass NULL to memcmp(), even if len is zero.
   * So, wrap it.
   * https://sourceware.org/bugzilla/show_bug.cgi?id=23878 */
  ਜੇ (unlikely (!len)) ਵਾਪਸ 0;
  ਵਾਪਸ memcmp (a, b, len);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ *
hb_memset (ਖਾਲੀ *s, ਪੂਰਨ c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n)
{
  /* It's illegal to pass NULL to memset(), even if n is zero. */
  ਜੇ (unlikely (!n)) ਵਾਪਸ s;
  ਵਾਪਸ memset (s, c, n);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ceil_ਮੰਜ਼ਿਲ_4 (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ v)
{
  ਵਾਪਸ ((v - 1) | 3) + 1;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
hb_ਵਿੱਚ_range (T u, T lo, T hi)
{
  static_assert (!std::is_signed<T>::value, "");

  /* The casts below are important as if T is smaller than int,
   * the subtract results will become a signed int! */
  ਵਾਪਸ (T)(u - lo) <= (T)(hi - lo);
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
hb_ਵਿੱਚ_ranges (T u, T lo1, T hi1)
{
  ਵਾਪਸ hb_ਵਿੱਚ_range (u, lo1, hi1);
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
hb_ਵਿੱਚ_ranges (T u, T lo1, T hi1, Ts... ds)
{
  ਵਾਪਸ hb_ਵਿੱਚ_range<T> (u, lo1, hi1) || hb_ਵਿੱਚ_ranges<T> (u, ds...);
}


/*
 * Overflow checking.
 */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
hb_unsigned_mul_overflows (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ, ਬਿਨਾਨਿਸ਼ਾਨੀ *ਨਤੀਜਾ = ਖਾਲੀ_ਸੂਚਕ)
{
#ਜੇ hb_has_builtin(__builtin_mul_overflow)
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਢੇਰ_ਨਤੀਜਾ;
  ਜੇ (!ਨਤੀਜਾ)
    ਨਤੀਜਾ = &ਢੇਰ_ਨਤੀਜਾ;
  ਵਾਪਸ __builtin_mul_overflow (ਗਿਣਤੀ_2, ਮਾਪ, ਨਤੀਜਾ);
#ਜੇ_ਅੰਤ

  ਜੇ (ਨਤੀਜਾ)
    *ਨਤੀਜਾ = ਗਿਣਤੀ_2 * ਮਾਪ;
  ਵਾਪਸ (ਮਾਪ > 0) && (ਗਿਣਤੀ_2 >= ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) -1) / ਮਾਪ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
hb_unsigned_ਜੋੜੋ_overflows (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ a, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ b, ਬਿਨਾਨਿਸ਼ਾਨੀ *ਨਤੀਜਾ = ਖਾਲੀ_ਸੂਚਕ)
{
#ਜੇ hb_has_builtin(__builtin_add_overflow)
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਢੇਰ_ਨਤੀਜਾ;
  ਜੇ (!ਨਤੀਜਾ)
    ਨਤੀਜਾ = &ਢੇਰ_ਨਤੀਜਾ;
  ਵਾਪਸ __builtin_add_overflow (a, b, ਨਤੀਜਾ);
#ਜੇ_ਅੰਤ

  ਜੇ (ਨਤੀਜਾ)
    *ਨਤੀਜਾ = a + b;
  ਵਾਪਸ b > (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) -1 - a;
}

/* Saturating arithmetic on size_t.  On platforms where size_t is wider than
 * unsigned int (i.e. 64-bit), the inputs to get_size()-style computations
 * (counts and static_sizes which are at most 32-bit) cannot overflow size_t,
 * so these reduce to plain arithmetic.  On 32-bit platforms (size_t is
 * unsigned int), they saturate to SIZE_MAX so sanitize/serialize callers
 * naturally reject the resulting size as out-of-range. */

ਸਥਿਰ ਸਤਰਅੰਦਰ size_t
hb_unsigned_mul_saturate (size_t a, size_t b)
{
  ਜੇ (ਆਕਾਰ (size_t) > ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ))
    ਵਾਪਸ a * b;
#ਜੇ hb_has_builtin(__builtin_mul_overflow)
  size_t ਨਤੀਜਾ;
  ਜੇ (__builtin_mul_overflow (a, b, &ਨਤੀਜਾ))
    ਵਾਪਸ (size_t) -1;
  ਵਾਪਸ ਨਤੀਜਾ;
#ਨਹੀਂਤਾਂ
  ਜੇ (b > 0 && a > ((size_t) -1) / b) ਵਾਪਸ (size_t) -1;
  ਵਾਪਸ a * b;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ size_t
hb_unsigned_ਜੋੜੋ_saturate (size_t a, size_t b)
{
  ਜੇ (ਆਕਾਰ (size_t) > ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ))
    ਵਾਪਸ a + b;
#ਜੇ hb_has_builtin(__builtin_add_overflow)
  size_t ਨਤੀਜਾ;
  ਜੇ (__builtin_add_overflow (a, b, &ਨਤੀਜਾ))
    ਵਾਪਸ (size_t) -1;
  ਵਾਪਸ ਨਤੀਜਾ;
#ਨਹੀਂਤਾਂ
  ਜੇ (b > ((size_t) -1) - a) ਵਾਪਸ (size_t) -1;
  ਵਾਪਸ a + b;
#ਜੇ_ਅੰਤ
}

/* Variadic forms: fold left across all arguments. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ...Ts>
ਸਥਿਰ ਸਤਰਅੰਦਰ size_t
hb_unsigned_mul_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ ਵਾਪਸ hb_unsigned_mul_saturate (hb_unsigned_mul_saturate (a, b), c, rest...); }

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ...Ts>
ਸਥਿਰ ਸਤਰਅੰਦਰ size_t
hb_unsigned_ਜੋੜੋ_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ ਵਾਪਸ hb_unsigned_ਜੋੜੋ_saturate (hb_unsigned_ਜੋੜੋ_saturate (a, b), c, rest...); }

/* Saturating mul-add: a * b + c.  Covers the dominant get_size() pattern
 * `count * static_size + min_size`. */
ਸਥਿਰ ਸਤਰਅੰਦਰ size_t
hb_unsigned_mul_ਜੋੜੋ_saturate (size_t a, size_t b, size_t c)
{ ਵਾਪਸ hb_unsigned_ਜੋੜੋ_saturate (hb_unsigned_mul_saturate (a, b), c); }


/*
 * Sort and search.
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ K, ਕਿਸਮ_ਨਾਂ V, ਕਿਸਮ_ਨਾਂ ...Ts>
ਸਥਿਰ ਪੂਰਨ
hb_cmp_ਵਿਧੀ (ਅਚਲ ਖਾਲੀ *pkey, ਅਚਲ ਖਾਲੀ *pval, Ts... ds)
{
  ਅਚਲ K& key = * (ਅਚਲ K*) pkey;
  ਅਚਲ V& val = * (ਅਚਲ V*) pval;

  ਵਾਪਸ val.cmp (key, ds...);
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ K, ਕਿਸਮ_ਨਾਂ V>
ਸਥਿਰ ਪੂਰਨ
_hb_cmp_operator (ਅਚਲ ਖਾਲੀ *pkey, ਅਚਲ ਖਾਲੀ *pval)
{
  ਅਚਲ K& key = * (ਅਚਲ K*) pkey;
  ਅਚਲ V& val = * (ਅਚਲ V*) pval;

  ਜੇ (key < val) ਵਾਪਸ -1;
  ਜੇ (key > val) ਵਾਪਸ  1;
  ਵਾਪਸ 0;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ V, ਕਿਸਮ_ਨਾਂ K, ਕਿਸਮ_ਨਾਂ ...Ts>
HB_HOT
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
hb_bsearch_impl (ਬਿਨਾਨਿਸ਼ਾਨੀ *pos, /* Out */
		 ਅਚਲ K& key,
		 V* ਅਧਾਰ, size_t nmemb, size_t stride,
		 ਪੂਰਨ (*compar)(ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ_3, ਅਚਲ ਖਾਲੀ *ਵਸਤੂ_2, Ts... _ds),
		 Ts... ds)
{
  /* This is our *only* bsearch implementation. */

  ਪੂਰਨ min = 0, max = (ਪੂਰਨ) nmemb - 1;
  ਜਦੋਂ (min <= max)
  {
    ਪੂਰਨ mid = ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) min + (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) max) / 2;
#ਹਦਾਇਤ GCC diagnostic push
#ਹਦਾਇਤ GCC diagnostic ignored "-Wcast-align"
    V* p = (V*) (((ਅਚਲ ਅੱਖਰ *) ਅਧਾਰ) + (mid * stride));
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
    ਪੂਰਨ c = compar ((ਅਚਲ ਖਾਲੀ *) std::addressof (key), (ਅਚਲ ਖਾਲੀ *) p, ds...);
    ਜੇ (c < 0)
      max = mid - 1;
    ਨਹੀਂਤਾਂ ਜੇ (c > 0)
      min = mid + 1;
    ਨਹੀਂਤਾਂ
    {
      *pos = mid;
      ਵਾਪਸ ਸੱਚ;
    }
  }
  *pos = min;
  ਵਾਪਸ ਝੂਠ;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ V, ਕਿਸਮ_ਨਾਂ K>
ਸਥਿਰ ਸਤਰਅੰਦਰ V*
hb_bsearch (ਅਚਲ K& key, V* ਅਧਾਰ,
	    size_t nmemb, size_t stride = ਆਕਾਰ (V),
	    ਪੂਰਨ (*compar)(ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ_3, ਅਚਲ ਖਾਲੀ *ਵਸਤੂ_2) = hb_cmp_ਵਿਧੀ<K, V>)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ pos;
#ਹਦਾਇਤ GCC diagnostic push
#ਹਦਾਇਤ GCC diagnostic ignored "-Wcast-align"
  ਵਾਪਸ hb_bsearch_impl (&pos, key, ਅਧਾਰ, nmemb, stride, compar) ?
	 (V*) (((ਅਚਲ ਅੱਖਰ *) ਅਧਾਰ) + (pos * stride)) : ਖਾਲੀ_ਸੂਚਕ;
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ V, ਕਿਸਮ_ਨਾਂ K, ਕਿਸਮ_ਨਾਂ ...Ts>
ਸਥਿਰ ਸਤਰਅੰਦਰ V*
hb_bsearch (ਅਚਲ K& key, V* ਅਧਾਰ,
	    size_t nmemb, size_t stride,
	    ਪੂਰਨ (*compar)(ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ_3, ਅਚਲ ਖਾਲੀ *ਵਸਤੂ_2, Ts... _ds),
	    Ts... ds)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ pos;
#ਹਦਾਇਤ GCC diagnostic push
#ਹਦਾਇਤ GCC diagnostic ignored "-Wcast-align"
  ਵਾਪਸ hb_bsearch_impl (&pos, key, ਅਧਾਰ, nmemb, stride, compar, ds...) ?
	 (V*) (((ਅਚਲ ਅੱਖਰ *) ਅਧਾਰ) + (pos * stride)) : ਖਾਲੀ_ਸੂਚਕ;
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
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
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ Compar>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_qsort_loop (T *ਅਧਾਰ, size_t nel, Compar compar)
{
  ਜਦੋਂ (nel > 24)
  {
    T *ਆਖਰੀ = ਅਧਾਰ + nel - 1;
    T *mid = ਅਧਾਰ + nel / 2;

    /* Median-of-three pivot, parked at last-1. */
    ਜੇ (compar (*ਅਧਾਰ, *mid) > 0) hb_ਅਦਲਾ_ਬਦਲੀ (*ਅਧਾਰ, *mid);
    ਜੇ (compar (*mid, *ਆਖਰੀ) > 0)
    {
      hb_ਅਦਲਾ_ਬਦਲੀ (*mid, *ਆਖਰੀ);
      ਜੇ (compar (*ਅਧਾਰ, *mid) > 0) hb_ਅਦਲਾ_ਬਦਲੀ (*ਅਧਾਰ, *mid);
    }
    hb_ਅਦਲਾ_ਬਦਲੀ (*mid, *(ਆਖਰੀ - 1));
    T &ਧੁਰਾ = *(ਆਖਰੀ - 1);

    /* Two-way Hoare partition.  Inner loops are unguarded:
     * median-of-three left *base <= pivot and *last >= pivot,
     * which act as sentinels. */
    T *i = ਅਧਾਰ, *j = ਆਖਰੀ - 1;
    ਜਦੋਂ (ਸੱਚ)
    {
      ਜਦੋਂ (compar (*++i, ਧੁਰਾ) < 0) {}
      ਜਦੋਂ (compar (*--j, ਧੁਰਾ) > 0) {}
      ਜੇ (i >= j) ਰੋਕੋ;
      hb_ਅਦਲਾ_ਬਦਲੀ (*i, *j);
    }
    hb_ਅਦਲਾ_ਬਦਲੀ (*i, *(ਆਖਰੀ - 1));

    /* Recurse on smaller side, loop on larger — bounds stack
     * depth at O(log n). */
    size_t ਖੱਬੇ  = (size_t) (i - ਅਧਾਰ);
    size_t ਸੱਜੇ = nel - ਖੱਬੇ - 1;
    ਜੇ (ਖੱਬੇ < ਸੱਜੇ)
    {
      hb_qsort_loop (ਅਧਾਰ, ਖੱਬੇ, compar);
      ਅਧਾਰ = i + 1;
      nel  = ਸੱਜੇ;
    }
    ਨਹੀਂਤਾਂ
    {
      hb_qsort_loop (i + 1, ਸੱਜੇ, compar);
      nel  = ਖੱਬੇ;
    }
  }
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ Compar>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_qsort_inline (T *ਅਧਾਰ, size_t nel, Compar compar)
{
  hb_qsort_loop (ਅਧਾਰ, nel, compar);

  /* Single final insertion sort over the whole array.  After
   * the partitioning loop, every element is within the threshold
   * of its sorted position, so this pass is O(n * threshold). */
  T *ਸਿਰਾ = ਅਧਾਰ + nel;
  ਲਈ (T *pi = ਅਧਾਰ + 1; pi < ਸਿਰਾ; pi++)
    ਲਈ (T *pj = pi; pj > ਅਧਾਰ && compar (pj[-1], pj[0]) > 0; pj--)
      hb_ਅਦਲਾ_ਬਦਲੀ (pj[-1], pj[0]);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_qsort (ਖਾਲੀ *ਅਧਾਰ, size_t nel, size_t ਚੌੜਾਈ,
	  ਪੂਰਨ (*compar)(ਅਚਲ ਖਾਲੀ *_a, ਅਚਲ ਖਾਲੀ *_b))
{
  qsort (ਅਧਾਰ, nel, ਚੌੜਾਈ, compar);
}




ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ T2, ਕਿਸਮ_ਨਾਂ T3 = ਪੂਰਨ> ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_stable_ਛਾਂਟੋ (T *array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len, ਪੂਰਨ(*compar)(ਅਚਲ T2 *, ਅਚਲ T2 *), T3 *array2 = ਖਾਲੀ_ਸੂਚਕ)
{
  static_assert (hb_is_trivially_copy_assignable (T), "");
  static_assert (hb_is_trivially_copy_assignable (T3), "");

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < len; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = i;
    ਜਦੋਂ (j && compar (&array[j - 1], &array[i]) > 0)
      j--;
    ਜੇ (i == j)
      ਜਾਰੀ;
    /* Move item i to occupy place for item j, shift what's in between. */
    {
      T t = array[i];
      memmove (&array[j + 1], &array[j], (i - j) * ਆਕਾਰ (T));
      array[j] = t;
    }
    ਜੇ (array2)
    {
      T3 t = array2[i];
      memmove (&array2[j + 1], &array2[j], (i - j) * ਆਕਾਰ (T3));
      array2[j] = t;
    }
  }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ hb_bool_t
hb_codepoint_parse (ਅਚਲ ਅੱਖਰ *s, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len, ਪੂਰਨ ਅਧਾਰ, hb_codepoint_t *ਬਾਹਰਲਾ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ v;
  ਅਚਲ ਅੱਖਰ *p = s;
  ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = p + len;
  ਜੇ (unlikely (!hb_parse_uint (&p, ਸਿਰਾ, &v, ਸੱਚ/* whole buffer */, ਅਧਾਰ)))
    ਵਾਪਸ ਝੂਠ;

  *ਬਾਹਰਲਾ = v;
  ਵਾਪਸ ਸੱਚ;
}


/* Operators. */

ਬਣਤਰ
{ HB_PARTIALIZE(2);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ T &a, ਅਚਲ T &b) ਅਚਲ HB_AUTO_RETURN (a & b)
}
HB_FUNCOBJ (hb_bitwise_and);
ਬਣਤਰ
{ HB_PARTIALIZE(2);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ T &a, ਅਚਲ T &b) ਅਚਲ HB_AUTO_RETURN (a | b)
}
HB_FUNCOBJ (hb_bitwise_or);
ਬਣਤਰ
{ HB_PARTIALIZE(2);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ T &a, ਅਚਲ T &b) ਅਚਲ HB_AUTO_RETURN (a ^ b)
}
HB_FUNCOBJ (hb_bitwise_xor);
ਬਣਤਰ
{ HB_PARTIALIZE(2);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ T &a, ਅਚਲ T &b) ਅਚਲ HB_AUTO_RETURN (~a & b)
}
HB_FUNCOBJ (hb_bitwise_lt);
ਬਣਤਰ
{ HB_PARTIALIZE(2);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ T &a, ਅਚਲ T &b) ਅਚਲ HB_AUTO_RETURN (a & ~b)
}
HB_FUNCOBJ (hb_bitwise_gt); // aka sub
ਬਣਤਰ
{ HB_PARTIALIZE(2);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ T &a, ਅਚਲ T &b) ਅਚਲ HB_AUTO_RETURN (~a | b)
}
HB_FUNCOBJ (hb_bitwise_le);
ਬਣਤਰ
{ HB_PARTIALIZE(2);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ T &a, ਅਚਲ T &b) ਅਚਲ HB_AUTO_RETURN (a | ~b)
}
HB_FUNCOBJ (hb_bitwise_ge);
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ T &a) ਅਚਲ HB_AUTO_RETURN (~a)
}
HB_FUNCOBJ (hb_bitwise_neg);

ਬਣਤਰ
{ HB_PARTIALIZE(2);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ T2> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ T &a, ਅਚਲ T2 &b) ਅਚਲ HB_AUTO_RETURN (a + b)
}
HB_FUNCOBJ (hb_ਜੋੜੋ);
ਬਣਤਰ
{ HB_PARTIALIZE(2);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ T2> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ T &a, ਅਚਲ T2 &b) ਅਚਲ HB_AUTO_RETURN (a - b)
}
HB_FUNCOBJ (hb_sub);
ਬਣਤਰ
{ HB_PARTIALIZE(2);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ T2> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ T &a, ਅਚਲ T2 &b) ਅਚਲ HB_AUTO_RETURN (b - a)
}
HB_FUNCOBJ (hb_rsub);
ਬਣਤਰ
{ HB_PARTIALIZE(2);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ T2> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ T &a, ਅਚਲ T2 &b) ਅਚਲ HB_AUTO_RETURN (a * b)
}
HB_FUNCOBJ (hb_mul);
ਬਣਤਰ
{ HB_PARTIALIZE(2);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ T2> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ T &a, ਅਚਲ T2 &b) ਅਚਲ HB_AUTO_RETURN (a / b)
}
HB_FUNCOBJ (hb_div);
ਬਣਤਰ
{ HB_PARTIALIZE(2);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ T2> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ T &a, ਅਚਲ T2 &b) ਅਚਲ HB_AUTO_RETURN (a % b)
}
HB_FUNCOBJ (hb_mod);
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ T &a) ਅਚਲ HB_AUTO_RETURN (+a)
}
HB_FUNCOBJ (hb_pos);
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ T &a) ਅਚਲ HB_AUTO_RETURN (-a)
}
HB_FUNCOBJ (hb_neg);
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (T &a) ਅਚਲ HB_AUTO_RETURN (++a)
}
HB_FUNCOBJ (hb_inc);
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (T &a) ਅਚਲ HB_AUTO_RETURN (--a)
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
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ func_t>
ਦੁੱਗਣਾ solve_itp (func_t f,
		  ਦੁੱਗਣਾ a, ਦੁੱਗਣਾ b,
		  ਦੁੱਗਣਾ epsilon,
		  ਦੁੱਗਣਾ min_y, ਦੁੱਗਣਾ max_y,
		  ਦੁੱਗਣਾ &ya, ਦੁੱਗਣਾ &yb, ਦੁੱਗਣਾ &y)
{
  // Guard against degenerate interval
  ਜੇ (b - a <= 0.0)
  {
    y = ya;
    ਵਾਪਸ a;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ n1_2 = (ਬਿਨਾਨਿਸ਼ਾਨੀ) (hb_max (ceil (log2 ((b - a) / epsilon)) - 1.0, 0.0));
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ n0 = 1; // Hardwired
  ਅਚਲ ਦੁੱਗਣਾ k1 = 0.2 / (b - a); // Hardwired.
  ਬਿਨਾਨਿਸ਼ਾਨੀ nmax = n0 + n1_2;
  ਦੁੱਗਣਾ scaled_epsilon = epsilon * ਦੁੱਗਣਾ (1llu << nmax);
  ਦੁੱਗਣਾ _2_epsilon = 2.0 * epsilon;
  ਜਦੋਂ (b - a > _2_epsilon)
  {
    ਦੁੱਗਣਾ x1_2 = 0.5 * (a + b);
    ਦੁੱਗਣਾ r = scaled_epsilon - 0.5 * (b - a);
    // Guard against yb == ya to prevent division by zero
    ਦੁੱਗਣਾ xf = (yb != ya) ? (yb * a - ya * b) / (yb - ya) : x1_2;
    ਦੁੱਗਣਾ sigma = x1_2 - xf;
    ਦੁੱਗਣਾ b_a = b - a;
    // This has k2 = 2 hardwired for efficiency.
    ਦੁੱਗਣਾ b_a_k2 = b_a * b_a;
    ਦੁੱਗਣਾ delta = k1 * b_a_k2;
    ਪੂਰਨ sigma_sign = sigma >= 0 ? +1 : -1;
    ਦੁੱਗਣਾ xt = delta <= fabs (x1_2 - xf) ? xf + delta * sigma_sign : x1_2;
    ਦੁੱਗਣਾ xitp = fabs (xt - x1_2) <= r ? xt : x1_2 - r * sigma_sign;
    ਦੁੱਗਣਾ yitp = f (xitp);
    ਜੇ (yitp > max_y)
    {
      b = xitp;
      yb = yitp;
    }
    ਨਹੀਂਤਾਂ ਜੇ (yitp < min_y)
    {
      a = xitp;
      ya = yitp;
    }
    ਨਹੀਂਤਾਂ
    {
      y = yitp;
      ਵਾਪਸ xitp;
    }
    scaled_epsilon *= 0.5;
  }
  ਵਾਪਸ 0.5 * (a + b);
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
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ F>
ਬਣਤਰ hb_scope_guard_t
{
  ਸਪਸ਼ਟ hb_scope_guard_t (F &&f) : f (std::move (f)), ਸਰਗਰਮ (ਸੱਚ) {}
  hb_scope_guard_t (hb_scope_guard_t &&o) noexcept
    : f (std::move (o.f)), ਸਰਗਰਮ (o.ਸਰਗਰਮ) { o.ਸਰਗਰਮ = ਝੂਠ; }
  hb_scope_guard_t (ਅਚਲ hb_scope_guard_t &) = ਮਿਟਾਓ;
  hb_scope_guard_t &ਸੰਚਾਲਕ= (ਅਚਲ hb_scope_guard_t &) = ਮਿਟਾਓ;
  hb_scope_guard_t &ਸੰਚਾਲਕ= (hb_scope_guard_t &&) = ਮਿਟਾਓ;
  ~hb_scope_guard_t () { ਜੇ (ਸਰਗਰਮ) f (); }

  /* Release: dismiss the guard so the cleanup does NOT run.  Use
   * when transferring ownership out of the scope. */
  ਖਾਲੀ release () { ਸਰਗਰਮ = ਝੂਠ; }

  ਨਿਜੀ:
  F f;
  ਤਾਰਕਿਕ ਸਰਗਰਮ;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ F>
ਸਥਿਰ ਸਤਰਅੰਦਰ hb_scope_guard_t<F> hb_make_scope_guard (F &&f)
{ ਵਾਪਸ hb_scope_guard_t<F> (std::forward<F> (f)); }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SCOPE_GUARD_NAME_(ਸਤਰ) hb_scope_guard_##ਸਤਰ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SCOPE_GUARD_NAME(ਸਤਰ) HB_SCOPE_GUARD_NAME_(ਸਤਰ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SCOPE_GUARD(stmt) \
  ਸਵੈਚਾਲਿਤ HB_SCOPE_GUARD_NAME(__LINE__) = \
    hb_make_scope_guard ([&]() { stmt; }); \
  (ਖਾਲੀ) HB_SCOPE_GUARD_NAME(__LINE__)


#ਜੇ_ਅੰਤ /* HB_ALGS_HH */
