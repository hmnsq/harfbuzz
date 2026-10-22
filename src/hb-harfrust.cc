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

#അന്തര്ഭാവ "hb.hh"

#ifdef HAVE_HARFRUST

#അന്തര്ഭാവ "hb-shaper-impl.hh"

#അന്തര്ഭാവ "hb-utf.hh"


/*
 * buffer
 */
ബാഹ്യ "C" ശൂന്യം *
_hb_harfrust_buffer_create_rs (ശൂന്യം);

ബാഹ്യ "C" ശൂന്യം
_hb_harfrust_buffer_destroy_rs (ശൂന്യം *data);


/*
 * shaper face data
 */

ബാഹ്യ "C" ശൂന്യം *
_hb_harfrust_shaper_face_data_create_rs (hb_face_t *face);

hb_harfrust_face_data_t *
_hb_harfrust_shaper_face_data_create (hb_face_t *face)
{
  തിരികെ (hb_harfrust_face_data_t *) _hb_harfrust_shaper_face_data_create_rs (face);
}

ബാഹ്യ "C" ശൂന്യം
_hb_harfrust_shaper_face_data_destroy_rs (ശൂന്യം *data);

ശൂന്യം
_hb_harfrust_shaper_face_data_destroy (hb_harfrust_face_data_t *data)
{
  _hb_harfrust_shaper_face_data_destroy_rs (data);
}


/*
 * shaper font data
 */

ബാഹ്യ "C" ശൂന്യം *
_hb_harfrust_shaper_font_data_create_rs (hb_font_t *font, സ്ഥിരം ശൂന്യം *face_data);

hb_harfrust_font_data_t *
_hb_harfrust_shaper_font_data_create (hb_font_t *font)
{
  സ്ഥിരം hb_harfrust_face_data_t *face_data = font->face->data.harfrust;
  തിരികെ (hb_harfrust_font_data_t *) _hb_harfrust_shaper_font_data_create_rs (font, face_data);
}

ബാഹ്യ "C" ശൂന്യം
_hb_harfrust_shaper_font_data_destroy_rs (ശൂന്യം *data);

ശൂന്യം
_hb_harfrust_shaper_font_data_destroy (hb_harfrust_font_data_t *data)
{
  _hb_harfrust_shaper_font_data_destroy_rs (data);
}


/*
 * shape plan
 */

ബാഹ്യ "C" ശൂന്യം *
_hb_harfrust_shape_plan_create_rs (സ്ഥിരം ശൂന്യം *font_data,
				   hb_script_t script,
				   hb_language_t language,
				   hb_direction_t direction,
				   സ്ഥിരം hb_feature_t *features,
				   unsigned പൂർണ്ണസംഖ്യ        അക്കം_features);

ബാഹ്യ "C" ശൂന്യം
_hb_harfrust_shape_plan_destroy_rs (ശൂന്യം *data);


/*
 * shaper
 */

ബാഹ്യ "C" hb_bool_t
_hb_harfrust_shape_rs (സ്ഥിരം ശൂന്യം         *font_data,
		       സ്ഥിരം ശൂന്യം         *rs_shape_plan,
		       സ്ഥിരം ശൂന്യം         *rs_buffer,
		       hb_font_t          *font,
		       hb_buffer_t        *buffer,
		       സ്ഥിരം hb_codepoint_t *pre_context,
		       uint32_t            pre_context_len,
		       സ്ഥിരം hb_codepoint_t *post_context,
		       uint32_t            post_context_len,
		       സ്ഥിരം hb_feature_t *features,
		       unsigned പൂർണ്ണസംഖ്യ        അക്കം_features);

സ്ഥിതം hb_user_data_താക്കോൽ_t hb_buffer_താക്കോൽ = {0};

hb_bool_t
_hb_harfrust_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_buffer_t        *buffer,
		    സ്ഥിരം hb_feature_t *features,
		    unsigned പൂർണ്ണസംഖ്യ        അക്കം_features)
{
  സ്ഥിരം hb_harfrust_font_data_t *font_data = font->data.harfrust;

retry_buffer:
  ശൂന്യം *hr_buffer = hb_buffer_get_user_data (buffer, &hb_buffer_താക്കോൽ);
  എങ്കിൽ (unlikely (!hr_buffer))
  {
    hr_buffer = _hb_harfrust_buffer_create_rs ();
    എങ്കിൽ (unlikely (!hr_buffer))
      തിരികെ അസത്യം;

    എങ്കിൽ (!hb_buffer_set_user_data (buffer,
				  &hb_buffer_താക്കോൽ,
				  hr_buffer,
				  _hb_harfrust_buffer_destroy_rs,
				  അസത്യം))
    {
      _hb_harfrust_buffer_destroy_rs (hr_buffer);
      പോകുക retry_buffer;
    }
  }

  ശൂന്യം *hr_shape_plan = nullptr;

retry_shape_plan:
  hr_shape_plan = shape_plan->harfrust_data.get_acquire ();
  എങ്കിൽ (unlikely (!hr_shape_plan))
  {
    hr_shape_plan = _hb_harfrust_shape_plan_create_rs (font_data,
						       shape_plan->key.props.script,
						       shape_plan->key.props.language,
						       shape_plan->key.props.direction,
						       features, അക്കം_features);
    എങ്കിൽ (hr_shape_plan &&
	!shape_plan->harfrust_data.cmpexch (nullptr, hr_shape_plan))
    {
      _hb_harfrust_shape_plan_destroy_rs (hr_shape_plan);
      പോകുക retry_shape_plan;
    }
  }

  തിരികെ _hb_harfrust_shape_rs (font_data,
				hr_shape_plan,
				hr_buffer,
				font,
				buffer,
				buffer->context[0],
				buffer->context_len[0],
				buffer->context[1],
				buffer->context_len[1],
				features,
				അക്കം_features);
}


#endif
