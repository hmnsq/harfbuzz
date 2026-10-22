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

#اگر_تعریف_نشده HB_ALGS_HH
#تعریف_کن HB_ALGS_HH

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
  إذا (اندازه && nmemb > SIZE_MAX / اندازه) أرجع اشاره_تهی;
  أرجع hb_malloc (nmemb * اندازه);
}
ساكن HB_ALWAYS_INLINE فراغ *
hb_realloc2 (فراغ *ptr, size_t nmemb, size_t اندازه)
{
  إذا (اندازه && nmemb > SIZE_MAX / اندازه) أرجع اشاره_تهی;
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
#اگر_تعریف_شده _MSC_VER
# pragma warning(disable:4200)
# pragma warning(disable:4800)
#پایان_اگر
#تعریف_کن HB_MARK_AS_FLAG_T(T) \
	خارجي "C++" { \
	  ساكن مضمن constexpr T عملگر | (T l, T r) { أرجع T ((بیعلامت) l | (بیعلامت) r); } \
	  ساكن مضمن constexpr T عملگر & (T l, T r) { أرجع T ((بیعلامت) l & (بیعلامت) r); } \
	  ساكن مضمن constexpr T عملگر ^ (T l, T r) { أرجع T ((بیعلامت) l ^ (بیعلامت) r); } \
	  ساكن مضمن constexpr بیعلامت عملگر ~ (T r) { أرجع (~(بیعلامت) r); } \
	  ساكن مضمن T& عملگر |= (T &l, T r) { l = l | r; أرجع l; } \
	  ساكن مضمن T& عملگر &= (T& l, T r) { l = l & r; أرجع l; } \
	  ساكن مضمن T& عملگر ^= (T& l, T r) { l = l ^ r; أرجع l; } \
	} \
	static_assert (درست, "")

/* Useful for set-operations on small enums.
 * For example, for testing "x ∈ {x1, x2, x3}" use:
 * (FLAG_UNSAFE(x) & (FLAG(x1) | FLAG(x2) | FLAG(x3)))
 */
#تعریف_کن FLAG(x) (static_assert_عبارت ((بیعلامت)(x) < 32) + (((uint32_t) 1U) << (بیعلامت)(x)))
#تعریف_کن FLAG_UNSAFE(x) ((بیعلامت)(x) < 32 ? (((uint32_t) 1U) << (بیعلامت)(x)) : 0)
#تعریف_کن FLAG_RANGE(x,y) (static_assert_عبارت ((x) < (y)) + FLAG(y+1) - FLAG(x))
#تعریف_کن FLAG64(x) (static_assert_عبارت ((بیعلامت)(x) < 64) + (((uint64_t) 1ULL) << (بیعلامت)(x)))
#تعریف_کن FLAG64_UNSAFE(x) ((بیعلامت)(x) < 64 ? (((uint64_t) 1ULL) << (بیعلامت)(x)) : 0)


/*
 * Fixed-endian integers / floats.
 */


/* Endian swap, used in Windows related backends */
ساكن مضمن constexpr uint16_t hb_uint16_جابجا (uint16_t v)
{ أرجع (v >> 8) | (v << 8); }
ساكن مضمن constexpr uint32_t hb_uint32_جابجا (uint32_t v)
{ أرجع (hb_uint16_جابجا (v) << 16) | hb_uint16_جابجا (v >> 16); }

قالب <نام_نوع Type>
بنية __attribute__((packed)) hb_packed_t
{
  hb_packed_t () = افتراضي;
  constexpr hb_packed_t (Type V) : v (V) {}
  عملگر Type () ثابت { أرجع v; }
  hb_packed_t & عملگر = (Type V) { v = V; أرجع *هذا; }

  خاص:
  Type v;
};

#اگر_تعریف_نشده HB_FAST_رقم_ACCESS

#إذا defined(__OPTIMIZE__) && \
    defined(__BYTE_ORDER) && \
    (__BYTE_ORDER == __BIG_ENDIAN || \
     (__BYTE_ORDER == __LITTLE_ENDIAN && \
      hb_has_builtin(__builtin_bswap16) && \
      hb_has_builtin(__builtin_bswap32) && \
      hb_has_builtin(__builtin_bswap64)))
#تعریف_کن HB_FAST_رقم_ACCESS 1
#وإلا
#تعریف_کن HB_FAST_رقم_ACCESS 0
#پایان_اگر

// https://github.com/harfbuzz/harfbuzz/issues/5456
#إذا defined(__GNUC__) && !defined(__clang__) && (__GNUC__ <= 12)
#لغو_تعریف HB_FAST_رقم_ACCESS
#تعریف_کن HB_FAST_رقم_ACCESS 0
#پایان_اگر

#پایان_اگر

قالب <منطقي BE, نام_نوع Type, صحيح بایتها_3 = حجم (Type)>
بنية HBInt;
قالب <منطقي BE, نام_نوع Type>
بنية HBInt<BE, Type, 1>
{
  عام:
  HBInt () = افتراضي;
  constexpr HBInt (Type V) : v {uint8_t (V)} {}
  constexpr عملگر Type () ثابت { أرجع v; }
  خاص: uint8_t v;
};
قالب <منطقي BE, نام_نوع Type>
بنية HBInt<BE, Type, 2>
{
  عام:
  HBInt () = افتراضي;

  HBInt (Type V)
#إذا HB_FAST_رقم_ACCESS
  {
    إذا (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint16_t> *) v) = V;
    وإلا
      *((hb_packed_t<uint16_t> *) v) = __builtin_bswap16 (V);
  }
#وإلا
    : v {BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >>  8) & 0xFF)} {}
#پایان_اگر

  constexpr عملگر Type () ثابت
  {
#إذا HB_FAST_رقم_ACCESS
    أرجع (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint16_t) *((ثابت hb_packed_t<uint16_t> *) v)
    :
      __builtin_bswap16 ((uint16_t) *((ثابت hb_packed_t<uint16_t> *) v))
    ;
#وإلا
    أرجع (BE ? (v[0] <<  8) : (v[0]      ))
	 + (BE ? (v[1]      ) : (v[1] <<  8));
#پایان_اگر
  }
  خاص: uint8_t v[2];
};
قالب <منطقي BE, نام_نوع Type>
بنية HBInt<BE, Type, 3>
{
  static_assert (!std::is_signed<Type>::value, "");
  عام:
  HBInt () = افتراضي;
  constexpr HBInt (Type V) : v {BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
				BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
				BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V      ) & 0xFF)} {}

  constexpr عملگر Type () ثابت { أرجع (BE ? (v[0] << 16) : (v[0]      ))
					  + (BE ? (v[1] <<  8) : (v[1] <<  8))
					  + (BE ? (v[2]      ) : (v[2] << 16)); }
  خاص: uint8_t v[3];
};
قالب <منطقي BE, نام_نوع Type>
بنية HBInt<BE, Type, 4>
{
  قالب <منطقي, نام_نوع, صحيح>
  دوست بنية HBFloat;

  عام:
  HBInt () = افتراضي;

  HBInt (Type V)
#إذا HB_FAST_رقم_ACCESS
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
#پایان_اگر

  constexpr عملگر Type () ثابت {
#إذا HB_FAST_رقم_ACCESS
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
#پایان_اگر
  }
  خاص: uint8_t v[4];
};
قالب <منطقي BE, نام_نوع Type>
بنية HBInt<BE, Type, 8>
{
  قالب <منطقي, نام_نوع, صحيح>
  دوست بنية HBFloat;

  عام:
  HBInt () = افتراضي;

  HBInt (Type V)
#إذا HB_FAST_رقم_ACCESS
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
#پایان_اگر

  constexpr عملگر Type () ثابت {
#إذا HB_FAST_رقم_ACCESS
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
#پایان_اگر
  }
  خاص: uint8_t v[8];
};

/* Floats. */

قالب <منطقي BE, نام_نوع Type, صحيح بایتها_3>
بنية HBFloat
{
  استفاده IntType = نام_نوع std::conditional<بایتها_3 == 4, uint32_t, uint64_t>::type;

  عام:
  HBFloat () = افتراضي;

  HBFloat (Type V)
  {
#إذا HB_FAST_رقم_ACCESS
    {
	      إذا (BE == (__BYTE_ORDER == __BIG_ENDIAN))
	      {
	        *((hb_packed_t<Type> *) v) = V;
	        أرجع;
	      }
	    }
#پایان_اگر

    اتحاد {
      hb_packed_t<Type> f;
      hb_packed_t<IntType> i;
    } u = {{V}};

    ثابت HBInt<BE, IntType> I = (IntType) u.i;
    لكل (بیعلامت i = 0; i < بایتها_3; i++)
      v[i] = I.v[i];
  }

  /* c++14 constexpr */ عملگر Type () ثابت
  {
#إذا HB_FAST_رقم_ACCESS
	    {
	      إذا (BE == (__BYTE_ORDER == __BIG_ENDIAN))
		أرجع (Type) *((ثابت hb_packed_t<Type> *) v);
	    }
#پایان_اگر

    HBInt<BE, IntType> I;
    لكل (بیعلامت i = 0; i < بایتها_3; i++)
      I.v[i] = v[i];

    اتحاد {
      hb_packed_t<IntType> i;
      hb_packed_t<Type> f;
    } u = {{I}};

    أرجع (Type) u.f;
  }
  خاص: uint8_t v[بایتها_3];
};


/* We want our rounding towards +infinity. */
ساكن مضمن مزدوج
_hb_roundf (مزدوج x) { أرجع floor (x + .5); }

ساكن مضمن عائم
_hb_roundf (عائم x) { أرجع floor ((مزدوج) x + .5); }

#تعریف_کن roundf(x) _hb_roundf(x)

ساكن مضمن فراغ
hb_sincos (عائم rotation, عائم &s, عائم &c)
{
#اگر_تعریف_شده HAVE_SINCOSF
  sincosf (rotation, &s, &c);
#وإلا
  c = cosf (rotation);
  s = sinf (rotation);
#پایان_اگر
}
ساكن مضمن فراغ
hb_sincos (مزدوج rotation, مزدوج &s, مزدوج &c)
{
#اگر_تعریف_شده HAVE_SINCOS
  sincos (rotation, &s, &c);
#وإلا
  c = کسینوس (rotation);
  s = سینوس (rotation);
#پایان_اگر
}


/* Encodes three unsigned integers in one 64-bit number.  If the inputs have more than 21 bits,
 * values will be truncated / overlap, and might not decode exactly. */
#تعریف_کن HB_CODEPOINT_ENCODE3(x,y,z) (((uint64_t) (x) << 42) | ((uint64_t) (y) << 21) | (uint64_t) (z))
#تعریف_کن HB_CODEPOINT_DECODE3_1(v) ((hb_codepoint_t) ((v) >> 42))
#تعریف_کن HB_CODEPOINT_DECODE3_2(v) ((hb_codepoint_t) ((v) >> 21) & 0x1FFFFFu)
#تعریف_کن HB_CODEPOINT_DECODE3_3(v) ((hb_codepoint_t) (v) & 0x1FFFFFu)

/* Custom encoding used by hb-ucd. */
#تعریف_کن HB_CODEPOINT_ENCODE3_11_7_14(x,y,z) (((uint32_t) ((x) & 0x07FFu) << 21) | (((uint32_t) (y) & 0x007Fu) << 14) | (uint32_t) ((z) & 0x3FFFu))
#تعریف_کن HB_CODEPOINT_DECODE3_11_7_14_1(v) ((hb_codepoint_t) ((v) >> 21))
#تعریف_کن HB_CODEPOINT_DECODE3_11_7_14_2(v) ((hb_codepoint_t) (((v) >> 14) & 0x007Fu) | 0x0300)
#تعریف_کن HB_CODEPOINT_DECODE3_11_7_14_3(v) ((hb_codepoint_t) (v) & 0x3FFFu)


بنية
{
  /* Note.  This is dangerous in that if it's passed an rvalue, it returns rvalue-reference. */
  قالب <نام_نوع T> constexpr خودکار
  عملگر () (T&& v) ثابت HB_AUTO_RETURN ( std::forward<T> (v) )
}
HB_FUNCOBJ (hb_عنصر_خنثی);
بنية
{
  /* Like identity(), but only retains lvalue-references.  Rvalues are returned as rvalues. */
  قالب <نام_نوع T> constexpr T&
  عملگر () (T& v) ثابت { أرجع v; }

  قالب <نام_نوع T> constexpr hb_remove_reference<T>
  عملگر () (T&& v) ثابت { أرجع v; }
}
HB_FUNCOBJ (hb_lidentity);
بنية
{
  /* Like identity(), but always returns rvalue. */
  قالب <نام_نوع T> constexpr hb_remove_reference<T>
  عملگر () (T&& v) ثابت { أرجع v; }
}
HB_FUNCOBJ (hb_ridentity);

بنية
{
  قالب <نام_نوع T> constexpr منطقي
  عملگر () (T&& v) ثابت { أرجع منطقي (std::forward<T> (v)); }
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
#تعریف_کن fasthash_mix(h) (					\
			(فراغ) ((h) ^= (h) >> 23),		\
			(فراغ) ((h) *= 0x2127599bf4325c37ULL),	\
			(h) ^= (h) >> 47)

ساكن مضمن uint64_t fasthash64(ثابت فراغ *buf, size_t len, uint64_t دانه)
{
	بنية __attribute__((packed)) packed_uint64_t { uint64_t v; };
	ثابت uint64_t    m = 0x880355f21e6d1965ULL;
	ثابت packed_uint64_t *pos = (ثابت packed_uint64_t *)buf;
	ثابت packed_uint64_t *انتها = pos + (len / 8);
	ثابت بیعلامت حرف *pos2;
	uint64_t h = دانه ^ (len * m);
	uint64_t v;

#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
	إذا (((uintptr_t) pos & 7) == 0)
	{
	  طالما (pos != انتها)
	  {
#رهنمود GCC diagnostic push
#رهنمود GCC diagnostic ignored "-Wcast-align"
	    v  = * (ثابت uint64_t *) (pos++);
#رهنمود GCC diagnostic برداشت
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}
	وإلا
#پایان_اگر
	{
	  طالما (pos != انتها)
	  {
	    v  = pos++->v;
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}

	pos2 = (ثابت بیعلامت حرف*)pos;
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

ساكن مضمن uint32_t fasthash32(ثابت فراغ *buf, size_t len, uint32_t دانه)
{
	// the following trick converts the 64-bit hashcode to Fermat
	// residue, which shall retain information from both the higher
	// and lower parts of hashcode.
        uint64_t h = fasthash64(buf, len, دانه);
	أرجع h - (h >> 32);
}

بنية
{
  خاص:

  قالب <نام_نوع T> constexpr خودکار
  impl (ثابت T& v, hb_اولویت<2>) ثابت HB_RETURN (uint32_t, hb_deref (v).hash ())

  // Horrible: std:hash() of integers seems to be identity in gcc / clang?!
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  //
  // For performance characteristics see:
  // https://github.com/harfbuzz/harfbuzz/pull/4228#issuecomment-1565079537
  قالب <نام_نوع T,
	    hb_enable_if (std::is_integral<T>::value && حجم (T) <= حجم (uint32_t))> constexpr خودکار
  impl (ثابت T& v, hb_اولویت<1>) ثابت HB_RETURN (uint32_t, (uint32_t) v * 2654435761u /* Knuh's multiplicative hash */)
  قالب <نام_نوع T,
	    hb_enable_if (std::is_integral<T>::value && حجم (T) > حجم (uint32_t))> constexpr خودکار
  impl (ثابت T& v, hb_اولویت<1>) ثابت HB_RETURN (uint32_t, (uint32_t) (v ^ (v >> 32)) * 2654435761u /* Knuth's multiplicative hash */)

  قالب <نام_نوع T,
	    hb_enable_if (std::is_floating_point<T>::value)> constexpr خودکار
  impl (ثابت T& v, hb_اولویت<1>) ثابت HB_RETURN (uint32_t, fasthash32 (std::addressof (v), حجم (T), 0xf437ffe6))

  قالب <نام_نوع T> constexpr خودکار
  impl (ثابت T& v, hb_اولویت<0>) ثابت HB_RETURN (uint32_t, std::hash<hb_decay<decltype (hb_deref (v))>>{} (hb_deref (v)))

  عام:

  قالب <نام_نوع T> constexpr خودکار
  عملگر () (ثابت T& v) ثابت HB_RETURN (uint32_t, impl (v, hb_prioritize))
}
HB_FUNCOBJ (hb_hash);


بنية
{
  خاص:

  /* Pointer-to-member-function. */
  قالب <نام_نوع Appl, نام_نوع T, نام_نوع ...Ts> خودکار
  impl (Appl&& a, hb_اولویت<2>, T &&v, Ts&&... ds) ثابت HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v)).*std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  /* Pointer-to-member. */
  قالب <نام_نوع Appl, نام_نوع T> خودکار
  impl (Appl&& a, hb_اولویت<1>, T &&v) ثابت HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v))).*std::forward<Appl> (a))

  /* Operator(). */
  قالب <نام_نوع Appl, نام_نوع ...Ts> خودکار
  impl (Appl&& a, hb_اولویت<0>, Ts&&... ds) ثابت HB_AUTO_RETURN
  (hb_deref (std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  عام:

  قالب <نام_نوع Appl, نام_نوع ...Ts> خودکار
  عملگر () (Appl&& a, Ts&&... ds) ثابت HB_AUTO_RETURN
  (
    impl (std::forward<Appl> (a),
	  hb_prioritize,
	  std::forward<Ts> (ds)...)
  )
}
HB_FUNCOBJ (hb_invoke);

قالب <بیعلامت Pos, نام_نوع Appl, نام_نوع V>
بنية hb_partial_t
{
  hb_partial_t (Appl a, V v) : a (a), v (v) {}

  static_assert (Pos > 0, "");

  قالب <نام_نوع ...Ts,
	    بیعلامت P = Pos,
	    hb_enable_if (P == 1)> خودکار
  عملگر () (Ts&& ...ds) -> decltype (hb_invoke (hb_declval (Appl),
						   hb_declval (V),
						   hb_declval (Ts)...))
  {
    أرجع hb_invoke (std::forward<Appl> (a),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }
  قالب <نام_نوع T0, نام_نوع ...Ts,
	    بیعلامت P = Pos,
	    hb_enable_if (P == 2)> خودکار
  عملگر () (T0&& d0, Ts&& ...ds) -> decltype (hb_invoke (hb_declval (Appl),
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
قالب <بیعلامت Pos=1, نام_نوع Appl, نام_نوع V>
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
#اگر_تعریف_شده _MSC_VER
/* https://github.com/harfbuzz/harfbuzz/issues/1730 */ \
#تعریف_کن HB_PARTIALIZE(Pos) \
  قالب <نام_نوع _T> \
  decltype(خودکار) عملگر () (_T&& _v) ثابت \
  { أرجع hb_partial<Pos> (هذا, std::forward<_T> (_v)); } \
  static_assert (درست, "")
#وإلا
/* https://github.com/harfbuzz/harfbuzz/issues/1724 */
#تعریف_کن HB_PARTIALIZE(Pos) \
  قالب <نام_نوع _T> \
  خودکار عملگر () (_T&& _v) ثابت HB_AUTO_RETURN \
  (hb_partial<Pos> (+هذا, std::forward<_T> (_v))) \
  static_assert (درست, "")
#پایان_اگر


بنية
{
  خاص:

  قالب <نام_نوع Pred, نام_نوع Val> خودکار
  impl (Pred&& p, Val &&v, hb_اولویت<1>) ثابت HB_AUTO_RETURN
  (
    hb_deref (std::forward<Pred> (p)).has (std::forward<Val> (v))
  )

  قالب <نام_نوع Pred, نام_نوع Val> خودکار
  impl (Pred&& p, Val &&v, hb_اولویت<0>) ثابت HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Pred> (p),
	       std::forward<Val> (v))
  )

  عام:

  قالب <نام_نوع Pred, نام_نوع Val> خودکار
  عملگر () (Pred&& p, Val &&v) ثابت HB_RETURN (منطقي,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_has);

بنية
{
  خاص:

  قالب <نام_نوع Pred, نام_نوع Val> خودکار
  impl (Pred&& p, Val &&v, hb_اولویت<1>) ثابت HB_AUTO_RETURN
  (
    hb_has (std::forward<Pred> (p),
	    std::forward<Val> (v))
  )

  قالب <نام_نوع Pred, نام_نوع Val> خودکار
  impl (Pred&& p, Val &&v, hb_اولویت<0>) ثابت HB_AUTO_RETURN
  (
    std::forward<Pred> (p) == std::forward<Val> (v)
  )

  عام:

  قالب <نام_نوع Pred, نام_نوع Val> خودکار
  عملگر () (Pred&& p, Val &&v) ثابت HB_RETURN (منطقي,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_تطابق);

بنية
{
  خاص:

  قالب <نام_نوع Proj, نام_نوع Val> خودکار
  impl (Proj&& f, Val &&v, hb_اولویت<2>) ثابت HB_AUTO_RETURN
  (
    hb_deref (std::forward<Proj> (f)).get (std::forward<Val> (v))
  )

  قالب <نام_نوع Proj, نام_نوع Val> خودکار
  impl (Proj&& f, Val &&v, hb_اولویت<1>) ثابت HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Proj> (f),
	       std::forward<Val> (v))
  )

  قالب <نام_نوع Proj, نام_نوع Val> خودکار
  impl (Proj&& f, Val &&v, hb_اولویت<0>) ثابت HB_AUTO_RETURN
  (
    std::forward<Proj> (f)[std::forward<Val> (v)]
  )

  عام:

  قالب <نام_نوع Proj, نام_نوع Val> خودکار
  عملگر () (Proj&& f, Val &&v) ثابت HB_AUTO_RETURN
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

  قالب <نام_نوع T1, نام_نوع T2> خودکار
  impl (T1&& v1, T2 &&v2, hb_اولویت<3>) ثابت HB_AUTO_RETURN
  (
    std::forward<T2> (v2).cmp (std::forward<T1> (v1)) == 0
  )

  قالب <نام_نوع T1, نام_نوع T2> خودکار
  impl (T1&& v1, T2 &&v2, hb_اولویت<2>) ثابت HB_AUTO_RETURN
  (
    std::forward<T1> (v1).cmp (std::forward<T2> (v2)) == 0
  )

  قالب <نام_نوع T1, نام_نوع T2> خودکار
  impl (T1&& v1, T2 &&v2, hb_اولویت<1>) ثابت HB_AUTO_RETURN
  (
    std::forward<T1> (v1) == std::forward<T2> (v2)
  )

  قالب <نام_نوع T1, نام_نوع T2> خودکار
  impl (T1&& v1, T2 &&v2, hb_اولویت<0>) ثابت HB_AUTO_RETURN
  (
    std::forward<T2> (v2) == std::forward<T1> (v1)
  )

  عام:

  قالب <نام_نوع T1, نام_نوع T2> خودکار
  عملگر () (T1&& v1, T2 &&v2) ثابت HB_AUTO_RETURN
  (
    impl (std::forward<T1> (v1),
	  std::forward<T2> (v2),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_equal);

بنية
{
  قالب <نام_نوع T> فراغ
  عملگر () (T& a, T& b) ثابت
  {
    استفاده std::swap; // allow ADL
    swap (a, b);
  }
}
HB_FUNCOBJ (hb_جابجا);


قالب <نام_نوع T1, نام_نوع T2>
بنية hb_pair_t
{
  تعريفنوع T1 اولی_t;
  تعريفنوع T2 دومی_t;
  تعريفنوع hb_pair_t<T1, T2> pair_t;

  قالب <نام_نوع U1 = T1, نام_نوع U2 = T2,
	    hb_enable_if (std::is_default_constructible<U1>::value &&
			  std::is_default_constructible<U2>::value)>
  hb_pair_t () : اولی (), second () {}
  hb_pair_t (T1 a, T2 b) : اولی (std::forward<T1> (a)), second (std::forward<T2> (b)) {}

  قالب <نام_نوع Q1, نام_نوع Q2,
	    hb_enable_if (hb_is_convertible (T1, Q1) &&
			  hb_is_convertible (T2, Q2))>
  عملگر hb_pair_t<Q1, Q2> () { أرجع hb_pair_t<Q1, Q2> (اولی, second); }

  hb_pair_t<T1, T2> reverse () ثابت
  { أرجع hb_pair_t<T1, T2> (second, اولی); }

  منطقي عملگر == (ثابت pair_t& o) ثابت { أرجع اولی == o.اولی && second == o.second; }
  منطقي عملگر != (ثابت pair_t& o) ثابت { أرجع !(*هذا == o); }
  منطقي عملگر < (ثابت pair_t& o) ثابت { أرجع اولی < o.اولی || (اولی == o.اولی && second < o.second); }
  منطقي عملگر >= (ثابت pair_t& o) ثابت { أرجع !(*هذا < o); }
  منطقي عملگر > (ثابت pair_t& o) ثابت { أرجع اولی > o.اولی || (اولی == o.اولی && second > o.second); }
  منطقي عملگر <= (ثابت pair_t& o) ثابت { أرجع !(*هذا > o); }

  ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    pair_t *a = (pair_t *) pa;
    pair_t *b = (pair_t *) pb;

    إذا (a->اولی < b->اولی) أرجع -1;
    إذا (a->اولی > b->اولی) أرجع +1;
    إذا (a->second < b->second) أرجع -1;
    إذا (a->second > b->second) أرجع +1;
    أرجع 0;
  }

  دوست فراغ swap (hb_pair_t& a, hb_pair_t& b) noexcept
  {
    hb_جابجا (a.اولی, b.اولی);
    hb_جابجا (a.second, b.second);
  }


  T1 اولی;
  T2 second;
};
قالب <نام_نوع T1, نام_نوع T2> ساكن مضمن hb_pair_t<T1, T2>
hb_pair (T1&& a, T2&& b) { أرجع hb_pair_t<T1, T2> (a, b); }

تعريفنوع hb_pair_t<hb_codepoint_t, hb_codepoint_t> hb_codepoint_pair_t;

بنية
{
  قالب <نام_نوع Pair> constexpr نام_نوع Pair::اولی_t
  عملگر () (ثابت Pair& pair) ثابت { أرجع pair.اولی; }
}
HB_FUNCOBJ (hb_اولی);

بنية
{
  قالب <نام_نوع Pair> constexpr نام_نوع Pair::دومی_t
  عملگر () (ثابت Pair& pair) ثابت { أرجع pair.second; }
}
HB_FUNCOBJ (hb_دومی);

/* Note.  In min/max impl, we can use hb_type_identity<T> for second argument.
 * However, that would silently convert between different-signedness integers.
 * Instead we accept two different types, such that compiler can err if
 * comparing integers of different signedness. */
بنية
{
  قالب <نام_نوع T, نام_نوع T2> constexpr خودکار
  عملگر () (T&& a, T2&& b) ثابت HB_AUTO_RETURN
  (a <= b ? a : b)
}
HB_FUNCOBJ (hb_min);
بنية
{
  قالب <نام_نوع T, نام_نوع T2> constexpr خودکار
  عملگر () (T&& a, T2&& b) ثابت HB_AUTO_RETURN
  (a >= b ? a : b)
}
HB_FUNCOBJ (hb_max);
بنية
{
  قالب <نام_نوع T, نام_نوع T2, نام_نوع T3> constexpr خودکار
  عملگر () (T&& x, T2&& min, T3&& max) ثابت HB_AUTO_RETURN
  (hb_min (hb_max (std::forward<T> (x), std::forward<T2> (min)), std::forward<T3> (max)))
}
HB_FUNCOBJ (hb_clamp);

/* Signed saturating arithmetic. */
قالب <نام_نوع T>
ساكن HB_ALWAYS_INLINE T
hb_saturate_جمع_کن (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#إذا hb_has_builtin(__builtin_add_overflow)
  T نتیجه;
  إذا (likely (!__builtin_add_overflow (a, b, &نتیجه)))
    أرجع نتیجه;
#وإلا
  إذا (b > 0)
  {
    إذا (likely (a <= hb_int_max (T) - b))
      أرجع a + b;
  }
  وإلا إذا (likely (a >= hb_int_min (T) - b))
    أرجع a + b;
#پایان_اگر

  أرجع a < 0 ? hb_int_min (T) : hb_int_max (T);
}

قالب <نام_نوع T>
ساكن HB_ALWAYS_INLINE T
hb_saturate_sub (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#إذا hb_has_builtin(__builtin_sub_overflow)
  T نتیجه;
  إذا (likely (!__builtin_sub_overflow (a, b, &نتیجه)))
    أرجع نتیجه;
#وإلا
  إذا (b > 0)
  {
    إذا (likely (a >= hb_int_min (T) + b))
      أرجع a - b;
  }
  وإلا إذا (likely (a <= hb_int_max (T) + b))
    أرجع a - b;
#پایان_اگر

  أرجع b < 0 ? hb_int_max (T) : hb_int_min (T);
}

قالب <نام_نوع T>
ساكن HB_ALWAYS_INLINE T
hb_saturate_neg (T a)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  أرجع hb_saturate_sub ((T) 0, a);
}

/* Convert a floating-point value to integer type T, saturating to T's
 * range instead of relying on the undefined behavior of an out-of-range
 * float-to-int conversion.  NaN saturates to the minimum of T. */
قالب <نام_نوع T>
ساكن HB_ALWAYS_INLINE T
hb_clamp_مقصد (مزدوج v)
{
  أرجع (T) hb_clamp (v,
		       (مزدوج) hb_int_min (T),
		       (مزدوج) hb_int_max (T));
}

قالب <نام_نوع T>
ساكن HB_ALWAYS_INLINE T
hb_clamp_مقصد (عائم v)
{
  /* Widen explicitly: float cannot represent all integer bounds exactly,
   * so the clamp itself is done in double. */
  أرجع hb_clamp_مقصد<T> ((مزدوج) v);
}

قالب <نام_نوع T>
ساكن HB_ALWAYS_INLINE T
hb_clamp_مقصد (int64_t v)
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
ساكن مضمن بیعلامت
hb_popcount8 (uint8_t v)
{
  ساكن ثابت uint8_t popcount4[16] = {
    0, 1, 1, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 2, 3, 3, 4
  };
  أرجع popcount4[v & 0xF] + popcount4[v >> 4];
}

/* Return the number of 1 bits in v. */
قالب <نام_نوع T>
ساكن مضمن بیعلامت صحيح
hb_popcount (T v)
{
#إذا hb_has_builtin(__builtin_popcount)
  إذا (حجم (T) <= حجم (بیعلامت صحيح))
    أرجع __builtin_popcount (v);
#پایان_اگر

#إذا hb_has_builtin(__builtin_popcountl)
  إذا (حجم (T) <= حجم (بیعلامت دراز))
    أرجع __builtin_popcountl (v);
#پایان_اگر

#إذا hb_has_builtin(__builtin_popcountll)
  إذا (حجم (T) <= حجم (بیعلامت دراز دراز))
    أرجع __builtin_popcountll (v);
#پایان_اگر

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
    بیعلامت صحيح لغزش = 64;
    أرجع hb_popcount<uint64_t> ((uint64_t) v) + hb_popcount ((uint64_t) (v >> لغزش));
  }

  أكد (0);
  أرجع 0; /* Shut up stupid compiler. */
}

/* Returns the number of bits needed to store number */
قالب <نام_نوع T>
ساكن مضمن بیعلامت صحيح
hb_بیت_storage (T v)
{
  إذا (unlikely (!v)) أرجع 0;

#إذا hb_has_builtin(__builtin_clz)
  إذا (حجم (T) <= حجم (بیعلامت صحيح))
    أرجع حجم (بیعلامت صحيح) * 8 - __builtin_clz (v);
#پایان_اگر

#إذا hb_has_builtin(__builtin_clzl)
  إذا (حجم (T) <= حجم (بیعلامت دراز))
    أرجع حجم (بیعلامت دراز) * 8 - __builtin_clzl (v);
#پایان_اگر

#إذا hb_has_builtin(__builtin_clzll)
  إذا (حجم (T) <= حجم (بیعلامت دراز دراز))
    أرجع حجم (بیعلامت دراز دراز) * 8 - __builtin_clzll (v);
#پایان_اگر

#إذا (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  إذا (حجم (T) <= حجم (بیعلامت صحيح))
  {
    بیعلامت دراز where;
    بیت_پیمایش_معکوس (&where, v);
    أرجع 1 + where;
  }
# إذا defined(_WIN64)
  إذا (حجم (T) <= 8)
  {
    بیعلامت دراز where;
    بیت_پیمایش_Reverse64 (&where, v);
    أرجع 1 + where;
  }
# endif
#پایان_اگر

  إذا (حجم (T) <= 4)
  {
    /* "bithacks" */
    ثابت بیعلامت صحيح b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    ثابت بیعلامت صحيح S[] = {1, 2, 4, 8, 16};
    بیعلامت صحيح r = 0;
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
    ثابت بیعلامت صحيح S[] = {1, 2, 4, 8, 16, 32};
    بیعلامت صحيح r = 0;
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
    بیعلامت صحيح لغزش = 64;
    أرجع (v >> لغزش) ? hb_بیت_storage<uint64_t> ((uint64_t) (v >> لغزش)) + لغزش :
			  hb_بیت_storage<uint64_t> ((uint64_t) v);
  }

  أكد (0);
  أرجع 0; /* Shut up stupid compiler. */
}

/* Returns the number of zero bits in the least significant side of v */
قالب <نام_نوع T>
ساكن مضمن بیعلامت صحيح
hb_ctz (T v)
{
  إذا (unlikely (!v)) أرجع 8 * حجم (T);

#إذا hb_has_builtin(__builtin_ctz)
  إذا (حجم (T) <= حجم (بیعلامت صحيح))
    أرجع __builtin_ctz (v);
#پایان_اگر

#إذا hb_has_builtin(__builtin_ctzl)
  إذا (حجم (T) <= حجم (بیعلامت دراز))
    أرجع __builtin_ctzl (v);
#پایان_اگر

#إذا hb_has_builtin(__builtin_ctzll)
  إذا (حجم (T) <= حجم (بیعلامت دراز دراز))
    أرجع __builtin_ctzll (v);
#پایان_اگر

#إذا (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  إذا (حجم (T) <= حجم (بیعلامت صحيح))
  {
    بیعلامت دراز where;
    بیت_پیمایش_Forward (&where, v);
    أرجع where;
  }
# إذا defined(_WIN64)
  إذا (حجم (T) <= 8)
  {
    بیعلامت دراز where;
    بیت_پیمایش_Forward64 (&where, v);
    أرجع where;
  }
# endif
#پایان_اگر

  إذا (حجم (T) <= 4)
  {
    /* "bithacks" */
    بیعلامت صحيح c = 32;
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
    بیعلامت صحيح c = 64;
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
    بیعلامت صحيح لغزش = 64;
    أرجع (uint64_t) v ? hb_بیت_storage<uint64_t> ((uint64_t) v) :
			  hb_بیت_storage<uint64_t> ((uint64_t) (v >> لغزش)) + لغزش;
  }

  أكد (0);
  أرجع 0; /* Shut up stupid compiler. */
}


/*
 * Tiny stuff.
 */

/* ASCII tag/character handling */
ساكن مضمن منطقي ISALPHA (بیعلامت حرف c)
{ أرجع (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
ساكن مضمن منطقي ISALNUM (بیعلامت حرف c)
{ أرجع (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }
ساكن مضمن منطقي ISSPACE (بیعلامت حرف c)
{ أرجع c == ' ' || c =='\f'|| c =='\n'|| c =='\r'|| c =='\t'|| c =='\v'; }
ساكن مضمن بیعلامت حرف TOUPPER (بیعلامت حرف c)
{ أرجع (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c; }
ساكن مضمن بیعلامت حرف TOLOWER (بیعلامت حرف c)
{ أرجع (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }
ساكن مضمن منطقي ISHEX (بیعلامت حرف c)
{ أرجع (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
ساكن مضمن بیعلامت حرف TOHEX (uint8_t c)
{ أرجع (c & 0xF) <= 9 ? (c & 0xF) + '0' : (c & 0xF) + 'a' - 10; }
ساكن مضمن uint8_t FROMHEX (بیعلامت حرف c)
{ أرجع (c >= '0' && c <= '9') ? c - '0' : TOLOWER (c) - 'a' + 10; }

ساكن مضمن بیعلامت صحيح DIV_CEIL (ثابت بیعلامت صحيح a, بیعلامت صحيح b)
{ أرجع (a + (b - 1)) / b; }


#لغو_تعریف  ARRAY_درازی
قالب <نام_نوع Type, بیعلامت صحيح n>
ساكن مضمن بیعلامت صحيح ARRAY_درازی (ثابت Type (&)[n]) { أرجع n; }
/* A const version, but does not detect erratically being called on pointers. */
#تعریف_کن ARRAY_درازی_CONST(__array) ((علامتدار صحيح) (حجم (__array) / حجم (__array[0])))


ساكن مضمن فراغ *
hb_memcpy (فراغ *__restrict dst, ثابت فراغ *__restrict src, size_t len)
{
  /* It's illegal to pass 0 as size to memcpy. */
  إذا (unlikely (!len)) أرجع dst;
  أرجع کپی_حافظه (dst, src, len);
}

ساكن مضمن صحيح
hb_memcmp (ثابت فراغ *a, ثابت فراغ *b, بیعلامت صحيح len)
{
  /* It's illegal to pass NULL to memcmp(), even if len is zero.
   * So, wrap it.
   * https://sourceware.org/bugzilla/show_bug.cgi?id=23878 */
  إذا (unlikely (!len)) أرجع 0;
  أرجع memcmp (a, b, len);
}

ساكن مضمن فراغ *
hb_memset (فراغ *s, صحيح c, بیعلامت صحيح n)
{
  /* It's illegal to pass NULL to memset(), even if n is zero. */
  إذا (unlikely (!n)) أرجع s;
  أرجع memset (s, c, n);
}

ساكن مضمن بیعلامت صحيح
hb_ceil_مقصد_4 (بیعلامت صحيح v)
{
  أرجع ((v - 1) | 3) + 1;
}

قالب <نام_نوع T> ساكن مضمن منطقي
hb_درون_range (T u, T lo, T hi)
{
  static_assert (!std::is_signed<T>::value, "");

  /* The casts below are important as if T is smaller than int,
   * the subtract results will become a signed int! */
  أرجع (T)(u - lo) <= (T)(hi - lo);
}
قالب <نام_نوع T> ساكن مضمن منطقي
hb_درون_ranges (T u, T lo1, T hi1)
{
  أرجع hb_درون_range (u, lo1, hi1);
}
قالب <نام_نوع T, نام_نوع ...Ts> ساكن مضمن منطقي
hb_درون_ranges (T u, T lo1, T hi1, Ts... ds)
{
  أرجع hb_درون_range<T> (u, lo1, hi1) || hb_درون_ranges<T> (u, ds...);
}


/*
 * Overflow checking.
 */

ساكن مضمن منطقي
hb_unsigned_mul_overflows (بیعلامت صحيح شمار, بیعلامت صحيح اندازه, بیعلامت *نتیجه = اشاره_تهی)
{
#إذا hb_has_builtin(__builtin_mul_overflow)
  بیعلامت پشته_نتیجه;
  إذا (!نتیجه)
    نتیجه = &پشته_نتیجه;
  أرجع __builtin_mul_overflow (شمار, اندازه, نتیجه);
#پایان_اگر

  إذا (نتیجه)
    *نتیجه = شمار * اندازه;
  أرجع (اندازه > 0) && (شمار >= ((بیعلامت صحيح) -1) / اندازه);
}

ساكن مضمن منطقي
hb_unsigned_جمع_کن_overflows (بیعلامت صحيح a, بیعلامت صحيح b, بیعلامت *نتیجه = اشاره_تهی)
{
#إذا hb_has_builtin(__builtin_add_overflow)
  بیعلامت پشته_نتیجه;
  إذا (!نتیجه)
    نتیجه = &پشته_نتیجه;
  أرجع __builtin_add_overflow (a, b, نتیجه);
#پایان_اگر

  إذا (نتیجه)
    *نتیجه = a + b;
  أرجع b > (بیعلامت صحيح) -1 - a;
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
  إذا (حجم (size_t) > حجم (بیعلامت صحيح))
    أرجع a * b;
#إذا hb_has_builtin(__builtin_mul_overflow)
  size_t نتیجه;
  إذا (__builtin_mul_overflow (a, b, &نتیجه))
    أرجع (size_t) -1;
  أرجع نتیجه;
#وإلا
  إذا (b > 0 && a > ((size_t) -1) / b) أرجع (size_t) -1;
  أرجع a * b;
#پایان_اگر
}

ساكن مضمن size_t
hb_unsigned_جمع_کن_saturate (size_t a, size_t b)
{
  إذا (حجم (size_t) > حجم (بیعلامت صحيح))
    أرجع a + b;
#إذا hb_has_builtin(__builtin_add_overflow)
  size_t نتیجه;
  إذا (__builtin_add_overflow (a, b, &نتیجه))
    أرجع (size_t) -1;
  أرجع نتیجه;
#وإلا
  إذا (b > ((size_t) -1) - a) أرجع (size_t) -1;
  أرجع a + b;
#پایان_اگر
}

/* Variadic forms: fold left across all arguments. */
قالب <نام_نوع ...Ts>
ساكن مضمن size_t
hb_unsigned_mul_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ أرجع hb_unsigned_mul_saturate (hb_unsigned_mul_saturate (a, b), c, rest...); }

قالب <نام_نوع ...Ts>
ساكن مضمن size_t
hb_unsigned_جمع_کن_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ أرجع hb_unsigned_جمع_کن_saturate (hb_unsigned_جمع_کن_saturate (a, b), c, rest...); }

/* Saturating mul-add: a * b + c.  Covers the dominant get_size() pattern
 * `count * static_size + min_size`. */
ساكن مضمن size_t
hb_unsigned_mul_جمع_کن_saturate (size_t a, size_t b, size_t c)
{ أرجع hb_unsigned_جمع_کن_saturate (hb_unsigned_mul_saturate (a, b), c); }


/*
 * Sort and search.
 */

قالب <نام_نوع K, نام_نوع V, نام_نوع ...Ts>
ساكن صحيح
hb_cmp_روش (ثابت فراغ *pkey, ثابت فراغ *pval, Ts... ds)
{
  ثابت K& key = * (ثابت K*) pkey;
  ثابت V& val = * (ثابت V*) pval;

  أرجع val.cmp (key, ds...);
}

قالب <نام_نوع K, نام_نوع V>
ساكن صحيح
_hb_cmp_operator (ثابت فراغ *pkey, ثابت فراغ *pval)
{
  ثابت K& key = * (ثابت K*) pkey;
  ثابت V& val = * (ثابت V*) pval;

  إذا (key < val) أرجع -1;
  إذا (key > val) أرجع  1;
  أرجع 0;
}

قالب <نام_نوع V, نام_نوع K, نام_نوع ...Ts>
HB_HOT
ساكن مضمن منطقي
hb_bsearch_impl (بیعلامت *pos, /* Out */
		 ثابت K& key,
		 V* پایه, size_t nmemb, size_t stride,
		 صحيح (*compar)(ثابت فراغ *کلید_3, ثابت فراغ *قلم_2, Ts... _ds),
		 Ts... ds)
{
  /* This is our *only* bsearch implementation. */

  صحيح min = 0, max = (صحيح) nmemb - 1;
  طالما (min <= max)
  {
    صحيح mid = ((بیعلامت صحيح) min + (بیعلامت صحيح) max) / 2;
#رهنمود GCC diagnostic push
#رهنمود GCC diagnostic ignored "-Wcast-align"
    V* p = (V*) (((ثابت حرف *) پایه) + (mid * stride));
#رهنمود GCC diagnostic برداشت
    صحيح c = compar ((ثابت فراغ *) std::addressof (key), (ثابت فراغ *) p, ds...);
    إذا (c < 0)
      max = mid - 1;
    وإلا إذا (c > 0)
      min = mid + 1;
    وإلا
    {
      *pos = mid;
      أرجع درست;
    }
  }
  *pos = min;
  أرجع خطأ;
}

قالب <نام_نوع V, نام_نوع K>
ساكن مضمن V*
hb_bsearch (ثابت K& key, V* پایه,
	    size_t nmemb, size_t stride = حجم (V),
	    صحيح (*compar)(ثابت فراغ *کلید_3, ثابت فراغ *قلم_2) = hb_cmp_روش<K, V>)
{
  بیعلامت pos;
#رهنمود GCC diagnostic push
#رهنمود GCC diagnostic ignored "-Wcast-align"
  أرجع hb_bsearch_impl (&pos, key, پایه, nmemb, stride, compar) ?
	 (V*) (((ثابت حرف *) پایه) + (pos * stride)) : اشاره_تهی;
#رهنمود GCC diagnostic برداشت
}
قالب <نام_نوع V, نام_نوع K, نام_نوع ...Ts>
ساكن مضمن V*
hb_bsearch (ثابت K& key, V* پایه,
	    size_t nmemb, size_t stride,
	    صحيح (*compar)(ثابت فراغ *کلید_3, ثابت فراغ *قلم_2, Ts... _ds),
	    Ts... ds)
{
  بیعلامت pos;
#رهنمود GCC diagnostic push
#رهنمود GCC diagnostic ignored "-Wcast-align"
  أرجع hb_bsearch_impl (&pos, key, پایه, nmemb, stride, compar, ds...) ?
	 (V*) (((ثابت حرف *) پایه) + (pos * stride)) : اشاره_تهی;
#رهنمود GCC diagnostic برداشت
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
قالب <نام_نوع T, نام_نوع Compar>
ساكن مضمن فراغ
hb_qsort_loop (T *پایه, size_t nel, Compar compar)
{
  طالما (nel > 24)
  {
    T *آخرین = پایه + nel - 1;
    T *mid = پایه + nel / 2;

    /* Median-of-three pivot, parked at last-1. */
    إذا (compar (*پایه, *mid) > 0) hb_جابجا (*پایه, *mid);
    إذا (compar (*mid, *آخرین) > 0)
    {
      hb_جابجا (*mid, *آخرین);
      إذا (compar (*پایه, *mid) > 0) hb_جابجا (*پایه, *mid);
    }
    hb_جابجا (*mid, *(آخرین - 1));
    T &محور = *(آخرین - 1);

    /* Two-way Hoare partition.  Inner loops are unguarded:
     * median-of-three left *base <= pivot and *last >= pivot,
     * which act as sentinels. */
    T *i = پایه, *j = آخرین - 1;
    طالما (درست)
    {
      طالما (compar (*++i, محور) < 0) {}
      طالما (compar (*--j, محور) > 0) {}
      إذا (i >= j) اكسر;
      hb_جابجا (*i, *j);
    }
    hb_جابجا (*i, *(آخرین - 1));

    /* Recurse on smaller side, loop on larger — bounds stack
     * depth at O(log n). */
    size_t چپ  = (size_t) (i - پایه);
    size_t راست = nel - چپ - 1;
    إذا (چپ < راست)
    {
      hb_qsort_loop (پایه, چپ, compar);
      پایه = i + 1;
      nel  = راست;
    }
    وإلا
    {
      hb_qsort_loop (i + 1, راست, compar);
      nel  = چپ;
    }
  }
}

قالب <نام_نوع T, نام_نوع Compar>
ساكن مضمن فراغ
hb_qsort_inline (T *پایه, size_t nel, Compar compar)
{
  hb_qsort_loop (پایه, nel, compar);

  /* Single final insertion sort over the whole array.  After
   * the partitioning loop, every element is within the threshold
   * of its sorted position, so this pass is O(n * threshold). */
  T *انتها = پایه + nel;
  لكل (T *pi = پایه + 1; pi < انتها; pi++)
    لكل (T *pj = pi; pj > پایه && compar (pj[-1], pj[0]) > 0; pj--)
      hb_جابجا (pj[-1], pj[0]);
}

ساكن مضمن فراغ
hb_qsort (فراغ *پایه, size_t nel, size_t عرض,
	  صحيح (*compar)(ثابت فراغ *_a, ثابت فراغ *_b))
{
  qsort (پایه, nel, عرض, compar);
}




قالب <نام_نوع T, نام_نوع T2, نام_نوع T3 = صحيح> ساكن مضمن فراغ
hb_stable_مرتب_سازی (T *array, بیعلامت صحيح len, صحيح(*compar)(ثابت T2 *, ثابت T2 *), T3 *array2 = اشاره_تهی)
{
  static_assert (hb_is_trivially_copy_assignable (T), "");
  static_assert (hb_is_trivially_copy_assignable (T3), "");

  لكل (بیعلامت صحيح i = 1; i < len; i++)
  {
    بیعلامت صحيح j = i;
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
hb_codepoint_parse (ثابت حرف *s, بیعلامت صحيح len, صحيح پایه, hb_codepoint_t *بیرون)
{
  بیعلامت صحيح v;
  ثابت حرف *p = s;
  ثابت حرف *انتها = p + len;
  إذا (unlikely (!hb_parse_uint (&p, انتها, &v, درست/* whole buffer */, پایه)))
    أرجع خطأ;

  *بیرون = v;
  أرجع درست;
}


/* Operators. */

بنية
{ HB_PARTIALIZE(2);
  قالب <نام_نوع T> constexpr خودکار
  عملگر () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (a & b)
}
HB_FUNCOBJ (hb_bitwise_and);
بنية
{ HB_PARTIALIZE(2);
  قالب <نام_نوع T> constexpr خودکار
  عملگر () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (a | b)
}
HB_FUNCOBJ (hb_bitwise_or);
بنية
{ HB_PARTIALIZE(2);
  قالب <نام_نوع T> constexpr خودکار
  عملگر () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (a ^ b)
}
HB_FUNCOBJ (hb_bitwise_xor);
بنية
{ HB_PARTIALIZE(2);
  قالب <نام_نوع T> constexpr خودکار
  عملگر () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (~a & b)
}
HB_FUNCOBJ (hb_bitwise_lt);
بنية
{ HB_PARTIALIZE(2);
  قالب <نام_نوع T> constexpr خودکار
  عملگر () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (a & ~b)
}
HB_FUNCOBJ (hb_bitwise_gt); // aka sub
بنية
{ HB_PARTIALIZE(2);
  قالب <نام_نوع T> constexpr خودکار
  عملگر () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (~a | b)
}
HB_FUNCOBJ (hb_bitwise_le);
بنية
{ HB_PARTIALIZE(2);
  قالب <نام_نوع T> constexpr خودکار
  عملگر () (ثابت T &a, ثابت T &b) ثابت HB_AUTO_RETURN (a | ~b)
}
HB_FUNCOBJ (hb_bitwise_ge);
بنية
{
  قالب <نام_نوع T> constexpr خودکار
  عملگر () (ثابت T &a) ثابت HB_AUTO_RETURN (~a)
}
HB_FUNCOBJ (hb_bitwise_neg);

بنية
{ HB_PARTIALIZE(2);
  قالب <نام_نوع T, نام_نوع T2> constexpr خودکار
  عملگر () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (a + b)
}
HB_FUNCOBJ (hb_جمع_کن);
بنية
{ HB_PARTIALIZE(2);
  قالب <نام_نوع T, نام_نوع T2> constexpr خودکار
  عملگر () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (a - b)
}
HB_FUNCOBJ (hb_sub);
بنية
{ HB_PARTIALIZE(2);
  قالب <نام_نوع T, نام_نوع T2> constexpr خودکار
  عملگر () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (b - a)
}
HB_FUNCOBJ (hb_rsub);
بنية
{ HB_PARTIALIZE(2);
  قالب <نام_نوع T, نام_نوع T2> constexpr خودکار
  عملگر () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (a * b)
}
HB_FUNCOBJ (hb_mul);
بنية
{ HB_PARTIALIZE(2);
  قالب <نام_نوع T, نام_نوع T2> constexpr خودکار
  عملگر () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (a / b)
}
HB_FUNCOBJ (hb_div);
بنية
{ HB_PARTIALIZE(2);
  قالب <نام_نوع T, نام_نوع T2> constexpr خودکار
  عملگر () (ثابت T &a, ثابت T2 &b) ثابت HB_AUTO_RETURN (a % b)
}
HB_FUNCOBJ (hb_mod);
بنية
{
  قالب <نام_نوع T> constexpr خودکار
  عملگر () (ثابت T &a) ثابت HB_AUTO_RETURN (+a)
}
HB_FUNCOBJ (hb_pos);
بنية
{
  قالب <نام_نوع T> constexpr خودکار
  عملگر () (ثابت T &a) ثابت HB_AUTO_RETURN (-a)
}
HB_FUNCOBJ (hb_neg);
بنية
{
  قالب <نام_نوع T> constexpr خودکار
  عملگر () (T &a) ثابت HB_AUTO_RETURN (++a)
}
HB_FUNCOBJ (hb_inc);
بنية
{
  قالب <نام_نوع T> constexpr خودکار
  عملگر () (T &a) ثابت HB_AUTO_RETURN (--a)
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
قالب <نام_نوع func_t>
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

  بیعلامت n1_2 = (بیعلامت) (hb_max (ceil (log2 ((b - a) / epsilon)) - 1.0, 0.0));
  ثابت بیعلامت n0 = 1; // Hardwired
  ثابت مزدوج k1 = 0.2 / (b - a); // Hardwired.
  بیعلامت nmax = n0 + n1_2;
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
قالب <نام_نوع F>
بنية hb_scope_guard_t
{
  صریح hb_scope_guard_t (F &&f) : f (std::move (f)), فعال (درست) {}
  hb_scope_guard_t (hb_scope_guard_t &&o) noexcept
    : f (std::move (o.f)), فعال (o.فعال) { o.فعال = خطأ; }
  hb_scope_guard_t (ثابت hb_scope_guard_t &) = حذف;
  hb_scope_guard_t &عملگر= (ثابت hb_scope_guard_t &) = حذف;
  hb_scope_guard_t &عملگر= (hb_scope_guard_t &&) = حذف;
  ~hb_scope_guard_t () { إذا (فعال) f (); }

  /* Release: dismiss the guard so the cleanup does NOT run.  Use
   * when transferring ownership out of the scope. */
  فراغ release () { فعال = خطأ; }

  خاص:
  F f;
  منطقي فعال;
};

قالب <نام_نوع F>
ساكن مضمن hb_scope_guard_t<F> hb_make_scope_guard (F &&f)
{ أرجع hb_scope_guard_t<F> (std::forward<F> (f)); }

#تعریف_کن HB_SCOPE_GUARD_NAME_(سطر) hb_scope_guard_##سطر
#تعریف_کن HB_SCOPE_GUARD_NAME(سطر) HB_SCOPE_GUARD_NAME_(سطر)
#تعریف_کن HB_SCOPE_GUARD(stmt) \
  خودکار HB_SCOPE_GUARD_NAME(__LINE__) = \
    hb_make_scope_guard ([&]() { stmt; }); \
  (فراغ) HB_SCOPE_GUARD_NAME(__LINE__)


#پایان_اگر /* HB_ALGS_HH */
