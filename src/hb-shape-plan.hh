/*
 * Copyright © 2012,2018  Google, Inc.
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

#ifndef HB_SHAPE_PLAN_HH
#define HB_SHAPE_PLAN_HH

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-shaper.hh"
#അന്തര്ഭാവ "hb-ot-shape.hh"

ഘടന hb_shape_plan_താക്കോൽ_t
{
  hb_segment_properties_t  props;

  സ്ഥിരം hb_feature_t      *user_features;
  unsigned പൂർണ്ണസംഖ്യ             അക്കം_user_features;

#ifndef HB_NO_OT_SHAPE
  hb_ot_shape_plan_താക്കോൽ_t   ot;
#endif

  hb_shape_func_t         *shaper_func;
  സ്ഥിരം അക്ഷരം              *shaper_name;

  HB_INTERNAL തർക്കം init (തർക്കം                           copy,
			 hb_face_t                     *face,
			 സ്ഥിരം hb_segment_properties_t *props,
			 സ്ഥിരം hb_feature_t            *user_features,
			 unsigned പൂർണ്ണസംഖ്യ                   അക്കം_user_features,
			 സ്ഥിരം പൂർണ്ണസംഖ്യ                     *coords,
			 unsigned പൂർണ്ണസംഖ്യ                   അക്കം_coords,
			 സ്ഥിരം അക്ഷരം * സ്ഥിരം            *shaper_list);

  HB_INTERNAL ശൂന്യം fini ()
  {
    hb_free ((ശൂന്യം *) user_features);
    user_features = nullptr;
#ifndef HB_NO_OT_SHAPE
    ot.fini ();
#endif
  }

  HB_INTERNAL തർക്കം user_features_match (സ്ഥിരം hb_shape_plan_താക്കോൽ_t *other);

  HB_INTERNAL തർക്കം equal (സ്ഥിരം hb_shape_plan_താക്കോൽ_t *other);
};

ഘടന hb_shape_plan_t
{
  HB_INTERNAL ~hb_shape_plan_t ();
  hb_object_header_t header;
  hb_face_t *face_unsafe; /* We don't carry a reference to face. */
  hb_shape_plan_താക്കോൽ_t key;
#ifndef HB_NO_OT_SHAPE
  hb_ot_shape_plan_t ot;
#endif
#ifdef HAVE_HARFRUST
  hb_atomic_t<ശൂന്യം *> harfrust_data;
#endif
};


#endif /* HB_SHAPE_PLAN_HH */
