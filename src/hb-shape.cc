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


#جيڪڏهن_وصف_نه_ٿيل HB_NO_SHAPER

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

ساكن ثابت حرف * ثابت nil_shaper_list[] = {خالي_اشارو};

ساكن بنية hb_shaper_list_lazy_loader_t : hb_lazy_loader_t<ثابت حرف *,
							      hb_shaper_list_lazy_loader_t>
{
  ساكن ثابت حرف ** create ()
  {
    ثابت حرف **shaper_list = (ثابت حرف **) hb_calloc (1 + HB_SHAPERS_ڳڻپ, حجم (ثابت حرف *));
    إذا (unlikely (!shaper_list))
      أرجع خالي_اشارو;

    ثابت hb_shaper_داخلا_t *shapers = _hb_shapers_get ();
    بنانشان صحيح i;
    لكل (i = 0; i < HB_SHAPERS_ڳڻپ; i++)
      shaper_list[i] = shapers[i].name;
    shaper_list[i] = خالي_اشارو;

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
hb_shape_ڀريل (hb_font_t          *font,
	       hb_بفر_t        *بفر,
	       ثابت hb_feature_t *features,
	       بنانشان صحيح        انگ_features,
	       ثابت حرف * ثابت *shaper_list)
{
  إذا (unlikely (!بفر->len))
    أرجع سچ;

  بفر->enter ();

  hb_بفر_t *متن_بفر = خالي_اشارو;
  إذا (بفر->flags & HB_بفر_FLAG_VERIFY)
  {
    متن_بفر = hb_بفر_create ();
    hb_بفر_append (متن_بفر, بفر, 0, -1);
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached2 (font->face, &بفر->props,
							      features, انگ_features,
							      font->coords, font->has_nonzero_coords ? font->انگ_coords : 0,
							      shaper_list);

  hb_bool_t res = hb_shape_plan_execute (shape_plan, font, بفر, features, انگ_features);

  hb_shape_plan_destroy (shape_plan);

  إذا (متن_بفر)
  {
    إذا (res && بفر->successful
	    && متن_بفر->successful
	    && !بفر->verify (متن_بفر,
				font,
				features,
				انگ_features,
				shaper_list))
      res = خطأ;
    hb_بفر_destroy (متن_بفر);
  }

  بفر->leave ();

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
	  hb_بفر_t         *بفر,
	  ثابت hb_feature_t  *features,
	  بنانشان صحيح         انگ_features)
{
  hb_shape_ڀريل (font, بفر, features, انگ_features, خالي_اشارو);
}


#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR

ساكن عائم
بفر_advance (hb_بفر_t *بفر)
{
  عائم a = 0;
  خودڪار *pos = بفر->pos;
  بنانشان ڳڻپ = بفر->len;
  إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      a += pos[i].x_advance;
  وإلا
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      a += pos[i].y_advance;
  أرجع a;
}

ساكن فراغ
reset_بفر (hb_بفر_t *بفر,
	      hb_array_t<ثابت hb_glyph_info_t> متن)
{
  أكد (بفر->ensure (متن.ڊيگهائي));
  بفر->have_positions = خطأ;
  بفر->len = متن.ڊيگهائي;
  hb_memcpy (بفر->info, متن.arrayZ, متن.ڊيگهائي * حجم (بفر->info[0]));
  hb_بفر_سيٽ_content_type (بفر, HB_بفر_CONTENT_TYPE_UNICODE);
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
		  بنانشان صحيح        انگ_features,
		  ثابت حرف * ثابت *shaper_list,
		  عائم               min_هدف_advance,
		  عائم               max_هدف_advance,
		  عائم              *advance, /* IN/OUT */
		  hb_ٽيگ_t           *var_ٽيگ, /*ٻاهريون*/
		  عائم              *var_قدر /*ٻاهريون*/)
{
  // TODO Negative font scales?

  /* If default advance already matches target, nothing to do. Shape and return. */
  إذا (min_هدف_advance <= *advance && *advance <= max_هدف_advance)
  {
    *var_ٽيگ = HB_ٽيگ_NONE;
    *var_قدر = 0.0f;
    أرجع hb_shape_ڀريل (font, بفر,
			  features, انگ_features,
			  shaper_list);
  }

  hb_face_t *face = font->face;

  /* Choose variation tag to use for justification. */

  hb_ٽيگ_t ٽيگ_2 = HB_ٽيگ_NONE;
  hb_ot_var_axis_info_t axis_info;

  hb_ٽيگ_t ٽيگ_فهرست[] =
  {
    HB_ٽيگ ('j','s','t','f'),
    HB_ٽيگ ('w','d','t','h'),
  };
  لكل (بنانشان i = 0; i < ARRAY_ڊيگهائي (ٽيگ_فهرست); i++)
    إذا (hb_ot_var_ڳوليو_axis_info (face, ٽيگ_فهرست[i], &axis_info))
    {
      ٽيگ_2 = *var_ٽيگ = ٽيگ_فهرست[i];
      اكسر;
    }

  /* If no suitable variation axis found, can't justify.  Just shape and return. */
  إذا (!ٽيگ_2)
  {
    *var_ٽيگ = HB_ٽيگ_NONE;
    *var_قدر = 0.0f;
    إذا (hb_shape_ڀريل (font, بفر,
		       features, انگ_features,
		       shaper_list))
    {
      *advance = بفر_advance (بفر);
      أرجع سچ;
    }
    وإلا
      أرجع خطأ;
  }

  /* Copy buffer text as we need it so we can shape multiple times. */
  بنانشان متن_len = بفر->len;
  خودڪار *متن_info = (hb_glyph_info_t *) hb_malloc2 (متن_len, حجم (بفر->info[0]));
  إذا (unlikely (متن_len && !متن_info))
    أرجع خطأ;
  HB_SCOPE_GUARD (hb_free (متن_info));
  hb_memcpy (متن_info, بفر->info, متن_len * حجم (بفر->info[0]));
  خودڪار متن = hb_array<ثابت hb_glyph_info_t> (متن_info, متن_len);

  /* If default advance was not provided to us, calculate it. */
  إذا (!*advance)
  {
    hb_font_سيٽ_variation (font, ٽيگ_2, axis_info.default_قدر);
    إذا (!hb_shape_ڀريل (font, بفر,
			features, انگ_features,
			shaper_list))
      أرجع خطأ;
    *advance = بفر_advance (بفر);
  }

  /* If default advance already matches target, nothing to do. Shape and return.
   * Do this again, in case advance was just calculated.
   */
  إذا (min_هدف_advance <= *advance && *advance <= max_هدف_advance)
  {
    *var_ٽيگ = HB_ٽيگ_NONE;
    *var_قدر = 0.0f;
    أرجع سچ;
  }

  /* Prepare for running the solver. */
  مزدوج a, b, ya, yb;
  إذا (*advance < min_هدف_advance)
  {
    /* Need to expand. */
    ya = (مزدوج) *advance;
    a = (مزدوج) axis_info.default_قدر;
    b = (مزدوج) axis_info.max_قدر;

    /* Shape buffer for maximum expansion to use as other
     * starting point for the solver. */
    hb_font_سيٽ_variation (font, ٽيگ_2, (عائم) b);
    reset_بفر (بفر, متن);
    إذا (!hb_shape_ڀريل (font, بفر,
			features, انگ_features,
			shaper_list))
      أرجع خطأ;
    yb = (مزدوج) بفر_advance (بفر);
    /* If the maximum expansion is less than max target,
     * there's nothing to solve for. Just return it. */
    إذا (yb <= (مزدوج) max_هدف_advance)
    {
      *var_قدر = (عائم) b;
      *advance = (عائم) yb;
      أرجع سچ;
    }
  }
  وإلا
  {
    /* Need to shrink. */
    yb = (مزدوج) *advance;
    a = (مزدوج) axis_info.min_قدر;
    b = (مزدوج) axis_info.default_قدر;

    /* Shape buffer for maximum shrinkate to use as other
     * starting point for the solver. */
    hb_font_سيٽ_variation (font, ٽيگ_2, (عائم) a);
    reset_بفر (بفر, متن);
    إذا (!hb_shape_ڀريل (font, بفر,
			features, انگ_features,
			shaper_list))
      أرجع خطأ;
    ya = (مزدوج) بفر_advance (بفر);
    /* If the maximum shrinkate is more than min target,
     * there's nothing to solve for. Just return it. */
    إذا (ya >= (مزدوج) min_هدف_advance)
    {
      *var_قدر = (عائم) a;
      *advance = (عائم) ya;
      أرجع سچ;
    }
  }

  /* Run the solver to find a var axis value that hits
   * the desired width. */

  مزدوج epsilon = (b - a) / (1<<14);
  منطقي failed = خطأ;

  خودڪار f = [&] (مزدوج x)
  {
    hb_font_سيٽ_variation (font, ٽيگ_2, (عائم) x);
    reset_بفر (بفر, متن);
    إذا (unlikely (!hb_shape_ڀريل (font, بفر,
				  features, انگ_features,
				  shaper_list)))
    {
      failed = سچ;
      أرجع (مزدوج) min_هدف_advance;
    }

    مزدوج w = (مزدوج) بفر_advance (بفر);
    DEBUG_نياپو (JUSTIFY, خالي_اشارو, "Trying '%c%c%c%c' axis parameter %f. Advance %g. Target: min %g max %g",
	       HB_UNTAG (ٽيگ_2), x, w,
	       (مزدوج) min_هدف_advance, (مزدوج) max_هدف_advance);
    أرجع w;
  };

  مزدوج y = 0;
  مزدوج itp = solve_itp (f,
			  a, b,
			  epsilon,
			  (مزدوج) min_هدف_advance, (مزدوج) max_هدف_advance,
			  ya, yb, y);

  إذا (failed)
    أرجع خطأ;

  *var_قدر = (عائم) itp;
  *advance = (عائم) y;

  أرجع سچ;
}
#جيڪڏهن_پورو
#جيڪڏهن_پورو


#جيڪڏهن_پورو
