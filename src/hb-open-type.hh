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
template <তার্কিক BE,
	  typename Type,
	  unsigned পূর্ণাঙ্ক Size = পরিমাণ (Type)>
সংরচনা অঙ্ক_Type
{
  প্রকারনির্দেশ Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  প্রকারনির্দেশ typename std::conditional<std::is_integral<Type>::value && পরিমাণ (Type) <= পরিমাণ(পূর্ণাঙ্ক),
				     typename std::conditional<std::is_signed<Type>::value, signed, unsigned>::type,
				     Type>::type WideType;

  অঙ্ক_Type () = স্বভাব;
  explicit constexpr অঙ্ক_Type (Type V) : v {V} {}
  অঙ্ক_Type& operator = (Type V) { v = V; প্রত্যাগম *এতত্; }

  operator WideType () ধ্রুব { প্রত্যাগম v; }

  তার্কিক operator == (ধ্রুব অঙ্ক_Type &o) ধ্রুব { প্রত্যাগম (Type) v == (Type) o.v; }
  তার্কিক operator != (ধ্রুব অঙ্ক_Type &o) ধ্রুব { প্রত্যাগম !(*এতত্ == o); }

  অঙ্ক_Type& operator += (WideType গুনতি) { *এতত্ = *এতত্ + গুনতি; প্রত্যাগম *এতত্; }
  অঙ্ক_Type& operator -= (WideType গুনতি) { *এতত্ = *এতত্ - গুনতি; প্রত্যাগম *এতত্; }
  অঙ্ক_Type& operator ++ () { *এতত্ += 1; প্রত্যাগম *এতত্; }
  অঙ্ক_Type& operator -- () { *এতত্ -= 1; প্রত্যাগম *এতত্; }
  অঙ্ক_Type operator ++ (পূর্ণাঙ্ক) { অঙ্ক_Type c (*এতত্); ++*এতত্; প্রত্যাগম c; }
  অঙ্ক_Type operator -- (পূর্ণাঙ্ক) { অঙ্ক_Type c (*এতত্); --*এতত্; প্রত্যাগম c; }

  uint32_t hash () ধ্রুব { প্রত্যাগম hb_array ((ধ্রুব বর্ণ *) &v, পরিমাণ (v)).hash (); }
  HB_INTERNAL স্থির পূর্ণাঙ্ক cmp (ধ্রুব অঙ্ক_Type *a, ধ্রুব অঙ্ক_Type *b)
  { প্রত্যাগম b->cmp (*a); }
  HB_INTERNAL স্থির পূর্ণাঙ্ক cmp (ধ্রুব শূন্য *a, ধ্রুব শূন্য *b)
  {
    অঙ্ক_Type *pa = (অঙ্ক_Type *) a;
    অঙ্ক_Type *pb = (অঙ্ক_Type *) b;

    প্রত্যাগম pb->cmp (*pa);
  }
  template <typename Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  পূর্ণাঙ্ক cmp (Type2 a) ধ্রুব
  {
    Type b = v;
    প্রত্যাগম (a > b) - (a < b);
  }
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }
  রক্ষিত:
  typename std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, Size>,
			    HBFloat<BE, Type, Size>>::type v;
  সার্বজনিক:
  DEFINE_SIZE_STATIC (Size);
};

প্রকারনির্দেশ অঙ্ক_Type<সত্য, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
প্রকারনির্দেশ অঙ্ক_Type<সত্য, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
প্রকারনির্দেশ অঙ্ক_Type<সত্য, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
প্রকারনির্দেশ অঙ্ক_Type<সত্য, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
প্রকারনির্দেশ অঙ্ক_Type<সত্য, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
প্রকারনির্দেশ অঙ্ক_Type<সত্য, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
প্রকারনির্দেশ অঙ্ক_Type<সত্য, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
প্রকারনির্দেশ অঙ্ক_Type<সত্য, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
প্রকারনির্দেশ অঙ্ক_Type<সত্য, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

প্রকারনির্দেশ অঙ্ক_Type<অসত্য, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
প্রকারনির্দেশ অঙ্ক_Type<অসত্য, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
প্রকারনির্দেশ অঙ্ক_Type<অসত্য, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
প্রকারনির্দেশ অঙ্ক_Type<অসত্য, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
প্রকারনির্দেশ অঙ্ক_Type<অসত্য, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
প্রকারনির্দেশ অঙ্ক_Type<অসত্য, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

প্রকারনির্দেশ অঙ্ক_Type<সত্য,  ভিন্ন>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
প্রকারনির্দেশ অঙ্ক_Type<সত্য,  দ্বিগুণ> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
প্রকারনির্দেশ অঙ্ক_Type<অসত্য, ভিন্ন>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
প্রকারনির্দেশ অঙ্ক_Type<অসত্য, দ্বিগুণ> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
সংরচনা HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& operator = (uint16_t i ) { HBUINT16::operator= (i); প্রত্যাগম *এতত্; }
  সার্বজনিক:
  DEFINE_SIZE_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
সংরচনা HBUINT32VAR
{
  size_t get_size () ধ্রুব
  {
    unsigned b0 = v[0];
    যদি (b0 < 0x80)
      প্রত্যাগম 1;
    অন্যথা যদি (b0 < 0xC0)
      প্রত্যাগম 2;
    অন্যথা যদি (b0 < 0xE0)
      প্রত্যাগম 3;
    অন্যথা যদি (b0 < 0xF0)
      প্রত্যাগম 4;
    অন্যথা
      প্রত্যাগম 5;
  }

  স্থির size_t get_size (uint32_t v)
  {
    যদি (v < 0x80)
      প্রত্যাগম 1;
    অন্যথা যদি (v < 0x4000)
      প্রত্যাগম 2;
    অন্যথা যদি (v < 0x200000)
      প্রত্যাগম 3;
    অন্যথা যদি (v < 0x10000000)
      প্রত্যাগম 4;
    অন্যথা
      প্রত্যাগম 5;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_size ()));
  }

  operator uint32_t () ধ্রুব
  {
    unsigned b0 = v[0];
    যদি (b0 < 0x80)
      প্রত্যাগম b0;
    অন্যথা যদি (b0 < 0xC0)
      প্রত্যাগম ((b0 & 0x3F) << 8) | v[1];
    অন্যথা যদি (b0 < 0xE0)
      প্রত্যাগম ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    অন্যথা যদি (b0 < 0xF0)
      প্রত্যাগম ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    অন্যথা
      প্রত্যাগম (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  স্থির unsigned serialize_unsafe (unsigned বর্ণ *buf, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned বর্ণ *p = buf + len;
    প্রতি (unsigned i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    যদি (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    প্রত্যাগম len;
  }

  স্থির তার্কিক serialize (hb_serialize_context_t *c, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned বর্ণ *buf = c->allocate_size<unsigned বর্ণ> (len, অসত্য);
    যদি (unlikely (!buf))
      প্রত্যাগম অসত্য;

    serialize_unsafe (buf, v);
    প্রত্যাগম সত্য;
  }

  রক্ষিত:
  unsigned বর্ণ v[5];

  সার্বজনিক:
  DEFINE_SIZE_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
প্রকারনির্দেশ HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
প্রকারনির্দেশ HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
প্রকারনির্দেশ HBUINT16 UFWORD;

template <typename Type, unsigned fraction_bits>
সংরচনা HBFixed : Type
{
  স্থির constexpr ভিন্ন mult = 1.f / (1 << fraction_bits);
  static_assert (Type::static_size * 8 > fraction_bits, "");

  operator signed () ধ্রুব = delete;
  operator unsigned () ধ্রুব = delete;
  explicit operator ভিন্ন () ধ্রুব { প্রত্যাগম to_float (); }
  typename Type::type to_int () ধ্রুব { প্রত্যাগম Type::v; }
  শূন্য set_int (typename Type::type i ) { Type::v = i; }
  ভিন্ন to_float (ভিন্ন offset = 0) ধ্রুব  { প্রত্যাগম ((int32_t) Type::v + offset) * mult; }
  শূন্য set_float (ভিন্ন f) { Type::v = roundf (f / mult); }
  সার্বজনিক:
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
সংরচনা LONGDATETIME
{
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }
  রক্ষিত:
  HBINT32 major;
  HBUINT32 minor;
  সার্বজনিক:
  DEFINE_SIZE_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
সংরচনা Tag : HBUINT32
{
  Tag& operator = (hb_tag_t i) { HBUINT32::operator= (i); প্রত্যাগম *এতত্; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  operator ধ্রুব বর্ণ* () ধ্রুব { প্রত্যাগম reinterpret_cast<ধ্রুব বর্ণ *> (এতত্); }
  operator বর্ণ* ()             { প্রত্যাগম reinterpret_cast<বর্ণ *> (এতত্); }
  সার্বজনিক:
  DEFINE_SIZE_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
সংরচনা HBGlyphID16 : HBUINT16
{
  HBGlyphID16& operator = (uint16_t i) { HBUINT16::operator= (i); প্রত্যাগম *এতত্; }
};
সংরচনা HBGlyphID24 : HBUINT24
{
  HBGlyphID24& operator = (uint32_t i) { HBUINT24::operator= (i); প্রত্যাগম *এতত্; }
};

/* Script/language-system/feature index */
সংরচনা Index : HBUINT16 {
  স্থির constexpr unsigned NOT_FOUND_INDEX = 0xFFFFu;
  Index& operator = (uint16_t i) { HBUINT16::operator= (i); প্রত্যাগম *এতত্; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, Index);

প্রকারনির্দেশ Index NameID;

সংরচনা VarIdx : HBUINT32 {
  স্থির constexpr unsigned NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_INDEX, "");
  VarIdx& operator = (uint32_t i) { HBUINT32::operator= (i); প্রত্যাগম *এতত্; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, VarIdx);

/* Offset, Null offset = 0 */
template <typename Type, তার্কিক has_null=সত্য>
সংরচনা Offset : Type
{
  Offset& operator = (typename Type::type i) { Type::operator= (i); প্রত্যাগম *এতত্; }

  প্রকারনির্দেশ Type type;

  তার্কিক is_null () ধ্রুব { প্রত্যাগম has_null && 0 == *এতত্; }

  সার্বজনিক:
  DEFINE_SIZE_STATIC (পরিমাণ (Type));
};

প্রকারনির্দেশ Offset<HBUINT16> Offset16;
প্রকারনির্দেশ Offset<HBUINT24> Offset24;
প্রকারনির্দেশ Offset<HBUINT32> Offset32;


/* CheckSum */
সংরচনা CheckSum : HBUINT32
{
  CheckSum& operator = (uint32_t i) { HBUINT32::operator= (i); প্রত্যাগম *এতত্; }

  /* This is reference implementation from the spec. */
  স্থির uint32_t CalcTableChecksum (ধ্রুব HBUINT32 *Table, uint32_t Length)
  {
    uint32_t Sum = 0L;
    প্রতিজ্ঞা (0 == (Length & 3));
    ধ্রুব HBUINT32 *EndPtr = Table + Length / HBUINT32::static_size;

    যাবত্ (Table < EndPtr)
      Sum += *Table++;
    প্রত্যাগম Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  শূন্য set_for_data (ধ্রুব শূন্য *data, unsigned পূর্ণাঙ্ক length)
  { *এতত্ = CalcTableChecksum ((ধ্রুব HBUINT32 *) data, length); }

  সার্বজনিক:
  DEFINE_SIZE_STATIC (4);
};


/*
 * Version Numbers
 */

template <typename FixedType=HBUINT16>
সংরচনা FixedVersion
{
  uint32_t to_int () ধ্রুব { প্রত্যাগম (major << (পরিমাণ (FixedType) * 8)) + minor; }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  FixedType major;
  FixedType minor;
  সার্বজনিক:
  DEFINE_SIZE_STATIC (2 * পরিমাণ (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

template <typename Type, তার্কিক has_null>
সংরচনা _hb_has_null
{
  স্থির ধ্রুব Type *get_null () { প্রত্যাগম nullptr; }
  স্থির Type *get_crap ()       { প্রত্যাগম nullptr; }
};
template <typename Type>
সংরচনা _hb_has_null<Type, সত্য>
{
  স্থির ধ্রুব Type *get_null () { প্রত্যাগম &Null (Type); }
  স্থির       Type *get_crap () { প্রত্যাগম &Crap (Type); }
};

template <typename Type, typename OffsetType, typename BaseType=শূন্য, তার্কিক has_null=সত্য>
সংরচনা OffsetTo : Offset<OffsetType, has_null>
{
  using target_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == অসত্য ||
		 (hb_has_null_size (Type) || !hb_has_min_size (Type)), "");

  HB_DELETE_COPY_ASSIGN (OffsetTo);
  OffsetTo () = স্বভাব;

  OffsetTo& operator = (typename OffsetType::type i) { OffsetType::operator= (i); প্রত্যাগম *এতত্; }

  ধ্রুব Type& operator () (ধ্রুব শূন্য *base) ধ্রুব
  {
    যদি (unlikely (এতত্->is_null ())) প্রত্যাগম *_hb_has_null<Type, has_null>::get_null ();
    প্রত্যাগম StructAtOffset<ধ্রুব Type> (base, *এতত্);
  }
  Type& operator () (শূন্য *base) ধ্রুব
  {
    যদি (unlikely (এতত্->is_null ())) প্রত্যাগম *_hb_has_null<Type, has_null>::get_crap ();
    প্রত্যাগম StructAtOffset<Type> (base, *এতত্);
  }

  template <typename Base,
	    hb_enable_if (hb_is_convertible (ধ্রুব Base, ধ্রুব BaseType *))>
  friend ধ্রুব Type& operator + (ধ্রুব Base &base, ধ্রুব OffsetTo &offset) { প্রত্যাগম offset ((ধ্রুব শূন্য *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (ধ্রুব Base, ধ্রুব BaseType *))>
  friend ধ্রুব Type& operator + (ধ্রুব OffsetTo &offset, ধ্রুব Base &base) { প্রত্যাগম offset ((ধ্রুব শূন্য *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (Base &&base, OffsetTo &offset) { প্রত্যাগম offset ((শূন্য *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (OffsetTo &offset, Base &&base) { প্রত্যাগম offset ((শূন্য *) base); }


  template <typename Base, typename ...Ts>
  তার্কিক serialize_subset (hb_subset_context_t *c, ধ্রুব OffsetTo& src,
			 ধ্রুব Base *src_base, Ts&&... ds)
  {
    *এতত্ = 0;
    যদি (src.is_null ())
      প্রত্যাগম অসত্য;

    auto *s = c->serializer;

    s->push ();

    তার্কিক ret = c->dispatch (src_base+src, std::forward<Ts> (ds)...);

    যদি (ret || !has_null)
      s->add_link (*এতত্, s->বের_করো_pack ());
    অন্যথা
      s->বের_করো_discard ();

    প্রত্যাগম ret;
  }


  template <typename ...Ts>
  তার্কিক serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *এতত্ = 0;

    Type* obj = c->push<Type> ();
    তার্কিক ret = obj->serialize (c, std::forward<Ts> (ds)...);

    যদি (ret)
      c->add_link (*এতত্, c->বের_করো_pack ());
    অন্যথা
      c->বের_করো_discard ();

    প্রত্যাগম ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  template <typename ...Ts>
  তার্কিক serialize_copy (hb_serialize_context_t *c, ধ্রুব OffsetTo& src,
		       ধ্রুব শূন্য *src_base, unsigned dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *এতত্ = 0;
    যদি (src.is_null ())
      প্রত্যাগম অসত্য;

    c->push ();

    তার্কিক ret = c->copy (src_base+src, std::forward<Ts> (ds)...);

    c->add_link (*এতত্, c->বের_করো_pack (), whence, dst_bias);

    প্রত্যাগম ret;
  }

  তার্কিক serialize_copy (hb_serialize_context_t *c, ধ্রুব OffsetTo& src,
		       ধ্রুব শূন্য *src_base, unsigned dst_bias = 0)
  { প্রত্যাগম serialize_copy (c, src, src_base, dst_bias, hb_serialize_context_t::Head); }

  তার্কিক sanitize_shallow (hb_sanitize_context_t *c, ধ্রুব BaseType *base) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!c->check_struct (এতত্))) return_trace (অসত্য);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    যদি (unlikely ((ধ্রুব বর্ণ *) base + (unsigned) *এতত্ < (ধ্রুব বর্ণ *) base)) return_trace (অসত্য);
    return_trace (সত্য);
  }

  template <typename ...Ts>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুব BaseType *base, Ts&&... ds) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (sanitize_shallow (c, base) &&
		  hb_barrier () &&
		  (এতত্->is_null () ||
		   c->dispatch (StructAtOffset<Type> (base, *এতত্), std::forward<Ts> (ds)...)));
  }

  DEFINE_SIZE_STATIC (পরিমাণ (OffsetType));
};
/* Partial specializations. */
template <typename Type, typename BaseType=শূন্য, তার্কিক has_null=সত্য> using Offset16To = OffsetTo<Type, HBUINT16, BaseType, has_null>;
template <typename Type, typename BaseType=শূন্য, তার্কিক has_null=সত্য> using Offset24To = OffsetTo<Type, HBUINT24, BaseType, has_null>;
template <typename Type, typename BaseType=শূন্য, তার্কিক has_null=সত্য> using Offset32To = OffsetTo<Type, HBUINT32, BaseType, has_null>;

template <typename Type, typename OffsetType, typename BaseType=শূন্য> using NNOffsetTo = OffsetTo<Type, OffsetType, BaseType, অসত্য>;
template <typename Type, typename BaseType=শূন্য> using NNOffset16To = Offset16To<Type, BaseType, অসত্য>;
template <typename Type, typename BaseType=শূন্য> using NNOffset24To = Offset24To<Type, BaseType, অসত্য>;
template <typename Type, typename BaseType=শূন্য> using NNOffset32To = Offset32To<Type, BaseType, অসত্য>;


/*
 * Array Types
 */

template <typename Type>
সংরচনা UnsizedArrayOf
{
  প্রকারনির্দেশ Type item_t;
  স্থির constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (UnsizedArrayOf);

  ধ্রুব Type& operator [] (unsigned পূর্ণাঙ্ক i) ধ্রুব
  {
    প্রত্যাগম arrayZ[i];
  }
  Type& operator [] (unsigned পূর্ণাঙ্ক i)
  {
    প্রত্যাগম arrayZ[i];
  }

  স্থির size_t get_size (unsigned পূর্ণাঙ্ক len)
  { প্রত্যাগম hb_unsigned_mul_saturate (len, Type::static_size); }

  template <typename T> operator T * () { প্রত্যাগম arrayZ; }
  template <typename T> operator ধ্রুব T * () ধ্রুব { প্রত্যাগম arrayZ; }
  hb_array_t<Type> as_array (unsigned পূর্ণাঙ্ক len)
  { প্রত্যাগম hb_array (arrayZ, len); }
  hb_array_t<ধ্রুব Type> as_array (unsigned পূর্ণাঙ্ক len) ধ্রুব
  { প্রত্যাগম hb_array (arrayZ, len); }

  template <typename T>
  Type &lsearch (unsigned পূর্ণাঙ্ক len, ধ্রুব T &x, Type &not_found = Crap (Type))
  { প্রত্যাগম *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  ধ্রুব Type &lsearch (unsigned পূর্ণাঙ্ক len, ধ্রুব T &x, ধ্রুব Type &not_found = Null (Type)) ধ্রুব
  { প্রত্যাগম *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  তার্কিক lfind (unsigned পূর্ণাঙ্ক len, ধ্রুব T &x, unsigned পূর্ণাঙ্ক *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned পূর্ণাঙ্ক to_store = (unsigned পূর্ণাঙ্ক) -1) ধ্রুব
  { প্রত্যাগম as_array (len).lfind (x, i, not_found, to_store); }

  শূন্য qsort (unsigned পূর্ণাঙ্ক len, unsigned পূর্ণাঙ্ক start = 0, unsigned পূর্ণাঙ্ক end = (unsigned পূর্ণাঙ্ক) -1)
  { as_array (len).qsort (start, end); }

  তার্কিক serialize (hb_serialize_context_t *c, unsigned পূর্ণাঙ্ক বস্তুসমূহ_len, তার্কিক clear = সত্য)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_size (এতত্, get_size (বস্তুসমূহ_len), clear))) return_trace (অসত্য);
    return_trace (সত্য);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  তার্কিক serialize (hb_serialize_context_t *c, Iterator বস্তুসমূহ)
  {
    TRACE_SERIALIZE (এতত্);
    unsigned গুনতি = hb_len (বস্তুসমূহ);
    যদি (unlikely (!serialize (c, গুনতি, অসত্য))) return_trace (অসত্য);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    প্রতি (unsigned i = 0; i < গুনতি; i++, ++বস্তুসমূহ)
      arrayZ[i] = *বস্তুসমূহ;
    return_trace (সত্য);
  }

  UnsizedArrayOf* copy (hb_serialize_context_t *c, unsigned গুনতি) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);
    auto *out = c->start_embed (এতত্);
    যদি (unlikely (!as_array (গুনতি).copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণাঙ্ক গুনতি, Ts&&... ds) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!sanitize_shallow (c, গুনতি))) return_trace (অসত্য);
    যদি (!পরিমাণ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (সত্য);
    hb_barrier ();
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (অসত্য);
    return_trace (সত্য);
  }

  তার্কিক sanitize_shallow (hb_sanitize_context_t *c, unsigned পূর্ণাঙ্ক গুনতি) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_array (arrayZ, গুনতি));
  }

  সার্বজনিক:
  Type		arrayZ[HB_VAR_ARRAY];
  সার্বজনিক:
  DEFINE_SIZE_UNBOUNDED (0);
};

/* Unsized array of offset's */
template <typename Type, typename OffsetType, typename BaseType=শূন্য, তার্কিক has_null=সত্য>
using UnsizedArray16OfOffsetTo = UnsizedArrayOf<OffsetTo<Type, OffsetType, BaseType, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
template <typename Type, typename OffsetType, typename BaseType=শূন্য, তার্কিক has_null=সত্য>
সংরচনা UnsizedListOfOffset16To : UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
{
  ধ্রুব Type& operator [] (পূর্ণাঙ্ক i_) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক i = (unsigned পূর্ণাঙ্ক) i_;
    ধ্রুব OffsetTo<Type, OffsetType, BaseType, has_null> *p = &এতত্->arrayZ[i];
    যদি (unlikely ((ধ্রুব শূন্য *) p < (ধ্রুব শূন্য *) এতত্->arrayZ)) প্রত্যাগম Null (Type); /* Overflowed. */
    hb_barrier ();
    প্রত্যাগম এতত্+*p;
  }
  Type& operator [] (পূর্ণাঙ্ক i_)
  {
    unsigned পূর্ণাঙ্ক i = (unsigned পূর্ণাঙ্ক) i_;
    ধ্রুব OffsetTo<Type, OffsetType, BaseType, has_null> *p = &এতত্->arrayZ[i];
    যদি (unlikely ((ধ্রুব শূন্য *) p < (ধ্রুব শূন্য *) এতত্->arrayZ)) প্রত্যাগম Crap (Type); /* Overflowed. */
    hb_barrier ();
    প্রত্যাগম এতত্+*p;
  }

  template <typename ...Ts>
  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণাঙ্ক গুনতি, Ts&&... ds) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace ((UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
		   ::sanitize (c, গুনতি, এতত্, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type>
সংরচনা SortedUnsizedArrayOf : UnsizedArrayOf<Type>
{
  hb_sorted_array_t<Type> as_array (unsigned পূর্ণাঙ্ক len)
  { প্রত্যাগম hb_sorted_array (এতত্->arrayZ, len); }
  hb_sorted_array_t<ধ্রুব Type> as_array (unsigned পূর্ণাঙ্ক len) ধ্রুব
  { প্রত্যাগম hb_sorted_array (এতত্->arrayZ, len); }
  operator hb_sorted_array_t<Type> ()             { প্রত্যাগম as_array (); }
  operator hb_sorted_array_t<ধ্রুব Type> () ধ্রুব { প্রত্যাগম as_array (); }

  template <typename T>
  Type &bsearch (unsigned পূর্ণাঙ্ক len, ধ্রুব T &x, Type &not_found = Crap (Type))
  { প্রত্যাগম *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  ধ্রুব Type &bsearch (unsigned পূর্ণাঙ্ক len, ধ্রুব T &x, ধ্রুব Type &not_found = Null (Type)) ধ্রুব
  { প্রত্যাগম *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  তার্কিক bfind (unsigned পূর্ণাঙ্ক len, ধ্রুব T &x, unsigned পূর্ণাঙ্ক *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned পূর্ণাঙ্ক to_store = (unsigned পূর্ণাঙ্ক) -1) ধ্রুব
  { প্রত্যাগম as_array (len).bfind (x, i, not_found, to_store); }
};


/* An array with a number of elements. */
template <typename Type, typename LenType>
সংরচনা ArrayOf
{
  প্রকারনির্দেশ Type item_t;
  স্থির constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOf);

  ধ্রুব Type& operator [] (পূর্ণাঙ্ক i_) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক i = (unsigned পূর্ণাঙ্ক) i_;
    যদি (unlikely (i >= len)) প্রত্যাগম Null (Type);
    hb_barrier ();
    প্রত্যাগম arrayZ[i];
  }
  Type& operator [] (পূর্ণাঙ্ক i_)
  {
    unsigned পূর্ণাঙ্ক i = (unsigned পূর্ণাঙ্ক) i_;
    যদি (unlikely (i >= len)) প্রত্যাগম Crap (Type);
    hb_barrier ();
    প্রত্যাগম arrayZ[i];
  }

  size_t get_size () ধ্রুব
  { প্রত্যাগম hb_unsigned_mul_add_saturate (len, Type::static_size, len.static_size); }

  explicit operator তার্কিক () ধ্রুব { প্রত্যাগম len; }

  শূন্য বের_করো () { len--; }

  hb_array_t<      Type> as_array ()       { প্রত্যাগম hb_array (arrayZ, len); }
  hb_array_t<ধ্রুব Type> as_array () ধ্রুব { প্রত্যাগম hb_array (arrayZ, len); }

  /* Iterator. */
  প্রকারনির্দেশ hb_array_t<ধ্রুব Type>   iter_t;
  প্রকারনির্দেশ hb_array_t<      Type> writer_t;
    iter_t   iter () ধ্রুব { প্রত্যাগম as_array (); }
  writer_t writer ()       { প্রত্যাগম as_array (); }
  operator   iter_t () ধ্রুব { প্রত্যাগম   iter (); }
  operator writer_t ()       { প্রত্যাগম writer (); }

  /* Faster range-based for loop. */
  ধ্রুব Type *begin () ধ্রুব { প্রত্যাগম arrayZ; }
  ধ্রুব Type *end () ধ্রুব { প্রত্যাগম arrayZ + len; }

  template <typename T>
  Type &lsearch (ধ্রুব T &x, Type &not_found = Crap (Type))
  { প্রত্যাগম *as_array ().lsearch (x, &not_found); }
  template <typename T>
  ধ্রুব Type &lsearch (ধ্রুব T &x, ধ্রুব Type &not_found = Null (Type)) ধ্রুব
  { প্রত্যাগম *as_array ().lsearch (x, &not_found); }
  template <typename T>
  তার্কিক lfind (ধ্রুব T &x, unsigned পূর্ণাঙ্ক *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned পূর্ণাঙ্ক to_store = (unsigned পূর্ণাঙ্ক) -1) ধ্রুব
  { প্রত্যাগম as_array ().lfind (x, i, not_found, to_store); }

  শূন্য qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD তার্কিক serialize (hb_serialize_context_t *c, unsigned বস্তুসমূহ_len, তার্কিক clear = সত্য)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (অসত্য);
    c->check_assign (len, বস্তুসমূহ_len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    যদি (unlikely (!c->extend_size (এতত্, get_size (), clear))) return_trace (অসত্য);
    return_trace (সত্য);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD তার্কিক serialize (hb_serialize_context_t *c, Iterator বস্তুসমূহ)
  {
    TRACE_SERIALIZE (এতত্);
    unsigned গুনতি = hb_len (বস্তুসমূহ);
    যদি (unlikely (!serialize (c, গুনতি, অসত্য))) return_trace (অসত্য);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    প্রতি (unsigned i = 0; i < গুনতি; i++, ++বস্তুসমূহ)
      arrayZ[i] = *বস্তুসমূহ;
    return_trace (সত্য);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (এতত্);
    len++;
    যদি (unlikely (!len || !c->extend (এতত্)))
    {
      len--;
      return_trace (nullptr);
    }
    return_trace (&arrayZ[len - 1]);
  }

  ArrayOf* copy (hb_serialize_context_t *c) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);
    auto *out = c->start_embed (এতত্);
    যদি (unlikely (!c->extend_min (out))) return_trace (nullptr);
    c->check_assign (out->len, len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    যদি (unlikely (!as_array ().copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  তার্কিক sanitize (hb_sanitize_context_t *c, Ts&&... ds) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!sanitize_shallow (c))) return_trace (অসত্য);
    যদি (!পরিমাণ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (সত্য);
    hb_barrier ();
    unsigned পূর্ণাঙ্ক গুনতি = len;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (অসত্য);
    return_trace (সত্য);
  }

  তার্কিক sanitize_shallow (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (len.sanitize (c) &&
		  hb_barrier () &&
		  c->check_array_sized (arrayZ, len, পরিমাণ (LenType)));
  }

  সার্বজনিক:
  LenType	len;
  Type		arrayZ[HB_VAR_ARRAY];
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (পরিমাণ (LenType), arrayZ);
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
সংরচনা List16OfOffsetTo : ArrayOf<OffsetTo<Type, OffsetType>, HBUINT16>
{
  ধ্রুব Type& operator [] (পূর্ণাঙ্ক i_) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক i = (unsigned পূর্ণাঙ্ক) i_;
    যদি (unlikely (i >= এতত্->len)) প্রত্যাগম Null (Type);
    hb_barrier ();
    প্রত্যাগম এতত্+এতত্->arrayZ[i];
  }
  ধ্রুব Type& operator [] (পূর্ণাঙ্ক i_)
  {
    unsigned পূর্ণাঙ্ক i = (unsigned পূর্ণাঙ্ক) i_;
    যদি (unlikely (i >= এতত্->len)) প্রত্যাগম Crap (Type);
    hb_barrier ();
    প্রত্যাগম এতত্+এতত্->arrayZ[i];
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    সংরচনা List16OfOffsetTo *out = c->serializer->embed (*এতত্);
    যদি (unlikely (!out)) return_trace (অসত্য);
    unsigned পূর্ণাঙ্ক গুনতি = এতত্->len;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      out->arrayZ[i].serialize_subset (c, এতত্->arrayZ[i], এতত্, out);
    return_trace (সত্য);
  }

  template <typename ...Ts>
  তার্কিক sanitize (hb_sanitize_context_t *c, Ts&&... ds) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace ((Array16Of<OffsetTo<Type, OffsetType>>::sanitize (c, এতত্, std::forward<Ts> (ds)...)));
  }
};

template <typename Type>
using List16OfOffset16To = List16OfOffsetTo<Type, HBUINT16>;

/* An array starting at second element. */
template <typename Type, typename LenType>
সংরচনা HeadlessArrayOf
{
  স্থির constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (HeadlessArrayOf);

  ধ্রুব Type& operator [] (পূর্ণাঙ্ক i_) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক i = (unsigned পূর্ণাঙ্ক) i_;
    যদি (unlikely (i >= lenP1 || !i)) প্রত্যাগম Null (Type);
    hb_barrier ();
    প্রত্যাগম arrayZ[i-1];
  }
  Type& operator [] (পূর্ণাঙ্ক i_)
  {
    unsigned পূর্ণাঙ্ক i = (unsigned পূর্ণাঙ্ক) i_;
    যদি (unlikely (i >= lenP1 || !i)) প্রত্যাগম Crap (Type);
    hb_barrier ();
    প্রত্যাগম arrayZ[i-1];
  }
  size_t get_size () ধ্রুব
  { প্রত্যাগম hb_unsigned_mul_add_saturate (get_length (), Type::static_size, lenP1.static_size); }

  unsigned get_length () ধ্রুব { প্রত্যাগম lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { প্রত্যাগম hb_array (arrayZ, get_length ()); }
  hb_array_t<ধ্রুব Type> as_array () ধ্রুব { প্রত্যাগম hb_array (arrayZ, get_length ()); }

  /* Iterator. */
  প্রকারনির্দেশ hb_array_t<ধ্রুব Type>   iter_t;
  প্রকারনির্দেশ hb_array_t<      Type> writer_t;
    iter_t   iter () ধ্রুব { প্রত্যাগম as_array (); }
  writer_t writer ()       { প্রত্যাগম as_array (); }
  operator   iter_t () ধ্রুব { প্রত্যাগম   iter (); }
  operator writer_t ()       { প্রত্যাগম writer (); }

  /* Faster range-based for loop. */
  ধ্রুব Type *begin () ধ্রুব { প্রত্যাগম arrayZ; }
  ধ্রুব Type *end () ধ্রুব { প্রত্যাগম arrayZ + get_length (); }

  HB_NODISCARD তার্কিক serialize (hb_serialize_context_t *c, unsigned পূর্ণাঙ্ক বস্তুসমূহ_len, তার্কিক clear = সত্য)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (অসত্য);
    c->check_assign (lenP1, বস্তুসমূহ_len + 1, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    যদি (unlikely (!c->extend_size (এতত্, get_size (), clear))) return_trace (অসত্য);
    return_trace (সত্য);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD তার্কিক serialize (hb_serialize_context_t *c, Iterator বস্তুসমূহ)
  {
    TRACE_SERIALIZE (এতত্);
    unsigned গুনতি = hb_len (বস্তুসমূহ);
    যদি (unlikely (!serialize (c, গুনতি, অসত্য))) return_trace (অসত্য);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    প্রতি (unsigned i = 0; i < গুনতি; i++, ++বস্তুসমূহ)
      arrayZ[i] = *বস্তুসমূহ;
    return_trace (সত্য);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  তার্কিক sanitize (hb_sanitize_context_t *c, Ts&&... ds) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!sanitize_shallow (c))) return_trace (অসত্য);
    যদি (!পরিমাণ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (সত্য);
    hb_barrier ();
    unsigned পূর্ণাঙ্ক গুনতি = get_length ();
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (অসত্য);
    return_trace (সত্য);
  }

  গুপ্ত:
  তার্কিক sanitize_shallow (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (lenP1.sanitize (c) &&
		  hb_barrier () &&
		  (!lenP1 || c->check_array_sized (arrayZ, lenP1 - 1, পরিমাণ (LenType))));
  }

  সার্বজনিক:
  LenType	lenP1;
  Type		arrayZ[HB_VAR_ARRAY];
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (পরিমাণ (LenType), arrayZ);
};
template <typename Type> using HeadlessArray16Of = HeadlessArrayOf<Type, HBUINT16>;

/* An array storing length-1. */
template <typename Type, typename LenType=HBUINT16>
সংরচনা ArrayOfM1
{
  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOfM1);

  ধ্রুব Type& operator [] (পূর্ণাঙ্ক i_) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক i = (unsigned পূর্ণাঙ্ক) i_;
    যদি (unlikely (i > lenM1)) প্রত্যাগম Null (Type);
    hb_barrier ();
    প্রত্যাগম arrayZ[i];
  }
  Type& operator [] (পূর্ণাঙ্ক i_)
  {
    unsigned পূর্ণাঙ্ক i = (unsigned পূর্ণাঙ্ক) i_;
    যদি (unlikely (i > lenM1)) প্রত্যাগম Crap (Type);
    hb_barrier ();
    প্রত্যাগম arrayZ[i];
  }
  size_t get_size () ধ্রুব
  { প্রত্যাগম hb_unsigned_mul_add_saturate (lenM1 + 1, Type::static_size, lenM1.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  তার্কিক sanitize (hb_sanitize_context_t *c, Ts&&... ds) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!sanitize_shallow (c))) return_trace (অসত্য);
    যদি (!পরিমাণ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (সত্য);
    hb_barrier ();
    unsigned পূর্ণাঙ্ক গুনতি = lenM1 + 1;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (অসত্য);
    return_trace (সত্য);
  }

  গুপ্ত:
  তার্কিক sanitize_shallow (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (lenM1.sanitize (c) &&
		  hb_barrier () &&
		  (c->check_array_sized (arrayZ, lenM1 + 1, পরিমাণ (LenType))));
  }

  সার্বজনিক:
  LenType	lenM1;
  Type		arrayZ[HB_VAR_ARRAY];
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (পরিমাণ (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type, typename LenType>
সংরচনা SortedArrayOf : ArrayOf<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { প্রত্যাগম hb_sorted_array (এতত্->arrayZ, এতত্->len); }
  hb_sorted_array_t<ধ্রুব Type> as_array () ধ্রুব { প্রত্যাগম hb_sorted_array (এতত্->arrayZ, এতত্->len); }

  /* Iterator. */
  প্রকারনির্দেশ hb_sorted_array_t<ধ্রুব Type>   iter_t;
  প্রকারনির্দেশ hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () ধ্রুব { প্রত্যাগম as_array (); }
  writer_t writer ()       { প্রত্যাগম as_array (); }
  operator   iter_t () ধ্রুব { প্রত্যাগম   iter (); }
  operator writer_t ()       { প্রত্যাগম writer (); }

  /* Faster range-based for loop. */
  ধ্রুব Type *begin () ধ্রুব { প্রত্যাগম এতত্->arrayZ; }
  ধ্রুব Type *end () ধ্রুব { প্রত্যাগম এতত্->arrayZ + এতত্->len; }

  তার্কিক serialize (hb_serialize_context_t *c, unsigned পূর্ণাঙ্ক বস্তুসমূহ_len)
  {
    TRACE_SERIALIZE (এতত্);
    তার্কিক ret = ArrayOf<Type, LenType>::serialize (c, বস্তুসমূহ_len);
    return_trace (ret);
  }
  template <typename Iterator,
	    hb_requires (hb_is_sorted_source_of (Iterator, Type))>
  তার্কিক serialize (hb_serialize_context_t *c, Iterator বস্তুসমূহ)
  {
    TRACE_SERIALIZE (এতত্);
    তার্কিক ret = ArrayOf<Type, LenType>::serialize (c, বস্তুসমূহ);
    return_trace (ret);
  }

  SortedArrayOf* copy (hb_serialize_context_t *c) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);
    SortedArrayOf* out = reinterpret_cast<SortedArrayOf *> (ArrayOf<Type, LenType>::copy (c));
    return_trace (out);
  }

  template <typename T>
  Type &bsearch (ধ্রুব T &x, Type &not_found = Crap (Type))
  { প্রত্যাগম *as_array ().bsearch (x, &not_found); }
  template <typename T>
  ধ্রুব Type &bsearch (ধ্রুব T &x, ধ্রুব Type &not_found = Null (Type)) ধ্রুব
  { প্রত্যাগম *as_array ().bsearch (x, &not_found); }
  template <typename T>
  তার্কিক bfind (ধ্রুব T &x, unsigned পূর্ণাঙ্ক *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned পূর্ণাঙ্ক to_store = (unsigned পূর্ণাঙ্ক) -1) ধ্রুব
  { প্রত্যাগম as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type> using SortedArray16Of = SortedArrayOf<Type, HBUINT16>;
template <typename Type> using SortedArray24Of = SortedArrayOf<Type, HBUINT24>;
template <typename Type> using SortedArray32Of = SortedArrayOf<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

template <typename LenType=HBUINT16>
সংরচনা Bin_অনুসন্ধান_Header
{
  operator uint32_t () ধ্রুব { প্রত্যাগম len; }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  Bin_অনুসন্ধান_Header& operator = (unsigned পূর্ণাঙ্ক v)
  {
    len = v;
    প্রতিজ্ঞা (len == v);
    entrySelector = hb_max (1u, hb_bit_storage (v)) - 1;
    অনুসন্ধান_Range = 16 * (1u << entrySelector);
    rangeShift = v * 16 > অনুসন্ধান_Range
		 ? 16 * v - অনুসন্ধান_Range
		 : 0;
    প্রত্যাগম *এতত্;
  }

  রক্ষিত:
  LenType	len;
  LenType	অনুসন্ধান_Range;
  LenType	entrySelector;
  LenType	rangeShift;

  সার্বজনিক:
  DEFINE_SIZE_STATIC (8);
};

template <typename Type, typename LenType=HBUINT16>
using Bin_অনুসন্ধান_Array_Of = SortedArrayOf<Type, Bin_অনুসন্ধান_Header<LenType>>;


সংরচনা Var_Sized_Bin_অনুসন্ধান_Header
{

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  HBUINT16	unitSize;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	অনুসন্ধান_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	entrySelector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	rangeShift;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  সার্বজনিক:
  DEFINE_SIZE_STATIC (10);
};

template <typename Type>
সংরচনা Var_Sized_Bin_অনুসন্ধান_Array_Of
{
  স্থির constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (Var_Sized_Bin_অনুসন্ধান_Array_Of);

  তার্কিক last_is_terminator () ধ্রুব
  {
    যদি (unlikely (!header.nUnits)) প্রত্যাগম অসত্য;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    ধ্রুব HBUINT16 *শব্দগুলি = &StructAtOffset<HBUINT16> (&bytesZ, (header.nUnits - 1) * header.unitSize);
    unsigned পূর্ণাঙ্ক গুনতি = Type::Termination_Word_গুনতি;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (শব্দগুলি[i] != 0xFFFFu)
	প্রত্যাগম অসত্য;
    প্রত্যাগম সত্য;
  }

  ধ্রুব Type& operator [] (পূর্ণাঙ্ক i_) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক i = (unsigned পূর্ণাঙ্ক) i_;
    যদি (unlikely (i >= get_length ())) প্রত্যাগম Null (Type);
    hb_barrier ();
    প্রত্যাগম StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  Type& operator [] (পূর্ণাঙ্ক i_)
  {
    unsigned পূর্ণাঙ্ক i = (unsigned পূর্ণাঙ্ক) i_;
    যদি (unlikely (i >= get_length ())) প্রত্যাগম Crap (Type);
    hb_barrier ();
    প্রত্যাগম StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  unsigned পূর্ণাঙ্ক get_length () ধ্রুব
  { প্রত্যাগম header.nUnits - last_is_terminator (); }
  size_t get_size () ধ্রুব
  { প্রত্যাগম hb_unsigned_mul_add_saturate (header.nUnits, header.unitSize, header.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  তার্কিক sanitize (hb_sanitize_context_t *c, Ts&&... ds) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!sanitize_shallow (c))) return_trace (অসত্য);
    যদি (!পরিমাণ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (সত্য);
    hb_barrier ();
    unsigned পূর্ণাঙ্ক গুনতি = get_length ();
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (unlikely (!(*এতত্)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (অসত্য);
    return_trace (সত্য);
  }

  template <typename T>
  ধ্রুব Type *bsearch (ধ্রুব T &key) ধ্রুব
  {
    unsigned pos;
    প্রত্যাগম hb_bsearch_impl (&pos,
			    key,
			    (ধ্রুব শূন্য *) bytesZ,
			    get_length (),
			    header.unitSize,
			    _hb_cmp_method<T, Type>)
	   ? (ধ্রুব Type *) (((ধ্রুব বর্ণ *) &bytesZ) + (pos * header.unitSize))
	   : nullptr;
  }

  গুপ্ত:
  তার্কিক sanitize_shallow (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (header.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_size <= header.unitSize &&
		  c->check_range (bytesZ.arrayZ,
				  header.nUnits,
				  header.unitSize));
  }

  রক্ষিত:
  Var_Sized_Bin_অনুসন্ধান_Header	header;
  UnsizedArrayOf<HBUINT8>	bytesZ;
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (10, bytesZ);
};


/* CFF INDEX */

template <typename গুনতি_3>
সংরচনা CFFIndex
{
  unsigned পূর্ণাঙ্ক offset_array_size () ধ্রুব
  { প্রত্যাগম offSize * (গুনতি + 1); }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুব Iterable &iterable,
		  ধ্রুব unsigned *p_data_size = nullptr,
                  unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (এতত্);
    unsigned data_size;
    যদি (p_data_size)
      data_size = *p_data_size;
    অন্যথা
      total_size (iterable, &data_size);

    auto it = hb_iter (iterable);
    যদি (unlikely (!serialize_header (c, +it, data_size, min_off_size))) return_trace (অসত্য);
    unsigned বর্ণ *ret = c->allocate_size<unsigned বর্ণ> (data_size, অসত্য);
    যদি (unlikely (!ret)) return_trace (অসত্য);
    unsigned remaining = data_size;
    প্রতি (ধ্রুব auto &_ : +it)
    {
      unsigned len = _.length;

      যদি (!len)
	অনুবর্ত;

      যদি (unlikely (len > remaining)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (অসত্য, HB_SERIALIZE_ERROR_INT_OVERFLOW));
      }

      remaining -= len;

      যদি (len <= 1)
      {
	*ret++ = *_.arrayZ;
	অনুবর্ত;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (সত্য);
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  তার্কিক serialize_header (hb_serialize_context_t *c,
			 Iterator it,
			 unsigned data_size,
                         unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (এতত্);

    unsigned off_size = (hb_bit_storage (data_size + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    /* serialize CFFIndex header */
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (অসত্য);
    এতত্->গুনতি = hb_len (it);
    যদি (!এতত্->গুনতি) return_trace (সত্য);
    যদি (unlikely (!c->extend (এতত্->offSize))) return_trace (অসত্য);
    এতত্->offSize = off_size;
    যদি (unlikely (!c->allocate_size<HBUINT8> (off_size * (এতত্->গুনতি + 1), অসত্য)))
      return_trace (অসত্য);

    /* serialize indices */
    unsigned পূর্ণাঙ্ক offset = 1;
    যদি (HB_OPTIMIZE_SIZE_VAL)
    {
      unsigned পূর্ণাঙ্ক i = 0;
      প্রতি (ধ্রুব auto &_ : +it)
      {
	set_offset_at (i++, offset);
	offset += hb_len_of (_);
      }
      set_offset_at (i, offset);
    }
    অন্যথা
      বিকল্প (off_size)
      {
	পক্ষ 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  প্রতি (ধ্রুব auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	ভঙ্গ;
	পক্ষ 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  প্রতি (ধ্রুব auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	ভঙ্গ;
	পক্ষ 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  প্রতি (ধ্রুব auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	ভঙ্গ;
	পক্ষ 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  প্রতি (ধ্রুব auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	ভঙ্গ;
	স্বভাব:
	ভঙ্গ;
      }

    প্রতিজ্ঞা (offset == data_size + 1);
    return_trace (সত্য);
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  স্থির unsigned total_size (ধ্রুব Iterable &iterable, unsigned *data_size = nullptr, unsigned min_off_size = 0)
  {
    auto it = + hb_iter (iterable);
    যদি (!it)
    {
      যদি (data_size) *data_size = 0;
      প্রত্যাগম min_size;
    }

    unsigned total = 0;
    প্রতি (ধ্রুব auto &_ : +it)
      total += hb_len_of (_);

    যদি (data_size) *data_size = total;

    unsigned off_size = (hb_bit_storage (total + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    প্রত্যাগম min_size + HBUINT8::static_size + (hb_len (it) + 1) * off_size + total;
  }

  শূন্য set_offset_at (unsigned পূর্ণাঙ্ক index, unsigned পূর্ণাঙ্ক offset)
  {
    প্রতিজ্ঞা (index <= গুনতি);

    unsigned পূর্ণাঙ্ক size = offSize;
    ধ্রুব HBUINT8 *p = offsets;
    বিকল্প (size)
    {
      পক্ষ 1: ((HBUINT8  *) p)[index] = offset; ভঙ্গ;
      পক্ষ 2: ((HBUINT16 *) p)[index] = offset; ভঙ্গ;
      পক্ষ 3: ((HBUINT24 *) p)[index] = offset; ভঙ্গ;
      পক্ষ 4: ((HBUINT32 *) p)[index] = offset; ভঙ্গ;
      স্বভাব: প্রত্যাগম;
    }
  }

  গুপ্ত:
  unsigned পূর্ণাঙ্ক offset_at (unsigned পূর্ণাঙ্ক index) ধ্রুব
  {
    প্রতিজ্ঞা (index <= গুনতি);

    unsigned পূর্ণাঙ্ক size = offSize;
    ধ্রুব HBUINT8 *p = offsets;
    বিকল্প (size)
    {
      পক্ষ 1: প্রত্যাগম ((HBUINT8  *) p)[index];
      পক্ষ 2: প্রত্যাগম ((HBUINT16 *) p)[index];
      পক্ষ 3: প্রত্যাগম ((HBUINT24 *) p)[index];
      পক্ষ 4: প্রত্যাগম ((HBUINT32 *) p)[index];
      স্বভাব: প্রত্যাগম 0;
    }
  }

  ধ্রুব unsigned বর্ণ *data_base () ধ্রুব
  { প্রত্যাগম (ধ্রুব unsigned বর্ণ *) এতত্ + min_size + offSize.static_size - 1 + offset_array_size (); }
  সার্বজনিক:

  hb_ubytes_t operator [] (unsigned পূর্ণাঙ্ক index) ধ্রুব
  {
    যদি (unlikely (index >= গুনতি)) প্রত্যাগম hb_ubytes_t ();
    hb_barrier ();
    unsigned offset0 = offset_at (index);
    unsigned offset1 = offset_at (index + 1);
    যদি (unlikely (offset1 < offset0 || offset1 > offset_at (গুনতি)))
      প্রত্যাগম hb_ubytes_t ();
    প্রত্যাগম hb_ubytes_t (data_base () + offset0, offset1 - offset0);
  }

  size_t get_size () ধ্রুব
  {
    যদি (গুনতি)
      প্রত্যাগম hb_unsigned_add_saturate (min_size, offSize.static_size,
				       offset_array_size (), offset_at (গুনতি) - 1);
    প্রত্যাগম min_size;  /* empty CFFIndex contains count only */
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  hb_barrier () &&
			  (গুনতি == 0 || /* empty INDEX */
			   (গুনতি < গুনতি + 1u &&
			    c->check_struct (&offSize) && offSize >= 1 && offSize <= 4 &&
			    c->check_array (offsets, offSize, গুনতি + 1u) &&
			    c->check_range (data_base (), offset_at (গুনতি))))));
  }

  সার্বজনিক:
  গুনতি_3		গুনতি;		/* Number of object data. Note there are (count+1) offsets */
  গুপ্ত:
  HBUINT8	offSize;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  সার্বজনিক:
  DEFINE_SIZE_MIN (গুনতি_3::static_size);
};
প্রকারনির্দেশ CFFIndex<HBUINT16> CFF1Index;
প্রকারনির্দেশ CFFIndex<HBUINT32> CFF2Index;


/* TupleValues */
সংরচনা Tuple_মানসমূহ
{
  গণনা packed_মান_flag_t
  {
    মানসমূহ_ARE_ZEROS     = 0x80,
    মানসমূহ_ARE_BYTES     = 0x00,
    মানসমূহ_ARE_শব্দগুলি     = 0x40,
    মানসমূহ_ARE_LONGS     = 0xC0,
    মানসমূহ_SIZE_MASK     = 0xC0,
    মান_RUN_গুনতি_MASK = 0x3F
  };

  স্থির unsigned compile_unsafe (hb_array_t<ধ্রুব পূর্ণাঙ্ক> values, /* IN */
				  unsigned বর্ণ *encoded_bytes /* OUT */)
  {
    unsigned অঙ্ক_মানসমূহ_3 = values.length;
    unsigned encoded_len = 0;
    unsigned i = 0;
    যাবত্ (i < অঙ্ক_মানসমূহ_3)
    {
      পূর্ণাঙ্ক val = values.arrayZ[i];
      যদি (val == 0)
        encoded_len += encode_মান_run_as_zeroes (i, encoded_bytes + encoded_len, values);
      অন্যথা যদি ((int8_t) val == val)
        encoded_len += encode_মান_run_as_bytes (i, encoded_bytes + encoded_len, values);
      অন্যথা যদি ((int16_t) val == val)
        encoded_len += encode_মান_run_as_শব্দগুলি (i, encoded_bytes + encoded_len, values);
      অন্যথা
        encoded_len += encode_মান_run_as_longs (i, encoded_bytes + encoded_len, values);
    }
    প্রত্যাগম encoded_len;
  }

  স্থির unsigned encode_মান_run_as_zeroes (unsigned& i,
					      unsigned বর্ণ *it,
					      hb_array_t<ধ্রুব পূর্ণাঙ্ক> values)
  {
    unsigned অঙ্ক_মানসমূহ_3 = values.length;
    unsigned run_length = 0;
    unsigned encoded_len = 0;
    যাবত্ (i < অঙ্ক_মানসমূহ_3 && values.arrayZ[i] == 0)
    {
      i++;
      run_length++;
    }

    যাবত্ (run_length >= 64)
    {
      *it++ = বর্ণ (মানসমূহ_ARE_ZEROS | 63);
      run_length -= 64;
      encoded_len++;
    }

    যদি (run_length)
    {
      *it++ = বর্ণ (মানসমূহ_ARE_ZEROS | (run_length - 1));
      encoded_len++;
    }
    প্রত্যাগম encoded_len;
  }

  স্থির unsigned encode_মান_run_as_bytes (unsigned &i,
					     unsigned বর্ণ *it,
					     hb_array_t<ধ্রুব পূর্ণাঙ্ক> values)
  {
    unsigned start = i;
    unsigned অঙ্ক_মানসমূহ_3 = values.length;
    যাবত্ (i < অঙ্ক_মানসমূহ_3)
    {
      পূর্ণাঙ্ক val = values.arrayZ[i];
      যদি ((int8_t) val != val)
        ভঙ্গ;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      যদি (val == 0 && i + 1 < অঙ্ক_মানসমূহ_3 && values.arrayZ[i+1] == 0)
        ভঙ্গ;

      i++;
    }
    unsigned run_length = i - start;

    unsigned encoded_len = 0;

    যাবত্ (run_length >= 64)
    {
      *it++ = (মানসমূহ_ARE_BYTES | 63);
      encoded_len++;

      প্রতি (unsigned j = 0; j < 64; j++)
	it[j] = static_cast<বর্ণ> (values.arrayZ[start + j]);
      it += 64;
      encoded_len += 64;

      start += 64;
      run_length -= 64;
    }

    যদি (run_length)
    {
      *it++ = (মানসমূহ_ARE_BYTES | (run_length - 1));
      encoded_len++;

      প্রতি (unsigned j = 0; j < run_length; j++)
        it[j] = static_cast<বর্ণ> (values.arrayZ[start + j]);
      encoded_len += run_length;
    }

    প্রত্যাগম encoded_len;
  }

  স্থির unsigned encode_মান_run_as_শব্দগুলি (unsigned &i,
					     unsigned বর্ণ *it,
					     hb_array_t<ধ্রুব পূর্ণাঙ্ক> values)
  {
    unsigned start = i;
    unsigned অঙ্ক_মানসমূহ_3 = values.length;
    যাবত্ (i < অঙ্ক_মানসমূহ_3)
    {
      পূর্ণাঙ্ক val = values.arrayZ[i];

      যদি ((int16_t) val != val)
        ভঙ্গ;

      /* start a new run for a single zero value. */
      যদি (val == 0) ভঙ্গ;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      যদি ((int8_t) val == val &&
          i + 1 < অঙ্ক_মানসমূহ_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        ভঙ্গ;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    যাবত্ (run_length >= 64)
    {
      *it++ = (মানসমূহ_ARE_শব্দগুলি | 63);
      encoded_len++;

      প্রতি (unsigned j = 0; j < 64; j++)
      {
        int16_t মান_val = values.arrayZ[start + j];
        *it++ = static_cast<বর্ণ> (মান_val >> 8);
        *it++ = static_cast<বর্ণ> (মান_val & 0xFF);

        encoded_len += 2;
      }

      start += 64;
      run_length -= 64;
    }

    যদি (run_length)
    {
      *it++ = (মানসমূহ_ARE_শব্দগুলি | (run_length - 1));
      encoded_len++;
      যাবত্ (start < i)
      {
        int16_t মান_val = values.arrayZ[start++];
        *it++ = static_cast<বর্ণ> (মান_val >> 8);
        *it++ = static_cast<বর্ণ> (মান_val & 0xFF);

        encoded_len += 2;
      }
    }
    প্রত্যাগম encoded_len;
  }

  স্থির unsigned encode_মান_run_as_longs (unsigned &i,
					     unsigned বর্ণ *it,
					     hb_array_t<ধ্রুব পূর্ণাঙ্ক> values)
  {
    unsigned start = i;
    unsigned অঙ্ক_মানসমূহ_3 = values.length;
    যাবত্ (i < অঙ্ক_মানসমূহ_3)
    {
      পূর্ণাঙ্ক val = values.arrayZ[i];

      যদি ((int16_t) val == val)
        ভঙ্গ;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    যাবত্ (run_length >= 64)
    {
      *it++ = (মানসমূহ_ARE_LONGS | 63);
      encoded_len++;

      প্রতি (unsigned j = 0; j < 64; j++)
      {
        int32_t মান_val = values.arrayZ[start + j];
        *it++ = static_cast<বর্ণ> (মান_val >> 24);
        *it++ = static_cast<বর্ণ> (মান_val >> 16);
        *it++ = static_cast<বর্ণ> (মান_val >> 8);
        *it++ = static_cast<বর্ণ> (মান_val & 0xFF);

        encoded_len += 4;
      }

      start += 64;
      run_length -= 64;
    }

    যদি (run_length)
    {
      *it++ = (মানসমূহ_ARE_LONGS | (run_length - 1));
      encoded_len++;
      যাবত্ (start < i)
      {
        int32_t মান_val = values.arrayZ[start++];
        *it++ = static_cast<বর্ণ> (মান_val >> 24);
        *it++ = static_cast<বর্ণ> (মান_val >> 16);
        *it++ = static_cast<বর্ণ> (মান_val >> 8);
        *it++ = static_cast<বর্ণ> (মান_val & 0xFF);

        encoded_len += 4;
      }
    }
    প্রত্যাগম encoded_len;
  }

  template <typename T>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  স্থির তার্কিক decompile (ধ্রুব HBUINT8 *&p /* IN/OUT */,
			 hb_vector_t<T> &values /* IN/OUT */,
			 ধ্রুব HBUINT8 *end,
			 তার্কিক consume_all = অসত্য,
			 unsigned start = 0)
  {
    unsigned i = 0;
    unsigned গুনতি = consume_all ? UINT_MAX : values.length;
    যদি (consume_all)
      values.alloc ((end - p) / 2);
    যাবত্ (i < গুনতি)
    {
      যদি (unlikely (p + 1 > end)) প্রত্যাগম consume_all;
      unsigned control = *p++;
      unsigned run_গুনতি = (control & মান_RUN_গুনতি_MASK) + 1;
      যদি (consume_all)
      {
        যদি (unlikely (!values.resize_dirty  (values.length + run_গুনতি)))
	  প্রত্যাগম অসত্য;
      }
      unsigned stop = i + run_গুনতি;
      যদি (unlikely (stop > গুনতি)) প্রত্যাগম অসত্য;

      unsigned skip = i < start ? hb_min (start - i, run_গুনতি) : 0;
      i += skip;

      যদি ((control & মানসমূহ_SIZE_MASK) == মানসমূহ_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * পরিমাণ (T));
	i = stop;
      }
      অন্যথা যদি ((control & মানসমূহ_SIZE_MASK) ==  মানসমূহ_ARE_শব্দগুলি)
      {
        যদি (unlikely (p + run_গুনতি * HBINT16::static_size > end)) প্রত্যাগম অসত্য;
	p += skip * HBINT16::static_size;
#ifndef HB_OPTIMIZE_SIZE
        প্রতি (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ধ্রুব HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 1] = * (ধ্রুব HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 2] = * (ধ্রুব HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 3] = * (ধ্রুব HBINT16 *) p;
	  p += HBINT16::static_size;
	}
#endif
        প্রতি (; i < stop; i++)
        {
          values.arrayZ[i] = * (ধ্রুব HBINT16 *) p;
          p += HBINT16::static_size;
        }
      }
      অন্যথা যদি ((control & মানসমূহ_SIZE_MASK) ==  মানসমূহ_ARE_LONGS)
      {
        যদি (unlikely (p + run_গুনতি * HBINT32::static_size > end)) প্রত্যাগম অসত্য;
	p += skip * HBINT32::static_size;
        প্রতি (; i < stop; i++)
        {
          values.arrayZ[i] = * (ধ্রুব HBINT32 *) p;
          p += HBINT32::static_size;
        }
      }
      অন্যথা যদি ((control & মানসমূহ_SIZE_MASK) ==  মানসমূহ_ARE_BYTES)
      {
        যদি (unlikely (p + run_গুনতি > end)) প্রত্যাগম অসত্য;
	p += skip * HBINT8::static_size;
#ifndef HB_OPTIMIZE_SIZE
	প্রতি (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ধ্রুব HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (ধ্রুব HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (ধ্রুব HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (ধ্রুব HBINT8 *) p++;
	}
#endif
        প্রতি (; i < stop; i++)
          values.arrayZ[i] = * (ধ্রুব HBINT8 *) p++;
      }
    }
    প্রত্যাগম সত্য;
  }

  স্থির তার্কিক skip (ধ্রুব unsigned বর্ণ *&p /* IN/OUT */,
		    ধ্রুব unsigned বর্ণ *end,
		    unsigned গুনতি)
  {
    unsigned seen = 0;
    যাবত্ (seen < গুনতি)
    {
      যদি (unlikely (p >= end)) প্রত্যাগম অসত্য;
      unsigned control = *p++;
      unsigned run_গুনতি = (control & মান_RUN_গুনতি_MASK) + 1;
      যদি (unlikely (run_গুনতি > গুনতি - seen)) প্রত্যাগম অসত্য;

      unsigned width = 0;
      বিকল্প (control & মানসমূহ_SIZE_MASK)
      {
	পক্ষ মানসমূহ_ARE_ZEROS: ভঙ্গ;
	পক্ষ মানসমূহ_ARE_BYTES: width = HBINT8::static_size; ভঙ্গ;
	পক্ষ মানসমূহ_ARE_শব্দগুলি: width = HBINT16::static_size; ভঙ্গ;
	পক্ষ মানসমূহ_ARE_LONGS: width = HBINT32::static_size; ভঙ্গ;
      }
      যদি (unlikely (unsigned (end - p) < run_গুনতি * width)) প্রত্যাগম অসত্য;
      p += run_গুনতি * width;
      seen += run_গুনতি;
    }
    প্রত্যাগম সত্য;
  }

  সংরচনা iter_t : hb_iter_with_fallback_t<iter_t, পূর্ণাঙ্ক>
  {
    iter_t (ধ্রুব unsigned বর্ণ *p_, unsigned len_)
	    : p (p_), endp (p_ + len_)
    { যদি (likely (ensure_run ())) read_মান (); }

    গুপ্ত:
    ধ্রুব unsigned বর্ণ *p;
    ধ্রুব unsigned বর্ণ * ধ্রুব endp;
    পূর্ণাঙ্ক current_মান = 0;
    signed run_গুনতি = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    তার্কিক ensure_run ()
    {
      যদি (likely (run_গুনতি > 0)) প্রত্যাগম সত্য;
      প্রত্যাগম _ensure_run ();
    }
    তার্কিক _ensure_run ()
    {
      যদি (unlikely (p >= endp))
      {
        run_গুনতি = 0;
        current_মান = 0;
	প্রত্যাগম অসত্য;
      }

      unsigned control = *p++;
      run_গুনতি = (control & মান_RUN_গুনতি_MASK) + 1;
      width = control & মানসমূহ_SIZE_MASK;
      বিকল্প (width)
      {
        পক্ষ মানসমূহ_ARE_ZEROS: width = 0; ভঙ্গ;
	পক্ষ মানসমূহ_ARE_BYTES: width = HBINT8::static_size;  ভঙ্গ;
	পক্ষ মানসমূহ_ARE_শব্দগুলি: width = HBINT16::static_size; ভঙ্গ;
	পক্ষ মানসমূহ_ARE_LONGS: width = HBINT32::static_size; ভঙ্গ;
	স্বভাব: প্রতিজ্ঞা (অসত্য);
      }

      যদি (unlikely (p + run_গুনতি * width > endp))
      {
	run_গুনতি = 0;
	current_মান = 0;
	প্রত্যাগম অসত্য;
      }

      প্রত্যাগম সত্য;
    }
    শূন্য read_মান ()
    {
      বিকল্প (width)
      {
        পক্ষ 0: current_মান = 0; ভঙ্গ;
	পক্ষ 1: current_মান = * (ধ্রুব HBINT8  *) p; ভঙ্গ;
	পক্ষ 2: current_মান = * (ধ্রুব HBINT16 *) p; ভঙ্গ;
	পক্ষ 4: current_মান = * (ধ্রুব HBINT32 *) p; ভঙ্গ;
      }
      p += width;
    }

    সার্বজনিক:

    প্রকারনির্দেশ পূর্ণাঙ্ক __item_t__;
    __item_t__ __item__ () ধ্রুব
    { প্রত্যাগম current_মান; }

    তার্কিক __more__ () ধ্রুব { প্রত্যাগম run_গুনতি || p < endp; }
    শূন্য __next__ ()
    {
      run_গুনতি--;
      যদি (unlikely (!ensure_run ()))
	প্রত্যাগম;
      read_মান ();
    }
    শূন্য __forward__ (unsigned n)
    {
      যদি (unlikely (!ensure_run ()))
	প্রত্যাগম;
      যাবত্ (n)
      {
	unsigned i = hb_min (n, (unsigned) run_গুনতি);
	run_গুনতি -= i;
	n -= i;
	p += (i - 1) * width;
	যদি (unlikely (!ensure_run ()))
	  প্রত্যাগম;
	read_মান ();
      }
    }
    তার্কিক operator != (ধ্রুব iter_t& o) ধ্রুব
    { প্রত্যাগম p != o.p || run_গুনতি != o.run_গুনতি; }
    iter_t __end__ () ধ্রুব
    {
      iter_t it (endp, 0);
      প্রত্যাগম it;
    }
  };

  সংরচনা fetcher_t
  {
    fetcher_t (ধ্রুব unsigned বর্ণ *p_, unsigned len_)
	      : p (p_), end (p_ + len_) {}

    গুপ্ত:
    ধ্রুব unsigned বর্ণ *p;
    ধ্রুব unsigned বর্ণ * ধ্রুব end;
    signed run_গুনতি = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    তার্কিক ensure_run ()
    {
      যদি (likely (run_গুনতি > 0)) প্রত্যাগম সত্য;
      প্রত্যাগম _ensure_run ();
    }

    তার্কিক _ensure_run ()
    {
      যদি (unlikely (p >= end))
      {
        run_গুনতি = 0;
	প্রত্যাগম অসত্য;
      }

      unsigned control = *p++;
      run_গুনতি = (control & মান_RUN_গুনতি_MASK) + 1;
      width = control & মানসমূহ_SIZE_MASK;
      বিকল্প (width)
      {
        পক্ষ মানসমূহ_ARE_ZEROS: width = 0; ভঙ্গ;
	পক্ষ মানসমূহ_ARE_BYTES: width = HBINT8::static_size;  ভঙ্গ;
	পক্ষ মানসমূহ_ARE_শব্দগুলি: width = HBINT16::static_size; ভঙ্গ;
	পক্ষ মানসমূহ_ARE_LONGS: width = HBINT32::static_size; ভঙ্গ;
	স্বভাব: প্রতিজ্ঞা (অসত্য);
      }

      যদি (unlikely (p + run_গুনতি * width > end))
      {
	run_গুনতি = 0;
	প্রত্যাগম অসত্য;
      }

      প্রত্যাগম সত্য;
    }

    সার্বজনিক:
    শূন্য skip (unsigned n)
    {
      যাবত্ (n)
      {
	যদি (unlikely (!ensure_run ()))
	  প্রত্যাগম;
	unsigned i = hb_min (n, (unsigned) run_গুনতি);
	run_গুনতি -= i;
	n -= i;
	p += i * width;
      }
    }

    গুপ্ত:
    template <তার্কিক scaled, typename T>
    শূন্য _add_to (hb_array_t<T> out, T scale = T (1))
    {
      unsigned n = out.length;
      T *arrayZ = out.arrayZ;

      প্রতি (unsigned i = 0; i < n;)
      {
	যদি (unlikely (!ensure_run ()))
	  ভঙ্গ;
	unsigned গুনতি = hb_min (n - i, (unsigned) run_গুনতি);
	বিকল্প (width)
	{
	  পক্ষ 0:
	  {
	    arrayZ += গুনতি;
	    ভঙ্গ;
	  }
	  পক্ষ 1:
	  {
	    ধ্রুব auto *pp = (ধ্রুব HBINT8 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    প্রতি (; j + 3 < গুনতি; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    প্রতি (; j < গুনতি; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ধ্রুব unsigned বর্ণ *) pp;
	  }
	  ভঙ্গ;
	  পক্ষ 2:
	  {
	    ধ্রুব auto *pp = (ধ্রুব HBINT16 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    প্রতি (; j + 3 < গুনতি; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    প্রতি (; j < গুনতি; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ধ্রুব unsigned বর্ণ *) pp;
	  }
	  ভঙ্গ;
	  পক্ষ 4:
	  {
	    ধ্রুব auto *pp = (ধ্রুব HBINT32 *) p;
	    প্রতি (unsigned j = 0; j < গুনতি; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ধ্রুব unsigned বর্ণ *) pp;
	  }
	  ভঙ্গ;
	}
	run_গুনতি -= গুনতি;
	i += গুনতি;
      }
    }

    সার্বজনিক:
    template <typename T>
    শূন্য add_to (hb_array_t<T> out, ভিন্ন scale = 1.0f)
    {
#ifndef HB_OPTIMIZE_SIZE
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      যদি (অসত্য && scale == 1.0f)
        _add_to<অসত্য> (out);
      অন্যথা
#endif
        _add_to<সত্য> (out, (T) scale);
    }
  };
};

সংরচনা TupleList : CFF2Index
{
  Tuple_মানসমূহ::iter_t operator [] (unsigned i) ধ্রুব
  {
    auto bytes = CFF2Index::operator [] (i);
    প্রত্যাগম Tuple_মানসমূহ::iter_t (bytes.arrayZ, bytes.length);
  }

  Tuple_মানসমূহ::fetcher_t fetcher (unsigned i) ধ্রুব
  {
    auto bytes = CFF2Index::operator [] (i);
    প্রত্যাগম Tuple_মানসমূহ::fetcher_t (bytes.arrayZ, bytes.length);
  }
};


// Alignment

template <unsigned পূর্ণাঙ্ক alignment>
সংরচনা Align
{
  size_t get_size (ধ্রুব শূন্য *base) ধ্রুব
  {
    unsigned offset = (ধ্রুব বর্ণ *) এতত্ - (ধ্রুব বর্ণ *) base;
    প্রত্যাগম (alignment - offset) & (alignment - 1);
  }

  সার্বজনিক:
  DEFINE_SIZE_MIN (0);
};



} /* namespace OT */


#endif /* HB_OPEN_TYPE_HH */
