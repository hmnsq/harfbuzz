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

#અન્તર્ભાવ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#અન્તર્ભાવ "hb-ot-shape-fallback.hh"
#અન્તર્ભાવ "hb-kern.hh"

સ્થિર unsigned પૂર્ણાંક
recategorize_combining_class (hb_codepoint_t u,
			      unsigned પૂર્ણાંક klass)
{
  જો (klass >= 200)
    પાછા klass;

  /* Thai / Lao need some per-character work. */
  જો ((u & ~0xFF) == 0x0E00u)
  {
    જો (unlikely (klass == 0))
    {
      પસંદગી (u)
      {
	સ્થિતિ 0x0E31u:
	સ્થિતિ 0x0E34u:
	સ્થિતિ 0x0E35u:
	સ્થિતિ 0x0E36u:
	સ્થિતિ 0x0E37u:
	સ્થિતિ 0x0E47u:
	સ્થિતિ 0x0E4Cu:
	સ્થિતિ 0x0E4Du:
	સ્થિતિ 0x0E4Eu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_જમણું;
	  તોડો;

	સ્થિતિ 0x0EB1u:
	સ્થિતિ 0x0EB4u:
	સ્થિતિ 0x0EB5u:
	સ્થિતિ 0x0EB6u:
	સ્થિતિ 0x0EB7u:
	સ્થિતિ 0x0EBBu:
	સ્થિતિ 0x0ECCu:
	સ્થિતિ 0x0ECDu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE;
	  તોડો;

	સ્થિતિ 0x0EBCu:
	  klass = HB_UNICODE_COMBINING_CLASS_BELOW;
	  તોડો;
      }
    } નહીંતર {
      /* Thai virama is below-right */
      જો (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_BELOW_જમણું;
    }
  }

  પસંદગી (klass)
  {

    /* Hebrew */

    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC10: /* sheva */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC11: /* hataf segol */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC12: /* hataf patah */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC13: /* hataf qamats */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC14: /* hiriq */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC15: /* tsere */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC16: /* segol */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC17: /* patah */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC18: /* qamats & qamats qatan */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC20: /* qubuts */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC22: /* meteg */
      પાછા HB_UNICODE_COMBINING_CLASS_BELOW;

    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      પાછા HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      પાછા HB_UNICODE_COMBINING_CLASS_ABOVE_જમણું;

    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      પાછા HB_UNICODE_COMBINING_CLASS_ABOVE_ડાબું;

    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      પાછા HB_UNICODE_COMBINING_CLASS_ABOVE;

    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC21: /* dagesh */
      તોડો;


    /* Arabic and Syriac */

    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC27: /* fathatan */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC28: /* dammatan */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC30: /* fatha */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC31: /* damma */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC33: /* shadda */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC34: /* sukun */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC35: /* superscript alef */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC36: /* superscript alaph */
      પાછા HB_UNICODE_COMBINING_CLASS_ABOVE;

    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      પાછા HB_UNICODE_COMBINING_CLASS_BELOW;


    /* Thai */

    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      પાછા HB_UNICODE_COMBINING_CLASS_BELOW_જમણું;

    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      પાછા HB_UNICODE_COMBINING_CLASS_ABOVE_જમણું;


    /* Lao */

    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      પાછા HB_UNICODE_COMBINING_CLASS_BELOW;

    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      પાછા HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      પાછા HB_UNICODE_COMBINING_CLASS_BELOW;

    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      પાછા HB_UNICODE_COMBINING_CLASS_ABOVE;

    સ્થિતિ HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      પાછા HB_UNICODE_COMBINING_CLASS_BELOW;

  }

  પાછા klass;
}

શૂન્ય
_hb_ot_shape_fallback_mark_position_recategorize_marks (અચલ hb_ot_shape_plan_t *plan HB_UNUSED,
							hb_font_t *font HB_UNUSED,
							hb_buffer_t  *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  પાછા;
#endif

  unsigned પૂર્ણાંક ગણતરી = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    જો (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      unsigned પૂર્ણાંક combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      _hb_glyph_info_set_modified_combining_class (&info[i], combining_class);
    }
}


સ્થિર શૂન્ય
zero_mark_advances (hb_buffer_t *buffer,
		    unsigned પૂર્ણાંક start,
		    unsigned પૂર્ણાંક end,
		    તાર્કિક adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = buffer->info;
  માટે (unsigned પૂર્ણાંક i = start; i < end; i++)
    જો (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      જો (adjust_offsets_when_zeroing)
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

સ્થિર અંતઃરેખ શૂન્ય
position_mark (અચલ hb_ot_shape_plan_t *plan HB_UNUSED,
	       hb_font_t *font,
	       hb_buffer_t  *buffer,
	       hb_glyph_extents_t &base_extents,
	       unsigned પૂર્ણાંક i,
	       unsigned પૂર્ણાંક combining_class)
{
  hb_glyph_extents_t mark_extents;
  જો (!font->get_glyph_extents (buffer->info[i].codepoint, &mark_extents))
    પાછા;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = buffer->pos[i];
  pos.x_offset = pos.y_offset = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  પસંદગી (combining_class)
  {
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      જો (buffer->props.direction == HB_DIRECTION_LTR) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	તોડો;
      } નહીંતર જો (buffer->props.direction == HB_DIRECTION_RTL) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	તોડો;
      }
      HB_FALLTHROUGH;

    મૂળભૂત:
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_BELOW:
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing +
						((int64_t) base_extents.width - mark_extents.width) / 2 -
						mark_extents.x_bearing);
      તોડો;

    સ્થિતિ HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_ડાબું:
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_BELOW_ડાબું:
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_ABOVE_ડાબું:
      /* Left align. */
      pos.x_offset = hb_saturate_sub (base_extents.x_bearing, mark_extents.x_bearing);
      તોડો;

    સ્થિતિ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_જમણું:
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_BELOW_જમણું:
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_ABOVE_જમણું:
      /* Right align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						mark_extents.width - mark_extents.x_bearing);
      તોડો;
  }

  /* Y positioning */
  પસંદગી (combining_class)
  {
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_BELOW_ડાબું:
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_BELOW:
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_BELOW_જમણું:
      /* Add gap, fall-through. */
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    સ્થિતિ HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_ડાબું:
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing +
						base_extents.height - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      જો ((y_gap > 0) == (pos.y_offset > 0))
      {
	base_extents.height = hb_saturate_sub (base_extents.height, pos.y_offset);
	pos.y_offset = 0;
      }
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      તોડો;

    સ્થિતિ HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_ABOVE_ડાબું:
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_ABOVE:
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_ABOVE_જમણું:
      /* Add gap, fall-through. */
      base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, y_gap);
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    સ્થિતિ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    સ્થિતિ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_જમણું:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing -
						mark_extents.y_bearing - mark_extents.height);
      /* Don't shift down "above" marks too much. */
      જો ((y_gap > 0) != (pos.y_offset > 0))
      {
	hb_position_t correction = -(pos.y_offset / 2);
	base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, correction);
	base_extents.height = hb_saturate_sub (base_extents.height, correction);
	pos.y_offset = hb_saturate_add (pos.y_offset, correction);
      }
      base_extents.y_bearing = hb_saturate_sub (base_extents.y_bearing, mark_extents.height);
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      તોડો;
  }
}

સ્થિર અંતઃરેખ શૂન્ય
position_around_base (અચલ hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_buffer_t  *buffer,
		      unsigned પૂર્ણાંક base,
		      unsigned પૂર્ણાંક end,
		      તાર્કિક adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  buffer->unsafe_to_break (base, end);

  hb_glyph_extents_t base_extents;
  જો (!font->get_glyph_extents (buffer->info[base].codepoint,
				&base_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (buffer, base + 1, end, adjust_offsets_when_zeroing);
    પાછા;
  }
  base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing,
					    buffer->pos[base].y_offset);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  base_extents.x_bearing = 0;
  base_extents.width = font->get_glyph_h_advance (buffer->info[base].codepoint);

  unsigned પૂર્ણાંક lig_id = _hb_glyph_info_get_lig_id (&buffer->info[base]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  પૂર્ણાંક અંક_lig_components = hb_glyph_info_get_lig_અંક_comps (&buffer->info[base]);

  hb_position_t x_offset = 0, y_offset = 0;
  જો (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
    x_offset = hb_saturate_neg (buffer->pos[base].x_advance);
    y_offset = hb_saturate_neg (buffer->pos[base].y_advance);
  }

  hb_glyph_extents_t component_extents = base_extents;
  પૂર્ણાંક last_lig_component = -1;
  unsigned પૂર્ણાંક last_combining_class = 255;
  hb_glyph_extents_t cluster_extents = base_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = buffer->info;
  માટે (unsigned પૂર્ણાંક i = base + 1; i < end; i++)
    જો (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      જો (અંક_lig_components > 1) {
	unsigned પૂર્ણાંક this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	પૂર્ણાંક this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
	/* Conditions for attaching to the last component. */
	જો (!lig_id || lig_id != this_lig_id || this_lig_component >= અંક_lig_components)
	  this_lig_component = અંક_lig_components - 1;
	જો (last_lig_component != this_lig_component)
	{
	  last_lig_component = this_lig_component;
	  last_combining_class = 255;
	  component_extents = base_extents;
	  જો (unlikely (horiz_dir == HB_DIRECTION_INVALID)) {
	    જો (HB_DIRECTION_IS_HORIZONTAL (plan->props.direction))
	      horiz_dir = plan->props.direction;
	    નહીંતર
	      horiz_dir = hb_script_get_horizontal_direction (plan->props.script);
	  }
	  જો (horiz_dir == HB_DIRECTION_LTR)
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) this_lig_component * component_extents.width / અંક_lig_components);
	  નહીંતર
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) (અંક_lig_components - 1 - this_lig_component) * component_extents.width / અંક_lig_components);
	  component_extents.width /= અંક_lig_components;
	}
      }

      unsigned પૂર્ણાંક this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      જો (last_combining_class != this_combining_class)
      {
	last_combining_class = this_combining_class;
	cluster_extents = component_extents;
      }

      position_mark (plan, font, buffer, cluster_extents, i, this_combining_class);

      buffer->pos[i].x_advance = 0;
      buffer->pos[i].y_advance = 0;
      buffer->pos[i].x_offset = hb_saturate_add (buffer->pos[i].x_offset, x_offset);
      buffer->pos[i].y_offset = hb_saturate_add (buffer->pos[i].y_offset, y_offset);

    } નહીંતર {
      જો (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
	x_offset = hb_saturate_sub (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_sub (y_offset, buffer->pos[i].y_advance);
      } નહીંતર {
	x_offset = hb_saturate_add (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_add (y_offset, buffer->pos[i].y_advance);
      }
    }
}

સ્થિર અંતઃરેખ શૂન્ય
position_cluster_impl (અચલ hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t  *buffer,
		       unsigned પૂર્ણાંક start,
		       unsigned પૂર્ણાંક end,
		       તાર્કિક adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = buffer->info;
  માટે (unsigned પૂર્ણાંક i = start; i < end; i++)
    જો (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      unsigned પૂર્ણાંક j;
      માટે (j = i + 1; j < end; j++)
      {
        જો (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  ચાલુ;
	જો (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  તોડો;
      }

      position_around_base (plan, font, buffer, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

સ્થિર HB_ALWAYS_INLINE શૂન્ય
position_cluster (અચલ hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_buffer_t  *buffer,
		  unsigned પૂર્ણાંક start,
		  unsigned પૂર્ણાંક end,
		  તાર્કિક adjust_offsets_when_zeroing)
{
  જો (end - start < 2)
    પાછા;

  position_cluster_impl (plan, font, buffer, start, end, adjust_offsets_when_zeroing);
}

શૂન્ય
_hb_ot_shape_fallback_mark_position (અચલ hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_buffer_t  *buffer,
				     તાર્કિક adjust_offsets_when_zeroing)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  પાછા;
#endif

  જો (!buffer->message (font, "start fallback mark"))
    પાછા;

  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned પૂર્ણાંક start = 0;
  unsigned પૂર્ણાંક ગણતરી = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  માટે (unsigned પૂર્ણાંક i = 1; i < ગણતરી; i++)
    જો (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, buffer, start, i, adjust_offsets_when_zeroing);
      start = i;
    }
  position_cluster (plan, font, buffer, start, ગણતરી, adjust_offsets_when_zeroing);

  (શૂન્ય) buffer->message (font, "end fallback mark");
}


#ifndef HB_DISABLE_DEPRECATED
સંરચના hb_ot_shape_fallback_kern_driver_t
{
  hb_ot_shape_fallback_kern_driver_t (hb_font_t   *font_,
				      hb_buffer_t *buffer) :
    font (font_), direction (buffer->props.direction) {}

  hb_position_t get_kerning (hb_codepoint_t first, hb_codepoint_t second) અચલ
  {
    hb_position_t kern = 0;
    font->get_glyph_kerning_for_direction (first, second,
					   direction,
					   &kern, &kern);
    પાછા kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#endif

/* Performs font-assisted kerning. */
શૂન્ય
_hb_ot_shape_fallback_kern (અચલ hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  પાછા;
#endif

#ifndef HB_DISABLE_DEPRECATED
  જો (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    પાછા;

  જો (!buffer->message (font, "start fallback kern"))
    પાછા;

  તાર્કિક reverse = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  જો (reverse)
    buffer->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, buffer);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, buffer, plan->kern_mask, ખોટું);

  જો (reverse)
    buffer->reverse ();

  (શૂન્ય) buffer->message (font, "end fallback kern");
#endif
}


/* Adjusts width of various spaces. */
શૂન્ય
_hb_ot_shape_fallback_spaces (અચલ hb_ot_shape_plan_t *plan HB_UNUSED,
			      hb_font_t *font,
			      hb_buffer_t  *buffer)
{
  hb_glyph_info_t *info = buffer->info;
  hb_glyph_position_t *pos = buffer->pos;
  તાર્કિક horizontal = HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction);
  unsigned પૂર્ણાંક ગણતરી = buffer->len;
  માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    જો (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      જો (buffer->invisible && info[i].codepoint == buffer->invisible)
      {
        જો (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        નહીંતર
	  pos[i].y_advance = -(int64_t) font->y_scale / 4;
      }

      hb_unicode_funcs_t::space_t space_type = _hb_glyph_info_get_unicode_space_fallback_type (&info[i]);
      hb_codepoint_t glyph;
      પ્રકારવ્યાખ્યા hb_unicode_funcs_t t;
      પસંદગી (space_type)
      {
	સ્થિતિ t::NOT_SPACE: /* Shouldn't happen. */
	સ્થિતિ t::SPACE:
	  તોડો;

	સ્થિતિ t::SPACE_EM:
	સ્થિતિ t::SPACE_EM_2:
	સ્થિતિ t::SPACE_EM_3:
	સ્થિતિ t::SPACE_EM_4:
	સ્થિતિ t::SPACE_EM_5:
	સ્થિતિ t::SPACE_EM_6:
	સ્થિતિ t::SPACE_EM_16:
	  જો (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((પૂર્ણાંક) space_type)/2) / (પૂર્ણાંક) space_type;
	  નહીંતર
	    pos[i].y_advance = -((int64_t) font->y_scale + ((પૂર્ણાંક) space_type)/2) / (પૂર્ણાંક) space_type;
	  તોડો;

	સ્થિતિ t::SPACE_4_EM_18:
	  જો (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  નહીંતર
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  તોડો;

	સ્થિતિ t::SPACE_FIGURE:
	  માટે (અક્ષર u = '0'; u <= '9'; u++)
	    જો (font->get_nominal_glyph (u, &glyph))
	    {
	      જો (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      નહીંતર
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      તોડો;
	    }
	  તોડો;

	સ્થિતિ t::SPACE_PUNCTUATION:
	  જો (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    જો (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    નહીંતર
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  તોડો;

	સ્થિતિ t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  જો (horizontal)
	    pos[i].x_advance /= 2;
	  નહીંતર
	    pos[i].y_advance /= 2;
	  તોડો;
      }
    }
}


#endif
