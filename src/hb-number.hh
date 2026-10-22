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

#ifndef HB_സംഖ്യ_HH
#define HB_സംഖ്യ_HH

HB_INTERNAL തർക്കം
hb_parse_int (സ്ഥിരം അക്ഷരം **pp, സ്ഥിരം അക്ഷരം *end, പൂർണ്ണസംഖ്യ *pv,
	      തർക്കം whole_buffer = അസത്യം);

HB_INTERNAL തർക്കം
hb_parse_uint (സ്ഥിരം അക്ഷരം **pp, സ്ഥിരം അക്ഷരം *end, unsigned പൂർണ്ണസംഖ്യ *pv,
	       തർക്കം whole_buffer = അസത്യം, പൂർണ്ണസംഖ്യ base = 10);

HB_INTERNAL തർക്കം
hb_parse_double (സ്ഥിരം അക്ഷരം **pp, സ്ഥിരം അക്ഷരം *end, ഇരട്ട *pv,
		 തർക്കം whole_buffer = അസത്യം);

#endif /* HB_NUMBER_HH */
