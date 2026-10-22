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

#که_نه_تعریف_شوی HB_NO_OT_SHAPE

#ضمن "hb-ot-shape-fallback.hh"
#ضمن "hb-kern.hh"

ساكن بینښه صحيح
recategorize_combining_class (hb_codepoint_t u,
			      بینښه صحيح klass)
{
  إذا (klass >= 200)
    أرجع klass;

  /* Thai / Lao need some per-character work. */
  إذا ((u & ~0xFF) == 0x0E00u)
  {
    إذا (unlikely (klass == 0))
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
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_ښی;
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
	  klass = HB_UNICODE_COMBINING_CLASS_لاندې;
	  اكسر;
      }
    } وإلا {
      /* Thai virama is below-right */
      إذا (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_لاندې_ښی;
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
      أرجع HB_UNICODE_COMBINING_CLASS_لاندې;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      أرجع HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      أرجع HB_UNICODE_COMBINING_CLASS_ABOVE_ښی;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      أرجع HB_UNICODE_COMBINING_CLASS_ABOVE_کیڼ;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      أرجع HB_UNICODE_COMBINING_CLASS_ABOVE;

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
      أرجع HB_UNICODE_COMBINING_CLASS_ABOVE;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    حالة HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      أرجع HB_UNICODE_COMBINING_CLASS_لاندې;


    /* Thai */

    حالة HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      أرجع HB_UNICODE_COMBINING_CLASS_لاندې_ښی;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      أرجع HB_UNICODE_COMBINING_CLASS_ABOVE_ښی;


    /* Lao */

    حالة HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      أرجع HB_UNICODE_COMBINING_CLASS_لاندې;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      أرجع HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    حالة HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      أرجع HB_UNICODE_COMBINING_CLASS_لاندې;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      أرجع HB_UNICODE_COMBINING_CLASS_ABOVE;

    حالة HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      أرجع HB_UNICODE_COMBINING_CLASS_لاندې;

  }

  أرجع klass;
}

فراغ
_hb_ot_shape_fallback_mark_position_recategorize_marks (ثابت hb_ot_shape_plan_t *plan HB_ناکارول_شوی,
							hb_font_t *font HB_ناکارول_شوی,
							hb_بفر_t  *بفر)
{
#که_تعریف_شوی HB_NO_OT_SHAPE_FALLBACK
  أرجع;
#د_که_پای

  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بینښه صحيح i = 0; i < شمېر; i++)
    إذا (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      بینښه صحيح combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      hb_glyph_info_سېټ_modified_combining_class (&info[i], combining_class);
    }
}


ساكن فراغ
zero_mark_advances (hb_بفر_t *بفر,
		    بینښه صحيح پیل,
		    بینښه صحيح پای_ټکی,
		    منطقي adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = بفر->info;
  لكل (بینښه صحيح i = پیل; i < پای_ټکی; i++)
    إذا (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      إذا (adjust_offsets_when_zeroing)
      {
	بفر->pos[i].x_دننه_واټن_2 = hb_saturate_sub (بفر->pos[i].x_دننه_واټن_2,
						     بفر->pos[i].x_advance);
	بفر->pos[i].y_دننه_واټن_2 = hb_saturate_sub (بفر->pos[i].y_دننه_واټن_2,
						     بفر->pos[i].y_advance);
      }
      بفر->pos[i].x_advance = 0;
      بفر->pos[i].y_advance = 0;
    }
}

ساكن مضمن فراغ
position_mark (ثابت hb_ot_shape_plan_t *plan HB_ناکارول_شوی,
	       hb_font_t *font,
	       hb_بفر_t  *بفر,
	       hb_glyph_extents_t &بنسټ_extents,
	       بینښه صحيح i,
	       بینښه صحيح combining_class)
{
  hb_glyph_extents_t mark_extents;
  إذا (!font->get_glyph_extents (بفر->info[i].codepoint, &mark_extents))
    أرجع;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = بفر->pos[i];
  pos.x_دننه_واټن_2 = pos.y_دننه_واټن_2 = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  بدل (combining_class)
  {
    حالة HB_UNICODE_COMBINING_CLASS_DOUBLE_لاندې:
    حالة HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      إذا (بفر->props.direction == HB_DIRECTION_LTR) {
	pos.x_دننه_واټن_2 = hb_clamp_منزل<hb_position_t> ((int64_t) بنسټ_extents.x_bearing + بنسټ_extents.پلنوالی -
						    mark_extents.پلنوالی / 2 - mark_extents.x_bearing);
	اكسر;
      } وإلا إذا (بفر->props.direction == HB_DIRECTION_RTL) {
	pos.x_دننه_واټن_2 = hb_clamp_منزل<hb_position_t> ((int64_t) بنسټ_extents.x_bearing -
						    mark_extents.پلنوالی / 2 - mark_extents.x_bearing);
	اكسر;
      }
      HB_FALLTHROUGH;

    افتراضي:
    حالة HB_UNICODE_COMBINING_CLASS_ATTACHED_لاندې:
    حالة HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    حالة HB_UNICODE_COMBINING_CLASS_لاندې:
    حالة HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_دننه_واټن_2 = hb_clamp_منزل<hb_position_t> ((int64_t) بنسټ_extents.x_bearing +
						((int64_t) بنسټ_extents.پلنوالی - mark_extents.پلنوالی) / 2 -
						mark_extents.x_bearing);
      اكسر;

    حالة HB_UNICODE_COMBINING_CLASS_ATTACHED_لاندې_کیڼ:
    حالة HB_UNICODE_COMBINING_CLASS_لاندې_کیڼ:
    حالة HB_UNICODE_COMBINING_CLASS_ABOVE_کیڼ:
      /* Left align. */
      pos.x_دننه_واټن_2 = hb_saturate_sub (بنسټ_extents.x_bearing, mark_extents.x_bearing);
      اكسر;

    حالة HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_ښی:
    حالة HB_UNICODE_COMBINING_CLASS_لاندې_ښی:
    حالة HB_UNICODE_COMBINING_CLASS_ABOVE_ښی:
      /* Right align. */
      pos.x_دننه_واټن_2 = hb_clamp_منزل<hb_position_t> ((int64_t) بنسټ_extents.x_bearing + بنسټ_extents.پلنوالی -
						mark_extents.پلنوالی - mark_extents.x_bearing);
      اكسر;
  }

  /* Y positioning */
  بدل (combining_class)
  {
    حالة HB_UNICODE_COMBINING_CLASS_DOUBLE_لاندې:
    حالة HB_UNICODE_COMBINING_CLASS_لاندې_کیڼ:
    حالة HB_UNICODE_COMBINING_CLASS_لاندې:
    حالة HB_UNICODE_COMBINING_CLASS_لاندې_ښی:
      /* Add gap, fall-through. */
      بنسټ_extents.لوړوالی = hb_saturate_sub (بنسټ_extents.لوړوالی, y_gap);
      HB_FALLTHROUGH;

    حالة HB_UNICODE_COMBINING_CLASS_ATTACHED_لاندې_کیڼ:
    حالة HB_UNICODE_COMBINING_CLASS_ATTACHED_لاندې:
      pos.y_دننه_واټن_2 = hb_clamp_منزل<hb_position_t> ((int64_t) بنسټ_extents.y_bearing +
						بنسټ_extents.لوړوالی - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      إذا ((y_gap > 0) == (pos.y_دننه_واټن_2 > 0))
      {
	بنسټ_extents.لوړوالی = hb_saturate_sub (بنسټ_extents.لوړوالی, pos.y_دننه_واټن_2);
	pos.y_دننه_واټن_2 = 0;
      }
      بنسټ_extents.لوړوالی = hb_saturate_جمع_کړه (بنسټ_extents.لوړوالی, mark_extents.لوړوالی);
      اكسر;

    حالة HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    حالة HB_UNICODE_COMBINING_CLASS_ABOVE_کیڼ:
    حالة HB_UNICODE_COMBINING_CLASS_ABOVE:
    حالة HB_UNICODE_COMBINING_CLASS_ABOVE_ښی:
      /* Add gap, fall-through. */
      بنسټ_extents.y_bearing = hb_saturate_جمع_کړه (بنسټ_extents.y_bearing, y_gap);
      بنسټ_extents.لوړوالی = hb_saturate_sub (بنسټ_extents.لوړوالی, y_gap);
      HB_FALLTHROUGH;

    حالة HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    حالة HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_ښی:
      pos.y_دننه_واټن_2 = hb_clamp_منزل<hb_position_t> ((int64_t) بنسټ_extents.y_bearing -
						mark_extents.y_bearing - mark_extents.لوړوالی);
      /* Don't shift down "above" marks too much. */
      إذا ((y_gap > 0) != (pos.y_دننه_واټن_2 > 0))
      {
	hb_position_t correction = -(pos.y_دننه_واټن_2 / 2);
	بنسټ_extents.y_bearing = hb_saturate_جمع_کړه (بنسټ_extents.y_bearing, correction);
	بنسټ_extents.لوړوالی = hb_saturate_sub (بنسټ_extents.لوړوالی, correction);
	pos.y_دننه_واټن_2 = hb_saturate_جمع_کړه (pos.y_دننه_واټن_2, correction);
      }
      بنسټ_extents.y_bearing = hb_saturate_sub (بنسټ_extents.y_bearing, mark_extents.لوړوالی);
      بنسټ_extents.لوړوالی = hb_saturate_جمع_کړه (بنسټ_extents.لوړوالی, mark_extents.لوړوالی);
      اكسر;
  }
}

ساكن مضمن فراغ
position_around_بنسټ (ثابت hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_بفر_t  *بفر,
		      بینښه صحيح بنسټ,
		      بینښه صحيح پای_ټکی,
		      منطقي adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  بفر->unsafe_منزل_break (بنسټ, پای_ټکی);

  hb_glyph_extents_t بنسټ_extents;
  إذا (!font->get_glyph_extents (بفر->info[بنسټ].codepoint,
				&بنسټ_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (بفر, بنسټ + 1, پای_ټکی, adjust_offsets_when_zeroing);
    أرجع;
  }
  بنسټ_extents.y_bearing = hb_saturate_جمع_کړه (بنسټ_extents.y_bearing,
					    بفر->pos[بنسټ].y_دننه_واټن_2);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  بنسټ_extents.x_bearing = 0;
  بنسټ_extents.پلنوالی = font->get_glyph_h_advance (بفر->info[بنسټ].codepoint);

  بینښه صحيح lig_id = _hb_glyph_info_get_lig_id (&بفر->info[بنسټ]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  صحيح عدد_lig_components = hb_glyph_info_get_lig_عدد_comps (&بفر->info[بنسټ]);

  hb_position_t x_دننه_واټن_2 = 0, y_دننه_واټن_2 = 0;
  إذا (HB_DIRECTION_IS_FORWARD (بفر->props.direction)) {
    x_دننه_واټن_2 = hb_saturate_neg (بفر->pos[بنسټ].x_advance);
    y_دننه_واټن_2 = hb_saturate_neg (بفر->pos[بنسټ].y_advance);
  }

  hb_glyph_extents_t component_extents = بنسټ_extents;
  صحيح وروستی_lig_component = -1;
  بینښه صحيح وروستی_combining_class = 255;
  hb_glyph_extents_t cluster_extents = بنسټ_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = بفر->info;
  لكل (بینښه صحيح i = بنسټ + 1; i < پای_ټکی; i++)
    إذا (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      إذا (عدد_lig_components > 1) {
	بینښه صحيح this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	صحيح this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
	/* Conditions for attaching to the last component. */
	إذا (!lig_id || lig_id != this_lig_id || this_lig_component >= عدد_lig_components)
	  this_lig_component = عدد_lig_components - 1;
	إذا (وروستی_lig_component != this_lig_component)
	{
	  وروستی_lig_component = this_lig_component;
	  وروستی_combining_class = 255;
	  component_extents = بنسټ_extents;
	  إذا (unlikely (horiz_dir == HB_DIRECTION_INVALID)) {
	    إذا (HB_DIRECTION_IS_HORIZONTAL (plan->props.direction))
	      horiz_dir = plan->props.direction;
	    وإلا
	      horiz_dir = hb_script_get_horizontal_direction (plan->props.script);
	  }
	  إذا (horiz_dir == HB_DIRECTION_LTR)
	    component_extents.x_bearing = hb_clamp_منزل<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) this_lig_component * component_extents.پلنوالی / عدد_lig_components);
	  وإلا
	    component_extents.x_bearing = hb_clamp_منزل<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) (عدد_lig_components - 1 - this_lig_component) * component_extents.پلنوالی / عدد_lig_components);
	  component_extents.پلنوالی /= عدد_lig_components;
	}
      }

      بینښه صحيح this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      إذا (وروستی_combining_class != this_combining_class)
      {
	وروستی_combining_class = this_combining_class;
	cluster_extents = component_extents;
      }

      position_mark (plan, font, بفر, cluster_extents, i, this_combining_class);

      بفر->pos[i].x_advance = 0;
      بفر->pos[i].y_advance = 0;
      بفر->pos[i].x_دننه_واټن_2 = hb_saturate_جمع_کړه (بفر->pos[i].x_دننه_واټن_2, x_دننه_واټن_2);
      بفر->pos[i].y_دننه_واټن_2 = hb_saturate_جمع_کړه (بفر->pos[i].y_دننه_واټن_2, y_دننه_واټن_2);

    } وإلا {
      إذا (HB_DIRECTION_IS_FORWARD (بفر->props.direction)) {
	x_دننه_واټن_2 = hb_saturate_sub (x_دننه_واټن_2, بفر->pos[i].x_advance);
	y_دننه_واټن_2 = hb_saturate_sub (y_دننه_واټن_2, بفر->pos[i].y_advance);
      } وإلا {
	x_دننه_واټن_2 = hb_saturate_جمع_کړه (x_دننه_واټن_2, بفر->pos[i].x_advance);
	y_دننه_واټن_2 = hb_saturate_جمع_کړه (y_دننه_واټن_2, بفر->pos[i].y_advance);
      }
    }
}

ساكن مضمن فراغ
position_cluster_impl (ثابت hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_بفر_t  *بفر,
		       بینښه صحيح پیل,
		       بینښه صحيح پای_ټکی,
		       منطقي adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = بفر->info;
  لكل (بینښه صحيح i = پیل; i < پای_ټکی; i++)
    إذا (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      بینښه صحيح j;
      لكل (j = i + 1; j < پای_ټکی; j++)
      {
        إذا (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  استمر;
	إذا (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  اكسر;
      }

      position_around_بنسټ (plan, font, بفر, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

ساكن HB_ALWAYS_INLINE فراغ
position_cluster (ثابت hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_بفر_t  *بفر,
		  بینښه صحيح پیل,
		  بینښه صحيح پای_ټکی,
		  منطقي adjust_offsets_when_zeroing)
{
  إذا (پای_ټکی - پیل < 2)
    أرجع;

  position_cluster_impl (plan, font, بفر, پیل, پای_ټکی, adjust_offsets_when_zeroing);
}

فراغ
_hb_ot_shape_fallback_mark_position (ثابت hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_بفر_t  *بفر,
				     منطقي adjust_offsets_when_zeroing)
{
#که_تعریف_شوی HB_NO_OT_SHAPE_FALLBACK
  أرجع;
#د_که_پای

  إذا (!بفر->پیغام (font, "start fallback mark"))
    أرجع;

  hb_بفر_assert_gsubgpos_vars (بفر);

  بینښه صحيح پیل = 0;
  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بینښه صحيح i = 1; i < شمېر; i++)
    إذا (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, بفر, پیل, i, adjust_offsets_when_zeroing);
      پیل = i;
    }
  position_cluster (plan, font, بفر, پیل, شمېر, adjust_offsets_when_zeroing);

  (فراغ) بفر->پیغام (font, "end fallback mark");
}


#که_نه_تعریف_شوی HB_DISABLE_DEPRECATED
بنية hb_ot_shape_fallback_kern_driver_t
{
  hb_ot_shape_fallback_kern_driver_t (hb_font_t   *font_,
				      hb_بفر_t *بفر) :
    font (font_), direction (بفر->props.direction) {}

  hb_position_t get_kerning (hb_codepoint_t لومړی, hb_codepoint_t second) ثابت
  {
    hb_position_t kern = 0;
    font->get_glyph_kerning_for_direction (لومړی, second,
					   direction,
					   &kern, &kern);
    أرجع kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#د_که_پای

/* Performs font-assisted kerning. */
فراغ
_hb_ot_shape_fallback_kern (ثابت hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_بفر_t *بفر)
{
#که_تعریف_شوی HB_NO_OT_SHAPE_FALLBACK
  أرجع;
#د_که_پای

#که_نه_تعریف_شوی HB_DISABLE_DEPRECATED
  إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    أرجع;

  إذا (!بفر->پیغام (font, "start fallback kern"))
    أرجع;

  منطقي reverse = HB_DIRECTION_IS_BACKWARD (بفر->props.direction);

  إذا (reverse)
    بفر->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, بفر);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, بفر, plan->kern_ماسک, خطأ);

  إذا (reverse)
    بفر->reverse ();

  (فراغ) بفر->پیغام (font, "end fallback kern");
#د_که_پای
}


/* Adjusts width of various spaces. */
فراغ
_hb_ot_shape_fallback_spaces (ثابت hb_ot_shape_plan_t *plan HB_ناکارول_شوی,
			      hb_font_t *font,
			      hb_بفر_t  *بفر)
{
  hb_glyph_info_t *info = بفر->info;
  hb_glyph_position_t *pos = بفر->pos;
  منطقي horizontal = HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction);
  بینښه صحيح شمېر = بفر->len;
  لكل (بینښه صحيح i = 0; i < شمېر; i++)
    إذا (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      إذا (بفر->invisible && info[i].codepoint == بفر->invisible)
      {
        إذا (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        وإلا
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
	  إذا (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((صحيح) space_type)/2) / (صحيح) space_type;
	  وإلا
	    pos[i].y_advance = -((int64_t) font->y_scale + ((صحيح) space_type)/2) / (صحيح) space_type;
	  اكسر;

	حالة t::SPACE_4_EM_18:
	  إذا (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  وإلا
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  اكسر;

	حالة t::SPACE_FIGURE:
	  لكل (حرف u = '0'; u <= '9'; u++)
	    إذا (font->get_nominal_glyph (u, &glyph))
	    {
	      إذا (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      وإلا
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      اكسر;
	    }
	  اكسر;

	حالة t::SPACE_PUNCTUATION:
	  إذا (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    إذا (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    وإلا
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  اكسر;

	حالة t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  إذا (horizontal)
	    pos[i].x_advance /= 2;
	  وإلا
	    pos[i].y_advance /= 2;
	  اكسر;
      }
    }
}


#د_که_پای
