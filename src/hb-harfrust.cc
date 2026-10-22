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

#অন্তর্ভাৱ "hb.hh"

#ifdef HAVE_HARFRUST

#অন্তর্ভাৱ "hb-shaper-impl.hh"

#অন্তর্ভাৱ "hb-utf.hh"


/*
 * buffer
 */
বাহ্য "C" শূন্য *
_hb_harfrust_buffer_create_rs (শূন্য);

বাহ্য "C" শূন্য
_hb_harfrust_buffer_destroy_rs (শূন্য *data);


/*
 * shaper face data
 */

বাহ্য "C" শূন্য *
_hb_harfrust_shaper_face_data_create_rs (hb_face_t *face);

hb_harfrust_face_data_t *
_hb_harfrust_shaper_face_data_create (hb_face_t *face)
{
  ঘূৰাই (hb_harfrust_face_data_t *) _hb_harfrust_shaper_face_data_create_rs (face);
}

বাহ্য "C" শূন্য
_hb_harfrust_shaper_face_data_destroy_rs (শূন্য *data);

শূন্য
_hb_harfrust_shaper_face_data_destroy (hb_harfrust_face_data_t *data)
{
  _hb_harfrust_shaper_face_data_destroy_rs (data);
}


/*
 * shaper font data
 */

বাহ্য "C" শূন্য *
_hb_harfrust_shaper_font_data_create_rs (hb_font_t *font, ধ্রুৱ শূন্য *face_data);

hb_harfrust_font_data_t *
_hb_harfrust_shaper_font_data_create (hb_font_t *font)
{
  ধ্রুৱ hb_harfrust_face_data_t *face_data = font->face->data.harfrust;
  ঘূৰাই (hb_harfrust_font_data_t *) _hb_harfrust_shaper_font_data_create_rs (font, face_data);
}

বাহ্য "C" শূন্য
_hb_harfrust_shaper_font_data_destroy_rs (শূন্য *data);

শূন্য
_hb_harfrust_shaper_font_data_destroy (hb_harfrust_font_data_t *data)
{
  _hb_harfrust_shaper_font_data_destroy_rs (data);
}


/*
 * shape plan
 */

বাহ্য "C" শূন্য *
_hb_harfrust_shape_plan_create_rs (ধ্রুৱ শূন্য *font_data,
				   hb_script_t script,
				   hb_language_t language,
				   hb_direction_t direction,
				   ধ্রুৱ hb_feature_t *features,
				   unsigned পূর্ণাঙ্ক        অঙ্ক_features);

বাহ্য "C" শূন্য
_hb_harfrust_shape_plan_destroy_rs (শূন্য *data);


/*
 * shaper
 */

বাহ্য "C" hb_bool_t
_hb_harfrust_shape_rs (ধ্রুৱ শূন্য         *font_data,
		       ধ্রুৱ শূন্য         *rs_shape_plan,
		       ধ্রুৱ শূন্য         *rs_buffer,
		       hb_font_t          *font,
		       hb_buffer_t        *buffer,
		       ধ্রুৱ hb_codepoint_t *pre_context,
		       uint32_t            pre_context_len,
		       ধ্রুৱ hb_codepoint_t *post_context,
		       uint32_t            post_context_len,
		       ধ্রুৱ hb_feature_t *features,
		       unsigned পূর্ণাঙ্ক        অঙ্ক_features);

স্থির hb_user_data_চাবি_t hb_buffer_চাবি = {0};

hb_bool_t
_hb_harfrust_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_buffer_t        *buffer,
		    ধ্রুৱ hb_feature_t *features,
		    unsigned পূর্ণাঙ্ক        অঙ্ক_features)
{
  ধ্রুৱ hb_harfrust_font_data_t *font_data = font->data.harfrust;

retry_buffer:
  শূন্য *hr_buffer = hb_buffer_get_user_data (buffer, &hb_buffer_চাবি);
  যদি (unlikely (!hr_buffer))
  {
    hr_buffer = _hb_harfrust_buffer_create_rs ();
    যদি (unlikely (!hr_buffer))
      ঘূৰাই মিছা;

    যদি (!hb_buffer_set_user_data (buffer,
				  &hb_buffer_চাবি,
				  hr_buffer,
				  _hb_harfrust_buffer_destroy_rs,
				  মিছা))
    {
      _hb_harfrust_buffer_destroy_rs (hr_buffer);
      গচ্ছ retry_buffer;
    }
  }

  শূন্য *hr_shape_plan = nullptr;

retry_shape_plan:
  hr_shape_plan = shape_plan->harfrust_data.get_acquire ();
  যদি (unlikely (!hr_shape_plan))
  {
    hr_shape_plan = _hb_harfrust_shape_plan_create_rs (font_data,
						       shape_plan->key.props.script,
						       shape_plan->key.props.language,
						       shape_plan->key.props.direction,
						       features, অঙ্ক_features);
    যদি (hr_shape_plan &&
	!shape_plan->harfrust_data.cmpexch (nullptr, hr_shape_plan))
    {
      _hb_harfrust_shape_plan_destroy_rs (hr_shape_plan);
      গচ্ছ retry_shape_plan;
    }
  }

  ঘূৰাই _hb_harfrust_shape_rs (font_data,
				hr_shape_plan,
				hr_buffer,
				font,
				buffer,
				buffer->context[0],
				buffer->context_len[0],
				buffer->context[1],
				buffer->context_len[1],
				features,
				অঙ্ক_features);
}


#endif
