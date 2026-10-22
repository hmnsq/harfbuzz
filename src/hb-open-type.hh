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

#నిర్వచితం_కాకపోతే HB_OPEN_TYPE_HH
#నిర్వచించు HB_OPEN_TYPE_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-blob.hh"
#అన్తర్భావ "hb-face.hh"
#అన్తర్భావ "hb-machinery.hh"
#అన్తర్భావ "hb-meta.hh"
#అన్తర్భావ "hb-subset.hh"


నామాకాశ OT {


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
మూస <తార్కిక BE,
	  రకం_పేరు Type,
	  గుర్తులేని పూర్ణాంకం కొలత_2 = పరిమాణం (Type)>
నిర్మాణం అంకె_Type
{
  రకనిర్వచనం Type type;
  /* For reason we define cast out operator for signed/unsigned, instead of Type, see:
   * https://github.com/harfbuzz/harfbuzz/pull/2875/commits/09836013995cab2b9f07577a179ad7b024130467 */
  రకనిర్వచనం రకం_పేరు std::conditional<std::is_integral<Type>::value && పరిమాణం (Type) <= పరిమాణం(పూర్ణాంకం),
				     రకం_పేరు std::conditional<std::is_signed<Type>::value, గుర్తుగల, గుర్తులేని>::type,
				     Type>::type WideType;

  అంకె_Type () = అప్రమేయ;
  స్పష్టమైన constexpr అంకె_Type (Type V) : v {V} {}
  అంకె_Type& క్రియాకారకం = (Type V) { v = V; తిరిగి *ఇది; }

  క్రియాకారకం WideType () స్థిరరాశి { తిరిగి v; }

  తార్కిక క్రియాకారకం == (స్థిరరాశి అంకె_Type &o) స్థిరరాశి { తిరిగి (Type) v == (Type) o.v; }
  తార్కిక క్రియాకారకం != (స్థిరరాశి అంకె_Type &o) స్థిరరాశి { తిరిగి !(*ఇది == o); }

  అంకె_Type& క్రియాకారకం += (WideType లెక్క) { *ఇది = *ఇది + లెక్క; తిరిగి *ఇది; }
  అంకె_Type& క్రియాకారకం -= (WideType లెక్క) { *ఇది = *ఇది - లెక్క; తిరిగి *ఇది; }
  అంకె_Type& క్రియాకారకం ++ () { *ఇది += 1; తిరిగి *ఇది; }
  అంకె_Type& క్రియాకారకం -- () { *ఇది -= 1; తిరిగి *ఇది; }
  అంకె_Type క్రియాకారకం ++ (పూర్ణాంకం) { అంకె_Type c (*ఇది); ++*ఇది; తిరిగి c; }
  అంకె_Type క్రియాకారకం -- (పూర్ణాంకం) { అంకె_Type c (*ఇది); --*ఇది; తిరిగి c; }

  uint32_t hash () స్థిరరాశి { తిరిగి hb_array ((స్థిరరాశి అక్షరం *) &v, పరిమాణం (v)).hash (); }
  HB_అంతర్గత స్థిర పూర్ణాంకం cmp (స్థిరరాశి అంకె_Type *a, స్థిరరాశి అంకె_Type *b)
  { తిరిగి b->cmp (*a); }
  HB_అంతర్గత స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
  {
    అంకె_Type *pa = (అంకె_Type *) a;
    అంకె_Type *pb = (అంకె_Type *) b;

    తిరిగి pb->cmp (*pa);
  }
  మూస <రకం_పేరు Type2,
	    hb_enable_if (hb_is_convertible (Type2, Type))>
  పూర్ణాంకం cmp (Type2 a) స్థిరరాశి
  {
    Type b = v;
    తిరిగి (a > b) - (a < b);
  }
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }
  రక్షిత:
  రకం_పేరు std::conditional<std::is_integral<Type>::value,
			    HBInt<BE, Type, కొలత_2>,
			    HBFloat<BE, Type, కొలత_2>>::type v;
  బహిరంగ:
  DEFINE_కొలత_STATIC (కొలత_2);
};

రకనిర్వచనం అంకె_Type<నిజం, uint8_t>  HBUINT8;	/* 8-bit big-endian unsigned integer. */
రకనిర్వచనం అంకె_Type<నిజం, int8_t>   HBINT8;		/* 8-bit big-endian signed integer. */
రకనిర్వచనం అంకె_Type<నిజం, uint16_t> HBUINT16;	/* 16-bit big-endian unsigned integer. */
రకనిర్వచనం అంకె_Type<నిజం, int16_t>  HBINT16;	/* 16-bit big-endian signed integer. */
రకనిర్వచనం అంకె_Type<నిజం, uint32_t> HBUINT32;	/* 32-bit big-endian unsigned integer. */
రకనిర్వచనం అంకె_Type<నిజం, int32_t>  HBINT32;	/* 32-bit big-endian signed integer. */
రకనిర్వచనం అంకె_Type<నిజం, uint64_t> HBUINT64;	/* 64-bit big-endian unsigned integer. */
రకనిర్వచనం అంకె_Type<నిజం, int64_t>  HBINT64;	/* 64-bit big-endian signed integer. */
/* Note: we cannot defined a signed HBINT24 because there's no corresponding C type.
 * Works for unsigned, but not signed, since we rely on compiler for sign-extension. */
రకనిర్వచనం అంకె_Type<నిజం, uint32_t, 3> HBUINT24;	/* 24-bit big-endian unsigned integer. */

రకనిర్వచనం అంకె_Type<అబద్ధం, uint16_t> HBUINT16LE;	/* 16-bit little-endian unsigned integer. */
రకనిర్వచనం అంకె_Type<అబద్ధం, int16_t>  HBINT16LE;	/* 16-bit little-endian signed integer. */
రకనిర్వచనం అంకె_Type<అబద్ధం, uint32_t> HBUINT32LE;	/* 32-bit little-endian unsigned integer. */
రకనిర్వచనం అంకె_Type<అబద్ధం, int32_t>  HBINT32LE;	/* 32-bit little-endian signed integer. */
రకనిర్వచనం అంకె_Type<అబద్ధం, uint64_t> HBUINT64LE;	/* 64-bit little-endian unsigned integer. */
రకనిర్వచనం అంకె_Type<అబద్ధం, int64_t>  HBINT64LE;	/* 64-bit little-endian signed integer. */

రకనిర్వచనం అంకె_Type<నిజం,  దశాంశం>  HBFLOAT32BE;	/* 32-bit little-endian floating point number. */
రకనిర్వచనం అంకె_Type<నిజం,  ద్విగుణ> HBFLOAT64BE;	/* 64-bit little-endian floating point number. */
రకనిర్వచనం అంకె_Type<అబద్ధం, దశాంశం>  HBFLOAT32LE;	/* 32-bit little-endian floating point number. */
రకనిర్వచనం అంకె_Type<అబద్ధం, ద్విగుణ> HBFLOAT64LE;	/* 64-bit little-endian floating point number. */

/* 15-bit unsigned number; top bit used for extension. */
నిర్మాణం HBUINT15 : HBUINT16
{
  /* TODO Flesh out; actually mask top bit. */
  HBUINT15& క్రియాకారకం = (uint16_t i ) { HBUINT16::క్రియాకారకం= (i); తిరిగి *ఇది; }
  బహిరంగ:
  DEFINE_కొలత_STATIC (2);
};

/* 32-bit unsigned integer with variable encoding. */
నిర్మాణం HBUINT32VAR
{
  size_t get_కొలత () స్థిరరాశి
  {
    గుర్తులేని b0 = v[0];
    ఒకవేళ (b0 < 0x80)
      తిరిగి 1;
    లేకపోతే ఒకవేళ (b0 < 0xC0)
      తిరిగి 2;
    లేకపోతే ఒకవేళ (b0 < 0xE0)
      తిరిగి 3;
    లేకపోతే ఒకవేళ (b0 < 0xF0)
      తిరిగి 4;
    లేకపోతే
      తిరిగి 5;
  }

  స్థిర size_t get_కొలత (uint32_t v)
  {
    ఒకవేళ (v < 0x80)
      తిరిగి 1;
    లేకపోతే ఒకవేళ (v < 0x4000)
      తిరిగి 2;
    లేకపోతే ఒకవేళ (v < 0x200000)
      తిరిగి 3;
    లేకపోతే ఒకవేళ (v < 0x10000000)
      తిరిగి 4;
    లేకపోతే
      తిరిగి 5;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_range (v, 1) &&
		  hb_barrier () &&
		  c->check_range (v, get_కొలత ()));
  }

  క్రియాకారకం uint32_t () స్థిరరాశి
  {
    గుర్తులేని b0 = v[0];
    ఒకవేళ (b0 < 0x80)
      తిరిగి b0;
    లేకపోతే ఒకవేళ (b0 < 0xC0)
      తిరిగి ((b0 & 0x3F) << 8) | v[1];
    లేకపోతే ఒకవేళ (b0 < 0xE0)
      తిరిగి ((b0 & 0x1F) << 16) | (v[1] << 8) | v[2];
    లేకపోతే ఒకవేళ (b0 < 0xF0)
      తిరిగి ((b0 & 0x0F) << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    లేకపోతే
      తిరిగి (uint32_t (v[1]) << 24) | (v[2] << 16) | (v[3] << 8) | v[4];
  }

  స్థిర గుర్తులేని serialize_unsafe (గుర్తులేని అక్షరం *buf, uint32_t v)
  {
    గుర్తులేని len = get_కొలత (v);
    గుర్తులేని అక్షరం *p = buf + len;
    కొరకు (గుర్తులేని i = 0; i < len; i++)
    {
      *--p = v & 0xFF;
      v >>= 8;
    }

    ఒకవేళ (len > 1)
      buf[0] |= ((1 << (len - 1)) - 1) << (9 - len);

    తిరిగి len;
  }

  స్థిర తార్కిక serialize (hb_serialize_context_t *c, uint32_t v)
  {
    గుర్తులేని len = get_కొలత (v);
    గుర్తులేని అక్షరం *buf = c->allocate_కొలత<గుర్తులేని అక్షరం> (len, అబద్ధం);
    ఒకవేళ (unlikely (!buf))
      తిరిగి అబద్ధం;

    serialize_unsafe (buf, v);
    తిరిగి నిజం;
  }

  రక్షిత:
  గుర్తులేని అక్షరం v[5];

  బహిరంగ:
  DEFINE_కొలత_MIN (1);
};

/* 16-bit signed integer (HBINT16) that describes a quantity in FUnits. */
రకనిర్వచనం HBINT16 FWORD;

/* 32-bit signed integer (HBINT32) that describes a quantity in FUnits. */
రకనిర్వచనం HBINT32 FWORD32;

/* 16-bit unsigned integer (HBUINT16) that describes a quantity in FUnits. */
రకనిర్వచనం HBUINT16 UFWORD;

మూస <రకం_పేరు Type, గుర్తులేని fraction_బిట్లు>
నిర్మాణం HBFixed : Type
{
  స్థిర constexpr దశాంశం mult = 1.f / (1 << fraction_బిట్లు);
  static_assert (Type::static_కొలత * 8 > fraction_బిట్లు, "");

  క్రియాకారకం గుర్తుగల () స్థిరరాశి = తొలగించు;
  క్రియాకారకం గుర్తులేని () స్థిరరాశి = తొలగించు;
  స్పష్టమైన క్రియాకారకం దశాంశం () స్థిరరాశి { తిరిగి గమ్యం_float (); }
  రకం_పేరు Type::type గమ్యం_int () స్థిరరాశి { తిరిగి Type::v; }
  శూన్య సమితి_int (రకం_పేరు Type::type i ) { Type::v = i; }
  దశాంశం గమ్యం_float (దశాంశం స్థానభ్రంశం = 0) స్థిరరాశి  { తిరిగి ((int32_t) Type::v + స్థానభ్రంశం) * mult; }
  శూన్య సమితి_float (దశాంశం f) { Type::v = roundf (f / mult); }
  బహిరంగ:
  DEFINE_కొలత_STATIC (Type::static_కొలత);
};

/* 16-bit signed fixed number with the low 14 bits of fraction (2.14). */
ఉపయోగించి F2DOT14 = HBFixed<HBINT16, 14>;
ఉపయోగించి F4DOT12 = HBFixed<HBINT16, 12>;
ఉపయోగించి F6DOT10 = HBFixed<HBINT16, 10>;

/* 32-bit signed fixed-point number (16.16). */
ఉపయోగించి F16DOT16 = HBFixed<HBINT32, 16>;

/* Date represented in number of seconds since 12:00 midnight, January 1,
 * 1904. The value is represented as a signed 64-bit integer. */
నిర్మాణం LONGDATETIME
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }
  రక్షిత:
  HBINT32 major;
  HBUINT32 minor;
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

/* Array of four uint8s (length = 32 bits) used to identify a script, language
 * system, feature, or baseline */
నిర్మాణం ట్యాగ్ : HBUINT32
{
  ట్యాగ్& క్రియాకారకం = (hb_ట్యాగ్_t i) { HBUINT32::క్రియాకారకం= (i); తిరిగి *ఇది; }
  /* What the char* converters return is NOT nul-terminated.  Print using "%.4s" */
  క్రియాకారకం స్థిరరాశి అక్షరం* () స్థిరరాశి { తిరిగి reinterpret_cast<స్థిరరాశి అక్షరం *> (ఇది); }
  క్రియాకారకం అక్షరం* ()             { తిరిగి reinterpret_cast<అక్షరం *> (ఇది); }
  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

/* Glyph index number, same as uint16 (length = 16 bits) */
నిర్మాణం HBGlyphID16 : HBUINT16
{
  HBGlyphID16& క్రియాకారకం = (uint16_t i) { HBUINT16::క్రియాకారకం= (i); తిరిగి *ఇది; }
};
నిర్మాణం HBGlyphID24 : HBUINT24
{
  HBGlyphID24& క్రియాకారకం = (uint32_t i) { HBUINT24::క్రియాకారకం= (i); తిరిగి *ఇది; }
};

/* Script/language-system/feature index */
నిర్మాణం సూచీ : HBUINT16 {
  స్థిర constexpr గుర్తులేని NOT_దొరికింది_సూచీ = 0xFFFFu;
  సూచీ& క్రియాకారకం = (uint16_t i) { HBUINT16::క్రియాకారకం= (i); తిరిగి *ఇది; }
};
DECLARE_NULL_NAMESPACE_బైట్లు (OT, సూచీ);

రకనిర్వచనం సూచీ NameID;

నిర్మాణం VarIdx : HBUINT32 {
  స్థిర constexpr గుర్తులేని NO_VARIATION = 0xFFFFFFFFu;
  static_assert (NO_VARIATION == HB_OT_LAYOUT_NO_VARIATIONS_సూచీ, "");
  VarIdx& క్రియాకారకం = (uint32_t i) { HBUINT32::క్రియాకారకం= (i); తిరిగి *ఇది; }
};
DECLARE_NULL_NAMESPACE_బైట్లు (OT, VarIdx);

/* Offset, Null offset = 0 */
మూస <రకం_పేరు Type, తార్కిక has_null=నిజం>
నిర్మాణం స్థానభ్రంశం_2 : Type
{
  స్థానభ్రంశం_2& క్రియాకారకం = (రకం_పేరు Type::type i) { Type::క్రియాకారకం= (i); తిరిగి *ఇది; }

  రకనిర్వచనం Type type;

  తార్కిక is_null () స్థిరరాశి { తిరిగి has_null && 0 == *ఇది; }

  బహిరంగ:
  DEFINE_కొలత_STATIC (పరిమాణం (Type));
};

రకనిర్వచనం స్థానభ్రంశం_2<HBUINT16> Offset16;
రకనిర్వచనం స్థానభ్రంశం_2<HBUINT24> Offset24;
రకనిర్వచనం స్థానభ్రంశం_2<HBUINT32> Offset32;


/* CheckSum */
నిర్మాణం Check_కూడిక : HBUINT32
{
  Check_కూడిక& క్రియాకారకం = (uint32_t i) { HBUINT32::క్రియాకారకం= (i); తిరిగి *ఇది; }

  /* This is reference implementation from the spec. */
  స్థిర uint32_t Calc_పట్టిక_Checksum (స్థిరరాశి HBUINT32 *పట్టిక_3, uint32_t నిడివి_3)
  {
    uint32_t Sum = 0L;
    ప్రతిజ్ఞా (0 == (నిడివి_3 & 3));
    స్థిరరాశి HBUINT32 *చివర_Ptr = పట్టిక_3 + నిడివి_3 / HBUINT32::static_కొలత;

    అయితే (పట్టిక_3 < చివర_Ptr)
      Sum += *పట్టిక_3++;
    తిరిగి Sum;
  }

  /* Note: data should be 4byte aligned and have 4byte padding at the end. */
  శూన్య సమితి_for_దత్తాంశం (స్థిరరాశి శూన్య *దత్తాంశం, గుర్తులేని పూర్ణాంకం నిడివి)
  { *ఇది = Calc_పట్టిక_Checksum ((స్థిరరాశి HBUINT32 *) దత్తాంశం, నిడివి); }

  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};


/*
 * Version Numbers
 */

మూస <రకం_పేరు FixedType=HBUINT16>
నిర్మాణం Fixed_సంస్కరణ
{
  uint32_t గమ్యం_int () స్థిరరాశి { తిరిగి (major << (పరిమాణం (FixedType) * 8)) + minor; }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  FixedType major;
  FixedType minor;
  బహిరంగ:
  DEFINE_కొలత_STATIC (2 * పరిమాణం (FixedType));
};


/*
 * Template subclasses of Offset that do the dereferencing.
 * Use: (base+offset)
 */

మూస <రకం_పేరు Type, తార్కిక has_null>
నిర్మాణం _hb_has_null
{
  స్థిర స్థిరరాశి Type *get_null () { తిరిగి శూన్యసూచిక; }
  స్థిర Type *get_crap ()       { తిరిగి శూన్యసూచిక; }
};
మూస <రకం_పేరు Type>
నిర్మాణం _hb_has_null<Type, నిజం>
{
  స్థిర స్థిరరాశి Type *get_null () { తిరిగి &Null (Type); }
  స్థిర       Type *get_crap () { తిరిగి &Crap (Type); }
};

మూస <రకం_పేరు Type, రకం_పేరు స్థానభ్రంశం_Type, రకం_పేరు పునాది_Type=శూన్య, తార్కిక has_null=నిజం>
నిర్మాణం స్థానభ్రంశం_గమ్యం : స్థానభ్రంశం_2<స్థానభ్రంశం_Type, has_null>
{
  ఉపయోగించి లక్ష్యం_t = Type;

  // Make sure Type is not unbounded; works only for types that are fully defined at OffsetTo time.
  static_assert (has_null == అబద్ధం ||
		 (hb_has_null_కొలత_2 (Type) || !hb_has_min_కొలత_2 (Type)), "");

  HB_DELETE_COPY_కేటాయించు (స్థానభ్రంశం_గమ్యం);
  స్థానభ్రంశం_గమ్యం () = అప్రమేయ;

  స్థానభ్రంశం_గమ్యం& క్రియాకారకం = (రకం_పేరు స్థానభ్రంశం_Type::type i) { స్థానభ్రంశం_Type::క్రియాకారకం= (i); తిరిగి *ఇది; }

  స్థిరరాశి Type& క్రియాకారకం () (స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    ఒకవేళ (unlikely (ఇది->is_null ())) తిరిగి *_hb_has_null<Type, has_null>::get_null ();
    తిరిగి Struct_At_స్థానభ్రంశం<స్థిరరాశి Type> (పునాది, *ఇది);
  }
  Type& క్రియాకారకం () (శూన్య *పునాది) స్థిరరాశి
  {
    ఒకవేళ (unlikely (ఇది->is_null ())) తిరిగి *_hb_has_null<Type, has_null>::get_crap ();
    తిరిగి Struct_At_స్థానభ్రంశం<Type> (పునాది, *ఇది);
  }

  మూస <రకం_పేరు పునాది_3,
	    hb_enable_if (hb_is_convertible (స్థిరరాశి పునాది_3, స్థిరరాశి పునాది_Type *))>
  మిత్ర స్థిరరాశి Type& క్రియాకారకం + (స్థిరరాశి పునాది_3 &పునాది, స్థిరరాశి స్థానభ్రంశం_గమ్యం &స్థానభ్రంశం) { తిరిగి స్థానభ్రంశం ((స్థిరరాశి శూన్య *) పునాది); }
  మూస <రకం_పేరు పునాది_3,
	    hb_enable_if (hb_is_convertible (స్థిరరాశి పునాది_3, స్థిరరాశి పునాది_Type *))>
  మిత్ర స్థిరరాశి Type& క్రియాకారకం + (స్థిరరాశి స్థానభ్రంశం_గమ్యం &స్థానభ్రంశం, స్థిరరాశి పునాది_3 &పునాది) { తిరిగి స్థానభ్రంశం ((స్థిరరాశి శూన్య *) పునాది); }
  మూస <రకం_పేరు పునాది_3,
	    hb_enable_if (hb_is_convertible (పునాది_3, పునాది_Type *))>
  మిత్ర Type& క్రియాకారకం + (పునాది_3 &&పునాది, స్థానభ్రంశం_గమ్యం &స్థానభ్రంశం) { తిరిగి స్థానభ్రంశం ((శూన్య *) పునాది); }
  మూస <రకం_పేరు పునాది_3,
	    hb_enable_if (hb_is_convertible (పునాది_3, పునాది_Type *))>
  మిత్ర Type& క్రియాకారకం + (స్థానభ్రంశం_గమ్యం &స్థానభ్రంశం, పునాది_3 &&పునాది) { తిరిగి స్థానభ్రంశం ((శూన్య *) పునాది); }


  మూస <రకం_పేరు పునాది_3, రకం_పేరు ...Ts>
  తార్కిక serialize_subset (hb_subset_context_t *c, స్థిరరాశి స్థానభ్రంశం_గమ్యం& src,
			 స్థిరరాశి పునాది_3 *src_పునాది, Ts&&... ds)
  {
    *ఇది = 0;
    ఒకవేళ (src.is_null ())
      తిరిగి అబద్ధం;

    స్వయంచాలక *s = c->serializer;

    s->push ();

    తార్కిక ret = c->dispatch (src_పునాది+src, std::forward<Ts> (ds)...);

    ఒకవేళ (ret || !has_null)
      s->కూడు_link (*ఇది, s->తీయి_pack ());
    లేకపోతే
      s->తీయి_discard ();

    తిరిగి ret;
  }


  మూస <రకం_పేరు ...Ts>
  తార్కిక serialize_serialize (hb_serialize_context_t *c, Ts&&... ds)
  {
    *ఇది = 0;

    Type* obj = c->push<Type> ();
    తార్కిక ret = obj->serialize (c, std::forward<Ts> (ds)...);

    ఒకవేళ (ret)
      c->కూడు_link (*ఇది, c->తీయి_pack ());
    లేకపోతే
      c->తీయి_discard ();

    తిరిగి ret;
  }

  /* TODO: Somehow merge this with previous function into a serialize_dispatch(). */
  /* Workaround clang bug: https://bugs.llvm.org/show_bug.cgi?id=23029
   * Can't compile: whence = hb_serialize_context_t::Head followed by Ts&&...
   */
  మూస <రకం_పేరు ...Ts>
  తార్కిక serialize_copy (hb_serialize_context_t *c, స్థిరరాశి స్థానభ్రంశం_గమ్యం& src,
		       స్థిరరాశి శూన్య *src_పునాది, గుర్తులేని dst_bias,
		       hb_serialize_context_t::whence_t whence,
		       Ts&&... ds)
  {
    *ఇది = 0;
    ఒకవేళ (src.is_null ())
      తిరిగి అబద్ధం;

    c->push ();

    తార్కిక ret = c->copy (src_పునాది+src, std::forward<Ts> (ds)...);

    c->కూడు_link (*ఇది, c->తీయి_pack (), whence, dst_bias);

    తిరిగి ret;
  }

  తార్కిక serialize_copy (hb_serialize_context_t *c, స్థిరరాశి స్థానభ్రంశం_గమ్యం& src,
		       స్థిరరాశి శూన్య *src_పునాది, గుర్తులేని dst_bias = 0)
  { తిరిగి serialize_copy (c, src, src_పునాది, dst_bias, hb_serialize_context_t::తల); }

  తార్కిక sanitize_shallow (hb_sanitize_context_t *c, స్థిరరాశి పునాది_Type *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!c->check_struct (ఇది))) return_trace (అబద్ధం);
    hb_barrier ();
    //if (unlikely (this->is_null ())) return_trace (true);
    ఒకవేళ (unlikely ((స్థిరరాశి అక్షరం *) పునాది + (గుర్తులేని) *ఇది < (స్థిరరాశి అక్షరం *) పునాది)) return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  మూస <రకం_పేరు ...Ts>
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి పునాది_Type *పునాది, Ts&&... ds) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (sanitize_shallow (c, పునాది) &&
		  hb_barrier () &&
		  (ఇది->is_null () ||
		   c->dispatch (Struct_At_స్థానభ్రంశం<Type> (పునాది, *ఇది), std::forward<Ts> (ds)...)));
  }

  DEFINE_కొలత_STATIC (పరిమాణం (స్థానభ్రంశం_Type));
};
/* Partial specializations. */
మూస <రకం_పేరు Type, రకం_పేరు పునాది_Type=శూన్య, తార్కిక has_null=నిజం> ఉపయోగించి Offset16_గమ్యం = స్థానభ్రంశం_గమ్యం<Type, HBUINT16, పునాది_Type, has_null>;
మూస <రకం_పేరు Type, రకం_పేరు పునాది_Type=శూన్య, తార్కిక has_null=నిజం> ఉపయోగించి Offset24_గమ్యం = స్థానభ్రంశం_గమ్యం<Type, HBUINT24, పునాది_Type, has_null>;
మూస <రకం_పేరు Type, రకం_పేరు పునాది_Type=శూన్య, తార్కిక has_null=నిజం> ఉపయోగించి Offset32_గమ్యం = స్థానభ్రంశం_గమ్యం<Type, HBUINT32, పునాది_Type, has_null>;

మూస <రకం_పేరు Type, రకం_పేరు స్థానభ్రంశం_Type, రకం_పేరు పునాది_Type=శూన్య> ఉపయోగించి NNOffset_గమ్యం = స్థానభ్రంశం_గమ్యం<Type, స్థానభ్రంశం_Type, పునాది_Type, అబద్ధం>;
మూస <రకం_పేరు Type, రకం_పేరు పునాది_Type=శూన్య> ఉపయోగించి NNOffset16_గమ్యం = Offset16_గమ్యం<Type, పునాది_Type, అబద్ధం>;
మూస <రకం_పేరు Type, రకం_పేరు పునాది_Type=శూన్య> ఉపయోగించి NNOffset24_గమ్యం = Offset24_గమ్యం<Type, పునాది_Type, అబద్ధం>;
మూస <రకం_పేరు Type, రకం_పేరు పునాది_Type=శూన్య> ఉపయోగించి NNOffset32_గమ్యం = Offset32_గమ్యం<Type, పునాది_Type, అబద్ధం>;


/*
 * Array Types
 */

మూస <రకం_పేరు Type>
నిర్మాణం Unsized_Array_యొక్క
{
  రకనిర్వచనం Type అంశం_t;
  స్థిర constexpr గుర్తులేని అంశం_కొలత = hb_static_కొలత (Type);

  HB_DELETE_CREATE_COPY_కేటాయించు (Unsized_Array_యొక్క);

  స్థిరరాశి Type& క్రియాకారకం [] (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  {
    తిరిగి arrayZ[i];
  }
  Type& క్రియాకారకం [] (గుర్తులేని పూర్ణాంకం i)
  {
    తిరిగి arrayZ[i];
  }

  స్థిర size_t get_కొలత (గుర్తులేని పూర్ణాంకం len)
  { తిరిగి hb_unsigned_mul_saturate (len, Type::static_కొలత); }

  మూస <రకం_పేరు T> క్రియాకారకం T * () { తిరిగి arrayZ; }
  మూస <రకం_పేరు T> క్రియాకారకం స్థిరరాశి T * () స్థిరరాశి { తిరిగి arrayZ; }
  hb_array_t<Type> as_array (గుర్తులేని పూర్ణాంకం len)
  { తిరిగి hb_array (arrayZ, len); }
  hb_array_t<స్థిరరాశి Type> as_array (గుర్తులేని పూర్ణాంకం len) స్థిరరాశి
  { తిరిగి hb_array (arrayZ, len); }

  మూస <రకం_పేరు T>
  Type &lsearch (గుర్తులేని పూర్ణాంకం len, స్థిరరాశి T &x, Type &not_దొరికింది = Crap (Type))
  { తిరిగి *as_array (len).lsearch (x, &not_దొరికింది); }
  మూస <రకం_పేరు T>
  స్థిరరాశి Type &lsearch (గుర్తులేని పూర్ణాంకం len, స్థిరరాశి T &x, స్థిరరాశి Type &not_దొరికింది = Null (Type)) స్థిరరాశి
  { తిరిగి *as_array (len).lsearch (x, &not_దొరికింది); }
  మూస <రకం_పేరు T>
  తార్కిక lfind (గుర్తులేని పూర్ణాంకం len, స్థిరరాశి T &x, గుర్తులేని పూర్ణాంకం *i = శూన్యసూచిక,
	      hb_not_దొరికింది_t not_దొరికింది = HB_NOT_దొరికింది_DONT_STORE,
	      గుర్తులేని పూర్ణాంకం గమ్యం_store = (గుర్తులేని పూర్ణాంకం) -1) స్థిరరాశి
  { తిరిగి as_array (len).lfind (x, i, not_దొరికింది, గమ్యం_store); }

  శూన్య qsort (గుర్తులేని పూర్ణాంకం len, గుర్తులేని పూర్ణాంకం ప్రారంభం = 0, గుర్తులేని పూర్ణాంకం చివర = (గుర్తులేని పూర్ణాంకం) -1)
  { as_array (len).qsort (ప్రారంభం, చివర); }

  తార్కిక serialize (hb_serialize_context_t *c, గుర్తులేని పూర్ణాంకం వస్తువులు_len, తార్కిక clear = నిజం)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_కొలత (ఇది, get_కొలత (వస్తువులు_len), clear))) return_trace (అబద్ధం);
    return_trace (నిజం);
  }
  మూస <రకం_పేరు Iterator,
	    hb_requires (hb_is_ఆధారం_యొక్క (Iterator, Type))>
  తార్కిక serialize (hb_serialize_context_t *c, Iterator వస్తువులు)
  {
    TRACE_SERIALIZE (ఇది);
    గుర్తులేని లెక్క = hb_len (వస్తువులు);
    ఒకవేళ (unlikely (!serialize (c, లెక్క, అబద్ధం))) return_trace (అబద్ధం);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++, ++వస్తువులు)
      arrayZ[i] = *వస్తువులు;
    return_trace (నిజం);
  }

  Unsized_Array_యొక్క* copy (hb_serialize_context_t *c, గుర్తులేని లెక్క) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక *బహిర్గతం = c->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!as_array (లెక్క).copy (c))) return_trace (శూన్యసూచిక);
    return_trace (బహిర్గతం);
  }

  మూస <రకం_పేరు ...Ts>
  HB_ALWAYS_INLINE
  తార్కిక sanitize (hb_sanitize_context_t *c, గుర్తులేని పూర్ణాంకం లెక్క, Ts&&... ds) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!sanitize_shallow (c, లెక్క))) return_trace (అబద్ధం);
    ఒకవేళ (!పరిమాణం... (Ts) && hb_is_trivially_copyable(Type)) return_trace (నిజం);
    hb_barrier ();
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  తార్కిక sanitize_shallow (hb_sanitize_context_t *c, గుర్తులేని పూర్ణాంకం లెక్క) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_array (arrayZ, లెక్క));
  }

  బహిరంగ:
  Type		arrayZ[HB_VAR_ARRAY];
  బహిరంగ:
  DEFINE_కొలత_UNBOUNDED (0);
};

/* Unsized array of offset's */
మూస <రకం_పేరు Type, రకం_పేరు స్థానభ్రంశం_Type, రకం_పేరు పునాది_Type=శూన్య, తార్కిక has_null=నిజం>
ఉపయోగించి Unsized_Array16_యొక్క_స్థానభ్రంశం_గమ్యం = Unsized_Array_యొక్క<స్థానభ్రంశం_గమ్యం<Type, స్థానభ్రంశం_Type, పునాది_Type, has_null>>;

/* Unsized array of offsets relative to the beginning of the array itself. */
మూస <రకం_పేరు Type, రకం_పేరు స్థానభ్రంశం_Type, రకం_పేరు పునాది_Type=శూన్య, తార్కిక has_null=నిజం>
నిర్మాణం Unsized_List_యొక్క_Offset16_గమ్యం : Unsized_Array16_యొక్క_స్థానభ్రంశం_గమ్యం<Type, స్థానభ్రంశం_Type, పునాది_Type, has_null>
{
  స్థిరరాశి Type& క్రియాకారకం [] (పూర్ణాంకం i_) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం i = (గుర్తులేని పూర్ణాంకం) i_;
    స్థిరరాశి స్థానభ్రంశం_గమ్యం<Type, స్థానభ్రంశం_Type, పునాది_Type, has_null> *p = &ఇది->arrayZ[i];
    ఒకవేళ (unlikely ((స్థిరరాశి శూన్య *) p < (స్థిరరాశి శూన్య *) ఇది->arrayZ)) తిరిగి Null (Type); /* Overflowed. */
    hb_barrier ();
    తిరిగి ఇది+*p;
  }
  Type& క్రియాకారకం [] (పూర్ణాంకం i_)
  {
    గుర్తులేని పూర్ణాంకం i = (గుర్తులేని పూర్ణాంకం) i_;
    స్థిరరాశి స్థానభ్రంశం_గమ్యం<Type, స్థానభ్రంశం_Type, పునాది_Type, has_null> *p = &ఇది->arrayZ[i];
    ఒకవేళ (unlikely ((స్థిరరాశి శూన్య *) p < (స్థిరరాశి శూన్య *) ఇది->arrayZ)) తిరిగి Crap (Type); /* Overflowed. */
    hb_barrier ();
    తిరిగి ఇది+*p;
  }

  మూస <రకం_పేరు ...Ts>
  తార్కిక sanitize (hb_sanitize_context_t *c, గుర్తులేని పూర్ణాంకం లెక్క, Ts&&... ds) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace ((Unsized_Array16_యొక్క_స్థానభ్రంశం_గమ్యం<Type, స్థానభ్రంశం_Type, పునాది_Type, has_null>
		   ::sanitize (c, లెక్క, ఇది, std::forward<Ts> (ds)...)));
  }
};

/* An array with sorted elements.  Supports binary searching. */
మూస <రకం_పేరు Type>
నిర్మాణం Sorted_Unsized_Array_యొక్క : Unsized_Array_యొక్క<Type>
{
  hb_sorted_array_t<Type> as_array (గుర్తులేని పూర్ణాంకం len)
  { తిరిగి hb_sorted_array (ఇది->arrayZ, len); }
  hb_sorted_array_t<స్థిరరాశి Type> as_array (గుర్తులేని పూర్ణాంకం len) స్థిరరాశి
  { తిరిగి hb_sorted_array (ఇది->arrayZ, len); }
  క్రియాకారకం hb_sorted_array_t<Type> ()             { తిరిగి as_array (); }
  క్రియాకారకం hb_sorted_array_t<స్థిరరాశి Type> () స్థిరరాశి { తిరిగి as_array (); }

  మూస <రకం_పేరు T>
  Type &bsearch (గుర్తులేని పూర్ణాంకం len, స్థిరరాశి T &x, Type &not_దొరికింది = Crap (Type))
  { తిరిగి *as_array (len).bsearch (x, &not_దొరికింది); }
  మూస <రకం_పేరు T>
  స్థిరరాశి Type &bsearch (గుర్తులేని పూర్ణాంకం len, స్థిరరాశి T &x, స్థిరరాశి Type &not_దొరికింది = Null (Type)) స్థిరరాశి
  { తిరిగి *as_array (len).bsearch (x, &not_దొరికింది); }
  మూస <రకం_పేరు T>
  తార్కిక bfind (గుర్తులేని పూర్ణాంకం len, స్థిరరాశి T &x, గుర్తులేని పూర్ణాంకం *i = శూన్యసూచిక,
	      hb_not_దొరికింది_t not_దొరికింది = HB_NOT_దొరికింది_DONT_STORE,
	      గుర్తులేని పూర్ణాంకం గమ్యం_store = (గుర్తులేని పూర్ణాంకం) -1) స్థిరరాశి
  { తిరిగి as_array (len).bfind (x, i, not_దొరికింది, గమ్యం_store); }
};


/* An array with a number of elements. */
మూస <రకం_పేరు Type, రకం_పేరు LenType>
నిర్మాణం Array_యొక్క
{
  రకనిర్వచనం Type అంశం_t;
  స్థిర constexpr గుర్తులేని అంశం_కొలత = hb_static_కొలత (Type);

  HB_DELETE_CREATE_COPY_కేటాయించు (Array_యొక్క);

  స్థిరరాశి Type& క్రియాకారకం [] (పూర్ణాంకం i_) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం i = (గుర్తులేని పూర్ణాంకం) i_;
    ఒకవేళ (unlikely (i >= len)) తిరిగి Null (Type);
    hb_barrier ();
    తిరిగి arrayZ[i];
  }
  Type& క్రియాకారకం [] (పూర్ణాంకం i_)
  {
    గుర్తులేని పూర్ణాంకం i = (గుర్తులేని పూర్ణాంకం) i_;
    ఒకవేళ (unlikely (i >= len)) తిరిగి Crap (Type);
    hb_barrier ();
    తిరిగి arrayZ[i];
  }

  size_t get_కొలత () స్థిరరాశి
  { తిరిగి hb_unsigned_mul_కూడు_saturate (len, Type::static_కొలత, len.static_కొలత); }

  స్పష్టమైన క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి len; }

  శూన్య తీయి () { len--; }

  hb_array_t<      Type> as_array ()       { తిరిగి hb_array (arrayZ, len); }
  hb_array_t<స్థిరరాశి Type> as_array () స్థిరరాశి { తిరిగి hb_array (arrayZ, len); }

  /* Iterator. */
  రకనిర్వచనం hb_array_t<స్థిరరాశి Type>   iter_t;
  రకనిర్వచనం hb_array_t<      Type> writer_t;
    iter_t   iter () స్థిరరాశి { తిరిగి as_array (); }
  writer_t writer ()       { తిరిగి as_array (); }
  క్రియాకారకం   iter_t () స్థిరరాశి { తిరిగి   iter (); }
  క్రియాకారకం writer_t ()       { తిరిగి writer (); }

  /* Faster range-based for loop. */
  స్థిరరాశి Type *begin () స్థిరరాశి { తిరిగి arrayZ; }
  స్థిరరాశి Type *చివర () స్థిరరాశి { తిరిగి arrayZ + len; }

  మూస <రకం_పేరు T>
  Type &lsearch (స్థిరరాశి T &x, Type &not_దొరికింది = Crap (Type))
  { తిరిగి *as_array ().lsearch (x, &not_దొరికింది); }
  మూస <రకం_పేరు T>
  స్థిరరాశి Type &lsearch (స్థిరరాశి T &x, స్థిరరాశి Type &not_దొరికింది = Null (Type)) స్థిరరాశి
  { తిరిగి *as_array ().lsearch (x, &not_దొరికింది); }
  మూస <రకం_పేరు T>
  తార్కిక lfind (స్థిరరాశి T &x, గుర్తులేని పూర్ణాంకం *i = శూన్యసూచిక,
	      hb_not_దొరికింది_t not_దొరికింది = HB_NOT_దొరికింది_DONT_STORE,
	      గుర్తులేని పూర్ణాంకం గమ్యం_store = (గుర్తులేని పూర్ణాంకం) -1) స్థిరరాశి
  { తిరిగి as_array ().lfind (x, i, not_దొరికింది, గమ్యం_store); }

  శూన్య qsort ()
  { as_array ().qsort (); }

  HB_NODISCARD తార్కిక serialize (hb_serialize_context_t *c, గుర్తులేని వస్తువులు_len, తార్కిక clear = నిజం)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    c->check_కేటాయించు (len, వస్తువులు_len, HB_SERIALIZE_ERROR_ARRAY_అధికప్రవాహం);
    ఒకవేళ (unlikely (!c->extend_కొలత (ఇది, get_కొలత (), clear))) return_trace (అబద్ధం);
    return_trace (నిజం);
  }
  మూస <రకం_పేరు Iterator,
	    hb_requires (hb_is_ఆధారం_యొక్క (Iterator, Type))>
  HB_NODISCARD తార్కిక serialize (hb_serialize_context_t *c, Iterator వస్తువులు)
  {
    TRACE_SERIALIZE (ఇది);
    గుర్తులేని లెక్క = hb_len (వస్తువులు);
    ఒకవేళ (unlikely (!serialize (c, లెక్క, అబద్ధం))) return_trace (అబద్ధం);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++, ++వస్తువులు)
      arrayZ[i] = *వస్తువులు;
    return_trace (నిజం);
  }

  Type* serialize_append (hb_serialize_context_t *c)
  {
    TRACE_SERIALIZE (ఇది);
    len++;
    ఒకవేళ (unlikely (!len || !c->extend (ఇది)))
    {
      len--;
      return_trace (శూన్యసూచిక);
    }
    return_trace (&arrayZ[len - 1]);
  }

  Array_యొక్క* copy (hb_serialize_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక *బహిర్గతం = c->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (బహిర్గతం))) return_trace (శూన్యసూచిక);
    c->check_కేటాయించు (బహిర్గతం->len, len, HB_SERIALIZE_ERROR_ARRAY_అధికప్రవాహం);
    ఒకవేళ (unlikely (!as_array ().copy (c))) return_trace (శూన్యసూచిక);
    return_trace (బహిర్గతం);
  }

  మూస <రకం_పేరు ...Ts>
  HB_ALWAYS_INLINE
  తార్కిక sanitize (hb_sanitize_context_t *c, Ts&&... ds) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!sanitize_shallow (c))) return_trace (అబద్ధం);
    ఒకవేళ (!పరిమాణం... (Ts) && hb_is_trivially_copyable(Type)) return_trace (నిజం);
    hb_barrier ();
    గుర్తులేని పూర్ణాంకం లెక్క = len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  తార్కిక sanitize_shallow (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (len.sanitize (c) &&
		  hb_barrier () &&
		  c->check_array_sized (arrayZ, len, పరిమాణం (LenType)));
  }

  బహిరంగ:
  LenType	len;
  Type		arrayZ[HB_VAR_ARRAY];
  బహిరంగ:
  DEFINE_కొలత_ARRAY (పరిమాణం (LenType), arrayZ);
};
మూస <రకం_పేరు Type> ఉపయోగించి Array16_యొక్క = Array_యొక్క<Type, HBUINT16>;
మూస <రకం_పేరు Type> ఉపయోగించి Array24_యొక్క = Array_యొక్క<Type, HBUINT24>;
మూస <రకం_పేరు Type> ఉపయోగించి Array32_యొక్క = Array_యొక్క<Type, HBUINT32>;
ఉపయోగించి PString = Array_యొక్క<HBUINT8, HBUINT8>;

/* Array of Offset's */
మూస <రకం_పేరు Type> ఉపయోగించి Array8_యొక్క_Offset24_గమ్యం = Array_యొక్క<స్థానభ్రంశం_గమ్యం<Type, HBUINT24>, HBUINT8>;
మూస <రకం_పేరు Type> ఉపయోగించి Array16_యొక్క_Offset16_గమ్యం = Array_యొక్క<స్థానభ్రంశం_గమ్యం<Type, HBUINT16>, HBUINT16>;
మూస <రకం_పేరు Type> ఉపయోగించి Array16_యొక్క_Offset32_గమ్యం = Array_యొక్క<స్థానభ్రంశం_గమ్యం<Type, HBUINT32>, HBUINT16>;
మూస <రకం_పేరు Type> ఉపయోగించి Array32_యొక్క_Offset32_గమ్యం = Array_యొక్క<స్థానభ్రంశం_గమ్యం<Type, HBUINT32>, HBUINT32>;

/* Array of offsets relative to the beginning of the array itself. */
మూస <రకం_పేరు Type, రకం_పేరు స్థానభ్రంశం_Type>
నిర్మాణం List16_యొక్క_స్థానభ్రంశం_గమ్యం : Array_యొక్క<స్థానభ్రంశం_గమ్యం<Type, స్థానభ్రంశం_Type>, HBUINT16>
{
  స్థిరరాశి Type& క్రియాకారకం [] (పూర్ణాంకం i_) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం i = (గుర్తులేని పూర్ణాంకం) i_;
    ఒకవేళ (unlikely (i >= ఇది->len)) తిరిగి Null (Type);
    hb_barrier ();
    తిరిగి ఇది+ఇది->arrayZ[i];
  }
  స్థిరరాశి Type& క్రియాకారకం [] (పూర్ణాంకం i_)
  {
    గుర్తులేని పూర్ణాంకం i = (గుర్తులేని పూర్ణాంకం) i_;
    ఒకవేళ (unlikely (i >= ఇది->len)) తిరిగి Crap (Type);
    hb_barrier ();
    తిరిగి ఇది+ఇది->arrayZ[i];
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    నిర్మాణం List16_యొక్క_స్థానభ్రంశం_గమ్యం *బహిర్గతం = c->serializer->embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);
    గుర్తులేని పూర్ణాంకం లెక్క = ఇది->len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      బహిర్గతం->arrayZ[i].serialize_subset (c, ఇది->arrayZ[i], ఇది, బహిర్గతం);
    return_trace (నిజం);
  }

  మూస <రకం_పేరు ...Ts>
  తార్కిక sanitize (hb_sanitize_context_t *c, Ts&&... ds) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace ((Array16_యొక్క<స్థానభ్రంశం_గమ్యం<Type, స్థానభ్రంశం_Type>>::sanitize (c, ఇది, std::forward<Ts> (ds)...)));
  }
};

మూస <రకం_పేరు Type>
ఉపయోగించి List16_యొక్క_Offset16_గమ్యం = List16_యొక్క_స్థానభ్రంశం_గమ్యం<Type, HBUINT16>;

/* An array starting at second element. */
మూస <రకం_పేరు Type, రకం_పేరు LenType>
నిర్మాణం Headless_Array_యొక్క
{
  స్థిర constexpr గుర్తులేని అంశం_కొలత = Type::static_కొలత;

  HB_DELETE_CREATE_COPY_కేటాయించు (Headless_Array_యొక్క);

  స్థిరరాశి Type& క్రియాకారకం [] (పూర్ణాంకం i_) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం i = (గుర్తులేని పూర్ణాంకం) i_;
    ఒకవేళ (unlikely (i >= lenP1 || !i)) తిరిగి Null (Type);
    hb_barrier ();
    తిరిగి arrayZ[i-1];
  }
  Type& క్రియాకారకం [] (పూర్ణాంకం i_)
  {
    గుర్తులేని పూర్ణాంకం i = (గుర్తులేని పూర్ణాంకం) i_;
    ఒకవేళ (unlikely (i >= lenP1 || !i)) తిరిగి Crap (Type);
    hb_barrier ();
    తిరిగి arrayZ[i-1];
  }
  size_t get_కొలత () స్థిరరాశి
  { తిరిగి hb_unsigned_mul_కూడు_saturate (get_నిడివి (), Type::static_కొలత, lenP1.static_కొలత); }

  గుర్తులేని get_నిడివి () స్థిరరాశి { తిరిగి lenP1 ? lenP1 - 1 : 0; }

  hb_array_t<      Type> as_array ()       { తిరిగి hb_array (arrayZ, get_నిడివి ()); }
  hb_array_t<స్థిరరాశి Type> as_array () స్థిరరాశి { తిరిగి hb_array (arrayZ, get_నిడివి ()); }

  /* Iterator. */
  రకనిర్వచనం hb_array_t<స్థిరరాశి Type>   iter_t;
  రకనిర్వచనం hb_array_t<      Type> writer_t;
    iter_t   iter () స్థిరరాశి { తిరిగి as_array (); }
  writer_t writer ()       { తిరిగి as_array (); }
  క్రియాకారకం   iter_t () స్థిరరాశి { తిరిగి   iter (); }
  క్రియాకారకం writer_t ()       { తిరిగి writer (); }

  /* Faster range-based for loop. */
  స్థిరరాశి Type *begin () స్థిరరాశి { తిరిగి arrayZ; }
  స్థిరరాశి Type *చివర () స్థిరరాశి { తిరిగి arrayZ + get_నిడివి (); }

  HB_NODISCARD తార్కిక serialize (hb_serialize_context_t *c, గుర్తులేని పూర్ణాంకం వస్తువులు_len, తార్కిక clear = నిజం)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    c->check_కేటాయించు (lenP1, వస్తువులు_len + 1, HB_SERIALIZE_ERROR_ARRAY_అధికప్రవాహం);
    ఒకవేళ (unlikely (!c->extend_కొలత (ఇది, get_కొలత (), clear))) return_trace (అబద్ధం);
    return_trace (నిజం);
  }
  మూస <రకం_పేరు Iterator,
	    hb_requires (hb_is_ఆధారం_యొక్క (Iterator, Type))>
  HB_NODISCARD తార్కిక serialize (hb_serialize_context_t *c, Iterator వస్తువులు)
  {
    TRACE_SERIALIZE (ఇది);
    గుర్తులేని లెక్క = hb_len (వస్తువులు);
    ఒకవేళ (unlikely (!serialize (c, లెక్క, అబద్ధం))) return_trace (అబద్ధం);
    /* TODO Umm. Just exhaust the iterator instead?  Being extra
     * cautious right now.. */
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++, ++వస్తువులు)
      arrayZ[i] = *వస్తువులు;
    return_trace (నిజం);
  }

  మూస <రకం_పేరు ...Ts>
  HB_ALWAYS_INLINE
  తార్కిక sanitize (hb_sanitize_context_t *c, Ts&&... ds) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!sanitize_shallow (c))) return_trace (అబద్ధం);
    ఒకవేళ (!పరిమాణం... (Ts) && hb_is_trivially_copyable(Type)) return_trace (నిజం);
    hb_barrier ();
    గుర్తులేని పూర్ణాంకం లెక్క = get_నిడివి ();
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  అంతరంగ:
  తార్కిక sanitize_shallow (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (lenP1.sanitize (c) &&
		  hb_barrier () &&
		  (!lenP1 || c->check_array_sized (arrayZ, lenP1 - 1, పరిమాణం (LenType))));
  }

  బహిరంగ:
  LenType	lenP1;
  Type		arrayZ[HB_VAR_ARRAY];
  బహిరంగ:
  DEFINE_కొలత_ARRAY (పరిమాణం (LenType), arrayZ);
};
మూస <రకం_పేరు Type> ఉపయోగించి Headless_Array16_యొక్క = Headless_Array_యొక్క<Type, HBUINT16>;

/* An array storing length-1. */
మూస <రకం_పేరు Type, రకం_పేరు LenType=HBUINT16>
నిర్మాణం Array_యొక్క_M1
{
  HB_DELETE_CREATE_COPY_కేటాయించు (Array_యొక్క_M1);

  స్థిరరాశి Type& క్రియాకారకం [] (పూర్ణాంకం i_) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం i = (గుర్తులేని పూర్ణాంకం) i_;
    ఒకవేళ (unlikely (i > lenM1)) తిరిగి Null (Type);
    hb_barrier ();
    తిరిగి arrayZ[i];
  }
  Type& క్రియాకారకం [] (పూర్ణాంకం i_)
  {
    గుర్తులేని పూర్ణాంకం i = (గుర్తులేని పూర్ణాంకం) i_;
    ఒకవేళ (unlikely (i > lenM1)) తిరిగి Crap (Type);
    hb_barrier ();
    తిరిగి arrayZ[i];
  }
  size_t get_కొలత () స్థిరరాశి
  { తిరిగి hb_unsigned_mul_కూడు_saturate (lenM1 + 1, Type::static_కొలత, lenM1.static_కొలత); }

  మూస <రకం_పేరు ...Ts>
  HB_ALWAYS_INLINE
  తార్కిక sanitize (hb_sanitize_context_t *c, Ts&&... ds) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!sanitize_shallow (c))) return_trace (అబద్ధం);
    ఒకవేళ (!పరిమాణం... (Ts) && hb_is_trivially_copyable(Type)) return_trace (నిజం);
    hb_barrier ();
    గుర్తులేని పూర్ణాంకం లెక్క = lenM1 + 1;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (unlikely (!c->dispatch (arrayZ[i], std::forward<Ts> (ds)...)))
	return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  అంతరంగ:
  తార్కిక sanitize_shallow (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (lenM1.sanitize (c) &&
		  hb_barrier () &&
		  (c->check_array_sized (arrayZ, lenM1 + 1, పరిమాణం (LenType))));
  }

  బహిరంగ:
  LenType	lenM1;
  Type		arrayZ[HB_VAR_ARRAY];
  బహిరంగ:
  DEFINE_కొలత_ARRAY (పరిమాణం (LenType), arrayZ);
};

/* An array with sorted elements.  Supports binary searching. */
మూస <రకం_పేరు Type, రకం_పేరు LenType>
నిర్మాణం Sorted_Array_యొక్క : Array_యొక్క<Type, LenType>
{
  hb_sorted_array_t<      Type> as_array ()       { తిరిగి hb_sorted_array (ఇది->arrayZ, ఇది->len); }
  hb_sorted_array_t<స్థిరరాశి Type> as_array () స్థిరరాశి { తిరిగి hb_sorted_array (ఇది->arrayZ, ఇది->len); }

  /* Iterator. */
  రకనిర్వచనం hb_sorted_array_t<స్థిరరాశి Type>   iter_t;
  రకనిర్వచనం hb_sorted_array_t<      Type> writer_t;
    iter_t   iter () స్థిరరాశి { తిరిగి as_array (); }
  writer_t writer ()       { తిరిగి as_array (); }
  క్రియాకారకం   iter_t () స్థిరరాశి { తిరిగి   iter (); }
  క్రియాకారకం writer_t ()       { తిరిగి writer (); }

  /* Faster range-based for loop. */
  స్థిరరాశి Type *begin () స్థిరరాశి { తిరిగి ఇది->arrayZ; }
  స్థిరరాశి Type *చివర () స్థిరరాశి { తిరిగి ఇది->arrayZ + ఇది->len; }

  తార్కిక serialize (hb_serialize_context_t *c, గుర్తులేని పూర్ణాంకం వస్తువులు_len)
  {
    TRACE_SERIALIZE (ఇది);
    తార్కిక ret = Array_యొక్క<Type, LenType>::serialize (c, వస్తువులు_len);
    return_trace (ret);
  }
  మూస <రకం_పేరు Iterator,
	    hb_requires (hb_is_sorted_ఆధారం_యొక్క (Iterator, Type))>
  తార్కిక serialize (hb_serialize_context_t *c, Iterator వస్తువులు)
  {
    TRACE_SERIALIZE (ఇది);
    తార్కిక ret = Array_యొక్క<Type, LenType>::serialize (c, వస్తువులు);
    return_trace (ret);
  }

  Sorted_Array_యొక్క* copy (hb_serialize_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    Sorted_Array_యొక్క* బహిర్గతం = reinterpret_cast<Sorted_Array_యొక్క *> (Array_యొక్క<Type, LenType>::copy (c));
    return_trace (బహిర్గతం);
  }

  మూస <రకం_పేరు T>
  Type &bsearch (స్థిరరాశి T &x, Type &not_దొరికింది = Crap (Type))
  { తిరిగి *as_array ().bsearch (x, &not_దొరికింది); }
  మూస <రకం_పేరు T>
  స్థిరరాశి Type &bsearch (స్థిరరాశి T &x, స్థిరరాశి Type &not_దొరికింది = Null (Type)) స్థిరరాశి
  { తిరిగి *as_array ().bsearch (x, &not_దొరికింది); }
  మూస <రకం_పేరు T>
  తార్కిక bfind (స్థిరరాశి T &x, గుర్తులేని పూర్ణాంకం *i = శూన్యసూచిక,
	      hb_not_దొరికింది_t not_దొరికింది = HB_NOT_దొరికింది_DONT_STORE,
	      గుర్తులేని పూర్ణాంకం గమ్యం_store = (గుర్తులేని పూర్ణాంకం) -1) స్థిరరాశి
  { తిరిగి as_array ().bfind (x, i, not_దొరికింది, గమ్యం_store); }
};

మూస <రకం_పేరు Type> ఉపయోగించి Sorted_Array16_యొక్క = Sorted_Array_యొక్క<Type, HBUINT16>;
మూస <రకం_పేరు Type> ఉపయోగించి Sorted_Array24_యొక్క = Sorted_Array_యొక్క<Type, HBUINT24>;
మూస <రకం_పేరు Type> ఉపయోగించి Sorted_Array32_యొక్క = Sorted_Array_యొక్క<Type, HBUINT32>;

/*
 * Binary-search arrays
 */

మూస <రకం_పేరు LenType=HBUINT16>
నిర్మాణం Bin_శోధన_శీర్షిక
{
  క్రియాకారకం uint32_t () స్థిరరాశి { తిరిగి len; }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  Bin_శోధన_శీర్షిక& క్రియాకారకం = (గుర్తులేని పూర్ణాంకం v)
  {
    len = v;
    ప్రతిజ్ఞా (len == v);
    నమోదు_Selector = hb_max (1u, hb_బిట్టు_storage (v)) - 1;
    శోధన_Range = 16 * (1u << నమోదు_Selector);
    range_జరుపు = v * 16 > శోధన_Range
		 ? 16 * v - శోధన_Range
		 : 0;
    తిరిగి *ఇది;
  }

  రక్షిత:
  LenType	len;
  LenType	శోధన_Range;
  LenType	నమోదు_Selector;
  LenType	range_జరుపు;

  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

మూస <రకం_పేరు Type, రకం_పేరు LenType=HBUINT16>
ఉపయోగించి Bin_శోధన_Array_యొక్క = Sorted_Array_యొక్క<Type, Bin_శోధన_శీర్షిక<LenType>>;


నిర్మాణం Var_Sized_Bin_శోధన_శీర్షిక
{

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  HBUINT16	unit_కొలత;	/* Size of a lookup unit for this search in bytes. */
  HBUINT16	nUnits;		/* Number of units of the preceding size to be searched. */
  HBUINT16	శోధన_Range;	/* The value of unitSize times the largest power of 2
				 * that is less than or equal to the value of nUnits. */
  HBUINT16	నమోదు_Selector;	/* The log base 2 of the largest power of 2 less than
				 * or equal to the value of nUnits. */
  HBUINT16	range_జరుపు;	/* The value of unitSize times the difference of the
				 * value of nUnits minus the largest power of 2 less
				 * than or equal to the value of nUnits. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (10);
};

మూస <రకం_పేరు Type>
నిర్మాణం Var_Sized_Bin_శోధన_Array_యొక్క
{
  స్థిర constexpr గుర్తులేని అంశం_కొలత = Type::static_కొలత;

  HB_DELETE_CREATE_COPY_కేటాయించు (Var_Sized_Bin_శోధన_Array_యొక్క);

  తార్కిక చివరి_is_terminator () స్థిరరాశి
  {
    ఒకవేళ (unlikely (!శీర్షిక.nUnits)) తిరిగి అబద్ధం;

    /* Gah.
     *
     * "The number of termination values that need to be included is table-specific.
     * The value that indicates binary search termination is 0xFFFF." */
    స్థిరరాశి HBUINT16 *పదాలు = &Struct_At_స్థానభ్రంశం<HBUINT16> (&బైట్లు_Z, (శీర్షిక.nUnits - 1) * శీర్షిక.unit_కొలత);
    గుర్తులేని పూర్ణాంకం లెక్క = Type::Termination_మాట_లెక్క;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (పదాలు[i] != 0xFFFFu)
	తిరిగి అబద్ధం;
    తిరిగి నిజం;
  }

  స్థిరరాశి Type& క్రియాకారకం [] (పూర్ణాంకం i_) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం i = (గుర్తులేని పూర్ణాంకం) i_;
    ఒకవేళ (unlikely (i >= get_నిడివి ())) తిరిగి Null (Type);
    hb_barrier ();
    తిరిగి Struct_At_స్థానభ్రంశం<Type> (&బైట్లు_Z, i * శీర్షిక.unit_కొలత);
  }
  Type& క్రియాకారకం [] (పూర్ణాంకం i_)
  {
    గుర్తులేని పూర్ణాంకం i = (గుర్తులేని పూర్ణాంకం) i_;
    ఒకవేళ (unlikely (i >= get_నిడివి ())) తిరిగి Crap (Type);
    hb_barrier ();
    తిరిగి Struct_At_స్థానభ్రంశం<Type> (&బైట్లు_Z, i * శీర్షిక.unit_కొలత);
  }
  గుర్తులేని పూర్ణాంకం get_నిడివి () స్థిరరాశి
  { తిరిగి శీర్షిక.nUnits - చివరి_is_terminator (); }
  size_t get_కొలత () స్థిరరాశి
  { తిరిగి hb_unsigned_mul_కూడు_saturate (శీర్షిక.nUnits, శీర్షిక.unit_కొలత, శీర్షిక.static_కొలత); }

  మూస <రకం_పేరు ...Ts>
  HB_ALWAYS_INLINE
  తార్కిక sanitize (hb_sanitize_context_t *c, Ts&&... ds) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!sanitize_shallow (c))) return_trace (అబద్ధం);
    ఒకవేళ (!పరిమాణం... (Ts) && hb_is_trivially_copyable(Type)) return_trace (నిజం);
    hb_barrier ();
    గుర్తులేని పూర్ణాంకం లెక్క = get_నిడివి ();
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (unlikely (!(*ఇది)[i].sanitize (c, std::forward<Ts> (ds)...)))
	return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  మూస <రకం_పేరు T>
  స్థిరరాశి Type *bsearch (స్థిరరాశి T &key) స్థిరరాశి
  {
    గుర్తులేని pos;
    తిరిగి hb_bsearch_impl (&pos,
			    key,
			    (స్థిరరాశి శూన్య *) బైట్లు_Z,
			    get_నిడివి (),
			    శీర్షిక.unit_కొలత,
			    hb_cmp_పద్ధతి<T, Type>)
	   ? (స్థిరరాశి Type *) (((స్థిరరాశి అక్షరం *) &బైట్లు_Z) + (pos * శీర్షిక.unit_కొలత))
	   : శూన్యసూచిక;
  }

  అంతరంగ:
  తార్కిక sanitize_shallow (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (శీర్షిక.sanitize (c) &&
		  hb_barrier () &&
		  Type::static_కొలత <= శీర్షిక.unit_కొలత &&
		  c->check_range (బైట్లు_Z.arrayZ,
				  శీర్షిక.nUnits,
				  శీర్షిక.unit_కొలత));
  }

  రక్షిత:
  Var_Sized_Bin_శోధన_శీర్షిక	శీర్షిక;
  Unsized_Array_యొక్క<HBUINT8>	బైట్లు_Z;
  బహిరంగ:
  DEFINE_కొలత_ARRAY (10, బైట్లు_Z);
};


/* CFF INDEX */

మూస <రకం_పేరు లెక్క_3>
నిర్మాణం CFFIndex
{
  గుర్తులేని పూర్ణాంకం స్థానభ్రంశం_array_కొలత () స్థిరరాశి
  { తిరిగి off_కొలత * (లెక్క + 1); }

  మూస <రకం_పేరు Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి Iterable &iterable,
		  స్థిరరాశి గుర్తులేని *p_దత్తాంశం_కొలత = శూన్యసూచిక,
                  గుర్తులేని min_off_కొలత = 0)
  {
    TRACE_SERIALIZE (ఇది);
    గుర్తులేని దత్తాంశం_కొలత;
    ఒకవేళ (p_దత్తాంశం_కొలత)
      దత్తాంశం_కొలత = *p_దత్తాంశం_కొలత;
    లేకపోతే
      మొత్తం_కొలత (iterable, &దత్తాంశం_కొలత);

    స్వయంచాలక it = hb_iter (iterable);
    ఒకవేళ (unlikely (!serialize_శీర్షిక (c, +it, దత్తాంశం_కొలత, min_off_కొలత))) return_trace (అబద్ధం);
    గుర్తులేని అక్షరం *ret = c->allocate_కొలత<గుర్తులేని అక్షరం> (దత్తాంశం_కొలత, అబద్ధం);
    ఒకవేళ (unlikely (!ret)) return_trace (అబద్ధం);
    గుర్తులేని మిగిలిన = దత్తాంశం_కొలత;
    కొరకు (స్థిరరాశి స్వయంచాలక &_ : +it)
    {
      గుర్తులేని len = _.నిడివి;

      ఒకవేళ (!len)
	కొనసాగు;

      ఒకవేళ (unlikely (len > మిగిలిన)) {
        // We have more bytes to write then the computed data size, so the size calculation
        // must have encountered overflow.
        return_trace (c->check_success (అబద్ధం, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
      }

      మిగిలిన -= len;

      ఒకవేళ (len <= 1)
      {
	*ret++ = *_.arrayZ;
	కొనసాగు;
      }
      hb_memcpy (ret, _.arrayZ, len);
      ret += len;
    }
    return_trace (నిజం);
  }

  మూస <రకం_పేరు Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  తార్కిక serialize_శీర్షిక (hb_serialize_context_t *c,
			 Iterator it,
			 గుర్తులేని దత్తాంశం_కొలత,
                         గుర్తులేని min_off_కొలత = 0)
  {
    TRACE_SERIALIZE (ఇది);

    గుర్తులేని off_కొలత_2 = (hb_బిట్టు_storage (దత్తాంశం_కొలత + 1) + 7) / 8;
    off_కొలత_2 = hb_max(min_off_కొలత, off_కొలత_2);

    /* serialize CFFIndex header */
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    ఇది->లెక్క = hb_len (it);
    ఒకవేళ (!ఇది->లెక్క) return_trace (నిజం);
    ఒకవేళ (unlikely (!c->extend (ఇది->off_కొలత))) return_trace (అబద్ధం);
    ఇది->off_కొలత = off_కొలత_2;
    ఒకవేళ (unlikely (!c->allocate_కొలత<HBUINT8> (off_కొలత_2 * (ఇది->లెక్క + 1), అబద్ధం)))
      return_trace (అబద్ధం);

    /* serialize indices */
    గుర్తులేని పూర్ణాంకం స్థానభ్రంశం = 1;
    ఒకవేళ (HB_OPTIMIZE_కొలత_VAL)
    {
      గుర్తులేని పూర్ణాంకం i = 0;
      కొరకు (స్థిరరాశి స్వయంచాలక &_ : +it)
      {
	సమితి_స్థానభ్రంశం_at (i++, స్థానభ్రంశం);
	స్థానభ్రంశం += hb_len_యొక్క (_);
      }
      సమితి_స్థానభ్రంశం_at (i, స్థానభ్రంశం);
    }
    లేకపోతే
      ఎంపిక (off_కొలత_2)
      {
	స్థితి 1:
	{
	  HBUINT8 *p = (HBUINT8 *) offsets;
	  కొరకు (స్థిరరాశి స్వయంచాలక &_ : +it)
	  {
	    *p++ = స్థానభ్రంశం;
	    స్థానభ్రంశం += hb_len_యొక్క (_);
	  }
	  *p = స్థానభ్రంశం;
	}
	ఆపు;
	స్థితి 2:
	{
	  HBUINT16 *p = (HBUINT16 *) offsets;
	  కొరకు (స్థిరరాశి స్వయంచాలక &_ : +it)
	  {
	    *p++ = స్థానభ్రంశం;
	    స్థానభ్రంశం += hb_len_యొక్క (_);
	  }
	  *p = స్థానభ్రంశం;
	}
	ఆపు;
	స్థితి 3:
	{
	  HBUINT24 *p = (HBUINT24 *) offsets;
	  కొరకు (స్థిరరాశి స్వయంచాలక &_ : +it)
	  {
	    *p++ = స్థానభ్రంశం;
	    స్థానభ్రంశం += hb_len_యొక్క (_);
	  }
	  *p = స్థానభ్రంశం;
	}
	ఆపు;
	స్థితి 4:
	{
	  HBUINT32 *p = (HBUINT32 *) offsets;
	  కొరకు (స్థిరరాశి స్వయంచాలక &_ : +it)
	  {
	    *p++ = స్థానభ్రంశం;
	    స్థానభ్రంశం += hb_len_యొక్క (_);
	  }
	  *p = స్థానభ్రంశం;
	}
	ఆపు;
	అప్రమేయ:
	ఆపు;
      }

    ప్రతిజ్ఞా (స్థానభ్రంశం == దత్తాంశం_కొలత + 1);
    return_trace (నిజం);
  }

  మూస <రకం_పేరు Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  స్థిర గుర్తులేని మొత్తం_కొలత (స్థిరరాశి Iterable &iterable, గుర్తులేని *దత్తాంశం_కొలత = శూన్యసూచిక, గుర్తులేని min_off_కొలత = 0)
  {
    స్వయంచాలక it = + hb_iter (iterable);
    ఒకవేళ (!it)
    {
      ఒకవేళ (దత్తాంశం_కొలత) *దత్తాంశం_కొలత = 0;
      తిరిగి min_కొలత;
    }

    గుర్తులేని మొత్తం = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక &_ : +it)
      మొత్తం += hb_len_యొక్క (_);

    ఒకవేళ (దత్తాంశం_కొలత) *దత్తాంశం_కొలత = మొత్తం;

    గుర్తులేని off_కొలత_2 = (hb_బిట్టు_storage (మొత్తం + 1) + 7) / 8;
    off_కొలత_2 = hb_max(min_off_కొలత, off_కొలత_2);

    తిరిగి min_కొలత + HBUINT8::static_కొలత + (hb_len (it) + 1) * off_కొలత_2 + మొత్తం;
  }

  శూన్య సమితి_స్థానభ్రంశం_at (గుర్తులేని పూర్ణాంకం సూచీ_2, గుర్తులేని పూర్ణాంకం స్థానభ్రంశం)
  {
    ప్రతిజ్ఞా (సూచీ_2 <= లెక్క);

    గుర్తులేని పూర్ణాంకం కొలత = off_కొలత;
    స్థిరరాశి HBUINT8 *p = offsets;
    ఎంపిక (కొలత)
    {
      స్థితి 1: ((HBUINT8  *) p)[సూచీ_2] = స్థానభ్రంశం; ఆపు;
      స్థితి 2: ((HBUINT16 *) p)[సూచీ_2] = స్థానభ్రంశం; ఆపు;
      స్థితి 3: ((HBUINT24 *) p)[సూచీ_2] = స్థానభ్రంశం; ఆపు;
      స్థితి 4: ((HBUINT32 *) p)[సూచీ_2] = స్థానభ్రంశం; ఆపు;
      అప్రమేయ: తిరిగి;
    }
  }

  అంతరంగ:
  గుర్తులేని పూర్ణాంకం స్థానభ్రంశం_at (గుర్తులేని పూర్ణాంకం సూచీ_2) స్థిరరాశి
  {
    ప్రతిజ్ఞా (సూచీ_2 <= లెక్క);

    గుర్తులేని పూర్ణాంకం కొలత = off_కొలత;
    స్థిరరాశి HBUINT8 *p = offsets;
    ఎంపిక (కొలత)
    {
      స్థితి 1: తిరిగి ((HBUINT8  *) p)[సూచీ_2];
      స్థితి 2: తిరిగి ((HBUINT16 *) p)[సూచీ_2];
      స్థితి 3: తిరిగి ((HBUINT24 *) p)[సూచీ_2];
      స్థితి 4: తిరిగి ((HBUINT32 *) p)[సూచీ_2];
      అప్రమేయ: తిరిగి 0;
    }
  }

  స్థిరరాశి గుర్తులేని అక్షరం *దత్తాంశం_పునాది () స్థిరరాశి
  { తిరిగి (స్థిరరాశి గుర్తులేని అక్షరం *) ఇది + min_కొలత + off_కొలత.static_కొలత - 1 + స్థానభ్రంశం_array_కొలత (); }
  బహిరంగ:

  hb_ubytes_t క్రియాకారకం [] (గుర్తులేని పూర్ణాంకం సూచీ_2) స్థిరరాశి
  {
    ఒకవేళ (unlikely (సూచీ_2 >= లెక్క)) తిరిగి hb_ubytes_t ();
    hb_barrier ();
    గుర్తులేని offset0 = స్థానభ్రంశం_at (సూచీ_2);
    గుర్తులేని offset1 = స్థానభ్రంశం_at (సూచీ_2 + 1);
    ఒకవేళ (unlikely (offset1 < offset0 || offset1 > స్థానభ్రంశం_at (లెక్క)))
      తిరిగి hb_ubytes_t ();
    తిరిగి hb_ubytes_t (దత్తాంశం_పునాది () + offset0, offset1 - offset0);
  }

  size_t get_కొలత () స్థిరరాశి
  {
    ఒకవేళ (లెక్క)
      తిరిగి hb_unsigned_కూడు_saturate (min_కొలత, off_కొలత.static_కొలత,
				       స్థానభ్రంశం_array_కొలత (), స్థానభ్రంశం_at (లెక్క) - 1);
    తిరిగి min_కొలత;  /* empty CFFIndex contains count only */
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  hb_barrier () &&
			  (లెక్క == 0 || /* empty INDEX */
			   (లెక్క < లెక్క + 1u &&
			    c->check_struct (&off_కొలత) && off_కొలత >= 1 && off_కొలత <= 4 &&
			    c->check_array (offsets, off_కొలత, లెక్క + 1u) &&
			    c->check_range (దత్తాంశం_పునాది (), స్థానభ్రంశం_at (లెక్క))))));
  }

  బహిరంగ:
  లెక్క_3		లెక్క;		/* Number of object data. Note there are (count+1) offsets */
  అంతరంగ:
  HBUINT8	off_కొలత;	/* The byte size of each offset in the offsets array. */
  HBUINT8	offsets[HB_VAR_ARRAY];
				/* The array of (count + 1) offsets into objects array (1-base). */
  /* HBUINT8 data[HB_VAR_ARRAY];	Object data */
  బహిరంగ:
  DEFINE_కొలత_MIN (లెక్క_3::static_కొలత);
};
రకనిర్వచనం CFFIndex<HBUINT16> CFF1_సూచీ;
రకనిర్వచనం CFFIndex<HBUINT32> CFF2_సూచీ;


/* TupleValues */
నిర్మాణం Tuple_విలువలు
{
  గణన packed_విలువ_flag_t
  {
    విలువలు_ARE_ZEROS     = 0x80,
    విలువలు_ARE_బైట్లు     = 0x00,
    విలువలు_ARE_పదాలు     = 0x40,
    విలువలు_ARE_LONGS     = 0xC0,
    విలువలు_కొలత_ముసుగు     = 0xC0,
    విలువ_నిరంతరం_లెక్క_ముసుగు = 0x3F
  };

  స్థిర గుర్తులేని compile_unsafe (hb_array_t<స్థిరరాశి పూర్ణాంకం> values, /*లోపల*/
				  గుర్తులేని అక్షరం *encoded_బైట్లు /*బహిర్గతం*/)
  {
    గుర్తులేని అంకె_విలువలు_3 = values.నిడివి;
    గుర్తులేని encoded_len = 0;
    గుర్తులేని i = 0;
    అయితే (i < అంకె_విలువలు_3)
    {
      పూర్ణాంకం val = values.arrayZ[i];
      ఒకవేళ (val == 0)
        encoded_len += encode_విలువ_నిరంతరం_as_zeroes (i, encoded_బైట్లు + encoded_len, values);
      లేకపోతే ఒకవేళ ((int8_t) val == val)
        encoded_len += encode_విలువ_నిరంతరం_as_బైట్లు (i, encoded_బైట్లు + encoded_len, values);
      లేకపోతే ఒకవేళ ((int16_t) val == val)
        encoded_len += encode_విలువ_నిరంతరం_as_పదాలు (i, encoded_బైట్లు + encoded_len, values);
      లేకపోతే
        encoded_len += encode_విలువ_నిరంతరం_as_longs (i, encoded_బైట్లు + encoded_len, values);
    }
    తిరిగి encoded_len;
  }

  స్థిర గుర్తులేని encode_విలువ_నిరంతరం_as_zeroes (గుర్తులేని& i,
					      గుర్తులేని అక్షరం *it,
					      hb_array_t<స్థిరరాశి పూర్ణాంకం> values)
  {
    గుర్తులేని అంకె_విలువలు_3 = values.నిడివి;
    గుర్తులేని నిరంతరం_నిడివి = 0;
    గుర్తులేని encoded_len = 0;
    అయితే (i < అంకె_విలువలు_3 && values.arrayZ[i] == 0)
    {
      i++;
      నిరంతరం_నిడివి++;
    }

    అయితే (నిరంతరం_నిడివి >= 64)
    {
      *it++ = అక్షరం (విలువలు_ARE_ZEROS | 63);
      నిరంతరం_నిడివి -= 64;
      encoded_len++;
    }

    ఒకవేళ (నిరంతరం_నిడివి)
    {
      *it++ = అక్షరం (విలువలు_ARE_ZEROS | (నిరంతరం_నిడివి - 1));
      encoded_len++;
    }
    తిరిగి encoded_len;
  }

  స్థిర గుర్తులేని encode_విలువ_నిరంతరం_as_బైట్లు (గుర్తులేని &i,
					     గుర్తులేని అక్షరం *it,
					     hb_array_t<స్థిరరాశి పూర్ణాంకం> values)
  {
    గుర్తులేని ప్రారంభం = i;
    గుర్తులేని అంకె_విలువలు_3 = values.నిడివి;
    అయితే (i < అంకె_విలువలు_3)
    {
      పూర్ణాంకం val = values.arrayZ[i];
      ఒకవేళ ((int8_t) val != val)
        ఆపు;

      /* from fonttools: if there're 2 or more zeros in a sequence,
       * it is better to start a new run to save bytes. */
      ఒకవేళ (val == 0 && i + 1 < అంకె_విలువలు_3 && values.arrayZ[i+1] == 0)
        ఆపు;

      i++;
    }
    గుర్తులేని నిరంతరం_నిడివి = i - ప్రారంభం;

    గుర్తులేని encoded_len = 0;

    అయితే (నిరంతరం_నిడివి >= 64)
    {
      *it++ = (విలువలు_ARE_బైట్లు | 63);
      encoded_len++;

      కొరకు (గుర్తులేని j = 0; j < 64; j++)
	it[j] = static_cast<అక్షరం> (values.arrayZ[ప్రారంభం + j]);
      it += 64;
      encoded_len += 64;

      ప్రారంభం += 64;
      నిరంతరం_నిడివి -= 64;
    }

    ఒకవేళ (నిరంతరం_నిడివి)
    {
      *it++ = (విలువలు_ARE_బైట్లు | (నిరంతరం_నిడివి - 1));
      encoded_len++;

      కొరకు (గుర్తులేని j = 0; j < నిరంతరం_నిడివి; j++)
        it[j] = static_cast<అక్షరం> (values.arrayZ[ప్రారంభం + j]);
      encoded_len += నిరంతరం_నిడివి;
    }

    తిరిగి encoded_len;
  }

  స్థిర గుర్తులేని encode_విలువ_నిరంతరం_as_పదాలు (గుర్తులేని &i,
					     గుర్తులేని అక్షరం *it,
					     hb_array_t<స్థిరరాశి పూర్ణాంకం> values)
  {
    గుర్తులేని ప్రారంభం = i;
    గుర్తులేని అంకె_విలువలు_3 = values.నిడివి;
    అయితే (i < అంకె_విలువలు_3)
    {
      పూర్ణాంకం val = values.arrayZ[i];

      ఒకవేళ ((int16_t) val != val)
        ఆపు;

      /* start a new run for a single zero value. */
      ఒకవేళ (val == 0) ఆపు;

      /* From fonttools: continue word-encoded run if there's only one
       * single value in the range [-128, 127] because it is more compact.
       * Only start a new run when there're 2 continuous such values. */
      ఒకవేళ ((int8_t) val == val &&
          i + 1 < అంకె_విలువలు_3 &&
          (int8_t) values.arrayZ[i+1] == values.arrayZ[i+1])
        ఆపు;

      i++;
    }

    గుర్తులేని నిరంతరం_నిడివి = i - ప్రారంభం;
    గుర్తులేని encoded_len = 0;
    అయితే (నిరంతరం_నిడివి >= 64)
    {
      *it++ = (విలువలు_ARE_పదాలు | 63);
      encoded_len++;

      కొరకు (గుర్తులేని j = 0; j < 64; j++)
      {
        int16_t విలువ_val = values.arrayZ[ప్రారంభం + j];
        *it++ = static_cast<అక్షరం> (విలువ_val >> 8);
        *it++ = static_cast<అక్షరం> (విలువ_val & 0xFF);

        encoded_len += 2;
      }

      ప్రారంభం += 64;
      నిరంతరం_నిడివి -= 64;
    }

    ఒకవేళ (నిరంతరం_నిడివి)
    {
      *it++ = (విలువలు_ARE_పదాలు | (నిరంతరం_నిడివి - 1));
      encoded_len++;
      అయితే (ప్రారంభం < i)
      {
        int16_t విలువ_val = values.arrayZ[ప్రారంభం++];
        *it++ = static_cast<అక్షరం> (విలువ_val >> 8);
        *it++ = static_cast<అక్షరం> (విలువ_val & 0xFF);

        encoded_len += 2;
      }
    }
    తిరిగి encoded_len;
  }

  స్థిర గుర్తులేని encode_విలువ_నిరంతరం_as_longs (గుర్తులేని &i,
					     గుర్తులేని అక్షరం *it,
					     hb_array_t<స్థిరరాశి పూర్ణాంకం> values)
  {
    గుర్తులేని ప్రారంభం = i;
    గుర్తులేని అంకె_విలువలు_3 = values.నిడివి;
    అయితే (i < అంకె_విలువలు_3)
    {
      పూర్ణాంకం val = values.arrayZ[i];

      ఒకవేళ ((int16_t) val == val)
        ఆపు;

      i++;
    }

    గుర్తులేని నిరంతరం_నిడివి = i - ప్రారంభం;
    గుర్తులేని encoded_len = 0;
    అయితే (నిరంతరం_నిడివి >= 64)
    {
      *it++ = (విలువలు_ARE_LONGS | 63);
      encoded_len++;

      కొరకు (గుర్తులేని j = 0; j < 64; j++)
      {
        int32_t విలువ_val = values.arrayZ[ప్రారంభం + j];
        *it++ = static_cast<అక్షరం> (విలువ_val >> 24);
        *it++ = static_cast<అక్షరం> (విలువ_val >> 16);
        *it++ = static_cast<అక్షరం> (విలువ_val >> 8);
        *it++ = static_cast<అక్షరం> (విలువ_val & 0xFF);

        encoded_len += 4;
      }

      ప్రారంభం += 64;
      నిరంతరం_నిడివి -= 64;
    }

    ఒకవేళ (నిరంతరం_నిడివి)
    {
      *it++ = (విలువలు_ARE_LONGS | (నిరంతరం_నిడివి - 1));
      encoded_len++;
      అయితే (ప్రారంభం < i)
      {
        int32_t విలువ_val = values.arrayZ[ప్రారంభం++];
        *it++ = static_cast<అక్షరం> (విలువ_val >> 24);
        *it++ = static_cast<అక్షరం> (విలువ_val >> 16);
        *it++ = static_cast<అక్షరం> (విలువ_val >> 8);
        *it++ = static_cast<అక్షరం> (విలువ_val & 0xFF);

        encoded_len += 4;
      }
    }
    తిరిగి encoded_len;
  }

  మూస <రకం_పేరు T>
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  స్థిర తార్కిక decompile (స్థిరరాశి HBUINT8 *&p /* IN/OUT */,
			 hb_సదిశ_t<T> &values /* IN/OUT */,
			 స్థిరరాశి HBUINT8 *చివర,
			 తార్కిక consume_all = అబద్ధం,
			 గుర్తులేని ప్రారంభం = 0)
  {
    గుర్తులేని i = 0;
    గుర్తులేని లెక్క = consume_all ? UINT_MAX : values.నిడివి;
    ఒకవేళ (consume_all)
      values.alloc ((చివర - p) / 2);
    అయితే (i < లెక్క)
    {
      ఒకవేళ (unlikely (p + 1 > చివర)) తిరిగి consume_all;
      గుర్తులేని control = *p++;
      గుర్తులేని నిరంతరం_లెక్క = (control & విలువ_నిరంతరం_లెక్క_ముసుగు) + 1;
      ఒకవేళ (consume_all)
      {
        ఒకవేళ (unlikely (!values.resize_dirty  (values.నిడివి + నిరంతరం_లెక్క)))
	  తిరిగి అబద్ధం;
      }
      గుర్తులేని stop = i + నిరంతరం_లెక్క;
      ఒకవేళ (unlikely (stop > లెక్క)) తిరిగి అబద్ధం;

      గుర్తులేని skip = i < ప్రారంభం ? hb_min (ప్రారంభం - i, నిరంతరం_లెక్క) : 0;
      i += skip;

      ఒకవేళ ((control & విలువలు_కొలత_ముసుగు) == విలువలు_ARE_ZEROS)
      {
	hb_memset (&values.arrayZ[i], 0, (stop - i) * పరిమాణం (T));
	i = stop;
      }
      లేకపోతే ఒకవేళ ((control & విలువలు_కొలత_ముసుగు) ==  విలువలు_ARE_పదాలు)
      {
        ఒకవేళ (unlikely (p + నిరంతరం_లెక్క * HBINT16::static_కొలత > చివర)) తిరిగి అబద్ధం;
	p += skip * HBINT16::static_కొలత;
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
        కొరకు (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (స్థిరరాశి HBINT16 *) p;
	  p += HBINT16::static_కొలత;
	  values.arrayZ[i + 1] = * (స్థిరరాశి HBINT16 *) p;
	  p += HBINT16::static_కొలత;
	  values.arrayZ[i + 2] = * (స్థిరరాశి HBINT16 *) p;
	  p += HBINT16::static_కొలత;
	  values.arrayZ[i + 3] = * (స్థిరరాశి HBINT16 *) p;
	  p += HBINT16::static_కొలత;
	}
#అయితే_ముగింపు
        కొరకు (; i < stop; i++)
        {
          values.arrayZ[i] = * (స్థిరరాశి HBINT16 *) p;
          p += HBINT16::static_కొలత;
        }
      }
      లేకపోతే ఒకవేళ ((control & విలువలు_కొలత_ముసుగు) ==  విలువలు_ARE_LONGS)
      {
        ఒకవేళ (unlikely (p + నిరంతరం_లెక్క * HBINT32::static_కొలత > చివర)) తిరిగి అబద్ధం;
	p += skip * HBINT32::static_కొలత;
        కొరకు (; i < stop; i++)
        {
          values.arrayZ[i] = * (స్థిరరాశి HBINT32 *) p;
          p += HBINT32::static_కొలత;
        }
      }
      లేకపోతే ఒకవేళ ((control & విలువలు_కొలత_ముసుగు) ==  విలువలు_ARE_బైట్లు)
      {
        ఒకవేళ (unlikely (p + నిరంతరం_లెక్క > చివర)) తిరిగి అబద్ధం;
	p += skip * HBINT8::static_కొలత;
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
	కొరకు (; i + 3 < stop; i += 4)
	{
	  values.arrayZ[i] = * (స్థిరరాశి HBINT8 *) p++;
	  values.arrayZ[i + 1] = * (స్థిరరాశి HBINT8 *) p++;
	  values.arrayZ[i + 2] = * (స్థిరరాశి HBINT8 *) p++;
	  values.arrayZ[i + 3] = * (స్థిరరాశి HBINT8 *) p++;
	}
#అయితే_ముగింపు
        కొరకు (; i < stop; i++)
          values.arrayZ[i] = * (స్థిరరాశి HBINT8 *) p++;
      }
    }
    తిరిగి నిజం;
  }

  స్థిర తార్కిక skip (స్థిరరాశి గుర్తులేని అక్షరం *&p /* IN/OUT */,
		    స్థిరరాశి గుర్తులేని అక్షరం *చివర,
		    గుర్తులేని లెక్క)
  {
    గుర్తులేని చూసిన = 0;
    అయితే (చూసిన < లెక్క)
    {
      ఒకవేళ (unlikely (p >= చివర)) తిరిగి అబద్ధం;
      గుర్తులేని control = *p++;
      గుర్తులేని నిరంతరం_లెక్క = (control & విలువ_నిరంతరం_లెక్క_ముసుగు) + 1;
      ఒకవేళ (unlikely (నిరంతరం_లెక్క > లెక్క - చూసిన)) తిరిగి అబద్ధం;

      గుర్తులేని వెడల్పు = 0;
      ఎంపిక (control & విలువలు_కొలత_ముసుగు)
      {
	స్థితి విలువలు_ARE_ZEROS: ఆపు;
	స్థితి విలువలు_ARE_బైట్లు: వెడల్పు = HBINT8::static_కొలత; ఆపు;
	స్థితి విలువలు_ARE_పదాలు: వెడల్పు = HBINT16::static_కొలత; ఆపు;
	స్థితి విలువలు_ARE_LONGS: వెడల్పు = HBINT32::static_కొలత; ఆపు;
      }
      ఒకవేళ (unlikely (గుర్తులేని (చివర - p) < నిరంతరం_లెక్క * వెడల్పు)) తిరిగి అబద్ధం;
      p += నిరంతరం_లెక్క * వెడల్పు;
      చూసిన += నిరంతరం_లెక్క;
    }
    తిరిగి నిజం;
  }

  నిర్మాణం iter_t : hb_iter_with_fallback_t<iter_t, పూర్ణాంకం>
  {
    iter_t (స్థిరరాశి గుర్తులేని అక్షరం *p_, గుర్తులేని len_)
	    : p (p_), endp (p_ + len_)
    { ఒకవేళ (likely (ensure_నిరంతరం ())) read_విలువ (); }

    అంతరంగ:
    స్థిరరాశి గుర్తులేని అక్షరం *p;
    స్థిరరాశి గుర్తులేని అక్షరం * స్థిరరాశి endp;
    పూర్ణాంకం current_విలువ = 0;
    గుర్తుగల నిరంతరం_లెక్క = 0;
    గుర్తులేని వెడల్పు = 0;

    HB_ALWAYS_INLINE
    తార్కిక ensure_నిరంతరం ()
    {
      ఒకవేళ (likely (నిరంతరం_లెక్క > 0)) తిరిగి నిజం;
      తిరిగి ensure_నిరంతరం_2 ();
    }
    తార్కిక ensure_నిరంతరం_2 ()
    {
      ఒకవేళ (unlikely (p >= endp))
      {
        నిరంతరం_లెక్క = 0;
        current_విలువ = 0;
	తిరిగి అబద్ధం;
      }

      గుర్తులేని control = *p++;
      నిరంతరం_లెక్క = (control & విలువ_నిరంతరం_లెక్క_ముసుగు) + 1;
      వెడల్పు = control & విలువలు_కొలత_ముసుగు;
      ఎంపిక (వెడల్పు)
      {
        స్థితి విలువలు_ARE_ZEROS: వెడల్పు = 0; ఆపు;
	స్థితి విలువలు_ARE_బైట్లు: వెడల్పు = HBINT8::static_కొలత;  ఆపు;
	స్థితి విలువలు_ARE_పదాలు: వెడల్పు = HBINT16::static_కొలత; ఆపు;
	స్థితి విలువలు_ARE_LONGS: వెడల్పు = HBINT32::static_కొలత; ఆపు;
	అప్రమేయ: ప్రతిజ్ఞా (అబద్ధం);
      }

      ఒకవేళ (unlikely (p + నిరంతరం_లెక్క * వెడల్పు > endp))
      {
	నిరంతరం_లెక్క = 0;
	current_విలువ = 0;
	తిరిగి అబద్ధం;
      }

      తిరిగి నిజం;
    }
    శూన్య read_విలువ ()
    {
      ఎంపిక (వెడల్పు)
      {
        స్థితి 0: current_విలువ = 0; ఆపు;
	స్థితి 1: current_విలువ = * (స్థిరరాశి HBINT8  *) p; ఆపు;
	స్థితి 2: current_విలువ = * (స్థిరరాశి HBINT16 *) p; ఆపు;
	స్థితి 4: current_విలువ = * (స్థిరరాశి HBINT32 *) p; ఆపు;
      }
      p += వెడల్పు;
    }

    బహిరంగ:

    రకనిర్వచనం పూర్ణాంకం __item_t__;
    __item_t__ __item__ () స్థిరరాశి
    { తిరిగి current_విలువ; }

    తార్కిక __more__ () స్థిరరాశి { తిరిగి నిరంతరం_లెక్క || p < endp; }
    శూన్య __next__ ()
    {
      నిరంతరం_లెక్క--;
      ఒకవేళ (unlikely (!ensure_నిరంతరం ()))
	తిరిగి;
      read_విలువ ();
    }
    శూన్య __forward__ (గుర్తులేని n)
    {
      ఒకవేళ (unlikely (!ensure_నిరంతరం ()))
	తిరిగి;
      అయితే (n)
      {
	గుర్తులేని i = hb_min (n, (గుర్తులేని) నిరంతరం_లెక్క);
	నిరంతరం_లెక్క -= i;
	n -= i;
	p += (i - 1) * వెడల్పు;
	ఒకవేళ (unlikely (!ensure_నిరంతరం ()))
	  తిరిగి;
	read_విలువ ();
      }
    }
    తార్కిక క్రియాకారకం != (స్థిరరాశి iter_t& o) స్థిరరాశి
    { తిరిగి p != o.p || నిరంతరం_లెక్క != o.నిరంతరం_లెక్క; }
    iter_t __end__ () స్థిరరాశి
    {
      iter_t it (endp, 0);
      తిరిగి it;
    }
  };

  నిర్మాణం fetcher_t
  {
    fetcher_t (స్థిరరాశి గుర్తులేని అక్షరం *p_, గుర్తులేని len_)
	      : p (p_), చివర (p_ + len_) {}

    అంతరంగ:
    స్థిరరాశి గుర్తులేని అక్షరం *p;
    స్థిరరాశి గుర్తులేని అక్షరం * స్థిరరాశి చివర;
    గుర్తుగల నిరంతరం_లెక్క = 0;
    గుర్తులేని వెడల్పు = 0;

    HB_ALWAYS_INLINE
    తార్కిక ensure_నిరంతరం ()
    {
      ఒకవేళ (likely (నిరంతరం_లెక్క > 0)) తిరిగి నిజం;
      తిరిగి ensure_నిరంతరం_2 ();
    }

    తార్కిక ensure_నిరంతరం_2 ()
    {
      ఒకవేళ (unlikely (p >= చివర))
      {
        నిరంతరం_లెక్క = 0;
	తిరిగి అబద్ధం;
      }

      గుర్తులేని control = *p++;
      నిరంతరం_లెక్క = (control & విలువ_నిరంతరం_లెక్క_ముసుగు) + 1;
      వెడల్పు = control & విలువలు_కొలత_ముసుగు;
      ఎంపిక (వెడల్పు)
      {
        స్థితి విలువలు_ARE_ZEROS: వెడల్పు = 0; ఆపు;
	స్థితి విలువలు_ARE_బైట్లు: వెడల్పు = HBINT8::static_కొలత;  ఆపు;
	స్థితి విలువలు_ARE_పదాలు: వెడల్పు = HBINT16::static_కొలత; ఆపు;
	స్థితి విలువలు_ARE_LONGS: వెడల్పు = HBINT32::static_కొలత; ఆపు;
	అప్రమేయ: ప్రతిజ్ఞా (అబద్ధం);
      }

      ఒకవేళ (unlikely (p + నిరంతరం_లెక్క * వెడల్పు > చివర))
      {
	నిరంతరం_లెక్క = 0;
	తిరిగి అబద్ధం;
      }

      తిరిగి నిజం;
    }

    బహిరంగ:
    శూన్య skip (గుర్తులేని n)
    {
      అయితే (n)
      {
	ఒకవేళ (unlikely (!ensure_నిరంతరం ()))
	  తిరిగి;
	గుర్తులేని i = hb_min (n, (గుర్తులేని) నిరంతరం_లెక్క);
	నిరంతరం_లెక్క -= i;
	n -= i;
	p += i * వెడల్పు;
      }
    }

    అంతరంగ:
    మూస <తార్కిక scaled, రకం_పేరు T>
    శూన్య కూడు_గమ్యం (hb_array_t<T> బహిర్గతం, T scale = T (1))
    {
      గుర్తులేని n = బహిర్గతం.నిడివి;
      T *arrayZ = బహిర్గతం.arrayZ;

      కొరకు (గుర్తులేని i = 0; i < n;)
      {
	ఒకవేళ (unlikely (!ensure_నిరంతరం ()))
	  ఆపు;
	గుర్తులేని లెక్క = hb_min (n - i, (గుర్తులేని) నిరంతరం_లెక్క);
	ఎంపిక (వెడల్పు)
	{
	  స్థితి 0:
	  {
	    arrayZ += లెక్క;
	    ఆపు;
	  }
	  స్థితి 1:
	  {
	    స్థిరరాశి స్వయంచాలక *pp = (స్థిరరాశి HBINT8 *) p;
	    గుర్తులేని j = 0;
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
	    కొరకు (; j + 3 < లెక్క; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#అయితే_ముగింపు
	    కొరకు (; j < లెక్క; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (స్థిరరాశి గుర్తులేని అక్షరం *) pp;
	  }
	  ఆపు;
	  స్థితి 2:
	  {
	    స్థిరరాశి స్వయంచాలక *pp = (స్థిరరాశి HBINT16 *) p;
	    గుర్తులేని j = 0;
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
	    కొరకు (; j + 3 < లెక్క; j += 4)
	    {
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;
	    }
#అయితే_ముగింపు
	    కొరకు (; j < లెక్క; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (స్థిరరాశి గుర్తులేని అక్షరం *) pp;
	  }
	  ఆపు;
	  స్థితి 4:
	  {
	    స్థిరరాశి స్వయంచాలక *pp = (స్థిరరాశి HBINT32 *) p;
	    కొరకు (గుర్తులేని j = 0; j < లెక్క; j++)
	      *arrayZ++ += scaled ? *pp++ * scale : *pp++;

	    p = (స్థిరరాశి గుర్తులేని అక్షరం *) pp;
	  }
	  ఆపు;
	}
	నిరంతరం_లెక్క -= లెక్క;
	i += లెక్క;
      }
    }

    బహిరంగ:
    మూస <రకం_పేరు T>
    శూన్య కూడు_గమ్యం_2 (hb_array_t<T> బహిర్గతం, దశాంశం scale = 1.0f)
    {
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
      // The following branch is supposed to speed things up by avoiding
      // the multiplication in _add_to<> if scale is 1.0f.
      // But in practice it seems to bloat the code and slow things down.
      ఒకవేళ (అబద్ధం && scale == 1.0f)
        కూడు_గమ్యం<అబద్ధం> (బహిర్గతం);
      లేకపోతే
#అయితే_ముగింపు
        కూడు_గమ్యం<నిజం> (బహిర్గతం, (T) scale);
    }
  };
};

నిర్మాణం TupleList : CFF2_సూచీ
{
  Tuple_విలువలు::iter_t క్రియాకారకం [] (గుర్తులేని i) స్థిరరాశి
  {
    స్వయంచాలక బైట్లు = CFF2_సూచీ::క్రియాకారకం [] (i);
    తిరిగి Tuple_విలువలు::iter_t (బైట్లు.arrayZ, బైట్లు.నిడివి);
  }

  Tuple_విలువలు::fetcher_t fetcher (గుర్తులేని i) స్థిరరాశి
  {
    స్వయంచాలక బైట్లు = CFF2_సూచీ::క్రియాకారకం [] (i);
    తిరిగి Tuple_విలువలు::fetcher_t (బైట్లు.arrayZ, బైట్లు.నిడివి);
  }
};


// Alignment

మూస <గుర్తులేని పూర్ణాంకం alignment>
నిర్మాణం Align
{
  size_t get_కొలత (స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    గుర్తులేని స్థానభ్రంశం = (స్థిరరాశి అక్షరం *) ఇది - (స్థిరరాశి అక్షరం *) పునాది;
    తిరిగి (alignment - స్థానభ్రంశం) & (alignment - 1);
  }

  బహిరంగ:
  DEFINE_కొలత_MIN (0);
};



} /* namespace OT */


#అయితే_ముగింపు /* HB_OPEN_TYPE_HH */
