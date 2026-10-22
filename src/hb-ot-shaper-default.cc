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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper.hh"


ਅਚਲ hb_ot_shaper_t _hb_ot_shaper_default =
{
  ਖਾਲੀ_ਸੂਚਕ, /* collect_features */
  ਖਾਲੀ_ਸੂਚਕ, /* override_features */
  ਖਾਲੀ_ਸੂਚਕ, /* data_create */
  ਖਾਲੀ_ਸੂਚਕ, /* data_destroy */
  ਖਾਲੀ_ਸੂਚਕ, /* preprocess_text */
  ਖਾਲੀ_ਸੂਚਕ, /* postprocess_glyphs */
  ਖਾਲੀ_ਸੂਚਕ, /* decompose */
  ਖਾਲੀ_ਸੂਚਕ, /* compose */
  ਖਾਲੀ_ਸੂਚਕ, /* setup_masks */
  ਖਾਲੀ_ਸੂਚਕ, /* reorder_marks */
  HB_ਟੈਗ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_DEFAULT,
  HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_BY_GDEF_LATE,
  ਸੱਚ, /* fallback_position */
};

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
/* Same as default but no mark advance zeroing / fallback positioning.
 * Dumbest shaper ever, basically. */
ਅਚਲ hb_ot_shaper_t _hb_ot_shaper_dumber =
{
  ਖਾਲੀ_ਸੂਚਕ, /* collect_features */
  ਖਾਲੀ_ਸੂਚਕ, /* override_features */
  ਖਾਲੀ_ਸੂਚਕ, /* data_create */
  ਖਾਲੀ_ਸੂਚਕ, /* data_destroy */
  ਖਾਲੀ_ਸੂਚਕ, /* preprocess_text */
  ਖਾਲੀ_ਸੂਚਕ, /* postprocess_glyphs */
  ਖਾਲੀ_ਸੂਚਕ, /* decompose */
  ਖਾਲੀ_ਸੂਚਕ, /* compose */
  ਖਾਲੀ_ਸੂਚਕ, /* setup_masks */
  ਖਾਲੀ_ਸੂਚਕ, /* reorder_marks */
  HB_ਟੈਗ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_DEFAULT,
  HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_NONE,
  ਝੂਠ, /* fallback_position */
};
#ਜੇ_ਅੰਤ


#ਜੇ_ਅੰਤ
