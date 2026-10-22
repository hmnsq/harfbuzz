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

#ضمن "hb.hh"
#ضمن "hb-blob.hh"
#ضمن "hb-face.hh"
#ضمن "hb-machinery.hh"
#ضمن "hb-meta.hh"
#ضمن "hb-subset.hh"


نطاق OT {


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
template <منطقي BE,
	  typename Type,
	  unsigned صحيح Size = حجم (Type)>
بنية NumType
{
  تعريفنوع Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  تعريفنوع typename std::conditional<std::is_integral<Type>::value && حجم (Type) <= حجم(صحيح),
				     typename std::conditional<std::is_signed<Type>::value, signed, unsigned>::type,
				     Type>::type WideType;

  NumType () = افتراضي;
  explicit constexpr NumType (Type V) : v {V} {}
  NumType& operator = (Type V) { v = V; ܦܢܐ *هذا; }

  operator WideType () ثابت { ܦܢܐ v; }

  منطقي operator == (ثابت NumType &o) ثابت { ܦܢܐ (Type) v == (Type) o.v; }
  منطقي operator != (ثابت NumType &o) ثابت { ܦܢܐ !(*هذا == o); }

  NumType& operator += (WideType count) { *هذا = *هذا + count; ܦܢܐ *هذا; }
  NumType& operator -= (WideType count) { *هذا = *هذا - count; ܦܢܐ *هذا; }
  NumType& operator ++ () { *هذا += 1; ܦܢܐ *هذا; }
  NumType& operator -- () { *هذا -= 1; ܦܢܐ *هذا; }
  NumType operator ++ (صحيح) { NumType c (*هذا); ++*هذا; ܦܢܐ c; }
  NumType operator -- (صحيح) { NumType c (*هذا); --*هذا; ܦܢܐ c; }

  uint32_t hash () ثابت { ܦܢܐ hb_array ((ثابت حرف *) &v, حجم (v)).hash (); }
  HB_INTERNAL ساكن صحيح cmp (ثابت NumType *a, ثابت NumType *b)
  { ܦܢܐ b->cmp (*a); }
  HB_INTERNAL ساكن صحيح cmp (ثابت فراغ *a, ثابت فراغ *b)
  {
    NumType *pa = (NumType *) a;
    NumType *pb = (NumType *) b;

    ܦܢܐ pb->cmp (*pa);
  }
  template <typename Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  صحيح cmp (Type2 a) ثابت
  {
    Type b = v;
    ܦܢܐ (a > b) - (a < b);
  }
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }
  محمي:
  typename std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, Size>,
			    HBFloat<BE, Type, Size>>::type v;
  عام:
  DEFINE_SIZE_STATIC (Size);
};

تعريفنوع NumType<ܫܪܪܐ, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
تعريفنوع NumType<ܫܪܪܐ, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
تعريفنوع NumType<ܫܪܪܐ, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
تعريفنوع NumType<ܫܪܪܐ, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
تعريفنوع NumType<ܫܪܪܐ, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
تعريفنوع NumType<ܫܪܪܐ, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
تعريفنوع NumType<ܫܪܪܐ, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
تعريفنوع NumType<ܫܪܪܐ, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
تعريفنوع NumType<ܫܪܪܐ, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

تعريفنوع NumType<ܕܓܠܐ, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
تعريفنوع NumType<ܕܓܠܐ, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
تعريفنوع NumType<ܕܓܠܐ, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
تعريفنوع NumType<ܕܓܠܐ, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
تعريفنوع NumType<ܕܓܠܐ, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
تعريفنوع NumType<ܕܓܠܐ, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

تعريفنوع NumType<ܫܪܪܐ,  عائم>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
تعريفنوع NumType<ܫܪܪܐ,  مزدوج> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
تعريفنوع NumType<ܕܓܠܐ, عائم>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
تعريفنوع NumType<ܕܓܠܐ, مزدوج> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
بنية HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& operator = (uint16_t i ) { HBUINT16::operator= (i); ܦܢܐ *هذا; }
  عام:
  DEFINE_SIZE_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
بنية HBUINT32VAR
{
  size_t get_size () ثابت
  {
    unsigned b0 = v[0];
    ܐܢ (b0 < 0x80)
      ܦܢܐ 1;
    ܐܚܪܢܐ ܐܢ (b0 < 0xC0)
      ܦܢܐ 2;
    ܐܚܪܢܐ ܐܢ (b0 < 0xE0)
      ܦܢܐ 3;
    ܐܚܪܢܐ ܐܢ (b0 < 0xF0)
      ܦܢܐ 4;
    ܐܚܪܢܐ
      ܦܢܐ 5;
  }

  ساكن size_t get_size (uint32_t v)
  {
    ܐܢ (v < 0x80)
      ܦܢܐ 1;
    ܐܚܪܢܐ ܐܢ (v < 0x4000)
      ܦܢܐ 2;
    ܐܚܪܢܐ ܐܢ (v < 0x200000)
      ܦܢܐ 3;
    ܐܚܪܢܐ ܐܢ (v < 0x10000000)
      ܦܢܐ 4;
    ܐܚܪܢܐ
      ܦܢܐ 5;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_size ()));
  }

  operator uint32_t () ثابت
  {
    unsigned b0 = v[0];
    ܐܢ (b0 < 0x80)
      ܦܢܐ b0;
    ܐܚܪܢܐ ܐܢ (b0 < 0xC0)
      ܦܢܐ ((b0 & 0x3F) << 8) | v[1];
    ܐܚܪܢܐ ܐܢ (b0 < 0xE0)
      ܦܢܐ ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    ܐܚܪܢܐ ܐܢ (b0 < 0xF0)
      ܦܢܐ ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    ܐܚܪܢܐ
      ܦܢܐ (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  ساكن unsigned serialize_unsafe (unsigned حرف *buf, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned حرف *p = buf + len;
    لكل (unsigned i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    ܐܢ (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    ܦܢܐ len;
  }

  ساكن منطقي serialize (hb_serialize_context_t *c, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned حرف *buf = c->allocate_size<unsigned حرف> (len, ܕܓܠܐ);
    ܐܢ (unlikely (!buf))
      ܦܢܐ ܕܓܠܐ;

    serialize_unsafe (buf, v);
    ܦܢܐ ܫܪܪܐ;
  }

  محمي:
  unsigned حرف v[5];

  عام:
  DEFINE_SIZE_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
تعريفنوع HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
تعريفنوع HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
تعريفنوع HBUINT16 UFWORD;

template <typename Type, unsigned fraction_bits>
بنية HBFixed : Type
{
  ساكن constexpr عائم mult = 1.f / (1 << fraction_bits);
  static_assert (Type::static_size * 8 > fraction_bits, "");

  operator signed () ثابت = delete;
  operator unsigned () ثابت = delete;
  explicit operator عائم () ثابت { ܦܢܐ to_float (); }
  typename Type::type to_int () ثابت { ܦܢܐ Type::v; }
  فراغ set_int (typename Type::type i ) { Type::v = i; }
  عائم to_float (عائم offset = 0) ثابت  { ܦܢܐ ((int32_t) Type::v + offset) * mult; }
  فراغ set_float (عائم f) { Type::v = roundf (f / mult); }
  عام:
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
بنية LONGDATETIME
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }
  محمي:
  HBINT32 major;
  HBUINT32 minor;
  عام:
  DEFINE_SIZE_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
بنية Tag : HBUINT32
{
  Tag& operator = (hb_tag_t i) { HBUINT32::operator= (i); ܦܢܐ *هذا; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  operator ثابت حرف* () ثابت { ܦܢܐ reinterpret_cast<ثابت حرف *> (هذا); }
  operator حرف* ()             { ܦܢܐ reinterpret_cast<حرف *> (هذا); }
  عام:
  DEFINE_SIZE_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
بنية HBGlyphID16 : HBUINT16
{
  HBGlyphID16& operator = (uint16_t i) { HBUINT16::operator= (i); ܦܢܐ *هذا; }
};
بنية HBGlyphID24 : HBUINT24
{
  HBGlyphID24& operator = (uint32_t i) { HBUINT24::operator= (i); ܦܢܐ *هذا; }
};

/* Script/language-system/feature index */
بنية Index : HBUINT16 {
  ساكن constexpr unsigned NOT_FOUND_INDEX = 0xFFFFu;
  Index& operator = (uint16_t i) { HBUINT16::operator= (i); ܦܢܐ *هذا; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, Index);

تعريفنوع Index NameID;

بنية VarIdx : HBUINT32 {
  ساكن constexpr unsigned NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_INDEX, "");
  VarIdx& operator = (uint32_t i) { HBUINT32::operator= (i); ܦܢܐ *هذا; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, VarIdx);

/* Offset, Null offset = 0 */
template <typename Type, منطقي has_null=ܫܪܪܐ>
بنية Offset : Type
{
  Offset& operator = (typename Type::type i) { Type::operator= (i); ܦܢܐ *هذا; }

  تعريفنوع Type type;

  منطقي is_null () ثابت { ܦܢܐ has_null && 0 == *هذا; }

  عام:
  DEFINE_SIZE_STATIC (حجم (Type));
};

تعريفنوع Offset<HBUINT16> Offset16;
تعريفنوع Offset<HBUINT24> Offset24;
تعريفنوع Offset<HBUINT32> Offset32;


/* CheckSum */
بنية CheckSum : HBUINT32
{
  CheckSum& operator = (uint32_t i) { HBUINT32::operator= (i); ܦܢܐ *هذا; }

  /* This is reference implementation from the spec. */
  ساكن uint32_t CalcTableChecksum (ثابت HBUINT32 *Table, uint32_t Length)
  {
    uint32_t Sum = 0L;
    أكد (0 == (Length & 3));
    ثابت HBUINT32 *EndPtr = Table + Length / HBUINT32::static_size;

    ܟܡܐ (Table < EndPtr)
      Sum += *Table++;
    ܦܢܐ Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  فراغ set_for_data (ثابت فراغ *data, unsigned صحيح length)
  { *هذا = CalcTableChecksum ((ثابت HBUINT32 *) data, length); }

  عام:
  DEFINE_SIZE_STATIC (4);
};


/*
 * Version Numbers
 */

template <typename FixedType=HBUINT16>
بنية FixedVersion
{
  uint32_t to_int () ثابت { ܦܢܐ (major << (حجم (FixedType) * 8)) + minor; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  FixedType major;
  FixedType minor;
  عام:
  DEFINE_SIZE_STATIC (2 * حجم (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

template <typename Type, منطقي has_null>
بنية _hb_has_null
{
  ساكن ثابت Type *get_null () { ܦܢܐ nullptr; }
  ساكن Type *get_crap ()       { ܦܢܐ nullptr; }
};
template <typename Type>
بنية _hb_has_null<Type, ܫܪܪܐ>
{
  ساكن ثابت Type *get_null () { ܦܢܐ &Null (Type); }
  ساكن       Type *get_crap () { ܦܢܐ &Crap (Type); }
};

template <typename Type, typename OffsetType, typename BaseType=فراغ, منطقي has_null=ܫܪܪܐ>
بنية OffsetTo : Offset<OffsetType, has_null>
{
  using target_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == ܕܓܠܐ ||
		 (hb_has_null_size (Type) || !hb_has_min_size (Type)), "");

  HB_DELETE_COPY_ASSIGN (OffsetTo);
  OffsetTo () = افتراضي;

  OffsetTo& operator = (typename OffsetType::type i) { OffsetType::operator= (i); ܦܢܐ *هذا; }

  ثابت Type& operator () (ثابت فراغ *base) ثابت
  {
    ܐܢ (unlikely (هذا->is_null ())) ܦܢܐ *_hb_has_null<Type, has_null>::get_null ();
    ܦܢܐ StructAtOffset<ثابت Type> (base, *هذا);
  }
  Type& operator () (فراغ *base) ثابت
  {
    ܐܢ (unlikely (هذا->is_null ())) ܦܢܐ *_hb_has_null<Type, has_null>::get_crap ();
    ܦܢܐ StructAtOffset<Type> (base, *هذا);
  }

  template <typename Base,
	    hb_enable_if (hb_is_convertible (ثابت Base, ثابت BaseType *))>
  friend ثابت Type& operator + (ثابت Base &base, ثابت OffsetTo &offset) { ܦܢܐ offset ((ثابت فراغ *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (ثابت Base, ثابت BaseType *))>
  friend ثابت Type& operator + (ثابت OffsetTo &offset, ثابت Base &base) { ܦܢܐ offset ((ثابت فراغ *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (Base &&base, OffsetTo &offset) { ܦܢܐ offset ((فراغ *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (OffsetTo &offset, Base &&base) { ܦܢܐ offset ((فراغ *) base); }


  template <typename Base, typename ...Ts>
  منطقي serialize_subset (hb_subset_context_t *c, ثابت OffsetTo& src,
			 ثابت Base *src_base, Ts&&... ds)
  {
    *هذا = 0;
    ܐܢ (src.is_null ())
      ܦܢܐ ܕܓܠܐ;

    auto *s = c->serializer;

    s->push ();

    منطقي ret = c->dispatch (src_base+src, std::forward<Ts> (ds)...);

    ܐܢ (ret || !has_null)
      s->add_link (*هذا, s->pop_pack ());
    ܐܚܪܢܐ
      s->pop_discard ();

    ܦܢܐ ret;
  }


  template <typename ...Ts>
  منطقي serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *هذا = 0;

    Type* obj = c->push<Type> ();
    منطقي ret = obj->serialize (c, std::forward<Ts> (ds)...);

    ܐܢ (ret)
      c->add_link (*هذا, c->pop_pack ());
    ܐܚܪܢܐ
      c->pop_discard ();

    ܦܢܐ ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  template <typename ...Ts>
  منطقي serialize_copy (hb_serialize_context_t *c, ثابت OffsetTo& src,
		       ثابت فراغ *src_base, unsigned dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *هذا = 0;
    ܐܢ (src.is_null ())
      ܦܢܐ ܕܓܠܐ;

    c->push ();

    منطقي ret = c->copy (src_base+src, std::forward<Ts> (ds)...);

    c->add_link (*هذا, c->pop_pack (), whence, dst_bias);

    ܦܢܐ ret;
  }

  منطقي serialize_copy (hb_serialize_context_t *c, ثابت OffsetTo& src,
		       ثابت فراغ *src_base, unsigned dst_bias = 0)
  { ܦܢܐ serialize_copy (c, src, src_base, dst_bias, hb_serialize_context_t::Head); }

  منطقي sanitize_shallow (hb_sanitize_context_t *c, ثابت BaseType *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (unlikely (!c->check_struct (هذا))) return_trace (ܕܓܠܐ);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    ܐܢ (unlikely ((ثابت حرف *) base + (unsigned) *هذا < (ثابت حرف *) base)) return_trace (ܕܓܠܐ);
    return_trace (ܫܪܪܐ);
  }

  template <typename ...Ts>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  منطقي sanitize (hb_sanitize_context_t *c, ثابت BaseType *base, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (sanitize_shallow (c, base) &&
		  hb_barrier () &&
		  (هذا->is_null () ||
		   c->dispatch (StructAtOffset<Type> (base, *هذا), std::forward<Ts> (ds)...)));
  }

  DEFINE_SIZE_STATIC (حجم (OffsetType));
};
/* Partial specializations. */
template <typename Type, typename BaseType=فراغ, منطقي has_null=ܫܪܪܐ> using Offset16To = OffsetTo<Type, HBUINT16, BaseType, has_null>;
template <typename Type, typename BaseType=فراغ, منطقي has_null=ܫܪܪܐ> using Offset24To = OffsetTo<Type, HBUINT24, BaseType, has_null>;
template <typename Type, typename BaseType=فراغ, منطقي has_null=ܫܪܪܐ> using Offset32To = OffsetTo<Type, HBUINT32, BaseType, has_null>;

template <typename Type, typename OffsetType, typename BaseType=فراغ> using NNOffsetTo = OffsetTo<Type, OffsetType, BaseType, ܕܓܠܐ>;
template <typename Type, typename BaseType=فراغ> using NNOffset16To = Offset16To<Type, BaseType, ܕܓܠܐ>;
template <typename Type, typename BaseType=فراغ> using NNOffset24To = Offset24To<Type, BaseType, ܕܓܠܐ>;
template <typename Type, typename BaseType=فراغ> using NNOffset32To = Offset32To<Type, BaseType, ܕܓܠܐ>;


/*
 * Array Types
 */

template <typename Type>
بنية UnsizedArrayOf
{
  تعريفنوع Type item_t;
  ساكن constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (UnsizedArrayOf);

  ثابت Type& operator [] (unsigned صحيح i) ثابت
  {
    ܦܢܐ arrayZ[i];
  }
  Type& operator [] (unsigned صحيح i)
  {
    ܦܢܐ arrayZ[i];
  }

  ساكن size_t get_size (unsigned صحيح len)
  { ܦܢܐ hb_unsigned_mul_saturate (len, Type::static_size); }

  template <typename T> operator T * () { ܦܢܐ arrayZ; }
  template <typename T> operator ثابت T * () ثابت { ܦܢܐ arrayZ; }
  hb_array_t<Type> as_array (unsigned صحيح len)
  { ܦܢܐ hb_array (arrayZ, len); }
  hb_array_t<ثابت Type> as_array (unsigned صحيح len) ثابت
  { ܦܢܐ hb_array (arrayZ, len); }

  template <typename T>
  Type &lsearch (unsigned صحيح len, ثابت T &x, Type &not_found = Crap (Type))
  { ܦܢܐ *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  ثابت Type &lsearch (unsigned صحيح len, ثابت T &x, ثابت Type &not_found = Null (Type)) ثابت
  { ܦܢܐ *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  منطقي lfind (unsigned صحيح len, ثابت T &x, unsigned صحيح *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned صحيح to_store = (unsigned صحيح) -1) ثابت
  { ܦܢܐ as_array (len).lfind (x, i, not_found, to_store); }

  فراغ qsort (unsigned صحيح len, unsigned صحيح start = 0, unsigned صحيح end = (unsigned صحيح) -1)
  { as_array (len).qsort (start, end); }

  منطقي serialize (hb_serialize_context_t *c, unsigned صحيح items_len, منطقي clear = ܫܪܪܐ)
  {
    TRACE_SERIALIZE (هذا);
    ܐܢ (unlikely (!c->extend_size (هذا, get_size (items_len), clear))) return_trace (ܕܓܠܐ);
    return_trace (ܫܪܪܐ);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  منطقي serialize (hb_serialize_context_t *c, Iterator items)
  {
    TRACE_SERIALIZE (هذا);
    unsigned count = hb_len (items);
    ܐܢ (unlikely (!serialize (c, count, ܕܓܠܐ))) return_trace (ܕܓܠܐ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    لكل (unsigned i = 0; i < count; i++, ++items)
      arrayZ[i] = *items;
    return_trace (ܫܪܪܐ);
  }

  UnsizedArrayOf* copy (hb_serialize_context_t *c, unsigned count) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *out = c->start_embed (هذا);
    ܐܢ (unlikely (!as_array (count).copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح count, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (unlikely (!sanitize_shallow (c, count))) return_trace (ܕܓܠܐ);
    ܐܢ (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ܫܪܪܐ);
    hb_barrier ();
    لكل (unsigned صحيح i = 0; i < count; i++)
      ܐܢ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ܕܓܠܐ);
    return_trace (ܫܪܪܐ);
  }

  منطقي sanitize_shallow (hb_sanitize_context_t *c, unsigned صحيح count) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_array (arrayZ, count));
  }

  عام:
  Type		arrayZ[HB_VAR_ARRAY];
  عام:
  DEFINE_SIZE_UNBOUNDED (0);
};

/* Unsized array of offset's */
template <typename Type, typename OffsetType, typename BaseType=فراغ, منطقي has_null=ܫܪܪܐ>
using UnsizedArray16OfOffsetTo = UnsizedArrayOf<OffsetTo<Type, OffsetType, BaseType, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
template <typename Type, typename OffsetType, typename BaseType=فراغ, منطقي has_null=ܫܪܪܐ>
بنية UnsizedListOfOffset16To : UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
{
  ثابت Type& operator [] (صحيح i_) ثابت
  {
    unsigned صحيح i = (unsigned صحيح) i_;
    ثابت OffsetTo<Type, OffsetType, BaseType, has_null> *p = &هذا->arrayZ[i];
    ܐܢ (unlikely ((ثابت فراغ *) p < (ثابت فراغ *) هذا->arrayZ)) ܦܢܐ Null (Type); /* Overflowed. */
    hb_barrier ();
    ܦܢܐ هذا+*p;
  }
  Type& operator [] (صحيح i_)
  {
    unsigned صحيح i = (unsigned صحيح) i_;
    ثابت OffsetTo<Type, OffsetType, BaseType, has_null> *p = &هذا->arrayZ[i];
    ܐܢ (unlikely ((ثابت فراغ *) p < (ثابت فراغ *) هذا->arrayZ)) ܦܢܐ Crap (Type); /* Overflowed. */
    hb_barrier ();
    ܦܢܐ هذا+*p;
  }

  template <typename ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح count, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace ((UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
		   ::sanitize (c, count, هذا, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type>
بنية SortedUnsizedArrayOf : UnsizedArrayOf<Type>
{
  hb_sorted_array_t<Type> as_array (unsigned صحيح len)
  { ܦܢܐ hb_sorted_array (هذا->arrayZ, len); }
  hb_sorted_array_t<ثابت Type> as_array (unsigned صحيح len) ثابت
  { ܦܢܐ hb_sorted_array (هذا->arrayZ, len); }
  operator hb_sorted_array_t<Type> ()             { ܦܢܐ as_array (); }
  operator hb_sorted_array_t<ثابت Type> () ثابت { ܦܢܐ as_array (); }

  template <typename T>
  Type &bsearch (unsigned صحيح len, ثابت T &x, Type &not_found = Crap (Type))
  { ܦܢܐ *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  ثابت Type &bsearch (unsigned صحيح len, ثابت T &x, ثابت Type &not_found = Null (Type)) ثابت
  { ܦܢܐ *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  منطقي bfind (unsigned صحيح len, ثابت T &x, unsigned صحيح *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned صحيح to_store = (unsigned صحيح) -1) ثابت
  { ܦܢܐ as_array (len).bfind (x, i, not_found, to_store); }
};


/* An array with a number of elements. */
template <typename Type, typename LenType>
بنية ArrayOf
{
  تعريفنوع Type item_t;
  ساكن constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOf);

  ثابت Type& operator [] (صحيح i_) ثابت
  {
    unsigned صحيح i = (unsigned صحيح) i_;
    ܐܢ (unlikely (i >= len)) ܦܢܐ Null (Type);
    hb_barrier ();
    ܦܢܐ arrayZ[i];
  }
  Type& operator [] (صحيح i_)
  {
    unsigned صحيح i = (unsigned صحيح) i_;
    ܐܢ (unlikely (i >= len)) ܦܢܐ Crap (Type);
    hb_barrier ();
    ܦܢܐ arrayZ[i];
  }

  size_t get_size () ثابت
  { ܦܢܐ hb_unsigned_mul_add_saturate (len, Type::static_size, len.static_size); }

  explicit operator منطقي () ثابت { ܦܢܐ len; }

  فراغ pop () { len--; }

  hb_array_t<      Type> as_array ()       { ܦܢܐ hb_array (arrayZ, len); }
  hb_array_t<ثابت Type> as_array () ثابت { ܦܢܐ hb_array (arrayZ, len); }

  /* Iterator. */
  تعريفنوع hb_array_t<ثابت Type>   iter_t;
  تعريفنوع hb_array_t<      Type> writer_t;
    iter_t   iter () ثابت { ܦܢܐ as_array (); }
  writer_t writer ()       { ܦܢܐ as_array (); }
  operator   iter_t () ثابت { ܦܢܐ   iter (); }
  operator writer_t ()       { ܦܢܐ writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { ܦܢܐ arrayZ; }
  ثابت Type *end () ثابت { ܦܢܐ arrayZ + len; }

  template <typename T>
  Type &lsearch (ثابت T &x, Type &not_found = Crap (Type))
  { ܦܢܐ *as_array ().lsearch (x, &not_found); }
  template <typename T>
  ثابت Type &lsearch (ثابت T &x, ثابت Type &not_found = Null (Type)) ثابت
  { ܦܢܐ *as_array ().lsearch (x, &not_found); }
  template <typename T>
  منطقي lfind (ثابت T &x, unsigned صحيح *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned صحيح to_store = (unsigned صحيح) -1) ثابت
  { ܦܢܐ as_array ().lfind (x, i, not_found, to_store); }

  فراغ qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, unsigned items_len, منطقي clear = ܫܪܪܐ)
  {
    TRACE_SERIALIZE (هذا);
    ܐܢ (unlikely (!c->extend_min (هذا))) return_trace (ܕܓܠܐ);
    c->check_assign (len, items_len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    ܐܢ (unlikely (!c->extend_size (هذا, get_size (), clear))) return_trace (ܕܓܠܐ);
    return_trace (ܫܪܪܐ);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, Iterator items)
  {
    TRACE_SERIALIZE (هذا);
    unsigned count = hb_len (items);
    ܐܢ (unlikely (!serialize (c, count, ܕܓܠܐ))) return_trace (ܕܓܠܐ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    لكل (unsigned i = 0; i < count; i++, ++items)
      arrayZ[i] = *items;
    return_trace (ܫܪܪܐ);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (هذا);
    len++;
    ܐܢ (unlikely (!len || !c->extend (هذا)))
    {
      len--;
      return_trace (nullptr);
    }
    return_trace (&arrayZ[len - 1]);
  }

  ArrayOf* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *out = c->start_embed (هذا);
    ܐܢ (unlikely (!c->extend_min (out))) return_trace (nullptr);
    c->check_assign (out->len, len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    ܐܢ (unlikely (!as_array ().copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (unlikely (!sanitize_shallow (c))) return_trace (ܕܓܠܐ);
    ܐܢ (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ܫܪܪܐ);
    hb_barrier ();
    unsigned صحيح count = len;
    لكل (unsigned صحيح i = 0; i < count; i++)
      ܐܢ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ܕܓܠܐ);
    return_trace (ܫܪܪܐ);
  }

  منطقي sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (len.sanitize (c) &&
		  hb_barrier () &&
		  c->check_array_sized (arrayZ, len, حجم (LenType)));
  }

  عام:
  LenType	len;
  Type		arrayZ[HB_VAR_ARRAY];
  عام:
  DEFINE_SIZE_ARRAY (حجم (LenType), arrayZ);
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
بنية List16OfOffsetTo : ArrayOf<OffsetTo<Type, OffsetType>, HBUINT16>
{
  ثابت Type& operator [] (صحيح i_) ثابت
  {
    unsigned صحيح i = (unsigned صحيح) i_;
    ܐܢ (unlikely (i >= هذا->len)) ܦܢܐ Null (Type);
    hb_barrier ();
    ܦܢܐ هذا+هذا->arrayZ[i];
  }
  ثابت Type& operator [] (صحيح i_)
  {
    unsigned صحيح i = (unsigned صحيح) i_;
    ܐܢ (unlikely (i >= هذا->len)) ܦܢܐ Crap (Type);
    hb_barrier ();
    ܦܢܐ هذا+هذا->arrayZ[i];
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    بنية List16OfOffsetTo *out = c->serializer->embed (*هذا);
    ܐܢ (unlikely (!out)) return_trace (ܕܓܠܐ);
    unsigned صحيح count = هذا->len;
    لكل (unsigned صحيح i = 0; i < count; i++)
      out->arrayZ[i].serialize_subset (c, هذا->arrayZ[i], هذا, out);
    return_trace (ܫܪܪܐ);
  }

  template <typename ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace ((Array16Of<OffsetTo<Type, OffsetType>>::sanitize (c, هذا, std::forward<Ts> (ds)...)));
  }
};

template <typename Type>
using List16OfOffset16To = List16OfOffsetTo<Type, HBUINT16>;

/* An array starting at second element. */
template <typename Type, typename LenType>
بنية HeadlessArrayOf
{
  ساكن constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (HeadlessArrayOf);

  ثابت Type& operator [] (صحيح i_) ثابت
  {
    unsigned صحيح i = (unsigned صحيح) i_;
    ܐܢ (unlikely (i >= lenP1 || !i)) ܦܢܐ Null (Type);
    hb_barrier ();
    ܦܢܐ arrayZ[i-1];
  }
  Type& operator [] (صحيح i_)
  {
    unsigned صحيح i = (unsigned صحيح) i_;
    ܐܢ (unlikely (i >= lenP1 || !i)) ܦܢܐ Crap (Type);
    hb_barrier ();
    ܦܢܐ arrayZ[i-1];
  }
  size_t get_size () ثابت
  { ܦܢܐ hb_unsigned_mul_add_saturate (get_length (), Type::static_size, lenP1.static_size); }

  unsigned get_length () ثابت { ܦܢܐ lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { ܦܢܐ hb_array (arrayZ, get_length ()); }
  hb_array_t<ثابت Type> as_array () ثابت { ܦܢܐ hb_array (arrayZ, get_length ()); }

  /* Iterator. */
  تعريفنوع hb_array_t<ثابت Type>   iter_t;
  تعريفنوع hb_array_t<      Type> writer_t;
    iter_t   iter () ثابت { ܦܢܐ as_array (); }
  writer_t writer ()       { ܦܢܐ as_array (); }
  operator   iter_t () ثابت { ܦܢܐ   iter (); }
  operator writer_t ()       { ܦܢܐ writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { ܦܢܐ arrayZ; }
  ثابت Type *end () ثابت { ܦܢܐ arrayZ + get_length (); }

  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, unsigned صحيح items_len, منطقي clear = ܫܪܪܐ)
  {
    TRACE_SERIALIZE (هذا);
    ܐܢ (unlikely (!c->extend_min (هذا))) return_trace (ܕܓܠܐ);
    c->check_assign (lenP1, items_len + 1, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    ܐܢ (unlikely (!c->extend_size (هذا, get_size (), clear))) return_trace (ܕܓܠܐ);
    return_trace (ܫܪܪܐ);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, Iterator items)
  {
    TRACE_SERIALIZE (هذا);
    unsigned count = hb_len (items);
    ܐܢ (unlikely (!serialize (c, count, ܕܓܠܐ))) return_trace (ܕܓܠܐ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    لكل (unsigned i = 0; i < count; i++, ++items)
      arrayZ[i] = *items;
    return_trace (ܫܪܪܐ);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (unlikely (!sanitize_shallow (c))) return_trace (ܕܓܠܐ);
    ܐܢ (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ܫܪܪܐ);
    hb_barrier ();
    unsigned صحيح count = get_length ();
    لكل (unsigned صحيح i = 0; i < count; i++)
      ܐܢ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ܕܓܠܐ);
    return_trace (ܫܪܪܐ);
  }

  خاص:
  منطقي sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (lenP1.sanitize (c) &&
		  hb_barrier () &&
		  (!lenP1 || c->check_array_sized (arrayZ, lenP1 - 1, حجم (LenType))));
  }

  عام:
  LenType	lenP1;
  Type		arrayZ[HB_VAR_ARRAY];
  عام:
  DEFINE_SIZE_ARRAY (حجم (LenType), arrayZ);
};
template <typename Type> using HeadlessArray16Of = HeadlessArrayOf<Type, HBUINT16>;

/* An array storing length-1. */
template <typename Type, typename LenType=HBUINT16>
بنية ArrayOfM1
{
  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOfM1);

  ثابت Type& operator [] (صحيح i_) ثابت
  {
    unsigned صحيح i = (unsigned صحيح) i_;
    ܐܢ (unlikely (i > lenM1)) ܦܢܐ Null (Type);
    hb_barrier ();
    ܦܢܐ arrayZ[i];
  }
  Type& operator [] (صحيح i_)
  {
    unsigned صحيح i = (unsigned صحيح) i_;
    ܐܢ (unlikely (i > lenM1)) ܦܢܐ Crap (Type);
    hb_barrier ();
    ܦܢܐ arrayZ[i];
  }
  size_t get_size () ثابت
  { ܦܢܐ hb_unsigned_mul_add_saturate (lenM1 + 1, Type::static_size, lenM1.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (unlikely (!sanitize_shallow (c))) return_trace (ܕܓܠܐ);
    ܐܢ (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ܫܪܪܐ);
    hb_barrier ();
    unsigned صحيح count = lenM1 + 1;
    لكل (unsigned صحيح i = 0; i < count; i++)
      ܐܢ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ܕܓܠܐ);
    return_trace (ܫܪܪܐ);
  }

  خاص:
  منطقي sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (lenM1.sanitize (c) &&
		  hb_barrier () &&
		  (c->check_array_sized (arrayZ, lenM1 + 1, حجم (LenType))));
  }

  عام:
  LenType	lenM1;
  Type		arrayZ[HB_VAR_ARRAY];
  عام:
  DEFINE_SIZE_ARRAY (حجم (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type, typename LenType>
بنية SortedArrayOf : ArrayOf<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { ܦܢܐ hb_sorted_array (هذا->arrayZ, هذا->len); }
  hb_sorted_array_t<ثابت Type> as_array () ثابت { ܦܢܐ hb_sorted_array (هذا->arrayZ, هذا->len); }

  /* Iterator. */
  تعريفنوع hb_sorted_array_t<ثابت Type>   iter_t;
  تعريفنوع hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () ثابت { ܦܢܐ as_array (); }
  writer_t writer ()       { ܦܢܐ as_array (); }
  operator   iter_t () ثابت { ܦܢܐ   iter (); }
  operator writer_t ()       { ܦܢܐ writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { ܦܢܐ هذا->arrayZ; }
  ثابت Type *end () ثابت { ܦܢܐ هذا->arrayZ + هذا->len; }

  منطقي serialize (hb_serialize_context_t *c, unsigned صحيح items_len)
  {
    TRACE_SERIALIZE (هذا);
    منطقي ret = ArrayOf<Type, LenType>::serialize (c, items_len);
    return_trace (ret);
  }
  template <typename Iterator,
	    hb_requires (hb_is_sorted_source_of (Iterator, Type))>
  منطقي serialize (hb_serialize_context_t *c, Iterator items)
  {
    TRACE_SERIALIZE (هذا);
    منطقي ret = ArrayOf<Type, LenType>::serialize (c, items);
    return_trace (ret);
  }

  SortedArrayOf* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    SortedArrayOf* out = reinterpret_cast<SortedArrayOf *> (ArrayOf<Type, LenType>::copy (c));
    return_trace (out);
  }

  template <typename T>
  Type &bsearch (ثابت T &x, Type &not_found = Crap (Type))
  { ܦܢܐ *as_array ().bsearch (x, &not_found); }
  template <typename T>
  ثابت Type &bsearch (ثابت T &x, ثابت Type &not_found = Null (Type)) ثابت
  { ܦܢܐ *as_array ().bsearch (x, &not_found); }
  template <typename T>
  منطقي bfind (ثابت T &x, unsigned صحيح *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned صحيح to_store = (unsigned صحيح) -1) ثابت
  { ܦܢܐ as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type> using SortedArray16Of = SortedArrayOf<Type, HBUINT16>;
template <typename Type> using SortedArray24Of = SortedArrayOf<Type, HBUINT24>;
template <typename Type> using SortedArray32Of = SortedArrayOf<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

template <typename LenType=HBUINT16>
بنية BinSearchHeader
{
  operator uint32_t () ثابت { ܦܢܐ len; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  BinSearchHeader& operator = (unsigned صحيح v)
  {
    len = v;
    أكد (len == v);
    entrySelector = hb_max (1u, hb_bit_storage (v)) - 1;
    searchRange = 16 * (1u << entrySelector);
    rangeShift = v * 16 > searchRange
		 ? 16 * v - searchRange
		 : 0;
    ܦܢܐ *هذا;
  }

  محمي:
  LenType	len;
  LenType	searchRange;
  LenType	entrySelector;
  LenType	rangeShift;

  عام:
  DEFINE_SIZE_STATIC (8);
};

template <typename Type, typename LenType=HBUINT16>
using BinSearchArrayOf = SortedArrayOf<Type, BinSearchHeader<LenType>>;


بنية VarSizedBinSearchHeader
{

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16	unitSize;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	searchRange;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	entrySelector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	rangeShift;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  عام:
  DEFINE_SIZE_STATIC (10);
};

template <typename Type>
بنية VarSizedBinSearchArrayOf
{
  ساكن constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (VarSizedBinSearchArrayOf);

  منطقي last_is_terminator () ثابت
  {
    ܐܢ (unlikely (!header.nUnits)) ܦܢܐ ܕܓܠܐ;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    ثابت HBUINT16 *words = &StructAtOffset<HBUINT16> (&bytesZ, (header.nUnits - 1) * header.unitSize);
    unsigned صحيح count = Type::TerminationWordCount;
    لكل (unsigned صحيح i = 0; i < count; i++)
      ܐܢ (words[i] != 0xFFFFu)
	ܦܢܐ ܕܓܠܐ;
    ܦܢܐ ܫܪܪܐ;
  }

  ثابت Type& operator [] (صحيح i_) ثابت
  {
    unsigned صحيح i = (unsigned صحيح) i_;
    ܐܢ (unlikely (i >= get_length ())) ܦܢܐ Null (Type);
    hb_barrier ();
    ܦܢܐ StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  Type& operator [] (صحيح i_)
  {
    unsigned صحيح i = (unsigned صحيح) i_;
    ܐܢ (unlikely (i >= get_length ())) ܦܢܐ Crap (Type);
    hb_barrier ();
    ܦܢܐ StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  unsigned صحيح get_length () ثابت
  { ܦܢܐ header.nUnits - last_is_terminator (); }
  size_t get_size () ثابت
  { ܦܢܐ hb_unsigned_mul_add_saturate (header.nUnits, header.unitSize, header.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (unlikely (!sanitize_shallow (c))) return_trace (ܕܓܠܐ);
    ܐܢ (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ܫܪܪܐ);
    hb_barrier ();
    unsigned صحيح count = get_length ();
    لكل (unsigned صحيح i = 0; i < count; i++)
      ܐܢ (unlikely (!(*هذا)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (ܕܓܠܐ);
    return_trace (ܫܪܪܐ);
  }

  template <typename T>
  ثابت Type *bsearch (ثابت T &key) ثابت
  {
    unsigned pos;
    ܦܢܐ hb_bsearch_impl (&pos,
			    key,
			    (ثابت فراغ *) bytesZ,
			    get_length (),
			    header.unitSize,
			    _hb_cmp_method<T, Type>)
	   ? (ثابت Type *) (((ثابت حرف *) &bytesZ) + (pos * header.unitSize))
	   : nullptr;
  }

  خاص:
  منطقي sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (header.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_size <= header.unitSize &&
		  c->check_range (bytesZ.arrayZ,
				  header.nUnits,
				  header.unitSize));
  }

  محمي:
  VarSizedBinSearchHeader	header;
  UnsizedArrayOf<HBUINT8>	bytesZ;
  عام:
  DEFINE_SIZE_ARRAY (10, bytesZ);
};


/* CFF INDEX */

template <typename COUNT>
بنية CFFIndex
{
  unsigned صحيح offset_array_size () ثابت
  { ܦܢܐ offSize * (count + 1); }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Iterable &iterable,
		  ثابت unsigned *p_data_size = nullptr,
                  unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (هذا);
    unsigned data_size;
    ܐܢ (p_data_size)
      data_size = *p_data_size;
    ܐܚܪܢܐ
      total_size (iterable, &data_size);

    auto it = hb_iter (iterable);
    ܐܢ (unlikely (!serialize_header (c, +it, data_size, min_off_size))) return_trace (ܕܓܠܐ);
    unsigned حرف *ret = c->allocate_size<unsigned حرف> (data_size, ܕܓܠܐ);
    ܐܢ (unlikely (!ret)) return_trace (ܕܓܠܐ);
    unsigned remaining = data_size;
    لكل (ثابت auto &_ : +it)
    {
      unsigned len = _.length;

      ܐܢ (!len)
	استمر;

      ܐܢ (unlikely (len > remaining)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (ܕܓܠܐ, HB_SERIALIZE_ERROR_INT_OVERFLOW));
      }

      remaining -= len;

      ܐܢ (len <= 1)
      {
	*ret++ = *_.arrayZ;
	استمر;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (ܫܪܪܐ);
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_header (hb_serialize_context_t *c,
			 Iterator it,
			 unsigned data_size,
                         unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (هذا);

    unsigned off_size = (hb_bit_storage (data_size + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    /* serialize CFFIndex header */
    ܐܢ (unlikely (!c->extend_min (هذا))) return_trace (ܕܓܠܐ);
    هذا->count = hb_len (it);
    ܐܢ (!هذا->count) return_trace (ܫܪܪܐ);
    ܐܢ (unlikely (!c->extend (هذا->offSize))) return_trace (ܕܓܠܐ);
    هذا->offSize = off_size;
    ܐܢ (unlikely (!c->allocate_size<HBUINT8> (off_size * (هذا->count + 1), ܕܓܠܐ)))
      return_trace (ܕܓܠܐ);

    /* serialize indices */
    unsigned صحيح offset = 1;
    ܐܢ (HB_OPTIMIZE_SIZE_VAL)
    {
      unsigned صحيح i = 0;
      لكل (ثابت auto &_ : +it)
      {
	set_offset_at (i++, offset);
	offset += hb_len_of (_);
      }
      set_offset_at (i, offset);
    }
    ܐܚܪܢܐ
      بدل (off_size)
      {
	حالة 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  لكل (ثابت auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	اكسر;
	حالة 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  لكل (ثابت auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	اكسر;
	حالة 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  لكل (ثابت auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	اكسر;
	حالة 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  لكل (ثابت auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	اكسر;
	افتراضي:
	اكسر;
      }

    أكد (offset == data_size + 1);
    return_trace (ܫܪܪܐ);
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  ساكن unsigned total_size (ثابت Iterable &iterable, unsigned *data_size = nullptr, unsigned min_off_size = 0)
  {
    auto it = + hb_iter (iterable);
    ܐܢ (!it)
    {
      ܐܢ (data_size) *data_size = 0;
      ܦܢܐ min_size;
    }

    unsigned total = 0;
    لكل (ثابت auto &_ : +it)
      total += hb_len_of (_);

    ܐܢ (data_size) *data_size = total;

    unsigned off_size = (hb_bit_storage (total + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    ܦܢܐ min_size + HBUINT8::static_size + (hb_len (it) + 1) * off_size + total;
  }

  فراغ set_offset_at (unsigned صحيح index, unsigned صحيح offset)
  {
    أكد (index <= count);

    unsigned صحيح size = offSize;
    ثابت HBUINT8 *p = offsets;
    بدل (size)
    {
      حالة 1: ((HBUINT8  *) p)[index] = offset; اكسر;
      حالة 2: ((HBUINT16 *) p)[index] = offset; اكسر;
      حالة 3: ((HBUINT24 *) p)[index] = offset; اكسر;
      حالة 4: ((HBUINT32 *) p)[index] = offset; اكسر;
      افتراضي: ܦܢܐ;
    }
  }

  خاص:
  unsigned صحيح offset_at (unsigned صحيح index) ثابت
  {
    أكد (index <= count);

    unsigned صحيح size = offSize;
    ثابت HBUINT8 *p = offsets;
    بدل (size)
    {
      حالة 1: ܦܢܐ ((HBUINT8  *) p)[index];
      حالة 2: ܦܢܐ ((HBUINT16 *) p)[index];
      حالة 3: ܦܢܐ ((HBUINT24 *) p)[index];
      حالة 4: ܦܢܐ ((HBUINT32 *) p)[index];
      افتراضي: ܦܢܐ 0;
    }
  }

  ثابت unsigned حرف *data_base () ثابت
  { ܦܢܐ (ثابت unsigned حرف *) هذا + min_size + offSize.static_size - 1 + offset_array_size (); }
  عام:

  hb_ubytes_t operator [] (unsigned صحيح index) ثابت
  {
    ܐܢ (unlikely (index >= count)) ܦܢܐ hb_ubytes_t ();
    hb_barrier ();
    unsigned offset0 = offset_at (index);
    unsigned offset1 = offset_at (index + 1);
    ܐܢ (unlikely (offset1 < offset0 || offset1 > offset_at (count)))
      ܦܢܐ hb_ubytes_t ();
    ܦܢܐ hb_ubytes_t (data_base () + offset0, offset1 - offset0);
  }

  size_t get_size () ثابت
  {
    ܐܢ (count)
      ܦܢܐ hb_unsigned_add_saturate (min_size, offSize.static_size,
				       offset_array_size (), offset_at (count) - 1);
    ܦܢܐ min_size;  /* empty CFFIndex contains count only */
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  (count == 0 || /* empty INDEX */
			   (count < count + 1u &&
			    c->check_struct (&offSize) && offSize >= 1 && offSize <= 4 &&
			    c->check_array (offsets, offSize, count + 1u) &&
			    c->check_range (data_base (), offset_at (count))))));
  }

  عام:
  COUNT		count;		/* Number of object data. Note there are (count+1) offsets */
  خاص:
  HBUINT8	offSize;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  عام:
  DEFINE_SIZE_MIN (COUNT::static_size);
};
تعريفنوع CFFIndex<HBUINT16> CFF1Index;
تعريفنوع CFFIndex<HBUINT32> CFF2Index;


/* TupleValues */
بنية TupleValues
{
  تعداد packed_value_flag_t
  {
    VALUES_ARE_ZEROS     = 0x80,
    VALUES_ARE_BYTES     = 0x00,
    VALUES_ARE_WORDS     = 0x40,
    VALUES_ARE_LONGS     = 0xC0,
    VALUES_SIZE_MASK     = 0xC0,
    VALUE_RUN_COUNT_MASK = 0x3F
  };

  ساكن unsigned compile_unsafe (hb_array_t<ثابت صحيح> values, /* IN */
				  unsigned حرف *encoded_bytes /* OUT */)
  {
    unsigned num_values = values.length;
    unsigned encoded_len = 0;
    unsigned i = 0;
    ܟܡܐ (i < num_values)
    {
      صحيح val = values.arrayZ[i];
      ܐܢ (val == 0)
        encoded_len += encode_value_run_as_zeroes (i, encoded_bytes + encoded_len, values);
      ܐܚܪܢܐ ܐܢ ((int8_t) val == val)
        encoded_len += encode_value_run_as_bytes (i, encoded_bytes + encoded_len, values);
      ܐܚܪܢܐ ܐܢ ((int16_t) val == val)
        encoded_len += encode_value_run_as_words (i, encoded_bytes + encoded_len, values);
      ܐܚܪܢܐ
        encoded_len += encode_value_run_as_longs (i, encoded_bytes + encoded_len, values);
    }
    ܦܢܐ encoded_len;
  }

  ساكن unsigned encode_value_run_as_zeroes (unsigned& i,
					      unsigned حرف *it,
					      hb_array_t<ثابت صحيح> values)
  {
    unsigned num_values = values.length;
    unsigned run_length = 0;
    unsigned encoded_len = 0;
    ܟܡܐ (i < num_values && values.arrayZ[i] == 0)
    {
      i++;
      run_length++;
    }

    ܟܡܐ (run_length >= 64)
    {
      *it++ = حرف (VALUES_ARE_ZEROS | 63);
      run_length -= 64;
      encoded_len++;
    }

    ܐܢ (run_length)
    {
      *it++ = حرف (VALUES_ARE_ZEROS | (run_length - 1));
      encoded_len++;
    }
    ܦܢܐ encoded_len;
  }

  ساكن unsigned encode_value_run_as_bytes (unsigned &i,
					     unsigned حرف *it,
					     hb_array_t<ثابت صحيح> values)
  {
    unsigned start = i;
    unsigned num_values = values.length;
    ܟܡܐ (i < num_values)
    {
      صحيح val = values.arrayZ[i];
      ܐܢ ((int8_t) val != val)
        اكسر;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      ܐܢ (val == 0 && i + 1 < num_values && values.arrayZ[i+1] == 0)
        اكسر;

      i++;
    }
    unsigned run_length = i - start;

    unsigned encoded_len = 0;

    ܟܡܐ (run_length >= 64)
    {
      *it++ = (VALUES_ARE_BYTES | 63);
      encoded_len++;

      لكل (unsigned j = 0; j < 64; j++)
	it[j] = static_cast<حرف> (values.arrayZ[start + j]);
      it += 64;
      encoded_len += 64;

      start += 64;
      run_length -= 64;
    }

    ܐܢ (run_length)
    {
      *it++ = (VALUES_ARE_BYTES | (run_length - 1));
      encoded_len++;

      لكل (unsigned j = 0; j < run_length; j++)
        it[j] = static_cast<حرف> (values.arrayZ[start + j]);
      encoded_len += run_length;
    }

    ܦܢܐ encoded_len;
  }

  ساكن unsigned encode_value_run_as_words (unsigned &i,
					     unsigned حرف *it,
					     hb_array_t<ثابت صحيح> values)
  {
    unsigned start = i;
    unsigned num_values = values.length;
    ܟܡܐ (i < num_values)
    {
      صحيح val = values.arrayZ[i];

      ܐܢ ((int16_t) val != val)
        اكسر;

      /* start a new run for a single zero value. */
      ܐܢ (val == 0) اكسر;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      ܐܢ ((int8_t) val == val &&
          i + 1 < num_values &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        اكسر;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    ܟܡܐ (run_length >= 64)
    {
      *it++ = (VALUES_ARE_WORDS | 63);
      encoded_len++;

      لكل (unsigned j = 0; j < 64; j++)
      {
        int16_t value_val = values.arrayZ[start + j];
        *it++ = static_cast<حرف> (value_val >> 8);
        *it++ = static_cast<حرف> (value_val & 0xFF);

        encoded_len += 2;
      }

      start += 64;
      run_length -= 64;
    }

    ܐܢ (run_length)
    {
      *it++ = (VALUES_ARE_WORDS | (run_length - 1));
      encoded_len++;
      ܟܡܐ (start < i)
      {
        int16_t value_val = values.arrayZ[start++];
        *it++ = static_cast<حرف> (value_val >> 8);
        *it++ = static_cast<حرف> (value_val & 0xFF);

        encoded_len += 2;
      }
    }
    ܦܢܐ encoded_len;
  }

  ساكن unsigned encode_value_run_as_longs (unsigned &i,
					     unsigned حرف *it,
					     hb_array_t<ثابت صحيح> values)
  {
    unsigned start = i;
    unsigned num_values = values.length;
    ܟܡܐ (i < num_values)
    {
      صحيح val = values.arrayZ[i];

      ܐܢ ((int16_t) val == val)
        اكسر;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    ܟܡܐ (run_length >= 64)
    {
      *it++ = (VALUES_ARE_LONGS | 63);
      encoded_len++;

      لكل (unsigned j = 0; j < 64; j++)
      {
        int32_t value_val = values.arrayZ[start + j];
        *it++ = static_cast<حرف> (value_val >> 24);
        *it++ = static_cast<حرف> (value_val >> 16);
        *it++ = static_cast<حرف> (value_val >> 8);
        *it++ = static_cast<حرف> (value_val & 0xFF);

        encoded_len += 4;
      }

      start += 64;
      run_length -= 64;
    }

    ܐܢ (run_length)
    {
      *it++ = (VALUES_ARE_LONGS | (run_length - 1));
      encoded_len++;
      ܟܡܐ (start < i)
      {
        int32_t value_val = values.arrayZ[start++];
        *it++ = static_cast<حرف> (value_val >> 24);
        *it++ = static_cast<حرف> (value_val >> 16);
        *it++ = static_cast<حرف> (value_val >> 8);
        *it++ = static_cast<حرف> (value_val & 0xFF);

        encoded_len += 4;
      }
    }
    ܦܢܐ encoded_len;
  }

  template <typename T>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ساكن منطقي decompile (ثابت HBUINT8 *&p /* IN/OUT */,
			 hb_vector_t<T> &values /* IN/OUT */,
			 ثابت HBUINT8 *end,
			 منطقي consume_all = ܕܓܠܐ,
			 unsigned start = 0)
  {
    unsigned i = 0;
    unsigned count = consume_all ? UINT_MAX : values.length;
    ܐܢ (consume_all)
      values.alloc ((end - p) / 2);
    ܟܡܐ (i < count)
    {
      ܐܢ (unlikely (p + 1 > end)) ܦܢܐ consume_all;
      unsigned control = *p++;
      unsigned run_count = (control & VALUE_RUN_COUNT_MASK) + 1;
      ܐܢ (consume_all)
      {
        ܐܢ (unlikely (!values.resize_dirty  (values.length + run_count)))
	  ܦܢܐ ܕܓܠܐ;
      }
      unsigned stop = i + run_count;
      ܐܢ (unlikely (stop > count)) ܦܢܐ ܕܓܠܐ;

      unsigned skip = i < start ? hb_min (start - i, run_count) : 0;
      i += skip;

      ܐܢ ((control & VALUES_SIZE_MASK) == VALUES_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * حجم (T));
	i = stop;
      }
      ܐܚܪܢܐ ܐܢ ((control & VALUES_SIZE_MASK) ==  VALUES_ARE_WORDS)
      {
        ܐܢ (unlikely (p + run_count * HBINT16::static_size > end)) ܦܢܐ ܕܓܠܐ;
	p += skip * HBINT16::static_size;
#ifndef HB_OPTIMIZE_SIZE
        لكل (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 1] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 2] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 3] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_size;
	}
#endif
        لكل (; i < stop; i++)
        {
          values.arrayZ[i] = * (ثابت HBINT16 *) p;
          p += HBINT16::static_size;
        }
      }
      ܐܚܪܢܐ ܐܢ ((control & VALUES_SIZE_MASK) ==  VALUES_ARE_LONGS)
      {
        ܐܢ (unlikely (p + run_count * HBINT32::static_size > end)) ܦܢܐ ܕܓܠܐ;
	p += skip * HBINT32::static_size;
        لكل (; i < stop; i++)
        {
          values.arrayZ[i] = * (ثابت HBINT32 *) p;
          p += HBINT32::static_size;
        }
      }
      ܐܚܪܢܐ ܐܢ ((control & VALUES_SIZE_MASK) ==  VALUES_ARE_BYTES)
      {
        ܐܢ (unlikely (p + run_count > end)) ܦܢܐ ܕܓܠܐ;
	p += skip * HBINT8::static_size;
#ifndef HB_OPTIMIZE_SIZE
	لكل (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (ثابت HBINT8 *) p++;
	}
#endif
        لكل (; i < stop; i++)
          values.arrayZ[i] = * (ثابت HBINT8 *) p++;
      }
    }
    ܦܢܐ ܫܪܪܐ;
  }

  ساكن منطقي skip (ثابت unsigned حرف *&p /* IN/OUT */,
		    ثابت unsigned حرف *end,
		    unsigned count)
  {
    unsigned seen = 0;
    ܟܡܐ (seen < count)
    {
      ܐܢ (unlikely (p >= end)) ܦܢܐ ܕܓܠܐ;
      unsigned control = *p++;
      unsigned run_count = (control & VALUE_RUN_COUNT_MASK) + 1;
      ܐܢ (unlikely (run_count > count - seen)) ܦܢܐ ܕܓܠܐ;

      unsigned width = 0;
      بدل (control & VALUES_SIZE_MASK)
      {
	حالة VALUES_ARE_ZEROS: اكسر;
	حالة VALUES_ARE_BYTES: width = HBINT8::static_size; اكسر;
	حالة VALUES_ARE_WORDS: width = HBINT16::static_size; اكسر;
	حالة VALUES_ARE_LONGS: width = HBINT32::static_size; اكسر;
      }
      ܐܢ (unlikely (unsigned (end - p) < run_count * width)) ܦܢܐ ܕܓܠܐ;
      p += run_count * width;
      seen += run_count;
    }
    ܦܢܐ ܫܪܪܐ;
  }

  بنية iter_t : hb_iter_with_fallback_t<iter_t, صحيح>
  {
    iter_t (ثابت unsigned حرف *p_, unsigned len_)
	    : p (p_), endp (p_ + len_)
    { ܐܢ (likely (ensure_run ())) read_value (); }

    خاص:
    ثابت unsigned حرف *p;
    ثابت unsigned حرف * ثابت endp;
    صحيح current_value = 0;
    signed run_count = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    منطقي ensure_run ()
    {
      ܐܢ (likely (run_count > 0)) ܦܢܐ ܫܪܪܐ;
      ܦܢܐ _ensure_run ();
    }
    منطقي _ensure_run ()
    {
      ܐܢ (unlikely (p >= endp))
      {
        run_count = 0;
        current_value = 0;
	ܦܢܐ ܕܓܠܐ;
      }

      unsigned control = *p++;
      run_count = (control & VALUE_RUN_COUNT_MASK) + 1;
      width = control & VALUES_SIZE_MASK;
      بدل (width)
      {
        حالة VALUES_ARE_ZEROS: width = 0; اكسر;
	حالة VALUES_ARE_BYTES: width = HBINT8::static_size;  اكسر;
	حالة VALUES_ARE_WORDS: width = HBINT16::static_size; اكسر;
	حالة VALUES_ARE_LONGS: width = HBINT32::static_size; اكسر;
	افتراضي: أكد (ܕܓܠܐ);
      }

      ܐܢ (unlikely (p + run_count * width > endp))
      {
	run_count = 0;
	current_value = 0;
	ܦܢܐ ܕܓܠܐ;
      }

      ܦܢܐ ܫܪܪܐ;
    }
    فراغ read_value ()
    {
      بدل (width)
      {
        حالة 0: current_value = 0; اكسر;
	حالة 1: current_value = * (ثابت HBINT8  *) p; اكسر;
	حالة 2: current_value = * (ثابت HBINT16 *) p; اكسر;
	حالة 4: current_value = * (ثابت HBINT32 *) p; اكسر;
      }
      p += width;
    }

    عام:

    تعريفنوع صحيح __item_t__;
    __item_t__ __item__ () ثابت
    { ܦܢܐ current_value; }

    منطقي __more__ () ثابت { ܦܢܐ run_count || p < endp; }
    فراغ __next__ ()
    {
      run_count--;
      ܐܢ (unlikely (!ensure_run ()))
	ܦܢܐ;
      read_value ();
    }
    فراغ __forward__ (unsigned n)
    {
      ܐܢ (unlikely (!ensure_run ()))
	ܦܢܐ;
      ܟܡܐ (n)
      {
	unsigned i = hb_min (n, (unsigned) run_count);
	run_count -= i;
	n -= i;
	p += (i - 1) * width;
	ܐܢ (unlikely (!ensure_run ()))
	  ܦܢܐ;
	read_value ();
      }
    }
    منطقي operator != (ثابت iter_t& o) ثابت
    { ܦܢܐ p != o.p || run_count != o.run_count; }
    iter_t __end__ () ثابت
    {
      iter_t it (endp, 0);
      ܦܢܐ it;
    }
  };

  بنية fetcher_t
  {
    fetcher_t (ثابت unsigned حرف *p_, unsigned len_)
	      : p (p_), end (p_ + len_) {}

    خاص:
    ثابت unsigned حرف *p;
    ثابت unsigned حرف * ثابت end;
    signed run_count = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    منطقي ensure_run ()
    {
      ܐܢ (likely (run_count > 0)) ܦܢܐ ܫܪܪܐ;
      ܦܢܐ _ensure_run ();
    }

    منطقي _ensure_run ()
    {
      ܐܢ (unlikely (p >= end))
      {
        run_count = 0;
	ܦܢܐ ܕܓܠܐ;
      }

      unsigned control = *p++;
      run_count = (control & VALUE_RUN_COUNT_MASK) + 1;
      width = control & VALUES_SIZE_MASK;
      بدل (width)
      {
        حالة VALUES_ARE_ZEROS: width = 0; اكسر;
	حالة VALUES_ARE_BYTES: width = HBINT8::static_size;  اكسر;
	حالة VALUES_ARE_WORDS: width = HBINT16::static_size; اكسر;
	حالة VALUES_ARE_LONGS: width = HBINT32::static_size; اكسر;
	افتراضي: أكد (ܕܓܠܐ);
      }

      ܐܢ (unlikely (p + run_count * width > end))
      {
	run_count = 0;
	ܦܢܐ ܕܓܠܐ;
      }

      ܦܢܐ ܫܪܪܐ;
    }

    عام:
    فراغ skip (unsigned n)
    {
      ܟܡܐ (n)
      {
	ܐܢ (unlikely (!ensure_run ()))
	  ܦܢܐ;
	unsigned i = hb_min (n, (unsigned) run_count);
	run_count -= i;
	n -= i;
	p += i * width;
      }
    }

    خاص:
    template <منطقي scaled, typename T>
    فراغ _add_to (hb_array_t<T> out, T scale = T (1))
    {
      unsigned n = out.length;
      T *arrayZ = out.arrayZ;

      لكل (unsigned i = 0; i < n;)
      {
	ܐܢ (unlikely (!ensure_run ()))
	  اكسر;
	unsigned count = hb_min (n - i, (unsigned) run_count);
	بدل (width)
	{
	  حالة 0:
	  {
	    arrayZ += count;
	    اكسر;
	  }
	  حالة 1:
	  {
	    ثابت auto *pp = (ثابت HBINT8 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    لكل (; j + 3 < count; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    لكل (; j < count; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت unsigned حرف *) pp;
	  }
	  اكسر;
	  حالة 2:
	  {
	    ثابت auto *pp = (ثابت HBINT16 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    لكل (; j + 3 < count; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    لكل (; j < count; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت unsigned حرف *) pp;
	  }
	  اكسر;
	  حالة 4:
	  {
	    ثابت auto *pp = (ثابت HBINT32 *) p;
	    لكل (unsigned j = 0; j < count; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت unsigned حرف *) pp;
	  }
	  اكسر;
	}
	run_count -= count;
	i += count;
      }
    }

    عام:
    template <typename T>
    فراغ add_to (hb_array_t<T> out, عائم scale = 1.0f)
    {
#ifndef HB_OPTIMIZE_SIZE
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      ܐܢ (ܕܓܠܐ && scale == 1.0f)
        _add_to<ܕܓܠܐ> (out);
      ܐܚܪܢܐ
#endif
        _add_to<ܫܪܪܐ> (out, (T) scale);
    }
  };
};

بنية TupleList : CFF2Index
{
  TupleValues::iter_t operator [] (unsigned i) ثابت
  {
    auto bytes = CFF2Index::operator [] (i);
    ܦܢܐ TupleValues::iter_t (bytes.arrayZ, bytes.length);
  }

  TupleValues::fetcher_t fetcher (unsigned i) ثابت
  {
    auto bytes = CFF2Index::operator [] (i);
    ܦܢܐ TupleValues::fetcher_t (bytes.arrayZ, bytes.length);
  }
};


// Alignment

template <unsigned صحيح alignment>
بنية Align
{
  size_t get_size (ثابت فراغ *base) ثابت
  {
    unsigned offset = (ثابت حرف *) هذا - (ثابت حرف *) base;
    ܦܢܐ (alignment - offset) & (alignment - 1);
  }

  عام:
  DEFINE_SIZE_MIN (0);
};



} /* namespace OT */


#endif /* HB_OPEN_TYPE_HH */
