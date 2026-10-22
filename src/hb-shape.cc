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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-shaper.hh"
#ଅନ୍ତର୍ଭାଵ "hb-shape-plan.hh"
#ଅନ୍ତର୍ଭାଵ "hb-buffer.hh"
#ଅନ୍ତର୍ଭାଵ "hb-font.hh"
#ଅନ୍ତର୍ଭାଵ "hb-machinery.hh"


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


ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ free_static_shaper_list ();

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ nil_shaper_list[] = {nullptr};

ସ୍ଥିର ସଂରଚନା hb_shaper_list_lazy_loader_t : hb_lazy_loader_t<ଅଚଳ ଅକ୍ଷର *,
							      hb_shaper_list_lazy_loader_t>
{
  ସ୍ଥିର ଅଚଳ ଅକ୍ଷର ** create ()
  {
    ଅଚଳ ଅକ୍ଷର **shaper_list = (ଅଚଳ ଅକ୍ଷର **) hb_calloc (1 + HB_SHAPERS_ଗଣନା, ଆକାର (ଅଚଳ ଅକ୍ଷର *));
    ଯଦି (unlikely (!shaper_list))
      ଫେରନ୍ତୁ nullptr;

    ଅଚଳ hb_shaper_entry_t *shapers = _hb_shapers_get ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ (i = 0; i < HB_SHAPERS_ଗଣନା; i++)
      shaper_list[i] = shapers[i].name;
    shaper_list[i] = nullptr;

    hb_atexit (free_static_shaper_list);

    ଫେରନ୍ତୁ shaper_list;
  }
  ସ୍ଥିର ଶୂନ୍ୟ destroy (ଅଚଳ ଅକ୍ଷର **l)
  { hb_free (l); }
  ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ * get_null ()
  { ଫେରନ୍ତୁ nil_shaper_list; }
} static_shaper_list;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ free_static_shaper_list ()
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
ଅଚଳ ଅକ୍ଷର **
hb_shape_list_shapers ()
{
  ଫେରନ୍ତୁ static_shaper_list.get_unconst ();
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
	       ଅଚଳ hb_feature_t *features,
	       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା        ଅଙ୍କ_features,
	       ଅଚଳ ଅକ୍ଷର * ଅଚଳ *shaper_list)
{
  ଯଦି (unlikely (!buffer->len))
    ଫେରନ୍ତୁ ସତ୍ୟ;

  buffer->enter ();

  hb_buffer_t *text_buffer = nullptr;
  ଯଦି (buffer->flags & HB_BUFFER_FLAG_VERIFY)
  {
    text_buffer = hb_buffer_create ();
    hb_buffer_append (text_buffer, buffer, 0, -1);
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached2 (font->face, &buffer->props,
							      features, ଅଙ୍କ_features,
							      font->coords, font->has_nonzero_coords ? font->ଅଙ୍କ_coords : 0,
							      shaper_list);

  hb_bool_t res = hb_shape_plan_execute (shape_plan, font, buffer, features, ଅଙ୍କ_features);

  hb_shape_plan_destroy (shape_plan);

  ଯଦି (text_buffer)
  {
    ଯଦି (res && buffer->successful
	    && text_buffer->successful
	    && !buffer->verify (text_buffer,
				font,
				features,
				ଅଙ୍କ_features,
				shaper_list))
      res = ମିଥ୍ୟା;
    hb_buffer_destroy (text_buffer);
  }

  buffer->leave ();

  ଫେରନ୍ତୁ res;
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
ଶୂନ୍ୟ
hb_shape (hb_font_t           *font,
	  hb_buffer_t         *buffer,
	  ଅଚଳ hb_feature_t  *features,
	  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା         ଅଙ୍କ_features)
{
  hb_shape_full (font, buffer, features, ଅଙ୍କ_features, nullptr);
}


#ifdef HB_EXPERIMENTAL_API
#ifndef HB_NO_VAR

ସ୍ଥିର ଦଶମିକ
buffer_advance (hb_buffer_t *buffer)
{
  ଦଶମିକ a = 0;
  auto *pos = buffer->pos;
  unsigned ଗଣନା_2 = buffer->len;
  ଯଦି (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      a += pos[i].x_advance;
  ଅନ୍ୟଥା
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      a += pos[i].y_advance;
  ଫେରନ୍ତୁ a;
}

ସ୍ଥିର ଶୂନ୍ୟ
reset_buffer (hb_buffer_t *buffer,
	      hb_array_t<ଅଚଳ hb_glyph_info_t> text)
{
  ପ୍ରତିଜ୍ଞା (buffer->ensure (text.length));
  buffer->have_positions = ମିଥ୍ୟା;
  buffer->len = text.length;
  hb_memcpy (buffer->info, text.arrayZ, text.length * ଆକାର (buffer->info[0]));
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
		  ଅଚଳ hb_feature_t *features,
		  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା        ଅଙ୍କ_features,
		  ଅଚଳ ଅକ୍ଷର * ଅଚଳ *shaper_list,
		  ଦଶମିକ               min_target_advance,
		  ଦଶମିକ               max_target_advance,
		  ଦଶମିକ              *advance, /* IN/OUT */
		  hb_tag_t           *var_tag, /* OUT */
		  ଦଶମିକ              *var_ମୂଲ୍ୟ /* OUT */)
{
  // TODO Negative font scales?

  /* If default advance already matches target, nothing to do. Shape and return. */
  ଯଦି (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_ମୂଲ୍ୟ = 0.0f;
    ଫେରନ୍ତୁ hb_shape_full (font, buffer,
			  features, ଅଙ୍କ_features,
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
  ପାଇଁ (unsigned i = 0; i < ARRAY_LENGTH (tags); i++)
    ଯଦି (hb_ot_var_find_axis_info (face, tags[i], &axis_info))
    {
      tag = *var_tag = tags[i];
      ଭାଙ୍ଗ;
    }

  /* If no suitable variation axis found, can't justify.  Just shape and return. */
  ଯଦି (!tag)
  {
    *var_tag = HB_TAG_NONE;
    *var_ମୂଲ୍ୟ = 0.0f;
    ଯଦି (hb_shape_full (font, buffer,
		       features, ଅଙ୍କ_features,
		       shaper_list))
    {
      *advance = buffer_advance (buffer);
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  /* Copy buffer text as we need it so we can shape multiple times. */
  unsigned text_len = buffer->len;
  auto *text_info = (hb_glyph_info_t *) hb_malloc2 (text_len, ଆକାର (buffer->info[0]));
  ଯଦି (unlikely (text_len && !text_info))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  HB_SCOPE_GUARD (hb_free (text_info));
  hb_memcpy (text_info, buffer->info, text_len * ଆକାର (buffer->info[0]));
  auto text = hb_array<ଅଚଳ hb_glyph_info_t> (text_info, text_len);

  /* If default advance was not provided to us, calculate it. */
  ଯଦି (!*advance)
  {
    hb_font_set_variation (font, tag, axis_info.default_ମୂଲ୍ୟ);
    ଯଦି (!hb_shape_full (font, buffer,
			features, ଅଙ୍କ_features,
			shaper_list))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    *advance = buffer_advance (buffer);
  }

  /* If default advance already matches target, nothing to do. Shape and return.
   * Do this again, in case advance was just calculated.
   */
  ଯଦି (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_ମୂଲ୍ୟ = 0.0f;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  /* Prepare for running the solver. */
  ଦ୍ୱିଗୁଣ a, b, ya, yb;
  ଯଦି (*advance < min_target_advance)
  {
    /* Need to expand. */
    ya = (ଦ୍ୱିଗୁଣ) *advance;
    a = (ଦ୍ୱିଗୁଣ) axis_info.default_ମୂଲ୍ୟ;
    b = (ଦ୍ୱିଗୁଣ) axis_info.max_ମୂଲ୍ୟ;

    /* Shape buffer for maximum expansion to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (ଦଶମିକ) b);
    reset_buffer (buffer, text);
    ଯଦି (!hb_shape_full (font, buffer,
			features, ଅଙ୍କ_features,
			shaper_list))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    yb = (ଦ୍ୱିଗୁଣ) buffer_advance (buffer);
    /* If the maximum expansion is less than max target,
     * there's nothing to solve for. Just return it. */
    ଯଦି (yb <= (ଦ୍ୱିଗୁଣ) max_target_advance)
    {
      *var_ମୂଲ୍ୟ = (ଦଶମିକ) b;
      *advance = (ଦଶମିକ) yb;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
  }
  ଅନ୍ୟଥା
  {
    /* Need to shrink. */
    yb = (ଦ୍ୱିଗୁଣ) *advance;
    a = (ଦ୍ୱିଗୁଣ) axis_info.min_ମୂଲ୍ୟ;
    b = (ଦ୍ୱିଗୁଣ) axis_info.default_ମୂଲ୍ୟ;

    /* Shape buffer for maximum shrinkate to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (ଦଶମିକ) a);
    reset_buffer (buffer, text);
    ଯଦି (!hb_shape_full (font, buffer,
			features, ଅଙ୍କ_features,
			shaper_list))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ya = (ଦ୍ୱିଗୁଣ) buffer_advance (buffer);
    /* If the maximum shrinkate is more than min target,
     * there's nothing to solve for. Just return it. */
    ଯଦି (ya >= (ଦ୍ୱିଗୁଣ) min_target_advance)
    {
      *var_ମୂଲ୍ୟ = (ଦଶମିକ) a;
      *advance = (ଦଶମିକ) ya;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
  }

  /* Run the solver to find a var axis value that hits
   * the desired width. */

  ଦ୍ୱିଗୁଣ epsilon = (b - a) / (1<<14);
  ତାର୍କିକ failed = ମିଥ୍ୟା;

  auto f = [&] (ଦ୍ୱିଗୁଣ x)
  {
    hb_font_set_variation (font, tag, (ଦଶମିକ) x);
    reset_buffer (buffer, text);
    ଯଦି (unlikely (!hb_shape_full (font, buffer,
				  features, ଅଙ୍କ_features,
				  shaper_list)))
    {
      failed = ସତ୍ୟ;
      ଫେରନ୍ତୁ (ଦ୍ୱିଗୁଣ) min_target_advance;
    }

    ଦ୍ୱିଗୁଣ w = (ଦ୍ୱିଗୁଣ) buffer_advance (buffer);
    DEBUG_ସନ୍ଦେଶ (JUSTIFY, nullptr, "Trying '%c%c%c%c' axis parameter %f. Advance %g. Target: min %g max %g",
	       HB_UNTAG (tag), x, w,
	       (ଦ୍ୱିଗୁଣ) min_target_advance, (ଦ୍ୱିଗୁଣ) max_target_advance);
    ଫେରନ୍ତୁ w;
  };

  ଦ୍ୱିଗୁଣ y = 0;
  ଦ୍ୱିଗୁଣ itp = solve_itp (f,
			  a, b,
			  epsilon,
			  (ଦ୍ୱିଗୁଣ) min_target_advance, (ଦ୍ୱିଗୁଣ) max_target_advance,
			  ya, yb, y);

  ଯଦି (failed)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  *var_ମୂଲ୍ୟ = (ଦଶମିକ) itp;
  *advance = (ଦଶମିକ) y;

  ଫେରନ୍ତୁ ସତ୍ୟ;
}
#endif
#endif


#endif
