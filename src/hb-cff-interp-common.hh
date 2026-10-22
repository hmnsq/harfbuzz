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
#که_نه_تعریف_شوی HB_CFF_INTERP_COMMON_HH
#تعریف_کړه HB_CFF_INTERP_COMMON_HH

خارجي HB_دننی ثابت بینښه حرف *endchar_str;

نطاق CFF {

کارول نطاق OT;

تعريفنوع بینښه صحيح op_کوډ_t;


/* === Dict operators === */

/* One byte operators (0-31) */
#تعریف_کړه Op_کوډ_نسخه		  0 /* CFF Top */
#تعریف_کړه Op_کوډ_Notice		  1 /* CFF Top */
#تعریف_کړه Op_کوډ_ډک_Name		  2 /* CFF Top */
#تعریف_کړه Op_کوډ_Family_Name	  3 /* CFF Top */
#تعریف_کړه Op_کوډ_وزن		  4 /* CFF Top */
#تعریف_کړه Op_کوډ_Font_BBox		  5 /* CFF Top */
#تعریف_کړه Op_کوډ_Blue_ارزښتونه	  6 /* CFF Private, CFF2 Private */
#تعریف_کړه Op_کوډ_Other_Blues	  7 /* CFF Private, CFF2 Private */
#تعریف_کړه Op_کوډ_Family_Blues	  8 /* CFF Private, CFF2 Private */
#تعریف_کړه Op_کوډ_Family_Other_Blues	  9 /* CFF Private, CFF2 Private */
#تعریف_کړه Op_کوډ_Std_HW		 10 /* CFF Private, CFF2 Private */
#تعریف_کړه Op_کوډ_Std_VW		 11 /* CFF Private, CFF2 Private */
#تعریف_کړه Op_کوډ_escape		 12 /* All. Shared with CS */
#تعریف_کړه Op_کوډ_Unique_ID		 13 /* CFF Top */
#تعریف_کړه Op_کوډ_XUID		 14 /* CFF Top */
#تعریف_کړه Op_کوډ_charset		 15 /* CFF Top (0) */
#تعریف_کړه Op_کوډ_Encoding		 16 /* CFF Top (0) */
#تعریف_کړه Op_کوډ_Char_مزي_ګانې	 17 /* CFF Top, CFF2 Top */
#تعریف_کړه Op_کوډ_شخصي		 18 /* CFF Top, CFF2 FD */
#تعریف_کړه Op_کوډ_Subrs		 19 /* CFF Private, CFF2 Private */
#تعریف_کړه Op_کوډ_default_پلنوالی_X	 20 /* CFF Private (0) */
#تعریف_کړه Op_کوډ_nominal_پلنوالی_X	 21 /* CFF Private (0) */
#تعریف_کړه Op_کوډ_vsindexdict	 22 /* CFF2 Private/CS */
#تعریف_کړه Op_کوډ_blenddict	 23 /* CFF2 Private/CS */
#تعریف_کړه Op_کوډ_vstore		 24 /* CFF2 Top */
#تعریف_کړه Op_کوډ_reserved25	 25
#تعریف_کړه Op_کوډ_reserved26	 26
#تعریف_کړه Op_کوډ_reserved27	 27

/* Numbers */
#تعریف_کړه Op_کوډ_shortint		 28 /* 16-bit integer, All */
#تعریف_کړه Op_کوډ_longintdict	 29 /* 32-bit integer, All */
#تعریف_کړه Op_کوډ_BCD		 30 /* Real number, CFF2 Top/FD */
#تعریف_کړه Op_کوډ_reserved31	 31

/* 1-byte integers */
#تعریف_کړه Op_کوډ_One_Byte_Int_لومړی	 32 /* All. beginning of the range of first byte ints */
#تعریف_کړه Op_کوډ_One_Byte_Int_وروستی	246 /* All. ending of the range of first byte int */

/* 2-byte integers */
#تعریف_کړه Op_کوډ_Two_Byte_Pos_Int0	247 /* All. first byte of two byte positive int (+108 to +1131) */
#تعریف_کړه Op_کوډ_Two_Byte_Pos_Int1	248
#تعریف_کړه Op_کوډ_Two_Byte_Pos_Int2	249
#تعریف_کړه Op_کوډ_Two_Byte_Pos_Int3	250

#تعریف_کړه Op_کوډ_Two_Byte_Neg_Int0	251 /* All. first byte of two byte negative int (-1131 to -108) */
#تعریف_کړه Op_کوډ_Two_Byte_Neg_Int1	252
#تعریف_کړه Op_کوډ_Two_Byte_Neg_Int2	253
#تعریف_کړه Op_کوډ_Two_Byte_Neg_Int3	254

/* Two byte escape operators 12, (0-41) */
#تعریف_کړه Op_کوډ_ESC_بنسټ		256
#تعریف_کړه Make_Op_کوډ_ESC(byte2)	((op_کوډ_t)(Op_کوډ_ESC_بنسټ + (byte2)))

مضمن op_کوډ_t Unmake_Op_کوډ_ESC (op_کوډ_t op)  { أرجع (op_کوډ_t)(op - Op_کوډ_ESC_بنسټ); }
مضمن منطقي Is_Op_کوډ_ESC (op_کوډ_t op) { أرجع op >= Op_کوډ_ESC_بنسټ; }
مضمن بینښه صحيح Op_کوډ_اندازه (op_کوډ_t op) { أرجع Is_Op_کوډ_ESC (op) ? 2: 1; }

#تعریف_کړه Op_کوډ_Copyright	Make_Op_کوډ_ESC(0) /* CFF Top */
#تعریف_کړه Op_کوډ_is_Fixed_Pitch	Make_Op_کوډ_ESC(1) /* CFF Top (false) */
#تعریف_کړه Op_کوډ_Italic_Angle	Make_Op_کوډ_ESC(2) /* CFF Top (0) */
#تعریف_کړه Op_کوډ_Underline_Position Make_Op_کوډ_ESC(3) /* CFF Top (-100) */
#تعریف_کړه Op_کوډ_Underline_Thickness Make_Op_کوډ_ESC(4) /* CFF Top (50) */
#تعریف_کړه Op_کوډ_Paint_Type	Make_Op_کوډ_ESC(5) /* CFF Top (0) */
#تعریف_کړه Op_کوډ_Charstring_Type	Make_Op_کوډ_ESC(6) /* CFF Top (2) */
#تعریف_کړه Op_کوډ_Font_ماتریکس	Make_Op_کوډ_ESC(7) /* CFF Top, CFF2 Top (.001 0 0 .001 0 0)*/
#تعریف_کړه Op_کوډ_Stroke_پلنوالی	Make_Op_کوډ_ESC(8) /* CFF Top (0) */
#تعریف_کړه Op_کوډ_Blue_Scale	Make_Op_کوډ_ESC(9) /* CFF Private, CFF2 Private (0.039625) */
#تعریف_کړه Op_کوډ_Blue_ښويدنه	Make_Op_کوډ_ESC(10) /* CFF Private, CFF2 Private (7) */
#تعریف_کړه Op_کوډ_Blue_Fuzz		Make_Op_کوډ_ESC(11) /* CFF Private, CFF2 Private (1) */
#تعریف_کړه Op_کوډ_Stem_Snap_H	Make_Op_کوډ_ESC(12) /* CFF Private, CFF2 Private */
#تعریف_کړه Op_کوډ_Stem_Snap_V	Make_Op_کوډ_ESC(13) /* CFF Private, CFF2 Private */
#تعریف_کړه Op_کوډ_Force_Bold	Make_Op_کوډ_ESC(14) /* CFF Private (false) */
#تعریف_کړه Op_کوډ_reserved_ESC15	Make_Op_کوډ_ESC(15)
#تعریف_کړه Op_کوډ_reserved_ESC16	Make_Op_کوډ_ESC(16)
#تعریف_کړه Op_کوډ_Language_ډله	Make_Op_کوډ_ESC(17) /* CFF Private, CFF2 Private (0) */
#تعریف_کړه Op_کوډ_Expansion_ضریب	Make_Op_کوډ_ESC(18) /* CFF Private, CFF2 Private (0.06) */
#تعریف_کړه Op_کوډ_initial_Random_تخم Make_Op_کوډ_ESC(19) /* CFF Private (0) */
#تعریف_کړه Op_کوډ_Synthetic_بنسټ	Make_Op_کوډ_ESC(20) /* CFF Top */
#تعریف_کړه Op_کوډ_Post_Script	Make_Op_کوډ_ESC(21) /* CFF Top */
#تعریف_کړه Op_کوډ_بنسټ_Font_Name	Make_Op_کوډ_ESC(22) /* CFF Top */
#تعریف_کړه Op_کوډ_بنسټ_Font_Blend	Make_Op_کوډ_ESC(23) /* CFF Top */
#تعریف_کړه Op_کوډ_reserved_ESC24	Make_Op_کوډ_ESC(24)
#تعریف_کړه Op_کوډ_reserved_ESC25	Make_Op_کوډ_ESC(25)
#تعریف_کړه Op_کوډ_reserved_ESC26	Make_Op_کوډ_ESC(26)
#تعریف_کړه Op_کوډ_reserved_ESC27	Make_Op_کوډ_ESC(27)
#تعریف_کړه Op_کوډ_reserved_ESC28	Make_Op_کوډ_ESC(28)
#تعریف_کړه Op_کوډ_reserved_ESC29	Make_Op_کوډ_ESC(29)
#تعریف_کړه Op_کوډ_ROS		Make_Op_کوډ_ESC(30) /* CFF Top_CID */
#تعریف_کړه Op_کوډ_CIDFont_نسخه	Make_Op_کوډ_ESC(31) /* CFF Top_CID (0) */
#تعریف_کړه Op_کوډ_CIDFont_Revision	Make_Op_کوډ_ESC(32) /* CFF Top_CID (0) */
#تعریف_کړه Op_کوډ_CIDFont_Type	Make_Op_کوډ_ESC(33) /* CFF Top_CID (0) */
#تعریف_کړه Op_کوډ_CIDCount		Make_Op_کوډ_ESC(34) /* CFF Top_CID (8720) */
#تعریف_کړه Op_کوډ_UIDBase		Make_Op_کوډ_ESC(35) /* CFF Top_CID */
#تعریف_کړه Op_کوډ_FDArray		Make_Op_کوډ_ESC(36) /* CFF Top_CID, CFF2 Top */
#تعریف_کړه Op_کوډ_FDSelect		Make_Op_کوډ_ESC(37) /* CFF Top_CID, CFF2 Top */
#تعریف_کړه Op_کوډ_Font_Name		Make_Op_کوډ_ESC(38) /* CFF Top_CID */


/* === CharString operators === */

#تعریف_کړه Op_کوډ_hstem		  1 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_Reserved2	  2
#تعریف_کړه Op_کوډ_vstem		  3 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_vmoveto		  4 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_rlineto		  5 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_hlineto		  6 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_vlineto		  7 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_rrcurveto	  8 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_Reserved9	  9
#تعریف_کړه Op_کوډ_callsubr		 10 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_return		 11 /* CFF */
//#define OpCode_escape		 12 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_Reserved13	 13
#تعریف_کړه Op_کوډ_endchar		 14 /* CFF */
#تعریف_کړه Op_کوډ_vsindexcs	 15 /* CFF2 */
#تعریف_کړه Op_کوډ_blendcs		 16 /* CFF2 */
#تعریف_کړه Op_کوډ_Reserved17	 17
#تعریف_کړه Op_کوډ_hstemhm		 18 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_hintmask		 19 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_cntrmask		 20 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_rmoveto		 21 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_hmoveto		 22 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_vstemhm		 23 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_rcurveline	 24 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_rlinecurve	 25 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_vvcurveto	 26 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_hhcurveto	 27 /* CFF, CFF2 */
//#define OpCode_shortint	 28 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_callgsubr	 29 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_vhcurveto	 30 /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_hvcurveto	 31 /* CFF, CFF2 */

#تعریف_کړه Op_کوډ_fixedcs		255 /* 32-bit fixed */

/* Two byte escape operators 12, (0-41) */
#تعریف_کړه Op_کوډ_dotsection	Make_Op_کوډ_ESC(0) /* CFF (obsoleted) */
#تعریف_کړه Op_کوډ_Reserved_ESC1	Make_Op_کوډ_ESC(1)
#تعریف_کړه Op_کوډ_Reserved_ESC2	Make_Op_کوډ_ESC(2)
#تعریف_کړه Op_کوډ_and		Make_Op_کوډ_ESC(3) /* CFF */
#تعریف_کړه Op_کوډ_or		Make_Op_کوډ_ESC(4) /* CFF */
#تعریف_کړه Op_کوډ_not		Make_Op_کوډ_ESC(5) /* CFF */
#تعریف_کړه Op_کوډ_Reserved_ESC6	Make_Op_کوډ_ESC(6)
#تعریف_کړه Op_کوډ_Reserved_ESC7	Make_Op_کوډ_ESC(7)
#تعریف_کړه Op_کوډ_Reserved_ESC8	Make_Op_کوډ_ESC(8)
#تعریف_کړه Op_کوډ_abs		Make_Op_کوډ_ESC(9) /* CFF */
#تعریف_کړه Op_کوډ_جمع_کړه		Make_Op_کوډ_ESC(10) /* CFF */
#تعریف_کړه Op_کوډ_sub		Make_Op_کوډ_ESC(11) /* CFF */
#تعریف_کړه Op_کوډ_div		Make_Op_کوډ_ESC(12) /* CFF */
#تعریف_کړه Op_کوډ_Reserved_ESC13	Make_Op_کوډ_ESC(13)
#تعریف_کړه Op_کوډ_neg		Make_Op_کوډ_ESC(14) /* CFF */
#تعریف_کړه Op_کوډ_eq		Make_Op_کوډ_ESC(15) /* CFF */
#تعریف_کړه Op_کوډ_Reserved_ESC16	Make_Op_کوډ_ESC(16)
#تعریف_کړه Op_کوډ_Reserved_ESC17	Make_Op_کوډ_ESC(17)
#تعریف_کړه Op_کوډ_drop		Make_Op_کوډ_ESC(18) /* CFF */
#تعریف_کړه Op_کوډ_Reserved_ESC19	Make_Op_کوډ_ESC(19)
#تعریف_کړه Op_کوډ_put		Make_Op_کوډ_ESC(20) /* CFF */
#تعریف_کړه Op_کوډ_get		Make_Op_کوډ_ESC(21) /* CFF */
#تعریف_کړه Op_کوډ_ifelse		Make_Op_کوډ_ESC(22) /* CFF */
#تعریف_کړه Op_کوډ_random		Make_Op_کوډ_ESC(23) /* CFF */
#تعریف_کړه Op_کوډ_mul		Make_Op_کوډ_ESC(24) /* CFF */
//#define OpCode_reservedESC25	Make_OpCode_ESC(25)
#تعریف_کړه Op_کوډ_sqrt		Make_Op_کوډ_ESC(26) /* CFF */
#تعریف_کړه Op_کوډ_dup		Make_Op_کوډ_ESC(27) /* CFF */
#تعریف_کړه Op_کوډ_exch		Make_Op_کوډ_ESC(28) /* CFF */
#تعریف_کړه Op_کوډ_شاخص		Make_Op_کوډ_ESC(29) /* CFF */
#تعریف_کړه Op_کوډ_roll		Make_Op_کوډ_ESC(30) /* CFF */
#تعریف_کړه Op_کوډ_reserved_ESC31	Make_Op_کوډ_ESC(31)
#تعریف_کړه Op_کوډ_reserved_ESC32	Make_Op_کوډ_ESC(32)
#تعریف_کړه Op_کوډ_reserved_ESC33	Make_Op_کوډ_ESC(33)
#تعریف_کړه Op_کوډ_hflex		Make_Op_کوډ_ESC(34) /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_flex		Make_Op_کوډ_ESC(35) /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_hflex1		Make_Op_کوډ_ESC(36) /* CFF, CFF2 */
#تعریف_کړه Op_کوډ_flex1		Make_Op_کوډ_ESC(37) /* CFF, CFF2 */


#تعریف_کړه Op_کوډ_Invalid		0xFFFFu


بنية شمېره_t
{
  فراغ سېټ_int (صحيح v)       { value = v; }
  منطقي منزل_int (صحيح *v) ثابت
  {
    إذا (unlikely (!(value >= INT_MIN && value <= لوی_عدد)))
    {
      *v = 0;
      أرجع خطأ;
    }
    *v = (صحيح) value;
    أرجع ریښتیا;
  }
  صحيح منزل_int () ثابت
  {
    صحيح v;
    إذا (likely (منزل_int (&v))) أرجع v;
    أرجع value < 0 ? INT_MIN : value > 0 ? لوی_عدد : 0;
  }

  فراغ سېټ_fixed (int32_t v) { value = v / 65536.0; }
  int32_t منزل_fixed () ثابت
  { أرجع (int32_t) hb_clamp (value * 65536.0, (مزدوج) INT32_MIN, (مزدوج) INT32_MAX); }

  فراغ سېټ_real (مزدوج v)   { value = v; }
  مزدوج منزل_real () ثابت    { أرجع value; }

  منطقي دننه_int_range () ثابت
  { أرجع value >= INT16_MIN && value <= INT16_MAX && (مزدوج) (int16_t) value == value; }

  منطقي چلوونکی >  (ثابت شمېره_t &n) ثابت { أرجع value > n.منزل_real (); }
  منطقي چلوونکی <  (ثابت شمېره_t &n) ثابت { أرجع n > *هذا; }
  منطقي چلوونکی >= (ثابت شمېره_t &n) ثابت { أرجع !(*هذا < n); }
  منطقي چلوونکی <= (ثابت شمېره_t &n) ثابت { أرجع !(*هذا > n); }

  ثابت شمېره_t &چلوونکی += (ثابت شمېره_t &n)
  {
    سېټ_real (منزل_real () + n.منزل_real ());

    أرجع *هذا;
  }

  محمي:
  مزدوج value = 0.;
};

/* byte string */
بنية UnsizedByteStr : Unsized_Array_د <HBUINT8>
{
  hb_ubytes_t as_ubytes (بینښه l) ثابت
  { أرجع hb_ubytes_t ((ثابت بینښه حرف *) هذا, l); }

  // encode 2-byte int (Dict/CharString) or 4-byte int (Dict)
  کينډۍ <ډول_نوم T, ډول_نوم V>
  ساكن منطقي serialize_int (hb_serialize_context_t *c, op_کوډ_t intOp, V value)
  {
    TRACE_SERIALIZE (هذا);

    HBUINT8 *p = c->allocate_اندازه<HBUINT8> (1);
    إذا (unlikely (!p)) return_trace (خطأ);
    *p = intOp;

    T *ip = c->allocate_اندازه<T> (T::static_اندازه);
    إذا (unlikely (!ip)) return_trace (خطأ);
    return_trace (c->check_ټاکل (*ip, value, HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  کينډۍ <ډول_نوم V>
  ساكن منطقي serialize_int4 (hb_serialize_context_t *c, V value)
  { أرجع serialize_int<HBINT32> (c, Op_کوډ_longintdict, value); }

  کينډۍ <ډول_نوم V>
  ساكن منطقي serialize_int2 (hb_serialize_context_t *c, V value)
  { أرجع serialize_int<HBINT16> (c, Op_کوډ_shortint, value); }

  /* Defining null_size allows a Null object may be created. Should be safe because:
   * A descendent struct Dict uses a Null pointer to indicate a missing table,
   * checked before access.
   */
  DEFINE_اندازه_MIN(0);
};

/* A byte string associated with the current offset and an error condition */
بنية byte_str_ref_t
{
  byte_str_ref_t ()
    : str () {}

  byte_str_ref_t (ثابت hb_ubytes_t &str_, بینښه صحيح دننه_واټن_3 = 0)
    : str (str_) { سېټ_دننه_واټن (دننه_واټن_3); }

  فراغ reset (ثابت hb_ubytes_t &str_, بینښه صحيح دننه_واټن_3 = 0)
  {
    str = str_;
    سېټ_دننه_واټن (دننه_واټن_3);
  }

  ثابت بینښه حرف& چلوونکی [] (صحيح i) {
    إذا (unlikely ((بینښه صحيح) (get_دننه_واټن () + i) >= str.اوږدتیا))
    {
      سېټ_error ();
      أرجع Null (بینښه حرف);
    }
    أرجع str.arrayZ[get_دننه_واټن () + i];
  }

  بینښه حرف سر_unchecked () ثابت { أرجع str.arrayZ[get_دننه_واټن ()]; }

  /* Conversion to hb_ubytes_t */
  چلوونکی hb_ubytes_t () ثابت { أرجع str.sub_array (get_دننه_واټن ()); }

  hb_ubytes_t sub_array (بینښه صحيح دننه_واټن_3, بینښه صحيح len_) ثابت
  { أرجع str.sub_array (دننه_واټن_3, len_); }

  منطقي avail (بینښه صحيح شمېر=1) ثابت
  { أرجع get_دننه_واټن () + شمېر <= str.اوږدتیا; }
  فراغ inc (بینښه صحيح شمېر=1)
  {
    /* Automatically puts us in error if count is out-of-range. */
    سېټ_دننه_واټن (get_دننه_واټن () + شمېر);
  }

  /* We (ab)use ubytes backwards_length as a cursor (called offset),
   * as well as to store error condition. */

  بینښه get_دننه_واټن () ثابت { أرجع str.backwards_اوږدتیا; }
  فراغ سېټ_دننه_واټن (بینښه دننه_واټن) { str.backwards_اوږدتیا = دننه_واټن; }

  فراغ سېټ_error ()      { str.backwards_اوږدتیا = str.اوږدتیا + 1; }
  منطقي دننه_error () ثابت { أرجع str.backwards_اوږدتیا > str.اوږدتیا; }

  بینښه ټول_اندازه () ثابت { أرجع str.اوږدتیا; }

  محمي:
  hb_ubytes_t       str;
};

/*ډېری*/
کينډۍ <ډول_نوم ELEM, صحيح حد_2>
بنية cff_ډېری_t
{
  ELEM& چلوونکی [] (بینښه صحيح i)
  {
    إذا (unlikely (i >= اوږدتیا))
    {
      سېټ_error ();
      أرجع Crap (ELEM);
    }
    أرجع arrayZ[i];
  }

  فراغ push (ثابت ELEM &v)
  {
    إذا (likely (اوږدتیا < حد_2))
      arrayZ[اوږدتیا++] = v;
    وإلا
      سېټ_error ();
  }
  ELEM &push ()
  {
    إذا (likely (اوږدتیا < حد_2))
      أرجع arrayZ[اوږدتیا++];
    وإلا
    {
      سېټ_error ();
      أرجع Crap (ELEM);
    }
  }

  ELEM& راوباسه ()
  {
    إذا (likely (اوږدتیا > 0))
      أرجع arrayZ[--اوږدتیا];
    وإلا
    {
      سېټ_error ();
      أرجع Crap (ELEM);
    }
  }
  فراغ راوباسه (بینښه صحيح n)
  {
    إذا (likely (اوږدتیا >= n))
      اوږدتیا -= n;
    وإلا
      سېټ_error ();
  }

  ثابت ELEM& peek ()
  {
    إذا (unlikely (اوږدتیا == 0))
    {
      سېټ_error ();
      أرجع Null (ELEM);
    }
    أرجع arrayZ[اوږدتیا - 1];
  }

  فراغ unpop ()
  {
    إذا (likely (اوږدتیا < حد_2))
      اوږدتیا++;
    وإلا
      سېټ_error ();
  }

  فراغ clear () { اوږدتیا = 0; }

  منطقي دننه_error () ثابت { أرجع (error); }
  فراغ سېټ_error ()      { error = ریښتیا; }

  بینښه صحيح get_شمېر () ثابت { أرجع اوږدتیا; }
  منطقي is_empty () ثابت          { أرجع !اوږدتیا; }

  hb_array_t<ثابت ELEM> sub_array (بینښه پیل, بینښه اوږدتیا) ثابت
  { أرجع hb_array_t<ثابت ELEM> (arrayZ).sub_array (پیل, اوږدتیا); }

  منطقي error = خطأ;
  بینښه صحيح اوږدتیا = 0;
  ELEM arrayZ[حد_2];
};

/* argument stack */
کينډۍ <ډول_نوم ARG=شمېره_t>
بنية arg_ډېری_t : cff_ډېری_t<ARG, 513>
{
  فراغ ټېل_int (صحيح v)
  {
    ARG &n = S::push ();
    n.سېټ_int (v);
  }

  فراغ ټېل_fixed (int32_t v)
  {
    ARG &n = S::push ();
    n.سېټ_fixed (v);
  }

  فراغ ټېل_real (مزدوج v)
  {
    ARG &n = S::push ();
    n.سېټ_real (v);
  }

  ARG& راوباسه_عدد () { أرجع هذا->راوباسه (); }

  صحيح راوباسه_int ()
  {
    صحيح v;
    إذا (unlikely (!هذا->راوباسه ().منزل_int (&v)))
      S::سېټ_error ();
    أرجع v;
  }

  بینښه صحيح راوباسه_uint ()
  {
    صحيح i = راوباسه_int ();
    إذا (unlikely (i < 0))
    {
      i = 0;
      S::سېټ_error ();
    }
    أرجع (بینښه) i;
  }

  فراغ ټېل_longint_from_substr (byte_str_ref_t& str_ref)
  {
    ټېل_int ((صحيح) ((uint32_t (str_ref[0]) << 24) | (str_ref[1] << 16) | (str_ref[2] << 8) | (str_ref[3])));
    str_ref.inc (4);
  }

  منطقي ټېل_fixed_from_substr (byte_str_ref_t& str_ref)
  {
    إذا (unlikely (!str_ref.avail (4)))
      أرجع خطأ;
    ټېل_fixed ((int32_t)*(ثابت HBUINT32*)&str_ref[0]);
    str_ref.inc (4);
    أرجع ریښتیا;
  }

  خاص:
  تعريفنوع cff_ډېری_t<ARG, 513> S;
};

/* an operator prefixed by its operands in a byte string */
بنية op_str_t
{
  /* This used to have a hb_ubytes_t. Using a pointer and length
   * in a particular order, saves 8 bytes in this struct and more
   * in our parsed_cs_op_t subclass. */

  ثابت بینښه حرف *ptr = تش_اشاره;

  op_کوډ_t  op = Op_کوډ_Invalid;

  uint8_t اوږدتیا = 0;
};

/* base of OP_SERIALIZER */
بنية op_serializer_t
{
  محمي:
  منطقي copy_opstr (hb_serialize_context_t *c, ثابت op_str_t& opstr) ثابت
  {
    TRACE_SERIALIZE (هذا);

    بینښه حرف *d = c->allocate_اندازه<بینښه حرف> (opstr.اوږدتیا);
    إذا (unlikely (!d)) return_trace (خطأ);
    /* Faster than hb_memcpy for small strings. */
    لكل (بینښه i = 0; i < opstr.اوږدتیا; i++)
      d[i] = opstr.ptr[i];
    return_trace (ریښتیا);
  }
};

کينډۍ <ډول_نوم VAL>
بنية parsed_ارزښتونه_t
{
  فراغ init ()
  {
    op_پیل = 0;
    values.init ();
  }
  فراغ fini () { values.fini (); }

  فراغ alloc (بینښه n)
  {
    values.alloc_exact (n);
  }

  فراغ جمع_کړه_op (op_کوډ_t op, ثابت byte_str_ref_t& str_ref = byte_str_ref_t (), ثابت VAL &v = VAL ())
  {
    VAL *val = values.push (v);
    val->op = op;
    خپلکاری arr = str_ref.sub_array (op_پیل, str_ref.get_دننه_واټن () - op_پیل);
    val->ptr = arr.arrayZ;
    val->اوږدتیا = arr.اوږدتیا;
    op_پیل = str_ref.get_دننه_واټن ();
  }

  منطقي has_op (op_کوډ_t op) ثابت
  {
    لكل (ثابت خپلکاری& v : values)
      إذا (v.op == op) أرجع ریښتیا;
    أرجع خطأ;
  }

  بینښه get_شمېر () ثابت { أرجع values.اوږدتیا; }
  ثابت VAL &چلوونکی [] (بینښه صحيح i) ثابت { أرجع values[i]; }

  بینښه صحيح       op_پیل;
  hb_ویکټور_t<VAL>   values;
};

کينډۍ <ډول_نوم ARG=شمېره_t>
بنية interp_env_t
{
  interp_env_t () {}
  interp_env_t (ثابت hb_ubytes_t &str_)
  {
    str_ref.reset (str_);
  }
  منطقي دننه_error () ثابت
  { أرجع str_ref.دننه_error () || arg_ډېری.دننه_error (); }

  فراغ سېټ_error () { str_ref.سېټ_error (); }

  op_کوډ_t fetch_op ()
  {
    op_کوډ_t  op = Op_کوډ_Invalid;
    إذا (unlikely (!str_ref.avail ()))
      أرجع Op_کوډ_Invalid;
    op = (op_کوډ_t) str_ref.سر_unchecked ();
    str_ref.inc ();
    إذا (op == Op_کوډ_escape) {
      إذا (unlikely (!str_ref.avail ()))
	أرجع Op_کوډ_Invalid;
      op = Make_Op_کوډ_ESC (str_ref.سر_unchecked ());
      str_ref.inc ();
    }
    أرجع op;
  }

  ثابت ARG& eval_arg (بینښه صحيح i) { أرجع arg_ډېری[i]; }

  ARG& راوباسه_arg () { أرجع arg_ډېری.راوباسه (); }
  فراغ راوباسه_n_دلیلونه (بینښه صحيح n) { arg_ډېری.راوباسه (n); }

  فراغ clear_دلیلونه () { راوباسه_n_دلیلونه (arg_ډېری.get_شمېر ()); }

  byte_str_ref_t
		str_ref;
  arg_ډېری_t<ARG>
		arg_ډېری;
};

کارول عدد_interp_env_t =  interp_env_t<>;

کينډۍ <ډول_نوم ARG=شمېره_t>
بنية opset_t
{
  ساكن فراغ process_op (op_کوډ_t op, interp_env_t<ARG>& env)
  {
    بدل (op) {
      حالة Op_کوډ_shortint:
	env.arg_ډېری.ټېل_int ((int16_t)((env.str_ref[0] << 8) | env.str_ref[1]));
	env.str_ref.inc (2);
	اكسر;

      حالة Op_کوډ_Two_Byte_Pos_Int0: حالة Op_کوډ_Two_Byte_Pos_Int1:
      حالة Op_کوډ_Two_Byte_Pos_Int2: حالة Op_کوډ_Two_Byte_Pos_Int3:
	env.arg_ډېری.ټېل_int ((int16_t)((op - Op_کوډ_Two_Byte_Pos_Int0) * 256 + env.str_ref[0] + 108));
	env.str_ref.inc ();
	اكسر;

      حالة Op_کوډ_Two_Byte_Neg_Int0: حالة Op_کوډ_Two_Byte_Neg_Int1:
      حالة Op_کوډ_Two_Byte_Neg_Int2: حالة Op_کوډ_Two_Byte_Neg_Int3:
	env.arg_ډېری.ټېل_int ((-(int16_t)(op - Op_کوډ_Two_Byte_Neg_Int0) * 256 - env.str_ref[0] - 108));
	env.str_ref.inc ();
	اكسر;

      افتراضي:
	/* 1-byte integer */
	إذا (likely ((Op_کوډ_One_Byte_Int_لومړی <= op) && (op <= Op_کوډ_One_Byte_Int_وروستی)))
	{
	  env.arg_ډېری.ټېل_int ((صحيح)op - 139);
	} وإلا {
	  /* invalid unknown operator */
	  env.clear_دلیلونه ();
	}
	اكسر;
    }
  }
};

کينډۍ <ډول_نوم ENV>
بنية interpreter_t
{
  interpreter_t (ENV& env_) : env (env_) {}
  ENV& env;
};

} /* namespace CFF */

#د_که_پای /* HB_CFF_INTERP_COMMON_HH */
