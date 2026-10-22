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

স্থির unsigned পূর্ণসংখ্যা
recategorize_combining_class (hb_codepoint_t u,
			      unsigned পূর্ণসংখ্যা klass)
{
  যদি (klass >= 200)
    ফেরত klass;

  /* Thai / Lao need some per-character work. */
  যদি ((u & ~0xFF) == 0x0E00u)
  {
    যদি (unlikely (klass == 0))
    {
      নির্বাচন (u)
      {
	ক্ষেত্র 0x0E31u:
	ক্ষেত্র 0x0E34u:
	ক্ষেত্র 0x0E35u:
	ক্ষেত্র 0x0E36u:
	ক্ষেত্র 0x0E37u:
	ক্ষেত্র 0x0E47u:
	ক্ষেত্র 0x0E4Cu:
	ক্ষেত্র 0x0E4Du:
	ক্ষেত্র 0x0E4Eu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_ডান;
	  বিরতি;

	ক্ষেত্র 0x0EB1u:
	ক্ষেত্র 0x0EB4u:
	ক্ষেত্র 0x0EB5u:
	ক্ষেত্র 0x0EB6u:
	ক্ষেত্র 0x0EB7u:
	ক্ষেত্র 0x0EBBu:
	ক্ষেত্র 0x0ECCu:
	ক্ষেত্র 0x0ECDu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE;
	  বিরতি;

	ক্ষেত্র 0x0EBCu:
	  klass = HB_UNICODE_COMBINING_CLASS_BELOW;
	  বিরতি;
      }
    } অন্যথা {
      /* Thai virama is below-right */
      যদি (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_BELOW_ডান;
    }
  }

  নির্বাচন (klass)
  {

    /* Hebrew */

    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC10: /* sheva */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC11: /* hataf segol */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC12: /* hataf patah */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC13: /* hataf qamats */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC14: /* hiriq */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC15: /* tsere */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC16: /* segol */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC17: /* patah */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC18: /* qamats & qamats qatan */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC20: /* qubuts */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC22: /* meteg */
      ফেরত HB_UNICODE_COMBINING_CLASS_BELOW;

    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      ফেরত HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      ফেরত HB_UNICODE_COMBINING_CLASS_ABOVE_ডান;

    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      ফেরত HB_UNICODE_COMBINING_CLASS_ABOVE_বাম;

    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      ফেরত HB_UNICODE_COMBINING_CLASS_ABOVE;

    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC21: /* dagesh */
      বিরতি;


    /* Arabic and Syriac */

    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC27: /* fathatan */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC28: /* dammatan */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC30: /* fatha */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC31: /* damma */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC33: /* shadda */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC34: /* sukun */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC35: /* superscript alef */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC36: /* superscript alaph */
      ফেরত HB_UNICODE_COMBINING_CLASS_ABOVE;

    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      ফেরত HB_UNICODE_COMBINING_CLASS_BELOW;


    /* Thai */

    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      ফেরত HB_UNICODE_COMBINING_CLASS_BELOW_ডান;

    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      ফেরত HB_UNICODE_COMBINING_CLASS_ABOVE_ডান;


    /* Lao */

    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      ফেরত HB_UNICODE_COMBINING_CLASS_BELOW;

    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      ফেরত HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      ফেরত HB_UNICODE_COMBINING_CLASS_BELOW;

    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      ফেরত HB_UNICODE_COMBINING_CLASS_ABOVE;

    ক্ষেত্র HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      ফেরত HB_UNICODE_COMBINING_CLASS_BELOW;

  }

  ফেরত klass;
}

শূন্য
_hb_ot_shape_fallback_mark_position_recategorize_marks (ধ্রুবক hb_ot_shape_plan_t *plan HB_UNUSED,
							hb_font_t *font HB_UNUSED,
							hb_বাফার_t  *বাফার)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  ফেরত;
#endif

  unsigned পূর্ণসংখ্যা গুনতি = বাফার->len;
  hb_glyph_info_t *info = বাফার->info;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    যদি (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      unsigned পূর্ণসংখ্যা combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      _hb_glyph_info_set_modified_combining_class (&info[i], combining_class);
    }
}


স্থির শূন্য
zero_mark_advances (hb_বাফার_t *বাফার,
		    unsigned পূর্ণসংখ্যা start,
		    unsigned পূর্ণসংখ্যা end,
		    যৌক্তিক adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = বাফার->info;
  জন্য (unsigned পূর্ণসংখ্যা i = start; i < end; i++)
    যদি (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      যদি (adjust_offsets_when_zeroing)
      {
	বাফার->pos[i].x_offset = hb_saturate_sub (বাফার->pos[i].x_offset,
						     বাফার->pos[i].x_advance);
	বাফার->pos[i].y_offset = hb_saturate_sub (বাফার->pos[i].y_offset,
						     বাফার->pos[i].y_advance);
      }
      বাফার->pos[i].x_advance = 0;
      বাফার->pos[i].y_advance = 0;
    }
}

স্থির অন্তঃরেখ শূন্য
position_mark (ধ্রুবক hb_ot_shape_plan_t *plan HB_UNUSED,
	       hb_font_t *font,
	       hb_বাফার_t  *বাফার,
	       hb_glyph_extents_t &base_extents,
	       unsigned পূর্ণসংখ্যা i,
	       unsigned পূর্ণসংখ্যা combining_class)
{
  hb_glyph_extents_t mark_extents;
  যদি (!font->get_glyph_extents (বাফার->info[i].codepoint, &mark_extents))
    ফেরত;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = বাফার->pos[i];
  pos.x_offset = pos.y_offset = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  নির্বাচন (combining_class)
  {
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      যদি (বাফার->props.direction == HB_DIRECTION_LTR) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	বিরতি;
      } অন্যথা যদি (বাফার->props.direction == HB_DIRECTION_RTL) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	বিরতি;
      }
      HB_FALLTHROUGH;

    পূর্বনির্ধারিত:
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_BELOW:
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing +
						((int64_t) base_extents.width - mark_extents.width) / 2 -
						mark_extents.x_bearing);
      বিরতি;

    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_বাম:
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_BELOW_বাম:
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_ABOVE_বাম:
      /* Left align. */
      pos.x_offset = hb_saturate_sub (base_extents.x_bearing, mark_extents.x_bearing);
      বিরতি;

    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_ডান:
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_BELOW_ডান:
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_ABOVE_ডান:
      /* Right align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						mark_extents.width - mark_extents.x_bearing);
      বিরতি;
  }

  /* Y positioning */
  নির্বাচন (combining_class)
  {
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_BELOW_বাম:
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_BELOW:
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_BELOW_ডান:
      /* Add gap, fall-through. */
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_বাম:
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing +
						base_extents.height - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      যদি ((y_gap > 0) == (pos.y_offset > 0))
      {
	base_extents.height = hb_saturate_sub (base_extents.height, pos.y_offset);
	pos.y_offset = 0;
      }
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      বিরতি;

    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_ABOVE_বাম:
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_ABOVE:
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_ABOVE_ডান:
      /* Add gap, fall-through. */
      base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, y_gap);
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    ক্ষেত্র HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_ডান:
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
      বিরতি;
  }
}

স্থির অন্তঃরেখ শূন্য
position_around_base (ধ্রুবক hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_বাফার_t  *বাফার,
		      unsigned পূর্ণসংখ্যা base,
		      unsigned পূর্ণসংখ্যা end,
		      যৌক্তিক adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  বাফার->unsafe_to_break (base, end);

  hb_glyph_extents_t base_extents;
  যদি (!font->get_glyph_extents (বাফার->info[base].codepoint,
				&base_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (বাফার, base + 1, end, adjust_offsets_when_zeroing);
    ফেরত;
  }
  base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing,
					    বাফার->pos[base].y_offset);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  base_extents.x_bearing = 0;
  base_extents.width = font->get_glyph_h_advance (বাফার->info[base].codepoint);

  unsigned পূর্ণসংখ্যা lig_id = _hb_glyph_info_get_lig_id (&বাফার->info[base]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  পূর্ণসংখ্যা অঙ্ক_lig_components = hb_glyph_info_get_lig_অঙ্ক_comps (&বাফার->info[base]);

  hb_position_t x_offset = 0, y_offset = 0;
  যদি (HB_DIRECTION_IS_FORWARD (বাফার->props.direction)) {
    x_offset = hb_saturate_neg (বাফার->pos[base].x_advance);
    y_offset = hb_saturate_neg (বাফার->pos[base].y_advance);
  }

  hb_glyph_extents_t component_extents = base_extents;
  পূর্ণসংখ্যা last_lig_component = -1;
  unsigned পূর্ণসংখ্যা last_combining_class = 255;
  hb_glyph_extents_t cluster_extents = base_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = বাফার->info;
  জন্য (unsigned পূর্ণসংখ্যা i = base + 1; i < end; i++)
    যদি (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      যদি (অঙ্ক_lig_components > 1) {
	unsigned পূর্ণসংখ্যা this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	পূর্ণসংখ্যা this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
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

      unsigned পূর্ণসংখ্যা this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      যদি (last_combining_class != this_combining_class)
      {
	last_combining_class = this_combining_class;
	cluster_extents = component_extents;
      }

      position_mark (plan, font, বাফার, cluster_extents, i, this_combining_class);

      বাফার->pos[i].x_advance = 0;
      বাফার->pos[i].y_advance = 0;
      বাফার->pos[i].x_offset = hb_saturate_add (বাফার->pos[i].x_offset, x_offset);
      বাফার->pos[i].y_offset = hb_saturate_add (বাফার->pos[i].y_offset, y_offset);

    } অন্যথা {
      যদি (HB_DIRECTION_IS_FORWARD (বাফার->props.direction)) {
	x_offset = hb_saturate_sub (x_offset, বাফার->pos[i].x_advance);
	y_offset = hb_saturate_sub (y_offset, বাফার->pos[i].y_advance);
      } অন্যথা {
	x_offset = hb_saturate_add (x_offset, বাফার->pos[i].x_advance);
	y_offset = hb_saturate_add (y_offset, বাফার->pos[i].y_advance);
      }
    }
}

স্থির অন্তঃরেখ শূন্য
position_cluster_impl (ধ্রুবক hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_বাফার_t  *বাফার,
		       unsigned পূর্ণসংখ্যা start,
		       unsigned পূর্ণসংখ্যা end,
		       যৌক্তিক adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = বাফার->info;
  জন্য (unsigned পূর্ণসংখ্যা i = start; i < end; i++)
    যদি (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      unsigned পূর্ণসংখ্যা j;
      জন্য (j = i + 1; j < end; j++)
      {
        যদি (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  চালু;
	যদি (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  বিরতি;
      }

      position_around_base (plan, font, বাফার, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

স্থির HB_ALWAYS_INLINE শূন্য
position_cluster (ধ্রুবক hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_বাফার_t  *বাফার,
		  unsigned পূর্ণসংখ্যা start,
		  unsigned পূর্ণসংখ্যা end,
		  যৌক্তিক adjust_offsets_when_zeroing)
{
  যদি (end - start < 2)
    ফেরত;

  position_cluster_impl (plan, font, বাফার, start, end, adjust_offsets_when_zeroing);
}

শূন্য
_hb_ot_shape_fallback_mark_position (ধ্রুবক hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_বাফার_t  *বাফার,
				     যৌক্তিক adjust_offsets_when_zeroing)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  ফেরত;
#endif

  যদি (!বাফার->message (font, "start fallback mark"))
    ফেরত;

  hb_বাফার_assert_gsubgpos_vars (বাফার);

  unsigned পূর্ণসংখ্যা start = 0;
  unsigned পূর্ণসংখ্যা গুনতি = বাফার->len;
  hb_glyph_info_t *info = বাফার->info;
  জন্য (unsigned পূর্ণসংখ্যা i = 1; i < গুনতি; i++)
    যদি (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, বাফার, start, i, adjust_offsets_when_zeroing);
      start = i;
    }
  position_cluster (plan, font, বাফার, start, গুনতি, adjust_offsets_when_zeroing);

  (শূন্য) বাফার->message (font, "end fallback mark");
}


#ifndef HB_DISABLE_DEPRECATED
কাঠামো hb_ot_shape_fallback_kern_driver_t
{
  hb_ot_shape_fallback_kern_driver_t (hb_font_t   *font_,
				      hb_বাফার_t *বাফার) :
    font (font_), direction (বাফার->props.direction) {}

  hb_position_t get_kerning (hb_codepoint_t first, hb_codepoint_t second) ধ্রুবক
  {
    hb_position_t kern = 0;
    font->get_glyph_kerning_for_direction (first, second,
					   direction,
					   &kern, &kern);
    ফেরত kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#endif

/* Performs font-assisted kerning. */
শূন্য
_hb_ot_shape_fallback_kern (ধ্রুবক hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_বাফার_t *বাফার)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  ফেরত;
#endif

#ifndef HB_DISABLE_DEPRECATED
  যদি (HB_DIRECTION_IS_HORIZONTAL (বাফার->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    ফেরত;

  যদি (!বাফার->message (font, "start fallback kern"))
    ফেরত;

  যৌক্তিক reverse = HB_DIRECTION_IS_BACKWARD (বাফার->props.direction);

  যদি (reverse)
    বাফার->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, বাফার);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, বাফার, plan->kern_mask, মিথ্যা);

  যদি (reverse)
    বাফার->reverse ();

  (শূন্য) বাফার->message (font, "end fallback kern");
#endif
}


/* Adjusts width of various spaces. */
শূন্য
_hb_ot_shape_fallback_spaces (ধ্রুবক hb_ot_shape_plan_t *plan HB_UNUSED,
			      hb_font_t *font,
			      hb_বাফার_t  *বাফার)
{
  hb_glyph_info_t *info = বাফার->info;
  hb_glyph_position_t *pos = বাফার->pos;
  যৌক্তিক horizontal = HB_DIRECTION_IS_HORIZONTAL (বাফার->props.direction);
  unsigned পূর্ণসংখ্যা গুনতি = বাফার->len;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    যদি (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      যদি (বাফার->invisible && info[i].codepoint == বাফার->invisible)
      {
        যদি (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        অন্যথা
	  pos[i].y_advance = -(int64_t) font->y_scale / 4;
      }

      hb_unicode_funcs_t::space_t space_type = _hb_glyph_info_get_unicode_space_fallback_type (&info[i]);
      hb_codepoint_t glyph;
      প্রকারলেখ hb_unicode_funcs_t t;
      নির্বাচন (space_type)
      {
	ক্ষেত্র t::NOT_SPACE: /* Shouldn't happen. */
	ক্ষেত্র t::SPACE:
	  বিরতি;

	ক্ষেত্র t::SPACE_EM:
	ক্ষেত্র t::SPACE_EM_2:
	ক্ষেত্র t::SPACE_EM_3:
	ক্ষেত্র t::SPACE_EM_4:
	ক্ষেত্র t::SPACE_EM_5:
	ক্ষেত্র t::SPACE_EM_6:
	ক্ষেত্র t::SPACE_EM_16:
	  যদি (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((পূর্ণসংখ্যা) space_type)/2) / (পূর্ণসংখ্যা) space_type;
	  অন্যথা
	    pos[i].y_advance = -((int64_t) font->y_scale + ((পূর্ণসংখ্যা) space_type)/2) / (পূর্ণসংখ্যা) space_type;
	  বিরতি;

	ক্ষেত্র t::SPACE_4_EM_18:
	  যদি (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  অন্যথা
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  বিরতি;

	ক্ষেত্র t::SPACE_FIGURE:
	  জন্য (অক্ষর u = '0'; u <= '9'; u++)
	    যদি (font->get_nominal_glyph (u, &glyph))
	    {
	      যদি (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      অন্যথা
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      বিরতি;
	    }
	  বিরতি;

	ক্ষেত্র t::SPACE_PUNCTUATION:
	  যদি (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    যদি (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    অন্যথা
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  বিরতি;

	ক্ষেত্র t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  যদি (horizontal)
	    pos[i].x_advance /= 2;
	  অন্যথা
	    pos[i].y_advance /= 2;
	  বিরতি;
      }
    }
}


#endif
