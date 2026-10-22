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

#ifndef HB_ସଂଖ୍ୟା_HH
#define HB_ସଂଖ୍ୟା_HH

HB_INTERNAL ତାର୍କିକ
hb_parse_int (ଅଚଳ ଅକ୍ଷର **pp, ଅଚଳ ଅକ୍ଷର *end, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *pv,
	      ତାର୍କିକ whole_buffer = ମିଥ୍ୟା);

HB_INTERNAL ତାର୍କିକ
hb_parse_uint (ଅଚଳ ଅକ୍ଷର **pp, ଅଚଳ ଅକ୍ଷର *end, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *pv,
	       ତାର୍କିକ whole_buffer = ମିଥ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା base = 10);

HB_INTERNAL ତାର୍କିକ
hb_parse_double (ଅଚଳ ଅକ୍ଷର **pp, ଅଚଳ ଅକ୍ଷର *end, ଦ୍ୱିଗୁଣ *pv,
		 ତାର୍କିକ whole_buffer = ମିଥ୍ୟା);

#endif /* HB_NUMBER_HH */
