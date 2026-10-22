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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-blob.hh"
#अन्तर्भाव "hb-face.hh"
#अन्तर्भाव "hb-machinery.hh"
#अन्तर्भाव "hb-meta.hh"
#अन्तर्भाव "hb-subset.hh"


नामाकाश OT {


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
template <तार्किक BE,
	  typename Type,
	  unsigned पूर्णाङ्क Size = परिमाण (Type)>
संरचना अङ्कः_Type
{
  प्रकारनिर्देश Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  प्रकारनिर्देश typename std::conditional<std::is_integral<Type>::value && परिमाण (Type) <= परिमाण(पूर्णाङ्क),
				     typename std::conditional<std::is_signed<Type>::value, signed, unsigned>::type,
				     Type>::type WideType;

  अङ्कः_Type () = स्वभाव;
  explicit constexpr अङ्कः_Type (Type V) : v {V} {}
  अङ्कः_Type& operator = (Type V) { v = V; पटिनिवत्त *एतत्; }

  operator WideType () ध्रुव { पटिनिवत्त v; }

  तार्किक operator == (ध्रुव अङ्कः_Type &o) ध्रुव { पटिनिवत्त (Type) v == (Type) o.v; }
  तार्किक operator != (ध्रुव अङ्कः_Type &o) ध्रुव { पटिनिवत्त !(*एतत् == o); }

  अङ्कः_Type& operator += (WideType गणनम्) { *एतत् = *एतत् + गणनम्; पटिनिवत्त *एतत्; }
  अङ्कः_Type& operator -= (WideType गणनम्) { *एतत् = *एतत् - गणनम्; पटिनिवत्त *एतत्; }
  अङ्कः_Type& operator ++ () { *एतत् += 1; पटिनिवत्त *एतत्; }
  अङ्कः_Type& operator -- () { *एतत् -= 1; पटिनिवत्त *एतत्; }
  अङ्कः_Type operator ++ (पूर्णाङ्क) { अङ्कः_Type c (*एतत्); ++*एतत्; पटिनिवत्त c; }
  अङ्कः_Type operator -- (पूर्णाङ्क) { अङ्कः_Type c (*एतत्); --*एतत्; पटिनिवत्त c; }

  uint32_t hash () ध्रुव { पटिनिवत्त hb_array ((ध्रुव वर्ण *) &v, परिमाण (v)).hash (); }
  HB_INTERNAL स्थिर पूर्णाङ्क cmp (ध्रुव अङ्कः_Type *a, ध्रुव अङ्कः_Type *b)
  { पटिनिवत्त b->cmp (*a); }
  HB_INTERNAL स्थिर पूर्णाङ्क cmp (ध्रुव शून्य *a, ध्रुव शून्य *b)
  {
    अङ्कः_Type *pa = (अङ्कः_Type *) a;
    अङ्कः_Type *pb = (अङ्कः_Type *) b;

    पटिनिवत्त pb->cmp (*pa);
  }
  template <typename Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  पूर्णाङ्क cmp (Type2 a) ध्रुव
  {
    Type b = v;
    पटिनिवत्त (a > b) - (a < b);
  }
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }
  रक्षित:
  typename std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, Size>,
			    HBFloat<BE, Type, Size>>::type v;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (Size);
};

प्रकारनिर्देश अङ्कः_Type<सच्च, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
प्रकारनिर्देश अङ्कः_Type<सच्च, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
प्रकारनिर्देश अङ्कः_Type<सच्च, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
प्रकारनिर्देश अङ्कः_Type<सच्च, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
प्रकारनिर्देश अङ्कः_Type<सच्च, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
प्रकारनिर्देश अङ्कः_Type<सच्च, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
प्रकारनिर्देश अङ्कः_Type<सच्च, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
प्रकारनिर्देश अङ्कः_Type<सच्च, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
प्रकारनिर्देश अङ्कः_Type<सच्च, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

प्रकारनिर्देश अङ्कः_Type<मिच्छा, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
प्रकारनिर्देश अङ्कः_Type<मिच्छा, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
प्रकारनिर्देश अङ्कः_Type<मिच्छा, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
प्रकारनिर्देश अङ्कः_Type<मिच्छा, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
प्रकारनिर्देश अङ्कः_Type<मिच्छा, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
प्रकारनिर्देश अङ्कः_Type<मिच्छा, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

प्रकारनिर्देश अङ्कः_Type<सच्च,  भिन्न>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
प्रकारनिर्देश अङ्कः_Type<सच्च,  द्विगुण> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
प्रकारनिर्देश अङ्कः_Type<मिच्छा, भिन्न>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
प्रकारनिर्देश अङ्कः_Type<मिच्छा, द्विगुण> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
संरचना HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& operator = (uint16_t i ) { HBUINT16::operator= (i); पटिनिवत्त *एतत्; }
  सार्वजनिक:
  DEFINE_SIZE_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
संरचना HBUINT32VAR
{
  size_t get_size () ध्रुव
  {
    unsigned b0 = v[0];
    सचे (b0 < 0x80)
      पटिनिवत्त 1;
    अञ्ञथा सचे (b0 < 0xC0)
      पटिनिवत्त 2;
    अञ्ञथा सचे (b0 < 0xE0)
      पटिनिवत्त 3;
    अञ्ञथा सचे (b0 < 0xF0)
      पटिनिवत्त 4;
    अञ्ञथा
      पटिनिवत्त 5;
  }

  स्थिर size_t get_size (uint32_t v)
  {
    सचे (v < 0x80)
      पटिनिवत्त 1;
    अञ्ञथा सचे (v < 0x4000)
      पटिनिवत्त 2;
    अञ्ञथा सचे (v < 0x200000)
      पटिनिवत्त 3;
    अञ्ञथा सचे (v < 0x10000000)
      पटिनिवत्त 4;
    अञ्ञथा
      पटिनिवत्त 5;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_size ()));
  }

  operator uint32_t () ध्रुव
  {
    unsigned b0 = v[0];
    सचे (b0 < 0x80)
      पटिनिवत्त b0;
    अञ्ञथा सचे (b0 < 0xC0)
      पटिनिवत्त ((b0 & 0x3F) << 8) | v[1];
    अञ्ञथा सचे (b0 < 0xE0)
      पटिनिवत्त ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    अञ्ञथा सचे (b0 < 0xF0)
      पटिनिवत्त ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    अञ्ञथा
      पटिनिवत्त (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  स्थिर unsigned serialize_unsafe (unsigned वर्ण *buf, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned वर्ण *p = buf + len;
    अत्थाय (unsigned i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    सचे (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    पटिनिवत्त len;
  }

  स्थिर तार्किक serialize (hb_serialize_context_t *c, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned वर्ण *buf = c->allocate_size<unsigned वर्ण> (len, मिच्छा);
    सचे (unlikely (!buf))
      पटिनिवत्त मिच्छा;

    serialize_unsafe (buf, v);
    पटिनिवत्त सच्च;
  }

  रक्षित:
  unsigned वर्ण v[5];

  सार्वजनिक:
  DEFINE_SIZE_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
प्रकारनिर्देश HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
प्रकारनिर्देश HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
प्रकारनिर्देश HBUINT16 UFWORD;

template <typename Type, unsigned fraction_bits>
संरचना HBFixed : Type
{
  स्थिर constexpr भिन्न mult = 1.f / (1 << fraction_bits);
  static_assert (Type::static_size * 8 > fraction_bits, "");

  operator signed () ध्रुव = delete;
  operator unsigned () ध्रुव = delete;
  explicit operator भिन्न () ध्रुव { पटिनिवत्त to_float (); }
  typename Type::type to_int () ध्रुव { पटिनिवत्त Type::v; }
  शून्य set_int (typename Type::type i ) { Type::v = i; }
  भिन्न to_float (भिन्न offset = 0) ध्रुव  { पटिनिवत्त ((int32_t) Type::v + offset) * mult; }
  शून्य set_float (भिन्न f) { Type::v = roundf (f / mult); }
  सार्वजनिक:
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
संरचना LONGDATETIME
{
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }
  रक्षित:
  HBINT32 major;
  HBUINT32 minor;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
संरचना Tag : HBUINT32
{
  Tag& operator = (hb_tag_t i) { HBUINT32::operator= (i); पटिनिवत्त *एतत्; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  operator ध्रुव वर्ण* () ध्रुव { पटिनिवत्त reinterpret_cast<ध्रुव वर्ण *> (एतत्); }
  operator वर्ण* ()             { पटिनिवत्त reinterpret_cast<वर्ण *> (एतत्); }
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
संरचना HBGlyphID16 : HBUINT16
{
  HBGlyphID16& operator = (uint16_t i) { HBUINT16::operator= (i); पटिनिवत्त *एतत्; }
};
संरचना HBGlyphID24 : HBUINT24
{
  HBGlyphID24& operator = (uint32_t i) { HBUINT24::operator= (i); पटिनिवत्त *एतत्; }
};

/* Script/language-system/feature index */
संरचना Index : HBUINT16 {
  स्थिर constexpr unsigned NOT_FOUND_INDEX = 0xFFFFu;
  Index& operator = (uint16_t i) { HBUINT16::operator= (i); पटिनिवत्त *एतत्; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, Index);

प्रकारनिर्देश Index NameID;

संरचना VarIdx : HBUINT32 {
  स्थिर constexpr unsigned NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_INDEX, "");
  VarIdx& operator = (uint32_t i) { HBUINT32::operator= (i); पटिनिवत्त *एतत्; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, VarIdx);

/* Offset, Null offset = 0 */
template <typename Type, तार्किक has_null=सच्च>
संरचना Offset : Type
{
  Offset& operator = (typename Type::type i) { Type::operator= (i); पटिनिवत्त *एतत्; }

  प्रकारनिर्देश Type type;

  तार्किक is_null () ध्रुव { पटिनिवत्त has_null && 0 == *एतत्; }

  सार्वजनिक:
  DEFINE_SIZE_STATIC (परिमाण (Type));
};

प्रकारनिर्देश Offset<HBUINT16> Offset16;
प्रकारनिर्देश Offset<HBUINT24> Offset24;
प्रकारनिर्देश Offset<HBUINT32> Offset32;


/* CheckSum */
संरचना CheckSum : HBUINT32
{
  CheckSum& operator = (uint32_t i) { HBUINT32::operator= (i); पटिनिवत्त *एतत्; }

  /* This is reference implementation from the spec. */
  स्थिर uint32_t CalcTableChecksum (ध्रुव HBUINT32 *Table, uint32_t Length)
  {
    uint32_t Sum = 0L;
    प्रतिज्ञा (0 == (Length & 3));
    ध्रुव HBUINT32 *EndPtr = Table + Length / HBUINT32::static_size;

    याव (Table < EndPtr)
      Sum += *Table++;
    पटिनिवत्त Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  शून्य set_for_data (ध्रुव शून्य *data, unsigned पूर्णाङ्क length)
  { *एतत् = CalcTableChecksum ((ध्रुव HBUINT32 *) data, length); }

  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};


/*
 * Version Numbers
 */

template <typename FixedType=HBUINT16>
संरचना FixedVersion
{
  uint32_t to_int () ध्रुव { पटिनिवत्त (major << (परिमाण (FixedType) * 8)) + minor; }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  FixedType major;
  FixedType minor;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (2 * परिमाण (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

template <typename Type, तार्किक has_null>
संरचना _hb_has_null
{
  स्थिर ध्रुव Type *get_null () { पटिनिवत्त nullptr; }
  स्थिर Type *get_crap ()       { पटिनिवत्त nullptr; }
};
template <typename Type>
संरचना _hb_has_null<Type, सच्च>
{
  स्थिर ध्रुव Type *get_null () { पटिनिवत्त &Null (Type); }
  स्थिर       Type *get_crap () { पटिनिवत्त &Crap (Type); }
};

template <typename Type, typename OffsetType, typename BaseType=शून्य, तार्किक has_null=सच्च>
संरचना OffsetTo : Offset<OffsetType, has_null>
{
  using target_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == मिच्छा ||
		 (hb_has_null_size (Type) || !hb_has_min_size (Type)), "");

  HB_DELETE_COPY_ASSIGN (OffsetTo);
  OffsetTo () = स्वभाव;

  OffsetTo& operator = (typename OffsetType::type i) { OffsetType::operator= (i); पटिनिवत्त *एतत्; }

  ध्रुव Type& operator () (ध्रुव शून्य *base) ध्रुव
  {
    सचे (unlikely (एतत्->is_null ())) पटिनिवत्त *_hb_has_null<Type, has_null>::get_null ();
    पटिनिवत्त StructAtOffset<ध्रुव Type> (base, *एतत्);
  }
  Type& operator () (शून्य *base) ध्रुव
  {
    सचे (unlikely (एतत्->is_null ())) पटिनिवत्त *_hb_has_null<Type, has_null>::get_crap ();
    पटिनिवत्त StructAtOffset<Type> (base, *एतत्);
  }

  template <typename Base,
	    hb_enable_if (hb_is_convertible (ध्रुव Base, ध्रुव BaseType *))>
  friend ध्रुव Type& operator + (ध्रुव Base &base, ध्रुव OffsetTo &offset) { पटिनिवत्त offset ((ध्रुव शून्य *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (ध्रुव Base, ध्रुव BaseType *))>
  friend ध्रुव Type& operator + (ध्रुव OffsetTo &offset, ध्रुव Base &base) { पटिनिवत्त offset ((ध्रुव शून्य *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (Base &&base, OffsetTo &offset) { पटिनिवत्त offset ((शून्य *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (OffsetTo &offset, Base &&base) { पटिनिवत्त offset ((शून्य *) base); }


  template <typename Base, typename ...Ts>
  तार्किक serialize_subset (hb_subset_context_t *c, ध्रुव OffsetTo& src,
			 ध्रुव Base *src_base, Ts&&... ds)
  {
    *एतत् = 0;
    सचे (src.is_null ())
      पटिनिवत्त मिच्छा;

    auto *s = c->serializer;

    s->push ();

    तार्किक ret = c->dispatch (src_base+src, std::forward<Ts> (ds)...);

    सचे (ret || !has_null)
      s->add_link (*एतत्, s->निष्कासय_pack ());
    अञ्ञथा
      s->निष्कासय_discard ();

    पटिनिवत्त ret;
  }


  template <typename ...Ts>
  तार्किक serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *एतत् = 0;

    Type* obj = c->push<Type> ();
    तार्किक ret = obj->serialize (c, std::forward<Ts> (ds)...);

    सचे (ret)
      c->add_link (*एतत्, c->निष्कासय_pack ());
    अञ्ञथा
      c->निष्कासय_discard ();

    पटिनिवत्त ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  template <typename ...Ts>
  तार्किक serialize_copy (hb_serialize_context_t *c, ध्रुव OffsetTo& src,
		       ध्रुव शून्य *src_base, unsigned dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *एतत् = 0;
    सचे (src.is_null ())
      पटिनिवत्त मिच्छा;

    c->push ();

    तार्किक ret = c->copy (src_base+src, std::forward<Ts> (ds)...);

    c->add_link (*एतत्, c->निष्कासय_pack (), whence, dst_bias);

    पटिनिवत्त ret;
  }

  तार्किक serialize_copy (hb_serialize_context_t *c, ध्रुव OffsetTo& src,
		       ध्रुव शून्य *src_base, unsigned dst_bias = 0)
  { पटिनिवत्त serialize_copy (c, src, src_base, dst_bias, hb_serialize_context_t::Head); }

  तार्किक sanitize_shallow (hb_sanitize_context_t *c, ध्रुव BaseType *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (unlikely (!c->check_struct (एतत्))) return_trace (मिच्छा);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    सचे (unlikely ((ध्रुव वर्ण *) base + (unsigned) *एतत् < (ध्रुव वर्ण *) base)) return_trace (मिच्छा);
    return_trace (सच्च);
  }

  template <typename ...Ts>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव BaseType *base, Ts&&... ds) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (sanitize_shallow (c, base) &&
		  hb_barrier () &&
		  (एतत्->is_null () ||
		   c->dispatch (StructAtOffset<Type> (base, *एतत्), std::forward<Ts> (ds)...)));
  }

  DEFINE_SIZE_STATIC (परिमाण (OffsetType));
};
/* Partial specializations. */
template <typename Type, typename BaseType=शून्य, तार्किक has_null=सच्च> using Offset16To = OffsetTo<Type, HBUINT16, BaseType, has_null>;
template <typename Type, typename BaseType=शून्य, तार्किक has_null=सच्च> using Offset24To = OffsetTo<Type, HBUINT24, BaseType, has_null>;
template <typename Type, typename BaseType=शून्य, तार्किक has_null=सच्च> using Offset32To = OffsetTo<Type, HBUINT32, BaseType, has_null>;

template <typename Type, typename OffsetType, typename BaseType=शून्य> using NNOffsetTo = OffsetTo<Type, OffsetType, BaseType, मिच्छा>;
template <typename Type, typename BaseType=शून्य> using NNOffset16To = Offset16To<Type, BaseType, मिच्छा>;
template <typename Type, typename BaseType=शून्य> using NNOffset24To = Offset24To<Type, BaseType, मिच्छा>;
template <typename Type, typename BaseType=शून्य> using NNOffset32To = Offset32To<Type, BaseType, मिच्छा>;


/*
 * Array Types
 */

template <typename Type>
संरचना UnsizedArrayOf
{
  प्रकारनिर्देश Type item_t;
  स्थिर constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (UnsizedArrayOf);

  ध्रुव Type& operator [] (unsigned पूर्णाङ्क i) ध्रुव
  {
    पटिनिवत्त arrayZ[i];
  }
  Type& operator [] (unsigned पूर्णाङ्क i)
  {
    पटिनिवत्त arrayZ[i];
  }

  स्थिर size_t get_size (unsigned पूर्णाङ्क len)
  { पटिनिवत्त hb_unsigned_mul_saturate (len, Type::static_size); }

  template <typename T> operator T * () { पटिनिवत्त arrayZ; }
  template <typename T> operator ध्रुव T * () ध्रुव { पटिनिवत्त arrayZ; }
  hb_array_t<Type> as_array (unsigned पूर्णाङ्क len)
  { पटिनिवत्त hb_array (arrayZ, len); }
  hb_array_t<ध्रुव Type> as_array (unsigned पूर्णाङ्क len) ध्रुव
  { पटिनिवत्त hb_array (arrayZ, len); }

  template <typename T>
  Type &lsearch (unsigned पूर्णाङ्क len, ध्रुव T &x, Type &not_found = Crap (Type))
  { पटिनिवत्त *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  ध्रुव Type &lsearch (unsigned पूर्णाङ्क len, ध्रुव T &x, ध्रुव Type &not_found = Null (Type)) ध्रुव
  { पटिनिवत्त *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  तार्किक lfind (unsigned पूर्णाङ्क len, ध्रुव T &x, unsigned पूर्णाङ्क *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned पूर्णाङ्क to_store = (unsigned पूर्णाङ्क) -1) ध्रुव
  { पटिनिवत्त as_array (len).lfind (x, i, not_found, to_store); }

  शून्य qsort (unsigned पूर्णाङ्क len, unsigned पूर्णाङ्क start = 0, unsigned पूर्णाङ्क end = (unsigned पूर्णाङ्क) -1)
  { as_array (len).qsort (start, end); }

  तार्किक serialize (hb_serialize_context_t *c, unsigned पूर्णाङ्क वस्तूनि_len, तार्किक clear = सच्च)
  {
    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (!c->extend_size (एतत्, get_size (वस्तूनि_len), clear))) return_trace (मिच्छा);
    return_trace (सच्च);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  तार्किक serialize (hb_serialize_context_t *c, Iterator वस्तूनि)
  {
    TRACE_SERIALIZE (एतत्);
    unsigned गणनम् = hb_len (वस्तूनि);
    सचे (unlikely (!serialize (c, गणनम्, मिच्छा))) return_trace (मिच्छा);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    अत्थाय (unsigned i = 0; i < गणनम्; i++, ++वस्तूनि)
      arrayZ[i] = *वस्तूनि;
    return_trace (सच्च);
  }

  UnsizedArrayOf* copy (hb_serialize_context_t *c, unsigned गणनम्) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto *out = c->start_embed (एतत्);
    सचे (unlikely (!as_array (गणनम्).copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  तार्किक sanitize (hb_sanitize_context_t *c, unsigned पूर्णाङ्क गणनम्, Ts&&... ds) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (unlikely (!sanitize_shallow (c, गणनम्))) return_trace (मिच्छा);
    सचे (!परिमाण... (Ts) && hb_is_trivially_copyable(Type)) return_trace (सच्च);
    hb_barrier ();
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      सचे (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (मिच्छा);
    return_trace (सच्च);
  }

  तार्किक sanitize_shallow (hb_sanitize_context_t *c, unsigned पूर्णाङ्क गणनम्) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_array (arrayZ, गणनम्));
  }

  सार्वजनिक:
  Type		arrayZ[HB_VAR_ARRAY];
  सार्वजनिक:
  DEFINE_SIZE_UNBOUNDED (0);
};

/* Unsized array of offset's */
template <typename Type, typename OffsetType, typename BaseType=शून्य, तार्किक has_null=सच्च>
using UnsizedArray16OfOffsetTo = UnsizedArrayOf<OffsetTo<Type, OffsetType, BaseType, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
template <typename Type, typename OffsetType, typename BaseType=शून्य, तार्किक has_null=सच्च>
संरचना UnsizedListOfOffset16To : UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
{
  ध्रुव Type& operator [] (पूर्णाङ्क i_) ध्रुव
  {
    unsigned पूर्णाङ्क i = (unsigned पूर्णाङ्क) i_;
    ध्रुव OffsetTo<Type, OffsetType, BaseType, has_null> *p = &एतत्->arrayZ[i];
    सचे (unlikely ((ध्रुव शून्य *) p < (ध्रुव शून्य *) एतत्->arrayZ)) पटिनिवत्त Null (Type); /* Overflowed. */
    hb_barrier ();
    पटिनिवत्त एतत्+*p;
  }
  Type& operator [] (पूर्णाङ्क i_)
  {
    unsigned पूर्णाङ्क i = (unsigned पूर्णाङ्क) i_;
    ध्रुव OffsetTo<Type, OffsetType, BaseType, has_null> *p = &एतत्->arrayZ[i];
    सचे (unlikely ((ध्रुव शून्य *) p < (ध्रुव शून्य *) एतत्->arrayZ)) पटिनिवत्त Crap (Type); /* Overflowed. */
    hb_barrier ();
    पटिनिवत्त एतत्+*p;
  }

  template <typename ...Ts>
  तार्किक sanitize (hb_sanitize_context_t *c, unsigned पूर्णाङ्क गणनम्, Ts&&... ds) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace ((UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
		   ::sanitize (c, गणनम्, एतत्, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type>
संरचना SortedUnsizedArrayOf : UnsizedArrayOf<Type>
{
  hb_sorted_array_t<Type> as_array (unsigned पूर्णाङ्क len)
  { पटिनिवत्त hb_sorted_array (एतत्->arrayZ, len); }
  hb_sorted_array_t<ध्रुव Type> as_array (unsigned पूर्णाङ्क len) ध्रुव
  { पटिनिवत्त hb_sorted_array (एतत्->arrayZ, len); }
  operator hb_sorted_array_t<Type> ()             { पटिनिवत्त as_array (); }
  operator hb_sorted_array_t<ध्रुव Type> () ध्रुव { पटिनिवत्त as_array (); }

  template <typename T>
  Type &bsearch (unsigned पूर्णाङ्क len, ध्रुव T &x, Type &not_found = Crap (Type))
  { पटिनिवत्त *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  ध्रुव Type &bsearch (unsigned पूर्णाङ्क len, ध्रुव T &x, ध्रुव Type &not_found = Null (Type)) ध्रुव
  { पटिनिवत्त *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  तार्किक bfind (unsigned पूर्णाङ्क len, ध्रुव T &x, unsigned पूर्णाङ्क *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned पूर्णाङ्क to_store = (unsigned पूर्णाङ्क) -1) ध्रुव
  { पटिनिवत्त as_array (len).bfind (x, i, not_found, to_store); }
};


/* An array with a number of elements. */
template <typename Type, typename LenType>
संरचना ArrayOf
{
  प्रकारनिर्देश Type item_t;
  स्थिर constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOf);

  ध्रुव Type& operator [] (पूर्णाङ्क i_) ध्रुव
  {
    unsigned पूर्णाङ्क i = (unsigned पूर्णाङ्क) i_;
    सचे (unlikely (i >= len)) पटिनिवत्त Null (Type);
    hb_barrier ();
    पटिनिवत्त arrayZ[i];
  }
  Type& operator [] (पूर्णाङ्क i_)
  {
    unsigned पूर्णाङ्क i = (unsigned पूर्णाङ्क) i_;
    सचे (unlikely (i >= len)) पटिनिवत्त Crap (Type);
    hb_barrier ();
    पटिनिवत्त arrayZ[i];
  }

  size_t get_size () ध्रुव
  { पटिनिवत्त hb_unsigned_mul_add_saturate (len, Type::static_size, len.static_size); }

  explicit operator तार्किक () ध्रुव { पटिनिवत्त len; }

  शून्य निष्कासय () { len--; }

  hb_array_t<      Type> as_array ()       { पटिनिवत्त hb_array (arrayZ, len); }
  hb_array_t<ध्रुव Type> as_array () ध्रुव { पटिनिवत्त hb_array (arrayZ, len); }

  /* Iterator. */
  प्रकारनिर्देश hb_array_t<ध्रुव Type>   iter_t;
  प्रकारनिर्देश hb_array_t<      Type> writer_t;
    iter_t   iter () ध्रुव { पटिनिवत्त as_array (); }
  writer_t writer ()       { पटिनिवत्त as_array (); }
  operator   iter_t () ध्रुव { पटिनिवत्त   iter (); }
  operator writer_t ()       { पटिनिवत्त writer (); }

  /* Faster range-based for loop. */
  ध्रुव Type *begin () ध्रुव { पटिनिवत्त arrayZ; }
  ध्रुव Type *end () ध्रुव { पटिनिवत्त arrayZ + len; }

  template <typename T>
  Type &lsearch (ध्रुव T &x, Type &not_found = Crap (Type))
  { पटिनिवत्त *as_array ().lsearch (x, &not_found); }
  template <typename T>
  ध्रुव Type &lsearch (ध्रुव T &x, ध्रुव Type &not_found = Null (Type)) ध्रुव
  { पटिनिवत्त *as_array ().lsearch (x, &not_found); }
  template <typename T>
  तार्किक lfind (ध्रुव T &x, unsigned पूर्णाङ्क *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned पूर्णाङ्क to_store = (unsigned पूर्णाङ्क) -1) ध्रुव
  { पटिनिवत्त as_array ().lfind (x, i, not_found, to_store); }

  शून्य qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD तार्किक serialize (hb_serialize_context_t *c, unsigned वस्तूनि_len, तार्किक clear = सच्च)
  {
    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छा);
    c->check_assign (len, वस्तूनि_len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    सचे (unlikely (!c->extend_size (एतत्, get_size (), clear))) return_trace (मिच्छा);
    return_trace (सच्च);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD तार्किक serialize (hb_serialize_context_t *c, Iterator वस्तूनि)
  {
    TRACE_SERIALIZE (एतत्);
    unsigned गणनम् = hb_len (वस्तूनि);
    सचे (unlikely (!serialize (c, गणनम्, मिच्छा))) return_trace (मिच्छा);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    अत्थाय (unsigned i = 0; i < गणनम्; i++, ++वस्तूनि)
      arrayZ[i] = *वस्तूनि;
    return_trace (सच्च);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (एतत्);
    len++;
    सचे (unlikely (!len || !c->extend (एतत्)))
    {
      len--;
      return_trace (nullptr);
    }
    return_trace (&arrayZ[len - 1]);
  }

  ArrayOf* copy (hb_serialize_context_t *c) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto *out = c->start_embed (एतत्);
    सचे (unlikely (!c->extend_min (out))) return_trace (nullptr);
    c->check_assign (out->len, len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    सचे (unlikely (!as_array ().copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (unlikely (!sanitize_shallow (c))) return_trace (मिच्छा);
    सचे (!परिमाण... (Ts) && hb_is_trivially_copyable(Type)) return_trace (सच्च);
    hb_barrier ();
    unsigned पूर्णाङ्क गणनम् = len;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      सचे (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (मिच्छा);
    return_trace (सच्च);
  }

  तार्किक sanitize_shallow (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (len.sanitize (c) &&
		  hb_barrier () &&
		  c->check_array_sized (arrayZ, len, परिमाण (LenType)));
  }

  सार्वजनिक:
  LenType	len;
  Type		arrayZ[HB_VAR_ARRAY];
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (परिमाण (LenType), arrayZ);
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
संरचना List16OfOffsetTo : ArrayOf<OffsetTo<Type, OffsetType>, HBUINT16>
{
  ध्रुव Type& operator [] (पूर्णाङ्क i_) ध्रुव
  {
    unsigned पूर्णाङ्क i = (unsigned पूर्णाङ्क) i_;
    सचे (unlikely (i >= एतत्->len)) पटिनिवत्त Null (Type);
    hb_barrier ();
    पटिनिवत्त एतत्+एतत्->arrayZ[i];
  }
  ध्रुव Type& operator [] (पूर्णाङ्क i_)
  {
    unsigned पूर्णाङ्क i = (unsigned पूर्णाङ्क) i_;
    सचे (unlikely (i >= एतत्->len)) पटिनिवत्त Crap (Type);
    hb_barrier ();
    पटिनिवत्त एतत्+एतत्->arrayZ[i];
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    संरचना List16OfOffsetTo *out = c->serializer->embed (*एतत्);
    सचे (unlikely (!out)) return_trace (मिच्छा);
    unsigned पूर्णाङ्क गणनम् = एतत्->len;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      out->arrayZ[i].serialize_subset (c, एतत्->arrayZ[i], एतत्, out);
    return_trace (सच्च);
  }

  template <typename ...Ts>
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace ((Array16Of<OffsetTo<Type, OffsetType>>::sanitize (c, एतत्, std::forward<Ts> (ds)...)));
  }
};

template <typename Type>
using List16OfOffset16To = List16OfOffsetTo<Type, HBUINT16>;

/* An array starting at second element. */
template <typename Type, typename LenType>
संरचना HeadlessArrayOf
{
  स्थिर constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (HeadlessArrayOf);

  ध्रुव Type& operator [] (पूर्णाङ्क i_) ध्रुव
  {
    unsigned पूर्णाङ्क i = (unsigned पूर्णाङ्क) i_;
    सचे (unlikely (i >= lenP1 || !i)) पटिनिवत्त Null (Type);
    hb_barrier ();
    पटिनिवत्त arrayZ[i-1];
  }
  Type& operator [] (पूर्णाङ्क i_)
  {
    unsigned पूर्णाङ्क i = (unsigned पूर्णाङ्क) i_;
    सचे (unlikely (i >= lenP1 || !i)) पटिनिवत्त Crap (Type);
    hb_barrier ();
    पटिनिवत्त arrayZ[i-1];
  }
  size_t get_size () ध्रुव
  { पटिनिवत्त hb_unsigned_mul_add_saturate (get_length (), Type::static_size, lenP1.static_size); }

  unsigned get_length () ध्रुव { पटिनिवत्त lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { पटिनिवत्त hb_array (arrayZ, get_length ()); }
  hb_array_t<ध्रुव Type> as_array () ध्रुव { पटिनिवत्त hb_array (arrayZ, get_length ()); }

  /* Iterator. */
  प्रकारनिर्देश hb_array_t<ध्रुव Type>   iter_t;
  प्रकारनिर्देश hb_array_t<      Type> writer_t;
    iter_t   iter () ध्रुव { पटिनिवत्त as_array (); }
  writer_t writer ()       { पटिनिवत्त as_array (); }
  operator   iter_t () ध्रुव { पटिनिवत्त   iter (); }
  operator writer_t ()       { पटिनिवत्त writer (); }

  /* Faster range-based for loop. */
  ध्रुव Type *begin () ध्रुव { पटिनिवत्त arrayZ; }
  ध्रुव Type *end () ध्रुव { पटिनिवत्त arrayZ + get_length (); }

  HB_NODISCARD तार्किक serialize (hb_serialize_context_t *c, unsigned पूर्णाङ्क वस्तूनि_len, तार्किक clear = सच्च)
  {
    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छा);
    c->check_assign (lenP1, वस्तूनि_len + 1, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    सचे (unlikely (!c->extend_size (एतत्, get_size (), clear))) return_trace (मिच्छा);
    return_trace (सच्च);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD तार्किक serialize (hb_serialize_context_t *c, Iterator वस्तूनि)
  {
    TRACE_SERIALIZE (एतत्);
    unsigned गणनम् = hb_len (वस्तूनि);
    सचे (unlikely (!serialize (c, गणनम्, मिच्छा))) return_trace (मिच्छा);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    अत्थाय (unsigned i = 0; i < गणनम्; i++, ++वस्तूनि)
      arrayZ[i] = *वस्तूनि;
    return_trace (सच्च);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (unlikely (!sanitize_shallow (c))) return_trace (मिच्छा);
    सचे (!परिमाण... (Ts) && hb_is_trivially_copyable(Type)) return_trace (सच्च);
    hb_barrier ();
    unsigned पूर्णाङ्क गणनम् = get_length ();
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      सचे (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (मिच्छा);
    return_trace (सच्च);
  }

  गुप्त:
  तार्किक sanitize_shallow (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (lenP1.sanitize (c) &&
		  hb_barrier () &&
		  (!lenP1 || c->check_array_sized (arrayZ, lenP1 - 1, परिमाण (LenType))));
  }

  सार्वजनिक:
  LenType	lenP1;
  Type		arrayZ[HB_VAR_ARRAY];
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (परिमाण (LenType), arrayZ);
};
template <typename Type> using HeadlessArray16Of = HeadlessArrayOf<Type, HBUINT16>;

/* An array storing length-1. */
template <typename Type, typename LenType=HBUINT16>
संरचना ArrayOfM1
{
  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOfM1);

  ध्रुव Type& operator [] (पूर्णाङ्क i_) ध्रुव
  {
    unsigned पूर्णाङ्क i = (unsigned पूर्णाङ्क) i_;
    सचे (unlikely (i > lenM1)) पटिनिवत्त Null (Type);
    hb_barrier ();
    पटिनिवत्त arrayZ[i];
  }
  Type& operator [] (पूर्णाङ्क i_)
  {
    unsigned पूर्णाङ्क i = (unsigned पूर्णाङ्क) i_;
    सचे (unlikely (i > lenM1)) पटिनिवत्त Crap (Type);
    hb_barrier ();
    पटिनिवत्त arrayZ[i];
  }
  size_t get_size () ध्रुव
  { पटिनिवत्त hb_unsigned_mul_add_saturate (lenM1 + 1, Type::static_size, lenM1.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (unlikely (!sanitize_shallow (c))) return_trace (मिच्छा);
    सचे (!परिमाण... (Ts) && hb_is_trivially_copyable(Type)) return_trace (सच्च);
    hb_barrier ();
    unsigned पूर्णाङ्क गणनम् = lenM1 + 1;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      सचे (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (मिच्छा);
    return_trace (सच्च);
  }

  गुप्त:
  तार्किक sanitize_shallow (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (lenM1.sanitize (c) &&
		  hb_barrier () &&
		  (c->check_array_sized (arrayZ, lenM1 + 1, परिमाण (LenType))));
  }

  सार्वजनिक:
  LenType	lenM1;
  Type		arrayZ[HB_VAR_ARRAY];
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (परिमाण (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type, typename LenType>
संरचना SortedArrayOf : ArrayOf<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { पटिनिवत्त hb_sorted_array (एतत्->arrayZ, एतत्->len); }
  hb_sorted_array_t<ध्रुव Type> as_array () ध्रुव { पटिनिवत्त hb_sorted_array (एतत्->arrayZ, एतत्->len); }

  /* Iterator. */
  प्रकारनिर्देश hb_sorted_array_t<ध्रुव Type>   iter_t;
  प्रकारनिर्देश hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () ध्रुव { पटिनिवत्त as_array (); }
  writer_t writer ()       { पटिनिवत्त as_array (); }
  operator   iter_t () ध्रुव { पटिनिवत्त   iter (); }
  operator writer_t ()       { पटिनिवत्त writer (); }

  /* Faster range-based for loop. */
  ध्रुव Type *begin () ध्रुव { पटिनिवत्त एतत्->arrayZ; }
  ध्रुव Type *end () ध्रुव { पटिनिवत्त एतत्->arrayZ + एतत्->len; }

  तार्किक serialize (hb_serialize_context_t *c, unsigned पूर्णाङ्क वस्तूनि_len)
  {
    TRACE_SERIALIZE (एतत्);
    तार्किक ret = ArrayOf<Type, LenType>::serialize (c, वस्तूनि_len);
    return_trace (ret);
  }
  template <typename Iterator,
	    hb_requires (hb_is_sorted_source_of (Iterator, Type))>
  तार्किक serialize (hb_serialize_context_t *c, Iterator वस्तूनि)
  {
    TRACE_SERIALIZE (एतत्);
    तार्किक ret = ArrayOf<Type, LenType>::serialize (c, वस्तूनि);
    return_trace (ret);
  }

  SortedArrayOf* copy (hb_serialize_context_t *c) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    SortedArrayOf* out = reinterpret_cast<SortedArrayOf *> (ArrayOf<Type, LenType>::copy (c));
    return_trace (out);
  }

  template <typename T>
  Type &bsearch (ध्रुव T &x, Type &not_found = Crap (Type))
  { पटिनिवत्त *as_array ().bsearch (x, &not_found); }
  template <typename T>
  ध्रुव Type &bsearch (ध्रुव T &x, ध्रुव Type &not_found = Null (Type)) ध्रुव
  { पटिनिवत्त *as_array ().bsearch (x, &not_found); }
  template <typename T>
  तार्किक bfind (ध्रुव T &x, unsigned पूर्णाङ्क *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned पूर्णाङ्क to_store = (unsigned पूर्णाङ्क) -1) ध्रुव
  { पटिनिवत्त as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type> using SortedArray16Of = SortedArrayOf<Type, HBUINT16>;
template <typename Type> using SortedArray24Of = SortedArrayOf<Type, HBUINT24>;
template <typename Type> using SortedArray32Of = SortedArrayOf<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

template <typename LenType=HBUINT16>
संरचना Bin_अन्वेषणम्_Header
{
  operator uint32_t () ध्रुव { पटिनिवत्त len; }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  Bin_अन्वेषणम्_Header& operator = (unsigned पूर्णाङ्क v)
  {
    len = v;
    प्रतिज्ञा (len == v);
    entrySelector = hb_max (1u, hb_bit_storage (v)) - 1;
    अन्वेषणम्_Range = 16 * (1u << entrySelector);
    rangeShift = v * 16 > अन्वेषणम्_Range
		 ? 16 * v - अन्वेषणम्_Range
		 : 0;
    पटिनिवत्त *एतत्;
  }

  रक्षित:
  LenType	len;
  LenType	अन्वेषणम्_Range;
  LenType	entrySelector;
  LenType	rangeShift;

  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

template <typename Type, typename LenType=HBUINT16>
using Bin_अन्वेषणम्_Array_Of = SortedArrayOf<Type, Bin_अन्वेषणम्_Header<LenType>>;


संरचना Var_Sized_Bin_अन्वेषणम्_Header
{

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  HBUINT16	unitSize;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	अन्वेषणम्_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	entrySelector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	rangeShift;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (10);
};

template <typename Type>
संरचना Var_Sized_Bin_अन्वेषणम्_Array_Of
{
  स्थिर constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (Var_Sized_Bin_अन्वेषणम्_Array_Of);

  तार्किक last_is_terminator () ध्रुव
  {
    सचे (unlikely (!header.nUnits)) पटिनिवत्त मिच्छा;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    ध्रुव HBUINT16 *शब्दाः = &StructAtOffset<HBUINT16> (&bytesZ, (header.nUnits - 1) * header.unitSize);
    unsigned पूर्णाङ्क गणनम् = Type::Termination_Word_गणनम्;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      सचे (शब्दाः[i] != 0xFFFFu)
	पटिनिवत्त मिच्छा;
    पटिनिवत्त सच्च;
  }

  ध्रुव Type& operator [] (पूर्णाङ्क i_) ध्रुव
  {
    unsigned पूर्णाङ्क i = (unsigned पूर्णाङ्क) i_;
    सचे (unlikely (i >= get_length ())) पटिनिवत्त Null (Type);
    hb_barrier ();
    पटिनिवत्त StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  Type& operator [] (पूर्णाङ्क i_)
  {
    unsigned पूर्णाङ्क i = (unsigned पूर्णाङ्क) i_;
    सचे (unlikely (i >= get_length ())) पटिनिवत्त Crap (Type);
    hb_barrier ();
    पटिनिवत्त StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  unsigned पूर्णाङ्क get_length () ध्रुव
  { पटिनिवत्त header.nUnits - last_is_terminator (); }
  size_t get_size () ध्रुव
  { पटिनिवत्त hb_unsigned_mul_add_saturate (header.nUnits, header.unitSize, header.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (unlikely (!sanitize_shallow (c))) return_trace (मिच्छा);
    सचे (!परिमाण... (Ts) && hb_is_trivially_copyable(Type)) return_trace (सच्च);
    hb_barrier ();
    unsigned पूर्णाङ्क गणनम् = get_length ();
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      सचे (unlikely (!(*एतत्)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (मिच्छा);
    return_trace (सच्च);
  }

  template <typename T>
  ध्रुव Type *bsearch (ध्रुव T &key) ध्रुव
  {
    unsigned pos;
    पटिनिवत्त hb_bsearch_impl (&pos,
			    key,
			    (ध्रुव शून्य *) bytesZ,
			    get_length (),
			    header.unitSize,
			    _hb_cmp_method<T, Type>)
	   ? (ध्रुव Type *) (((ध्रुव वर्ण *) &bytesZ) + (pos * header.unitSize))
	   : nullptr;
  }

  गुप्त:
  तार्किक sanitize_shallow (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (header.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_size <= header.unitSize &&
		  c->check_range (bytesZ.arrayZ,
				  header.nUnits,
				  header.unitSize));
  }

  रक्षित:
  Var_Sized_Bin_अन्वेषणम्_Header	header;
  UnsizedArrayOf<HBUINT8>	bytesZ;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (10, bytesZ);
};


/* CFF INDEX */

template <typename गणनम्_3>
संरचना CFFIndex
{
  unsigned पूर्णाङ्क offset_array_size () ध्रुव
  { पटिनिवत्त offSize * (गणनम् + 1); }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव Iterable &iterable,
		  ध्रुव unsigned *p_data_size = nullptr,
                  unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (एतत्);
    unsigned data_size;
    सचे (p_data_size)
      data_size = *p_data_size;
    अञ्ञथा
      total_size (iterable, &data_size);

    auto it = hb_iter (iterable);
    सचे (unlikely (!serialize_header (c, +it, data_size, min_off_size))) return_trace (मिच्छा);
    unsigned वर्ण *ret = c->allocate_size<unsigned वर्ण> (data_size, मिच्छा);
    सचे (unlikely (!ret)) return_trace (मिच्छा);
    unsigned remaining = data_size;
    अत्थाय (ध्रुव auto &_ : +it)
    {
      unsigned len = _.length;

      सचे (!len)
	अनुवर्त;

      सचे (unlikely (len > remaining)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (मिच्छा, HB_SERIALIZE_ERROR_INT_OVERFLOW));
      }

      remaining -= len;

      सचे (len <= 1)
      {
	*ret++ = *_.arrayZ;
	अनुवर्त;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (सच्च);
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize_header (hb_serialize_context_t *c,
			 Iterator it,
			 unsigned data_size,
                         unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (एतत्);

    unsigned off_size = (hb_bit_storage (data_size + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    /* serialize CFFIndex header */
    सचे (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छा);
    एतत्->गणनम् = hb_len (it);
    सचे (!एतत्->गणनम्) return_trace (सच्च);
    सचे (unlikely (!c->extend (एतत्->offSize))) return_trace (मिच्छा);
    एतत्->offSize = off_size;
    सचे (unlikely (!c->allocate_size<HBUINT8> (off_size * (एतत्->गणनम् + 1), मिच्छा)))
      return_trace (मिच्छा);

    /* serialize indices */
    unsigned पूर्णाङ्क offset = 1;
    सचे (HB_OPTIMIZE_SIZE_VAL)
    {
      unsigned पूर्णाङ्क i = 0;
      अत्थाय (ध्रुव auto &_ : +it)
      {
	set_offset_at (i++, offset);
	offset += hb_len_of (_);
      }
      set_offset_at (i, offset);
    }
    अञ्ञथा
      विकल्प (off_size)
      {
	पक्ष 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  अत्थाय (ध्रुव auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	भङ्ग;
	पक्ष 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  अत्थाय (ध्रुव auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	भङ्ग;
	पक्ष 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  अत्थाय (ध्रुव auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	भङ्ग;
	पक्ष 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  अत्थाय (ध्रुव auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	भङ्ग;
	स्वभाव:
	भङ्ग;
      }

    प्रतिज्ञा (offset == data_size + 1);
    return_trace (सच्च);
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  स्थिर unsigned total_size (ध्रुव Iterable &iterable, unsigned *data_size = nullptr, unsigned min_off_size = 0)
  {
    auto it = + hb_iter (iterable);
    सचे (!it)
    {
      सचे (data_size) *data_size = 0;
      पटिनिवत्त min_size;
    }

    unsigned total = 0;
    अत्थाय (ध्रुव auto &_ : +it)
      total += hb_len_of (_);

    सचे (data_size) *data_size = total;

    unsigned off_size = (hb_bit_storage (total + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    पटिनिवत्त min_size + HBUINT8::static_size + (hb_len (it) + 1) * off_size + total;
  }

  शून्य set_offset_at (unsigned पूर्णाङ्क index, unsigned पूर्णाङ्क offset)
  {
    प्रतिज्ञा (index <= गणनम्);

    unsigned पूर्णाङ्क size = offSize;
    ध्रुव HBUINT8 *p = offsets;
    विकल्प (size)
    {
      पक्ष 1: ((HBUINT8  *) p)[index] = offset; भङ्ग;
      पक्ष 2: ((HBUINT16 *) p)[index] = offset; भङ्ग;
      पक्ष 3: ((HBUINT24 *) p)[index] = offset; भङ्ग;
      पक्ष 4: ((HBUINT32 *) p)[index] = offset; भङ्ग;
      स्वभाव: पटिनिवत्त;
    }
  }

  गुप्त:
  unsigned पूर्णाङ्क offset_at (unsigned पूर्णाङ्क index) ध्रुव
  {
    प्रतिज्ञा (index <= गणनम्);

    unsigned पूर्णाङ्क size = offSize;
    ध्रुव HBUINT8 *p = offsets;
    विकल्प (size)
    {
      पक्ष 1: पटिनिवत्त ((HBUINT8  *) p)[index];
      पक्ष 2: पटिनिवत्त ((HBUINT16 *) p)[index];
      पक्ष 3: पटिनिवत्त ((HBUINT24 *) p)[index];
      पक्ष 4: पटिनिवत्त ((HBUINT32 *) p)[index];
      स्वभाव: पटिनिवत्त 0;
    }
  }

  ध्रुव unsigned वर्ण *data_base () ध्रुव
  { पटिनिवत्त (ध्रुव unsigned वर्ण *) एतत् + min_size + offSize.static_size - 1 + offset_array_size (); }
  सार्वजनिक:

  hb_ubytes_t operator [] (unsigned पूर्णाङ्क index) ध्रुव
  {
    सचे (unlikely (index >= गणनम्)) पटिनिवत्त hb_ubytes_t ();
    hb_barrier ();
    unsigned offset0 = offset_at (index);
    unsigned offset1 = offset_at (index + 1);
    सचे (unlikely (offset1 < offset0 || offset1 > offset_at (गणनम्)))
      पटिनिवत्त hb_ubytes_t ();
    पटिनिवत्त hb_ubytes_t (data_base () + offset0, offset1 - offset0);
  }

  size_t get_size () ध्रुव
  {
    सचे (गणनम्)
      पटिनिवत्त hb_unsigned_add_saturate (min_size, offSize.static_size,
				       offset_array_size (), offset_at (गणनम्) - 1);
    पटिनिवत्त min_size;  /* empty CFFIndex contains count only */
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  hb_barrier () &&
			  (गणनम् == 0 || /* empty INDEX */
			   (गणनम् < गणनम् + 1u &&
			    c->check_struct (&offSize) && offSize >= 1 && offSize <= 4 &&
			    c->check_array (offsets, offSize, गणनम् + 1u) &&
			    c->check_range (data_base (), offset_at (गणनम्))))));
  }

  सार्वजनिक:
  गणनम्_3		गणनम्;		/* Number of object data. Note there are (count+1) offsets */
  गुप्त:
  HBUINT8	offSize;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  सार्वजनिक:
  DEFINE_SIZE_MIN (गणनम्_3::static_size);
};
प्रकारनिर्देश CFFIndex<HBUINT16> CFF1Index;
प्रकारनिर्देश CFFIndex<HBUINT32> CFF2Index;


/* TupleValues */
संरचना Tuple_मानानि
{
  गणना packed_मानम्_flag_t
  {
    मानानि_ARE_ZEROS     = 0x80,
    मानानि_ARE_BYTES     = 0x00,
    मानानि_ARE_शब्दाः     = 0x40,
    मानानि_ARE_LONGS     = 0xC0,
    मानानि_SIZE_MASK     = 0xC0,
    मानम्_RUN_गणनम्_MASK = 0x3F
  };

  स्थिर unsigned compile_unsafe (hb_array_t<ध्रुव पूर्णाङ्क> values, /* IN */
				  unsigned वर्ण *encoded_bytes /* OUT */)
  {
    unsigned अङ्कः_मानानि_3 = values.length;
    unsigned encoded_len = 0;
    unsigned i = 0;
    याव (i < अङ्कः_मानानि_3)
    {
      पूर्णाङ्क val = values.arrayZ[i];
      सचे (val == 0)
        encoded_len += encode_मानम्_run_as_zeroes (i, encoded_bytes + encoded_len, values);
      अञ्ञथा सचे ((int8_t) val == val)
        encoded_len += encode_मानम्_run_as_bytes (i, encoded_bytes + encoded_len, values);
      अञ्ञथा सचे ((int16_t) val == val)
        encoded_len += encode_मानम्_run_as_शब्दाः (i, encoded_bytes + encoded_len, values);
      अञ्ञथा
        encoded_len += encode_मानम्_run_as_longs (i, encoded_bytes + encoded_len, values);
    }
    पटिनिवत्त encoded_len;
  }

  स्थिर unsigned encode_मानम्_run_as_zeroes (unsigned& i,
					      unsigned वर्ण *it,
					      hb_array_t<ध्रुव पूर्णाङ्क> values)
  {
    unsigned अङ्कः_मानानि_3 = values.length;
    unsigned run_length = 0;
    unsigned encoded_len = 0;
    याव (i < अङ्कः_मानानि_3 && values.arrayZ[i] == 0)
    {
      i++;
      run_length++;
    }

    याव (run_length >= 64)
    {
      *it++ = वर्ण (मानानि_ARE_ZEROS | 63);
      run_length -= 64;
      encoded_len++;
    }

    सचे (run_length)
    {
      *it++ = वर्ण (मानानि_ARE_ZEROS | (run_length - 1));
      encoded_len++;
    }
    पटिनिवत्त encoded_len;
  }

  स्थिर unsigned encode_मानम्_run_as_bytes (unsigned &i,
					     unsigned वर्ण *it,
					     hb_array_t<ध्रुव पूर्णाङ्क> values)
  {
    unsigned start = i;
    unsigned अङ्कः_मानानि_3 = values.length;
    याव (i < अङ्कः_मानानि_3)
    {
      पूर्णाङ्क val = values.arrayZ[i];
      सचे ((int8_t) val != val)
        भङ्ग;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      सचे (val == 0 && i + 1 < अङ्कः_मानानि_3 && values.arrayZ[i+1] == 0)
        भङ्ग;

      i++;
    }
    unsigned run_length = i - start;

    unsigned encoded_len = 0;

    याव (run_length >= 64)
    {
      *it++ = (मानानि_ARE_BYTES | 63);
      encoded_len++;

      अत्थाय (unsigned j = 0; j < 64; j++)
	it[j] = static_cast<वर्ण> (values.arrayZ[start + j]);
      it += 64;
      encoded_len += 64;

      start += 64;
      run_length -= 64;
    }

    सचे (run_length)
    {
      *it++ = (मानानि_ARE_BYTES | (run_length - 1));
      encoded_len++;

      अत्थाय (unsigned j = 0; j < run_length; j++)
        it[j] = static_cast<वर्ण> (values.arrayZ[start + j]);
      encoded_len += run_length;
    }

    पटिनिवत्त encoded_len;
  }

  स्थिर unsigned encode_मानम्_run_as_शब्दाः (unsigned &i,
					     unsigned वर्ण *it,
					     hb_array_t<ध्रुव पूर्णाङ्क> values)
  {
    unsigned start = i;
    unsigned अङ्कः_मानानि_3 = values.length;
    याव (i < अङ्कः_मानानि_3)
    {
      पूर्णाङ्क val = values.arrayZ[i];

      सचे ((int16_t) val != val)
        भङ्ग;

      /* start a new run for a single zero value. */
      सचे (val == 0) भङ्ग;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      सचे ((int8_t) val == val &&
          i + 1 < अङ्कः_मानानि_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        भङ्ग;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    याव (run_length >= 64)
    {
      *it++ = (मानानि_ARE_शब्दाः | 63);
      encoded_len++;

      अत्थाय (unsigned j = 0; j < 64; j++)
      {
        int16_t मानम्_val = values.arrayZ[start + j];
        *it++ = static_cast<वर्ण> (मानम्_val >> 8);
        *it++ = static_cast<वर्ण> (मानम्_val & 0xFF);

        encoded_len += 2;
      }

      start += 64;
      run_length -= 64;
    }

    सचे (run_length)
    {
      *it++ = (मानानि_ARE_शब्दाः | (run_length - 1));
      encoded_len++;
      याव (start < i)
      {
        int16_t मानम्_val = values.arrayZ[start++];
        *it++ = static_cast<वर्ण> (मानम्_val >> 8);
        *it++ = static_cast<वर्ण> (मानम्_val & 0xFF);

        encoded_len += 2;
      }
    }
    पटिनिवत्त encoded_len;
  }

  स्थिर unsigned encode_मानम्_run_as_longs (unsigned &i,
					     unsigned वर्ण *it,
					     hb_array_t<ध्रुव पूर्णाङ्क> values)
  {
    unsigned start = i;
    unsigned अङ्कः_मानानि_3 = values.length;
    याव (i < अङ्कः_मानानि_3)
    {
      पूर्णाङ्क val = values.arrayZ[i];

      सचे ((int16_t) val == val)
        भङ्ग;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    याव (run_length >= 64)
    {
      *it++ = (मानानि_ARE_LONGS | 63);
      encoded_len++;

      अत्थाय (unsigned j = 0; j < 64; j++)
      {
        int32_t मानम्_val = values.arrayZ[start + j];
        *it++ = static_cast<वर्ण> (मानम्_val >> 24);
        *it++ = static_cast<वर्ण> (मानम्_val >> 16);
        *it++ = static_cast<वर्ण> (मानम्_val >> 8);
        *it++ = static_cast<वर्ण> (मानम्_val & 0xFF);

        encoded_len += 4;
      }

      start += 64;
      run_length -= 64;
    }

    सचे (run_length)
    {
      *it++ = (मानानि_ARE_LONGS | (run_length - 1));
      encoded_len++;
      याव (start < i)
      {
        int32_t मानम्_val = values.arrayZ[start++];
        *it++ = static_cast<वर्ण> (मानम्_val >> 24);
        *it++ = static_cast<वर्ण> (मानम्_val >> 16);
        *it++ = static_cast<वर्ण> (मानम्_val >> 8);
        *it++ = static_cast<वर्ण> (मानम्_val & 0xFF);

        encoded_len += 4;
      }
    }
    पटिनिवत्त encoded_len;
  }

  template <typename T>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  स्थिर तार्किक decompile (ध्रुव HBUINT8 *&p /* IN/OUT */,
			 hb_vector_t<T> &values /* IN/OUT */,
			 ध्रुव HBUINT8 *end,
			 तार्किक consume_all = मिच्छा,
			 unsigned start = 0)
  {
    unsigned i = 0;
    unsigned गणनम् = consume_all ? UINT_MAX : values.length;
    सचे (consume_all)
      values.alloc ((end - p) / 2);
    याव (i < गणनम्)
    {
      सचे (unlikely (p + 1 > end)) पटिनिवत्त consume_all;
      unsigned control = *p++;
      unsigned run_गणनम् = (control & मानम्_RUN_गणनम्_MASK) + 1;
      सचे (consume_all)
      {
        सचे (unlikely (!values.resize_dirty  (values.length + run_गणनम्)))
	  पटिनिवत्त मिच्छा;
      }
      unsigned stop = i + run_गणनम्;
      सचे (unlikely (stop > गणनम्)) पटिनिवत्त मिच्छा;

      unsigned skip = i < start ? hb_min (start - i, run_गणनम्) : 0;
      i += skip;

      सचे ((control & मानानि_SIZE_MASK) == मानानि_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * परिमाण (T));
	i = stop;
      }
      अञ्ञथा सचे ((control & मानानि_SIZE_MASK) ==  मानानि_ARE_शब्दाः)
      {
        सचे (unlikely (p + run_गणनम् * HBINT16::static_size > end)) पटिनिवत्त मिच्छा;
	p += skip * HBINT16::static_size;
#ifndef HB_OPTIMIZE_SIZE
        अत्थाय (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ध्रुव HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 1] = * (ध्रुव HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 2] = * (ध्रुव HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 3] = * (ध्रुव HBINT16 *) p;
	  p += HBINT16::static_size;
	}
#endif
        अत्थाय (; i < stop; i++)
        {
          values.arrayZ[i] = * (ध्रुव HBINT16 *) p;
          p += HBINT16::static_size;
        }
      }
      अञ्ञथा सचे ((control & मानानि_SIZE_MASK) ==  मानानि_ARE_LONGS)
      {
        सचे (unlikely (p + run_गणनम् * HBINT32::static_size > end)) पटिनिवत्त मिच्छा;
	p += skip * HBINT32::static_size;
        अत्थाय (; i < stop; i++)
        {
          values.arrayZ[i] = * (ध्रुव HBINT32 *) p;
          p += HBINT32::static_size;
        }
      }
      अञ्ञथा सचे ((control & मानानि_SIZE_MASK) ==  मानानि_ARE_BYTES)
      {
        सचे (unlikely (p + run_गणनम् > end)) पटिनिवत्त मिच्छा;
	p += skip * HBINT8::static_size;
#ifndef HB_OPTIMIZE_SIZE
	अत्थाय (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ध्रुव HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (ध्रुव HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (ध्रुव HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (ध्रुव HBINT8 *) p++;
	}
#endif
        अत्थाय (; i < stop; i++)
          values.arrayZ[i] = * (ध्रुव HBINT8 *) p++;
      }
    }
    पटिनिवत्त सच्च;
  }

  स्थिर तार्किक skip (ध्रुव unsigned वर्ण *&p /* IN/OUT */,
		    ध्रुव unsigned वर्ण *end,
		    unsigned गणनम्)
  {
    unsigned seen = 0;
    याव (seen < गणनम्)
    {
      सचे (unlikely (p >= end)) पटिनिवत्त मिच्छा;
      unsigned control = *p++;
      unsigned run_गणनम् = (control & मानम्_RUN_गणनम्_MASK) + 1;
      सचे (unlikely (run_गणनम् > गणनम् - seen)) पटिनिवत्त मिच्छा;

      unsigned width = 0;
      विकल्प (control & मानानि_SIZE_MASK)
      {
	पक्ष मानानि_ARE_ZEROS: भङ्ग;
	पक्ष मानानि_ARE_BYTES: width = HBINT8::static_size; भङ्ग;
	पक्ष मानानि_ARE_शब्दाः: width = HBINT16::static_size; भङ्ग;
	पक्ष मानानि_ARE_LONGS: width = HBINT32::static_size; भङ्ग;
      }
      सचे (unlikely (unsigned (end - p) < run_गणनम् * width)) पटिनिवत्त मिच्छा;
      p += run_गणनम् * width;
      seen += run_गणनम्;
    }
    पटिनिवत्त सच्च;
  }

  संरचना iter_t : hb_iter_with_fallback_t<iter_t, पूर्णाङ्क>
  {
    iter_t (ध्रुव unsigned वर्ण *p_, unsigned len_)
	    : p (p_), endp (p_ + len_)
    { सचे (likely (ensure_run ())) read_मानम् (); }

    गुप्त:
    ध्रुव unsigned वर्ण *p;
    ध्रुव unsigned वर्ण * ध्रुव endp;
    पूर्णाङ्क current_मानम् = 0;
    signed run_गणनम् = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    तार्किक ensure_run ()
    {
      सचे (likely (run_गणनम् > 0)) पटिनिवत्त सच्च;
      पटिनिवत्त _ensure_run ();
    }
    तार्किक _ensure_run ()
    {
      सचे (unlikely (p >= endp))
      {
        run_गणनम् = 0;
        current_मानम् = 0;
	पटिनिवत्त मिच्छा;
      }

      unsigned control = *p++;
      run_गणनम् = (control & मानम्_RUN_गणनम्_MASK) + 1;
      width = control & मानानि_SIZE_MASK;
      विकल्प (width)
      {
        पक्ष मानानि_ARE_ZEROS: width = 0; भङ्ग;
	पक्ष मानानि_ARE_BYTES: width = HBINT8::static_size;  भङ्ग;
	पक्ष मानानि_ARE_शब्दाः: width = HBINT16::static_size; भङ्ग;
	पक्ष मानानि_ARE_LONGS: width = HBINT32::static_size; भङ्ग;
	स्वभाव: प्रतिज्ञा (मिच्छा);
      }

      सचे (unlikely (p + run_गणनम् * width > endp))
      {
	run_गणनम् = 0;
	current_मानम् = 0;
	पटिनिवत्त मिच्छा;
      }

      पटिनिवत्त सच्च;
    }
    शून्य read_मानम् ()
    {
      विकल्प (width)
      {
        पक्ष 0: current_मानम् = 0; भङ्ग;
	पक्ष 1: current_मानम् = * (ध्रुव HBINT8  *) p; भङ्ग;
	पक्ष 2: current_मानम् = * (ध्रुव HBINT16 *) p; भङ्ग;
	पक्ष 4: current_मानम् = * (ध्रुव HBINT32 *) p; भङ्ग;
      }
      p += width;
    }

    सार्वजनिक:

    प्रकारनिर्देश पूर्णाङ्क __item_t__;
    __item_t__ __item__ () ध्रुव
    { पटिनिवत्त current_मानम्; }

    तार्किक __more__ () ध्रुव { पटिनिवत्त run_गणनम् || p < endp; }
    शून्य __next__ ()
    {
      run_गणनम्--;
      सचे (unlikely (!ensure_run ()))
	पटिनिवत्त;
      read_मानम् ();
    }
    शून्य __forward__ (unsigned n)
    {
      सचे (unlikely (!ensure_run ()))
	पटिनिवत्त;
      याव (n)
      {
	unsigned i = hb_min (n, (unsigned) run_गणनम्);
	run_गणनम् -= i;
	n -= i;
	p += (i - 1) * width;
	सचे (unlikely (!ensure_run ()))
	  पटिनिवत्त;
	read_मानम् ();
      }
    }
    तार्किक operator != (ध्रुव iter_t& o) ध्रुव
    { पटिनिवत्त p != o.p || run_गणनम् != o.run_गणनम्; }
    iter_t __end__ () ध्रुव
    {
      iter_t it (endp, 0);
      पटिनिवत्त it;
    }
  };

  संरचना fetcher_t
  {
    fetcher_t (ध्रुव unsigned वर्ण *p_, unsigned len_)
	      : p (p_), end (p_ + len_) {}

    गुप्त:
    ध्रुव unsigned वर्ण *p;
    ध्रुव unsigned वर्ण * ध्रुव end;
    signed run_गणनम् = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    तार्किक ensure_run ()
    {
      सचे (likely (run_गणनम् > 0)) पटिनिवत्त सच्च;
      पटिनिवत्त _ensure_run ();
    }

    तार्किक _ensure_run ()
    {
      सचे (unlikely (p >= end))
      {
        run_गणनम् = 0;
	पटिनिवत्त मिच्छा;
      }

      unsigned control = *p++;
      run_गणनम् = (control & मानम्_RUN_गणनम्_MASK) + 1;
      width = control & मानानि_SIZE_MASK;
      विकल्प (width)
      {
        पक्ष मानानि_ARE_ZEROS: width = 0; भङ्ग;
	पक्ष मानानि_ARE_BYTES: width = HBINT8::static_size;  भङ्ग;
	पक्ष मानानि_ARE_शब्दाः: width = HBINT16::static_size; भङ्ग;
	पक्ष मानानि_ARE_LONGS: width = HBINT32::static_size; भङ्ग;
	स्वभाव: प्रतिज्ञा (मिच्छा);
      }

      सचे (unlikely (p + run_गणनम् * width > end))
      {
	run_गणनम् = 0;
	पटिनिवत्त मिच्छा;
      }

      पटिनिवत्त सच्च;
    }

    सार्वजनिक:
    शून्य skip (unsigned n)
    {
      याव (n)
      {
	सचे (unlikely (!ensure_run ()))
	  पटिनिवत्त;
	unsigned i = hb_min (n, (unsigned) run_गणनम्);
	run_गणनम् -= i;
	n -= i;
	p += i * width;
      }
    }

    गुप्त:
    template <तार्किक scaled, typename T>
    शून्य _add_to (hb_array_t<T> out, T scale = T (1))
    {
      unsigned n = out.length;
      T *arrayZ = out.arrayZ;

      अत्थाय (unsigned i = 0; i < n;)
      {
	सचे (unlikely (!ensure_run ()))
	  भङ्ग;
	unsigned गणनम् = hb_min (n - i, (unsigned) run_गणनम्);
	विकल्प (width)
	{
	  पक्ष 0:
	  {
	    arrayZ += गणनम्;
	    भङ्ग;
	  }
	  पक्ष 1:
	  {
	    ध्रुव auto *pp = (ध्रुव HBINT8 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    अत्थाय (; j + 3 < गणनम्; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    अत्थाय (; j < गणनम्; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ध्रुव unsigned वर्ण *) pp;
	  }
	  भङ्ग;
	  पक्ष 2:
	  {
	    ध्रुव auto *pp = (ध्रुव HBINT16 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    अत्थाय (; j + 3 < गणनम्; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    अत्थाय (; j < गणनम्; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ध्रुव unsigned वर्ण *) pp;
	  }
	  भङ्ग;
	  पक्ष 4:
	  {
	    ध्रुव auto *pp = (ध्रुव HBINT32 *) p;
	    अत्थाय (unsigned j = 0; j < गणनम्; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ध्रुव unsigned वर्ण *) pp;
	  }
	  भङ्ग;
	}
	run_गणनम् -= गणनम्;
	i += गणनम्;
      }
    }

    सार्वजनिक:
    template <typename T>
    शून्य add_to (hb_array_t<T> out, भिन्न scale = 1.0f)
    {
#ifndef HB_OPTIMIZE_SIZE
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      सचे (मिच्छा && scale == 1.0f)
        _add_to<मिच्छा> (out);
      अञ्ञथा
#endif
        _add_to<सच्च> (out, (T) scale);
    }
  };
};

संरचना TupleList : CFF2Index
{
  Tuple_मानानि::iter_t operator [] (unsigned i) ध्रुव
  {
    auto bytes = CFF2Index::operator [] (i);
    पटिनिवत्त Tuple_मानानि::iter_t (bytes.arrayZ, bytes.length);
  }

  Tuple_मानानि::fetcher_t fetcher (unsigned i) ध्रुव
  {
    auto bytes = CFF2Index::operator [] (i);
    पटिनिवत्त Tuple_मानानि::fetcher_t (bytes.arrayZ, bytes.length);
  }
};


// Alignment

template <unsigned पूर्णाङ्क alignment>
संरचना Align
{
  size_t get_size (ध्रुव शून्य *base) ध्रुव
  {
    unsigned offset = (ध्रुव वर्ण *) एतत् - (ध्रुव वर्ण *) base;
    पटिनिवत्त (alignment - offset) & (alignment - 1);
  }

  सार्वजनिक:
  DEFINE_SIZE_MIN (0);
};



} /* namespace OT */


#endif /* HB_OPEN_TYPE_HH */
