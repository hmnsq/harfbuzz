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

#അന്തര്ഭാവ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#അന്തര്ഭാവ "hb-ot-shape-fallback.hh"
#അന്തര്ഭാവ "hb-kern.hh"

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
recategorize_combining_class (hb_codepoint_t u,
			      unsigned പൂർണ്ണസംഖ്യ klass)
{
  എങ്കിൽ (klass >= 200)
    തിരികെ klass;

  /* Thai / Lao need some per-character work. */
  എങ്കിൽ ((u & ~0xFF) == 0x0E00u)
  {
    എങ്കിൽ (unlikely (klass == 0))
    {
      തിരഞ്ഞെടുപ്പ് (u)
      {
	അവസ്ഥ 0x0E31u:
	അവസ്ഥ 0x0E34u:
	അവസ്ഥ 0x0E35u:
	അവസ്ഥ 0x0E36u:
	അവസ്ഥ 0x0E37u:
	അവസ്ഥ 0x0E47u:
	അവസ്ഥ 0x0E4Cu:
	അവസ്ഥ 0x0E4Du:
	അവസ്ഥ 0x0E4Eu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_വലത്;
	  നിർത്തുക;

	അവസ്ഥ 0x0EB1u:
	അവസ്ഥ 0x0EB4u:
	അവസ്ഥ 0x0EB5u:
	അവസ്ഥ 0x0EB6u:
	അവസ്ഥ 0x0EB7u:
	അവസ്ഥ 0x0EBBu:
	അവസ്ഥ 0x0ECCu:
	അവസ്ഥ 0x0ECDu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE;
	  നിർത്തുക;

	അവസ്ഥ 0x0EBCu:
	  klass = HB_UNICODE_COMBINING_CLASS_BELOW;
	  നിർത്തുക;
      }
    } അല്ലെങ്കിൽ {
      /* Thai virama is below-right */
      എങ്കിൽ (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_BELOW_വലത്;
    }
  }

  തിരഞ്ഞെടുപ്പ് (klass)
  {

    /* Hebrew */

    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC10: /* sheva */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC11: /* hataf segol */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC12: /* hataf patah */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC13: /* hataf qamats */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC14: /* hiriq */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC15: /* tsere */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC16: /* segol */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC17: /* patah */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC18: /* qamats & qamats qatan */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC20: /* qubuts */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC22: /* meteg */
      തിരികെ HB_UNICODE_COMBINING_CLASS_BELOW;

    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      തിരികെ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      തിരികെ HB_UNICODE_COMBINING_CLASS_ABOVE_വലത്;

    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      തിരികെ HB_UNICODE_COMBINING_CLASS_ABOVE_ഇടത്;

    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      തിരികെ HB_UNICODE_COMBINING_CLASS_ABOVE;

    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC21: /* dagesh */
      നിർത്തുക;


    /* Arabic and Syriac */

    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC27: /* fathatan */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC28: /* dammatan */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC30: /* fatha */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC31: /* damma */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC33: /* shadda */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC34: /* sukun */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC35: /* superscript alef */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC36: /* superscript alaph */
      തിരികെ HB_UNICODE_COMBINING_CLASS_ABOVE;

    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      തിരികെ HB_UNICODE_COMBINING_CLASS_BELOW;


    /* Thai */

    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      തിരികെ HB_UNICODE_COMBINING_CLASS_BELOW_വലത്;

    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      തിരികെ HB_UNICODE_COMBINING_CLASS_ABOVE_വലത്;


    /* Lao */

    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      തിരികെ HB_UNICODE_COMBINING_CLASS_BELOW;

    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      തിരികെ HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      തിരികെ HB_UNICODE_COMBINING_CLASS_BELOW;

    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      തിരികെ HB_UNICODE_COMBINING_CLASS_ABOVE;

    അവസ്ഥ HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      തിരികെ HB_UNICODE_COMBINING_CLASS_BELOW;

  }

  തിരികെ klass;
}

ശൂന്യം
_hb_ot_shape_fallback_mark_position_recategorize_marks (സ്ഥിരം hb_ot_shape_plan_t *plan HB_UNUSED,
							hb_font_t *font HB_UNUSED,
							hb_buffer_t  *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  തിരികെ;
#endif

  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    എങ്കിൽ (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      unsigned പൂർണ്ണസംഖ്യ combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      _hb_glyph_info_set_modified_combining_class (&info[i], combining_class);
    }
}


സ്ഥിതം ശൂന്യം
zero_mark_advances (hb_buffer_t *buffer,
		    unsigned പൂർണ്ണസംഖ്യ start,
		    unsigned പൂർണ്ണസംഖ്യ end,
		    തർക്കം adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = buffer->info;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = start; i < end; i++)
    എങ്കിൽ (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      എങ്കിൽ (adjust_offsets_when_zeroing)
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

സ്ഥിതം വരിയിൽ ശൂന്യം
position_mark (സ്ഥിരം hb_ot_shape_plan_t *plan HB_UNUSED,
	       hb_font_t *font,
	       hb_buffer_t  *buffer,
	       hb_glyph_extents_t &base_extents,
	       unsigned പൂർണ്ണസംഖ്യ i,
	       unsigned പൂർണ്ണസംഖ്യ combining_class)
{
  hb_glyph_extents_t mark_extents;
  എങ്കിൽ (!font->get_glyph_extents (buffer->info[i].codepoint, &mark_extents))
    തിരികെ;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = buffer->pos[i];
  pos.x_offset = pos.y_offset = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  തിരഞ്ഞെടുപ്പ് (combining_class)
  {
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      എങ്കിൽ (buffer->props.direction == HB_DIRECTION_LTR) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	നിർത്തുക;
      } അല്ലെങ്കിൽ എങ്കിൽ (buffer->props.direction == HB_DIRECTION_RTL) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	നിർത്തുക;
      }
      HB_FALLTHROUGH;

    സ്ഥിരസ്ഥിതി:
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_BELOW:
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing +
						((int64_t) base_extents.width - mark_extents.width) / 2 -
						mark_extents.x_bearing);
      നിർത്തുക;

    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_ഇടത്:
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_BELOW_ഇടത്:
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_ABOVE_ഇടത്:
      /* Left align. */
      pos.x_offset = hb_saturate_sub (base_extents.x_bearing, mark_extents.x_bearing);
      നിർത്തുക;

    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_വലത്:
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_BELOW_വലത്:
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_ABOVE_വലത്:
      /* Right align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						mark_extents.width - mark_extents.x_bearing);
      നിർത്തുക;
  }

  /* Y positioning */
  തിരഞ്ഞെടുപ്പ് (combining_class)
  {
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_BELOW_ഇടത്:
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_BELOW:
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_BELOW_വലത്:
      /* Add gap, fall-through. */
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_ഇടത്:
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing +
						base_extents.height - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      എങ്കിൽ ((y_gap > 0) == (pos.y_offset > 0))
      {
	base_extents.height = hb_saturate_sub (base_extents.height, pos.y_offset);
	pos.y_offset = 0;
      }
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      നിർത്തുക;

    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_ABOVE_ഇടത്:
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_ABOVE:
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_ABOVE_വലത്:
      /* Add gap, fall-through. */
      base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, y_gap);
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    അവസ്ഥ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_വലത്:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing -
						mark_extents.y_bearing - mark_extents.height);
      /* Don't shift down "above" marks too much. */
      എങ്കിൽ ((y_gap > 0) != (pos.y_offset > 0))
      {
	hb_position_t correction = -(pos.y_offset / 2);
	base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, correction);
	base_extents.height = hb_saturate_sub (base_extents.height, correction);
	pos.y_offset = hb_saturate_add (pos.y_offset, correction);
      }
      base_extents.y_bearing = hb_saturate_sub (base_extents.y_bearing, mark_extents.height);
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      നിർത്തുക;
  }
}

സ്ഥിതം വരിയിൽ ശൂന്യം
position_around_base (സ്ഥിരം hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_buffer_t  *buffer,
		      unsigned പൂർണ്ണസംഖ്യ base,
		      unsigned പൂർണ്ണസംഖ്യ end,
		      തർക്കം adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  buffer->unsafe_to_break (base, end);

  hb_glyph_extents_t base_extents;
  എങ്കിൽ (!font->get_glyph_extents (buffer->info[base].codepoint,
				&base_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (buffer, base + 1, end, adjust_offsets_when_zeroing);
    തിരികെ;
  }
  base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing,
					    buffer->pos[base].y_offset);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  base_extents.x_bearing = 0;
  base_extents.width = font->get_glyph_h_advance (buffer->info[base].codepoint);

  unsigned പൂർണ്ണസംഖ്യ lig_id = _hb_glyph_info_get_lig_id (&buffer->info[base]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  പൂർണ്ണസംഖ്യ അക്കം_lig_components = hb_glyph_info_get_lig_അക്കം_comps (&buffer->info[base]);

  hb_position_t x_offset = 0, y_offset = 0;
  എങ്കിൽ (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
    x_offset = hb_saturate_neg (buffer->pos[base].x_advance);
    y_offset = hb_saturate_neg (buffer->pos[base].y_advance);
  }

  hb_glyph_extents_t component_extents = base_extents;
  പൂർണ്ണസംഖ്യ last_lig_component = -1;
  unsigned പൂർണ്ണസംഖ്യ last_combining_class = 255;
  hb_glyph_extents_t cluster_extents = base_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = buffer->info;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = base + 1; i < end; i++)
    എങ്കിൽ (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      എങ്കിൽ (അക്കം_lig_components > 1) {
	unsigned പൂർണ്ണസംഖ്യ this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	പൂർണ്ണസംഖ്യ this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
	/* Conditions for attaching to the last component. */
	എങ്കിൽ (!lig_id || lig_id != this_lig_id || this_lig_component >= അക്കം_lig_components)
	  this_lig_component = അക്കം_lig_components - 1;
	എങ്കിൽ (last_lig_component != this_lig_component)
	{
	  last_lig_component = this_lig_component;
	  last_combining_class = 255;
	  component_extents = base_extents;
	  എങ്കിൽ (unlikely (horiz_dir == HB_DIRECTION_INVALID)) {
	    എങ്കിൽ (HB_DIRECTION_IS_HORIZONTAL (plan->props.direction))
	      horiz_dir = plan->props.direction;
	    അല്ലെങ്കിൽ
	      horiz_dir = hb_script_get_horizontal_direction (plan->props.script);
	  }
	  എങ്കിൽ (horiz_dir == HB_DIRECTION_LTR)
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) this_lig_component * component_extents.width / അക്കം_lig_components);
	  അല്ലെങ്കിൽ
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) (അക്കം_lig_components - 1 - this_lig_component) * component_extents.width / അക്കം_lig_components);
	  component_extents.width /= അക്കം_lig_components;
	}
      }

      unsigned പൂർണ്ണസംഖ്യ this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      എങ്കിൽ (last_combining_class != this_combining_class)
      {
	last_combining_class = this_combining_class;
	cluster_extents = component_extents;
      }

      position_mark (plan, font, buffer, cluster_extents, i, this_combining_class);

      buffer->pos[i].x_advance = 0;
      buffer->pos[i].y_advance = 0;
      buffer->pos[i].x_offset = hb_saturate_add (buffer->pos[i].x_offset, x_offset);
      buffer->pos[i].y_offset = hb_saturate_add (buffer->pos[i].y_offset, y_offset);

    } അല്ലെങ്കിൽ {
      എങ്കിൽ (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
	x_offset = hb_saturate_sub (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_sub (y_offset, buffer->pos[i].y_advance);
      } അല്ലെങ്കിൽ {
	x_offset = hb_saturate_add (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_add (y_offset, buffer->pos[i].y_advance);
      }
    }
}

സ്ഥിതം വരിയിൽ ശൂന്യം
position_cluster_impl (സ്ഥിരം hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t  *buffer,
		       unsigned പൂർണ്ണസംഖ്യ start,
		       unsigned പൂർണ്ണസംഖ്യ end,
		       തർക്കം adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = buffer->info;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = start; i < end; i++)
    എങ്കിൽ (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      unsigned പൂർണ്ണസംഖ്യ j;
      വേണ്ടി (j = i + 1; j < end; j++)
      {
        എങ്കിൽ (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  തുടരുക;
	എങ്കിൽ (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  നിർത്തുക;
      }

      position_around_base (plan, font, buffer, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

സ്ഥിതം HB_ALWAYS_INLINE ശൂന്യം
position_cluster (സ്ഥിരം hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_buffer_t  *buffer,
		  unsigned പൂർണ്ണസംഖ്യ start,
		  unsigned പൂർണ്ണസംഖ്യ end,
		  തർക്കം adjust_offsets_when_zeroing)
{
  എങ്കിൽ (end - start < 2)
    തിരികെ;

  position_cluster_impl (plan, font, buffer, start, end, adjust_offsets_when_zeroing);
}

ശൂന്യം
_hb_ot_shape_fallback_mark_position (സ്ഥിരം hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_buffer_t  *buffer,
				     തർക്കം adjust_offsets_when_zeroing)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  തിരികെ;
#endif

  എങ്കിൽ (!buffer->message (font, "start fallback mark"))
    തിരികെ;

  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned പൂർണ്ണസംഖ്യ start = 0;
  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < എണ്ണം; i++)
    എങ്കിൽ (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, buffer, start, i, adjust_offsets_when_zeroing);
      start = i;
    }
  position_cluster (plan, font, buffer, start, എണ്ണം, adjust_offsets_when_zeroing);

  (ശൂന്യം) buffer->message (font, "end fallback mark");
}


#ifndef HB_DISABLE_DEPRECATED
ഘടന hb_ot_shape_fallback_kern_driver_t
{
  hb_ot_shape_fallback_kern_driver_t (hb_font_t   *font_,
				      hb_buffer_t *buffer) :
    font (font_), direction (buffer->props.direction) {}

  hb_position_t get_kerning (hb_codepoint_t first, hb_codepoint_t second) സ്ഥിരം
  {
    hb_position_t kern = 0;
    font->get_glyph_kerning_for_direction (first, second,
					   direction,
					   &kern, &kern);
    തിരികെ kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#endif

/* Performs font-assisted kerning. */
ശൂന്യം
_hb_ot_shape_fallback_kern (സ്ഥിരം hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  തിരികെ;
#endif

#ifndef HB_DISABLE_DEPRECATED
  എങ്കിൽ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    തിരികെ;

  എങ്കിൽ (!buffer->message (font, "start fallback kern"))
    തിരികെ;

  തർക്കം reverse = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  എങ്കിൽ (reverse)
    buffer->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, buffer);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, buffer, plan->kern_mask, അസത്യം);

  എങ്കിൽ (reverse)
    buffer->reverse ();

  (ശൂന്യം) buffer->message (font, "end fallback kern");
#endif
}


/* Adjusts width of various spaces. */
ശൂന്യം
_hb_ot_shape_fallback_spaces (സ്ഥിരം hb_ot_shape_plan_t *plan HB_UNUSED,
			      hb_font_t *font,
			      hb_buffer_t  *buffer)
{
  hb_glyph_info_t *info = buffer->info;
  hb_glyph_position_t *pos = buffer->pos;
  തർക്കം horizontal = HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction);
  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    എങ്കിൽ (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      എങ്കിൽ (buffer->invisible && info[i].codepoint == buffer->invisible)
      {
        എങ്കിൽ (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        അല്ലെങ്കിൽ
	  pos[i].y_advance = -(int64_t) font->y_scale / 4;
      }

      hb_unicode_funcs_t::space_t space_type = _hb_glyph_info_get_unicode_space_fallback_type (&info[i]);
      hb_codepoint_t glyph;
      തരനിർവചനം hb_unicode_funcs_t t;
      തിരഞ്ഞെടുപ്പ് (space_type)
      {
	അവസ്ഥ t::NOT_SPACE: /* Shouldn't happen. */
	അവസ്ഥ t::SPACE:
	  നിർത്തുക;

	അവസ്ഥ t::SPACE_EM:
	അവസ്ഥ t::SPACE_EM_2:
	അവസ്ഥ t::SPACE_EM_3:
	അവസ്ഥ t::SPACE_EM_4:
	അവസ്ഥ t::SPACE_EM_5:
	അവസ്ഥ t::SPACE_EM_6:
	അവസ്ഥ t::SPACE_EM_16:
	  എങ്കിൽ (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((പൂർണ്ണസംഖ്യ) space_type)/2) / (പൂർണ്ണസംഖ്യ) space_type;
	  അല്ലെങ്കിൽ
	    pos[i].y_advance = -((int64_t) font->y_scale + ((പൂർണ്ണസംഖ്യ) space_type)/2) / (പൂർണ്ണസംഖ്യ) space_type;
	  നിർത്തുക;

	അവസ്ഥ t::SPACE_4_EM_18:
	  എങ്കിൽ (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  അല്ലെങ്കിൽ
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  നിർത്തുക;

	അവസ്ഥ t::SPACE_FIGURE:
	  വേണ്ടി (അക്ഷരം u = '0'; u <= '9'; u++)
	    എങ്കിൽ (font->get_nominal_glyph (u, &glyph))
	    {
	      എങ്കിൽ (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      അല്ലെങ്കിൽ
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      നിർത്തുക;
	    }
	  നിർത്തുക;

	അവസ്ഥ t::SPACE_PUNCTUATION:
	  എങ്കിൽ (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    എങ്കിൽ (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    അല്ലെങ്കിൽ
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  നിർത്തുക;

	അവസ്ഥ t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  എങ്കിൽ (horizontal)
	    pos[i].x_advance /= 2;
	  അല്ലെങ്കിൽ
	    pos[i].y_advance /= 2;
	  നിർത്തുക;
      }
    }
}


#endif
