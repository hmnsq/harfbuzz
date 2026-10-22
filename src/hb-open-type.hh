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

#شامل "hb.hh"
#شامل "hb-blob.hh"
#شامل "hb-face.hh"
#شامل "hb-machinery.hh"
#شامل "hb-meta.hh"
#شامل "hb-subset.hh"


فضاینام OT {


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
template <منطقی BE,
	  typename Type,
	  unsigned صحیح Size = اندازه (Type)>
ساختار رقم_Type
{
  تعریفنوع Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  تعریفنوع typename std::conditional<std::is_integral<Type>::value && اندازه (Type) <= اندازه(صحیح),
				     typename std::conditional<std::is_signed<Type>::value, signed, unsigned>::type,
				     Type>::type WideType;

  رقم_Type () = پیشفرض;
  explicit constexpr رقم_Type (Type V) : v {V} {}
  رقم_Type& operator = (Type V) { v = V; بازگشت *این; }

  operator WideType () ثابت { بازگشت v; }

  منطقی operator == (ثابت رقم_Type &o) ثابت { بازگشت (Type) v == (Type) o.v; }
  منطقی operator != (ثابت رقم_Type &o) ثابت { بازگشت !(*این == o); }

  رقم_Type& operator += (WideType شمار) { *این = *این + شمار; بازگشت *این; }
  رقم_Type& operator -= (WideType شمار) { *این = *این - شمار; بازگشت *این; }
  رقم_Type& operator ++ () { *این += 1; بازگشت *این; }
  رقم_Type& operator -- () { *این -= 1; بازگشت *این; }
  رقم_Type operator ++ (صحیح) { رقم_Type c (*این); ++*این; بازگشت c; }
  رقم_Type operator -- (صحیح) { رقم_Type c (*این); --*این; بازگشت c; }

  uint32_t hash () ثابت { بازگشت hb_array ((ثابت نویسه *) &v, اندازه (v)).hash (); }
  HB_INTERNAL ایستا صحیح cmp (ثابت رقم_Type *a, ثابت رقم_Type *b)
  { بازگشت b->cmp (*a); }
  HB_INTERNAL ایستا صحیح cmp (ثابت تهی *a, ثابت تهی *b)
  {
    رقم_Type *pa = (رقم_Type *) a;
    رقم_Type *pb = (رقم_Type *) b;

    بازگشت pb->cmp (*pa);
  }
  template <typename Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  صحیح cmp (Type2 a) ثابت
  {
    Type b = v;
    بازگشت (a > b) - (a < b);
  }
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }
  محافظتشده:
  typename std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, Size>,
			    HBFloat<BE, Type, Size>>::type v;
  عمومی:
  DEFINE_SIZE_STATIC (Size);
};

تعریفنوع رقم_Type<درست, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
تعریفنوع رقم_Type<درست, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
تعریفنوع رقم_Type<درست, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
تعریفنوع رقم_Type<درست, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
تعریفنوع رقم_Type<درست, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
تعریفنوع رقم_Type<درست, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
تعریفنوع رقم_Type<درست, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
تعریفنوع رقم_Type<درست, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
تعریفنوع رقم_Type<درست, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

تعریفنوع رقم_Type<نادرست, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
تعریفنوع رقم_Type<نادرست, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
تعریفنوع رقم_Type<نادرست, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
تعریفنوع رقم_Type<نادرست, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
تعریفنوع رقم_Type<نادرست, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
تعریفنوع رقم_Type<نادرست, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

تعریفنوع رقم_Type<درست,  اعشاری>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
تعریفنوع رقم_Type<درست,  دوگانه> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
تعریفنوع رقم_Type<نادرست, اعشاری>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
تعریفنوع رقم_Type<نادرست, دوگانه> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
ساختار HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& operator = (uint16_t i ) { HBUINT16::operator= (i); بازگشت *این; }
  عمومی:
  DEFINE_SIZE_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
ساختار HBUINT32VAR
{
  size_t get_size () ثابت
  {
    unsigned b0 = v[0];
    اگر (b0 < 0x80)
      بازگشت 1;
    وگرنه اگر (b0 < 0xC0)
      بازگشت 2;
    وگرنه اگر (b0 < 0xE0)
      بازگشت 3;
    وگرنه اگر (b0 < 0xF0)
      بازگشت 4;
    وگرنه
      بازگشت 5;
  }

  ایستا size_t get_size (uint32_t v)
  {
    اگر (v < 0x80)
      بازگشت 1;
    وگرنه اگر (v < 0x4000)
      بازگشت 2;
    وگرنه اگر (v < 0x200000)
      بازگشت 3;
    وگرنه اگر (v < 0x10000000)
      بازگشت 4;
    وگرنه
      بازگشت 5;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_size ()));
  }

  operator uint32_t () ثابت
  {
    unsigned b0 = v[0];
    اگر (b0 < 0x80)
      بازگشت b0;
    وگرنه اگر (b0 < 0xC0)
      بازگشت ((b0 & 0x3F) << 8) | v[1];
    وگرنه اگر (b0 < 0xE0)
      بازگشت ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    وگرنه اگر (b0 < 0xF0)
      بازگشت ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    وگرنه
      بازگشت (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  ایستا unsigned serialize_unsafe (unsigned نویسه *buf, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned نویسه *p = buf + len;
    برای (unsigned i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    اگر (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    بازگشت len;
  }

  ایستا منطقی serialize (hb_serialize_context_t *c, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned نویسه *buf = c->allocate_size<unsigned نویسه> (len, نادرست);
    اگر (unlikely (!buf))
      بازگشت نادرست;

    serialize_unsafe (buf, v);
    بازگشت درست;
  }

  محافظتشده:
  unsigned نویسه v[5];

  عمومی:
  DEFINE_SIZE_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
تعریفنوع HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
تعریفنوع HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
تعریفنوع HBUINT16 UFWORD;

template <typename Type, unsigned fraction_bits>
ساختار HBFixed : Type
{
  ایستا constexpr اعشاری mult = 1.f / (1 << fraction_bits);
  static_assert (Type::static_size * 8 > fraction_bits, "");

  operator signed () ثابت = delete;
  operator unsigned () ثابت = delete;
  explicit operator اعشاری () ثابت { بازگشت to_float (); }
  typename Type::type to_int () ثابت { بازگشت Type::v; }
  تهی set_int (typename Type::type i ) { Type::v = i; }
  اعشاری to_float (اعشاری offset = 0) ثابت  { بازگشت ((int32_t) Type::v + offset) * mult; }
  تهی set_float (اعشاری f) { Type::v = roundf (f / mult); }
  عمومی:
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
ساختار LONGDATETIME
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }
  محافظتشده:
  HBINT32 major;
  HBUINT32 minor;
  عمومی:
  DEFINE_SIZE_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
ساختار Tag : HBUINT32
{
  Tag& operator = (hb_tag_t i) { HBUINT32::operator= (i); بازگشت *این; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  operator ثابت نویسه* () ثابت { بازگشت reinterpret_cast<ثابت نویسه *> (این); }
  operator نویسه* ()             { بازگشت reinterpret_cast<نویسه *> (این); }
  عمومی:
  DEFINE_SIZE_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
ساختار HBGlyphID16 : HBUINT16
{
  HBGlyphID16& operator = (uint16_t i) { HBUINT16::operator= (i); بازگشت *این; }
};
ساختار HBGlyphID24 : HBUINT24
{
  HBGlyphID24& operator = (uint32_t i) { HBUINT24::operator= (i); بازگشت *این; }
};

/* Script/language-system/feature index */
ساختار Index : HBUINT16 {
  ایستا constexpr unsigned NOT_FOUND_INDEX = 0xFFFFu;
  Index& operator = (uint16_t i) { HBUINT16::operator= (i); بازگشت *این; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, Index);

تعریفنوع Index NameID;

ساختار VarIdx : HBUINT32 {
  ایستا constexpr unsigned NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_INDEX, "");
  VarIdx& operator = (uint32_t i) { HBUINT32::operator= (i); بازگشت *این; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, VarIdx);

/* Offset, Null offset = 0 */
template <typename Type, منطقی has_null=درست>
ساختار Offset : Type
{
  Offset& operator = (typename Type::type i) { Type::operator= (i); بازگشت *این; }

  تعریفنوع Type type;

  منطقی is_null () ثابت { بازگشت has_null && 0 == *این; }

  عمومی:
  DEFINE_SIZE_STATIC (اندازه (Type));
};

تعریفنوع Offset<HBUINT16> Offset16;
تعریفنوع Offset<HBUINT24> Offset24;
تعریفنوع Offset<HBUINT32> Offset32;


/* CheckSum */
ساختار CheckSum : HBUINT32
{
  CheckSum& operator = (uint32_t i) { HBUINT32::operator= (i); بازگشت *این; }

  /* This is reference implementation from the spec. */
  ایستا uint32_t CalcTableChecksum (ثابت HBUINT32 *Table, uint32_t Length)
  {
    uint32_t Sum = 0L;
    أكد (0 == (Length & 3));
    ثابت HBUINT32 *EndPtr = Table + Length / HBUINT32::static_size;

    وقتی (Table < EndPtr)
      Sum += *Table++;
    بازگشت Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  تهی set_for_data (ثابت تهی *data, unsigned صحیح length)
  { *این = CalcTableChecksum ((ثابت HBUINT32 *) data, length); }

  عمومی:
  DEFINE_SIZE_STATIC (4);
};


/*
 * Version Numbers
 */

template <typename FixedType=HBUINT16>
ساختار FixedVersion
{
  uint32_t to_int () ثابت { بازگشت (major << (اندازه (FixedType) * 8)) + minor; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  FixedType major;
  FixedType minor;
  عمومی:
  DEFINE_SIZE_STATIC (2 * اندازه (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

template <typename Type, منطقی has_null>
ساختار _hb_has_null
{
  ایستا ثابت Type *get_null () { بازگشت nullptr; }
  ایستا Type *get_crap ()       { بازگشت nullptr; }
};
template <typename Type>
ساختار _hb_has_null<Type, درست>
{
  ایستا ثابت Type *get_null () { بازگشت &Null (Type); }
  ایستا       Type *get_crap () { بازگشت &Crap (Type); }
};

template <typename Type, typename OffsetType, typename BaseType=تهی, منطقی has_null=درست>
ساختار OffsetTo : Offset<OffsetType, has_null>
{
  using target_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == نادرست ||
		 (hb_has_null_size (Type) || !hb_has_min_size (Type)), "");

  HB_DELETE_COPY_ASSIGN (OffsetTo);
  OffsetTo () = پیشفرض;

  OffsetTo& operator = (typename OffsetType::type i) { OffsetType::operator= (i); بازگشت *این; }

  ثابت Type& operator () (ثابت تهی *base) ثابت
  {
    اگر (unlikely (این->is_null ())) بازگشت *_hb_has_null<Type, has_null>::get_null ();
    بازگشت StructAtOffset<ثابت Type> (base, *این);
  }
  Type& operator () (تهی *base) ثابت
  {
    اگر (unlikely (این->is_null ())) بازگشت *_hb_has_null<Type, has_null>::get_crap ();
    بازگشت StructAtOffset<Type> (base, *این);
  }

  template <typename Base,
	    hb_enable_if (hb_is_convertible (ثابت Base, ثابت BaseType *))>
  friend ثابت Type& operator + (ثابت Base &base, ثابت OffsetTo &offset) { بازگشت offset ((ثابت تهی *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (ثابت Base, ثابت BaseType *))>
  friend ثابت Type& operator + (ثابت OffsetTo &offset, ثابت Base &base) { بازگشت offset ((ثابت تهی *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (Base &&base, OffsetTo &offset) { بازگشت offset ((تهی *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (OffsetTo &offset, Base &&base) { بازگشت offset ((تهی *) base); }


  template <typename Base, typename ...Ts>
  منطقی serialize_subset (hb_subset_context_t *c, ثابت OffsetTo& src,
			 ثابت Base *src_base, Ts&&... ds)
  {
    *این = 0;
    اگر (src.is_null ())
      بازگشت نادرست;

    auto *s = c->serializer;

    s->push ();

    منطقی ret = c->dispatch (src_base+src, std::forward<Ts> (ds)...);

    اگر (ret || !has_null)
      s->add_link (*این, s->برداشت_pack ());
    وگرنه
      s->برداشت_discard ();

    بازگشت ret;
  }


  template <typename ...Ts>
  منطقی serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *این = 0;

    Type* obj = c->push<Type> ();
    منطقی ret = obj->serialize (c, std::forward<Ts> (ds)...);

    اگر (ret)
      c->add_link (*این, c->برداشت_pack ());
    وگرنه
      c->برداشت_discard ();

    بازگشت ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  template <typename ...Ts>
  منطقی serialize_copy (hb_serialize_context_t *c, ثابت OffsetTo& src,
		       ثابت تهی *src_base, unsigned dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *این = 0;
    اگر (src.is_null ())
      بازگشت نادرست;

    c->push ();

    منطقی ret = c->copy (src_base+src, std::forward<Ts> (ds)...);

    c->add_link (*این, c->برداشت_pack (), whence, dst_bias);

    بازگشت ret;
  }

  منطقی serialize_copy (hb_serialize_context_t *c, ثابت OffsetTo& src,
		       ثابت تهی *src_base, unsigned dst_bias = 0)
  { بازگشت serialize_copy (c, src, src_base, dst_bias, hb_serialize_context_t::Head); }

  منطقی sanitize_shallow (hb_sanitize_context_t *c, ثابت BaseType *base) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!c->check_struct (این))) return_trace (نادرست);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    اگر (unlikely ((ثابت نویسه *) base + (unsigned) *این < (ثابت نویسه *) base)) return_trace (نادرست);
    return_trace (درست);
  }

  template <typename ...Ts>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  منطقی sanitize (hb_sanitize_context_t *c, ثابت BaseType *base, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (sanitize_shallow (c, base) &&
		  hb_barrier () &&
		  (این->is_null () ||
		   c->dispatch (StructAtOffset<Type> (base, *این), std::forward<Ts> (ds)...)));
  }

  DEFINE_SIZE_STATIC (اندازه (OffsetType));
};
/* Partial specializations. */
template <typename Type, typename BaseType=تهی, منطقی has_null=درست> using Offset16To = OffsetTo<Type, HBUINT16, BaseType, has_null>;
template <typename Type, typename BaseType=تهی, منطقی has_null=درست> using Offset24To = OffsetTo<Type, HBUINT24, BaseType, has_null>;
template <typename Type, typename BaseType=تهی, منطقی has_null=درست> using Offset32To = OffsetTo<Type, HBUINT32, BaseType, has_null>;

template <typename Type, typename OffsetType, typename BaseType=تهی> using NNOffsetTo = OffsetTo<Type, OffsetType, BaseType, نادرست>;
template <typename Type, typename BaseType=تهی> using NNOffset16To = Offset16To<Type, BaseType, نادرست>;
template <typename Type, typename BaseType=تهی> using NNOffset24To = Offset24To<Type, BaseType, نادرست>;
template <typename Type, typename BaseType=تهی> using NNOffset32To = Offset32To<Type, BaseType, نادرست>;


/*
 * Array Types
 */

template <typename Type>
ساختار UnsizedArrayOf
{
  تعریفنوع Type item_t;
  ایستا constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (UnsizedArrayOf);

  ثابت Type& operator [] (unsigned صحیح i) ثابت
  {
    بازگشت arrayZ[i];
  }
  Type& operator [] (unsigned صحیح i)
  {
    بازگشت arrayZ[i];
  }

  ایستا size_t get_size (unsigned صحیح len)
  { بازگشت hb_unsigned_mul_saturate (len, Type::static_size); }

  template <typename T> operator T * () { بازگشت arrayZ; }
  template <typename T> operator ثابت T * () ثابت { بازگشت arrayZ; }
  hb_array_t<Type> as_array (unsigned صحیح len)
  { بازگشت hb_array (arrayZ, len); }
  hb_array_t<ثابت Type> as_array (unsigned صحیح len) ثابت
  { بازگشت hb_array (arrayZ, len); }

  template <typename T>
  Type &lsearch (unsigned صحیح len, ثابت T &x, Type &not_found = Crap (Type))
  { بازگشت *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  ثابت Type &lsearch (unsigned صحیح len, ثابت T &x, ثابت Type &not_found = Null (Type)) ثابت
  { بازگشت *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  منطقی lfind (unsigned صحیح len, ثابت T &x, unsigned صحیح *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned صحیح to_store = (unsigned صحیح) -1) ثابت
  { بازگشت as_array (len).lfind (x, i, not_found, to_store); }

  تهی qsort (unsigned صحیح len, unsigned صحیح start = 0, unsigned صحیح end = (unsigned صحیح) -1)
  { as_array (len).qsort (start, end); }

  منطقی serialize (hb_serialize_context_t *c, unsigned صحیح اقلام_len, منطقی clear = درست)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_size (این, get_size (اقلام_len), clear))) return_trace (نادرست);
    return_trace (درست);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  منطقی serialize (hb_serialize_context_t *c, Iterator اقلام)
  {
    TRACE_SERIALIZE (این);
    unsigned شمار = hb_len (اقلام);
    اگر (unlikely (!serialize (c, شمار, نادرست))) return_trace (نادرست);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    برای (unsigned i = 0; i < شمار; i++, ++اقلام)
      arrayZ[i] = *اقلام;
    return_trace (درست);
  }

  UnsizedArrayOf* copy (hb_serialize_context_t *c, unsigned شمار) ثابت
  {
    TRACE_SERIALIZE (این);
    auto *out = c->start_embed (این);
    اگر (unlikely (!as_array (شمار).copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  منطقی sanitize (hb_sanitize_context_t *c, unsigned صحیح شمار, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!sanitize_shallow (c, شمار))) return_trace (نادرست);
    اگر (!اندازه... (Ts) && hb_is_trivially_copyable(Type)) return_trace (درست);
    hb_barrier ();
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (نادرست);
    return_trace (درست);
  }

  منطقی sanitize_shallow (hb_sanitize_context_t *c, unsigned صحیح شمار) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_array (arrayZ, شمار));
  }

  عمومی:
  Type		arrayZ[HB_VAR_ARRAY];
  عمومی:
  DEFINE_SIZE_UNBOUNDED (0);
};

/* Unsized array of offset's */
template <typename Type, typename OffsetType, typename BaseType=تهی, منطقی has_null=درست>
using UnsizedArray16OfOffsetTo = UnsizedArrayOf<OffsetTo<Type, OffsetType, BaseType, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
template <typename Type, typename OffsetType, typename BaseType=تهی, منطقی has_null=درست>
ساختار UnsizedListOfOffset16To : UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
{
  ثابت Type& operator [] (صحیح i_) ثابت
  {
    unsigned صحیح i = (unsigned صحیح) i_;
    ثابت OffsetTo<Type, OffsetType, BaseType, has_null> *p = &این->arrayZ[i];
    اگر (unlikely ((ثابت تهی *) p < (ثابت تهی *) این->arrayZ)) بازگشت Null (Type); /* Overflowed. */
    hb_barrier ();
    بازگشت این+*p;
  }
  Type& operator [] (صحیح i_)
  {
    unsigned صحیح i = (unsigned صحیح) i_;
    ثابت OffsetTo<Type, OffsetType, BaseType, has_null> *p = &این->arrayZ[i];
    اگر (unlikely ((ثابت تهی *) p < (ثابت تهی *) این->arrayZ)) بازگشت Crap (Type); /* Overflowed. */
    hb_barrier ();
    بازگشت این+*p;
  }

  template <typename ...Ts>
  منطقی sanitize (hb_sanitize_context_t *c, unsigned صحیح شمار, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace ((UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
		   ::sanitize (c, شمار, این, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type>
ساختار SortedUnsizedArrayOf : UnsizedArrayOf<Type>
{
  hb_sorted_array_t<Type> as_array (unsigned صحیح len)
  { بازگشت hb_sorted_array (این->arrayZ, len); }
  hb_sorted_array_t<ثابت Type> as_array (unsigned صحیح len) ثابت
  { بازگشت hb_sorted_array (این->arrayZ, len); }
  operator hb_sorted_array_t<Type> ()             { بازگشت as_array (); }
  operator hb_sorted_array_t<ثابت Type> () ثابت { بازگشت as_array (); }

  template <typename T>
  Type &bsearch (unsigned صحیح len, ثابت T &x, Type &not_found = Crap (Type))
  { بازگشت *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  ثابت Type &bsearch (unsigned صحیح len, ثابت T &x, ثابت Type &not_found = Null (Type)) ثابت
  { بازگشت *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  منطقی bfind (unsigned صحیح len, ثابت T &x, unsigned صحیح *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned صحیح to_store = (unsigned صحیح) -1) ثابت
  { بازگشت as_array (len).bfind (x, i, not_found, to_store); }
};


/* An array with a number of elements. */
template <typename Type, typename LenType>
ساختار ArrayOf
{
  تعریفنوع Type item_t;
  ایستا constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOf);

  ثابت Type& operator [] (صحیح i_) ثابت
  {
    unsigned صحیح i = (unsigned صحیح) i_;
    اگر (unlikely (i >= len)) بازگشت Null (Type);
    hb_barrier ();
    بازگشت arrayZ[i];
  }
  Type& operator [] (صحیح i_)
  {
    unsigned صحیح i = (unsigned صحیح) i_;
    اگر (unlikely (i >= len)) بازگشت Crap (Type);
    hb_barrier ();
    بازگشت arrayZ[i];
  }

  size_t get_size () ثابت
  { بازگشت hb_unsigned_mul_add_saturate (len, Type::static_size, len.static_size); }

  explicit operator منطقی () ثابت { بازگشت len; }

  تهی برداشت () { len--; }

  hb_array_t<      Type> as_array ()       { بازگشت hb_array (arrayZ, len); }
  hb_array_t<ثابت Type> as_array () ثابت { بازگشت hb_array (arrayZ, len); }

  /* Iterator. */
  تعریفنوع hb_array_t<ثابت Type>   iter_t;
  تعریفنوع hb_array_t<      Type> writer_t;
    iter_t   iter () ثابت { بازگشت as_array (); }
  writer_t writer ()       { بازگشت as_array (); }
  operator   iter_t () ثابت { بازگشت   iter (); }
  operator writer_t ()       { بازگشت writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { بازگشت arrayZ; }
  ثابت Type *end () ثابت { بازگشت arrayZ + len; }

  template <typename T>
  Type &lsearch (ثابت T &x, Type &not_found = Crap (Type))
  { بازگشت *as_array ().lsearch (x, &not_found); }
  template <typename T>
  ثابت Type &lsearch (ثابت T &x, ثابت Type &not_found = Null (Type)) ثابت
  { بازگشت *as_array ().lsearch (x, &not_found); }
  template <typename T>
  منطقی lfind (ثابت T &x, unsigned صحیح *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned صحیح to_store = (unsigned صحیح) -1) ثابت
  { بازگشت as_array ().lfind (x, i, not_found, to_store); }

  تهی qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD منطقی serialize (hb_serialize_context_t *c, unsigned اقلام_len, منطقی clear = درست)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);
    c->check_assign (len, اقلام_len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    اگر (unlikely (!c->extend_size (این, get_size (), clear))) return_trace (نادرست);
    return_trace (درست);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD منطقی serialize (hb_serialize_context_t *c, Iterator اقلام)
  {
    TRACE_SERIALIZE (این);
    unsigned شمار = hb_len (اقلام);
    اگر (unlikely (!serialize (c, شمار, نادرست))) return_trace (نادرست);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    برای (unsigned i = 0; i < شمار; i++, ++اقلام)
      arrayZ[i] = *اقلام;
    return_trace (درست);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (این);
    len++;
    اگر (unlikely (!len || !c->extend (این)))
    {
      len--;
      return_trace (nullptr);
    }
    return_trace (&arrayZ[len - 1]);
  }

  ArrayOf* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (این);
    auto *out = c->start_embed (این);
    اگر (unlikely (!c->extend_min (out))) return_trace (nullptr);
    c->check_assign (out->len, len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    اگر (unlikely (!as_array ().copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  منطقی sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!sanitize_shallow (c))) return_trace (نادرست);
    اگر (!اندازه... (Ts) && hb_is_trivially_copyable(Type)) return_trace (درست);
    hb_barrier ();
    unsigned صحیح شمار = len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (نادرست);
    return_trace (درست);
  }

  منطقی sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (len.sanitize (c) &&
		  hb_barrier () &&
		  c->check_array_sized (arrayZ, len, اندازه (LenType)));
  }

  عمومی:
  LenType	len;
  Type		arrayZ[HB_VAR_ARRAY];
  عمومی:
  DEFINE_SIZE_ARRAY (اندازه (LenType), arrayZ);
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
ساختار List16OfOffsetTo : ArrayOf<OffsetTo<Type, OffsetType>, HBUINT16>
{
  ثابت Type& operator [] (صحیح i_) ثابت
  {
    unsigned صحیح i = (unsigned صحیح) i_;
    اگر (unlikely (i >= این->len)) بازگشت Null (Type);
    hb_barrier ();
    بازگشت این+این->arrayZ[i];
  }
  ثابت Type& operator [] (صحیح i_)
  {
    unsigned صحیح i = (unsigned صحیح) i_;
    اگر (unlikely (i >= این->len)) بازگشت Crap (Type);
    hb_barrier ();
    بازگشت این+این->arrayZ[i];
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ساختار List16OfOffsetTo *out = c->serializer->embed (*این);
    اگر (unlikely (!out)) return_trace (نادرست);
    unsigned صحیح شمار = این->len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      out->arrayZ[i].serialize_subset (c, این->arrayZ[i], این, out);
    return_trace (درست);
  }

  template <typename ...Ts>
  منطقی sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace ((Array16Of<OffsetTo<Type, OffsetType>>::sanitize (c, این, std::forward<Ts> (ds)...)));
  }
};

template <typename Type>
using List16OfOffset16To = List16OfOffsetTo<Type, HBUINT16>;

/* An array starting at second element. */
template <typename Type, typename LenType>
ساختار HeadlessArrayOf
{
  ایستا constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (HeadlessArrayOf);

  ثابت Type& operator [] (صحیح i_) ثابت
  {
    unsigned صحیح i = (unsigned صحیح) i_;
    اگر (unlikely (i >= lenP1 || !i)) بازگشت Null (Type);
    hb_barrier ();
    بازگشت arrayZ[i-1];
  }
  Type& operator [] (صحیح i_)
  {
    unsigned صحیح i = (unsigned صحیح) i_;
    اگر (unlikely (i >= lenP1 || !i)) بازگشت Crap (Type);
    hb_barrier ();
    بازگشت arrayZ[i-1];
  }
  size_t get_size () ثابت
  { بازگشت hb_unsigned_mul_add_saturate (get_length (), Type::static_size, lenP1.static_size); }

  unsigned get_length () ثابت { بازگشت lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { بازگشت hb_array (arrayZ, get_length ()); }
  hb_array_t<ثابت Type> as_array () ثابت { بازگشت hb_array (arrayZ, get_length ()); }

  /* Iterator. */
  تعریفنوع hb_array_t<ثابت Type>   iter_t;
  تعریفنوع hb_array_t<      Type> writer_t;
    iter_t   iter () ثابت { بازگشت as_array (); }
  writer_t writer ()       { بازگشت as_array (); }
  operator   iter_t () ثابت { بازگشت   iter (); }
  operator writer_t ()       { بازگشت writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { بازگشت arrayZ; }
  ثابت Type *end () ثابت { بازگشت arrayZ + get_length (); }

  HB_NODISCARD منطقی serialize (hb_serialize_context_t *c, unsigned صحیح اقلام_len, منطقی clear = درست)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);
    c->check_assign (lenP1, اقلام_len + 1, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    اگر (unlikely (!c->extend_size (این, get_size (), clear))) return_trace (نادرست);
    return_trace (درست);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD منطقی serialize (hb_serialize_context_t *c, Iterator اقلام)
  {
    TRACE_SERIALIZE (این);
    unsigned شمار = hb_len (اقلام);
    اگر (unlikely (!serialize (c, شمار, نادرست))) return_trace (نادرست);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    برای (unsigned i = 0; i < شمار; i++, ++اقلام)
      arrayZ[i] = *اقلام;
    return_trace (درست);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  منطقی sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!sanitize_shallow (c))) return_trace (نادرست);
    اگر (!اندازه... (Ts) && hb_is_trivially_copyable(Type)) return_trace (درست);
    hb_barrier ();
    unsigned صحیح شمار = get_length ();
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (نادرست);
    return_trace (درست);
  }

  خصوصی:
  منطقی sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (lenP1.sanitize (c) &&
		  hb_barrier () &&
		  (!lenP1 || c->check_array_sized (arrayZ, lenP1 - 1, اندازه (LenType))));
  }

  عمومی:
  LenType	lenP1;
  Type		arrayZ[HB_VAR_ARRAY];
  عمومی:
  DEFINE_SIZE_ARRAY (اندازه (LenType), arrayZ);
};
template <typename Type> using HeadlessArray16Of = HeadlessArrayOf<Type, HBUINT16>;

/* An array storing length-1. */
template <typename Type, typename LenType=HBUINT16>
ساختار ArrayOfM1
{
  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOfM1);

  ثابت Type& operator [] (صحیح i_) ثابت
  {
    unsigned صحیح i = (unsigned صحیح) i_;
    اگر (unlikely (i > lenM1)) بازگشت Null (Type);
    hb_barrier ();
    بازگشت arrayZ[i];
  }
  Type& operator [] (صحیح i_)
  {
    unsigned صحیح i = (unsigned صحیح) i_;
    اگر (unlikely (i > lenM1)) بازگشت Crap (Type);
    hb_barrier ();
    بازگشت arrayZ[i];
  }
  size_t get_size () ثابت
  { بازگشت hb_unsigned_mul_add_saturate (lenM1 + 1, Type::static_size, lenM1.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  منطقی sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!sanitize_shallow (c))) return_trace (نادرست);
    اگر (!اندازه... (Ts) && hb_is_trivially_copyable(Type)) return_trace (درست);
    hb_barrier ();
    unsigned صحیح شمار = lenM1 + 1;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (نادرست);
    return_trace (درست);
  }

  خصوصی:
  منطقی sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (lenM1.sanitize (c) &&
		  hb_barrier () &&
		  (c->check_array_sized (arrayZ, lenM1 + 1, اندازه (LenType))));
  }

  عمومی:
  LenType	lenM1;
  Type		arrayZ[HB_VAR_ARRAY];
  عمومی:
  DEFINE_SIZE_ARRAY (اندازه (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type, typename LenType>
ساختار SortedArrayOf : ArrayOf<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { بازگشت hb_sorted_array (این->arrayZ, این->len); }
  hb_sorted_array_t<ثابت Type> as_array () ثابت { بازگشت hb_sorted_array (این->arrayZ, این->len); }

  /* Iterator. */
  تعریفنوع hb_sorted_array_t<ثابت Type>   iter_t;
  تعریفنوع hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () ثابت { بازگشت as_array (); }
  writer_t writer ()       { بازگشت as_array (); }
  operator   iter_t () ثابت { بازگشت   iter (); }
  operator writer_t ()       { بازگشت writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { بازگشت این->arrayZ; }
  ثابت Type *end () ثابت { بازگشت این->arrayZ + این->len; }

  منطقی serialize (hb_serialize_context_t *c, unsigned صحیح اقلام_len)
  {
    TRACE_SERIALIZE (این);
    منطقی ret = ArrayOf<Type, LenType>::serialize (c, اقلام_len);
    return_trace (ret);
  }
  template <typename Iterator,
	    hb_requires (hb_is_sorted_source_of (Iterator, Type))>
  منطقی serialize (hb_serialize_context_t *c, Iterator اقلام)
  {
    TRACE_SERIALIZE (این);
    منطقی ret = ArrayOf<Type, LenType>::serialize (c, اقلام);
    return_trace (ret);
  }

  SortedArrayOf* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (این);
    SortedArrayOf* out = reinterpret_cast<SortedArrayOf *> (ArrayOf<Type, LenType>::copy (c));
    return_trace (out);
  }

  template <typename T>
  Type &bsearch (ثابت T &x, Type &not_found = Crap (Type))
  { بازگشت *as_array ().bsearch (x, &not_found); }
  template <typename T>
  ثابت Type &bsearch (ثابت T &x, ثابت Type &not_found = Null (Type)) ثابت
  { بازگشت *as_array ().bsearch (x, &not_found); }
  template <typename T>
  منطقی bfind (ثابت T &x, unsigned صحیح *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned صحیح to_store = (unsigned صحیح) -1) ثابت
  { بازگشت as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type> using SortedArray16Of = SortedArrayOf<Type, HBUINT16>;
template <typename Type> using SortedArray24Of = SortedArrayOf<Type, HBUINT24>;
template <typename Type> using SortedArray32Of = SortedArrayOf<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

template <typename LenType=HBUINT16>
ساختار Bin_جستجو_Header
{
  operator uint32_t () ثابت { بازگشت len; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  Bin_جستجو_Header& operator = (unsigned صحیح v)
  {
    len = v;
    أكد (len == v);
    entrySelector = hb_max (1u, hb_bit_storage (v)) - 1;
    جستجو_Range = 16 * (1u << entrySelector);
    rangeShift = v * 16 > جستجو_Range
		 ? 16 * v - جستجو_Range
		 : 0;
    بازگشت *این;
  }

  محافظتشده:
  LenType	len;
  LenType	جستجو_Range;
  LenType	entrySelector;
  LenType	rangeShift;

  عمومی:
  DEFINE_SIZE_STATIC (8);
};

template <typename Type, typename LenType=HBUINT16>
using Bin_جستجو_Array_Of = SortedArrayOf<Type, Bin_جستجو_Header<LenType>>;


ساختار Var_Sized_Bin_جستجو_Header
{

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  HBUINT16	unitSize;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	جستجو_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	entrySelector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	rangeShift;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  عمومی:
  DEFINE_SIZE_STATIC (10);
};

template <typename Type>
ساختار Var_Sized_Bin_جستجو_Array_Of
{
  ایستا constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (Var_Sized_Bin_جستجو_Array_Of);

  منطقی last_is_terminator () ثابت
  {
    اگر (unlikely (!header.nUnits)) بازگشت نادرست;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    ثابت HBUINT16 *واژگان = &StructAtOffset<HBUINT16> (&bytesZ, (header.nUnits - 1) * header.unitSize);
    unsigned صحیح شمار = Type::Termination_Word_شمار;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (واژگان[i] != 0xFFFFu)
	بازگشت نادرست;
    بازگشت درست;
  }

  ثابت Type& operator [] (صحیح i_) ثابت
  {
    unsigned صحیح i = (unsigned صحیح) i_;
    اگر (unlikely (i >= get_length ())) بازگشت Null (Type);
    hb_barrier ();
    بازگشت StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  Type& operator [] (صحیح i_)
  {
    unsigned صحیح i = (unsigned صحیح) i_;
    اگر (unlikely (i >= get_length ())) بازگشت Crap (Type);
    hb_barrier ();
    بازگشت StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  unsigned صحیح get_length () ثابت
  { بازگشت header.nUnits - last_is_terminator (); }
  size_t get_size () ثابت
  { بازگشت hb_unsigned_mul_add_saturate (header.nUnits, header.unitSize, header.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  منطقی sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!sanitize_shallow (c))) return_trace (نادرست);
    اگر (!اندازه... (Ts) && hb_is_trivially_copyable(Type)) return_trace (درست);
    hb_barrier ();
    unsigned صحیح شمار = get_length ();
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (unlikely (!(*این)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (نادرست);
    return_trace (درست);
  }

  template <typename T>
  ثابت Type *bsearch (ثابت T &key) ثابت
  {
    unsigned pos;
    بازگشت hb_bsearch_impl (&pos,
			    key,
			    (ثابت تهی *) bytesZ,
			    get_length (),
			    header.unitSize,
			    _hb_cmp_method<T, Type>)
	   ? (ثابت Type *) (((ثابت نویسه *) &bytesZ) + (pos * header.unitSize))
	   : nullptr;
  }

  خصوصی:
  منطقی sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (header.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_size <= header.unitSize &&
		  c->check_range (bytesZ.arrayZ,
				  header.nUnits,
				  header.unitSize));
  }

  محافظتشده:
  Var_Sized_Bin_جستجو_Header	header;
  UnsizedArrayOf<HBUINT8>	bytesZ;
  عمومی:
  DEFINE_SIZE_ARRAY (10, bytesZ);
};


/* CFF INDEX */

template <typename شمار_3>
ساختار CFFIndex
{
  unsigned صحیح offset_array_size () ثابت
  { بازگشت offSize * (شمار + 1); }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  منطقی serialize (hb_serialize_context_t *c,
		  ثابت Iterable &iterable,
		  ثابت unsigned *p_data_size = nullptr,
                  unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (این);
    unsigned data_size;
    اگر (p_data_size)
      data_size = *p_data_size;
    وگرنه
      total_size (iterable, &data_size);

    auto it = hb_iter (iterable);
    اگر (unlikely (!serialize_header (c, +it, data_size, min_off_size))) return_trace (نادرست);
    unsigned نویسه *ret = c->allocate_size<unsigned نویسه> (data_size, نادرست);
    اگر (unlikely (!ret)) return_trace (نادرست);
    unsigned remaining = data_size;
    برای (ثابت auto &_ : +it)
    {
      unsigned len = _.length;

      اگر (!len)
	ادامه;

      اگر (unlikely (len > remaining)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (نادرست, HB_SERIALIZE_ERROR_INT_OVERFLOW));
      }

      remaining -= len;

      اگر (len <= 1)
      {
	*ret++ = *_.arrayZ;
	ادامه;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (درست);
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  منطقی serialize_header (hb_serialize_context_t *c,
			 Iterator it,
			 unsigned data_size,
                         unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (این);

    unsigned off_size = (hb_bit_storage (data_size + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    /* serialize CFFIndex header */
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);
    این->شمار = hb_len (it);
    اگر (!این->شمار) return_trace (درست);
    اگر (unlikely (!c->extend (این->offSize))) return_trace (نادرست);
    این->offSize = off_size;
    اگر (unlikely (!c->allocate_size<HBUINT8> (off_size * (این->شمار + 1), نادرست)))
      return_trace (نادرست);

    /* serialize indices */
    unsigned صحیح offset = 1;
    اگر (HB_OPTIMIZE_SIZE_VAL)
    {
      unsigned صحیح i = 0;
      برای (ثابت auto &_ : +it)
      {
	set_offset_at (i++, offset);
	offset += hb_len_of (_);
      }
      set_offset_at (i, offset);
    }
    وگرنه
      انتخاب (off_size)
      {
	حالت 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  برای (ثابت auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	بشکن;
	حالت 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  برای (ثابت auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	بشکن;
	حالت 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  برای (ثابت auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	بشکن;
	حالت 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  برای (ثابت auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	بشکن;
	پیشفرض:
	بشکن;
      }

    أكد (offset == data_size + 1);
    return_trace (درست);
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  ایستا unsigned total_size (ثابت Iterable &iterable, unsigned *data_size = nullptr, unsigned min_off_size = 0)
  {
    auto it = + hb_iter (iterable);
    اگر (!it)
    {
      اگر (data_size) *data_size = 0;
      بازگشت min_size;
    }

    unsigned total = 0;
    برای (ثابت auto &_ : +it)
      total += hb_len_of (_);

    اگر (data_size) *data_size = total;

    unsigned off_size = (hb_bit_storage (total + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    بازگشت min_size + HBUINT8::static_size + (hb_len (it) + 1) * off_size + total;
  }

  تهی set_offset_at (unsigned صحیح index, unsigned صحیح offset)
  {
    أكد (index <= شمار);

    unsigned صحیح size = offSize;
    ثابت HBUINT8 *p = offsets;
    انتخاب (size)
    {
      حالت 1: ((HBUINT8  *) p)[index] = offset; بشکن;
      حالت 2: ((HBUINT16 *) p)[index] = offset; بشکن;
      حالت 3: ((HBUINT24 *) p)[index] = offset; بشکن;
      حالت 4: ((HBUINT32 *) p)[index] = offset; بشکن;
      پیشفرض: بازگشت;
    }
  }

  خصوصی:
  unsigned صحیح offset_at (unsigned صحیح index) ثابت
  {
    أكد (index <= شمار);

    unsigned صحیح size = offSize;
    ثابت HBUINT8 *p = offsets;
    انتخاب (size)
    {
      حالت 1: بازگشت ((HBUINT8  *) p)[index];
      حالت 2: بازگشت ((HBUINT16 *) p)[index];
      حالت 3: بازگشت ((HBUINT24 *) p)[index];
      حالت 4: بازگشت ((HBUINT32 *) p)[index];
      پیشفرض: بازگشت 0;
    }
  }

  ثابت unsigned نویسه *data_base () ثابت
  { بازگشت (ثابت unsigned نویسه *) این + min_size + offSize.static_size - 1 + offset_array_size (); }
  عمومی:

  hb_ubytes_t operator [] (unsigned صحیح index) ثابت
  {
    اگر (unlikely (index >= شمار)) بازگشت hb_ubytes_t ();
    hb_barrier ();
    unsigned offset0 = offset_at (index);
    unsigned offset1 = offset_at (index + 1);
    اگر (unlikely (offset1 < offset0 || offset1 > offset_at (شمار)))
      بازگشت hb_ubytes_t ();
    بازگشت hb_ubytes_t (data_base () + offset0, offset1 - offset0);
  }

  size_t get_size () ثابت
  {
    اگر (شمار)
      بازگشت hb_unsigned_add_saturate (min_size, offSize.static_size,
				       offset_array_size (), offset_at (شمار) - 1);
    بازگشت min_size;  /* empty CFFIndex contains count only */
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  hb_barrier () &&
			  (شمار == 0 || /* empty INDEX */
			   (شمار < شمار + 1u &&
			    c->check_struct (&offSize) && offSize >= 1 && offSize <= 4 &&
			    c->check_array (offsets, offSize, شمار + 1u) &&
			    c->check_range (data_base (), offset_at (شمار))))));
  }

  عمومی:
  شمار_3		شمار;		/* Number of object data. Note there are (count+1) offsets */
  خصوصی:
  HBUINT8	offSize;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  عمومی:
  DEFINE_SIZE_MIN (شمار_3::static_size);
};
تعریفنوع CFFIndex<HBUINT16> CFF1Index;
تعریفنوع CFFIndex<HBUINT32> CFF2Index;


/* TupleValues */
ساختار Tuple_مقادیر
{
  شمارش packed_مقدار_flag_t
  {
    مقادیر_ARE_ZEROS     = 0x80,
    مقادیر_ARE_BYTES     = 0x00,
    مقادیر_ARE_واژگان     = 0x40,
    مقادیر_ARE_LONGS     = 0xC0,
    مقادیر_SIZE_MASK     = 0xC0,
    مقدار_RUN_شمار_MASK = 0x3F
  };

  ایستا unsigned compile_unsafe (hb_array_t<ثابت صحیح> values, /* IN */
				  unsigned نویسه *encoded_bytes /* OUT */)
  {
    unsigned رقم_مقادیر_3 = values.length;
    unsigned encoded_len = 0;
    unsigned i = 0;
    وقتی (i < رقم_مقادیر_3)
    {
      صحیح val = values.arrayZ[i];
      اگر (val == 0)
        encoded_len += encode_مقدار_run_as_zeroes (i, encoded_bytes + encoded_len, values);
      وگرنه اگر ((int8_t) val == val)
        encoded_len += encode_مقدار_run_as_bytes (i, encoded_bytes + encoded_len, values);
      وگرنه اگر ((int16_t) val == val)
        encoded_len += encode_مقدار_run_as_واژگان (i, encoded_bytes + encoded_len, values);
      وگرنه
        encoded_len += encode_مقدار_run_as_longs (i, encoded_bytes + encoded_len, values);
    }
    بازگشت encoded_len;
  }

  ایستا unsigned encode_مقدار_run_as_zeroes (unsigned& i,
					      unsigned نویسه *it,
					      hb_array_t<ثابت صحیح> values)
  {
    unsigned رقم_مقادیر_3 = values.length;
    unsigned run_length = 0;
    unsigned encoded_len = 0;
    وقتی (i < رقم_مقادیر_3 && values.arrayZ[i] == 0)
    {
      i++;
      run_length++;
    }

    وقتی (run_length >= 64)
    {
      *it++ = نویسه (مقادیر_ARE_ZEROS | 63);
      run_length -= 64;
      encoded_len++;
    }

    اگر (run_length)
    {
      *it++ = نویسه (مقادیر_ARE_ZEROS | (run_length - 1));
      encoded_len++;
    }
    بازگشت encoded_len;
  }

  ایستا unsigned encode_مقدار_run_as_bytes (unsigned &i,
					     unsigned نویسه *it,
					     hb_array_t<ثابت صحیح> values)
  {
    unsigned start = i;
    unsigned رقم_مقادیر_3 = values.length;
    وقتی (i < رقم_مقادیر_3)
    {
      صحیح val = values.arrayZ[i];
      اگر ((int8_t) val != val)
        بشکن;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      اگر (val == 0 && i + 1 < رقم_مقادیر_3 && values.arrayZ[i+1] == 0)
        بشکن;

      i++;
    }
    unsigned run_length = i - start;

    unsigned encoded_len = 0;

    وقتی (run_length >= 64)
    {
      *it++ = (مقادیر_ARE_BYTES | 63);
      encoded_len++;

      برای (unsigned j = 0; j < 64; j++)
	it[j] = static_cast<نویسه> (values.arrayZ[start + j]);
      it += 64;
      encoded_len += 64;

      start += 64;
      run_length -= 64;
    }

    اگر (run_length)
    {
      *it++ = (مقادیر_ARE_BYTES | (run_length - 1));
      encoded_len++;

      برای (unsigned j = 0; j < run_length; j++)
        it[j] = static_cast<نویسه> (values.arrayZ[start + j]);
      encoded_len += run_length;
    }

    بازگشت encoded_len;
  }

  ایستا unsigned encode_مقدار_run_as_واژگان (unsigned &i,
					     unsigned نویسه *it,
					     hb_array_t<ثابت صحیح> values)
  {
    unsigned start = i;
    unsigned رقم_مقادیر_3 = values.length;
    وقتی (i < رقم_مقادیر_3)
    {
      صحیح val = values.arrayZ[i];

      اگر ((int16_t) val != val)
        بشکن;

      /* start a new run for a single zero value. */
      اگر (val == 0) بشکن;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      اگر ((int8_t) val == val &&
          i + 1 < رقم_مقادیر_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        بشکن;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    وقتی (run_length >= 64)
    {
      *it++ = (مقادیر_ARE_واژگان | 63);
      encoded_len++;

      برای (unsigned j = 0; j < 64; j++)
      {
        int16_t مقدار_val = values.arrayZ[start + j];
        *it++ = static_cast<نویسه> (مقدار_val >> 8);
        *it++ = static_cast<نویسه> (مقدار_val & 0xFF);

        encoded_len += 2;
      }

      start += 64;
      run_length -= 64;
    }

    اگر (run_length)
    {
      *it++ = (مقادیر_ARE_واژگان | (run_length - 1));
      encoded_len++;
      وقتی (start < i)
      {
        int16_t مقدار_val = values.arrayZ[start++];
        *it++ = static_cast<نویسه> (مقدار_val >> 8);
        *it++ = static_cast<نویسه> (مقدار_val & 0xFF);

        encoded_len += 2;
      }
    }
    بازگشت encoded_len;
  }

  ایستا unsigned encode_مقدار_run_as_longs (unsigned &i,
					     unsigned نویسه *it,
					     hb_array_t<ثابت صحیح> values)
  {
    unsigned start = i;
    unsigned رقم_مقادیر_3 = values.length;
    وقتی (i < رقم_مقادیر_3)
    {
      صحیح val = values.arrayZ[i];

      اگر ((int16_t) val == val)
        بشکن;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    وقتی (run_length >= 64)
    {
      *it++ = (مقادیر_ARE_LONGS | 63);
      encoded_len++;

      برای (unsigned j = 0; j < 64; j++)
      {
        int32_t مقدار_val = values.arrayZ[start + j];
        *it++ = static_cast<نویسه> (مقدار_val >> 24);
        *it++ = static_cast<نویسه> (مقدار_val >> 16);
        *it++ = static_cast<نویسه> (مقدار_val >> 8);
        *it++ = static_cast<نویسه> (مقدار_val & 0xFF);

        encoded_len += 4;
      }

      start += 64;
      run_length -= 64;
    }

    اگر (run_length)
    {
      *it++ = (مقادیر_ARE_LONGS | (run_length - 1));
      encoded_len++;
      وقتی (start < i)
      {
        int32_t مقدار_val = values.arrayZ[start++];
        *it++ = static_cast<نویسه> (مقدار_val >> 24);
        *it++ = static_cast<نویسه> (مقدار_val >> 16);
        *it++ = static_cast<نویسه> (مقدار_val >> 8);
        *it++ = static_cast<نویسه> (مقدار_val & 0xFF);

        encoded_len += 4;
      }
    }
    بازگشت encoded_len;
  }

  template <typename T>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ایستا منطقی decompile (ثابت HBUINT8 *&p /* IN/OUT */,
			 hb_vector_t<T> &values /* IN/OUT */,
			 ثابت HBUINT8 *end,
			 منطقی consume_all = نادرست,
			 unsigned start = 0)
  {
    unsigned i = 0;
    unsigned شمار = consume_all ? UINT_MAX : values.length;
    اگر (consume_all)
      values.alloc ((end - p) / 2);
    وقتی (i < شمار)
    {
      اگر (unlikely (p + 1 > end)) بازگشت consume_all;
      unsigned control = *p++;
      unsigned run_شمار = (control & مقدار_RUN_شمار_MASK) + 1;
      اگر (consume_all)
      {
        اگر (unlikely (!values.resize_dirty  (values.length + run_شمار)))
	  بازگشت نادرست;
      }
      unsigned stop = i + run_شمار;
      اگر (unlikely (stop > شمار)) بازگشت نادرست;

      unsigned skip = i < start ? hb_min (start - i, run_شمار) : 0;
      i += skip;

      اگر ((control & مقادیر_SIZE_MASK) == مقادیر_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * اندازه (T));
	i = stop;
      }
      وگرنه اگر ((control & مقادیر_SIZE_MASK) ==  مقادیر_ARE_واژگان)
      {
        اگر (unlikely (p + run_شمار * HBINT16::static_size > end)) بازگشت نادرست;
	p += skip * HBINT16::static_size;
#ifndef HB_OPTIMIZE_SIZE
        برای (; i + 3 < stop; i += 4)
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
        برای (; i < stop; i++)
        {
          values.arrayZ[i] = * (ثابت HBINT16 *) p;
          p += HBINT16::static_size;
        }
      }
      وگرنه اگر ((control & مقادیر_SIZE_MASK) ==  مقادیر_ARE_LONGS)
      {
        اگر (unlikely (p + run_شمار * HBINT32::static_size > end)) بازگشت نادرست;
	p += skip * HBINT32::static_size;
        برای (; i < stop; i++)
        {
          values.arrayZ[i] = * (ثابت HBINT32 *) p;
          p += HBINT32::static_size;
        }
      }
      وگرنه اگر ((control & مقادیر_SIZE_MASK) ==  مقادیر_ARE_BYTES)
      {
        اگر (unlikely (p + run_شمار > end)) بازگشت نادرست;
	p += skip * HBINT8::static_size;
#ifndef HB_OPTIMIZE_SIZE
	برای (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (ثابت HBINT8 *) p++;
	}
#endif
        برای (; i < stop; i++)
          values.arrayZ[i] = * (ثابت HBINT8 *) p++;
      }
    }
    بازگشت درست;
  }

  ایستا منطقی skip (ثابت unsigned نویسه *&p /* IN/OUT */,
		    ثابت unsigned نویسه *end,
		    unsigned شمار)
  {
    unsigned seen = 0;
    وقتی (seen < شمار)
    {
      اگر (unlikely (p >= end)) بازگشت نادرست;
      unsigned control = *p++;
      unsigned run_شمار = (control & مقدار_RUN_شمار_MASK) + 1;
      اگر (unlikely (run_شمار > شمار - seen)) بازگشت نادرست;

      unsigned width = 0;
      انتخاب (control & مقادیر_SIZE_MASK)
      {
	حالت مقادیر_ARE_ZEROS: بشکن;
	حالت مقادیر_ARE_BYTES: width = HBINT8::static_size; بشکن;
	حالت مقادیر_ARE_واژگان: width = HBINT16::static_size; بشکن;
	حالت مقادیر_ARE_LONGS: width = HBINT32::static_size; بشکن;
      }
      اگر (unlikely (unsigned (end - p) < run_شمار * width)) بازگشت نادرست;
      p += run_شمار * width;
      seen += run_شمار;
    }
    بازگشت درست;
  }

  ساختار iter_t : hb_iter_with_fallback_t<iter_t, صحیح>
  {
    iter_t (ثابت unsigned نویسه *p_, unsigned len_)
	    : p (p_), endp (p_ + len_)
    { اگر (likely (ensure_run ())) read_مقدار (); }

    خصوصی:
    ثابت unsigned نویسه *p;
    ثابت unsigned نویسه * ثابت endp;
    صحیح current_مقدار = 0;
    signed run_شمار = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    منطقی ensure_run ()
    {
      اگر (likely (run_شمار > 0)) بازگشت درست;
      بازگشت _ensure_run ();
    }
    منطقی _ensure_run ()
    {
      اگر (unlikely (p >= endp))
      {
        run_شمار = 0;
        current_مقدار = 0;
	بازگشت نادرست;
      }

      unsigned control = *p++;
      run_شمار = (control & مقدار_RUN_شمار_MASK) + 1;
      width = control & مقادیر_SIZE_MASK;
      انتخاب (width)
      {
        حالت مقادیر_ARE_ZEROS: width = 0; بشکن;
	حالت مقادیر_ARE_BYTES: width = HBINT8::static_size;  بشکن;
	حالت مقادیر_ARE_واژگان: width = HBINT16::static_size; بشکن;
	حالت مقادیر_ARE_LONGS: width = HBINT32::static_size; بشکن;
	پیشفرض: أكد (نادرست);
      }

      اگر (unlikely (p + run_شمار * width > endp))
      {
	run_شمار = 0;
	current_مقدار = 0;
	بازگشت نادرست;
      }

      بازگشت درست;
    }
    تهی read_مقدار ()
    {
      انتخاب (width)
      {
        حالت 0: current_مقدار = 0; بشکن;
	حالت 1: current_مقدار = * (ثابت HBINT8  *) p; بشکن;
	حالت 2: current_مقدار = * (ثابت HBINT16 *) p; بشکن;
	حالت 4: current_مقدار = * (ثابت HBINT32 *) p; بشکن;
      }
      p += width;
    }

    عمومی:

    تعریفنوع صحیح __item_t__;
    __item_t__ __item__ () ثابت
    { بازگشت current_مقدار; }

    منطقی __more__ () ثابت { بازگشت run_شمار || p < endp; }
    تهی __next__ ()
    {
      run_شمار--;
      اگر (unlikely (!ensure_run ()))
	بازگشت;
      read_مقدار ();
    }
    تهی __forward__ (unsigned n)
    {
      اگر (unlikely (!ensure_run ()))
	بازگشت;
      وقتی (n)
      {
	unsigned i = hb_min (n, (unsigned) run_شمار);
	run_شمار -= i;
	n -= i;
	p += (i - 1) * width;
	اگر (unlikely (!ensure_run ()))
	  بازگشت;
	read_مقدار ();
      }
    }
    منطقی operator != (ثابت iter_t& o) ثابت
    { بازگشت p != o.p || run_شمار != o.run_شمار; }
    iter_t __end__ () ثابت
    {
      iter_t it (endp, 0);
      بازگشت it;
    }
  };

  ساختار fetcher_t
  {
    fetcher_t (ثابت unsigned نویسه *p_, unsigned len_)
	      : p (p_), end (p_ + len_) {}

    خصوصی:
    ثابت unsigned نویسه *p;
    ثابت unsigned نویسه * ثابت end;
    signed run_شمار = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    منطقی ensure_run ()
    {
      اگر (likely (run_شمار > 0)) بازگشت درست;
      بازگشت _ensure_run ();
    }

    منطقی _ensure_run ()
    {
      اگر (unlikely (p >= end))
      {
        run_شمار = 0;
	بازگشت نادرست;
      }

      unsigned control = *p++;
      run_شمار = (control & مقدار_RUN_شمار_MASK) + 1;
      width = control & مقادیر_SIZE_MASK;
      انتخاب (width)
      {
        حالت مقادیر_ARE_ZEROS: width = 0; بشکن;
	حالت مقادیر_ARE_BYTES: width = HBINT8::static_size;  بشکن;
	حالت مقادیر_ARE_واژگان: width = HBINT16::static_size; بشکن;
	حالت مقادیر_ARE_LONGS: width = HBINT32::static_size; بشکن;
	پیشفرض: أكد (نادرست);
      }

      اگر (unlikely (p + run_شمار * width > end))
      {
	run_شمار = 0;
	بازگشت نادرست;
      }

      بازگشت درست;
    }

    عمومی:
    تهی skip (unsigned n)
    {
      وقتی (n)
      {
	اگر (unlikely (!ensure_run ()))
	  بازگشت;
	unsigned i = hb_min (n, (unsigned) run_شمار);
	run_شمار -= i;
	n -= i;
	p += i * width;
      }
    }

    خصوصی:
    template <منطقی scaled, typename T>
    تهی _add_to (hb_array_t<T> out, T scale = T (1))
    {
      unsigned n = out.length;
      T *arrayZ = out.arrayZ;

      برای (unsigned i = 0; i < n;)
      {
	اگر (unlikely (!ensure_run ()))
	  بشکن;
	unsigned شمار = hb_min (n - i, (unsigned) run_شمار);
	انتخاب (width)
	{
	  حالت 0:
	  {
	    arrayZ += شمار;
	    بشکن;
	  }
	  حالت 1:
	  {
	    ثابت auto *pp = (ثابت HBINT8 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    برای (; j + 3 < شمار; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    برای (; j < شمار; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت unsigned نویسه *) pp;
	  }
	  بشکن;
	  حالت 2:
	  {
	    ثابت auto *pp = (ثابت HBINT16 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    برای (; j + 3 < شمار; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    برای (; j < شمار; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت unsigned نویسه *) pp;
	  }
	  بشکن;
	  حالت 4:
	  {
	    ثابت auto *pp = (ثابت HBINT32 *) p;
	    برای (unsigned j = 0; j < شمار; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت unsigned نویسه *) pp;
	  }
	  بشکن;
	}
	run_شمار -= شمار;
	i += شمار;
      }
    }

    عمومی:
    template <typename T>
    تهی add_to (hb_array_t<T> out, اعشاری scale = 1.0f)
    {
#ifndef HB_OPTIMIZE_SIZE
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      اگر (نادرست && scale == 1.0f)
        _add_to<نادرست> (out);
      وگرنه
#endif
        _add_to<درست> (out, (T) scale);
    }
  };
};

ساختار TupleList : CFF2Index
{
  Tuple_مقادیر::iter_t operator [] (unsigned i) ثابت
  {
    auto bytes = CFF2Index::operator [] (i);
    بازگشت Tuple_مقادیر::iter_t (bytes.arrayZ, bytes.length);
  }

  Tuple_مقادیر::fetcher_t fetcher (unsigned i) ثابت
  {
    auto bytes = CFF2Index::operator [] (i);
    بازگشت Tuple_مقادیر::fetcher_t (bytes.arrayZ, bytes.length);
  }
};


// Alignment

template <unsigned صحیح alignment>
ساختار Align
{
  size_t get_size (ثابت تهی *base) ثابت
  {
    unsigned offset = (ثابت نویسه *) این - (ثابت نویسه *) base;
    بازگشت (alignment - offset) & (alignment - 1);
  }

  عمومی:
  DEFINE_SIZE_MIN (0);
};



} /* namespace OT */


#endif /* HB_OPEN_TYPE_HH */
