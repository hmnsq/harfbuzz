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

#include "hb.hh"
#include "hb-blob.hh"
#include "hb-face.hh"
#include "hb-machinery.hh"
#include "hb-meta.hh"
#include "hb-subset.hh"


namespace OT {


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
template <bool BE,
	  typename Type,
	  unsigned ကိန်းပြည့် Size = parimaa_na (Type)>
sa_nracanaa NumType
{
  typedef Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  typedef typename std::conditional<std::is_integral<Type>::value && parimaa_na (Type) <= parimaa_na(ကိန်းပြည့်),
				     typename std::conditional<std::is_signed<Type>::value, signed, unsigned>::type,
				     Type>::type WideType;

  NumType () = default;
  explicit constexpr NumType (Type V) : v {V} {}
  NumType& operator = (Type V) { v = V; ပြန်ပေး *this; }

  operator WideType () const { ပြန်ပေး v; }

  bool operator == (const NumType &o) const { ပြန်ပေး (Type) v == (Type) o.v; }
  bool operator != (const NumType &o) const { ပြန်ပေး !(*this == o); }

  NumType& operator += (WideType count) { *this = *this + count; ပြန်ပေး *this; }
  NumType& operator -= (WideType count) { *this = *this - count; ပြန်ပေး *this; }
  NumType& operator ++ () { *this += 1; ပြန်ပေး *this; }
  NumType& operator -- () { *this -= 1; ပြန်ပေး *this; }
  NumType operator ++ (ကိန်းပြည့်) { NumType c (*this); ++*this; ပြန်ပေး c; }
  NumType operator -- (ကိန်းပြည့်) { NumType c (*this); --*this; ပြန်ပေး c; }

  uint32_t hash () const { ပြန်ပေး hb_array ((const var_na *) &v, parimaa_na (v)).hash (); }
  HB_INTERNAL static ကိန်းပြည့် cmp (const NumType *a, const NumType *b)
  { ပြန်ပေး b->cmp (*a); }
  HB_INTERNAL static ကိန်းပြည့် cmp (const void *a, const void *b)
  {
    NumType *pa = (NumType *) a;
    NumType *pb = (NumType *) b;

    ပြန်ပေး pb->cmp (*pa);
  }
  template <typename Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  ကိန်းပြည့် cmp (Type2 a) const
  {
    Type b = v;
    ပြန်ပေး (a > b) - (a < b);
  }
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }
  rak_sita:
  typename std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, Size>,
			    HBFloat<BE, Type, Size>>::type v;
  public:
  DEFINE_SIZE_STATIC (Size);
};

typedef NumType<true, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
typedef NumType<true, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
typedef NumType<true, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
typedef NumType<true, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
typedef NumType<true, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
typedef NumType<true, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
typedef NumType<true, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
typedef NumType<true, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
typedef NumType<true, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

typedef NumType<false, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
typedef NumType<false, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
typedef NumType<false, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
typedef NumType<false, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
typedef NumType<false, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
typedef NumType<false, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

typedef NumType<true,  float>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
typedef NumType<true,  dviguu_na> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
typedef NumType<false, float>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
typedef NumType<false, dviguu_na> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
sa_nracanaa HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& operator = (uint16_t i ) { HBUINT16::operator= (i); ပြန်ပေး *this; }
  public:
  DEFINE_SIZE_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
sa_nracanaa HBUINT32VAR
{
  size_t get_size () const
  {
    unsigned b0 = v[0];
    အကယ်၍ (b0 < 0x80)
      ပြန်ပေး 1;
    မဟုတ်ပါက အကယ်၍ (b0 < 0xC0)
      ပြန်ပေး 2;
    မဟုတ်ပါက အကယ်၍ (b0 < 0xE0)
      ပြန်ပေး 3;
    မဟုတ်ပါက အကယ်၍ (b0 < 0xF0)
      ပြန်ပေး 4;
    မဟုတ်ပါက
      ပြန်ပေး 5;
  }

  static size_t get_size (uint32_t v)
  {
    အကယ်၍ (v < 0x80)
      ပြန်ပေး 1;
    မဟုတ်ပါက အကယ်၍ (v < 0x4000)
      ပြန်ပေး 2;
    မဟုတ်ပါက အကယ်၍ (v < 0x200000)
      ပြန်ပေး 3;
    မဟုတ်ပါက အကယ်၍ (v < 0x10000000)
      ပြန်ပေး 4;
    မဟုတ်ပါက
      ပြန်ပေး 5;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_size ()));
  }

  operator uint32_t () const
  {
    unsigned b0 = v[0];
    အကယ်၍ (b0 < 0x80)
      ပြန်ပေး b0;
    မဟုတ်ပါက အကယ်၍ (b0 < 0xC0)
      ပြန်ပေး ((b0 & 0x3F) << 8) | v[1];
    မဟုတ်ပါက အကယ်၍ (b0 < 0xE0)
      ပြန်ပေး ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    မဟုတ်ပါက အကယ်၍ (b0 < 0xF0)
      ပြန်ပေး ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    မဟုတ်ပါက
      ပြန်ပေး (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  static unsigned serialize_unsafe (unsigned var_na *buf, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned var_na *p = buf + len;
    အတွက် (unsigned i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    အကယ်၍ (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    ပြန်ပေး len;
  }

  static bool serialize (hb_serialize_context_t *c, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned var_na *buf = c->allocate_size<unsigned var_na> (len, false);
    အကယ်၍ (unlikely (!buf))
      ပြန်ပေး false;

    serialize_unsafe (buf, v);
    ပြန်ပေး true;
  }

  rak_sita:
  unsigned var_na v[5];

  public:
  DEFINE_SIZE_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
typedef HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
typedef HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
typedef HBUINT16 UFWORD;

template <typename Type, unsigned fraction_bits>
sa_nracanaa HBFixed : Type
{
  static constexpr float mult = 1.f / (1 << fraction_bits);
  static_assert (Type::static_size * 8 > fraction_bits, "");

  operator signed () const = delete;
  operator unsigned () const = delete;
  explicit operator float () const { ပြန်ပေး to_float (); }
  typename Type::type to_int () const { ပြန်ပေး Type::v; }
  void set_int (typename Type::type i ) { Type::v = i; }
  float to_float (float offset = 0) const  { ပြန်ပေး ((int32_t) Type::v + offset) * mult; }
  void set_float (float f) { Type::v = roundf (f / mult); }
  public:
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
sa_nracanaa LONGDATETIME
{
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }
  rak_sita:
  HBINT32 major;
  HBUINT32 minor;
  public:
  DEFINE_SIZE_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
sa_nracanaa Tag : HBUINT32
{
  Tag& operator = (hb_tag_t i) { HBUINT32::operator= (i); ပြန်ပေး *this; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  operator const var_na* () const { ပြန်ပေး reinterpret_cast<const var_na *> (this); }
  operator var_na* ()             { ပြန်ပေး reinterpret_cast<var_na *> (this); }
  public:
  DEFINE_SIZE_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
sa_nracanaa HBGlyphID16 : HBUINT16
{
  HBGlyphID16& operator = (uint16_t i) { HBUINT16::operator= (i); ပြန်ပေး *this; }
};
sa_nracanaa HBGlyphID24 : HBUINT24
{
  HBGlyphID24& operator = (uint32_t i) { HBUINT24::operator= (i); ပြန်ပေး *this; }
};

/* Script/language-system/feature index */
sa_nracanaa Index : HBUINT16 {
  static constexpr unsigned NOT_FOUND_INDEX = 0xFFFFu;
  Index& operator = (uint16_t i) { HBUINT16::operator= (i); ပြန်ပေး *this; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, Index);

typedef Index NameID;

sa_nracanaa VarIdx : HBUINT32 {
  static constexpr unsigned NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_INDEX, "");
  VarIdx& operator = (uint32_t i) { HBUINT32::operator= (i); ပြန်ပေး *this; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, VarIdx);

/* Offset, Null offset = 0 */
template <typename Type, bool has_null=true>
sa_nracanaa Offset : Type
{
  Offset& operator = (typename Type::type i) { Type::operator= (i); ပြန်ပေး *this; }

  typedef Type type;

  bool is_null () const { ပြန်ပေး has_null && 0 == *this; }

  public:
  DEFINE_SIZE_STATIC (parimaa_na (Type));
};

typedef Offset<HBUINT16> Offset16;
typedef Offset<HBUINT24> Offset24;
typedef Offset<HBUINT32> Offset32;


/* CheckSum */
sa_nracanaa CheckSum : HBUINT32
{
  CheckSum& operator = (uint32_t i) { HBUINT32::operator= (i); ပြန်ပေး *this; }

  /* This is reference implementation from the spec. */
  static uint32_t CalcTableChecksum (const HBUINT32 *Table, uint32_t Length)
  {
    uint32_t Sum = 0L;
    assert (0 == (Length & 3));
    const HBUINT32 *EndPtr = Table + Length / HBUINT32::static_size;

    နေစဉ် (Table < EndPtr)
      Sum += *Table++;
    ပြန်ပေး Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  void set_for_data (const void *data, unsigned ကိန်းပြည့် length)
  { *this = CalcTableChecksum ((const HBUINT32 *) data, length); }

  public:
  DEFINE_SIZE_STATIC (4);
};


/*
 * Version Numbers
 */

template <typename FixedType=HBUINT16>
sa_nracanaa FixedVersion
{
  uint32_t to_int () const { ပြန်ပေး (major << (parimaa_na (FixedType) * 8)) + minor; }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  FixedType major;
  FixedType minor;
  public:
  DEFINE_SIZE_STATIC (2 * parimaa_na (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

template <typename Type, bool has_null>
sa_nracanaa _hb_has_null
{
  static const Type *get_null () { ပြန်ပေး nullptr; }
  static Type *get_crap ()       { ပြန်ပေး nullptr; }
};
template <typename Type>
sa_nracanaa _hb_has_null<Type, true>
{
  static const Type *get_null () { ပြန်ပေး &Null (Type); }
  static       Type *get_crap () { ပြန်ပေး &Crap (Type); }
};

template <typename Type, typename OffsetType, typename BaseType=void, bool has_null=true>
sa_nracanaa OffsetTo : Offset<OffsetType, has_null>
{
  using target_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == false ||
		 (hb_has_null_size (Type) || !hb_has_min_size (Type)), "");

  HB_DELETE_COPY_ASSIGN (OffsetTo);
  OffsetTo () = default;

  OffsetTo& operator = (typename OffsetType::type i) { OffsetType::operator= (i); ပြန်ပေး *this; }

  const Type& operator () (const void *base) const
  {
    အကယ်၍ (unlikely (this->is_null ())) ပြန်ပေး *_hb_has_null<Type, has_null>::get_null ();
    ပြန်ပေး StructAtOffset<const Type> (base, *this);
  }
  Type& operator () (void *base) const
  {
    အကယ်၍ (unlikely (this->is_null ())) ပြန်ပေး *_hb_has_null<Type, has_null>::get_crap ();
    ပြန်ပေး StructAtOffset<Type> (base, *this);
  }

  template <typename Base,
	    hb_enable_if (hb_is_convertible (const Base, const BaseType *))>
  friend const Type& operator + (const Base &base, const OffsetTo &offset) { ပြန်ပေး offset ((const void *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (const Base, const BaseType *))>
  friend const Type& operator + (const OffsetTo &offset, const Base &base) { ပြန်ပေး offset ((const void *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (Base &&base, OffsetTo &offset) { ပြန်ပေး offset ((void *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (OffsetTo &offset, Base &&base) { ပြန်ပေး offset ((void *) base); }


  template <typename Base, typename ...Ts>
  bool serialize_subset (hb_subset_context_t *c, const OffsetTo& src,
			 const Base *src_base, Ts&&... ds)
  {
    *this = 0;
    အကယ်၍ (src.is_null ())
      ပြန်ပေး false;

    auto *s = c->serializer;

    s->push ();

    bool ret = c->dispatch (src_base+src, std::forward<Ts> (ds)...);

    အကယ်၍ (ret || !has_null)
      s->add_link (*this, s->pop_pack ());
    မဟုတ်ပါက
      s->pop_discard ();

    ပြန်ပေး ret;
  }


  template <typename ...Ts>
  bool serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *this = 0;

    Type* obj = c->push<Type> ();
    bool ret = obj->serialize (c, std::forward<Ts> (ds)...);

    အကယ်၍ (ret)
      c->add_link (*this, c->pop_pack ());
    မဟုတ်ပါက
      c->pop_discard ();

    ပြန်ပေး ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  template <typename ...Ts>
  bool serialize_copy (hb_serialize_context_t *c, const OffsetTo& src,
		       const void *src_base, unsigned dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *this = 0;
    အကယ်၍ (src.is_null ())
      ပြန်ပေး false;

    c->push ();

    bool ret = c->copy (src_base+src, std::forward<Ts> (ds)...);

    c->add_link (*this, c->pop_pack (), whence, dst_bias);

    ပြန်ပေး ret;
  }

  bool serialize_copy (hb_serialize_context_t *c, const OffsetTo& src,
		       const void *src_base, unsigned dst_bias = 0)
  { ပြန်ပေး serialize_copy (c, src, src_base, dst_bias, hb_serialize_context_t::Head); }

  bool sanitize_shallow (hb_sanitize_context_t *c, const BaseType *base) const
  {
    TRACE_SANITIZE (this);
    အကယ်၍ (unlikely (!c->check_struct (this))) return_trace (false);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    အကယ်၍ (unlikely ((const var_na *) base + (unsigned) *this < (const var_na *) base)) return_trace (false);
    return_trace (true);
  }

  template <typename ...Ts>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  bool sanitize (hb_sanitize_context_t *c, const BaseType *base, Ts&&... ds) const
  {
    TRACE_SANITIZE (this);
    return_trace (sanitize_shallow (c, base) &&
		  hb_barrier () &&
		  (this->is_null () ||
		   c->dispatch (StructAtOffset<Type> (base, *this), std::forward<Ts> (ds)...)));
  }

  DEFINE_SIZE_STATIC (parimaa_na (OffsetType));
};
/* Partial specializations. */
template <typename Type, typename BaseType=void, bool has_null=true> using Offset16To = OffsetTo<Type, HBUINT16, BaseType, has_null>;
template <typename Type, typename BaseType=void, bool has_null=true> using Offset24To = OffsetTo<Type, HBUINT24, BaseType, has_null>;
template <typename Type, typename BaseType=void, bool has_null=true> using Offset32To = OffsetTo<Type, HBUINT32, BaseType, has_null>;

template <typename Type, typename OffsetType, typename BaseType=void> using NNOffsetTo = OffsetTo<Type, OffsetType, BaseType, false>;
template <typename Type, typename BaseType=void> using NNOffset16To = Offset16To<Type, BaseType, false>;
template <typename Type, typename BaseType=void> using NNOffset24To = Offset24To<Type, BaseType, false>;
template <typename Type, typename BaseType=void> using NNOffset32To = Offset32To<Type, BaseType, false>;


/*
 * Array Types
 */

template <typename Type>
sa_nracanaa UnsizedArrayOf
{
  typedef Type item_t;
  static constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (UnsizedArrayOf);

  const Type& operator [] (unsigned ကိန်းပြည့် i) const
  {
    ပြန်ပေး arrayZ[i];
  }
  Type& operator [] (unsigned ကိန်းပြည့် i)
  {
    ပြန်ပေး arrayZ[i];
  }

  static size_t get_size (unsigned ကိန်းပြည့် len)
  { ပြန်ပေး hb_unsigned_mul_saturate (len, Type::static_size); }

  template <typename T> operator T * () { ပြန်ပေး arrayZ; }
  template <typename T> operator const T * () const { ပြန်ပေး arrayZ; }
  hb_array_t<Type> as_array (unsigned ကိန်းပြည့် len)
  { ပြန်ပေး hb_array (arrayZ, len); }
  hb_array_t<const Type> as_array (unsigned ကိန်းပြည့် len) const
  { ပြန်ပေး hb_array (arrayZ, len); }

  template <typename T>
  Type &lsearch (unsigned ကိန်းပြည့် len, const T &x, Type &not_found = Crap (Type))
  { ပြန်ပေး *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  const Type &lsearch (unsigned ကိန်းပြည့် len, const T &x, const Type &not_found = Null (Type)) const
  { ပြန်ပေး *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  bool lfind (unsigned ကိန်းပြည့် len, const T &x, unsigned ကိန်းပြည့် *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ကိန်းပြည့် to_store = (unsigned ကိန်းပြည့်) -1) const
  { ပြန်ပေး as_array (len).lfind (x, i, not_found, to_store); }

  void qsort (unsigned ကိန်းပြည့် len, unsigned ကိန်းပြည့် start = 0, unsigned ကိန်းပြည့် end = (unsigned ကိန်းပြည့်) -1)
  { as_array (len).qsort (start, end); }

  bool serialize (hb_serialize_context_t *c, unsigned ကိန်းပြည့် items_len, bool clear = true)
  {
    TRACE_SERIALIZE (this);
    အကယ်၍ (unlikely (!c->extend_size (this, get_size (items_len), clear))) return_trace (false);
    return_trace (true);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  bool serialize (hb_serialize_context_t *c, Iterator items)
  {
    TRACE_SERIALIZE (this);
    unsigned count = hb_len (items);
    အကယ်၍ (unlikely (!serialize (c, count, false))) return_trace (false);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    အတွက် (unsigned i = 0; i < count; i++, ++items)
      arrayZ[i] = *items;
    return_trace (true);
  }

  UnsizedArrayOf* copy (hb_serialize_context_t *c, unsigned count) const
  {
    TRACE_SERIALIZE (this);
    auto *out = c->start_embed (this);
    အကယ်၍ (unlikely (!as_array (count).copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  bool sanitize (hb_sanitize_context_t *c, unsigned ကိန်းပြည့် count, Ts&&... ds) const
  {
    TRACE_SANITIZE (this);
    အကယ်၍ (unlikely (!sanitize_shallow (c, count))) return_trace (false);
    အကယ်၍ (!parimaa_na... (Ts) && hb_is_trivially_copyable(Type)) return_trace (true);
    hb_barrier ();
    အတွက် (unsigned ကိန်းပြည့် i = 0; i < count; i++)
      အကယ်၍ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (false);
    return_trace (true);
  }

  bool sanitize_shallow (hb_sanitize_context_t *c, unsigned ကိန်းပြည့် count) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_array (arrayZ, count));
  }

  public:
  Type		arrayZ[HB_VAR_ARRAY];
  public:
  DEFINE_SIZE_UNBOUNDED (0);
};

/* Unsized array of offset's */
template <typename Type, typename OffsetType, typename BaseType=void, bool has_null=true>
using UnsizedArray16OfOffsetTo = UnsizedArrayOf<OffsetTo<Type, OffsetType, BaseType, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
template <typename Type, typename OffsetType, typename BaseType=void, bool has_null=true>
sa_nracanaa UnsizedListOfOffset16To : UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
{
  const Type& operator [] (ကိန်းပြည့် i_) const
  {
    unsigned ကိန်းပြည့် i = (unsigned ကိန်းပြည့်) i_;
    const OffsetTo<Type, OffsetType, BaseType, has_null> *p = &this->arrayZ[i];
    အကယ်၍ (unlikely ((const void *) p < (const void *) this->arrayZ)) ပြန်ပေး Null (Type); /* Overflowed. */
    hb_barrier ();
    ပြန်ပေး this+*p;
  }
  Type& operator [] (ကိန်းပြည့် i_)
  {
    unsigned ကိန်းပြည့် i = (unsigned ကိန်းပြည့်) i_;
    const OffsetTo<Type, OffsetType, BaseType, has_null> *p = &this->arrayZ[i];
    အကယ်၍ (unlikely ((const void *) p < (const void *) this->arrayZ)) ပြန်ပေး Crap (Type); /* Overflowed. */
    hb_barrier ();
    ပြန်ပေး this+*p;
  }

  template <typename ...Ts>
  bool sanitize (hb_sanitize_context_t *c, unsigned ကိန်းပြည့် count, Ts&&... ds) const
  {
    TRACE_SANITIZE (this);
    return_trace ((UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
		   ::sanitize (c, count, this, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type>
sa_nracanaa SortedUnsizedArrayOf : UnsizedArrayOf<Type>
{
  hb_sorted_array_t<Type> as_array (unsigned ကိန်းပြည့် len)
  { ပြန်ပေး hb_sorted_array (this->arrayZ, len); }
  hb_sorted_array_t<const Type> as_array (unsigned ကိန်းပြည့် len) const
  { ပြန်ပေး hb_sorted_array (this->arrayZ, len); }
  operator hb_sorted_array_t<Type> ()             { ပြန်ပေး as_array (); }
  operator hb_sorted_array_t<const Type> () const { ပြန်ပေး as_array (); }

  template <typename T>
  Type &bsearch (unsigned ကိန်းပြည့် len, const T &x, Type &not_found = Crap (Type))
  { ပြန်ပေး *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  const Type &bsearch (unsigned ကိန်းပြည့် len, const T &x, const Type &not_found = Null (Type)) const
  { ပြန်ပေး *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  bool bfind (unsigned ကိန်းပြည့် len, const T &x, unsigned ကိန်းပြည့် *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ကိန်းပြည့် to_store = (unsigned ကိန်းပြည့်) -1) const
  { ပြန်ပေး as_array (len).bfind (x, i, not_found, to_store); }
};


/* An array with a number of elements. */
template <typename Type, typename LenType>
sa_nracanaa ArrayOf
{
  typedef Type item_t;
  static constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOf);

  const Type& operator [] (ကိန်းပြည့် i_) const
  {
    unsigned ကိန်းပြည့် i = (unsigned ကိန်းပြည့်) i_;
    အကယ်၍ (unlikely (i >= len)) ပြန်ပေး Null (Type);
    hb_barrier ();
    ပြန်ပေး arrayZ[i];
  }
  Type& operator [] (ကိန်းပြည့် i_)
  {
    unsigned ကိန်းပြည့် i = (unsigned ကိန်းပြည့်) i_;
    အကယ်၍ (unlikely (i >= len)) ပြန်ပေး Crap (Type);
    hb_barrier ();
    ပြန်ပေး arrayZ[i];
  }

  size_t get_size () const
  { ပြန်ပေး hb_unsigned_mul_add_saturate (len, Type::static_size, len.static_size); }

  explicit operator bool () const { ပြန်ပေး len; }

  void pop () { len--; }

  hb_array_t<      Type> as_array ()       { ပြန်ပေး hb_array (arrayZ, len); }
  hb_array_t<const Type> as_array () const { ပြန်ပေး hb_array (arrayZ, len); }

  /* Iterator. */
  typedef hb_array_t<const Type>   iter_t;
  typedef hb_array_t<      Type> writer_t;
    iter_t   iter () const { ပြန်ပေး as_array (); }
  writer_t writer ()       { ပြန်ပေး as_array (); }
  operator   iter_t () const { ပြန်ပေး   iter (); }
  operator writer_t ()       { ပြန်ပေး writer (); }

  /* Faster range-based for loop. */
  const Type *begin () const { ပြန်ပေး arrayZ; }
  const Type *end () const { ပြန်ပေး arrayZ + len; }

  template <typename T>
  Type &lsearch (const T &x, Type &not_found = Crap (Type))
  { ပြန်ပေး *as_array ().lsearch (x, &not_found); }
  template <typename T>
  const Type &lsearch (const T &x, const Type &not_found = Null (Type)) const
  { ပြန်ပေး *as_array ().lsearch (x, &not_found); }
  template <typename T>
  bool lfind (const T &x, unsigned ကိန်းပြည့် *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ကိန်းပြည့် to_store = (unsigned ကိန်းပြည့်) -1) const
  { ပြန်ပေး as_array ().lfind (x, i, not_found, to_store); }

  void qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD bool serialize (hb_serialize_context_t *c, unsigned items_len, bool clear = true)
  {
    TRACE_SERIALIZE (this);
    အကယ်၍ (unlikely (!c->extend_min (this))) return_trace (false);
    c->check_assign (len, items_len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    အကယ်၍ (unlikely (!c->extend_size (this, get_size (), clear))) return_trace (false);
    return_trace (true);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD bool serialize (hb_serialize_context_t *c, Iterator items)
  {
    TRACE_SERIALIZE (this);
    unsigned count = hb_len (items);
    အကယ်၍ (unlikely (!serialize (c, count, false))) return_trace (false);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    အတွက် (unsigned i = 0; i < count; i++, ++items)
      arrayZ[i] = *items;
    return_trace (true);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (this);
    len++;
    အကယ်၍ (unlikely (!len || !c->extend (this)))
    {
      len--;
      return_trace (nullptr);
    }
    return_trace (&arrayZ[len - 1]);
  }

  ArrayOf* copy (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    auto *out = c->start_embed (this);
    အကယ်၍ (unlikely (!c->extend_min (out))) return_trace (nullptr);
    c->check_assign (out->len, len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    အကယ်၍ (unlikely (!as_array ().copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  bool sanitize (hb_sanitize_context_t *c, Ts&&... ds) const
  {
    TRACE_SANITIZE (this);
    အကယ်၍ (unlikely (!sanitize_shallow (c))) return_trace (false);
    အကယ်၍ (!parimaa_na... (Ts) && hb_is_trivially_copyable(Type)) return_trace (true);
    hb_barrier ();
    unsigned ကိန်းပြည့် count = len;
    အတွက် (unsigned ကိန်းပြည့် i = 0; i < count; i++)
      အကယ်၍ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (false);
    return_trace (true);
  }

  bool sanitize_shallow (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (len.sanitize (c) &&
		  hb_barrier () &&
		  c->check_array_sized (arrayZ, len, parimaa_na (LenType)));
  }

  public:
  LenType	len;
  Type		arrayZ[HB_VAR_ARRAY];
  public:
  DEFINE_SIZE_ARRAY (parimaa_na (LenType), arrayZ);
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
sa_nracanaa List16OfOffsetTo : ArrayOf<OffsetTo<Type, OffsetType>, HBUINT16>
{
  const Type& operator [] (ကိန်းပြည့် i_) const
  {
    unsigned ကိန်းပြည့် i = (unsigned ကိန်းပြည့်) i_;
    အကယ်၍ (unlikely (i >= this->len)) ပြန်ပေး Null (Type);
    hb_barrier ();
    ပြန်ပေး this+this->arrayZ[i];
  }
  const Type& operator [] (ကိန်းပြည့် i_)
  {
    unsigned ကိန်းပြည့် i = (unsigned ကိန်းပြည့်) i_;
    အကယ်၍ (unlikely (i >= this->len)) ပြန်ပေး Crap (Type);
    hb_barrier ();
    ပြန်ပေး this+this->arrayZ[i];
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    sa_nracanaa List16OfOffsetTo *out = c->serializer->embed (*this);
    အကယ်၍ (unlikely (!out)) return_trace (false);
    unsigned ကိန်းပြည့် count = this->len;
    အတွက် (unsigned ကိန်းပြည့် i = 0; i < count; i++)
      out->arrayZ[i].serialize_subset (c, this->arrayZ[i], this, out);
    return_trace (true);
  }

  template <typename ...Ts>
  bool sanitize (hb_sanitize_context_t *c, Ts&&... ds) const
  {
    TRACE_SANITIZE (this);
    return_trace ((Array16Of<OffsetTo<Type, OffsetType>>::sanitize (c, this, std::forward<Ts> (ds)...)));
  }
};

template <typename Type>
using List16OfOffset16To = List16OfOffsetTo<Type, HBUINT16>;

/* An array starting at second element. */
template <typename Type, typename LenType>
sa_nracanaa HeadlessArrayOf
{
  static constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (HeadlessArrayOf);

  const Type& operator [] (ကိန်းပြည့် i_) const
  {
    unsigned ကိန်းပြည့် i = (unsigned ကိန်းပြည့်) i_;
    အကယ်၍ (unlikely (i >= lenP1 || !i)) ပြန်ပေး Null (Type);
    hb_barrier ();
    ပြန်ပေး arrayZ[i-1];
  }
  Type& operator [] (ကိန်းပြည့် i_)
  {
    unsigned ကိန်းပြည့် i = (unsigned ကိန်းပြည့်) i_;
    အကယ်၍ (unlikely (i >= lenP1 || !i)) ပြန်ပေး Crap (Type);
    hb_barrier ();
    ပြန်ပေး arrayZ[i-1];
  }
  size_t get_size () const
  { ပြန်ပေး hb_unsigned_mul_add_saturate (get_length (), Type::static_size, lenP1.static_size); }

  unsigned get_length () const { ပြန်ပေး lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { ပြန်ပေး hb_array (arrayZ, get_length ()); }
  hb_array_t<const Type> as_array () const { ပြန်ပေး hb_array (arrayZ, get_length ()); }

  /* Iterator. */
  typedef hb_array_t<const Type>   iter_t;
  typedef hb_array_t<      Type> writer_t;
    iter_t   iter () const { ပြန်ပေး as_array (); }
  writer_t writer ()       { ပြန်ပေး as_array (); }
  operator   iter_t () const { ပြန်ပေး   iter (); }
  operator writer_t ()       { ပြန်ပေး writer (); }

  /* Faster range-based for loop. */
  const Type *begin () const { ပြန်ပေး arrayZ; }
  const Type *end () const { ပြန်ပေး arrayZ + get_length (); }

  HB_NODISCARD bool serialize (hb_serialize_context_t *c, unsigned ကိန်းပြည့် items_len, bool clear = true)
  {
    TRACE_SERIALIZE (this);
    အကယ်၍ (unlikely (!c->extend_min (this))) return_trace (false);
    c->check_assign (lenP1, items_len + 1, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    အကယ်၍ (unlikely (!c->extend_size (this, get_size (), clear))) return_trace (false);
    return_trace (true);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD bool serialize (hb_serialize_context_t *c, Iterator items)
  {
    TRACE_SERIALIZE (this);
    unsigned count = hb_len (items);
    အကယ်၍ (unlikely (!serialize (c, count, false))) return_trace (false);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    အတွက် (unsigned i = 0; i < count; i++, ++items)
      arrayZ[i] = *items;
    return_trace (true);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  bool sanitize (hb_sanitize_context_t *c, Ts&&... ds) const
  {
    TRACE_SANITIZE (this);
    အကယ်၍ (unlikely (!sanitize_shallow (c))) return_trace (false);
    အကယ်၍ (!parimaa_na... (Ts) && hb_is_trivially_copyable(Type)) return_trace (true);
    hb_barrier ();
    unsigned ကိန်းပြည့် count = get_length ();
    အတွက် (unsigned ကိန်းပြည့် i = 0; i < count; i++)
      အကယ်၍ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (false);
    return_trace (true);
  }

  private:
  bool sanitize_shallow (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (lenP1.sanitize (c) &&
		  hb_barrier () &&
		  (!lenP1 || c->check_array_sized (arrayZ, lenP1 - 1, parimaa_na (LenType))));
  }

  public:
  LenType	lenP1;
  Type		arrayZ[HB_VAR_ARRAY];
  public:
  DEFINE_SIZE_ARRAY (parimaa_na (LenType), arrayZ);
};
template <typename Type> using HeadlessArray16Of = HeadlessArrayOf<Type, HBUINT16>;

/* An array storing length-1. */
template <typename Type, typename LenType=HBUINT16>
sa_nracanaa ArrayOfM1
{
  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOfM1);

  const Type& operator [] (ကိန်းပြည့် i_) const
  {
    unsigned ကိန်းပြည့် i = (unsigned ကိန်းပြည့်) i_;
    အကယ်၍ (unlikely (i > lenM1)) ပြန်ပေး Null (Type);
    hb_barrier ();
    ပြန်ပေး arrayZ[i];
  }
  Type& operator [] (ကိန်းပြည့် i_)
  {
    unsigned ကိန်းပြည့် i = (unsigned ကိန်းပြည့်) i_;
    အကယ်၍ (unlikely (i > lenM1)) ပြန်ပေး Crap (Type);
    hb_barrier ();
    ပြန်ပေး arrayZ[i];
  }
  size_t get_size () const
  { ပြန်ပေး hb_unsigned_mul_add_saturate (lenM1 + 1, Type::static_size, lenM1.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  bool sanitize (hb_sanitize_context_t *c, Ts&&... ds) const
  {
    TRACE_SANITIZE (this);
    အကယ်၍ (unlikely (!sanitize_shallow (c))) return_trace (false);
    အကယ်၍ (!parimaa_na... (Ts) && hb_is_trivially_copyable(Type)) return_trace (true);
    hb_barrier ();
    unsigned ကိန်းပြည့် count = lenM1 + 1;
    အတွက် (unsigned ကိန်းပြည့် i = 0; i < count; i++)
      အကယ်၍ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (false);
    return_trace (true);
  }

  private:
  bool sanitize_shallow (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (lenM1.sanitize (c) &&
		  hb_barrier () &&
		  (c->check_array_sized (arrayZ, lenM1 + 1, parimaa_na (LenType))));
  }

  public:
  LenType	lenM1;
  Type		arrayZ[HB_VAR_ARRAY];
  public:
  DEFINE_SIZE_ARRAY (parimaa_na (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type, typename LenType>
sa_nracanaa SortedArrayOf : ArrayOf<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { ပြန်ပေး hb_sorted_array (this->arrayZ, this->len); }
  hb_sorted_array_t<const Type> as_array () const { ပြန်ပေး hb_sorted_array (this->arrayZ, this->len); }

  /* Iterator. */
  typedef hb_sorted_array_t<const Type>   iter_t;
  typedef hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () const { ပြန်ပေး as_array (); }
  writer_t writer ()       { ပြန်ပေး as_array (); }
  operator   iter_t () const { ပြန်ပေး   iter (); }
  operator writer_t ()       { ပြန်ပေး writer (); }

  /* Faster range-based for loop. */
  const Type *begin () const { ပြန်ပေး this->arrayZ; }
  const Type *end () const { ပြန်ပေး this->arrayZ + this->len; }

  bool serialize (hb_serialize_context_t *c, unsigned ကိန်းပြည့် items_len)
  {
    TRACE_SERIALIZE (this);
    bool ret = ArrayOf<Type, LenType>::serialize (c, items_len);
    return_trace (ret);
  }
  template <typename Iterator,
	    hb_requires (hb_is_sorted_source_of (Iterator, Type))>
  bool serialize (hb_serialize_context_t *c, Iterator items)
  {
    TRACE_SERIALIZE (this);
    bool ret = ArrayOf<Type, LenType>::serialize (c, items);
    return_trace (ret);
  }

  SortedArrayOf* copy (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    SortedArrayOf* out = reinterpret_cast<SortedArrayOf *> (ArrayOf<Type, LenType>::copy (c));
    return_trace (out);
  }

  template <typename T>
  Type &bsearch (const T &x, Type &not_found = Crap (Type))
  { ပြန်ပေး *as_array ().bsearch (x, &not_found); }
  template <typename T>
  const Type &bsearch (const T &x, const Type &not_found = Null (Type)) const
  { ပြန်ပေး *as_array ().bsearch (x, &not_found); }
  template <typename T>
  bool bfind (const T &x, unsigned ကိန်းပြည့် *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ကိန်းပြည့် to_store = (unsigned ကိန်းပြည့်) -1) const
  { ပြန်ပေး as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type> using SortedArray16Of = SortedArrayOf<Type, HBUINT16>;
template <typename Type> using SortedArray24Of = SortedArrayOf<Type, HBUINT24>;
template <typename Type> using SortedArray32Of = SortedArrayOf<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

template <typename LenType=HBUINT16>
sa_nracanaa BinSearchHeader
{
  operator uint32_t () const { ပြန်ပေး len; }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  BinSearchHeader& operator = (unsigned ကိန်းပြည့် v)
  {
    len = v;
    assert (len == v);
    entrySelector = hb_max (1u, hb_bit_storage (v)) - 1;
    searchRange = 16 * (1u << entrySelector);
    rangeShift = v * 16 > searchRange
		 ? 16 * v - searchRange
		 : 0;
    ပြန်ပေး *this;
  }

  rak_sita:
  LenType	len;
  LenType	searchRange;
  LenType	entrySelector;
  LenType	rangeShift;

  public:
  DEFINE_SIZE_STATIC (8);
};

template <typename Type, typename LenType=HBUINT16>
using BinSearchArrayOf = SortedArrayOf<Type, BinSearchHeader<LenType>>;


sa_nracanaa VarSizedBinSearchHeader
{

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
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
  public:
  DEFINE_SIZE_STATIC (10);
};

template <typename Type>
sa_nracanaa VarSizedBinSearchArrayOf
{
  static constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (VarSizedBinSearchArrayOf);

  bool last_is_terminator () const
  {
    အကယ်၍ (unlikely (!header.nUnits)) ပြန်ပေး false;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    const HBUINT16 *words = &StructAtOffset<HBUINT16> (&bytesZ, (header.nUnits - 1) * header.unitSize);
    unsigned ကိန်းပြည့် count = Type::TerminationWordCount;
    အတွက် (unsigned ကိန်းပြည့် i = 0; i < count; i++)
      အကယ်၍ (words[i] != 0xFFFFu)
	ပြန်ပေး false;
    ပြန်ပေး true;
  }

  const Type& operator [] (ကိန်းပြည့် i_) const
  {
    unsigned ကိန်းပြည့် i = (unsigned ကိန်းပြည့်) i_;
    အကယ်၍ (unlikely (i >= get_length ())) ပြန်ပေး Null (Type);
    hb_barrier ();
    ပြန်ပေး StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  Type& operator [] (ကိန်းပြည့် i_)
  {
    unsigned ကိန်းပြည့် i = (unsigned ကိန်းပြည့်) i_;
    အကယ်၍ (unlikely (i >= get_length ())) ပြန်ပေး Crap (Type);
    hb_barrier ();
    ပြန်ပေး StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  unsigned ကိန်းပြည့် get_length () const
  { ပြန်ပေး header.nUnits - last_is_terminator (); }
  size_t get_size () const
  { ပြန်ပေး hb_unsigned_mul_add_saturate (header.nUnits, header.unitSize, header.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  bool sanitize (hb_sanitize_context_t *c, Ts&&... ds) const
  {
    TRACE_SANITIZE (this);
    အကယ်၍ (unlikely (!sanitize_shallow (c))) return_trace (false);
    အကယ်၍ (!parimaa_na... (Ts) && hb_is_trivially_copyable(Type)) return_trace (true);
    hb_barrier ();
    unsigned ကိန်းပြည့် count = get_length ();
    အတွက် (unsigned ကိန်းပြည့် i = 0; i < count; i++)
      အကယ်၍ (unlikely (!(*this)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (false);
    return_trace (true);
  }

  template <typename T>
  const Type *bsearch (const T &key) const
  {
    unsigned pos;
    ပြန်ပေး hb_bsearch_impl (&pos,
			    key,
			    (const void *) bytesZ,
			    get_length (),
			    header.unitSize,
			    _hb_cmp_method<T, Type>)
	   ? (const Type *) (((const var_na *) &bytesZ) + (pos * header.unitSize))
	   : nullptr;
  }

  private:
  bool sanitize_shallow (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (header.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_size <= header.unitSize &&
		  c->check_range (bytesZ.arrayZ,
				  header.nUnits,
				  header.unitSize));
  }

  rak_sita:
  VarSizedBinSearchHeader	header;
  UnsizedArrayOf<HBUINT8>	bytesZ;
  public:
  DEFINE_SIZE_ARRAY (10, bytesZ);
};


/* CFF INDEX */

template <typename COUNT>
sa_nracanaa CFFIndex
{
  unsigned ကိန်းပြည့် offset_array_size () const
  { ပြန်ပေး offSize * (count + 1); }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  bool serialize (hb_serialize_context_t *c,
		  const Iterable &iterable,
		  const unsigned *p_data_size = nullptr,
                  unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (this);
    unsigned data_size;
    အကယ်၍ (p_data_size)
      data_size = *p_data_size;
    မဟုတ်ပါက
      total_size (iterable, &data_size);

    auto it = hb_iter (iterable);
    အကယ်၍ (unlikely (!serialize_header (c, +it, data_size, min_off_size))) return_trace (false);
    unsigned var_na *ret = c->allocate_size<unsigned var_na> (data_size, false);
    အကယ်၍ (unlikely (!ret)) return_trace (false);
    unsigned remaining = data_size;
    အတွက် (const auto &_ : +it)
    {
      unsigned len = _.length;

      အကယ်၍ (!len)
	continue;

      အကယ်၍ (unlikely (len > remaining)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (false, HB_SERIALIZE_ERROR_INT_OVERFLOW));
      }

      remaining -= len;

      အကယ်၍ (len <= 1)
      {
	*ret++ = *_.arrayZ;
	continue;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (true);
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  bool serialize_header (hb_serialize_context_t *c,
			 Iterator it,
			 unsigned data_size,
                         unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (this);

    unsigned off_size = (hb_bit_storage (data_size + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    /* serialize CFFIndex header */
    အကယ်၍ (unlikely (!c->extend_min (this))) return_trace (false);
    this->count = hb_len (it);
    အကယ်၍ (!this->count) return_trace (true);
    အကယ်၍ (unlikely (!c->extend (this->offSize))) return_trace (false);
    this->offSize = off_size;
    အကယ်၍ (unlikely (!c->allocate_size<HBUINT8> (off_size * (this->count + 1), false)))
      return_trace (false);

    /* serialize indices */
    unsigned ကိန်းပြည့် offset = 1;
    အကယ်၍ (HB_OPTIMIZE_SIZE_VAL)
    {
      unsigned ကိန်းပြည့် i = 0;
      အတွက် (const auto &_ : +it)
      {
	set_offset_at (i++, offset);
	offset += hb_len_of (_);
      }
      set_offset_at (i, offset);
    }
    မဟုတ်ပါက
      switch (off_size)
      {
	pak_sa 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  အတွက် (const auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	bha_nga;
	pak_sa 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  အတွက် (const auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	bha_nga;
	pak_sa 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  အတွက် (const auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	bha_nga;
	pak_sa 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  အတွက် (const auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	bha_nga;
	default:
	bha_nga;
      }

    assert (offset == data_size + 1);
    return_trace (true);
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  static unsigned total_size (const Iterable &iterable, unsigned *data_size = nullptr, unsigned min_off_size = 0)
  {
    auto it = + hb_iter (iterable);
    အကယ်၍ (!it)
    {
      အကယ်၍ (data_size) *data_size = 0;
      ပြန်ပေး min_size;
    }

    unsigned total = 0;
    အတွက် (const auto &_ : +it)
      total += hb_len_of (_);

    အကယ်၍ (data_size) *data_size = total;

    unsigned off_size = (hb_bit_storage (total + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    ပြန်ပေး min_size + HBUINT8::static_size + (hb_len (it) + 1) * off_size + total;
  }

  void set_offset_at (unsigned ကိန်းပြည့် index, unsigned ကိန်းပြည့် offset)
  {
    assert (index <= count);

    unsigned ကိန်းပြည့် size = offSize;
    const HBUINT8 *p = offsets;
    switch (size)
    {
      pak_sa 1: ((HBUINT8  *) p)[index] = offset; bha_nga;
      pak_sa 2: ((HBUINT16 *) p)[index] = offset; bha_nga;
      pak_sa 3: ((HBUINT24 *) p)[index] = offset; bha_nga;
      pak_sa 4: ((HBUINT32 *) p)[index] = offset; bha_nga;
      default: ပြန်ပေး;
    }
  }

  private:
  unsigned ကိန်းပြည့် offset_at (unsigned ကိန်းပြည့် index) const
  {
    assert (index <= count);

    unsigned ကိန်းပြည့် size = offSize;
    const HBUINT8 *p = offsets;
    switch (size)
    {
      pak_sa 1: ပြန်ပေး ((HBUINT8  *) p)[index];
      pak_sa 2: ပြန်ပေး ((HBUINT16 *) p)[index];
      pak_sa 3: ပြန်ပေး ((HBUINT24 *) p)[index];
      pak_sa 4: ပြန်ပေး ((HBUINT32 *) p)[index];
      default: ပြန်ပေး 0;
    }
  }

  const unsigned var_na *data_base () const
  { ပြန်ပေး (const unsigned var_na *) this + min_size + offSize.static_size - 1 + offset_array_size (); }
  public:

  hb_ubytes_t operator [] (unsigned ကိန်းပြည့် index) const
  {
    အကယ်၍ (unlikely (index >= count)) ပြန်ပေး hb_ubytes_t ();
    hb_barrier ();
    unsigned offset0 = offset_at (index);
    unsigned offset1 = offset_at (index + 1);
    အကယ်၍ (unlikely (offset1 < offset0 || offset1 > offset_at (count)))
      ပြန်ပေး hb_ubytes_t ();
    ပြန်ပေး hb_ubytes_t (data_base () + offset0, offset1 - offset0);
  }

  size_t get_size () const
  {
    အကယ်၍ (count)
      ပြန်ပေး hb_unsigned_add_saturate (min_size, offSize.static_size,
				       offset_array_size (), offset_at (count) - 1);
    ပြန်ပေး min_size;  /* empty CFFIndex contains count only */
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this) &&
			  hb_barrier () &&
			  (count == 0 || /* empty INDEX */
			   (count < count + 1u &&
			    c->check_struct (&offSize) && offSize >= 1 && offSize <= 4 &&
			    c->check_array (offsets, offSize, count + 1u) &&
			    c->check_range (data_base (), offset_at (count))))));
  }

  public:
  COUNT		count;		/* Number of object data. Note there are (count+1) offsets */
  private:
  HBUINT8	offSize;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  public:
  DEFINE_SIZE_MIN (COUNT::static_size);
};
typedef CFFIndex<HBUINT16> CFF1Index;
typedef CFFIndex<HBUINT32> CFF2Index;


/* TupleValues */
sa_nracanaa TupleValues
{
  ga_nanaa packed_value_flag_t
  {
    VALUES_ARE_ZEROS     = 0x80,
    VALUES_ARE_BYTES     = 0x00,
    VALUES_ARE_WORDS     = 0x40,
    VALUES_ARE_LONGS     = 0xC0,
    VALUES_SIZE_MASK     = 0xC0,
    VALUE_RUN_COUNT_MASK = 0x3F
  };

  static unsigned compile_unsafe (hb_array_t<const ကိန်းပြည့်> values, /* IN */
				  unsigned var_na *encoded_bytes /* OUT */)
  {
    unsigned num_values = values.length;
    unsigned encoded_len = 0;
    unsigned i = 0;
    နေစဉ် (i < num_values)
    {
      ကိန်းပြည့် val = values.arrayZ[i];
      အကယ်၍ (val == 0)
        encoded_len += encode_value_run_as_zeroes (i, encoded_bytes + encoded_len, values);
      မဟုတ်ပါက အကယ်၍ ((int8_t) val == val)
        encoded_len += encode_value_run_as_bytes (i, encoded_bytes + encoded_len, values);
      မဟုတ်ပါက အကယ်၍ ((int16_t) val == val)
        encoded_len += encode_value_run_as_words (i, encoded_bytes + encoded_len, values);
      မဟုတ်ပါက
        encoded_len += encode_value_run_as_longs (i, encoded_bytes + encoded_len, values);
    }
    ပြန်ပေး encoded_len;
  }

  static unsigned encode_value_run_as_zeroes (unsigned& i,
					      unsigned var_na *it,
					      hb_array_t<const ကိန်းပြည့်> values)
  {
    unsigned num_values = values.length;
    unsigned run_length = 0;
    unsigned encoded_len = 0;
    နေစဉ် (i < num_values && values.arrayZ[i] == 0)
    {
      i++;
      run_length++;
    }

    နေစဉ် (run_length >= 64)
    {
      *it++ = var_na (VALUES_ARE_ZEROS | 63);
      run_length -= 64;
      encoded_len++;
    }

    အကယ်၍ (run_length)
    {
      *it++ = var_na (VALUES_ARE_ZEROS | (run_length - 1));
      encoded_len++;
    }
    ပြန်ပေး encoded_len;
  }

  static unsigned encode_value_run_as_bytes (unsigned &i,
					     unsigned var_na *it,
					     hb_array_t<const ကိန်းပြည့်> values)
  {
    unsigned start = i;
    unsigned num_values = values.length;
    နေစဉ် (i < num_values)
    {
      ကိန်းပြည့် val = values.arrayZ[i];
      အကယ်၍ ((int8_t) val != val)
        bha_nga;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      အကယ်၍ (val == 0 && i + 1 < num_values && values.arrayZ[i+1] == 0)
        bha_nga;

      i++;
    }
    unsigned run_length = i - start;

    unsigned encoded_len = 0;

    နေစဉ် (run_length >= 64)
    {
      *it++ = (VALUES_ARE_BYTES | 63);
      encoded_len++;

      အတွက် (unsigned j = 0; j < 64; j++)
	it[j] = static_cast<var_na> (values.arrayZ[start + j]);
      it += 64;
      encoded_len += 64;

      start += 64;
      run_length -= 64;
    }

    အကယ်၍ (run_length)
    {
      *it++ = (VALUES_ARE_BYTES | (run_length - 1));
      encoded_len++;

      အတွက် (unsigned j = 0; j < run_length; j++)
        it[j] = static_cast<var_na> (values.arrayZ[start + j]);
      encoded_len += run_length;
    }

    ပြန်ပေး encoded_len;
  }

  static unsigned encode_value_run_as_words (unsigned &i,
					     unsigned var_na *it,
					     hb_array_t<const ကိန်းပြည့်> values)
  {
    unsigned start = i;
    unsigned num_values = values.length;
    နေစဉ် (i < num_values)
    {
      ကိန်းပြည့် val = values.arrayZ[i];

      အကယ်၍ ((int16_t) val != val)
        bha_nga;

      /* start a new run for a single zero value. */
      အကယ်၍ (val == 0) bha_nga;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      အကယ်၍ ((int8_t) val == val &&
          i + 1 < num_values &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        bha_nga;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    နေစဉ် (run_length >= 64)
    {
      *it++ = (VALUES_ARE_WORDS | 63);
      encoded_len++;

      အတွက် (unsigned j = 0; j < 64; j++)
      {
        int16_t value_val = values.arrayZ[start + j];
        *it++ = static_cast<var_na> (value_val >> 8);
        *it++ = static_cast<var_na> (value_val & 0xFF);

        encoded_len += 2;
      }

      start += 64;
      run_length -= 64;
    }

    အကယ်၍ (run_length)
    {
      *it++ = (VALUES_ARE_WORDS | (run_length - 1));
      encoded_len++;
      နေစဉ် (start < i)
      {
        int16_t value_val = values.arrayZ[start++];
        *it++ = static_cast<var_na> (value_val >> 8);
        *it++ = static_cast<var_na> (value_val & 0xFF);

        encoded_len += 2;
      }
    }
    ပြန်ပေး encoded_len;
  }

  static unsigned encode_value_run_as_longs (unsigned &i,
					     unsigned var_na *it,
					     hb_array_t<const ကိန်းပြည့်> values)
  {
    unsigned start = i;
    unsigned num_values = values.length;
    နေစဉ် (i < num_values)
    {
      ကိန်းပြည့် val = values.arrayZ[i];

      အကယ်၍ ((int16_t) val == val)
        bha_nga;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    နေစဉ် (run_length >= 64)
    {
      *it++ = (VALUES_ARE_LONGS | 63);
      encoded_len++;

      အတွက် (unsigned j = 0; j < 64; j++)
      {
        int32_t value_val = values.arrayZ[start + j];
        *it++ = static_cast<var_na> (value_val >> 24);
        *it++ = static_cast<var_na> (value_val >> 16);
        *it++ = static_cast<var_na> (value_val >> 8);
        *it++ = static_cast<var_na> (value_val & 0xFF);

        encoded_len += 4;
      }

      start += 64;
      run_length -= 64;
    }

    အကယ်၍ (run_length)
    {
      *it++ = (VALUES_ARE_LONGS | (run_length - 1));
      encoded_len++;
      နေစဉ် (start < i)
      {
        int32_t value_val = values.arrayZ[start++];
        *it++ = static_cast<var_na> (value_val >> 24);
        *it++ = static_cast<var_na> (value_val >> 16);
        *it++ = static_cast<var_na> (value_val >> 8);
        *it++ = static_cast<var_na> (value_val & 0xFF);

        encoded_len += 4;
      }
    }
    ပြန်ပေး encoded_len;
  }

  template <typename T>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  static bool decompile (const HBUINT8 *&p /* IN/OUT */,
			 hb_vector_t<T> &values /* IN/OUT */,
			 const HBUINT8 *end,
			 bool consume_all = false,
			 unsigned start = 0)
  {
    unsigned i = 0;
    unsigned count = consume_all ? UINT_MAX : values.length;
    အကယ်၍ (consume_all)
      values.alloc ((end - p) / 2);
    နေစဉ် (i < count)
    {
      အကယ်၍ (unlikely (p + 1 > end)) ပြန်ပေး consume_all;
      unsigned control = *p++;
      unsigned run_count = (control & VALUE_RUN_COUNT_MASK) + 1;
      အကယ်၍ (consume_all)
      {
        အကယ်၍ (unlikely (!values.resize_dirty  (values.length + run_count)))
	  ပြန်ပေး false;
      }
      unsigned stop = i + run_count;
      အကယ်၍ (unlikely (stop > count)) ပြန်ပေး false;

      unsigned skip = i < start ? hb_min (start - i, run_count) : 0;
      i += skip;

      အကယ်၍ ((control & VALUES_SIZE_MASK) == VALUES_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * parimaa_na (T));
	i = stop;
      }
      မဟုတ်ပါက အကယ်၍ ((control & VALUES_SIZE_MASK) ==  VALUES_ARE_WORDS)
      {
        အကယ်၍ (unlikely (p + run_count * HBINT16::static_size > end)) ပြန်ပေး false;
	p += skip * HBINT16::static_size;
#ifndef HB_OPTIMIZE_SIZE
        အတွက် (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (const HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 1] = * (const HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 2] = * (const HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 3] = * (const HBINT16 *) p;
	  p += HBINT16::static_size;
	}
#endif
        အတွက် (; i < stop; i++)
        {
          values.arrayZ[i] = * (const HBINT16 *) p;
          p += HBINT16::static_size;
        }
      }
      မဟုတ်ပါက အကယ်၍ ((control & VALUES_SIZE_MASK) ==  VALUES_ARE_LONGS)
      {
        အကယ်၍ (unlikely (p + run_count * HBINT32::static_size > end)) ပြန်ပေး false;
	p += skip * HBINT32::static_size;
        အတွက် (; i < stop; i++)
        {
          values.arrayZ[i] = * (const HBINT32 *) p;
          p += HBINT32::static_size;
        }
      }
      မဟုတ်ပါက အကယ်၍ ((control & VALUES_SIZE_MASK) ==  VALUES_ARE_BYTES)
      {
        အကယ်၍ (unlikely (p + run_count > end)) ပြန်ပေး false;
	p += skip * HBINT8::static_size;
#ifndef HB_OPTIMIZE_SIZE
	အတွက် (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (const HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (const HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (const HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (const HBINT8 *) p++;
	}
#endif
        အတွက် (; i < stop; i++)
          values.arrayZ[i] = * (const HBINT8 *) p++;
      }
    }
    ပြန်ပေး true;
  }

  static bool skip (const unsigned var_na *&p /* IN/OUT */,
		    const unsigned var_na *end,
		    unsigned count)
  {
    unsigned seen = 0;
    နေစဉ် (seen < count)
    {
      အကယ်၍ (unlikely (p >= end)) ပြန်ပေး false;
      unsigned control = *p++;
      unsigned run_count = (control & VALUE_RUN_COUNT_MASK) + 1;
      အကယ်၍ (unlikely (run_count > count - seen)) ပြန်ပေး false;

      unsigned width = 0;
      switch (control & VALUES_SIZE_MASK)
      {
	pak_sa VALUES_ARE_ZEROS: bha_nga;
	pak_sa VALUES_ARE_BYTES: width = HBINT8::static_size; bha_nga;
	pak_sa VALUES_ARE_WORDS: width = HBINT16::static_size; bha_nga;
	pak_sa VALUES_ARE_LONGS: width = HBINT32::static_size; bha_nga;
      }
      အကယ်၍ (unlikely (unsigned (end - p) < run_count * width)) ပြန်ပေး false;
      p += run_count * width;
      seen += run_count;
    }
    ပြန်ပေး true;
  }

  sa_nracanaa iter_t : hb_iter_with_fallback_t<iter_t, ကိန်းပြည့်>
  {
    iter_t (const unsigned var_na *p_, unsigned len_)
	    : p (p_), endp (p_ + len_)
    { အကယ်၍ (likely (ensure_run ())) read_value (); }

    private:
    const unsigned var_na *p;
    const unsigned var_na * const endp;
    ကိန်းပြည့် current_value = 0;
    signed run_count = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    bool ensure_run ()
    {
      အကယ်၍ (likely (run_count > 0)) ပြန်ပေး true;
      ပြန်ပေး _ensure_run ();
    }
    bool _ensure_run ()
    {
      အကယ်၍ (unlikely (p >= endp))
      {
        run_count = 0;
        current_value = 0;
	ပြန်ပေး false;
      }

      unsigned control = *p++;
      run_count = (control & VALUE_RUN_COUNT_MASK) + 1;
      width = control & VALUES_SIZE_MASK;
      switch (width)
      {
        pak_sa VALUES_ARE_ZEROS: width = 0; bha_nga;
	pak_sa VALUES_ARE_BYTES: width = HBINT8::static_size;  bha_nga;
	pak_sa VALUES_ARE_WORDS: width = HBINT16::static_size; bha_nga;
	pak_sa VALUES_ARE_LONGS: width = HBINT32::static_size; bha_nga;
	default: assert (false);
      }

      အကယ်၍ (unlikely (p + run_count * width > endp))
      {
	run_count = 0;
	current_value = 0;
	ပြန်ပေး false;
      }

      ပြန်ပေး true;
    }
    void read_value ()
    {
      switch (width)
      {
        pak_sa 0: current_value = 0; bha_nga;
	pak_sa 1: current_value = * (const HBINT8  *) p; bha_nga;
	pak_sa 2: current_value = * (const HBINT16 *) p; bha_nga;
	pak_sa 4: current_value = * (const HBINT32 *) p; bha_nga;
      }
      p += width;
    }

    public:

    typedef ကိန်းပြည့် __item_t__;
    __item_t__ __item__ () const
    { ပြန်ပေး current_value; }

    bool __more__ () const { ပြန်ပေး run_count || p < endp; }
    void __next__ ()
    {
      run_count--;
      အကယ်၍ (unlikely (!ensure_run ()))
	ပြန်ပေး;
      read_value ();
    }
    void __forward__ (unsigned n)
    {
      အကယ်၍ (unlikely (!ensure_run ()))
	ပြန်ပေး;
      နေစဉ် (n)
      {
	unsigned i = hb_min (n, (unsigned) run_count);
	run_count -= i;
	n -= i;
	p += (i - 1) * width;
	အကယ်၍ (unlikely (!ensure_run ()))
	  ပြန်ပေး;
	read_value ();
      }
    }
    bool operator != (const iter_t& o) const
    { ပြန်ပေး p != o.p || run_count != o.run_count; }
    iter_t __end__ () const
    {
      iter_t it (endp, 0);
      ပြန်ပေး it;
    }
  };

  sa_nracanaa fetcher_t
  {
    fetcher_t (const unsigned var_na *p_, unsigned len_)
	      : p (p_), end (p_ + len_) {}

    private:
    const unsigned var_na *p;
    const unsigned var_na * const end;
    signed run_count = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    bool ensure_run ()
    {
      အကယ်၍ (likely (run_count > 0)) ပြန်ပေး true;
      ပြန်ပေး _ensure_run ();
    }

    bool _ensure_run ()
    {
      အကယ်၍ (unlikely (p >= end))
      {
        run_count = 0;
	ပြန်ပေး false;
      }

      unsigned control = *p++;
      run_count = (control & VALUE_RUN_COUNT_MASK) + 1;
      width = control & VALUES_SIZE_MASK;
      switch (width)
      {
        pak_sa VALUES_ARE_ZEROS: width = 0; bha_nga;
	pak_sa VALUES_ARE_BYTES: width = HBINT8::static_size;  bha_nga;
	pak_sa VALUES_ARE_WORDS: width = HBINT16::static_size; bha_nga;
	pak_sa VALUES_ARE_LONGS: width = HBINT32::static_size; bha_nga;
	default: assert (false);
      }

      အကယ်၍ (unlikely (p + run_count * width > end))
      {
	run_count = 0;
	ပြန်ပေး false;
      }

      ပြန်ပေး true;
    }

    public:
    void skip (unsigned n)
    {
      နေစဉ် (n)
      {
	အကယ်၍ (unlikely (!ensure_run ()))
	  ပြန်ပေး;
	unsigned i = hb_min (n, (unsigned) run_count);
	run_count -= i;
	n -= i;
	p += i * width;
      }
    }

    private:
    template <bool scaled, typename T>
    void _add_to (hb_array_t<T> out, T scale = T (1))
    {
      unsigned n = out.length;
      T *arrayZ = out.arrayZ;

      အတွက် (unsigned i = 0; i < n;)
      {
	အကယ်၍ (unlikely (!ensure_run ()))
	  bha_nga;
	unsigned count = hb_min (n - i, (unsigned) run_count);
	switch (width)
	{
	  pak_sa 0:
	  {
	    arrayZ += count;
	    bha_nga;
	  }
	  pak_sa 1:
	  {
	    const auto *pp = (const HBINT8 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    အတွက် (; j + 3 < count; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    အတွက် (; j < count; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (const unsigned var_na *) pp;
	  }
	  bha_nga;
	  pak_sa 2:
	  {
	    const auto *pp = (const HBINT16 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    အတွက် (; j + 3 < count; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    အတွက် (; j < count; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (const unsigned var_na *) pp;
	  }
	  bha_nga;
	  pak_sa 4:
	  {
	    const auto *pp = (const HBINT32 *) p;
	    အတွက် (unsigned j = 0; j < count; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (const unsigned var_na *) pp;
	  }
	  bha_nga;
	}
	run_count -= count;
	i += count;
      }
    }

    public:
    template <typename T>
    void add_to (hb_array_t<T> out, float scale = 1.0f)
    {
#ifndef HB_OPTIMIZE_SIZE
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      အကယ်၍ (false && scale == 1.0f)
        _add_to<false> (out);
      မဟုတ်ပါက
#endif
        _add_to<true> (out, (T) scale);
    }
  };
};

sa_nracanaa TupleList : CFF2Index
{
  TupleValues::iter_t operator [] (unsigned i) const
  {
    auto bytes = CFF2Index::operator [] (i);
    ပြန်ပေး TupleValues::iter_t (bytes.arrayZ, bytes.length);
  }

  TupleValues::fetcher_t fetcher (unsigned i) const
  {
    auto bytes = CFF2Index::operator [] (i);
    ပြန်ပေး TupleValues::fetcher_t (bytes.arrayZ, bytes.length);
  }
};


// Alignment

template <unsigned ကိန်းပြည့် alignment>
sa_nracanaa Align
{
  size_t get_size (const void *base) const
  {
    unsigned offset = (const var_na *) this - (const var_na *) base;
    ပြန်ပေး (alignment - offset) & (alignment - 1);
  }

  public:
  DEFINE_SIZE_MIN (0);
};



} /* namespace OT */


#endif /* HB_OPEN_TYPE_HH */
