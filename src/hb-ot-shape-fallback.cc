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

#ضمن "hb.hh"

#ifndef HB_NO_OT_SHAPE

#ضمن "hb-ot-shape-fallback.hh"
#ضمن "hb-kern.hh"

ساكن unsigned صحيح
recategorize_combining_class (hb_codepoint_t u,
			      unsigned صحيح klass)
{
  ܐܢ (klass >= 200)
    ܦܢܐ klass;

  /* Thai / Lao need some per-character work. */
  ܐܢ ((u & ~0xFF) == 0x0E00u)
  {
    ܐܢ (unlikely (klass == 0))
    {
      بدل (u)
      {
	حالة 0x0E31u:
	حالة 0x0E34u:
	حالة 0x0E35u:
	حالة 0x0E36u:
	حالة 0x0E37u:
	حالة 0x0E47u:
	حالة 0x0E4Cu:
	حالة 0x0E4Du:
	حالة 0x0E4Eu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_RIGHT;
	  اكسر;

	حالة 0x0EB1u:
	حالة 0x0EB4u:
	حالة 0x0EB5u:
	حالة 0x0EB6u:
	حالة 0x0EB7u:
	حالة 0x0EBBu:
	حالة 0x0ECCu:
	حالة 0x0ECDu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE;
	  اكسر;

	حالة 0x0EBCu:
	  klass = HB_UNICODE_COMBINING_CLASS_BELOW;
	  اكسر;
      }
    } ܐܚܪܢܐ {
      /* Thai virama is below-right */
      ܐܢ (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_BELOW_RIGHT;
    }
  }

  بدل (klass)
  {

    /* Hebrew */

    حالة HB_MODIFIED_COMBINING_CLASS_CCC10: /* sheva */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC11: /* hataf segol */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC12: /* hataf patah */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC13: /* hataf qamats */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC14: /* hiriq */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC15: /* tsere */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC16: /* segol */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC17: /* patah */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC18: /* qamats & qamats qatan */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC20: /* qubuts */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC22: /* meteg */
      ܦܢܐ HB_UNICODE_COMBINING_CLASS_BELOW;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      ܦܢܐ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      ܦܢܐ HB_UNICODE_COMBINING_CLASS_ABOVE_RIGHT;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      ܦܢܐ HB_UNICODE_COMBINING_CLASS_ABOVE_LEFT;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      ܦܢܐ HB_UNICODE_COMBINING_CLASS_ABOVE;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC21: /* dagesh */
      اكسر;


    /* Arabic and Syriac */

    حالة HB_MODIFIED_COMBINING_CLASS_CCC27: /* fathatan */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC28: /* dammatan */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC30: /* fatha */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC31: /* damma */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC33: /* shadda */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC34: /* sukun */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC35: /* superscript alef */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC36: /* superscript alaph */
      ܦܢܐ HB_UNICODE_COMBINING_CLASS_ABOVE;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      ܦܢܐ HB_UNICODE_COMBINING_CLASS_BELOW;


    /* Thai */

    حالة HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      ܦܢܐ HB_UNICODE_COMBINING_CLASS_BELOW_RIGHT;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      ܦܢܐ HB_UNICODE_COMBINING_CLASS_ABOVE_RIGHT;


    /* Lao */

    حالة HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      ܦܢܐ HB_UNICODE_COMBINING_CLASS_BELOW;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      ܦܢܐ HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    حالة HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      ܦܢܐ HB_UNICODE_COMBINING_CLASS_BELOW;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      ܦܢܐ HB_UNICODE_COMBINING_CLASS_ABOVE;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      ܦܢܐ HB_UNICODE_COMBINING_CLASS_BELOW;

  }

  ܦܢܐ klass;
}

فراغ
_hb_ot_shape_fallback_mark_position_recategorize_marks (ثابت hb_ot_shape_plan_t *plan HB_UNUSED,
							hb_font_t *font HB_UNUSED,
							hb_buffer_t  *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  ܦܢܐ;
#endif

  unsigned صحيح count = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  لكل (unsigned صحيح i = 0; i < count; i++)
    ܐܢ (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      unsigned صحيح combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      _hb_glyph_info_set_modified_combining_class (&info[i], combining_class);
    }
}


ساكن فراغ
zero_mark_advances (hb_buffer_t *buffer,
		    unsigned صحيح start,
		    unsigned صحيح end,
		    منطقي adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = buffer->info;
  لكل (unsigned صحيح i = start; i < end; i++)
    ܐܢ (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      ܐܢ (adjust_offsets_when_zeroing)
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

ساكن مضمن فراغ
position_mark (ثابت hb_ot_shape_plan_t *plan HB_UNUSED,
	       hb_font_t *font,
	       hb_buffer_t  *buffer,
	       hb_glyph_extents_t &base_extents,
	       unsigned صحيح i,
	       unsigned صحيح combining_class)
{
  hb_glyph_extents_t mark_extents;
  ܐܢ (!font->get_glyph_extents (buffer->info[i].codepoint, &mark_extents))
    ܦܢܐ;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = buffer->pos[i];
  pos.x_offset = pos.y_offset = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  بدل (combining_class)
  {
    حالة HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    حالة HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      ܐܢ (buffer->props.direction == HB_DIRECTION_LTR) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	اكسر;
      } ܐܚܪܢܐ ܐܢ (buffer->props.direction == HB_DIRECTION_RTL) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	اكسر;
      }
      HB_FALLTHROUGH;

    افتراضي:
    حالة HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
    حالة HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    حالة HB_UNICODE_COMBINING_CLASS_BELOW:
    حالة HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing +
						((int64_t) base_extents.width - mark_extents.width) / 2 -
						mark_extents.x_bearing);
      اكسر;

    حالة HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_LEFT:
    حالة HB_UNICODE_COMBINING_CLASS_BELOW_LEFT:
    حالة HB_UNICODE_COMBINING_CLASS_ABOVE_LEFT:
      /* Left align. */
      pos.x_offset = hb_saturate_sub (base_extents.x_bearing, mark_extents.x_bearing);
      اكسر;

    حالة HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_RIGHT:
    حالة HB_UNICODE_COMBINING_CLASS_BELOW_RIGHT:
    حالة HB_UNICODE_COMBINING_CLASS_ABOVE_RIGHT:
      /* Right align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						mark_extents.width - mark_extents.x_bearing);
      اكسر;
  }

  /* Y positioning */
  بدل (combining_class)
  {
    حالة HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    حالة HB_UNICODE_COMBINING_CLASS_BELOW_LEFT:
    حالة HB_UNICODE_COMBINING_CLASS_BELOW:
    حالة HB_UNICODE_COMBINING_CLASS_BELOW_RIGHT:
      /* Add gap, fall-through. */
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    حالة HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_LEFT:
    حالة HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing +
						base_extents.height - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      ܐܢ ((y_gap > 0) == (pos.y_offset > 0))
      {
	base_extents.height = hb_saturate_sub (base_extents.height, pos.y_offset);
	pos.y_offset = 0;
      }
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      اكسر;

    حالة HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    حالة HB_UNICODE_COMBINING_CLASS_ABOVE_LEFT:
    حالة HB_UNICODE_COMBINING_CLASS_ABOVE:
    حالة HB_UNICODE_COMBINING_CLASS_ABOVE_RIGHT:
      /* Add gap, fall-through. */
      base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, y_gap);
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    حالة HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    حالة HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_RIGHT:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing -
						mark_extents.y_bearing - mark_extents.height);
      /* Don't shift down "above" marks too much. */
      ܐܢ ((y_gap > 0) != (pos.y_offset > 0))
      {
	hb_position_t correction = -(pos.y_offset / 2);
	base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, correction);
	base_extents.height = hb_saturate_sub (base_extents.height, correction);
	pos.y_offset = hb_saturate_add (pos.y_offset, correction);
      }
      base_extents.y_bearing = hb_saturate_sub (base_extents.y_bearing, mark_extents.height);
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      اكسر;
  }
}

ساكن مضمن فراغ
position_around_base (ثابت hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_buffer_t  *buffer,
		      unsigned صحيح base,
		      unsigned صحيح end,
		      منطقي adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  buffer->unsafe_to_break (base, end);

  hb_glyph_extents_t base_extents;
  ܐܢ (!font->get_glyph_extents (buffer->info[base].codepoint,
				&base_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (buffer, base + 1, end, adjust_offsets_when_zeroing);
    ܦܢܐ;
  }
  base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing,
					    buffer->pos[base].y_offset);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  base_extents.x_bearing = 0;
  base_extents.width = font->get_glyph_h_advance (buffer->info[base].codepoint);

  unsigned صحيح lig_id = _hb_glyph_info_get_lig_id (&buffer->info[base]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  صحيح num_lig_components = _hb_glyph_info_get_lig_num_comps (&buffer->info[base]);

  hb_position_t x_offset = 0, y_offset = 0;
  ܐܢ (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
    x_offset = hb_saturate_neg (buffer->pos[base].x_advance);
    y_offset = hb_saturate_neg (buffer->pos[base].y_advance);
  }

  hb_glyph_extents_t component_extents = base_extents;
  صحيح last_lig_component = -1;
  unsigned صحيح last_combining_class = 255;
  hb_glyph_extents_t cluster_extents = base_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = buffer->info;
  لكل (unsigned صحيح i = base + 1; i < end; i++)
    ܐܢ (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      ܐܢ (num_lig_components > 1) {
	unsigned صحيح this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	صحيح this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
	/* Conditions for attaching to the last component. */
	ܐܢ (!lig_id || lig_id != this_lig_id || this_lig_component >= num_lig_components)
	  this_lig_component = num_lig_components - 1;
	ܐܢ (last_lig_component != this_lig_component)
	{
	  last_lig_component = this_lig_component;
	  last_combining_class = 255;
	  component_extents = base_extents;
	  ܐܢ (unlikely (horiz_dir == HB_DIRECTION_INVALID)) {
	    ܐܢ (HB_DIRECTION_IS_HORIZONTAL (plan->props.direction))
	      horiz_dir = plan->props.direction;
	    ܐܚܪܢܐ
	      horiz_dir = hb_script_get_horizontal_direction (plan->props.script);
	  }
	  ܐܢ (horiz_dir == HB_DIRECTION_LTR)
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) this_lig_component * component_extents.width / num_lig_components);
	  ܐܚܪܢܐ
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) (num_lig_components - 1 - this_lig_component) * component_extents.width / num_lig_components);
	  component_extents.width /= num_lig_components;
	}
      }

      unsigned صحيح this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      ܐܢ (last_combining_class != this_combining_class)
      {
	last_combining_class = this_combining_class;
	cluster_extents = component_extents;
      }

      position_mark (plan, font, buffer, cluster_extents, i, this_combining_class);

      buffer->pos[i].x_advance = 0;
      buffer->pos[i].y_advance = 0;
      buffer->pos[i].x_offset = hb_saturate_add (buffer->pos[i].x_offset, x_offset);
      buffer->pos[i].y_offset = hb_saturate_add (buffer->pos[i].y_offset, y_offset);

    } ܐܚܪܢܐ {
      ܐܢ (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
	x_offset = hb_saturate_sub (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_sub (y_offset, buffer->pos[i].y_advance);
      } ܐܚܪܢܐ {
	x_offset = hb_saturate_add (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_add (y_offset, buffer->pos[i].y_advance);
      }
    }
}

ساكن مضمن فراغ
position_cluster_impl (ثابت hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t  *buffer,
		       unsigned صحيح start,
		       unsigned صحيح end,
		       منطقي adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = buffer->info;
  لكل (unsigned صحيح i = start; i < end; i++)
    ܐܢ (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      unsigned صحيح j;
      لكل (j = i + 1; j < end; j++)
      {
        ܐܢ (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  استمر;
	ܐܢ (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  اكسر;
      }

      position_around_base (plan, font, buffer, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

ساكن HB_ALWAYS_INLINE فراغ
position_cluster (ثابت hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_buffer_t  *buffer,
		  unsigned صحيح start,
		  unsigned صحيح end,
		  منطقي adjust_offsets_when_zeroing)
{
  ܐܢ (end - start < 2)
    ܦܢܐ;

  position_cluster_impl (plan, font, buffer, start, end, adjust_offsets_when_zeroing);
}

فراغ
_hb_ot_shape_fallback_mark_position (ثابت hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_buffer_t  *buffer,
				     منطقي adjust_offsets_when_zeroing)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  ܦܢܐ;
#endif

  ܐܢ (!buffer->message (font, "start fallback mark"))
    ܦܢܐ;

  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned صحيح start = 0;
  unsigned صحيح count = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  لكل (unsigned صحيح i = 1; i < count; i++)
    ܐܢ (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, buffer, start, i, adjust_offsets_when_zeroing);
      start = i;
    }
  position_cluster (plan, font, buffer, start, count, adjust_offsets_when_zeroing);

  (فراغ) buffer->message (font, "end fallback mark");
}


#ifndef HB_DISABLE_DEPRECATED
بنية hb_ot_shape_fallback_kern_driver_t
{
  hb_ot_shape_fallback_kern_driver_t (hb_font_t   *font_,
				      hb_buffer_t *buffer) :
    font (font_), direction (buffer->props.direction) {}

  hb_position_t get_kerning (hb_codepoint_t first, hb_codepoint_t second) ثابت
  {
    hb_position_t kern = 0;
    font->get_glyph_kerning_for_direction (first, second,
					   direction,
					   &kern, &kern);
    ܦܢܐ kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#endif

/* Performs font-assisted kerning. */
فراغ
_hb_ot_shape_fallback_kern (ثابت hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  ܦܢܐ;
#endif

#ifndef HB_DISABLE_DEPRECATED
  ܐܢ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    ܦܢܐ;

  ܐܢ (!buffer->message (font, "start fallback kern"))
    ܦܢܐ;

  منطقي reverse = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  ܐܢ (reverse)
    buffer->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, buffer);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, buffer, plan->kern_mask, ܕܓܠܐ);

  ܐܢ (reverse)
    buffer->reverse ();

  (فراغ) buffer->message (font, "end fallback kern");
#endif
}


/* Adjusts width of various spaces. */
فراغ
_hb_ot_shape_fallback_spaces (ثابت hb_ot_shape_plan_t *plan HB_UNUSED,
			      hb_font_t *font,
			      hb_buffer_t  *buffer)
{
  hb_glyph_info_t *info = buffer->info;
  hb_glyph_position_t *pos = buffer->pos;
  منطقي horizontal = HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction);
  unsigned صحيح count = buffer->len;
  لكل (unsigned صحيح i = 0; i < count; i++)
    ܐܢ (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      ܐܢ (buffer->invisible && info[i].codepoint == buffer->invisible)
      {
        ܐܢ (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        ܐܚܪܢܐ
	  pos[i].y_advance = -(int64_t) font->y_scale / 4;
      }

      hb_unicode_funcs_t::space_t space_type = _hb_glyph_info_get_unicode_space_fallback_type (&info[i]);
      hb_codepoint_t glyph;
      تعريفنوع hb_unicode_funcs_t t;
      بدل (space_type)
      {
	حالة t::NOT_SPACE: /* Shouldn't happen. */
	حالة t::SPACE:
	  اكسر;

	حالة t::SPACE_EM:
	حالة t::SPACE_EM_2:
	حالة t::SPACE_EM_3:
	حالة t::SPACE_EM_4:
	حالة t::SPACE_EM_5:
	حالة t::SPACE_EM_6:
	حالة t::SPACE_EM_16:
	  ܐܢ (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((صحيح) space_type)/2) / (صحيح) space_type;
	  ܐܚܪܢܐ
	    pos[i].y_advance = -((int64_t) font->y_scale + ((صحيح) space_type)/2) / (صحيح) space_type;
	  اكسر;

	حالة t::SPACE_4_EM_18:
	  ܐܢ (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  ܐܚܪܢܐ
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  اكسر;

	حالة t::SPACE_FIGURE:
	  لكل (حرف u = '0'; u <= '9'; u++)
	    ܐܢ (font->get_nominal_glyph (u, &glyph))
	    {
	      ܐܢ (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      ܐܚܪܢܐ
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      اكسر;
	    }
	  اكسر;

	حالة t::SPACE_PUNCTUATION:
	  ܐܢ (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    ܐܢ (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    ܐܚܪܢܐ
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  اكسر;

	حالة t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  ܐܢ (horizontal)
	    pos[i].x_advance /= 2;
	  ܐܚܪܢܐ
	    pos[i].y_advance /= 2;
	  اكسر;
      }
    }
}


#endif
