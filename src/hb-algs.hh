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

#నిర్వచితం_కాకపోతే HB_ALGS_HH
#నిర్వచించు HB_ALGS_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-meta.hh"
#అన్తర్భావ "hb-null.hh"
#అన్తర్భావ "hb-number.hh"

#అన్తర్భావ <algorithm>
#అన్తర్భావ <initializer_list>
#అన్తర్భావ <functional>
#అన్తర్భావ <new>

స్థిర HB_ALWAYS_INLINE శూన్య *
hb_malloc2 (size_t nmemb, size_t కొలత)
{
  ఒకవేళ (కొలత && nmemb > SIZE_MAX / కొలత) తిరిగి శూన్యసూచిక;
  తిరిగి hb_malloc (nmemb * కొలత);
}
స్థిర HB_ALWAYS_INLINE శూన్య *
hb_realloc2 (శూన్య *ptr, size_t nmemb, size_t కొలత)
{
  ఒకవేళ (కొలత && nmemb > SIZE_MAX / కొలత) తిరిగి శూన్యసూచిక;
  తిరిగి hb_realloc (ptr, nmemb * కొలత);
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
#నిర్వచితమైతే _MSC_VER
# pragma warning(disable:4200)
# pragma warning(disable:4800)
#అయితే_ముగింపు
#నిర్వచించు HB_MARK_AS_FLAG_T(T) \
	బాహ్య "C++" { \
	  స్థిర అంతఃవరుస constexpr T క్రియాకారకం | (T l, T r) { తిరిగి T ((గుర్తులేని) l | (గుర్తులేని) r); } \
	  స్థిర అంతఃవరుస constexpr T క్రియాకారకం & (T l, T r) { తిరిగి T ((గుర్తులేని) l & (గుర్తులేని) r); } \
	  స్థిర అంతఃవరుస constexpr T క్రియాకారకం ^ (T l, T r) { తిరిగి T ((గుర్తులేని) l ^ (గుర్తులేని) r); } \
	  స్థిర అంతఃవరుస constexpr గుర్తులేని క్రియాకారకం ~ (T r) { తిరిగి (~(గుర్తులేని) r); } \
	  స్థిర అంతఃవరుస T& క్రియాకారకం |= (T &l, T r) { l = l | r; తిరిగి l; } \
	  స్థిర అంతఃవరుస T& క్రియాకారకం &= (T& l, T r) { l = l & r; తిరిగి l; } \
	  స్థిర అంతఃవరుస T& క్రియాకారకం ^= (T& l, T r) { l = l ^ r; తిరిగి l; } \
	} \
	static_assert (నిజం, "")

/* Useful for set-operations on small enums.
 * For example, for testing "x ∈ {x1, x2, x3}" use:
 * (FLAG_UNSAFE(x) & (FLAG(x1) | FLAG(x2) | FLAG(x3)))
 */
#నిర్వచించు FLAG(x) (static_assert_సమాసం ((గుర్తులేని)(x) < 32) + (((uint32_t) 1U) << (గుర్తులేని)(x)))
#నిర్వచించు FLAG_UNSAFE(x) ((గుర్తులేని)(x) < 32 ? (((uint32_t) 1U) << (గుర్తులేని)(x)) : 0)
#నిర్వచించు FLAG_RANGE(x,y) (static_assert_సమాసం ((x) < (y)) + FLAG(y+1) - FLAG(x))
#నిర్వచించు FLAG64(x) (static_assert_సమాసం ((గుర్తులేని)(x) < 64) + (((uint64_t) 1ULL) << (గుర్తులేని)(x)))
#నిర్వచించు FLAG64_UNSAFE(x) ((గుర్తులేని)(x) < 64 ? (((uint64_t) 1ULL) << (గుర్తులేని)(x)) : 0)


/*
 * Fixed-endian integers / floats.
 */


/* Endian swap, used in Windows related backends */
స్థిర అంతఃవరుస constexpr uint16_t hb_uint16_మార్పిడి (uint16_t v)
{ తిరిగి (v >> 8) | (v << 8); }
స్థిర అంతఃవరుస constexpr uint32_t hb_uint32_మార్పిడి (uint32_t v)
{ తిరిగి (hb_uint16_మార్పిడి (v) << 16) | hb_uint16_మార్పిడి (v >> 16); }

మూస <రకం_పేరు Type>
నిర్మాణం __attribute__((packed)) hb_packed_t
{
  hb_packed_t () = అప్రమేయ;
  constexpr hb_packed_t (Type V) : v (V) {}
  క్రియాకారకం Type () స్థిరరాశి { తిరిగి v; }
  hb_packed_t & క్రియాకారకం = (Type V) { v = V; తిరిగి *ఇది; }

  అంతరంగ:
  Type v;
};

#నిర్వచితం_కాకపోతే HB_FAST_అంకె_ACCESS

#ఒకవేళ defined(__OPTIMIZE__) && \
    defined(__BYTE_ORDER) && \
    (__BYTE_ORDER == __BIG_ENDIAN || \
     (__BYTE_ORDER == __LITTLE_ENDIAN && \
      hb_has_builtin(__builtin_bswap16) && \
      hb_has_builtin(__builtin_bswap32) && \
      hb_has_builtin(__builtin_bswap64)))
#నిర్వచించు HB_FAST_అంకె_ACCESS 1
#లేకపోతే
#నిర్వచించు HB_FAST_అంకె_ACCESS 0
#అయితే_ముగింపు

// https://github.com/harfbuzz/harfbuzz/issues/5456
#ఒకవేళ defined(__GNUC__) && !defined(__clang__) && (__GNUC__ <= 12)
#నిర్వచనం_రద్దు HB_FAST_అంకె_ACCESS
#నిర్వచించు HB_FAST_అంకె_ACCESS 0
#అయితే_ముగింపు

#అయితే_ముగింపు

మూస <తార్కిక BE, రకం_పేరు Type, పూర్ణాంకం బైట్లు_3 = పరిమాణం (Type)>
నిర్మాణం HBInt;
మూస <తార్కిక BE, రకం_పేరు Type>
నిర్మాణం HBInt<BE, Type, 1>
{
  బహిరంగ:
  HBInt () = అప్రమేయ;
  constexpr HBInt (Type V) : v {uint8_t (V)} {}
  constexpr క్రియాకారకం Type () స్థిరరాశి { తిరిగి v; }
  అంతరంగ: uint8_t v;
};
మూస <తార్కిక BE, రకం_పేరు Type>
నిర్మాణం HBInt<BE, Type, 2>
{
  బహిరంగ:
  HBInt () = అప్రమేయ;

  HBInt (Type V)
#ఒకవేళ HB_FAST_అంకె_ACCESS
  {
    ఒకవేళ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint16_t> *) v) = V;
    లేకపోతే
      *((hb_packed_t<uint16_t> *) v) = __builtin_bswap16 (V);
  }
#లేకపోతే
    : v {BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >>  8) & 0xFF)} {}
#అయితే_ముగింపు

  constexpr క్రియాకారకం Type () స్థిరరాశి
  {
#ఒకవేళ HB_FAST_అంకె_ACCESS
    తిరిగి (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint16_t) *((స్థిరరాశి hb_packed_t<uint16_t> *) v)
    :
      __builtin_bswap16 ((uint16_t) *((స్థిరరాశి hb_packed_t<uint16_t> *) v))
    ;
#లేకపోతే
    తిరిగి (BE ? (v[0] <<  8) : (v[0]      ))
	 + (BE ? (v[1]      ) : (v[1] <<  8));
#అయితే_ముగింపు
  }
  అంతరంగ: uint8_t v[2];
};
మూస <తార్కిక BE, రకం_పేరు Type>
నిర్మాణం HBInt<BE, Type, 3>
{
  static_assert (!std::is_signed<Type>::value, "");
  బహిరంగ:
  HBInt () = అప్రమేయ;
  constexpr HBInt (Type V) : v {BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
				BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
				BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V      ) & 0xFF)} {}

  constexpr క్రియాకారకం Type () స్థిరరాశి { తిరిగి (BE ? (v[0] << 16) : (v[0]      ))
					  + (BE ? (v[1] <<  8) : (v[1] <<  8))
					  + (BE ? (v[2]      ) : (v[2] << 16)); }
  అంతరంగ: uint8_t v[3];
};
మూస <తార్కిక BE, రకం_పేరు Type>
నిర్మాణం HBInt<BE, Type, 4>
{
  మూస <తార్కిక, రకం_పేరు, పూర్ణాంకం>
  మిత్ర నిర్మాణం HBFloat;

  బహిరంగ:
  HBInt () = అప్రమేయ;

  HBInt (Type V)
#ఒకవేళ HB_FAST_అంకె_ACCESS
  {
    ఒకవేళ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint32_t> *) v) = V;
    లేకపోతే
      *((hb_packed_t<uint32_t> *) v) = __builtin_bswap32 (V);
  }
#లేకపోతే
    : v {BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 24) & 0xFF)} {}
#అయితే_ముగింపు

  constexpr క్రియాకారకం Type () స్థిరరాశి {
#ఒకవేళ HB_FAST_అంకె_ACCESS
    తిరిగి (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint32_t) *((స్థిరరాశి hb_packed_t<uint32_t> *) v)
    :
      __builtin_bswap32 ((uint32_t) *((స్థిరరాశి hb_packed_t<uint32_t> *) v))
    ;
#లేకపోతే
    తిరిగి (BE ? (uint32_t (v[0]) << 24) : (uint32_t (v[0])      ))
	 + (BE ? (uint32_t (v[1]) << 16) : (uint32_t (v[1]) <<  8))
	 + (BE ? (uint32_t (v[2]) <<  8) : (uint32_t (v[2]) << 16))
	 + (BE ? (uint32_t (v[3])      ) : (uint32_t (v[3]) << 24));
#అయితే_ముగింపు
  }
  అంతరంగ: uint8_t v[4];
};
మూస <తార్కిక BE, రకం_పేరు Type>
నిర్మాణం HBInt<BE, Type, 8>
{
  మూస <తార్కిక, రకం_పేరు, పూర్ణాంకం>
  మిత్ర నిర్మాణం HBFloat;

  బహిరంగ:
  HBInt () = అప్రమేయ;

  HBInt (Type V)
#ఒకవేళ HB_FAST_అంకె_ACCESS
  {
    ఒకవేళ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
      *((hb_packed_t<uint64_t> *) v) = V;
    లేకపోతే
      *((hb_packed_t<uint64_t> *) v) = __builtin_bswap64 (V);
  }
#లేకపోతే
    : v {BE ? uint8_t ((V >> 56) & 0xFF) : uint8_t ((V      ) & 0xFF),
	 BE ? uint8_t ((V >> 48) & 0xFF) : uint8_t ((V >>  8) & 0xFF),
	 BE ? uint8_t ((V >> 40) & 0xFF) : uint8_t ((V >> 16) & 0xFF),
	 BE ? uint8_t ((V >> 32) & 0xFF) : uint8_t ((V >> 24) & 0xFF),
	 BE ? uint8_t ((V >> 24) & 0xFF) : uint8_t ((V >> 32) & 0xFF),
	 BE ? uint8_t ((V >> 16) & 0xFF) : uint8_t ((V >> 40) & 0xFF),
	 BE ? uint8_t ((V >>  8) & 0xFF) : uint8_t ((V >> 48) & 0xFF),
	 BE ? uint8_t ((V      ) & 0xFF) : uint8_t ((V >> 56) & 0xFF)} {}
#అయితే_ముగింపు

  constexpr క్రియాకారకం Type () స్థిరరాశి {
#ఒకవేళ HB_FAST_అంకె_ACCESS
    తిరిగి (BE == (__BYTE_ORDER == __BIG_ENDIAN)) ?
      (uint64_t) *((స్థిరరాశి hb_packed_t<uint64_t> *) v)
    :
      __builtin_bswap64 ((uint64_t) *((స్థిరరాశి hb_packed_t<uint64_t> *) v))
    ;
#లేకపోతే
    తిరిగి (BE ? (uint64_t (v[0]) << 56) : (uint64_t (v[0])      ))
	 + (BE ? (uint64_t (v[1]) << 48) : (uint64_t (v[1]) <<  8))
	 + (BE ? (uint64_t (v[2]) << 40) : (uint64_t (v[2]) << 16))
	 + (BE ? (uint64_t (v[3]) << 32) : (uint64_t (v[3]) << 24))
	 + (BE ? (uint64_t (v[4]) << 24) : (uint64_t (v[4]) << 32))
	 + (BE ? (uint64_t (v[5]) << 16) : (uint64_t (v[5]) << 40))
	 + (BE ? (uint64_t (v[6]) <<  8) : (uint64_t (v[6]) << 48))
	 + (BE ? (uint64_t (v[7])      ) : (uint64_t (v[7]) << 56));
#అయితే_ముగింపు
  }
  అంతరంగ: uint8_t v[8];
};

/* Floats. */

మూస <తార్కిక BE, రకం_పేరు Type, పూర్ణాంకం బైట్లు_3>
నిర్మాణం HBFloat
{
  ఉపయోగించి IntType = రకం_పేరు std::conditional<బైట్లు_3 == 4, uint32_t, uint64_t>::type;

  బహిరంగ:
  HBFloat () = అప్రమేయ;

  HBFloat (Type V)
  {
#ఒకవేళ HB_FAST_అంకె_ACCESS
    {
	      ఒకవేళ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
	      {
	        *((hb_packed_t<Type> *) v) = V;
	        తిరిగి;
	      }
	    }
#అయితే_ముగింపు

    సంఘం {
      hb_packed_t<Type> f;
      hb_packed_t<IntType> i;
    } u = {{V}};

    స్థిరరాశి HBInt<BE, IntType> I = (IntType) u.i;
    కొరకు (గుర్తులేని i = 0; i < బైట్లు_3; i++)
      v[i] = I.v[i];
  }

  /* c++14 constexpr */ క్రియాకారకం Type () స్థిరరాశి
  {
#ఒకవేళ HB_FAST_అంకె_ACCESS
	    {
	      ఒకవేళ (BE == (__BYTE_ORDER == __BIG_ENDIAN))
		తిరిగి (Type) *((స్థిరరాశి hb_packed_t<Type> *) v);
	    }
#అయితే_ముగింపు

    HBInt<BE, IntType> I;
    కొరకు (గుర్తులేని i = 0; i < బైట్లు_3; i++)
      I.v[i] = v[i];

    సంఘం {
      hb_packed_t<IntType> i;
      hb_packed_t<Type> f;
    } u = {{I}};

    తిరిగి (Type) u.f;
  }
  అంతరంగ: uint8_t v[బైట్లు_3];
};


/* We want our rounding towards +infinity. */
స్థిర అంతఃవరుస ద్విగుణ
_hb_roundf (ద్విగుణ x) { తిరిగి floor (x + .5); }

స్థిర అంతఃవరుస దశాంశం
_hb_roundf (దశాంశం x) { తిరిగి floor ((ద్విగుణ) x + .5); }

#నిర్వచించు roundf(x) _hb_roundf(x)

స్థిర అంతఃవరుస శూన్య
hb_sincos (దశాంశం rotation, దశాంశం &s, దశాంశం &c)
{
#నిర్వచితమైతే HAVE_SINCOSF
  sincosf (rotation, &s, &c);
#లేకపోతే
  c = cosf (rotation);
  s = sinf (rotation);
#అయితే_ముగింపు
}
స్థిర అంతఃవరుస శూన్య
hb_sincos (ద్విగుణ rotation, ద్విగుణ &s, ద్విగుణ &c)
{
#నిర్వచితమైతే HAVE_SINCOS
  sincos (rotation, &s, &c);
#లేకపోతే
  c = కోజ్యా (rotation);
  s = జ్యా (rotation);
#అయితే_ముగింపు
}


/* Encodes three unsigned integers in one 64-bit number.  If the inputs have more than 21 bits,
 * values will be truncated / overlap, and might not decode exactly. */
#నిర్వచించు HB_CODEPOINT_ENCODE3(x,y,z) (((uint64_t) (x) << 42) | ((uint64_t) (y) << 21) | (uint64_t) (z))
#నిర్వచించు HB_CODEPOINT_DECODE3_1(v) ((hb_codepoint_t) ((v) >> 42))
#నిర్వచించు HB_CODEPOINT_DECODE3_2(v) ((hb_codepoint_t) ((v) >> 21) & 0x1FFFFFu)
#నిర్వచించు HB_CODEPOINT_DECODE3_3(v) ((hb_codepoint_t) (v) & 0x1FFFFFu)

/* Custom encoding used by hb-ucd. */
#నిర్వచించు HB_CODEPOINT_ENCODE3_11_7_14(x,y,z) (((uint32_t) ((x) & 0x07FFu) << 21) | (((uint32_t) (y) & 0x007Fu) << 14) | (uint32_t) ((z) & 0x3FFFu))
#నిర్వచించు HB_CODEPOINT_DECODE3_11_7_14_1(v) ((hb_codepoint_t) ((v) >> 21))
#నిర్వచించు HB_CODEPOINT_DECODE3_11_7_14_2(v) ((hb_codepoint_t) (((v) >> 14) & 0x007Fu) | 0x0300)
#నిర్వచించు HB_CODEPOINT_DECODE3_11_7_14_3(v) ((hb_codepoint_t) (v) & 0x3FFFu)


నిర్మాణం
{
  /* Note.  This is dangerous in that if it's passed an rvalue, it returns rvalue-reference. */
  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (T&& v) స్థిరరాశి HB_AUTO_RETURN ( std::forward<T> (v) )
}
HB_FUNCOBJ (hb_తటస్థం);
నిర్మాణం
{
  /* Like identity(), but only retains lvalue-references.  Rvalues are returned as rvalues. */
  మూస <రకం_పేరు T> constexpr T&
  క్రియాకారకం () (T& v) స్థిరరాశి { తిరిగి v; }

  మూస <రకం_పేరు T> constexpr hb_remove_reference<T>
  క్రియాకారకం () (T&& v) స్థిరరాశి { తిరిగి v; }
}
HB_FUNCOBJ (hb_lidentity);
నిర్మాణం
{
  /* Like identity(), but always returns rvalue. */
  మూస <రకం_పేరు T> constexpr hb_remove_reference<T>
  క్రియాకారకం () (T&& v) స్థిరరాశి { తిరిగి v; }
}
HB_FUNCOBJ (hb_ridentity);

నిర్మాణం
{
  మూస <రకం_పేరు T> constexpr తార్కిక
  క్రియాకారకం () (T&& v) స్థిరరాశి { తిరిగి తార్కిక (std::forward<T> (v)); }
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
#నిర్వచించు fasthash_mix(h) (					\
			(శూన్య) ((h) ^= (h) >> 23),		\
			(శూన్య) ((h) *= 0x2127599bf4325c37ULL),	\
			(h) ^= (h) >> 47)

స్థిర అంతఃవరుస uint64_t fasthash64(స్థిరరాశి శూన్య *buf, size_t len, uint64_t విత్తనం)
{
	నిర్మాణం __attribute__((packed)) packed_uint64_t { uint64_t v; };
	స్థిరరాశి uint64_t    m = 0x880355f21e6d1965ULL;
	స్థిరరాశి packed_uint64_t *pos = (స్థిరరాశి packed_uint64_t *)buf;
	స్థిరరాశి packed_uint64_t *చివర = pos + (len / 8);
	స్థిరరాశి గుర్తులేని అక్షరం *pos2;
	uint64_t h = విత్తనం ^ (len * m);
	uint64_t v;

#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
	ఒకవేళ (((uintptr_t) pos & 7) == 0)
	{
	  అయితే (pos != చివర)
	  {
#సూచన GCC diagnostic push
#సూచన GCC diagnostic ignored "-Wcast-align"
	    v  = * (స్థిరరాశి uint64_t *) (pos++);
#సూచన GCC diagnostic తీయి
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}
	లేకపోతే
#అయితే_ముగింపు
	{
	  అయితే (pos != చివర)
	  {
	    v  = pos++->v;
	    h ^= fasthash_mix(v);
	    h *= m;
	  }
	}

	pos2 = (స్థిరరాశి గుర్తులేని అక్షరం*)pos;
	v = 0;

	ఎంపిక (len & 7) {
	స్థితి 7: v ^= (uint64_t)pos2[6] << 48; HB_FALLTHROUGH;
	స్థితి 6: v ^= (uint64_t)pos2[5] << 40; HB_FALLTHROUGH;
	స్థితి 5: v ^= (uint64_t)pos2[4] << 32; HB_FALLTHROUGH;
	స్థితి 4: v ^= (uint64_t)pos2[3] << 24; HB_FALLTHROUGH;
	స్థితి 3: v ^= (uint64_t)pos2[2] << 16; HB_FALLTHROUGH;
	స్థితి 2: v ^= (uint64_t)pos2[1] <<  8; HB_FALLTHROUGH;
	స్థితి 1: v ^= (uint64_t)pos2[0];
		h ^= fasthash_mix(v);
		h *= m;
	}

	తిరిగి fasthash_mix(h);
}

స్థిర అంతఃవరుస uint32_t fasthash32(స్థిరరాశి శూన్య *buf, size_t len, uint32_t విత్తనం)
{
	// the following trick converts the 64-bit hashcode to Fermat
	// residue, which shall retain information from both the higher
	// and lower parts of hashcode.
        uint64_t h = fasthash64(buf, len, విత్తనం);
	తిరిగి h - (h >> 32);
}

నిర్మాణం
{
  అంతరంగ:

  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  impl (స్థిరరాశి T& v, hb_ప్రాధాన్యత<2>) స్థిరరాశి HB_RETURN (uint32_t, hb_deref (v).hash ())

  // Horrible: std:hash() of integers seems to be identity in gcc / clang?!
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  //
  // For performance characteristics see:
  // https://github.com/harfbuzz/harfbuzz/pull/4228#issuecomment-1565079537
  మూస <రకం_పేరు T,
	    hb_enable_if (std::is_integral<T>::value && పరిమాణం (T) <= పరిమాణం (uint32_t))> constexpr స్వయంచాలక
  impl (స్థిరరాశి T& v, hb_ప్రాధాన్యత<1>) స్థిరరాశి HB_RETURN (uint32_t, (uint32_t) v * 2654435761u /* Knuh's multiplicative hash */)
  మూస <రకం_పేరు T,
	    hb_enable_if (std::is_integral<T>::value && పరిమాణం (T) > పరిమాణం (uint32_t))> constexpr స్వయంచాలక
  impl (స్థిరరాశి T& v, hb_ప్రాధాన్యత<1>) స్థిరరాశి HB_RETURN (uint32_t, (uint32_t) (v ^ (v >> 32)) * 2654435761u /* Knuth's multiplicative hash */)

  మూస <రకం_పేరు T,
	    hb_enable_if (std::is_floating_point<T>::value)> constexpr స్వయంచాలక
  impl (స్థిరరాశి T& v, hb_ప్రాధాన్యత<1>) స్థిరరాశి HB_RETURN (uint32_t, fasthash32 (std::addressof (v), పరిమాణం (T), 0xf437ffe6))

  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  impl (స్థిరరాశి T& v, hb_ప్రాధాన్యత<0>) స్థిరరాశి HB_RETURN (uint32_t, std::hash<hb_decay<decltype (hb_deref (v))>>{} (hb_deref (v)))

  బహిరంగ:

  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి T& v) స్థిరరాశి HB_RETURN (uint32_t, impl (v, hb_prioritize))
}
HB_FUNCOBJ (hb_hash);


నిర్మాణం
{
  అంతరంగ:

  /* Pointer-to-member-function. */
  మూస <రకం_పేరు Appl, రకం_పేరు T, రకం_పేరు ...Ts> స్వయంచాలక
  impl (Appl&& a, hb_ప్రాధాన్యత<2>, T &&v, Ts&&... ds) స్థిరరాశి HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v)).*std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  /* Pointer-to-member. */
  మూస <రకం_పేరు Appl, రకం_పేరు T> స్వయంచాలక
  impl (Appl&& a, hb_ప్రాధాన్యత<1>, T &&v) స్థిరరాశి HB_AUTO_RETURN
  ((hb_deref (std::forward<T> (v))).*std::forward<Appl> (a))

  /* Operator(). */
  మూస <రకం_పేరు Appl, రకం_పేరు ...Ts> స్వయంచాలక
  impl (Appl&& a, hb_ప్రాధాన్యత<0>, Ts&&... ds) స్థిరరాశి HB_AUTO_RETURN
  (hb_deref (std::forward<Appl> (a)) (std::forward<Ts> (ds)...))

  బహిరంగ:

  మూస <రకం_పేరు Appl, రకం_పేరు ...Ts> స్వయంచాలక
  క్రియాకారకం () (Appl&& a, Ts&&... ds) స్థిరరాశి HB_AUTO_RETURN
  (
    impl (std::forward<Appl> (a),
	  hb_prioritize,
	  std::forward<Ts> (ds)...)
  )
}
HB_FUNCOBJ (hb_invoke);

మూస <గుర్తులేని Pos, రకం_పేరు Appl, రకం_పేరు V>
నిర్మాణం hb_partial_t
{
  hb_partial_t (Appl a, V v) : a (a), v (v) {}

  static_assert (Pos > 0, "");

  మూస <రకం_పేరు ...Ts,
	    గుర్తులేని P = Pos,
	    hb_enable_if (P == 1)> స్వయంచాలక
  క్రియాకారకం () (Ts&& ...ds) -> decltype (hb_invoke (hb_declval (Appl),
						   hb_declval (V),
						   hb_declval (Ts)...))
  {
    తిరిగి hb_invoke (std::forward<Appl> (a),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }
  మూస <రకం_పేరు T0, రకం_పేరు ...Ts,
	    గుర్తులేని P = Pos,
	    hb_enable_if (P == 2)> స్వయంచాలక
  క్రియాకారకం () (T0&& d0, Ts&& ...ds) -> decltype (hb_invoke (hb_declval (Appl),
							    hb_declval (T0),
							    hb_declval (V),
							    hb_declval (Ts)...))
  {
    తిరిగి hb_invoke (std::forward<Appl> (a),
		      std::forward<T0> (d0),
		      std::forward<V> (v),
		      std::forward<Ts> (ds)...);
  }

  అంతరంగ:
  hb_reference_wrapper<Appl> a;
  V v;
};
మూస <గుర్తులేని Pos=1, రకం_పేరు Appl, రకం_పేరు V>
స్వయంచాలక hb_partial (Appl&& a, V&& v) HB_AUTO_RETURN
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
#నిర్వచితమైతే _MSC_VER
/* https://github.com/harfbuzz/harfbuzz/issues/1730 */ \
#నిర్వచించు HB_PARTIALIZE(Pos) \
  మూస <రకం_పేరు _T> \
  decltype(స్వయంచాలక) క్రియాకారకం () (_T&& _v) స్థిరరాశి \
  { తిరిగి hb_partial<Pos> (ఇది, std::forward<_T> (_v)); } \
  static_assert (నిజం, "")
#లేకపోతే
/* https://github.com/harfbuzz/harfbuzz/issues/1724 */
#నిర్వచించు HB_PARTIALIZE(Pos) \
  మూస <రకం_పేరు _T> \
  స్వయంచాలక క్రియాకారకం () (_T&& _v) స్థిరరాశి HB_AUTO_RETURN \
  (hb_partial<Pos> (+ఇది, std::forward<_T> (_v))) \
  static_assert (నిజం, "")
#అయితే_ముగింపు


నిర్మాణం
{
  అంతరంగ:

  మూస <రకం_పేరు Pred, రకం_పేరు Val> స్వయంచాలక
  impl (Pred&& p, Val &&v, hb_ప్రాధాన్యత<1>) స్థిరరాశి HB_AUTO_RETURN
  (
    hb_deref (std::forward<Pred> (p)).has (std::forward<Val> (v))
  )

  మూస <రకం_పేరు Pred, రకం_పేరు Val> స్వయంచాలక
  impl (Pred&& p, Val &&v, hb_ప్రాధాన్యత<0>) స్థిరరాశి HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Pred> (p),
	       std::forward<Val> (v))
  )

  బహిరంగ:

  మూస <రకం_పేరు Pred, రకం_పేరు Val> స్వయంచాలక
  క్రియాకారకం () (Pred&& p, Val &&v) స్థిరరాశి HB_RETURN (తార్కిక,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_has);

నిర్మాణం
{
  అంతరంగ:

  మూస <రకం_పేరు Pred, రకం_పేరు Val> స్వయంచాలక
  impl (Pred&& p, Val &&v, hb_ప్రాధాన్యత<1>) స్థిరరాశి HB_AUTO_RETURN
  (
    hb_has (std::forward<Pred> (p),
	    std::forward<Val> (v))
  )

  మూస <రకం_పేరు Pred, రకం_పేరు Val> స్వయంచాలక
  impl (Pred&& p, Val &&v, hb_ప్రాధాన్యత<0>) స్థిరరాశి HB_AUTO_RETURN
  (
    std::forward<Pred> (p) == std::forward<Val> (v)
  )

  బహిరంగ:

  మూస <రకం_పేరు Pred, రకం_పేరు Val> స్వయంచాలక
  క్రియాకారకం () (Pred&& p, Val &&v) స్థిరరాశి HB_RETURN (తార్కిక,
    impl (std::forward<Pred> (p),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_సరిపోలిక);

నిర్మాణం
{
  అంతరంగ:

  మూస <రకం_పేరు Proj, రకం_పేరు Val> స్వయంచాలక
  impl (Proj&& f, Val &&v, hb_ప్రాధాన్యత<2>) స్థిరరాశి HB_AUTO_RETURN
  (
    hb_deref (std::forward<Proj> (f)).get (std::forward<Val> (v))
  )

  మూస <రకం_పేరు Proj, రకం_పేరు Val> స్వయంచాలక
  impl (Proj&& f, Val &&v, hb_ప్రాధాన్యత<1>) స్థిరరాశి HB_AUTO_RETURN
  (
    hb_invoke (std::forward<Proj> (f),
	       std::forward<Val> (v))
  )

  మూస <రకం_పేరు Proj, రకం_పేరు Val> స్వయంచాలక
  impl (Proj&& f, Val &&v, hb_ప్రాధాన్యత<0>) స్థిరరాశి HB_AUTO_RETURN
  (
    std::forward<Proj> (f)[std::forward<Val> (v)]
  )

  బహిరంగ:

  మూస <రకం_పేరు Proj, రకం_పేరు Val> స్వయంచాలక
  క్రియాకారకం () (Proj&& f, Val &&v) స్థిరరాశి HB_AUTO_RETURN
  (
    impl (std::forward<Proj> (f),
	  std::forward<Val> (v),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_get);

నిర్మాణం
{
  అంతరంగ:

  మూస <రకం_పేరు T1, రకం_పేరు T2> స్వయంచాలక
  impl (T1&& v1, T2 &&v2, hb_ప్రాధాన్యత<3>) స్థిరరాశి HB_AUTO_RETURN
  (
    std::forward<T2> (v2).cmp (std::forward<T1> (v1)) == 0
  )

  మూస <రకం_పేరు T1, రకం_పేరు T2> స్వయంచాలక
  impl (T1&& v1, T2 &&v2, hb_ప్రాధాన్యత<2>) స్థిరరాశి HB_AUTO_RETURN
  (
    std::forward<T1> (v1).cmp (std::forward<T2> (v2)) == 0
  )

  మూస <రకం_పేరు T1, రకం_పేరు T2> స్వయంచాలక
  impl (T1&& v1, T2 &&v2, hb_ప్రాధాన్యత<1>) స్థిరరాశి HB_AUTO_RETURN
  (
    std::forward<T1> (v1) == std::forward<T2> (v2)
  )

  మూస <రకం_పేరు T1, రకం_పేరు T2> స్వయంచాలక
  impl (T1&& v1, T2 &&v2, hb_ప్రాధాన్యత<0>) స్థిరరాశి HB_AUTO_RETURN
  (
    std::forward<T2> (v2) == std::forward<T1> (v1)
  )

  బహిరంగ:

  మూస <రకం_పేరు T1, రకం_పేరు T2> స్వయంచాలక
  క్రియాకారకం () (T1&& v1, T2 &&v2) స్థిరరాశి HB_AUTO_RETURN
  (
    impl (std::forward<T1> (v1),
	  std::forward<T2> (v2),
	  hb_prioritize)
  )
}
HB_FUNCOBJ (hb_equal);

నిర్మాణం
{
  మూస <రకం_పేరు T> శూన్య
  క్రియాకారకం () (T& a, T& b) స్థిరరాశి
  {
    ఉపయోగించి std::swap; // allow ADL
    swap (a, b);
  }
}
HB_FUNCOBJ (hb_మార్పిడి);


మూస <రకం_పేరు T1, రకం_పేరు T2>
నిర్మాణం hb_pair_t
{
  రకనిర్వచనం T1 మొదటి_t;
  రకనిర్వచనం T2 రెండవ_t;
  రకనిర్వచనం hb_pair_t<T1, T2> pair_t;

  మూస <రకం_పేరు U1 = T1, రకం_పేరు U2 = T2,
	    hb_enable_if (std::is_default_constructible<U1>::value &&
			  std::is_default_constructible<U2>::value)>
  hb_pair_t () : మొదటి (), second () {}
  hb_pair_t (T1 a, T2 b) : మొదటి (std::forward<T1> (a)), second (std::forward<T2> (b)) {}

  మూస <రకం_పేరు Q1, రకం_పేరు Q2,
	    hb_enable_if (hb_is_convertible (T1, Q1) &&
			  hb_is_convertible (T2, Q2))>
  క్రియాకారకం hb_pair_t<Q1, Q2> () { తిరిగి hb_pair_t<Q1, Q2> (మొదటి, second); }

  hb_pair_t<T1, T2> reverse () స్థిరరాశి
  { తిరిగి hb_pair_t<T1, T2> (second, మొదటి); }

  తార్కిక క్రియాకారకం == (స్థిరరాశి pair_t& o) స్థిరరాశి { తిరిగి మొదటి == o.మొదటి && second == o.second; }
  తార్కిక క్రియాకారకం != (స్థిరరాశి pair_t& o) స్థిరరాశి { తిరిగి !(*ఇది == o); }
  తార్కిక క్రియాకారకం < (స్థిరరాశి pair_t& o) స్థిరరాశి { తిరిగి మొదటి < o.మొదటి || (మొదటి == o.మొదటి && second < o.second); }
  తార్కిక క్రియాకారకం >= (స్థిరరాశి pair_t& o) స్థిరరాశి { తిరిగి !(*ఇది < o); }
  తార్కిక క్రియాకారకం > (స్థిరరాశి pair_t& o) స్థిరరాశి { తిరిగి మొదటి > o.మొదటి || (మొదటి == o.మొదటి && second > o.second); }
  తార్కిక క్రియాకారకం <= (స్థిరరాశి pair_t& o) స్థిరరాశి { తిరిగి !(*ఇది > o); }

  స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb)
  {
    pair_t *a = (pair_t *) pa;
    pair_t *b = (pair_t *) pb;

    ఒకవేళ (a->మొదటి < b->మొదటి) తిరిగి -1;
    ఒకవేళ (a->మొదటి > b->మొదటి) తిరిగి +1;
    ఒకవేళ (a->second < b->second) తిరిగి -1;
    ఒకవేళ (a->second > b->second) తిరిగి +1;
    తిరిగి 0;
  }

  మిత్ర శూన్య swap (hb_pair_t& a, hb_pair_t& b) noexcept
  {
    hb_మార్పిడి (a.మొదటి, b.మొదటి);
    hb_మార్పిడి (a.second, b.second);
  }


  T1 మొదటి;
  T2 second;
};
మూస <రకం_పేరు T1, రకం_పేరు T2> స్థిర అంతఃవరుస hb_pair_t<T1, T2>
hb_pair (T1&& a, T2&& b) { తిరిగి hb_pair_t<T1, T2> (a, b); }

రకనిర్వచనం hb_pair_t<hb_codepoint_t, hb_codepoint_t> hb_codepoint_pair_t;

నిర్మాణం
{
  మూస <రకం_పేరు Pair> constexpr రకం_పేరు Pair::మొదటి_t
  క్రియాకారకం () (స్థిరరాశి Pair& pair) స్థిరరాశి { తిరిగి pair.మొదటి; }
}
HB_FUNCOBJ (hb_మొదటి);

నిర్మాణం
{
  మూస <రకం_పేరు Pair> constexpr రకం_పేరు Pair::రెండవ_t
  క్రియాకారకం () (స్థిరరాశి Pair& pair) స్థిరరాశి { తిరిగి pair.second; }
}
HB_FUNCOBJ (hb_రెండవ);

/* Note.  In min/max impl, we can use hb_type_identity<T> for second argument.
 * However, that would silently convert between different-signedness integers.
 * Instead we accept two different types, such that compiler can err if
 * comparing integers of different signedness. */
నిర్మాణం
{
  మూస <రకం_పేరు T, రకం_పేరు T2> constexpr స్వయంచాలక
  క్రియాకారకం () (T&& a, T2&& b) స్థిరరాశి HB_AUTO_RETURN
  (a <= b ? a : b)
}
HB_FUNCOBJ (hb_min);
నిర్మాణం
{
  మూస <రకం_పేరు T, రకం_పేరు T2> constexpr స్వయంచాలక
  క్రియాకారకం () (T&& a, T2&& b) స్థిరరాశి HB_AUTO_RETURN
  (a >= b ? a : b)
}
HB_FUNCOBJ (hb_max);
నిర్మాణం
{
  మూస <రకం_పేరు T, రకం_పేరు T2, రకం_పేరు T3> constexpr స్వయంచాలక
  క్రియాకారకం () (T&& x, T2&& min, T3&& max) స్థిరరాశి HB_AUTO_RETURN
  (hb_min (hb_max (std::forward<T> (x), std::forward<T2> (min)), std::forward<T3> (max)))
}
HB_FUNCOBJ (hb_clamp);

/* Signed saturating arithmetic. */
మూస <రకం_పేరు T>
స్థిర HB_ALWAYS_INLINE T
hb_saturate_కూడు (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#ఒకవేళ hb_has_builtin(__builtin_add_overflow)
  T ఫలితం;
  ఒకవేళ (likely (!__builtin_add_overflow (a, b, &ఫలితం)))
    తిరిగి ఫలితం;
#లేకపోతే
  ఒకవేళ (b > 0)
  {
    ఒకవేళ (likely (a <= hb_int_max (T) - b))
      తిరిగి a + b;
  }
  లేకపోతే ఒకవేళ (likely (a >= hb_int_min (T) - b))
    తిరిగి a + b;
#అయితే_ముగింపు

  తిరిగి a < 0 ? hb_int_min (T) : hb_int_max (T);
}

మూస <రకం_పేరు T>
స్థిర HB_ALWAYS_INLINE T
hb_saturate_sub (T a, T b)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

#ఒకవేళ hb_has_builtin(__builtin_sub_overflow)
  T ఫలితం;
  ఒకవేళ (likely (!__builtin_sub_overflow (a, b, &ఫలితం)))
    తిరిగి ఫలితం;
#లేకపోతే
  ఒకవేళ (b > 0)
  {
    ఒకవేళ (likely (a >= hb_int_min (T) + b))
      తిరిగి a - b;
  }
  లేకపోతే ఒకవేళ (likely (a <= hb_int_max (T) + b))
    తిరిగి a - b;
#అయితే_ముగింపు

  తిరిగి b < 0 ? hb_int_max (T) : hb_int_min (T);
}

మూస <రకం_పేరు T>
స్థిర HB_ALWAYS_INLINE T
hb_saturate_neg (T a)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  తిరిగి hb_saturate_sub ((T) 0, a);
}

/* Convert a floating-point value to integer type T, saturating to T's
 * range instead of relying on the undefined behavior of an out-of-range
 * float-to-int conversion.  NaN saturates to the minimum of T. */
మూస <రకం_పేరు T>
స్థిర HB_ALWAYS_INLINE T
hb_clamp_గమ్యం (ద్విగుణ v)
{
  తిరిగి (T) hb_clamp (v,
		       (ద్విగుణ) hb_int_min (T),
		       (ద్విగుణ) hb_int_max (T));
}

మూస <రకం_పేరు T>
స్థిర HB_ALWAYS_INLINE T
hb_clamp_గమ్యం (దశాంశం v)
{
  /* Widen explicitly: float cannot represent all integer bounds exactly,
   * so the clamp itself is done in double. */
  తిరిగి hb_clamp_గమ్యం<T> ((ద్విగుణ) v);
}

మూస <రకం_పేరు T>
స్థిర HB_ALWAYS_INLINE T
hb_clamp_గమ్యం (int64_t v)
{
  static_assert (std::is_integral<T>::value && std::is_signed<T>::value, "");

  తిరిగి (T) hb_clamp (v,
		       (int64_t) hb_int_min (T),
		       (int64_t) hb_int_max (T));
}

/*
 * Bithacks.
 */

/* Return the number of 1 bits in a uint8_t; faster than hb_popcount() */
స్థిర అంతఃవరుస గుర్తులేని
hb_popcount8 (uint8_t v)
{
  స్థిర స్థిరరాశి uint8_t popcount4[16] = {
    0, 1, 1, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 2, 3, 3, 4
  };
  తిరిగి popcount4[v & 0xF] + popcount4[v >> 4];
}

/* Return the number of 1 bits in v. */
మూస <రకం_పేరు T>
స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
hb_popcount (T v)
{
#ఒకవేళ hb_has_builtin(__builtin_popcount)
  ఒకవేళ (పరిమాణం (T) <= పరిమాణం (గుర్తులేని పూర్ణాంకం))
    తిరిగి __builtin_popcount (v);
#అయితే_ముగింపు

#ఒకవేళ hb_has_builtin(__builtin_popcountl)
  ఒకవేళ (పరిమాణం (T) <= పరిమాణం (గుర్తులేని దీర్ఘ))
    తిరిగి __builtin_popcountl (v);
#అయితే_ముగింపు

#ఒకవేళ hb_has_builtin(__builtin_popcountll)
  ఒకవేళ (పరిమాణం (T) <= పరిమాణం (గుర్తులేని దీర్ఘ దీర్ఘ))
    తిరిగి __builtin_popcountll (v);
#అయితే_ముగింపు

  ఒకవేళ (పరిమాణం (T) <= 4)
  {
    /* "HACKMEM 169" */
    uint32_t y;
    y = (v >> 1) &033333333333;
    y = v - y - ((y >>1) & 033333333333);
    తిరిగి (((y + (y >> 3)) & 030707070707) % 077);
  }

  ఒకవేళ (పరిమాణం (T) == 8)
  {
    uint64_t y = (uint64_t) v;
    y -= ((y >> 1) & 0x5555555555555555ull);
    y = (y & 0x3333333333333333ull) + (y >> 2 & 0x3333333333333333ull);
    తిరిగి ((y + (y >> 4)) & 0xf0f0f0f0f0f0f0full) * 0x101010101010101ull >> 56;
  }

  ఒకవేళ (పరిమాణం (T) == 16)
  {
    గుర్తులేని పూర్ణాంకం జరుపు = 64;
    తిరిగి hb_popcount<uint64_t> ((uint64_t) v) + hb_popcount ((uint64_t) (v >> జరుపు));
  }

  ప్రతిజ్ఞా (0);
  తిరిగి 0; /* Shut up stupid compiler. */
}

/* Returns the number of bits needed to store number */
మూస <రకం_పేరు T>
స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
hb_బిట్టు_storage (T v)
{
  ఒకవేళ (unlikely (!v)) తిరిగి 0;

#ఒకవేళ hb_has_builtin(__builtin_clz)
  ఒకవేళ (పరిమాణం (T) <= పరిమాణం (గుర్తులేని పూర్ణాంకం))
    తిరిగి పరిమాణం (గుర్తులేని పూర్ణాంకం) * 8 - __builtin_clz (v);
#అయితే_ముగింపు

#ఒకవేళ hb_has_builtin(__builtin_clzl)
  ఒకవేళ (పరిమాణం (T) <= పరిమాణం (గుర్తులేని దీర్ఘ))
    తిరిగి పరిమాణం (గుర్తులేని దీర్ఘ) * 8 - __builtin_clzl (v);
#అయితే_ముగింపు

#ఒకవేళ hb_has_builtin(__builtin_clzll)
  ఒకవేళ (పరిమాణం (T) <= పరిమాణం (గుర్తులేని దీర్ఘ దీర్ఘ))
    తిరిగి పరిమాణం (గుర్తులేని దీర్ఘ దీర్ఘ) * 8 - __builtin_clzll (v);
#అయితే_ముగింపు

#ఒకవేళ (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  ఒకవేళ (పరిమాణం (T) <= పరిమాణం (గుర్తులేని పూర్ణాంకం))
  {
    గుర్తులేని దీర్ఘ where;
    బిట్టు_పరిశీలన_తిరగేయి (&where, v);
    తిరిగి 1 + where;
  }
# ఒకవేళ defined(_WIN64)
  ఒకవేళ (పరిమాణం (T) <= 8)
  {
    గుర్తులేని దీర్ఘ where;
    బిట్టు_పరిశీలన_Reverse64 (&where, v);
    తిరిగి 1 + where;
  }
# endif
#అయితే_ముగింపు

  ఒకవేళ (పరిమాణం (T) <= 4)
  {
    /* "bithacks" */
    స్థిరరాశి గుర్తులేని పూర్ణాంకం b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    స్థిరరాశి గుర్తులేని పూర్ణాంకం S[] = {1, 2, 4, 8, 16};
    గుర్తులేని పూర్ణాంకం r = 0;
    కొరకు (పూర్ణాంకం i = 4; i >= 0; i--)
      ఒకవేళ (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    తిరిగి r + 1;
  }
  ఒకవేళ (పరిమాణం (T) <= 8)
  {
    /* "bithacks" */
    స్థిరరాశి uint64_t b[] = {0x2ULL, 0xCULL, 0xF0ULL, 0xFF00ULL, 0xFFFF0000ULL, 0xFFFFFFFF00000000ULL};
    స్థిరరాశి గుర్తులేని పూర్ణాంకం S[] = {1, 2, 4, 8, 16, 32};
    గుర్తులేని పూర్ణాంకం r = 0;
    కొరకు (పూర్ణాంకం i = 5; i >= 0; i--)
      ఒకవేళ (v & b[i])
      {
	v >>= S[i];
	r |= S[i];
      }
    తిరిగి r + 1;
  }
  ఒకవేళ (పరిమాణం (T) == 16)
  {
    గుర్తులేని పూర్ణాంకం జరుపు = 64;
    తిరిగి (v >> జరుపు) ? hb_బిట్టు_storage<uint64_t> ((uint64_t) (v >> జరుపు)) + జరుపు :
			  hb_బిట్టు_storage<uint64_t> ((uint64_t) v);
  }

  ప్రతిజ్ఞా (0);
  తిరిగి 0; /* Shut up stupid compiler. */
}

/* Returns the number of zero bits in the least significant side of v */
మూస <రకం_పేరు T>
స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
hb_ctz (T v)
{
  ఒకవేళ (unlikely (!v)) తిరిగి 8 * పరిమాణం (T);

#ఒకవేళ hb_has_builtin(__builtin_ctz)
  ఒకవేళ (పరిమాణం (T) <= పరిమాణం (గుర్తులేని పూర్ణాంకం))
    తిరిగి __builtin_ctz (v);
#అయితే_ముగింపు

#ఒకవేళ hb_has_builtin(__builtin_ctzl)
  ఒకవేళ (పరిమాణం (T) <= పరిమాణం (గుర్తులేని దీర్ఘ))
    తిరిగి __builtin_ctzl (v);
#అయితే_ముగింపు

#ఒకవేళ hb_has_builtin(__builtin_ctzll)
  ఒకవేళ (పరిమాణం (T) <= పరిమాణం (గుర్తులేని దీర్ఘ దీర్ఘ))
    తిరిగి __builtin_ctzll (v);
#అయితే_ముగింపు

#ఒకవేళ (defined(_MSC_VER) && _MSC_VER >= 1500) || (defined(__MINGW32__) && (__GNUC__ < 4))
  ఒకవేళ (పరిమాణం (T) <= పరిమాణం (గుర్తులేని పూర్ణాంకం))
  {
    గుర్తులేని దీర్ఘ where;
    బిట్టు_పరిశీలన_Forward (&where, v);
    తిరిగి where;
  }
# ఒకవేళ defined(_WIN64)
  ఒకవేళ (పరిమాణం (T) <= 8)
  {
    గుర్తులేని దీర్ఘ where;
    బిట్టు_పరిశీలన_Forward64 (&where, v);
    తిరిగి where;
  }
# endif
#అయితే_ముగింపు

  ఒకవేళ (పరిమాణం (T) <= 4)
  {
    /* "bithacks" */
    గుర్తులేని పూర్ణాంకం c = 32;
    v &= - (int32_t) v;
    ఒకవేళ (v) c--;
    ఒకవేళ (v & 0x0000FFFF) c -= 16;
    ఒకవేళ (v & 0x00FF00FF) c -= 8;
    ఒకవేళ (v & 0x0F0F0F0F) c -= 4;
    ఒకవేళ (v & 0x33333333) c -= 2;
    ఒకవేళ (v & 0x55555555) c -= 1;
    తిరిగి c;
  }
  ఒకవేళ (పరిమాణం (T) <= 8)
  {
    /* "bithacks" */
    గుర్తులేని పూర్ణాంకం c = 64;
    v &= - (int64_t) (v);
    ఒకవేళ (v) c--;
    ఒకవేళ (v & 0x00000000FFFFFFFFULL) c -= 32;
    ఒకవేళ (v & 0x0000FFFF0000FFFFULL) c -= 16;
    ఒకవేళ (v & 0x00FF00FF00FF00FFULL) c -= 8;
    ఒకవేళ (v & 0x0F0F0F0F0F0F0F0FULL) c -= 4;
    ఒకవేళ (v & 0x3333333333333333ULL) c -= 2;
    ఒకవేళ (v & 0x5555555555555555ULL) c -= 1;
    తిరిగి c;
  }
  ఒకవేళ (పరిమాణం (T) == 16)
  {
    గుర్తులేని పూర్ణాంకం జరుపు = 64;
    తిరిగి (uint64_t) v ? hb_బిట్టు_storage<uint64_t> ((uint64_t) v) :
			  hb_బిట్టు_storage<uint64_t> ((uint64_t) (v >> జరుపు)) + జరుపు;
  }

  ప్రతిజ్ఞా (0);
  తిరిగి 0; /* Shut up stupid compiler. */
}


/*
 * Tiny stuff.
 */

/* ASCII tag/character handling */
స్థిర అంతఃవరుస తార్కిక ISALPHA (గుర్తులేని అక్షరం c)
{ తిరిగి (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
స్థిర అంతఃవరుస తార్కిక ISALNUM (గుర్తులేని అక్షరం c)
{ తిరిగి (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }
స్థిర అంతఃవరుస తార్కిక ISSPACE (గుర్తులేని అక్షరం c)
{ తిరిగి c == ' ' || c =='\f'|| c =='\n'|| c =='\r'|| c =='\t'|| c =='\v'; }
స్థిర అంతఃవరుస గుర్తులేని అక్షరం TOUPPER (గుర్తులేని అక్షరం c)
{ తిరిగి (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c; }
స్థిర అంతఃవరుస గుర్తులేని అక్షరం TOLOWER (గుర్తులేని అక్షరం c)
{ తిరిగి (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }
స్థిర అంతఃవరుస తార్కిక ISHEX (గుర్తులేని అక్షరం c)
{ తిరిగి (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
స్థిర అంతఃవరుస గుర్తులేని అక్షరం TOHEX (uint8_t c)
{ తిరిగి (c & 0xF) <= 9 ? (c & 0xF) + '0' : (c & 0xF) + 'a' - 10; }
స్థిర అంతఃవరుస uint8_t FROMHEX (గుర్తులేని అక్షరం c)
{ తిరిగి (c >= '0' && c <= '9') ? c - '0' : TOLOWER (c) - 'a' + 10; }

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం DIV_CEIL (స్థిరరాశి గుర్తులేని పూర్ణాంకం a, గుర్తులేని పూర్ణాంకం b)
{ తిరిగి (a + (b - 1)) / b; }


#నిర్వచనం_రద్దు  ARRAY_నిడివి
మూస <రకం_పేరు Type, గుర్తులేని పూర్ణాంకం n>
స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం ARRAY_నిడివి (స్థిరరాశి Type (&)[n]) { తిరిగి n; }
/* A const version, but does not detect erratically being called on pointers. */
#నిర్వచించు ARRAY_నిడివి_CONST(__array) ((గుర్తుగల పూర్ణాంకం) (పరిమాణం (__array) / పరిమాణం (__array[0])))


స్థిర అంతఃవరుస శూన్య *
hb_memcpy (శూన్య *__restrict dst, స్థిరరాశి శూన్య *__restrict src, size_t len)
{
  /* It's illegal to pass 0 as size to memcpy. */
  ఒకవేళ (unlikely (!len)) తిరిగి dst;
  తిరిగి జ్ఞాపకం_నకలు (dst, src, len);
}

స్థిర అంతఃవరుస పూర్ణాంకం
hb_memcmp (స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b, గుర్తులేని పూర్ణాంకం len)
{
  /* It's illegal to pass NULL to memcmp(), even if len is zero.
   * So, wrap it.
   * https://sourceware.org/bugzilla/show_bug.cgi?id=23878 */
  ఒకవేళ (unlikely (!len)) తిరిగి 0;
  తిరిగి memcmp (a, b, len);
}

స్థిర అంతఃవరుస శూన్య *
hb_memset (శూన్య *s, పూర్ణాంకం c, గుర్తులేని పూర్ణాంకం n)
{
  /* It's illegal to pass NULL to memset(), even if n is zero. */
  ఒకవేళ (unlikely (!n)) తిరిగి s;
  తిరిగి memset (s, c, n);
}

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
hb_ceil_గమ్యం_4 (గుర్తులేని పూర్ణాంకం v)
{
  తిరిగి ((v - 1) | 3) + 1;
}

మూస <రకం_పేరు T> స్థిర అంతఃవరుస తార్కిక
hb_లోపల_range (T u, T lo, T hi)
{
  static_assert (!std::is_signed<T>::value, "");

  /* The casts below are important as if T is smaller than int,
   * the subtract results will become a signed int! */
  తిరిగి (T)(u - lo) <= (T)(hi - lo);
}
మూస <రకం_పేరు T> స్థిర అంతఃవరుస తార్కిక
hb_లోపల_ranges (T u, T lo1, T hi1)
{
  తిరిగి hb_లోపల_range (u, lo1, hi1);
}
మూస <రకం_పేరు T, రకం_పేరు ...Ts> స్థిర అంతఃవరుస తార్కిక
hb_లోపల_ranges (T u, T lo1, T hi1, Ts... ds)
{
  తిరిగి hb_లోపల_range<T> (u, lo1, hi1) || hb_లోపల_ranges<T> (u, ds...);
}


/*
 * Overflow checking.
 */

స్థిర అంతఃవరుస తార్కిక
hb_unsigned_mul_overflows (గుర్తులేని పూర్ణాంకం లెక్క, గుర్తులేని పూర్ణాంకం కొలత, గుర్తులేని *ఫలితం = శూన్యసూచిక)
{
#ఒకవేళ hb_has_builtin(__builtin_mul_overflow)
  గుర్తులేని దొంతర_ఫలితం;
  ఒకవేళ (!ఫలితం)
    ఫలితం = &దొంతర_ఫలితం;
  తిరిగి __builtin_mul_overflow (లెక్క, కొలత, ఫలితం);
#అయితే_ముగింపు

  ఒకవేళ (ఫలితం)
    *ఫలితం = లెక్క * కొలత;
  తిరిగి (కొలత > 0) && (లెక్క >= ((గుర్తులేని పూర్ణాంకం) -1) / కొలత);
}

స్థిర అంతఃవరుస తార్కిక
hb_unsigned_కూడు_overflows (గుర్తులేని పూర్ణాంకం a, గుర్తులేని పూర్ణాంకం b, గుర్తులేని *ఫలితం = శూన్యసూచిక)
{
#ఒకవేళ hb_has_builtin(__builtin_add_overflow)
  గుర్తులేని దొంతర_ఫలితం;
  ఒకవేళ (!ఫలితం)
    ఫలితం = &దొంతర_ఫలితం;
  తిరిగి __builtin_add_overflow (a, b, ఫలితం);
#అయితే_ముగింపు

  ఒకవేళ (ఫలితం)
    *ఫలితం = a + b;
  తిరిగి b > (గుర్తులేని పూర్ణాంకం) -1 - a;
}

/* Saturating arithmetic on size_t.  On platforms where size_t is wider than
 * unsigned int (i.e. 64-bit), the inputs to get_size()-style computations
 * (counts and static_sizes which are at most 32-bit) cannot overflow size_t,
 * so these reduce to plain arithmetic.  On 32-bit platforms (size_t is
 * unsigned int), they saturate to SIZE_MAX so sanitize/serialize callers
 * naturally reject the resulting size as out-of-range. */

స్థిర అంతఃవరుస size_t
hb_unsigned_mul_saturate (size_t a, size_t b)
{
  ఒకవేళ (పరిమాణం (size_t) > పరిమాణం (గుర్తులేని పూర్ణాంకం))
    తిరిగి a * b;
#ఒకవేళ hb_has_builtin(__builtin_mul_overflow)
  size_t ఫలితం;
  ఒకవేళ (__builtin_mul_overflow (a, b, &ఫలితం))
    తిరిగి (size_t) -1;
  తిరిగి ఫలితం;
#లేకపోతే
  ఒకవేళ (b > 0 && a > ((size_t) -1) / b) తిరిగి (size_t) -1;
  తిరిగి a * b;
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస size_t
hb_unsigned_కూడు_saturate (size_t a, size_t b)
{
  ఒకవేళ (పరిమాణం (size_t) > పరిమాణం (గుర్తులేని పూర్ణాంకం))
    తిరిగి a + b;
#ఒకవేళ hb_has_builtin(__builtin_add_overflow)
  size_t ఫలితం;
  ఒకవేళ (__builtin_add_overflow (a, b, &ఫలితం))
    తిరిగి (size_t) -1;
  తిరిగి ఫలితం;
#లేకపోతే
  ఒకవేళ (b > ((size_t) -1) - a) తిరిగి (size_t) -1;
  తిరిగి a + b;
#అయితే_ముగింపు
}

/* Variadic forms: fold left across all arguments. */
మూస <రకం_పేరు ...Ts>
స్థిర అంతఃవరుస size_t
hb_unsigned_mul_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ తిరిగి hb_unsigned_mul_saturate (hb_unsigned_mul_saturate (a, b), c, rest...); }

మూస <రకం_పేరు ...Ts>
స్థిర అంతఃవరుస size_t
hb_unsigned_కూడు_saturate (size_t a, size_t b, size_t c, Ts... rest)
{ తిరిగి hb_unsigned_కూడు_saturate (hb_unsigned_కూడు_saturate (a, b), c, rest...); }

/* Saturating mul-add: a * b + c.  Covers the dominant get_size() pattern
 * `count * static_size + min_size`. */
స్థిర అంతఃవరుస size_t
hb_unsigned_mul_కూడు_saturate (size_t a, size_t b, size_t c)
{ తిరిగి hb_unsigned_కూడు_saturate (hb_unsigned_mul_saturate (a, b), c); }


/*
 * Sort and search.
 */

మూస <రకం_పేరు K, రకం_పేరు V, రకం_పేరు ...Ts>
స్థిర పూర్ణాంకం
hb_cmp_పద్ధతి (స్థిరరాశి శూన్య *pkey, స్థిరరాశి శూన్య *pval, Ts... ds)
{
  స్థిరరాశి K& key = * (స్థిరరాశి K*) pkey;
  స్థిరరాశి V& val = * (స్థిరరాశి V*) pval;

  తిరిగి val.cmp (key, ds...);
}

మూస <రకం_పేరు K, రకం_పేరు V>
స్థిర పూర్ణాంకం
_hb_cmp_operator (స్థిరరాశి శూన్య *pkey, స్థిరరాశి శూన్య *pval)
{
  స్థిరరాశి K& key = * (స్థిరరాశి K*) pkey;
  స్థిరరాశి V& val = * (స్థిరరాశి V*) pval;

  ఒకవేళ (key < val) తిరిగి -1;
  ఒకవేళ (key > val) తిరిగి  1;
  తిరిగి 0;
}

మూస <రకం_పేరు V, రకం_పేరు K, రకం_పేరు ...Ts>
HB_HOT
స్థిర అంతఃవరుస తార్కిక
hb_bsearch_impl (గుర్తులేని *pos, /* Out */
		 స్థిరరాశి K& key,
		 V* పునాది, size_t nmemb, size_t stride,
		 పూర్ణాంకం (*compar)(స్థిరరాశి శూన్య *కీలకం_3, స్థిరరాశి శూన్య *అంశం_2, Ts... _ds),
		 Ts... ds)
{
  /* This is our *only* bsearch implementation. */

  పూర్ణాంకం min = 0, max = (పూర్ణాంకం) nmemb - 1;
  అయితే (min <= max)
  {
    పూర్ణాంకం mid = ((గుర్తులేని పూర్ణాంకం) min + (గుర్తులేని పూర్ణాంకం) max) / 2;
#సూచన GCC diagnostic push
#సూచన GCC diagnostic ignored "-Wcast-align"
    V* p = (V*) (((స్థిరరాశి అక్షరం *) పునాది) + (mid * stride));
#సూచన GCC diagnostic తీయి
    పూర్ణాంకం c = compar ((స్థిరరాశి శూన్య *) std::addressof (key), (స్థిరరాశి శూన్య *) p, ds...);
    ఒకవేళ (c < 0)
      max = mid - 1;
    లేకపోతే ఒకవేళ (c > 0)
      min = mid + 1;
    లేకపోతే
    {
      *pos = mid;
      తిరిగి నిజం;
    }
  }
  *pos = min;
  తిరిగి అబద్ధం;
}

మూస <రకం_పేరు V, రకం_పేరు K>
స్థిర అంతఃవరుస V*
hb_bsearch (స్థిరరాశి K& key, V* పునాది,
	    size_t nmemb, size_t stride = పరిమాణం (V),
	    పూర్ణాంకం (*compar)(స్థిరరాశి శూన్య *కీలకం_3, స్థిరరాశి శూన్య *అంశం_2) = hb_cmp_పద్ధతి<K, V>)
{
  గుర్తులేని pos;
#సూచన GCC diagnostic push
#సూచన GCC diagnostic ignored "-Wcast-align"
  తిరిగి hb_bsearch_impl (&pos, key, పునాది, nmemb, stride, compar) ?
	 (V*) (((స్థిరరాశి అక్షరం *) పునాది) + (pos * stride)) : శూన్యసూచిక;
#సూచన GCC diagnostic తీయి
}
మూస <రకం_పేరు V, రకం_పేరు K, రకం_పేరు ...Ts>
స్థిర అంతఃవరుస V*
hb_bsearch (స్థిరరాశి K& key, V* పునాది,
	    size_t nmemb, size_t stride,
	    పూర్ణాంకం (*compar)(స్థిరరాశి శూన్య *కీలకం_3, స్థిరరాశి శూన్య *అంశం_2, Ts... _ds),
	    Ts... ds)
{
  గుర్తులేని pos;
#సూచన GCC diagnostic push
#సూచన GCC diagnostic ignored "-Wcast-align"
  తిరిగి hb_bsearch_impl (&pos, key, పునాది, nmemb, stride, compar, ds...) ?
	 (V*) (((స్థిరరాశి అక్షరం *) పునాది) + (pos * stride)) : శూన్యసూచిక;
#సూచన GCC diagnostic తీయి
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
మూస <రకం_పేరు T, రకం_పేరు Compar>
స్థిర అంతఃవరుస శూన్య
hb_qsort_loop (T *పునాది, size_t nel, Compar compar)
{
  అయితే (nel > 24)
  {
    T *చివరి = పునాది + nel - 1;
    T *mid = పునాది + nel / 2;

    /* Median-of-three pivot, parked at last-1. */
    ఒకవేళ (compar (*పునాది, *mid) > 0) hb_మార్పిడి (*పునాది, *mid);
    ఒకవేళ (compar (*mid, *చివరి) > 0)
    {
      hb_మార్పిడి (*mid, *చివరి);
      ఒకవేళ (compar (*పునాది, *mid) > 0) hb_మార్పిడి (*పునాది, *mid);
    }
    hb_మార్పిడి (*mid, *(చివరి - 1));
    T &ఇరుసు = *(చివరి - 1);

    /* Two-way Hoare partition.  Inner loops are unguarded:
     * median-of-three left *base <= pivot and *last >= pivot,
     * which act as sentinels. */
    T *i = పునాది, *j = చివరి - 1;
    అయితే (నిజం)
    {
      అయితే (compar (*++i, ఇరుసు) < 0) {}
      అయితే (compar (*--j, ఇరుసు) > 0) {}
      ఒకవేళ (i >= j) ఆపు;
      hb_మార్పిడి (*i, *j);
    }
    hb_మార్పిడి (*i, *(చివరి - 1));

    /* Recurse on smaller side, loop on larger — bounds stack
     * depth at O(log n). */
    size_t ఎడమ  = (size_t) (i - పునాది);
    size_t కుడి = nel - ఎడమ - 1;
    ఒకవేళ (ఎడమ < కుడి)
    {
      hb_qsort_loop (పునాది, ఎడమ, compar);
      పునాది = i + 1;
      nel  = కుడి;
    }
    లేకపోతే
    {
      hb_qsort_loop (i + 1, కుడి, compar);
      nel  = ఎడమ;
    }
  }
}

మూస <రకం_పేరు T, రకం_పేరు Compar>
స్థిర అంతఃవరుస శూన్య
hb_qsort_inline (T *పునాది, size_t nel, Compar compar)
{
  hb_qsort_loop (పునాది, nel, compar);

  /* Single final insertion sort over the whole array.  After
   * the partitioning loop, every element is within the threshold
   * of its sorted position, so this pass is O(n * threshold). */
  T *చివర = పునాది + nel;
  కొరకు (T *pi = పునాది + 1; pi < చివర; pi++)
    కొరకు (T *pj = pi; pj > పునాది && compar (pj[-1], pj[0]) > 0; pj--)
      hb_మార్పిడి (pj[-1], pj[0]);
}

స్థిర అంతఃవరుస శూన్య
hb_qsort (శూన్య *పునాది, size_t nel, size_t వెడల్పు,
	  పూర్ణాంకం (*compar)(స్థిరరాశి శూన్య *_a, స్థిరరాశి శూన్య *_b))
{
  qsort (పునాది, nel, వెడల్పు, compar);
}




మూస <రకం_పేరు T, రకం_పేరు T2, రకం_పేరు T3 = పూర్ణాంకం> స్థిర అంతఃవరుస శూన్య
hb_stable_క్రమీకరించు (T *array, గుర్తులేని పూర్ణాంకం len, పూర్ణాంకం(*compar)(స్థిరరాశి T2 *, స్థిరరాశి T2 *), T3 *array2 = శూన్యసూచిక)
{
  static_assert (hb_is_trivially_copy_assignable (T), "");
  static_assert (hb_is_trivially_copy_assignable (T3), "");

  కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < len; i++)
  {
    గుర్తులేని పూర్ణాంకం j = i;
    అయితే (j && compar (&array[j - 1], &array[i]) > 0)
      j--;
    ఒకవేళ (i == j)
      కొనసాగు;
    /* Move item i to occupy place for item j, shift what's in between. */
    {
      T t = array[i];
      memmove (&array[j + 1], &array[j], (i - j) * పరిమాణం (T));
      array[j] = t;
    }
    ఒకవేళ (array2)
    {
      T3 t = array2[i];
      memmove (&array2[j + 1], &array2[j], (i - j) * పరిమాణం (T3));
      array2[j] = t;
    }
  }
}

స్థిర అంతఃవరుస hb_bool_t
hb_codepoint_parse (స్థిరరాశి అక్షరం *s, గుర్తులేని పూర్ణాంకం len, పూర్ణాంకం పునాది, hb_codepoint_t *బహిర్గతం)
{
  గుర్తులేని పూర్ణాంకం v;
  స్థిరరాశి అక్షరం *p = s;
  స్థిరరాశి అక్షరం *చివర = p + len;
  ఒకవేళ (unlikely (!hb_parse_uint (&p, చివర, &v, నిజం/* whole buffer */, పునాది)))
    తిరిగి అబద్ధం;

  *బహిర్గతం = v;
  తిరిగి నిజం;
}


/* Operators. */

నిర్మాణం
{ HB_PARTIALIZE(2);
  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి T &a, స్థిరరాశి T &b) స్థిరరాశి HB_AUTO_RETURN (a & b)
}
HB_FUNCOBJ (hb_bitwise_and);
నిర్మాణం
{ HB_PARTIALIZE(2);
  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి T &a, స్థిరరాశి T &b) స్థిరరాశి HB_AUTO_RETURN (a | b)
}
HB_FUNCOBJ (hb_bitwise_or);
నిర్మాణం
{ HB_PARTIALIZE(2);
  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి T &a, స్థిరరాశి T &b) స్థిరరాశి HB_AUTO_RETURN (a ^ b)
}
HB_FUNCOBJ (hb_bitwise_xor);
నిర్మాణం
{ HB_PARTIALIZE(2);
  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి T &a, స్థిరరాశి T &b) స్థిరరాశి HB_AUTO_RETURN (~a & b)
}
HB_FUNCOBJ (hb_bitwise_lt);
నిర్మాణం
{ HB_PARTIALIZE(2);
  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి T &a, స్థిరరాశి T &b) స్థిరరాశి HB_AUTO_RETURN (a & ~b)
}
HB_FUNCOBJ (hb_bitwise_gt); // aka sub
నిర్మాణం
{ HB_PARTIALIZE(2);
  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి T &a, స్థిరరాశి T &b) స్థిరరాశి HB_AUTO_RETURN (~a | b)
}
HB_FUNCOBJ (hb_bitwise_le);
నిర్మాణం
{ HB_PARTIALIZE(2);
  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి T &a, స్థిరరాశి T &b) స్థిరరాశి HB_AUTO_RETURN (a | ~b)
}
HB_FUNCOBJ (hb_bitwise_ge);
నిర్మాణం
{
  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి T &a) స్థిరరాశి HB_AUTO_RETURN (~a)
}
HB_FUNCOBJ (hb_bitwise_neg);

నిర్మాణం
{ HB_PARTIALIZE(2);
  మూస <రకం_పేరు T, రకం_పేరు T2> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి T &a, స్థిరరాశి T2 &b) స్థిరరాశి HB_AUTO_RETURN (a + b)
}
HB_FUNCOBJ (hb_కూడు);
నిర్మాణం
{ HB_PARTIALIZE(2);
  మూస <రకం_పేరు T, రకం_పేరు T2> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి T &a, స్థిరరాశి T2 &b) స్థిరరాశి HB_AUTO_RETURN (a - b)
}
HB_FUNCOBJ (hb_sub);
నిర్మాణం
{ HB_PARTIALIZE(2);
  మూస <రకం_పేరు T, రకం_పేరు T2> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి T &a, స్థిరరాశి T2 &b) స్థిరరాశి HB_AUTO_RETURN (b - a)
}
HB_FUNCOBJ (hb_rsub);
నిర్మాణం
{ HB_PARTIALIZE(2);
  మూస <రకం_పేరు T, రకం_పేరు T2> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి T &a, స్థిరరాశి T2 &b) స్థిరరాశి HB_AUTO_RETURN (a * b)
}
HB_FUNCOBJ (hb_mul);
నిర్మాణం
{ HB_PARTIALIZE(2);
  మూస <రకం_పేరు T, రకం_పేరు T2> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి T &a, స్థిరరాశి T2 &b) స్థిరరాశి HB_AUTO_RETURN (a / b)
}
HB_FUNCOBJ (hb_div);
నిర్మాణం
{ HB_PARTIALIZE(2);
  మూస <రకం_పేరు T, రకం_పేరు T2> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి T &a, స్థిరరాశి T2 &b) స్థిరరాశి HB_AUTO_RETURN (a % b)
}
HB_FUNCOBJ (hb_mod);
నిర్మాణం
{
  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి T &a) స్థిరరాశి HB_AUTO_RETURN (+a)
}
HB_FUNCOBJ (hb_pos);
నిర్మాణం
{
  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి T &a) స్థిరరాశి HB_AUTO_RETURN (-a)
}
HB_FUNCOBJ (hb_neg);
నిర్మాణం
{
  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (T &a) స్థిరరాశి HB_AUTO_RETURN (++a)
}
HB_FUNCOBJ (hb_inc);
నిర్మాణం
{
  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (T &a) స్థిరరాశి HB_AUTO_RETURN (--a)
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
మూస <రకం_పేరు func_t>
ద్విగుణ solve_itp (func_t f,
		  ద్విగుణ a, ద్విగుణ b,
		  ద్విగుణ epsilon,
		  ద్విగుణ min_y, ద్విగుణ max_y,
		  ద్విగుణ &ya, ద్విగుణ &yb, ద్విగుణ &y)
{
  // Guard against degenerate interval
  ఒకవేళ (b - a <= 0.0)
  {
    y = ya;
    తిరిగి a;
  }

  గుర్తులేని n1_2 = (గుర్తులేని) (hb_max (ceil (log2 ((b - a) / epsilon)) - 1.0, 0.0));
  స్థిరరాశి గుర్తులేని n0 = 1; // Hardwired
  స్థిరరాశి ద్విగుణ k1 = 0.2 / (b - a); // Hardwired.
  గుర్తులేని nmax = n0 + n1_2;
  ద్విగుణ scaled_epsilon = epsilon * ద్విగుణ (1llu << nmax);
  ద్విగుణ _2_epsilon = 2.0 * epsilon;
  అయితే (b - a > _2_epsilon)
  {
    ద్విగుణ x1_2 = 0.5 * (a + b);
    ద్విగుణ r = scaled_epsilon - 0.5 * (b - a);
    // Guard against yb == ya to prevent division by zero
    ద్విగుణ xf = (yb != ya) ? (yb * a - ya * b) / (yb - ya) : x1_2;
    ద్విగుణ sigma = x1_2 - xf;
    ద్విగుణ b_a = b - a;
    // This has k2 = 2 hardwired for efficiency.
    ద్విగుణ b_a_k2 = b_a * b_a;
    ద్విగుణ delta = k1 * b_a_k2;
    పూర్ణాంకం sigma_sign = sigma >= 0 ? +1 : -1;
    ద్విగుణ xt = delta <= fabs (x1_2 - xf) ? xf + delta * sigma_sign : x1_2;
    ద్విగుణ xitp = fabs (xt - x1_2) <= r ? xt : x1_2 - r * sigma_sign;
    ద్విగుణ yitp = f (xitp);
    ఒకవేళ (yitp > max_y)
    {
      b = xitp;
      yb = yitp;
    }
    లేకపోతే ఒకవేళ (yitp < min_y)
    {
      a = xitp;
      ya = yitp;
    }
    లేకపోతే
    {
      y = yitp;
      తిరిగి xitp;
    }
    scaled_epsilon *= 0.5;
  }
  తిరిగి 0.5 * (a + b);
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
మూస <రకం_పేరు F>
నిర్మాణం hb_scope_guard_t
{
  స్పష్టమైన hb_scope_guard_t (F &&f) : f (std::move (f)), చురుకైన (నిజం) {}
  hb_scope_guard_t (hb_scope_guard_t &&o) noexcept
    : f (std::move (o.f)), చురుకైన (o.చురుకైన) { o.చురుకైన = అబద్ధం; }
  hb_scope_guard_t (స్థిరరాశి hb_scope_guard_t &) = తొలగించు;
  hb_scope_guard_t &క్రియాకారకం= (స్థిరరాశి hb_scope_guard_t &) = తొలగించు;
  hb_scope_guard_t &క్రియాకారకం= (hb_scope_guard_t &&) = తొలగించు;
  ~hb_scope_guard_t () { ఒకవేళ (చురుకైన) f (); }

  /* Release: dismiss the guard so the cleanup does NOT run.  Use
   * when transferring ownership out of the scope. */
  శూన్య release () { చురుకైన = అబద్ధం; }

  అంతరంగ:
  F f;
  తార్కిక చురుకైన;
};

మూస <రకం_పేరు F>
స్థిర అంతఃవరుస hb_scope_guard_t<F> hb_make_scope_guard (F &&f)
{ తిరిగి hb_scope_guard_t<F> (std::forward<F> (f)); }

#నిర్వచించు HB_SCOPE_GUARD_NAME_(పంక్తి) hb_scope_guard_##పంక్తి
#నిర్వచించు HB_SCOPE_GUARD_NAME(పంక్తి) HB_SCOPE_GUARD_NAME_(పంక్తి)
#నిర్వచించు HB_SCOPE_GUARD(stmt) \
  స్వయంచాలక HB_SCOPE_GUARD_NAME(__LINE__) = \
    hb_make_scope_guard ([&]() { stmt; }); \
  (శూన్య) HB_SCOPE_GUARD_NAME(__LINE__)


#అయితే_ముగింపు /* HB_ALGS_HH */
