/*
 * Copyright © 2012  Google, Inc.
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_SHAPE_FALLBACK_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_SHAPE_FALLBACK_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shape.hh"


HB_ਅੰਦਰੂਨੀ ਖਾਲੀ _hb_ot_shape_fallback_mark_position (ਅਚਲ hb_ot_shape_plan_t *plan,
						      hb_font_t *font,
						      hb_ਬਫਰ_t  *ਬਫਰ,
						      ਤਾਰਕਿਕ adjust_offsets_when_zeroing);

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ _hb_ot_shape_fallback_mark_position_recategorize_marks (ਅਚਲ hb_ot_shape_plan_t *plan,
									 hb_font_t *font,
									 hb_ਬਫਰ_t  *ਬਫਰ);


HB_ਅੰਦਰੂਨੀ ਖਾਲੀ _hb_ot_shape_fallback_kern (ਅਚਲ hb_ot_shape_plan_t *plan,
					     hb_font_t *font,
					     hb_ਬਫਰ_t  *ਬਫਰ);

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ _hb_ot_shape_fallback_spaces (ਅਚਲ hb_ot_shape_plan_t *plan,
					       hb_font_t *font,
					       hb_ਬਫਰ_t  *ਬਫਰ);


#ਜੇ_ਅੰਤ /* HB_OT_SHAPE_FALLBACK_HH */
