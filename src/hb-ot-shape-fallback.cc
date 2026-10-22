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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#ಅನ್ತರ್ಭಾವ "hb-ot-shape-fallback.hh"
#ಅನ್ತರ್ಭಾವ "hb-kern.hh"

ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
recategorize_combining_class (hb_codepoint_t u,
			      unsigned ಪೂರ್ಣಾಙ್ಕ klass)
{
  ಯದಿ (klass >= 200)
    ಪ್ರತ್ಯಾಗಮ klass;

  /* Thai / Lao need some per-character work. */
  ಯದಿ ((u & ~0xFF) == 0x0E00u)
  {
    ಯದಿ (unlikely (klass == 0))
    {
      ವಿಕಲ್ಪ (u)
      {
	ಪಕ್ಷ 0x0E31u:
	ಪಕ್ಷ 0x0E34u:
	ಪಕ್ಷ 0x0E35u:
	ಪಕ್ಷ 0x0E36u:
	ಪಕ್ಷ 0x0E37u:
	ಪಕ್ಷ 0x0E47u:
	ಪಕ್ಷ 0x0E4Cu:
	ಪಕ್ಷ 0x0E4Du:
	ಪಕ್ಷ 0x0E4Eu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_ಬಲ;
	  ಭಙ್ಗ;

	ಪಕ್ಷ 0x0EB1u:
	ಪಕ್ಷ 0x0EB4u:
	ಪಕ್ಷ 0x0EB5u:
	ಪಕ್ಷ 0x0EB6u:
	ಪಕ್ಷ 0x0EB7u:
	ಪಕ್ಷ 0x0EBBu:
	ಪಕ್ಷ 0x0ECCu:
	ಪಕ್ಷ 0x0ECDu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE;
	  ಭಙ್ಗ;

	ಪಕ್ಷ 0x0EBCu:
	  klass = HB_UNICODE_COMBINING_CLASS_BELOW;
	  ಭಙ್ಗ;
      }
    } ಅನ್ಯಥಾ {
      /* Thai virama is below-right */
      ಯದಿ (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_BELOW_ಬಲ;
    }
  }

  ವಿಕಲ್ಪ (klass)
  {

    /* Hebrew */

    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC10: /* sheva */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC11: /* hataf segol */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC12: /* hataf patah */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC13: /* hataf qamats */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC14: /* hiriq */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC15: /* tsere */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC16: /* segol */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC17: /* patah */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC18: /* qamats & qamats qatan */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC20: /* qubuts */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC22: /* meteg */
      ಪ್ರತ್ಯಾಗಮ HB_UNICODE_COMBINING_CLASS_BELOW;

    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      ಪ್ರತ್ಯಾಗಮ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      ಪ್ರತ್ಯಾಗಮ HB_UNICODE_COMBINING_CLASS_ABOVE_ಬಲ;

    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      ಪ್ರತ್ಯಾಗಮ HB_UNICODE_COMBINING_CLASS_ABOVE_ಎಡ;

    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      ಪ್ರತ್ಯಾಗಮ HB_UNICODE_COMBINING_CLASS_ABOVE;

    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC21: /* dagesh */
      ಭಙ್ಗ;


    /* Arabic and Syriac */

    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC27: /* fathatan */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC28: /* dammatan */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC30: /* fatha */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC31: /* damma */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC33: /* shadda */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC34: /* sukun */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC35: /* superscript alef */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC36: /* superscript alaph */
      ಪ್ರತ್ಯಾಗಮ HB_UNICODE_COMBINING_CLASS_ABOVE;

    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      ಪ್ರತ್ಯಾಗಮ HB_UNICODE_COMBINING_CLASS_BELOW;


    /* Thai */

    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      ಪ್ರತ್ಯಾಗಮ HB_UNICODE_COMBINING_CLASS_BELOW_ಬಲ;

    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      ಪ್ರತ್ಯಾಗಮ HB_UNICODE_COMBINING_CLASS_ABOVE_ಬಲ;


    /* Lao */

    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      ಪ್ರತ್ಯಾಗಮ HB_UNICODE_COMBINING_CLASS_BELOW;

    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      ಪ್ರತ್ಯಾಗಮ HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      ಪ್ರತ್ಯಾಗಮ HB_UNICODE_COMBINING_CLASS_BELOW;

    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      ಪ್ರತ್ಯಾಗಮ HB_UNICODE_COMBINING_CLASS_ABOVE;

    ಪಕ್ಷ HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      ಪ್ರತ್ಯಾಗಮ HB_UNICODE_COMBINING_CLASS_BELOW;

  }

  ಪ್ರತ್ಯಾಗಮ klass;
}

ಶೂನ್ಯ
_hb_ot_shape_fallback_mark_position_recategorize_marks (ಧ್ರುವ hb_ot_shape_plan_t *plan HB_UNUSED,
							hb_font_t *font HB_UNUSED,
							hb_buffer_t  *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  ಪ್ರತ್ಯಾಗಮ;
#endif

  unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    ಯದಿ (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      unsigned ಪೂರ್ಣಾಙ್ಕ combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      _hb_glyph_info_set_modified_combining_class (&info[i], combining_class);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
zero_mark_advances (hb_buffer_t *buffer,
		    unsigned ಪೂರ್ಣಾಙ್ಕ start,
		    unsigned ಪೂರ್ಣಾಙ್ಕ end,
		    ತಾರ್ಕಿಕ adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = buffer->info;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = start; i < end; i++)
    ಯದಿ (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      ಯದಿ (adjust_offsets_when_zeroing)
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

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
position_mark (ಧ್ರುವ hb_ot_shape_plan_t *plan HB_UNUSED,
	       hb_font_t *font,
	       hb_buffer_t  *buffer,
	       hb_glyph_extents_t &base_extents,
	       unsigned ಪೂರ್ಣಾಙ್ಕ i,
	       unsigned ಪೂರ್ಣಾಙ್ಕ combining_class)
{
  hb_glyph_extents_t mark_extents;
  ಯದಿ (!font->get_glyph_extents (buffer->info[i].codepoint, &mark_extents))
    ಪ್ರತ್ಯಾಗಮ;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = buffer->pos[i];
  pos.x_offset = pos.y_offset = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  ವಿಕಲ್ಪ (combining_class)
  {
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      ಯದಿ (buffer->props.direction == HB_DIRECTION_LTR) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	ಭಙ್ಗ;
      } ಅನ್ಯಥಾ ಯದಿ (buffer->props.direction == HB_DIRECTION_RTL) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	ಭಙ್ಗ;
      }
      HB_FALLTHROUGH;

    ಸ್ವಭಾವ:
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_BELOW:
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing +
						((int64_t) base_extents.width - mark_extents.width) / 2 -
						mark_extents.x_bearing);
      ಭಙ್ಗ;

    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_ಎಡ:
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_BELOW_ಎಡ:
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_ABOVE_ಎಡ:
      /* Left align. */
      pos.x_offset = hb_saturate_sub (base_extents.x_bearing, mark_extents.x_bearing);
      ಭಙ್ಗ;

    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_ಬಲ:
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_BELOW_ಬಲ:
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_ABOVE_ಬಲ:
      /* Right align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						mark_extents.width - mark_extents.x_bearing);
      ಭಙ್ಗ;
  }

  /* Y positioning */
  ವಿಕಲ್ಪ (combining_class)
  {
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_BELOW_ಎಡ:
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_BELOW:
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_BELOW_ಬಲ:
      /* Add gap, fall-through. */
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_ಎಡ:
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing +
						base_extents.height - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      ಯದಿ ((y_gap > 0) == (pos.y_offset > 0))
      {
	base_extents.height = hb_saturate_sub (base_extents.height, pos.y_offset);
	pos.y_offset = 0;
      }
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      ಭಙ್ಗ;

    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_ABOVE_ಎಡ:
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_ABOVE:
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_ABOVE_ಬಲ:
      /* Add gap, fall-through. */
      base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, y_gap);
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    ಪಕ್ಷ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_ಬಲ:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing -
						mark_extents.y_bearing - mark_extents.height);
      /* Don't shift down "above" marks too much. */
      ಯದಿ ((y_gap > 0) != (pos.y_offset > 0))
      {
	hb_position_t correction = -(pos.y_offset / 2);
	base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, correction);
	base_extents.height = hb_saturate_sub (base_extents.height, correction);
	pos.y_offset = hb_saturate_add (pos.y_offset, correction);
      }
      base_extents.y_bearing = hb_saturate_sub (base_extents.y_bearing, mark_extents.height);
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      ಭಙ್ಗ;
  }
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
position_around_base (ಧ್ರುವ hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_buffer_t  *buffer,
		      unsigned ಪೂರ್ಣಾಙ್ಕ base,
		      unsigned ಪೂರ್ಣಾಙ್ಕ end,
		      ತಾರ್ಕಿಕ adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  buffer->unsafe_to_break (base, end);

  hb_glyph_extents_t base_extents;
  ಯದಿ (!font->get_glyph_extents (buffer->info[base].codepoint,
				&base_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (buffer, base + 1, end, adjust_offsets_when_zeroing);
    ಪ್ರತ್ಯಾಗಮ;
  }
  base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing,
					    buffer->pos[base].y_offset);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  base_extents.x_bearing = 0;
  base_extents.width = font->get_glyph_h_advance (buffer->info[base].codepoint);

  unsigned ಪೂರ್ಣಾಙ್ಕ lig_id = _hb_glyph_info_get_lig_id (&buffer->info[base]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_lig_components = hb_glyph_info_get_lig_ಅಂಕೆ_comps (&buffer->info[base]);

  hb_position_t x_offset = 0, y_offset = 0;
  ಯದಿ (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
    x_offset = hb_saturate_neg (buffer->pos[base].x_advance);
    y_offset = hb_saturate_neg (buffer->pos[base].y_advance);
  }

  hb_glyph_extents_t component_extents = base_extents;
  ಪೂರ್ಣಾಙ್ಕ last_lig_component = -1;
  unsigned ಪೂರ್ಣಾಙ್ಕ last_combining_class = 255;
  hb_glyph_extents_t cluster_extents = base_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = buffer->info;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = base + 1; i < end; i++)
    ಯದಿ (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      ಯದಿ (ಅಂಕೆ_lig_components > 1) {
	unsigned ಪೂರ್ಣಾಙ್ಕ this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	ಪೂರ್ಣಾಙ್ಕ this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
	/* Conditions for attaching to the last component. */
	ಯದಿ (!lig_id || lig_id != this_lig_id || this_lig_component >= ಅಂಕೆ_lig_components)
	  this_lig_component = ಅಂಕೆ_lig_components - 1;
	ಯದಿ (last_lig_component != this_lig_component)
	{
	  last_lig_component = this_lig_component;
	  last_combining_class = 255;
	  component_extents = base_extents;
	  ಯದಿ (unlikely (horiz_dir == HB_DIRECTION_INVALID)) {
	    ಯದಿ (HB_DIRECTION_IS_HORIZONTAL (plan->props.direction))
	      horiz_dir = plan->props.direction;
	    ಅನ್ಯಥಾ
	      horiz_dir = hb_script_get_horizontal_direction (plan->props.script);
	  }
	  ಯದಿ (horiz_dir == HB_DIRECTION_LTR)
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) this_lig_component * component_extents.width / ಅಂಕೆ_lig_components);
	  ಅನ್ಯಥಾ
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) (ಅಂಕೆ_lig_components - 1 - this_lig_component) * component_extents.width / ಅಂಕೆ_lig_components);
	  component_extents.width /= ಅಂಕೆ_lig_components;
	}
      }

      unsigned ಪೂರ್ಣಾಙ್ಕ this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      ಯದಿ (last_combining_class != this_combining_class)
      {
	last_combining_class = this_combining_class;
	cluster_extents = component_extents;
      }

      position_mark (plan, font, buffer, cluster_extents, i, this_combining_class);

      buffer->pos[i].x_advance = 0;
      buffer->pos[i].y_advance = 0;
      buffer->pos[i].x_offset = hb_saturate_add (buffer->pos[i].x_offset, x_offset);
      buffer->pos[i].y_offset = hb_saturate_add (buffer->pos[i].y_offset, y_offset);

    } ಅನ್ಯಥಾ {
      ಯದಿ (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
	x_offset = hb_saturate_sub (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_sub (y_offset, buffer->pos[i].y_advance);
      } ಅನ್ಯಥಾ {
	x_offset = hb_saturate_add (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_add (y_offset, buffer->pos[i].y_advance);
      }
    }
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
position_cluster_impl (ಧ್ರುವ hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t  *buffer,
		       unsigned ಪೂರ್ಣಾಙ್ಕ start,
		       unsigned ಪೂರ್ಣಾಙ್ಕ end,
		       ತಾರ್ಕಿಕ adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = buffer->info;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = start; i < end; i++)
    ಯದಿ (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      unsigned ಪೂರ್ಣಾಙ್ಕ j;
      ಪ್ರತಿ (j = i + 1; j < end; j++)
      {
        ಯದಿ (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  ಅನುವರ್ತ;
	ಯದಿ (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  ಭಙ್ಗ;
      }

      position_around_base (plan, font, buffer, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

ಸ್ಥಿರ HB_ALWAYS_INLINE ಶೂನ್ಯ
position_cluster (ಧ್ರುವ hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_buffer_t  *buffer,
		  unsigned ಪೂರ್ಣಾಙ್ಕ start,
		  unsigned ಪೂರ್ಣಾಙ್ಕ end,
		  ತಾರ್ಕಿಕ adjust_offsets_when_zeroing)
{
  ಯದಿ (end - start < 2)
    ಪ್ರತ್ಯಾಗಮ;

  position_cluster_impl (plan, font, buffer, start, end, adjust_offsets_when_zeroing);
}

ಶೂನ್ಯ
_hb_ot_shape_fallback_mark_position (ಧ್ರುವ hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_buffer_t  *buffer,
				     ತಾರ್ಕಿಕ adjust_offsets_when_zeroing)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  ಪ್ರತ್ಯಾಗಮ;
#endif

  ಯದಿ (!buffer->message (font, "start fallback mark"))
    ಪ್ರತ್ಯಾಗಮ;

  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned ಪೂರ್ಣಾಙ್ಕ start = 0;
  unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < ಎಣಿಕೆ; i++)
    ಯದಿ (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, buffer, start, i, adjust_offsets_when_zeroing);
      start = i;
    }
  position_cluster (plan, font, buffer, start, ಎಣಿಕೆ, adjust_offsets_when_zeroing);

  (ಶೂನ್ಯ) buffer->message (font, "end fallback mark");
}


#ifndef HB_DISABLE_DEPRECATED
ಸಂರಚನಾ hb_ot_shape_fallback_kern_driver_t
{
  hb_ot_shape_fallback_kern_driver_t (hb_font_t   *font_,
				      hb_buffer_t *buffer) :
    font (font_), direction (buffer->props.direction) {}

  hb_position_t get_kerning (hb_codepoint_t first, hb_codepoint_t second) ಧ್ರುವ
  {
    hb_position_t kern = 0;
    font->get_glyph_kerning_for_direction (first, second,
					   direction,
					   &kern, &kern);
    ಪ್ರತ್ಯಾಗಮ kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#endif

/* Performs font-assisted kerning. */
ಶೂನ್ಯ
_hb_ot_shape_fallback_kern (ಧ್ರುವ hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  ಪ್ರತ್ಯಾಗಮ;
#endif

#ifndef HB_DISABLE_DEPRECATED
  ಯದಿ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (!buffer->message (font, "start fallback kern"))
    ಪ್ರತ್ಯಾಗಮ;

  ತಾರ್ಕಿಕ reverse = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  ಯದಿ (reverse)
    buffer->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, buffer);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, buffer, plan->kern_mask, ಅಸತ್ಯ);

  ಯದಿ (reverse)
    buffer->reverse ();

  (ಶೂನ್ಯ) buffer->message (font, "end fallback kern");
#endif
}


/* Adjusts width of various spaces. */
ಶೂನ್ಯ
_hb_ot_shape_fallback_spaces (ಧ್ರುವ hb_ot_shape_plan_t *plan HB_UNUSED,
			      hb_font_t *font,
			      hb_buffer_t  *buffer)
{
  hb_glyph_info_t *info = buffer->info;
  hb_glyph_position_t *pos = buffer->pos;
  ತಾರ್ಕಿಕ horizontal = HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction);
  unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = buffer->len;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    ಯದಿ (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      ಯದಿ (buffer->invisible && info[i].codepoint == buffer->invisible)
      {
        ಯದಿ (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        ಅನ್ಯಥಾ
	  pos[i].y_advance = -(int64_t) font->y_scale / 4;
      }

      hb_unicode_funcs_t::space_t space_type = _hb_glyph_info_get_unicode_space_fallback_type (&info[i]);
      hb_codepoint_t glyph;
      ಪ್ರಕಾರನಿರ್ದೇಶ hb_unicode_funcs_t t;
      ವಿಕಲ್ಪ (space_type)
      {
	ಪಕ್ಷ t::NOT_SPACE: /* Shouldn't happen. */
	ಪಕ್ಷ t::SPACE:
	  ಭಙ್ಗ;

	ಪಕ್ಷ t::SPACE_EM:
	ಪಕ್ಷ t::SPACE_EM_2:
	ಪಕ್ಷ t::SPACE_EM_3:
	ಪಕ್ಷ t::SPACE_EM_4:
	ಪಕ್ಷ t::SPACE_EM_5:
	ಪಕ್ಷ t::SPACE_EM_6:
	ಪಕ್ಷ t::SPACE_EM_16:
	  ಯದಿ (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((ಪೂರ್ಣಾಙ್ಕ) space_type)/2) / (ಪೂರ್ಣಾಙ್ಕ) space_type;
	  ಅನ್ಯಥಾ
	    pos[i].y_advance = -((int64_t) font->y_scale + ((ಪೂರ್ಣಾಙ್ಕ) space_type)/2) / (ಪೂರ್ಣಾಙ್ಕ) space_type;
	  ಭಙ್ಗ;

	ಪಕ್ಷ t::SPACE_4_EM_18:
	  ಯದಿ (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  ಅನ್ಯಥಾ
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  ಭಙ್ಗ;

	ಪಕ್ಷ t::SPACE_FIGURE:
	  ಪ್ರತಿ (ವರ್ಣ u = '0'; u <= '9'; u++)
	    ಯದಿ (font->get_nominal_glyph (u, &glyph))
	    {
	      ಯದಿ (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      ಅನ್ಯಥಾ
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      ಭಙ್ಗ;
	    }
	  ಭಙ್ಗ;

	ಪಕ್ಷ t::SPACE_PUNCTUATION:
	  ಯದಿ (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    ಯದಿ (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    ಅನ್ಯಥಾ
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  ಭಙ್ಗ;

	ಪಕ್ಷ t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  ಯದಿ (horizontal)
	    pos[i].x_advance /= 2;
	  ಅನ್ಯಥಾ
	    pos[i].y_advance /= 2;
	  ಭಙ್ಗ;
      }
    }
}


#endif
