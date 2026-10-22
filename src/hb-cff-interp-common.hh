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
#నిర్వచితం_కాకపోతే HB_CFF_INTERP_COMMON_HH
#నిర్వచించు HB_CFF_INTERP_COMMON_HH

బాహ్య HB_అంతర్గత స్థిరరాశి గుర్తులేని అక్షరం *endchar_str;

నామాకాశ CFF {

ఉపయోగించి నామాకాశ OT;

రకనిర్వచనం గుర్తులేని పూర్ణాంకం op_సంకేతం_t;


/* === Dict operators === */

/* One byte operators (0-31) */
#నిర్వచించు Op_సంకేతం_సంస్కరణ		  0 /* CFF Top */
#నిర్వచించు Op_సంకేతం_Notice		  1 /* CFF Top */
#నిర్వచించు Op_సంకేతం_నిండిన_Name		  2 /* CFF Top */
#నిర్వచించు Op_సంకేతం_Family_Name	  3 /* CFF Top */
#నిర్వచించు Op_సంకేతం_బరువు		  4 /* CFF Top */
#నిర్వచించు Op_సంకేతం_Font_BBox		  5 /* CFF Top */
#నిర్వచించు Op_సంకేతం_Blue_విలువలు	  6 /* CFF Private, CFF2 Private */
#నిర్వచించు Op_సంకేతం_Other_Blues	  7 /* CFF Private, CFF2 Private */
#నిర్వచించు Op_సంకేతం_Family_Blues	  8 /* CFF Private, CFF2 Private */
#నిర్వచించు Op_సంకేతం_Family_Other_Blues	  9 /* CFF Private, CFF2 Private */
#నిర్వచించు Op_సంకేతం_Std_HW		 10 /* CFF Private, CFF2 Private */
#నిర్వచించు Op_సంకేతం_Std_VW		 11 /* CFF Private, CFF2 Private */
#నిర్వచించు Op_సంకేతం_escape		 12 /* All. Shared with CS */
#నిర్వచించు Op_సంకేతం_Unique_ID		 13 /* CFF Top */
#నిర్వచించు Op_సంకేతం_XUID		 14 /* CFF Top */
#నిర్వచించు Op_సంకేతం_charset		 15 /* CFF Top (0) */
#నిర్వచించు Op_సంకేతం_Encoding		 16 /* CFF Top (0) */
#నిర్వచించు Op_సంకేతం_Char_అక్షరశ్రేణులు	 17 /* CFF Top, CFF2 Top */
#నిర్వచించు Op_సంకేతం_వ్యక్తిగత		 18 /* CFF Top, CFF2 FD */
#నిర్వచించు Op_సంకేతం_Subrs		 19 /* CFF Private, CFF2 Private */
#నిర్వచించు Op_సంకేతం_default_వెడల్పు_X	 20 /* CFF Private (0) */
#నిర్వచించు Op_సంకేతం_nominal_వెడల్పు_X	 21 /* CFF Private (0) */
#నిర్వచించు Op_సంకేతం_vsindexdict	 22 /* CFF2 Private/CS */
#నిర్వచించు Op_సంకేతం_blenddict	 23 /* CFF2 Private/CS */
#నిర్వచించు Op_సంకేతం_vstore		 24 /* CFF2 Top */
#నిర్వచించు Op_సంకేతం_reserved25	 25
#నిర్వచించు Op_సంకేతం_reserved26	 26
#నిర్వచించు Op_సంకేతం_reserved27	 27

/* Numbers */
#నిర్వచించు Op_సంకేతం_shortint		 28 /* 16-bit integer, All */
#నిర్వచించు Op_సంకేతం_longintdict	 29 /* 32-bit integer, All */
#నిర్వచించు Op_సంకేతం_BCD		 30 /* Real number, CFF2 Top/FD */
#నిర్వచించు Op_సంకేతం_reserved31	 31

/* 1-byte integers */
#నిర్వచించు Op_సంకేతం_One_Byte_Int_మొదటి	 32 /* All. beginning of the range of first byte ints */
#నిర్వచించు Op_సంకేతం_One_Byte_Int_చివరి	246 /* All. ending of the range of first byte int */

/* 2-byte integers */
#నిర్వచించు Op_సంకేతం_Two_Byte_Pos_Int0	247 /* All. first byte of two byte positive int (+108 to +1131) */
#నిర్వచించు Op_సంకేతం_Two_Byte_Pos_Int1	248
#నిర్వచించు Op_సంకేతం_Two_Byte_Pos_Int2	249
#నిర్వచించు Op_సంకేతం_Two_Byte_Pos_Int3	250

#నిర్వచించు Op_సంకేతం_Two_Byte_Neg_Int0	251 /* All. first byte of two byte negative int (-1131 to -108) */
#నిర్వచించు Op_సంకేతం_Two_Byte_Neg_Int1	252
#నిర్వచించు Op_సంకేతం_Two_Byte_Neg_Int2	253
#నిర్వచించు Op_సంకేతం_Two_Byte_Neg_Int3	254

/* Two byte escape operators 12, (0-41) */
#నిర్వచించు Op_సంకేతం_ESC_పునాది		256
#నిర్వచించు Make_Op_సంకేతం_ESC(byte2)	((op_సంకేతం_t)(Op_సంకేతం_ESC_పునాది + (byte2)))

అంతఃవరుస op_సంకేతం_t Unmake_Op_సంకేతం_ESC (op_సంకేతం_t op)  { తిరిగి (op_సంకేతం_t)(op - Op_సంకేతం_ESC_పునాది); }
అంతఃవరుస తార్కిక Is_Op_సంకేతం_ESC (op_సంకేతం_t op) { తిరిగి op >= Op_సంకేతం_ESC_పునాది; }
అంతఃవరుస గుర్తులేని పూర్ణాంకం Op_సంకేతం_కొలత (op_సంకేతం_t op) { తిరిగి Is_Op_సంకేతం_ESC (op) ? 2: 1; }

#నిర్వచించు Op_సంకేతం_Copyright	Make_Op_సంకేతం_ESC(0) /* CFF Top */
#నిర్వచించు Op_సంకేతం_is_Fixed_Pitch	Make_Op_సంకేతం_ESC(1) /* CFF Top (false) */
#నిర్వచించు Op_సంకేతం_Italic_Angle	Make_Op_సంకేతం_ESC(2) /* CFF Top (0) */
#నిర్వచించు Op_సంకేతం_Underline_Position Make_Op_సంకేతం_ESC(3) /* CFF Top (-100) */
#నిర్వచించు Op_సంకేతం_Underline_Thickness Make_Op_సంకేతం_ESC(4) /* CFF Top (50) */
#నిర్వచించు Op_సంకేతం_Paint_Type	Make_Op_సంకేతం_ESC(5) /* CFF Top (0) */
#నిర్వచించు Op_సంకేతం_Charstring_Type	Make_Op_సంకేతం_ESC(6) /* CFF Top (2) */
#నిర్వచించు Op_సంకేతం_Font_మాతృక	Make_Op_సంకేతం_ESC(7) /* CFF Top, CFF2 Top (.001 0 0 .001 0 0)*/
#నిర్వచించు Op_సంకేతం_Stroke_వెడల్పు	Make_Op_సంకేతం_ESC(8) /* CFF Top (0) */
#నిర్వచించు Op_సంకేతం_Blue_Scale	Make_Op_సంకేతం_ESC(9) /* CFF Private, CFF2 Private (0.039625) */
#నిర్వచించు Op_సంకేతం_Blue_జరుపు	Make_Op_సంకేతం_ESC(10) /* CFF Private, CFF2 Private (7) */
#నిర్వచించు Op_సంకేతం_Blue_Fuzz		Make_Op_సంకేతం_ESC(11) /* CFF Private, CFF2 Private (1) */
#నిర్వచించు Op_సంకేతం_Stem_Snap_H	Make_Op_సంకేతం_ESC(12) /* CFF Private, CFF2 Private */
#నిర్వచించు Op_సంకేతం_Stem_Snap_V	Make_Op_సంకేతం_ESC(13) /* CFF Private, CFF2 Private */
#నిర్వచించు Op_సంకేతం_Force_Bold	Make_Op_సంకేతం_ESC(14) /* CFF Private (false) */
#నిర్వచించు Op_సంకేతం_reserved_ESC15	Make_Op_సంకేతం_ESC(15)
#నిర్వచించు Op_సంకేతం_reserved_ESC16	Make_Op_సంకేతం_ESC(16)
#నిర్వచించు Op_సంకేతం_Language_సమూహం	Make_Op_సంకేతం_ESC(17) /* CFF Private, CFF2 Private (0) */
#నిర్వచించు Op_సంకేతం_Expansion_గుణకం	Make_Op_సంకేతం_ESC(18) /* CFF Private, CFF2 Private (0.06) */
#నిర్వచించు Op_సంకేతం_initial_Random_విత్తనం Make_Op_సంకేతం_ESC(19) /* CFF Private (0) */
#నిర్వచించు Op_సంకేతం_Synthetic_పునాది	Make_Op_సంకేతం_ESC(20) /* CFF Top */
#నిర్వచించు Op_సంకేతం_Post_Script	Make_Op_సంకేతం_ESC(21) /* CFF Top */
#నిర్వచించు Op_సంకేతం_పునాది_Font_Name	Make_Op_సంకేతం_ESC(22) /* CFF Top */
#నిర్వచించు Op_సంకేతం_పునాది_Font_Blend	Make_Op_సంకేతం_ESC(23) /* CFF Top */
#నిర్వచించు Op_సంకేతం_reserved_ESC24	Make_Op_సంకేతం_ESC(24)
#నిర్వచించు Op_సంకేతం_reserved_ESC25	Make_Op_సంకేతం_ESC(25)
#నిర్వచించు Op_సంకేతం_reserved_ESC26	Make_Op_సంకేతం_ESC(26)
#నిర్వచించు Op_సంకేతం_reserved_ESC27	Make_Op_సంకేతం_ESC(27)
#నిర్వచించు Op_సంకేతం_reserved_ESC28	Make_Op_సంకేతం_ESC(28)
#నిర్వచించు Op_సంకేతం_reserved_ESC29	Make_Op_సంకేతం_ESC(29)
#నిర్వచించు Op_సంకేతం_ROS		Make_Op_సంకేతం_ESC(30) /* CFF Top_CID */
#నిర్వచించు Op_సంకేతం_CIDFont_సంస్కరణ	Make_Op_సంకేతం_ESC(31) /* CFF Top_CID (0) */
#నిర్వచించు Op_సంకేతం_CIDFont_Revision	Make_Op_సంకేతం_ESC(32) /* CFF Top_CID (0) */
#నిర్వచించు Op_సంకేతం_CIDFont_Type	Make_Op_సంకేతం_ESC(33) /* CFF Top_CID (0) */
#నిర్వచించు Op_సంకేతం_CIDCount		Make_Op_సంకేతం_ESC(34) /* CFF Top_CID (8720) */
#నిర్వచించు Op_సంకేతం_UIDBase		Make_Op_సంకేతం_ESC(35) /* CFF Top_CID */
#నిర్వచించు Op_సంకేతం_FDArray		Make_Op_సంకేతం_ESC(36) /* CFF Top_CID, CFF2 Top */
#నిర్వచించు Op_సంకేతం_FDSelect		Make_Op_సంకేతం_ESC(37) /* CFF Top_CID, CFF2 Top */
#నిర్వచించు Op_సంకేతం_Font_Name		Make_Op_సంకేతం_ESC(38) /* CFF Top_CID */


/* === CharString operators === */

#నిర్వచించు Op_సంకేతం_hstem		  1 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_Reserved2	  2
#నిర్వచించు Op_సంకేతం_vstem		  3 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_vmoveto		  4 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_rlineto		  5 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_hlineto		  6 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_vlineto		  7 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_rrcurveto	  8 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_Reserved9	  9
#నిర్వచించు Op_సంకేతం_callsubr		 10 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_return		 11 /* CFF */
//#define OpCode_escape		 12 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_Reserved13	 13
#నిర్వచించు Op_సంకేతం_endchar		 14 /* CFF */
#నిర్వచించు Op_సంకేతం_vsindexcs	 15 /* CFF2 */
#నిర్వచించు Op_సంకేతం_blendcs		 16 /* CFF2 */
#నిర్వచించు Op_సంకేతం_Reserved17	 17
#నిర్వచించు Op_సంకేతం_hstemhm		 18 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_hintmask		 19 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_cntrmask		 20 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_rmoveto		 21 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_hmoveto		 22 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_vstemhm		 23 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_rcurveline	 24 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_rlinecurve	 25 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_vvcurveto	 26 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_hhcurveto	 27 /* CFF, CFF2 */
//#define OpCode_shortint	 28 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_callgsubr	 29 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_vhcurveto	 30 /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_hvcurveto	 31 /* CFF, CFF2 */

#నిర్వచించు Op_సంకేతం_fixedcs		255 /* 32-bit fixed */

/* Two byte escape operators 12, (0-41) */
#నిర్వచించు Op_సంకేతం_dotsection	Make_Op_సంకేతం_ESC(0) /* CFF (obsoleted) */
#నిర్వచించు Op_సంకేతం_Reserved_ESC1	Make_Op_సంకేతం_ESC(1)
#నిర్వచించు Op_సంకేతం_Reserved_ESC2	Make_Op_సంకేతం_ESC(2)
#నిర్వచించు Op_సంకేతం_and		Make_Op_సంకేతం_ESC(3) /* CFF */
#నిర్వచించు Op_సంకేతం_or		Make_Op_సంకేతం_ESC(4) /* CFF */
#నిర్వచించు Op_సంకేతం_not		Make_Op_సంకేతం_ESC(5) /* CFF */
#నిర్వచించు Op_సంకేతం_Reserved_ESC6	Make_Op_సంకేతం_ESC(6)
#నిర్వచించు Op_సంకేతం_Reserved_ESC7	Make_Op_సంకేతం_ESC(7)
#నిర్వచించు Op_సంకేతం_Reserved_ESC8	Make_Op_సంకేతం_ESC(8)
#నిర్వచించు Op_సంకేతం_abs		Make_Op_సంకేతం_ESC(9) /* CFF */
#నిర్వచించు Op_సంకేతం_కూడు		Make_Op_సంకేతం_ESC(10) /* CFF */
#నిర్వచించు Op_సంకేతం_sub		Make_Op_సంకేతం_ESC(11) /* CFF */
#నిర్వచించు Op_సంకేతం_div		Make_Op_సంకేతం_ESC(12) /* CFF */
#నిర్వచించు Op_సంకేతం_Reserved_ESC13	Make_Op_సంకేతం_ESC(13)
#నిర్వచించు Op_సంకేతం_neg		Make_Op_సంకేతం_ESC(14) /* CFF */
#నిర్వచించు Op_సంకేతం_eq		Make_Op_సంకేతం_ESC(15) /* CFF */
#నిర్వచించు Op_సంకేతం_Reserved_ESC16	Make_Op_సంకేతం_ESC(16)
#నిర్వచించు Op_సంకేతం_Reserved_ESC17	Make_Op_సంకేతం_ESC(17)
#నిర్వచించు Op_సంకేతం_drop		Make_Op_సంకేతం_ESC(18) /* CFF */
#నిర్వచించు Op_సంకేతం_Reserved_ESC19	Make_Op_సంకేతం_ESC(19)
#నిర్వచించు Op_సంకేతం_put		Make_Op_సంకేతం_ESC(20) /* CFF */
#నిర్వచించు Op_సంకేతం_get		Make_Op_సంకేతం_ESC(21) /* CFF */
#నిర్వచించు Op_సంకేతం_ifelse		Make_Op_సంకేతం_ESC(22) /* CFF */
#నిర్వచించు Op_సంకేతం_random		Make_Op_సంకేతం_ESC(23) /* CFF */
#నిర్వచించు Op_సంకేతం_mul		Make_Op_సంకేతం_ESC(24) /* CFF */
//#define OpCode_reservedESC25	Make_OpCode_ESC(25)
#నిర్వచించు Op_సంకేతం_sqrt		Make_Op_సంకేతం_ESC(26) /* CFF */
#నిర్వచించు Op_సంకేతం_dup		Make_Op_సంకేతం_ESC(27) /* CFF */
#నిర్వచించు Op_సంకేతం_exch		Make_Op_సంకేతం_ESC(28) /* CFF */
#నిర్వచించు Op_సంకేతం_సూచీ		Make_Op_సంకేతం_ESC(29) /* CFF */
#నిర్వచించు Op_సంకేతం_roll		Make_Op_సంకేతం_ESC(30) /* CFF */
#నిర్వచించు Op_సంకేతం_reserved_ESC31	Make_Op_సంకేతం_ESC(31)
#నిర్వచించు Op_సంకేతం_reserved_ESC32	Make_Op_సంకేతం_ESC(32)
#నిర్వచించు Op_సంకేతం_reserved_ESC33	Make_Op_సంకేతం_ESC(33)
#నిర్వచించు Op_సంకేతం_hflex		Make_Op_సంకేతం_ESC(34) /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_flex		Make_Op_సంకేతం_ESC(35) /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_hflex1		Make_Op_సంకేతం_ESC(36) /* CFF, CFF2 */
#నిర్వచించు Op_సంకేతం_flex1		Make_Op_సంకేతం_ESC(37) /* CFF, CFF2 */


#నిర్వచించు Op_సంకేతం_Invalid		0xFFFFu


నిర్మాణం సంఖ్య_t
{
  శూన్య సమితి_int (పూర్ణాంకం v)       { value = v; }
  తార్కిక గమ్యం_int (పూర్ణాంకం *v) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!(value >= INT_MIN && value <= గరిష్ఠ_పూర్ణాంకం)))
    {
      *v = 0;
      తిరిగి అబద్ధం;
    }
    *v = (పూర్ణాంకం) value;
    తిరిగి నిజం;
  }
  పూర్ణాంకం గమ్యం_int () స్థిరరాశి
  {
    పూర్ణాంకం v;
    ఒకవేళ (likely (గమ్యం_int (&v))) తిరిగి v;
    తిరిగి value < 0 ? INT_MIN : value > 0 ? గరిష్ఠ_పూర్ణాంకం : 0;
  }

  శూన్య సమితి_fixed (int32_t v) { value = v / 65536.0; }
  int32_t గమ్యం_fixed () స్థిరరాశి
  { తిరిగి (int32_t) hb_clamp (value * 65536.0, (ద్విగుణ) INT32_MIN, (ద్విగుణ) INT32_MAX); }

  శూన్య సమితి_real (ద్విగుణ v)   { value = v; }
  ద్విగుణ గమ్యం_real () స్థిరరాశి    { తిరిగి value; }

  తార్కిక లోపల_int_range () స్థిరరాశి
  { తిరిగి value >= INT16_MIN && value <= INT16_MAX && (ద్విగుణ) (int16_t) value == value; }

  తార్కిక క్రియాకారకం >  (స్థిరరాశి సంఖ్య_t &n) స్థిరరాశి { తిరిగి value > n.గమ్యం_real (); }
  తార్కిక క్రియాకారకం <  (స్థిరరాశి సంఖ్య_t &n) స్థిరరాశి { తిరిగి n > *ఇది; }
  తార్కిక క్రియాకారకం >= (స్థిరరాశి సంఖ్య_t &n) స్థిరరాశి { తిరిగి !(*ఇది < n); }
  తార్కిక క్రియాకారకం <= (స్థిరరాశి సంఖ్య_t &n) స్థిరరాశి { తిరిగి !(*ఇది > n); }

  స్థిరరాశి సంఖ్య_t &క్రియాకారకం += (స్థిరరాశి సంఖ్య_t &n)
  {
    సమితి_real (గమ్యం_real () + n.గమ్యం_real ());

    తిరిగి *ఇది;
  }

  రక్షిత:
  ద్విగుణ value = 0.;
};

/* byte string */
నిర్మాణం UnsizedByteStr : Unsized_Array_యొక్క <HBUINT8>
{
  hb_ubytes_t as_ubytes (గుర్తులేని l) స్థిరరాశి
  { తిరిగి hb_ubytes_t ((స్థిరరాశి గుర్తులేని అక్షరం *) ఇది, l); }

  // encode 2-byte int (Dict/CharString) or 4-byte int (Dict)
  మూస <రకం_పేరు T, రకం_పేరు V>
  స్థిర తార్కిక serialize_int (hb_serialize_context_t *c, op_సంకేతం_t intOp, V value)
  {
    TRACE_SERIALIZE (ఇది);

    HBUINT8 *p = c->allocate_కొలత<HBUINT8> (1);
    ఒకవేళ (unlikely (!p)) return_trace (అబద్ధం);
    *p = intOp;

    T *ip = c->allocate_కొలత<T> (T::static_కొలత);
    ఒకవేళ (unlikely (!ip)) return_trace (అబద్ధం);
    return_trace (c->check_కేటాయించు (*ip, value, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  మూస <రకం_పేరు V>
  స్థిర తార్కిక serialize_int4 (hb_serialize_context_t *c, V value)
  { తిరిగి serialize_int<HBINT32> (c, Op_సంకేతం_longintdict, value); }

  మూస <రకం_పేరు V>
  స్థిర తార్కిక serialize_int2 (hb_serialize_context_t *c, V value)
  { తిరిగి serialize_int<HBINT16> (c, Op_సంకేతం_shortint, value); }

  /* Defining null_size allows a Null object may be created. Should be safe because:
   * A descendent struct Dict uses a Null pointer to indicate a missing table,
   * checked before access.
   */
  DEFINE_కొలత_MIN(0);
};

/* A byte string associated with the current offset and an error condition */
నిర్మాణం byte_str_ref_t
{
  byte_str_ref_t ()
    : str () {}

  byte_str_ref_t (స్థిరరాశి hb_ubytes_t &str_, గుర్తులేని పూర్ణాంకం స్థానభ్రంశం_3 = 0)
    : str (str_) { సమితి_స్థానభ్రంశం (స్థానభ్రంశం_3); }

  శూన్య reset (స్థిరరాశి hb_ubytes_t &str_, గుర్తులేని పూర్ణాంకం స్థానభ్రంశం_3 = 0)
  {
    str = str_;
    సమితి_స్థానభ్రంశం (స్థానభ్రంశం_3);
  }

  స్థిరరాశి గుర్తులేని అక్షరం& క్రియాకారకం [] (పూర్ణాంకం i) {
    ఒకవేళ (unlikely ((గుర్తులేని పూర్ణాంకం) (get_స్థానభ్రంశం () + i) >= str.నిడివి))
    {
      సమితి_error ();
      తిరిగి Null (గుర్తులేని అక్షరం);
    }
    తిరిగి str.arrayZ[get_స్థానభ్రంశం () + i];
  }

  గుర్తులేని అక్షరం తల_unchecked () స్థిరరాశి { తిరిగి str.arrayZ[get_స్థానభ్రంశం ()]; }

  /* Conversion to hb_ubytes_t */
  క్రియాకారకం hb_ubytes_t () స్థిరరాశి { తిరిగి str.sub_array (get_స్థానభ్రంశం ()); }

  hb_ubytes_t sub_array (గుర్తులేని పూర్ణాంకం స్థానభ్రంశం_3, గుర్తులేని పూర్ణాంకం len_) స్థిరరాశి
  { తిరిగి str.sub_array (స్థానభ్రంశం_3, len_); }

  తార్కిక avail (గుర్తులేని పూర్ణాంకం లెక్క=1) స్థిరరాశి
  { తిరిగి get_స్థానభ్రంశం () + లెక్క <= str.నిడివి; }
  శూన్య inc (గుర్తులేని పూర్ణాంకం లెక్క=1)
  {
    /* Automatically puts us in error if count is out-of-range. */
    సమితి_స్థానభ్రంశం (get_స్థానభ్రంశం () + లెక్క);
  }

  /* We (ab)use ubytes backwards_length as a cursor (called offset),
   * as well as to store error condition. */

  గుర్తులేని get_స్థానభ్రంశం () స్థిరరాశి { తిరిగి str.backwards_నిడివి; }
  శూన్య సమితి_స్థానభ్రంశం (గుర్తులేని స్థానభ్రంశం) { str.backwards_నిడివి = స్థానభ్రంశం; }

  శూన్య సమితి_error ()      { str.backwards_నిడివి = str.నిడివి + 1; }
  తార్కిక లోపల_error () స్థిరరాశి { తిరిగి str.backwards_నిడివి > str.నిడివి; }

  గుర్తులేని మొత్తం_కొలత () స్థిరరాశి { తిరిగి str.నిడివి; }

  రక్షిత:
  hb_ubytes_t       str;
};

/*దొంతర*/
మూస <రకం_పేరు ELEM, పూర్ణాంకం హద్దు_2>
నిర్మాణం cff_దొంతర_t
{
  ELEM& క్రియాకారకం [] (గుర్తులేని పూర్ణాంకం i)
  {
    ఒకవేళ (unlikely (i >= నిడివి))
    {
      సమితి_error ();
      తిరిగి Crap (ELEM);
    }
    తిరిగి arrayZ[i];
  }

  శూన్య push (స్థిరరాశి ELEM &v)
  {
    ఒకవేళ (likely (నిడివి < హద్దు_2))
      arrayZ[నిడివి++] = v;
    లేకపోతే
      సమితి_error ();
  }
  ELEM &push ()
  {
    ఒకవేళ (likely (నిడివి < హద్దు_2))
      తిరిగి arrayZ[నిడివి++];
    లేకపోతే
    {
      సమితి_error ();
      తిరిగి Crap (ELEM);
    }
  }

  ELEM& తీయి ()
  {
    ఒకవేళ (likely (నిడివి > 0))
      తిరిగి arrayZ[--నిడివి];
    లేకపోతే
    {
      సమితి_error ();
      తిరిగి Crap (ELEM);
    }
  }
  శూన్య తీయి (గుర్తులేని పూర్ణాంకం n)
  {
    ఒకవేళ (likely (నిడివి >= n))
      నిడివి -= n;
    లేకపోతే
      సమితి_error ();
  }

  స్థిరరాశి ELEM& peek ()
  {
    ఒకవేళ (unlikely (నిడివి == 0))
    {
      సమితి_error ();
      తిరిగి Null (ELEM);
    }
    తిరిగి arrayZ[నిడివి - 1];
  }

  శూన్య unpop ()
  {
    ఒకవేళ (likely (నిడివి < హద్దు_2))
      నిడివి++;
    లేకపోతే
      సమితి_error ();
  }

  శూన్య clear () { నిడివి = 0; }

  తార్కిక లోపల_error () స్థిరరాశి { తిరిగి (error); }
  శూన్య సమితి_error ()      { error = నిజం; }

  గుర్తులేని పూర్ణాంకం get_లెక్క () స్థిరరాశి { తిరిగి నిడివి; }
  తార్కిక is_empty () స్థిరరాశి          { తిరిగి !నిడివి; }

  hb_array_t<స్థిరరాశి ELEM> sub_array (గుర్తులేని ప్రారంభం, గుర్తులేని నిడివి) స్థిరరాశి
  { తిరిగి hb_array_t<స్థిరరాశి ELEM> (arrayZ).sub_array (ప్రారంభం, నిడివి); }

  తార్కిక error = అబద్ధం;
  గుర్తులేని పూర్ణాంకం నిడివి = 0;
  ELEM arrayZ[హద్దు_2];
};

/* argument stack */
మూస <రకం_పేరు ARG=సంఖ్య_t>
నిర్మాణం arg_దొంతర_t : cff_దొంతర_t<ARG, 513>
{
  శూన్య నెట్టు_int (పూర్ణాంకం v)
  {
    ARG &n = S::push ();
    n.సమితి_int (v);
  }

  శూన్య నెట్టు_fixed (int32_t v)
  {
    ARG &n = S::push ();
    n.సమితి_fixed (v);
  }

  శూన్య నెట్టు_real (ద్విగుణ v)
  {
    ARG &n = S::push ();
    n.సమితి_real (v);
  }

  ARG& తీయి_అంకె () { తిరిగి ఇది->తీయి (); }

  పూర్ణాంకం తీయి_int ()
  {
    పూర్ణాంకం v;
    ఒకవేళ (unlikely (!ఇది->తీయి ().గమ్యం_int (&v)))
      S::సమితి_error ();
    తిరిగి v;
  }

  గుర్తులేని పూర్ణాంకం తీయి_uint ()
  {
    పూర్ణాంకం i = తీయి_int ();
    ఒకవేళ (unlikely (i < 0))
    {
      i = 0;
      S::సమితి_error ();
    }
    తిరిగి (గుర్తులేని) i;
  }

  శూన్య నెట్టు_longint_from_substr (byte_str_ref_t& str_ref)
  {
    నెట్టు_int ((పూర్ణాంకం) ((uint32_t (str_ref[0]) << 24) | (str_ref[1] << 16) | (str_ref[2] << 8) | (str_ref[3])));
    str_ref.inc (4);
  }

  తార్కిక నెట్టు_fixed_from_substr (byte_str_ref_t& str_ref)
  {
    ఒకవేళ (unlikely (!str_ref.avail (4)))
      తిరిగి అబద్ధం;
    నెట్టు_fixed ((int32_t)*(స్థిరరాశి HBUINT32*)&str_ref[0]);
    str_ref.inc (4);
    తిరిగి నిజం;
  }

  అంతరంగ:
  రకనిర్వచనం cff_దొంతర_t<ARG, 513> S;
};

/* an operator prefixed by its operands in a byte string */
నిర్మాణం op_str_t
{
  /* This used to have a hb_ubytes_t. Using a pointer and length
   * in a particular order, saves 8 bytes in this struct and more
   * in our parsed_cs_op_t subclass. */

  స్థిరరాశి గుర్తులేని అక్షరం *ptr = శూన్యసూచిక;

  op_సంకేతం_t  op = Op_సంకేతం_Invalid;

  uint8_t నిడివి = 0;
};

/* base of OP_SERIALIZER */
నిర్మాణం op_serializer_t
{
  రక్షిత:
  తార్కిక copy_opstr (hb_serialize_context_t *c, స్థిరరాశి op_str_t& opstr) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);

    గుర్తులేని అక్షరం *d = c->allocate_కొలత<గుర్తులేని అక్షరం> (opstr.నిడివి);
    ఒకవేళ (unlikely (!d)) return_trace (అబద్ధం);
    /* Faster than hb_memcpy for small strings. */
    కొరకు (గుర్తులేని i = 0; i < opstr.నిడివి; i++)
      d[i] = opstr.ptr[i];
    return_trace (నిజం);
  }
};

మూస <రకం_పేరు VAL>
నిర్మాణం parsed_విలువలు_t
{
  శూన్య init ()
  {
    op_ప్రారంభం = 0;
    values.init ();
  }
  శూన్య fini () { values.fini (); }

  శూన్య alloc (గుర్తులేని n)
  {
    values.alloc_exact (n);
  }

  శూన్య కూడు_op (op_సంకేతం_t op, స్థిరరాశి byte_str_ref_t& str_ref = byte_str_ref_t (), స్థిరరాశి VAL &v = VAL ())
  {
    VAL *val = values.push (v);
    val->op = op;
    స్వయంచాలక arr = str_ref.sub_array (op_ప్రారంభం, str_ref.get_స్థానభ్రంశం () - op_ప్రారంభం);
    val->ptr = arr.arrayZ;
    val->నిడివి = arr.నిడివి;
    op_ప్రారంభం = str_ref.get_స్థానభ్రంశం ();
  }

  తార్కిక has_op (op_సంకేతం_t op) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి స్వయంచాలక& v : values)
      ఒకవేళ (v.op == op) తిరిగి నిజం;
    తిరిగి అబద్ధం;
  }

  గుర్తులేని get_లెక్క () స్థిరరాశి { తిరిగి values.నిడివి; }
  స్థిరరాశి VAL &క్రియాకారకం [] (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి { తిరిగి values[i]; }

  గుర్తులేని పూర్ణాంకం       op_ప్రారంభం;
  hb_సదిశ_t<VAL>   values;
};

మూస <రకం_పేరు ARG=సంఖ్య_t>
నిర్మాణం interp_env_t
{
  interp_env_t () {}
  interp_env_t (స్థిరరాశి hb_ubytes_t &str_)
  {
    str_ref.reset (str_);
  }
  తార్కిక లోపల_error () స్థిరరాశి
  { తిరిగి str_ref.లోపల_error () || arg_దొంతర.లోపల_error (); }

  శూన్య సమితి_error () { str_ref.సమితి_error (); }

  op_సంకేతం_t fetch_op ()
  {
    op_సంకేతం_t  op = Op_సంకేతం_Invalid;
    ఒకవేళ (unlikely (!str_ref.avail ()))
      తిరిగి Op_సంకేతం_Invalid;
    op = (op_సంకేతం_t) str_ref.తల_unchecked ();
    str_ref.inc ();
    ఒకవేళ (op == Op_సంకేతం_escape) {
      ఒకవేళ (unlikely (!str_ref.avail ()))
	తిరిగి Op_సంకేతం_Invalid;
      op = Make_Op_సంకేతం_ESC (str_ref.తల_unchecked ());
      str_ref.inc ();
    }
    తిరిగి op;
  }

  స్థిరరాశి ARG& eval_arg (గుర్తులేని పూర్ణాంకం i) { తిరిగి arg_దొంతర[i]; }

  ARG& తీయి_arg () { తిరిగి arg_దొంతర.తీయి (); }
  శూన్య తీయి_n_వాదనలు (గుర్తులేని పూర్ణాంకం n) { arg_దొంతర.తీయి (n); }

  శూన్య clear_వాదనలు () { తీయి_n_వాదనలు (arg_దొంతర.get_లెక్క ()); }

  byte_str_ref_t
		str_ref;
  arg_దొంతర_t<ARG>
		arg_దొంతర;
};

ఉపయోగించి అంకె_interp_env_t =  interp_env_t<>;

మూస <రకం_పేరు ARG=సంఖ్య_t>
నిర్మాణం opset_t
{
  స్థిర శూన్య process_op (op_సంకేతం_t op, interp_env_t<ARG>& env)
  {
    ఎంపిక (op) {
      స్థితి Op_సంకేతం_shortint:
	env.arg_దొంతర.నెట్టు_int ((int16_t)((env.str_ref[0] << 8) | env.str_ref[1]));
	env.str_ref.inc (2);
	ఆపు;

      స్థితి Op_సంకేతం_Two_Byte_Pos_Int0: స్థితి Op_సంకేతం_Two_Byte_Pos_Int1:
      స్థితి Op_సంకేతం_Two_Byte_Pos_Int2: స్థితి Op_సంకేతం_Two_Byte_Pos_Int3:
	env.arg_దొంతర.నెట్టు_int ((int16_t)((op - Op_సంకేతం_Two_Byte_Pos_Int0) * 256 + env.str_ref[0] + 108));
	env.str_ref.inc ();
	ఆపు;

      స్థితి Op_సంకేతం_Two_Byte_Neg_Int0: స్థితి Op_సంకేతం_Two_Byte_Neg_Int1:
      స్థితి Op_సంకేతం_Two_Byte_Neg_Int2: స్థితి Op_సంకేతం_Two_Byte_Neg_Int3:
	env.arg_దొంతర.నెట్టు_int ((-(int16_t)(op - Op_సంకేతం_Two_Byte_Neg_Int0) * 256 - env.str_ref[0] - 108));
	env.str_ref.inc ();
	ఆపు;

      అప్రమేయ:
	/* 1-byte integer */
	ఒకవేళ (likely ((Op_సంకేతం_One_Byte_Int_మొదటి <= op) && (op <= Op_సంకేతం_One_Byte_Int_చివరి)))
	{
	  env.arg_దొంతర.నెట్టు_int ((పూర్ణాంకం)op - 139);
	} లేకపోతే {
	  /* invalid unknown operator */
	  env.clear_వాదనలు ();
	}
	ఆపు;
    }
  }
};

మూస <రకం_పేరు ENV>
నిర్మాణం interpreter_t
{
  interpreter_t (ENV& env_) : env (env_) {}
  ENV& env;
};

} /* namespace CFF */

#అయితే_ముగింపు /* HB_CFF_INTERP_COMMON_HH */
