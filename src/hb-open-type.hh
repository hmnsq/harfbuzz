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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPEN_TYPE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OPEN_TYPE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-blob.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-face.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-meta.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset.hh"


ਨਾਮਾਕਾਸ਼ OT {


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
ਸਾਂਚਾ <ਤਾਰਕਿਕ BE,
	  ਕਿਸਮ_ਨਾਂ Type,
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ_2 = ਆਕਾਰ (Type)>
ਬਣਤਰ ਅੰਕ_Type
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ std::conditional<std::is_integral<Type>::value && ਆਕਾਰ (Type) <= ਆਕਾਰ(ਪੂਰਨ),
				     ਕਿਸਮ_ਨਾਂ std::conditional<std::is_signed<Type>::value, ਨਿਸ਼ਾਨੀਵਾਲਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ>::type,
				     Type>::type WideType;

  ਅੰਕ_Type () = ਮੂਲ;
  ਸਪਸ਼ਟ constexpr ਅੰਕ_Type (Type V) : v {V} {}
  ਅੰਕ_Type& ਸੰਚਾਲਕ = (Type V) { v = V; ਵਾਪਸ *ਇਹ; }

  ਸੰਚਾਲਕ WideType () ਅਚਲ { ਵਾਪਸ v; }

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ ਅੰਕ_Type &o) ਅਚਲ { ਵਾਪਸ (Type) v == (Type) o.v; }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ ਅੰਕ_Type &o) ਅਚਲ { ਵਾਪਸ !(*ਇਹ == o); }

  ਅੰਕ_Type& ਸੰਚਾਲਕ += (WideType ਗਿਣਤੀ_2) { *ਇਹ = *ਇਹ + ਗਿਣਤੀ_2; ਵਾਪਸ *ਇਹ; }
  ਅੰਕ_Type& ਸੰਚਾਲਕ -= (WideType ਗਿਣਤੀ_2) { *ਇਹ = *ਇਹ - ਗਿਣਤੀ_2; ਵਾਪਸ *ਇਹ; }
  ਅੰਕ_Type& ਸੰਚਾਲਕ ++ () { *ਇਹ += 1; ਵਾਪਸ *ਇਹ; }
  ਅੰਕ_Type& ਸੰਚਾਲਕ -- () { *ਇਹ -= 1; ਵਾਪਸ *ਇਹ; }
  ਅੰਕ_Type ਸੰਚਾਲਕ ++ (ਪੂਰਨ) { ਅੰਕ_Type c (*ਇਹ); ++*ਇਹ; ਵਾਪਸ c; }
  ਅੰਕ_Type ਸੰਚਾਲਕ -- (ਪੂਰਨ) { ਅੰਕ_Type c (*ਇਹ); --*ਇਹ; ਵਾਪਸ c; }

  uint32_t hash () ਅਚਲ { ਵਾਪਸ hb_array ((ਅਚਲ ਅੱਖਰ *) &v, ਆਕਾਰ (v)).hash (); }
  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਅੰਕ_Type *a, ਅਚਲ ਅੰਕ_Type *b)
  { ਵਾਪਸ b->cmp (*a); }
  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
  {
    ਅੰਕ_Type *pa = (ਅੰਕ_Type *) a;
    ਅੰਕ_Type *pb = (ਅੰਕ_Type *) b;

    ਵਾਪਸ pb->cmp (*pa);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  ਪੂਰਨ cmp (Type2 a) ਅਚਲ
  {
    Type b = v;
    ਵਾਪਸ (a > b) - (a < b);
  }
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }
  ਸੁਰੱਖਿਅਤ:
  ਕਿਸਮ_ਨਾਂ std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, ਮਾਪ_2>,
			    HBFloat<BE, Type, ਮਾਪ_2>>::type v;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (ਮਾਪ_2);
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਸੱਚ, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਸੱਚ, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਸੱਚ, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਸੱਚ, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਸੱਚ, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਸੱਚ, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਸੱਚ, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਸੱਚ, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਸੱਚ, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਝੂਠ, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਝੂਠ, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਝੂਠ, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਝੂਠ, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਝੂਠ, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਝੂਠ, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਸੱਚ,  ਦਸ਼ਮਲਵ>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਸੱਚ,  ਦੁੱਗਣਾ> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਝੂਠ, ਦਸ਼ਮਲਵ>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੰਕ_Type<ਝੂਠ, ਦੁੱਗਣਾ> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
ਬਣਤਰ HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& ਸੰਚਾਲਕ = (uint16_t i ) { HBUINT16::ਸੰਚਾਲਕ= (i); ਵਾਪਸ *ਇਹ; }
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
ਬਣਤਰ HBUINT32VAR
{
  size_t get_ਮਾਪ () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ b0 = v[0];
    ਜੇ (b0 < 0x80)
      ਵਾਪਸ 1;
    ਨਹੀਂਤਾਂ ਜੇ (b0 < 0xC0)
      ਵਾਪਸ 2;
    ਨਹੀਂਤਾਂ ਜੇ (b0 < 0xE0)
      ਵਾਪਸ 3;
    ਨਹੀਂਤਾਂ ਜੇ (b0 < 0xF0)
      ਵਾਪਸ 4;
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ 5;
  }

  ਸਥਿਰ size_t get_ਮਾਪ (uint32_t v)
  {
    ਜੇ (v < 0x80)
      ਵਾਪਸ 1;
    ਨਹੀਂਤਾਂ ਜੇ (v < 0x4000)
      ਵਾਪਸ 2;
    ਨਹੀਂਤਾਂ ਜੇ (v < 0x200000)
      ਵਾਪਸ 3;
    ਨਹੀਂਤਾਂ ਜੇ (v < 0x10000000)
      ਵਾਪਸ 4;
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ 5;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_ਮਾਪ ()));
  }

  ਸੰਚਾਲਕ uint32_t () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ b0 = v[0];
    ਜੇ (b0 < 0x80)
      ਵਾਪਸ b0;
    ਨਹੀਂਤਾਂ ਜੇ (b0 < 0xC0)
      ਵਾਪਸ ((b0 & 0x3F) << 8) | v[1];
    ਨਹੀਂਤਾਂ ਜੇ (b0 < 0xE0)
      ਵਾਪਸ ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    ਨਹੀਂਤਾਂ ਜੇ (b0 < 0xF0)
      ਵਾਪਸ ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ serialize_unsafe (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *buf, uint32_t v)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ len = get_ਮਾਪ (v);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *p = buf + len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    ਜੇ (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    ਵਾਪਸ len;
  }

  ਸਥਿਰ ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, uint32_t v)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ len = get_ਮਾਪ (v);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *buf = c->allocate_ਮਾਪ<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> (len, ਝੂਠ);
    ਜੇ (unlikely (!buf))
      ਵਾਪਸ ਝੂਠ;

    serialize_unsafe (buf, v);
    ਵਾਪਸ ਸੱਚ;
  }

  ਸੁਰੱਖਿਅਤ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ v[5];

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ HBUINT16 UFWORD;

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਬਿਨਾਨਿਸ਼ਾਨੀ fraction_ਬਿੱਟ>
ਬਣਤਰ HBFixed : Type
{
  ਸਥਿਰ constexpr ਦਸ਼ਮਲਵ mult = 1.f / (1 << fraction_ਬਿੱਟ);
  static_assert (Type::static_ਮਾਪ * 8 > fraction_ਬਿੱਟ, "");

  ਸੰਚਾਲਕ ਨਿਸ਼ਾਨੀਵਾਲਾ () ਅਚਲ = ਮਿਟਾਓ;
  ਸੰਚਾਲਕ ਬਿਨਾਨਿਸ਼ਾਨੀ () ਅਚਲ = ਮਿਟਾਓ;
  ਸਪਸ਼ਟ ਸੰਚਾਲਕ ਦਸ਼ਮਲਵ () ਅਚਲ { ਵਾਪਸ ਮੰਜ਼ਿਲ_float (); }
  ਕਿਸਮ_ਨਾਂ Type::type ਮੰਜ਼ਿਲ_int () ਅਚਲ { ਵਾਪਸ Type::v; }
  ਖਾਲੀ ਸੈੱਟ_int (ਕਿਸਮ_ਨਾਂ Type::type i ) { Type::v = i; }
  ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_float (ਦਸ਼ਮਲਵ ਵਿੱਥ = 0) ਅਚਲ  { ਵਾਪਸ ((int32_t) Type::v + ਵਿੱਥ) * mult; }
  ਖਾਲੀ ਸੈੱਟ_float (ਦਸ਼ਮਲਵ f) { Type::v = roundf (f / mult); }
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (Type::static_ਮਾਪ);
};

/* 16-bit signed fixed number with the low 14 bits of fraction (2.14). */
ਵਰਤੋਂ F2DOT14 = HBFixed<HBINT16, 14>;
ਵਰਤੋਂ F4DOT12 = HBFixed<HBINT16, 12>;
ਵਰਤੋਂ F6DOT10 = HBFixed<HBINT16, 10>;

/* 32-bit signed fixed-point number (16.16). */
ਵਰਤੋਂ F16DOT16 = HBFixed<HBINT32, 16>;

/* Date represented in number of seconds since 12:00 midnight, January 1,
 * 1904. The value is represented as a signed 64-bit integer. */
ਬਣਤਰ LONGDATETIME
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }
  ਸੁਰੱਖਿਅਤ:
  HBINT32 major;
  HBUINT32 minor;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
ਬਣਤਰ ਟੈਗ : HBUINT32
{
  ਟੈਗ& ਸੰਚਾਲਕ = (hb_ਟੈਗ_t i) { HBUINT32::ਸੰਚਾਲਕ= (i); ਵਾਪਸ *ਇਹ; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  ਸੰਚਾਲਕ ਅਚਲ ਅੱਖਰ* () ਅਚਲ { ਵਾਪਸ reinterpret_cast<ਅਚਲ ਅੱਖਰ *> (ਇਹ); }
  ਸੰਚਾਲਕ ਅੱਖਰ* ()             { ਵਾਪਸ reinterpret_cast<ਅੱਖਰ *> (ਇਹ); }
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
ਬਣਤਰ HBGlyphID16 : HBUINT16
{
  HBGlyphID16& ਸੰਚਾਲਕ = (uint16_t i) { HBUINT16::ਸੰਚਾਲਕ= (i); ਵਾਪਸ *ਇਹ; }
};
ਬਣਤਰ HBGlyphID24 : HBUINT24
{
  HBGlyphID24& ਸੰਚਾਲਕ = (uint32_t i) { HBUINT24::ਸੰਚਾਲਕ= (i); ਵਾਪਸ *ਇਹ; }
};

/* Script/language-system/feature index */
ਬਣਤਰ ਸੂਚਕਾਂਕ : HBUINT16 {
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ = 0xFFFFu;
  ਸੂਚਕਾਂਕ& ਸੰਚਾਲਕ = (uint16_t i) { HBUINT16::ਸੰਚਾਲਕ= (i); ਵਾਪਸ *ਇਹ; }
};
DECLARE_NULL_NAMESPACE_ਬਾਈਟਾਂ (OT, ਸੂਚਕਾਂਕ);

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਸੂਚਕਾਂਕ NameID;

ਬਣਤਰ VarIdx : HBUINT32 {
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ, "");
  VarIdx& ਸੰਚਾਲਕ = (uint32_t i) { HBUINT32::ਸੰਚਾਲਕ= (i); ਵਾਪਸ *ਇਹ; }
};
DECLARE_NULL_NAMESPACE_ਬਾਈਟਾਂ (OT, VarIdx);

/* Offset, Null offset = 0 */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਤਾਰਕਿਕ has_null=ਸੱਚ>
ਬਣਤਰ ਵਿੱਥ_2 : Type
{
  ਵਿੱਥ_2& ਸੰਚਾਲਕ = (ਕਿਸਮ_ਨਾਂ Type::type i) { Type::ਸੰਚਾਲਕ= (i); ਵਾਪਸ *ਇਹ; }

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Type type;

  ਤਾਰਕਿਕ is_null () ਅਚਲ { ਵਾਪਸ has_null && 0 == *ਇਹ; }

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (ਆਕਾਰ (Type));
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਵਿੱਥ_2<HBUINT16> Offset16;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਵਿੱਥ_2<HBUINT24> Offset24;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਵਿੱਥ_2<HBUINT32> Offset32;


/* CheckSum */
ਬਣਤਰ Check_ਜੋੜ : HBUINT32
{
  Check_ਜੋੜ& ਸੰਚਾਲਕ = (uint32_t i) { HBUINT32::ਸੰਚਾਲਕ= (i); ਵਾਪਸ *ਇਹ; }

  /* This is reference implementation from the spec. */
  ਸਥਿਰ uint32_t Calc_ਸਾਰਣੀ_Checksum (ਅਚਲ HBUINT32 *ਸਾਰਣੀ_3, uint32_t ਲੰਮਾਈ_3)
  {
    uint32_t Sum = 0L;
    ਪ੍ਰਤਿਜ੍ਞਾ (0 == (ਲੰਮਾਈ_3 & 3));
    ਅਚਲ HBUINT32 *ਸਿਰਾ_Ptr = ਸਾਰਣੀ_3 + ਲੰਮਾਈ_3 / HBUINT32::static_ਮਾਪ;

    ਜਦੋਂ (ਸਾਰਣੀ_3 < ਸਿਰਾ_Ptr)
      Sum += *ਸਾਰਣੀ_3++;
    ਵਾਪਸ Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  ਖਾਲੀ ਸੈੱਟ_for_ਅੰਕੜੇ (ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ)
  { *ਇਹ = Calc_ਸਾਰਣੀ_Checksum ((ਅਚਲ HBUINT32 *) ਅੰਕੜੇ, ਲੰਮਾਈ); }

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};


/*
 * Version Numbers
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ FixedType=HBUINT16>
ਬਣਤਰ Fixed_ਸੰਸਕਰਣ
{
  uint32_t ਮੰਜ਼ਿਲ_int () ਅਚਲ { ਵਾਪਸ (major << (ਆਕਾਰ (FixedType) * 8)) + minor; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  FixedType major;
  FixedType minor;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (2 * ਆਕਾਰ (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਤਾਰਕਿਕ has_null>
ਬਣਤਰ _hb_has_null
{
  ਸਥਿਰ ਅਚਲ Type *get_null () { ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ; }
  ਸਥਿਰ Type *get_crap ()       { ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ; }
};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ _hb_has_null<Type, ਸੱਚ>
{
  ਸਥਿਰ ਅਚਲ Type *get_null () { ਵਾਪਸ &Null (Type); }
  ਸਥਿਰ       Type *get_crap () { ਵਾਪਸ &Crap (Type); }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ਵਿੱਥ_Type, ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_Type=ਖਾਲੀ, ਤਾਰਕਿਕ has_null=ਸੱਚ>
ਬਣਤਰ ਵਿੱਥ_ਮੰਜ਼ਿਲ : ਵਿੱਥ_2<ਵਿੱਥ_Type, has_null>
{
  ਵਰਤੋਂ ਨਿਸ਼ਾਨਾ_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == ਝੂਠ ||
		 (hb_has_null_ਮਾਪ_2 (Type) || !hb_has_min_ਮਾਪ_2 (Type)), "");

  HB_DELETE_COPY_ਸੌਂਪੋ (ਵਿੱਥ_ਮੰਜ਼ਿਲ);
  ਵਿੱਥ_ਮੰਜ਼ਿਲ () = ਮੂਲ;

  ਵਿੱਥ_ਮੰਜ਼ਿਲ& ਸੰਚਾਲਕ = (ਕਿਸਮ_ਨਾਂ ਵਿੱਥ_Type::type i) { ਵਿੱਥ_Type::ਸੰਚਾਲਕ= (i); ਵਾਪਸ *ਇਹ; }

  ਅਚਲ Type& ਸੰਚਾਲਕ () (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    ਜੇ (unlikely (ਇਹ->is_null ())) ਵਾਪਸ *_hb_has_null<Type, has_null>::get_null ();
    ਵਾਪਸ Struct_At_ਵਿੱਥ<ਅਚਲ Type> (ਅਧਾਰ, *ਇਹ);
  }
  Type& ਸੰਚਾਲਕ () (ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    ਜੇ (unlikely (ਇਹ->is_null ())) ਵਾਪਸ *_hb_has_null<Type, has_null>::get_crap ();
    ਵਾਪਸ Struct_At_ਵਿੱਥ<Type> (ਅਧਾਰ, *ਇਹ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_3,
	    hb_enable_if (hb_is_convertible (ਅਚਲ ਅਧਾਰ_3, ਅਚਲ ਅਧਾਰ_Type *))>
  ਮਿੱਤਰ ਅਚਲ Type& ਸੰਚਾਲਕ + (ਅਚਲ ਅਧਾਰ_3 &ਅਧਾਰ, ਅਚਲ ਵਿੱਥ_ਮੰਜ਼ਿਲ &ਵਿੱਥ) { ਵਾਪਸ ਵਿੱਥ ((ਅਚਲ ਖਾਲੀ *) ਅਧਾਰ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_3,
	    hb_enable_if (hb_is_convertible (ਅਚਲ ਅਧਾਰ_3, ਅਚਲ ਅਧਾਰ_Type *))>
  ਮਿੱਤਰ ਅਚਲ Type& ਸੰਚਾਲਕ + (ਅਚਲ ਵਿੱਥ_ਮੰਜ਼ਿਲ &ਵਿੱਥ, ਅਚਲ ਅਧਾਰ_3 &ਅਧਾਰ) { ਵਾਪਸ ਵਿੱਥ ((ਅਚਲ ਖਾਲੀ *) ਅਧਾਰ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_3,
	    hb_enable_if (hb_is_convertible (ਅਧਾਰ_3, ਅਧਾਰ_Type *))>
  ਮਿੱਤਰ Type& ਸੰਚਾਲਕ + (ਅਧਾਰ_3 &&ਅਧਾਰ, ਵਿੱਥ_ਮੰਜ਼ਿਲ &ਵਿੱਥ) { ਵਾਪਸ ਵਿੱਥ ((ਖਾਲੀ *) ਅਧਾਰ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_3,
	    hb_enable_if (hb_is_convertible (ਅਧਾਰ_3, ਅਧਾਰ_Type *))>
  ਮਿੱਤਰ Type& ਸੰਚਾਲਕ + (ਵਿੱਥ_ਮੰਜ਼ਿਲ &ਵਿੱਥ, ਅਧਾਰ_3 &&ਅਧਾਰ) { ਵਾਪਸ ਵਿੱਥ ((ਖਾਲੀ *) ਅਧਾਰ); }


  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_3, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਤਾਰਕਿਕ serialize_subset (hb_subset_context_t *c, ਅਚਲ ਵਿੱਥ_ਮੰਜ਼ਿਲ& src,
			 ਅਚਲ ਅਧਾਰ_3 *src_ਅਧਾਰ, Ts&&... ds)
  {
    *ਇਹ = 0;
    ਜੇ (src.is_null ())
      ਵਾਪਸ ਝੂਠ;

    ਸਵੈਚਾਲਿਤ *s = c->serializer;

    s->push ();

    ਤਾਰਕਿਕ ret = c->dispatch (src_ਅਧਾਰ+src, std::forward<Ts> (ds)...);

    ਜੇ (ret || !has_null)
      s->ਜੋੜੋ_link (*ਇਹ, s->ਕੱਢੋ_pack ());
    ਨਹੀਂਤਾਂ
      s->ਕੱਢੋ_discard ();

    ਵਾਪਸ ret;
  }


  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ...Ts>
  ਤਾਰਕਿਕ serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *ਇਹ = 0;

    Type* obj = c->push<Type> ();
    ਤਾਰਕਿਕ ret = obj->serialize (c, std::forward<Ts> (ds)...);

    ਜੇ (ret)
      c->ਜੋੜੋ_link (*ਇਹ, c->ਕੱਢੋ_pack ());
    ਨਹੀਂਤਾਂ
      c->ਕੱਢੋ_discard ();

    ਵਾਪਸ ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ...Ts>
  ਤਾਰਕਿਕ serialize_copy (hb_serialize_context_t *c, ਅਚਲ ਵਿੱਥ_ਮੰਜ਼ਿਲ& src,
		       ਅਚਲ ਖਾਲੀ *src_ਅਧਾਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *ਇਹ = 0;
    ਜੇ (src.is_null ())
      ਵਾਪਸ ਝੂਠ;

    c->push ();

    ਤਾਰਕਿਕ ret = c->copy (src_ਅਧਾਰ+src, std::forward<Ts> (ds)...);

    c->ਜੋੜੋ_link (*ਇਹ, c->ਕੱਢੋ_pack (), whence, dst_bias);

    ਵਾਪਸ ret;
  }

  ਤਾਰਕਿਕ serialize_copy (hb_serialize_context_t *c, ਅਚਲ ਵਿੱਥ_ਮੰਜ਼ਿਲ& src,
		       ਅਚਲ ਖਾਲੀ *src_ਅਧਾਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ dst_bias = 0)
  { ਵਾਪਸ serialize_copy (c, src, src_ਅਧਾਰ, dst_bias, hb_serialize_context_t::ਸਿਰ); }

  ਤਾਰਕਿਕ sanitize_shallow (hb_sanitize_context_t *c, ਅਚਲ ਅਧਾਰ_Type *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!c->check_struct (ਇਹ))) return_trace (ਝੂਠ);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    ਜੇ (unlikely ((ਅਚਲ ਅੱਖਰ *) ਅਧਾਰ + (ਬਿਨਾਨਿਸ਼ਾਨੀ) *ਇਹ < (ਅਚਲ ਅੱਖਰ *) ਅਧਾਰ)) return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ...Ts>
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਅਧਾਰ_Type *ਅਧਾਰ, Ts&&... ds) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (sanitize_shallow (c, ਅਧਾਰ) &&
		  hb_barrier () &&
		  (ਇਹ->is_null () ||
		   c->dispatch (Struct_At_ਵਿੱਥ<Type> (ਅਧਾਰ, *ਇਹ), std::forward<Ts> (ds)...)));
  }

  DEFINE_ਮਾਪ_STATIC (ਆਕਾਰ (ਵਿੱਥ_Type));
};
/* Partial specializations. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_Type=ਖਾਲੀ, ਤਾਰਕਿਕ has_null=ਸੱਚ> ਵਰਤੋਂ Offset16_ਮੰਜ਼ਿਲ = ਵਿੱਥ_ਮੰਜ਼ਿਲ<Type, HBUINT16, ਅਧਾਰ_Type, has_null>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_Type=ਖਾਲੀ, ਤਾਰਕਿਕ has_null=ਸੱਚ> ਵਰਤੋਂ Offset24_ਮੰਜ਼ਿਲ = ਵਿੱਥ_ਮੰਜ਼ਿਲ<Type, HBUINT24, ਅਧਾਰ_Type, has_null>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_Type=ਖਾਲੀ, ਤਾਰਕਿਕ has_null=ਸੱਚ> ਵਰਤੋਂ Offset32_ਮੰਜ਼ਿਲ = ਵਿੱਥ_ਮੰਜ਼ਿਲ<Type, HBUINT32, ਅਧਾਰ_Type, has_null>;

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ਵਿੱਥ_Type, ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_Type=ਖਾਲੀ> ਵਰਤੋਂ NNOffset_ਮੰਜ਼ਿਲ = ਵਿੱਥ_ਮੰਜ਼ਿਲ<Type, ਵਿੱਥ_Type, ਅਧਾਰ_Type, ਝੂਠ>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_Type=ਖਾਲੀ> ਵਰਤੋਂ NNOffset16_ਮੰਜ਼ਿਲ = Offset16_ਮੰਜ਼ਿਲ<Type, ਅਧਾਰ_Type, ਝੂਠ>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_Type=ਖਾਲੀ> ਵਰਤੋਂ NNOffset24_ਮੰਜ਼ਿਲ = Offset24_ਮੰਜ਼ਿਲ<Type, ਅਧਾਰ_Type, ਝੂਠ>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_Type=ਖਾਲੀ> ਵਰਤੋਂ NNOffset32_ਮੰਜ਼ਿਲ = Offset32_ਮੰਜ਼ਿਲ<Type, ਅਧਾਰ_Type, ਝੂਠ>;


/*
 * Array Types
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ Unsized_Array_ਦਾ
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Type ਵਸਤੂ_t;
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਸਤੂ_ਮਾਪ = hb_static_ਮਾਪ (Type);

  HB_DELETE_CREATE_COPY_ਸੌਂਪੋ (Unsized_Array_ਦਾ);

  ਅਚਲ Type& ਸੰਚਾਲਕ [] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  {
    ਵਾਪਸ arrayZ[i];
  }
  Type& ਸੰਚਾਲਕ [] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i)
  {
    ਵਾਪਸ arrayZ[i];
  }

  ਸਥਿਰ size_t get_ਮਾਪ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len)
  { ਵਾਪਸ hb_unsigned_mul_saturate (len, Type::static_ਮਾਪ); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸੰਚਾਲਕ T * () { ਵਾਪਸ arrayZ; }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸੰਚਾਲਕ ਅਚਲ T * () ਅਚਲ { ਵਾਪਸ arrayZ; }
  hb_array_t<Type> as_array (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len)
  { ਵਾਪਸ hb_array (arrayZ, len); }
  hb_array_t<ਅਚਲ Type> as_array (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len) ਅਚਲ
  { ਵਾਪਸ hb_array (arrayZ, len); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  Type &lsearch (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len, ਅਚਲ T &x, Type &not_ਮਿਲਿਆ = Crap (Type))
  { ਵਾਪਸ *as_array (len).lsearch (x, &not_ਮਿਲਿਆ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਅਚਲ Type &lsearch (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len, ਅਚਲ T &x, ਅਚਲ Type &not_ਮਿਲਿਆ = Null (Type)) ਅਚਲ
  { ਵਾਪਸ *as_array (len).lsearch (x, &not_ਮਿਲਿਆ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ lfind (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len, ਅਚਲ T &x, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *i = ਖਾਲੀ_ਸੂਚਕ,
	      hb_not_ਮਿਲਿਆ_t not_ਮਿਲਿਆ = HB_NOT_ਮਿਲਿਆ_DONT_STORE,
	      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੰਜ਼ਿਲ_store = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) -1) ਅਚਲ
  { ਵਾਪਸ as_array (len).lfind (x, i, not_ਮਿਲਿਆ, ਮੰਜ਼ਿਲ_store); }

  ਖਾਲੀ qsort (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = 0, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) -1)
  { as_array (len).qsort (ਸ਼ੁਰੂ, ਸਿਰਾ); }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਚੀਜ਼ਾਂ_len, ਤਾਰਕਿਕ clear = ਸੱਚ)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_ਮਾਪ (ਇਹ, get_ਮਾਪ (ਚੀਜ਼ਾਂ_len), clear))) return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
	    hb_requires (hb_is_ਸਰੋਤ_ਦਾ (Iterator, Type))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, Iterator ਚੀਜ਼ਾਂ)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = hb_len (ਚੀਜ਼ਾਂ);
    ਜੇ (unlikely (!serialize (c, ਗਿਣਤੀ_2, ਝੂਠ))) return_trace (ਝੂਠ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++, ++ਚੀਜ਼ਾਂ)
      arrayZ[i] = *ਚੀਜ਼ਾਂ;
    return_trace (ਸੱਚ);
  }

  Unsized_Array_ਦਾ* copy (hb_serialize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!as_array (ਗਿਣਤੀ_2).copy (c))) return_trace (ਖਾਲੀ_ਸੂਚਕ);
    return_trace (ਬਾਹਰਲਾ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ...Ts>
  HB_ALWAYS_INLINE
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, Ts&&... ds) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!sanitize_shallow (c, ਗਿਣਤੀ_2))) return_trace (ਝੂਠ);
    ਜੇ (!ਆਕਾਰ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ਸੱਚ);
    hb_barrier ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize_shallow (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_array (arrayZ, ਗਿਣਤੀ_2));
  }

  ਜਨਤਕ:
  Type		arrayZ[HB_VAR_ARRAY];
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNBOUNDED (0);
};

/* Unsized array of offset's */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ਵਿੱਥ_Type, ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_Type=ਖਾਲੀ, ਤਾਰਕਿਕ has_null=ਸੱਚ>
ਵਰਤੋਂ Unsized_Array16_ਦਾ_ਵਿੱਥ_ਮੰਜ਼ਿਲ = Unsized_Array_ਦਾ<ਵਿੱਥ_ਮੰਜ਼ਿਲ<Type, ਵਿੱਥ_Type, ਅਧਾਰ_Type, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ਵਿੱਥ_Type, ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_Type=ਖਾਲੀ, ਤਾਰਕਿਕ has_null=ਸੱਚ>
ਬਣਤਰ Unsized_List_ਦਾ_Offset16_ਮੰਜ਼ਿਲ : Unsized_Array16_ਦਾ_ਵਿੱਥ_ਮੰਜ਼ਿਲ<Type, ਵਿੱਥ_Type, ਅਧਾਰ_Type, has_null>
{
  ਅਚਲ Type& ਸੰਚਾਲਕ [] (ਪੂਰਨ i_) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) i_;
    ਅਚਲ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Type, ਵਿੱਥ_Type, ਅਧਾਰ_Type, has_null> *p = &ਇਹ->arrayZ[i];
    ਜੇ (unlikely ((ਅਚਲ ਖਾਲੀ *) p < (ਅਚਲ ਖਾਲੀ *) ਇਹ->arrayZ)) ਵਾਪਸ Null (Type); /* Overflowed. */
    hb_barrier ();
    ਵਾਪਸ ਇਹ+*p;
  }
  Type& ਸੰਚਾਲਕ [] (ਪੂਰਨ i_)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) i_;
    ਅਚਲ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Type, ਵਿੱਥ_Type, ਅਧਾਰ_Type, has_null> *p = &ਇਹ->arrayZ[i];
    ਜੇ (unlikely ((ਅਚਲ ਖਾਲੀ *) p < (ਅਚਲ ਖਾਲੀ *) ਇਹ->arrayZ)) ਵਾਪਸ Crap (Type); /* Overflowed. */
    hb_barrier ();
    ਵਾਪਸ ਇਹ+*p;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ...Ts>
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, Ts&&... ds) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace ((Unsized_Array16_ਦਾ_ਵਿੱਥ_ਮੰਜ਼ਿਲ<Type, ਵਿੱਥ_Type, ਅਧਾਰ_Type, has_null>
		   ::sanitize (c, ਗਿਣਤੀ_2, ਇਹ, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ Sorted_Unsized_Array_ਦਾ : Unsized_Array_ਦਾ<Type>
{
  hb_sorted_array_t<Type> as_array (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len)
  { ਵਾਪਸ hb_sorted_array (ਇਹ->arrayZ, len); }
  hb_sorted_array_t<ਅਚਲ Type> as_array (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len) ਅਚਲ
  { ਵਾਪਸ hb_sorted_array (ਇਹ->arrayZ, len); }
  ਸੰਚਾਲਕ hb_sorted_array_t<Type> ()             { ਵਾਪਸ as_array (); }
  ਸੰਚਾਲਕ hb_sorted_array_t<ਅਚਲ Type> () ਅਚਲ { ਵਾਪਸ as_array (); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  Type &bsearch (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len, ਅਚਲ T &x, Type &not_ਮਿਲਿਆ = Crap (Type))
  { ਵਾਪਸ *as_array (len).bsearch (x, &not_ਮਿਲਿਆ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਅਚਲ Type &bsearch (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len, ਅਚਲ T &x, ਅਚਲ Type &not_ਮਿਲਿਆ = Null (Type)) ਅਚਲ
  { ਵਾਪਸ *as_array (len).bsearch (x, &not_ਮਿਲਿਆ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ bfind (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len, ਅਚਲ T &x, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *i = ਖਾਲੀ_ਸੂਚਕ,
	      hb_not_ਮਿਲਿਆ_t not_ਮਿਲਿਆ = HB_NOT_ਮਿਲਿਆ_DONT_STORE,
	      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੰਜ਼ਿਲ_store = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) -1) ਅਚਲ
  { ਵਾਪਸ as_array (len).bfind (x, i, not_ਮਿਲਿਆ, ਮੰਜ਼ਿਲ_store); }
};


/* An array with a number of elements. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ LenType>
ਬਣਤਰ Array_ਦਾ
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Type ਵਸਤੂ_t;
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਸਤੂ_ਮਾਪ = hb_static_ਮਾਪ (Type);

  HB_DELETE_CREATE_COPY_ਸੌਂਪੋ (Array_ਦਾ);

  ਅਚਲ Type& ਸੰਚਾਲਕ [] (ਪੂਰਨ i_) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) i_;
    ਜੇ (unlikely (i >= len)) ਵਾਪਸ Null (Type);
    hb_barrier ();
    ਵਾਪਸ arrayZ[i];
  }
  Type& ਸੰਚਾਲਕ [] (ਪੂਰਨ i_)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) i_;
    ਜੇ (unlikely (i >= len)) ਵਾਪਸ Crap (Type);
    hb_barrier ();
    ਵਾਪਸ arrayZ[i];
  }

  size_t get_ਮਾਪ () ਅਚਲ
  { ਵਾਪਸ hb_unsigned_mul_ਜੋੜੋ_saturate (len, Type::static_ਮਾਪ, len.static_ਮਾਪ); }

  ਸਪਸ਼ਟ ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ len; }

  ਖਾਲੀ ਕੱਢੋ () { len--; }

  hb_array_t<      Type> as_array ()       { ਵਾਪਸ hb_array (arrayZ, len); }
  hb_array_t<ਅਚਲ Type> as_array () ਅਚਲ { ਵਾਪਸ hb_array (arrayZ, len); }

  /* Iterator. */
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_array_t<ਅਚਲ Type>   iter_t;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_array_t<      Type> writer_t;
    iter_t   iter () ਅਚਲ { ਵਾਪਸ as_array (); }
  writer_t writer ()       { ਵਾਪਸ as_array (); }
  ਸੰਚਾਲਕ   iter_t () ਅਚਲ { ਵਾਪਸ   iter (); }
  ਸੰਚਾਲਕ writer_t ()       { ਵਾਪਸ writer (); }

  /* Faster range-based for loop. */
  ਅਚਲ Type *begin () ਅਚਲ { ਵਾਪਸ arrayZ; }
  ਅਚਲ Type *ਸਿਰਾ () ਅਚਲ { ਵਾਪਸ arrayZ + len; }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  Type &lsearch (ਅਚਲ T &x, Type &not_ਮਿਲਿਆ = Crap (Type))
  { ਵਾਪਸ *as_array ().lsearch (x, &not_ਮਿਲਿਆ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਅਚਲ Type &lsearch (ਅਚਲ T &x, ਅਚਲ Type &not_ਮਿਲਿਆ = Null (Type)) ਅਚਲ
  { ਵਾਪਸ *as_array ().lsearch (x, &not_ਮਿਲਿਆ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ lfind (ਅਚਲ T &x, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *i = ਖਾਲੀ_ਸੂਚਕ,
	      hb_not_ਮਿਲਿਆ_t not_ਮਿਲਿਆ = HB_NOT_ਮਿਲਿਆ_DONT_STORE,
	      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੰਜ਼ਿਲ_store = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) -1) ਅਚਲ
  { ਵਾਪਸ as_array ().lfind (x, i, not_ਮਿਲਿਆ, ਮੰਜ਼ਿਲ_store); }

  ਖਾਲੀ qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੀਜ਼ਾਂ_len, ਤਾਰਕਿਕ clear = ਸੱਚ)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    c->check_ਸੌਂਪੋ (len, ਚੀਜ਼ਾਂ_len, HB_SERIALIZE_ERROR_ARRAY_ਉਛਾਲ);
    ਜੇ (unlikely (!c->extend_ਮਾਪ (ਇਹ, get_ਮਾਪ (), clear))) return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
	    hb_requires (hb_is_ਸਰੋਤ_ਦਾ (Iterator, Type))>
  HB_NODISCARD ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, Iterator ਚੀਜ਼ਾਂ)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = hb_len (ਚੀਜ਼ਾਂ);
    ਜੇ (unlikely (!serialize (c, ਗਿਣਤੀ_2, ਝੂਠ))) return_trace (ਝੂਠ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++, ++ਚੀਜ਼ਾਂ)
      arrayZ[i] = *ਚੀਜ਼ਾਂ;
    return_trace (ਸੱਚ);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (ਇਹ);
    len++;
    ਜੇ (unlikely (!len || !c->extend (ਇਹ)))
    {
      len--;
      return_trace (ਖਾਲੀ_ਸੂਚਕ);
    }
    return_trace (&arrayZ[len - 1]);
  }

  Array_ਦਾ* copy (hb_serialize_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਬਾਹਰਲਾ))) return_trace (ਖਾਲੀ_ਸੂਚਕ);
    c->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->len, len, HB_SERIALIZE_ERROR_ARRAY_ਉਛਾਲ);
    ਜੇ (unlikely (!as_array ().copy (c))) return_trace (ਖਾਲੀ_ਸੂਚਕ);
    return_trace (ਬਾਹਰਲਾ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ...Ts>
  HB_ALWAYS_INLINE
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!sanitize_shallow (c))) return_trace (ਝੂਠ);
    ਜੇ (!ਆਕਾਰ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ਸੱਚ);
    hb_barrier ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize_shallow (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (len.sanitize (c) &&
		  hb_barrier () &&
		  c->check_array_sized (arrayZ, len, ਆਕਾਰ (LenType)));
  }

  ਜਨਤਕ:
  LenType	len;
  Type		arrayZ[HB_VAR_ARRAY];
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (ਆਕਾਰ (LenType), arrayZ);
};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type> ਵਰਤੋਂ Array16_ਦਾ = Array_ਦਾ<Type, HBUINT16>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type> ਵਰਤੋਂ Array24_ਦਾ = Array_ਦਾ<Type, HBUINT24>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type> ਵਰਤੋਂ Array32_ਦਾ = Array_ਦਾ<Type, HBUINT32>;
ਵਰਤੋਂ PString = Array_ਦਾ<HBUINT8, HBUINT8>;

/* Array of Offset's */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type> ਵਰਤੋਂ Array8_ਦਾ_Offset24_ਮੰਜ਼ਿਲ = Array_ਦਾ<ਵਿੱਥ_ਮੰਜ਼ਿਲ<Type, HBUINT24>, HBUINT8>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type> ਵਰਤੋਂ Array16_ਦਾ_Offset16_ਮੰਜ਼ਿਲ = Array_ਦਾ<ਵਿੱਥ_ਮੰਜ਼ਿਲ<Type, HBUINT16>, HBUINT16>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type> ਵਰਤੋਂ Array16_ਦਾ_Offset32_ਮੰਜ਼ਿਲ = Array_ਦਾ<ਵਿੱਥ_ਮੰਜ਼ਿਲ<Type, HBUINT32>, HBUINT16>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type> ਵਰਤੋਂ Array32_ਦਾ_Offset32_ਮੰਜ਼ਿਲ = Array_ਦਾ<ਵਿੱਥ_ਮੰਜ਼ਿਲ<Type, HBUINT32>, HBUINT32>;

/* Array of offsets relative to the beginning of the array itself. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ਵਿੱਥ_Type>
ਬਣਤਰ List16_ਦਾ_ਵਿੱਥ_ਮੰਜ਼ਿਲ : Array_ਦਾ<ਵਿੱਥ_ਮੰਜ਼ਿਲ<Type, ਵਿੱਥ_Type>, HBUINT16>
{
  ਅਚਲ Type& ਸੰਚਾਲਕ [] (ਪੂਰਨ i_) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) i_;
    ਜੇ (unlikely (i >= ਇਹ->len)) ਵਾਪਸ Null (Type);
    hb_barrier ();
    ਵਾਪਸ ਇਹ+ਇਹ->arrayZ[i];
  }
  ਅਚਲ Type& ਸੰਚਾਲਕ [] (ਪੂਰਨ i_)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) i_;
    ਜੇ (unlikely (i >= ਇਹ->len)) ਵਾਪਸ Crap (Type);
    hb_barrier ();
    ਵਾਪਸ ਇਹ+ਇਹ->arrayZ[i];
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਬਣਤਰ List16_ਦਾ_ਵਿੱਥ_ਮੰਜ਼ਿਲ *ਬਾਹਰਲਾ = c->serializer->embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਇਹ->len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਬਾਹਰਲਾ->arrayZ[i].serialize_subset (c, ਇਹ->arrayZ[i], ਇਹ, ਬਾਹਰਲਾ);
    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ...Ts>
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace ((Array16_ਦਾ<ਵਿੱਥ_ਮੰਜ਼ਿਲ<Type, ਵਿੱਥ_Type>>::sanitize (c, ਇਹ, std::forward<Ts> (ds)...)));
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਵਰਤੋਂ List16_ਦਾ_Offset16_ਮੰਜ਼ਿਲ = List16_ਦਾ_ਵਿੱਥ_ਮੰਜ਼ਿਲ<Type, HBUINT16>;

/* An array starting at second element. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ LenType>
ਬਣਤਰ Headless_Array_ਦਾ
{
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਸਤੂ_ਮਾਪ = Type::static_ਮਾਪ;

  HB_DELETE_CREATE_COPY_ਸੌਂਪੋ (Headless_Array_ਦਾ);

  ਅਚਲ Type& ਸੰਚਾਲਕ [] (ਪੂਰਨ i_) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) i_;
    ਜੇ (unlikely (i >= lenP1 || !i)) ਵਾਪਸ Null (Type);
    hb_barrier ();
    ਵਾਪਸ arrayZ[i-1];
  }
  Type& ਸੰਚਾਲਕ [] (ਪੂਰਨ i_)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) i_;
    ਜੇ (unlikely (i >= lenP1 || !i)) ਵਾਪਸ Crap (Type);
    hb_barrier ();
    ਵਾਪਸ arrayZ[i-1];
  }
  size_t get_ਮਾਪ () ਅਚਲ
  { ਵਾਪਸ hb_unsigned_mul_ਜੋੜੋ_saturate (get_ਲੰਮਾਈ (), Type::static_ਮਾਪ, lenP1.static_ਮਾਪ); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਲੰਮਾਈ () ਅਚਲ { ਵਾਪਸ lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { ਵਾਪਸ hb_array (arrayZ, get_ਲੰਮਾਈ ()); }
  hb_array_t<ਅਚਲ Type> as_array () ਅਚਲ { ਵਾਪਸ hb_array (arrayZ, get_ਲੰਮਾਈ ()); }

  /* Iterator. */
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_array_t<ਅਚਲ Type>   iter_t;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_array_t<      Type> writer_t;
    iter_t   iter () ਅਚਲ { ਵਾਪਸ as_array (); }
  writer_t writer ()       { ਵਾਪਸ as_array (); }
  ਸੰਚਾਲਕ   iter_t () ਅਚਲ { ਵਾਪਸ   iter (); }
  ਸੰਚਾਲਕ writer_t ()       { ਵਾਪਸ writer (); }

  /* Faster range-based for loop. */
  ਅਚਲ Type *begin () ਅਚਲ { ਵਾਪਸ arrayZ; }
  ਅਚਲ Type *ਸਿਰਾ () ਅਚਲ { ਵਾਪਸ arrayZ + get_ਲੰਮਾਈ (); }

  HB_NODISCARD ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਚੀਜ਼ਾਂ_len, ਤਾਰਕਿਕ clear = ਸੱਚ)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    c->check_ਸੌਂਪੋ (lenP1, ਚੀਜ਼ਾਂ_len + 1, HB_SERIALIZE_ERROR_ARRAY_ਉਛਾਲ);
    ਜੇ (unlikely (!c->extend_ਮਾਪ (ਇਹ, get_ਮਾਪ (), clear))) return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
	    hb_requires (hb_is_ਸਰੋਤ_ਦਾ (Iterator, Type))>
  HB_NODISCARD ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, Iterator ਚੀਜ਼ਾਂ)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = hb_len (ਚੀਜ਼ਾਂ);
    ਜੇ (unlikely (!serialize (c, ਗਿਣਤੀ_2, ਝੂਠ))) return_trace (ਝੂਠ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++, ++ਚੀਜ਼ਾਂ)
      arrayZ[i] = *ਚੀਜ਼ਾਂ;
    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ...Ts>
  HB_ALWAYS_INLINE
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!sanitize_shallow (c))) return_trace (ਝੂਠ);
    ਜੇ (!ਆਕਾਰ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ਸੱਚ);
    hb_barrier ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = get_ਲੰਮਾਈ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਨਿਜੀ:
  ਤਾਰਕਿਕ sanitize_shallow (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (lenP1.sanitize (c) &&
		  hb_barrier () &&
		  (!lenP1 || c->check_array_sized (arrayZ, lenP1 - 1, ਆਕਾਰ (LenType))));
  }

  ਜਨਤਕ:
  LenType	lenP1;
  Type		arrayZ[HB_VAR_ARRAY];
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (ਆਕਾਰ (LenType), arrayZ);
};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type> ਵਰਤੋਂ Headless_Array16_ਦਾ = Headless_Array_ਦਾ<Type, HBUINT16>;

/* An array storing length-1. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ LenType=HBUINT16>
ਬਣਤਰ Array_ਦਾ_M1
{
  HB_DELETE_CREATE_COPY_ਸੌਂਪੋ (Array_ਦਾ_M1);

  ਅਚਲ Type& ਸੰਚਾਲਕ [] (ਪੂਰਨ i_) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) i_;
    ਜੇ (unlikely (i > lenM1)) ਵਾਪਸ Null (Type);
    hb_barrier ();
    ਵਾਪਸ arrayZ[i];
  }
  Type& ਸੰਚਾਲਕ [] (ਪੂਰਨ i_)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) i_;
    ਜੇ (unlikely (i > lenM1)) ਵਾਪਸ Crap (Type);
    hb_barrier ();
    ਵਾਪਸ arrayZ[i];
  }
  size_t get_ਮਾਪ () ਅਚਲ
  { ਵਾਪਸ hb_unsigned_mul_ਜੋੜੋ_saturate (lenM1 + 1, Type::static_ਮਾਪ, lenM1.static_ਮਾਪ); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ...Ts>
  HB_ALWAYS_INLINE
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!sanitize_shallow (c))) return_trace (ਝੂਠ);
    ਜੇ (!ਆਕਾਰ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ਸੱਚ);
    hb_barrier ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = lenM1 + 1;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਨਿਜੀ:
  ਤਾਰਕਿਕ sanitize_shallow (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (lenM1.sanitize (c) &&
		  hb_barrier () &&
		  (c->check_array_sized (arrayZ, lenM1 + 1, ਆਕਾਰ (LenType))));
  }

  ਜਨਤਕ:
  LenType	lenM1;
  Type		arrayZ[HB_VAR_ARRAY];
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (ਆਕਾਰ (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ LenType>
ਬਣਤਰ Sorted_Array_ਦਾ : Array_ਦਾ<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { ਵਾਪਸ hb_sorted_array (ਇਹ->arrayZ, ਇਹ->len); }
  hb_sorted_array_t<ਅਚਲ Type> as_array () ਅਚਲ { ਵਾਪਸ hb_sorted_array (ਇਹ->arrayZ, ਇਹ->len); }

  /* Iterator. */
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_sorted_array_t<ਅਚਲ Type>   iter_t;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () ਅਚਲ { ਵਾਪਸ as_array (); }
  writer_t writer ()       { ਵਾਪਸ as_array (); }
  ਸੰਚਾਲਕ   iter_t () ਅਚਲ { ਵਾਪਸ   iter (); }
  ਸੰਚਾਲਕ writer_t ()       { ਵਾਪਸ writer (); }

  /* Faster range-based for loop. */
  ਅਚਲ Type *begin () ਅਚਲ { ਵਾਪਸ ਇਹ->arrayZ; }
  ਅਚਲ Type *ਸਿਰਾ () ਅਚਲ { ਵਾਪਸ ਇਹ->arrayZ + ਇਹ->len; }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਚੀਜ਼ਾਂ_len)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਤਾਰਕਿਕ ret = Array_ਦਾ<Type, LenType>::serialize (c, ਚੀਜ਼ਾਂ_len);
    return_trace (ret);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
	    hb_requires (hb_is_sorted_ਸਰੋਤ_ਦਾ (Iterator, Type))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, Iterator ਚੀਜ਼ਾਂ)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਤਾਰਕਿਕ ret = Array_ਦਾ<Type, LenType>::serialize (c, ਚੀਜ਼ਾਂ);
    return_trace (ret);
  }

  Sorted_Array_ਦਾ* copy (hb_serialize_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    Sorted_Array_ਦਾ* ਬਾਹਰਲਾ = reinterpret_cast<Sorted_Array_ਦਾ *> (Array_ਦਾ<Type, LenType>::copy (c));
    return_trace (ਬਾਹਰਲਾ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  Type &bsearch (ਅਚਲ T &x, Type &not_ਮਿਲਿਆ = Crap (Type))
  { ਵਾਪਸ *as_array ().bsearch (x, &not_ਮਿਲਿਆ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਅਚਲ Type &bsearch (ਅਚਲ T &x, ਅਚਲ Type &not_ਮਿਲਿਆ = Null (Type)) ਅਚਲ
  { ਵਾਪਸ *as_array ().bsearch (x, &not_ਮਿਲਿਆ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ bfind (ਅਚਲ T &x, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *i = ਖਾਲੀ_ਸੂਚਕ,
	      hb_not_ਮਿਲਿਆ_t not_ਮਿਲਿਆ = HB_NOT_ਮਿਲਿਆ_DONT_STORE,
	      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੰਜ਼ਿਲ_store = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) -1) ਅਚਲ
  { ਵਾਪਸ as_array ().bfind (x, i, not_ਮਿਲਿਆ, ਮੰਜ਼ਿਲ_store); }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type> ਵਰਤੋਂ Sorted_Array16_ਦਾ = Sorted_Array_ਦਾ<Type, HBUINT16>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type> ਵਰਤੋਂ Sorted_Array24_ਦਾ = Sorted_Array_ਦਾ<Type, HBUINT24>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type> ਵਰਤੋਂ Sorted_Array32_ਦਾ = Sorted_Array_ਦਾ<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ LenType=HBUINT16>
ਬਣਤਰ Bin_ਖੋਜ_ਸਿਰਲੇਖ
{
  ਸੰਚਾਲਕ uint32_t () ਅਚਲ { ਵਾਪਸ len; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  Bin_ਖੋਜ_ਸਿਰਲੇਖ& ਸੰਚਾਲਕ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ v)
  {
    len = v;
    ਪ੍ਰਤਿਜ੍ਞਾ (len == v);
    ਇੰਦਰਾਜ_Selector = hb_max (1u, hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (v)) - 1;
    ਖੋਜ_Range = 16 * (1u << ਇੰਦਰਾਜ_Selector);
    range_ਖਿਸਕਾਓ = v * 16 > ਖੋਜ_Range
		 ? 16 * v - ਖੋਜ_Range
		 : 0;
    ਵਾਪਸ *ਇਹ;
  }

  ਸੁਰੱਖਿਅਤ:
  LenType	len;
  LenType	ਖੋਜ_Range;
  LenType	ਇੰਦਰਾਜ_Selector;
  LenType	range_ਖਿਸਕਾਓ;

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ LenType=HBUINT16>
ਵਰਤੋਂ Bin_ਖੋਜ_Array_ਦਾ = Sorted_Array_ਦਾ<Type, Bin_ਖੋਜ_ਸਿਰਲੇਖ<LenType>>;


ਬਣਤਰ Var_Sized_Bin_ਖੋਜ_ਸਿਰਲੇਖ
{

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  HBUINT16	unit_ਮਾਪ;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	ਖੋਜ_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	ਇੰਦਰਾਜ_Selector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	range_ਖਿਸਕਾਓ;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (10);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ Var_Sized_Bin_ਖੋਜ_Array_ਦਾ
{
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਸਤੂ_ਮਾਪ = Type::static_ਮਾਪ;

  HB_DELETE_CREATE_COPY_ਸੌਂਪੋ (Var_Sized_Bin_ਖੋਜ_Array_ਦਾ);

  ਤਾਰਕਿਕ ਆਖਰੀ_is_terminator () ਅਚਲ
  {
    ਜੇ (unlikely (!ਸਿਰਲੇਖ.nUnits)) ਵਾਪਸ ਝੂਠ;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    ਅਚਲ HBUINT16 *ਸ਼ਬਦ_2 = &Struct_At_ਵਿੱਥ<HBUINT16> (&ਬਾਈਟਾਂ_Z, (ਸਿਰਲੇਖ.nUnits - 1) * ਸਿਰਲੇਖ.unit_ਮਾਪ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = Type::Termination_ਸ਼ਬਦ_ਗਿਣਤੀ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (ਸ਼ਬਦ_2[i] != 0xFFFFu)
	ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
  }

  ਅਚਲ Type& ਸੰਚਾਲਕ [] (ਪੂਰਨ i_) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) i_;
    ਜੇ (unlikely (i >= get_ਲੰਮਾਈ ())) ਵਾਪਸ Null (Type);
    hb_barrier ();
    ਵਾਪਸ Struct_At_ਵਿੱਥ<Type> (&ਬਾਈਟਾਂ_Z, i * ਸਿਰਲੇਖ.unit_ਮਾਪ);
  }
  Type& ਸੰਚਾਲਕ [] (ਪੂਰਨ i_)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) i_;
    ਜੇ (unlikely (i >= get_ਲੰਮਾਈ ())) ਵਾਪਸ Crap (Type);
    hb_barrier ();
    ਵਾਪਸ Struct_At_ਵਿੱਥ<Type> (&ਬਾਈਟਾਂ_Z, i * ਸਿਰਲੇਖ.unit_ਮਾਪ);
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਲੰਮਾਈ () ਅਚਲ
  { ਵਾਪਸ ਸਿਰਲੇਖ.nUnits - ਆਖਰੀ_is_terminator (); }
  size_t get_ਮਾਪ () ਅਚਲ
  { ਵਾਪਸ hb_unsigned_mul_ਜੋੜੋ_saturate (ਸਿਰਲੇਖ.nUnits, ਸਿਰਲੇਖ.unit_ਮਾਪ, ਸਿਰਲੇਖ.static_ਮਾਪ); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ...Ts>
  HB_ALWAYS_INLINE
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!sanitize_shallow (c))) return_trace (ਝੂਠ);
    ਜੇ (!ਆਕਾਰ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ਸੱਚ);
    hb_barrier ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = get_ਲੰਮਾਈ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (unlikely (!(*ਇਹ)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਅਚਲ Type *bsearch (ਅਚਲ T &key) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ pos;
    ਵਾਪਸ hb_bsearch_impl (&pos,
			    key,
			    (ਅਚਲ ਖਾਲੀ *) ਬਾਈਟਾਂ_Z,
			    get_ਲੰਮਾਈ (),
			    ਸਿਰਲੇਖ.unit_ਮਾਪ,
			    hb_cmp_ਵਿਧੀ<T, Type>)
	   ? (ਅਚਲ Type *) (((ਅਚਲ ਅੱਖਰ *) &ਬਾਈਟਾਂ_Z) + (pos * ਸਿਰਲੇਖ.unit_ਮਾਪ))
	   : ਖਾਲੀ_ਸੂਚਕ;
  }

  ਨਿਜੀ:
  ਤਾਰਕਿਕ sanitize_shallow (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ਸਿਰਲੇਖ.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_ਮਾਪ <= ਸਿਰਲੇਖ.unit_ਮਾਪ &&
		  c->check_range (ਬਾਈਟਾਂ_Z.arrayZ,
				  ਸਿਰਲੇਖ.nUnits,
				  ਸਿਰਲੇਖ.unit_ਮਾਪ));
  }

  ਸੁਰੱਖਿਅਤ:
  Var_Sized_Bin_ਖੋਜ_ਸਿਰਲੇਖ	ਸਿਰਲੇਖ;
  Unsized_Array_ਦਾ<HBUINT8>	ਬਾਈਟਾਂ_Z;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (10, ਬਾਈਟਾਂ_Z);
};


/* CFF INDEX */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਗਿਣਤੀ_4>
ਬਣਤਰ CFFIndex
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ_array_ਮਾਪ () ਅਚਲ
  { ਵਾਪਸ off_ਮਾਪ * (ਗਿਣਤੀ_2 + 1); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ Iterable &iterable,
		  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *p_ਅੰਕੜੇ_ਮਾਪ = ਖਾਲੀ_ਸੂਚਕ,
                  ਬਿਨਾਨਿਸ਼ਾਨੀ min_off_ਮਾਪ = 0)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕੜੇ_ਮਾਪ;
    ਜੇ (p_ਅੰਕੜੇ_ਮਾਪ)
      ਅੰਕੜੇ_ਮਾਪ = *p_ਅੰਕੜੇ_ਮਾਪ;
    ਨਹੀਂਤਾਂ
      ਕੁੱਲ_ਮਾਪ (iterable, &ਅੰਕੜੇ_ਮਾਪ);

    ਸਵੈਚਾਲਿਤ it = hb_iter (iterable);
    ਜੇ (unlikely (!serialize_ਸਿਰਲੇਖ (c, +it, ਅੰਕੜੇ_ਮਾਪ, min_off_ਮਾਪ))) return_trace (ਝੂਠ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ret = c->allocate_ਮਾਪ<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> (ਅੰਕੜੇ_ਮਾਪ, ਝੂਠ);
    ਜੇ (unlikely (!ret)) return_trace (ਝੂਠ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਚਿਆ = ਅੰਕੜੇ_ਮਾਪ;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &_ : +it)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ len = _.ਲੰਮਾਈ;

      ਜੇ (!len)
	ਜਾਰੀ;

      ਜੇ (unlikely (len > ਬਚਿਆ)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (ਝੂਠ, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
      }

      ਬਚਿਆ -= len;

      ਜੇ (len <= 1)
      {
	*ret++ = *_.arrayZ;
	ਜਾਰੀ;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  ਤਾਰਕਿਕ serialize_ਸਿਰਲੇਖ (hb_serialize_context_t *c,
			 Iterator it,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕੜੇ_ਮਾਪ,
                         ਬਿਨਾਨਿਸ਼ਾਨੀ min_off_ਮਾਪ = 0)
  {
    TRACE_SERIALIZE (ਇਹ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ off_ਮਾਪ_2 = (hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (ਅੰਕੜੇ_ਮਾਪ + 1) + 7) / 8;
    off_ਮਾਪ_2 = hb_max(min_off_ਮਾਪ, off_ਮਾਪ_2);

    /* serialize CFFIndex header */
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    ਇਹ->ਗਿਣਤੀ_2 = hb_len (it);
    ਜੇ (!ਇਹ->ਗਿਣਤੀ_2) return_trace (ਸੱਚ);
    ਜੇ (unlikely (!c->extend (ਇਹ->off_ਮਾਪ))) return_trace (ਝੂਠ);
    ਇਹ->off_ਮਾਪ = off_ਮਾਪ_2;
    ਜੇ (unlikely (!c->allocate_ਮਾਪ<HBUINT8> (off_ਮਾਪ_2 * (ਇਹ->ਗਿਣਤੀ_2 + 1), ਝੂਠ)))
      return_trace (ਝੂਠ);

    /* serialize indices */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ = 1;
    ਜੇ (HB_OPTIMIZE_ਮਾਪ_VAL)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &_ : +it)
      {
	ਸੈੱਟ_ਵਿੱਥ_at (i++, ਵਿੱਥ);
	ਵਿੱਥ += hb_len_ਦਾ (_);
      }
      ਸੈੱਟ_ਵਿੱਥ_at (i, ਵਿੱਥ);
    }
    ਨਹੀਂਤਾਂ
      ਚੋਣ (off_ਮਾਪ_2)
      {
	ਹਾਲਤ 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &_ : +it)
	  {
	    *p++ = ਵਿੱਥ;
	    ਵਿੱਥ += hb_len_ਦਾ (_);
	  }
	  *p = ਵਿੱਥ;
	}
	ਰੋਕੋ;
	ਹਾਲਤ 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &_ : +it)
	  {
	    *p++ = ਵਿੱਥ;
	    ਵਿੱਥ += hb_len_ਦਾ (_);
	  }
	  *p = ਵਿੱਥ;
	}
	ਰੋਕੋ;
	ਹਾਲਤ 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &_ : +it)
	  {
	    *p++ = ਵਿੱਥ;
	    ਵਿੱਥ += hb_len_ਦਾ (_);
	  }
	  *p = ਵਿੱਥ;
	}
	ਰੋਕੋ;
	ਹਾਲਤ 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &_ : +it)
	  {
	    *p++ = ਵਿੱਥ;
	    ਵਿੱਥ += hb_len_ਦਾ (_);
	  }
	  *p = ਵਿੱਥ;
	}
	ਰੋਕੋ;
	ਮੂਲ:
	ਰੋਕੋ;
      }

    ਪ੍ਰਤਿਜ੍ਞਾ (ਵਿੱਥ == ਅੰਕੜੇ_ਮਾਪ + 1);
    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ਮਾਪ (ਅਚਲ Iterable &iterable, ਬਿਨਾਨਿਸ਼ਾਨੀ *ਅੰਕੜੇ_ਮਾਪ = ਖਾਲੀ_ਸੂਚਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ min_off_ਮਾਪ = 0)
  {
    ਸਵੈਚਾਲਿਤ it = + hb_iter (iterable);
    ਜੇ (!it)
    {
      ਜੇ (ਅੰਕੜੇ_ਮਾਪ) *ਅੰਕੜੇ_ਮਾਪ = 0;
      ਵਾਪਸ min_ਮਾਪ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &_ : +it)
      ਕੁੱਲ += hb_len_ਦਾ (_);

    ਜੇ (ਅੰਕੜੇ_ਮਾਪ) *ਅੰਕੜੇ_ਮਾਪ = ਕੁੱਲ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ off_ਮਾਪ_2 = (hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (ਕੁੱਲ + 1) + 7) / 8;
    off_ਮਾਪ_2 = hb_max(min_off_ਮਾਪ, off_ਮਾਪ_2);

    ਵਾਪਸ min_ਮਾਪ + HBUINT8::static_ਮਾਪ + (hb_len (it) + 1) * off_ਮਾਪ_2 + ਕੁੱਲ;
  }

  ਖਾਲੀ ਸੈੱਟ_ਵਿੱਥ_at (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ)
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (ਸੂਚਕਾਂਕ_2 <= ਗਿਣਤੀ_2);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ = off_ਮਾਪ;
    ਅਚਲ HBUINT8 *p = offsets;
    ਚੋਣ (ਮਾਪ)
    {
      ਹਾਲਤ 1: ((HBUINT8  *) p)[ਸੂਚਕਾਂਕ_2] = ਵਿੱਥ; ਰੋਕੋ;
      ਹਾਲਤ 2: ((HBUINT16 *) p)[ਸੂਚਕਾਂਕ_2] = ਵਿੱਥ; ਰੋਕੋ;
      ਹਾਲਤ 3: ((HBUINT24 *) p)[ਸੂਚਕਾਂਕ_2] = ਵਿੱਥ; ਰੋਕੋ;
      ਹਾਲਤ 4: ((HBUINT32 *) p)[ਸੂਚਕਾਂਕ_2] = ਵਿੱਥ; ਰੋਕੋ;
      ਮੂਲ: ਵਾਪਸ;
    }
  }

  ਨਿਜੀ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ_at (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2) ਅਚਲ
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (ਸੂਚਕਾਂਕ_2 <= ਗਿਣਤੀ_2);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ = off_ਮਾਪ;
    ਅਚਲ HBUINT8 *p = offsets;
    ਚੋਣ (ਮਾਪ)
    {
      ਹਾਲਤ 1: ਵਾਪਸ ((HBUINT8  *) p)[ਸੂਚਕਾਂਕ_2];
      ਹਾਲਤ 2: ਵਾਪਸ ((HBUINT16 *) p)[ਸੂਚਕਾਂਕ_2];
      ਹਾਲਤ 3: ਵਾਪਸ ((HBUINT24 *) p)[ਸੂਚਕਾਂਕ_2];
      ਹਾਲਤ 4: ਵਾਪਸ ((HBUINT32 *) p)[ਸੂਚਕਾਂਕ_2];
      ਮੂਲ: ਵਾਪਸ 0;
    }
  }

  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਅੰਕੜੇ_ਅਧਾਰ () ਅਚਲ
  { ਵਾਪਸ (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) ਇਹ + min_ਮਾਪ + off_ਮਾਪ.static_ਮਾਪ - 1 + ਵਿੱਥ_array_ਮਾਪ (); }
  ਜਨਤਕ:

  hb_ubytes_t ਸੰਚਾਲਕ [] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2) ਅਚਲ
  {
    ਜੇ (unlikely (ਸੂਚਕਾਂਕ_2 >= ਗਿਣਤੀ_2)) ਵਾਪਸ hb_ubytes_t ();
    hb_barrier ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ offset0 = ਵਿੱਥ_at (ਸੂਚਕਾਂਕ_2);
    ਬਿਨਾਨਿਸ਼ਾਨੀ offset1 = ਵਿੱਥ_at (ਸੂਚਕਾਂਕ_2 + 1);
    ਜੇ (unlikely (offset1 < offset0 || offset1 > ਵਿੱਥ_at (ਗਿਣਤੀ_2)))
      ਵਾਪਸ hb_ubytes_t ();
    ਵਾਪਸ hb_ubytes_t (ਅੰਕੜੇ_ਅਧਾਰ () + offset0, offset1 - offset0);
  }

  size_t get_ਮਾਪ () ਅਚਲ
  {
    ਜੇ (ਗਿਣਤੀ_2)
      ਵਾਪਸ hb_unsigned_ਜੋੜੋ_saturate (min_ਮਾਪ, off_ਮਾਪ.static_ਮਾਪ,
				       ਵਿੱਥ_array_ਮਾਪ (), ਵਿੱਥ_at (ਗਿਣਤੀ_2) - 1);
    ਵਾਪਸ min_ਮਾਪ;  /* empty CFFIndex contains count only */
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  hb_barrier () &&
			  (ਗਿਣਤੀ_2 == 0 || /* empty INDEX */
			   (ਗਿਣਤੀ_2 < ਗਿਣਤੀ_2 + 1u &&
			    c->check_struct (&off_ਮਾਪ) && off_ਮਾਪ >= 1 && off_ਮਾਪ <= 4 &&
			    c->check_array (offsets, off_ਮਾਪ, ਗਿਣਤੀ_2 + 1u) &&
			    c->check_range (ਅੰਕੜੇ_ਅਧਾਰ (), ਵਿੱਥ_at (ਗਿਣਤੀ_2))))));
  }

  ਜਨਤਕ:
  ਗਿਣਤੀ_4		ਗਿਣਤੀ_2;		/* Number of object data. Note there are (count+1) offsets */
  ਨਿਜੀ:
  HBUINT8	off_ਮਾਪ;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (ਗਿਣਤੀ_4::static_ਮਾਪ);
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ CFFIndex<HBUINT16> CFF1_ਸੂਚਕਾਂਕ;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ CFFIndex<HBUINT32> CFF2_ਸੂਚਕਾਂਕ;


/* TupleValues */
ਬਣਤਰ Tuple_ਮੁੱਲਾਂ
{
  ਗਿਣਤੀ packed_ਮੁੱਲ_flag_t
  {
    ਮੁੱਲਾਂ_ARE_ZEROS     = 0x80,
    ਮੁੱਲਾਂ_ARE_ਬਾਈਟਾਂ     = 0x00,
    ਮੁੱਲਾਂ_ARE_ਸ਼ਬਦ     = 0x40,
    ਮੁੱਲਾਂ_ARE_LONGS     = 0xC0,
    ਮੁੱਲਾਂ_ਮਾਪ_ਨਕਾਬ     = 0xC0,
    ਮੁੱਲ_ਲੜੀਵਾਰ_ਗਿਣਤੀ_ਨਕਾਬ = 0x3F
  };

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ compile_unsafe (hb_array_t<ਅਚਲ ਪੂਰਨ> values, /*ਵਿੱਚ*/
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *encoded_ਬਾਈਟਾਂ /*ਬਾਹਰਲਾ*/)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਮੁੱਲਾਂ_3 = values.ਲੰਮਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ encoded_len = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
    ਜਦੋਂ (i < ਅੰਕ_ਮੁੱਲਾਂ_3)
    {
      ਪੂਰਨ val = values.arrayZ[i];
      ਜੇ (val == 0)
        encoded_len += encode_ਮੁੱਲ_ਲੜੀਵਾਰ_as_zeroes (i, encoded_ਬਾਈਟਾਂ + encoded_len, values);
      ਨਹੀਂਤਾਂ ਜੇ ((int8_t) val == val)
        encoded_len += encode_ਮੁੱਲ_ਲੜੀਵਾਰ_as_ਬਾਈਟਾਂ (i, encoded_ਬਾਈਟਾਂ + encoded_len, values);
      ਨਹੀਂਤਾਂ ਜੇ ((int16_t) val == val)
        encoded_len += encode_ਮੁੱਲ_ਲੜੀਵਾਰ_as_ਸ਼ਬਦ (i, encoded_ਬਾਈਟਾਂ + encoded_len, values);
      ਨਹੀਂਤਾਂ
        encoded_len += encode_ਮੁੱਲ_ਲੜੀਵਾਰ_as_longs (i, encoded_ਬਾਈਟਾਂ + encoded_len, values);
    }
    ਵਾਪਸ encoded_len;
  }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ encode_ਮੁੱਲ_ਲੜੀਵਾਰ_as_zeroes (ਬਿਨਾਨਿਸ਼ਾਨੀ& i,
					      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *it,
					      hb_array_t<ਅਚਲ ਪੂਰਨ> values)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਮੁੱਲਾਂ_3 = values.ਲੰਮਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੜੀਵਾਰ_ਲੰਮਾਈ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ encoded_len = 0;
    ਜਦੋਂ (i < ਅੰਕ_ਮੁੱਲਾਂ_3 && values.arrayZ[i] == 0)
    {
      i++;
      ਲੜੀਵਾਰ_ਲੰਮਾਈ++;
    }

    ਜਦੋਂ (ਲੜੀਵਾਰ_ਲੰਮਾਈ >= 64)
    {
      *it++ = ਅੱਖਰ (ਮੁੱਲਾਂ_ARE_ZEROS | 63);
      ਲੜੀਵਾਰ_ਲੰਮਾਈ -= 64;
      encoded_len++;
    }

    ਜੇ (ਲੜੀਵਾਰ_ਲੰਮਾਈ)
    {
      *it++ = ਅੱਖਰ (ਮੁੱਲਾਂ_ARE_ZEROS | (ਲੜੀਵਾਰ_ਲੰਮਾਈ - 1));
      encoded_len++;
    }
    ਵਾਪਸ encoded_len;
  }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ encode_ਮੁੱਲ_ਲੜੀਵਾਰ_as_ਬਾਈਟਾਂ (ਬਿਨਾਨਿਸ਼ਾਨੀ &i,
					     ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *it,
					     hb_array_t<ਅਚਲ ਪੂਰਨ> values)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = i;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਮੁੱਲਾਂ_3 = values.ਲੰਮਾਈ;
    ਜਦੋਂ (i < ਅੰਕ_ਮੁੱਲਾਂ_3)
    {
      ਪੂਰਨ val = values.arrayZ[i];
      ਜੇ ((int8_t) val != val)
        ਰੋਕੋ;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      ਜੇ (val == 0 && i + 1 < ਅੰਕ_ਮੁੱਲਾਂ_3 && values.arrayZ[i+1] == 0)
        ਰੋਕੋ;

      i++;
    }
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੜੀਵਾਰ_ਲੰਮਾਈ = i - ਸ਼ੁਰੂ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ encoded_len = 0;

    ਜਦੋਂ (ਲੜੀਵਾਰ_ਲੰਮਾਈ >= 64)
    {
      *it++ = (ਮੁੱਲਾਂ_ARE_ਬਾਈਟਾਂ | 63);
      encoded_len++;

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < 64; j++)
	it[j] = static_cast<ਅੱਖਰ> (values.arrayZ[ਸ਼ੁਰੂ + j]);
      it += 64;
      encoded_len += 64;

      ਸ਼ੁਰੂ += 64;
      ਲੜੀਵਾਰ_ਲੰਮਾਈ -= 64;
    }

    ਜੇ (ਲੜੀਵਾਰ_ਲੰਮਾਈ)
    {
      *it++ = (ਮੁੱਲਾਂ_ARE_ਬਾਈਟਾਂ | (ਲੜੀਵਾਰ_ਲੰਮਾਈ - 1));
      encoded_len++;

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < ਲੜੀਵਾਰ_ਲੰਮਾਈ; j++)
        it[j] = static_cast<ਅੱਖਰ> (values.arrayZ[ਸ਼ੁਰੂ + j]);
      encoded_len += ਲੜੀਵਾਰ_ਲੰਮਾਈ;
    }

    ਵਾਪਸ encoded_len;
  }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ encode_ਮੁੱਲ_ਲੜੀਵਾਰ_as_ਸ਼ਬਦ (ਬਿਨਾਨਿਸ਼ਾਨੀ &i,
					     ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *it,
					     hb_array_t<ਅਚਲ ਪੂਰਨ> values)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = i;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਮੁੱਲਾਂ_3 = values.ਲੰਮਾਈ;
    ਜਦੋਂ (i < ਅੰਕ_ਮੁੱਲਾਂ_3)
    {
      ਪੂਰਨ val = values.arrayZ[i];

      ਜੇ ((int16_t) val != val)
        ਰੋਕੋ;

      /* start a new run for a single zero value. */
      ਜੇ (val == 0) ਰੋਕੋ;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      ਜੇ ((int8_t) val == val &&
          i + 1 < ਅੰਕ_ਮੁੱਲਾਂ_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        ਰੋਕੋ;

      i++;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੜੀਵਾਰ_ਲੰਮਾਈ = i - ਸ਼ੁਰੂ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ encoded_len = 0;
    ਜਦੋਂ (ਲੜੀਵਾਰ_ਲੰਮਾਈ >= 64)
    {
      *it++ = (ਮੁੱਲਾਂ_ARE_ਸ਼ਬਦ | 63);
      encoded_len++;

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < 64; j++)
      {
        int16_t ਮੁੱਲ_val = values.arrayZ[ਸ਼ੁਰੂ + j];
        *it++ = static_cast<ਅੱਖਰ> (ਮੁੱਲ_val >> 8);
        *it++ = static_cast<ਅੱਖਰ> (ਮੁੱਲ_val & 0xFF);

        encoded_len += 2;
      }

      ਸ਼ੁਰੂ += 64;
      ਲੜੀਵਾਰ_ਲੰਮਾਈ -= 64;
    }

    ਜੇ (ਲੜੀਵਾਰ_ਲੰਮਾਈ)
    {
      *it++ = (ਮੁੱਲਾਂ_ARE_ਸ਼ਬਦ | (ਲੜੀਵਾਰ_ਲੰਮਾਈ - 1));
      encoded_len++;
      ਜਦੋਂ (ਸ਼ੁਰੂ < i)
      {
        int16_t ਮੁੱਲ_val = values.arrayZ[ਸ਼ੁਰੂ++];
        *it++ = static_cast<ਅੱਖਰ> (ਮੁੱਲ_val >> 8);
        *it++ = static_cast<ਅੱਖਰ> (ਮੁੱਲ_val & 0xFF);

        encoded_len += 2;
      }
    }
    ਵਾਪਸ encoded_len;
  }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ encode_ਮੁੱਲ_ਲੜੀਵਾਰ_as_longs (ਬਿਨਾਨਿਸ਼ਾਨੀ &i,
					     ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *it,
					     hb_array_t<ਅਚਲ ਪੂਰਨ> values)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = i;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਮੁੱਲਾਂ_3 = values.ਲੰਮਾਈ;
    ਜਦੋਂ (i < ਅੰਕ_ਮੁੱਲਾਂ_3)
    {
      ਪੂਰਨ val = values.arrayZ[i];

      ਜੇ ((int16_t) val == val)
        ਰੋਕੋ;

      i++;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੜੀਵਾਰ_ਲੰਮਾਈ = i - ਸ਼ੁਰੂ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ encoded_len = 0;
    ਜਦੋਂ (ਲੜੀਵਾਰ_ਲੰਮਾਈ >= 64)
    {
      *it++ = (ਮੁੱਲਾਂ_ARE_LONGS | 63);
      encoded_len++;

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < 64; j++)
      {
        int32_t ਮੁੱਲ_val = values.arrayZ[ਸ਼ੁਰੂ + j];
        *it++ = static_cast<ਅੱਖਰ> (ਮੁੱਲ_val >> 24);
        *it++ = static_cast<ਅੱਖਰ> (ਮੁੱਲ_val >> 16);
        *it++ = static_cast<ਅੱਖਰ> (ਮੁੱਲ_val >> 8);
        *it++ = static_cast<ਅੱਖਰ> (ਮੁੱਲ_val & 0xFF);

        encoded_len += 4;
      }

      ਸ਼ੁਰੂ += 64;
      ਲੜੀਵਾਰ_ਲੰਮਾਈ -= 64;
    }

    ਜੇ (ਲੜੀਵਾਰ_ਲੰਮਾਈ)
    {
      *it++ = (ਮੁੱਲਾਂ_ARE_LONGS | (ਲੜੀਵਾਰ_ਲੰਮਾਈ - 1));
      encoded_len++;
      ਜਦੋਂ (ਸ਼ੁਰੂ < i)
      {
        int32_t ਮੁੱਲ_val = values.arrayZ[ਸ਼ੁਰੂ++];
        *it++ = static_cast<ਅੱਖਰ> (ਮੁੱਲ_val >> 24);
        *it++ = static_cast<ਅੱਖਰ> (ਮੁੱਲ_val >> 16);
        *it++ = static_cast<ਅੱਖਰ> (ਮੁੱਲ_val >> 8);
        *it++ = static_cast<ਅੱਖਰ> (ਮੁੱਲ_val & 0xFF);

        encoded_len += 4;
      }
    }
    ਵਾਪਸ encoded_len;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  ਸਥਿਰ ਤਾਰਕਿਕ decompile (ਅਚਲ HBUINT8 *&p /* IN/OUT */,
			 hb_ਸਦਿਸ਼_t<T> &values /* IN/OUT */,
			 ਅਚਲ HBUINT8 *ਸਿਰਾ,
			 ਤਾਰਕਿਕ consume_all = ਝੂਠ,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = 0)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = consume_all ? UINT_MAX : values.ਲੰਮਾਈ;
    ਜੇ (consume_all)
      values.alloc ((ਸਿਰਾ - p) / 2);
    ਜਦੋਂ (i < ਗਿਣਤੀ_2)
    {
      ਜੇ (unlikely (p + 1 > ਸਿਰਾ)) ਵਾਪਸ consume_all;
      ਬਿਨਾਨਿਸ਼ਾਨੀ control = *p++;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੜੀਵਾਰ_ਗਿਣਤੀ = (control & ਮੁੱਲ_ਲੜੀਵਾਰ_ਗਿਣਤੀ_ਨਕਾਬ) + 1;
      ਜੇ (consume_all)
      {
        ਜੇ (unlikely (!values.resize_dirty  (values.ਲੰਮਾਈ + ਲੜੀਵਾਰ_ਗਿਣਤੀ)))
	  ਵਾਪਸ ਝੂਠ;
      }
      ਬਿਨਾਨਿਸ਼ਾਨੀ stop = i + ਲੜੀਵਾਰ_ਗਿਣਤੀ;
      ਜੇ (unlikely (stop > ਗਿਣਤੀ_2)) ਵਾਪਸ ਝੂਠ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ skip = i < ਸ਼ੁਰੂ ? hb_min (ਸ਼ੁਰੂ - i, ਲੜੀਵਾਰ_ਗਿਣਤੀ) : 0;
      i += skip;

      ਜੇ ((control & ਮੁੱਲਾਂ_ਮਾਪ_ਨਕਾਬ) == ਮੁੱਲਾਂ_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * ਆਕਾਰ (T));
	i = stop;
      }
      ਨਹੀਂਤਾਂ ਜੇ ((control & ਮੁੱਲਾਂ_ਮਾਪ_ਨਕਾਬ) ==  ਮੁੱਲਾਂ_ARE_ਸ਼ਬਦ)
      {
        ਜੇ (unlikely (p + ਲੜੀਵਾਰ_ਗਿਣਤੀ * HBINT16::static_ਮਾਪ > ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;
	p += skip * HBINT16::static_ਮਾਪ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
        ਲਈ (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ਅਚਲ HBINT16 *) p;
	  p += HBINT16::static_ਮਾਪ;
	  values.arrayZ[i + 1] = * (ਅਚਲ HBINT16 *) p;
	  p += HBINT16::static_ਮਾਪ;
	  values.arrayZ[i + 2] = * (ਅਚਲ HBINT16 *) p;
	  p += HBINT16::static_ਮਾਪ;
	  values.arrayZ[i + 3] = * (ਅਚਲ HBINT16 *) p;
	  p += HBINT16::static_ਮਾਪ;
	}
#ਜੇ_ਅੰਤ
        ਲਈ (; i < stop; i++)
        {
          values.arrayZ[i] = * (ਅਚਲ HBINT16 *) p;
          p += HBINT16::static_ਮਾਪ;
        }
      }
      ਨਹੀਂਤਾਂ ਜੇ ((control & ਮੁੱਲਾਂ_ਮਾਪ_ਨਕਾਬ) ==  ਮੁੱਲਾਂ_ARE_LONGS)
      {
        ਜੇ (unlikely (p + ਲੜੀਵਾਰ_ਗਿਣਤੀ * HBINT32::static_ਮਾਪ > ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;
	p += skip * HBINT32::static_ਮਾਪ;
        ਲਈ (; i < stop; i++)
        {
          values.arrayZ[i] = * (ਅਚਲ HBINT32 *) p;
          p += HBINT32::static_ਮਾਪ;
        }
      }
      ਨਹੀਂਤਾਂ ਜੇ ((control & ਮੁੱਲਾਂ_ਮਾਪ_ਨਕਾਬ) ==  ਮੁੱਲਾਂ_ARE_ਬਾਈਟਾਂ)
      {
        ਜੇ (unlikely (p + ਲੜੀਵਾਰ_ਗਿਣਤੀ > ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;
	p += skip * HBINT8::static_ਮਾਪ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
	ਲਈ (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ਅਚਲ HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (ਅਚਲ HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (ਅਚਲ HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (ਅਚਲ HBINT8 *) p++;
	}
#ਜੇ_ਅੰਤ
        ਲਈ (; i < stop; i++)
          values.arrayZ[i] = * (ਅਚਲ HBINT8 *) p++;
      }
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਸਥਿਰ ਤਾਰਕਿਕ skip (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *&p /* IN/OUT */,
		    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਸਿਰਾ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਵੇਖਿਆ = 0;
    ਜਦੋਂ (ਵੇਖਿਆ < ਗਿਣਤੀ_2)
    {
      ਜੇ (unlikely (p >= ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ control = *p++;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੜੀਵਾਰ_ਗਿਣਤੀ = (control & ਮੁੱਲ_ਲੜੀਵਾਰ_ਗਿਣਤੀ_ਨਕਾਬ) + 1;
      ਜੇ (unlikely (ਲੜੀਵਾਰ_ਗਿਣਤੀ > ਗਿਣਤੀ_2 - ਵੇਖਿਆ)) ਵਾਪਸ ਝੂਠ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ = 0;
      ਚੋਣ (control & ਮੁੱਲਾਂ_ਮਾਪ_ਨਕਾਬ)
      {
	ਹਾਲਤ ਮੁੱਲਾਂ_ARE_ZEROS: ਰੋਕੋ;
	ਹਾਲਤ ਮੁੱਲਾਂ_ARE_ਬਾਈਟਾਂ: ਚੌੜਾਈ = HBINT8::static_ਮਾਪ; ਰੋਕੋ;
	ਹਾਲਤ ਮੁੱਲਾਂ_ARE_ਸ਼ਬਦ: ਚੌੜਾਈ = HBINT16::static_ਮਾਪ; ਰੋਕੋ;
	ਹਾਲਤ ਮੁੱਲਾਂ_ARE_LONGS: ਚੌੜਾਈ = HBINT32::static_ਮਾਪ; ਰੋਕੋ;
      }
      ਜੇ (unlikely (ਬਿਨਾਨਿਸ਼ਾਨੀ (ਸਿਰਾ - p) < ਲੜੀਵਾਰ_ਗਿਣਤੀ * ਚੌੜਾਈ)) ਵਾਪਸ ਝੂਠ;
      p += ਲੜੀਵਾਰ_ਗਿਣਤੀ * ਚੌੜਾਈ;
      ਵੇਖਿਆ += ਲੜੀਵਾਰ_ਗਿਣਤੀ;
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਬਣਤਰ iter_t : hb_iter_with_fallback_t<iter_t, ਪੂਰਨ>
  {
    iter_t (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *p_, ਬਿਨਾਨਿਸ਼ਾਨੀ len_)
	    : p (p_), endp (p_ + len_)
    { ਜੇ (likely (ensure_ਲੜੀਵਾਰ ())) read_ਮੁੱਲ (); }

    ਨਿਜੀ:
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *p;
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ * ਅਚਲ endp;
    ਪੂਰਨ current_ਮੁੱਲ = 0;
    ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੜੀਵਾਰ_ਗਿਣਤੀ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ = 0;

    HB_ALWAYS_INLINE
    ਤਾਰਕਿਕ ensure_ਲੜੀਵਾਰ ()
    {
      ਜੇ (likely (ਲੜੀਵਾਰ_ਗਿਣਤੀ > 0)) ਵਾਪਸ ਸੱਚ;
      ਵਾਪਸ ensure_ਲੜੀਵਾਰ_2 ();
    }
    ਤਾਰਕਿਕ ensure_ਲੜੀਵਾਰ_2 ()
    {
      ਜੇ (unlikely (p >= endp))
      {
        ਲੜੀਵਾਰ_ਗਿਣਤੀ = 0;
        current_ਮੁੱਲ = 0;
	ਵਾਪਸ ਝੂਠ;
      }

      ਬਿਨਾਨਿਸ਼ਾਨੀ control = *p++;
      ਲੜੀਵਾਰ_ਗਿਣਤੀ = (control & ਮੁੱਲ_ਲੜੀਵਾਰ_ਗਿਣਤੀ_ਨਕਾਬ) + 1;
      ਚੌੜਾਈ = control & ਮੁੱਲਾਂ_ਮਾਪ_ਨਕਾਬ;
      ਚੋਣ (ਚੌੜਾਈ)
      {
        ਹਾਲਤ ਮੁੱਲਾਂ_ARE_ZEROS: ਚੌੜਾਈ = 0; ਰੋਕੋ;
	ਹਾਲਤ ਮੁੱਲਾਂ_ARE_ਬਾਈਟਾਂ: ਚੌੜਾਈ = HBINT8::static_ਮਾਪ;  ਰੋਕੋ;
	ਹਾਲਤ ਮੁੱਲਾਂ_ARE_ਸ਼ਬਦ: ਚੌੜਾਈ = HBINT16::static_ਮਾਪ; ਰੋਕੋ;
	ਹਾਲਤ ਮੁੱਲਾਂ_ARE_LONGS: ਚੌੜਾਈ = HBINT32::static_ਮਾਪ; ਰੋਕੋ;
	ਮੂਲ: ਪ੍ਰਤਿਜ੍ਞਾ (ਝੂਠ);
      }

      ਜੇ (unlikely (p + ਲੜੀਵਾਰ_ਗਿਣਤੀ * ਚੌੜਾਈ > endp))
      {
	ਲੜੀਵਾਰ_ਗਿਣਤੀ = 0;
	current_ਮੁੱਲ = 0;
	ਵਾਪਸ ਝੂਠ;
      }

      ਵਾਪਸ ਸੱਚ;
    }
    ਖਾਲੀ read_ਮੁੱਲ ()
    {
      ਚੋਣ (ਚੌੜਾਈ)
      {
        ਹਾਲਤ 0: current_ਮੁੱਲ = 0; ਰੋਕੋ;
	ਹਾਲਤ 1: current_ਮੁੱਲ = * (ਅਚਲ HBINT8  *) p; ਰੋਕੋ;
	ਹਾਲਤ 2: current_ਮੁੱਲ = * (ਅਚਲ HBINT16 *) p; ਰੋਕੋ;
	ਹਾਲਤ 4: current_ਮੁੱਲ = * (ਅਚਲ HBINT32 *) p; ਰੋਕੋ;
      }
      p += ਚੌੜਾਈ;
    }

    ਜਨਤਕ:

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ __item_t__;
    __item_t__ __item__ () ਅਚਲ
    { ਵਾਪਸ current_ਮੁੱਲ; }

    ਤਾਰਕਿਕ __more__ () ਅਚਲ { ਵਾਪਸ ਲੜੀਵਾਰ_ਗਿਣਤੀ || p < endp; }
    ਖਾਲੀ __next__ ()
    {
      ਲੜੀਵਾਰ_ਗਿਣਤੀ--;
      ਜੇ (unlikely (!ensure_ਲੜੀਵਾਰ ()))
	ਵਾਪਸ;
      read_ਮੁੱਲ ();
    }
    ਖਾਲੀ __forward__ (ਬਿਨਾਨਿਸ਼ਾਨੀ n)
    {
      ਜੇ (unlikely (!ensure_ਲੜੀਵਾਰ ()))
	ਵਾਪਸ;
      ਜਦੋਂ (n)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ i = hb_min (n, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਲੜੀਵਾਰ_ਗਿਣਤੀ);
	ਲੜੀਵਾਰ_ਗਿਣਤੀ -= i;
	n -= i;
	p += (i - 1) * ਚੌੜਾਈ;
	ਜੇ (unlikely (!ensure_ਲੜੀਵਾਰ ()))
	  ਵਾਪਸ;
	read_ਮੁੱਲ ();
      }
    }
    ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ iter_t& o) ਅਚਲ
    { ਵਾਪਸ p != o.p || ਲੜੀਵਾਰ_ਗਿਣਤੀ != o.ਲੜੀਵਾਰ_ਗਿਣਤੀ; }
    iter_t __end__ () ਅਚਲ
    {
      iter_t it (endp, 0);
      ਵਾਪਸ it;
    }
  };

  ਬਣਤਰ fetcher_t
  {
    fetcher_t (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *p_, ਬਿਨਾਨਿਸ਼ਾਨੀ len_)
	      : p (p_), ਸਿਰਾ (p_ + len_) {}

    ਨਿਜੀ:
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *p;
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ * ਅਚਲ ਸਿਰਾ;
    ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੜੀਵਾਰ_ਗਿਣਤੀ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ = 0;

    HB_ALWAYS_INLINE
    ਤਾਰਕਿਕ ensure_ਲੜੀਵਾਰ ()
    {
      ਜੇ (likely (ਲੜੀਵਾਰ_ਗਿਣਤੀ > 0)) ਵਾਪਸ ਸੱਚ;
      ਵਾਪਸ ensure_ਲੜੀਵਾਰ_2 ();
    }

    ਤਾਰਕਿਕ ensure_ਲੜੀਵਾਰ_2 ()
    {
      ਜੇ (unlikely (p >= ਸਿਰਾ))
      {
        ਲੜੀਵਾਰ_ਗਿਣਤੀ = 0;
	ਵਾਪਸ ਝੂਠ;
      }

      ਬਿਨਾਨਿਸ਼ਾਨੀ control = *p++;
      ਲੜੀਵਾਰ_ਗਿਣਤੀ = (control & ਮੁੱਲ_ਲੜੀਵਾਰ_ਗਿਣਤੀ_ਨਕਾਬ) + 1;
      ਚੌੜਾਈ = control & ਮੁੱਲਾਂ_ਮਾਪ_ਨਕਾਬ;
      ਚੋਣ (ਚੌੜਾਈ)
      {
        ਹਾਲਤ ਮੁੱਲਾਂ_ARE_ZEROS: ਚੌੜਾਈ = 0; ਰੋਕੋ;
	ਹਾਲਤ ਮੁੱਲਾਂ_ARE_ਬਾਈਟਾਂ: ਚੌੜਾਈ = HBINT8::static_ਮਾਪ;  ਰੋਕੋ;
	ਹਾਲਤ ਮੁੱਲਾਂ_ARE_ਸ਼ਬਦ: ਚੌੜਾਈ = HBINT16::static_ਮਾਪ; ਰੋਕੋ;
	ਹਾਲਤ ਮੁੱਲਾਂ_ARE_LONGS: ਚੌੜਾਈ = HBINT32::static_ਮਾਪ; ਰੋਕੋ;
	ਮੂਲ: ਪ੍ਰਤਿਜ੍ਞਾ (ਝੂਠ);
      }

      ਜੇ (unlikely (p + ਲੜੀਵਾਰ_ਗਿਣਤੀ * ਚੌੜਾਈ > ਸਿਰਾ))
      {
	ਲੜੀਵਾਰ_ਗਿਣਤੀ = 0;
	ਵਾਪਸ ਝੂਠ;
      }

      ਵਾਪਸ ਸੱਚ;
    }

    ਜਨਤਕ:
    ਖਾਲੀ skip (ਬਿਨਾਨਿਸ਼ਾਨੀ n)
    {
      ਜਦੋਂ (n)
      {
	ਜੇ (unlikely (!ensure_ਲੜੀਵਾਰ ()))
	  ਵਾਪਸ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ i = hb_min (n, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਲੜੀਵਾਰ_ਗਿਣਤੀ);
	ਲੜੀਵਾਰ_ਗਿਣਤੀ -= i;
	n -= i;
	p += i * ਚੌੜਾਈ;
      }
    }

    ਨਿਜੀ:
    ਸਾਂਚਾ <ਤਾਰਕਿਕ scaled, ਕਿਸਮ_ਨਾਂ T>
    ਖਾਲੀ ਜੋੜੋ_ਮੰਜ਼ਿਲ (hb_array_t<T> ਬਾਹਰਲਾ, T scale = T (1))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ n = ਬਾਹਰਲਾ.ਲੰਮਾਈ;
      T *arrayZ = ਬਾਹਰਲਾ.arrayZ;

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n;)
      {
	ਜੇ (unlikely (!ensure_ਲੜੀਵਾਰ ()))
	  ਰੋਕੋ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = hb_min (n - i, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਲੜੀਵਾਰ_ਗਿਣਤੀ);
	ਚੋਣ (ਚੌੜਾਈ)
	{
	  ਹਾਲਤ 0:
	  {
	    arrayZ += ਗਿਣਤੀ_2;
	    ਰੋਕੋ;
	  }
	  ਹਾਲਤ 1:
	  {
	    ਅਚਲ ਸਵੈਚਾਲਿਤ *pp = (ਅਚਲ HBINT8 *) p;
	    ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
	    ਲਈ (; j + 3 < ਗਿਣਤੀ_2; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#ਜੇ_ਅੰਤ
	    ਲਈ (; j < ਗਿਣਤੀ_2; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) pp;
	  }
	  ਰੋਕੋ;
	  ਹਾਲਤ 2:
	  {
	    ਅਚਲ ਸਵੈਚਾਲਿਤ *pp = (ਅਚਲ HBINT16 *) p;
	    ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
	    ਲਈ (; j + 3 < ਗਿਣਤੀ_2; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#ਜੇ_ਅੰਤ
	    ਲਈ (; j < ਗਿਣਤੀ_2; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) pp;
	  }
	  ਰੋਕੋ;
	  ਹਾਲਤ 4:
	  {
	    ਅਚਲ ਸਵੈਚਾਲਿਤ *pp = (ਅਚਲ HBINT32 *) p;
	    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < ਗਿਣਤੀ_2; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) pp;
	  }
	  ਰੋਕੋ;
	}
	ਲੜੀਵਾਰ_ਗਿਣਤੀ -= ਗਿਣਤੀ_2;
	i += ਗਿਣਤੀ_2;
      }
    }

    ਜਨਤਕ:
    ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
    ਖਾਲੀ ਜੋੜੋ_ਮੰਜ਼ਿਲ_2 (hb_array_t<T> ਬਾਹਰਲਾ, ਦਸ਼ਮਲਵ scale = 1.0f)
    {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      ਜੇ (ਝੂਠ && scale == 1.0f)
        ਜੋੜੋ_ਮੰਜ਼ਿਲ<ਝੂਠ> (ਬਾਹਰਲਾ);
      ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        ਜੋੜੋ_ਮੰਜ਼ਿਲ<ਸੱਚ> (ਬਾਹਰਲਾ, (T) scale);
    }
  };
};

ਬਣਤਰ TupleList : CFF2_ਸੂਚਕਾਂਕ
{
  Tuple_ਮੁੱਲਾਂ::iter_t ਸੰਚਾਲਕ [] (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ
  {
    ਸਵੈਚਾਲਿਤ ਬਾਈਟਾਂ = CFF2_ਸੂਚਕਾਂਕ::ਸੰਚਾਲਕ [] (i);
    ਵਾਪਸ Tuple_ਮੁੱਲਾਂ::iter_t (ਬਾਈਟਾਂ.arrayZ, ਬਾਈਟਾਂ.ਲੰਮਾਈ);
  }

  Tuple_ਮੁੱਲਾਂ::fetcher_t fetcher (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ
  {
    ਸਵੈਚਾਲਿਤ ਬਾਈਟਾਂ = CFF2_ਸੂਚਕਾਂਕ::ਸੰਚਾਲਕ [] (i);
    ਵਾਪਸ Tuple_ਮੁੱਲਾਂ::fetcher_t (ਬਾਈਟਾਂ.arrayZ, ਬਾਈਟਾਂ.ਲੰਮਾਈ);
  }
};


// Alignment

ਸਾਂਚਾ <ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ alignment>
ਬਣਤਰ Align
{
  size_t get_ਮਾਪ (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ = (ਅਚਲ ਅੱਖਰ *) ਇਹ - (ਅਚਲ ਅੱਖਰ *) ਅਧਾਰ;
    ਵਾਪਸ (alignment - ਵਿੱਥ) & (alignment - 1);
  }

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (0);
};



} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OPEN_TYPE_HH */
