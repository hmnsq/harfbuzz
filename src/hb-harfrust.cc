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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_HARFRUST

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-impl.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-utf.hh"


/*
 * buffer
 */
ਬਾਹਰੀ "C" ਖਾਲੀ *
hb_harfrust_ਬਫਰ_create_rs (ਖਾਲੀ);

ਬਾਹਰੀ "C" ਖਾਲੀ
hb_harfrust_ਬਫਰ_destroy_rs (ਖਾਲੀ *ਅੰਕੜੇ);


/*
 * shaper face data
 */

ਬਾਹਰੀ "C" ਖਾਲੀ *
hb_harfrust_shaper_face_ਅੰਕੜੇ_create_rs (hb_face_t *face);

hb_harfrust_face_ਅੰਕੜੇ_t *
hb_harfrust_shaper_face_ਅੰਕੜੇ_create (hb_face_t *face)
{
  ਵਾਪਸ (hb_harfrust_face_ਅੰਕੜੇ_t *) hb_harfrust_shaper_face_ਅੰਕੜੇ_create_rs (face);
}

ਬਾਹਰੀ "C" ਖਾਲੀ
hb_harfrust_shaper_face_ਅੰਕੜੇ_destroy_rs (ਖਾਲੀ *ਅੰਕੜੇ);

ਖਾਲੀ
hb_harfrust_shaper_face_ਅੰਕੜੇ_destroy (hb_harfrust_face_ਅੰਕੜੇ_t *ਅੰਕੜੇ)
{
  hb_harfrust_shaper_face_ਅੰਕੜੇ_destroy_rs (ਅੰਕੜੇ);
}


/*
 * shaper font data
 */

ਬਾਹਰੀ "C" ਖਾਲੀ *
hb_harfrust_shaper_font_ਅੰਕੜੇ_create_rs (hb_font_t *font, ਅਚਲ ਖਾਲੀ *face_ਅੰਕੜੇ);

hb_harfrust_font_ਅੰਕੜੇ_t *
hb_harfrust_shaper_font_ਅੰਕੜੇ_create (hb_font_t *font)
{
  ਅਚਲ hb_harfrust_face_ਅੰਕੜੇ_t *face_ਅੰਕੜੇ = font->face->ਅੰਕੜੇ.harfrust;
  ਵਾਪਸ (hb_harfrust_font_ਅੰਕੜੇ_t *) hb_harfrust_shaper_font_ਅੰਕੜੇ_create_rs (font, face_ਅੰਕੜੇ);
}

ਬਾਹਰੀ "C" ਖਾਲੀ
hb_harfrust_shaper_font_ਅੰਕੜੇ_destroy_rs (ਖਾਲੀ *ਅੰਕੜੇ);

ਖਾਲੀ
hb_harfrust_shaper_font_ਅੰਕੜੇ_destroy (hb_harfrust_font_ਅੰਕੜੇ_t *ਅੰਕੜੇ)
{
  hb_harfrust_shaper_font_ਅੰਕੜੇ_destroy_rs (ਅੰਕੜੇ);
}


/*
 * shape plan
 */

ਬਾਹਰੀ "C" ਖਾਲੀ *
_hb_harfrust_shape_plan_create_rs (ਅਚਲ ਖਾਲੀ *font_ਅੰਕੜੇ,
				   hb_script_t script,
				   hb_language_t language,
				   hb_direction_t direction,
				   ਅਚਲ hb_feature_t *features,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features);

ਬਾਹਰੀ "C" ਖਾਲੀ
_hb_harfrust_shape_plan_destroy_rs (ਖਾਲੀ *ਅੰਕੜੇ);


/*
 * shaper
 */

ਬਾਹਰੀ "C" hb_bool_t
_hb_harfrust_shape_rs (ਅਚਲ ਖਾਲੀ         *font_ਅੰਕੜੇ,
		       ਅਚਲ ਖਾਲੀ         *rs_shape_plan,
		       ਅਚਲ ਖਾਲੀ         *rs_ਬਫਰ,
		       hb_font_t          *font,
		       hb_ਬਫਰ_t        *ਬਫਰ,
		       ਅਚਲ hb_codepoint_t *pre_context,
		       uint32_t            pre_context_len,
		       ਅਚਲ hb_codepoint_t *post_context,
		       uint32_t            post_context_len,
		       ਅਚਲ hb_feature_t *features,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features);

ਸਥਿਰ hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t hb_ਬਫਰ_ਕੁੰਜੀ = {0};

hb_bool_t
_hb_harfrust_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_ਬਫਰ_t        *ਬਫਰ,
		    ਅਚਲ hb_feature_t *features,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features)
{
  ਅਚਲ hb_harfrust_font_ਅੰਕੜੇ_t *font_ਅੰਕੜੇ = font->ਅੰਕੜੇ.harfrust;

retry_ਬਫਰ:
  ਖਾਲੀ *hr_ਬਫਰ = hb_ਬਫਰ_get_user_ਅੰਕੜੇ (ਬਫਰ, &hb_ਬਫਰ_ਕੁੰਜੀ);
  ਜੇ (unlikely (!hr_ਬਫਰ))
  {
    hr_ਬਫਰ = hb_harfrust_ਬਫਰ_create_rs ();
    ਜੇ (unlikely (!hr_ਬਫਰ))
      ਵਾਪਸ ਝੂਠ;

    ਜੇ (!hb_ਬਫਰ_ਸੈੱਟ_user_ਅੰਕੜੇ (ਬਫਰ,
				  &hb_ਬਫਰ_ਕੁੰਜੀ,
				  hr_ਬਫਰ,
				  hb_harfrust_ਬਫਰ_destroy_rs,
				  ਝੂਠ))
    {
      hb_harfrust_ਬਫਰ_destroy_rs (hr_ਬਫਰ);
      ਜਾਓ retry_ਬਫਰ;
    }
  }

  ਖਾਲੀ *hr_shape_plan = ਖਾਲੀ_ਸੂਚਕ;

retry_shape_plan:
  hr_shape_plan = shape_plan->harfrust_ਅੰਕੜੇ.get_acquire ();
  ਜੇ (unlikely (!hr_shape_plan))
  {
    hr_shape_plan = _hb_harfrust_shape_plan_create_rs (font_ਅੰਕੜੇ,
						       shape_plan->key.props.script,
						       shape_plan->key.props.language,
						       shape_plan->key.props.direction,
						       features, ਅੰਕ_features);
    ਜੇ (hr_shape_plan &&
	!shape_plan->harfrust_ਅੰਕੜੇ.cmpexch (ਖਾਲੀ_ਸੂਚਕ, hr_shape_plan))
    {
      _hb_harfrust_shape_plan_destroy_rs (hr_shape_plan);
      ਜਾਓ retry_shape_plan;
    }
  }

  ਵਾਪਸ _hb_harfrust_shape_rs (font_ਅੰਕੜੇ,
				hr_shape_plan,
				hr_ਬਫਰ,
				font,
				ਬਫਰ,
				ਬਫਰ->context[0],
				ਬਫਰ->context_len[0],
				ਬਫਰ->context[1],
				ਬਫਰ->context_len[1],
				features,
				ਅੰਕ_features);
}


#ਜੇ_ਅੰਤ
