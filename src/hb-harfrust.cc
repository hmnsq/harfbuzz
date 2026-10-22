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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifdef HAVE_HARFRUST

#ଅନ୍ତର୍ଭାଵ "hb-shaper-impl.hh"

#ଅନ୍ତର୍ଭାଵ "hb-utf.hh"


/*
 * buffer
 */
ବାହ୍ୟ "C" ଶୂନ୍ୟ *
_hb_harfrust_buffer_create_rs (ଶୂନ୍ୟ);

ବାହ୍ୟ "C" ଶୂନ୍ୟ
_hb_harfrust_buffer_destroy_rs (ଶୂନ୍ୟ *data);


/*
 * shaper face data
 */

ବାହ୍ୟ "C" ଶୂନ୍ୟ *
_hb_harfrust_shaper_face_data_create_rs (hb_face_t *face);

hb_harfrust_face_data_t *
_hb_harfrust_shaper_face_data_create (hb_face_t *face)
{
  ଫେରନ୍ତୁ (hb_harfrust_face_data_t *) _hb_harfrust_shaper_face_data_create_rs (face);
}

ବାହ୍ୟ "C" ଶୂନ୍ୟ
_hb_harfrust_shaper_face_data_destroy_rs (ଶୂନ୍ୟ *data);

ଶୂନ୍ୟ
_hb_harfrust_shaper_face_data_destroy (hb_harfrust_face_data_t *data)
{
  _hb_harfrust_shaper_face_data_destroy_rs (data);
}


/*
 * shaper font data
 */

ବାହ୍ୟ "C" ଶୂନ୍ୟ *
_hb_harfrust_shaper_font_data_create_rs (hb_font_t *font, ଅଚଳ ଶୂନ୍ୟ *face_data);

hb_harfrust_font_data_t *
_hb_harfrust_shaper_font_data_create (hb_font_t *font)
{
  ଅଚଳ hb_harfrust_face_data_t *face_data = font->face->data.harfrust;
  ଫେରନ୍ତୁ (hb_harfrust_font_data_t *) _hb_harfrust_shaper_font_data_create_rs (font, face_data);
}

ବାହ୍ୟ "C" ଶୂନ୍ୟ
_hb_harfrust_shaper_font_data_destroy_rs (ଶୂନ୍ୟ *data);

ଶୂନ୍ୟ
_hb_harfrust_shaper_font_data_destroy (hb_harfrust_font_data_t *data)
{
  _hb_harfrust_shaper_font_data_destroy_rs (data);
}


/*
 * shape plan
 */

ବାହ୍ୟ "C" ଶୂନ୍ୟ *
_hb_harfrust_shape_plan_create_rs (ଅଚଳ ଶୂନ୍ୟ *font_data,
				   hb_script_t script,
				   hb_language_t language,
				   hb_direction_t direction,
				   ଅଚଳ hb_feature_t *features,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା        ଅଙ୍କ_features);

ବାହ୍ୟ "C" ଶୂନ୍ୟ
_hb_harfrust_shape_plan_destroy_rs (ଶୂନ୍ୟ *data);


/*
 * shaper
 */

ବାହ୍ୟ "C" hb_bool_t
_hb_harfrust_shape_rs (ଅଚଳ ଶୂନ୍ୟ         *font_data,
		       ଅଚଳ ଶୂନ୍ୟ         *rs_shape_plan,
		       ଅଚଳ ଶୂନ୍ୟ         *rs_buffer,
		       hb_font_t          *font,
		       hb_buffer_t        *buffer,
		       ଅଚଳ hb_codepoint_t *pre_context,
		       uint32_t            pre_context_len,
		       ଅଚଳ hb_codepoint_t *post_context,
		       uint32_t            post_context_len,
		       ଅଚଳ hb_feature_t *features,
		       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା        ଅଙ୍କ_features);

ସ୍ଥିର hb_user_data_ଚାବି_t hb_buffer_ଚାବି = {0};

hb_bool_t
_hb_harfrust_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_buffer_t        *buffer,
		    ଅଚଳ hb_feature_t *features,
		    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା        ଅଙ୍କ_features)
{
  ଅଚଳ hb_harfrust_font_data_t *font_data = font->data.harfrust;

retry_buffer:
  ଶୂନ୍ୟ *hr_buffer = hb_buffer_get_user_data (buffer, &hb_buffer_ଚାବି);
  ଯଦି (unlikely (!hr_buffer))
  {
    hr_buffer = _hb_harfrust_buffer_create_rs ();
    ଯଦି (unlikely (!hr_buffer))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (!hb_buffer_set_user_data (buffer,
				  &hb_buffer_ଚାବି,
				  hr_buffer,
				  _hb_harfrust_buffer_destroy_rs,
				  ମିଥ୍ୟା))
    {
      _hb_harfrust_buffer_destroy_rs (hr_buffer);
      ଯାଅ retry_buffer;
    }
  }

  ଶୂନ୍ୟ *hr_shape_plan = nullptr;

retry_shape_plan:
  hr_shape_plan = shape_plan->harfrust_data.get_acquire ();
  ଯଦି (unlikely (!hr_shape_plan))
  {
    hr_shape_plan = _hb_harfrust_shape_plan_create_rs (font_data,
						       shape_plan->key.props.script,
						       shape_plan->key.props.language,
						       shape_plan->key.props.direction,
						       features, ଅଙ୍କ_features);
    ଯଦି (hr_shape_plan &&
	!shape_plan->harfrust_data.cmpexch (nullptr, hr_shape_plan))
    {
      _hb_harfrust_shape_plan_destroy_rs (hr_shape_plan);
      ଯାଅ retry_shape_plan;
    }
  }

  ଫେରନ୍ତୁ _hb_harfrust_shape_rs (font_data,
				hr_shape_plan,
				hr_buffer,
				font,
				buffer,
				buffer->context[0],
				buffer->context_len[0],
				buffer->context[1],
				buffer->context_len[1],
				features,
				ଅଙ୍କ_features);
}


#endif
