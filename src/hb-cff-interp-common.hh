/*
 * Copyright © 2018 Adobe Inc.
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
 * Adobe Author(s): Michiharu Ariza
 */
#ifndef HB_CFF_INTERP_COMMON_HH
#define HB_CFF_INTERP_COMMON_HH

বাহ্যিক HB_INTERNAL ধ্রুবক unsigned অক্ষর *endchar_str;

নামাকাশ CFF {

using নামাকাশ OT;

প্রকারলেখ unsigned পূর্ণসংখ্যা op_code_t;


/* === Dict operators === */

/* One byte operators (0-31) */
#define OpCode_version		  0 /* CFF Top */
#define OpCode_Notice		  1 /* CFF Top */
#define OpCode_FullName		  2 /* CFF Top */
#define OpCode_FamilyName	  3 /* CFF Top */
#define OpCode_Weight		  4 /* CFF Top */
#define OpCode_FontBBox		  5 /* CFF Top */
#define Op_Code_Blue_মানসমূহ	  6 /* CFF Private, CFF2 Private */
#define OpCode_OtherBlues	  7 /* CFF Private, CFF2 Private */
#define OpCode_FamilyBlues	  8 /* CFF Private, CFF2 Private */
#define OpCode_FamilyOtherBlues	  9 /* CFF Private, CFF2 Private */
#define OpCode_StdHW		 10 /* CFF Private, CFF2 Private */
#define OpCode_StdVW		 11 /* CFF Private, CFF2 Private */
#define OpCode_escape		 12 /* All. Shared with CS */
#define OpCode_UniqueID		 13 /* CFF Top */
#define OpCode_XUID		 14 /* CFF Top */
#define OpCode_charset		 15 /* CFF Top (0) */
#define OpCode_Encoding		 16 /* CFF Top (0) */
#define OpCode_CharStrings	 17 /* CFF Top, CFF2 Top */
#define OpCode_Private		 18 /* CFF Top, CFF2 FD */
#define OpCode_Subrs		 19 /* CFF Private, CFF2 Private */
#define OpCode_defaultWidthX	 20 /* CFF Private (0) */
#define OpCode_nominalWidthX	 21 /* CFF Private (0) */
#define OpCode_vsindexdict	 22 /* CFF2 Private/CS */
#define OpCode_blenddict	 23 /* CFF2 Private/CS */
#define OpCode_vstore		 24 /* CFF2 Top */
#define OpCode_reserved25	 25
#define OpCode_reserved26	 26
#define OpCode_reserved27	 27

/* Numbers */
#define OpCode_shortint		 28 /* 16-bit integer, All */
#define OpCode_longintdict	 29 /* 32-bit integer, All */
#define OpCode_BCD		 30 /* Real number, CFF2 Top/FD */
#define OpCode_reserved31	 31

/* 1-byte integers */
#define OpCode_OneByteIntFirst	 32 /* All. beginning of the range of first byte ints */
#define OpCode_OneByteIntLast	246 /* All. ending of the range of first byte int */

/* 2-byte integers */
#define OpCode_TwoBytePosInt0	247 /* All. first byte of two byte positive int (+108 to +1131) */
#define OpCode_TwoBytePosInt1	248
#define OpCode_TwoBytePosInt2	249
#define OpCode_TwoBytePosInt3	250

#define OpCode_TwoByteNegInt0	251 /* All. first byte of two byte negative int (-1131 to -108) */
#define OpCode_TwoByteNegInt1	252
#define OpCode_TwoByteNegInt2	253
#define OpCode_TwoByteNegInt3	254

/* Two byte escape operators 12, (0-41) */
#define OpCode_ESC_Base		256
#define Make_OpCode_ESC(byte2)	((op_code_t)(OpCode_ESC_Base + (byte2)))

অন্তঃরেখ op_code_t Unmake_OpCode_ESC (op_code_t op)  { ফেরত (op_code_t)(op - OpCode_ESC_Base); }
অন্তঃরেখ যৌক্তিক Is_OpCode_ESC (op_code_t op) { ফেরত op >= OpCode_ESC_Base; }
অন্তঃরেখ unsigned পূর্ণসংখ্যা Op_Code_আকার (op_code_t op) { ফেরত Is_OpCode_ESC (op) ? 2: 1; }

#define OpCode_Copyright	Make_OpCode_ESC(0) /* CFF Top */
#define OpCode_isFixedPitch	Make_OpCode_ESC(1) /* CFF Top (false) */
#define OpCode_ItalicAngle	Make_OpCode_ESC(2) /* CFF Top (0) */
#define OpCode_UnderlinePosition Make_OpCode_ESC(3) /* CFF Top (-100) */
#define OpCode_UnderlineThickness Make_OpCode_ESC(4) /* CFF Top (50) */
#define OpCode_PaintType	Make_OpCode_ESC(5) /* CFF Top (0) */
#define OpCode_CharstringType	Make_OpCode_ESC(6) /* CFF Top (2) */
#define OpCode_FontMatrix	Make_OpCode_ESC(7) /* CFF Top, CFF2 Top (.001 0 0 .001 0 0)*/
#define OpCode_StrokeWidth	Make_OpCode_ESC(8) /* CFF Top (0) */
#define OpCode_BlueScale	Make_OpCode_ESC(9) /* CFF Private, CFF2 Private (0.039625) */
#define OpCode_BlueShift	Make_OpCode_ESC(10) /* CFF Private, CFF2 Private (7) */
#define OpCode_BlueFuzz		Make_OpCode_ESC(11) /* CFF Private, CFF2 Private (1) */
#define OpCode_StemSnapH	Make_OpCode_ESC(12) /* CFF Private, CFF2 Private */
#define OpCode_StemSnapV	Make_OpCode_ESC(13) /* CFF Private, CFF2 Private */
#define OpCode_ForceBold	Make_OpCode_ESC(14) /* CFF Private (false) */
#define OpCode_reservedESC15	Make_OpCode_ESC(15)
#define OpCode_reservedESC16	Make_OpCode_ESC(16)
#define OpCode_LanguageGroup	Make_OpCode_ESC(17) /* CFF Private, CFF2 Private (0) */
#define OpCode_ExpansionFactor	Make_OpCode_ESC(18) /* CFF Private, CFF2 Private (0.06) */
#define OpCode_initialRandomSeed Make_OpCode_ESC(19) /* CFF Private (0) */
#define OpCode_SyntheticBase	Make_OpCode_ESC(20) /* CFF Top */
#define OpCode_PostScript	Make_OpCode_ESC(21) /* CFF Top */
#define OpCode_BaseFontName	Make_OpCode_ESC(22) /* CFF Top */
#define OpCode_BaseFontBlend	Make_OpCode_ESC(23) /* CFF Top */
#define OpCode_reservedESC24	Make_OpCode_ESC(24)
#define OpCode_reservedESC25	Make_OpCode_ESC(25)
#define OpCode_reservedESC26	Make_OpCode_ESC(26)
#define OpCode_reservedESC27	Make_OpCode_ESC(27)
#define OpCode_reservedESC28	Make_OpCode_ESC(28)
#define OpCode_reservedESC29	Make_OpCode_ESC(29)
#define OpCode_ROS		Make_OpCode_ESC(30) /* CFF Top_CID */
#define OpCode_CIDFontVersion	Make_OpCode_ESC(31) /* CFF Top_CID (0) */
#define OpCode_CIDFontRevision	Make_OpCode_ESC(32) /* CFF Top_CID (0) */
#define OpCode_CIDFontType	Make_OpCode_ESC(33) /* CFF Top_CID (0) */
#define OpCode_CIDCount		Make_OpCode_ESC(34) /* CFF Top_CID (8720) */
#define OpCode_UIDBase		Make_OpCode_ESC(35) /* CFF Top_CID */
#define OpCode_FDArray		Make_OpCode_ESC(36) /* CFF Top_CID, CFF2 Top */
#define OpCode_FDSelect		Make_OpCode_ESC(37) /* CFF Top_CID, CFF2 Top */
#define OpCode_FontName		Make_OpCode_ESC(38) /* CFF Top_CID */


/* === CharString operators === */

#define OpCode_hstem		  1 /* CFF, CFF2 */
#define OpCode_Reserved2	  2
#define OpCode_vstem		  3 /* CFF, CFF2 */
#define OpCode_vmoveto		  4 /* CFF, CFF2 */
#define OpCode_rlineto		  5 /* CFF, CFF2 */
#define OpCode_hlineto		  6 /* CFF, CFF2 */
#define OpCode_vlineto		  7 /* CFF, CFF2 */
#define OpCode_rrcurveto	  8 /* CFF, CFF2 */
#define OpCode_Reserved9	  9
#define OpCode_callsubr		 10 /* CFF, CFF2 */
#define OpCode_return		 11 /* CFF */
//#define OpCode_escape		 12 /* CFF, CFF2 */
#define OpCode_Reserved13	 13
#define OpCode_endchar		 14 /* CFF */
#define OpCode_vsindexcs	 15 /* CFF2 */
#define OpCode_blendcs		 16 /* CFF2 */
#define OpCode_Reserved17	 17
#define OpCode_hstemhm		 18 /* CFF, CFF2 */
#define OpCode_hintmask		 19 /* CFF, CFF2 */
#define OpCode_cntrmask		 20 /* CFF, CFF2 */
#define OpCode_rmoveto		 21 /* CFF, CFF2 */
#define OpCode_hmoveto		 22 /* CFF, CFF2 */
#define OpCode_vstemhm		 23 /* CFF, CFF2 */
#define OpCode_rcurveline	 24 /* CFF, CFF2 */
#define OpCode_rlinecurve	 25 /* CFF, CFF2 */
#define OpCode_vvcurveto	 26 /* CFF, CFF2 */
#define OpCode_hhcurveto	 27 /* CFF, CFF2 */
//#define OpCode_shortint	 28 /* CFF, CFF2 */
#define OpCode_callgsubr	 29 /* CFF, CFF2 */
#define OpCode_vhcurveto	 30 /* CFF, CFF2 */
#define OpCode_hvcurveto	 31 /* CFF, CFF2 */

#define OpCode_fixedcs		255 /* 32-bit fixed */

/* Two byte escape operators 12, (0-41) */
#define OpCode_dotsection	Make_OpCode_ESC(0) /* CFF (obsoleted) */
#define OpCode_ReservedESC1	Make_OpCode_ESC(1)
#define OpCode_ReservedESC2	Make_OpCode_ESC(2)
#define OpCode_and		Make_OpCode_ESC(3) /* CFF */
#define OpCode_or		Make_OpCode_ESC(4) /* CFF */
#define OpCode_not		Make_OpCode_ESC(5) /* CFF */
#define OpCode_ReservedESC6	Make_OpCode_ESC(6)
#define OpCode_ReservedESC7	Make_OpCode_ESC(7)
#define OpCode_ReservedESC8	Make_OpCode_ESC(8)
#define OpCode_abs		Make_OpCode_ESC(9) /* CFF */
#define OpCode_add		Make_OpCode_ESC(10) /* CFF */
#define OpCode_sub		Make_OpCode_ESC(11) /* CFF */
#define OpCode_div		Make_OpCode_ESC(12) /* CFF */
#define OpCode_ReservedESC13	Make_OpCode_ESC(13)
#define OpCode_neg		Make_OpCode_ESC(14) /* CFF */
#define OpCode_eq		Make_OpCode_ESC(15) /* CFF */
#define OpCode_ReservedESC16	Make_OpCode_ESC(16)
#define OpCode_ReservedESC17	Make_OpCode_ESC(17)
#define OpCode_drop		Make_OpCode_ESC(18) /* CFF */
#define OpCode_ReservedESC19	Make_OpCode_ESC(19)
#define OpCode_put		Make_OpCode_ESC(20) /* CFF */
#define OpCode_get		Make_OpCode_ESC(21) /* CFF */
#define OpCode_ifelse		Make_OpCode_ESC(22) /* CFF */
#define OpCode_random		Make_OpCode_ESC(23) /* CFF */
#define OpCode_mul		Make_OpCode_ESC(24) /* CFF */
//#define OpCode_reservedESC25	Make_OpCode_ESC(25)
#define OpCode_sqrt		Make_OpCode_ESC(26) /* CFF */
#define OpCode_dup		Make_OpCode_ESC(27) /* CFF */
#define OpCode_exch		Make_OpCode_ESC(28) /* CFF */
#define Op_Code_সূচক		Make_OpCode_ESC(29) /* CFF */
#define OpCode_roll		Make_OpCode_ESC(30) /* CFF */
#define OpCode_reservedESC31	Make_OpCode_ESC(31)
#define OpCode_reservedESC32	Make_OpCode_ESC(32)
#define OpCode_reservedESC33	Make_OpCode_ESC(33)
#define OpCode_hflex		Make_OpCode_ESC(34) /* CFF, CFF2 */
#define OpCode_flex		Make_OpCode_ESC(35) /* CFF, CFF2 */
#define OpCode_hflex1		Make_OpCode_ESC(36) /* CFF, CFF2 */
#define OpCode_flex1		Make_OpCode_ESC(37) /* CFF, CFF2 */


#define OpCode_Invalid		0xFFFFu


কাঠামো সংখ্যা_t
{
  শূন্য set_int (পূর্ণসংখ্যা v)       { value = v; }
  যৌক্তিক to_int (পূর্ণসংখ্যা *v) ধ্রুবক
  {
    যদি (unlikely (!(value >= INT_MIN && value <= INT_MAX)))
    {
      *v = 0;
      ফেরত মিথ্যা;
    }
    *v = (পূর্ণসংখ্যা) value;
    ফেরত সত্য;
  }
  পূর্ণসংখ্যা to_int () ধ্রুবক
  {
    পূর্ণসংখ্যা v;
    যদি (likely (to_int (&v))) ফেরত v;
    ফেরত value < 0 ? INT_MIN : value > 0 ? INT_MAX : 0;
  }

  শূন্য set_fixed (int32_t v) { value = v / 65536.0; }
  int32_t to_fixed () ধ্রুবক
  { ফেরত (int32_t) hb_clamp (value * 65536.0, (দ্বিগুণ) INT32_MIN, (দ্বিগুণ) INT32_MAX); }

  শূন্য set_real (দ্বিগুণ v)   { value = v; }
  দ্বিগুণ to_real () ধ্রুবক    { ফেরত value; }

  যৌক্তিক in_int_range () ধ্রুবক
  { ফেরত value >= INT16_MIN && value <= INT16_MAX && (দ্বিগুণ) (int16_t) value == value; }

  যৌক্তিক operator >  (ধ্রুবক সংখ্যা_t &n) ধ্রুবক { ফেরত value > n.to_real (); }
  যৌক্তিক operator <  (ধ্রুবক সংখ্যা_t &n) ধ্রুবক { ফেরত n > *এই; }
  যৌক্তিক operator >= (ধ্রুবক সংখ্যা_t &n) ধ্রুবক { ফেরত !(*এই < n); }
  যৌক্তিক operator <= (ধ্রুবক সংখ্যা_t &n) ধ্রুবক { ফেরত !(*এই > n); }

  ধ্রুবক সংখ্যা_t &operator += (ধ্রুবক সংখ্যা_t &n)
  {
    set_real (to_real () + n.to_real ());

    ফেরত *এই;
  }

  সুরক্ষিত:
  দ্বিগুণ value = 0.;
};

/* byte string */
কাঠামো UnsizedByteStr : UnsizedArrayOf <HBUINT8>
{
  hb_ubytes_t as_ubytes (unsigned l) ধ্রুবক
  { ফেরত hb_ubytes_t ((ধ্রুবক unsigned অক্ষর *) এই, l); }

  // encode 2-byte int (Dict/CharString) or 4-byte int (Dict)
  template <typename T, typename V>
  স্থির যৌক্তিক serialize_int (hb_serialize_context_t *c, op_code_t intOp, V value)
  {
    TRACE_SERIALIZE (এই);

    HBUINT8 *p = c->allocate_আকার<HBUINT8> (1);
    যদি (unlikely (!p)) return_trace (মিথ্যা);
    *p = intOp;

    T *ip = c->allocate_আকার<T> (T::static_আকার);
    যদি (unlikely (!ip)) return_trace (মিথ্যা);
    return_trace (c->check_assign (*ip, value, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  template <typename V>
  স্থির যৌক্তিক serialize_int4 (hb_serialize_context_t *c, V value)
  { ফেরত serialize_int<HBINT32> (c, OpCode_longintdict, value); }

  template <typename V>
  স্থির যৌক্তিক serialize_int2 (hb_serialize_context_t *c, V value)
  { ফেরত serialize_int<HBINT16> (c, OpCode_shortint, value); }

  /* Defining null_size allows a Null object may be created. Should be safe because:
   * A descendent struct Dict uses a Null pointer to indicate a missing table,
   * checked before access.
   */
  DEFINE_আকার_MIN(0);
};

/* A byte string associated with the current offset and an error condition */
কাঠামো byte_str_ref_t
{
  byte_str_ref_t ()
    : str () {}

  byte_str_ref_t (ধ্রুবক hb_ubytes_t &str_, unsigned পূর্ণসংখ্যা offset_ = 0)
    : str (str_) { set_offset (offset_); }

  শূন্য reset (ধ্রুবক hb_ubytes_t &str_, unsigned পূর্ণসংখ্যা offset_ = 0)
  {
    str = str_;
    set_offset (offset_);
  }

  ধ্রুবক unsigned অক্ষর& operator [] (পূর্ণসংখ্যা i) {
    যদি (unlikely ((unsigned পূর্ণসংখ্যা) (get_offset () + i) >= str.দৈর্ঘ্য))
    {
      set_error ();
      ফেরত Null (unsigned অক্ষর);
    }
    ফেরত str.arrayZ[get_offset () + i];
  }

  unsigned অক্ষর শীর্ষ_unchecked () ধ্রুবক { ফেরত str.arrayZ[get_offset ()]; }

  /* Conversion to hb_ubytes_t */
  operator hb_ubytes_t () ধ্রুবক { ফেরত str.sub_array (get_offset ()); }

  hb_ubytes_t sub_array (unsigned পূর্ণসংখ্যা offset_, unsigned পূর্ণসংখ্যা len_) ধ্রুবক
  { ফেরত str.sub_array (offset_, len_); }

  যৌক্তিক avail (unsigned পূর্ণসংখ্যা গুনতি=1) ধ্রুবক
  { ফেরত get_offset () + গুনতি <= str.দৈর্ঘ্য; }
  শূন্য inc (unsigned পূর্ণসংখ্যা গুনতি=1)
  {
    /* Automatically puts us in error if count is out-of-range. */
    set_offset (get_offset () + গুনতি);
  }

  /* We (ab)use ubytes backwards_length as a cursor (called offset),
   * as well as to store error condition. */

  unsigned get_offset () ধ্রুবক { ফেরত str.backwards_দৈর্ঘ্য; }
  শূন্য set_offset (unsigned offset) { str.backwards_দৈর্ঘ্য = offset; }

  শূন্য set_error ()      { str.backwards_দৈর্ঘ্য = str.দৈর্ঘ্য + 1; }
  যৌক্তিক in_error () ধ্রুবক { ফেরত str.backwards_দৈর্ঘ্য > str.দৈর্ঘ্য; }

  unsigned যোগ_আকার () ধ্রুবক { ফেরত str.দৈর্ঘ্য; }

  সুরক্ষিত:
  hb_ubytes_t       str;
};

/*স্তূপ*/
template <typename ELEM, পূর্ণসংখ্যা LIMIT>
কাঠামো cff_স্তূপ_t
{
  ELEM& operator [] (unsigned পূর্ণসংখ্যা i)
  {
    যদি (unlikely (i >= দৈর্ঘ্য))
    {
      set_error ();
      ফেরত Crap (ELEM);
    }
    ফেরত arrayZ[i];
  }

  শূন্য push (ধ্রুবক ELEM &v)
  {
    যদি (likely (দৈর্ঘ্য < LIMIT))
      arrayZ[দৈর্ঘ্য++] = v;
    অন্যথা
      set_error ();
  }
  ELEM &push ()
  {
    যদি (likely (দৈর্ঘ্য < LIMIT))
      ফেরত arrayZ[দৈর্ঘ্য++];
    অন্যথা
    {
      set_error ();
      ফেরত Crap (ELEM);
    }
  }

  ELEM& বের_করো ()
  {
    যদি (likely (দৈর্ঘ্য > 0))
      ফেরত arrayZ[--দৈর্ঘ্য];
    অন্যথা
    {
      set_error ();
      ফেরত Crap (ELEM);
    }
  }
  শূন্য বের_করো (unsigned পূর্ণসংখ্যা n)
  {
    যদি (likely (দৈর্ঘ্য >= n))
      দৈর্ঘ্য -= n;
    অন্যথা
      set_error ();
  }

  ধ্রুবক ELEM& peek ()
  {
    যদি (unlikely (দৈর্ঘ্য == 0))
    {
      set_error ();
      ফেরত Null (ELEM);
    }
    ফেরত arrayZ[দৈর্ঘ্য - 1];
  }

  শূন্য unpop ()
  {
    যদি (likely (দৈর্ঘ্য < LIMIT))
      দৈর্ঘ্য++;
    অন্যথা
      set_error ();
  }

  শূন্য clear () { দৈর্ঘ্য = 0; }

  যৌক্তিক in_error () ধ্রুবক { ফেরত (error); }
  শূন্য set_error ()      { error = সত্য; }

  unsigned পূর্ণসংখ্যা get_গুনতি () ধ্রুবক { ফেরত দৈর্ঘ্য; }
  যৌক্তিক is_empty () ধ্রুবক          { ফেরত !দৈর্ঘ্য; }

  hb_array_t<ধ্রুবক ELEM> sub_array (unsigned start, unsigned দৈর্ঘ্য) ধ্রুবক
  { ফেরত hb_array_t<ধ্রুবক ELEM> (arrayZ).sub_array (start, দৈর্ঘ্য); }

  যৌক্তিক error = মিথ্যা;
  unsigned পূর্ণসংখ্যা দৈর্ঘ্য = 0;
  ELEM arrayZ[LIMIT];
};

/* argument stack */
template <typename ARG=সংখ্যা_t>
কাঠামো arg_স্তূপ_t : cff_স্তূপ_t<ARG, 513>
{
  শূন্য ঠেলো_int (পূর্ণসংখ্যা v)
  {
    ARG &n = S::push ();
    n.set_int (v);
  }

  শূন্য ঠেলো_fixed (int32_t v)
  {
    ARG &n = S::push ();
    n.set_fixed (v);
  }

  শূন্য ঠেলো_real (দ্বিগুণ v)
  {
    ARG &n = S::push ();
    n.set_real (v);
  }

  ARG& বের_করো_অঙ্ক () { ফেরত এই->বের_করো (); }

  পূর্ণসংখ্যা বের_করো_int ()
  {
    পূর্ণসংখ্যা v;
    যদি (unlikely (!এই->বের_করো ().to_int (&v)))
      S::set_error ();
    ফেরত v;
  }

  unsigned পূর্ণসংখ্যা বের_করো_uint ()
  {
    পূর্ণসংখ্যা i = বের_করো_int ();
    যদি (unlikely (i < 0))
    {
      i = 0;
      S::set_error ();
    }
    ফেরত (unsigned) i;
  }

  শূন্য ঠেলো_longint_from_substr (byte_str_ref_t& str_ref)
  {
    ঠেলো_int ((পূর্ণসংখ্যা) ((uint32_t (str_ref[0]) << 24) | (str_ref[1] << 16) | (str_ref[2] << 8) | (str_ref[3])));
    str_ref.inc (4);
  }

  যৌক্তিক ঠেলো_fixed_from_substr (byte_str_ref_t& str_ref)
  {
    যদি (unlikely (!str_ref.avail (4)))
      ফেরত মিথ্যা;
    ঠেলো_fixed ((int32_t)*(ধ্রুবক HBUINT32*)&str_ref[0]);
    str_ref.inc (4);
    ফেরত সত্য;
  }

  ব্যক্তিগত:
  প্রকারলেখ cff_স্তূপ_t<ARG, 513> S;
};

/* an operator prefixed by its operands in a byte string */
কাঠামো op_str_t
{
  /* This used to have a hb_ubytes_t. Using a pointer and length
   * in a particular order, saves 8 bytes in this struct and more
   * in our parsed_cs_op_t subclass. */

  ধ্রুবক unsigned অক্ষর *ptr = nullptr;

  op_code_t  op = OpCode_Invalid;

  uint8_t দৈর্ঘ্য = 0;
};

/* base of OP_SERIALIZER */
কাঠামো op_serializer_t
{
  সুরক্ষিত:
  যৌক্তিক copy_opstr (hb_serialize_context_t *c, ধ্রুবক op_str_t& opstr) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);

    unsigned অক্ষর *d = c->allocate_আকার<unsigned অক্ষর> (opstr.দৈর্ঘ্য);
    যদি (unlikely (!d)) return_trace (মিথ্যা);
    /* Faster than hb_memcpy for small strings. */
    জন্য (unsigned i = 0; i < opstr.দৈর্ঘ্য; i++)
      d[i] = opstr.ptr[i];
    return_trace (সত্য);
  }
};

template <typename VAL>
কাঠামো parsed_মানসমূহ_t
{
  শূন্য init ()
  {
    opStart = 0;
    values.init ();
  }
  শূন্য fini () { values.fini (); }

  শূন্য alloc (unsigned n)
  {
    values.alloc_exact (n);
  }

  শূন্য add_op (op_code_t op, ধ্রুবক byte_str_ref_t& str_ref = byte_str_ref_t (), ধ্রুবক VAL &v = VAL ())
  {
    VAL *val = values.push (v);
    val->op = op;
    auto arr = str_ref.sub_array (opStart, str_ref.get_offset () - opStart);
    val->ptr = arr.arrayZ;
    val->দৈর্ঘ্য = arr.দৈর্ঘ্য;
    opStart = str_ref.get_offset ();
  }

  যৌক্তিক has_op (op_code_t op) ধ্রুবক
  {
    জন্য (ধ্রুবক auto& v : values)
      যদি (v.op == op) ফেরত সত্য;
    ফেরত মিথ্যা;
  }

  unsigned get_গুনতি () ধ্রুবক { ফেরত values.দৈর্ঘ্য; }
  ধ্রুবক VAL &operator [] (unsigned পূর্ণসংখ্যা i) ধ্রুবক { ফেরত values[i]; }

  unsigned পূর্ণসংখ্যা       opStart;
  hb_vector_t<VAL>   values;
};

template <typename ARG=সংখ্যা_t>
কাঠামো interp_env_t
{
  interp_env_t () {}
  interp_env_t (ধ্রুবক hb_ubytes_t &str_)
  {
    str_ref.reset (str_);
  }
  যৌক্তিক in_error () ধ্রুবক
  { ফেরত str_ref.in_error () || arg_স্তূপ.in_error (); }

  শূন্য set_error () { str_ref.set_error (); }

  op_code_t fetch_op ()
  {
    op_code_t  op = OpCode_Invalid;
    যদি (unlikely (!str_ref.avail ()))
      ফেরত OpCode_Invalid;
    op = (op_code_t) str_ref.শীর্ষ_unchecked ();
    str_ref.inc ();
    যদি (op == OpCode_escape) {
      যদি (unlikely (!str_ref.avail ()))
	ফেরত OpCode_Invalid;
      op = Make_OpCode_ESC (str_ref.শীর্ষ_unchecked ());
      str_ref.inc ();
    }
    ফেরত op;
  }

  ধ্রুবক ARG& eval_arg (unsigned পূর্ণসংখ্যা i) { ফেরত arg_স্তূপ[i]; }

  ARG& বের_করো_arg () { ফেরত arg_স্তূপ.বের_করো (); }
  শূন্য বের_করো_n_args (unsigned পূর্ণসংখ্যা n) { arg_স্তূপ.বের_করো (n); }

  শূন্য clear_args () { বের_করো_n_args (arg_স্তূপ.get_গুনতি ()); }

  byte_str_ref_t
		str_ref;
  arg_স্তূপ_t<ARG>
		arg_স্তূপ;
};

using অঙ্ক_interp_env_t =  interp_env_t<>;

template <typename ARG=সংখ্যা_t>
কাঠামো opset_t
{
  স্থির শূন্য process_op (op_code_t op, interp_env_t<ARG>& env)
  {
    নির্বাচন (op) {
      ক্ষেত্র OpCode_shortint:
	env.arg_স্তূপ.ঠেলো_int ((int16_t)((env.str_ref[0] << 8) | env.str_ref[1]));
	env.str_ref.inc (2);
	বিরতি;

      ক্ষেত্র OpCode_TwoBytePosInt0: ক্ষেত্র OpCode_TwoBytePosInt1:
      ক্ষেত্র OpCode_TwoBytePosInt2: ক্ষেত্র OpCode_TwoBytePosInt3:
	env.arg_স্তূপ.ঠেলো_int ((int16_t)((op - OpCode_TwoBytePosInt0) * 256 + env.str_ref[0] + 108));
	env.str_ref.inc ();
	বিরতি;

      ক্ষেত্র OpCode_TwoByteNegInt0: ক্ষেত্র OpCode_TwoByteNegInt1:
      ক্ষেত্র OpCode_TwoByteNegInt2: ক্ষেত্র OpCode_TwoByteNegInt3:
	env.arg_স্তূপ.ঠেলো_int ((-(int16_t)(op - OpCode_TwoByteNegInt0) * 256 - env.str_ref[0] - 108));
	env.str_ref.inc ();
	বিরতি;

      পূর্বনির্ধারিত:
	/* 1-byte integer */
	যদি (likely ((OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast)))
	{
	  env.arg_স্তূপ.ঠেলো_int ((পূর্ণসংখ্যা)op - 139);
	} অন্যথা {
	  /* invalid unknown operator */
	  env.clear_args ();
	}
	বিরতি;
    }
  }
};

template <typename ENV>
কাঠামো interpreter_t
{
  interpreter_t (ENV& env_) : env (env_) {}
  ENV& env;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_COMMON_HH */
