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

#অন্তর্ভাব "hb.hh"

#ifndef HB_NO_OT_SHAPE

#অন্তর্ভাব "hb-ot-shape-fallback.hh"
#অন্তর্ভাব "hb-kern.hh"

স্থির unsigned পূর্ণাঙ্ক
recategorize_combining_class (hb_codepoint_t u,
			      unsigned পূর্ণাঙ্ক klass)
{
  যদি (klass >= 200)
    প্রত্যাগম klass;

  /* Thai / Lao need some per-character work. */
  যদি ((u & ~0xFF) == 0x0E00u)
  {
    যদি (unlikely (klass == 0))
    {
      বিকল্প (u)
      {
	পক্ষ 0x0E31u:
	পক্ষ 0x0E34u:
	পক্ষ 0x0E35u:
	পক্ষ 0x0E36u:
	পক্ষ 0x0E37u:
	পক্ষ 0x0E47u:
	পক্ষ 0x0E4Cu:
	পক্ষ 0x0E4Du:
	পক্ষ 0x0E4Eu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_ডান;
	  ভঙ্গ;

	পক্ষ 0x0EB1u:
	পক্ষ 0x0EB4u:
	পক্ষ 0x0EB5u:
	পক্ষ 0x0EB6u:
	পক্ষ 0x0EB7u:
	পক্ষ 0x0EBBu:
	পক্ষ 0x0ECCu:
	পক্ষ 0x0ECDu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE;
	  ভঙ্গ;

	পক্ষ 0x0EBCu:
	  klass = HB_UNICODE_COMBINING_CLASS_BELOW;
	  ভঙ্গ;
      }
    } অন্যথা {
      /* Thai virama is below-right */
      যদি (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_BELOW_ডান;
    }
  }

  বিকল্প (klass)
  {

    /* Hebrew */

    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC10: /* sheva */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC11: /* hataf segol */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC12: /* hataf patah */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC13: /* hataf qamats */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC14: /* hiriq */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC15: /* tsere */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC16: /* segol */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC17: /* patah */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC18: /* qamats & qamats qatan */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC20: /* qubuts */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC22: /* meteg */
      প্রত্যাগম HB_UNICODE_COMBINING_CLASS_BELOW;

    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      প্রত্যাগম HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      প্রত্যাগম HB_UNICODE_COMBINING_CLASS_ABOVE_ডান;

    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      প্রত্যাগম HB_UNICODE_COMBINING_CLASS_ABOVE_বাম;

    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      প্রত্যাগম HB_UNICODE_COMBINING_CLASS_ABOVE;

    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC21: /* dagesh */
      ভঙ্গ;


    /* Arabic and Syriac */

    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC27: /* fathatan */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC28: /* dammatan */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC30: /* fatha */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC31: /* damma */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC33: /* shadda */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC34: /* sukun */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC35: /* superscript alef */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC36: /* superscript alaph */
      প্রত্যাগম HB_UNICODE_COMBINING_CLASS_ABOVE;

    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      প্রত্যাগম HB_UNICODE_COMBINING_CLASS_BELOW;


    /* Thai */

    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      প্রত্যাগম HB_UNICODE_COMBINING_CLASS_BELOW_ডান;

    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      প্রত্যাগম HB_UNICODE_COMBINING_CLASS_ABOVE_ডান;


    /* Lao */

    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      প্রত্যাগম HB_UNICODE_COMBINING_CLASS_BELOW;

    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      প্রত্যাগম HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      প্রত্যাগম HB_UNICODE_COMBINING_CLASS_BELOW;

    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      প্রত্যাগম HB_UNICODE_COMBINING_CLASS_ABOVE;

    পক্ষ HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      প্রত্যাগম HB_UNICODE_COMBINING_CLASS_BELOW;

  }

  প্রত্যাগম klass;
}

শূন্য
_hb_ot_shape_fallback_mark_position_recategorize_marks (ধ্রুব hb_ot_shape_plan_t *plan HB_UNUSED,
							hb_font_t *font HB_UNUSED,
							hb_buffer_t  *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  প্রত্যাগম;
#endif

  unsigned পূর্ণাঙ্ক গুনতি = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    যদি (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      unsigned পূর্ণাঙ্ক combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      _hb_glyph_info_set_modified_combining_class (&info[i], combining_class);
    }
}


স্থির শূন্য
zero_mark_advances (hb_buffer_t *buffer,
		    unsigned পূর্ণাঙ্ক start,
		    unsigned পূর্ণাঙ্ক end,
		    তার্কিক adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = buffer->info;
  প্রতি (unsigned পূর্ণাঙ্ক i = start; i < end; i++)
    যদি (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      যদি (adjust_offsets_when_zeroing)
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

স্থির অন্তঃস্থ শূন্য
position_mark (ধ্রুব hb_ot_shape_plan_t *plan HB_UNUSED,
	       hb_font_t *font,
	       hb_buffer_t  *buffer,
	       hb_glyph_extents_t &base_extents,
	       unsigned পূর্ণাঙ্ক i,
	       unsigned পূর্ণাঙ্ক combining_class)
{
  hb_glyph_extents_t mark_extents;
  যদি (!font->get_glyph_extents (buffer->info[i].codepoint, &mark_extents))
    প্রত্যাগম;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = buffer->pos[i];
  pos.x_offset = pos.y_offset = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  বিকল্প (combining_class)
  {
    পক্ষ HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    পক্ষ HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      যদি (buffer->props.direction == HB_DIRECTION_LTR) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	ভঙ্গ;
      } অন্যথা যদি (buffer->props.direction == HB_DIRECTION_RTL) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	ভঙ্গ;
      }
      HB_FALLTHROUGH;

    স্বভাব:
    পক্ষ HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
    পক্ষ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    পক্ষ HB_UNICODE_COMBINING_CLASS_BELOW:
    পক্ষ HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing +
						((int64_t) base_extents.width - mark_extents.width) / 2 -
						mark_extents.x_bearing);
      ভঙ্গ;

    পক্ষ HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_বাম:
    পক্ষ HB_UNICODE_COMBINING_CLASS_BELOW_বাম:
    পক্ষ HB_UNICODE_COMBINING_CLASS_ABOVE_বাম:
      /* Left align. */
      pos.x_offset = hb_saturate_sub (base_extents.x_bearing, mark_extents.x_bearing);
      ভঙ্গ;

    পক্ষ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_ডান:
    পক্ষ HB_UNICODE_COMBINING_CLASS_BELOW_ডান:
    পক্ষ HB_UNICODE_COMBINING_CLASS_ABOVE_ডান:
      /* Right align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						mark_extents.width - mark_extents.x_bearing);
      ভঙ্গ;
  }

  /* Y positioning */
  বিকল্প (combining_class)
  {
    পক্ষ HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    পক্ষ HB_UNICODE_COMBINING_CLASS_BELOW_বাম:
    পক্ষ HB_UNICODE_COMBINING_CLASS_BELOW:
    পক্ষ HB_UNICODE_COMBINING_CLASS_BELOW_ডান:
      /* Add gap, fall-through. */
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    পক্ষ HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_বাম:
    পক্ষ HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing +
						base_extents.height - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      যদি ((y_gap > 0) == (pos.y_offset > 0))
      {
	base_extents.height = hb_saturate_sub (base_extents.height, pos.y_offset);
	pos.y_offset = 0;
      }
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      ভঙ্গ;

    পক্ষ HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    পক্ষ HB_UNICODE_COMBINING_CLASS_ABOVE_বাম:
    পক্ষ HB_UNICODE_COMBINING_CLASS_ABOVE:
    পক্ষ HB_UNICODE_COMBINING_CLASS_ABOVE_ডান:
      /* Add gap, fall-through. */
      base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, y_gap);
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    পক্ষ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    পক্ষ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_ডান:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing -
						mark_extents.y_bearing - mark_extents.height);
      /* Don't shift down "above" marks too much. */
      যদি ((y_gap > 0) != (pos.y_offset > 0))
      {
	hb_position_t correction = -(pos.y_offset / 2);
	base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, correction);
	base_extents.height = hb_saturate_sub (base_extents.height, correction);
	pos.y_offset = hb_saturate_add (pos.y_offset, correction);
      }
      base_extents.y_bearing = hb_saturate_sub (base_extents.y_bearing, mark_extents.height);
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      ভঙ্গ;
  }
}

স্থির অন্তঃস্থ শূন্য
position_around_base (ধ্রুব hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_buffer_t  *buffer,
		      unsigned পূর্ণাঙ্ক base,
		      unsigned পূর্ণাঙ্ক end,
		      তার্কিক adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  buffer->unsafe_to_break (base, end);

  hb_glyph_extents_t base_extents;
  যদি (!font->get_glyph_extents (buffer->info[base].codepoint,
				&base_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (buffer, base + 1, end, adjust_offsets_when_zeroing);
    প্রত্যাগম;
  }
  base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing,
					    buffer->pos[base].y_offset);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  base_extents.x_bearing = 0;
  base_extents.width = font->get_glyph_h_advance (buffer->info[base].codepoint);

  unsigned পূর্ণাঙ্ক lig_id = _hb_glyph_info_get_lig_id (&buffer->info[base]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  পূর্ণাঙ্ক অঙ্ক_lig_components = hb_glyph_info_get_lig_অঙ্ক_comps (&buffer->info[base]);

  hb_position_t x_offset = 0, y_offset = 0;
  যদি (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
    x_offset = hb_saturate_neg (buffer->pos[base].x_advance);
    y_offset = hb_saturate_neg (buffer->pos[base].y_advance);
  }

  hb_glyph_extents_t component_extents = base_extents;
  পূর্ণাঙ্ক last_lig_component = -1;
  unsigned পূর্ণাঙ্ক last_combining_class = 255;
  hb_glyph_extents_t cluster_extents = base_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = buffer->info;
  প্রতি (unsigned পূর্ণাঙ্ক i = base + 1; i < end; i++)
    যদি (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      যদি (অঙ্ক_lig_components > 1) {
	unsigned পূর্ণাঙ্ক this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	পূর্ণাঙ্ক this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
	/* Conditions for attaching to the last component. */
	যদি (!lig_id || lig_id != this_lig_id || this_lig_component >= অঙ্ক_lig_components)
	  this_lig_component = অঙ্ক_lig_components - 1;
	যদি (last_lig_component != this_lig_component)
	{
	  last_lig_component = this_lig_component;
	  last_combining_class = 255;
	  component_extents = base_extents;
	  যদি (unlikely (horiz_dir == HB_DIRECTION_INVALID)) {
	    যদি (HB_DIRECTION_IS_HORIZONTAL (plan->props.direction))
	      horiz_dir = plan->props.direction;
	    অন্যথা
	      horiz_dir = hb_script_get_horizontal_direction (plan->props.script);
	  }
	  যদি (horiz_dir == HB_DIRECTION_LTR)
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) this_lig_component * component_extents.width / অঙ্ক_lig_components);
	  অন্যথা
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) (অঙ্ক_lig_components - 1 - this_lig_component) * component_extents.width / অঙ্ক_lig_components);
	  component_extents.width /= অঙ্ক_lig_components;
	}
      }

      unsigned পূর্ণাঙ্ক this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      যদি (last_combining_class != this_combining_class)
      {
	last_combining_class = this_combining_class;
	cluster_extents = component_extents;
      }

      position_mark (plan, font, buffer, cluster_extents, i, this_combining_class);

      buffer->pos[i].x_advance = 0;
      buffer->pos[i].y_advance = 0;
      buffer->pos[i].x_offset = hb_saturate_add (buffer->pos[i].x_offset, x_offset);
      buffer->pos[i].y_offset = hb_saturate_add (buffer->pos[i].y_offset, y_offset);

    } অন্যথা {
      যদি (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
	x_offset = hb_saturate_sub (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_sub (y_offset, buffer->pos[i].y_advance);
      } অন্যথা {
	x_offset = hb_saturate_add (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_add (y_offset, buffer->pos[i].y_advance);
      }
    }
}

স্থির অন্তঃস্থ শূন্য
position_cluster_impl (ধ্রুব hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t  *buffer,
		       unsigned পূর্ণাঙ্ক start,
		       unsigned পূর্ণাঙ্ক end,
		       তার্কিক adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = buffer->info;
  প্রতি (unsigned পূর্ণাঙ্ক i = start; i < end; i++)
    যদি (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      unsigned পূর্ণাঙ্ক j;
      প্রতি (j = i + 1; j < end; j++)
      {
        যদি (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  অনুবর্ত;
	যদি (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  ভঙ্গ;
      }

      position_around_base (plan, font, buffer, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

স্থির HB_ALWAYS_INLINE শূন্য
position_cluster (ধ্রুব hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_buffer_t  *buffer,
		  unsigned পূর্ণাঙ্ক start,
		  unsigned পূর্ণাঙ্ক end,
		  তার্কিক adjust_offsets_when_zeroing)
{
  যদি (end - start < 2)
    প্রত্যাগম;

  position_cluster_impl (plan, font, buffer, start, end, adjust_offsets_when_zeroing);
}

শূন্য
_hb_ot_shape_fallback_mark_position (ধ্রুব hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_buffer_t  *buffer,
				     তার্কিক adjust_offsets_when_zeroing)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  প্রত্যাগম;
#endif

  যদি (!buffer->message (font, "start fallback mark"))
    প্রত্যাগম;

  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned পূর্ণাঙ্ক start = 0;
  unsigned পূর্ণাঙ্ক গুনতি = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  প্রতি (unsigned পূর্ণাঙ্ক i = 1; i < গুনতি; i++)
    যদি (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, buffer, start, i, adjust_offsets_when_zeroing);
      start = i;
    }
  position_cluster (plan, font, buffer, start, গুনতি, adjust_offsets_when_zeroing);

  (শূন্য) buffer->message (font, "end fallback mark");
}


#ifndef HB_DISABLE_DEPRECATED
সংরচনা hb_ot_shape_fallback_kern_driver_t
{
  hb_ot_shape_fallback_kern_driver_t (hb_font_t   *font_,
				      hb_buffer_t *buffer) :
    font (font_), direction (buffer->props.direction) {}

  hb_position_t get_kerning (hb_codepoint_t first, hb_codepoint_t second) ধ্রুব
  {
    hb_position_t kern = 0;
    font->get_glyph_kerning_for_direction (first, second,
					   direction,
					   &kern, &kern);
    প্রত্যাগম kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#endif

/* Performs font-assisted kerning. */
শূন্য
_hb_ot_shape_fallback_kern (ধ্রুব hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  প্রত্যাগম;
#endif

#ifndef HB_DISABLE_DEPRECATED
  যদি (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    প্রত্যাগম;

  যদি (!buffer->message (font, "start fallback kern"))
    প্রত্যাগম;

  তার্কিক reverse = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  যদি (reverse)
    buffer->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, buffer);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, buffer, plan->kern_mask, অসত্য);

  যদি (reverse)
    buffer->reverse ();

  (শূন্য) buffer->message (font, "end fallback kern");
#endif
}


/* Adjusts width of various spaces. */
শূন্য
_hb_ot_shape_fallback_spaces (ধ্রুব hb_ot_shape_plan_t *plan HB_UNUSED,
			      hb_font_t *font,
			      hb_buffer_t  *buffer)
{
  hb_glyph_info_t *info = buffer->info;
  hb_glyph_position_t *pos = buffer->pos;
  তার্কিক horizontal = HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction);
  unsigned পূর্ণাঙ্ক গুনতি = buffer->len;
  প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    যদি (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      যদি (buffer->invisible && info[i].codepoint == buffer->invisible)
      {
        যদি (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        অন্যথা
	  pos[i].y_advance = -(int64_t) font->y_scale / 4;
      }

      hb_unicode_funcs_t::space_t space_type = _hb_glyph_info_get_unicode_space_fallback_type (&info[i]);
      hb_codepoint_t glyph;
      প্রকারনির্দেশ hb_unicode_funcs_t t;
      বিকল্প (space_type)
      {
	পক্ষ t::NOT_SPACE: /* Shouldn't happen. */
	পক্ষ t::SPACE:
	  ভঙ্গ;

	পক্ষ t::SPACE_EM:
	পক্ষ t::SPACE_EM_2:
	পক্ষ t::SPACE_EM_3:
	পক্ষ t::SPACE_EM_4:
	পক্ষ t::SPACE_EM_5:
	পক্ষ t::SPACE_EM_6:
	পক্ষ t::SPACE_EM_16:
	  যদি (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((পূর্ণাঙ্ক) space_type)/2) / (পূর্ণাঙ্ক) space_type;
	  অন্যথা
	    pos[i].y_advance = -((int64_t) font->y_scale + ((পূর্ণাঙ্ক) space_type)/2) / (পূর্ণাঙ্ক) space_type;
	  ভঙ্গ;

	পক্ষ t::SPACE_4_EM_18:
	  যদি (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  অন্যথা
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  ভঙ্গ;

	পক্ষ t::SPACE_FIGURE:
	  প্রতি (বর্ণ u = '0'; u <= '9'; u++)
	    যদি (font->get_nominal_glyph (u, &glyph))
	    {
	      যদি (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      অন্যথা
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      ভঙ্গ;
	    }
	  ভঙ্গ;

	পক্ষ t::SPACE_PUNCTUATION:
	  যদি (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    যদি (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    অন্যথা
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  ভঙ্গ;

	পক্ষ t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  যদি (horizontal)
	    pos[i].x_advance /= 2;
	  অন্যথা
	    pos[i].y_advance /= 2;
	  ভঙ্গ;
      }
    }
}


#endif
