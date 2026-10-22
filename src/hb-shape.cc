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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-shaper.hh"
#அந்தர்பாவ "hb-shape-plan.hh"
#அந்தர்பாவ "hb-buffer.hh"
#அந்தர்பாவ "hb-font.hh"
#அந்தர்பாவ "hb-machinery.hh"


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


நிலையான உள்வரி வெறுமை free_static_shaper_list ();

நிலையான மாறிலி எழுத்து * மாறிலி nil_shaper_list[] = {nullptr};

நிலையான கட்டமைப்பு hb_shaper_list_lazy_loader_t : hb_lazy_loader_t<மாறிலி எழுத்து *,
							      hb_shaper_list_lazy_loader_t>
{
  நிலையான மாறிலி எழுத்து ** create ()
  {
    மாறிலி எழுத்து **shaper_list = (மாறிலி எழுத்து **) hb_calloc (1 + HB_SHAPERS_மொத்தம், அளவு (மாறிலி எழுத்து *));
    எனில் (unlikely (!shaper_list))
      திரும்பு nullptr;

    மாறிலி hb_shaper_entry_t *shapers = _hb_shapers_get ();
    unsigned முழு i;
    க்கு (i = 0; i < HB_SHAPERS_மொத்தம்; i++)
      shaper_list[i] = shapers[i].name;
    shaper_list[i] = nullptr;

    hb_atexit (free_static_shaper_list);

    திரும்பு shaper_list;
  }
  நிலையான வெறுமை destroy (மாறிலி எழுத்து **l)
  { hb_free (l); }
  நிலையான மாறிலி எழுத்து * மாறிலி * get_null ()
  { திரும்பு nil_shaper_list; }
} static_shaper_list;

நிலையான உள்வரி
வெறுமை free_static_shaper_list ()
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
மாறிலி எழுத்து **
hb_shape_list_shapers ()
{
  திரும்பு static_shaper_list.get_unconst ();
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
	       மாறிலி hb_feature_t *features,
	       unsigned முழு        இலக்கம்_features,
	       மாறிலி எழுத்து * மாறிலி *shaper_list)
{
  எனில் (unlikely (!buffer->len))
    திரும்பு உண்மை;

  buffer->enter ();

  hb_buffer_t *text_buffer = nullptr;
  எனில் (buffer->flags & HB_BUFFER_FLAG_VERIFY)
  {
    text_buffer = hb_buffer_create ();
    hb_buffer_append (text_buffer, buffer, 0, -1);
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached2 (font->face, &buffer->props,
							      features, இலக்கம்_features,
							      font->coords, font->has_nonzero_coords ? font->இலக்கம்_coords : 0,
							      shaper_list);

  hb_bool_t res = hb_shape_plan_execute (shape_plan, font, buffer, features, இலக்கம்_features);

  hb_shape_plan_destroy (shape_plan);

  எனில் (text_buffer)
  {
    எனில் (res && buffer->successful
	    && text_buffer->successful
	    && !buffer->verify (text_buffer,
				font,
				features,
				இலக்கம்_features,
				shaper_list))
      res = பொய்;
    hb_buffer_destroy (text_buffer);
  }

  buffer->leave ();

  திரும்பு res;
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
வெறுமை
hb_shape (hb_font_t           *font,
	  hb_buffer_t         *buffer,
	  மாறிலி hb_feature_t  *features,
	  unsigned முழு         இலக்கம்_features)
{
  hb_shape_full (font, buffer, features, இலக்கம்_features, nullptr);
}


#ifdef HB_EXPERIMENTAL_API
#ifndef HB_NO_VAR

நிலையான தசம
buffer_advance (hb_buffer_t *buffer)
{
  தசம a = 0;
  auto *pos = buffer->pos;
  unsigned மொத்தம் = buffer->len;
  எனில் (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      a += pos[i].x_advance;
  இல்லையெனில்
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      a += pos[i].y_advance;
  திரும்பு a;
}

நிலையான வெறுமை
reset_buffer (hb_buffer_t *buffer,
	      hb_array_t<மாறிலி hb_glyph_info_t> text)
{
  ப்ரதிஜ்ஞா (buffer->ensure (text.length));
  buffer->have_positions = பொய்;
  buffer->len = text.length;
  hb_memcpy (buffer->info, text.arrayZ, text.length * அளவு (buffer->info[0]));
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
		  மாறிலி hb_feature_t *features,
		  unsigned முழு        இலக்கம்_features,
		  மாறிலி எழுத்து * மாறிலி *shaper_list,
		  தசம               min_target_advance,
		  தசம               max_target_advance,
		  தசம              *advance, /* IN/OUT */
		  hb_tag_t           *var_tag, /* OUT */
		  தசம              *var_மதிப்பு /* OUT */)
{
  // TODO Negative font scales?

  /* If default advance already matches target, nothing to do. Shape and return. */
  எனில் (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_மதிப்பு = 0.0f;
    திரும்பு hb_shape_full (font, buffer,
			  features, இலக்கம்_features,
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
  க்கு (unsigned i = 0; i < ARRAY_LENGTH (tags); i++)
    எனில் (hb_ot_var_find_axis_info (face, tags[i], &axis_info))
    {
      tag = *var_tag = tags[i];
      நிறுத்து;
    }

  /* If no suitable variation axis found, can't justify.  Just shape and return. */
  எனில் (!tag)
  {
    *var_tag = HB_TAG_NONE;
    *var_மதிப்பு = 0.0f;
    எனில் (hb_shape_full (font, buffer,
		       features, இலக்கம்_features,
		       shaper_list))
    {
      *advance = buffer_advance (buffer);
      திரும்பு உண்மை;
    }
    இல்லையெனில்
      திரும்பு பொய்;
  }

  /* Copy buffer text as we need it so we can shape multiple times. */
  unsigned text_len = buffer->len;
  auto *text_info = (hb_glyph_info_t *) hb_malloc2 (text_len, அளவு (buffer->info[0]));
  எனில் (unlikely (text_len && !text_info))
    திரும்பு பொய்;
  HB_SCOPE_GUARD (hb_free (text_info));
  hb_memcpy (text_info, buffer->info, text_len * அளவு (buffer->info[0]));
  auto text = hb_array<மாறிலி hb_glyph_info_t> (text_info, text_len);

  /* If default advance was not provided to us, calculate it. */
  எனில் (!*advance)
  {
    hb_font_set_variation (font, tag, axis_info.default_மதிப்பு);
    எனில் (!hb_shape_full (font, buffer,
			features, இலக்கம்_features,
			shaper_list))
      திரும்பு பொய்;
    *advance = buffer_advance (buffer);
  }

  /* If default advance already matches target, nothing to do. Shape and return.
   * Do this again, in case advance was just calculated.
   */
  எனில் (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_மதிப்பு = 0.0f;
    திரும்பு உண்மை;
  }

  /* Prepare for running the solver. */
  இரட்டை a, b, ya, yb;
  எனில் (*advance < min_target_advance)
  {
    /* Need to expand. */
    ya = (இரட்டை) *advance;
    a = (இரட்டை) axis_info.default_மதிப்பு;
    b = (இரட்டை) axis_info.max_மதிப்பு;

    /* Shape buffer for maximum expansion to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (தசம) b);
    reset_buffer (buffer, text);
    எனில் (!hb_shape_full (font, buffer,
			features, இலக்கம்_features,
			shaper_list))
      திரும்பு பொய்;
    yb = (இரட்டை) buffer_advance (buffer);
    /* If the maximum expansion is less than max target,
     * there's nothing to solve for. Just return it. */
    எனில் (yb <= (இரட்டை) max_target_advance)
    {
      *var_மதிப்பு = (தசம) b;
      *advance = (தசம) yb;
      திரும்பு உண்மை;
    }
  }
  இல்லையெனில்
  {
    /* Need to shrink. */
    yb = (இரட்டை) *advance;
    a = (இரட்டை) axis_info.min_மதிப்பு;
    b = (இரட்டை) axis_info.default_மதிப்பு;

    /* Shape buffer for maximum shrinkate to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (தசம) a);
    reset_buffer (buffer, text);
    எனில் (!hb_shape_full (font, buffer,
			features, இலக்கம்_features,
			shaper_list))
      திரும்பு பொய்;
    ya = (இரட்டை) buffer_advance (buffer);
    /* If the maximum shrinkate is more than min target,
     * there's nothing to solve for. Just return it. */
    எனில் (ya >= (இரட்டை) min_target_advance)
    {
      *var_மதிப்பு = (தசம) a;
      *advance = (தசம) ya;
      திரும்பு உண்மை;
    }
  }

  /* Run the solver to find a var axis value that hits
   * the desired width. */

  இரட்டை epsilon = (b - a) / (1<<14);
  தருக்க failed = பொய்;

  auto f = [&] (இரட்டை x)
  {
    hb_font_set_variation (font, tag, (தசம) x);
    reset_buffer (buffer, text);
    எனில் (unlikely (!hb_shape_full (font, buffer,
				  features, இலக்கம்_features,
				  shaper_list)))
    {
      failed = உண்மை;
      திரும்பு (இரட்டை) min_target_advance;
    }

    இரட்டை w = (இரட்டை) buffer_advance (buffer);
    DEBUG_செய்தி (JUSTIFY, nullptr, "Trying '%c%c%c%c' axis parameter %f. Advance %g. Target: min %g max %g",
	       HB_UNTAG (tag), x, w,
	       (இரட்டை) min_target_advance, (இரட்டை) max_target_advance);
    திரும்பு w;
  };

  இரட்டை y = 0;
  இரட்டை itp = solve_itp (f,
			  a, b,
			  epsilon,
			  (இரட்டை) min_target_advance, (இரட்டை) max_target_advance,
			  ya, yb, y);

  எனில் (failed)
    திரும்பு பொய்;

  *var_மதிப்பு = (தசம) itp;
  *advance = (தசம) y;

  திரும்பு உண்மை;
}
#endif
#endif


#endif
