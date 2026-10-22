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

#اگر_تعریف_نشده HB_NO_OT_SHAPE

#ضمن "hb-ot-shaper.hh"


ثابت hb_ot_shaper_t _hb_ot_shaper_default =
{
  اشاره_تهی, /* collect_features */
  اشاره_تهی, /* override_features */
  اشاره_تهی, /* data_create */
  اشاره_تهی, /* data_destroy */
  اشاره_تهی, /* preprocess_text */
  اشاره_تهی, /* postprocess_glyphs */
  اشاره_تهی, /* decompose */
  اشاره_تهی, /* compose */
  اشاره_تهی, /* setup_masks */
  اشاره_تهی, /* reorder_marks */
  HB_برچسب_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_نما_DEFAULT,
  HB_OT_SHAPE_ZERO_عرض_MARKS_BY_GDEF_LATE,
  درست, /* fallback_position */
};

#اگر_تعریف_نشده HB_NO_AAT_SHAPE
/* Same as default but no mark advance zeroing / fallback positioning.
 * Dumbest shaper ever, basically. */
ثابت hb_ot_shaper_t _hb_ot_shaper_dumber =
{
  اشاره_تهی, /* collect_features */
  اشاره_تهی, /* override_features */
  اشاره_تهی, /* data_create */
  اشاره_تهی, /* data_destroy */
  اشاره_تهی, /* preprocess_text */
  اشاره_تهی, /* postprocess_glyphs */
  اشاره_تهی, /* decompose */
  اشاره_تهی, /* compose */
  اشاره_تهی, /* setup_masks */
  اشاره_تهی, /* reorder_marks */
  HB_برچسب_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_نما_DEFAULT,
  HB_OT_SHAPE_ZERO_عرض_MARKS_NONE,
  خطأ, /* fallback_position */
};
#پایان_اگر


#پایان_اگر
