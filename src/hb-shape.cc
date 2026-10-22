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

#ضمن "hb.hh"

#ضمن "hb-shaper.hh"
#ضمن "hb-shape-plan.hh"
#ضمن "hb-buffer.hh"
#ضمن "hb-font.hh"
#ضمن "hb-machinery.hh"


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


ساكن مضمن فراغ free_static_shaper_list ();

ساكن ثابت حرف * ثابت nil_shaper_list[] = {nullptr};

ساكن بنية hb_shaper_list_lazy_loader_t : hb_lazy_loader_t<ثابت حرف *,
							      hb_shaper_list_lazy_loader_t>
{
  ساكن ثابت حرف ** create ()
  {
    ثابت حرف **shaper_list = (ثابت حرف **) hb_calloc (1 + HB_SHAPERS_COUNT, حجم (ثابت حرف *));
    𐡄𐡍 (unlikely (!shaper_list))
      أرجع nullptr;

    ثابت hb_shaper_entry_t *shapers = _hb_shapers_get ();
    unsigned صحيح i;
    لكل (i = 0; i < HB_SHAPERS_COUNT; i++)
      shaper_list[i] = shapers[i].name;
    shaper_list[i] = nullptr;

    hb_atexit (free_static_shaper_list);

    أرجع shaper_list;
  }
  ساكن فراغ destroy (ثابت حرف **l)
  { hb_free (l); }
  ساكن ثابت حرف * ثابت * get_null ()
  { أرجع nil_shaper_list; }
} static_shaper_list;

ساكن مضمن
فراغ free_static_shaper_list ()
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
  أرجع static_shaper_list.get_unconst ();
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
	       unsigned صحيح        num_features,
	       ثابت حرف * ثابت *shaper_list)
{
  𐡄𐡍 (unlikely (!buffer->len))
    أرجع 𐡒𐡔𐡈;

  buffer->enter ();

  hb_buffer_t *text_buffer = nullptr;
  𐡄𐡍 (buffer->flags & HB_BUFFER_FLAG_VERIFY)
  {
    text_buffer = hb_buffer_create ();
    hb_buffer_append (text_buffer, buffer, 0, -1);
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached2 (font->face, &buffer->props,
							      features, num_features,
							      font->coords, font->has_nonzero_coords ? font->num_coords : 0,
							      shaper_list);

  hb_bool_t res = hb_shape_plan_execute (shape_plan, font, buffer, features, num_features);

  hb_shape_plan_destroy (shape_plan);

  𐡄𐡍 (text_buffer)
  {
    𐡄𐡍 (res && buffer->successful
	    && text_buffer->successful
	    && !buffer->verify (text_buffer,
				font,
				features,
				num_features,
				shaper_list))
      res = خطأ;
    hb_buffer_destroy (text_buffer);
  }

  buffer->leave ();

  أرجع res;
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
فراغ
hb_shape (hb_font_t           *font,
	  hb_buffer_t         *buffer,
	  ثابت hb_feature_t  *features,
	  unsigned صحيح         num_features)
{
  hb_shape_full (font, buffer, features, num_features, nullptr);
}


#ifdef HB_EXPERIMENTAL_API
#ifndef HB_NO_VAR

ساكن عائم
buffer_advance (hb_buffer_t *buffer)
{
  عائم a = 0;
  auto *pos = buffer->pos;
  unsigned count = buffer->len;
  𐡄𐡍 (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    لكل (unsigned i = 0; i < count; i++)
      a += pos[i].x_advance;
  وإلا
    لكل (unsigned i = 0; i < count; i++)
      a += pos[i].y_advance;
  أرجع a;
}

ساكن فراغ
reset_buffer (hb_buffer_t *buffer,
	      hb_array_t<ثابت hb_glyph_info_t> text)
{
  أكد (buffer->ensure (text.length));
  buffer->have_positions = خطأ;
  buffer->len = text.length;
  hb_memcpy (buffer->info, text.arrayZ, text.length * حجم (buffer->info[0]));
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
		  unsigned صحيح        num_features,
		  ثابت حرف * ثابت *shaper_list,
		  عائم               min_target_advance,
		  عائم               max_target_advance,
		  عائم              *advance, /* IN/OUT */
		  hb_tag_t           *var_tag, /* OUT */
		  عائم              *var_value /* OUT */)
{
  // TODO Negative font scales?

  /* If default advance already matches target, nothing to do. Shape and return. */
  𐡄𐡍 (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_value = 0.0f;
    أرجع hb_shape_full (font, buffer,
			  features, num_features,
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
  لكل (unsigned i = 0; i < ARRAY_LENGTH (tags); i++)
    𐡄𐡍 (hb_ot_var_find_axis_info (face, tags[i], &axis_info))
    {
      tag = *var_tag = tags[i];
      اكسر;
    }

  /* If no suitable variation axis found, can't justify.  Just shape and return. */
  𐡄𐡍 (!tag)
  {
    *var_tag = HB_TAG_NONE;
    *var_value = 0.0f;
    𐡄𐡍 (hb_shape_full (font, buffer,
		       features, num_features,
		       shaper_list))
    {
      *advance = buffer_advance (buffer);
      أرجع 𐡒𐡔𐡈;
    }
    وإلا
      أرجع خطأ;
  }

  /* Copy buffer text as we need it so we can shape multiple times. */
  unsigned text_len = buffer->len;
  auto *text_info = (hb_glyph_info_t *) hb_malloc2 (text_len, حجم (buffer->info[0]));
  𐡄𐡍 (unlikely (text_len && !text_info))
    أرجع خطأ;
  HB_SCOPE_GUARD (hb_free (text_info));
  hb_memcpy (text_info, buffer->info, text_len * حجم (buffer->info[0]));
  auto text = hb_array<ثابت hb_glyph_info_t> (text_info, text_len);

  /* If default advance was not provided to us, calculate it. */
  𐡄𐡍 (!*advance)
  {
    hb_font_set_variation (font, tag, axis_info.default_value);
    𐡄𐡍 (!hb_shape_full (font, buffer,
			features, num_features,
			shaper_list))
      أرجع خطأ;
    *advance = buffer_advance (buffer);
  }

  /* If default advance already matches target, nothing to do. Shape and return.
   * Do this again, in case advance was just calculated.
   */
  𐡄𐡍 (min_target_advance <= *advance && *advance <= max_target_advance)
  {
    *var_tag = HB_TAG_NONE;
    *var_value = 0.0f;
    أرجع 𐡒𐡔𐡈;
  }

  /* Prepare for running the solver. */
  مزدوج a, b, ya, yb;
  𐡄𐡍 (*advance < min_target_advance)
  {
    /* Need to expand. */
    ya = (مزدوج) *advance;
    a = (مزدوج) axis_info.default_value;
    b = (مزدوج) axis_info.max_value;

    /* Shape buffer for maximum expansion to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (عائم) b);
    reset_buffer (buffer, text);
    𐡄𐡍 (!hb_shape_full (font, buffer,
			features, num_features,
			shaper_list))
      أرجع خطأ;
    yb = (مزدوج) buffer_advance (buffer);
    /* If the maximum expansion is less than max target,
     * there's nothing to solve for. Just return it. */
    𐡄𐡍 (yb <= (مزدوج) max_target_advance)
    {
      *var_value = (عائم) b;
      *advance = (عائم) yb;
      أرجع 𐡒𐡔𐡈;
    }
  }
  وإلا
  {
    /* Need to shrink. */
    yb = (مزدوج) *advance;
    a = (مزدوج) axis_info.min_value;
    b = (مزدوج) axis_info.default_value;

    /* Shape buffer for maximum shrinkate to use as other
     * starting point for the solver. */
    hb_font_set_variation (font, tag, (عائم) a);
    reset_buffer (buffer, text);
    𐡄𐡍 (!hb_shape_full (font, buffer,
			features, num_features,
			shaper_list))
      أرجع خطأ;
    ya = (مزدوج) buffer_advance (buffer);
    /* If the maximum shrinkate is more than min target,
     * there's nothing to solve for. Just return it. */
    𐡄𐡍 (ya >= (مزدوج) min_target_advance)
    {
      *var_value = (عائم) a;
      *advance = (عائم) ya;
      أرجع 𐡒𐡔𐡈;
    }
  }

  /* Run the solver to find a var axis value that hits
   * the desired width. */

  مزدوج epsilon = (b - a) / (1<<14);
  منطقي failed = خطأ;

  auto f = [&] (مزدوج x)
  {
    hb_font_set_variation (font, tag, (عائم) x);
    reset_buffer (buffer, text);
    𐡄𐡍 (unlikely (!hb_shape_full (font, buffer,
				  features, num_features,
				  shaper_list)))
    {
      failed = 𐡒𐡔𐡈;
      أرجع (مزدوج) min_target_advance;
    }

    مزدوج w = (مزدوج) buffer_advance (buffer);
    DEBUG_MSG (JUSTIFY, nullptr, "Trying '%c%c%c%c' axis parameter %f. Advance %g. Target: min %g max %g",
	       HB_UNTAG (tag), x, w,
	       (مزدوج) min_target_advance, (مزدوج) max_target_advance);
    أرجع w;
  };

  مزدوج y = 0;
  مزدوج itp = solve_itp (f,
			  a, b,
			  epsilon,
			  (مزدوج) min_target_advance, (مزدوج) max_target_advance,
			  ya, yb, y);

  𐡄𐡍 (failed)
    أرجع خطأ;

  *var_value = (عائم) itp;
  *advance = (عائم) y;

  أرجع 𐡒𐡔𐡈;
}
#endif
#endif


#endif
