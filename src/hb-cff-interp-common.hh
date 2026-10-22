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
#اگر_غیر_تعریف_شدہ HB_CFF_INTERP_COMMON_HH
#تعریف_کرو HB_CFF_INTERP_COMMON_HH

خارجی HB_اندرونی ثابت غیرعلامتی حرف *endchar_str;

نامخلا CFF {

استعمال نامخلا OT;

قسمتعریف غیرعلامتی صحیح op_رمز_t;


/* === Dict operators === */

/* One byte operators (0-31) */
#تعریف_کرو Op_رمز_نسخہ		  0 /* CFF Top */
#تعریف_کرو Op_رمز_Notice		  1 /* CFF Top */
#تعریف_کرو Op_رمز_بھرا_Name		  2 /* CFF Top */
#تعریف_کرو Op_رمز_Family_Name	  3 /* CFF Top */
#تعریف_کرو Op_رمز_وزن		  4 /* CFF Top */
#تعریف_کرو Op_رمز_Font_BBox		  5 /* CFF Top */
#تعریف_کرو Op_رمز_Blue_اقدار	  6 /* CFF Private, CFF2 Private */
#تعریف_کرو Op_رمز_Other_Blues	  7 /* CFF Private, CFF2 Private */
#تعریف_کرو Op_رمز_Family_Blues	  8 /* CFF Private, CFF2 Private */
#تعریف_کرو Op_رمز_Family_Other_Blues	  9 /* CFF Private, CFF2 Private */
#تعریف_کرو Op_رمز_Std_HW		 10 /* CFF Private, CFF2 Private */
#تعریف_کرو Op_رمز_Std_VW		 11 /* CFF Private, CFF2 Private */
#تعریف_کرو Op_رمز_escape		 12 /* All. Shared with CS */
#تعریف_کرو Op_رمز_Unique_ID		 13 /* CFF Top */
#تعریف_کرو Op_رمز_XUID		 14 /* CFF Top */
#تعریف_کرو Op_رمز_charset		 15 /* CFF Top (0) */
#تعریف_کرو Op_رمز_Encoding		 16 /* CFF Top (0) */
#تعریف_کرو Op_رمز_Char_لڑیاں	 17 /* CFF Top, CFF2 Top */
#تعریف_کرو Op_رمز_نجی		 18 /* CFF Top, CFF2 FD */
#تعریف_کرو Op_رمز_Subrs		 19 /* CFF Private, CFF2 Private */
#تعریف_کرو Op_رمز_default_چوڑائی_X	 20 /* CFF Private (0) */
#تعریف_کرو Op_رمز_nominal_چوڑائی_X	 21 /* CFF Private (0) */
#تعریف_کرو Op_رمز_vsindexdict	 22 /* CFF2 Private/CS */
#تعریف_کرو Op_رمز_blenddict	 23 /* CFF2 Private/CS */
#تعریف_کرو Op_رمز_vstore		 24 /* CFF2 Top */
#تعریف_کرو Op_رمز_reserved25	 25
#تعریف_کرو Op_رمز_reserved26	 26
#تعریف_کرو Op_رمز_reserved27	 27

/* Numbers */
#تعریف_کرو Op_رمز_shortint		 28 /* 16-bit integer, All */
#تعریف_کرو Op_رمز_longintdict	 29 /* 32-bit integer, All */
#تعریف_کرو Op_رمز_BCD		 30 /* Real number, CFF2 Top/FD */
#تعریف_کرو Op_رمز_reserved31	 31

/* 1-byte integers */
#تعریف_کرو Op_رمز_One_Byte_Int_پہلا	 32 /* All. beginning of the range of first byte ints */
#تعریف_کرو Op_رمز_One_Byte_Int_آخری	246 /* All. ending of the range of first byte int */

/* 2-byte integers */
#تعریف_کرو Op_رمز_Two_Byte_Pos_Int0	247 /* All. first byte of two byte positive int (+108 to +1131) */
#تعریف_کرو Op_رمز_Two_Byte_Pos_Int1	248
#تعریف_کرو Op_رمز_Two_Byte_Pos_Int2	249
#تعریف_کرو Op_رمز_Two_Byte_Pos_Int3	250

#تعریف_کرو Op_رمز_Two_Byte_Neg_Int0	251 /* All. first byte of two byte negative int (-1131 to -108) */
#تعریف_کرو Op_رمز_Two_Byte_Neg_Int1	252
#تعریف_کرو Op_رمز_Two_Byte_Neg_Int2	253
#تعریف_کرو Op_رمز_Two_Byte_Neg_Int3	254

/* Two byte escape operators 12, (0-41) */
#تعریف_کرو Op_رمز_ESC_بنیاد		256
#تعریف_کرو Make_Op_رمز_ESC(byte2)	((op_رمز_t)(Op_رمز_ESC_بنیاد + (byte2)))

اندرونی op_رمز_t Unmake_Op_رمز_ESC (op_رمز_t op)  { واپس (op_رمز_t)(op - Op_رمز_ESC_بنیاد); }
اندرونی منطقی Is_Op_رمز_ESC (op_رمز_t op) { واپس op >= Op_رمز_ESC_بنیاد; }
اندرونی غیرعلامتی صحیح Op_رمز_سائز (op_رمز_t op) { واپس Is_Op_رمز_ESC (op) ? 2: 1; }

#تعریف_کرو Op_رمز_Copyright	Make_Op_رمز_ESC(0) /* CFF Top */
#تعریف_کرو Op_رمز_is_Fixed_Pitch	Make_Op_رمز_ESC(1) /* CFF Top (false) */
#تعریف_کرو Op_رمز_Italic_Angle	Make_Op_رمز_ESC(2) /* CFF Top (0) */
#تعریف_کرو Op_رمز_Underline_Position Make_Op_رمز_ESC(3) /* CFF Top (-100) */
#تعریف_کرو Op_رمز_Underline_Thickness Make_Op_رمز_ESC(4) /* CFF Top (50) */
#تعریف_کرو Op_رمز_Paint_Type	Make_Op_رمز_ESC(5) /* CFF Top (0) */
#تعریف_کرو Op_رمز_Charstring_Type	Make_Op_رمز_ESC(6) /* CFF Top (2) */
#تعریف_کرو Op_رمز_Font_میٹرکس	Make_Op_رمز_ESC(7) /* CFF Top, CFF2 Top (.001 0 0 .001 0 0)*/
#تعریف_کرو Op_رمز_Stroke_چوڑائی	Make_Op_رمز_ESC(8) /* CFF Top (0) */
#تعریف_کرو Op_رمز_Blue_Scale	Make_Op_رمز_ESC(9) /* CFF Private, CFF2 Private (0.039625) */
#تعریف_کرو Op_رمز_Blue_سرکاؤ	Make_Op_رمز_ESC(10) /* CFF Private, CFF2 Private (7) */
#تعریف_کرو Op_رمز_Blue_Fuzz		Make_Op_رمز_ESC(11) /* CFF Private, CFF2 Private (1) */
#تعریف_کرو Op_رمز_Stem_Snap_H	Make_Op_رمز_ESC(12) /* CFF Private, CFF2 Private */
#تعریف_کرو Op_رمز_Stem_Snap_V	Make_Op_رمز_ESC(13) /* CFF Private, CFF2 Private */
#تعریف_کرو Op_رمز_Force_Bold	Make_Op_رمز_ESC(14) /* CFF Private (false) */
#تعریف_کرو Op_رمز_reserved_ESC15	Make_Op_رمز_ESC(15)
#تعریف_کرو Op_رمز_reserved_ESC16	Make_Op_رمز_ESC(16)
#تعریف_کرو Op_رمز_Language_گروہ	Make_Op_رمز_ESC(17) /* CFF Private, CFF2 Private (0) */
#تعریف_کرو Op_رمز_Expansion_ضربی_عدد	Make_Op_رمز_ESC(18) /* CFF Private, CFF2 Private (0.06) */
#تعریف_کرو Op_رمز_initial_Random_بیج Make_Op_رمز_ESC(19) /* CFF Private (0) */
#تعریف_کرو Op_رمز_Synthetic_بنیاد	Make_Op_رمز_ESC(20) /* CFF Top */
#تعریف_کرو Op_رمز_Post_Script	Make_Op_رمز_ESC(21) /* CFF Top */
#تعریف_کرو Op_رمز_بنیاد_Font_Name	Make_Op_رمز_ESC(22) /* CFF Top */
#تعریف_کرو Op_رمز_بنیاد_Font_Blend	Make_Op_رمز_ESC(23) /* CFF Top */
#تعریف_کرو Op_رمز_reserved_ESC24	Make_Op_رمز_ESC(24)
#تعریف_کرو Op_رمز_reserved_ESC25	Make_Op_رمز_ESC(25)
#تعریف_کرو Op_رمز_reserved_ESC26	Make_Op_رمز_ESC(26)
#تعریف_کرو Op_رمز_reserved_ESC27	Make_Op_رمز_ESC(27)
#تعریف_کرو Op_رمز_reserved_ESC28	Make_Op_رمز_ESC(28)
#تعریف_کرو Op_رمز_reserved_ESC29	Make_Op_رمز_ESC(29)
#تعریف_کرو Op_رمز_ROS		Make_Op_رمز_ESC(30) /* CFF Top_CID */
#تعریف_کرو Op_رمز_CIDFont_نسخہ	Make_Op_رمز_ESC(31) /* CFF Top_CID (0) */
#تعریف_کرو Op_رمز_CIDFont_Revision	Make_Op_رمز_ESC(32) /* CFF Top_CID (0) */
#تعریف_کرو Op_رمز_CIDFont_Type	Make_Op_رمز_ESC(33) /* CFF Top_CID (0) */
#تعریف_کرو Op_رمز_CIDCount		Make_Op_رمز_ESC(34) /* CFF Top_CID (8720) */
#تعریف_کرو Op_رمز_UIDBase		Make_Op_رمز_ESC(35) /* CFF Top_CID */
#تعریف_کرو Op_رمز_FDArray		Make_Op_رمز_ESC(36) /* CFF Top_CID, CFF2 Top */
#تعریف_کرو Op_رمز_FDSelect		Make_Op_رمز_ESC(37) /* CFF Top_CID, CFF2 Top */
#تعریف_کرو Op_رمز_Font_Name		Make_Op_رمز_ESC(38) /* CFF Top_CID */


/* === CharString operators === */

#تعریف_کرو Op_رمز_hstem		  1 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_Reserved2	  2
#تعریف_کرو Op_رمز_vstem		  3 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_vmoveto		  4 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_rlineto		  5 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_hlineto		  6 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_vlineto		  7 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_rrcurveto	  8 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_Reserved9	  9
#تعریف_کرو Op_رمز_callsubr		 10 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_return		 11 /* CFF */
//#define OpCode_escape		 12 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_Reserved13	 13
#تعریف_کرو Op_رمز_endchar		 14 /* CFF */
#تعریف_کرو Op_رمز_vsindexcs	 15 /* CFF2 */
#تعریف_کرو Op_رمز_blendcs		 16 /* CFF2 */
#تعریف_کرو Op_رمز_Reserved17	 17
#تعریف_کرو Op_رمز_hstemhm		 18 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_hintmask		 19 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_cntrmask		 20 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_rmoveto		 21 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_hmoveto		 22 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_vstemhm		 23 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_rcurveline	 24 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_rlinecurve	 25 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_vvcurveto	 26 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_hhcurveto	 27 /* CFF, CFF2 */
//#define OpCode_shortint	 28 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_callgsubr	 29 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_vhcurveto	 30 /* CFF, CFF2 */
#تعریف_کرو Op_رمز_hvcurveto	 31 /* CFF, CFF2 */

#تعریف_کرو Op_رمز_fixedcs		255 /* 32-bit fixed */

/* Two byte escape operators 12, (0-41) */
#تعریف_کرو Op_رمز_dotsection	Make_Op_رمز_ESC(0) /* CFF (obsoleted) */
#تعریف_کرو Op_رمز_Reserved_ESC1	Make_Op_رمز_ESC(1)
#تعریف_کرو Op_رمز_Reserved_ESC2	Make_Op_رمز_ESC(2)
#تعریف_کرو Op_رمز_and		Make_Op_رمز_ESC(3) /* CFF */
#تعریف_کرو Op_رمز_or		Make_Op_رمز_ESC(4) /* CFF */
#تعریف_کرو Op_رمز_not		Make_Op_رمز_ESC(5) /* CFF */
#تعریف_کرو Op_رمز_Reserved_ESC6	Make_Op_رمز_ESC(6)
#تعریف_کرو Op_رمز_Reserved_ESC7	Make_Op_رمز_ESC(7)
#تعریف_کرو Op_رمز_Reserved_ESC8	Make_Op_رمز_ESC(8)
#تعریف_کرو Op_رمز_abs		Make_Op_رمز_ESC(9) /* CFF */
#تعریف_کرو Op_رمز_جوڑو		Make_Op_رمز_ESC(10) /* CFF */
#تعریف_کرو Op_رمز_sub		Make_Op_رمز_ESC(11) /* CFF */
#تعریف_کرو Op_رمز_div		Make_Op_رمز_ESC(12) /* CFF */
#تعریف_کرو Op_رمز_Reserved_ESC13	Make_Op_رمز_ESC(13)
#تعریف_کرو Op_رمز_neg		Make_Op_رمز_ESC(14) /* CFF */
#تعریف_کرو Op_رمز_eq		Make_Op_رمز_ESC(15) /* CFF */
#تعریف_کرو Op_رمز_Reserved_ESC16	Make_Op_رمز_ESC(16)
#تعریف_کرو Op_رمز_Reserved_ESC17	Make_Op_رمز_ESC(17)
#تعریف_کرو Op_رمز_drop		Make_Op_رمز_ESC(18) /* CFF */
#تعریف_کرو Op_رمز_Reserved_ESC19	Make_Op_رمز_ESC(19)
#تعریف_کرو Op_رمز_put		Make_Op_رمز_ESC(20) /* CFF */
#تعریف_کرو Op_رمز_get		Make_Op_رمز_ESC(21) /* CFF */
#تعریف_کرو Op_رمز_ifelse		Make_Op_رمز_ESC(22) /* CFF */
#تعریف_کرو Op_رمز_random		Make_Op_رمز_ESC(23) /* CFF */
#تعریف_کرو Op_رمز_mul		Make_Op_رمز_ESC(24) /* CFF */
//#define OpCode_reservedESC25	Make_OpCode_ESC(25)
#تعریف_کرو Op_رمز_sqrt		Make_Op_رمز_ESC(26) /* CFF */
#تعریف_کرو Op_رمز_dup		Make_Op_رمز_ESC(27) /* CFF */
#تعریف_کرو Op_رمز_exch		Make_Op_رمز_ESC(28) /* CFF */
#تعریف_کرو Op_رمز_اشاریہ		Make_Op_رمز_ESC(29) /* CFF */
#تعریف_کرو Op_رمز_roll		Make_Op_رمز_ESC(30) /* CFF */
#تعریف_کرو Op_رمز_reserved_ESC31	Make_Op_رمز_ESC(31)
#تعریف_کرو Op_رمز_reserved_ESC32	Make_Op_رمز_ESC(32)
#تعریف_کرو Op_رمز_reserved_ESC33	Make_Op_رمز_ESC(33)
#تعریف_کرو Op_رمز_hflex		Make_Op_رمز_ESC(34) /* CFF, CFF2 */
#تعریف_کرو Op_رمز_flex		Make_Op_رمز_ESC(35) /* CFF, CFF2 */
#تعریف_کرو Op_رمز_hflex1		Make_Op_رمز_ESC(36) /* CFF, CFF2 */
#تعریف_کرو Op_رمز_flex1		Make_Op_رمز_ESC(37) /* CFF, CFF2 */


#تعریف_کرو Op_رمز_Invalid		0xFFFFu


ساخت نمبر_t
{
  خالی سیٹ_int (صحیح v)       { value = v; }
  منطقی مقصد_int (صحیح *v) ثابت
  {
    اگر (unlikely (!(value >= INT_MIN && value <= زیادہ_سے_زیادہ_عدد)))
    {
      *v = 0;
      واپس جھوٹ;
    }
    *v = (صحیح) value;
    واپس سچ;
  }
  صحیح مقصد_int () ثابت
  {
    صحیح v;
    اگر (likely (مقصد_int (&v))) واپس v;
    واپس value < 0 ? INT_MIN : value > 0 ? زیادہ_سے_زیادہ_عدد : 0;
  }

  خالی سیٹ_fixed (int32_t v) { value = v / 65536.0; }
  int32_t مقصد_fixed () ثابت
  { واپس (int32_t) hb_clamp (value * 65536.0, (دوہرا) INT32_MIN, (دوہرا) INT32_MAX); }

  خالی سیٹ_real (دوہرا v)   { value = v; }
  دوہرا مقصد_real () ثابت    { واپس value; }

  منطقی اندر_int_range () ثابت
  { واپس value >= INT16_MIN && value <= INT16_MAX && (دوہرا) (int16_t) value == value; }

  منطقی عامل >  (ثابت نمبر_t &n) ثابت { واپس value > n.مقصد_real (); }
  منطقی عامل <  (ثابت نمبر_t &n) ثابت { واپس n > *یہ; }
  منطقی عامل >= (ثابت نمبر_t &n) ثابت { واپس !(*یہ < n); }
  منطقی عامل <= (ثابت نمبر_t &n) ثابت { واپس !(*یہ > n); }

  ثابت نمبر_t &عامل += (ثابت نمبر_t &n)
  {
    سیٹ_real (مقصد_real () + n.مقصد_real ());

    واپس *یہ;
  }

  محفوظ:
  دوہرا value = 0.;
};

/* byte string */
ساخت UnsizedByteStr : Unsized_Array_کا <HBUINT8>
{
  hb_ubytes_t as_ubytes (غیرعلامتی l) ثابت
  { واپس hb_ubytes_t ((ثابت غیرعلامتی حرف *) یہ, l); }

  // encode 2-byte int (Dict/CharString) or 4-byte int (Dict)
  سانچہ <قسم_نام T, قسم_نام V>
  ساکن منطقی serialize_int (hb_serialize_context_t *c, op_رمز_t intOp, V value)
  {
    TRACE_SERIALIZE (یہ);

    HBUINT8 *p = c->allocate_سائز<HBUINT8> (1);
    اگر (unlikely (!p)) return_trace (جھوٹ);
    *p = intOp;

    T *ip = c->allocate_سائز<T> (T::static_سائز);
    اگر (unlikely (!ip)) return_trace (جھوٹ);
    return_trace (c->check_تفویض (*ip, value, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  سانچہ <قسم_نام V>
  ساکن منطقی serialize_int4 (hb_serialize_context_t *c, V value)
  { واپس serialize_int<HBINT32> (c, Op_رمز_longintdict, value); }

  سانچہ <قسم_نام V>
  ساکن منطقی serialize_int2 (hb_serialize_context_t *c, V value)
  { واپس serialize_int<HBINT16> (c, Op_رمز_shortint, value); }

  /* Defining null_size allows a Null object may be created. Should be safe because:
   * A descendent struct Dict uses a Null pointer to indicate a missing table,
   * checked before access.
   */
  DEFINE_سائز_MIN(0);
};

/* A byte string associated with the current offset and an error condition */
ساخت byte_str_ref_t
{
  byte_str_ref_t ()
    : str () {}

  byte_str_ref_t (ثابت hb_ubytes_t &str_, غیرعلامتی صحیح انحراف_3 = 0)
    : str (str_) { سیٹ_انحراف (انحراف_3); }

  خالی reset (ثابت hb_ubytes_t &str_, غیرعلامتی صحیح انحراف_3 = 0)
  {
    str = str_;
    سیٹ_انحراف (انحراف_3);
  }

  ثابت غیرعلامتی حرف& عامل [] (صحیح i) {
    اگر (unlikely ((غیرعلامتی صحیح) (get_انحراف () + i) >= str.لمبائی))
    {
      سیٹ_error ();
      واپس Null (غیرعلامتی حرف);
    }
    واپس str.arrayZ[get_انحراف () + i];
  }

  غیرعلامتی حرف سر_unchecked () ثابت { واپس str.arrayZ[get_انحراف ()]; }

  /* Conversion to hb_ubytes_t */
  عامل hb_ubytes_t () ثابت { واپس str.sub_array (get_انحراف ()); }

  hb_ubytes_t sub_array (غیرعلامتی صحیح انحراف_3, غیرعلامتی صحیح len_) ثابت
  { واپس str.sub_array (انحراف_3, len_); }

  منطقی avail (غیرعلامتی صحیح گنتی=1) ثابت
  { واپس get_انحراف () + گنتی <= str.لمبائی; }
  خالی inc (غیرعلامتی صحیح گنتی=1)
  {
    /* Automatically puts us in error if count is out-of-range. */
    سیٹ_انحراف (get_انحراف () + گنتی);
  }

  /* We (ab)use ubytes backwards_length as a cursor (called offset),
   * as well as to store error condition. */

  غیرعلامتی get_انحراف () ثابت { واپس str.backwards_لمبائی; }
  خالی سیٹ_انحراف (غیرعلامتی انحراف) { str.backwards_لمبائی = انحراف; }

  خالی سیٹ_error ()      { str.backwards_لمبائی = str.لمبائی + 1; }
  منطقی اندر_error () ثابت { واپس str.backwards_لمبائی > str.لمبائی; }

  غیرعلامتی کل_سائز () ثابت { واپس str.لمبائی; }

  محفوظ:
  hb_ubytes_t       str;
};

/*ڈھیر*/
سانچہ <قسم_نام ELEM, صحیح حد_بندی_2>
ساخت cff_ڈھیر_t
{
  ELEM& عامل [] (غیرعلامتی صحیح i)
  {
    اگر (unlikely (i >= لمبائی))
    {
      سیٹ_error ();
      واپس Crap (ELEM);
    }
    واپس arrayZ[i];
  }

  خالی push (ثابت ELEM &v)
  {
    اگر (likely (لمبائی < حد_بندی_2))
      arrayZ[لمبائی++] = v;
    ورنہ
      سیٹ_error ();
  }
  ELEM &push ()
  {
    اگر (likely (لمبائی < حد_بندی_2))
      واپس arrayZ[لمبائی++];
    ورنہ
    {
      سیٹ_error ();
      واپس Crap (ELEM);
    }
  }

  ELEM& نکالو ()
  {
    اگر (likely (لمبائی > 0))
      واپس arrayZ[--لمبائی];
    ورنہ
    {
      سیٹ_error ();
      واپس Crap (ELEM);
    }
  }
  خالی نکالو (غیرعلامتی صحیح n)
  {
    اگر (likely (لمبائی >= n))
      لمبائی -= n;
    ورنہ
      سیٹ_error ();
  }

  ثابت ELEM& peek ()
  {
    اگر (unlikely (لمبائی == 0))
    {
      سیٹ_error ();
      واپس Null (ELEM);
    }
    واپس arrayZ[لمبائی - 1];
  }

  خالی unpop ()
  {
    اگر (likely (لمبائی < حد_بندی_2))
      لمبائی++;
    ورنہ
      سیٹ_error ();
  }

  خالی clear () { لمبائی = 0; }

  منطقی اندر_error () ثابت { واپس (error); }
  خالی سیٹ_error ()      { error = سچ; }

  غیرعلامتی صحیح get_گنتی () ثابت { واپس لمبائی; }
  منطقی is_empty () ثابت          { واپس !لمبائی; }

  hb_array_t<ثابت ELEM> sub_array (غیرعلامتی آغاز, غیرعلامتی لمبائی) ثابت
  { واپس hb_array_t<ثابت ELEM> (arrayZ).sub_array (آغاز, لمبائی); }

  منطقی error = جھوٹ;
  غیرعلامتی صحیح لمبائی = 0;
  ELEM arrayZ[حد_بندی_2];
};

/* argument stack */
سانچہ <قسم_نام ARG=نمبر_t>
ساخت arg_ڈھیر_t : cff_ڈھیر_t<ARG, 513>
{
  خالی ڈالو_int (صحیح v)
  {
    ARG &n = S::push ();
    n.سیٹ_int (v);
  }

  خالی ڈالو_fixed (int32_t v)
  {
    ARG &n = S::push ();
    n.سیٹ_fixed (v);
  }

  خالی ڈالو_real (دوہرا v)
  {
    ARG &n = S::push ();
    n.سیٹ_real (v);
  }

  ARG& نکالو_عدد () { واپس یہ->نکالو (); }

  صحیح نکالو_int ()
  {
    صحیح v;
    اگر (unlikely (!یہ->نکالو ().مقصد_int (&v)))
      S::سیٹ_error ();
    واپس v;
  }

  غیرعلامتی صحیح نکالو_uint ()
  {
    صحیح i = نکالو_int ();
    اگر (unlikely (i < 0))
    {
      i = 0;
      S::سیٹ_error ();
    }
    واپس (غیرعلامتی) i;
  }

  خالی ڈالو_longint_from_substr (byte_str_ref_t& str_ref)
  {
    ڈالو_int ((صحیح) ((uint32_t (str_ref[0]) << 24) | (str_ref[1] << 16) | (str_ref[2] << 8) | (str_ref[3])));
    str_ref.inc (4);
  }

  منطقی ڈالو_fixed_from_substr (byte_str_ref_t& str_ref)
  {
    اگر (unlikely (!str_ref.avail (4)))
      واپس جھوٹ;
    ڈالو_fixed ((int32_t)*(ثابت HBUINT32*)&str_ref[0]);
    str_ref.inc (4);
    واپس سچ;
  }

  نجی:
  قسمتعریف cff_ڈھیر_t<ARG, 513> S;
};

/* an operator prefixed by its operands in a byte string */
ساخت op_str_t
{
  /* This used to have a hb_ubytes_t. Using a pointer and length
   * in a particular order, saves 8 bytes in this struct and more
   * in our parsed_cs_op_t subclass. */

  ثابت غیرعلامتی حرف *ptr = خالی_اشاریہ;

  op_رمز_t  op = Op_رمز_Invalid;

  uint8_t لمبائی = 0;
};

/* base of OP_SERIALIZER */
ساخت op_serializer_t
{
  محفوظ:
  منطقی copy_opstr (hb_serialize_context_t *c, ثابت op_str_t& opstr) ثابت
  {
    TRACE_SERIALIZE (یہ);

    غیرعلامتی حرف *d = c->allocate_سائز<غیرعلامتی حرف> (opstr.لمبائی);
    اگر (unlikely (!d)) return_trace (جھوٹ);
    /* Faster than hb_memcpy for small strings. */
    برائے (غیرعلامتی i = 0; i < opstr.لمبائی; i++)
      d[i] = opstr.ptr[i];
    return_trace (سچ);
  }
};

سانچہ <قسم_نام VAL>
ساخت parsed_اقدار_t
{
  خالی init ()
  {
    op_آغاز = 0;
    values.init ();
  }
  خالی fini () { values.fini (); }

  خالی alloc (غیرعلامتی n)
  {
    values.alloc_exact (n);
  }

  خالی جوڑو_op (op_رمز_t op, ثابت byte_str_ref_t& str_ref = byte_str_ref_t (), ثابت VAL &v = VAL ())
  {
    VAL *val = values.push (v);
    val->op = op;
    خودکار arr = str_ref.sub_array (op_آغاز, str_ref.get_انحراف () - op_آغاز);
    val->ptr = arr.arrayZ;
    val->لمبائی = arr.لمبائی;
    op_آغاز = str_ref.get_انحراف ();
  }

  منطقی has_op (op_رمز_t op) ثابت
  {
    برائے (ثابت خودکار& v : values)
      اگر (v.op == op) واپس سچ;
    واپس جھوٹ;
  }

  غیرعلامتی get_گنتی () ثابت { واپس values.لمبائی; }
  ثابت VAL &عامل [] (غیرعلامتی صحیح i) ثابت { واپس values[i]; }

  غیرعلامتی صحیح       op_آغاز;
  hb_سمتیہ_t<VAL>   values;
};

سانچہ <قسم_نام ARG=نمبر_t>
ساخت interp_env_t
{
  interp_env_t () {}
  interp_env_t (ثابت hb_ubytes_t &str_)
  {
    str_ref.reset (str_);
  }
  منطقی اندر_error () ثابت
  { واپس str_ref.اندر_error () || arg_ڈھیر.اندر_error (); }

  خالی سیٹ_error () { str_ref.سیٹ_error (); }

  op_رمز_t fetch_op ()
  {
    op_رمز_t  op = Op_رمز_Invalid;
    اگر (unlikely (!str_ref.avail ()))
      واپس Op_رمز_Invalid;
    op = (op_رمز_t) str_ref.سر_unchecked ();
    str_ref.inc ();
    اگر (op == Op_رمز_escape) {
      اگر (unlikely (!str_ref.avail ()))
	واپس Op_رمز_Invalid;
      op = Make_Op_رمز_ESC (str_ref.سر_unchecked ());
      str_ref.inc ();
    }
    واپس op;
  }

  ثابت ARG& eval_arg (غیرعلامتی صحیح i) { واپس arg_ڈھیر[i]; }

  ARG& نکالو_arg () { واپس arg_ڈھیر.نکالو (); }
  خالی نکالو_n_دلائل (غیرعلامتی صحیح n) { arg_ڈھیر.نکالو (n); }

  خالی clear_دلائل () { نکالو_n_دلائل (arg_ڈھیر.get_گنتی ()); }

  byte_str_ref_t
		str_ref;
  arg_ڈھیر_t<ARG>
		arg_ڈھیر;
};

استعمال عدد_interp_env_t =  interp_env_t<>;

سانچہ <قسم_نام ARG=نمبر_t>
ساخت opset_t
{
  ساکن خالی process_op (op_رمز_t op, interp_env_t<ARG>& env)
  {
    انتخاب (op) {
      حالت Op_رمز_shortint:
	env.arg_ڈھیر.ڈالو_int ((int16_t)((env.str_ref[0] << 8) | env.str_ref[1]));
	env.str_ref.inc (2);
	توڑو;

      حالت Op_رمز_Two_Byte_Pos_Int0: حالت Op_رمز_Two_Byte_Pos_Int1:
      حالت Op_رمز_Two_Byte_Pos_Int2: حالت Op_رمز_Two_Byte_Pos_Int3:
	env.arg_ڈھیر.ڈالو_int ((int16_t)((op - Op_رمز_Two_Byte_Pos_Int0) * 256 + env.str_ref[0] + 108));
	env.str_ref.inc ();
	توڑو;

      حالت Op_رمز_Two_Byte_Neg_Int0: حالت Op_رمز_Two_Byte_Neg_Int1:
      حالت Op_رمز_Two_Byte_Neg_Int2: حالت Op_رمز_Two_Byte_Neg_Int3:
	env.arg_ڈھیر.ڈالو_int ((-(int16_t)(op - Op_رمز_Two_Byte_Neg_Int0) * 256 - env.str_ref[0] - 108));
	env.str_ref.inc ();
	توڑو;

      طےشدہ:
	/* 1-byte integer */
	اگر (likely ((Op_رمز_One_Byte_Int_پہلا <= op) && (op <= Op_رمز_One_Byte_Int_آخری)))
	{
	  env.arg_ڈھیر.ڈالو_int ((صحیح)op - 139);
	} ورنہ {
	  /* invalid unknown operator */
	  env.clear_دلائل ();
	}
	توڑو;
    }
  }
};

سانچہ <قسم_نام ENV>
ساخت interpreter_t
{
  interpreter_t (ENV& env_) : env (env_) {}
  ENV& env;
};

} /* namespace CFF */

#اگر_ختم /* HB_CFF_INTERP_COMMON_HH */
