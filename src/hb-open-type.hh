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

#جيڪڏهن_وصف_نه_ٿيل HB_OPEN_TYPE_HH
#وصف_ڪريو HB_OPEN_TYPE_HH

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
سانچو <منطقي BE,
	  قسم_نالو Type,
	  بنانشان صحيح سائيز_2 = حجم (Type)>
بنية انگ_Type
{
  تعريفنوع Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  تعريفنوع قسم_نالو std::conditional<std::is_integral<Type>::value && حجم (Type) <= حجم(صحيح),
				     قسم_نالو std::conditional<std::is_signed<Type>::value, نشانوارو, بنانشان>::type,
				     Type>::type WideType;

  انگ_Type () = افتراضي;
  واضح constexpr انگ_Type (Type V) : v {V} {}
  انگ_Type& عامل = (Type V) { v = V; أرجع *هذا; }

  عامل WideType () ثابت { أرجع v; }

  منطقي عامل == (ثابت انگ_Type &o) ثابت { أرجع (Type) v == (Type) o.v; }
  منطقي عامل != (ثابت انگ_Type &o) ثابت { أرجع !(*هذا == o); }

  انگ_Type& عامل += (WideType ڳڻپ) { *هذا = *هذا + ڳڻپ; أرجع *هذا; }
  انگ_Type& عامل -= (WideType ڳڻپ) { *هذا = *هذا - ڳڻپ; أرجع *هذا; }
  انگ_Type& عامل ++ () { *هذا += 1; أرجع *هذا; }
  انگ_Type& عامل -- () { *هذا -= 1; أرجع *هذا; }
  انگ_Type عامل ++ (صحيح) { انگ_Type c (*هذا); ++*هذا; أرجع c; }
  انگ_Type عامل -- (صحيح) { انگ_Type c (*هذا); --*هذا; أرجع c; }

  uint32_t hash () ثابت { أرجع hb_array ((ثابت حرف *) &v, حجم (v)).hash (); }
  HB_اندروني ساكن صحيح cmp (ثابت انگ_Type *a, ثابت انگ_Type *b)
  { أرجع b->cmp (*a); }
  HB_اندروني ساكن صحيح cmp (ثابت فراغ *a, ثابت فراغ *b)
  {
    انگ_Type *pa = (انگ_Type *) a;
    انگ_Type *pb = (انگ_Type *) b;

    أرجع pb->cmp (*pa);
  }
  سانچو <قسم_نالو Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  صحيح cmp (Type2 a) ثابت
  {
    Type b = v;
    أرجع (a > b) - (a < b);
  }
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }
  محمي:
  قسم_نالو std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, سائيز_2>,
			    HBFloat<BE, Type, سائيز_2>>::type v;
  عام:
  DEFINE_سائيز_STATIC (سائيز_2);
};

تعريفنوع انگ_Type<سچ, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
تعريفنوع انگ_Type<سچ, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
تعريفنوع انگ_Type<سچ, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
تعريفنوع انگ_Type<سچ, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
تعريفنوع انگ_Type<سچ, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
تعريفنوع انگ_Type<سچ, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
تعريفنوع انگ_Type<سچ, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
تعريفنوع انگ_Type<سچ, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
تعريفنوع انگ_Type<سچ, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

تعريفنوع انگ_Type<خطأ, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
تعريفنوع انگ_Type<خطأ, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
تعريفنوع انگ_Type<خطأ, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
تعريفنوع انگ_Type<خطأ, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
تعريفنوع انگ_Type<خطأ, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
تعريفنوع انگ_Type<خطأ, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

تعريفنوع انگ_Type<سچ,  عائم>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
تعريفنوع انگ_Type<سچ,  مزدوج> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
تعريفنوع انگ_Type<خطأ, عائم>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
تعريفنوع انگ_Type<خطأ, مزدوج> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
بنية HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& عامل = (uint16_t i ) { HBUINT16::عامل= (i); أرجع *هذا; }
  عام:
  DEFINE_سائيز_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
بنية HBUINT32VAR
{
  size_t get_سائيز () ثابت
  {
    بنانشان b0 = v[0];
    إذا (b0 < 0x80)
      أرجع 1;
    وإلا إذا (b0 < 0xC0)
      أرجع 2;
    وإلا إذا (b0 < 0xE0)
      أرجع 3;
    وإلا إذا (b0 < 0xF0)
      أرجع 4;
    وإلا
      أرجع 5;
  }

  ساكن size_t get_سائيز (uint32_t v)
  {
    إذا (v < 0x80)
      أرجع 1;
    وإلا إذا (v < 0x4000)
      أرجع 2;
    وإلا إذا (v < 0x200000)
      أرجع 3;
    وإلا إذا (v < 0x10000000)
      أرجع 4;
    وإلا
      أرجع 5;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_سائيز ()));
  }

  عامل uint32_t () ثابت
  {
    بنانشان b0 = v[0];
    إذا (b0 < 0x80)
      أرجع b0;
    وإلا إذا (b0 < 0xC0)
      أرجع ((b0 & 0x3F) << 8) | v[1];
    وإلا إذا (b0 < 0xE0)
      أرجع ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    وإلا إذا (b0 < 0xF0)
      أرجع ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    وإلا
      أرجع (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  ساكن بنانشان serialize_unsafe (بنانشان حرف *buf, uint32_t v)
  {
    بنانشان len = get_سائيز (v);
    بنانشان حرف *p = buf + len;
    لكل (بنانشان i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    إذا (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    أرجع len;
  }

  ساكن منطقي serialize (hb_serialize_context_t *c, uint32_t v)
  {
    بنانشان len = get_سائيز (v);
    بنانشان حرف *buf = c->allocate_سائيز<بنانشان حرف> (len, خطأ);
    إذا (unlikely (!buf))
      أرجع خطأ;

    serialize_unsafe (buf, v);
    أرجع سچ;
  }

  محمي:
  بنانشان حرف v[5];

  عام:
  DEFINE_سائيز_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
تعريفنوع HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
تعريفنوع HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
تعريفنوع HBUINT16 UFWORD;

سانچو <قسم_نالو Type, بنانشان fraction_بٽ>
بنية HBFixed : Type
{
  ساكن constexpr عائم mult = 1.f / (1 << fraction_بٽ);
  static_assert (Type::static_سائيز * 8 > fraction_بٽ, "");

  عامل نشانوارو () ثابت = ميٽيو;
  عامل بنانشان () ثابت = ميٽيو;
  واضح عامل عائم () ثابت { أرجع منزل_float (); }
  قسم_نالو Type::type منزل_int () ثابت { أرجع Type::v; }
  فراغ سيٽ_int (قسم_نالو Type::type i ) { Type::v = i; }
  عائم منزل_float (عائم وٿي = 0) ثابت  { أرجع ((int32_t) Type::v + وٿي) * mult; }
  فراغ سيٽ_float (عائم f) { Type::v = roundf (f / mult); }
  عام:
  DEFINE_سائيز_STATIC (Type::static_سائيز);
};

/* 16-bit signed fixed number with the low 14 bits of fraction (2.14). */
استعمال F2DOT14 = HBFixed<HBINT16, 14>;
استعمال F4DOT12 = HBFixed<HBINT16, 12>;
استعمال F6DOT10 = HBFixed<HBINT16, 10>;

/* 32-bit signed fixed-point number (16.16). */
استعمال F16DOT16 = HBFixed<HBINT32, 16>;

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
  DEFINE_سائيز_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
بنية ٽيگ : HBUINT32
{
  ٽيگ& عامل = (hb_ٽيگ_t i) { HBUINT32::عامل= (i); أرجع *هذا; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  عامل ثابت حرف* () ثابت { أرجع reinterpret_cast<ثابت حرف *> (هذا); }
  عامل حرف* ()             { أرجع reinterpret_cast<حرف *> (هذا); }
  عام:
  DEFINE_سائيز_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
بنية HBGlyphID16 : HBUINT16
{
  HBGlyphID16& عامل = (uint16_t i) { HBUINT16::عامل= (i); أرجع *هذا; }
};
بنية HBGlyphID24 : HBUINT24
{
  HBGlyphID24& عامل = (uint32_t i) { HBUINT24::عامل= (i); أرجع *هذا; }
};

/* Script/language-system/feature index */
بنية اشاريو : HBUINT16 {
  ساكن constexpr بنانشان NOT_مليو_اشاريو = 0xFFFFu;
  اشاريو& عامل = (uint16_t i) { HBUINT16::عامل= (i); أرجع *هذا; }
};
DECLARE_NULL_NAMESPACE_بائيٽون (OT, اشاريو);

تعريفنوع اشاريو NameID;

بنية VarIdx : HBUINT32 {
  ساكن constexpr بنانشان NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_اشاريو, "");
  VarIdx& عامل = (uint32_t i) { HBUINT32::عامل= (i); أرجع *هذا; }
};
DECLARE_NULL_NAMESPACE_بائيٽون (OT, VarIdx);

/* Offset, Null offset = 0 */
سانچو <قسم_نالو Type, منطقي has_null=سچ>
بنية وٿي_2 : Type
{
  وٿي_2& عامل = (قسم_نالو Type::type i) { Type::عامل= (i); أرجع *هذا; }

  تعريفنوع Type type;

  منطقي is_null () ثابت { أرجع has_null && 0 == *هذا; }

  عام:
  DEFINE_سائيز_STATIC (حجم (Type));
};

تعريفنوع وٿي_2<HBUINT16> Offset16;
تعريفنوع وٿي_2<HBUINT24> Offset24;
تعريفنوع وٿي_2<HBUINT32> Offset32;


/* CheckSum */
بنية Check_جوڙ : HBUINT32
{
  Check_جوڙ& عامل = (uint32_t i) { HBUINT32::عامل= (i); أرجع *هذا; }

  /* This is reference implementation from the spec. */
  ساكن uint32_t Calc_جدول_Checksum (ثابت HBUINT32 *جدول_3, uint32_t ڊيگهائي_3)
  {
    uint32_t Sum = 0L;
    أكد (0 == (ڊيگهائي_3 & 3));
    ثابت HBUINT32 *پڇاڙي_Ptr = جدول_3 + ڊيگهائي_3 / HBUINT32::static_سائيز;

    طالما (جدول_3 < پڇاڙي_Ptr)
      Sum += *جدول_3++;
    أرجع Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  فراغ سيٽ_for_معلومات (ثابت فراغ *معلومات, بنانشان صحيح ڊيگهائي)
  { *هذا = Calc_جدول_Checksum ((ثابت HBUINT32 *) معلومات, ڊيگهائي); }

  عام:
  DEFINE_سائيز_STATIC (4);
};


/*
 * Version Numbers
 */

سانچو <قسم_نالو FixedType=HBUINT16>
بنية Fixed_نسخو
{
  uint32_t منزل_int () ثابت { أرجع (major << (حجم (FixedType) * 8)) + minor; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  FixedType major;
  FixedType minor;
  عام:
  DEFINE_سائيز_STATIC (2 * حجم (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

سانچو <قسم_نالو Type, منطقي has_null>
بنية _hb_has_null
{
  ساكن ثابت Type *get_null () { أرجع خالي_اشارو; }
  ساكن Type *get_crap ()       { أرجع خالي_اشارو; }
};
سانچو <قسم_نالو Type>
بنية _hb_has_null<Type, سچ>
{
  ساكن ثابت Type *get_null () { أرجع &Null (Type); }
  ساكن       Type *get_crap () { أرجع &Crap (Type); }
};

سانچو <قسم_نالو Type, قسم_نالو وٿي_Type, قسم_نالو بنياد_Type=فراغ, منطقي has_null=سچ>
بنية وٿي_منزل : وٿي_2<وٿي_Type, has_null>
{
  استعمال هدف_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == خطأ ||
		 (hb_has_null_سائيز_2 (Type) || !hb_has_min_سائيز_2 (Type)), "");

  HB_DELETE_COPY_سونپيو (وٿي_منزل);
  وٿي_منزل () = افتراضي;

  وٿي_منزل& عامل = (قسم_نالو وٿي_Type::type i) { وٿي_Type::عامل= (i); أرجع *هذا; }

  ثابت Type& عامل () (ثابت فراغ *بنياد) ثابت
  {
    إذا (unlikely (هذا->is_null ())) أرجع *_hb_has_null<Type, has_null>::get_null ();
    أرجع Struct_At_وٿي<ثابت Type> (بنياد, *هذا);
  }
  Type& عامل () (فراغ *بنياد) ثابت
  {
    إذا (unlikely (هذا->is_null ())) أرجع *_hb_has_null<Type, has_null>::get_crap ();
    أرجع Struct_At_وٿي<Type> (بنياد, *هذا);
  }

  سانچو <قسم_نالو بنياد_3,
	    hb_enable_if (hb_is_convertible (ثابت بنياد_3, ثابت بنياد_Type *))>
  دوست ثابت Type& عامل + (ثابت بنياد_3 &بنياد, ثابت وٿي_منزل &وٿي) { أرجع وٿي ((ثابت فراغ *) بنياد); }
  سانچو <قسم_نالو بنياد_3,
	    hb_enable_if (hb_is_convertible (ثابت بنياد_3, ثابت بنياد_Type *))>
  دوست ثابت Type& عامل + (ثابت وٿي_منزل &وٿي, ثابت بنياد_3 &بنياد) { أرجع وٿي ((ثابت فراغ *) بنياد); }
  سانچو <قسم_نالو بنياد_3,
	    hb_enable_if (hb_is_convertible (بنياد_3, بنياد_Type *))>
  دوست Type& عامل + (بنياد_3 &&بنياد, وٿي_منزل &وٿي) { أرجع وٿي ((فراغ *) بنياد); }
  سانچو <قسم_نالو بنياد_3,
	    hb_enable_if (hb_is_convertible (بنياد_3, بنياد_Type *))>
  دوست Type& عامل + (وٿي_منزل &وٿي, بنياد_3 &&بنياد) { أرجع وٿي ((فراغ *) بنياد); }


  سانچو <قسم_نالو بنياد_3, قسم_نالو ...Ts>
  منطقي serialize_subset (hb_subset_context_t *c, ثابت وٿي_منزل& src,
			 ثابت بنياد_3 *src_بنياد, Ts&&... ds)
  {
    *هذا = 0;
    إذا (src.is_null ())
      أرجع خطأ;

    خودڪار *s = c->serializer;

    s->push ();

    منطقي ret = c->dispatch (src_بنياد+src, std::forward<Ts> (ds)...);

    إذا (ret || !has_null)
      s->جوڙيو_link (*هذا, s->ڪڍو_pack ());
    وإلا
      s->ڪڍو_discard ();

    أرجع ret;
  }


  سانچو <قسم_نالو ...Ts>
  منطقي serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *هذا = 0;

    Type* obj = c->push<Type> ();
    منطقي ret = obj->serialize (c, std::forward<Ts> (ds)...);

    إذا (ret)
      c->جوڙيو_link (*هذا, c->ڪڍو_pack ());
    وإلا
      c->ڪڍو_discard ();

    أرجع ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  سانچو <قسم_نالو ...Ts>
  منطقي serialize_copy (hb_serialize_context_t *c, ثابت وٿي_منزل& src,
		       ثابت فراغ *src_بنياد, بنانشان dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *هذا = 0;
    إذا (src.is_null ())
      أرجع خطأ;

    c->push ();

    منطقي ret = c->copy (src_بنياد+src, std::forward<Ts> (ds)...);

    c->جوڙيو_link (*هذا, c->ڪڍو_pack (), whence, dst_bias);

    أرجع ret;
  }

  منطقي serialize_copy (hb_serialize_context_t *c, ثابت وٿي_منزل& src,
		       ثابت فراغ *src_بنياد, بنانشان dst_bias = 0)
  { أرجع serialize_copy (c, src, src_بنياد, dst_bias, hb_serialize_context_t::سر); }

  منطقي sanitize_shallow (hb_sanitize_context_t *c, ثابت بنياد_Type *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    إذا (unlikely ((ثابت حرف *) بنياد + (بنانشان) *هذا < (ثابت حرف *) بنياد)) return_trace (خطأ);
    return_trace (سچ);
  }

  سانچو <قسم_نالو ...Ts>
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
  منطقي sanitize (hb_sanitize_context_t *c, ثابت بنياد_Type *بنياد, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (sanitize_shallow (c, بنياد) &&
		  hb_barrier () &&
		  (هذا->is_null () ||
		   c->dispatch (Struct_At_وٿي<Type> (بنياد, *هذا), std::forward<Ts> (ds)...)));
  }

  DEFINE_سائيز_STATIC (حجم (وٿي_Type));
};
/* Partial specializations. */
سانچو <قسم_نالو Type, قسم_نالو بنياد_Type=فراغ, منطقي has_null=سچ> استعمال Offset16_منزل = وٿي_منزل<Type, HBUINT16, بنياد_Type, has_null>;
سانچو <قسم_نالو Type, قسم_نالو بنياد_Type=فراغ, منطقي has_null=سچ> استعمال Offset24_منزل = وٿي_منزل<Type, HBUINT24, بنياد_Type, has_null>;
سانچو <قسم_نالو Type, قسم_نالو بنياد_Type=فراغ, منطقي has_null=سچ> استعمال Offset32_منزل = وٿي_منزل<Type, HBUINT32, بنياد_Type, has_null>;

سانچو <قسم_نالو Type, قسم_نالو وٿي_Type, قسم_نالو بنياد_Type=فراغ> استعمال NNOffset_منزل = وٿي_منزل<Type, وٿي_Type, بنياد_Type, خطأ>;
سانچو <قسم_نالو Type, قسم_نالو بنياد_Type=فراغ> استعمال NNOffset16_منزل = Offset16_منزل<Type, بنياد_Type, خطأ>;
سانچو <قسم_نالو Type, قسم_نالو بنياد_Type=فراغ> استعمال NNOffset24_منزل = Offset24_منزل<Type, بنياد_Type, خطأ>;
سانچو <قسم_نالو Type, قسم_نالو بنياد_Type=فراغ> استعمال NNOffset32_منزل = Offset32_منزل<Type, بنياد_Type, خطأ>;


/*
 * Array Types
 */

سانچو <قسم_نالو Type>
بنية Unsized_Array_جو
{
  تعريفنوع Type شي_t;
  ساكن constexpr بنانشان شي_سائيز = hb_static_سائيز (Type);

  HB_DELETE_CREATE_COPY_سونپيو (Unsized_Array_جو);

  ثابت Type& عامل [] (بنانشان صحيح i) ثابت
  {
    أرجع arrayZ[i];
  }
  Type& عامل [] (بنانشان صحيح i)
  {
    أرجع arrayZ[i];
  }

  ساكن size_t get_سائيز (بنانشان صحيح len)
  { أرجع hb_unsigned_mul_saturate (len, Type::static_سائيز); }

  سانچو <قسم_نالو T> عامل T * () { أرجع arrayZ; }
  سانچو <قسم_نالو T> عامل ثابت T * () ثابت { أرجع arrayZ; }
  hb_array_t<Type> as_array (بنانشان صحيح len)
  { أرجع hb_array (arrayZ, len); }
  hb_array_t<ثابت Type> as_array (بنانشان صحيح len) ثابت
  { أرجع hb_array (arrayZ, len); }

  سانچو <قسم_نالو T>
  Type &lsearch (بنانشان صحيح len, ثابت T &x, Type &not_مليو = Crap (Type))
  { أرجع *as_array (len).lsearch (x, &not_مليو); }
  سانچو <قسم_نالو T>
  ثابت Type &lsearch (بنانشان صحيح len, ثابت T &x, ثابت Type &not_مليو = Null (Type)) ثابت
  { أرجع *as_array (len).lsearch (x, &not_مليو); }
  سانچو <قسم_نالو T>
  منطقي lfind (بنانشان صحيح len, ثابت T &x, بنانشان صحيح *i = خالي_اشارو,
	      hb_not_مليو_t not_مليو = HB_NOT_مليو_DONT_STORE,
	      بنانشان صحيح منزل_store = (بنانشان صحيح) -1) ثابت
  { أرجع as_array (len).lfind (x, i, not_مليو, منزل_store); }

  فراغ qsort (بنانشان صحيح len, بنانشان صحيح شروعات = 0, بنانشان صحيح پڇاڙي = (بنانشان صحيح) -1)
  { as_array (len).qsort (شروعات, پڇاڙي); }

  منطقي serialize (hb_serialize_context_t *c, بنانشان صحيح شيون_len, منطقي clear = سچ)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_سائيز (هذا, get_سائيز (شيون_len), clear))) return_trace (خطأ);
    return_trace (سچ);
  }
  سانچو <قسم_نالو Iterator,
	    hb_requires (hb_is_ذريعو_جو (Iterator, Type))>
  منطقي serialize (hb_serialize_context_t *c, Iterator شيون)
  {
    TRACE_SERIALIZE (هذا);
    بنانشان ڳڻپ = hb_len (شيون);
    إذا (unlikely (!serialize (c, ڳڻپ, خطأ))) return_trace (خطأ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    لكل (بنانشان i = 0; i < ڳڻپ; i++, ++شيون)
      arrayZ[i] = *شيون;
    return_trace (سچ);
  }

  Unsized_Array_جو* copy (hb_serialize_context_t *c, بنانشان ڳڻپ) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار *ٻاهريون = c->شروعات_embed (هذا);
    إذا (unlikely (!as_array (ڳڻپ).copy (c))) return_trace (خالي_اشارو);
    return_trace (ٻاهريون);
  }

  سانچو <قسم_نالو ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, بنانشان صحيح ڳڻپ, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c, ڳڻپ))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (سچ);
    hb_barrier ();
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (خطأ);
    return_trace (سچ);
  }

  منطقي sanitize_shallow (hb_sanitize_context_t *c, بنانشان صحيح ڳڻپ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_array (arrayZ, ڳڻپ));
  }

  عام:
  Type		arrayZ[HB_VAR_ARRAY];
  عام:
  DEFINE_سائيز_UNBOUNDED (0);
};

/* Unsized array of offset's */
سانچو <قسم_نالو Type, قسم_نالو وٿي_Type, قسم_نالو بنياد_Type=فراغ, منطقي has_null=سچ>
استعمال Unsized_Array16_جو_وٿي_منزل = Unsized_Array_جو<وٿي_منزل<Type, وٿي_Type, بنياد_Type, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
سانچو <قسم_نالو Type, قسم_نالو وٿي_Type, قسم_نالو بنياد_Type=فراغ, منطقي has_null=سچ>
بنية Unsized_List_جو_Offset16_منزل : Unsized_Array16_جو_وٿي_منزل<Type, وٿي_Type, بنياد_Type, has_null>
{
  ثابت Type& عامل [] (صحيح i_) ثابت
  {
    بنانشان صحيح i = (بنانشان صحيح) i_;
    ثابت وٿي_منزل<Type, وٿي_Type, بنياد_Type, has_null> *p = &هذا->arrayZ[i];
    إذا (unlikely ((ثابت فراغ *) p < (ثابت فراغ *) هذا->arrayZ)) أرجع Null (Type); /* Overflowed. */
    hb_barrier ();
    أرجع هذا+*p;
  }
  Type& عامل [] (صحيح i_)
  {
    بنانشان صحيح i = (بنانشان صحيح) i_;
    ثابت وٿي_منزل<Type, وٿي_Type, بنياد_Type, has_null> *p = &هذا->arrayZ[i];
    إذا (unlikely ((ثابت فراغ *) p < (ثابت فراغ *) هذا->arrayZ)) أرجع Crap (Type); /* Overflowed. */
    hb_barrier ();
    أرجع هذا+*p;
  }

  سانچو <قسم_نالو ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, بنانشان صحيح ڳڻپ, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace ((Unsized_Array16_جو_وٿي_منزل<Type, وٿي_Type, بنياد_Type, has_null>
		   ::sanitize (c, ڳڻپ, هذا, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
سانچو <قسم_نالو Type>
بنية Sorted_Unsized_Array_جو : Unsized_Array_جو<Type>
{
  hb_sorted_array_t<Type> as_array (بنانشان صحيح len)
  { أرجع hb_sorted_array (هذا->arrayZ, len); }
  hb_sorted_array_t<ثابت Type> as_array (بنانشان صحيح len) ثابت
  { أرجع hb_sorted_array (هذا->arrayZ, len); }
  عامل hb_sorted_array_t<Type> ()             { أرجع as_array (); }
  عامل hb_sorted_array_t<ثابت Type> () ثابت { أرجع as_array (); }

  سانچو <قسم_نالو T>
  Type &bsearch (بنانشان صحيح len, ثابت T &x, Type &not_مليو = Crap (Type))
  { أرجع *as_array (len).bsearch (x, &not_مليو); }
  سانچو <قسم_نالو T>
  ثابت Type &bsearch (بنانشان صحيح len, ثابت T &x, ثابت Type &not_مليو = Null (Type)) ثابت
  { أرجع *as_array (len).bsearch (x, &not_مليو); }
  سانچو <قسم_نالو T>
  منطقي bfind (بنانشان صحيح len, ثابت T &x, بنانشان صحيح *i = خالي_اشارو,
	      hb_not_مليو_t not_مليو = HB_NOT_مليو_DONT_STORE,
	      بنانشان صحيح منزل_store = (بنانشان صحيح) -1) ثابت
  { أرجع as_array (len).bfind (x, i, not_مليو, منزل_store); }
};


/* An array with a number of elements. */
سانچو <قسم_نالو Type, قسم_نالو LenType>
بنية Array_جو
{
  تعريفنوع Type شي_t;
  ساكن constexpr بنانشان شي_سائيز = hb_static_سائيز (Type);

  HB_DELETE_CREATE_COPY_سونپيو (Array_جو);

  ثابت Type& عامل [] (صحيح i_) ثابت
  {
    بنانشان صحيح i = (بنانشان صحيح) i_;
    إذا (unlikely (i >= len)) أرجع Null (Type);
    hb_barrier ();
    أرجع arrayZ[i];
  }
  Type& عامل [] (صحيح i_)
  {
    بنانشان صحيح i = (بنانشان صحيح) i_;
    إذا (unlikely (i >= len)) أرجع Crap (Type);
    hb_barrier ();
    أرجع arrayZ[i];
  }

  size_t get_سائيز () ثابت
  { أرجع hb_unsigned_mul_جوڙيو_saturate (len, Type::static_سائيز, len.static_سائيز); }

  واضح عامل منطقي () ثابت { أرجع len; }

  فراغ ڪڍو () { len--; }

  hb_array_t<      Type> as_array ()       { أرجع hb_array (arrayZ, len); }
  hb_array_t<ثابت Type> as_array () ثابت { أرجع hb_array (arrayZ, len); }

  /* Iterator. */
  تعريفنوع hb_array_t<ثابت Type>   iter_t;
  تعريفنوع hb_array_t<      Type> writer_t;
    iter_t   iter () ثابت { أرجع as_array (); }
  writer_t writer ()       { أرجع as_array (); }
  عامل   iter_t () ثابت { أرجع   iter (); }
  عامل writer_t ()       { أرجع writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { أرجع arrayZ; }
  ثابت Type *پڇاڙي () ثابت { أرجع arrayZ + len; }

  سانچو <قسم_نالو T>
  Type &lsearch (ثابت T &x, Type &not_مليو = Crap (Type))
  { أرجع *as_array ().lsearch (x, &not_مليو); }
  سانچو <قسم_نالو T>
  ثابت Type &lsearch (ثابت T &x, ثابت Type &not_مليو = Null (Type)) ثابت
  { أرجع *as_array ().lsearch (x, &not_مليو); }
  سانچو <قسم_نالو T>
  منطقي lfind (ثابت T &x, بنانشان صحيح *i = خالي_اشارو,
	      hb_not_مليو_t not_مليو = HB_NOT_مليو_DONT_STORE,
	      بنانشان صحيح منزل_store = (بنانشان صحيح) -1) ثابت
  { أرجع as_array ().lfind (x, i, not_مليو, منزل_store); }

  فراغ qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, بنانشان شيون_len, منطقي clear = سچ)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    c->check_سونپيو (len, شيون_len, HB_SERIALIZE_ERROR_ARRAY_حد_کان_مٿي);
    إذا (unlikely (!c->extend_سائيز (هذا, get_سائيز (), clear))) return_trace (خطأ);
    return_trace (سچ);
  }
  سانچو <قسم_نالو Iterator,
	    hb_requires (hb_is_ذريعو_جو (Iterator, Type))>
  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, Iterator شيون)
  {
    TRACE_SERIALIZE (هذا);
    بنانشان ڳڻپ = hb_len (شيون);
    إذا (unlikely (!serialize (c, ڳڻپ, خطأ))) return_trace (خطأ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    لكل (بنانشان i = 0; i < ڳڻپ; i++, ++شيون)
      arrayZ[i] = *شيون;
    return_trace (سچ);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (هذا);
    len++;
    إذا (unlikely (!len || !c->extend (هذا)))
    {
      len--;
      return_trace (خالي_اشارو);
    }
    return_trace (&arrayZ[len - 1]);
  }

  Array_جو* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار *ٻاهريون = c->شروعات_embed (هذا);
    إذا (unlikely (!c->extend_min (ٻاهريون))) return_trace (خالي_اشارو);
    c->check_سونپيو (ٻاهريون->len, len, HB_SERIALIZE_ERROR_ARRAY_حد_کان_مٿي);
    إذا (unlikely (!as_array ().copy (c))) return_trace (خالي_اشارو);
    return_trace (ٻاهريون);
  }

  سانچو <قسم_نالو ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (سچ);
    hb_barrier ();
    بنانشان صحيح ڳڻپ = len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (خطأ);
    return_trace (سچ);
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
  DEFINE_سائيز_ARRAY (حجم (LenType), arrayZ);
};
سانچو <قسم_نالو Type> استعمال Array16_جو = Array_جو<Type, HBUINT16>;
سانچو <قسم_نالو Type> استعمال Array24_جو = Array_جو<Type, HBUINT24>;
سانچو <قسم_نالو Type> استعمال Array32_جو = Array_جو<Type, HBUINT32>;
استعمال PString = Array_جو<HBUINT8, HBUINT8>;

/* Array of Offset's */
سانچو <قسم_نالو Type> استعمال Array8_جو_Offset24_منزل = Array_جو<وٿي_منزل<Type, HBUINT24>, HBUINT8>;
سانچو <قسم_نالو Type> استعمال Array16_جو_Offset16_منزل = Array_جو<وٿي_منزل<Type, HBUINT16>, HBUINT16>;
سانچو <قسم_نالو Type> استعمال Array16_جو_Offset32_منزل = Array_جو<وٿي_منزل<Type, HBUINT32>, HBUINT16>;
سانچو <قسم_نالو Type> استعمال Array32_جو_Offset32_منزل = Array_جو<وٿي_منزل<Type, HBUINT32>, HBUINT32>;

/* Array of offsets relative to the beginning of the array itself. */
سانچو <قسم_نالو Type, قسم_نالو وٿي_Type>
بنية List16_جو_وٿي_منزل : Array_جو<وٿي_منزل<Type, وٿي_Type>, HBUINT16>
{
  ثابت Type& عامل [] (صحيح i_) ثابت
  {
    بنانشان صحيح i = (بنانشان صحيح) i_;
    إذا (unlikely (i >= هذا->len)) أرجع Null (Type);
    hb_barrier ();
    أرجع هذا+هذا->arrayZ[i];
  }
  ثابت Type& عامل [] (صحيح i_)
  {
    بنانشان صحيح i = (بنانشان صحيح) i_;
    إذا (unlikely (i >= هذا->len)) أرجع Crap (Type);
    hb_barrier ();
    أرجع هذا+هذا->arrayZ[i];
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    بنية List16_جو_وٿي_منزل *ٻاهريون = c->serializer->embed (*هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);
    بنانشان صحيح ڳڻپ = هذا->len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      ٻاهريون->arrayZ[i].serialize_subset (c, هذا->arrayZ[i], هذا, ٻاهريون);
    return_trace (سچ);
  }

  سانچو <قسم_نالو ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace ((Array16_جو<وٿي_منزل<Type, وٿي_Type>>::sanitize (c, هذا, std::forward<Ts> (ds)...)));
  }
};

سانچو <قسم_نالو Type>
استعمال List16_جو_Offset16_منزل = List16_جو_وٿي_منزل<Type, HBUINT16>;

/* An array starting at second element. */
سانچو <قسم_نالو Type, قسم_نالو LenType>
بنية Headless_Array_جو
{
  ساكن constexpr بنانشان شي_سائيز = Type::static_سائيز;

  HB_DELETE_CREATE_COPY_سونپيو (Headless_Array_جو);

  ثابت Type& عامل [] (صحيح i_) ثابت
  {
    بنانشان صحيح i = (بنانشان صحيح) i_;
    إذا (unlikely (i >= lenP1 || !i)) أرجع Null (Type);
    hb_barrier ();
    أرجع arrayZ[i-1];
  }
  Type& عامل [] (صحيح i_)
  {
    بنانشان صحيح i = (بنانشان صحيح) i_;
    إذا (unlikely (i >= lenP1 || !i)) أرجع Crap (Type);
    hb_barrier ();
    أرجع arrayZ[i-1];
  }
  size_t get_سائيز () ثابت
  { أرجع hb_unsigned_mul_جوڙيو_saturate (get_ڊيگهائي (), Type::static_سائيز, lenP1.static_سائيز); }

  بنانشان get_ڊيگهائي () ثابت { أرجع lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { أرجع hb_array (arrayZ, get_ڊيگهائي ()); }
  hb_array_t<ثابت Type> as_array () ثابت { أرجع hb_array (arrayZ, get_ڊيگهائي ()); }

  /* Iterator. */
  تعريفنوع hb_array_t<ثابت Type>   iter_t;
  تعريفنوع hb_array_t<      Type> writer_t;
    iter_t   iter () ثابت { أرجع as_array (); }
  writer_t writer ()       { أرجع as_array (); }
  عامل   iter_t () ثابت { أرجع   iter (); }
  عامل writer_t ()       { أرجع writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { أرجع arrayZ; }
  ثابت Type *پڇاڙي () ثابت { أرجع arrayZ + get_ڊيگهائي (); }

  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, بنانشان صحيح شيون_len, منطقي clear = سچ)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    c->check_سونپيو (lenP1, شيون_len + 1, HB_SERIALIZE_ERROR_ARRAY_حد_کان_مٿي);
    إذا (unlikely (!c->extend_سائيز (هذا, get_سائيز (), clear))) return_trace (خطأ);
    return_trace (سچ);
  }
  سانچو <قسم_نالو Iterator,
	    hb_requires (hb_is_ذريعو_جو (Iterator, Type))>
  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, Iterator شيون)
  {
    TRACE_SERIALIZE (هذا);
    بنانشان ڳڻپ = hb_len (شيون);
    إذا (unlikely (!serialize (c, ڳڻپ, خطأ))) return_trace (خطأ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    لكل (بنانشان i = 0; i < ڳڻپ; i++, ++شيون)
      arrayZ[i] = *شيون;
    return_trace (سچ);
  }

  سانچو <قسم_نالو ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (سچ);
    hb_barrier ();
    بنانشان صحيح ڳڻپ = get_ڊيگهائي ();
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (خطأ);
    return_trace (سچ);
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
  DEFINE_سائيز_ARRAY (حجم (LenType), arrayZ);
};
سانچو <قسم_نالو Type> استعمال Headless_Array16_جو = Headless_Array_جو<Type, HBUINT16>;

/* An array storing length-1. */
سانچو <قسم_نالو Type, قسم_نالو LenType=HBUINT16>
بنية Array_جو_M1
{
  HB_DELETE_CREATE_COPY_سونپيو (Array_جو_M1);

  ثابت Type& عامل [] (صحيح i_) ثابت
  {
    بنانشان صحيح i = (بنانشان صحيح) i_;
    إذا (unlikely (i > lenM1)) أرجع Null (Type);
    hb_barrier ();
    أرجع arrayZ[i];
  }
  Type& عامل [] (صحيح i_)
  {
    بنانشان صحيح i = (بنانشان صحيح) i_;
    إذا (unlikely (i > lenM1)) أرجع Crap (Type);
    hb_barrier ();
    أرجع arrayZ[i];
  }
  size_t get_سائيز () ثابت
  { أرجع hb_unsigned_mul_جوڙيو_saturate (lenM1 + 1, Type::static_سائيز, lenM1.static_سائيز); }

  سانچو <قسم_نالو ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (سچ);
    hb_barrier ();
    بنانشان صحيح ڳڻپ = lenM1 + 1;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (خطأ);
    return_trace (سچ);
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
  DEFINE_سائيز_ARRAY (حجم (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
سانچو <قسم_نالو Type, قسم_نالو LenType>
بنية Sorted_Array_جو : Array_جو<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { أرجع hb_sorted_array (هذا->arrayZ, هذا->len); }
  hb_sorted_array_t<ثابت Type> as_array () ثابت { أرجع hb_sorted_array (هذا->arrayZ, هذا->len); }

  /* Iterator. */
  تعريفنوع hb_sorted_array_t<ثابت Type>   iter_t;
  تعريفنوع hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () ثابت { أرجع as_array (); }
  writer_t writer ()       { أرجع as_array (); }
  عامل   iter_t () ثابت { أرجع   iter (); }
  عامل writer_t ()       { أرجع writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { أرجع هذا->arrayZ; }
  ثابت Type *پڇاڙي () ثابت { أرجع هذا->arrayZ + هذا->len; }

  منطقي serialize (hb_serialize_context_t *c, بنانشان صحيح شيون_len)
  {
    TRACE_SERIALIZE (هذا);
    منطقي ret = Array_جو<Type, LenType>::serialize (c, شيون_len);
    return_trace (ret);
  }
  سانچو <قسم_نالو Iterator,
	    hb_requires (hb_is_sorted_ذريعو_جو (Iterator, Type))>
  منطقي serialize (hb_serialize_context_t *c, Iterator شيون)
  {
    TRACE_SERIALIZE (هذا);
    منطقي ret = Array_جو<Type, LenType>::serialize (c, شيون);
    return_trace (ret);
  }

  Sorted_Array_جو* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    Sorted_Array_جو* ٻاهريون = reinterpret_cast<Sorted_Array_جو *> (Array_جو<Type, LenType>::copy (c));
    return_trace (ٻاهريون);
  }

  سانچو <قسم_نالو T>
  Type &bsearch (ثابت T &x, Type &not_مليو = Crap (Type))
  { أرجع *as_array ().bsearch (x, &not_مليو); }
  سانچو <قسم_نالو T>
  ثابت Type &bsearch (ثابت T &x, ثابت Type &not_مليو = Null (Type)) ثابت
  { أرجع *as_array ().bsearch (x, &not_مليو); }
  سانچو <قسم_نالو T>
  منطقي bfind (ثابت T &x, بنانشان صحيح *i = خالي_اشارو,
	      hb_not_مليو_t not_مليو = HB_NOT_مليو_DONT_STORE,
	      بنانشان صحيح منزل_store = (بنانشان صحيح) -1) ثابت
  { أرجع as_array ().bfind (x, i, not_مليو, منزل_store); }
};

سانچو <قسم_نالو Type> استعمال Sorted_Array16_جو = Sorted_Array_جو<Type, HBUINT16>;
سانچو <قسم_نالو Type> استعمال Sorted_Array24_جو = Sorted_Array_جو<Type, HBUINT24>;
سانچو <قسم_نالو Type> استعمال Sorted_Array32_جو = Sorted_Array_جو<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

سانچو <قسم_نالو LenType=HBUINT16>
بنية Bin_ڳولا_سرو
{
  عامل uint32_t () ثابت { أرجع len; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  Bin_ڳولا_سرو& عامل = (بنانشان صحيح v)
  {
    len = v;
    أكد (len == v);
    داخلا_Selector = hb_max (1u, hb_ٻٽو_انگ_storage (v)) - 1;
    ڳولا_Range = 16 * (1u << داخلا_Selector);
    range_سرڪايو = v * 16 > ڳولا_Range
		 ? 16 * v - ڳولا_Range
		 : 0;
    أرجع *هذا;
  }

  محمي:
  LenType	len;
  LenType	ڳولا_Range;
  LenType	داخلا_Selector;
  LenType	range_سرڪايو;

  عام:
  DEFINE_سائيز_STATIC (8);
};

سانچو <قسم_نالو Type, قسم_نالو LenType=HBUINT16>
استعمال Bin_ڳولا_Array_جو = Sorted_Array_جو<Type, Bin_ڳولا_سرو<LenType>>;


بنية Var_Sized_Bin_ڳولا_سرو
{

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16	unit_سائيز;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	ڳولا_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	داخلا_Selector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	range_سرڪايو;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  عام:
  DEFINE_سائيز_STATIC (10);
};

سانچو <قسم_نالو Type>
بنية Var_Sized_Bin_ڳولا_Array_جو
{
  ساكن constexpr بنانشان شي_سائيز = Type::static_سائيز;

  HB_DELETE_CREATE_COPY_سونپيو (Var_Sized_Bin_ڳولا_Array_جو);

  منطقي آخري_is_terminator () ثابت
  {
    إذا (unlikely (!سرو.nUnits)) أرجع خطأ;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    ثابت HBUINT16 *لفظ_2 = &Struct_At_وٿي<HBUINT16> (&بائيٽون_Z, (سرو.nUnits - 1) * سرو.unit_سائيز);
    بنانشان صحيح ڳڻپ = Type::Termination_لفظ_ڳڻپ;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (لفظ_2[i] != 0xFFFFu)
	أرجع خطأ;
    أرجع سچ;
  }

  ثابت Type& عامل [] (صحيح i_) ثابت
  {
    بنانشان صحيح i = (بنانشان صحيح) i_;
    إذا (unlikely (i >= get_ڊيگهائي ())) أرجع Null (Type);
    hb_barrier ();
    أرجع Struct_At_وٿي<Type> (&بائيٽون_Z, i * سرو.unit_سائيز);
  }
  Type& عامل [] (صحيح i_)
  {
    بنانشان صحيح i = (بنانشان صحيح) i_;
    إذا (unlikely (i >= get_ڊيگهائي ())) أرجع Crap (Type);
    hb_barrier ();
    أرجع Struct_At_وٿي<Type> (&بائيٽون_Z, i * سرو.unit_سائيز);
  }
  بنانشان صحيح get_ڊيگهائي () ثابت
  { أرجع سرو.nUnits - آخري_is_terminator (); }
  size_t get_سائيز () ثابت
  { أرجع hb_unsigned_mul_جوڙيو_saturate (سرو.nUnits, سرو.unit_سائيز, سرو.static_سائيز); }

  سانچو <قسم_نالو ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (سچ);
    hb_barrier ();
    بنانشان صحيح ڳڻپ = get_ڊيگهائي ();
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (unlikely (!(*هذا)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (خطأ);
    return_trace (سچ);
  }

  سانچو <قسم_نالو T>
  ثابت Type *bsearch (ثابت T &key) ثابت
  {
    بنانشان pos;
    أرجع hb_bsearch_impl (&pos,
			    key,
			    (ثابت فراغ *) بائيٽون_Z,
			    get_ڊيگهائي (),
			    سرو.unit_سائيز,
			    hb_cmp_طريقو<T, Type>)
	   ? (ثابت Type *) (((ثابت حرف *) &بائيٽون_Z) + (pos * سرو.unit_سائيز))
	   : خالي_اشارو;
  }

  خاص:
  منطقي sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (سرو.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_سائيز <= سرو.unit_سائيز &&
		  c->check_range (بائيٽون_Z.arrayZ,
				  سرو.nUnits,
				  سرو.unit_سائيز));
  }

  محمي:
  Var_Sized_Bin_ڳولا_سرو	سرو;
  Unsized_Array_جو<HBUINT8>	بائيٽون_Z;
  عام:
  DEFINE_سائيز_ARRAY (10, بائيٽون_Z);
};


/* CFF INDEX */

سانچو <قسم_نالو ڳڻپ_3>
بنية CFFIndex
{
  بنانشان صحيح وٿي_array_سائيز () ثابت
  { أرجع off_سائيز * (ڳڻپ + 1); }

  سانچو <قسم_نالو Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Iterable &iterable,
		  ثابت بنانشان *p_معلومات_سائيز = خالي_اشارو,
                  بنانشان min_off_سائيز = 0)
  {
    TRACE_SERIALIZE (هذا);
    بنانشان معلومات_سائيز;
    إذا (p_معلومات_سائيز)
      معلومات_سائيز = *p_معلومات_سائيز;
    وإلا
      ڪل_سائيز (iterable, &معلومات_سائيز);

    خودڪار it = hb_iter (iterable);
    إذا (unlikely (!serialize_سرو (c, +it, معلومات_سائيز, min_off_سائيز))) return_trace (خطأ);
    بنانشان حرف *ret = c->allocate_سائيز<بنانشان حرف> (معلومات_سائيز, خطأ);
    إذا (unlikely (!ret)) return_trace (خطأ);
    بنانشان بچيل = معلومات_سائيز;
    لكل (ثابت خودڪار &_ : +it)
    {
      بنانشان len = _.ڊيگهائي;

      إذا (!len)
	استمر;

      إذا (unlikely (len > بچيل)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (خطأ, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
      }

      بچيل -= len;

      إذا (len <= 1)
      {
	*ret++ = *_.arrayZ;
	استمر;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (سچ);
  }

  سانچو <قسم_نالو Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_سرو (hb_serialize_context_t *c,
			 Iterator it,
			 بنانشان معلومات_سائيز,
                         بنانشان min_off_سائيز = 0)
  {
    TRACE_SERIALIZE (هذا);

    بنانشان off_سائيز_2 = (hb_ٻٽو_انگ_storage (معلومات_سائيز + 1) + 7) / 8;
    off_سائيز_2 = hb_max(min_off_سائيز, off_سائيز_2);

    /* serialize CFFIndex header */
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    هذا->ڳڻپ = hb_len (it);
    إذا (!هذا->ڳڻپ) return_trace (سچ);
    إذا (unlikely (!c->extend (هذا->off_سائيز))) return_trace (خطأ);
    هذا->off_سائيز = off_سائيز_2;
    إذا (unlikely (!c->allocate_سائيز<HBUINT8> (off_سائيز_2 * (هذا->ڳڻپ + 1), خطأ)))
      return_trace (خطأ);

    /* serialize indices */
    بنانشان صحيح وٿي = 1;
    إذا (HB_OPTIMIZE_سائيز_VAL)
    {
      بنانشان صحيح i = 0;
      لكل (ثابت خودڪار &_ : +it)
      {
	سيٽ_وٿي_at (i++, وٿي);
	وٿي += hb_len_جو (_);
      }
      سيٽ_وٿي_at (i, وٿي);
    }
    وإلا
      بدل (off_سائيز_2)
      {
	حالة 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  لكل (ثابت خودڪار &_ : +it)
	  {
	    *p++ = وٿي;
	    وٿي += hb_len_جو (_);
	  }
	  *p = وٿي;
	}
	اكسر;
	حالة 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  لكل (ثابت خودڪار &_ : +it)
	  {
	    *p++ = وٿي;
	    وٿي += hb_len_جو (_);
	  }
	  *p = وٿي;
	}
	اكسر;
	حالة 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  لكل (ثابت خودڪار &_ : +it)
	  {
	    *p++ = وٿي;
	    وٿي += hb_len_جو (_);
	  }
	  *p = وٿي;
	}
	اكسر;
	حالة 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  لكل (ثابت خودڪار &_ : +it)
	  {
	    *p++ = وٿي;
	    وٿي += hb_len_جو (_);
	  }
	  *p = وٿي;
	}
	اكسر;
	افتراضي:
	اكسر;
      }

    أكد (وٿي == معلومات_سائيز + 1);
    return_trace (سچ);
  }

  سانچو <قسم_نالو Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  ساكن بنانشان ڪل_سائيز (ثابت Iterable &iterable, بنانشان *معلومات_سائيز = خالي_اشارو, بنانشان min_off_سائيز = 0)
  {
    خودڪار it = + hb_iter (iterable);
    إذا (!it)
    {
      إذا (معلومات_سائيز) *معلومات_سائيز = 0;
      أرجع min_سائيز;
    }

    بنانشان ڪل = 0;
    لكل (ثابت خودڪار &_ : +it)
      ڪل += hb_len_جو (_);

    إذا (معلومات_سائيز) *معلومات_سائيز = ڪل;

    بنانشان off_سائيز_2 = (hb_ٻٽو_انگ_storage (ڪل + 1) + 7) / 8;
    off_سائيز_2 = hb_max(min_off_سائيز, off_سائيز_2);

    أرجع min_سائيز + HBUINT8::static_سائيز + (hb_len (it) + 1) * off_سائيز_2 + ڪل;
  }

  فراغ سيٽ_وٿي_at (بنانشان صحيح اشاريو_2, بنانشان صحيح وٿي)
  {
    أكد (اشاريو_2 <= ڳڻپ);

    بنانشان صحيح سائيز = off_سائيز;
    ثابت HBUINT8 *p = offsets;
    بدل (سائيز)
    {
      حالة 1: ((HBUINT8  *) p)[اشاريو_2] = وٿي; اكسر;
      حالة 2: ((HBUINT16 *) p)[اشاريو_2] = وٿي; اكسر;
      حالة 3: ((HBUINT24 *) p)[اشاريو_2] = وٿي; اكسر;
      حالة 4: ((HBUINT32 *) p)[اشاريو_2] = وٿي; اكسر;
      افتراضي: أرجع;
    }
  }

  خاص:
  بنانشان صحيح وٿي_at (بنانشان صحيح اشاريو_2) ثابت
  {
    أكد (اشاريو_2 <= ڳڻپ);

    بنانشان صحيح سائيز = off_سائيز;
    ثابت HBUINT8 *p = offsets;
    بدل (سائيز)
    {
      حالة 1: أرجع ((HBUINT8  *) p)[اشاريو_2];
      حالة 2: أرجع ((HBUINT16 *) p)[اشاريو_2];
      حالة 3: أرجع ((HBUINT24 *) p)[اشاريو_2];
      حالة 4: أرجع ((HBUINT32 *) p)[اشاريو_2];
      افتراضي: أرجع 0;
    }
  }

  ثابت بنانشان حرف *معلومات_بنياد () ثابت
  { أرجع (ثابت بنانشان حرف *) هذا + min_سائيز + off_سائيز.static_سائيز - 1 + وٿي_array_سائيز (); }
  عام:

  hb_ubytes_t عامل [] (بنانشان صحيح اشاريو_2) ثابت
  {
    إذا (unlikely (اشاريو_2 >= ڳڻپ)) أرجع hb_ubytes_t ();
    hb_barrier ();
    بنانشان offset0 = وٿي_at (اشاريو_2);
    بنانشان offset1 = وٿي_at (اشاريو_2 + 1);
    إذا (unlikely (offset1 < offset0 || offset1 > وٿي_at (ڳڻپ)))
      أرجع hb_ubytes_t ();
    أرجع hb_ubytes_t (معلومات_بنياد () + offset0, offset1 - offset0);
  }

  size_t get_سائيز () ثابت
  {
    إذا (ڳڻپ)
      أرجع hb_unsigned_جوڙيو_saturate (min_سائيز, off_سائيز.static_سائيز,
				       وٿي_array_سائيز (), وٿي_at (ڳڻپ) - 1);
    أرجع min_سائيز;  /* empty CFFIndex contains count only */
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  (ڳڻپ == 0 || /* empty INDEX */
			   (ڳڻپ < ڳڻپ + 1u &&
			    c->check_struct (&off_سائيز) && off_سائيز >= 1 && off_سائيز <= 4 &&
			    c->check_array (offsets, off_سائيز, ڳڻپ + 1u) &&
			    c->check_range (معلومات_بنياد (), وٿي_at (ڳڻپ))))));
  }

  عام:
  ڳڻپ_3		ڳڻپ;		/* Number of object data. Note there are (count+1) offsets */
  خاص:
  HBUINT8	off_سائيز;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  عام:
  DEFINE_سائيز_MIN (ڳڻپ_3::static_سائيز);
};
تعريفنوع CFFIndex<HBUINT16> CFF1_اشاريو;
تعريفنوع CFFIndex<HBUINT32> CFF2_اشاريو;


/* TupleValues */
بنية Tuple_قدرون
{
  تعداد packed_قدر_flag_t
  {
    قدرون_ARE_ZEROS     = 0x80,
    قدرون_ARE_بائيٽون     = 0x00,
    قدرون_ARE_لفظ     = 0x40,
    قدرون_ARE_LONGS     = 0xC0,
    قدرون_سائيز_نقاب     = 0xC0,
    قدر_لاڳيتو_ڳڻپ_نقاب = 0x3F
  };

  ساكن بنانشان compile_unsafe (hb_array_t<ثابت صحيح> values, /*اندريون*/
				  بنانشان حرف *encoded_بائيٽون /*ٻاهريون*/)
  {
    بنانشان انگ_قدرون_3 = values.ڊيگهائي;
    بنانشان encoded_len = 0;
    بنانشان i = 0;
    طالما (i < انگ_قدرون_3)
    {
      صحيح val = values.arrayZ[i];
      إذا (val == 0)
        encoded_len += encode_قدر_لاڳيتو_as_zeroes (i, encoded_بائيٽون + encoded_len, values);
      وإلا إذا ((int8_t) val == val)
        encoded_len += encode_قدر_لاڳيتو_as_بائيٽون (i, encoded_بائيٽون + encoded_len, values);
      وإلا إذا ((int16_t) val == val)
        encoded_len += encode_قدر_لاڳيتو_as_لفظ (i, encoded_بائيٽون + encoded_len, values);
      وإلا
        encoded_len += encode_قدر_لاڳيتو_as_longs (i, encoded_بائيٽون + encoded_len, values);
    }
    أرجع encoded_len;
  }

  ساكن بنانشان encode_قدر_لاڳيتو_as_zeroes (بنانشان& i,
					      بنانشان حرف *it,
					      hb_array_t<ثابت صحيح> values)
  {
    بنانشان انگ_قدرون_3 = values.ڊيگهائي;
    بنانشان لاڳيتو_ڊيگهائي = 0;
    بنانشان encoded_len = 0;
    طالما (i < انگ_قدرون_3 && values.arrayZ[i] == 0)
    {
      i++;
      لاڳيتو_ڊيگهائي++;
    }

    طالما (لاڳيتو_ڊيگهائي >= 64)
    {
      *it++ = حرف (قدرون_ARE_ZEROS | 63);
      لاڳيتو_ڊيگهائي -= 64;
      encoded_len++;
    }

    إذا (لاڳيتو_ڊيگهائي)
    {
      *it++ = حرف (قدرون_ARE_ZEROS | (لاڳيتو_ڊيگهائي - 1));
      encoded_len++;
    }
    أرجع encoded_len;
  }

  ساكن بنانشان encode_قدر_لاڳيتو_as_بائيٽون (بنانشان &i,
					     بنانشان حرف *it,
					     hb_array_t<ثابت صحيح> values)
  {
    بنانشان شروعات = i;
    بنانشان انگ_قدرون_3 = values.ڊيگهائي;
    طالما (i < انگ_قدرون_3)
    {
      صحيح val = values.arrayZ[i];
      إذا ((int8_t) val != val)
        اكسر;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      إذا (val == 0 && i + 1 < انگ_قدرون_3 && values.arrayZ[i+1] == 0)
        اكسر;

      i++;
    }
    بنانشان لاڳيتو_ڊيگهائي = i - شروعات;

    بنانشان encoded_len = 0;

    طالما (لاڳيتو_ڊيگهائي >= 64)
    {
      *it++ = (قدرون_ARE_بائيٽون | 63);
      encoded_len++;

      لكل (بنانشان j = 0; j < 64; j++)
	it[j] = static_cast<حرف> (values.arrayZ[شروعات + j]);
      it += 64;
      encoded_len += 64;

      شروعات += 64;
      لاڳيتو_ڊيگهائي -= 64;
    }

    إذا (لاڳيتو_ڊيگهائي)
    {
      *it++ = (قدرون_ARE_بائيٽون | (لاڳيتو_ڊيگهائي - 1));
      encoded_len++;

      لكل (بنانشان j = 0; j < لاڳيتو_ڊيگهائي; j++)
        it[j] = static_cast<حرف> (values.arrayZ[شروعات + j]);
      encoded_len += لاڳيتو_ڊيگهائي;
    }

    أرجع encoded_len;
  }

  ساكن بنانشان encode_قدر_لاڳيتو_as_لفظ (بنانشان &i,
					     بنانشان حرف *it,
					     hb_array_t<ثابت صحيح> values)
  {
    بنانشان شروعات = i;
    بنانشان انگ_قدرون_3 = values.ڊيگهائي;
    طالما (i < انگ_قدرون_3)
    {
      صحيح val = values.arrayZ[i];

      إذا ((int16_t) val != val)
        اكسر;

      /* start a new run for a single zero value. */
      إذا (val == 0) اكسر;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      إذا ((int8_t) val == val &&
          i + 1 < انگ_قدرون_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        اكسر;

      i++;
    }

    بنانشان لاڳيتو_ڊيگهائي = i - شروعات;
    بنانشان encoded_len = 0;
    طالما (لاڳيتو_ڊيگهائي >= 64)
    {
      *it++ = (قدرون_ARE_لفظ | 63);
      encoded_len++;

      لكل (بنانشان j = 0; j < 64; j++)
      {
        int16_t قدر_val = values.arrayZ[شروعات + j];
        *it++ = static_cast<حرف> (قدر_val >> 8);
        *it++ = static_cast<حرف> (قدر_val & 0xFF);

        encoded_len += 2;
      }

      شروعات += 64;
      لاڳيتو_ڊيگهائي -= 64;
    }

    إذا (لاڳيتو_ڊيگهائي)
    {
      *it++ = (قدرون_ARE_لفظ | (لاڳيتو_ڊيگهائي - 1));
      encoded_len++;
      طالما (شروعات < i)
      {
        int16_t قدر_val = values.arrayZ[شروعات++];
        *it++ = static_cast<حرف> (قدر_val >> 8);
        *it++ = static_cast<حرف> (قدر_val & 0xFF);

        encoded_len += 2;
      }
    }
    أرجع encoded_len;
  }

  ساكن بنانشان encode_قدر_لاڳيتو_as_longs (بنانشان &i,
					     بنانشان حرف *it,
					     hb_array_t<ثابت صحيح> values)
  {
    بنانشان شروعات = i;
    بنانشان انگ_قدرون_3 = values.ڊيگهائي;
    طالما (i < انگ_قدرون_3)
    {
      صحيح val = values.arrayZ[i];

      إذا ((int16_t) val == val)
        اكسر;

      i++;
    }

    بنانشان لاڳيتو_ڊيگهائي = i - شروعات;
    بنانشان encoded_len = 0;
    طالما (لاڳيتو_ڊيگهائي >= 64)
    {
      *it++ = (قدرون_ARE_LONGS | 63);
      encoded_len++;

      لكل (بنانشان j = 0; j < 64; j++)
      {
        int32_t قدر_val = values.arrayZ[شروعات + j];
        *it++ = static_cast<حرف> (قدر_val >> 24);
        *it++ = static_cast<حرف> (قدر_val >> 16);
        *it++ = static_cast<حرف> (قدر_val >> 8);
        *it++ = static_cast<حرف> (قدر_val & 0xFF);

        encoded_len += 4;
      }

      شروعات += 64;
      لاڳيتو_ڊيگهائي -= 64;
    }

    إذا (لاڳيتو_ڊيگهائي)
    {
      *it++ = (قدرون_ARE_LONGS | (لاڳيتو_ڊيگهائي - 1));
      encoded_len++;
      طالما (شروعات < i)
      {
        int32_t قدر_val = values.arrayZ[شروعات++];
        *it++ = static_cast<حرف> (قدر_val >> 24);
        *it++ = static_cast<حرف> (قدر_val >> 16);
        *it++ = static_cast<حرف> (قدر_val >> 8);
        *it++ = static_cast<حرف> (قدر_val & 0xFF);

        encoded_len += 4;
      }
    }
    أرجع encoded_len;
  }

  سانچو <قسم_نالو T>
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
  ساكن منطقي decompile (ثابت HBUINT8 *&p /* IN/OUT */,
			 hb_ويڪٽر_t<T> &values /* IN/OUT */,
			 ثابت HBUINT8 *پڇاڙي,
			 منطقي consume_all = خطأ,
			 بنانشان شروعات = 0)
  {
    بنانشان i = 0;
    بنانشان ڳڻپ = consume_all ? UINT_MAX : values.ڊيگهائي;
    إذا (consume_all)
      values.alloc ((پڇاڙي - p) / 2);
    طالما (i < ڳڻپ)
    {
      إذا (unlikely (p + 1 > پڇاڙي)) أرجع consume_all;
      بنانشان control = *p++;
      بنانشان لاڳيتو_ڳڻپ = (control & قدر_لاڳيتو_ڳڻپ_نقاب) + 1;
      إذا (consume_all)
      {
        إذا (unlikely (!values.resize_dirty  (values.ڊيگهائي + لاڳيتو_ڳڻپ)))
	  أرجع خطأ;
      }
      بنانشان stop = i + لاڳيتو_ڳڻپ;
      إذا (unlikely (stop > ڳڻپ)) أرجع خطأ;

      بنانشان skip = i < شروعات ? hb_min (شروعات - i, لاڳيتو_ڳڻپ) : 0;
      i += skip;

      إذا ((control & قدرون_سائيز_نقاب) == قدرون_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * حجم (T));
	i = stop;
      }
      وإلا إذا ((control & قدرون_سائيز_نقاب) ==  قدرون_ARE_لفظ)
      {
        إذا (unlikely (p + لاڳيتو_ڳڻپ * HBINT16::static_سائيز > پڇاڙي)) أرجع خطأ;
	p += skip * HBINT16::static_سائيز;
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
        لكل (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_سائيز;
	  values.arrayZ[i + 1] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_سائيز;
	  values.arrayZ[i + 2] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_سائيز;
	  values.arrayZ[i + 3] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_سائيز;
	}
#جيڪڏهن_پورو
        لكل (; i < stop; i++)
        {
          values.arrayZ[i] = * (ثابت HBINT16 *) p;
          p += HBINT16::static_سائيز;
        }
      }
      وإلا إذا ((control & قدرون_سائيز_نقاب) ==  قدرون_ARE_LONGS)
      {
        إذا (unlikely (p + لاڳيتو_ڳڻپ * HBINT32::static_سائيز > پڇاڙي)) أرجع خطأ;
	p += skip * HBINT32::static_سائيز;
        لكل (; i < stop; i++)
        {
          values.arrayZ[i] = * (ثابت HBINT32 *) p;
          p += HBINT32::static_سائيز;
        }
      }
      وإلا إذا ((control & قدرون_سائيز_نقاب) ==  قدرون_ARE_بائيٽون)
      {
        إذا (unlikely (p + لاڳيتو_ڳڻپ > پڇاڙي)) أرجع خطأ;
	p += skip * HBINT8::static_سائيز;
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
	لكل (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (ثابت HBINT8 *) p++;
	}
#جيڪڏهن_پورو
        لكل (; i < stop; i++)
          values.arrayZ[i] = * (ثابت HBINT8 *) p++;
      }
    }
    أرجع سچ;
  }

  ساكن منطقي skip (ثابت بنانشان حرف *&p /* IN/OUT */,
		    ثابت بنانشان حرف *پڇاڙي,
		    بنانشان ڳڻپ)
  {
    بنانشان ڏٺل = 0;
    طالما (ڏٺل < ڳڻپ)
    {
      إذا (unlikely (p >= پڇاڙي)) أرجع خطأ;
      بنانشان control = *p++;
      بنانشان لاڳيتو_ڳڻپ = (control & قدر_لاڳيتو_ڳڻپ_نقاب) + 1;
      إذا (unlikely (لاڳيتو_ڳڻپ > ڳڻپ - ڏٺل)) أرجع خطأ;

      بنانشان ويڪر = 0;
      بدل (control & قدرون_سائيز_نقاب)
      {
	حالة قدرون_ARE_ZEROS: اكسر;
	حالة قدرون_ARE_بائيٽون: ويڪر = HBINT8::static_سائيز; اكسر;
	حالة قدرون_ARE_لفظ: ويڪر = HBINT16::static_سائيز; اكسر;
	حالة قدرون_ARE_LONGS: ويڪر = HBINT32::static_سائيز; اكسر;
      }
      إذا (unlikely (بنانشان (پڇاڙي - p) < لاڳيتو_ڳڻپ * ويڪر)) أرجع خطأ;
      p += لاڳيتو_ڳڻپ * ويڪر;
      ڏٺل += لاڳيتو_ڳڻپ;
    }
    أرجع سچ;
  }

  بنية iter_t : hb_iter_with_fallback_t<iter_t, صحيح>
  {
    iter_t (ثابت بنانشان حرف *p_, بنانشان len_)
	    : p (p_), endp (p_ + len_)
    { إذا (likely (ensure_لاڳيتو ())) read_قدر (); }

    خاص:
    ثابت بنانشان حرف *p;
    ثابت بنانشان حرف * ثابت endp;
    صحيح current_قدر = 0;
    نشانوارو لاڳيتو_ڳڻپ = 0;
    بنانشان ويڪر = 0;

    HB_ALWAYS_INLINE
    منطقي ensure_لاڳيتو ()
    {
      إذا (likely (لاڳيتو_ڳڻپ > 0)) أرجع سچ;
      أرجع ensure_لاڳيتو_2 ();
    }
    منطقي ensure_لاڳيتو_2 ()
    {
      إذا (unlikely (p >= endp))
      {
        لاڳيتو_ڳڻپ = 0;
        current_قدر = 0;
	أرجع خطأ;
      }

      بنانشان control = *p++;
      لاڳيتو_ڳڻپ = (control & قدر_لاڳيتو_ڳڻپ_نقاب) + 1;
      ويڪر = control & قدرون_سائيز_نقاب;
      بدل (ويڪر)
      {
        حالة قدرون_ARE_ZEROS: ويڪر = 0; اكسر;
	حالة قدرون_ARE_بائيٽون: ويڪر = HBINT8::static_سائيز;  اكسر;
	حالة قدرون_ARE_لفظ: ويڪر = HBINT16::static_سائيز; اكسر;
	حالة قدرون_ARE_LONGS: ويڪر = HBINT32::static_سائيز; اكسر;
	افتراضي: أكد (خطأ);
      }

      إذا (unlikely (p + لاڳيتو_ڳڻپ * ويڪر > endp))
      {
	لاڳيتو_ڳڻپ = 0;
	current_قدر = 0;
	أرجع خطأ;
      }

      أرجع سچ;
    }
    فراغ read_قدر ()
    {
      بدل (ويڪر)
      {
        حالة 0: current_قدر = 0; اكسر;
	حالة 1: current_قدر = * (ثابت HBINT8  *) p; اكسر;
	حالة 2: current_قدر = * (ثابت HBINT16 *) p; اكسر;
	حالة 4: current_قدر = * (ثابت HBINT32 *) p; اكسر;
      }
      p += ويڪر;
    }

    عام:

    تعريفنوع صحيح __item_t__;
    __item_t__ __item__ () ثابت
    { أرجع current_قدر; }

    منطقي __more__ () ثابت { أرجع لاڳيتو_ڳڻپ || p < endp; }
    فراغ __next__ ()
    {
      لاڳيتو_ڳڻپ--;
      إذا (unlikely (!ensure_لاڳيتو ()))
	أرجع;
      read_قدر ();
    }
    فراغ __forward__ (بنانشان n)
    {
      إذا (unlikely (!ensure_لاڳيتو ()))
	أرجع;
      طالما (n)
      {
	بنانشان i = hb_min (n, (بنانشان) لاڳيتو_ڳڻپ);
	لاڳيتو_ڳڻپ -= i;
	n -= i;
	p += (i - 1) * ويڪر;
	إذا (unlikely (!ensure_لاڳيتو ()))
	  أرجع;
	read_قدر ();
      }
    }
    منطقي عامل != (ثابت iter_t& o) ثابت
    { أرجع p != o.p || لاڳيتو_ڳڻپ != o.لاڳيتو_ڳڻپ; }
    iter_t __end__ () ثابت
    {
      iter_t it (endp, 0);
      أرجع it;
    }
  };

  بنية fetcher_t
  {
    fetcher_t (ثابت بنانشان حرف *p_, بنانشان len_)
	      : p (p_), پڇاڙي (p_ + len_) {}

    خاص:
    ثابت بنانشان حرف *p;
    ثابت بنانشان حرف * ثابت پڇاڙي;
    نشانوارو لاڳيتو_ڳڻپ = 0;
    بنانشان ويڪر = 0;

    HB_ALWAYS_INLINE
    منطقي ensure_لاڳيتو ()
    {
      إذا (likely (لاڳيتو_ڳڻپ > 0)) أرجع سچ;
      أرجع ensure_لاڳيتو_2 ();
    }

    منطقي ensure_لاڳيتو_2 ()
    {
      إذا (unlikely (p >= پڇاڙي))
      {
        لاڳيتو_ڳڻپ = 0;
	أرجع خطأ;
      }

      بنانشان control = *p++;
      لاڳيتو_ڳڻپ = (control & قدر_لاڳيتو_ڳڻپ_نقاب) + 1;
      ويڪر = control & قدرون_سائيز_نقاب;
      بدل (ويڪر)
      {
        حالة قدرون_ARE_ZEROS: ويڪر = 0; اكسر;
	حالة قدرون_ARE_بائيٽون: ويڪر = HBINT8::static_سائيز;  اكسر;
	حالة قدرون_ARE_لفظ: ويڪر = HBINT16::static_سائيز; اكسر;
	حالة قدرون_ARE_LONGS: ويڪر = HBINT32::static_سائيز; اكسر;
	افتراضي: أكد (خطأ);
      }

      إذا (unlikely (p + لاڳيتو_ڳڻپ * ويڪر > پڇاڙي))
      {
	لاڳيتو_ڳڻپ = 0;
	أرجع خطأ;
      }

      أرجع سچ;
    }

    عام:
    فراغ skip (بنانشان n)
    {
      طالما (n)
      {
	إذا (unlikely (!ensure_لاڳيتو ()))
	  أرجع;
	بنانشان i = hb_min (n, (بنانشان) لاڳيتو_ڳڻپ);
	لاڳيتو_ڳڻپ -= i;
	n -= i;
	p += i * ويڪر;
      }
    }

    خاص:
    سانچو <منطقي scaled, قسم_نالو T>
    فراغ جوڙيو_منزل (hb_array_t<T> ٻاهريون, T scale = T (1))
    {
      بنانشان n = ٻاهريون.ڊيگهائي;
      T *arrayZ = ٻاهريون.arrayZ;

      لكل (بنانشان i = 0; i < n;)
      {
	إذا (unlikely (!ensure_لاڳيتو ()))
	  اكسر;
	بنانشان ڳڻپ = hb_min (n - i, (بنانشان) لاڳيتو_ڳڻپ);
	بدل (ويڪر)
	{
	  حالة 0:
	  {
	    arrayZ += ڳڻپ;
	    اكسر;
	  }
	  حالة 1:
	  {
	    ثابت خودڪار *pp = (ثابت HBINT8 *) p;
	    بنانشان j = 0;
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
	    لكل (; j + 3 < ڳڻپ; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#جيڪڏهن_پورو
	    لكل (; j < ڳڻپ; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت بنانشان حرف *) pp;
	  }
	  اكسر;
	  حالة 2:
	  {
	    ثابت خودڪار *pp = (ثابت HBINT16 *) p;
	    بنانشان j = 0;
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
	    لكل (; j + 3 < ڳڻپ; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#جيڪڏهن_پورو
	    لكل (; j < ڳڻپ; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت بنانشان حرف *) pp;
	  }
	  اكسر;
	  حالة 4:
	  {
	    ثابت خودڪار *pp = (ثابت HBINT32 *) p;
	    لكل (بنانشان j = 0; j < ڳڻپ; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت بنانشان حرف *) pp;
	  }
	  اكسر;
	}
	لاڳيتو_ڳڻپ -= ڳڻپ;
	i += ڳڻپ;
      }
    }

    عام:
    سانچو <قسم_نالو T>
    فراغ جوڙيو_منزل_2 (hb_array_t<T> ٻاهريون, عائم scale = 1.0f)
    {
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      إذا (خطأ && scale == 1.0f)
        جوڙيو_منزل<خطأ> (ٻاهريون);
      وإلا
#جيڪڏهن_پورو
        جوڙيو_منزل<سچ> (ٻاهريون, (T) scale);
    }
  };
};

بنية TupleList : CFF2_اشاريو
{
  Tuple_قدرون::iter_t عامل [] (بنانشان i) ثابت
  {
    خودڪار بائيٽون = CFF2_اشاريو::عامل [] (i);
    أرجع Tuple_قدرون::iter_t (بائيٽون.arrayZ, بائيٽون.ڊيگهائي);
  }

  Tuple_قدرون::fetcher_t fetcher (بنانشان i) ثابت
  {
    خودڪار بائيٽون = CFF2_اشاريو::عامل [] (i);
    أرجع Tuple_قدرون::fetcher_t (بائيٽون.arrayZ, بائيٽون.ڊيگهائي);
  }
};


// Alignment

سانچو <بنانشان صحيح alignment>
بنية Align
{
  size_t get_سائيز (ثابت فراغ *بنياد) ثابت
  {
    بنانشان وٿي = (ثابت حرف *) هذا - (ثابت حرف *) بنياد;
    أرجع (alignment - وٿي) & (alignment - 1);
  }

  عام:
  DEFINE_سائيز_MIN (0);
};



} /* namespace OT */


#جيڪڏهن_پورو /* HB_OPEN_TYPE_HH */
