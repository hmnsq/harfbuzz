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
#अगर_अघोषित HB_CFF_INTERP_COMMON_HH
#घोषणा HB_CFF_INTERP_COMMON_HH

बाह्य HB_INTERNAL स्थिर अचिन्हित अक्षर *endchar_str;

namespace CFF {

using namespace OT;

प्रकार अचिन्हित पूर्णांक op_code_t;


/* === Dict operators === */

/* One byte operators (0-31) */
#घोषणा OpCode_version		  0 /* CFF Top */
#घोषणा OpCode_Notice		  1 /* CFF Top */
#घोषणा OpCode_FullName		  2 /* CFF Top */
#घोषणा OpCode_FamilyName	  3 /* CFF Top */
#घोषणा OpCode_Weight		  4 /* CFF Top */
#घोषणा OpCode_FontBBox		  5 /* CFF Top */
#घोषणा Op_Code_Blue_मूल्यावली	  6 /* CFF Private, CFF2 Private */
#घोषणा OpCode_OtherBlues	  7 /* CFF Private, CFF2 Private */
#घोषणा OpCode_FamilyBlues	  8 /* CFF Private, CFF2 Private */
#घोषणा OpCode_FamilyOtherBlues	  9 /* CFF Private, CFF2 Private */
#घोषणा OpCode_StdHW		 10 /* CFF Private, CFF2 Private */
#घोषणा OpCode_StdVW		 11 /* CFF Private, CFF2 Private */
#घोषणा OpCode_escape		 12 /* All. Shared with CS */
#घोषणा OpCode_UniqueID		 13 /* CFF Top */
#घोषणा OpCode_XUID		 14 /* CFF Top */
#घोषणा OpCode_charset		 15 /* CFF Top (0) */
#घोषणा OpCode_Encoding		 16 /* CFF Top (0) */
#घोषणा OpCode_CharStrings	 17 /* CFF Top, CFF2 Top */
#घोषणा OpCode_Private		 18 /* CFF Top, CFF2 FD */
#घोषणा OpCode_Subrs		 19 /* CFF Private, CFF2 Private */
#घोषणा OpCode_defaultWidthX	 20 /* CFF Private (0) */
#घोषणा OpCode_nominalWidthX	 21 /* CFF Private (0) */
#घोषणा OpCode_vsindexdict	 22 /* CFF2 Private/CS */
#घोषणा OpCode_blenddict	 23 /* CFF2 Private/CS */
#घोषणा OpCode_vstore		 24 /* CFF2 Top */
#घोषणा OpCode_reserved25	 25
#घोषणा OpCode_reserved26	 26
#घोषणा OpCode_reserved27	 27

/* Numbers */
#घोषणा OpCode_shortint		 28 /* 16-bit integer, All */
#घोषणा OpCode_longintdict	 29 /* 32-bit integer, All */
#घोषणा OpCode_BCD		 30 /* Real number, CFF2 Top/FD */
#घोषणा OpCode_reserved31	 31

/* 1-byte integers */
#घोषणा OpCode_OneByteIntFirst	 32 /* All. beginning of the range of first byte ints */
#घोषणा OpCode_OneByteIntLast	246 /* All. ending of the range of first byte int */

/* 2-byte integers */
#घोषणा OpCode_TwoBytePosInt0	247 /* All. first byte of two byte positive int (+108 to +1131) */
#घोषणा OpCode_TwoBytePosInt1	248
#घोषणा OpCode_TwoBytePosInt2	249
#घोषणा OpCode_TwoBytePosInt3	250

#घोषणा OpCode_TwoByteNegInt0	251 /* All. first byte of two byte negative int (-1131 to -108) */
#घोषणा OpCode_TwoByteNegInt1	252
#घोषणा OpCode_TwoByteNegInt2	253
#घोषणा OpCode_TwoByteNegInt3	254

/* Two byte escape operators 12, (0-41) */
#घोषणा OpCode_ESC_Base		256
#घोषणा Make_OpCode_ESC(byte2)	((op_code_t)(OpCode_ESC_Base + (byte2)))

अंतरभूत op_code_t Unmake_OpCode_ESC (op_code_t op)  { वापस (op_code_t)(op - OpCode_ESC_Base); }
अंतरभूत bool Is_OpCode_ESC (op_code_t op) { वापस op >= OpCode_ESC_Base; }
अंतरभूत अचिन्हित पूर्णांक Op_Code_आकार (op_code_t op) { वापस Is_OpCode_ESC (op) ? 2: 1; }

#घोषणा OpCode_Copyright	Make_OpCode_ESC(0) /* CFF Top */
#घोषणा OpCode_isFixedPitch	Make_OpCode_ESC(1) /* CFF Top (false) */
#घोषणा OpCode_ItalicAngle	Make_OpCode_ESC(2) /* CFF Top (0) */
#घोषणा OpCode_UnderlinePosition Make_OpCode_ESC(3) /* CFF Top (-100) */
#घोषणा OpCode_UnderlineThickness Make_OpCode_ESC(4) /* CFF Top (50) */
#घोषणा OpCode_PaintType	Make_OpCode_ESC(5) /* CFF Top (0) */
#घोषणा OpCode_CharstringType	Make_OpCode_ESC(6) /* CFF Top (2) */
#घोषणा OpCode_FontMatrix	Make_OpCode_ESC(7) /* CFF Top, CFF2 Top (.001 0 0 .001 0 0)*/
#घोषणा OpCode_StrokeWidth	Make_OpCode_ESC(8) /* CFF Top (0) */
#घोषणा OpCode_BlueScale	Make_OpCode_ESC(9) /* CFF Private, CFF2 Private (0.039625) */
#घोषणा OpCode_BlueShift	Make_OpCode_ESC(10) /* CFF Private, CFF2 Private (7) */
#घोषणा OpCode_BlueFuzz		Make_OpCode_ESC(11) /* CFF Private, CFF2 Private (1) */
#घोषणा OpCode_StemSnapH	Make_OpCode_ESC(12) /* CFF Private, CFF2 Private */
#घोषणा OpCode_StemSnapV	Make_OpCode_ESC(13) /* CFF Private, CFF2 Private */
#घोषणा OpCode_ForceBold	Make_OpCode_ESC(14) /* CFF Private (false) */
#घोषणा OpCode_reservedESC15	Make_OpCode_ESC(15)
#घोषणा OpCode_reservedESC16	Make_OpCode_ESC(16)
#घोषणा OpCode_LanguageGroup	Make_OpCode_ESC(17) /* CFF Private, CFF2 Private (0) */
#घोषणा OpCode_ExpansionFactor	Make_OpCode_ESC(18) /* CFF Private, CFF2 Private (0.06) */
#घोषणा OpCode_initialRandomSeed Make_OpCode_ESC(19) /* CFF Private (0) */
#घोषणा OpCode_SyntheticBase	Make_OpCode_ESC(20) /* CFF Top */
#घोषणा OpCode_PostScript	Make_OpCode_ESC(21) /* CFF Top */
#घोषणा OpCode_BaseFontName	Make_OpCode_ESC(22) /* CFF Top */
#घोषणा OpCode_BaseFontBlend	Make_OpCode_ESC(23) /* CFF Top */
#घोषणा OpCode_reservedESC24	Make_OpCode_ESC(24)
#घोषणा OpCode_reservedESC25	Make_OpCode_ESC(25)
#घोषणा OpCode_reservedESC26	Make_OpCode_ESC(26)
#घोषणा OpCode_reservedESC27	Make_OpCode_ESC(27)
#घोषणा OpCode_reservedESC28	Make_OpCode_ESC(28)
#घोषणा OpCode_reservedESC29	Make_OpCode_ESC(29)
#घोषणा OpCode_ROS		Make_OpCode_ESC(30) /* CFF Top_CID */
#घोषणा OpCode_CIDFontVersion	Make_OpCode_ESC(31) /* CFF Top_CID (0) */
#घोषणा OpCode_CIDFontRevision	Make_OpCode_ESC(32) /* CFF Top_CID (0) */
#घोषणा OpCode_CIDFontType	Make_OpCode_ESC(33) /* CFF Top_CID (0) */
#घोषणा OpCode_CIDCount		Make_OpCode_ESC(34) /* CFF Top_CID (8720) */
#घोषणा OpCode_UIDBase		Make_OpCode_ESC(35) /* CFF Top_CID */
#घोषणा OpCode_FDArray		Make_OpCode_ESC(36) /* CFF Top_CID, CFF2 Top */
#घोषणा OpCode_FDSelect		Make_OpCode_ESC(37) /* CFF Top_CID, CFF2 Top */
#घोषणा OpCode_FontName		Make_OpCode_ESC(38) /* CFF Top_CID */


/* === CharString operators === */

#घोषणा OpCode_hstem		  1 /* CFF, CFF2 */
#घोषणा OpCode_Reserved2	  2
#घोषणा OpCode_vstem		  3 /* CFF, CFF2 */
#घोषणा OpCode_vmoveto		  4 /* CFF, CFF2 */
#घोषणा OpCode_rlineto		  5 /* CFF, CFF2 */
#घोषणा OpCode_hlineto		  6 /* CFF, CFF2 */
#घोषणा OpCode_vlineto		  7 /* CFF, CFF2 */
#घोषणा OpCode_rrcurveto	  8 /* CFF, CFF2 */
#घोषणा OpCode_Reserved9	  9
#घोषणा OpCode_callsubr		 10 /* CFF, CFF2 */
#घोषणा OpCode_return		 11 /* CFF */
//#define OpCode_escape		 12 /* CFF, CFF2 */
#घोषणा OpCode_Reserved13	 13
#घोषणा OpCode_endchar		 14 /* CFF */
#घोषणा OpCode_vsindexcs	 15 /* CFF2 */
#घोषणा OpCode_blendcs		 16 /* CFF2 */
#घोषणा OpCode_Reserved17	 17
#घोषणा OpCode_hstemhm		 18 /* CFF, CFF2 */
#घोषणा OpCode_hintmask		 19 /* CFF, CFF2 */
#घोषणा OpCode_cntrmask		 20 /* CFF, CFF2 */
#घोषणा OpCode_rmoveto		 21 /* CFF, CFF2 */
#घोषणा OpCode_hmoveto		 22 /* CFF, CFF2 */
#घोषणा OpCode_vstemhm		 23 /* CFF, CFF2 */
#घोषणा OpCode_rcurveline	 24 /* CFF, CFF2 */
#घोषणा OpCode_rlinecurve	 25 /* CFF, CFF2 */
#घोषणा OpCode_vvcurveto	 26 /* CFF, CFF2 */
#घोषणा OpCode_hhcurveto	 27 /* CFF, CFF2 */
//#define OpCode_shortint	 28 /* CFF, CFF2 */
#घोषणा OpCode_callgsubr	 29 /* CFF, CFF2 */
#घोषणा OpCode_vhcurveto	 30 /* CFF, CFF2 */
#घोषणा OpCode_hvcurveto	 31 /* CFF, CFF2 */

#घोषणा OpCode_fixedcs		255 /* 32-bit fixed */

/* Two byte escape operators 12, (0-41) */
#घोषणा OpCode_dotsection	Make_OpCode_ESC(0) /* CFF (obsoleted) */
#घोषणा OpCode_ReservedESC1	Make_OpCode_ESC(1)
#घोषणा OpCode_ReservedESC2	Make_OpCode_ESC(2)
#घोषणा OpCode_and		Make_OpCode_ESC(3) /* CFF */
#घोषणा OpCode_or		Make_OpCode_ESC(4) /* CFF */
#घोषणा OpCode_not		Make_OpCode_ESC(5) /* CFF */
#घोषणा OpCode_ReservedESC6	Make_OpCode_ESC(6)
#घोषणा OpCode_ReservedESC7	Make_OpCode_ESC(7)
#घोषणा OpCode_ReservedESC8	Make_OpCode_ESC(8)
#घोषणा OpCode_abs		Make_OpCode_ESC(9) /* CFF */
#घोषणा OpCode_add		Make_OpCode_ESC(10) /* CFF */
#घोषणा OpCode_sub		Make_OpCode_ESC(11) /* CFF */
#घोषणा OpCode_div		Make_OpCode_ESC(12) /* CFF */
#घोषणा OpCode_ReservedESC13	Make_OpCode_ESC(13)
#घोषणा OpCode_neg		Make_OpCode_ESC(14) /* CFF */
#घोषणा OpCode_eq		Make_OpCode_ESC(15) /* CFF */
#घोषणा OpCode_ReservedESC16	Make_OpCode_ESC(16)
#घोषणा OpCode_ReservedESC17	Make_OpCode_ESC(17)
#घोषणा OpCode_drop		Make_OpCode_ESC(18) /* CFF */
#घोषणा OpCode_ReservedESC19	Make_OpCode_ESC(19)
#घोषणा OpCode_put		Make_OpCode_ESC(20) /* CFF */
#घोषणा OpCode_get		Make_OpCode_ESC(21) /* CFF */
#घोषणा OpCode_ifelse		Make_OpCode_ESC(22) /* CFF */
#घोषणा OpCode_random		Make_OpCode_ESC(23) /* CFF */
#घोषणा OpCode_mul		Make_OpCode_ESC(24) /* CFF */
//#define OpCode_reservedESC25	Make_OpCode_ESC(25)
#घोषणा OpCode_sqrt		Make_OpCode_ESC(26) /* CFF */
#घोषणा OpCode_dup		Make_OpCode_ESC(27) /* CFF */
#घोषणा OpCode_exch		Make_OpCode_ESC(28) /* CFF */
#घोषणा Op_Code_सूचकांक		Make_OpCode_ESC(29) /* CFF */
#घोषणा OpCode_roll		Make_OpCode_ESC(30) /* CFF */
#घोषणा OpCode_reservedESC31	Make_OpCode_ESC(31)
#घोषणा OpCode_reservedESC32	Make_OpCode_ESC(32)
#घोषणा OpCode_reservedESC33	Make_OpCode_ESC(33)
#घोषणा OpCode_hflex		Make_OpCode_ESC(34) /* CFF, CFF2 */
#घोषणा OpCode_flex		Make_OpCode_ESC(35) /* CFF, CFF2 */
#घोषणा OpCode_hflex1		Make_OpCode_ESC(36) /* CFF, CFF2 */
#घोषणा OpCode_flex1		Make_OpCode_ESC(37) /* CFF, CFF2 */


#घोषणा OpCode_Invalid		0xFFFFu


काष्ठा संख्या_t
{
  व्योम set_int (पूर्णांक v)       { value = v; }
  bool to_int (पूर्णांक *v) स्थिर
  {
    अगर (unlikely (!(value >= पूर्णांक_न्यून && value <= पूर्णांक_उच्च)))
    {
      *v = 0;
      वापस false;
    }
    *v = (पूर्णांक) value;
    वापस true;
  }
  पूर्णांक to_int () स्थिर
  {
    पूर्णांक v;
    अगर (likely (to_int (&v))) वापस v;
    वापस value < 0 ? पूर्णांक_न्यून : value > 0 ? पूर्णांक_उच्च : 0;
  }

  व्योम set_fixed (int32_t v) { value = v / 65536.0; }
  int32_t to_fixed () स्थिर
  { वापस (int32_t) hb_clamp (value * 65536.0, (द्विगुन) INT32_MIN, (द्विगुन) INT32_MAX); }

  व्योम set_real (द्विगुन v)   { value = v; }
  द्विगुन to_real () स्थिर    { वापस value; }

  bool in_int_range () स्थिर
  { वापस value >= INT16_MIN && value <= INT16_MAX && (द्विगुन) (int16_t) value == value; }

  bool चालक >  (स्थिर संख्या_t &n) स्थिर { वापस value > n.to_real (); }
  bool चालक <  (स्थिर संख्या_t &n) स्थिर { वापस n > *यह; }
  bool चालक >= (स्थिर संख्या_t &n) स्थिर { वापस !(*यह < n); }
  bool चालक <= (स्थिर संख्या_t &n) स्थिर { वापस !(*यह > n); }

  स्थिर संख्या_t &चालक += (स्थिर संख्या_t &n)
  {
    set_real (to_real () + n.to_real ());

    वापस *यह;
  }

  रक्षित:
  द्विगुन value = 0.;
};

/* byte string */
काष्ठा UnsizedByteStr : UnsizedArrayOf <HBUINT8>
{
  hb_ubytes_t as_ubytes (अचिन्हित l) स्थिर
  { वापस hb_ubytes_t ((स्थिर अचिन्हित अक्षर *) यह, l); }

  // encode 2-byte int (Dict/CharString) or 4-byte int (Dict)
  ढाँचा <typename T, typename V>
  अटल bool serialize_int (hb_serialize_context_t *c, op_code_t intOp, V value)
  {
    TRACE_SERIALIZE (यह);

    HBUINT8 *p = c->allocate_आकार<HBUINT8> (1);
    अगर (unlikely (!p)) return_trace (false);
    *p = intOp;

    T *ip = c->allocate_आकार<T> (T::static_आकार);
    अगर (unlikely (!ip)) return_trace (false);
    return_trace (c->check_assign (*ip, value, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ढाँचा <typename V>
  अटल bool serialize_int4 (hb_serialize_context_t *c, V value)
  { वापस serialize_int<HBINT32> (c, OpCode_longintdict, value); }

  ढाँचा <typename V>
  अटल bool serialize_int2 (hb_serialize_context_t *c, V value)
  { वापस serialize_int<HBINT16> (c, OpCode_shortint, value); }

  /* Defining null_size allows a Null object may be created. Should be safe because:
   * A descendent struct Dict uses a Null pointer to indicate a missing table,
   * checked before access.
   */
  DEFINE_आकार_MIN(0);
};

/* A byte string associated with the current offset and an error condition */
काष्ठा byte_str_ref_t
{
  byte_str_ref_t ()
    : str () {}

  byte_str_ref_t (स्थिर hb_ubytes_t &str_, अचिन्हित पूर्णांक offset_ = 0)
    : str (str_) { set_offset (offset_); }

  व्योम reset (स्थिर hb_ubytes_t &str_, अचिन्हित पूर्णांक offset_ = 0)
  {
    str = str_;
    set_offset (offset_);
  }

  स्थिर अचिन्हित अक्षर& चालक [] (पूर्णांक i) {
    अगर (unlikely ((अचिन्हित पूर्णांक) (get_offset () + i) >= str.लंबाई))
    {
      set_error ();
      वापस Null (अचिन्हित अक्षर);
    }
    वापस str.arrayZ[get_offset () + i];
  }

  अचिन्हित अक्षर शीर्ष_unchecked () स्थिर { वापस str.arrayZ[get_offset ()]; }

  /* Conversion to hb_ubytes_t */
  चालक hb_ubytes_t () स्थिर { वापस str.sub_array (get_offset ()); }

  hb_ubytes_t sub_array (अचिन्हित पूर्णांक offset_, अचिन्हित पूर्णांक len_) स्थिर
  { वापस str.sub_array (offset_, len_); }

  bool avail (अचिन्हित पूर्णांक गिनती=1) स्थिर
  { वापस get_offset () + गिनती <= str.लंबाई; }
  व्योम inc (अचिन्हित पूर्णांक गिनती=1)
  {
    /* Automatically puts us in error if count is out-of-range. */
    set_offset (get_offset () + गिनती);
  }

  /* We (ab)use ubytes backwards_length as a cursor (called offset),
   * as well as to store error condition. */

  अचिन्हित get_offset () स्थिर { वापस str.backwards_लंबाई; }
  व्योम set_offset (अचिन्हित offset) { str.backwards_लंबाई = offset; }

  व्योम set_error ()      { str.backwards_लंबाई = str.लंबाई + 1; }
  bool in_error () स्थिर { वापस str.backwards_लंबाई > str.लंबाई; }

  अचिन्हित योग_आकार () स्थिर { वापस str.लंबाई; }

  रक्षित:
  hb_ubytes_t       str;
};

/*ढेर*/
ढाँचा <typename ELEM, पूर्णांक LIMIT>
काष्ठा cff_ढेर_t
{
  ELEM& चालक [] (अचिन्हित पूर्णांक i)
  {
    अगर (unlikely (i >= लंबाई))
    {
      set_error ();
      वापस Crap (ELEM);
    }
    वापस arrayZ[i];
  }

  व्योम push (स्थिर ELEM &v)
  {
    अगर (likely (लंबाई < LIMIT))
      arrayZ[लंबाई++] = v;
    अन्यथा
      set_error ();
  }
  ELEM &push ()
  {
    अगर (likely (लंबाई < LIMIT))
      वापस arrayZ[लंबाई++];
    अन्यथा
    {
      set_error ();
      वापस Crap (ELEM);
    }
  }

  ELEM& निकालो ()
  {
    अगर (likely (लंबाई > 0))
      वापस arrayZ[--लंबाई];
    अन्यथा
    {
      set_error ();
      वापस Crap (ELEM);
    }
  }
  व्योम निकालो (अचिन्हित पूर्णांक n)
  {
    अगर (likely (लंबाई >= n))
      लंबाई -= n;
    अन्यथा
      set_error ();
  }

  स्थिर ELEM& peek ()
  {
    अगर (unlikely (लंबाई == 0))
    {
      set_error ();
      वापस Null (ELEM);
    }
    वापस arrayZ[लंबाई - 1];
  }

  व्योम unpop ()
  {
    अगर (likely (लंबाई < LIMIT))
      लंबाई++;
    अन्यथा
      set_error ();
  }

  व्योम सफा () { लंबाई = 0; }

  bool in_error () स्थिर { वापस (error); }
  व्योम set_error ()      { error = true; }

  अचिन्हित पूर्णांक get_गिनती () स्थिर { वापस लंबाई; }
  bool is_empty () स्थिर          { वापस !लंबाई; }

  hb_array_t<स्थिर ELEM> sub_array (अचिन्हित start, अचिन्हित लंबाई) स्थिर
  { वापस hb_array_t<स्थिर ELEM> (arrayZ).sub_array (start, लंबाई); }

  bool error = false;
  अचिन्हित पूर्णांक लंबाई = 0;
  ELEM arrayZ[LIMIT];
};

/* argument stack */
ढाँचा <typename ARG=संख्या_t>
काष्ठा arg_ढेर_t : cff_ढेर_t<ARG, 513>
{
  व्योम धकेलो_int (पूर्णांक v)
  {
    ARG &n = S::push ();
    n.set_int (v);
  }

  व्योम धकेलो_fixed (int32_t v)
  {
    ARG &n = S::push ();
    n.set_fixed (v);
  }

  व्योम धकेलो_real (द्विगुन v)
  {
    ARG &n = S::push ();
    n.set_real (v);
  }

  ARG& निकालो_अंक () { वापस यह->निकालो (); }

  पूर्णांक निकालो_int ()
  {
    पूर्णांक v;
    अगर (unlikely (!यह->निकालो ().to_int (&v)))
      S::set_error ();
    वापस v;
  }

  अचिन्हित पूर्णांक निकालो_uint ()
  {
    पूर्णांक i = निकालो_int ();
    अगर (unlikely (i < 0))
    {
      i = 0;
      S::set_error ();
    }
    वापस (अचिन्हित) i;
  }

  व्योम धकेलो_longint_from_substr (byte_str_ref_t& str_ref)
  {
    धकेलो_int ((पूर्णांक) ((uint32_t (str_ref[0]) << 24) | (str_ref[1] << 16) | (str_ref[2] << 8) | (str_ref[3])));
    str_ref.inc (4);
  }

  bool धकेलो_fixed_from_substr (byte_str_ref_t& str_ref)
  {
    अगर (unlikely (!str_ref.avail (4)))
      वापस false;
    धकेलो_fixed ((int32_t)*(स्थिर HBUINT32*)&str_ref[0]);
    str_ref.inc (4);
    वापस true;
  }

  निजी:
  प्रकार cff_ढेर_t<ARG, 513> S;
};

/* an operator prefixed by its operands in a byte string */
काष्ठा op_str_t
{
  /* This used to have a hb_ubytes_t. Using a pointer and length
   * in a particular order, saves 8 bytes in this struct and more
   * in our parsed_cs_op_t subclass. */

  स्थिर अचिन्हित अक्षर *ptr = nullptr;

  op_code_t  op = OpCode_Invalid;

  uint8_t लंबाई = 0;
};

/* base of OP_SERIALIZER */
काष्ठा op_serializer_t
{
  रक्षित:
  bool copy_opstr (hb_serialize_context_t *c, स्थिर op_str_t& opstr) स्थिर
  {
    TRACE_SERIALIZE (यह);

    अचिन्हित अक्षर *d = c->allocate_आकार<अचिन्हित अक्षर> (opstr.लंबाई);
    अगर (unlikely (!d)) return_trace (false);
    /* Faster than hb_memcpy for small strings. */
    क्रम (अचिन्हित i = 0; i < opstr.लंबाई; i++)
      d[i] = opstr.ptr[i];
    return_trace (true);
  }
};

ढाँचा <typename VAL>
काष्ठा parsed_मूल्यावली_t
{
  व्योम init ()
  {
    opStart = 0;
    values.init ();
  }
  व्योम fini () { values.fini (); }

  व्योम alloc (अचिन्हित n)
  {
    values.alloc_exact (n);
  }

  व्योम add_op (op_code_t op, स्थिर byte_str_ref_t& str_ref = byte_str_ref_t (), स्थिर VAL &v = VAL ())
  {
    VAL *val = values.push (v);
    val->op = op;
    स्वतः arr = str_ref.sub_array (opStart, str_ref.get_offset () - opStart);
    val->ptr = arr.arrayZ;
    val->लंबाई = arr.लंबाई;
    opStart = str_ref.get_offset ();
  }

  bool has_op (op_code_t op) स्थिर
  {
    क्रम (स्थिर स्वतः& v : values)
      अगर (v.op == op) वापस true;
    वापस false;
  }

  अचिन्हित get_गिनती () स्थिर { वापस values.लंबाई; }
  स्थिर VAL &चालक [] (अचिन्हित पूर्णांक i) स्थिर { वापस values[i]; }

  अचिन्हित पूर्णांक       opStart;
  hb_vector_t<VAL>   values;
};

ढाँचा <typename ARG=संख्या_t>
काष्ठा interp_env_t
{
  interp_env_t () {}
  interp_env_t (स्थिर hb_ubytes_t &str_)
  {
    str_ref.reset (str_);
  }
  bool in_error () स्थिर
  { वापस str_ref.in_error () || arg_ढेर.in_error (); }

  व्योम set_error () { str_ref.set_error (); }

  op_code_t fetch_op ()
  {
    op_code_t  op = OpCode_Invalid;
    अगर (unlikely (!str_ref.avail ()))
      वापस OpCode_Invalid;
    op = (op_code_t) str_ref.शीर्ष_unchecked ();
    str_ref.inc ();
    अगर (op == OpCode_escape) {
      अगर (unlikely (!str_ref.avail ()))
	वापस OpCode_Invalid;
      op = Make_OpCode_ESC (str_ref.शीर्ष_unchecked ());
      str_ref.inc ();
    }
    वापस op;
  }

  स्थिर ARG& eval_arg (अचिन्हित पूर्णांक i) { वापस arg_ढेर[i]; }

  ARG& निकालो_arg () { वापस arg_ढेर.निकालो (); }
  व्योम निकालो_n_args (अचिन्हित पूर्णांक n) { arg_ढेर.निकालो (n); }

  व्योम clear_args () { निकालो_n_args (arg_ढेर.get_गिनती ()); }

  byte_str_ref_t
		str_ref;
  arg_ढेर_t<ARG>
		arg_ढेर;
};

using अंक_interp_env_t =  interp_env_t<>;

ढाँचा <typename ARG=संख्या_t>
काष्ठा opset_t
{
  अटल व्योम process_op (op_code_t op, interp_env_t<ARG>& env)
  {
    चयन (op) {
      हाल OpCode_shortint:
	env.arg_ढेर.धकेलो_int ((int16_t)((env.str_ref[0] << 8) | env.str_ref[1]));
	env.str_ref.inc (2);
	अवरोध;

      हाल OpCode_TwoBytePosInt0: हाल OpCode_TwoBytePosInt1:
      हाल OpCode_TwoBytePosInt2: हाल OpCode_TwoBytePosInt3:
	env.arg_ढेर.धकेलो_int ((int16_t)((op - OpCode_TwoBytePosInt0) * 256 + env.str_ref[0] + 108));
	env.str_ref.inc ();
	अवरोध;

      हाल OpCode_TwoByteNegInt0: हाल OpCode_TwoByteNegInt1:
      हाल OpCode_TwoByteNegInt2: हाल OpCode_TwoByteNegInt3:
	env.arg_ढेर.धकेलो_int ((-(int16_t)(op - OpCode_TwoByteNegInt0) * 256 - env.str_ref[0] - 108));
	env.str_ref.inc ();
	अवरोध;

      शेष:
	/* 1-byte integer */
	अगर (likely ((OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast)))
	{
	  env.arg_ढेर.धकेलो_int ((पूर्णांक)op - 139);
	} अन्यथा {
	  /* invalid unknown operator */
	  env.clear_args ();
	}
	अवरोध;
    }
  }
};

ढाँचा <typename ENV>
काष्ठा interpreter_t
{
  interpreter_t (ENV& env_) : env (env_) {}
  ENV& env;
};

} /* namespace CFF */

#पूर्ण_अगर /* HB_CFF_INTERP_COMMON_HH */
