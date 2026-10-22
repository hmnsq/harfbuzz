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
#جے_بنا_تعریف HB_CFF_INTERP_COMMON_HH
#تعریف_کرو HB_CFF_INTERP_COMMON_HH

خارجي HB_اندرونی ثابت بنانشانی حرف *endchar_str;

نطاق CFF {

ورتوں نطاق OT;

تعريفنوع بنانشانی صحيح op_کوڈ_t;


/* === Dict operators === */

/* One byte operators (0-31) */
#تعریف_کرو Op_کوڈ_نسخہ		  0 /* CFF Top */
#تعریف_کرو Op_کوڈ_Notice		  1 /* CFF Top */
#تعریف_کرو Op_کوڈ_بھریا_Name		  2 /* CFF Top */
#تعریف_کرو Op_کوڈ_Family_Name	  3 /* CFF Top */
#تعریف_کرو Op_کوڈ_بھار		  4 /* CFF Top */
#تعریف_کرو Op_کوڈ_Font_BBox		  5 /* CFF Top */
#تعریف_کرو Op_کوڈ_Blue_اقدار	  6 /* CFF Private, CFF2 Private */
#تعریف_کرو Op_کوڈ_Other_Blues	  7 /* CFF Private, CFF2 Private */
#تعریف_کرو Op_کوڈ_Family_Blues	  8 /* CFF Private, CFF2 Private */
#تعریف_کرو Op_کوڈ_Family_Other_Blues	  9 /* CFF Private, CFF2 Private */
#تعریف_کرو Op_کوڈ_Std_HW		 10 /* CFF Private, CFF2 Private */
#تعریف_کرو Op_کوڈ_Std_VW		 11 /* CFF Private, CFF2 Private */
#تعریف_کرو Op_کوڈ_escape		 12 /* All. Shared with CS */
#تعریف_کرو Op_کوڈ_Unique_ID		 13 /* CFF Top */
#تعریف_کرو Op_کوڈ_XUID		 14 /* CFF Top */
#تعریف_کرو Op_کوڈ_charset		 15 /* CFF Top (0) */
#تعریف_کرو Op_کوڈ_Encoding		 16 /* CFF Top (0) */
#تعریف_کرو Op_کوڈ_Char_لڑیاں	 17 /* CFF Top, CFF2 Top */
#تعریف_کرو Op_کوڈ_نجی		 18 /* CFF Top, CFF2 FD */
#تعریف_کرو Op_کوڈ_Subrs		 19 /* CFF Private, CFF2 Private */
#تعریف_کرو Op_کوڈ_default_چوڑائی_X	 20 /* CFF Private (0) */
#تعریف_کرو Op_کوڈ_nominal_چوڑائی_X	 21 /* CFF Private (0) */
#تعریف_کرو Op_کوڈ_vsindexdict	 22 /* CFF2 Private/CS */
#تعریف_کرو Op_کوڈ_blenddict	 23 /* CFF2 Private/CS */
#تعریف_کرو Op_کوڈ_vstore		 24 /* CFF2 Top */
#تعریف_کرو Op_کوڈ_reserved25	 25
#تعریف_کرو Op_کوڈ_reserved26	 26
#تعریف_کرو Op_کوڈ_reserved27	 27

/* Numbers */
#تعریف_کرو Op_کوڈ_shortint		 28 /* 16-bit integer, All */
#تعریف_کرو Op_کوڈ_longintdict	 29 /* 32-bit integer, All */
#تعریف_کرو Op_کوڈ_BCD		 30 /* Real number, CFF2 Top/FD */
#تعریف_کرو Op_کوڈ_reserved31	 31

/* 1-byte integers */
#تعریف_کرو Op_کوڈ_One_Byte_Int_پہلا	 32 /* All. beginning of the range of first byte ints */
#تعریف_کرو Op_کوڈ_One_Byte_Int_آخری	246 /* All. ending of the range of first byte int */

/* 2-byte integers */
#تعریف_کرو Op_کوڈ_Two_Byte_Pos_Int0	247 /* All. first byte of two byte positive int (+108 to +1131) */
#تعریف_کرو Op_کوڈ_Two_Byte_Pos_Int1	248
#تعریف_کرو Op_کوڈ_Two_Byte_Pos_Int2	249
#تعریف_کرو Op_کوڈ_Two_Byte_Pos_Int3	250

#تعریف_کرو Op_کوڈ_Two_Byte_Neg_Int0	251 /* All. first byte of two byte negative int (-1131 to -108) */
#تعریف_کرو Op_کوڈ_Two_Byte_Neg_Int1	252
#تعریف_کرو Op_کوڈ_Two_Byte_Neg_Int2	253
#تعریف_کرو Op_کوڈ_Two_Byte_Neg_Int3	254

/* Two byte escape operators 12, (0-41) */
#تعریف_کرو Op_کوڈ_ESC_ادھار		256
#تعریف_کرو Make_Op_کوڈ_ESC(byte2)	((op_کوڈ_t)(Op_کوڈ_ESC_ادھار + (byte2)))

مضمن op_کوڈ_t Unmake_Op_کوڈ_ESC (op_کوڈ_t op)  { أرجع (op_کوڈ_t)(op - Op_کوڈ_ESC_ادھار); }
مضمن منطقي Is_Op_کوڈ_ESC (op_کوڈ_t op) { أرجع op >= Op_کوڈ_ESC_ادھار; }
مضمن بنانشانی صحيح Op_کوڈ_ناپ (op_کوڈ_t op) { أرجع Is_Op_کوڈ_ESC (op) ? 2: 1; }

#تعریف_کرو Op_کوڈ_Copyright	Make_Op_کوڈ_ESC(0) /* CFF Top */
#تعریف_کرو Op_کوڈ_is_Fixed_Pitch	Make_Op_کوڈ_ESC(1) /* CFF Top (false) */
#تعریف_کرو Op_کوڈ_Italic_Angle	Make_Op_کوڈ_ESC(2) /* CFF Top (0) */
#تعریف_کرو Op_کوڈ_Underline_Position Make_Op_کوڈ_ESC(3) /* CFF Top (-100) */
#تعریف_کرو Op_کوڈ_Underline_Thickness Make_Op_کوڈ_ESC(4) /* CFF Top (50) */
#تعریف_کرو Op_کوڈ_Paint_Type	Make_Op_کوڈ_ESC(5) /* CFF Top (0) */
#تعریف_کرو Op_کوڈ_Charstring_Type	Make_Op_کوڈ_ESC(6) /* CFF Top (2) */
#تعریف_کرو Op_کوڈ_Font_میٹرکس	Make_Op_کوڈ_ESC(7) /* CFF Top, CFF2 Top (.001 0 0 .001 0 0)*/
#تعریف_کرو Op_کوڈ_Stroke_چوڑائی	Make_Op_کوڈ_ESC(8) /* CFF Top (0) */
#تعریف_کرو Op_کوڈ_Blue_Scale	Make_Op_کوڈ_ESC(9) /* CFF Private, CFF2 Private (0.039625) */
#تعریف_کرو Op_کوڈ_Blue_کھسکاؤ	Make_Op_کوڈ_ESC(10) /* CFF Private, CFF2 Private (7) */
#تعریف_کرو Op_کوڈ_Blue_Fuzz		Make_Op_کوڈ_ESC(11) /* CFF Private, CFF2 Private (1) */
#تعریف_کرو Op_کوڈ_Stem_Snap_H	Make_Op_کوڈ_ESC(12) /* CFF Private, CFF2 Private */
#تعریف_کرو Op_کوڈ_Stem_Snap_V	Make_Op_کوڈ_ESC(13) /* CFF Private, CFF2 Private */
#تعریف_کرو Op_کوڈ_Force_Bold	Make_Op_کوڈ_ESC(14) /* CFF Private (false) */
#تعریف_کرو Op_کوڈ_reserved_ESC15	Make_Op_کوڈ_ESC(15)
#تعریف_کرو Op_کوڈ_reserved_ESC16	Make_Op_کوڈ_ESC(16)
#تعریف_کرو Op_کوڈ_Language_گروہ	Make_Op_کوڈ_ESC(17) /* CFF Private, CFF2 Private (0) */
#تعریف_کرو Op_کوڈ_Expansion_ضارب	Make_Op_کوڈ_ESC(18) /* CFF Private, CFF2 Private (0.06) */
#تعریف_کرو Op_کوڈ_initial_Random_بیج Make_Op_کوڈ_ESC(19) /* CFF Private (0) */
#تعریف_کرو Op_کوڈ_Synthetic_ادھار	Make_Op_کوڈ_ESC(20) /* CFF Top */
#تعریف_کرو Op_کوڈ_Post_Script	Make_Op_کوڈ_ESC(21) /* CFF Top */
#تعریف_کرو Op_کوڈ_ادھار_Font_Name	Make_Op_کوڈ_ESC(22) /* CFF Top */
#تعریف_کرو Op_کوڈ_ادھار_Font_Blend	Make_Op_کوڈ_ESC(23) /* CFF Top */
#تعریف_کرو Op_کوڈ_reserved_ESC24	Make_Op_کوڈ_ESC(24)
#تعریف_کرو Op_کوڈ_reserved_ESC25	Make_Op_کوڈ_ESC(25)
#تعریف_کرو Op_کوڈ_reserved_ESC26	Make_Op_کوڈ_ESC(26)
#تعریف_کرو Op_کوڈ_reserved_ESC27	Make_Op_کوڈ_ESC(27)
#تعریف_کرو Op_کوڈ_reserved_ESC28	Make_Op_کوڈ_ESC(28)
#تعریف_کرو Op_کوڈ_reserved_ESC29	Make_Op_کوڈ_ESC(29)
#تعریف_کرو Op_کوڈ_ROS		Make_Op_کوڈ_ESC(30) /* CFF Top_CID */
#تعریف_کرو Op_کوڈ_CIDFont_نسخہ	Make_Op_کوڈ_ESC(31) /* CFF Top_CID (0) */
#تعریف_کرو Op_کوڈ_CIDFont_Revision	Make_Op_کوڈ_ESC(32) /* CFF Top_CID (0) */
#تعریف_کرو Op_کوڈ_CIDFont_Type	Make_Op_کوڈ_ESC(33) /* CFF Top_CID (0) */
#تعریف_کرو Op_کوڈ_CIDCount		Make_Op_کوڈ_ESC(34) /* CFF Top_CID (8720) */
#تعریف_کرو Op_کوڈ_UIDBase		Make_Op_کوڈ_ESC(35) /* CFF Top_CID */
#تعریف_کرو Op_کوڈ_FDArray		Make_Op_کوڈ_ESC(36) /* CFF Top_CID, CFF2 Top */
#تعریف_کرو Op_کوڈ_FDSelect		Make_Op_کوڈ_ESC(37) /* CFF Top_CID, CFF2 Top */
#تعریف_کرو Op_کوڈ_Font_Name		Make_Op_کوڈ_ESC(38) /* CFF Top_CID */


/* === CharString operators === */

#تعریف_کرو Op_کوڈ_hstem		  1 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_Reserved2	  2
#تعریف_کرو Op_کوڈ_vstem		  3 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_vmoveto		  4 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_rlineto		  5 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_hlineto		  6 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_vlineto		  7 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_rrcurveto	  8 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_Reserved9	  9
#تعریف_کرو Op_کوڈ_callsubr		 10 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_return		 11 /* CFF */
//#define OpCode_escape		 12 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_Reserved13	 13
#تعریف_کرو Op_کوڈ_endchar		 14 /* CFF */
#تعریف_کرو Op_کوڈ_vsindexcs	 15 /* CFF2 */
#تعریف_کرو Op_کوڈ_blendcs		 16 /* CFF2 */
#تعریف_کرو Op_کوڈ_Reserved17	 17
#تعریف_کرو Op_کوڈ_hstemhm		 18 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_hintmask		 19 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_cntrmask		 20 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_rmoveto		 21 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_hmoveto		 22 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_vstemhm		 23 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_rcurveline	 24 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_rlinecurve	 25 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_vvcurveto	 26 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_hhcurveto	 27 /* CFF, CFF2 */
//#define OpCode_shortint	 28 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_callgsubr	 29 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_vhcurveto	 30 /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_hvcurveto	 31 /* CFF, CFF2 */

#تعریف_کرو Op_کوڈ_fixedcs		255 /* 32-bit fixed */

/* Two byte escape operators 12, (0-41) */
#تعریف_کرو Op_کوڈ_dotsection	Make_Op_کوڈ_ESC(0) /* CFF (obsoleted) */
#تعریف_کرو Op_کوڈ_Reserved_ESC1	Make_Op_کوڈ_ESC(1)
#تعریف_کرو Op_کوڈ_Reserved_ESC2	Make_Op_کوڈ_ESC(2)
#تعریف_کرو Op_کوڈ_and		Make_Op_کوڈ_ESC(3) /* CFF */
#تعریف_کرو Op_کوڈ_or		Make_Op_کوڈ_ESC(4) /* CFF */
#تعریف_کرو Op_کوڈ_not		Make_Op_کوڈ_ESC(5) /* CFF */
#تعریف_کرو Op_کوڈ_Reserved_ESC6	Make_Op_کوڈ_ESC(6)
#تعریف_کرو Op_کوڈ_Reserved_ESC7	Make_Op_کوڈ_ESC(7)
#تعریف_کرو Op_کوڈ_Reserved_ESC8	Make_Op_کوڈ_ESC(8)
#تعریف_کرو Op_کوڈ_abs		Make_Op_کوڈ_ESC(9) /* CFF */
#تعریف_کرو Op_کوڈ_جوڑو		Make_Op_کوڈ_ESC(10) /* CFF */
#تعریف_کرو Op_کوڈ_sub		Make_Op_کوڈ_ESC(11) /* CFF */
#تعریف_کرو Op_کوڈ_div		Make_Op_کوڈ_ESC(12) /* CFF */
#تعریف_کرو Op_کوڈ_Reserved_ESC13	Make_Op_کوڈ_ESC(13)
#تعریف_کرو Op_کوڈ_neg		Make_Op_کوڈ_ESC(14) /* CFF */
#تعریف_کرو Op_کوڈ_eq		Make_Op_کوڈ_ESC(15) /* CFF */
#تعریف_کرو Op_کوڈ_Reserved_ESC16	Make_Op_کوڈ_ESC(16)
#تعریف_کرو Op_کوڈ_Reserved_ESC17	Make_Op_کوڈ_ESC(17)
#تعریف_کرو Op_کوڈ_drop		Make_Op_کوڈ_ESC(18) /* CFF */
#تعریف_کرو Op_کوڈ_Reserved_ESC19	Make_Op_کوڈ_ESC(19)
#تعریف_کرو Op_کوڈ_put		Make_Op_کوڈ_ESC(20) /* CFF */
#تعریف_کرو Op_کوڈ_get		Make_Op_کوڈ_ESC(21) /* CFF */
#تعریف_کرو Op_کوڈ_ifelse		Make_Op_کوڈ_ESC(22) /* CFF */
#تعریف_کرو Op_کوڈ_random		Make_Op_کوڈ_ESC(23) /* CFF */
#تعریف_کرو Op_کوڈ_mul		Make_Op_کوڈ_ESC(24) /* CFF */
//#define OpCode_reservedESC25	Make_OpCode_ESC(25)
#تعریف_کرو Op_کوڈ_sqrt		Make_Op_کوڈ_ESC(26) /* CFF */
#تعریف_کرو Op_کوڈ_dup		Make_Op_کوڈ_ESC(27) /* CFF */
#تعریف_کرو Op_کوڈ_exch		Make_Op_کوڈ_ESC(28) /* CFF */
#تعریف_کرو Op_کوڈ_اشاریہ		Make_Op_کوڈ_ESC(29) /* CFF */
#تعریف_کرو Op_کوڈ_roll		Make_Op_کوڈ_ESC(30) /* CFF */
#تعریف_کرو Op_کوڈ_reserved_ESC31	Make_Op_کوڈ_ESC(31)
#تعریف_کرو Op_کوڈ_reserved_ESC32	Make_Op_کوڈ_ESC(32)
#تعریف_کرو Op_کوڈ_reserved_ESC33	Make_Op_کوڈ_ESC(33)
#تعریف_کرو Op_کوڈ_hflex		Make_Op_کوڈ_ESC(34) /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_flex		Make_Op_کوڈ_ESC(35) /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_hflex1		Make_Op_کوڈ_ESC(36) /* CFF, CFF2 */
#تعریف_کرو Op_کوڈ_flex1		Make_Op_کوڈ_ESC(37) /* CFF, CFF2 */


#تعریف_کرو Op_کوڈ_Invalid		0xFFFFu


بنية نمبر_t
{
  فراغ سیٹ_int (صحيح v)       { value = v; }
  منطقي منزل_int (صحيح *v) ثابت
  {
    إذا (unlikely (!(value >= INT_MIN && value <= ودھ_توں_ودھ_عدد)))
    {
      *v = 0;
      أرجع خطأ;
    }
    *v = (صحيح) value;
    أرجع سچ;
  }
  صحيح منزل_int () ثابت
  {
    صحيح v;
    إذا (likely (منزل_int (&v))) أرجع v;
    أرجع value < 0 ? INT_MIN : value > 0 ? ودھ_توں_ودھ_عدد : 0;
  }

  فراغ سیٹ_fixed (int32_t v) { value = v / 65536.0; }
  int32_t منزل_fixed () ثابت
  { أرجع (int32_t) hb_clamp (value * 65536.0, (مزدوج) INT32_MIN, (مزدوج) INT32_MAX); }

  فراغ سیٹ_real (مزدوج v)   { value = v; }
  مزدوج منزل_real () ثابت    { أرجع value; }

  منطقي وچ_int_range () ثابت
  { أرجع value >= INT16_MIN && value <= INT16_MAX && (مزدوج) (int16_t) value == value; }

  منطقي عامل >  (ثابت نمبر_t &n) ثابت { أرجع value > n.منزل_real (); }
  منطقي عامل <  (ثابت نمبر_t &n) ثابت { أرجع n > *هذا; }
  منطقي عامل >= (ثابت نمبر_t &n) ثابت { أرجع !(*هذا < n); }
  منطقي عامل <= (ثابت نمبر_t &n) ثابت { أرجع !(*هذا > n); }

  ثابت نمبر_t &عامل += (ثابت نمبر_t &n)
  {
    سیٹ_real (منزل_real () + n.منزل_real ());

    أرجع *هذا;
  }

  محمي:
  مزدوج value = 0.;
};

/* byte string */
بنية UnsizedByteStr : Unsized_Array_دا <HBUINT8>
{
  hb_ubytes_t as_ubytes (بنانشانی l) ثابت
  { أرجع hb_ubytes_t ((ثابت بنانشانی حرف *) هذا, l); }

  // encode 2-byte int (Dict/CharString) or 4-byte int (Dict)
  سانچا <قسم_ناں T, قسم_ناں V>
  ساكن منطقي serialize_int (hb_serialize_context_t *c, op_کوڈ_t intOp, V value)
  {
    TRACE_SERIALIZE (هذا);

    HBUINT8 *p = c->allocate_ناپ<HBUINT8> (1);
    إذا (unlikely (!p)) return_trace (خطأ);
    *p = intOp;

    T *ip = c->allocate_ناپ<T> (T::static_ناپ);
    إذا (unlikely (!ip)) return_trace (خطأ);
    return_trace (c->check_سونپو (*ip, value, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  سانچا <قسم_ناں V>
  ساكن منطقي serialize_int4 (hb_serialize_context_t *c, V value)
  { أرجع serialize_int<HBINT32> (c, Op_کوڈ_longintdict, value); }

  سانچا <قسم_ناں V>
  ساكن منطقي serialize_int2 (hb_serialize_context_t *c, V value)
  { أرجع serialize_int<HBINT16> (c, Op_کوڈ_shortint, value); }

  /* Defining null_size allows a Null object may be created. Should be safe because:
   * A descendent struct Dict uses a Null pointer to indicate a missing table,
   * checked before access.
   */
  DEFINE_ناپ_MIN(0);
};

/* A byte string associated with the current offset and an error condition */
بنية byte_str_ref_t
{
  byte_str_ref_t ()
    : str () {}

  byte_str_ref_t (ثابت hb_ubytes_t &str_, بنانشانی صحيح وتھ_3 = 0)
    : str (str_) { سیٹ_وتھ (وتھ_3); }

  فراغ reset (ثابت hb_ubytes_t &str_, بنانشانی صحيح وتھ_3 = 0)
  {
    str = str_;
    سیٹ_وتھ (وتھ_3);
  }

  ثابت بنانشانی حرف& عامل [] (صحيح i) {
    إذا (unlikely ((بنانشانی صحيح) (get_وتھ () + i) >= str.لمائی))
    {
      سیٹ_error ();
      أرجع Null (بنانشانی حرف);
    }
    أرجع str.arrayZ[get_وتھ () + i];
  }

  بنانشانی حرف سر_unchecked () ثابت { أرجع str.arrayZ[get_وتھ ()]; }

  /* Conversion to hb_ubytes_t */
  عامل hb_ubytes_t () ثابت { أرجع str.sub_array (get_وتھ ()); }

  hb_ubytes_t sub_array (بنانشانی صحيح وتھ_3, بنانشانی صحيح len_) ثابت
  { أرجع str.sub_array (وتھ_3, len_); }

  منطقي avail (بنانشانی صحيح گنتی=1) ثابت
  { أرجع get_وتھ () + گنتی <= str.لمائی; }
  فراغ inc (بنانشانی صحيح گنتی=1)
  {
    /* Automatically puts us in error if count is out-of-range. */
    سیٹ_وتھ (get_وتھ () + گنتی);
  }

  /* We (ab)use ubytes backwards_length as a cursor (called offset),
   * as well as to store error condition. */

  بنانشانی get_وتھ () ثابت { أرجع str.backwards_لمائی; }
  فراغ سیٹ_وتھ (بنانشانی وتھ) { str.backwards_لمائی = وتھ; }

  فراغ سیٹ_error ()      { str.backwards_لمائی = str.لمائی + 1; }
  منطقي وچ_error () ثابت { أرجع str.backwards_لمائی > str.لمائی; }

  بنانشانی کل_ناپ () ثابت { أرجع str.لمائی; }

  محمي:
  hb_ubytes_t       str;
};

/*ڈھیر*/
سانچا <قسم_ناں ELEM, صحيح حد_بندی_2>
بنية cff_ڈھیر_t
{
  ELEM& عامل [] (بنانشانی صحيح i)
  {
    إذا (unlikely (i >= لمائی))
    {
      سیٹ_error ();
      أرجع Crap (ELEM);
    }
    أرجع arrayZ[i];
  }

  فراغ push (ثابت ELEM &v)
  {
    إذا (likely (لمائی < حد_بندی_2))
      arrayZ[لمائی++] = v;
    وإلا
      سیٹ_error ();
  }
  ELEM &push ()
  {
    إذا (likely (لمائی < حد_بندی_2))
      أرجع arrayZ[لمائی++];
    وإلا
    {
      سیٹ_error ();
      أرجع Crap (ELEM);
    }
  }

  ELEM& نکالو ()
  {
    إذا (likely (لمائی > 0))
      أرجع arrayZ[--لمائی];
    وإلا
    {
      سیٹ_error ();
      أرجع Crap (ELEM);
    }
  }
  فراغ نکالو (بنانشانی صحيح n)
  {
    إذا (likely (لمائی >= n))
      لمائی -= n;
    وإلا
      سیٹ_error ();
  }

  ثابت ELEM& peek ()
  {
    إذا (unlikely (لمائی == 0))
    {
      سیٹ_error ();
      أرجع Null (ELEM);
    }
    أرجع arrayZ[لمائی - 1];
  }

  فراغ unpop ()
  {
    إذا (likely (لمائی < حد_بندی_2))
      لمائی++;
    وإلا
      سیٹ_error ();
  }

  فراغ clear () { لمائی = 0; }

  منطقي وچ_error () ثابت { أرجع (error); }
  فراغ سیٹ_error ()      { error = سچ; }

  بنانشانی صحيح get_گنتی () ثابت { أرجع لمائی; }
  منطقي is_empty () ثابت          { أرجع !لمائی; }

  hb_array_t<ثابت ELEM> sub_array (بنانشانی شروع, بنانشانی لمائی) ثابت
  { أرجع hb_array_t<ثابت ELEM> (arrayZ).sub_array (شروع, لمائی); }

  منطقي error = خطأ;
  بنانشانی صحيح لمائی = 0;
  ELEM arrayZ[حد_بندی_2];
};

/* argument stack */
سانچا <قسم_ناں ARG=نمبر_t>
بنية arg_ڈھیر_t : cff_ڈھیر_t<ARG, 513>
{
  فراغ ڈالو_int (صحيح v)
  {
    ARG &n = S::push ();
    n.سیٹ_int (v);
  }

  فراغ ڈالو_fixed (int32_t v)
  {
    ARG &n = S::push ();
    n.سیٹ_fixed (v);
  }

  فراغ ڈالو_real (مزدوج v)
  {
    ARG &n = S::push ();
    n.سیٹ_real (v);
  }

  ARG& نکالو_عدد () { أرجع هذا->نکالو (); }

  صحيح نکالو_int ()
  {
    صحيح v;
    إذا (unlikely (!هذا->نکالو ().منزل_int (&v)))
      S::سیٹ_error ();
    أرجع v;
  }

  بنانشانی صحيح نکالو_uint ()
  {
    صحيح i = نکالو_int ();
    إذا (unlikely (i < 0))
    {
      i = 0;
      S::سیٹ_error ();
    }
    أرجع (بنانشانی) i;
  }

  فراغ ڈالو_longint_from_substr (byte_str_ref_t& str_ref)
  {
    ڈالو_int ((صحيح) ((uint32_t (str_ref[0]) << 24) | (str_ref[1] << 16) | (str_ref[2] << 8) | (str_ref[3])));
    str_ref.inc (4);
  }

  منطقي ڈالو_fixed_from_substr (byte_str_ref_t& str_ref)
  {
    إذا (unlikely (!str_ref.avail (4)))
      أرجع خطأ;
    ڈالو_fixed ((int32_t)*(ثابت HBUINT32*)&str_ref[0]);
    str_ref.inc (4);
    أرجع سچ;
  }

  خاص:
  تعريفنوع cff_ڈھیر_t<ARG, 513> S;
};

/* an operator prefixed by its operands in a byte string */
بنية op_str_t
{
  /* This used to have a hb_ubytes_t. Using a pointer and length
   * in a particular order, saves 8 bytes in this struct and more
   * in our parsed_cs_op_t subclass. */

  ثابت بنانشانی حرف *ptr = خالی_سوچک;

  op_کوڈ_t  op = Op_کوڈ_Invalid;

  uint8_t لمائی = 0;
};

/* base of OP_SERIALIZER */
بنية op_serializer_t
{
  محمي:
  منطقي copy_opstr (hb_serialize_context_t *c, ثابت op_str_t& opstr) ثابت
  {
    TRACE_SERIALIZE (هذا);

    بنانشانی حرف *d = c->allocate_ناپ<بنانشانی حرف> (opstr.لمائی);
    إذا (unlikely (!d)) return_trace (خطأ);
    /* Faster than hb_memcpy for small strings. */
    لكل (بنانشانی i = 0; i < opstr.لمائی; i++)
      d[i] = opstr.ptr[i];
    return_trace (سچ);
  }
};

سانچا <قسم_ناں VAL>
بنية parsed_اقدار_t
{
  فراغ init ()
  {
    op_شروع = 0;
    values.init ();
  }
  فراغ fini () { values.fini (); }

  فراغ alloc (بنانشانی n)
  {
    values.alloc_exact (n);
  }

  فراغ جوڑو_op (op_کوڈ_t op, ثابت byte_str_ref_t& str_ref = byte_str_ref_t (), ثابت VAL &v = VAL ())
  {
    VAL *val = values.push (v);
    val->op = op;
    خودکار arr = str_ref.sub_array (op_شروع, str_ref.get_وتھ () - op_شروع);
    val->ptr = arr.arrayZ;
    val->لمائی = arr.لمائی;
    op_شروع = str_ref.get_وتھ ();
  }

  منطقي has_op (op_کوڈ_t op) ثابت
  {
    لكل (ثابت خودکار& v : values)
      إذا (v.op == op) أرجع سچ;
    أرجع خطأ;
  }

  بنانشانی get_گنتی () ثابت { أرجع values.لمائی; }
  ثابت VAL &عامل [] (بنانشانی صحيح i) ثابت { أرجع values[i]; }

  بنانشانی صحيح       op_شروع;
  hb_سمتیہ_t<VAL>   values;
};

سانچا <قسم_ناں ARG=نمبر_t>
بنية interp_env_t
{
  interp_env_t () {}
  interp_env_t (ثابت hb_ubytes_t &str_)
  {
    str_ref.reset (str_);
  }
  منطقي وچ_error () ثابت
  { أرجع str_ref.وچ_error () || arg_ڈھیر.وچ_error (); }

  فراغ سیٹ_error () { str_ref.سیٹ_error (); }

  op_کوڈ_t fetch_op ()
  {
    op_کوڈ_t  op = Op_کوڈ_Invalid;
    إذا (unlikely (!str_ref.avail ()))
      أرجع Op_کوڈ_Invalid;
    op = (op_کوڈ_t) str_ref.سر_unchecked ();
    str_ref.inc ();
    إذا (op == Op_کوڈ_escape) {
      إذا (unlikely (!str_ref.avail ()))
	أرجع Op_کوڈ_Invalid;
      op = Make_Op_کوڈ_ESC (str_ref.سر_unchecked ());
      str_ref.inc ();
    }
    أرجع op;
  }

  ثابت ARG& eval_arg (بنانشانی صحيح i) { أرجع arg_ڈھیر[i]; }

  ARG& نکالو_arg () { أرجع arg_ڈھیر.نکالو (); }
  فراغ نکالو_n_دلیلاں (بنانشانی صحيح n) { arg_ڈھیر.نکالو (n); }

  فراغ clear_دلیلاں () { نکالو_n_دلیلاں (arg_ڈھیر.get_گنتی ()); }

  byte_str_ref_t
		str_ref;
  arg_ڈھیر_t<ARG>
		arg_ڈھیر;
};

ورتوں عدد_interp_env_t =  interp_env_t<>;

سانچا <قسم_ناں ARG=نمبر_t>
بنية opset_t
{
  ساكن فراغ process_op (op_کوڈ_t op, interp_env_t<ARG>& env)
  {
    بدل (op) {
      حالة Op_کوڈ_shortint:
	env.arg_ڈھیر.ڈالو_int ((int16_t)((env.str_ref[0] << 8) | env.str_ref[1]));
	env.str_ref.inc (2);
	اكسر;

      حالة Op_کوڈ_Two_Byte_Pos_Int0: حالة Op_کوڈ_Two_Byte_Pos_Int1:
      حالة Op_کوڈ_Two_Byte_Pos_Int2: حالة Op_کوڈ_Two_Byte_Pos_Int3:
	env.arg_ڈھیر.ڈالو_int ((int16_t)((op - Op_کوڈ_Two_Byte_Pos_Int0) * 256 + env.str_ref[0] + 108));
	env.str_ref.inc ();
	اكسر;

      حالة Op_کوڈ_Two_Byte_Neg_Int0: حالة Op_کوڈ_Two_Byte_Neg_Int1:
      حالة Op_کوڈ_Two_Byte_Neg_Int2: حالة Op_کوڈ_Two_Byte_Neg_Int3:
	env.arg_ڈھیر.ڈالو_int ((-(int16_t)(op - Op_کوڈ_Two_Byte_Neg_Int0) * 256 - env.str_ref[0] - 108));
	env.str_ref.inc ();
	اكسر;

      افتراضي:
	/* 1-byte integer */
	إذا (likely ((Op_کوڈ_One_Byte_Int_پہلا <= op) && (op <= Op_کوڈ_One_Byte_Int_آخری)))
	{
	  env.arg_ڈھیر.ڈالو_int ((صحيح)op - 139);
	} وإلا {
	  /* invalid unknown operator */
	  env.clear_دلیلاں ();
	}
	اكسر;
    }
  }
};

سانچا <قسم_ناں ENV>
بنية interpreter_t
{
  interpreter_t (ENV& env_) : env (env_) {}
  ENV& env;
};

} /* namespace CFF */

#جے_انت /* HB_CFF_INTERP_COMMON_HH */
