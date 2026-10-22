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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_CFF_INTERP_COMMON_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CFF_INTERP_COMMON_HH

ਬਾਹਰੀ HB_ਅੰਦਰੂਨੀ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *endchar_str;

ਨਾਮਾਕਾਸ਼ CFF {

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ OT;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ op_ਕੋਡ_t;


/* === Dict operators === */

/* One byte operators (0-31) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_ਸੰਸਕਰਣ		  0 /* CFF Top */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Notice		  1 /* CFF Top */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_ਭਰਿਆ_Name		  2 /* CFF Top */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Family_Name	  3 /* CFF Top */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_ਭਾਰ		  4 /* CFF Top */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Font_BBox		  5 /* CFF Top */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Blue_ਮੁੱਲਾਂ	  6 /* CFF Private, CFF2 Private */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Other_Blues	  7 /* CFF Private, CFF2 Private */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Family_Blues	  8 /* CFF Private, CFF2 Private */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Family_Other_Blues	  9 /* CFF Private, CFF2 Private */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Std_HW		 10 /* CFF Private, CFF2 Private */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Std_VW		 11 /* CFF Private, CFF2 Private */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_escape		 12 /* All. Shared with CS */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Unique_ID		 13 /* CFF Top */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_XUID		 14 /* CFF Top */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_charset		 15 /* CFF Top (0) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Encoding		 16 /* CFF Top (0) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Char_ਲੜੀਆਂ	 17 /* CFF Top, CFF2 Top */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_ਨਿੱਜੀ		 18 /* CFF Top, CFF2 FD */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Subrs		 19 /* CFF Private, CFF2 Private */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_default_ਚੌੜਾਈ_X	 20 /* CFF Private (0) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_nominal_ਚੌੜਾਈ_X	 21 /* CFF Private (0) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_vsindexdict	 22 /* CFF2 Private/CS */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_blenddict	 23 /* CFF2 Private/CS */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_vstore		 24 /* CFF2 Top */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_reserved25	 25
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_reserved26	 26
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_reserved27	 27

/* Numbers */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_shortint		 28 /* 16-bit integer, All */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_longintdict	 29 /* 32-bit integer, All */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_BCD		 30 /* Real number, CFF2 Top/FD */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_reserved31	 31

/* 1-byte integers */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_One_Byte_Int_ਪਹਿਲਾ	 32 /* All. beginning of the range of first byte ints */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_One_Byte_Int_ਆਖਰੀ	246 /* All. ending of the range of first byte int */

/* 2-byte integers */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Two_Byte_Pos_Int0	247 /* All. first byte of two byte positive int (+108 to +1131) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Two_Byte_Pos_Int1	248
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Two_Byte_Pos_Int2	249
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Two_Byte_Pos_Int3	250

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Two_Byte_Neg_Int0	251 /* All. first byte of two byte negative int (-1131 to -108) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Two_Byte_Neg_Int1	252
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Two_Byte_Neg_Int2	253
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Two_Byte_Neg_Int3	254

/* Two byte escape operators 12, (0-41) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_ESC_ਅਧਾਰ		256
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Make_Op_ਕੋਡ_ESC(byte2)	((op_ਕੋਡ_t)(Op_ਕੋਡ_ESC_ਅਧਾਰ + (byte2)))

ਸਤਰਅੰਦਰ op_ਕੋਡ_t Unmake_Op_ਕੋਡ_ESC (op_ਕੋਡ_t op)  { ਵਾਪਸ (op_ਕੋਡ_t)(op - Op_ਕੋਡ_ESC_ਅਧਾਰ); }
ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ Is_Op_ਕੋਡ_ESC (op_ਕੋਡ_t op) { ਵਾਪਸ op >= Op_ਕੋਡ_ESC_ਅਧਾਰ; }
ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ Op_ਕੋਡ_ਮਾਪ (op_ਕੋਡ_t op) { ਵਾਪਸ Is_Op_ਕੋਡ_ESC (op) ? 2: 1; }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Copyright	Make_Op_ਕੋਡ_ESC(0) /* CFF Top */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_is_Fixed_Pitch	Make_Op_ਕੋਡ_ESC(1) /* CFF Top (false) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Italic_Angle	Make_Op_ਕੋਡ_ESC(2) /* CFF Top (0) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Underline_Position Make_Op_ਕੋਡ_ESC(3) /* CFF Top (-100) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Underline_Thickness Make_Op_ਕੋਡ_ESC(4) /* CFF Top (50) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Paint_Type	Make_Op_ਕੋਡ_ESC(5) /* CFF Top (0) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Charstring_Type	Make_Op_ਕੋਡ_ESC(6) /* CFF Top (2) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Font_ਮੈਟ੍ਰਿਕਸ	Make_Op_ਕੋਡ_ESC(7) /* CFF Top, CFF2 Top (.001 0 0 .001 0 0)*/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Stroke_ਚੌੜਾਈ	Make_Op_ਕੋਡ_ESC(8) /* CFF Top (0) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Blue_Scale	Make_Op_ਕੋਡ_ESC(9) /* CFF Private, CFF2 Private (0.039625) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Blue_ਖਿਸਕਾਓ	Make_Op_ਕੋਡ_ESC(10) /* CFF Private, CFF2 Private (7) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Blue_Fuzz		Make_Op_ਕੋਡ_ESC(11) /* CFF Private, CFF2 Private (1) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Stem_Snap_H	Make_Op_ਕੋਡ_ESC(12) /* CFF Private, CFF2 Private */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Stem_Snap_V	Make_Op_ਕੋਡ_ESC(13) /* CFF Private, CFF2 Private */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Force_Bold	Make_Op_ਕੋਡ_ESC(14) /* CFF Private (false) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_reserved_ESC15	Make_Op_ਕੋਡ_ESC(15)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_reserved_ESC16	Make_Op_ਕੋਡ_ESC(16)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Language_ਸਮੂਹ	Make_Op_ਕੋਡ_ESC(17) /* CFF Private, CFF2 Private (0) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Expansion_ਗੁਣਕ	Make_Op_ਕੋਡ_ESC(18) /* CFF Private, CFF2 Private (0.06) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_initial_Random_ਬੀਜ Make_Op_ਕੋਡ_ESC(19) /* CFF Private (0) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Synthetic_ਅਧਾਰ	Make_Op_ਕੋਡ_ESC(20) /* CFF Top */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Post_Script	Make_Op_ਕੋਡ_ESC(21) /* CFF Top */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_ਅਧਾਰ_Font_Name	Make_Op_ਕੋਡ_ESC(22) /* CFF Top */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_ਅਧਾਰ_Font_Blend	Make_Op_ਕੋਡ_ESC(23) /* CFF Top */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_reserved_ESC24	Make_Op_ਕੋਡ_ESC(24)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_reserved_ESC25	Make_Op_ਕੋਡ_ESC(25)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_reserved_ESC26	Make_Op_ਕੋਡ_ESC(26)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_reserved_ESC27	Make_Op_ਕੋਡ_ESC(27)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_reserved_ESC28	Make_Op_ਕੋਡ_ESC(28)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_reserved_ESC29	Make_Op_ਕੋਡ_ESC(29)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_ROS		Make_Op_ਕੋਡ_ESC(30) /* CFF Top_CID */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_CIDFont_ਸੰਸਕਰਣ	Make_Op_ਕੋਡ_ESC(31) /* CFF Top_CID (0) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_CIDFont_Revision	Make_Op_ਕੋਡ_ESC(32) /* CFF Top_CID (0) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_CIDFont_Type	Make_Op_ਕੋਡ_ESC(33) /* CFF Top_CID (0) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_CIDCount		Make_Op_ਕੋਡ_ESC(34) /* CFF Top_CID (8720) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_UIDBase		Make_Op_ਕੋਡ_ESC(35) /* CFF Top_CID */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_FDArray		Make_Op_ਕੋਡ_ESC(36) /* CFF Top_CID, CFF2 Top */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_FDSelect		Make_Op_ਕੋਡ_ESC(37) /* CFF Top_CID, CFF2 Top */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Font_Name		Make_Op_ਕੋਡ_ESC(38) /* CFF Top_CID */


/* === CharString operators === */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_hstem		  1 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Reserved2	  2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_vstem		  3 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_vmoveto		  4 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_rlineto		  5 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_hlineto		  6 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_vlineto		  7 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_rrcurveto	  8 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Reserved9	  9
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_callsubr		 10 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_return		 11 /* CFF */
//#define OpCode_escape		 12 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Reserved13	 13
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_endchar		 14 /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_vsindexcs	 15 /* CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_blendcs		 16 /* CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Reserved17	 17
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_hstemhm		 18 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_hintmask		 19 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_cntrmask		 20 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_rmoveto		 21 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_hmoveto		 22 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_vstemhm		 23 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_rcurveline	 24 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_rlinecurve	 25 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_vvcurveto	 26 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_hhcurveto	 27 /* CFF, CFF2 */
//#define OpCode_shortint	 28 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_callgsubr	 29 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_vhcurveto	 30 /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_hvcurveto	 31 /* CFF, CFF2 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_fixedcs		255 /* 32-bit fixed */

/* Two byte escape operators 12, (0-41) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_dotsection	Make_Op_ਕੋਡ_ESC(0) /* CFF (obsoleted) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Reserved_ESC1	Make_Op_ਕੋਡ_ESC(1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Reserved_ESC2	Make_Op_ਕੋਡ_ESC(2)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_and		Make_Op_ਕੋਡ_ESC(3) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_or		Make_Op_ਕੋਡ_ESC(4) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_not		Make_Op_ਕੋਡ_ESC(5) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Reserved_ESC6	Make_Op_ਕੋਡ_ESC(6)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Reserved_ESC7	Make_Op_ਕੋਡ_ESC(7)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Reserved_ESC8	Make_Op_ਕੋਡ_ESC(8)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_abs		Make_Op_ਕੋਡ_ESC(9) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_ਜੋੜੋ		Make_Op_ਕੋਡ_ESC(10) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_sub		Make_Op_ਕੋਡ_ESC(11) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_div		Make_Op_ਕੋਡ_ESC(12) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Reserved_ESC13	Make_Op_ਕੋਡ_ESC(13)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_neg		Make_Op_ਕੋਡ_ESC(14) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_eq		Make_Op_ਕੋਡ_ESC(15) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Reserved_ESC16	Make_Op_ਕੋਡ_ESC(16)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Reserved_ESC17	Make_Op_ਕੋਡ_ESC(17)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_drop		Make_Op_ਕੋਡ_ESC(18) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Reserved_ESC19	Make_Op_ਕੋਡ_ESC(19)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_put		Make_Op_ਕੋਡ_ESC(20) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_get		Make_Op_ਕੋਡ_ESC(21) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_ifelse		Make_Op_ਕੋਡ_ESC(22) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_random		Make_Op_ਕੋਡ_ESC(23) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_mul		Make_Op_ਕੋਡ_ESC(24) /* CFF */
//#define OpCode_reservedESC25	Make_OpCode_ESC(25)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_sqrt		Make_Op_ਕੋਡ_ESC(26) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_dup		Make_Op_ਕੋਡ_ESC(27) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_exch		Make_Op_ਕੋਡ_ESC(28) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_ਸੂਚਕਾਂਕ		Make_Op_ਕੋਡ_ESC(29) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_roll		Make_Op_ਕੋਡ_ESC(30) /* CFF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_reserved_ESC31	Make_Op_ਕੋਡ_ESC(31)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_reserved_ESC32	Make_Op_ਕੋਡ_ESC(32)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_reserved_ESC33	Make_Op_ਕੋਡ_ESC(33)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_hflex		Make_Op_ਕੋਡ_ESC(34) /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_flex		Make_Op_ਕੋਡ_ESC(35) /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_hflex1		Make_Op_ਕੋਡ_ESC(36) /* CFF, CFF2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_flex1		Make_Op_ਕੋਡ_ESC(37) /* CFF, CFF2 */


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Op_ਕੋਡ_Invalid		0xFFFFu


ਬਣਤਰ ਸੰਖਿਆ_t
{
  ਖਾਲੀ ਸੈੱਟ_int (ਪੂਰਨ v)       { value = v; }
  ਤਾਰਕਿਕ ਮੰਜ਼ਿਲ_int (ਪੂਰਨ *v) ਅਚਲ
  {
    ਜੇ (unlikely (!(value >= INT_MIN && value <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)))
    {
      *v = 0;
      ਵਾਪਸ ਝੂਠ;
    }
    *v = (ਪੂਰਨ) value;
    ਵਾਪਸ ਸੱਚ;
  }
  ਪੂਰਨ ਮੰਜ਼ਿਲ_int () ਅਚਲ
  {
    ਪੂਰਨ v;
    ਜੇ (likely (ਮੰਜ਼ਿਲ_int (&v))) ਵਾਪਸ v;
    ਵਾਪਸ value < 0 ? INT_MIN : value > 0 ? ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ : 0;
  }

  ਖਾਲੀ ਸੈੱਟ_fixed (int32_t v) { value = v / 65536.0; }
  int32_t ਮੰਜ਼ਿਲ_fixed () ਅਚਲ
  { ਵਾਪਸ (int32_t) hb_clamp (value * 65536.0, (ਦੁੱਗਣਾ) INT32_MIN, (ਦੁੱਗਣਾ) INT32_MAX); }

  ਖਾਲੀ ਸੈੱਟ_real (ਦੁੱਗਣਾ v)   { value = v; }
  ਦੁੱਗਣਾ ਮੰਜ਼ਿਲ_real () ਅਚਲ    { ਵਾਪਸ value; }

  ਤਾਰਕਿਕ ਵਿੱਚ_int_range () ਅਚਲ
  { ਵਾਪਸ value >= INT16_MIN && value <= INT16_MAX && (ਦੁੱਗਣਾ) (int16_t) value == value; }

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ >  (ਅਚਲ ਸੰਖਿਆ_t &n) ਅਚਲ { ਵਾਪਸ value > n.ਮੰਜ਼ਿਲ_real (); }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ <  (ਅਚਲ ਸੰਖਿਆ_t &n) ਅਚਲ { ਵਾਪਸ n > *ਇਹ; }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ >= (ਅਚਲ ਸੰਖਿਆ_t &n) ਅਚਲ { ਵਾਪਸ !(*ਇਹ < n); }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ <= (ਅਚਲ ਸੰਖਿਆ_t &n) ਅਚਲ { ਵਾਪਸ !(*ਇਹ > n); }

  ਅਚਲ ਸੰਖਿਆ_t &ਸੰਚਾਲਕ += (ਅਚਲ ਸੰਖਿਆ_t &n)
  {
    ਸੈੱਟ_real (ਮੰਜ਼ਿਲ_real () + n.ਮੰਜ਼ਿਲ_real ());

    ਵਾਪਸ *ਇਹ;
  }

  ਸੁਰੱਖਿਅਤ:
  ਦੁੱਗਣਾ value = 0.;
};

/* byte string */
ਬਣਤਰ UnsizedByteStr : Unsized_Array_ਦਾ <HBUINT8>
{
  hb_ubytes_t as_ubytes (ਬਿਨਾਨਿਸ਼ਾਨੀ l) ਅਚਲ
  { ਵਾਪਸ hb_ubytes_t ((ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) ਇਹ, l); }

  // encode 2-byte int (Dict/CharString) or 4-byte int (Dict)
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ V>
  ਸਥਿਰ ਤਾਰਕਿਕ serialize_int (hb_serialize_context_t *c, op_ਕੋਡ_t intOp, V value)
  {
    TRACE_SERIALIZE (ਇਹ);

    HBUINT8 *p = c->allocate_ਮਾਪ<HBUINT8> (1);
    ਜੇ (unlikely (!p)) return_trace (ਝੂਠ);
    *p = intOp;

    T *ip = c->allocate_ਮਾਪ<T> (T::static_ਮਾਪ);
    ਜੇ (unlikely (!ip)) return_trace (ਝੂਠ);
    return_trace (c->check_ਸੌਂਪੋ (*ip, value, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ V>
  ਸਥਿਰ ਤਾਰਕਿਕ serialize_int4 (hb_serialize_context_t *c, V value)
  { ਵਾਪਸ serialize_int<HBINT32> (c, Op_ਕੋਡ_longintdict, value); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ V>
  ਸਥਿਰ ਤਾਰਕਿਕ serialize_int2 (hb_serialize_context_t *c, V value)
  { ਵਾਪਸ serialize_int<HBINT16> (c, Op_ਕੋਡ_shortint, value); }

  /* Defining null_size allows a Null object may be created. Should be safe because:
   * A descendent struct Dict uses a Null pointer to indicate a missing table,
   * checked before access.
   */
  DEFINE_ਮਾਪ_MIN(0);
};

/* A byte string associated with the current offset and an error condition */
ਬਣਤਰ byte_str_ref_t
{
  byte_str_ref_t ()
    : str () {}

  byte_str_ref_t (ਅਚਲ hb_ubytes_t &str_, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ_3 = 0)
    : str (str_) { ਸੈੱਟ_ਵਿੱਥ (ਵਿੱਥ_3); }

  ਖਾਲੀ reset (ਅਚਲ hb_ubytes_t &str_, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ_3 = 0)
  {
    str = str_;
    ਸੈੱਟ_ਵਿੱਥ (ਵਿੱਥ_3);
  }

  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ& ਸੰਚਾਲਕ [] (ਪੂਰਨ i) {
    ਜੇ (unlikely ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (get_ਵਿੱਥ () + i) >= str.ਲੰਮਾਈ))
    {
      ਸੈੱਟ_error ();
      ਵਾਪਸ Null (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ);
    }
    ਵਾਪਸ str.arrayZ[get_ਵਿੱਥ () + i];
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਸਿਰ_unchecked () ਅਚਲ { ਵਾਪਸ str.arrayZ[get_ਵਿੱਥ ()]; }

  /* Conversion to hb_ubytes_t */
  ਸੰਚਾਲਕ hb_ubytes_t () ਅਚਲ { ਵਾਪਸ str.sub_array (get_ਵਿੱਥ ()); }

  hb_ubytes_t sub_array (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ_3, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len_) ਅਚਲ
  { ਵਾਪਸ str.sub_array (ਵਿੱਥ_3, len_); }

  ਤਾਰਕਿਕ avail (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2=1) ਅਚਲ
  { ਵਾਪਸ get_ਵਿੱਥ () + ਗਿਣਤੀ_2 <= str.ਲੰਮਾਈ; }
  ਖਾਲੀ inc (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2=1)
  {
    /* Automatically puts us in error if count is out-of-range. */
    ਸੈੱਟ_ਵਿੱਥ (get_ਵਿੱਥ () + ਗਿਣਤੀ_2);
  }

  /* We (ab)use ubytes backwards_length as a cursor (called offset),
   * as well as to store error condition. */

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਵਿੱਥ () ਅਚਲ { ਵਾਪਸ str.backwards_ਲੰਮਾਈ; }
  ਖਾਲੀ ਸੈੱਟ_ਵਿੱਥ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ) { str.backwards_ਲੰਮਾਈ = ਵਿੱਥ; }

  ਖਾਲੀ ਸੈੱਟ_error ()      { str.backwards_ਲੰਮਾਈ = str.ਲੰਮਾਈ + 1; }
  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ { ਵਾਪਸ str.backwards_ਲੰਮਾਈ > str.ਲੰਮਾਈ; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ਮਾਪ () ਅਚਲ { ਵਾਪਸ str.ਲੰਮਾਈ; }

  ਸੁਰੱਖਿਅਤ:
  hb_ubytes_t       str;
};

/*ਢੇਰ*/
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ELEM, ਪੂਰਨ ਹੱਦਬੰਦੀ_2>
ਬਣਤਰ cff_ਢੇਰ_t
{
  ELEM& ਸੰਚਾਲਕ [] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i)
  {
    ਜੇ (unlikely (i >= ਲੰਮਾਈ))
    {
      ਸੈੱਟ_error ();
      ਵਾਪਸ Crap (ELEM);
    }
    ਵਾਪਸ arrayZ[i];
  }

  ਖਾਲੀ push (ਅਚਲ ELEM &v)
  {
    ਜੇ (likely (ਲੰਮਾਈ < ਹੱਦਬੰਦੀ_2))
      arrayZ[ਲੰਮਾਈ++] = v;
    ਨਹੀਂਤਾਂ
      ਸੈੱਟ_error ();
  }
  ELEM &push ()
  {
    ਜੇ (likely (ਲੰਮਾਈ < ਹੱਦਬੰਦੀ_2))
      ਵਾਪਸ arrayZ[ਲੰਮਾਈ++];
    ਨਹੀਂਤਾਂ
    {
      ਸੈੱਟ_error ();
      ਵਾਪਸ Crap (ELEM);
    }
  }

  ELEM& ਕੱਢੋ ()
  {
    ਜੇ (likely (ਲੰਮਾਈ > 0))
      ਵਾਪਸ arrayZ[--ਲੰਮਾਈ];
    ਨਹੀਂਤਾਂ
    {
      ਸੈੱਟ_error ();
      ਵਾਪਸ Crap (ELEM);
    }
  }
  ਖਾਲੀ ਕੱਢੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n)
  {
    ਜੇ (likely (ਲੰਮਾਈ >= n))
      ਲੰਮਾਈ -= n;
    ਨਹੀਂਤਾਂ
      ਸੈੱਟ_error ();
  }

  ਅਚਲ ELEM& peek ()
  {
    ਜੇ (unlikely (ਲੰਮਾਈ == 0))
    {
      ਸੈੱਟ_error ();
      ਵਾਪਸ Null (ELEM);
    }
    ਵਾਪਸ arrayZ[ਲੰਮਾਈ - 1];
  }

  ਖਾਲੀ unpop ()
  {
    ਜੇ (likely (ਲੰਮਾਈ < ਹੱਦਬੰਦੀ_2))
      ਲੰਮਾਈ++;
    ਨਹੀਂਤਾਂ
      ਸੈੱਟ_error ();
  }

  ਖਾਲੀ clear () { ਲੰਮਾਈ = 0; }

  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ { ਵਾਪਸ (error); }
  ਖਾਲੀ ਸੈੱਟ_error ()      { error = ਸੱਚ; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ ਲੰਮਾਈ; }
  ਤਾਰਕਿਕ is_empty () ਅਚਲ          { ਵਾਪਸ !ਲੰਮਾਈ; }

  hb_array_t<ਅਚਲ ELEM> sub_array (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ) ਅਚਲ
  { ਵਾਪਸ hb_array_t<ਅਚਲ ELEM> (arrayZ).sub_array (ਸ਼ੁਰੂ, ਲੰਮਾਈ); }

  ਤਾਰਕਿਕ error = ਝੂਠ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ = 0;
  ELEM arrayZ[ਹੱਦਬੰਦੀ_2];
};

/* argument stack */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ARG=ਸੰਖਿਆ_t>
ਬਣਤਰ arg_ਢੇਰ_t : cff_ਢੇਰ_t<ARG, 513>
{
  ਖਾਲੀ ਧੱਕੋ_int (ਪੂਰਨ v)
  {
    ARG &n = S::push ();
    n.ਸੈੱਟ_int (v);
  }

  ਖਾਲੀ ਧੱਕੋ_fixed (int32_t v)
  {
    ARG &n = S::push ();
    n.ਸੈੱਟ_fixed (v);
  }

  ਖਾਲੀ ਧੱਕੋ_real (ਦੁੱਗਣਾ v)
  {
    ARG &n = S::push ();
    n.ਸੈੱਟ_real (v);
  }

  ARG& ਕੱਢੋ_ਅੰਕ () { ਵਾਪਸ ਇਹ->ਕੱਢੋ (); }

  ਪੂਰਨ ਕੱਢੋ_int ()
  {
    ਪੂਰਨ v;
    ਜੇ (unlikely (!ਇਹ->ਕੱਢੋ ().ਮੰਜ਼ਿਲ_int (&v)))
      S::ਸੈੱਟ_error ();
    ਵਾਪਸ v;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕੱਢੋ_uint ()
  {
    ਪੂਰਨ i = ਕੱਢੋ_int ();
    ਜੇ (unlikely (i < 0))
    {
      i = 0;
      S::ਸੈੱਟ_error ();
    }
    ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ) i;
  }

  ਖਾਲੀ ਧੱਕੋ_longint_from_substr (byte_str_ref_t& str_ref)
  {
    ਧੱਕੋ_int ((ਪੂਰਨ) ((uint32_t (str_ref[0]) << 24) | (str_ref[1] << 16) | (str_ref[2] << 8) | (str_ref[3])));
    str_ref.inc (4);
  }

  ਤਾਰਕਿਕ ਧੱਕੋ_fixed_from_substr (byte_str_ref_t& str_ref)
  {
    ਜੇ (unlikely (!str_ref.avail (4)))
      ਵਾਪਸ ਝੂਠ;
    ਧੱਕੋ_fixed ((int32_t)*(ਅਚਲ HBUINT32*)&str_ref[0]);
    str_ref.inc (4);
    ਵਾਪਸ ਸੱਚ;
  }

  ਨਿਜੀ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ cff_ਢੇਰ_t<ARG, 513> S;
};

/* an operator prefixed by its operands in a byte string */
ਬਣਤਰ op_str_t
{
  /* This used to have a hb_ubytes_t. Using a pointer and length
   * in a particular order, saves 8 bytes in this struct and more
   * in our parsed_cs_op_t subclass. */

  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ptr = ਖਾਲੀ_ਸੂਚਕ;

  op_ਕੋਡ_t  op = Op_ਕੋਡ_Invalid;

  uint8_t ਲੰਮਾਈ = 0;
};

/* base of OP_SERIALIZER */
ਬਣਤਰ op_serializer_t
{
  ਸੁਰੱਖਿਅਤ:
  ਤਾਰਕਿਕ copy_opstr (hb_serialize_context_t *c, ਅਚਲ op_str_t& opstr) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *d = c->allocate_ਮਾਪ<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> (opstr.ਲੰਮਾਈ);
    ਜੇ (unlikely (!d)) return_trace (ਝੂਠ);
    /* Faster than hb_memcpy for small strings. */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < opstr.ਲੰਮਾਈ; i++)
      d[i] = opstr.ptr[i];
    return_trace (ਸੱਚ);
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ VAL>
ਬਣਤਰ parsed_ਮੁੱਲਾਂ_t
{
  ਖਾਲੀ init ()
  {
    op_ਸ਼ੁਰੂ = 0;
    values.init ();
  }
  ਖਾਲੀ fini () { values.fini (); }

  ਖਾਲੀ alloc (ਬਿਨਾਨਿਸ਼ਾਨੀ n)
  {
    values.alloc_exact (n);
  }

  ਖਾਲੀ ਜੋੜੋ_op (op_ਕੋਡ_t op, ਅਚਲ byte_str_ref_t& str_ref = byte_str_ref_t (), ਅਚਲ VAL &v = VAL ())
  {
    VAL *val = values.push (v);
    val->op = op;
    ਸਵੈਚਾਲਿਤ arr = str_ref.sub_array (op_ਸ਼ੁਰੂ, str_ref.get_ਵਿੱਥ () - op_ਸ਼ੁਰੂ);
    val->ptr = arr.arrayZ;
    val->ਲੰਮਾਈ = arr.ਲੰਮਾਈ;
    op_ਸ਼ੁਰੂ = str_ref.get_ਵਿੱਥ ();
  }

  ਤਾਰਕਿਕ has_op (op_ਕੋਡ_t op) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& v : values)
      ਜੇ (v.op == op) ਵਾਪਸ ਸੱਚ;
    ਵਾਪਸ ਝੂਠ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ values.ਲੰਮਾਈ; }
  ਅਚਲ VAL &ਸੰਚਾਲਕ [] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ { ਵਾਪਸ values[i]; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ       op_ਸ਼ੁਰੂ;
  hb_ਸਦਿਸ਼_t<VAL>   values;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ARG=ਸੰਖਿਆ_t>
ਬਣਤਰ interp_env_t
{
  interp_env_t () {}
  interp_env_t (ਅਚਲ hb_ubytes_t &str_)
  {
    str_ref.reset (str_);
  }
  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ
  { ਵਾਪਸ str_ref.ਵਿੱਚ_error () || arg_ਢੇਰ.ਵਿੱਚ_error (); }

  ਖਾਲੀ ਸੈੱਟ_error () { str_ref.ਸੈੱਟ_error (); }

  op_ਕੋਡ_t fetch_op ()
  {
    op_ਕੋਡ_t  op = Op_ਕੋਡ_Invalid;
    ਜੇ (unlikely (!str_ref.avail ()))
      ਵਾਪਸ Op_ਕੋਡ_Invalid;
    op = (op_ਕੋਡ_t) str_ref.ਸਿਰ_unchecked ();
    str_ref.inc ();
    ਜੇ (op == Op_ਕੋਡ_escape) {
      ਜੇ (unlikely (!str_ref.avail ()))
	ਵਾਪਸ Op_ਕੋਡ_Invalid;
      op = Make_Op_ਕੋਡ_ESC (str_ref.ਸਿਰ_unchecked ());
      str_ref.inc ();
    }
    ਵਾਪਸ op;
  }

  ਅਚਲ ARG& eval_arg (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) { ਵਾਪਸ arg_ਢੇਰ[i]; }

  ARG& ਕੱਢੋ_arg () { ਵਾਪਸ arg_ਢੇਰ.ਕੱਢੋ (); }
  ਖਾਲੀ ਕੱਢੋ_n_ਦਲੀਲਾਂ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n) { arg_ਢੇਰ.ਕੱਢੋ (n); }

  ਖਾਲੀ clear_ਦਲੀਲਾਂ () { ਕੱਢੋ_n_ਦਲੀਲਾਂ (arg_ਢੇਰ.get_ਗਿਣਤੀ ()); }

  byte_str_ref_t
		str_ref;
  arg_ਢੇਰ_t<ARG>
		arg_ਢੇਰ;
};

ਵਰਤੋਂ ਅੰਕ_interp_env_t =  interp_env_t<>;

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ARG=ਸੰਖਿਆ_t>
ਬਣਤਰ opset_t
{
  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, interp_env_t<ARG>& env)
  {
    ਚੋਣ (op) {
      ਹਾਲਤ Op_ਕੋਡ_shortint:
	env.arg_ਢੇਰ.ਧੱਕੋ_int ((int16_t)((env.str_ref[0] << 8) | env.str_ref[1]));
	env.str_ref.inc (2);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_Two_Byte_Pos_Int0: ਹਾਲਤ Op_ਕੋਡ_Two_Byte_Pos_Int1:
      ਹਾਲਤ Op_ਕੋਡ_Two_Byte_Pos_Int2: ਹਾਲਤ Op_ਕੋਡ_Two_Byte_Pos_Int3:
	env.arg_ਢੇਰ.ਧੱਕੋ_int ((int16_t)((op - Op_ਕੋਡ_Two_Byte_Pos_Int0) * 256 + env.str_ref[0] + 108));
	env.str_ref.inc ();
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_Two_Byte_Neg_Int0: ਹਾਲਤ Op_ਕੋਡ_Two_Byte_Neg_Int1:
      ਹਾਲਤ Op_ਕੋਡ_Two_Byte_Neg_Int2: ਹਾਲਤ Op_ਕੋਡ_Two_Byte_Neg_Int3:
	env.arg_ਢੇਰ.ਧੱਕੋ_int ((-(int16_t)(op - Op_ਕੋਡ_Two_Byte_Neg_Int0) * 256 - env.str_ref[0] - 108));
	env.str_ref.inc ();
	ਰੋਕੋ;

      ਮੂਲ:
	/* 1-byte integer */
	ਜੇ (likely ((Op_ਕੋਡ_One_Byte_Int_ਪਹਿਲਾ <= op) && (op <= Op_ਕੋਡ_One_Byte_Int_ਆਖਰੀ)))
	{
	  env.arg_ਢੇਰ.ਧੱਕੋ_int ((ਪੂਰਨ)op - 139);
	} ਨਹੀਂਤਾਂ {
	  /* invalid unknown operator */
	  env.clear_ਦਲੀਲਾਂ ();
	}
	ਰੋਕੋ;
    }
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ENV>
ਬਣਤਰ interpreter_t
{
  interpreter_t (ENV& env_) : env (env_) {}
  ENV& env;
};

} /* namespace CFF */

#ਜੇ_ਅੰਤ /* HB_CFF_INTERP_COMMON_HH */
