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

#شامل "hb.hh"

#اگر_تعریف_شدہ HAVE_HARFRUST

#شامل "hb-shaper-impl.hh"

#شامل "hb-utf.hh"


/*
 * buffer
 */
خارجی "C" خالی *
hb_harfrust_بفر_create_rs (خالی);

خارجی "C" خالی
hb_harfrust_بفر_destroy_rs (خالی *ڈیٹا);


/*
 * shaper face data
 */

خارجی "C" خالی *
hb_harfrust_shaper_face_ڈیٹا_create_rs (hb_face_t *face);

hb_harfrust_face_ڈیٹا_t *
hb_harfrust_shaper_face_ڈیٹا_create (hb_face_t *face)
{
  واپس (hb_harfrust_face_ڈیٹا_t *) hb_harfrust_shaper_face_ڈیٹا_create_rs (face);
}

خارجی "C" خالی
hb_harfrust_shaper_face_ڈیٹا_destroy_rs (خالی *ڈیٹا);

خالی
hb_harfrust_shaper_face_ڈیٹا_destroy (hb_harfrust_face_ڈیٹا_t *ڈیٹا)
{
  hb_harfrust_shaper_face_ڈیٹا_destroy_rs (ڈیٹا);
}


/*
 * shaper font data
 */

خارجی "C" خالی *
hb_harfrust_shaper_font_ڈیٹا_create_rs (hb_font_t *font, ثابت خالی *face_ڈیٹا);

hb_harfrust_font_ڈیٹا_t *
hb_harfrust_shaper_font_ڈیٹا_create (hb_font_t *font)
{
  ثابت hb_harfrust_face_ڈیٹا_t *face_ڈیٹا = font->face->ڈیٹا.harfrust;
  واپس (hb_harfrust_font_ڈیٹا_t *) hb_harfrust_shaper_font_ڈیٹا_create_rs (font, face_ڈیٹا);
}

خارجی "C" خالی
hb_harfrust_shaper_font_ڈیٹا_destroy_rs (خالی *ڈیٹا);

خالی
hb_harfrust_shaper_font_ڈیٹا_destroy (hb_harfrust_font_ڈیٹا_t *ڈیٹا)
{
  hb_harfrust_shaper_font_ڈیٹا_destroy_rs (ڈیٹا);
}


/*
 * shape plan
 */

خارجی "C" خالی *
_hb_harfrust_shape_plan_create_rs (ثابت خالی *font_ڈیٹا,
				   hb_script_t script,
				   hb_language_t language,
				   hb_direction_t direction,
				   ثابت hb_feature_t *features,
				   غیرعلامتی صحیح        عدد_features);

خارجی "C" خالی
_hb_harfrust_shape_plan_destroy_rs (خالی *ڈیٹا);


/*
 * shaper
 */

خارجی "C" hb_bool_t
_hb_harfrust_shape_rs (ثابت خالی         *font_ڈیٹا,
		       ثابت خالی         *rs_shape_plan,
		       ثابت خالی         *rs_بفر,
		       hb_font_t          *font,
		       hb_بفر_t        *بفر,
		       ثابت hb_codepoint_t *pre_context,
		       uint32_t            pre_context_len,
		       ثابت hb_codepoint_t *post_context,
		       uint32_t            post_context_len,
		       ثابت hb_feature_t *features,
		       غیرعلامتی صحیح        عدد_features);

ساکن hb_user_ڈیٹا_کلید_t hb_بفر_کلید = {0};

hb_bool_t
_hb_harfrust_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_بفر_t        *بفر,
		    ثابت hb_feature_t *features,
		    غیرعلامتی صحیح        عدد_features)
{
  ثابت hb_harfrust_font_ڈیٹا_t *font_ڈیٹا = font->ڈیٹا.harfrust;

retry_بفر:
  خالی *hr_بفر = hb_بفر_get_user_ڈیٹا (بفر, &hb_بفر_کلید);
  اگر (unlikely (!hr_بفر))
  {
    hr_بفر = hb_harfrust_بفر_create_rs ();
    اگر (unlikely (!hr_بفر))
      واپس جھوٹ;

    اگر (!hb_بفر_سیٹ_user_ڈیٹا (بفر,
				  &hb_بفر_کلید,
				  hr_بفر,
				  hb_harfrust_بفر_destroy_rs,
				  جھوٹ))
    {
      hb_harfrust_بفر_destroy_rs (hr_بفر);
      جاؤ retry_بفر;
    }
  }

  خالی *hr_shape_plan = خالی_اشاریہ;

retry_shape_plan:
  hr_shape_plan = shape_plan->harfrust_ڈیٹا.get_acquire ();
  اگر (unlikely (!hr_shape_plan))
  {
    hr_shape_plan = _hb_harfrust_shape_plan_create_rs (font_ڈیٹا,
						       shape_plan->key.props.script,
						       shape_plan->key.props.language,
						       shape_plan->key.props.direction,
						       features, عدد_features);
    اگر (hr_shape_plan &&
	!shape_plan->harfrust_ڈیٹا.cmpexch (خالی_اشاریہ, hr_shape_plan))
    {
      _hb_harfrust_shape_plan_destroy_rs (hr_shape_plan);
      جاؤ retry_shape_plan;
    }
  }

  واپس _hb_harfrust_shape_rs (font_ڈیٹا,
				hr_shape_plan,
				hr_بفر,
				font,
				بفر,
				بفر->context[0],
				بفر->context_len[0],
				بفر->context[1],
				بفر->context_len[1],
				features,
				عدد_features);
}


#اگر_ختم
