
#line 1 "hb-buffer-deserialize-text-glyphs.rl"
/*
 * Copyright © 2013  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_BUFFER_DESERIALIZE_TEXT_GLYPHS_HH
#define HB_BUFFER_DESERIALIZE_TEXT_GLYPHS_HH

#અન્તર્ભાવ "hb.hh"


#line 33 "hb-buffer-deserialize-text-glyphs.hh"
સ્થિર અચલ unsigned અક્ષર deserialize_text_glyphs_trans_ચાવીઓ[] = {
	0u, 0u, 35u, 124u, 48u, 57u, 60u, 124u, 45u, 57u, 48u, 57u, 44u, 44u, 45u, 57u, 
	48u, 57u, 44u, 44u, 45u, 57u, 48u, 57u, 44u, 44u, 45u, 57u, 48u, 57u, 62u, 62u, 
	93u, 124u, 45u, 57u, 48u, 57u, 35u, 124u, 45u, 57u, 48u, 57u, 35u, 124u, 35u, 124u, 
	35u, 124u, 35u, 124u, 35u, 124u, 35u, 124u, 48u, 57u, 35u, 124u, 45u, 57u, 48u, 57u, 
	44u, 44u, 45u, 57u, 48u, 57u, 35u, 124u, 35u, 124u, 44u, 57u, 35u, 124u, 43u, 124u, 
	35u, 124u, 48u, 62u, 44u, 57u, 44u, 57u, 44u, 57u, 48u, 124u, 35u, 124u, 35u, 124u, 
	35u, 124u, 0
};

સ્થિર અચલ અક્ષર deserialize_text_glyphs_ચાવી_spans[] = {
	0, 90, 10, 65, 13, 10, 1, 13, 
	10, 1, 13, 10, 1, 13, 10, 1, 
	32, 13, 10, 90, 13, 10, 90, 90, 
	90, 90, 90, 90, 10, 90, 13, 10, 
	1, 13, 10, 90, 90, 14, 90, 82, 
	90, 15, 14, 14, 14, 77, 90, 90, 
	90
};

સ્થિર અચલ short _deserialize_text_glyphs_index_offsets[] = {
	0, 0, 91, 102, 168, 182, 193, 195, 
	209, 220, 222, 236, 247, 249, 263, 274, 
	276, 309, 323, 334, 425, 439, 450, 541, 
	632, 723, 814, 905, 996, 1007, 1098, 1112, 
	1123, 1125, 1139, 1150, 1241, 1332, 1347, 1438, 
	1521, 1612, 1628, 1643, 1658, 1673, 1751, 1842, 
	1933
};

સ્થિર અચલ અક્ષર _deserialize_text_glyphs_indicies[] = {
	1, 0, 0, 0, 0, 0, 0, 
	0, 2, 3, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 4, 5, 0, 0, 6, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 7, 8, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 8, 0, 9, 10, 10, 10, 
	10, 10, 10, 10, 10, 10, 3, 11, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	12, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 12, 
	3, 13, 3, 3, 14, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 3, 14, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 3, 16, 3, 17, 3, 3, 18, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 3, 18, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 3, 20, 3, 21, 
	3, 3, 22, 23, 23, 23, 23, 23, 
	23, 23, 23, 23, 3, 22, 23, 23, 
	23, 23, 23, 23, 23, 23, 23, 3, 
	24, 3, 25, 3, 3, 26, 27, 27, 
	27, 27, 27, 27, 27, 27, 27, 3, 
	26, 27, 27, 27, 27, 27, 27, 27, 
	27, 27, 3, 28, 3, 29, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 29, 3, 30, 3, 
	3, 31, 32, 32, 32, 32, 32, 32, 
	32, 32, 32, 3, 33, 34, 34, 34, 
	34, 34, 34, 34, 34, 34, 3, 35, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	36, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	37, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 38, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	38, 3, 39, 3, 3, 40, 41, 41, 
	41, 41, 41, 41, 41, 41, 41, 3, 
	42, 43, 43, 43, 43, 43, 43, 43, 
	43, 43, 3, 44, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 45, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 46, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 46, 3, 44, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 43, 43, 43, 43, 43, 
	43, 43, 43, 43, 43, 3, 3, 45, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	46, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 46, 
	3, 35, 3, 3, 3, 3, 3, 3, 
	3, 3, 36, 3, 3, 3, 34, 34, 
	34, 34, 34, 34, 34, 34, 34, 34, 
	3, 3, 37, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 38, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 38, 3, 1, 0, 0, 0, 
	0, 0, 0, 0, 2, 3, 47, 0, 
	0, 48, 49, 49, 49, 49, 49, 49, 
	49, 49, 49, 0, 0, 4, 5, 0, 
	0, 6, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 7, 8, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 8, 0, 1, 
	0, 0, 0, 0, 0, 0, 0, 2, 
	3, 0, 0, 0, 48, 49, 49, 49, 
	49, 49, 49, 49, 49, 49, 0, 0, 
	4, 5, 0, 0, 6, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	7, 8, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	8, 0, 1, 0, 0, 0, 0, 0, 
	0, 0, 2, 16, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 4, 5, 0, 0, 6, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 7, 8, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 8, 0, 50, 51, 51, 
	51, 51, 51, 51, 51, 51, 51, 3, 
	52, 3, 3, 3, 3, 3, 3, 3, 
	53, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 54, 3, 3, 3, 55, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 56, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 56, 3, 57, 3, 3, 58, 59, 
	59, 59, 59, 59, 59, 59, 59, 59, 
	3, 60, 61, 61, 61, 61, 61, 61, 
	61, 61, 61, 3, 62, 3, 63, 3, 
	3, 64, 65, 65, 65, 65, 65, 65, 
	65, 65, 65, 3, 66, 67, 67, 67, 
	67, 67, 67, 67, 67, 67, 3, 68, 
	3, 3, 3, 3, 3, 3, 3, 69, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	70, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 71, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	71, 3, 68, 3, 3, 3, 3, 3, 
	3, 3, 69, 3, 3, 3, 3, 67, 
	67, 67, 67, 67, 67, 67, 67, 67, 
	67, 3, 3, 70, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 71, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 71, 3, 62, 3, 3, 
	3, 61, 61, 61, 61, 61, 61, 61, 
	61, 61, 61, 3, 52, 3, 3, 3, 
	3, 3, 3, 3, 53, 3, 3, 3, 
	3, 72, 72, 72, 72, 72, 72, 72, 
	72, 72, 72, 3, 3, 54, 3, 3, 
	3, 55, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 56, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 56, 3, 0, 
	0, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 0, 3, 3, 0, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	0, 0, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	0, 3, 1, 0, 0, 0, 0, 0, 
	0, 0, 2, 16, 0, 0, 0, 49, 
	49, 49, 49, 49, 49, 49, 49, 49, 
	49, 0, 0, 4, 5, 0, 0, 6, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 7, 8, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 8, 0, 27, 27, 27, 
	27, 27, 27, 27, 27, 27, 27, 3, 
	3, 3, 3, 28, 3, 24, 3, 3, 
	3, 23, 23, 23, 23, 23, 23, 23, 
	23, 23, 23, 3, 20, 3, 3, 3, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 3, 16, 3, 3, 3, 15, 
	15, 15, 15, 15, 15, 15, 15, 15, 
	15, 3, 73, 73, 73, 73, 73, 73, 
	73, 73, 73, 73, 3, 3, 11, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 12, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 12, 3, 
	75, 74, 74, 74, 74, 74, 74, 74, 
	76, 3, 74, 74, 74, 74, 74, 74, 
	74, 74, 74, 74, 74, 74, 74, 74, 
	74, 77, 78, 74, 74, 79, 74, 74, 
	74, 74, 74, 74, 74, 74, 74, 74, 
	74, 74, 74, 74, 74, 74, 74, 74, 
	74, 74, 74, 74, 74, 74, 74, 74, 
	80, 81, 82, 74, 74, 74, 74, 74, 
	74, 74, 74, 74, 74, 74, 74, 74, 
	74, 74, 74, 74, 74, 74, 74, 74, 
	74, 74, 74, 74, 74, 74, 74, 74, 
	74, 82, 74, 84, 83, 83, 83, 83, 
	83, 83, 83, 85, 3, 83, 83, 83, 
	83, 83, 83, 83, 83, 83, 83, 83, 
	83, 83, 83, 83, 86, 87, 83, 83, 
	88, 83, 83, 83, 83, 83, 83, 83, 
	83, 83, 83, 83, 83, 83, 83, 83, 
	83, 83, 83, 83, 83, 83, 83, 83, 
	83, 83, 83, 83, 89, 90, 83, 83, 
	83, 83, 83, 83, 83, 83, 83, 83, 
	83, 83, 83, 83, 83, 83, 83, 83, 
	83, 83, 83, 83, 83, 83, 83, 83, 
	83, 83, 83, 83, 90, 83, 91, 74, 
	74, 74, 74, 74, 74, 74, 92, 3, 
	74, 74, 74, 74, 74, 74, 74, 74, 
	74, 74, 74, 74, 74, 74, 74, 93, 
	94, 74, 74, 95, 74, 74, 74, 74, 
	74, 74, 74, 74, 74, 74, 74, 74, 
	74, 74, 74, 74, 74, 74, 74, 74, 
	74, 74, 74, 74, 74, 74, 74, 81, 
	96, 74, 74, 74, 74, 74, 74, 74, 
	74, 74, 74, 74, 74, 74, 74, 74, 
	74, 74, 74, 74, 74, 74, 74, 74, 
	74, 74, 74, 74, 74, 74, 74, 96, 
	74, 0
};

સ્થિર અચલ અક્ષર _deserialize_text_glyphs_trans_targs[] = {
	1, 2, 17, 0, 25, 28, 30, 39, 
	47, 3, 45, 4, 47, 5, 6, 44, 
	7, 8, 9, 43, 10, 11, 12, 42, 
	13, 14, 15, 41, 16, 47, 18, 19, 
	24, 19, 24, 2, 20, 4, 47, 21, 
	22, 23, 22, 23, 2, 4, 47, 26, 
	27, 40, 29, 38, 2, 17, 4, 30, 
	47, 31, 32, 37, 32, 37, 33, 34, 
	35, 36, 35, 36, 2, 17, 4, 47, 
	38, 45, 1, 2, 17, 25, 28, 30, 
	48, 39, 47, 1, 2, 17, 25, 28, 
	30, 39, 47, 2, 17, 25, 28, 30, 
	47
};

સ્થિર અચલ અક્ષર _deserialize_text_glyphs_trans_actions[] = {
	0, 1, 1, 0, 1, 1, 1, 0, 
	1, 2, 2, 3, 3, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 2, 2, 
	2, 0, 0, 4, 4, 4, 4, 2, 
	2, 2, 0, 0, 5, 5, 5, 0, 
	0, 0, 2, 2, 6, 6, 6, 6, 
	6, 2, 2, 2, 0, 0, 7, 2, 
	2, 2, 0, 0, 8, 8, 8, 8, 
	0, 0, 9, 10, 10, 10, 10, 10, 
	9, 9, 10, 12, 13, 13, 13, 13, 
	13, 12, 13, 14, 14, 14, 14, 14, 
	14
};

સ્થિર અચલ અક્ષર _deserialize_text_glyphs_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 11, 
	0
};

સ્થિર અચલ પૂર્ણાંક deserialize_text_glyphs_start = 46;
સ્થિર અચલ પૂર્ણાંક deserialize_text_glyphs_first_final = 46;
સ્થિર અચલ પૂર્ણાંક deserialize_text_glyphs_error = 0;

સ્થિર અચલ પૂર્ણાંક deserialize_text_glyphs_en_main = 46;


#line 101 "hb-buffer-deserialize-text-glyphs.rl"


સ્થિર hb_bool_t
_hb_buffer_deserialize_text_glyphs (hb_buffer_t *buffer,
				    અચલ અક્ષર *buf,
				    unsigned પૂર્ણાંક buf_len,
				    અચલ અક્ષર **end_ptr,
				    hb_font_t *font)
{
  અચલ અક્ષર *p = buf, *pe = buf + buf_len, *eof = pe;

  /* Ensure we have positions. */
  (શૂન્ય) hb_buffer_get_glyph_positions (buffer, nullptr);

  અચલ અક્ષર *tok = nullptr;
  પૂર્ણાંક cs;
  hb_glyph_info_t info = {0};
  hb_glyph_position_t pos = {0};
  
#line 386 "hb-buffer-deserialize-text-glyphs.hh"
	{
	cs = deserialize_text_glyphs_start;
	}

#line 389 "hb-buffer-deserialize-text-glyphs.hh"
	{
	પૂર્ણાંક _slen;
	પૂર્ણાંક _trans;
	અચલ unsigned અક્ષર *ચાવીઓ_2;
	અચલ અક્ષર *_inds;
	જો ( p == pe )
		જાઓ _test_eof;
	જો ( cs == 0 )
		જાઓ _out;
_resume:
	ચાવીઓ_2 = deserialize_text_glyphs_trans_ચાવીઓ + (cs<<1);
	_inds = _deserialize_text_glyphs_indicies + _deserialize_text_glyphs_index_offsets[cs];

	_slen = deserialize_text_glyphs_ચાવી_spans[cs];
	_trans = _inds[ _slen > 0 && ચાવીઓ_2[0] <=(*p) &&
		(*p) <= ચાવીઓ_2[1] ?
		(*p) - ચાવીઓ_2[0] : _slen ];

	cs = _deserialize_text_glyphs_trans_targs[_trans];

	જો ( _deserialize_text_glyphs_trans_actions[_trans] == 0 )
		જાઓ _again;

	પસંદગી ( _deserialize_text_glyphs_trans_actions[_trans] ) {
	સ્થિતિ 2:
#line 50 "hb-buffer-deserialize-text-glyphs.rl"
	{
	tok = p;
}
	તોડો;
	સ્થિતિ 1:
#line 54 "hb-buffer-deserialize-text-glyphs.rl"
	{
	/* TODO Unescape delimiters. */
	જો (!hb_font_glyph_from_string (font,
					tok, p - tok,
					&info.codepoint))
	  પાછા ખોટું;
}
	તોડો;
	સ્થિતિ 6:
#line 62 "hb-buffer-deserialize-text-glyphs.rl"
	{ જો (!parse_uint (tok, p, &info.cluster )) પાછા ખોટું; }
	તોડો;
	સ્થિતિ 7:
#line 63 "hb-buffer-deserialize-text-glyphs.rl"
	{ જો (!parse_int  (tok, p, &pos.x_offset )) પાછા ખોટું; }
	તોડો;
	સ્થિતિ 8:
#line 64 "hb-buffer-deserialize-text-glyphs.rl"
	{ જો (!parse_int  (tok, p, &pos.y_offset )) પાછા ખોટું; }
	તોડો;
	સ્થિતિ 4:
#line 65 "hb-buffer-deserialize-text-glyphs.rl"
	{ જો (!parse_int  (tok, p, &pos.x_advance)) પાછા ખોટું; }
	તોડો;
	સ્થિતિ 5:
#line 66 "hb-buffer-deserialize-text-glyphs.rl"
	{ જો (!parse_int  (tok, p, &pos.y_advance)) પાછા ખોટું; }
	તોડો;
	સ્થિતિ 3:
#line 67 "hb-buffer-deserialize-text-glyphs.rl"
	{ જો (!parse_uint (tok, p, &info.mask    )) પાછા ખોટું; }
	તોડો;
	સ્થિતિ 9:
#line 38 "hb-buffer-deserialize-text-glyphs.rl"
	{
	hb_memset (&info, 0, કદ (info));
	hb_memset (&pos , 0, કદ (pos ));
}
#line 50 "hb-buffer-deserialize-text-glyphs.rl"
	{
	tok = p;
}
	તોડો;
	સ્થિતિ 10:
#line 38 "hb-buffer-deserialize-text-glyphs.rl"
	{
	hb_memset (&info, 0, કદ (info));
	hb_memset (&pos , 0, કદ (pos ));
}
#line 50 "hb-buffer-deserialize-text-glyphs.rl"
	{
	tok = p;
}
#line 54 "hb-buffer-deserialize-text-glyphs.rl"
	{
	/* TODO Unescape delimiters. */
	જો (!hb_font_glyph_from_string (font,
					tok, p - tok,
					&info.codepoint))
	  પાછા ખોટું;
}
	તોડો;
	સ્થિતિ 12:
#line 43 "hb-buffer-deserialize-text-glyphs.rl"
	{
	buffer->add_info_and_pos (info, pos);
	જો (unlikely (!buffer->successful))
	  પાછા ખોટું;
	*end_ptr = p;
}
#line 38 "hb-buffer-deserialize-text-glyphs.rl"
	{
	hb_memset (&info, 0, કદ (info));
	hb_memset (&pos , 0, કદ (pos ));
}
#line 50 "hb-buffer-deserialize-text-glyphs.rl"
	{
	tok = p;
}
	તોડો;
	સ્થિતિ 14:
#line 54 "hb-buffer-deserialize-text-glyphs.rl"
	{
	/* TODO Unescape delimiters. */
	જો (!hb_font_glyph_from_string (font,
					tok, p - tok,
					&info.codepoint))
	  પાછા ખોટું;
}
#line 38 "hb-buffer-deserialize-text-glyphs.rl"
	{
	hb_memset (&info, 0, કદ (info));
	hb_memset (&pos , 0, કદ (pos ));
}
#line 50 "hb-buffer-deserialize-text-glyphs.rl"
	{
	tok = p;
}
	તોડો;
	સ્થિતિ 13:
#line 43 "hb-buffer-deserialize-text-glyphs.rl"
	{
	buffer->add_info_and_pos (info, pos);
	જો (unlikely (!buffer->successful))
	  પાછા ખોટું;
	*end_ptr = p;
}
#line 38 "hb-buffer-deserialize-text-glyphs.rl"
	{
	hb_memset (&info, 0, કદ (info));
	hb_memset (&pos , 0, કદ (pos ));
}
#line 50 "hb-buffer-deserialize-text-glyphs.rl"
	{
	tok = p;
}
#line 54 "hb-buffer-deserialize-text-glyphs.rl"
	{
	/* TODO Unescape delimiters. */
	જો (!hb_font_glyph_from_string (font,
					tok, p - tok,
					&info.codepoint))
	  પાછા ખોટું;
}
	તોડો;
#line 523 "hb-buffer-deserialize-text-glyphs.hh"
	}

_again:
	જો ( cs == 0 )
		જાઓ _out;
	જો ( ++p != pe )
		જાઓ _resume;
	_test_eof: {}
	જો ( p == eof )
	{
	પસંદગી ( _deserialize_text_glyphs_eof_actions[cs] ) {
	સ્થિતિ 11:
#line 43 "hb-buffer-deserialize-text-glyphs.rl"
	{
	buffer->add_info_and_pos (info, pos);
	જો (unlikely (!buffer->successful))
	  પાછા ખોટું;
	*end_ptr = p;
}
	તોડો;
#line 542 "hb-buffer-deserialize-text-glyphs.hh"
	}
	}

	_out: {}
	}

#line 122 "hb-buffer-deserialize-text-glyphs.rl"


  *end_ptr = p;

  પાછા p == pe;
}

#endif /* HB_BUFFER_DESERIALIZE_TEXT_GLYPHS_HH */
