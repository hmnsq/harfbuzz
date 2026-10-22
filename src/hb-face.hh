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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-shaper.hh"
#অন্তর্ভাব "hb-shape-plan.hh"
#অন্তর্ভাব "hb-ot-face.hh"


/*
 * hb_face_t
 */

#define HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_তথ্য_INSTANTIATE_SHAPERS(shaper, face);
#অন্তর্ভাব "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

কাঠামো hb_face_t
{
  hb_object_header_t header;

  unsigned পূর্ণসংখ্যা সূচক_2;			/* Face index in a collection, zero-based. */
  mutable hb_atomic_t<unsigned> upem;	/* Units-per-EM. */
  mutable hb_atomic_t<unsigned> অঙ্ক_glyphs;/* Number of glyphs. */

  hb_reference_table_func_t  reference_table_func;
  শূন্য                      *user_তথ্য;
  hb_destroy_func_t          destroy;

  hb_get_table_tags_func_t   get_table_tags_func;
  শূন্য                      *get_table_tags_user_তথ্য;
  hb_destroy_func_t          get_table_tags_destroy;

  hb_shaper_object_dataset_t<hb_face_t> তথ্য;/* Various shaper data. */
  hb_ot_face_t table;			/* All the face's tables. */

  /* Cache */
  কাঠামো plan_গ্রন্থি_t
  {
    hb_shape_plan_t *shape_plan;
    plan_গ্রন্থি_t *next;
  };
#ifndef HB_NO_SHAPER
  hb_atomic_t<plan_গ্রন্থি_t *> shape_plans;
#endif

  hb_blob_t *reference_table (hb_tag_t tag) ধ্রুবক
  {
    hb_blob_t *blob;

    যদি (unlikely (!reference_table_func))
      ফেরত hb_blob_get_empty ();

    blob = reference_table_func (/*Oh, well.*/const_cast<hb_face_t *> (এই), tag, user_তথ্য);
    যদি (unlikely (!blob))
      ফেরত hb_blob_get_empty ();

    ফেরত blob;
  }

  unsigned পূর্ণসংখ্যা get_upem () ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা ret = upem;
    যদি (unlikely (!ret))
    {
      ফেরত load_upem ();
    }
    ফেরত ret;
  }

  unsigned পূর্ণসংখ্যা get_অঙ্ক_glyphs () ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা ret = অঙ্ক_glyphs;
    যদি (unlikely (ret == UINT_MAX))
      ফেরত load_অঙ্ক_glyphs ();
    ফেরত ret;
  }

  ব্যক্তিগত:
  HB_INTERNAL unsigned পূর্ণসংখ্যা load_upem () ধ্রুবক;
  HB_INTERNAL unsigned পূর্ণসংখ্যা load_অঙ্ক_glyphs () ধ্রুবক;
};
DECLARE_NULL_INSTANCE (hb_face_t);


#endif /* HB_FACE_HH */
