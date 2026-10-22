/*
 * Copyright © 2011,2012  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#अन्तर्भाव "hb.hh"

#ifndef HB_NO_OT_SHAPE

#अन्तर्भाव "hb-ot-shape-fallback.hh"
#अन्तर्भाव "hb-kern.hh"

स्थिर अचिन्हित पूर्णांक
recategorize_combining_class (hb_codepoint_t u,
			      अचिन्हित पूर्णांक klass)
{
  जर (klass >= 200)
    परत klass;

  /* Thai / Lao need some per-character work. */
  जर ((u & ~0xFF) == 0x0E00u)
  {
    जर (unlikely (klass == 0))
    {
      निवड (u)
      {
	स्थिती 0x0E31u:
	स्थिती 0x0E34u:
	स्थिती 0x0E35u:
	स्थिती 0x0E36u:
	स्थिती 0x0E37u:
	स्थिती 0x0E47u:
	स्थिती 0x0E4Cu:
	स्थिती 0x0E4Du:
	स्थिती 0x0E4Eu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_उजवा;
	  थांबा;

	स्थिती 0x0EB1u:
	स्थिती 0x0EB4u:
	स्थिती 0x0EB5u:
	स्थिती 0x0EB6u:
	स्थिती 0x0EB7u:
	स्थिती 0x0EBBu:
	स्थिती 0x0ECCu:
	स्थिती 0x0ECDu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE;
	  थांबा;

	स्थिती 0x0EBCu:
	  klass = HB_UNICODE_COMBINING_CLASS_BELOW;
	  थांबा;
      }
    } अन्यथा {
      /* Thai virama is below-right */
      जर (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_BELOW_उजवा;
    }
  }

  निवड (klass)
  {

    /* Hebrew */

    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC10: /* sheva */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC11: /* hataf segol */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC12: /* hataf patah */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC13: /* hataf qamats */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC14: /* hiriq */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC15: /* tsere */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC16: /* segol */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC17: /* patah */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC18: /* qamats & qamats qatan */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC20: /* qubuts */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC22: /* meteg */
      परत HB_UNICODE_COMBINING_CLASS_BELOW;

    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      परत HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      परत HB_UNICODE_COMBINING_CLASS_ABOVE_उजवा;

    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      परत HB_UNICODE_COMBINING_CLASS_ABOVE_डावा;

    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      परत HB_UNICODE_COMBINING_CLASS_ABOVE;

    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC21: /* dagesh */
      थांबा;


    /* Arabic and Syriac */

    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC27: /* fathatan */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC28: /* dammatan */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC30: /* fatha */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC31: /* damma */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC33: /* shadda */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC34: /* sukun */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC35: /* superscript alef */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC36: /* superscript alaph */
      परत HB_UNICODE_COMBINING_CLASS_ABOVE;

    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      परत HB_UNICODE_COMBINING_CLASS_BELOW;


    /* Thai */

    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      परत HB_UNICODE_COMBINING_CLASS_BELOW_उजवा;

    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      परत HB_UNICODE_COMBINING_CLASS_ABOVE_उजवा;


    /* Lao */

    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      परत HB_UNICODE_COMBINING_CLASS_BELOW;

    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      परत HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      परत HB_UNICODE_COMBINING_CLASS_BELOW;

    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      परत HB_UNICODE_COMBINING_CLASS_ABOVE;

    स्थिती HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      परत HB_UNICODE_COMBINING_CLASS_BELOW;

  }

  परत klass;
}

शून्य
_hb_ot_shape_fallback_mark_position_recategorize_marks (स्थिरांक hb_ot_shape_plan_t *plan HB_UNUSED,
							hb_font_t *font HB_UNUSED,
							hb_buffer_t  *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  परत;
#endif

  अचिन्हित पूर्णांक मोजणी = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    जर (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      अचिन्हित पूर्णांक combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      _hb_glyph_info_set_modified_combining_class (&info[i], combining_class);
    }
}


स्थिर शून्य
zero_mark_advances (hb_buffer_t *buffer,
		    अचिन्हित पूर्णांक start,
		    अचिन्हित पूर्णांक end,
		    तार्किक adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = buffer->info;
  साठी (अचिन्हित पूर्णांक i = start; i < end; i++)
    जर (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      जर (adjust_offsets_when_zeroing)
      {
	buffer->pos[i].x_offset = hb_saturate_sub (buffer->pos[i].x_offset,
						     buffer->pos[i].x_advance);
	buffer->pos[i].y_offset = hb_saturate_sub (buffer->pos[i].y_offset,
						     buffer->pos[i].y_advance);
      }
      buffer->pos[i].x_advance = 0;
      buffer->pos[i].y_advance = 0;
    }
}

स्थिर ओळीत शून्य
position_mark (स्थिरांक hb_ot_shape_plan_t *plan HB_UNUSED,
	       hb_font_t *font,
	       hb_buffer_t  *buffer,
	       hb_glyph_extents_t &base_extents,
	       अचिन्हित पूर्णांक i,
	       अचिन्हित पूर्णांक combining_class)
{
  hb_glyph_extents_t mark_extents;
  जर (!font->get_glyph_extents (buffer->info[i].codepoint, &mark_extents))
    परत;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = buffer->pos[i];
  pos.x_offset = pos.y_offset = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  निवड (combining_class)
  {
    स्थिती HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    स्थिती HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      जर (buffer->props.direction == HB_DIRECTION_LTR) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	थांबा;
      } अन्यथा जर (buffer->props.direction == HB_DIRECTION_RTL) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	थांबा;
      }
      HB_FALLTHROUGH;

    पूर्वनिर्धारित:
    स्थिती HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
    स्थिती HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    स्थिती HB_UNICODE_COMBINING_CLASS_BELOW:
    स्थिती HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing +
						((int64_t) base_extents.width - mark_extents.width) / 2 -
						mark_extents.x_bearing);
      थांबा;

    स्थिती HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_डावा:
    स्थिती HB_UNICODE_COMBINING_CLASS_BELOW_डावा:
    स्थिती HB_UNICODE_COMBINING_CLASS_ABOVE_डावा:
      /* Left align. */
      pos.x_offset = hb_saturate_sub (base_extents.x_bearing, mark_extents.x_bearing);
      थांबा;

    स्थिती HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_उजवा:
    स्थिती HB_UNICODE_COMBINING_CLASS_BELOW_उजवा:
    स्थिती HB_UNICODE_COMBINING_CLASS_ABOVE_उजवा:
      /* Right align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						mark_extents.width - mark_extents.x_bearing);
      थांबा;
  }

  /* Y positioning */
  निवड (combining_class)
  {
    स्थिती HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    स्थिती HB_UNICODE_COMBINING_CLASS_BELOW_डावा:
    स्थिती HB_UNICODE_COMBINING_CLASS_BELOW:
    स्थिती HB_UNICODE_COMBINING_CLASS_BELOW_उजवा:
      /* Add gap, fall-through. */
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    स्थिती HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_डावा:
    स्थिती HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing +
						base_extents.height - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      जर ((y_gap > 0) == (pos.y_offset > 0))
      {
	base_extents.height = hb_saturate_sub (base_extents.height, pos.y_offset);
	pos.y_offset = 0;
      }
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      थांबा;

    स्थिती HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    स्थिती HB_UNICODE_COMBINING_CLASS_ABOVE_डावा:
    स्थिती HB_UNICODE_COMBINING_CLASS_ABOVE:
    स्थिती HB_UNICODE_COMBINING_CLASS_ABOVE_उजवा:
      /* Add gap, fall-through. */
      base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, y_gap);
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    स्थिती HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    स्थिती HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_उजवा:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing -
						mark_extents.y_bearing - mark_extents.height);
      /* Don't shift down "above" marks too much. */
      जर ((y_gap > 0) != (pos.y_offset > 0))
      {
	hb_position_t correction = -(pos.y_offset / 2);
	base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, correction);
	base_extents.height = hb_saturate_sub (base_extents.height, correction);
	pos.y_offset = hb_saturate_add (pos.y_offset, correction);
      }
      base_extents.y_bearing = hb_saturate_sub (base_extents.y_bearing, mark_extents.height);
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      थांबा;
  }
}

स्थिर ओळीत शून्य
position_around_base (स्थिरांक hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_buffer_t  *buffer,
		      अचिन्हित पूर्णांक base,
		      अचिन्हित पूर्णांक end,
		      तार्किक adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  buffer->unsafe_to_break (base, end);

  hb_glyph_extents_t base_extents;
  जर (!font->get_glyph_extents (buffer->info[base].codepoint,
				&base_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (buffer, base + 1, end, adjust_offsets_when_zeroing);
    परत;
  }
  base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing,
					    buffer->pos[base].y_offset);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  base_extents.x_bearing = 0;
  base_extents.width = font->get_glyph_h_advance (buffer->info[base].codepoint);

  अचिन्हित पूर्णांक lig_id = _hb_glyph_info_get_lig_id (&buffer->info[base]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  पूर्णांक अंक_lig_components = hb_glyph_info_get_lig_अंक_comps (&buffer->info[base]);

  hb_position_t x_offset = 0, y_offset = 0;
  जर (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
    x_offset = hb_saturate_neg (buffer->pos[base].x_advance);
    y_offset = hb_saturate_neg (buffer->pos[base].y_advance);
  }

  hb_glyph_extents_t component_extents = base_extents;
  पूर्णांक last_lig_component = -1;
  अचिन्हित पूर्णांक last_combining_class = 255;
  hb_glyph_extents_t cluster_extents = base_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = buffer->info;
  साठी (अचिन्हित पूर्णांक i = base + 1; i < end; i++)
    जर (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      जर (अंक_lig_components > 1) {
	अचिन्हित पूर्णांक this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	पूर्णांक this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
	/* Conditions for attaching to the last component. */
	जर (!lig_id || lig_id != this_lig_id || this_lig_component >= अंक_lig_components)
	  this_lig_component = अंक_lig_components - 1;
	जर (last_lig_component != this_lig_component)
	{
	  last_lig_component = this_lig_component;
	  last_combining_class = 255;
	  component_extents = base_extents;
	  जर (unlikely (horiz_dir == HB_DIRECTION_INVALID)) {
	    जर (HB_DIRECTION_IS_HORIZONTAL (plan->props.direction))
	      horiz_dir = plan->props.direction;
	    अन्यथा
	      horiz_dir = hb_script_get_horizontal_direction (plan->props.script);
	  }
	  जर (horiz_dir == HB_DIRECTION_LTR)
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) this_lig_component * component_extents.width / अंक_lig_components);
	  अन्यथा
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) (अंक_lig_components - 1 - this_lig_component) * component_extents.width / अंक_lig_components);
	  component_extents.width /= अंक_lig_components;
	}
      }

      अचिन्हित पूर्णांक this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      जर (last_combining_class != this_combining_class)
      {
	last_combining_class = this_combining_class;
	cluster_extents = component_extents;
      }

      position_mark (plan, font, buffer, cluster_extents, i, this_combining_class);

      buffer->pos[i].x_advance = 0;
      buffer->pos[i].y_advance = 0;
      buffer->pos[i].x_offset = hb_saturate_add (buffer->pos[i].x_offset, x_offset);
      buffer->pos[i].y_offset = hb_saturate_add (buffer->pos[i].y_offset, y_offset);

    } अन्यथा {
      जर (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
	x_offset = hb_saturate_sub (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_sub (y_offset, buffer->pos[i].y_advance);
      } अन्यथा {
	x_offset = hb_saturate_add (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_add (y_offset, buffer->pos[i].y_advance);
      }
    }
}

स्थिर ओळीत शून्य
position_cluster_impl (स्थिरांक hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t  *buffer,
		       अचिन्हित पूर्णांक start,
		       अचिन्हित पूर्णांक end,
		       तार्किक adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = buffer->info;
  साठी (अचिन्हित पूर्णांक i = start; i < end; i++)
    जर (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      अचिन्हित पूर्णांक j;
      साठी (j = i + 1; j < end; j++)
      {
        जर (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  सुरूठेवा;
	जर (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  थांबा;
      }

      position_around_base (plan, font, buffer, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

स्थिर HB_ALWAYS_INLINE शून्य
position_cluster (स्थिरांक hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_buffer_t  *buffer,
		  अचिन्हित पूर्णांक start,
		  अचिन्हित पूर्णांक end,
		  तार्किक adjust_offsets_when_zeroing)
{
  जर (end - start < 2)
    परत;

  position_cluster_impl (plan, font, buffer, start, end, adjust_offsets_when_zeroing);
}

शून्य
_hb_ot_shape_fallback_mark_position (स्थिरांक hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_buffer_t  *buffer,
				     तार्किक adjust_offsets_when_zeroing)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  परत;
#endif

  जर (!buffer->message (font, "start fallback mark"))
    परत;

  _hb_buffer_assert_gsubgpos_vars (buffer);

  अचिन्हित पूर्णांक start = 0;
  अचिन्हित पूर्णांक मोजणी = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  साठी (अचिन्हित पूर्णांक i = 1; i < मोजणी; i++)
    जर (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, buffer, start, i, adjust_offsets_when_zeroing);
      start = i;
    }
  position_cluster (plan, font, buffer, start, मोजणी, adjust_offsets_when_zeroing);

  (शून्य) buffer->message (font, "end fallback mark");
}


#ifndef HB_DISABLE_DEPRECATED
संरचना hb_ot_shape_fallback_kern_driver_t
{
  hb_ot_shape_fallback_kern_driver_t (hb_font_t   *font_,
				      hb_buffer_t *buffer) :
    font (font_), direction (buffer->props.direction) {}

  hb_position_t get_kerning (hb_codepoint_t first, hb_codepoint_t second) स्थिरांक
  {
    hb_position_t kern = 0;
    font->get_glyph_kerning_for_direction (first, second,
					   direction,
					   &kern, &kern);
    परत kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#endif

/* Performs font-assisted kerning. */
शून्य
_hb_ot_shape_fallback_kern (स्थिरांक hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  परत;
#endif

#ifndef HB_DISABLE_DEPRECATED
  जर (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    परत;

  जर (!buffer->message (font, "start fallback kern"))
    परत;

  तार्किक reverse = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  जर (reverse)
    buffer->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, buffer);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, buffer, plan->kern_mask, असत्य);

  जर (reverse)
    buffer->reverse ();

  (शून्य) buffer->message (font, "end fallback kern");
#endif
}


/* Adjusts width of various spaces. */
शून्य
_hb_ot_shape_fallback_spaces (स्थिरांक hb_ot_shape_plan_t *plan HB_UNUSED,
			      hb_font_t *font,
			      hb_buffer_t  *buffer)
{
  hb_glyph_info_t *info = buffer->info;
  hb_glyph_position_t *pos = buffer->pos;
  तार्किक horizontal = HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction);
  अचिन्हित पूर्णांक मोजणी = buffer->len;
  साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    जर (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      जर (buffer->invisible && info[i].codepoint == buffer->invisible)
      {
        जर (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        अन्यथा
	  pos[i].y_advance = -(int64_t) font->y_scale / 4;
      }

      hb_unicode_funcs_t::space_t space_type = _hb_glyph_info_get_unicode_space_fallback_type (&info[i]);
      hb_codepoint_t glyph;
      प्रकारव्याख्या hb_unicode_funcs_t t;
      निवड (space_type)
      {
	स्थिती t::NOT_SPACE: /* Shouldn't happen. */
	स्थिती t::SPACE:
	  थांबा;

	स्थिती t::SPACE_EM:
	स्थिती t::SPACE_EM_2:
	स्थिती t::SPACE_EM_3:
	स्थिती t::SPACE_EM_4:
	स्थिती t::SPACE_EM_5:
	स्थिती t::SPACE_EM_6:
	स्थिती t::SPACE_EM_16:
	  जर (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((पूर्णांक) space_type)/2) / (पूर्णांक) space_type;
	  अन्यथा
	    pos[i].y_advance = -((int64_t) font->y_scale + ((पूर्णांक) space_type)/2) / (पूर्णांक) space_type;
	  थांबा;

	स्थिती t::SPACE_4_EM_18:
	  जर (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  अन्यथा
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  थांबा;

	स्थिती t::SPACE_FIGURE:
	  साठी (अक्षर u = '0'; u <= '9'; u++)
	    जर (font->get_nominal_glyph (u, &glyph))
	    {
	      जर (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      अन्यथा
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      थांबा;
	    }
	  थांबा;

	स्थिती t::SPACE_PUNCTUATION:
	  जर (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    जर (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    अन्यथा
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  थांबा;

	स्थिती t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  जर (horizontal)
	    pos[i].x_advance /= 2;
	  अन्यथा
	    pos[i].y_advance /= 2;
	  थांबा;
      }
    }
}


#endif
