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

#شامل "hb.hh"

#شامل "hb-shaper.hh"
#شامل "hb-shape-plan.hh"
#شامل "hb-buffer.hh"
#شامل "hb-font.hh"
#شامل "hb-machinery.hh"


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


ایستا درونخطی تهی free_static_shaper_list ();

ایستا ثابت نویسه * ثابت nil_shaper_list[] = {nullptr};

ایستا ساختار hb_shaper_list_lazy_loader_t : hb_lazy_loader_t<ثابت نویسه *,
							      hb_shaper_list_lazy_loader_t>
{
  ایستا ثابت نویسه ** create ()
  {
    ثابت نویسه **shaper_list = (ثابت نویسه **) hb_calloc (1 + HB_SHAPERS_شمار, اندازه (ثابت نویسه *));
    اگر (unlikely (!shaper_list))
      بازگشت nullptr;

    ثابت hb_shaper_entry_t *shapers = _hb_shapers_get ();
    unsigned صحیح i;
    برای (i = 0; i < HB_SHAPERS_شمار; i++)
      shaper_list[i] = shapers[i].name;
    shaper_list[i] = nullptr;

    hb_atexit (free_static_shaper_list);

    بازگشت shaper_list;
  }
  ایستا تهی destroy (ثابت نویسه **l)
  { hb_free (l); }
  ایستا ثابت نویسه * ثابت * get_null ()
  { بازگشت nil_shaper_list; }
} static_shaper_list;

ایستا درونخطی
تهی free_static_shaper_list ()
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
ثابت نویسه **
hb_shape_list_shapers ()
{
  بازگشت static_shaper_list.get_unconst ();
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
	       ثابت hb_feature_t *features,
	       unsigned صحیح        رقم_features,
	       ثابت نویسه * ثابت *shaper_list)
{
  اگر (unlikely (!buffer->len))
    بازگشت درست;

  buffer->enter ();

  hb_buffer_t *text_buffer = nullptr;
  اگر (buffer->flags & HB_BUFFER_FLAG_VERIFY)
  {
    text_buffer = hb_buffer_create ();
    hb_buffer_append (text_buffer, buffer, 0, -1);
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached2 (font->face, &buffer->props,
							      features, رقم_features,
							      font->coords, font->has_nonzero_coords ? font->رقم_coords : 0,
							      shaper_list);

  hb_bool_t res = hb_shape_plan_execute (shape_plan, font, buffer, features, رقم_features);

  hb_shape_plan_destroy (shape_plan);

  اگر (text_buffer)
  {
    اگر (res && buffer->successful
	    && text_buffer->successful
	    && !buffer->verify (text_buffer,
				font,
				features,
				رقم_features,
				shaper_list))
      res = نادرست;
    hb_buffer_destroy (text_buffer);
  }

  buffer->leave ();

  بازگشت res;
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
تهی
hb_shape (hb_font_t           *font,
	  hb_buffer_t         *buffer,
	  ثابت hb_feature_t  *features,
	  unsigned صحیح         رقم_features)
{
  hb_shape_full (font, buffer, features, رقم_features, nullptr);
}


#ifdef HB_EXPERIMENTAL_API
#ifndef HB_NO_VAR

ایستا اعشاری
buffer_advance (hb_buffer_t *buffer)
{
  اعشاری a = 0;
  auto *pos = buffer->pos;
  unsigned شمار = buffer->len;
  اگر (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    برای (unsigned i = 0; i < شمار; i++)
      a += pos[i].x_advance;
  وگرنه
    برای (unsigned i = 0; i < شمار; i++)
      a += pos[i].y_advance;
  بازگشت a;
}

ایستا تهی
reset_buffer (hb_buffer_t *buffer,
	      hb_array_t<ثابت hb_glyph_info_t> text)
{
  أكد (buffer->ensure (text.length));
  buffer->have_positions = نادرست;
  buffer->len = text.length;
  hb_memcpy (buffer->info, text.arrayZ, text.length * اندازه (buffer->info[0]));
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
		  ثابت hb_feature_t *features,
		  unsigned صحیح        رقم_features,
		  ثابت نویسه * ثابت *shaper_list,
		  اعشاری               min_target_advance,
		  اعشاری               max_target_advance,
		  اعشاری              *advance, /* IN/OUT */
		  hb_tag_t           *var_tag, /* OUT */
		  اعشاری              *var_مقدار /* OUT */)
{
  // TODO Negative font scales?

  /* If default advance already matches target, nothing to do. Shape and return. */
  اگر (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_مقدار = 0.0f;
    بازگشت hb_shape_full (font, buffer,
			  features, رقم_features,
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
  برای (unsigned i = 0; i < ARRAY_LENGTH (tags); i++)
    اگر (hb_ot_var_find_axis_info (face, tags[i], &axis_info))
    {
      tag = *var_tag = tags[i];
      بشکن;
    }

  /* If no suitable variation axis found, can't justify.  Just shape and return. */
  اگر (!tag)
  {
    *var_tag = HB_TAG_NONE;
    *var_مقدار = 0.0f;
    اگر (hb_shape_full (font, buffer,
		       features, رقم_features,
		       shaper_list))
    {
      *advance = buffer_advance (buffer);
      بازگشت درست;
    }
    وگرنه
      بازگشت نادرست;
  }

  /* Copy buffer text as we need it so we can shape multiple times. */
  unsigned text_len = buffer->len;
  auto *text_info = (hb_glyph_info_t *) hb_malloc2 (text_len, اندازه (buffer->info[0]));
  اگر (unlikely (text_len && !text_info))
    بازگشت نادرست;
  HB_SCOPE_GUARD (hb_free (text_info));
  hb_memcpy (text_info, buffer->info, text_len * اندازه (buffer->info[0]));
  auto text = hb_array<ثابت hb_glyph_info_t> (text_info, text_len);

  /* If default advance was not provided to us, calculate it. */
  اگر (!*advance)
  {
    hb_font_set_variation (font, tag, axis_info.default_مقدار);
    اگر (!hb_shape_full (font, buffer,
			features, رقم_features,
			shaper_list))
      بازگشت نادرست;
    *advance = buffer_advance (buffer);
  }

  /* If default advance already matches target, nothing to do. Shape and return.
   * Do this again, in case advance was just calculated.
   */
  اگر (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_مقدار = 0.0f;
    بازگشت درست;
  }

  /* Prepare for running the solver. */
  دوگانه a, b, ya, yb;
  اگر (*advance < min_target_advance)
  {
    /* Need to expand. */
    ya = (دوگانه) *advance;
    a = (دوگانه) axis_info.default_مقدار;
    b = (دوگانه) axis_info.max_مقدار;

    /* Shape buffer for maximum expansion to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (اعشاری) b);
    reset_buffer (buffer, text);
    اگر (!hb_shape_full (font, buffer,
			features, رقم_features,
			shaper_list))
      بازگشت نادرست;
    yb = (دوگانه) buffer_advance (buffer);
    /* If the maximum expansion is less than max target,
     * there's nothing to solve for. Just return it. */
    اگر (yb <= (دوگانه) max_target_advance)
    {
      *var_مقدار = (اعشاری) b;
      *advance = (اعشاری) yb;
      بازگشت درست;
    }
  }
  وگرنه
  {
    /* Need to shrink. */
    yb = (دوگانه) *advance;
    a = (دوگانه) axis_info.min_مقدار;
    b = (دوگانه) axis_info.default_مقدار;

    /* Shape buffer for maximum shrinkate to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (اعشاری) a);
    reset_buffer (buffer, text);
    اگر (!hb_shape_full (font, buffer,
			features, رقم_features,
			shaper_list))
      بازگشت نادرست;
    ya = (دوگانه) buffer_advance (buffer);
    /* If the maximum shrinkate is more than min target,
     * there's nothing to solve for. Just return it. */
    اگر (ya >= (دوگانه) min_target_advance)
    {
      *var_مقدار = (اعشاری) a;
      *advance = (اعشاری) ya;
      بازگشت درست;
    }
  }

  /* Run the solver to find a var axis value that hits
   * the desired width. */

  دوگانه epsilon = (b - a) / (1<<14);
  منطقی failed = نادرست;

  auto f = [&] (دوگانه x)
  {
    hb_font_set_variation (font, tag, (اعشاری) x);
    reset_buffer (buffer, text);
    اگر (unlikely (!hb_shape_full (font, buffer,
				  features, رقم_features,
				  shaper_list)))
    {
      failed = درست;
      بازگشت (دوگانه) min_target_advance;
    }

    دوگانه w = (دوگانه) buffer_advance (buffer);
    DEBUG_پیام (JUSTIFY, nullptr, "Trying '%c%c%c%c' axis parameter %f. Advance %g. Target: min %g max %g",
	       HB_UNTAG (tag), x, w,
	       (دوگانه) min_target_advance, (دوگانه) max_target_advance);
    بازگشت w;
  };

  دوگانه y = 0;
  دوگانه itp = solve_itp (f,
			  a, b,
			  epsilon,
			  (دوگانه) min_target_advance, (دوگانه) max_target_advance,
			  ya, yb, y);

  اگر (failed)
    بازگشت نادرست;

  *var_مقدار = (اعشاری) itp;
  *advance = (اعشاری) y;

  بازگشت درست;
}
#endif
#endif


#endif
