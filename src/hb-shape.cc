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

#समावेश "hb.hh"

#समावेश "hb-shaper.hh"
#समावेश "hb-shape-plan.hh"
#समावेश "hb-buffer.hh"
#समावेश "hb-font.hh"
#समावेश "hb-machinery.hh"


#अगर_अघोषित HB_NO_SHAPER

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


अटल अंतरभूत व्योम free_static_shaper_list ();

अटल स्थिर अक्षर * स्थिर nil_shaper_list[] = {nullptr};

अटल काष्ठा hb_shaper_list_lazy_loader_t : hb_lazy_loader_t<स्थिर अक्षर *,
							      hb_shaper_list_lazy_loader_t>
{
  अटल स्थिर अक्षर ** create ()
  {
    स्थिर अक्षर **shaper_list = (स्थिर अक्षर **) hb_calloc (1 + HB_SHAPERS_गिनती, माप (स्थिर अक्षर *));
    अगर (unlikely (!shaper_list))
      वापस nullptr;

    स्थिर hb_shaper_entry_t *shapers = _hb_shapers_get ();
    अचिन्हित पूर्णांक i;
    क्रम (i = 0; i < HB_SHAPERS_गिनती; i++)
      shaper_list[i] = shapers[i].name;
    shaper_list[i] = nullptr;

    hb_atexit (free_static_shaper_list);

    वापस shaper_list;
  }
  अटल व्योम destroy (स्थिर अक्षर **l)
  { hb_free (l); }
  अटल स्थिर अक्षर * स्थिर * get_null ()
  { वापस nil_shaper_list; }
} static_shaper_list;

अटल अंतरभूत
व्योम free_static_shaper_list ()
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
स्थिर अक्षर **
hb_shape_list_shapers ()
{
  वापस static_shaper_list.get_unconst ();
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
hb_shape_full (hb_font_t          *font,
	       hb_बफर_t        *बफर,
	       स्थिर hb_feature_t *features,
	       अचिन्हित पूर्णांक        अंक_features,
	       स्थिर अक्षर * स्थिर *shaper_list)
{
  अगर (unlikely (!बफर->len))
    वापस true;

  बफर->enter ();

  hb_बफर_t *text_बफर = nullptr;
  अगर (बफर->flags & HB_बफर_FLAG_VERIFY)
  {
    text_बफर = hb_बफर_create ();
    hb_बफर_append (text_बफर, बफर, 0, -1);
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached2 (font->face, &बफर->props,
							      features, अंक_features,
							      font->coords, font->has_nonzero_coords ? font->अंक_coords : 0,
							      shaper_list);

  hb_bool_t res = hb_shape_plan_execute (shape_plan, font, बफर, features, अंक_features);

  hb_shape_plan_destroy (shape_plan);

  अगर (text_बफर)
  {
    अगर (res && बफर->successful
	    && text_बफर->successful
	    && !बफर->verify (text_बफर,
				font,
				features,
				अंक_features,
				shaper_list))
      res = false;
    hb_बफर_destroy (text_बफर);
  }

  बफर->leave ();

  वापस res;
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
व्योम
hb_shape (hb_font_t           *font,
	  hb_बफर_t         *बफर,
	  स्थिर hb_feature_t  *features,
	  अचिन्हित पूर्णांक         अंक_features)
{
  hb_shape_full (font, बफर, features, अंक_features, nullptr);
}


#अगर_घोषित HB_EXPERIMENTAL_API
#अगर_अघोषित HB_NO_VAR

अटल भग्न
बफर_advance (hb_बफर_t *बफर)
{
  भग्न a = 0;
  स्वतः *pos = बफर->pos;
  अचिन्हित गिनती = बफर->len;
  अगर (HB_DIRECTION_IS_HORIZONTAL (बफर->props.direction))
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      a += pos[i].x_advance;
  अन्यथा
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      a += pos[i].y_advance;
  वापस a;
}

अटल व्योम
reset_बफर (hb_बफर_t *बफर,
	      hb_array_t<स्थिर hb_glyph_info_t> text)
{
  निश्चित (बफर->ensure (text.लंबाई));
  बफर->have_positions = false;
  बफर->len = text.लंबाई;
  hb_memcpy (बफर->info, text.arrayZ, text.लंबाई * माप (बफर->info[0]));
  hb_बफर_set_content_type (बफर, HB_बफर_CONTENT_TYPE_UNICODE);
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
		  hb_बफर_t        *बफर,
		  स्थिर hb_feature_t *features,
		  अचिन्हित पूर्णांक        अंक_features,
		  स्थिर अक्षर * स्थिर *shaper_list,
		  भग्न               min_target_advance,
		  भग्न               max_target_advance,
		  भग्न              *advance, /* IN/OUT */
		  hb_tag_t           *var_tag, /* OUT */
		  भग्न              *var_मूल्य /* OUT */)
{
  // TODO Negative font scales?

  /* If default advance already matches target, nothing to do. Shape and return. */
  अगर (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_मूल्य = 0.0f;
    वापस hb_shape_full (font, बफर,
			  features, अंक_features,
			  shaper_list);
  }

  hb_face_t *face = font->face;

  /* Choose variation tag to use for justification. */

  hb_tag_t tag = HB_TAG_NONE;
  hb_ot_var_axis_info_t axis_info;

  hb_tag_t tags[] =
  {
    HB_TAG ('j','s','t','f'),
    HB_TAG ('w','d','t','h'),
  };
  क्रम (अचिन्हित i = 0; i < ARRAY_लंबाई (tags); i++)
    अगर (hb_ot_var_find_axis_info (face, tags[i], &axis_info))
    {
      tag = *var_tag = tags[i];
      अवरोध;
    }

  /* If no suitable variation axis found, can't justify.  Just shape and return. */
  अगर (!tag)
  {
    *var_tag = HB_TAG_NONE;
    *var_मूल्य = 0.0f;
    अगर (hb_shape_full (font, बफर,
		       features, अंक_features,
		       shaper_list))
    {
      *advance = बफर_advance (बफर);
      वापस true;
    }
    अन्यथा
      वापस false;
  }

  /* Copy buffer text as we need it so we can shape multiple times. */
  अचिन्हित text_len = बफर->len;
  स्वतः *text_info = (hb_glyph_info_t *) hb_malloc2 (text_len, माप (बफर->info[0]));
  अगर (unlikely (text_len && !text_info))
    वापस false;
  HB_SCOPE_GUARD (hb_free (text_info));
  hb_memcpy (text_info, बफर->info, text_len * माप (बफर->info[0]));
  स्वतः text = hb_array<स्थिर hb_glyph_info_t> (text_info, text_len);

  /* If default advance was not provided to us, calculate it. */
  अगर (!*advance)
  {
    hb_font_set_variation (font, tag, axis_info.default_मूल्य);
    अगर (!hb_shape_full (font, बफर,
			features, अंक_features,
			shaper_list))
      वापस false;
    *advance = बफर_advance (बफर);
  }

  /* If default advance already matches target, nothing to do. Shape and return.
   * Do this again, in case advance was just calculated.
   */
  अगर (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_मूल्य = 0.0f;
    वापस true;
  }

  /* Prepare for running the solver. */
  द्विगुन a, b, ya, yb;
  अगर (*advance < min_target_advance)
  {
    /* Need to expand. */
    ya = (द्विगुन) *advance;
    a = (द्विगुन) axis_info.default_मूल्य;
    b = (द्विगुन) axis_info.max_मूल्य;

    /* Shape buffer for maximum expansion to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (भग्न) b);
    reset_बफर (बफर, text);
    अगर (!hb_shape_full (font, बफर,
			features, अंक_features,
			shaper_list))
      वापस false;
    yb = (द्विगुन) बफर_advance (बफर);
    /* If the maximum expansion is less than max target,
     * there's nothing to solve for. Just return it. */
    अगर (yb <= (द्विगुन) max_target_advance)
    {
      *var_मूल्य = (भग्न) b;
      *advance = (भग्न) yb;
      वापस true;
    }
  }
  अन्यथा
  {
    /* Need to shrink. */
    yb = (द्विगुन) *advance;
    a = (द्विगुन) axis_info.min_मूल्य;
    b = (द्विगुन) axis_info.default_मूल्य;

    /* Shape buffer for maximum shrinkate to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (भग्न) a);
    reset_बफर (बफर, text);
    अगर (!hb_shape_full (font, बफर,
			features, अंक_features,
			shaper_list))
      वापस false;
    ya = (द्विगुन) बफर_advance (बफर);
    /* If the maximum shrinkate is more than min target,
     * there's nothing to solve for. Just return it. */
    अगर (ya >= (द्विगुन) min_target_advance)
    {
      *var_मूल्य = (भग्न) a;
      *advance = (भग्न) ya;
      वापस true;
    }
  }

  /* Run the solver to find a var axis value that hits
   * the desired width. */

  द्विगुन epsilon = (b - a) / (1<<14);
  bool failed = false;

  स्वतः f = [&] (द्विगुन x)
  {
    hb_font_set_variation (font, tag, (भग्न) x);
    reset_बफर (बफर, text);
    अगर (unlikely (!hb_shape_full (font, बफर,
				  features, अंक_features,
				  shaper_list)))
    {
      failed = true;
      वापस (द्विगुन) min_target_advance;
    }

    द्विगुन w = (द्विगुन) बफर_advance (बफर);
    DEBUG_संदेश (JUSTIFY, nullptr, "Trying '%c%c%c%c' axis parameter %f. Advance %g. Target: min %g max %g",
	       HB_UNTAG (tag), x, w,
	       (द्विगुन) min_target_advance, (द्विगुन) max_target_advance);
    वापस w;
  };

  द्विगुन y = 0;
  द्विगुन itp = solve_itp (f,
			  a, b,
			  epsilon,
			  (द्विगुन) min_target_advance, (द्विगुन) max_target_advance,
			  ya, yb, y);

  अगर (failed)
    वापस false;

  *var_मूल्य = (भग्न) itp;
  *advance = (भग्न) y;

  वापस true;
}
#पूर्ण_अगर
#पूर्ण_अगर


#पूर्ण_अगर
