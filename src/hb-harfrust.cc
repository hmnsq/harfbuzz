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

#अन्तर्भाव "hb.hh"

#ifdef HAVE_HARFRUST

#अन्तर्भाव "hb-shaper-impl.hh"

#अन्तर्भाव "hb-utf.hh"


/*
 * buffer
 */
बाह्य "C" शून्य *
_hb_harfrust_buffer_create_rs (शून्य);

बाह्य "C" शून्य
_hb_harfrust_buffer_destroy_rs (शून्य *data);


/*
 * shaper face data
 */

बाह्य "C" शून्य *
_hb_harfrust_shaper_face_data_create_rs (hb_face_t *face);

hb_harfrust_face_data_t *
_hb_harfrust_shaper_face_data_create (hb_face_t *face)
{
  प्रत्यागम (hb_harfrust_face_data_t *) _hb_harfrust_shaper_face_data_create_rs (face);
}

बाह्य "C" शून्य
_hb_harfrust_shaper_face_data_destroy_rs (शून्य *data);

शून्य
_hb_harfrust_shaper_face_data_destroy (hb_harfrust_face_data_t *data)
{
  _hb_harfrust_shaper_face_data_destroy_rs (data);
}


/*
 * shaper font data
 */

बाह्य "C" शून्य *
_hb_harfrust_shaper_font_data_create_rs (hb_font_t *font, ध्रुव शून्य *face_data);

hb_harfrust_font_data_t *
_hb_harfrust_shaper_font_data_create (hb_font_t *font)
{
  ध्रुव hb_harfrust_face_data_t *face_data = font->face->data.harfrust;
  प्रत्यागम (hb_harfrust_font_data_t *) _hb_harfrust_shaper_font_data_create_rs (font, face_data);
}

बाह्य "C" शून्य
_hb_harfrust_shaper_font_data_destroy_rs (शून्य *data);

शून्य
_hb_harfrust_shaper_font_data_destroy (hb_harfrust_font_data_t *data)
{
  _hb_harfrust_shaper_font_data_destroy_rs (data);
}


/*
 * shape plan
 */

बाह्य "C" शून्य *
_hb_harfrust_shape_plan_create_rs (ध्रुव शून्य *font_data,
				   hb_script_t script,
				   hb_language_t language,
				   hb_direction_t direction,
				   ध्रुव hb_feature_t *features,
				   unsigned पूर्णाङ्क        अंक_features);

बाह्य "C" शून्य
_hb_harfrust_shape_plan_destroy_rs (शून्य *data);


/*
 * shaper
 */

बाह्य "C" hb_bool_t
_hb_harfrust_shape_rs (ध्रुव शून्य         *font_data,
		       ध्रुव शून्य         *rs_shape_plan,
		       ध्रुव शून्य         *rs_buffer,
		       hb_font_t          *font,
		       hb_buffer_t        *buffer,
		       ध्रुव hb_codepoint_t *pre_context,
		       uint32_t            pre_context_len,
		       ध्रुव hb_codepoint_t *post_context,
		       uint32_t            post_context_len,
		       ध्रुव hb_feature_t *features,
		       unsigned पूर्णाङ्क        अंक_features);

स्थिर hb_user_data_कुंजी_t hb_buffer_कुंजी = {0};

hb_bool_t
_hb_harfrust_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_buffer_t        *buffer,
		    ध्रुव hb_feature_t *features,
		    unsigned पूर्णाङ्क        अंक_features)
{
  ध्रुव hb_harfrust_font_data_t *font_data = font->data.harfrust;

retry_buffer:
  शून्य *hr_buffer = hb_buffer_get_user_data (buffer, &hb_buffer_कुंजी);
  यदि (unlikely (!hr_buffer))
  {
    hr_buffer = _hb_harfrust_buffer_create_rs ();
    यदि (unlikely (!hr_buffer))
      प्रत्यागम असत्य;

    यदि (!hb_buffer_set_user_data (buffer,
				  &hb_buffer_कुंजी,
				  hr_buffer,
				  _hb_harfrust_buffer_destroy_rs,
				  असत्य))
    {
      _hb_harfrust_buffer_destroy_rs (hr_buffer);
      गच्छ retry_buffer;
    }
  }

  शून्य *hr_shape_plan = nullptr;

retry_shape_plan:
  hr_shape_plan = shape_plan->harfrust_data.get_acquire ();
  यदि (unlikely (!hr_shape_plan))
  {
    hr_shape_plan = _hb_harfrust_shape_plan_create_rs (font_data,
						       shape_plan->key.props.script,
						       shape_plan->key.props.language,
						       shape_plan->key.props.direction,
						       features, अंक_features);
    यदि (hr_shape_plan &&
	!shape_plan->harfrust_data.cmpexch (nullptr, hr_shape_plan))
    {
      _hb_harfrust_shape_plan_destroy_rs (hr_shape_plan);
      गच्छ retry_shape_plan;
    }
  }

  प्रत्यागम _hb_harfrust_shape_rs (font_data,
				hr_shape_plan,
				hr_buffer,
				font,
				buffer,
				buffer->context[0],
				buffer->context_len[0],
				buffer->context[1],
				buffer->context_len[1],
				features,
				अंक_features);
}


#endif
