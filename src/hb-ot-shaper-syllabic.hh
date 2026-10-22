/*
 * Copyright © 2021  Behdad Esfahbod.
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
 */

#ifndef HB_OT_SHAPER_SYLLABIC_HH
#define HB_OT_SHAPER_SYLLABIC_HH

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-ot-shaper.hh"


HB_INTERNAL તાર્કિક
hb_syllabic_ઉમેરો_dotted_circles (hb_font_t *font,
				   hb_buffer_t *buffer,
				   unsigned પૂર્ણાંક broken_syllable_type,
				   unsigned પૂર્ણાંક dottedcircle_category,
				   પૂર્ણાંક repha_category = -1,
				   પૂર્ણાંક dottedcircle_position = -1);

HB_INTERNAL તાર્કિક
hb_syllabic_clear_var (અચલ hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t *buffer);


#endif /* HB_OT_SHAPER_SYLLABIC_HH */
