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

#అన్తర్భావ "hb.hh"

#నిర్వచితమైతే HAVE_HARFRUST

#అన్తర్భావ "hb-shaper-impl.hh"

#అన్తర్భావ "hb-utf.hh"


/*
 * buffer
 */
బాహ్య "C" శూన్య *
hb_harfrust_బఫరు_create_rs (శూన్య);

బాహ్య "C" శూన్య
hb_harfrust_బఫరు_destroy_rs (శూన్య *దత్తాంశం);


/*
 * shaper face data
 */

బాహ్య "C" శూన్య *
hb_harfrust_shaper_face_దత్తాంశం_create_rs (hb_face_t *face);

hb_harfrust_face_దత్తాంశం_t *
hb_harfrust_shaper_face_దత్తాంశం_create (hb_face_t *face)
{
  తిరిగి (hb_harfrust_face_దత్తాంశం_t *) hb_harfrust_shaper_face_దత్తాంశం_create_rs (face);
}

బాహ్య "C" శూన్య
hb_harfrust_shaper_face_దత్తాంశం_destroy_rs (శూన్య *దత్తాంశం);

శూన్య
hb_harfrust_shaper_face_దత్తాంశం_destroy (hb_harfrust_face_దత్తాంశం_t *దత్తాంశం)
{
  hb_harfrust_shaper_face_దత్తాంశం_destroy_rs (దత్తాంశం);
}


/*
 * shaper font data
 */

బాహ్య "C" శూన్య *
hb_harfrust_shaper_font_దత్తాంశం_create_rs (hb_font_t *font, స్థిరరాశి శూన్య *face_దత్తాంశం);

hb_harfrust_font_దత్తాంశం_t *
hb_harfrust_shaper_font_దత్తాంశం_create (hb_font_t *font)
{
  స్థిరరాశి hb_harfrust_face_దత్తాంశం_t *face_దత్తాంశం = font->face->దత్తాంశం.harfrust;
  తిరిగి (hb_harfrust_font_దత్తాంశం_t *) hb_harfrust_shaper_font_దత్తాంశం_create_rs (font, face_దత్తాంశం);
}

బాహ్య "C" శూన్య
hb_harfrust_shaper_font_దత్తాంశం_destroy_rs (శూన్య *దత్తాంశం);

శూన్య
hb_harfrust_shaper_font_దత్తాంశం_destroy (hb_harfrust_font_దత్తాంశం_t *దత్తాంశం)
{
  hb_harfrust_shaper_font_దత్తాంశం_destroy_rs (దత్తాంశం);
}


/*
 * shape plan
 */

బాహ్య "C" శూన్య *
_hb_harfrust_shape_plan_create_rs (స్థిరరాశి శూన్య *font_దత్తాంశం,
				   hb_script_t script,
				   hb_language_t language,
				   hb_direction_t direction,
				   స్థిరరాశి hb_feature_t *features,
				   గుర్తులేని పూర్ణాంకం        అంకె_features);

బాహ్య "C" శూన్య
_hb_harfrust_shape_plan_destroy_rs (శూన్య *దత్తాంశం);


/*
 * shaper
 */

బాహ్య "C" hb_bool_t
_hb_harfrust_shape_rs (స్థిరరాశి శూన్య         *font_దత్తాంశం,
		       స్థిరరాశి శూన్య         *rs_shape_plan,
		       స్థిరరాశి శూన్య         *rs_బఫరు,
		       hb_font_t          *font,
		       hb_బఫరు_t        *బఫరు,
		       స్థిరరాశి hb_codepoint_t *pre_context,
		       uint32_t            pre_context_len,
		       స్థిరరాశి hb_codepoint_t *post_context,
		       uint32_t            post_context_len,
		       స్థిరరాశి hb_feature_t *features,
		       గుర్తులేని పూర్ణాంకం        అంకె_features);

స్థిర hb_user_దత్తాంశం_కీలకం_t hb_బఫరు_కీలకం = {0};

hb_bool_t
_hb_harfrust_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_బఫరు_t        *బఫరు,
		    స్థిరరాశి hb_feature_t *features,
		    గుర్తులేని పూర్ణాంకం        అంకె_features)
{
  స్థిరరాశి hb_harfrust_font_దత్తాంశం_t *font_దత్తాంశం = font->దత్తాంశం.harfrust;

retry_బఫరు:
  శూన్య *hr_బఫరు = hb_బఫరు_get_user_దత్తాంశం (బఫరు, &hb_బఫరు_కీలకం);
  ఒకవేళ (unlikely (!hr_బఫరు))
  {
    hr_బఫరు = hb_harfrust_బఫరు_create_rs ();
    ఒకవేళ (unlikely (!hr_బఫరు))
      తిరిగి అబద్ధం;

    ఒకవేళ (!hb_బఫరు_సమితి_user_దత్తాంశం (బఫరు,
				  &hb_బఫరు_కీలకం,
				  hr_బఫరు,
				  hb_harfrust_బఫరు_destroy_rs,
				  అబద్ధం))
    {
      hb_harfrust_బఫరు_destroy_rs (hr_బఫరు);
      వెళ్ళు retry_బఫరు;
    }
  }

  శూన్య *hr_shape_plan = శూన్యసూచిక;

retry_shape_plan:
  hr_shape_plan = shape_plan->harfrust_దత్తాంశం.get_acquire ();
  ఒకవేళ (unlikely (!hr_shape_plan))
  {
    hr_shape_plan = _hb_harfrust_shape_plan_create_rs (font_దత్తాంశం,
						       shape_plan->key.props.script,
						       shape_plan->key.props.language,
						       shape_plan->key.props.direction,
						       features, అంకె_features);
    ఒకవేళ (hr_shape_plan &&
	!shape_plan->harfrust_దత్తాంశం.cmpexch (శూన్యసూచిక, hr_shape_plan))
    {
      _hb_harfrust_shape_plan_destroy_rs (hr_shape_plan);
      వెళ్ళు retry_shape_plan;
    }
  }

  తిరిగి _hb_harfrust_shape_rs (font_దత్తాంశం,
				hr_shape_plan,
				hr_బఫరు,
				font,
				బఫరు,
				బఫరు->context[0],
				బఫరు->context_len[0],
				బఫరు->context[1],
				బఫరు->context_len[1],
				features,
				అంకె_features);
}


#అయితే_ముగింపు
