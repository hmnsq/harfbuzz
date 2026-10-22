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

#اگر_غیر_تعریف_شدہ HB_ALGS_HH
#تعریف_کرو HB_ALGS_HH

#شامل "hb.hh"
#شامل "hb-meta.hh"
#شامل "hb-null.hh"
#شامل "hb-number.hh"

#شامل <algorithm>
#شامل <initializer_list>
#شامل <functional>
#شامل <new>

ساکن HB_ALWAYS_INLINE خالی *
hb_malloc2 (size_t nmemb, size_t سائز)
{
  اگر (سائز && nmemb > SIZE_MAX / سائز) واپس خالی_اشاریہ;
  واپس hb_malloc (nmemb * سائز);
}
ساکن HB_ALWAYS_INLINE خالی *
hb_realloc2 (خالی *ptr, size_t nmemb, size_t سائز)
{
  اگر (سائز && nmemb > SIZE_MAX / سائز) واپس خالی_اشاریہ;
  واپس hb_realloc (ptr, nmemb * سائز);
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
#اگر_تعریف_شدہ _MSC_VER
# pragma warning(disable:4200)
# pragma warning(disable:4800)
#اگر_ختم
#تعریف_کرو HB_MARK_AS_FLAG_T(T) \
	خارجی "C++" { \
	  ساکن اندرونی constexpr T عامل | (T l, T r) { واپس T ((غیرعلامتی) l | (غیرعلامتی) r); } \
	  ساکن اندرونی constexpr T عامل & (T l, T r) { واپس T ((غیرعلامتی) l & (غیرعلامتی) r); } \
	  ساکن اندرونی constexpr T عامل ^ (T l, T r) { واپس T ((غیرعلامتی) l ^ (غیرعلامتی) r); } \
	  ساکن اندرونی constexpr غیرعلامتی عامل ~ (T r) { واپس (~(غیرعلامتی) r); } \
	  ساکن اندرونی T& عامل |= (T &l, T r) { l = l | r; واپس l; } \
	  ساکن اندرونی T& عامل &= (T& l, T r) { l = l & r; واپس l; } \
	  ساکن اندرونی T& عامل ^= (T& l, T r) { l = l ^ r; واپس l; } \
	} \
	static_assert (سچ, "")

/* Useful for set-operations on small enums.
 * For example, for testing "x ∈ {x1, x2, x3}" use:
 * (FLAG_UNSAFE(x) & (FLAG(x1) | FLAG(x2) | FLAG(x3)))
 */
#تعریف_کرو FLAG(x) (static_assert_اظہاریہ ((غیرعلامتی)(x) < 32) + (((uint32_t) 1U) << (غیرعلامتی)(x)))
#تعریف_کرو FLAG_UNSAFE(x) ((غیرعلامتی)(x) < 32 ? (((uint32_t) 1U) << (غیرعلامتی)(x)) : 0)
#تعریف_کرو FLAG_RANGE(x,y) (static_assert_اظہاریہ ((x) < (y)) + FLAG(y+1) - FLAG(x))
#تعریف_کرو FLAG64(x) (static_assert_اظہاریہ ((غیرعلامتی)(x) < 64) + (((uint64_t) 1ULL) << (غیرعلامتی)(x)))
#تعریف_کرو FLAG64_UNSAFE(x) ((غیرعلامتی)(x) < 64 ? (((uint64_t) 1ULL) << (غیرعلامتی)(x)) : 0)


/*
 * Fixed-endian integers / floats.
 */


/* Endian swap, used in Windows related backends */
ساکن اندرونی constexpr uint16_t hb_uint16_تبادلہ (uint16_t v)
{ واپس (v >> 8) | (v << 8); }
ساکن اندرونی constexpr uint32_t hb_uint32_تبادلہ (uint32_t v)
{ واپس (hb_uint16_تبادلہ (v) << 16) | hb_uint16_تبادلہ (v >> 16); }

سانچہ <قسم_نام Type>
ساخت __attribute__((packed)) hb_packed_t
{
  hb_packed_t () = طےشدہ;
  constexpr hb_packed_t (Type V) : v (V) {}
  عامل Type () ثابت { واپس v; }
  hb_packed_t & عامل = (Type V) { v = V; واپس *یہ; }

  نجی:
  Type v;
};

#اگر_غیر_تعریف_شدہ HB_FAST_عدد_ACCESS

#اگر defined(__OPTIMIZE__) && \
    defined(__BYTE_ORDER) && \
    (__BYTE_ORDER == __BIG_ENDIAN || \
     (__BYTE_ORDER == __LITTLE_ENDIAN && \
      hb_has_builtin(__builtin_bswap16) && \
      hb_has_builtin(__builtin_bswap32) && \
      hb_has_builtin(__builtin_bswap64)))
#تعریف_کرو HB_FAST_عدد_ACCESS 1
#ورنہ
#تعریف_کرو HB_FAST_عدد_ACCESS 0
#اگر_ختم

// https://github.com/harfbuzz/harfbuzz/issues/5456
#اگر defined(__GNUC__) && !defined(__clang__) && (__GNUC__ <= 12)
#تعریف_منسوخ HB_FAST_عدد_ACCESS
#تعریف_کرو HB_FAST_عدد_ACCESS 0
#اگر_ختم

#اگر_ختم

سانچہ <منطقی BE, قسم_نام Type, صحیح بائٹیں_3 = حجم (Type)>
ساخت HBInt;
سانچہ <منطقی BE, قسم_نام Type>
ساخت HBInt<BE, Type, 1>
{
  عوامی:
  HBInt () = طےشدہ;
  constexpr HBInt (Type V) : v {uint8_t (V)} {}
  constexpr عامل Type () ثابت { واپس v; }
  نجی: uint8_t v;
};
سانچہ <منطقی BE, قسم_نام Type>
ساخت HBInt<BE, Type, 2>
{
  عوامی:
  HBInt () = طےشدہ;

  HBInt (Type V)
#اگر HB_FAST_عدد_ACCESS
  {
    اگر (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint16_t> *) v) = V;
    ورنہ
      *((hb_packed_t<uint16_t> *) v) = __builtin_bswap16 (V);
  }
#ورنہ
    : v {BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >>  8) & 0xFF)} {}
#اگر_ختم

  constexpr عامل Type () ثابت
  {
#اگر HB_FAST_عدد_ACCESS
    واپس (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint16_t) *((ثابت hb_packed_t<uint16_t> *) v)
    :
      __builtin_bswap16 ((uint16_t) *((ثابت hb_packed_t<uint16_t> *) v))
    ;
#ورنہ
    واپس (BE ? (v[0] <<  8) : (v[0]      ))
	 + (BE ? (v[1]      ) : (v[1] <<  8));
#اگر_ختم
  }
  نجی: uint8_t v[2];
};
سانچہ <منطقی BE, قسم_نام Type>
ساخت HBInt<BE, Type, 3>
{
  static_assert (!std::is_signed<Type>::value, "");
  عوامی:
  HBInt () = طےشدہ;
  constexpr HBInt (Type V) : v {BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
				BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
				BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V      ) & 0xFF)} {}

  constexpr عامل Type () ثابت { واپس (BE ? (v[0] << 16) : (v[0]      ))
					  + (BE ? (v[1] <<  8) : (v[1] <<  8))
					  + (BE ? (v[2]      ) : (v[2] << 16)); }
  نجی: uint8_t v[3];
};
سانچہ <منطقی BE, قسم_نام Type>
ساخت HBInt<BE, Type, 4>
{
  سانچہ <منطقی, قسم_نام, صحیح>
  دوست ساخت HBFloat;

  عوامی:
  HBInt () = طےشدہ;

  HBInt (Type V)
#اگر HB_FAST_عدد_ACCESS
  {
    اگر (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint32_t> *) v) = V;
    ورنہ
      *((hb_packed_t<uint32_t> *) v) = __builtin_bswap32 (V);
  }
#ورنہ
    : v {BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 24) & 0xFF)} {}
#اگر_ختم

  constexpr عامل Type () ثابت {
#اگر HB_FAST_عدد_ACCESS
    واپس (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint32_t) *((ثابت hb_packed_t<uint32_t> *) v)
    :
      __builtin_bswap32 ((uint32_t) *((ثابت hb_packed_t<uint32_t> *) v))
    ;
#ورنہ
    واپس (BE ? (uint32_t (v[0]) << 24) : (uint32_t (v[0])      ))
	 + (BE ? (uint32_t (v[1]) << 16) : (uint32_t (v[1]) <<  8))
	 + (BE ? (uint32_t (v[2]) <<  8) : (uint32_t (v[2]) << 16))
	 + (BE ? (uint32_t (v[3])      ) : (uint32_t (v[3]) << 24));
#اگر_ختم
  }
  نجی: uint8_t v[4];
};
سانچہ <منطقی BE, قسم_نام Type>
ساخت HBInt<BE, Type, 8>
{
  سانچہ <منطقی, قسم_نام, صحیح>
  دوست ساخت HBFloat;

  عوامی:
  HBInt () = طےشدہ;

  HBInt (Type V)
#اگر HB_FAST_عدد_ACCESS
  {
    اگر (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint64_t> *) v) = V;
    ورنہ
      *((hb_packed_t<uint64_t> *) v) = __builtin_bswap64 (V);
  }
#ورنہ
    : v {BE ? uint8_t ((V >> 56) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 48) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >> 40) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V >> 32) & 0xFF) : uint8_t ((V >> 24) & 0xFF),
	 BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V >> 32) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 40) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 48) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 56) & 0xFF)} {}
#اگر_ختم

  constexpr عامل Type () ثابت {
#اگر HB_FAST_عدد_ACCESS
    واپس (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint64_t) *((ثابت hb_packed_t<uint64_t> *) v)
    :
      __builtin_bswap64 ((uint64_t) *((ثابت hb_packed_t<uint64_t> *) v))
    ;
#ورنہ
    واپس (BE ? (uint64_t (v[0]) << 56) : (uint64_t (v[0])      ))
	 + (BE ? (uint64_t (v[1]) << 48) : (uint64_t (v[1]) <<  8))
	 + (BE ? (uint64_t (v[2]) << 40) : (uint64_t (v[2]) << 16))
	 + (BE ? (uint64_t (v[3]) << 32) : (uint64_t (v[3]) << 24))
	 + (BE ? (uint64_t (v[4]) << 24) : (uint64_t (v[4]) << 32))
	 + (BE ? (uint64_t (v[5]) << 16) : (uint64_t (v[5]) << 40))
	 + (BE ? (uint64_t (v[6]) <<  8) : (uint64_t (v[6]) << 48))
	 + (BE ? (uint64_t (v[7])      ) : (uint64_t (v[7]) << 56));
#اگر_ختم
  }
  نجی: uint8_t v[8];
};

/* Floats. */

سانچہ <منطقی BE, قسم_نام Type, صحیح بائٹیں_3>
ساخت HBFloat
{
  استعمال IntType = قسم_نام std::conditional<بائٹیں_3 == 4, uint32_t, uint64_t>::type;

  عوامی:
  HBFloat () = طےشدہ;

  HBFloat (Type V)
  {
#اگر HB_FAST_عدد_ACCESS
    {
	      اگر (BE == (__BYTE_ORDER == __BIG_ENDIAN))
	      {
	        *((hb_packed_t<Type> *) v) = V;
	        واپس;
	      }
	    }
#اگر_ختم

    اتحاد {
      hb_packed_t<Type> f;
      hb_packed_t<IntType> i;
    } u = {{V}};

    ثابت HBInt<BE, IntType> I = (IntType) u.i;
    برائے (غیرعلامتی i = 0; i < بائٹیں_3; i++)
      v[i] = I.v[i];
  }

  /* c++14 constexpr */ عامل Type () ثابت
  {
#اگر HB_FAST_عدد_ACCESS
	    {
	      اگر (BE == (__BYTE_ORDER == __BIG_ENDIAN))
		واپس (Type) *((ثابت hb_packed_t<Type> *) v);
	    }
#اگر_ختم

    HBInt<BE, IntType> I;
    برائے (غیرعلامتی i = 0; i < بائٹیں_3; i++)
      I.v[i] = v[i];

    اتحاد {
      hb_packed_t<IntType> i;
      hb_packed_t<Type> f;
    } u = {{I}};

    واپس (Type) u.f;
  }
  نجی: uint8_t v[بائٹیں_3];
};


/* We want our rounding towards +infinity. */
ساکن اندرونی دوہرا
_hb_roundf (دوہرا x) { واپس floor (x + .5); }

ساکن اندرونی اعشاری
_hb_roundf (اعشاری x) { واپس floor ((دوہرا) x + .5); }

#تعریف_کرو roundf(x) _hb_roundf(x)

ساکن اندرونی خالی
hb_sincos (اعشاری rotation, اعشاری &s, اعشاری &c)
{
#اگر_تعریف_شدہ HAVE_SINCOSF
  sincosf (rotation, &s, &c);
#ورنہ
  c = cosf (rotation);
  s = sinf (rotation);
#اگر_ختم
}
ساکن اندرونی خالی
hb_sincos (دوہرا rotation, دوہرا &s, دوہرا &c)
{
#اگر_تعریف_شدہ HAVE_SINCOS
  sincos (rotation, &s, &c);
#ورنہ
  c = جیب_تمام (rotation);
  s = جیب (rotation);
#اگر_ختم
}


/* Encodes three unsigned integers in one 64-bit number.  If the inputs have more than 21 bits,
 * values will be truncated / overlap, and might not decode exactly. */
#تعریف_کرو HB_CODEPOINT_ENCODE3(x,y,z) (((uint64_t) (x) << 42) | ((uint64_t) (y) << 21) | (uint64_t) (z))
#تعریف_کرو HB_CODEPOINT_DECODE3_1(v) ((hb_codepoint_t) ((v) >> 42))
#تعریف_کرو HB_CODEPOINT_DECODE3_2(v) ((hb_codepoint_t) ((v) >> 21) & 0x1FFFFFu)
#تعریف_کرو HB_CODEPOINT_DECODE3_3(v) ((hb_codepoint_t) (v) & 0x1FFFFFu)

/* Custom encoding used by hb-ucd. */
#تعریف_کرو HB_CODEPOINT_ENCODE3_11_7_14(x,y,z) (((uint32_t) ((x) & 0x07FFu) << 21) | (((uint32_t) (y) & 0x007Fu) << 14) | (uint32_t) ((z) & 0x3FFFu))
#تعریف_کرو HB_CODEPOINT_DECODE3_11_7_14_1(v) ((hb_codepoint_t) ((v) >> 21))
#تعریف_کرو HB_CODEPOINT_DECODE3_11_7_14_2(v) ((hb_codepoint_t) (((v) >> 14) & 0x007Fu) | 0x0300)
#تعریف_کرو HB_CODEPOINT_DECODE3_11_7_14_3(v) ((hb_codepoint_t) (v) & 0x3FFFu)


ساخت
{
  /* Note.  This is dangerous in that if it's passed an rvalue, it returns rvalue-reference. */
  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (T&& v) ثابت HB_AUTO_RETURN ( std::forward<T> (v) )
}
HB_FUNCOBJ (hb_شناختی_عنصر);
ساخت
{
  /* Like identity(), but only retains lvalue-references.  Rvalues are returned as rvalues. */
  سانچہ <قسم_نام T> constexpr T&
  عامل () (T& v) ثابت { واپس v; }

  سانچہ <قسم_نام T> constexpr hb_remove_reference<T>
  عامل () (T&& v) ثابت { واپس v; }
}
HB_FUNCOBJ (hb_lidentity);
ساخت
{
  /* Like identity(), but always returns rvalue. */
  سانچہ <قسم_نام T> constexpr hb_remove_reference<T>
  عامل () (T&& v) ثابت { واپس v; }
}
HB_FUNCOBJ (hb_ridentity);

ساخت
{
  سانچہ <قسم_نام T> constexpr منطقی
  عامل () (T&& v) ثابت { واپس منطقی (std::forward<T> (v)); }
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
#تعریف_کرو fasthash_mix(h) (					\
			(خالی) ((h) ^= (h) >> 23),		\
			(خالی) ((h) *= 0x2127599bf4325c37ULL),	\
			(h) ^= (h) >> 47)

ساکن اندرونی uint64_t fasthash64(ثابت خالی *buf, size_t len, uint64_t بیج)
{
	ساخت __attribute__((packed)) packed_uint64_t { uint64_t v; };
	ثابت uint64_t    m = 0x880355f21e6d1965ULL;
	ثابت packed_uint64_t *pos = (ثابت packed_uint64_t *)buf;
	ثابت packed_uint64_t *انتہا = pos + (len / 8);
	ثابت غیرعلامتی حرف *pos2;
	uint64_t h = بیج ^ (len * m);
	uint64_t v;

#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
	اگر (((uintptr_t) pos & 7) == 0)
	{
	  جب (pos != انتہا)
	  {
#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wcast-align"
	    v  = * (ثابت uint64_t *) (pos++);
#ہدایت GCC diagnostic نکالو
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}
	ورنہ
#اگر_ختم
	{
	  جب (pos != انتہا)
	  {
	    v  = pos++->v;
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}

	pos2 = (ثابت غیرعلامتی حرف*)pos;
	v = 0;

	انتخاب (len & 7) {
	حالت 7: v ^= (uint64_t)pos2[6] << 48; HB_FALLTHROUGH;
	حالت 6: v ^= (uint64_t)pos2[5] << 40; HB_FALLTHROUGH;
	حالت 5: v ^= (uint64_t)pos2[4] << 32; HB_FALLTHROUGH;
	حالت 4: v ^= (uint64_t)pos2[3] << 24; HB_FALLTHROUGH;
	حالت 3: v ^= (uint64_t)pos2[2] << 16; HB_FALLTHROUGH;
	حالت 2: v ^= (uint64_t)pos2[1] <<  8; HB_FALLTHROUGH;
	حالت 1: v ^= (uint64_t)pos2[0];
		h ^= fasthash_mix(v);
		h *= m;
	}

	واپس fasthash_mix(h);
}

ساکن اندرونی uint32_t fasthash32(ثابت خالی *buf, size_t len, uint32_t بیج)
{
	// the following trick converts the 64-bit hashcode to Fermat
	// residue, which shall retain information from both the higher
	// and lower parts of hashcode.
        uint64_t h = fasthash64(buf, len, بیج);
	واپس h - (h >> 32);
}

ساخت
{
  نجی:

  سانچہ <قسم_نام T> constexpr خودکار
  impl (ثابت T& v, hb_ترجیح<2>) ثابت HB_RETURN (uint32_t, hb_deref (v).hash ())

  // Horrible: std:hash() of integers seems to be identity in gcc / clang?!
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  //
  // For performance characteristics see:
  // https://github.com/harfbuzz/harfbuzz/pull/4228#issuecomment-1565079537
  سانچہ <قسم_نام T,
	    hb_enable_if (std::is_integral<T>::value && حجم (T) <= حجم (uint32_t))> constexpr خودکار
  impl (ثابت T& v, hb_ترجیح<1>) ثابت HB_RETURN (uint32_t, (uint32_t) v * 2654435761u /* Knuh's multiplicative hash */)
  سانچہ <قسم_نام T,
	    hb_enable_if (std::is_integral<T>::value && حجم (T) > حجم (uint32_t))> constexpr خودکار
  impl (ثابت T& v, hb_ترجیح<1>) ثابت HB_RETURN (uint32_t, (uint32_t) (v ^ (v >> 32)) * 2654435761u /* Knuth's multiplicative hash */)

  سانچہ <قسم_نام T,
	    hb_enable_if (std::is_floating_point<T>::value)> constexpr خودکار
  impl (ثابت T& v, hb_ترجیح<1>) ثابت HB_RETURN (uint32_t, fasthash32 (std::addressof (v), حجم (T), 0xf437ffe6))

  سانچہ <قسم_نام T> constexpr خودکار
  impl (ثابت T& v, hb_ترجیح<0>) ثابت HB_RETURN (uint32_t, std::hash<hb_decay<decltype (hb_deref (v))>>{} (hb_deref (v)))

  عوامی:

  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (ثابت T& v) ثابت HB_RETURN (uint32_t, impl (v, hb_prioritize))
}
HB_FUNCOBJ (hb_hash);


ساخت
{
  نجی:

  /* Pointer-to-member-function. */
  سانچہ <قسم_نام Appl, قسم_نام T, قسم_نام ...Ts> خودکار
  impl (Appl&& a, hb_ترجیح<2>, T &&v, Ts&&... ds) ثابت HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v)).*std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  /* Pointer-to-member. */
  سانچہ <قسم_نام Appl, قسم_نام T> خودکار
  impl (Appl&& a, hb_ترجیح<1>, T &&v) ثابت HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v))).*std::forward<Appl> (a))

  /* Operator(). */
  سانچہ <قسم_نام Appl, قسم_نام ...Ts> خودکار
  impl (Appl&& a, hb_ترجیح<0>, Ts&&... ds) ثابت HB_AUTO_RETURN
  (hb_deref (std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  عوامی:

  سانچہ <قسم_نام Appl, قسم_نام ...Ts> خودکار
  عامل () (Appl&& a, Ts&&... ds) ثابت HB_AUTO_RETURN
  (
    impl (std::forward<Appl> (a),
	  hb_prioritize,
	  std::forward<Ts> (ds)...)
  )
}
HB_FUNCOBJ (hb_invoke);

سانچہ <غیرعلامتی Pos, قسم_نام Appl, قسم_نام V>
ساخت hb_partial_t
{
  hb_partial_t (Appl a, V v) : a (a), v (v) {}

  static_assert (Pos > 0, "");

  سانچہ <قسم_نام ...Ts,
	    غیرعلامتی P = Pos,
	    hb_enable_if (P == 1)> خودکار
  عامل () (Ts&& ...ds) -> decltype (hb_invoke (hb_declval (Appl),
						   hb_declval (V),
						   hb_declval (Ts)...))
  {
    واپس hb_invoke (std::forward<Appl> (a),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }
  سانچہ <قسم_نام T0, قسم_نام ...Ts,
	    غیرعلامتی P = Pos,
	    hb_enable_if (P == 2)> خودکار
  عامل () (T0&& d0, Ts&& ...ds) -> decltype (hb_invoke (hb_declval (Appl),
							    hb_declval (T0),
							    hb_declval (V),
							    hb_declval (Ts)...))
  {
    واپس hb_invoke (std::forward<Appl> (a),
		      std::forward<T0> (d0),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }

  نجی:
  hb_reference_wrapper<Appl> a;
  V v;
};
سانچہ <غیرعلامتی Pos=1, قسم_نام Appl, قسم_نام V>
خودکار hb_partial (Appl&& a, V&& v) HB_AUTO_RETURN
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
#اگر_تعریف_شدہ _MSC_VER
/* https://github.com/harfbuzz/harfbuzz/issues/1730 */ \
#تعریف_کرو HB_PARTIALIZE(Pos) \
  سانچہ <قسم_نام _T> \
  decltype(خودکار) عامل () (_T&& _v) ثابت \
  { واپس hb_partial<Pos> (یہ, std::forward<_T> (_v)); } \
  static_assert (سچ, "")
#ورنہ
/* https://github.com/harfbuzz/harfbuzz/issues/1724 */
#تعریف_کرو HB_PARTIALIZE(Pos) \
  سانچہ <قسم_نام _T> \
  خودکار عامل () (_T&& _v) ثابت HB_AUTO_RETURN \
  (hb_partial<Pos> (+یہ, std::forward<_T> (_v))) \
  static_assert (سچ, "")
#اگر_ختم


ساخت
{
  نجی:

  سانچہ <قسم_نام Pred, قسم_نام Val> خودکار
  impl (Pred&& p, Val &&v, hb_ترجیح<1>) ثابت HB_AUTO_RETURN
  (
    hb_deref (std::forward<Pred> (p)).has (std::forward<Val> (v))
  )

  سانچہ <قسم_نام Pred, قسم_نام Val> خودکار
  impl (Pred&& p, Val &&v, hb_ترجیح<0>) ثابت HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Pred> (p),
	       std::forward<Val> (v))
  )

  عوامی:

  سانچہ <قسم_نام Pred, قسم_نام Val> خودکار
  عامل () (Pred&& p, Val &&v) ثابت HB_RETURN (منطقی,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_has);

ساخت
{
  نجی:

  سانچہ <قسم_نام Pred, قسم_نام Val> خودکار
  impl (Pred&& p, Val &&v, hb_ترجیح<1>) ثابت HB_AUTO_RETURN
  (
    hb_has (std::forward<Pred> (p),
	    std::forward<Val> (v))
  )

  سانچہ <قسم_نام Pred, قسم_نام Val> خودکار
  impl (Pred&& p, Val &&v, hb_ترجیح<0>) ثابت HB_AUTO_RETURN
  (
    std::forward<Pred> (p) == std::forward<Val> (v)
  )

  عوامی:

  سانچہ <قسم_نام Pred, قسم_نام Val> خودکار
  عامل () (Pred&& p, Val &&v) ثابت HB_RETURN (منطقی,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_مطابقت);

ساخت
{
  نجی:

  سانچہ <قسم_نام Proj, قسم_نام Val> خودکار
  impl (Proj&& f, Val &&v, hb_ترجیح<2>) ثابت HB_AUTO_RETURN
  (
    hb_deref (std::forward<Proj> (f)).get (std::forward<Val> (v))
  )

  سانچہ <قسم_نام Proj, قسم_نام Val> خودکار
  impl (Proj&& f, Val &&v, hb_ترجیح<1>) ثابت HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Proj> (f),
	       std::forward<Val> (v))
  )

  سانچہ <قسم_نام Proj, قسم_نام Val> خودکار
  impl (Proj&& f, Val &&v, hb_ترجیح<0>) ثابت HB_AUTO_RETURN
  (
    std::forward<Proj> (f)[std::forward<Val> (v)]
  )

  عوامی:

  سانچہ <قسم_نام Proj, قسم_نام Val> خودکار
  عامل () (Proj&& f, Val &&v) ثابت HB_AUTO_RETURN
  (
    impl (std::forward<Proj> (f),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_get);

ساخت
{
  نجی:

  سانچہ <قسم_نام T1, قسم_نام T2> خودکار
  impl (T1&& v1, T2 &&v2, hb_ترجیح<3>) ثابت HB_AUTO_RETURN
  (
    std::forward<T2> (v2).cmp (std::forward<T1> (v1)) == 0
  )

  سانچہ <قسم_نام T1, قسم_نام T2> خودکار
  impl (T1&& v1, T2 &&v2, hb_ترجیح<2>) ثابت HB_AUTO_RETURN
  (
    std::forward<T1> (v1).cmp (std::forward<T2> (v2)) == 0
  )

  سانچہ <قسم_نام T1, قسم_نام T2> خودکار
  impl (T1&& v1, T2 &&v2, hb_ترجیح<1>) ثابت HB_AUTO_RETURN
  (
    std::forward<T1> (v1) == std::forward<T2> (v2)
  )

  سانچہ <قسم_نام T1, قسم_نام T2> خودکار
  impl (T1&& v1, T2 &&v2, hb_ترجیح<0>) ثابت HB_AUTO_RETURN
  (
    std::forward<T2> (v2) == std::forward<T1> (v1)
  )

  عوامی:

  سانچہ <قسم_نام T1, قسم_نام T2> خودکار
  عامل () (T1&& v1, T2 &&v2) ثابت HB_AUTO_RETURN
  (
    impl (std::forward<T1> (v1),
	  std::forward<T2> (v2),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_equal);

ساخت
{
  سانچہ <قسم_نام T> خالی
  عامل () (T& a, T& b) ثابت
  {
    استعمال std::swap; // allow ADL
    swap (a, b);
  }
}
HB_FUNCOBJ (hb_تبادلہ);


سانچہ <قسم_نام T1, قسم_نام T2>
ساخت hb_pair_t
{
  قسمتعریف T1 پہلا_t;
  قسمتعریف T2 دوسرا_t;
  قسمتعریف hb_pair_t<T1, T2> pair_t;

  سانچہ <قسم_نام U1 = T1, قسم_نام U2 = T2,
	    hb_enable_if (std::is_default_constructible<U1>::value &&
			  std::is_default_constructible<U2>::value)>
  hb_pair_t () : پہلا (), second () {}
  hb_pair_t (T1 a, T2 b) : پہلا (std::forward<T1> (a)), second (std::forward<T2> (b)) {}

  سانچہ <قسم_نام Q1, قسم_نام Q2,
	    hb_enable_if (hb_is_convertible (T1, Q1) &&
			  hb_is_convertible (T2, Q2))>
  عامل hb_pair_t<Q1, Q2> () { واپس hb_pair_t<Q1, Q2> (پہلا, second); }

  hb_pair_t<T1, T2> reverse () ثابت
  { واپس hb_pair_t<T1, T2> (second, پہلا); }

  منطقی عامل == (ثابت pair_t& o) ثابت { واپس پہلا == o.پہلا && second == o.second; }
  منطقی عامل != (ثابت pair_t& o) ثابت { واپس !(*یہ == o); }
  منطقی عامل < (ثابت pair_t& o) ثابت { واپس پہلا < o.پہلا || (پہلا == o.پہلا && second < o.second); }
  منطقی عامل >= (ثابت pair_t& o) ثابت { واپس !(*یہ < o); }
  منطقی عامل > (ثابت pair_t& o) ثابت { واپس پہلا > o.پہلا || (پہلا == o.پہلا && second > o.second); }
  منطقی عامل <= (ثابت pair_t& o) ثابت { واپس !(*یہ > o); }

  ساکن صحیح cmp (ثابت خالی *pa, ثابت خالی *pb)
  {
    pair_t *a = (pair_t *) pa;
    pair_t *b = (pair_t *) pb;

    اگر (a->پہلا < b->پہلا) واپس -1;
    اگر (a->پہلا > b->پہلا) واپس +1;
    اگر (a->second < b->second) واپس -1;
    اگر (a->second > b->second) واپس +1;
    واپس 0;
  }

  دوست خالی swap (hb_pair_t& a, hb_pair_t& b) noexcept
  {
    hb_تبادلہ (a.پہلا, b.پہلا);
    hb_تبادلہ (a.second, b.second);
  }


  T1 پہلا;
  T2 second;
};
سانچہ <قسم_نام T1, قسم_نام T2> ساکن اندرونی hb_pair_t<T1, T2>
hb_pair (T1&& a, T2&& b) { واپس hb_pair_t<T1, T2> (a, b); }

قسمتعریف hb_pair_t<hb_codepoint_t, hb_codepoint_t> hb_codepoint_pair_t;

ساخت
{
  سانچہ <قسم_نام Pair> constexpr قسم_نام Pair::پہلا_t
  عامل () (ثابت Pair& pair) ثابت { واپس pair.پہلا; }
}
HB_FUNCOBJ (hb_پہلا);

ساخت
{
  سانچہ <قسم_نام Pair> constexpr قسم_نام Pair::دوسرا_t
  عامل () (ثابت Pair& pair) ثابت { واپس pair.second; }
}
HB_FUNCOBJ (hb_دوسرا);

/* Note.  In min/max impl, we can use hb_type_identity<T> for second argument.
 * However, that would silently convert between different-signedness integers.
 * Instead we accept two different types, such that compiler can err if
 * comparing integers of different signedness. */
ساخت
{
  سانچہ <قسم_نام T, قسم_نام T2> constexpr خودکار
  عامل () (T&& a, T2&& b) ثابت HB_AUTO_RETURN
  (a <= b ? a : b)
}
HB_FUNCOBJ (hb_min);
ساخت
{
  سانچہ <قسم_نام T, قسم_نام T2> constexpr خودکار
  عامل () (T&& a, T2&& b) ثابت HB_AUTO_RETURN
  (a >= b ? a : b)
}
HB_FUNCOBJ (hb_max);
ساخت
{
  سانچہ <قسم_نام T, قسم_نام T2, قسم_نام T3> constexpr خودکار
  عامل () (T&& x, T2&& min, T3&& max) ثابت HB_AUTO_RETURN
  (hb_min (hb_max (std::forward<T> (x), std::forward<T2> (min)), std::forward<T3> (max)))
}
HB_FUNCOBJ (hb_clamp);

/* Signed saturating arithmetic. */
سانچہ <قسم_نام T>
ساکن HB_ALWAYS_INLINE T
hb_saturate_جوڑو (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#اگر hb_has_builtin(__builtin_add_overflow)
  T نتیجہ;
  اگر (likely (!__builtin_add_overflow (a, b, &نتیجہ)))
    واپس نتیجہ;
#ورنہ
  اگر (b > 0)
  {
    اگر (likely (a <= hb_int_max (T) - b))
      واپس a + b;
  }
  ورنہ اگر (likely (a >= hb_int_min (T) - b))
    واپس a + b;
#اگر_ختم

  واپس a < 0 ? hb_int_min (T) : hb_int_max (T);
}

سانچہ <قسم_نام T>
ساکن HB_ALWAYS_INLINE T
hb_saturate_sub (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#اگر hb_has_builtin(__builtin_sub_overflow)
  T نتیجہ;
  اگر (likely (!__builtin_sub_overflow (a, b, &نتیجہ)))
    واپس نتیجہ;
#ورنہ
  اگر (b > 0)
  {
    اگر (likely (a >= hb_int_min (T) + b))
      واپس a - b;
  }
  ورنہ اگر (likely (a <= hb_int_max (T) + b))
    واپس a - b;
#اگر_ختم

  واپس b < 0 ? hb_int_max (T) : hb_int_min (T);
}

سانچہ <قسم_نام T>
ساکن HB_ALWAYS_INLINE T
hb_saturate_neg (T a)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  واپس hb_saturate_sub ((T) 0, a);
}

/* Convert a floating-point value to integer type T, saturating to T's
 * range instead of relying on the undefined behavior of an out-of-range
 * float-to-int conversion.  NaN saturates to the minimum of T. */
سانچہ <قسم_نام T>
ساکن HB_ALWAYS_INLINE T
hb_clamp_مقصد (دوہرا v)
{
  واپس (T) hb_clamp (v,
		       (دوہرا) hb_int_min (T),
		       (دوہرا) hb_int_max (T));
}

سانچہ <قسم_نام T>
ساکن HB_ALWAYS_INLINE T
hb_clamp_مقصد (اعشاری v)
{
  /* Widen explicitly: float cannot represent all integer bounds exactly,
   * so the clamp itself is done in double. */
  واپس hb_clamp_مقصد<T> ((دوہرا) v);
}

سانچہ <قسم_نام T>
ساکن HB_ALWAYS_INLINE T
hb_clamp_مقصد (int64_t v)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  واپس (T) hb_clamp (v,
		       (int64_t) hb_int_min (T),
		       (int64_t) hb_int_max (T));
}

/*
 * Bithacks.
 */

/* Return the number of 1 bits in a uint8_t; faster than hb_popcount() */
ساکن اندرونی غیرعلامتی
hb_popcount8 (uint8_t v)
{
  ساکن ثابت uint8_t popcount4[16] = {
    0, 1, 1, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 2, 3, 3, 4
  };
  واپس popcount4[v & 0xF] + popcount4[v >> 4];
}

/* Return the number of 1 bits in v. */
سانچہ <قسم_نام T>
ساکن اندرونی غیرعلامتی صحیح
hb_popcount (T v)
{
#اگر hb_has_builtin(__builtin_popcount)
  اگر (حجم (T) <= حجم (غیرعلامتی صحیح))
    واپس __builtin_popcount (v);
#اگر_ختم

#اگر hb_has_builtin(__builtin_popcountl)
  اگر (حجم (T) <= حجم (غیرعلامتی طویل))
    واپس __builtin_popcountl (v);
#اگر_ختم

#اگر hb_has_builtin(__builtin_popcountll)
  اگر (حجم (T) <= حجم (غیرعلامتی طویل طویل))
    واپس __builtin_popcountll (v);
#اگر_ختم

  اگر (حجم (T) <= 4)
  {
    /* "HACKMEM 169" */
    uint32_t y;
    y = (v >> 1) &033333333333;
    y = v - y - ((y >>1) & 033333333333);
    واپس (((y + (y >> 3)) & 030707070707) % 077);
  }

  اگر (حجم (T) == 8)
  {
    uint64_t y = (uint64_t) v;
    y -= ((y >> 1) & 0x5555555555555555ull);
    y = (y & 0x3333333333333333ull) + (y >> 2 & 0x3333333333333333ull);
    واپس ((y + (y >> 4)) & 0xf0f0f0f0f0f0f0full) * 0x101010101010101ull >> 56;
  }

  اگر (حجم (T) == 16)
  {
    غیرعلامتی صحیح سرکاؤ = 64;
    واپس hb_popcount<uint64_t> ((uint64_t) v) + hb_popcount ((uint64_t) (v >> سرکاؤ));
  }

  أكد (0);
  واپس 0; /* Shut up stupid compiler. */
}

/* Returns the number of bits needed to store number */
سانچہ <قسم_نام T>
ساکن اندرونی غیرعلامتی صحیح
hb_ثنائی_ہندسہ_storage (T v)
{
  اگر (unlikely (!v)) واپس 0;

#اگر hb_has_builtin(__builtin_clz)
  اگر (حجم (T) <= حجم (غیرعلامتی صحیح))
    واپس حجم (غیرعلامتی صحیح) * 8 - __builtin_clz (v);
#اگر_ختم

#اگر hb_has_builtin(__builtin_clzl)
  اگر (حجم (T) <= حجم (غیرعلامتی طویل))
    واپس حجم (غیرعلامتی طویل) * 8 - __builtin_clzl (v);
#اگر_ختم

#اگر hb_has_builtin(__builtin_clzll)
  اگر (حجم (T) <= حجم (غیرعلامتی طویل طویل))
    واپس حجم (غیرعلامتی طویل طویل) * 8 - __builtin_clzll (v);
#اگر_ختم

#اگر (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  اگر (حجم (T) <= حجم (غیرعلامتی صحیح))
  {
    غیرعلامتی طویل where;
    ثنائی_ہندسہ_جائزہ_الٹ (&where, v);
    واپس 1 + where;
  }
# اگر defined(_WIN64)
  اگر (حجم (T) <= 8)
  {
    غیرعلامتی طویل where;
    ثنائی_ہندسہ_جائزہ_Reverse64 (&where, v);
    واپس 1 + where;
  }
# endif
#اگر_ختم

  اگر (حجم (T) <= 4)
  {
    /* "bithacks" */
    ثابت غیرعلامتی صحیح b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    ثابت غیرعلامتی صحیح S[] = {1, 2, 4, 8, 16};
    غیرعلامتی صحیح r = 0;
    برائے (صحیح i = 4; i >= 0; i--)
      اگر (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    واپس r + 1;
  }
  اگر (حجم (T) <= 8)
  {
    /* "bithacks" */
    ثابت uint64_t b[] = {0x2ULL, 0xCULL, 0xF0ULL, 0xFF00ULL, 0xFFFF0000ULL, 0xFFFFFFFF00000000ULL};
    ثابت غیرعلامتی صحیح S[] = {1, 2, 4, 8, 16, 32};
    غیرعلامتی صحیح r = 0;
    برائے (صحیح i = 5; i >= 0; i--)
      اگر (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    واپس r + 1;
  }
  اگر (حجم (T) == 16)
  {
    غیرعلامتی صحیح سرکاؤ = 64;
    واپس (v >> سرکاؤ) ? hb_ثنائی_ہندسہ_storage<uint64_t> ((uint64_t) (v >> سرکاؤ)) + سرکاؤ :
			  hb_ثنائی_ہندسہ_storage<uint64_t> ((uint64_t) v);
  }

  أكد (0);
  واپس 0; /* Shut up stupid compiler. */
}

/* Returns the number of zero bits in the least significant side of v */
سانچہ <قسم_نام T>
ساکن اندرونی غیرعلامتی صحیح
hb_ctz (T v)
{
  اگر (unlikely (!v)) واپس 8 * حجم (T);

#اگر hb_has_builtin(__builtin_ctz)
  اگر (حجم (T) <= حجم (غیرعلامتی صحیح))
    واپس __builtin_ctz (v);
#اگر_ختم

#اگر hb_has_builtin(__builtin_ctzl)
  اگر (حجم (T) <= حجم (غیرعلامتی طویل))
    واپس __builtin_ctzl (v);
#اگر_ختم

#اگر hb_has_builtin(__builtin_ctzll)
  اگر (حجم (T) <= حجم (غیرعلامتی طویل طویل))
    واپس __builtin_ctzll (v);
#اگر_ختم

#اگر (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  اگر (حجم (T) <= حجم (غیرعلامتی صحیح))
  {
    غیرعلامتی طویل where;
    ثنائی_ہندسہ_جائزہ_Forward (&where, v);
    واپس where;
  }
# اگر defined(_WIN64)
  اگر (حجم (T) <= 8)
  {
    غیرعلامتی طویل where;
    ثنائی_ہندسہ_جائزہ_Forward64 (&where, v);
    واپس where;
  }
# endif
#اگر_ختم

  اگر (حجم (T) <= 4)
  {
    /* "bithacks" */
    غیرعلامتی صحیح c = 32;
    v &= - (int32_t) v;
    اگر (v) c--;
    اگر (v & 0x0000FFFF) c -= 16;
    اگر (v & 0x00FF00FF) c -= 8;
    اگر (v & 0x0F0F0F0F) c -= 4;
    اگر (v & 0x33333333) c -= 2;
    اگر (v & 0x55555555) c -= 1;
    واپس c;
  }
  اگر (حجم (T) <= 8)
  {
    /* "bithacks" */
    غیرعلامتی صحیح c = 64;
    v &= - (int64_t) (v);
    اگر (v) c--;
    اگر (v & 0x00000000FFFFFFFFULL) c -= 32;
    اگر (v & 0x0000FFFF0000FFFFULL) c -= 16;
    اگر (v & 0x00FF00FF00FF00FFULL) c -= 8;
    اگر (v & 0x0F0F0F0F0F0F0F0FULL) c -= 4;
    اگر (v & 0x3333333333333333ULL) c -= 2;
    اگر (v & 0x5555555555555555ULL) c -= 1;
    واپس c;
  }
  اگر (حجم (T) == 16)
  {
    غیرعلامتی صحیح سرکاؤ = 64;
    واپس (uint64_t) v ? hb_ثنائی_ہندسہ_storage<uint64_t> ((uint64_t) v) :
			  hb_ثنائی_ہندسہ_storage<uint64_t> ((uint64_t) (v >> سرکاؤ)) + سرکاؤ;
  }

  أكد (0);
  واپس 0; /* Shut up stupid compiler. */
}


/*
 * Tiny stuff.
 */

/* ASCII tag/character handling */
ساکن اندرونی منطقی ISALPHA (غیرعلامتی حرف c)
{ واپس (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
ساکن اندرونی منطقی ISALNUM (غیرعلامتی حرف c)
{ واپس (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }
ساکن اندرونی منطقی ISSPACE (غیرعلامتی حرف c)
{ واپس c == ' ' || c =='\f'|| c =='\n'|| c =='\r'|| c =='\t'|| c =='\v'; }
ساکن اندرونی غیرعلامتی حرف TOUPPER (غیرعلامتی حرف c)
{ واپس (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c; }
ساکن اندرونی غیرعلامتی حرف TOLOWER (غیرعلامتی حرف c)
{ واپس (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }
ساکن اندرونی منطقی ISHEX (غیرعلامتی حرف c)
{ واپس (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
ساکن اندرونی غیرعلامتی حرف TOHEX (uint8_t c)
{ واپس (c & 0xF) <= 9 ? (c & 0xF) + '0' : (c & 0xF) + 'a' - 10; }
ساکن اندرونی uint8_t FROMHEX (غیرعلامتی حرف c)
{ واپس (c >= '0' && c <= '9') ? c - '0' : TOLOWER (c) - 'a' + 10; }

ساکن اندرونی غیرعلامتی صحیح DIV_CEIL (ثابت غیرعلامتی صحیح a, غیرعلامتی صحیح b)
{ واپس (a + (b - 1)) / b; }


#تعریف_منسوخ  ARRAY_لمبائی
سانچہ <قسم_نام Type, غیرعلامتی صحیح n>
ساکن اندرونی غیرعلامتی صحیح ARRAY_لمبائی (ثابت Type (&)[n]) { واپس n; }
/* A const version, but does not detect erratically being called on pointers. */
#تعریف_کرو ARRAY_لمبائی_CONST(__array) ((علامتی صحیح) (حجم (__array) / حجم (__array[0])))


ساکن اندرونی خالی *
hb_memcpy (خالی *__restrict dst, ثابت خالی *__restrict src, size_t len)
{
  /* It's illegal to pass 0 as size to memcpy. */
  اگر (unlikely (!len)) واپس dst;
  واپس حافظہ_نقل (dst, src, len);
}

ساکن اندرونی صحیح
hb_memcmp (ثابت خالی *a, ثابت خالی *b, غیرعلامتی صحیح len)
{
  /* It's illegal to pass NULL to memcmp(), even if len is zero.
   * So, wrap it.
   * https://sourceware.org/bugzilla/show_bug.cgi?id=23878 */
  اگر (unlikely (!len)) واپس 0;
  واپس memcmp (a, b, len);
}

ساکن اندرونی خالی *
hb_memset (خالی *s, صحیح c, غیرعلامتی صحیح n)
{
  /* It's illegal to pass NULL to memset(), even if n is zero. */
  اگر (unlikely (!n)) واپس s;
  واپس memset (s, c, n);
}

ساکن اندرونی غیرعلامتی صحیح
hb_ceil_مقصد_4 (غیرعلامتی صحیح v)
{
  واپس ((v - 1) | 3) + 1;
}

سانچہ <قسم_نام T> ساکن اندرونی منطقی
hb_اندر_range (T u, T lo, T hi)
{
  static_assert (!std::is_signed<T>::value, "");

  /* The casts below are important as if T is smaller than int,
   * the subtract results will become a signed int! */
  واپس (T)(u - lo) <= (T)(hi - lo);
}
سانچہ <قسم_نام T> ساکن اندرونی منطقی
hb_اندر_ranges (T u, T lo1, T hi1)
{
  واپس hb_اندر_range (u, lo1, hi1);
}
سانچہ <قسم_نام T, قسم_نام ...Ts> ساکن اندرونی منطقی
hb_اندر_ranges (T u, T lo1, T hi1, Ts... ds)
{
  واپس hb_اندر_range<T> (u, lo1, hi1) || hb_اندر_ranges<T> (u, ds...);
}


/*
 * Overflow checking.
 */

ساکن اندرونی منطقی
hb_unsigned_mul_overflows (غیرعلامتی صحیح گنتی, غیرعلامتی صحیح سائز, غیرعلامتی *نتیجہ = خالی_اشاریہ)
{
#اگر hb_has_builtin(__builtin_mul_overflow)
  غیرعلامتی ڈھیر_نتیجہ;
  اگر (!نتیجہ)
    نتیجہ = &ڈھیر_نتیجہ;
  واپس __builtin_mul_overflow (گنتی, سائز, نتیجہ);
#اگر_ختم

  اگر (نتیجہ)
    *نتیجہ = گنتی * سائز;
  واپس (سائز > 0) && (گنتی >= ((غیرعلامتی صحیح) -1) / سائز);
}

ساکن اندرونی منطقی
hb_unsigned_جوڑو_overflows (غیرعلامتی صحیح a, غیرعلامتی صحیح b, غیرعلامتی *نتیجہ = خالی_اشاریہ)
{
#اگر hb_has_builtin(__builtin_add_overflow)
  غیرعلامتی ڈھیر_نتیجہ;
  اگر (!نتیجہ)
    نتیجہ = &ڈھیر_نتیجہ;
  واپس __builtin_add_overflow (a, b, نتیجہ);
#اگر_ختم

  اگر (نتیجہ)
    *نتیجہ = a + b;
  واپس b > (غیرعلامتی صحیح) -1 - a;
}

/* Saturating arithmetic on size_t.  On platforms where size_t is wider than
 * unsigned int (i.e. 64-bit), the inputs to get_size()-style computations
 * (counts and static_sizes which are at most 32-bit) cannot overflow size_t,
 * so these reduce to plain arithmetic.  On 32-bit platforms (size_t is
 * unsigned int), they saturate to SIZE_MAX so sanitize/serialize callers
 * naturally reject the resulting size as out-of-range. */

ساکن اندرونی size_t
hb_unsigned_mul_saturate (size_t a, size_t b)
{
  اگر (حجم (size_t) > حجم (غیرعلامتی صحیح))
    واپس a * b;
#اگر hb_has_builtin(__builtin_mul_overflow)
  size_t نتیجہ;
  اگر (__builtin_mul_overflow (a, b, &نتیجہ))
    واپس (size_t) -1;
  واپس نتیجہ;
#ورنہ
  اگر (b > 0 && a > ((size_t) -1) / b) واپس (size_t) -1;
  واپس a * b;
#اگر_ختم
}

ساکن اندرونی size_t
hb_unsigned_جوڑو_saturate (size_t a, size_t b)
{
  اگر (حجم (size_t) > حجم (غیرعلامتی صحیح))
    واپس a + b;
#اگر hb_has_builtin(__builtin_add_overflow)
  size_t نتیجہ;
  اگر (__builtin_add_overflow (a, b, &نتیجہ))
    واپس (size_t) -1;
  واپس نتیجہ;
#ورنہ
  اگر (b > ((size_t) -1) - a) واپس (size_t) -1;
  واپس a + b;
#اگر_ختم
}

/* Variadic forms: fold left across all arguments. */
سانچہ <قسم_نام ...Ts>
ساکن اندرونی size_t
hb_unsigned_mul_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ واپس hb_unsigned_mul_saturate (hb_unsigned_mul_saturate (a, b), c, rest...); }

سانچہ <قسم_نام ...Ts>
ساکن اندرونی size_t
hb_unsigned_جوڑو_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ واپس hb_unsigned_جوڑو_saturate (hb_unsigned_جوڑو_saturate (a, b), c, rest...); }

/* Saturating mul-add: a * b + c.  Covers the dominant get_size() pattern
 * `count * static_size + min_size`. */
ساکن اندرونی size_t
hb_unsigned_mul_جوڑو_saturate (size_t a, size_t b, size_t c)
{ واپس hb_unsigned_جوڑو_saturate (hb_unsigned_mul_saturate (a, b), c); }


/*
 * Sort and search.
 */

سانچہ <قسم_نام K, قسم_نام V, قسم_نام ...Ts>
ساکن صحیح
hb_cmp_طریقہ (ثابت خالی *pkey, ثابت خالی *pval, Ts... ds)
{
  ثابت K& key = * (ثابت K*) pkey;
  ثابت V& val = * (ثابت V*) pval;

  واپس val.cmp (key, ds...);
}

سانچہ <قسم_نام K, قسم_نام V>
ساکن صحیح
_hb_cmp_operator (ثابت خالی *pkey, ثابت خالی *pval)
{
  ثابت K& key = * (ثابت K*) pkey;
  ثابت V& val = * (ثابت V*) pval;

  اگر (key < val) واپس -1;
  اگر (key > val) واپس  1;
  واپس 0;
}

سانچہ <قسم_نام V, قسم_نام K, قسم_نام ...Ts>
HB_HOT
ساکن اندرونی منطقی
hb_bsearch_impl (غیرعلامتی *pos, /* Out */
		 ثابت K& key,
		 V* بنیاد, size_t nmemb, size_t stride,
		 صحیح (*compar)(ثابت خالی *کلید_3, ثابت خالی *شے_2, Ts... _ds),
		 Ts... ds)
{
  /* This is our *only* bsearch implementation. */

  صحیح min = 0, max = (صحیح) nmemb - 1;
  جب (min <= max)
  {
    صحیح mid = ((غیرعلامتی صحیح) min + (غیرعلامتی صحیح) max) / 2;
#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wcast-align"
    V* p = (V*) (((ثابت حرف *) بنیاد) + (mid * stride));
#ہدایت GCC diagnostic نکالو
    صحیح c = compar ((ثابت خالی *) std::addressof (key), (ثابت خالی *) p, ds...);
    اگر (c < 0)
      max = mid - 1;
    ورنہ اگر (c > 0)
      min = mid + 1;
    ورنہ
    {
      *pos = mid;
      واپس سچ;
    }
  }
  *pos = min;
  واپس جھوٹ;
}

سانچہ <قسم_نام V, قسم_نام K>
ساکن اندرونی V*
hb_bsearch (ثابت K& key, V* بنیاد,
	    size_t nmemb, size_t stride = حجم (V),
	    صحیح (*compar)(ثابت خالی *کلید_3, ثابت خالی *شے_2) = hb_cmp_طریقہ<K, V>)
{
  غیرعلامتی pos;
#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wcast-align"
  واپس hb_bsearch_impl (&pos, key, بنیاد, nmemb, stride, compar) ?
	 (V*) (((ثابت حرف *) بنیاد) + (pos * stride)) : خالی_اشاریہ;
#ہدایت GCC diagnostic نکالو
}
سانچہ <قسم_نام V, قسم_نام K, قسم_نام ...Ts>
ساکن اندرونی V*
hb_bsearch (ثابت K& key, V* بنیاد,
	    size_t nmemb, size_t stride,
	    صحیح (*compar)(ثابت خالی *کلید_3, ثابت خالی *شے_2, Ts... _ds),
	    Ts... ds)
{
  غیرعلامتی pos;
#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wcast-align"
  واپس hb_bsearch_impl (&pos, key, بنیاد, nmemb, stride, compar, ds...) ?
	 (V*) (((ثابت حرف *) بنیاد) + (pos * stride)) : خالی_اشاریہ;
#ہدایت GCC diagnostic نکالو
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
سانچہ <قسم_نام T, قسم_نام Compar>
ساکن اندرونی خالی
hb_qsort_loop (T *بنیاد, size_t nel, Compar compar)
{
  جب (nel > 24)
  {
    T *آخری = بنیاد + nel - 1;
    T *mid = بنیاد + nel / 2;

    /* Median-of-three pivot, parked at last-1. */
    اگر (compar (*بنیاد, *mid) > 0) hb_تبادلہ (*بنیاد, *mid);
    اگر (compar (*mid, *آخری) > 0)
    {
      hb_تبادلہ (*mid, *آخری);
      اگر (compar (*بنیاد, *mid) > 0) hb_تبادلہ (*بنیاد, *mid);
    }
    hb_تبادلہ (*mid, *(آخری - 1));
    T &محور = *(آخری - 1);

    /* Two-way Hoare partition.  Inner loops are unguarded:
     * median-of-three left *base <= pivot and *last >= pivot,
     * which act as sentinels. */
    T *i = بنیاد, *j = آخری - 1;
    جب (سچ)
    {
      جب (compar (*++i, محور) < 0) {}
      جب (compar (*--j, محور) > 0) {}
      اگر (i >= j) توڑو;
      hb_تبادلہ (*i, *j);
    }
    hb_تبادلہ (*i, *(آخری - 1));

    /* Recurse on smaller side, loop on larger — bounds stack
     * depth at O(log n). */
    size_t بائیں  = (size_t) (i - بنیاد);
    size_t دائیں = nel - بائیں - 1;
    اگر (بائیں < دائیں)
    {
      hb_qsort_loop (بنیاد, بائیں, compar);
      بنیاد = i + 1;
      nel  = دائیں;
    }
    ورنہ
    {
      hb_qsort_loop (i + 1, دائیں, compar);
      nel  = بائیں;
    }
  }
}

سانچہ <قسم_نام T, قسم_نام Compar>
ساکن اندرونی خالی
hb_qsort_inline (T *بنیاد, size_t nel, Compar compar)
{
  hb_qsort_loop (بنیاد, nel, compar);

  /* Single final insertion sort over the whole array.  After
   * the partitioning loop, every element is within the threshold
   * of its sorted position, so this pass is O(n * threshold). */
  T *انتہا = بنیاد + nel;
  برائے (T *pi = بنیاد + 1; pi < انتہا; pi++)
    برائے (T *pj = pi; pj > بنیاد && compar (pj[-1], pj[0]) > 0; pj--)
      hb_تبادلہ (pj[-1], pj[0]);
}

ساکن اندرونی خالی
hb_qsort (خالی *بنیاد, size_t nel, size_t چوڑائی,
	  صحیح (*compar)(ثابت خالی *_a, ثابت خالی *_b))
{
  qsort (بنیاد, nel, چوڑائی, compar);
}




سانچہ <قسم_نام T, قسم_نام T2, قسم_نام T3 = صحیح> ساکن اندرونی خالی
hb_stable_ترتیب_دو (T *array, غیرعلامتی صحیح len, صحیح(*compar)(ثابت T2 *, ثابت T2 *), T3 *array2 = خالی_اشاریہ)
{
  static_assert (hb_is_trivially_copy_assignable (T), "");
  static_assert (hb_is_trivially_copy_assignable (T3), "");

  برائے (غیرعلامتی صحیح i = 1; i < len; i++)
  {
    غیرعلامتی صحیح j = i;
    جب (j && compar (&array[j - 1], &array[i]) > 0)
      j--;
    اگر (i == j)
      جاری;
    /* Move item i to occupy place for item j, shift what's in between. */
    {
      T t = array[i];
      memmove (&array[j + 1], &array[j], (i - j) * حجم (T));
      array[j] = t;
    }
    اگر (array2)
    {
      T3 t = array2[i];
      memmove (&array2[j + 1], &array2[j], (i - j) * حجم (T3));
      array2[j] = t;
    }
  }
}

ساکن اندرونی hb_bool_t
hb_codepoint_parse (ثابت حرف *s, غیرعلامتی صحیح len, صحیح بنیاد, hb_codepoint_t *برآمد)
{
  غیرعلامتی صحیح v;
  ثابت حرف *p = s;
  ثابت حرف *انتہا = p + len;
  اگر (unlikely (!hb_parse_uint (&p, انتہا, &v, سچ/* whole buffer */, بنیاد)))
    واپس جھوٹ;

  *برآمد = v;
  واپس سچ;
}


/* Operators. */

ساخت
{ HB_PARTIALIZE(2);
  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (a & b)
}
HB_FUNCOBJ (hb_bitwise_and);
ساخت
{ HB_PARTIALIZE(2);
  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (a | b)
}
HB_FUNCOBJ (hb_bitwise_or);
ساخت
{ HB_PARTIALIZE(2);
  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (a ^ b)
}
HB_FUNCOBJ (hb_bitwise_xor);
ساخت
{ HB_PARTIALIZE(2);
  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (~a & b)
}
HB_FUNCOBJ (hb_bitwise_lt);
ساخت
{ HB_PARTIALIZE(2);
  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (a & ~b)
}
HB_FUNCOBJ (hb_bitwise_gt); // aka sub
ساخت
{ HB_PARTIALIZE(2);
  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (~a | b)
}
HB_FUNCOBJ (hb_bitwise_le);
ساخت
{ HB_PARTIALIZE(2);
  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (a | ~b)
}
HB_FUNCOBJ (hb_bitwise_ge);
ساخت
{
  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (ثابت T &a) ثابت HB_AUTO_RETURN (~a)
}
HB_FUNCOBJ (hb_bitwise_neg);

ساخت
{ HB_PARTIALIZE(2);
  سانچہ <قسم_نام T, قسم_نام T2> constexpr خودکار
  عامل () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (a + b)
}
HB_FUNCOBJ (hb_جوڑو);
ساخت
{ HB_PARTIALIZE(2);
  سانچہ <قسم_نام T, قسم_نام T2> constexpr خودکار
  عامل () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (a - b)
}
HB_FUNCOBJ (hb_sub);
ساخت
{ HB_PARTIALIZE(2);
  سانچہ <قسم_نام T, قسم_نام T2> constexpr خودکار
  عامل () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (b - a)
}
HB_FUNCOBJ (hb_rsub);
ساخت
{ HB_PARTIALIZE(2);
  سانچہ <قسم_نام T, قسم_نام T2> constexpr خودکار
  عامل () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (a * b)
}
HB_FUNCOBJ (hb_mul);
ساخت
{ HB_PARTIALIZE(2);
  سانچہ <قسم_نام T, قسم_نام T2> constexpr خودکار
  عامل () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (a / b)
}
HB_FUNCOBJ (hb_div);
ساخت
{ HB_PARTIALIZE(2);
  سانچہ <قسم_نام T, قسم_نام T2> constexpr خودکار
  عامل () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (a % b)
}
HB_FUNCOBJ (hb_mod);
ساخت
{
  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (ثابت T &a) ثابت HB_AUTO_RETURN (+a)
}
HB_FUNCOBJ (hb_pos);
ساخت
{
  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (ثابت T &a) ثابت HB_AUTO_RETURN (-a)
}
HB_FUNCOBJ (hb_neg);
ساخت
{
  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (T &a) ثابت HB_AUTO_RETURN (++a)
}
HB_FUNCOBJ (hb_inc);
ساخت
{
  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (T &a) ثابت HB_AUTO_RETURN (--a)
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
سانچہ <قسم_نام func_t>
دوہرا solve_itp (func_t f,
		  دوہرا a, دوہرا b,
		  دوہرا epsilon,
		  دوہرا min_y, دوہرا max_y,
		  دوہرا &ya, دوہرا &yb, دوہرا &y)
{
  // Guard against degenerate interval
  اگر (b - a <= 0.0)
  {
    y = ya;
    واپس a;
  }

  غیرعلامتی n1_2 = (غیرعلامتی) (hb_max (ceil (log2 ((b - a) / epsilon)) - 1.0, 0.0));
  ثابت غیرعلامتی n0 = 1; // Hardwired
  ثابت دوہرا k1 = 0.2 / (b - a); // Hardwired.
  غیرعلامتی nmax = n0 + n1_2;
  دوہرا scaled_epsilon = epsilon * دوہرا (1llu << nmax);
  دوہرا _2_epsilon = 2.0 * epsilon;
  جب (b - a > _2_epsilon)
  {
    دوہرا x1_2 = 0.5 * (a + b);
    دوہرا r = scaled_epsilon - 0.5 * (b - a);
    // Guard against yb == ya to prevent division by zero
    دوہرا xf = (yb != ya) ? (yb * a - ya * b) / (yb - ya) : x1_2;
    دوہرا sigma = x1_2 - xf;
    دوہرا b_a = b - a;
    // This has k2 = 2 hardwired for efficiency.
    دوہرا b_a_k2 = b_a * b_a;
    دوہرا delta = k1 * b_a_k2;
    صحیح sigma_sign = sigma >= 0 ? +1 : -1;
    دوہرا xt = delta <= fabs (x1_2 - xf) ? xf + delta * sigma_sign : x1_2;
    دوہرا xitp = fabs (xt - x1_2) <= r ? xt : x1_2 - r * sigma_sign;
    دوہرا yitp = f (xitp);
    اگر (yitp > max_y)
    {
      b = xitp;
      yb = yitp;
    }
    ورنہ اگر (yitp < min_y)
    {
      a = xitp;
      ya = yitp;
    }
    ورنہ
    {
      y = yitp;
      واپس xitp;
    }
    scaled_epsilon *= 0.5;
  }
  واپس 0.5 * (a + b);
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
سانچہ <قسم_نام F>
ساخت hb_scope_guard_t
{
  صریح hb_scope_guard_t (F &&f) : f (std::move (f)), فعال (سچ) {}
  hb_scope_guard_t (hb_scope_guard_t &&o) noexcept
    : f (std::move (o.f)), فعال (o.فعال) { o.فعال = جھوٹ; }
  hb_scope_guard_t (ثابت hb_scope_guard_t &) = حذف;
  hb_scope_guard_t &عامل= (ثابت hb_scope_guard_t &) = حذف;
  hb_scope_guard_t &عامل= (hb_scope_guard_t &&) = حذف;
  ~hb_scope_guard_t () { اگر (فعال) f (); }

  /* Release: dismiss the guard so the cleanup does NOT run.  Use
   * when transferring ownership out of the scope. */
  خالی release () { فعال = جھوٹ; }

  نجی:
  F f;
  منطقی فعال;
};

سانچہ <قسم_نام F>
ساکن اندرونی hb_scope_guard_t<F> hb_make_scope_guard (F &&f)
{ واپس hb_scope_guard_t<F> (std::forward<F> (f)); }

#تعریف_کرو HB_SCOPE_GUARD_NAME_(سطر) hb_scope_guard_##سطر
#تعریف_کرو HB_SCOPE_GUARD_NAME(سطر) HB_SCOPE_GUARD_NAME_(سطر)
#تعریف_کرو HB_SCOPE_GUARD(stmt) \
  خودکار HB_SCOPE_GUARD_NAME(__LINE__) = \
    hb_make_scope_guard ([&]() { stmt; }); \
  (خالی) HB_SCOPE_GUARD_NAME(__LINE__)


#اگر_ختم /* HB_ALGS_HH */
