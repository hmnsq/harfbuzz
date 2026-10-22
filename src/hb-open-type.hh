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

#که_نه_تعریف_شوی HB_OPEN_TYPE_HH
#تعریف_کړه HB_OPEN_TYPE_HH

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
کينډۍ <منطقي BE,
	  ډول_نوم Type,
	  بینښه صحيح اندازه_2 = حجم (Type)>
بنية عدد_Type
{
  تعريفنوع Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  تعريفنوع ډول_نوم std::conditional<std::is_integral<Type>::value && حجم (Type) <= حجم(صحيح),
				     ډول_نوم std::conditional<std::is_signed<Type>::value, نښهدار, بینښه>::type,
				     Type>::type WideType;

  عدد_Type () = افتراضي;
  څرګند constexpr عدد_Type (Type V) : v {V} {}
  عدد_Type& چلوونکی = (Type V) { v = V; أرجع *هذا; }

  چلوونکی WideType () ثابت { أرجع v; }

  منطقي چلوونکی == (ثابت عدد_Type &o) ثابت { أرجع (Type) v == (Type) o.v; }
  منطقي چلوونکی != (ثابت عدد_Type &o) ثابت { أرجع !(*هذا == o); }

  عدد_Type& چلوونکی += (WideType شمېر) { *هذا = *هذا + شمېر; أرجع *هذا; }
  عدد_Type& چلوونکی -= (WideType شمېر) { *هذا = *هذا - شمېر; أرجع *هذا; }
  عدد_Type& چلوونکی ++ () { *هذا += 1; أرجع *هذا; }
  عدد_Type& چلوونکی -- () { *هذا -= 1; أرجع *هذا; }
  عدد_Type چلوونکی ++ (صحيح) { عدد_Type c (*هذا); ++*هذا; أرجع c; }
  عدد_Type چلوونکی -- (صحيح) { عدد_Type c (*هذا); --*هذا; أرجع c; }

  uint32_t hash () ثابت { أرجع hb_array ((ثابت حرف *) &v, حجم (v)).hash (); }
  HB_دننی ساكن صحيح cmp (ثابت عدد_Type *a, ثابت عدد_Type *b)
  { أرجع b->cmp (*a); }
  HB_دننی ساكن صحيح cmp (ثابت فراغ *a, ثابت فراغ *b)
  {
    عدد_Type *pa = (عدد_Type *) a;
    عدد_Type *pb = (عدد_Type *) b;

    أرجع pb->cmp (*pa);
  }
  کينډۍ <ډول_نوم Type2,
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
  ډول_نوم std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, اندازه_2>,
			    HBFloat<BE, Type, اندازه_2>>::type v;
  عام:
  DEFINE_اندازه_STATIC (اندازه_2);
};

تعريفنوع عدد_Type<ریښتیا, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
تعريفنوع عدد_Type<ریښتیا, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
تعريفنوع عدد_Type<ریښتیا, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
تعريفنوع عدد_Type<ریښتیا, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
تعريفنوع عدد_Type<ریښتیا, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
تعريفنوع عدد_Type<ریښتیا, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
تعريفنوع عدد_Type<ریښتیا, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
تعريفنوع عدد_Type<ریښتیا, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
تعريفنوع عدد_Type<ریښتیا, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

تعريفنوع عدد_Type<خطأ, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
تعريفنوع عدد_Type<خطأ, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
تعريفنوع عدد_Type<خطأ, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
تعريفنوع عدد_Type<خطأ, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
تعريفنوع عدد_Type<خطأ, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
تعريفنوع عدد_Type<خطأ, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

تعريفنوع عدد_Type<ریښتیا,  عائم>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
تعريفنوع عدد_Type<ریښتیا,  مزدوج> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
تعريفنوع عدد_Type<خطأ, عائم>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
تعريفنوع عدد_Type<خطأ, مزدوج> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
بنية HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& چلوونکی = (uint16_t i ) { HBUINT16::چلوونکی= (i); أرجع *هذا; }
  عام:
  DEFINE_اندازه_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
بنية HBUINT32VAR
{
  size_t get_اندازه () ثابت
  {
    بینښه b0 = v[0];
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

  چلوونکی uint32_t () ثابت
  {
    بینښه b0 = v[0];
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

  ساكن بینښه serialize_unsafe (بینښه حرف *buf, uint32_t v)
  {
    بینښه len = get_اندازه (v);
    بینښه حرف *p = buf + len;
    لكل (بینښه i = 0; i < len; i++)
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
    بینښه len = get_اندازه (v);
    بینښه حرف *buf = c->allocate_اندازه<بینښه حرف> (len, خطأ);
    إذا (unlikely (!buf))
      أرجع خطأ;

    serialize_unsafe (buf, v);
    أرجع ریښتیا;
  }

  محمي:
  بینښه حرف v[5];

  عام:
  DEFINE_اندازه_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
تعريفنوع HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
تعريفنوع HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
تعريفنوع HBUINT16 UFWORD;

کينډۍ <ډول_نوم Type, بینښه fraction_بټونه>
بنية HBFixed : Type
{
  ساكن constexpr عائم mult = 1.f / (1 << fraction_بټونه);
  static_assert (Type::static_اندازه * 8 > fraction_بټونه, "");

  چلوونکی نښهدار () ثابت = ړنګول;
  چلوونکی بینښه () ثابت = ړنګول;
  څرګند چلوونکی عائم () ثابت { أرجع منزل_float (); }
  ډول_نوم Type::type منزل_int () ثابت { أرجع Type::v; }
  فراغ سېټ_int (ډول_نوم Type::type i ) { Type::v = i; }
  عائم منزل_float (عائم دننه_واټن = 0) ثابت  { أرجع ((int32_t) Type::v + دننه_واټن) * mult; }
  فراغ سېټ_float (عائم f) { Type::v = roundf (f / mult); }
  عام:
  DEFINE_اندازه_STATIC (Type::static_اندازه);
};

/* 16-bit signed fixed number with the low 14 bits of fraction (2.14). */
کارول F2DOT14 = HBFixed<HBINT16, 14>;
کارول F4DOT12 = HBFixed<HBINT16, 12>;
کارول F6DOT10 = HBFixed<HBINT16, 10>;

/* 32-bit signed fixed-point number (16.16). */
کارول F16DOT16 = HBFixed<HBINT32, 16>;

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
بنية نښان : HBUINT32
{
  نښان& چلوونکی = (hb_نښان_t i) { HBUINT32::چلوونکی= (i); أرجع *هذا; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  چلوونکی ثابت حرف* () ثابت { أرجع reinterpret_cast<ثابت حرف *> (هذا); }
  چلوونکی حرف* ()             { أرجع reinterpret_cast<حرف *> (هذا); }
  عام:
  DEFINE_اندازه_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
بنية HBGlyphID16 : HBUINT16
{
  HBGlyphID16& چلوونکی = (uint16_t i) { HBUINT16::چلوونکی= (i); أرجع *هذا; }
};
بنية HBGlyphID24 : HBUINT24
{
  HBGlyphID24& چلوونکی = (uint32_t i) { HBUINT24::چلوونکی= (i); أرجع *هذا; }
};

/* Script/language-system/feature index */
بنية شاخص : HBUINT16 {
  ساكن constexpr بینښه NOT_وموندل_شاخص = 0xFFFFu;
  شاخص& چلوونکی = (uint16_t i) { HBUINT16::چلوونکی= (i); أرجع *هذا; }
};
DECLARE_NULL_NAMESPACE_بایټونه (OT, شاخص);

تعريفنوع شاخص NameID;

بنية VarIdx : HBUINT32 {
  ساكن constexpr بینښه NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_شاخص, "");
  VarIdx& چلوونکی = (uint32_t i) { HBUINT32::چلوونکی= (i); أرجع *هذا; }
};
DECLARE_NULL_NAMESPACE_بایټونه (OT, VarIdx);

/* Offset, Null offset = 0 */
کينډۍ <ډول_نوم Type, منطقي has_null=ریښتیا>
بنية دننه_واټن_2 : Type
{
  دننه_واټن_2& چلوونکی = (ډول_نوم Type::type i) { Type::چلوونکی= (i); أرجع *هذا; }

  تعريفنوع Type type;

  منطقي is_null () ثابت { أرجع has_null && 0 == *هذا; }

  عام:
  DEFINE_اندازه_STATIC (حجم (Type));
};

تعريفنوع دننه_واټن_2<HBUINT16> Offset16;
تعريفنوع دننه_واټن_2<HBUINT24> Offset24;
تعريفنوع دننه_واټن_2<HBUINT32> Offset32;


/* CheckSum */
بنية Check_مجموعه : HBUINT32
{
  Check_مجموعه& چلوونکی = (uint32_t i) { HBUINT32::چلوونکی= (i); أرجع *هذا; }

  /* This is reference implementation from the spec. */
  ساكن uint32_t Calc_جدول_Checksum (ثابت HBUINT32 *جدول_3, uint32_t اوږدتیا_3)
  {
    uint32_t Sum = 0L;
    أكد (0 == (اوږدتیا_3 & 3));
    ثابت HBUINT32 *پای_ټکی_Ptr = جدول_3 + اوږدتیا_3 / HBUINT32::static_اندازه;

    طالما (جدول_3 < پای_ټکی_Ptr)
      Sum += *جدول_3++;
    أرجع Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  فراغ سېټ_for_معلومات (ثابت فراغ *معلومات, بینښه صحيح اوږدتیا)
  { *هذا = Calc_جدول_Checksum ((ثابت HBUINT32 *) معلومات, اوږدتیا); }

  عام:
  DEFINE_اندازه_STATIC (4);
};


/*
 * Version Numbers
 */

کينډۍ <ډول_نوم FixedType=HBUINT16>
بنية Fixed_نسخه
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
  DEFINE_اندازه_STATIC (2 * حجم (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

کينډۍ <ډول_نوم Type, منطقي has_null>
بنية _hb_has_null
{
  ساكن ثابت Type *get_null () { أرجع تش_اشاره; }
  ساكن Type *get_crap ()       { أرجع تش_اشاره; }
};
کينډۍ <ډول_نوم Type>
بنية _hb_has_null<Type, ریښتیا>
{
  ساكن ثابت Type *get_null () { أرجع &Null (Type); }
  ساكن       Type *get_crap () { أرجع &Crap (Type); }
};

کينډۍ <ډول_نوم Type, ډول_نوم دننه_واټن_Type, ډول_نوم بنسټ_Type=فراغ, منطقي has_null=ریښتیا>
بنية دننه_واټن_منزل : دننه_واټن_2<دننه_واټن_Type, has_null>
{
  کارول موخه_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == خطأ ||
		 (hb_has_null_اندازه_2 (Type) || !hb_has_min_اندازه_2 (Type)), "");

  HB_DELETE_COPY_ټاکل (دننه_واټن_منزل);
  دننه_واټن_منزل () = افتراضي;

  دننه_واټن_منزل& چلوونکی = (ډول_نوم دننه_واټن_Type::type i) { دننه_واټن_Type::چلوونکی= (i); أرجع *هذا; }

  ثابت Type& چلوونکی () (ثابت فراغ *بنسټ) ثابت
  {
    إذا (unlikely (هذا->is_null ())) أرجع *_hb_has_null<Type, has_null>::get_null ();
    أرجع Struct_At_دننه_واټن<ثابت Type> (بنسټ, *هذا);
  }
  Type& چلوونکی () (فراغ *بنسټ) ثابت
  {
    إذا (unlikely (هذا->is_null ())) أرجع *_hb_has_null<Type, has_null>::get_crap ();
    أرجع Struct_At_دننه_واټن<Type> (بنسټ, *هذا);
  }

  کينډۍ <ډول_نوم بنسټ_3,
	    hb_enable_if (hb_is_convertible (ثابت بنسټ_3, ثابت بنسټ_Type *))>
  ملګری ثابت Type& چلوونکی + (ثابت بنسټ_3 &بنسټ, ثابت دننه_واټن_منزل &دننه_واټن) { أرجع دننه_واټن ((ثابت فراغ *) بنسټ); }
  کينډۍ <ډول_نوم بنسټ_3,
	    hb_enable_if (hb_is_convertible (ثابت بنسټ_3, ثابت بنسټ_Type *))>
  ملګری ثابت Type& چلوونکی + (ثابت دننه_واټن_منزل &دننه_واټن, ثابت بنسټ_3 &بنسټ) { أرجع دننه_واټن ((ثابت فراغ *) بنسټ); }
  کينډۍ <ډول_نوم بنسټ_3,
	    hb_enable_if (hb_is_convertible (بنسټ_3, بنسټ_Type *))>
  ملګری Type& چلوونکی + (بنسټ_3 &&بنسټ, دننه_واټن_منزل &دننه_واټن) { أرجع دننه_واټن ((فراغ *) بنسټ); }
  کينډۍ <ډول_نوم بنسټ_3,
	    hb_enable_if (hb_is_convertible (بنسټ_3, بنسټ_Type *))>
  ملګری Type& چلوونکی + (دننه_واټن_منزل &دننه_واټن, بنسټ_3 &&بنسټ) { أرجع دننه_واټن ((فراغ *) بنسټ); }


  کينډۍ <ډول_نوم بنسټ_3, ډول_نوم ...Ts>
  منطقي serialize_subset (hb_subset_context_t *c, ثابت دننه_واټن_منزل& src,
			 ثابت بنسټ_3 *src_بنسټ, Ts&&... ds)
  {
    *هذا = 0;
    إذا (src.is_null ())
      أرجع خطأ;

    خپلکاری *s = c->serializer;

    s->push ();

    منطقي ret = c->dispatch (src_بنسټ+src, std::forward<Ts> (ds)...);

    إذا (ret || !has_null)
      s->جمع_کړه_link (*هذا, s->راوباسه_pack ());
    وإلا
      s->راوباسه_discard ();

    أرجع ret;
  }


  کينډۍ <ډول_نوم ...Ts>
  منطقي serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *هذا = 0;

    Type* obj = c->push<Type> ();
    منطقي ret = obj->serialize (c, std::forward<Ts> (ds)...);

    إذا (ret)
      c->جمع_کړه_link (*هذا, c->راوباسه_pack ());
    وإلا
      c->راوباسه_discard ();

    أرجع ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  کينډۍ <ډول_نوم ...Ts>
  منطقي serialize_copy (hb_serialize_context_t *c, ثابت دننه_واټن_منزل& src,
		       ثابت فراغ *src_بنسټ, بینښه dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *هذا = 0;
    إذا (src.is_null ())
      أرجع خطأ;

    c->push ();

    منطقي ret = c->copy (src_بنسټ+src, std::forward<Ts> (ds)...);

    c->جمع_کړه_link (*هذا, c->راوباسه_pack (), whence, dst_bias);

    أرجع ret;
  }

  منطقي serialize_copy (hb_serialize_context_t *c, ثابت دننه_واټن_منزل& src,
		       ثابت فراغ *src_بنسټ, بینښه dst_bias = 0)
  { أرجع serialize_copy (c, src, src_بنسټ, dst_bias, hb_serialize_context_t::سر); }

  منطقي sanitize_shallow (hb_sanitize_context_t *c, ثابت بنسټ_Type *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    إذا (unlikely ((ثابت حرف *) بنسټ + (بینښه) *هذا < (ثابت حرف *) بنسټ)) return_trace (خطأ);
    return_trace (ریښتیا);
  }

  کينډۍ <ډول_نوم ...Ts>
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  منطقي sanitize (hb_sanitize_context_t *c, ثابت بنسټ_Type *بنسټ, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (sanitize_shallow (c, بنسټ) &&
		  hb_barrier () &&
		  (هذا->is_null () ||
		   c->dispatch (Struct_At_دننه_واټن<Type> (بنسټ, *هذا), std::forward<Ts> (ds)...)));
  }

  DEFINE_اندازه_STATIC (حجم (دننه_واټن_Type));
};
/* Partial specializations. */
کينډۍ <ډول_نوم Type, ډول_نوم بنسټ_Type=فراغ, منطقي has_null=ریښتیا> کارول Offset16_منزل = دننه_واټن_منزل<Type, HBUINT16, بنسټ_Type, has_null>;
کينډۍ <ډول_نوم Type, ډول_نوم بنسټ_Type=فراغ, منطقي has_null=ریښتیا> کارول Offset24_منزل = دننه_واټن_منزل<Type, HBUINT24, بنسټ_Type, has_null>;
کينډۍ <ډول_نوم Type, ډول_نوم بنسټ_Type=فراغ, منطقي has_null=ریښتیا> کارول Offset32_منزل = دننه_واټن_منزل<Type, HBUINT32, بنسټ_Type, has_null>;

کينډۍ <ډول_نوم Type, ډول_نوم دننه_واټن_Type, ډول_نوم بنسټ_Type=فراغ> کارول NNOffset_منزل = دننه_واټن_منزل<Type, دننه_واټن_Type, بنسټ_Type, خطأ>;
کينډۍ <ډول_نوم Type, ډول_نوم بنسټ_Type=فراغ> کارول NNOffset16_منزل = Offset16_منزل<Type, بنسټ_Type, خطأ>;
کينډۍ <ډول_نوم Type, ډول_نوم بنسټ_Type=فراغ> کارول NNOffset24_منزل = Offset24_منزل<Type, بنسټ_Type, خطأ>;
کينډۍ <ډول_نوم Type, ډول_نوم بنسټ_Type=فراغ> کارول NNOffset32_منزل = Offset32_منزل<Type, بنسټ_Type, خطأ>;


/*
 * Array Types
 */

کينډۍ <ډول_نوم Type>
بنية Unsized_Array_د
{
  تعريفنوع Type توکی_t;
  ساكن constexpr بینښه توکی_اندازه = hb_static_اندازه (Type);

  HB_DELETE_CREATE_COPY_ټاکل (Unsized_Array_د);

  ثابت Type& چلوونکی [] (بینښه صحيح i) ثابت
  {
    أرجع arrayZ[i];
  }
  Type& چلوونکی [] (بینښه صحيح i)
  {
    أرجع arrayZ[i];
  }

  ساكن size_t get_اندازه (بینښه صحيح len)
  { أرجع hb_unsigned_mul_saturate (len, Type::static_اندازه); }

  کينډۍ <ډول_نوم T> چلوونکی T * () { أرجع arrayZ; }
  کينډۍ <ډول_نوم T> چلوونکی ثابت T * () ثابت { أرجع arrayZ; }
  hb_array_t<Type> as_array (بینښه صحيح len)
  { أرجع hb_array (arrayZ, len); }
  hb_array_t<ثابت Type> as_array (بینښه صحيح len) ثابت
  { أرجع hb_array (arrayZ, len); }

  کينډۍ <ډول_نوم T>
  Type &lsearch (بینښه صحيح len, ثابت T &x, Type &not_وموندل = Crap (Type))
  { أرجع *as_array (len).lsearch (x, &not_وموندل); }
  کينډۍ <ډول_نوم T>
  ثابت Type &lsearch (بینښه صحيح len, ثابت T &x, ثابت Type &not_وموندل = Null (Type)) ثابت
  { أرجع *as_array (len).lsearch (x, &not_وموندل); }
  کينډۍ <ډول_نوم T>
  منطقي lfind (بینښه صحيح len, ثابت T &x, بینښه صحيح *i = تش_اشاره,
	      hb_not_وموندل_t not_وموندل = HB_NOT_وموندل_DONT_STORE,
	      بینښه صحيح منزل_store = (بینښه صحيح) -1) ثابت
  { أرجع as_array (len).lfind (x, i, not_وموندل, منزل_store); }

  فراغ qsort (بینښه صحيح len, بینښه صحيح پیل = 0, بینښه صحيح پای_ټکی = (بینښه صحيح) -1)
  { as_array (len).qsort (پیل, پای_ټکی); }

  منطقي serialize (hb_serialize_context_t *c, بینښه صحيح توکي_len, منطقي clear = ریښتیا)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_اندازه (هذا, get_اندازه (توکي_len), clear))) return_trace (خطأ);
    return_trace (ریښتیا);
  }
  کينډۍ <ډول_نوم Iterator,
	    hb_requires (hb_is_سرچینه_د (Iterator, Type))>
  منطقي serialize (hb_serialize_context_t *c, Iterator توکي)
  {
    TRACE_SERIALIZE (هذا);
    بینښه شمېر = hb_len (توکي);
    إذا (unlikely (!serialize (c, شمېر, خطأ))) return_trace (خطأ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    لكل (بینښه i = 0; i < شمېر; i++, ++توکي)
      arrayZ[i] = *توکي;
    return_trace (ریښتیا);
  }

  Unsized_Array_د* copy (hb_serialize_context_t *c, بینښه شمېر) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری *بهر = c->پیل_embed (هذا);
    إذا (unlikely (!as_array (شمېر).copy (c))) return_trace (تش_اشاره);
    return_trace (بهر);
  }

  کينډۍ <ډول_نوم ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, بینښه صحيح شمېر, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c, شمېر))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ریښتیا);
    hb_barrier ();
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (خطأ);
    return_trace (ریښتیا);
  }

  منطقي sanitize_shallow (hb_sanitize_context_t *c, بینښه صحيح شمېر) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_array (arrayZ, شمېر));
  }

  عام:
  Type		arrayZ[HB_VAR_ARRAY];
  عام:
  DEFINE_اندازه_UNBOUNDED (0);
};

/* Unsized array of offset's */
کينډۍ <ډول_نوم Type, ډول_نوم دننه_واټن_Type, ډول_نوم بنسټ_Type=فراغ, منطقي has_null=ریښتیا>
کارول Unsized_Array16_د_دننه_واټن_منزل = Unsized_Array_د<دننه_واټن_منزل<Type, دننه_واټن_Type, بنسټ_Type, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
کينډۍ <ډول_نوم Type, ډول_نوم دننه_واټن_Type, ډول_نوم بنسټ_Type=فراغ, منطقي has_null=ریښتیا>
بنية Unsized_List_د_Offset16_منزل : Unsized_Array16_د_دننه_واټن_منزل<Type, دننه_واټن_Type, بنسټ_Type, has_null>
{
  ثابت Type& چلوونکی [] (صحيح i_) ثابت
  {
    بینښه صحيح i = (بینښه صحيح) i_;
    ثابت دننه_واټن_منزل<Type, دننه_واټن_Type, بنسټ_Type, has_null> *p = &هذا->arrayZ[i];
    إذا (unlikely ((ثابت فراغ *) p < (ثابت فراغ *) هذا->arrayZ)) أرجع Null (Type); /* Overflowed. */
    hb_barrier ();
    أرجع هذا+*p;
  }
  Type& چلوونکی [] (صحيح i_)
  {
    بینښه صحيح i = (بینښه صحيح) i_;
    ثابت دننه_واټن_منزل<Type, دننه_واټن_Type, بنسټ_Type, has_null> *p = &هذا->arrayZ[i];
    إذا (unlikely ((ثابت فراغ *) p < (ثابت فراغ *) هذا->arrayZ)) أرجع Crap (Type); /* Overflowed. */
    hb_barrier ();
    أرجع هذا+*p;
  }

  کينډۍ <ډول_نوم ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, بینښه صحيح شمېر, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace ((Unsized_Array16_د_دننه_واټن_منزل<Type, دننه_واټن_Type, بنسټ_Type, has_null>
		   ::sanitize (c, شمېر, هذا, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
کينډۍ <ډول_نوم Type>
بنية Sorted_Unsized_Array_د : Unsized_Array_د<Type>
{
  hb_sorted_array_t<Type> as_array (بینښه صحيح len)
  { أرجع hb_sorted_array (هذا->arrayZ, len); }
  hb_sorted_array_t<ثابت Type> as_array (بینښه صحيح len) ثابت
  { أرجع hb_sorted_array (هذا->arrayZ, len); }
  چلوونکی hb_sorted_array_t<Type> ()             { أرجع as_array (); }
  چلوونکی hb_sorted_array_t<ثابت Type> () ثابت { أرجع as_array (); }

  کينډۍ <ډول_نوم T>
  Type &bsearch (بینښه صحيح len, ثابت T &x, Type &not_وموندل = Crap (Type))
  { أرجع *as_array (len).bsearch (x, &not_وموندل); }
  کينډۍ <ډول_نوم T>
  ثابت Type &bsearch (بینښه صحيح len, ثابت T &x, ثابت Type &not_وموندل = Null (Type)) ثابت
  { أرجع *as_array (len).bsearch (x, &not_وموندل); }
  کينډۍ <ډول_نوم T>
  منطقي bfind (بینښه صحيح len, ثابت T &x, بینښه صحيح *i = تش_اشاره,
	      hb_not_وموندل_t not_وموندل = HB_NOT_وموندل_DONT_STORE,
	      بینښه صحيح منزل_store = (بینښه صحيح) -1) ثابت
  { أرجع as_array (len).bfind (x, i, not_وموندل, منزل_store); }
};


/* An array with a number of elements. */
کينډۍ <ډول_نوم Type, ډول_نوم LenType>
بنية Array_د
{
  تعريفنوع Type توکی_t;
  ساكن constexpr بینښه توکی_اندازه = hb_static_اندازه (Type);

  HB_DELETE_CREATE_COPY_ټاکل (Array_د);

  ثابت Type& چلوونکی [] (صحيح i_) ثابت
  {
    بینښه صحيح i = (بینښه صحيح) i_;
    إذا (unlikely (i >= len)) أرجع Null (Type);
    hb_barrier ();
    أرجع arrayZ[i];
  }
  Type& چلوونکی [] (صحيح i_)
  {
    بینښه صحيح i = (بینښه صحيح) i_;
    إذا (unlikely (i >= len)) أرجع Crap (Type);
    hb_barrier ();
    أرجع arrayZ[i];
  }

  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_mul_جمع_کړه_saturate (len, Type::static_اندازه, len.static_اندازه); }

  څرګند چلوونکی منطقي () ثابت { أرجع len; }

  فراغ راوباسه () { len--; }

  hb_array_t<      Type> as_array ()       { أرجع hb_array (arrayZ, len); }
  hb_array_t<ثابت Type> as_array () ثابت { أرجع hb_array (arrayZ, len); }

  /* Iterator. */
  تعريفنوع hb_array_t<ثابت Type>   iter_t;
  تعريفنوع hb_array_t<      Type> writer_t;
    iter_t   iter () ثابت { أرجع as_array (); }
  writer_t writer ()       { أرجع as_array (); }
  چلوونکی   iter_t () ثابت { أرجع   iter (); }
  چلوونکی writer_t ()       { أرجع writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { أرجع arrayZ; }
  ثابت Type *پای_ټکی () ثابت { أرجع arrayZ + len; }

  کينډۍ <ډول_نوم T>
  Type &lsearch (ثابت T &x, Type &not_وموندل = Crap (Type))
  { أرجع *as_array ().lsearch (x, &not_وموندل); }
  کينډۍ <ډول_نوم T>
  ثابت Type &lsearch (ثابت T &x, ثابت Type &not_وموندل = Null (Type)) ثابت
  { أرجع *as_array ().lsearch (x, &not_وموندل); }
  کينډۍ <ډول_نوم T>
  منطقي lfind (ثابت T &x, بینښه صحيح *i = تش_اشاره,
	      hb_not_وموندل_t not_وموندل = HB_NOT_وموندل_DONT_STORE,
	      بینښه صحيح منزل_store = (بینښه صحيح) -1) ثابت
  { أرجع as_array ().lfind (x, i, not_وموندل, منزل_store); }

  فراغ qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, بینښه توکي_len, منطقي clear = ریښتیا)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    c->check_ټاکل (len, توکي_len, HB_SERIALIZE_ERROR_ARRAY_ډکېدنه);
    إذا (unlikely (!c->extend_اندازه (هذا, get_اندازه (), clear))) return_trace (خطأ);
    return_trace (ریښتیا);
  }
  کينډۍ <ډول_نوم Iterator,
	    hb_requires (hb_is_سرچینه_د (Iterator, Type))>
  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, Iterator توکي)
  {
    TRACE_SERIALIZE (هذا);
    بینښه شمېر = hb_len (توکي);
    إذا (unlikely (!serialize (c, شمېر, خطأ))) return_trace (خطأ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    لكل (بینښه i = 0; i < شمېر; i++, ++توکي)
      arrayZ[i] = *توکي;
    return_trace (ریښتیا);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (هذا);
    len++;
    إذا (unlikely (!len || !c->extend (هذا)))
    {
      len--;
      return_trace (تش_اشاره);
    }
    return_trace (&arrayZ[len - 1]);
  }

  Array_د* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری *بهر = c->پیل_embed (هذا);
    إذا (unlikely (!c->extend_min (بهر))) return_trace (تش_اشاره);
    c->check_ټاکل (بهر->len, len, HB_SERIALIZE_ERROR_ARRAY_ډکېدنه);
    إذا (unlikely (!as_array ().copy (c))) return_trace (تش_اشاره);
    return_trace (بهر);
  }

  کينډۍ <ډول_نوم ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ریښتیا);
    hb_barrier ();
    بینښه صحيح شمېر = len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (خطأ);
    return_trace (ریښتیا);
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
کينډۍ <ډول_نوم Type> کارول Array16_د = Array_د<Type, HBUINT16>;
کينډۍ <ډول_نوم Type> کارول Array24_د = Array_د<Type, HBUINT24>;
کينډۍ <ډول_نوم Type> کارول Array32_د = Array_د<Type, HBUINT32>;
کارول PString = Array_د<HBUINT8, HBUINT8>;

/* Array of Offset's */
کينډۍ <ډول_نوم Type> کارول Array8_د_Offset24_منزل = Array_د<دننه_واټن_منزل<Type, HBUINT24>, HBUINT8>;
کينډۍ <ډول_نوم Type> کارول Array16_د_Offset16_منزل = Array_د<دننه_واټن_منزل<Type, HBUINT16>, HBUINT16>;
کينډۍ <ډول_نوم Type> کارول Array16_د_Offset32_منزل = Array_د<دننه_واټن_منزل<Type, HBUINT32>, HBUINT16>;
کينډۍ <ډول_نوم Type> کارول Array32_د_Offset32_منزل = Array_د<دننه_واټن_منزل<Type, HBUINT32>, HBUINT32>;

/* Array of offsets relative to the beginning of the array itself. */
کينډۍ <ډول_نوم Type, ډول_نوم دننه_واټن_Type>
بنية List16_د_دننه_واټن_منزل : Array_د<دننه_واټن_منزل<Type, دننه_واټن_Type>, HBUINT16>
{
  ثابت Type& چلوونکی [] (صحيح i_) ثابت
  {
    بینښه صحيح i = (بینښه صحيح) i_;
    إذا (unlikely (i >= هذا->len)) أرجع Null (Type);
    hb_barrier ();
    أرجع هذا+هذا->arrayZ[i];
  }
  ثابت Type& چلوونکی [] (صحيح i_)
  {
    بینښه صحيح i = (بینښه صحيح) i_;
    إذا (unlikely (i >= هذا->len)) أرجع Crap (Type);
    hb_barrier ();
    أرجع هذا+هذا->arrayZ[i];
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    بنية List16_د_دننه_واټن_منزل *بهر = c->serializer->embed (*هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);
    بینښه صحيح شمېر = هذا->len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      بهر->arrayZ[i].serialize_subset (c, هذا->arrayZ[i], هذا, بهر);
    return_trace (ریښتیا);
  }

  کينډۍ <ډول_نوم ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace ((Array16_د<دننه_واټن_منزل<Type, دننه_واټن_Type>>::sanitize (c, هذا, std::forward<Ts> (ds)...)));
  }
};

کينډۍ <ډول_نوم Type>
کارول List16_د_Offset16_منزل = List16_د_دننه_واټن_منزل<Type, HBUINT16>;

/* An array starting at second element. */
کينډۍ <ډول_نوم Type, ډول_نوم LenType>
بنية Headless_Array_د
{
  ساكن constexpr بینښه توکی_اندازه = Type::static_اندازه;

  HB_DELETE_CREATE_COPY_ټاکل (Headless_Array_د);

  ثابت Type& چلوونکی [] (صحيح i_) ثابت
  {
    بینښه صحيح i = (بینښه صحيح) i_;
    إذا (unlikely (i >= lenP1 || !i)) أرجع Null (Type);
    hb_barrier ();
    أرجع arrayZ[i-1];
  }
  Type& چلوونکی [] (صحيح i_)
  {
    بینښه صحيح i = (بینښه صحيح) i_;
    إذا (unlikely (i >= lenP1 || !i)) أرجع Crap (Type);
    hb_barrier ();
    أرجع arrayZ[i-1];
  }
  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_mul_جمع_کړه_saturate (get_اوږدتیا (), Type::static_اندازه, lenP1.static_اندازه); }

  بینښه get_اوږدتیا () ثابت { أرجع lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { أرجع hb_array (arrayZ, get_اوږدتیا ()); }
  hb_array_t<ثابت Type> as_array () ثابت { أرجع hb_array (arrayZ, get_اوږدتیا ()); }

  /* Iterator. */
  تعريفنوع hb_array_t<ثابت Type>   iter_t;
  تعريفنوع hb_array_t<      Type> writer_t;
    iter_t   iter () ثابت { أرجع as_array (); }
  writer_t writer ()       { أرجع as_array (); }
  چلوونکی   iter_t () ثابت { أرجع   iter (); }
  چلوونکی writer_t ()       { أرجع writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { أرجع arrayZ; }
  ثابت Type *پای_ټکی () ثابت { أرجع arrayZ + get_اوږدتیا (); }

  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, بینښه صحيح توکي_len, منطقي clear = ریښتیا)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    c->check_ټاکل (lenP1, توکي_len + 1, HB_SERIALIZE_ERROR_ARRAY_ډکېدنه);
    إذا (unlikely (!c->extend_اندازه (هذا, get_اندازه (), clear))) return_trace (خطأ);
    return_trace (ریښتیا);
  }
  کينډۍ <ډول_نوم Iterator,
	    hb_requires (hb_is_سرچینه_د (Iterator, Type))>
  HB_NODISCARD منطقي serialize (hb_serialize_context_t *c, Iterator توکي)
  {
    TRACE_SERIALIZE (هذا);
    بینښه شمېر = hb_len (توکي);
    إذا (unlikely (!serialize (c, شمېر, خطأ))) return_trace (خطأ);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    لكل (بینښه i = 0; i < شمېر; i++, ++توکي)
      arrayZ[i] = *توکي;
    return_trace (ریښتیا);
  }

  کينډۍ <ډول_نوم ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ریښتیا);
    hb_barrier ();
    بینښه صحيح شمېر = get_اوږدتیا ();
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (خطأ);
    return_trace (ریښتیا);
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
کينډۍ <ډول_نوم Type> کارول Headless_Array16_د = Headless_Array_د<Type, HBUINT16>;

/* An array storing length-1. */
کينډۍ <ډول_نوم Type, ډول_نوم LenType=HBUINT16>
بنية Array_د_M1
{
  HB_DELETE_CREATE_COPY_ټاکل (Array_د_M1);

  ثابت Type& چلوونکی [] (صحيح i_) ثابت
  {
    بینښه صحيح i = (بینښه صحيح) i_;
    إذا (unlikely (i > lenM1)) أرجع Null (Type);
    hb_barrier ();
    أرجع arrayZ[i];
  }
  Type& چلوونکی [] (صحيح i_)
  {
    بینښه صحيح i = (بینښه صحيح) i_;
    إذا (unlikely (i > lenM1)) أرجع Crap (Type);
    hb_barrier ();
    أرجع arrayZ[i];
  }
  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_mul_جمع_کړه_saturate (lenM1 + 1, Type::static_اندازه, lenM1.static_اندازه); }

  کينډۍ <ډول_نوم ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ریښتیا);
    hb_barrier ();
    بینښه صحيح شمېر = lenM1 + 1;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (خطأ);
    return_trace (ریښتیا);
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
کينډۍ <ډول_نوم Type, ډول_نوم LenType>
بنية Sorted_Array_د : Array_د<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { أرجع hb_sorted_array (هذا->arrayZ, هذا->len); }
  hb_sorted_array_t<ثابت Type> as_array () ثابت { أرجع hb_sorted_array (هذا->arrayZ, هذا->len); }

  /* Iterator. */
  تعريفنوع hb_sorted_array_t<ثابت Type>   iter_t;
  تعريفنوع hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () ثابت { أرجع as_array (); }
  writer_t writer ()       { أرجع as_array (); }
  چلوونکی   iter_t () ثابت { أرجع   iter (); }
  چلوونکی writer_t ()       { أرجع writer (); }

  /* Faster range-based for loop. */
  ثابت Type *begin () ثابت { أرجع هذا->arrayZ; }
  ثابت Type *پای_ټکی () ثابت { أرجع هذا->arrayZ + هذا->len; }

  منطقي serialize (hb_serialize_context_t *c, بینښه صحيح توکي_len)
  {
    TRACE_SERIALIZE (هذا);
    منطقي ret = Array_د<Type, LenType>::serialize (c, توکي_len);
    return_trace (ret);
  }
  کينډۍ <ډول_نوم Iterator,
	    hb_requires (hb_is_sorted_سرچینه_د (Iterator, Type))>
  منطقي serialize (hb_serialize_context_t *c, Iterator توکي)
  {
    TRACE_SERIALIZE (هذا);
    منطقي ret = Array_د<Type, LenType>::serialize (c, توکي);
    return_trace (ret);
  }

  Sorted_Array_د* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    Sorted_Array_د* بهر = reinterpret_cast<Sorted_Array_د *> (Array_د<Type, LenType>::copy (c));
    return_trace (بهر);
  }

  کينډۍ <ډول_نوم T>
  Type &bsearch (ثابت T &x, Type &not_وموندل = Crap (Type))
  { أرجع *as_array ().bsearch (x, &not_وموندل); }
  کينډۍ <ډول_نوم T>
  ثابت Type &bsearch (ثابت T &x, ثابت Type &not_وموندل = Null (Type)) ثابت
  { أرجع *as_array ().bsearch (x, &not_وموندل); }
  کينډۍ <ډول_نوم T>
  منطقي bfind (ثابت T &x, بینښه صحيح *i = تش_اشاره,
	      hb_not_وموندل_t not_وموندل = HB_NOT_وموندل_DONT_STORE,
	      بینښه صحيح منزل_store = (بینښه صحيح) -1) ثابت
  { أرجع as_array ().bfind (x, i, not_وموندل, منزل_store); }
};

کينډۍ <ډول_نوم Type> کارول Sorted_Array16_د = Sorted_Array_د<Type, HBUINT16>;
کينډۍ <ډول_نوم Type> کارول Sorted_Array24_د = Sorted_Array_د<Type, HBUINT24>;
کينډۍ <ډول_نوم Type> کارول Sorted_Array32_د = Sorted_Array_د<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

کينډۍ <ډول_نوم LenType=HBUINT16>
بنية Bin_پلټنه_سرلیک
{
  چلوونکی uint32_t () ثابت { أرجع len; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  Bin_پلټنه_سرلیک& چلوونکی = (بینښه صحيح v)
  {
    len = v;
    أكد (len == v);
    ثبت_Selector = hb_max (1u, hb_بټ_storage (v)) - 1;
    پلټنه_Range = 16 * (1u << ثبت_Selector);
    range_ښويدنه = v * 16 > پلټنه_Range
		 ? 16 * v - پلټنه_Range
		 : 0;
    أرجع *هذا;
  }

  محمي:
  LenType	len;
  LenType	پلټنه_Range;
  LenType	ثبت_Selector;
  LenType	range_ښويدنه;

  عام:
  DEFINE_اندازه_STATIC (8);
};

کينډۍ <ډول_نوم Type, ډول_نوم LenType=HBUINT16>
کارول Bin_پلټنه_Array_د = Sorted_Array_د<Type, Bin_پلټنه_سرلیک<LenType>>;


بنية Var_Sized_Bin_پلټنه_سرلیک
{

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16	unit_اندازه;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	پلټنه_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	ثبت_Selector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	range_ښويدنه;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  عام:
  DEFINE_اندازه_STATIC (10);
};

کينډۍ <ډول_نوم Type>
بنية Var_Sized_Bin_پلټنه_Array_د
{
  ساكن constexpr بینښه توکی_اندازه = Type::static_اندازه;

  HB_DELETE_CREATE_COPY_ټاکل (Var_Sized_Bin_پلټنه_Array_د);

  منطقي وروستی_is_terminator () ثابت
  {
    إذا (unlikely (!سرلیک.nUnits)) أرجع خطأ;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    ثابت HBUINT16 *ټکي = &Struct_At_دننه_واټن<HBUINT16> (&بایټونه_Z, (سرلیک.nUnits - 1) * سرلیک.unit_اندازه);
    بینښه صحيح شمېر = Type::Termination_کلمه_شمېر;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (ټکي[i] != 0xFFFFu)
	أرجع خطأ;
    أرجع ریښتیا;
  }

  ثابت Type& چلوونکی [] (صحيح i_) ثابت
  {
    بینښه صحيح i = (بینښه صحيح) i_;
    إذا (unlikely (i >= get_اوږدتیا ())) أرجع Null (Type);
    hb_barrier ();
    أرجع Struct_At_دننه_واټن<Type> (&بایټونه_Z, i * سرلیک.unit_اندازه);
  }
  Type& چلوونکی [] (صحيح i_)
  {
    بینښه صحيح i = (بینښه صحيح) i_;
    إذا (unlikely (i >= get_اوږدتیا ())) أرجع Crap (Type);
    hb_barrier ();
    أرجع Struct_At_دننه_واټن<Type> (&بایټونه_Z, i * سرلیک.unit_اندازه);
  }
  بینښه صحيح get_اوږدتیا () ثابت
  { أرجع سرلیک.nUnits - وروستی_is_terminator (); }
  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_mul_جمع_کړه_saturate (سرلیک.nUnits, سرلیک.unit_اندازه, سرلیک.static_اندازه); }

  کينډۍ <ډول_نوم ...Ts>
  HB_ALWAYS_INLINE
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!sanitize_shallow (c))) return_trace (خطأ);
    إذا (!حجم... (Ts) && hb_is_trivially_copyable(Type)) return_trace (ریښتیا);
    hb_barrier ();
    بینښه صحيح شمېر = get_اوږدتیا ();
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (unlikely (!(*هذا)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (خطأ);
    return_trace (ریښتیا);
  }

  کينډۍ <ډول_نوم T>
  ثابت Type *bsearch (ثابت T &key) ثابت
  {
    بینښه pos;
    أرجع hb_bsearch_impl (&pos,
			    key,
			    (ثابت فراغ *) بایټونه_Z,
			    get_اوږدتیا (),
			    سرلیک.unit_اندازه,
			    hb_cmp_لاره<T, Type>)
	   ? (ثابت Type *) (((ثابت حرف *) &بایټونه_Z) + (pos * سرلیک.unit_اندازه))
	   : تش_اشاره;
  }

  خاص:
  منطقي sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (سرلیک.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_اندازه <= سرلیک.unit_اندازه &&
		  c->check_range (بایټونه_Z.arrayZ,
				  سرلیک.nUnits,
				  سرلیک.unit_اندازه));
  }

  محمي:
  Var_Sized_Bin_پلټنه_سرلیک	سرلیک;
  Unsized_Array_د<HBUINT8>	بایټونه_Z;
  عام:
  DEFINE_اندازه_ARRAY (10, بایټونه_Z);
};


/* CFF INDEX */

کينډۍ <ډول_نوم شمېر_3>
بنية CFFIndex
{
  بینښه صحيح دننه_واټن_array_اندازه () ثابت
  { أرجع off_اندازه * (شمېر + 1); }

  کينډۍ <ډول_نوم Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Iterable &iterable,
		  ثابت بینښه *p_معلومات_اندازه = تش_اشاره,
                  بینښه min_off_اندازه = 0)
  {
    TRACE_SERIALIZE (هذا);
    بینښه معلومات_اندازه;
    إذا (p_معلومات_اندازه)
      معلومات_اندازه = *p_معلومات_اندازه;
    وإلا
      ټول_اندازه (iterable, &معلومات_اندازه);

    خپلکاری it = hb_iter (iterable);
    إذا (unlikely (!serialize_سرلیک (c, +it, معلومات_اندازه, min_off_اندازه))) return_trace (خطأ);
    بینښه حرف *ret = c->allocate_اندازه<بینښه حرف> (معلومات_اندازه, خطأ);
    إذا (unlikely (!ret)) return_trace (خطأ);
    بینښه پاتې_شوی = معلومات_اندازه;
    لكل (ثابت خپلکاری &_ : +it)
    {
      بینښه len = _.اوږدتیا;

      إذا (!len)
	استمر;

      إذا (unlikely (len > پاتې_شوی)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (خطأ, HB_SERIALIZE_ERROR_INT_ډکېدنه));
      }

      پاتې_شوی -= len;

      إذا (len <= 1)
      {
	*ret++ = *_.arrayZ;
	استمر;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (ریښتیا);
  }

  کينډۍ <ډول_نوم Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_سرلیک (hb_serialize_context_t *c,
			 Iterator it,
			 بینښه معلومات_اندازه,
                         بینښه min_off_اندازه = 0)
  {
    TRACE_SERIALIZE (هذا);

    بینښه off_اندازه_2 = (hb_بټ_storage (معلومات_اندازه + 1) + 7) / 8;
    off_اندازه_2 = hb_max(min_off_اندازه, off_اندازه_2);

    /* serialize CFFIndex header */
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    هذا->شمېر = hb_len (it);
    إذا (!هذا->شمېر) return_trace (ریښتیا);
    إذا (unlikely (!c->extend (هذا->off_اندازه))) return_trace (خطأ);
    هذا->off_اندازه = off_اندازه_2;
    إذا (unlikely (!c->allocate_اندازه<HBUINT8> (off_اندازه_2 * (هذا->شمېر + 1), خطأ)))
      return_trace (خطأ);

    /* serialize indices */
    بینښه صحيح دننه_واټن = 1;
    إذا (HB_OPTIMIZE_اندازه_VAL)
    {
      بینښه صحيح i = 0;
      لكل (ثابت خپلکاری &_ : +it)
      {
	سېټ_دننه_واټن_at (i++, دننه_واټن);
	دننه_واټن += hb_len_د (_);
      }
      سېټ_دننه_واټن_at (i, دننه_واټن);
    }
    وإلا
      بدل (off_اندازه_2)
      {
	حالة 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  لكل (ثابت خپلکاری &_ : +it)
	  {
	    *p++ = دننه_واټن;
	    دننه_واټن += hb_len_د (_);
	  }
	  *p = دننه_واټن;
	}
	اكسر;
	حالة 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  لكل (ثابت خپلکاری &_ : +it)
	  {
	    *p++ = دننه_واټن;
	    دننه_واټن += hb_len_د (_);
	  }
	  *p = دننه_واټن;
	}
	اكسر;
	حالة 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  لكل (ثابت خپلکاری &_ : +it)
	  {
	    *p++ = دننه_واټن;
	    دننه_واټن += hb_len_د (_);
	  }
	  *p = دننه_واټن;
	}
	اكسر;
	حالة 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  لكل (ثابت خپلکاری &_ : +it)
	  {
	    *p++ = دننه_واټن;
	    دننه_واټن += hb_len_د (_);
	  }
	  *p = دننه_واټن;
	}
	اكسر;
	افتراضي:
	اكسر;
      }

    أكد (دننه_واټن == معلومات_اندازه + 1);
    return_trace (ریښتیا);
  }

  کينډۍ <ډول_نوم Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  ساكن بینښه ټول_اندازه (ثابت Iterable &iterable, بینښه *معلومات_اندازه = تش_اشاره, بینښه min_off_اندازه = 0)
  {
    خپلکاری it = + hb_iter (iterable);
    إذا (!it)
    {
      إذا (معلومات_اندازه) *معلومات_اندازه = 0;
      أرجع min_اندازه;
    }

    بینښه ټول = 0;
    لكل (ثابت خپلکاری &_ : +it)
      ټول += hb_len_د (_);

    إذا (معلومات_اندازه) *معلومات_اندازه = ټول;

    بینښه off_اندازه_2 = (hb_بټ_storage (ټول + 1) + 7) / 8;
    off_اندازه_2 = hb_max(min_off_اندازه, off_اندازه_2);

    أرجع min_اندازه + HBUINT8::static_اندازه + (hb_len (it) + 1) * off_اندازه_2 + ټول;
  }

  فراغ سېټ_دننه_واټن_at (بینښه صحيح شاخص_2, بینښه صحيح دننه_واټن)
  {
    أكد (شاخص_2 <= شمېر);

    بینښه صحيح اندازه = off_اندازه;
    ثابت HBUINT8 *p = offsets;
    بدل (اندازه)
    {
      حالة 1: ((HBUINT8  *) p)[شاخص_2] = دننه_واټن; اكسر;
      حالة 2: ((HBUINT16 *) p)[شاخص_2] = دننه_واټن; اكسر;
      حالة 3: ((HBUINT24 *) p)[شاخص_2] = دننه_واټن; اكسر;
      حالة 4: ((HBUINT32 *) p)[شاخص_2] = دننه_واټن; اكسر;
      افتراضي: أرجع;
    }
  }

  خاص:
  بینښه صحيح دننه_واټن_at (بینښه صحيح شاخص_2) ثابت
  {
    أكد (شاخص_2 <= شمېر);

    بینښه صحيح اندازه = off_اندازه;
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

  ثابت بینښه حرف *معلومات_بنسټ () ثابت
  { أرجع (ثابت بینښه حرف *) هذا + min_اندازه + off_اندازه.static_اندازه - 1 + دننه_واټن_array_اندازه (); }
  عام:

  hb_ubytes_t چلوونکی [] (بینښه صحيح شاخص_2) ثابت
  {
    إذا (unlikely (شاخص_2 >= شمېر)) أرجع hb_ubytes_t ();
    hb_barrier ();
    بینښه offset0 = دننه_واټن_at (شاخص_2);
    بینښه offset1 = دننه_واټن_at (شاخص_2 + 1);
    إذا (unlikely (offset1 < offset0 || offset1 > دننه_واټن_at (شمېر)))
      أرجع hb_ubytes_t ();
    أرجع hb_ubytes_t (معلومات_بنسټ () + offset0, offset1 - offset0);
  }

  size_t get_اندازه () ثابت
  {
    إذا (شمېر)
      أرجع hb_unsigned_جمع_کړه_saturate (min_اندازه, off_اندازه.static_اندازه,
				       دننه_واټن_array_اندازه (), دننه_واټن_at (شمېر) - 1);
    أرجع min_اندازه;  /* empty CFFIndex contains count only */
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  (شمېر == 0 || /* empty INDEX */
			   (شمېر < شمېر + 1u &&
			    c->check_struct (&off_اندازه) && off_اندازه >= 1 && off_اندازه <= 4 &&
			    c->check_array (offsets, off_اندازه, شمېر + 1u) &&
			    c->check_range (معلومات_بنسټ (), دننه_واټن_at (شمېر))))));
  }

  عام:
  شمېر_3		شمېر;		/* Number of object data. Note there are (count+1) offsets */
  خاص:
  HBUINT8	off_اندازه;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  عام:
  DEFINE_اندازه_MIN (شمېر_3::static_اندازه);
};
تعريفنوع CFFIndex<HBUINT16> CFF1_شاخص;
تعريفنوع CFFIndex<HBUINT32> CFF2_شاخص;


/* TupleValues */
بنية Tuple_ارزښتونه
{
  تعداد packed_ارزښت_flag_t
  {
    ارزښتونه_ARE_ZEROS     = 0x80,
    ارزښتونه_ARE_بایټونه     = 0x00,
    ارزښتونه_ARE_ټکي     = 0x40,
    ارزښتونه_ARE_LONGS     = 0xC0,
    ارزښتونه_اندازه_ماسک     = 0xC0,
    ارزښت_پرلپسې_شمېر_ماسک = 0x3F
  };

  ساكن بینښه compile_unsafe (hb_array_t<ثابت صحيح> values, /*دننه*/
				  بینښه حرف *encoded_بایټونه /*بهر*/)
  {
    بینښه عدد_ارزښتونه_3 = values.اوږدتیا;
    بینښه encoded_len = 0;
    بینښه i = 0;
    طالما (i < عدد_ارزښتونه_3)
    {
      صحيح val = values.arrayZ[i];
      إذا (val == 0)
        encoded_len += encode_ارزښت_پرلپسې_as_zeroes (i, encoded_بایټونه + encoded_len, values);
      وإلا إذا ((int8_t) val == val)
        encoded_len += encode_ارزښت_پرلپسې_as_بایټونه (i, encoded_بایټونه + encoded_len, values);
      وإلا إذا ((int16_t) val == val)
        encoded_len += encode_ارزښت_پرلپسې_as_ټکي (i, encoded_بایټونه + encoded_len, values);
      وإلا
        encoded_len += encode_ارزښت_پرلپسې_as_longs (i, encoded_بایټونه + encoded_len, values);
    }
    أرجع encoded_len;
  }

  ساكن بینښه encode_ارزښت_پرلپسې_as_zeroes (بینښه& i,
					      بینښه حرف *it,
					      hb_array_t<ثابت صحيح> values)
  {
    بینښه عدد_ارزښتونه_3 = values.اوږدتیا;
    بینښه پرلپسې_اوږدتیا = 0;
    بینښه encoded_len = 0;
    طالما (i < عدد_ارزښتونه_3 && values.arrayZ[i] == 0)
    {
      i++;
      پرلپسې_اوږدتیا++;
    }

    طالما (پرلپسې_اوږدتیا >= 64)
    {
      *it++ = حرف (ارزښتونه_ARE_ZEROS | 63);
      پرلپسې_اوږدتیا -= 64;
      encoded_len++;
    }

    إذا (پرلپسې_اوږدتیا)
    {
      *it++ = حرف (ارزښتونه_ARE_ZEROS | (پرلپسې_اوږدتیا - 1));
      encoded_len++;
    }
    أرجع encoded_len;
  }

  ساكن بینښه encode_ارزښت_پرلپسې_as_بایټونه (بینښه &i,
					     بینښه حرف *it,
					     hb_array_t<ثابت صحيح> values)
  {
    بینښه پیل = i;
    بینښه عدد_ارزښتونه_3 = values.اوږدتیا;
    طالما (i < عدد_ارزښتونه_3)
    {
      صحيح val = values.arrayZ[i];
      إذا ((int8_t) val != val)
        اكسر;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      إذا (val == 0 && i + 1 < عدد_ارزښتونه_3 && values.arrayZ[i+1] == 0)
        اكسر;

      i++;
    }
    بینښه پرلپسې_اوږدتیا = i - پیل;

    بینښه encoded_len = 0;

    طالما (پرلپسې_اوږدتیا >= 64)
    {
      *it++ = (ارزښتونه_ARE_بایټونه | 63);
      encoded_len++;

      لكل (بینښه j = 0; j < 64; j++)
	it[j] = static_cast<حرف> (values.arrayZ[پیل + j]);
      it += 64;
      encoded_len += 64;

      پیل += 64;
      پرلپسې_اوږدتیا -= 64;
    }

    إذا (پرلپسې_اوږدتیا)
    {
      *it++ = (ارزښتونه_ARE_بایټونه | (پرلپسې_اوږدتیا - 1));
      encoded_len++;

      لكل (بینښه j = 0; j < پرلپسې_اوږدتیا; j++)
        it[j] = static_cast<حرف> (values.arrayZ[پیل + j]);
      encoded_len += پرلپسې_اوږدتیا;
    }

    أرجع encoded_len;
  }

  ساكن بینښه encode_ارزښت_پرلپسې_as_ټکي (بینښه &i,
					     بینښه حرف *it,
					     hb_array_t<ثابت صحيح> values)
  {
    بینښه پیل = i;
    بینښه عدد_ارزښتونه_3 = values.اوږدتیا;
    طالما (i < عدد_ارزښتونه_3)
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
          i + 1 < عدد_ارزښتونه_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        اكسر;

      i++;
    }

    بینښه پرلپسې_اوږدتیا = i - پیل;
    بینښه encoded_len = 0;
    طالما (پرلپسې_اوږدتیا >= 64)
    {
      *it++ = (ارزښتونه_ARE_ټکي | 63);
      encoded_len++;

      لكل (بینښه j = 0; j < 64; j++)
      {
        int16_t ارزښت_val = values.arrayZ[پیل + j];
        *it++ = static_cast<حرف> (ارزښت_val >> 8);
        *it++ = static_cast<حرف> (ارزښت_val & 0xFF);

        encoded_len += 2;
      }

      پیل += 64;
      پرلپسې_اوږدتیا -= 64;
    }

    إذا (پرلپسې_اوږدتیا)
    {
      *it++ = (ارزښتونه_ARE_ټکي | (پرلپسې_اوږدتیا - 1));
      encoded_len++;
      طالما (پیل < i)
      {
        int16_t ارزښت_val = values.arrayZ[پیل++];
        *it++ = static_cast<حرف> (ارزښت_val >> 8);
        *it++ = static_cast<حرف> (ارزښت_val & 0xFF);

        encoded_len += 2;
      }
    }
    أرجع encoded_len;
  }

  ساكن بینښه encode_ارزښت_پرلپسې_as_longs (بینښه &i,
					     بینښه حرف *it,
					     hb_array_t<ثابت صحيح> values)
  {
    بینښه پیل = i;
    بینښه عدد_ارزښتونه_3 = values.اوږدتیا;
    طالما (i < عدد_ارزښتونه_3)
    {
      صحيح val = values.arrayZ[i];

      إذا ((int16_t) val == val)
        اكسر;

      i++;
    }

    بینښه پرلپسې_اوږدتیا = i - پیل;
    بینښه encoded_len = 0;
    طالما (پرلپسې_اوږدتیا >= 64)
    {
      *it++ = (ارزښتونه_ARE_LONGS | 63);
      encoded_len++;

      لكل (بینښه j = 0; j < 64; j++)
      {
        int32_t ارزښت_val = values.arrayZ[پیل + j];
        *it++ = static_cast<حرف> (ارزښت_val >> 24);
        *it++ = static_cast<حرف> (ارزښت_val >> 16);
        *it++ = static_cast<حرف> (ارزښت_val >> 8);
        *it++ = static_cast<حرف> (ارزښت_val & 0xFF);

        encoded_len += 4;
      }

      پیل += 64;
      پرلپسې_اوږدتیا -= 64;
    }

    إذا (پرلپسې_اوږدتیا)
    {
      *it++ = (ارزښتونه_ARE_LONGS | (پرلپسې_اوږدتیا - 1));
      encoded_len++;
      طالما (پیل < i)
      {
        int32_t ارزښت_val = values.arrayZ[پیل++];
        *it++ = static_cast<حرف> (ارزښت_val >> 24);
        *it++ = static_cast<حرف> (ارزښت_val >> 16);
        *it++ = static_cast<حرف> (ارزښت_val >> 8);
        *it++ = static_cast<حرف> (ارزښت_val & 0xFF);

        encoded_len += 4;
      }
    }
    أرجع encoded_len;
  }

  کينډۍ <ډول_نوم T>
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  ساكن منطقي decompile (ثابت HBUINT8 *&p /* IN/OUT */,
			 hb_ویکټور_t<T> &values /* IN/OUT */,
			 ثابت HBUINT8 *پای_ټکی,
			 منطقي consume_all = خطأ,
			 بینښه پیل = 0)
  {
    بینښه i = 0;
    بینښه شمېر = consume_all ? UINT_MAX : values.اوږدتیا;
    إذا (consume_all)
      values.alloc ((پای_ټکی - p) / 2);
    طالما (i < شمېر)
    {
      إذا (unlikely (p + 1 > پای_ټکی)) أرجع consume_all;
      بینښه control = *p++;
      بینښه پرلپسې_شمېر = (control & ارزښت_پرلپسې_شمېر_ماسک) + 1;
      إذا (consume_all)
      {
        إذا (unlikely (!values.resize_dirty  (values.اوږدتیا + پرلپسې_شمېر)))
	  أرجع خطأ;
      }
      بینښه stop = i + پرلپسې_شمېر;
      إذا (unlikely (stop > شمېر)) أرجع خطأ;

      بینښه skip = i < پیل ? hb_min (پیل - i, پرلپسې_شمېر) : 0;
      i += skip;

      إذا ((control & ارزښتونه_اندازه_ماسک) == ارزښتونه_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * حجم (T));
	i = stop;
      }
      وإلا إذا ((control & ارزښتونه_اندازه_ماسک) ==  ارزښتونه_ARE_ټکي)
      {
        إذا (unlikely (p + پرلپسې_شمېر * HBINT16::static_اندازه > پای_ټکی)) أرجع خطأ;
	p += skip * HBINT16::static_اندازه;
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
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
#د_که_پای
        لكل (; i < stop; i++)
        {
          values.arrayZ[i] = * (ثابت HBINT16 *) p;
          p += HBINT16::static_اندازه;
        }
      }
      وإلا إذا ((control & ارزښتونه_اندازه_ماسک) ==  ارزښتونه_ARE_LONGS)
      {
        إذا (unlikely (p + پرلپسې_شمېر * HBINT32::static_اندازه > پای_ټکی)) أرجع خطأ;
	p += skip * HBINT32::static_اندازه;
        لكل (; i < stop; i++)
        {
          values.arrayZ[i] = * (ثابت HBINT32 *) p;
          p += HBINT32::static_اندازه;
        }
      }
      وإلا إذا ((control & ارزښتونه_اندازه_ماسک) ==  ارزښتونه_ARE_بایټونه)
      {
        إذا (unlikely (p + پرلپسې_شمېر > پای_ټکی)) أرجع خطأ;
	p += skip * HBINT8::static_اندازه;
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
	لكل (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (ثابت HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (ثابت HBINT8 *) p++;
	}
#د_که_پای
        لكل (; i < stop; i++)
          values.arrayZ[i] = * (ثابت HBINT8 *) p++;
      }
    }
    أرجع ریښتیا;
  }

  ساكن منطقي skip (ثابت بینښه حرف *&p /* IN/OUT */,
		    ثابت بینښه حرف *پای_ټکی,
		    بینښه شمېر)
  {
    بینښه کتل_شوی = 0;
    طالما (کتل_شوی < شمېر)
    {
      إذا (unlikely (p >= پای_ټکی)) أرجع خطأ;
      بینښه control = *p++;
      بینښه پرلپسې_شمېر = (control & ارزښت_پرلپسې_شمېر_ماسک) + 1;
      إذا (unlikely (پرلپسې_شمېر > شمېر - کتل_شوی)) أرجع خطأ;

      بینښه پلنوالی = 0;
      بدل (control & ارزښتونه_اندازه_ماسک)
      {
	حالة ارزښتونه_ARE_ZEROS: اكسر;
	حالة ارزښتونه_ARE_بایټونه: پلنوالی = HBINT8::static_اندازه; اكسر;
	حالة ارزښتونه_ARE_ټکي: پلنوالی = HBINT16::static_اندازه; اكسر;
	حالة ارزښتونه_ARE_LONGS: پلنوالی = HBINT32::static_اندازه; اكسر;
      }
      إذا (unlikely (بینښه (پای_ټکی - p) < پرلپسې_شمېر * پلنوالی)) أرجع خطأ;
      p += پرلپسې_شمېر * پلنوالی;
      کتل_شوی += پرلپسې_شمېر;
    }
    أرجع ریښتیا;
  }

  بنية iter_t : hb_iter_with_fallback_t<iter_t, صحيح>
  {
    iter_t (ثابت بینښه حرف *p_, بینښه len_)
	    : p (p_), endp (p_ + len_)
    { إذا (likely (ensure_پرلپسې ())) read_ارزښت (); }

    خاص:
    ثابت بینښه حرف *p;
    ثابت بینښه حرف * ثابت endp;
    صحيح current_ارزښت = 0;
    نښهدار پرلپسې_شمېر = 0;
    بینښه پلنوالی = 0;

    HB_ALWAYS_INLINE
    منطقي ensure_پرلپسې ()
    {
      إذا (likely (پرلپسې_شمېر > 0)) أرجع ریښتیا;
      أرجع ensure_پرلپسې_2 ();
    }
    منطقي ensure_پرلپسې_2 ()
    {
      إذا (unlikely (p >= endp))
      {
        پرلپسې_شمېر = 0;
        current_ارزښت = 0;
	أرجع خطأ;
      }

      بینښه control = *p++;
      پرلپسې_شمېر = (control & ارزښت_پرلپسې_شمېر_ماسک) + 1;
      پلنوالی = control & ارزښتونه_اندازه_ماسک;
      بدل (پلنوالی)
      {
        حالة ارزښتونه_ARE_ZEROS: پلنوالی = 0; اكسر;
	حالة ارزښتونه_ARE_بایټونه: پلنوالی = HBINT8::static_اندازه;  اكسر;
	حالة ارزښتونه_ARE_ټکي: پلنوالی = HBINT16::static_اندازه; اكسر;
	حالة ارزښتونه_ARE_LONGS: پلنوالی = HBINT32::static_اندازه; اكسر;
	افتراضي: أكد (خطأ);
      }

      إذا (unlikely (p + پرلپسې_شمېر * پلنوالی > endp))
      {
	پرلپسې_شمېر = 0;
	current_ارزښت = 0;
	أرجع خطأ;
      }

      أرجع ریښتیا;
    }
    فراغ read_ارزښت ()
    {
      بدل (پلنوالی)
      {
        حالة 0: current_ارزښت = 0; اكسر;
	حالة 1: current_ارزښت = * (ثابت HBINT8  *) p; اكسر;
	حالة 2: current_ارزښت = * (ثابت HBINT16 *) p; اكسر;
	حالة 4: current_ارزښت = * (ثابت HBINT32 *) p; اكسر;
      }
      p += پلنوالی;
    }

    عام:

    تعريفنوع صحيح __item_t__;
    __item_t__ __item__ () ثابت
    { أرجع current_ارزښت; }

    منطقي __more__ () ثابت { أرجع پرلپسې_شمېر || p < endp; }
    فراغ __next__ ()
    {
      پرلپسې_شمېر--;
      إذا (unlikely (!ensure_پرلپسې ()))
	أرجع;
      read_ارزښت ();
    }
    فراغ __forward__ (بینښه n)
    {
      إذا (unlikely (!ensure_پرلپسې ()))
	أرجع;
      طالما (n)
      {
	بینښه i = hb_min (n, (بینښه) پرلپسې_شمېر);
	پرلپسې_شمېر -= i;
	n -= i;
	p += (i - 1) * پلنوالی;
	إذا (unlikely (!ensure_پرلپسې ()))
	  أرجع;
	read_ارزښت ();
      }
    }
    منطقي چلوونکی != (ثابت iter_t& o) ثابت
    { أرجع p != o.p || پرلپسې_شمېر != o.پرلپسې_شمېر; }
    iter_t __end__ () ثابت
    {
      iter_t it (endp, 0);
      أرجع it;
    }
  };

  بنية fetcher_t
  {
    fetcher_t (ثابت بینښه حرف *p_, بینښه len_)
	      : p (p_), پای_ټکی (p_ + len_) {}

    خاص:
    ثابت بینښه حرف *p;
    ثابت بینښه حرف * ثابت پای_ټکی;
    نښهدار پرلپسې_شمېر = 0;
    بینښه پلنوالی = 0;

    HB_ALWAYS_INLINE
    منطقي ensure_پرلپسې ()
    {
      إذا (likely (پرلپسې_شمېر > 0)) أرجع ریښتیا;
      أرجع ensure_پرلپسې_2 ();
    }

    منطقي ensure_پرلپسې_2 ()
    {
      إذا (unlikely (p >= پای_ټکی))
      {
        پرلپسې_شمېر = 0;
	أرجع خطأ;
      }

      بینښه control = *p++;
      پرلپسې_شمېر = (control & ارزښت_پرلپسې_شمېر_ماسک) + 1;
      پلنوالی = control & ارزښتونه_اندازه_ماسک;
      بدل (پلنوالی)
      {
        حالة ارزښتونه_ARE_ZEROS: پلنوالی = 0; اكسر;
	حالة ارزښتونه_ARE_بایټونه: پلنوالی = HBINT8::static_اندازه;  اكسر;
	حالة ارزښتونه_ARE_ټکي: پلنوالی = HBINT16::static_اندازه; اكسر;
	حالة ارزښتونه_ARE_LONGS: پلنوالی = HBINT32::static_اندازه; اكسر;
	افتراضي: أكد (خطأ);
      }

      إذا (unlikely (p + پرلپسې_شمېر * پلنوالی > پای_ټکی))
      {
	پرلپسې_شمېر = 0;
	أرجع خطأ;
      }

      أرجع ریښتیا;
    }

    عام:
    فراغ skip (بینښه n)
    {
      طالما (n)
      {
	إذا (unlikely (!ensure_پرلپسې ()))
	  أرجع;
	بینښه i = hb_min (n, (بینښه) پرلپسې_شمېر);
	پرلپسې_شمېر -= i;
	n -= i;
	p += i * پلنوالی;
      }
    }

    خاص:
    کينډۍ <منطقي scaled, ډول_نوم T>
    فراغ جمع_کړه_منزل (hb_array_t<T> بهر, T scale = T (1))
    {
      بینښه n = بهر.اوږدتیا;
      T *arrayZ = بهر.arrayZ;

      لكل (بینښه i = 0; i < n;)
      {
	إذا (unlikely (!ensure_پرلپسې ()))
	  اكسر;
	بینښه شمېر = hb_min (n - i, (بینښه) پرلپسې_شمېر);
	بدل (پلنوالی)
	{
	  حالة 0:
	  {
	    arrayZ += شمېر;
	    اكسر;
	  }
	  حالة 1:
	  {
	    ثابت خپلکاری *pp = (ثابت HBINT8 *) p;
	    بینښه j = 0;
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
	    لكل (; j + 3 < شمېر; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#د_که_پای
	    لكل (; j < شمېر; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت بینښه حرف *) pp;
	  }
	  اكسر;
	  حالة 2:
	  {
	    ثابت خپلکاری *pp = (ثابت HBINT16 *) p;
	    بینښه j = 0;
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
	    لكل (; j + 3 < شمېر; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#د_که_پای
	    لكل (; j < شمېر; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت بینښه حرف *) pp;
	  }
	  اكسر;
	  حالة 4:
	  {
	    ثابت خپلکاری *pp = (ثابت HBINT32 *) p;
	    لكل (بینښه j = 0; j < شمېر; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (ثابت بینښه حرف *) pp;
	  }
	  اكسر;
	}
	پرلپسې_شمېر -= شمېر;
	i += شمېر;
      }
    }

    عام:
    کينډۍ <ډول_نوم T>
    فراغ جمع_کړه_منزل_2 (hb_array_t<T> بهر, عائم scale = 1.0f)
    {
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      إذا (خطأ && scale == 1.0f)
        جمع_کړه_منزل<خطأ> (بهر);
      وإلا
#د_که_پای
        جمع_کړه_منزل<ریښتیا> (بهر, (T) scale);
    }
  };
};

بنية TupleList : CFF2_شاخص
{
  Tuple_ارزښتونه::iter_t چلوونکی [] (بینښه i) ثابت
  {
    خپلکاری بایټونه = CFF2_شاخص::چلوونکی [] (i);
    أرجع Tuple_ارزښتونه::iter_t (بایټونه.arrayZ, بایټونه.اوږدتیا);
  }

  Tuple_ارزښتونه::fetcher_t fetcher (بینښه i) ثابت
  {
    خپلکاری بایټونه = CFF2_شاخص::چلوونکی [] (i);
    أرجع Tuple_ارزښتونه::fetcher_t (بایټونه.arrayZ, بایټونه.اوږدتیا);
  }
};


// Alignment

کينډۍ <بینښه صحيح alignment>
بنية Align
{
  size_t get_اندازه (ثابت فراغ *بنسټ) ثابت
  {
    بینښه دننه_واټن = (ثابت حرف *) هذا - (ثابت حرف *) بنسټ;
    أرجع (alignment - دننه_واټن) & (alignment - 1);
  }

  عام:
  DEFINE_اندازه_MIN (0);
};



} /* namespace OT */


#د_که_پای /* HB_OPEN_TYPE_HH */
