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

বাহ্য HB_INTERNAL ধ্রুৱ unsigned ৱর্ণ *endchar_str;

নামাকাশ CFF {

using নামাকাশ OT;

প্রকারনির্দেশ unsigned পূর্ণাঙ্ক op_code_t;


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

অন্তঃস্থ op_code_t Unmake_OpCode_ESC (op_code_t op)  { ঘূৰাই (op_code_t)(op - OpCode_ESC_Base); }
অন্তঃস্থ তার্কিক Is_OpCode_ESC (op_code_t op) { ঘূৰাই op >= OpCode_ESC_Base; }
অন্তঃস্থ unsigned পূর্ণাঙ্ক OpCode_Size (op_code_t op) { ঘূৰাই Is_OpCode_ESC (op) ? 2: 1; }

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


সংরচনা সংখ্যা_t
{
  শূন্য set_int (পূর্ণাঙ্ক v)       { value = v; }
  তার্কিক to_int (পূর্ণাঙ্ক *v) ধ্রুৱ
  {
    যদি (unlikely (!(value >= INT_MIN && value <= INT_MAX)))
    {
      *v = 0;
      ঘূৰাই মিছা;
    }
    *v = (পূর্ণাঙ্ক) value;
    ঘূৰাই সঁচা;
  }
  পূর্ণাঙ্ক to_int () ধ্রুৱ
  {
    পূর্ণাঙ্ক v;
    যদি (likely (to_int (&v))) ঘূৰাই v;
    ঘূৰাই value < 0 ? INT_MIN : value > 0 ? INT_MAX : 0;
  }

  শূন্য set_fixed (int32_t v) { value = v / 65536.0; }
  int32_t to_fixed () ধ্রুৱ
  { ঘূৰাই (int32_t) hb_clamp (value * 65536.0, (দ্ৱিগুণ) INT32_MIN, (দ্ৱিগুণ) INT32_MAX); }

  শূন্য set_real (দ্ৱিগুণ v)   { value = v; }
  দ্ৱিগুণ to_real () ধ্রুৱ    { ঘূৰাই value; }

  তার্কিক in_int_range () ধ্রুৱ
  { ঘূৰাই value >= INT16_MIN && value <= INT16_MAX && (দ্ৱিগুণ) (int16_t) value == value; }

  তার্কিক operator >  (ধ্রুৱ সংখ্যা_t &n) ধ্রুৱ { ঘূৰাই value > n.to_real (); }
  তার্কিক operator <  (ধ্রুৱ সংখ্যা_t &n) ধ্রুৱ { ঘূৰাই n > *এতত্; }
  তার্কিক operator >= (ধ্রুৱ সংখ্যা_t &n) ধ্রুৱ { ঘূৰাই !(*এতত্ < n); }
  তার্কিক operator <= (ধ্রুৱ সংখ্যা_t &n) ধ্রুৱ { ঘূৰাই !(*এতত্ > n); }

  ধ্রুৱ সংখ্যা_t &operator += (ধ্রুৱ সংখ্যা_t &n)
  {
    set_real (to_real () + n.to_real ());

    ঘূৰাই *এতত্;
  }

  রক্ষিত:
  দ্ৱিগুণ value = 0.;
};

/* byte string */
সংরচনা UnsizedByteStr : UnsizedArrayOf <HBUINT8>
{
  hb_ubytes_t as_ubytes (unsigned l) ধ্রুৱ
  { ঘূৰাই hb_ubytes_t ((ধ্রুৱ unsigned ৱর্ণ *) এতত্, l); }

  // encode 2-byte int (Dict/CharString) or 4-byte int (Dict)
  template <typename T, typename V>
  স্থির তার্কিক serialize_int (hb_serialize_context_t *c, op_code_t intOp, V value)
  {
    TRACE_SERIALIZE (এতত্);

    HBUINT8 *p = c->allocate_size<HBUINT8> (1);
    যদি (unlikely (!p)) return_trace (মিছা);
    *p = intOp;

    T *ip = c->allocate_size<T> (T::static_size);
    যদি (unlikely (!ip)) return_trace (মিছা);
    return_trace (c->check_assign (*ip, value, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  template <typename V>
  স্থির তার্কিক serialize_int4 (hb_serialize_context_t *c, V value)
  { ঘূৰাই serialize_int<HBINT32> (c, OpCode_longintdict, value); }

  template <typename V>
  স্থির তার্কিক serialize_int2 (hb_serialize_context_t *c, V value)
  { ঘূৰাই serialize_int<HBINT16> (c, OpCode_shortint, value); }

  /* Defining null_size allows a Null object may be created. Should be safe because:
   * A descendent struct Dict uses a Null pointer to indicate a missing table,
   * checked before access.
   */
  DEFINE_SIZE_MIN(0);
};

/* A byte string associated with the current offset and an error condition */
সংরচনা byte_str_ref_t
{
  byte_str_ref_t ()
    : str () {}

  byte_str_ref_t (ধ্রুৱ hb_ubytes_t &str_, unsigned পূর্ণাঙ্ক offset_ = 0)
    : str (str_) { set_offset (offset_); }

  শূন্য reset (ধ্রুৱ hb_ubytes_t &str_, unsigned পূর্ণাঙ্ক offset_ = 0)
  {
    str = str_;
    set_offset (offset_);
  }

  ধ্রুৱ unsigned ৱর্ণ& operator [] (পূর্ণাঙ্ক i) {
    যদি (unlikely ((unsigned পূর্ণাঙ্ক) (get_offset () + i) >= str.length))
    {
      set_error ();
      ঘূৰাই Null (unsigned ৱর্ণ);
    }
    ঘূৰাই str.arrayZ[get_offset () + i];
  }

  unsigned ৱর্ণ head_unchecked () ধ্রুৱ { ঘূৰাই str.arrayZ[get_offset ()]; }

  /* Conversion to hb_ubytes_t */
  operator hb_ubytes_t () ধ্রুৱ { ঘূৰাই str.sub_array (get_offset ()); }

  hb_ubytes_t sub_array (unsigned পূর্ণাঙ্ক offset_, unsigned পূর্ণাঙ্ক len_) ধ্রুৱ
  { ঘূৰাই str.sub_array (offset_, len_); }

  তার্কিক avail (unsigned পূর্ণাঙ্ক গুনতি=1) ধ্রুৱ
  { ঘূৰাই get_offset () + গুনতি <= str.length; }
  শূন্য inc (unsigned পূর্ণাঙ্ক গুনতি=1)
  {
    /* Automatically puts us in error if count is out-of-range. */
    set_offset (get_offset () + গুনতি);
  }

  /* We (ab)use ubytes backwards_length as a cursor (called offset),
   * as well as to store error condition. */

  unsigned get_offset () ধ্রুৱ { ঘূৰাই str.backwards_length; }
  শূন্য set_offset (unsigned offset) { str.backwards_length = offset; }

  শূন্য set_error ()      { str.backwards_length = str.length + 1; }
  তার্কিক in_error () ধ্রুৱ { ঘূৰাই str.backwards_length > str.length; }

  unsigned total_size () ধ্রুৱ { ঘূৰাই str.length; }

  রক্ষিত:
  hb_ubytes_t       str;
};

/*স্তূপ*/
template <typename ELEM, পূর্ণাঙ্ক LIMIT>
সংরচনা cff_স্তূপ_t
{
  ELEM& operator [] (unsigned পূর্ণাঙ্ক i)
  {
    যদি (unlikely (i >= length))
    {
      set_error ();
      ঘূৰাই Crap (ELEM);
    }
    ঘূৰাই arrayZ[i];
  }

  শূন্য push (ধ্রুৱ ELEM &v)
  {
    যদি (likely (length < LIMIT))
      arrayZ[length++] = v;
    অন্যথা
      set_error ();
  }
  ELEM &push ()
  {
    যদি (likely (length < LIMIT))
      ঘূৰাই arrayZ[length++];
    অন্যথা
    {
      set_error ();
      ঘূৰাই Crap (ELEM);
    }
  }

  ELEM& বেৰ_কৰো ()
  {
    যদি (likely (length > 0))
      ঘূৰাই arrayZ[--length];
    অন্যথা
    {
      set_error ();
      ঘূৰাই Crap (ELEM);
    }
  }
  শূন্য বেৰ_কৰো (unsigned পূর্ণাঙ্ক n)
  {
    যদি (likely (length >= n))
      length -= n;
    অন্যথা
      set_error ();
  }

  ধ্রুৱ ELEM& peek ()
  {
    যদি (unlikely (length == 0))
    {
      set_error ();
      ঘূৰাই Null (ELEM);
    }
    ঘূৰাই arrayZ[length - 1];
  }

  শূন্য unpop ()
  {
    যদি (likely (length < LIMIT))
      length++;
    অন্যথা
      set_error ();
  }

  শূন্য clear () { length = 0; }

  তার্কিক in_error () ধ্রুৱ { ঘূৰাই (error); }
  শূন্য set_error ()      { error = সঁচা; }

  unsigned পূর্ণাঙ্ক get_গুনতি () ধ্রুৱ { ঘূৰাই length; }
  তার্কিক is_empty () ধ্রুৱ          { ঘূৰাই !length; }

  hb_array_t<ধ্রুৱ ELEM> sub_array (unsigned start, unsigned length) ধ্রুৱ
  { ঘূৰাই hb_array_t<ধ্রুৱ ELEM> (arrayZ).sub_array (start, length); }

  তার্কিক error = মিছা;
  unsigned পূর্ণাঙ্ক length = 0;
  ELEM arrayZ[LIMIT];
};

/* argument stack */
template <typename ARG=সংখ্যা_t>
সংরচনা arg_স্তূপ_t : cff_স্তূপ_t<ARG, 513>
{
  শূন্য ঠেলো_int (পূর্ণাঙ্ক v)
  {
    ARG &n = S::push ();
    n.set_int (v);
  }

  শূন্য ঠেলো_fixed (int32_t v)
  {
    ARG &n = S::push ();
    n.set_fixed (v);
  }

  শূন্য ঠেলো_real (দ্ৱিগুণ v)
  {
    ARG &n = S::push ();
    n.set_real (v);
  }

  ARG& বেৰ_কৰো_অঙ্ক () { ঘূৰাই এতত্->বেৰ_কৰো (); }

  পূর্ণাঙ্ক বেৰ_কৰো_int ()
  {
    পূর্ণাঙ্ক v;
    যদি (unlikely (!এতত্->বেৰ_কৰো ().to_int (&v)))
      S::set_error ();
    ঘূৰাই v;
  }

  unsigned পূর্ণাঙ্ক বেৰ_কৰো_uint ()
  {
    পূর্ণাঙ্ক i = বেৰ_কৰো_int ();
    যদি (unlikely (i < 0))
    {
      i = 0;
      S::set_error ();
    }
    ঘূৰাই (unsigned) i;
  }

  শূন্য ঠেলো_longint_from_substr (byte_str_ref_t& str_ref)
  {
    ঠেলো_int ((পূর্ণাঙ্ক) ((uint32_t (str_ref[0]) << 24) | (str_ref[1] << 16) | (str_ref[2] << 8) | (str_ref[3])));
    str_ref.inc (4);
  }

  তার্কিক ঠেলো_fixed_from_substr (byte_str_ref_t& str_ref)
  {
    যদি (unlikely (!str_ref.avail (4)))
      ঘূৰাই মিছা;
    ঠেলো_fixed ((int32_t)*(ধ্রুৱ HBUINT32*)&str_ref[0]);
    str_ref.inc (4);
    ঘূৰাই সঁচা;
  }

  গুপ্ত:
  প্রকারনির্দেশ cff_স্তূপ_t<ARG, 513> S;
};

/* an operator prefixed by its operands in a byte string */
সংরচনা op_str_t
{
  /* This used to have a hb_ubytes_t. Using a pointer and length
   * in a particular order, saves 8 bytes in this struct and more
   * in our parsed_cs_op_t subclass. */

  ধ্রুৱ unsigned ৱর্ণ *ptr = nullptr;

  op_code_t  op = OpCode_Invalid;

  uint8_t length = 0;
};

/* base of OP_SERIALIZER */
সংরচনা op_serializer_t
{
  রক্ষিত:
  তার্কিক copy_opstr (hb_serialize_context_t *c, ধ্রুৱ op_str_t& opstr) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);

    unsigned ৱর্ণ *d = c->allocate_size<unsigned ৱর্ণ> (opstr.length);
    যদি (unlikely (!d)) return_trace (মিছা);
    /* Faster than hb_memcpy for small strings. */
    বাবে (unsigned i = 0; i < opstr.length; i++)
      d[i] = opstr.ptr[i];
    return_trace (সঁচা);
  }
};

template <typename VAL>
সংরচনা parsed_মানসমূহ_t
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

  শূন্য add_op (op_code_t op, ধ্রুৱ byte_str_ref_t& str_ref = byte_str_ref_t (), ধ্রুৱ VAL &v = VAL ())
  {
    VAL *val = values.push (v);
    val->op = op;
    auto arr = str_ref.sub_array (opStart, str_ref.get_offset () - opStart);
    val->ptr = arr.arrayZ;
    val->length = arr.length;
    opStart = str_ref.get_offset ();
  }

  তার্কিক has_op (op_code_t op) ধ্রুৱ
  {
    বাবে (ধ্রুৱ auto& v : values)
      যদি (v.op == op) ঘূৰাই সঁচা;
    ঘূৰাই মিছা;
  }

  unsigned get_গুনতি () ধ্রুৱ { ঘূৰাই values.length; }
  ধ্রুৱ VAL &operator [] (unsigned পূর্ণাঙ্ক i) ধ্রুৱ { ঘূৰাই values[i]; }

  unsigned পূর্ণাঙ্ক       opStart;
  hb_vector_t<VAL>   values;
};

template <typename ARG=সংখ্যা_t>
সংরচনা interp_env_t
{
  interp_env_t () {}
  interp_env_t (ধ্রুৱ hb_ubytes_t &str_)
  {
    str_ref.reset (str_);
  }
  তার্কিক in_error () ধ্রুৱ
  { ঘূৰাই str_ref.in_error () || arg_স্তূপ.in_error (); }

  শূন্য set_error () { str_ref.set_error (); }

  op_code_t fetch_op ()
  {
    op_code_t  op = OpCode_Invalid;
    যদি (unlikely (!str_ref.avail ()))
      ঘূৰাই OpCode_Invalid;
    op = (op_code_t) str_ref.head_unchecked ();
    str_ref.inc ();
    যদি (op == OpCode_escape) {
      যদি (unlikely (!str_ref.avail ()))
	ঘূৰাই OpCode_Invalid;
      op = Make_OpCode_ESC (str_ref.head_unchecked ());
      str_ref.inc ();
    }
    ঘূৰাই op;
  }

  ধ্রুৱ ARG& eval_arg (unsigned পূর্ণাঙ্ক i) { ঘূৰাই arg_স্তূপ[i]; }

  ARG& বেৰ_কৰো_arg () { ঘূৰাই arg_স্তূপ.বেৰ_কৰো (); }
  শূন্য বেৰ_কৰো_n_args (unsigned পূর্ণাঙ্ক n) { arg_স্তূপ.বেৰ_কৰো (n); }

  শূন্য clear_args () { বেৰ_কৰো_n_args (arg_স্তূপ.get_গুনতি ()); }

  byte_str_ref_t
		str_ref;
  arg_স্তূপ_t<ARG>
		arg_স্তূপ;
};

using অঙ্ক_interp_env_t =  interp_env_t<>;

template <typename ARG=সংখ্যা_t>
সংরচনা opset_t
{
  স্থির শূন্য process_op (op_code_t op, interp_env_t<ARG>& env)
  {
    ৱিকল্প (op) {
      পক্ষ OpCode_shortint:
	env.arg_স্তূপ.ঠেলো_int ((int16_t)((env.str_ref[0] << 8) | env.str_ref[1]));
	env.str_ref.inc (2);
	ভঙ্গ;

      পক্ষ OpCode_TwoBytePosInt0: পক্ষ OpCode_TwoBytePosInt1:
      পক্ষ OpCode_TwoBytePosInt2: পক্ষ OpCode_TwoBytePosInt3:
	env.arg_স্তূপ.ঠেলো_int ((int16_t)((op - OpCode_TwoBytePosInt0) * 256 + env.str_ref[0] + 108));
	env.str_ref.inc ();
	ভঙ্গ;

      পক্ষ OpCode_TwoByteNegInt0: পক্ষ OpCode_TwoByteNegInt1:
      পক্ষ OpCode_TwoByteNegInt2: পক্ষ OpCode_TwoByteNegInt3:
	env.arg_স্তূপ.ঠেলো_int ((-(int16_t)(op - OpCode_TwoByteNegInt0) * 256 - env.str_ref[0] - 108));
	env.str_ref.inc ();
	ভঙ্গ;

      স্ৱভাৱ:
	/* 1-byte integer */
	যদি (likely ((OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast)))
	{
	  env.arg_স্তূপ.ঠেলো_int ((পূর্ণাঙ্ক)op - 139);
	} অন্যথা {
	  /* invalid unknown operator */
	  env.clear_args ();
	}
	ভঙ্গ;
    }
  }
};

template <typename ENV>
সংরচনা interpreter_t
{
  interpreter_t (ENV& env_) : env (env_) {}
  ENV& env;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_COMMON_HH */
