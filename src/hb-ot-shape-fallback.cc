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

#شامل "hb.hh"

#ifndef HB_NO_OT_SHAPE

#شامل "hb-ot-shape-fallback.hh"
#شامل "hb-kern.hh"

ایستا unsigned صحیح
recategorize_combining_class (hb_codepoint_t u,
			      unsigned صحیح klass)
{
  اگر (klass >= 200)
    بازگشت klass;

  /* Thai / Lao need some per-character work. */
  اگر ((u & ~0xFF) == 0x0E00u)
  {
    اگر (unlikely (klass == 0))
    {
      انتخاب (u)
      {
	حالت 0x0E31u:
	حالت 0x0E34u:
	حالت 0x0E35u:
	حالت 0x0E36u:
	حالت 0x0E37u:
	حالت 0x0E47u:
	حالت 0x0E4Cu:
	حالت 0x0E4Du:
	حالت 0x0E4Eu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_راست;
	  بشکن;

	حالت 0x0EB1u:
	حالت 0x0EB4u:
	حالت 0x0EB5u:
	حالت 0x0EB6u:
	حالت 0x0EB7u:
	حالت 0x0EBBu:
	حالت 0x0ECCu:
	حالت 0x0ECDu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE;
	  بشکن;

	حالت 0x0EBCu:
	  klass = HB_UNICODE_COMBINING_CLASS_BELOW;
	  بشکن;
      }
    } وگرنه {
      /* Thai virama is below-right */
      اگر (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_BELOW_راست;
    }
  }

  انتخاب (klass)
  {

    /* Hebrew */

    حالت HB_MODIFIED_COMBINING_CLASS_CCC10: /* sheva */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC11: /* hataf segol */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC12: /* hataf patah */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC13: /* hataf qamats */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC14: /* hiriq */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC15: /* tsere */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC16: /* segol */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC17: /* patah */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC18: /* qamats & qamats qatan */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC20: /* qubuts */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC22: /* meteg */
      بازگشت HB_UNICODE_COMBINING_CLASS_BELOW;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      بازگشت HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      بازگشت HB_UNICODE_COMBINING_CLASS_ABOVE_راست;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      بازگشت HB_UNICODE_COMBINING_CLASS_ABOVE_چپ;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      بازگشت HB_UNICODE_COMBINING_CLASS_ABOVE;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC21: /* dagesh */
      بشکن;


    /* Arabic and Syriac */

    حالت HB_MODIFIED_COMBINING_CLASS_CCC27: /* fathatan */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC28: /* dammatan */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC30: /* fatha */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC31: /* damma */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC33: /* shadda */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC34: /* sukun */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC35: /* superscript alef */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC36: /* superscript alaph */
      بازگشت HB_UNICODE_COMBINING_CLASS_ABOVE;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      بازگشت HB_UNICODE_COMBINING_CLASS_BELOW;


    /* Thai */

    حالت HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      بازگشت HB_UNICODE_COMBINING_CLASS_BELOW_راست;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      بازگشت HB_UNICODE_COMBINING_CLASS_ABOVE_راست;


    /* Lao */

    حالت HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      بازگشت HB_UNICODE_COMBINING_CLASS_BELOW;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      بازگشت HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    حالت HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      بازگشت HB_UNICODE_COMBINING_CLASS_BELOW;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      بازگشت HB_UNICODE_COMBINING_CLASS_ABOVE;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      بازگشت HB_UNICODE_COMBINING_CLASS_BELOW;

  }

  بازگشت klass;
}

تهی
_hb_ot_shape_fallback_mark_position_recategorize_marks (ثابت hb_ot_shape_plan_t *plan HB_UNUSED,
							hb_font_t *font HB_UNUSED,
							hb_buffer_t  *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  بازگشت;
#endif

  unsigned صحیح شمار = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  برای (unsigned صحیح i = 0; i < شمار; i++)
    اگر (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      unsigned صحیح combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      _hb_glyph_info_set_modified_combining_class (&info[i], combining_class);
    }
}


ایستا تهی
zero_mark_advances (hb_buffer_t *buffer,
		    unsigned صحیح start,
		    unsigned صحیح end,
		    منطقی adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = buffer->info;
  برای (unsigned صحیح i = start; i < end; i++)
    اگر (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      اگر (adjust_offsets_when_zeroing)
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

ایستا درونخطی تهی
position_mark (ثابت hb_ot_shape_plan_t *plan HB_UNUSED,
	       hb_font_t *font,
	       hb_buffer_t  *buffer,
	       hb_glyph_extents_t &base_extents,
	       unsigned صحیح i,
	       unsigned صحیح combining_class)
{
  hb_glyph_extents_t mark_extents;
  اگر (!font->get_glyph_extents (buffer->info[i].codepoint, &mark_extents))
    بازگشت;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = buffer->pos[i];
  pos.x_offset = pos.y_offset = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  انتخاب (combining_class)
  {
    حالت HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    حالت HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      اگر (buffer->props.direction == HB_DIRECTION_LTR) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	بشکن;
      } وگرنه اگر (buffer->props.direction == HB_DIRECTION_RTL) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	بشکن;
      }
      HB_FALLTHROUGH;

    پیشفرض:
    حالت HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
    حالت HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    حالت HB_UNICODE_COMBINING_CLASS_BELOW:
    حالت HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing +
						((int64_t) base_extents.width - mark_extents.width) / 2 -
						mark_extents.x_bearing);
      بشکن;

    حالت HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_چپ:
    حالت HB_UNICODE_COMBINING_CLASS_BELOW_چپ:
    حالت HB_UNICODE_COMBINING_CLASS_ABOVE_چپ:
      /* Left align. */
      pos.x_offset = hb_saturate_sub (base_extents.x_bearing, mark_extents.x_bearing);
      بشکن;

    حالت HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_راست:
    حالت HB_UNICODE_COMBINING_CLASS_BELOW_راست:
    حالت HB_UNICODE_COMBINING_CLASS_ABOVE_راست:
      /* Right align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						mark_extents.width - mark_extents.x_bearing);
      بشکن;
  }

  /* Y positioning */
  انتخاب (combining_class)
  {
    حالت HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    حالت HB_UNICODE_COMBINING_CLASS_BELOW_چپ:
    حالت HB_UNICODE_COMBINING_CLASS_BELOW:
    حالت HB_UNICODE_COMBINING_CLASS_BELOW_راست:
      /* Add gap, fall-through. */
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    حالت HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_چپ:
    حالت HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing +
						base_extents.height - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      اگر ((y_gap > 0) == (pos.y_offset > 0))
      {
	base_extents.height = hb_saturate_sub (base_extents.height, pos.y_offset);
	pos.y_offset = 0;
      }
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      بشکن;

    حالت HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    حالت HB_UNICODE_COMBINING_CLASS_ABOVE_چپ:
    حالت HB_UNICODE_COMBINING_CLASS_ABOVE:
    حالت HB_UNICODE_COMBINING_CLASS_ABOVE_راست:
      /* Add gap, fall-through. */
      base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, y_gap);
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    حالت HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    حالت HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_راست:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing -
						mark_extents.y_bearing - mark_extents.height);
      /* Don't shift down "above" marks too much. */
      اگر ((y_gap > 0) != (pos.y_offset > 0))
      {
	hb_position_t correction = -(pos.y_offset / 2);
	base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, correction);
	base_extents.height = hb_saturate_sub (base_extents.height, correction);
	pos.y_offset = hb_saturate_add (pos.y_offset, correction);
      }
      base_extents.y_bearing = hb_saturate_sub (base_extents.y_bearing, mark_extents.height);
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      بشکن;
  }
}

ایستا درونخطی تهی
position_around_base (ثابت hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_buffer_t  *buffer,
		      unsigned صحیح base,
		      unsigned صحیح end,
		      منطقی adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  buffer->unsafe_to_break (base, end);

  hb_glyph_extents_t base_extents;
  اگر (!font->get_glyph_extents (buffer->info[base].codepoint,
				&base_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (buffer, base + 1, end, adjust_offsets_when_zeroing);
    بازگشت;
  }
  base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing,
					    buffer->pos[base].y_offset);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  base_extents.x_bearing = 0;
  base_extents.width = font->get_glyph_h_advance (buffer->info[base].codepoint);

  unsigned صحیح lig_id = _hb_glyph_info_get_lig_id (&buffer->info[base]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  صحیح رقم_lig_components = hb_glyph_info_get_lig_رقم_comps (&buffer->info[base]);

  hb_position_t x_offset = 0, y_offset = 0;
  اگر (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
    x_offset = hb_saturate_neg (buffer->pos[base].x_advance);
    y_offset = hb_saturate_neg (buffer->pos[base].y_advance);
  }

  hb_glyph_extents_t component_extents = base_extents;
  صحیح last_lig_component = -1;
  unsigned صحیح last_combining_class = 255;
  hb_glyph_extents_t cluster_extents = base_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = buffer->info;
  برای (unsigned صحیح i = base + 1; i < end; i++)
    اگر (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      اگر (رقم_lig_components > 1) {
	unsigned صحیح this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	صحیح this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
	/* Conditions for attaching to the last component. */
	اگر (!lig_id || lig_id != this_lig_id || this_lig_component >= رقم_lig_components)
	  this_lig_component = رقم_lig_components - 1;
	اگر (last_lig_component != this_lig_component)
	{
	  last_lig_component = this_lig_component;
	  last_combining_class = 255;
	  component_extents = base_extents;
	  اگر (unlikely (horiz_dir == HB_DIRECTION_INVALID)) {
	    اگر (HB_DIRECTION_IS_HORIZONTAL (plan->props.direction))
	      horiz_dir = plan->props.direction;
	    وگرنه
	      horiz_dir = hb_script_get_horizontal_direction (plan->props.script);
	  }
	  اگر (horiz_dir == HB_DIRECTION_LTR)
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) this_lig_component * component_extents.width / رقم_lig_components);
	  وگرنه
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) (رقم_lig_components - 1 - this_lig_component) * component_extents.width / رقم_lig_components);
	  component_extents.width /= رقم_lig_components;
	}
      }

      unsigned صحیح this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      اگر (last_combining_class != this_combining_class)
      {
	last_combining_class = this_combining_class;
	cluster_extents = component_extents;
      }

      position_mark (plan, font, buffer, cluster_extents, i, this_combining_class);

      buffer->pos[i].x_advance = 0;
      buffer->pos[i].y_advance = 0;
      buffer->pos[i].x_offset = hb_saturate_add (buffer->pos[i].x_offset, x_offset);
      buffer->pos[i].y_offset = hb_saturate_add (buffer->pos[i].y_offset, y_offset);

    } وگرنه {
      اگر (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
	x_offset = hb_saturate_sub (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_sub (y_offset, buffer->pos[i].y_advance);
      } وگرنه {
	x_offset = hb_saturate_add (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_add (y_offset, buffer->pos[i].y_advance);
      }
    }
}

ایستا درونخطی تهی
position_cluster_impl (ثابت hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t  *buffer,
		       unsigned صحیح start,
		       unsigned صحیح end,
		       منطقی adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = buffer->info;
  برای (unsigned صحیح i = start; i < end; i++)
    اگر (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      unsigned صحیح j;
      برای (j = i + 1; j < end; j++)
      {
        اگر (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  ادامه;
	اگر (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  بشکن;
      }

      position_around_base (plan, font, buffer, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

ایستا HB_ALWAYS_INLINE تهی
position_cluster (ثابت hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_buffer_t  *buffer,
		  unsigned صحیح start,
		  unsigned صحیح end,
		  منطقی adjust_offsets_when_zeroing)
{
  اگر (end - start < 2)
    بازگشت;

  position_cluster_impl (plan, font, buffer, start, end, adjust_offsets_when_zeroing);
}

تهی
_hb_ot_shape_fallback_mark_position (ثابت hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_buffer_t  *buffer,
				     منطقی adjust_offsets_when_zeroing)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  بازگشت;
#endif

  اگر (!buffer->message (font, "start fallback mark"))
    بازگشت;

  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned صحیح start = 0;
  unsigned صحیح شمار = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  برای (unsigned صحیح i = 1; i < شمار; i++)
    اگر (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, buffer, start, i, adjust_offsets_when_zeroing);
      start = i;
    }
  position_cluster (plan, font, buffer, start, شمار, adjust_offsets_when_zeroing);

  (تهی) buffer->message (font, "end fallback mark");
}


#ifndef HB_DISABLE_DEPRECATED
ساختار hb_ot_shape_fallback_kern_driver_t
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
    بازگشت kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#endif

/* Performs font-assisted kerning. */
تهی
_hb_ot_shape_fallback_kern (ثابت hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  بازگشت;
#endif

#ifndef HB_DISABLE_DEPRECATED
  اگر (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    بازگشت;

  اگر (!buffer->message (font, "start fallback kern"))
    بازگشت;

  منطقی reverse = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  اگر (reverse)
    buffer->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, buffer);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, buffer, plan->kern_mask, نادرست);

  اگر (reverse)
    buffer->reverse ();

  (تهی) buffer->message (font, "end fallback kern");
#endif
}


/* Adjusts width of various spaces. */
تهی
_hb_ot_shape_fallback_spaces (ثابت hb_ot_shape_plan_t *plan HB_UNUSED,
			      hb_font_t *font,
			      hb_buffer_t  *buffer)
{
  hb_glyph_info_t *info = buffer->info;
  hb_glyph_position_t *pos = buffer->pos;
  منطقی horizontal = HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction);
  unsigned صحیح شمار = buffer->len;
  برای (unsigned صحیح i = 0; i < شمار; i++)
    اگر (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      اگر (buffer->invisible && info[i].codepoint == buffer->invisible)
      {
        اگر (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        وگرنه
	  pos[i].y_advance = -(int64_t) font->y_scale / 4;
      }

      hb_unicode_funcs_t::space_t space_type = _hb_glyph_info_get_unicode_space_fallback_type (&info[i]);
      hb_codepoint_t glyph;
      تعریفنوع hb_unicode_funcs_t t;
      انتخاب (space_type)
      {
	حالت t::NOT_SPACE: /* Shouldn't happen. */
	حالت t::SPACE:
	  بشکن;

	حالت t::SPACE_EM:
	حالت t::SPACE_EM_2:
	حالت t::SPACE_EM_3:
	حالت t::SPACE_EM_4:
	حالت t::SPACE_EM_5:
	حالت t::SPACE_EM_6:
	حالت t::SPACE_EM_16:
	  اگر (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((صحیح) space_type)/2) / (صحیح) space_type;
	  وگرنه
	    pos[i].y_advance = -((int64_t) font->y_scale + ((صحیح) space_type)/2) / (صحیح) space_type;
	  بشکن;

	حالت t::SPACE_4_EM_18:
	  اگر (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  وگرنه
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  بشکن;

	حالت t::SPACE_FIGURE:
	  برای (نویسه u = '0'; u <= '9'; u++)
	    اگر (font->get_nominal_glyph (u, &glyph))
	    {
	      اگر (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      وگرنه
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      بشکن;
	    }
	  بشکن;

	حالت t::SPACE_PUNCTUATION:
	  اگر (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    اگر (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    وگرنه
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  بشکن;

	حالت t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  اگر (horizontal)
	    pos[i].x_advance /= 2;
	  وگرنه
	    pos[i].y_advance /= 2;
	  بشکن;
      }
    }
}


#endif
