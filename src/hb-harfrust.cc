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

#اگر_تعریف_شده HAVE_HARFRUST

#ضمن "hb-shaper-impl.hh"

#ضمن "hb-utf.hh"


/*
 * buffer
 */
خارجي "C" فراغ *
hb_harfrust_میانگیر_create_rs (فراغ);

خارجي "C" فراغ
hb_harfrust_میانگیر_destroy_rs (فراغ *داده);


/*
 * shaper face data
 */

خارجي "C" فراغ *
hb_harfrust_shaper_face_داده_create_rs (hb_face_t *face);

hb_harfrust_face_داده_t *
hb_harfrust_shaper_face_داده_create (hb_face_t *face)
{
  أرجع (hb_harfrust_face_داده_t *) hb_harfrust_shaper_face_داده_create_rs (face);
}

خارجي "C" فراغ
hb_harfrust_shaper_face_داده_destroy_rs (فراغ *داده);

فراغ
hb_harfrust_shaper_face_داده_destroy (hb_harfrust_face_داده_t *داده)
{
  hb_harfrust_shaper_face_داده_destroy_rs (داده);
}


/*
 * shaper font data
 */

خارجي "C" فراغ *
hb_harfrust_shaper_font_داده_create_rs (hb_font_t *font, ثابت فراغ *face_داده);

hb_harfrust_font_داده_t *
hb_harfrust_shaper_font_داده_create (hb_font_t *font)
{
  ثابت hb_harfrust_face_داده_t *face_داده = font->face->داده.harfrust;
  أرجع (hb_harfrust_font_داده_t *) hb_harfrust_shaper_font_داده_create_rs (font, face_داده);
}

خارجي "C" فراغ
hb_harfrust_shaper_font_داده_destroy_rs (فراغ *داده);

فراغ
hb_harfrust_shaper_font_داده_destroy (hb_harfrust_font_داده_t *داده)
{
  hb_harfrust_shaper_font_داده_destroy_rs (داده);
}


/*
 * shape plan
 */

خارجي "C" فراغ *
_hb_harfrust_shape_plan_create_rs (ثابت فراغ *font_داده,
				   hb_script_t script,
				   hb_language_t language,
				   hb_direction_t direction,
				   ثابت hb_feature_t *features,
				   بیعلامت صحيح        رقم_features);

خارجي "C" فراغ
_hb_harfrust_shape_plan_destroy_rs (فراغ *داده);


/*
 * shaper
 */

خارجي "C" hb_bool_t
_hb_harfrust_shape_rs (ثابت فراغ         *font_داده,
		       ثابت فراغ         *rs_shape_plan,
		       ثابت فراغ         *rs_میانگیر,
		       hb_font_t          *font,
		       hb_میانگیر_t        *میانگیر,
		       ثابت hb_codepoint_t *pre_context,
		       uint32_t            pre_context_len,
		       ثابت hb_codepoint_t *post_context,
		       uint32_t            post_context_len,
		       ثابت hb_feature_t *features,
		       بیعلامت صحيح        رقم_features);

ساكن hb_user_داده_کلید_t hb_میانگیر_کلید = {0};

hb_bool_t
_hb_harfrust_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_میانگیر_t        *میانگیر,
		    ثابت hb_feature_t *features,
		    بیعلامت صحيح        رقم_features)
{
  ثابت hb_harfrust_font_داده_t *font_داده = font->داده.harfrust;

retry_میانگیر:
  فراغ *hr_میانگیر = hb_میانگیر_get_user_داده (میانگیر, &hb_میانگیر_کلید);
  إذا (unlikely (!hr_میانگیر))
  {
    hr_میانگیر = hb_harfrust_میانگیر_create_rs ();
    إذا (unlikely (!hr_میانگیر))
      أرجع خطأ;

    إذا (!hb_میانگیر_مجموعه_user_داده (میانگیر,
				  &hb_میانگیر_کلید,
				  hr_میانگیر,
				  hb_harfrust_میانگیر_destroy_rs,
				  خطأ))
    {
      hb_harfrust_میانگیر_destroy_rs (hr_میانگیر);
      اذهب retry_میانگیر;
    }
  }

  فراغ *hr_shape_plan = اشاره_تهی;

retry_shape_plan:
  hr_shape_plan = shape_plan->harfrust_داده.get_acquire ();
  إذا (unlikely (!hr_shape_plan))
  {
    hr_shape_plan = _hb_harfrust_shape_plan_create_rs (font_داده,
						       shape_plan->key.props.script,
						       shape_plan->key.props.language,
						       shape_plan->key.props.direction,
						       features, رقم_features);
    إذا (hr_shape_plan &&
	!shape_plan->harfrust_داده.cmpexch (اشاره_تهی, hr_shape_plan))
    {
      _hb_harfrust_shape_plan_destroy_rs (hr_shape_plan);
      اذهب retry_shape_plan;
    }
  }

  أرجع _hb_harfrust_shape_rs (font_داده,
				hr_shape_plan,
				hr_میانگیر,
				font,
				میانگیر,
				میانگیر->context[0],
				میانگیر->context_len[0],
				میانگیر->context[1],
				میانگیر->context_len[1],
				features,
				رقم_features);
}


#پایان_اگر
