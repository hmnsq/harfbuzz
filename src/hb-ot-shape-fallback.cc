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

स्थिर अचिन्हित पूर्णाङ्क
recategorize_combining_class (hb_codepoint_t u,
			      अचिन्हित पूर्णाङ्क klass)
{
  यदि (klass >= 200)
    फर्काऊ klass;

  /* Thai / Lao need some per-character work. */
  यदि ((u & ~0xFF) == 0x0E00u)
  {
    यदि (unlikely (klass == 0))
    {
      विकल्प (u)
      {
	पक्ष 0x0E31u:
	पक्ष 0x0E34u:
	पक्ष 0x0E35u:
	पक्ष 0x0E36u:
	पक्ष 0x0E37u:
	पक्ष 0x0E47u:
	पक्ष 0x0E4Cu:
	पक्ष 0x0E4Du:
	पक्ष 0x0E4Eu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_दायाँ;
	  रोक;

	पक्ष 0x0EB1u:
	पक्ष 0x0EB4u:
	पक्ष 0x0EB5u:
	पक्ष 0x0EB6u:
	पक्ष 0x0EB7u:
	पक्ष 0x0EBBu:
	पक्ष 0x0ECCu:
	पक्ष 0x0ECDu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE;
	  रोक;

	पक्ष 0x0EBCu:
	  klass = HB_UNICODE_COMBINING_CLASS_BELOW;
	  रोक;
      }
    } अन्यथा {
      /* Thai virama is below-right */
      यदि (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_BELOW_दायाँ;
    }
  }

  विकल्प (klass)
  {

    /* Hebrew */

    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC10: /* sheva */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC11: /* hataf segol */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC12: /* hataf patah */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC13: /* hataf qamats */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC14: /* hiriq */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC15: /* tsere */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC16: /* segol */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC17: /* patah */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC18: /* qamats & qamats qatan */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC20: /* qubuts */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC22: /* meteg */
      फर्काऊ HB_UNICODE_COMBINING_CLASS_BELOW;

    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      फर्काऊ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      फर्काऊ HB_UNICODE_COMBINING_CLASS_ABOVE_दायाँ;

    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      फर्काऊ HB_UNICODE_COMBINING_CLASS_ABOVE_बायाँ;

    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      फर्काऊ HB_UNICODE_COMBINING_CLASS_ABOVE;

    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC21: /* dagesh */
      रोक;


    /* Arabic and Syriac */

    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC27: /* fathatan */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC28: /* dammatan */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC30: /* fatha */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC31: /* damma */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC33: /* shadda */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC34: /* sukun */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC35: /* superscript alef */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC36: /* superscript alaph */
      फर्काऊ HB_UNICODE_COMBINING_CLASS_ABOVE;

    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      फर्काऊ HB_UNICODE_COMBINING_CLASS_BELOW;


    /* Thai */

    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      फर्काऊ HB_UNICODE_COMBINING_CLASS_BELOW_दायाँ;

    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      फर्काऊ HB_UNICODE_COMBINING_CLASS_ABOVE_दायाँ;


    /* Lao */

    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      फर्काऊ HB_UNICODE_COMBINING_CLASS_BELOW;

    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      फर्काऊ HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      फर्काऊ HB_UNICODE_COMBINING_CLASS_BELOW;

    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      फर्काऊ HB_UNICODE_COMBINING_CLASS_ABOVE;

    पक्ष HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      फर्काऊ HB_UNICODE_COMBINING_CLASS_BELOW;

  }

  फर्काऊ klass;
}

शून्य
_hb_ot_shape_fallback_mark_position_recategorize_marks (अचर hb_ot_shape_plan_t *plan HB_UNUSED,
							hb_font_t *font HB_UNUSED,
							hb_buffer_t  *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  फर्काऊ;
#endif

  अचिन्हित पूर्णाङ्क गन्ती = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    यदि (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      अचिन्हित पूर्णाङ्क combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      _hb_glyph_info_set_modified_combining_class (&info[i], combining_class);
    }
}


स्थिर शून्य
zero_mark_advances (hb_buffer_t *buffer,
		    अचिन्हित पूर्णाङ्क start,
		    अचिन्हित पूर्णाङ्क end,
		    तार्किक adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = buffer->info;
  लागि (अचिन्हित पूर्णाङ्क i = start; i < end; i++)
    यदि (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      यदि (adjust_offsets_when_zeroing)
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

स्थिर अन्तःस्थ शून्य
position_mark (अचर hb_ot_shape_plan_t *plan HB_UNUSED,
	       hb_font_t *font,
	       hb_buffer_t  *buffer,
	       hb_glyph_extents_t &base_extents,
	       अचिन्हित पूर्णाङ्क i,
	       अचिन्हित पूर्णाङ्क combining_class)
{
  hb_glyph_extents_t mark_extents;
  यदि (!font->get_glyph_extents (buffer->info[i].codepoint, &mark_extents))
    फर्काऊ;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = buffer->pos[i];
  pos.x_offset = pos.y_offset = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  विकल्प (combining_class)
  {
    पक्ष HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    पक्ष HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      यदि (buffer->props.direction == HB_DIRECTION_LTR) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	रोक;
      } अन्यथा यदि (buffer->props.direction == HB_DIRECTION_RTL) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	रोक;
      }
      HB_FALLTHROUGH;

    स्वभाव:
    पक्ष HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
    पक्ष HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    पक्ष HB_UNICODE_COMBINING_CLASS_BELOW:
    पक्ष HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing +
						((int64_t) base_extents.width - mark_extents.width) / 2 -
						mark_extents.x_bearing);
      रोक;

    पक्ष HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_बायाँ:
    पक्ष HB_UNICODE_COMBINING_CLASS_BELOW_बायाँ:
    पक्ष HB_UNICODE_COMBINING_CLASS_ABOVE_बायाँ:
      /* Left align. */
      pos.x_offset = hb_saturate_sub (base_extents.x_bearing, mark_extents.x_bearing);
      रोक;

    पक्ष HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_दायाँ:
    पक्ष HB_UNICODE_COMBINING_CLASS_BELOW_दायाँ:
    पक्ष HB_UNICODE_COMBINING_CLASS_ABOVE_दायाँ:
      /* Right align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						mark_extents.width - mark_extents.x_bearing);
      रोक;
  }

  /* Y positioning */
  विकल्प (combining_class)
  {
    पक्ष HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    पक्ष HB_UNICODE_COMBINING_CLASS_BELOW_बायाँ:
    पक्ष HB_UNICODE_COMBINING_CLASS_BELOW:
    पक्ष HB_UNICODE_COMBINING_CLASS_BELOW_दायाँ:
      /* Add gap, fall-through. */
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    पक्ष HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_बायाँ:
    पक्ष HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing +
						base_extents.height - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      यदि ((y_gap > 0) == (pos.y_offset > 0))
      {
	base_extents.height = hb_saturate_sub (base_extents.height, pos.y_offset);
	pos.y_offset = 0;
      }
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      रोक;

    पक्ष HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    पक्ष HB_UNICODE_COMBINING_CLASS_ABOVE_बायाँ:
    पक्ष HB_UNICODE_COMBINING_CLASS_ABOVE:
    पक्ष HB_UNICODE_COMBINING_CLASS_ABOVE_दायाँ:
      /* Add gap, fall-through. */
      base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, y_gap);
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    पक्ष HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    पक्ष HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_दायाँ:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing -
						mark_extents.y_bearing - mark_extents.height);
      /* Don't shift down "above" marks too much. */
      यदि ((y_gap > 0) != (pos.y_offset > 0))
      {
	hb_position_t correction = -(pos.y_offset / 2);
	base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, correction);
	base_extents.height = hb_saturate_sub (base_extents.height, correction);
	pos.y_offset = hb_saturate_add (pos.y_offset, correction);
      }
      base_extents.y_bearing = hb_saturate_sub (base_extents.y_bearing, mark_extents.height);
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      रोक;
  }
}

स्थिर अन्तःस्थ शून्य
position_around_base (अचर hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_buffer_t  *buffer,
		      अचिन्हित पूर्णाङ्क base,
		      अचिन्हित पूर्णाङ्क end,
		      तार्किक adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  buffer->unsafe_to_break (base, end);

  hb_glyph_extents_t base_extents;
  यदि (!font->get_glyph_extents (buffer->info[base].codepoint,
				&base_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (buffer, base + 1, end, adjust_offsets_when_zeroing);
    फर्काऊ;
  }
  base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing,
					    buffer->pos[base].y_offset);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  base_extents.x_bearing = 0;
  base_extents.width = font->get_glyph_h_advance (buffer->info[base].codepoint);

  अचिन्हित पूर्णाङ्क lig_id = _hb_glyph_info_get_lig_id (&buffer->info[base]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  पूर्णाङ्क अङ्क_lig_components = hb_glyph_info_get_lig_अङ्क_comps (&buffer->info[base]);

  hb_position_t x_offset = 0, y_offset = 0;
  यदि (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
    x_offset = hb_saturate_neg (buffer->pos[base].x_advance);
    y_offset = hb_saturate_neg (buffer->pos[base].y_advance);
  }

  hb_glyph_extents_t component_extents = base_extents;
  पूर्णाङ्क last_lig_component = -1;
  अचिन्हित पूर्णाङ्क last_combining_class = 255;
  hb_glyph_extents_t cluster_extents = base_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = buffer->info;
  लागि (अचिन्हित पूर्णाङ्क i = base + 1; i < end; i++)
    यदि (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      यदि (अङ्क_lig_components > 1) {
	अचिन्हित पूर्णाङ्क this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	पूर्णाङ्क this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
	/* Conditions for attaching to the last component. */
	यदि (!lig_id || lig_id != this_lig_id || this_lig_component >= अङ्क_lig_components)
	  this_lig_component = अङ्क_lig_components - 1;
	यदि (last_lig_component != this_lig_component)
	{
	  last_lig_component = this_lig_component;
	  last_combining_class = 255;
	  component_extents = base_extents;
	  यदि (unlikely (horiz_dir == HB_DIRECTION_INVALID)) {
	    यदि (HB_DIRECTION_IS_HORIZONTAL (plan->props.direction))
	      horiz_dir = plan->props.direction;
	    अन्यथा
	      horiz_dir = hb_script_get_horizontal_direction (plan->props.script);
	  }
	  यदि (horiz_dir == HB_DIRECTION_LTR)
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) this_lig_component * component_extents.width / अङ्क_lig_components);
	  अन्यथा
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) (अङ्क_lig_components - 1 - this_lig_component) * component_extents.width / अङ्क_lig_components);
	  component_extents.width /= अङ्क_lig_components;
	}
      }

      अचिन्हित पूर्णाङ्क this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      यदि (last_combining_class != this_combining_class)
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
      यदि (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
	x_offset = hb_saturate_sub (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_sub (y_offset, buffer->pos[i].y_advance);
      } अन्यथा {
	x_offset = hb_saturate_add (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_add (y_offset, buffer->pos[i].y_advance);
      }
    }
}

स्थिर अन्तःस्थ शून्य
position_cluster_impl (अचर hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t  *buffer,
		       अचिन्हित पूर्णाङ्क start,
		       अचिन्हित पूर्णाङ्क end,
		       तार्किक adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = buffer->info;
  लागि (अचिन्हित पूर्णाङ्क i = start; i < end; i++)
    यदि (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      अचिन्हित पूर्णाङ्क j;
      लागि (j = i + 1; j < end; j++)
      {
        यदि (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  जारी;
	यदि (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  रोक;
      }

      position_around_base (plan, font, buffer, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

स्थिर HB_ALWAYS_INLINE शून्य
position_cluster (अचर hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_buffer_t  *buffer,
		  अचिन्हित पूर्णाङ्क start,
		  अचिन्हित पूर्णाङ्क end,
		  तार्किक adjust_offsets_when_zeroing)
{
  यदि (end - start < 2)
    फर्काऊ;

  position_cluster_impl (plan, font, buffer, start, end, adjust_offsets_when_zeroing);
}

शून्य
_hb_ot_shape_fallback_mark_position (अचर hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_buffer_t  *buffer,
				     तार्किक adjust_offsets_when_zeroing)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  फर्काऊ;
#endif

  यदि (!buffer->message (font, "start fallback mark"))
    फर्काऊ;

  _hb_buffer_assert_gsubgpos_vars (buffer);

  अचिन्हित पूर्णाङ्क start = 0;
  अचिन्हित पूर्णाङ्क गन्ती = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  लागि (अचिन्हित पूर्णाङ्क i = 1; i < गन्ती; i++)
    यदि (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, buffer, start, i, adjust_offsets_when_zeroing);
      start = i;
    }
  position_cluster (plan, font, buffer, start, गन्ती, adjust_offsets_when_zeroing);

  (शून्य) buffer->message (font, "end fallback mark");
}


#ifndef HB_DISABLE_DEPRECATED
संरचना hb_ot_shape_fallback_kern_driver_t
{
  hb_ot_shape_fallback_kern_driver_t (hb_font_t   *font_,
				      hb_buffer_t *buffer) :
    font (font_), direction (buffer->props.direction) {}

  hb_position_t get_kerning (hb_codepoint_t first, hb_codepoint_t second) अचर
  {
    hb_position_t kern = 0;
    font->get_glyph_kerning_for_direction (first, second,
					   direction,
					   &kern, &kern);
    फर्काऊ kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#endif

/* Performs font-assisted kerning. */
शून्य
_hb_ot_shape_fallback_kern (अचर hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  फर्काऊ;
#endif

#ifndef HB_DISABLE_DEPRECATED
  यदि (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    फर्काऊ;

  यदि (!buffer->message (font, "start fallback kern"))
    फर्काऊ;

  तार्किक reverse = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  यदि (reverse)
    buffer->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, buffer);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, buffer, plan->kern_mask, असत्य);

  यदि (reverse)
    buffer->reverse ();

  (शून्य) buffer->message (font, "end fallback kern");
#endif
}


/* Adjusts width of various spaces. */
शून्य
_hb_ot_shape_fallback_spaces (अचर hb_ot_shape_plan_t *plan HB_UNUSED,
			      hb_font_t *font,
			      hb_buffer_t  *buffer)
{
  hb_glyph_info_t *info = buffer->info;
  hb_glyph_position_t *pos = buffer->pos;
  तार्किक horizontal = HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction);
  अचिन्हित पूर्णाङ्क गन्ती = buffer->len;
  लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    यदि (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      यदि (buffer->invisible && info[i].codepoint == buffer->invisible)
      {
        यदि (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        अन्यथा
	  pos[i].y_advance = -(int64_t) font->y_scale / 4;
      }

      hb_unicode_funcs_t::space_t space_type = _hb_glyph_info_get_unicode_space_fallback_type (&info[i]);
      hb_codepoint_t glyph;
      प्रकारनिर्देश hb_unicode_funcs_t t;
      विकल्प (space_type)
      {
	पक्ष t::NOT_SPACE: /* Shouldn't happen. */
	पक्ष t::SPACE:
	  रोक;

	पक्ष t::SPACE_EM:
	पक्ष t::SPACE_EM_2:
	पक्ष t::SPACE_EM_3:
	पक्ष t::SPACE_EM_4:
	पक्ष t::SPACE_EM_5:
	पक्ष t::SPACE_EM_6:
	पक्ष t::SPACE_EM_16:
	  यदि (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((पूर्णाङ्क) space_type)/2) / (पूर्णाङ्क) space_type;
	  अन्यथा
	    pos[i].y_advance = -((int64_t) font->y_scale + ((पूर्णाङ्क) space_type)/2) / (पूर्णाङ्क) space_type;
	  रोक;

	पक्ष t::SPACE_4_EM_18:
	  यदि (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  अन्यथा
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  रोक;

	पक्ष t::SPACE_FIGURE:
	  लागि (वर्ण u = '0'; u <= '9'; u++)
	    यदि (font->get_nominal_glyph (u, &glyph))
	    {
	      यदि (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      अन्यथा
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      रोक;
	    }
	  रोक;

	पक्ष t::SPACE_PUNCTUATION:
	  यदि (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    यदि (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    अन्यथा
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  रोक;

	पक्ष t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  यदि (horizontal)
	    pos[i].x_advance /= 2;
	  अन्यथा
	    pos[i].y_advance /= 2;
	  रोक;
      }
    }
}


#endif
