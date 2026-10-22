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

#که_نه_تعریف_شوی HB_ALGS_HH
#تعریف_کړه HB_ALGS_HH

#ضمن "hb.hh"
#ضمن "hb-meta.hh"
#ضمن "hb-null.hh"
#ضمن "hb-number.hh"

#ضمن <algorithm>
#ضمن <initializer_list>
#ضمن <functional>
#ضمن <new>

ساكن HB_ALWAYS_INLINE فراغ *
hb_malloc2 (size_t nmemb, size_t اندازه)
{
  إذا (اندازه && nmemb > SIZE_MAX / اندازه) أرجع تش_اشاره;
  أرجع hb_malloc (nmemb * اندازه);
}
ساكن HB_ALWAYS_INLINE فراغ *
hb_realloc2 (فراغ *ptr, size_t nmemb, size_t اندازه)
{
  إذا (اندازه && nmemb > SIZE_MAX / اندازه) أرجع تش_اشاره;
  أرجع hb_realloc (ptr, nmemb * اندازه);
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
#که_تعریف_شوی _MSC_VER
# pragma warning(disable:4200)
# pragma warning(disable:4800)
#د_که_پای
#تعریف_کړه HB_MARK_AS_FLAG_T(T) \
	خارجي "C++" { \
	  ساكن مضمن constexpr T چلوونکی | (T l, T r) { أرجع T ((بینښه) l | (بینښه) r); } \
	  ساكن مضمن constexpr T چلوونکی & (T l, T r) { أرجع T ((بینښه) l & (بینښه) r); } \
	  ساكن مضمن constexpr T چلوونکی ^ (T l, T r) { أرجع T ((بینښه) l ^ (بینښه) r); } \
	  ساكن مضمن constexpr بینښه چلوونکی ~ (T r) { أرجع (~(بینښه) r); } \
	  ساكن مضمن T& چلوونکی |= (T &l, T r) { l = l | r; أرجع l; } \
	  ساكن مضمن T& چلوونکی &= (T& l, T r) { l = l & r; أرجع l; } \
	  ساكن مضمن T& چلوونکی ^= (T& l, T r) { l = l ^ r; أرجع l; } \
	} \
	static_assert (ریښتیا, "")

/* Useful for set-operations on small enums.
 * For example, for testing "x ∈ {x1, x2, x3}" use:
 * (FLAG_UNSAFE(x) & (FLAG(x1) | FLAG(x2) | FLAG(x3)))
 */
#تعریف_کړه FLAG(x) (static_assert_بیان ((بینښه)(x) < 32) + (((uint32_t) 1U) << (بینښه)(x)))
#تعریف_کړه FLAG_UNSAFE(x) ((بینښه)(x) < 32 ? (((uint32_t) 1U) << (بینښه)(x)) : 0)
#تعریف_کړه FLAG_RANGE(x,y) (static_assert_بیان ((x) < (y)) + FLAG(y+1) - FLAG(x))
#تعریف_کړه FLAG64(x) (static_assert_بیان ((بینښه)(x) < 64) + (((uint64_t) 1ULL) << (بینښه)(x)))
#تعریف_کړه FLAG64_UNSAFE(x) ((بینښه)(x) < 64 ? (((uint64_t) 1ULL) << (بینښه)(x)) : 0)


/*
 * Fixed-endian integers / floats.
 */


/* Endian swap, used in Windows related backends */
ساكن مضمن constexpr uint16_t hb_uint16_بدلون (uint16_t v)
{ أرجع (v >> 8) | (v << 8); }
ساكن مضمن constexpr uint32_t hb_uint32_بدلون (uint32_t v)
{ أرجع (hb_uint16_بدلون (v) << 16) | hb_uint16_بدلون (v >> 16); }

کينډۍ <ډول_نوم Type>
بنية __attribute__((packed)) hb_packed_t
{
  hb_packed_t () = افتراضي;
  constexpr hb_packed_t (Type V) : v (V) {}
  چلوونکی Type () ثابت { أرجع v; }
  hb_packed_t & چلوونکی = (Type V) { v = V; أرجع *هذا; }

  خاص:
  Type v;
};

#که_نه_تعریف_شوی HB_FAST_عدد_ACCESS

#إذا defined(__OPTIMIZE__) && \
    defined(__BYTE_ORDER) && \
    (__BYTE_ORDER == __BIG_ENDIAN || \
     (__BYTE_ORDER == __LITTLE_ENDIAN && \
      hb_has_builtin(__builtin_bswap16) && \
      hb_has_builtin(__builtin_bswap32) && \
      hb_has_builtin(__builtin_bswap64)))
#تعریف_کړه HB_FAST_عدد_ACCESS 1
#وإلا
#تعریف_کړه HB_FAST_عدد_ACCESS 0
#د_که_پای

// https://github.com/harfbuzz/harfbuzz/issues/5456
#إذا defined(__GNUC__) && !defined(__clang__) && (__GNUC__ <= 12)
#تعریف_لغوه HB_FAST_عدد_ACCESS
#تعریف_کړه HB_FAST_عدد_ACCESS 0
#د_که_پای

#د_که_پای

کينډۍ <منطقي BE, ډول_نوم Type, صحيح بایټونه_3 = حجم (Type)>
بنية HBInt;
کينډۍ <منطقي BE, ډول_نوم Type>
بنية HBInt<BE, Type, 1>
{
  عام:
  HBInt () = افتراضي;
  constexpr HBInt (Type V) : v {uint8_t (V)} {}
  constexpr چلوونکی Type () ثابت { أرجع v; }
  خاص: uint8_t v;
};
کينډۍ <منطقي BE, ډول_نوم Type>
بنية HBInt<BE, Type, 2>
{
  عام:
  HBInt () = افتراضي;

  HBInt (Type V)
#إذا HB_FAST_عدد_ACCESS
  {
    إذا (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint16_t> *) v) = V;
    وإلا
      *((hb_packed_t<uint16_t> *) v) = __builtin_bswap16 (V);
  }
#وإلا
    : v {BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >>  8) & 0xFF)} {}
#د_که_پای

  constexpr چلوونکی Type () ثابت
  {
#إذا HB_FAST_عدد_ACCESS
    أرجع (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint16_t) *((ثابت hb_packed_t<uint16_t> *) v)
    :
      __builtin_bswap16 ((uint16_t) *((ثابت hb_packed_t<uint16_t> *) v))
    ;
#وإلا
    أرجع (BE ? (v[0] <<  8) : (v[0]      ))
	 + (BE ? (v[1]      ) : (v[1] <<  8));
#د_که_پای
  }
  خاص: uint8_t v[2];
};
کينډۍ <منطقي BE, ډول_نوم Type>
بنية HBInt<BE, Type, 3>
{
  static_assert (!std::is_signed<Type>::value, "");
  عام:
  HBInt () = افتراضي;
  constexpr HBInt (Type V) : v {BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
				BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
				BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V      ) & 0xFF)} {}

  constexpr چلوونکی Type () ثابت { أرجع (BE ? (v[0] << 16) : (v[0]      ))
					  + (BE ? (v[1] <<  8) : (v[1] <<  8))
					  + (BE ? (v[2]      ) : (v[2] << 16)); }
  خاص: uint8_t v[3];
};
کينډۍ <منطقي BE, ډول_نوم Type>
بنية HBInt<BE, Type, 4>
{
  کينډۍ <منطقي, ډول_نوم, صحيح>
  ملګری بنية HBFloat;

  عام:
  HBInt () = افتراضي;

  HBInt (Type V)
#إذا HB_FAST_عدد_ACCESS
  {
    إذا (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint32_t> *) v) = V;
    وإلا
      *((hb_packed_t<uint32_t> *) v) = __builtin_bswap32 (V);
  }
#وإلا
    : v {BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 24) & 0xFF)} {}
#د_که_پای

  constexpr چلوونکی Type () ثابت {
#إذا HB_FAST_عدد_ACCESS
    أرجع (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint32_t) *((ثابت hb_packed_t<uint32_t> *) v)
    :
      __builtin_bswap32 ((uint32_t) *((ثابت hb_packed_t<uint32_t> *) v))
    ;
#وإلا
    أرجع (BE ? (uint32_t (v[0]) << 24) : (uint32_t (v[0])      ))
	 + (BE ? (uint32_t (v[1]) << 16) : (uint32_t (v[1]) <<  8))
	 + (BE ? (uint32_t (v[2]) <<  8) : (uint32_t (v[2]) << 16))
	 + (BE ? (uint32_t (v[3])      ) : (uint32_t (v[3]) << 24));
#د_که_پای
  }
  خاص: uint8_t v[4];
};
کينډۍ <منطقي BE, ډول_نوم Type>
بنية HBInt<BE, Type, 8>
{
  کينډۍ <منطقي, ډول_نوم, صحيح>
  ملګری بنية HBFloat;

  عام:
  HBInt () = افتراضي;

  HBInt (Type V)
#إذا HB_FAST_عدد_ACCESS
  {
    إذا (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint64_t> *) v) = V;
    وإلا
      *((hb_packed_t<uint64_t> *) v) = __builtin_bswap64 (V);
  }
#وإلا
    : v {BE ? uint8_t ((V >> 56) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 48) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >> 40) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V >> 32) & 0xFF) : uint8_t ((V >> 24) & 0xFF),
	 BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V >> 32) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 40) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 48) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 56) & 0xFF)} {}
#د_که_پای

  constexpr چلوونکی Type () ثابت {
#إذا HB_FAST_عدد_ACCESS
    أرجع (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint64_t) *((ثابت hb_packed_t<uint64_t> *) v)
    :
      __builtin_bswap64 ((uint64_t) *((ثابت hb_packed_t<uint64_t> *) v))
    ;
#وإلا
    أرجع (BE ? (uint64_t (v[0]) << 56) : (uint64_t (v[0])      ))
	 + (BE ? (uint64_t (v[1]) << 48) : (uint64_t (v[1]) <<  8))
	 + (BE ? (uint64_t (v[2]) << 40) : (uint64_t (v[2]) << 16))
	 + (BE ? (uint64_t (v[3]) << 32) : (uint64_t (v[3]) << 24))
	 + (BE ? (uint64_t (v[4]) << 24) : (uint64_t (v[4]) << 32))
	 + (BE ? (uint64_t (v[5]) << 16) : (uint64_t (v[5]) << 40))
	 + (BE ? (uint64_t (v[6]) <<  8) : (uint64_t (v[6]) << 48))
	 + (BE ? (uint64_t (v[7])      ) : (uint64_t (v[7]) << 56));
#د_که_پای
  }
  خاص: uint8_t v[8];
};

/* Floats. */

کينډۍ <منطقي BE, ډول_نوم Type, صحيح بایټونه_3>
بنية HBFloat
{
  کارول IntType = ډول_نوم std::conditional<بایټونه_3 == 4, uint32_t, uint64_t>::type;

  عام:
  HBFloat () = افتراضي;

  HBFloat (Type V)
  {
#إذا HB_FAST_عدد_ACCESS
    {
	      إذا (BE == (__BYTE_ORDER == __BIG_ENDIAN))
	      {
	        *((hb_packed_t<Type> *) v) = V;
	        أرجع;
	      }
	    }
#د_که_پای

    اتحاد {
      hb_packed_t<Type> f;
      hb_packed_t<IntType> i;
    } u = {{V}};

    ثابت HBInt<BE, IntType> I = (IntType) u.i;
    لكل (بینښه i = 0; i < بایټونه_3; i++)
      v[i] = I.v[i];
  }

  /* c++14 constexpr */ چلوونکی Type () ثابت
  {
#إذا HB_FAST_عدد_ACCESS
	    {
	      إذا (BE == (__BYTE_ORDER == __BIG_ENDIAN))
		أرجع (Type) *((ثابت hb_packed_t<Type> *) v);
	    }
#د_که_پای

    HBInt<BE, IntType> I;
    لكل (بینښه i = 0; i < بایټونه_3; i++)
      I.v[i] = v[i];

    اتحاد {
      hb_packed_t<IntType> i;
      hb_packed_t<Type> f;
    } u = {{I}};

    أرجع (Type) u.f;
  }
  خاص: uint8_t v[بایټونه_3];
};


/* We want our rounding towards +infinity. */
ساكن مضمن مزدوج
_hb_roundf (مزدوج x) { أرجع floor (x + .5); }

ساكن مضمن عائم
_hb_roundf (عائم x) { أرجع floor ((مزدوج) x + .5); }

#تعریف_کړه roundf(x) _hb_roundf(x)

ساكن مضمن فراغ
hb_sincos (عائم rotation, عائم &s, عائم &c)
{
#که_تعریف_شوی HAVE_SINCOSF
  sincosf (rotation, &s, &c);
#وإلا
  c = cosf (rotation);
  s = sinf (rotation);
#د_که_پای
}
ساكن مضمن فراغ
hb_sincos (مزدوج rotation, مزدوج &s, مزدوج &c)
{
#که_تعریف_شوی HAVE_SINCOS
  sincos (rotation, &s, &c);
#وإلا
  c = کوساین (rotation);
  s = ساین (rotation);
#د_که_پای
}


/* Encodes three unsigned integers in one 64-bit number.  If the inputs have more than 21 bits,
 * values will be truncated / overlap, and might not decode exactly. */
#تعریف_کړه HB_CODEPOINT_ENCODE3(x,y,z) (((uint64_t) (x) << 42) | ((uint64_t) (y) << 21) | (uint64_t) (z))
#تعریف_کړه HB_CODEPOINT_DECODE3_1(v) ((hb_codepoint_t) ((v) >> 42))
#تعریف_کړه HB_CODEPOINT_DECODE3_2(v) ((hb_codepoint_t) ((v) >> 21) & 0x1FFFFFu)
#تعریف_کړه HB_CODEPOINT_DECODE3_3(v) ((hb_codepoint_t) (v) & 0x1FFFFFu)

/* Custom encoding used by hb-ucd. */
#تعریف_کړه HB_CODEPOINT_ENCODE3_11_7_14(x,y,z) (((uint32_t) ((x) & 0x07FFu) << 21) | (((uint32_t) (y) & 0x007Fu) << 14) | (uint32_t) ((z) & 0x3FFFu))
#تعریف_کړه HB_CODEPOINT_DECODE3_11_7_14_1(v) ((hb_codepoint_t) ((v) >> 21))
#تعریف_کړه HB_CODEPOINT_DECODE3_11_7_14_2(v) ((hb_codepoint_t) (((v) >> 14) & 0x007Fu) | 0x0300)
#تعریف_کړه HB_CODEPOINT_DECODE3_11_7_14_3(v) ((hb_codepoint_t) (v) & 0x3FFFu)


بنية
{
  /* Note.  This is dangerous in that if it's passed an rvalue, it returns rvalue-reference. */
  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (T&& v) ثابت HB_AUTO_RETURN ( std::forward<T> (v) )
}
HB_FUNCOBJ (hb_بې_اغېزه_عنصر);
بنية
{
  /* Like identity(), but only retains lvalue-references.  Rvalues are returned as rvalues. */
  کينډۍ <ډول_نوم T> constexpr T&
  چلوونکی () (T& v) ثابت { أرجع v; }

  کينډۍ <ډول_نوم T> constexpr hb_remove_reference<T>
  چلوونکی () (T&& v) ثابت { أرجع v; }
}
HB_FUNCOBJ (hb_lidentity);
بنية
{
  /* Like identity(), but always returns rvalue. */
  کينډۍ <ډول_نوم T> constexpr hb_remove_reference<T>
  چلوونکی () (T&& v) ثابت { أرجع v; }
}
HB_FUNCOBJ (hb_ridentity);

بنية
{
  کينډۍ <ډول_نوم T> constexpr منطقي
  چلوونکی () (T&& v) ثابت { أرجع منطقي (std::forward<T> (v)); }
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
#تعریف_کړه fasthash_mix(h) (					\
			(فراغ) ((h) ^= (h) >> 23),		\
			(فراغ) ((h) *= 0x2127599bf4325c37ULL),	\
			(h) ^= (h) >> 47)

ساكن مضمن uint64_t fasthash64(ثابت فراغ *buf, size_t len, uint64_t تخم)
{
	بنية __attribute__((packed)) packed_uint64_t { uint64_t v; };
	ثابت uint64_t    m = 0x880355f21e6d1965ULL;
	ثابت packed_uint64_t *pos = (ثابت packed_uint64_t *)buf;
	ثابت packed_uint64_t *پای_ټکی = pos + (len / 8);
	ثابت بینښه حرف *pos2;
	uint64_t h = تخم ^ (len * m);
	uint64_t v;

#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
	إذا (((uintptr_t) pos & 7) == 0)
	{
	  طالما (pos != پای_ټکی)
	  {
#لارښوونه GCC diagnostic push
#لارښوونه GCC diagnostic ignored "-Wcast-align"
	    v  = * (ثابت uint64_t *) (pos++);
#لارښوونه GCC diagnostic راوباسه
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}
	وإلا
#د_که_پای
	{
	  طالما (pos != پای_ټکی)
	  {
	    v  = pos++->v;
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}

	pos2 = (ثابت بینښه حرف*)pos;
	v = 0;

	بدل (len & 7) {
	حالة 7: v ^= (uint64_t)pos2[6] << 48; HB_FALLTHROUGH;
	حالة 6: v ^= (uint64_t)pos2[5] << 40; HB_FALLTHROUGH;
	حالة 5: v ^= (uint64_t)pos2[4] << 32; HB_FALLTHROUGH;
	حالة 4: v ^= (uint64_t)pos2[3] << 24; HB_FALLTHROUGH;
	حالة 3: v ^= (uint64_t)pos2[2] << 16; HB_FALLTHROUGH;
	حالة 2: v ^= (uint64_t)pos2[1] <<  8; HB_FALLTHROUGH;
	حالة 1: v ^= (uint64_t)pos2[0];
		h ^= fasthash_mix(v);
		h *= m;
	}

	أرجع fasthash_mix(h);
}

ساكن مضمن uint32_t fasthash32(ثابت فراغ *buf, size_t len, uint32_t تخم)
{
	// the following trick converts the 64-bit hashcode to Fermat
	// residue, which shall retain information from both the higher
	// and lower parts of hashcode.
        uint64_t h = fasthash64(buf, len, تخم);
	أرجع h - (h >> 32);
}

بنية
{
  خاص:

  کينډۍ <ډول_نوم T> constexpr خپلکاری
  impl (ثابت T& v, hb_لومړیتوب<2>) ثابت HB_RETURN (uint32_t, hb_deref (v).hash ())

  // Horrible: std:hash() of integers seems to be identity in gcc / clang?!
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  //
  // For performance characteristics see:
  // https://github.com/harfbuzz/harfbuzz/pull/4228#issuecomment-1565079537
  کينډۍ <ډول_نوم T,
	    hb_enable_if (std::is_integral<T>::value && حجم (T) <= حجم (uint32_t))> constexpr خپلکاری
  impl (ثابت T& v, hb_لومړیتوب<1>) ثابت HB_RETURN (uint32_t, (uint32_t) v * 2654435761u /* Knuh's multiplicative hash */)
  کينډۍ <ډول_نوم T,
	    hb_enable_if (std::is_integral<T>::value && حجم (T) > حجم (uint32_t))> constexpr خپلکاری
  impl (ثابت T& v, hb_لومړیتوب<1>) ثابت HB_RETURN (uint32_t, (uint32_t) (v ^ (v >> 32)) * 2654435761u /* Knuth's multiplicative hash */)

  کينډۍ <ډول_نوم T,
	    hb_enable_if (std::is_floating_point<T>::value)> constexpr خپلکاری
  impl (ثابت T& v, hb_لومړیتوب<1>) ثابت HB_RETURN (uint32_t, fasthash32 (std::addressof (v), حجم (T), 0xf437ffe6))

  کينډۍ <ډول_نوم T> constexpr خپلکاری
  impl (ثابت T& v, hb_لومړیتوب<0>) ثابت HB_RETURN (uint32_t, std::hash<hb_decay<decltype (hb_deref (v))>>{} (hb_deref (v)))

  عام:

  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (ثابت T& v) ثابت HB_RETURN (uint32_t, impl (v, hb_prioritize))
}
HB_FUNCOBJ (hb_hash);


بنية
{
  خاص:

  /* Pointer-to-member-function. */
  کينډۍ <ډول_نوم Appl, ډول_نوم T, ډول_نوم ...Ts> خپلکاری
  impl (Appl&& a, hb_لومړیتوب<2>, T &&v, Ts&&... ds) ثابت HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v)).*std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  /* Pointer-to-member. */
  کينډۍ <ډول_نوم Appl, ډول_نوم T> خپلکاری
  impl (Appl&& a, hb_لومړیتوب<1>, T &&v) ثابت HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v))).*std::forward<Appl> (a))

  /* Operator(). */
  کينډۍ <ډول_نوم Appl, ډول_نوم ...Ts> خپلکاری
  impl (Appl&& a, hb_لومړیتوب<0>, Ts&&... ds) ثابت HB_AUTO_RETURN
  (hb_deref (std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  عام:

  کينډۍ <ډول_نوم Appl, ډول_نوم ...Ts> خپلکاری
  چلوونکی () (Appl&& a, Ts&&... ds) ثابت HB_AUTO_RETURN
  (
    impl (std::forward<Appl> (a),
	  hb_prioritize,
	  std::forward<Ts> (ds)...)
  )
}
HB_FUNCOBJ (hb_invoke);

کينډۍ <بینښه Pos, ډول_نوم Appl, ډول_نوم V>
بنية hb_partial_t
{
  hb_partial_t (Appl a, V v) : a (a), v (v) {}

  static_assert (Pos > 0, "");

  کينډۍ <ډول_نوم ...Ts,
	    بینښه P = Pos,
	    hb_enable_if (P == 1)> خپلکاری
  چلوونکی () (Ts&& ...ds) -> decltype (hb_invoke (hb_declval (Appl),
						   hb_declval (V),
						   hb_declval (Ts)...))
  {
    أرجع hb_invoke (std::forward<Appl> (a),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }
  کينډۍ <ډول_نوم T0, ډول_نوم ...Ts,
	    بینښه P = Pos,
	    hb_enable_if (P == 2)> خپلکاری
  چلوونکی () (T0&& d0, Ts&& ...ds) -> decltype (hb_invoke (hb_declval (Appl),
							    hb_declval (T0),
							    hb_declval (V),
							    hb_declval (Ts)...))
  {
    أرجع hb_invoke (std::forward<Appl> (a),
		      std::forward<T0> (d0),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }

  خاص:
  hb_reference_wrapper<Appl> a;
  V v;
};
کينډۍ <بینښه Pos=1, ډول_نوم Appl, ډول_نوم V>
خپلکاری hb_partial (Appl&& a, V&& v) HB_AUTO_RETURN
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
#که_تعریف_شوی _MSC_VER
/* https://github.com/harfbuzz/harfbuzz/issues/1730 */ \
#تعریف_کړه HB_PARTIALIZE(Pos) \
  کينډۍ <ډول_نوم _T> \
  decltype(خپلکاری) چلوونکی () (_T&& _v) ثابت \
  { أرجع hb_partial<Pos> (هذا, std::forward<_T> (_v)); } \
  static_assert (ریښتیا, "")
#وإلا
/* https://github.com/harfbuzz/harfbuzz/issues/1724 */
#تعریف_کړه HB_PARTIALIZE(Pos) \
  کينډۍ <ډول_نوم _T> \
  خپلکاری چلوونکی () (_T&& _v) ثابت HB_AUTO_RETURN \
  (hb_partial<Pos> (+هذا, std::forward<_T> (_v))) \
  static_assert (ریښتیا, "")
#د_که_پای


بنية
{
  خاص:

  کينډۍ <ډول_نوم Pred, ډول_نوم Val> خپلکاری
  impl (Pred&& p, Val &&v, hb_لومړیتوب<1>) ثابت HB_AUTO_RETURN
  (
    hb_deref (std::forward<Pred> (p)).has (std::forward<Val> (v))
  )

  کينډۍ <ډول_نوم Pred, ډول_نوم Val> خپلکاری
  impl (Pred&& p, Val &&v, hb_لومړیتوب<0>) ثابت HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Pred> (p),
	       std::forward<Val> (v))
  )

  عام:

  کينډۍ <ډول_نوم Pred, ډول_نوم Val> خپلکاری
  چلوونکی () (Pred&& p, Val &&v) ثابت HB_RETURN (منطقي,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_has);

بنية
{
  خاص:

  کينډۍ <ډول_نوم Pred, ډول_نوم Val> خپلکاری
  impl (Pred&& p, Val &&v, hb_لومړیتوب<1>) ثابت HB_AUTO_RETURN
  (
    hb_has (std::forward<Pred> (p),
	    std::forward<Val> (v))
  )

  کينډۍ <ډول_نوم Pred, ډول_نوم Val> خپلکاری
  impl (Pred&& p, Val &&v, hb_لومړیتوب<0>) ثابت HB_AUTO_RETURN
  (
    std::forward<Pred> (p) == std::forward<Val> (v)
  )

  عام:

  کينډۍ <ډول_نوم Pred, ډول_نوم Val> خپلکاری
  چلوونکی () (Pred&& p, Val &&v) ثابت HB_RETURN (منطقي,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_سمون);

بنية
{
  خاص:

  کينډۍ <ډول_نوم Proj, ډول_نوم Val> خپلکاری
  impl (Proj&& f, Val &&v, hb_لومړیتوب<2>) ثابت HB_AUTO_RETURN
  (
    hb_deref (std::forward<Proj> (f)).get (std::forward<Val> (v))
  )

  کينډۍ <ډول_نوم Proj, ډول_نوم Val> خپلکاری
  impl (Proj&& f, Val &&v, hb_لومړیتوب<1>) ثابت HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Proj> (f),
	       std::forward<Val> (v))
  )

  کينډۍ <ډول_نوم Proj, ډول_نوم Val> خپلکاری
  impl (Proj&& f, Val &&v, hb_لومړیتوب<0>) ثابت HB_AUTO_RETURN
  (
    std::forward<Proj> (f)[std::forward<Val> (v)]
  )

  عام:

  کينډۍ <ډول_نوم Proj, ډول_نوم Val> خپلکاری
  چلوونکی () (Proj&& f, Val &&v) ثابت HB_AUTO_RETURN
  (
    impl (std::forward<Proj> (f),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_get);

بنية
{
  خاص:

  کينډۍ <ډول_نوم T1, ډول_نوم T2> خپلکاری
  impl (T1&& v1, T2 &&v2, hb_لومړیتوب<3>) ثابت HB_AUTO_RETURN
  (
    std::forward<T2> (v2).cmp (std::forward<T1> (v1)) == 0
  )

  کينډۍ <ډول_نوم T1, ډول_نوم T2> خپلکاری
  impl (T1&& v1, T2 &&v2, hb_لومړیتوب<2>) ثابت HB_AUTO_RETURN
  (
    std::forward<T1> (v1).cmp (std::forward<T2> (v2)) == 0
  )

  کينډۍ <ډول_نوم T1, ډول_نوم T2> خپلکاری
  impl (T1&& v1, T2 &&v2, hb_لومړیتوب<1>) ثابت HB_AUTO_RETURN
  (
    std::forward<T1> (v1) == std::forward<T2> (v2)
  )

  کينډۍ <ډول_نوم T1, ډول_نوم T2> خپلکاری
  impl (T1&& v1, T2 &&v2, hb_لومړیتوب<0>) ثابت HB_AUTO_RETURN
  (
    std::forward<T2> (v2) == std::forward<T1> (v1)
  )

  عام:

  کينډۍ <ډول_نوم T1, ډول_نوم T2> خپلکاری
  چلوونکی () (T1&& v1, T2 &&v2) ثابت HB_AUTO_RETURN
  (
    impl (std::forward<T1> (v1),
	  std::forward<T2> (v2),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_equal);

بنية
{
  کينډۍ <ډول_نوم T> فراغ
  چلوونکی () (T& a, T& b) ثابت
  {
    کارول std::swap; // allow ADL
    swap (a, b);
  }
}
HB_FUNCOBJ (hb_بدلون);


کينډۍ <ډول_نوم T1, ډول_نوم T2>
بنية hb_pair_t
{
  تعريفنوع T1 لومړی_t;
  تعريفنوع T2 دوهم_t;
  تعريفنوع hb_pair_t<T1, T2> pair_t;

  کينډۍ <ډول_نوم U1 = T1, ډول_نوم U2 = T2,
	    hb_enable_if (std::is_default_constructible<U1>::value &&
			  std::is_default_constructible<U2>::value)>
  hb_pair_t () : لومړی (), second () {}
  hb_pair_t (T1 a, T2 b) : لومړی (std::forward<T1> (a)), second (std::forward<T2> (b)) {}

  کينډۍ <ډول_نوم Q1, ډول_نوم Q2,
	    hb_enable_if (hb_is_convertible (T1, Q1) &&
			  hb_is_convertible (T2, Q2))>
  چلوونکی hb_pair_t<Q1, Q2> () { أرجع hb_pair_t<Q1, Q2> (لومړی, second); }

  hb_pair_t<T1, T2> reverse () ثابت
  { أرجع hb_pair_t<T1, T2> (second, لومړی); }

  منطقي چلوونکی == (ثابت pair_t& o) ثابت { أرجع لومړی == o.لومړی && second == o.second; }
  منطقي چلوونکی != (ثابت pair_t& o) ثابت { أرجع !(*هذا == o); }
  منطقي چلوونکی < (ثابت pair_t& o) ثابت { أرجع لومړی < o.لومړی || (لومړی == o.لومړی && second < o.second); }
  منطقي چلوونکی >= (ثابت pair_t& o) ثابت { أرجع !(*هذا < o); }
  منطقي چلوونکی > (ثابت pair_t& o) ثابت { أرجع لومړی > o.لومړی || (لومړی == o.لومړی && second > o.second); }
  منطقي چلوونکی <= (ثابت pair_t& o) ثابت { أرجع !(*هذا > o); }

  ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    pair_t *a = (pair_t *) pa;
    pair_t *b = (pair_t *) pb;

    إذا (a->لومړی < b->لومړی) أرجع -1;
    إذا (a->لومړی > b->لومړی) أرجع +1;
    إذا (a->second < b->second) أرجع -1;
    إذا (a->second > b->second) أرجع +1;
    أرجع 0;
  }

  ملګری فراغ swap (hb_pair_t& a, hb_pair_t& b) noexcept
  {
    hb_بدلون (a.لومړی, b.لومړی);
    hb_بدلون (a.second, b.second);
  }


  T1 لومړی;
  T2 second;
};
کينډۍ <ډول_نوم T1, ډول_نوم T2> ساكن مضمن hb_pair_t<T1, T2>
hb_pair (T1&& a, T2&& b) { أرجع hb_pair_t<T1, T2> (a, b); }

تعريفنوع hb_pair_t<hb_codepoint_t, hb_codepoint_t> hb_codepoint_pair_t;

بنية
{
  کينډۍ <ډول_نوم Pair> constexpr ډول_نوم Pair::لومړی_t
  چلوونکی () (ثابت Pair& pair) ثابت { أرجع pair.لومړی; }
}
HB_FUNCOBJ (hb_لومړی);

بنية
{
  کينډۍ <ډول_نوم Pair> constexpr ډول_نوم Pair::دوهم_t
  چلوونکی () (ثابت Pair& pair) ثابت { أرجع pair.second; }
}
HB_FUNCOBJ (hb_دوهم);

/* Note.  In min/max impl, we can use hb_type_identity<T> for second argument.
 * However, that would silently convert between different-signedness integers.
 * Instead we accept two different types, such that compiler can err if
 * comparing integers of different signedness. */
بنية
{
  کينډۍ <ډول_نوم T, ډول_نوم T2> constexpr خپلکاری
  چلوونکی () (T&& a, T2&& b) ثابت HB_AUTO_RETURN
  (a <= b ? a : b)
}
HB_FUNCOBJ (hb_min);
بنية
{
  کينډۍ <ډول_نوم T, ډول_نوم T2> constexpr خپلکاری
  چلوونکی () (T&& a, T2&& b) ثابت HB_AUTO_RETURN
  (a >= b ? a : b)
}
HB_FUNCOBJ (hb_max);
بنية
{
  کينډۍ <ډول_نوم T, ډول_نوم T2, ډول_نوم T3> constexpr خپلکاری
  چلوونکی () (T&& x, T2&& min, T3&& max) ثابت HB_AUTO_RETURN
  (hb_min (hb_max (std::forward<T> (x), std::forward<T2> (min)), std::forward<T3> (max)))
}
HB_FUNCOBJ (hb_clamp);

/* Signed saturating arithmetic. */
کينډۍ <ډول_نوم T>
ساكن HB_ALWAYS_INLINE T
hb_saturate_جمع_کړه (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#إذا hb_has_builtin(__builtin_add_overflow)
  T پایله;
  إذا (likely (!__builtin_add_overflow (a, b, &پایله)))
    أرجع پایله;
#وإلا
  إذا (b > 0)
  {
    إذا (likely (a <= hb_int_max (T) - b))
      أرجع a + b;
  }
  وإلا إذا (likely (a >= hb_int_min (T) - b))
    أرجع a + b;
#د_که_پای

  أرجع a < 0 ? hb_int_min (T) : hb_int_max (T);
}

کينډۍ <ډول_نوم T>
ساكن HB_ALWAYS_INLINE T
hb_saturate_sub (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#إذا hb_has_builtin(__builtin_sub_overflow)
  T پایله;
  إذا (likely (!__builtin_sub_overflow (a, b, &پایله)))
    أرجع پایله;
#وإلا
  إذا (b > 0)
  {
    إذا (likely (a >= hb_int_min (T) + b))
      أرجع a - b;
  }
  وإلا إذا (likely (a <= hb_int_max (T) + b))
    أرجع a - b;
#د_که_پای

  أرجع b < 0 ? hb_int_max (T) : hb_int_min (T);
}

کينډۍ <ډول_نوم T>
ساكن HB_ALWAYS_INLINE T
hb_saturate_neg (T a)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  أرجع hb_saturate_sub ((T) 0, a);
}

/* Convert a floating-point value to integer type T, saturating to T's
 * range instead of relying on the undefined behavior of an out-of-range
 * float-to-int conversion.  NaN saturates to the minimum of T. */
کينډۍ <ډول_نوم T>
ساكن HB_ALWAYS_INLINE T
hb_clamp_منزل (مزدوج v)
{
  أرجع (T) hb_clamp (v,
		       (مزدوج) hb_int_min (T),
		       (مزدوج) hb_int_max (T));
}

کينډۍ <ډول_نوم T>
ساكن HB_ALWAYS_INLINE T
hb_clamp_منزل (عائم v)
{
  /* Widen explicitly: float cannot represent all integer bounds exactly,
   * so the clamp itself is done in double. */
  أرجع hb_clamp_منزل<T> ((مزدوج) v);
}

کينډۍ <ډول_نوم T>
ساكن HB_ALWAYS_INLINE T
hb_clamp_منزل (int64_t v)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  أرجع (T) hb_clamp (v,
		       (int64_t) hb_int_min (T),
		       (int64_t) hb_int_max (T));
}

/*
 * Bithacks.
 */

/* Return the number of 1 bits in a uint8_t; faster than hb_popcount() */
ساكن مضمن بینښه
hb_popcount8 (uint8_t v)
{
  ساكن ثابت uint8_t popcount4[16] = {
    0, 1, 1, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 2, 3, 3, 4
  };
  أرجع popcount4[v & 0xF] + popcount4[v >> 4];
}

/* Return the number of 1 bits in v. */
کينډۍ <ډول_نوم T>
ساكن مضمن بینښه صحيح
hb_popcount (T v)
{
#إذا hb_has_builtin(__builtin_popcount)
  إذا (حجم (T) <= حجم (بینښه صحيح))
    أرجع __builtin_popcount (v);
#د_که_پای

#إذا hb_has_builtin(__builtin_popcountl)
  إذا (حجم (T) <= حجم (بینښه اوږد))
    أرجع __builtin_popcountl (v);
#د_که_پای

#إذا hb_has_builtin(__builtin_popcountll)
  إذا (حجم (T) <= حجم (بینښه اوږد اوږد))
    أرجع __builtin_popcountll (v);
#د_که_پای

  إذا (حجم (T) <= 4)
  {
    /* "HACKMEM 169" */
    uint32_t y;
    y = (v >> 1) &033333333333;
    y = v - y - ((y >>1) & 033333333333);
    أرجع (((y + (y >> 3)) & 030707070707) % 077);
  }

  إذا (حجم (T) == 8)
  {
    uint64_t y = (uint64_t) v;
    y -= ((y >> 1) & 0x5555555555555555ull);
    y = (y & 0x3333333333333333ull) + (y >> 2 & 0x3333333333333333ull);
    أرجع ((y + (y >> 4)) & 0xf0f0f0f0f0f0f0full) * 0x101010101010101ull >> 56;
  }

  إذا (حجم (T) == 16)
  {
    بینښه صحيح ښويدنه = 64;
    أرجع hb_popcount<uint64_t> ((uint64_t) v) + hb_popcount ((uint64_t) (v >> ښويدنه));
  }

  أكد (0);
  أرجع 0; /* Shut up stupid compiler. */
}

/* Returns the number of bits needed to store number */
کينډۍ <ډول_نوم T>
ساكن مضمن بینښه صحيح
hb_بټ_storage (T v)
{
  إذا (unlikely (!v)) أرجع 0;

#إذا hb_has_builtin(__builtin_clz)
  إذا (حجم (T) <= حجم (بینښه صحيح))
    أرجع حجم (بینښه صحيح) * 8 - __builtin_clz (v);
#د_که_پای

#إذا hb_has_builtin(__builtin_clzl)
  إذا (حجم (T) <= حجم (بینښه اوږد))
    أرجع حجم (بینښه اوږد) * 8 - __builtin_clzl (v);
#د_که_پای

#إذا hb_has_builtin(__builtin_clzll)
  إذا (حجم (T) <= حجم (بینښه اوږد اوږد))
    أرجع حجم (بینښه اوږد اوږد) * 8 - __builtin_clzll (v);
#د_که_پای

#إذا (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  إذا (حجم (T) <= حجم (بینښه صحيح))
  {
    بینښه اوږد where;
    بټ_لټون_سرچپه (&where, v);
    أرجع 1 + where;
  }
# إذا defined(_WIN64)
  إذا (حجم (T) <= 8)
  {
    بینښه اوږد where;
    بټ_لټون_Reverse64 (&where, v);
    أرجع 1 + where;
  }
# endif
#د_که_پای

  إذا (حجم (T) <= 4)
  {
    /* "bithacks" */
    ثابت بینښه صحيح b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    ثابت بینښه صحيح S[] = {1, 2, 4, 8, 16};
    بینښه صحيح r = 0;
    لكل (صحيح i = 4; i >= 0; i--)
      إذا (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    أرجع r + 1;
  }
  إذا (حجم (T) <= 8)
  {
    /* "bithacks" */
    ثابت uint64_t b[] = {0x2ULL, 0xCULL, 0xF0ULL, 0xFF00ULL, 0xFFFF0000ULL, 0xFFFFFFFF00000000ULL};
    ثابت بینښه صحيح S[] = {1, 2, 4, 8, 16, 32};
    بینښه صحيح r = 0;
    لكل (صحيح i = 5; i >= 0; i--)
      إذا (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    أرجع r + 1;
  }
  إذا (حجم (T) == 16)
  {
    بینښه صحيح ښويدنه = 64;
    أرجع (v >> ښويدنه) ? hb_بټ_storage<uint64_t> ((uint64_t) (v >> ښويدنه)) + ښويدنه :
			  hb_بټ_storage<uint64_t> ((uint64_t) v);
  }

  أكد (0);
  أرجع 0; /* Shut up stupid compiler. */
}

/* Returns the number of zero bits in the least significant side of v */
کينډۍ <ډول_نوم T>
ساكن مضمن بینښه صحيح
hb_ctz (T v)
{
  إذا (unlikely (!v)) أرجع 8 * حجم (T);

#إذا hb_has_builtin(__builtin_ctz)
  إذا (حجم (T) <= حجم (بینښه صحيح))
    أرجع __builtin_ctz (v);
#د_که_پای

#إذا hb_has_builtin(__builtin_ctzl)
  إذا (حجم (T) <= حجم (بینښه اوږد))
    أرجع __builtin_ctzl (v);
#د_که_پای

#إذا hb_has_builtin(__builtin_ctzll)
  إذا (حجم (T) <= حجم (بینښه اوږد اوږد))
    أرجع __builtin_ctzll (v);
#د_که_پای

#إذا (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  إذا (حجم (T) <= حجم (بینښه صحيح))
  {
    بینښه اوږد where;
    بټ_لټون_Forward (&where, v);
    أرجع where;
  }
# إذا defined(_WIN64)
  إذا (حجم (T) <= 8)
  {
    بینښه اوږد where;
    بټ_لټون_Forward64 (&where, v);
    أرجع where;
  }
# endif
#د_که_پای

  إذا (حجم (T) <= 4)
  {
    /* "bithacks" */
    بینښه صحيح c = 32;
    v &= - (int32_t) v;
    إذا (v) c--;
    إذا (v & 0x0000FFFF) c -= 16;
    إذا (v & 0x00FF00FF) c -= 8;
    إذا (v & 0x0F0F0F0F) c -= 4;
    إذا (v & 0x33333333) c -= 2;
    إذا (v & 0x55555555) c -= 1;
    أرجع c;
  }
  إذا (حجم (T) <= 8)
  {
    /* "bithacks" */
    بینښه صحيح c = 64;
    v &= - (int64_t) (v);
    إذا (v) c--;
    إذا (v & 0x00000000FFFFFFFFULL) c -= 32;
    إذا (v & 0x0000FFFF0000FFFFULL) c -= 16;
    إذا (v & 0x00FF00FF00FF00FFULL) c -= 8;
    إذا (v & 0x0F0F0F0F0F0F0F0FULL) c -= 4;
    إذا (v & 0x3333333333333333ULL) c -= 2;
    إذا (v & 0x5555555555555555ULL) c -= 1;
    أرجع c;
  }
  إذا (حجم (T) == 16)
  {
    بینښه صحيح ښويدنه = 64;
    أرجع (uint64_t) v ? hb_بټ_storage<uint64_t> ((uint64_t) v) :
			  hb_بټ_storage<uint64_t> ((uint64_t) (v >> ښويدنه)) + ښويدنه;
  }

  أكد (0);
  أرجع 0; /* Shut up stupid compiler. */
}


/*
 * Tiny stuff.
 */

/* ASCII tag/character handling */
ساكن مضمن منطقي ISALPHA (بینښه حرف c)
{ أرجع (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
ساكن مضمن منطقي ISALNUM (بینښه حرف c)
{ أرجع (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }
ساكن مضمن منطقي ISSPACE (بینښه حرف c)
{ أرجع c == ' ' || c =='\f'|| c =='\n'|| c =='\r'|| c =='\t'|| c =='\v'; }
ساكن مضمن بینښه حرف TOUPPER (بینښه حرف c)
{ أرجع (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c; }
ساكن مضمن بینښه حرف TOLOWER (بینښه حرف c)
{ أرجع (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }
ساكن مضمن منطقي ISHEX (بینښه حرف c)
{ أرجع (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
ساكن مضمن بینښه حرف TOHEX (uint8_t c)
{ أرجع (c & 0xF) <= 9 ? (c & 0xF) + '0' : (c & 0xF) + 'a' - 10; }
ساكن مضمن uint8_t FROMHEX (بینښه حرف c)
{ أرجع (c >= '0' && c <= '9') ? c - '0' : TOLOWER (c) - 'a' + 10; }

ساكن مضمن بینښه صحيح DIV_CEIL (ثابت بینښه صحيح a, بینښه صحيح b)
{ أرجع (a + (b - 1)) / b; }


#تعریف_لغوه  ARRAY_اوږدتیا
کينډۍ <ډول_نوم Type, بینښه صحيح n>
ساكن مضمن بینښه صحيح ARRAY_اوږدتیا (ثابت Type (&)[n]) { أرجع n; }
/* A const version, but does not detect erratically being called on pointers. */
#تعریف_کړه ARRAY_اوږدتیا_CONST(__array) ((نښهدار صحيح) (حجم (__array) / حجم (__array[0])))


ساكن مضمن فراغ *
hb_memcpy (فراغ *__restrict dst, ثابت فراغ *__restrict src, size_t len)
{
  /* It's illegal to pass 0 as size to memcpy. */
  إذا (unlikely (!len)) أرجع dst;
  أرجع حافظه_نقل (dst, src, len);
}

ساكن مضمن صحيح
hb_memcmp (ثابت فراغ *a, ثابت فراغ *b, بینښه صحيح len)
{
  /* It's illegal to pass NULL to memcmp(), even if len is zero.
   * So, wrap it.
   * https://sourceware.org/bugzilla/show_bug.cgi?id=23878 */
  إذا (unlikely (!len)) أرجع 0;
  أرجع memcmp (a, b, len);
}

ساكن مضمن فراغ *
hb_memset (فراغ *s, صحيح c, بینښه صحيح n)
{
  /* It's illegal to pass NULL to memset(), even if n is zero. */
  إذا (unlikely (!n)) أرجع s;
  أرجع memset (s, c, n);
}

ساكن مضمن بینښه صحيح
hb_ceil_منزل_4 (بینښه صحيح v)
{
  أرجع ((v - 1) | 3) + 1;
}

کينډۍ <ډول_نوم T> ساكن مضمن منطقي
hb_دننه_range (T u, T lo, T hi)
{
  static_assert (!std::is_signed<T>::value, "");

  /* The casts below are important as if T is smaller than int,
   * the subtract results will become a signed int! */
  أرجع (T)(u - lo) <= (T)(hi - lo);
}
کينډۍ <ډول_نوم T> ساكن مضمن منطقي
hb_دننه_ranges (T u, T lo1, T hi1)
{
  أرجع hb_دننه_range (u, lo1, hi1);
}
کينډۍ <ډول_نوم T, ډول_نوم ...Ts> ساكن مضمن منطقي
hb_دننه_ranges (T u, T lo1, T hi1, Ts... ds)
{
  أرجع hb_دننه_range<T> (u, lo1, hi1) || hb_دننه_ranges<T> (u, ds...);
}


/*
 * Overflow checking.
 */

ساكن مضمن منطقي
hb_unsigned_mul_overflows (بینښه صحيح شمېر, بینښه صحيح اندازه, بینښه *پایله = تش_اشاره)
{
#إذا hb_has_builtin(__builtin_mul_overflow)
  بینښه ډېری_پایله;
  إذا (!پایله)
    پایله = &ډېری_پایله;
  أرجع __builtin_mul_overflow (شمېر, اندازه, پایله);
#د_که_پای

  إذا (پایله)
    *پایله = شمېر * اندازه;
  أرجع (اندازه > 0) && (شمېر >= ((بینښه صحيح) -1) / اندازه);
}

ساكن مضمن منطقي
hb_unsigned_جمع_کړه_overflows (بینښه صحيح a, بینښه صحيح b, بینښه *پایله = تش_اشاره)
{
#إذا hb_has_builtin(__builtin_add_overflow)
  بینښه ډېری_پایله;
  إذا (!پایله)
    پایله = &ډېری_پایله;
  أرجع __builtin_add_overflow (a, b, پایله);
#د_که_پای

  إذا (پایله)
    *پایله = a + b;
  أرجع b > (بینښه صحيح) -1 - a;
}

/* Saturating arithmetic on size_t.  On platforms where size_t is wider than
 * unsigned int (i.e. 64-bit), the inputs to get_size()-style computations
 * (counts and static_sizes which are at most 32-bit) cannot overflow size_t,
 * so these reduce to plain arithmetic.  On 32-bit platforms (size_t is
 * unsigned int), they saturate to SIZE_MAX so sanitize/serialize callers
 * naturally reject the resulting size as out-of-range. */

ساكن مضمن size_t
hb_unsigned_mul_saturate (size_t a, size_t b)
{
  إذا (حجم (size_t) > حجم (بینښه صحيح))
    أرجع a * b;
#إذا hb_has_builtin(__builtin_mul_overflow)
  size_t پایله;
  إذا (__builtin_mul_overflow (a, b, &پایله))
    أرجع (size_t) -1;
  أرجع پایله;
#وإلا
  إذا (b > 0 && a > ((size_t) -1) / b) أرجع (size_t) -1;
  أرجع a * b;
#د_که_پای
}

ساكن مضمن size_t
hb_unsigned_جمع_کړه_saturate (size_t a, size_t b)
{
  إذا (حجم (size_t) > حجم (بینښه صحيح))
    أرجع a + b;
#إذا hb_has_builtin(__builtin_add_overflow)
  size_t پایله;
  إذا (__builtin_add_overflow (a, b, &پایله))
    أرجع (size_t) -1;
  أرجع پایله;
#وإلا
  إذا (b > ((size_t) -1) - a) أرجع (size_t) -1;
  أرجع a + b;
#د_که_پای
}

/* Variadic forms: fold left across all arguments. */
کينډۍ <ډول_نوم ...Ts>
ساكن مضمن size_t
hb_unsigned_mul_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ أرجع hb_unsigned_mul_saturate (hb_unsigned_mul_saturate (a, b), c, rest...); }

کينډۍ <ډول_نوم ...Ts>
ساكن مضمن size_t
hb_unsigned_جمع_کړه_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ أرجع hb_unsigned_جمع_کړه_saturate (hb_unsigned_جمع_کړه_saturate (a, b), c, rest...); }

/* Saturating mul-add: a * b + c.  Covers the dominant get_size() pattern
 * `count * static_size + min_size`. */
ساكن مضمن size_t
hb_unsigned_mul_جمع_کړه_saturate (size_t a, size_t b, size_t c)
{ أرجع hb_unsigned_جمع_کړه_saturate (hb_unsigned_mul_saturate (a, b), c); }


/*
 * Sort and search.
 */

کينډۍ <ډول_نوم K, ډول_نوم V, ډول_نوم ...Ts>
ساكن صحيح
hb_cmp_لاره (ثابت فراغ *pkey, ثابت فراغ *pval, Ts... ds)
{
  ثابت K& key = * (ثابت K*) pkey;
  ثابت V& val = * (ثابت V*) pval;

  أرجع val.cmp (key, ds...);
}

کينډۍ <ډول_نوم K, ډول_نوم V>
ساكن صحيح
_hb_cmp_operator (ثابت فراغ *pkey, ثابت فراغ *pval)
{
  ثابت K& key = * (ثابت K*) pkey;
  ثابت V& val = * (ثابت V*) pval;

  إذا (key < val) أرجع -1;
  إذا (key > val) أرجع  1;
  أرجع 0;
}

کينډۍ <ډول_نوم V, ډول_نوم K, ډول_نوم ...Ts>
HB_HOT
ساكن مضمن منطقي
hb_bsearch_impl (بینښه *pos, /* Out */
		 ثابت K& key,
		 V* بنسټ, size_t nmemb, size_t stride,
		 صحيح (*compar)(ثابت فراغ *کلی_3, ثابت فراغ *توکی_2, Ts... _ds),
		 Ts... ds)
{
  /* This is our *only* bsearch implementation. */

  صحيح min = 0, max = (صحيح) nmemb - 1;
  طالما (min <= max)
  {
    صحيح mid = ((بینښه صحيح) min + (بینښه صحيح) max) / 2;
#لارښوونه GCC diagnostic push
#لارښوونه GCC diagnostic ignored "-Wcast-align"
    V* p = (V*) (((ثابت حرف *) بنسټ) + (mid * stride));
#لارښوونه GCC diagnostic راوباسه
    صحيح c = compar ((ثابت فراغ *) std::addressof (key), (ثابت فراغ *) p, ds...);
    إذا (c < 0)
      max = mid - 1;
    وإلا إذا (c > 0)
      min = mid + 1;
    وإلا
    {
      *pos = mid;
      أرجع ریښتیا;
    }
  }
  *pos = min;
  أرجع خطأ;
}

کينډۍ <ډول_نوم V, ډول_نوم K>
ساكن مضمن V*
hb_bsearch (ثابت K& key, V* بنسټ,
	    size_t nmemb, size_t stride = حجم (V),
	    صحيح (*compar)(ثابت فراغ *کلی_3, ثابت فراغ *توکی_2) = hb_cmp_لاره<K, V>)
{
  بینښه pos;
#لارښوونه GCC diagnostic push
#لارښوونه GCC diagnostic ignored "-Wcast-align"
  أرجع hb_bsearch_impl (&pos, key, بنسټ, nmemb, stride, compar) ?
	 (V*) (((ثابت حرف *) بنسټ) + (pos * stride)) : تش_اشاره;
#لارښوونه GCC diagnostic راوباسه
}
کينډۍ <ډول_نوم V, ډول_نوم K, ډول_نوم ...Ts>
ساكن مضمن V*
hb_bsearch (ثابت K& key, V* بنسټ,
	    size_t nmemb, size_t stride,
	    صحيح (*compar)(ثابت فراغ *کلی_3, ثابت فراغ *توکی_2, Ts... _ds),
	    Ts... ds)
{
  بینښه pos;
#لارښوونه GCC diagnostic push
#لارښوونه GCC diagnostic ignored "-Wcast-align"
  أرجع hb_bsearch_impl (&pos, key, بنسټ, nmemb, stride, compar, ds...) ?
	 (V*) (((ثابت حرف *) بنسټ) + (pos * stride)) : تش_اشاره;
#لارښوونه GCC diagnostic راوباسه
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
کينډۍ <ډول_نوم T, ډول_نوم Compar>
ساكن مضمن فراغ
hb_qsort_loop (T *بنسټ, size_t nel, Compar compar)
{
  طالما (nel > 24)
  {
    T *وروستی = بنسټ + nel - 1;
    T *mid = بنسټ + nel / 2;

    /* Median-of-three pivot, parked at last-1. */
    إذا (compar (*بنسټ, *mid) > 0) hb_بدلون (*بنسټ, *mid);
    إذا (compar (*mid, *وروستی) > 0)
    {
      hb_بدلون (*mid, *وروستی);
      إذا (compar (*بنسټ, *mid) > 0) hb_بدلون (*بنسټ, *mid);
    }
    hb_بدلون (*mid, *(وروستی - 1));
    T &محور = *(وروستی - 1);

    /* Two-way Hoare partition.  Inner loops are unguarded:
     * median-of-three left *base <= pivot and *last >= pivot,
     * which act as sentinels. */
    T *i = بنسټ, *j = وروستی - 1;
    طالما (ریښتیا)
    {
      طالما (compar (*++i, محور) < 0) {}
      طالما (compar (*--j, محور) > 0) {}
      إذا (i >= j) اكسر;
      hb_بدلون (*i, *j);
    }
    hb_بدلون (*i, *(وروستی - 1));

    /* Recurse on smaller side, loop on larger — bounds stack
     * depth at O(log n). */
    size_t کیڼ  = (size_t) (i - بنسټ);
    size_t ښی = nel - کیڼ - 1;
    إذا (کیڼ < ښی)
    {
      hb_qsort_loop (بنسټ, کیڼ, compar);
      بنسټ = i + 1;
      nel  = ښی;
    }
    وإلا
    {
      hb_qsort_loop (i + 1, ښی, compar);
      nel  = کیڼ;
    }
  }
}

کينډۍ <ډول_نوم T, ډول_نوم Compar>
ساكن مضمن فراغ
hb_qsort_inline (T *بنسټ, size_t nel, Compar compar)
{
  hb_qsort_loop (بنسټ, nel, compar);

  /* Single final insertion sort over the whole array.  After
   * the partitioning loop, every element is within the threshold
   * of its sorted position, so this pass is O(n * threshold). */
  T *پای_ټکی = بنسټ + nel;
  لكل (T *pi = بنسټ + 1; pi < پای_ټکی; pi++)
    لكل (T *pj = pi; pj > بنسټ && compar (pj[-1], pj[0]) > 0; pj--)
      hb_بدلون (pj[-1], pj[0]);
}

ساكن مضمن فراغ
hb_qsort (فراغ *بنسټ, size_t nel, size_t پلنوالی,
	  صحيح (*compar)(ثابت فراغ *_a, ثابت فراغ *_b))
{
  qsort (بنسټ, nel, پلنوالی, compar);
}




کينډۍ <ډول_نوم T, ډول_نوم T2, ډول_نوم T3 = صحيح> ساكن مضمن فراغ
hb_stable_ترتیبول (T *array, بینښه صحيح len, صحيح(*compar)(ثابت T2 *, ثابت T2 *), T3 *array2 = تش_اشاره)
{
  static_assert (hb_is_trivially_copy_assignable (T), "");
  static_assert (hb_is_trivially_copy_assignable (T3), "");

  لكل (بینښه صحيح i = 1; i < len; i++)
  {
    بینښه صحيح j = i;
    طالما (j && compar (&array[j - 1], &array[i]) > 0)
      j--;
    إذا (i == j)
      استمر;
    /* Move item i to occupy place for item j, shift what's in between. */
    {
      T t = array[i];
      memmove (&array[j + 1], &array[j], (i - j) * حجم (T));
      array[j] = t;
    }
    إذا (array2)
    {
      T3 t = array2[i];
      memmove (&array2[j + 1], &array2[j], (i - j) * حجم (T3));
      array2[j] = t;
    }
  }
}

ساكن مضمن hb_bool_t
hb_codepoint_parse (ثابت حرف *s, بینښه صحيح len, صحيح بنسټ, hb_codepoint_t *بهر)
{
  بینښه صحيح v;
  ثابت حرف *p = s;
  ثابت حرف *پای_ټکی = p + len;
  إذا (unlikely (!hb_parse_uint (&p, پای_ټکی, &v, ریښتیا/* whole buffer */, بنسټ)))
    أرجع خطأ;

  *بهر = v;
  أرجع ریښتیا;
}


/* Operators. */

بنية
{ HB_PARTIALIZE(2);
  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (a & b)
}
HB_FUNCOBJ (hb_bitwise_and);
بنية
{ HB_PARTIALIZE(2);
  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (a | b)
}
HB_FUNCOBJ (hb_bitwise_or);
بنية
{ HB_PARTIALIZE(2);
  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (a ^ b)
}
HB_FUNCOBJ (hb_bitwise_xor);
بنية
{ HB_PARTIALIZE(2);
  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (~a & b)
}
HB_FUNCOBJ (hb_bitwise_lt);
بنية
{ HB_PARTIALIZE(2);
  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (a & ~b)
}
HB_FUNCOBJ (hb_bitwise_gt); // aka sub
بنية
{ HB_PARTIALIZE(2);
  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (~a | b)
}
HB_FUNCOBJ (hb_bitwise_le);
بنية
{ HB_PARTIALIZE(2);
  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (a | ~b)
}
HB_FUNCOBJ (hb_bitwise_ge);
بنية
{
  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (ثابت T &a) ثابت HB_AUTO_RETURN (~a)
}
HB_FUNCOBJ (hb_bitwise_neg);

بنية
{ HB_PARTIALIZE(2);
  کينډۍ <ډول_نوم T, ډول_نوم T2> constexpr خپلکاری
  چلوونکی () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (a + b)
}
HB_FUNCOBJ (hb_جمع_کړه);
بنية
{ HB_PARTIALIZE(2);
  کينډۍ <ډول_نوم T, ډول_نوم T2> constexpr خپلکاری
  چلوونکی () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (a - b)
}
HB_FUNCOBJ (hb_sub);
بنية
{ HB_PARTIALIZE(2);
  کينډۍ <ډول_نوم T, ډول_نوم T2> constexpr خپلکاری
  چلوونکی () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (b - a)
}
HB_FUNCOBJ (hb_rsub);
بنية
{ HB_PARTIALIZE(2);
  کينډۍ <ډول_نوم T, ډول_نوم T2> constexpr خپلکاری
  چلوونکی () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (a * b)
}
HB_FUNCOBJ (hb_mul);
بنية
{ HB_PARTIALIZE(2);
  کينډۍ <ډول_نوم T, ډول_نوم T2> constexpr خپلکاری
  چلوونکی () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (a / b)
}
HB_FUNCOBJ (hb_div);
بنية
{ HB_PARTIALIZE(2);
  کينډۍ <ډول_نوم T, ډول_نوم T2> constexpr خپلکاری
  چلوونکی () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (a % b)
}
HB_FUNCOBJ (hb_mod);
بنية
{
  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (ثابت T &a) ثابت HB_AUTO_RETURN (+a)
}
HB_FUNCOBJ (hb_pos);
بنية
{
  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (ثابت T &a) ثابت HB_AUTO_RETURN (-a)
}
HB_FUNCOBJ (hb_neg);
بنية
{
  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (T &a) ثابت HB_AUTO_RETURN (++a)
}
HB_FUNCOBJ (hb_inc);
بنية
{
  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (T &a) ثابت HB_AUTO_RETURN (--a)
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
کينډۍ <ډول_نوم func_t>
مزدوج solve_itp (func_t f,
		  مزدوج a, مزدوج b,
		  مزدوج epsilon,
		  مزدوج min_y, مزدوج max_y,
		  مزدوج &ya, مزدوج &yb, مزدوج &y)
{
  // Guard against degenerate interval
  إذا (b - a <= 0.0)
  {
    y = ya;
    أرجع a;
  }

  بینښه n1_2 = (بینښه) (hb_max (ceil (log2 ((b - a) / epsilon)) - 1.0, 0.0));
  ثابت بینښه n0 = 1; // Hardwired
  ثابت مزدوج k1 = 0.2 / (b - a); // Hardwired.
  بینښه nmax = n0 + n1_2;
  مزدوج scaled_epsilon = epsilon * مزدوج (1llu << nmax);
  مزدوج _2_epsilon = 2.0 * epsilon;
  طالما (b - a > _2_epsilon)
  {
    مزدوج x1_2 = 0.5 * (a + b);
    مزدوج r = scaled_epsilon - 0.5 * (b - a);
    // Guard against yb == ya to prevent division by zero
    مزدوج xf = (yb != ya) ? (yb * a - ya * b) / (yb - ya) : x1_2;
    مزدوج sigma = x1_2 - xf;
    مزدوج b_a = b - a;
    // This has k2 = 2 hardwired for efficiency.
    مزدوج b_a_k2 = b_a * b_a;
    مزدوج delta = k1 * b_a_k2;
    صحيح sigma_sign = sigma >= 0 ? +1 : -1;
    مزدوج xt = delta <= fabs (x1_2 - xf) ? xf + delta * sigma_sign : x1_2;
    مزدوج xitp = fabs (xt - x1_2) <= r ? xt : x1_2 - r * sigma_sign;
    مزدوج yitp = f (xitp);
    إذا (yitp > max_y)
    {
      b = xitp;
      yb = yitp;
    }
    وإلا إذا (yitp < min_y)
    {
      a = xitp;
      ya = yitp;
    }
    وإلا
    {
      y = yitp;
      أرجع xitp;
    }
    scaled_epsilon *= 0.5;
  }
  أرجع 0.5 * (a + b);
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
کينډۍ <ډول_نوم F>
بنية hb_scope_guard_t
{
  څرګند hb_scope_guard_t (F &&f) : f (std::move (f)), فعال (ریښتیا) {}
  hb_scope_guard_t (hb_scope_guard_t &&o) noexcept
    : f (std::move (o.f)), فعال (o.فعال) { o.فعال = خطأ; }
  hb_scope_guard_t (ثابت hb_scope_guard_t &) = ړنګول;
  hb_scope_guard_t &چلوونکی= (ثابت hb_scope_guard_t &) = ړنګول;
  hb_scope_guard_t &چلوونکی= (hb_scope_guard_t &&) = ړنګول;
  ~hb_scope_guard_t () { إذا (فعال) f (); }

  /* Release: dismiss the guard so the cleanup does NOT run.  Use
   * when transferring ownership out of the scope. */
  فراغ release () { فعال = خطأ; }

  خاص:
  F f;
  منطقي فعال;
};

کينډۍ <ډول_نوم F>
ساكن مضمن hb_scope_guard_t<F> hb_make_scope_guard (F &&f)
{ أرجع hb_scope_guard_t<F> (std::forward<F> (f)); }

#تعریف_کړه HB_SCOPE_GUARD_NAME_(کرښه) hb_scope_guard_##کرښه
#تعریف_کړه HB_SCOPE_GUARD_NAME(کرښه) HB_SCOPE_GUARD_NAME_(کرښه)
#تعریف_کړه HB_SCOPE_GUARD(stmt) \
  خپلکاری HB_SCOPE_GUARD_NAME(__LINE__) = \
    hb_make_scope_guard ([&]() { stmt; }); \
  (فراغ) HB_SCOPE_GUARD_NAME(__LINE__)


#د_که_پای /* HB_ALGS_HH */
