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

ବାହ୍ୟ HB_INTERNAL ଅଚଳ unsigned ଅକ୍ଷର *endchar_str;

ନାମାକାଶ CFF {

using ନାମାକାଶ OT;

ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା op_code_t;


/* === Dict operators === */

/* One byte operators (0-31) */
#define OpCode_version		  0 /* CFF Top */
#define OpCode_Notice		  1 /* CFF Top */
#define OpCode_FullName		  2 /* CFF Top */
#define OpCode_FamilyName	  3 /* CFF Top */
#define OpCode_Weight		  4 /* CFF Top */
#define OpCode_FontBBox		  5 /* CFF Top */
#define Op_Code_Blue_ମୂଲ୍ୟଗୁଡ଼ିକ	  6 /* CFF Private, CFF2 Private */
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

ଧାଡ଼ିଭିତରେ op_code_t Unmake_OpCode_ESC (op_code_t op)  { ଫେରନ୍ତୁ (op_code_t)(op - OpCode_ESC_Base); }
ଧାଡ଼ିଭିତରେ ତାର୍କିକ Is_OpCode_ESC (op_code_t op) { ଫେରନ୍ତୁ op >= OpCode_ESC_Base; }
ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା OpCode_Size (op_code_t op) { ଫେରନ୍ତୁ Is_OpCode_ESC (op) ? 2: 1; }

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


ସଂରଚନା ସଂଖ୍ୟା_t
{
  ଶୂନ୍ୟ set_int (ପୂର୍ଣ୍ଣସଂଖ୍ୟା v)       { value = v; }
  ତାର୍କିକ to_int (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *v) ଅଚଳ
  {
    ଯଦି (unlikely (!(value >= INT_MIN && value <= INT_MAX)))
    {
      *v = 0;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    *v = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) value;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା to_int () ଅଚଳ
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା v;
    ଯଦି (likely (to_int (&v))) ଫେରନ୍ତୁ v;
    ଫେରନ୍ତୁ value < 0 ? INT_MIN : value > 0 ? INT_MAX : 0;
  }

  ଶୂନ୍ୟ set_fixed (int32_t v) { value = v / 65536.0; }
  int32_t to_fixed () ଅଚଳ
  { ଫେରନ୍ତୁ (int32_t) hb_clamp (value * 65536.0, (ଦ୍ୱିଗୁଣ) INT32_MIN, (ଦ୍ୱିଗୁଣ) INT32_MAX); }

  ଶୂନ୍ୟ set_real (ଦ୍ୱିଗୁଣ v)   { value = v; }
  ଦ୍ୱିଗୁଣ to_real () ଅଚଳ    { ଫେରନ୍ତୁ value; }

  ତାର୍କିକ in_int_range () ଅଚଳ
  { ଫେରନ୍ତୁ value >= INT16_MIN && value <= INT16_MAX && (ଦ୍ୱିଗୁଣ) (int16_t) value == value; }

  ତାର୍କିକ operator >  (ଅଚଳ ସଂଖ୍ୟା_t &n) ଅଚଳ { ଫେରନ୍ତୁ value > n.to_real (); }
  ତାର୍କିକ operator <  (ଅଚଳ ସଂଖ୍ୟା_t &n) ଅଚଳ { ଫେରନ୍ତୁ n > *ଏହି; }
  ତାର୍କିକ operator >= (ଅଚଳ ସଂଖ୍ୟା_t &n) ଅଚଳ { ଫେରନ୍ତୁ !(*ଏହି < n); }
  ତାର୍କିକ operator <= (ଅଚଳ ସଂଖ୍ୟା_t &n) ଅଚଳ { ଫେରନ୍ତୁ !(*ଏହି > n); }

  ଅଚଳ ସଂଖ୍ୟା_t &operator += (ଅଚଳ ସଂଖ୍ୟା_t &n)
  {
    set_real (to_real () + n.to_real ());

    ଫେରନ୍ତୁ *ଏହି;
  }

  ସୁରକ୍ଷିତ:
  ଦ୍ୱିଗୁଣ value = 0.;
};

/* byte string */
ସଂରଚନା UnsizedByteStr : UnsizedArrayOf <HBUINT8>
{
  hb_ubytes_t as_ubytes (unsigned l) ଅଚଳ
  { ଫେରନ୍ତୁ hb_ubytes_t ((ଅଚଳ unsigned ଅକ୍ଷର *) ଏହି, l); }

  // encode 2-byte int (Dict/CharString) or 4-byte int (Dict)
  template <typename T, typename V>
  ସ୍ଥିର ତାର୍କିକ serialize_int (hb_serialize_context_t *c, op_code_t intOp, V value)
  {
    TRACE_SERIALIZE (ଏହି);

    HBUINT8 *p = c->allocate_size<HBUINT8> (1);
    ଯଦି (unlikely (!p)) return_trace (ମିଥ୍ୟା);
    *p = intOp;

    T *ip = c->allocate_size<T> (T::static_size);
    ଯଦି (unlikely (!ip)) return_trace (ମିଥ୍ୟା);
    return_trace (c->check_assign (*ip, value, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  template <typename V>
  ସ୍ଥିର ତାର୍କିକ serialize_int4 (hb_serialize_context_t *c, V value)
  { ଫେରନ୍ତୁ serialize_int<HBINT32> (c, OpCode_longintdict, value); }

  template <typename V>
  ସ୍ଥିର ତାର୍କିକ serialize_int2 (hb_serialize_context_t *c, V value)
  { ଫେରନ୍ତୁ serialize_int<HBINT16> (c, OpCode_shortint, value); }

  /* Defining null_size allows a Null object may be created. Should be safe because:
   * A descendent struct Dict uses a Null pointer to indicate a missing table,
   * checked before access.
   */
  DEFINE_SIZE_MIN(0);
};

/* A byte string associated with the current offset and an error condition */
ସଂରଚନା byte_str_ref_t
{
  byte_str_ref_t ()
    : str () {}

  byte_str_ref_t (ଅଚଳ hb_ubytes_t &str_, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset_ = 0)
    : str (str_) { set_offset (offset_); }

  ଶୂନ୍ୟ reset (ଅଚଳ hb_ubytes_t &str_, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset_ = 0)
  {
    str = str_;
    set_offset (offset_);
  }

  ଅଚଳ unsigned ଅକ୍ଷର& operator [] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) {
    ଯଦି (unlikely ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (get_offset () + i) >= str.length))
    {
      set_error ();
      ଫେରନ୍ତୁ Null (unsigned ଅକ୍ଷର);
    }
    ଫେରନ୍ତୁ str.arrayZ[get_offset () + i];
  }

  unsigned ଅକ୍ଷର head_unchecked () ଅଚଳ { ଫେରନ୍ତୁ str.arrayZ[get_offset ()]; }

  /* Conversion to hb_ubytes_t */
  operator hb_ubytes_t () ଅଚଳ { ଫେରନ୍ତୁ str.sub_array (get_offset ()); }

  hb_ubytes_t sub_array (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset_, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len_) ଅଚଳ
  { ଫେରନ୍ତୁ str.sub_array (offset_, len_); }

  ତାର୍କିକ avail (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2=1) ଅଚଳ
  { ଫେରନ୍ତୁ get_offset () + ଗଣନା_2 <= str.length; }
  ଶୂନ୍ୟ inc (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2=1)
  {
    /* Automatically puts us in error if count is out-of-range. */
    set_offset (get_offset () + ଗଣନା_2);
  }

  /* We (ab)use ubytes backwards_length as a cursor (called offset),
   * as well as to store error condition. */

  unsigned get_offset () ଅଚଳ { ଫେରନ୍ତୁ str.backwards_length; }
  ଶୂନ୍ୟ set_offset (unsigned offset) { str.backwards_length = offset; }

  ଶୂନ୍ୟ set_error ()      { str.backwards_length = str.length + 1; }
  ତାର୍କିକ in_error () ଅଚଳ { ଫେରନ୍ତୁ str.backwards_length > str.length; }

  unsigned total_size () ଅଚଳ { ଫେରନ୍ତୁ str.length; }

  ସୁରକ୍ଷିତ:
  hb_ubytes_t       str;
};

/*ଗଦା*/
template <typename ELEM, ପୂର୍ଣ୍ଣସଂଖ୍ୟା LIMIT>
ସଂରଚନା cff_ଗଦା_t
{
  ELEM& operator [] (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
  {
    ଯଦି (unlikely (i >= length))
    {
      set_error ();
      ଫେରନ୍ତୁ Crap (ELEM);
    }
    ଫେରନ୍ତୁ arrayZ[i];
  }

  ଶୂନ୍ୟ push (ଅଚଳ ELEM &v)
  {
    ଯଦି (likely (length < LIMIT))
      arrayZ[length++] = v;
    ଅନ୍ୟଥା
      set_error ();
  }
  ELEM &push ()
  {
    ଯଦି (likely (length < LIMIT))
      ଫେରନ୍ତୁ arrayZ[length++];
    ଅନ୍ୟଥା
    {
      set_error ();
      ଫେରନ୍ତୁ Crap (ELEM);
    }
  }

  ELEM& କାଢ଼ ()
  {
    ଯଦି (likely (length > 0))
      ଫେରନ୍ତୁ arrayZ[--length];
    ଅନ୍ୟଥା
    {
      set_error ();
      ଫେରନ୍ତୁ Crap (ELEM);
    }
  }
  ଶୂନ୍ୟ କାଢ଼ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
  {
    ଯଦି (likely (length >= n))
      length -= n;
    ଅନ୍ୟଥା
      set_error ();
  }

  ଅଚଳ ELEM& peek ()
  {
    ଯଦି (unlikely (length == 0))
    {
      set_error ();
      ଫେରନ୍ତୁ Null (ELEM);
    }
    ଫେରନ୍ତୁ arrayZ[length - 1];
  }

  ଶୂନ୍ୟ unpop ()
  {
    ଯଦି (likely (length < LIMIT))
      length++;
    ଅନ୍ୟଥା
      set_error ();
  }

  ଶୂନ୍ୟ clear () { length = 0; }

  ତାର୍କିକ in_error () ଅଚଳ { ଫେରନ୍ତୁ (error); }
  ଶୂନ୍ୟ set_error ()      { error = ସତ୍ୟ; }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ length; }
  ତାର୍କିକ is_empty () ଅଚଳ          { ଫେରନ୍ତୁ !length; }

  hb_array_t<ଅଚଳ ELEM> sub_array (unsigned start, unsigned length) ଅଚଳ
  { ଫେରନ୍ତୁ hb_array_t<ଅଚଳ ELEM> (arrayZ).sub_array (start, length); }

  ତାର୍କିକ error = ମିଥ୍ୟା;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length = 0;
  ELEM arrayZ[LIMIT];
};

/* argument stack */
template <typename ARG=ସଂଖ୍ୟା_t>
ସଂରଚନା arg_ଗଦା_t : cff_ଗଦା_t<ARG, 513>
{
  ଶୂନ୍ୟ ଠେଲ_int (ପୂର୍ଣ୍ଣସଂଖ୍ୟା v)
  {
    ARG &n = S::push ();
    n.set_int (v);
  }

  ଶୂନ୍ୟ ଠେଲ_fixed (int32_t v)
  {
    ARG &n = S::push ();
    n.set_fixed (v);
  }

  ଶୂନ୍ୟ ଠେଲ_real (ଦ୍ୱିଗୁଣ v)
  {
    ARG &n = S::push ();
    n.set_real (v);
  }

  ARG& କାଢ଼_ଅଙ୍କ () { ଫେରନ୍ତୁ ଏହି->କାଢ଼ (); }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା କାଢ଼_int ()
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା v;
    ଯଦି (unlikely (!ଏହି->କାଢ଼ ().to_int (&v)))
      S::set_error ();
    ଫେରନ୍ତୁ v;
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା କାଢ଼_uint ()
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = କାଢ଼_int ();
    ଯଦି (unlikely (i < 0))
    {
      i = 0;
      S::set_error ();
    }
    ଫେରନ୍ତୁ (unsigned) i;
  }

  ଶୂନ୍ୟ ଠେଲ_longint_from_substr (byte_str_ref_t& str_ref)
  {
    ଠେଲ_int ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ((uint32_t (str_ref[0]) << 24) | (str_ref[1] << 16) | (str_ref[2] << 8) | (str_ref[3])));
    str_ref.inc (4);
  }

  ତାର୍କିକ ଠେଲ_fixed_from_substr (byte_str_ref_t& str_ref)
  {
    ଯଦି (unlikely (!str_ref.avail (4)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଠେଲ_fixed ((int32_t)*(ଅଚଳ HBUINT32*)&str_ref[0]);
    str_ref.inc (4);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ବ୍ୟକ୍ତିଗତ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା cff_ଗଦା_t<ARG, 513> S;
};

/* an operator prefixed by its operands in a byte string */
ସଂରଚନା op_str_t
{
  /* This used to have a hb_ubytes_t. Using a pointer and length
   * in a particular order, saves 8 bytes in this struct and more
   * in our parsed_cs_op_t subclass. */

  ଅଚଳ unsigned ଅକ୍ଷର *ptr = nullptr;

  op_code_t  op = OpCode_Invalid;

  uint8_t length = 0;
};

/* base of OP_SERIALIZER */
ସଂରଚନା op_serializer_t
{
  ସୁରକ୍ଷିତ:
  ତାର୍କିକ copy_opstr (hb_serialize_context_t *c, ଅଚଳ op_str_t& opstr) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);

    unsigned ଅକ୍ଷର *d = c->allocate_size<unsigned ଅକ୍ଷର> (opstr.length);
    ଯଦି (unlikely (!d)) return_trace (ମିଥ୍ୟା);
    /* Faster than hb_memcpy for small strings. */
    ପାଇଁ (unsigned i = 0; i < opstr.length; i++)
      d[i] = opstr.ptr[i];
    return_trace (ସତ୍ୟ);
  }
};

template <typename VAL>
ସଂରଚନା parsed_ମୂଲ୍ୟଗୁଡ଼ିକ_t
{
  ଶୂନ୍ୟ init ()
  {
    opStart = 0;
    values.init ();
  }
  ଶୂନ୍ୟ fini () { values.fini (); }

  ଶୂନ୍ୟ alloc (unsigned n)
  {
    values.alloc_exact (n);
  }

  ଶୂନ୍ୟ add_op (op_code_t op, ଅଚଳ byte_str_ref_t& str_ref = byte_str_ref_t (), ଅଚଳ VAL &v = VAL ())
  {
    VAL *val = values.push (v);
    val->op = op;
    auto arr = str_ref.sub_array (opStart, str_ref.get_offset () - opStart);
    val->ptr = arr.arrayZ;
    val->length = arr.length;
    opStart = str_ref.get_offset ();
  }

  ତାର୍କିକ has_op (op_code_t op) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ auto& v : values)
      ଯଦି (v.op == op) ଫେରନ୍ତୁ ସତ୍ୟ;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  unsigned get_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ values.length; }
  ଅଚଳ VAL &operator [] (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ { ଫେରନ୍ତୁ values[i]; }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା       opStart;
  hb_vector_t<VAL>   values;
};

template <typename ARG=ସଂଖ୍ୟା_t>
ସଂରଚନା interp_env_t
{
  interp_env_t () {}
  interp_env_t (ଅଚଳ hb_ubytes_t &str_)
  {
    str_ref.reset (str_);
  }
  ତାର୍କିକ in_error () ଅଚଳ
  { ଫେରନ୍ତୁ str_ref.in_error () || arg_ଗଦା.in_error (); }

  ଶୂନ୍ୟ set_error () { str_ref.set_error (); }

  op_code_t fetch_op ()
  {
    op_code_t  op = OpCode_Invalid;
    ଯଦି (unlikely (!str_ref.avail ()))
      ଫେରନ୍ତୁ OpCode_Invalid;
    op = (op_code_t) str_ref.head_unchecked ();
    str_ref.inc ();
    ଯଦି (op == OpCode_escape) {
      ଯଦି (unlikely (!str_ref.avail ()))
	ଫେରନ୍ତୁ OpCode_Invalid;
      op = Make_OpCode_ESC (str_ref.head_unchecked ());
      str_ref.inc ();
    }
    ଫେରନ୍ତୁ op;
  }

  ଅଚଳ ARG& eval_arg (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) { ଫେରନ୍ତୁ arg_ଗଦା[i]; }

  ARG& କାଢ଼_arg () { ଫେରନ୍ତୁ arg_ଗଦା.କାଢ଼ (); }
  ଶୂନ୍ୟ କାଢ଼_n_args (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) { arg_ଗଦା.କାଢ଼ (n); }

  ଶୂନ୍ୟ clear_args () { କାଢ଼_n_args (arg_ଗଦା.get_ଗଣନା ()); }

  byte_str_ref_t
		str_ref;
  arg_ଗଦା_t<ARG>
		arg_ଗଦା;
};

using ଅଙ୍କ_interp_env_t =  interp_env_t<>;

template <typename ARG=ସଂଖ୍ୟା_t>
ସଂରଚନା opset_t
{
  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, interp_env_t<ARG>& env)
  {
    ବାଛ (op) {
      ସ୍ଥିତି OpCode_shortint:
	env.arg_ଗଦା.ଠେଲ_int ((int16_t)((env.str_ref[0] << 8) | env.str_ref[1]));
	env.str_ref.inc (2);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_TwoBytePosInt0: ସ୍ଥିତି OpCode_TwoBytePosInt1:
      ସ୍ଥିତି OpCode_TwoBytePosInt2: ସ୍ଥିତି OpCode_TwoBytePosInt3:
	env.arg_ଗଦା.ଠେଲ_int ((int16_t)((op - OpCode_TwoBytePosInt0) * 256 + env.str_ref[0] + 108));
	env.str_ref.inc ();
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_TwoByteNegInt0: ସ୍ଥିତି OpCode_TwoByteNegInt1:
      ସ୍ଥିତି OpCode_TwoByteNegInt2: ସ୍ଥିତି OpCode_TwoByteNegInt3:
	env.arg_ଗଦା.ଠେଲ_int ((-(int16_t)(op - OpCode_TwoByteNegInt0) * 256 - env.str_ref[0] - 108));
	env.str_ref.inc ();
	ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	/* 1-byte integer */
	ଯଦି (likely ((OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast)))
	{
	  env.arg_ଗଦା.ଠେଲ_int ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)op - 139);
	} ଅନ୍ୟଥା {
	  /* invalid unknown operator */
	  env.clear_args ();
	}
	ଭାଙ୍ଗ;
    }
  }
};

template <typename ENV>
ସଂରଚନା interpreter_t
{
  interpreter_t (ENV& env_) : env (env_) {}
  ENV& env;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_COMMON_HH */
