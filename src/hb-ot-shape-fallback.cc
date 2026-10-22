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

#அந்தர்பாவ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#அந்தர்பாவ "hb-ot-shape-fallback.hh"
#அந்தர்பாவ "hb-kern.hh"

நிலையான unsigned முழு
recategorize_combining_class (hb_codepoint_t u,
			      unsigned முழு klass)
{
  எனில் (klass >= 200)
    திரும்பு klass;

  /* Thai / Lao need some per-character work. */
  எனில் ((u & ~0xFF) == 0x0E00u)
  {
    எனில் (unlikely (klass == 0))
    {
      தேர்வு (u)
      {
	நிலை 0x0E31u:
	நிலை 0x0E34u:
	நிலை 0x0E35u:
	நிலை 0x0E36u:
	நிலை 0x0E37u:
	நிலை 0x0E47u:
	நிலை 0x0E4Cu:
	நிலை 0x0E4Du:
	நிலை 0x0E4Eu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_வலது;
	  நிறுத்து;

	நிலை 0x0EB1u:
	நிலை 0x0EB4u:
	நிலை 0x0EB5u:
	நிலை 0x0EB6u:
	நிலை 0x0EB7u:
	நிலை 0x0EBBu:
	நிலை 0x0ECCu:
	நிலை 0x0ECDu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE;
	  நிறுத்து;

	நிலை 0x0EBCu:
	  klass = HB_UNICODE_COMBINING_CLASS_BELOW;
	  நிறுத்து;
      }
    } இல்லையெனில் {
      /* Thai virama is below-right */
      எனில் (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_BELOW_வலது;
    }
  }

  தேர்வு (klass)
  {

    /* Hebrew */

    நிலை HB_MODIFIED_COMBINING_CLASS_CCC10: /* sheva */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC11: /* hataf segol */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC12: /* hataf patah */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC13: /* hataf qamats */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC14: /* hiriq */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC15: /* tsere */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC16: /* segol */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC17: /* patah */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC18: /* qamats & qamats qatan */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC20: /* qubuts */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC22: /* meteg */
      திரும்பு HB_UNICODE_COMBINING_CLASS_BELOW;

    நிலை HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      திரும்பு HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    நிலை HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      திரும்பு HB_UNICODE_COMBINING_CLASS_ABOVE_வலது;

    நிலை HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      திரும்பு HB_UNICODE_COMBINING_CLASS_ABOVE_இடது;

    நிலை HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      திரும்பு HB_UNICODE_COMBINING_CLASS_ABOVE;

    நிலை HB_MODIFIED_COMBINING_CLASS_CCC21: /* dagesh */
      நிறுத்து;


    /* Arabic and Syriac */

    நிலை HB_MODIFIED_COMBINING_CLASS_CCC27: /* fathatan */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC28: /* dammatan */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC30: /* fatha */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC31: /* damma */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC33: /* shadda */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC34: /* sukun */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC35: /* superscript alef */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC36: /* superscript alaph */
      திரும்பு HB_UNICODE_COMBINING_CLASS_ABOVE;

    நிலை HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    நிலை HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      திரும்பு HB_UNICODE_COMBINING_CLASS_BELOW;


    /* Thai */

    நிலை HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      திரும்பு HB_UNICODE_COMBINING_CLASS_BELOW_வலது;

    நிலை HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      திரும்பு HB_UNICODE_COMBINING_CLASS_ABOVE_வலது;


    /* Lao */

    நிலை HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      திரும்பு HB_UNICODE_COMBINING_CLASS_BELOW;

    நிலை HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      திரும்பு HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    நிலை HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      திரும்பு HB_UNICODE_COMBINING_CLASS_BELOW;

    நிலை HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      திரும்பு HB_UNICODE_COMBINING_CLASS_ABOVE;

    நிலை HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      திரும்பு HB_UNICODE_COMBINING_CLASS_BELOW;

  }

  திரும்பு klass;
}

வெறுமை
_hb_ot_shape_fallback_mark_position_recategorize_marks (மாறிலி hb_ot_shape_plan_t *plan HB_UNUSED,
							hb_font_t *font HB_UNUSED,
							hb_buffer_t  *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  திரும்பு;
#endif

  unsigned முழு மொத்தம் = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    எனில் (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      unsigned முழு combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      _hb_glyph_info_set_modified_combining_class (&info[i], combining_class);
    }
}


நிலையான வெறுமை
zero_mark_advances (hb_buffer_t *buffer,
		    unsigned முழு start,
		    unsigned முழு end,
		    தருக்க adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = buffer->info;
  க்கு (unsigned முழு i = start; i < end; i++)
    எனில் (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      எனில் (adjust_offsets_when_zeroing)
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

நிலையான உள்வரி வெறுமை
position_mark (மாறிலி hb_ot_shape_plan_t *plan HB_UNUSED,
	       hb_font_t *font,
	       hb_buffer_t  *buffer,
	       hb_glyph_extents_t &base_extents,
	       unsigned முழு i,
	       unsigned முழு combining_class)
{
  hb_glyph_extents_t mark_extents;
  எனில் (!font->get_glyph_extents (buffer->info[i].codepoint, &mark_extents))
    திரும்பு;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = buffer->pos[i];
  pos.x_offset = pos.y_offset = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  தேர்வு (combining_class)
  {
    நிலை HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    நிலை HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      எனில் (buffer->props.direction == HB_DIRECTION_LTR) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	நிறுத்து;
      } இல்லையெனில் எனில் (buffer->props.direction == HB_DIRECTION_RTL) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	நிறுத்து;
      }
      HB_FALLTHROUGH;

    இயல்பு:
    நிலை HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
    நிலை HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    நிலை HB_UNICODE_COMBINING_CLASS_BELOW:
    நிலை HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing +
						((int64_t) base_extents.width - mark_extents.width) / 2 -
						mark_extents.x_bearing);
      நிறுத்து;

    நிலை HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_இடது:
    நிலை HB_UNICODE_COMBINING_CLASS_BELOW_இடது:
    நிலை HB_UNICODE_COMBINING_CLASS_ABOVE_இடது:
      /* Left align. */
      pos.x_offset = hb_saturate_sub (base_extents.x_bearing, mark_extents.x_bearing);
      நிறுத்து;

    நிலை HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_வலது:
    நிலை HB_UNICODE_COMBINING_CLASS_BELOW_வலது:
    நிலை HB_UNICODE_COMBINING_CLASS_ABOVE_வலது:
      /* Right align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						mark_extents.width - mark_extents.x_bearing);
      நிறுத்து;
  }

  /* Y positioning */
  தேர்வு (combining_class)
  {
    நிலை HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    நிலை HB_UNICODE_COMBINING_CLASS_BELOW_இடது:
    நிலை HB_UNICODE_COMBINING_CLASS_BELOW:
    நிலை HB_UNICODE_COMBINING_CLASS_BELOW_வலது:
      /* Add gap, fall-through. */
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    நிலை HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_இடது:
    நிலை HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing +
						base_extents.height - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      எனில் ((y_gap > 0) == (pos.y_offset > 0))
      {
	base_extents.height = hb_saturate_sub (base_extents.height, pos.y_offset);
	pos.y_offset = 0;
      }
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      நிறுத்து;

    நிலை HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    நிலை HB_UNICODE_COMBINING_CLASS_ABOVE_இடது:
    நிலை HB_UNICODE_COMBINING_CLASS_ABOVE:
    நிலை HB_UNICODE_COMBINING_CLASS_ABOVE_வலது:
      /* Add gap, fall-through. */
      base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, y_gap);
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    நிலை HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    நிலை HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_வலது:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing -
						mark_extents.y_bearing - mark_extents.height);
      /* Don't shift down "above" marks too much. */
      எனில் ((y_gap > 0) != (pos.y_offset > 0))
      {
	hb_position_t correction = -(pos.y_offset / 2);
	base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, correction);
	base_extents.height = hb_saturate_sub (base_extents.height, correction);
	pos.y_offset = hb_saturate_add (pos.y_offset, correction);
      }
      base_extents.y_bearing = hb_saturate_sub (base_extents.y_bearing, mark_extents.height);
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      நிறுத்து;
  }
}

நிலையான உள்வரி வெறுமை
position_around_base (மாறிலி hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_buffer_t  *buffer,
		      unsigned முழு base,
		      unsigned முழு end,
		      தருக்க adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  buffer->unsafe_to_break (base, end);

  hb_glyph_extents_t base_extents;
  எனில் (!font->get_glyph_extents (buffer->info[base].codepoint,
				&base_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (buffer, base + 1, end, adjust_offsets_when_zeroing);
    திரும்பு;
  }
  base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing,
					    buffer->pos[base].y_offset);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  base_extents.x_bearing = 0;
  base_extents.width = font->get_glyph_h_advance (buffer->info[base].codepoint);

  unsigned முழு lig_id = _hb_glyph_info_get_lig_id (&buffer->info[base]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  முழு இலக்கம்_lig_components = hb_glyph_info_get_lig_இலக்கம்_comps (&buffer->info[base]);

  hb_position_t x_offset = 0, y_offset = 0;
  எனில் (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
    x_offset = hb_saturate_neg (buffer->pos[base].x_advance);
    y_offset = hb_saturate_neg (buffer->pos[base].y_advance);
  }

  hb_glyph_extents_t component_extents = base_extents;
  முழு last_lig_component = -1;
  unsigned முழு last_combining_class = 255;
  hb_glyph_extents_t cluster_extents = base_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = buffer->info;
  க்கு (unsigned முழு i = base + 1; i < end; i++)
    எனில் (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      எனில் (இலக்கம்_lig_components > 1) {
	unsigned முழு this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	முழு this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
	/* Conditions for attaching to the last component. */
	எனில் (!lig_id || lig_id != this_lig_id || this_lig_component >= இலக்கம்_lig_components)
	  this_lig_component = இலக்கம்_lig_components - 1;
	எனில் (last_lig_component != this_lig_component)
	{
	  last_lig_component = this_lig_component;
	  last_combining_class = 255;
	  component_extents = base_extents;
	  எனில் (unlikely (horiz_dir == HB_DIRECTION_INVALID)) {
	    எனில் (HB_DIRECTION_IS_HORIZONTAL (plan->props.direction))
	      horiz_dir = plan->props.direction;
	    இல்லையெனில்
	      horiz_dir = hb_script_get_horizontal_direction (plan->props.script);
	  }
	  எனில் (horiz_dir == HB_DIRECTION_LTR)
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) this_lig_component * component_extents.width / இலக்கம்_lig_components);
	  இல்லையெனில்
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) (இலக்கம்_lig_components - 1 - this_lig_component) * component_extents.width / இலக்கம்_lig_components);
	  component_extents.width /= இலக்கம்_lig_components;
	}
      }

      unsigned முழு this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      எனில் (last_combining_class != this_combining_class)
      {
	last_combining_class = this_combining_class;
	cluster_extents = component_extents;
      }

      position_mark (plan, font, buffer, cluster_extents, i, this_combining_class);

      buffer->pos[i].x_advance = 0;
      buffer->pos[i].y_advance = 0;
      buffer->pos[i].x_offset = hb_saturate_add (buffer->pos[i].x_offset, x_offset);
      buffer->pos[i].y_offset = hb_saturate_add (buffer->pos[i].y_offset, y_offset);

    } இல்லையெனில் {
      எனில் (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
	x_offset = hb_saturate_sub (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_sub (y_offset, buffer->pos[i].y_advance);
      } இல்லையெனில் {
	x_offset = hb_saturate_add (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_add (y_offset, buffer->pos[i].y_advance);
      }
    }
}

நிலையான உள்வரி வெறுமை
position_cluster_impl (மாறிலி hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t  *buffer,
		       unsigned முழு start,
		       unsigned முழு end,
		       தருக்க adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = buffer->info;
  க்கு (unsigned முழு i = start; i < end; i++)
    எனில் (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      unsigned முழு j;
      க்கு (j = i + 1; j < end; j++)
      {
        எனில் (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  தொடர்;
	எனில் (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  நிறுத்து;
      }

      position_around_base (plan, font, buffer, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

நிலையான HB_ALWAYS_INLINE வெறுமை
position_cluster (மாறிலி hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_buffer_t  *buffer,
		  unsigned முழு start,
		  unsigned முழு end,
		  தருக்க adjust_offsets_when_zeroing)
{
  எனில் (end - start < 2)
    திரும்பு;

  position_cluster_impl (plan, font, buffer, start, end, adjust_offsets_when_zeroing);
}

வெறுமை
_hb_ot_shape_fallback_mark_position (மாறிலி hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_buffer_t  *buffer,
				     தருக்க adjust_offsets_when_zeroing)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  திரும்பு;
#endif

  எனில் (!buffer->message (font, "start fallback mark"))
    திரும்பு;

  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned முழு start = 0;
  unsigned முழு மொத்தம் = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  க்கு (unsigned முழு i = 1; i < மொத்தம்; i++)
    எனில் (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, buffer, start, i, adjust_offsets_when_zeroing);
      start = i;
    }
  position_cluster (plan, font, buffer, start, மொத்தம், adjust_offsets_when_zeroing);

  (வெறுமை) buffer->message (font, "end fallback mark");
}


#ifndef HB_DISABLE_DEPRECATED
கட்டமைப்பு hb_ot_shape_fallback_kern_driver_t
{
  hb_ot_shape_fallback_kern_driver_t (hb_font_t   *font_,
				      hb_buffer_t *buffer) :
    font (font_), direction (buffer->props.direction) {}

  hb_position_t get_kerning (hb_codepoint_t first, hb_codepoint_t second) மாறிலி
  {
    hb_position_t kern = 0;
    font->get_glyph_kerning_for_direction (first, second,
					   direction,
					   &kern, &kern);
    திரும்பு kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#endif

/* Performs font-assisted kerning. */
வெறுமை
_hb_ot_shape_fallback_kern (மாறிலி hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  திரும்பு;
#endif

#ifndef HB_DISABLE_DEPRECATED
  எனில் (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    திரும்பு;

  எனில் (!buffer->message (font, "start fallback kern"))
    திரும்பு;

  தருக்க reverse = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  எனில் (reverse)
    buffer->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, buffer);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, buffer, plan->kern_mask, பொய்);

  எனில் (reverse)
    buffer->reverse ();

  (வெறுமை) buffer->message (font, "end fallback kern");
#endif
}


/* Adjusts width of various spaces. */
வெறுமை
_hb_ot_shape_fallback_spaces (மாறிலி hb_ot_shape_plan_t *plan HB_UNUSED,
			      hb_font_t *font,
			      hb_buffer_t  *buffer)
{
  hb_glyph_info_t *info = buffer->info;
  hb_glyph_position_t *pos = buffer->pos;
  தருக்க horizontal = HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction);
  unsigned முழு மொத்தம் = buffer->len;
  க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    எனில் (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      எனில் (buffer->invisible && info[i].codepoint == buffer->invisible)
      {
        எனில் (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        இல்லையெனில்
	  pos[i].y_advance = -(int64_t) font->y_scale / 4;
      }

      hb_unicode_funcs_t::space_t space_type = _hb_glyph_info_get_unicode_space_fallback_type (&info[i]);
      hb_codepoint_t glyph;
      வகைவரை hb_unicode_funcs_t t;
      தேர்வு (space_type)
      {
	நிலை t::NOT_SPACE: /* Shouldn't happen. */
	நிலை t::SPACE:
	  நிறுத்து;

	நிலை t::SPACE_EM:
	நிலை t::SPACE_EM_2:
	நிலை t::SPACE_EM_3:
	நிலை t::SPACE_EM_4:
	நிலை t::SPACE_EM_5:
	நிலை t::SPACE_EM_6:
	நிலை t::SPACE_EM_16:
	  எனில் (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((முழு) space_type)/2) / (முழு) space_type;
	  இல்லையெனில்
	    pos[i].y_advance = -((int64_t) font->y_scale + ((முழு) space_type)/2) / (முழு) space_type;
	  நிறுத்து;

	நிலை t::SPACE_4_EM_18:
	  எனில் (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  இல்லையெனில்
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  நிறுத்து;

	நிலை t::SPACE_FIGURE:
	  க்கு (எழுத்து u = '0'; u <= '9'; u++)
	    எனில் (font->get_nominal_glyph (u, &glyph))
	    {
	      எனில் (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      இல்லையெனில்
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      நிறுத்து;
	    }
	  நிறுத்து;

	நிலை t::SPACE_PUNCTUATION:
	  எனில் (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    எனில் (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    இல்லையெனில்
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  நிறுத்து;

	நிலை t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  எனில் (horizontal)
	    pos[i].x_advance /= 2;
	  இல்லையெனில்
	    pos[i].y_advance /= 2;
	  நிறுத்து;
      }
    }
}


#endif
