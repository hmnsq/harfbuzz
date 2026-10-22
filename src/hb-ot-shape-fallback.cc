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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#ଅନ୍ତର୍ଭାଵ "hb-ot-shape-fallback.hh"
#ଅନ୍ତର୍ଭାଵ "hb-kern.hh"

ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
recategorize_combining_class (hb_codepoint_t u,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା klass)
{
  ଯଦି (klass >= 200)
    ଫେରନ୍ତୁ klass;

  /* Thai / Lao need some per-character work. */
  ଯଦି ((u & ~0xFF) == 0x0E00u)
  {
    ଯଦି (unlikely (klass == 0))
    {
      ବାଛ (u)
      {
	ସ୍ଥିତି 0x0E31u:
	ସ୍ଥିତି 0x0E34u:
	ସ୍ଥିତି 0x0E35u:
	ସ୍ଥିତି 0x0E36u:
	ସ୍ଥିତି 0x0E37u:
	ସ୍ଥିତି 0x0E47u:
	ସ୍ଥିତି 0x0E4Cu:
	ସ୍ଥିତି 0x0E4Du:
	ସ୍ଥିତି 0x0E4Eu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_ଡାହାଣ;
	  ଭାଙ୍ଗ;

	ସ୍ଥିତି 0x0EB1u:
	ସ୍ଥିତି 0x0EB4u:
	ସ୍ଥିତି 0x0EB5u:
	ସ୍ଥିତି 0x0EB6u:
	ସ୍ଥିତି 0x0EB7u:
	ସ୍ଥିତି 0x0EBBu:
	ସ୍ଥିତି 0x0ECCu:
	ସ୍ଥିତି 0x0ECDu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE;
	  ଭାଙ୍ଗ;

	ସ୍ଥିତି 0x0EBCu:
	  klass = HB_UNICODE_COMBINING_CLASS_BELOW;
	  ଭାଙ୍ଗ;
      }
    } ଅନ୍ୟଥା {
      /* Thai virama is below-right */
      ଯଦି (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_BELOW_ଡାହାଣ;
    }
  }

  ବାଛ (klass)
  {

    /* Hebrew */

    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC10: /* sheva */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC11: /* hataf segol */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC12: /* hataf patah */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC13: /* hataf qamats */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC14: /* hiriq */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC15: /* tsere */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC16: /* segol */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC17: /* patah */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC18: /* qamats & qamats qatan */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC20: /* qubuts */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC22: /* meteg */
      ଫେରନ୍ତୁ HB_UNICODE_COMBINING_CLASS_BELOW;

    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      ଫେରନ୍ତୁ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      ଫେରନ୍ତୁ HB_UNICODE_COMBINING_CLASS_ABOVE_ଡାହାଣ;

    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      ଫେରନ୍ତୁ HB_UNICODE_COMBINING_CLASS_ABOVE_ବାମ;

    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      ଫେରନ୍ତୁ HB_UNICODE_COMBINING_CLASS_ABOVE;

    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC21: /* dagesh */
      ଭାଙ୍ଗ;


    /* Arabic and Syriac */

    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC27: /* fathatan */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC28: /* dammatan */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC30: /* fatha */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC31: /* damma */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC33: /* shadda */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC34: /* sukun */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC35: /* superscript alef */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC36: /* superscript alaph */
      ଫେରନ୍ତୁ HB_UNICODE_COMBINING_CLASS_ABOVE;

    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      ଫେରନ୍ତୁ HB_UNICODE_COMBINING_CLASS_BELOW;


    /* Thai */

    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      ଫେରନ୍ତୁ HB_UNICODE_COMBINING_CLASS_BELOW_ଡାହାଣ;

    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      ଫେରନ୍ତୁ HB_UNICODE_COMBINING_CLASS_ABOVE_ଡାହାଣ;


    /* Lao */

    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      ଫେରନ୍ତୁ HB_UNICODE_COMBINING_CLASS_BELOW;

    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      ଫେରନ୍ତୁ HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      ଫେରନ୍ତୁ HB_UNICODE_COMBINING_CLASS_BELOW;

    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      ଫେରନ୍ତୁ HB_UNICODE_COMBINING_CLASS_ABOVE;

    ସ୍ଥିତି HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      ଫେରନ୍ତୁ HB_UNICODE_COMBINING_CLASS_BELOW;

  }

  ଫେରନ୍ତୁ klass;
}

ଶୂନ୍ୟ
_hb_ot_shape_fallback_mark_position_recategorize_marks (ଅଚଳ hb_ot_shape_plan_t *plan HB_UNUSED,
							hb_font_t *font HB_UNUSED,
							hb_buffer_t  *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  ଫେରନ୍ତୁ;
#endif

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    ଯଦି (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      _hb_glyph_info_set_modified_combining_class (&info[i], combining_class);
    }
}


ସ୍ଥିର ଶୂନ୍ୟ
zero_mark_advances (hb_buffer_t *buffer,
		    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start,
		    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end,
		    ତାର୍କିକ adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = buffer->info;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = start; i < end; i++)
    ଯଦି (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      ଯଦି (adjust_offsets_when_zeroing)
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

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
position_mark (ଅଚଳ hb_ot_shape_plan_t *plan HB_UNUSED,
	       hb_font_t *font,
	       hb_buffer_t  *buffer,
	       hb_glyph_extents_t &base_extents,
	       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,
	       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା combining_class)
{
  hb_glyph_extents_t mark_extents;
  ଯଦି (!font->get_glyph_extents (buffer->info[i].codepoint, &mark_extents))
    ଫେରନ୍ତୁ;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = buffer->pos[i];
  pos.x_offset = pos.y_offset = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  ବାଛ (combining_class)
  {
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      ଯଦି (buffer->props.direction == HB_DIRECTION_LTR) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	ଭାଙ୍ଗ;
      } ଅନ୍ୟଥା ଯଦି (buffer->props.direction == HB_DIRECTION_RTL) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	ଭାଙ୍ଗ;
      }
      HB_FALLTHROUGH;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_BELOW:
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing +
						((int64_t) base_extents.width - mark_extents.width) / 2 -
						mark_extents.x_bearing);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_ବାମ:
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_BELOW_ବାମ:
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_ABOVE_ବାମ:
      /* Left align. */
      pos.x_offset = hb_saturate_sub (base_extents.x_bearing, mark_extents.x_bearing);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_ଡାହାଣ:
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_BELOW_ଡାହାଣ:
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_ABOVE_ଡାହାଣ:
      /* Right align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						mark_extents.width - mark_extents.x_bearing);
      ଭାଙ୍ଗ;
  }

  /* Y positioning */
  ବାଛ (combining_class)
  {
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_BELOW_ବାମ:
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_BELOW:
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_BELOW_ଡାହାଣ:
      /* Add gap, fall-through. */
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_ବାମ:
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing +
						base_extents.height - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      ଯଦି ((y_gap > 0) == (pos.y_offset > 0))
      {
	base_extents.height = hb_saturate_sub (base_extents.height, pos.y_offset);
	pos.y_offset = 0;
      }
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_ABOVE_ବାମ:
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_ABOVE:
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_ABOVE_ଡାହାଣ:
      /* Add gap, fall-through. */
      base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, y_gap);
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    ସ୍ଥିତି HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_ଡାହାଣ:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing -
						mark_extents.y_bearing - mark_extents.height);
      /* Don't shift down "above" marks too much. */
      ଯଦି ((y_gap > 0) != (pos.y_offset > 0))
      {
	hb_position_t correction = -(pos.y_offset / 2);
	base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, correction);
	base_extents.height = hb_saturate_sub (base_extents.height, correction);
	pos.y_offset = hb_saturate_add (pos.y_offset, correction);
      }
      base_extents.y_bearing = hb_saturate_sub (base_extents.y_bearing, mark_extents.height);
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      ଭାଙ୍ଗ;
  }
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
position_around_base (ଅଚଳ hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_buffer_t  *buffer,
		      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା base,
		      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end,
		      ତାର୍କିକ adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  buffer->unsafe_to_break (base, end);

  hb_glyph_extents_t base_extents;
  ଯଦି (!font->get_glyph_extents (buffer->info[base].codepoint,
				&base_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (buffer, base + 1, end, adjust_offsets_when_zeroing);
    ଫେରନ୍ତୁ;
  }
  base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing,
					    buffer->pos[base].y_offset);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  base_extents.x_bearing = 0;
  base_extents.width = font->get_glyph_h_advance (buffer->info[base].codepoint);

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lig_id = _hb_glyph_info_get_lig_id (&buffer->info[base]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_lig_components = hb_glyph_info_get_lig_ଅଙ୍କ_comps (&buffer->info[base]);

  hb_position_t x_offset = 0, y_offset = 0;
  ଯଦି (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
    x_offset = hb_saturate_neg (buffer->pos[base].x_advance);
    y_offset = hb_saturate_neg (buffer->pos[base].y_advance);
  }

  hb_glyph_extents_t component_extents = base_extents;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_lig_component = -1;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_combining_class = 255;
  hb_glyph_extents_t cluster_extents = base_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = buffer->info;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = base + 1; i < end; i++)
    ଯଦି (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      ଯଦି (ଅଙ୍କ_lig_components > 1) {
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
	/* Conditions for attaching to the last component. */
	ଯଦି (!lig_id || lig_id != this_lig_id || this_lig_component >= ଅଙ୍କ_lig_components)
	  this_lig_component = ଅଙ୍କ_lig_components - 1;
	ଯଦି (last_lig_component != this_lig_component)
	{
	  last_lig_component = this_lig_component;
	  last_combining_class = 255;
	  component_extents = base_extents;
	  ଯଦି (unlikely (horiz_dir == HB_DIRECTION_INVALID)) {
	    ଯଦି (HB_DIRECTION_IS_HORIZONTAL (plan->props.direction))
	      horiz_dir = plan->props.direction;
	    ଅନ୍ୟଥା
	      horiz_dir = hb_script_get_horizontal_direction (plan->props.script);
	  }
	  ଯଦି (horiz_dir == HB_DIRECTION_LTR)
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) this_lig_component * component_extents.width / ଅଙ୍କ_lig_components);
	  ଅନ୍ୟଥା
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) (ଅଙ୍କ_lig_components - 1 - this_lig_component) * component_extents.width / ଅଙ୍କ_lig_components);
	  component_extents.width /= ଅଙ୍କ_lig_components;
	}
      }

      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      ଯଦି (last_combining_class != this_combining_class)
      {
	last_combining_class = this_combining_class;
	cluster_extents = component_extents;
      }

      position_mark (plan, font, buffer, cluster_extents, i, this_combining_class);

      buffer->pos[i].x_advance = 0;
      buffer->pos[i].y_advance = 0;
      buffer->pos[i].x_offset = hb_saturate_add (buffer->pos[i].x_offset, x_offset);
      buffer->pos[i].y_offset = hb_saturate_add (buffer->pos[i].y_offset, y_offset);

    } ଅନ୍ୟଥା {
      ଯଦି (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
	x_offset = hb_saturate_sub (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_sub (y_offset, buffer->pos[i].y_advance);
      } ଅନ୍ୟଥା {
	x_offset = hb_saturate_add (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_add (y_offset, buffer->pos[i].y_advance);
      }
    }
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
position_cluster_impl (ଅଚଳ hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t  *buffer,
		       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start,
		       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end,
		       ତାର୍କିକ adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = buffer->info;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = start; i < end; i++)
    ଯଦି (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
      ପାଇଁ (j = i + 1; j < end; j++)
      {
        ଯଦି (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  ଜାରି;
	ଯଦି (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  ଭାଙ୍ଗ;
      }

      position_around_base (plan, font, buffer, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

ସ୍ଥିର HB_ALWAYS_INLINE ଶୂନ୍ୟ
position_cluster (ଅଚଳ hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_buffer_t  *buffer,
		  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start,
		  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end,
		  ତାର୍କିକ adjust_offsets_when_zeroing)
{
  ଯଦି (end - start < 2)
    ଫେରନ୍ତୁ;

  position_cluster_impl (plan, font, buffer, start, end, adjust_offsets_when_zeroing);
}

ଶୂନ୍ୟ
_hb_ot_shape_fallback_mark_position (ଅଚଳ hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_buffer_t  *buffer,
				     ତାର୍କିକ adjust_offsets_when_zeroing)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  ଫେରନ୍ତୁ;
#endif

  ଯଦି (!buffer->message (font, "start fallback mark"))
    ଫେରନ୍ତୁ;

  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = 0;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < ଗଣନା_2; i++)
    ଯଦି (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, buffer, start, i, adjust_offsets_when_zeroing);
      start = i;
    }
  position_cluster (plan, font, buffer, start, ଗଣନା_2, adjust_offsets_when_zeroing);

  (ଶୂନ୍ୟ) buffer->message (font, "end fallback mark");
}


#ifndef HB_DISABLE_DEPRECATED
ସଂରଚନା hb_ot_shape_fallback_kern_driver_t
{
  hb_ot_shape_fallback_kern_driver_t (hb_font_t   *font_,
				      hb_buffer_t *buffer) :
    font (font_), direction (buffer->props.direction) {}

  hb_position_t get_kerning (hb_codepoint_t first, hb_codepoint_t second) ଅଚଳ
  {
    hb_position_t kern = 0;
    font->get_glyph_kerning_for_direction (first, second,
					   direction,
					   &kern, &kern);
    ଫେରନ୍ତୁ kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#endif

/* Performs font-assisted kerning. */
ଶୂନ୍ୟ
_hb_ot_shape_fallback_kern (ଅଚଳ hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  ଫେରନ୍ତୁ;
#endif

#ifndef HB_DISABLE_DEPRECATED
  ଯଦି (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    ଫେରନ୍ତୁ;

  ଯଦି (!buffer->message (font, "start fallback kern"))
    ଫେରନ୍ତୁ;

  ତାର୍କିକ reverse = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  ଯଦି (reverse)
    buffer->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, buffer);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, buffer, plan->kern_mask, ମିଥ୍ୟା);

  ଯଦି (reverse)
    buffer->reverse ();

  (ଶୂନ୍ୟ) buffer->message (font, "end fallback kern");
#endif
}


/* Adjusts width of various spaces. */
ଶୂନ୍ୟ
_hb_ot_shape_fallback_spaces (ଅଚଳ hb_ot_shape_plan_t *plan HB_UNUSED,
			      hb_font_t *font,
			      hb_buffer_t  *buffer)
{
  hb_glyph_info_t *info = buffer->info;
  hb_glyph_position_t *pos = buffer->pos;
  ତାର୍କିକ horizontal = HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction);
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = buffer->len;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    ଯଦି (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      ଯଦି (buffer->invisible && info[i].codepoint == buffer->invisible)
      {
        ଯଦି (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        ଅନ୍ୟଥା
	  pos[i].y_advance = -(int64_t) font->y_scale / 4;
      }

      hb_unicode_funcs_t::space_t space_type = _hb_glyph_info_get_unicode_space_fallback_type (&info[i]);
      hb_codepoint_t glyph;
      ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_unicode_funcs_t t;
      ବାଛ (space_type)
      {
	ସ୍ଥିତି t::NOT_SPACE: /* Shouldn't happen. */
	ସ୍ଥିତି t::SPACE:
	  ଭାଙ୍ଗ;

	ସ୍ଥିତି t::SPACE_EM:
	ସ୍ଥିତି t::SPACE_EM_2:
	ସ୍ଥିତି t::SPACE_EM_3:
	ସ୍ଥିତି t::SPACE_EM_4:
	ସ୍ଥିତି t::SPACE_EM_5:
	ସ୍ଥିତି t::SPACE_EM_6:
	ସ୍ଥିତି t::SPACE_EM_16:
	  ଯଦି (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) space_type)/2) / (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) space_type;
	  ଅନ୍ୟଥା
	    pos[i].y_advance = -((int64_t) font->y_scale + ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) space_type)/2) / (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) space_type;
	  ଭାଙ୍ଗ;

	ସ୍ଥିତି t::SPACE_4_EM_18:
	  ଯଦି (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  ଅନ୍ୟଥା
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  ଭାଙ୍ଗ;

	ସ୍ଥିତି t::SPACE_FIGURE:
	  ପାଇଁ (ଅକ୍ଷର u = '0'; u <= '9'; u++)
	    ଯଦି (font->get_nominal_glyph (u, &glyph))
	    {
	      ଯଦି (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      ଅନ୍ୟଥା
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      ଭାଙ୍ଗ;
	    }
	  ଭାଙ୍ଗ;

	ସ୍ଥିତି t::SPACE_PUNCTUATION:
	  ଯଦି (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    ଯଦି (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    ଅନ୍ୟଥା
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  ଭାଙ୍ଗ;

	ସ୍ଥିତି t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  ଯଦି (horizontal)
	    pos[i].x_advance /= 2;
	  ଅନ୍ୟଥା
	    pos[i].y_advance /= 2;
	  ଭାଙ୍ଗ;
      }
    }
}


#endif
