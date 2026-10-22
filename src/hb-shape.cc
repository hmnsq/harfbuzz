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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-shaper.hh"
#অন্তর্ভাব "hb-shape-plan.hh"
#অন্তর্ভাব "hb-buffer.hh"
#অন্তর্ভাব "hb-font.hh"
#অন্তর্ভাব "hb-machinery.hh"


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


স্থির অন্তঃস্থ শূন্য free_static_shaper_list ();

স্থির ধ্রুব বর্ণ * ধ্রুব nil_shaper_list[] = {nullptr};

স্থির সংরচনা hb_shaper_list_lazy_loader_t : hb_lazy_loader_t<ধ্রুব বর্ণ *,
							      hb_shaper_list_lazy_loader_t>
{
  স্থির ধ্রুব বর্ণ ** create ()
  {
    ধ্রুব বর্ণ **shaper_list = (ধ্রুব বর্ণ **) hb_calloc (1 + HB_SHAPERS_গুনতি, পরিমাণ (ধ্রুব বর্ণ *));
    যদি (unlikely (!shaper_list))
      প্রত্যাগম nullptr;

    ধ্রুব hb_shaper_entry_t *shapers = _hb_shapers_get ();
    unsigned পূর্ণাঙ্ক i;
    প্রতি (i = 0; i < HB_SHAPERS_গুনতি; i++)
      shaper_list[i] = shapers[i].name;
    shaper_list[i] = nullptr;

    hb_atexit (free_static_shaper_list);

    প্রত্যাগম shaper_list;
  }
  স্থির শূন্য destroy (ধ্রুব বর্ণ **l)
  { hb_free (l); }
  স্থির ধ্রুব বর্ণ * ধ্রুব * get_null ()
  { প্রত্যাগম nil_shaper_list; }
} static_shaper_list;

স্থির অন্তঃস্থ
শূন্য free_static_shaper_list ()
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
ধ্রুব বর্ণ **
hb_shape_list_shapers ()
{
  প্রত্যাগম static_shaper_list.get_unconst ();
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
	       ধ্রুব hb_feature_t *features,
	       unsigned পূর্ণাঙ্ক        অঙ্ক_features,
	       ধ্রুব বর্ণ * ধ্রুব *shaper_list)
{
  যদি (unlikely (!buffer->len))
    প্রত্যাগম সত্য;

  buffer->enter ();

  hb_buffer_t *text_buffer = nullptr;
  যদি (buffer->flags & HB_BUFFER_FLAG_VERIFY)
  {
    text_buffer = hb_buffer_create ();
    hb_buffer_append (text_buffer, buffer, 0, -1);
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached2 (font->face, &buffer->props,
							      features, অঙ্ক_features,
							      font->coords, font->has_nonzero_coords ? font->অঙ্ক_coords : 0,
							      shaper_list);

  hb_bool_t res = hb_shape_plan_execute (shape_plan, font, buffer, features, অঙ্ক_features);

  hb_shape_plan_destroy (shape_plan);

  যদি (text_buffer)
  {
    যদি (res && buffer->successful
	    && text_buffer->successful
	    && !buffer->verify (text_buffer,
				font,
				features,
				অঙ্ক_features,
				shaper_list))
      res = অসত্য;
    hb_buffer_destroy (text_buffer);
  }

  buffer->leave ();

  প্রত্যাগম res;
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
শূন্য
hb_shape (hb_font_t           *font,
	  hb_buffer_t         *buffer,
	  ধ্রুব hb_feature_t  *features,
	  unsigned পূর্ণাঙ্ক         অঙ্ক_features)
{
  hb_shape_full (font, buffer, features, অঙ্ক_features, nullptr);
}


#ifdef HB_EXPERIMENTAL_API
#ifndef HB_NO_VAR

স্থির ভিন্ন
buffer_advance (hb_buffer_t *buffer)
{
  ভিন্ন a = 0;
  auto *pos = buffer->pos;
  unsigned গুনতি = buffer->len;
  যদি (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    প্রতি (unsigned i = 0; i < গুনতি; i++)
      a += pos[i].x_advance;
  অন্যথা
    প্রতি (unsigned i = 0; i < গুনতি; i++)
      a += pos[i].y_advance;
  প্রত্যাগম a;
}

স্থির শূন্য
reset_buffer (hb_buffer_t *buffer,
	      hb_array_t<ধ্রুব hb_glyph_info_t> text)
{
  প্রতিজ্ঞা (buffer->ensure (text.length));
  buffer->have_positions = অসত্য;
  buffer->len = text.length;
  hb_memcpy (buffer->info, text.arrayZ, text.length * পরিমাণ (buffer->info[0]));
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
		  ধ্রুব hb_feature_t *features,
		  unsigned পূর্ণাঙ্ক        অঙ্ক_features,
		  ধ্রুব বর্ণ * ধ্রুব *shaper_list,
		  ভিন্ন               min_target_advance,
		  ভিন্ন               max_target_advance,
		  ভিন্ন              *advance, /* IN/OUT */
		  hb_tag_t           *var_tag, /* OUT */
		  ভিন্ন              *var_মান /* OUT */)
{
  // TODO Negative font scales?

  /* If default advance already matches target, nothing to do. Shape and return. */
  যদি (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_মান = 0.0f;
    প্রত্যাগম hb_shape_full (font, buffer,
			  features, অঙ্ক_features,
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
  প্রতি (unsigned i = 0; i < ARRAY_LENGTH (tags); i++)
    যদি (hb_ot_var_find_axis_info (face, tags[i], &axis_info))
    {
      tag = *var_tag = tags[i];
      ভঙ্গ;
    }

  /* If no suitable variation axis found, can't justify.  Just shape and return. */
  যদি (!tag)
  {
    *var_tag = HB_TAG_NONE;
    *var_মান = 0.0f;
    যদি (hb_shape_full (font, buffer,
		       features, অঙ্ক_features,
		       shaper_list))
    {
      *advance = buffer_advance (buffer);
      প্রত্যাগম সত্য;
    }
    অন্যথা
      প্রত্যাগম অসত্য;
  }

  /* Copy buffer text as we need it so we can shape multiple times. */
  unsigned text_len = buffer->len;
  auto *text_info = (hb_glyph_info_t *) hb_malloc2 (text_len, পরিমাণ (buffer->info[0]));
  যদি (unlikely (text_len && !text_info))
    প্রত্যাগম অসত্য;
  HB_SCOPE_GUARD (hb_free (text_info));
  hb_memcpy (text_info, buffer->info, text_len * পরিমাণ (buffer->info[0]));
  auto text = hb_array<ধ্রুব hb_glyph_info_t> (text_info, text_len);

  /* If default advance was not provided to us, calculate it. */
  যদি (!*advance)
  {
    hb_font_set_variation (font, tag, axis_info.default_মান);
    যদি (!hb_shape_full (font, buffer,
			features, অঙ্ক_features,
			shaper_list))
      প্রত্যাগম অসত্য;
    *advance = buffer_advance (buffer);
  }

  /* If default advance already matches target, nothing to do. Shape and return.
   * Do this again, in case advance was just calculated.
   */
  যদি (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_মান = 0.0f;
    প্রত্যাগম সত্য;
  }

  /* Prepare for running the solver. */
  দ্বিগুণ a, b, ya, yb;
  যদি (*advance < min_target_advance)
  {
    /* Need to expand. */
    ya = (দ্বিগুণ) *advance;
    a = (দ্বিগুণ) axis_info.default_মান;
    b = (দ্বিগুণ) axis_info.max_মান;

    /* Shape buffer for maximum expansion to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (ভিন্ন) b);
    reset_buffer (buffer, text);
    যদি (!hb_shape_full (font, buffer,
			features, অঙ্ক_features,
			shaper_list))
      প্রত্যাগম অসত্য;
    yb = (দ্বিগুণ) buffer_advance (buffer);
    /* If the maximum expansion is less than max target,
     * there's nothing to solve for. Just return it. */
    যদি (yb <= (দ্বিগুণ) max_target_advance)
    {
      *var_মান = (ভিন্ন) b;
      *advance = (ভিন্ন) yb;
      প্রত্যাগম সত্য;
    }
  }
  অন্যথা
  {
    /* Need to shrink. */
    yb = (দ্বিগুণ) *advance;
    a = (দ্বিগুণ) axis_info.min_মান;
    b = (দ্বিগুণ) axis_info.default_মান;

    /* Shape buffer for maximum shrinkate to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (ভিন্ন) a);
    reset_buffer (buffer, text);
    যদি (!hb_shape_full (font, buffer,
			features, অঙ্ক_features,
			shaper_list))
      প্রত্যাগম অসত্য;
    ya = (দ্বিগুণ) buffer_advance (buffer);
    /* If the maximum shrinkate is more than min target,
     * there's nothing to solve for. Just return it. */
    যদি (ya >= (দ্বিগুণ) min_target_advance)
    {
      *var_মান = (ভিন্ন) a;
      *advance = (ভিন্ন) ya;
      প্রত্যাগম সত্য;
    }
  }

  /* Run the solver to find a var axis value that hits
   * the desired width. */

  দ্বিগুণ epsilon = (b - a) / (1<<14);
  তার্কিক failed = অসত্য;

  auto f = [&] (দ্বিগুণ x)
  {
    hb_font_set_variation (font, tag, (ভিন্ন) x);
    reset_buffer (buffer, text);
    যদি (unlikely (!hb_shape_full (font, buffer,
				  features, অঙ্ক_features,
				  shaper_list)))
    {
      failed = সত্য;
      প্রত্যাগম (দ্বিগুণ) min_target_advance;
    }

    দ্বিগুণ w = (দ্বিগুণ) buffer_advance (buffer);
    DEBUG_বার্তা (JUSTIFY, nullptr, "Trying '%c%c%c%c' axis parameter %f. Advance %g. Target: min %g max %g",
	       HB_UNTAG (tag), x, w,
	       (দ্বিগুণ) min_target_advance, (দ্বিগুণ) max_target_advance);
    প্রত্যাগম w;
  };

  দ্বিগুণ y = 0;
  দ্বিগুণ itp = solve_itp (f,
			  a, b,
			  epsilon,
			  (দ্বিগুণ) min_target_advance, (দ্বিগুণ) max_target_advance,
			  ya, yb, y);

  যদি (failed)
    প্রত্যাগম অসত্য;

  *var_মান = (ভিন্ন) itp;
  *advance = (ভিন্ন) y;

  প্রত্যাগম সত্য;
}
#endif
#endif


#endif
