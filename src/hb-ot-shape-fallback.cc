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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE

#అన్తర్భావ "hb-ot-shape-fallback.hh"
#అన్తర్భావ "hb-kern.hh"

స్థిర గుర్తులేని పూర్ణాంకం
recategorize_combining_class (hb_codepoint_t u,
			      గుర్తులేని పూర్ణాంకం klass)
{
  ఒకవేళ (klass >= 200)
    తిరిగి klass;

  /* Thai / Lao need some per-character work. */
  ఒకవేళ ((u & ~0xFF) == 0x0E00u)
  {
    ఒకవేళ (unlikely (klass == 0))
    {
      ఎంపిక (u)
      {
	స్థితి 0x0E31u:
	స్థితి 0x0E34u:
	స్థితి 0x0E35u:
	స్థితి 0x0E36u:
	స్థితి 0x0E37u:
	స్థితి 0x0E47u:
	స్థితి 0x0E4Cu:
	స్థితి 0x0E4Du:
	స్థితి 0x0E4Eu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_కుడి;
	  ఆపు;

	స్థితి 0x0EB1u:
	స్థితి 0x0EB4u:
	స్థితి 0x0EB5u:
	స్థితి 0x0EB6u:
	స్థితి 0x0EB7u:
	స్థితి 0x0EBBu:
	స్థితి 0x0ECCu:
	స్థితి 0x0ECDu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE;
	  ఆపు;

	స్థితి 0x0EBCu:
	  klass = HB_UNICODE_COMBINING_CLASS_క్రింద;
	  ఆపు;
      }
    } లేకపోతే {
      /* Thai virama is below-right */
      ఒకవేళ (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_క్రింద_కుడి;
    }
  }

  ఎంపిక (klass)
  {

    /* Hebrew */

    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC10: /* sheva */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC11: /* hataf segol */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC12: /* hataf patah */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC13: /* hataf qamats */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC14: /* hiriq */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC15: /* tsere */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC16: /* segol */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC17: /* patah */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC18: /* qamats & qamats qatan */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC20: /* qubuts */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC22: /* meteg */
      తిరిగి HB_UNICODE_COMBINING_CLASS_క్రింద;

    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      తిరిగి HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      తిరిగి HB_UNICODE_COMBINING_CLASS_ABOVE_కుడి;

    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      తిరిగి HB_UNICODE_COMBINING_CLASS_ABOVE_ఎడమ;

    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      తిరిగి HB_UNICODE_COMBINING_CLASS_ABOVE;

    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC21: /* dagesh */
      ఆపు;


    /* Arabic and Syriac */

    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC27: /* fathatan */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC28: /* dammatan */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC30: /* fatha */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC31: /* damma */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC33: /* shadda */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC34: /* sukun */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC35: /* superscript alef */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC36: /* superscript alaph */
      తిరిగి HB_UNICODE_COMBINING_CLASS_ABOVE;

    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      తిరిగి HB_UNICODE_COMBINING_CLASS_క్రింద;


    /* Thai */

    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      తిరిగి HB_UNICODE_COMBINING_CLASS_క్రింద_కుడి;

    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      తిరిగి HB_UNICODE_COMBINING_CLASS_ABOVE_కుడి;


    /* Lao */

    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      తిరిగి HB_UNICODE_COMBINING_CLASS_క్రింద;

    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      తిరిగి HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      తిరిగి HB_UNICODE_COMBINING_CLASS_క్రింద;

    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      తిరిగి HB_UNICODE_COMBINING_CLASS_ABOVE;

    స్థితి HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      తిరిగి HB_UNICODE_COMBINING_CLASS_క్రింద;

  }

  తిరిగి klass;
}

శూన్య
_hb_ot_shape_fallback_mark_position_recategorize_marks (స్థిరరాశి hb_ot_shape_plan_t *plan HB_వాడని,
							hb_font_t *font HB_వాడని,
							hb_బఫరు_t  *బఫరు)
{
#నిర్వచితమైతే HB_NO_OT_SHAPE_FALLBACK
  తిరిగి;
#అయితే_ముగింపు

  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    ఒకవేళ (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      గుర్తులేని పూర్ణాంకం combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      hb_glyph_info_సమితి_modified_combining_class (&info[i], combining_class);
    }
}


స్థిర శూన్య
zero_mark_advances (hb_బఫరు_t *బఫరు,
		    గుర్తులేని పూర్ణాంకం ప్రారంభం,
		    గుర్తులేని పూర్ణాంకం చివర,
		    తార్కిక adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
    ఒకవేళ (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      ఒకవేళ (adjust_offsets_when_zeroing)
      {
	బఫరు->pos[i].x_స్థానభ్రంశం_2 = hb_saturate_sub (బఫరు->pos[i].x_స్థానభ్రంశం_2,
						     బఫరు->pos[i].x_advance);
	బఫరు->pos[i].y_స్థానభ్రంశం_2 = hb_saturate_sub (బఫరు->pos[i].y_స్థానభ్రంశం_2,
						     బఫరు->pos[i].y_advance);
      }
      బఫరు->pos[i].x_advance = 0;
      బఫరు->pos[i].y_advance = 0;
    }
}

స్థిర అంతఃవరుస శూన్య
position_mark (స్థిరరాశి hb_ot_shape_plan_t *plan HB_వాడని,
	       hb_font_t *font,
	       hb_బఫరు_t  *బఫరు,
	       hb_glyph_extents_t &పునాది_extents,
	       గుర్తులేని పూర్ణాంకం i,
	       గుర్తులేని పూర్ణాంకం combining_class)
{
  hb_glyph_extents_t mark_extents;
  ఒకవేళ (!font->get_glyph_extents (బఫరు->info[i].codepoint, &mark_extents))
    తిరిగి;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = బఫరు->pos[i];
  pos.x_స్థానభ్రంశం_2 = pos.y_స్థానభ్రంశం_2 = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  ఎంపిక (combining_class)
  {
    స్థితి HB_UNICODE_COMBINING_CLASS_DOUBLE_క్రింద:
    స్థితి HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      ఒకవేళ (బఫరు->props.direction == HB_DIRECTION_LTR) {
	pos.x_స్థానభ్రంశం_2 = hb_clamp_గమ్యం<hb_position_t> ((int64_t) పునాది_extents.x_bearing + పునాది_extents.వెడల్పు -
						    mark_extents.వెడల్పు / 2 - mark_extents.x_bearing);
	ఆపు;
      } లేకపోతే ఒకవేళ (బఫరు->props.direction == HB_DIRECTION_RTL) {
	pos.x_స్థానభ్రంశం_2 = hb_clamp_గమ్యం<hb_position_t> ((int64_t) పునాది_extents.x_bearing -
						    mark_extents.వెడల్పు / 2 - mark_extents.x_bearing);
	ఆపు;
      }
      HB_FALLTHROUGH;

    అప్రమేయ:
    స్థితి HB_UNICODE_COMBINING_CLASS_ATTACHED_క్రింద:
    స్థితి HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    స్థితి HB_UNICODE_COMBINING_CLASS_క్రింద:
    స్థితి HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_స్థానభ్రంశం_2 = hb_clamp_గమ్యం<hb_position_t> ((int64_t) పునాది_extents.x_bearing +
						((int64_t) పునాది_extents.వెడల్పు - mark_extents.వెడల్పు) / 2 -
						mark_extents.x_bearing);
      ఆపు;

    స్థితి HB_UNICODE_COMBINING_CLASS_ATTACHED_క్రింద_ఎడమ:
    స్థితి HB_UNICODE_COMBINING_CLASS_క్రింద_ఎడమ:
    స్థితి HB_UNICODE_COMBINING_CLASS_ABOVE_ఎడమ:
      /* Left align. */
      pos.x_స్థానభ్రంశం_2 = hb_saturate_sub (పునాది_extents.x_bearing, mark_extents.x_bearing);
      ఆపు;

    స్థితి HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_కుడి:
    స్థితి HB_UNICODE_COMBINING_CLASS_క్రింద_కుడి:
    స్థితి HB_UNICODE_COMBINING_CLASS_ABOVE_కుడి:
      /* Right align. */
      pos.x_స్థానభ్రంశం_2 = hb_clamp_గమ్యం<hb_position_t> ((int64_t) పునాది_extents.x_bearing + పునాది_extents.వెడల్పు -
						mark_extents.వెడల్పు - mark_extents.x_bearing);
      ఆపు;
  }

  /* Y positioning */
  ఎంపిక (combining_class)
  {
    స్థితి HB_UNICODE_COMBINING_CLASS_DOUBLE_క్రింద:
    స్థితి HB_UNICODE_COMBINING_CLASS_క్రింద_ఎడమ:
    స్థితి HB_UNICODE_COMBINING_CLASS_క్రింద:
    స్థితి HB_UNICODE_COMBINING_CLASS_క్రింద_కుడి:
      /* Add gap, fall-through. */
      పునాది_extents.ఎత్తు = hb_saturate_sub (పునాది_extents.ఎత్తు, y_gap);
      HB_FALLTHROUGH;

    స్థితి HB_UNICODE_COMBINING_CLASS_ATTACHED_క్రింద_ఎడమ:
    స్థితి HB_UNICODE_COMBINING_CLASS_ATTACHED_క్రింద:
      pos.y_స్థానభ్రంశం_2 = hb_clamp_గమ్యం<hb_position_t> ((int64_t) పునాది_extents.y_bearing +
						పునాది_extents.ఎత్తు - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      ఒకవేళ ((y_gap > 0) == (pos.y_స్థానభ్రంశం_2 > 0))
      {
	పునాది_extents.ఎత్తు = hb_saturate_sub (పునాది_extents.ఎత్తు, pos.y_స్థానభ్రంశం_2);
	pos.y_స్థానభ్రంశం_2 = 0;
      }
      పునాది_extents.ఎత్తు = hb_saturate_కూడు (పునాది_extents.ఎత్తు, mark_extents.ఎత్తు);
      ఆపు;

    స్థితి HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    స్థితి HB_UNICODE_COMBINING_CLASS_ABOVE_ఎడమ:
    స్థితి HB_UNICODE_COMBINING_CLASS_ABOVE:
    స్థితి HB_UNICODE_COMBINING_CLASS_ABOVE_కుడి:
      /* Add gap, fall-through. */
      పునాది_extents.y_bearing = hb_saturate_కూడు (పునాది_extents.y_bearing, y_gap);
      పునాది_extents.ఎత్తు = hb_saturate_sub (పునాది_extents.ఎత్తు, y_gap);
      HB_FALLTHROUGH;

    స్థితి HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    స్థితి HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_కుడి:
      pos.y_స్థానభ్రంశం_2 = hb_clamp_గమ్యం<hb_position_t> ((int64_t) పునాది_extents.y_bearing -
						mark_extents.y_bearing - mark_extents.ఎత్తు);
      /* Don't shift down "above" marks too much. */
      ఒకవేళ ((y_gap > 0) != (pos.y_స్థానభ్రంశం_2 > 0))
      {
	hb_position_t correction = -(pos.y_స్థానభ్రంశం_2 / 2);
	పునాది_extents.y_bearing = hb_saturate_కూడు (పునాది_extents.y_bearing, correction);
	పునాది_extents.ఎత్తు = hb_saturate_sub (పునాది_extents.ఎత్తు, correction);
	pos.y_స్థానభ్రంశం_2 = hb_saturate_కూడు (pos.y_స్థానభ్రంశం_2, correction);
      }
      పునాది_extents.y_bearing = hb_saturate_sub (పునాది_extents.y_bearing, mark_extents.ఎత్తు);
      పునాది_extents.ఎత్తు = hb_saturate_కూడు (పునాది_extents.ఎత్తు, mark_extents.ఎత్తు);
      ఆపు;
  }
}

స్థిర అంతఃవరుస శూన్య
position_around_పునాది (స్థిరరాశి hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_బఫరు_t  *బఫరు,
		      గుర్తులేని పూర్ణాంకం పునాది,
		      గుర్తులేని పూర్ణాంకం చివర,
		      తార్కిక adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  బఫరు->unsafe_గమ్యం_break (పునాది, చివర);

  hb_glyph_extents_t పునాది_extents;
  ఒకవేళ (!font->get_glyph_extents (బఫరు->info[పునాది].codepoint,
				&పునాది_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (బఫరు, పునాది + 1, చివర, adjust_offsets_when_zeroing);
    తిరిగి;
  }
  పునాది_extents.y_bearing = hb_saturate_కూడు (పునాది_extents.y_bearing,
					    బఫరు->pos[పునాది].y_స్థానభ్రంశం_2);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  పునాది_extents.x_bearing = 0;
  పునాది_extents.వెడల్పు = font->get_glyph_h_advance (బఫరు->info[పునాది].codepoint);

  గుర్తులేని పూర్ణాంకం lig_id = _hb_glyph_info_get_lig_id (&బఫరు->info[పునాది]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  పూర్ణాంకం అంకె_lig_components = hb_glyph_info_get_lig_అంకె_comps (&బఫరు->info[పునాది]);

  hb_position_t x_స్థానభ్రంశం_2 = 0, y_స్థానభ్రంశం_2 = 0;
  ఒకవేళ (HB_DIRECTION_IS_FORWARD (బఫరు->props.direction)) {
    x_స్థానభ్రంశం_2 = hb_saturate_neg (బఫరు->pos[పునాది].x_advance);
    y_స్థానభ్రంశం_2 = hb_saturate_neg (బఫరు->pos[పునాది].y_advance);
  }

  hb_glyph_extents_t component_extents = పునాది_extents;
  పూర్ణాంకం చివరి_lig_component = -1;
  గుర్తులేని పూర్ణాంకం చివరి_combining_class = 255;
  hb_glyph_extents_t cluster_extents = పునాది_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = పునాది + 1; i < చివర; i++)
    ఒకవేళ (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      ఒకవేళ (అంకె_lig_components > 1) {
	గుర్తులేని పూర్ణాంకం this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	పూర్ణాంకం this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
	/* Conditions for attaching to the last component. */
	ఒకవేళ (!lig_id || lig_id != this_lig_id || this_lig_component >= అంకె_lig_components)
	  this_lig_component = అంకె_lig_components - 1;
	ఒకవేళ (చివరి_lig_component != this_lig_component)
	{
	  చివరి_lig_component = this_lig_component;
	  చివరి_combining_class = 255;
	  component_extents = పునాది_extents;
	  ఒకవేళ (unlikely (horiz_dir == HB_DIRECTION_INVALID)) {
	    ఒకవేళ (HB_DIRECTION_IS_HORIZONTAL (plan->props.direction))
	      horiz_dir = plan->props.direction;
	    లేకపోతే
	      horiz_dir = hb_script_get_horizontal_direction (plan->props.script);
	  }
	  ఒకవేళ (horiz_dir == HB_DIRECTION_LTR)
	    component_extents.x_bearing = hb_clamp_గమ్యం<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) this_lig_component * component_extents.వెడల్పు / అంకె_lig_components);
	  లేకపోతే
	    component_extents.x_bearing = hb_clamp_గమ్యం<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) (అంకె_lig_components - 1 - this_lig_component) * component_extents.వెడల్పు / అంకె_lig_components);
	  component_extents.వెడల్పు /= అంకె_lig_components;
	}
      }

      గుర్తులేని పూర్ణాంకం this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      ఒకవేళ (చివరి_combining_class != this_combining_class)
      {
	చివరి_combining_class = this_combining_class;
	cluster_extents = component_extents;
      }

      position_mark (plan, font, బఫరు, cluster_extents, i, this_combining_class);

      బఫరు->pos[i].x_advance = 0;
      బఫరు->pos[i].y_advance = 0;
      బఫరు->pos[i].x_స్థానభ్రంశం_2 = hb_saturate_కూడు (బఫరు->pos[i].x_స్థానభ్రంశం_2, x_స్థానభ్రంశం_2);
      బఫరు->pos[i].y_స్థానభ్రంశం_2 = hb_saturate_కూడు (బఫరు->pos[i].y_స్థానభ్రంశం_2, y_స్థానభ్రంశం_2);

    } లేకపోతే {
      ఒకవేళ (HB_DIRECTION_IS_FORWARD (బఫరు->props.direction)) {
	x_స్థానభ్రంశం_2 = hb_saturate_sub (x_స్థానభ్రంశం_2, బఫరు->pos[i].x_advance);
	y_స్థానభ్రంశం_2 = hb_saturate_sub (y_స్థానభ్రంశం_2, బఫరు->pos[i].y_advance);
      } లేకపోతే {
	x_స్థానభ్రంశం_2 = hb_saturate_కూడు (x_స్థానభ్రంశం_2, బఫరు->pos[i].x_advance);
	y_స్థానభ్రంశం_2 = hb_saturate_కూడు (y_స్థానభ్రంశం_2, బఫరు->pos[i].y_advance);
      }
    }
}

స్థిర అంతఃవరుస శూన్య
position_cluster_impl (స్థిరరాశి hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_బఫరు_t  *బఫరు,
		       గుర్తులేని పూర్ణాంకం ప్రారంభం,
		       గుర్తులేని పూర్ణాంకం చివర,
		       తార్కిక adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
    ఒకవేళ (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      గుర్తులేని పూర్ణాంకం j;
      కొరకు (j = i + 1; j < చివర; j++)
      {
        ఒకవేళ (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  కొనసాగు;
	ఒకవేళ (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  ఆపు;
      }

      position_around_పునాది (plan, font, బఫరు, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

స్థిర HB_ALWAYS_INLINE శూన్య
position_cluster (స్థిరరాశి hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_బఫరు_t  *బఫరు,
		  గుర్తులేని పూర్ణాంకం ప్రారంభం,
		  గుర్తులేని పూర్ణాంకం చివర,
		  తార్కిక adjust_offsets_when_zeroing)
{
  ఒకవేళ (చివర - ప్రారంభం < 2)
    తిరిగి;

  position_cluster_impl (plan, font, బఫరు, ప్రారంభం, చివర, adjust_offsets_when_zeroing);
}

శూన్య
_hb_ot_shape_fallback_mark_position (స్థిరరాశి hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_బఫరు_t  *బఫరు,
				     తార్కిక adjust_offsets_when_zeroing)
{
#నిర్వచితమైతే HB_NO_OT_SHAPE_FALLBACK
  తిరిగి;
#అయితే_ముగింపు

  ఒకవేళ (!బఫరు->సందేశం (font, "start fallback mark"))
    తిరిగి;

  hb_బఫరు_assert_gsubgpos_vars (బఫరు);

  గుర్తులేని పూర్ణాంకం ప్రారంభం = 0;
  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < లెక్క; i++)
    ఒకవేళ (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, బఫరు, ప్రారంభం, i, adjust_offsets_when_zeroing);
      ప్రారంభం = i;
    }
  position_cluster (plan, font, బఫరు, ప్రారంభం, లెక్క, adjust_offsets_when_zeroing);

  (శూన్య) బఫరు->సందేశం (font, "end fallback mark");
}


#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED
నిర్మాణం hb_ot_shape_fallback_kern_driver_t
{
  hb_ot_shape_fallback_kern_driver_t (hb_font_t   *font_,
				      hb_బఫరు_t *బఫరు) :
    font (font_), direction (బఫరు->props.direction) {}

  hb_position_t get_kerning (hb_codepoint_t మొదటి, hb_codepoint_t second) స్థిరరాశి
  {
    hb_position_t kern = 0;
    font->get_glyph_kerning_for_direction (మొదటి, second,
					   direction,
					   &kern, &kern);
    తిరిగి kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#అయితే_ముగింపు

/* Performs font-assisted kerning. */
శూన్య
_hb_ot_shape_fallback_kern (స్థిరరాశి hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_బఫరు_t *బఫరు)
{
#నిర్వచితమైతే HB_NO_OT_SHAPE_FALLBACK
  తిరిగి;
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED
  ఒకవేళ (HB_DIRECTION_IS_HORIZONTAL (బఫరు->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    తిరిగి;

  ఒకవేళ (!బఫరు->సందేశం (font, "start fallback kern"))
    తిరిగి;

  తార్కిక reverse = HB_DIRECTION_IS_BACKWARD (బఫరు->props.direction);

  ఒకవేళ (reverse)
    బఫరు->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, బఫరు);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, బఫరు, plan->kern_ముసుగు, అబద్ధం);

  ఒకవేళ (reverse)
    బఫరు->reverse ();

  (శూన్య) బఫరు->సందేశం (font, "end fallback kern");
#అయితే_ముగింపు
}


/* Adjusts width of various spaces. */
శూన్య
_hb_ot_shape_fallback_spaces (స్థిరరాశి hb_ot_shape_plan_t *plan HB_వాడని,
			      hb_font_t *font,
			      hb_బఫరు_t  *బఫరు)
{
  hb_glyph_info_t *info = బఫరు->info;
  hb_glyph_position_t *pos = బఫరు->pos;
  తార్కిక horizontal = HB_DIRECTION_IS_HORIZONTAL (బఫరు->props.direction);
  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    ఒకవేళ (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      ఒకవేళ (బఫరు->invisible && info[i].codepoint == బఫరు->invisible)
      {
        ఒకవేళ (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        లేకపోతే
	  pos[i].y_advance = -(int64_t) font->y_scale / 4;
      }

      hb_unicode_funcs_t::space_t space_type = _hb_glyph_info_get_unicode_space_fallback_type (&info[i]);
      hb_codepoint_t glyph;
      రకనిర్వచనం hb_unicode_funcs_t t;
      ఎంపిక (space_type)
      {
	స్థితి t::NOT_SPACE: /* Shouldn't happen. */
	స్థితి t::SPACE:
	  ఆపు;

	స్థితి t::SPACE_EM:
	స్థితి t::SPACE_EM_2:
	స్థితి t::SPACE_EM_3:
	స్థితి t::SPACE_EM_4:
	స్థితి t::SPACE_EM_5:
	స్థితి t::SPACE_EM_6:
	స్థితి t::SPACE_EM_16:
	  ఒకవేళ (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((పూర్ణాంకం) space_type)/2) / (పూర్ణాంకం) space_type;
	  లేకపోతే
	    pos[i].y_advance = -((int64_t) font->y_scale + ((పూర్ణాంకం) space_type)/2) / (పూర్ణాంకం) space_type;
	  ఆపు;

	స్థితి t::SPACE_4_EM_18:
	  ఒకవేళ (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  లేకపోతే
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  ఆపు;

	స్థితి t::SPACE_FIGURE:
	  కొరకు (అక్షరం u = '0'; u <= '9'; u++)
	    ఒకవేళ (font->get_nominal_glyph (u, &glyph))
	    {
	      ఒకవేళ (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      లేకపోతే
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      ఆపు;
	    }
	  ఆపు;

	స్థితి t::SPACE_PUNCTUATION:
	  ఒకవేళ (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    ఒకవేళ (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    లేకపోతే
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  ఆపు;

	స్థితి t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  ఒకవేళ (horizontal)
	    pos[i].x_advance /= 2;
	  లేకపోతే
	    pos[i].y_advance /= 2;
	  ఆపు;
      }
    }
}


#అయితే_ముగింపు
