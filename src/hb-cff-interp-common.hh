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
#اگر_تعریف_نشده HB_CFF_INTERP_COMMON_HH
#تعریف_کن HB_CFF_INTERP_COMMON_HH

خارجي HB_داخلی ثابت بیعلامت حرف *endchar_str;

نطاق CFF {

استفاده نطاق OT;

تعريفنوع بیعلامت صحيح op_کد_t;


/* === Dict operators === */

/* One byte operators (0-31) */
#تعریف_کن Op_کد_نسخه		  0 /* CFF Top */
#تعریف_کن Op_کد_Notice		  1 /* CFF Top */
#تعریف_کن Op_کد_پر_Name		  2 /* CFF Top */
#تعریف_کن Op_کد_Family_Name	  3 /* CFF Top */
#تعریف_کن Op_کد_وزن		  4 /* CFF Top */
#تعریف_کن Op_کد_Font_BBox		  5 /* CFF Top */
#تعریف_کن Op_کد_Blue_مقادیر	  6 /* CFF Private, CFF2 Private */
#تعریف_کن Op_کد_Other_Blues	  7 /* CFF Private, CFF2 Private */
#تعریف_کن Op_کد_Family_Blues	  8 /* CFF Private, CFF2 Private */
#تعریف_کن Op_کد_Family_Other_Blues	  9 /* CFF Private, CFF2 Private */
#تعریف_کن Op_کد_Std_HW		 10 /* CFF Private, CFF2 Private */
#تعریف_کن Op_کد_Std_VW		 11 /* CFF Private, CFF2 Private */
#تعریف_کن Op_کد_escape		 12 /* All. Shared with CS */
#تعریف_کن Op_کد_Unique_ID		 13 /* CFF Top */
#تعریف_کن Op_کد_XUID		 14 /* CFF Top */
#تعریف_کن Op_کد_charset		 15 /* CFF Top (0) */
#تعریف_کن Op_کد_Encoding		 16 /* CFF Top (0) */
#تعریف_کن Op_کد_Char_رشتهها	 17 /* CFF Top, CFF2 Top */
#تعریف_کن Op_کد_خصوصی		 18 /* CFF Top, CFF2 FD */
#تعریف_کن Op_کد_Subrs		 19 /* CFF Private, CFF2 Private */
#تعریف_کن Op_کد_default_عرض_X	 20 /* CFF Private (0) */
#تعریف_کن Op_کد_nominal_عرض_X	 21 /* CFF Private (0) */
#تعریف_کن Op_کد_vsindexdict	 22 /* CFF2 Private/CS */
#تعریف_کن Op_کد_blenddict	 23 /* CFF2 Private/CS */
#تعریف_کن Op_کد_vstore		 24 /* CFF2 Top */
#تعریف_کن Op_کد_reserved25	 25
#تعریف_کن Op_کد_reserved26	 26
#تعریف_کن Op_کد_reserved27	 27

/* Numbers */
#تعریف_کن Op_کد_shortint		 28 /* 16-bit integer, All */
#تعریف_کن Op_کد_longintdict	 29 /* 32-bit integer, All */
#تعریف_کن Op_کد_BCD		 30 /* Real number, CFF2 Top/FD */
#تعریف_کن Op_کد_reserved31	 31

/* 1-byte integers */
#تعریف_کن Op_کد_One_Byte_Int_اولی	 32 /* All. beginning of the range of first byte ints */
#تعریف_کن Op_کد_One_Byte_Int_آخرین	246 /* All. ending of the range of first byte int */

/* 2-byte integers */
#تعریف_کن Op_کد_Two_Byte_Pos_Int0	247 /* All. first byte of two byte positive int (+108 to +1131) */
#تعریف_کن Op_کد_Two_Byte_Pos_Int1	248
#تعریف_کن Op_کد_Two_Byte_Pos_Int2	249
#تعریف_کن Op_کد_Two_Byte_Pos_Int3	250

#تعریف_کن Op_کد_Two_Byte_Neg_Int0	251 /* All. first byte of two byte negative int (-1131 to -108) */
#تعریف_کن Op_کد_Two_Byte_Neg_Int1	252
#تعریف_کن Op_کد_Two_Byte_Neg_Int2	253
#تعریف_کن Op_کد_Two_Byte_Neg_Int3	254

/* Two byte escape operators 12, (0-41) */
#تعریف_کن Op_کد_ESC_پایه		256
#تعریف_کن Make_Op_کد_ESC(byte2)	((op_کد_t)(Op_کد_ESC_پایه + (byte2)))

مضمن op_کد_t Unmake_Op_کد_ESC (op_کد_t op)  { أرجع (op_کد_t)(op - Op_کد_ESC_پایه); }
مضمن منطقي Is_Op_کد_ESC (op_کد_t op) { أرجع op >= Op_کد_ESC_پایه; }
مضمن بیعلامت صحيح Op_کد_اندازه (op_کد_t op) { أرجع Is_Op_کد_ESC (op) ? 2: 1; }

#تعریف_کن Op_کد_Copyright	Make_Op_کد_ESC(0) /* CFF Top */
#تعریف_کن Op_کد_is_Fixed_Pitch	Make_Op_کد_ESC(1) /* CFF Top (false) */
#تعریف_کن Op_کد_Italic_Angle	Make_Op_کد_ESC(2) /* CFF Top (0) */
#تعریف_کن Op_کد_Underline_Position Make_Op_کد_ESC(3) /* CFF Top (-100) */
#تعریف_کن Op_کد_Underline_Thickness Make_Op_کد_ESC(4) /* CFF Top (50) */
#تعریف_کن Op_کد_Paint_Type	Make_Op_کد_ESC(5) /* CFF Top (0) */
#تعریف_کن Op_کد_Charstring_Type	Make_Op_کد_ESC(6) /* CFF Top (2) */
#تعریف_کن Op_کد_Font_ماتریس	Make_Op_کد_ESC(7) /* CFF Top, CFF2 Top (.001 0 0 .001 0 0)*/
#تعریف_کن Op_کد_Stroke_عرض	Make_Op_کد_ESC(8) /* CFF Top (0) */
#تعریف_کن Op_کد_Blue_Scale	Make_Op_کد_ESC(9) /* CFF Private, CFF2 Private (0.039625) */
#تعریف_کن Op_کد_Blue_لغزش	Make_Op_کد_ESC(10) /* CFF Private, CFF2 Private (7) */
#تعریف_کن Op_کد_Blue_Fuzz		Make_Op_کد_ESC(11) /* CFF Private, CFF2 Private (1) */
#تعریف_کن Op_کد_Stem_Snap_H	Make_Op_کد_ESC(12) /* CFF Private, CFF2 Private */
#تعریف_کن Op_کد_Stem_Snap_V	Make_Op_کد_ESC(13) /* CFF Private, CFF2 Private */
#تعریف_کن Op_کد_Force_Bold	Make_Op_کد_ESC(14) /* CFF Private (false) */
#تعریف_کن Op_کد_reserved_ESC15	Make_Op_کد_ESC(15)
#تعریف_کن Op_کد_reserved_ESC16	Make_Op_کد_ESC(16)
#تعریف_کن Op_کد_Language_گروه	Make_Op_کد_ESC(17) /* CFF Private, CFF2 Private (0) */
#تعریف_کن Op_کد_Expansion_ضریب	Make_Op_کد_ESC(18) /* CFF Private, CFF2 Private (0.06) */
#تعریف_کن Op_کد_initial_Random_دانه Make_Op_کد_ESC(19) /* CFF Private (0) */
#تعریف_کن Op_کد_Synthetic_پایه	Make_Op_کد_ESC(20) /* CFF Top */
#تعریف_کن Op_کد_Post_Script	Make_Op_کد_ESC(21) /* CFF Top */
#تعریف_کن Op_کد_پایه_Font_Name	Make_Op_کد_ESC(22) /* CFF Top */
#تعریف_کن Op_کد_پایه_Font_Blend	Make_Op_کد_ESC(23) /* CFF Top */
#تعریف_کن Op_کد_reserved_ESC24	Make_Op_کد_ESC(24)
#تعریف_کن Op_کد_reserved_ESC25	Make_Op_کد_ESC(25)
#تعریف_کن Op_کد_reserved_ESC26	Make_Op_کد_ESC(26)
#تعریف_کن Op_کد_reserved_ESC27	Make_Op_کد_ESC(27)
#تعریف_کن Op_کد_reserved_ESC28	Make_Op_کد_ESC(28)
#تعریف_کن Op_کد_reserved_ESC29	Make_Op_کد_ESC(29)
#تعریف_کن Op_کد_ROS		Make_Op_کد_ESC(30) /* CFF Top_CID */
#تعریف_کن Op_کد_CIDFont_نسخه	Make_Op_کد_ESC(31) /* CFF Top_CID (0) */
#تعریف_کن Op_کد_CIDFont_Revision	Make_Op_کد_ESC(32) /* CFF Top_CID (0) */
#تعریف_کن Op_کد_CIDFont_Type	Make_Op_کد_ESC(33) /* CFF Top_CID (0) */
#تعریف_کن Op_کد_CIDCount		Make_Op_کد_ESC(34) /* CFF Top_CID (8720) */
#تعریف_کن Op_کد_UIDBase		Make_Op_کد_ESC(35) /* CFF Top_CID */
#تعریف_کن Op_کد_FDArray		Make_Op_کد_ESC(36) /* CFF Top_CID, CFF2 Top */
#تعریف_کن Op_کد_FDSelect		Make_Op_کد_ESC(37) /* CFF Top_CID, CFF2 Top */
#تعریف_کن Op_کد_Font_Name		Make_Op_کد_ESC(38) /* CFF Top_CID */


/* === CharString operators === */

#تعریف_کن Op_کد_hstem		  1 /* CFF, CFF2 */
#تعریف_کن Op_کد_Reserved2	  2
#تعریف_کن Op_کد_vstem		  3 /* CFF, CFF2 */
#تعریف_کن Op_کد_vmoveto		  4 /* CFF, CFF2 */
#تعریف_کن Op_کد_rlineto		  5 /* CFF, CFF2 */
#تعریف_کن Op_کد_hlineto		  6 /* CFF, CFF2 */
#تعریف_کن Op_کد_vlineto		  7 /* CFF, CFF2 */
#تعریف_کن Op_کد_rrcurveto	  8 /* CFF, CFF2 */
#تعریف_کن Op_کد_Reserved9	  9
#تعریف_کن Op_کد_callsubr		 10 /* CFF, CFF2 */
#تعریف_کن Op_کد_return		 11 /* CFF */
//#define OpCode_escape		 12 /* CFF, CFF2 */
#تعریف_کن Op_کد_Reserved13	 13
#تعریف_کن Op_کد_endchar		 14 /* CFF */
#تعریف_کن Op_کد_vsindexcs	 15 /* CFF2 */
#تعریف_کن Op_کد_blendcs		 16 /* CFF2 */
#تعریف_کن Op_کد_Reserved17	 17
#تعریف_کن Op_کد_hstemhm		 18 /* CFF, CFF2 */
#تعریف_کن Op_کد_hintmask		 19 /* CFF, CFF2 */
#تعریف_کن Op_کد_cntrmask		 20 /* CFF, CFF2 */
#تعریف_کن Op_کد_rmoveto		 21 /* CFF, CFF2 */
#تعریف_کن Op_کد_hmoveto		 22 /* CFF, CFF2 */
#تعریف_کن Op_کد_vstemhm		 23 /* CFF, CFF2 */
#تعریف_کن Op_کد_rcurveline	 24 /* CFF, CFF2 */
#تعریف_کن Op_کد_rlinecurve	 25 /* CFF, CFF2 */
#تعریف_کن Op_کد_vvcurveto	 26 /* CFF, CFF2 */
#تعریف_کن Op_کد_hhcurveto	 27 /* CFF, CFF2 */
//#define OpCode_shortint	 28 /* CFF, CFF2 */
#تعریف_کن Op_کد_callgsubr	 29 /* CFF, CFF2 */
#تعریف_کن Op_کد_vhcurveto	 30 /* CFF, CFF2 */
#تعریف_کن Op_کد_hvcurveto	 31 /* CFF, CFF2 */

#تعریف_کن Op_کد_fixedcs		255 /* 32-bit fixed */

/* Two byte escape operators 12, (0-41) */
#تعریف_کن Op_کد_dotsection	Make_Op_کد_ESC(0) /* CFF (obsoleted) */
#تعریف_کن Op_کد_Reserved_ESC1	Make_Op_کد_ESC(1)
#تعریف_کن Op_کد_Reserved_ESC2	Make_Op_کد_ESC(2)
#تعریف_کن Op_کد_and		Make_Op_کد_ESC(3) /* CFF */
#تعریف_کن Op_کد_or		Make_Op_کد_ESC(4) /* CFF */
#تعریف_کن Op_کد_not		Make_Op_کد_ESC(5) /* CFF */
#تعریف_کن Op_کد_Reserved_ESC6	Make_Op_کد_ESC(6)
#تعریف_کن Op_کد_Reserved_ESC7	Make_Op_کد_ESC(7)
#تعریف_کن Op_کد_Reserved_ESC8	Make_Op_کد_ESC(8)
#تعریف_کن Op_کد_abs		Make_Op_کد_ESC(9) /* CFF */
#تعریف_کن Op_کد_جمع_کن		Make_Op_کد_ESC(10) /* CFF */
#تعریف_کن Op_کد_sub		Make_Op_کد_ESC(11) /* CFF */
#تعریف_کن Op_کد_div		Make_Op_کد_ESC(12) /* CFF */
#تعریف_کن Op_کد_Reserved_ESC13	Make_Op_کد_ESC(13)
#تعریف_کن Op_کد_neg		Make_Op_کد_ESC(14) /* CFF */
#تعریف_کن Op_کد_eq		Make_Op_کد_ESC(15) /* CFF */
#تعریف_کن Op_کد_Reserved_ESC16	Make_Op_کد_ESC(16)
#تعریف_کن Op_کد_Reserved_ESC17	Make_Op_کد_ESC(17)
#تعریف_کن Op_کد_drop		Make_Op_کد_ESC(18) /* CFF */
#تعریف_کن Op_کد_Reserved_ESC19	Make_Op_کد_ESC(19)
#تعریف_کن Op_کد_put		Make_Op_کد_ESC(20) /* CFF */
#تعریف_کن Op_کد_get		Make_Op_کد_ESC(21) /* CFF */
#تعریف_کن Op_کد_ifelse		Make_Op_کد_ESC(22) /* CFF */
#تعریف_کن Op_کد_random		Make_Op_کد_ESC(23) /* CFF */
#تعریف_کن Op_کد_mul		Make_Op_کد_ESC(24) /* CFF */
//#define OpCode_reservedESC25	Make_OpCode_ESC(25)
#تعریف_کن Op_کد_sqrt		Make_Op_کد_ESC(26) /* CFF */
#تعریف_کن Op_کد_dup		Make_Op_کد_ESC(27) /* CFF */
#تعریف_کن Op_کد_exch		Make_Op_کد_ESC(28) /* CFF */
#تعریف_کن Op_کد_شاخص		Make_Op_کد_ESC(29) /* CFF */
#تعریف_کن Op_کد_roll		Make_Op_کد_ESC(30) /* CFF */
#تعریف_کن Op_کد_reserved_ESC31	Make_Op_کد_ESC(31)
#تعریف_کن Op_کد_reserved_ESC32	Make_Op_کد_ESC(32)
#تعریف_کن Op_کد_reserved_ESC33	Make_Op_کد_ESC(33)
#تعریف_کن Op_کد_hflex		Make_Op_کد_ESC(34) /* CFF, CFF2 */
#تعریف_کن Op_کد_flex		Make_Op_کد_ESC(35) /* CFF, CFF2 */
#تعریف_کن Op_کد_hflex1		Make_Op_کد_ESC(36) /* CFF, CFF2 */
#تعریف_کن Op_کد_flex1		Make_Op_کد_ESC(37) /* CFF, CFF2 */


#تعریف_کن Op_کد_Invalid		0xFFFFu


بنية عدد_t
{
  فراغ مجموعه_int (صحيح v)       { value = v; }
  منطقي مقصد_int (صحيح *v) ثابت
  {
    إذا (unlikely (!(value >= INT_MIN && value <= بیشترین_صحیح)))
    {
      *v = 0;
      أرجع خطأ;
    }
    *v = (صحيح) value;
    أرجع درست;
  }
  صحيح مقصد_int () ثابت
  {
    صحيح v;
    إذا (likely (مقصد_int (&v))) أرجع v;
    أرجع value < 0 ? INT_MIN : value > 0 ? بیشترین_صحیح : 0;
  }

  فراغ مجموعه_fixed (int32_t v) { value = v / 65536.0; }
  int32_t مقصد_fixed () ثابت
  { أرجع (int32_t) hb_clamp (value * 65536.0, (مزدوج) INT32_MIN, (مزدوج) INT32_MAX); }

  فراغ مجموعه_real (مزدوج v)   { value = v; }
  مزدوج مقصد_real () ثابت    { أرجع value; }

  منطقي درون_int_range () ثابت
  { أرجع value >= INT16_MIN && value <= INT16_MAX && (مزدوج) (int16_t) value == value; }

  منطقي عملگر >  (ثابت عدد_t &n) ثابت { أرجع value > n.مقصد_real (); }
  منطقي عملگر <  (ثابت عدد_t &n) ثابت { أرجع n > *هذا; }
  منطقي عملگر >= (ثابت عدد_t &n) ثابت { أرجع !(*هذا < n); }
  منطقي عملگر <= (ثابت عدد_t &n) ثابت { أرجع !(*هذا > n); }

  ثابت عدد_t &عملگر += (ثابت عدد_t &n)
  {
    مجموعه_real (مقصد_real () + n.مقصد_real ());

    أرجع *هذا;
  }

  محمي:
  مزدوج value = 0.;
};

/* byte string */
بنية UnsizedByteStr : Unsized_Array_از <HBUINT8>
{
  hb_ubytes_t as_ubytes (بیعلامت l) ثابت
  { أرجع hb_ubytes_t ((ثابت بیعلامت حرف *) هذا, l); }

  // encode 2-byte int (Dict/CharString) or 4-byte int (Dict)
  قالب <نام_نوع T, نام_نوع V>
  ساكن منطقي serialize_int (hb_serialize_context_t *c, op_کد_t intOp, V value)
  {
    TRACE_SERIALIZE (هذا);

    HBUINT8 *p = c->allocate_اندازه<HBUINT8> (1);
    إذا (unlikely (!p)) return_trace (خطأ);
    *p = intOp;

    T *ip = c->allocate_اندازه<T> (T::static_اندازه);
    إذا (unlikely (!ip)) return_trace (خطأ);
    return_trace (c->check_تخصیص_بده (*ip, value, HB_SERIALIZE_ERROR_INT_سرریز));
  }

  قالب <نام_نوع V>
  ساكن منطقي serialize_int4 (hb_serialize_context_t *c, V value)
  { أرجع serialize_int<HBINT32> (c, Op_کد_longintdict, value); }

  قالب <نام_نوع V>
  ساكن منطقي serialize_int2 (hb_serialize_context_t *c, V value)
  { أرجع serialize_int<HBINT16> (c, Op_کد_shortint, value); }

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

  byte_str_ref_t (ثابت hb_ubytes_t &str_, بیعلامت صحيح جابه_جایی_3 = 0)
    : str (str_) { مجموعه_جابه_جایی (جابه_جایی_3); }

  فراغ reset (ثابت hb_ubytes_t &str_, بیعلامت صحيح جابه_جایی_3 = 0)
  {
    str = str_;
    مجموعه_جابه_جایی (جابه_جایی_3);
  }

  ثابت بیعلامت حرف& عملگر [] (صحيح i) {
    إذا (unlikely ((بیعلامت صحيح) (get_جابه_جایی () + i) >= str.درازی))
    {
      مجموعه_error ();
      أرجع Null (بیعلامت حرف);
    }
    أرجع str.arrayZ[get_جابه_جایی () + i];
  }

  بیعلامت حرف سر_unchecked () ثابت { أرجع str.arrayZ[get_جابه_جایی ()]; }

  /* Conversion to hb_ubytes_t */
  عملگر hb_ubytes_t () ثابت { أرجع str.sub_array (get_جابه_جایی ()); }

  hb_ubytes_t sub_array (بیعلامت صحيح جابه_جایی_3, بیعلامت صحيح len_) ثابت
  { أرجع str.sub_array (جابه_جایی_3, len_); }

  منطقي avail (بیعلامت صحيح شمار=1) ثابت
  { أرجع get_جابه_جایی () + شمار <= str.درازی; }
  فراغ inc (بیعلامت صحيح شمار=1)
  {
    /* Automatically puts us in error if count is out-of-range. */
    مجموعه_جابه_جایی (get_جابه_جایی () + شمار);
  }

  /* We (ab)use ubytes backwards_length as a cursor (called offset),
   * as well as to store error condition. */

  بیعلامت get_جابه_جایی () ثابت { أرجع str.backwards_درازی; }
  فراغ مجموعه_جابه_جایی (بیعلامت جابه_جایی) { str.backwards_درازی = جابه_جایی; }

  فراغ مجموعه_error ()      { str.backwards_درازی = str.درازی + 1; }
  منطقي درون_error () ثابت { أرجع str.backwards_درازی > str.درازی; }

  بیعلامت مجموع_اندازه () ثابت { أرجع str.درازی; }

  محمي:
  hb_ubytes_t       str;
};

/*پشته*/
قالب <نام_نوع ELEM, صحيح حد_2>
بنية cff_پشته_t
{
  ELEM& عملگر [] (بیعلامت صحيح i)
  {
    إذا (unlikely (i >= درازی))
    {
      مجموعه_error ();
      أرجع Crap (ELEM);
    }
    أرجع arrayZ[i];
  }

  فراغ push (ثابت ELEM &v)
  {
    إذا (likely (درازی < حد_2))
      arrayZ[درازی++] = v;
    وإلا
      مجموعه_error ();
  }
  ELEM &push ()
  {
    إذا (likely (درازی < حد_2))
      أرجع arrayZ[درازی++];
    وإلا
    {
      مجموعه_error ();
      أرجع Crap (ELEM);
    }
  }

  ELEM& برداشت ()
  {
    إذا (likely (درازی > 0))
      أرجع arrayZ[--درازی];
    وإلا
    {
      مجموعه_error ();
      أرجع Crap (ELEM);
    }
  }
  فراغ برداشت (بیعلامت صحيح n)
  {
    إذا (likely (درازی >= n))
      درازی -= n;
    وإلا
      مجموعه_error ();
  }

  ثابت ELEM& peek ()
  {
    إذا (unlikely (درازی == 0))
    {
      مجموعه_error ();
      أرجع Null (ELEM);
    }
    أرجع arrayZ[درازی - 1];
  }

  فراغ unpop ()
  {
    إذا (likely (درازی < حد_2))
      درازی++;
    وإلا
      مجموعه_error ();
  }

  فراغ clear () { درازی = 0; }

  منطقي درون_error () ثابت { أرجع (error); }
  فراغ مجموعه_error ()      { error = درست; }

  بیعلامت صحيح get_شمار () ثابت { أرجع درازی; }
  منطقي is_empty () ثابت          { أرجع !درازی; }

  hb_array_t<ثابت ELEM> sub_array (بیعلامت شروع, بیعلامت درازی) ثابت
  { أرجع hb_array_t<ثابت ELEM> (arrayZ).sub_array (شروع, درازی); }

  منطقي error = خطأ;
  بیعلامت صحيح درازی = 0;
  ELEM arrayZ[حد_2];
};

/* argument stack */
قالب <نام_نوع ARG=عدد_t>
بنية arg_پشته_t : cff_پشته_t<ARG, 513>
{
  فراغ افزودن_int (صحيح v)
  {
    ARG &n = S::push ();
    n.مجموعه_int (v);
  }

  فراغ افزودن_fixed (int32_t v)
  {
    ARG &n = S::push ();
    n.مجموعه_fixed (v);
  }

  فراغ افزودن_real (مزدوج v)
  {
    ARG &n = S::push ();
    n.مجموعه_real (v);
  }

  ARG& برداشت_رقم () { أرجع هذا->برداشت (); }

  صحيح برداشت_int ()
  {
    صحيح v;
    إذا (unlikely (!هذا->برداشت ().مقصد_int (&v)))
      S::مجموعه_error ();
    أرجع v;
  }

  بیعلامت صحيح برداشت_uint ()
  {
    صحيح i = برداشت_int ();
    إذا (unlikely (i < 0))
    {
      i = 0;
      S::مجموعه_error ();
    }
    أرجع (بیعلامت) i;
  }

  فراغ افزودن_longint_from_substr (byte_str_ref_t& str_ref)
  {
    افزودن_int ((صحيح) ((uint32_t (str_ref[0]) << 24) | (str_ref[1] << 16) | (str_ref[2] << 8) | (str_ref[3])));
    str_ref.inc (4);
  }

  منطقي افزودن_fixed_from_substr (byte_str_ref_t& str_ref)
  {
    إذا (unlikely (!str_ref.avail (4)))
      أرجع خطأ;
    افزودن_fixed ((int32_t)*(ثابت HBUINT32*)&str_ref[0]);
    str_ref.inc (4);
    أرجع درست;
  }

  خاص:
  تعريفنوع cff_پشته_t<ARG, 513> S;
};

/* an operator prefixed by its operands in a byte string */
بنية op_str_t
{
  /* This used to have a hb_ubytes_t. Using a pointer and length
   * in a particular order, saves 8 bytes in this struct and more
   * in our parsed_cs_op_t subclass. */

  ثابت بیعلامت حرف *ptr = اشاره_تهی;

  op_کد_t  op = Op_کد_Invalid;

  uint8_t درازی = 0;
};

/* base of OP_SERIALIZER */
بنية op_serializer_t
{
  محمي:
  منطقي copy_opstr (hb_serialize_context_t *c, ثابت op_str_t& opstr) ثابت
  {
    TRACE_SERIALIZE (هذا);

    بیعلامت حرف *d = c->allocate_اندازه<بیعلامت حرف> (opstr.درازی);
    إذا (unlikely (!d)) return_trace (خطأ);
    /* Faster than hb_memcpy for small strings. */
    لكل (بیعلامت i = 0; i < opstr.درازی; i++)
      d[i] = opstr.ptr[i];
    return_trace (درست);
  }
};

قالب <نام_نوع VAL>
بنية parsed_مقادیر_t
{
  فراغ init ()
  {
    op_شروع = 0;
    values.init ();
  }
  فراغ fini () { values.fini (); }

  فراغ alloc (بیعلامت n)
  {
    values.alloc_exact (n);
  }

  فراغ جمع_کن_op (op_کد_t op, ثابت byte_str_ref_t& str_ref = byte_str_ref_t (), ثابت VAL &v = VAL ())
  {
    VAL *val = values.push (v);
    val->op = op;
    خودکار arr = str_ref.sub_array (op_شروع, str_ref.get_جابه_جایی () - op_شروع);
    val->ptr = arr.arrayZ;
    val->درازی = arr.درازی;
    op_شروع = str_ref.get_جابه_جایی ();
  }

  منطقي has_op (op_کد_t op) ثابت
  {
    لكل (ثابت خودکار& v : values)
      إذا (v.op == op) أرجع درست;
    أرجع خطأ;
  }

  بیعلامت get_شمار () ثابت { أرجع values.درازی; }
  ثابت VAL &عملگر [] (بیعلامت صحيح i) ثابت { أرجع values[i]; }

  بیعلامت صحيح       op_شروع;
  hb_وکتور_t<VAL>   values;
};

قالب <نام_نوع ARG=عدد_t>
بنية interp_env_t
{
  interp_env_t () {}
  interp_env_t (ثابت hb_ubytes_t &str_)
  {
    str_ref.reset (str_);
  }
  منطقي درون_error () ثابت
  { أرجع str_ref.درون_error () || arg_پشته.درون_error (); }

  فراغ مجموعه_error () { str_ref.مجموعه_error (); }

  op_کد_t fetch_op ()
  {
    op_کد_t  op = Op_کد_Invalid;
    إذا (unlikely (!str_ref.avail ()))
      أرجع Op_کد_Invalid;
    op = (op_کد_t) str_ref.سر_unchecked ();
    str_ref.inc ();
    إذا (op == Op_کد_escape) {
      إذا (unlikely (!str_ref.avail ()))
	أرجع Op_کد_Invalid;
      op = Make_Op_کد_ESC (str_ref.سر_unchecked ());
      str_ref.inc ();
    }
    أرجع op;
  }

  ثابت ARG& eval_arg (بیعلامت صحيح i) { أرجع arg_پشته[i]; }

  ARG& برداشت_arg () { أرجع arg_پشته.برداشت (); }
  فراغ برداشت_n_آرگومانها (بیعلامت صحيح n) { arg_پشته.برداشت (n); }

  فراغ clear_آرگومانها () { برداشت_n_آرگومانها (arg_پشته.get_شمار ()); }

  byte_str_ref_t
		str_ref;
  arg_پشته_t<ARG>
		arg_پشته;
};

استفاده رقم_interp_env_t =  interp_env_t<>;

قالب <نام_نوع ARG=عدد_t>
بنية opset_t
{
  ساكن فراغ process_op (op_کد_t op, interp_env_t<ARG>& env)
  {
    بدل (op) {
      حالة Op_کد_shortint:
	env.arg_پشته.افزودن_int ((int16_t)((env.str_ref[0] << 8) | env.str_ref[1]));
	env.str_ref.inc (2);
	اكسر;

      حالة Op_کد_Two_Byte_Pos_Int0: حالة Op_کد_Two_Byte_Pos_Int1:
      حالة Op_کد_Two_Byte_Pos_Int2: حالة Op_کد_Two_Byte_Pos_Int3:
	env.arg_پشته.افزودن_int ((int16_t)((op - Op_کد_Two_Byte_Pos_Int0) * 256 + env.str_ref[0] + 108));
	env.str_ref.inc ();
	اكسر;

      حالة Op_کد_Two_Byte_Neg_Int0: حالة Op_کد_Two_Byte_Neg_Int1:
      حالة Op_کد_Two_Byte_Neg_Int2: حالة Op_کد_Two_Byte_Neg_Int3:
	env.arg_پشته.افزودن_int ((-(int16_t)(op - Op_کد_Two_Byte_Neg_Int0) * 256 - env.str_ref[0] - 108));
	env.str_ref.inc ();
	اكسر;

      افتراضي:
	/* 1-byte integer */
	إذا (likely ((Op_کد_One_Byte_Int_اولی <= op) && (op <= Op_کد_One_Byte_Int_آخرین)))
	{
	  env.arg_پشته.افزودن_int ((صحيح)op - 139);
	} وإلا {
	  /* invalid unknown operator */
	  env.clear_آرگومانها ();
	}
	اكسر;
    }
  }
};

قالب <نام_نوع ENV>
بنية interpreter_t
{
  interpreter_t (ENV& env_) : env (env_) {}
  ENV& env;
};

} /* namespace CFF */

#پایان_اگر /* HB_CFF_INTERP_COMMON_HH */
