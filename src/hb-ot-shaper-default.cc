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

#که_نه_تعریف_شوی HB_NO_OT_SHAPE

#ضمن "hb-ot-shaper.hh"


ثابت hb_ot_shaper_t _hb_ot_shaper_default =
{
  تش_اشاره, /* collect_features */
  تش_اشاره, /* override_features */
  تش_اشاره, /* data_create */
  تش_اشاره, /* data_destroy */
  تش_اشاره, /* preprocess_text */
  تش_اشاره, /* postprocess_glyphs */
  تش_اشاره, /* decompose */
  تش_اشاره, /* compose */
  تش_اشاره, /* setup_masks */
  تش_اشاره, /* reorder_marks */
  HB_نښان_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_ډېرپېښ_DEFAULT,
  HB_OT_SHAPE_ZERO_پلنوالی_MARKS_BY_GDEF_LATE,
  ریښتیا, /* fallback_position */
};

#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
/* Same as default but no mark advance zeroing / fallback positioning.
 * Dumbest shaper ever, basically. */
ثابت hb_ot_shaper_t _hb_ot_shaper_dumber =
{
  تش_اشاره, /* collect_features */
  تش_اشاره, /* override_features */
  تش_اشاره, /* data_create */
  تش_اشاره, /* data_destroy */
  تش_اشاره, /* preprocess_text */
  تش_اشاره, /* postprocess_glyphs */
  تش_اشاره, /* decompose */
  تش_اشاره, /* compose */
  تش_اشاره, /* setup_masks */
  تش_اشاره, /* reorder_marks */
  HB_نښان_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_ډېرپېښ_DEFAULT,
  HB_OT_SHAPE_ZERO_پلنوالی_MARKS_NONE,
  خطأ, /* fallback_position */
};
#د_که_پای


#د_که_پای
