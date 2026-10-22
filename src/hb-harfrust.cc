/*
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
 * Author(s): Behdad Esfahbod
 */

#समावेश "hb.hh"

#अगर_घोषित HAVE_HARFRUST

#समावेश "hb-shaper-impl.hh"

#समावेश "hb-utf.hh"


/*
 * buffer
 */
बाह्य "C" व्योम *
hb_harfrust_बफर_create_rs (व्योम);

बाह्य "C" व्योम
hb_harfrust_बफर_destroy_rs (व्योम *तथ्य);


/*
 * shaper face data
 */

बाह्य "C" व्योम *
hb_harfrust_shaper_face_तथ्य_create_rs (hb_face_t *face);

hb_harfrust_face_तथ्य_t *
hb_harfrust_shaper_face_तथ्य_create (hb_face_t *face)
{
  वापस (hb_harfrust_face_तथ्य_t *) hb_harfrust_shaper_face_तथ्य_create_rs (face);
}

बाह्य "C" व्योम
hb_harfrust_shaper_face_तथ्य_destroy_rs (व्योम *तथ्य);

व्योम
hb_harfrust_shaper_face_तथ्य_destroy (hb_harfrust_face_तथ्य_t *तथ्य)
{
  hb_harfrust_shaper_face_तथ्य_destroy_rs (तथ्य);
}


/*
 * shaper font data
 */

बाह्य "C" व्योम *
hb_harfrust_shaper_font_तथ्य_create_rs (hb_font_t *font, स्थिर व्योम *face_तथ्य);

hb_harfrust_font_तथ्य_t *
hb_harfrust_shaper_font_तथ्य_create (hb_font_t *font)
{
  स्थिर hb_harfrust_face_तथ्य_t *face_तथ्य = font->face->तथ्य.harfrust;
  वापस (hb_harfrust_font_तथ्य_t *) hb_harfrust_shaper_font_तथ्य_create_rs (font, face_तथ्य);
}

बाह्य "C" व्योम
hb_harfrust_shaper_font_तथ्य_destroy_rs (व्योम *तथ्य);

व्योम
hb_harfrust_shaper_font_तथ्य_destroy (hb_harfrust_font_तथ्य_t *तथ्य)
{
  hb_harfrust_shaper_font_तथ्य_destroy_rs (तथ्य);
}


/*
 * shape plan
 */

बाह्य "C" व्योम *
_hb_harfrust_shape_plan_create_rs (स्थिर व्योम *font_तथ्य,
				   hb_script_t script,
				   hb_language_t language,
				   hb_direction_t direction,
				   स्थिर hb_feature_t *features,
				   अचिन्हित पूर्णांक        अंक_features);

बाह्य "C" व्योम
_hb_harfrust_shape_plan_destroy_rs (व्योम *तथ्य);


/*
 * shaper
 */

बाह्य "C" hb_bool_t
_hb_harfrust_shape_rs (स्थिर व्योम         *font_तथ्य,
		       स्थिर व्योम         *rs_shape_plan,
		       स्थिर व्योम         *rs_बफर,
		       hb_font_t          *font,
		       hb_बफर_t        *बफर,
		       स्थिर hb_codepoint_t *pre_context,
		       uint32_t            pre_context_len,
		       स्थिर hb_codepoint_t *post_context,
		       uint32_t            post_context_len,
		       स्थिर hb_feature_t *features,
		       अचिन्हित पूर्णांक        अंक_features);

अटल hb_user_तथ्य_कुंजी_t hb_बफर_कुंजी = {0};

hb_bool_t
_hb_harfrust_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_बफर_t        *बफर,
		    स्थिर hb_feature_t *features,
		    अचिन्हित पूर्णांक        अंक_features)
{
  स्थिर hb_harfrust_font_तथ्य_t *font_तथ्य = font->तथ्य.harfrust;

retry_बफर:
  व्योम *hr_बफर = hb_बफर_get_user_तथ्य (बफर, &hb_बफर_कुंजी);
  अगर (unlikely (!hr_बफर))
  {
    hr_बफर = hb_harfrust_बफर_create_rs ();
    अगर (unlikely (!hr_बफर))
      वापस false;

    अगर (!hb_बफर_set_user_तथ्य (बफर,
				  &hb_बफर_कुंजी,
				  hr_बफर,
				  hb_harfrust_बफर_destroy_rs,
				  false))
    {
      hb_harfrust_बफर_destroy_rs (hr_बफर);
      जाओ retry_बफर;
    }
  }

  व्योम *hr_shape_plan = nullptr;

retry_shape_plan:
  hr_shape_plan = shape_plan->harfrust_तथ्य.get_acquire ();
  अगर (unlikely (!hr_shape_plan))
  {
    hr_shape_plan = _hb_harfrust_shape_plan_create_rs (font_तथ्य,
						       shape_plan->key.props.script,
						       shape_plan->key.props.language,
						       shape_plan->key.props.direction,
						       features, अंक_features);
    अगर (hr_shape_plan &&
	!shape_plan->harfrust_तथ्य.cmpexch (nullptr, hr_shape_plan))
    {
      _hb_harfrust_shape_plan_destroy_rs (hr_shape_plan);
      जाओ retry_shape_plan;
    }
  }

  वापस _hb_harfrust_shape_rs (font_तथ्य,
				hr_shape_plan,
				hr_बफर,
				font,
				बफर,
				बफर->context[0],
				बफर->context_len[0],
				बफर->context[1],
				बफर->context_len[1],
				features,
				अंक_features);
}


#पूर्ण_अगर
