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

#ضمن "hb.hh"

#جے_تعریف_شدہ HAVE_HARFRUST

#ضمن "hb-shaper-impl.hh"

#ضمن "hb-utf.hh"


/*
 * buffer
 */
خارجي "C" فراغ *
hb_harfrust_بفر_create_rs (فراغ);

خارجي "C" فراغ
hb_harfrust_بفر_destroy_rs (فراغ *انکڑے);


/*
 * shaper face data
 */

خارجي "C" فراغ *
hb_harfrust_shaper_face_انکڑے_create_rs (hb_face_t *face);

hb_harfrust_face_انکڑے_t *
hb_harfrust_shaper_face_انکڑے_create (hb_face_t *face)
{
  أرجع (hb_harfrust_face_انکڑے_t *) hb_harfrust_shaper_face_انکڑے_create_rs (face);
}

خارجي "C" فراغ
hb_harfrust_shaper_face_انکڑے_destroy_rs (فراغ *انکڑے);

فراغ
hb_harfrust_shaper_face_انکڑے_destroy (hb_harfrust_face_انکڑے_t *انکڑے)
{
  hb_harfrust_shaper_face_انکڑے_destroy_rs (انکڑے);
}


/*
 * shaper font data
 */

خارجي "C" فراغ *
hb_harfrust_shaper_font_انکڑے_create_rs (hb_font_t *font, ثابت فراغ *face_انکڑے);

hb_harfrust_font_انکڑے_t *
hb_harfrust_shaper_font_انکڑے_create (hb_font_t *font)
{
  ثابت hb_harfrust_face_انکڑے_t *face_انکڑے = font->face->انکڑے.harfrust;
  أرجع (hb_harfrust_font_انکڑے_t *) hb_harfrust_shaper_font_انکڑے_create_rs (font, face_انکڑے);
}

خارجي "C" فراغ
hb_harfrust_shaper_font_انکڑے_destroy_rs (فراغ *انکڑے);

فراغ
hb_harfrust_shaper_font_انکڑے_destroy (hb_harfrust_font_انکڑے_t *انکڑے)
{
  hb_harfrust_shaper_font_انکڑے_destroy_rs (انکڑے);
}


/*
 * shape plan
 */

خارجي "C" فراغ *
_hb_harfrust_shape_plan_create_rs (ثابت فراغ *font_انکڑے,
				   hb_script_t script,
				   hb_language_t language,
				   hb_direction_t direction,
				   ثابت hb_feature_t *features,
				   بنانشانی صحيح        عدد_features);

خارجي "C" فراغ
_hb_harfrust_shape_plan_destroy_rs (فراغ *انکڑے);


/*
 * shaper
 */

خارجي "C" hb_bool_t
_hb_harfrust_shape_rs (ثابت فراغ         *font_انکڑے,
		       ثابت فراغ         *rs_shape_plan,
		       ثابت فراغ         *rs_بفر,
		       hb_font_t          *font,
		       hb_بفر_t        *بفر,
		       ثابت hb_codepoint_t *pre_context,
		       uint32_t            pre_context_len,
		       ثابت hb_codepoint_t *post_context,
		       uint32_t            post_context_len,
		       ثابت hb_feature_t *features,
		       بنانشانی صحيح        عدد_features);

ساكن hb_user_انکڑے_کلید_t hb_بفر_کلید = {0};

hb_bool_t
_hb_harfrust_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_بفر_t        *بفر,
		    ثابت hb_feature_t *features,
		    بنانشانی صحيح        عدد_features)
{
  ثابت hb_harfrust_font_انکڑے_t *font_انکڑے = font->انکڑے.harfrust;

retry_بفر:
  فراغ *hr_بفر = hb_بفر_get_user_انکڑے (بفر, &hb_بفر_کلید);
  إذا (unlikely (!hr_بفر))
  {
    hr_بفر = hb_harfrust_بفر_create_rs ();
    إذا (unlikely (!hr_بفر))
      أرجع خطأ;

    إذا (!hb_بفر_سیٹ_user_انکڑے (بفر,
				  &hb_بفر_کلید,
				  hr_بفر,
				  hb_harfrust_بفر_destroy_rs,
				  خطأ))
    {
      hb_harfrust_بفر_destroy_rs (hr_بفر);
      اذهب retry_بفر;
    }
  }

  فراغ *hr_shape_plan = خالی_سوچک;

retry_shape_plan:
  hr_shape_plan = shape_plan->harfrust_انکڑے.get_acquire ();
  إذا (unlikely (!hr_shape_plan))
  {
    hr_shape_plan = _hb_harfrust_shape_plan_create_rs (font_انکڑے,
						       shape_plan->key.props.script,
						       shape_plan->key.props.language,
						       shape_plan->key.props.direction,
						       features, عدد_features);
    إذا (hr_shape_plan &&
	!shape_plan->harfrust_انکڑے.cmpexch (خالی_سوچک, hr_shape_plan))
    {
      _hb_harfrust_shape_plan_destroy_rs (hr_shape_plan);
      اذهب retry_shape_plan;
    }
  }

  أرجع _hb_harfrust_shape_rs (font_انکڑے,
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


#جے_انت
