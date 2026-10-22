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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_FACE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FACE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shape-plan.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-face.hh"


/*
 * hb_face_t
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_ਅੰਕੜੇ_INSTANTIATE_SHAPERS(shaper, face);
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-list.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_SHAPER_IMPLEMENT

ਬਣਤਰ hb_face_t
{
  hb_object_ਸਿਰਲੇਖ_t ਸਿਰਲੇਖ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2;			/* Face index in a collection, zero-based. */
  ਬਦਲਣਯੋਗ hb_atomic_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> upem;	/* Units-per-EM. */
  ਬਦਲਣਯੋਗ hb_atomic_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਅੰਕ_glyphs;/* Number of glyphs. */

  hb_reference_ਸਾਰਣੀ_func_t  reference_ਸਾਰਣੀ_func;
  ਖਾਲੀ                      *user_ਅੰਕੜੇ;
  hb_destroy_func_t          destroy;

  hb_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_func_t   get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_func;
  ਖਾਲੀ                      *get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_user_ਅੰਕੜੇ;
  hb_destroy_func_t          get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_destroy;

  hb_shaper_object_dataset_t<hb_face_t> ਅੰਕੜੇ;/* Various shaper data. */
  hb_ot_face_t ਸਾਰਣੀ;			/* All the face's tables. */

  /* Cache */
  ਬਣਤਰ plan_ਗੰਢ_t
  {
    hb_shape_plan_t *shape_plan;
    plan_ਗੰਢ_t *next;
  };
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SHAPER
  hb_atomic_t<plan_ਗੰਢ_t *> shape_plans;
#ਜੇ_ਅੰਤ

  hb_blob_t *reference_ਸਾਰਣੀ (hb_ਟੈਗ_t ਟੈਗ_2) ਅਚਲ
  {
    hb_blob_t *blob;

    ਜੇ (unlikely (!reference_ਸਾਰਣੀ_func))
      ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();

    blob = reference_ਸਾਰਣੀ_func (/*Oh, well.*/const_cast<hb_face_t *> (ਇਹ), ਟੈਗ_2, user_ਅੰਕੜੇ);
    ਜੇ (unlikely (!blob))
      ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();

    ਵਾਪਸ blob;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_upem () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ret = upem;
    ਜੇ (unlikely (!ret))
    {
      ਵਾਪਸ ਲੱਦੋ_upem ();
    }
    ਵਾਪਸ ret;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਅੰਕ_glyphs () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ret = ਅੰਕ_glyphs;
    ਜੇ (unlikely (ret == UINT_MAX))
      ਵਾਪਸ ਲੱਦੋ_ਅੰਕ_glyphs ();
    ਵਾਪਸ ret;
  }

  ਨਿਜੀ:
  HB_ਅੰਦਰੂਨੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੱਦੋ_upem () ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੱਦੋ_ਅੰਕ_glyphs () ਅਚਲ;
};
DECLARE_NULL_INSTANCE (hb_face_t);


#ਜੇ_ਅੰਤ /* HB_FACE_HH */
