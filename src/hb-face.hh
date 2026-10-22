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

#include "hb.hh"

#include "hb-shaper.hh"
#include "hb-shape-plan.hh"
#include "hb-ot-face.hh"


/*
 * hb_face_t
 */

#define HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_DATA_INSTANTIATE_SHAPERS(shaper, face);
#include "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

sa_nracanaa hb_face_t
{
  hb_object_header_t header;

  unsigned ຈຳນວນເຕັມ index;			/* Face index in a collection, zero-based. */
  mutable hb_atomic_t<unsigned> upem;	/* Units-per-EM. */
  mutable hb_atomic_t<unsigned> num_glyphs;/* Number of glyphs. */

  hb_reference_table_func_t  reference_table_func;
  void                      *user_data;
  hb_destroy_func_t          destroy;

  hb_get_table_tags_func_t   get_table_tags_func;
  void                      *get_table_tags_user_data;
  hb_destroy_func_t          get_table_tags_destroy;

  hb_shaper_object_dataset_t<hb_face_t> data;/* Various shaper data. */
  hb_ot_face_t table;			/* All the face's tables. */

  /* Cache */
  sa_nracanaa plan_node_t
  {
    hb_shape_plan_t *shape_plan;
    plan_node_t *next;
  };
#ifndef HB_NO_SHAPER
  hb_atomic_t<plan_node_t *> shape_plans;
#endif

  hb_blob_t *reference_table (hb_tag_t tag) const
  {
    hb_blob_t *blob;

    ຖ້າ (unlikely (!reference_table_func))
      ສົ່ງຄືນ hb_blob_get_empty ();

    blob = reference_table_func (/*Oh, well.*/const_cast<hb_face_t *> (this), tag, user_data);
    ຖ້າ (unlikely (!blob))
      ສົ່ງຄືນ hb_blob_get_empty ();

    ສົ່ງຄືນ blob;
  }

  unsigned ຈຳນວນເຕັມ get_upem () const
  {
    unsigned ຈຳນວນເຕັມ ret = upem;
    ຖ້າ (unlikely (!ret))
    {
      ສົ່ງຄືນ load_upem ();
    }
    ສົ່ງຄືນ ret;
  }

  unsigned ຈຳນວນເຕັມ get_num_glyphs () const
  {
    unsigned ຈຳນວນເຕັມ ret = num_glyphs;
    ຖ້າ (unlikely (ret == UINT_MAX))
      ສົ່ງຄືນ load_num_glyphs ();
    ສົ່ງຄືນ ret;
  }

  private:
  HB_INTERNAL unsigned ຈຳນວນເຕັມ load_upem () const;
  HB_INTERNAL unsigned ຈຳນວນເຕັມ load_num_glyphs () const;
};
DECLARE_NULL_INSTANCE (hb_face_t);


#endif /* HB_FACE_HH */
