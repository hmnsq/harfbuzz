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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-shaper.hh"
#अन्तर्भाव "hb-shape-plan.hh"
#अन्तर्भाव "hb-buffer.hh"
#अन्तर्भाव "hb-font.hh"
#अन्तर्भाव "hb-machinery.hh"


#ifndef HB_NO_SHAPER

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


स्थिर अन्तःस्थ शून्य free_static_shaper_list ();

स्थिर ध्रुव वर्ण * ध्रुव nil_shaper_list[] = {nullptr};

स्थिर संरचना hb_shaper_list_lazy_loader_t : hb_lazy_loader_t<ध्रुव वर्ण *,
							      hb_shaper_list_lazy_loader_t>
{
  स्थिर ध्रुव वर्ण ** create ()
  {
    ध्रुव वर्ण **shaper_list = (ध्रुव वर्ण **) hb_calloc (1 + HB_SHAPERS_गिनती, परिमाण (ध्रुव वर्ण *));
    यदि (unlikely (!shaper_list))
      प्रत्यागम nullptr;

    ध्रुव hb_shaper_entry_t *shapers = _hb_shapers_get ();
    अचिन्हित पूर्णाङ्क i;
    प्रति (i = 0; i < HB_SHAPERS_गिनती; i++)
      shaper_list[i] = shapers[i].name;
    shaper_list[i] = nullptr;

    hb_atexit (free_static_shaper_list);

    प्रत्यागम shaper_list;
  }
  स्थिर शून्य destroy (ध्रुव वर्ण **l)
  { hb_free (l); }
  स्थिर ध्रुव वर्ण * ध्रुव * get_null ()
  { प्रत्यागम nil_shaper_list; }
} static_shaper_list;

स्थिर अन्तःस्थ
शून्य free_static_shaper_list ()
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
ध्रुव वर्ण **
hb_shape_list_shapers ()
{
  प्रत्यागम static_shaper_list.get_unconst ();
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
	       hb_buffer_t        *buffer,
	       ध्रुव hb_feature_t *features,
	       अचिन्हित पूर्णाङ्क        अंक_features,
	       ध्रुव वर्ण * ध्रुव *shaper_list)
{
  यदि (unlikely (!buffer->len))
    प्रत्यागम सत्य;

  buffer->enter ();

  hb_buffer_t *text_buffer = nullptr;
  यदि (buffer->flags & HB_BUFFER_FLAG_VERIFY)
  {
    text_buffer = hb_buffer_create ();
    hb_buffer_append (text_buffer, buffer, 0, -1);
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached2 (font->face, &buffer->props,
							      features, अंक_features,
							      font->coords, font->has_nonzero_coords ? font->अंक_coords : 0,
							      shaper_list);

  hb_bool_t res = hb_shape_plan_execute (shape_plan, font, buffer, features, अंक_features);

  hb_shape_plan_destroy (shape_plan);

  यदि (text_buffer)
  {
    यदि (res && buffer->successful
	    && text_buffer->successful
	    && !buffer->verify (text_buffer,
				font,
				features,
				अंक_features,
				shaper_list))
      res = असत्य;
    hb_buffer_destroy (text_buffer);
  }

  buffer->leave ();

  प्रत्यागम res;
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
शून्य
hb_shape (hb_font_t           *font,
	  hb_buffer_t         *buffer,
	  ध्रुव hb_feature_t  *features,
	  अचिन्हित पूर्णाङ्क         अंक_features)
{
  hb_shape_full (font, buffer, features, अंक_features, nullptr);
}


#ifdef HB_EXPERIMENTAL_API
#ifndef HB_NO_VAR

स्थिर भिन्न
buffer_advance (hb_buffer_t *buffer)
{
  भिन्न a = 0;
  स्वतः *pos = buffer->pos;
  अचिन्हित गिनती = buffer->len;
  यदि (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    प्रति (अचिन्हित i = 0; i < गिनती; i++)
      a += pos[i].x_advance;
  अन्यथा
    प्रति (अचिन्हित i = 0; i < गिनती; i++)
      a += pos[i].y_advance;
  प्रत्यागम a;
}

स्थिर शून्य
reset_buffer (hb_buffer_t *buffer,
	      hb_array_t<ध्रुव hb_glyph_info_t> text)
{
  प्रतिज्ञा (buffer->ensure (text.length));
  buffer->have_positions = असत्य;
  buffer->len = text.length;
  hb_memcpy (buffer->info, text.arrayZ, text.length * परिमाण (buffer->info[0]));
  hb_buffer_set_content_type (buffer, HB_BUFFER_CONTENT_TYPE_UNICODE);
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
		  hb_buffer_t        *buffer,
		  ध्रुव hb_feature_t *features,
		  अचिन्हित पूर्णाङ्क        अंक_features,
		  ध्रुव वर्ण * ध्रुव *shaper_list,
		  भिन्न               min_target_advance,
		  भिन्न               max_target_advance,
		  भिन्न              *advance, /* IN/OUT */
		  hb_tag_t           *var_tag, /* OUT */
		  भिन्न              *var_मूल्य /* OUT */)
{
  // TODO Negative font scales?

  /* If default advance already matches target, nothing to do. Shape and return. */
  यदि (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_मूल्य = 0.0f;
    प्रत्यागम hb_shape_full (font, buffer,
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
  प्रति (अचिन्हित i = 0; i < ARRAY_LENGTH (tags); i++)
    यदि (hb_ot_var_find_axis_info (face, tags[i], &axis_info))
    {
      tag = *var_tag = tags[i];
      भङ्ग;
    }

  /* If no suitable variation axis found, can't justify.  Just shape and return. */
  यदि (!tag)
  {
    *var_tag = HB_TAG_NONE;
    *var_मूल्य = 0.0f;
    यदि (hb_shape_full (font, buffer,
		       features, अंक_features,
		       shaper_list))
    {
      *advance = buffer_advance (buffer);
      प्रत्यागम सत्य;
    }
    अन्यथा
      प्रत्यागम असत्य;
  }

  /* Copy buffer text as we need it so we can shape multiple times. */
  अचिन्हित text_len = buffer->len;
  स्वतः *text_info = (hb_glyph_info_t *) hb_malloc2 (text_len, परिमाण (buffer->info[0]));
  यदि (unlikely (text_len && !text_info))
    प्रत्यागम असत्य;
  HB_SCOPE_GUARD (hb_free (text_info));
  hb_memcpy (text_info, buffer->info, text_len * परिमाण (buffer->info[0]));
  स्वतः text = hb_array<ध्रुव hb_glyph_info_t> (text_info, text_len);

  /* If default advance was not provided to us, calculate it. */
  यदि (!*advance)
  {
    hb_font_set_variation (font, tag, axis_info.default_मूल्य);
    यदि (!hb_shape_full (font, buffer,
			features, अंक_features,
			shaper_list))
      प्रत्यागम असत्य;
    *advance = buffer_advance (buffer);
  }

  /* If default advance already matches target, nothing to do. Shape and return.
   * Do this again, in case advance was just calculated.
   */
  यदि (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_मूल्य = 0.0f;
    प्रत्यागम सत्य;
  }

  /* Prepare for running the solver. */
  द्विगुण a, b, ya, yb;
  यदि (*advance < min_target_advance)
  {
    /* Need to expand. */
    ya = (द्विगुण) *advance;
    a = (द्विगुण) axis_info.default_मूल्य;
    b = (द्विगुण) axis_info.max_मूल्य;

    /* Shape buffer for maximum expansion to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (भिन्न) b);
    reset_buffer (buffer, text);
    यदि (!hb_shape_full (font, buffer,
			features, अंक_features,
			shaper_list))
      प्रत्यागम असत्य;
    yb = (द्विगुण) buffer_advance (buffer);
    /* If the maximum expansion is less than max target,
     * there's nothing to solve for. Just return it. */
    यदि (yb <= (द्विगुण) max_target_advance)
    {
      *var_मूल्य = (भिन्न) b;
      *advance = (भिन्न) yb;
      प्रत्यागम सत्य;
    }
  }
  अन्यथा
  {
    /* Need to shrink. */
    yb = (द्विगुण) *advance;
    a = (द्विगुण) axis_info.min_मूल्य;
    b = (द्विगुण) axis_info.default_मूल्य;

    /* Shape buffer for maximum shrinkate to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (भिन्न) a);
    reset_buffer (buffer, text);
    यदि (!hb_shape_full (font, buffer,
			features, अंक_features,
			shaper_list))
      प्रत्यागम असत्य;
    ya = (द्विगुण) buffer_advance (buffer);
    /* If the maximum shrinkate is more than min target,
     * there's nothing to solve for. Just return it. */
    यदि (ya >= (द्विगुण) min_target_advance)
    {
      *var_मूल्य = (भिन्न) a;
      *advance = (भिन्न) ya;
      प्रत्यागम सत्य;
    }
  }

  /* Run the solver to find a var axis value that hits
   * the desired width. */

  द्विगुण epsilon = (b - a) / (1<<14);
  तार्किक failed = असत्य;

  स्वतः f = [&] (द्विगुण x)
  {
    hb_font_set_variation (font, tag, (भिन्न) x);
    reset_buffer (buffer, text);
    यदि (unlikely (!hb_shape_full (font, buffer,
				  features, अंक_features,
				  shaper_list)))
    {
      failed = सत्य;
      प्रत्यागम (द्विगुण) min_target_advance;
    }

    द्विगुण w = (द्विगुण) buffer_advance (buffer);
    DEBUG_संदेश (JUSTIFY, nullptr, "Trying '%c%c%c%c' axis parameter %f. Advance %g. Target: min %g max %g",
	       HB_UNTAG (tag), x, w,
	       (द्विगुण) min_target_advance, (द्विगुण) max_target_advance);
    प्रत्यागम w;
  };

  द्विगुण y = 0;
  द्विगुण itp = solve_itp (f,
			  a, b,
			  epsilon,
			  (द्विगुण) min_target_advance, (द्विगुण) max_target_advance,
			  ya, yb, y);

  यदि (failed)
    प्रत्यागम असत्य;

  *var_मूल्य = (भिन्न) itp;
  *advance = (भिन्न) y;

  प्रत्यागम सत्य;
}
#endif
#endif


#endif
