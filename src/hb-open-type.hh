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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-blob.hh"
#அந்தர்பாவ "hb-face.hh"
#அந்தர்பாவ "hb-machinery.hh"
#அந்தர்பாவ "hb-meta.hh"
#அந்தர்பாவ "hb-subset.hh"


நாமாகாஶ OT {


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
template <தருக்க BE,
	  typename Type,
	  unsigned முழு Size = அளவு (Type)>
கட்டமைப்பு இலக்கம்_Type
{
  வகைவரை Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  வகைவரை typename std::conditional<std::is_integral<Type>::value && அளவு (Type) <= அளவு(முழு),
				     typename std::conditional<std::is_signed<Type>::value, signed, unsigned>::type,
				     Type>::type WideType;

  இலக்கம்_Type () = இயல்பு;
  explicit constexpr இலக்கம்_Type (Type V) : v {V} {}
  இலக்கம்_Type& operator = (Type V) { v = V; திரும்பு *இது; }

  operator WideType () மாறிலி { திரும்பு v; }

  தருக்க operator == (மாறிலி இலக்கம்_Type &o) மாறிலி { திரும்பு (Type) v == (Type) o.v; }
  தருக்க operator != (மாறிலி இலக்கம்_Type &o) மாறிலி { திரும்பு !(*இது == o); }

  இலக்கம்_Type& operator += (WideType மொத்தம்) { *இது = *இது + மொத்தம்; திரும்பு *இது; }
  இலக்கம்_Type& operator -= (WideType மொத்தம்) { *இது = *இது - மொத்தம்; திரும்பு *இது; }
  இலக்கம்_Type& operator ++ () { *இது += 1; திரும்பு *இது; }
  இலக்கம்_Type& operator -- () { *இது -= 1; திரும்பு *இது; }
  இலக்கம்_Type operator ++ (முழு) { இலக்கம்_Type c (*இது); ++*இது; திரும்பு c; }
  இலக்கம்_Type operator -- (முழு) { இலக்கம்_Type c (*இது); --*இது; திரும்பு c; }

  uint32_t hash () மாறிலி { திரும்பு hb_array ((மாறிலி எழுத்து *) &v, அளவு (v)).hash (); }
  HB_INTERNAL நிலையான முழு cmp (மாறிலி இலக்கம்_Type *a, மாறிலி இலக்கம்_Type *b)
  { திரும்பு b->cmp (*a); }
  HB_INTERNAL நிலையான முழு cmp (மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
  {
    இலக்கம்_Type *pa = (இலக்கம்_Type *) a;
    இலக்கம்_Type *pb = (இலக்கம்_Type *) b;

    திரும்பு pb->cmp (*pa);
  }
  template <typename Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  முழு cmp (Type2 a) மாறிலி
  {
    Type b = v;
    திரும்பு (a > b) - (a < b);
  }
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }
  பாதுகாக்கப்பட்ட:
  typename std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, Size>,
			    HBFloat<BE, Type, Size>>::type v;
  பொது:
  DEFINE_SIZE_STATIC (Size);
};

வகைவரை இலக்கம்_Type<உண்மை, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
வகைவரை இலக்கம்_Type<உண்மை, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
வகைவரை இலக்கம்_Type<உண்மை, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
வகைவரை இலக்கம்_Type<உண்மை, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
வகைவரை இலக்கம்_Type<உண்மை, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
வகைவரை இலக்கம்_Type<உண்மை, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
வகைவரை இலக்கம்_Type<உண்மை, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
வகைவரை இலக்கம்_Type<உண்மை, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
வகைவரை இலக்கம்_Type<உண்மை, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

வகைவரை இலக்கம்_Type<பொய், uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
வகைவரை இலக்கம்_Type<பொய், int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
வகைவரை இலக்கம்_Type<பொய், uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
வகைவரை இலக்கம்_Type<பொய், int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
வகைவரை இலக்கம்_Type<பொய், uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
வகைவரை இலக்கம்_Type<பொய், int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

வகைவரை இலக்கம்_Type<உண்மை,  தசம>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
வகைவரை இலக்கம்_Type<உண்மை,  இரட்டை> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
வகைவரை இலக்கம்_Type<பொய், தசம>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
வகைவரை இலக்கம்_Type<பொய், இரட்டை> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
கட்டமைப்பு HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& operator = (uint16_t i ) { HBUINT16::operator= (i); திரும்பு *இது; }
  பொது:
  DEFINE_SIZE_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
கட்டமைப்பு HBUINT32VAR
{
  size_t get_size () மாறிலி
  {
    unsigned b0 = v[0];
    எனில் (b0 < 0x80)
      திரும்பு 1;
    இல்லையெனில் எனில் (b0 < 0xC0)
      திரும்பு 2;
    இல்லையெனில் எனில் (b0 < 0xE0)
      திரும்பு 3;
    இல்லையெனில் எனில் (b0 < 0xF0)
      திரும்பு 4;
    இல்லையெனில்
      திரும்பு 5;
  }

  நிலையான size_t get_size (uint32_t v)
  {
    எனில் (v < 0x80)
      திரும்பு 1;
    இல்லையெனில் எனில் (v < 0x4000)
      திரும்பு 2;
    இல்லையெனில் எனில் (v < 0x200000)
      திரும்பு 3;
    இல்லையெனில் எனில் (v < 0x10000000)
      திரும்பு 4;
    இல்லையெனில்
      திரும்பு 5;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_size ()));
  }

  operator uint32_t () மாறிலி
  {
    unsigned b0 = v[0];
    எனில் (b0 < 0x80)
      திரும்பு b0;
    இல்லையெனில் எனில் (b0 < 0xC0)
      திரும்பு ((b0 & 0x3F) << 8) | v[1];
    இல்லையெனில் எனில் (b0 < 0xE0)
      திரும்பு ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    இல்லையெனில் எனில் (b0 < 0xF0)
      திரும்பு ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    இல்லையெனில்
      திரும்பு (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  நிலையான unsigned serialize_unsafe (unsigned எழுத்து *buf, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned எழுத்து *p = buf + len;
    க்கு (unsigned i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    எனில் (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    திரும்பு len;
  }

  நிலையான தருக்க serialize (hb_serialize_context_t *c, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned எழுத்து *buf = c->allocate_size<unsigned எழுத்து> (len, பொய்);
    எனில் (unlikely (!buf))
      திரும்பு பொய்;

    serialize_unsafe (buf, v);
    திரும்பு உண்மை;
  }

  பாதுகாக்கப்பட்ட:
  unsigned எழுத்து v[5];

  பொது:
  DEFINE_SIZE_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
வகைவரை HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
வகைவரை HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
வகைவரை HBUINT16 UFWORD;

template <typename Type, unsigned fraction_bits>
கட்டமைப்பு HBFixed : Type
{
  நிலையான constexpr தசம mult = 1.f / (1 << fraction_bits);
  static_assert (Type::static_size * 8 > fraction_bits, "");

  operator signed () மாறிலி = delete;
  operator unsigned () மாறிலி = delete;
  explicit operator தசம () மாறிலி { திரும்பு to_float (); }
  typename Type::type to_int () மாறிலி { திரும்பு Type::v; }
  வெறுமை set_int (typename Type::type i ) { Type::v = i; }
  தசம to_float (தசம offset = 0) மாறிலி  { திரும்பு ((int32_t) Type::v + offset) * mult; }
  வெறுமை set_float (தசம f) { Type::v = roundf (f / mult); }
  பொது:
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
கட்டமைப்பு LONGDATETIME
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }
  பாதுகாக்கப்பட்ட:
  HBINT32 major;
  HBUINT32 minor;
  பொது:
  DEFINE_SIZE_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
கட்டமைப்பு Tag : HBUINT32
{
  Tag& operator = (hb_tag_t i) { HBUINT32::operator= (i); திரும்பு *இது; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  operator மாறிலி எழுத்து* () மாறிலி { திரும்பு reinterpret_cast<மாறிலி எழுத்து *> (இது); }
  operator எழுத்து* ()             { திரும்பு reinterpret_cast<எழுத்து *> (இது); }
  பொது:
  DEFINE_SIZE_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
கட்டமைப்பு HBGlyphID16 : HBUINT16
{
  HBGlyphID16& operator = (uint16_t i) { HBUINT16::operator= (i); திரும்பு *இது; }
};
கட்டமைப்பு HBGlyphID24 : HBUINT24
{
  HBGlyphID24& operator = (uint32_t i) { HBUINT24::operator= (i); திரும்பு *இது; }
};

/* Script/language-system/feature index */
கட்டமைப்பு Index : HBUINT16 {
  நிலையான constexpr unsigned NOT_FOUND_INDEX = 0xFFFFu;
  Index& operator = (uint16_t i) { HBUINT16::operator= (i); திரும்பு *இது; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, Index);

வகைவரை Index NameID;

கட்டமைப்பு VarIdx : HBUINT32 {
  நிலையான constexpr unsigned NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_INDEX, "");
  VarIdx& operator = (uint32_t i) { HBUINT32::operator= (i); திரும்பு *இது; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, VarIdx);

/* Offset, Null offset = 0 */
template <typename Type, தருக்க has_null=உண்மை>
கட்டமைப்பு Offset : Type
{
  Offset& operator = (typename Type::type i) { Type::operator= (i); திரும்பு *இது; }

  வகைவரை Type type;

  தருக்க is_null () மாறிலி { திரும்பு has_null && 0 == *இது; }

  பொது:
  DEFINE_SIZE_STATIC (அளவு (Type));
};

வகைவரை Offset<HBUINT16> Offset16;
வகைவரை Offset<HBUINT24> Offset24;
வகைவரை Offset<HBUINT32> Offset32;


/* CheckSum */
கட்டமைப்பு CheckSum : HBUINT32
{
  CheckSum& operator = (uint32_t i) { HBUINT32::operator= (i); திரும்பு *இது; }

  /* This is reference implementation from the spec. */
  நிலையான uint32_t CalcTableChecksum (மாறிலி HBUINT32 *Table, uint32_t Length)
  {
    uint32_t Sum = 0L;
    ப்ரதிஜ்ஞா (0 == (Length & 3));
    மாறிலி HBUINT32 *EndPtr = Table + Length / HBUINT32::static_size;

    போது (Table < EndPtr)
      Sum += *Table++;
    திரும்பு Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  வெறுமை set_for_data (மாறிலி வெறுமை *data, unsigned முழு length)
  { *இது = CalcTableChecksum ((மாறிலி HBUINT32 *) data, length); }

  பொது:
  DEFINE_SIZE_STATIC (4);
};


/*
 * Version Numbers
 */

template <typename FixedType=HBUINT16>
கட்டமைப்பு FixedVersion
{
  uint32_t to_int () மாறிலி { திரும்பு (major << (அளவு (FixedType) * 8)) + minor; }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  FixedType major;
  FixedType minor;
  பொது:
  DEFINE_SIZE_STATIC (2 * அளவு (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

template <typename Type, தருக்க has_null>
கட்டமைப்பு _hb_has_null
{
  நிலையான மாறிலி Type *get_null () { திரும்பு nullptr; }
  நிலையான Type *get_crap ()       { திரும்பு nullptr; }
};
template <typename Type>
கட்டமைப்பு _hb_has_null<Type, உண்மை>
{
  நிலையான மாறிலி Type *get_null () { திரும்பு &Null (Type); }
  நிலையான       Type *get_crap () { திரும்பு &Crap (Type); }
};

template <typename Type, typename OffsetType, typename BaseType=வெறுமை, தருக்க has_null=உண்மை>
கட்டமைப்பு OffsetTo : Offset<OffsetType, has_null>
{
  using target_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == பொய் ||
		 (hb_has_null_size (Type) || !hb_has_min_size (Type)), "");

  HB_DELETE_COPY_ASSIGN (OffsetTo);
  OffsetTo () = இயல்பு;

  OffsetTo& operator = (typename OffsetType::type i) { OffsetType::operator= (i); திரும்பு *இது; }

  மாறிலி Type& operator () (மாறிலி வெறுமை *base) மாறிலி
  {
    எனில் (unlikely (இது->is_null ())) திரும்பு *_hb_has_null<Type, has_null>::get_null ();
    திரும்பு StructAtOffset<மாறிலி Type> (base, *இது);
  }
  Type& operator () (வெறுமை *base) மாறிலி
  {
    எனில் (unlikely (இது->is_null ())) திரும்பு *_hb_has_null<Type, has_null>::get_crap ();
    திரும்பு StructAtOffset<Type> (base, *இது);
  }

  template <typename Base,
	    hb_enable_if (hb_is_convertible (மாறிலி Base, மாறிலி BaseType *))>
  friend மாறிலி Type& operator + (மாறிலி Base &base, மாறிலி OffsetTo &offset) { திரும்பு offset ((மாறிலி வெறுமை *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (மாறிலி Base, மாறிலி BaseType *))>
  friend மாறிலி Type& operator + (மாறிலி OffsetTo &offset, மாறிலி Base &base) { திரும்பு offset ((மாறிலி வெறுமை *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (Base &&base, OffsetTo &offset) { திரும்பு offset ((வெறுமை *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (OffsetTo &offset, Base &&base) { திரும்பு offset ((வெறுமை *) base); }


  template <typename Base, typename ...Ts>
  தருக்க serialize_subset (hb_subset_context_t *c, மாறிலி OffsetTo& src,
			 மாறிலி Base *src_base, Ts&&... ds)
  {
    *இது = 0;
    எனில் (src.is_null ())
      திரும்பு பொய்;

    auto *s = c->serializer;

    s->push ();

    தருக்க ret = c->dispatch (src_base+src, std::forward<Ts> (ds)...);

    எனில் (ret || !has_null)
      s->add_link (*இது, s->எடு_pack ());
    இல்லையெனில்
      s->எடு_discard ();

    திரும்பு ret;
  }


  template <typename ...Ts>
  தருக்க serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *இது = 0;

    Type* obj = c->push<Type> ();
    தருக்க ret = obj->serialize (c, std::forward<Ts> (ds)...);

    எனில் (ret)
      c->add_link (*இது, c->எடு_pack ());
    இல்லையெனில்
      c->எடு_discard ();

    திரும்பு ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  template <typename ...Ts>
  தருக்க serialize_copy (hb_serialize_context_t *c, மாறிலி OffsetTo& src,
		       மாறிலி வெறுமை *src_base, unsigned dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *இது = 0;
    எனில் (src.is_null ())
      திரும்பு பொய்;

    c->push ();

    தருக்க ret = c->copy (src_base+src, std::forward<Ts> (ds)...);

    c->add_link (*இது, c->எடு_pack (), whence, dst_bias);

    திரும்பு ret;
  }

  தருக்க serialize_copy (hb_serialize_context_t *c, மாறிலி OffsetTo& src,
		       மாறிலி வெறுமை *src_base, unsigned dst_bias = 0)
  { திரும்பு serialize_copy (c, src, src_base, dst_bias, hb_serialize_context_t::Head); }

  தருக்க sanitize_shallow (hb_sanitize_context_t *c, மாறிலி BaseType *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!c->check_struct (இது))) return_trace (பொய்);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    எனில் (unlikely ((மாறிலி எழுத்து *) base + (unsigned) *இது < (மாறிலி எழுத்து *) base)) return_trace (பொய்);
    return_trace (உண்மை);
  }

  template <typename ...Ts>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி BaseType *base, Ts&&... ds) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (sanitize_shallow (c, base) &&
		  hb_barrier () &&
		  (இது->is_null () ||
		   c->dispatch (StructAtOffset<Type> (base, *இது), std::forward<Ts> (ds)...)));
  }

  DEFINE_SIZE_STATIC (அளவு (OffsetType));
};
/* Partial specializations. */
template <typename Type, typename BaseType=வெறுமை, தருக்க has_null=உண்மை> using Offset16To = OffsetTo<Type, HBUINT16, BaseType, has_null>;
template <typename Type, typename BaseType=வெறுமை, தருக்க has_null=உண்மை> using Offset24To = OffsetTo<Type, HBUINT24, BaseType, has_null>;
template <typename Type, typename BaseType=வெறுமை, தருக்க has_null=உண்மை> using Offset32To = OffsetTo<Type, HBUINT32, BaseType, has_null>;

template <typename Type, typename OffsetType, typename BaseType=வெறுமை> using NNOffsetTo = OffsetTo<Type, OffsetType, BaseType, பொய்>;
template <typename Type, typename BaseType=வெறுமை> using NNOffset16To = Offset16To<Type, BaseType, பொய்>;
template <typename Type, typename BaseType=வெறுமை> using NNOffset24To = Offset24To<Type, BaseType, பொய்>;
template <typename Type, typename BaseType=வெறுமை> using NNOffset32To = Offset32To<Type, BaseType, பொய்>;


/*
 * Array Types
 */

template <typename Type>
கட்டமைப்பு UnsizedArrayOf
{
  வகைவரை Type item_t;
  நிலையான constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (UnsizedArrayOf);

  மாறிலி Type& operator [] (unsigned முழு i) மாறிலி
  {
    திரும்பு arrayZ[i];
  }
  Type& operator [] (unsigned முழு i)
  {
    திரும்பு arrayZ[i];
  }

  நிலையான size_t get_size (unsigned முழு len)
  { திரும்பு hb_unsigned_mul_saturate (len, Type::static_size); }

  template <typename T> operator T * () { திரும்பு arrayZ; }
  template <typename T> operator மாறிலி T * () மாறிலி { திரும்பு arrayZ; }
  hb_array_t<Type> as_array (unsigned முழு len)
  { திரும்பு hb_array (arrayZ, len); }
  hb_array_t<மாறிலி Type> as_array (unsigned முழு len) மாறிலி
  { திரும்பு hb_array (arrayZ, len); }

  template <typename T>
  Type &lsearch (unsigned முழு len, மாறிலி T &x, Type &not_found = Crap (Type))
  { திரும்பு *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  மாறிலி Type &lsearch (unsigned முழு len, மாறிலி T &x, மாறிலி Type &not_found = Null (Type)) மாறிலி
  { திரும்பு *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  தருக்க lfind (unsigned முழு len, மாறிலி T &x, unsigned முழு *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned முழு to_store = (unsigned முழு) -1) மாறிலி
  { திரும்பு as_array (len).lfind (x, i, not_found, to_store); }

  வெறுமை qsort (unsigned முழு len, unsigned முழு start = 0, unsigned முழு end = (unsigned முழு) -1)
  { as_array (len).qsort (start, end); }

  தருக்க serialize (hb_serialize_context_t *c, unsigned முழு பொருட்கள்_len, தருக்க clear = உண்மை)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_size (இது, get_size (பொருட்கள்_len), clear))) return_trace (பொய்);
    return_trace (உண்மை);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  தருக்க serialize (hb_serialize_context_t *c, Iterator பொருட்கள்)
  {
    TRACE_SERIALIZE (இது);
    unsigned மொத்தம் = hb_len (பொருட்கள்);
    எனில் (unlikely (!serialize (c, மொத்தம், பொய்))) return_trace (பொய்);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    க்கு (unsigned i = 0; i < மொத்தம்; i++, ++பொருட்கள்)
      arrayZ[i] = *பொருட்கள்;
    return_trace (உண்மை);
  }

  UnsizedArrayOf* copy (hb_serialize_context_t *c, unsigned மொத்தம்) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto *out = c->start_embed (இது);
    எனில் (unlikely (!as_array (மொத்தம்).copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  தருக்க sanitize (hb_sanitize_context_t *c, unsigned முழு மொத்தம், Ts&&... ds) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!sanitize_shallow (c, மொத்தம்))) return_trace (பொய்);
    எனில் (!அளவு... (Ts) && hb_is_trivially_copyable(Type)) return_trace (உண்மை);
    hb_barrier ();
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (பொய்);
    return_trace (உண்மை);
  }

  தருக்க sanitize_shallow (hb_sanitize_context_t *c, unsigned முழு மொத்தம்) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_array (arrayZ, மொத்தம்));
  }

  பொது:
  Type		arrayZ[HB_VAR_ARRAY];
  பொது:
  DEFINE_SIZE_UNBOUNDED (0);
};

/* Unsized array of offset's */
template <typename Type, typename OffsetType, typename BaseType=வெறுமை, தருக்க has_null=உண்மை>
using UnsizedArray16OfOffsetTo = UnsizedArrayOf<OffsetTo<Type, OffsetType, BaseType, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
template <typename Type, typename OffsetType, typename BaseType=வெறுமை, தருக்க has_null=உண்மை>
கட்டமைப்பு UnsizedListOfOffset16To : UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
{
  மாறிலி Type& operator [] (முழு i_) மாறிலி
  {
    unsigned முழு i = (unsigned முழு) i_;
    மாறிலி OffsetTo<Type, OffsetType, BaseType, has_null> *p = &இது->arrayZ[i];
    எனில் (unlikely ((மாறிலி வெறுமை *) p < (மாறிலி வெறுமை *) இது->arrayZ)) திரும்பு Null (Type); /* Overflowed. */
    hb_barrier ();
    திரும்பு இது+*p;
  }
  Type& operator [] (முழு i_)
  {
    unsigned முழு i = (unsigned முழு) i_;
    மாறிலி OffsetTo<Type, OffsetType, BaseType, has_null> *p = &இது->arrayZ[i];
    எனில் (unlikely ((மாறிலி வெறுமை *) p < (மாறிலி வெறுமை *) இது->arrayZ)) திரும்பு Crap (Type); /* Overflowed. */
    hb_barrier ();
    திரும்பு இது+*p;
  }

  template <typename ...Ts>
  தருக்க sanitize (hb_sanitize_context_t *c, unsigned முழு மொத்தம், Ts&&... ds) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace ((UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
		   ::sanitize (c, மொத்தம், இது, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type>
கட்டமைப்பு SortedUnsizedArrayOf : UnsizedArrayOf<Type>
{
  hb_sorted_array_t<Type> as_array (unsigned முழு len)
  { திரும்பு hb_sorted_array (இது->arrayZ, len); }
  hb_sorted_array_t<மாறிலி Type> as_array (unsigned முழு len) மாறிலி
  { திரும்பு hb_sorted_array (இது->arrayZ, len); }
  operator hb_sorted_array_t<Type> ()             { திரும்பு as_array (); }
  operator hb_sorted_array_t<மாறிலி Type> () மாறிலி { திரும்பு as_array (); }

  template <typename T>
  Type &bsearch (unsigned முழு len, மாறிலி T &x, Type &not_found = Crap (Type))
  { திரும்பு *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  மாறிலி Type &bsearch (unsigned முழு len, மாறிலி T &x, மாறிலி Type &not_found = Null (Type)) மாறிலி
  { திரும்பு *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  தருக்க bfind (unsigned முழு len, மாறிலி T &x, unsigned முழு *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned முழு to_store = (unsigned முழு) -1) மாறிலி
  { திரும்பு as_array (len).bfind (x, i, not_found, to_store); }
};


/* An array with a number of elements. */
template <typename Type, typename LenType>
கட்டமைப்பு ArrayOf
{
  வகைவரை Type item_t;
  நிலையான constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOf);

  மாறிலி Type& operator [] (முழு i_) மாறிலி
  {
    unsigned முழு i = (unsigned முழு) i_;
    எனில் (unlikely (i >= len)) திரும்பு Null (Type);
    hb_barrier ();
    திரும்பு arrayZ[i];
  }
  Type& operator [] (முழு i_)
  {
    unsigned முழு i = (unsigned முழு) i_;
    எனில் (unlikely (i >= len)) திரும்பு Crap (Type);
    hb_barrier ();
    திரும்பு arrayZ[i];
  }

  size_t get_size () மாறிலி
  { திரும்பு hb_unsigned_mul_add_saturate (len, Type::static_size, len.static_size); }

  explicit operator தருக்க () மாறிலி { திரும்பு len; }

  வெறுமை எடு () { len--; }

  hb_array_t<      Type> as_array ()       { திரும்பு hb_array (arrayZ, len); }
  hb_array_t<மாறிலி Type> as_array () மாறிலி { திரும்பு hb_array (arrayZ, len); }

  /* Iterator. */
  வகைவரை hb_array_t<மாறிலி Type>   iter_t;
  வகைவரை hb_array_t<      Type> writer_t;
    iter_t   iter () மாறிலி { திரும்பு as_array (); }
  writer_t writer ()       { திரும்பு as_array (); }
  operator   iter_t () மாறிலி { திரும்பு   iter (); }
  operator writer_t ()       { திரும்பு writer (); }

  /* Faster range-based for loop. */
  மாறிலி Type *begin () மாறிலி { திரும்பு arrayZ; }
  மாறிலி Type *end () மாறிலி { திரும்பு arrayZ + len; }

  template <typename T>
  Type &lsearch (மாறிலி T &x, Type &not_found = Crap (Type))
  { திரும்பு *as_array ().lsearch (x, &not_found); }
  template <typename T>
  மாறிலி Type &lsearch (மாறிலி T &x, மாறிலி Type &not_found = Null (Type)) மாறிலி
  { திரும்பு *as_array ().lsearch (x, &not_found); }
  template <typename T>
  தருக்க lfind (மாறிலி T &x, unsigned முழு *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned முழு to_store = (unsigned முழு) -1) மாறிலி
  { திரும்பு as_array ().lfind (x, i, not_found, to_store); }

  வெறுமை qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD தருக்க serialize (hb_serialize_context_t *c, unsigned பொருட்கள்_len, தருக்க clear = உண்மை)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);
    c->check_assign (len, பொருட்கள்_len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    எனில் (unlikely (!c->extend_size (இது, get_size (), clear))) return_trace (பொய்);
    return_trace (உண்மை);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD தருக்க serialize (hb_serialize_context_t *c, Iterator பொருட்கள்)
  {
    TRACE_SERIALIZE (இது);
    unsigned மொத்தம் = hb_len (பொருட்கள்);
    எனில் (unlikely (!serialize (c, மொத்தம், பொய்))) return_trace (பொய்);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    க்கு (unsigned i = 0; i < மொத்தம்; i++, ++பொருட்கள்)
      arrayZ[i] = *பொருட்கள்;
    return_trace (உண்மை);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (இது);
    len++;
    எனில் (unlikely (!len || !c->extend (இது)))
    {
      len--;
      return_trace (nullptr);
    }
    return_trace (&arrayZ[len - 1]);
  }

  ArrayOf* copy (hb_serialize_context_t *c) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto *out = c->start_embed (இது);
    எனில் (unlikely (!c->extend_min (out))) return_trace (nullptr);
    c->check_assign (out->len, len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    எனில் (unlikely (!as_array ().copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  தருக்க sanitize (hb_sanitize_context_t *c, Ts&&... ds) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!sanitize_shallow (c))) return_trace (பொய்);
    எனில் (!அளவு... (Ts) && hb_is_trivially_copyable(Type)) return_trace (உண்மை);
    hb_barrier ();
    unsigned முழு மொத்தம் = len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (பொய்);
    return_trace (உண்மை);
  }

  தருக்க sanitize_shallow (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (len.sanitize (c) &&
		  hb_barrier () &&
		  c->check_array_sized (arrayZ, len, அளவு (LenType)));
  }

  பொது:
  LenType	len;
  Type		arrayZ[HB_VAR_ARRAY];
  பொது:
  DEFINE_SIZE_ARRAY (அளவு (LenType), arrayZ);
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
கட்டமைப்பு List16OfOffsetTo : ArrayOf<OffsetTo<Type, OffsetType>, HBUINT16>
{
  மாறிலி Type& operator [] (முழு i_) மாறிலி
  {
    unsigned முழு i = (unsigned முழு) i_;
    எனில் (unlikely (i >= இது->len)) திரும்பு Null (Type);
    hb_barrier ();
    திரும்பு இது+இது->arrayZ[i];
  }
  மாறிலி Type& operator [] (முழு i_)
  {
    unsigned முழு i = (unsigned முழு) i_;
    எனில் (unlikely (i >= இது->len)) திரும்பு Crap (Type);
    hb_barrier ();
    திரும்பு இது+இது->arrayZ[i];
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    கட்டமைப்பு List16OfOffsetTo *out = c->serializer->embed (*இது);
    எனில் (unlikely (!out)) return_trace (பொய்);
    unsigned முழு மொத்தம் = இது->len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      out->arrayZ[i].serialize_subset (c, இது->arrayZ[i], இது, out);
    return_trace (உண்மை);
  }

  template <typename ...Ts>
  தருக்க sanitize (hb_sanitize_context_t *c, Ts&&... ds) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace ((Array16Of<OffsetTo<Type, OffsetType>>::sanitize (c, இது, std::forward<Ts> (ds)...)));
  }
};

template <typename Type>
using List16OfOffset16To = List16OfOffsetTo<Type, HBUINT16>;

/* An array starting at second element. */
template <typename Type, typename LenType>
கட்டமைப்பு HeadlessArrayOf
{
  நிலையான constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (HeadlessArrayOf);

  மாறிலி Type& operator [] (முழு i_) மாறிலி
  {
    unsigned முழு i = (unsigned முழு) i_;
    எனில் (unlikely (i >= lenP1 || !i)) திரும்பு Null (Type);
    hb_barrier ();
    திரும்பு arrayZ[i-1];
  }
  Type& operator [] (முழு i_)
  {
    unsigned முழு i = (unsigned முழு) i_;
    எனில் (unlikely (i >= lenP1 || !i)) திரும்பு Crap (Type);
    hb_barrier ();
    திரும்பு arrayZ[i-1];
  }
  size_t get_size () மாறிலி
  { திரும்பு hb_unsigned_mul_add_saturate (get_length (), Type::static_size, lenP1.static_size); }

  unsigned get_length () மாறிலி { திரும்பு lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { திரும்பு hb_array (arrayZ, get_length ()); }
  hb_array_t<மாறிலி Type> as_array () மாறிலி { திரும்பு hb_array (arrayZ, get_length ()); }

  /* Iterator. */
  வகைவரை hb_array_t<மாறிலி Type>   iter_t;
  வகைவரை hb_array_t<      Type> writer_t;
    iter_t   iter () மாறிலி { திரும்பு as_array (); }
  writer_t writer ()       { திரும்பு as_array (); }
  operator   iter_t () மாறிலி { திரும்பு   iter (); }
  operator writer_t ()       { திரும்பு writer (); }

  /* Faster range-based for loop. */
  மாறிலி Type *begin () மாறிலி { திரும்பு arrayZ; }
  மாறிலி Type *end () மாறிலி { திரும்பு arrayZ + get_length (); }

  HB_NODISCARD தருக்க serialize (hb_serialize_context_t *c, unsigned முழு பொருட்கள்_len, தருக்க clear = உண்மை)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);
    c->check_assign (lenP1, பொருட்கள்_len + 1, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    எனில் (unlikely (!c->extend_size (இது, get_size (), clear))) return_trace (பொய்);
    return_trace (உண்மை);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD தருக்க serialize (hb_serialize_context_t *c, Iterator பொருட்கள்)
  {
    TRACE_SERIALIZE (இது);
    unsigned மொத்தம் = hb_len (பொருட்கள்);
    எனில் (unlikely (!serialize (c, மொத்தம், பொய்))) return_trace (பொய்);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    க்கு (unsigned i = 0; i < மொத்தம்; i++, ++பொருட்கள்)
      arrayZ[i] = *பொருட்கள்;
    return_trace (உண்மை);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  தருக்க sanitize (hb_sanitize_context_t *c, Ts&&... ds) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!sanitize_shallow (c))) return_trace (பொய்);
    எனில் (!அளவு... (Ts) && hb_is_trivially_copyable(Type)) return_trace (உண்மை);
    hb_barrier ();
    unsigned முழு மொத்தம் = get_length ();
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (பொய்);
    return_trace (உண்மை);
  }

  தனிப்பட்ட:
  தருக்க sanitize_shallow (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (lenP1.sanitize (c) &&
		  hb_barrier () &&
		  (!lenP1 || c->check_array_sized (arrayZ, lenP1 - 1, அளவு (LenType))));
  }

  பொது:
  LenType	lenP1;
  Type		arrayZ[HB_VAR_ARRAY];
  பொது:
  DEFINE_SIZE_ARRAY (அளவு (LenType), arrayZ);
};
template <typename Type> using HeadlessArray16Of = HeadlessArrayOf<Type, HBUINT16>;

/* An array storing length-1. */
template <typename Type, typename LenType=HBUINT16>
கட்டமைப்பு ArrayOfM1
{
  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOfM1);

  மாறிலி Type& operator [] (முழு i_) மாறிலி
  {
    unsigned முழு i = (unsigned முழு) i_;
    எனில் (unlikely (i > lenM1)) திரும்பு Null (Type);
    hb_barrier ();
    திரும்பு arrayZ[i];
  }
  Type& operator [] (முழு i_)
  {
    unsigned முழு i = (unsigned முழு) i_;
    எனில் (unlikely (i > lenM1)) திரும்பு Crap (Type);
    hb_barrier ();
    திரும்பு arrayZ[i];
  }
  size_t get_size () மாறிலி
  { திரும்பு hb_unsigned_mul_add_saturate (lenM1 + 1, Type::static_size, lenM1.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  தருக்க sanitize (hb_sanitize_context_t *c, Ts&&... ds) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!sanitize_shallow (c))) return_trace (பொய்);
    எனில் (!அளவு... (Ts) && hb_is_trivially_copyable(Type)) return_trace (உண்மை);
    hb_barrier ();
    unsigned முழு மொத்தம் = lenM1 + 1;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (பொய்);
    return_trace (உண்மை);
  }

  தனிப்பட்ட:
  தருக்க sanitize_shallow (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (lenM1.sanitize (c) &&
		  hb_barrier () &&
		  (c->check_array_sized (arrayZ, lenM1 + 1, அளவு (LenType))));
  }

  பொது:
  LenType	lenM1;
  Type		arrayZ[HB_VAR_ARRAY];
  பொது:
  DEFINE_SIZE_ARRAY (அளவு (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type, typename LenType>
கட்டமைப்பு SortedArrayOf : ArrayOf<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { திரும்பு hb_sorted_array (இது->arrayZ, இது->len); }
  hb_sorted_array_t<மாறிலி Type> as_array () மாறிலி { திரும்பு hb_sorted_array (இது->arrayZ, இது->len); }

  /* Iterator. */
  வகைவரை hb_sorted_array_t<மாறிலி Type>   iter_t;
  வகைவரை hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () மாறிலி { திரும்பு as_array (); }
  writer_t writer ()       { திரும்பு as_array (); }
  operator   iter_t () மாறிலி { திரும்பு   iter (); }
  operator writer_t ()       { திரும்பு writer (); }

  /* Faster range-based for loop. */
  மாறிலி Type *begin () மாறிலி { திரும்பு இது->arrayZ; }
  மாறிலி Type *end () மாறிலி { திரும்பு இது->arrayZ + இது->len; }

  தருக்க serialize (hb_serialize_context_t *c, unsigned முழு பொருட்கள்_len)
  {
    TRACE_SERIALIZE (இது);
    தருக்க ret = ArrayOf<Type, LenType>::serialize (c, பொருட்கள்_len);
    return_trace (ret);
  }
  template <typename Iterator,
	    hb_requires (hb_is_sorted_source_of (Iterator, Type))>
  தருக்க serialize (hb_serialize_context_t *c, Iterator பொருட்கள்)
  {
    TRACE_SERIALIZE (இது);
    தருக்க ret = ArrayOf<Type, LenType>::serialize (c, பொருட்கள்);
    return_trace (ret);
  }

  SortedArrayOf* copy (hb_serialize_context_t *c) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    SortedArrayOf* out = reinterpret_cast<SortedArrayOf *> (ArrayOf<Type, LenType>::copy (c));
    return_trace (out);
  }

  template <typename T>
  Type &bsearch (மாறிலி T &x, Type &not_found = Crap (Type))
  { திரும்பு *as_array ().bsearch (x, &not_found); }
  template <typename T>
  மாறிலி Type &bsearch (மாறிலி T &x, மாறிலி Type &not_found = Null (Type)) மாறிலி
  { திரும்பு *as_array ().bsearch (x, &not_found); }
  template <typename T>
  தருக்க bfind (மாறிலி T &x, unsigned முழு *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned முழு to_store = (unsigned முழு) -1) மாறிலி
  { திரும்பு as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type> using SortedArray16Of = SortedArrayOf<Type, HBUINT16>;
template <typename Type> using SortedArray24Of = SortedArrayOf<Type, HBUINT24>;
template <typename Type> using SortedArray32Of = SortedArrayOf<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

template <typename LenType=HBUINT16>
கட்டமைப்பு Bin_தேடல்_Header
{
  operator uint32_t () மாறிலி { திரும்பு len; }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  Bin_தேடல்_Header& operator = (unsigned முழு v)
  {
    len = v;
    ப்ரதிஜ்ஞா (len == v);
    entrySelector = hb_max (1u, hb_bit_storage (v)) - 1;
    தேடல்_Range = 16 * (1u << entrySelector);
    rangeShift = v * 16 > தேடல்_Range
		 ? 16 * v - தேடல்_Range
		 : 0;
    திரும்பு *இது;
  }

  பாதுகாக்கப்பட்ட:
  LenType	len;
  LenType	தேடல்_Range;
  LenType	entrySelector;
  LenType	rangeShift;

  பொது:
  DEFINE_SIZE_STATIC (8);
};

template <typename Type, typename LenType=HBUINT16>
using Bin_தேடல்_Array_Of = SortedArrayOf<Type, Bin_தேடல்_Header<LenType>>;


கட்டமைப்பு Var_Sized_Bin_தேடல்_Header
{

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  HBUINT16	unitSize;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	தேடல்_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	entrySelector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	rangeShift;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  பொது:
  DEFINE_SIZE_STATIC (10);
};

template <typename Type>
கட்டமைப்பு Var_Sized_Bin_தேடல்_Array_Of
{
  நிலையான constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (Var_Sized_Bin_தேடல்_Array_Of);

  தருக்க last_is_terminator () மாறிலி
  {
    எனில் (unlikely (!header.nUnits)) திரும்பு பொய்;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    மாறிலி HBUINT16 *சொற்கள் = &StructAtOffset<HBUINT16> (&bytesZ, (header.nUnits - 1) * header.unitSize);
    unsigned முழு மொத்தம் = Type::Termination_Word_மொத்தம்;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (சொற்கள்[i] != 0xFFFFu)
	திரும்பு பொய்;
    திரும்பு உண்மை;
  }

  மாறிலி Type& operator [] (முழு i_) மாறிலி
  {
    unsigned முழு i = (unsigned முழு) i_;
    எனில் (unlikely (i >= get_length ())) திரும்பு Null (Type);
    hb_barrier ();
    திரும்பு StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  Type& operator [] (முழு i_)
  {
    unsigned முழு i = (unsigned முழு) i_;
    எனில் (unlikely (i >= get_length ())) திரும்பு Crap (Type);
    hb_barrier ();
    திரும்பு StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  unsigned முழு get_length () மாறிலி
  { திரும்பு header.nUnits - last_is_terminator (); }
  size_t get_size () மாறிலி
  { திரும்பு hb_unsigned_mul_add_saturate (header.nUnits, header.unitSize, header.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  தருக்க sanitize (hb_sanitize_context_t *c, Ts&&... ds) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!sanitize_shallow (c))) return_trace (பொய்);
    எனில் (!அளவு... (Ts) && hb_is_trivially_copyable(Type)) return_trace (உண்மை);
    hb_barrier ();
    unsigned முழு மொத்தம் = get_length ();
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (unlikely (!(*இது)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (பொய்);
    return_trace (உண்மை);
  }

  template <typename T>
  மாறிலி Type *bsearch (மாறிலி T &key) மாறிலி
  {
    unsigned pos;
    திரும்பு hb_bsearch_impl (&pos,
			    key,
			    (மாறிலி வெறுமை *) bytesZ,
			    get_length (),
			    header.unitSize,
			    _hb_cmp_method<T, Type>)
	   ? (மாறிலி Type *) (((மாறிலி எழுத்து *) &bytesZ) + (pos * header.unitSize))
	   : nullptr;
  }

  தனிப்பட்ட:
  தருக்க sanitize_shallow (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (header.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_size <= header.unitSize &&
		  c->check_range (bytesZ.arrayZ,
				  header.nUnits,
				  header.unitSize));
  }

  பாதுகாக்கப்பட்ட:
  Var_Sized_Bin_தேடல்_Header	header;
  UnsizedArrayOf<HBUINT8>	bytesZ;
  பொது:
  DEFINE_SIZE_ARRAY (10, bytesZ);
};


/* CFF INDEX */

template <typename மொத்தம்_3>
கட்டமைப்பு CFFIndex
{
  unsigned முழு offset_array_size () மாறிலி
  { திரும்பு offSize * (மொத்தம் + 1); }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி Iterable &iterable,
		  மாறிலி unsigned *p_data_size = nullptr,
                  unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (இது);
    unsigned data_size;
    எனில் (p_data_size)
      data_size = *p_data_size;
    இல்லையெனில்
      total_size (iterable, &data_size);

    auto it = hb_iter (iterable);
    எனில் (unlikely (!serialize_header (c, +it, data_size, min_off_size))) return_trace (பொய்);
    unsigned எழுத்து *ret = c->allocate_size<unsigned எழுத்து> (data_size, பொய்);
    எனில் (unlikely (!ret)) return_trace (பொய்);
    unsigned remaining = data_size;
    க்கு (மாறிலி auto &_ : +it)
    {
      unsigned len = _.length;

      எனில் (!len)
	தொடர்;

      எனில் (unlikely (len > remaining)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (பொய், HB_SERIALIZE_ERROR_INT_OVERFLOW));
      }

      remaining -= len;

      எனில் (len <= 1)
      {
	*ret++ = *_.arrayZ;
	தொடர்;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (உண்மை);
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  தருக்க serialize_header (hb_serialize_context_t *c,
			 Iterator it,
			 unsigned data_size,
                         unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (இது);

    unsigned off_size = (hb_bit_storage (data_size + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    /* serialize CFFIndex header */
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);
    இது->மொத்தம் = hb_len (it);
    எனில் (!இது->மொத்தம்) return_trace (உண்மை);
    எனில் (unlikely (!c->extend (இது->offSize))) return_trace (பொய்);
    இது->offSize = off_size;
    எனில் (unlikely (!c->allocate_size<HBUINT8> (off_size * (இது->மொத்தம் + 1), பொய்)))
      return_trace (பொய்);

    /* serialize indices */
    unsigned முழு offset = 1;
    எனில் (HB_OPTIMIZE_SIZE_VAL)
    {
      unsigned முழு i = 0;
      க்கு (மாறிலி auto &_ : +it)
      {
	set_offset_at (i++, offset);
	offset += hb_len_of (_);
      }
      set_offset_at (i, offset);
    }
    இல்லையெனில்
      தேர்வு (off_size)
      {
	நிலை 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  க்கு (மாறிலி auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	நிறுத்து;
	நிலை 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  க்கு (மாறிலி auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	நிறுத்து;
	நிலை 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  க்கு (மாறிலி auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	நிறுத்து;
	நிலை 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  க்கு (மாறிலி auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	நிறுத்து;
	இயல்பு:
	நிறுத்து;
      }

    ப்ரதிஜ்ஞா (offset == data_size + 1);
    return_trace (உண்மை);
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  நிலையான unsigned total_size (மாறிலி Iterable &iterable, unsigned *data_size = nullptr, unsigned min_off_size = 0)
  {
    auto it = + hb_iter (iterable);
    எனில் (!it)
    {
      எனில் (data_size) *data_size = 0;
      திரும்பு min_size;
    }

    unsigned total = 0;
    க்கு (மாறிலி auto &_ : +it)
      total += hb_len_of (_);

    எனில் (data_size) *data_size = total;

    unsigned off_size = (hb_bit_storage (total + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    திரும்பு min_size + HBUINT8::static_size + (hb_len (it) + 1) * off_size + total;
  }

  வெறுமை set_offset_at (unsigned முழு index, unsigned முழு offset)
  {
    ப்ரதிஜ்ஞா (index <= மொத்தம்);

    unsigned முழு size = offSize;
    மாறிலி HBUINT8 *p = offsets;
    தேர்வு (size)
    {
      நிலை 1: ((HBUINT8  *) p)[index] = offset; நிறுத்து;
      நிலை 2: ((HBUINT16 *) p)[index] = offset; நிறுத்து;
      நிலை 3: ((HBUINT24 *) p)[index] = offset; நிறுத்து;
      நிலை 4: ((HBUINT32 *) p)[index] = offset; நிறுத்து;
      இயல்பு: திரும்பு;
    }
  }

  தனிப்பட்ட:
  unsigned முழு offset_at (unsigned முழு index) மாறிலி
  {
    ப்ரதிஜ்ஞா (index <= மொத்தம்);

    unsigned முழு size = offSize;
    மாறிலி HBUINT8 *p = offsets;
    தேர்வு (size)
    {
      நிலை 1: திரும்பு ((HBUINT8  *) p)[index];
      நிலை 2: திரும்பு ((HBUINT16 *) p)[index];
      நிலை 3: திரும்பு ((HBUINT24 *) p)[index];
      நிலை 4: திரும்பு ((HBUINT32 *) p)[index];
      இயல்பு: திரும்பு 0;
    }
  }

  மாறிலி unsigned எழுத்து *data_base () மாறிலி
  { திரும்பு (மாறிலி unsigned எழுத்து *) இது + min_size + offSize.static_size - 1 + offset_array_size (); }
  பொது:

  hb_ubytes_t operator [] (unsigned முழு index) மாறிலி
  {
    எனில் (unlikely (index >= மொத்தம்)) திரும்பு hb_ubytes_t ();
    hb_barrier ();
    unsigned offset0 = offset_at (index);
    unsigned offset1 = offset_at (index + 1);
    எனில் (unlikely (offset1 < offset0 || offset1 > offset_at (மொத்தம்)))
      திரும்பு hb_ubytes_t ();
    திரும்பு hb_ubytes_t (data_base () + offset0, offset1 - offset0);
  }

  size_t get_size () மாறிலி
  {
    எனில் (மொத்தம்)
      திரும்பு hb_unsigned_add_saturate (min_size, offSize.static_size,
				       offset_array_size (), offset_at (மொத்தம்) - 1);
    திரும்பு min_size;  /* empty CFFIndex contains count only */
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  hb_barrier () &&
			  (மொத்தம் == 0 || /* empty INDEX */
			   (மொத்தம் < மொத்தம் + 1u &&
			    c->check_struct (&offSize) && offSize >= 1 && offSize <= 4 &&
			    c->check_array (offsets, offSize, மொத்தம் + 1u) &&
			    c->check_range (data_base (), offset_at (மொத்தம்))))));
  }

  பொது:
  மொத்தம்_3		மொத்தம்;		/* Number of object data. Note there are (count+1) offsets */
  தனிப்பட்ட:
  HBUINT8	offSize;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  பொது:
  DEFINE_SIZE_MIN (மொத்தம்_3::static_size);
};
வகைவரை CFFIndex<HBUINT16> CFF1Index;
வகைவரை CFFIndex<HBUINT32> CFF2Index;


/* TupleValues */
கட்டமைப்பு Tuple_மதிப்புகள்
{
  எண்ணிக்கை packed_மதிப்பு_flag_t
  {
    மதிப்புகள்_ARE_ZEROS     = 0x80,
    மதிப்புகள்_ARE_BYTES     = 0x00,
    மதிப்புகள்_ARE_சொற்கள்     = 0x40,
    மதிப்புகள்_ARE_LONGS     = 0xC0,
    மதிப்புகள்_SIZE_MASK     = 0xC0,
    மதிப்பு_RUN_மொத்தம்_MASK = 0x3F
  };

  நிலையான unsigned compile_unsafe (hb_array_t<மாறிலி முழு> values, /* IN */
				  unsigned எழுத்து *encoded_bytes /* OUT */)
  {
    unsigned இலக்கம்_மதிப்புகள்_3 = values.length;
    unsigned encoded_len = 0;
    unsigned i = 0;
    போது (i < இலக்கம்_மதிப்புகள்_3)
    {
      முழு val = values.arrayZ[i];
      எனில் (val == 0)
        encoded_len += encode_மதிப்பு_run_as_zeroes (i, encoded_bytes + encoded_len, values);
      இல்லையெனில் எனில் ((int8_t) val == val)
        encoded_len += encode_மதிப்பு_run_as_bytes (i, encoded_bytes + encoded_len, values);
      இல்லையெனில் எனில் ((int16_t) val == val)
        encoded_len += encode_மதிப்பு_run_as_சொற்கள் (i, encoded_bytes + encoded_len, values);
      இல்லையெனில்
        encoded_len += encode_மதிப்பு_run_as_longs (i, encoded_bytes + encoded_len, values);
    }
    திரும்பு encoded_len;
  }

  நிலையான unsigned encode_மதிப்பு_run_as_zeroes (unsigned& i,
					      unsigned எழுத்து *it,
					      hb_array_t<மாறிலி முழு> values)
  {
    unsigned இலக்கம்_மதிப்புகள்_3 = values.length;
    unsigned run_length = 0;
    unsigned encoded_len = 0;
    போது (i < இலக்கம்_மதிப்புகள்_3 && values.arrayZ[i] == 0)
    {
      i++;
      run_length++;
    }

    போது (run_length >= 64)
    {
      *it++ = எழுத்து (மதிப்புகள்_ARE_ZEROS | 63);
      run_length -= 64;
      encoded_len++;
    }

    எனில் (run_length)
    {
      *it++ = எழுத்து (மதிப்புகள்_ARE_ZEROS | (run_length - 1));
      encoded_len++;
    }
    திரும்பு encoded_len;
  }

  நிலையான unsigned encode_மதிப்பு_run_as_bytes (unsigned &i,
					     unsigned எழுத்து *it,
					     hb_array_t<மாறிலி முழு> values)
  {
    unsigned start = i;
    unsigned இலக்கம்_மதிப்புகள்_3 = values.length;
    போது (i < இலக்கம்_மதிப்புகள்_3)
    {
      முழு val = values.arrayZ[i];
      எனில் ((int8_t) val != val)
        நிறுத்து;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      எனில் (val == 0 && i + 1 < இலக்கம்_மதிப்புகள்_3 && values.arrayZ[i+1] == 0)
        நிறுத்து;

      i++;
    }
    unsigned run_length = i - start;

    unsigned encoded_len = 0;

    போது (run_length >= 64)
    {
      *it++ = (மதிப்புகள்_ARE_BYTES | 63);
      encoded_len++;

      க்கு (unsigned j = 0; j < 64; j++)
	it[j] = static_cast<எழுத்து> (values.arrayZ[start + j]);
      it += 64;
      encoded_len += 64;

      start += 64;
      run_length -= 64;
    }

    எனில் (run_length)
    {
      *it++ = (மதிப்புகள்_ARE_BYTES | (run_length - 1));
      encoded_len++;

      க்கு (unsigned j = 0; j < run_length; j++)
        it[j] = static_cast<எழுத்து> (values.arrayZ[start + j]);
      encoded_len += run_length;
    }

    திரும்பு encoded_len;
  }

  நிலையான unsigned encode_மதிப்பு_run_as_சொற்கள் (unsigned &i,
					     unsigned எழுத்து *it,
					     hb_array_t<மாறிலி முழு> values)
  {
    unsigned start = i;
    unsigned இலக்கம்_மதிப்புகள்_3 = values.length;
    போது (i < இலக்கம்_மதிப்புகள்_3)
    {
      முழு val = values.arrayZ[i];

      எனில் ((int16_t) val != val)
        நிறுத்து;

      /* start a new run for a single zero value. */
      எனில் (val == 0) நிறுத்து;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      எனில் ((int8_t) val == val &&
          i + 1 < இலக்கம்_மதிப்புகள்_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        நிறுத்து;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    போது (run_length >= 64)
    {
      *it++ = (மதிப்புகள்_ARE_சொற்கள் | 63);
      encoded_len++;

      க்கு (unsigned j = 0; j < 64; j++)
      {
        int16_t மதிப்பு_val = values.arrayZ[start + j];
        *it++ = static_cast<எழுத்து> (மதிப்பு_val >> 8);
        *it++ = static_cast<எழுத்து> (மதிப்பு_val & 0xFF);

        encoded_len += 2;
      }

      start += 64;
      run_length -= 64;
    }

    எனில் (run_length)
    {
      *it++ = (மதிப்புகள்_ARE_சொற்கள் | (run_length - 1));
      encoded_len++;
      போது (start < i)
      {
        int16_t மதிப்பு_val = values.arrayZ[start++];
        *it++ = static_cast<எழுத்து> (மதிப்பு_val >> 8);
        *it++ = static_cast<எழுத்து> (மதிப்பு_val & 0xFF);

        encoded_len += 2;
      }
    }
    திரும்பு encoded_len;
  }

  நிலையான unsigned encode_மதிப்பு_run_as_longs (unsigned &i,
					     unsigned எழுத்து *it,
					     hb_array_t<மாறிலி முழு> values)
  {
    unsigned start = i;
    unsigned இலக்கம்_மதிப்புகள்_3 = values.length;
    போது (i < இலக்கம்_மதிப்புகள்_3)
    {
      முழு val = values.arrayZ[i];

      எனில் ((int16_t) val == val)
        நிறுத்து;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    போது (run_length >= 64)
    {
      *it++ = (மதிப்புகள்_ARE_LONGS | 63);
      encoded_len++;

      க்கு (unsigned j = 0; j < 64; j++)
      {
        int32_t மதிப்பு_val = values.arrayZ[start + j];
        *it++ = static_cast<எழுத்து> (மதிப்பு_val >> 24);
        *it++ = static_cast<எழுத்து> (மதிப்பு_val >> 16);
        *it++ = static_cast<எழுத்து> (மதிப்பு_val >> 8);
        *it++ = static_cast<எழுத்து> (மதிப்பு_val & 0xFF);

        encoded_len += 4;
      }

      start += 64;
      run_length -= 64;
    }

    எனில் (run_length)
    {
      *it++ = (மதிப்புகள்_ARE_LONGS | (run_length - 1));
      encoded_len++;
      போது (start < i)
      {
        int32_t மதிப்பு_val = values.arrayZ[start++];
        *it++ = static_cast<எழுத்து> (மதிப்பு_val >> 24);
        *it++ = static_cast<எழுத்து> (மதிப்பு_val >> 16);
        *it++ = static_cast<எழுத்து> (மதிப்பு_val >> 8);
        *it++ = static_cast<எழுத்து> (மதிப்பு_val & 0xFF);

        encoded_len += 4;
      }
    }
    திரும்பு encoded_len;
  }

  template <typename T>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  நிலையான தருக்க decompile (மாறிலி HBUINT8 *&p /* IN/OUT */,
			 hb_vector_t<T> &values /* IN/OUT */,
			 மாறிலி HBUINT8 *end,
			 தருக்க consume_all = பொய்,
			 unsigned start = 0)
  {
    unsigned i = 0;
    unsigned மொத்தம் = consume_all ? UINT_MAX : values.length;
    எனில் (consume_all)
      values.alloc ((end - p) / 2);
    போது (i < மொத்தம்)
    {
      எனில் (unlikely (p + 1 > end)) திரும்பு consume_all;
      unsigned control = *p++;
      unsigned run_மொத்தம் = (control & மதிப்பு_RUN_மொத்தம்_MASK) + 1;
      எனில் (consume_all)
      {
        எனில் (unlikely (!values.resize_dirty  (values.length + run_மொத்தம்)))
	  திரும்பு பொய்;
      }
      unsigned stop = i + run_மொத்தம்;
      எனில் (unlikely (stop > மொத்தம்)) திரும்பு பொய்;

      unsigned skip = i < start ? hb_min (start - i, run_மொத்தம்) : 0;
      i += skip;

      எனில் ((control & மதிப்புகள்_SIZE_MASK) == மதிப்புகள்_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * அளவு (T));
	i = stop;
      }
      இல்லையெனில் எனில் ((control & மதிப்புகள்_SIZE_MASK) ==  மதிப்புகள்_ARE_சொற்கள்)
      {
        எனில் (unlikely (p + run_மொத்தம் * HBINT16::static_size > end)) திரும்பு பொய்;
	p += skip * HBINT16::static_size;
#ifndef HB_OPTIMIZE_SIZE
        க்கு (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (மாறிலி HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 1] = * (மாறிலி HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 2] = * (மாறிலி HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 3] = * (மாறிலி HBINT16 *) p;
	  p += HBINT16::static_size;
	}
#endif
        க்கு (; i < stop; i++)
        {
          values.arrayZ[i] = * (மாறிலி HBINT16 *) p;
          p += HBINT16::static_size;
        }
      }
      இல்லையெனில் எனில் ((control & மதிப்புகள்_SIZE_MASK) ==  மதிப்புகள்_ARE_LONGS)
      {
        எனில் (unlikely (p + run_மொத்தம் * HBINT32::static_size > end)) திரும்பு பொய்;
	p += skip * HBINT32::static_size;
        க்கு (; i < stop; i++)
        {
          values.arrayZ[i] = * (மாறிலி HBINT32 *) p;
          p += HBINT32::static_size;
        }
      }
      இல்லையெனில் எனில் ((control & மதிப்புகள்_SIZE_MASK) ==  மதிப்புகள்_ARE_BYTES)
      {
        எனில் (unlikely (p + run_மொத்தம் > end)) திரும்பு பொய்;
	p += skip * HBINT8::static_size;
#ifndef HB_OPTIMIZE_SIZE
	க்கு (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (மாறிலி HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (மாறிலி HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (மாறிலி HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (மாறிலி HBINT8 *) p++;
	}
#endif
        க்கு (; i < stop; i++)
          values.arrayZ[i] = * (மாறிலி HBINT8 *) p++;
      }
    }
    திரும்பு உண்மை;
  }

  நிலையான தருக்க skip (மாறிலி unsigned எழுத்து *&p /* IN/OUT */,
		    மாறிலி unsigned எழுத்து *end,
		    unsigned மொத்தம்)
  {
    unsigned seen = 0;
    போது (seen < மொத்தம்)
    {
      எனில் (unlikely (p >= end)) திரும்பு பொய்;
      unsigned control = *p++;
      unsigned run_மொத்தம் = (control & மதிப்பு_RUN_மொத்தம்_MASK) + 1;
      எனில் (unlikely (run_மொத்தம் > மொத்தம் - seen)) திரும்பு பொய்;

      unsigned width = 0;
      தேர்வு (control & மதிப்புகள்_SIZE_MASK)
      {
	நிலை மதிப்புகள்_ARE_ZEROS: நிறுத்து;
	நிலை மதிப்புகள்_ARE_BYTES: width = HBINT8::static_size; நிறுத்து;
	நிலை மதிப்புகள்_ARE_சொற்கள்: width = HBINT16::static_size; நிறுத்து;
	நிலை மதிப்புகள்_ARE_LONGS: width = HBINT32::static_size; நிறுத்து;
      }
      எனில் (unlikely (unsigned (end - p) < run_மொத்தம் * width)) திரும்பு பொய்;
      p += run_மொத்தம் * width;
      seen += run_மொத்தம்;
    }
    திரும்பு உண்மை;
  }

  கட்டமைப்பு iter_t : hb_iter_with_fallback_t<iter_t, முழு>
  {
    iter_t (மாறிலி unsigned எழுத்து *p_, unsigned len_)
	    : p (p_), endp (p_ + len_)
    { எனில் (likely (ensure_run ())) read_மதிப்பு (); }

    தனிப்பட்ட:
    மாறிலி unsigned எழுத்து *p;
    மாறிலி unsigned எழுத்து * மாறிலி endp;
    முழு current_மதிப்பு = 0;
    signed run_மொத்தம் = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    தருக்க ensure_run ()
    {
      எனில் (likely (run_மொத்தம் > 0)) திரும்பு உண்மை;
      திரும்பு _ensure_run ();
    }
    தருக்க _ensure_run ()
    {
      எனில் (unlikely (p >= endp))
      {
        run_மொத்தம் = 0;
        current_மதிப்பு = 0;
	திரும்பு பொய்;
      }

      unsigned control = *p++;
      run_மொத்தம் = (control & மதிப்பு_RUN_மொத்தம்_MASK) + 1;
      width = control & மதிப்புகள்_SIZE_MASK;
      தேர்வு (width)
      {
        நிலை மதிப்புகள்_ARE_ZEROS: width = 0; நிறுத்து;
	நிலை மதிப்புகள்_ARE_BYTES: width = HBINT8::static_size;  நிறுத்து;
	நிலை மதிப்புகள்_ARE_சொற்கள்: width = HBINT16::static_size; நிறுத்து;
	நிலை மதிப்புகள்_ARE_LONGS: width = HBINT32::static_size; நிறுத்து;
	இயல்பு: ப்ரதிஜ்ஞா (பொய்);
      }

      எனில் (unlikely (p + run_மொத்தம் * width > endp))
      {
	run_மொத்தம் = 0;
	current_மதிப்பு = 0;
	திரும்பு பொய்;
      }

      திரும்பு உண்மை;
    }
    வெறுமை read_மதிப்பு ()
    {
      தேர்வு (width)
      {
        நிலை 0: current_மதிப்பு = 0; நிறுத்து;
	நிலை 1: current_மதிப்பு = * (மாறிலி HBINT8  *) p; நிறுத்து;
	நிலை 2: current_மதிப்பு = * (மாறிலி HBINT16 *) p; நிறுத்து;
	நிலை 4: current_மதிப்பு = * (மாறிலி HBINT32 *) p; நிறுத்து;
      }
      p += width;
    }

    பொது:

    வகைவரை முழு __item_t__;
    __item_t__ __item__ () மாறிலி
    { திரும்பு current_மதிப்பு; }

    தருக்க __more__ () மாறிலி { திரும்பு run_மொத்தம் || p < endp; }
    வெறுமை __next__ ()
    {
      run_மொத்தம்--;
      எனில் (unlikely (!ensure_run ()))
	திரும்பு;
      read_மதிப்பு ();
    }
    வெறுமை __forward__ (unsigned n)
    {
      எனில் (unlikely (!ensure_run ()))
	திரும்பு;
      போது (n)
      {
	unsigned i = hb_min (n, (unsigned) run_மொத்தம்);
	run_மொத்தம் -= i;
	n -= i;
	p += (i - 1) * width;
	எனில் (unlikely (!ensure_run ()))
	  திரும்பு;
	read_மதிப்பு ();
      }
    }
    தருக்க operator != (மாறிலி iter_t& o) மாறிலி
    { திரும்பு p != o.p || run_மொத்தம் != o.run_மொத்தம்; }
    iter_t __end__ () மாறிலி
    {
      iter_t it (endp, 0);
      திரும்பு it;
    }
  };

  கட்டமைப்பு fetcher_t
  {
    fetcher_t (மாறிலி unsigned எழுத்து *p_, unsigned len_)
	      : p (p_), end (p_ + len_) {}

    தனிப்பட்ட:
    மாறிலி unsigned எழுத்து *p;
    மாறிலி unsigned எழுத்து * மாறிலி end;
    signed run_மொத்தம் = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    தருக்க ensure_run ()
    {
      எனில் (likely (run_மொத்தம் > 0)) திரும்பு உண்மை;
      திரும்பு _ensure_run ();
    }

    தருக்க _ensure_run ()
    {
      எனில் (unlikely (p >= end))
      {
        run_மொத்தம் = 0;
	திரும்பு பொய்;
      }

      unsigned control = *p++;
      run_மொத்தம் = (control & மதிப்பு_RUN_மொத்தம்_MASK) + 1;
      width = control & மதிப்புகள்_SIZE_MASK;
      தேர்வு (width)
      {
        நிலை மதிப்புகள்_ARE_ZEROS: width = 0; நிறுத்து;
	நிலை மதிப்புகள்_ARE_BYTES: width = HBINT8::static_size;  நிறுத்து;
	நிலை மதிப்புகள்_ARE_சொற்கள்: width = HBINT16::static_size; நிறுத்து;
	நிலை மதிப்புகள்_ARE_LONGS: width = HBINT32::static_size; நிறுத்து;
	இயல்பு: ப்ரதிஜ்ஞா (பொய்);
      }

      எனில் (unlikely (p + run_மொத்தம் * width > end))
      {
	run_மொத்தம் = 0;
	திரும்பு பொய்;
      }

      திரும்பு உண்மை;
    }

    பொது:
    வெறுமை skip (unsigned n)
    {
      போது (n)
      {
	எனில் (unlikely (!ensure_run ()))
	  திரும்பு;
	unsigned i = hb_min (n, (unsigned) run_மொத்தம்);
	run_மொத்தம் -= i;
	n -= i;
	p += i * width;
      }
    }

    தனிப்பட்ட:
    template <தருக்க scaled, typename T>
    வெறுமை _add_to (hb_array_t<T> out, T scale = T (1))
    {
      unsigned n = out.length;
      T *arrayZ = out.arrayZ;

      க்கு (unsigned i = 0; i < n;)
      {
	எனில் (unlikely (!ensure_run ()))
	  நிறுத்து;
	unsigned மொத்தம் = hb_min (n - i, (unsigned) run_மொத்தம்);
	தேர்வு (width)
	{
	  நிலை 0:
	  {
	    arrayZ += மொத்தம்;
	    நிறுத்து;
	  }
	  நிலை 1:
	  {
	    மாறிலி auto *pp = (மாறிலி HBINT8 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    க்கு (; j + 3 < மொத்தம்; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    க்கு (; j < மொத்தம்; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (மாறிலி unsigned எழுத்து *) pp;
	  }
	  நிறுத்து;
	  நிலை 2:
	  {
	    மாறிலி auto *pp = (மாறிலி HBINT16 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    க்கு (; j + 3 < மொத்தம்; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    க்கு (; j < மொத்தம்; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (மாறிலி unsigned எழுத்து *) pp;
	  }
	  நிறுத்து;
	  நிலை 4:
	  {
	    மாறிலி auto *pp = (மாறிலி HBINT32 *) p;
	    க்கு (unsigned j = 0; j < மொத்தம்; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (மாறிலி unsigned எழுத்து *) pp;
	  }
	  நிறுத்து;
	}
	run_மொத்தம் -= மொத்தம்;
	i += மொத்தம்;
      }
    }

    பொது:
    template <typename T>
    வெறுமை add_to (hb_array_t<T> out, தசம scale = 1.0f)
    {
#ifndef HB_OPTIMIZE_SIZE
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      எனில் (பொய் && scale == 1.0f)
        _add_to<பொய்> (out);
      இல்லையெனில்
#endif
        _add_to<உண்மை> (out, (T) scale);
    }
  };
};

கட்டமைப்பு TupleList : CFF2Index
{
  Tuple_மதிப்புகள்::iter_t operator [] (unsigned i) மாறிலி
  {
    auto bytes = CFF2Index::operator [] (i);
    திரும்பு Tuple_மதிப்புகள்::iter_t (bytes.arrayZ, bytes.length);
  }

  Tuple_மதிப்புகள்::fetcher_t fetcher (unsigned i) மாறிலி
  {
    auto bytes = CFF2Index::operator [] (i);
    திரும்பு Tuple_மதிப்புகள்::fetcher_t (bytes.arrayZ, bytes.length);
  }
};


// Alignment

template <unsigned முழு alignment>
கட்டமைப்பு Align
{
  size_t get_size (மாறிலி வெறுமை *base) மாறிலி
  {
    unsigned offset = (மாறிலி எழுத்து *) இது - (மாறிலி எழுத்து *) base;
    திரும்பு (alignment - offset) & (alignment - 1);
  }

  பொது:
  DEFINE_SIZE_MIN (0);
};



} /* namespace OT */


#endif /* HB_OPEN_TYPE_HH */
