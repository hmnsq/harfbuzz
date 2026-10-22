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

#अगर_अघोषित HB_OPEN_TYPE_HH
#घोषणा HB_OPEN_TYPE_HH

#समावेश "hb.hh"
#समावेश "hb-blob.hh"
#समावेश "hb-face.hh"
#समावेश "hb-machinery.hh"
#समावेश "hb-meta.hh"
#समावेश "hb-subset.hh"


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
ढाँचा <bool BE,
	  typename Type,
	  अचिन्हित पूर्णांक आकार_2 = माप (Type)>
काष्ठा अंक_Type
{
  प्रकार Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  प्रकार typename std::conditional<std::is_integral<Type>::value && माप (Type) <= माप(पूर्णांक),
				     typename std::conditional<std::is_signed<Type>::value, चिन्हित, अचिन्हित>::type,
				     Type>::type WideType;

  अंक_Type () = शेष;
  explicit constexpr अंक_Type (Type V) : v {V} {}
  अंक_Type& चालक = (Type V) { v = V; वापस *यह; }

  चालक WideType () स्थिर { वापस v; }

  bool चालक == (स्थिर अंक_Type &o) स्थिर { वापस (Type) v == (Type) o.v; }
  bool चालक != (स्थिर अंक_Type &o) स्थिर { वापस !(*यह == o); }

  अंक_Type& चालक += (WideType गिनती) { *यह = *यह + गिनती; वापस *यह; }
  अंक_Type& चालक -= (WideType गिनती) { *यह = *यह - गिनती; वापस *यह; }
  अंक_Type& चालक ++ () { *यह += 1; वापस *यह; }
  अंक_Type& चालक -- () { *यह -= 1; वापस *यह; }
  अंक_Type चालक ++ (पूर्णांक) { अंक_Type c (*यह); ++*यह; वापस c; }
  अंक_Type चालक -- (पूर्णांक) { अंक_Type c (*यह); --*यह; वापस c; }

  uint32_t hash () स्थिर { वापस hb_array ((स्थिर अक्षर *) &v, माप (v)).hash (); }
  HB_INTERNAL अटल पूर्णांक cmp (स्थिर अंक_Type *a, स्थिर अंक_Type *b)
  { वापस b->cmp (*a); }
  HB_INTERNAL अटल पूर्णांक cmp (स्थिर व्योम *a, स्थिर व्योम *b)
  {
    अंक_Type *pa = (अंक_Type *) a;
    अंक_Type *pb = (अंक_Type *) b;

    वापस pb->cmp (*pa);
  }
  ढाँचा <typename Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  पूर्णांक cmp (Type2 a) स्थिर
  {
    Type b = v;
    वापस (a > b) - (a < b);
  }
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }
  रक्षित:
  typename std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, आकार_2>,
			    HBFloat<BE, Type, आकार_2>>::type v;
  खुला:
  DEFINE_आकार_STATIC (आकार_2);
};

प्रकार अंक_Type<true, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
प्रकार अंक_Type<true, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
प्रकार अंक_Type<true, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
प्रकार अंक_Type<true, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
प्रकार अंक_Type<true, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
प्रकार अंक_Type<true, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
प्रकार अंक_Type<true, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
प्रकार अंक_Type<true, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
प्रकार अंक_Type<true, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

प्रकार अंक_Type<false, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
प्रकार अंक_Type<false, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
प्रकार अंक_Type<false, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
प्रकार अंक_Type<false, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
प्रकार अंक_Type<false, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
प्रकार अंक_Type<false, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

प्रकार अंक_Type<true,  भग्न>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
प्रकार अंक_Type<true,  द्विगुन> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
प्रकार अंक_Type<false, भग्न>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
प्रकार अंक_Type<false, द्विगुन> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
काष्ठा HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& चालक = (uint16_t i ) { HBUINT16::चालक= (i); वापस *यह; }
  खुला:
  DEFINE_आकार_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
काष्ठा HBUINT32VAR
{
  माप_प्रकार get_आकार () स्थिर
  {
    अचिन्हित b0 = v[0];
    अगर (b0 < 0x80)
      वापस 1;
    अन्यथा अगर (b0 < 0xC0)
      वापस 2;
    अन्यथा अगर (b0 < 0xE0)
      वापस 3;
    अन्यथा अगर (b0 < 0xF0)
      वापस 4;
    अन्यथा
      वापस 5;
  }

  अटल माप_प्रकार get_आकार (uint32_t v)
  {
    अगर (v < 0x80)
      वापस 1;
    अन्यथा अगर (v < 0x4000)
      वापस 2;
    अन्यथा अगर (v < 0x200000)
      वापस 3;
    अन्यथा अगर (v < 0x10000000)
      वापस 4;
    अन्यथा
      वापस 5;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_आकार ()));
  }

  चालक uint32_t () स्थिर
  {
    अचिन्हित b0 = v[0];
    अगर (b0 < 0x80)
      वापस b0;
    अन्यथा अगर (b0 < 0xC0)
      वापस ((b0 & 0x3F) << 8) | v[1];
    अन्यथा अगर (b0 < 0xE0)
      वापस ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    अन्यथा अगर (b0 < 0xF0)
      वापस ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    अन्यथा
      वापस (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  अटल अचिन्हित serialize_unsafe (अचिन्हित अक्षर *buf, uint32_t v)
  {
    अचिन्हित len = get_आकार (v);
    अचिन्हित अक्षर *p = buf + len;
    क्रम (अचिन्हित i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    अगर (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    वापस len;
  }

  अटल bool serialize (hb_serialize_context_t *c, uint32_t v)
  {
    अचिन्हित len = get_आकार (v);
    अचिन्हित अक्षर *buf = c->allocate_आकार<अचिन्हित अक्षर> (len, false);
    अगर (unlikely (!buf))
      वापस false;

    serialize_unsafe (buf, v);
    वापस true;
  }

  रक्षित:
  अचिन्हित अक्षर v[5];

  खुला:
  DEFINE_आकार_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
प्रकार HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
प्रकार HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
प्रकार HBUINT16 UFWORD;

ढाँचा <typename Type, अचिन्हित fraction_bits>
काष्ठा HBFixed : Type
{
  अटल constexpr भग्न mult = 1.f / (1 << fraction_bits);
  static_assert (Type::static_आकार * 8 > fraction_bits, "");

  चालक चिन्हित () स्थिर = मिटाओ;
  चालक अचिन्हित () स्थिर = मिटाओ;
  explicit चालक भग्न () स्थिर { वापस to_float (); }
  typename Type::type to_int () स्थिर { वापस Type::v; }
  व्योम set_int (typename Type::type i ) { Type::v = i; }
  भग्न to_float (भग्न offset = 0) स्थिर  { वापस ((int32_t) Type::v + offset) * mult; }
  व्योम set_float (भग्न f) { Type::v = roundf (f / mult); }
  खुला:
  DEFINE_आकार_STATIC (Type::static_आकार);
};

/* 16-bit signed fixed number with the low 14 bits of fraction (2.14). */
using F2DOT14 = HBFixed<HBINT16, 14>;
using F4DOT12 = HBFixed<HBINT16, 12>;
using F6DOT10 = HBFixed<HBINT16, 10>;

/* 32-bit signed fixed-point number (16.16). */
using F16DOT16 = HBFixed<HBINT32, 16>;

/* Date represented in number of seconds since 12:00 midnight, January 1,
 * 1904. The value is represented as a signed 64-bit integer. */
काष्ठा LONGDATETIME
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }
  रक्षित:
  HBINT32 major;
  HBUINT32 minor;
  खुला:
  DEFINE_आकार_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
काष्ठा Tag : HBUINT32
{
  Tag& चालक = (hb_tag_t i) { HBUINT32::चालक= (i); वापस *यह; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  चालक स्थिर अक्षर* () स्थिर { वापस reinterpret_cast<स्थिर अक्षर *> (यह); }
  चालक अक्षर* ()             { वापस reinterpret_cast<अक्षर *> (यह); }
  खुला:
  DEFINE_आकार_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
काष्ठा HBGlyphID16 : HBUINT16
{
  HBGlyphID16& चालक = (uint16_t i) { HBUINT16::चालक= (i); वापस *यह; }
};
काष्ठा HBGlyphID24 : HBUINT24
{
  HBGlyphID24& चालक = (uint32_t i) { HBUINT24::चालक= (i); वापस *यह; }
};

/* Script/language-system/feature index */
काष्ठा सूचकांक : HBUINT16 {
  अटल constexpr अचिन्हित NOT_FOUND_सूचकांक = 0xFFFFu;
  सूचकांक& चालक = (uint16_t i) { HBUINT16::चालक= (i); वापस *यह; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, सूचकांक);

प्रकार सूचकांक NameID;

काष्ठा VarIdx : HBUINT32 {
  अटल constexpr अचिन्हित NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक, "");
  VarIdx& चालक = (uint32_t i) { HBUINT32::चालक= (i); वापस *यह; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, VarIdx);

/* Offset, Null offset = 0 */
ढाँचा <typename Type, bool has_null=true>
काष्ठा Offset : Type
{
  Offset& चालक = (typename Type::type i) { Type::चालक= (i); वापस *यह; }

  प्रकार Type type;

  bool is_null () स्थिर { वापस has_null && 0 == *यह; }

  खुला:
  DEFINE_आकार_STATIC (माप (Type));
};

प्रकार Offset<HBUINT16> Offset16;
प्रकार Offset<HBUINT24> Offset24;
प्रकार Offset<HBUINT32> Offset32;


/* CheckSum */
काष्ठा Check_योग : HBUINT32
{
  Check_योग& चालक = (uint32_t i) { HBUINT32::चालक= (i); वापस *यह; }

  /* This is reference implementation from the spec. */
  अटल uint32_t CalcTableChecksum (स्थिर HBUINT32 *Table, uint32_t लंबाई_3)
  {
    uint32_t Sum = 0L;
    निश्चित (0 == (लंबाई_3 & 3));
    स्थिर HBUINT32 *EndPtr = Table + लंबाई_3 / HBUINT32::static_आकार;

    जबतक (Table < EndPtr)
      Sum += *Table++;
    वापस Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  व्योम set_for_तथ्य (स्थिर व्योम *तथ्य, अचिन्हित पूर्णांक लंबाई)
  { *यह = CalcTableChecksum ((स्थिर HBUINT32 *) तथ्य, लंबाई); }

  खुला:
  DEFINE_आकार_STATIC (4);
};


/*
 * Version Numbers
 */

ढाँचा <typename FixedType=HBUINT16>
काष्ठा FixedVersion
{
  uint32_t to_int () स्थिर { वापस (major << (माप (FixedType) * 8)) + minor; }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  FixedType major;
  FixedType minor;
  खुला:
  DEFINE_आकार_STATIC (2 * माप (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

ढाँचा <typename Type, bool has_null>
काष्ठा _hb_has_null
{
  अटल स्थिर Type *get_null () { वापस nullptr; }
  अटल Type *get_crap ()       { वापस nullptr; }
};
ढाँचा <typename Type>
काष्ठा _hb_has_null<Type, true>
{
  अटल स्थिर Type *get_null () { वापस &Null (Type); }
  अटल       Type *get_crap () { वापस &Crap (Type); }
};

ढाँचा <typename Type, typename OffsetType, typename BaseType=व्योम, bool has_null=true>
काष्ठा OffsetTo : Offset<OffsetType, has_null>
{
  using target_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == false ||
		 (hb_has_null_आकार_2 (Type) || !hb_has_min_आकार_2 (Type)), "");

  HB_DELETE_COPY_ASSIGN (OffsetTo);
  OffsetTo () = शेष;

  OffsetTo& चालक = (typename OffsetType::type i) { OffsetType::चालक= (i); वापस *यह; }

  स्थिर Type& चालक () (स्थिर व्योम *base) स्थिर
  {
    अगर (unlikely (यह->is_null ())) वापस *_hb_has_null<Type, has_null>::get_null ();
    वापस StructAtOffset<स्थिर Type> (base, *यह);
  }
  Type& चालक () (व्योम *base) स्थिर
  {
    अगर (unlikely (यह->is_null ())) वापस *_hb_has_null<Type, has_null>::get_crap ();
    वापस StructAtOffset<Type> (base, *यह);
  }

  ढाँचा <typename Base,
	    hb_enable_if (hb_is_convertible (स्थिर Base, स्थिर BaseType *))>
  मित्र स्थिर Type& चालक + (स्थिर Base &base, स्थिर OffsetTo &offset) { वापस offset ((स्थिर व्योम *) base); }
  ढाँचा <typename Base,
	    hb_enable_if (hb_is_convertible (स्थिर Base, स्थिर BaseType *))>
  मित्र स्थिर Type& चालक + (स्थिर OffsetTo &offset, स्थिर Base &base) { वापस offset ((स्थिर व्योम *) base); }
  ढाँचा <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  मित्र Type& चालक + (Base &&base, OffsetTo &offset) { वापस offset ((व्योम *) base); }
  ढाँचा <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  मित्र Type& चालक + (OffsetTo &offset, Base &&base) { वापस offset ((व्योम *) base); }


  ढाँचा <typename Base, typename ...Ts>
  bool serialize_subset (hb_subset_context_t *c, स्थिर OffsetTo& src,
			 स्थिर Base *src_base, Ts&&... ds)
  {
    *यह = 0;
    अगर (src.is_null ())
      वापस false;

    स्वतः *s = c->serializer;

    s->push ();

    bool ret = c->dispatch (src_base+src, std::forward<Ts> (ds)...);

    अगर (ret || !has_null)
      s->add_link (*यह, s->निकालो_pack ());
    अन्यथा
      s->निकालो_discard ();

    वापस ret;
  }


  ढाँचा <typename ...Ts>
  bool serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *यह = 0;

    Type* obj = c->push<Type> ();
    bool ret = obj->serialize (c, std::forward<Ts> (ds)...);

    अगर (ret)
      c->add_link (*यह, c->निकालो_pack ());
    अन्यथा
      c->निकालो_discard ();

    वापस ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  ढाँचा <typename ...Ts>
  bool serialize_copy (hb_serialize_context_t *c, स्थिर OffsetTo& src,
		       स्थिर व्योम *src_base, अचिन्हित dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *यह = 0;
    अगर (src.is_null ())
      वापस false;

    c->push ();

    bool ret = c->copy (src_base+src, std::forward<Ts> (ds)...);

    c->add_link (*यह, c->निकालो_pack (), whence, dst_bias);

    वापस ret;
  }

  bool serialize_copy (hb_serialize_context_t *c, स्थिर OffsetTo& src,
		       स्थिर व्योम *src_base, अचिन्हित dst_bias = 0)
  { वापस serialize_copy (c, src, src_base, dst_bias, hb_serialize_context_t::शीर्ष); }

  bool sanitize_shallow (hb_sanitize_context_t *c, स्थिर BaseType *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!c->check_struct (यह))) return_trace (false);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    अगर (unlikely ((स्थिर अक्षर *) base + (अचिन्हित) *यह < (स्थिर अक्षर *) base)) return_trace (false);
    return_trace (true);
  }

  ढाँचा <typename ...Ts>
#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  bool sanitize (hb_sanitize_context_t *c, स्थिर BaseType *base, Ts&&... ds) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (sanitize_shallow (c, base) &&
		  hb_barrier () &&
		  (यह->is_null () ||
		   c->dispatch (StructAtOffset<Type> (base, *यह), std::forward<Ts> (ds)...)));
  }

  DEFINE_आकार_STATIC (माप (OffsetType));
};
/* Partial specializations. */
ढाँचा <typename Type, typename BaseType=व्योम, bool has_null=true> using Offset16To = OffsetTo<Type, HBUINT16, BaseType, has_null>;
ढाँचा <typename Type, typename BaseType=व्योम, bool has_null=true> using Offset24To = OffsetTo<Type, HBUINT24, BaseType, has_null>;
ढाँचा <typename Type, typename BaseType=व्योम, bool has_null=true> using Offset32To = OffsetTo<Type, HBUINT32, BaseType, has_null>;

ढाँचा <typename Type, typename OffsetType, typename BaseType=व्योम> using NNOffsetTo = OffsetTo<Type, OffsetType, BaseType, false>;
ढाँचा <typename Type, typename BaseType=व्योम> using NNOffset16To = Offset16To<Type, BaseType, false>;
ढाँचा <typename Type, typename BaseType=व्योम> using NNOffset24To = Offset24To<Type, BaseType, false>;
ढाँचा <typename Type, typename BaseType=व्योम> using NNOffset32To = Offset32To<Type, BaseType, false>;


/*
 * Array Types
 */

ढाँचा <typename Type>
काष्ठा UnsizedArrayOf
{
  प्रकार Type item_t;
  अटल constexpr अचिन्हित item_आकार = hb_static_आकार (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (UnsizedArrayOf);

  स्थिर Type& चालक [] (अचिन्हित पूर्णांक i) स्थिर
  {
    वापस arrayZ[i];
  }
  Type& चालक [] (अचिन्हित पूर्णांक i)
  {
    वापस arrayZ[i];
  }

  अटल माप_प्रकार get_आकार (अचिन्हित पूर्णांक len)
  { वापस hb_unsigned_mul_saturate (len, Type::static_आकार); }

  ढाँचा <typename T> चालक T * () { वापस arrayZ; }
  ढाँचा <typename T> चालक स्थिर T * () स्थिर { वापस arrayZ; }
  hb_array_t<Type> as_array (अचिन्हित पूर्णांक len)
  { वापस hb_array (arrayZ, len); }
  hb_array_t<स्थिर Type> as_array (अचिन्हित पूर्णांक len) स्थिर
  { वापस hb_array (arrayZ, len); }

  ढाँचा <typename T>
  Type &lsearch (अचिन्हित पूर्णांक len, स्थिर T &x, Type &not_found = Crap (Type))
  { वापस *as_array (len).lsearch (x, &not_found); }
  ढाँचा <typename T>
  स्थिर Type &lsearch (अचिन्हित पूर्णांक len, स्थिर T &x, स्थिर Type &not_found = Null (Type)) स्थिर
  { वापस *as_array (len).lsearch (x, &not_found); }
  ढाँचा <typename T>
  bool lfind (अचिन्हित पूर्णांक len, स्थिर T &x, अचिन्हित पूर्णांक *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णांक to_store = (अचिन्हित पूर्णांक) -1) स्थिर
  { वापस as_array (len).lfind (x, i, not_found, to_store); }

  व्योम क्विक (अचिन्हित पूर्णांक len, अचिन्हित पूर्णांक start = 0, अचिन्हित पूर्णांक end = (अचिन्हित पूर्णांक) -1)
  { as_array (len).क्विक (start, end); }

  bool serialize (hb_serialize_context_t *c, अचिन्हित पूर्णांक वस्तुएँ_len, bool सफा = true)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_आकार (यह, get_आकार (वस्तुएँ_len), सफा))) return_trace (false);
    return_trace (true);
  }
  ढाँचा <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  bool serialize (hb_serialize_context_t *c, Iterator वस्तुएँ)
  {
    TRACE_SERIALIZE (यह);
    अचिन्हित गिनती = hb_len (वस्तुएँ);
    अगर (unlikely (!serialize (c, गिनती, false))) return_trace (false);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    क्रम (अचिन्हित i = 0; i < गिनती; i++, ++वस्तुएँ)
      arrayZ[i] = *वस्तुएँ;
    return_trace (true);
  }

  UnsizedArrayOf* copy (hb_serialize_context_t *c, अचिन्हित गिनती) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः *out = c->start_embed (यह);
    अगर (unlikely (!as_array (गिनती).copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  ढाँचा <typename ...Ts>
  HB_ALWAYS_INLINE
  bool sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक गिनती, Ts&&... ds) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!sanitize_shallow (c, गिनती))) return_trace (false);
    अगर (!माप... (Ts) && hb_is_trivially_copyable(Type)) return_trace (true);
    hb_barrier ();
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (false);
    return_trace (true);
  }

  bool sanitize_shallow (hb_sanitize_context_t *c, अचिन्हित पूर्णांक गिनती) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_array (arrayZ, गिनती));
  }

  खुला:
  Type		arrayZ[HB_VAR_ARRAY];
  खुला:
  DEFINE_आकार_UNBOUNDED (0);
};

/* Unsized array of offset's */
ढाँचा <typename Type, typename OffsetType, typename BaseType=व्योम, bool has_null=true>
using UnsizedArray16OfOffsetTo = UnsizedArrayOf<OffsetTo<Type, OffsetType, BaseType, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
ढाँचा <typename Type, typename OffsetType, typename BaseType=व्योम, bool has_null=true>
काष्ठा UnsizedListOfOffset16To : UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
{
  स्थिर Type& चालक [] (पूर्णांक i_) स्थिर
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    स्थिर OffsetTo<Type, OffsetType, BaseType, has_null> *p = &यह->arrayZ[i];
    अगर (unlikely ((स्थिर व्योम *) p < (स्थिर व्योम *) यह->arrayZ)) वापस Null (Type); /* Overflowed. */
    hb_barrier ();
    वापस यह+*p;
  }
  Type& चालक [] (पूर्णांक i_)
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    स्थिर OffsetTo<Type, OffsetType, BaseType, has_null> *p = &यह->arrayZ[i];
    अगर (unlikely ((स्थिर व्योम *) p < (स्थिर व्योम *) यह->arrayZ)) वापस Crap (Type); /* Overflowed. */
    hb_barrier ();
    वापस यह+*p;
  }

  ढाँचा <typename ...Ts>
  bool sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक गिनती, Ts&&... ds) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace ((UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
		   ::sanitize (c, गिनती, यह, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
ढाँचा <typename Type>
काष्ठा SortedUnsizedArrayOf : UnsizedArrayOf<Type>
{
  hb_sorted_array_t<Type> as_array (अचिन्हित पूर्णांक len)
  { वापस hb_sorted_array (यह->arrayZ, len); }
  hb_sorted_array_t<स्थिर Type> as_array (अचिन्हित पूर्णांक len) स्थिर
  { वापस hb_sorted_array (यह->arrayZ, len); }
  चालक hb_sorted_array_t<Type> ()             { वापस as_array (); }
  चालक hb_sorted_array_t<स्थिर Type> () स्थिर { वापस as_array (); }

  ढाँचा <typename T>
  Type &द्वा_खोज (अचिन्हित पूर्णांक len, स्थिर T &x, Type &not_found = Crap (Type))
  { वापस *as_array (len).द्वा_खोज (x, &not_found); }
  ढाँचा <typename T>
  स्थिर Type &द्वा_खोज (अचिन्हित पूर्णांक len, स्थिर T &x, स्थिर Type &not_found = Null (Type)) स्थिर
  { वापस *as_array (len).द्वा_खोज (x, &not_found); }
  ढाँचा <typename T>
  bool bfind (अचिन्हित पूर्णांक len, स्थिर T &x, अचिन्हित पूर्णांक *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णांक to_store = (अचिन्हित पूर्णांक) -1) स्थिर
  { वापस as_array (len).bfind (x, i, not_found, to_store); }
};


/* An array with a number of elements. */
ढाँचा <typename Type, typename LenType>
काष्ठा ArrayOf
{
  प्रकार Type item_t;
  अटल constexpr अचिन्हित item_आकार = hb_static_आकार (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOf);

  स्थिर Type& चालक [] (पूर्णांक i_) स्थिर
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    अगर (unlikely (i >= len)) वापस Null (Type);
    hb_barrier ();
    वापस arrayZ[i];
  }
  Type& चालक [] (पूर्णांक i_)
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    अगर (unlikely (i >= len)) वापस Crap (Type);
    hb_barrier ();
    वापस arrayZ[i];
  }

  माप_प्रकार get_आकार () स्थिर
  { वापस hb_unsigned_mul_add_saturate (len, Type::static_आकार, len.static_आकार); }

  explicit चालक bool () स्थिर { वापस len; }

  व्योम निकालो () { len--; }

  hb_array_t<      Type> as_array ()       { वापस hb_array (arrayZ, len); }
  hb_array_t<स्थिर Type> as_array () स्थिर { वापस hb_array (arrayZ, len); }

  /* Iterator. */
  प्रकार hb_array_t<स्थिर Type>   iter_t;
  प्रकार hb_array_t<      Type> writer_t;
    iter_t   iter () स्थिर { वापस as_array (); }
  writer_t writer ()       { वापस as_array (); }
  चालक   iter_t () स्थिर { वापस   iter (); }
  चालक writer_t ()       { वापस writer (); }

  /* Faster range-based for loop. */
  स्थिर Type *begin () स्थिर { वापस arrayZ; }
  स्थिर Type *end () स्थिर { वापस arrayZ + len; }

  ढाँचा <typename T>
  Type &lsearch (स्थिर T &x, Type &not_found = Crap (Type))
  { वापस *as_array ().lsearch (x, &not_found); }
  ढाँचा <typename T>
  स्थिर Type &lsearch (स्थिर T &x, स्थिर Type &not_found = Null (Type)) स्थिर
  { वापस *as_array ().lsearch (x, &not_found); }
  ढाँचा <typename T>
  bool lfind (स्थिर T &x, अचिन्हित पूर्णांक *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णांक to_store = (अचिन्हित पूर्णांक) -1) स्थिर
  { वापस as_array ().lfind (x, i, not_found, to_store); }

  व्योम क्विक ()
  { as_array ().क्विक (); }

  HB_NODISCARD bool serialize (hb_serialize_context_t *c, अचिन्हित वस्तुएँ_len, bool सफा = true)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    c->check_assign (len, वस्तुएँ_len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    अगर (unlikely (!c->extend_आकार (यह, get_आकार (), सफा))) return_trace (false);
    return_trace (true);
  }
  ढाँचा <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD bool serialize (hb_serialize_context_t *c, Iterator वस्तुएँ)
  {
    TRACE_SERIALIZE (यह);
    अचिन्हित गिनती = hb_len (वस्तुएँ);
    अगर (unlikely (!serialize (c, गिनती, false))) return_trace (false);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    क्रम (अचिन्हित i = 0; i < गिनती; i++, ++वस्तुएँ)
      arrayZ[i] = *वस्तुएँ;
    return_trace (true);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (यह);
    len++;
    अगर (unlikely (!len || !c->extend (यह)))
    {
      len--;
      return_trace (nullptr);
    }
    return_trace (&arrayZ[len - 1]);
  }

  ArrayOf* copy (hb_serialize_context_t *c) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः *out = c->start_embed (यह);
    अगर (unlikely (!c->extend_min (out))) return_trace (nullptr);
    c->check_assign (out->len, len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    अगर (unlikely (!as_array ().copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  ढाँचा <typename ...Ts>
  HB_ALWAYS_INLINE
  bool sanitize (hb_sanitize_context_t *c, Ts&&... ds) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!sanitize_shallow (c))) return_trace (false);
    अगर (!माप... (Ts) && hb_is_trivially_copyable(Type)) return_trace (true);
    hb_barrier ();
    अचिन्हित पूर्णांक गिनती = len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (false);
    return_trace (true);
  }

  bool sanitize_shallow (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (len.sanitize (c) &&
		  hb_barrier () &&
		  c->check_array_sized (arrayZ, len, माप (LenType)));
  }

  खुला:
  LenType	len;
  Type		arrayZ[HB_VAR_ARRAY];
  खुला:
  DEFINE_आकार_ARRAY (माप (LenType), arrayZ);
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
काष्ठा List16OfOffsetTo : ArrayOf<OffsetTo<Type, OffsetType>, HBUINT16>
{
  स्थिर Type& चालक [] (पूर्णांक i_) स्थिर
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    अगर (unlikely (i >= यह->len)) वापस Null (Type);
    hb_barrier ();
    वापस यह+यह->arrayZ[i];
  }
  स्थिर Type& चालक [] (पूर्णांक i_)
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    अगर (unlikely (i >= यह->len)) वापस Crap (Type);
    hb_barrier ();
    वापस यह+यह->arrayZ[i];
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    काष्ठा List16OfOffsetTo *out = c->serializer->embed (*यह);
    अगर (unlikely (!out)) return_trace (false);
    अचिन्हित पूर्णांक गिनती = यह->len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      out->arrayZ[i].serialize_subset (c, यह->arrayZ[i], यह, out);
    return_trace (true);
  }

  ढाँचा <typename ...Ts>
  bool sanitize (hb_sanitize_context_t *c, Ts&&... ds) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace ((Array16Of<OffsetTo<Type, OffsetType>>::sanitize (c, यह, std::forward<Ts> (ds)...)));
  }
};

ढाँचा <typename Type>
using List16OfOffset16To = List16OfOffsetTo<Type, HBUINT16>;

/* An array starting at second element. */
ढाँचा <typename Type, typename LenType>
काष्ठा HeadlessArrayOf
{
  अटल constexpr अचिन्हित item_आकार = Type::static_आकार;

  HB_DELETE_CREATE_COPY_ASSIGN (HeadlessArrayOf);

  स्थिर Type& चालक [] (पूर्णांक i_) स्थिर
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    अगर (unlikely (i >= lenP1 || !i)) वापस Null (Type);
    hb_barrier ();
    वापस arrayZ[i-1];
  }
  Type& चालक [] (पूर्णांक i_)
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    अगर (unlikely (i >= lenP1 || !i)) वापस Crap (Type);
    hb_barrier ();
    वापस arrayZ[i-1];
  }
  माप_प्रकार get_आकार () स्थिर
  { वापस hb_unsigned_mul_add_saturate (get_लंबाई (), Type::static_आकार, lenP1.static_आकार); }

  अचिन्हित get_लंबाई () स्थिर { वापस lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { वापस hb_array (arrayZ, get_लंबाई ()); }
  hb_array_t<स्थिर Type> as_array () स्थिर { वापस hb_array (arrayZ, get_लंबाई ()); }

  /* Iterator. */
  प्रकार hb_array_t<स्थिर Type>   iter_t;
  प्रकार hb_array_t<      Type> writer_t;
    iter_t   iter () स्थिर { वापस as_array (); }
  writer_t writer ()       { वापस as_array (); }
  चालक   iter_t () स्थिर { वापस   iter (); }
  चालक writer_t ()       { वापस writer (); }

  /* Faster range-based for loop. */
  स्थिर Type *begin () स्थिर { वापस arrayZ; }
  स्थिर Type *end () स्थिर { वापस arrayZ + get_लंबाई (); }

  HB_NODISCARD bool serialize (hb_serialize_context_t *c, अचिन्हित पूर्णांक वस्तुएँ_len, bool सफा = true)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    c->check_assign (lenP1, वस्तुएँ_len + 1, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    अगर (unlikely (!c->extend_आकार (यह, get_आकार (), सफा))) return_trace (false);
    return_trace (true);
  }
  ढाँचा <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD bool serialize (hb_serialize_context_t *c, Iterator वस्तुएँ)
  {
    TRACE_SERIALIZE (यह);
    अचिन्हित गिनती = hb_len (वस्तुएँ);
    अगर (unlikely (!serialize (c, गिनती, false))) return_trace (false);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    क्रम (अचिन्हित i = 0; i < गिनती; i++, ++वस्तुएँ)
      arrayZ[i] = *वस्तुएँ;
    return_trace (true);
  }

  ढाँचा <typename ...Ts>
  HB_ALWAYS_INLINE
  bool sanitize (hb_sanitize_context_t *c, Ts&&... ds) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!sanitize_shallow (c))) return_trace (false);
    अगर (!माप... (Ts) && hb_is_trivially_copyable(Type)) return_trace (true);
    hb_barrier ();
    अचिन्हित पूर्णांक गिनती = get_लंबाई ();
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (false);
    return_trace (true);
  }

  निजी:
  bool sanitize_shallow (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (lenP1.sanitize (c) &&
		  hb_barrier () &&
		  (!lenP1 || c->check_array_sized (arrayZ, lenP1 - 1, माप (LenType))));
  }

  खुला:
  LenType	lenP1;
  Type		arrayZ[HB_VAR_ARRAY];
  खुला:
  DEFINE_आकार_ARRAY (माप (LenType), arrayZ);
};
ढाँचा <typename Type> using HeadlessArray16Of = HeadlessArrayOf<Type, HBUINT16>;

/* An array storing length-1. */
ढाँचा <typename Type, typename LenType=HBUINT16>
काष्ठा ArrayOfM1
{
  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOfM1);

  स्थिर Type& चालक [] (पूर्णांक i_) स्थिर
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    अगर (unlikely (i > lenM1)) वापस Null (Type);
    hb_barrier ();
    वापस arrayZ[i];
  }
  Type& चालक [] (पूर्णांक i_)
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    अगर (unlikely (i > lenM1)) वापस Crap (Type);
    hb_barrier ();
    वापस arrayZ[i];
  }
  माप_प्रकार get_आकार () स्थिर
  { वापस hb_unsigned_mul_add_saturate (lenM1 + 1, Type::static_आकार, lenM1.static_आकार); }

  ढाँचा <typename ...Ts>
  HB_ALWAYS_INLINE
  bool sanitize (hb_sanitize_context_t *c, Ts&&... ds) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!sanitize_shallow (c))) return_trace (false);
    अगर (!माप... (Ts) && hb_is_trivially_copyable(Type)) return_trace (true);
    hb_barrier ();
    अचिन्हित पूर्णांक गिनती = lenM1 + 1;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (false);
    return_trace (true);
  }

  निजी:
  bool sanitize_shallow (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (lenM1.sanitize (c) &&
		  hb_barrier () &&
		  (c->check_array_sized (arrayZ, lenM1 + 1, माप (LenType))));
  }

  खुला:
  LenType	lenM1;
  Type		arrayZ[HB_VAR_ARRAY];
  खुला:
  DEFINE_आकार_ARRAY (माप (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
ढाँचा <typename Type, typename LenType>
काष्ठा SortedArrayOf : ArrayOf<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { वापस hb_sorted_array (यह->arrayZ, यह->len); }
  hb_sorted_array_t<स्थिर Type> as_array () स्थिर { वापस hb_sorted_array (यह->arrayZ, यह->len); }

  /* Iterator. */
  प्रकार hb_sorted_array_t<स्थिर Type>   iter_t;
  प्रकार hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () स्थिर { वापस as_array (); }
  writer_t writer ()       { वापस as_array (); }
  चालक   iter_t () स्थिर { वापस   iter (); }
  चालक writer_t ()       { वापस writer (); }

  /* Faster range-based for loop. */
  स्थिर Type *begin () स्थिर { वापस यह->arrayZ; }
  स्थिर Type *end () स्थिर { वापस यह->arrayZ + यह->len; }

  bool serialize (hb_serialize_context_t *c, अचिन्हित पूर्णांक वस्तुएँ_len)
  {
    TRACE_SERIALIZE (यह);
    bool ret = ArrayOf<Type, LenType>::serialize (c, वस्तुएँ_len);
    return_trace (ret);
  }
  ढाँचा <typename Iterator,
	    hb_requires (hb_is_sorted_source_of (Iterator, Type))>
  bool serialize (hb_serialize_context_t *c, Iterator वस्तुएँ)
  {
    TRACE_SERIALIZE (यह);
    bool ret = ArrayOf<Type, LenType>::serialize (c, वस्तुएँ);
    return_trace (ret);
  }

  SortedArrayOf* copy (hb_serialize_context_t *c) स्थिर
  {
    TRACE_SERIALIZE (यह);
    SortedArrayOf* out = reinterpret_cast<SortedArrayOf *> (ArrayOf<Type, LenType>::copy (c));
    return_trace (out);
  }

  ढाँचा <typename T>
  Type &द्वा_खोज (स्थिर T &x, Type &not_found = Crap (Type))
  { वापस *as_array ().द्वा_खोज (x, &not_found); }
  ढाँचा <typename T>
  स्थिर Type &द्वा_खोज (स्थिर T &x, स्थिर Type &not_found = Null (Type)) स्थिर
  { वापस *as_array ().द्वा_खोज (x, &not_found); }
  ढाँचा <typename T>
  bool bfind (स्थिर T &x, अचिन्हित पूर्णांक *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णांक to_store = (अचिन्हित पूर्णांक) -1) स्थिर
  { वापस as_array ().bfind (x, i, not_found, to_store); }
};

ढाँचा <typename Type> using SortedArray16Of = SortedArrayOf<Type, HBUINT16>;
ढाँचा <typename Type> using SortedArray24Of = SortedArrayOf<Type, HBUINT24>;
ढाँचा <typename Type> using SortedArray32Of = SortedArrayOf<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

ढाँचा <typename LenType=HBUINT16>
काष्ठा Bin_खोज_Header
{
  चालक uint32_t () स्थिर { वापस len; }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  Bin_खोज_Header& चालक = (अचिन्हित पूर्णांक v)
  {
    len = v;
    निश्चित (len == v);
    entrySelector = hb_max (1u, hb_bit_storage (v)) - 1;
    खोज_Range = 16 * (1u << entrySelector);
    rangeShift = v * 16 > खोज_Range
		 ? 16 * v - खोज_Range
		 : 0;
    वापस *यह;
  }

  रक्षित:
  LenType	len;
  LenType	खोज_Range;
  LenType	entrySelector;
  LenType	rangeShift;

  खुला:
  DEFINE_आकार_STATIC (8);
};

ढाँचा <typename Type, typename LenType=HBUINT16>
using Bin_खोज_Array_Of = SortedArrayOf<Type, Bin_खोज_Header<LenType>>;


काष्ठा Var_Sized_Bin_खोज_Header
{

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  HBUINT16	unit_आकार;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	खोज_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	entrySelector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	rangeShift;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  खुला:
  DEFINE_आकार_STATIC (10);
};

ढाँचा <typename Type>
काष्ठा Var_Sized_Bin_खोज_Array_Of
{
  अटल constexpr अचिन्हित item_आकार = Type::static_आकार;

  HB_DELETE_CREATE_COPY_ASSIGN (Var_Sized_Bin_खोज_Array_Of);

  bool last_is_terminator () स्थिर
  {
    अगर (unlikely (!header.nUnits)) वापस false;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    स्थिर HBUINT16 *शब्दगण = &StructAtOffset<HBUINT16> (&bytesZ, (header.nUnits - 1) * header.unit_आकार);
    अचिन्हित पूर्णांक गिनती = Type::Termination_Word_गिनती;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (शब्दगण[i] != 0xFFFFu)
	वापस false;
    वापस true;
  }

  स्थिर Type& चालक [] (पूर्णांक i_) स्थिर
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    अगर (unlikely (i >= get_लंबाई ())) वापस Null (Type);
    hb_barrier ();
    वापस StructAtOffset<Type> (&bytesZ, i * header.unit_आकार);
  }
  Type& चालक [] (पूर्णांक i_)
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    अगर (unlikely (i >= get_लंबाई ())) वापस Crap (Type);
    hb_barrier ();
    वापस StructAtOffset<Type> (&bytesZ, i * header.unit_आकार);
  }
  अचिन्हित पूर्णांक get_लंबाई () स्थिर
  { वापस header.nUnits - last_is_terminator (); }
  माप_प्रकार get_आकार () स्थिर
  { वापस hb_unsigned_mul_add_saturate (header.nUnits, header.unit_आकार, header.static_आकार); }

  ढाँचा <typename ...Ts>
  HB_ALWAYS_INLINE
  bool sanitize (hb_sanitize_context_t *c, Ts&&... ds) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!sanitize_shallow (c))) return_trace (false);
    अगर (!माप... (Ts) && hb_is_trivially_copyable(Type)) return_trace (true);
    hb_barrier ();
    अचिन्हित पूर्णांक गिनती = get_लंबाई ();
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (unlikely (!(*यह)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (false);
    return_trace (true);
  }

  ढाँचा <typename T>
  स्थिर Type *द्वा_खोज (स्थिर T &key) स्थिर
  {
    अचिन्हित pos;
    वापस hb_bsearch_impl (&pos,
			    key,
			    (स्थिर व्योम *) bytesZ,
			    get_लंबाई (),
			    header.unit_आकार,
			    _hb_cmp_method<T, Type>)
	   ? (स्थिर Type *) (((स्थिर अक्षर *) &bytesZ) + (pos * header.unit_आकार))
	   : nullptr;
  }

  निजी:
  bool sanitize_shallow (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (header.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_आकार <= header.unit_आकार &&
		  c->check_range (bytesZ.arrayZ,
				  header.nUnits,
				  header.unit_आकार));
  }

  रक्षित:
  Var_Sized_Bin_खोज_Header	header;
  UnsizedArrayOf<HBUINT8>	bytesZ;
  खुला:
  DEFINE_आकार_ARRAY (10, bytesZ);
};


/* CFF INDEX */

ढाँचा <typename गिनती_3>
काष्ठा CFFIndex
{
  अचिन्हित पूर्णांक offset_array_आकार () स्थिर
  { वापस off_आकार * (गिनती + 1); }

  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  bool serialize (hb_serialize_context_t *c,
		  स्थिर Iterable &iterable,
		  स्थिर अचिन्हित *p_तथ्य_आकार = nullptr,
                  अचिन्हित min_off_आकार = 0)
  {
    TRACE_SERIALIZE (यह);
    अचिन्हित तथ्य_आकार;
    अगर (p_तथ्य_आकार)
      तथ्य_आकार = *p_तथ्य_आकार;
    अन्यथा
      योग_आकार (iterable, &तथ्य_आकार);

    स्वतः it = hb_iter (iterable);
    अगर (unlikely (!serialize_header (c, +it, तथ्य_आकार, min_off_आकार))) return_trace (false);
    अचिन्हित अक्षर *ret = c->allocate_आकार<अचिन्हित अक्षर> (तथ्य_आकार, false);
    अगर (unlikely (!ret)) return_trace (false);
    अचिन्हित remaining = तथ्य_आकार;
    क्रम (स्थिर स्वतः &_ : +it)
    {
      अचिन्हित len = _.लंबाई;

      अगर (!len)
	जारी;

      अगर (unlikely (len > remaining)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (false, HB_SERIALIZE_ERROR_INT_OVERFLOW));
      }

      remaining -= len;

      अगर (len <= 1)
      {
	*ret++ = *_.arrayZ;
	जारी;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (true);
  }

  ढाँचा <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  bool serialize_header (hb_serialize_context_t *c,
			 Iterator it,
			 अचिन्हित तथ्य_आकार,
                         अचिन्हित min_off_आकार = 0)
  {
    TRACE_SERIALIZE (यह);

    अचिन्हित off_आकार_2 = (hb_bit_storage (तथ्य_आकार + 1) + 7) / 8;
    off_आकार_2 = hb_max(min_off_आकार, off_आकार_2);

    /* serialize CFFIndex header */
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    यह->गिनती = hb_len (it);
    अगर (!यह->गिनती) return_trace (true);
    अगर (unlikely (!c->extend (यह->off_आकार))) return_trace (false);
    यह->off_आकार = off_आकार_2;
    अगर (unlikely (!c->allocate_आकार<HBUINT8> (off_आकार_2 * (यह->गिनती + 1), false)))
      return_trace (false);

    /* serialize indices */
    अचिन्हित पूर्णांक offset = 1;
    अगर (HB_OPTIMIZE_आकार_VAL)
    {
      अचिन्हित पूर्णांक i = 0;
      क्रम (स्थिर स्वतः &_ : +it)
      {
	set_offset_at (i++, offset);
	offset += hb_len_of (_);
      }
      set_offset_at (i, offset);
    }
    अन्यथा
      चयन (off_आकार_2)
      {
	हाल 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  क्रम (स्थिर स्वतः &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	अवरोध;
	हाल 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  क्रम (स्थिर स्वतः &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	अवरोध;
	हाल 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  क्रम (स्थिर स्वतः &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	अवरोध;
	हाल 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  क्रम (स्थिर स्वतः &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	अवरोध;
	शेष:
	अवरोध;
      }

    निश्चित (offset == तथ्य_आकार + 1);
    return_trace (true);
  }

  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  अटल अचिन्हित योग_आकार (स्थिर Iterable &iterable, अचिन्हित *तथ्य_आकार = nullptr, अचिन्हित min_off_आकार = 0)
  {
    स्वतः it = + hb_iter (iterable);
    अगर (!it)
    {
      अगर (तथ्य_आकार) *तथ्य_आकार = 0;
      वापस min_आकार;
    }

    अचिन्हित योग = 0;
    क्रम (स्थिर स्वतः &_ : +it)
      योग += hb_len_of (_);

    अगर (तथ्य_आकार) *तथ्य_आकार = योग;

    अचिन्हित off_आकार_2 = (hb_bit_storage (योग + 1) + 7) / 8;
    off_आकार_2 = hb_max(min_off_आकार, off_आकार_2);

    वापस min_आकार + HBUINT8::static_आकार + (hb_len (it) + 1) * off_आकार_2 + योग;
  }

  व्योम set_offset_at (अचिन्हित पूर्णांक सूचकांक_2, अचिन्हित पूर्णांक offset)
  {
    निश्चित (सूचकांक_2 <= गिनती);

    अचिन्हित पूर्णांक आकार = off_आकार;
    स्थिर HBUINT8 *p = offsets;
    चयन (आकार)
    {
      हाल 1: ((HBUINT8  *) p)[सूचकांक_2] = offset; अवरोध;
      हाल 2: ((HBUINT16 *) p)[सूचकांक_2] = offset; अवरोध;
      हाल 3: ((HBUINT24 *) p)[सूचकांक_2] = offset; अवरोध;
      हाल 4: ((HBUINT32 *) p)[सूचकांक_2] = offset; अवरोध;
      शेष: वापस;
    }
  }

  निजी:
  अचिन्हित पूर्णांक offset_at (अचिन्हित पूर्णांक सूचकांक_2) स्थिर
  {
    निश्चित (सूचकांक_2 <= गिनती);

    अचिन्हित पूर्णांक आकार = off_आकार;
    स्थिर HBUINT8 *p = offsets;
    चयन (आकार)
    {
      हाल 1: वापस ((HBUINT8  *) p)[सूचकांक_2];
      हाल 2: वापस ((HBUINT16 *) p)[सूचकांक_2];
      हाल 3: वापस ((HBUINT24 *) p)[सूचकांक_2];
      हाल 4: वापस ((HBUINT32 *) p)[सूचकांक_2];
      शेष: वापस 0;
    }
  }

  स्थिर अचिन्हित अक्षर *तथ्य_base () स्थिर
  { वापस (स्थिर अचिन्हित अक्षर *) यह + min_आकार + off_आकार.static_आकार - 1 + offset_array_आकार (); }
  खुला:

  hb_ubytes_t चालक [] (अचिन्हित पूर्णांक सूचकांक_2) स्थिर
  {
    अगर (unlikely (सूचकांक_2 >= गिनती)) वापस hb_ubytes_t ();
    hb_barrier ();
    अचिन्हित offset0 = offset_at (सूचकांक_2);
    अचिन्हित offset1 = offset_at (सूचकांक_2 + 1);
    अगर (unlikely (offset1 < offset0 || offset1 > offset_at (गिनती)))
      वापस hb_ubytes_t ();
    वापस hb_ubytes_t (तथ्य_base () + offset0, offset1 - offset0);
  }

  माप_प्रकार get_आकार () स्थिर
  {
    अगर (गिनती)
      वापस hb_unsigned_add_saturate (min_आकार, off_आकार.static_आकार,
				       offset_array_आकार (), offset_at (गिनती) - 1);
    वापस min_आकार;  /* empty CFFIndex contains count only */
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  hb_barrier () &&
			  (गिनती == 0 || /* empty INDEX */
			   (गिनती < गिनती + 1u &&
			    c->check_struct (&off_आकार) && off_आकार >= 1 && off_आकार <= 4 &&
			    c->check_array (offsets, off_आकार, गिनती + 1u) &&
			    c->check_range (तथ्य_base (), offset_at (गिनती))))));
  }

  खुला:
  गिनती_3		गिनती;		/* Number of object data. Note there are (count+1) offsets */
  निजी:
  HBUINT8	off_आकार;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  खुला:
  DEFINE_आकार_MIN (गिनती_3::static_आकार);
};
प्रकार CFFIndex<HBUINT16> CFF1_सूचकांक;
प्रकार CFFIndex<HBUINT32> CFF2_सूचकांक;


/* TupleValues */
काष्ठा Tuple_मूल्यावली
{
  क्रमागत packed_मूल्य_flag_t
  {
    मूल्यावली_ARE_ZEROS     = 0x80,
    मूल्यावली_ARE_BYTES     = 0x00,
    मूल्यावली_ARE_शब्दगण     = 0x40,
    मूल्यावली_ARE_LONGS     = 0xC0,
    मूल्यावली_आकार_MASK     = 0xC0,
    मूल्य_RUN_गिनती_MASK = 0x3F
  };

  अटल अचिन्हित compile_unsafe (hb_array_t<स्थिर पूर्णांक> values, /* IN */
				  अचिन्हित अक्षर *encoded_bytes /* OUT */)
  {
    अचिन्हित अंक_मूल्यावली_3 = values.लंबाई;
    अचिन्हित encoded_len = 0;
    अचिन्हित i = 0;
    जबतक (i < अंक_मूल्यावली_3)
    {
      पूर्णांक val = values.arrayZ[i];
      अगर (val == 0)
        encoded_len += encode_मूल्य_run_as_zeroes (i, encoded_bytes + encoded_len, values);
      अन्यथा अगर ((int8_t) val == val)
        encoded_len += encode_मूल्य_run_as_bytes (i, encoded_bytes + encoded_len, values);
      अन्यथा अगर ((int16_t) val == val)
        encoded_len += encode_मूल्य_run_as_शब्दगण (i, encoded_bytes + encoded_len, values);
      अन्यथा
        encoded_len += encode_मूल्य_run_as_longs (i, encoded_bytes + encoded_len, values);
    }
    वापस encoded_len;
  }

  अटल अचिन्हित encode_मूल्य_run_as_zeroes (अचिन्हित& i,
					      अचिन्हित अक्षर *it,
					      hb_array_t<स्थिर पूर्णांक> values)
  {
    अचिन्हित अंक_मूल्यावली_3 = values.लंबाई;
    अचिन्हित run_लंबाई = 0;
    अचिन्हित encoded_len = 0;
    जबतक (i < अंक_मूल्यावली_3 && values.arrayZ[i] == 0)
    {
      i++;
      run_लंबाई++;
    }

    जबतक (run_लंबाई >= 64)
    {
      *it++ = अक्षर (मूल्यावली_ARE_ZEROS | 63);
      run_लंबाई -= 64;
      encoded_len++;
    }

    अगर (run_लंबाई)
    {
      *it++ = अक्षर (मूल्यावली_ARE_ZEROS | (run_लंबाई - 1));
      encoded_len++;
    }
    वापस encoded_len;
  }

  अटल अचिन्हित encode_मूल्य_run_as_bytes (अचिन्हित &i,
					     अचिन्हित अक्षर *it,
					     hb_array_t<स्थिर पूर्णांक> values)
  {
    अचिन्हित start = i;
    अचिन्हित अंक_मूल्यावली_3 = values.लंबाई;
    जबतक (i < अंक_मूल्यावली_3)
    {
      पूर्णांक val = values.arrayZ[i];
      अगर ((int8_t) val != val)
        अवरोध;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      अगर (val == 0 && i + 1 < अंक_मूल्यावली_3 && values.arrayZ[i+1] == 0)
        अवरोध;

      i++;
    }
    अचिन्हित run_लंबाई = i - start;

    अचिन्हित encoded_len = 0;

    जबतक (run_लंबाई >= 64)
    {
      *it++ = (मूल्यावली_ARE_BYTES | 63);
      encoded_len++;

      क्रम (अचिन्हित j = 0; j < 64; j++)
	it[j] = static_cast<अक्षर> (values.arrayZ[start + j]);
      it += 64;
      encoded_len += 64;

      start += 64;
      run_लंबाई -= 64;
    }

    अगर (run_लंबाई)
    {
      *it++ = (मूल्यावली_ARE_BYTES | (run_लंबाई - 1));
      encoded_len++;

      क्रम (अचिन्हित j = 0; j < run_लंबाई; j++)
        it[j] = static_cast<अक्षर> (values.arrayZ[start + j]);
      encoded_len += run_लंबाई;
    }

    वापस encoded_len;
  }

  अटल अचिन्हित encode_मूल्य_run_as_शब्दगण (अचिन्हित &i,
					     अचिन्हित अक्षर *it,
					     hb_array_t<स्थिर पूर्णांक> values)
  {
    अचिन्हित start = i;
    अचिन्हित अंक_मूल्यावली_3 = values.लंबाई;
    जबतक (i < अंक_मूल्यावली_3)
    {
      पूर्णांक val = values.arrayZ[i];

      अगर ((int16_t) val != val)
        अवरोध;

      /* start a new run for a single zero value. */
      अगर (val == 0) अवरोध;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      अगर ((int8_t) val == val &&
          i + 1 < अंक_मूल्यावली_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        अवरोध;

      i++;
    }

    अचिन्हित run_लंबाई = i - start;
    अचिन्हित encoded_len = 0;
    जबतक (run_लंबाई >= 64)
    {
      *it++ = (मूल्यावली_ARE_शब्दगण | 63);
      encoded_len++;

      क्रम (अचिन्हित j = 0; j < 64; j++)
      {
        int16_t मूल्य_val = values.arrayZ[start + j];
        *it++ = static_cast<अक्षर> (मूल्य_val >> 8);
        *it++ = static_cast<अक्षर> (मूल्य_val & 0xFF);

        encoded_len += 2;
      }

      start += 64;
      run_लंबाई -= 64;
    }

    अगर (run_लंबाई)
    {
      *it++ = (मूल्यावली_ARE_शब्दगण | (run_लंबाई - 1));
      encoded_len++;
      जबतक (start < i)
      {
        int16_t मूल्य_val = values.arrayZ[start++];
        *it++ = static_cast<अक्षर> (मूल्य_val >> 8);
        *it++ = static_cast<अक्षर> (मूल्य_val & 0xFF);

        encoded_len += 2;
      }
    }
    वापस encoded_len;
  }

  अटल अचिन्हित encode_मूल्य_run_as_longs (अचिन्हित &i,
					     अचिन्हित अक्षर *it,
					     hb_array_t<स्थिर पूर्णांक> values)
  {
    अचिन्हित start = i;
    अचिन्हित अंक_मूल्यावली_3 = values.लंबाई;
    जबतक (i < अंक_मूल्यावली_3)
    {
      पूर्णांक val = values.arrayZ[i];

      अगर ((int16_t) val == val)
        अवरोध;

      i++;
    }

    अचिन्हित run_लंबाई = i - start;
    अचिन्हित encoded_len = 0;
    जबतक (run_लंबाई >= 64)
    {
      *it++ = (मूल्यावली_ARE_LONGS | 63);
      encoded_len++;

      क्रम (अचिन्हित j = 0; j < 64; j++)
      {
        int32_t मूल्य_val = values.arrayZ[start + j];
        *it++ = static_cast<अक्षर> (मूल्य_val >> 24);
        *it++ = static_cast<अक्षर> (मूल्य_val >> 16);
        *it++ = static_cast<अक्षर> (मूल्य_val >> 8);
        *it++ = static_cast<अक्षर> (मूल्य_val & 0xFF);

        encoded_len += 4;
      }

      start += 64;
      run_लंबाई -= 64;
    }

    अगर (run_लंबाई)
    {
      *it++ = (मूल्यावली_ARE_LONGS | (run_लंबाई - 1));
      encoded_len++;
      जबतक (start < i)
      {
        int32_t मूल्य_val = values.arrayZ[start++];
        *it++ = static_cast<अक्षर> (मूल्य_val >> 24);
        *it++ = static_cast<अक्षर> (मूल्य_val >> 16);
        *it++ = static_cast<अक्षर> (मूल्य_val >> 8);
        *it++ = static_cast<अक्षर> (मूल्य_val & 0xFF);

        encoded_len += 4;
      }
    }
    वापस encoded_len;
  }

  ढाँचा <typename T>
#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  अटल bool decompile (स्थिर HBUINT8 *&p /* IN/OUT */,
			 hb_vector_t<T> &values /* IN/OUT */,
			 स्थिर HBUINT8 *end,
			 bool consume_all = false,
			 अचिन्हित start = 0)
  {
    अचिन्हित i = 0;
    अचिन्हित गिनती = consume_all ? अच_पूर्णांक_उच्च : values.लंबाई;
    अगर (consume_all)
      values.alloc ((end - p) / 2);
    जबतक (i < गिनती)
    {
      अगर (unlikely (p + 1 > end)) वापस consume_all;
      अचिन्हित control = *p++;
      अचिन्हित run_गिनती = (control & मूल्य_RUN_गिनती_MASK) + 1;
      अगर (consume_all)
      {
        अगर (unlikely (!values.resize_dirty  (values.लंबाई + run_गिनती)))
	  वापस false;
      }
      अचिन्हित stop = i + run_गिनती;
      अगर (unlikely (stop > गिनती)) वापस false;

      अचिन्हित skip = i < start ? hb_min (start - i, run_गिनती) : 0;
      i += skip;

      अगर ((control & मूल्यावली_आकार_MASK) == मूल्यावली_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * माप (T));
	i = stop;
      }
      अन्यथा अगर ((control & मूल्यावली_आकार_MASK) ==  मूल्यावली_ARE_शब्दगण)
      {
        अगर (unlikely (p + run_गिनती * HBINT16::static_आकार > end)) वापस false;
	p += skip * HBINT16::static_आकार;
#अगर_अघोषित HB_OPTIMIZE_आकार
        क्रम (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (स्थिर HBINT16 *) p;
	  p += HBINT16::static_आकार;
	  values.arrayZ[i + 1] = * (स्थिर HBINT16 *) p;
	  p += HBINT16::static_आकार;
	  values.arrayZ[i + 2] = * (स्थिर HBINT16 *) p;
	  p += HBINT16::static_आकार;
	  values.arrayZ[i + 3] = * (स्थिर HBINT16 *) p;
	  p += HBINT16::static_आकार;
	}
#पूर्ण_अगर
        क्रम (; i < stop; i++)
        {
          values.arrayZ[i] = * (स्थिर HBINT16 *) p;
          p += HBINT16::static_आकार;
        }
      }
      अन्यथा अगर ((control & मूल्यावली_आकार_MASK) ==  मूल्यावली_ARE_LONGS)
      {
        अगर (unlikely (p + run_गिनती * HBINT32::static_आकार > end)) वापस false;
	p += skip * HBINT32::static_आकार;
        क्रम (; i < stop; i++)
        {
          values.arrayZ[i] = * (स्थिर HBINT32 *) p;
          p += HBINT32::static_आकार;
        }
      }
      अन्यथा अगर ((control & मूल्यावली_आकार_MASK) ==  मूल्यावली_ARE_BYTES)
      {
        अगर (unlikely (p + run_गिनती > end)) वापस false;
	p += skip * HBINT8::static_आकार;
#अगर_अघोषित HB_OPTIMIZE_आकार
	क्रम (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (स्थिर HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (स्थिर HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (स्थिर HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (स्थिर HBINT8 *) p++;
	}
#पूर्ण_अगर
        क्रम (; i < stop; i++)
          values.arrayZ[i] = * (स्थिर HBINT8 *) p++;
      }
    }
    वापस true;
  }

  अटल bool skip (स्थिर अचिन्हित अक्षर *&p /* IN/OUT */,
		    स्थिर अचिन्हित अक्षर *end,
		    अचिन्हित गिनती)
  {
    अचिन्हित seen = 0;
    जबतक (seen < गिनती)
    {
      अगर (unlikely (p >= end)) वापस false;
      अचिन्हित control = *p++;
      अचिन्हित run_गिनती = (control & मूल्य_RUN_गिनती_MASK) + 1;
      अगर (unlikely (run_गिनती > गिनती - seen)) वापस false;

      अचिन्हित width = 0;
      चयन (control & मूल्यावली_आकार_MASK)
      {
	हाल मूल्यावली_ARE_ZEROS: अवरोध;
	हाल मूल्यावली_ARE_BYTES: width = HBINT8::static_आकार; अवरोध;
	हाल मूल्यावली_ARE_शब्दगण: width = HBINT16::static_आकार; अवरोध;
	हाल मूल्यावली_ARE_LONGS: width = HBINT32::static_आकार; अवरोध;
      }
      अगर (unlikely (अचिन्हित (end - p) < run_गिनती * width)) वापस false;
      p += run_गिनती * width;
      seen += run_गिनती;
    }
    वापस true;
  }

  काष्ठा iter_t : hb_iter_with_fallback_t<iter_t, पूर्णांक>
  {
    iter_t (स्थिर अचिन्हित अक्षर *p_, अचिन्हित len_)
	    : p (p_), endp (p_ + len_)
    { अगर (likely (ensure_run ())) read_मूल्य (); }

    निजी:
    स्थिर अचिन्हित अक्षर *p;
    स्थिर अचिन्हित अक्षर * स्थिर endp;
    पूर्णांक current_मूल्य = 0;
    चिन्हित run_गिनती = 0;
    अचिन्हित width = 0;

    HB_ALWAYS_INLINE
    bool ensure_run ()
    {
      अगर (likely (run_गिनती > 0)) वापस true;
      वापस _ensure_run ();
    }
    bool _ensure_run ()
    {
      अगर (unlikely (p >= endp))
      {
        run_गिनती = 0;
        current_मूल्य = 0;
	वापस false;
      }

      अचिन्हित control = *p++;
      run_गिनती = (control & मूल्य_RUN_गिनती_MASK) + 1;
      width = control & मूल्यावली_आकार_MASK;
      चयन (width)
      {
        हाल मूल्यावली_ARE_ZEROS: width = 0; अवरोध;
	हाल मूल्यावली_ARE_BYTES: width = HBINT8::static_आकार;  अवरोध;
	हाल मूल्यावली_ARE_शब्दगण: width = HBINT16::static_आकार; अवरोध;
	हाल मूल्यावली_ARE_LONGS: width = HBINT32::static_आकार; अवरोध;
	शेष: निश्चित (false);
      }

      अगर (unlikely (p + run_गिनती * width > endp))
      {
	run_गिनती = 0;
	current_मूल्य = 0;
	वापस false;
      }

      वापस true;
    }
    व्योम read_मूल्य ()
    {
      चयन (width)
      {
        हाल 0: current_मूल्य = 0; अवरोध;
	हाल 1: current_मूल्य = * (स्थिर HBINT8  *) p; अवरोध;
	हाल 2: current_मूल्य = * (स्थिर HBINT16 *) p; अवरोध;
	हाल 4: current_मूल्य = * (स्थिर HBINT32 *) p; अवरोध;
      }
      p += width;
    }

    खुला:

    प्रकार पूर्णांक __item_t__;
    __item_t__ __item__ () स्थिर
    { वापस current_मूल्य; }

    bool __more__ () स्थिर { वापस run_गिनती || p < endp; }
    व्योम __next__ ()
    {
      run_गिनती--;
      अगर (unlikely (!ensure_run ()))
	वापस;
      read_मूल्य ();
    }
    व्योम __forward__ (अचिन्हित n)
    {
      अगर (unlikely (!ensure_run ()))
	वापस;
      जबतक (n)
      {
	अचिन्हित i = hb_min (n, (अचिन्हित) run_गिनती);
	run_गिनती -= i;
	n -= i;
	p += (i - 1) * width;
	अगर (unlikely (!ensure_run ()))
	  वापस;
	read_मूल्य ();
      }
    }
    bool चालक != (स्थिर iter_t& o) स्थिर
    { वापस p != o.p || run_गिनती != o.run_गिनती; }
    iter_t __end__ () स्थिर
    {
      iter_t it (endp, 0);
      वापस it;
    }
  };

  काष्ठा fetcher_t
  {
    fetcher_t (स्थिर अचिन्हित अक्षर *p_, अचिन्हित len_)
	      : p (p_), end (p_ + len_) {}

    निजी:
    स्थिर अचिन्हित अक्षर *p;
    स्थिर अचिन्हित अक्षर * स्थिर end;
    चिन्हित run_गिनती = 0;
    अचिन्हित width = 0;

    HB_ALWAYS_INLINE
    bool ensure_run ()
    {
      अगर (likely (run_गिनती > 0)) वापस true;
      वापस _ensure_run ();
    }

    bool _ensure_run ()
    {
      अगर (unlikely (p >= end))
      {
        run_गिनती = 0;
	वापस false;
      }

      अचिन्हित control = *p++;
      run_गिनती = (control & मूल्य_RUN_गिनती_MASK) + 1;
      width = control & मूल्यावली_आकार_MASK;
      चयन (width)
      {
        हाल मूल्यावली_ARE_ZEROS: width = 0; अवरोध;
	हाल मूल्यावली_ARE_BYTES: width = HBINT8::static_आकार;  अवरोध;
	हाल मूल्यावली_ARE_शब्दगण: width = HBINT16::static_आकार; अवरोध;
	हाल मूल्यावली_ARE_LONGS: width = HBINT32::static_आकार; अवरोध;
	शेष: निश्चित (false);
      }

      अगर (unlikely (p + run_गिनती * width > end))
      {
	run_गिनती = 0;
	वापस false;
      }

      वापस true;
    }

    खुला:
    व्योम skip (अचिन्हित n)
    {
      जबतक (n)
      {
	अगर (unlikely (!ensure_run ()))
	  वापस;
	अचिन्हित i = hb_min (n, (अचिन्हित) run_गिनती);
	run_गिनती -= i;
	n -= i;
	p += i * width;
      }
    }

    निजी:
    ढाँचा <bool scaled, typename T>
    व्योम _add_to (hb_array_t<T> out, T scale = T (1))
    {
      अचिन्हित n = out.लंबाई;
      T *arrayZ = out.arrayZ;

      क्रम (अचिन्हित i = 0; i < n;)
      {
	अगर (unlikely (!ensure_run ()))
	  अवरोध;
	अचिन्हित गिनती = hb_min (n - i, (अचिन्हित) run_गिनती);
	चयन (width)
	{
	  हाल 0:
	  {
	    arrayZ += गिनती;
	    अवरोध;
	  }
	  हाल 1:
	  {
	    स्थिर स्वतः *pp = (स्थिर HBINT8 *) p;
	    अचिन्हित j = 0;
#अगर_अघोषित HB_OPTIMIZE_आकार
	    क्रम (; j + 3 < गिनती; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#पूर्ण_अगर
	    क्रम (; j < गिनती; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (स्थिर अचिन्हित अक्षर *) pp;
	  }
	  अवरोध;
	  हाल 2:
	  {
	    स्थिर स्वतः *pp = (स्थिर HBINT16 *) p;
	    अचिन्हित j = 0;
#अगर_अघोषित HB_OPTIMIZE_आकार
	    क्रम (; j + 3 < गिनती; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#पूर्ण_अगर
	    क्रम (; j < गिनती; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (स्थिर अचिन्हित अक्षर *) pp;
	  }
	  अवरोध;
	  हाल 4:
	  {
	    स्थिर स्वतः *pp = (स्थिर HBINT32 *) p;
	    क्रम (अचिन्हित j = 0; j < गिनती; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (स्थिर अचिन्हित अक्षर *) pp;
	  }
	  अवरोध;
	}
	run_गिनती -= गिनती;
	i += गिनती;
      }
    }

    खुला:
    ढाँचा <typename T>
    व्योम add_to (hb_array_t<T> out, भग्न scale = 1.0f)
    {
#अगर_अघोषित HB_OPTIMIZE_आकार
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      अगर (false && scale == 1.0f)
        _add_to<false> (out);
      अन्यथा
#पूर्ण_अगर
        _add_to<true> (out, (T) scale);
    }
  };
};

काष्ठा TupleList : CFF2_सूचकांक
{
  Tuple_मूल्यावली::iter_t चालक [] (अचिन्हित i) स्थिर
  {
    स्वतः bytes = CFF2_सूचकांक::चालक [] (i);
    वापस Tuple_मूल्यावली::iter_t (bytes.arrayZ, bytes.लंबाई);
  }

  Tuple_मूल्यावली::fetcher_t fetcher (अचिन्हित i) स्थिर
  {
    स्वतः bytes = CFF2_सूचकांक::चालक [] (i);
    वापस Tuple_मूल्यावली::fetcher_t (bytes.arrayZ, bytes.लंबाई);
  }
};


// Alignment

ढाँचा <अचिन्हित पूर्णांक alignment>
काष्ठा Align
{
  माप_प्रकार get_आकार (स्थिर व्योम *base) स्थिर
  {
    अचिन्हित offset = (स्थिर अक्षर *) यह - (स्थिर अक्षर *) base;
    वापस (alignment - offset) & (alignment - 1);
  }

  खुला:
  DEFINE_आकार_MIN (0);
};



} /* namespace OT */


#पूर्ण_अगर /* HB_OPEN_TYPE_HH */
