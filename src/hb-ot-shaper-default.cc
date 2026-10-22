/*
 * Copyright © 2010,2012  Google, Inc.
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

#ضمن "hb.hh"

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPE

#ضمن "hb-ot-shaper.hh"


ثابت hb_ot_shaper_t _hb_ot_shaper_default =
{
  خالي_اشارو, /* collect_features */
  خالي_اشارو, /* override_features */
  خالي_اشارو, /* data_create */
  خالي_اشارو, /* data_destroy */
  خالي_اشارو, /* preprocess_text */
  خالي_اشارو, /* postprocess_glyphs */
  خالي_اشارو, /* decompose */
  خالي_اشارو, /* compose */
  خالي_اشارو, /* setup_masks */
  خالي_اشارو, /* reorder_marks */
  HB_ٽيگ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_گهڻائي_DEFAULT,
  HB_OT_SHAPE_ZERO_ويڪر_MARKS_BY_GDEF_LATE,
  سچ, /* fallback_position */
};

#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
/* Same as default but no mark advance zeroing / fallback positioning.
 * Dumbest shaper ever, basically. */
ثابت hb_ot_shaper_t _hb_ot_shaper_dumber =
{
  خالي_اشارو, /* collect_features */
  خالي_اشارو, /* override_features */
  خالي_اشارو, /* data_create */
  خالي_اشارو, /* data_destroy */
  خالي_اشارو, /* preprocess_text */
  خالي_اشارو, /* postprocess_glyphs */
  خالي_اشارو, /* decompose */
  خالي_اشارو, /* compose */
  خالي_اشارو, /* setup_masks */
  خالي_اشارو, /* reorder_marks */
  HB_ٽيگ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_گهڻائي_DEFAULT,
  HB_OT_SHAPE_ZERO_ويڪر_MARKS_NONE,
  خطأ, /* fallback_position */
};
#جيڪڏهن_پورو


#جيڪڏهن_پورو
