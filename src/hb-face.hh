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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-shaper.hh"
#ଅନ୍ତର୍ଭାଵ "hb-shape-plan.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-face.hh"


/*
 * hb_face_t
 */

#define HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_DATA_INSTANTIATE_SHAPERS(shaper, face);
#ଅନ୍ତର୍ଭାଵ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

ସଂରଚନା hb_face_t
{
  hb_object_header_t header;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index;			/* Face index in a collection, zero-based. */
  mutable hb_atomic_t<unsigned> upem;	/* Units-per-EM. */
  mutable hb_atomic_t<unsigned> ଅଙ୍କ_glyphs;/* Number of glyphs. */

  hb_reference_table_func_t  reference_table_func;
  ଶୂନ୍ୟ                      *user_data;
  hb_destroy_func_t          destroy;

  hb_get_table_tags_func_t   get_table_tags_func;
  ଶୂନ୍ୟ                      *get_table_tags_user_data;
  hb_destroy_func_t          get_table_tags_destroy;

  hb_shaper_object_dataset_t<hb_face_t> data;/* Various shaper data. */
  hb_ot_face_t table;			/* All the face's tables. */

  /* Cache */
  ସଂରଚନା plan_ଗଣ୍ଠି_t
  {
    hb_shape_plan_t *shape_plan;
    plan_ଗଣ୍ଠି_t *next;
  };
#ifndef HB_NO_SHAPER
  hb_atomic_t<plan_ଗଣ୍ଠି_t *> shape_plans;
#endif

  hb_blob_t *reference_table (hb_tag_t tag) ଅଚଳ
  {
    hb_blob_t *blob;

    ଯଦି (unlikely (!reference_table_func))
      ଫେରନ୍ତୁ hb_blob_get_empty ();

    blob = reference_table_func (/*Oh, well.*/const_cast<hb_face_t *> (ଏହି), tag, user_data);
    ଯଦି (unlikely (!blob))
      ଫେରନ୍ତୁ hb_blob_get_empty ();

    ଫେରନ୍ତୁ blob;
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_upem () ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = upem;
    ଯଦି (unlikely (!ret))
    {
      ଫେରନ୍ତୁ load_upem ();
    }
    ଫେରନ୍ତୁ ret;
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ଅଙ୍କ_glyphs () ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = ଅଙ୍କ_glyphs;
    ଯଦି (unlikely (ret == UINT_MAX))
      ଫେରନ୍ତୁ load_ଅଙ୍କ_glyphs ();
    ଫେରନ୍ତୁ ret;
  }

  ବ୍ୟକ୍ତିଗତ:
  HB_INTERNAL unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା load_upem () ଅଚଳ;
  HB_INTERNAL unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା load_ଅଙ୍କ_glyphs () ଅଚଳ;
};
DECLARE_NULL_INSTANCE (hb_face_t);


#endif /* HB_FACE_HH */
