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

#ifndef HB_OT_SHAPE_FALLBACK_HH
#define HB_OT_SHAPE_FALLBACK_HH

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-ot-shape.hh"


HB_INTERNAL शून्य _hb_ot_shape_fallback_mark_position (अचर hb_ot_shape_plan_t *plan,
						      hb_font_t *font,
						      hb_buffer_t  *buffer,
						      तार्किक adjust_offsets_when_zeroing);

HB_INTERNAL शून्य _hb_ot_shape_fallback_mark_position_recategorize_marks (अचर hb_ot_shape_plan_t *plan,
									 hb_font_t *font,
									 hb_buffer_t  *buffer);


HB_INTERNAL शून्य _hb_ot_shape_fallback_kern (अचर hb_ot_shape_plan_t *plan,
					     hb_font_t *font,
					     hb_buffer_t  *buffer);

HB_INTERNAL शून्य _hb_ot_shape_fallback_spaces (अचर hb_ot_shape_plan_t *plan,
					       hb_font_t *font,
					       hb_buffer_t  *buffer);


#endif /* HB_OT_SHAPE_FALLBACK_HH */
