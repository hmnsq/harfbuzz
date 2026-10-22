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

#अगर_अघोषित HB_FACE_HH
#घोषणा HB_FACE_HH

#समावेश "hb.hh"

#समावेश "hb-shaper.hh"
#समावेश "hb-shape-plan.hh"
#समावेश "hb-ot-face.hh"


/*
 * hb_face_t
 */

#घोषणा HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_तथ्य_INSTANTIATE_SHAPERS(shaper, face);
#समावेश "hb-shaper-list.hh"
#अघोषित HB_SHAPER_IMPLEMENT

काष्ठा hb_face_t
{
  hb_object_header_t header;

  अचिन्हित पूर्णांक सूचकांक_2;			/* Face index in a collection, zero-based. */
  mutable hb_atomic_t<अचिन्हित> upem;	/* Units-per-EM. */
  mutable hb_atomic_t<अचिन्हित> अंक_glyphs;/* Number of glyphs. */

  hb_reference_table_func_t  reference_table_func;
  व्योम                      *user_तथ्य;
  hb_destroy_func_t          destroy;

  hb_get_table_tags_func_t   get_table_tags_func;
  व्योम                      *get_table_tags_user_तथ्य;
  hb_destroy_func_t          get_table_tags_destroy;

  hb_shaper_object_dataset_t<hb_face_t> तथ्य;/* Various shaper data. */
  hb_ot_face_t table;			/* All the face's tables. */

  /* Cache */
  काष्ठा plan_गाँठ_t
  {
    hb_shape_plan_t *shape_plan;
    plan_गाँठ_t *next;
  };
#अगर_अघोषित HB_NO_SHAPER
  hb_atomic_t<plan_गाँठ_t *> shape_plans;
#पूर्ण_अगर

  hb_blob_t *reference_table (hb_tag_t tag) स्थिर
  {
    hb_blob_t *blob;

    अगर (unlikely (!reference_table_func))
      वापस hb_blob_get_empty ();

    blob = reference_table_func (/*Oh, well.*/const_cast<hb_face_t *> (यह), tag, user_तथ्य);
    अगर (unlikely (!blob))
      वापस hb_blob_get_empty ();

    वापस blob;
  }

  अचिन्हित पूर्णांक get_upem () स्थिर
  {
    अचिन्हित पूर्णांक ret = upem;
    अगर (unlikely (!ret))
    {
      वापस load_upem ();
    }
    वापस ret;
  }

  अचिन्हित पूर्णांक get_अंक_glyphs () स्थिर
  {
    अचिन्हित पूर्णांक ret = अंक_glyphs;
    अगर (unlikely (ret == अच_पूर्णांक_उच्च))
      वापस load_अंक_glyphs ();
    वापस ret;
  }

  निजी:
  HB_INTERNAL अचिन्हित पूर्णांक load_upem () स्थिर;
  HB_INTERNAL अचिन्हित पूर्णांक load_अंक_glyphs () स्थिर;
};
DECLARE_NULL_INSTANCE (hb_face_t);


#पूर्ण_अगर /* HB_FACE_HH */
