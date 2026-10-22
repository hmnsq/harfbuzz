/*
 * Copyright © 2007,2008,2009,2010  Red Hat, Inc.
 * Copyright © 2012  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_OPEN_TYPE_HH
#define HB_OPEN_TYPE_HH

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-blob.hh"
#ଅନ୍ତର୍ଭାଵ "hb-face.hh"
#ଅନ୍ତର୍ଭାଵ "hb-machinery.hh"
#ଅନ୍ତର୍ଭାଵ "hb-meta.hh"
#ଅନ୍ତର୍ଭାଵ "hb-subset.hh"


ନାମାକାଶ OT {


/*
 *
 * The OpenType Font File: Data Types
 */


/* "The following data types are used in the OpenType font file.
 *  All OpenType fonts use Motorola-style byte ordering (Big Endian):" */

/*
 * Int types
 */

/* Integer types in big-endian order and no alignment requirement */
template <ତାର୍କିକ BE,
	  typename Type,
	  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା Size = ଆକାର (Type)>
ସଂରଚନା ଅଙ୍କ_Type
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename std::conditional<std::is_integral<Type>::value && ଆକାର (Type) <= ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା),
				     typename std::conditional<std::is_signed<Type>::value, signed, unsigned>::type,
				     Type>::type WideType;

  ଅଙ୍କ_Type () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  explicit constexpr ଅଙ୍କ_Type (Type V) : v {V} {}
  ଅଙ୍କ_Type& operator = (Type V) { v = V; ଫେରନ୍ତୁ *ଏହି; }

  operator WideType () ଅଚଳ { ଫେରନ୍ତୁ v; }

  ତାର୍କିକ operator == (ଅଚଳ ଅଙ୍କ_Type &o) ଅଚଳ { ଫେରନ୍ତୁ (Type) v == (Type) o.v; }
  ତାର୍କିକ operator != (ଅଚଳ ଅଙ୍କ_Type &o) ଅଚଳ { ଫେରନ୍ତୁ !(*ଏହି == o); }

  ଅଙ୍କ_Type& operator += (WideType ଗଣନା_2) { *ଏହି = *ଏହି + ଗଣନା_2; ଫେରନ୍ତୁ *ଏହି; }
  ଅଙ୍କ_Type& operator -= (WideType ଗଣନା_2) { *ଏହି = *ଏହି - ଗଣନା_2; ଫେରନ୍ତୁ *ଏହି; }
  ଅଙ୍କ_Type& operator ++ () { *ଏହି += 1; ଫେରନ୍ତୁ *ଏହି; }
  ଅଙ୍କ_Type& operator -- () { *ଏହି -= 1; ଫେରନ୍ତୁ *ଏହି; }
  ଅଙ୍କ_Type operator ++ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) { ଅଙ୍କ_Type c (*ଏହି); ++*ଏହି; ଫେରନ୍ତୁ c; }
  ଅଙ୍କ_Type operator -- (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) { ଅଙ୍କ_Type c (*ଏହି); --*ଏହି; ଫେରନ୍ତୁ c; }

  uint32_t hash () ଅଚଳ { ଫେରନ୍ତୁ hb_array ((ଅଚଳ ଅକ୍ଷର *) &v, ଆକାର (v)).hash (); }
  HB_INTERNAL ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ ଅଙ୍କ_Type *a, ଅଚଳ ଅଙ୍କ_Type *b)
  { ଫେରନ୍ତୁ b->cmp (*a); }
  HB_INTERNAL ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
  {
    ଅଙ୍କ_Type *pa = (ଅଙ୍କ_Type *) a;
    ଅଙ୍କ_Type *pb = (ଅଙ୍କ_Type *) b;

    ଫେରନ୍ତୁ pb->cmp (*pa);
  }
  template <typename Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (Type2 a) ଅଚଳ
  {
    Type b = v;
    ଫେରନ୍ତୁ (a > b) - (a < b);
  }
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }
  ସୁରକ୍ଷିତ:
  typename std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, Size>,
			    HBFloat<BE, Type, Size>>::type v;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (Size);
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ସତ୍ୟ, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ସତ୍ୟ, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ସତ୍ୟ, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ସତ୍ୟ, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ସତ୍ୟ, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ସତ୍ୟ, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ସତ୍ୟ, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ସତ୍ୟ, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ସତ୍ୟ, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ମିଥ୍ୟା, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ମିଥ୍ୟା, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ମିଥ୍ୟା, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ମିଥ୍ୟା, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ମିଥ୍ୟା, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ମିଥ୍ୟା, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ସତ୍ୟ,  ଦଶମିକ>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ସତ୍ୟ,  ଦ୍ୱିଗୁଣ> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ମିଥ୍ୟା, ଦଶମିକ>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଙ୍କ_Type<ମିଥ୍ୟା, ଦ୍ୱିଗୁଣ> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
ସଂରଚନା HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& operator = (uint16_t i ) { HBUINT16::operator= (i); ଫେରନ୍ତୁ *ଏହି; }
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
ସଂରଚନା HBUINT32VAR
{
  size_t get_size () ଅଚଳ
  {
    unsigned b0 = v[0];
    ଯଦି (b0 < 0x80)
      ଫେରନ୍ତୁ 1;
    ଅନ୍ୟଥା ଯଦି (b0 < 0xC0)
      ଫେରନ୍ତୁ 2;
    ଅନ୍ୟଥା ଯଦି (b0 < 0xE0)
      ଫେରନ୍ତୁ 3;
    ଅନ୍ୟଥା ଯଦି (b0 < 0xF0)
      ଫେରନ୍ତୁ 4;
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ 5;
  }

  ସ୍ଥିର size_t get_size (uint32_t v)
  {
    ଯଦି (v < 0x80)
      ଫେରନ୍ତୁ 1;
    ଅନ୍ୟଥା ଯଦି (v < 0x4000)
      ଫେରନ୍ତୁ 2;
    ଅନ୍ୟଥା ଯଦି (v < 0x200000)
      ଫେରନ୍ତୁ 3;
    ଅନ୍ୟଥା ଯଦି (v < 0x10000000)
      ଫେରନ୍ତୁ 4;
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ 5;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_size ()));
  }

  operator uint32_t () ଅଚଳ
  {
    unsigned b0 = v[0];
    ଯଦି (b0 < 0x80)
      ଫେରନ୍ତୁ b0;
    ଅନ୍ୟଥା ଯଦି (b0 < 0xC0)
      ଫେରନ୍ତୁ ((b0 & 0x3F) << 8) | v[1];
    ଅନ୍ୟଥା ଯଦି (b0 < 0xE0)
      ଫେରନ୍ତୁ ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    ଅନ୍ୟଥା ଯଦି (b0 < 0xF0)
      ଫେରନ୍ତୁ ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  ସ୍ଥିର unsigned serialize_unsafe (unsigned ଅକ୍ଷର *buf, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned ଅକ୍ଷର *p = buf + len;
    ପାଇଁ (unsigned i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    ଯଦି (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    ଫେରନ୍ତୁ len;
  }

  ସ୍ଥିର ତାର୍କିକ serialize (hb_serialize_context_t *c, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned ଅକ୍ଷର *buf = c->allocate_size<unsigned ଅକ୍ଷର> (len, ମିଥ୍ୟା);
    ଯଦି (unlikely (!buf))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    serialize_unsafe (buf, v);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ସୁରକ୍ଷିତ:
  unsigned ଅକ୍ଷର v[5];

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା HBUINT16 UFWORD;

template <typename Type, unsigned fraction_bits>
ସଂରଚନା HBFixed : Type
{
  ସ୍ଥିର constexpr ଦଶମିକ mult = 1.f / (1 << fraction_bits);
  static_assert (Type::static_size * 8 > fraction_bits, "");

  operator signed () ଅଚଳ = delete;
  operator unsigned () ଅଚଳ = delete;
  explicit operator ଦଶମିକ () ଅଚଳ { ଫେରନ୍ତୁ to_float (); }
  typename Type::type to_int () ଅଚଳ { ଫେରନ୍ତୁ Type::v; }
  ଶୂନ୍ୟ set_int (typename Type::type i ) { Type::v = i; }
  ଦଶମିକ to_float (ଦଶମିକ offset = 0) ଅଚଳ  { ଫେରନ୍ତୁ ((int32_t) Type::v + offset) * mult; }
  ଶୂନ୍ୟ set_float (ଦଶମିକ f) { Type::v = roundf (f / mult); }
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (Type::static_size);
};

/* 16-bit signed fixed number with the low 14 bits of fraction (2.14). */
using F2DOT14 = HBFixed<HBINT16, 14>;
using F4DOT12 = HBFixed<HBINT16, 12>;
using F6DOT10 = HBFixed<HBINT16, 10>;

/* 32-bit signed fixed-point number (16.16). */
using F16DOT16 = HBFixed<HBINT32, 16>;

/* Date represented in number of seconds since 12:00 midnight, January 1,
 * 1904. The value is represented as a signed 64-bit integer. */
ସଂରଚନା LONGDATETIME
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }
  ସୁରକ୍ଷିତ:
  HBINT32 major;
  HBUINT32 minor;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
ସଂରଚନା Tag : HBUINT32
{
  Tag& operator = (hb_tag_t i) { HBUINT32::operator= (i); ଫେରନ୍ତୁ *ଏହି; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  operator ଅଚଳ ଅକ୍ଷର* () ଅଚଳ { ଫେରନ୍ତୁ reinterpret_cast<ଅଚଳ ଅକ୍ଷର *> (ଏହି); }
  operator ଅକ୍ଷର* ()             { ଫେରନ୍ତୁ reinterpret_cast<ଅକ୍ଷର *> (ଏହି); }
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
ସଂରଚନା HBGlyphID16 : HBUINT16
{
  HBGlyphID16& operator = (uint16_t i) { HBUINT16::operator= (i); ଫେରନ୍ତୁ *ଏହି; }
};
ସଂରଚନା HBGlyphID24 : HBUINT24
{
  HBGlyphID24& operator = (uint32_t i) { HBUINT24::operator= (i); ଫେରନ୍ତୁ *ଏହି; }
};

/* Script/language-system/feature index */
ସଂରଚନା Index : HBUINT16 {
  ସ୍ଥିର constexpr unsigned NOT_FOUND_INDEX = 0xFFFFu;
  Index& operator = (uint16_t i) { HBUINT16::operator= (i); ଫେରନ୍ତୁ *ଏହି; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, Index);

ପ୍ରକାରବ୍ୟାଖ୍ୟା Index NameID;

ସଂରଚନା VarIdx : HBUINT32 {
  ସ୍ଥିର constexpr unsigned NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_INDEX, "");
  VarIdx& operator = (uint32_t i) { HBUINT32::operator= (i); ଫେରନ୍ତୁ *ଏହି; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, VarIdx);

/* Offset, Null offset = 0 */
template <typename Type, ତାର୍କିକ has_null=ସତ୍ୟ>
ସଂରଚନା Offset : Type
{
  Offset& operator = (typename Type::type i) { Type::operator= (i); ଫେରନ୍ତୁ *ଏହି; }

  ପ୍ରକାରବ୍ୟାଖ୍ୟା Type type;

  ତାର୍କିକ is_null () ଅଚଳ { ଫେରନ୍ତୁ has_null && 0 == *ଏହି; }

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (ଆକାର (Type));
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା Offset<HBUINT16> Offset16;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Offset<HBUINT24> Offset24;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Offset<HBUINT32> Offset32;


/* CheckSum */
ସଂରଚନା CheckSum : HBUINT32
{
  CheckSum& operator = (uint32_t i) { HBUINT32::operator= (i); ଫେରନ୍ତୁ *ଏହି; }

  /* This is reference implementation from the spec. */
  ସ୍ଥିର uint32_t CalcTableChecksum (ଅଚଳ HBUINT32 *Table, uint32_t Length)
  {
    uint32_t Sum = 0L;
    ପ୍ରତିଜ୍ଞା (0 == (Length & 3));
    ଅଚଳ HBUINT32 *EndPtr = Table + Length / HBUINT32::static_size;

    ଯେତେବେଳେ (Table < EndPtr)
      Sum += *Table++;
    ଫେରନ୍ତୁ Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  ଶୂନ୍ୟ set_for_data (ଅଚଳ ଶୂନ୍ୟ *data, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length)
  { *ଏହି = CalcTableChecksum ((ଅଚଳ HBUINT32 *) data, length); }

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};


/*
 * Version Numbers
 */

template <typename FixedType=HBUINT16>
ସଂରଚନା FixedVersion
{
  uint32_t to_int () ଅଚଳ { ଫେରନ୍ତୁ (major << (ଆକାର (FixedType) * 8)) + minor; }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  FixedType major;
  FixedType minor;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (2 * ଆକାର (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

template <typename Type, ତାର୍କିକ has_null>
ସଂରଚନା _hb_has_null
{
  ସ୍ଥିର ଅଚଳ Type *get_null () { ଫେରନ୍ତୁ nullptr; }
  ସ୍ଥିର Type *get_crap ()       { ଫେରନ୍ତୁ nullptr; }
};
template <typename Type>
ସଂରଚନା _hb_has_null<Type, ସତ୍ୟ>
{
  ସ୍ଥିର ଅଚଳ Type *get_null () { ଫେରନ୍ତୁ &Null (Type); }
  ସ୍ଥିର       Type *get_crap () { ଫେରନ୍ତୁ &Crap (Type); }
};

template <typename Type, typename OffsetType, typename BaseType=ଶୂନ୍ୟ, ତାର୍କିକ has_null=ସତ୍ୟ>
ସଂରଚନା OffsetTo : Offset<OffsetType, has_null>
{
  using target_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == ମିଥ୍ୟା ||
		 (hb_has_null_size (Type) || !hb_has_min_size (Type)), "");

  HB_DELETE_COPY_ASSIGN (OffsetTo);
  OffsetTo () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

  OffsetTo& operator = (typename OffsetType::type i) { OffsetType::operator= (i); ଫେରନ୍ତୁ *ଏହି; }

  ଅଚଳ Type& operator () (ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    ଯଦି (unlikely (ଏହି->is_null ())) ଫେରନ୍ତୁ *_hb_has_null<Type, has_null>::get_null ();
    ଫେରନ୍ତୁ StructAtOffset<ଅଚଳ Type> (base, *ଏହି);
  }
  Type& operator () (ଶୂନ୍ୟ *base) ଅଚଳ
  {
    ଯଦି (unlikely (ଏହି->is_null ())) ଫେରନ୍ତୁ *_hb_has_null<Type, has_null>::get_crap ();
    ଫେରନ୍ତୁ StructAtOffset<Type> (base, *ଏହି);
  }

  template <typename Base,
	    hb_enable_if (hb_is_convertible (ଅଚଳ Base, ଅଚଳ BaseType *))>
  friend ଅଚଳ Type& operator + (ଅଚଳ Base &base, ଅଚଳ OffsetTo &offset) { ଫେରନ୍ତୁ offset ((ଅଚଳ ଶୂନ୍ୟ *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (ଅଚଳ Base, ଅଚଳ BaseType *))>
  friend ଅଚଳ Type& operator + (ଅଚଳ OffsetTo &offset, ଅଚଳ Base &base) { ଫେରନ୍ତୁ offset ((ଅଚଳ ଶୂନ୍ୟ *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (Base &&base, OffsetTo &offset) { ଫେରନ୍ତୁ offset ((ଶୂନ୍ୟ *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (OffsetTo &offset, Base &&base) { ଫେରନ୍ତୁ offset ((ଶୂନ୍ୟ *) base); }


  template <typename Base, typename ...Ts>
  ତାର୍କିକ serialize_subset (hb_subset_context_t *c, ଅଚଳ OffsetTo& src,
			 ଅଚଳ Base *src_base, Ts&&... ds)
  {
    *ଏହି = 0;
    ଯଦି (src.is_null ())
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    auto *s = c->serializer;

    s->push ();

    ତାର୍କିକ ret = c->dispatch (src_base+src, std::forward<Ts> (ds)...);

    ଯଦି (ret || !has_null)
      s->add_link (*ଏହି, s->କାଢ଼_pack ());
    ଅନ୍ୟଥା
      s->କାଢ଼_discard ();

    ଫେରନ୍ତୁ ret;
  }


  template <typename ...Ts>
  ତାର୍କିକ serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *ଏହି = 0;

    Type* obj = c->push<Type> ();
    ତାର୍କିକ ret = obj->serialize (c, std::forward<Ts> (ds)...);

    ଯଦି (ret)
      c->add_link (*ଏହି, c->କାଢ଼_pack ());
    ଅନ୍ୟଥା
      c->କାଢ଼_discard ();

    ଫେରନ୍ତୁ ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  template <typename ...Ts>
  ତାର୍କିକ serialize_copy (hb_serialize_context_t *c, ଅଚଳ OffsetTo& src,
		       ଅଚଳ ଶୂନ୍ୟ *src_base, unsigned dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *ଏହି = 0;
    ଯଦି (src.is_null ())
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    c->push ();

    ତାର୍କିକ ret = c->copy (src_base+src, std::forward<Ts> (ds)...);

    c->add_link (*ଏହି, c->କାଢ଼_pack (), whence, dst_bias);

    ଫେରନ୍ତୁ ret;
  }

  ତାର୍କିକ serialize_copy (hb_serialize_context_t *c, ଅଚଳ OffsetTo& src,
		       ଅଚଳ ଶୂନ୍ୟ *src_base, unsigned dst_bias = 0)
  { ଫେରନ୍ତୁ serialize_copy (c, src, src_base, dst_bias, hb_serialize_context_t::Head); }

  ତାର୍କିକ sanitize_shallow (hb_sanitize_context_t *c, ଅଚଳ BaseType *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!c->check_struct (ଏହି))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    ଯଦି (unlikely ((ଅଚଳ ଅକ୍ଷର *) base + (unsigned) *ଏହି < (ଅଚଳ ଅକ୍ଷର *) base)) return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  template <typename ...Ts>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ BaseType *base, Ts&&... ds) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (sanitize_shallow (c, base) &&
		  hb_barrier () &&
		  (ଏହି->is_null () ||
		   c->dispatch (StructAtOffset<Type> (base, *ଏହି), std::forward<Ts> (ds)...)));
  }

  DEFINE_SIZE_STATIC (ଆକାର (OffsetType));
};
/* Partial specializations. */
template <typename Type, typename BaseType=ଶୂନ୍ୟ, ତାର୍କିକ has_null=ସତ୍ୟ> using Offset16To = OffsetTo<Type, HBUINT16, BaseType, has_null>;
template <typename Type, typename BaseType=ଶୂନ୍ୟ, ତାର୍କିକ has_null=ସତ୍ୟ> using Offset24To = OffsetTo<Type, HBUINT24, BaseType, has_null>;
template <typename Type, typename BaseType=ଶୂନ୍ୟ, ତାର୍କିକ has_null=ସତ୍ୟ> using Offset32To = OffsetTo<Type, HBUINT32, BaseType, has_null>;

template <typename Type, typename OffsetType, typename BaseType=ଶୂନ୍ୟ> using NNOffsetTo = OffsetTo<Type, OffsetType, BaseType, ମିଥ୍ୟା>;
template <typename Type, typename BaseType=ଶୂନ୍ୟ> using NNOffset16To = Offset16To<Type, BaseType, ମିଥ୍ୟା>;
template <typename Type, typename BaseType=ଶୂନ୍ୟ> using NNOffset24To = Offset24To<Type, BaseType, ମିଥ୍ୟା>;
template <typename Type, typename BaseType=ଶୂନ୍ୟ> using NNOffset32To = Offset32To<Type, BaseType, ମିଥ୍ୟା>;


/*
 * Array Types
 */

template <typename Type>
ସଂରଚନା UnsizedArrayOf
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା Type item_t;
  ସ୍ଥିର constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (UnsizedArrayOf);

  ଅଚଳ Type& operator [] (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  {
    ଫେରନ୍ତୁ arrayZ[i];
  }
  Type& operator [] (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
  {
    ଫେରନ୍ତୁ arrayZ[i];
  }

  ସ୍ଥିର size_t get_size (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
  { ଫେରନ୍ତୁ hb_unsigned_mul_saturate (len, Type::static_size); }

  template <typename T> operator T * () { ଫେରନ୍ତୁ arrayZ; }
  template <typename T> operator ଅଚଳ T * () ଅଚଳ { ଫେରନ୍ତୁ arrayZ; }
  hb_array_t<Type> as_array (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
  { ଫେରନ୍ତୁ hb_array (arrayZ, len); }
  hb_array_t<ଅଚଳ Type> as_array (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) ଅଚଳ
  { ଫେରନ୍ତୁ hb_array (arrayZ, len); }

  template <typename T>
  Type &lsearch (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଅଚଳ T &x, Type &not_found = Crap (Type))
  { ଫେରନ୍ତୁ *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  ଅଚଳ Type &lsearch (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଅଚଳ T &x, ଅଚଳ Type &not_found = Null (Type)) ଅଚଳ
  { ଫେରନ୍ତୁ *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  ତାର୍କିକ lfind (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଅଚଳ T &x, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା to_store = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) -1) ଅଚଳ
  { ଫେରନ୍ତୁ as_array (len).lfind (x, i, not_found, to_store); }

  ଶୂନ୍ୟ qsort (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = 0, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) -1)
  { as_array (len).qsort (start, end); }

  ତାର୍କିକ serialize (hb_serialize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବସ୍ତୁଗୁଡ଼ିକ_len, ତାର୍କିକ clear = ସତ୍ୟ)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_size (ଏହି, get_size (ବସ୍ତୁଗୁଡ଼ିକ_len), clear))) return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c, Iterator ବସ୍ତୁଗୁଡ଼ିକ)
  {
    TRACE_SERIALIZE (ଏହି);
    unsigned ଗଣନା_2 = hb_len (ବସ୍ତୁଗୁଡ଼ିକ);
    ଯଦି (unlikely (!serialize (c, ଗଣନା_2, ମିଥ୍ୟା))) return_trace (ମିଥ୍ୟା);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++, ++ବସ୍ତୁଗୁଡ଼ିକ)
      arrayZ[i] = *ବସ୍ତୁଗୁଡ଼ିକ;
    return_trace (ସତ୍ୟ);
  }

  UnsizedArrayOf* copy (hb_serialize_context_t *c, unsigned ଗଣନା_2) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto *out = c->start_embed (ଏହି);
    ଯଦି (unlikely (!as_array (ଗଣନା_2).copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, Ts&&... ds) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!sanitize_shallow (c, ଗଣନା_2))) return_trace (ମିଥ୍ୟା);
    ଯଦି (!ଆକାର... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ସତ୍ୟ);
    hb_barrier ();
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize_shallow (hb_sanitize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_array (arrayZ, ଗଣନା_2));
  }

  ସର୍ବସାଧାରଣ:
  Type		arrayZ[HB_VAR_ARRAY];
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNBOUNDED (0);
};

/* Unsized array of offset's */
template <typename Type, typename OffsetType, typename BaseType=ଶୂନ୍ୟ, ତାର୍କିକ has_null=ସତ୍ୟ>
using UnsizedArray16OfOffsetTo = UnsizedArrayOf<OffsetTo<Type, OffsetType, BaseType, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
template <typename Type, typename OffsetType, typename BaseType=ଶୂନ୍ୟ, ତାର୍କିକ has_null=ସତ୍ୟ>
ସଂରଚନା UnsizedListOfOffset16To : UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
{
  ଅଚଳ Type& operator [] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i_) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i_;
    ଅଚଳ OffsetTo<Type, OffsetType, BaseType, has_null> *p = &ଏହି->arrayZ[i];
    ଯଦି (unlikely ((ଅଚଳ ଶୂନ୍ୟ *) p < (ଅଚଳ ଶୂନ୍ୟ *) ଏହି->arrayZ)) ଫେରନ୍ତୁ Null (Type); /* Overflowed. */
    hb_barrier ();
    ଫେରନ୍ତୁ ଏହି+*p;
  }
  Type& operator [] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i_)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i_;
    ଅଚଳ OffsetTo<Type, OffsetType, BaseType, has_null> *p = &ଏହି->arrayZ[i];
    ଯଦି (unlikely ((ଅଚଳ ଶୂନ୍ୟ *) p < (ଅଚଳ ଶୂନ୍ୟ *) ଏହି->arrayZ)) ଫେରନ୍ତୁ Crap (Type); /* Overflowed. */
    hb_barrier ();
    ଫେରନ୍ତୁ ଏହି+*p;
  }

  template <typename ...Ts>
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, Ts&&... ds) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace ((UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
		   ::sanitize (c, ଗଣନା_2, ଏହି, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type>
ସଂରଚନା SortedUnsizedArrayOf : UnsizedArrayOf<Type>
{
  hb_sorted_array_t<Type> as_array (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
  { ଫେରନ୍ତୁ hb_sorted_array (ଏହି->arrayZ, len); }
  hb_sorted_array_t<ଅଚଳ Type> as_array (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) ଅଚଳ
  { ଫେରନ୍ତୁ hb_sorted_array (ଏହି->arrayZ, len); }
  operator hb_sorted_array_t<Type> ()             { ଫେରନ୍ତୁ as_array (); }
  operator hb_sorted_array_t<ଅଚଳ Type> () ଅଚଳ { ଫେରନ୍ତୁ as_array (); }

  template <typename T>
  Type &bsearch (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଅଚଳ T &x, Type &not_found = Crap (Type))
  { ଫେରନ୍ତୁ *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  ଅଚଳ Type &bsearch (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଅଚଳ T &x, ଅଚଳ Type &not_found = Null (Type)) ଅଚଳ
  { ଫେରନ୍ତୁ *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  ତାର୍କିକ bfind (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଅଚଳ T &x, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା to_store = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) -1) ଅଚଳ
  { ଫେରନ୍ତୁ as_array (len).bfind (x, i, not_found, to_store); }
};


/* An array with a number of elements. */
template <typename Type, typename LenType>
ସଂରଚନା ArrayOf
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା Type item_t;
  ସ୍ଥିର constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOf);

  ଅଚଳ Type& operator [] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i_) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i_;
    ଯଦି (unlikely (i >= len)) ଫେରନ୍ତୁ Null (Type);
    hb_barrier ();
    ଫେରନ୍ତୁ arrayZ[i];
  }
  Type& operator [] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i_)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i_;
    ଯଦି (unlikely (i >= len)) ଫେରନ୍ତୁ Crap (Type);
    hb_barrier ();
    ଫେରନ୍ତୁ arrayZ[i];
  }

  size_t get_size () ଅଚଳ
  { ଫେରନ୍ତୁ hb_unsigned_mul_add_saturate (len, Type::static_size, len.static_size); }

  explicit operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ len; }

  ଶୂନ୍ୟ କାଢ଼ () { len--; }

  hb_array_t<      Type> as_array ()       { ଫେରନ୍ତୁ hb_array (arrayZ, len); }
  hb_array_t<ଅଚଳ Type> as_array () ଅଚଳ { ଫେରନ୍ତୁ hb_array (arrayZ, len); }

  /* Iterator. */
  ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_array_t<ଅଚଳ Type>   iter_t;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_array_t<      Type> writer_t;
    iter_t   iter () ଅଚଳ { ଫେରନ୍ତୁ as_array (); }
  writer_t writer ()       { ଫେରନ୍ତୁ as_array (); }
  operator   iter_t () ଅଚଳ { ଫେରନ୍ତୁ   iter (); }
  operator writer_t ()       { ଫେରନ୍ତୁ writer (); }

  /* Faster range-based for loop. */
  ଅଚଳ Type *begin () ଅଚଳ { ଫେରନ୍ତୁ arrayZ; }
  ଅଚଳ Type *end () ଅଚଳ { ଫେରନ୍ତୁ arrayZ + len; }

  template <typename T>
  Type &lsearch (ଅଚଳ T &x, Type &not_found = Crap (Type))
  { ଫେରନ୍ତୁ *as_array ().lsearch (x, &not_found); }
  template <typename T>
  ଅଚଳ Type &lsearch (ଅଚଳ T &x, ଅଚଳ Type &not_found = Null (Type)) ଅଚଳ
  { ଫେରନ୍ତୁ *as_array ().lsearch (x, &not_found); }
  template <typename T>
  ତାର୍କିକ lfind (ଅଚଳ T &x, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା to_store = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) -1) ଅଚଳ
  { ଫେରନ୍ତୁ as_array ().lfind (x, i, not_found, to_store); }

  ଶୂନ୍ୟ qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD ତାର୍କିକ serialize (hb_serialize_context_t *c, unsigned ବସ୍ତୁଗୁଡ଼ିକ_len, ତାର୍କିକ clear = ସତ୍ୟ)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);
    c->check_assign (len, ବସ୍ତୁଗୁଡ଼ିକ_len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    ଯଦି (unlikely (!c->extend_size (ଏହି, get_size (), clear))) return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD ତାର୍କିକ serialize (hb_serialize_context_t *c, Iterator ବସ୍ତୁଗୁଡ଼ିକ)
  {
    TRACE_SERIALIZE (ଏହି);
    unsigned ଗଣନା_2 = hb_len (ବସ୍ତୁଗୁଡ଼ିକ);
    ଯଦି (unlikely (!serialize (c, ଗଣନା_2, ମିଥ୍ୟା))) return_trace (ମିଥ୍ୟା);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++, ++ବସ୍ତୁଗୁଡ଼ିକ)
      arrayZ[i] = *ବସ୍ତୁଗୁଡ଼ିକ;
    return_trace (ସତ୍ୟ);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (ଏହି);
    len++;
    ଯଦି (unlikely (!len || !c->extend (ଏହି)))
    {
      len--;
      return_trace (nullptr);
    }
    return_trace (&arrayZ[len - 1]);
  }

  ArrayOf* copy (hb_serialize_context_t *c) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto *out = c->start_embed (ଏହି);
    ଯଦି (unlikely (!c->extend_min (out))) return_trace (nullptr);
    c->check_assign (out->len, len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    ଯଦି (unlikely (!as_array ().copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!sanitize_shallow (c))) return_trace (ମିଥ୍ୟା);
    ଯଦି (!ଆକାର... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ସତ୍ୟ);
    hb_barrier ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize_shallow (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (len.sanitize (c) &&
		  hb_barrier () &&
		  c->check_array_sized (arrayZ, len, ଆକାର (LenType)));
  }

  ସର୍ବସାଧାରଣ:
  LenType	len;
  Type		arrayZ[HB_VAR_ARRAY];
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (ଆକାର (LenType), arrayZ);
};
template <typename Type> using Array16Of = ArrayOf<Type, HBUINT16>;
template <typename Type> using Array24Of = ArrayOf<Type, HBUINT24>;
template <typename Type> using Array32Of = ArrayOf<Type, HBUINT32>;
using PString = ArrayOf<HBUINT8, HBUINT8>;

/* Array of Offset's */
template <typename Type> using Array8OfOffset24To = ArrayOf<OffsetTo<Type, HBUINT24>, HBUINT8>;
template <typename Type> using Array16OfOffset16To = ArrayOf<OffsetTo<Type, HBUINT16>, HBUINT16>;
template <typename Type> using Array16OfOffset32To = ArrayOf<OffsetTo<Type, HBUINT32>, HBUINT16>;
template <typename Type> using Array32OfOffset32To = ArrayOf<OffsetTo<Type, HBUINT32>, HBUINT32>;

/* Array of offsets relative to the beginning of the array itself. */
template <typename Type, typename OffsetType>
ସଂରଚନା List16OfOffsetTo : ArrayOf<OffsetTo<Type, OffsetType>, HBUINT16>
{
  ଅଚଳ Type& operator [] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i_) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i_;
    ଯଦି (unlikely (i >= ଏହି->len)) ଫେରନ୍ତୁ Null (Type);
    hb_barrier ();
    ଫେରନ୍ତୁ ଏହି+ଏହି->arrayZ[i];
  }
  ଅଚଳ Type& operator [] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i_)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i_;
    ଯଦି (unlikely (i >= ଏହି->len)) ଫେରନ୍ତୁ Crap (Type);
    hb_barrier ();
    ଫେରନ୍ତୁ ଏହି+ଏହି->arrayZ[i];
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ସଂରଚନା List16OfOffsetTo *out = c->serializer->embed (*ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = ଏହି->len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      out->arrayZ[i].serialize_subset (c, ଏହି->arrayZ[i], ଏହି, out);
    return_trace (ସତ୍ୟ);
  }

  template <typename ...Ts>
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace ((Array16Of<OffsetTo<Type, OffsetType>>::sanitize (c, ଏହି, std::forward<Ts> (ds)...)));
  }
};

template <typename Type>
using List16OfOffset16To = List16OfOffsetTo<Type, HBUINT16>;

/* An array starting at second element. */
template <typename Type, typename LenType>
ସଂରଚନା HeadlessArrayOf
{
  ସ୍ଥିର constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (HeadlessArrayOf);

  ଅଚଳ Type& operator [] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i_) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i_;
    ଯଦି (unlikely (i >= lenP1 || !i)) ଫେରନ୍ତୁ Null (Type);
    hb_barrier ();
    ଫେରନ୍ତୁ arrayZ[i-1];
  }
  Type& operator [] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i_)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i_;
    ଯଦି (unlikely (i >= lenP1 || !i)) ଫେରନ୍ତୁ Crap (Type);
    hb_barrier ();
    ଫେରନ୍ତୁ arrayZ[i-1];
  }
  size_t get_size () ଅଚଳ
  { ଫେରନ୍ତୁ hb_unsigned_mul_add_saturate (get_length (), Type::static_size, lenP1.static_size); }

  unsigned get_length () ଅଚଳ { ଫେରନ୍ତୁ lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { ଫେରନ୍ତୁ hb_array (arrayZ, get_length ()); }
  hb_array_t<ଅଚଳ Type> as_array () ଅଚଳ { ଫେରନ୍ତୁ hb_array (arrayZ, get_length ()); }

  /* Iterator. */
  ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_array_t<ଅଚଳ Type>   iter_t;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_array_t<      Type> writer_t;
    iter_t   iter () ଅଚଳ { ଫେରନ୍ତୁ as_array (); }
  writer_t writer ()       { ଫେରନ୍ତୁ as_array (); }
  operator   iter_t () ଅଚଳ { ଫେରନ୍ତୁ   iter (); }
  operator writer_t ()       { ଫେରନ୍ତୁ writer (); }

  /* Faster range-based for loop. */
  ଅଚଳ Type *begin () ଅଚଳ { ଫେରନ୍ତୁ arrayZ; }
  ଅଚଳ Type *end () ଅଚଳ { ଫେରନ୍ତୁ arrayZ + get_length (); }

  HB_NODISCARD ତାର୍କିକ serialize (hb_serialize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବସ୍ତୁଗୁଡ଼ିକ_len, ତାର୍କିକ clear = ସତ୍ୟ)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);
    c->check_assign (lenP1, ବସ୍ତୁଗୁଡ଼ିକ_len + 1, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    ଯଦି (unlikely (!c->extend_size (ଏହି, get_size (), clear))) return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD ତାର୍କିକ serialize (hb_serialize_context_t *c, Iterator ବସ୍ତୁଗୁଡ଼ିକ)
  {
    TRACE_SERIALIZE (ଏହି);
    unsigned ଗଣନା_2 = hb_len (ବସ୍ତୁଗୁଡ଼ିକ);
    ଯଦି (unlikely (!serialize (c, ଗଣନା_2, ମିଥ୍ୟା))) return_trace (ମିଥ୍ୟା);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++, ++ବସ୍ତୁଗୁଡ଼ିକ)
      arrayZ[i] = *ବସ୍ତୁଗୁଡ଼ିକ;
    return_trace (ସତ୍ୟ);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!sanitize_shallow (c))) return_trace (ମିଥ୍ୟା);
    ଯଦି (!ଆକାର... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ସତ୍ୟ);
    hb_barrier ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = get_length ();
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  ବ୍ୟକ୍ତିଗତ:
  ତାର୍କିକ sanitize_shallow (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (lenP1.sanitize (c) &&
		  hb_barrier () &&
		  (!lenP1 || c->check_array_sized (arrayZ, lenP1 - 1, ଆକାର (LenType))));
  }

  ସର୍ବସାଧାରଣ:
  LenType	lenP1;
  Type		arrayZ[HB_VAR_ARRAY];
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (ଆକାର (LenType), arrayZ);
};
template <typename Type> using HeadlessArray16Of = HeadlessArrayOf<Type, HBUINT16>;

/* An array storing length-1. */
template <typename Type, typename LenType=HBUINT16>
ସଂରଚନା ArrayOfM1
{
  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOfM1);

  ଅଚଳ Type& operator [] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i_) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i_;
    ଯଦି (unlikely (i > lenM1)) ଫେରନ୍ତୁ Null (Type);
    hb_barrier ();
    ଫେରନ୍ତୁ arrayZ[i];
  }
  Type& operator [] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i_)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i_;
    ଯଦି (unlikely (i > lenM1)) ଫେରନ୍ତୁ Crap (Type);
    hb_barrier ();
    ଫେରନ୍ତୁ arrayZ[i];
  }
  size_t get_size () ଅଚଳ
  { ଫେରନ୍ତୁ hb_unsigned_mul_add_saturate (lenM1 + 1, Type::static_size, lenM1.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!sanitize_shallow (c))) return_trace (ମିଥ୍ୟା);
    ଯଦି (!ଆକାର... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ସତ୍ୟ);
    hb_barrier ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = lenM1 + 1;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  ବ୍ୟକ୍ତିଗତ:
  ତାର୍କିକ sanitize_shallow (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (lenM1.sanitize (c) &&
		  hb_barrier () &&
		  (c->check_array_sized (arrayZ, lenM1 + 1, ଆକାର (LenType))));
  }

  ସର୍ବସାଧାରଣ:
  LenType	lenM1;
  Type		arrayZ[HB_VAR_ARRAY];
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (ଆକାର (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type, typename LenType>
ସଂରଚନା SortedArrayOf : ArrayOf<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { ଫେରନ୍ତୁ hb_sorted_array (ଏହି->arrayZ, ଏହି->len); }
  hb_sorted_array_t<ଅଚଳ Type> as_array () ଅଚଳ { ଫେରନ୍ତୁ hb_sorted_array (ଏହି->arrayZ, ଏହି->len); }

  /* Iterator. */
  ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_sorted_array_t<ଅଚଳ Type>   iter_t;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () ଅଚଳ { ଫେରନ୍ତୁ as_array (); }
  writer_t writer ()       { ଫେରନ୍ତୁ as_array (); }
  operator   iter_t () ଅଚଳ { ଫେରନ୍ତୁ   iter (); }
  operator writer_t ()       { ଫେରନ୍ତୁ writer (); }

  /* Faster range-based for loop. */
  ଅଚଳ Type *begin () ଅଚଳ { ଫେରନ୍ତୁ ଏହି->arrayZ; }
  ଅଚଳ Type *end () ଅଚଳ { ଫେରନ୍ତୁ ଏହି->arrayZ + ଏହି->len; }

  ତାର୍କିକ serialize (hb_serialize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବସ୍ତୁଗୁଡ଼ିକ_len)
  {
    TRACE_SERIALIZE (ଏହି);
    ତାର୍କିକ ret = ArrayOf<Type, LenType>::serialize (c, ବସ୍ତୁଗୁଡ଼ିକ_len);
    return_trace (ret);
  }
  template <typename Iterator,
	    hb_requires (hb_is_sorted_source_of (Iterator, Type))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c, Iterator ବସ୍ତୁଗୁଡ଼ିକ)
  {
    TRACE_SERIALIZE (ଏହି);
    ତାର୍କିକ ret = ArrayOf<Type, LenType>::serialize (c, ବସ୍ତୁଗୁଡ଼ିକ);
    return_trace (ret);
  }

  SortedArrayOf* copy (hb_serialize_context_t *c) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    SortedArrayOf* out = reinterpret_cast<SortedArrayOf *> (ArrayOf<Type, LenType>::copy (c));
    return_trace (out);
  }

  template <typename T>
  Type &bsearch (ଅଚଳ T &x, Type &not_found = Crap (Type))
  { ଫେରନ୍ତୁ *as_array ().bsearch (x, &not_found); }
  template <typename T>
  ଅଚଳ Type &bsearch (ଅଚଳ T &x, ଅଚଳ Type &not_found = Null (Type)) ଅଚଳ
  { ଫେରନ୍ତୁ *as_array ().bsearch (x, &not_found); }
  template <typename T>
  ତାର୍କିକ bfind (ଅଚଳ T &x, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା to_store = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) -1) ଅଚଳ
  { ଫେରନ୍ତୁ as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type> using SortedArray16Of = SortedArrayOf<Type, HBUINT16>;
template <typename Type> using SortedArray24Of = SortedArrayOf<Type, HBUINT24>;
template <typename Type> using SortedArray32Of = SortedArrayOf<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

template <typename LenType=HBUINT16>
ସଂରଚନା Bin_ସନ୍ଧାନ_Header
{
  operator uint32_t () ଅଚଳ { ଫେରନ୍ତୁ len; }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  Bin_ସନ୍ଧାନ_Header& operator = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା v)
  {
    len = v;
    ପ୍ରତିଜ୍ଞା (len == v);
    entrySelector = hb_max (1u, hb_bit_storage (v)) - 1;
    ସନ୍ଧାନ_Range = 16 * (1u << entrySelector);
    rangeShift = v * 16 > ସନ୍ଧାନ_Range
		 ? 16 * v - ସନ୍ଧାନ_Range
		 : 0;
    ଫେରନ୍ତୁ *ଏହି;
  }

  ସୁରକ୍ଷିତ:
  LenType	len;
  LenType	ସନ୍ଧାନ_Range;
  LenType	entrySelector;
  LenType	rangeShift;

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

template <typename Type, typename LenType=HBUINT16>
using Bin_ସନ୍ଧାନ_Array_Of = SortedArrayOf<Type, Bin_ସନ୍ଧାନ_Header<LenType>>;


ସଂରଚନା Var_Sized_Bin_ସନ୍ଧାନ_Header
{

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  HBUINT16	unitSize;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	ସନ୍ଧାନ_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	entrySelector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	rangeShift;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (10);
};

template <typename Type>
ସଂରଚନା Var_Sized_Bin_ସନ୍ଧାନ_Array_Of
{
  ସ୍ଥିର constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (Var_Sized_Bin_ସନ୍ଧାନ_Array_Of);

  ତାର୍କିକ last_is_terminator () ଅଚଳ
  {
    ଯଦି (unlikely (!header.nUnits)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    ଅଚଳ HBUINT16 *ଶବ୍ଦଗୁଡ଼ିକ = &StructAtOffset<HBUINT16> (&bytesZ, (header.nUnits - 1) * header.unitSize);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = Type::Termination_Word_ଗଣନା;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (ଶବ୍ଦଗୁଡ଼ିକ[i] != 0xFFFFu)
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଅଚଳ Type& operator [] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i_) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i_;
    ଯଦି (unlikely (i >= get_length ())) ଫେରନ୍ତୁ Null (Type);
    hb_barrier ();
    ଫେରନ୍ତୁ StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  Type& operator [] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i_)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i_;
    ଯଦି (unlikely (i >= get_length ())) ଫେରନ୍ତୁ Crap (Type);
    hb_barrier ();
    ଫେରନ୍ତୁ StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_length () ଅଚଳ
  { ଫେରନ୍ତୁ header.nUnits - last_is_terminator (); }
  size_t get_size () ଅଚଳ
  { ଫେରନ୍ତୁ hb_unsigned_mul_add_saturate (header.nUnits, header.unitSize, header.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!sanitize_shallow (c))) return_trace (ମିଥ୍ୟା);
    ଯଦି (!ଆକାର... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ସତ୍ୟ);
    hb_barrier ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = get_length ();
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (unlikely (!(*ଏହି)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  template <typename T>
  ଅଚଳ Type *bsearch (ଅଚଳ T &key) ଅଚଳ
  {
    unsigned pos;
    ଫେରନ୍ତୁ hb_bsearch_impl (&pos,
			    key,
			    (ଅଚଳ ଶୂନ୍ୟ *) bytesZ,
			    get_length (),
			    header.unitSize,
			    _hb_cmp_method<T, Type>)
	   ? (ଅଚଳ Type *) (((ଅଚଳ ଅକ୍ଷର *) &bytesZ) + (pos * header.unitSize))
	   : nullptr;
  }

  ବ୍ୟକ୍ତିଗତ:
  ତାର୍କିକ sanitize_shallow (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (header.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_size <= header.unitSize &&
		  c->check_range (bytesZ.arrayZ,
				  header.nUnits,
				  header.unitSize));
  }

  ସୁରକ୍ଷିତ:
  Var_Sized_Bin_ସନ୍ଧାନ_Header	header;
  UnsizedArrayOf<HBUINT8>	bytesZ;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (10, bytesZ);
};


/* CFF INDEX */

template <typename ଗଣନା_4>
ସଂରଚନା CFFIndex
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset_array_size () ଅଚଳ
  { ଫେରନ୍ତୁ offSize * (ଗଣନା_2 + 1); }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ Iterable &iterable,
		  ଅଚଳ unsigned *p_data_size = nullptr,
                  unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (ଏହି);
    unsigned data_size;
    ଯଦି (p_data_size)
      data_size = *p_data_size;
    ଅନ୍ୟଥା
      total_size (iterable, &data_size);

    auto it = hb_iter (iterable);
    ଯଦି (unlikely (!serialize_header (c, +it, data_size, min_off_size))) return_trace (ମିଥ୍ୟା);
    unsigned ଅକ୍ଷର *ret = c->allocate_size<unsigned ଅକ୍ଷର> (data_size, ମିଥ୍ୟା);
    ଯଦି (unlikely (!ret)) return_trace (ମିଥ୍ୟା);
    unsigned remaining = data_size;
    ପାଇଁ (ଅଚଳ auto &_ : +it)
    {
      unsigned len = _.length;

      ଯଦି (!len)
	ଜାରି;

      ଯଦି (unlikely (len > remaining)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (ମିଥ୍ୟା, HB_SERIALIZE_ERROR_INT_OVERFLOW));
      }

      remaining -= len;

      ଯଦି (len <= 1)
      {
	*ret++ = *_.arrayZ;
	ଜାରି;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (ସତ୍ୟ);
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  ତାର୍କିକ serialize_header (hb_serialize_context_t *c,
			 Iterator it,
			 unsigned data_size,
                         unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (ଏହି);

    unsigned off_size = (hb_bit_storage (data_size + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    /* serialize CFFIndex header */
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);
    ଏହି->ଗଣନା_2 = hb_len (it);
    ଯଦି (!ଏହି->ଗଣନା_2) return_trace (ସତ୍ୟ);
    ଯଦି (unlikely (!c->extend (ଏହି->offSize))) return_trace (ମିଥ୍ୟା);
    ଏହି->offSize = off_size;
    ଯଦି (unlikely (!c->allocate_size<HBUINT8> (off_size * (ଏହି->ଗଣନା_2 + 1), ମିଥ୍ୟା)))
      return_trace (ମିଥ୍ୟା);

    /* serialize indices */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = 1;
    ଯଦି (HB_OPTIMIZE_SIZE_VAL)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
      ପାଇଁ (ଅଚଳ auto &_ : +it)
      {
	set_offset_at (i++, offset);
	offset += hb_len_of (_);
      }
      set_offset_at (i, offset);
    }
    ଅନ୍ୟଥା
      ବାଛ (off_size)
      {
	ସ୍ଥିତି 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  ପାଇଁ (ଅଚଳ auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	ଭାଙ୍ଗ;
	ସ୍ଥିତି 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  ପାଇଁ (ଅଚଳ auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	ଭାଙ୍ଗ;
	ସ୍ଥିତି 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  ପାଇଁ (ଅଚଳ auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	ଭାଙ୍ଗ;
	ସ୍ଥିତି 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  ପାଇଁ (ଅଚଳ auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	ଭାଙ୍ଗ;
      }

    ପ୍ରତିଜ୍ଞା (offset == data_size + 1);
    return_trace (ସତ୍ୟ);
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  ସ୍ଥିର unsigned total_size (ଅଚଳ Iterable &iterable, unsigned *data_size = nullptr, unsigned min_off_size = 0)
  {
    auto it = + hb_iter (iterable);
    ଯଦି (!it)
    {
      ଯଦି (data_size) *data_size = 0;
      ଫେରନ୍ତୁ min_size;
    }

    unsigned total = 0;
    ପାଇଁ (ଅଚଳ auto &_ : +it)
      total += hb_len_of (_);

    ଯଦି (data_size) *data_size = total;

    unsigned off_size = (hb_bit_storage (total + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    ଫେରନ୍ତୁ min_size + HBUINT8::static_size + (hb_len (it) + 1) * off_size + total;
  }

  ଶୂନ୍ୟ set_offset_at (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset)
  {
    ପ୍ରତିଜ୍ଞା (index <= ଗଣନା_2);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = offSize;
    ଅଚଳ HBUINT8 *p = offsets;
    ବାଛ (size)
    {
      ସ୍ଥିତି 1: ((HBUINT8  *) p)[index] = offset; ଭାଙ୍ଗ;
      ସ୍ଥିତି 2: ((HBUINT16 *) p)[index] = offset; ଭାଙ୍ଗ;
      ସ୍ଥିତି 3: ((HBUINT24 *) p)[index] = offset; ଭାଙ୍ଗ;
      ସ୍ଥିତି 4: ((HBUINT32 *) p)[index] = offset; ଭାଙ୍ଗ;
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ;
    }
  }

  ବ୍ୟକ୍ତିଗତ:
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset_at (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index) ଅଚଳ
  {
    ପ୍ରତିଜ୍ଞା (index <= ଗଣନା_2);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = offSize;
    ଅଚଳ HBUINT8 *p = offsets;
    ବାଛ (size)
    {
      ସ୍ଥିତି 1: ଫେରନ୍ତୁ ((HBUINT8  *) p)[index];
      ସ୍ଥିତି 2: ଫେରନ୍ତୁ ((HBUINT16 *) p)[index];
      ସ୍ଥିତି 3: ଫେରନ୍ତୁ ((HBUINT24 *) p)[index];
      ସ୍ଥିତି 4: ଫେରନ୍ତୁ ((HBUINT32 *) p)[index];
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ 0;
    }
  }

  ଅଚଳ unsigned ଅକ୍ଷର *data_base () ଅଚଳ
  { ଫେରନ୍ତୁ (ଅଚଳ unsigned ଅକ୍ଷର *) ଏହି + min_size + offSize.static_size - 1 + offset_array_size (); }
  ସର୍ବସାଧାରଣ:

  hb_ubytes_t operator [] (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index) ଅଚଳ
  {
    ଯଦି (unlikely (index >= ଗଣନା_2)) ଫେରନ୍ତୁ hb_ubytes_t ();
    hb_barrier ();
    unsigned offset0 = offset_at (index);
    unsigned offset1 = offset_at (index + 1);
    ଯଦି (unlikely (offset1 < offset0 || offset1 > offset_at (ଗଣନା_2)))
      ଫେରନ୍ତୁ hb_ubytes_t ();
    ଫେରନ୍ତୁ hb_ubytes_t (data_base () + offset0, offset1 - offset0);
  }

  size_t get_size () ଅଚଳ
  {
    ଯଦି (ଗଣନା_2)
      ଫେରନ୍ତୁ hb_unsigned_add_saturate (min_size, offSize.static_size,
				       offset_array_size (), offset_at (ଗଣନା_2) - 1);
    ଫେରନ୍ତୁ min_size;  /* empty CFFIndex contains count only */
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  hb_barrier () &&
			  (ଗଣନା_2 == 0 || /* empty INDEX */
			   (ଗଣନା_2 < ଗଣନା_2 + 1u &&
			    c->check_struct (&offSize) && offSize >= 1 && offSize <= 4 &&
			    c->check_array (offsets, offSize, ଗଣନା_2 + 1u) &&
			    c->check_range (data_base (), offset_at (ଗଣନା_2))))));
  }

  ସର୍ବସାଧାରଣ:
  ଗଣନା_4		ଗଣନା_2;		/* Number of object data. Note there are (count+1) offsets */
  ବ୍ୟକ୍ତିଗତ:
  HBUINT8	offSize;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (ଗଣନା_4::static_size);
};
ପ୍ରକାରବ୍ୟାଖ୍ୟା CFFIndex<HBUINT16> CFF1Index;
ପ୍ରକାରବ୍ୟାଖ୍ୟା CFFIndex<HBUINT32> CFF2Index;


/* TupleValues */
ସଂରଚନା Tuple_ମୂଲ୍ୟଗୁଡ଼ିକ
{
  ଗଣନା packed_ମୂଲ୍ୟ_flag_t
  {
    ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_ZEROS     = 0x80,
    ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_BYTES     = 0x00,
    ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_ଶବ୍ଦଗୁଡ଼ିକ     = 0x40,
    ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_LONGS     = 0xC0,
    ମୂଲ୍ୟଗୁଡ଼ିକ_SIZE_MASK     = 0xC0,
    ମୂଲ୍ୟ_RUN_ଗଣନା_MASK = 0x3F
  };

  ସ୍ଥିର unsigned compile_unsafe (hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> values, /* IN */
				  unsigned ଅକ୍ଷର *encoded_bytes /* OUT */)
  {
    unsigned ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_3 = values.length;
    unsigned encoded_len = 0;
    unsigned i = 0;
    ଯେତେବେଳେ (i < ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_3)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା val = values.arrayZ[i];
      ଯଦି (val == 0)
        encoded_len += encode_ମୂଲ୍ୟ_run_as_zeroes (i, encoded_bytes + encoded_len, values);
      ଅନ୍ୟଥା ଯଦି ((int8_t) val == val)
        encoded_len += encode_ମୂଲ୍ୟ_run_as_bytes (i, encoded_bytes + encoded_len, values);
      ଅନ୍ୟଥା ଯଦି ((int16_t) val == val)
        encoded_len += encode_ମୂଲ୍ୟ_run_as_ଶବ୍ଦଗୁଡ଼ିକ (i, encoded_bytes + encoded_len, values);
      ଅନ୍ୟଥା
        encoded_len += encode_ମୂଲ୍ୟ_run_as_longs (i, encoded_bytes + encoded_len, values);
    }
    ଫେରନ୍ତୁ encoded_len;
  }

  ସ୍ଥିର unsigned encode_ମୂଲ୍ୟ_run_as_zeroes (unsigned& i,
					      unsigned ଅକ୍ଷର *it,
					      hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> values)
  {
    unsigned ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_3 = values.length;
    unsigned run_length = 0;
    unsigned encoded_len = 0;
    ଯେତେବେଳେ (i < ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_3 && values.arrayZ[i] == 0)
    {
      i++;
      run_length++;
    }

    ଯେତେବେଳେ (run_length >= 64)
    {
      *it++ = ଅକ୍ଷର (ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_ZEROS | 63);
      run_length -= 64;
      encoded_len++;
    }

    ଯଦି (run_length)
    {
      *it++ = ଅକ୍ଷର (ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_ZEROS | (run_length - 1));
      encoded_len++;
    }
    ଫେରନ୍ତୁ encoded_len;
  }

  ସ୍ଥିର unsigned encode_ମୂଲ୍ୟ_run_as_bytes (unsigned &i,
					     unsigned ଅକ୍ଷର *it,
					     hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> values)
  {
    unsigned start = i;
    unsigned ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_3 = values.length;
    ଯେତେବେଳେ (i < ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_3)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା val = values.arrayZ[i];
      ଯଦି ((int8_t) val != val)
        ଭାଙ୍ଗ;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      ଯଦି (val == 0 && i + 1 < ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_3 && values.arrayZ[i+1] == 0)
        ଭାଙ୍ଗ;

      i++;
    }
    unsigned run_length = i - start;

    unsigned encoded_len = 0;

    ଯେତେବେଳେ (run_length >= 64)
    {
      *it++ = (ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_BYTES | 63);
      encoded_len++;

      ପାଇଁ (unsigned j = 0; j < 64; j++)
	it[j] = static_cast<ଅକ୍ଷର> (values.arrayZ[start + j]);
      it += 64;
      encoded_len += 64;

      start += 64;
      run_length -= 64;
    }

    ଯଦି (run_length)
    {
      *it++ = (ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_BYTES | (run_length - 1));
      encoded_len++;

      ପାଇଁ (unsigned j = 0; j < run_length; j++)
        it[j] = static_cast<ଅକ୍ଷର> (values.arrayZ[start + j]);
      encoded_len += run_length;
    }

    ଫେରନ୍ତୁ encoded_len;
  }

  ସ୍ଥିର unsigned encode_ମୂଲ୍ୟ_run_as_ଶବ୍ଦଗୁଡ଼ିକ (unsigned &i,
					     unsigned ଅକ୍ଷର *it,
					     hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> values)
  {
    unsigned start = i;
    unsigned ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_3 = values.length;
    ଯେତେବେଳେ (i < ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_3)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା val = values.arrayZ[i];

      ଯଦି ((int16_t) val != val)
        ଭାଙ୍ଗ;

      /* start a new run for a single zero value. */
      ଯଦି (val == 0) ଭାଙ୍ଗ;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      ଯଦି ((int8_t) val == val &&
          i + 1 < ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        ଭାଙ୍ଗ;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    ଯେତେବେଳେ (run_length >= 64)
    {
      *it++ = (ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_ଶବ୍ଦଗୁଡ଼ିକ | 63);
      encoded_len++;

      ପାଇଁ (unsigned j = 0; j < 64; j++)
      {
        int16_t ମୂଲ୍ୟ_val = values.arrayZ[start + j];
        *it++ = static_cast<ଅକ୍ଷର> (ମୂଲ୍ୟ_val >> 8);
        *it++ = static_cast<ଅକ୍ଷର> (ମୂଲ୍ୟ_val & 0xFF);

        encoded_len += 2;
      }

      start += 64;
      run_length -= 64;
    }

    ଯଦି (run_length)
    {
      *it++ = (ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_ଶବ୍ଦଗୁଡ଼ିକ | (run_length - 1));
      encoded_len++;
      ଯେତେବେଳେ (start < i)
      {
        int16_t ମୂଲ୍ୟ_val = values.arrayZ[start++];
        *it++ = static_cast<ଅକ୍ଷର> (ମୂଲ୍ୟ_val >> 8);
        *it++ = static_cast<ଅକ୍ଷର> (ମୂଲ୍ୟ_val & 0xFF);

        encoded_len += 2;
      }
    }
    ଫେରନ୍ତୁ encoded_len;
  }

  ସ୍ଥିର unsigned encode_ମୂଲ୍ୟ_run_as_longs (unsigned &i,
					     unsigned ଅକ୍ଷର *it,
					     hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> values)
  {
    unsigned start = i;
    unsigned ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_3 = values.length;
    ଯେତେବେଳେ (i < ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_3)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା val = values.arrayZ[i];

      ଯଦି ((int16_t) val == val)
        ଭାଙ୍ଗ;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    ଯେତେବେଳେ (run_length >= 64)
    {
      *it++ = (ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_LONGS | 63);
      encoded_len++;

      ପାଇଁ (unsigned j = 0; j < 64; j++)
      {
        int32_t ମୂଲ୍ୟ_val = values.arrayZ[start + j];
        *it++ = static_cast<ଅକ୍ଷର> (ମୂଲ୍ୟ_val >> 24);
        *it++ = static_cast<ଅକ୍ଷର> (ମୂଲ୍ୟ_val >> 16);
        *it++ = static_cast<ଅକ୍ଷର> (ମୂଲ୍ୟ_val >> 8);
        *it++ = static_cast<ଅକ୍ଷର> (ମୂଲ୍ୟ_val & 0xFF);

        encoded_len += 4;
      }

      start += 64;
      run_length -= 64;
    }

    ଯଦି (run_length)
    {
      *it++ = (ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_LONGS | (run_length - 1));
      encoded_len++;
      ଯେତେବେଳେ (start < i)
      {
        int32_t ମୂଲ୍ୟ_val = values.arrayZ[start++];
        *it++ = static_cast<ଅକ୍ଷର> (ମୂଲ୍ୟ_val >> 24);
        *it++ = static_cast<ଅକ୍ଷର> (ମୂଲ୍ୟ_val >> 16);
        *it++ = static_cast<ଅକ୍ଷର> (ମୂଲ୍ୟ_val >> 8);
        *it++ = static_cast<ଅକ୍ଷର> (ମୂଲ୍ୟ_val & 0xFF);

        encoded_len += 4;
      }
    }
    ଫେରନ୍ତୁ encoded_len;
  }

  template <typename T>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ସ୍ଥିର ତାର୍କିକ decompile (ଅଚଳ HBUINT8 *&p /* IN/OUT */,
			 hb_vector_t<T> &values /* IN/OUT */,
			 ଅଚଳ HBUINT8 *end,
			 ତାର୍କିକ consume_all = ମିଥ୍ୟା,
			 unsigned start = 0)
  {
    unsigned i = 0;
    unsigned ଗଣନା_2 = consume_all ? UINT_MAX : values.length;
    ଯଦି (consume_all)
      values.alloc ((end - p) / 2);
    ଯେତେବେଳେ (i < ଗଣନା_2)
    {
      ଯଦି (unlikely (p + 1 > end)) ଫେରନ୍ତୁ consume_all;
      unsigned control = *p++;
      unsigned run_ଗଣନା = (control & ମୂଲ୍ୟ_RUN_ଗଣନା_MASK) + 1;
      ଯଦି (consume_all)
      {
        ଯଦି (unlikely (!values.resize_dirty  (values.length + run_ଗଣନା)))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
      unsigned stop = i + run_ଗଣନା;
      ଯଦି (unlikely (stop > ଗଣନା_2)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

      unsigned skip = i < start ? hb_min (start - i, run_ଗଣନା) : 0;
      i += skip;

      ଯଦି ((control & ମୂଲ୍ୟଗୁଡ଼ିକ_SIZE_MASK) == ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * ଆକାର (T));
	i = stop;
      }
      ଅନ୍ୟଥା ଯଦି ((control & ମୂଲ୍ୟଗୁଡ଼ିକ_SIZE_MASK) ==  ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_ଶବ୍ଦଗୁଡ଼ିକ)
      {
        ଯଦି (unlikely (p + run_ଗଣନା * HBINT16::static_size > end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	p += skip * HBINT16::static_size;
#ifndef HB_OPTIMIZE_SIZE
        ପାଇଁ (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ଅଚଳ HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 1] = * (ଅଚଳ HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 2] = * (ଅଚଳ HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 3] = * (ଅଚଳ HBINT16 *) p;
	  p += HBINT16::static_size;
	}
#endif
        ପାଇଁ (; i < stop; i++)
        {
          values.arrayZ[i] = * (ଅଚଳ HBINT16 *) p;
          p += HBINT16::static_size;
        }
      }
      ଅନ୍ୟଥା ଯଦି ((control & ମୂଲ୍ୟଗୁଡ଼ିକ_SIZE_MASK) ==  ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_LONGS)
      {
        ଯଦି (unlikely (p + run_ଗଣନା * HBINT32::static_size > end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	p += skip * HBINT32::static_size;
        ପାଇଁ (; i < stop; i++)
        {
          values.arrayZ[i] = * (ଅଚଳ HBINT32 *) p;
          p += HBINT32::static_size;
        }
      }
      ଅନ୍ୟଥା ଯଦି ((control & ମୂଲ୍ୟଗୁଡ଼ିକ_SIZE_MASK) ==  ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_BYTES)
      {
        ଯଦି (unlikely (p + run_ଗଣନା > end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	p += skip * HBINT8::static_size;
#ifndef HB_OPTIMIZE_SIZE
	ପାଇଁ (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ଅଚଳ HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (ଅଚଳ HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (ଅଚଳ HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (ଅଚଳ HBINT8 *) p++;
	}
#endif
        ପାଇଁ (; i < stop; i++)
          values.arrayZ[i] = * (ଅଚଳ HBINT8 *) p++;
      }
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ସ୍ଥିର ତାର୍କିକ skip (ଅଚଳ unsigned ଅକ୍ଷର *&p /* IN/OUT */,
		    ଅଚଳ unsigned ଅକ୍ଷର *end,
		    unsigned ଗଣନା_2)
  {
    unsigned seen = 0;
    ଯେତେବେଳେ (seen < ଗଣନା_2)
    {
      ଯଦି (unlikely (p >= end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      unsigned control = *p++;
      unsigned run_ଗଣନା = (control & ମୂଲ୍ୟ_RUN_ଗଣନା_MASK) + 1;
      ଯଦି (unlikely (run_ଗଣନା > ଗଣନା_2 - seen)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

      unsigned width = 0;
      ବାଛ (control & ମୂଲ୍ୟଗୁଡ଼ିକ_SIZE_MASK)
      {
	ସ୍ଥିତି ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_ZEROS: ଭାଙ୍ଗ;
	ସ୍ଥିତି ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_BYTES: width = HBINT8::static_size; ଭାଙ୍ଗ;
	ସ୍ଥିତି ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_ଶବ୍ଦଗୁଡ଼ିକ: width = HBINT16::static_size; ଭାଙ୍ଗ;
	ସ୍ଥିତି ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_LONGS: width = HBINT32::static_size; ଭାଙ୍ଗ;
      }
      ଯଦି (unlikely (unsigned (end - p) < run_ଗଣନା * width)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      p += run_ଗଣନା * width;
      seen += run_ଗଣନା;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ସଂରଚନା iter_t : hb_iter_with_fallback_t<iter_t, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>
  {
    iter_t (ଅଚଳ unsigned ଅକ୍ଷର *p_, unsigned len_)
	    : p (p_), endp (p_ + len_)
    { ଯଦି (likely (ensure_run ())) read_ମୂଲ୍ୟ (); }

    ବ୍ୟକ୍ତିଗତ:
    ଅଚଳ unsigned ଅକ୍ଷର *p;
    ଅଚଳ unsigned ଅକ୍ଷର * ଅଚଳ endp;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା current_ମୂଲ୍ୟ = 0;
    signed run_ଗଣନା = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    ତାର୍କିକ ensure_run ()
    {
      ଯଦି (likely (run_ଗଣନା > 0)) ଫେରନ୍ତୁ ସତ୍ୟ;
      ଫେରନ୍ତୁ _ensure_run ();
    }
    ତାର୍କିକ _ensure_run ()
    {
      ଯଦି (unlikely (p >= endp))
      {
        run_ଗଣନା = 0;
        current_ମୂଲ୍ୟ = 0;
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }

      unsigned control = *p++;
      run_ଗଣନା = (control & ମୂଲ୍ୟ_RUN_ଗଣନା_MASK) + 1;
      width = control & ମୂଲ୍ୟଗୁଡ଼ିକ_SIZE_MASK;
      ବାଛ (width)
      {
        ସ୍ଥିତି ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_ZEROS: width = 0; ଭାଙ୍ଗ;
	ସ୍ଥିତି ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_BYTES: width = HBINT8::static_size;  ଭାଙ୍ଗ;
	ସ୍ଥିତି ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_ଶବ୍ଦଗୁଡ଼ିକ: width = HBINT16::static_size; ଭାଙ୍ଗ;
	ସ୍ଥିତି ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_LONGS: width = HBINT32::static_size; ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ପ୍ରତିଜ୍ଞା (ମିଥ୍ୟା);
      }

      ଯଦି (unlikely (p + run_ଗଣନା * width > endp))
      {
	run_ଗଣନା = 0;
	current_ମୂଲ୍ୟ = 0;
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }

      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଶୂନ୍ୟ read_ମୂଲ୍ୟ ()
    {
      ବାଛ (width)
      {
        ସ୍ଥିତି 0: current_ମୂଲ୍ୟ = 0; ଭାଙ୍ଗ;
	ସ୍ଥିତି 1: current_ମୂଲ୍ୟ = * (ଅଚଳ HBINT8  *) p; ଭାଙ୍ଗ;
	ସ୍ଥିତି 2: current_ମୂଲ୍ୟ = * (ଅଚଳ HBINT16 *) p; ଭାଙ୍ଗ;
	ସ୍ଥିତି 4: current_ମୂଲ୍ୟ = * (ଅଚଳ HBINT32 *) p; ଭାଙ୍ଗ;
      }
      p += width;
    }

    ସର୍ବସାଧାରଣ:

    ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା __item_t__;
    __item_t__ __item__ () ଅଚଳ
    { ଫେରନ୍ତୁ current_ମୂଲ୍ୟ; }

    ତାର୍କିକ __more__ () ଅଚଳ { ଫେରନ୍ତୁ run_ଗଣନା || p < endp; }
    ଶୂନ୍ୟ __next__ ()
    {
      run_ଗଣନା--;
      ଯଦି (unlikely (!ensure_run ()))
	ଫେରନ୍ତୁ;
      read_ମୂଲ୍ୟ ();
    }
    ଶୂନ୍ୟ __forward__ (unsigned n)
    {
      ଯଦି (unlikely (!ensure_run ()))
	ଫେରନ୍ତୁ;
      ଯେତେବେଳେ (n)
      {
	unsigned i = hb_min (n, (unsigned) run_ଗଣନା);
	run_ଗଣନା -= i;
	n -= i;
	p += (i - 1) * width;
	ଯଦି (unlikely (!ensure_run ()))
	  ଫେରନ୍ତୁ;
	read_ମୂଲ୍ୟ ();
      }
    }
    ତାର୍କିକ operator != (ଅଚଳ iter_t& o) ଅଚଳ
    { ଫେରନ୍ତୁ p != o.p || run_ଗଣନା != o.run_ଗଣନା; }
    iter_t __end__ () ଅଚଳ
    {
      iter_t it (endp, 0);
      ଫେରନ୍ତୁ it;
    }
  };

  ସଂରଚନା fetcher_t
  {
    fetcher_t (ଅଚଳ unsigned ଅକ୍ଷର *p_, unsigned len_)
	      : p (p_), end (p_ + len_) {}

    ବ୍ୟକ୍ତିଗତ:
    ଅଚଳ unsigned ଅକ୍ଷର *p;
    ଅଚଳ unsigned ଅକ୍ଷର * ଅଚଳ end;
    signed run_ଗଣନା = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    ତାର୍କିକ ensure_run ()
    {
      ଯଦି (likely (run_ଗଣନା > 0)) ଫେରନ୍ତୁ ସତ୍ୟ;
      ଫେରନ୍ତୁ _ensure_run ();
    }

    ତାର୍କିକ _ensure_run ()
    {
      ଯଦି (unlikely (p >= end))
      {
        run_ଗଣନା = 0;
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }

      unsigned control = *p++;
      run_ଗଣନା = (control & ମୂଲ୍ୟ_RUN_ଗଣନା_MASK) + 1;
      width = control & ମୂଲ୍ୟଗୁଡ଼ିକ_SIZE_MASK;
      ବାଛ (width)
      {
        ସ୍ଥିତି ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_ZEROS: width = 0; ଭାଙ୍ଗ;
	ସ୍ଥିତି ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_BYTES: width = HBINT8::static_size;  ଭାଙ୍ଗ;
	ସ୍ଥିତି ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_ଶବ୍ଦଗୁଡ଼ିକ: width = HBINT16::static_size; ଭାଙ୍ଗ;
	ସ୍ଥିତି ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_LONGS: width = HBINT32::static_size; ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ପ୍ରତିଜ୍ଞା (ମିଥ୍ୟା);
      }

      ଯଦି (unlikely (p + run_ଗଣନା * width > end))
      {
	run_ଗଣନା = 0;
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }

      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ସର୍ବସାଧାରଣ:
    ଶୂନ୍ୟ skip (unsigned n)
    {
      ଯେତେବେଳେ (n)
      {
	ଯଦି (unlikely (!ensure_run ()))
	  ଫେରନ୍ତୁ;
	unsigned i = hb_min (n, (unsigned) run_ଗଣନା);
	run_ଗଣନା -= i;
	n -= i;
	p += i * width;
      }
    }

    ବ୍ୟକ୍ତିଗତ:
    template <ତାର୍କିକ scaled, typename T>
    ଶୂନ୍ୟ _add_to (hb_array_t<T> out, T scale = T (1))
    {
      unsigned n = out.length;
      T *arrayZ = out.arrayZ;

      ପାଇଁ (unsigned i = 0; i < n;)
      {
	ଯଦି (unlikely (!ensure_run ()))
	  ଭାଙ୍ଗ;
	unsigned ଗଣନା_2 = hb_min (n - i, (unsigned) run_ଗଣନା);
	ବାଛ (width)
	{
	  ସ୍ଥିତି 0:
	  {
	    arrayZ += ଗଣନା_2;
	    ଭାଙ୍ଗ;
	  }
	  ସ୍ଥିତି 1:
	  {
	    ଅଚଳ auto *pp = (ଅଚଳ HBINT8 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    ପାଇଁ (; j + 3 < ଗଣନା_2; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    ପାଇଁ (; j < ଗଣନା_2; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ଅଚଳ unsigned ଅକ୍ଷର *) pp;
	  }
	  ଭାଙ୍ଗ;
	  ସ୍ଥିତି 2:
	  {
	    ଅଚଳ auto *pp = (ଅଚଳ HBINT16 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    ପାଇଁ (; j + 3 < ଗଣନା_2; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    ପାଇଁ (; j < ଗଣନା_2; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ଅଚଳ unsigned ଅକ୍ଷର *) pp;
	  }
	  ଭାଙ୍ଗ;
	  ସ୍ଥିତି 4:
	  {
	    ଅଚଳ auto *pp = (ଅଚଳ HBINT32 *) p;
	    ପାଇଁ (unsigned j = 0; j < ଗଣନା_2; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ଅଚଳ unsigned ଅକ୍ଷର *) pp;
	  }
	  ଭାଙ୍ଗ;
	}
	run_ଗଣନା -= ଗଣନା_2;
	i += ଗଣନା_2;
      }
    }

    ସର୍ବସାଧାରଣ:
    template <typename T>
    ଶୂନ୍ୟ add_to (hb_array_t<T> out, ଦଶମିକ scale = 1.0f)
    {
#ifndef HB_OPTIMIZE_SIZE
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      ଯଦି (ମିଥ୍ୟା && scale == 1.0f)
        _add_to<ମିଥ୍ୟା> (out);
      ଅନ୍ୟଥା
#endif
        _add_to<ସତ୍ୟ> (out, (T) scale);
    }
  };
};

ସଂରଚନା TupleList : CFF2Index
{
  Tuple_ମୂଲ୍ୟଗୁଡ଼ିକ::iter_t operator [] (unsigned i) ଅଚଳ
  {
    auto bytes = CFF2Index::operator [] (i);
    ଫେରନ୍ତୁ Tuple_ମୂଲ୍ୟଗୁଡ଼ିକ::iter_t (bytes.arrayZ, bytes.length);
  }

  Tuple_ମୂଲ୍ୟଗୁଡ଼ିକ::fetcher_t fetcher (unsigned i) ଅଚଳ
  {
    auto bytes = CFF2Index::operator [] (i);
    ଫେରନ୍ତୁ Tuple_ମୂଲ୍ୟଗୁଡ଼ିକ::fetcher_t (bytes.arrayZ, bytes.length);
  }
};


// Alignment

template <unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା alignment>
ସଂରଚନା Align
{
  size_t get_size (ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    unsigned offset = (ଅଚଳ ଅକ୍ଷର *) ଏହି - (ଅଚଳ ଅକ୍ଷର *) base;
    ଫେରନ୍ତୁ (alignment - offset) & (alignment - 1);
  }

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (0);
};



} /* namespace OT */


#endif /* HB_OPEN_TYPE_HH */
