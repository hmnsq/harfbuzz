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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-blob.hh"
#অন্তর্ভাব "hb-face.hh"
#অন্তর্ভাব "hb-machinery.hh"
#অন্তর্ভাব "hb-meta.hh"
#অন্তর্ভাব "hb-subset.hh"


নামাকাশ OT {


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
template <যৌক্তিক BE,
	  typename Type,
	  unsigned পূর্ণসংখ্যা আকার_3 = আকার (Type)>
কাঠামো অঙ্ক_Type
{
  প্রকারলেখ Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  প্রকারলেখ typename std::conditional<std::is_integral<Type>::value && আকার (Type) <= আকার(পূর্ণসংখ্যা),
				     typename std::conditional<std::is_signed<Type>::value, signed, unsigned>::type,
				     Type>::type WideType;

  অঙ্ক_Type () = পূর্বনির্ধারিত;
  explicit constexpr অঙ্ক_Type (Type V) : v {V} {}
  অঙ্ক_Type& operator = (Type V) { v = V; ফেরত *এই; }

  operator WideType () ধ্রুবক { ফেরত v; }

  যৌক্তিক operator == (ধ্রুবক অঙ্ক_Type &o) ধ্রুবক { ফেরত (Type) v == (Type) o.v; }
  যৌক্তিক operator != (ধ্রুবক অঙ্ক_Type &o) ধ্রুবক { ফেরত !(*এই == o); }

  অঙ্ক_Type& operator += (WideType গুনতি) { *এই = *এই + গুনতি; ফেরত *এই; }
  অঙ্ক_Type& operator -= (WideType গুনতি) { *এই = *এই - গুনতি; ফেরত *এই; }
  অঙ্ক_Type& operator ++ () { *এই += 1; ফেরত *এই; }
  অঙ্ক_Type& operator -- () { *এই -= 1; ফেরত *এই; }
  অঙ্ক_Type operator ++ (পূর্ণসংখ্যা) { অঙ্ক_Type c (*এই); ++*এই; ফেরত c; }
  অঙ্ক_Type operator -- (পূর্ণসংখ্যা) { অঙ্ক_Type c (*এই); --*এই; ফেরত c; }

  uint32_t hash () ধ্রুবক { ফেরত hb_array ((ধ্রুবক অক্ষর *) &v, আকার (v)).hash (); }
  HB_INTERNAL স্থির পূর্ণসংখ্যা cmp (ধ্রুবক অঙ্ক_Type *a, ধ্রুবক অঙ্ক_Type *b)
  { ফেরত b->cmp (*a); }
  HB_INTERNAL স্থির পূর্ণসংখ্যা cmp (ধ্রুবক শূন্য *a, ধ্রুবক শূন্য *b)
  {
    অঙ্ক_Type *pa = (অঙ্ক_Type *) a;
    অঙ্ক_Type *pb = (অঙ্ক_Type *) b;

    ফেরত pb->cmp (*pa);
  }
  template <typename Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  পূর্ণসংখ্যা cmp (Type2 a) ধ্রুবক
  {
    Type b = v;
    ফেরত (a > b) - (a < b);
  }
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }
  সুরক্ষিত:
  typename std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, আকার_3>,
			    HBFloat<BE, Type, আকার_3>>::type v;
  সর্বজনীন:
  DEFINE_আকার_STATIC (আকার_3);
};

প্রকারলেখ অঙ্ক_Type<সত্য, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
প্রকারলেখ অঙ্ক_Type<সত্য, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
প্রকারলেখ অঙ্ক_Type<সত্য, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
প্রকারলেখ অঙ্ক_Type<সত্য, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
প্রকারলেখ অঙ্ক_Type<সত্য, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
প্রকারলেখ অঙ্ক_Type<সত্য, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
প্রকারলেখ অঙ্ক_Type<সত্য, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
প্রকারলেখ অঙ্ক_Type<সত্য, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
প্রকারলেখ অঙ্ক_Type<সত্য, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

প্রকারলেখ অঙ্ক_Type<মিথ্যা, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
প্রকারলেখ অঙ্ক_Type<মিথ্যা, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
প্রকারলেখ অঙ্ক_Type<মিথ্যা, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
প্রকারলেখ অঙ্ক_Type<মিথ্যা, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
প্রকারলেখ অঙ্ক_Type<মিথ্যা, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
প্রকারলেখ অঙ্ক_Type<মিথ্যা, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

প্রকারলেখ অঙ্ক_Type<সত্য,  দশমিক>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
প্রকারলেখ অঙ্ক_Type<সত্য,  দ্বিগুণ> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
প্রকারলেখ অঙ্ক_Type<মিথ্যা, দশমিক>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
প্রকারলেখ অঙ্ক_Type<মিথ্যা, দ্বিগুণ> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
কাঠামো HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& operator = (uint16_t i ) { HBUINT16::operator= (i); ফেরত *এই; }
  সর্বজনীন:
  DEFINE_আকার_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
কাঠামো HBUINT32VAR
{
  size_t get_আকার () ধ্রুবক
  {
    unsigned b0 = v[0];
    যদি (b0 < 0x80)
      ফেরত 1;
    অন্যথা যদি (b0 < 0xC0)
      ফেরত 2;
    অন্যথা যদি (b0 < 0xE0)
      ফেরত 3;
    অন্যথা যদি (b0 < 0xF0)
      ফেরত 4;
    অন্যথা
      ফেরত 5;
  }

  স্থির size_t get_আকার (uint32_t v)
  {
    যদি (v < 0x80)
      ফেরত 1;
    অন্যথা যদি (v < 0x4000)
      ফেরত 2;
    অন্যথা যদি (v < 0x200000)
      ফেরত 3;
    অন্যথা যদি (v < 0x10000000)
      ফেরত 4;
    অন্যথা
      ফেরত 5;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_আকার ()));
  }

  operator uint32_t () ধ্রুবক
  {
    unsigned b0 = v[0];
    যদি (b0 < 0x80)
      ফেরত b0;
    অন্যথা যদি (b0 < 0xC0)
      ফেরত ((b0 & 0x3F) << 8) | v[1];
    অন্যথা যদি (b0 < 0xE0)
      ফেরত ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    অন্যথা যদি (b0 < 0xF0)
      ফেরত ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    অন্যথা
      ফেরত (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  স্থির unsigned serialize_unsafe (unsigned অক্ষর *buf, uint32_t v)
  {
    unsigned len = get_আকার (v);
    unsigned অক্ষর *p = buf + len;
    জন্য (unsigned i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    যদি (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    ফেরত len;
  }

  স্থির যৌক্তিক serialize (hb_serialize_context_t *c, uint32_t v)
  {
    unsigned len = get_আকার (v);
    unsigned অক্ষর *buf = c->allocate_আকার<unsigned অক্ষর> (len, মিথ্যা);
    যদি (unlikely (!buf))
      ফেরত মিথ্যা;

    serialize_unsafe (buf, v);
    ফেরত সত্য;
  }

  সুরক্ষিত:
  unsigned অক্ষর v[5];

  সর্বজনীন:
  DEFINE_আকার_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
প্রকারলেখ HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
প্রকারলেখ HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
প্রকারলেখ HBUINT16 UFWORD;

template <typename Type, unsigned fraction_bits>
কাঠামো HBFixed : Type
{
  স্থির constexpr দশমিক mult = 1.f / (1 << fraction_bits);
  static_assert (Type::static_আকার * 8 > fraction_bits, "");

  operator signed () ধ্রুবক = delete;
  operator unsigned () ধ্রুবক = delete;
  explicit operator দশমিক () ধ্রুবক { ফেরত to_float (); }
  typename Type::type to_int () ধ্রুবক { ফেরত Type::v; }
  শূন্য set_int (typename Type::type i ) { Type::v = i; }
  দশমিক to_float (দশমিক offset = 0) ধ্রুবক  { ফেরত ((int32_t) Type::v + offset) * mult; }
  শূন্য set_float (দশমিক f) { Type::v = roundf (f / mult); }
  সর্বজনীন:
  DEFINE_আকার_STATIC (Type::static_আকার);
};

/* 16-bit signed fixed number with the low 14 bits of fraction (2.14). */
using F2DOT14 = HBFixed<HBINT16, 14>;
using F4DOT12 = HBFixed<HBINT16, 12>;
using F6DOT10 = HBFixed<HBINT16, 10>;

/* 32-bit signed fixed-point number (16.16). */
using F16DOT16 = HBFixed<HBINT32, 16>;

/* Date represented in number of seconds since 12:00 midnight, January 1,
 * 1904. The value is represented as a signed 64-bit integer. */
কাঠামো LONGDATETIME
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }
  সুরক্ষিত:
  HBINT32 major;
  HBUINT32 minor;
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
কাঠামো Tag : HBUINT32
{
  Tag& operator = (hb_tag_t i) { HBUINT32::operator= (i); ফেরত *এই; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  operator ধ্রুবক অক্ষর* () ধ্রুবক { ফেরত reinterpret_cast<ধ্রুবক অক্ষর *> (এই); }
  operator অক্ষর* ()             { ফেরত reinterpret_cast<অক্ষর *> (এই); }
  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
কাঠামো HBGlyphID16 : HBUINT16
{
  HBGlyphID16& operator = (uint16_t i) { HBUINT16::operator= (i); ফেরত *এই; }
};
কাঠামো HBGlyphID24 : HBUINT24
{
  HBGlyphID24& operator = (uint32_t i) { HBUINT24::operator= (i); ফেরত *এই; }
};

/* Script/language-system/feature index */
কাঠামো সূচক : HBUINT16 {
  স্থির constexpr unsigned NOT_FOUND_সূচক = 0xFFFFu;
  সূচক& operator = (uint16_t i) { HBUINT16::operator= (i); ফেরত *এই; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, সূচক);

প্রকারলেখ সূচক NameID;

কাঠামো VarIdx : HBUINT32 {
  স্থির constexpr unsigned NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_সূচক, "");
  VarIdx& operator = (uint32_t i) { HBUINT32::operator= (i); ফেরত *এই; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, VarIdx);

/* Offset, Null offset = 0 */
template <typename Type, যৌক্তিক has_null=সত্য>
কাঠামো Offset : Type
{
  Offset& operator = (typename Type::type i) { Type::operator= (i); ফেরত *এই; }

  প্রকারলেখ Type type;

  যৌক্তিক is_null () ধ্রুবক { ফেরত has_null && 0 == *এই; }

  সর্বজনীন:
  DEFINE_আকার_STATIC (আকার (Type));
};

প্রকারলেখ Offset<HBUINT16> Offset16;
প্রকারলেখ Offset<HBUINT24> Offset24;
প্রকারলেখ Offset<HBUINT32> Offset32;


/* CheckSum */
কাঠামো Check_যোগ : HBUINT32
{
  Check_যোগ& operator = (uint32_t i) { HBUINT32::operator= (i); ফেরত *এই; }

  /* This is reference implementation from the spec. */
  স্থির uint32_t CalcTableChecksum (ধ্রুবক HBUINT32 *Table, uint32_t দৈর্ঘ্য_3)
  {
    uint32_t Sum = 0L;
    প্রতিজ্ঞা (0 == (দৈর্ঘ্য_3 & 3));
    ধ্রুবক HBUINT32 *EndPtr = Table + দৈর্ঘ্য_3 / HBUINT32::static_আকার;

    যখন (Table < EndPtr)
      Sum += *Table++;
    ফেরত Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  শূন্য set_for_তথ্য (ধ্রুবক শূন্য *তথ্য, unsigned পূর্ণসংখ্যা দৈর্ঘ্য)
  { *এই = CalcTableChecksum ((ধ্রুবক HBUINT32 *) তথ্য, দৈর্ঘ্য); }

  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};


/*
 * Version Numbers
 */

template <typename FixedType=HBUINT16>
কাঠামো FixedVersion
{
  uint32_t to_int () ধ্রুবক { ফেরত (major << (আকার (FixedType) * 8)) + minor; }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  FixedType major;
  FixedType minor;
  সর্বজনীন:
  DEFINE_আকার_STATIC (2 * আকার (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

template <typename Type, যৌক্তিক has_null>
কাঠামো _hb_has_null
{
  স্থির ধ্রুবক Type *get_null () { ফেরত nullptr; }
  স্থির Type *get_crap ()       { ফেরত nullptr; }
};
template <typename Type>
কাঠামো _hb_has_null<Type, সত্য>
{
  স্থির ধ্রুবক Type *get_null () { ফেরত &Null (Type); }
  স্থির       Type *get_crap () { ফেরত &Crap (Type); }
};

template <typename Type, typename OffsetType, typename BaseType=শূন্য, যৌক্তিক has_null=সত্য>
কাঠামো OffsetTo : Offset<OffsetType, has_null>
{
  using target_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == মিথ্যা ||
		 (hb_has_null_আকার_2 (Type) || !hb_has_min_আকার_2 (Type)), "");

  HB_DELETE_COPY_ASSIGN (OffsetTo);
  OffsetTo () = পূর্বনির্ধারিত;

  OffsetTo& operator = (typename OffsetType::type i) { OffsetType::operator= (i); ফেরত *এই; }

  ধ্রুবক Type& operator () (ধ্রুবক শূন্য *base) ধ্রুবক
  {
    যদি (unlikely (এই->is_null ())) ফেরত *_hb_has_null<Type, has_null>::get_null ();
    ফেরত StructAtOffset<ধ্রুবক Type> (base, *এই);
  }
  Type& operator () (শূন্য *base) ধ্রুবক
  {
    যদি (unlikely (এই->is_null ())) ফেরত *_hb_has_null<Type, has_null>::get_crap ();
    ফেরত StructAtOffset<Type> (base, *এই);
  }

  template <typename Base,
	    hb_enable_if (hb_is_convertible (ধ্রুবক Base, ধ্রুবক BaseType *))>
  friend ধ্রুবক Type& operator + (ধ্রুবক Base &base, ধ্রুবক OffsetTo &offset) { ফেরত offset ((ধ্রুবক শূন্য *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (ধ্রুবক Base, ধ্রুবক BaseType *))>
  friend ধ্রুবক Type& operator + (ধ্রুবক OffsetTo &offset, ধ্রুবক Base &base) { ফেরত offset ((ধ্রুবক শূন্য *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (Base &&base, OffsetTo &offset) { ফেরত offset ((শূন্য *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (OffsetTo &offset, Base &&base) { ফেরত offset ((শূন্য *) base); }


  template <typename Base, typename ...Ts>
  যৌক্তিক serialize_subset (hb_subset_context_t *c, ধ্রুবক OffsetTo& src,
			 ধ্রুবক Base *src_base, Ts&&... ds)
  {
    *এই = 0;
    যদি (src.is_null ())
      ফেরত মিথ্যা;

    auto *s = c->serializer;

    s->push ();

    যৌক্তিক ret = c->dispatch (src_base+src, std::forward<Ts> (ds)...);

    যদি (ret || !has_null)
      s->add_link (*এই, s->বের_করো_pack ());
    অন্যথা
      s->বের_করো_discard ();

    ফেরত ret;
  }


  template <typename ...Ts>
  যৌক্তিক serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *এই = 0;

    Type* obj = c->push<Type> ();
    যৌক্তিক ret = obj->serialize (c, std::forward<Ts> (ds)...);

    যদি (ret)
      c->add_link (*এই, c->বের_করো_pack ());
    অন্যথা
      c->বের_করো_discard ();

    ফেরত ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  template <typename ...Ts>
  যৌক্তিক serialize_copy (hb_serialize_context_t *c, ধ্রুবক OffsetTo& src,
		       ধ্রুবক শূন্য *src_base, unsigned dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *এই = 0;
    যদি (src.is_null ())
      ফেরত মিথ্যা;

    c->push ();

    যৌক্তিক ret = c->copy (src_base+src, std::forward<Ts> (ds)...);

    c->add_link (*এই, c->বের_করো_pack (), whence, dst_bias);

    ফেরত ret;
  }

  যৌক্তিক serialize_copy (hb_serialize_context_t *c, ধ্রুবক OffsetTo& src,
		       ধ্রুবক শূন্য *src_base, unsigned dst_bias = 0)
  { ফেরত serialize_copy (c, src, src_base, dst_bias, hb_serialize_context_t::শীর্ষ); }

  যৌক্তিক sanitize_shallow (hb_sanitize_context_t *c, ধ্রুবক BaseType *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!c->check_struct (এই))) return_trace (মিথ্যা);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    যদি (unlikely ((ধ্রুবক অক্ষর *) base + (unsigned) *এই < (ধ্রুবক অক্ষর *) base)) return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  template <typename ...Ts>
#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক BaseType *base, Ts&&... ds) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (sanitize_shallow (c, base) &&
		  hb_barrier () &&
		  (এই->is_null () ||
		   c->dispatch (StructAtOffset<Type> (base, *এই), std::forward<Ts> (ds)...)));
  }

  DEFINE_আকার_STATIC (আকার (OffsetType));
};
/* Partial specializations. */
template <typename Type, typename BaseType=শূন্য, যৌক্তিক has_null=সত্য> using Offset16To = OffsetTo<Type, HBUINT16, BaseType, has_null>;
template <typename Type, typename BaseType=শূন্য, যৌক্তিক has_null=সত্য> using Offset24To = OffsetTo<Type, HBUINT24, BaseType, has_null>;
template <typename Type, typename BaseType=শূন্য, যৌক্তিক has_null=সত্য> using Offset32To = OffsetTo<Type, HBUINT32, BaseType, has_null>;

template <typename Type, typename OffsetType, typename BaseType=শূন্য> using NNOffsetTo = OffsetTo<Type, OffsetType, BaseType, মিথ্যা>;
template <typename Type, typename BaseType=শূন্য> using NNOffset16To = Offset16To<Type, BaseType, মিথ্যা>;
template <typename Type, typename BaseType=শূন্য> using NNOffset24To = Offset24To<Type, BaseType, মিথ্যা>;
template <typename Type, typename BaseType=শূন্য> using NNOffset32To = Offset32To<Type, BaseType, মিথ্যা>;


/*
 * Array Types
 */

template <typename Type>
কাঠামো UnsizedArrayOf
{
  প্রকারলেখ Type item_t;
  স্থির constexpr unsigned item_আকার = hb_static_আকার (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (UnsizedArrayOf);

  ধ্রুবক Type& operator [] (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  {
    ফেরত arrayZ[i];
  }
  Type& operator [] (unsigned পূর্ণসংখ্যা i)
  {
    ফেরত arrayZ[i];
  }

  স্থির size_t get_আকার (unsigned পূর্ণসংখ্যা len)
  { ফেরত hb_unsigned_mul_saturate (len, Type::static_আকার); }

  template <typename T> operator T * () { ফেরত arrayZ; }
  template <typename T> operator ধ্রুবক T * () ধ্রুবক { ফেরত arrayZ; }
  hb_array_t<Type> as_array (unsigned পূর্ণসংখ্যা len)
  { ফেরত hb_array (arrayZ, len); }
  hb_array_t<ধ্রুবক Type> as_array (unsigned পূর্ণসংখ্যা len) ধ্রুবক
  { ফেরত hb_array (arrayZ, len); }

  template <typename T>
  Type &lsearch (unsigned পূর্ণসংখ্যা len, ধ্রুবক T &x, Type &not_found = Crap (Type))
  { ফেরত *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  ধ্রুবক Type &lsearch (unsigned পূর্ণসংখ্যা len, ধ্রুবক T &x, ধ্রুবক Type &not_found = Null (Type)) ধ্রুবক
  { ফেরত *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  যৌক্তিক lfind (unsigned পূর্ণসংখ্যা len, ধ্রুবক T &x, unsigned পূর্ণসংখ্যা *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned পূর্ণসংখ্যা to_store = (unsigned পূর্ণসংখ্যা) -1) ধ্রুবক
  { ফেরত as_array (len).lfind (x, i, not_found, to_store); }

  শূন্য qsort (unsigned পূর্ণসংখ্যা len, unsigned পূর্ণসংখ্যা start = 0, unsigned পূর্ণসংখ্যা end = (unsigned পূর্ণসংখ্যা) -1)
  { as_array (len).qsort (start, end); }

  যৌক্তিক serialize (hb_serialize_context_t *c, unsigned পূর্ণসংখ্যা বস্তুসমূহ_len, যৌক্তিক clear = সত্য)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_আকার (এই, get_আকার (বস্তুসমূহ_len), clear))) return_trace (মিথ্যা);
    return_trace (সত্য);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  যৌক্তিক serialize (hb_serialize_context_t *c, Iterator বস্তুসমূহ)
  {
    TRACE_SERIALIZE (এই);
    unsigned গুনতি = hb_len (বস্তুসমূহ);
    যদি (unlikely (!serialize (c, গুনতি, মিথ্যা))) return_trace (মিথ্যা);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    জন্য (unsigned i = 0; i < গুনতি; i++, ++বস্তুসমূহ)
      arrayZ[i] = *বস্তুসমূহ;
    return_trace (সত্য);
  }

  UnsizedArrayOf* copy (hb_serialize_context_t *c, unsigned গুনতি) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto *out = c->start_embed (এই);
    যদি (unlikely (!as_array (গুনতি).copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  যৌক্তিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণসংখ্যা গুনতি, Ts&&... ds) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!sanitize_shallow (c, গুনতি))) return_trace (মিথ্যা);
    যদি (!আকার... (Ts) && hb_is_trivially_copyable(Type)) return_trace (সত্য);
    hb_barrier ();
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  যৌক্তিক sanitize_shallow (hb_sanitize_context_t *c, unsigned পূর্ণসংখ্যা গুনতি) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_array (arrayZ, গুনতি));
  }

  সর্বজনীন:
  Type		arrayZ[HB_VAR_ARRAY];
  সর্বজনীন:
  DEFINE_আকার_UNBOUNDED (0);
};

/* Unsized array of offset's */
template <typename Type, typename OffsetType, typename BaseType=শূন্য, যৌক্তিক has_null=সত্য>
using UnsizedArray16OfOffsetTo = UnsizedArrayOf<OffsetTo<Type, OffsetType, BaseType, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
template <typename Type, typename OffsetType, typename BaseType=শূন্য, যৌক্তিক has_null=সত্য>
কাঠামো UnsizedListOfOffset16To : UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
{
  ধ্রুবক Type& operator [] (পূর্ণসংখ্যা i_) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা i = (unsigned পূর্ণসংখ্যা) i_;
    ধ্রুবক OffsetTo<Type, OffsetType, BaseType, has_null> *p = &এই->arrayZ[i];
    যদি (unlikely ((ধ্রুবক শূন্য *) p < (ধ্রুবক শূন্য *) এই->arrayZ)) ফেরত Null (Type); /* Overflowed. */
    hb_barrier ();
    ফেরত এই+*p;
  }
  Type& operator [] (পূর্ণসংখ্যা i_)
  {
    unsigned পূর্ণসংখ্যা i = (unsigned পূর্ণসংখ্যা) i_;
    ধ্রুবক OffsetTo<Type, OffsetType, BaseType, has_null> *p = &এই->arrayZ[i];
    যদি (unlikely ((ধ্রুবক শূন্য *) p < (ধ্রুবক শূন্য *) এই->arrayZ)) ফেরত Crap (Type); /* Overflowed. */
    hb_barrier ();
    ফেরত এই+*p;
  }

  template <typename ...Ts>
  যৌক্তিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণসংখ্যা গুনতি, Ts&&... ds) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace ((UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
		   ::sanitize (c, গুনতি, এই, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type>
কাঠামো SortedUnsizedArrayOf : UnsizedArrayOf<Type>
{
  hb_sorted_array_t<Type> as_array (unsigned পূর্ণসংখ্যা len)
  { ফেরত hb_sorted_array (এই->arrayZ, len); }
  hb_sorted_array_t<ধ্রুবক Type> as_array (unsigned পূর্ণসংখ্যা len) ধ্রুবক
  { ফেরত hb_sorted_array (এই->arrayZ, len); }
  operator hb_sorted_array_t<Type> ()             { ফেরত as_array (); }
  operator hb_sorted_array_t<ধ্রুবক Type> () ধ্রুবক { ফেরত as_array (); }

  template <typename T>
  Type &bsearch (unsigned পূর্ণসংখ্যা len, ধ্রুবক T &x, Type &not_found = Crap (Type))
  { ফেরত *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  ধ্রুবক Type &bsearch (unsigned পূর্ণসংখ্যা len, ধ্রুবক T &x, ধ্রুবক Type &not_found = Null (Type)) ধ্রুবক
  { ফেরত *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  যৌক্তিক bfind (unsigned পূর্ণসংখ্যা len, ধ্রুবক T &x, unsigned পূর্ণসংখ্যা *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned পূর্ণসংখ্যা to_store = (unsigned পূর্ণসংখ্যা) -1) ধ্রুবক
  { ফেরত as_array (len).bfind (x, i, not_found, to_store); }
};


/* An array with a number of elements. */
template <typename Type, typename LenType>
কাঠামো ArrayOf
{
  প্রকারলেখ Type item_t;
  স্থির constexpr unsigned item_আকার = hb_static_আকার (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOf);

  ধ্রুবক Type& operator [] (পূর্ণসংখ্যা i_) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা i = (unsigned পূর্ণসংখ্যা) i_;
    যদি (unlikely (i >= len)) ফেরত Null (Type);
    hb_barrier ();
    ফেরত arrayZ[i];
  }
  Type& operator [] (পূর্ণসংখ্যা i_)
  {
    unsigned পূর্ণসংখ্যা i = (unsigned পূর্ণসংখ্যা) i_;
    যদি (unlikely (i >= len)) ফেরত Crap (Type);
    hb_barrier ();
    ফেরত arrayZ[i];
  }

  size_t get_আকার () ধ্রুবক
  { ফেরত hb_unsigned_mul_add_saturate (len, Type::static_আকার, len.static_আকার); }

  explicit operator যৌক্তিক () ধ্রুবক { ফেরত len; }

  শূন্য বের_করো () { len--; }

  hb_array_t<      Type> as_array ()       { ফেরত hb_array (arrayZ, len); }
  hb_array_t<ধ্রুবক Type> as_array () ধ্রুবক { ফেরত hb_array (arrayZ, len); }

  /* Iterator. */
  প্রকারলেখ hb_array_t<ধ্রুবক Type>   iter_t;
  প্রকারলেখ hb_array_t<      Type> writer_t;
    iter_t   iter () ধ্রুবক { ফেরত as_array (); }
  writer_t writer ()       { ফেরত as_array (); }
  operator   iter_t () ধ্রুবক { ফেরত   iter (); }
  operator writer_t ()       { ফেরত writer (); }

  /* Faster range-based for loop. */
  ধ্রুবক Type *begin () ধ্রুবক { ফেরত arrayZ; }
  ধ্রুবক Type *end () ধ্রুবক { ফেরত arrayZ + len; }

  template <typename T>
  Type &lsearch (ধ্রুবক T &x, Type &not_found = Crap (Type))
  { ফেরত *as_array ().lsearch (x, &not_found); }
  template <typename T>
  ধ্রুবক Type &lsearch (ধ্রুবক T &x, ধ্রুবক Type &not_found = Null (Type)) ধ্রুবক
  { ফেরত *as_array ().lsearch (x, &not_found); }
  template <typename T>
  যৌক্তিক lfind (ধ্রুবক T &x, unsigned পূর্ণসংখ্যা *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned পূর্ণসংখ্যা to_store = (unsigned পূর্ণসংখ্যা) -1) ধ্রুবক
  { ফেরত as_array ().lfind (x, i, not_found, to_store); }

  শূন্য qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD যৌক্তিক serialize (hb_serialize_context_t *c, unsigned বস্তুসমূহ_len, যৌক্তিক clear = সত্য)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    c->check_assign (len, বস্তুসমূহ_len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    যদি (unlikely (!c->extend_আকার (এই, get_আকার (), clear))) return_trace (মিথ্যা);
    return_trace (সত্য);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD যৌক্তিক serialize (hb_serialize_context_t *c, Iterator বস্তুসমূহ)
  {
    TRACE_SERIALIZE (এই);
    unsigned গুনতি = hb_len (বস্তুসমূহ);
    যদি (unlikely (!serialize (c, গুনতি, মিথ্যা))) return_trace (মিথ্যা);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    জন্য (unsigned i = 0; i < গুনতি; i++, ++বস্তুসমূহ)
      arrayZ[i] = *বস্তুসমূহ;
    return_trace (সত্য);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (এই);
    len++;
    যদি (unlikely (!len || !c->extend (এই)))
    {
      len--;
      return_trace (nullptr);
    }
    return_trace (&arrayZ[len - 1]);
  }

  ArrayOf* copy (hb_serialize_context_t *c) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto *out = c->start_embed (এই);
    যদি (unlikely (!c->extend_min (out))) return_trace (nullptr);
    c->check_assign (out->len, len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    যদি (unlikely (!as_array ().copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  যৌক্তিক sanitize (hb_sanitize_context_t *c, Ts&&... ds) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!sanitize_shallow (c))) return_trace (মিথ্যা);
    যদি (!আকার... (Ts) && hb_is_trivially_copyable(Type)) return_trace (সত্য);
    hb_barrier ();
    unsigned পূর্ণসংখ্যা গুনতি = len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  যৌক্তিক sanitize_shallow (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (len.sanitize (c) &&
		  hb_barrier () &&
		  c->check_array_sized (arrayZ, len, আকার (LenType)));
  }

  সর্বজনীন:
  LenType	len;
  Type		arrayZ[HB_VAR_ARRAY];
  সর্বজনীন:
  DEFINE_আকার_ARRAY (আকার (LenType), arrayZ);
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
কাঠামো List16OfOffsetTo : ArrayOf<OffsetTo<Type, OffsetType>, HBUINT16>
{
  ধ্রুবক Type& operator [] (পূর্ণসংখ্যা i_) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা i = (unsigned পূর্ণসংখ্যা) i_;
    যদি (unlikely (i >= এই->len)) ফেরত Null (Type);
    hb_barrier ();
    ফেরত এই+এই->arrayZ[i];
  }
  ধ্রুবক Type& operator [] (পূর্ণসংখ্যা i_)
  {
    unsigned পূর্ণসংখ্যা i = (unsigned পূর্ণসংখ্যা) i_;
    যদি (unlikely (i >= এই->len)) ফেরত Crap (Type);
    hb_barrier ();
    ফেরত এই+এই->arrayZ[i];
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    কাঠামো List16OfOffsetTo *out = c->serializer->embed (*এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);
    unsigned পূর্ণসংখ্যা গুনতি = এই->len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      out->arrayZ[i].serialize_subset (c, এই->arrayZ[i], এই, out);
    return_trace (সত্য);
  }

  template <typename ...Ts>
  যৌক্তিক sanitize (hb_sanitize_context_t *c, Ts&&... ds) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace ((Array16Of<OffsetTo<Type, OffsetType>>::sanitize (c, এই, std::forward<Ts> (ds)...)));
  }
};

template <typename Type>
using List16OfOffset16To = List16OfOffsetTo<Type, HBUINT16>;

/* An array starting at second element. */
template <typename Type, typename LenType>
কাঠামো HeadlessArrayOf
{
  স্থির constexpr unsigned item_আকার = Type::static_আকার;

  HB_DELETE_CREATE_COPY_ASSIGN (HeadlessArrayOf);

  ধ্রুবক Type& operator [] (পূর্ণসংখ্যা i_) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা i = (unsigned পূর্ণসংখ্যা) i_;
    যদি (unlikely (i >= lenP1 || !i)) ফেরত Null (Type);
    hb_barrier ();
    ফেরত arrayZ[i-1];
  }
  Type& operator [] (পূর্ণসংখ্যা i_)
  {
    unsigned পূর্ণসংখ্যা i = (unsigned পূর্ণসংখ্যা) i_;
    যদি (unlikely (i >= lenP1 || !i)) ফেরত Crap (Type);
    hb_barrier ();
    ফেরত arrayZ[i-1];
  }
  size_t get_আকার () ধ্রুবক
  { ফেরত hb_unsigned_mul_add_saturate (get_দৈর্ঘ্য (), Type::static_আকার, lenP1.static_আকার); }

  unsigned get_দৈর্ঘ্য () ধ্রুবক { ফেরত lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { ফেরত hb_array (arrayZ, get_দৈর্ঘ্য ()); }
  hb_array_t<ধ্রুবক Type> as_array () ধ্রুবক { ফেরত hb_array (arrayZ, get_দৈর্ঘ্য ()); }

  /* Iterator. */
  প্রকারলেখ hb_array_t<ধ্রুবক Type>   iter_t;
  প্রকারলেখ hb_array_t<      Type> writer_t;
    iter_t   iter () ধ্রুবক { ফেরত as_array (); }
  writer_t writer ()       { ফেরত as_array (); }
  operator   iter_t () ধ্রুবক { ফেরত   iter (); }
  operator writer_t ()       { ফেরত writer (); }

  /* Faster range-based for loop. */
  ধ্রুবক Type *begin () ধ্রুবক { ফেরত arrayZ; }
  ধ্রুবক Type *end () ধ্রুবক { ফেরত arrayZ + get_দৈর্ঘ্য (); }

  HB_NODISCARD যৌক্তিক serialize (hb_serialize_context_t *c, unsigned পূর্ণসংখ্যা বস্তুসমূহ_len, যৌক্তিক clear = সত্য)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    c->check_assign (lenP1, বস্তুসমূহ_len + 1, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    যদি (unlikely (!c->extend_আকার (এই, get_আকার (), clear))) return_trace (মিথ্যা);
    return_trace (সত্য);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD যৌক্তিক serialize (hb_serialize_context_t *c, Iterator বস্তুসমূহ)
  {
    TRACE_SERIALIZE (এই);
    unsigned গুনতি = hb_len (বস্তুসমূহ);
    যদি (unlikely (!serialize (c, গুনতি, মিথ্যা))) return_trace (মিথ্যা);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    জন্য (unsigned i = 0; i < গুনতি; i++, ++বস্তুসমূহ)
      arrayZ[i] = *বস্তুসমূহ;
    return_trace (সত্য);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  যৌক্তিক sanitize (hb_sanitize_context_t *c, Ts&&... ds) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!sanitize_shallow (c))) return_trace (মিথ্যা);
    যদি (!আকার... (Ts) && hb_is_trivially_copyable(Type)) return_trace (সত্য);
    hb_barrier ();
    unsigned পূর্ণসংখ্যা গুনতি = get_দৈর্ঘ্য ();
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  ব্যক্তিগত:
  যৌক্তিক sanitize_shallow (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (lenP1.sanitize (c) &&
		  hb_barrier () &&
		  (!lenP1 || c->check_array_sized (arrayZ, lenP1 - 1, আকার (LenType))));
  }

  সর্বজনীন:
  LenType	lenP1;
  Type		arrayZ[HB_VAR_ARRAY];
  সর্বজনীন:
  DEFINE_আকার_ARRAY (আকার (LenType), arrayZ);
};
template <typename Type> using HeadlessArray16Of = HeadlessArrayOf<Type, HBUINT16>;

/* An array storing length-1. */
template <typename Type, typename LenType=HBUINT16>
কাঠামো ArrayOfM1
{
  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOfM1);

  ধ্রুবক Type& operator [] (পূর্ণসংখ্যা i_) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা i = (unsigned পূর্ণসংখ্যা) i_;
    যদি (unlikely (i > lenM1)) ফেরত Null (Type);
    hb_barrier ();
    ফেরত arrayZ[i];
  }
  Type& operator [] (পূর্ণসংখ্যা i_)
  {
    unsigned পূর্ণসংখ্যা i = (unsigned পূর্ণসংখ্যা) i_;
    যদি (unlikely (i > lenM1)) ফেরত Crap (Type);
    hb_barrier ();
    ফেরত arrayZ[i];
  }
  size_t get_আকার () ধ্রুবক
  { ফেরত hb_unsigned_mul_add_saturate (lenM1 + 1, Type::static_আকার, lenM1.static_আকার); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  যৌক্তিক sanitize (hb_sanitize_context_t *c, Ts&&... ds) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!sanitize_shallow (c))) return_trace (মিথ্যা);
    যদি (!আকার... (Ts) && hb_is_trivially_copyable(Type)) return_trace (সত্য);
    hb_barrier ();
    unsigned পূর্ণসংখ্যা গুনতি = lenM1 + 1;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  ব্যক্তিগত:
  যৌক্তিক sanitize_shallow (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (lenM1.sanitize (c) &&
		  hb_barrier () &&
		  (c->check_array_sized (arrayZ, lenM1 + 1, আকার (LenType))));
  }

  সর্বজনীন:
  LenType	lenM1;
  Type		arrayZ[HB_VAR_ARRAY];
  সর্বজনীন:
  DEFINE_আকার_ARRAY (আকার (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type, typename LenType>
কাঠামো SortedArrayOf : ArrayOf<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { ফেরত hb_sorted_array (এই->arrayZ, এই->len); }
  hb_sorted_array_t<ধ্রুবক Type> as_array () ধ্রুবক { ফেরত hb_sorted_array (এই->arrayZ, এই->len); }

  /* Iterator. */
  প্রকারলেখ hb_sorted_array_t<ধ্রুবক Type>   iter_t;
  প্রকারলেখ hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () ধ্রুবক { ফেরত as_array (); }
  writer_t writer ()       { ফেরত as_array (); }
  operator   iter_t () ধ্রুবক { ফেরত   iter (); }
  operator writer_t ()       { ফেরত writer (); }

  /* Faster range-based for loop. */
  ধ্রুবক Type *begin () ধ্রুবক { ফেরত এই->arrayZ; }
  ধ্রুবক Type *end () ধ্রুবক { ফেরত এই->arrayZ + এই->len; }

  যৌক্তিক serialize (hb_serialize_context_t *c, unsigned পূর্ণসংখ্যা বস্তুসমূহ_len)
  {
    TRACE_SERIALIZE (এই);
    যৌক্তিক ret = ArrayOf<Type, LenType>::serialize (c, বস্তুসমূহ_len);
    return_trace (ret);
  }
  template <typename Iterator,
	    hb_requires (hb_is_sorted_source_of (Iterator, Type))>
  যৌক্তিক serialize (hb_serialize_context_t *c, Iterator বস্তুসমূহ)
  {
    TRACE_SERIALIZE (এই);
    যৌক্তিক ret = ArrayOf<Type, LenType>::serialize (c, বস্তুসমূহ);
    return_trace (ret);
  }

  SortedArrayOf* copy (hb_serialize_context_t *c) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    SortedArrayOf* out = reinterpret_cast<SortedArrayOf *> (ArrayOf<Type, LenType>::copy (c));
    return_trace (out);
  }

  template <typename T>
  Type &bsearch (ধ্রুবক T &x, Type &not_found = Crap (Type))
  { ফেরত *as_array ().bsearch (x, &not_found); }
  template <typename T>
  ধ্রুবক Type &bsearch (ধ্রুবক T &x, ধ্রুবক Type &not_found = Null (Type)) ধ্রুবক
  { ফেরত *as_array ().bsearch (x, &not_found); }
  template <typename T>
  যৌক্তিক bfind (ধ্রুবক T &x, unsigned পূর্ণসংখ্যা *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned পূর্ণসংখ্যা to_store = (unsigned পূর্ণসংখ্যা) -1) ধ্রুবক
  { ফেরত as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type> using SortedArray16Of = SortedArrayOf<Type, HBUINT16>;
template <typename Type> using SortedArray24Of = SortedArrayOf<Type, HBUINT24>;
template <typename Type> using SortedArray32Of = SortedArrayOf<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

template <typename LenType=HBUINT16>
কাঠামো Bin_অনুসন্ধান_Header
{
  operator uint32_t () ধ্রুবক { ফেরত len; }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  Bin_অনুসন্ধান_Header& operator = (unsigned পূর্ণসংখ্যা v)
  {
    len = v;
    প্রতিজ্ঞা (len == v);
    entrySelector = hb_max (1u, hb_bit_storage (v)) - 1;
    অনুসন্ধান_Range = 16 * (1u << entrySelector);
    rangeShift = v * 16 > অনুসন্ধান_Range
		 ? 16 * v - অনুসন্ধান_Range
		 : 0;
    ফেরত *এই;
  }

  সুরক্ষিত:
  LenType	len;
  LenType	অনুসন্ধান_Range;
  LenType	entrySelector;
  LenType	rangeShift;

  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

template <typename Type, typename LenType=HBUINT16>
using Bin_অনুসন্ধান_Array_Of = SortedArrayOf<Type, Bin_অনুসন্ধান_Header<LenType>>;


কাঠামো Var_Sized_Bin_অনুসন্ধান_Header
{

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  HBUINT16	unit_আকার;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	অনুসন্ধান_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	entrySelector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	rangeShift;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (10);
};

template <typename Type>
কাঠামো Var_Sized_Bin_অনুসন্ধান_Array_Of
{
  স্থির constexpr unsigned item_আকার = Type::static_আকার;

  HB_DELETE_CREATE_COPY_ASSIGN (Var_Sized_Bin_অনুসন্ধান_Array_Of);

  যৌক্তিক last_is_terminator () ধ্রুবক
  {
    যদি (unlikely (!header.nUnits)) ফেরত মিথ্যা;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    ধ্রুবক HBUINT16 *শব্দগুলি = &StructAtOffset<HBUINT16> (&bytesZ, (header.nUnits - 1) * header.unit_আকার);
    unsigned পূর্ণসংখ্যা গুনতি = Type::Termination_Word_গুনতি;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (শব্দগুলি[i] != 0xFFFFu)
	ফেরত মিথ্যা;
    ফেরত সত্য;
  }

  ধ্রুবক Type& operator [] (পূর্ণসংখ্যা i_) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা i = (unsigned পূর্ণসংখ্যা) i_;
    যদি (unlikely (i >= get_দৈর্ঘ্য ())) ফেরত Null (Type);
    hb_barrier ();
    ফেরত StructAtOffset<Type> (&bytesZ, i * header.unit_আকার);
  }
  Type& operator [] (পূর্ণসংখ্যা i_)
  {
    unsigned পূর্ণসংখ্যা i = (unsigned পূর্ণসংখ্যা) i_;
    যদি (unlikely (i >= get_দৈর্ঘ্য ())) ফেরত Crap (Type);
    hb_barrier ();
    ফেরত StructAtOffset<Type> (&bytesZ, i * header.unit_আকার);
  }
  unsigned পূর্ণসংখ্যা get_দৈর্ঘ্য () ধ্রুবক
  { ফেরত header.nUnits - last_is_terminator (); }
  size_t get_আকার () ধ্রুবক
  { ফেরত hb_unsigned_mul_add_saturate (header.nUnits, header.unit_আকার, header.static_আকার); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  যৌক্তিক sanitize (hb_sanitize_context_t *c, Ts&&... ds) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!sanitize_shallow (c))) return_trace (মিথ্যা);
    যদি (!আকার... (Ts) && hb_is_trivially_copyable(Type)) return_trace (সত্য);
    hb_barrier ();
    unsigned পূর্ণসংখ্যা গুনতি = get_দৈর্ঘ্য ();
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (unlikely (!(*এই)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  template <typename T>
  ধ্রুবক Type *bsearch (ধ্রুবক T &key) ধ্রুবক
  {
    unsigned pos;
    ফেরত hb_bsearch_impl (&pos,
			    key,
			    (ধ্রুবক শূন্য *) bytesZ,
			    get_দৈর্ঘ্য (),
			    header.unit_আকার,
			    _hb_cmp_method<T, Type>)
	   ? (ধ্রুবক Type *) (((ধ্রুবক অক্ষর *) &bytesZ) + (pos * header.unit_আকার))
	   : nullptr;
  }

  ব্যক্তিগত:
  যৌক্তিক sanitize_shallow (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (header.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_আকার <= header.unit_আকার &&
		  c->check_range (bytesZ.arrayZ,
				  header.nUnits,
				  header.unit_আকার));
  }

  সুরক্ষিত:
  Var_Sized_Bin_অনুসন্ধান_Header	header;
  UnsizedArrayOf<HBUINT8>	bytesZ;
  সর্বজনীন:
  DEFINE_আকার_ARRAY (10, bytesZ);
};


/* CFF INDEX */

template <typename গুনতি_3>
কাঠামো CFFIndex
{
  unsigned পূর্ণসংখ্যা offset_array_আকার () ধ্রুবক
  { ফেরত off_আকার * (গুনতি + 1); }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক Iterable &iterable,
		  ধ্রুবক unsigned *p_তথ্য_আকার = nullptr,
                  unsigned min_off_আকার = 0)
  {
    TRACE_SERIALIZE (এই);
    unsigned তথ্য_আকার;
    যদি (p_তথ্য_আকার)
      তথ্য_আকার = *p_তথ্য_আকার;
    অন্যথা
      যোগ_আকার (iterable, &তথ্য_আকার);

    auto it = hb_iter (iterable);
    যদি (unlikely (!serialize_header (c, +it, তথ্য_আকার, min_off_আকার))) return_trace (মিথ্যা);
    unsigned অক্ষর *ret = c->allocate_আকার<unsigned অক্ষর> (তথ্য_আকার, মিথ্যা);
    যদি (unlikely (!ret)) return_trace (মিথ্যা);
    unsigned remaining = তথ্য_আকার;
    জন্য (ধ্রুবক auto &_ : +it)
    {
      unsigned len = _.দৈর্ঘ্য;

      যদি (!len)
	চালু;

      যদি (unlikely (len > remaining)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (মিথ্যা, HB_SERIALIZE_ERROR_INT_OVERFLOW));
      }

      remaining -= len;

      যদি (len <= 1)
      {
	*ret++ = *_.arrayZ;
	চালু;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (সত্য);
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  যৌক্তিক serialize_header (hb_serialize_context_t *c,
			 Iterator it,
			 unsigned তথ্য_আকার,
                         unsigned min_off_আকার = 0)
  {
    TRACE_SERIALIZE (এই);

    unsigned off_আকার_2 = (hb_bit_storage (তথ্য_আকার + 1) + 7) / 8;
    off_আকার_2 = hb_max(min_off_আকার, off_আকার_2);

    /* serialize CFFIndex header */
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    এই->গুনতি = hb_len (it);
    যদি (!এই->গুনতি) return_trace (সত্য);
    যদি (unlikely (!c->extend (এই->off_আকার))) return_trace (মিথ্যা);
    এই->off_আকার = off_আকার_2;
    যদি (unlikely (!c->allocate_আকার<HBUINT8> (off_আকার_2 * (এই->গুনতি + 1), মিথ্যা)))
      return_trace (মিথ্যা);

    /* serialize indices */
    unsigned পূর্ণসংখ্যা offset = 1;
    যদি (HB_OPTIMIZE_আকার_VAL)
    {
      unsigned পূর্ণসংখ্যা i = 0;
      জন্য (ধ্রুবক auto &_ : +it)
      {
	set_offset_at (i++, offset);
	offset += hb_len_of (_);
      }
      set_offset_at (i, offset);
    }
    অন্যথা
      নির্বাচন (off_আকার_2)
      {
	ক্ষেত্র 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  জন্য (ধ্রুবক auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	বিরতি;
	ক্ষেত্র 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  জন্য (ধ্রুবক auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	বিরতি;
	ক্ষেত্র 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  জন্য (ধ্রুবক auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	বিরতি;
	ক্ষেত্র 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  জন্য (ধ্রুবক auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	বিরতি;
	পূর্বনির্ধারিত:
	বিরতি;
      }

    প্রতিজ্ঞা (offset == তথ্য_আকার + 1);
    return_trace (সত্য);
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  স্থির unsigned যোগ_আকার (ধ্রুবক Iterable &iterable, unsigned *তথ্য_আকার = nullptr, unsigned min_off_আকার = 0)
  {
    auto it = + hb_iter (iterable);
    যদি (!it)
    {
      যদি (তথ্য_আকার) *তথ্য_আকার = 0;
      ফেরত min_আকার;
    }

    unsigned যোগ = 0;
    জন্য (ধ্রুবক auto &_ : +it)
      যোগ += hb_len_of (_);

    যদি (তথ্য_আকার) *তথ্য_আকার = যোগ;

    unsigned off_আকার_2 = (hb_bit_storage (যোগ + 1) + 7) / 8;
    off_আকার_2 = hb_max(min_off_আকার, off_আকার_2);

    ফেরত min_আকার + HBUINT8::static_আকার + (hb_len (it) + 1) * off_আকার_2 + যোগ;
  }

  শূন্য set_offset_at (unsigned পূর্ণসংখ্যা সূচক_2, unsigned পূর্ণসংখ্যা offset)
  {
    প্রতিজ্ঞা (সূচক_2 <= গুনতি);

    unsigned পূর্ণসংখ্যা আকার_2 = off_আকার;
    ধ্রুবক HBUINT8 *p = offsets;
    নির্বাচন (আকার_2)
    {
      ক্ষেত্র 1: ((HBUINT8  *) p)[সূচক_2] = offset; বিরতি;
      ক্ষেত্র 2: ((HBUINT16 *) p)[সূচক_2] = offset; বিরতি;
      ক্ষেত্র 3: ((HBUINT24 *) p)[সূচক_2] = offset; বিরতি;
      ক্ষেত্র 4: ((HBUINT32 *) p)[সূচক_2] = offset; বিরতি;
      পূর্বনির্ধারিত: ফেরত;
    }
  }

  ব্যক্তিগত:
  unsigned পূর্ণসংখ্যা offset_at (unsigned পূর্ণসংখ্যা সূচক_2) ধ্রুবক
  {
    প্রতিজ্ঞা (সূচক_2 <= গুনতি);

    unsigned পূর্ণসংখ্যা আকার_2 = off_আকার;
    ধ্রুবক HBUINT8 *p = offsets;
    নির্বাচন (আকার_2)
    {
      ক্ষেত্র 1: ফেরত ((HBUINT8  *) p)[সূচক_2];
      ক্ষেত্র 2: ফেরত ((HBUINT16 *) p)[সূচক_2];
      ক্ষেত্র 3: ফেরত ((HBUINT24 *) p)[সূচক_2];
      ক্ষেত্র 4: ফেরত ((HBUINT32 *) p)[সূচক_2];
      পূর্বনির্ধারিত: ফেরত 0;
    }
  }

  ধ্রুবক unsigned অক্ষর *তথ্য_base () ধ্রুবক
  { ফেরত (ধ্রুবক unsigned অক্ষর *) এই + min_আকার + off_আকার.static_আকার - 1 + offset_array_আকার (); }
  সর্বজনীন:

  hb_ubytes_t operator [] (unsigned পূর্ণসংখ্যা সূচক_2) ধ্রুবক
  {
    যদি (unlikely (সূচক_2 >= গুনতি)) ফেরত hb_ubytes_t ();
    hb_barrier ();
    unsigned offset0 = offset_at (সূচক_2);
    unsigned offset1 = offset_at (সূচক_2 + 1);
    যদি (unlikely (offset1 < offset0 || offset1 > offset_at (গুনতি)))
      ফেরত hb_ubytes_t ();
    ফেরত hb_ubytes_t (তথ্য_base () + offset0, offset1 - offset0);
  }

  size_t get_আকার () ধ্রুবক
  {
    যদি (গুনতি)
      ফেরত hb_unsigned_add_saturate (min_আকার, off_আকার.static_আকার,
				       offset_array_আকার (), offset_at (গুনতি) - 1);
    ফেরত min_আকার;  /* empty CFFIndex contains count only */
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  hb_barrier () &&
			  (গুনতি == 0 || /* empty INDEX */
			   (গুনতি < গুনতি + 1u &&
			    c->check_struct (&off_আকার) && off_আকার >= 1 && off_আকার <= 4 &&
			    c->check_array (offsets, off_আকার, গুনতি + 1u) &&
			    c->check_range (তথ্য_base (), offset_at (গুনতি))))));
  }

  সর্বজনীন:
  গুনতি_3		গুনতি;		/* Number of object data. Note there are (count+1) offsets */
  ব্যক্তিগত:
  HBUINT8	off_আকার;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  সর্বজনীন:
  DEFINE_আকার_MIN (গুনতি_3::static_আকার);
};
প্রকারলেখ CFFIndex<HBUINT16> CFF1_সূচক;
প্রকারলেখ CFFIndex<HBUINT32> CFF2_সূচক;


/* TupleValues */
কাঠামো Tuple_মানসমূহ
{
  গণনা packed_মান_flag_t
  {
    মানসমূহ_ARE_ZEROS     = 0x80,
    মানসমূহ_ARE_BYTES     = 0x00,
    মানসমূহ_ARE_শব্দগুলি     = 0x40,
    মানসমূহ_ARE_LONGS     = 0xC0,
    মানসমূহ_আকার_MASK     = 0xC0,
    মান_RUN_গুনতি_MASK = 0x3F
  };

  স্থির unsigned compile_unsafe (hb_array_t<ধ্রুবক পূর্ণসংখ্যা> values, /* IN */
				  unsigned অক্ষর *encoded_bytes /* OUT */)
  {
    unsigned অঙ্ক_মানসমূহ_3 = values.দৈর্ঘ্য;
    unsigned encoded_len = 0;
    unsigned i = 0;
    যখন (i < অঙ্ক_মানসমূহ_3)
    {
      পূর্ণসংখ্যা val = values.arrayZ[i];
      যদি (val == 0)
        encoded_len += encode_মান_run_as_zeroes (i, encoded_bytes + encoded_len, values);
      অন্যথা যদি ((int8_t) val == val)
        encoded_len += encode_মান_run_as_bytes (i, encoded_bytes + encoded_len, values);
      অন্যথা যদি ((int16_t) val == val)
        encoded_len += encode_মান_run_as_শব্দগুলি (i, encoded_bytes + encoded_len, values);
      অন্যথা
        encoded_len += encode_মান_run_as_longs (i, encoded_bytes + encoded_len, values);
    }
    ফেরত encoded_len;
  }

  স্থির unsigned encode_মান_run_as_zeroes (unsigned& i,
					      unsigned অক্ষর *it,
					      hb_array_t<ধ্রুবক পূর্ণসংখ্যা> values)
  {
    unsigned অঙ্ক_মানসমূহ_3 = values.দৈর্ঘ্য;
    unsigned run_দৈর্ঘ্য = 0;
    unsigned encoded_len = 0;
    যখন (i < অঙ্ক_মানসমূহ_3 && values.arrayZ[i] == 0)
    {
      i++;
      run_দৈর্ঘ্য++;
    }

    যখন (run_দৈর্ঘ্য >= 64)
    {
      *it++ = অক্ষর (মানসমূহ_ARE_ZEROS | 63);
      run_দৈর্ঘ্য -= 64;
      encoded_len++;
    }

    যদি (run_দৈর্ঘ্য)
    {
      *it++ = অক্ষর (মানসমূহ_ARE_ZEROS | (run_দৈর্ঘ্য - 1));
      encoded_len++;
    }
    ফেরত encoded_len;
  }

  স্থির unsigned encode_মান_run_as_bytes (unsigned &i,
					     unsigned অক্ষর *it,
					     hb_array_t<ধ্রুবক পূর্ণসংখ্যা> values)
  {
    unsigned start = i;
    unsigned অঙ্ক_মানসমূহ_3 = values.দৈর্ঘ্য;
    যখন (i < অঙ্ক_মানসমূহ_3)
    {
      পূর্ণসংখ্যা val = values.arrayZ[i];
      যদি ((int8_t) val != val)
        বিরতি;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      যদি (val == 0 && i + 1 < অঙ্ক_মানসমূহ_3 && values.arrayZ[i+1] == 0)
        বিরতি;

      i++;
    }
    unsigned run_দৈর্ঘ্য = i - start;

    unsigned encoded_len = 0;

    যখন (run_দৈর্ঘ্য >= 64)
    {
      *it++ = (মানসমূহ_ARE_BYTES | 63);
      encoded_len++;

      জন্য (unsigned j = 0; j < 64; j++)
	it[j] = static_cast<অক্ষর> (values.arrayZ[start + j]);
      it += 64;
      encoded_len += 64;

      start += 64;
      run_দৈর্ঘ্য -= 64;
    }

    যদি (run_দৈর্ঘ্য)
    {
      *it++ = (মানসমূহ_ARE_BYTES | (run_দৈর্ঘ্য - 1));
      encoded_len++;

      জন্য (unsigned j = 0; j < run_দৈর্ঘ্য; j++)
        it[j] = static_cast<অক্ষর> (values.arrayZ[start + j]);
      encoded_len += run_দৈর্ঘ্য;
    }

    ফেরত encoded_len;
  }

  স্থির unsigned encode_মান_run_as_শব্দগুলি (unsigned &i,
					     unsigned অক্ষর *it,
					     hb_array_t<ধ্রুবক পূর্ণসংখ্যা> values)
  {
    unsigned start = i;
    unsigned অঙ্ক_মানসমূহ_3 = values.দৈর্ঘ্য;
    যখন (i < অঙ্ক_মানসমূহ_3)
    {
      পূর্ণসংখ্যা val = values.arrayZ[i];

      যদি ((int16_t) val != val)
        বিরতি;

      /* start a new run for a single zero value. */
      যদি (val == 0) বিরতি;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      যদি ((int8_t) val == val &&
          i + 1 < অঙ্ক_মানসমূহ_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        বিরতি;

      i++;
    }

    unsigned run_দৈর্ঘ্য = i - start;
    unsigned encoded_len = 0;
    যখন (run_দৈর্ঘ্য >= 64)
    {
      *it++ = (মানসমূহ_ARE_শব্দগুলি | 63);
      encoded_len++;

      জন্য (unsigned j = 0; j < 64; j++)
      {
        int16_t মান_val = values.arrayZ[start + j];
        *it++ = static_cast<অক্ষর> (মান_val >> 8);
        *it++ = static_cast<অক্ষর> (মান_val & 0xFF);

        encoded_len += 2;
      }

      start += 64;
      run_দৈর্ঘ্য -= 64;
    }

    যদি (run_দৈর্ঘ্য)
    {
      *it++ = (মানসমূহ_ARE_শব্দগুলি | (run_দৈর্ঘ্য - 1));
      encoded_len++;
      যখন (start < i)
      {
        int16_t মান_val = values.arrayZ[start++];
        *it++ = static_cast<অক্ষর> (মান_val >> 8);
        *it++ = static_cast<অক্ষর> (মান_val & 0xFF);

        encoded_len += 2;
      }
    }
    ফেরত encoded_len;
  }

  স্থির unsigned encode_মান_run_as_longs (unsigned &i,
					     unsigned অক্ষর *it,
					     hb_array_t<ধ্রুবক পূর্ণসংখ্যা> values)
  {
    unsigned start = i;
    unsigned অঙ্ক_মানসমূহ_3 = values.দৈর্ঘ্য;
    যখন (i < অঙ্ক_মানসমূহ_3)
    {
      পূর্ণসংখ্যা val = values.arrayZ[i];

      যদি ((int16_t) val == val)
        বিরতি;

      i++;
    }

    unsigned run_দৈর্ঘ্য = i - start;
    unsigned encoded_len = 0;
    যখন (run_দৈর্ঘ্য >= 64)
    {
      *it++ = (মানসমূহ_ARE_LONGS | 63);
      encoded_len++;

      জন্য (unsigned j = 0; j < 64; j++)
      {
        int32_t মান_val = values.arrayZ[start + j];
        *it++ = static_cast<অক্ষর> (মান_val >> 24);
        *it++ = static_cast<অক্ষর> (মান_val >> 16);
        *it++ = static_cast<অক্ষর> (মান_val >> 8);
        *it++ = static_cast<অক্ষর> (মান_val & 0xFF);

        encoded_len += 4;
      }

      start += 64;
      run_দৈর্ঘ্য -= 64;
    }

    যদি (run_দৈর্ঘ্য)
    {
      *it++ = (মানসমূহ_ARE_LONGS | (run_দৈর্ঘ্য - 1));
      encoded_len++;
      যখন (start < i)
      {
        int32_t মান_val = values.arrayZ[start++];
        *it++ = static_cast<অক্ষর> (মান_val >> 24);
        *it++ = static_cast<অক্ষর> (মান_val >> 16);
        *it++ = static_cast<অক্ষর> (মান_val >> 8);
        *it++ = static_cast<অক্ষর> (মান_val & 0xFF);

        encoded_len += 4;
      }
    }
    ফেরত encoded_len;
  }

  template <typename T>
#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  স্থির যৌক্তিক decompile (ধ্রুবক HBUINT8 *&p /* IN/OUT */,
			 hb_vector_t<T> &values /* IN/OUT */,
			 ধ্রুবক HBUINT8 *end,
			 যৌক্তিক consume_all = মিথ্যা,
			 unsigned start = 0)
  {
    unsigned i = 0;
    unsigned গুনতি = consume_all ? UINT_MAX : values.দৈর্ঘ্য;
    যদি (consume_all)
      values.alloc ((end - p) / 2);
    যখন (i < গুনতি)
    {
      যদি (unlikely (p + 1 > end)) ফেরত consume_all;
      unsigned control = *p++;
      unsigned run_গুনতি = (control & মান_RUN_গুনতি_MASK) + 1;
      যদি (consume_all)
      {
        যদি (unlikely (!values.resize_dirty  (values.দৈর্ঘ্য + run_গুনতি)))
	  ফেরত মিথ্যা;
      }
      unsigned stop = i + run_গুনতি;
      যদি (unlikely (stop > গুনতি)) ফেরত মিথ্যা;

      unsigned skip = i < start ? hb_min (start - i, run_গুনতি) : 0;
      i += skip;

      যদি ((control & মানসমূহ_আকার_MASK) == মানসমূহ_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * আকার (T));
	i = stop;
      }
      অন্যথা যদি ((control & মানসমূহ_আকার_MASK) ==  মানসমূহ_ARE_শব্দগুলি)
      {
        যদি (unlikely (p + run_গুনতি * HBINT16::static_আকার > end)) ফেরত মিথ্যা;
	p += skip * HBINT16::static_আকার;
#ifndef HB_OPTIMIZE_আকার
        জন্য (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ধ্রুবক HBINT16 *) p;
	  p += HBINT16::static_আকার;
	  values.arrayZ[i + 1] = * (ধ্রুবক HBINT16 *) p;
	  p += HBINT16::static_আকার;
	  values.arrayZ[i + 2] = * (ধ্রুবক HBINT16 *) p;
	  p += HBINT16::static_আকার;
	  values.arrayZ[i + 3] = * (ধ্রুবক HBINT16 *) p;
	  p += HBINT16::static_আকার;
	}
#endif
        জন্য (; i < stop; i++)
        {
          values.arrayZ[i] = * (ধ্রুবক HBINT16 *) p;
          p += HBINT16::static_আকার;
        }
      }
      অন্যথা যদি ((control & মানসমূহ_আকার_MASK) ==  মানসমূহ_ARE_LONGS)
      {
        যদি (unlikely (p + run_গুনতি * HBINT32::static_আকার > end)) ফেরত মিথ্যা;
	p += skip * HBINT32::static_আকার;
        জন্য (; i < stop; i++)
        {
          values.arrayZ[i] = * (ধ্রুবক HBINT32 *) p;
          p += HBINT32::static_আকার;
        }
      }
      অন্যথা যদি ((control & মানসমূহ_আকার_MASK) ==  মানসমূহ_ARE_BYTES)
      {
        যদি (unlikely (p + run_গুনতি > end)) ফেরত মিথ্যা;
	p += skip * HBINT8::static_আকার;
#ifndef HB_OPTIMIZE_আকার
	জন্য (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ধ্রুবক HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (ধ্রুবক HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (ধ্রুবক HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (ধ্রুবক HBINT8 *) p++;
	}
#endif
        জন্য (; i < stop; i++)
          values.arrayZ[i] = * (ধ্রুবক HBINT8 *) p++;
      }
    }
    ফেরত সত্য;
  }

  স্থির যৌক্তিক skip (ধ্রুবক unsigned অক্ষর *&p /* IN/OUT */,
		    ধ্রুবক unsigned অক্ষর *end,
		    unsigned গুনতি)
  {
    unsigned seen = 0;
    যখন (seen < গুনতি)
    {
      যদি (unlikely (p >= end)) ফেরত মিথ্যা;
      unsigned control = *p++;
      unsigned run_গুনতি = (control & মান_RUN_গুনতি_MASK) + 1;
      যদি (unlikely (run_গুনতি > গুনতি - seen)) ফেরত মিথ্যা;

      unsigned width = 0;
      নির্বাচন (control & মানসমূহ_আকার_MASK)
      {
	ক্ষেত্র মানসমূহ_ARE_ZEROS: বিরতি;
	ক্ষেত্র মানসমূহ_ARE_BYTES: width = HBINT8::static_আকার; বিরতি;
	ক্ষেত্র মানসমূহ_ARE_শব্দগুলি: width = HBINT16::static_আকার; বিরতি;
	ক্ষেত্র মানসমূহ_ARE_LONGS: width = HBINT32::static_আকার; বিরতি;
      }
      যদি (unlikely (unsigned (end - p) < run_গুনতি * width)) ফেরত মিথ্যা;
      p += run_গুনতি * width;
      seen += run_গুনতি;
    }
    ফেরত সত্য;
  }

  কাঠামো iter_t : hb_iter_with_fallback_t<iter_t, পূর্ণসংখ্যা>
  {
    iter_t (ধ্রুবক unsigned অক্ষর *p_, unsigned len_)
	    : p (p_), endp (p_ + len_)
    { যদি (likely (ensure_run ())) read_মান (); }

    ব্যক্তিগত:
    ধ্রুবক unsigned অক্ষর *p;
    ধ্রুবক unsigned অক্ষর * ধ্রুবক endp;
    পূর্ণসংখ্যা current_মান = 0;
    signed run_গুনতি = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    যৌক্তিক ensure_run ()
    {
      যদি (likely (run_গুনতি > 0)) ফেরত সত্য;
      ফেরত _ensure_run ();
    }
    যৌক্তিক _ensure_run ()
    {
      যদি (unlikely (p >= endp))
      {
        run_গুনতি = 0;
        current_মান = 0;
	ফেরত মিথ্যা;
      }

      unsigned control = *p++;
      run_গুনতি = (control & মান_RUN_গুনতি_MASK) + 1;
      width = control & মানসমূহ_আকার_MASK;
      নির্বাচন (width)
      {
        ক্ষেত্র মানসমূহ_ARE_ZEROS: width = 0; বিরতি;
	ক্ষেত্র মানসমূহ_ARE_BYTES: width = HBINT8::static_আকার;  বিরতি;
	ক্ষেত্র মানসমূহ_ARE_শব্দগুলি: width = HBINT16::static_আকার; বিরতি;
	ক্ষেত্র মানসমূহ_ARE_LONGS: width = HBINT32::static_আকার; বিরতি;
	পূর্বনির্ধারিত: প্রতিজ্ঞা (মিথ্যা);
      }

      যদি (unlikely (p + run_গুনতি * width > endp))
      {
	run_গুনতি = 0;
	current_মান = 0;
	ফেরত মিথ্যা;
      }

      ফেরত সত্য;
    }
    শূন্য read_মান ()
    {
      নির্বাচন (width)
      {
        ক্ষেত্র 0: current_মান = 0; বিরতি;
	ক্ষেত্র 1: current_মান = * (ধ্রুবক HBINT8  *) p; বিরতি;
	ক্ষেত্র 2: current_মান = * (ধ্রুবক HBINT16 *) p; বিরতি;
	ক্ষেত্র 4: current_মান = * (ধ্রুবক HBINT32 *) p; বিরতি;
      }
      p += width;
    }

    সর্বজনীন:

    প্রকারলেখ পূর্ণসংখ্যা __item_t__;
    __item_t__ __item__ () ধ্রুবক
    { ফেরত current_মান; }

    যৌক্তিক __more__ () ধ্রুবক { ফেরত run_গুনতি || p < endp; }
    শূন্য __next__ ()
    {
      run_গুনতি--;
      যদি (unlikely (!ensure_run ()))
	ফেরত;
      read_মান ();
    }
    শূন্য __forward__ (unsigned n)
    {
      যদি (unlikely (!ensure_run ()))
	ফেরত;
      যখন (n)
      {
	unsigned i = hb_min (n, (unsigned) run_গুনতি);
	run_গুনতি -= i;
	n -= i;
	p += (i - 1) * width;
	যদি (unlikely (!ensure_run ()))
	  ফেরত;
	read_মান ();
      }
    }
    যৌক্তিক operator != (ধ্রুবক iter_t& o) ধ্রুবক
    { ফেরত p != o.p || run_গুনতি != o.run_গুনতি; }
    iter_t __end__ () ধ্রুবক
    {
      iter_t it (endp, 0);
      ফেরত it;
    }
  };

  কাঠামো fetcher_t
  {
    fetcher_t (ধ্রুবক unsigned অক্ষর *p_, unsigned len_)
	      : p (p_), end (p_ + len_) {}

    ব্যক্তিগত:
    ধ্রুবক unsigned অক্ষর *p;
    ধ্রুবক unsigned অক্ষর * ধ্রুবক end;
    signed run_গুনতি = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    যৌক্তিক ensure_run ()
    {
      যদি (likely (run_গুনতি > 0)) ফেরত সত্য;
      ফেরত _ensure_run ();
    }

    যৌক্তিক _ensure_run ()
    {
      যদি (unlikely (p >= end))
      {
        run_গুনতি = 0;
	ফেরত মিথ্যা;
      }

      unsigned control = *p++;
      run_গুনতি = (control & মান_RUN_গুনতি_MASK) + 1;
      width = control & মানসমূহ_আকার_MASK;
      নির্বাচন (width)
      {
        ক্ষেত্র মানসমূহ_ARE_ZEROS: width = 0; বিরতি;
	ক্ষেত্র মানসমূহ_ARE_BYTES: width = HBINT8::static_আকার;  বিরতি;
	ক্ষেত্র মানসমূহ_ARE_শব্দগুলি: width = HBINT16::static_আকার; বিরতি;
	ক্ষেত্র মানসমূহ_ARE_LONGS: width = HBINT32::static_আকার; বিরতি;
	পূর্বনির্ধারিত: প্রতিজ্ঞা (মিথ্যা);
      }

      যদি (unlikely (p + run_গুনতি * width > end))
      {
	run_গুনতি = 0;
	ফেরত মিথ্যা;
      }

      ফেরত সত্য;
    }

    সর্বজনীন:
    শূন্য skip (unsigned n)
    {
      যখন (n)
      {
	যদি (unlikely (!ensure_run ()))
	  ফেরত;
	unsigned i = hb_min (n, (unsigned) run_গুনতি);
	run_গুনতি -= i;
	n -= i;
	p += i * width;
      }
    }

    ব্যক্তিগত:
    template <যৌক্তিক scaled, typename T>
    শূন্য _add_to (hb_array_t<T> out, T scale = T (1))
    {
      unsigned n = out.দৈর্ঘ্য;
      T *arrayZ = out.arrayZ;

      জন্য (unsigned i = 0; i < n;)
      {
	যদি (unlikely (!ensure_run ()))
	  বিরতি;
	unsigned গুনতি = hb_min (n - i, (unsigned) run_গুনতি);
	নির্বাচন (width)
	{
	  ক্ষেত্র 0:
	  {
	    arrayZ += গুনতি;
	    বিরতি;
	  }
	  ক্ষেত্র 1:
	  {
	    ধ্রুবক auto *pp = (ধ্রুবক HBINT8 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_আকার
	    জন্য (; j + 3 < গুনতি; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    জন্য (; j < গুনতি; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ধ্রুবক unsigned অক্ষর *) pp;
	  }
	  বিরতি;
	  ক্ষেত্র 2:
	  {
	    ধ্রুবক auto *pp = (ধ্রুবক HBINT16 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_আকার
	    জন্য (; j + 3 < গুনতি; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    জন্য (; j < গুনতি; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ধ্রুবক unsigned অক্ষর *) pp;
	  }
	  বিরতি;
	  ক্ষেত্র 4:
	  {
	    ধ্রুবক auto *pp = (ধ্রুবক HBINT32 *) p;
	    জন্য (unsigned j = 0; j < গুনতি; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ধ্রুবক unsigned অক্ষর *) pp;
	  }
	  বিরতি;
	}
	run_গুনতি -= গুনতি;
	i += গুনতি;
      }
    }

    সর্বজনীন:
    template <typename T>
    শূন্য add_to (hb_array_t<T> out, দশমিক scale = 1.0f)
    {
#ifndef HB_OPTIMIZE_আকার
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      যদি (মিথ্যা && scale == 1.0f)
        _add_to<মিথ্যা> (out);
      অন্যথা
#endif
        _add_to<সত্য> (out, (T) scale);
    }
  };
};

কাঠামো TupleList : CFF2_সূচক
{
  Tuple_মানসমূহ::iter_t operator [] (unsigned i) ধ্রুবক
  {
    auto bytes = CFF2_সূচক::operator [] (i);
    ফেরত Tuple_মানসমূহ::iter_t (bytes.arrayZ, bytes.দৈর্ঘ্য);
  }

  Tuple_মানসমূহ::fetcher_t fetcher (unsigned i) ধ্রুবক
  {
    auto bytes = CFF2_সূচক::operator [] (i);
    ফেরত Tuple_মানসমূহ::fetcher_t (bytes.arrayZ, bytes.দৈর্ঘ্য);
  }
};


// Alignment

template <unsigned পূর্ণসংখ্যা alignment>
কাঠামো Align
{
  size_t get_আকার (ধ্রুবক শূন্য *base) ধ্রুবক
  {
    unsigned offset = (ধ্রুবক অক্ষর *) এই - (ধ্রুবক অক্ষর *) base;
    ফেরত (alignment - offset) & (alignment - 1);
  }

  সর্বজনীন:
  DEFINE_আকার_MIN (0);
};



} /* namespace OT */


#endif /* HB_OPEN_TYPE_HH */
