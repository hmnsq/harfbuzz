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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-blob.hh"
#અન્તર્ભાવ "hb-face.hh"
#અન્તર્ભાવ "hb-machinery.hh"
#અન્તર્ભાવ "hb-meta.hh"
#અન્તર્ભાવ "hb-subset.hh"


નામાકાશ OT {


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
template <તાર્કિક BE,
	  typename Type,
	  unsigned પૂર્ણાંક Size = કદ (Type)>
સંરચના અંક_Type
{
  પ્રકારવ્યાખ્યા Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  પ્રકારવ્યાખ્યા typename std::conditional<std::is_integral<Type>::value && કદ (Type) <= કદ(પૂર્ણાંક),
				     typename std::conditional<std::is_signed<Type>::value, signed, unsigned>::type,
				     Type>::type WideType;

  અંક_Type () = મૂળભૂત;
  explicit constexpr અંક_Type (Type V) : v {V} {}
  અંક_Type& operator = (Type V) { v = V; પાછા *આ; }

  operator WideType () અચલ { પાછા v; }

  તાર્કિક operator == (અચલ અંક_Type &o) અચલ { પાછા (Type) v == (Type) o.v; }
  તાર્કિક operator != (અચલ અંક_Type &o) અચલ { પાછા !(*આ == o); }

  અંક_Type& operator += (WideType ગણતરી) { *આ = *આ + ગણતરી; પાછા *આ; }
  અંક_Type& operator -= (WideType ગણતરી) { *આ = *આ - ગણતરી; પાછા *આ; }
  અંક_Type& operator ++ () { *આ += 1; પાછા *આ; }
  અંક_Type& operator -- () { *આ -= 1; પાછા *આ; }
  અંક_Type operator ++ (પૂર્ણાંક) { અંક_Type c (*આ); ++*આ; પાછા c; }
  અંક_Type operator -- (પૂર્ણાંક) { અંક_Type c (*આ); --*આ; પાછા c; }

  uint32_t hash () અચલ { પાછા hb_array ((અચલ અક્ષર *) &v, કદ (v)).hash (); }
  HB_INTERNAL સ્થિર પૂર્ણાંક cmp (અચલ અંક_Type *a, અચલ અંક_Type *b)
  { પાછા b->cmp (*a); }
  HB_INTERNAL સ્થિર પૂર્ણાંક cmp (અચલ શૂન્ય *a, અચલ શૂન્ય *b)
  {
    અંક_Type *pa = (અંક_Type *) a;
    અંક_Type *pb = (અંક_Type *) b;

    પાછા pb->cmp (*pa);
  }
  template <typename Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  પૂર્ણાંક cmp (Type2 a) અચલ
  {
    Type b = v;
    પાછા (a > b) - (a < b);
  }
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }
  સુરક્ષિત:
  typename std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, Size>,
			    HBFloat<BE, Type, Size>>::type v;
  જાહેર:
  DEFINE_SIZE_STATIC (Size);
};

પ્રકારવ્યાખ્યા અંક_Type<સાચું, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
પ્રકારવ્યાખ્યા અંક_Type<સાચું, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
પ્રકારવ્યાખ્યા અંક_Type<સાચું, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
પ્રકારવ્યાખ્યા અંક_Type<સાચું, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
પ્રકારવ્યાખ્યા અંક_Type<સાચું, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
પ્રકારવ્યાખ્યા અંક_Type<સાચું, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
પ્રકારવ્યાખ્યા અંક_Type<સાચું, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
પ્રકારવ્યાખ્યા અંક_Type<સાચું, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
પ્રકારવ્યાખ્યા અંક_Type<સાચું, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

પ્રકારવ્યાખ્યા અંક_Type<ખોટું, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
પ્રકારવ્યાખ્યા અંક_Type<ખોટું, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
પ્રકારવ્યાખ્યા અંક_Type<ખોટું, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
પ્રકારવ્યાખ્યા અંક_Type<ખોટું, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
પ્રકારવ્યાખ્યા અંક_Type<ખોટું, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
પ્રકારવ્યાખ્યા અંક_Type<ખોટું, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

પ્રકારવ્યાખ્યા અંક_Type<સાચું,  દશાંશ>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
પ્રકારવ્યાખ્યા અંક_Type<સાચું,  દ્વિગુણ> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
પ્રકારવ્યાખ્યા અંક_Type<ખોટું, દશાંશ>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
પ્રકારવ્યાખ્યા અંક_Type<ખોટું, દ્વિગુણ> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
સંરચના HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& operator = (uint16_t i ) { HBUINT16::operator= (i); પાછા *આ; }
  જાહેર:
  DEFINE_SIZE_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
સંરચના HBUINT32VAR
{
  size_t get_size () અચલ
  {
    unsigned b0 = v[0];
    જો (b0 < 0x80)
      પાછા 1;
    નહીંતર જો (b0 < 0xC0)
      પાછા 2;
    નહીંતર જો (b0 < 0xE0)
      પાછા 3;
    નહીંતર જો (b0 < 0xF0)
      પાછા 4;
    નહીંતર
      પાછા 5;
  }

  સ્થિર size_t get_size (uint32_t v)
  {
    જો (v < 0x80)
      પાછા 1;
    નહીંતર જો (v < 0x4000)
      પાછા 2;
    નહીંતર જો (v < 0x200000)
      પાછા 3;
    નહીંતર જો (v < 0x10000000)
      પાછા 4;
    નહીંતર
      પાછા 5;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_size ()));
  }

  operator uint32_t () અચલ
  {
    unsigned b0 = v[0];
    જો (b0 < 0x80)
      પાછા b0;
    નહીંતર જો (b0 < 0xC0)
      પાછા ((b0 & 0x3F) << 8) | v[1];
    નહીંતર જો (b0 < 0xE0)
      પાછા ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    નહીંતર જો (b0 < 0xF0)
      પાછા ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    નહીંતર
      પાછા (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  સ્થિર unsigned serialize_unsafe (unsigned અક્ષર *buf, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned અક્ષર *p = buf + len;
    માટે (unsigned i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    જો (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    પાછા len;
  }

  સ્થિર તાર્કિક serialize (hb_serialize_context_t *c, uint32_t v)
  {
    unsigned len = get_size (v);
    unsigned અક્ષર *buf = c->allocate_size<unsigned અક્ષર> (len, ખોટું);
    જો (unlikely (!buf))
      પાછા ખોટું;

    serialize_unsafe (buf, v);
    પાછા સાચું;
  }

  સુરક્ષિત:
  unsigned અક્ષર v[5];

  જાહેર:
  DEFINE_SIZE_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
પ્રકારવ્યાખ્યા HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
પ્રકારવ્યાખ્યા HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
પ્રકારવ્યાખ્યા HBUINT16 UFWORD;

template <typename Type, unsigned fraction_bits>
સંરચના HBFixed : Type
{
  સ્થિર constexpr દશાંશ mult = 1.f / (1 << fraction_bits);
  static_assert (Type::static_size * 8 > fraction_bits, "");

  operator signed () અચલ = delete;
  operator unsigned () અચલ = delete;
  explicit operator દશાંશ () અચલ { પાછા to_float (); }
  typename Type::type to_int () અચલ { પાછા Type::v; }
  શૂન્ય set_int (typename Type::type i ) { Type::v = i; }
  દશાંશ to_float (દશાંશ offset = 0) અચલ  { પાછા ((int32_t) Type::v + offset) * mult; }
  શૂન્ય set_float (દશાંશ f) { Type::v = roundf (f / mult); }
  જાહેર:
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
સંરચના LONGDATETIME
{
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }
  સુરક્ષિત:
  HBINT32 major;
  HBUINT32 minor;
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
સંરચના Tag : HBUINT32
{
  Tag& operator = (hb_tag_t i) { HBUINT32::operator= (i); પાછા *આ; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  operator અચલ અક્ષર* () અચલ { પાછા reinterpret_cast<અચલ અક્ષર *> (આ); }
  operator અક્ષર* ()             { પાછા reinterpret_cast<અક્ષર *> (આ); }
  જાહેર:
  DEFINE_SIZE_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
સંરચના HBGlyphID16 : HBUINT16
{
  HBGlyphID16& operator = (uint16_t i) { HBUINT16::operator= (i); પાછા *આ; }
};
સંરચના HBGlyphID24 : HBUINT24
{
  HBGlyphID24& operator = (uint32_t i) { HBUINT24::operator= (i); પાછા *આ; }
};

/* Script/language-system/feature index */
સંરચના Index : HBUINT16 {
  સ્થિર constexpr unsigned NOT_FOUND_INDEX = 0xFFFFu;
  Index& operator = (uint16_t i) { HBUINT16::operator= (i); પાછા *આ; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, Index);

પ્રકારવ્યાખ્યા Index NameID;

સંરચના VarIdx : HBUINT32 {
  સ્થિર constexpr unsigned NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_INDEX, "");
  VarIdx& operator = (uint32_t i) { HBUINT32::operator= (i); પાછા *આ; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, VarIdx);

/* Offset, Null offset = 0 */
template <typename Type, તાર્કિક has_null=સાચું>
સંરચના Offset : Type
{
  Offset& operator = (typename Type::type i) { Type::operator= (i); પાછા *આ; }

  પ્રકારવ્યાખ્યા Type type;

  તાર્કિક is_null () અચલ { પાછા has_null && 0 == *આ; }

  જાહેર:
  DEFINE_SIZE_STATIC (કદ (Type));
};

પ્રકારવ્યાખ્યા Offset<HBUINT16> Offset16;
પ્રકારવ્યાખ્યા Offset<HBUINT24> Offset24;
પ્રકારવ્યાખ્યા Offset<HBUINT32> Offset32;


/* CheckSum */
સંરચના CheckSum : HBUINT32
{
  CheckSum& operator = (uint32_t i) { HBUINT32::operator= (i); પાછા *આ; }

  /* This is reference implementation from the spec. */
  સ્થિર uint32_t CalcTableChecksum (અચલ HBUINT32 *Table, uint32_t Length)
  {
    uint32_t Sum = 0L;
    પ્રતિજ્ઞા (0 == (Length & 3));
    અચલ HBUINT32 *EndPtr = Table + Length / HBUINT32::static_size;

    જ્યારે (Table < EndPtr)
      Sum += *Table++;
    પાછા Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  શૂન્ય set_for_data (અચલ શૂન્ય *data, unsigned પૂર્ણાંક length)
  { *આ = CalcTableChecksum ((અચલ HBUINT32 *) data, length); }

  જાહેર:
  DEFINE_SIZE_STATIC (4);
};


/*
 * Version Numbers
 */

template <typename FixedType=HBUINT16>
સંરચના FixedVersion
{
  uint32_t to_int () અચલ { પાછા (major << (કદ (FixedType) * 8)) + minor; }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  FixedType major;
  FixedType minor;
  જાહેર:
  DEFINE_SIZE_STATIC (2 * કદ (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

template <typename Type, તાર્કિક has_null>
સંરચના _hb_has_null
{
  સ્થિર અચલ Type *get_null () { પાછા nullptr; }
  સ્થિર Type *get_crap ()       { પાછા nullptr; }
};
template <typename Type>
સંરચના _hb_has_null<Type, સાચું>
{
  સ્થિર અચલ Type *get_null () { પાછા &Null (Type); }
  સ્થિર       Type *get_crap () { પાછા &Crap (Type); }
};

template <typename Type, typename OffsetType, typename BaseType=શૂન્ય, તાર્કિક has_null=સાચું>
સંરચના OffsetTo : Offset<OffsetType, has_null>
{
  using target_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == ખોટું ||
		 (hb_has_null_size (Type) || !hb_has_min_size (Type)), "");

  HB_DELETE_COPY_ASSIGN (OffsetTo);
  OffsetTo () = મૂળભૂત;

  OffsetTo& operator = (typename OffsetType::type i) { OffsetType::operator= (i); પાછા *આ; }

  અચલ Type& operator () (અચલ શૂન્ય *base) અચલ
  {
    જો (unlikely (આ->is_null ())) પાછા *_hb_has_null<Type, has_null>::get_null ();
    પાછા StructAtOffset<અચલ Type> (base, *આ);
  }
  Type& operator () (શૂન્ય *base) અચલ
  {
    જો (unlikely (આ->is_null ())) પાછા *_hb_has_null<Type, has_null>::get_crap ();
    પાછા StructAtOffset<Type> (base, *આ);
  }

  template <typename Base,
	    hb_enable_if (hb_is_convertible (અચલ Base, અચલ BaseType *))>
  friend અચલ Type& operator + (અચલ Base &base, અચલ OffsetTo &offset) { પાછા offset ((અચલ શૂન્ય *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (અચલ Base, અચલ BaseType *))>
  friend અચલ Type& operator + (અચલ OffsetTo &offset, અચલ Base &base) { પાછા offset ((અચલ શૂન્ય *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (Base &&base, OffsetTo &offset) { પાછા offset ((શૂન્ય *) base); }
  template <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  friend Type& operator + (OffsetTo &offset, Base &&base) { પાછા offset ((શૂન્ય *) base); }


  template <typename Base, typename ...Ts>
  તાર્કિક serialize_subset (hb_subset_context_t *c, અચલ OffsetTo& src,
			 અચલ Base *src_base, Ts&&... ds)
  {
    *આ = 0;
    જો (src.is_null ())
      પાછા ખોટું;

    auto *s = c->serializer;

    s->push ();

    તાર્કિક ret = c->dispatch (src_base+src, std::forward<Ts> (ds)...);

    જો (ret || !has_null)
      s->add_link (*આ, s->કાઢો_pack ());
    નહીંતર
      s->કાઢો_discard ();

    પાછા ret;
  }


  template <typename ...Ts>
  તાર્કિક serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *આ = 0;

    Type* obj = c->push<Type> ();
    તાર્કિક ret = obj->serialize (c, std::forward<Ts> (ds)...);

    જો (ret)
      c->add_link (*આ, c->કાઢો_pack ());
    નહીંતર
      c->કાઢો_discard ();

    પાછા ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  template <typename ...Ts>
  તાર્કિક serialize_copy (hb_serialize_context_t *c, અચલ OffsetTo& src,
		       અચલ શૂન્ય *src_base, unsigned dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *આ = 0;
    જો (src.is_null ())
      પાછા ખોટું;

    c->push ();

    તાર્કિક ret = c->copy (src_base+src, std::forward<Ts> (ds)...);

    c->add_link (*આ, c->કાઢો_pack (), whence, dst_bias);

    પાછા ret;
  }

  તાર્કિક serialize_copy (hb_serialize_context_t *c, અચલ OffsetTo& src,
		       અચલ શૂન્ય *src_base, unsigned dst_bias = 0)
  { પાછા serialize_copy (c, src, src_base, dst_bias, hb_serialize_context_t::Head); }

  તાર્કિક sanitize_shallow (hb_sanitize_context_t *c, અચલ BaseType *base) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!c->check_struct (આ))) return_trace (ખોટું);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    જો (unlikely ((અચલ અક્ષર *) base + (unsigned) *આ < (અચલ અક્ષર *) base)) return_trace (ખોટું);
    return_trace (સાચું);
  }

  template <typename ...Ts>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ BaseType *base, Ts&&... ds) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (sanitize_shallow (c, base) &&
		  hb_barrier () &&
		  (આ->is_null () ||
		   c->dispatch (StructAtOffset<Type> (base, *આ), std::forward<Ts> (ds)...)));
  }

  DEFINE_SIZE_STATIC (કદ (OffsetType));
};
/* Partial specializations. */
template <typename Type, typename BaseType=શૂન્ય, તાર્કિક has_null=સાચું> using Offset16To = OffsetTo<Type, HBUINT16, BaseType, has_null>;
template <typename Type, typename BaseType=શૂન્ય, તાર્કિક has_null=સાચું> using Offset24To = OffsetTo<Type, HBUINT24, BaseType, has_null>;
template <typename Type, typename BaseType=શૂન્ય, તાર્કિક has_null=સાચું> using Offset32To = OffsetTo<Type, HBUINT32, BaseType, has_null>;

template <typename Type, typename OffsetType, typename BaseType=શૂન્ય> using NNOffsetTo = OffsetTo<Type, OffsetType, BaseType, ખોટું>;
template <typename Type, typename BaseType=શૂન્ય> using NNOffset16To = Offset16To<Type, BaseType, ખોટું>;
template <typename Type, typename BaseType=શૂન્ય> using NNOffset24To = Offset24To<Type, BaseType, ખોટું>;
template <typename Type, typename BaseType=શૂન્ય> using NNOffset32To = Offset32To<Type, BaseType, ખોટું>;


/*
 * Array Types
 */

template <typename Type>
સંરચના UnsizedArrayOf
{
  પ્રકારવ્યાખ્યા Type item_t;
  સ્થિર constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (UnsizedArrayOf);

  અચલ Type& operator [] (unsigned પૂર્ણાંક i) અચલ
  {
    પાછા arrayZ[i];
  }
  Type& operator [] (unsigned પૂર્ણાંક i)
  {
    પાછા arrayZ[i];
  }

  સ્થિર size_t get_size (unsigned પૂર્ણાંક len)
  { પાછા hb_unsigned_mul_saturate (len, Type::static_size); }

  template <typename T> operator T * () { પાછા arrayZ; }
  template <typename T> operator અચલ T * () અચલ { પાછા arrayZ; }
  hb_array_t<Type> as_array (unsigned પૂર્ણાંક len)
  { પાછા hb_array (arrayZ, len); }
  hb_array_t<અચલ Type> as_array (unsigned પૂર્ણાંક len) અચલ
  { પાછા hb_array (arrayZ, len); }

  template <typename T>
  Type &lsearch (unsigned પૂર્ણાંક len, અચલ T &x, Type &not_found = Crap (Type))
  { પાછા *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  અચલ Type &lsearch (unsigned પૂર્ણાંક len, અચલ T &x, અચલ Type &not_found = Null (Type)) અચલ
  { પાછા *as_array (len).lsearch (x, &not_found); }
  template <typename T>
  તાર્કિક lfind (unsigned પૂર્ણાંક len, અચલ T &x, unsigned પૂર્ણાંક *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned પૂર્ણાંક to_store = (unsigned પૂર્ણાંક) -1) અચલ
  { પાછા as_array (len).lfind (x, i, not_found, to_store); }

  શૂન્ય qsort (unsigned પૂર્ણાંક len, unsigned પૂર્ણાંક start = 0, unsigned પૂર્ણાંક end = (unsigned પૂર્ણાંક) -1)
  { as_array (len).qsort (start, end); }

  તાર્કિક serialize (hb_serialize_context_t *c, unsigned પૂર્ણાંક વસ્તુઓ_len, તાર્કિક clear = સાચું)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_size (આ, get_size (વસ્તુઓ_len), clear))) return_trace (ખોટું);
    return_trace (સાચું);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  તાર્કિક serialize (hb_serialize_context_t *c, Iterator વસ્તુઓ)
  {
    TRACE_SERIALIZE (આ);
    unsigned ગણતરી = hb_len (વસ્તુઓ);
    જો (unlikely (!serialize (c, ગણતરી, ખોટું))) return_trace (ખોટું);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    માટે (unsigned i = 0; i < ગણતરી; i++, ++વસ્તુઓ)
      arrayZ[i] = *વસ્તુઓ;
    return_trace (સાચું);
  }

  UnsizedArrayOf* copy (hb_serialize_context_t *c, unsigned ગણતરી) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto *out = c->start_embed (આ);
    જો (unlikely (!as_array (ગણતરી).copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  તાર્કિક sanitize (hb_sanitize_context_t *c, unsigned પૂર્ણાંક ગણતરી, Ts&&... ds) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!sanitize_shallow (c, ગણતરી))) return_trace (ખોટું);
    જો (!કદ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (સાચું);
    hb_barrier ();
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ખોટું);
    return_trace (સાચું);
  }

  તાર્કિક sanitize_shallow (hb_sanitize_context_t *c, unsigned પૂર્ણાંક ગણતરી) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_array (arrayZ, ગણતરી));
  }

  જાહેર:
  Type		arrayZ[HB_VAR_ARRAY];
  જાહેર:
  DEFINE_SIZE_UNBOUNDED (0);
};

/* Unsized array of offset's */
template <typename Type, typename OffsetType, typename BaseType=શૂન્ય, તાર્કિક has_null=સાચું>
using UnsizedArray16OfOffsetTo = UnsizedArrayOf<OffsetTo<Type, OffsetType, BaseType, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
template <typename Type, typename OffsetType, typename BaseType=શૂન્ય, તાર્કિક has_null=સાચું>
સંરચના UnsizedListOfOffset16To : UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
{
  અચલ Type& operator [] (પૂર્ણાંક i_) અચલ
  {
    unsigned પૂર્ણાંક i = (unsigned પૂર્ણાંક) i_;
    અચલ OffsetTo<Type, OffsetType, BaseType, has_null> *p = &આ->arrayZ[i];
    જો (unlikely ((અચલ શૂન્ય *) p < (અચલ શૂન્ય *) આ->arrayZ)) પાછા Null (Type); /* Overflowed. */
    hb_barrier ();
    પાછા આ+*p;
  }
  Type& operator [] (પૂર્ણાંક i_)
  {
    unsigned પૂર્ણાંક i = (unsigned પૂર્ણાંક) i_;
    અચલ OffsetTo<Type, OffsetType, BaseType, has_null> *p = &આ->arrayZ[i];
    જો (unlikely ((અચલ શૂન્ય *) p < (અચલ શૂન્ય *) આ->arrayZ)) પાછા Crap (Type); /* Overflowed. */
    hb_barrier ();
    પાછા આ+*p;
  }

  template <typename ...Ts>
  તાર્કિક sanitize (hb_sanitize_context_t *c, unsigned પૂર્ણાંક ગણતરી, Ts&&... ds) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace ((UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
		   ::sanitize (c, ગણતરી, આ, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type>
સંરચના SortedUnsizedArrayOf : UnsizedArrayOf<Type>
{
  hb_sorted_array_t<Type> as_array (unsigned પૂર્ણાંક len)
  { પાછા hb_sorted_array (આ->arrayZ, len); }
  hb_sorted_array_t<અચલ Type> as_array (unsigned પૂર્ણાંક len) અચલ
  { પાછા hb_sorted_array (આ->arrayZ, len); }
  operator hb_sorted_array_t<Type> ()             { પાછા as_array (); }
  operator hb_sorted_array_t<અચલ Type> () અચલ { પાછા as_array (); }

  template <typename T>
  Type &bsearch (unsigned પૂર્ણાંક len, અચલ T &x, Type &not_found = Crap (Type))
  { પાછા *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  અચલ Type &bsearch (unsigned પૂર્ણાંક len, અચલ T &x, અચલ Type &not_found = Null (Type)) અચલ
  { પાછા *as_array (len).bsearch (x, &not_found); }
  template <typename T>
  તાર્કિક bfind (unsigned પૂર્ણાંક len, અચલ T &x, unsigned પૂર્ણાંક *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned પૂર્ણાંક to_store = (unsigned પૂર્ણાંક) -1) અચલ
  { પાછા as_array (len).bfind (x, i, not_found, to_store); }
};


/* An array with a number of elements. */
template <typename Type, typename LenType>
સંરચના ArrayOf
{
  પ્રકારવ્યાખ્યા Type item_t;
  સ્થિર constexpr unsigned item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOf);

  અચલ Type& operator [] (પૂર્ણાંક i_) અચલ
  {
    unsigned પૂર્ણાંક i = (unsigned પૂર્ણાંક) i_;
    જો (unlikely (i >= len)) પાછા Null (Type);
    hb_barrier ();
    પાછા arrayZ[i];
  }
  Type& operator [] (પૂર્ણાંક i_)
  {
    unsigned પૂર્ણાંક i = (unsigned પૂર્ણાંક) i_;
    જો (unlikely (i >= len)) પાછા Crap (Type);
    hb_barrier ();
    પાછા arrayZ[i];
  }

  size_t get_size () અચલ
  { પાછા hb_unsigned_mul_add_saturate (len, Type::static_size, len.static_size); }

  explicit operator તાર્કિક () અચલ { પાછા len; }

  શૂન્ય કાઢો () { len--; }

  hb_array_t<      Type> as_array ()       { પાછા hb_array (arrayZ, len); }
  hb_array_t<અચલ Type> as_array () અચલ { પાછા hb_array (arrayZ, len); }

  /* Iterator. */
  પ્રકારવ્યાખ્યા hb_array_t<અચલ Type>   iter_t;
  પ્રકારવ્યાખ્યા hb_array_t<      Type> writer_t;
    iter_t   iter () અચલ { પાછા as_array (); }
  writer_t writer ()       { પાછા as_array (); }
  operator   iter_t () અચલ { પાછા   iter (); }
  operator writer_t ()       { પાછા writer (); }

  /* Faster range-based for loop. */
  અચલ Type *begin () અચલ { પાછા arrayZ; }
  અચલ Type *end () અચલ { પાછા arrayZ + len; }

  template <typename T>
  Type &lsearch (અચલ T &x, Type &not_found = Crap (Type))
  { પાછા *as_array ().lsearch (x, &not_found); }
  template <typename T>
  અચલ Type &lsearch (અચલ T &x, અચલ Type &not_found = Null (Type)) અચલ
  { પાછા *as_array ().lsearch (x, &not_found); }
  template <typename T>
  તાર્કિક lfind (અચલ T &x, unsigned પૂર્ણાંક *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned પૂર્ણાંક to_store = (unsigned પૂર્ણાંક) -1) અચલ
  { પાછા as_array ().lfind (x, i, not_found, to_store); }

  શૂન્ય qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD તાર્કિક serialize (hb_serialize_context_t *c, unsigned વસ્તુઓ_len, તાર્કિક clear = સાચું)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    c->check_assign (len, વસ્તુઓ_len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    જો (unlikely (!c->extend_size (આ, get_size (), clear))) return_trace (ખોટું);
    return_trace (સાચું);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD તાર્કિક serialize (hb_serialize_context_t *c, Iterator વસ્તુઓ)
  {
    TRACE_SERIALIZE (આ);
    unsigned ગણતરી = hb_len (વસ્તુઓ);
    જો (unlikely (!serialize (c, ગણતરી, ખોટું))) return_trace (ખોટું);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    માટે (unsigned i = 0; i < ગણતરી; i++, ++વસ્તુઓ)
      arrayZ[i] = *વસ્તુઓ;
    return_trace (સાચું);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (આ);
    len++;
    જો (unlikely (!len || !c->extend (આ)))
    {
      len--;
      return_trace (nullptr);
    }
    return_trace (&arrayZ[len - 1]);
  }

  ArrayOf* copy (hb_serialize_context_t *c) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto *out = c->start_embed (આ);
    જો (unlikely (!c->extend_min (out))) return_trace (nullptr);
    c->check_assign (out->len, len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    જો (unlikely (!as_array ().copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  તાર્કિક sanitize (hb_sanitize_context_t *c, Ts&&... ds) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!sanitize_shallow (c))) return_trace (ખોટું);
    જો (!કદ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (સાચું);
    hb_barrier ();
    unsigned પૂર્ણાંક ગણતરી = len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ખોટું);
    return_trace (સાચું);
  }

  તાર્કિક sanitize_shallow (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (len.sanitize (c) &&
		  hb_barrier () &&
		  c->check_array_sized (arrayZ, len, કદ (LenType)));
  }

  જાહેર:
  LenType	len;
  Type		arrayZ[HB_VAR_ARRAY];
  જાહેર:
  DEFINE_SIZE_ARRAY (કદ (LenType), arrayZ);
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
સંરચના List16OfOffsetTo : ArrayOf<OffsetTo<Type, OffsetType>, HBUINT16>
{
  અચલ Type& operator [] (પૂર્ણાંક i_) અચલ
  {
    unsigned પૂર્ણાંક i = (unsigned પૂર્ણાંક) i_;
    જો (unlikely (i >= આ->len)) પાછા Null (Type);
    hb_barrier ();
    પાછા આ+આ->arrayZ[i];
  }
  અચલ Type& operator [] (પૂર્ણાંક i_)
  {
    unsigned પૂર્ણાંક i = (unsigned પૂર્ણાંક) i_;
    જો (unlikely (i >= આ->len)) પાછા Crap (Type);
    hb_barrier ();
    પાછા આ+આ->arrayZ[i];
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    સંરચના List16OfOffsetTo *out = c->serializer->embed (*આ);
    જો (unlikely (!out)) return_trace (ખોટું);
    unsigned પૂર્ણાંક ગણતરી = આ->len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      out->arrayZ[i].serialize_subset (c, આ->arrayZ[i], આ, out);
    return_trace (સાચું);
  }

  template <typename ...Ts>
  તાર્કિક sanitize (hb_sanitize_context_t *c, Ts&&... ds) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace ((Array16Of<OffsetTo<Type, OffsetType>>::sanitize (c, આ, std::forward<Ts> (ds)...)));
  }
};

template <typename Type>
using List16OfOffset16To = List16OfOffsetTo<Type, HBUINT16>;

/* An array starting at second element. */
template <typename Type, typename LenType>
સંરચના HeadlessArrayOf
{
  સ્થિર constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (HeadlessArrayOf);

  અચલ Type& operator [] (પૂર્ણાંક i_) અચલ
  {
    unsigned પૂર્ણાંક i = (unsigned પૂર્ણાંક) i_;
    જો (unlikely (i >= lenP1 || !i)) પાછા Null (Type);
    hb_barrier ();
    પાછા arrayZ[i-1];
  }
  Type& operator [] (પૂર્ણાંક i_)
  {
    unsigned પૂર્ણાંક i = (unsigned પૂર્ણાંક) i_;
    જો (unlikely (i >= lenP1 || !i)) પાછા Crap (Type);
    hb_barrier ();
    પાછા arrayZ[i-1];
  }
  size_t get_size () અચલ
  { પાછા hb_unsigned_mul_add_saturate (get_length (), Type::static_size, lenP1.static_size); }

  unsigned get_length () અચલ { પાછા lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { પાછા hb_array (arrayZ, get_length ()); }
  hb_array_t<અચલ Type> as_array () અચલ { પાછા hb_array (arrayZ, get_length ()); }

  /* Iterator. */
  પ્રકારવ્યાખ્યા hb_array_t<અચલ Type>   iter_t;
  પ્રકારવ્યાખ્યા hb_array_t<      Type> writer_t;
    iter_t   iter () અચલ { પાછા as_array (); }
  writer_t writer ()       { પાછા as_array (); }
  operator   iter_t () અચલ { પાછા   iter (); }
  operator writer_t ()       { પાછા writer (); }

  /* Faster range-based for loop. */
  અચલ Type *begin () અચલ { પાછા arrayZ; }
  અચલ Type *end () અચલ { પાછા arrayZ + get_length (); }

  HB_NODISCARD તાર્કિક serialize (hb_serialize_context_t *c, unsigned પૂર્ણાંક વસ્તુઓ_len, તાર્કિક clear = સાચું)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    c->check_assign (lenP1, વસ્તુઓ_len + 1, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    જો (unlikely (!c->extend_size (આ, get_size (), clear))) return_trace (ખોટું);
    return_trace (સાચું);
  }
  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD તાર્કિક serialize (hb_serialize_context_t *c, Iterator વસ્તુઓ)
  {
    TRACE_SERIALIZE (આ);
    unsigned ગણતરી = hb_len (વસ્તુઓ);
    જો (unlikely (!serialize (c, ગણતરી, ખોટું))) return_trace (ખોટું);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    માટે (unsigned i = 0; i < ગણતરી; i++, ++વસ્તુઓ)
      arrayZ[i] = *વસ્તુઓ;
    return_trace (સાચું);
  }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  તાર્કિક sanitize (hb_sanitize_context_t *c, Ts&&... ds) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!sanitize_shallow (c))) return_trace (ખોટું);
    જો (!કદ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (સાચું);
    hb_barrier ();
    unsigned પૂર્ણાંક ગણતરી = get_length ();
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ખોટું);
    return_trace (સાચું);
  }

  ખાનગી:
  તાર્કિક sanitize_shallow (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (lenP1.sanitize (c) &&
		  hb_barrier () &&
		  (!lenP1 || c->check_array_sized (arrayZ, lenP1 - 1, કદ (LenType))));
  }

  જાહેર:
  LenType	lenP1;
  Type		arrayZ[HB_VAR_ARRAY];
  જાહેર:
  DEFINE_SIZE_ARRAY (કદ (LenType), arrayZ);
};
template <typename Type> using HeadlessArray16Of = HeadlessArrayOf<Type, HBUINT16>;

/* An array storing length-1. */
template <typename Type, typename LenType=HBUINT16>
સંરચના ArrayOfM1
{
  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOfM1);

  અચલ Type& operator [] (પૂર્ણાંક i_) અચલ
  {
    unsigned પૂર્ણાંક i = (unsigned પૂર્ણાંક) i_;
    જો (unlikely (i > lenM1)) પાછા Null (Type);
    hb_barrier ();
    પાછા arrayZ[i];
  }
  Type& operator [] (પૂર્ણાંક i_)
  {
    unsigned પૂર્ણાંક i = (unsigned પૂર્ણાંક) i_;
    જો (unlikely (i > lenM1)) પાછા Crap (Type);
    hb_barrier ();
    પાછા arrayZ[i];
  }
  size_t get_size () અચલ
  { પાછા hb_unsigned_mul_add_saturate (lenM1 + 1, Type::static_size, lenM1.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  તાર્કિક sanitize (hb_sanitize_context_t *c, Ts&&... ds) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!sanitize_shallow (c))) return_trace (ખોટું);
    જો (!કદ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (સાચું);
    hb_barrier ();
    unsigned પૂર્ણાંક ગણતરી = lenM1 + 1;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (ખોટું);
    return_trace (સાચું);
  }

  ખાનગી:
  તાર્કિક sanitize_shallow (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (lenM1.sanitize (c) &&
		  hb_barrier () &&
		  (c->check_array_sized (arrayZ, lenM1 + 1, કદ (LenType))));
  }

  જાહેર:
  LenType	lenM1;
  Type		arrayZ[HB_VAR_ARRAY];
  જાહેર:
  DEFINE_SIZE_ARRAY (કદ (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
template <typename Type, typename LenType>
સંરચના SortedArrayOf : ArrayOf<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { પાછા hb_sorted_array (આ->arrayZ, આ->len); }
  hb_sorted_array_t<અચલ Type> as_array () અચલ { પાછા hb_sorted_array (આ->arrayZ, આ->len); }

  /* Iterator. */
  પ્રકારવ્યાખ્યા hb_sorted_array_t<અચલ Type>   iter_t;
  પ્રકારવ્યાખ્યા hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () અચલ { પાછા as_array (); }
  writer_t writer ()       { પાછા as_array (); }
  operator   iter_t () અચલ { પાછા   iter (); }
  operator writer_t ()       { પાછા writer (); }

  /* Faster range-based for loop. */
  અચલ Type *begin () અચલ { પાછા આ->arrayZ; }
  અચલ Type *end () અચલ { પાછા આ->arrayZ + આ->len; }

  તાર્કિક serialize (hb_serialize_context_t *c, unsigned પૂર્ણાંક વસ્તુઓ_len)
  {
    TRACE_SERIALIZE (આ);
    તાર્કિક ret = ArrayOf<Type, LenType>::serialize (c, વસ્તુઓ_len);
    return_trace (ret);
  }
  template <typename Iterator,
	    hb_requires (hb_is_sorted_source_of (Iterator, Type))>
  તાર્કિક serialize (hb_serialize_context_t *c, Iterator વસ્તુઓ)
  {
    TRACE_SERIALIZE (આ);
    તાર્કિક ret = ArrayOf<Type, LenType>::serialize (c, વસ્તુઓ);
    return_trace (ret);
  }

  SortedArrayOf* copy (hb_serialize_context_t *c) અચલ
  {
    TRACE_SERIALIZE (આ);
    SortedArrayOf* out = reinterpret_cast<SortedArrayOf *> (ArrayOf<Type, LenType>::copy (c));
    return_trace (out);
  }

  template <typename T>
  Type &bsearch (અચલ T &x, Type &not_found = Crap (Type))
  { પાછા *as_array ().bsearch (x, &not_found); }
  template <typename T>
  અચલ Type &bsearch (અચલ T &x, અચલ Type &not_found = Null (Type)) અચલ
  { પાછા *as_array ().bsearch (x, &not_found); }
  template <typename T>
  તાર્કિક bfind (અચલ T &x, unsigned પૂર્ણાંક *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned પૂર્ણાંક to_store = (unsigned પૂર્ણાંક) -1) અચલ
  { પાછા as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type> using SortedArray16Of = SortedArrayOf<Type, HBUINT16>;
template <typename Type> using SortedArray24Of = SortedArrayOf<Type, HBUINT24>;
template <typename Type> using SortedArray32Of = SortedArrayOf<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

template <typename LenType=HBUINT16>
સંરચના Bin_શોધ_Header
{
  operator uint32_t () અચલ { પાછા len; }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  Bin_શોધ_Header& operator = (unsigned પૂર્ણાંક v)
  {
    len = v;
    પ્રતિજ્ઞા (len == v);
    entrySelector = hb_max (1u, hb_bit_storage (v)) - 1;
    શોધ_Range = 16 * (1u << entrySelector);
    rangeShift = v * 16 > શોધ_Range
		 ? 16 * v - શોધ_Range
		 : 0;
    પાછા *આ;
  }

  સુરક્ષિત:
  LenType	len;
  LenType	શોધ_Range;
  LenType	entrySelector;
  LenType	rangeShift;

  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

template <typename Type, typename LenType=HBUINT16>
using Bin_શોધ_Array_Of = SortedArrayOf<Type, Bin_શોધ_Header<LenType>>;


સંરચના Var_Sized_Bin_શોધ_Header
{

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  HBUINT16	unitSize;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	શોધ_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	entrySelector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	rangeShift;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  જાહેર:
  DEFINE_SIZE_STATIC (10);
};

template <typename Type>
સંરચના Var_Sized_Bin_શોધ_Array_Of
{
  સ્થિર constexpr unsigned item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (Var_Sized_Bin_શોધ_Array_Of);

  તાર્કિક last_is_terminator () અચલ
  {
    જો (unlikely (!header.nUnits)) પાછા ખોટું;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    અચલ HBUINT16 *શબ્દો = &StructAtOffset<HBUINT16> (&bytesZ, (header.nUnits - 1) * header.unitSize);
    unsigned પૂર્ણાંક ગણતરી = Type::Termination_Word_ગણતરી;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (શબ્દો[i] != 0xFFFFu)
	પાછા ખોટું;
    પાછા સાચું;
  }

  અચલ Type& operator [] (પૂર્ણાંક i_) અચલ
  {
    unsigned પૂર્ણાંક i = (unsigned પૂર્ણાંક) i_;
    જો (unlikely (i >= get_length ())) પાછા Null (Type);
    hb_barrier ();
    પાછા StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  Type& operator [] (પૂર્ણાંક i_)
  {
    unsigned પૂર્ણાંક i = (unsigned પૂર્ણાંક) i_;
    જો (unlikely (i >= get_length ())) પાછા Crap (Type);
    hb_barrier ();
    પાછા StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  unsigned પૂર્ણાંક get_length () અચલ
  { પાછા header.nUnits - last_is_terminator (); }
  size_t get_size () અચલ
  { પાછા hb_unsigned_mul_add_saturate (header.nUnits, header.unitSize, header.static_size); }

  template <typename ...Ts>
  HB_ALWAYS_INLINE
  તાર્કિક sanitize (hb_sanitize_context_t *c, Ts&&... ds) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!sanitize_shallow (c))) return_trace (ખોટું);
    જો (!કદ... (Ts) && hb_is_trivially_copyable(Type)) return_trace (સાચું);
    hb_barrier ();
    unsigned પૂર્ણાંક ગણતરી = get_length ();
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (unlikely (!(*આ)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (ખોટું);
    return_trace (સાચું);
  }

  template <typename T>
  અચલ Type *bsearch (અચલ T &key) અચલ
  {
    unsigned pos;
    પાછા hb_bsearch_impl (&pos,
			    key,
			    (અચલ શૂન્ય *) bytesZ,
			    get_length (),
			    header.unitSize,
			    _hb_cmp_method<T, Type>)
	   ? (અચલ Type *) (((અચલ અક્ષર *) &bytesZ) + (pos * header.unitSize))
	   : nullptr;
  }

  ખાનગી:
  તાર્કિક sanitize_shallow (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (header.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_size <= header.unitSize &&
		  c->check_range (bytesZ.arrayZ,
				  header.nUnits,
				  header.unitSize));
  }

  સુરક્ષિત:
  Var_Sized_Bin_શોધ_Header	header;
  UnsizedArrayOf<HBUINT8>	bytesZ;
  જાહેર:
  DEFINE_SIZE_ARRAY (10, bytesZ);
};


/* CFF INDEX */

template <typename ગણતરી_3>
સંરચના CFFIndex
{
  unsigned પૂર્ણાંક offset_array_size () અચલ
  { પાછા offSize * (ગણતરી + 1); }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ Iterable &iterable,
		  અચલ unsigned *p_data_size = nullptr,
                  unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (આ);
    unsigned data_size;
    જો (p_data_size)
      data_size = *p_data_size;
    નહીંતર
      total_size (iterable, &data_size);

    auto it = hb_iter (iterable);
    જો (unlikely (!serialize_header (c, +it, data_size, min_off_size))) return_trace (ખોટું);
    unsigned અક્ષર *ret = c->allocate_size<unsigned અક્ષર> (data_size, ખોટું);
    જો (unlikely (!ret)) return_trace (ખોટું);
    unsigned remaining = data_size;
    માટે (અચલ auto &_ : +it)
    {
      unsigned len = _.length;

      જો (!len)
	ચાલુ;

      જો (unlikely (len > remaining)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (ખોટું, HB_SERIALIZE_ERROR_INT_OVERFLOW));
      }

      remaining -= len;

      જો (len <= 1)
      {
	*ret++ = *_.arrayZ;
	ચાલુ;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (સાચું);
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  તાર્કિક serialize_header (hb_serialize_context_t *c,
			 Iterator it,
			 unsigned data_size,
                         unsigned min_off_size = 0)
  {
    TRACE_SERIALIZE (આ);

    unsigned off_size = (hb_bit_storage (data_size + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    /* serialize CFFIndex header */
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    આ->ગણતરી = hb_len (it);
    જો (!આ->ગણતરી) return_trace (સાચું);
    જો (unlikely (!c->extend (આ->offSize))) return_trace (ખોટું);
    આ->offSize = off_size;
    જો (unlikely (!c->allocate_size<HBUINT8> (off_size * (આ->ગણતરી + 1), ખોટું)))
      return_trace (ખોટું);

    /* serialize indices */
    unsigned પૂર્ણાંક offset = 1;
    જો (HB_OPTIMIZE_SIZE_VAL)
    {
      unsigned પૂર્ણાંક i = 0;
      માટે (અચલ auto &_ : +it)
      {
	set_offset_at (i++, offset);
	offset += hb_len_of (_);
      }
      set_offset_at (i, offset);
    }
    નહીંતર
      પસંદગી (off_size)
      {
	સ્થિતિ 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  માટે (અચલ auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	તોડો;
	સ્થિતિ 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  માટે (અચલ auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	તોડો;
	સ્થિતિ 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  માટે (અચલ auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	તોડો;
	સ્થિતિ 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  માટે (અચલ auto &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	તોડો;
	મૂળભૂત:
	તોડો;
      }

    પ્રતિજ્ઞા (offset == data_size + 1);
    return_trace (સાચું);
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  સ્થિર unsigned total_size (અચલ Iterable &iterable, unsigned *data_size = nullptr, unsigned min_off_size = 0)
  {
    auto it = + hb_iter (iterable);
    જો (!it)
    {
      જો (data_size) *data_size = 0;
      પાછા min_size;
    }

    unsigned total = 0;
    માટે (અચલ auto &_ : +it)
      total += hb_len_of (_);

    જો (data_size) *data_size = total;

    unsigned off_size = (hb_bit_storage (total + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    પાછા min_size + HBUINT8::static_size + (hb_len (it) + 1) * off_size + total;
  }

  શૂન્ય set_offset_at (unsigned પૂર્ણાંક index, unsigned પૂર્ણાંક offset)
  {
    પ્રતિજ્ઞા (index <= ગણતરી);

    unsigned પૂર્ણાંક size = offSize;
    અચલ HBUINT8 *p = offsets;
    પસંદગી (size)
    {
      સ્થિતિ 1: ((HBUINT8  *) p)[index] = offset; તોડો;
      સ્થિતિ 2: ((HBUINT16 *) p)[index] = offset; તોડો;
      સ્થિતિ 3: ((HBUINT24 *) p)[index] = offset; તોડો;
      સ્થિતિ 4: ((HBUINT32 *) p)[index] = offset; તોડો;
      મૂળભૂત: પાછા;
    }
  }

  ખાનગી:
  unsigned પૂર્ણાંક offset_at (unsigned પૂર્ણાંક index) અચલ
  {
    પ્રતિજ્ઞા (index <= ગણતરી);

    unsigned પૂર્ણાંક size = offSize;
    અચલ HBUINT8 *p = offsets;
    પસંદગી (size)
    {
      સ્થિતિ 1: પાછા ((HBUINT8  *) p)[index];
      સ્થિતિ 2: પાછા ((HBUINT16 *) p)[index];
      સ્થિતિ 3: પાછા ((HBUINT24 *) p)[index];
      સ્થિતિ 4: પાછા ((HBUINT32 *) p)[index];
      મૂળભૂત: પાછા 0;
    }
  }

  અચલ unsigned અક્ષર *data_base () અચલ
  { પાછા (અચલ unsigned અક્ષર *) આ + min_size + offSize.static_size - 1 + offset_array_size (); }
  જાહેર:

  hb_ubytes_t operator [] (unsigned પૂર્ણાંક index) અચલ
  {
    જો (unlikely (index >= ગણતરી)) પાછા hb_ubytes_t ();
    hb_barrier ();
    unsigned offset0 = offset_at (index);
    unsigned offset1 = offset_at (index + 1);
    જો (unlikely (offset1 < offset0 || offset1 > offset_at (ગણતરી)))
      પાછા hb_ubytes_t ();
    પાછા hb_ubytes_t (data_base () + offset0, offset1 - offset0);
  }

  size_t get_size () અચલ
  {
    જો (ગણતરી)
      પાછા hb_unsigned_add_saturate (min_size, offSize.static_size,
				       offset_array_size (), offset_at (ગણતરી) - 1);
    પાછા min_size;  /* empty CFFIndex contains count only */
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  hb_barrier () &&
			  (ગણતરી == 0 || /* empty INDEX */
			   (ગણતરી < ગણતરી + 1u &&
			    c->check_struct (&offSize) && offSize >= 1 && offSize <= 4 &&
			    c->check_array (offsets, offSize, ગણતરી + 1u) &&
			    c->check_range (data_base (), offset_at (ગણતરી))))));
  }

  જાહેર:
  ગણતરી_3		ગણતરી;		/* Number of object data. Note there are (count+1) offsets */
  ખાનગી:
  HBUINT8	offSize;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  જાહેર:
  DEFINE_SIZE_MIN (ગણતરી_3::static_size);
};
પ્રકારવ્યાખ્યા CFFIndex<HBUINT16> CFF1Index;
પ્રકારવ્યાખ્યા CFFIndex<HBUINT32> CFF2Index;


/* TupleValues */
સંરચના Tuple_કિંમતો
{
  ગણના packed_કિંમત_flag_t
  {
    કિંમતો_ARE_ZEROS     = 0x80,
    કિંમતો_ARE_BYTES     = 0x00,
    કિંમતો_ARE_શબ્દો     = 0x40,
    કિંમતો_ARE_LONGS     = 0xC0,
    કિંમતો_SIZE_MASK     = 0xC0,
    કિંમત_RUN_ગણતરી_MASK = 0x3F
  };

  સ્થિર unsigned compile_unsafe (hb_array_t<અચલ પૂર્ણાંક> values, /* IN */
				  unsigned અક્ષર *encoded_bytes /* OUT */)
  {
    unsigned અંક_કિંમતો_3 = values.length;
    unsigned encoded_len = 0;
    unsigned i = 0;
    જ્યારે (i < અંક_કિંમતો_3)
    {
      પૂર્ણાંક val = values.arrayZ[i];
      જો (val == 0)
        encoded_len += encode_કિંમત_run_as_zeroes (i, encoded_bytes + encoded_len, values);
      નહીંતર જો ((int8_t) val == val)
        encoded_len += encode_કિંમત_run_as_bytes (i, encoded_bytes + encoded_len, values);
      નહીંતર જો ((int16_t) val == val)
        encoded_len += encode_કિંમત_run_as_શબ્દો (i, encoded_bytes + encoded_len, values);
      નહીંતર
        encoded_len += encode_કિંમત_run_as_longs (i, encoded_bytes + encoded_len, values);
    }
    પાછા encoded_len;
  }

  સ્થિર unsigned encode_કિંમત_run_as_zeroes (unsigned& i,
					      unsigned અક્ષર *it,
					      hb_array_t<અચલ પૂર્ણાંક> values)
  {
    unsigned અંક_કિંમતો_3 = values.length;
    unsigned run_length = 0;
    unsigned encoded_len = 0;
    જ્યારે (i < અંક_કિંમતો_3 && values.arrayZ[i] == 0)
    {
      i++;
      run_length++;
    }

    જ્યારે (run_length >= 64)
    {
      *it++ = અક્ષર (કિંમતો_ARE_ZEROS | 63);
      run_length -= 64;
      encoded_len++;
    }

    જો (run_length)
    {
      *it++ = અક્ષર (કિંમતો_ARE_ZEROS | (run_length - 1));
      encoded_len++;
    }
    પાછા encoded_len;
  }

  સ્થિર unsigned encode_કિંમત_run_as_bytes (unsigned &i,
					     unsigned અક્ષર *it,
					     hb_array_t<અચલ પૂર્ણાંક> values)
  {
    unsigned start = i;
    unsigned અંક_કિંમતો_3 = values.length;
    જ્યારે (i < અંક_કિંમતો_3)
    {
      પૂર્ણાંક val = values.arrayZ[i];
      જો ((int8_t) val != val)
        તોડો;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      જો (val == 0 && i + 1 < અંક_કિંમતો_3 && values.arrayZ[i+1] == 0)
        તોડો;

      i++;
    }
    unsigned run_length = i - start;

    unsigned encoded_len = 0;

    જ્યારે (run_length >= 64)
    {
      *it++ = (કિંમતો_ARE_BYTES | 63);
      encoded_len++;

      માટે (unsigned j = 0; j < 64; j++)
	it[j] = static_cast<અક્ષર> (values.arrayZ[start + j]);
      it += 64;
      encoded_len += 64;

      start += 64;
      run_length -= 64;
    }

    જો (run_length)
    {
      *it++ = (કિંમતો_ARE_BYTES | (run_length - 1));
      encoded_len++;

      માટે (unsigned j = 0; j < run_length; j++)
        it[j] = static_cast<અક્ષર> (values.arrayZ[start + j]);
      encoded_len += run_length;
    }

    પાછા encoded_len;
  }

  સ્થિર unsigned encode_કિંમત_run_as_શબ્દો (unsigned &i,
					     unsigned અક્ષર *it,
					     hb_array_t<અચલ પૂર્ણાંક> values)
  {
    unsigned start = i;
    unsigned અંક_કિંમતો_3 = values.length;
    જ્યારે (i < અંક_કિંમતો_3)
    {
      પૂર્ણાંક val = values.arrayZ[i];

      જો ((int16_t) val != val)
        તોડો;

      /* start a new run for a single zero value. */
      જો (val == 0) તોડો;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      જો ((int8_t) val == val &&
          i + 1 < અંક_કિંમતો_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        તોડો;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    જ્યારે (run_length >= 64)
    {
      *it++ = (કિંમતો_ARE_શબ્દો | 63);
      encoded_len++;

      માટે (unsigned j = 0; j < 64; j++)
      {
        int16_t કિંમત_val = values.arrayZ[start + j];
        *it++ = static_cast<અક્ષર> (કિંમત_val >> 8);
        *it++ = static_cast<અક્ષર> (કિંમત_val & 0xFF);

        encoded_len += 2;
      }

      start += 64;
      run_length -= 64;
    }

    જો (run_length)
    {
      *it++ = (કિંમતો_ARE_શબ્દો | (run_length - 1));
      encoded_len++;
      જ્યારે (start < i)
      {
        int16_t કિંમત_val = values.arrayZ[start++];
        *it++ = static_cast<અક્ષર> (કિંમત_val >> 8);
        *it++ = static_cast<અક્ષર> (કિંમત_val & 0xFF);

        encoded_len += 2;
      }
    }
    પાછા encoded_len;
  }

  સ્થિર unsigned encode_કિંમત_run_as_longs (unsigned &i,
					     unsigned અક્ષર *it,
					     hb_array_t<અચલ પૂર્ણાંક> values)
  {
    unsigned start = i;
    unsigned અંક_કિંમતો_3 = values.length;
    જ્યારે (i < અંક_કિંમતો_3)
    {
      પૂર્ણાંક val = values.arrayZ[i];

      જો ((int16_t) val == val)
        તોડો;

      i++;
    }

    unsigned run_length = i - start;
    unsigned encoded_len = 0;
    જ્યારે (run_length >= 64)
    {
      *it++ = (કિંમતો_ARE_LONGS | 63);
      encoded_len++;

      માટે (unsigned j = 0; j < 64; j++)
      {
        int32_t કિંમત_val = values.arrayZ[start + j];
        *it++ = static_cast<અક્ષર> (કિંમત_val >> 24);
        *it++ = static_cast<અક્ષર> (કિંમત_val >> 16);
        *it++ = static_cast<અક્ષર> (કિંમત_val >> 8);
        *it++ = static_cast<અક્ષર> (કિંમત_val & 0xFF);

        encoded_len += 4;
      }

      start += 64;
      run_length -= 64;
    }

    જો (run_length)
    {
      *it++ = (કિંમતો_ARE_LONGS | (run_length - 1));
      encoded_len++;
      જ્યારે (start < i)
      {
        int32_t કિંમત_val = values.arrayZ[start++];
        *it++ = static_cast<અક્ષર> (કિંમત_val >> 24);
        *it++ = static_cast<અક્ષર> (કિંમત_val >> 16);
        *it++ = static_cast<અક્ષર> (કિંમત_val >> 8);
        *it++ = static_cast<અક્ષર> (કિંમત_val & 0xFF);

        encoded_len += 4;
      }
    }
    પાછા encoded_len;
  }

  template <typename T>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  સ્થિર તાર્કિક decompile (અચલ HBUINT8 *&p /* IN/OUT */,
			 hb_vector_t<T> &values /* IN/OUT */,
			 અચલ HBUINT8 *end,
			 તાર્કિક consume_all = ખોટું,
			 unsigned start = 0)
  {
    unsigned i = 0;
    unsigned ગણતરી = consume_all ? UINT_MAX : values.length;
    જો (consume_all)
      values.alloc ((end - p) / 2);
    જ્યારે (i < ગણતરી)
    {
      જો (unlikely (p + 1 > end)) પાછા consume_all;
      unsigned control = *p++;
      unsigned run_ગણતરી = (control & કિંમત_RUN_ગણતરી_MASK) + 1;
      જો (consume_all)
      {
        જો (unlikely (!values.resize_dirty  (values.length + run_ગણતરી)))
	  પાછા ખોટું;
      }
      unsigned stop = i + run_ગણતરી;
      જો (unlikely (stop > ગણતરી)) પાછા ખોટું;

      unsigned skip = i < start ? hb_min (start - i, run_ગણતરી) : 0;
      i += skip;

      જો ((control & કિંમતો_SIZE_MASK) == કિંમતો_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * કદ (T));
	i = stop;
      }
      નહીંતર જો ((control & કિંમતો_SIZE_MASK) ==  કિંમતો_ARE_શબ્દો)
      {
        જો (unlikely (p + run_ગણતરી * HBINT16::static_size > end)) પાછા ખોટું;
	p += skip * HBINT16::static_size;
#ifndef HB_OPTIMIZE_SIZE
        માટે (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (અચલ HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 1] = * (અચલ HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 2] = * (અચલ HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 3] = * (અચલ HBINT16 *) p;
	  p += HBINT16::static_size;
	}
#endif
        માટે (; i < stop; i++)
        {
          values.arrayZ[i] = * (અચલ HBINT16 *) p;
          p += HBINT16::static_size;
        }
      }
      નહીંતર જો ((control & કિંમતો_SIZE_MASK) ==  કિંમતો_ARE_LONGS)
      {
        જો (unlikely (p + run_ગણતરી * HBINT32::static_size > end)) પાછા ખોટું;
	p += skip * HBINT32::static_size;
        માટે (; i < stop; i++)
        {
          values.arrayZ[i] = * (અચલ HBINT32 *) p;
          p += HBINT32::static_size;
        }
      }
      નહીંતર જો ((control & કિંમતો_SIZE_MASK) ==  કિંમતો_ARE_BYTES)
      {
        જો (unlikely (p + run_ગણતરી > end)) પાછા ખોટું;
	p += skip * HBINT8::static_size;
#ifndef HB_OPTIMIZE_SIZE
	માટે (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (અચલ HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (અચલ HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (અચલ HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (અચલ HBINT8 *) p++;
	}
#endif
        માટે (; i < stop; i++)
          values.arrayZ[i] = * (અચલ HBINT8 *) p++;
      }
    }
    પાછા સાચું;
  }

  સ્થિર તાર્કિક skip (અચલ unsigned અક્ષર *&p /* IN/OUT */,
		    અચલ unsigned અક્ષર *end,
		    unsigned ગણતરી)
  {
    unsigned seen = 0;
    જ્યારે (seen < ગણતરી)
    {
      જો (unlikely (p >= end)) પાછા ખોટું;
      unsigned control = *p++;
      unsigned run_ગણતરી = (control & કિંમત_RUN_ગણતરી_MASK) + 1;
      જો (unlikely (run_ગણતરી > ગણતરી - seen)) પાછા ખોટું;

      unsigned width = 0;
      પસંદગી (control & કિંમતો_SIZE_MASK)
      {
	સ્થિતિ કિંમતો_ARE_ZEROS: તોડો;
	સ્થિતિ કિંમતો_ARE_BYTES: width = HBINT8::static_size; તોડો;
	સ્થિતિ કિંમતો_ARE_શબ્દો: width = HBINT16::static_size; તોડો;
	સ્થિતિ કિંમતો_ARE_LONGS: width = HBINT32::static_size; તોડો;
      }
      જો (unlikely (unsigned (end - p) < run_ગણતરી * width)) પાછા ખોટું;
      p += run_ગણતરી * width;
      seen += run_ગણતરી;
    }
    પાછા સાચું;
  }

  સંરચના iter_t : hb_iter_with_fallback_t<iter_t, પૂર્ણાંક>
  {
    iter_t (અચલ unsigned અક્ષર *p_, unsigned len_)
	    : p (p_), endp (p_ + len_)
    { જો (likely (ensure_run ())) read_કિંમત (); }

    ખાનગી:
    અચલ unsigned અક્ષર *p;
    અચલ unsigned અક્ષર * અચલ endp;
    પૂર્ણાંક current_કિંમત = 0;
    signed run_ગણતરી = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    તાર્કિક ensure_run ()
    {
      જો (likely (run_ગણતરી > 0)) પાછા સાચું;
      પાછા _ensure_run ();
    }
    તાર્કિક _ensure_run ()
    {
      જો (unlikely (p >= endp))
      {
        run_ગણતરી = 0;
        current_કિંમત = 0;
	પાછા ખોટું;
      }

      unsigned control = *p++;
      run_ગણતરી = (control & કિંમત_RUN_ગણતરી_MASK) + 1;
      width = control & કિંમતો_SIZE_MASK;
      પસંદગી (width)
      {
        સ્થિતિ કિંમતો_ARE_ZEROS: width = 0; તોડો;
	સ્થિતિ કિંમતો_ARE_BYTES: width = HBINT8::static_size;  તોડો;
	સ્થિતિ કિંમતો_ARE_શબ્દો: width = HBINT16::static_size; તોડો;
	સ્થિતિ કિંમતો_ARE_LONGS: width = HBINT32::static_size; તોડો;
	મૂળભૂત: પ્રતિજ્ઞા (ખોટું);
      }

      જો (unlikely (p + run_ગણતરી * width > endp))
      {
	run_ગણતરી = 0;
	current_કિંમત = 0;
	પાછા ખોટું;
      }

      પાછા સાચું;
    }
    શૂન્ય read_કિંમત ()
    {
      પસંદગી (width)
      {
        સ્થિતિ 0: current_કિંમત = 0; તોડો;
	સ્થિતિ 1: current_કિંમત = * (અચલ HBINT8  *) p; તોડો;
	સ્થિતિ 2: current_કિંમત = * (અચલ HBINT16 *) p; તોડો;
	સ્થિતિ 4: current_કિંમત = * (અચલ HBINT32 *) p; તોડો;
      }
      p += width;
    }

    જાહેર:

    પ્રકારવ્યાખ્યા પૂર્ણાંક __item_t__;
    __item_t__ __item__ () અચલ
    { પાછા current_કિંમત; }

    તાર્કિક __more__ () અચલ { પાછા run_ગણતરી || p < endp; }
    શૂન્ય __next__ ()
    {
      run_ગણતરી--;
      જો (unlikely (!ensure_run ()))
	પાછા;
      read_કિંમત ();
    }
    શૂન્ય __forward__ (unsigned n)
    {
      જો (unlikely (!ensure_run ()))
	પાછા;
      જ્યારે (n)
      {
	unsigned i = hb_min (n, (unsigned) run_ગણતરી);
	run_ગણતરી -= i;
	n -= i;
	p += (i - 1) * width;
	જો (unlikely (!ensure_run ()))
	  પાછા;
	read_કિંમત ();
      }
    }
    તાર્કિક operator != (અચલ iter_t& o) અચલ
    { પાછા p != o.p || run_ગણતરી != o.run_ગણતરી; }
    iter_t __end__ () અચલ
    {
      iter_t it (endp, 0);
      પાછા it;
    }
  };

  સંરચના fetcher_t
  {
    fetcher_t (અચલ unsigned અક્ષર *p_, unsigned len_)
	      : p (p_), end (p_ + len_) {}

    ખાનગી:
    અચલ unsigned અક્ષર *p;
    અચલ unsigned અક્ષર * અચલ end;
    signed run_ગણતરી = 0;
    unsigned width = 0;

    HB_ALWAYS_INLINE
    તાર્કિક ensure_run ()
    {
      જો (likely (run_ગણતરી > 0)) પાછા સાચું;
      પાછા _ensure_run ();
    }

    તાર્કિક _ensure_run ()
    {
      જો (unlikely (p >= end))
      {
        run_ગણતરી = 0;
	પાછા ખોટું;
      }

      unsigned control = *p++;
      run_ગણતરી = (control & કિંમત_RUN_ગણતરી_MASK) + 1;
      width = control & કિંમતો_SIZE_MASK;
      પસંદગી (width)
      {
        સ્થિતિ કિંમતો_ARE_ZEROS: width = 0; તોડો;
	સ્થિતિ કિંમતો_ARE_BYTES: width = HBINT8::static_size;  તોડો;
	સ્થિતિ કિંમતો_ARE_શબ્દો: width = HBINT16::static_size; તોડો;
	સ્થિતિ કિંમતો_ARE_LONGS: width = HBINT32::static_size; તોડો;
	મૂળભૂત: પ્રતિજ્ઞા (ખોટું);
      }

      જો (unlikely (p + run_ગણતરી * width > end))
      {
	run_ગણતરી = 0;
	પાછા ખોટું;
      }

      પાછા સાચું;
    }

    જાહેર:
    શૂન્ય skip (unsigned n)
    {
      જ્યારે (n)
      {
	જો (unlikely (!ensure_run ()))
	  પાછા;
	unsigned i = hb_min (n, (unsigned) run_ગણતરી);
	run_ગણતરી -= i;
	n -= i;
	p += i * width;
      }
    }

    ખાનગી:
    template <તાર્કિક scaled, typename T>
    શૂન્ય _add_to (hb_array_t<T> out, T scale = T (1))
    {
      unsigned n = out.length;
      T *arrayZ = out.arrayZ;

      માટે (unsigned i = 0; i < n;)
      {
	જો (unlikely (!ensure_run ()))
	  તોડો;
	unsigned ગણતરી = hb_min (n - i, (unsigned) run_ગણતરી);
	પસંદગી (width)
	{
	  સ્થિતિ 0:
	  {
	    arrayZ += ગણતરી;
	    તોડો;
	  }
	  સ્થિતિ 1:
	  {
	    અચલ auto *pp = (અચલ HBINT8 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    માટે (; j + 3 < ગણતરી; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    માટે (; j < ગણતરી; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (અચલ unsigned અક્ષર *) pp;
	  }
	  તોડો;
	  સ્થિતિ 2:
	  {
	    અચલ auto *pp = (અચલ HBINT16 *) p;
	    unsigned j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    માટે (; j + 3 < ગણતરી; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    માટે (; j < ગણતરી; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (અચલ unsigned અક્ષર *) pp;
	  }
	  તોડો;
	  સ્થિતિ 4:
	  {
	    અચલ auto *pp = (અચલ HBINT32 *) p;
	    માટે (unsigned j = 0; j < ગણતરી; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (અચલ unsigned અક્ષર *) pp;
	  }
	  તોડો;
	}
	run_ગણતરી -= ગણતરી;
	i += ગણતરી;
      }
    }

    જાહેર:
    template <typename T>
    શૂન્ય add_to (hb_array_t<T> out, દશાંશ scale = 1.0f)
    {
#ifndef HB_OPTIMIZE_SIZE
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      જો (ખોટું && scale == 1.0f)
        _add_to<ખોટું> (out);
      નહીંતર
#endif
        _add_to<સાચું> (out, (T) scale);
    }
  };
};

સંરચના TupleList : CFF2Index
{
  Tuple_કિંમતો::iter_t operator [] (unsigned i) અચલ
  {
    auto bytes = CFF2Index::operator [] (i);
    પાછા Tuple_કિંમતો::iter_t (bytes.arrayZ, bytes.length);
  }

  Tuple_કિંમતો::fetcher_t fetcher (unsigned i) અચલ
  {
    auto bytes = CFF2Index::operator [] (i);
    પાછા Tuple_કિંમતો::fetcher_t (bytes.arrayZ, bytes.length);
  }
};


// Alignment

template <unsigned પૂર્ણાંક alignment>
સંરચના Align
{
  size_t get_size (અચલ શૂન્ય *base) અચલ
  {
    unsigned offset = (અચલ અક્ષર *) આ - (અચલ અક્ષર *) base;
    પાછા (alignment - offset) & (alignment - 1);
  }

  જાહેર:
  DEFINE_SIZE_MIN (0);
};



} /* namespace OT */


#endif /* HB_OPEN_TYPE_HH */
