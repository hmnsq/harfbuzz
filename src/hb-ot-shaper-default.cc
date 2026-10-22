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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE

#అన్తర్భావ "hb-ot-shaper.hh"


స్థిరరాశి hb_ot_shaper_t _hb_ot_shaper_default =
{
  శూన్యసూచిక, /* collect_features */
  శూన్యసూచిక, /* override_features */
  శూన్యసూచిక, /* data_create */
  శూన్యసూచిక, /* data_destroy */
  శూన్యసూచిక, /* preprocess_text */
  శూన్యసూచిక, /* postprocess_glyphs */
  శూన్యసూచిక, /* decompose */
  శూన్యసూచిక, /* compose */
  శూన్యసూచిక, /* setup_masks */
  శూన్యసూచిక, /* reorder_marks */
  HB_ట్యాగ్_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_బహుళకం_DEFAULT,
  HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_BY_GDEF_LATE,
  నిజం, /* fallback_position */
};

#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
/* Same as default but no mark advance zeroing / fallback positioning.
 * Dumbest shaper ever, basically. */
స్థిరరాశి hb_ot_shaper_t _hb_ot_shaper_dumber =
{
  శూన్యసూచిక, /* collect_features */
  శూన్యసూచిక, /* override_features */
  శూన్యసూచిక, /* data_create */
  శూన్యసూచిక, /* data_destroy */
  శూన్యసూచిక, /* preprocess_text */
  శూన్యసూచిక, /* postprocess_glyphs */
  శూన్యసూచిక, /* decompose */
  శూన్యసూచిక, /* compose */
  శూన్యసూచిక, /* setup_masks */
  శూన్యసూచిక, /* reorder_marks */
  HB_ట్యాగ్_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_బహుళకం_DEFAULT,
  HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_NONE,
  అబద్ధం, /* fallback_position */
};
#అయితే_ముగింపు


#అయితే_ముగింపు
