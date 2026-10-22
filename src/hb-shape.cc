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


#اگر_غیر_تعریف_شدہ HB_NO_SHAPER

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


ساکن اندرونی خالی free_static_shaper_list ();

ساکن ثابت حرف * ثابت nil_shaper_list[] = {خالی_اشاریہ};

ساکن ساخت hb_shaper_list_lazy_loader_t : hb_lazy_loader_t<ثابت حرف *,
							      hb_shaper_list_lazy_loader_t>
{
  ساکن ثابت حرف ** create ()
  {
    ثابت حرف **shaper_list = (ثابت حرف **) hb_calloc (1 + HB_SHAPERS_گنتی, حجم (ثابت حرف *));
    اگر (unlikely (!shaper_list))
      واپس خالی_اشاریہ;

    ثابت hb_shaper_اندراج_t *shapers = _hb_shapers_get ();
    غیرعلامتی صحیح i;
    برائے (i = 0; i < HB_SHAPERS_گنتی; i++)
      shaper_list[i] = shapers[i].name;
    shaper_list[i] = خالی_اشاریہ;

    hb_atexit (free_static_shaper_list);

    واپس shaper_list;
  }
  ساکن خالی destroy (ثابت حرف **l)
  { hb_free (l); }
  ساکن ثابت حرف * ثابت * get_null ()
  { واپس nil_shaper_list; }
} static_shaper_list;

ساکن اندرونی
خالی free_static_shaper_list ()
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
ثابت حرف **
hb_shape_list_shapers ()
{
  واپس static_shaper_list.get_unconst ();
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
hb_shape_بھرا (hb_font_t          *font,
	       hb_بفر_t        *بفر,
	       ثابت hb_feature_t *features,
	       غیرعلامتی صحیح        عدد_features,
	       ثابت حرف * ثابت *shaper_list)
{
  اگر (unlikely (!بفر->len))
    واپس سچ;

  بفر->enter ();

  hb_بفر_t *متن_بفر = خالی_اشاریہ;
  اگر (بفر->flags & HB_بفر_FLAG_VERIFY)
  {
    متن_بفر = hb_بفر_create ();
    hb_بفر_append (متن_بفر, بفر, 0, -1);
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached2 (font->face, &بفر->props,
							      features, عدد_features,
							      font->coords, font->has_nonzero_coords ? font->عدد_coords : 0,
							      shaper_list);

  hb_bool_t res = hb_shape_plan_execute (shape_plan, font, بفر, features, عدد_features);

  hb_shape_plan_destroy (shape_plan);

  اگر (متن_بفر)
  {
    اگر (res && بفر->successful
	    && متن_بفر->successful
	    && !بفر->verify (متن_بفر,
				font,
				features,
				عدد_features,
				shaper_list))
      res = جھوٹ;
    hb_بفر_destroy (متن_بفر);
  }

  بفر->leave ();

  واپس res;
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
خالی
hb_shape (hb_font_t           *font,
	  hb_بفر_t         *بفر,
	  ثابت hb_feature_t  *features,
	  غیرعلامتی صحیح         عدد_features)
{
  hb_shape_بھرا (font, بفر, features, عدد_features, خالی_اشاریہ);
}


#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
#اگر_غیر_تعریف_شدہ HB_NO_VAR

ساکن اعشاری
بفر_advance (hb_بفر_t *بفر)
{
  اعشاری a = 0;
  خودکار *pos = بفر->pos;
  غیرعلامتی گنتی = بفر->len;
  اگر (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      a += pos[i].x_advance;
  ورنہ
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      a += pos[i].y_advance;
  واپس a;
}

ساکن خالی
reset_بفر (hb_بفر_t *بفر,
	      hb_array_t<ثابت hb_glyph_info_t> متن)
{
  أكد (بفر->ensure (متن.لمبائی));
  بفر->have_positions = جھوٹ;
  بفر->len = متن.لمبائی;
  hb_memcpy (بفر->info, متن.arrayZ, متن.لمبائی * حجم (بفر->info[0]));
  hb_بفر_سیٹ_content_type (بفر, HB_بفر_CONTENT_TYPE_UNICODE);
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
		  hb_بفر_t        *بفر,
		  ثابت hb_feature_t *features,
		  غیرعلامتی صحیح        عدد_features,
		  ثابت حرف * ثابت *shaper_list,
		  اعشاری               min_ہدف_advance,
		  اعشاری               max_ہدف_advance,
		  اعشاری              *advance, /* IN/OUT */
		  hb_لیبل_t           *var_لیبل, /*برآمد*/
		  اعشاری              *var_قدر /*برآمد*/)
{
  // TODO Negative font scales?

  /* If default advance already matches target, nothing to do. Shape and return. */
  اگر (min_ہدف_advance <= *advance && *advance <= max_ہدف_advance)
  {
    *var_لیبل = HB_لیبل_NONE;
    *var_قدر = 0.0f;
    واپس hb_shape_بھرا (font, بفر,
			  features, عدد_features,
			  shaper_list);
  }

  hb_face_t *face = font->face;

  /* Choose variation tag to use for justification. */

  hb_لیبل_t لیبل_2 = HB_لیبل_NONE;
  hb_ot_var_axis_info_t axis_info;

  hb_لیبل_t لیبل_فہرست[] =
  {
    HB_لیبل ('j','s','t','f'),
    HB_لیبل ('w','d','t','h'),
  };
  برائے (غیرعلامتی i = 0; i < ARRAY_لمبائی (لیبل_فہرست); i++)
    اگر (hb_ot_var_ڈھونڈو_axis_info (face, لیبل_فہرست[i], &axis_info))
    {
      لیبل_2 = *var_لیبل = لیبل_فہرست[i];
      توڑو;
    }

  /* If no suitable variation axis found, can't justify.  Just shape and return. */
  اگر (!لیبل_2)
  {
    *var_لیبل = HB_لیبل_NONE;
    *var_قدر = 0.0f;
    اگر (hb_shape_بھرا (font, بفر,
		       features, عدد_features,
		       shaper_list))
    {
      *advance = بفر_advance (بفر);
      واپس سچ;
    }
    ورنہ
      واپس جھوٹ;
  }

  /* Copy buffer text as we need it so we can shape multiple times. */
  غیرعلامتی متن_len = بفر->len;
  خودکار *متن_info = (hb_glyph_info_t *) hb_malloc2 (متن_len, حجم (بفر->info[0]));
  اگر (unlikely (متن_len && !متن_info))
    واپس جھوٹ;
  HB_SCOPE_GUARD (hb_free (متن_info));
  hb_memcpy (متن_info, بفر->info, متن_len * حجم (بفر->info[0]));
  خودکار متن = hb_array<ثابت hb_glyph_info_t> (متن_info, متن_len);

  /* If default advance was not provided to us, calculate it. */
  اگر (!*advance)
  {
    hb_font_سیٹ_variation (font, لیبل_2, axis_info.default_قدر);
    اگر (!hb_shape_بھرا (font, بفر,
			features, عدد_features,
			shaper_list))
      واپس جھوٹ;
    *advance = بفر_advance (بفر);
  }

  /* If default advance already matches target, nothing to do. Shape and return.
   * Do this again, in case advance was just calculated.
   */
  اگر (min_ہدف_advance <= *advance && *advance <= max_ہدف_advance)
  {
    *var_لیبل = HB_لیبل_NONE;
    *var_قدر = 0.0f;
    واپس سچ;
  }

  /* Prepare for running the solver. */
  دوہرا a, b, ya, yb;
  اگر (*advance < min_ہدف_advance)
  {
    /* Need to expand. */
    ya = (دوہرا) *advance;
    a = (دوہرا) axis_info.default_قدر;
    b = (دوہرا) axis_info.max_قدر;

    /* Shape buffer for maximum expansion to use as other
     * starting point for the solver. */
    hb_font_سیٹ_variation (font, لیبل_2, (اعشاری) b);
    reset_بفر (بفر, متن);
    اگر (!hb_shape_بھرا (font, بفر,
			features, عدد_features,
			shaper_list))
      واپس جھوٹ;
    yb = (دوہرا) بفر_advance (بفر);
    /* If the maximum expansion is less than max target,
     * there's nothing to solve for. Just return it. */
    اگر (yb <= (دوہرا) max_ہدف_advance)
    {
      *var_قدر = (اعشاری) b;
      *advance = (اعشاری) yb;
      واپس سچ;
    }
  }
  ورنہ
  {
    /* Need to shrink. */
    yb = (دوہرا) *advance;
    a = (دوہرا) axis_info.min_قدر;
    b = (دوہرا) axis_info.default_قدر;

    /* Shape buffer for maximum shrinkate to use as other
     * starting point for the solver. */
    hb_font_سیٹ_variation (font, لیبل_2, (اعشاری) a);
    reset_بفر (بفر, متن);
    اگر (!hb_shape_بھرا (font, بفر,
			features, عدد_features,
			shaper_list))
      واپس جھوٹ;
    ya = (دوہرا) بفر_advance (بفر);
    /* If the maximum shrinkate is more than min target,
     * there's nothing to solve for. Just return it. */
    اگر (ya >= (دوہرا) min_ہدف_advance)
    {
      *var_قدر = (اعشاری) a;
      *advance = (اعشاری) ya;
      واپس سچ;
    }
  }

  /* Run the solver to find a var axis value that hits
   * the desired width. */

  دوہرا epsilon = (b - a) / (1<<14);
  منطقی failed = جھوٹ;

  خودکار f = [&] (دوہرا x)
  {
    hb_font_سیٹ_variation (font, لیبل_2, (اعشاری) x);
    reset_بفر (بفر, متن);
    اگر (unlikely (!hb_shape_بھرا (font, بفر,
				  features, عدد_features,
				  shaper_list)))
    {
      failed = سچ;
      واپس (دوہرا) min_ہدف_advance;
    }

    دوہرا w = (دوہرا) بفر_advance (بفر);
    DEBUG_پیغام (JUSTIFY, خالی_اشاریہ, "Trying '%c%c%c%c' axis parameter %f. Advance %g. Target: min %g max %g",
	       HB_UNTAG (لیبل_2), x, w,
	       (دوہرا) min_ہدف_advance, (دوہرا) max_ہدف_advance);
    واپس w;
  };

  دوہرا y = 0;
  دوہرا itp = solve_itp (f,
			  a, b,
			  epsilon,
			  (دوہرا) min_ہدف_advance, (دوہرا) max_ہدف_advance,
			  ya, yb, y);

  اگر (failed)
    واپس جھوٹ;

  *var_قدر = (اعشاری) itp;
  *advance = (اعشاری) y;

  واپس سچ;
}
#اگر_ختم
#اگر_ختم


#اگر_ختم
