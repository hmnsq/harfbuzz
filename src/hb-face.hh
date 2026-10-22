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

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-shaper.hh"
#અન્તર્ભાવ "hb-shape-plan.hh"
#અન્તર્ભાવ "hb-ot-face.hh"


/*
 * hb_face_t
 */

#define HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_DATA_INSTANTIATE_SHAPERS(shaper, face);
#અન્તર્ભાવ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

સંરચના hb_face_t
{
  hb_object_header_t header;

  unsigned પૂર્ણાંક index;			/* Face index in a collection, zero-based. */
  mutable hb_atomic_t<unsigned> upem;	/* Units-per-EM. */
  mutable hb_atomic_t<unsigned> અંક_glyphs;/* Number of glyphs. */

  hb_reference_table_func_t  reference_table_func;
  શૂન્ય                      *user_data;
  hb_destroy_func_t          destroy;

  hb_get_table_tags_func_t   get_table_tags_func;
  શૂન્ય                      *get_table_tags_user_data;
  hb_destroy_func_t          get_table_tags_destroy;

  hb_shaper_object_dataset_t<hb_face_t> data;/* Various shaper data. */
  hb_ot_face_t table;			/* All the face's tables. */

  /* Cache */
  સંરચના plan_ગાંઠ_t
  {
    hb_shape_plan_t *shape_plan;
    plan_ગાંઠ_t *next;
  };
#ifndef HB_NO_SHAPER
  hb_atomic_t<plan_ગાંઠ_t *> shape_plans;
#endif

  hb_blob_t *reference_table (hb_tag_t tag) અચલ
  {
    hb_blob_t *blob;

    જો (unlikely (!reference_table_func))
      પાછા hb_blob_get_empty ();

    blob = reference_table_func (/*Oh, well.*/const_cast<hb_face_t *> (આ), tag, user_data);
    જો (unlikely (!blob))
      પાછા hb_blob_get_empty ();

    પાછા blob;
  }

  unsigned પૂર્ણાંક get_upem () અચલ
  {
    unsigned પૂર્ણાંક ret = upem;
    જો (unlikely (!ret))
    {
      પાછા load_upem ();
    }
    પાછા ret;
  }

  unsigned પૂર્ણાંક get_અંક_glyphs () અચલ
  {
    unsigned પૂર્ણાંક ret = અંક_glyphs;
    જો (unlikely (ret == UINT_MAX))
      પાછા load_અંક_glyphs ();
    પાછા ret;
  }

  ખાનગી:
  HB_INTERNAL unsigned પૂર્ણાંક load_upem () અચલ;
  HB_INTERNAL unsigned પૂર્ણાંક load_અંક_glyphs () અચલ;
};
DECLARE_NULL_INSTANCE (hb_face_t);


#endif /* HB_FACE_HH */
