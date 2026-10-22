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
#جيڪڏهن_وصف_نه_ٿيل HB_CFF_INTERP_COMMON_HH
#وصف_ڪريو HB_CFF_INTERP_COMMON_HH

خارجي HB_اندروني ثابت بنانشان حرف *endchar_str;

نطاق CFF {

استعمال نطاق OT;

تعريفنوع بنانشان صحيح op_ڪوڊ_t;


/* === Dict operators === */

/* One byte operators (0-31) */
#وصف_ڪريو Op_ڪوڊ_نسخو		  0 /* CFF Top */
#وصف_ڪريو Op_ڪوڊ_Notice		  1 /* CFF Top */
#وصف_ڪريو Op_ڪوڊ_ڀريل_Name		  2 /* CFF Top */
#وصف_ڪريو Op_ڪوڊ_Family_Name	  3 /* CFF Top */
#وصف_ڪريو Op_ڪوڊ_وزن		  4 /* CFF Top */
#وصف_ڪريو Op_ڪوڊ_Font_BBox		  5 /* CFF Top */
#وصف_ڪريو Op_ڪوڊ_Blue_قدرون	  6 /* CFF Private, CFF2 Private */
#وصف_ڪريو Op_ڪوڊ_Other_Blues	  7 /* CFF Private, CFF2 Private */
#وصف_ڪريو Op_ڪوڊ_Family_Blues	  8 /* CFF Private, CFF2 Private */
#وصف_ڪريو Op_ڪوڊ_Family_Other_Blues	  9 /* CFF Private, CFF2 Private */
#وصف_ڪريو Op_ڪوڊ_Std_HW		 10 /* CFF Private, CFF2 Private */
#وصف_ڪريو Op_ڪوڊ_Std_VW		 11 /* CFF Private, CFF2 Private */
#وصف_ڪريو Op_ڪوڊ_escape		 12 /* All. Shared with CS */
#وصف_ڪريو Op_ڪوڊ_Unique_ID		 13 /* CFF Top */
#وصف_ڪريو Op_ڪوڊ_XUID		 14 /* CFF Top */
#وصف_ڪريو Op_ڪوڊ_charset		 15 /* CFF Top (0) */
#وصف_ڪريو Op_ڪوڊ_Encoding		 16 /* CFF Top (0) */
#وصف_ڪريو Op_ڪوڊ_Char_لڙيون	 17 /* CFF Top, CFF2 Top */
#وصف_ڪريو Op_ڪوڊ_ذاتي		 18 /* CFF Top, CFF2 FD */
#وصف_ڪريو Op_ڪوڊ_Subrs		 19 /* CFF Private, CFF2 Private */
#وصف_ڪريو Op_ڪوڊ_default_ويڪر_X	 20 /* CFF Private (0) */
#وصف_ڪريو Op_ڪوڊ_nominal_ويڪر_X	 21 /* CFF Private (0) */
#وصف_ڪريو Op_ڪوڊ_vsindexdict	 22 /* CFF2 Private/CS */
#وصف_ڪريو Op_ڪوڊ_blenddict	 23 /* CFF2 Private/CS */
#وصف_ڪريو Op_ڪوڊ_vstore		 24 /* CFF2 Top */
#وصف_ڪريو Op_ڪوڊ_reserved25	 25
#وصف_ڪريو Op_ڪوڊ_reserved26	 26
#وصف_ڪريو Op_ڪوڊ_reserved27	 27

/* Numbers */
#وصف_ڪريو Op_ڪوڊ_shortint		 28 /* 16-bit integer, All */
#وصف_ڪريو Op_ڪوڊ_longintdict	 29 /* 32-bit integer, All */
#وصف_ڪريو Op_ڪوڊ_BCD		 30 /* Real number, CFF2 Top/FD */
#وصف_ڪريو Op_ڪوڊ_reserved31	 31

/* 1-byte integers */
#وصف_ڪريو Op_ڪوڊ_One_Byte_Int_پهريون	 32 /* All. beginning of the range of first byte ints */
#وصف_ڪريو Op_ڪوڊ_One_Byte_Int_آخري	246 /* All. ending of the range of first byte int */

/* 2-byte integers */
#وصف_ڪريو Op_ڪوڊ_Two_Byte_Pos_Int0	247 /* All. first byte of two byte positive int (+108 to +1131) */
#وصف_ڪريو Op_ڪوڊ_Two_Byte_Pos_Int1	248
#وصف_ڪريو Op_ڪوڊ_Two_Byte_Pos_Int2	249
#وصف_ڪريو Op_ڪوڊ_Two_Byte_Pos_Int3	250

#وصف_ڪريو Op_ڪوڊ_Two_Byte_Neg_Int0	251 /* All. first byte of two byte negative int (-1131 to -108) */
#وصف_ڪريو Op_ڪوڊ_Two_Byte_Neg_Int1	252
#وصف_ڪريو Op_ڪوڊ_Two_Byte_Neg_Int2	253
#وصف_ڪريو Op_ڪوڊ_Two_Byte_Neg_Int3	254

/* Two byte escape operators 12, (0-41) */
#وصف_ڪريو Op_ڪوڊ_ESC_بنياد		256
#وصف_ڪريو Make_Op_ڪوڊ_ESC(byte2)	((op_ڪوڊ_t)(Op_ڪوڊ_ESC_بنياد + (byte2)))

مضمن op_ڪوڊ_t Unmake_Op_ڪوڊ_ESC (op_ڪوڊ_t op)  { أرجع (op_ڪوڊ_t)(op - Op_ڪوڊ_ESC_بنياد); }
مضمن منطقي Is_Op_ڪوڊ_ESC (op_ڪوڊ_t op) { أرجع op >= Op_ڪوڊ_ESC_بنياد; }
مضمن بنانشان صحيح Op_ڪوڊ_سائيز (op_ڪوڊ_t op) { أرجع Is_Op_ڪوڊ_ESC (op) ? 2: 1; }

#وصف_ڪريو Op_ڪوڊ_Copyright	Make_Op_ڪوڊ_ESC(0) /* CFF Top */
#وصف_ڪريو Op_ڪوڊ_is_Fixed_Pitch	Make_Op_ڪوڊ_ESC(1) /* CFF Top (false) */
#وصف_ڪريو Op_ڪوڊ_Italic_Angle	Make_Op_ڪوڊ_ESC(2) /* CFF Top (0) */
#وصف_ڪريو Op_ڪوڊ_Underline_Position Make_Op_ڪوڊ_ESC(3) /* CFF Top (-100) */
#وصف_ڪريو Op_ڪوڊ_Underline_Thickness Make_Op_ڪوڊ_ESC(4) /* CFF Top (50) */
#وصف_ڪريو Op_ڪوڊ_Paint_Type	Make_Op_ڪوڊ_ESC(5) /* CFF Top (0) */
#وصف_ڪريو Op_ڪوڊ_Charstring_Type	Make_Op_ڪوڊ_ESC(6) /* CFF Top (2) */
#وصف_ڪريو Op_ڪوڊ_Font_ميٽرڪس	Make_Op_ڪوڊ_ESC(7) /* CFF Top, CFF2 Top (.001 0 0 .001 0 0)*/
#وصف_ڪريو Op_ڪوڊ_Stroke_ويڪر	Make_Op_ڪوڊ_ESC(8) /* CFF Top (0) */
#وصف_ڪريو Op_ڪوڊ_Blue_Scale	Make_Op_ڪوڊ_ESC(9) /* CFF Private, CFF2 Private (0.039625) */
#وصف_ڪريو Op_ڪوڊ_Blue_سرڪايو	Make_Op_ڪوڊ_ESC(10) /* CFF Private, CFF2 Private (7) */
#وصف_ڪريو Op_ڪوڊ_Blue_Fuzz		Make_Op_ڪوڊ_ESC(11) /* CFF Private, CFF2 Private (1) */
#وصف_ڪريو Op_ڪوڊ_Stem_Snap_H	Make_Op_ڪوڊ_ESC(12) /* CFF Private, CFF2 Private */
#وصف_ڪريو Op_ڪوڊ_Stem_Snap_V	Make_Op_ڪوڊ_ESC(13) /* CFF Private, CFF2 Private */
#وصف_ڪريو Op_ڪوڊ_Force_Bold	Make_Op_ڪوڊ_ESC(14) /* CFF Private (false) */
#وصف_ڪريو Op_ڪوڊ_reserved_ESC15	Make_Op_ڪوڊ_ESC(15)
#وصف_ڪريو Op_ڪوڊ_reserved_ESC16	Make_Op_ڪوڊ_ESC(16)
#وصف_ڪريو Op_ڪوڊ_Language_گروهه	Make_Op_ڪوڊ_ESC(17) /* CFF Private, CFF2 Private (0) */
#وصف_ڪريو Op_ڪوڊ_Expansion_ضرب_ڪندڙ	Make_Op_ڪوڊ_ESC(18) /* CFF Private, CFF2 Private (0.06) */
#وصف_ڪريو Op_ڪوڊ_initial_Random_ٻج Make_Op_ڪوڊ_ESC(19) /* CFF Private (0) */
#وصف_ڪريو Op_ڪوڊ_Synthetic_بنياد	Make_Op_ڪوڊ_ESC(20) /* CFF Top */
#وصف_ڪريو Op_ڪوڊ_Post_Script	Make_Op_ڪوڊ_ESC(21) /* CFF Top */
#وصف_ڪريو Op_ڪوڊ_بنياد_Font_Name	Make_Op_ڪوڊ_ESC(22) /* CFF Top */
#وصف_ڪريو Op_ڪوڊ_بنياد_Font_Blend	Make_Op_ڪوڊ_ESC(23) /* CFF Top */
#وصف_ڪريو Op_ڪوڊ_reserved_ESC24	Make_Op_ڪوڊ_ESC(24)
#وصف_ڪريو Op_ڪوڊ_reserved_ESC25	Make_Op_ڪوڊ_ESC(25)
#وصف_ڪريو Op_ڪوڊ_reserved_ESC26	Make_Op_ڪوڊ_ESC(26)
#وصف_ڪريو Op_ڪوڊ_reserved_ESC27	Make_Op_ڪوڊ_ESC(27)
#وصف_ڪريو Op_ڪوڊ_reserved_ESC28	Make_Op_ڪوڊ_ESC(28)
#وصف_ڪريو Op_ڪوڊ_reserved_ESC29	Make_Op_ڪوڊ_ESC(29)
#وصف_ڪريو Op_ڪوڊ_ROS		Make_Op_ڪوڊ_ESC(30) /* CFF Top_CID */
#وصف_ڪريو Op_ڪوڊ_CIDFont_نسخو	Make_Op_ڪوڊ_ESC(31) /* CFF Top_CID (0) */
#وصف_ڪريو Op_ڪوڊ_CIDFont_Revision	Make_Op_ڪوڊ_ESC(32) /* CFF Top_CID (0) */
#وصف_ڪريو Op_ڪوڊ_CIDFont_Type	Make_Op_ڪوڊ_ESC(33) /* CFF Top_CID (0) */
#وصف_ڪريو Op_ڪوڊ_CIDCount		Make_Op_ڪوڊ_ESC(34) /* CFF Top_CID (8720) */
#وصف_ڪريو Op_ڪوڊ_UIDBase		Make_Op_ڪوڊ_ESC(35) /* CFF Top_CID */
#وصف_ڪريو Op_ڪوڊ_FDArray		Make_Op_ڪوڊ_ESC(36) /* CFF Top_CID, CFF2 Top */
#وصف_ڪريو Op_ڪوڊ_FDSelect		Make_Op_ڪوڊ_ESC(37) /* CFF Top_CID, CFF2 Top */
#وصف_ڪريو Op_ڪوڊ_Font_Name		Make_Op_ڪوڊ_ESC(38) /* CFF Top_CID */


/* === CharString operators === */

#وصف_ڪريو Op_ڪوڊ_hstem		  1 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_Reserved2	  2
#وصف_ڪريو Op_ڪوڊ_vstem		  3 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_vmoveto		  4 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_rlineto		  5 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_hlineto		  6 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_vlineto		  7 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_rrcurveto	  8 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_Reserved9	  9
#وصف_ڪريو Op_ڪوڊ_callsubr		 10 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_return		 11 /* CFF */
//#define OpCode_escape		 12 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_Reserved13	 13
#وصف_ڪريو Op_ڪوڊ_endchar		 14 /* CFF */
#وصف_ڪريو Op_ڪوڊ_vsindexcs	 15 /* CFF2 */
#وصف_ڪريو Op_ڪوڊ_blendcs		 16 /* CFF2 */
#وصف_ڪريو Op_ڪوڊ_Reserved17	 17
#وصف_ڪريو Op_ڪوڊ_hstemhm		 18 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_hintmask		 19 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_cntrmask		 20 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_rmoveto		 21 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_hmoveto		 22 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_vstemhm		 23 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_rcurveline	 24 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_rlinecurve	 25 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_vvcurveto	 26 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_hhcurveto	 27 /* CFF, CFF2 */
//#define OpCode_shortint	 28 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_callgsubr	 29 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_vhcurveto	 30 /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_hvcurveto	 31 /* CFF, CFF2 */

#وصف_ڪريو Op_ڪوڊ_fixedcs		255 /* 32-bit fixed */

/* Two byte escape operators 12, (0-41) */
#وصف_ڪريو Op_ڪوڊ_dotsection	Make_Op_ڪوڊ_ESC(0) /* CFF (obsoleted) */
#وصف_ڪريو Op_ڪوڊ_Reserved_ESC1	Make_Op_ڪوڊ_ESC(1)
#وصف_ڪريو Op_ڪوڊ_Reserved_ESC2	Make_Op_ڪوڊ_ESC(2)
#وصف_ڪريو Op_ڪوڊ_and		Make_Op_ڪوڊ_ESC(3) /* CFF */
#وصف_ڪريو Op_ڪوڊ_or		Make_Op_ڪوڊ_ESC(4) /* CFF */
#وصف_ڪريو Op_ڪوڊ_not		Make_Op_ڪوڊ_ESC(5) /* CFF */
#وصف_ڪريو Op_ڪوڊ_Reserved_ESC6	Make_Op_ڪوڊ_ESC(6)
#وصف_ڪريو Op_ڪوڊ_Reserved_ESC7	Make_Op_ڪوڊ_ESC(7)
#وصف_ڪريو Op_ڪوڊ_Reserved_ESC8	Make_Op_ڪوڊ_ESC(8)
#وصف_ڪريو Op_ڪوڊ_abs		Make_Op_ڪوڊ_ESC(9) /* CFF */
#وصف_ڪريو Op_ڪوڊ_جوڙيو		Make_Op_ڪوڊ_ESC(10) /* CFF */
#وصف_ڪريو Op_ڪوڊ_sub		Make_Op_ڪوڊ_ESC(11) /* CFF */
#وصف_ڪريو Op_ڪوڊ_div		Make_Op_ڪوڊ_ESC(12) /* CFF */
#وصف_ڪريو Op_ڪوڊ_Reserved_ESC13	Make_Op_ڪوڊ_ESC(13)
#وصف_ڪريو Op_ڪوڊ_neg		Make_Op_ڪوڊ_ESC(14) /* CFF */
#وصف_ڪريو Op_ڪوڊ_eq		Make_Op_ڪوڊ_ESC(15) /* CFF */
#وصف_ڪريو Op_ڪوڊ_Reserved_ESC16	Make_Op_ڪوڊ_ESC(16)
#وصف_ڪريو Op_ڪوڊ_Reserved_ESC17	Make_Op_ڪوڊ_ESC(17)
#وصف_ڪريو Op_ڪوڊ_drop		Make_Op_ڪوڊ_ESC(18) /* CFF */
#وصف_ڪريو Op_ڪوڊ_Reserved_ESC19	Make_Op_ڪوڊ_ESC(19)
#وصف_ڪريو Op_ڪوڊ_put		Make_Op_ڪوڊ_ESC(20) /* CFF */
#وصف_ڪريو Op_ڪوڊ_get		Make_Op_ڪوڊ_ESC(21) /* CFF */
#وصف_ڪريو Op_ڪوڊ_ifelse		Make_Op_ڪوڊ_ESC(22) /* CFF */
#وصف_ڪريو Op_ڪوڊ_random		Make_Op_ڪوڊ_ESC(23) /* CFF */
#وصف_ڪريو Op_ڪوڊ_mul		Make_Op_ڪوڊ_ESC(24) /* CFF */
//#define OpCode_reservedESC25	Make_OpCode_ESC(25)
#وصف_ڪريو Op_ڪوڊ_sqrt		Make_Op_ڪوڊ_ESC(26) /* CFF */
#وصف_ڪريو Op_ڪوڊ_dup		Make_Op_ڪوڊ_ESC(27) /* CFF */
#وصف_ڪريو Op_ڪوڊ_exch		Make_Op_ڪوڊ_ESC(28) /* CFF */
#وصف_ڪريو Op_ڪوڊ_اشاريو		Make_Op_ڪوڊ_ESC(29) /* CFF */
#وصف_ڪريو Op_ڪوڊ_roll		Make_Op_ڪوڊ_ESC(30) /* CFF */
#وصف_ڪريو Op_ڪوڊ_reserved_ESC31	Make_Op_ڪوڊ_ESC(31)
#وصف_ڪريو Op_ڪوڊ_reserved_ESC32	Make_Op_ڪوڊ_ESC(32)
#وصف_ڪريو Op_ڪوڊ_reserved_ESC33	Make_Op_ڪوڊ_ESC(33)
#وصف_ڪريو Op_ڪوڊ_hflex		Make_Op_ڪوڊ_ESC(34) /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_flex		Make_Op_ڪوڊ_ESC(35) /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_hflex1		Make_Op_ڪوڊ_ESC(36) /* CFF, CFF2 */
#وصف_ڪريو Op_ڪوڊ_flex1		Make_Op_ڪوڊ_ESC(37) /* CFF, CFF2 */


#وصف_ڪريو Op_ڪوڊ_Invalid		0xFFFFu


بنية عدد_t
{
  فراغ سيٽ_int (صحيح v)       { value = v; }
  منطقي منزل_int (صحيح *v) ثابت
  {
    إذا (unlikely (!(value >= INT_MIN && value <= وڌ_کان_وڌ_عدد)))
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
    أرجع value < 0 ? INT_MIN : value > 0 ? وڌ_کان_وڌ_عدد : 0;
  }

  فراغ سيٽ_fixed (int32_t v) { value = v / 65536.0; }
  int32_t منزل_fixed () ثابت
  { أرجع (int32_t) hb_clamp (value * 65536.0, (مزدوج) INT32_MIN, (مزدوج) INT32_MAX); }

  فراغ سيٽ_real (مزدوج v)   { value = v; }
  مزدوج منزل_real () ثابت    { أرجع value; }

  منطقي اندريون_int_range () ثابت
  { أرجع value >= INT16_MIN && value <= INT16_MAX && (مزدوج) (int16_t) value == value; }

  منطقي عامل >  (ثابت عدد_t &n) ثابت { أرجع value > n.منزل_real (); }
  منطقي عامل <  (ثابت عدد_t &n) ثابت { أرجع n > *هذا; }
  منطقي عامل >= (ثابت عدد_t &n) ثابت { أرجع !(*هذا < n); }
  منطقي عامل <= (ثابت عدد_t &n) ثابت { أرجع !(*هذا > n); }

  ثابت عدد_t &عامل += (ثابت عدد_t &n)
  {
    سيٽ_real (منزل_real () + n.منزل_real ());

    أرجع *هذا;
  }

  محمي:
  مزدوج value = 0.;
};

/* byte string */
بنية UnsizedByteStr : Unsized_Array_جو <HBUINT8>
{
  hb_ubytes_t as_ubytes (بنانشان l) ثابت
  { أرجع hb_ubytes_t ((ثابت بنانشان حرف *) هذا, l); }

  // encode 2-byte int (Dict/CharString) or 4-byte int (Dict)
  سانچو <قسم_نالو T, قسم_نالو V>
  ساكن منطقي serialize_int (hb_serialize_context_t *c, op_ڪوڊ_t intOp, V value)
  {
    TRACE_SERIALIZE (هذا);

    HBUINT8 *p = c->allocate_سائيز<HBUINT8> (1);
    إذا (unlikely (!p)) return_trace (خطأ);
    *p = intOp;

    T *ip = c->allocate_سائيز<T> (T::static_سائيز);
    إذا (unlikely (!ip)) return_trace (خطأ);
    return_trace (c->check_سونپيو (*ip, value, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  سانچو <قسم_نالو V>
  ساكن منطقي serialize_int4 (hb_serialize_context_t *c, V value)
  { أرجع serialize_int<HBINT32> (c, Op_ڪوڊ_longintdict, value); }

  سانچو <قسم_نالو V>
  ساكن منطقي serialize_int2 (hb_serialize_context_t *c, V value)
  { أرجع serialize_int<HBINT16> (c, Op_ڪوڊ_shortint, value); }

  /* Defining null_size allows a Null object may be created. Should be safe because:
   * A descendent struct Dict uses a Null pointer to indicate a missing table,
   * checked before access.
   */
  DEFINE_سائيز_MIN(0);
};

/* A byte string associated with the current offset and an error condition */
بنية byte_str_ref_t
{
  byte_str_ref_t ()
    : str () {}

  byte_str_ref_t (ثابت hb_ubytes_t &str_, بنانشان صحيح وٿي_3 = 0)
    : str (str_) { سيٽ_وٿي (وٿي_3); }

  فراغ reset (ثابت hb_ubytes_t &str_, بنانشان صحيح وٿي_3 = 0)
  {
    str = str_;
    سيٽ_وٿي (وٿي_3);
  }

  ثابت بنانشان حرف& عامل [] (صحيح i) {
    إذا (unlikely ((بنانشان صحيح) (get_وٿي () + i) >= str.ڊيگهائي))
    {
      سيٽ_error ();
      أرجع Null (بنانشان حرف);
    }
    أرجع str.arrayZ[get_وٿي () + i];
  }

  بنانشان حرف سر_unchecked () ثابت { أرجع str.arrayZ[get_وٿي ()]; }

  /* Conversion to hb_ubytes_t */
  عامل hb_ubytes_t () ثابت { أرجع str.sub_array (get_وٿي ()); }

  hb_ubytes_t sub_array (بنانشان صحيح وٿي_3, بنانشان صحيح len_) ثابت
  { أرجع str.sub_array (وٿي_3, len_); }

  منطقي avail (بنانشان صحيح ڳڻپ=1) ثابت
  { أرجع get_وٿي () + ڳڻپ <= str.ڊيگهائي; }
  فراغ inc (بنانشان صحيح ڳڻپ=1)
  {
    /* Automatically puts us in error if count is out-of-range. */
    سيٽ_وٿي (get_وٿي () + ڳڻپ);
  }

  /* We (ab)use ubytes backwards_length as a cursor (called offset),
   * as well as to store error condition. */

  بنانشان get_وٿي () ثابت { أرجع str.backwards_ڊيگهائي; }
  فراغ سيٽ_وٿي (بنانشان وٿي) { str.backwards_ڊيگهائي = وٿي; }

  فراغ سيٽ_error ()      { str.backwards_ڊيگهائي = str.ڊيگهائي + 1; }
  منطقي اندريون_error () ثابت { أرجع str.backwards_ڊيگهائي > str.ڊيگهائي; }

  بنانشان ڪل_سائيز () ثابت { أرجع str.ڊيگهائي; }

  محمي:
  hb_ubytes_t       str;
};

/*ڍير*/
سانچو <قسم_نالو ELEM, صحيح حد_بندي_2>
بنية cff_ڍير_t
{
  ELEM& عامل [] (بنانشان صحيح i)
  {
    إذا (unlikely (i >= ڊيگهائي))
    {
      سيٽ_error ();
      أرجع Crap (ELEM);
    }
    أرجع arrayZ[i];
  }

  فراغ push (ثابت ELEM &v)
  {
    إذا (likely (ڊيگهائي < حد_بندي_2))
      arrayZ[ڊيگهائي++] = v;
    وإلا
      سيٽ_error ();
  }
  ELEM &push ()
  {
    إذا (likely (ڊيگهائي < حد_بندي_2))
      أرجع arrayZ[ڊيگهائي++];
    وإلا
    {
      سيٽ_error ();
      أرجع Crap (ELEM);
    }
  }

  ELEM& ڪڍو ()
  {
    إذا (likely (ڊيگهائي > 0))
      أرجع arrayZ[--ڊيگهائي];
    وإلا
    {
      سيٽ_error ();
      أرجع Crap (ELEM);
    }
  }
  فراغ ڪڍو (بنانشان صحيح n)
  {
    إذا (likely (ڊيگهائي >= n))
      ڊيگهائي -= n;
    وإلا
      سيٽ_error ();
  }

  ثابت ELEM& peek ()
  {
    إذا (unlikely (ڊيگهائي == 0))
    {
      سيٽ_error ();
      أرجع Null (ELEM);
    }
    أرجع arrayZ[ڊيگهائي - 1];
  }

  فراغ unpop ()
  {
    إذا (likely (ڊيگهائي < حد_بندي_2))
      ڊيگهائي++;
    وإلا
      سيٽ_error ();
  }

  فراغ clear () { ڊيگهائي = 0; }

  منطقي اندريون_error () ثابت { أرجع (error); }
  فراغ سيٽ_error ()      { error = سچ; }

  بنانشان صحيح get_ڳڻپ () ثابت { أرجع ڊيگهائي; }
  منطقي is_empty () ثابت          { أرجع !ڊيگهائي; }

  hb_array_t<ثابت ELEM> sub_array (بنانشان شروعات, بنانشان ڊيگهائي) ثابت
  { أرجع hb_array_t<ثابت ELEM> (arrayZ).sub_array (شروعات, ڊيگهائي); }

  منطقي error = خطأ;
  بنانشان صحيح ڊيگهائي = 0;
  ELEM arrayZ[حد_بندي_2];
};

/* argument stack */
سانچو <قسم_نالو ARG=عدد_t>
بنية arg_ڍير_t : cff_ڍير_t<ARG, 513>
{
  فراغ ڌڪو_int (صحيح v)
  {
    ARG &n = S::push ();
    n.سيٽ_int (v);
  }

  فراغ ڌڪو_fixed (int32_t v)
  {
    ARG &n = S::push ();
    n.سيٽ_fixed (v);
  }

  فراغ ڌڪو_real (مزدوج v)
  {
    ARG &n = S::push ();
    n.سيٽ_real (v);
  }

  ARG& ڪڍو_انگ () { أرجع هذا->ڪڍو (); }

  صحيح ڪڍو_int ()
  {
    صحيح v;
    إذا (unlikely (!هذا->ڪڍو ().منزل_int (&v)))
      S::سيٽ_error ();
    أرجع v;
  }

  بنانشان صحيح ڪڍو_uint ()
  {
    صحيح i = ڪڍو_int ();
    إذا (unlikely (i < 0))
    {
      i = 0;
      S::سيٽ_error ();
    }
    أرجع (بنانشان) i;
  }

  فراغ ڌڪو_longint_from_substr (byte_str_ref_t& str_ref)
  {
    ڌڪو_int ((صحيح) ((uint32_t (str_ref[0]) << 24) | (str_ref[1] << 16) | (str_ref[2] << 8) | (str_ref[3])));
    str_ref.inc (4);
  }

  منطقي ڌڪو_fixed_from_substr (byte_str_ref_t& str_ref)
  {
    إذا (unlikely (!str_ref.avail (4)))
      أرجع خطأ;
    ڌڪو_fixed ((int32_t)*(ثابت HBUINT32*)&str_ref[0]);
    str_ref.inc (4);
    أرجع سچ;
  }

  خاص:
  تعريفنوع cff_ڍير_t<ARG, 513> S;
};

/* an operator prefixed by its operands in a byte string */
بنية op_str_t
{
  /* This used to have a hb_ubytes_t. Using a pointer and length
   * in a particular order, saves 8 bytes in this struct and more
   * in our parsed_cs_op_t subclass. */

  ثابت بنانشان حرف *ptr = خالي_اشارو;

  op_ڪوڊ_t  op = Op_ڪوڊ_Invalid;

  uint8_t ڊيگهائي = 0;
};

/* base of OP_SERIALIZER */
بنية op_serializer_t
{
  محمي:
  منطقي copy_opstr (hb_serialize_context_t *c, ثابت op_str_t& opstr) ثابت
  {
    TRACE_SERIALIZE (هذا);

    بنانشان حرف *d = c->allocate_سائيز<بنانشان حرف> (opstr.ڊيگهائي);
    إذا (unlikely (!d)) return_trace (خطأ);
    /* Faster than hb_memcpy for small strings. */
    لكل (بنانشان i = 0; i < opstr.ڊيگهائي; i++)
      d[i] = opstr.ptr[i];
    return_trace (سچ);
  }
};

سانچو <قسم_نالو VAL>
بنية parsed_قدرون_t
{
  فراغ init ()
  {
    op_شروعات = 0;
    values.init ();
  }
  فراغ fini () { values.fini (); }

  فراغ alloc (بنانشان n)
  {
    values.alloc_exact (n);
  }

  فراغ جوڙيو_op (op_ڪوڊ_t op, ثابت byte_str_ref_t& str_ref = byte_str_ref_t (), ثابت VAL &v = VAL ())
  {
    VAL *val = values.push (v);
    val->op = op;
    خودڪار arr = str_ref.sub_array (op_شروعات, str_ref.get_وٿي () - op_شروعات);
    val->ptr = arr.arrayZ;
    val->ڊيگهائي = arr.ڊيگهائي;
    op_شروعات = str_ref.get_وٿي ();
  }

  منطقي has_op (op_ڪوڊ_t op) ثابت
  {
    لكل (ثابت خودڪار& v : values)
      إذا (v.op == op) أرجع سچ;
    أرجع خطأ;
  }

  بنانشان get_ڳڻپ () ثابت { أرجع values.ڊيگهائي; }
  ثابت VAL &عامل [] (بنانشان صحيح i) ثابت { أرجع values[i]; }

  بنانشان صحيح       op_شروعات;
  hb_ويڪٽر_t<VAL>   values;
};

سانچو <قسم_نالو ARG=عدد_t>
بنية interp_env_t
{
  interp_env_t () {}
  interp_env_t (ثابت hb_ubytes_t &str_)
  {
    str_ref.reset (str_);
  }
  منطقي اندريون_error () ثابت
  { أرجع str_ref.اندريون_error () || arg_ڍير.اندريون_error (); }

  فراغ سيٽ_error () { str_ref.سيٽ_error (); }

  op_ڪوڊ_t fetch_op ()
  {
    op_ڪوڊ_t  op = Op_ڪوڊ_Invalid;
    إذا (unlikely (!str_ref.avail ()))
      أرجع Op_ڪوڊ_Invalid;
    op = (op_ڪوڊ_t) str_ref.سر_unchecked ();
    str_ref.inc ();
    إذا (op == Op_ڪوڊ_escape) {
      إذا (unlikely (!str_ref.avail ()))
	أرجع Op_ڪوڊ_Invalid;
      op = Make_Op_ڪوڊ_ESC (str_ref.سر_unchecked ());
      str_ref.inc ();
    }
    أرجع op;
  }

  ثابت ARG& eval_arg (بنانشان صحيح i) { أرجع arg_ڍير[i]; }

  ARG& ڪڍو_arg () { أرجع arg_ڍير.ڪڍو (); }
  فراغ ڪڍو_n_دليلون (بنانشان صحيح n) { arg_ڍير.ڪڍو (n); }

  فراغ clear_دليلون () { ڪڍو_n_دليلون (arg_ڍير.get_ڳڻپ ()); }

  byte_str_ref_t
		str_ref;
  arg_ڍير_t<ARG>
		arg_ڍير;
};

استعمال انگ_interp_env_t =  interp_env_t<>;

سانچو <قسم_نالو ARG=عدد_t>
بنية opset_t
{
  ساكن فراغ process_op (op_ڪوڊ_t op, interp_env_t<ARG>& env)
  {
    بدل (op) {
      حالة Op_ڪوڊ_shortint:
	env.arg_ڍير.ڌڪو_int ((int16_t)((env.str_ref[0] << 8) | env.str_ref[1]));
	env.str_ref.inc (2);
	اكسر;

      حالة Op_ڪوڊ_Two_Byte_Pos_Int0: حالة Op_ڪوڊ_Two_Byte_Pos_Int1:
      حالة Op_ڪوڊ_Two_Byte_Pos_Int2: حالة Op_ڪوڊ_Two_Byte_Pos_Int3:
	env.arg_ڍير.ڌڪو_int ((int16_t)((op - Op_ڪوڊ_Two_Byte_Pos_Int0) * 256 + env.str_ref[0] + 108));
	env.str_ref.inc ();
	اكسر;

      حالة Op_ڪوڊ_Two_Byte_Neg_Int0: حالة Op_ڪوڊ_Two_Byte_Neg_Int1:
      حالة Op_ڪوڊ_Two_Byte_Neg_Int2: حالة Op_ڪوڊ_Two_Byte_Neg_Int3:
	env.arg_ڍير.ڌڪو_int ((-(int16_t)(op - Op_ڪوڊ_Two_Byte_Neg_Int0) * 256 - env.str_ref[0] - 108));
	env.str_ref.inc ();
	اكسر;

      افتراضي:
	/* 1-byte integer */
	إذا (likely ((Op_ڪوڊ_One_Byte_Int_پهريون <= op) && (op <= Op_ڪوڊ_One_Byte_Int_آخري)))
	{
	  env.arg_ڍير.ڌڪو_int ((صحيح)op - 139);
	} وإلا {
	  /* invalid unknown operator */
	  env.clear_دليلون ();
	}
	اكسر;
    }
  }
};

سانچو <قسم_نالو ENV>
بنية interpreter_t
{
  interpreter_t (ENV& env_) : env (env_) {}
  ENV& env;
};

} /* namespace CFF */

#جيڪڏهن_پورو /* HB_CFF_INTERP_COMMON_HH */
