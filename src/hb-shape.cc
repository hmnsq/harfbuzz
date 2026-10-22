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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-shaper.hh"
#അന്തര്ഭാവ "hb-shape-plan.hh"
#അന്തര്ഭാവ "hb-buffer.hh"
#അന്തര്ഭാവ "hb-font.hh"
#അന്തര്ഭാവ "hb-machinery.hh"


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


സ്ഥിതം വരിയിൽ ശൂന്യം free_static_shaper_list ();

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം nil_shaper_list[] = {nullptr};

സ്ഥിതം ഘടന hb_shaper_list_lazy_loader_t : hb_lazy_loader_t<സ്ഥിരം അക്ഷരം *,
							      hb_shaper_list_lazy_loader_t>
{
  സ്ഥിതം സ്ഥിരം അക്ഷരം ** create ()
  {
    സ്ഥിരം അക്ഷരം **shaper_list = (സ്ഥിരം അക്ഷരം **) hb_calloc (1 + HB_SHAPERS_എണ്ണം, വലുപ്പം (സ്ഥിരം അക്ഷരം *));
    എങ്കിൽ (unlikely (!shaper_list))
      തിരികെ nullptr;

    സ്ഥിരം hb_shaper_entry_t *shapers = _hb_shapers_get ();
    unsigned പൂർണ്ണസംഖ്യ i;
    വേണ്ടി (i = 0; i < HB_SHAPERS_എണ്ണം; i++)
      shaper_list[i] = shapers[i].name;
    shaper_list[i] = nullptr;

    hb_atexit (free_static_shaper_list);

    തിരികെ shaper_list;
  }
  സ്ഥിതം ശൂന്യം destroy (സ്ഥിരം അക്ഷരം **l)
  { hb_free (l); }
  സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം * get_null ()
  { തിരികെ nil_shaper_list; }
} static_shaper_list;

സ്ഥിതം വരിയിൽ
ശൂന്യം free_static_shaper_list ()
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
സ്ഥിരം അക്ഷരം **
hb_shape_list_shapers ()
{
  തിരികെ static_shaper_list.get_unconst ();
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
	       സ്ഥിരം hb_feature_t *features,
	       unsigned പൂർണ്ണസംഖ്യ        അക്കം_features,
	       സ്ഥിരം അക്ഷരം * സ്ഥിരം *shaper_list)
{
  എങ്കിൽ (unlikely (!buffer->len))
    തിരികെ സത്യം;

  buffer->enter ();

  hb_buffer_t *text_buffer = nullptr;
  എങ്കിൽ (buffer->flags & HB_BUFFER_FLAG_VERIFY)
  {
    text_buffer = hb_buffer_create ();
    hb_buffer_append (text_buffer, buffer, 0, -1);
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached2 (font->face, &buffer->props,
							      features, അക്കം_features,
							      font->coords, font->has_nonzero_coords ? font->അക്കം_coords : 0,
							      shaper_list);

  hb_bool_t res = hb_shape_plan_execute (shape_plan, font, buffer, features, അക്കം_features);

  hb_shape_plan_destroy (shape_plan);

  എങ്കിൽ (text_buffer)
  {
    എങ്കിൽ (res && buffer->successful
	    && text_buffer->successful
	    && !buffer->verify (text_buffer,
				font,
				features,
				അക്കം_features,
				shaper_list))
      res = അസത്യം;
    hb_buffer_destroy (text_buffer);
  }

  buffer->leave ();

  തിരികെ res;
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
ശൂന്യം
hb_shape (hb_font_t           *font,
	  hb_buffer_t         *buffer,
	  സ്ഥിരം hb_feature_t  *features,
	  unsigned പൂർണ്ണസംഖ്യ         അക്കം_features)
{
  hb_shape_full (font, buffer, features, അക്കം_features, nullptr);
}


#ifdef HB_EXPERIMENTAL_API
#ifndef HB_NO_VAR

സ്ഥിതം ദശാംശം
buffer_advance (hb_buffer_t *buffer)
{
  ദശാംശം a = 0;
  auto *pos = buffer->pos;
  unsigned എണ്ണം = buffer->len;
  എങ്കിൽ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      a += pos[i].x_advance;
  അല്ലെങ്കിൽ
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      a += pos[i].y_advance;
  തിരികെ a;
}

സ്ഥിതം ശൂന്യം
reset_buffer (hb_buffer_t *buffer,
	      hb_array_t<സ്ഥിരം hb_glyph_info_t> text)
{
  പ്രതിജ്ഞാ (buffer->ensure (text.length));
  buffer->have_positions = അസത്യം;
  buffer->len = text.length;
  hb_memcpy (buffer->info, text.arrayZ, text.length * വലുപ്പം (buffer->info[0]));
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
		  സ്ഥിരം hb_feature_t *features,
		  unsigned പൂർണ്ണസംഖ്യ        അക്കം_features,
		  സ്ഥിരം അക്ഷരം * സ്ഥിരം *shaper_list,
		  ദശാംശം               min_target_advance,
		  ദശാംശം               max_target_advance,
		  ദശാംശം              *advance, /* IN/OUT */
		  hb_tag_t           *var_tag, /* OUT */
		  ദശാംശം              *var_മൂല്യം /* OUT */)
{
  // TODO Negative font scales?

  /* If default advance already matches target, nothing to do. Shape and return. */
  എങ്കിൽ (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_മൂല്യം = 0.0f;
    തിരികെ hb_shape_full (font, buffer,
			  features, അക്കം_features,
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
  വേണ്ടി (unsigned i = 0; i < ARRAY_LENGTH (tags); i++)
    എങ്കിൽ (hb_ot_var_find_axis_info (face, tags[i], &axis_info))
    {
      tag = *var_tag = tags[i];
      നിർത്തുക;
    }

  /* If no suitable variation axis found, can't justify.  Just shape and return. */
  എങ്കിൽ (!tag)
  {
    *var_tag = HB_TAG_NONE;
    *var_മൂല്യം = 0.0f;
    എങ്കിൽ (hb_shape_full (font, buffer,
		       features, അക്കം_features,
		       shaper_list))
    {
      *advance = buffer_advance (buffer);
      തിരികെ സത്യം;
    }
    അല്ലെങ്കിൽ
      തിരികെ അസത്യം;
  }

  /* Copy buffer text as we need it so we can shape multiple times. */
  unsigned text_len = buffer->len;
  auto *text_info = (hb_glyph_info_t *) hb_malloc2 (text_len, വലുപ്പം (buffer->info[0]));
  എങ്കിൽ (unlikely (text_len && !text_info))
    തിരികെ അസത്യം;
  HB_SCOPE_GUARD (hb_free (text_info));
  hb_memcpy (text_info, buffer->info, text_len * വലുപ്പം (buffer->info[0]));
  auto text = hb_array<സ്ഥിരം hb_glyph_info_t> (text_info, text_len);

  /* If default advance was not provided to us, calculate it. */
  എങ്കിൽ (!*advance)
  {
    hb_font_set_variation (font, tag, axis_info.default_മൂല്യം);
    എങ്കിൽ (!hb_shape_full (font, buffer,
			features, അക്കം_features,
			shaper_list))
      തിരികെ അസത്യം;
    *advance = buffer_advance (buffer);
  }

  /* If default advance already matches target, nothing to do. Shape and return.
   * Do this again, in case advance was just calculated.
   */
  എങ്കിൽ (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_മൂല്യം = 0.0f;
    തിരികെ സത്യം;
  }

  /* Prepare for running the solver. */
  ഇരട്ട a, b, ya, yb;
  എങ്കിൽ (*advance < min_target_advance)
  {
    /* Need to expand. */
    ya = (ഇരട്ട) *advance;
    a = (ഇരട്ട) axis_info.default_മൂല്യം;
    b = (ഇരട്ട) axis_info.max_മൂല്യം;

    /* Shape buffer for maximum expansion to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (ദശാംശം) b);
    reset_buffer (buffer, text);
    എങ്കിൽ (!hb_shape_full (font, buffer,
			features, അക്കം_features,
			shaper_list))
      തിരികെ അസത്യം;
    yb = (ഇരട്ട) buffer_advance (buffer);
    /* If the maximum expansion is less than max target,
     * there's nothing to solve for. Just return it. */
    എങ്കിൽ (yb <= (ഇരട്ട) max_target_advance)
    {
      *var_മൂല്യം = (ദശാംശം) b;
      *advance = (ദശാംശം) yb;
      തിരികെ സത്യം;
    }
  }
  അല്ലെങ്കിൽ
  {
    /* Need to shrink. */
    yb = (ഇരട്ട) *advance;
    a = (ഇരട്ട) axis_info.min_മൂല്യം;
    b = (ഇരട്ട) axis_info.default_മൂല്യം;

    /* Shape buffer for maximum shrinkate to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (ദശാംശം) a);
    reset_buffer (buffer, text);
    എങ്കിൽ (!hb_shape_full (font, buffer,
			features, അക്കം_features,
			shaper_list))
      തിരികെ അസത്യം;
    ya = (ഇരട്ട) buffer_advance (buffer);
    /* If the maximum shrinkate is more than min target,
     * there's nothing to solve for. Just return it. */
    എങ്കിൽ (ya >= (ഇരട്ട) min_target_advance)
    {
      *var_മൂല്യം = (ദശാംശം) a;
      *advance = (ദശാംശം) ya;
      തിരികെ സത്യം;
    }
  }

  /* Run the solver to find a var axis value that hits
   * the desired width. */

  ഇരട്ട epsilon = (b - a) / (1<<14);
  തർക്കം failed = അസത്യം;

  auto f = [&] (ഇരട്ട x)
  {
    hb_font_set_variation (font, tag, (ദശാംശം) x);
    reset_buffer (buffer, text);
    എങ്കിൽ (unlikely (!hb_shape_full (font, buffer,
				  features, അക്കം_features,
				  shaper_list)))
    {
      failed = സത്യം;
      തിരികെ (ഇരട്ട) min_target_advance;
    }

    ഇരട്ട w = (ഇരട്ട) buffer_advance (buffer);
    DEBUG_സന്ദേശം (JUSTIFY, nullptr, "Trying '%c%c%c%c' axis parameter %f. Advance %g. Target: min %g max %g",
	       HB_UNTAG (tag), x, w,
	       (ഇരട്ട) min_target_advance, (ഇരട്ട) max_target_advance);
    തിരികെ w;
  };

  ഇരട്ട y = 0;
  ഇരട്ട itp = solve_itp (f,
			  a, b,
			  epsilon,
			  (ഇരട്ട) min_target_advance, (ഇരട്ട) max_target_advance,
			  ya, yb, y);

  എങ്കിൽ (failed)
    തിരികെ അസത്യം;

  *var_മൂല്യം = (ദശാംശം) itp;
  *advance = (ദശാംശം) y;

  തിരികെ സത്യം;
}
#endif
#endif


#endif
