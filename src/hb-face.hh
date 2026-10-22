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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-shaper.hh"
#அந்தர்பாவ "hb-shape-plan.hh"
#அந்தர்பாவ "hb-ot-face.hh"


/*
 * hb_face_t
 */

#define HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_DATA_INSTANTIATE_SHAPERS(shaper, face);
#அந்தர்பாவ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

கட்டமைப்பு hb_face_t
{
  hb_object_header_t header;

  unsigned முழு index;			/* Face index in a collection, zero-based. */
  mutable hb_atomic_t<unsigned> upem;	/* Units-per-EM. */
  mutable hb_atomic_t<unsigned> இலக்கம்_glyphs;/* Number of glyphs. */

  hb_reference_table_func_t  reference_table_func;
  வெறுமை                      *user_data;
  hb_destroy_func_t          destroy;

  hb_get_table_tags_func_t   get_table_tags_func;
  வெறுமை                      *get_table_tags_user_data;
  hb_destroy_func_t          get_table_tags_destroy;

  hb_shaper_object_dataset_t<hb_face_t> data;/* Various shaper data. */
  hb_ot_face_t table;			/* All the face's tables. */

  /* Cache */
  கட்டமைப்பு plan_முனை_t
  {
    hb_shape_plan_t *shape_plan;
    plan_முனை_t *next;
  };
#ifndef HB_NO_SHAPER
  hb_atomic_t<plan_முனை_t *> shape_plans;
#endif

  hb_blob_t *reference_table (hb_tag_t tag) மாறிலி
  {
    hb_blob_t *blob;

    எனில் (unlikely (!reference_table_func))
      திரும்பு hb_blob_get_empty ();

    blob = reference_table_func (/*Oh, well.*/const_cast<hb_face_t *> (இது), tag, user_data);
    எனில் (unlikely (!blob))
      திரும்பு hb_blob_get_empty ();

    திரும்பு blob;
  }

  unsigned முழு get_upem () மாறிலி
  {
    unsigned முழு ret = upem;
    எனில் (unlikely (!ret))
    {
      திரும்பு load_upem ();
    }
    திரும்பு ret;
  }

  unsigned முழு get_இலக்கம்_glyphs () மாறிலி
  {
    unsigned முழு ret = இலக்கம்_glyphs;
    எனில் (unlikely (ret == UINT_MAX))
      திரும்பு load_இலக்கம்_glyphs ();
    திரும்பு ret;
  }

  தனிப்பட்ட:
  HB_INTERNAL unsigned முழு load_upem () மாறிலி;
  HB_INTERNAL unsigned முழு load_இலக்கம்_glyphs () மாறிலி;
};
DECLARE_NULL_INSTANCE (hb_face_t);


#endif /* HB_FACE_HH */
