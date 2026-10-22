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
	  अचिन्हित पूर्णांक Size = आकार (Type)>
संरचना अंक_Type
{
  प्रकारव्याख्या Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  प्रकारव्याख्या typename std::conditional<std::is_integral<Type>::value && आकार (Type) <= आकार(पूर्णांक),
				     typename std::conditional<std::is_signed<Type>::value, चिन्हित, अचिन्हित>::type,
				     Type>::type WideType;

  अंक_Type () = पूर्वनिर्धारित;
  explicit constexpr अंक_Type (Type V) : v {V} {}
  अंक_Type& चालक = (Type V) { v = V; परत *हे; }

  चालक WideType () स्थिरांक { परत v; }

  तार्किक चालक == (स्थिरांक अंक_Type &o) स्थिरांक { परत (Type) v == (Type) o.v; }
  तार्किक चालक != (स्थिरांक अंक_Type &o) स्थिरांक { परत !(*हे == o); }

  अंक_Type& चालक += (WideType मोजणी) { *हे = *हे + मोजणी; परत *हे; }
  अंक_Type& चालक -= (WideType मोजणी) { *हे = *हे - मोजणी; परत *हे; }
  अंक_Type& चालक ++ () { *हे += 1; परत *हे; }
  अंक_Type& चालक -- () { *हे -= 1; परत *हे; }
  अंक_Type चालक ++ (पूर्णांक) { अंक_Type c (*हे); ++*हे; परत c; }
  अंक_Type चालक -- (पूर्णांक) { अंक_Type c (*हे); --*हे; परत c; }

  uint32_t hash () स्थिरांक { परत hb_array ((स्थिरांक अक्षर *) &v, आकार (v)).hash (); }
  HB_INTERNAL स्थिर पूर्णांक cmp (स्थिरांक अंक_Type *a, स्थिरांक अंक_Type *b)
  { परत b->cmp (*a); }
  HB_INTERNAL स्थिर पूर्णांक cmp (स्थिरांक शून्य *a, स्थिरांक शून्य *b)
  {
    अंक_Type *pa = (अंक_Type *) a;
    अंक_Type *pb = (अंक_Type *) b;

    परत pb->cmp (*pa);
  }
  ढाँचा <typename Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  पूर्णांक cmp (Type2 a) स्थिरांक
  {
    Type b = v;
    परत (a > b) - (a < b);
  }
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }
  संरक्षित:
  typename std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, Size>,
			    HBFloat<BE, Type, Size>>::type v;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (Size);
};

प्रकारव्याख्या अंक_Type<सत्य, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
प्रकारव्याख्या अंक_Type<सत्य, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
प्रकारव्याख्या अंक_Type<सत्य, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
प्रकारव्याख्या अंक_Type<सत्य, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
प्रकारव्याख्या अंक_Type<सत्य, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
प्रकारव्याख्या अंक_Type<सत्य, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
प्रकारव्याख्या अंक_Type<सत्य, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
प्रकारव्याख्या अंक_Type<सत्य, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
प्रकारव्याख्या अंक_Type<सत्य, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

प्रकारव्याख्या अंक_Type<असत्य, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
प्रकारव्याख्या अंक_Type<असत्य, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
प्रकारव्याख्या अंक_Type<असत्य, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
प्रकारव्याख्या अंक_Type<असत्य, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
प्रकारव्याख्या अंक_Type<असत्य, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
प्रकारव्याख्या अंक_Type<असत्य, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

प्रकारव्याख्या अंक_Type<सत्य,  दशांश>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
प्रकारव्याख्या अंक_Type<सत्य,  द्विगुण> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
प्रकारव्याख्या अंक_Type<असत्य, दशांश>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
प्रकारव्याख्या अंक_Type<असत्य, द्विगुण> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
संरचना HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& चालक = (uint16_t i ) { HBUINT16::चालक= (i); परत *हे; }
  सार्वजनिक:
  DEFINE_SIZE_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
संरचना HBUINT32VAR
{
  माप_प्रकार get_size () स्थिरांक
  {
    अचिन्हित b0 = v[0];
    जर (b0 < 0x80)
      परत 1;
    अन्यथा जर (b0 < 0xC0)
      परत 2;
    अन्यथा जर (b0 < 0xE0)
      परत 3;
    अन्यथा जर (b0 < 0xF0)
      परत 4;
    अन्यथा
      परत 5;
  }

  स्थिर माप_प्रकार get_size (uint32_t v)
  {
    जर (v < 0x80)
      परत 1;
    अन्यथा जर (v < 0x4000)
      परत 2;
    अन्यथा जर (v < 0x200000)
      परत 3;
    अन्यथा जर (v < 0x10000000)
      परत 4;
    अन्यथा
      परत 5;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_size ()));
  }

  चालक uint32_t () स्थिरांक
  {
    अचिन्हित b0 = v[0];
    जर (b0 < 0x80)
      परत b0;
    अन्यथा जर (b0 < 0xC0)
      परत ((b0 & 0x3F) << 8) | v[1];
    अन्यथा जर (b0 < 0xE0)
      परत ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    अन्यथा जर (b0 < 0xF0)
      परत ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    अन्यथा
      परत (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  स्थिर अचिन्हित serialize_unsafe (अचिन्हित अक्षर *buf, uint32_t v)
  {
    अचिन्हित len = get_size (v);
    अचिन्हित अक्षर *p = buf + len;
    साठी (अचिन्हित i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    जर (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    परत len;
  }

  स्थिर तार्किक serialize (hb_serialize_context_t *c, uint32_t v)
  {
    अचिन्हित len = get_size (v);
    अचिन्हित अक्षर *buf = c->allocate_size<अचिन्हित अक्षर> (len, असत्य);
    जर (unlikely (!buf))
      परत असत्य;

    serialize_unsafe (buf, v);
    परत सत्य;
  }

  संरक्षित:
  अचिन्हित अक्षर v[5];

  सार्वजनिक:
  DEFINE_SIZE_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
प्रकारव्याख्या HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
प्रकारव्याख्या HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
प्रकारव्याख्या HBUINT16 UFWORD;

ढाँचा <typename Type, अचिन्हित fraction_bits>
संरचना HBFixed : Type
{
  स्थिर constexpr दशांश mult = 1.f / (1 << fraction_bits);
  static_assert (Type::static_size * 8 > fraction_bits, "");

  चालक चिन्हित () स्थिरांक = delete;
  चालक अचिन्हित () स्थिरांक = delete;
  explicit चालक दशांश () स्थिरांक { परत to_float (); }
  typename Type::type to_int () स्थिरांक { परत Type::v; }
  शून्य set_int (typename Type::type i ) { Type::v = i; }
  दशांश to_float (दशांश offset = 0) स्थिरांक  { परत ((int32_t) Type::v + offset) * mult; }
  शून्य set_float (दशांश f) { Type::v = roundf (f / mult); }
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
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }
  संरक्षित:
  HBINT32 major;
  HBUINT32 minor;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
संरचना Tag : HBUINT32
{
  Tag& चालक = (hb_tag_t i) { HBUINT32::चालक= (i); परत *हे; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  चालक स्थिरांक अक्षर* () स्थिरांक { परत reinterpret_cast<स्थिरांक अक्षर *> (हे); }
  चालक अक्षर* ()             { परत reinterpret_cast<अक्षर *> (हे); }
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
संरचना HBGlyphID16 : HBUINT16
{
  HBGlyphID16& चालक = (uint16_t i) { HBUINT16::चालक= (i); परत *हे; }
};
संरचना HBGlyphID24 : HBUINT24
{
  HBGlyphID24& चालक = (uint32_t i) { HBUINT24::चालक= (i); परत *हे; }
};

/* Script/language-system/feature index */
संरचना Index : HBUINT16 {
  स्थिर constexpr अचिन्हित NOT_FOUND_INDEX = 0xFFFFu;
  Index& चालक = (uint16_t i) { HBUINT16::चालक= (i); परत *हे; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, Index);

प्रकारव्याख्या Index NameID;

संरचना VarIdx : HBUINT32 {
  स्थिर constexpr अचिन्हित NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_INDEX, "");
  VarIdx& चालक = (uint32_t i) { HBUINT32::चालक= (i); परत *हे; }
};
DECLARE_NULL_NAMESPACE_BYTES (OT, VarIdx);

/* Offset, Null offset = 0 */
ढाँचा <typename Type, तार्किक has_null=सत्य>
संरचना Offset : Type
{
  Offset& चालक = (typename Type::type i) { Type::चालक= (i); परत *हे; }

  प्रकारव्याख्या Type type;

  तार्किक is_null () स्थिरांक { परत has_null && 0 == *हे; }

  सार्वजनिक:
  DEFINE_SIZE_STATIC (आकार (Type));
};

प्रकारव्याख्या Offset<HBUINT16> Offset16;
प्रकारव्याख्या Offset<HBUINT24> Offset24;
प्रकारव्याख्या Offset<HBUINT32> Offset32;


/* CheckSum */
संरचना CheckSum : HBUINT32
{
  CheckSum& चालक = (uint32_t i) { HBUINT32::चालक= (i); परत *हे; }

  /* This is reference implementation from the spec. */
  स्थिर uint32_t CalcTableChecksum (स्थिरांक HBUINT32 *Table, uint32_t Length)
  {
    uint32_t Sum = 0L;
    प्रतिज्ञा (0 == (Length & 3));
    स्थिरांक HBUINT32 *EndPtr = Table + Length / HBUINT32::static_size;

    पर्यंत (Table < EndPtr)
      Sum += *Table++;
    परत Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  शून्य set_for_data (स्थिरांक शून्य *data, अचिन्हित पूर्णांक length)
  { *हे = CalcTableChecksum ((स्थिरांक HBUINT32 *) data, length); }

  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};


/*
 * Version Numbers
 */

ढाँचा <typename FixedType=HBUINT16>
संरचना FixedVersion
{
  uint32_t to_int () स्थिरांक { परत (major << (आकार (FixedType) * 8)) + minor; }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  FixedType major;
  FixedType minor;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (2 * आकार (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

ढाँचा <typename Type, तार्किक has_null>
संरचना _hb_has_null
{
  स्थिर स्थिरांक Type *get_null () { परत nullptr; }
  स्थिर Type *get_crap ()       { परत nullptr; }
};
ढाँचा <typename Type>
संरचना _hb_has_null<Type, सत्य>
{
  स्थिर स्थिरांक Type *get_null () { परत &Null (Type); }
  स्थिर       Type *get_crap () { परत &Crap (Type); }
};

ढाँचा <typename Type, typename OffsetType, typename BaseType=शून्य, तार्किक has_null=सत्य>
संरचना OffsetTo : Offset<OffsetType, has_null>
{
  using target_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == असत्य ||
		 (hb_has_null_size (Type) || !hb_has_min_size (Type)), "");

  HB_DELETE_COPY_ASSIGN (OffsetTo);
  OffsetTo () = पूर्वनिर्धारित;

  OffsetTo& चालक = (typename OffsetType::type i) { OffsetType::चालक= (i); परत *हे; }

  स्थिरांक Type& चालक () (स्थिरांक शून्य *base) स्थिरांक
  {
    जर (unlikely (हे->is_null ())) परत *_hb_has_null<Type, has_null>::get_null ();
    परत StructAtOffset<स्थिरांक Type> (base, *हे);
  }
  Type& चालक () (शून्य *base) स्थिरांक
  {
    जर (unlikely (हे->is_null ())) परत *_hb_has_null<Type, has_null>::get_crap ();
    परत StructAtOffset<Type> (base, *हे);
  }

  ढाँचा <typename Base,
	    hb_enable_if (hb_is_convertible (स्थिरांक Base, स्थिरांक BaseType *))>
  मित्र स्थिरांक Type& चालक + (स्थिरांक Base &base, स्थिरांक OffsetTo &offset) { परत offset ((स्थिरांक शून्य *) base); }
  ढाँचा <typename Base,
	    hb_enable_if (hb_is_convertible (स्थिरांक Base, स्थिरांक BaseType *))>
  मित्र स्थिरांक Type& चालक + (स्थिरांक OffsetTo &offset, स्थिरांक Base &base) { परत offset ((स्थिरांक शून्य *) base); }
  ढाँचा <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  मित्र Type& चालक + (Base &&base, OffsetTo &offset) { परत offset ((शून्य *) base); }
  ढाँचा <typename Base,
	    hb_enable_if (hb_is_convertible (Base, BaseType *))>
  मित्र Type& चालक + (OffsetTo &offset, Base &&base) { परत offset ((शून्य *) base); }


  ढाँचा <typename Base, typename ...Ts>
  तार्किक serialize_subset (hb_subset_context_t *c, स्थिरांक OffsetTo& src,
			 स्थिरांक Base *src_base, Ts&&... ds)
  {
    *हे = 0;
    जर (src.is_null ())
      परत असत्य;

    स्वतः *s = c->serializer;

    s->push ();

    तार्किक ret = c->dispatch (src_base+src, std::forward<Ts> (ds)...);

    जर (ret || !has_null)
      s->add_link (*हे, s->काढा_pack ());
    अन्यथा
      s->काढा_discard ();

    परत ret;
  }


  ढाँचा <typename ...Ts>
  तार्किक serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *हे = 0;

    Type* obj = c->push<Type> ();
    तार्किक ret = obj->serialize (c, std::forward<Ts> (ds)...);

    जर (ret)
      c->add_link (*हे, c->काढा_pack ());
    अन्यथा
      c->काढा_discard ();

    परत ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  ढाँचा <typename ...Ts>
  तार्किक serialize_copy (hb_serialize_context_t *c, स्थिरांक OffsetTo& src,
		       स्थिरांक शून्य *src_base, अचिन्हित dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *हे = 0;
    जर (src.is_null ())
      परत असत्य;

    c->push ();

    तार्किक ret = c->copy (src_base+src, std::forward<Ts> (ds)...);

    c->add_link (*हे, c->काढा_pack (), whence, dst_bias);

    परत ret;
  }

  तार्किक serialize_copy (hb_serialize_context_t *c, स्थिरांक OffsetTo& src,
		       स्थिरांक शून्य *src_base, अचिन्हित dst_bias = 0)
  { परत serialize_copy (c, src, src_base, dst_bias, hb_serialize_context_t::Head); }

  तार्किक sanitize_shallow (hb_sanitize_context_t *c, स्थिरांक BaseType *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!c->check_struct (हे))) return_trace (असत्य);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    जर (unlikely ((स्थिरांक अक्षर *) base + (अचिन्हित) *हे < (स्थिरांक अक्षर *) base)) return_trace (असत्य);
    return_trace (सत्य);
  }

  ढाँचा <typename ...Ts>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक BaseType *base, Ts&&... ds) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (sanitize_shallow (c, base) &&
		  hb_barrier () &&
		  (हे->is_null () ||
		   c->dispatch (StructAtOffset<Type> (base, *हे), std::forward<Ts> (ds)...)));
  }

  DEFINE_SIZE_STATIC (आकार (OffsetType));
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
  प्रकारव्याख्या Type item_t;
  स्थिर constexpr अचिन्हित item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (UnsizedArrayOf);

  स्थिरांक Type& चालक [] (अचिन्हित पूर्णांक i) स्थिरांक
  {
    परत arrayZ[i];
  }
  Type& चालक [] (अचिन्हित पूर्णांक i)
  {
    परत arrayZ[i];
  }

  स्थिर माप_प्रकार get_size (अचिन्हित पूर्णांक len)
  { परत hb_unsigned_mul_saturate (len, Type::static_size); }

  ढाँचा <typename T> चालक T * () { परत arrayZ; }
  ढाँचा <typename T> चालक स्थिरांक T * () स्थिरांक { परत arrayZ; }
  hb_array_t<Type> as_array (अचिन्हित पूर्णांक len)
  { परत hb_array (arrayZ, len); }
  hb_array_t<स्थिरांक Type> as_array (अचिन्हित पूर्णांक len) स्थिरांक
  { परत hb_array (arrayZ, len); }

  ढाँचा <typename T>
  Type &lsearch (अचिन्हित पूर्णांक len, स्थिरांक T &x, Type &not_found = Crap (Type))
  { परत *as_array (len).lsearch (x, &not_found); }
  ढाँचा <typename T>
  स्थिरांक Type &lsearch (अचिन्हित पूर्णांक len, स्थिरांक T &x, स्थिरांक Type &not_found = Null (Type)) स्थिरांक
  { परत *as_array (len).lsearch (x, &not_found); }
  ढाँचा <typename T>
  तार्किक lfind (अचिन्हित पूर्णांक len, स्थिरांक T &x, अचिन्हित पूर्णांक *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णांक to_store = (अचिन्हित पूर्णांक) -1) स्थिरांक
  { परत as_array (len).lfind (x, i, not_found, to_store); }

  शून्य क्विक (अचिन्हित पूर्णांक len, अचिन्हित पूर्णांक start = 0, अचिन्हित पूर्णांक end = (अचिन्हित पूर्णांक) -1)
  { as_array (len).क्विक (start, end); }

  तार्किक serialize (hb_serialize_context_t *c, अचिन्हित पूर्णांक वस्तुसंच_len, तार्किक clear = सत्य)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_size (हे, get_size (वस्तुसंच_len), clear))) return_trace (असत्य);
    return_trace (सत्य);
  }
  ढाँचा <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  तार्किक serialize (hb_serialize_context_t *c, Iterator वस्तुसंच)
  {
    TRACE_SERIALIZE (हे);
    अचिन्हित मोजणी = hb_len (वस्तुसंच);
    जर (unlikely (!serialize (c, मोजणी, असत्य))) return_trace (असत्य);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    साठी (अचिन्हित i = 0; i < मोजणी; i++, ++वस्तुसंच)
      arrayZ[i] = *वस्तुसंच;
    return_trace (सत्य);
  }

  UnsizedArrayOf* copy (hb_serialize_context_t *c, अचिन्हित मोजणी) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    स्वतः *out = c->start_embed (हे);
    जर (unlikely (!as_array (मोजणी).copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  ढाँचा <typename ...Ts>
  HB_ALWAYS_INLINE
  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक मोजणी, Ts&&... ds) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!sanitize_shallow (c, मोजणी))) return_trace (असत्य);
    जर (!आकार... (Ts) && hb_is_trivially_copyable(Type)) return_trace (सत्य);
    hb_barrier ();
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (असत्य);
    return_trace (सत्य);
  }

  तार्किक sanitize_shallow (hb_sanitize_context_t *c, अचिन्हित पूर्णांक मोजणी) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_array (arrayZ, मोजणी));
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
  स्थिरांक Type& चालक [] (पूर्णांक i_) स्थिरांक
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    स्थिरांक OffsetTo<Type, OffsetType, BaseType, has_null> *p = &हे->arrayZ[i];
    जर (unlikely ((स्थिरांक शून्य *) p < (स्थिरांक शून्य *) हे->arrayZ)) परत Null (Type); /* Overflowed. */
    hb_barrier ();
    परत हे+*p;
  }
  Type& चालक [] (पूर्णांक i_)
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    स्थिरांक OffsetTo<Type, OffsetType, BaseType, has_null> *p = &हे->arrayZ[i];
    जर (unlikely ((स्थिरांक शून्य *) p < (स्थिरांक शून्य *) हे->arrayZ)) परत Crap (Type); /* Overflowed. */
    hb_barrier ();
    परत हे+*p;
  }

  ढाँचा <typename ...Ts>
  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक मोजणी, Ts&&... ds) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace ((UnsizedArray16OfOffsetTo<Type, OffsetType, BaseType, has_null>
		   ::sanitize (c, मोजणी, हे, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
ढाँचा <typename Type>
संरचना SortedUnsizedArrayOf : UnsizedArrayOf<Type>
{
  hb_sorted_array_t<Type> as_array (अचिन्हित पूर्णांक len)
  { परत hb_sorted_array (हे->arrayZ, len); }
  hb_sorted_array_t<स्थिरांक Type> as_array (अचिन्हित पूर्णांक len) स्थिरांक
  { परत hb_sorted_array (हे->arrayZ, len); }
  चालक hb_sorted_array_t<Type> ()             { परत as_array (); }
  चालक hb_sorted_array_t<स्थिरांक Type> () स्थिरांक { परत as_array (); }

  ढाँचा <typename T>
  Type &द्वा_खोज (अचिन्हित पूर्णांक len, स्थिरांक T &x, Type &not_found = Crap (Type))
  { परत *as_array (len).द्वा_खोज (x, &not_found); }
  ढाँचा <typename T>
  स्थिरांक Type &द्वा_खोज (अचिन्हित पूर्णांक len, स्थिरांक T &x, स्थिरांक Type &not_found = Null (Type)) स्थिरांक
  { परत *as_array (len).द्वा_खोज (x, &not_found); }
  ढाँचा <typename T>
  तार्किक bfind (अचिन्हित पूर्णांक len, स्थिरांक T &x, अचिन्हित पूर्णांक *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णांक to_store = (अचिन्हित पूर्णांक) -1) स्थिरांक
  { परत as_array (len).bfind (x, i, not_found, to_store); }
};


/* An array with a number of elements. */
ढाँचा <typename Type, typename LenType>
संरचना ArrayOf
{
  प्रकारव्याख्या Type item_t;
  स्थिर constexpr अचिन्हित item_size = hb_static_size (Type);

  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOf);

  स्थिरांक Type& चालक [] (पूर्णांक i_) स्थिरांक
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    जर (unlikely (i >= len)) परत Null (Type);
    hb_barrier ();
    परत arrayZ[i];
  }
  Type& चालक [] (पूर्णांक i_)
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    जर (unlikely (i >= len)) परत Crap (Type);
    hb_barrier ();
    परत arrayZ[i];
  }

  माप_प्रकार get_size () स्थिरांक
  { परत hb_unsigned_mul_add_saturate (len, Type::static_size, len.static_size); }

  explicit चालक तार्किक () स्थिरांक { परत len; }

  शून्य काढा () { len--; }

  hb_array_t<      Type> as_array ()       { परत hb_array (arrayZ, len); }
  hb_array_t<स्थिरांक Type> as_array () स्थिरांक { परत hb_array (arrayZ, len); }

  /* Iterator. */
  प्रकारव्याख्या hb_array_t<स्थिरांक Type>   iter_t;
  प्रकारव्याख्या hb_array_t<      Type> writer_t;
    iter_t   iter () स्थिरांक { परत as_array (); }
  writer_t writer ()       { परत as_array (); }
  चालक   iter_t () स्थिरांक { परत   iter (); }
  चालक writer_t ()       { परत writer (); }

  /* Faster range-based for loop. */
  स्थिरांक Type *begin () स्थिरांक { परत arrayZ; }
  स्थिरांक Type *end () स्थिरांक { परत arrayZ + len; }

  ढाँचा <typename T>
  Type &lsearch (स्थिरांक T &x, Type &not_found = Crap (Type))
  { परत *as_array ().lsearch (x, &not_found); }
  ढाँचा <typename T>
  स्थिरांक Type &lsearch (स्थिरांक T &x, स्थिरांक Type &not_found = Null (Type)) स्थिरांक
  { परत *as_array ().lsearch (x, &not_found); }
  ढाँचा <typename T>
  तार्किक lfind (स्थिरांक T &x, अचिन्हित पूर्णांक *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णांक to_store = (अचिन्हित पूर्णांक) -1) स्थिरांक
  { परत as_array ().lfind (x, i, not_found, to_store); }

  शून्य क्विक ()
  { as_array ().क्विक (); }

  HB_NODISCARD तार्किक serialize (hb_serialize_context_t *c, अचिन्हित वस्तुसंच_len, तार्किक clear = सत्य)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);
    c->check_assign (len, वस्तुसंच_len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    जर (unlikely (!c->extend_size (हे, get_size (), clear))) return_trace (असत्य);
    return_trace (सत्य);
  }
  ढाँचा <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD तार्किक serialize (hb_serialize_context_t *c, Iterator वस्तुसंच)
  {
    TRACE_SERIALIZE (हे);
    अचिन्हित मोजणी = hb_len (वस्तुसंच);
    जर (unlikely (!serialize (c, मोजणी, असत्य))) return_trace (असत्य);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    साठी (अचिन्हित i = 0; i < मोजणी; i++, ++वस्तुसंच)
      arrayZ[i] = *वस्तुसंच;
    return_trace (सत्य);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (हे);
    len++;
    जर (unlikely (!len || !c->extend (हे)))
    {
      len--;
      return_trace (nullptr);
    }
    return_trace (&arrayZ[len - 1]);
  }

  ArrayOf* copy (hb_serialize_context_t *c) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    स्वतः *out = c->start_embed (हे);
    जर (unlikely (!c->extend_min (out))) return_trace (nullptr);
    c->check_assign (out->len, len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    जर (unlikely (!as_array ().copy (c))) return_trace (nullptr);
    return_trace (out);
  }

  ढाँचा <typename ...Ts>
  HB_ALWAYS_INLINE
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!sanitize_shallow (c))) return_trace (असत्य);
    जर (!आकार... (Ts) && hb_is_trivially_copyable(Type)) return_trace (सत्य);
    hb_barrier ();
    अचिन्हित पूर्णांक मोजणी = len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (असत्य);
    return_trace (सत्य);
  }

  तार्किक sanitize_shallow (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (len.sanitize (c) &&
		  hb_barrier () &&
		  c->check_array_sized (arrayZ, len, आकार (LenType)));
  }

  सार्वजनिक:
  LenType	len;
  Type		arrayZ[HB_VAR_ARRAY];
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (आकार (LenType), arrayZ);
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
  स्थिरांक Type& चालक [] (पूर्णांक i_) स्थिरांक
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    जर (unlikely (i >= हे->len)) परत Null (Type);
    hb_barrier ();
    परत हे+हे->arrayZ[i];
  }
  स्थिरांक Type& चालक [] (पूर्णांक i_)
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    जर (unlikely (i >= हे->len)) परत Crap (Type);
    hb_barrier ();
    परत हे+हे->arrayZ[i];
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    संरचना List16OfOffsetTo *out = c->serializer->embed (*हे);
    जर (unlikely (!out)) return_trace (असत्य);
    अचिन्हित पूर्णांक मोजणी = हे->len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      out->arrayZ[i].serialize_subset (c, हे->arrayZ[i], हे, out);
    return_trace (सत्य);
  }

  ढाँचा <typename ...Ts>
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace ((Array16Of<OffsetTo<Type, OffsetType>>::sanitize (c, हे, std::forward<Ts> (ds)...)));
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

  स्थिरांक Type& चालक [] (पूर्णांक i_) स्थिरांक
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    जर (unlikely (i >= lenP1 || !i)) परत Null (Type);
    hb_barrier ();
    परत arrayZ[i-1];
  }
  Type& चालक [] (पूर्णांक i_)
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    जर (unlikely (i >= lenP1 || !i)) परत Crap (Type);
    hb_barrier ();
    परत arrayZ[i-1];
  }
  माप_प्रकार get_size () स्थिरांक
  { परत hb_unsigned_mul_add_saturate (get_length (), Type::static_size, lenP1.static_size); }

  अचिन्हित get_length () स्थिरांक { परत lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { परत hb_array (arrayZ, get_length ()); }
  hb_array_t<स्थिरांक Type> as_array () स्थिरांक { परत hb_array (arrayZ, get_length ()); }

  /* Iterator. */
  प्रकारव्याख्या hb_array_t<स्थिरांक Type>   iter_t;
  प्रकारव्याख्या hb_array_t<      Type> writer_t;
    iter_t   iter () स्थिरांक { परत as_array (); }
  writer_t writer ()       { परत as_array (); }
  चालक   iter_t () स्थिरांक { परत   iter (); }
  चालक writer_t ()       { परत writer (); }

  /* Faster range-based for loop. */
  स्थिरांक Type *begin () स्थिरांक { परत arrayZ; }
  स्थिरांक Type *end () स्थिरांक { परत arrayZ + get_length (); }

  HB_NODISCARD तार्किक serialize (hb_serialize_context_t *c, अचिन्हित पूर्णांक वस्तुसंच_len, तार्किक clear = सत्य)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);
    c->check_assign (lenP1, वस्तुसंच_len + 1, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW);
    जर (unlikely (!c->extend_size (हे, get_size (), clear))) return_trace (असत्य);
    return_trace (सत्य);
  }
  ढाँचा <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, Type))>
  HB_NODISCARD तार्किक serialize (hb_serialize_context_t *c, Iterator वस्तुसंच)
  {
    TRACE_SERIALIZE (हे);
    अचिन्हित मोजणी = hb_len (वस्तुसंच);
    जर (unlikely (!serialize (c, मोजणी, असत्य))) return_trace (असत्य);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    साठी (अचिन्हित i = 0; i < मोजणी; i++, ++वस्तुसंच)
      arrayZ[i] = *वस्तुसंच;
    return_trace (सत्य);
  }

  ढाँचा <typename ...Ts>
  HB_ALWAYS_INLINE
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!sanitize_shallow (c))) return_trace (असत्य);
    जर (!आकार... (Ts) && hb_is_trivially_copyable(Type)) return_trace (सत्य);
    hb_barrier ();
    अचिन्हित पूर्णांक मोजणी = get_length ();
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (असत्य);
    return_trace (सत्य);
  }

  खाजगी:
  तार्किक sanitize_shallow (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (lenP1.sanitize (c) &&
		  hb_barrier () &&
		  (!lenP1 || c->check_array_sized (arrayZ, lenP1 - 1, आकार (LenType))));
  }

  सार्वजनिक:
  LenType	lenP1;
  Type		arrayZ[HB_VAR_ARRAY];
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (आकार (LenType), arrayZ);
};
ढाँचा <typename Type> using HeadlessArray16Of = HeadlessArrayOf<Type, HBUINT16>;

/* An array storing length-1. */
ढाँचा <typename Type, typename LenType=HBUINT16>
संरचना ArrayOfM1
{
  HB_DELETE_CREATE_COPY_ASSIGN (ArrayOfM1);

  स्थिरांक Type& चालक [] (पूर्णांक i_) स्थिरांक
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    जर (unlikely (i > lenM1)) परत Null (Type);
    hb_barrier ();
    परत arrayZ[i];
  }
  Type& चालक [] (पूर्णांक i_)
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    जर (unlikely (i > lenM1)) परत Crap (Type);
    hb_barrier ();
    परत arrayZ[i];
  }
  माप_प्रकार get_size () स्थिरांक
  { परत hb_unsigned_mul_add_saturate (lenM1 + 1, Type::static_size, lenM1.static_size); }

  ढाँचा <typename ...Ts>
  HB_ALWAYS_INLINE
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!sanitize_shallow (c))) return_trace (असत्य);
    जर (!आकार... (Ts) && hb_is_trivially_copyable(Type)) return_trace (सत्य);
    hb_barrier ();
    अचिन्हित पूर्णांक मोजणी = lenM1 + 1;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (असत्य);
    return_trace (सत्य);
  }

  खाजगी:
  तार्किक sanitize_shallow (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (lenM1.sanitize (c) &&
		  hb_barrier () &&
		  (c->check_array_sized (arrayZ, lenM1 + 1, आकार (LenType))));
  }

  सार्वजनिक:
  LenType	lenM1;
  Type		arrayZ[HB_VAR_ARRAY];
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (आकार (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
ढाँचा <typename Type, typename LenType>
संरचना SortedArrayOf : ArrayOf<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { परत hb_sorted_array (हे->arrayZ, हे->len); }
  hb_sorted_array_t<स्थिरांक Type> as_array () स्थिरांक { परत hb_sorted_array (हे->arrayZ, हे->len); }

  /* Iterator. */
  प्रकारव्याख्या hb_sorted_array_t<स्थिरांक Type>   iter_t;
  प्रकारव्याख्या hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () स्थिरांक { परत as_array (); }
  writer_t writer ()       { परत as_array (); }
  चालक   iter_t () स्थिरांक { परत   iter (); }
  चालक writer_t ()       { परत writer (); }

  /* Faster range-based for loop. */
  स्थिरांक Type *begin () स्थिरांक { परत हे->arrayZ; }
  स्थिरांक Type *end () स्थिरांक { परत हे->arrayZ + हे->len; }

  तार्किक serialize (hb_serialize_context_t *c, अचिन्हित पूर्णांक वस्तुसंच_len)
  {
    TRACE_SERIALIZE (हे);
    तार्किक ret = ArrayOf<Type, LenType>::serialize (c, वस्तुसंच_len);
    return_trace (ret);
  }
  ढाँचा <typename Iterator,
	    hb_requires (hb_is_sorted_source_of (Iterator, Type))>
  तार्किक serialize (hb_serialize_context_t *c, Iterator वस्तुसंच)
  {
    TRACE_SERIALIZE (हे);
    तार्किक ret = ArrayOf<Type, LenType>::serialize (c, वस्तुसंच);
    return_trace (ret);
  }

  SortedArrayOf* copy (hb_serialize_context_t *c) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    SortedArrayOf* out = reinterpret_cast<SortedArrayOf *> (ArrayOf<Type, LenType>::copy (c));
    return_trace (out);
  }

  ढाँचा <typename T>
  Type &द्वा_खोज (स्थिरांक T &x, Type &not_found = Crap (Type))
  { परत *as_array ().द्वा_खोज (x, &not_found); }
  ढाँचा <typename T>
  स्थिरांक Type &द्वा_खोज (स्थिरांक T &x, स्थिरांक Type &not_found = Null (Type)) स्थिरांक
  { परत *as_array ().द्वा_खोज (x, &not_found); }
  ढाँचा <typename T>
  तार्किक bfind (स्थिरांक T &x, अचिन्हित पूर्णांक *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णांक to_store = (अचिन्हित पूर्णांक) -1) स्थिरांक
  { परत as_array ().bfind (x, i, not_found, to_store); }
};

ढाँचा <typename Type> using SortedArray16Of = SortedArrayOf<Type, HBUINT16>;
ढाँचा <typename Type> using SortedArray24Of = SortedArrayOf<Type, HBUINT24>;
ढाँचा <typename Type> using SortedArray32Of = SortedArrayOf<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

ढाँचा <typename LenType=HBUINT16>
संरचना Bin_शोध_Header
{
  चालक uint32_t () स्थिरांक { परत len; }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  Bin_शोध_Header& चालक = (अचिन्हित पूर्णांक v)
  {
    len = v;
    प्रतिज्ञा (len == v);
    entrySelector = hb_max (1u, hb_bit_storage (v)) - 1;
    शोध_Range = 16 * (1u << entrySelector);
    rangeShift = v * 16 > शोध_Range
		 ? 16 * v - शोध_Range
		 : 0;
    परत *हे;
  }

  संरक्षित:
  LenType	len;
  LenType	शोध_Range;
  LenType	entrySelector;
  LenType	rangeShift;

  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

ढाँचा <typename Type, typename LenType=HBUINT16>
using Bin_शोध_Array_Of = SortedArrayOf<Type, Bin_शोध_Header<LenType>>;


संरचना Var_Sized_Bin_शोध_Header
{

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  HBUINT16	unitSize;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	शोध_Range;	/* The value of unitSize times the largest power of 2
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
संरचना Var_Sized_Bin_शोध_Array_Of
{
  स्थिर constexpr अचिन्हित item_size = Type::static_size;

  HB_DELETE_CREATE_COPY_ASSIGN (Var_Sized_Bin_शोध_Array_Of);

  तार्किक last_is_terminator () स्थिरांक
  {
    जर (unlikely (!header.nUnits)) परत असत्य;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    स्थिरांक HBUINT16 *शब्द = &StructAtOffset<HBUINT16> (&bytesZ, (header.nUnits - 1) * header.unitSize);
    अचिन्हित पूर्णांक मोजणी = Type::Termination_Word_मोजणी;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (शब्द[i] != 0xFFFFu)
	परत असत्य;
    परत सत्य;
  }

  स्थिरांक Type& चालक [] (पूर्णांक i_) स्थिरांक
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    जर (unlikely (i >= get_length ())) परत Null (Type);
    hb_barrier ();
    परत StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  Type& चालक [] (पूर्णांक i_)
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    जर (unlikely (i >= get_length ())) परत Crap (Type);
    hb_barrier ();
    परत StructAtOffset<Type> (&bytesZ, i * header.unitSize);
  }
  अचिन्हित पूर्णांक get_length () स्थिरांक
  { परत header.nUnits - last_is_terminator (); }
  माप_प्रकार get_size () स्थिरांक
  { परत hb_unsigned_mul_add_saturate (header.nUnits, header.unitSize, header.static_size); }

  ढाँचा <typename ...Ts>
  HB_ALWAYS_INLINE
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!sanitize_shallow (c))) return_trace (असत्य);
    जर (!आकार... (Ts) && hb_is_trivially_copyable(Type)) return_trace (सत्य);
    hb_barrier ();
    अचिन्हित पूर्णांक मोजणी = get_length ();
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (unlikely (!(*हे)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (असत्य);
    return_trace (सत्य);
  }

  ढाँचा <typename T>
  स्थिरांक Type *द्वा_खोज (स्थिरांक T &key) स्थिरांक
  {
    अचिन्हित pos;
    परत hb_bsearch_impl (&pos,
			    key,
			    (स्थिरांक शून्य *) bytesZ,
			    get_length (),
			    header.unitSize,
			    _hb_cmp_method<T, Type>)
	   ? (स्थिरांक Type *) (((स्थिरांक अक्षर *) &bytesZ) + (pos * header.unitSize))
	   : nullptr;
  }

  खाजगी:
  तार्किक sanitize_shallow (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (header.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_size <= header.unitSize &&
		  c->check_range (bytesZ.arrayZ,
				  header.nUnits,
				  header.unitSize));
  }

  संरक्षित:
  Var_Sized_Bin_शोध_Header	header;
  UnsizedArrayOf<HBUINT8>	bytesZ;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (10, bytesZ);
};


/* CFF INDEX */

ढाँचा <typename मोजणी_3>
संरचना CFFIndex
{
  अचिन्हित पूर्णांक offset_array_size () स्थिरांक
  { परत offSize * (मोजणी + 1); }

  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक Iterable &iterable,
		  स्थिरांक अचिन्हित *p_data_size = nullptr,
                  अचिन्हित min_off_size = 0)
  {
    TRACE_SERIALIZE (हे);
    अचिन्हित data_size;
    जर (p_data_size)
      data_size = *p_data_size;
    अन्यथा
      total_size (iterable, &data_size);

    स्वतः it = hb_iter (iterable);
    जर (unlikely (!serialize_header (c, +it, data_size, min_off_size))) return_trace (असत्य);
    अचिन्हित अक्षर *ret = c->allocate_size<अचिन्हित अक्षर> (data_size, असत्य);
    जर (unlikely (!ret)) return_trace (असत्य);
    अचिन्हित remaining = data_size;
    साठी (स्थिरांक स्वतः &_ : +it)
    {
      अचिन्हित len = _.length;

      जर (!len)
	सुरूठेवा;

      जर (unlikely (len > remaining)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (असत्य, HB_SERIALIZE_ERROR_INT_OVERFLOW));
      }

      remaining -= len;

      जर (len <= 1)
      {
	*ret++ = *_.arrayZ;
	सुरूठेवा;
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
    TRACE_SERIALIZE (हे);

    अचिन्हित off_size = (hb_bit_storage (data_size + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    /* serialize CFFIndex header */
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);
    हे->मोजणी = hb_len (it);
    जर (!हे->मोजणी) return_trace (सत्य);
    जर (unlikely (!c->extend (हे->offSize))) return_trace (असत्य);
    हे->offSize = off_size;
    जर (unlikely (!c->allocate_size<HBUINT8> (off_size * (हे->मोजणी + 1), असत्य)))
      return_trace (असत्य);

    /* serialize indices */
    अचिन्हित पूर्णांक offset = 1;
    जर (HB_OPTIMIZE_SIZE_VAL)
    {
      अचिन्हित पूर्णांक i = 0;
      साठी (स्थिरांक स्वतः &_ : +it)
      {
	set_offset_at (i++, offset);
	offset += hb_len_of (_);
      }
      set_offset_at (i, offset);
    }
    अन्यथा
      निवड (off_size)
      {
	स्थिती 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  साठी (स्थिरांक स्वतः &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	थांबा;
	स्थिती 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  साठी (स्थिरांक स्वतः &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	थांबा;
	स्थिती 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  साठी (स्थिरांक स्वतः &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	थांबा;
	स्थिती 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  साठी (स्थिरांक स्वतः &_ : +it)
	  {
	    *p++ = offset;
	    offset += hb_len_of (_);
	  }
	  *p = offset;
	}
	थांबा;
	पूर्वनिर्धारित:
	थांबा;
      }

    प्रतिज्ञा (offset == data_size + 1);
    return_trace (सत्य);
  }

  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  स्थिर अचिन्हित total_size (स्थिरांक Iterable &iterable, अचिन्हित *data_size = nullptr, अचिन्हित min_off_size = 0)
  {
    स्वतः it = + hb_iter (iterable);
    जर (!it)
    {
      जर (data_size) *data_size = 0;
      परत min_size;
    }

    अचिन्हित total = 0;
    साठी (स्थिरांक स्वतः &_ : +it)
      total += hb_len_of (_);

    जर (data_size) *data_size = total;

    अचिन्हित off_size = (hb_bit_storage (total + 1) + 7) / 8;
    off_size = hb_max(min_off_size, off_size);

    परत min_size + HBUINT8::static_size + (hb_len (it) + 1) * off_size + total;
  }

  शून्य set_offset_at (अचिन्हित पूर्णांक index, अचिन्हित पूर्णांक offset)
  {
    प्रतिज्ञा (index <= मोजणी);

    अचिन्हित पूर्णांक size = offSize;
    स्थिरांक HBUINT8 *p = offsets;
    निवड (size)
    {
      स्थिती 1: ((HBUINT8  *) p)[index] = offset; थांबा;
      स्थिती 2: ((HBUINT16 *) p)[index] = offset; थांबा;
      स्थिती 3: ((HBUINT24 *) p)[index] = offset; थांबा;
      स्थिती 4: ((HBUINT32 *) p)[index] = offset; थांबा;
      पूर्वनिर्धारित: परत;
    }
  }

  खाजगी:
  अचिन्हित पूर्णांक offset_at (अचिन्हित पूर्णांक index) स्थिरांक
  {
    प्रतिज्ञा (index <= मोजणी);

    अचिन्हित पूर्णांक size = offSize;
    स्थिरांक HBUINT8 *p = offsets;
    निवड (size)
    {
      स्थिती 1: परत ((HBUINT8  *) p)[index];
      स्थिती 2: परत ((HBUINT16 *) p)[index];
      स्थिती 3: परत ((HBUINT24 *) p)[index];
      स्थिती 4: परत ((HBUINT32 *) p)[index];
      पूर्वनिर्धारित: परत 0;
    }
  }

  स्थिरांक अचिन्हित अक्षर *data_base () स्थिरांक
  { परत (स्थिरांक अचिन्हित अक्षर *) हे + min_size + offSize.static_size - 1 + offset_array_size (); }
  सार्वजनिक:

  hb_ubytes_t चालक [] (अचिन्हित पूर्णांक index) स्थिरांक
  {
    जर (unlikely (index >= मोजणी)) परत hb_ubytes_t ();
    hb_barrier ();
    अचिन्हित offset0 = offset_at (index);
    अचिन्हित offset1 = offset_at (index + 1);
    जर (unlikely (offset1 < offset0 || offset1 > offset_at (मोजणी)))
      परत hb_ubytes_t ();
    परत hb_ubytes_t (data_base () + offset0, offset1 - offset0);
  }

  माप_प्रकार get_size () स्थिरांक
  {
    जर (मोजणी)
      परत hb_unsigned_add_saturate (min_size, offSize.static_size,
				       offset_array_size (), offset_at (मोजणी) - 1);
    परत min_size;  /* empty CFFIndex contains count only */
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  hb_barrier () &&
			  (मोजणी == 0 || /* empty INDEX */
			   (मोजणी < मोजणी + 1u &&
			    c->check_struct (&offSize) && offSize >= 1 && offSize <= 4 &&
			    c->check_array (offsets, offSize, मोजणी + 1u) &&
			    c->check_range (data_base (), offset_at (मोजणी))))));
  }

  सार्वजनिक:
  मोजणी_3		मोजणी;		/* Number of object data. Note there are (count+1) offsets */
  खाजगी:
  HBUINT8	offSize;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  सार्वजनिक:
  DEFINE_SIZE_MIN (मोजणी_3::static_size);
};
प्रकारव्याख्या CFFIndex<HBUINT16> CFF1Index;
प्रकारव्याख्या CFFIndex<HBUINT32> CFF2Index;


/* TupleValues */
संरचना Tuple_मूल्ये
{
  गणना packed_मूल्य_flag_t
  {
    मूल्ये_ARE_ZEROS     = 0x80,
    मूल्ये_ARE_BYTES     = 0x00,
    मूल्ये_ARE_शब्द     = 0x40,
    मूल्ये_ARE_LONGS     = 0xC0,
    मूल्ये_SIZE_MASK     = 0xC0,
    मूल्य_RUN_मोजणी_MASK = 0x3F
  };

  स्थिर अचिन्हित compile_unsafe (hb_array_t<स्थिरांक पूर्णांक> values, /* IN */
				  अचिन्हित अक्षर *encoded_bytes /* OUT */)
  {
    अचिन्हित अंक_मूल्ये_3 = values.length;
    अचिन्हित encoded_len = 0;
    अचिन्हित i = 0;
    पर्यंत (i < अंक_मूल्ये_3)
    {
      पूर्णांक val = values.arrayZ[i];
      जर (val == 0)
        encoded_len += encode_मूल्य_run_as_zeroes (i, encoded_bytes + encoded_len, values);
      अन्यथा जर ((int8_t) val == val)
        encoded_len += encode_मूल्य_run_as_bytes (i, encoded_bytes + encoded_len, values);
      अन्यथा जर ((int16_t) val == val)
        encoded_len += encode_मूल्य_run_as_शब्द (i, encoded_bytes + encoded_len, values);
      अन्यथा
        encoded_len += encode_मूल्य_run_as_longs (i, encoded_bytes + encoded_len, values);
    }
    परत encoded_len;
  }

  स्थिर अचिन्हित encode_मूल्य_run_as_zeroes (अचिन्हित& i,
					      अचिन्हित अक्षर *it,
					      hb_array_t<स्थिरांक पूर्णांक> values)
  {
    अचिन्हित अंक_मूल्ये_3 = values.length;
    अचिन्हित run_length = 0;
    अचिन्हित encoded_len = 0;
    पर्यंत (i < अंक_मूल्ये_3 && values.arrayZ[i] == 0)
    {
      i++;
      run_length++;
    }

    पर्यंत (run_length >= 64)
    {
      *it++ = अक्षर (मूल्ये_ARE_ZEROS | 63);
      run_length -= 64;
      encoded_len++;
    }

    जर (run_length)
    {
      *it++ = अक्षर (मूल्ये_ARE_ZEROS | (run_length - 1));
      encoded_len++;
    }
    परत encoded_len;
  }

  स्थिर अचिन्हित encode_मूल्य_run_as_bytes (अचिन्हित &i,
					     अचिन्हित अक्षर *it,
					     hb_array_t<स्थिरांक पूर्णांक> values)
  {
    अचिन्हित start = i;
    अचिन्हित अंक_मूल्ये_3 = values.length;
    पर्यंत (i < अंक_मूल्ये_3)
    {
      पूर्णांक val = values.arrayZ[i];
      जर ((int8_t) val != val)
        थांबा;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      जर (val == 0 && i + 1 < अंक_मूल्ये_3 && values.arrayZ[i+1] == 0)
        थांबा;

      i++;
    }
    अचिन्हित run_length = i - start;

    अचिन्हित encoded_len = 0;

    पर्यंत (run_length >= 64)
    {
      *it++ = (मूल्ये_ARE_BYTES | 63);
      encoded_len++;

      साठी (अचिन्हित j = 0; j < 64; j++)
	it[j] = static_cast<अक्षर> (values.arrayZ[start + j]);
      it += 64;
      encoded_len += 64;

      start += 64;
      run_length -= 64;
    }

    जर (run_length)
    {
      *it++ = (मूल्ये_ARE_BYTES | (run_length - 1));
      encoded_len++;

      साठी (अचिन्हित j = 0; j < run_length; j++)
        it[j] = static_cast<अक्षर> (values.arrayZ[start + j]);
      encoded_len += run_length;
    }

    परत encoded_len;
  }

  स्थिर अचिन्हित encode_मूल्य_run_as_शब्द (अचिन्हित &i,
					     अचिन्हित अक्षर *it,
					     hb_array_t<स्थिरांक पूर्णांक> values)
  {
    अचिन्हित start = i;
    अचिन्हित अंक_मूल्ये_3 = values.length;
    पर्यंत (i < अंक_मूल्ये_3)
    {
      पूर्णांक val = values.arrayZ[i];

      जर ((int16_t) val != val)
        थांबा;

      /* start a new run for a single zero value. */
      जर (val == 0) थांबा;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      जर ((int8_t) val == val &&
          i + 1 < अंक_मूल्ये_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        थांबा;

      i++;
    }

    अचिन्हित run_length = i - start;
    अचिन्हित encoded_len = 0;
    पर्यंत (run_length >= 64)
    {
      *it++ = (मूल्ये_ARE_शब्द | 63);
      encoded_len++;

      साठी (अचिन्हित j = 0; j < 64; j++)
      {
        int16_t मूल्य_val = values.arrayZ[start + j];
        *it++ = static_cast<अक्षर> (मूल्य_val >> 8);
        *it++ = static_cast<अक्षर> (मूल्य_val & 0xFF);

        encoded_len += 2;
      }

      start += 64;
      run_length -= 64;
    }

    जर (run_length)
    {
      *it++ = (मूल्ये_ARE_शब्द | (run_length - 1));
      encoded_len++;
      पर्यंत (start < i)
      {
        int16_t मूल्य_val = values.arrayZ[start++];
        *it++ = static_cast<अक्षर> (मूल्य_val >> 8);
        *it++ = static_cast<अक्षर> (मूल्य_val & 0xFF);

        encoded_len += 2;
      }
    }
    परत encoded_len;
  }

  स्थिर अचिन्हित encode_मूल्य_run_as_longs (अचिन्हित &i,
					     अचिन्हित अक्षर *it,
					     hb_array_t<स्थिरांक पूर्णांक> values)
  {
    अचिन्हित start = i;
    अचिन्हित अंक_मूल्ये_3 = values.length;
    पर्यंत (i < अंक_मूल्ये_3)
    {
      पूर्णांक val = values.arrayZ[i];

      जर ((int16_t) val == val)
        थांबा;

      i++;
    }

    अचिन्हित run_length = i - start;
    अचिन्हित encoded_len = 0;
    पर्यंत (run_length >= 64)
    {
      *it++ = (मूल्ये_ARE_LONGS | 63);
      encoded_len++;

      साठी (अचिन्हित j = 0; j < 64; j++)
      {
        int32_t मूल्य_val = values.arrayZ[start + j];
        *it++ = static_cast<अक्षर> (मूल्य_val >> 24);
        *it++ = static_cast<अक्षर> (मूल्य_val >> 16);
        *it++ = static_cast<अक्षर> (मूल्य_val >> 8);
        *it++ = static_cast<अक्षर> (मूल्य_val & 0xFF);

        encoded_len += 4;
      }

      start += 64;
      run_length -= 64;
    }

    जर (run_length)
    {
      *it++ = (मूल्ये_ARE_LONGS | (run_length - 1));
      encoded_len++;
      पर्यंत (start < i)
      {
        int32_t मूल्य_val = values.arrayZ[start++];
        *it++ = static_cast<अक्षर> (मूल्य_val >> 24);
        *it++ = static_cast<अक्षर> (मूल्य_val >> 16);
        *it++ = static_cast<अक्षर> (मूल्य_val >> 8);
        *it++ = static_cast<अक्षर> (मूल्य_val & 0xFF);

        encoded_len += 4;
      }
    }
    परत encoded_len;
  }

  ढाँचा <typename T>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  स्थिर तार्किक decompile (स्थिरांक HBUINT8 *&p /* IN/OUT */,
			 hb_vector_t<T> &values /* IN/OUT */,
			 स्थिरांक HBUINT8 *end,
			 तार्किक consume_all = असत्य,
			 अचिन्हित start = 0)
  {
    अचिन्हित i = 0;
    अचिन्हित मोजणी = consume_all ? अच_पूर्णांक_उच्च : values.length;
    जर (consume_all)
      values.alloc ((end - p) / 2);
    पर्यंत (i < मोजणी)
    {
      जर (unlikely (p + 1 > end)) परत consume_all;
      अचिन्हित control = *p++;
      अचिन्हित run_मोजणी = (control & मूल्य_RUN_मोजणी_MASK) + 1;
      जर (consume_all)
      {
        जर (unlikely (!values.resize_dirty  (values.length + run_मोजणी)))
	  परत असत्य;
      }
      अचिन्हित stop = i + run_मोजणी;
      जर (unlikely (stop > मोजणी)) परत असत्य;

      अचिन्हित skip = i < start ? hb_min (start - i, run_मोजणी) : 0;
      i += skip;

      जर ((control & मूल्ये_SIZE_MASK) == मूल्ये_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * आकार (T));
	i = stop;
      }
      अन्यथा जर ((control & मूल्ये_SIZE_MASK) ==  मूल्ये_ARE_शब्द)
      {
        जर (unlikely (p + run_मोजणी * HBINT16::static_size > end)) परत असत्य;
	p += skip * HBINT16::static_size;
#ifndef HB_OPTIMIZE_SIZE
        साठी (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (स्थिरांक HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 1] = * (स्थिरांक HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 2] = * (स्थिरांक HBINT16 *) p;
	  p += HBINT16::static_size;
	  values.arrayZ[i + 3] = * (स्थिरांक HBINT16 *) p;
	  p += HBINT16::static_size;
	}
#endif
        साठी (; i < stop; i++)
        {
          values.arrayZ[i] = * (स्थिरांक HBINT16 *) p;
          p += HBINT16::static_size;
        }
      }
      अन्यथा जर ((control & मूल्ये_SIZE_MASK) ==  मूल्ये_ARE_LONGS)
      {
        जर (unlikely (p + run_मोजणी * HBINT32::static_size > end)) परत असत्य;
	p += skip * HBINT32::static_size;
        साठी (; i < stop; i++)
        {
          values.arrayZ[i] = * (स्थिरांक HBINT32 *) p;
          p += HBINT32::static_size;
        }
      }
      अन्यथा जर ((control & मूल्ये_SIZE_MASK) ==  मूल्ये_ARE_BYTES)
      {
        जर (unlikely (p + run_मोजणी > end)) परत असत्य;
	p += skip * HBINT8::static_size;
#ifndef HB_OPTIMIZE_SIZE
	साठी (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (स्थिरांक HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (स्थिरांक HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (स्थिरांक HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (स्थिरांक HBINT8 *) p++;
	}
#endif
        साठी (; i < stop; i++)
          values.arrayZ[i] = * (स्थिरांक HBINT8 *) p++;
      }
    }
    परत सत्य;
  }

  स्थिर तार्किक skip (स्थिरांक अचिन्हित अक्षर *&p /* IN/OUT */,
		    स्थिरांक अचिन्हित अक्षर *end,
		    अचिन्हित मोजणी)
  {
    अचिन्हित seen = 0;
    पर्यंत (seen < मोजणी)
    {
      जर (unlikely (p >= end)) परत असत्य;
      अचिन्हित control = *p++;
      अचिन्हित run_मोजणी = (control & मूल्य_RUN_मोजणी_MASK) + 1;
      जर (unlikely (run_मोजणी > मोजणी - seen)) परत असत्य;

      अचिन्हित width = 0;
      निवड (control & मूल्ये_SIZE_MASK)
      {
	स्थिती मूल्ये_ARE_ZEROS: थांबा;
	स्थिती मूल्ये_ARE_BYTES: width = HBINT8::static_size; थांबा;
	स्थिती मूल्ये_ARE_शब्द: width = HBINT16::static_size; थांबा;
	स्थिती मूल्ये_ARE_LONGS: width = HBINT32::static_size; थांबा;
      }
      जर (unlikely (अचिन्हित (end - p) < run_मोजणी * width)) परत असत्य;
      p += run_मोजणी * width;
      seen += run_मोजणी;
    }
    परत सत्य;
  }

  संरचना iter_t : hb_iter_with_fallback_t<iter_t, पूर्णांक>
  {
    iter_t (स्थिरांक अचिन्हित अक्षर *p_, अचिन्हित len_)
	    : p (p_), endp (p_ + len_)
    { जर (likely (ensure_run ())) read_मूल्य (); }

    खाजगी:
    स्थिरांक अचिन्हित अक्षर *p;
    स्थिरांक अचिन्हित अक्षर * स्थिरांक endp;
    पूर्णांक current_मूल्य = 0;
    चिन्हित run_मोजणी = 0;
    अचिन्हित width = 0;

    HB_ALWAYS_INLINE
    तार्किक ensure_run ()
    {
      जर (likely (run_मोजणी > 0)) परत सत्य;
      परत _ensure_run ();
    }
    तार्किक _ensure_run ()
    {
      जर (unlikely (p >= endp))
      {
        run_मोजणी = 0;
        current_मूल्य = 0;
	परत असत्य;
      }

      अचिन्हित control = *p++;
      run_मोजणी = (control & मूल्य_RUN_मोजणी_MASK) + 1;
      width = control & मूल्ये_SIZE_MASK;
      निवड (width)
      {
        स्थिती मूल्ये_ARE_ZEROS: width = 0; थांबा;
	स्थिती मूल्ये_ARE_BYTES: width = HBINT8::static_size;  थांबा;
	स्थिती मूल्ये_ARE_शब्द: width = HBINT16::static_size; थांबा;
	स्थिती मूल्ये_ARE_LONGS: width = HBINT32::static_size; थांबा;
	पूर्वनिर्धारित: प्रतिज्ञा (असत्य);
      }

      जर (unlikely (p + run_मोजणी * width > endp))
      {
	run_मोजणी = 0;
	current_मूल्य = 0;
	परत असत्य;
      }

      परत सत्य;
    }
    शून्य read_मूल्य ()
    {
      निवड (width)
      {
        स्थिती 0: current_मूल्य = 0; थांबा;
	स्थिती 1: current_मूल्य = * (स्थिरांक HBINT8  *) p; थांबा;
	स्थिती 2: current_मूल्य = * (स्थिरांक HBINT16 *) p; थांबा;
	स्थिती 4: current_मूल्य = * (स्थिरांक HBINT32 *) p; थांबा;
      }
      p += width;
    }

    सार्वजनिक:

    प्रकारव्याख्या पूर्णांक __item_t__;
    __item_t__ __item__ () स्थिरांक
    { परत current_मूल्य; }

    तार्किक __more__ () स्थिरांक { परत run_मोजणी || p < endp; }
    शून्य __next__ ()
    {
      run_मोजणी--;
      जर (unlikely (!ensure_run ()))
	परत;
      read_मूल्य ();
    }
    शून्य __forward__ (अचिन्हित n)
    {
      जर (unlikely (!ensure_run ()))
	परत;
      पर्यंत (n)
      {
	अचिन्हित i = hb_min (n, (अचिन्हित) run_मोजणी);
	run_मोजणी -= i;
	n -= i;
	p += (i - 1) * width;
	जर (unlikely (!ensure_run ()))
	  परत;
	read_मूल्य ();
      }
    }
    तार्किक चालक != (स्थिरांक iter_t& o) स्थिरांक
    { परत p != o.p || run_मोजणी != o.run_मोजणी; }
    iter_t __end__ () स्थिरांक
    {
      iter_t it (endp, 0);
      परत it;
    }
  };

  संरचना fetcher_t
  {
    fetcher_t (स्थिरांक अचिन्हित अक्षर *p_, अचिन्हित len_)
	      : p (p_), end (p_ + len_) {}

    खाजगी:
    स्थिरांक अचिन्हित अक्षर *p;
    स्थिरांक अचिन्हित अक्षर * स्थिरांक end;
    चिन्हित run_मोजणी = 0;
    अचिन्हित width = 0;

    HB_ALWAYS_INLINE
    तार्किक ensure_run ()
    {
      जर (likely (run_मोजणी > 0)) परत सत्य;
      परत _ensure_run ();
    }

    तार्किक _ensure_run ()
    {
      जर (unlikely (p >= end))
      {
        run_मोजणी = 0;
	परत असत्य;
      }

      अचिन्हित control = *p++;
      run_मोजणी = (control & मूल्य_RUN_मोजणी_MASK) + 1;
      width = control & मूल्ये_SIZE_MASK;
      निवड (width)
      {
        स्थिती मूल्ये_ARE_ZEROS: width = 0; थांबा;
	स्थिती मूल्ये_ARE_BYTES: width = HBINT8::static_size;  थांबा;
	स्थिती मूल्ये_ARE_शब्द: width = HBINT16::static_size; थांबा;
	स्थिती मूल्ये_ARE_LONGS: width = HBINT32::static_size; थांबा;
	पूर्वनिर्धारित: प्रतिज्ञा (असत्य);
      }

      जर (unlikely (p + run_मोजणी * width > end))
      {
	run_मोजणी = 0;
	परत असत्य;
      }

      परत सत्य;
    }

    सार्वजनिक:
    शून्य skip (अचिन्हित n)
    {
      पर्यंत (n)
      {
	जर (unlikely (!ensure_run ()))
	  परत;
	अचिन्हित i = hb_min (n, (अचिन्हित) run_मोजणी);
	run_मोजणी -= i;
	n -= i;
	p += i * width;
      }
    }

    खाजगी:
    ढाँचा <तार्किक scaled, typename T>
    शून्य _add_to (hb_array_t<T> out, T scale = T (1))
    {
      अचिन्हित n = out.length;
      T *arrayZ = out.arrayZ;

      साठी (अचिन्हित i = 0; i < n;)
      {
	जर (unlikely (!ensure_run ()))
	  थांबा;
	अचिन्हित मोजणी = hb_min (n - i, (अचिन्हित) run_मोजणी);
	निवड (width)
	{
	  स्थिती 0:
	  {
	    arrayZ += मोजणी;
	    थांबा;
	  }
	  स्थिती 1:
	  {
	    स्थिरांक स्वतः *pp = (स्थिरांक HBINT8 *) p;
	    अचिन्हित j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    साठी (; j + 3 < मोजणी; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    साठी (; j < मोजणी; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (स्थिरांक अचिन्हित अक्षर *) pp;
	  }
	  थांबा;
	  स्थिती 2:
	  {
	    स्थिरांक स्वतः *pp = (स्थिरांक HBINT16 *) p;
	    अचिन्हित j = 0;
#ifndef HB_OPTIMIZE_SIZE
	    साठी (; j + 3 < मोजणी; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#endif
	    साठी (; j < मोजणी; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (स्थिरांक अचिन्हित अक्षर *) pp;
	  }
	  थांबा;
	  स्थिती 4:
	  {
	    स्थिरांक स्वतः *pp = (स्थिरांक HBINT32 *) p;
	    साठी (अचिन्हित j = 0; j < मोजणी; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (स्थिरांक अचिन्हित अक्षर *) pp;
	  }
	  थांबा;
	}
	run_मोजणी -= मोजणी;
	i += मोजणी;
      }
    }

    सार्वजनिक:
    ढाँचा <typename T>
    शून्य add_to (hb_array_t<T> out, दशांश scale = 1.0f)
    {
#ifndef HB_OPTIMIZE_SIZE
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      जर (असत्य && scale == 1.0f)
        _add_to<असत्य> (out);
      अन्यथा
#endif
        _add_to<सत्य> (out, (T) scale);
    }
  };
};

संरचना TupleList : CFF2Index
{
  Tuple_मूल्ये::iter_t चालक [] (अचिन्हित i) स्थिरांक
  {
    स्वतः bytes = CFF2Index::चालक [] (i);
    परत Tuple_मूल्ये::iter_t (bytes.arrayZ, bytes.length);
  }

  Tuple_मूल्ये::fetcher_t fetcher (अचिन्हित i) स्थिरांक
  {
    स्वतः bytes = CFF2Index::चालक [] (i);
    परत Tuple_मूल्ये::fetcher_t (bytes.arrayZ, bytes.length);
  }
};


// Alignment

ढाँचा <अचिन्हित पूर्णांक alignment>
संरचना Align
{
  माप_प्रकार get_size (स्थिरांक शून्य *base) स्थिरांक
  {
    अचिन्हित offset = (स्थिरांक अक्षर *) हे - (स्थिरांक अक्षर *) base;
    परत (alignment - offset) & (alignment - 1);
  }

  सार्वजनिक:
  DEFINE_SIZE_MIN (0);
};



} /* namespace OT */


#endif /* HB_OPEN_TYPE_HH */
