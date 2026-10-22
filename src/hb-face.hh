/*
 * Copyright © 2009  Red Hat, Inc.
 * Copyright © 2011  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_FACE_HH
#define HB_FACE_HH

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-shaper.hh"
#ಅನ್ತರ್ಭಾವ "hb-shape-plan.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-face.hh"


/*
 * hb_face_t
 */

#define HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_DATA_INSTANTIATE_SHAPERS(shaper, face);
#ಅನ್ತರ್ಭಾವ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

ಸಂರಚನಾ hb_face_t
{
  hb_object_header_t header;

  unsigned ಪೂರ್ಣಾಙ್ಕ index;			/* Face index in a collection, zero-based. */
  mutable hb_atomic_t<unsigned> upem;	/* Units-per-EM. */
  mutable hb_atomic_t<unsigned> ಅಂಕೆ_glyphs;/* Number of glyphs. */

  hb_reference_table_func_t  reference_table_func;
  ಶೂನ್ಯ                      *user_data;
  hb_destroy_func_t          destroy;

  hb_get_table_tags_func_t   get_table_tags_func;
  ಶೂನ್ಯ                      *get_table_tags_user_data;
  hb_destroy_func_t          get_table_tags_destroy;

  hb_shaper_object_dataset_t<hb_face_t> data;/* Various shaper data. */
  hb_ot_face_t table;			/* All the face's tables. */

  /* Cache */
  ಸಂರಚನಾ plan_ಗಂಟು_t
  {
    hb_shape_plan_t *shape_plan;
    plan_ಗಂಟು_t *next;
  };
#ifndef HB_NO_SHAPER
  hb_atomic_t<plan_ಗಂಟು_t *> shape_plans;
#endif

  hb_blob_t *reference_table (hb_tag_t tag) ಧ್ರುವ
  {
    hb_blob_t *blob;

    ಯದಿ (unlikely (!reference_table_func))
      ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();

    blob = reference_table_func (/*Oh, well.*/const_cast<hb_face_t *> (ಏತತ್), tag, user_data);
    ಯದಿ (unlikely (!blob))
      ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();

    ಪ್ರತ್ಯಾಗಮ blob;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_upem () ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ret = upem;
    ಯದಿ (unlikely (!ret))
    {
      ಪ್ರತ್ಯಾಗಮ load_upem ();
    }
    ಪ್ರತ್ಯಾಗಮ ret;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_ಅಂಕೆ_glyphs () ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ret = ಅಂಕೆ_glyphs;
    ಯದಿ (unlikely (ret == UINT_MAX))
      ಪ್ರತ್ಯಾಗಮ load_ಅಂಕೆ_glyphs ();
    ಪ್ರತ್ಯಾಗಮ ret;
  }

  ಗುಪ್ತ:
  HB_INTERNAL unsigned ಪೂರ್ಣಾಙ್ಕ load_upem () ಧ್ರುವ;
  HB_INTERNAL unsigned ಪೂರ್ಣಾಙ್ಕ load_ಅಂಕೆ_glyphs () ಧ್ರುವ;
};
DECLARE_NULL_INSTANCE (hb_face_t);


#endif /* HB_FACE_HH */
