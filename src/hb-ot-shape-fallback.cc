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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_OT_SHAPE

#समावेश "hb-ot-shape-fallback.hh"
#समावेश "hb-kern.hh"

अटल अचिन्हित पूर्णांक
recategorize_combining_class (hb_codepoint_t u,
			      अचिन्हित पूर्णांक klass)
{
  अगर (klass >= 200)
    वापस klass;

  /* Thai / Lao need some per-character work. */
  अगर ((u & ~0xFF) == 0x0E00u)
  {
    अगर (unlikely (klass == 0))
    {
      चयन (u)
      {
	हाल 0x0E31u:
	हाल 0x0E34u:
	हाल 0x0E35u:
	हाल 0x0E36u:
	हाल 0x0E37u:
	हाल 0x0E47u:
	हाल 0x0E4Cu:
	हाल 0x0E4Du:
	हाल 0x0E4Eu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_दायाँ;
	  अवरोध;

	हाल 0x0EB1u:
	हाल 0x0EB4u:
	हाल 0x0EB5u:
	हाल 0x0EB6u:
	हाल 0x0EB7u:
	हाल 0x0EBBu:
	हाल 0x0ECCu:
	हाल 0x0ECDu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE;
	  अवरोध;

	हाल 0x0EBCu:
	  klass = HB_UNICODE_COMBINING_CLASS_BELOW;
	  अवरोध;
      }
    } अन्यथा {
      /* Thai virama is below-right */
      अगर (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_BELOW_दायाँ;
    }
  }

  चयन (klass)
  {

    /* Hebrew */

    हाल HB_MODIFIED_COMBINING_CLASS_CCC10: /* sheva */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC11: /* hataf segol */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC12: /* hataf patah */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC13: /* hataf qamats */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC14: /* hiriq */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC15: /* tsere */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC16: /* segol */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC17: /* patah */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC18: /* qamats & qamats qatan */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC20: /* qubuts */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC22: /* meteg */
      वापस HB_UNICODE_COMBINING_CLASS_BELOW;

    हाल HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      वापस HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    हाल HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      वापस HB_UNICODE_COMBINING_CLASS_ABOVE_दायाँ;

    हाल HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      वापस HB_UNICODE_COMBINING_CLASS_ABOVE_बायाँ;

    हाल HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      वापस HB_UNICODE_COMBINING_CLASS_ABOVE;

    हाल HB_MODIFIED_COMBINING_CLASS_CCC21: /* dagesh */
      अवरोध;


    /* Arabic and Syriac */

    हाल HB_MODIFIED_COMBINING_CLASS_CCC27: /* fathatan */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC28: /* dammatan */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC30: /* fatha */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC31: /* damma */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC33: /* shadda */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC34: /* sukun */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC35: /* superscript alef */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC36: /* superscript alaph */
      वापस HB_UNICODE_COMBINING_CLASS_ABOVE;

    हाल HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    हाल HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      वापस HB_UNICODE_COMBINING_CLASS_BELOW;


    /* Thai */

    हाल HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      वापस HB_UNICODE_COMBINING_CLASS_BELOW_दायाँ;

    हाल HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      वापस HB_UNICODE_COMBINING_CLASS_ABOVE_दायाँ;


    /* Lao */

    हाल HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      वापस HB_UNICODE_COMBINING_CLASS_BELOW;

    हाल HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      वापस HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    हाल HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      वापस HB_UNICODE_COMBINING_CLASS_BELOW;

    हाल HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      वापस HB_UNICODE_COMBINING_CLASS_ABOVE;

    हाल HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      वापस HB_UNICODE_COMBINING_CLASS_BELOW;

  }

  वापस klass;
}

व्योम
_hb_ot_shape_fallback_mark_position_recategorize_marks (स्थिर hb_ot_shape_plan_t *plan HB_UNUSED,
							hb_font_t *font HB_UNUSED,
							hb_बफर_t  *बफर)
{
#अगर_घोषित HB_NO_OT_SHAPE_FALLBACK
  वापस;
#पूर्ण_अगर

  अचिन्हित पूर्णांक गिनती = बफर->len;
  hb_glyph_info_t *info = बफर->info;
  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    अगर (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      अचिन्हित पूर्णांक combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      _hb_glyph_info_set_modified_combining_class (&info[i], combining_class);
    }
}


अटल व्योम
zero_mark_advances (hb_बफर_t *बफर,
		    अचिन्हित पूर्णांक start,
		    अचिन्हित पूर्णांक end,
		    bool adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = बफर->info;
  क्रम (अचिन्हित पूर्णांक i = start; i < end; i++)
    अगर (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      अगर (adjust_offsets_when_zeroing)
      {
	बफर->pos[i].x_offset = hb_saturate_sub (बफर->pos[i].x_offset,
						     बफर->pos[i].x_advance);
	बफर->pos[i].y_offset = hb_saturate_sub (बफर->pos[i].y_offset,
						     बफर->pos[i].y_advance);
      }
      बफर->pos[i].x_advance = 0;
      बफर->pos[i].y_advance = 0;
    }
}

अटल अंतरभूत व्योम
position_mark (स्थिर hb_ot_shape_plan_t *plan HB_UNUSED,
	       hb_font_t *font,
	       hb_बफर_t  *बफर,
	       hb_glyph_extents_t &base_extents,
	       अचिन्हित पूर्णांक i,
	       अचिन्हित पूर्णांक combining_class)
{
  hb_glyph_extents_t mark_extents;
  अगर (!font->get_glyph_extents (बफर->info[i].codepoint, &mark_extents))
    वापस;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = बफर->pos[i];
  pos.x_offset = pos.y_offset = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  चयन (combining_class)
  {
    हाल HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    हाल HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      अगर (बफर->props.direction == HB_DIRECTION_LTR) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	अवरोध;
      } अन्यथा अगर (बफर->props.direction == HB_DIRECTION_RTL) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	अवरोध;
      }
      HB_FALLTHROUGH;

    शेष:
    हाल HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
    हाल HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    हाल HB_UNICODE_COMBINING_CLASS_BELOW:
    हाल HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing +
						((int64_t) base_extents.width - mark_extents.width) / 2 -
						mark_extents.x_bearing);
      अवरोध;

    हाल HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_बायाँ:
    हाल HB_UNICODE_COMBINING_CLASS_BELOW_बायाँ:
    हाल HB_UNICODE_COMBINING_CLASS_ABOVE_बायाँ:
      /* Left align. */
      pos.x_offset = hb_saturate_sub (base_extents.x_bearing, mark_extents.x_bearing);
      अवरोध;

    हाल HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_दायाँ:
    हाल HB_UNICODE_COMBINING_CLASS_BELOW_दायाँ:
    हाल HB_UNICODE_COMBINING_CLASS_ABOVE_दायाँ:
      /* Right align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						mark_extents.width - mark_extents.x_bearing);
      अवरोध;
  }

  /* Y positioning */
  चयन (combining_class)
  {
    हाल HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    हाल HB_UNICODE_COMBINING_CLASS_BELOW_बायाँ:
    हाल HB_UNICODE_COMBINING_CLASS_BELOW:
    हाल HB_UNICODE_COMBINING_CLASS_BELOW_दायाँ:
      /* Add gap, fall-through. */
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    हाल HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_बायाँ:
    हाल HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing +
						base_extents.height - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      अगर ((y_gap > 0) == (pos.y_offset > 0))
      {
	base_extents.height = hb_saturate_sub (base_extents.height, pos.y_offset);
	pos.y_offset = 0;
      }
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      अवरोध;

    हाल HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    हाल HB_UNICODE_COMBINING_CLASS_ABOVE_बायाँ:
    हाल HB_UNICODE_COMBINING_CLASS_ABOVE:
    हाल HB_UNICODE_COMBINING_CLASS_ABOVE_दायाँ:
      /* Add gap, fall-through. */
      base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, y_gap);
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    हाल HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    हाल HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_दायाँ:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing -
						mark_extents.y_bearing - mark_extents.height);
      /* Don't shift down "above" marks too much. */
      अगर ((y_gap > 0) != (pos.y_offset > 0))
      {
	hb_position_t correction = -(pos.y_offset / 2);
	base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, correction);
	base_extents.height = hb_saturate_sub (base_extents.height, correction);
	pos.y_offset = hb_saturate_add (pos.y_offset, correction);
      }
      base_extents.y_bearing = hb_saturate_sub (base_extents.y_bearing, mark_extents.height);
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      अवरोध;
  }
}

अटल अंतरभूत व्योम
position_around_base (स्थिर hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_बफर_t  *बफर,
		      अचिन्हित पूर्णांक base,
		      अचिन्हित पूर्णांक end,
		      bool adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  बफर->unsafe_to_break (base, end);

  hb_glyph_extents_t base_extents;
  अगर (!font->get_glyph_extents (बफर->info[base].codepoint,
				&base_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (बफर, base + 1, end, adjust_offsets_when_zeroing);
    वापस;
  }
  base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing,
					    बफर->pos[base].y_offset);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  base_extents.x_bearing = 0;
  base_extents.width = font->get_glyph_h_advance (बफर->info[base].codepoint);

  अचिन्हित पूर्णांक lig_id = _hb_glyph_info_get_lig_id (&बफर->info[base]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  पूर्णांक अंक_lig_components = hb_glyph_info_get_lig_अंक_comps (&बफर->info[base]);

  hb_position_t x_offset = 0, y_offset = 0;
  अगर (HB_DIRECTION_IS_FORWARD (बफर->props.direction)) {
    x_offset = hb_saturate_neg (बफर->pos[base].x_advance);
    y_offset = hb_saturate_neg (बफर->pos[base].y_advance);
  }

  hb_glyph_extents_t component_extents = base_extents;
  पूर्णांक last_lig_component = -1;
  अचिन्हित पूर्णांक last_combining_class = 255;
  hb_glyph_extents_t cluster_extents = base_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = बफर->info;
  क्रम (अचिन्हित पूर्णांक i = base + 1; i < end; i++)
    अगर (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      अगर (अंक_lig_components > 1) {
	अचिन्हित पूर्णांक this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	पूर्णांक this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
	/* Conditions for attaching to the last component. */
	अगर (!lig_id || lig_id != this_lig_id || this_lig_component >= अंक_lig_components)
	  this_lig_component = अंक_lig_components - 1;
	अगर (last_lig_component != this_lig_component)
	{
	  last_lig_component = this_lig_component;
	  last_combining_class = 255;
	  component_extents = base_extents;
	  अगर (unlikely (horiz_dir == HB_DIRECTION_INVALID)) {
	    अगर (HB_DIRECTION_IS_HORIZONTAL (plan->props.direction))
	      horiz_dir = plan->props.direction;
	    अन्यथा
	      horiz_dir = hb_script_get_horizontal_direction (plan->props.script);
	  }
	  अगर (horiz_dir == HB_DIRECTION_LTR)
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) this_lig_component * component_extents.width / अंक_lig_components);
	  अन्यथा
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) (अंक_lig_components - 1 - this_lig_component) * component_extents.width / अंक_lig_components);
	  component_extents.width /= अंक_lig_components;
	}
      }

      अचिन्हित पूर्णांक this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      अगर (last_combining_class != this_combining_class)
      {
	last_combining_class = this_combining_class;
	cluster_extents = component_extents;
      }

      position_mark (plan, font, बफर, cluster_extents, i, this_combining_class);

      बफर->pos[i].x_advance = 0;
      बफर->pos[i].y_advance = 0;
      बफर->pos[i].x_offset = hb_saturate_add (बफर->pos[i].x_offset, x_offset);
      बफर->pos[i].y_offset = hb_saturate_add (बफर->pos[i].y_offset, y_offset);

    } अन्यथा {
      अगर (HB_DIRECTION_IS_FORWARD (बफर->props.direction)) {
	x_offset = hb_saturate_sub (x_offset, बफर->pos[i].x_advance);
	y_offset = hb_saturate_sub (y_offset, बफर->pos[i].y_advance);
      } अन्यथा {
	x_offset = hb_saturate_add (x_offset, बफर->pos[i].x_advance);
	y_offset = hb_saturate_add (y_offset, बफर->pos[i].y_advance);
      }
    }
}

अटल अंतरभूत व्योम
position_cluster_impl (स्थिर hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_बफर_t  *बफर,
		       अचिन्हित पूर्णांक start,
		       अचिन्हित पूर्णांक end,
		       bool adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = बफर->info;
  क्रम (अचिन्हित पूर्णांक i = start; i < end; i++)
    अगर (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      अचिन्हित पूर्णांक j;
      क्रम (j = i + 1; j < end; j++)
      {
        अगर (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  जारी;
	अगर (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  अवरोध;
      }

      position_around_base (plan, font, बफर, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

अटल HB_ALWAYS_INLINE व्योम
position_cluster (स्थिर hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_बफर_t  *बफर,
		  अचिन्हित पूर्णांक start,
		  अचिन्हित पूर्णांक end,
		  bool adjust_offsets_when_zeroing)
{
  अगर (end - start < 2)
    वापस;

  position_cluster_impl (plan, font, बफर, start, end, adjust_offsets_when_zeroing);
}

व्योम
_hb_ot_shape_fallback_mark_position (स्थिर hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_बफर_t  *बफर,
				     bool adjust_offsets_when_zeroing)
{
#अगर_घोषित HB_NO_OT_SHAPE_FALLBACK
  वापस;
#पूर्ण_अगर

  अगर (!बफर->message (font, "start fallback mark"))
    वापस;

  hb_बफर_assert_gsubgpos_vars (बफर);

  अचिन्हित पूर्णांक start = 0;
  अचिन्हित पूर्णांक गिनती = बफर->len;
  hb_glyph_info_t *info = बफर->info;
  क्रम (अचिन्हित पूर्णांक i = 1; i < गिनती; i++)
    अगर (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, बफर, start, i, adjust_offsets_when_zeroing);
      start = i;
    }
  position_cluster (plan, font, बफर, start, गिनती, adjust_offsets_when_zeroing);

  (व्योम) बफर->message (font, "end fallback mark");
}


#अगर_अघोषित HB_DISABLE_DEPRECATED
काष्ठा hb_ot_shape_fallback_kern_driver_t
{
  hb_ot_shape_fallback_kern_driver_t (hb_font_t   *font_,
				      hb_बफर_t *बफर) :
    font (font_), direction (बफर->props.direction) {}

  hb_position_t get_kerning (hb_codepoint_t first, hb_codepoint_t second) स्थिर
  {
    hb_position_t kern = 0;
    font->get_glyph_kerning_for_direction (first, second,
					   direction,
					   &kern, &kern);
    वापस kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#पूर्ण_अगर

/* Performs font-assisted kerning. */
व्योम
_hb_ot_shape_fallback_kern (स्थिर hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_बफर_t *बफर)
{
#अगर_घोषित HB_NO_OT_SHAPE_FALLBACK
  वापस;
#पूर्ण_अगर

#अगर_अघोषित HB_DISABLE_DEPRECATED
  अगर (HB_DIRECTION_IS_HORIZONTAL (बफर->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    वापस;

  अगर (!बफर->message (font, "start fallback kern"))
    वापस;

  bool reverse = HB_DIRECTION_IS_BACKWARD (बफर->props.direction);

  अगर (reverse)
    बफर->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, बफर);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, बफर, plan->kern_mask, false);

  अगर (reverse)
    बफर->reverse ();

  (व्योम) बफर->message (font, "end fallback kern");
#पूर्ण_अगर
}


/* Adjusts width of various spaces. */
व्योम
_hb_ot_shape_fallback_spaces (स्थिर hb_ot_shape_plan_t *plan HB_UNUSED,
			      hb_font_t *font,
			      hb_बफर_t  *बफर)
{
  hb_glyph_info_t *info = बफर->info;
  hb_glyph_position_t *pos = बफर->pos;
  bool horizontal = HB_DIRECTION_IS_HORIZONTAL (बफर->props.direction);
  अचिन्हित पूर्णांक गिनती = बफर->len;
  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    अगर (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      अगर (बफर->invisible && info[i].codepoint == बफर->invisible)
      {
        अगर (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        अन्यथा
	  pos[i].y_advance = -(int64_t) font->y_scale / 4;
      }

      hb_unicode_funcs_t::space_t space_type = _hb_glyph_info_get_unicode_space_fallback_type (&info[i]);
      hb_codepoint_t glyph;
      प्रकार hb_unicode_funcs_t t;
      चयन (space_type)
      {
	हाल t::NOT_SPACE: /* Shouldn't happen. */
	हाल t::SPACE:
	  अवरोध;

	हाल t::SPACE_EM:
	हाल t::SPACE_EM_2:
	हाल t::SPACE_EM_3:
	हाल t::SPACE_EM_4:
	हाल t::SPACE_EM_5:
	हाल t::SPACE_EM_6:
	हाल t::SPACE_EM_16:
	  अगर (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((पूर्णांक) space_type)/2) / (पूर्णांक) space_type;
	  अन्यथा
	    pos[i].y_advance = -((int64_t) font->y_scale + ((पूर्णांक) space_type)/2) / (पूर्णांक) space_type;
	  अवरोध;

	हाल t::SPACE_4_EM_18:
	  अगर (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  अन्यथा
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  अवरोध;

	हाल t::SPACE_FIGURE:
	  क्रम (अक्षर u = '0'; u <= '9'; u++)
	    अगर (font->get_nominal_glyph (u, &glyph))
	    {
	      अगर (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      अन्यथा
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      अवरोध;
	    }
	  अवरोध;

	हाल t::SPACE_PUNCTUATION:
	  अगर (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    अगर (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    अन्यथा
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  अवरोध;

	हाल t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  अगर (horizontal)
	    pos[i].x_advance /= 2;
	  अन्यथा
	    pos[i].y_advance /= 2;
	  अवरोध;
      }
    }
}


#पूर्ण_अगर
