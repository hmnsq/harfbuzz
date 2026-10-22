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

#جے_بنا_تعریف HB_OPEN_TYPE_HH
#تعریف_کرو HB_OPEN_TYPE_HH

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
سانچا <منطقي BE,
	  قسم_ناں Type,
	  بنانشانی صحيح ناپ_2 = حجم (Type)>
بنية عدد_Type
{
  تعريفنوع Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  تعريفنوع قسم_ناں std::conditional<std::is_integral<Type>::value && حجم (Type) <= حجم(صحيح),
				     قسم_ناں std::conditional<std::is_signed<Type>::value, نشانیوالا, بنانشانی>::type,
				     Type>::type WideType;

  عدد_Type () = افتراضي;
  واضح constexpr عدد_Type (Type V) : v {V} {}
  عدد_Type& عامل = (Type V) { v = V; أرجع *هذا; }

  عامل WideType () ثابت { أرجع v; }

  منطقي عامل == (ثابت عدد_Type &o) ثابت { أرجع (Type) v == (Type) o.v; }
  منطقي عامل != (ثابت عدد_Type &o) ثابت { أرجع !(*هذا == o); }

  عدد_Type& عامل += (WideType گنتی) { *هذا = *هذا + گنتی; أرجع *هذا; }
  عدد_Type& عامل -= (WideType گنتی) { *هذا = *هذا - گنتی; أرجع *هذا; }
  عدد_Type& عامل ++ () { *هذا += 1; أرجع *هذا; }
  عدد_Type& عامل -- () { *هذا -= 1; أرجع *هذا; }
  عدد_Type عامل ++ (صحيح) { عدد_Type c (*هذا); ++*هذا; أرجع c; }
  عدد_Type عامل -- (صحيح) { عدد_Type c (*هذا); --*هذا; أرجع c; }

  uint32_t hash () ثابت { أرجع hb_array ((ثابت حرف *) &v, حجم (v)).hash (); }
  HB_اندرونی ساكن صحيح cmp (ثابت عدد_Type *a, ثابت عدد_Type *b)
  { أرجع b->cmp (*a); }
  HB_اندرونی ساكن صحيح cmp (ثابت فراغ *a, ثابت فراغ *b)
  {
    عدد_Type *pa = (عدد_Type *) a;
    عدد_Type *pb = (عدد_Type *) b;

    أرجع pb->cmp (*pa);
  }
  سانچا <قسم_ناں Type2,
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
  قسم_ناں std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, ناپ_2>,
			    HBFloat<BE, Type, ناپ_2>>::type v;
  عام:
  DEFINE_ناپ_STATIC (ناپ_2);
};

تعريفنوع عدد_Type<سچ, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
تعريفنوع عدد_Type<سچ, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
تعريفنوع عدد_Type<سچ, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
تعريفنوع عدد_Type<سچ, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
تعريفنوع عدد_Type<سچ, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
تعريفنوع عدد_Type<سچ, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
تعريفنوع عدد_Type<سچ, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
تعريفنوع عدد_Type<سچ, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
تعريفنوع عدد_Type<سچ, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

تعريفنوع عدد_Type<خطأ, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
تعريفنوع عدد_Type<خطأ, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
تعريفنوع عدد_Type<خطأ, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
تعريفنوع عدد_Type<خطأ, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
تعريفنوع عدد_Type<خطأ, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
تعريفنوع عدد_Type<خطأ, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

تعريفنوع عدد_Type<سچ,  عائم>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
تعريفنوع عدد_Type<سچ,  مزدوج> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
تعريفنوع عدد_Type<خطأ, عائم>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
تعريفنوع عدد_Type<خطأ, مزدوج> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
بنية HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& عامل = (uint16_t i ) { HBUINT16::عامل= (i); أرجع *هذا; }
  عام:
  DEFINE_ناپ_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
بنية HBUINT32VAR
{
  size_t get_ناپ () ثابت
  {
    بنانشانی b0 = v[0];
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

  ساكن size_t get_ناپ (uint32_t v)
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
		  c->check_range (v, get_ناپ ()));
  }

  عامل uint32_t () ثابت
  {
    بنانشانی b0 = v[0];
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

  ساكن بنانشانی serialize_unsafe (بنانشانی حرف *buf, uint32_t v)
  {
    بنانشانی len = get_ناپ (v);
    بنانشانی حرف *p = buf + len;
    لكل (بنانشانی i = 0; i < len; i++)
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
    بنانشانی len = get_ناپ (v);
    بنانشانی حرف *buf = c->allocate_ناپ<بنانشانی حرف> (len, خطأ);
    إذا (unlikely (!buf))
      أرجع خطأ;

    serialize_unsafe (buf, v);
    أرجع سچ;
  }

  محمي:
  بنانشانی حرف v[5];

  عام:
  DEFINE_ناپ_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
تعريفنوع HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
تعريفنوع HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
تعريفنوع HBUINT16 UFWORD;

سانچا <قسم_ناں Type, بنانشانی fraction_بٹ>
بنية HBFixed : Type
{
  ساكن constexpr عائم mult = 1.f / (1 << fraction_بٹ);
  static_assert (Type::static_ناپ * 8 > fraction_بٹ, "");

  عامل نشانیوالا () ثابت = مٹاؤ;
  عامل بنانشانی () ثابت = مٹاؤ;
  واضح عامل عائم () ثابت { أرجع منزل_float (); }
  قسم_ناں Type::type منزل_int () ثابت { أرجع Type::v; }
  فراغ سیٹ_int (قسم_ناں Type::type i ) { Type::v = i; }
  عائم منزل_float (عائم وتھ = 0) ثابت  { أرجع ((int32_t) Type::v + وتھ) * mult; }
  فراغ سیٹ_float (عائم f) { Type::v = roundf (f / mult); }
  عام:
  DEFINE_ناپ_STATIC (Type::static_ناپ);
};

/* 16-bit signed fixed number with the low 14 bits of fraction (2.14). */
ورتوں F2DOT14 = HBFixed<HBINT16, 14>;
ورتوں F4DOT12 = HBFixed<HBINT16, 12>;
ورتوں F6DOT10 = HBFixed<HBINT16, 10>;

/* 32-bit signed fixed-point number (16.16). */
ورتوں F16DOT16 = HBFixed<HBINT32, 16>;

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
  DEFINE_ناپ_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
بنية ٹیگ : HBUINT32
{
  ٹیگ& عامل = (hb_ٹیگ_t i) { HBUINT32::عامل= (i); أرجع *هذا; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  عامل ثابت حرف* () ثابت { أرجع reinterpret_cast<ثابت حرف *> (هذا); }
  عامل حرف* ()             { أرجع reinterpret_cast<حرف *> (هذا); }
  عام:
  DEFINE_ناپ_STATIC (4);
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
بنية اشاریہ : HBUINT16 {
  ساكن constexpr بنانشانی NOT_لبھیا_اشاریہ = 0xFFFFu;
  اشاریہ& عامل = (uint16_t i) { HBUINT16::عامل= (i); أرجع *هذا; }
};
DECLARE_NULL_NAMESPACE_بائٹاں (OT, اشاریہ);

تعريفنوع اشاریہ NameID;

بنية VarIdx : HBUINT32 {
  ساكن constexpr بنانشانی NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ, "");
  VarIdx& عامل = (uint32_t i) { HBUINT32::عامل= (i); أرجع *هذا; }
};
DECLARE_NULL_NAMESPACE_بائٹاں (OT, VarIdx);

/* Offset, Null offset = 0 */
سانچا <قسم_ناں Type, منطقي has_null=سچ>
بنية وتھ_2 : Type
{
  وتھ_2& عامل = (قسم_ناں Type::type i) { Type::عامل= (i); أرجع *هذا; }

  تعريفنوع Type type;

  منطقي is_null () ثابت { أرجع has_null && 0 == *هذا; }

  عام:
  DEFINE_ناپ_STATIC (حجم (Type));
};

تعريفنوع وتھ_2<HBUINT16> Offset16;
تعريفنوع وتھ_2<HBUINT24> Offset24;
تعريفنوع وتھ_2<HBUINT32> Offset32;


/* CheckSum */
بنية Check_جوڑ : HBUINT32
{
  Check_جوڑ& عامل = (uint32_t i) { HBUINT32::عامل= (i); أرجع *هذا; }

  /* This is reference implementation from the spec. */
  ساكن uint32_t Calc_جدول_Checksum (ثابت HBUINT32 *جدول_3, uint32_t لمائی_3)
  {
    uint32_t Sum = 0L;
    أكد (0 == (لمائی_3 & 3));
    ثابت HBUINT32 *سرا_Ptr = جدول_3 + لمائی_3 / HBUINT32::static_ناپ;

    طالما (جدول_3 < سرا_Ptr)
      Sum += *جدول_3++;
    أرجع Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  فراغ سیٹ_for_انکڑے (ثابت فراغ *انکڑے, بنانشانی صحيح لمائی)
  { *هذا = Calc_جدول_Checksum ((ثابت HBUINT32 *) انکڑے, لمائی); }

  عام:
  DEFINE_ناپ_STATIC (4);
};


/*
 * Version Numbers
 */

سانچا <قسم_ناں FixedType=HBUINT16>
بنية Fixed_نسخہ
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
  DEFINE_ناپ_STATIC (2 * حجم (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

سانچا <قسم_ناں Type, منطقي has_null>
بنية _hb_has_null
{
  ساكن ثابت Type *get_null () { أرجع خالی_سوچک; }
  ساكن Type *get_crap ()       { أرجع خالی_سوچک; }
};
سانچا <قسم_ناں Type>
بنية _hb_has_null<Type, سچ>
{
  ساكن ثابت Type *get_null () { أرجع &Null (Type); }
  ساكن       Type *get_crap () { أرجع &Crap (Type); }
};

سانچا <قسم_ناں Type, قسم_ناں وتھ_Type, قسم_ناں ادھار_Type=فراغ, منطقي has_null=سچ>
بنية وتھ_منزل : وتھ_2<وتھ_Type, has_null>
{
  ورتوں نشانہ_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == خطأ ||
		 (hb_has_null_ناپ_2 (Type) || !hb_has_min_ناپ_2 (Type)), "");

  HB_DELETE_COPY_سونپو (وتھ_منزل);
  وتھ_منزل () = افتراضي;

  وتھ_منزل& عامل = (قسم_ناں وتھ_Type::type i) { وتھ_Type::عامل= (i); أرجع *هذا; }

  ثابت Type& عامل () (ثابت فراغ *ادھار) ثابت
  {
    إذا (unlikely (هذا->is_null ())) أرجع *_hb_has_null<Type, has_null>::get_null ();
    أرجع Struct_At_وتھ<ثابت Type> (ادھار, *هذا);
  }
  Type& عامل () (فراغ *ادھار) ثابت
  {
    إذا (unlikely (هذا->is_null ())) أرجع *_hb_has_null<Type, has_null>::get_crap ();
    أرجع Struct_At_وتھ<Type> (ادھار, *هذا);
  }

  سانچا <قسم_ناں ادھار_3,
	    hb_enable_if (hb_is_convertible (ثابت ادھار_3, ثابت ادھار_Type *))>
  یار ثابت Type& عامل + (ثابت ادھار_3 &ادھار, ثابت وتھ_منزل &وتھ) { أرجع وتھ ((ثابت فراغ *) ادھار); }
  سانچا <قسم_ناں ادھار_3,
	    hb_enable_if (hb_is_convertible (ثابت ادھار_3, ثابت ادھار_Type *))>
  یار ثابت Type& عامل + (ثابت وتھ_منزل &وتھ, ثابت ادھار_3 &ادھار) { أرجع وتھ ((ثابت فراغ *) ادھار); }
  سانچا <قسم_ناں ادھار_3,
	    hb_enable_if (hb_is_convertible (ادھار_3, ادھار_Type *))>
  یار Type& عامل + (ادھار_3 &&ادھار, وتھ_منزل &وتھ) { أرجع وتھ ((فراغ *) ادھار); }
  سانچا <قسم_ناں ادھار_3,
	    hb_enable_if (hb_is_convertible (ادھار_3, ادھار_Type *))>
  یار Type& عامل + (وتھ_منزل &وتھ, ادھار_3 &&ادھار) { أرجع وتھ ((فراغ *) ادھار); }


  سانچا <قسم_ناں ادھار_3, قسم_ناں ...Ts>
  منطقي serialize_subset (hb_subset_context_t *c, ثابت وتھ_منزل& src,
			 ثابت ادھار_3 *src_ادھار, Ts&&... ds)
  {
    *هذا = 0;
    إذا (src.is_null ())
      أرجع خطأ;

    خودکار *s = c->serializer;

    s->push ();

    منطقي ret = c->dispatch (src_ادھار+src, std::forward<Ts> (ds)...);

    إذا (ret || !has_null)
      s->جوڑو_link (*هذا, s->نکالو_pack ());
    وإلا
      s->نکالو_discard ();

    أرجع ret;
  }


  سانچا <قسم_ناں ...Ts>
  منطقي serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *هذا = 0;

    Type* obj = c->push<Type> ();
    منطقي ret = obj->serialize (c, std::forward<Ts> (ds)...);

    إذا (ret)
      c->جوڑو_link (*هذا, c->نکالو_pack ());
    وإلا
      c->نکالو_discard ();

    أرجع ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  سانچا <قسم_ناں ...Ts>
  منطقي serialize_copy (hb_serialize_context_t *c, ثابت وتھ_منزل& src,
		       ثابت فراغ *src_ادھار, بنانشانی dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *هذا = 0;
    إذا (src.is_null ())
      أرجع خطأ;

    c->push ();

    منطقي ret = c->copy (src_ادھار+src, std::forward<Ts> (ds)...);

    c->جوڑو_link (*هذا, c->نکالو_pack (), whence, dst_bias);

    أرجع ret;
  }

  منطقي serialize_copy (hb_serialize_context_t *c, ثابت وتھ_منزل& src,
		       ثابت فراغ *src_ادھار, بنانشانی dst_bias = 0)
  { أرجع serialize_copy (c, src, src_ادھار, dst_bias, hb_serialize_context_t::سر); }

  منطقي sanitize_shallow (hb_sanitize_context_t *c, ثابت ادھار_Type *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    إذا (unlikely ((ثابت حرف *) ادھار + (بنانشانی) *هذا < (ثابت حرف *) ادھار)) return_trace (خطأ);
    return_trace (سچ);
  }

  سانچا <قسم_ناں ...Ts>
#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  منطقي sanitize (hb_sanitize_context_t *c, ثابت ادھار_Type *ادھار, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (sanitize_shallow (c, ادھار) &&
		  hb_barrier () &&
		  (هذا->is_null () ||
		   c->dispatch (Struct_At_وتھ<Type> (ادھار, *هذا), std::forward<Ts> (ds)...)));
  }

  DEFINE_ناپ_STATIC (حجم (وتھ_Type));
};
/* Partial specializations. */
سانچا <قسم_ناں Type, قسم_ناں ادھار_Type=فراغ, منطقي has_null=سچ> ورتوں Offset16_منزل = وتھ_منزل<Type, HBUINT16, ادھار_Type, has_null>;
سانچا <قسم_ناں Type, قسم_ناں ادھار_Type=فراغ, منطقي has_null=سچ> ورتوں Offset24_منزل = وتھ_منزل<Type, HBUINT24, ادھار_Type, has_null>;
سانچا <قسم_ناں Type, قسم_ناں ادھار_Type=فراغ, منطقي has_null=سچ> ورتوں Offset32_منزل = وتھ_منزل<Type, HBUINT32, ادھار_Type, has_null>;

سانچا <قسم_ناں Type, قسم_ناں وتھ_Type, قسم_ناں ادھار_Type=فراغ> ورتوں NNOffset_منزل = وتھ_منزل<Type, وتھ_Type, ادھار_Type, خطأ>;
سانچا <قسم_ناں Type, قسم_ناں ادھار_Type=فراغ> ورتوں NNOffset16_منزل = Offset16_منزل<Type, ادھار_Type, خطأ>;
سانچا <قسم_ناں Type, قسم_ناں ادھار_Type=فراغ> ورتوں NNOffset24_منزل = Offset24_منزل<Type, ادھار_Type, خطأ>;
سانچا <قسم_ناں Type, قسم_ناں ادھار_Type=فراغ> ورتوں NNOffset32_منزل = Offset32_منزل<Type, ادھار_Type, خطأ>;


/*
 * Array Types
 */

سانچا <قسم_ناں Type>
بنية Unsized_Array_دا
{
  تعريفنوع Type شے_t;
  ساكن constexpr بنانشانی شے_ناپ = hb_static_ناپ (Type);

  HB_DELETE_CREATE_COPY_سونپو (Unsized_Array_دا);

  ثابت Type& عامل [] (بنانشانی صحيح i) ثابت
  {
    أرجع arrayZ[i];
  }
  Type& عامل [] (بنانشانی صحيح i)
  {
    أرجع arrayZ[i];
  }

  ساكن size_t get_ناپ (بنانشانی صحيح len)
  { أرجع hb_unsigned_mul_saturate (len, Type::static_ناپ); }

  سانچا <قسم_ناں T> عامل T * () { أرجع arrayZ; }
  سانچا <قسم_ناں T> عامل ثابت T * () ثابت { أرجع arrayZ; }
  hb_array_t<Type> as_array (بنانشانی صحيح len)
  { أرجع hb_array (arrayZ, len); }
  hb_array_t<ثابت Type> as_array (بنانشانی صحيح len) ثابت
  { أرجع hb_array (arrayZ, len); }

  سانچا <قسم_ناں T>
  Type &lsearch (بنانشانی صحيح len, ثابت T &x, Type &not_لبھیا = Crap (Type))
  { أرجع *as_array (len).lsearch (x, &not_لبھیا); }
  سانچا <قسم_ناں T>
  ثابت Type &lsearch (بنانشانی صحيح len, ثابت T &x, ثابت Type &not_لبھیا = Null (Type)) ثابت
  { أرجع *as_array (len).lsearch (x, &not_لبھیا); }
  سانچا <قسم_ناں T>
  منطقي lfind (بنانشانی صحيح len, ثابت T &x, بنانشانی صحيح *i = خالی_سوچک,
	      hb_not_لبھیا_t not_لبھیا = HB_NOT_لبھیا_DONT_STORE,
	      بنانشانی صحيح منزل_store = (بنانشانی صحيح) -1) ثابت
  { أرجع as_array (len).lfind (x, i, not_لبھیا, منزل_store); }

  فراغ qsort (بنانشانی صحيح len, بنانشانی صحيح شروع = 0, بنانشانی صحيح سرا = (بنانشانی صحيح) -1)
  { as_array (len).qsort (شروع, سرا); }

  منطقي serialize (hb_serialize_context_t *c, بنانشانی صحيح اشیاء_len, منطقي clear = سچ)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_ناپ (هذا, get_ناپ (اشیاء_len), clear))) return_trace (خطأ);
    return_trace (سچ);
  }
  سانچا <قسم_ناں Iterator,
	    hb_requires (hb_is_سروت_دا (Iterator, Type))>
  منطقي serialize (hb_serialize_context_t *c, Iterator اشیاء)
  {
    TRACE_SERIALIZE (هذا);
    بنانشانی گنتی = hb_len (اشیاء);
    إذا (unlikely (!serialize (c, گنتی, خطأ))) return_trace (خطأ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    لكل (بنانشانی i = 0; i < گنتی; i++, ++اشیاء)
      arrayZ[i] = *اشیاء;
    return_trace (سچ);
  }

  Unsized_Array_دا* copy (hb_serialize_context_t *c, بنانشانی گنتی) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *باہرلا = c->شروع_embed (هذا);
    إذا (unlikely (!as_array (گنتی).copy (c))) return_trace (خالی_سوچک);
    return_trace (باہرلا);
  }

  سانچا <قسم_ناں ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, بنانشانی صحيح گنتی, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c, گنتی))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (سچ);
    hb_barrier ();
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (خطأ);
    return_trace (سچ);
  }

  منطقي sanitize_shallow (hb_sanitize_context_t *c, بنانشانی صحيح گنتی) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_array (arrayZ, گنتی));
  }

  عام:
  Type		arrayZ[HB_VAR_ARRAY];
  عام:
  DEFINE_ناپ_UNBOUNDED (0);
};

/* Unsized array of offset's */
سانچا <قسم_ناں Type, قسم_ناں وتھ_Type, قسم_ناں ادھار_Type=فراغ, منطقي has_null=سچ>
ورتوں Unsized_Array16_دا_وتھ_منزل = Unsized_Array_دا<وتھ_منزل<Type, وتھ_Type, ادھار_Type, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
سانچا <قسم_ناں Type, قسم_ناں وتھ_Type, قسم_ناں ادھار_Type=فراغ, منطقي has_null=سچ>
بنية Unsized_List_دا_Offset16_منزل : Unsized_Array16_دا_وتھ_منزل<Type, وتھ_Type, ادھار_Type, has_null>
{
  ثابت Type& عامل [] (صحيح i_) ثابت
  {
    بنانشانی صحيح i = (بنانشانی صحيح) i_;
    ثابت وتھ_منزل<Type, وتھ_Type, ادھار_Type, has_null> *p = &هذا->arrayZ[i];
    إذا (unlikely ((ثابت فراغ *) p < (ثابت فراغ *) هذا->arrayZ)) أرجع Null (Type); /* Overflowed. */
    hb_barrier ();
    أرجع هذا+*p;
  }
  Type& عامل [] (صحيح i_)
  {
    بنانشانی صحيح i = (بنانشانی صحيح) i_;
    ثابت وتھ_منزل<Type, وتھ_Type, ادھار_Type, has_null> *p = &هذا->arrayZ[i];
    إذا (unlikely ((ثابت فراغ *) p < (ثابت فراغ *) هذا->arrayZ)) أرجع Crap (Type); /* Overflowed. */
    hb_barrier ();
    أرجع هذا+*p;
  }

  سانچا <قسم_ناں ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, بنانشانی صحيح گنتی, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace ((Unsized_Array16_دا_وتھ_منزل<Type, وتھ_Type, ادھار_Type, has_null>
		   ::sanitize (c, گنتی, هذا, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
سانچا <قسم_ناں Type>
بنية Sorted_Unsized_Array_دا : Unsized_Array_دا<Type>
{
  hb_sorted_array_t<Type> as_array (بنانشانی صحيح len)
  { أرجع hb_sorted_array (هذا->arrayZ, len); }
  hb_sorted_array_t<ثابت Type> as_array (بنانشانی صحيح len) ثابت
  { أرجع hb_sorted_array (هذا->arrayZ, len); }
  عامل hb_sorted_array_t<Type> ()             { أرجع as_array (); }
  عامل hb_sorted_array_t<ثابت Type> () ثابت { أرجع as_array (); }

  سانچا <قسم_ناں T>
  Type &bsearch (بنانشانی صحيح len, ثابت T &x, Type &not_لبھیا = Crap (Type))
  { أرجع *as_array (len).bsearch (x, &not_لبھیا); }
  سانچا <قسم_ناں T>
  ثابت Type &bsearch (بنانشانی صحيح len, ثابت T &x, ثابت Type &not_لبھیا = Null (Type)) ثابت
  { أرجع *as_array (len).bsearch (x, &not_لبھیا); }
  سانچا <قسم_ناں T>
  منطقي bfind (بنانشانی صحيح len, ثابت T &x, بنانشانی صحيح *i = خالی_سوچک,
	      hb_not_لبھیا_t not_لبھیا = HB_NOT_لبھیا_DONT_STORE,
	      بنانشانی صحيح منزل_store = (بنانشانی صحيح) -1) ثابت
  { أرجع as_array (len).bfind (x, i, not_لبھیا, منزل_store); }
};


/* An array with a number of elements. */
سانچا <قسم_ناں Type, قسم_ناں LenType>
بنية Array_دا
{
  تعريفنوع Type شے_t;
  ساكن constexpr بنانشانی شے_ناپ = hb_static_ناپ (Type);

  HB_DELETE_CREATE_COPY_سونپو (Array_دا);

  ثابت Type& عامل [] (صحيح i_) ثابت
  {
    بنانشانی صحيح i = (بنانشانی صحيح) i_;
    إذا (unlikely (i >= len)) أرجع Null (Type);
    hb_barrier ();
    أرجع arrayZ[i];
  }
  Type& عامل [] (صحيح i_)
  {
    بنانشانی صحيح i = (بنانشانی صحيح) i_;
    إذا (unlikely (i >= len)) أرجع Crap (Type);
    hb_barrier ();
    أرجع arrayZ[i];
  }

  size_t get_ناپ () ثابت
  { أرجع hb_unsigned_mul_جوڑو_saturate (len, Type::static_ناپ, len.static_ناپ); }

  واضح عامل منطقي () ثابت { أرجع len; }

  فراغ نکالو () { len--; }

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
  ثابت Type *سرا () ثابت { أرجع arrayZ + len; }

  سانچا <قسم_ناں T>
  Type &lsearch (ثابت T &x, Type &not_لبھیا = Crap (Type))
  { أرجع *as_array ().lsearch (x, &not_لبھیا); }
  سانچا <قسم_ناں T>
  ثابت Type &lsearch (ثابت T &x, ثابت Type &not_لبھیا = Null (Type)) ثابت
  { أرجع *as_array ().lsearch (x, &not_لبھیا); }
  سانچا <قسم_ناں T>
  منطقي lfind (ثابت T &x, بنانشانی صحيح *i = خالی_سوچک,
	      hb_not_لبھیا_t not_لبھیا = HB_NOT_لبھیا_DONT_STORE,
	      بنانشانی صحيح منزل_store = (بنانشانی صحيح) -1) ثابت
  { أرجع as_array ().lfind (x, i, not_لبھیا, منزل_store); }

  فراغ qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, بنانشانی اشیاء_len, منطقي clear = سچ)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    c->check_سونپو (len, اشیاء_len, HB_SERIALIZE_ERROR_ARRAY_حد_توں_ودھ);
    إذا (unlikely (!c->extend_ناپ (هذا, get_ناپ (), clear))) return_trace (خطأ);
    return_trace (سچ);
  }
  سانچا <قسم_ناں Iterator,
	    hb_requires (hb_is_سروت_دا (Iterator, Type))>
  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, Iterator اشیاء)
  {
    TRACE_SERIALIZE (هذا);
    بنانشانی گنتی = hb_len (اشیاء);
    إذا (unlikely (!serialize (c, گنتی, خطأ))) return_trace (خطأ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    لكل (بنانشانی i = 0; i < گنتی; i++, ++اشیاء)
      arrayZ[i] = *اشیاء;
    return_trace (سچ);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (هذا);
    len++;
    إذا (unlikely (!len || !c->extend (هذا)))
    {
      len--;
      return_trace (خالی_سوچک);
    }
    return_trace (&arrayZ[len - 1]);
  }

  Array_دا* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *باہرلا = c->شروع_embed (هذا);
    إذا (unlikely (!c->extend_min (باہرلا))) return_trace (خالی_سوچک);
    c->check_سونپو (باہرلا->len, len, HB_SERIALIZE_ERROR_ARRAY_حد_توں_ودھ);
    إذا (unlikely (!as_array ().copy (c))) return_trace (خالی_سوچک);
    return_trace (باہرلا);
  }

  سانچا <قسم_ناں ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (سچ);
    hb_barrier ();
    بنانشانی صحيح گنتی = len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
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
  DEFINE_ناپ_ARRAY (حجم (LenType), arrayZ);
};
سانچا <قسم_ناں Type> ورتوں Array16_دا = Array_دا<Type, HBUINT16>;
سانچا <قسم_ناں Type> ورتوں Array24_دا = Array_دا<Type, HBUINT24>;
سانچا <قسم_ناں Type> ورتوں Array32_دا = Array_دا<Type, HBUINT32>;
ورتوں PString = Array_دا<HBUINT8, HBUINT8>;

/* Array of Offset's */
سانچا <قسم_ناں Type> ورتوں Array8_دا_Offset24_منزل = Array_دا<وتھ_منزل<Type, HBUINT24>, HBUINT8>;
سانچا <قسم_ناں Type> ورتوں Array16_دا_Offset16_منزل = Array_دا<وتھ_منزل<Type, HBUINT16>, HBUINT16>;
سانچا <قسم_ناں Type> ورتوں Array16_دا_Offset32_منزل = Array_دا<وتھ_منزل<Type, HBUINT32>, HBUINT16>;
سانچا <قسم_ناں Type> ورتوں Array32_دا_Offset32_منزل = Array_دا<وتھ_منزل<Type, HBUINT32>, HBUINT32>;

/* Array of offsets relative to the beginning of the array itself. */
سانچا <قسم_ناں Type, قسم_ناں وتھ_Type>
بنية List16_دا_وتھ_منزل : Array_دا<وتھ_منزل<Type, وتھ_Type>, HBUINT16>
{
  ثابت Type& عامل [] (صحيح i_) ثابت
  {
    بنانشانی صحيح i = (بنانشانی صحيح) i_;
    إذا (unlikely (i >= هذا->len)) أرجع Null (Type);
    hb_barrier ();
    أرجع هذا+هذا->arrayZ[i];
  }
  ثابت Type& عامل [] (صحيح i_)
  {
    بنانشانی صحيح i = (بنانشانی صحيح) i_;
    إذا (unlikely (i >= هذا->len)) أرجع Crap (Type);
    hb_barrier ();
    أرجع هذا+هذا->arrayZ[i];
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    بنية List16_دا_وتھ_منزل *باہرلا = c->serializer->embed (*هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);
    بنانشانی صحيح گنتی = هذا->len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      باہرلا->arrayZ[i].serialize_subset (c, هذا->arrayZ[i], هذا, باہرلا);
    return_trace (سچ);
  }

  سانچا <قسم_ناں ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace ((Array16_دا<وتھ_منزل<Type, وتھ_Type>>::sanitize (c, هذا, std::forward<Ts> (ds)...)));
  }
};

سانچا <قسم_ناں Type>
ورتوں List16_دا_Offset16_منزل = List16_دا_وتھ_منزل<Type, HBUINT16>;

/* An array starting at second element. */
سانچا <قسم_ناں Type, قسم_ناں LenType>
بنية Headless_Array_دا
{
  ساكن constexpr بنانشانی شے_ناپ = Type::static_ناپ;

  HB_DELETE_CREATE_COPY_سونپو (Headless_Array_دا);

  ثابت Type& عامل [] (صحيح i_) ثابت
  {
    بنانشانی صحيح i = (بنانشانی صحيح) i_;
    إذا (unlikely (i >= lenP1 || !i)) أرجع Null (Type);
    hb_barrier ();
    أرجع arrayZ[i-1];
  }
  Type& عامل [] (صحيح i_)
  {
    بنانشانی صحيح i = (بنانشانی صحيح) i_;
    إذا (unlikely (i >= lenP1 || !i)) أرجع Crap (Type);
    hb_barrier ();
    أرجع arrayZ[i-1];
  }
  size_t get_ناپ () ثابت
  { أرجع hb_unsigned_mul_جوڑو_saturate (get_لمائی (), Type::static_ناپ, lenP1.static_ناپ); }

  بنانشانی get_لمائی () ثابت { أرجع lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { أرجع hb_array (arrayZ, get_لمائی ()); }
  hb_array_t<ثابت Type> as_array () ثابت { أرجع hb_array (arrayZ, get_لمائی ()); }

  /* Iterator. */
  تعريفنوع hb_array_t<ثابت Type>   iter_t;
  تعريفنوع hb_array_t<      Type> writer_t;
    iter_t   iter () ثابت { أرجع as_array (); }
  writer_t writer ()       { أرجع as_array (); }
  عامل   iter_t () ثابت { أرجع   iter (); }
  عامل writer_t ()       { أرجع writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { أرجع arrayZ; }
  ثابت Type *سرا () ثابت { أرجع arrayZ + get_لمائی (); }

  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, بنانشانی صحيح اشیاء_len, منطقي clear = سچ)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    c->check_سونپو (lenP1, اشیاء_len + 1, HB_SERIALIZE_ERROR_ARRAY_حد_توں_ودھ);
    إذا (unlikely (!c->extend_ناپ (هذا, get_ناپ (), clear))) return_trace (خطأ);
    return_trace (سچ);
  }
  سانچا <قسم_ناں Iterator,
	    hb_requires (hb_is_سروت_دا (Iterator, Type))>
  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, Iterator اشیاء)
  {
    TRACE_SERIALIZE (هذا);
    بنانشانی گنتی = hb_len (اشیاء);
    إذا (unlikely (!serialize (c, گنتی, خطأ))) return_trace (خطأ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    لكل (بنانشانی i = 0; i < گنتی; i++, ++اشیاء)
      arrayZ[i] = *اشیاء;
    return_trace (سچ);
  }

  سانچا <قسم_ناں ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (سچ);
    hb_barrier ();
    بنانشانی صحيح گنتی = get_لمائی ();
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
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
  DEFINE_ناپ_ARRAY (حجم (LenType), arrayZ);
};
سانچا <قسم_ناں Type> ورتوں Headless_Array16_دا = Headless_Array_دا<Type, HBUINT16>;

/* An array storing length-1. */
سانچا <قسم_ناں Type, قسم_ناں LenType=HBUINT16>
بنية Array_دا_M1
{
  HB_DELETE_CREATE_COPY_سونپو (Array_دا_M1);

  ثابت Type& عامل [] (صحيح i_) ثابت
  {
    بنانشانی صحيح i = (بنانشانی صحيح) i_;
    إذا (unlikely (i > lenM1)) أرجع Null (Type);
    hb_barrier ();
    أرجع arrayZ[i];
  }
  Type& عامل [] (صحيح i_)
  {
    بنانشانی صحيح i = (بنانشانی صحيح) i_;
    إذا (unlikely (i > lenM1)) أرجع Crap (Type);
    hb_barrier ();
    أرجع arrayZ[i];
  }
  size_t get_ناپ () ثابت
  { أرجع hb_unsigned_mul_جوڑو_saturate (lenM1 + 1, Type::static_ناپ, lenM1.static_ناپ); }

  سانچا <قسم_ناں ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (سچ);
    hb_barrier ();
    بنانشانی صحيح گنتی = lenM1 + 1;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
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
  DEFINE_ناپ_ARRAY (حجم (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
سانچا <قسم_ناں Type, قسم_ناں LenType>
بنية Sorted_Array_دا : Array_دا<Type, LenType>
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
  ثابت Type *سرا () ثابت { أرجع هذا->arrayZ + هذا->len; }

  منطقي serialize (hb_serialize_context_t *c, بنانشانی صحيح اشیاء_len)
  {
    TRACE_SERIALIZE (هذا);
    منطقي ret = Array_دا<Type, LenType>::serialize (c, اشیاء_len);
    return_trace (ret);
  }
  سانچا <قسم_ناں Iterator,
	    hb_requires (hb_is_sorted_سروت_دا (Iterator, Type))>
  منطقي serialize (hb_serialize_context_t *c, Iterator اشیاء)
  {
    TRACE_SERIALIZE (هذا);
    منطقي ret = Array_دا<Type, LenType>::serialize (c, اشیاء);
    return_trace (ret);
  }

  Sorted_Array_دا* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    Sorted_Array_دا* باہرلا = reinterpret_cast<Sorted_Array_دا *> (Array_دا<Type, LenType>::copy (c));
    return_trace (باہرلا);
  }

  سانچا <قسم_ناں T>
  Type &bsearch (ثابت T &x, Type &not_لبھیا = Crap (Type))
  { أرجع *as_array ().bsearch (x, &not_لبھیا); }
  سانچا <قسم_ناں T>
  ثابت Type &bsearch (ثابت T &x, ثابت Type &not_لبھیا = Null (Type)) ثابت
  { أرجع *as_array ().bsearch (x, &not_لبھیا); }
  سانچا <قسم_ناں T>
  منطقي bfind (ثابت T &x, بنانشانی صحيح *i = خالی_سوچک,
	      hb_not_لبھیا_t not_لبھیا = HB_NOT_لبھیا_DONT_STORE,
	      بنانشانی صحيح منزل_store = (بنانشانی صحيح) -1) ثابت
  { أرجع as_array ().bfind (x, i, not_لبھیا, منزل_store); }
};

سانچا <قسم_ناں Type> ورتوں Sorted_Array16_دا = Sorted_Array_دا<Type, HBUINT16>;
سانچا <قسم_ناں Type> ورتوں Sorted_Array24_دا = Sorted_Array_دا<Type, HBUINT24>;
سانچا <قسم_ناں Type> ورتوں Sorted_Array32_دا = Sorted_Array_دا<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

سانچا <قسم_ناں LenType=HBUINT16>
بنية Bin_تلاش_سرخی
{
  عامل uint32_t () ثابت { أرجع len; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  Bin_تلاش_سرخی& عامل = (بنانشانی صحيح v)
  {
    len = v;
    أكد (len == v);
    اندراج_Selector = hb_max (1u, hb_ثنائی_ہندسہ_storage (v)) - 1;
    تلاش_Range = 16 * (1u << اندراج_Selector);
    range_کھسکاؤ = v * 16 > تلاش_Range
		 ? 16 * v - تلاش_Range
		 : 0;
    أرجع *هذا;
  }

  محمي:
  LenType	len;
  LenType	تلاش_Range;
  LenType	اندراج_Selector;
  LenType	range_کھسکاؤ;

  عام:
  DEFINE_ناپ_STATIC (8);
};

سانچا <قسم_ناں Type, قسم_ناں LenType=HBUINT16>
ورتوں Bin_تلاش_Array_دا = Sorted_Array_دا<Type, Bin_تلاش_سرخی<LenType>>;


بنية Var_Sized_Bin_تلاش_سرخی
{

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16	unit_ناپ;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	تلاش_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	اندراج_Selector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	range_کھسکاؤ;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  عام:
  DEFINE_ناپ_STATIC (10);
};

سانچا <قسم_ناں Type>
بنية Var_Sized_Bin_تلاش_Array_دا
{
  ساكن constexpr بنانشانی شے_ناپ = Type::static_ناپ;

  HB_DELETE_CREATE_COPY_سونپو (Var_Sized_Bin_تلاش_Array_دا);

  منطقي آخری_is_terminator () ثابت
  {
    إذا (unlikely (!سرخی.nUnits)) أرجع خطأ;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    ثابت HBUINT16 *الفاظ = &Struct_At_وتھ<HBUINT16> (&بائٹاں_Z, (سرخی.nUnits - 1) * سرخی.unit_ناپ);
    بنانشانی صحيح گنتی = Type::Termination_لفظ_گنتی;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (الفاظ[i] != 0xFFFFu)
	أرجع خطأ;
    أرجع سچ;
  }

  ثابت Type& عامل [] (صحيح i_) ثابت
  {
    بنانشانی صحيح i = (بنانشانی صحيح) i_;
    إذا (unlikely (i >= get_لمائی ())) أرجع Null (Type);
    hb_barrier ();
    أرجع Struct_At_وتھ<Type> (&بائٹاں_Z, i * سرخی.unit_ناپ);
  }
  Type& عامل [] (صحيح i_)
  {
    بنانشانی صحيح i = (بنانشانی صحيح) i_;
    إذا (unlikely (i >= get_لمائی ())) أرجع Crap (Type);
    hb_barrier ();
    أرجع Struct_At_وتھ<Type> (&بائٹاں_Z, i * سرخی.unit_ناپ);
  }
  بنانشانی صحيح get_لمائی () ثابت
  { أرجع سرخی.nUnits - آخری_is_terminator (); }
  size_t get_ناپ () ثابت
  { أرجع hb_unsigned_mul_جوڑو_saturate (سرخی.nUnits, سرخی.unit_ناپ, سرخی.static_ناپ); }

  سانچا <قسم_ناں ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (سچ);
    hb_barrier ();
    بنانشانی صحيح گنتی = get_لمائی ();
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (unlikely (!(*هذا)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (خطأ);
    return_trace (سچ);
  }

  سانچا <قسم_ناں T>
  ثابت Type *bsearch (ثابت T &key) ثابت
  {
    بنانشانی pos;
    أرجع hb_bsearch_impl (&pos,
			    key,
			    (ثابت فراغ *) بائٹاں_Z,
			    get_لمائی (),
			    سرخی.unit_ناپ,
			    hb_cmp_طریقہ<T, Type>)
	   ? (ثابت Type *) (((ثابت حرف *) &بائٹاں_Z) + (pos * سرخی.unit_ناپ))
	   : خالی_سوچک;
  }

  خاص:
  منطقي sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (سرخی.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_ناپ <= سرخی.unit_ناپ &&
		  c->check_range (بائٹاں_Z.arrayZ,
				  سرخی.nUnits,
				  سرخی.unit_ناپ));
  }

  محمي:
  Var_Sized_Bin_تلاش_سرخی	سرخی;
  Unsized_Array_دا<HBUINT8>	بائٹاں_Z;
  عام:
  DEFINE_ناپ_ARRAY (10, بائٹاں_Z);
};


/* CFF INDEX */

سانچا <قسم_ناں گنتی_3>
بنية CFFIndex
{
  بنانشانی صحيح وتھ_array_ناپ () ثابت
  { أرجع off_ناپ * (گنتی + 1); }

  سانچا <قسم_ناں Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Iterable &iterable,
		  ثابت بنانشانی *p_انکڑے_ناپ = خالی_سوچک,
                  بنانشانی min_off_ناپ = 0)
  {
    TRACE_SERIALIZE (هذا);
    بنانشانی انکڑے_ناپ;
    إذا (p_انکڑے_ناپ)
      انکڑے_ناپ = *p_انکڑے_ناپ;
    وإلا
      کل_ناپ (iterable, &انکڑے_ناپ);

    خودکار it = hb_iter (iterable);
    إذا (unlikely (!serialize_سرخی (c, +it, انکڑے_ناپ, min_off_ناپ))) return_trace (خطأ);
    بنانشانی حرف *ret = c->allocate_ناپ<بنانشانی حرف> (انکڑے_ناپ, خطأ);
    إذا (unlikely (!ret)) return_trace (خطأ);
    بنانشانی بچیا = انکڑے_ناپ;
    لكل (ثابت خودکار &_ : +it)
    {
      بنانشانی len = _.لمائی;

      إذا (!len)
	استمر;

      إذا (unlikely (len > بچیا)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (خطأ, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
      }

      بچیا -= len;

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

  سانچا <قسم_ناں Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_سرخی (hb_serialize_context_t *c,
			 Iterator it,
			 بنانشانی انکڑے_ناپ,
                         بنانشانی min_off_ناپ = 0)
  {
    TRACE_SERIALIZE (هذا);

    بنانشانی off_ناپ_2 = (hb_ثنائی_ہندسہ_storage (انکڑے_ناپ + 1) + 7) / 8;
    off_ناپ_2 = hb_max(min_off_ناپ, off_ناپ_2);

    /* serialize CFFIndex header */
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    هذا->گنتی = hb_len (it);
    إذا (!هذا->گنتی) return_trace (سچ);
    إذا (unlikely (!c->extend (هذا->off_ناپ))) return_trace (خطأ);
    هذا->off_ناپ = off_ناپ_2;
    إذا (unlikely (!c->allocate_ناپ<HBUINT8> (off_ناپ_2 * (هذا->گنتی + 1), خطأ)))
      return_trace (خطأ);

    /* serialize indices */
    بنانشانی صحيح وتھ = 1;
    إذا (HB_OPTIMIZE_ناپ_VAL)
    {
      بنانشانی صحيح i = 0;
      لكل (ثابت خودکار &_ : +it)
      {
	سیٹ_وتھ_at (i++, وتھ);
	وتھ += hb_len_دا (_);
      }
      سیٹ_وتھ_at (i, وتھ);
    }
    وإلا
      بدل (off_ناپ_2)
      {
	حالة 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  لكل (ثابت خودکار &_ : +it)
	  {
	    *p++ = وتھ;
	    وتھ += hb_len_دا (_);
	  }
	  *p = وتھ;
	}
	اكسر;
	حالة 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  لكل (ثابت خودکار &_ : +it)
	  {
	    *p++ = وتھ;
	    وتھ += hb_len_دا (_);
	  }
	  *p = وتھ;
	}
	اكسر;
	حالة 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  لكل (ثابت خودکار &_ : +it)
	  {
	    *p++ = وتھ;
	    وتھ += hb_len_دا (_);
	  }
	  *p = وتھ;
	}
	اكسر;
	حالة 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  لكل (ثابت خودکار &_ : +it)
	  {
	    *p++ = وتھ;
	    وتھ += hb_len_دا (_);
	  }
	  *p = وتھ;
	}
	اكسر;
	افتراضي:
	اكسر;
      }

    أكد (وتھ == انکڑے_ناپ + 1);
    return_trace (سچ);
  }

  سانچا <قسم_ناں Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  ساكن بنانشانی کل_ناپ (ثابت Iterable &iterable, بنانشانی *انکڑے_ناپ = خالی_سوچک, بنانشانی min_off_ناپ = 0)
  {
    خودکار it = + hb_iter (iterable);
    إذا (!it)
    {
      إذا (انکڑے_ناپ) *انکڑے_ناپ = 0;
      أرجع min_ناپ;
    }

    بنانشانی کل = 0;
    لكل (ثابت خودکار &_ : +it)
      کل += hb_len_دا (_);

    إذا (انکڑے_ناپ) *انکڑے_ناپ = کل;

    بنانشانی off_ناپ_2 = (hb_ثنائی_ہندسہ_storage (کل + 1) + 7) / 8;
    off_ناپ_2 = hb_max(min_off_ناپ, off_ناپ_2);

    أرجع min_ناپ + HBUINT8::static_ناپ + (hb_len (it) + 1) * off_ناپ_2 + کل;
  }

  فراغ سیٹ_وتھ_at (بنانشانی صحيح اشاریہ_2, بنانشانی صحيح وتھ)
  {
    أكد (اشاریہ_2 <= گنتی);

    بنانشانی صحيح ناپ = off_ناپ;
    ثابت HBUINT8 *p = offsets;
    بدل (ناپ)
    {
      حالة 1: ((HBUINT8  *) p)[اشاریہ_2] = وتھ; اكسر;
      حالة 2: ((HBUINT16 *) p)[اشاریہ_2] = وتھ; اكسر;
      حالة 3: ((HBUINT24 *) p)[اشاریہ_2] = وتھ; اكسر;
      حالة 4: ((HBUINT32 *) p)[اشاریہ_2] = وتھ; اكسر;
      افتراضي: أرجع;
    }
  }

  خاص:
  بنانشانی صحيح وتھ_at (بنانشانی صحيح اشاریہ_2) ثابت
  {
    أكد (اشاریہ_2 <= گنتی);

    بنانشانی صحيح ناپ = off_ناپ;
    ثابت HBUINT8 *p = offsets;
    بدل (ناپ)
    {
      حالة 1: أرجع ((HBUINT8  *) p)[اشاریہ_2];
      حالة 2: أرجع ((HBUINT16 *) p)[اشاریہ_2];
      حالة 3: أرجع ((HBUINT24 *) p)[اشاریہ_2];
      حالة 4: أرجع ((HBUINT32 *) p)[اشاریہ_2];
      افتراضي: أرجع 0;
    }
  }

  ثابت بنانشانی حرف *انکڑے_ادھار () ثابت
  { أرجع (ثابت بنانشانی حرف *) هذا + min_ناپ + off_ناپ.static_ناپ - 1 + وتھ_array_ناپ (); }
  عام:

  hb_ubytes_t عامل [] (بنانشانی صحيح اشاریہ_2) ثابت
  {
    إذا (unlikely (اشاریہ_2 >= گنتی)) أرجع hb_ubytes_t ();
    hb_barrier ();
    بنانشانی offset0 = وتھ_at (اشاریہ_2);
    بنانشانی offset1 = وتھ_at (اشاریہ_2 + 1);
    إذا (unlikely (offset1 < offset0 || offset1 > وتھ_at (گنتی)))
      أرجع hb_ubytes_t ();
    أرجع hb_ubytes_t (انکڑے_ادھار () + offset0, offset1 - offset0);
  }

  size_t get_ناپ () ثابت
  {
    إذا (گنتی)
      أرجع hb_unsigned_جوڑو_saturate (min_ناپ, off_ناپ.static_ناپ,
				       وتھ_array_ناپ (), وتھ_at (گنتی) - 1);
    أرجع min_ناپ;  /* empty CFFIndex contains count only */
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  (گنتی == 0 || /* empty INDEX */
			   (گنتی < گنتی + 1u &&
			    c->check_struct (&off_ناپ) && off_ناپ >= 1 && off_ناپ <= 4 &&
			    c->check_array (offsets, off_ناپ, گنتی + 1u) &&
			    c->check_range (انکڑے_ادھار (), وتھ_at (گنتی))))));
  }

  عام:
  گنتی_3		گنتی;		/* Number of object data. Note there are (count+1) offsets */
  خاص:
  HBUINT8	off_ناپ;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  عام:
  DEFINE_ناپ_MIN (گنتی_3::static_ناپ);
};
تعريفنوع CFFIndex<HBUINT16> CFF1_اشاریہ;
تعريفنوع CFFIndex<HBUINT32> CFF2_اشاریہ;


/* TupleValues */
بنية Tuple_اقدار
{
  تعداد packed_قدر_flag_t
  {
    اقدار_ARE_ZEROS     = 0x80,
    اقدار_ARE_بائٹاں     = 0x00,
    اقدار_ARE_الفاظ     = 0x40,
    اقدار_ARE_LONGS     = 0xC0,
    اقدار_ناپ_نقاب     = 0xC0,
    قدر_لگاتار_گنتی_نقاب = 0x3F
  };

  ساكن بنانشانی compile_unsafe (hb_array_t<ثابت صحيح> values, /*وچ*/
				  بنانشانی حرف *encoded_بائٹاں /*باہرلا*/)
  {
    بنانشانی عدد_اقدار_3 = values.لمائی;
    بنانشانی encoded_len = 0;
    بنانشانی i = 0;
    طالما (i < عدد_اقدار_3)
    {
      صحيح val = values.arrayZ[i];
      إذا (val == 0)
        encoded_len += encode_قدر_لگاتار_as_zeroes (i, encoded_بائٹاں + encoded_len, values);
      وإلا إذا ((int8_t) val == val)
        encoded_len += encode_قدر_لگاتار_as_بائٹاں (i, encoded_بائٹاں + encoded_len, values);
      وإلا إذا ((int16_t) val == val)
        encoded_len += encode_قدر_لگاتار_as_الفاظ (i, encoded_بائٹاں + encoded_len, values);
      وإلا
        encoded_len += encode_قدر_لگاتار_as_longs (i, encoded_بائٹاں + encoded_len, values);
    }
    أرجع encoded_len;
  }

  ساكن بنانشانی encode_قدر_لگاتار_as_zeroes (بنانشانی& i,
					      بنانشانی حرف *it,
					      hb_array_t<ثابت صحيح> values)
  {
    بنانشانی عدد_اقدار_3 = values.لمائی;
    بنانشانی لگاتار_لمائی = 0;
    بنانشانی encoded_len = 0;
    طالما (i < عدد_اقدار_3 && values.arrayZ[i] == 0)
    {
      i++;
      لگاتار_لمائی++;
    }

    طالما (لگاتار_لمائی >= 64)
    {
      *it++ = حرف (اقدار_ARE_ZEROS | 63);
      لگاتار_لمائی -= 64;
      encoded_len++;
    }

    إذا (لگاتار_لمائی)
    {
      *it++ = حرف (اقدار_ARE_ZEROS | (لگاتار_لمائی - 1));
      encoded_len++;
    }
    أرجع encoded_len;
  }

  ساكن بنانشانی encode_قدر_لگاتار_as_بائٹاں (بنانشانی &i,
					     بنانشانی حرف *it,
					     hb_array_t<ثابت صحيح> values)
  {
    بنانشانی شروع = i;
    بنانشانی عدد_اقدار_3 = values.لمائی;
    طالما (i < عدد_اقدار_3)
    {
      صحيح val = values.arrayZ[i];
      إذا ((int8_t) val != val)
        اكسر;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      إذا (val == 0 && i + 1 < عدد_اقدار_3 && values.arrayZ[i+1] == 0)
        اكسر;

      i++;
    }
    بنانشانی لگاتار_لمائی = i - شروع;

    بنانشانی encoded_len = 0;

    طالما (لگاتار_لمائی >= 64)
    {
      *it++ = (اقدار_ARE_بائٹاں | 63);
      encoded_len++;

      لكل (بنانشانی j = 0; j < 64; j++)
	it[j] = static_cast<حرف> (values.arrayZ[شروع + j]);
      it += 64;
      encoded_len += 64;

      شروع += 64;
      لگاتار_لمائی -= 64;
    }

    إذا (لگاتار_لمائی)
    {
      *it++ = (اقدار_ARE_بائٹاں | (لگاتار_لمائی - 1));
      encoded_len++;

      لكل (بنانشانی j = 0; j < لگاتار_لمائی; j++)
        it[j] = static_cast<حرف> (values.arrayZ[شروع + j]);
      encoded_len += لگاتار_لمائی;
    }

    أرجع encoded_len;
  }

  ساكن بنانشانی encode_قدر_لگاتار_as_الفاظ (بنانشانی &i,
					     بنانشانی حرف *it,
					     hb_array_t<ثابت صحيح> values)
  {
    بنانشانی شروع = i;
    بنانشانی عدد_اقدار_3 = values.لمائی;
    طالما (i < عدد_اقدار_3)
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
          i + 1 < عدد_اقدار_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        اكسر;

      i++;
    }

    بنانشانی لگاتار_لمائی = i - شروع;
    بنانشانی encoded_len = 0;
    طالما (لگاتار_لمائی >= 64)
    {
      *it++ = (اقدار_ARE_الفاظ | 63);
      encoded_len++;

      لكل (بنانشانی j = 0; j < 64; j++)
      {
        int16_t قدر_val = values.arrayZ[شروع + j];
        *it++ = static_cast<حرف> (قدر_val >> 8);
        *it++ = static_cast<حرف> (قدر_val & 0xFF);

        encoded_len += 2;
      }

      شروع += 64;
      لگاتار_لمائی -= 64;
    }

    إذا (لگاتار_لمائی)
    {
      *it++ = (اقدار_ARE_الفاظ | (لگاتار_لمائی - 1));
      encoded_len++;
      طالما (شروع < i)
      {
        int16_t قدر_val = values.arrayZ[شروع++];
        *it++ = static_cast<حرف> (قدر_val >> 8);
        *it++ = static_cast<حرف> (قدر_val & 0xFF);

        encoded_len += 2;
      }
    }
    أرجع encoded_len;
  }

  ساكن بنانشانی encode_قدر_لگاتار_as_longs (بنانشانی &i,
					     بنانشانی حرف *it,
					     hb_array_t<ثابت صحيح> values)
  {
    بنانشانی شروع = i;
    بنانشانی عدد_اقدار_3 = values.لمائی;
    طالما (i < عدد_اقدار_3)
    {
      صحيح val = values.arrayZ[i];

      إذا ((int16_t) val == val)
        اكسر;

      i++;
    }

    بنانشانی لگاتار_لمائی = i - شروع;
    بنانشانی encoded_len = 0;
    طالما (لگاتار_لمائی >= 64)
    {
      *it++ = (اقدار_ARE_LONGS | 63);
      encoded_len++;

      لكل (بنانشانی j = 0; j < 64; j++)
      {
        int32_t قدر_val = values.arrayZ[شروع + j];
        *it++ = static_cast<حرف> (قدر_val >> 24);
        *it++ = static_cast<حرف> (قدر_val >> 16);
        *it++ = static_cast<حرف> (قدر_val >> 8);
        *it++ = static_cast<حرف> (قدر_val & 0xFF);

        encoded_len += 4;
      }

      شروع += 64;
      لگاتار_لمائی -= 64;
    }

    إذا (لگاتار_لمائی)
    {
      *it++ = (اقدار_ARE_LONGS | (لگاتار_لمائی - 1));
      encoded_len++;
      طالما (شروع < i)
      {
        int32_t قدر_val = values.arrayZ[شروع++];
        *it++ = static_cast<حرف> (قدر_val >> 24);
        *it++ = static_cast<حرف> (قدر_val >> 16);
        *it++ = static_cast<حرف> (قدر_val >> 8);
        *it++ = static_cast<حرف> (قدر_val & 0xFF);

        encoded_len += 4;
      }
    }
    أرجع encoded_len;
  }

  سانچا <قسم_ناں T>
#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  ساكن منطقي decompile (ثابت HBUINT8 *&p /* IN/OUT */,
			 hb_سمتیہ_t<T> &values /* IN/OUT */,
			 ثابت HBUINT8 *سرا,
			 منطقي consume_all = خطأ,
			 بنانشانی شروع = 0)
  {
    بنانشانی i = 0;
    بنانشانی گنتی = consume_all ? UINT_MAX : values.لمائی;
    إذا (consume_all)
      values.alloc ((سرا - p) / 2);
    طالما (i < گنتی)
    {
      إذا (unlikely (p + 1 > سرا)) أرجع consume_all;
      بنانشانی control = *p++;
      بنانشانی لگاتار_گنتی = (control & قدر_لگاتار_گنتی_نقاب) + 1;
      إذا (consume_all)
      {
        إذا (unlikely (!values.resize_dirty  (values.لمائی + لگاتار_گنتی)))
	  أرجع خطأ;
      }
      بنانشانی stop = i + لگاتار_گنتی;
      إذا (unlikely (stop > گنتی)) أرجع خطأ;

      بنانشانی skip = i < شروع ? hb_min (شروع - i, لگاتار_گنتی) : 0;
      i += skip;

      إذا ((control & اقدار_ناپ_نقاب) == اقدار_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * حجم (T));
	i = stop;
      }
      وإلا إذا ((control & اقدار_ناپ_نقاب) ==  اقدار_ARE_الفاظ)
      {
        إذا (unlikely (p + لگاتار_گنتی * HBINT16::static_ناپ > سرا)) أرجع خطأ;
	p += skip * HBINT16::static_ناپ;
#جے_بنا_تعریف HB_OPTIMIZE_ناپ
        لكل (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_ناپ;
	  values.arrayZ[i + 1] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_ناپ;
	  values.arrayZ[i + 2] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_ناپ;
	  values.arrayZ[i + 3] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_ناپ;
	}
#جے_انت
        لكل (; i < stop; i++)
        {
          values.arrayZ[i] = * (ثابت HBINT16 *) p;
          p += HBINT16::static_ناپ;
        }
      }
      وإلا إذا ((control & اقدار_ناپ_نقاب) ==  اقدار_ARE_LONGS)
      {
        إذا (unlikely (p + لگاتار_گنتی * HBINT32::static_ناپ > سرا)) أرجع خطأ;
	p += skip * HBINT32::static_ناپ;
        لكل (; i < stop; i++)
        {
          values.arrayZ[i] = * (ثابت HBINT32 *) p;
          p += HBINT32::static_ناپ;
        }
      }
      وإلا إذا ((control & اقدار_ناپ_نقاب) ==  اقدار_ARE_بائٹاں)
      {
        إذا (unlikely (p + لگاتار_گنتی > سرا)) أرجع خطأ;
	p += skip * HBINT8::static_ناپ;
#جے_بنا_تعریف HB_OPTIMIZE_ناپ
	لكل (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (ثابت HBINT8 *) p++;
	}
#جے_انت
        لكل (; i < stop; i++)
          values.arrayZ[i] = * (ثابت HBINT8 *) p++;
      }
    }
    أرجع سچ;
  }

  ساكن منطقي skip (ثابت بنانشانی حرف *&p /* IN/OUT */,
		    ثابت بنانشانی حرف *سرا,
		    بنانشانی گنتی)
  {
    بنانشانی تکیا = 0;
    طالما (تکیا < گنتی)
    {
      إذا (unlikely (p >= سرا)) أرجع خطأ;
      بنانشانی control = *p++;
      بنانشانی لگاتار_گنتی = (control & قدر_لگاتار_گنتی_نقاب) + 1;
      إذا (unlikely (لگاتار_گنتی > گنتی - تکیا)) أرجع خطأ;

      بنانشانی چوڑائی = 0;
      بدل (control & اقدار_ناپ_نقاب)
      {
	حالة اقدار_ARE_ZEROS: اكسر;
	حالة اقدار_ARE_بائٹاں: چوڑائی = HBINT8::static_ناپ; اكسر;
	حالة اقدار_ARE_الفاظ: چوڑائی = HBINT16::static_ناپ; اكسر;
	حالة اقدار_ARE_LONGS: چوڑائی = HBINT32::static_ناپ; اكسر;
      }
      إذا (unlikely (بنانشانی (سرا - p) < لگاتار_گنتی * چوڑائی)) أرجع خطأ;
      p += لگاتار_گنتی * چوڑائی;
      تکیا += لگاتار_گنتی;
    }
    أرجع سچ;
  }

  بنية iter_t : hb_iter_with_fallback_t<iter_t, صحيح>
  {
    iter_t (ثابت بنانشانی حرف *p_, بنانشانی len_)
	    : p (p_), endp (p_ + len_)
    { إذا (likely (ensure_لگاتار ())) read_قدر (); }

    خاص:
    ثابت بنانشانی حرف *p;
    ثابت بنانشانی حرف * ثابت endp;
    صحيح current_قدر = 0;
    نشانیوالا لگاتار_گنتی = 0;
    بنانشانی چوڑائی = 0;

    HB_ALWAYS_INLINE
    منطقي ensure_لگاتار ()
    {
      إذا (likely (لگاتار_گنتی > 0)) أرجع سچ;
      أرجع ensure_لگاتار_2 ();
    }
    منطقي ensure_لگاتار_2 ()
    {
      إذا (unlikely (p >= endp))
      {
        لگاتار_گنتی = 0;
        current_قدر = 0;
	أرجع خطأ;
      }

      بنانشانی control = *p++;
      لگاتار_گنتی = (control & قدر_لگاتار_گنتی_نقاب) + 1;
      چوڑائی = control & اقدار_ناپ_نقاب;
      بدل (چوڑائی)
      {
        حالة اقدار_ARE_ZEROS: چوڑائی = 0; اكسر;
	حالة اقدار_ARE_بائٹاں: چوڑائی = HBINT8::static_ناپ;  اكسر;
	حالة اقدار_ARE_الفاظ: چوڑائی = HBINT16::static_ناپ; اكسر;
	حالة اقدار_ARE_LONGS: چوڑائی = HBINT32::static_ناپ; اكسر;
	افتراضي: أكد (خطأ);
      }

      إذا (unlikely (p + لگاتار_گنتی * چوڑائی > endp))
      {
	لگاتار_گنتی = 0;
	current_قدر = 0;
	أرجع خطأ;
      }

      أرجع سچ;
    }
    فراغ read_قدر ()
    {
      بدل (چوڑائی)
      {
        حالة 0: current_قدر = 0; اكسر;
	حالة 1: current_قدر = * (ثابت HBINT8  *) p; اكسر;
	حالة 2: current_قدر = * (ثابت HBINT16 *) p; اكسر;
	حالة 4: current_قدر = * (ثابت HBINT32 *) p; اكسر;
      }
      p += چوڑائی;
    }

    عام:

    تعريفنوع صحيح __item_t__;
    __item_t__ __item__ () ثابت
    { أرجع current_قدر; }

    منطقي __more__ () ثابت { أرجع لگاتار_گنتی || p < endp; }
    فراغ __next__ ()
    {
      لگاتار_گنتی--;
      إذا (unlikely (!ensure_لگاتار ()))
	أرجع;
      read_قدر ();
    }
    فراغ __forward__ (بنانشانی n)
    {
      إذا (unlikely (!ensure_لگاتار ()))
	أرجع;
      طالما (n)
      {
	بنانشانی i = hb_min (n, (بنانشانی) لگاتار_گنتی);
	لگاتار_گنتی -= i;
	n -= i;
	p += (i - 1) * چوڑائی;
	إذا (unlikely (!ensure_لگاتار ()))
	  أرجع;
	read_قدر ();
      }
    }
    منطقي عامل != (ثابت iter_t& o) ثابت
    { أرجع p != o.p || لگاتار_گنتی != o.لگاتار_گنتی; }
    iter_t __end__ () ثابت
    {
      iter_t it (endp, 0);
      أرجع it;
    }
  };

  بنية fetcher_t
  {
    fetcher_t (ثابت بنانشانی حرف *p_, بنانشانی len_)
	      : p (p_), سرا (p_ + len_) {}

    خاص:
    ثابت بنانشانی حرف *p;
    ثابت بنانشانی حرف * ثابت سرا;
    نشانیوالا لگاتار_گنتی = 0;
    بنانشانی چوڑائی = 0;

    HB_ALWAYS_INLINE
    منطقي ensure_لگاتار ()
    {
      إذا (likely (لگاتار_گنتی > 0)) أرجع سچ;
      أرجع ensure_لگاتار_2 ();
    }

    منطقي ensure_لگاتار_2 ()
    {
      إذا (unlikely (p >= سرا))
      {
        لگاتار_گنتی = 0;
	أرجع خطأ;
      }

      بنانشانی control = *p++;
      لگاتار_گنتی = (control & قدر_لگاتار_گنتی_نقاب) + 1;
      چوڑائی = control & اقدار_ناپ_نقاب;
      بدل (چوڑائی)
      {
        حالة اقدار_ARE_ZEROS: چوڑائی = 0; اكسر;
	حالة اقدار_ARE_بائٹاں: چوڑائی = HBINT8::static_ناپ;  اكسر;
	حالة اقدار_ARE_الفاظ: چوڑائی = HBINT16::static_ناپ; اكسر;
	حالة اقدار_ARE_LONGS: چوڑائی = HBINT32::static_ناپ; اكسر;
	افتراضي: أكد (خطأ);
      }

      إذا (unlikely (p + لگاتار_گنتی * چوڑائی > سرا))
      {
	لگاتار_گنتی = 0;
	أرجع خطأ;
      }

      أرجع سچ;
    }

    عام:
    فراغ skip (بنانشانی n)
    {
      طالما (n)
      {
	إذا (unlikely (!ensure_لگاتار ()))
	  أرجع;
	بنانشانی i = hb_min (n, (بنانشانی) لگاتار_گنتی);
	لگاتار_گنتی -= i;
	n -= i;
	p += i * چوڑائی;
      }
    }

    خاص:
    سانچا <منطقي scaled, قسم_ناں T>
    فراغ جوڑو_منزل (hb_array_t<T> باہرلا, T scale = T (1))
    {
      بنانشانی n = باہرلا.لمائی;
      T *arrayZ = باہرلا.arrayZ;

      لكل (بنانشانی i = 0; i < n;)
      {
	إذا (unlikely (!ensure_لگاتار ()))
	  اكسر;
	بنانشانی گنتی = hb_min (n - i, (بنانشانی) لگاتار_گنتی);
	بدل (چوڑائی)
	{
	  حالة 0:
	  {
	    arrayZ += گنتی;
	    اكسر;
	  }
	  حالة 1:
	  {
	    ثابت خودکار *pp = (ثابت HBINT8 *) p;
	    بنانشانی j = 0;
#جے_بنا_تعریف HB_OPTIMIZE_ناپ
	    لكل (; j + 3 < گنتی; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#جے_انت
	    لكل (; j < گنتی; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت بنانشانی حرف *) pp;
	  }
	  اكسر;
	  حالة 2:
	  {
	    ثابت خودکار *pp = (ثابت HBINT16 *) p;
	    بنانشانی j = 0;
#جے_بنا_تعریف HB_OPTIMIZE_ناپ
	    لكل (; j + 3 < گنتی; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#جے_انت
	    لكل (; j < گنتی; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت بنانشانی حرف *) pp;
	  }
	  اكسر;
	  حالة 4:
	  {
	    ثابت خودکار *pp = (ثابت HBINT32 *) p;
	    لكل (بنانشانی j = 0; j < گنتی; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت بنانشانی حرف *) pp;
	  }
	  اكسر;
	}
	لگاتار_گنتی -= گنتی;
	i += گنتی;
      }
    }

    عام:
    سانچا <قسم_ناں T>
    فراغ جوڑو_منزل_2 (hb_array_t<T> باہرلا, عائم scale = 1.0f)
    {
#جے_بنا_تعریف HB_OPTIMIZE_ناپ
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      إذا (خطأ && scale == 1.0f)
        جوڑو_منزل<خطأ> (باہرلا);
      وإلا
#جے_انت
        جوڑو_منزل<سچ> (باہرلا, (T) scale);
    }
  };
};

بنية TupleList : CFF2_اشاریہ
{
  Tuple_اقدار::iter_t عامل [] (بنانشانی i) ثابت
  {
    خودکار بائٹاں = CFF2_اشاریہ::عامل [] (i);
    أرجع Tuple_اقدار::iter_t (بائٹاں.arrayZ, بائٹاں.لمائی);
  }

  Tuple_اقدار::fetcher_t fetcher (بنانشانی i) ثابت
  {
    خودکار بائٹاں = CFF2_اشاریہ::عامل [] (i);
    أرجع Tuple_اقدار::fetcher_t (بائٹاں.arrayZ, بائٹاں.لمائی);
  }
};


// Alignment

سانچا <بنانشانی صحيح alignment>
بنية Align
{
  size_t get_ناپ (ثابت فراغ *ادھار) ثابت
  {
    بنانشانی وتھ = (ثابت حرف *) هذا - (ثابت حرف *) ادھار;
    أرجع (alignment - وتھ) & (alignment - 1);
  }

  عام:
  DEFINE_ناپ_MIN (0);
};



} /* namespace OT */


#جے_انت /* HB_OPEN_TYPE_HH */
