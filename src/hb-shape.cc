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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-shaper.hh"
#ಅನ್ತರ್ಭಾವ "hb-shape-plan.hh"
#ಅನ್ತರ್ಭಾವ "hb-buffer.hh"
#ಅನ್ತರ್ಭಾವ "hb-font.hh"
#ಅನ್ತರ್ಭಾವ "hb-machinery.hh"


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


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ free_static_shaper_list ();

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ nil_shaper_list[] = {nullptr};

ಸ್ಥಿರ ಸಂರಚನಾ hb_shaper_list_lazy_loader_t : hb_lazy_loader_t<ಧ್ರುವ ವರ್ಣ *,
							      hb_shaper_list_lazy_loader_t>
{
  ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ ** create ()
  {
    ಧ್ರುವ ವರ್ಣ **shaper_list = (ಧ್ರುವ ವರ್ಣ **) hb_calloc (1 + HB_SHAPERS_ಎಣಿಕೆ, ಪರಿಮಾಣ (ಧ್ರುವ ವರ್ಣ *));
    ಯದಿ (unlikely (!shaper_list))
      ಪ್ರತ್ಯಾಗಮ nullptr;

    ಧ್ರುವ hb_shaper_entry_t *shapers = _hb_shapers_get ();
    unsigned ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ (i = 0; i < HB_SHAPERS_ಎಣಿಕೆ; i++)
      shaper_list[i] = shapers[i].name;
    shaper_list[i] = nullptr;

    hb_atexit (free_static_shaper_list);

    ಪ್ರತ್ಯಾಗಮ shaper_list;
  }
  ಸ್ಥಿರ ಶೂನ್ಯ destroy (ಧ್ರುವ ವರ್ಣ **l)
  { hb_free (l); }
  ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ * get_null ()
  { ಪ್ರತ್ಯಾಗಮ nil_shaper_list; }
} static_shaper_list;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ free_static_shaper_list ()
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
ಧ್ರುವ ವರ್ಣ **
hb_shape_list_shapers ()
{
  ಪ್ರತ್ಯಾಗಮ static_shaper_list.get_unconst ();
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
	       ಧ್ರುವ hb_feature_t *features,
	       unsigned ಪೂರ್ಣಾಙ್ಕ        ಅಂಕೆ_features,
	       ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ *shaper_list)
{
  ಯದಿ (unlikely (!buffer->len))
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

  buffer->enter ();

  hb_buffer_t *text_buffer = nullptr;
  ಯದಿ (buffer->flags & HB_BUFFER_FLAG_VERIFY)
  {
    text_buffer = hb_buffer_create ();
    hb_buffer_append (text_buffer, buffer, 0, -1);
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached2 (font->face, &buffer->props,
							      features, ಅಂಕೆ_features,
							      font->coords, font->has_nonzero_coords ? font->ಅಂಕೆ_coords : 0,
							      shaper_list);

  hb_bool_t res = hb_shape_plan_execute (shape_plan, font, buffer, features, ಅಂಕೆ_features);

  hb_shape_plan_destroy (shape_plan);

  ಯದಿ (text_buffer)
  {
    ಯದಿ (res && buffer->successful
	    && text_buffer->successful
	    && !buffer->verify (text_buffer,
				font,
				features,
				ಅಂಕೆ_features,
				shaper_list))
      res = ಅಸತ್ಯ;
    hb_buffer_destroy (text_buffer);
  }

  buffer->leave ();

  ಪ್ರತ್ಯಾಗಮ res;
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
ಶೂನ್ಯ
hb_shape (hb_font_t           *font,
	  hb_buffer_t         *buffer,
	  ಧ್ರುವ hb_feature_t  *features,
	  unsigned ಪೂರ್ಣಾಙ್ಕ         ಅಂಕೆ_features)
{
  hb_shape_full (font, buffer, features, ಅಂಕೆ_features, nullptr);
}


#ifdef HB_EXPERIMENTAL_API
#ifndef HB_NO_VAR

ಸ್ಥಿರ ಭಿನ್ನ
buffer_advance (hb_buffer_t *buffer)
{
  ಭಿನ್ನ a = 0;
  auto *pos = buffer->pos;
  unsigned ಎಣಿಕೆ = buffer->len;
  ಯದಿ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      a += pos[i].x_advance;
  ಅನ್ಯಥಾ
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      a += pos[i].y_advance;
  ಪ್ರತ್ಯಾಗಮ a;
}

ಸ್ಥಿರ ಶೂನ್ಯ
reset_buffer (hb_buffer_t *buffer,
	      hb_array_t<ಧ್ರುವ hb_glyph_info_t> text)
{
  ಪ್ರತಿಜ್ಞಾ (buffer->ensure (text.length));
  buffer->have_positions = ಅಸತ್ಯ;
  buffer->len = text.length;
  hb_memcpy (buffer->info, text.arrayZ, text.length * ಪರಿಮಾಣ (buffer->info[0]));
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
		  ಧ್ರುವ hb_feature_t *features,
		  unsigned ಪೂರ್ಣಾಙ್ಕ        ಅಂಕೆ_features,
		  ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ *shaper_list,
		  ಭಿನ್ನ               min_target_advance,
		  ಭಿನ್ನ               max_target_advance,
		  ಭಿನ್ನ              *advance, /* IN/OUT */
		  hb_tag_t           *var_tag, /* OUT */
		  ಭಿನ್ನ              *var_ಮೌಲ್ಯ /* OUT */)
{
  // TODO Negative font scales?

  /* If default advance already matches target, nothing to do. Shape and return. */
  ಯದಿ (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_ಮೌಲ್ಯ = 0.0f;
    ಪ್ರತ್ಯಾಗಮ hb_shape_full (font, buffer,
			  features, ಅಂಕೆ_features,
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
  ಪ್ರತಿ (unsigned i = 0; i < ARRAY_LENGTH (tags); i++)
    ಯದಿ (hb_ot_var_find_axis_info (face, tags[i], &axis_info))
    {
      tag = *var_tag = tags[i];
      ಭಙ್ಗ;
    }

  /* If no suitable variation axis found, can't justify.  Just shape and return. */
  ಯದಿ (!tag)
  {
    *var_tag = HB_TAG_NONE;
    *var_ಮೌಲ್ಯ = 0.0f;
    ಯದಿ (hb_shape_full (font, buffer,
		       features, ಅಂಕೆ_features,
		       shaper_list))
    {
      *advance = buffer_advance (buffer);
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  /* Copy buffer text as we need it so we can shape multiple times. */
  unsigned text_len = buffer->len;
  auto *text_info = (hb_glyph_info_t *) hb_malloc2 (text_len, ಪರಿಮಾಣ (buffer->info[0]));
  ಯದಿ (unlikely (text_len && !text_info))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  HB_SCOPE_GUARD (hb_free (text_info));
  hb_memcpy (text_info, buffer->info, text_len * ಪರಿಮಾಣ (buffer->info[0]));
  auto text = hb_array<ಧ್ರುವ hb_glyph_info_t> (text_info, text_len);

  /* If default advance was not provided to us, calculate it. */
  ಯದಿ (!*advance)
  {
    hb_font_set_variation (font, tag, axis_info.default_ಮೌಲ್ಯ);
    ಯದಿ (!hb_shape_full (font, buffer,
			features, ಅಂಕೆ_features,
			shaper_list))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    *advance = buffer_advance (buffer);
  }

  /* If default advance already matches target, nothing to do. Shape and return.
   * Do this again, in case advance was just calculated.
   */
  ಯದಿ (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_ಮೌಲ್ಯ = 0.0f;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  /* Prepare for running the solver. */
  ದ್ವಿಗುಣ a, b, ya, yb;
  ಯದಿ (*advance < min_target_advance)
  {
    /* Need to expand. */
    ya = (ದ್ವಿಗುಣ) *advance;
    a = (ದ್ವಿಗುಣ) axis_info.default_ಮೌಲ್ಯ;
    b = (ದ್ವಿಗುಣ) axis_info.max_ಮೌಲ್ಯ;

    /* Shape buffer for maximum expansion to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (ಭಿನ್ನ) b);
    reset_buffer (buffer, text);
    ಯದಿ (!hb_shape_full (font, buffer,
			features, ಅಂಕೆ_features,
			shaper_list))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    yb = (ದ್ವಿಗುಣ) buffer_advance (buffer);
    /* If the maximum expansion is less than max target,
     * there's nothing to solve for. Just return it. */
    ಯದಿ (yb <= (ದ್ವಿಗುಣ) max_target_advance)
    {
      *var_ಮೌಲ್ಯ = (ಭಿನ್ನ) b;
      *advance = (ಭಿನ್ನ) yb;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
  }
  ಅನ್ಯಥಾ
  {
    /* Need to shrink. */
    yb = (ದ್ವಿಗುಣ) *advance;
    a = (ದ್ವಿಗುಣ) axis_info.min_ಮೌಲ್ಯ;
    b = (ದ್ವಿಗುಣ) axis_info.default_ಮೌಲ್ಯ;

    /* Shape buffer for maximum shrinkate to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (ಭಿನ್ನ) a);
    reset_buffer (buffer, text);
    ಯದಿ (!hb_shape_full (font, buffer,
			features, ಅಂಕೆ_features,
			shaper_list))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ya = (ದ್ವಿಗುಣ) buffer_advance (buffer);
    /* If the maximum shrinkate is more than min target,
     * there's nothing to solve for. Just return it. */
    ಯದಿ (ya >= (ದ್ವಿಗುಣ) min_target_advance)
    {
      *var_ಮೌಲ್ಯ = (ಭಿನ್ನ) a;
      *advance = (ಭಿನ್ನ) ya;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
  }

  /* Run the solver to find a var axis value that hits
   * the desired width. */

  ದ್ವಿಗುಣ epsilon = (b - a) / (1<<14);
  ತಾರ್ಕಿಕ failed = ಅಸತ್ಯ;

  auto f = [&] (ದ್ವಿಗುಣ x)
  {
    hb_font_set_variation (font, tag, (ಭಿನ್ನ) x);
    reset_buffer (buffer, text);
    ಯದಿ (unlikely (!hb_shape_full (font, buffer,
				  features, ಅಂಕೆ_features,
				  shaper_list)))
    {
      failed = ಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ (ದ್ವಿಗುಣ) min_target_advance;
    }

    ದ್ವಿಗುಣ w = (ದ್ವಿಗುಣ) buffer_advance (buffer);
    DEBUG_ಸಂದೇಶ (JUSTIFY, nullptr, "Trying '%c%c%c%c' axis parameter %f. Advance %g. Target: min %g max %g",
	       HB_UNTAG (tag), x, w,
	       (ದ್ವಿಗುಣ) min_target_advance, (ದ್ವಿಗುಣ) max_target_advance);
    ಪ್ರತ್ಯಾಗಮ w;
  };

  ದ್ವಿಗುಣ y = 0;
  ದ್ವಿಗುಣ itp = solve_itp (f,
			  a, b,
			  epsilon,
			  (ದ್ವಿಗುಣ) min_target_advance, (ದ್ವಿಗುಣ) max_target_advance,
			  ya, yb, y);

  ಯದಿ (failed)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  *var_ಮೌಲ್ಯ = (ಭಿನ್ನ) itp;
  *advance = (ಭಿನ್ನ) y;

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}
#endif
#endif


#endif
