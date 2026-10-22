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

#اگر_تعریف_نشده HB_OPEN_TYPE_HH
#تعریف_کن HB_OPEN_TYPE_HH

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
قالب <منطقي BE,
	  نام_نوع Type,
	  بیعلامت صحيح اندازه_2 = حجم (Type)>
بنية رقم_Type
{
  تعريفنوع Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  تعريفنوع نام_نوع std::conditional<std::is_integral<Type>::value && حجم (Type) <= حجم(صحيح),
				     نام_نوع std::conditional<std::is_signed<Type>::value, علامتدار, بیعلامت>::type,
				     Type>::type WideType;

  رقم_Type () = افتراضي;
  صریح constexpr رقم_Type (Type V) : v {V} {}
  رقم_Type& عملگر = (Type V) { v = V; أرجع *هذا; }

  عملگر WideType () ثابت { أرجع v; }

  منطقي عملگر == (ثابت رقم_Type &o) ثابت { أرجع (Type) v == (Type) o.v; }
  منطقي عملگر != (ثابت رقم_Type &o) ثابت { أرجع !(*هذا == o); }

  رقم_Type& عملگر += (WideType شمار) { *هذا = *هذا + شمار; أرجع *هذا; }
  رقم_Type& عملگر -= (WideType شمار) { *هذا = *هذا - شمار; أرجع *هذا; }
  رقم_Type& عملگر ++ () { *هذا += 1; أرجع *هذا; }
  رقم_Type& عملگر -- () { *هذا -= 1; أرجع *هذا; }
  رقم_Type عملگر ++ (صحيح) { رقم_Type c (*هذا); ++*هذا; أرجع c; }
  رقم_Type عملگر -- (صحيح) { رقم_Type c (*هذا); --*هذا; أرجع c; }

  uint32_t hash () ثابت { أرجع hb_array ((ثابت حرف *) &v, حجم (v)).hash (); }
  HB_داخلی ساكن صحيح cmp (ثابت رقم_Type *a, ثابت رقم_Type *b)
  { أرجع b->cmp (*a); }
  HB_داخلی ساكن صحيح cmp (ثابت فراغ *a, ثابت فراغ *b)
  {
    رقم_Type *pa = (رقم_Type *) a;
    رقم_Type *pb = (رقم_Type *) b;

    أرجع pb->cmp (*pa);
  }
  قالب <نام_نوع Type2,
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
  نام_نوع std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, اندازه_2>,
			    HBFloat<BE, Type, اندازه_2>>::type v;
  عام:
  DEFINE_اندازه_STATIC (اندازه_2);
};

تعريفنوع رقم_Type<درست, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
تعريفنوع رقم_Type<درست, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
تعريفنوع رقم_Type<درست, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
تعريفنوع رقم_Type<درست, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
تعريفنوع رقم_Type<درست, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
تعريفنوع رقم_Type<درست, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
تعريفنوع رقم_Type<درست, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
تعريفنوع رقم_Type<درست, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
تعريفنوع رقم_Type<درست, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

تعريفنوع رقم_Type<خطأ, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
تعريفنوع رقم_Type<خطأ, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
تعريفنوع رقم_Type<خطأ, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
تعريفنوع رقم_Type<خطأ, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
تعريفنوع رقم_Type<خطأ, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
تعريفنوع رقم_Type<خطأ, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

تعريفنوع رقم_Type<درست,  عائم>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
تعريفنوع رقم_Type<درست,  مزدوج> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
تعريفنوع رقم_Type<خطأ, عائم>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
تعريفنوع رقم_Type<خطأ, مزدوج> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
بنية HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& عملگر = (uint16_t i ) { HBUINT16::عملگر= (i); أرجع *هذا; }
  عام:
  DEFINE_اندازه_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
بنية HBUINT32VAR
{
  size_t get_اندازه () ثابت
  {
    بیعلامت b0 = v[0];
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

  ساكن size_t get_اندازه (uint32_t v)
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
		  c->check_range (v, get_اندازه ()));
  }

  عملگر uint32_t () ثابت
  {
    بیعلامت b0 = v[0];
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

  ساكن بیعلامت serialize_unsafe (بیعلامت حرف *buf, uint32_t v)
  {
    بیعلامت len = get_اندازه (v);
    بیعلامت حرف *p = buf + len;
    لكل (بیعلامت i = 0; i < len; i++)
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
    بیعلامت len = get_اندازه (v);
    بیعلامت حرف *buf = c->allocate_اندازه<بیعلامت حرف> (len, خطأ);
    إذا (unlikely (!buf))
      أرجع خطأ;

    serialize_unsafe (buf, v);
    أرجع درست;
  }

  محمي:
  بیعلامت حرف v[5];

  عام:
  DEFINE_اندازه_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
تعريفنوع HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
تعريفنوع HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
تعريفنوع HBUINT16 UFWORD;

قالب <نام_نوع Type, بیعلامت fraction_بیتها>
بنية HBFixed : Type
{
  ساكن constexpr عائم mult = 1.f / (1 << fraction_بیتها);
  static_assert (Type::static_اندازه * 8 > fraction_بیتها, "");

  عملگر علامتدار () ثابت = حذف;
  عملگر بیعلامت () ثابت = حذف;
  صریح عملگر عائم () ثابت { أرجع مقصد_float (); }
  نام_نوع Type::type مقصد_int () ثابت { أرجع Type::v; }
  فراغ مجموعه_int (نام_نوع Type::type i ) { Type::v = i; }
  عائم مقصد_float (عائم جابه_جایی = 0) ثابت  { أرجع ((int32_t) Type::v + جابه_جایی) * mult; }
  فراغ مجموعه_float (عائم f) { Type::v = roundf (f / mult); }
  عام:
  DEFINE_اندازه_STATIC (Type::static_اندازه);
};

/* 16-bit signed fixed number with the low 14 bits of fraction (2.14). */
استفاده F2DOT14 = HBFixed<HBINT16, 14>;
استفاده F4DOT12 = HBFixed<HBINT16, 12>;
استفاده F6DOT10 = HBFixed<HBINT16, 10>;

/* 32-bit signed fixed-point number (16.16). */
استفاده F16DOT16 = HBFixed<HBINT32, 16>;

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
  DEFINE_اندازه_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
بنية برچسب : HBUINT32
{
  برچسب& عملگر = (hb_برچسب_t i) { HBUINT32::عملگر= (i); أرجع *هذا; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  عملگر ثابت حرف* () ثابت { أرجع reinterpret_cast<ثابت حرف *> (هذا); }
  عملگر حرف* ()             { أرجع reinterpret_cast<حرف *> (هذا); }
  عام:
  DEFINE_اندازه_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
بنية HBGlyphID16 : HBUINT16
{
  HBGlyphID16& عملگر = (uint16_t i) { HBUINT16::عملگر= (i); أرجع *هذا; }
};
بنية HBGlyphID24 : HBUINT24
{
  HBGlyphID24& عملگر = (uint32_t i) { HBUINT24::عملگر= (i); أرجع *هذا; }
};

/* Script/language-system/feature index */
بنية شاخص : HBUINT16 {
  ساكن constexpr بیعلامت NOT_یافت_شاخص = 0xFFFFu;
  شاخص& عملگر = (uint16_t i) { HBUINT16::عملگر= (i); أرجع *هذا; }
};
DECLARE_NULL_NAMESPACE_بایتها (OT, شاخص);

تعريفنوع شاخص NameID;

بنية VarIdx : HBUINT32 {
  ساكن constexpr بیعلامت NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_شاخص, "");
  VarIdx& عملگر = (uint32_t i) { HBUINT32::عملگر= (i); أرجع *هذا; }
};
DECLARE_NULL_NAMESPACE_بایتها (OT, VarIdx);

/* Offset, Null offset = 0 */
قالب <نام_نوع Type, منطقي has_null=درست>
بنية جابه_جایی_2 : Type
{
  جابه_جایی_2& عملگر = (نام_نوع Type::type i) { Type::عملگر= (i); أرجع *هذا; }

  تعريفنوع Type type;

  منطقي is_null () ثابت { أرجع has_null && 0 == *هذا; }

  عام:
  DEFINE_اندازه_STATIC (حجم (Type));
};

تعريفنوع جابه_جایی_2<HBUINT16> Offset16;
تعريفنوع جابه_جایی_2<HBUINT24> Offset24;
تعريفنوع جابه_جایی_2<HBUINT32> Offset32;


/* CheckSum */
بنية Check_جمع : HBUINT32
{
  Check_جمع& عملگر = (uint32_t i) { HBUINT32::عملگر= (i); أرجع *هذا; }

  /* This is reference implementation from the spec. */
  ساكن uint32_t Calc_جدول_Checksum (ثابت HBUINT32 *جدول_3, uint32_t درازی_3)
  {
    uint32_t Sum = 0L;
    أكد (0 == (درازی_3 & 3));
    ثابت HBUINT32 *انتها_Ptr = جدول_3 + درازی_3 / HBUINT32::static_اندازه;

    طالما (جدول_3 < انتها_Ptr)
      Sum += *جدول_3++;
    أرجع Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  فراغ مجموعه_for_داده (ثابت فراغ *داده, بیعلامت صحيح درازی)
  { *هذا = Calc_جدول_Checksum ((ثابت HBUINT32 *) داده, درازی); }

  عام:
  DEFINE_اندازه_STATIC (4);
};


/*
 * Version Numbers
 */

قالب <نام_نوع FixedType=HBUINT16>
بنية Fixed_نسخه
{
  uint32_t مقصد_int () ثابت { أرجع (major << (حجم (FixedType) * 8)) + minor; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  FixedType major;
  FixedType minor;
  عام:
  DEFINE_اندازه_STATIC (2 * حجم (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

قالب <نام_نوع Type, منطقي has_null>
بنية _hb_has_null
{
  ساكن ثابت Type *get_null () { أرجع اشاره_تهی; }
  ساكن Type *get_crap ()       { أرجع اشاره_تهی; }
};
قالب <نام_نوع Type>
بنية _hb_has_null<Type, درست>
{
  ساكن ثابت Type *get_null () { أرجع &Null (Type); }
  ساكن       Type *get_crap () { أرجع &Crap (Type); }
};

قالب <نام_نوع Type, نام_نوع جابه_جایی_Type, نام_نوع پایه_Type=فراغ, منطقي has_null=درست>
بنية جابه_جایی_مقصد : جابه_جایی_2<جابه_جایی_Type, has_null>
{
  استفاده هدف_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == خطأ ||
		 (hb_has_null_اندازه_2 (Type) || !hb_has_min_اندازه_2 (Type)), "");

  HB_DELETE_COPY_تخصیص_بده (جابه_جایی_مقصد);
  جابه_جایی_مقصد () = افتراضي;

  جابه_جایی_مقصد& عملگر = (نام_نوع جابه_جایی_Type::type i) { جابه_جایی_Type::عملگر= (i); أرجع *هذا; }

  ثابت Type& عملگر () (ثابت فراغ *پایه) ثابت
  {
    إذا (unlikely (هذا->is_null ())) أرجع *_hb_has_null<Type, has_null>::get_null ();
    أرجع Struct_At_جابه_جایی<ثابت Type> (پایه, *هذا);
  }
  Type& عملگر () (فراغ *پایه) ثابت
  {
    إذا (unlikely (هذا->is_null ())) أرجع *_hb_has_null<Type, has_null>::get_crap ();
    أرجع Struct_At_جابه_جایی<Type> (پایه, *هذا);
  }

  قالب <نام_نوع پایه_3,
	    hb_enable_if (hb_is_convertible (ثابت پایه_3, ثابت پایه_Type *))>
  دوست ثابت Type& عملگر + (ثابت پایه_3 &پایه, ثابت جابه_جایی_مقصد &جابه_جایی) { أرجع جابه_جایی ((ثابت فراغ *) پایه); }
  قالب <نام_نوع پایه_3,
	    hb_enable_if (hb_is_convertible (ثابت پایه_3, ثابت پایه_Type *))>
  دوست ثابت Type& عملگر + (ثابت جابه_جایی_مقصد &جابه_جایی, ثابت پایه_3 &پایه) { أرجع جابه_جایی ((ثابت فراغ *) پایه); }
  قالب <نام_نوع پایه_3,
	    hb_enable_if (hb_is_convertible (پایه_3, پایه_Type *))>
  دوست Type& عملگر + (پایه_3 &&پایه, جابه_جایی_مقصد &جابه_جایی) { أرجع جابه_جایی ((فراغ *) پایه); }
  قالب <نام_نوع پایه_3,
	    hb_enable_if (hb_is_convertible (پایه_3, پایه_Type *))>
  دوست Type& عملگر + (جابه_جایی_مقصد &جابه_جایی, پایه_3 &&پایه) { أرجع جابه_جایی ((فراغ *) پایه); }


  قالب <نام_نوع پایه_3, نام_نوع ...Ts>
  منطقي serialize_subset (hb_subset_context_t *c, ثابت جابه_جایی_مقصد& src,
			 ثابت پایه_3 *src_پایه, Ts&&... ds)
  {
    *هذا = 0;
    إذا (src.is_null ())
      أرجع خطأ;

    خودکار *s = c->serializer;

    s->push ();

    منطقي ret = c->dispatch (src_پایه+src, std::forward<Ts> (ds)...);

    إذا (ret || !has_null)
      s->جمع_کن_link (*هذا, s->برداشت_pack ());
    وإلا
      s->برداشت_discard ();

    أرجع ret;
  }


  قالب <نام_نوع ...Ts>
  منطقي serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *هذا = 0;

    Type* obj = c->push<Type> ();
    منطقي ret = obj->serialize (c, std::forward<Ts> (ds)...);

    إذا (ret)
      c->جمع_کن_link (*هذا, c->برداشت_pack ());
    وإلا
      c->برداشت_discard ();

    أرجع ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  قالب <نام_نوع ...Ts>
  منطقي serialize_copy (hb_serialize_context_t *c, ثابت جابه_جایی_مقصد& src,
		       ثابت فراغ *src_پایه, بیعلامت dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *هذا = 0;
    إذا (src.is_null ())
      أرجع خطأ;

    c->push ();

    منطقي ret = c->copy (src_پایه+src, std::forward<Ts> (ds)...);

    c->جمع_کن_link (*هذا, c->برداشت_pack (), whence, dst_bias);

    أرجع ret;
  }

  منطقي serialize_copy (hb_serialize_context_t *c, ثابت جابه_جایی_مقصد& src,
		       ثابت فراغ *src_پایه, بیعلامت dst_bias = 0)
  { أرجع serialize_copy (c, src, src_پایه, dst_bias, hb_serialize_context_t::سر); }

  منطقي sanitize_shallow (hb_sanitize_context_t *c, ثابت پایه_Type *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    إذا (unlikely ((ثابت حرف *) پایه + (بیعلامت) *هذا < (ثابت حرف *) پایه)) return_trace (خطأ);
    return_trace (درست);
  }

  قالب <نام_نوع ...Ts>
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  منطقي sanitize (hb_sanitize_context_t *c, ثابت پایه_Type *پایه, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (sanitize_shallow (c, پایه) &&
		  hb_barrier () &&
		  (هذا->is_null () ||
		   c->dispatch (Struct_At_جابه_جایی<Type> (پایه, *هذا), std::forward<Ts> (ds)...)));
  }

  DEFINE_اندازه_STATIC (حجم (جابه_جایی_Type));
};
/* Partial specializations. */
قالب <نام_نوع Type, نام_نوع پایه_Type=فراغ, منطقي has_null=درست> استفاده Offset16_مقصد = جابه_جایی_مقصد<Type, HBUINT16, پایه_Type, has_null>;
قالب <نام_نوع Type, نام_نوع پایه_Type=فراغ, منطقي has_null=درست> استفاده Offset24_مقصد = جابه_جایی_مقصد<Type, HBUINT24, پایه_Type, has_null>;
قالب <نام_نوع Type, نام_نوع پایه_Type=فراغ, منطقي has_null=درست> استفاده Offset32_مقصد = جابه_جایی_مقصد<Type, HBUINT32, پایه_Type, has_null>;

قالب <نام_نوع Type, نام_نوع جابه_جایی_Type, نام_نوع پایه_Type=فراغ> استفاده NNOffset_مقصد = جابه_جایی_مقصد<Type, جابه_جایی_Type, پایه_Type, خطأ>;
قالب <نام_نوع Type, نام_نوع پایه_Type=فراغ> استفاده NNOffset16_مقصد = Offset16_مقصد<Type, پایه_Type, خطأ>;
قالب <نام_نوع Type, نام_نوع پایه_Type=فراغ> استفاده NNOffset24_مقصد = Offset24_مقصد<Type, پایه_Type, خطأ>;
قالب <نام_نوع Type, نام_نوع پایه_Type=فراغ> استفاده NNOffset32_مقصد = Offset32_مقصد<Type, پایه_Type, خطأ>;


/*
 * Array Types
 */

قالب <نام_نوع Type>
بنية Unsized_Array_از
{
  تعريفنوع Type قلم_t;
  ساكن constexpr بیعلامت قلم_اندازه = hb_static_اندازه (Type);

  HB_DELETE_CREATE_COPY_تخصیص_بده (Unsized_Array_از);

  ثابت Type& عملگر [] (بیعلامت صحيح i) ثابت
  {
    أرجع arrayZ[i];
  }
  Type& عملگر [] (بیعلامت صحيح i)
  {
    أرجع arrayZ[i];
  }

  ساكن size_t get_اندازه (بیعلامت صحيح len)
  { أرجع hb_unsigned_mul_saturate (len, Type::static_اندازه); }

  قالب <نام_نوع T> عملگر T * () { أرجع arrayZ; }
  قالب <نام_نوع T> عملگر ثابت T * () ثابت { أرجع arrayZ; }
  hb_array_t<Type> as_array (بیعلامت صحيح len)
  { أرجع hb_array (arrayZ, len); }
  hb_array_t<ثابت Type> as_array (بیعلامت صحيح len) ثابت
  { أرجع hb_array (arrayZ, len); }

  قالب <نام_نوع T>
  Type &lsearch (بیعلامت صحيح len, ثابت T &x, Type &not_یافت = Crap (Type))
  { أرجع *as_array (len).lsearch (x, &not_یافت); }
  قالب <نام_نوع T>
  ثابت Type &lsearch (بیعلامت صحيح len, ثابت T &x, ثابت Type &not_یافت = Null (Type)) ثابت
  { أرجع *as_array (len).lsearch (x, &not_یافت); }
  قالب <نام_نوع T>
  منطقي lfind (بیعلامت صحيح len, ثابت T &x, بیعلامت صحيح *i = اشاره_تهی,
	      hb_not_یافت_t not_یافت = HB_NOT_یافت_DONT_STORE,
	      بیعلامت صحيح مقصد_store = (بیعلامت صحيح) -1) ثابت
  { أرجع as_array (len).lfind (x, i, not_یافت, مقصد_store); }

  فراغ qsort (بیعلامت صحيح len, بیعلامت صحيح شروع = 0, بیعلامت صحيح انتها = (بیعلامت صحيح) -1)
  { as_array (len).qsort (شروع, انتها); }

  منطقي serialize (hb_serialize_context_t *c, بیعلامت صحيح اقلام_len, منطقي clear = درست)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_اندازه (هذا, get_اندازه (اقلام_len), clear))) return_trace (خطأ);
    return_trace (درست);
  }
  قالب <نام_نوع Iterator,
	    hb_requires (hb_is_منبع_از (Iterator, Type))>
  منطقي serialize (hb_serialize_context_t *c, Iterator اقلام)
  {
    TRACE_SERIALIZE (هذا);
    بیعلامت شمار = hb_len (اقلام);
    إذا (unlikely (!serialize (c, شمار, خطأ))) return_trace (خطأ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    لكل (بیعلامت i = 0; i < شمار; i++, ++اقلام)
      arrayZ[i] = *اقلام;
    return_trace (درست);
  }

  Unsized_Array_از* copy (hb_serialize_context_t *c, بیعلامت شمار) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *بیرون = c->شروع_embed (هذا);
    إذا (unlikely (!as_array (شمار).copy (c))) return_trace (اشاره_تهی);
    return_trace (بیرون);
  }

  قالب <نام_نوع ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, بیعلامت صحيح شمار, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c, شمار))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (درست);
    hb_barrier ();
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (خطأ);
    return_trace (درست);
  }

  منطقي sanitize_shallow (hb_sanitize_context_t *c, بیعلامت صحيح شمار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_array (arrayZ, شمار));
  }

  عام:
  Type		arrayZ[HB_VAR_ARRAY];
  عام:
  DEFINE_اندازه_UNBOUNDED (0);
};

/* Unsized array of offset's */
قالب <نام_نوع Type, نام_نوع جابه_جایی_Type, نام_نوع پایه_Type=فراغ, منطقي has_null=درست>
استفاده Unsized_Array16_از_جابه_جایی_مقصد = Unsized_Array_از<جابه_جایی_مقصد<Type, جابه_جایی_Type, پایه_Type, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
قالب <نام_نوع Type, نام_نوع جابه_جایی_Type, نام_نوع پایه_Type=فراغ, منطقي has_null=درست>
بنية Unsized_List_از_Offset16_مقصد : Unsized_Array16_از_جابه_جایی_مقصد<Type, جابه_جایی_Type, پایه_Type, has_null>
{
  ثابت Type& عملگر [] (صحيح i_) ثابت
  {
    بیعلامت صحيح i = (بیعلامت صحيح) i_;
    ثابت جابه_جایی_مقصد<Type, جابه_جایی_Type, پایه_Type, has_null> *p = &هذا->arrayZ[i];
    إذا (unlikely ((ثابت فراغ *) p < (ثابت فراغ *) هذا->arrayZ)) أرجع Null (Type); /* Overflowed. */
    hb_barrier ();
    أرجع هذا+*p;
  }
  Type& عملگر [] (صحيح i_)
  {
    بیعلامت صحيح i = (بیعلامت صحيح) i_;
    ثابت جابه_جایی_مقصد<Type, جابه_جایی_Type, پایه_Type, has_null> *p = &هذا->arrayZ[i];
    إذا (unlikely ((ثابت فراغ *) p < (ثابت فراغ *) هذا->arrayZ)) أرجع Crap (Type); /* Overflowed. */
    hb_barrier ();
    أرجع هذا+*p;
  }

  قالب <نام_نوع ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, بیعلامت صحيح شمار, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace ((Unsized_Array16_از_جابه_جایی_مقصد<Type, جابه_جایی_Type, پایه_Type, has_null>
		   ::sanitize (c, شمار, هذا, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
قالب <نام_نوع Type>
بنية Sorted_Unsized_Array_از : Unsized_Array_از<Type>
{
  hb_sorted_array_t<Type> as_array (بیعلامت صحيح len)
  { أرجع hb_sorted_array (هذا->arrayZ, len); }
  hb_sorted_array_t<ثابت Type> as_array (بیعلامت صحيح len) ثابت
  { أرجع hb_sorted_array (هذا->arrayZ, len); }
  عملگر hb_sorted_array_t<Type> ()             { أرجع as_array (); }
  عملگر hb_sorted_array_t<ثابت Type> () ثابت { أرجع as_array (); }

  قالب <نام_نوع T>
  Type &bsearch (بیعلامت صحيح len, ثابت T &x, Type &not_یافت = Crap (Type))
  { أرجع *as_array (len).bsearch (x, &not_یافت); }
  قالب <نام_نوع T>
  ثابت Type &bsearch (بیعلامت صحيح len, ثابت T &x, ثابت Type &not_یافت = Null (Type)) ثابت
  { أرجع *as_array (len).bsearch (x, &not_یافت); }
  قالب <نام_نوع T>
  منطقي bfind (بیعلامت صحيح len, ثابت T &x, بیعلامت صحيح *i = اشاره_تهی,
	      hb_not_یافت_t not_یافت = HB_NOT_یافت_DONT_STORE,
	      بیعلامت صحيح مقصد_store = (بیعلامت صحيح) -1) ثابت
  { أرجع as_array (len).bfind (x, i, not_یافت, مقصد_store); }
};


/* An array with a number of elements. */
قالب <نام_نوع Type, نام_نوع LenType>
بنية Array_از
{
  تعريفنوع Type قلم_t;
  ساكن constexpr بیعلامت قلم_اندازه = hb_static_اندازه (Type);

  HB_DELETE_CREATE_COPY_تخصیص_بده (Array_از);

  ثابت Type& عملگر [] (صحيح i_) ثابت
  {
    بیعلامت صحيح i = (بیعلامت صحيح) i_;
    إذا (unlikely (i >= len)) أرجع Null (Type);
    hb_barrier ();
    أرجع arrayZ[i];
  }
  Type& عملگر [] (صحيح i_)
  {
    بیعلامت صحيح i = (بیعلامت صحيح) i_;
    إذا (unlikely (i >= len)) أرجع Crap (Type);
    hb_barrier ();
    أرجع arrayZ[i];
  }

  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_mul_جمع_کن_saturate (len, Type::static_اندازه, len.static_اندازه); }

  صریح عملگر منطقي () ثابت { أرجع len; }

  فراغ برداشت () { len--; }

  hb_array_t<      Type> as_array ()       { أرجع hb_array (arrayZ, len); }
  hb_array_t<ثابت Type> as_array () ثابت { أرجع hb_array (arrayZ, len); }

  /* Iterator. */
  تعريفنوع hb_array_t<ثابت Type>   iter_t;
  تعريفنوع hb_array_t<      Type> writer_t;
    iter_t   iter () ثابت { أرجع as_array (); }
  writer_t writer ()       { أرجع as_array (); }
  عملگر   iter_t () ثابت { أرجع   iter (); }
  عملگر writer_t ()       { أرجع writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { أرجع arrayZ; }
  ثابت Type *انتها () ثابت { أرجع arrayZ + len; }

  قالب <نام_نوع T>
  Type &lsearch (ثابت T &x, Type &not_یافت = Crap (Type))
  { أرجع *as_array ().lsearch (x, &not_یافت); }
  قالب <نام_نوع T>
  ثابت Type &lsearch (ثابت T &x, ثابت Type &not_یافت = Null (Type)) ثابت
  { أرجع *as_array ().lsearch (x, &not_یافت); }
  قالب <نام_نوع T>
  منطقي lfind (ثابت T &x, بیعلامت صحيح *i = اشاره_تهی,
	      hb_not_یافت_t not_یافت = HB_NOT_یافت_DONT_STORE,
	      بیعلامت صحيح مقصد_store = (بیعلامت صحيح) -1) ثابت
  { أرجع as_array ().lfind (x, i, not_یافت, مقصد_store); }

  فراغ qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, بیعلامت اقلام_len, منطقي clear = درست)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    c->check_تخصیص_بده (len, اقلام_len, HB_SERIALIZE_ERROR_ARRAY_سرریز);
    إذا (unlikely (!c->extend_اندازه (هذا, get_اندازه (), clear))) return_trace (خطأ);
    return_trace (درست);
  }
  قالب <نام_نوع Iterator,
	    hb_requires (hb_is_منبع_از (Iterator, Type))>
  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, Iterator اقلام)
  {
    TRACE_SERIALIZE (هذا);
    بیعلامت شمار = hb_len (اقلام);
    إذا (unlikely (!serialize (c, شمار, خطأ))) return_trace (خطأ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    لكل (بیعلامت i = 0; i < شمار; i++, ++اقلام)
      arrayZ[i] = *اقلام;
    return_trace (درست);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (هذا);
    len++;
    إذا (unlikely (!len || !c->extend (هذا)))
    {
      len--;
      return_trace (اشاره_تهی);
    }
    return_trace (&arrayZ[len - 1]);
  }

  Array_از* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *بیرون = c->شروع_embed (هذا);
    إذا (unlikely (!c->extend_min (بیرون))) return_trace (اشاره_تهی);
    c->check_تخصیص_بده (بیرون->len, len, HB_SERIALIZE_ERROR_ARRAY_سرریز);
    إذا (unlikely (!as_array ().copy (c))) return_trace (اشاره_تهی);
    return_trace (بیرون);
  }

  قالب <نام_نوع ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (درست);
    hb_barrier ();
    بیعلامت صحيح شمار = len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (خطأ);
    return_trace (درست);
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
  DEFINE_اندازه_ARRAY (حجم (LenType), arrayZ);
};
قالب <نام_نوع Type> استفاده Array16_از = Array_از<Type, HBUINT16>;
قالب <نام_نوع Type> استفاده Array24_از = Array_از<Type, HBUINT24>;
قالب <نام_نوع Type> استفاده Array32_از = Array_از<Type, HBUINT32>;
استفاده PString = Array_از<HBUINT8, HBUINT8>;

/* Array of Offset's */
قالب <نام_نوع Type> استفاده Array8_از_Offset24_مقصد = Array_از<جابه_جایی_مقصد<Type, HBUINT24>, HBUINT8>;
قالب <نام_نوع Type> استفاده Array16_از_Offset16_مقصد = Array_از<جابه_جایی_مقصد<Type, HBUINT16>, HBUINT16>;
قالب <نام_نوع Type> استفاده Array16_از_Offset32_مقصد = Array_از<جابه_جایی_مقصد<Type, HBUINT32>, HBUINT16>;
قالب <نام_نوع Type> استفاده Array32_از_Offset32_مقصد = Array_از<جابه_جایی_مقصد<Type, HBUINT32>, HBUINT32>;

/* Array of offsets relative to the beginning of the array itself. */
قالب <نام_نوع Type, نام_نوع جابه_جایی_Type>
بنية List16_از_جابه_جایی_مقصد : Array_از<جابه_جایی_مقصد<Type, جابه_جایی_Type>, HBUINT16>
{
  ثابت Type& عملگر [] (صحيح i_) ثابت
  {
    بیعلامت صحيح i = (بیعلامت صحيح) i_;
    إذا (unlikely (i >= هذا->len)) أرجع Null (Type);
    hb_barrier ();
    أرجع هذا+هذا->arrayZ[i];
  }
  ثابت Type& عملگر [] (صحيح i_)
  {
    بیعلامت صحيح i = (بیعلامت صحيح) i_;
    إذا (unlikely (i >= هذا->len)) أرجع Crap (Type);
    hb_barrier ();
    أرجع هذا+هذا->arrayZ[i];
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    بنية List16_از_جابه_جایی_مقصد *بیرون = c->serializer->embed (*هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);
    بیعلامت صحيح شمار = هذا->len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      بیرون->arrayZ[i].serialize_subset (c, هذا->arrayZ[i], هذا, بیرون);
    return_trace (درست);
  }

  قالب <نام_نوع ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace ((Array16_از<جابه_جایی_مقصد<Type, جابه_جایی_Type>>::sanitize (c, هذا, std::forward<Ts> (ds)...)));
  }
};

قالب <نام_نوع Type>
استفاده List16_از_Offset16_مقصد = List16_از_جابه_جایی_مقصد<Type, HBUINT16>;

/* An array starting at second element. */
قالب <نام_نوع Type, نام_نوع LenType>
بنية Headless_Array_از
{
  ساكن constexpr بیعلامت قلم_اندازه = Type::static_اندازه;

  HB_DELETE_CREATE_COPY_تخصیص_بده (Headless_Array_از);

  ثابت Type& عملگر [] (صحيح i_) ثابت
  {
    بیعلامت صحيح i = (بیعلامت صحيح) i_;
    إذا (unlikely (i >= lenP1 || !i)) أرجع Null (Type);
    hb_barrier ();
    أرجع arrayZ[i-1];
  }
  Type& عملگر [] (صحيح i_)
  {
    بیعلامت صحيح i = (بیعلامت صحيح) i_;
    إذا (unlikely (i >= lenP1 || !i)) أرجع Crap (Type);
    hb_barrier ();
    أرجع arrayZ[i-1];
  }
  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_mul_جمع_کن_saturate (get_درازی (), Type::static_اندازه, lenP1.static_اندازه); }

  بیعلامت get_درازی () ثابت { أرجع lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { أرجع hb_array (arrayZ, get_درازی ()); }
  hb_array_t<ثابت Type> as_array () ثابت { أرجع hb_array (arrayZ, get_درازی ()); }

  /* Iterator. */
  تعريفنوع hb_array_t<ثابت Type>   iter_t;
  تعريفنوع hb_array_t<      Type> writer_t;
    iter_t   iter () ثابت { أرجع as_array (); }
  writer_t writer ()       { أرجع as_array (); }
  عملگر   iter_t () ثابت { أرجع   iter (); }
  عملگر writer_t ()       { أرجع writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { أرجع arrayZ; }
  ثابت Type *انتها () ثابت { أرجع arrayZ + get_درازی (); }

  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, بیعلامت صحيح اقلام_len, منطقي clear = درست)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    c->check_تخصیص_بده (lenP1, اقلام_len + 1, HB_SERIALIZE_ERROR_ARRAY_سرریز);
    إذا (unlikely (!c->extend_اندازه (هذا, get_اندازه (), clear))) return_trace (خطأ);
    return_trace (درست);
  }
  قالب <نام_نوع Iterator,
	    hb_requires (hb_is_منبع_از (Iterator, Type))>
  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, Iterator اقلام)
  {
    TRACE_SERIALIZE (هذا);
    بیعلامت شمار = hb_len (اقلام);
    إذا (unlikely (!serialize (c, شمار, خطأ))) return_trace (خطأ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    لكل (بیعلامت i = 0; i < شمار; i++, ++اقلام)
      arrayZ[i] = *اقلام;
    return_trace (درست);
  }

  قالب <نام_نوع ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (درست);
    hb_barrier ();
    بیعلامت صحيح شمار = get_درازی ();
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (خطأ);
    return_trace (درست);
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
  DEFINE_اندازه_ARRAY (حجم (LenType), arrayZ);
};
قالب <نام_نوع Type> استفاده Headless_Array16_از = Headless_Array_از<Type, HBUINT16>;

/* An array storing length-1. */
قالب <نام_نوع Type, نام_نوع LenType=HBUINT16>
بنية Array_از_M1
{
  HB_DELETE_CREATE_COPY_تخصیص_بده (Array_از_M1);

  ثابت Type& عملگر [] (صحيح i_) ثابت
  {
    بیعلامت صحيح i = (بیعلامت صحيح) i_;
    إذا (unlikely (i > lenM1)) أرجع Null (Type);
    hb_barrier ();
    أرجع arrayZ[i];
  }
  Type& عملگر [] (صحيح i_)
  {
    بیعلامت صحيح i = (بیعلامت صحيح) i_;
    إذا (unlikely (i > lenM1)) أرجع Crap (Type);
    hb_barrier ();
    أرجع arrayZ[i];
  }
  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_mul_جمع_کن_saturate (lenM1 + 1, Type::static_اندازه, lenM1.static_اندازه); }

  قالب <نام_نوع ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (درست);
    hb_barrier ();
    بیعلامت صحيح شمار = lenM1 + 1;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (خطأ);
    return_trace (درست);
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
  DEFINE_اندازه_ARRAY (حجم (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
قالب <نام_نوع Type, نام_نوع LenType>
بنية Sorted_Array_از : Array_از<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { أرجع hb_sorted_array (هذا->arrayZ, هذا->len); }
  hb_sorted_array_t<ثابت Type> as_array () ثابت { أرجع hb_sorted_array (هذا->arrayZ, هذا->len); }

  /* Iterator. */
  تعريفنوع hb_sorted_array_t<ثابت Type>   iter_t;
  تعريفنوع hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () ثابت { أرجع as_array (); }
  writer_t writer ()       { أرجع as_array (); }
  عملگر   iter_t () ثابت { أرجع   iter (); }
  عملگر writer_t ()       { أرجع writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { أرجع هذا->arrayZ; }
  ثابت Type *انتها () ثابت { أرجع هذا->arrayZ + هذا->len; }

  منطقي serialize (hb_serialize_context_t *c, بیعلامت صحيح اقلام_len)
  {
    TRACE_SERIALIZE (هذا);
    منطقي ret = Array_از<Type, LenType>::serialize (c, اقلام_len);
    return_trace (ret);
  }
  قالب <نام_نوع Iterator,
	    hb_requires (hb_is_sorted_منبع_از (Iterator, Type))>
  منطقي serialize (hb_serialize_context_t *c, Iterator اقلام)
  {
    TRACE_SERIALIZE (هذا);
    منطقي ret = Array_از<Type, LenType>::serialize (c, اقلام);
    return_trace (ret);
  }

  Sorted_Array_از* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    Sorted_Array_از* بیرون = reinterpret_cast<Sorted_Array_از *> (Array_از<Type, LenType>::copy (c));
    return_trace (بیرون);
  }

  قالب <نام_نوع T>
  Type &bsearch (ثابت T &x, Type &not_یافت = Crap (Type))
  { أرجع *as_array ().bsearch (x, &not_یافت); }
  قالب <نام_نوع T>
  ثابت Type &bsearch (ثابت T &x, ثابت Type &not_یافت = Null (Type)) ثابت
  { أرجع *as_array ().bsearch (x, &not_یافت); }
  قالب <نام_نوع T>
  منطقي bfind (ثابت T &x, بیعلامت صحيح *i = اشاره_تهی,
	      hb_not_یافت_t not_یافت = HB_NOT_یافت_DONT_STORE,
	      بیعلامت صحيح مقصد_store = (بیعلامت صحيح) -1) ثابت
  { أرجع as_array ().bfind (x, i, not_یافت, مقصد_store); }
};

قالب <نام_نوع Type> استفاده Sorted_Array16_از = Sorted_Array_از<Type, HBUINT16>;
قالب <نام_نوع Type> استفاده Sorted_Array24_از = Sorted_Array_از<Type, HBUINT24>;
قالب <نام_نوع Type> استفاده Sorted_Array32_از = Sorted_Array_از<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

قالب <نام_نوع LenType=HBUINT16>
بنية Bin_جستجو_سرآیند
{
  عملگر uint32_t () ثابت { أرجع len; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  Bin_جستجو_سرآیند& عملگر = (بیعلامت صحيح v)
  {
    len = v;
    أكد (len == v);
    مدخل_Selector = hb_max (1u, hb_بیت_storage (v)) - 1;
    جستجو_Range = 16 * (1u << مدخل_Selector);
    range_لغزش = v * 16 > جستجو_Range
		 ? 16 * v - جستجو_Range
		 : 0;
    أرجع *هذا;
  }

  محمي:
  LenType	len;
  LenType	جستجو_Range;
  LenType	مدخل_Selector;
  LenType	range_لغزش;

  عام:
  DEFINE_اندازه_STATIC (8);
};

قالب <نام_نوع Type, نام_نوع LenType=HBUINT16>
استفاده Bin_جستجو_Array_از = Sorted_Array_از<Type, Bin_جستجو_سرآیند<LenType>>;


بنية Var_Sized_Bin_جستجو_سرآیند
{

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16	unit_اندازه;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	جستجو_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	مدخل_Selector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	range_لغزش;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  عام:
  DEFINE_اندازه_STATIC (10);
};

قالب <نام_نوع Type>
بنية Var_Sized_Bin_جستجو_Array_از
{
  ساكن constexpr بیعلامت قلم_اندازه = Type::static_اندازه;

  HB_DELETE_CREATE_COPY_تخصیص_بده (Var_Sized_Bin_جستجو_Array_از);

  منطقي آخرین_is_terminator () ثابت
  {
    إذا (unlikely (!سرآیند.nUnits)) أرجع خطأ;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    ثابت HBUINT16 *واژگان = &Struct_At_جابه_جایی<HBUINT16> (&بایتها_Z, (سرآیند.nUnits - 1) * سرآیند.unit_اندازه);
    بیعلامت صحيح شمار = Type::Termination_کلمه_شمار;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (واژگان[i] != 0xFFFFu)
	أرجع خطأ;
    أرجع درست;
  }

  ثابت Type& عملگر [] (صحيح i_) ثابت
  {
    بیعلامت صحيح i = (بیعلامت صحيح) i_;
    إذا (unlikely (i >= get_درازی ())) أرجع Null (Type);
    hb_barrier ();
    أرجع Struct_At_جابه_جایی<Type> (&بایتها_Z, i * سرآیند.unit_اندازه);
  }
  Type& عملگر [] (صحيح i_)
  {
    بیعلامت صحيح i = (بیعلامت صحيح) i_;
    إذا (unlikely (i >= get_درازی ())) أرجع Crap (Type);
    hb_barrier ();
    أرجع Struct_At_جابه_جایی<Type> (&بایتها_Z, i * سرآیند.unit_اندازه);
  }
  بیعلامت صحيح get_درازی () ثابت
  { أرجع سرآیند.nUnits - آخرین_is_terminator (); }
  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_mul_جمع_کن_saturate (سرآیند.nUnits, سرآیند.unit_اندازه, سرآیند.static_اندازه); }

  قالب <نام_نوع ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (درست);
    hb_barrier ();
    بیعلامت صحيح شمار = get_درازی ();
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (unlikely (!(*هذا)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (خطأ);
    return_trace (درست);
  }

  قالب <نام_نوع T>
  ثابت Type *bsearch (ثابت T &key) ثابت
  {
    بیعلامت pos;
    أرجع hb_bsearch_impl (&pos,
			    key,
			    (ثابت فراغ *) بایتها_Z,
			    get_درازی (),
			    سرآیند.unit_اندازه,
			    hb_cmp_روش<T, Type>)
	   ? (ثابت Type *) (((ثابت حرف *) &بایتها_Z) + (pos * سرآیند.unit_اندازه))
	   : اشاره_تهی;
  }

  خاص:
  منطقي sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (سرآیند.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_اندازه <= سرآیند.unit_اندازه &&
		  c->check_range (بایتها_Z.arrayZ,
				  سرآیند.nUnits,
				  سرآیند.unit_اندازه));
  }

  محمي:
  Var_Sized_Bin_جستجو_سرآیند	سرآیند;
  Unsized_Array_از<HBUINT8>	بایتها_Z;
  عام:
  DEFINE_اندازه_ARRAY (10, بایتها_Z);
};


/* CFF INDEX */

قالب <نام_نوع شمار_3>
بنية CFFIndex
{
  بیعلامت صحيح جابه_جایی_array_اندازه () ثابت
  { أرجع off_اندازه * (شمار + 1); }

  قالب <نام_نوع Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Iterable &iterable,
		  ثابت بیعلامت *p_داده_اندازه = اشاره_تهی,
                  بیعلامت min_off_اندازه = 0)
  {
    TRACE_SERIALIZE (هذا);
    بیعلامت داده_اندازه;
    إذا (p_داده_اندازه)
      داده_اندازه = *p_داده_اندازه;
    وإلا
      مجموع_اندازه (iterable, &داده_اندازه);

    خودکار it = hb_iter (iterable);
    إذا (unlikely (!serialize_سرآیند (c, +it, داده_اندازه, min_off_اندازه))) return_trace (خطأ);
    بیعلامت حرف *ret = c->allocate_اندازه<بیعلامت حرف> (داده_اندازه, خطأ);
    إذا (unlikely (!ret)) return_trace (خطأ);
    بیعلامت مانده = داده_اندازه;
    لكل (ثابت خودکار &_ : +it)
    {
      بیعلامت len = _.درازی;

      إذا (!len)
	استمر;

      إذا (unlikely (len > مانده)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (خطأ, HB_SERIALIZE_ERROR_INT_سرریز));
      }

      مانده -= len;

      إذا (len <= 1)
      {
	*ret++ = *_.arrayZ;
	استمر;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (درست);
  }

  قالب <نام_نوع Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_سرآیند (hb_serialize_context_t *c,
			 Iterator it,
			 بیعلامت داده_اندازه,
                         بیعلامت min_off_اندازه = 0)
  {
    TRACE_SERIALIZE (هذا);

    بیعلامت off_اندازه_2 = (hb_بیت_storage (داده_اندازه + 1) + 7) / 8;
    off_اندازه_2 = hb_max(min_off_اندازه, off_اندازه_2);

    /* serialize CFFIndex header */
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    هذا->شمار = hb_len (it);
    إذا (!هذا->شمار) return_trace (درست);
    إذا (unlikely (!c->extend (هذا->off_اندازه))) return_trace (خطأ);
    هذا->off_اندازه = off_اندازه_2;
    إذا (unlikely (!c->allocate_اندازه<HBUINT8> (off_اندازه_2 * (هذا->شمار + 1), خطأ)))
      return_trace (خطأ);

    /* serialize indices */
    بیعلامت صحيح جابه_جایی = 1;
    إذا (HB_OPTIMIZE_اندازه_VAL)
    {
      بیعلامت صحيح i = 0;
      لكل (ثابت خودکار &_ : +it)
      {
	مجموعه_جابه_جایی_at (i++, جابه_جایی);
	جابه_جایی += hb_len_از (_);
      }
      مجموعه_جابه_جایی_at (i, جابه_جایی);
    }
    وإلا
      بدل (off_اندازه_2)
      {
	حالة 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  لكل (ثابت خودکار &_ : +it)
	  {
	    *p++ = جابه_جایی;
	    جابه_جایی += hb_len_از (_);
	  }
	  *p = جابه_جایی;
	}
	اكسر;
	حالة 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  لكل (ثابت خودکار &_ : +it)
	  {
	    *p++ = جابه_جایی;
	    جابه_جایی += hb_len_از (_);
	  }
	  *p = جابه_جایی;
	}
	اكسر;
	حالة 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  لكل (ثابت خودکار &_ : +it)
	  {
	    *p++ = جابه_جایی;
	    جابه_جایی += hb_len_از (_);
	  }
	  *p = جابه_جایی;
	}
	اكسر;
	حالة 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  لكل (ثابت خودکار &_ : +it)
	  {
	    *p++ = جابه_جایی;
	    جابه_جایی += hb_len_از (_);
	  }
	  *p = جابه_جایی;
	}
	اكسر;
	افتراضي:
	اكسر;
      }

    أكد (جابه_جایی == داده_اندازه + 1);
    return_trace (درست);
  }

  قالب <نام_نوع Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  ساكن بیعلامت مجموع_اندازه (ثابت Iterable &iterable, بیعلامت *داده_اندازه = اشاره_تهی, بیعلامت min_off_اندازه = 0)
  {
    خودکار it = + hb_iter (iterable);
    إذا (!it)
    {
      إذا (داده_اندازه) *داده_اندازه = 0;
      أرجع min_اندازه;
    }

    بیعلامت مجموع = 0;
    لكل (ثابت خودکار &_ : +it)
      مجموع += hb_len_از (_);

    إذا (داده_اندازه) *داده_اندازه = مجموع;

    بیعلامت off_اندازه_2 = (hb_بیت_storage (مجموع + 1) + 7) / 8;
    off_اندازه_2 = hb_max(min_off_اندازه, off_اندازه_2);

    أرجع min_اندازه + HBUINT8::static_اندازه + (hb_len (it) + 1) * off_اندازه_2 + مجموع;
  }

  فراغ مجموعه_جابه_جایی_at (بیعلامت صحيح شاخص_2, بیعلامت صحيح جابه_جایی)
  {
    أكد (شاخص_2 <= شمار);

    بیعلامت صحيح اندازه = off_اندازه;
    ثابت HBUINT8 *p = offsets;
    بدل (اندازه)
    {
      حالة 1: ((HBUINT8  *) p)[شاخص_2] = جابه_جایی; اكسر;
      حالة 2: ((HBUINT16 *) p)[شاخص_2] = جابه_جایی; اكسر;
      حالة 3: ((HBUINT24 *) p)[شاخص_2] = جابه_جایی; اكسر;
      حالة 4: ((HBUINT32 *) p)[شاخص_2] = جابه_جایی; اكسر;
      افتراضي: أرجع;
    }
  }

  خاص:
  بیعلامت صحيح جابه_جایی_at (بیعلامت صحيح شاخص_2) ثابت
  {
    أكد (شاخص_2 <= شمار);

    بیعلامت صحيح اندازه = off_اندازه;
    ثابت HBUINT8 *p = offsets;
    بدل (اندازه)
    {
      حالة 1: أرجع ((HBUINT8  *) p)[شاخص_2];
      حالة 2: أرجع ((HBUINT16 *) p)[شاخص_2];
      حالة 3: أرجع ((HBUINT24 *) p)[شاخص_2];
      حالة 4: أرجع ((HBUINT32 *) p)[شاخص_2];
      افتراضي: أرجع 0;
    }
  }

  ثابت بیعلامت حرف *داده_پایه () ثابت
  { أرجع (ثابت بیعلامت حرف *) هذا + min_اندازه + off_اندازه.static_اندازه - 1 + جابه_جایی_array_اندازه (); }
  عام:

  hb_ubytes_t عملگر [] (بیعلامت صحيح شاخص_2) ثابت
  {
    إذا (unlikely (شاخص_2 >= شمار)) أرجع hb_ubytes_t ();
    hb_barrier ();
    بیعلامت offset0 = جابه_جایی_at (شاخص_2);
    بیعلامت offset1 = جابه_جایی_at (شاخص_2 + 1);
    إذا (unlikely (offset1 < offset0 || offset1 > جابه_جایی_at (شمار)))
      أرجع hb_ubytes_t ();
    أرجع hb_ubytes_t (داده_پایه () + offset0, offset1 - offset0);
  }

  size_t get_اندازه () ثابت
  {
    إذا (شمار)
      أرجع hb_unsigned_جمع_کن_saturate (min_اندازه, off_اندازه.static_اندازه,
				       جابه_جایی_array_اندازه (), جابه_جایی_at (شمار) - 1);
    أرجع min_اندازه;  /* empty CFFIndex contains count only */
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  (شمار == 0 || /* empty INDEX */
			   (شمار < شمار + 1u &&
			    c->check_struct (&off_اندازه) && off_اندازه >= 1 && off_اندازه <= 4 &&
			    c->check_array (offsets, off_اندازه, شمار + 1u) &&
			    c->check_range (داده_پایه (), جابه_جایی_at (شمار))))));
  }

  عام:
  شمار_3		شمار;		/* Number of object data. Note there are (count+1) offsets */
  خاص:
  HBUINT8	off_اندازه;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  عام:
  DEFINE_اندازه_MIN (شمار_3::static_اندازه);
};
تعريفنوع CFFIndex<HBUINT16> CFF1_شاخص;
تعريفنوع CFFIndex<HBUINT32> CFF2_شاخص;


/* TupleValues */
بنية Tuple_مقادیر
{
  تعداد packed_مقدار_flag_t
  {
    مقادیر_ARE_ZEROS     = 0x80,
    مقادیر_ARE_بایتها     = 0x00,
    مقادیر_ARE_واژگان     = 0x40,
    مقادیر_ARE_LONGS     = 0xC0,
    مقادیر_اندازه_ماسک     = 0xC0,
    مقدار_پیاپی_شمار_ماسک = 0x3F
  };

  ساكن بیعلامت compile_unsafe (hb_array_t<ثابت صحيح> values, /*درون*/
				  بیعلامت حرف *encoded_بایتها /*بیرون*/)
  {
    بیعلامت رقم_مقادیر_3 = values.درازی;
    بیعلامت encoded_len = 0;
    بیعلامت i = 0;
    طالما (i < رقم_مقادیر_3)
    {
      صحيح val = values.arrayZ[i];
      إذا (val == 0)
        encoded_len += encode_مقدار_پیاپی_as_zeroes (i, encoded_بایتها + encoded_len, values);
      وإلا إذا ((int8_t) val == val)
        encoded_len += encode_مقدار_پیاپی_as_بایتها (i, encoded_بایتها + encoded_len, values);
      وإلا إذا ((int16_t) val == val)
        encoded_len += encode_مقدار_پیاپی_as_واژگان (i, encoded_بایتها + encoded_len, values);
      وإلا
        encoded_len += encode_مقدار_پیاپی_as_longs (i, encoded_بایتها + encoded_len, values);
    }
    أرجع encoded_len;
  }

  ساكن بیعلامت encode_مقدار_پیاپی_as_zeroes (بیعلامت& i,
					      بیعلامت حرف *it,
					      hb_array_t<ثابت صحيح> values)
  {
    بیعلامت رقم_مقادیر_3 = values.درازی;
    بیعلامت پیاپی_درازی = 0;
    بیعلامت encoded_len = 0;
    طالما (i < رقم_مقادیر_3 && values.arrayZ[i] == 0)
    {
      i++;
      پیاپی_درازی++;
    }

    طالما (پیاپی_درازی >= 64)
    {
      *it++ = حرف (مقادیر_ARE_ZEROS | 63);
      پیاپی_درازی -= 64;
      encoded_len++;
    }

    إذا (پیاپی_درازی)
    {
      *it++ = حرف (مقادیر_ARE_ZEROS | (پیاپی_درازی - 1));
      encoded_len++;
    }
    أرجع encoded_len;
  }

  ساكن بیعلامت encode_مقدار_پیاپی_as_بایتها (بیعلامت &i,
					     بیعلامت حرف *it,
					     hb_array_t<ثابت صحيح> values)
  {
    بیعلامت شروع = i;
    بیعلامت رقم_مقادیر_3 = values.درازی;
    طالما (i < رقم_مقادیر_3)
    {
      صحيح val = values.arrayZ[i];
      إذا ((int8_t) val != val)
        اكسر;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      إذا (val == 0 && i + 1 < رقم_مقادیر_3 && values.arrayZ[i+1] == 0)
        اكسر;

      i++;
    }
    بیعلامت پیاپی_درازی = i - شروع;

    بیعلامت encoded_len = 0;

    طالما (پیاپی_درازی >= 64)
    {
      *it++ = (مقادیر_ARE_بایتها | 63);
      encoded_len++;

      لكل (بیعلامت j = 0; j < 64; j++)
	it[j] = static_cast<حرف> (values.arrayZ[شروع + j]);
      it += 64;
      encoded_len += 64;

      شروع += 64;
      پیاپی_درازی -= 64;
    }

    إذا (پیاپی_درازی)
    {
      *it++ = (مقادیر_ARE_بایتها | (پیاپی_درازی - 1));
      encoded_len++;

      لكل (بیعلامت j = 0; j < پیاپی_درازی; j++)
        it[j] = static_cast<حرف> (values.arrayZ[شروع + j]);
      encoded_len += پیاپی_درازی;
    }

    أرجع encoded_len;
  }

  ساكن بیعلامت encode_مقدار_پیاپی_as_واژگان (بیعلامت &i,
					     بیعلامت حرف *it,
					     hb_array_t<ثابت صحيح> values)
  {
    بیعلامت شروع = i;
    بیعلامت رقم_مقادیر_3 = values.درازی;
    طالما (i < رقم_مقادیر_3)
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
          i + 1 < رقم_مقادیر_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        اكسر;

      i++;
    }

    بیعلامت پیاپی_درازی = i - شروع;
    بیعلامت encoded_len = 0;
    طالما (پیاپی_درازی >= 64)
    {
      *it++ = (مقادیر_ARE_واژگان | 63);
      encoded_len++;

      لكل (بیعلامت j = 0; j < 64; j++)
      {
        int16_t مقدار_val = values.arrayZ[شروع + j];
        *it++ = static_cast<حرف> (مقدار_val >> 8);
        *it++ = static_cast<حرف> (مقدار_val & 0xFF);

        encoded_len += 2;
      }

      شروع += 64;
      پیاپی_درازی -= 64;
    }

    إذا (پیاپی_درازی)
    {
      *it++ = (مقادیر_ARE_واژگان | (پیاپی_درازی - 1));
      encoded_len++;
      طالما (شروع < i)
      {
        int16_t مقدار_val = values.arrayZ[شروع++];
        *it++ = static_cast<حرف> (مقدار_val >> 8);
        *it++ = static_cast<حرف> (مقدار_val & 0xFF);

        encoded_len += 2;
      }
    }
    أرجع encoded_len;
  }

  ساكن بیعلامت encode_مقدار_پیاپی_as_longs (بیعلامت &i,
					     بیعلامت حرف *it,
					     hb_array_t<ثابت صحيح> values)
  {
    بیعلامت شروع = i;
    بیعلامت رقم_مقادیر_3 = values.درازی;
    طالما (i < رقم_مقادیر_3)
    {
      صحيح val = values.arrayZ[i];

      إذا ((int16_t) val == val)
        اكسر;

      i++;
    }

    بیعلامت پیاپی_درازی = i - شروع;
    بیعلامت encoded_len = 0;
    طالما (پیاپی_درازی >= 64)
    {
      *it++ = (مقادیر_ARE_LONGS | 63);
      encoded_len++;

      لكل (بیعلامت j = 0; j < 64; j++)
      {
        int32_t مقدار_val = values.arrayZ[شروع + j];
        *it++ = static_cast<حرف> (مقدار_val >> 24);
        *it++ = static_cast<حرف> (مقدار_val >> 16);
        *it++ = static_cast<حرف> (مقدار_val >> 8);
        *it++ = static_cast<حرف> (مقدار_val & 0xFF);

        encoded_len += 4;
      }

      شروع += 64;
      پیاپی_درازی -= 64;
    }

    إذا (پیاپی_درازی)
    {
      *it++ = (مقادیر_ARE_LONGS | (پیاپی_درازی - 1));
      encoded_len++;
      طالما (شروع < i)
      {
        int32_t مقدار_val = values.arrayZ[شروع++];
        *it++ = static_cast<حرف> (مقدار_val >> 24);
        *it++ = static_cast<حرف> (مقدار_val >> 16);
        *it++ = static_cast<حرف> (مقدار_val >> 8);
        *it++ = static_cast<حرف> (مقدار_val & 0xFF);

        encoded_len += 4;
      }
    }
    أرجع encoded_len;
  }

  قالب <نام_نوع T>
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  ساكن منطقي decompile (ثابت HBUINT8 *&p /* IN/OUT */,
			 hb_وکتور_t<T> &values /* IN/OUT */,
			 ثابت HBUINT8 *انتها,
			 منطقي consume_all = خطأ,
			 بیعلامت شروع = 0)
  {
    بیعلامت i = 0;
    بیعلامت شمار = consume_all ? UINT_MAX : values.درازی;
    إذا (consume_all)
      values.alloc ((انتها - p) / 2);
    طالما (i < شمار)
    {
      إذا (unlikely (p + 1 > انتها)) أرجع consume_all;
      بیعلامت control = *p++;
      بیعلامت پیاپی_شمار = (control & مقدار_پیاپی_شمار_ماسک) + 1;
      إذا (consume_all)
      {
        إذا (unlikely (!values.resize_dirty  (values.درازی + پیاپی_شمار)))
	  أرجع خطأ;
      }
      بیعلامت stop = i + پیاپی_شمار;
      إذا (unlikely (stop > شمار)) أرجع خطأ;

      بیعلامت skip = i < شروع ? hb_min (شروع - i, پیاپی_شمار) : 0;
      i += skip;

      إذا ((control & مقادیر_اندازه_ماسک) == مقادیر_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * حجم (T));
	i = stop;
      }
      وإلا إذا ((control & مقادیر_اندازه_ماسک) ==  مقادیر_ARE_واژگان)
      {
        إذا (unlikely (p + پیاپی_شمار * HBINT16::static_اندازه > انتها)) أرجع خطأ;
	p += skip * HBINT16::static_اندازه;
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
        لكل (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_اندازه;
	  values.arrayZ[i + 1] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_اندازه;
	  values.arrayZ[i + 2] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_اندازه;
	  values.arrayZ[i + 3] = * (ثابت HBINT16 *) p;
	  p += HBINT16::static_اندازه;
	}
#پایان_اگر
        لكل (; i < stop; i++)
        {
          values.arrayZ[i] = * (ثابت HBINT16 *) p;
          p += HBINT16::static_اندازه;
        }
      }
      وإلا إذا ((control & مقادیر_اندازه_ماسک) ==  مقادیر_ARE_LONGS)
      {
        إذا (unlikely (p + پیاپی_شمار * HBINT32::static_اندازه > انتها)) أرجع خطأ;
	p += skip * HBINT32::static_اندازه;
        لكل (; i < stop; i++)
        {
          values.arrayZ[i] = * (ثابت HBINT32 *) p;
          p += HBINT32::static_اندازه;
        }
      }
      وإلا إذا ((control & مقادیر_اندازه_ماسک) ==  مقادیر_ARE_بایتها)
      {
        إذا (unlikely (p + پیاپی_شمار > انتها)) أرجع خطأ;
	p += skip * HBINT8::static_اندازه;
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
	لكل (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (ثابت HBINT8 *) p++;
	}
#پایان_اگر
        لكل (; i < stop; i++)
          values.arrayZ[i] = * (ثابت HBINT8 *) p++;
      }
    }
    أرجع درست;
  }

  ساكن منطقي skip (ثابت بیعلامت حرف *&p /* IN/OUT */,
		    ثابت بیعلامت حرف *انتها,
		    بیعلامت شمار)
  {
    بیعلامت دیده = 0;
    طالما (دیده < شمار)
    {
      إذا (unlikely (p >= انتها)) أرجع خطأ;
      بیعلامت control = *p++;
      بیعلامت پیاپی_شمار = (control & مقدار_پیاپی_شمار_ماسک) + 1;
      إذا (unlikely (پیاپی_شمار > شمار - دیده)) أرجع خطأ;

      بیعلامت عرض = 0;
      بدل (control & مقادیر_اندازه_ماسک)
      {
	حالة مقادیر_ARE_ZEROS: اكسر;
	حالة مقادیر_ARE_بایتها: عرض = HBINT8::static_اندازه; اكسر;
	حالة مقادیر_ARE_واژگان: عرض = HBINT16::static_اندازه; اكسر;
	حالة مقادیر_ARE_LONGS: عرض = HBINT32::static_اندازه; اكسر;
      }
      إذا (unlikely (بیعلامت (انتها - p) < پیاپی_شمار * عرض)) أرجع خطأ;
      p += پیاپی_شمار * عرض;
      دیده += پیاپی_شمار;
    }
    أرجع درست;
  }

  بنية iter_t : hb_iter_with_fallback_t<iter_t, صحيح>
  {
    iter_t (ثابت بیعلامت حرف *p_, بیعلامت len_)
	    : p (p_), endp (p_ + len_)
    { إذا (likely (ensure_پیاپی ())) read_مقدار (); }

    خاص:
    ثابت بیعلامت حرف *p;
    ثابت بیعلامت حرف * ثابت endp;
    صحيح current_مقدار = 0;
    علامتدار پیاپی_شمار = 0;
    بیعلامت عرض = 0;

    HB_ALWAYS_INLINE
    منطقي ensure_پیاپی ()
    {
      إذا (likely (پیاپی_شمار > 0)) أرجع درست;
      أرجع ensure_پیاپی_2 ();
    }
    منطقي ensure_پیاپی_2 ()
    {
      إذا (unlikely (p >= endp))
      {
        پیاپی_شمار = 0;
        current_مقدار = 0;
	أرجع خطأ;
      }

      بیعلامت control = *p++;
      پیاپی_شمار = (control & مقدار_پیاپی_شمار_ماسک) + 1;
      عرض = control & مقادیر_اندازه_ماسک;
      بدل (عرض)
      {
        حالة مقادیر_ARE_ZEROS: عرض = 0; اكسر;
	حالة مقادیر_ARE_بایتها: عرض = HBINT8::static_اندازه;  اكسر;
	حالة مقادیر_ARE_واژگان: عرض = HBINT16::static_اندازه; اكسر;
	حالة مقادیر_ARE_LONGS: عرض = HBINT32::static_اندازه; اكسر;
	افتراضي: أكد (خطأ);
      }

      إذا (unlikely (p + پیاپی_شمار * عرض > endp))
      {
	پیاپی_شمار = 0;
	current_مقدار = 0;
	أرجع خطأ;
      }

      أرجع درست;
    }
    فراغ read_مقدار ()
    {
      بدل (عرض)
      {
        حالة 0: current_مقدار = 0; اكسر;
	حالة 1: current_مقدار = * (ثابت HBINT8  *) p; اكسر;
	حالة 2: current_مقدار = * (ثابت HBINT16 *) p; اكسر;
	حالة 4: current_مقدار = * (ثابت HBINT32 *) p; اكسر;
      }
      p += عرض;
    }

    عام:

    تعريفنوع صحيح __item_t__;
    __item_t__ __item__ () ثابت
    { أرجع current_مقدار; }

    منطقي __more__ () ثابت { أرجع پیاپی_شمار || p < endp; }
    فراغ __next__ ()
    {
      پیاپی_شمار--;
      إذا (unlikely (!ensure_پیاپی ()))
	أرجع;
      read_مقدار ();
    }
    فراغ __forward__ (بیعلامت n)
    {
      إذا (unlikely (!ensure_پیاپی ()))
	أرجع;
      طالما (n)
      {
	بیعلامت i = hb_min (n, (بیعلامت) پیاپی_شمار);
	پیاپی_شمار -= i;
	n -= i;
	p += (i - 1) * عرض;
	إذا (unlikely (!ensure_پیاپی ()))
	  أرجع;
	read_مقدار ();
      }
    }
    منطقي عملگر != (ثابت iter_t& o) ثابت
    { أرجع p != o.p || پیاپی_شمار != o.پیاپی_شمار; }
    iter_t __end__ () ثابت
    {
      iter_t it (endp, 0);
      أرجع it;
    }
  };

  بنية fetcher_t
  {
    fetcher_t (ثابت بیعلامت حرف *p_, بیعلامت len_)
	      : p (p_), انتها (p_ + len_) {}

    خاص:
    ثابت بیعلامت حرف *p;
    ثابت بیعلامت حرف * ثابت انتها;
    علامتدار پیاپی_شمار = 0;
    بیعلامت عرض = 0;

    HB_ALWAYS_INLINE
    منطقي ensure_پیاپی ()
    {
      إذا (likely (پیاپی_شمار > 0)) أرجع درست;
      أرجع ensure_پیاپی_2 ();
    }

    منطقي ensure_پیاپی_2 ()
    {
      إذا (unlikely (p >= انتها))
      {
        پیاپی_شمار = 0;
	أرجع خطأ;
      }

      بیعلامت control = *p++;
      پیاپی_شمار = (control & مقدار_پیاپی_شمار_ماسک) + 1;
      عرض = control & مقادیر_اندازه_ماسک;
      بدل (عرض)
      {
        حالة مقادیر_ARE_ZEROS: عرض = 0; اكسر;
	حالة مقادیر_ARE_بایتها: عرض = HBINT8::static_اندازه;  اكسر;
	حالة مقادیر_ARE_واژگان: عرض = HBINT16::static_اندازه; اكسر;
	حالة مقادیر_ARE_LONGS: عرض = HBINT32::static_اندازه; اكسر;
	افتراضي: أكد (خطأ);
      }

      إذا (unlikely (p + پیاپی_شمار * عرض > انتها))
      {
	پیاپی_شمار = 0;
	أرجع خطأ;
      }

      أرجع درست;
    }

    عام:
    فراغ skip (بیعلامت n)
    {
      طالما (n)
      {
	إذا (unlikely (!ensure_پیاپی ()))
	  أرجع;
	بیعلامت i = hb_min (n, (بیعلامت) پیاپی_شمار);
	پیاپی_شمار -= i;
	n -= i;
	p += i * عرض;
      }
    }

    خاص:
    قالب <منطقي scaled, نام_نوع T>
    فراغ جمع_کن_مقصد (hb_array_t<T> بیرون, T scale = T (1))
    {
      بیعلامت n = بیرون.درازی;
      T *arrayZ = بیرون.arrayZ;

      لكل (بیعلامت i = 0; i < n;)
      {
	إذا (unlikely (!ensure_پیاپی ()))
	  اكسر;
	بیعلامت شمار = hb_min (n - i, (بیعلامت) پیاپی_شمار);
	بدل (عرض)
	{
	  حالة 0:
	  {
	    arrayZ += شمار;
	    اكسر;
	  }
	  حالة 1:
	  {
	    ثابت خودکار *pp = (ثابت HBINT8 *) p;
	    بیعلامت j = 0;
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
	    لكل (; j + 3 < شمار; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#پایان_اگر
	    لكل (; j < شمار; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت بیعلامت حرف *) pp;
	  }
	  اكسر;
	  حالة 2:
	  {
	    ثابت خودکار *pp = (ثابت HBINT16 *) p;
	    بیعلامت j = 0;
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
	    لكل (; j + 3 < شمار; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#پایان_اگر
	    لكل (; j < شمار; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت بیعلامت حرف *) pp;
	  }
	  اكسر;
	  حالة 4:
	  {
	    ثابت خودکار *pp = (ثابت HBINT32 *) p;
	    لكل (بیعلامت j = 0; j < شمار; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت بیعلامت حرف *) pp;
	  }
	  اكسر;
	}
	پیاپی_شمار -= شمار;
	i += شمار;
      }
    }

    عام:
    قالب <نام_نوع T>
    فراغ جمع_کن_مقصد_2 (hb_array_t<T> بیرون, عائم scale = 1.0f)
    {
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      إذا (خطأ && scale == 1.0f)
        جمع_کن_مقصد<خطأ> (بیرون);
      وإلا
#پایان_اگر
        جمع_کن_مقصد<درست> (بیرون, (T) scale);
    }
  };
};

بنية TupleList : CFF2_شاخص
{
  Tuple_مقادیر::iter_t عملگر [] (بیعلامت i) ثابت
  {
    خودکار بایتها = CFF2_شاخص::عملگر [] (i);
    أرجع Tuple_مقادیر::iter_t (بایتها.arrayZ, بایتها.درازی);
  }

  Tuple_مقادیر::fetcher_t fetcher (بیعلامت i) ثابت
  {
    خودکار بایتها = CFF2_شاخص::عملگر [] (i);
    أرجع Tuple_مقادیر::fetcher_t (بایتها.arrayZ, بایتها.درازی);
  }
};


// Alignment

قالب <بیعلامت صحيح alignment>
بنية Align
{
  size_t get_اندازه (ثابت فراغ *پایه) ثابت
  {
    بیعلامت جابه_جایی = (ثابت حرف *) هذا - (ثابت حرف *) پایه;
    أرجع (alignment - جابه_جایی) & (alignment - 1);
  }

  عام:
  DEFINE_اندازه_MIN (0);
};



} /* namespace OT */


#پایان_اگر /* HB_OPEN_TYPE_HH */
