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

#اگر_غیر_تعریف_شدہ HB_OPEN_TYPE_HH
#تعریف_کرو HB_OPEN_TYPE_HH

#شامل "hb.hh"
#شامل "hb-blob.hh"
#شامل "hb-face.hh"
#شامل "hb-machinery.hh"
#شامل "hb-meta.hh"
#شامل "hb-subset.hh"


نامخلا OT {


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
سانچہ <منطقی BE,
	  قسم_نام Type,
	  غیرعلامتی صحیح سائز_2 = حجم (Type)>
ساخت عدد_Type
{
  قسمتعریف Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  قسمتعریف قسم_نام std::conditional<std::is_integral<Type>::value && حجم (Type) <= حجم(صحیح),
				     قسم_نام std::conditional<std::is_signed<Type>::value, علامتی, غیرعلامتی>::type,
				     Type>::type WideType;

  عدد_Type () = طےشدہ;
  صریح constexpr عدد_Type (Type V) : v {V} {}
  عدد_Type& عامل = (Type V) { v = V; واپس *یہ; }

  عامل WideType () ثابت { واپس v; }

  منطقی عامل == (ثابت عدد_Type &o) ثابت { واپس (Type) v == (Type) o.v; }
  منطقی عامل != (ثابت عدد_Type &o) ثابت { واپس !(*یہ == o); }

  عدد_Type& عامل += (WideType گنتی) { *یہ = *یہ + گنتی; واپس *یہ; }
  عدد_Type& عامل -= (WideType گنتی) { *یہ = *یہ - گنتی; واپس *یہ; }
  عدد_Type& عامل ++ () { *یہ += 1; واپس *یہ; }
  عدد_Type& عامل -- () { *یہ -= 1; واپس *یہ; }
  عدد_Type عامل ++ (صحیح) { عدد_Type c (*یہ); ++*یہ; واپس c; }
  عدد_Type عامل -- (صحیح) { عدد_Type c (*یہ); --*یہ; واپس c; }

  uint32_t hash () ثابت { واپس hb_array ((ثابت حرف *) &v, حجم (v)).hash (); }
  HB_اندرونی ساکن صحیح cmp (ثابت عدد_Type *a, ثابت عدد_Type *b)
  { واپس b->cmp (*a); }
  HB_اندرونی ساکن صحیح cmp (ثابت خالی *a, ثابت خالی *b)
  {
    عدد_Type *pa = (عدد_Type *) a;
    عدد_Type *pb = (عدد_Type *) b;

    واپس pb->cmp (*pa);
  }
  سانچہ <قسم_نام Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  صحیح cmp (Type2 a) ثابت
  {
    Type b = v;
    واپس (a > b) - (a < b);
  }
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }
  محفوظ:
  قسم_نام std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, سائز_2>,
			    HBFloat<BE, Type, سائز_2>>::type v;
  عوامی:
  DEFINE_سائز_STATIC (سائز_2);
};

قسمتعریف عدد_Type<سچ, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
قسمتعریف عدد_Type<سچ, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
قسمتعریف عدد_Type<سچ, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
قسمتعریف عدد_Type<سچ, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
قسمتعریف عدد_Type<سچ, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
قسمتعریف عدد_Type<سچ, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
قسمتعریف عدد_Type<سچ, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
قسمتعریف عدد_Type<سچ, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
قسمتعریف عدد_Type<سچ, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

قسمتعریف عدد_Type<جھوٹ, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
قسمتعریف عدد_Type<جھوٹ, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
قسمتعریف عدد_Type<جھوٹ, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
قسمتعریف عدد_Type<جھوٹ, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
قسمتعریف عدد_Type<جھوٹ, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
قسمتعریف عدد_Type<جھوٹ, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

قسمتعریف عدد_Type<سچ,  اعشاری>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
قسمتعریف عدد_Type<سچ,  دوہرا> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
قسمتعریف عدد_Type<جھوٹ, اعشاری>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
قسمتعریف عدد_Type<جھوٹ, دوہرا> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
ساخت HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& عامل = (uint16_t i ) { HBUINT16::عامل= (i); واپس *یہ; }
  عوامی:
  DEFINE_سائز_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
ساخت HBUINT32VAR
{
  size_t get_سائز () ثابت
  {
    غیرعلامتی b0 = v[0];
    اگر (b0 < 0x80)
      واپس 1;
    ورنہ اگر (b0 < 0xC0)
      واپس 2;
    ورنہ اگر (b0 < 0xE0)
      واپس 3;
    ورنہ اگر (b0 < 0xF0)
      واپس 4;
    ورنہ
      واپس 5;
  }

  ساکن size_t get_سائز (uint32_t v)
  {
    اگر (v < 0x80)
      واپس 1;
    ورنہ اگر (v < 0x4000)
      واپس 2;
    ورنہ اگر (v < 0x200000)
      واپس 3;
    ورنہ اگر (v < 0x10000000)
      واپس 4;
    ورنہ
      واپس 5;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_سائز ()));
  }

  عامل uint32_t () ثابت
  {
    غیرعلامتی b0 = v[0];
    اگر (b0 < 0x80)
      واپس b0;
    ورنہ اگر (b0 < 0xC0)
      واپس ((b0 & 0x3F) << 8) | v[1];
    ورنہ اگر (b0 < 0xE0)
      واپس ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    ورنہ اگر (b0 < 0xF0)
      واپس ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    ورنہ
      واپس (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  ساکن غیرعلامتی serialize_unsafe (غیرعلامتی حرف *buf, uint32_t v)
  {
    غیرعلامتی len = get_سائز (v);
    غیرعلامتی حرف *p = buf + len;
    برائے (غیرعلامتی i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    اگر (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    واپس len;
  }

  ساکن منطقی serialize (hb_serialize_context_t *c, uint32_t v)
  {
    غیرعلامتی len = get_سائز (v);
    غیرعلامتی حرف *buf = c->allocate_سائز<غیرعلامتی حرف> (len, جھوٹ);
    اگر (unlikely (!buf))
      واپس جھوٹ;

    serialize_unsafe (buf, v);
    واپس سچ;
  }

  محفوظ:
  غیرعلامتی حرف v[5];

  عوامی:
  DEFINE_سائز_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
قسمتعریف HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
قسمتعریف HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
قسمتعریف HBUINT16 UFWORD;

سانچہ <قسم_نام Type, غیرعلامتی fraction_بٹ>
ساخت HBFixed : Type
{
  ساکن constexpr اعشاری mult = 1.f / (1 << fraction_بٹ);
  static_assert (Type::static_سائز * 8 > fraction_بٹ, "");

  عامل علامتی () ثابت = حذف;
  عامل غیرعلامتی () ثابت = حذف;
  صریح عامل اعشاری () ثابت { واپس مقصد_float (); }
  قسم_نام Type::type مقصد_int () ثابت { واپس Type::v; }
  خالی سیٹ_int (قسم_نام Type::type i ) { Type::v = i; }
  اعشاری مقصد_float (اعشاری انحراف = 0) ثابت  { واپس ((int32_t) Type::v + انحراف) * mult; }
  خالی سیٹ_float (اعشاری f) { Type::v = roundf (f / mult); }
  عوامی:
  DEFINE_سائز_STATIC (Type::static_سائز);
};

/* 16-bit signed fixed number with the low 14 bits of fraction (2.14). */
استعمال F2DOT14 = HBFixed<HBINT16, 14>;
استعمال F4DOT12 = HBFixed<HBINT16, 12>;
استعمال F6DOT10 = HBFixed<HBINT16, 10>;

/* 32-bit signed fixed-point number (16.16). */
استعمال F16DOT16 = HBFixed<HBINT32, 16>;

/* Date represented in number of seconds since 12:00 midnight, January 1,
 * 1904. The value is represented as a signed 64-bit integer. */
ساخت LONGDATETIME
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }
  محفوظ:
  HBINT32 major;
  HBUINT32 minor;
  عوامی:
  DEFINE_سائز_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
ساخت لیبل : HBUINT32
{
  لیبل& عامل = (hb_لیبل_t i) { HBUINT32::عامل= (i); واپس *یہ; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  عامل ثابت حرف* () ثابت { واپس reinterpret_cast<ثابت حرف *> (یہ); }
  عامل حرف* ()             { واپس reinterpret_cast<حرف *> (یہ); }
  عوامی:
  DEFINE_سائز_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
ساخت HBGlyphID16 : HBUINT16
{
  HBGlyphID16& عامل = (uint16_t i) { HBUINT16::عامل= (i); واپس *یہ; }
};
ساخت HBGlyphID24 : HBUINT24
{
  HBGlyphID24& عامل = (uint32_t i) { HBUINT24::عامل= (i); واپس *یہ; }
};

/* Script/language-system/feature index */
ساخت اشاریہ : HBUINT16 {
  ساکن constexpr غیرعلامتی NOT_ملا_اشاریہ = 0xFFFFu;
  اشاریہ& عامل = (uint16_t i) { HBUINT16::عامل= (i); واپس *یہ; }
};
DECLARE_NULL_NAMESPACE_بائٹیں (OT, اشاریہ);

قسمتعریف اشاریہ NameID;

ساخت VarIdx : HBUINT32 {
  ساکن constexpr غیرعلامتی NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ, "");
  VarIdx& عامل = (uint32_t i) { HBUINT32::عامل= (i); واپس *یہ; }
};
DECLARE_NULL_NAMESPACE_بائٹیں (OT, VarIdx);

/* Offset, Null offset = 0 */
سانچہ <قسم_نام Type, منطقی has_null=سچ>
ساخت انحراف_2 : Type
{
  انحراف_2& عامل = (قسم_نام Type::type i) { Type::عامل= (i); واپس *یہ; }

  قسمتعریف Type type;

  منطقی is_null () ثابت { واپس has_null && 0 == *یہ; }

  عوامی:
  DEFINE_سائز_STATIC (حجم (Type));
};

قسمتعریف انحراف_2<HBUINT16> Offset16;
قسمتعریف انحراف_2<HBUINT24> Offset24;
قسمتعریف انحراف_2<HBUINT32> Offset32;


/* CheckSum */
ساخت Check_مجموع : HBUINT32
{
  Check_مجموع& عامل = (uint32_t i) { HBUINT32::عامل= (i); واپس *یہ; }

  /* This is reference implementation from the spec. */
  ساکن uint32_t Calc_جدول_Checksum (ثابت HBUINT32 *جدول_3, uint32_t لمبائی_3)
  {
    uint32_t Sum = 0L;
    أكد (0 == (لمبائی_3 & 3));
    ثابت HBUINT32 *انتہا_Ptr = جدول_3 + لمبائی_3 / HBUINT32::static_سائز;

    جب (جدول_3 < انتہا_Ptr)
      Sum += *جدول_3++;
    واپس Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  خالی سیٹ_for_ڈیٹا (ثابت خالی *ڈیٹا, غیرعلامتی صحیح لمبائی)
  { *یہ = Calc_جدول_Checksum ((ثابت HBUINT32 *) ڈیٹا, لمبائی); }

  عوامی:
  DEFINE_سائز_STATIC (4);
};


/*
 * Version Numbers
 */

سانچہ <قسم_نام FixedType=HBUINT16>
ساخت Fixed_نسخہ
{
  uint32_t مقصد_int () ثابت { واپس (major << (حجم (FixedType) * 8)) + minor; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  FixedType major;
  FixedType minor;
  عوامی:
  DEFINE_سائز_STATIC (2 * حجم (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

سانچہ <قسم_نام Type, منطقی has_null>
ساخت _hb_has_null
{
  ساکن ثابت Type *get_null () { واپس خالی_اشاریہ; }
  ساکن Type *get_crap ()       { واپس خالی_اشاریہ; }
};
سانچہ <قسم_نام Type>
ساخت _hb_has_null<Type, سچ>
{
  ساکن ثابت Type *get_null () { واپس &Null (Type); }
  ساکن       Type *get_crap () { واپس &Crap (Type); }
};

سانچہ <قسم_نام Type, قسم_نام انحراف_Type, قسم_نام بنیاد_Type=خالی, منطقی has_null=سچ>
ساخت انحراف_مقصد : انحراف_2<انحراف_Type, has_null>
{
  استعمال ہدف_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == جھوٹ ||
		 (hb_has_null_سائز_2 (Type) || !hb_has_min_سائز_2 (Type)), "");

  HB_DELETE_COPY_تفویض (انحراف_مقصد);
  انحراف_مقصد () = طےشدہ;

  انحراف_مقصد& عامل = (قسم_نام انحراف_Type::type i) { انحراف_Type::عامل= (i); واپس *یہ; }

  ثابت Type& عامل () (ثابت خالی *بنیاد) ثابت
  {
    اگر (unlikely (یہ->is_null ())) واپس *_hb_has_null<Type, has_null>::get_null ();
    واپس Struct_At_انحراف<ثابت Type> (بنیاد, *یہ);
  }
  Type& عامل () (خالی *بنیاد) ثابت
  {
    اگر (unlikely (یہ->is_null ())) واپس *_hb_has_null<Type, has_null>::get_crap ();
    واپس Struct_At_انحراف<Type> (بنیاد, *یہ);
  }

  سانچہ <قسم_نام بنیاد_3,
	    hb_enable_if (hb_is_convertible (ثابت بنیاد_3, ثابت بنیاد_Type *))>
  دوست ثابت Type& عامل + (ثابت بنیاد_3 &بنیاد, ثابت انحراف_مقصد &انحراف) { واپس انحراف ((ثابت خالی *) بنیاد); }
  سانچہ <قسم_نام بنیاد_3,
	    hb_enable_if (hb_is_convertible (ثابت بنیاد_3, ثابت بنیاد_Type *))>
  دوست ثابت Type& عامل + (ثابت انحراف_مقصد &انحراف, ثابت بنیاد_3 &بنیاد) { واپس انحراف ((ثابت خالی *) بنیاد); }
  سانچہ <قسم_نام بنیاد_3,
	    hb_enable_if (hb_is_convertible (بنیاد_3, بنیاد_Type *))>
  دوست Type& عامل + (بنیاد_3 &&بنیاد, انحراف_مقصد &انحراف) { واپس انحراف ((خالی *) بنیاد); }
  سانچہ <قسم_نام بنیاد_3,
	    hb_enable_if (hb_is_convertible (بنیاد_3, بنیاد_Type *))>
  دوست Type& عامل + (انحراف_مقصد &انحراف, بنیاد_3 &&بنیاد) { واپس انحراف ((خالی *) بنیاد); }


  سانچہ <قسم_نام بنیاد_3, قسم_نام ...Ts>
  منطقی serialize_subset (hb_subset_context_t *c, ثابت انحراف_مقصد& src,
			 ثابت بنیاد_3 *src_بنیاد, Ts&&... ds)
  {
    *یہ = 0;
    اگر (src.is_null ())
      واپس جھوٹ;

    خودکار *s = c->serializer;

    s->push ();

    منطقی ret = c->dispatch (src_بنیاد+src, std::forward<Ts> (ds)...);

    اگر (ret || !has_null)
      s->جوڑو_link (*یہ, s->نکالو_pack ());
    ورنہ
      s->نکالو_discard ();

    واپس ret;
  }


  سانچہ <قسم_نام ...Ts>
  منطقی serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *یہ = 0;

    Type* obj = c->push<Type> ();
    منطقی ret = obj->serialize (c, std::forward<Ts> (ds)...);

    اگر (ret)
      c->جوڑو_link (*یہ, c->نکالو_pack ());
    ورنہ
      c->نکالو_discard ();

    واپس ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  سانچہ <قسم_نام ...Ts>
  منطقی serialize_copy (hb_serialize_context_t *c, ثابت انحراف_مقصد& src,
		       ثابت خالی *src_بنیاد, غیرعلامتی dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *یہ = 0;
    اگر (src.is_null ())
      واپس جھوٹ;

    c->push ();

    منطقی ret = c->copy (src_بنیاد+src, std::forward<Ts> (ds)...);

    c->جوڑو_link (*یہ, c->نکالو_pack (), whence, dst_bias);

    واپس ret;
  }

  منطقی serialize_copy (hb_serialize_context_t *c, ثابت انحراف_مقصد& src,
		       ثابت خالی *src_بنیاد, غیرعلامتی dst_bias = 0)
  { واپس serialize_copy (c, src, src_بنیاد, dst_bias, hb_serialize_context_t::سر); }

  منطقی sanitize_shallow (hb_sanitize_context_t *c, ثابت بنیاد_Type *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!c->check_struct (یہ))) return_trace (جھوٹ);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    اگر (unlikely ((ثابت حرف *) بنیاد + (غیرعلامتی) *یہ < (ثابت حرف *) بنیاد)) return_trace (جھوٹ);
    return_trace (سچ);
  }

  سانچہ <قسم_نام ...Ts>
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  منطقی sanitize (hb_sanitize_context_t *c, ثابت بنیاد_Type *بنیاد, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (sanitize_shallow (c, بنیاد) &&
		  hb_barrier () &&
		  (یہ->is_null () ||
		   c->dispatch (Struct_At_انحراف<Type> (بنیاد, *یہ), std::forward<Ts> (ds)...)));
  }

  DEFINE_سائز_STATIC (حجم (انحراف_Type));
};
/* Partial specializations. */
سانچہ <قسم_نام Type, قسم_نام بنیاد_Type=خالی, منطقی has_null=سچ> استعمال Offset16_مقصد = انحراف_مقصد<Type, HBUINT16, بنیاد_Type, has_null>;
سانچہ <قسم_نام Type, قسم_نام بنیاد_Type=خالی, منطقی has_null=سچ> استعمال Offset24_مقصد = انحراف_مقصد<Type, HBUINT24, بنیاد_Type, has_null>;
سانچہ <قسم_نام Type, قسم_نام بنیاد_Type=خالی, منطقی has_null=سچ> استعمال Offset32_مقصد = انحراف_مقصد<Type, HBUINT32, بنیاد_Type, has_null>;

سانچہ <قسم_نام Type, قسم_نام انحراف_Type, قسم_نام بنیاد_Type=خالی> استعمال NNOffset_مقصد = انحراف_مقصد<Type, انحراف_Type, بنیاد_Type, جھوٹ>;
سانچہ <قسم_نام Type, قسم_نام بنیاد_Type=خالی> استعمال NNOffset16_مقصد = Offset16_مقصد<Type, بنیاد_Type, جھوٹ>;
سانچہ <قسم_نام Type, قسم_نام بنیاد_Type=خالی> استعمال NNOffset24_مقصد = Offset24_مقصد<Type, بنیاد_Type, جھوٹ>;
سانچہ <قسم_نام Type, قسم_نام بنیاد_Type=خالی> استعمال NNOffset32_مقصد = Offset32_مقصد<Type, بنیاد_Type, جھوٹ>;


/*
 * Array Types
 */

سانچہ <قسم_نام Type>
ساخت Unsized_Array_کا
{
  قسمتعریف Type شے_t;
  ساکن constexpr غیرعلامتی شے_سائز = hb_static_سائز (Type);

  HB_DELETE_CREATE_COPY_تفویض (Unsized_Array_کا);

  ثابت Type& عامل [] (غیرعلامتی صحیح i) ثابت
  {
    واپس arrayZ[i];
  }
  Type& عامل [] (غیرعلامتی صحیح i)
  {
    واپس arrayZ[i];
  }

  ساکن size_t get_سائز (غیرعلامتی صحیح len)
  { واپس hb_unsigned_mul_saturate (len, Type::static_سائز); }

  سانچہ <قسم_نام T> عامل T * () { واپس arrayZ; }
  سانچہ <قسم_نام T> عامل ثابت T * () ثابت { واپس arrayZ; }
  hb_array_t<Type> as_array (غیرعلامتی صحیح len)
  { واپس hb_array (arrayZ, len); }
  hb_array_t<ثابت Type> as_array (غیرعلامتی صحیح len) ثابت
  { واپس hb_array (arrayZ, len); }

  سانچہ <قسم_نام T>
  Type &lsearch (غیرعلامتی صحیح len, ثابت T &x, Type &not_ملا = Crap (Type))
  { واپس *as_array (len).lsearch (x, &not_ملا); }
  سانچہ <قسم_نام T>
  ثابت Type &lsearch (غیرعلامتی صحیح len, ثابت T &x, ثابت Type &not_ملا = Null (Type)) ثابت
  { واپس *as_array (len).lsearch (x, &not_ملا); }
  سانچہ <قسم_نام T>
  منطقی lfind (غیرعلامتی صحیح len, ثابت T &x, غیرعلامتی صحیح *i = خالی_اشاریہ,
	      hb_not_ملا_t not_ملا = HB_NOT_ملا_DONT_STORE,
	      غیرعلامتی صحیح مقصد_store = (غیرعلامتی صحیح) -1) ثابت
  { واپس as_array (len).lfind (x, i, not_ملا, مقصد_store); }

  خالی qsort (غیرعلامتی صحیح len, غیرعلامتی صحیح آغاز = 0, غیرعلامتی صحیح انتہا = (غیرعلامتی صحیح) -1)
  { as_array (len).qsort (آغاز, انتہا); }

  منطقی serialize (hb_serialize_context_t *c, غیرعلامتی صحیح اشیاء_len, منطقی clear = سچ)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_سائز (یہ, get_سائز (اشیاء_len), clear))) return_trace (جھوٹ);
    return_trace (سچ);
  }
  سانچہ <قسم_نام Iterator,
	    hb_requires (hb_is_ماخذ_کا (Iterator, Type))>
  منطقی serialize (hb_serialize_context_t *c, Iterator اشیاء)
  {
    TRACE_SERIALIZE (یہ);
    غیرعلامتی گنتی = hb_len (اشیاء);
    اگر (unlikely (!serialize (c, گنتی, جھوٹ))) return_trace (جھوٹ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    برائے (غیرعلامتی i = 0; i < گنتی; i++, ++اشیاء)
      arrayZ[i] = *اشیاء;
    return_trace (سچ);
  }

  Unsized_Array_کا* copy (hb_serialize_context_t *c, غیرعلامتی گنتی) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار *برآمد = c->آغاز_embed (یہ);
    اگر (unlikely (!as_array (گنتی).copy (c))) return_trace (خالی_اشاریہ);
    return_trace (برآمد);
  }

  سانچہ <قسم_نام ...Ts>
  HB_ALWAYS_INLINE
  منطقی sanitize (hb_sanitize_context_t *c, غیرعلامتی صحیح گنتی, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!sanitize_shallow (c, گنتی))) return_trace (جھوٹ);
    اگر (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (سچ);
    hb_barrier ();
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (جھوٹ);
    return_trace (سچ);
  }

  منطقی sanitize_shallow (hb_sanitize_context_t *c, غیرعلامتی صحیح گنتی) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_array (arrayZ, گنتی));
  }

  عوامی:
  Type		arrayZ[HB_VAR_ARRAY];
  عوامی:
  DEFINE_سائز_UNBOUNDED (0);
};

/* Unsized array of offset's */
سانچہ <قسم_نام Type, قسم_نام انحراف_Type, قسم_نام بنیاد_Type=خالی, منطقی has_null=سچ>
استعمال Unsized_Array16_کا_انحراف_مقصد = Unsized_Array_کا<انحراف_مقصد<Type, انحراف_Type, بنیاد_Type, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
سانچہ <قسم_نام Type, قسم_نام انحراف_Type, قسم_نام بنیاد_Type=خالی, منطقی has_null=سچ>
ساخت Unsized_List_کا_Offset16_مقصد : Unsized_Array16_کا_انحراف_مقصد<Type, انحراف_Type, بنیاد_Type, has_null>
{
  ثابت Type& عامل [] (صحیح i_) ثابت
  {
    غیرعلامتی صحیح i = (غیرعلامتی صحیح) i_;
    ثابت انحراف_مقصد<Type, انحراف_Type, بنیاد_Type, has_null> *p = &یہ->arrayZ[i];
    اگر (unlikely ((ثابت خالی *) p < (ثابت خالی *) یہ->arrayZ)) واپس Null (Type); /* Overflowed. */
    hb_barrier ();
    واپس یہ+*p;
  }
  Type& عامل [] (صحیح i_)
  {
    غیرعلامتی صحیح i = (غیرعلامتی صحیح) i_;
    ثابت انحراف_مقصد<Type, انحراف_Type, بنیاد_Type, has_null> *p = &یہ->arrayZ[i];
    اگر (unlikely ((ثابت خالی *) p < (ثابت خالی *) یہ->arrayZ)) واپس Crap (Type); /* Overflowed. */
    hb_barrier ();
    واپس یہ+*p;
  }

  سانچہ <قسم_نام ...Ts>
  منطقی sanitize (hb_sanitize_context_t *c, غیرعلامتی صحیح گنتی, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace ((Unsized_Array16_کا_انحراف_مقصد<Type, انحراف_Type, بنیاد_Type, has_null>
		   ::sanitize (c, گنتی, یہ, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
سانچہ <قسم_نام Type>
ساخت Sorted_Unsized_Array_کا : Unsized_Array_کا<Type>
{
  hb_sorted_array_t<Type> as_array (غیرعلامتی صحیح len)
  { واپس hb_sorted_array (یہ->arrayZ, len); }
  hb_sorted_array_t<ثابت Type> as_array (غیرعلامتی صحیح len) ثابت
  { واپس hb_sorted_array (یہ->arrayZ, len); }
  عامل hb_sorted_array_t<Type> ()             { واپس as_array (); }
  عامل hb_sorted_array_t<ثابت Type> () ثابت { واپس as_array (); }

  سانچہ <قسم_نام T>
  Type &bsearch (غیرعلامتی صحیح len, ثابت T &x, Type &not_ملا = Crap (Type))
  { واپس *as_array (len).bsearch (x, &not_ملا); }
  سانچہ <قسم_نام T>
  ثابت Type &bsearch (غیرعلامتی صحیح len, ثابت T &x, ثابت Type &not_ملا = Null (Type)) ثابت
  { واپس *as_array (len).bsearch (x, &not_ملا); }
  سانچہ <قسم_نام T>
  منطقی bfind (غیرعلامتی صحیح len, ثابت T &x, غیرعلامتی صحیح *i = خالی_اشاریہ,
	      hb_not_ملا_t not_ملا = HB_NOT_ملا_DONT_STORE,
	      غیرعلامتی صحیح مقصد_store = (غیرعلامتی صحیح) -1) ثابت
  { واپس as_array (len).bfind (x, i, not_ملا, مقصد_store); }
};


/* An array with a number of elements. */
سانچہ <قسم_نام Type, قسم_نام LenType>
ساخت Array_کا
{
  قسمتعریف Type شے_t;
  ساکن constexpr غیرعلامتی شے_سائز = hb_static_سائز (Type);

  HB_DELETE_CREATE_COPY_تفویض (Array_کا);

  ثابت Type& عامل [] (صحیح i_) ثابت
  {
    غیرعلامتی صحیح i = (غیرعلامتی صحیح) i_;
    اگر (unlikely (i >= len)) واپس Null (Type);
    hb_barrier ();
    واپس arrayZ[i];
  }
  Type& عامل [] (صحیح i_)
  {
    غیرعلامتی صحیح i = (غیرعلامتی صحیح) i_;
    اگر (unlikely (i >= len)) واپس Crap (Type);
    hb_barrier ();
    واپس arrayZ[i];
  }

  size_t get_سائز () ثابت
  { واپس hb_unsigned_mul_جوڑو_saturate (len, Type::static_سائز, len.static_سائز); }

  صریح عامل منطقی () ثابت { واپس len; }

  خالی نکالو () { len--; }

  hb_array_t<      Type> as_array ()       { واپس hb_array (arrayZ, len); }
  hb_array_t<ثابت Type> as_array () ثابت { واپس hb_array (arrayZ, len); }

  /* Iterator. */
  قسمتعریف hb_array_t<ثابت Type>   iter_t;
  قسمتعریف hb_array_t<      Type> writer_t;
    iter_t   iter () ثابت { واپس as_array (); }
  writer_t writer ()       { واپس as_array (); }
  عامل   iter_t () ثابت { واپس   iter (); }
  عامل writer_t ()       { واپس writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { واپس arrayZ; }
  ثابت Type *انتہا () ثابت { واپس arrayZ + len; }

  سانچہ <قسم_نام T>
  Type &lsearch (ثابت T &x, Type &not_ملا = Crap (Type))
  { واپس *as_array ().lsearch (x, &not_ملا); }
  سانچہ <قسم_نام T>
  ثابت Type &lsearch (ثابت T &x, ثابت Type &not_ملا = Null (Type)) ثابت
  { واپس *as_array ().lsearch (x, &not_ملا); }
  سانچہ <قسم_نام T>
  منطقی lfind (ثابت T &x, غیرعلامتی صحیح *i = خالی_اشاریہ,
	      hb_not_ملا_t not_ملا = HB_NOT_ملا_DONT_STORE,
	      غیرعلامتی صحیح مقصد_store = (غیرعلامتی صحیح) -1) ثابت
  { واپس as_array ().lfind (x, i, not_ملا, مقصد_store); }

  خالی qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD منطقی serialize (hb_serialize_context_t *c, غیرعلامتی اشیاء_len, منطقی clear = سچ)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    c->check_تفویض (len, اشیاء_len, HB_SERIALIZE_ERROR_ARRAY_حد_سے_تجاوز);
    اگر (unlikely (!c->extend_سائز (یہ, get_سائز (), clear))) return_trace (جھوٹ);
    return_trace (سچ);
  }
  سانچہ <قسم_نام Iterator,
	    hb_requires (hb_is_ماخذ_کا (Iterator, Type))>
  HB_NODISCARD منطقی serialize (hb_serialize_context_t *c, Iterator اشیاء)
  {
    TRACE_SERIALIZE (یہ);
    غیرعلامتی گنتی = hb_len (اشیاء);
    اگر (unlikely (!serialize (c, گنتی, جھوٹ))) return_trace (جھوٹ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    برائے (غیرعلامتی i = 0; i < گنتی; i++, ++اشیاء)
      arrayZ[i] = *اشیاء;
    return_trace (سچ);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (یہ);
    len++;
    اگر (unlikely (!len || !c->extend (یہ)))
    {
      len--;
      return_trace (خالی_اشاریہ);
    }
    return_trace (&arrayZ[len - 1]);
  }

  Array_کا* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار *برآمد = c->آغاز_embed (یہ);
    اگر (unlikely (!c->extend_min (برآمد))) return_trace (خالی_اشاریہ);
    c->check_تفویض (برآمد->len, len, HB_SERIALIZE_ERROR_ARRAY_حد_سے_تجاوز);
    اگر (unlikely (!as_array ().copy (c))) return_trace (خالی_اشاریہ);
    return_trace (برآمد);
  }

  سانچہ <قسم_نام ...Ts>
  HB_ALWAYS_INLINE
  منطقی sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!sanitize_shallow (c))) return_trace (جھوٹ);
    اگر (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (سچ);
    hb_barrier ();
    غیرعلامتی صحیح گنتی = len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (جھوٹ);
    return_trace (سچ);
  }

  منطقی sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (len.sanitize (c) &&
		  hb_barrier () &&
		  c->check_array_sized (arrayZ, len, حجم (LenType)));
  }

  عوامی:
  LenType	len;
  Type		arrayZ[HB_VAR_ARRAY];
  عوامی:
  DEFINE_سائز_ARRAY (حجم (LenType), arrayZ);
};
سانچہ <قسم_نام Type> استعمال Array16_کا = Array_کا<Type, HBUINT16>;
سانچہ <قسم_نام Type> استعمال Array24_کا = Array_کا<Type, HBUINT24>;
سانچہ <قسم_نام Type> استعمال Array32_کا = Array_کا<Type, HBUINT32>;
استعمال PString = Array_کا<HBUINT8, HBUINT8>;

/* Array of Offset's */
سانچہ <قسم_نام Type> استعمال Array8_کا_Offset24_مقصد = Array_کا<انحراف_مقصد<Type, HBUINT24>, HBUINT8>;
سانچہ <قسم_نام Type> استعمال Array16_کا_Offset16_مقصد = Array_کا<انحراف_مقصد<Type, HBUINT16>, HBUINT16>;
سانچہ <قسم_نام Type> استعمال Array16_کا_Offset32_مقصد = Array_کا<انحراف_مقصد<Type, HBUINT32>, HBUINT16>;
سانچہ <قسم_نام Type> استعمال Array32_کا_Offset32_مقصد = Array_کا<انحراف_مقصد<Type, HBUINT32>, HBUINT32>;

/* Array of offsets relative to the beginning of the array itself. */
سانچہ <قسم_نام Type, قسم_نام انحراف_Type>
ساخت List16_کا_انحراف_مقصد : Array_کا<انحراف_مقصد<Type, انحراف_Type>, HBUINT16>
{
  ثابت Type& عامل [] (صحیح i_) ثابت
  {
    غیرعلامتی صحیح i = (غیرعلامتی صحیح) i_;
    اگر (unlikely (i >= یہ->len)) واپس Null (Type);
    hb_barrier ();
    واپس یہ+یہ->arrayZ[i];
  }
  ثابت Type& عامل [] (صحیح i_)
  {
    غیرعلامتی صحیح i = (غیرعلامتی صحیح) i_;
    اگر (unlikely (i >= یہ->len)) واپس Crap (Type);
    hb_barrier ();
    واپس یہ+یہ->arrayZ[i];
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ساخت List16_کا_انحراف_مقصد *برآمد = c->serializer->embed (*یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);
    غیرعلامتی صحیح گنتی = یہ->len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      برآمد->arrayZ[i].serialize_subset (c, یہ->arrayZ[i], یہ, برآمد);
    return_trace (سچ);
  }

  سانچہ <قسم_نام ...Ts>
  منطقی sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace ((Array16_کا<انحراف_مقصد<Type, انحراف_Type>>::sanitize (c, یہ, std::forward<Ts> (ds)...)));
  }
};

سانچہ <قسم_نام Type>
استعمال List16_کا_Offset16_مقصد = List16_کا_انحراف_مقصد<Type, HBUINT16>;

/* An array starting at second element. */
سانچہ <قسم_نام Type, قسم_نام LenType>
ساخت Headless_Array_کا
{
  ساکن constexpr غیرعلامتی شے_سائز = Type::static_سائز;

  HB_DELETE_CREATE_COPY_تفویض (Headless_Array_کا);

  ثابت Type& عامل [] (صحیح i_) ثابت
  {
    غیرعلامتی صحیح i = (غیرعلامتی صحیح) i_;
    اگر (unlikely (i >= lenP1 || !i)) واپس Null (Type);
    hb_barrier ();
    واپس arrayZ[i-1];
  }
  Type& عامل [] (صحیح i_)
  {
    غیرعلامتی صحیح i = (غیرعلامتی صحیح) i_;
    اگر (unlikely (i >= lenP1 || !i)) واپس Crap (Type);
    hb_barrier ();
    واپس arrayZ[i-1];
  }
  size_t get_سائز () ثابت
  { واپس hb_unsigned_mul_جوڑو_saturate (get_لمبائی (), Type::static_سائز, lenP1.static_سائز); }

  غیرعلامتی get_لمبائی () ثابت { واپس lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { واپس hb_array (arrayZ, get_لمبائی ()); }
  hb_array_t<ثابت Type> as_array () ثابت { واپس hb_array (arrayZ, get_لمبائی ()); }

  /* Iterator. */
  قسمتعریف hb_array_t<ثابت Type>   iter_t;
  قسمتعریف hb_array_t<      Type> writer_t;
    iter_t   iter () ثابت { واپس as_array (); }
  writer_t writer ()       { واپس as_array (); }
  عامل   iter_t () ثابت { واپس   iter (); }
  عامل writer_t ()       { واپس writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { واپس arrayZ; }
  ثابت Type *انتہا () ثابت { واپس arrayZ + get_لمبائی (); }

  HB_NODISCARD منطقی serialize (hb_serialize_context_t *c, غیرعلامتی صحیح اشیاء_len, منطقی clear = سچ)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    c->check_تفویض (lenP1, اشیاء_len + 1, HB_SERIALIZE_ERROR_ARRAY_حد_سے_تجاوز);
    اگر (unlikely (!c->extend_سائز (یہ, get_سائز (), clear))) return_trace (جھوٹ);
    return_trace (سچ);
  }
  سانچہ <قسم_نام Iterator,
	    hb_requires (hb_is_ماخذ_کا (Iterator, Type))>
  HB_NODISCARD منطقی serialize (hb_serialize_context_t *c, Iterator اشیاء)
  {
    TRACE_SERIALIZE (یہ);
    غیرعلامتی گنتی = hb_len (اشیاء);
    اگر (unlikely (!serialize (c, گنتی, جھوٹ))) return_trace (جھوٹ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    برائے (غیرعلامتی i = 0; i < گنتی; i++, ++اشیاء)
      arrayZ[i] = *اشیاء;
    return_trace (سچ);
  }

  سانچہ <قسم_نام ...Ts>
  HB_ALWAYS_INLINE
  منطقی sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!sanitize_shallow (c))) return_trace (جھوٹ);
    اگر (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (سچ);
    hb_barrier ();
    غیرعلامتی صحیح گنتی = get_لمبائی ();
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (جھوٹ);
    return_trace (سچ);
  }

  نجی:
  منطقی sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (lenP1.sanitize (c) &&
		  hb_barrier () &&
		  (!lenP1 || c->check_array_sized (arrayZ, lenP1 - 1, حجم (LenType))));
  }

  عوامی:
  LenType	lenP1;
  Type		arrayZ[HB_VAR_ARRAY];
  عوامی:
  DEFINE_سائز_ARRAY (حجم (LenType), arrayZ);
};
سانچہ <قسم_نام Type> استعمال Headless_Array16_کا = Headless_Array_کا<Type, HBUINT16>;

/* An array storing length-1. */
سانچہ <قسم_نام Type, قسم_نام LenType=HBUINT16>
ساخت Array_کا_M1
{
  HB_DELETE_CREATE_COPY_تفویض (Array_کا_M1);

  ثابت Type& عامل [] (صحیح i_) ثابت
  {
    غیرعلامتی صحیح i = (غیرعلامتی صحیح) i_;
    اگر (unlikely (i > lenM1)) واپس Null (Type);
    hb_barrier ();
    واپس arrayZ[i];
  }
  Type& عامل [] (صحیح i_)
  {
    غیرعلامتی صحیح i = (غیرعلامتی صحیح) i_;
    اگر (unlikely (i > lenM1)) واپس Crap (Type);
    hb_barrier ();
    واپس arrayZ[i];
  }
  size_t get_سائز () ثابت
  { واپس hb_unsigned_mul_جوڑو_saturate (lenM1 + 1, Type::static_سائز, lenM1.static_سائز); }

  سانچہ <قسم_نام ...Ts>
  HB_ALWAYS_INLINE
  منطقی sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!sanitize_shallow (c))) return_trace (جھوٹ);
    اگر (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (سچ);
    hb_barrier ();
    غیرعلامتی صحیح گنتی = lenM1 + 1;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (جھوٹ);
    return_trace (سچ);
  }

  نجی:
  منطقی sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (lenM1.sanitize (c) &&
		  hb_barrier () &&
		  (c->check_array_sized (arrayZ, lenM1 + 1, حجم (LenType))));
  }

  عوامی:
  LenType	lenM1;
  Type		arrayZ[HB_VAR_ARRAY];
  عوامی:
  DEFINE_سائز_ARRAY (حجم (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
سانچہ <قسم_نام Type, قسم_نام LenType>
ساخت Sorted_Array_کا : Array_کا<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { واپس hb_sorted_array (یہ->arrayZ, یہ->len); }
  hb_sorted_array_t<ثابت Type> as_array () ثابت { واپس hb_sorted_array (یہ->arrayZ, یہ->len); }

  /* Iterator. */
  قسمتعریف hb_sorted_array_t<ثابت Type>   iter_t;
  قسمتعریف hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () ثابت { واپس as_array (); }
  writer_t writer ()       { واپس as_array (); }
  عامل   iter_t () ثابت { واپس   iter (); }
  عامل writer_t ()       { واپس writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { واپس یہ->arrayZ; }
  ثابت Type *انتہا () ثابت { واپس یہ->arrayZ + یہ->len; }

  منطقی serialize (hb_serialize_context_t *c, غیرعلامتی صحیح اشیاء_len)
  {
    TRACE_SERIALIZE (یہ);
    منطقی ret = Array_کا<Type, LenType>::serialize (c, اشیاء_len);
    return_trace (ret);
  }
  سانچہ <قسم_نام Iterator,
	    hb_requires (hb_is_sorted_ماخذ_کا (Iterator, Type))>
  منطقی serialize (hb_serialize_context_t *c, Iterator اشیاء)
  {
    TRACE_SERIALIZE (یہ);
    منطقی ret = Array_کا<Type, LenType>::serialize (c, اشیاء);
    return_trace (ret);
  }

  Sorted_Array_کا* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (یہ);
    Sorted_Array_کا* برآمد = reinterpret_cast<Sorted_Array_کا *> (Array_کا<Type, LenType>::copy (c));
    return_trace (برآمد);
  }

  سانچہ <قسم_نام T>
  Type &bsearch (ثابت T &x, Type &not_ملا = Crap (Type))
  { واپس *as_array ().bsearch (x, &not_ملا); }
  سانچہ <قسم_نام T>
  ثابت Type &bsearch (ثابت T &x, ثابت Type &not_ملا = Null (Type)) ثابت
  { واپس *as_array ().bsearch (x, &not_ملا); }
  سانچہ <قسم_نام T>
  منطقی bfind (ثابت T &x, غیرعلامتی صحیح *i = خالی_اشاریہ,
	      hb_not_ملا_t not_ملا = HB_NOT_ملا_DONT_STORE,
	      غیرعلامتی صحیح مقصد_store = (غیرعلامتی صحیح) -1) ثابت
  { واپس as_array ().bfind (x, i, not_ملا, مقصد_store); }
};

سانچہ <قسم_نام Type> استعمال Sorted_Array16_کا = Sorted_Array_کا<Type, HBUINT16>;
سانچہ <قسم_نام Type> استعمال Sorted_Array24_کا = Sorted_Array_کا<Type, HBUINT24>;
سانچہ <قسم_نام Type> استعمال Sorted_Array32_کا = Sorted_Array_کا<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

سانچہ <قسم_نام LenType=HBUINT16>
ساخت Bin_تلاش_سرخی
{
  عامل uint32_t () ثابت { واپس len; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  Bin_تلاش_سرخی& عامل = (غیرعلامتی صحیح v)
  {
    len = v;
    أكد (len == v);
    اندراج_Selector = hb_max (1u, hb_ثنائی_ہندسہ_storage (v)) - 1;
    تلاش_Range = 16 * (1u << اندراج_Selector);
    range_سرکاؤ = v * 16 > تلاش_Range
		 ? 16 * v - تلاش_Range
		 : 0;
    واپس *یہ;
  }

  محفوظ:
  LenType	len;
  LenType	تلاش_Range;
  LenType	اندراج_Selector;
  LenType	range_سرکاؤ;

  عوامی:
  DEFINE_سائز_STATIC (8);
};

سانچہ <قسم_نام Type, قسم_نام LenType=HBUINT16>
استعمال Bin_تلاش_Array_کا = Sorted_Array_کا<Type, Bin_تلاش_سرخی<LenType>>;


ساخت Var_Sized_Bin_تلاش_سرخی
{

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  HBUINT16	unit_سائز;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	تلاش_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	اندراج_Selector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	range_سرکاؤ;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  عوامی:
  DEFINE_سائز_STATIC (10);
};

سانچہ <قسم_نام Type>
ساخت Var_Sized_Bin_تلاش_Array_کا
{
  ساکن constexpr غیرعلامتی شے_سائز = Type::static_سائز;

  HB_DELETE_CREATE_COPY_تفویض (Var_Sized_Bin_تلاش_Array_کا);

  منطقی آخری_is_terminator () ثابت
  {
    اگر (unlikely (!سرخی.nUnits)) واپس جھوٹ;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    ثابت HBUINT16 *الفاظ = &Struct_At_انحراف<HBUINT16> (&بائٹیں_Z, (سرخی.nUnits - 1) * سرخی.unit_سائز);
    غیرعلامتی صحیح گنتی = Type::Termination_لفظ_گنتی;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (الفاظ[i] != 0xFFFFu)
	واپس جھوٹ;
    واپس سچ;
  }

  ثابت Type& عامل [] (صحیح i_) ثابت
  {
    غیرعلامتی صحیح i = (غیرعلامتی صحیح) i_;
    اگر (unlikely (i >= get_لمبائی ())) واپس Null (Type);
    hb_barrier ();
    واپس Struct_At_انحراف<Type> (&بائٹیں_Z, i * سرخی.unit_سائز);
  }
  Type& عامل [] (صحیح i_)
  {
    غیرعلامتی صحیح i = (غیرعلامتی صحیح) i_;
    اگر (unlikely (i >= get_لمبائی ())) واپس Crap (Type);
    hb_barrier ();
    واپس Struct_At_انحراف<Type> (&بائٹیں_Z, i * سرخی.unit_سائز);
  }
  غیرعلامتی صحیح get_لمبائی () ثابت
  { واپس سرخی.nUnits - آخری_is_terminator (); }
  size_t get_سائز () ثابت
  { واپس hb_unsigned_mul_جوڑو_saturate (سرخی.nUnits, سرخی.unit_سائز, سرخی.static_سائز); }

  سانچہ <قسم_نام ...Ts>
  HB_ALWAYS_INLINE
  منطقی sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!sanitize_shallow (c))) return_trace (جھوٹ);
    اگر (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (سچ);
    hb_barrier ();
    غیرعلامتی صحیح گنتی = get_لمبائی ();
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (unlikely (!(*یہ)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (جھوٹ);
    return_trace (سچ);
  }

  سانچہ <قسم_نام T>
  ثابت Type *bsearch (ثابت T &key) ثابت
  {
    غیرعلامتی pos;
    واپس hb_bsearch_impl (&pos,
			    key,
			    (ثابت خالی *) بائٹیں_Z,
			    get_لمبائی (),
			    سرخی.unit_سائز,
			    hb_cmp_طریقہ<T, Type>)
	   ? (ثابت Type *) (((ثابت حرف *) &بائٹیں_Z) + (pos * سرخی.unit_سائز))
	   : خالی_اشاریہ;
  }

  نجی:
  منطقی sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (سرخی.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_سائز <= سرخی.unit_سائز &&
		  c->check_range (بائٹیں_Z.arrayZ,
				  سرخی.nUnits,
				  سرخی.unit_سائز));
  }

  محفوظ:
  Var_Sized_Bin_تلاش_سرخی	سرخی;
  Unsized_Array_کا<HBUINT8>	بائٹیں_Z;
  عوامی:
  DEFINE_سائز_ARRAY (10, بائٹیں_Z);
};


/* CFF INDEX */

سانچہ <قسم_نام گنتی_3>
ساخت CFFIndex
{
  غیرعلامتی صحیح انحراف_array_سائز () ثابت
  { واپس off_سائز * (گنتی + 1); }

  سانچہ <قسم_نام Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  منطقی serialize (hb_serialize_context_t *c,
		  ثابت Iterable &iterable,
		  ثابت غیرعلامتی *p_ڈیٹا_سائز = خالی_اشاریہ,
                  غیرعلامتی min_off_سائز = 0)
  {
    TRACE_SERIALIZE (یہ);
    غیرعلامتی ڈیٹا_سائز;
    اگر (p_ڈیٹا_سائز)
      ڈیٹا_سائز = *p_ڈیٹا_سائز;
    ورنہ
      کل_سائز (iterable, &ڈیٹا_سائز);

    خودکار it = hb_iter (iterable);
    اگر (unlikely (!serialize_سرخی (c, +it, ڈیٹا_سائز, min_off_سائز))) return_trace (جھوٹ);
    غیرعلامتی حرف *ret = c->allocate_سائز<غیرعلامتی حرف> (ڈیٹا_سائز, جھوٹ);
    اگر (unlikely (!ret)) return_trace (جھوٹ);
    غیرعلامتی بقایا = ڈیٹا_سائز;
    برائے (ثابت خودکار &_ : +it)
    {
      غیرعلامتی len = _.لمبائی;

      اگر (!len)
	جاری;

      اگر (unlikely (len > بقایا)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (جھوٹ, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
      }

      بقایا -= len;

      اگر (len <= 1)
      {
	*ret++ = *_.arrayZ;
	جاری;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (سچ);
  }

  سانچہ <قسم_نام Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  منطقی serialize_سرخی (hb_serialize_context_t *c,
			 Iterator it,
			 غیرعلامتی ڈیٹا_سائز,
                         غیرعلامتی min_off_سائز = 0)
  {
    TRACE_SERIALIZE (یہ);

    غیرعلامتی off_سائز_2 = (hb_ثنائی_ہندسہ_storage (ڈیٹا_سائز + 1) + 7) / 8;
    off_سائز_2 = hb_max(min_off_سائز, off_سائز_2);

    /* serialize CFFIndex header */
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    یہ->گنتی = hb_len (it);
    اگر (!یہ->گنتی) return_trace (سچ);
    اگر (unlikely (!c->extend (یہ->off_سائز))) return_trace (جھوٹ);
    یہ->off_سائز = off_سائز_2;
    اگر (unlikely (!c->allocate_سائز<HBUINT8> (off_سائز_2 * (یہ->گنتی + 1), جھوٹ)))
      return_trace (جھوٹ);

    /* serialize indices */
    غیرعلامتی صحیح انحراف = 1;
    اگر (HB_OPTIMIZE_سائز_VAL)
    {
      غیرعلامتی صحیح i = 0;
      برائے (ثابت خودکار &_ : +it)
      {
	سیٹ_انحراف_at (i++, انحراف);
	انحراف += hb_len_کا (_);
      }
      سیٹ_انحراف_at (i, انحراف);
    }
    ورنہ
      انتخاب (off_سائز_2)
      {
	حالت 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  برائے (ثابت خودکار &_ : +it)
	  {
	    *p++ = انحراف;
	    انحراف += hb_len_کا (_);
	  }
	  *p = انحراف;
	}
	توڑو;
	حالت 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  برائے (ثابت خودکار &_ : +it)
	  {
	    *p++ = انحراف;
	    انحراف += hb_len_کا (_);
	  }
	  *p = انحراف;
	}
	توڑو;
	حالت 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  برائے (ثابت خودکار &_ : +it)
	  {
	    *p++ = انحراف;
	    انحراف += hb_len_کا (_);
	  }
	  *p = انحراف;
	}
	توڑو;
	حالت 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  برائے (ثابت خودکار &_ : +it)
	  {
	    *p++ = انحراف;
	    انحراف += hb_len_کا (_);
	  }
	  *p = انحراف;
	}
	توڑو;
	طےشدہ:
	توڑو;
      }

    أكد (انحراف == ڈیٹا_سائز + 1);
    return_trace (سچ);
  }

  سانچہ <قسم_نام Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  ساکن غیرعلامتی کل_سائز (ثابت Iterable &iterable, غیرعلامتی *ڈیٹا_سائز = خالی_اشاریہ, غیرعلامتی min_off_سائز = 0)
  {
    خودکار it = + hb_iter (iterable);
    اگر (!it)
    {
      اگر (ڈیٹا_سائز) *ڈیٹا_سائز = 0;
      واپس min_سائز;
    }

    غیرعلامتی کل = 0;
    برائے (ثابت خودکار &_ : +it)
      کل += hb_len_کا (_);

    اگر (ڈیٹا_سائز) *ڈیٹا_سائز = کل;

    غیرعلامتی off_سائز_2 = (hb_ثنائی_ہندسہ_storage (کل + 1) + 7) / 8;
    off_سائز_2 = hb_max(min_off_سائز, off_سائز_2);

    واپس min_سائز + HBUINT8::static_سائز + (hb_len (it) + 1) * off_سائز_2 + کل;
  }

  خالی سیٹ_انحراف_at (غیرعلامتی صحیح اشاریہ_2, غیرعلامتی صحیح انحراف)
  {
    أكد (اشاریہ_2 <= گنتی);

    غیرعلامتی صحیح سائز = off_سائز;
    ثابت HBUINT8 *p = offsets;
    انتخاب (سائز)
    {
      حالت 1: ((HBUINT8  *) p)[اشاریہ_2] = انحراف; توڑو;
      حالت 2: ((HBUINT16 *) p)[اشاریہ_2] = انحراف; توڑو;
      حالت 3: ((HBUINT24 *) p)[اشاریہ_2] = انحراف; توڑو;
      حالت 4: ((HBUINT32 *) p)[اشاریہ_2] = انحراف; توڑو;
      طےشدہ: واپس;
    }
  }

  نجی:
  غیرعلامتی صحیح انحراف_at (غیرعلامتی صحیح اشاریہ_2) ثابت
  {
    أكد (اشاریہ_2 <= گنتی);

    غیرعلامتی صحیح سائز = off_سائز;
    ثابت HBUINT8 *p = offsets;
    انتخاب (سائز)
    {
      حالت 1: واپس ((HBUINT8  *) p)[اشاریہ_2];
      حالت 2: واپس ((HBUINT16 *) p)[اشاریہ_2];
      حالت 3: واپس ((HBUINT24 *) p)[اشاریہ_2];
      حالت 4: واپس ((HBUINT32 *) p)[اشاریہ_2];
      طےشدہ: واپس 0;
    }
  }

  ثابت غیرعلامتی حرف *ڈیٹا_بنیاد () ثابت
  { واپس (ثابت غیرعلامتی حرف *) یہ + min_سائز + off_سائز.static_سائز - 1 + انحراف_array_سائز (); }
  عوامی:

  hb_ubytes_t عامل [] (غیرعلامتی صحیح اشاریہ_2) ثابت
  {
    اگر (unlikely (اشاریہ_2 >= گنتی)) واپس hb_ubytes_t ();
    hb_barrier ();
    غیرعلامتی offset0 = انحراف_at (اشاریہ_2);
    غیرعلامتی offset1 = انحراف_at (اشاریہ_2 + 1);
    اگر (unlikely (offset1 < offset0 || offset1 > انحراف_at (گنتی)))
      واپس hb_ubytes_t ();
    واپس hb_ubytes_t (ڈیٹا_بنیاد () + offset0, offset1 - offset0);
  }

  size_t get_سائز () ثابت
  {
    اگر (گنتی)
      واپس hb_unsigned_جوڑو_saturate (min_سائز, off_سائز.static_سائز,
				       انحراف_array_سائز (), انحراف_at (گنتی) - 1);
    واپس min_سائز;  /* empty CFFIndex contains count only */
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  hb_barrier () &&
			  (گنتی == 0 || /* empty INDEX */
			   (گنتی < گنتی + 1u &&
			    c->check_struct (&off_سائز) && off_سائز >= 1 && off_سائز <= 4 &&
			    c->check_array (offsets, off_سائز, گنتی + 1u) &&
			    c->check_range (ڈیٹا_بنیاد (), انحراف_at (گنتی))))));
  }

  عوامی:
  گنتی_3		گنتی;		/* Number of object data. Note there are (count+1) offsets */
  نجی:
  HBUINT8	off_سائز;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  عوامی:
  DEFINE_سائز_MIN (گنتی_3::static_سائز);
};
قسمتعریف CFFIndex<HBUINT16> CFF1_اشاریہ;
قسمتعریف CFFIndex<HBUINT32> CFF2_اشاریہ;


/* TupleValues */
ساخت Tuple_اقدار
{
  شمار packed_قدر_flag_t
  {
    اقدار_ARE_ZEROS     = 0x80,
    اقدار_ARE_بائٹیں     = 0x00,
    اقدار_ARE_الفاظ     = 0x40,
    اقدار_ARE_LONGS     = 0xC0,
    اقدار_سائز_نقاب     = 0xC0,
    قدر_تسلسل_گنتی_نقاب = 0x3F
  };

  ساکن غیرعلامتی compile_unsafe (hb_array_t<ثابت صحیح> values, /*اندر*/
				  غیرعلامتی حرف *encoded_بائٹیں /*برآمد*/)
  {
    غیرعلامتی عدد_اقدار_3 = values.لمبائی;
    غیرعلامتی encoded_len = 0;
    غیرعلامتی i = 0;
    جب (i < عدد_اقدار_3)
    {
      صحیح val = values.arrayZ[i];
      اگر (val == 0)
        encoded_len += encode_قدر_تسلسل_as_zeroes (i, encoded_بائٹیں + encoded_len, values);
      ورنہ اگر ((int8_t) val == val)
        encoded_len += encode_قدر_تسلسل_as_بائٹیں (i, encoded_بائٹیں + encoded_len, values);
      ورنہ اگر ((int16_t) val == val)
        encoded_len += encode_قدر_تسلسل_as_الفاظ (i, encoded_بائٹیں + encoded_len, values);
      ورنہ
        encoded_len += encode_قدر_تسلسل_as_longs (i, encoded_بائٹیں + encoded_len, values);
    }
    واپس encoded_len;
  }

  ساکن غیرعلامتی encode_قدر_تسلسل_as_zeroes (غیرعلامتی& i,
					      غیرعلامتی حرف *it,
					      hb_array_t<ثابت صحیح> values)
  {
    غیرعلامتی عدد_اقدار_3 = values.لمبائی;
    غیرعلامتی تسلسل_لمبائی = 0;
    غیرعلامتی encoded_len = 0;
    جب (i < عدد_اقدار_3 && values.arrayZ[i] == 0)
    {
      i++;
      تسلسل_لمبائی++;
    }

    جب (تسلسل_لمبائی >= 64)
    {
      *it++ = حرف (اقدار_ARE_ZEROS | 63);
      تسلسل_لمبائی -= 64;
      encoded_len++;
    }

    اگر (تسلسل_لمبائی)
    {
      *it++ = حرف (اقدار_ARE_ZEROS | (تسلسل_لمبائی - 1));
      encoded_len++;
    }
    واپس encoded_len;
  }

  ساکن غیرعلامتی encode_قدر_تسلسل_as_بائٹیں (غیرعلامتی &i,
					     غیرعلامتی حرف *it,
					     hb_array_t<ثابت صحیح> values)
  {
    غیرعلامتی آغاز = i;
    غیرعلامتی عدد_اقدار_3 = values.لمبائی;
    جب (i < عدد_اقدار_3)
    {
      صحیح val = values.arrayZ[i];
      اگر ((int8_t) val != val)
        توڑو;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      اگر (val == 0 && i + 1 < عدد_اقدار_3 && values.arrayZ[i+1] == 0)
        توڑو;

      i++;
    }
    غیرعلامتی تسلسل_لمبائی = i - آغاز;

    غیرعلامتی encoded_len = 0;

    جب (تسلسل_لمبائی >= 64)
    {
      *it++ = (اقدار_ARE_بائٹیں | 63);
      encoded_len++;

      برائے (غیرعلامتی j = 0; j < 64; j++)
	it[j] = static_cast<حرف> (values.arrayZ[آغاز + j]);
      it += 64;
      encoded_len += 64;

      آغاز += 64;
      تسلسل_لمبائی -= 64;
    }

    اگر (تسلسل_لمبائی)
    {
      *it++ = (اقدار_ARE_بائٹیں | (تسلسل_لمبائی - 1));
      encoded_len++;

      برائے (غیرعلامتی j = 0; j < تسلسل_لمبائی; j++)
        it[j] = static_cast<حرف> (values.arrayZ[آغاز + j]);
      encoded_len += تسلسل_لمبائی;
    }

    واپس encoded_len;
  }

  ساکن غیرعلامتی encode_قدر_تسلسل_as_الفاظ (غیرعلامتی &i,
					     غیرعلامتی حرف *it,
					     hb_array_t<ثابت صحیح> values)
  {
    غیرعلامتی آغاز = i;
    غیرعلامتی عدد_اقدار_3 = values.لمبائی;
    جب (i < عدد_اقدار_3)
    {
      صحیح val = values.arrayZ[i];

      اگر ((int16_t) val != val)
        توڑو;

      /* start a new run for a single zero value. */
      اگر (val == 0) توڑو;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      اگر ((int8_t) val == val &&
          i + 1 < عدد_اقدار_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        توڑو;

      i++;
    }

    غیرعلامتی تسلسل_لمبائی = i - آغاز;
    غیرعلامتی encoded_len = 0;
    جب (تسلسل_لمبائی >= 64)
    {
      *it++ = (اقدار_ARE_الفاظ | 63);
      encoded_len++;

      برائے (غیرعلامتی j = 0; j < 64; j++)
      {
        int16_t قدر_val = values.arrayZ[آغاز + j];
        *it++ = static_cast<حرف> (قدر_val >> 8);
        *it++ = static_cast<حرف> (قدر_val & 0xFF);

        encoded_len += 2;
      }

      آغاز += 64;
      تسلسل_لمبائی -= 64;
    }

    اگر (تسلسل_لمبائی)
    {
      *it++ = (اقدار_ARE_الفاظ | (تسلسل_لمبائی - 1));
      encoded_len++;
      جب (آغاز < i)
      {
        int16_t قدر_val = values.arrayZ[آغاز++];
        *it++ = static_cast<حرف> (قدر_val >> 8);
        *it++ = static_cast<حرف> (قدر_val & 0xFF);

        encoded_len += 2;
      }
    }
    واپس encoded_len;
  }

  ساکن غیرعلامتی encode_قدر_تسلسل_as_longs (غیرعلامتی &i,
					     غیرعلامتی حرف *it,
					     hb_array_t<ثابت صحیح> values)
  {
    غیرعلامتی آغاز = i;
    غیرعلامتی عدد_اقدار_3 = values.لمبائی;
    جب (i < عدد_اقدار_3)
    {
      صحیح val = values.arrayZ[i];

      اگر ((int16_t) val == val)
        توڑو;

      i++;
    }

    غیرعلامتی تسلسل_لمبائی = i - آغاز;
    غیرعلامتی encoded_len = 0;
    جب (تسلسل_لمبائی >= 64)
    {
      *it++ = (اقدار_ARE_LONGS | 63);
      encoded_len++;

      برائے (غیرعلامتی j = 0; j < 64; j++)
      {
        int32_t قدر_val = values.arrayZ[آغاز + j];
        *it++ = static_cast<حرف> (قدر_val >> 24);
        *it++ = static_cast<حرف> (قدر_val >> 16);
        *it++ = static_cast<حرف> (قدر_val >> 8);
        *it++ = static_cast<حرف> (قدر_val & 0xFF);

        encoded_len += 4;
      }

      آغاز += 64;
      تسلسل_لمبائی -= 64;
    }

    اگر (تسلسل_لمبائی)
    {
      *it++ = (اقدار_ARE_LONGS | (تسلسل_لمبائی - 1));
      encoded_len++;
      جب (آغاز < i)
      {
        int32_t قدر_val = values.arrayZ[آغاز++];
        *it++ = static_cast<حرف> (قدر_val >> 24);
        *it++ = static_cast<حرف> (قدر_val >> 16);
        *it++ = static_cast<حرف> (قدر_val >> 8);
        *it++ = static_cast<حرف> (قدر_val & 0xFF);

        encoded_len += 4;
      }
    }
    واپس encoded_len;
  }

  سانچہ <قسم_نام T>
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  ساکن منطقی decompile (ثابت HBUINT8 *&p /* IN/OUT */,
			 hb_سمتیہ_t<T> &values /* IN/OUT */,
			 ثابت HBUINT8 *انتہا,
			 منطقی consume_all = جھوٹ,
			 غیرعلامتی آغاز = 0)
  {
    غیرعلامتی i = 0;
    غیرعلامتی گنتی = consume_all ? UINT_MAX : values.لمبائی;
    اگر (consume_all)
      values.alloc ((انتہا - p) / 2);
    جب (i < گنتی)
    {
      اگر (unlikely (p + 1 > انتہا)) واپس consume_all;
      غیرعلامتی control = *p++;
      غیرعلامتی تسلسل_گنتی = (control & قدر_تسلسل_گنتی_نقاب) + 1;
      اگر (consume_all)
      {
        اگر (unlikely (!values.resize_dirty  (values.لمبائی + تسلسل_گنتی)))
	  واپس جھوٹ;
      }
      غیرعلامتی stop = i + تسلسل_گنتی;
      اگر (unlikely (stop > گنتی)) واپس جھوٹ;

      غیرعلامتی skip = i < آغاز ? hb_min (آغاز - i, تسلسل_گنتی) : 0;
      i += skip;

      اگر ((control & اقدار_سائز_نقاب) == اقدار_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * حجم (T));
	i = stop;
      }
      ورنہ اگر ((control & اقدار_سائز_نقاب) ==  اقدار_ARE_الفاظ)
      {
        اگر (unlikely (p + تسلسل_گنتی * HBINT16::static_سائز > انتہا)) واپس جھوٹ;
	p += skip * HBINT16::static_سائز;
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
        برائے (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_سائز;
	  values.arrayZ[i + 1] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_سائز;
	  values.arrayZ[i + 2] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_سائز;
	  values.arrayZ[i + 3] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_سائز;
	}
#اگر_ختم
        برائے (; i < stop; i++)
        {
          values.arrayZ[i] = * (ثابت HBINT16 *) p;
          p += HBINT16::static_سائز;
        }
      }
      ورنہ اگر ((control & اقدار_سائز_نقاب) ==  اقدار_ARE_LONGS)
      {
        اگر (unlikely (p + تسلسل_گنتی * HBINT32::static_سائز > انتہا)) واپس جھوٹ;
	p += skip * HBINT32::static_سائز;
        برائے (; i < stop; i++)
        {
          values.arrayZ[i] = * (ثابت HBINT32 *) p;
          p += HBINT32::static_سائز;
        }
      }
      ورنہ اگر ((control & اقدار_سائز_نقاب) ==  اقدار_ARE_بائٹیں)
      {
        اگر (unlikely (p + تسلسل_گنتی > انتہا)) واپس جھوٹ;
	p += skip * HBINT8::static_سائز;
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
	برائے (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (ثابت HBINT8 *) p++;
	}
#اگر_ختم
        برائے (; i < stop; i++)
          values.arrayZ[i] = * (ثابت HBINT8 *) p++;
      }
    }
    واپس سچ;
  }

  ساکن منطقی skip (ثابت غیرعلامتی حرف *&p /* IN/OUT */,
		    ثابت غیرعلامتی حرف *انتہا,
		    غیرعلامتی گنتی)
  {
    غیرعلامتی نظر_آیا = 0;
    جب (نظر_آیا < گنتی)
    {
      اگر (unlikely (p >= انتہا)) واپس جھوٹ;
      غیرعلامتی control = *p++;
      غیرعلامتی تسلسل_گنتی = (control & قدر_تسلسل_گنتی_نقاب) + 1;
      اگر (unlikely (تسلسل_گنتی > گنتی - نظر_آیا)) واپس جھوٹ;

      غیرعلامتی چوڑائی = 0;
      انتخاب (control & اقدار_سائز_نقاب)
      {
	حالت اقدار_ARE_ZEROS: توڑو;
	حالت اقدار_ARE_بائٹیں: چوڑائی = HBINT8::static_سائز; توڑو;
	حالت اقدار_ARE_الفاظ: چوڑائی = HBINT16::static_سائز; توڑو;
	حالت اقدار_ARE_LONGS: چوڑائی = HBINT32::static_سائز; توڑو;
      }
      اگر (unlikely (غیرعلامتی (انتہا - p) < تسلسل_گنتی * چوڑائی)) واپس جھوٹ;
      p += تسلسل_گنتی * چوڑائی;
      نظر_آیا += تسلسل_گنتی;
    }
    واپس سچ;
  }

  ساخت iter_t : hb_iter_with_fallback_t<iter_t, صحیح>
  {
    iter_t (ثابت غیرعلامتی حرف *p_, غیرعلامتی len_)
	    : p (p_), endp (p_ + len_)
    { اگر (likely (ensure_تسلسل ())) read_قدر (); }

    نجی:
    ثابت غیرعلامتی حرف *p;
    ثابت غیرعلامتی حرف * ثابت endp;
    صحیح current_قدر = 0;
    علامتی تسلسل_گنتی = 0;
    غیرعلامتی چوڑائی = 0;

    HB_ALWAYS_INLINE
    منطقی ensure_تسلسل ()
    {
      اگر (likely (تسلسل_گنتی > 0)) واپس سچ;
      واپس ensure_تسلسل_2 ();
    }
    منطقی ensure_تسلسل_2 ()
    {
      اگر (unlikely (p >= endp))
      {
        تسلسل_گنتی = 0;
        current_قدر = 0;
	واپس جھوٹ;
      }

      غیرعلامتی control = *p++;
      تسلسل_گنتی = (control & قدر_تسلسل_گنتی_نقاب) + 1;
      چوڑائی = control & اقدار_سائز_نقاب;
      انتخاب (چوڑائی)
      {
        حالت اقدار_ARE_ZEROS: چوڑائی = 0; توڑو;
	حالت اقدار_ARE_بائٹیں: چوڑائی = HBINT8::static_سائز;  توڑو;
	حالت اقدار_ARE_الفاظ: چوڑائی = HBINT16::static_سائز; توڑو;
	حالت اقدار_ARE_LONGS: چوڑائی = HBINT32::static_سائز; توڑو;
	طےشدہ: أكد (جھوٹ);
      }

      اگر (unlikely (p + تسلسل_گنتی * چوڑائی > endp))
      {
	تسلسل_گنتی = 0;
	current_قدر = 0;
	واپس جھوٹ;
      }

      واپس سچ;
    }
    خالی read_قدر ()
    {
      انتخاب (چوڑائی)
      {
        حالت 0: current_قدر = 0; توڑو;
	حالت 1: current_قدر = * (ثابت HBINT8  *) p; توڑو;
	حالت 2: current_قدر = * (ثابت HBINT16 *) p; توڑو;
	حالت 4: current_قدر = * (ثابت HBINT32 *) p; توڑو;
      }
      p += چوڑائی;
    }

    عوامی:

    قسمتعریف صحیح __item_t__;
    __item_t__ __item__ () ثابت
    { واپس current_قدر; }

    منطقی __more__ () ثابت { واپس تسلسل_گنتی || p < endp; }
    خالی __next__ ()
    {
      تسلسل_گنتی--;
      اگر (unlikely (!ensure_تسلسل ()))
	واپس;
      read_قدر ();
    }
    خالی __forward__ (غیرعلامتی n)
    {
      اگر (unlikely (!ensure_تسلسل ()))
	واپس;
      جب (n)
      {
	غیرعلامتی i = hb_min (n, (غیرعلامتی) تسلسل_گنتی);
	تسلسل_گنتی -= i;
	n -= i;
	p += (i - 1) * چوڑائی;
	اگر (unlikely (!ensure_تسلسل ()))
	  واپس;
	read_قدر ();
      }
    }
    منطقی عامل != (ثابت iter_t& o) ثابت
    { واپس p != o.p || تسلسل_گنتی != o.تسلسل_گنتی; }
    iter_t __end__ () ثابت
    {
      iter_t it (endp, 0);
      واپس it;
    }
  };

  ساخت fetcher_t
  {
    fetcher_t (ثابت غیرعلامتی حرف *p_, غیرعلامتی len_)
	      : p (p_), انتہا (p_ + len_) {}

    نجی:
    ثابت غیرعلامتی حرف *p;
    ثابت غیرعلامتی حرف * ثابت انتہا;
    علامتی تسلسل_گنتی = 0;
    غیرعلامتی چوڑائی = 0;

    HB_ALWAYS_INLINE
    منطقی ensure_تسلسل ()
    {
      اگر (likely (تسلسل_گنتی > 0)) واپس سچ;
      واپس ensure_تسلسل_2 ();
    }

    منطقی ensure_تسلسل_2 ()
    {
      اگر (unlikely (p >= انتہا))
      {
        تسلسل_گنتی = 0;
	واپس جھوٹ;
      }

      غیرعلامتی control = *p++;
      تسلسل_گنتی = (control & قدر_تسلسل_گنتی_نقاب) + 1;
      چوڑائی = control & اقدار_سائز_نقاب;
      انتخاب (چوڑائی)
      {
        حالت اقدار_ARE_ZEROS: چوڑائی = 0; توڑو;
	حالت اقدار_ARE_بائٹیں: چوڑائی = HBINT8::static_سائز;  توڑو;
	حالت اقدار_ARE_الفاظ: چوڑائی = HBINT16::static_سائز; توڑو;
	حالت اقدار_ARE_LONGS: چوڑائی = HBINT32::static_سائز; توڑو;
	طےشدہ: أكد (جھوٹ);
      }

      اگر (unlikely (p + تسلسل_گنتی * چوڑائی > انتہا))
      {
	تسلسل_گنتی = 0;
	واپس جھوٹ;
      }

      واپس سچ;
    }

    عوامی:
    خالی skip (غیرعلامتی n)
    {
      جب (n)
      {
	اگر (unlikely (!ensure_تسلسل ()))
	  واپس;
	غیرعلامتی i = hb_min (n, (غیرعلامتی) تسلسل_گنتی);
	تسلسل_گنتی -= i;
	n -= i;
	p += i * چوڑائی;
      }
    }

    نجی:
    سانچہ <منطقی scaled, قسم_نام T>
    خالی جوڑو_مقصد (hb_array_t<T> برآمد, T scale = T (1))
    {
      غیرعلامتی n = برآمد.لمبائی;
      T *arrayZ = برآمد.arrayZ;

      برائے (غیرعلامتی i = 0; i < n;)
      {
	اگر (unlikely (!ensure_تسلسل ()))
	  توڑو;
	غیرعلامتی گنتی = hb_min (n - i, (غیرعلامتی) تسلسل_گنتی);
	انتخاب (چوڑائی)
	{
	  حالت 0:
	  {
	    arrayZ += گنتی;
	    توڑو;
	  }
	  حالت 1:
	  {
	    ثابت خودکار *pp = (ثابت HBINT8 *) p;
	    غیرعلامتی j = 0;
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
	    برائے (; j + 3 < گنتی; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#اگر_ختم
	    برائے (; j < گنتی; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت غیرعلامتی حرف *) pp;
	  }
	  توڑو;
	  حالت 2:
	  {
	    ثابت خودکار *pp = (ثابت HBINT16 *) p;
	    غیرعلامتی j = 0;
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
	    برائے (; j + 3 < گنتی; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#اگر_ختم
	    برائے (; j < گنتی; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت غیرعلامتی حرف *) pp;
	  }
	  توڑو;
	  حالت 4:
	  {
	    ثابت خودکار *pp = (ثابت HBINT32 *) p;
	    برائے (غیرعلامتی j = 0; j < گنتی; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت غیرعلامتی حرف *) pp;
	  }
	  توڑو;
	}
	تسلسل_گنتی -= گنتی;
	i += گنتی;
      }
    }

    عوامی:
    سانچہ <قسم_نام T>
    خالی جوڑو_مقصد_2 (hb_array_t<T> برآمد, اعشاری scale = 1.0f)
    {
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      اگر (جھوٹ && scale == 1.0f)
        جوڑو_مقصد<جھوٹ> (برآمد);
      ورنہ
#اگر_ختم
        جوڑو_مقصد<سچ> (برآمد, (T) scale);
    }
  };
};

ساخت TupleList : CFF2_اشاریہ
{
  Tuple_اقدار::iter_t عامل [] (غیرعلامتی i) ثابت
  {
    خودکار بائٹیں = CFF2_اشاریہ::عامل [] (i);
    واپس Tuple_اقدار::iter_t (بائٹیں.arrayZ, بائٹیں.لمبائی);
  }

  Tuple_اقدار::fetcher_t fetcher (غیرعلامتی i) ثابت
  {
    خودکار بائٹیں = CFF2_اشاریہ::عامل [] (i);
    واپس Tuple_اقدار::fetcher_t (بائٹیں.arrayZ, بائٹیں.لمبائی);
  }
};


// Alignment

سانچہ <غیرعلامتی صحیح alignment>
ساخت Align
{
  size_t get_سائز (ثابت خالی *بنیاد) ثابت
  {
    غیرعلامتی انحراف = (ثابت حرف *) یہ - (ثابت حرف *) بنیاد;
    واپس (alignment - انحراف) & (alignment - 1);
  }

  عوامی:
  DEFINE_سائز_MIN (0);
};



} /* namespace OT */


#اگر_ختم /* HB_OPEN_TYPE_HH */
