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

#include "hb.hh"

#ifndef HB_NO_OT_SHAPE

#include "hb-ot-shape-fallback.hh"
#include "hb-kern.hh"

static unsigned ціле
recategorize_combining_class (hb_codepoint_t u,
			      unsigned ціле klass)
{
  якщо (klass >= 200)
    повернути klass;

  /* Thai / Lao need some per-character work. */
  якщо ((u & ~0xFF) == 0x0E00u)
  {
    якщо (unlikely (klass == 0))
    {
      switch (u)
      {
	case 0x0E31u:
	case 0x0E34u:
	case 0x0E35u:
	case 0x0E36u:
	case 0x0E37u:
	case 0x0E47u:
	case 0x0E4Cu:
	case 0x0E4Du:
	case 0x0E4Eu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_RIGHT;
	  break;

	case 0x0EB1u:
	case 0x0EB4u:
	case 0x0EB5u:
	case 0x0EB6u:
	case 0x0EB7u:
	case 0x0EBBu:
	case 0x0ECCu:
	case 0x0ECDu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE;
	  break;

	case 0x0EBCu:
	  klass = HB_UNICODE_COMBINING_CLASS_BELOW;
	  break;
      }
    } інакше {
      /* Thai virama is below-right */
      якщо (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_BELOW_RIGHT;
    }
  }

  switch (klass)
  {

    /* Hebrew */

    case HB_MODIFIED_COMBINING_CLASS_CCC10: /* sheva */
    case HB_MODIFIED_COMBINING_CLASS_CCC11: /* hataf segol */
    case HB_MODIFIED_COMBINING_CLASS_CCC12: /* hataf patah */
    case HB_MODIFIED_COMBINING_CLASS_CCC13: /* hataf qamats */
    case HB_MODIFIED_COMBINING_CLASS_CCC14: /* hiriq */
    case HB_MODIFIED_COMBINING_CLASS_CCC15: /* tsere */
    case HB_MODIFIED_COMBINING_CLASS_CCC16: /* segol */
    case HB_MODIFIED_COMBINING_CLASS_CCC17: /* patah */
    case HB_MODIFIED_COMBINING_CLASS_CCC18: /* qamats & qamats qatan */
    case HB_MODIFIED_COMBINING_CLASS_CCC20: /* qubuts */
    case HB_MODIFIED_COMBINING_CLASS_CCC22: /* meteg */
      повернути HB_UNICODE_COMBINING_CLASS_BELOW;

    case HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      повернути HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    case HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      повернути HB_UNICODE_COMBINING_CLASS_ABOVE_RIGHT;

    case HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    case HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      повернути HB_UNICODE_COMBINING_CLASS_ABOVE_LEFT;

    case HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      повернути HB_UNICODE_COMBINING_CLASS_ABOVE;

    case HB_MODIFIED_COMBINING_CLASS_CCC21: /* dagesh */
      break;


    /* Arabic and Syriac */

    case HB_MODIFIED_COMBINING_CLASS_CCC27: /* fathatan */
    case HB_MODIFIED_COMBINING_CLASS_CCC28: /* dammatan */
    case HB_MODIFIED_COMBINING_CLASS_CCC30: /* fatha */
    case HB_MODIFIED_COMBINING_CLASS_CCC31: /* damma */
    case HB_MODIFIED_COMBINING_CLASS_CCC33: /* shadda */
    case HB_MODIFIED_COMBINING_CLASS_CCC34: /* sukun */
    case HB_MODIFIED_COMBINING_CLASS_CCC35: /* superscript alef */
    case HB_MODIFIED_COMBINING_CLASS_CCC36: /* superscript alaph */
      повернути HB_UNICODE_COMBINING_CLASS_ABOVE;

    case HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    case HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      повернути HB_UNICODE_COMBINING_CLASS_BELOW;


    /* Thai */

    case HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      повернути HB_UNICODE_COMBINING_CLASS_BELOW_RIGHT;

    case HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      повернути HB_UNICODE_COMBINING_CLASS_ABOVE_RIGHT;


    /* Lao */

    case HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      повернути HB_UNICODE_COMBINING_CLASS_BELOW;

    case HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      повернути HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    case HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      повернути HB_UNICODE_COMBINING_CLASS_BELOW;

    case HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      повернути HB_UNICODE_COMBINING_CLASS_ABOVE;

    case HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      повернути HB_UNICODE_COMBINING_CLASS_BELOW;

  }

  повернути klass;
}

void
_hb_ot_shape_fallback_mark_position_recategorize_marks (const hb_ot_shape_plan_t *plan HB_UNUSED,
							hb_font_t *font HB_UNUSED,
							hb_buffer_t  *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  повернути;
#endif

  unsigned ціле count = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  для (unsigned ціле i = 0; i < count; i++)
    якщо (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      unsigned ціле combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      _hb_glyph_info_set_modified_combining_class (&info[i], combining_class);
    }
}


static void
zero_mark_advances (hb_buffer_t *buffer,
		    unsigned ціле start,
		    unsigned ціле end,
		    bool adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = buffer->info;
  для (unsigned ціле i = start; i < end; i++)
    якщо (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      якщо (adjust_offsets_when_zeroing)
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

static inline void
position_mark (const hb_ot_shape_plan_t *plan HB_UNUSED,
	       hb_font_t *font,
	       hb_buffer_t  *buffer,
	       hb_glyph_extents_t &base_extents,
	       unsigned ціле i,
	       unsigned ціле combining_class)
{
  hb_glyph_extents_t mark_extents;
  якщо (!font->get_glyph_extents (buffer->info[i].codepoint, &mark_extents))
    повернути;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = buffer->pos[i];
  pos.x_offset = pos.y_offset = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  switch (combining_class)
  {
    case HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    case HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      якщо (buffer->props.direction == HB_DIRECTION_LTR) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	break;
      } інакше якщо (buffer->props.direction == HB_DIRECTION_RTL) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	break;
      }
      HB_FALLTHROUGH;

    default:
    case HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
    case HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    case HB_UNICODE_COMBINING_CLASS_BELOW:
    case HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing +
						((int64_t) base_extents.width - mark_extents.width) / 2 -
						mark_extents.x_bearing);
      break;

    case HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_LEFT:
    case HB_UNICODE_COMBINING_CLASS_BELOW_LEFT:
    case HB_UNICODE_COMBINING_CLASS_ABOVE_LEFT:
      /* Left align. */
      pos.x_offset = hb_saturate_sub (base_extents.x_bearing, mark_extents.x_bearing);
      break;

    case HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_RIGHT:
    case HB_UNICODE_COMBINING_CLASS_BELOW_RIGHT:
    case HB_UNICODE_COMBINING_CLASS_ABOVE_RIGHT:
      /* Right align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						mark_extents.width - mark_extents.x_bearing);
      break;
  }

  /* Y positioning */
  switch (combining_class)
  {
    case HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    case HB_UNICODE_COMBINING_CLASS_BELOW_LEFT:
    case HB_UNICODE_COMBINING_CLASS_BELOW:
    case HB_UNICODE_COMBINING_CLASS_BELOW_RIGHT:
      /* Add gap, fall-through. */
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    case HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_LEFT:
    case HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing +
						base_extents.height - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      якщо ((y_gap > 0) == (pos.y_offset > 0))
      {
	base_extents.height = hb_saturate_sub (base_extents.height, pos.y_offset);
	pos.y_offset = 0;
      }
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      break;

    case HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    case HB_UNICODE_COMBINING_CLASS_ABOVE_LEFT:
    case HB_UNICODE_COMBINING_CLASS_ABOVE:
    case HB_UNICODE_COMBINING_CLASS_ABOVE_RIGHT:
      /* Add gap, fall-through. */
      base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, y_gap);
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    case HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    case HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_RIGHT:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing -
						mark_extents.y_bearing - mark_extents.height);
      /* Don't shift down "above" marks too much. */
      якщо ((y_gap > 0) != (pos.y_offset > 0))
      {
	hb_position_t correction = -(pos.y_offset / 2);
	base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, correction);
	base_extents.height = hb_saturate_sub (base_extents.height, correction);
	pos.y_offset = hb_saturate_add (pos.y_offset, correction);
      }
      base_extents.y_bearing = hb_saturate_sub (base_extents.y_bearing, mark_extents.height);
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      break;
  }
}

static inline void
position_around_base (const hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_buffer_t  *buffer,
		      unsigned ціле base,
		      unsigned ціле end,
		      bool adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  buffer->unsafe_to_break (base, end);

  hb_glyph_extents_t base_extents;
  якщо (!font->get_glyph_extents (buffer->info[base].codepoint,
				&base_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (buffer, base + 1, end, adjust_offsets_when_zeroing);
    повернути;
  }
  base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing,
					    buffer->pos[base].y_offset);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  base_extents.x_bearing = 0;
  base_extents.width = font->get_glyph_h_advance (buffer->info[base].codepoint);

  unsigned ціле lig_id = _hb_glyph_info_get_lig_id (&buffer->info[base]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  ціле num_lig_components = _hb_glyph_info_get_lig_num_comps (&buffer->info[base]);

  hb_position_t x_offset = 0, y_offset = 0;
  якщо (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
    x_offset = hb_saturate_neg (buffer->pos[base].x_advance);
    y_offset = hb_saturate_neg (buffer->pos[base].y_advance);
  }

  hb_glyph_extents_t component_extents = base_extents;
  ціле last_lig_component = -1;
  unsigned ціле last_combining_class = 255;
  hb_glyph_extents_t cluster_extents = base_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = buffer->info;
  для (unsigned ціле i = base + 1; i < end; i++)
    якщо (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      якщо (num_lig_components > 1) {
	unsigned ціле this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	ціле this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
	/* Conditions for attaching to the last component. */
	якщо (!lig_id || lig_id != this_lig_id || this_lig_component >= num_lig_components)
	  this_lig_component = num_lig_components - 1;
	якщо (last_lig_component != this_lig_component)
	{
	  last_lig_component = this_lig_component;
	  last_combining_class = 255;
	  component_extents = base_extents;
	  якщо (unlikely (horiz_dir == HB_DIRECTION_INVALID)) {
	    якщо (HB_DIRECTION_IS_HORIZONTAL (plan->props.direction))
	      horiz_dir = plan->props.direction;
	    інакше
	      horiz_dir = hb_script_get_horizontal_direction (plan->props.script);
	  }
	  якщо (horiz_dir == HB_DIRECTION_LTR)
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) this_lig_component * component_extents.width / num_lig_components);
	  інакше
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) (num_lig_components - 1 - this_lig_component) * component_extents.width / num_lig_components);
	  component_extents.width /= num_lig_components;
	}
      }

      unsigned ціле this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      якщо (last_combining_class != this_combining_class)
      {
	last_combining_class = this_combining_class;
	cluster_extents = component_extents;
      }

      position_mark (plan, font, buffer, cluster_extents, i, this_combining_class);

      buffer->pos[i].x_advance = 0;
      buffer->pos[i].y_advance = 0;
      buffer->pos[i].x_offset = hb_saturate_add (buffer->pos[i].x_offset, x_offset);
      buffer->pos[i].y_offset = hb_saturate_add (buffer->pos[i].y_offset, y_offset);

    } інакше {
      якщо (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
	x_offset = hb_saturate_sub (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_sub (y_offset, buffer->pos[i].y_advance);
      } інакше {
	x_offset = hb_saturate_add (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_add (y_offset, buffer->pos[i].y_advance);
      }
    }
}

static inline void
position_cluster_impl (const hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t  *buffer,
		       unsigned ціле start,
		       unsigned ціле end,
		       bool adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = buffer->info;
  для (unsigned ціле i = start; i < end; i++)
    якщо (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      unsigned ціле j;
      для (j = i + 1; j < end; j++)
      {
        якщо (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  continue;
	якщо (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  break;
      }

      position_around_base (plan, font, buffer, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

static HB_ALWAYS_INLINE void
position_cluster (const hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_buffer_t  *buffer,
		  unsigned ціле start,
		  unsigned ціле end,
		  bool adjust_offsets_when_zeroing)
{
  якщо (end - start < 2)
    повернути;

  position_cluster_impl (plan, font, buffer, start, end, adjust_offsets_when_zeroing);
}

void
_hb_ot_shape_fallback_mark_position (const hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_buffer_t  *buffer,
				     bool adjust_offsets_when_zeroing)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  повернути;
#endif

  якщо (!buffer->message (font, "start fallback mark"))
    повернути;

  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned ціле start = 0;
  unsigned ціле count = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  для (unsigned ціле i = 1; i < count; i++)
    якщо (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, buffer, start, i, adjust_offsets_when_zeroing);
      start = i;
    }
  position_cluster (plan, font, buffer, start, count, adjust_offsets_when_zeroing);

  (void) buffer->message (font, "end fallback mark");
}


#ifndef HB_DISABLE_DEPRECATED
struct hb_ot_shape_fallback_kern_driver_t
{
  hb_ot_shape_fallback_kern_driver_t (hb_font_t   *font_,
				      hb_buffer_t *buffer) :
    font (font_), direction (buffer->props.direction) {}

  hb_position_t get_kerning (hb_codepoint_t first, hb_codepoint_t second) const
  {
    hb_position_t kern = 0;
    font->get_glyph_kerning_for_direction (first, second,
					   direction,
					   &kern, &kern);
    повернути kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#endif

/* Performs font-assisted kerning. */
void
_hb_ot_shape_fallback_kern (const hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  повернути;
#endif

#ifndef HB_DISABLE_DEPRECATED
  якщо (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    повернути;

  якщо (!buffer->message (font, "start fallback kern"))
    повернути;

  bool reverse = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  якщо (reverse)
    buffer->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, buffer);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, buffer, plan->kern_mask, false);

  якщо (reverse)
    buffer->reverse ();

  (void) buffer->message (font, "end fallback kern");
#endif
}


/* Adjusts width of various spaces. */
void
_hb_ot_shape_fallback_spaces (const hb_ot_shape_plan_t *plan HB_UNUSED,
			      hb_font_t *font,
			      hb_buffer_t  *buffer)
{
  hb_glyph_info_t *info = buffer->info;
  hb_glyph_position_t *pos = buffer->pos;
  bool horizontal = HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction);
  unsigned ціле count = buffer->len;
  для (unsigned ціле i = 0; i < count; i++)
    якщо (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      якщо (buffer->invisible && info[i].codepoint == buffer->invisible)
      {
        якщо (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        інакше
	  pos[i].y_advance = -(int64_t) font->y_scale / 4;
      }

      hb_unicode_funcs_t::space_t space_type = _hb_glyph_info_get_unicode_space_fallback_type (&info[i]);
      hb_codepoint_t glyph;
      typedef hb_unicode_funcs_t t;
      switch (space_type)
      {
	case t::NOT_SPACE: /* Shouldn't happen. */
	case t::SPACE:
	  break;

	case t::SPACE_EM:
	case t::SPACE_EM_2:
	case t::SPACE_EM_3:
	case t::SPACE_EM_4:
	case t::SPACE_EM_5:
	case t::SPACE_EM_6:
	case t::SPACE_EM_16:
	  якщо (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((ціле) space_type)/2) / (ціле) space_type;
	  інакше
	    pos[i].y_advance = -((int64_t) font->y_scale + ((ціле) space_type)/2) / (ціле) space_type;
	  break;

	case t::SPACE_4_EM_18:
	  якщо (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  інакше
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  break;

	case t::SPACE_FIGURE:
	  для (char u = '0'; u <= '9'; u++)
	    якщо (font->get_nominal_glyph (u, &glyph))
	    {
	      якщо (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      інакше
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      break;
	    }
	  break;

	case t::SPACE_PUNCTUATION:
	  якщо (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    якщо (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    інакше
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  break;

	case t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  якщо (horizontal)
	    pos[i].x_advance /= 2;
	  інакше
	    pos[i].y_advance /= 2;
	  break;
      }
    }
}


#endif
