/*
 * Copyright © 2009  Red Hat, Inc.
 * Copyright © 2012  Google, Inc.
 *
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-shaper.hh"
#అన్తర్భావ "hb-shape-plan.hh"
#అన్తర్భావ "hb-buffer.hh"
#అన్తర్భావ "hb-font.hh"
#అన్తర్భావ "hb-machinery.hh"


#నిర్వచితం_కాకపోతే HB_NO_SHAPER

/**
 * SECTION:hb-shape
 * @title: hb-shape
 * @short_description: Conversion of text strings into positioned glyphs
 * @include: hb.h
 *
 * Shaping is the central operation of HarfBuzz. Shaping operates on buffers,
 * which are sequences of Unicode characters that use the same font and have
 * the same text direction, script, and language. After shaping the buffer
 * contains the output glyphs and their positions.
 **/


స్థిర అంతఃవరుస శూన్య free_static_shaper_list ();

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి nil_shaper_list[] = {శూన్యసూచిక};

స్థిర నిర్మాణం hb_shaper_list_lazy_loader_t : hb_lazy_loader_t<స్థిరరాశి అక్షరం *,
							      hb_shaper_list_lazy_loader_t>
{
  స్థిర స్థిరరాశి అక్షరం ** create ()
  {
    స్థిరరాశి అక్షరం **shaper_list = (స్థిరరాశి అక్షరం **) hb_calloc (1 + HB_SHAPERS_లెక్క, పరిమాణం (స్థిరరాశి అక్షరం *));
    ఒకవేళ (unlikely (!shaper_list))
      తిరిగి శూన్యసూచిక;

    స్థిరరాశి hb_shaper_నమోదు_t *shapers = _hb_shapers_get ();
    గుర్తులేని పూర్ణాంకం i;
    కొరకు (i = 0; i < HB_SHAPERS_లెక్క; i++)
      shaper_list[i] = shapers[i].name;
    shaper_list[i] = శూన్యసూచిక;

    hb_atexit (free_static_shaper_list);

    తిరిగి shaper_list;
  }
  స్థిర శూన్య destroy (స్థిరరాశి అక్షరం **l)
  { hb_free (l); }
  స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి * get_null ()
  { తిరిగి nil_shaper_list; }
} static_shaper_list;

స్థిర అంతఃవరుస
శూన్య free_static_shaper_list ()
{
  static_shaper_list.free_instance ();
}


/**
 * hb_shape_list_shapers:
 *
 * Retrieves the list of shapers supported by HarfBuzz.
 *
 * Return value: (transfer none) (array zero-terminated=1): a
 *    `NULL`-terminated array of supported shapers constant string.
 *    The returned array is owned by HarfBuzz and should not be
 *    modified or freed.
 *
 * Since: 0.9.2
 **/
స్థిరరాశి అక్షరం **
hb_shape_list_shapers ()
{
  తిరిగి static_shaper_list.get_unconst ();
}


/**
 * hb_shape_full:
 * @font: an #hb_font_t to use for shaping
 * @buffer: an #hb_buffer_t to shape
 * @features: (array length=num_features) (nullable): an array of user
 *    specified #hb_feature_t or `NULL`
 * @num_features: the length of @features array
 * @shaper_list: (array zero-terminated=1) (nullable): a `NULL`-terminated
 *    array of shapers to use or `NULL`
 *
 * See hb_shape() for details. If @shaper_list is not `NULL`, the specified
 * shapers will be used in the given order, otherwise the default shapers list
 * will be used.
 *
 * Return value: false if all shapers failed, true otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_shape_నిండిన (hb_font_t          *font,
	       hb_బఫరు_t        *బఫరు,
	       స్థిరరాశి hb_feature_t *features,
	       గుర్తులేని పూర్ణాంకం        అంకె_features,
	       స్థిరరాశి అక్షరం * స్థిరరాశి *shaper_list)
{
  ఒకవేళ (unlikely (!బఫరు->len))
    తిరిగి నిజం;

  బఫరు->enter ();

  hb_బఫరు_t *పాఠ్యం_బఫరు = శూన్యసూచిక;
  ఒకవేళ (బఫరు->flags & HB_బఫరు_FLAG_VERIFY)
  {
    పాఠ్యం_బఫరు = hb_బఫరు_create ();
    hb_బఫరు_append (పాఠ్యం_బఫరు, బఫరు, 0, -1);
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached2 (font->face, &బఫరు->props,
							      features, అంకె_features,
							      font->coords, font->has_nonzero_coords ? font->అంకె_coords : 0,
							      shaper_list);

  hb_bool_t res = hb_shape_plan_execute (shape_plan, font, బఫరు, features, అంకె_features);

  hb_shape_plan_destroy (shape_plan);

  ఒకవేళ (పాఠ్యం_బఫరు)
  {
    ఒకవేళ (res && బఫరు->successful
	    && పాఠ్యం_బఫరు->successful
	    && !బఫరు->verify (పాఠ్యం_బఫరు,
				font,
				features,
				అంకె_features,
				shaper_list))
      res = అబద్ధం;
    hb_బఫరు_destroy (పాఠ్యం_బఫరు);
  }

  బఫరు->leave ();

  తిరిగి res;
}

/**
 * hb_shape:
 * @font: an #hb_font_t to use for shaping
 * @buffer: an #hb_buffer_t to shape
 * @features: (array length=num_features) (nullable): an array of user
 *    specified #hb_feature_t or `NULL`
 * @num_features: the length of @features array
 *
 * Shapes @buffer using @font turning its Unicode characters content to
 * positioned glyphs. If @features is not `NULL`, it will be used to control the
 * features applied during shaping. If two @features have the same tag but
 * overlapping ranges the value of the feature with the higher index takes
 * precedence.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_shape (hb_font_t           *font,
	  hb_బఫరు_t         *బఫరు,
	  స్థిరరాశి hb_feature_t  *features,
	  గుర్తులేని పూర్ణాంకం         అంకె_features)
{
  hb_shape_నిండిన (font, బఫరు, features, అంకె_features, శూన్యసూచిక);
}


#నిర్వచితమైతే HB_EXPERIMENTAL_API
#నిర్వచితం_కాకపోతే HB_NO_VAR

స్థిర దశాంశం
బఫరు_advance (hb_బఫరు_t *బఫరు)
{
  దశాంశం a = 0;
  స్వయంచాలక *pos = బఫరు->pos;
  గుర్తులేని లెక్క = బఫరు->len;
  ఒకవేళ (HB_DIRECTION_IS_HORIZONTAL (బఫరు->props.direction))
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      a += pos[i].x_advance;
  లేకపోతే
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      a += pos[i].y_advance;
  తిరిగి a;
}

స్థిర శూన్య
reset_బఫరు (hb_బఫరు_t *బఫరు,
	      hb_array_t<స్థిరరాశి hb_glyph_info_t> పాఠ్యం)
{
  ప్రతిజ్ఞా (బఫరు->ensure (పాఠ్యం.నిడివి));
  బఫరు->have_positions = అబద్ధం;
  బఫరు->len = పాఠ్యం.నిడివి;
  hb_memcpy (బఫరు->info, పాఠ్యం.arrayZ, పాఠ్యం.నిడివి * పరిమాణం (బఫరు->info[0]));
  hb_బఫరు_సమితి_content_type (బఫరు, HB_బఫరు_CONTENT_TYPE_UNICODE);
}

/**
 * hb_shape_justify:
 * @font: a mutable #hb_font_t to use for shaping
 * @buffer: an #hb_buffer_t to shape
 * @features: (array length=num_features) (nullable): an array of user
 *    specified #hb_feature_t or `NULL`
 * @num_features: the length of @features array
 * @shaper_list: (array zero-terminated=1) (nullable): a `NULL`-terminated
 *    array of shapers to use or `NULL`
 * @min_target_advance: Minimum advance width/height to aim for.
 * @max_target_advance: Maximum advance width/height to aim for.
 * @advance: (inout): Input/output advance width/height of the buffer.
 * @var_tag: (out): Variation-axis tag used for justification.
 * @var_value: (out): Variation-axis value used to reach target justification.
 *
 * See hb_shape_full() for basic details. If @shaper_list is not `NULL`, the specified
 * shapers will be used in the given order, otherwise the default shapers list
 * will be used.
 *
 * In addition, justify the shaping results such that the shaping results reach
 * the target advance width/height, depending on the buffer direction.
 *
 * If the advance of the buffer shaped with hb_shape_full() is already known,
 * put that in *advance. Otherwise set *advance to zero.
 *
 * This API is currently experimental and will probably change in the future.
 *
 * Return value: false if all shapers failed, true otherwise
 *
 * XSince: EXPERIMENTAL
 **/
hb_bool_t
hb_shape_justify (hb_font_t          *font,
		  hb_బఫరు_t        *బఫరు,
		  స్థిరరాశి hb_feature_t *features,
		  గుర్తులేని పూర్ణాంకం        అంకె_features,
		  స్థిరరాశి అక్షరం * స్థిరరాశి *shaper_list,
		  దశాంశం               min_లక్ష్యం_advance,
		  దశాంశం               max_లక్ష్యం_advance,
		  దశాంశం              *advance, /* IN/OUT */
		  hb_ట్యాగ్_t           *var_ట్యాగ్, /*బహిర్గతం*/
		  దశాంశం              *var_విలువ /*బహిర్గతం*/)
{
  // TODO Negative font scales?

  /* If default advance already matches target, nothing to do. Shape and return. */
  ఒకవేళ (min_లక్ష్యం_advance <= *advance && *advance <= max_లక్ష్యం_advance)
  {
    *var_ట్యాగ్ = HB_ట్యాగ్_NONE;
    *var_విలువ = 0.0f;
    తిరిగి hb_shape_నిండిన (font, బఫరు,
			  features, అంకె_features,
			  shaper_list);
  }

  hb_face_t *face = font->face;

  /* Choose variation tag to use for justification. */

  hb_ట్యాగ్_t ట్యాగ్_2 = HB_ట్యాగ్_NONE;
  hb_ot_var_axis_info_t axis_info;

  hb_ట్యాగ్_t ట్యాగులు[] =
  {
    HB_ట్యాగ్ ('j','s','t','f'),
    HB_ట్యాగ్ ('w','d','t','h'),
  };
  కొరకు (గుర్తులేని i = 0; i < ARRAY_నిడివి (ట్యాగులు); i++)
    ఒకవేళ (hb_ot_var_కనుగొను_axis_info (face, ట్యాగులు[i], &axis_info))
    {
      ట్యాగ్_2 = *var_ట్యాగ్ = ట్యాగులు[i];
      ఆపు;
    }

  /* If no suitable variation axis found, can't justify.  Just shape and return. */
  ఒకవేళ (!ట్యాగ్_2)
  {
    *var_ట్యాగ్ = HB_ట్యాగ్_NONE;
    *var_విలువ = 0.0f;
    ఒకవేళ (hb_shape_నిండిన (font, బఫరు,
		       features, అంకె_features,
		       shaper_list))
    {
      *advance = బఫరు_advance (బఫరు);
      తిరిగి నిజం;
    }
    లేకపోతే
      తిరిగి అబద్ధం;
  }

  /* Copy buffer text as we need it so we can shape multiple times. */
  గుర్తులేని పాఠ్యం_len = బఫరు->len;
  స్వయంచాలక *పాఠ్యం_info = (hb_glyph_info_t *) hb_malloc2 (పాఠ్యం_len, పరిమాణం (బఫరు->info[0]));
  ఒకవేళ (unlikely (పాఠ్యం_len && !పాఠ్యం_info))
    తిరిగి అబద్ధం;
  HB_SCOPE_GUARD (hb_free (పాఠ్యం_info));
  hb_memcpy (పాఠ్యం_info, బఫరు->info, పాఠ్యం_len * పరిమాణం (బఫరు->info[0]));
  స్వయంచాలక పాఠ్యం = hb_array<స్థిరరాశి hb_glyph_info_t> (పాఠ్యం_info, పాఠ్యం_len);

  /* If default advance was not provided to us, calculate it. */
  ఒకవేళ (!*advance)
  {
    hb_font_సమితి_variation (font, ట్యాగ్_2, axis_info.default_విలువ);
    ఒకవేళ (!hb_shape_నిండిన (font, బఫరు,
			features, అంకె_features,
			shaper_list))
      తిరిగి అబద్ధం;
    *advance = బఫరు_advance (బఫరు);
  }

  /* If default advance already matches target, nothing to do. Shape and return.
   * Do this again, in case advance was just calculated.
   */
  ఒకవేళ (min_లక్ష్యం_advance <= *advance && *advance <= max_లక్ష్యం_advance)
  {
    *var_ట్యాగ్ = HB_ట్యాగ్_NONE;
    *var_విలువ = 0.0f;
    తిరిగి నిజం;
  }

  /* Prepare for running the solver. */
  ద్విగుణ a, b, ya, yb;
  ఒకవేళ (*advance < min_లక్ష్యం_advance)
  {
    /* Need to expand. */
    ya = (ద్విగుణ) *advance;
    a = (ద్విగుణ) axis_info.default_విలువ;
    b = (ద్విగుణ) axis_info.max_విలువ;

    /* Shape buffer for maximum expansion to use as other
     * starting point for the solver. */
    hb_font_సమితి_variation (font, ట్యాగ్_2, (దశాంశం) b);
    reset_బఫరు (బఫరు, పాఠ్యం);
    ఒకవేళ (!hb_shape_నిండిన (font, బఫరు,
			features, అంకె_features,
			shaper_list))
      తిరిగి అబద్ధం;
    yb = (ద్విగుణ) బఫరు_advance (బఫరు);
    /* If the maximum expansion is less than max target,
     * there's nothing to solve for. Just return it. */
    ఒకవేళ (yb <= (ద్విగుణ) max_లక్ష్యం_advance)
    {
      *var_విలువ = (దశాంశం) b;
      *advance = (దశాంశం) yb;
      తిరిగి నిజం;
    }
  }
  లేకపోతే
  {
    /* Need to shrink. */
    yb = (ద్విగుణ) *advance;
    a = (ద్విగుణ) axis_info.min_విలువ;
    b = (ద్విగుణ) axis_info.default_విలువ;

    /* Shape buffer for maximum shrinkate to use as other
     * starting point for the solver. */
    hb_font_సమితి_variation (font, ట్యాగ్_2, (దశాంశం) a);
    reset_బఫరు (బఫరు, పాఠ్యం);
    ఒకవేళ (!hb_shape_నిండిన (font, బఫరు,
			features, అంకె_features,
			shaper_list))
      తిరిగి అబద్ధం;
    ya = (ద్విగుణ) బఫరు_advance (బఫరు);
    /* If the maximum shrinkate is more than min target,
     * there's nothing to solve for. Just return it. */
    ఒకవేళ (ya >= (ద్విగుణ) min_లక్ష్యం_advance)
    {
      *var_విలువ = (దశాంశం) a;
      *advance = (దశాంశం) ya;
      తిరిగి నిజం;
    }
  }

  /* Run the solver to find a var axis value that hits
   * the desired width. */

  ద్విగుణ epsilon = (b - a) / (1<<14);
  తార్కిక failed = అబద్ధం;

  స్వయంచాలక f = [&] (ద్విగుణ x)
  {
    hb_font_సమితి_variation (font, ట్యాగ్_2, (దశాంశం) x);
    reset_బఫరు (బఫరు, పాఠ్యం);
    ఒకవేళ (unlikely (!hb_shape_నిండిన (font, బఫరు,
				  features, అంకె_features,
				  shaper_list)))
    {
      failed = నిజం;
      తిరిగి (ద్విగుణ) min_లక్ష్యం_advance;
    }

    ద్విగుణ w = (ద్విగుణ) బఫరు_advance (బఫరు);
    DEBUG_సందేశం (JUSTIFY, శూన్యసూచిక, "Trying '%c%c%c%c' axis parameter %f. Advance %g. Target: min %g max %g",
	       HB_UNTAG (ట్యాగ్_2), x, w,
	       (ద్విగుణ) min_లక్ష్యం_advance, (ద్విగుణ) max_లక్ష్యం_advance);
    తిరిగి w;
  };

  ద్విగుణ y = 0;
  ద్విగుణ itp = solve_itp (f,
			  a, b,
			  epsilon,
			  (ద్విగుణ) min_లక్ష్యం_advance, (ద్విగుణ) max_లక్ష్యం_advance,
			  ya, yb, y);

  ఒకవేళ (failed)
    తిరిగి అబద్ధం;

  *var_విలువ = (దశాంశం) itp;
  *advance = (దశాంశం) y;

  తిరిగి నిజం;
}
#అయితే_ముగింపు
#అయితే_ముగింపు


#అయితే_ముగింపు
