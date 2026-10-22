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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_SHAPER_INDIC_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_SHAPER_INDIC_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-syllabic.hh"


/* Visual positions in a syllable from left to right. */
ਗਿਣਤੀ ot_position_t {
  POS_ਸ਼ੁਰੂ = 0,

  POS_RA_ਮੰਜ਼ਿਲ_BECOME_REPH = 1,
  POS_PRE_M = 2,
  POS_PRE_C = 3,

  POS_ਅਧਾਰ_C = 4,
  POS_AFTER_MAIN = 5,

  POS_ABOVE_C = 6,

  POS_BEFORE_SUB = 7,
  POS_ਹੇਠਾਂ_C = 8,
  POS_AFTER_SUB = 9,

  POS_BEFORE_POST = 10,
  POS_POST_C = 11,
  POS_AFTER_POST = 12,

  POS_SMVD = 13,

  POS_ਸਿਰਾ = 14
};


HB_ਅੰਦਰੂਨੀ uint16_t
hb_indic_get_categories (hb_codepoint_t u);


#ਜੇ_ਅੰਤ /* HB_OT_SHAPER_INDIC_HH */
