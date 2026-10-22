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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-blob.hh"
#അന്തര്ഭാവ "hb-face.hh"
#അന്തര്ഭാവ "hb-machinery.hh"
#അന്തര്ഭാവ "hb-meta.hh"
#അന്തര്ഭാവ "hb-subset.hh"


നാമാകാശ OT {


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
template <തർക്കം BE,
	  typename Type,
	  unsigned പൂർണ്ണസംഖ്യ Size = വലുപ്പം (Type)>
ഘടന അക്കം_Type
{
  തരനിർവചനം Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  തരനിർവചനം typename std::conditional<std::is_integral<Type>::value && വലുപ്പം (Type) <= വലുപ്പം(പൂർണ്ണസംഖ്യ),
				     typename std::conditional<std::is_signed<Type>::value, signed, unsigned>::type,
				     Type>::type WideType;

  അക്കം_Type () = സ്ഥിരസ്ഥിതി;
  explicit constexpr അക്കം_Type (Type V) : v {V} {}
  അക്കം_Type& operator = (Type V) { v = V; തിരികെ *ഇത്; }

  operator WideType () സ്ഥിരം { തിരികെ v; }

  തർക്കം operator == (സ്ഥിരം അക്കം_Type &o) സ്ഥിരം { തിരികെ (Type) v == (Type) o.v; }
  തർക്കം operator != (സ്ഥിരം അക്കം_Type &o) സ്ഥിരം { തിരികെ !(*ഇത് == o); }

  അക്കം_Type& operator += (WideType എണ്ണം) { *ഇത് = *ഇത് + എണ്ണം; തിരികെ *ഇത്; }
  അക്കം_Type& operator -= (WideType എണ്ണം) { *ഇത് = *ഇത് - എണ്ണം; തിരികെ *ഇത്; }
  അക്കം_Type& operator ++ () { *ഇത് += 1; തിരികെ *ഇത്; }
  അക്കം_Type& operator -- () { *ഇത് -= 1; തിരികെ *ഇത്; }
  അക്കം_Type operator ++ (പൂർണ്ണസംഖ്യ) { അക്കം_Type c (*ഇത്); ++*ഇത്; തിരികെ c; }
  അക്കം_Type operator -- (പൂർണ്ണസംഖ്യ) { അക്കം_Type c (*ഇത്); --*ഇത്; തിരികെ c; }

  uint32_t hash () സ്ഥിരം { തിരികെ hb_array ((സ്ഥിരം അക്ഷരം *) &v, വലുപ്പം (v)).hash (); }
  HB_INTERNAL സ്ഥിതം പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം അക്കം_Type *a, സ്ഥിരം അക്കം_Type *b)
  { തിരികെ b->cmp (*a); }
  HB_INTERNAL സ്ഥിതം പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
  {
    അക്കം_Type *pa = (അക്കം_Type *) a;
    അക്കം_Type *pb = (അക്കം_Type *) b;

    തിരികെ pb->cmp (*pa);
  }
  template <typename Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  പൂർണ്ണസംഖ്യ cmp (Type2 a) സ്ഥിരം
  {
    Type b = v;
    തിരികെ (a > b) - (a < b);
  }
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }
  സംരക്ഷിത:
  typename std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, Size>,
			    HBFloat<BE, Type, Size>>::type v;
  പൊതു:
  DEFINE_SIZE_STATIC (Size);
};

തരനിർവചനം അക്കം_Type<സത്യം, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
തരനിർവചനം അക്കം_Type<സത്യം, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
തരനിർവചനം അക്കം_Type<സത്യം, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
തരനിർവചനം അക്കം_Type<സത്യം, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
തരനിർവചനം അക്കം_Type<സത്യം, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
തരനിർവചനം അക്കം_Type<സത്യം, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
തരനിർവചനം അക്കം_Type<സത്യം, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
തരനിർവചനം അക്കം_Type<സത്യം, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
തരനിർവചനം അക്കം_Type<സത്യം, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

തരനിർവചനം അക്കം_Type<അസത്യം, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
തരനിർവചനം അക്കം_Type<അസത്യം, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
തരനിർവചനം അക്കം_Type<അസത്യം, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
തരനിർവചനം അക്കം_Type<അസത്യം, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
തരനിർവചനം അക്കം_Type<അസത്യം, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
തരനിർവചനം അക്കം_Type<അസത്യം, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

തരനിർവചനം അക്കം_Type<സത്യം,  ദശാംശം>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
തരനിർവചനം അക്കം_Type<സത്യം,  ഇരട്ട> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
തരനിർവചനം അക്കം_Type<അസത്യം, ദശാംശം>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
തരനിർവചനം അക്കം_Type<അസത്യം, ഇരട്ട> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
ഘടന HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& operator = (uint16_t i ) { HBUINT16::operator= (i); തിരികെ *ഇത്; }
  പൊതു:
  DEFINE_SIZE_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
ഘടന HBUINT32VAR
{
  size_t get_size () സ്ഥിരം
  {
    unsigned b0 = v[0];
    എങ്കിൽ (b0 < 0x80)
      തിരികെ 1;
    അല്ലെങ്കിൽ എങ്കിൽ (b0 < 0xC0)
      തിരികെ 2;
    അല്ലെങ്കിൽ എങ്കിൽ (b0 < 0xE0)
      തിരികെ 3;
    അല്ലെങ്കിൽ എങ്കിൽ (b0 < 0xF0)
      തിരികെ 4;
    അല്ലെങ്കിൽ
      തിരികെ 5;
  }

  സ്ഥിതം size_t get_size (uint32_t v)
  {
    എങ്കിൽ (v < 0x80)
      തിരികെ 1;
    അല്ലെങ്കിൽ എങ്കിൽ (v < 0x4000)
      തിരികെ 2;
    അല്ലെങ്കിൽ എങ്കിൽ (v < 0x200000)
      തിരികെ 3;
    അല്ലെങ്കിൽ എങ്കിൽ (v < 0x10000000)
      തിരികെ 4;
    അല്ലെങ്കിൽ
      തിരികെ 5;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_size ()));
  }

  operator uint32_t () സ്ഥിരം
  {
    unsigned b0 = v[0];
    എങ്കിൽ (b0 < 0x80)
      തിരികെ b0;
    അല്ലെങ്കിൽ എങ്കിൽ (b0 < 0xC0)
      തിരികെ ((b0 & 0x3F) << 8) | v[1];
    അല്ലെങ്കിൽ എങ്കിൽ (b0 < 0xE0)
      തിരികെ ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    അല്ലെങ്കിൽ എങ്കിൽ (b0 < 0xF0)
      തിരികെ ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    അല്ലെങ്കിൽ
      തിരികെ (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  സ്ഥിതം unsigned serialize_unsafe (unsigned അക്ഷരം *buf, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned അക്ഷരം *p = buf + len;
    വേണ്ടി (unsigned i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    എങ്കിൽ (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    തിരികെ len;
  }

  സ്ഥിതം തർക്കം serialize (hb_serialize_context_t *c, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned അക്ഷരം *buf = c->allocate_size<unsigned അക്ഷരം> (len, അസത്യം);
    എങ്കിൽ (unlikely (!buf))
      തിരികെ അസത്യം;

    serialize_unsafe (buf, v);
    തിരികെ സത്യം;
  }

  സംരക്ഷിത:
  unsigned അക്ഷരം v[5];

  പൊതു:
  DEFINE_SIZE_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
തരനിർവചനം HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
തരനിർവചനം HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
തരനിർവചനം HBUINT16 UFWORD;

template <typename Type, unsigned fraction_bits>
ഘടന HBFixed : Type
{
  സ്ഥിതം constexpr ദശാംശം mult = 1.f / (1 << fraction_bits);
  static_assert (Type::static_size * 8 > fraction_bits, "");

  operator signed () സ്ഥിരം = delete;
  operator unsigned () സ്ഥിരം = delete;
  explicit operator ദശാംശം () സ്ഥിരം { തിരികെ to_float (); }
  typename Type::type to_int () സ്ഥിരം { തിരികെ Type::v; }
  ശൂന്യം set_int (typename Type::type i ) { Type::v = i; }
  ദശാംശം to_float (ദശാംശം offset = 0) സ്ഥിരം  { തിരികെ ((int32_t) Type::v + offset) * mult; }
  ശൂന്യം set_float (ദശാംശം f) { Type::v = roundf (f / mult); }
  പൊതു:
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
ഘടന LONGDATETIME
{
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }
  സംരക്ഷിത:
  HBINT32 major;
  HBUINT32 minor;
  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
ഘടന Tag : HBUINT32
{
  Tag& operator = (hb_tag_t i) { HBUINT32::operator= (i); തിരികെ *ഇത്; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  operator സ്ഥിരം അക്ഷരം* () സ്ഥിരം { തിരികെ reinterpret_cast<സ്ഥിരം അക്ഷരം *> (ഇത്); }
  operator അക്ഷരം* ()             { തിരികെ reinterpret_cast<അക്ഷരം *> (ഇത്); }
  പൊതു:
  DEFINE_SIZE_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
ഘടന HBGlyphID16 : HBUINT16
{
  HBGlyphID16& operator = (uint16_t i) { HBUINT16::operator= (i); തിരികെ *ഇത്; }
};
ഘടന HBGlyphID24 : HBUINT24
{
  HBGlyphID24& operator = (uint32_t i) { HBUINT24::operator= (i); തിരികെ *ഇത്; }
};

/* Script/language-system/feature index */
ഘടന Index : HBUINT16 {
  സ്ഥിതം constexpr unsigned NOT_FOUND_INDEX = 0xFFFFu;
  Index& operator = (uint16_t i) { HBUINT16::operator= (i); തിരികെ *ഇത്; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, Index);

തരനിർവചനം Index NameID;

ഘടന VarIdx : HBUINT32 {
  സ്ഥിതം constexpr unsigned NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_INDEX, "");
  VarIdx& operator = (uint32_t i) { HBUINT32::operator= (i); തിരികെ *ഇത്; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, VarIdx);

/* Offset, Null offset = 0 */
template <typename Type, തർക്കം has_null=സത്യം>
ഘടന Offset : Type
{
  Offset& operator = (typename Type::type i) { Type::operator= (i); തിരികെ *ഇത്; }

  തരനിർവചനം Type type;

  തർക്കം is_null () സ്ഥിരം { തിരികെ has_null && 0 == *ഇത്; }

  പൊതു:
  DEFINE_SIZE_STATIC (വലുപ്പം (Type));
};

തരനിർവചനം Offset<HBUINT16> Offset16;
തരനിർവചനം Offset<HBUINT24> Offset24;
തരനിർവചനം Offset<HBUINT32> Offset32;


/* CheckSum */
ഘടന CheckSum : HBUINT32
{
  CheckSum& operator = (uint32_t i) { HBUINT32::operator= (i); തിരികെ *ഇത്; }

  /* This is reference implementation from the spec. */
  സ്ഥിതം uint32_t CalcTableChecksum (സ്ഥിരം HBUINT32 *Table, uint32_t Length)
  {
    uint32_t Sum = 0L;
    പ്രതിജ്ഞാ (0 == (Length & 3));
    സ്ഥിരം HBUINT32 *EndPtr = Table + Length / HBUINT32::static_size;

    ആകുമ്പോൾ (Table < EndPtr)
      Sum += *Table++;
    തിരികെ Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  ശൂന്യം set_for_data (സ്ഥിരം ശൂന്യം *data, unsigned പൂർണ്ണസംഖ്യ length)
  { *ഇത് = CalcTableChecksum ((സ്ഥിരം HBUINT32 *) data, length); }

  പൊതു:
  DEFINE_SIZE_STATIC (4);
};


/*
 * Version Numbers
 */

template <typename FixedType=HBUINT16>
ഘടന FixedVersion
{
  uint32_t to_int () സ്ഥിരം { തിരികെ (major << (വലുപ്പം (FixedType) * 8)) + minor; }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  FixedType major;
  FixedType minor;
  പൊതു:
  DEFINE_SIZE_STATIC (2 * വലുപ്പം (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

template <typename Type, തർക്കം has_null>
ഘടന _hb_has_null
{
  സ്ഥിതം സ്ഥിരം Type *get_null () { തിരികെ nullptr; }
  സ്ഥിതം Type *get_crap ()       { തിരികെ nullptr; }
};
template <typename Type>
ഘടന _hb_has_null<Type, സത്യം>
{
  സ്ഥിതം സ്ഥിരം Type *get_null () { തിരികെ &Null (Type); }
  സ്ഥിതം       Type *get_crap () { തിരികെ &Crap (Type); }
};

template <typename Type, typename OffsetType, typename BaseType=ശൂന്യം, തർക്കം has_null=സത്യം>
ഘടന OffsetTo : Offset<OffsetType, has_null>
{
  using target_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == അസത്യം ||
		 (hb_has_null_size (Type) || !hb_has_min_size (Type)), "");

  HB_DELETE_COPY_ASSIGN (OffsetTo);
  OffsetTo () = സ്ഥിരസ്ഥിതി;

  OffsetTo& operator = (typename OffsetType::type i) { OffsetType::operator= (i); തിരികെ *ഇത്; }

  സ്ഥിരം Type& operator () (സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (ഇത്->is_null ())) തിരികെ *_hb_has_null<Type, has_null>::get_null ();
    തിരികെ StructAtOffset<സ്ഥിരം Type> (base, *ഇത്);
  }
  Type& operator () (ശൂന്യം *base) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (ഇത്->is_null ())) തിരികെ *_hb_has_null<Type, has_null>::get_crap ();
    തിരികെ StructAtOffset<Type> (base, *ഇത്);
  }

  template <typename Base,
	    hb_enable_if (hb_is_convertible (സ്ഥിരം Base, സ്ഥിരം BaseType *))>
  friend സ്ഥിരം Type& operator + (സ്ഥിരം Base &base, സ്ഥിരം OffsetTo &offset) { തിരികെ offset ((സ്ഥിരം ശൂന്യം *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (സ്ഥിരം Base, സ്ഥിരം BaseType *))>
  friend സ്ഥിരം Type& operator + (സ്ഥിരം OffsetTo &offset, സ്ഥിരം Base &base) { തിരികെ offset ((സ്ഥിരം ശൂന്യം *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (Base &&base, OffsetTo &offset) { തിരികെ offset ((ശൂന്യം *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (OffsetTo &offset, Base &&base) { തിരികെ offset ((ശൂന്യം *) base); }


  template <typename Base, typename ...Ts>
  തർക്കം serialize_subset (hb_subset_context_t *c, സ്ഥിരം OffsetTo& src,
			 സ്ഥിരം Base *src_base, Ts&&... ds)
  {
    *ഇത് = 0;
    എങ്കിൽ (src.is_null ())
      തിരികെ അസത്യം;

    auto *s = c->serializer;

    s->push ();

    തർക്കം ret = c->dispatch (src_base+src, std::forward<Ts> (ds)...);

    എങ്കിൽ (ret || !has_null)
      s->add_link (*ഇത്, s->എടുക്കുക_pack ());
    അല്ലെങ്കിൽ
      s->എടുക്കുക_discard ();

    തിരികെ ret;
  }


  template <typename ...Ts>
  തർക്കം serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *ഇത് = 0;

    Type* obj = c->push<Type> ();
    തർക്കം ret = obj->serialize (c, std::forward<Ts> (ds)...);

    എങ്കിൽ (ret)
      c->add_link (*ഇത്, c->എടുക്കുക_pack ());
    അല്ലെങ്കിൽ
      c->എടുക്കുക_discard ();

    തിരികെ ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  template <typename ...Ts>
  തർക്കം serialize_copy (hb_serialize_context_t *c, സ്ഥിരം OffsetTo& src,
		       സ്ഥിരം ശൂന്യം *src_base, unsigned dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *ഇത് = 0;
    എങ്കിൽ (src.is_null ())
      തിരികെ അസത്യം;

    c->push ();

    തർക്കം ret = c->copy (src_base+src, std::forward<Ts> (ds)...);

    c->add_link (*ഇത്, c->എടുക്കുക_pack (), whence, dst_bias);

    തിരികെ ret;
  }

  തർക്കം serialize_copy (hb_serialize_context_t *c, സ്ഥിരം OffsetTo& src,
		       സ്ഥിരം ശൂന്യം *src_base, unsigned dst_bias = 0)
  { തിരികെ serialize_copy (c, src, src_base, dst_bias, hb_serialize_context_t::Head); }

  തർക്കം sanitize_shallow (hb_sanitize_context_t *c, സ്ഥിരം BaseType *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->check_struct (ഇത്))) return_trace (അസത്യം);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    എങ്കിൽ (unlikely ((സ്ഥിരം അക്ഷരം *) base + (unsigned) *ഇത് < (സ്ഥിരം അക്ഷരം *) base)) return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  template <typename ...Ts>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം BaseType *base, Ts&&... ds) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (sanitize_shallow (c, base) &&
		  hb_barrier () &&
		  (ഇത്->is_null () ||
		   c->dispatch (StructAtOffset<Type> (base, *ഇത്), std::forward<Ts> (ds)...)));
  }

  DEFINE_SIZE_STATIC (വലുപ്പം (OffsetType));
};
/* Partial specializations. */
template <typename Type, typename BaseType=ശൂന്യം, തർക്കം has_null=സത്യം> using Offset16To = OffsetTo<Type, HBUINT16, BaseType, has_null>;
template <typename Type, typename BaseType=ശൂന്യം, തർക്കം has_null=സത്യം> using Offset24To = OffsetTo<Type, HBUINT24, BaseType, has_null>;
template <typename Type, typename BaseType=ശൂന്യം, തർക്കം has_null=സത്യം> using Offset32To = OffsetTo<Type, HBUINT32, BaseType, has_null>;

template <typename Type, typename OffsetType, typename BaseType=ശൂന്യം> using NNOffsetTo = OffsetTo<Type, OffsetType, BaseType, അസത്യം>;
template <typename Type, typename BaseType=ശൂന്യം> using NNOffset16To = Offset16To<Type, BaseType, അസത്യം>;
template <typename Type, typename BaseType=ശൂന്യം> using NNOffset24To = Offset24To<Type, BaseType, അസത്യം>;
template <typename Type, typename BaseType=ശൂന്യം> using NNOffset32To = Offset32To<Type, BaseType, അസത്യം>;


/*
 * Array Types
 */

template <typename Type>
ഘടന UnsizedArrayOf
{
  തരനിർവചനം Type item_t;
  സ്ഥിതം constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (UnsizedArrayOf);

  സ്ഥിരം Type& operator [] (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  {
    തിരികെ arrayZ[i];
  }
  Type& operator [] (unsigned പൂർണ്ണസംഖ്യ i)
  {
    തിരികെ arrayZ[i];
  }

  സ്ഥിതം size_t get_size (unsigned പൂർണ്ണസംഖ്യ len)
  { തിരികെ hb_unsigned_mul_saturate (len, Type::static_size); }

  template <typename T> operator T * () { തിരികെ arrayZ; }
  template <typename T> operator സ്ഥിരം T * () സ്ഥിരം { തിരികെ arrayZ; }
  hb_array_t<Type> as_array (unsigned പൂർണ്ണസംഖ്യ len)
  { തിരികെ hb_array (arrayZ, len); }
  hb_array_t<സ്ഥിരം Type> as_array (unsigned പൂർണ്ണസംഖ്യ len) സ്ഥിരം
  { തിരികെ hb_array (arrayZ, len); }

  template <typename T>
  Type &lsearch (unsigned പൂർണ്ണസംഖ്യ len, സ്ഥിരം T &x, Type &not_found = Crap (Type))
  { തിരികെ *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  സ്ഥിരം Type &lsearch (unsigned പൂർണ്ണസംഖ്യ len, സ്ഥിരം T &x, സ്ഥിരം Type &not_found = Null (Type)) സ്ഥിരം
  { തിരികെ *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  തർക്കം lfind (unsigned പൂർണ്ണസംഖ്യ len, സ്ഥിരം T &x, unsigned പൂർണ്ണസംഖ്യ *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned പൂർണ്ണസംഖ്യ to_store = (unsigned പൂർണ്ണസംഖ്യ) -1) സ്ഥിരം
  { തിരികെ as_array (len).lfind (x, i, not_found, to_store); }

  ശൂന്യം qsort (unsigned പൂർണ്ണസംഖ്യ len, unsigned പൂർണ്ണസംഖ്യ start = 0, unsigned പൂർണ്ണസംഖ്യ end = (unsigned പൂർണ്ണസംഖ്യ) -1)
  { as_array (len).qsort (start, end); }

  തർക്കം serialize (hb_serialize_context_t *c, unsigned പൂർണ്ണസംഖ്യ ഇനങ്ങൾ_len, തർക്കം clear = സത്യം)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_size (ഇത്, get_size (ഇനങ്ങൾ_len), clear))) return_trace (അസത്യം);
    return_trace (സത്യം);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  തർക്കം serialize (hb_serialize_context_t *c, Iterator ഇനങ്ങൾ)
  {
    TRACE_SERIALIZE (ഇത്);
    unsigned എണ്ണം = hb_len (ഇനങ്ങൾ);
    എങ്കിൽ (unlikely (!serialize (c, എണ്ണം, അസത്യം))) return_trace (അസത്യം);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++, ++ഇനങ്ങൾ)
      arrayZ[i] = *ഇനങ്ങൾ;
    return_trace (സത്യം);
  }

  UnsizedArrayOf* copy (hb_serialize_context_t *c, unsigned എണ്ണം) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto *out = c->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!as_array (എണ്ണം).copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  തർക്കം sanitize (hb_sanitize_context_t *c, unsigned പൂർണ്ണസംഖ്യ എണ്ണം, Ts&&... ds) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!sanitize_shallow (c, എണ്ണം))) return_trace (അസത്യം);
    എങ്കിൽ (!വലുപ്പം... (Ts) && hb_is_trivially_copyable(Type)) return_trace (സത്യം);
    hb_barrier ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  തർക്കം sanitize_shallow (hb_sanitize_context_t *c, unsigned പൂർണ്ണസംഖ്യ എണ്ണം) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_array (arrayZ, എണ്ണം));
  }

  പൊതു:
  Type		arrayZ[HB_VAR_ARRAY];
  പൊതു:
  DEFINE_SIZE_UNBOUNDED (0);
};

/* Unsized array of offset's */
template <typename Type, typename OffsetType, typename BaseType=ശൂന്യം, തർക്കം has_null=സത്യം>
using UnsizedArray16OfOffsetTo = UnsizedArrayOf<OffsetTo<Type, OffsetType, BaseType, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
template <typename Type, typename OffsetType, typename BaseType=ശൂന്യം, തർക്കം has_null=സത്യം>
ഘടന UnsizedListOfOffset16To : UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
{
  സ്ഥിരം Type& operator [] (പൂർണ്ണസംഖ്യ i_) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ i = (unsigned പൂർണ്ണസംഖ്യ) i_;
    സ്ഥിരം OffsetTo<Type, OffsetType, BaseType, has_null> *p = &ഇത്->arrayZ[i];
    എങ്കിൽ (unlikely ((സ്ഥിരം ശൂന്യം *) p < (സ്ഥിരം ശൂന്യം *) ഇത്->arrayZ)) തിരികെ Null (Type); /* Overflowed. */
    hb_barrier ();
    തിരികെ ഇത്+*p;
  }
  Type& operator [] (പൂർണ്ണസംഖ്യ i_)
  {
    unsigned പൂർണ്ണസംഖ്യ i = (unsigned പൂർണ്ണസംഖ്യ) i_;
    സ്ഥിരം OffsetTo<Type, OffsetType, BaseType, has_null> *p = &ഇത്->arrayZ[i];
    എങ്കിൽ (unlikely ((സ്ഥിരം ശൂന്യം *) p < (സ്ഥിരം ശൂന്യം *) ഇത്->arrayZ)) തിരികെ Crap (Type); /* Overflowed. */
    hb_barrier ();
    തിരികെ ഇത്+*p;
  }

  template <typename ...Ts>
  തർക്കം sanitize (hb_sanitize_context_t *c, unsigned പൂർണ്ണസംഖ്യ എണ്ണം, Ts&&... ds) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace ((UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
		   ::sanitize (c, എണ്ണം, ഇത്, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type>
ഘടന SortedUnsizedArrayOf : UnsizedArrayOf<Type>
{
  hb_sorted_array_t<Type> as_array (unsigned പൂർണ്ണസംഖ്യ len)
  { തിരികെ hb_sorted_array (ഇത്->arrayZ, len); }
  hb_sorted_array_t<സ്ഥിരം Type> as_array (unsigned പൂർണ്ണസംഖ്യ len) സ്ഥിരം
  { തിരികെ hb_sorted_array (ഇത്->arrayZ, len); }
  operator hb_sorted_array_t<Type> ()             { തിരികെ as_array (); }
  operator hb_sorted_array_t<സ്ഥിരം Type> () സ്ഥിരം { തിരികെ as_array (); }

  template <typename T>
  Type &bsearch (unsigned പൂർണ്ണസംഖ്യ len, സ്ഥിരം T &x, Type &not_found = Crap (Type))
  { തിരികെ *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  സ്ഥിരം Type &bsearch (unsigned പൂർണ്ണസംഖ്യ len, സ്ഥിരം T &x, സ്ഥിരം Type &not_found = Null (Type)) സ്ഥിരം
  { തിരികെ *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  തർക്കം bfind (unsigned പൂർണ്ണസംഖ്യ len, സ്ഥിരം T &x, unsigned പൂർണ്ണസംഖ്യ *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned പൂർണ്ണസംഖ്യ to_store = (unsigned പൂർണ്ണസംഖ്യ) -1) സ്ഥിരം
  { തിരികെ as_array (len).bfind (x, i, not_found, to_store); }
};


/* An array with a number of elements. */
template <typename Type, typename LenType>
ഘടന ArrayOf
{
  തരനിർവചനം Type item_t;
  സ്ഥിതം constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOf);

  സ്ഥിരം Type& operator [] (പൂർണ്ണസംഖ്യ i_) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ i = (unsigned പൂർണ്ണസംഖ്യ) i_;
    എങ്കിൽ (unlikely (i >= len)) തിരികെ Null (Type);
    hb_barrier ();
    തിരികെ arrayZ[i];
  }
  Type& operator [] (പൂർണ്ണസംഖ്യ i_)
  {
    unsigned പൂർണ്ണസംഖ്യ i = (unsigned പൂർണ്ണസംഖ്യ) i_;
    എങ്കിൽ (unlikely (i >= len)) തിരികെ Crap (Type);
    hb_barrier ();
    തിരികെ arrayZ[i];
  }

  size_t get_size () സ്ഥിരം
  { തിരികെ hb_unsigned_mul_add_saturate (len, Type::static_size, len.static_size); }

  explicit operator തർക്കം () സ്ഥിരം { തിരികെ len; }

  ശൂന്യം എടുക്കുക () { len--; }

  hb_array_t<      Type> as_array ()       { തിരികെ hb_array (arrayZ, len); }
  hb_array_t<സ്ഥിരം Type> as_array () സ്ഥിരം { തിരികെ hb_array (arrayZ, len); }

  /* Iterator. */
  തരനിർവചനം hb_array_t<സ്ഥിരം Type>   iter_t;
  തരനിർവചനം hb_array_t<      Type> writer_t;
    iter_t   iter () സ്ഥിരം { തിരികെ as_array (); }
  writer_t writer ()       { തിരികെ as_array (); }
  operator   iter_t () സ്ഥിരം { തിരികെ   iter (); }
  operator writer_t ()       { തിരികെ writer (); }

  /* Faster range-based for loop. */
  സ്ഥിരം Type *begin () സ്ഥിരം { തിരികെ arrayZ; }
  സ്ഥിരം Type *end () സ്ഥിരം { തിരികെ arrayZ + len; }

  template <typename T>
  Type &lsearch (സ്ഥിരം T &x, Type &not_found = Crap (Type))
  { തിരികെ *as_array ().lsearch (x, &not_found); }
  template <typename T>
  സ്ഥിരം Type &lsearch (സ്ഥിരം T &x, സ്ഥിരം Type &not_found = Null (Type)) സ്ഥിരം
  { തിരികെ *as_array ().lsearch (x, &not_found); }
  template <typename T>
  തർക്കം lfind (സ്ഥിരം T &x, unsigned പൂർണ്ണസംഖ്യ *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned പൂർണ്ണസംഖ്യ to_store = (unsigned പൂർണ്ണസംഖ്യ) -1) സ്ഥിരം
  { തിരികെ as_array ().lfind (x, i, not_found, to_store); }

  ശൂന്യം qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD തർക്കം serialize (hb_serialize_context_t *c, unsigned ഇനങ്ങൾ_len, തർക്കം clear = സത്യം)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    c->check_assign (len, ഇനങ്ങൾ_len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    എങ്കിൽ (unlikely (!c->extend_size (ഇത്, get_size (), clear))) return_trace (അസത്യം);
    return_trace (സത്യം);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD തർക്കം serialize (hb_serialize_context_t *c, Iterator ഇനങ്ങൾ)
  {
    TRACE_SERIALIZE (ഇത്);
    unsigned എണ്ണം = hb_len (ഇനങ്ങൾ);
    എങ്കിൽ (unlikely (!serialize (c, എണ്ണം, അസത്യം))) return_trace (അസത്യം);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++, ++ഇനങ്ങൾ)
      arrayZ[i] = *ഇനങ്ങൾ;
    return_trace (സത്യം);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (ഇത്);
    len++;
    എങ്കിൽ (unlikely (!len || !c->extend (ഇത്)))
    {
      len--;
      return_trace (nullptr);
    }
    return_trace (&arrayZ[len - 1]);
  }

  ArrayOf* copy (hb_serialize_context_t *c) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto *out = c->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (out))) return_trace (nullptr);
    c->check_assign (out->len, len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    എങ്കിൽ (unlikely (!as_array ().copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  തർക്കം sanitize (hb_sanitize_context_t *c, Ts&&... ds) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!sanitize_shallow (c))) return_trace (അസത്യം);
    എങ്കിൽ (!വലുപ്പം... (Ts) && hb_is_trivially_copyable(Type)) return_trace (സത്യം);
    hb_barrier ();
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  തർക്കം sanitize_shallow (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (len.sanitize (c) &&
		  hb_barrier () &&
		  c->check_array_sized (arrayZ, len, വലുപ്പം (LenType)));
  }

  പൊതു:
  LenType	len;
  Type		arrayZ[HB_VAR_ARRAY];
  പൊതു:
  DEFINE_SIZE_ARRAY (വലുപ്പം (LenType), arrayZ);
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
ഘടന List16OfOffsetTo : ArrayOf<OffsetTo<Type, OffsetType>, HBUINT16>
{
  സ്ഥിരം Type& operator [] (പൂർണ്ണസംഖ്യ i_) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ i = (unsigned പൂർണ്ണസംഖ്യ) i_;
    എങ്കിൽ (unlikely (i >= ഇത്->len)) തിരികെ Null (Type);
    hb_barrier ();
    തിരികെ ഇത്+ഇത്->arrayZ[i];
  }
  സ്ഥിരം Type& operator [] (പൂർണ്ണസംഖ്യ i_)
  {
    unsigned പൂർണ്ണസംഖ്യ i = (unsigned പൂർണ്ണസംഖ്യ) i_;
    എങ്കിൽ (unlikely (i >= ഇത്->len)) തിരികെ Crap (Type);
    hb_barrier ();
    തിരികെ ഇത്+ഇത്->arrayZ[i];
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    ഘടന List16OfOffsetTo *out = c->serializer->embed (*ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = ഇത്->len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      out->arrayZ[i].serialize_subset (c, ഇത്->arrayZ[i], ഇത്, out);
    return_trace (സത്യം);
  }

  template <typename ...Ts>
  തർക്കം sanitize (hb_sanitize_context_t *c, Ts&&... ds) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace ((Array16Of<OffsetTo<Type, OffsetType>>::sanitize (c, ഇത്, std::forward<Ts> (ds)...)));
  }
};

template <typename Type>
using List16OfOffset16To = List16OfOffsetTo<Type, HBUINT16>;

/* An array starting at second element. */
template <typename Type, typename LenType>
ഘടന HeadlessArrayOf
{
  സ്ഥിതം constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (HeadlessArrayOf);

  സ്ഥിരം Type& operator [] (പൂർണ്ണസംഖ്യ i_) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ i = (unsigned പൂർണ്ണസംഖ്യ) i_;
    എങ്കിൽ (unlikely (i >= lenP1 || !i)) തിരികെ Null (Type);
    hb_barrier ();
    തിരികെ arrayZ[i-1];
  }
  Type& operator [] (പൂർണ്ണസംഖ്യ i_)
  {
    unsigned പൂർണ്ണസംഖ്യ i = (unsigned പൂർണ്ണസംഖ്യ) i_;
    എങ്കിൽ (unlikely (i >= lenP1 || !i)) തിരികെ Crap (Type);
    hb_barrier ();
    തിരികെ arrayZ[i-1];
  }
  size_t get_size () സ്ഥിരം
  { തിരികെ hb_unsigned_mul_add_saturate (get_length (), Type::static_size, lenP1.static_size); }

  unsigned get_length () സ്ഥിരം { തിരികെ lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { തിരികെ hb_array (arrayZ, get_length ()); }
  hb_array_t<സ്ഥിരം Type> as_array () സ്ഥിരം { തിരികെ hb_array (arrayZ, get_length ()); }

  /* Iterator. */
  തരനിർവചനം hb_array_t<സ്ഥിരം Type>   iter_t;
  തരനിർവചനം hb_array_t<      Type> writer_t;
    iter_t   iter () സ്ഥിരം { തിരികെ as_array (); }
  writer_t writer ()       { തിരികെ as_array (); }
  operator   iter_t () സ്ഥിരം { തിരികെ   iter (); }
  operator writer_t ()       { തിരികെ writer (); }

  /* Faster range-based for loop. */
  സ്ഥിരം Type *begin () സ്ഥിരം { തിരികെ arrayZ; }
  സ്ഥിരം Type *end () സ്ഥിരം { തിരികെ arrayZ + get_length (); }

  HB_NODISCARD തർക്കം serialize (hb_serialize_context_t *c, unsigned പൂർണ്ണസംഖ്യ ഇനങ്ങൾ_len, തർക്കം clear = സത്യം)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    c->check_assign (lenP1, ഇനങ്ങൾ_len + 1, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    എങ്കിൽ (unlikely (!c->extend_size (ഇത്, get_size (), clear))) return_trace (അസത്യം);
    return_trace (സത്യം);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD തർക്കം serialize (hb_serialize_context_t *c, Iterator ഇനങ്ങൾ)
  {
    TRACE_SERIALIZE (ഇത്);
    unsigned എണ്ണം = hb_len (ഇനങ്ങൾ);
    എങ്കിൽ (unlikely (!serialize (c, എണ്ണം, അസത്യം))) return_trace (അസത്യം);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++, ++ഇനങ്ങൾ)
      arrayZ[i] = *ഇനങ്ങൾ;
    return_trace (സത്യം);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  തർക്കം sanitize (hb_sanitize_context_t *c, Ts&&... ds) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!sanitize_shallow (c))) return_trace (അസത്യം);
    എങ്കിൽ (!വലുപ്പം... (Ts) && hb_is_trivially_copyable(Type)) return_trace (സത്യം);
    hb_barrier ();
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = get_length ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  സ്വകാര്യ:
  തർക്കം sanitize_shallow (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (lenP1.sanitize (c) &&
		  hb_barrier () &&
		  (!lenP1 || c->check_array_sized (arrayZ, lenP1 - 1, വലുപ്പം (LenType))));
  }

  പൊതു:
  LenType	lenP1;
  Type		arrayZ[HB_VAR_ARRAY];
  പൊതു:
  DEFINE_SIZE_ARRAY (വലുപ്പം (LenType), arrayZ);
};
template <typename Type> using HeadlessArray16Of = HeadlessArrayOf<Type, HBUINT16>;

/* An array storing length-1. */
template <typename Type, typename LenType=HBUINT16>
ഘടന ArrayOfM1
{
  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOfM1);

  സ്ഥിരം Type& operator [] (പൂർണ്ണസംഖ്യ i_) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ i = (unsigned പൂർണ്ണസംഖ്യ) i_;
    എങ്കിൽ (unlikely (i > lenM1)) തിരികെ Null (Type);
    hb_barrier ();
    തിരികെ arrayZ[i];
  }
  Type& operator [] (പൂർണ്ണസംഖ്യ i_)
  {
    unsigned പൂർണ്ണസംഖ്യ i = (unsigned പൂർണ്ണസംഖ്യ) i_;
    എങ്കിൽ (unlikely (i > lenM1)) തിരികെ Crap (Type);
    hb_barrier ();
    തിരികെ arrayZ[i];
  }
  size_t get_size () സ്ഥിരം
  { തിരികെ hb_unsigned_mul_add_saturate (lenM1 + 1, Type::static_size, lenM1.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  തർക്കം sanitize (hb_sanitize_context_t *c, Ts&&... ds) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!sanitize_shallow (c))) return_trace (അസത്യം);
    എങ്കിൽ (!വലുപ്പം... (Ts) && hb_is_trivially_copyable(Type)) return_trace (സത്യം);
    hb_barrier ();
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = lenM1 + 1;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  സ്വകാര്യ:
  തർക്കം sanitize_shallow (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (lenM1.sanitize (c) &&
		  hb_barrier () &&
		  (c->check_array_sized (arrayZ, lenM1 + 1, വലുപ്പം (LenType))));
  }

  പൊതു:
  LenType	lenM1;
  Type		arrayZ[HB_VAR_ARRAY];
  പൊതു:
  DEFINE_SIZE_ARRAY (വലുപ്പം (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type, typename LenType>
ഘടന SortedArrayOf : ArrayOf<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { തിരികെ hb_sorted_array (ഇത്->arrayZ, ഇത്->len); }
  hb_sorted_array_t<സ്ഥിരം Type> as_array () സ്ഥിരം { തിരികെ hb_sorted_array (ഇത്->arrayZ, ഇത്->len); }

  /* Iterator. */
  തരനിർവചനം hb_sorted_array_t<സ്ഥിരം Type>   iter_t;
  തരനിർവചനം hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () സ്ഥിരം { തിരികെ as_array (); }
  writer_t writer ()       { തിരികെ as_array (); }
  operator   iter_t () സ്ഥിരം { തിരികെ   iter (); }
  operator writer_t ()       { തിരികെ writer (); }

  /* Faster range-based for loop. */
  സ്ഥിരം Type *begin () സ്ഥിരം { തിരികെ ഇത്->arrayZ; }
  സ്ഥിരം Type *end () സ്ഥിരം { തിരികെ ഇത്->arrayZ + ഇത്->len; }

  തർക്കം serialize (hb_serialize_context_t *c, unsigned പൂർണ്ണസംഖ്യ ഇനങ്ങൾ_len)
  {
    TRACE_SERIALIZE (ഇത്);
    തർക്കം ret = ArrayOf<Type, LenType>::serialize (c, ഇനങ്ങൾ_len);
    return_trace (ret);
  }
  template <typename Iterator,
	    hb_requires (hb_is_sorted_source_of (Iterator, Type))>
  തർക്കം serialize (hb_serialize_context_t *c, Iterator ഇനങ്ങൾ)
  {
    TRACE_SERIALIZE (ഇത്);
    തർക്കം ret = ArrayOf<Type, LenType>::serialize (c, ഇനങ്ങൾ);
    return_trace (ret);
  }

  SortedArrayOf* copy (hb_serialize_context_t *c) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    SortedArrayOf* out = reinterpret_cast<SortedArrayOf *> (ArrayOf<Type, LenType>::copy (c));
    return_trace (out);
  }

  template <typename T>
  Type &bsearch (സ്ഥിരം T &x, Type &not_found = Crap (Type))
  { തിരികെ *as_array ().bsearch (x, &not_found); }
  template <typename T>
  സ്ഥിരം Type &bsearch (സ്ഥിരം T &x, സ്ഥിരം Type &not_found = Null (Type)) സ്ഥിരം
  { തിരികെ *as_array ().bsearch (x, &not_found); }
  template <typename T>
  തർക്കം bfind (സ്ഥിരം T &x, unsigned പൂർണ്ണസംഖ്യ *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned പൂർണ്ണസംഖ്യ to_store = (unsigned പൂർണ്ണസംഖ്യ) -1) സ്ഥിരം
  { തിരികെ as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type> using SortedArray16Of = SortedArrayOf<Type, HBUINT16>;
template <typename Type> using SortedArray24Of = SortedArrayOf<Type, HBUINT24>;
template <typename Type> using SortedArray32Of = SortedArrayOf<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

template <typename LenType=HBUINT16>
ഘടന Bin_തിരയൽ_Header
{
  operator uint32_t () സ്ഥിരം { തിരികെ len; }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  Bin_തിരയൽ_Header& operator = (unsigned പൂർണ്ണസംഖ്യ v)
  {
    len = v;
    പ്രതിജ്ഞാ (len == v);
    entrySelector = hb_max (1u, hb_bit_storage (v)) - 1;
    തിരയൽ_Range = 16 * (1u << entrySelector);
    rangeShift = v * 16 > തിരയൽ_Range
		 ? 16 * v - തിരയൽ_Range
		 : 0;
    തിരികെ *ഇത്;
  }

  സംരക്ഷിത:
  LenType	len;
  LenType	തിരയൽ_Range;
  LenType	entrySelector;
  LenType	rangeShift;

  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

template <typename Type, typename LenType=HBUINT16>
using Bin_തിരയൽ_Array_Of = SortedArrayOf<Type, Bin_തിരയൽ_Header<LenType>>;


ഘടന Var_Sized_Bin_തിരയൽ_Header
{

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  HBUINT16	unitSize;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	തിരയൽ_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	entrySelector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	rangeShift;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  പൊതു:
  DEFINE_SIZE_STATIC (10);
};

template <typename Type>
ഘടന Var_Sized_Bin_തിരയൽ_Array_Of
{
  സ്ഥിതം constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (Var_Sized_Bin_തിരയൽ_Array_Of);

  തർക്കം last_is_terminator () സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!header.nUnits)) തിരികെ അസത്യം;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    സ്ഥിരം HBUINT16 *വാക്കുകൾ = &StructAtOffset<HBUINT16> (&bytesZ, (header.nUnits - 1) * header.unitSize);
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = Type::Termination_Word_എണ്ണം;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (വാക്കുകൾ[i] != 0xFFFFu)
	തിരികെ അസത്യം;
    തിരികെ സത്യം;
  }

  സ്ഥിരം Type& operator [] (പൂർണ്ണസംഖ്യ i_) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ i = (unsigned പൂർണ്ണസംഖ്യ) i_;
    എങ്കിൽ (unlikely (i >= get_length ())) തിരികെ Null (Type);
    hb_barrier ();
    തിരികെ StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  Type& operator [] (പൂർണ്ണസംഖ്യ i_)
  {
    unsigned പൂർണ്ണസംഖ്യ i = (unsigned പൂർണ്ണസംഖ്യ) i_;
    എങ്കിൽ (unlikely (i >= get_length ())) തിരികെ Crap (Type);
    hb_barrier ();
    തിരികെ StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  unsigned പൂർണ്ണസംഖ്യ get_length () സ്ഥിരം
  { തിരികെ header.nUnits - last_is_terminator (); }
  size_t get_size () സ്ഥിരം
  { തിരികെ hb_unsigned_mul_add_saturate (header.nUnits, header.unitSize, header.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  തർക്കം sanitize (hb_sanitize_context_t *c, Ts&&... ds) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!sanitize_shallow (c))) return_trace (അസത്യം);
    എങ്കിൽ (!വലുപ്പം... (Ts) && hb_is_trivially_copyable(Type)) return_trace (സത്യം);
    hb_barrier ();
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = get_length ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (unlikely (!(*ഇത്)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  template <typename T>
  സ്ഥിരം Type *bsearch (സ്ഥിരം T &key) സ്ഥിരം
  {
    unsigned pos;
    തിരികെ hb_bsearch_impl (&pos,
			    key,
			    (സ്ഥിരം ശൂന്യം *) bytesZ,
			    get_length (),
			    header.unitSize,
			    _hb_cmp_method<T, Type>)
	   ? (സ്ഥിരം Type *) (((സ്ഥിരം അക്ഷരം *) &bytesZ) + (pos * header.unitSize))
	   : nullptr;
  }

  സ്വകാര്യ:
  തർക്കം sanitize_shallow (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (header.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_size <= header.unitSize &&
		  c->check_range (bytesZ.arrayZ,
				  header.nUnits,
				  header.unitSize));
  }

  സംരക്ഷിത:
  Var_Sized_Bin_തിരയൽ_Header	header;
  UnsizedArrayOf<HBUINT8>	bytesZ;
  പൊതു:
  DEFINE_SIZE_ARRAY (10, bytesZ);
};


/* CFF INDEX */

template <typename എണ്ണം_3>
ഘടന CFFIndex
{
  unsigned പൂർണ്ണസംഖ്യ offset_array_size () സ്ഥിരം
  { തിരികെ offSize * (എണ്ണം + 1); }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം Iterable &iterable,
		  സ്ഥിരം unsigned *p_data_size = nullptr,
                  unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (ഇത്);
    unsigned data_size;
    എങ്കിൽ (p_data_size)
      data_size = *p_data_size;
    അല്ലെങ്കിൽ
      total_size (iterable, &data_size);

    auto it = hb_iter (iterable);
    എങ്കിൽ (unlikely (!serialize_header (c, +it, data_size, min_off_size))) return_trace (അസത്യം);
    unsigned അക്ഷരം *ret = c->allocate_size<unsigned അക്ഷരം> (data_size, അസത്യം);
    എങ്കിൽ (unlikely (!ret)) return_trace (അസത്യം);
    unsigned remaining = data_size;
    വേണ്ടി (സ്ഥിരം auto &_ : +it)
    {
      unsigned len = _.length;

      എങ്കിൽ (!len)
	തുടരുക;

      എങ്കിൽ (unlikely (len > remaining)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (അസത്യം, HB_SERIALIZE_ERROR_INT_OVERFLOW));
      }

      remaining -= len;

      എങ്കിൽ (len <= 1)
      {
	*ret++ = *_.arrayZ;
	തുടരുക;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (സത്യം);
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  തർക്കം serialize_header (hb_serialize_context_t *c,
			 Iterator it,
			 unsigned data_size,
                         unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (ഇത്);

    unsigned off_size = (hb_bit_storage (data_size + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    /* serialize CFFIndex header */
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    ഇത്->എണ്ണം = hb_len (it);
    എങ്കിൽ (!ഇത്->എണ്ണം) return_trace (സത്യം);
    എങ്കിൽ (unlikely (!c->extend (ഇത്->offSize))) return_trace (അസത്യം);
    ഇത്->offSize = off_size;
    എങ്കിൽ (unlikely (!c->allocate_size<HBUINT8> (off_size * (ഇത്->എണ്ണം + 1), അസത്യം)))
      return_trace (അസത്യം);

    /* serialize indices */
    unsigned പൂർണ്ണസംഖ്യ offset = 1;
    എങ്കിൽ (HB_OPTIMIZE_SIZE_VAL)
    {
      unsigned പൂർണ്ണസംഖ്യ i = 0;
      വേണ്ടി (സ്ഥിരം auto &_ : +it)
      {
	set_offset_at (i++, offset);
	offset += hb_len_of (_);
      }
      set_offset_at (i, offset);
    }
    അല്ലെങ്കിൽ
      തിരഞ്ഞെടുപ്പ് (off_size)
      {
	അവസ്ഥ 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  വേണ്ടി (സ്ഥിരം auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	നിർത്തുക;
	അവസ്ഥ 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  വേണ്ടി (സ്ഥിരം auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	നിർത്തുക;
	അവസ്ഥ 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  വേണ്ടി (സ്ഥിരം auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	നിർത്തുക;
	അവസ്ഥ 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  വേണ്ടി (സ്ഥിരം auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
	നിർത്തുക;
      }

    പ്രതിജ്ഞാ (offset == data_size + 1);
    return_trace (സത്യം);
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  സ്ഥിതം unsigned total_size (സ്ഥിരം Iterable &iterable, unsigned *data_size = nullptr, unsigned min_off_size = 0)
  {
    auto it = + hb_iter (iterable);
    എങ്കിൽ (!it)
    {
      എങ്കിൽ (data_size) *data_size = 0;
      തിരികെ min_size;
    }

    unsigned total = 0;
    വേണ്ടി (സ്ഥിരം auto &_ : +it)
      total += hb_len_of (_);

    എങ്കിൽ (data_size) *data_size = total;

    unsigned off_size = (hb_bit_storage (total + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    തിരികെ min_size + HBUINT8::static_size + (hb_len (it) + 1) * off_size + total;
  }

  ശൂന്യം set_offset_at (unsigned പൂർണ്ണസംഖ്യ index, unsigned പൂർണ്ണസംഖ്യ offset)
  {
    പ്രതിജ്ഞാ (index <= എണ്ണം);

    unsigned പൂർണ്ണസംഖ്യ size = offSize;
    സ്ഥിരം HBUINT8 *p = offsets;
    തിരഞ്ഞെടുപ്പ് (size)
    {
      അവസ്ഥ 1: ((HBUINT8  *) p)[index] = offset; നിർത്തുക;
      അവസ്ഥ 2: ((HBUINT16 *) p)[index] = offset; നിർത്തുക;
      അവസ്ഥ 3: ((HBUINT24 *) p)[index] = offset; നിർത്തുക;
      അവസ്ഥ 4: ((HBUINT32 *) p)[index] = offset; നിർത്തുക;
      സ്ഥിരസ്ഥിതി: തിരികെ;
    }
  }

  സ്വകാര്യ:
  unsigned പൂർണ്ണസംഖ്യ offset_at (unsigned പൂർണ്ണസംഖ്യ index) സ്ഥിരം
  {
    പ്രതിജ്ഞാ (index <= എണ്ണം);

    unsigned പൂർണ്ണസംഖ്യ size = offSize;
    സ്ഥിരം HBUINT8 *p = offsets;
    തിരഞ്ഞെടുപ്പ് (size)
    {
      അവസ്ഥ 1: തിരികെ ((HBUINT8  *) p)[index];
      അവസ്ഥ 2: തിരികെ ((HBUINT16 *) p)[index];
      അവസ്ഥ 3: തിരികെ ((HBUINT24 *) p)[index];
      അവസ്ഥ 4: തിരികെ ((HBUINT32 *) p)[index];
      സ്ഥിരസ്ഥിതി: തിരികെ 0;
    }
  }

  സ്ഥിരം unsigned അക്ഷരം *data_base () സ്ഥിരം
  { തിരികെ (സ്ഥിരം unsigned അക്ഷരം *) ഇത് + min_size + offSize.static_size - 1 + offset_array_size (); }
  പൊതു:

  hb_ubytes_t operator [] (unsigned പൂർണ്ണസംഖ്യ index) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (index >= എണ്ണം)) തിരികെ hb_ubytes_t ();
    hb_barrier ();
    unsigned offset0 = offset_at (index);
    unsigned offset1 = offset_at (index + 1);
    എങ്കിൽ (unlikely (offset1 < offset0 || offset1 > offset_at (എണ്ണം)))
      തിരികെ hb_ubytes_t ();
    തിരികെ hb_ubytes_t (data_base () + offset0, offset1 - offset0);
  }

  size_t get_size () സ്ഥിരം
  {
    എങ്കിൽ (എണ്ണം)
      തിരികെ hb_unsigned_add_saturate (min_size, offSize.static_size,
				       offset_array_size (), offset_at (എണ്ണം) - 1);
    തിരികെ min_size;  /* empty CFFIndex contains count only */
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  hb_barrier () &&
			  (എണ്ണം == 0 || /* empty INDEX */
			   (എണ്ണം < എണ്ണം + 1u &&
			    c->check_struct (&offSize) && offSize >= 1 && offSize <= 4 &&
			    c->check_array (offsets, offSize, എണ്ണം + 1u) &&
			    c->check_range (data_base (), offset_at (എണ്ണം))))));
  }

  പൊതു:
  എണ്ണം_3		എണ്ണം;		/* Number of object data. Note there are (count+1) offsets */
  സ്വകാര്യ:
  HBUINT8	offSize;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  പൊതു:
  DEFINE_SIZE_MIN (എണ്ണം_3::static_size);
};
തരനിർവചനം CFFIndex<HBUINT16> CFF1Index;
തരനിർവചനം CFFIndex<HBUINT32> CFF2Index;


/* TupleValues */
ഘടന Tuple_മൂല്യങ്ങൾ
{
  എണ്ണൽ packed_മൂല്യം_flag_t
  {
    മൂല്യങ്ങൾ_ARE_ZEROS     = 0x80,
    മൂല്യങ്ങൾ_ARE_BYTES     = 0x00,
    മൂല്യങ്ങൾ_ARE_വാക്കുകൾ     = 0x40,
    മൂല്യങ്ങൾ_ARE_LONGS     = 0xC0,
    മൂല്യങ്ങൾ_SIZE_MASK     = 0xC0,
    മൂല്യം_RUN_എണ്ണം_MASK = 0x3F
  };

  സ്ഥിതം unsigned compile_unsafe (hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> values, /* IN */
				  unsigned അക്ഷരം *encoded_bytes /* OUT */)
  {
    unsigned അക്കം_മൂല്യങ്ങൾ_3 = values.length;
    unsigned encoded_len = 0;
    unsigned i = 0;
    ആകുമ്പോൾ (i < അക്കം_മൂല്യങ്ങൾ_3)
    {
      പൂർണ്ണസംഖ്യ val = values.arrayZ[i];
      എങ്കിൽ (val == 0)
        encoded_len += encode_മൂല്യം_run_as_zeroes (i, encoded_bytes + encoded_len, values);
      അല്ലെങ്കിൽ എങ്കിൽ ((int8_t) val == val)
        encoded_len += encode_മൂല്യം_run_as_bytes (i, encoded_bytes + encoded_len, values);
      അല്ലെങ്കിൽ എങ്കിൽ ((int16_t) val == val)
        encoded_len += encode_മൂല്യം_run_as_വാക്കുകൾ (i, encoded_bytes + encoded_len, values);
      അല്ലെങ്കിൽ
        encoded_len += encode_മൂല്യം_run_as_longs (i, encoded_bytes + encoded_len, values);
    }
    തിരികെ encoded_len;
  }

  സ്ഥിതം unsigned encode_മൂല്യം_run_as_zeroes (unsigned& i,
					      unsigned അക്ഷരം *it,
					      hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> values)
  {
    unsigned അക്കം_മൂല്യങ്ങൾ_3 = values.length;
    unsigned run_length = 0;
    unsigned encoded_len = 0;
    ആകുമ്പോൾ (i < അക്കം_മൂല്യങ്ങൾ_3 && values.arrayZ[i] == 0)
    {
      i++;
      run_length++;
    }

    ആകുമ്പോൾ (run_length >= 64)
    {
      *it++ = അക്ഷരം (മൂല്യങ്ങൾ_ARE_ZEROS | 63);
      run_length -= 64;
      encoded_len++;
    }

    എങ്കിൽ (run_length)
    {
      *it++ = അക്ഷരം (മൂല്യങ്ങൾ_ARE_ZEROS | (run_length - 1));
      encoded_len++;
    }
    തിരികെ encoded_len;
  }

  സ്ഥിതം unsigned encode_മൂല്യം_run_as_bytes (unsigned &i,
					     unsigned അക്ഷരം *it,
					     hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> values)
  {
    unsigned start = i;
    unsigned അക്കം_മൂല്യങ്ങൾ_3 = values.length;
    ആകുമ്പോൾ (i < അക്കം_മൂല്യങ്ങൾ_3)
    {
      പൂർണ്ണസംഖ്യ val = values.arrayZ[i];
      എങ്കിൽ ((int8_t) val != val)
        നിർത്തുക;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      എങ്കിൽ (val == 0 && i + 1 < അക്കം_മൂല്യങ്ങൾ_3 && values.arrayZ[i+1] == 0)
        നിർത്തുക;

      i++;
    }
    unsigned run_length = i - start;

    unsigned encoded_len = 0;

    ആകുമ്പോൾ (run_length >= 64)
    {
      *it++ = (മൂല്യങ്ങൾ_ARE_BYTES | 63);
      encoded_len++;

      വേണ്ടി (unsigned j = 0; j < 64; j++)
	it[j] = static_cast<അക്ഷരം> (values.arrayZ[start + j]);
      it += 64;
      encoded_len += 64;

      start += 64;
      run_length -= 64;
    }

    എങ്കിൽ (run_length)
    {
      *it++ = (മൂല്യങ്ങൾ_ARE_BYTES | (run_length - 1));
      encoded_len++;

      വേണ്ടി (unsigned j = 0; j < run_length; j++)
        it[j] = static_cast<അക്ഷരം> (values.arrayZ[start + j]);
      encoded_len += run_length;
    }

    തിരികെ encoded_len;
  }

  സ്ഥിതം unsigned encode_മൂല്യം_run_as_വാക്കുകൾ (unsigned &i,
					     unsigned അക്ഷരം *it,
					     hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> values)
  {
    unsigned start = i;
    unsigned അക്കം_മൂല്യങ്ങൾ_3 = values.length;
    ആകുമ്പോൾ (i < അക്കം_മൂല്യങ്ങൾ_3)
    {
      പൂർണ്ണസംഖ്യ val = values.arrayZ[i];

      എങ്കിൽ ((int16_t) val != val)
        നിർത്തുക;

      /* start a new run for a single zero value. */
      എങ്കിൽ (val == 0) നിർത്തുക;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      എങ്കിൽ ((int8_t) val == val &&
          i + 1 < അക്കം_മൂല്യങ്ങൾ_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        നിർത്തുക;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    ആകുമ്പോൾ (run_length >= 64)
    {
      *it++ = (മൂല്യങ്ങൾ_ARE_വാക്കുകൾ | 63);
      encoded_len++;

      വേണ്ടി (unsigned j = 0; j < 64; j++)
      {
        int16_t മൂല്യം_val = values.arrayZ[start + j];
        *it++ = static_cast<അക്ഷരം> (മൂല്യം_val >> 8);
        *it++ = static_cast<അക്ഷരം> (മൂല്യം_val & 0xFF);

        encoded_len += 2;
      }

      start += 64;
      run_length -= 64;
    }

    എങ്കിൽ (run_length)
    {
      *it++ = (മൂല്യങ്ങൾ_ARE_വാക്കുകൾ | (run_length - 1));
      encoded_len++;
      ആകുമ്പോൾ (start < i)
      {
        int16_t മൂല്യം_val = values.arrayZ[start++];
        *it++ = static_cast<അക്ഷരം> (മൂല്യം_val >> 8);
        *it++ = static_cast<അക്ഷരം> (മൂല്യം_val & 0xFF);

        encoded_len += 2;
      }
    }
    തിരികെ encoded_len;
  }

  സ്ഥിതം unsigned encode_മൂല്യം_run_as_longs (unsigned &i,
					     unsigned അക്ഷരം *it,
					     hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> values)
  {
    unsigned start = i;
    unsigned അക്കം_മൂല്യങ്ങൾ_3 = values.length;
    ആകുമ്പോൾ (i < അക്കം_മൂല്യങ്ങൾ_3)
    {
      പൂർണ്ണസംഖ്യ val = values.arrayZ[i];

      എങ്കിൽ ((int16_t) val == val)
        നിർത്തുക;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    ആകുമ്പോൾ (run_length >= 64)
    {
      *it++ = (മൂല്യങ്ങൾ_ARE_LONGS | 63);
      encoded_len++;

      വേണ്ടി (unsigned j = 0; j < 64; j++)
      {
        int32_t മൂല്യം_val = values.arrayZ[start + j];
        *it++ = static_cast<അക്ഷരം> (മൂല്യം_val >> 24);
        *it++ = static_cast<അക്ഷരം> (മൂല്യം_val >> 16);
        *it++ = static_cast<അക്ഷരം> (മൂല്യം_val >> 8);
        *it++ = static_cast<അക്ഷരം> (മൂല്യം_val & 0xFF);

        encoded_len += 4;
      }

      start += 64;
      run_length -= 64;
    }

    എങ്കിൽ (run_length)
    {
      *it++ = (മൂല്യങ്ങൾ_ARE_LONGS | (run_length - 1));
      encoded_len++;
      ആകുമ്പോൾ (start < i)
      {
        int32_t മൂല്യം_val = values.arrayZ[start++];
        *it++ = static_cast<അക്ഷരം> (മൂല്യം_val >> 24);
        *it++ = static_cast<അക്ഷരം> (മൂല്യം_val >> 16);
        *it++ = static_cast<അക്ഷരം> (മൂല്യം_val >> 8);
        *it++ = static_cast<അക്ഷരം> (മൂല്യം_val & 0xFF);

        encoded_len += 4;
      }
    }
    തിരികെ encoded_len;
  }

  template <typename T>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  സ്ഥിതം തർക്കം decompile (സ്ഥിരം HBUINT8 *&p /* IN/OUT */,
			 hb_vector_t<T> &values /* IN/OUT */,
			 സ്ഥിരം HBUINT8 *end,
			 തർക്കം consume_all = അസത്യം,
			 unsigned start = 0)
  {
    unsigned i = 0;
    unsigned എണ്ണം = consume_all ? UINT_MAX : values.length;
    എങ്കിൽ (consume_all)
      values.alloc ((end - p) / 2);
    ആകുമ്പോൾ (i < എണ്ണം)
    {
      എങ്കിൽ (unlikely (p + 1 > end)) തിരികെ consume_all;
      unsigned control = *p++;
      unsigned run_എണ്ണം = (control & മൂല്യം_RUN_എണ്ണം_MASK) + 1;
      എങ്കിൽ (consume_all)
      {
        എങ്കിൽ (unlikely (!values.resize_dirty  (values.length + run_എണ്ണം)))
	  തിരികെ അസത്യം;
      }
      unsigned stop = i + run_എണ്ണം;
      എങ്കിൽ (unlikely (stop > എണ്ണം)) തിരികെ അസത്യം;

      unsigned skip = i < start ? hb_min (start - i, run_എണ്ണം) : 0;
      i += skip;

      എങ്കിൽ ((control & മൂല്യങ്ങൾ_SIZE_MASK) == മൂല്യങ്ങൾ_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * വലുപ്പം (T));
	i = stop;
      }
      അല്ലെങ്കിൽ എങ്കിൽ ((control & മൂല്യങ്ങൾ_SIZE_MASK) ==  മൂല്യങ്ങൾ_ARE_വാക്കുകൾ)
      {
        എങ്കിൽ (unlikely (p + run_എണ്ണം * HBINT16::static_size > end)) തിരികെ അസത്യം;
	p += skip * HBINT16::static_size;
#ifndef HB_OPTIMIZE_SIZE
        വേണ്ടി (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (സ്ഥിരം HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 1] = * (സ്ഥിരം HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 2] = * (സ്ഥിരം HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 3] = * (സ്ഥിരം HBINT16 *) p;
	  p += HBINT16::static_size;
	}
#endif
        വേണ്ടി (; i < stop; i++)
        {
          values.arrayZ[i] = * (സ്ഥിരം HBINT16 *) p;
          p += HBINT16::static_size;
        }
      }
      അല്ലെങ്കിൽ എങ്കിൽ ((control & മൂല്യങ്ങൾ_SIZE_MASK) ==  മൂല്യങ്ങൾ_ARE_LONGS)
      {
        എങ്കിൽ (unlikely (p + run_എണ്ണം * HBINT32::static_size > end)) തിരികെ അസത്യം;
	p += skip * HBINT32::static_size;
        വേണ്ടി (; i < stop; i++)
        {
          values.arrayZ[i] = * (സ്ഥിരം HBINT32 *) p;
          p += HBINT32::static_size;
        }
      }
      അല്ലെങ്കിൽ എങ്കിൽ ((control & മൂല്യങ്ങൾ_SIZE_MASK) ==  മൂല്യങ്ങൾ_ARE_BYTES)
      {
        എങ്കിൽ (unlikely (p + run_എണ്ണം > end)) തിരികെ അസത്യം;
	p += skip * HBINT8::static_size;
#ifndef HB_OPTIMIZE_SIZE
	വേണ്ടി (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (സ്ഥിരം HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (സ്ഥിരം HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (സ്ഥിരം HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (സ്ഥിരം HBINT8 *) p++;
	}
#endif
        വേണ്ടി (; i < stop; i++)
          values.arrayZ[i] = * (സ്ഥിരം HBINT8 *) p++;
      }
    }
    തിരികെ സത്യം;
  }

  സ്ഥിതം തർക്കം skip (സ്ഥിരം unsigned അക്ഷരം *&p /* IN/OUT */,
		    സ്ഥിരം unsigned അക്ഷരം *end,
		    unsigned എണ്ണം)
  {
    unsigned seen = 0;
    ആകുമ്പോൾ (seen < എണ്ണം)
    {
      എങ്കിൽ (unlikely (p >= end)) തിരികെ അസത്യം;
      unsigned control = *p++;
      unsigned run_എണ്ണം = (control & മൂല്യം_RUN_എണ്ണം_MASK) + 1;
      എങ്കിൽ (unlikely (run_എണ്ണം > എണ്ണം - seen)) തിരികെ അസത്യം;

      unsigned width = 0;
      തിരഞ്ഞെടുപ്പ് (control & മൂല്യങ്ങൾ_SIZE_MASK)
      {
	അവസ്ഥ മൂല്യങ്ങൾ_ARE_ZEROS: നിർത്തുക;
	അവസ്ഥ മൂല്യങ്ങൾ_ARE_BYTES: width = HBINT8::static_size; നിർത്തുക;
	അവസ്ഥ മൂല്യങ്ങൾ_ARE_വാക്കുകൾ: width = HBINT16::static_size; നിർത്തുക;
	അവസ്ഥ മൂല്യങ്ങൾ_ARE_LONGS: width = HBINT32::static_size; നിർത്തുക;
      }
      എങ്കിൽ (unlikely (unsigned (end - p) < run_എണ്ണം * width)) തിരികെ അസത്യം;
      p += run_എണ്ണം * width;
      seen += run_എണ്ണം;
    }
    തിരികെ സത്യം;
  }

  ഘടന iter_t : hb_iter_with_fallback_t<iter_t, പൂർണ്ണസംഖ്യ>
  {
    iter_t (സ്ഥിരം unsigned അക്ഷരം *p_, unsigned len_)
	    : p (p_), endp (p_ + len_)
    { എങ്കിൽ (likely (ensure_run ())) read_മൂല്യം (); }

    സ്വകാര്യ:
    സ്ഥിരം unsigned അക്ഷരം *p;
    സ്ഥിരം unsigned അക്ഷരം * സ്ഥിരം endp;
    പൂർണ്ണസംഖ്യ current_മൂല്യം = 0;
    signed run_എണ്ണം = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    തർക്കം ensure_run ()
    {
      എങ്കിൽ (likely (run_എണ്ണം > 0)) തിരികെ സത്യം;
      തിരികെ _ensure_run ();
    }
    തർക്കം _ensure_run ()
    {
      എങ്കിൽ (unlikely (p >= endp))
      {
        run_എണ്ണം = 0;
        current_മൂല്യം = 0;
	തിരികെ അസത്യം;
      }

      unsigned control = *p++;
      run_എണ്ണം = (control & മൂല്യം_RUN_എണ്ണം_MASK) + 1;
      width = control & മൂല്യങ്ങൾ_SIZE_MASK;
      തിരഞ്ഞെടുപ്പ് (width)
      {
        അവസ്ഥ മൂല്യങ്ങൾ_ARE_ZEROS: width = 0; നിർത്തുക;
	അവസ്ഥ മൂല്യങ്ങൾ_ARE_BYTES: width = HBINT8::static_size;  നിർത്തുക;
	അവസ്ഥ മൂല്യങ്ങൾ_ARE_വാക്കുകൾ: width = HBINT16::static_size; നിർത്തുക;
	അവസ്ഥ മൂല്യങ്ങൾ_ARE_LONGS: width = HBINT32::static_size; നിർത്തുക;
	സ്ഥിരസ്ഥിതി: പ്രതിജ്ഞാ (അസത്യം);
      }

      എങ്കിൽ (unlikely (p + run_എണ്ണം * width > endp))
      {
	run_എണ്ണം = 0;
	current_മൂല്യം = 0;
	തിരികെ അസത്യം;
      }

      തിരികെ സത്യം;
    }
    ശൂന്യം read_മൂല്യം ()
    {
      തിരഞ്ഞെടുപ്പ് (width)
      {
        അവസ്ഥ 0: current_മൂല്യം = 0; നിർത്തുക;
	അവസ്ഥ 1: current_മൂല്യം = * (സ്ഥിരം HBINT8  *) p; നിർത്തുക;
	അവസ്ഥ 2: current_മൂല്യം = * (സ്ഥിരം HBINT16 *) p; നിർത്തുക;
	അവസ്ഥ 4: current_മൂല്യം = * (സ്ഥിരം HBINT32 *) p; നിർത്തുക;
      }
      p += width;
    }

    പൊതു:

    തരനിർവചനം പൂർണ്ണസംഖ്യ __item_t__;
    __item_t__ __item__ () സ്ഥിരം
    { തിരികെ current_മൂല്യം; }

    തർക്കം __more__ () സ്ഥിരം { തിരികെ run_എണ്ണം || p < endp; }
    ശൂന്യം __next__ ()
    {
      run_എണ്ണം--;
      എങ്കിൽ (unlikely (!ensure_run ()))
	തിരികെ;
      read_മൂല്യം ();
    }
    ശൂന്യം __forward__ (unsigned n)
    {
      എങ്കിൽ (unlikely (!ensure_run ()))
	തിരികെ;
      ആകുമ്പോൾ (n)
      {
	unsigned i = hb_min (n, (unsigned) run_എണ്ണം);
	run_എണ്ണം -= i;
	n -= i;
	p += (i - 1) * width;
	എങ്കിൽ (unlikely (!ensure_run ()))
	  തിരികെ;
	read_മൂല്യം ();
      }
    }
    തർക്കം operator != (സ്ഥിരം iter_t& o) സ്ഥിരം
    { തിരികെ p != o.p || run_എണ്ണം != o.run_എണ്ണം; }
    iter_t __end__ () സ്ഥിരം
    {
      iter_t it (endp, 0);
      തിരികെ it;
    }
  };

  ഘടന fetcher_t
  {
    fetcher_t (സ്ഥിരം unsigned അക്ഷരം *p_, unsigned len_)
	      : p (p_), end (p_ + len_) {}

    സ്വകാര്യ:
    സ്ഥിരം unsigned അക്ഷരം *p;
    സ്ഥിരം unsigned അക്ഷരം * സ്ഥിരം end;
    signed run_എണ്ണം = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    തർക്കം ensure_run ()
    {
      എങ്കിൽ (likely (run_എണ്ണം > 0)) തിരികെ സത്യം;
      തിരികെ _ensure_run ();
    }

    തർക്കം _ensure_run ()
    {
      എങ്കിൽ (unlikely (p >= end))
      {
        run_എണ്ണം = 0;
	തിരികെ അസത്യം;
      }

      unsigned control = *p++;
      run_എണ്ണം = (control & മൂല്യം_RUN_എണ്ണം_MASK) + 1;
      width = control & മൂല്യങ്ങൾ_SIZE_MASK;
      തിരഞ്ഞെടുപ്പ് (width)
      {
        അവസ്ഥ മൂല്യങ്ങൾ_ARE_ZEROS: width = 0; നിർത്തുക;
	അവസ്ഥ മൂല്യങ്ങൾ_ARE_BYTES: width = HBINT8::static_size;  നിർത്തുക;
	അവസ്ഥ മൂല്യങ്ങൾ_ARE_വാക്കുകൾ: width = HBINT16::static_size; നിർത്തുക;
	അവസ്ഥ മൂല്യങ്ങൾ_ARE_LONGS: width = HBINT32::static_size; നിർത്തുക;
	സ്ഥിരസ്ഥിതി: പ്രതിജ്ഞാ (അസത്യം);
      }

      എങ്കിൽ (unlikely (p + run_എണ്ണം * width > end))
      {
	run_എണ്ണം = 0;
	തിരികെ അസത്യം;
      }

      തിരികെ സത്യം;
    }

    പൊതു:
    ശൂന്യം skip (unsigned n)
    {
      ആകുമ്പോൾ (n)
      {
	എങ്കിൽ (unlikely (!ensure_run ()))
	  തിരികെ;
	unsigned i = hb_min (n, (unsigned) run_എണ്ണം);
	run_എണ്ണം -= i;
	n -= i;
	p += i * width;
      }
    }

    സ്വകാര്യ:
    template <തർക്കം scaled, typename T>
    ശൂന്യം _add_to (hb_array_t<T> out, T scale = T (1))
    {
      unsigned n = out.length;
      T *arrayZ = out.arrayZ;

      വേണ്ടി (unsigned i = 0; i < n;)
      {
	എങ്കിൽ (unlikely (!ensure_run ()))
	  നിർത്തുക;
	unsigned എണ്ണം = hb_min (n - i, (unsigned) run_എണ്ണം);
	തിരഞ്ഞെടുപ്പ് (width)
	{
	  അവസ്ഥ 0:
	  {
	    arrayZ += എണ്ണം;
	    നിർത്തുക;
	  }
	  അവസ്ഥ 1:
	  {
	    സ്ഥിരം auto *pp = (സ്ഥിരം HBINT8 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    വേണ്ടി (; j + 3 < എണ്ണം; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    വേണ്ടി (; j < എണ്ണം; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (സ്ഥിരം unsigned അക്ഷരം *) pp;
	  }
	  നിർത്തുക;
	  അവസ്ഥ 2:
	  {
	    സ്ഥിരം auto *pp = (സ്ഥിരം HBINT16 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    വേണ്ടി (; j + 3 < എണ്ണം; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    വേണ്ടി (; j < എണ്ണം; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (സ്ഥിരം unsigned അക്ഷരം *) pp;
	  }
	  നിർത്തുക;
	  അവസ്ഥ 4:
	  {
	    സ്ഥിരം auto *pp = (സ്ഥിരം HBINT32 *) p;
	    വേണ്ടി (unsigned j = 0; j < എണ്ണം; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (സ്ഥിരം unsigned അക്ഷരം *) pp;
	  }
	  നിർത്തുക;
	}
	run_എണ്ണം -= എണ്ണം;
	i += എണ്ണം;
      }
    }

    പൊതു:
    template <typename T>
    ശൂന്യം add_to (hb_array_t<T> out, ദശാംശം scale = 1.0f)
    {
#ifndef HB_OPTIMIZE_SIZE
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      എങ്കിൽ (അസത്യം && scale == 1.0f)
        _add_to<അസത്യം> (out);
      അല്ലെങ്കിൽ
#endif
        _add_to<സത്യം> (out, (T) scale);
    }
  };
};

ഘടന TupleList : CFF2Index
{
  Tuple_മൂല്യങ്ങൾ::iter_t operator [] (unsigned i) സ്ഥിരം
  {
    auto bytes = CFF2Index::operator [] (i);
    തിരികെ Tuple_മൂല്യങ്ങൾ::iter_t (bytes.arrayZ, bytes.length);
  }

  Tuple_മൂല്യങ്ങൾ::fetcher_t fetcher (unsigned i) സ്ഥിരം
  {
    auto bytes = CFF2Index::operator [] (i);
    തിരികെ Tuple_മൂല്യങ്ങൾ::fetcher_t (bytes.arrayZ, bytes.length);
  }
};


// Alignment

template <unsigned പൂർണ്ണസംഖ്യ alignment>
ഘടന Align
{
  size_t get_size (സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    unsigned offset = (സ്ഥിരം അക്ഷരം *) ഇത് - (സ്ഥിരം അക്ഷരം *) base;
    തിരികെ (alignment - offset) & (alignment - 1);
  }

  പൊതു:
  DEFINE_SIZE_MIN (0);
};



} /* namespace OT */


#endif /* HB_OPEN_TYPE_HH */
