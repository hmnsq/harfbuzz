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

बाह्य HB_INTERNAL अचर अचिन्हित वर्ण *endchar_str;

नामाकाश CFF {

using नामाकाश OT;

प्रकारनिर्देश अचिन्हित पूर्णाङ्क op_code_t;


/* === Dict operators === */

/* One byte operators (0-31) */
#define OpCode_version		  0 /* CFF Top */
#define OpCode_Notice		  1 /* CFF Top */
#define OpCode_FullName		  2 /* CFF Top */
#define OpCode_FamilyName	  3 /* CFF Top */
#define OpCode_Weight		  4 /* CFF Top */
#define OpCode_FontBBox		  5 /* CFF Top */
#define Op_Code_Blue_मानहरू	  6 /* CFF Private, CFF2 Private */
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

अन्तःस्थ op_code_t Unmake_OpCode_ESC (op_code_t op)  { फर्काऊ (op_code_t)(op - OpCode_ESC_Base); }
अन्तःस्थ तार्किक Is_OpCode_ESC (op_code_t op) { फर्काऊ op >= OpCode_ESC_Base; }
अन्तःस्थ अचिन्हित पूर्णाङ्क OpCode_Size (op_code_t op) { फर्काऊ Is_OpCode_ESC (op) ? 2: 1; }

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
#define OpCode_index		Make_OpCode_ESC(29) /* CFF */
#define OpCode_roll		Make_OpCode_ESC(30) /* CFF */
#define OpCode_reservedESC31	Make_OpCode_ESC(31)
#define OpCode_reservedESC32	Make_OpCode_ESC(32)
#define OpCode_reservedESC33	Make_OpCode_ESC(33)
#define OpCode_hflex		Make_OpCode_ESC(34) /* CFF, CFF2 */
#define OpCode_flex		Make_OpCode_ESC(35) /* CFF, CFF2 */
#define OpCode_hflex1		Make_OpCode_ESC(36) /* CFF, CFF2 */
#define OpCode_flex1		Make_OpCode_ESC(37) /* CFF, CFF2 */


#define OpCode_Invalid		0xFFFFu


संरचना सङ्ख्या_t
{
  शून्य set_int (पूर्णाङ्क v)       { value = v; }
  तार्किक to_int (पूर्णाङ्क *v) अचर
  {
    यदि (unlikely (!(value >= पूर्णांक_न्यून && value <= पूर्णांक_उच्च)))
    {
      *v = 0;
      फर्काऊ असत्य;
    }
    *v = (पूर्णाङ्क) value;
    फर्काऊ सत्य;
  }
  पूर्णाङ्क to_int () अचर
  {
    पूर्णाङ्क v;
    यदि (likely (to_int (&v))) फर्काऊ v;
    फर्काऊ value < 0 ? पूर्णांक_न्यून : value > 0 ? पूर्णांक_उच्च : 0;
  }

  शून्य set_fixed (int32_t v) { value = v / 65536.0; }
  int32_t to_fixed () अचर
  { फर्काऊ (int32_t) hb_clamp (value * 65536.0, (द्विगुण) INT32_MIN, (द्विगुण) INT32_MAX); }

  शून्य set_real (द्विगुण v)   { value = v; }
  द्विगुण to_real () अचर    { फर्काऊ value; }

  तार्किक in_int_range () अचर
  { फर्काऊ value >= INT16_MIN && value <= INT16_MAX && (द्विगुण) (int16_t) value == value; }

  तार्किक चालक >  (अचर सङ्ख्या_t &n) अचर { फर्काऊ value > n.to_real (); }
  तार्किक चालक <  (अचर सङ्ख्या_t &n) अचर { फर्काऊ n > *एतत्; }
  तार्किक चालक >= (अचर सङ्ख्या_t &n) अचर { फर्काऊ !(*एतत् < n); }
  तार्किक चालक <= (अचर सङ्ख्या_t &n) अचर { फर्काऊ !(*एतत् > n); }

  अचर सङ्ख्या_t &चालक += (अचर सङ्ख्या_t &n)
  {
    set_real (to_real () + n.to_real ());

    फर्काऊ *एतत्;
  }

  रक्षित:
  द्विगुण value = 0.;
};

/* byte string */
संरचना UnsizedByteStr : UnsizedArrayOf <HBUINT8>
{
  hb_ubytes_t as_ubytes (अचिन्हित l) अचर
  { फर्काऊ hb_ubytes_t ((अचर अचिन्हित वर्ण *) एतत्, l); }

  // encode 2-byte int (Dict/CharString) or 4-byte int (Dict)
  ढाँचा <typename T, typename V>
  स्थिर तार्किक serialize_int (hb_serialize_context_t *c, op_code_t intOp, V value)
  {
    TRACE_SERIALIZE (एतत्);

    HBUINT8 *p = c->allocate_size<HBUINT8> (1);
    यदि (unlikely (!p)) return_trace (असत्य);
    *p = intOp;

    T *ip = c->allocate_size<T> (T::static_size);
    यदि (unlikely (!ip)) return_trace (असत्य);
    return_trace (c->check_assign (*ip, value, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ढाँचा <typename V>
  स्थिर तार्किक serialize_int4 (hb_serialize_context_t *c, V value)
  { फर्काऊ serialize_int<HBINT32> (c, OpCode_longintdict, value); }

  ढाँचा <typename V>
  स्थिर तार्किक serialize_int2 (hb_serialize_context_t *c, V value)
  { फर्काऊ serialize_int<HBINT16> (c, OpCode_shortint, value); }

  /* Defining null_size allows a Null object may be created. Should be safe because:
   * A descendent struct Dict uses a Null pointer to indicate a missing table,
   * checked before access.
   */
  DEFINE_SIZE_MIN(0);
};

/* A byte string associated with the current offset and an error condition */
संरचना byte_str_ref_t
{
  byte_str_ref_t ()
    : str () {}

  byte_str_ref_t (अचर hb_ubytes_t &str_, अचिन्हित पूर्णाङ्क offset_ = 0)
    : str (str_) { set_offset (offset_); }

  शून्य reset (अचर hb_ubytes_t &str_, अचिन्हित पूर्णाङ्क offset_ = 0)
  {
    str = str_;
    set_offset (offset_);
  }

  अचर अचिन्हित वर्ण& चालक [] (पूर्णाङ्क i) {
    यदि (unlikely ((अचिन्हित पूर्णाङ्क) (get_offset () + i) >= str.length))
    {
      set_error ();
      फर्काऊ Null (अचिन्हित वर्ण);
    }
    फर्काऊ str.arrayZ[get_offset () + i];
  }

  अचिन्हित वर्ण head_unchecked () अचर { फर्काऊ str.arrayZ[get_offset ()]; }

  /* Conversion to hb_ubytes_t */
  चालक hb_ubytes_t () अचर { फर्काऊ str.sub_array (get_offset ()); }

  hb_ubytes_t sub_array (अचिन्हित पूर्णाङ्क offset_, अचिन्हित पूर्णाङ्क len_) अचर
  { फर्काऊ str.sub_array (offset_, len_); }

  तार्किक avail (अचिन्हित पूर्णाङ्क गन्ती=1) अचर
  { फर्काऊ get_offset () + गन्ती <= str.length; }
  शून्य inc (अचिन्हित पूर्णाङ्क गन्ती=1)
  {
    /* Automatically puts us in error if count is out-of-range. */
    set_offset (get_offset () + गन्ती);
  }

  /* We (ab)use ubytes backwards_length as a cursor (called offset),
   * as well as to store error condition. */

  अचिन्हित get_offset () अचर { फर्काऊ str.backwards_length; }
  शून्य set_offset (अचिन्हित offset) { str.backwards_length = offset; }

  शून्य set_error ()      { str.backwards_length = str.length + 1; }
  तार्किक in_error () अचर { फर्काऊ str.backwards_length > str.length; }

  अचिन्हित total_size () अचर { फर्काऊ str.length; }

  रक्षित:
  hb_ubytes_t       str;
};

/*थाक*/
ढाँचा <typename ELEM, पूर्णाङ्क LIMIT>
संरचना cff_थाक_t
{
  ELEM& चालक [] (अचिन्हित पूर्णाङ्क i)
  {
    यदि (unlikely (i >= length))
    {
      set_error ();
      फर्काऊ Crap (ELEM);
    }
    फर्काऊ arrayZ[i];
  }

  शून्य push (अचर ELEM &v)
  {
    यदि (likely (length < LIMIT))
      arrayZ[length++] = v;
    अन्यथा
      set_error ();
  }
  ELEM &push ()
  {
    यदि (likely (length < LIMIT))
      फर्काऊ arrayZ[length++];
    अन्यथा
    {
      set_error ();
      फर्काऊ Crap (ELEM);
    }
  }

  ELEM& झिक ()
  {
    यदि (likely (length > 0))
      फर्काऊ arrayZ[--length];
    अन्यथा
    {
      set_error ();
      फर्काऊ Crap (ELEM);
    }
  }
  शून्य झिक (अचिन्हित पूर्णाङ्क n)
  {
    यदि (likely (length >= n))
      length -= n;
    अन्यथा
      set_error ();
  }

  अचर ELEM& peek ()
  {
    यदि (unlikely (length == 0))
    {
      set_error ();
      फर्काऊ Null (ELEM);
    }
    फर्काऊ arrayZ[length - 1];
  }

  शून्य unpop ()
  {
    यदि (likely (length < LIMIT))
      length++;
    अन्यथा
      set_error ();
  }

  शून्य clear () { length = 0; }

  तार्किक in_error () अचर { फर्काऊ (error); }
  शून्य set_error ()      { error = सत्य; }

  अचिन्हित पूर्णाङ्क get_गन्ती () अचर { फर्काऊ length; }
  तार्किक is_empty () अचर          { फर्काऊ !length; }

  hb_array_t<अचर ELEM> sub_array (अचिन्हित start, अचिन्हित length) अचर
  { फर्काऊ hb_array_t<अचर ELEM> (arrayZ).sub_array (start, length); }

  तार्किक error = असत्य;
  अचिन्हित पूर्णाङ्क length = 0;
  ELEM arrayZ[LIMIT];
};

/* argument stack */
ढाँचा <typename ARG=सङ्ख्या_t>
संरचना arg_थाक_t : cff_थाक_t<ARG, 513>
{
  शून्य थप_int (पूर्णाङ्क v)
  {
    ARG &n = S::push ();
    n.set_int (v);
  }

  शून्य थप_fixed (int32_t v)
  {
    ARG &n = S::push ();
    n.set_fixed (v);
  }

  शून्य थप_real (द्विगुण v)
  {
    ARG &n = S::push ();
    n.set_real (v);
  }

  ARG& झिक_अङ्क () { फर्काऊ एतत्->झिक (); }

  पूर्णाङ्क झिक_int ()
  {
    पूर्णाङ्क v;
    यदि (unlikely (!एतत्->झिक ().to_int (&v)))
      S::set_error ();
    फर्काऊ v;
  }

  अचिन्हित पूर्णाङ्क झिक_uint ()
  {
    पूर्णाङ्क i = झिक_int ();
    यदि (unlikely (i < 0))
    {
      i = 0;
      S::set_error ();
    }
    फर्काऊ (अचिन्हित) i;
  }

  शून्य थप_longint_from_substr (byte_str_ref_t& str_ref)
  {
    थप_int ((पूर्णाङ्क) ((uint32_t (str_ref[0]) << 24) | (str_ref[1] << 16) | (str_ref[2] << 8) | (str_ref[3])));
    str_ref.inc (4);
  }

  तार्किक थप_fixed_from_substr (byte_str_ref_t& str_ref)
  {
    यदि (unlikely (!str_ref.avail (4)))
      फर्काऊ असत्य;
    थप_fixed ((int32_t)*(अचर HBUINT32*)&str_ref[0]);
    str_ref.inc (4);
    फर्काऊ सत्य;
  }

  गुप्त:
  प्रकारनिर्देश cff_थाक_t<ARG, 513> S;
};

/* an operator prefixed by its operands in a byte string */
संरचना op_str_t
{
  /* This used to have a hb_ubytes_t. Using a pointer and length
   * in a particular order, saves 8 bytes in this struct and more
   * in our parsed_cs_op_t subclass. */

  अचर अचिन्हित वर्ण *ptr = nullptr;

  op_code_t  op = OpCode_Invalid;

  uint8_t length = 0;
};

/* base of OP_SERIALIZER */
संरचना op_serializer_t
{
  रक्षित:
  तार्किक copy_opstr (hb_serialize_context_t *c, अचर op_str_t& opstr) अचर
  {
    TRACE_SERIALIZE (एतत्);

    अचिन्हित वर्ण *d = c->allocate_size<अचिन्हित वर्ण> (opstr.length);
    यदि (unlikely (!d)) return_trace (असत्य);
    /* Faster than hb_memcpy for small strings. */
    लागि (अचिन्हित i = 0; i < opstr.length; i++)
      d[i] = opstr.ptr[i];
    return_trace (सत्य);
  }
};

ढाँचा <typename VAL>
संरचना parsed_मानहरू_t
{
  शून्य init ()
  {
    opStart = 0;
    values.init ();
  }
  शून्य fini () { values.fini (); }

  शून्य alloc (अचिन्हित n)
  {
    values.alloc_exact (n);
  }

  शून्य add_op (op_code_t op, अचर byte_str_ref_t& str_ref = byte_str_ref_t (), अचर VAL &v = VAL ())
  {
    VAL *val = values.push (v);
    val->op = op;
    स्वतः arr = str_ref.sub_array (opStart, str_ref.get_offset () - opStart);
    val->ptr = arr.arrayZ;
    val->length = arr.length;
    opStart = str_ref.get_offset ();
  }

  तार्किक has_op (op_code_t op) अचर
  {
    लागि (अचर स्वतः& v : values)
      यदि (v.op == op) फर्काऊ सत्य;
    फर्काऊ असत्य;
  }

  अचिन्हित get_गन्ती () अचर { फर्काऊ values.length; }
  अचर VAL &चालक [] (अचिन्हित पूर्णाङ्क i) अचर { फर्काऊ values[i]; }

  अचिन्हित पूर्णाङ्क       opStart;
  hb_vector_t<VAL>   values;
};

ढाँचा <typename ARG=सङ्ख्या_t>
संरचना interp_env_t
{
  interp_env_t () {}
  interp_env_t (अचर hb_ubytes_t &str_)
  {
    str_ref.reset (str_);
  }
  तार्किक in_error () अचर
  { फर्काऊ str_ref.in_error () || arg_थाक.in_error (); }

  शून्य set_error () { str_ref.set_error (); }

  op_code_t fetch_op ()
  {
    op_code_t  op = OpCode_Invalid;
    यदि (unlikely (!str_ref.avail ()))
      फर्काऊ OpCode_Invalid;
    op = (op_code_t) str_ref.head_unchecked ();
    str_ref.inc ();
    यदि (op == OpCode_escape) {
      यदि (unlikely (!str_ref.avail ()))
	फर्काऊ OpCode_Invalid;
      op = Make_OpCode_ESC (str_ref.head_unchecked ());
      str_ref.inc ();
    }
    फर्काऊ op;
  }

  अचर ARG& eval_arg (अचिन्हित पूर्णाङ्क i) { फर्काऊ arg_थाक[i]; }

  ARG& झिक_arg () { फर्काऊ arg_थाक.झिक (); }
  शून्य झिक_n_args (अचिन्हित पूर्णाङ्क n) { arg_थाक.झिक (n); }

  शून्य clear_args () { झिक_n_args (arg_थाक.get_गन्ती ()); }

  byte_str_ref_t
		str_ref;
  arg_थाक_t<ARG>
		arg_थाक;
};

using अङ्क_interp_env_t =  interp_env_t<>;

ढाँचा <typename ARG=सङ्ख्या_t>
संरचना opset_t
{
  स्थिर शून्य process_op (op_code_t op, interp_env_t<ARG>& env)
  {
    विकल्प (op) {
      पक्ष OpCode_shortint:
	env.arg_थाक.थप_int ((int16_t)((env.str_ref[0] << 8) | env.str_ref[1]));
	env.str_ref.inc (2);
	रोक;

      पक्ष OpCode_TwoBytePosInt0: पक्ष OpCode_TwoBytePosInt1:
      पक्ष OpCode_TwoBytePosInt2: पक्ष OpCode_TwoBytePosInt3:
	env.arg_थाक.थप_int ((int16_t)((op - OpCode_TwoBytePosInt0) * 256 + env.str_ref[0] + 108));
	env.str_ref.inc ();
	रोक;

      पक्ष OpCode_TwoByteNegInt0: पक्ष OpCode_TwoByteNegInt1:
      पक्ष OpCode_TwoByteNegInt2: पक्ष OpCode_TwoByteNegInt3:
	env.arg_थाक.थप_int ((-(int16_t)(op - OpCode_TwoByteNegInt0) * 256 - env.str_ref[0] - 108));
	env.str_ref.inc ();
	रोक;

      स्वभाव:
	/* 1-byte integer */
	यदि (likely ((OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast)))
	{
	  env.arg_थाक.थप_int ((पूर्णाङ्क)op - 139);
	} अन्यथा {
	  /* invalid unknown operator */
	  env.clear_args ();
	}
	रोक;
    }
  }
};

ढाँचा <typename ENV>
संरचना interpreter_t
{
  interpreter_t (ENV& env_) : env (env_) {}
  ENV& env;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_COMMON_HH */
