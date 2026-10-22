/*
 * Copyright © 2019  Ebrahim Byagowi
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
 */

#ifndef HB_সংখ্যা_HH
#define HB_সংখ্যা_HH

HB_INTERNAL যৌক্তিক
hb_parse_int (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, পূর্ণসংখ্যা *pv,
	      যৌক্তিক whole_বাফার = মিথ্যা);

HB_INTERNAL যৌক্তিক
hb_parse_uint (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, unsigned পূর্ণসংখ্যা *pv,
	       যৌক্তিক whole_বাফার = মিথ্যা, পূর্ণসংখ্যা base = 10);

HB_INTERNAL যৌক্তিক
hb_parse_double (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, দ্বিগুণ *pv,
		 যৌক্তিক whole_বাফার = মিথ্যা);

#endif /* HB_NUMBER_HH */
