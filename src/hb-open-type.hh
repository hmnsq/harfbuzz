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
ढाँचा <तार्किक BE,
	  typename Type,
	  अचिन्हित पूर्णाङ्क Size = परिमाण (Type)>
संरचना अङ्क_Type
{
  प्रकारनिर्देश Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  प्रकारनिर्देश typename std::conditional<std::is_integral<Type>::value && परिमाण (Type) <= परिमाण(पूर्णाङ्क),
				     typename std::conditional<std::is_signed<Type>::value, चिन्हित, अचिन्हित>::type,
				     Type>::type WideType;

  अङ्क_Type () = स्वभाव;
  explicit constexpr अङ्क_Type (Type V) : v {V} {}
  अङ्क_Type& चालक = (Type V) { v = V; फर्काऊ *एतत्; }

  चालक WideType () अचर { फर्काऊ v; }

  तार्किक चालक == (अचर अङ्क_Type &o) अचर { फर्काऊ (Type) v == (Type) o.v; }
  तार्किक चालक != (अचर अङ्क_Type &o) अचर { फर्काऊ !(*एतत् == o); }

  अङ्क_Type& चालक += (WideType गन्ती) { *एतत् = *एतत् + गन्ती; फर्काऊ *एतत्; }
  अङ्क_Type& चालक -= (WideType गन्ती) { *एतत् = *एतत् - गन्ती; फर्काऊ *एतत्; }
  अङ्क_Type& चालक ++ () { *एतत् += 1; फर्काऊ *एतत्; }
  अङ्क_Type& चालक -- () { *एतत् -= 1; फर्काऊ *एतत्; }
  अङ्क_Type चालक ++ (पूर्णाङ्क) { अङ्क_Type c (*एतत्); ++*एतत्; फर्काऊ c; }
  अङ्क_Type चालक -- (पूर्णाङ्क) { अङ्क_Type c (*एतत्); --*एतत्; फर्काऊ c; }

  uint32_t hash () अचर { फर्काऊ hb_array ((अचर वर्ण *) &v, परिमाण (v)).hash (); }
  HB_INTERNAL स्थिर पूर्णाङ्क cmp (अचर अङ्क_Type *a, अचर अङ्क_Type *b)
  { फर्काऊ b->cmp (*a); }
  HB_INTERNAL स्थिर पूर्णाङ्क cmp (अचर शून्य *a, अचर शून्य *b)
  {
    अङ्क_Type *pa = (अङ्क_Type *) a;
    अङ्क_Type *pb = (अङ्क_Type *) b;

    फर्काऊ pb->cmp (*pa);
  }
  ढाँचा <typename Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  पूर्णाङ्क cmp (Type2 a) अचर
  {
    Type b = v;
    फर्काऊ (a > b) - (a < b);
  }
  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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

प्रकारनिर्देश अङ्क_Type<सत्य, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
प्रकारनिर्देश अङ्क_Type<सत्य, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
प्रकारनिर्देश अङ्क_Type<सत्य, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
प्रकारनिर्देश अङ्क_Type<सत्य, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
प्रकारनिर्देश अङ्क_Type<सत्य, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
प्रकारनिर्देश अङ्क_Type<सत्य, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
प्रकारनिर्देश अङ्क_Type<सत्य, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
प्रकारनिर्देश अङ्क_Type<सत्य, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
प्रकारनिर्देश अङ्क_Type<सत्य, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

प्रकारनिर्देश अङ्क_Type<असत्य, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
प्रकारनिर्देश अङ्क_Type<असत्य, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
प्रकारनिर्देश अङ्क_Type<असत्य, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
प्रकारनिर्देश अङ्क_Type<असत्य, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
प्रकारनिर्देश अङ्क_Type<असत्य, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
प्रकारनिर्देश अङ्क_Type<असत्य, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

प्रकारनिर्देश अङ्क_Type<सत्य,  भिन्न>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
प्रकारनिर्देश अङ्क_Type<सत्य,  द्विगुण> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
प्रकारनिर्देश अङ्क_Type<असत्य, भिन्न>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
प्रकारनिर्देश अङ्क_Type<असत्य, द्विगुण> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
संरचना HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& चालक = (uint16_t i ) { HBUINT16::चालक= (i); फर्काऊ *एतत्; }
  सार्वजनिक:
  DEFINE_SIZE_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
संरचना HBUINT32VAR
{
  माप_प्रकार get_size () अचर
  {
    अचिन्हित b0 = v[0];
    यदि (b0 < 0x80)
      फर्काऊ 1;
    अन्यथा यदि (b0 < 0xC0)
      फर्काऊ 2;
    अन्यथा यदि (b0 < 0xE0)
      फर्काऊ 3;
    अन्यथा यदि (b0 < 0xF0)
      फर्काऊ 4;
    अन्यथा
      फर्काऊ 5;
  }

  स्थिर माप_प्रकार get_size (uint32_t v)
  {
    यदि (v < 0x80)
      फर्काऊ 1;
    अन्यथा यदि (v < 0x4000)
      फर्काऊ 2;
    अन्यथा यदि (v < 0x200000)
      फर्काऊ 3;
    अन्यथा यदि (v < 0x10000000)
      फर्काऊ 4;
    अन्यथा
      फर्काऊ 5;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_size ()));
  }

  चालक uint32_t () अचर
  {
    अचिन्हित b0 = v[0];
    यदि (b0 < 0x80)
      फर्काऊ b0;
    अन्यथा यदि (b0 < 0xC0)
      फर्काऊ ((b0 & 0x3F) << 8) | v[1];
    अन्यथा यदि (b0 < 0xE0)
      फर्काऊ ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    अन्यथा यदि (b0 < 0xF0)
      फर्काऊ ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    अन्यथा
      फर्काऊ (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  स्थिर अचिन्हित serialize_unsafe (अचिन्हित वर्ण *buf, uint32_t v)
  {
    अचिन्हित len = get_size (v);
    अचिन्हित वर्ण *p = buf + len;
    लागि (अचिन्हित i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    यदि (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    फर्काऊ len;
  }

  स्थिर तार्किक serialize (hb_serialize_context_t *c, uint32_t v)
  {
    अचिन्हित len = get_size (v);
    अचिन्हित वर्ण *buf = c->allocate_size<अचिन्हित वर्ण> (len, असत्य);
    यदि (unlikely (!buf))
      फर्काऊ असत्य;

    serialize_unsafe (buf, v);
    फर्काऊ सत्य;
  }

  रक्षित:
  अचिन्हित वर्ण v[5];

  सार्वजनिक:
  DEFINE_SIZE_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
प्रकारनिर्देश HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
प्रकारनिर्देश HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
प्रकारनिर्देश HBUINT16 UFWORD;

ढाँचा <typename Type, अचिन्हित fraction_bits>
संरचना HBFixed : Type
{
  स्थिर constexpr भिन्न mult = 1.f / (1 << fraction_bits);
  static_assert (Type::static_size * 8 > fraction_bits, "");

  चालक चिन्हित () अचर = delete;
  चालक अचिन्हित () अचर = delete;
  explicit चालक भिन्न () अचर { फर्काऊ to_float (); }
  typename Type::type to_int () अचर { फर्काऊ Type::v; }
  शून्य set_int (typename Type::type i ) { Type::v = i; }
  भिन्न to_float (भिन्न offset = 0) अचर  { फर्काऊ ((int32_t) Type::v + offset) * mult; }
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
  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
  Tag& चालक = (hb_tag_t i) { HBUINT32::चालक= (i); फर्काऊ *एतत्; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  चालक अचर वर्ण* () अचर { फर्काऊ reinterpret_cast<अचर वर्ण *> (एतत्); }
  चालक वर्ण* ()             { फर्काऊ reinterpret_cast<वर्ण *> (एतत्); }
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
संरचना HBGlyphID16 : HBUINT16
{
  HBGlyphID16& चालक = (uint16_t i) { HBUINT16::चालक= (i); फर्काऊ *एतत्; }
};
संरचना HBGlyphID24 : HBUINT24
{
  HBGlyphID24& चालक = (uint32_t i) { HBUINT24::चालक= (i); फर्काऊ *एतत्; }
};

/* Script/language-system/feature index */
संरचना Index : HBUINT16 {
  स्थिर constexpr अचिन्हित NOT_FOUND_INDEX = 0xFFFFu;
  Index& चालक = (uint16_t i) { HBUINT16::चालक= (i); फर्काऊ *एतत्; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, Index);

प्रकारनिर्देश Index NameID;

संरचना VarIdx : HBUINT32 {
  स्थिर constexpr अचिन्हित NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_INDEX, "");
  VarIdx& चालक = (uint32_t i) { HBUINT32::चालक= (i); फर्काऊ *एतत्; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, VarIdx);

/* Offset, Null offset = 0 */
ढाँचा <typename Type, तार्किक has_null=सत्य>
संरचना Offset : Type
{
  Offset& चालक = (typename Type::type i) { Type::चालक= (i); फर्काऊ *एतत्; }

  प्रकारनिर्देश Type type;

  तार्किक is_null () अचर { फर्काऊ has_null && 0 == *एतत्; }

  सार्वजनिक:
  DEFINE_SIZE_STATIC (परिमाण (Type));
};

प्रकारनिर्देश Offset<HBUINT16> Offset16;
प्रकारनिर्देश Offset<HBUINT24> Offset24;
प्रकारनिर्देश Offset<HBUINT32> Offset32;


/* CheckSum */
संरचना CheckSum : HBUINT32
{
  CheckSum& चालक = (uint32_t i) { HBUINT32::चालक= (i); फर्काऊ *एतत्; }

  /* This is reference implementation from the spec. */
  स्थिर uint32_t CalcTableChecksum (अचर HBUINT32 *Table, uint32_t Length)
  {
    uint32_t Sum = 0L;
    प्रतिज्ञा (0 == (Length & 3));
    अचर HBUINT32 *EndPtr = Table + Length / HBUINT32::static_size;

    जबसम्म (Table < EndPtr)
      Sum += *Table++;
    फर्काऊ Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  शून्य set_for_data (अचर शून्य *data, अचिन्हित पूर्णाङ्क length)
  { *एतत् = CalcTableChecksum ((अचर HBUINT32 *) data, length); }

  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};


/*
 * Version Numbers
 */

ढाँचा <typename FixedType=HBUINT16>
संरचना FixedVersion
{
  uint32_t to_int () अचर { फर्काऊ (major << (परिमाण (FixedType) * 8)) + minor; }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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

ढाँचा <typename Type, तार्किक has_null>
संरचना _hb_has_null
{
  स्थिर अचर Type *get_null () { फर्काऊ nullptr; }
  स्थिर Type *get_crap ()       { फर्काऊ nullptr; }
};
ढाँचा <typename Type>
संरचना _hb_has_null<Type, सत्य>
{
  स्थिर अचर Type *get_null () { फर्काऊ &Null (Type); }
  स्थिर       Type *get_crap () { फर्काऊ &Crap (Type); }
};

ढाँचा <typename Type, typename OffsetType, typename BaseType=शून्य, तार्किक has_null=सत्य>
संरचना OffsetTo : Offset<OffsetType, has_null>
{
  using target_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == असत्य ||
		 (hb_has_null_size (Type) || !hb_has_min_size (Type)), "");

  HB_DELETE_COPY_ASSIGN (OffsetTo);
  OffsetTo () = स्वभाव;

  OffsetTo& चालक = (typename OffsetType::type i) { OffsetType::चालक= (i); फर्काऊ *एतत्; }

  अचर Type& चालक () (अचर शून्य *base) अचर
  {
    यदि (unlikely (एतत्->is_null ())) फर्काऊ *_hb_has_null<Type, has_null>::get_null ();
    फर्काऊ StructAtOffset<अचर Type> (base, *एतत्);
  }
  Type& चालक () (शून्य *base) अचर
  {
    यदि (unlikely (एतत्->is_null ())) फर्काऊ *_hb_has_null<Type, has_null>::get_crap ();
    फर्काऊ StructAtOffset<Type> (base, *एतत्);
  }

  ढाँचा <typename Base,
	    hb_enable_if (hb_is_convertible (अचर Base, अचर BaseType *))>
  मित्र अचर Type& चालक + (अचर Base &base, अचर OffsetTo &offset) { फर्काऊ offset ((अचर शून्य *) base); }
  ढाँचा <typename Base,
	    hb_enable_if (hb_is_convertible (अचर Base, अचर BaseType *))>
  मित्र अचर Type& चालक + (अचर OffsetTo &offset, अचर Base &base) { फर्काऊ offset ((अचर शून्य *) base); }
  ढाँचा <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  मित्र Type& चालक + (Base &&base, OffsetTo &offset) { फर्काऊ offset ((शून्य *) base); }
  ढाँचा <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  मित्र Type& चालक + (OffsetTo &offset, Base &&base) { फर्काऊ offset ((शून्य *) base); }


  ढाँचा <typename Base, typename ...Ts>
  तार्किक serialize_subset (hb_subset_context_t *c, अचर OffsetTo& src,
			 अचर Base *src_base, Ts&&... ds)
  {
    *एतत् = 0;
    यदि (src.is_null ())
      फर्काऊ असत्य;

    स्वतः *s = c->serializer;

    s->push ();

    तार्किक ret = c->dispatch (src_base+src, std::forward<Ts> (ds)...);

    यदि (ret || !has_null)
      s->add_link (*एतत्, s->झिक_pack ());
    अन्यथा
      s->झिक_discard ();

    फर्काऊ ret;
  }


  ढाँचा <typename ...Ts>
  तार्किक serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *एतत् = 0;

    Type* obj = c->push<Type> ();
    तार्किक ret = obj->serialize (c, std::forward<Ts> (ds)...);

    यदि (ret)
      c->add_link (*एतत्, c->झिक_pack ());
    अन्यथा
      c->झिक_discard ();

    फर्काऊ ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  ढाँचा <typename ...Ts>
  तार्किक serialize_copy (hb_serialize_context_t *c, अचर OffsetTo& src,
		       अचर शून्य *src_base, अचिन्हित dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *एतत् = 0;
    यदि (src.is_null ())
      फर्काऊ असत्य;

    c->push ();

    तार्किक ret = c->copy (src_base+src, std::forward<Ts> (ds)...);

    c->add_link (*एतत्, c->झिक_pack (), whence, dst_bias);

    फर्काऊ ret;
  }

  तार्किक serialize_copy (hb_serialize_context_t *c, अचर OffsetTo& src,
		       अचर शून्य *src_base, अचिन्हित dst_bias = 0)
  { फर्काऊ serialize_copy (c, src, src_base, dst_bias, hb_serialize_context_t::Head); }

  तार्किक sanitize_shallow (hb_sanitize_context_t *c, अचर BaseType *base) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!c->check_struct (एतत्))) return_trace (असत्य);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    यदि (unlikely ((अचर वर्ण *) base + (अचिन्हित) *एतत् < (अचर वर्ण *) base)) return_trace (असत्य);
    return_trace (सत्य);
  }

  ढाँचा <typename ...Ts>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  तार्किक sanitize (hb_sanitize_context_t *c, अचर BaseType *base, Ts&&... ds) अचर
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
ढाँचा <typename Type, typename BaseType=शून्य, तार्किक has_null=सत्य> using Offset16To = OffsetTo<Type, HBUINT16, BaseType, has_null>;
ढाँचा <typename Type, typename BaseType=शून्य, तार्किक has_null=सत्य> using Offset24To = OffsetTo<Type, HBUINT24, BaseType, has_null>;
ढाँचा <typename Type, typename BaseType=शून्य, तार्किक has_null=सत्य> using Offset32To = OffsetTo<Type, HBUINT32, BaseType, has_null>;

ढाँचा <typename Type, typename OffsetType, typename BaseType=शून्य> using NNOffsetTo = OffsetTo<Type, OffsetType, BaseType, असत्य>;
ढाँचा <typename Type, typename BaseType=शून्य> using NNOffset16To = Offset16To<Type, BaseType, असत्य>;
ढाँचा <typename Type, typename BaseType=शून्य> using NNOffset24To = Offset24To<Type, BaseType, असत्य>;
ढाँचा <typename Type, typename BaseType=शून्य> using NNOffset32To = Offset32To<Type, BaseType, असत्य>;


/*
 * Array Types
 */

ढाँचा <typename Type>
संरचना UnsizedArrayOf
{
  प्रकारनिर्देश Type item_t;
  स्थिर constexpr अचिन्हित item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (UnsizedArrayOf);

  अचर Type& चालक [] (अचिन्हित पूर्णाङ्क i) अचर
  {
    फर्काऊ arrayZ[i];
  }
  Type& चालक [] (अचिन्हित पूर्णाङ्क i)
  {
    फर्काऊ arrayZ[i];
  }

  स्थिर माप_प्रकार get_size (अचिन्हित पूर्णाङ्क len)
  { फर्काऊ hb_unsigned_mul_saturate (len, Type::static_size); }

  ढाँचा <typename T> चालक T * () { फर्काऊ arrayZ; }
  ढाँचा <typename T> चालक अचर T * () अचर { फर्काऊ arrayZ; }
  hb_array_t<Type> as_array (अचिन्हित पूर्णाङ्क len)
  { फर्काऊ hb_array (arrayZ, len); }
  hb_array_t<अचर Type> as_array (अचिन्हित पूर्णाङ्क len) अचर
  { फर्काऊ hb_array (arrayZ, len); }

  ढाँचा <typename T>
  Type &lsearch (अचिन्हित पूर्णाङ्क len, अचर T &x, Type &not_found = Crap (Type))
  { फर्काऊ *as_array (len).lsearch (x, &not_found); }
  ढाँचा <typename T>
  अचर Type &lsearch (अचिन्हित पूर्णाङ्क len, अचर T &x, अचर Type &not_found = Null (Type)) अचर
  { फर्काऊ *as_array (len).lsearch (x, &not_found); }
  ढाँचा <typename T>
  तार्किक lfind (अचिन्हित पूर्णाङ्क len, अचर T &x, अचिन्हित पूर्णाङ्क *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णाङ्क to_store = (अचिन्हित पूर्णाङ्क) -1) अचर
  { फर्काऊ as_array (len).lfind (x, i, not_found, to_store); }

  शून्य क्विक (अचिन्हित पूर्णाङ्क len, अचिन्हित पूर्णाङ्क start = 0, अचिन्हित पूर्णाङ्क end = (अचिन्हित पूर्णाङ्क) -1)
  { as_array (len).क्विक (start, end); }

  तार्किक serialize (hb_serialize_context_t *c, अचिन्हित पूर्णाङ्क वस्तुहरू_len, तार्किक clear = सत्य)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_size (एतत्, get_size (वस्तुहरू_len), clear))) return_trace (असत्य);
    return_trace (सत्य);
  }
  ढाँचा <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  तार्किक serialize (hb_serialize_context_t *c, Iterator वस्तुहरू)
  {
    TRACE_SERIALIZE (एतत्);
    अचिन्हित गन्ती = hb_len (वस्तुहरू);
    यदि (unlikely (!serialize (c, गन्ती, असत्य))) return_trace (असत्य);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    लागि (अचिन्हित i = 0; i < गन्ती; i++, ++वस्तुहरू)
      arrayZ[i] = *वस्तुहरू;
    return_trace (सत्य);
  }

  UnsizedArrayOf* copy (hb_serialize_context_t *c, अचिन्हित गन्ती) अचर
  {
    TRACE_SERIALIZE (एतत्);
    स्वतः *out = c->start_embed (एतत्);
    यदि (unlikely (!as_array (गन्ती).copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  ढाँचा <typename ...Ts>
  HB_ALWAYS_INLINE
  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णाङ्क गन्ती, Ts&&... ds) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!sanitize_shallow (c, गन्ती))) return_trace (असत्य);
    यदि (!परिमाण... (Ts) && hb_is_trivially_copyable(Type)) return_trace (सत्य);
    hb_barrier ();
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      यदि (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (असत्य);
    return_trace (सत्य);
  }

  तार्किक sanitize_shallow (hb_sanitize_context_t *c, अचिन्हित पूर्णाङ्क गन्ती) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_array (arrayZ, गन्ती));
  }

  सार्वजनिक:
  Type		arrayZ[HB_VAR_ARRAY];
  सार्वजनिक:
  DEFINE_SIZE_UNBOUNDED (0);
};

/* Unsized array of offset's */
ढाँचा <typename Type, typename OffsetType, typename BaseType=शून्य, तार्किक has_null=सत्य>
using UnsizedArray16OfOffsetTo = UnsizedArrayOf<OffsetTo<Type, OffsetType, BaseType, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
ढाँचा <typename Type, typename OffsetType, typename BaseType=शून्य, तार्किक has_null=सत्य>
संरचना UnsizedListOfOffset16To : UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
{
  अचर Type& चालक [] (पूर्णाङ्क i_) अचर
  {
    अचिन्हित पूर्णाङ्क i = (अचिन्हित पूर्णाङ्क) i_;
    अचर OffsetTo<Type, OffsetType, BaseType, has_null> *p = &एतत्->arrayZ[i];
    यदि (unlikely ((अचर शून्य *) p < (अचर शून्य *) एतत्->arrayZ)) फर्काऊ Null (Type); /* Overflowed. */
    hb_barrier ();
    फर्काऊ एतत्+*p;
  }
  Type& चालक [] (पूर्णाङ्क i_)
  {
    अचिन्हित पूर्णाङ्क i = (अचिन्हित पूर्णाङ्क) i_;
    अचर OffsetTo<Type, OffsetType, BaseType, has_null> *p = &एतत्->arrayZ[i];
    यदि (unlikely ((अचर शून्य *) p < (अचर शून्य *) एतत्->arrayZ)) फर्काऊ Crap (Type); /* Overflowed. */
    hb_barrier ();
    फर्काऊ एतत्+*p;
  }

  ढाँचा <typename ...Ts>
  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णाङ्क गन्ती, Ts&&... ds) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace ((UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
		   ::sanitize (c, गन्ती, एतत्, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
ढाँचा <typename Type>
संरचना SortedUnsizedArrayOf : UnsizedArrayOf<Type>
{
  hb_sorted_array_t<Type> as_array (अचिन्हित पूर्णाङ्क len)
  { फर्काऊ hb_sorted_array (एतत्->arrayZ, len); }
  hb_sorted_array_t<अचर Type> as_array (अचिन्हित पूर्णाङ्क len) अचर
  { फर्काऊ hb_sorted_array (एतत्->arrayZ, len); }
  चालक hb_sorted_array_t<Type> ()             { फर्काऊ as_array (); }
  चालक hb_sorted_array_t<अचर Type> () अचर { फर्काऊ as_array (); }

  ढाँचा <typename T>
  Type &द्वा_खोज (अचिन्हित पूर्णाङ्क len, अचर T &x, Type &not_found = Crap (Type))
  { फर्काऊ *as_array (len).द्वा_खोज (x, &not_found); }
  ढाँचा <typename T>
  अचर Type &द्वा_खोज (अचिन्हित पूर्णाङ्क len, अचर T &x, अचर Type &not_found = Null (Type)) अचर
  { फर्काऊ *as_array (len).द्वा_खोज (x, &not_found); }
  ढाँचा <typename T>
  तार्किक bfind (अचिन्हित पूर्णाङ्क len, अचर T &x, अचिन्हित पूर्णाङ्क *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णाङ्क to_store = (अचिन्हित पूर्णाङ्क) -1) अचर
  { फर्काऊ as_array (len).bfind (x, i, not_found, to_store); }
};


/* An array with a number of elements. */
ढाँचा <typename Type, typename LenType>
संरचना ArrayOf
{
  प्रकारनिर्देश Type item_t;
  स्थिर constexpr अचिन्हित item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOf);

  अचर Type& चालक [] (पूर्णाङ्क i_) अचर
  {
    अचिन्हित पूर्णाङ्क i = (अचिन्हित पूर्णाङ्क) i_;
    यदि (unlikely (i >= len)) फर्काऊ Null (Type);
    hb_barrier ();
    फर्काऊ arrayZ[i];
  }
  Type& चालक [] (पूर्णाङ्क i_)
  {
    अचिन्हित पूर्णाङ्क i = (अचिन्हित पूर्णाङ्क) i_;
    यदि (unlikely (i >= len)) फर्काऊ Crap (Type);
    hb_barrier ();
    फर्काऊ arrayZ[i];
  }

  माप_प्रकार get_size () अचर
  { फर्काऊ hb_unsigned_mul_add_saturate (len, Type::static_size, len.static_size); }

  explicit चालक तार्किक () अचर { फर्काऊ len; }

  शून्य झिक () { len--; }

  hb_array_t<      Type> as_array ()       { फर्काऊ hb_array (arrayZ, len); }
  hb_array_t<अचर Type> as_array () अचर { फर्काऊ hb_array (arrayZ, len); }

  /* Iterator. */
  प्रकारनिर्देश hb_array_t<अचर Type>   iter_t;
  प्रकारनिर्देश hb_array_t<      Type> writer_t;
    iter_t   iter () अचर { फर्काऊ as_array (); }
  writer_t writer ()       { फर्काऊ as_array (); }
  चालक   iter_t () अचर { फर्काऊ   iter (); }
  चालक writer_t ()       { फर्काऊ writer (); }

  /* Faster range-based for loop. */
  अचर Type *begin () अचर { फर्काऊ arrayZ; }
  अचर Type *end () अचर { फर्काऊ arrayZ + len; }

  ढाँचा <typename T>
  Type &lsearch (अचर T &x, Type &not_found = Crap (Type))
  { फर्काऊ *as_array ().lsearch (x, &not_found); }
  ढाँचा <typename T>
  अचर Type &lsearch (अचर T &x, अचर Type &not_found = Null (Type)) अचर
  { फर्काऊ *as_array ().lsearch (x, &not_found); }
  ढाँचा <typename T>
  तार्किक lfind (अचर T &x, अचिन्हित पूर्णाङ्क *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णाङ्क to_store = (अचिन्हित पूर्णाङ्क) -1) अचर
  { फर्काऊ as_array ().lfind (x, i, not_found, to_store); }

  शून्य क्विक ()
  { as_array ().क्विक (); }

  HB_NODISCARD तार्किक serialize (hb_serialize_context_t *c, अचिन्हित वस्तुहरू_len, तार्किक clear = सत्य)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);
    c->check_assign (len, वस्तुहरू_len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    यदि (unlikely (!c->extend_size (एतत्, get_size (), clear))) return_trace (असत्य);
    return_trace (सत्य);
  }
  ढाँचा <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD तार्किक serialize (hb_serialize_context_t *c, Iterator वस्तुहरू)
  {
    TRACE_SERIALIZE (एतत्);
    अचिन्हित गन्ती = hb_len (वस्तुहरू);
    यदि (unlikely (!serialize (c, गन्ती, असत्य))) return_trace (असत्य);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    लागि (अचिन्हित i = 0; i < गन्ती; i++, ++वस्तुहरू)
      arrayZ[i] = *वस्तुहरू;
    return_trace (सत्य);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (एतत्);
    len++;
    यदि (unlikely (!len || !c->extend (एतत्)))
    {
      len--;
      return_trace (nullptr);
    }
    return_trace (&arrayZ[len - 1]);
  }

  ArrayOf* copy (hb_serialize_context_t *c) अचर
  {
    TRACE_SERIALIZE (एतत्);
    स्वतः *out = c->start_embed (एतत्);
    यदि (unlikely (!c->extend_min (out))) return_trace (nullptr);
    c->check_assign (out->len, len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    यदि (unlikely (!as_array ().copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  ढाँचा <typename ...Ts>
  HB_ALWAYS_INLINE
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!sanitize_shallow (c))) return_trace (असत्य);
    यदि (!परिमाण... (Ts) && hb_is_trivially_copyable(Type)) return_trace (सत्य);
    hb_barrier ();
    अचिन्हित पूर्णाङ्क गन्ती = len;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      यदि (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (असत्य);
    return_trace (सत्य);
  }

  तार्किक sanitize_shallow (hb_sanitize_context_t *c) अचर
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
ढाँचा <typename Type> using Array16Of = ArrayOf<Type, HBUINT16>;
ढाँचा <typename Type> using Array24Of = ArrayOf<Type, HBUINT24>;
ढाँचा <typename Type> using Array32Of = ArrayOf<Type, HBUINT32>;
using PString = ArrayOf<HBUINT8, HBUINT8>;

/* Array of Offset's */
ढाँचा <typename Type> using Array8OfOffset24To = ArrayOf<OffsetTo<Type, HBUINT24>, HBUINT8>;
ढाँचा <typename Type> using Array16OfOffset16To = ArrayOf<OffsetTo<Type, HBUINT16>, HBUINT16>;
ढाँचा <typename Type> using Array16OfOffset32To = ArrayOf<OffsetTo<Type, HBUINT32>, HBUINT16>;
ढाँचा <typename Type> using Array32OfOffset32To = ArrayOf<OffsetTo<Type, HBUINT32>, HBUINT32>;

/* Array of offsets relative to the beginning of the array itself. */
ढाँचा <typename Type, typename OffsetType>
संरचना List16OfOffsetTo : ArrayOf<OffsetTo<Type, OffsetType>, HBUINT16>
{
  अचर Type& चालक [] (पूर्णाङ्क i_) अचर
  {
    अचिन्हित पूर्णाङ्क i = (अचिन्हित पूर्णाङ्क) i_;
    यदि (unlikely (i >= एतत्->len)) फर्काऊ Null (Type);
    hb_barrier ();
    फर्काऊ एतत्+एतत्->arrayZ[i];
  }
  अचर Type& चालक [] (पूर्णाङ्क i_)
  {
    अचिन्हित पूर्णाङ्क i = (अचिन्हित पूर्णाङ्क) i_;
    यदि (unlikely (i >= एतत्->len)) फर्काऊ Crap (Type);
    hb_barrier ();
    फर्काऊ एतत्+एतत्->arrayZ[i];
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    संरचना List16OfOffsetTo *out = c->serializer->embed (*एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);
    अचिन्हित पूर्णाङ्क गन्ती = एतत्->len;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      out->arrayZ[i].serialize_subset (c, एतत्->arrayZ[i], एतत्, out);
    return_trace (सत्य);
  }

  ढाँचा <typename ...Ts>
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace ((Array16Of<OffsetTo<Type, OffsetType>>::sanitize (c, एतत्, std::forward<Ts> (ds)...)));
  }
};

ढाँचा <typename Type>
using List16OfOffset16To = List16OfOffsetTo<Type, HBUINT16>;

/* An array starting at second element. */
ढाँचा <typename Type, typename LenType>
संरचना HeadlessArrayOf
{
  स्थिर constexpr अचिन्हित item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (HeadlessArrayOf);

  अचर Type& चालक [] (पूर्णाङ्क i_) अचर
  {
    अचिन्हित पूर्णाङ्क i = (अचिन्हित पूर्णाङ्क) i_;
    यदि (unlikely (i >= lenP1 || !i)) फर्काऊ Null (Type);
    hb_barrier ();
    फर्काऊ arrayZ[i-1];
  }
  Type& चालक [] (पूर्णाङ्क i_)
  {
    अचिन्हित पूर्णाङ्क i = (अचिन्हित पूर्णाङ्क) i_;
    यदि (unlikely (i >= lenP1 || !i)) फर्काऊ Crap (Type);
    hb_barrier ();
    फर्काऊ arrayZ[i-1];
  }
  माप_प्रकार get_size () अचर
  { फर्काऊ hb_unsigned_mul_add_saturate (get_length (), Type::static_size, lenP1.static_size); }

  अचिन्हित get_length () अचर { फर्काऊ lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { फर्काऊ hb_array (arrayZ, get_length ()); }
  hb_array_t<अचर Type> as_array () अचर { फर्काऊ hb_array (arrayZ, get_length ()); }

  /* Iterator. */
  प्रकारनिर्देश hb_array_t<अचर Type>   iter_t;
  प्रकारनिर्देश hb_array_t<      Type> writer_t;
    iter_t   iter () अचर { फर्काऊ as_array (); }
  writer_t writer ()       { फर्काऊ as_array (); }
  चालक   iter_t () अचर { फर्काऊ   iter (); }
  चालक writer_t ()       { फर्काऊ writer (); }

  /* Faster range-based for loop. */
  अचर Type *begin () अचर { फर्काऊ arrayZ; }
  अचर Type *end () अचर { फर्काऊ arrayZ + get_length (); }

  HB_NODISCARD तार्किक serialize (hb_serialize_context_t *c, अचिन्हित पूर्णाङ्क वस्तुहरू_len, तार्किक clear = सत्य)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);
    c->check_assign (lenP1, वस्तुहरू_len + 1, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    यदि (unlikely (!c->extend_size (एतत्, get_size (), clear))) return_trace (असत्य);
    return_trace (सत्य);
  }
  ढाँचा <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD तार्किक serialize (hb_serialize_context_t *c, Iterator वस्तुहरू)
  {
    TRACE_SERIALIZE (एतत्);
    अचिन्हित गन्ती = hb_len (वस्तुहरू);
    यदि (unlikely (!serialize (c, गन्ती, असत्य))) return_trace (असत्य);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    लागि (अचिन्हित i = 0; i < गन्ती; i++, ++वस्तुहरू)
      arrayZ[i] = *वस्तुहरू;
    return_trace (सत्य);
  }

  ढाँचा <typename ...Ts>
  HB_ALWAYS_INLINE
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!sanitize_shallow (c))) return_trace (असत्य);
    यदि (!परिमाण... (Ts) && hb_is_trivially_copyable(Type)) return_trace (सत्य);
    hb_barrier ();
    अचिन्हित पूर्णाङ्क गन्ती = get_length ();
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      यदि (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (असत्य);
    return_trace (सत्य);
  }

  गुप्त:
  तार्किक sanitize_shallow (hb_sanitize_context_t *c) अचर
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
ढाँचा <typename Type> using HeadlessArray16Of = HeadlessArrayOf<Type, HBUINT16>;

/* An array storing length-1. */
ढाँचा <typename Type, typename LenType=HBUINT16>
संरचना ArrayOfM1
{
  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOfM1);

  अचर Type& चालक [] (पूर्णाङ्क i_) अचर
  {
    अचिन्हित पूर्णाङ्क i = (अचिन्हित पूर्णाङ्क) i_;
    यदि (unlikely (i > lenM1)) फर्काऊ Null (Type);
    hb_barrier ();
    फर्काऊ arrayZ[i];
  }
  Type& चालक [] (पूर्णाङ्क i_)
  {
    अचिन्हित पूर्णाङ्क i = (अचिन्हित पूर्णाङ्क) i_;
    यदि (unlikely (i > lenM1)) फर्काऊ Crap (Type);
    hb_barrier ();
    फर्काऊ arrayZ[i];
  }
  माप_प्रकार get_size () अचर
  { फर्काऊ hb_unsigned_mul_add_saturate (lenM1 + 1, Type::static_size, lenM1.static_size); }

  ढाँचा <typename ...Ts>
  HB_ALWAYS_INLINE
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!sanitize_shallow (c))) return_trace (असत्य);
    यदि (!परिमाण... (Ts) && hb_is_trivially_copyable(Type)) return_trace (सत्य);
    hb_barrier ();
    अचिन्हित पूर्णाङ्क गन्ती = lenM1 + 1;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      यदि (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (असत्य);
    return_trace (सत्य);
  }

  गुप्त:
  तार्किक sanitize_shallow (hb_sanitize_context_t *c) अचर
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
ढाँचा <typename Type, typename LenType>
संरचना SortedArrayOf : ArrayOf<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { फर्काऊ hb_sorted_array (एतत्->arrayZ, एतत्->len); }
  hb_sorted_array_t<अचर Type> as_array () अचर { फर्काऊ hb_sorted_array (एतत्->arrayZ, एतत्->len); }

  /* Iterator. */
  प्रकारनिर्देश hb_sorted_array_t<अचर Type>   iter_t;
  प्रकारनिर्देश hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () अचर { फर्काऊ as_array (); }
  writer_t writer ()       { फर्काऊ as_array (); }
  चालक   iter_t () अचर { फर्काऊ   iter (); }
  चालक writer_t ()       { फर्काऊ writer (); }

  /* Faster range-based for loop. */
  अचर Type *begin () अचर { फर्काऊ एतत्->arrayZ; }
  अचर Type *end () अचर { फर्काऊ एतत्->arrayZ + एतत्->len; }

  तार्किक serialize (hb_serialize_context_t *c, अचिन्हित पूर्णाङ्क वस्तुहरू_len)
  {
    TRACE_SERIALIZE (एतत्);
    तार्किक ret = ArrayOf<Type, LenType>::serialize (c, वस्तुहरू_len);
    return_trace (ret);
  }
  ढाँचा <typename Iterator,
	    hb_requires (hb_is_sorted_source_of (Iterator, Type))>
  तार्किक serialize (hb_serialize_context_t *c, Iterator वस्तुहरू)
  {
    TRACE_SERIALIZE (एतत्);
    तार्किक ret = ArrayOf<Type, LenType>::serialize (c, वस्तुहरू);
    return_trace (ret);
  }

  SortedArrayOf* copy (hb_serialize_context_t *c) अचर
  {
    TRACE_SERIALIZE (एतत्);
    SortedArrayOf* out = reinterpret_cast<SortedArrayOf *> (ArrayOf<Type, LenType>::copy (c));
    return_trace (out);
  }

  ढाँचा <typename T>
  Type &द्वा_खोज (अचर T &x, Type &not_found = Crap (Type))
  { फर्काऊ *as_array ().द्वा_खोज (x, &not_found); }
  ढाँचा <typename T>
  अचर Type &द्वा_खोज (अचर T &x, अचर Type &not_found = Null (Type)) अचर
  { फर्काऊ *as_array ().द्वा_खोज (x, &not_found); }
  ढाँचा <typename T>
  तार्किक bfind (अचर T &x, अचिन्हित पूर्णाङ्क *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णाङ्क to_store = (अचिन्हित पूर्णाङ्क) -1) अचर
  { फर्काऊ as_array ().bfind (x, i, not_found, to_store); }
};

ढाँचा <typename Type> using SortedArray16Of = SortedArrayOf<Type, HBUINT16>;
ढाँचा <typename Type> using SortedArray24Of = SortedArrayOf<Type, HBUINT24>;
ढाँचा <typename Type> using SortedArray32Of = SortedArrayOf<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

ढाँचा <typename LenType=HBUINT16>
संरचना Bin_खोज_Header
{
  चालक uint32_t () अचर { फर्काऊ len; }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  Bin_खोज_Header& चालक = (अचिन्हित पूर्णाङ्क v)
  {
    len = v;
    प्रतिज्ञा (len == v);
    entrySelector = hb_max (1u, hb_bit_storage (v)) - 1;
    खोज_Range = 16 * (1u << entrySelector);
    rangeShift = v * 16 > खोज_Range
		 ? 16 * v - खोज_Range
		 : 0;
    फर्काऊ *एतत्;
  }

  रक्षित:
  LenType	len;
  LenType	खोज_Range;
  LenType	entrySelector;
  LenType	rangeShift;

  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

ढाँचा <typename Type, typename LenType=HBUINT16>
using Bin_खोज_Array_Of = SortedArrayOf<Type, Bin_खोज_Header<LenType>>;


संरचना Var_Sized_Bin_खोज_Header
{

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  HBUINT16	unitSize;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	खोज_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	entrySelector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	rangeShift;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (10);
};

ढाँचा <typename Type>
संरचना Var_Sized_Bin_खोज_Array_Of
{
  स्थिर constexpr अचिन्हित item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (Var_Sized_Bin_खोज_Array_Of);

  तार्किक last_is_terminator () अचर
  {
    यदि (unlikely (!header.nUnits)) फर्काऊ असत्य;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    अचर HBUINT16 *शब्दहरू = &StructAtOffset<HBUINT16> (&bytesZ, (header.nUnits - 1) * header.unitSize);
    अचिन्हित पूर्णाङ्क गन्ती = Type::Termination_Word_गन्ती;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      यदि (शब्दहरू[i] != 0xFFFFu)
	फर्काऊ असत्य;
    फर्काऊ सत्य;
  }

  अचर Type& चालक [] (पूर्णाङ्क i_) अचर
  {
    अचिन्हित पूर्णाङ्क i = (अचिन्हित पूर्णाङ्क) i_;
    यदि (unlikely (i >= get_length ())) फर्काऊ Null (Type);
    hb_barrier ();
    फर्काऊ StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  Type& चालक [] (पूर्णाङ्क i_)
  {
    अचिन्हित पूर्णाङ्क i = (अचिन्हित पूर्णाङ्क) i_;
    यदि (unlikely (i >= get_length ())) फर्काऊ Crap (Type);
    hb_barrier ();
    फर्काऊ StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  अचिन्हित पूर्णाङ्क get_length () अचर
  { फर्काऊ header.nUnits - last_is_terminator (); }
  माप_प्रकार get_size () अचर
  { फर्काऊ hb_unsigned_mul_add_saturate (header.nUnits, header.unitSize, header.static_size); }

  ढाँचा <typename ...Ts>
  HB_ALWAYS_INLINE
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!sanitize_shallow (c))) return_trace (असत्य);
    यदि (!परिमाण... (Ts) && hb_is_trivially_copyable(Type)) return_trace (सत्य);
    hb_barrier ();
    अचिन्हित पूर्णाङ्क गन्ती = get_length ();
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      यदि (unlikely (!(*एतत्)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (असत्य);
    return_trace (सत्य);
  }

  ढाँचा <typename T>
  अचर Type *द्वा_खोज (अचर T &key) अचर
  {
    अचिन्हित pos;
    फर्काऊ hb_bsearch_impl (&pos,
			    key,
			    (अचर शून्य *) bytesZ,
			    get_length (),
			    header.unitSize,
			    _hb_cmp_method<T, Type>)
	   ? (अचर Type *) (((अचर वर्ण *) &bytesZ) + (pos * header.unitSize))
	   : nullptr;
  }

  गुप्त:
  तार्किक sanitize_shallow (hb_sanitize_context_t *c) अचर
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
  Var_Sized_Bin_खोज_Header	header;
  UnsizedArrayOf<HBUINT8>	bytesZ;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (10, bytesZ);
};


/* CFF INDEX */

ढाँचा <typename गन्ती_3>
संरचना CFFIndex
{
  अचिन्हित पूर्णाङ्क offset_array_size () अचर
  { फर्काऊ offSize * (गन्ती + 1); }

  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  तार्किक serialize (hb_serialize_context_t *c,
		  अचर Iterable &iterable,
		  अचर अचिन्हित *p_data_size = nullptr,
                  अचिन्हित min_off_size = 0)
  {
    TRACE_SERIALIZE (एतत्);
    अचिन्हित data_size;
    यदि (p_data_size)
      data_size = *p_data_size;
    अन्यथा
      total_size (iterable, &data_size);

    स्वतः it = hb_iter (iterable);
    यदि (unlikely (!serialize_header (c, +it, data_size, min_off_size))) return_trace (असत्य);
    अचिन्हित वर्ण *ret = c->allocate_size<अचिन्हित वर्ण> (data_size, असत्य);
    यदि (unlikely (!ret)) return_trace (असत्य);
    अचिन्हित remaining = data_size;
    लागि (अचर स्वतः &_ : +it)
    {
      अचिन्हित len = _.length;

      यदि (!len)
	जारी;

      यदि (unlikely (len > remaining)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (असत्य, HB_SERIALIZE_ERROR_INT_OVERFLOW));
      }

      remaining -= len;

      यदि (len <= 1)
      {
	*ret++ = *_.arrayZ;
	जारी;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (सत्य);
  }

  ढाँचा <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize_header (hb_serialize_context_t *c,
			 Iterator it,
			 अचिन्हित data_size,
                         अचिन्हित min_off_size = 0)
  {
    TRACE_SERIALIZE (एतत्);

    अचिन्हित off_size = (hb_bit_storage (data_size + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    /* serialize CFFIndex header */
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);
    एतत्->गन्ती = hb_len (it);
    यदि (!एतत्->गन्ती) return_trace (सत्य);
    यदि (unlikely (!c->extend (एतत्->offSize))) return_trace (असत्य);
    एतत्->offSize = off_size;
    यदि (unlikely (!c->allocate_size<HBUINT8> (off_size * (एतत्->गन्ती + 1), असत्य)))
      return_trace (असत्य);

    /* serialize indices */
    अचिन्हित पूर्णाङ्क offset = 1;
    यदि (HB_OPTIMIZE_SIZE_VAL)
    {
      अचिन्हित पूर्णाङ्क i = 0;
      लागि (अचर स्वतः &_ : +it)
      {
	set_offset_at (i++, offset);
	offset += hb_len_of (_);
      }
      set_offset_at (i, offset);
    }
    अन्यथा
      विकल्प (off_size)
      {
	पक्ष 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  लागि (अचर स्वतः &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	रोक;
	पक्ष 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  लागि (अचर स्वतः &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	रोक;
	पक्ष 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  लागि (अचर स्वतः &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	रोक;
	पक्ष 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  लागि (अचर स्वतः &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	रोक;
	स्वभाव:
	रोक;
      }

    प्रतिज्ञा (offset == data_size + 1);
    return_trace (सत्य);
  }

  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  स्थिर अचिन्हित total_size (अचर Iterable &iterable, अचिन्हित *data_size = nullptr, अचिन्हित min_off_size = 0)
  {
    स्वतः it = + hb_iter (iterable);
    यदि (!it)
    {
      यदि (data_size) *data_size = 0;
      फर्काऊ min_size;
    }

    अचिन्हित total = 0;
    लागि (अचर स्वतः &_ : +it)
      total += hb_len_of (_);

    यदि (data_size) *data_size = total;

    अचिन्हित off_size = (hb_bit_storage (total + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    फर्काऊ min_size + HBUINT8::static_size + (hb_len (it) + 1) * off_size + total;
  }

  शून्य set_offset_at (अचिन्हित पूर्णाङ्क index, अचिन्हित पूर्णाङ्क offset)
  {
    प्रतिज्ञा (index <= गन्ती);

    अचिन्हित पूर्णाङ्क size = offSize;
    अचर HBUINT8 *p = offsets;
    विकल्प (size)
    {
      पक्ष 1: ((HBUINT8  *) p)[index] = offset; रोक;
      पक्ष 2: ((HBUINT16 *) p)[index] = offset; रोक;
      पक्ष 3: ((HBUINT24 *) p)[index] = offset; रोक;
      पक्ष 4: ((HBUINT32 *) p)[index] = offset; रोक;
      स्वभाव: फर्काऊ;
    }
  }

  गुप्त:
  अचिन्हित पूर्णाङ्क offset_at (अचिन्हित पूर्णाङ्क index) अचर
  {
    प्रतिज्ञा (index <= गन्ती);

    अचिन्हित पूर्णाङ्क size = offSize;
    अचर HBUINT8 *p = offsets;
    विकल्प (size)
    {
      पक्ष 1: फर्काऊ ((HBUINT8  *) p)[index];
      पक्ष 2: फर्काऊ ((HBUINT16 *) p)[index];
      पक्ष 3: फर्काऊ ((HBUINT24 *) p)[index];
      पक्ष 4: फर्काऊ ((HBUINT32 *) p)[index];
      स्वभाव: फर्काऊ 0;
    }
  }

  अचर अचिन्हित वर्ण *data_base () अचर
  { फर्काऊ (अचर अचिन्हित वर्ण *) एतत् + min_size + offSize.static_size - 1 + offset_array_size (); }
  सार्वजनिक:

  hb_ubytes_t चालक [] (अचिन्हित पूर्णाङ्क index) अचर
  {
    यदि (unlikely (index >= गन्ती)) फर्काऊ hb_ubytes_t ();
    hb_barrier ();
    अचिन्हित offset0 = offset_at (index);
    अचिन्हित offset1 = offset_at (index + 1);
    यदि (unlikely (offset1 < offset0 || offset1 > offset_at (गन्ती)))
      फर्काऊ hb_ubytes_t ();
    फर्काऊ hb_ubytes_t (data_base () + offset0, offset1 - offset0);
  }

  माप_प्रकार get_size () अचर
  {
    यदि (गन्ती)
      फर्काऊ hb_unsigned_add_saturate (min_size, offSize.static_size,
				       offset_array_size (), offset_at (गन्ती) - 1);
    फर्काऊ min_size;  /* empty CFFIndex contains count only */
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  hb_barrier () &&
			  (गन्ती == 0 || /* empty INDEX */
			   (गन्ती < गन्ती + 1u &&
			    c->check_struct (&offSize) && offSize >= 1 && offSize <= 4 &&
			    c->check_array (offsets, offSize, गन्ती + 1u) &&
			    c->check_range (data_base (), offset_at (गन्ती))))));
  }

  सार्वजनिक:
  गन्ती_3		गन्ती;		/* Number of object data. Note there are (count+1) offsets */
  गुप्त:
  HBUINT8	offSize;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  सार्वजनिक:
  DEFINE_SIZE_MIN (गन्ती_3::static_size);
};
प्रकारनिर्देश CFFIndex<HBUINT16> CFF1Index;
प्रकारनिर्देश CFFIndex<HBUINT32> CFF2Index;


/* TupleValues */
संरचना Tuple_मानहरू
{
  गणना packed_मान_flag_t
  {
    मानहरू_ARE_ZEROS     = 0x80,
    मानहरू_ARE_BYTES     = 0x00,
    मानहरू_ARE_शब्दहरू     = 0x40,
    मानहरू_ARE_LONGS     = 0xC0,
    मानहरू_SIZE_MASK     = 0xC0,
    मान_RUN_गन्ती_MASK = 0x3F
  };

  स्थिर अचिन्हित compile_unsafe (hb_array_t<अचर पूर्णाङ्क> values, /* IN */
				  अचिन्हित वर्ण *encoded_bytes /* OUT */)
  {
    अचिन्हित अङ्क_मानहरू_3 = values.length;
    अचिन्हित encoded_len = 0;
    अचिन्हित i = 0;
    जबसम्म (i < अङ्क_मानहरू_3)
    {
      पूर्णाङ्क val = values.arrayZ[i];
      यदि (val == 0)
        encoded_len += encode_मान_run_as_zeroes (i, encoded_bytes + encoded_len, values);
      अन्यथा यदि ((int8_t) val == val)
        encoded_len += encode_मान_run_as_bytes (i, encoded_bytes + encoded_len, values);
      अन्यथा यदि ((int16_t) val == val)
        encoded_len += encode_मान_run_as_शब्दहरू (i, encoded_bytes + encoded_len, values);
      अन्यथा
        encoded_len += encode_मान_run_as_longs (i, encoded_bytes + encoded_len, values);
    }
    फर्काऊ encoded_len;
  }

  स्थिर अचिन्हित encode_मान_run_as_zeroes (अचिन्हित& i,
					      अचिन्हित वर्ण *it,
					      hb_array_t<अचर पूर्णाङ्क> values)
  {
    अचिन्हित अङ्क_मानहरू_3 = values.length;
    अचिन्हित run_length = 0;
    अचिन्हित encoded_len = 0;
    जबसम्म (i < अङ्क_मानहरू_3 && values.arrayZ[i] == 0)
    {
      i++;
      run_length++;
    }

    जबसम्म (run_length >= 64)
    {
      *it++ = वर्ण (मानहरू_ARE_ZEROS | 63);
      run_length -= 64;
      encoded_len++;
    }

    यदि (run_length)
    {
      *it++ = वर्ण (मानहरू_ARE_ZEROS | (run_length - 1));
      encoded_len++;
    }
    फर्काऊ encoded_len;
  }

  स्थिर अचिन्हित encode_मान_run_as_bytes (अचिन्हित &i,
					     अचिन्हित वर्ण *it,
					     hb_array_t<अचर पूर्णाङ्क> values)
  {
    अचिन्हित start = i;
    अचिन्हित अङ्क_मानहरू_3 = values.length;
    जबसम्म (i < अङ्क_मानहरू_3)
    {
      पूर्णाङ्क val = values.arrayZ[i];
      यदि ((int8_t) val != val)
        रोक;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      यदि (val == 0 && i + 1 < अङ्क_मानहरू_3 && values.arrayZ[i+1] == 0)
        रोक;

      i++;
    }
    अचिन्हित run_length = i - start;

    अचिन्हित encoded_len = 0;

    जबसम्म (run_length >= 64)
    {
      *it++ = (मानहरू_ARE_BYTES | 63);
      encoded_len++;

      लागि (अचिन्हित j = 0; j < 64; j++)
	it[j] = static_cast<वर्ण> (values.arrayZ[start + j]);
      it += 64;
      encoded_len += 64;

      start += 64;
      run_length -= 64;
    }

    यदि (run_length)
    {
      *it++ = (मानहरू_ARE_BYTES | (run_length - 1));
      encoded_len++;

      लागि (अचिन्हित j = 0; j < run_length; j++)
        it[j] = static_cast<वर्ण> (values.arrayZ[start + j]);
      encoded_len += run_length;
    }

    फर्काऊ encoded_len;
  }

  स्थिर अचिन्हित encode_मान_run_as_शब्दहरू (अचिन्हित &i,
					     अचिन्हित वर्ण *it,
					     hb_array_t<अचर पूर्णाङ्क> values)
  {
    अचिन्हित start = i;
    अचिन्हित अङ्क_मानहरू_3 = values.length;
    जबसम्म (i < अङ्क_मानहरू_3)
    {
      पूर्णाङ्क val = values.arrayZ[i];

      यदि ((int16_t) val != val)
        रोक;

      /* start a new run for a single zero value. */
      यदि (val == 0) रोक;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      यदि ((int8_t) val == val &&
          i + 1 < अङ्क_मानहरू_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        रोक;

      i++;
    }

    अचिन्हित run_length = i - start;
    अचिन्हित encoded_len = 0;
    जबसम्म (run_length >= 64)
    {
      *it++ = (मानहरू_ARE_शब्दहरू | 63);
      encoded_len++;

      लागि (अचिन्हित j = 0; j < 64; j++)
      {
        int16_t मान_val = values.arrayZ[start + j];
        *it++ = static_cast<वर्ण> (मान_val >> 8);
        *it++ = static_cast<वर्ण> (मान_val & 0xFF);

        encoded_len += 2;
      }

      start += 64;
      run_length -= 64;
    }

    यदि (run_length)
    {
      *it++ = (मानहरू_ARE_शब्दहरू | (run_length - 1));
      encoded_len++;
      जबसम्म (start < i)
      {
        int16_t मान_val = values.arrayZ[start++];
        *it++ = static_cast<वर्ण> (मान_val >> 8);
        *it++ = static_cast<वर्ण> (मान_val & 0xFF);

        encoded_len += 2;
      }
    }
    फर्काऊ encoded_len;
  }

  स्थिर अचिन्हित encode_मान_run_as_longs (अचिन्हित &i,
					     अचिन्हित वर्ण *it,
					     hb_array_t<अचर पूर्णाङ्क> values)
  {
    अचिन्हित start = i;
    अचिन्हित अङ्क_मानहरू_3 = values.length;
    जबसम्म (i < अङ्क_मानहरू_3)
    {
      पूर्णाङ्क val = values.arrayZ[i];

      यदि ((int16_t) val == val)
        रोक;

      i++;
    }

    अचिन्हित run_length = i - start;
    अचिन्हित encoded_len = 0;
    जबसम्म (run_length >= 64)
    {
      *it++ = (मानहरू_ARE_LONGS | 63);
      encoded_len++;

      लागि (अचिन्हित j = 0; j < 64; j++)
      {
        int32_t मान_val = values.arrayZ[start + j];
        *it++ = static_cast<वर्ण> (मान_val >> 24);
        *it++ = static_cast<वर्ण> (मान_val >> 16);
        *it++ = static_cast<वर्ण> (मान_val >> 8);
        *it++ = static_cast<वर्ण> (मान_val & 0xFF);

        encoded_len += 4;
      }

      start += 64;
      run_length -= 64;
    }

    यदि (run_length)
    {
      *it++ = (मानहरू_ARE_LONGS | (run_length - 1));
      encoded_len++;
      जबसम्म (start < i)
      {
        int32_t मान_val = values.arrayZ[start++];
        *it++ = static_cast<वर्ण> (मान_val >> 24);
        *it++ = static_cast<वर्ण> (मान_val >> 16);
        *it++ = static_cast<वर्ण> (मान_val >> 8);
        *it++ = static_cast<वर्ण> (मान_val & 0xFF);

        encoded_len += 4;
      }
    }
    फर्काऊ encoded_len;
  }

  ढाँचा <typename T>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  स्थिर तार्किक decompile (अचर HBUINT8 *&p /* IN/OUT */,
			 hb_vector_t<T> &values /* IN/OUT */,
			 अचर HBUINT8 *end,
			 तार्किक consume_all = असत्य,
			 अचिन्हित start = 0)
  {
    अचिन्हित i = 0;
    अचिन्हित गन्ती = consume_all ? अच_पूर्णांक_उच्च : values.length;
    यदि (consume_all)
      values.alloc ((end - p) / 2);
    जबसम्म (i < गन्ती)
    {
      यदि (unlikely (p + 1 > end)) फर्काऊ consume_all;
      अचिन्हित control = *p++;
      अचिन्हित run_गन्ती = (control & मान_RUN_गन्ती_MASK) + 1;
      यदि (consume_all)
      {
        यदि (unlikely (!values.resize_dirty  (values.length + run_गन्ती)))
	  फर्काऊ असत्य;
      }
      अचिन्हित stop = i + run_गन्ती;
      यदि (unlikely (stop > गन्ती)) फर्काऊ असत्य;

      अचिन्हित skip = i < start ? hb_min (start - i, run_गन्ती) : 0;
      i += skip;

      यदि ((control & मानहरू_SIZE_MASK) == मानहरू_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * परिमाण (T));
	i = stop;
      }
      अन्यथा यदि ((control & मानहरू_SIZE_MASK) ==  मानहरू_ARE_शब्दहरू)
      {
        यदि (unlikely (p + run_गन्ती * HBINT16::static_size > end)) फर्काऊ असत्य;
	p += skip * HBINT16::static_size;
#ifndef HB_OPTIMIZE_SIZE
        लागि (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (अचर HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 1] = * (अचर HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 2] = * (अचर HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 3] = * (अचर HBINT16 *) p;
	  p += HBINT16::static_size;
	}
#endif
        लागि (; i < stop; i++)
        {
          values.arrayZ[i] = * (अचर HBINT16 *) p;
          p += HBINT16::static_size;
        }
      }
      अन्यथा यदि ((control & मानहरू_SIZE_MASK) ==  मानहरू_ARE_LONGS)
      {
        यदि (unlikely (p + run_गन्ती * HBINT32::static_size > end)) फर्काऊ असत्य;
	p += skip * HBINT32::static_size;
        लागि (; i < stop; i++)
        {
          values.arrayZ[i] = * (अचर HBINT32 *) p;
          p += HBINT32::static_size;
        }
      }
      अन्यथा यदि ((control & मानहरू_SIZE_MASK) ==  मानहरू_ARE_BYTES)
      {
        यदि (unlikely (p + run_गन्ती > end)) फर्काऊ असत्य;
	p += skip * HBINT8::static_size;
#ifndef HB_OPTIMIZE_SIZE
	लागि (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (अचर HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (अचर HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (अचर HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (अचर HBINT8 *) p++;
	}
#endif
        लागि (; i < stop; i++)
          values.arrayZ[i] = * (अचर HBINT8 *) p++;
      }
    }
    फर्काऊ सत्य;
  }

  स्थिर तार्किक skip (अचर अचिन्हित वर्ण *&p /* IN/OUT */,
		    अचर अचिन्हित वर्ण *end,
		    अचिन्हित गन्ती)
  {
    अचिन्हित seen = 0;
    जबसम्म (seen < गन्ती)
    {
      यदि (unlikely (p >= end)) फर्काऊ असत्य;
      अचिन्हित control = *p++;
      अचिन्हित run_गन्ती = (control & मान_RUN_गन्ती_MASK) + 1;
      यदि (unlikely (run_गन्ती > गन्ती - seen)) फर्काऊ असत्य;

      अचिन्हित width = 0;
      विकल्प (control & मानहरू_SIZE_MASK)
      {
	पक्ष मानहरू_ARE_ZEROS: रोक;
	पक्ष मानहरू_ARE_BYTES: width = HBINT8::static_size; रोक;
	पक्ष मानहरू_ARE_शब्दहरू: width = HBINT16::static_size; रोक;
	पक्ष मानहरू_ARE_LONGS: width = HBINT32::static_size; रोक;
      }
      यदि (unlikely (अचिन्हित (end - p) < run_गन्ती * width)) फर्काऊ असत्य;
      p += run_गन्ती * width;
      seen += run_गन्ती;
    }
    फर्काऊ सत्य;
  }

  संरचना iter_t : hb_iter_with_fallback_t<iter_t, पूर्णाङ्क>
  {
    iter_t (अचर अचिन्हित वर्ण *p_, अचिन्हित len_)
	    : p (p_), endp (p_ + len_)
    { यदि (likely (ensure_run ())) read_मान (); }

    गुप्त:
    अचर अचिन्हित वर्ण *p;
    अचर अचिन्हित वर्ण * अचर endp;
    पूर्णाङ्क current_मान = 0;
    चिन्हित run_गन्ती = 0;
    अचिन्हित width = 0;

    HB_ALWAYS_INLINE
    तार्किक ensure_run ()
    {
      यदि (likely (run_गन्ती > 0)) फर्काऊ सत्य;
      फर्काऊ _ensure_run ();
    }
    तार्किक _ensure_run ()
    {
      यदि (unlikely (p >= endp))
      {
        run_गन्ती = 0;
        current_मान = 0;
	फर्काऊ असत्य;
      }

      अचिन्हित control = *p++;
      run_गन्ती = (control & मान_RUN_गन्ती_MASK) + 1;
      width = control & मानहरू_SIZE_MASK;
      विकल्प (width)
      {
        पक्ष मानहरू_ARE_ZEROS: width = 0; रोक;
	पक्ष मानहरू_ARE_BYTES: width = HBINT8::static_size;  रोक;
	पक्ष मानहरू_ARE_शब्दहरू: width = HBINT16::static_size; रोक;
	पक्ष मानहरू_ARE_LONGS: width = HBINT32::static_size; रोक;
	स्वभाव: प्रतिज्ञा (असत्य);
      }

      यदि (unlikely (p + run_गन्ती * width > endp))
      {
	run_गन्ती = 0;
	current_मान = 0;
	फर्काऊ असत्य;
      }

      फर्काऊ सत्य;
    }
    शून्य read_मान ()
    {
      विकल्प (width)
      {
        पक्ष 0: current_मान = 0; रोक;
	पक्ष 1: current_मान = * (अचर HBINT8  *) p; रोक;
	पक्ष 2: current_मान = * (अचर HBINT16 *) p; रोक;
	पक्ष 4: current_मान = * (अचर HBINT32 *) p; रोक;
      }
      p += width;
    }

    सार्वजनिक:

    प्रकारनिर्देश पूर्णाङ्क __item_t__;
    __item_t__ __item__ () अचर
    { फर्काऊ current_मान; }

    तार्किक __more__ () अचर { फर्काऊ run_गन्ती || p < endp; }
    शून्य __next__ ()
    {
      run_गन्ती--;
      यदि (unlikely (!ensure_run ()))
	फर्काऊ;
      read_मान ();
    }
    शून्य __forward__ (अचिन्हित n)
    {
      यदि (unlikely (!ensure_run ()))
	फर्काऊ;
      जबसम्म (n)
      {
	अचिन्हित i = hb_min (n, (अचिन्हित) run_गन्ती);
	run_गन्ती -= i;
	n -= i;
	p += (i - 1) * width;
	यदि (unlikely (!ensure_run ()))
	  फर्काऊ;
	read_मान ();
      }
    }
    तार्किक चालक != (अचर iter_t& o) अचर
    { फर्काऊ p != o.p || run_गन्ती != o.run_गन्ती; }
    iter_t __end__ () अचर
    {
      iter_t it (endp, 0);
      फर्काऊ it;
    }
  };

  संरचना fetcher_t
  {
    fetcher_t (अचर अचिन्हित वर्ण *p_, अचिन्हित len_)
	      : p (p_), end (p_ + len_) {}

    गुप्त:
    अचर अचिन्हित वर्ण *p;
    अचर अचिन्हित वर्ण * अचर end;
    चिन्हित run_गन्ती = 0;
    अचिन्हित width = 0;

    HB_ALWAYS_INLINE
    तार्किक ensure_run ()
    {
      यदि (likely (run_गन्ती > 0)) फर्काऊ सत्य;
      फर्काऊ _ensure_run ();
    }

    तार्किक _ensure_run ()
    {
      यदि (unlikely (p >= end))
      {
        run_गन्ती = 0;
	फर्काऊ असत्य;
      }

      अचिन्हित control = *p++;
      run_गन्ती = (control & मान_RUN_गन्ती_MASK) + 1;
      width = control & मानहरू_SIZE_MASK;
      विकल्प (width)
      {
        पक्ष मानहरू_ARE_ZEROS: width = 0; रोक;
	पक्ष मानहरू_ARE_BYTES: width = HBINT8::static_size;  रोक;
	पक्ष मानहरू_ARE_शब्दहरू: width = HBINT16::static_size; रोक;
	पक्ष मानहरू_ARE_LONGS: width = HBINT32::static_size; रोक;
	स्वभाव: प्रतिज्ञा (असत्य);
      }

      यदि (unlikely (p + run_गन्ती * width > end))
      {
	run_गन्ती = 0;
	फर्काऊ असत्य;
      }

      फर्काऊ सत्य;
    }

    सार्वजनिक:
    शून्य skip (अचिन्हित n)
    {
      जबसम्म (n)
      {
	यदि (unlikely (!ensure_run ()))
	  फर्काऊ;
	अचिन्हित i = hb_min (n, (अचिन्हित) run_गन्ती);
	run_गन्ती -= i;
	n -= i;
	p += i * width;
      }
    }

    गुप्त:
    ढाँचा <तार्किक scaled, typename T>
    शून्य _add_to (hb_array_t<T> out, T scale = T (1))
    {
      अचिन्हित n = out.length;
      T *arrayZ = out.arrayZ;

      लागि (अचिन्हित i = 0; i < n;)
      {
	यदि (unlikely (!ensure_run ()))
	  रोक;
	अचिन्हित गन्ती = hb_min (n - i, (अचिन्हित) run_गन्ती);
	विकल्प (width)
	{
	  पक्ष 0:
	  {
	    arrayZ += गन्ती;
	    रोक;
	  }
	  पक्ष 1:
	  {
	    अचर स्वतः *pp = (अचर HBINT8 *) p;
	    अचिन्हित j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    लागि (; j + 3 < गन्ती; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    लागि (; j < गन्ती; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (अचर अचिन्हित वर्ण *) pp;
	  }
	  रोक;
	  पक्ष 2:
	  {
	    अचर स्वतः *pp = (अचर HBINT16 *) p;
	    अचिन्हित j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    लागि (; j + 3 < गन्ती; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    लागि (; j < गन्ती; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (अचर अचिन्हित वर्ण *) pp;
	  }
	  रोक;
	  पक्ष 4:
	  {
	    अचर स्वतः *pp = (अचर HBINT32 *) p;
	    लागि (अचिन्हित j = 0; j < गन्ती; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (अचर अचिन्हित वर्ण *) pp;
	  }
	  रोक;
	}
	run_गन्ती -= गन्ती;
	i += गन्ती;
      }
    }

    सार्वजनिक:
    ढाँचा <typename T>
    शून्य add_to (hb_array_t<T> out, भिन्न scale = 1.0f)
    {
#ifndef HB_OPTIMIZE_SIZE
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      यदि (असत्य && scale == 1.0f)
        _add_to<असत्य> (out);
      अन्यथा
#endif
        _add_to<सत्य> (out, (T) scale);
    }
  };
};

संरचना TupleList : CFF2Index
{
  Tuple_मानहरू::iter_t चालक [] (अचिन्हित i) अचर
  {
    स्वतः bytes = CFF2Index::चालक [] (i);
    फर्काऊ Tuple_मानहरू::iter_t (bytes.arrayZ, bytes.length);
  }

  Tuple_मानहरू::fetcher_t fetcher (अचिन्हित i) अचर
  {
    स्वतः bytes = CFF2Index::चालक [] (i);
    फर्काऊ Tuple_मानहरू::fetcher_t (bytes.arrayZ, bytes.length);
  }
};


// Alignment

ढाँचा <अचिन्हित पूर्णाङ्क alignment>
संरचना Align
{
  माप_प्रकार get_size (अचर शून्य *base) अचर
  {
    अचिन्हित offset = (अचर वर्ण *) एतत् - (अचर वर्ण *) base;
    फर्काऊ (alignment - offset) & (alignment - 1);
  }

  सार्वजनिक:
  DEFINE_SIZE_MIN (0);
};



} /* namespace OT */


#endif /* HB_OPEN_TYPE_HH */
