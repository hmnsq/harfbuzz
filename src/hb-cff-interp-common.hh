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

ബാഹ്യ HB_INTERNAL സ്ഥിരം unsigned അക്ഷരം *endchar_str;

നാമാകാശ CFF {

using നാമാകാശ OT;

തരനിർവചനം unsigned പൂർണ്ണസംഖ്യ op_code_t;


/* === Dict operators === */

/* One byte operators (0-31) */
#define OpCode_version		  0 /* CFF Top */
#define OpCode_Notice		  1 /* CFF Top */
#define OpCode_FullName		  2 /* CFF Top */
#define OpCode_FamilyName	  3 /* CFF Top */
#define OpCode_Weight		  4 /* CFF Top */
#define OpCode_FontBBox		  5 /* CFF Top */
#define Op_Code_Blue_മൂല്യങ്ങൾ	  6 /* CFF Private, CFF2 Private */
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

വരിയിൽ op_code_t Unmake_OpCode_ESC (op_code_t op)  { തിരികെ (op_code_t)(op - OpCode_ESC_Base); }
വരിയിൽ തർക്കം Is_OpCode_ESC (op_code_t op) { തിരികെ op >= OpCode_ESC_Base; }
വരിയിൽ unsigned പൂർണ്ണസംഖ്യ OpCode_Size (op_code_t op) { തിരികെ Is_OpCode_ESC (op) ? 2: 1; }

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


ഘടന സംഖ്യ_t
{
  ശൂന്യം set_int (പൂർണ്ണസംഖ്യ v)       { value = v; }
  തർക്കം to_int (പൂർണ്ണസംഖ്യ *v) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!(value >= INT_MIN && value <= INT_MAX)))
    {
      *v = 0;
      തിരികെ അസത്യം;
    }
    *v = (പൂർണ്ണസംഖ്യ) value;
    തിരികെ സത്യം;
  }
  പൂർണ്ണസംഖ്യ to_int () സ്ഥിരം
  {
    പൂർണ്ണസംഖ്യ v;
    എങ്കിൽ (likely (to_int (&v))) തിരികെ v;
    തിരികെ value < 0 ? INT_MIN : value > 0 ? INT_MAX : 0;
  }

  ശൂന്യം set_fixed (int32_t v) { value = v / 65536.0; }
  int32_t to_fixed () സ്ഥിരം
  { തിരികെ (int32_t) hb_clamp (value * 65536.0, (ഇരട്ട) INT32_MIN, (ഇരട്ട) INT32_MAX); }

  ശൂന്യം set_real (ഇരട്ട v)   { value = v; }
  ഇരട്ട to_real () സ്ഥിരം    { തിരികെ value; }

  തർക്കം in_int_range () സ്ഥിരം
  { തിരികെ value >= INT16_MIN && value <= INT16_MAX && (ഇരട്ട) (int16_t) value == value; }

  തർക്കം operator >  (സ്ഥിരം സംഖ്യ_t &n) സ്ഥിരം { തിരികെ value > n.to_real (); }
  തർക്കം operator <  (സ്ഥിരം സംഖ്യ_t &n) സ്ഥിരം { തിരികെ n > *ഇത്; }
  തർക്കം operator >= (സ്ഥിരം സംഖ്യ_t &n) സ്ഥിരം { തിരികെ !(*ഇത് < n); }
  തർക്കം operator <= (സ്ഥിരം സംഖ്യ_t &n) സ്ഥിരം { തിരികെ !(*ഇത് > n); }

  സ്ഥിരം സംഖ്യ_t &operator += (സ്ഥിരം സംഖ്യ_t &n)
  {
    set_real (to_real () + n.to_real ());

    തിരികെ *ഇത്;
  }

  സംരക്ഷിത:
  ഇരട്ട value = 0.;
};

/* byte string */
ഘടന UnsizedByteStr : UnsizedArrayOf <HBUINT8>
{
  hb_ubytes_t as_ubytes (unsigned l) സ്ഥിരം
  { തിരികെ hb_ubytes_t ((സ്ഥിരം unsigned അക്ഷരം *) ഇത്, l); }

  // encode 2-byte int (Dict/CharString) or 4-byte int (Dict)
  template <typename T, typename V>
  സ്ഥിതം തർക്കം serialize_int (hb_serialize_context_t *c, op_code_t intOp, V value)
  {
    TRACE_SERIALIZE (ഇത്);

    HBUINT8 *p = c->allocate_size<HBUINT8> (1);
    എങ്കിൽ (unlikely (!p)) return_trace (അസത്യം);
    *p = intOp;

    T *ip = c->allocate_size<T> (T::static_size);
    എങ്കിൽ (unlikely (!ip)) return_trace (അസത്യം);
    return_trace (c->check_assign (*ip, value, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  template <typename V>
  സ്ഥിതം തർക്കം serialize_int4 (hb_serialize_context_t *c, V value)
  { തിരികെ serialize_int<HBINT32> (c, OpCode_longintdict, value); }

  template <typename V>
  സ്ഥിതം തർക്കം serialize_int2 (hb_serialize_context_t *c, V value)
  { തിരികെ serialize_int<HBINT16> (c, OpCode_shortint, value); }

  /* Defining null_size allows a Null object may be created. Should be safe because:
   * A descendent struct Dict uses a Null pointer to indicate a missing table,
   * checked before access.
   */
  DEFINE_SIZE_MIN(0);
};

/* A byte string associated with the current offset and an error condition */
ഘടന byte_str_ref_t
{
  byte_str_ref_t ()
    : str () {}

  byte_str_ref_t (സ്ഥിരം hb_ubytes_t &str_, unsigned പൂർണ്ണസംഖ്യ offset_ = 0)
    : str (str_) { set_offset (offset_); }

  ശൂന്യം reset (സ്ഥിരം hb_ubytes_t &str_, unsigned പൂർണ്ണസംഖ്യ offset_ = 0)
  {
    str = str_;
    set_offset (offset_);
  }

  സ്ഥിരം unsigned അക്ഷരം& operator [] (പൂർണ്ണസംഖ്യ i) {
    എങ്കിൽ (unlikely ((unsigned പൂർണ്ണസംഖ്യ) (get_offset () + i) >= str.length))
    {
      set_error ();
      തിരികെ Null (unsigned അക്ഷരം);
    }
    തിരികെ str.arrayZ[get_offset () + i];
  }

  unsigned അക്ഷരം head_unchecked () സ്ഥിരം { തിരികെ str.arrayZ[get_offset ()]; }

  /* Conversion to hb_ubytes_t */
  operator hb_ubytes_t () സ്ഥിരം { തിരികെ str.sub_array (get_offset ()); }

  hb_ubytes_t sub_array (unsigned പൂർണ്ണസംഖ്യ offset_, unsigned പൂർണ്ണസംഖ്യ len_) സ്ഥിരം
  { തിരികെ str.sub_array (offset_, len_); }

  തർക്കം avail (unsigned പൂർണ്ണസംഖ്യ എണ്ണം=1) സ്ഥിരം
  { തിരികെ get_offset () + എണ്ണം <= str.length; }
  ശൂന്യം inc (unsigned പൂർണ്ണസംഖ്യ എണ്ണം=1)
  {
    /* Automatically puts us in error if count is out-of-range. */
    set_offset (get_offset () + എണ്ണം);
  }

  /* We (ab)use ubytes backwards_length as a cursor (called offset),
   * as well as to store error condition. */

  unsigned get_offset () സ്ഥിരം { തിരികെ str.backwards_length; }
  ശൂന്യം set_offset (unsigned offset) { str.backwards_length = offset; }

  ശൂന്യം set_error ()      { str.backwards_length = str.length + 1; }
  തർക്കം in_error () സ്ഥിരം { തിരികെ str.backwards_length > str.length; }

  unsigned total_size () സ്ഥിരം { തിരികെ str.length; }

  സംരക്ഷിത:
  hb_ubytes_t       str;
};

/*അട്ടി*/
template <typename ELEM, പൂർണ്ണസംഖ്യ LIMIT>
ഘടന cff_അട്ടി_t
{
  ELEM& operator [] (unsigned പൂർണ്ണസംഖ്യ i)
  {
    എങ്കിൽ (unlikely (i >= length))
    {
      set_error ();
      തിരികെ Crap (ELEM);
    }
    തിരികെ arrayZ[i];
  }

  ശൂന്യം push (സ്ഥിരം ELEM &v)
  {
    എങ്കിൽ (likely (length < LIMIT))
      arrayZ[length++] = v;
    അല്ലെങ്കിൽ
      set_error ();
  }
  ELEM &push ()
  {
    എങ്കിൽ (likely (length < LIMIT))
      തിരികെ arrayZ[length++];
    അല്ലെങ്കിൽ
    {
      set_error ();
      തിരികെ Crap (ELEM);
    }
  }

  ELEM& എടുക്കുക ()
  {
    എങ്കിൽ (likely (length > 0))
      തിരികെ arrayZ[--length];
    അല്ലെങ്കിൽ
    {
      set_error ();
      തിരികെ Crap (ELEM);
    }
  }
  ശൂന്യം എടുക്കുക (unsigned പൂർണ്ണസംഖ്യ n)
  {
    എങ്കിൽ (likely (length >= n))
      length -= n;
    അല്ലെങ്കിൽ
      set_error ();
  }

  സ്ഥിരം ELEM& peek ()
  {
    എങ്കിൽ (unlikely (length == 0))
    {
      set_error ();
      തിരികെ Null (ELEM);
    }
    തിരികെ arrayZ[length - 1];
  }

  ശൂന്യം unpop ()
  {
    എങ്കിൽ (likely (length < LIMIT))
      length++;
    അല്ലെങ്കിൽ
      set_error ();
  }

  ശൂന്യം clear () { length = 0; }

  തർക്കം in_error () സ്ഥിരം { തിരികെ (error); }
  ശൂന്യം set_error ()      { error = സത്യം; }

  unsigned പൂർണ്ണസംഖ്യ get_എണ്ണം () സ്ഥിരം { തിരികെ length; }
  തർക്കം is_empty () സ്ഥിരം          { തിരികെ !length; }

  hb_array_t<സ്ഥിരം ELEM> sub_array (unsigned start, unsigned length) സ്ഥിരം
  { തിരികെ hb_array_t<സ്ഥിരം ELEM> (arrayZ).sub_array (start, length); }

  തർക്കം error = അസത്യം;
  unsigned പൂർണ്ണസംഖ്യ length = 0;
  ELEM arrayZ[LIMIT];
};

/* argument stack */
template <typename ARG=സംഖ്യ_t>
ഘടന arg_അട്ടി_t : cff_അട്ടി_t<ARG, 513>
{
  ശൂന്യം തള്ളുക_int (പൂർണ്ണസംഖ്യ v)
  {
    ARG &n = S::push ();
    n.set_int (v);
  }

  ശൂന്യം തള്ളുക_fixed (int32_t v)
  {
    ARG &n = S::push ();
    n.set_fixed (v);
  }

  ശൂന്യം തള്ളുക_real (ഇരട്ട v)
  {
    ARG &n = S::push ();
    n.set_real (v);
  }

  ARG& എടുക്കുക_അക്കം () { തിരികെ ഇത്->എടുക്കുക (); }

  പൂർണ്ണസംഖ്യ എടുക്കുക_int ()
  {
    പൂർണ്ണസംഖ്യ v;
    എങ്കിൽ (unlikely (!ഇത്->എടുക്കുക ().to_int (&v)))
      S::set_error ();
    തിരികെ v;
  }

  unsigned പൂർണ്ണസംഖ്യ എടുക്കുക_uint ()
  {
    പൂർണ്ണസംഖ്യ i = എടുക്കുക_int ();
    എങ്കിൽ (unlikely (i < 0))
    {
      i = 0;
      S::set_error ();
    }
    തിരികെ (unsigned) i;
  }

  ശൂന്യം തള്ളുക_longint_from_substr (byte_str_ref_t& str_ref)
  {
    തള്ളുക_int ((പൂർണ്ണസംഖ്യ) ((uint32_t (str_ref[0]) << 24) | (str_ref[1] << 16) | (str_ref[2] << 8) | (str_ref[3])));
    str_ref.inc (4);
  }

  തർക്കം തള്ളുക_fixed_from_substr (byte_str_ref_t& str_ref)
  {
    എങ്കിൽ (unlikely (!str_ref.avail (4)))
      തിരികെ അസത്യം;
    തള്ളുക_fixed ((int32_t)*(സ്ഥിരം HBUINT32*)&str_ref[0]);
    str_ref.inc (4);
    തിരികെ സത്യം;
  }

  സ്വകാര്യ:
  തരനിർവചനം cff_അട്ടി_t<ARG, 513> S;
};

/* an operator prefixed by its operands in a byte string */
ഘടന op_str_t
{
  /* This used to have a hb_ubytes_t. Using a pointer and length
   * in a particular order, saves 8 bytes in this struct and more
   * in our parsed_cs_op_t subclass. */

  സ്ഥിരം unsigned അക്ഷരം *ptr = nullptr;

  op_code_t  op = OpCode_Invalid;

  uint8_t length = 0;
};

/* base of OP_SERIALIZER */
ഘടന op_serializer_t
{
  സംരക്ഷിത:
  തർക്കം copy_opstr (hb_serialize_context_t *c, സ്ഥിരം op_str_t& opstr) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);

    unsigned അക്ഷരം *d = c->allocate_size<unsigned അക്ഷരം> (opstr.length);
    എങ്കിൽ (unlikely (!d)) return_trace (അസത്യം);
    /* Faster than hb_memcpy for small strings. */
    വേണ്ടി (unsigned i = 0; i < opstr.length; i++)
      d[i] = opstr.ptr[i];
    return_trace (സത്യം);
  }
};

template <typename VAL>
ഘടന parsed_മൂല്യങ്ങൾ_t
{
  ശൂന്യം init ()
  {
    opStart = 0;
    values.init ();
  }
  ശൂന്യം fini () { values.fini (); }

  ശൂന്യം alloc (unsigned n)
  {
    values.alloc_exact (n);
  }

  ശൂന്യം add_op (op_code_t op, സ്ഥിരം byte_str_ref_t& str_ref = byte_str_ref_t (), സ്ഥിരം VAL &v = VAL ())
  {
    VAL *val = values.push (v);
    val->op = op;
    auto arr = str_ref.sub_array (opStart, str_ref.get_offset () - opStart);
    val->ptr = arr.arrayZ;
    val->length = arr.length;
    opStart = str_ref.get_offset ();
  }

  തർക്കം has_op (op_code_t op) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം auto& v : values)
      എങ്കിൽ (v.op == op) തിരികെ സത്യം;
    തിരികെ അസത്യം;
  }

  unsigned get_എണ്ണം () സ്ഥിരം { തിരികെ values.length; }
  സ്ഥിരം VAL &operator [] (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം { തിരികെ values[i]; }

  unsigned പൂർണ്ണസംഖ്യ       opStart;
  hb_vector_t<VAL>   values;
};

template <typename ARG=സംഖ്യ_t>
ഘടന interp_env_t
{
  interp_env_t () {}
  interp_env_t (സ്ഥിരം hb_ubytes_t &str_)
  {
    str_ref.reset (str_);
  }
  തർക്കം in_error () സ്ഥിരം
  { തിരികെ str_ref.in_error () || arg_അട്ടി.in_error (); }

  ശൂന്യം set_error () { str_ref.set_error (); }

  op_code_t fetch_op ()
  {
    op_code_t  op = OpCode_Invalid;
    എങ്കിൽ (unlikely (!str_ref.avail ()))
      തിരികെ OpCode_Invalid;
    op = (op_code_t) str_ref.head_unchecked ();
    str_ref.inc ();
    എങ്കിൽ (op == OpCode_escape) {
      എങ്കിൽ (unlikely (!str_ref.avail ()))
	തിരികെ OpCode_Invalid;
      op = Make_OpCode_ESC (str_ref.head_unchecked ());
      str_ref.inc ();
    }
    തിരികെ op;
  }

  സ്ഥിരം ARG& eval_arg (unsigned പൂർണ്ണസംഖ്യ i) { തിരികെ arg_അട്ടി[i]; }

  ARG& എടുക്കുക_arg () { തിരികെ arg_അട്ടി.എടുക്കുക (); }
  ശൂന്യം എടുക്കുക_n_args (unsigned പൂർണ്ണസംഖ്യ n) { arg_അട്ടി.എടുക്കുക (n); }

  ശൂന്യം clear_args () { എടുക്കുക_n_args (arg_അട്ടി.get_എണ്ണം ()); }

  byte_str_ref_t
		str_ref;
  arg_അട്ടി_t<ARG>
		arg_അട്ടി;
};

using അക്കം_interp_env_t =  interp_env_t<>;

template <typename ARG=സംഖ്യ_t>
ഘടന opset_t
{
  സ്ഥിതം ശൂന്യം process_op (op_code_t op, interp_env_t<ARG>& env)
  {
    തിരഞ്ഞെടുപ്പ് (op) {
      അവസ്ഥ OpCode_shortint:
	env.arg_അട്ടി.തള്ളുക_int ((int16_t)((env.str_ref[0] << 8) | env.str_ref[1]));
	env.str_ref.inc (2);
	നിർത്തുക;

      അവസ്ഥ OpCode_TwoBytePosInt0: അവസ്ഥ OpCode_TwoBytePosInt1:
      അവസ്ഥ OpCode_TwoBytePosInt2: അവസ്ഥ OpCode_TwoBytePosInt3:
	env.arg_അട്ടി.തള്ളുക_int ((int16_t)((op - OpCode_TwoBytePosInt0) * 256 + env.str_ref[0] + 108));
	env.str_ref.inc ();
	നിർത്തുക;

      അവസ്ഥ OpCode_TwoByteNegInt0: അവസ്ഥ OpCode_TwoByteNegInt1:
      അവസ്ഥ OpCode_TwoByteNegInt2: അവസ്ഥ OpCode_TwoByteNegInt3:
	env.arg_അട്ടി.തള്ളുക_int ((-(int16_t)(op - OpCode_TwoByteNegInt0) * 256 - env.str_ref[0] - 108));
	env.str_ref.inc ();
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
	/* 1-byte integer */
	എങ്കിൽ (likely ((OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast)))
	{
	  env.arg_അട്ടി.തള്ളുക_int ((പൂർണ്ണസംഖ്യ)op - 139);
	} അല്ലെങ്കിൽ {
	  /* invalid unknown operator */
	  env.clear_args ();
	}
	നിർത്തുക;
    }
  }
};

template <typename ENV>
ഘടന interpreter_t
{
  interpreter_t (ENV& env_) : env (env_) {}
  ENV& env;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_COMMON_HH */
