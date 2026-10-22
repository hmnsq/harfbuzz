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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shape-plan.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-buffer.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-font.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SHAPER

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


ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_shaper_list ();

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ nil_shaper_list[] = {ਖਾਲੀ_ਸੂਚਕ};

ਸਥਿਰ ਬਣਤਰ hb_shaper_list_lazy_loader_t : hb_lazy_loader_t<ਅਚਲ ਅੱਖਰ *,
							      hb_shaper_list_lazy_loader_t>
{
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ ** create ()
  {
    ਅਚਲ ਅੱਖਰ **shaper_list = (ਅਚਲ ਅੱਖਰ **) hb_calloc (1 + HB_SHAPERS_ਗਿਣਤੀ, ਆਕਾਰ (ਅਚਲ ਅੱਖਰ *));
    ਜੇ (unlikely (!shaper_list))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    ਅਚਲ hb_shaper_ਇੰਦਰਾਜ_t *shapers = _hb_shapers_get ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
    ਲਈ (i = 0; i < HB_SHAPERS_ਗਿਣਤੀ; i++)
      shaper_list[i] = shapers[i].name;
    shaper_list[i] = ਖਾਲੀ_ਸੂਚਕ;

    hb_atexit (free_static_shaper_list);

    ਵਾਪਸ shaper_list;
  }
  ਸਥਿਰ ਖਾਲੀ destroy (ਅਚਲ ਅੱਖਰ **l)
  { hb_free (l); }
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ * get_null ()
  { ਵਾਪਸ nil_shaper_list; }
} static_shaper_list;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_shaper_list ()
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
ਅਚਲ ਅੱਖਰ **
hb_shape_list_shapers ()
{
  ਵਾਪਸ static_shaper_list.get_unconst ();
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
hb_shape_ਭਰਿਆ (hb_font_t          *font,
	       hb_ਬਫਰ_t        *ਬਫਰ,
	       ਅਚਲ hb_feature_t *features,
	       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features,
	       ਅਚਲ ਅੱਖਰ * ਅਚਲ *shaper_list)
{
  ਜੇ (unlikely (!ਬਫਰ->len))
    ਵਾਪਸ ਸੱਚ;

  ਬਫਰ->enter ();

  hb_ਬਫਰ_t *ਪਾਠ_ਬਫਰ = ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (ਬਫਰ->flags & HB_ਬਫਰ_FLAG_VERIFY)
  {
    ਪਾਠ_ਬਫਰ = hb_ਬਫਰ_create ();
    hb_ਬਫਰ_append (ਪਾਠ_ਬਫਰ, ਬਫਰ, 0, -1);
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached2 (font->face, &ਬਫਰ->props,
							      features, ਅੰਕ_features,
							      font->coords, font->has_nonzero_coords ? font->ਅੰਕ_coords : 0,
							      shaper_list);

  hb_bool_t res = hb_shape_plan_execute (shape_plan, font, ਬਫਰ, features, ਅੰਕ_features);

  hb_shape_plan_destroy (shape_plan);

  ਜੇ (ਪਾਠ_ਬਫਰ)
  {
    ਜੇ (res && ਬਫਰ->successful
	    && ਪਾਠ_ਬਫਰ->successful
	    && !ਬਫਰ->verify (ਪਾਠ_ਬਫਰ,
				font,
				features,
				ਅੰਕ_features,
				shaper_list))
      res = ਝੂਠ;
    hb_ਬਫਰ_destroy (ਪਾਠ_ਬਫਰ);
  }

  ਬਫਰ->leave ();

  ਵਾਪਸ res;
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
ਖਾਲੀ
hb_shape (hb_font_t           *font,
	  hb_ਬਫਰ_t         *ਬਫਰ,
	  ਅਚਲ hb_feature_t  *features,
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ         ਅੰਕ_features)
{
  hb_shape_ਭਰਿਆ (font, ਬਫਰ, features, ਅੰਕ_features, ਖਾਲੀ_ਸੂਚਕ);
}


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR

ਸਥਿਰ ਦਸ਼ਮਲਵ
ਬਫਰ_advance (hb_ਬਫਰ_t *ਬਫਰ)
{
  ਦਸ਼ਮਲਵ a = 0;
  ਸਵੈਚਾਲਿਤ *pos = ਬਫਰ->pos;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  ਜੇ (HB_DIRECTION_IS_HORIZONTAL (ਬਫਰ->props.direction))
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      a += pos[i].x_advance;
  ਨਹੀਂਤਾਂ
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      a += pos[i].y_advance;
  ਵਾਪਸ a;
}

ਸਥਿਰ ਖਾਲੀ
reset_ਬਫਰ (hb_ਬਫਰ_t *ਬਫਰ,
	      hb_array_t<ਅਚਲ hb_glyph_info_t> ਪਾਠ)
{
  ਪ੍ਰਤਿਜ੍ਞਾ (ਬਫਰ->ensure (ਪਾਠ.ਲੰਮਾਈ));
  ਬਫਰ->have_positions = ਝੂਠ;
  ਬਫਰ->len = ਪਾਠ.ਲੰਮਾਈ;
  hb_memcpy (ਬਫਰ->info, ਪਾਠ.arrayZ, ਪਾਠ.ਲੰਮਾਈ * ਆਕਾਰ (ਬਫਰ->info[0]));
  hb_ਬਫਰ_ਸੈੱਟ_content_type (ਬਫਰ, HB_ਬਫਰ_CONTENT_TYPE_UNICODE);
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
		  hb_ਬਫਰ_t        *ਬਫਰ,
		  ਅਚਲ hb_feature_t *features,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features,
		  ਅਚਲ ਅੱਖਰ * ਅਚਲ *shaper_list,
		  ਦਸ਼ਮਲਵ               min_ਨਿਸ਼ਾਨਾ_advance,
		  ਦਸ਼ਮਲਵ               max_ਨਿਸ਼ਾਨਾ_advance,
		  ਦਸ਼ਮਲਵ              *advance, /* IN/OUT */
		  hb_ਟੈਗ_t           *var_ਟੈਗ, /*ਬਾਹਰਲਾ*/
		  ਦਸ਼ਮਲਵ              *var_ਮੁੱਲ /*ਬਾਹਰਲਾ*/)
{
  // TODO Negative font scales?

  /* If default advance already matches target, nothing to do. Shape and return. */
  ਜੇ (min_ਨਿਸ਼ਾਨਾ_advance <= *advance && *advance <= max_ਨਿਸ਼ਾਨਾ_advance)
  {
    *var_ਟੈਗ = HB_ਟੈਗ_NONE;
    *var_ਮੁੱਲ = 0.0f;
    ਵਾਪਸ hb_shape_ਭਰਿਆ (font, ਬਫਰ,
			  features, ਅੰਕ_features,
			  shaper_list);
  }

  hb_face_t *face = font->face;

  /* Choose variation tag to use for justification. */

  hb_ਟੈਗ_t ਟੈਗ_2 = HB_ਟੈਗ_NONE;
  hb_ot_var_axis_info_t axis_info;

  hb_ਟੈਗ_t ਟੈਗ_ਸੂਚੀ[] =
  {
    HB_ਟੈਗ ('j','s','t','f'),
    HB_ਟੈਗ ('w','d','t','h'),
  };
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ARRAY_ਲੰਮਾਈ (ਟੈਗ_ਸੂਚੀ); i++)
    ਜੇ (hb_ot_var_ਲੱਭੋ_axis_info (face, ਟੈਗ_ਸੂਚੀ[i], &axis_info))
    {
      ਟੈਗ_2 = *var_ਟੈਗ = ਟੈਗ_ਸੂਚੀ[i];
      ਰੋਕੋ;
    }

  /* If no suitable variation axis found, can't justify.  Just shape and return. */
  ਜੇ (!ਟੈਗ_2)
  {
    *var_ਟੈਗ = HB_ਟੈਗ_NONE;
    *var_ਮੁੱਲ = 0.0f;
    ਜੇ (hb_shape_ਭਰਿਆ (font, ਬਫਰ,
		       features, ਅੰਕ_features,
		       shaper_list))
    {
      *advance = ਬਫਰ_advance (ਬਫਰ);
      ਵਾਪਸ ਸੱਚ;
    }
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ ਝੂਠ;
  }

  /* Copy buffer text as we need it so we can shape multiple times. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਾਠ_len = ਬਫਰ->len;
  ਸਵੈਚਾਲਿਤ *ਪਾਠ_info = (hb_glyph_info_t *) hb_malloc2 (ਪਾਠ_len, ਆਕਾਰ (ਬਫਰ->info[0]));
  ਜੇ (unlikely (ਪਾਠ_len && !ਪਾਠ_info))
    ਵਾਪਸ ਝੂਠ;
  HB_SCOPE_GUARD (hb_free (ਪਾਠ_info));
  hb_memcpy (ਪਾਠ_info, ਬਫਰ->info, ਪਾਠ_len * ਆਕਾਰ (ਬਫਰ->info[0]));
  ਸਵੈਚਾਲਿਤ ਪਾਠ = hb_array<ਅਚਲ hb_glyph_info_t> (ਪਾਠ_info, ਪਾਠ_len);

  /* If default advance was not provided to us, calculate it. */
  ਜੇ (!*advance)
  {
    hb_font_ਸੈੱਟ_variation (font, ਟੈਗ_2, axis_info.default_ਮੁੱਲ);
    ਜੇ (!hb_shape_ਭਰਿਆ (font, ਬਫਰ,
			features, ਅੰਕ_features,
			shaper_list))
      ਵਾਪਸ ਝੂਠ;
    *advance = ਬਫਰ_advance (ਬਫਰ);
  }

  /* If default advance already matches target, nothing to do. Shape and return.
   * Do this again, in case advance was just calculated.
   */
  ਜੇ (min_ਨਿਸ਼ਾਨਾ_advance <= *advance && *advance <= max_ਨਿਸ਼ਾਨਾ_advance)
  {
    *var_ਟੈਗ = HB_ਟੈਗ_NONE;
    *var_ਮੁੱਲ = 0.0f;
    ਵਾਪਸ ਸੱਚ;
  }

  /* Prepare for running the solver. */
  ਦੁੱਗਣਾ a, b, ya, yb;
  ਜੇ (*advance < min_ਨਿਸ਼ਾਨਾ_advance)
  {
    /* Need to expand. */
    ya = (ਦੁੱਗਣਾ) *advance;
    a = (ਦੁੱਗਣਾ) axis_info.default_ਮੁੱਲ;
    b = (ਦੁੱਗਣਾ) axis_info.max_ਮੁੱਲ;

    /* Shape buffer for maximum expansion to use as other
     * starting point for the solver. */
    hb_font_ਸੈੱਟ_variation (font, ਟੈਗ_2, (ਦਸ਼ਮਲਵ) b);
    reset_ਬਫਰ (ਬਫਰ, ਪਾਠ);
    ਜੇ (!hb_shape_ਭਰਿਆ (font, ਬਫਰ,
			features, ਅੰਕ_features,
			shaper_list))
      ਵਾਪਸ ਝੂਠ;
    yb = (ਦੁੱਗਣਾ) ਬਫਰ_advance (ਬਫਰ);
    /* If the maximum expansion is less than max target,
     * there's nothing to solve for. Just return it. */
    ਜੇ (yb <= (ਦੁੱਗਣਾ) max_ਨਿਸ਼ਾਨਾ_advance)
    {
      *var_ਮੁੱਲ = (ਦਸ਼ਮਲਵ) b;
      *advance = (ਦਸ਼ਮਲਵ) yb;
      ਵਾਪਸ ਸੱਚ;
    }
  }
  ਨਹੀਂਤਾਂ
  {
    /* Need to shrink. */
    yb = (ਦੁੱਗਣਾ) *advance;
    a = (ਦੁੱਗਣਾ) axis_info.min_ਮੁੱਲ;
    b = (ਦੁੱਗਣਾ) axis_info.default_ਮੁੱਲ;

    /* Shape buffer for maximum shrinkate to use as other
     * starting point for the solver. */
    hb_font_ਸੈੱਟ_variation (font, ਟੈਗ_2, (ਦਸ਼ਮਲਵ) a);
    reset_ਬਫਰ (ਬਫਰ, ਪਾਠ);
    ਜੇ (!hb_shape_ਭਰਿਆ (font, ਬਫਰ,
			features, ਅੰਕ_features,
			shaper_list))
      ਵਾਪਸ ਝੂਠ;
    ya = (ਦੁੱਗਣਾ) ਬਫਰ_advance (ਬਫਰ);
    /* If the maximum shrinkate is more than min target,
     * there's nothing to solve for. Just return it. */
    ਜੇ (ya >= (ਦੁੱਗਣਾ) min_ਨਿਸ਼ਾਨਾ_advance)
    {
      *var_ਮੁੱਲ = (ਦਸ਼ਮਲਵ) a;
      *advance = (ਦਸ਼ਮਲਵ) ya;
      ਵਾਪਸ ਸੱਚ;
    }
  }

  /* Run the solver to find a var axis value that hits
   * the desired width. */

  ਦੁੱਗਣਾ epsilon = (b - a) / (1<<14);
  ਤਾਰਕਿਕ failed = ਝੂਠ;

  ਸਵੈਚਾਲਿਤ f = [&] (ਦੁੱਗਣਾ x)
  {
    hb_font_ਸੈੱਟ_variation (font, ਟੈਗ_2, (ਦਸ਼ਮਲਵ) x);
    reset_ਬਫਰ (ਬਫਰ, ਪਾਠ);
    ਜੇ (unlikely (!hb_shape_ਭਰਿਆ (font, ਬਫਰ,
				  features, ਅੰਕ_features,
				  shaper_list)))
    {
      failed = ਸੱਚ;
      ਵਾਪਸ (ਦੁੱਗਣਾ) min_ਨਿਸ਼ਾਨਾ_advance;
    }

    ਦੁੱਗਣਾ w = (ਦੁੱਗਣਾ) ਬਫਰ_advance (ਬਫਰ);
    DEBUG_ਸੁਨੇਹਾ (JUSTIFY, ਖਾਲੀ_ਸੂਚਕ, "Trying '%c%c%c%c' axis parameter %f. Advance %g. Target: min %g max %g",
	       HB_UNTAG (ਟੈਗ_2), x, w,
	       (ਦੁੱਗਣਾ) min_ਨਿਸ਼ਾਨਾ_advance, (ਦੁੱਗਣਾ) max_ਨਿਸ਼ਾਨਾ_advance);
    ਵਾਪਸ w;
  };

  ਦੁੱਗਣਾ y = 0;
  ਦੁੱਗਣਾ itp = solve_itp (f,
			  a, b,
			  epsilon,
			  (ਦੁੱਗਣਾ) min_ਨਿਸ਼ਾਨਾ_advance, (ਦੁੱਗਣਾ) max_ਨਿਸ਼ਾਨਾ_advance,
			  ya, yb, y);

  ਜੇ (failed)
    ਵਾਪਸ ਝੂਠ;

  *var_ਮੁੱਲ = (ਦਸ਼ਮਲਵ) itp;
  *advance = (ਦਸ਼ਮਲਵ) y;

  ਵਾਪਸ ਸੱਚ;
}
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ


#ਜੇ_ਅੰਤ
