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

#অন্তর্ভাব "hb.hh"

#ifdef HAVE_HARFRUST

#অন্তর্ভাব "hb-shaper-impl.hh"

#অন্তর্ভাব "hb-utf.hh"


/*
 * buffer
 */
বাহ্যিক "C" শূন্য *
hb_harfrust_বাফার_create_rs (শূন্য);

বাহ্যিক "C" শূন্য
hb_harfrust_বাফার_destroy_rs (শূন্য *তথ্য);


/*
 * shaper face data
 */

বাহ্যিক "C" শূন্য *
hb_harfrust_shaper_face_তথ্য_create_rs (hb_face_t *face);

hb_harfrust_face_তথ্য_t *
hb_harfrust_shaper_face_তথ্য_create (hb_face_t *face)
{
  ফেরত (hb_harfrust_face_তথ্য_t *) hb_harfrust_shaper_face_তথ্য_create_rs (face);
}

বাহ্যিক "C" শূন্য
hb_harfrust_shaper_face_তথ্য_destroy_rs (শূন্য *তথ্য);

শূন্য
hb_harfrust_shaper_face_তথ্য_destroy (hb_harfrust_face_তথ্য_t *তথ্য)
{
  hb_harfrust_shaper_face_তথ্য_destroy_rs (তথ্য);
}


/*
 * shaper font data
 */

বাহ্যিক "C" শূন্য *
hb_harfrust_shaper_font_তথ্য_create_rs (hb_font_t *font, ধ্রুবক শূন্য *face_তথ্য);

hb_harfrust_font_তথ্য_t *
hb_harfrust_shaper_font_তথ্য_create (hb_font_t *font)
{
  ধ্রুবক hb_harfrust_face_তথ্য_t *face_তথ্য = font->face->তথ্য.harfrust;
  ফেরত (hb_harfrust_font_তথ্য_t *) hb_harfrust_shaper_font_তথ্য_create_rs (font, face_তথ্য);
}

বাহ্যিক "C" শূন্য
hb_harfrust_shaper_font_তথ্য_destroy_rs (শূন্য *তথ্য);

শূন্য
hb_harfrust_shaper_font_তথ্য_destroy (hb_harfrust_font_তথ্য_t *তথ্য)
{
  hb_harfrust_shaper_font_তথ্য_destroy_rs (তথ্য);
}


/*
 * shape plan
 */

বাহ্যিক "C" শূন্য *
_hb_harfrust_shape_plan_create_rs (ধ্রুবক শূন্য *font_তথ্য,
				   hb_script_t script,
				   hb_language_t language,
				   hb_direction_t direction,
				   ধ্রুবক hb_feature_t *features,
				   unsigned পূর্ণসংখ্যা        অঙ্ক_features);

বাহ্যিক "C" শূন্য
_hb_harfrust_shape_plan_destroy_rs (শূন্য *তথ্য);


/*
 * shaper
 */

বাহ্যিক "C" hb_bool_t
_hb_harfrust_shape_rs (ধ্রুবক শূন্য         *font_তথ্য,
		       ধ্রুবক শূন্য         *rs_shape_plan,
		       ধ্রুবক শূন্য         *rs_বাফার,
		       hb_font_t          *font,
		       hb_বাফার_t        *বাফার,
		       ধ্রুবক hb_codepoint_t *pre_context,
		       uint32_t            pre_context_len,
		       ধ্রুবক hb_codepoint_t *post_context,
		       uint32_t            post_context_len,
		       ধ্রুবক hb_feature_t *features,
		       unsigned পূর্ণসংখ্যা        অঙ্ক_features);

স্থির hb_user_তথ্য_চাবি_t hb_বাফার_চাবি = {0};

hb_bool_t
_hb_harfrust_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_বাফার_t        *বাফার,
		    ধ্রুবক hb_feature_t *features,
		    unsigned পূর্ণসংখ্যা        অঙ্ক_features)
{
  ধ্রুবক hb_harfrust_font_তথ্য_t *font_তথ্য = font->তথ্য.harfrust;

retry_বাফার:
  শূন্য *hr_বাফার = hb_বাফার_get_user_তথ্য (বাফার, &hb_বাফার_চাবি);
  যদি (unlikely (!hr_বাফার))
  {
    hr_বাফার = hb_harfrust_বাফার_create_rs ();
    যদি (unlikely (!hr_বাফার))
      ফেরত মিথ্যা;

    যদি (!hb_বাফার_set_user_তথ্য (বাফার,
				  &hb_বাফার_চাবি,
				  hr_বাফার,
				  hb_harfrust_বাফার_destroy_rs,
				  মিথ্যা))
    {
      hb_harfrust_বাফার_destroy_rs (hr_বাফার);
      যাও retry_বাফার;
    }
  }

  শূন্য *hr_shape_plan = nullptr;

retry_shape_plan:
  hr_shape_plan = shape_plan->harfrust_তথ্য.get_acquire ();
  যদি (unlikely (!hr_shape_plan))
  {
    hr_shape_plan = _hb_harfrust_shape_plan_create_rs (font_তথ্য,
						       shape_plan->key.props.script,
						       shape_plan->key.props.language,
						       shape_plan->key.props.direction,
						       features, অঙ্ক_features);
    যদি (hr_shape_plan &&
	!shape_plan->harfrust_তথ্য.cmpexch (nullptr, hr_shape_plan))
    {
      _hb_harfrust_shape_plan_destroy_rs (hr_shape_plan);
      যাও retry_shape_plan;
    }
  }

  ফেরত _hb_harfrust_shape_rs (font_তথ্য,
				hr_shape_plan,
				hr_বাফার,
				font,
				বাফার,
				বাফার->context[0],
				বাফার->context_len[0],
				বাফার->context[1],
				বাফার->context_len[1],
				features,
				অঙ্ক_features);
}


#endif
