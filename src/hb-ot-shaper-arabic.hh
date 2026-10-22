/*
 * Copyright © 2015  Mozilla Foundation.
 * Copyright © 2015  Google, Inc.
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
 * Mozilla Author(s): Jonathan Kew
 * Google Author(s): Behdad Esfahbod
 */

#جے_بنا_تعریف HB_OT_SHAPER_ARABIC_HH
#تعریف_کرو HB_OT_SHAPER_ARABIC_HH

#ضمن "hb.hh"

#ضمن "hb-ot-shaper.hh"


بنية arabic_shape_plan_t;

HB_اندرونی فراغ *
انکڑے_create_arabic (ثابت hb_ot_shape_plan_t *plan);

HB_اندرونی فراغ
انکڑے_destroy_arabic (فراغ *انکڑے);

HB_اندرونی فراغ
تیاری_masks_arabic_plan (ثابت arabic_shape_plan_t *arabic_plan,
			 hb_بفر_t               *بفر,
			 hb_script_t                script);

#جے_انت /* HB_OT_SHAPER_ARABIC_HH */
