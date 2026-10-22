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

#அந்தர்பாவ "hb.hh"

#ifdef HAVE_HARFRUST

#அந்தர்பாவ "hb-shaper-impl.hh"

#அந்தர்பாவ "hb-utf.hh"


/*
 * buffer
 */
வெளி "C" வெறுமை *
_hb_harfrust_buffer_create_rs (வெறுமை);

வெளி "C" வெறுமை
_hb_harfrust_buffer_destroy_rs (வெறுமை *data);


/*
 * shaper face data
 */

வெளி "C" வெறுமை *
_hb_harfrust_shaper_face_data_create_rs (hb_face_t *face);

hb_harfrust_face_data_t *
_hb_harfrust_shaper_face_data_create (hb_face_t *face)
{
  திரும்பு (hb_harfrust_face_data_t *) _hb_harfrust_shaper_face_data_create_rs (face);
}

வெளி "C" வெறுமை
_hb_harfrust_shaper_face_data_destroy_rs (வெறுமை *data);

வெறுமை
_hb_harfrust_shaper_face_data_destroy (hb_harfrust_face_data_t *data)
{
  _hb_harfrust_shaper_face_data_destroy_rs (data);
}


/*
 * shaper font data
 */

வெளி "C" வெறுமை *
_hb_harfrust_shaper_font_data_create_rs (hb_font_t *font, மாறிலி வெறுமை *face_data);

hb_harfrust_font_data_t *
_hb_harfrust_shaper_font_data_create (hb_font_t *font)
{
  மாறிலி hb_harfrust_face_data_t *face_data = font->face->data.harfrust;
  திரும்பு (hb_harfrust_font_data_t *) _hb_harfrust_shaper_font_data_create_rs (font, face_data);
}

வெளி "C" வெறுமை
_hb_harfrust_shaper_font_data_destroy_rs (வெறுமை *data);

வெறுமை
_hb_harfrust_shaper_font_data_destroy (hb_harfrust_font_data_t *data)
{
  _hb_harfrust_shaper_font_data_destroy_rs (data);
}


/*
 * shape plan
 */

வெளி "C" வெறுமை *
_hb_harfrust_shape_plan_create_rs (மாறிலி வெறுமை *font_data,
				   hb_script_t script,
				   hb_language_t language,
				   hb_direction_t direction,
				   மாறிலி hb_feature_t *features,
				   unsigned முழு        இலக்கம்_features);

வெளி "C" வெறுமை
_hb_harfrust_shape_plan_destroy_rs (வெறுமை *data);


/*
 * shaper
 */

வெளி "C" hb_bool_t
_hb_harfrust_shape_rs (மாறிலி வெறுமை         *font_data,
		       மாறிலி வெறுமை         *rs_shape_plan,
		       மாறிலி வெறுமை         *rs_buffer,
		       hb_font_t          *font,
		       hb_buffer_t        *buffer,
		       மாறிலி hb_codepoint_t *pre_context,
		       uint32_t            pre_context_len,
		       மாறிலி hb_codepoint_t *post_context,
		       uint32_t            post_context_len,
		       மாறிலி hb_feature_t *features,
		       unsigned முழு        இலக்கம்_features);

நிலையான hb_user_data_சாவி_t hb_buffer_சாவி = {0};

hb_bool_t
_hb_harfrust_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_buffer_t        *buffer,
		    மாறிலி hb_feature_t *features,
		    unsigned முழு        இலக்கம்_features)
{
  மாறிலி hb_harfrust_font_data_t *font_data = font->data.harfrust;

retry_buffer:
  வெறுமை *hr_buffer = hb_buffer_get_user_data (buffer, &hb_buffer_சாவி);
  எனில் (unlikely (!hr_buffer))
  {
    hr_buffer = _hb_harfrust_buffer_create_rs ();
    எனில் (unlikely (!hr_buffer))
      திரும்பு பொய்;

    எனில் (!hb_buffer_set_user_data (buffer,
				  &hb_buffer_சாவி,
				  hr_buffer,
				  _hb_harfrust_buffer_destroy_rs,
				  பொய்))
    {
      _hb_harfrust_buffer_destroy_rs (hr_buffer);
      செல் retry_buffer;
    }
  }

  வெறுமை *hr_shape_plan = nullptr;

retry_shape_plan:
  hr_shape_plan = shape_plan->harfrust_data.get_acquire ();
  எனில் (unlikely (!hr_shape_plan))
  {
    hr_shape_plan = _hb_harfrust_shape_plan_create_rs (font_data,
						       shape_plan->key.props.script,
						       shape_plan->key.props.language,
						       shape_plan->key.props.direction,
						       features, இலக்கம்_features);
    எனில் (hr_shape_plan &&
	!shape_plan->harfrust_data.cmpexch (nullptr, hr_shape_plan))
    {
      _hb_harfrust_shape_plan_destroy_rs (hr_shape_plan);
      செல் retry_shape_plan;
    }
  }

  திரும்பு _hb_harfrust_shape_rs (font_data,
				hr_shape_plan,
				hr_buffer,
				font,
				buffer,
				buffer->context[0],
				buffer->context_len[0],
				buffer->context[1],
				buffer->context_len[1],
				features,
				இலக்கம்_features);
}


#endif
