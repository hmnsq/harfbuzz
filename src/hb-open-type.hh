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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-blob.hh"
#ಅನ್ತರ್ಭಾವ "hb-face.hh"
#ಅನ್ತರ್ಭಾವ "hb-machinery.hh"
#ಅನ್ತರ್ಭಾವ "hb-meta.hh"
#ಅನ್ತರ್ಭಾವ "hb-subset.hh"


ನಾಮಾಕಾಶ OT {


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
template <ತಾರ್ಕಿಕ BE,
	  typename Type,
	  unsigned ಪೂರ್ಣಾಙ್ಕ Size = ಪರಿಮಾಣ (Type)>
ಸಂರಚನಾ ಅಂಕೆ_Type
{
  ಪ್ರಕಾರನಿರ್ದೇಶ Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  ಪ್ರಕಾರನಿರ್ದೇಶ typename std::conditional<std::is_integral<Type>::value && ಪರಿಮಾಣ (Type) <= ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ),
				     typename std::conditional<std::is_signed<Type>::value, signed, unsigned>::type,
				     Type>::type WideType;

  ಅಂಕೆ_Type () = ಸ್ವಭಾವ;
  explicit constexpr ಅಂಕೆ_Type (Type V) : v {V} {}
  ಅಂಕೆ_Type& operator = (Type V) { v = V; ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

  operator WideType () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v; }

  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ ಅಂಕೆ_Type &o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (Type) v == (Type) o.v; }
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ ಅಂಕೆ_Type &o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !(*ಏತತ್ == o); }

  ಅಂಕೆ_Type& operator += (WideType ಎಣಿಕೆ) { *ಏತತ್ = *ಏತತ್ + ಎಣಿಕೆ; ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  ಅಂಕೆ_Type& operator -= (WideType ಎಣಿಕೆ) { *ಏತತ್ = *ಏತತ್ - ಎಣಿಕೆ; ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  ಅಂಕೆ_Type& operator ++ () { *ಏತತ್ += 1; ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  ಅಂಕೆ_Type& operator -- () { *ಏತತ್ -= 1; ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  ಅಂಕೆ_Type operator ++ (ಪೂರ್ಣಾಙ್ಕ) { ಅಂಕೆ_Type c (*ಏತತ್); ++*ಏತತ್; ಪ್ರತ್ಯಾಗಮ c; }
  ಅಂಕೆ_Type operator -- (ಪೂರ್ಣಾಙ್ಕ) { ಅಂಕೆ_Type c (*ಏತತ್); --*ಏತತ್; ಪ್ರತ್ಯಾಗಮ c; }

  uint32_t hash () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_array ((ಧ್ರುವ ವರ್ಣ *) &v, ಪರಿಮಾಣ (v)).hash (); }
  HB_INTERNAL ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ ಅಂಕೆ_Type *a, ಧ್ರುವ ಅಂಕೆ_Type *b)
  { ಪ್ರತ್ಯಾಗಮ b->cmp (*a); }
  HB_INTERNAL ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b)
  {
    ಅಂಕೆ_Type *pa = (ಅಂಕೆ_Type *) a;
    ಅಂಕೆ_Type *pb = (ಅಂಕೆ_Type *) b;

    ಪ್ರತ್ಯಾಗಮ pb->cmp (*pa);
  }
  template <typename Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  ಪೂರ್ಣಾಙ್ಕ cmp (Type2 a) ಧ್ರುವ
  {
    Type b = v;
    ಪ್ರತ್ಯಾಗಮ (a > b) - (a < b);
  }
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }
  ರಕ್ಷಿತ:
  typename std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, Size>,
			    HBFloat<BE, Type, Size>>::type v;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (Size);
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಸತ್ಯ, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಸತ್ಯ, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಸತ್ಯ, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಸತ್ಯ, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಸತ್ಯ, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಸತ್ಯ, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಸತ್ಯ, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಸತ್ಯ, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಸತ್ಯ, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಅಸತ್ಯ, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಅಸತ್ಯ, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಅಸತ್ಯ, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಅಸತ್ಯ, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಅಸತ್ಯ, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಅಸತ್ಯ, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಸತ್ಯ,  ಭಿನ್ನ>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಸತ್ಯ,  ದ್ವಿಗುಣ> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಅಸತ್ಯ, ಭಿನ್ನ>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಅಂಕೆ_Type<ಅಸತ್ಯ, ದ್ವಿಗುಣ> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
ಸಂರಚನಾ HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& operator = (uint16_t i ) { HBUINT16::operator= (i); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
ಸಂರಚನಾ HBUINT32VAR
{
  size_t get_size () ಧ್ರುವ
  {
    unsigned b0 = v[0];
    ಯದಿ (b0 < 0x80)
      ಪ್ರತ್ಯಾಗಮ 1;
    ಅನ್ಯಥಾ ಯದಿ (b0 < 0xC0)
      ಪ್ರತ್ಯಾಗಮ 2;
    ಅನ್ಯಥಾ ಯದಿ (b0 < 0xE0)
      ಪ್ರತ್ಯಾಗಮ 3;
    ಅನ್ಯಥಾ ಯದಿ (b0 < 0xF0)
      ಪ್ರತ್ಯಾಗಮ 4;
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ 5;
  }

  ಸ್ಥಿರ size_t get_size (uint32_t v)
  {
    ಯದಿ (v < 0x80)
      ಪ್ರತ್ಯಾಗಮ 1;
    ಅನ್ಯಥಾ ಯದಿ (v < 0x4000)
      ಪ್ರತ್ಯಾಗಮ 2;
    ಅನ್ಯಥಾ ಯದಿ (v < 0x200000)
      ಪ್ರತ್ಯಾಗಮ 3;
    ಅನ್ಯಥಾ ಯದಿ (v < 0x10000000)
      ಪ್ರತ್ಯಾಗಮ 4;
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ 5;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_size ()));
  }

  operator uint32_t () ಧ್ರುವ
  {
    unsigned b0 = v[0];
    ಯದಿ (b0 < 0x80)
      ಪ್ರತ್ಯಾಗಮ b0;
    ಅನ್ಯಥಾ ಯದಿ (b0 < 0xC0)
      ಪ್ರತ್ಯಾಗಮ ((b0 & 0x3F) << 8) | v[1];
    ಅನ್ಯಥಾ ಯದಿ (b0 < 0xE0)
      ಪ್ರತ್ಯಾಗಮ ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    ಅನ್ಯಥಾ ಯದಿ (b0 < 0xF0)
      ಪ್ರತ್ಯಾಗಮ ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  ಸ್ಥಿರ unsigned serialize_unsafe (unsigned ವರ್ಣ *buf, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned ವರ್ಣ *p = buf + len;
    ಪ್ರತಿ (unsigned i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    ಯದಿ (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    ಪ್ರತ್ಯಾಗಮ len;
  }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned ವರ್ಣ *buf = c->allocate_size<unsigned ವರ್ಣ> (len, ಅಸತ್ಯ);
    ಯದಿ (unlikely (!buf))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    serialize_unsafe (buf, v);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ರಕ್ಷಿತ:
  unsigned ವರ್ಣ v[5];

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
ಪ್ರಕಾರನಿರ್ದೇಶ HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
ಪ್ರಕಾರನಿರ್ದೇಶ HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
ಪ್ರಕಾರನಿರ್ದೇಶ HBUINT16 UFWORD;

template <typename Type, unsigned fraction_bits>
ಸಂರಚನಾ HBFixed : Type
{
  ಸ್ಥಿರ constexpr ಭಿನ್ನ mult = 1.f / (1 << fraction_bits);
  static_assert (Type::static_size * 8 > fraction_bits, "");

  operator signed () ಧ್ರುವ = delete;
  operator unsigned () ಧ್ರುವ = delete;
  explicit operator ಭಿನ್ನ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ to_float (); }
  typename Type::type to_int () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ Type::v; }
  ಶೂನ್ಯ set_int (typename Type::type i ) { Type::v = i; }
  ಭಿನ್ನ to_float (ಭಿನ್ನ offset = 0) ಧ್ರುವ  { ಪ್ರತ್ಯಾಗಮ ((int32_t) Type::v + offset) * mult; }
  ಶೂನ್ಯ set_float (ಭಿನ್ನ f) { Type::v = roundf (f / mult); }
  ಸಾರ್ವಜನಿಕ:
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
ಸಂರಚನಾ LONGDATETIME
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }
  ರಕ್ಷಿತ:
  HBINT32 major;
  HBUINT32 minor;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
ಸಂರಚನಾ Tag : HBUINT32
{
  Tag& operator = (hb_tag_t i) { HBUINT32::operator= (i); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  operator ಧ್ರುವ ವರ್ಣ* () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ reinterpret_cast<ಧ್ರುವ ವರ್ಣ *> (ಏತತ್); }
  operator ವರ್ಣ* ()             { ಪ್ರತ್ಯಾಗಮ reinterpret_cast<ವರ್ಣ *> (ಏತತ್); }
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
ಸಂರಚನಾ HBGlyphID16 : HBUINT16
{
  HBGlyphID16& operator = (uint16_t i) { HBUINT16::operator= (i); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
};
ಸಂರಚನಾ HBGlyphID24 : HBUINT24
{
  HBGlyphID24& operator = (uint32_t i) { HBUINT24::operator= (i); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
};

/* Script/language-system/feature index */
ಸಂರಚನಾ Index : HBUINT16 {
  ಸ್ಥಿರ constexpr unsigned NOT_FOUND_INDEX = 0xFFFFu;
  Index& operator = (uint16_t i) { HBUINT16::operator= (i); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, Index);

ಪ್ರಕಾರನಿರ್ದೇಶ Index NameID;

ಸಂರಚನಾ VarIdx : HBUINT32 {
  ಸ್ಥಿರ constexpr unsigned NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_INDEX, "");
  VarIdx& operator = (uint32_t i) { HBUINT32::operator= (i); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, VarIdx);

/* Offset, Null offset = 0 */
template <typename Type, ತಾರ್ಕಿಕ has_null=ಸತ್ಯ>
ಸಂರಚನಾ Offset : Type
{
  Offset& operator = (typename Type::type i) { Type::operator= (i); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

  ಪ್ರಕಾರನಿರ್ದೇಶ Type type;

  ತಾರ್ಕಿಕ is_null () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_null && 0 == *ಏತತ್; }

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (ಪರಿಮಾಣ (Type));
};

ಪ್ರಕಾರನಿರ್ದೇಶ Offset<HBUINT16> Offset16;
ಪ್ರಕಾರನಿರ್ದೇಶ Offset<HBUINT24> Offset24;
ಪ್ರಕಾರನಿರ್ದೇಶ Offset<HBUINT32> Offset32;


/* CheckSum */
ಸಂರಚನಾ CheckSum : HBUINT32
{
  CheckSum& operator = (uint32_t i) { HBUINT32::operator= (i); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

  /* This is reference implementation from the spec. */
  ಸ್ಥಿರ uint32_t CalcTableChecksum (ಧ್ರುವ HBUINT32 *Table, uint32_t Length)
  {
    uint32_t Sum = 0L;
    ಪ್ರತಿಜ್ಞಾ (0 == (Length & 3));
    ಧ್ರುವ HBUINT32 *EndPtr = Table + Length / HBUINT32::static_size;

    ಯಾವತ್ (Table < EndPtr)
      Sum += *Table++;
    ಪ್ರತ್ಯಾಗಮ Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  ಶೂನ್ಯ set_for_data (ಧ್ರುವ ಶೂನ್ಯ *data, unsigned ಪೂರ್ಣಾಙ್ಕ length)
  { *ಏತತ್ = CalcTableChecksum ((ಧ್ರುವ HBUINT32 *) data, length); }

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};


/*
 * Version Numbers
 */

template <typename FixedType=HBUINT16>
ಸಂರಚನಾ FixedVersion
{
  uint32_t to_int () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (major << (ಪರಿಮಾಣ (FixedType) * 8)) + minor; }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  FixedType major;
  FixedType minor;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (2 * ಪರಿಮಾಣ (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

template <typename Type, ತಾರ್ಕಿಕ has_null>
ಸಂರಚನಾ _hb_has_null
{
  ಸ್ಥಿರ ಧ್ರುವ Type *get_null () { ಪ್ರತ್ಯಾಗಮ nullptr; }
  ಸ್ಥಿರ Type *get_crap ()       { ಪ್ರತ್ಯಾಗಮ nullptr; }
};
template <typename Type>
ಸಂರಚನಾ _hb_has_null<Type, ಸತ್ಯ>
{
  ಸ್ಥಿರ ಧ್ರುವ Type *get_null () { ಪ್ರತ್ಯಾಗಮ &Null (Type); }
  ಸ್ಥಿರ       Type *get_crap () { ಪ್ರತ್ಯಾಗಮ &Crap (Type); }
};

template <typename Type, typename OffsetType, typename BaseType=ಶೂನ್ಯ, ತಾರ್ಕಿಕ has_null=ಸತ್ಯ>
ಸಂರಚನಾ OffsetTo : Offset<OffsetType, has_null>
{
  using target_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == ಅಸತ್ಯ ||
		 (hb_has_null_size (Type) || !hb_has_min_size (Type)), "");

  HB_DELETE_COPY_ASSIGN (OffsetTo);
  OffsetTo () = ಸ್ವಭಾವ;

  OffsetTo& operator = (typename OffsetType::type i) { OffsetType::operator= (i); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

  ಧ್ರುವ Type& operator () (ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    ಯದಿ (unlikely (ಏತತ್->is_null ())) ಪ್ರತ್ಯಾಗಮ *_hb_has_null<Type, has_null>::get_null ();
    ಪ್ರತ್ಯಾಗಮ StructAtOffset<ಧ್ರುವ Type> (base, *ಏತತ್);
  }
  Type& operator () (ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    ಯದಿ (unlikely (ಏತತ್->is_null ())) ಪ್ರತ್ಯಾಗಮ *_hb_has_null<Type, has_null>::get_crap ();
    ಪ್ರತ್ಯಾಗಮ StructAtOffset<Type> (base, *ಏತತ್);
  }

  template <typename Base,
	    hb_enable_if (hb_is_convertible (ಧ್ರುವ Base, ಧ್ರುವ BaseType *))>
  friend ಧ್ರುವ Type& operator + (ಧ್ರುವ Base &base, ಧ್ರುವ OffsetTo &offset) { ಪ್ರತ್ಯಾಗಮ offset ((ಧ್ರುವ ಶೂನ್ಯ *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (ಧ್ರುವ Base, ಧ್ರುವ BaseType *))>
  friend ಧ್ರುವ Type& operator + (ಧ್ರುವ OffsetTo &offset, ಧ್ರುವ Base &base) { ಪ್ರತ್ಯಾಗಮ offset ((ಧ್ರುವ ಶೂನ್ಯ *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (Base &&base, OffsetTo &offset) { ಪ್ರತ್ಯಾಗಮ offset ((ಶೂನ್ಯ *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (OffsetTo &offset, Base &&base) { ಪ್ರತ್ಯಾಗಮ offset ((ಶೂನ್ಯ *) base); }


  template <typename Base, typename ...Ts>
  ತಾರ್ಕಿಕ serialize_subset (hb_subset_context_t *c, ಧ್ರುವ OffsetTo& src,
			 ಧ್ರುವ Base *src_base, Ts&&... ds)
  {
    *ಏತತ್ = 0;
    ಯದಿ (src.is_null ())
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    auto *s = c->serializer;

    s->push ();

    ತಾರ್ಕಿಕ ret = c->dispatch (src_base+src, std::forward<Ts> (ds)...);

    ಯದಿ (ret || !has_null)
      s->add_link (*ಏತತ್, s->ತೆಗೆ_pack ());
    ಅನ್ಯಥಾ
      s->ತೆಗೆ_discard ();

    ಪ್ರತ್ಯಾಗಮ ret;
  }


  template <typename ...Ts>
  ತಾರ್ಕಿಕ serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *ಏತತ್ = 0;

    Type* obj = c->push<Type> ();
    ತಾರ್ಕಿಕ ret = obj->serialize (c, std::forward<Ts> (ds)...);

    ಯದಿ (ret)
      c->add_link (*ಏತತ್, c->ತೆಗೆ_pack ());
    ಅನ್ಯಥಾ
      c->ತೆಗೆ_discard ();

    ಪ್ರತ್ಯಾಗಮ ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  template <typename ...Ts>
  ತಾರ್ಕಿಕ serialize_copy (hb_serialize_context_t *c, ಧ್ರುವ OffsetTo& src,
		       ಧ್ರುವ ಶೂನ್ಯ *src_base, unsigned dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *ಏತತ್ = 0;
    ಯದಿ (src.is_null ())
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    c->push ();

    ತಾರ್ಕಿಕ ret = c->copy (src_base+src, std::forward<Ts> (ds)...);

    c->add_link (*ಏತತ್, c->ತೆಗೆ_pack (), whence, dst_bias);

    ಪ್ರತ್ಯಾಗಮ ret;
  }

  ತಾರ್ಕಿಕ serialize_copy (hb_serialize_context_t *c, ಧ್ರುವ OffsetTo& src,
		       ಧ್ರುವ ಶೂನ್ಯ *src_base, unsigned dst_bias = 0)
  { ಪ್ರತ್ಯಾಗಮ serialize_copy (c, src, src_base, dst_bias, hb_serialize_context_t::Head); }

  ತಾರ್ಕಿಕ sanitize_shallow (hb_sanitize_context_t *c, ಧ್ರುವ BaseType *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->check_struct (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    ಯದಿ (unlikely ((ಧ್ರುವ ವರ್ಣ *) base + (unsigned) *ಏತತ್ < (ಧ್ರುವ ವರ್ಣ *) base)) return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  template <typename ...Ts>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ BaseType *base, Ts&&... ds) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (sanitize_shallow (c, base) &&
		  hb_barrier () &&
		  (ಏತತ್->is_null () ||
		   c->dispatch (StructAtOffset<Type> (base, *ಏತತ್), std::forward<Ts> (ds)...)));
  }

  DEFINE_SIZE_STATIC (ಪರಿಮಾಣ (OffsetType));
};
/* Partial specializations. */
template <typename Type, typename BaseType=ಶೂನ್ಯ, ತಾರ್ಕಿಕ has_null=ಸತ್ಯ> using Offset16To = OffsetTo<Type, HBUINT16, BaseType, has_null>;
template <typename Type, typename BaseType=ಶೂನ್ಯ, ತಾರ್ಕಿಕ has_null=ಸತ್ಯ> using Offset24To = OffsetTo<Type, HBUINT24, BaseType, has_null>;
template <typename Type, typename BaseType=ಶೂನ್ಯ, ತಾರ್ಕಿಕ has_null=ಸತ್ಯ> using Offset32To = OffsetTo<Type, HBUINT32, BaseType, has_null>;

template <typename Type, typename OffsetType, typename BaseType=ಶೂನ್ಯ> using NNOffsetTo = OffsetTo<Type, OffsetType, BaseType, ಅಸತ್ಯ>;
template <typename Type, typename BaseType=ಶೂನ್ಯ> using NNOffset16To = Offset16To<Type, BaseType, ಅಸತ್ಯ>;
template <typename Type, typename BaseType=ಶೂನ್ಯ> using NNOffset24To = Offset24To<Type, BaseType, ಅಸತ್ಯ>;
template <typename Type, typename BaseType=ಶೂನ್ಯ> using NNOffset32To = Offset32To<Type, BaseType, ಅಸತ್ಯ>;


/*
 * Array Types
 */

template <typename Type>
ಸಂರಚನಾ UnsizedArrayOf
{
  ಪ್ರಕಾರನಿರ್ದೇಶ Type item_t;
  ಸ್ಥಿರ constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (UnsizedArrayOf);

  ಧ್ರುವ Type& operator [] (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ arrayZ[i];
  }
  Type& operator [] (unsigned ಪೂರ್ಣಾಙ್ಕ i)
  {
    ಪ್ರತ್ಯಾಗಮ arrayZ[i];
  }

  ಸ್ಥಿರ size_t get_size (unsigned ಪೂರ್ಣಾಙ್ಕ len)
  { ಪ್ರತ್ಯಾಗಮ hb_unsigned_mul_saturate (len, Type::static_size); }

  template <typename T> operator T * () { ಪ್ರತ್ಯಾಗಮ arrayZ; }
  template <typename T> operator ಧ್ರುವ T * () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ arrayZ; }
  hb_array_t<Type> as_array (unsigned ಪೂರ್ಣಾಙ್ಕ len)
  { ಪ್ರತ್ಯಾಗಮ hb_array (arrayZ, len); }
  hb_array_t<ಧ್ರುವ Type> as_array (unsigned ಪೂರ್ಣಾಙ್ಕ len) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_array (arrayZ, len); }

  template <typename T>
  Type &lsearch (unsigned ಪೂರ್ಣಾಙ್ಕ len, ಧ್ರುವ T &x, Type &not_found = Crap (Type))
  { ಪ್ರತ್ಯಾಗಮ *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  ಧ್ರುವ Type &lsearch (unsigned ಪೂರ್ಣಾಙ್ಕ len, ಧ್ರುವ T &x, ಧ್ರುವ Type &not_found = Null (Type)) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  ತಾರ್ಕಿಕ lfind (unsigned ಪೂರ್ಣಾಙ್ಕ len, ಧ್ರುವ T &x, unsigned ಪೂರ್ಣಾಙ್ಕ *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ಪೂರ್ಣಾಙ್ಕ to_store = (unsigned ಪೂರ್ಣಾಙ್ಕ) -1) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ as_array (len).lfind (x, i, not_found, to_store); }

  ಶೂನ್ಯ qsort (unsigned ಪೂರ್ಣಾಙ್ಕ len, unsigned ಪೂರ್ಣಾಙ್ಕ start = 0, unsigned ಪೂರ್ಣಾಙ್ಕ end = (unsigned ಪೂರ್ಣಾಙ್ಕ) -1)
  { as_array (len).qsort (start, end); }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ ವಸ್ತುಗಳು_len, ತಾರ್ಕಿಕ clear = ಸತ್ಯ)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_size (ಏತತ್, get_size (ವಸ್ತುಗಳು_len), clear))) return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, Iterator ವಸ್ತುಗಳು)
  {
    TRACE_SERIALIZE (ಏತತ್);
    unsigned ಎಣಿಕೆ = hb_len (ವಸ್ತುಗಳು);
    ಯದಿ (unlikely (!serialize (c, ಎಣಿಕೆ, ಅಸತ್ಯ))) return_trace (ಅಸತ್ಯ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++, ++ವಸ್ತುಗಳು)
      arrayZ[i] = *ವಸ್ತುಗಳು;
    return_trace (ಸತ್ಯ);
  }

  UnsizedArrayOf* copy (hb_serialize_context_t *c, unsigned ಎಣಿಕೆ) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto *out = c->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!as_array (ಎಣಿಕೆ).copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, Ts&&... ds) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!sanitize_shallow (c, ಎಣಿಕೆ))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!ಪರಿಮಾಣ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ಸತ್ಯ);
    hb_barrier ();
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize_shallow (hb_sanitize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_array (arrayZ, ಎಣಿಕೆ));
  }

  ಸಾರ್ವಜನಿಕ:
  Type		arrayZ[HB_VAR_ARRAY];
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNBOUNDED (0);
};

/* Unsized array of offset's */
template <typename Type, typename OffsetType, typename BaseType=ಶೂನ್ಯ, ತಾರ್ಕಿಕ has_null=ಸತ್ಯ>
using UnsizedArray16OfOffsetTo = UnsizedArrayOf<OffsetTo<Type, OffsetType, BaseType, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
template <typename Type, typename OffsetType, typename BaseType=ಶೂನ್ಯ, ತಾರ್ಕಿಕ has_null=ಸತ್ಯ>
ಸಂರಚನಾ UnsizedListOfOffset16To : UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
{
  ಧ್ರುವ Type& operator [] (ಪೂರ್ಣಾಙ್ಕ i_) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = (unsigned ಪೂರ್ಣಾಙ್ಕ) i_;
    ಧ್ರುವ OffsetTo<Type, OffsetType, BaseType, has_null> *p = &ಏತತ್->arrayZ[i];
    ಯದಿ (unlikely ((ಧ್ರುವ ಶೂನ್ಯ *) p < (ಧ್ರುವ ಶೂನ್ಯ *) ಏತತ್->arrayZ)) ಪ್ರತ್ಯಾಗಮ Null (Type); /* Overflowed. */
    hb_barrier ();
    ಪ್ರತ್ಯಾಗಮ ಏತತ್+*p;
  }
  Type& operator [] (ಪೂರ್ಣಾಙ್ಕ i_)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = (unsigned ಪೂರ್ಣಾಙ್ಕ) i_;
    ಧ್ರುವ OffsetTo<Type, OffsetType, BaseType, has_null> *p = &ಏತತ್->arrayZ[i];
    ಯದಿ (unlikely ((ಧ್ರುವ ಶೂನ್ಯ *) p < (ಧ್ರುವ ಶೂನ್ಯ *) ಏತತ್->arrayZ)) ಪ್ರತ್ಯಾಗಮ Crap (Type); /* Overflowed. */
    hb_barrier ();
    ಪ್ರತ್ಯಾಗಮ ಏತತ್+*p;
  }

  template <typename ...Ts>
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, Ts&&... ds) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace ((UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
		   ::sanitize (c, ಎಣಿಕೆ, ಏತತ್, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type>
ಸಂರಚನಾ SortedUnsizedArrayOf : UnsizedArrayOf<Type>
{
  hb_sorted_array_t<Type> as_array (unsigned ಪೂರ್ಣಾಙ್ಕ len)
  { ಪ್ರತ್ಯಾಗಮ hb_sorted_array (ಏತತ್->arrayZ, len); }
  hb_sorted_array_t<ಧ್ರುವ Type> as_array (unsigned ಪೂರ್ಣಾಙ್ಕ len) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_sorted_array (ಏತತ್->arrayZ, len); }
  operator hb_sorted_array_t<Type> ()             { ಪ್ರತ್ಯಾಗಮ as_array (); }
  operator hb_sorted_array_t<ಧ್ರುವ Type> () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ as_array (); }

  template <typename T>
  Type &bsearch (unsigned ಪೂರ್ಣಾಙ್ಕ len, ಧ್ರುವ T &x, Type &not_found = Crap (Type))
  { ಪ್ರತ್ಯಾಗಮ *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  ಧ್ರುವ Type &bsearch (unsigned ಪೂರ್ಣಾಙ್ಕ len, ಧ್ರುವ T &x, ಧ್ರುವ Type &not_found = Null (Type)) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  ತಾರ್ಕಿಕ bfind (unsigned ಪೂರ್ಣಾಙ್ಕ len, ಧ್ರುವ T &x, unsigned ಪೂರ್ಣಾಙ್ಕ *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ಪೂರ್ಣಾಙ್ಕ to_store = (unsigned ಪೂರ್ಣಾಙ್ಕ) -1) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ as_array (len).bfind (x, i, not_found, to_store); }
};


/* An array with a number of elements. */
template <typename Type, typename LenType>
ಸಂರಚನಾ ArrayOf
{
  ಪ್ರಕಾರನಿರ್ದೇಶ Type item_t;
  ಸ್ಥಿರ constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOf);

  ಧ್ರುವ Type& operator [] (ಪೂರ್ಣಾಙ್ಕ i_) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = (unsigned ಪೂರ್ಣಾಙ್ಕ) i_;
    ಯದಿ (unlikely (i >= len)) ಪ್ರತ್ಯಾಗಮ Null (Type);
    hb_barrier ();
    ಪ್ರತ್ಯಾಗಮ arrayZ[i];
  }
  Type& operator [] (ಪೂರ್ಣಾಙ್ಕ i_)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = (unsigned ಪೂರ್ಣಾಙ್ಕ) i_;
    ಯದಿ (unlikely (i >= len)) ಪ್ರತ್ಯಾಗಮ Crap (Type);
    hb_barrier ();
    ಪ್ರತ್ಯಾಗಮ arrayZ[i];
  }

  size_t get_size () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_unsigned_mul_add_saturate (len, Type::static_size, len.static_size); }

  explicit operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ len; }

  ಶೂನ್ಯ ತೆಗೆ () { len--; }

  hb_array_t<      Type> as_array ()       { ಪ್ರತ್ಯಾಗಮ hb_array (arrayZ, len); }
  hb_array_t<ಧ್ರುವ Type> as_array () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_array (arrayZ, len); }

  /* Iterator. */
  ಪ್ರಕಾರನಿರ್ದೇಶ hb_array_t<ಧ್ರುವ Type>   iter_t;
  ಪ್ರಕಾರನಿರ್ದೇಶ hb_array_t<      Type> writer_t;
    iter_t   iter () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ as_array (); }
  writer_t writer ()       { ಪ್ರತ್ಯಾಗಮ as_array (); }
  operator   iter_t () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ   iter (); }
  operator writer_t ()       { ಪ್ರತ್ಯಾಗಮ writer (); }

  /* Faster range-based for loop. */
  ಧ್ರುವ Type *begin () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ arrayZ; }
  ಧ್ರುವ Type *end () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ arrayZ + len; }

  template <typename T>
  Type &lsearch (ಧ್ರುವ T &x, Type &not_found = Crap (Type))
  { ಪ್ರತ್ಯಾಗಮ *as_array ().lsearch (x, &not_found); }
  template <typename T>
  ಧ್ರುವ Type &lsearch (ಧ್ರುವ T &x, ಧ್ರುವ Type &not_found = Null (Type)) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ *as_array ().lsearch (x, &not_found); }
  template <typename T>
  ತಾರ್ಕಿಕ lfind (ಧ್ರುವ T &x, unsigned ಪೂರ್ಣಾಙ್ಕ *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ಪೂರ್ಣಾಙ್ಕ to_store = (unsigned ಪೂರ್ಣಾಙ್ಕ) -1) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ as_array ().lfind (x, i, not_found, to_store); }

  ಶೂನ್ಯ qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, unsigned ವಸ್ತುಗಳು_len, ತಾರ್ಕಿಕ clear = ಸತ್ಯ)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    c->check_assign (len, ವಸ್ತುಗಳು_len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    ಯದಿ (unlikely (!c->extend_size (ಏತತ್, get_size (), clear))) return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, Iterator ವಸ್ತುಗಳು)
  {
    TRACE_SERIALIZE (ಏತತ್);
    unsigned ಎಣಿಕೆ = hb_len (ವಸ್ತುಗಳು);
    ಯದಿ (unlikely (!serialize (c, ಎಣಿಕೆ, ಅಸತ್ಯ))) return_trace (ಅಸತ್ಯ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++, ++ವಸ್ತುಗಳು)
      arrayZ[i] = *ವಸ್ತುಗಳು;
    return_trace (ಸತ್ಯ);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (ಏತತ್);
    len++;
    ಯದಿ (unlikely (!len || !c->extend (ಏತತ್)))
    {
      len--;
      return_trace (nullptr);
    }
    return_trace (&arrayZ[len - 1]);
  }

  ArrayOf* copy (hb_serialize_context_t *c) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto *out = c->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (out))) return_trace (nullptr);
    c->check_assign (out->len, len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    ಯದಿ (unlikely (!as_array ().copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!sanitize_shallow (c))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!ಪರಿಮಾಣ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ಸತ್ಯ);
    hb_barrier ();
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize_shallow (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (len.sanitize (c) &&
		  hb_barrier () &&
		  c->check_array_sized (arrayZ, len, ಪರಿಮಾಣ (LenType)));
  }

  ಸಾರ್ವಜನಿಕ:
  LenType	len;
  Type		arrayZ[HB_VAR_ARRAY];
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (ಪರಿಮಾಣ (LenType), arrayZ);
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
ಸಂರಚನಾ List16OfOffsetTo : ArrayOf<OffsetTo<Type, OffsetType>, HBUINT16>
{
  ಧ್ರುವ Type& operator [] (ಪೂರ್ಣಾಙ್ಕ i_) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = (unsigned ಪೂರ್ಣಾಙ್ಕ) i_;
    ಯದಿ (unlikely (i >= ಏತತ್->len)) ಪ್ರತ್ಯಾಗಮ Null (Type);
    hb_barrier ();
    ಪ್ರತ್ಯಾಗಮ ಏತತ್+ಏತತ್->arrayZ[i];
  }
  ಧ್ರುವ Type& operator [] (ಪೂರ್ಣಾಙ್ಕ i_)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = (unsigned ಪೂರ್ಣಾಙ್ಕ) i_;
    ಯದಿ (unlikely (i >= ಏತತ್->len)) ಪ್ರತ್ಯಾಗಮ Crap (Type);
    hb_barrier ();
    ಪ್ರತ್ಯಾಗಮ ಏತತ್+ಏತತ್->arrayZ[i];
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಸಂರಚನಾ List16OfOffsetTo *out = c->serializer->embed (*ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = ಏತತ್->len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      out->arrayZ[i].serialize_subset (c, ಏತತ್->arrayZ[i], ಏತತ್, out);
    return_trace (ಸತ್ಯ);
  }

  template <typename ...Ts>
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace ((Array16Of<OffsetTo<Type, OffsetType>>::sanitize (c, ಏತತ್, std::forward<Ts> (ds)...)));
  }
};

template <typename Type>
using List16OfOffset16To = List16OfOffsetTo<Type, HBUINT16>;

/* An array starting at second element. */
template <typename Type, typename LenType>
ಸಂರಚನಾ HeadlessArrayOf
{
  ಸ್ಥಿರ constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (HeadlessArrayOf);

  ಧ್ರುವ Type& operator [] (ಪೂರ್ಣಾಙ್ಕ i_) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = (unsigned ಪೂರ್ಣಾಙ್ಕ) i_;
    ಯದಿ (unlikely (i >= lenP1 || !i)) ಪ್ರತ್ಯಾಗಮ Null (Type);
    hb_barrier ();
    ಪ್ರತ್ಯಾಗಮ arrayZ[i-1];
  }
  Type& operator [] (ಪೂರ್ಣಾಙ್ಕ i_)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = (unsigned ಪೂರ್ಣಾಙ್ಕ) i_;
    ಯದಿ (unlikely (i >= lenP1 || !i)) ಪ್ರತ್ಯಾಗಮ Crap (Type);
    hb_barrier ();
    ಪ್ರತ್ಯಾಗಮ arrayZ[i-1];
  }
  size_t get_size () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_unsigned_mul_add_saturate (get_length (), Type::static_size, lenP1.static_size); }

  unsigned get_length () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { ಪ್ರತ್ಯಾಗಮ hb_array (arrayZ, get_length ()); }
  hb_array_t<ಧ್ರುವ Type> as_array () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_array (arrayZ, get_length ()); }

  /* Iterator. */
  ಪ್ರಕಾರನಿರ್ದೇಶ hb_array_t<ಧ್ರುವ Type>   iter_t;
  ಪ್ರಕಾರನಿರ್ದೇಶ hb_array_t<      Type> writer_t;
    iter_t   iter () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ as_array (); }
  writer_t writer ()       { ಪ್ರತ್ಯಾಗಮ as_array (); }
  operator   iter_t () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ   iter (); }
  operator writer_t ()       { ಪ್ರತ್ಯಾಗಮ writer (); }

  /* Faster range-based for loop. */
  ಧ್ರುವ Type *begin () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ arrayZ; }
  ಧ್ರುವ Type *end () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ arrayZ + get_length (); }

  HB_NODISCARD ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ ವಸ್ತುಗಳು_len, ತಾರ್ಕಿಕ clear = ಸತ್ಯ)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    c->check_assign (lenP1, ವಸ್ತುಗಳು_len + 1, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    ಯದಿ (unlikely (!c->extend_size (ಏತತ್, get_size (), clear))) return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, Iterator ವಸ್ತುಗಳು)
  {
    TRACE_SERIALIZE (ಏತತ್);
    unsigned ಎಣಿಕೆ = hb_len (ವಸ್ತುಗಳು);
    ಯದಿ (unlikely (!serialize (c, ಎಣಿಕೆ, ಅಸತ್ಯ))) return_trace (ಅಸತ್ಯ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++, ++ವಸ್ತುಗಳು)
      arrayZ[i] = *ವಸ್ತುಗಳು;
    return_trace (ಸತ್ಯ);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!sanitize_shallow (c))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!ಪರಿಮಾಣ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ಸತ್ಯ);
    hb_barrier ();
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = get_length ();
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  ಗುಪ್ತ:
  ತಾರ್ಕಿಕ sanitize_shallow (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (lenP1.sanitize (c) &&
		  hb_barrier () &&
		  (!lenP1 || c->check_array_sized (arrayZ, lenP1 - 1, ಪರಿಮಾಣ (LenType))));
  }

  ಸಾರ್ವಜನಿಕ:
  LenType	lenP1;
  Type		arrayZ[HB_VAR_ARRAY];
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (ಪರಿಮಾಣ (LenType), arrayZ);
};
template <typename Type> using HeadlessArray16Of = HeadlessArrayOf<Type, HBUINT16>;

/* An array storing length-1. */
template <typename Type, typename LenType=HBUINT16>
ಸಂರಚನಾ ArrayOfM1
{
  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOfM1);

  ಧ್ರುವ Type& operator [] (ಪೂರ್ಣಾಙ್ಕ i_) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = (unsigned ಪೂರ್ಣಾಙ್ಕ) i_;
    ಯದಿ (unlikely (i > lenM1)) ಪ್ರತ್ಯಾಗಮ Null (Type);
    hb_barrier ();
    ಪ್ರತ್ಯಾಗಮ arrayZ[i];
  }
  Type& operator [] (ಪೂರ್ಣಾಙ್ಕ i_)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = (unsigned ಪೂರ್ಣಾಙ್ಕ) i_;
    ಯದಿ (unlikely (i > lenM1)) ಪ್ರತ್ಯಾಗಮ Crap (Type);
    hb_barrier ();
    ಪ್ರತ್ಯಾಗಮ arrayZ[i];
  }
  size_t get_size () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_unsigned_mul_add_saturate (lenM1 + 1, Type::static_size, lenM1.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!sanitize_shallow (c))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!ಪರಿಮಾಣ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ಸತ್ಯ);
    hb_barrier ();
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = lenM1 + 1;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  ಗುಪ್ತ:
  ತಾರ್ಕಿಕ sanitize_shallow (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (lenM1.sanitize (c) &&
		  hb_barrier () &&
		  (c->check_array_sized (arrayZ, lenM1 + 1, ಪರಿಮಾಣ (LenType))));
  }

  ಸಾರ್ವಜನಿಕ:
  LenType	lenM1;
  Type		arrayZ[HB_VAR_ARRAY];
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (ಪರಿಮಾಣ (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type, typename LenType>
ಸಂರಚನಾ SortedArrayOf : ArrayOf<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { ಪ್ರತ್ಯಾಗಮ hb_sorted_array (ಏತತ್->arrayZ, ಏತತ್->len); }
  hb_sorted_array_t<ಧ್ರುವ Type> as_array () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_sorted_array (ಏತತ್->arrayZ, ಏತತ್->len); }

  /* Iterator. */
  ಪ್ರಕಾರನಿರ್ದೇಶ hb_sorted_array_t<ಧ್ರುವ Type>   iter_t;
  ಪ್ರಕಾರನಿರ್ದೇಶ hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ as_array (); }
  writer_t writer ()       { ಪ್ರತ್ಯಾಗಮ as_array (); }
  operator   iter_t () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ   iter (); }
  operator writer_t ()       { ಪ್ರತ್ಯಾಗಮ writer (); }

  /* Faster range-based for loop. */
  ಧ್ರುವ Type *begin () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್->arrayZ; }
  ಧ್ರುವ Type *end () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್->arrayZ + ಏತತ್->len; }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ ವಸ್ತುಗಳು_len)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ತಾರ್ಕಿಕ ret = ArrayOf<Type, LenType>::serialize (c, ವಸ್ತುಗಳು_len);
    return_trace (ret);
  }
  template <typename Iterator,
	    hb_requires (hb_is_sorted_source_of (Iterator, Type))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, Iterator ವಸ್ತುಗಳು)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ತಾರ್ಕಿಕ ret = ArrayOf<Type, LenType>::serialize (c, ವಸ್ತುಗಳು);
    return_trace (ret);
  }

  SortedArrayOf* copy (hb_serialize_context_t *c) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    SortedArrayOf* out = reinterpret_cast<SortedArrayOf *> (ArrayOf<Type, LenType>::copy (c));
    return_trace (out);
  }

  template <typename T>
  Type &bsearch (ಧ್ರುವ T &x, Type &not_found = Crap (Type))
  { ಪ್ರತ್ಯಾಗಮ *as_array ().bsearch (x, &not_found); }
  template <typename T>
  ಧ್ರುವ Type &bsearch (ಧ್ರುವ T &x, ಧ್ರುವ Type &not_found = Null (Type)) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ *as_array ().bsearch (x, &not_found); }
  template <typename T>
  ತಾರ್ಕಿಕ bfind (ಧ್ರುವ T &x, unsigned ಪೂರ್ಣಾಙ್ಕ *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ಪೂರ್ಣಾಙ್ಕ to_store = (unsigned ಪೂರ್ಣಾಙ್ಕ) -1) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type> using SortedArray16Of = SortedArrayOf<Type, HBUINT16>;
template <typename Type> using SortedArray24Of = SortedArrayOf<Type, HBUINT24>;
template <typename Type> using SortedArray32Of = SortedArrayOf<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

template <typename LenType=HBUINT16>
ಸಂರಚನಾ Bin_ಹುಡುಕು_Header
{
  operator uint32_t () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ len; }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  Bin_ಹುಡುಕು_Header& operator = (unsigned ಪೂರ್ಣಾಙ್ಕ v)
  {
    len = v;
    ಪ್ರತಿಜ್ಞಾ (len == v);
    entrySelector = hb_max (1u, hb_bit_storage (v)) - 1;
    ಹುಡುಕು_Range = 16 * (1u << entrySelector);
    rangeShift = v * 16 > ಹುಡುಕು_Range
		 ? 16 * v - ಹುಡುಕು_Range
		 : 0;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  ರಕ್ಷಿತ:
  LenType	len;
  LenType	ಹುಡುಕು_Range;
  LenType	entrySelector;
  LenType	rangeShift;

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

template <typename Type, typename LenType=HBUINT16>
using Bin_ಹುಡುಕು_Array_Of = SortedArrayOf<Type, Bin_ಹುಡುಕು_Header<LenType>>;


ಸಂರಚನಾ Var_Sized_Bin_ಹುಡುಕು_Header
{

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  HBUINT16	unitSize;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	ಹುಡುಕು_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	entrySelector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	rangeShift;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (10);
};

template <typename Type>
ಸಂರಚನಾ Var_Sized_Bin_ಹುಡುಕು_Array_Of
{
  ಸ್ಥಿರ constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (Var_Sized_Bin_ಹುಡುಕು_Array_Of);

  ತಾರ್ಕಿಕ last_is_terminator () ಧ್ರುವ
  {
    ಯದಿ (unlikely (!header.nUnits)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    ಧ್ರುವ HBUINT16 *ಪದಗಳು = &StructAtOffset<HBUINT16> (&bytesZ, (header.nUnits - 1) * header.unitSize);
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = Type::Termination_Word_ಎಣಿಕೆ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (ಪದಗಳು[i] != 0xFFFFu)
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಧ್ರುವ Type& operator [] (ಪೂರ್ಣಾಙ್ಕ i_) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = (unsigned ಪೂರ್ಣಾಙ್ಕ) i_;
    ಯದಿ (unlikely (i >= get_length ())) ಪ್ರತ್ಯಾಗಮ Null (Type);
    hb_barrier ();
    ಪ್ರತ್ಯಾಗಮ StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  Type& operator [] (ಪೂರ್ಣಾಙ್ಕ i_)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = (unsigned ಪೂರ್ಣಾಙ್ಕ) i_;
    ಯದಿ (unlikely (i >= get_length ())) ಪ್ರತ್ಯಾಗಮ Crap (Type);
    hb_barrier ();
    ಪ್ರತ್ಯಾಗಮ StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_length () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ header.nUnits - last_is_terminator (); }
  size_t get_size () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_unsigned_mul_add_saturate (header.nUnits, header.unitSize, header.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!sanitize_shallow (c))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!ಪರಿಮಾಣ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ಸತ್ಯ);
    hb_barrier ();
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = get_length ();
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (unlikely (!(*ಏತತ್)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  template <typename T>
  ಧ್ರುವ Type *bsearch (ಧ್ರುವ T &key) ಧ್ರುವ
  {
    unsigned pos;
    ಪ್ರತ್ಯಾಗಮ hb_bsearch_impl (&pos,
			    key,
			    (ಧ್ರುವ ಶೂನ್ಯ *) bytesZ,
			    get_length (),
			    header.unitSize,
			    _hb_cmp_method<T, Type>)
	   ? (ಧ್ರುವ Type *) (((ಧ್ರುವ ವರ್ಣ *) &bytesZ) + (pos * header.unitSize))
	   : nullptr;
  }

  ಗುಪ್ತ:
  ತಾರ್ಕಿಕ sanitize_shallow (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (header.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_size <= header.unitSize &&
		  c->check_range (bytesZ.arrayZ,
				  header.nUnits,
				  header.unitSize));
  }

  ರಕ್ಷಿತ:
  Var_Sized_Bin_ಹುಡುಕು_Header	header;
  UnsizedArrayOf<HBUINT8>	bytesZ;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (10, bytesZ);
};


/* CFF INDEX */

template <typename ಎಣಿಕೆ_3>
ಸಂರಚನಾ CFFIndex
{
  unsigned ಪೂರ್ಣಾಙ್ಕ offset_array_size () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ offSize * (ಎಣಿಕೆ + 1); }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ Iterable &iterable,
		  ಧ್ರುವ unsigned *p_data_size = nullptr,
                  unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (ಏತತ್);
    unsigned data_size;
    ಯದಿ (p_data_size)
      data_size = *p_data_size;
    ಅನ್ಯಥಾ
      total_size (iterable, &data_size);

    auto it = hb_iter (iterable);
    ಯದಿ (unlikely (!serialize_header (c, +it, data_size, min_off_size))) return_trace (ಅಸತ್ಯ);
    unsigned ವರ್ಣ *ret = c->allocate_size<unsigned ವರ್ಣ> (data_size, ಅಸತ್ಯ);
    ಯದಿ (unlikely (!ret)) return_trace (ಅಸತ್ಯ);
    unsigned remaining = data_size;
    ಪ್ರತಿ (ಧ್ರುವ auto &_ : +it)
    {
      unsigned len = _.length;

      ಯದಿ (!len)
	ಅನುವರ್ತ;

      ಯದಿ (unlikely (len > remaining)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (ಅಸತ್ಯ, HB_SERIALIZE_ERROR_INT_OVERFLOW));
      }

      remaining -= len;

      ಯದಿ (len <= 1)
      {
	*ret++ = *_.arrayZ;
	ಅನುವರ್ತ;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (ಸತ್ಯ);
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  ತಾರ್ಕಿಕ serialize_header (hb_serialize_context_t *c,
			 Iterator it,
			 unsigned data_size,
                         unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (ಏತತ್);

    unsigned off_size = (hb_bit_storage (data_size + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    /* serialize CFFIndex header */
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    ಏತತ್->ಎಣಿಕೆ = hb_len (it);
    ಯದಿ (!ಏತತ್->ಎಣಿಕೆ) return_trace (ಸತ್ಯ);
    ಯದಿ (unlikely (!c->extend (ಏತತ್->offSize))) return_trace (ಅಸತ್ಯ);
    ಏತತ್->offSize = off_size;
    ಯದಿ (unlikely (!c->allocate_size<HBUINT8> (off_size * (ಏತತ್->ಎಣಿಕೆ + 1), ಅಸತ್ಯ)))
      return_trace (ಅಸತ್ಯ);

    /* serialize indices */
    unsigned ಪೂರ್ಣಾಙ್ಕ offset = 1;
    ಯದಿ (HB_OPTIMIZE_SIZE_VAL)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ i = 0;
      ಪ್ರತಿ (ಧ್ರುವ auto &_ : +it)
      {
	set_offset_at (i++, offset);
	offset += hb_len_of (_);
      }
      set_offset_at (i, offset);
    }
    ಅನ್ಯಥಾ
      ವಿಕಲ್ಪ (off_size)
      {
	ಪಕ್ಷ 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  ಪ್ರತಿ (ಧ್ರುವ auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	ಭಙ್ಗ;
	ಪಕ್ಷ 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  ಪ್ರತಿ (ಧ್ರುವ auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	ಭಙ್ಗ;
	ಪಕ್ಷ 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  ಪ್ರತಿ (ಧ್ರುವ auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	ಭಙ್ಗ;
	ಪಕ್ಷ 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  ಪ್ರತಿ (ಧ್ರುವ auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	ಭಙ್ಗ;
	ಸ್ವಭಾವ:
	ಭಙ್ಗ;
      }

    ಪ್ರತಿಜ್ಞಾ (offset == data_size + 1);
    return_trace (ಸತ್ಯ);
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  ಸ್ಥಿರ unsigned total_size (ಧ್ರುವ Iterable &iterable, unsigned *data_size = nullptr, unsigned min_off_size = 0)
  {
    auto it = + hb_iter (iterable);
    ಯದಿ (!it)
    {
      ಯದಿ (data_size) *data_size = 0;
      ಪ್ರತ್ಯಾಗಮ min_size;
    }

    unsigned total = 0;
    ಪ್ರತಿ (ಧ್ರುವ auto &_ : +it)
      total += hb_len_of (_);

    ಯದಿ (data_size) *data_size = total;

    unsigned off_size = (hb_bit_storage (total + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    ಪ್ರತ್ಯಾಗಮ min_size + HBUINT8::static_size + (hb_len (it) + 1) * off_size + total;
  }

  ಶೂನ್ಯ set_offset_at (unsigned ಪೂರ್ಣಾಙ್ಕ index, unsigned ಪೂರ್ಣಾಙ್ಕ offset)
  {
    ಪ್ರತಿಜ್ಞಾ (index <= ಎಣಿಕೆ);

    unsigned ಪೂರ್ಣಾಙ್ಕ size = offSize;
    ಧ್ರುವ HBUINT8 *p = offsets;
    ವಿಕಲ್ಪ (size)
    {
      ಪಕ್ಷ 1: ((HBUINT8  *) p)[index] = offset; ಭಙ್ಗ;
      ಪಕ್ಷ 2: ((HBUINT16 *) p)[index] = offset; ಭಙ್ಗ;
      ಪಕ್ಷ 3: ((HBUINT24 *) p)[index] = offset; ಭಙ್ಗ;
      ಪಕ್ಷ 4: ((HBUINT32 *) p)[index] = offset; ಭಙ್ಗ;
      ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ;
    }
  }

  ಗುಪ್ತ:
  unsigned ಪೂರ್ಣಾಙ್ಕ offset_at (unsigned ಪೂರ್ಣಾಙ್ಕ index) ಧ್ರುವ
  {
    ಪ್ರತಿಜ್ಞಾ (index <= ಎಣಿಕೆ);

    unsigned ಪೂರ್ಣಾಙ್ಕ size = offSize;
    ಧ್ರುವ HBUINT8 *p = offsets;
    ವಿಕಲ್ಪ (size)
    {
      ಪಕ್ಷ 1: ಪ್ರತ್ಯಾಗಮ ((HBUINT8  *) p)[index];
      ಪಕ್ಷ 2: ಪ್ರತ್ಯಾಗಮ ((HBUINT16 *) p)[index];
      ಪಕ್ಷ 3: ಪ್ರತ್ಯಾಗಮ ((HBUINT24 *) p)[index];
      ಪಕ್ಷ 4: ಪ್ರತ್ಯಾಗಮ ((HBUINT32 *) p)[index];
      ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  ಧ್ರುವ unsigned ವರ್ಣ *data_base () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಧ್ರುವ unsigned ವರ್ಣ *) ಏತತ್ + min_size + offSize.static_size - 1 + offset_array_size (); }
  ಸಾರ್ವಜನಿಕ:

  hb_ubytes_t operator [] (unsigned ಪೂರ್ಣಾಙ್ಕ index) ಧ್ರುವ
  {
    ಯದಿ (unlikely (index >= ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ hb_ubytes_t ();
    hb_barrier ();
    unsigned offset0 = offset_at (index);
    unsigned offset1 = offset_at (index + 1);
    ಯದಿ (unlikely (offset1 < offset0 || offset1 > offset_at (ಎಣಿಕೆ)))
      ಪ್ರತ್ಯಾಗಮ hb_ubytes_t ();
    ಪ್ರತ್ಯಾಗಮ hb_ubytes_t (data_base () + offset0, offset1 - offset0);
  }

  size_t get_size () ಧ್ರುವ
  {
    ಯದಿ (ಎಣಿಕೆ)
      ಪ್ರತ್ಯಾಗಮ hb_unsigned_add_saturate (min_size, offSize.static_size,
				       offset_array_size (), offset_at (ಎಣಿಕೆ) - 1);
    ಪ್ರತ್ಯಾಗಮ min_size;  /* empty CFFIndex contains count only */
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  hb_barrier () &&
			  (ಎಣಿಕೆ == 0 || /* empty INDEX */
			   (ಎಣಿಕೆ < ಎಣಿಕೆ + 1u &&
			    c->check_struct (&offSize) && offSize >= 1 && offSize <= 4 &&
			    c->check_array (offsets, offSize, ಎಣಿಕೆ + 1u) &&
			    c->check_range (data_base (), offset_at (ಎಣಿಕೆ))))));
  }

  ಸಾರ್ವಜನಿಕ:
  ಎಣಿಕೆ_3		ಎಣಿಕೆ;		/* Number of object data. Note there are (count+1) offsets */
  ಗುಪ್ತ:
  HBUINT8	offSize;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (ಎಣಿಕೆ_3::static_size);
};
ಪ್ರಕಾರನಿರ್ದೇಶ CFFIndex<HBUINT16> CFF1Index;
ಪ್ರಕಾರನಿರ್ದೇಶ CFFIndex<HBUINT32> CFF2Index;


/* TupleValues */
ಸಂರಚನಾ Tuple_ಮೌಲ್ಯಗಳು
{
  ಗಣನಾ packed_ಮೌಲ್ಯ_flag_t
  {
    ಮೌಲ್ಯಗಳು_ARE_ZEROS     = 0x80,
    ಮೌಲ್ಯಗಳು_ARE_BYTES     = 0x00,
    ಮೌಲ್ಯಗಳು_ARE_ಪದಗಳು     = 0x40,
    ಮೌಲ್ಯಗಳು_ARE_LONGS     = 0xC0,
    ಮೌಲ್ಯಗಳು_SIZE_MASK     = 0xC0,
    ಮೌಲ್ಯ_RUN_ಎಣಿಕೆ_MASK = 0x3F
  };

  ಸ್ಥಿರ unsigned compile_unsafe (hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> values, /* IN */
				  unsigned ವರ್ಣ *encoded_bytes /* OUT */)
  {
    unsigned ಅಂಕೆ_ಮೌಲ್ಯಗಳು_3 = values.length;
    unsigned encoded_len = 0;
    unsigned i = 0;
    ಯಾವತ್ (i < ಅಂಕೆ_ಮೌಲ್ಯಗಳು_3)
    {
      ಪೂರ್ಣಾಙ್ಕ val = values.arrayZ[i];
      ಯದಿ (val == 0)
        encoded_len += encode_ಮೌಲ್ಯ_run_as_zeroes (i, encoded_bytes + encoded_len, values);
      ಅನ್ಯಥಾ ಯದಿ ((int8_t) val == val)
        encoded_len += encode_ಮೌಲ್ಯ_run_as_bytes (i, encoded_bytes + encoded_len, values);
      ಅನ್ಯಥಾ ಯದಿ ((int16_t) val == val)
        encoded_len += encode_ಮೌಲ್ಯ_run_as_ಪದಗಳು (i, encoded_bytes + encoded_len, values);
      ಅನ್ಯಥಾ
        encoded_len += encode_ಮೌಲ್ಯ_run_as_longs (i, encoded_bytes + encoded_len, values);
    }
    ಪ್ರತ್ಯಾಗಮ encoded_len;
  }

  ಸ್ಥಿರ unsigned encode_ಮೌಲ್ಯ_run_as_zeroes (unsigned& i,
					      unsigned ವರ್ಣ *it,
					      hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> values)
  {
    unsigned ಅಂಕೆ_ಮೌಲ್ಯಗಳು_3 = values.length;
    unsigned run_length = 0;
    unsigned encoded_len = 0;
    ಯಾವತ್ (i < ಅಂಕೆ_ಮೌಲ್ಯಗಳು_3 && values.arrayZ[i] == 0)
    {
      i++;
      run_length++;
    }

    ಯಾವತ್ (run_length >= 64)
    {
      *it++ = ವರ್ಣ (ಮೌಲ್ಯಗಳು_ARE_ZEROS | 63);
      run_length -= 64;
      encoded_len++;
    }

    ಯದಿ (run_length)
    {
      *it++ = ವರ್ಣ (ಮೌಲ್ಯಗಳು_ARE_ZEROS | (run_length - 1));
      encoded_len++;
    }
    ಪ್ರತ್ಯಾಗಮ encoded_len;
  }

  ಸ್ಥಿರ unsigned encode_ಮೌಲ್ಯ_run_as_bytes (unsigned &i,
					     unsigned ವರ್ಣ *it,
					     hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> values)
  {
    unsigned start = i;
    unsigned ಅಂಕೆ_ಮೌಲ್ಯಗಳು_3 = values.length;
    ಯಾವತ್ (i < ಅಂಕೆ_ಮೌಲ್ಯಗಳು_3)
    {
      ಪೂರ್ಣಾಙ್ಕ val = values.arrayZ[i];
      ಯದಿ ((int8_t) val != val)
        ಭಙ್ಗ;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      ಯದಿ (val == 0 && i + 1 < ಅಂಕೆ_ಮೌಲ್ಯಗಳು_3 && values.arrayZ[i+1] == 0)
        ಭಙ್ಗ;

      i++;
    }
    unsigned run_length = i - start;

    unsigned encoded_len = 0;

    ಯಾವತ್ (run_length >= 64)
    {
      *it++ = (ಮೌಲ್ಯಗಳು_ARE_BYTES | 63);
      encoded_len++;

      ಪ್ರತಿ (unsigned j = 0; j < 64; j++)
	it[j] = static_cast<ವರ್ಣ> (values.arrayZ[start + j]);
      it += 64;
      encoded_len += 64;

      start += 64;
      run_length -= 64;
    }

    ಯದಿ (run_length)
    {
      *it++ = (ಮೌಲ್ಯಗಳು_ARE_BYTES | (run_length - 1));
      encoded_len++;

      ಪ್ರತಿ (unsigned j = 0; j < run_length; j++)
        it[j] = static_cast<ವರ್ಣ> (values.arrayZ[start + j]);
      encoded_len += run_length;
    }

    ಪ್ರತ್ಯಾಗಮ encoded_len;
  }

  ಸ್ಥಿರ unsigned encode_ಮೌಲ್ಯ_run_as_ಪದಗಳು (unsigned &i,
					     unsigned ವರ್ಣ *it,
					     hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> values)
  {
    unsigned start = i;
    unsigned ಅಂಕೆ_ಮೌಲ್ಯಗಳು_3 = values.length;
    ಯಾವತ್ (i < ಅಂಕೆ_ಮೌಲ್ಯಗಳು_3)
    {
      ಪೂರ್ಣಾಙ್ಕ val = values.arrayZ[i];

      ಯದಿ ((int16_t) val != val)
        ಭಙ್ಗ;

      /* start a new run for a single zero value. */
      ಯದಿ (val == 0) ಭಙ್ಗ;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      ಯದಿ ((int8_t) val == val &&
          i + 1 < ಅಂಕೆ_ಮೌಲ್ಯಗಳು_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        ಭಙ್ಗ;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    ಯಾವತ್ (run_length >= 64)
    {
      *it++ = (ಮೌಲ್ಯಗಳು_ARE_ಪದಗಳು | 63);
      encoded_len++;

      ಪ್ರತಿ (unsigned j = 0; j < 64; j++)
      {
        int16_t ಮೌಲ್ಯ_val = values.arrayZ[start + j];
        *it++ = static_cast<ವರ್ಣ> (ಮೌಲ್ಯ_val >> 8);
        *it++ = static_cast<ವರ್ಣ> (ಮೌಲ್ಯ_val & 0xFF);

        encoded_len += 2;
      }

      start += 64;
      run_length -= 64;
    }

    ಯದಿ (run_length)
    {
      *it++ = (ಮೌಲ್ಯಗಳು_ARE_ಪದಗಳು | (run_length - 1));
      encoded_len++;
      ಯಾವತ್ (start < i)
      {
        int16_t ಮೌಲ್ಯ_val = values.arrayZ[start++];
        *it++ = static_cast<ವರ್ಣ> (ಮೌಲ್ಯ_val >> 8);
        *it++ = static_cast<ವರ್ಣ> (ಮೌಲ್ಯ_val & 0xFF);

        encoded_len += 2;
      }
    }
    ಪ್ರತ್ಯಾಗಮ encoded_len;
  }

  ಸ್ಥಿರ unsigned encode_ಮೌಲ್ಯ_run_as_longs (unsigned &i,
					     unsigned ವರ್ಣ *it,
					     hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> values)
  {
    unsigned start = i;
    unsigned ಅಂಕೆ_ಮೌಲ್ಯಗಳು_3 = values.length;
    ಯಾವತ್ (i < ಅಂಕೆ_ಮೌಲ್ಯಗಳು_3)
    {
      ಪೂರ್ಣಾಙ್ಕ val = values.arrayZ[i];

      ಯದಿ ((int16_t) val == val)
        ಭಙ್ಗ;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    ಯಾವತ್ (run_length >= 64)
    {
      *it++ = (ಮೌಲ್ಯಗಳು_ARE_LONGS | 63);
      encoded_len++;

      ಪ್ರತಿ (unsigned j = 0; j < 64; j++)
      {
        int32_t ಮೌಲ್ಯ_val = values.arrayZ[start + j];
        *it++ = static_cast<ವರ್ಣ> (ಮೌಲ್ಯ_val >> 24);
        *it++ = static_cast<ವರ್ಣ> (ಮೌಲ್ಯ_val >> 16);
        *it++ = static_cast<ವರ್ಣ> (ಮೌಲ್ಯ_val >> 8);
        *it++ = static_cast<ವರ್ಣ> (ಮೌಲ್ಯ_val & 0xFF);

        encoded_len += 4;
      }

      start += 64;
      run_length -= 64;
    }

    ಯದಿ (run_length)
    {
      *it++ = (ಮೌಲ್ಯಗಳು_ARE_LONGS | (run_length - 1));
      encoded_len++;
      ಯಾವತ್ (start < i)
      {
        int32_t ಮೌಲ್ಯ_val = values.arrayZ[start++];
        *it++ = static_cast<ವರ್ಣ> (ಮೌಲ್ಯ_val >> 24);
        *it++ = static_cast<ವರ್ಣ> (ಮೌಲ್ಯ_val >> 16);
        *it++ = static_cast<ವರ್ಣ> (ಮೌಲ್ಯ_val >> 8);
        *it++ = static_cast<ವರ್ಣ> (ಮೌಲ್ಯ_val & 0xFF);

        encoded_len += 4;
      }
    }
    ಪ್ರತ್ಯಾಗಮ encoded_len;
  }

  template <typename T>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ಸ್ಥಿರ ತಾರ್ಕಿಕ decompile (ಧ್ರುವ HBUINT8 *&p /* IN/OUT */,
			 hb_vector_t<T> &values /* IN/OUT */,
			 ಧ್ರುವ HBUINT8 *end,
			 ತಾರ್ಕಿಕ consume_all = ಅಸತ್ಯ,
			 unsigned start = 0)
  {
    unsigned i = 0;
    unsigned ಎಣಿಕೆ = consume_all ? UINT_MAX : values.length;
    ಯದಿ (consume_all)
      values.alloc ((end - p) / 2);
    ಯಾವತ್ (i < ಎಣಿಕೆ)
    {
      ಯದಿ (unlikely (p + 1 > end)) ಪ್ರತ್ಯಾಗಮ consume_all;
      unsigned control = *p++;
      unsigned run_ಎಣಿಕೆ = (control & ಮೌಲ್ಯ_RUN_ಎಣಿಕೆ_MASK) + 1;
      ಯದಿ (consume_all)
      {
        ಯದಿ (unlikely (!values.resize_dirty  (values.length + run_ಎಣಿಕೆ)))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
      unsigned stop = i + run_ಎಣಿಕೆ;
      ಯದಿ (unlikely (stop > ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      unsigned skip = i < start ? hb_min (start - i, run_ಎಣಿಕೆ) : 0;
      i += skip;

      ಯದಿ ((control & ಮೌಲ್ಯಗಳು_SIZE_MASK) == ಮೌಲ್ಯಗಳು_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * ಪರಿಮಾಣ (T));
	i = stop;
      }
      ಅನ್ಯಥಾ ಯದಿ ((control & ಮೌಲ್ಯಗಳು_SIZE_MASK) ==  ಮೌಲ್ಯಗಳು_ARE_ಪದಗಳು)
      {
        ಯದಿ (unlikely (p + run_ಎಣಿಕೆ * HBINT16::static_size > end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	p += skip * HBINT16::static_size;
#ifndef HB_OPTIMIZE_SIZE
        ಪ್ರತಿ (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ಧ್ರುವ HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 1] = * (ಧ್ರುವ HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 2] = * (ಧ್ರುವ HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 3] = * (ಧ್ರುವ HBINT16 *) p;
	  p += HBINT16::static_size;
	}
#endif
        ಪ್ರತಿ (; i < stop; i++)
        {
          values.arrayZ[i] = * (ಧ್ರುವ HBINT16 *) p;
          p += HBINT16::static_size;
        }
      }
      ಅನ್ಯಥಾ ಯದಿ ((control & ಮೌಲ್ಯಗಳು_SIZE_MASK) ==  ಮೌಲ್ಯಗಳು_ARE_LONGS)
      {
        ಯದಿ (unlikely (p + run_ಎಣಿಕೆ * HBINT32::static_size > end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	p += skip * HBINT32::static_size;
        ಪ್ರತಿ (; i < stop; i++)
        {
          values.arrayZ[i] = * (ಧ್ರುವ HBINT32 *) p;
          p += HBINT32::static_size;
        }
      }
      ಅನ್ಯಥಾ ಯದಿ ((control & ಮೌಲ್ಯಗಳು_SIZE_MASK) ==  ಮೌಲ್ಯಗಳು_ARE_BYTES)
      {
        ಯದಿ (unlikely (p + run_ಎಣಿಕೆ > end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	p += skip * HBINT8::static_size;
#ifndef HB_OPTIMIZE_SIZE
	ಪ್ರತಿ (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ಧ್ರುವ HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (ಧ್ರುವ HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (ಧ್ರುವ HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (ಧ್ರುವ HBINT8 *) p++;
	}
#endif
        ಪ್ರತಿ (; i < stop; i++)
          values.arrayZ[i] = * (ಧ್ರುವ HBINT8 *) p++;
      }
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ skip (ಧ್ರುವ unsigned ವರ್ಣ *&p /* IN/OUT */,
		    ಧ್ರುವ unsigned ವರ್ಣ *end,
		    unsigned ಎಣಿಕೆ)
  {
    unsigned seen = 0;
    ಯಾವತ್ (seen < ಎಣಿಕೆ)
    {
      ಯದಿ (unlikely (p >= end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      unsigned control = *p++;
      unsigned run_ಎಣಿಕೆ = (control & ಮೌಲ್ಯ_RUN_ಎಣಿಕೆ_MASK) + 1;
      ಯದಿ (unlikely (run_ಎಣಿಕೆ > ಎಣಿಕೆ - seen)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      unsigned width = 0;
      ವಿಕಲ್ಪ (control & ಮೌಲ್ಯಗಳು_SIZE_MASK)
      {
	ಪಕ್ಷ ಮೌಲ್ಯಗಳು_ARE_ZEROS: ಭಙ್ಗ;
	ಪಕ್ಷ ಮೌಲ್ಯಗಳು_ARE_BYTES: width = HBINT8::static_size; ಭಙ್ಗ;
	ಪಕ್ಷ ಮೌಲ್ಯಗಳು_ARE_ಪದಗಳು: width = HBINT16::static_size; ಭಙ್ಗ;
	ಪಕ್ಷ ಮೌಲ್ಯಗಳು_ARE_LONGS: width = HBINT32::static_size; ಭಙ್ಗ;
      }
      ಯದಿ (unlikely (unsigned (end - p) < run_ಎಣಿಕೆ * width)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      p += run_ಎಣಿಕೆ * width;
      seen += run_ಎಣಿಕೆ;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಸಂರಚನಾ iter_t : hb_iter_with_fallback_t<iter_t, ಪೂರ್ಣಾಙ್ಕ>
  {
    iter_t (ಧ್ರುವ unsigned ವರ್ಣ *p_, unsigned len_)
	    : p (p_), endp (p_ + len_)
    { ಯದಿ (likely (ensure_run ())) read_ಮೌಲ್ಯ (); }

    ಗುಪ್ತ:
    ಧ್ರುವ unsigned ವರ್ಣ *p;
    ಧ್ರುವ unsigned ವರ್ಣ * ಧ್ರುವ endp;
    ಪೂರ್ಣಾಙ್ಕ current_ಮೌಲ್ಯ = 0;
    signed run_ಎಣಿಕೆ = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    ತಾರ್ಕಿಕ ensure_run ()
    {
      ಯದಿ (likely (run_ಎಣಿಕೆ > 0)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ _ensure_run ();
    }
    ತಾರ್ಕಿಕ _ensure_run ()
    {
      ಯದಿ (unlikely (p >= endp))
      {
        run_ಎಣಿಕೆ = 0;
        current_ಮೌಲ್ಯ = 0;
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }

      unsigned control = *p++;
      run_ಎಣಿಕೆ = (control & ಮೌಲ್ಯ_RUN_ಎಣಿಕೆ_MASK) + 1;
      width = control & ಮೌಲ್ಯಗಳು_SIZE_MASK;
      ವಿಕಲ್ಪ (width)
      {
        ಪಕ್ಷ ಮೌಲ್ಯಗಳು_ARE_ZEROS: width = 0; ಭಙ್ಗ;
	ಪಕ್ಷ ಮೌಲ್ಯಗಳು_ARE_BYTES: width = HBINT8::static_size;  ಭಙ್ಗ;
	ಪಕ್ಷ ಮೌಲ್ಯಗಳು_ARE_ಪದಗಳು: width = HBINT16::static_size; ಭಙ್ಗ;
	ಪಕ್ಷ ಮೌಲ್ಯಗಳು_ARE_LONGS: width = HBINT32::static_size; ಭಙ್ಗ;
	ಸ್ವಭಾವ: ಪ್ರತಿಜ್ಞಾ (ಅಸತ್ಯ);
      }

      ಯದಿ (unlikely (p + run_ಎಣಿಕೆ * width > endp))
      {
	run_ಎಣಿಕೆ = 0;
	current_ಮೌಲ್ಯ = 0;
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }

      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಶೂನ್ಯ read_ಮೌಲ್ಯ ()
    {
      ವಿಕಲ್ಪ (width)
      {
        ಪಕ್ಷ 0: current_ಮೌಲ್ಯ = 0; ಭಙ್ಗ;
	ಪಕ್ಷ 1: current_ಮೌಲ್ಯ = * (ಧ್ರುವ HBINT8  *) p; ಭಙ್ಗ;
	ಪಕ್ಷ 2: current_ಮೌಲ್ಯ = * (ಧ್ರುವ HBINT16 *) p; ಭಙ್ಗ;
	ಪಕ್ಷ 4: current_ಮೌಲ್ಯ = * (ಧ್ರುವ HBINT32 *) p; ಭಙ್ಗ;
      }
      p += width;
    }

    ಸಾರ್ವಜನಿಕ:

    ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ __item_t__;
    __item_t__ __item__ () ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ current_ಮೌಲ್ಯ; }

    ತಾರ್ಕಿಕ __more__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ run_ಎಣಿಕೆ || p < endp; }
    ಶೂನ್ಯ __next__ ()
    {
      run_ಎಣಿಕೆ--;
      ಯದಿ (unlikely (!ensure_run ()))
	ಪ್ರತ್ಯಾಗಮ;
      read_ಮೌಲ್ಯ ();
    }
    ಶೂನ್ಯ __forward__ (unsigned n)
    {
      ಯದಿ (unlikely (!ensure_run ()))
	ಪ್ರತ್ಯಾಗಮ;
      ಯಾವತ್ (n)
      {
	unsigned i = hb_min (n, (unsigned) run_ಎಣಿಕೆ);
	run_ಎಣಿಕೆ -= i;
	n -= i;
	p += (i - 1) * width;
	ಯದಿ (unlikely (!ensure_run ()))
	  ಪ್ರತ್ಯಾಗಮ;
	read_ಮೌಲ್ಯ ();
      }
    }
    ತಾರ್ಕಿಕ operator != (ಧ್ರುವ iter_t& o) ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ p != o.p || run_ಎಣಿಕೆ != o.run_ಎಣಿಕೆ; }
    iter_t __end__ () ಧ್ರುವ
    {
      iter_t it (endp, 0);
      ಪ್ರತ್ಯಾಗಮ it;
    }
  };

  ಸಂರಚನಾ fetcher_t
  {
    fetcher_t (ಧ್ರುವ unsigned ವರ್ಣ *p_, unsigned len_)
	      : p (p_), end (p_ + len_) {}

    ಗುಪ್ತ:
    ಧ್ರುವ unsigned ವರ್ಣ *p;
    ಧ್ರುವ unsigned ವರ್ಣ * ಧ್ರುವ end;
    signed run_ಎಣಿಕೆ = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    ತಾರ್ಕಿಕ ensure_run ()
    {
      ಯದಿ (likely (run_ಎಣಿಕೆ > 0)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ _ensure_run ();
    }

    ತಾರ್ಕಿಕ _ensure_run ()
    {
      ಯದಿ (unlikely (p >= end))
      {
        run_ಎಣಿಕೆ = 0;
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }

      unsigned control = *p++;
      run_ಎಣಿಕೆ = (control & ಮೌಲ್ಯ_RUN_ಎಣಿಕೆ_MASK) + 1;
      width = control & ಮೌಲ್ಯಗಳು_SIZE_MASK;
      ವಿಕಲ್ಪ (width)
      {
        ಪಕ್ಷ ಮೌಲ್ಯಗಳು_ARE_ZEROS: width = 0; ಭಙ್ಗ;
	ಪಕ್ಷ ಮೌಲ್ಯಗಳು_ARE_BYTES: width = HBINT8::static_size;  ಭಙ್ಗ;
	ಪಕ್ಷ ಮೌಲ್ಯಗಳು_ARE_ಪದಗಳು: width = HBINT16::static_size; ಭಙ್ಗ;
	ಪಕ್ಷ ಮೌಲ್ಯಗಳು_ARE_LONGS: width = HBINT32::static_size; ಭಙ್ಗ;
	ಸ್ವಭಾವ: ಪ್ರತಿಜ್ಞಾ (ಅಸತ್ಯ);
      }

      ಯದಿ (unlikely (p + run_ಎಣಿಕೆ * width > end))
      {
	run_ಎಣಿಕೆ = 0;
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }

      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ಸಾರ್ವಜನಿಕ:
    ಶೂನ್ಯ skip (unsigned n)
    {
      ಯಾವತ್ (n)
      {
	ಯದಿ (unlikely (!ensure_run ()))
	  ಪ್ರತ್ಯಾಗಮ;
	unsigned i = hb_min (n, (unsigned) run_ಎಣಿಕೆ);
	run_ಎಣಿಕೆ -= i;
	n -= i;
	p += i * width;
      }
    }

    ಗುಪ್ತ:
    template <ತಾರ್ಕಿಕ scaled, typename T>
    ಶೂನ್ಯ _add_to (hb_array_t<T> out, T scale = T (1))
    {
      unsigned n = out.length;
      T *arrayZ = out.arrayZ;

      ಪ್ರತಿ (unsigned i = 0; i < n;)
      {
	ಯದಿ (unlikely (!ensure_run ()))
	  ಭಙ್ಗ;
	unsigned ಎಣಿಕೆ = hb_min (n - i, (unsigned) run_ಎಣಿಕೆ);
	ವಿಕಲ್ಪ (width)
	{
	  ಪಕ್ಷ 0:
	  {
	    arrayZ += ಎಣಿಕೆ;
	    ಭಙ್ಗ;
	  }
	  ಪಕ್ಷ 1:
	  {
	    ಧ್ರುವ auto *pp = (ಧ್ರುವ HBINT8 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    ಪ್ರತಿ (; j + 3 < ಎಣಿಕೆ; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    ಪ್ರತಿ (; j < ಎಣಿಕೆ; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ಧ್ರುವ unsigned ವರ್ಣ *) pp;
	  }
	  ಭಙ್ಗ;
	  ಪಕ್ಷ 2:
	  {
	    ಧ್ರುವ auto *pp = (ಧ್ರುವ HBINT16 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    ಪ್ರತಿ (; j + 3 < ಎಣಿಕೆ; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    ಪ್ರತಿ (; j < ಎಣಿಕೆ; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ಧ್ರುವ unsigned ವರ್ಣ *) pp;
	  }
	  ಭಙ್ಗ;
	  ಪಕ್ಷ 4:
	  {
	    ಧ್ರುವ auto *pp = (ಧ್ರುವ HBINT32 *) p;
	    ಪ್ರತಿ (unsigned j = 0; j < ಎಣಿಕೆ; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ಧ್ರುವ unsigned ವರ್ಣ *) pp;
	  }
	  ಭಙ್ಗ;
	}
	run_ಎಣಿಕೆ -= ಎಣಿಕೆ;
	i += ಎಣಿಕೆ;
      }
    }

    ಸಾರ್ವಜನಿಕ:
    template <typename T>
    ಶೂನ್ಯ add_to (hb_array_t<T> out, ಭಿನ್ನ scale = 1.0f)
    {
#ifndef HB_OPTIMIZE_SIZE
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      ಯದಿ (ಅಸತ್ಯ && scale == 1.0f)
        _add_to<ಅಸತ್ಯ> (out);
      ಅನ್ಯಥಾ
#endif
        _add_to<ಸತ್ಯ> (out, (T) scale);
    }
  };
};

ಸಂರಚನಾ TupleList : CFF2Index
{
  Tuple_ಮೌಲ್ಯಗಳು::iter_t operator [] (unsigned i) ಧ್ರುವ
  {
    auto bytes = CFF2Index::operator [] (i);
    ಪ್ರತ್ಯಾಗಮ Tuple_ಮೌಲ್ಯಗಳು::iter_t (bytes.arrayZ, bytes.length);
  }

  Tuple_ಮೌಲ್ಯಗಳು::fetcher_t fetcher (unsigned i) ಧ್ರುವ
  {
    auto bytes = CFF2Index::operator [] (i);
    ಪ್ರತ್ಯಾಗಮ Tuple_ಮೌಲ್ಯಗಳು::fetcher_t (bytes.arrayZ, bytes.length);
  }
};


// Alignment

template <unsigned ಪೂರ್ಣಾಙ್ಕ alignment>
ಸಂರಚನಾ Align
{
  size_t get_size (ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    unsigned offset = (ಧ್ರುವ ವರ್ಣ *) ಏತತ್ - (ಧ್ರುವ ವರ್ಣ *) base;
    ಪ್ರತ್ಯಾಗಮ (alignment - offset) & (alignment - 1);
  }

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (0);
};



} /* namespace OT */


#endif /* HB_OPEN_TYPE_HH */
