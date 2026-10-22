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

#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE

#شامل "hb-ot-shape-fallback.hh"
#شامل "hb-kern.hh"

ساکن غیرعلامتی صحیح
recategorize_combining_class (hb_codepoint_t u,
			      غیرعلامتی صحیح klass)
{
  اگر (klass >= 200)
    واپس klass;

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
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_دایاں;
	  توڑو;

	حالت 0x0EB1u:
	حالت 0x0EB4u:
	حالت 0x0EB5u:
	حالت 0x0EB6u:
	حالت 0x0EB7u:
	حالت 0x0EBBu:
	حالت 0x0ECCu:
	حالت 0x0ECDu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE;
	  توڑو;

	حالت 0x0EBCu:
	  klass = HB_UNICODE_COMBINING_CLASS_نیچے;
	  توڑو;
      }
    } ورنہ {
      /* Thai virama is below-right */
      اگر (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_نیچے_دایاں;
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
      واپس HB_UNICODE_COMBINING_CLASS_نیچے;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      واپس HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      واپس HB_UNICODE_COMBINING_CLASS_ABOVE_دایاں;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      واپس HB_UNICODE_COMBINING_CLASS_ABOVE_بایاں;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      واپس HB_UNICODE_COMBINING_CLASS_ABOVE;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC21: /* dagesh */
      توڑو;


    /* Arabic and Syriac */

    حالت HB_MODIFIED_COMBINING_CLASS_CCC27: /* fathatan */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC28: /* dammatan */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC30: /* fatha */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC31: /* damma */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC33: /* shadda */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC34: /* sukun */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC35: /* superscript alef */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC36: /* superscript alaph */
      واپس HB_UNICODE_COMBINING_CLASS_ABOVE;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    حالت HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      واپس HB_UNICODE_COMBINING_CLASS_نیچے;


    /* Thai */

    حالت HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      واپس HB_UNICODE_COMBINING_CLASS_نیچے_دایاں;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      واپس HB_UNICODE_COMBINING_CLASS_ABOVE_دایاں;


    /* Lao */

    حالت HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      واپس HB_UNICODE_COMBINING_CLASS_نیچے;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      واپس HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    حالت HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      واپس HB_UNICODE_COMBINING_CLASS_نیچے;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      واپس HB_UNICODE_COMBINING_CLASS_ABOVE;

    حالت HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      واپس HB_UNICODE_COMBINING_CLASS_نیچے;

  }

  واپس klass;
}

خالی
_hb_ot_shape_fallback_mark_position_recategorize_marks (ثابت hb_ot_shape_plan_t *plan HB_غیر_مستعمل,
							hb_font_t *font HB_غیر_مستعمل,
							hb_بفر_t  *بفر)
{
#اگر_تعریف_شدہ HB_NO_OT_SHAPE_FALLBACK
  واپس;
#اگر_ختم

  غیرعلامتی صحیح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    اگر (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      غیرعلامتی صحیح combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      hb_glyph_info_سیٹ_modified_combining_class (&info[i], combining_class);
    }
}


ساکن خالی
zero_mark_advances (hb_بفر_t *بفر,
		    غیرعلامتی صحیح آغاز,
		    غیرعلامتی صحیح انتہا,
		    منطقی adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = بفر->info;
  برائے (غیرعلامتی صحیح i = آغاز; i < انتہا; i++)
    اگر (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      اگر (adjust_offsets_when_zeroing)
      {
	بفر->pos[i].x_انحراف_2 = hb_saturate_sub (بفر->pos[i].x_انحراف_2,
						     بفر->pos[i].x_advance);
	بفر->pos[i].y_انحراف_2 = hb_saturate_sub (بفر->pos[i].y_انحراف_2,
						     بفر->pos[i].y_advance);
      }
      بفر->pos[i].x_advance = 0;
      بفر->pos[i].y_advance = 0;
    }
}

ساکن اندرونی خالی
position_mark (ثابت hb_ot_shape_plan_t *plan HB_غیر_مستعمل,
	       hb_font_t *font,
	       hb_بفر_t  *بفر,
	       hb_glyph_extents_t &بنیاد_extents,
	       غیرعلامتی صحیح i,
	       غیرعلامتی صحیح combining_class)
{
  hb_glyph_extents_t mark_extents;
  اگر (!font->get_glyph_extents (بفر->info[i].codepoint, &mark_extents))
    واپس;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = بفر->pos[i];
  pos.x_انحراف_2 = pos.y_انحراف_2 = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  انتخاب (combining_class)
  {
    حالت HB_UNICODE_COMBINING_CLASS_DOUBLE_نیچے:
    حالت HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      اگر (بفر->props.direction == HB_DIRECTION_LTR) {
	pos.x_انحراف_2 = hb_clamp_مقصد<hb_position_t> ((int64_t) بنیاد_extents.x_bearing + بنیاد_extents.چوڑائی -
						    mark_extents.چوڑائی / 2 - mark_extents.x_bearing);
	توڑو;
      } ورنہ اگر (بفر->props.direction == HB_DIRECTION_RTL) {
	pos.x_انحراف_2 = hb_clamp_مقصد<hb_position_t> ((int64_t) بنیاد_extents.x_bearing -
						    mark_extents.چوڑائی / 2 - mark_extents.x_bearing);
	توڑو;
      }
      HB_FALLTHROUGH;

    طےشدہ:
    حالت HB_UNICODE_COMBINING_CLASS_ATTACHED_نیچے:
    حالت HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    حالت HB_UNICODE_COMBINING_CLASS_نیچے:
    حالت HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_انحراف_2 = hb_clamp_مقصد<hb_position_t> ((int64_t) بنیاد_extents.x_bearing +
						((int64_t) بنیاد_extents.چوڑائی - mark_extents.چوڑائی) / 2 -
						mark_extents.x_bearing);
      توڑو;

    حالت HB_UNICODE_COMBINING_CLASS_ATTACHED_نیچے_بایاں:
    حالت HB_UNICODE_COMBINING_CLASS_نیچے_بایاں:
    حالت HB_UNICODE_COMBINING_CLASS_ABOVE_بایاں:
      /* Left align. */
      pos.x_انحراف_2 = hb_saturate_sub (بنیاد_extents.x_bearing, mark_extents.x_bearing);
      توڑو;

    حالت HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_دایاں:
    حالت HB_UNICODE_COMBINING_CLASS_نیچے_دایاں:
    حالت HB_UNICODE_COMBINING_CLASS_ABOVE_دایاں:
      /* Right align. */
      pos.x_انحراف_2 = hb_clamp_مقصد<hb_position_t> ((int64_t) بنیاد_extents.x_bearing + بنیاد_extents.چوڑائی -
						mark_extents.چوڑائی - mark_extents.x_bearing);
      توڑو;
  }

  /* Y positioning */
  انتخاب (combining_class)
  {
    حالت HB_UNICODE_COMBINING_CLASS_DOUBLE_نیچے:
    حالت HB_UNICODE_COMBINING_CLASS_نیچے_بایاں:
    حالت HB_UNICODE_COMBINING_CLASS_نیچے:
    حالت HB_UNICODE_COMBINING_CLASS_نیچے_دایاں:
      /* Add gap, fall-through. */
      بنیاد_extents.اونچائی = hb_saturate_sub (بنیاد_extents.اونچائی, y_gap);
      HB_FALLTHROUGH;

    حالت HB_UNICODE_COMBINING_CLASS_ATTACHED_نیچے_بایاں:
    حالت HB_UNICODE_COMBINING_CLASS_ATTACHED_نیچے:
      pos.y_انحراف_2 = hb_clamp_مقصد<hb_position_t> ((int64_t) بنیاد_extents.y_bearing +
						بنیاد_extents.اونچائی - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      اگر ((y_gap > 0) == (pos.y_انحراف_2 > 0))
      {
	بنیاد_extents.اونچائی = hb_saturate_sub (بنیاد_extents.اونچائی, pos.y_انحراف_2);
	pos.y_انحراف_2 = 0;
      }
      بنیاد_extents.اونچائی = hb_saturate_جوڑو (بنیاد_extents.اونچائی, mark_extents.اونچائی);
      توڑو;

    حالت HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    حالت HB_UNICODE_COMBINING_CLASS_ABOVE_بایاں:
    حالت HB_UNICODE_COMBINING_CLASS_ABOVE:
    حالت HB_UNICODE_COMBINING_CLASS_ABOVE_دایاں:
      /* Add gap, fall-through. */
      بنیاد_extents.y_bearing = hb_saturate_جوڑو (بنیاد_extents.y_bearing, y_gap);
      بنیاد_extents.اونچائی = hb_saturate_sub (بنیاد_extents.اونچائی, y_gap);
      HB_FALLTHROUGH;

    حالت HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    حالت HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_دایاں:
      pos.y_انحراف_2 = hb_clamp_مقصد<hb_position_t> ((int64_t) بنیاد_extents.y_bearing -
						mark_extents.y_bearing - mark_extents.اونچائی);
      /* Don't shift down "above" marks too much. */
      اگر ((y_gap > 0) != (pos.y_انحراف_2 > 0))
      {
	hb_position_t correction = -(pos.y_انحراف_2 / 2);
	بنیاد_extents.y_bearing = hb_saturate_جوڑو (بنیاد_extents.y_bearing, correction);
	بنیاد_extents.اونچائی = hb_saturate_sub (بنیاد_extents.اونچائی, correction);
	pos.y_انحراف_2 = hb_saturate_جوڑو (pos.y_انحراف_2, correction);
      }
      بنیاد_extents.y_bearing = hb_saturate_sub (بنیاد_extents.y_bearing, mark_extents.اونچائی);
      بنیاد_extents.اونچائی = hb_saturate_جوڑو (بنیاد_extents.اونچائی, mark_extents.اونچائی);
      توڑو;
  }
}

ساکن اندرونی خالی
position_around_بنیاد (ثابت hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_بفر_t  *بفر,
		      غیرعلامتی صحیح بنیاد,
		      غیرعلامتی صحیح انتہا,
		      منطقی adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  بفر->unsafe_مقصد_break (بنیاد, انتہا);

  hb_glyph_extents_t بنیاد_extents;
  اگر (!font->get_glyph_extents (بفر->info[بنیاد].codepoint,
				&بنیاد_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (بفر, بنیاد + 1, انتہا, adjust_offsets_when_zeroing);
    واپس;
  }
  بنیاد_extents.y_bearing = hb_saturate_جوڑو (بنیاد_extents.y_bearing,
					    بفر->pos[بنیاد].y_انحراف_2);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  بنیاد_extents.x_bearing = 0;
  بنیاد_extents.چوڑائی = font->get_glyph_h_advance (بفر->info[بنیاد].codepoint);

  غیرعلامتی صحیح lig_id = _hb_glyph_info_get_lig_id (&بفر->info[بنیاد]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  صحیح عدد_lig_components = hb_glyph_info_get_lig_عدد_comps (&بفر->info[بنیاد]);

  hb_position_t x_انحراف_2 = 0, y_انحراف_2 = 0;
  اگر (HB_DIRECTION_IS_FORWARD (بفر->props.direction)) {
    x_انحراف_2 = hb_saturate_neg (بفر->pos[بنیاد].x_advance);
    y_انحراف_2 = hb_saturate_neg (بفر->pos[بنیاد].y_advance);
  }

  hb_glyph_extents_t component_extents = بنیاد_extents;
  صحیح آخری_lig_component = -1;
  غیرعلامتی صحیح آخری_combining_class = 255;
  hb_glyph_extents_t cluster_extents = بنیاد_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = بفر->info;
  برائے (غیرعلامتی صحیح i = بنیاد + 1; i < انتہا; i++)
    اگر (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      اگر (عدد_lig_components > 1) {
	غیرعلامتی صحیح this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	صحیح this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
	/* Conditions for attaching to the last component. */
	اگر (!lig_id || lig_id != this_lig_id || this_lig_component >= عدد_lig_components)
	  this_lig_component = عدد_lig_components - 1;
	اگر (آخری_lig_component != this_lig_component)
	{
	  آخری_lig_component = this_lig_component;
	  آخری_combining_class = 255;
	  component_extents = بنیاد_extents;
	  اگر (unlikely (horiz_dir == HB_DIRECTION_INVALID)) {
	    اگر (HB_DIRECTION_IS_HORIZONTAL (plan->props.direction))
	      horiz_dir = plan->props.direction;
	    ورنہ
	      horiz_dir = hb_script_get_horizontal_direction (plan->props.script);
	  }
	  اگر (horiz_dir == HB_DIRECTION_LTR)
	    component_extents.x_bearing = hb_clamp_مقصد<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) this_lig_component * component_extents.چوڑائی / عدد_lig_components);
	  ورنہ
	    component_extents.x_bearing = hb_clamp_مقصد<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) (عدد_lig_components - 1 - this_lig_component) * component_extents.چوڑائی / عدد_lig_components);
	  component_extents.چوڑائی /= عدد_lig_components;
	}
      }

      غیرعلامتی صحیح this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      اگر (آخری_combining_class != this_combining_class)
      {
	آخری_combining_class = this_combining_class;
	cluster_extents = component_extents;
      }

      position_mark (plan, font, بفر, cluster_extents, i, this_combining_class);

      بفر->pos[i].x_advance = 0;
      بفر->pos[i].y_advance = 0;
      بفر->pos[i].x_انحراف_2 = hb_saturate_جوڑو (بفر->pos[i].x_انحراف_2, x_انحراف_2);
      بفر->pos[i].y_انحراف_2 = hb_saturate_جوڑو (بفر->pos[i].y_انحراف_2, y_انحراف_2);

    } ورنہ {
      اگر (HB_DIRECTION_IS_FORWARD (بفر->props.direction)) {
	x_انحراف_2 = hb_saturate_sub (x_انحراف_2, بفر->pos[i].x_advance);
	y_انحراف_2 = hb_saturate_sub (y_انحراف_2, بفر->pos[i].y_advance);
      } ورنہ {
	x_انحراف_2 = hb_saturate_جوڑو (x_انحراف_2, بفر->pos[i].x_advance);
	y_انحراف_2 = hb_saturate_جوڑو (y_انحراف_2, بفر->pos[i].y_advance);
      }
    }
}

ساکن اندرونی خالی
position_cluster_impl (ثابت hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_بفر_t  *بفر,
		       غیرعلامتی صحیح آغاز,
		       غیرعلامتی صحیح انتہا,
		       منطقی adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = بفر->info;
  برائے (غیرعلامتی صحیح i = آغاز; i < انتہا; i++)
    اگر (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      غیرعلامتی صحیح j;
      برائے (j = i + 1; j < انتہا; j++)
      {
        اگر (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  جاری;
	اگر (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  توڑو;
      }

      position_around_بنیاد (plan, font, بفر, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

ساکن HB_ALWAYS_INLINE خالی
position_cluster (ثابت hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_بفر_t  *بفر,
		  غیرعلامتی صحیح آغاز,
		  غیرعلامتی صحیح انتہا,
		  منطقی adjust_offsets_when_zeroing)
{
  اگر (انتہا - آغاز < 2)
    واپس;

  position_cluster_impl (plan, font, بفر, آغاز, انتہا, adjust_offsets_when_zeroing);
}

خالی
_hb_ot_shape_fallback_mark_position (ثابت hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_بفر_t  *بفر,
				     منطقی adjust_offsets_when_zeroing)
{
#اگر_تعریف_شدہ HB_NO_OT_SHAPE_FALLBACK
  واپس;
#اگر_ختم

  اگر (!بفر->پیغام (font, "start fallback mark"))
    واپس;

  hb_بفر_assert_gsubgpos_vars (بفر);

  غیرعلامتی صحیح آغاز = 0;
  غیرعلامتی صحیح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  برائے (غیرعلامتی صحیح i = 1; i < گنتی; i++)
    اگر (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, بفر, آغاز, i, adjust_offsets_when_zeroing);
      آغاز = i;
    }
  position_cluster (plan, font, بفر, آغاز, گنتی, adjust_offsets_when_zeroing);

  (خالی) بفر->پیغام (font, "end fallback mark");
}


#اگر_غیر_تعریف_شدہ HB_DISABLE_DEPRECATED
ساخت hb_ot_shape_fallback_kern_driver_t
{
  hb_ot_shape_fallback_kern_driver_t (hb_font_t   *font_,
				      hb_بفر_t *بفر) :
    font (font_), direction (بفر->props.direction) {}

  hb_position_t get_kerning (hb_codepoint_t پہلا, hb_codepoint_t second) ثابت
  {
    hb_position_t kern = 0;
    font->get_glyph_kerning_for_direction (پہلا, second,
					   direction,
					   &kern, &kern);
    واپس kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#اگر_ختم

/* Performs font-assisted kerning. */
خالی
_hb_ot_shape_fallback_kern (ثابت hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_بفر_t *بفر)
{
#اگر_تعریف_شدہ HB_NO_OT_SHAPE_FALLBACK
  واپس;
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_DISABLE_DEPRECATED
  اگر (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    واپس;

  اگر (!بفر->پیغام (font, "start fallback kern"))
    واپس;

  منطقی reverse = HB_DIRECTION_IS_BACKWARD (بفر->props.direction);

  اگر (reverse)
    بفر->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, بفر);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, بفر, plan->kern_نقاب, جھوٹ);

  اگر (reverse)
    بفر->reverse ();

  (خالی) بفر->پیغام (font, "end fallback kern");
#اگر_ختم
}


/* Adjusts width of various spaces. */
خالی
_hb_ot_shape_fallback_spaces (ثابت hb_ot_shape_plan_t *plan HB_غیر_مستعمل,
			      hb_font_t *font,
			      hb_بفر_t  *بفر)
{
  hb_glyph_info_t *info = بفر->info;
  hb_glyph_position_t *pos = بفر->pos;
  منطقی horizontal = HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction);
  غیرعلامتی صحیح گنتی = بفر->len;
  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    اگر (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      اگر (بفر->invisible && info[i].codepoint == بفر->invisible)
      {
        اگر (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        ورنہ
	  pos[i].y_advance = -(int64_t) font->y_scale / 4;
      }

      hb_unicode_funcs_t::space_t space_type = _hb_glyph_info_get_unicode_space_fallback_type (&info[i]);
      hb_codepoint_t glyph;
      قسمتعریف hb_unicode_funcs_t t;
      انتخاب (space_type)
      {
	حالت t::NOT_SPACE: /* Shouldn't happen. */
	حالت t::SPACE:
	  توڑو;

	حالت t::SPACE_EM:
	حالت t::SPACE_EM_2:
	حالت t::SPACE_EM_3:
	حالت t::SPACE_EM_4:
	حالت t::SPACE_EM_5:
	حالت t::SPACE_EM_6:
	حالت t::SPACE_EM_16:
	  اگر (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((صحیح) space_type)/2) / (صحیح) space_type;
	  ورنہ
	    pos[i].y_advance = -((int64_t) font->y_scale + ((صحیح) space_type)/2) / (صحیح) space_type;
	  توڑو;

	حالت t::SPACE_4_EM_18:
	  اگر (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  ورنہ
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  توڑو;

	حالت t::SPACE_FIGURE:
	  برائے (حرف u = '0'; u <= '9'; u++)
	    اگر (font->get_nominal_glyph (u, &glyph))
	    {
	      اگر (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      ورنہ
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      توڑو;
	    }
	  توڑو;

	حالت t::SPACE_PUNCTUATION:
	  اگر (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    اگر (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    ورنہ
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  توڑو;

	حالت t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  اگر (horizontal)
	    pos[i].x_advance /= 2;
	  ورنہ
	    pos[i].y_advance /= 2;
	  توڑو;
      }
    }
}


#اگر_ختم
