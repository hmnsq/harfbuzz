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

બાહ્ય HB_INTERNAL અચલ unsigned અક્ષર *endchar_str;

નામાકાશ CFF {

using નામાકાશ OT;

પ્રકારવ્યાખ્યા unsigned પૂર્ણાંક op_code_t;


/* === Dict operators === */

/* One byte operators (0-31) */
#define OpCode_version		  0 /* CFF Top */
#define OpCode_Notice		  1 /* CFF Top */
#define OpCode_FullName		  2 /* CFF Top */
#define OpCode_FamilyName	  3 /* CFF Top */
#define OpCode_Weight		  4 /* CFF Top */
#define OpCode_FontBBox		  5 /* CFF Top */
#define Op_Code_Blue_કિંમતો	  6 /* CFF Private, CFF2 Private */
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

અંતઃરેખ op_code_t Unmake_OpCode_ESC (op_code_t op)  { પાછા (op_code_t)(op - OpCode_ESC_Base); }
અંતઃરેખ તાર્કિક Is_OpCode_ESC (op_code_t op) { પાછા op >= OpCode_ESC_Base; }
અંતઃરેખ unsigned પૂર્ણાંક OpCode_Size (op_code_t op) { પાછા Is_OpCode_ESC (op) ? 2: 1; }

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


સંરચના સંખ્યા_t
{
  શૂન્ય set_int (પૂર્ણાંક v)       { value = v; }
  તાર્કિક to_int (પૂર્ણાંક *v) અચલ
  {
    જો (unlikely (!(value >= INT_MIN && value <= INT_MAX)))
    {
      *v = 0;
      પાછા ખોટું;
    }
    *v = (પૂર્ણાંક) value;
    પાછા સાચું;
  }
  પૂર્ણાંક to_int () અચલ
  {
    પૂર્ણાંક v;
    જો (likely (to_int (&v))) પાછા v;
    પાછા value < 0 ? INT_MIN : value > 0 ? INT_MAX : 0;
  }

  શૂન્ય set_fixed (int32_t v) { value = v / 65536.0; }
  int32_t to_fixed () અચલ
  { પાછા (int32_t) hb_clamp (value * 65536.0, (દ્વિગુણ) INT32_MIN, (દ્વિગુણ) INT32_MAX); }

  શૂન્ય set_real (દ્વિગુણ v)   { value = v; }
  દ્વિગુણ to_real () અચલ    { પાછા value; }

  તાર્કિક in_int_range () અચલ
  { પાછા value >= INT16_MIN && value <= INT16_MAX && (દ્વિગુણ) (int16_t) value == value; }

  તાર્કિક operator >  (અચલ સંખ્યા_t &n) અચલ { પાછા value > n.to_real (); }
  તાર્કિક operator <  (અચલ સંખ્યા_t &n) અચલ { પાછા n > *આ; }
  તાર્કિક operator >= (અચલ સંખ્યા_t &n) અચલ { પાછા !(*આ < n); }
  તાર્કિક operator <= (અચલ સંખ્યા_t &n) અચલ { પાછા !(*આ > n); }

  અચલ સંખ્યા_t &operator += (અચલ સંખ્યા_t &n)
  {
    set_real (to_real () + n.to_real ());

    પાછા *આ;
  }

  સુરક્ષિત:
  દ્વિગુણ value = 0.;
};

/* byte string */
સંરચના UnsizedByteStr : UnsizedArrayOf <HBUINT8>
{
  hb_ubytes_t as_ubytes (unsigned l) અચલ
  { પાછા hb_ubytes_t ((અચલ unsigned અક્ષર *) આ, l); }

  // encode 2-byte int (Dict/CharString) or 4-byte int (Dict)
  template <typename T, typename V>
  સ્થિર તાર્કિક serialize_int (hb_serialize_context_t *c, op_code_t intOp, V value)
  {
    TRACE_SERIALIZE (આ);

    HBUINT8 *p = c->allocate_size<HBUINT8> (1);
    જો (unlikely (!p)) return_trace (ખોટું);
    *p = intOp;

    T *ip = c->allocate_size<T> (T::static_size);
    જો (unlikely (!ip)) return_trace (ખોટું);
    return_trace (c->check_assign (*ip, value, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  template <typename V>
  સ્થિર તાર્કિક serialize_int4 (hb_serialize_context_t *c, V value)
  { પાછા serialize_int<HBINT32> (c, OpCode_longintdict, value); }

  template <typename V>
  સ્થિર તાર્કિક serialize_int2 (hb_serialize_context_t *c, V value)
  { પાછા serialize_int<HBINT16> (c, OpCode_shortint, value); }

  /* Defining null_size allows a Null object may be created. Should be safe because:
   * A descendent struct Dict uses a Null pointer to indicate a missing table,
   * checked before access.
   */
  DEFINE_SIZE_MIN(0);
};

/* A byte string associated with the current offset and an error condition */
સંરચના byte_str_ref_t
{
  byte_str_ref_t ()
    : str () {}

  byte_str_ref_t (અચલ hb_ubytes_t &str_, unsigned પૂર્ણાંક offset_ = 0)
    : str (str_) { set_offset (offset_); }

  શૂન્ય reset (અચલ hb_ubytes_t &str_, unsigned પૂર્ણાંક offset_ = 0)
  {
    str = str_;
    set_offset (offset_);
  }

  અચલ unsigned અક્ષર& operator [] (પૂર્ણાંક i) {
    જો (unlikely ((unsigned પૂર્ણાંક) (get_offset () + i) >= str.length))
    {
      set_error ();
      પાછા Null (unsigned અક્ષર);
    }
    પાછા str.arrayZ[get_offset () + i];
  }

  unsigned અક્ષર head_unchecked () અચલ { પાછા str.arrayZ[get_offset ()]; }

  /* Conversion to hb_ubytes_t */
  operator hb_ubytes_t () અચલ { પાછા str.sub_array (get_offset ()); }

  hb_ubytes_t sub_array (unsigned પૂર્ણાંક offset_, unsigned પૂર્ણાંક len_) અચલ
  { પાછા str.sub_array (offset_, len_); }

  તાર્કિક avail (unsigned પૂર્ણાંક ગણતરી=1) અચલ
  { પાછા get_offset () + ગણતરી <= str.length; }
  શૂન્ય inc (unsigned પૂર્ણાંક ગણતરી=1)
  {
    /* Automatically puts us in error if count is out-of-range. */
    set_offset (get_offset () + ગણતરી);
  }

  /* We (ab)use ubytes backwards_length as a cursor (called offset),
   * as well as to store error condition. */

  unsigned get_offset () અચલ { પાછા str.backwards_length; }
  શૂન્ય set_offset (unsigned offset) { str.backwards_length = offset; }

  શૂન્ય set_error ()      { str.backwards_length = str.length + 1; }
  તાર્કિક in_error () અચલ { પાછા str.backwards_length > str.length; }

  unsigned total_size () અચલ { પાછા str.length; }

  સુરક્ષિત:
  hb_ubytes_t       str;
};

/*ઢગલો*/
template <typename ELEM, પૂર્ણાંક LIMIT>
સંરચના cff_ઢગલો_t
{
  ELEM& operator [] (unsigned પૂર્ણાંક i)
  {
    જો (unlikely (i >= length))
    {
      set_error ();
      પાછા Crap (ELEM);
    }
    પાછા arrayZ[i];
  }

  શૂન્ય push (અચલ ELEM &v)
  {
    જો (likely (length < LIMIT))
      arrayZ[length++] = v;
    નહીંતર
      set_error ();
  }
  ELEM &push ()
  {
    જો (likely (length < LIMIT))
      પાછા arrayZ[length++];
    નહીંતર
    {
      set_error ();
      પાછા Crap (ELEM);
    }
  }

  ELEM& કાઢો ()
  {
    જો (likely (length > 0))
      પાછા arrayZ[--length];
    નહીંતર
    {
      set_error ();
      પાછા Crap (ELEM);
    }
  }
  શૂન્ય કાઢો (unsigned પૂર્ણાંક n)
  {
    જો (likely (length >= n))
      length -= n;
    નહીંતર
      set_error ();
  }

  અચલ ELEM& peek ()
  {
    જો (unlikely (length == 0))
    {
      set_error ();
      પાછા Null (ELEM);
    }
    પાછા arrayZ[length - 1];
  }

  શૂન્ય unpop ()
  {
    જો (likely (length < LIMIT))
      length++;
    નહીંતર
      set_error ();
  }

  શૂન્ય clear () { length = 0; }

  તાર્કિક in_error () અચલ { પાછા (error); }
  શૂન્ય set_error ()      { error = સાચું; }

  unsigned પૂર્ણાંક get_ગણતરી () અચલ { પાછા length; }
  તાર્કિક is_empty () અચલ          { પાછા !length; }

  hb_array_t<અચલ ELEM> sub_array (unsigned start, unsigned length) અચલ
  { પાછા hb_array_t<અચલ ELEM> (arrayZ).sub_array (start, length); }

  તાર્કિક error = ખોટું;
  unsigned પૂર્ણાંક length = 0;
  ELEM arrayZ[LIMIT];
};

/* argument stack */
template <typename ARG=સંખ્યા_t>
સંરચના arg_ઢગલો_t : cff_ઢગલો_t<ARG, 513>
{
  શૂન્ય ધકેલો_int (પૂર્ણાંક v)
  {
    ARG &n = S::push ();
    n.set_int (v);
  }

  શૂન્ય ધકેલો_fixed (int32_t v)
  {
    ARG &n = S::push ();
    n.set_fixed (v);
  }

  શૂન્ય ધકેલો_real (દ્વિગુણ v)
  {
    ARG &n = S::push ();
    n.set_real (v);
  }

  ARG& કાઢો_અંક () { પાછા આ->કાઢો (); }

  પૂર્ણાંક કાઢો_int ()
  {
    પૂર્ણાંક v;
    જો (unlikely (!આ->કાઢો ().to_int (&v)))
      S::set_error ();
    પાછા v;
  }

  unsigned પૂર્ણાંક કાઢો_uint ()
  {
    પૂર્ણાંક i = કાઢો_int ();
    જો (unlikely (i < 0))
    {
      i = 0;
      S::set_error ();
    }
    પાછા (unsigned) i;
  }

  શૂન્ય ધકેલો_longint_from_substr (byte_str_ref_t& str_ref)
  {
    ધકેલો_int ((પૂર્ણાંક) ((uint32_t (str_ref[0]) << 24) | (str_ref[1] << 16) | (str_ref[2] << 8) | (str_ref[3])));
    str_ref.inc (4);
  }

  તાર્કિક ધકેલો_fixed_from_substr (byte_str_ref_t& str_ref)
  {
    જો (unlikely (!str_ref.avail (4)))
      પાછા ખોટું;
    ધકેલો_fixed ((int32_t)*(અચલ HBUINT32*)&str_ref[0]);
    str_ref.inc (4);
    પાછા સાચું;
  }

  ખાનગી:
  પ્રકારવ્યાખ્યા cff_ઢગલો_t<ARG, 513> S;
};

/* an operator prefixed by its operands in a byte string */
સંરચના op_str_t
{
  /* This used to have a hb_ubytes_t. Using a pointer and length
   * in a particular order, saves 8 bytes in this struct and more
   * in our parsed_cs_op_t subclass. */

  અચલ unsigned અક્ષર *ptr = nullptr;

  op_code_t  op = OpCode_Invalid;

  uint8_t length = 0;
};

/* base of OP_SERIALIZER */
સંરચના op_serializer_t
{
  સુરક્ષિત:
  તાર્કિક copy_opstr (hb_serialize_context_t *c, અચલ op_str_t& opstr) અચલ
  {
    TRACE_SERIALIZE (આ);

    unsigned અક્ષર *d = c->allocate_size<unsigned અક્ષર> (opstr.length);
    જો (unlikely (!d)) return_trace (ખોટું);
    /* Faster than hb_memcpy for small strings. */
    માટે (unsigned i = 0; i < opstr.length; i++)
      d[i] = opstr.ptr[i];
    return_trace (સાચું);
  }
};

template <typename VAL>
સંરચના parsed_કિંમતો_t
{
  શૂન્ય init ()
  {
    opStart = 0;
    values.init ();
  }
  શૂન્ય fini () { values.fini (); }

  શૂન્ય alloc (unsigned n)
  {
    values.alloc_exact (n);
  }

  શૂન્ય add_op (op_code_t op, અચલ byte_str_ref_t& str_ref = byte_str_ref_t (), અચલ VAL &v = VAL ())
  {
    VAL *val = values.push (v);
    val->op = op;
    auto arr = str_ref.sub_array (opStart, str_ref.get_offset () - opStart);
    val->ptr = arr.arrayZ;
    val->length = arr.length;
    opStart = str_ref.get_offset ();
  }

  તાર્કિક has_op (op_code_t op) અચલ
  {
    માટે (અચલ auto& v : values)
      જો (v.op == op) પાછા સાચું;
    પાછા ખોટું;
  }

  unsigned get_ગણતરી () અચલ { પાછા values.length; }
  અચલ VAL &operator [] (unsigned પૂર્ણાંક i) અચલ { પાછા values[i]; }

  unsigned પૂર્ણાંક       opStart;
  hb_vector_t<VAL>   values;
};

template <typename ARG=સંખ્યા_t>
સંરચના interp_env_t
{
  interp_env_t () {}
  interp_env_t (અચલ hb_ubytes_t &str_)
  {
    str_ref.reset (str_);
  }
  તાર્કિક in_error () અચલ
  { પાછા str_ref.in_error () || arg_ઢગલો.in_error (); }

  શૂન્ય set_error () { str_ref.set_error (); }

  op_code_t fetch_op ()
  {
    op_code_t  op = OpCode_Invalid;
    જો (unlikely (!str_ref.avail ()))
      પાછા OpCode_Invalid;
    op = (op_code_t) str_ref.head_unchecked ();
    str_ref.inc ();
    જો (op == OpCode_escape) {
      જો (unlikely (!str_ref.avail ()))
	પાછા OpCode_Invalid;
      op = Make_OpCode_ESC (str_ref.head_unchecked ());
      str_ref.inc ();
    }
    પાછા op;
  }

  અચલ ARG& eval_arg (unsigned પૂર્ણાંક i) { પાછા arg_ઢગલો[i]; }

  ARG& કાઢો_arg () { પાછા arg_ઢગલો.કાઢો (); }
  શૂન્ય કાઢો_n_args (unsigned પૂર્ણાંક n) { arg_ઢગલો.કાઢો (n); }

  શૂન્ય clear_args () { કાઢો_n_args (arg_ઢગલો.get_ગણતરી ()); }

  byte_str_ref_t
		str_ref;
  arg_ઢગલો_t<ARG>
		arg_ઢગલો;
};

using અંક_interp_env_t =  interp_env_t<>;

template <typename ARG=સંખ્યા_t>
સંરચના opset_t
{
  સ્થિર શૂન્ય process_op (op_code_t op, interp_env_t<ARG>& env)
  {
    પસંદગી (op) {
      સ્થિતિ OpCode_shortint:
	env.arg_ઢગલો.ધકેલો_int ((int16_t)((env.str_ref[0] << 8) | env.str_ref[1]));
	env.str_ref.inc (2);
	તોડો;

      સ્થિતિ OpCode_TwoBytePosInt0: સ્થિતિ OpCode_TwoBytePosInt1:
      સ્થિતિ OpCode_TwoBytePosInt2: સ્થિતિ OpCode_TwoBytePosInt3:
	env.arg_ઢગલો.ધકેલો_int ((int16_t)((op - OpCode_TwoBytePosInt0) * 256 + env.str_ref[0] + 108));
	env.str_ref.inc ();
	તોડો;

      સ્થિતિ OpCode_TwoByteNegInt0: સ્થિતિ OpCode_TwoByteNegInt1:
      સ્થિતિ OpCode_TwoByteNegInt2: સ્થિતિ OpCode_TwoByteNegInt3:
	env.arg_ઢગલો.ધકેલો_int ((-(int16_t)(op - OpCode_TwoByteNegInt0) * 256 - env.str_ref[0] - 108));
	env.str_ref.inc ();
	તોડો;

      મૂળભૂત:
	/* 1-byte integer */
	જો (likely ((OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast)))
	{
	  env.arg_ઢગલો.ધકેલો_int ((પૂર્ણાંક)op - 139);
	} નહીંતર {
	  /* invalid unknown operator */
	  env.clear_args ();
	}
	તોડો;
    }
  }
};

template <typename ENV>
સંરચના interpreter_t
{
  interpreter_t (ENV& env_) : env (env_) {}
  ENV& env;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_COMMON_HH */
