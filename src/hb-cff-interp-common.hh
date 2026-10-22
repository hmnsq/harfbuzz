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

வெளி HB_INTERNAL மாறிலி unsigned எழுத்து *endchar_str;

நாமாகாஶ CFF {

using நாமாகாஶ OT;

வகைவரை unsigned முழு op_code_t;


/* === Dict operators === */

/* One byte operators (0-31) */
#define OpCode_version		  0 /* CFF Top */
#define OpCode_Notice		  1 /* CFF Top */
#define OpCode_FullName		  2 /* CFF Top */
#define OpCode_FamilyName	  3 /* CFF Top */
#define OpCode_Weight		  4 /* CFF Top */
#define OpCode_FontBBox		  5 /* CFF Top */
#define Op_Code_Blue_மதிப்புகள்	  6 /* CFF Private, CFF2 Private */
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

உள்வரி op_code_t Unmake_OpCode_ESC (op_code_t op)  { திரும்பு (op_code_t)(op - OpCode_ESC_Base); }
உள்வரி தருக்க Is_OpCode_ESC (op_code_t op) { திரும்பு op >= OpCode_ESC_Base; }
உள்வரி unsigned முழு OpCode_Size (op_code_t op) { திரும்பு Is_OpCode_ESC (op) ? 2: 1; }

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


கட்டமைப்பு எண்_t
{
  வெறுமை set_int (முழு v)       { value = v; }
  தருக்க to_int (முழு *v) மாறிலி
  {
    எனில் (unlikely (!(value >= INT_MIN && value <= INT_MAX)))
    {
      *v = 0;
      திரும்பு பொய்;
    }
    *v = (முழு) value;
    திரும்பு உண்மை;
  }
  முழு to_int () மாறிலி
  {
    முழு v;
    எனில் (likely (to_int (&v))) திரும்பு v;
    திரும்பு value < 0 ? INT_MIN : value > 0 ? INT_MAX : 0;
  }

  வெறுமை set_fixed (int32_t v) { value = v / 65536.0; }
  int32_t to_fixed () மாறிலி
  { திரும்பு (int32_t) hb_clamp (value * 65536.0, (இரட்டை) INT32_MIN, (இரட்டை) INT32_MAX); }

  வெறுமை set_real (இரட்டை v)   { value = v; }
  இரட்டை to_real () மாறிலி    { திரும்பு value; }

  தருக்க in_int_range () மாறிலி
  { திரும்பு value >= INT16_MIN && value <= INT16_MAX && (இரட்டை) (int16_t) value == value; }

  தருக்க operator >  (மாறிலி எண்_t &n) மாறிலி { திரும்பு value > n.to_real (); }
  தருக்க operator <  (மாறிலி எண்_t &n) மாறிலி { திரும்பு n > *இது; }
  தருக்க operator >= (மாறிலி எண்_t &n) மாறிலி { திரும்பு !(*இது < n); }
  தருக்க operator <= (மாறிலி எண்_t &n) மாறிலி { திரும்பு !(*இது > n); }

  மாறிலி எண்_t &operator += (மாறிலி எண்_t &n)
  {
    set_real (to_real () + n.to_real ());

    திரும்பு *இது;
  }

  பாதுகாக்கப்பட்ட:
  இரட்டை value = 0.;
};

/* byte string */
கட்டமைப்பு UnsizedByteStr : UnsizedArrayOf <HBUINT8>
{
  hb_ubytes_t as_ubytes (unsigned l) மாறிலி
  { திரும்பு hb_ubytes_t ((மாறிலி unsigned எழுத்து *) இது, l); }

  // encode 2-byte int (Dict/CharString) or 4-byte int (Dict)
  template <typename T, typename V>
  நிலையான தருக்க serialize_int (hb_serialize_context_t *c, op_code_t intOp, V value)
  {
    TRACE_SERIALIZE (இது);

    HBUINT8 *p = c->allocate_size<HBUINT8> (1);
    எனில் (unlikely (!p)) return_trace (பொய்);
    *p = intOp;

    T *ip = c->allocate_size<T> (T::static_size);
    எனில் (unlikely (!ip)) return_trace (பொய்);
    return_trace (c->check_assign (*ip, value, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  template <typename V>
  நிலையான தருக்க serialize_int4 (hb_serialize_context_t *c, V value)
  { திரும்பு serialize_int<HBINT32> (c, OpCode_longintdict, value); }

  template <typename V>
  நிலையான தருக்க serialize_int2 (hb_serialize_context_t *c, V value)
  { திரும்பு serialize_int<HBINT16> (c, OpCode_shortint, value); }

  /* Defining null_size allows a Null object may be created. Should be safe because:
   * A descendent struct Dict uses a Null pointer to indicate a missing table,
   * checked before access.
   */
  DEFINE_SIZE_MIN(0);
};

/* A byte string associated with the current offset and an error condition */
கட்டமைப்பு byte_str_ref_t
{
  byte_str_ref_t ()
    : str () {}

  byte_str_ref_t (மாறிலி hb_ubytes_t &str_, unsigned முழு offset_ = 0)
    : str (str_) { set_offset (offset_); }

  வெறுமை reset (மாறிலி hb_ubytes_t &str_, unsigned முழு offset_ = 0)
  {
    str = str_;
    set_offset (offset_);
  }

  மாறிலி unsigned எழுத்து& operator [] (முழு i) {
    எனில் (unlikely ((unsigned முழு) (get_offset () + i) >= str.length))
    {
      set_error ();
      திரும்பு Null (unsigned எழுத்து);
    }
    திரும்பு str.arrayZ[get_offset () + i];
  }

  unsigned எழுத்து head_unchecked () மாறிலி { திரும்பு str.arrayZ[get_offset ()]; }

  /* Conversion to hb_ubytes_t */
  operator hb_ubytes_t () மாறிலி { திரும்பு str.sub_array (get_offset ()); }

  hb_ubytes_t sub_array (unsigned முழு offset_, unsigned முழு len_) மாறிலி
  { திரும்பு str.sub_array (offset_, len_); }

  தருக்க avail (unsigned முழு மொத்தம்=1) மாறிலி
  { திரும்பு get_offset () + மொத்தம் <= str.length; }
  வெறுமை inc (unsigned முழு மொத்தம்=1)
  {
    /* Automatically puts us in error if count is out-of-range. */
    set_offset (get_offset () + மொத்தம்);
  }

  /* We (ab)use ubytes backwards_length as a cursor (called offset),
   * as well as to store error condition. */

  unsigned get_offset () மாறிலி { திரும்பு str.backwards_length; }
  வெறுமை set_offset (unsigned offset) { str.backwards_length = offset; }

  வெறுமை set_error ()      { str.backwards_length = str.length + 1; }
  தருக்க in_error () மாறிலி { திரும்பு str.backwards_length > str.length; }

  unsigned total_size () மாறிலி { திரும்பு str.length; }

  பாதுகாக்கப்பட்ட:
  hb_ubytes_t       str;
};

/*அடுக்கு*/
template <typename ELEM, முழு LIMIT>
கட்டமைப்பு cff_அடுக்கு_t
{
  ELEM& operator [] (unsigned முழு i)
  {
    எனில் (unlikely (i >= length))
    {
      set_error ();
      திரும்பு Crap (ELEM);
    }
    திரும்பு arrayZ[i];
  }

  வெறுமை push (மாறிலி ELEM &v)
  {
    எனில் (likely (length < LIMIT))
      arrayZ[length++] = v;
    இல்லையெனில்
      set_error ();
  }
  ELEM &push ()
  {
    எனில் (likely (length < LIMIT))
      திரும்பு arrayZ[length++];
    இல்லையெனில்
    {
      set_error ();
      திரும்பு Crap (ELEM);
    }
  }

  ELEM& எடு ()
  {
    எனில் (likely (length > 0))
      திரும்பு arrayZ[--length];
    இல்லையெனில்
    {
      set_error ();
      திரும்பு Crap (ELEM);
    }
  }
  வெறுமை எடு (unsigned முழு n)
  {
    எனில் (likely (length >= n))
      length -= n;
    இல்லையெனில்
      set_error ();
  }

  மாறிலி ELEM& peek ()
  {
    எனில் (unlikely (length == 0))
    {
      set_error ();
      திரும்பு Null (ELEM);
    }
    திரும்பு arrayZ[length - 1];
  }

  வெறுமை unpop ()
  {
    எனில் (likely (length < LIMIT))
      length++;
    இல்லையெனில்
      set_error ();
  }

  வெறுமை clear () { length = 0; }

  தருக்க in_error () மாறிலி { திரும்பு (error); }
  வெறுமை set_error ()      { error = உண்மை; }

  unsigned முழு get_மொத்தம் () மாறிலி { திரும்பு length; }
  தருக்க is_empty () மாறிலி          { திரும்பு !length; }

  hb_array_t<மாறிலி ELEM> sub_array (unsigned start, unsigned length) மாறிலி
  { திரும்பு hb_array_t<மாறிலி ELEM> (arrayZ).sub_array (start, length); }

  தருக்க error = பொய்;
  unsigned முழு length = 0;
  ELEM arrayZ[LIMIT];
};

/* argument stack */
template <typename ARG=எண்_t>
கட்டமைப்பு arg_அடுக்கு_t : cff_அடுக்கு_t<ARG, 513>
{
  வெறுமை தள்ளு_int (முழு v)
  {
    ARG &n = S::push ();
    n.set_int (v);
  }

  வெறுமை தள்ளு_fixed (int32_t v)
  {
    ARG &n = S::push ();
    n.set_fixed (v);
  }

  வெறுமை தள்ளு_real (இரட்டை v)
  {
    ARG &n = S::push ();
    n.set_real (v);
  }

  ARG& எடு_இலக்கம் () { திரும்பு இது->எடு (); }

  முழு எடு_int ()
  {
    முழு v;
    எனில் (unlikely (!இது->எடு ().to_int (&v)))
      S::set_error ();
    திரும்பு v;
  }

  unsigned முழு எடு_uint ()
  {
    முழு i = எடு_int ();
    எனில் (unlikely (i < 0))
    {
      i = 0;
      S::set_error ();
    }
    திரும்பு (unsigned) i;
  }

  வெறுமை தள்ளு_longint_from_substr (byte_str_ref_t& str_ref)
  {
    தள்ளு_int ((முழு) ((uint32_t (str_ref[0]) << 24) | (str_ref[1] << 16) | (str_ref[2] << 8) | (str_ref[3])));
    str_ref.inc (4);
  }

  தருக்க தள்ளு_fixed_from_substr (byte_str_ref_t& str_ref)
  {
    எனில் (unlikely (!str_ref.avail (4)))
      திரும்பு பொய்;
    தள்ளு_fixed ((int32_t)*(மாறிலி HBUINT32*)&str_ref[0]);
    str_ref.inc (4);
    திரும்பு உண்மை;
  }

  தனிப்பட்ட:
  வகைவரை cff_அடுக்கு_t<ARG, 513> S;
};

/* an operator prefixed by its operands in a byte string */
கட்டமைப்பு op_str_t
{
  /* This used to have a hb_ubytes_t. Using a pointer and length
   * in a particular order, saves 8 bytes in this struct and more
   * in our parsed_cs_op_t subclass. */

  மாறிலி unsigned எழுத்து *ptr = nullptr;

  op_code_t  op = OpCode_Invalid;

  uint8_t length = 0;
};

/* base of OP_SERIALIZER */
கட்டமைப்பு op_serializer_t
{
  பாதுகாக்கப்பட்ட:
  தருக்க copy_opstr (hb_serialize_context_t *c, மாறிலி op_str_t& opstr) மாறிலி
  {
    TRACE_SERIALIZE (இது);

    unsigned எழுத்து *d = c->allocate_size<unsigned எழுத்து> (opstr.length);
    எனில் (unlikely (!d)) return_trace (பொய்);
    /* Faster than hb_memcpy for small strings. */
    க்கு (unsigned i = 0; i < opstr.length; i++)
      d[i] = opstr.ptr[i];
    return_trace (உண்மை);
  }
};

template <typename VAL>
கட்டமைப்பு parsed_மதிப்புகள்_t
{
  வெறுமை init ()
  {
    opStart = 0;
    values.init ();
  }
  வெறுமை fini () { values.fini (); }

  வெறுமை alloc (unsigned n)
  {
    values.alloc_exact (n);
  }

  வெறுமை add_op (op_code_t op, மாறிலி byte_str_ref_t& str_ref = byte_str_ref_t (), மாறிலி VAL &v = VAL ())
  {
    VAL *val = values.push (v);
    val->op = op;
    auto arr = str_ref.sub_array (opStart, str_ref.get_offset () - opStart);
    val->ptr = arr.arrayZ;
    val->length = arr.length;
    opStart = str_ref.get_offset ();
  }

  தருக்க has_op (op_code_t op) மாறிலி
  {
    க்கு (மாறிலி auto& v : values)
      எனில் (v.op == op) திரும்பு உண்மை;
    திரும்பு பொய்;
  }

  unsigned get_மொத்தம் () மாறிலி { திரும்பு values.length; }
  மாறிலி VAL &operator [] (unsigned முழு i) மாறிலி { திரும்பு values[i]; }

  unsigned முழு       opStart;
  hb_vector_t<VAL>   values;
};

template <typename ARG=எண்_t>
கட்டமைப்பு interp_env_t
{
  interp_env_t () {}
  interp_env_t (மாறிலி hb_ubytes_t &str_)
  {
    str_ref.reset (str_);
  }
  தருக்க in_error () மாறிலி
  { திரும்பு str_ref.in_error () || arg_அடுக்கு.in_error (); }

  வெறுமை set_error () { str_ref.set_error (); }

  op_code_t fetch_op ()
  {
    op_code_t  op = OpCode_Invalid;
    எனில் (unlikely (!str_ref.avail ()))
      திரும்பு OpCode_Invalid;
    op = (op_code_t) str_ref.head_unchecked ();
    str_ref.inc ();
    எனில் (op == OpCode_escape) {
      எனில் (unlikely (!str_ref.avail ()))
	திரும்பு OpCode_Invalid;
      op = Make_OpCode_ESC (str_ref.head_unchecked ());
      str_ref.inc ();
    }
    திரும்பு op;
  }

  மாறிலி ARG& eval_arg (unsigned முழு i) { திரும்பு arg_அடுக்கு[i]; }

  ARG& எடு_arg () { திரும்பு arg_அடுக்கு.எடு (); }
  வெறுமை எடு_n_args (unsigned முழு n) { arg_அடுக்கு.எடு (n); }

  வெறுமை clear_args () { எடு_n_args (arg_அடுக்கு.get_மொத்தம் ()); }

  byte_str_ref_t
		str_ref;
  arg_அடுக்கு_t<ARG>
		arg_அடுக்கு;
};

using இலக்கம்_interp_env_t =  interp_env_t<>;

template <typename ARG=எண்_t>
கட்டமைப்பு opset_t
{
  நிலையான வெறுமை process_op (op_code_t op, interp_env_t<ARG>& env)
  {
    தேர்வு (op) {
      நிலை OpCode_shortint:
	env.arg_அடுக்கு.தள்ளு_int ((int16_t)((env.str_ref[0] << 8) | env.str_ref[1]));
	env.str_ref.inc (2);
	நிறுத்து;

      நிலை OpCode_TwoBytePosInt0: நிலை OpCode_TwoBytePosInt1:
      நிலை OpCode_TwoBytePosInt2: நிலை OpCode_TwoBytePosInt3:
	env.arg_அடுக்கு.தள்ளு_int ((int16_t)((op - OpCode_TwoBytePosInt0) * 256 + env.str_ref[0] + 108));
	env.str_ref.inc ();
	நிறுத்து;

      நிலை OpCode_TwoByteNegInt0: நிலை OpCode_TwoByteNegInt1:
      நிலை OpCode_TwoByteNegInt2: நிலை OpCode_TwoByteNegInt3:
	env.arg_அடுக்கு.தள்ளு_int ((-(int16_t)(op - OpCode_TwoByteNegInt0) * 256 - env.str_ref[0] - 108));
	env.str_ref.inc ();
	நிறுத்து;

      இயல்பு:
	/* 1-byte integer */
	எனில் (likely ((OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast)))
	{
	  env.arg_அடுக்கு.தள்ளு_int ((முழு)op - 139);
	} இல்லையெனில் {
	  /* invalid unknown operator */
	  env.clear_args ();
	}
	நிறுத்து;
    }
  }
};

template <typename ENV>
கட்டமைப்பு interpreter_t
{
  interpreter_t (ENV& env_) : env (env_) {}
  ENV& env;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_COMMON_HH */
