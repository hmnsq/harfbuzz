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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-shaper.hh"
#അന്തര്ഭാവ "hb-shape-plan.hh"
#അന്തര്ഭാവ "hb-ot-face.hh"


/*
 * hb_face_t
 */

#define HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_DATA_INSTANTIATE_SHAPERS(shaper, face);
#അന്തര്ഭാവ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

ഘടന hb_face_t
{
  hb_object_header_t header;

  unsigned പൂർണ്ണസംഖ്യ index;			/* Face index in a collection, zero-based. */
  mutable hb_atomic_t<unsigned> upem;	/* Units-per-EM. */
  mutable hb_atomic_t<unsigned> അക്കം_glyphs;/* Number of glyphs. */

  hb_reference_table_func_t  reference_table_func;
  ശൂന്യം                      *user_data;
  hb_destroy_func_t          destroy;

  hb_get_table_tags_func_t   get_table_tags_func;
  ശൂന്യം                      *get_table_tags_user_data;
  hb_destroy_func_t          get_table_tags_destroy;

  hb_shaper_object_dataset_t<hb_face_t> data;/* Various shaper data. */
  hb_ot_face_t table;			/* All the face's tables. */

  /* Cache */
  ഘടന plan_കണ്ണി_t
  {
    hb_shape_plan_t *shape_plan;
    plan_കണ്ണി_t *next;
  };
#ifndef HB_NO_SHAPER
  hb_atomic_t<plan_കണ്ണി_t *> shape_plans;
#endif

  hb_blob_t *reference_table (hb_tag_t tag) സ്ഥിരം
  {
    hb_blob_t *blob;

    എങ്കിൽ (unlikely (!reference_table_func))
      തിരികെ hb_blob_get_empty ();

    blob = reference_table_func (/*Oh, well.*/const_cast<hb_face_t *> (ഇത്), tag, user_data);
    എങ്കിൽ (unlikely (!blob))
      തിരികെ hb_blob_get_empty ();

    തിരികെ blob;
  }

  unsigned പൂർണ്ണസംഖ്യ get_upem () സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ ret = upem;
    എങ്കിൽ (unlikely (!ret))
    {
      തിരികെ load_upem ();
    }
    തിരികെ ret;
  }

  unsigned പൂർണ്ണസംഖ്യ get_അക്കം_glyphs () സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ ret = അക്കം_glyphs;
    എങ്കിൽ (unlikely (ret == UINT_MAX))
      തിരികെ load_അക്കം_glyphs ();
    തിരികെ ret;
  }

  സ്വകാര്യ:
  HB_INTERNAL unsigned പൂർണ്ണസംഖ്യ load_upem () സ്ഥിരം;
  HB_INTERNAL unsigned പൂർണ്ണസംഖ്യ load_അക്കം_glyphs () സ്ഥിരം;
};
DECLARE_NULL_INSTANCE (hb_face_t);


#endif /* HB_FACE_HH */
