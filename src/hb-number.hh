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

#अगर_अघोषित HB_संख्या_HH
#घोषणा HB_संख्या_HH

HB_INTERNAL bool
hb_parse_int (स्थिर अक्षर **pp, स्थिर अक्षर *end, पूर्णांक *pv,
	      bool whole_बफर = false);

HB_INTERNAL bool
hb_parse_uint (स्थिर अक्षर **pp, स्थिर अक्षर *end, अचिन्हित पूर्णांक *pv,
	       bool whole_बफर = false, पूर्णांक base = 10);

HB_INTERNAL bool
hb_parse_double (स्थिर अक्षर **pp, स्थिर अक्षर *end, द्विगुन *pv,
		 bool whole_बफर = false);

#पूर्ण_अगर /* HB_NUMBER_HH */
