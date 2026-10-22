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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifdef HAVE_HARFRUST

#ಅನ್ತರ್ಭಾವ "hb-shaper-impl.hh"

#ಅನ್ತರ್ಭಾವ "hb-utf.hh"


/*
 * buffer
 */
ಬಾಹ್ಯ "C" ಶೂನ್ಯ *
_hb_harfrust_buffer_create_rs (ಶೂನ್ಯ);

ಬಾಹ್ಯ "C" ಶೂನ್ಯ
_hb_harfrust_buffer_destroy_rs (ಶೂನ್ಯ *data);


/*
 * shaper face data
 */

ಬಾಹ್ಯ "C" ಶೂನ್ಯ *
_hb_harfrust_shaper_face_data_create_rs (hb_face_t *face);

hb_harfrust_face_data_t *
_hb_harfrust_shaper_face_data_create (hb_face_t *face)
{
  ಪ್ರತ್ಯಾಗಮ (hb_harfrust_face_data_t *) _hb_harfrust_shaper_face_data_create_rs (face);
}

ಬಾಹ್ಯ "C" ಶೂನ್ಯ
_hb_harfrust_shaper_face_data_destroy_rs (ಶೂನ್ಯ *data);

ಶೂನ್ಯ
_hb_harfrust_shaper_face_data_destroy (hb_harfrust_face_data_t *data)
{
  _hb_harfrust_shaper_face_data_destroy_rs (data);
}


/*
 * shaper font data
 */

ಬಾಹ್ಯ "C" ಶೂನ್ಯ *
_hb_harfrust_shaper_font_data_create_rs (hb_font_t *font, ಧ್ರುವ ಶೂನ್ಯ *face_data);

hb_harfrust_font_data_t *
_hb_harfrust_shaper_font_data_create (hb_font_t *font)
{
  ಧ್ರುವ hb_harfrust_face_data_t *face_data = font->face->data.harfrust;
  ಪ್ರತ್ಯಾಗಮ (hb_harfrust_font_data_t *) _hb_harfrust_shaper_font_data_create_rs (font, face_data);
}

ಬಾಹ್ಯ "C" ಶೂನ್ಯ
_hb_harfrust_shaper_font_data_destroy_rs (ಶೂನ್ಯ *data);

ಶೂನ್ಯ
_hb_harfrust_shaper_font_data_destroy (hb_harfrust_font_data_t *data)
{
  _hb_harfrust_shaper_font_data_destroy_rs (data);
}


/*
 * shape plan
 */

ಬಾಹ್ಯ "C" ಶೂನ್ಯ *
_hb_harfrust_shape_plan_create_rs (ಧ್ರುವ ಶೂನ್ಯ *font_data,
				   hb_script_t script,
				   hb_language_t language,
				   hb_direction_t direction,
				   ಧ್ರುವ hb_feature_t *features,
				   unsigned ಪೂರ್ಣಾಙ್ಕ        ಅಂಕೆ_features);

ಬಾಹ್ಯ "C" ಶೂನ್ಯ
_hb_harfrust_shape_plan_destroy_rs (ಶೂನ್ಯ *data);


/*
 * shaper
 */

ಬಾಹ್ಯ "C" hb_bool_t
_hb_harfrust_shape_rs (ಧ್ರುವ ಶೂನ್ಯ         *font_data,
		       ಧ್ರುವ ಶೂನ್ಯ         *rs_shape_plan,
		       ಧ್ರುವ ಶೂನ್ಯ         *rs_buffer,
		       hb_font_t          *font,
		       hb_buffer_t        *buffer,
		       ಧ್ರುವ hb_codepoint_t *pre_context,
		       uint32_t            pre_context_len,
		       ಧ್ರುವ hb_codepoint_t *post_context,
		       uint32_t            post_context_len,
		       ಧ್ರುವ hb_feature_t *features,
		       unsigned ಪೂರ್ಣಾಙ್ಕ        ಅಂಕೆ_features);

ಸ್ಥಿರ hb_user_data_ಕೀಲಿ_t hb_buffer_ಕೀಲಿ = {0};

hb_bool_t
_hb_harfrust_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_buffer_t        *buffer,
		    ಧ್ರುವ hb_feature_t *features,
		    unsigned ಪೂರ್ಣಾಙ್ಕ        ಅಂಕೆ_features)
{
  ಧ್ರುವ hb_harfrust_font_data_t *font_data = font->data.harfrust;

retry_buffer:
  ಶೂನ್ಯ *hr_buffer = hb_buffer_get_user_data (buffer, &hb_buffer_ಕೀಲಿ);
  ಯದಿ (unlikely (!hr_buffer))
  {
    hr_buffer = _hb_harfrust_buffer_create_rs ();
    ಯದಿ (unlikely (!hr_buffer))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (!hb_buffer_set_user_data (buffer,
				  &hb_buffer_ಕೀಲಿ,
				  hr_buffer,
				  _hb_harfrust_buffer_destroy_rs,
				  ಅಸತ್ಯ))
    {
      _hb_harfrust_buffer_destroy_rs (hr_buffer);
      ಗಚ್ಛ retry_buffer;
    }
  }

  ಶೂನ್ಯ *hr_shape_plan = nullptr;

retry_shape_plan:
  hr_shape_plan = shape_plan->harfrust_data.get_acquire ();
  ಯದಿ (unlikely (!hr_shape_plan))
  {
    hr_shape_plan = _hb_harfrust_shape_plan_create_rs (font_data,
						       shape_plan->key.props.script,
						       shape_plan->key.props.language,
						       shape_plan->key.props.direction,
						       features, ಅಂಕೆ_features);
    ಯದಿ (hr_shape_plan &&
	!shape_plan->harfrust_data.cmpexch (nullptr, hr_shape_plan))
    {
      _hb_harfrust_shape_plan_destroy_rs (hr_shape_plan);
      ಗಚ್ಛ retry_shape_plan;
    }
  }

  ಪ್ರತ್ಯಾಗಮ _hb_harfrust_shape_rs (font_data,
				hr_shape_plan,
				hr_buffer,
				font,
				buffer,
				buffer->context[0],
				buffer->context_len[0],
				buffer->context[1],
				buffer->context_len[1],
				features,
				ಅಂಕೆ_features);
}


#endif
