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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shape-fallback.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-kern.hh"

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
recategorize_combining_class (hb_codepoint_t u,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ klass)
{
  ਜੇ (klass >= 200)
    ਵਾਪਸ klass;

  /* Thai / Lao need some per-character work. */
  ਜੇ ((u & ~0xFF) == 0x0E00u)
  {
    ਜੇ (unlikely (klass == 0))
    {
      ਚੋਣ (u)
      {
	ਹਾਲਤ 0x0E31u:
	ਹਾਲਤ 0x0E34u:
	ਹਾਲਤ 0x0E35u:
	ਹਾਲਤ 0x0E36u:
	ਹਾਲਤ 0x0E37u:
	ਹਾਲਤ 0x0E47u:
	ਹਾਲਤ 0x0E4Cu:
	ਹਾਲਤ 0x0E4Du:
	ਹਾਲਤ 0x0E4Eu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_ਸੱਜਾ;
	  ਰੋਕੋ;

	ਹਾਲਤ 0x0EB1u:
	ਹਾਲਤ 0x0EB4u:
	ਹਾਲਤ 0x0EB5u:
	ਹਾਲਤ 0x0EB6u:
	ਹਾਲਤ 0x0EB7u:
	ਹਾਲਤ 0x0EBBu:
	ਹਾਲਤ 0x0ECCu:
	ਹਾਲਤ 0x0ECDu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE;
	  ਰੋਕੋ;

	ਹਾਲਤ 0x0EBCu:
	  klass = HB_UNICODE_COMBINING_CLASS_ਹੇਠਾਂ;
	  ਰੋਕੋ;
      }
    } ਨਹੀਂਤਾਂ {
      /* Thai virama is below-right */
      ਜੇ (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_ਹੇਠਾਂ_ਸੱਜਾ;
    }
  }

  ਚੋਣ (klass)
  {

    /* Hebrew */

    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC10: /* sheva */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC11: /* hataf segol */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC12: /* hataf patah */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC13: /* hataf qamats */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC14: /* hiriq */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC15: /* tsere */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC16: /* segol */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC17: /* patah */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC18: /* qamats & qamats qatan */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC20: /* qubuts */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC22: /* meteg */
      ਵਾਪਸ HB_UNICODE_COMBINING_CLASS_ਹੇਠਾਂ;

    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      ਵਾਪਸ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      ਵਾਪਸ HB_UNICODE_COMBINING_CLASS_ABOVE_ਸੱਜਾ;

    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      ਵਾਪਸ HB_UNICODE_COMBINING_CLASS_ABOVE_ਖੱਬਾ;

    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      ਵਾਪਸ HB_UNICODE_COMBINING_CLASS_ABOVE;

    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC21: /* dagesh */
      ਰੋਕੋ;


    /* Arabic and Syriac */

    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC27: /* fathatan */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC28: /* dammatan */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC30: /* fatha */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC31: /* damma */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC33: /* shadda */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC34: /* sukun */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC35: /* superscript alef */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC36: /* superscript alaph */
      ਵਾਪਸ HB_UNICODE_COMBINING_CLASS_ABOVE;

    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      ਵਾਪਸ HB_UNICODE_COMBINING_CLASS_ਹੇਠਾਂ;


    /* Thai */

    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      ਵਾਪਸ HB_UNICODE_COMBINING_CLASS_ਹੇਠਾਂ_ਸੱਜਾ;

    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      ਵਾਪਸ HB_UNICODE_COMBINING_CLASS_ABOVE_ਸੱਜਾ;


    /* Lao */

    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      ਵਾਪਸ HB_UNICODE_COMBINING_CLASS_ਹੇਠਾਂ;

    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      ਵਾਪਸ HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      ਵਾਪਸ HB_UNICODE_COMBINING_CLASS_ਹੇਠਾਂ;

    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      ਵਾਪਸ HB_UNICODE_COMBINING_CLASS_ABOVE;

    ਹਾਲਤ HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      ਵਾਪਸ HB_UNICODE_COMBINING_CLASS_ਹੇਠਾਂ;

  }

  ਵਾਪਸ klass;
}

ਖਾਲੀ
_hb_ot_shape_fallback_mark_position_recategorize_marks (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
							hb_font_t *font HB_ਅਣਵਰਤਿਆ,
							hb_ਬਫਰ_t  *ਬਫਰ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE_FALLBACK
  ਵਾਪਸ;
#ਜੇ_ਅੰਤ

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    ਜੇ (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      hb_glyph_info_ਸੈੱਟ_modified_combining_class (&info[i], combining_class);
    }
}


ਸਥਿਰ ਖਾਲੀ
zero_mark_advances (hb_ਬਫਰ_t *ਬਫਰ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ,
		    ਤਾਰਕਿਕ adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
    ਜੇ (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      ਜੇ (adjust_offsets_when_zeroing)
      {
	ਬਫਰ->pos[i].x_ਵਿੱਥ_2 = hb_saturate_sub (ਬਫਰ->pos[i].x_ਵਿੱਥ_2,
						     ਬਫਰ->pos[i].x_advance);
	ਬਫਰ->pos[i].y_ਵਿੱਥ_2 = hb_saturate_sub (ਬਫਰ->pos[i].y_ਵਿੱਥ_2,
						     ਬਫਰ->pos[i].y_advance);
      }
      ਬਫਰ->pos[i].x_advance = 0;
      ਬਫਰ->pos[i].y_advance = 0;
    }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
position_mark (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
	       hb_font_t *font,
	       hb_ਬਫਰ_t  *ਬਫਰ,
	       hb_glyph_extents_t &ਅਧਾਰ_extents,
	       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i,
	       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ combining_class)
{
  hb_glyph_extents_t mark_extents;
  ਜੇ (!font->get_glyph_extents (ਬਫਰ->info[i].codepoint, &mark_extents))
    ਵਾਪਸ;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = ਬਫਰ->pos[i];
  pos.x_ਵਿੱਥ_2 = pos.y_ਵਿੱਥ_2 = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  ਚੋਣ (combining_class)
  {
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_DOUBLE_ਹੇਠਾਂ:
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      ਜੇ (ਬਫਰ->props.direction == HB_DIRECTION_LTR) {
	pos.x_ਵਿੱਥ_2 = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) ਅਧਾਰ_extents.x_bearing + ਅਧਾਰ_extents.ਚੌੜਾਈ -
						    mark_extents.ਚੌੜਾਈ / 2 - mark_extents.x_bearing);
	ਰੋਕੋ;
      } ਨਹੀਂਤਾਂ ਜੇ (ਬਫਰ->props.direction == HB_DIRECTION_RTL) {
	pos.x_ਵਿੱਥ_2 = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) ਅਧਾਰ_extents.x_bearing -
						    mark_extents.ਚੌੜਾਈ / 2 - mark_extents.x_bearing);
	ਰੋਕੋ;
      }
      HB_FALLTHROUGH;

    ਮੂਲ:
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ATTACHED_ਹੇਠਾਂ:
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ਹੇਠਾਂ:
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_ਵਿੱਥ_2 = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) ਅਧਾਰ_extents.x_bearing +
						((int64_t) ਅਧਾਰ_extents.ਚੌੜਾਈ - mark_extents.ਚੌੜਾਈ) / 2 -
						mark_extents.x_bearing);
      ਰੋਕੋ;

    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ATTACHED_ਹੇਠਾਂ_ਖੱਬਾ:
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ਹੇਠਾਂ_ਖੱਬਾ:
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ABOVE_ਖੱਬਾ:
      /* Left align. */
      pos.x_ਵਿੱਥ_2 = hb_saturate_sub (ਅਧਾਰ_extents.x_bearing, mark_extents.x_bearing);
      ਰੋਕੋ;

    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_ਸੱਜਾ:
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ਹੇਠਾਂ_ਸੱਜਾ:
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ABOVE_ਸੱਜਾ:
      /* Right align. */
      pos.x_ਵਿੱਥ_2 = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) ਅਧਾਰ_extents.x_bearing + ਅਧਾਰ_extents.ਚੌੜਾਈ -
						mark_extents.ਚੌੜਾਈ - mark_extents.x_bearing);
      ਰੋਕੋ;
  }

  /* Y positioning */
  ਚੋਣ (combining_class)
  {
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_DOUBLE_ਹੇਠਾਂ:
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ਹੇਠਾਂ_ਖੱਬਾ:
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ਹੇਠਾਂ:
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ਹੇਠਾਂ_ਸੱਜਾ:
      /* Add gap, fall-through. */
      ਅਧਾਰ_extents.ਉਚਾਈ = hb_saturate_sub (ਅਧਾਰ_extents.ਉਚਾਈ, y_gap);
      HB_FALLTHROUGH;

    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ATTACHED_ਹੇਠਾਂ_ਖੱਬਾ:
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ATTACHED_ਹੇਠਾਂ:
      pos.y_ਵਿੱਥ_2 = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) ਅਧਾਰ_extents.y_bearing +
						ਅਧਾਰ_extents.ਉਚਾਈ - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      ਜੇ ((y_gap > 0) == (pos.y_ਵਿੱਥ_2 > 0))
      {
	ਅਧਾਰ_extents.ਉਚਾਈ = hb_saturate_sub (ਅਧਾਰ_extents.ਉਚਾਈ, pos.y_ਵਿੱਥ_2);
	pos.y_ਵਿੱਥ_2 = 0;
      }
      ਅਧਾਰ_extents.ਉਚਾਈ = hb_saturate_ਜੋੜੋ (ਅਧਾਰ_extents.ਉਚਾਈ, mark_extents.ਉਚਾਈ);
      ਰੋਕੋ;

    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ABOVE_ਖੱਬਾ:
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ABOVE:
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ABOVE_ਸੱਜਾ:
      /* Add gap, fall-through. */
      ਅਧਾਰ_extents.y_bearing = hb_saturate_ਜੋੜੋ (ਅਧਾਰ_extents.y_bearing, y_gap);
      ਅਧਾਰ_extents.ਉਚਾਈ = hb_saturate_sub (ਅਧਾਰ_extents.ਉਚਾਈ, y_gap);
      HB_FALLTHROUGH;

    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    ਹਾਲਤ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_ਸੱਜਾ:
      pos.y_ਵਿੱਥ_2 = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) ਅਧਾਰ_extents.y_bearing -
						mark_extents.y_bearing - mark_extents.ਉਚਾਈ);
      /* Don't shift down "above" marks too much. */
      ਜੇ ((y_gap > 0) != (pos.y_ਵਿੱਥ_2 > 0))
      {
	hb_position_t correction = -(pos.y_ਵਿੱਥ_2 / 2);
	ਅਧਾਰ_extents.y_bearing = hb_saturate_ਜੋੜੋ (ਅਧਾਰ_extents.y_bearing, correction);
	ਅਧਾਰ_extents.ਉਚਾਈ = hb_saturate_sub (ਅਧਾਰ_extents.ਉਚਾਈ, correction);
	pos.y_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (pos.y_ਵਿੱਥ_2, correction);
      }
      ਅਧਾਰ_extents.y_bearing = hb_saturate_sub (ਅਧਾਰ_extents.y_bearing, mark_extents.ਉਚਾਈ);
      ਅਧਾਰ_extents.ਉਚਾਈ = hb_saturate_ਜੋੜੋ (ਅਧਾਰ_extents.ਉਚਾਈ, mark_extents.ਉਚਾਈ);
      ਰੋਕੋ;
  }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
position_around_ਅਧਾਰ (ਅਚਲ hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_ਬਫਰ_t  *ਬਫਰ,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਧਾਰ,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ,
		      ਤਾਰਕਿਕ adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (ਅਧਾਰ, ਸਿਰਾ);

  hb_glyph_extents_t ਅਧਾਰ_extents;
  ਜੇ (!font->get_glyph_extents (ਬਫਰ->info[ਅਧਾਰ].codepoint,
				&ਅਧਾਰ_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (ਬਫਰ, ਅਧਾਰ + 1, ਸਿਰਾ, adjust_offsets_when_zeroing);
    ਵਾਪਸ;
  }
  ਅਧਾਰ_extents.y_bearing = hb_saturate_ਜੋੜੋ (ਅਧਾਰ_extents.y_bearing,
					    ਬਫਰ->pos[ਅਧਾਰ].y_ਵਿੱਥ_2);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  ਅਧਾਰ_extents.x_bearing = 0;
  ਅਧਾਰ_extents.ਚੌੜਾਈ = font->get_glyph_h_advance (ਬਫਰ->info[ਅਧਾਰ].codepoint);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lig_id = _hb_glyph_info_get_lig_id (&ਬਫਰ->info[ਅਧਾਰ]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  ਪੂਰਨ ਅੰਕ_lig_components = hb_glyph_info_get_lig_ਅੰਕ_comps (&ਬਫਰ->info[ਅਧਾਰ]);

  hb_position_t x_ਵਿੱਥ_2 = 0, y_ਵਿੱਥ_2 = 0;
  ਜੇ (HB_DIRECTION_IS_FORWARD (ਬਫਰ->props.direction)) {
    x_ਵਿੱਥ_2 = hb_saturate_neg (ਬਫਰ->pos[ਅਧਾਰ].x_advance);
    y_ਵਿੱਥ_2 = hb_saturate_neg (ਬਫਰ->pos[ਅਧਾਰ].y_advance);
  }

  hb_glyph_extents_t component_extents = ਅਧਾਰ_extents;
  ਪੂਰਨ ਆਖਰੀ_lig_component = -1;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ_combining_class = 255;
  hb_glyph_extents_t cluster_extents = ਅਧਾਰ_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਅਧਾਰ + 1; i < ਸਿਰਾ; i++)
    ਜੇ (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      ਜੇ (ਅੰਕ_lig_components > 1) {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	ਪੂਰਨ this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
	/* Conditions for attaching to the last component. */
	ਜੇ (!lig_id || lig_id != this_lig_id || this_lig_component >= ਅੰਕ_lig_components)
	  this_lig_component = ਅੰਕ_lig_components - 1;
	ਜੇ (ਆਖਰੀ_lig_component != this_lig_component)
	{
	  ਆਖਰੀ_lig_component = this_lig_component;
	  ਆਖਰੀ_combining_class = 255;
	  component_extents = ਅਧਾਰ_extents;
	  ਜੇ (unlikely (horiz_dir == HB_DIRECTION_INVALID)) {
	    ਜੇ (HB_DIRECTION_IS_HORIZONTAL (plan->props.direction))
	      horiz_dir = plan->props.direction;
	    ਨਹੀਂਤਾਂ
	      horiz_dir = hb_script_get_horizontal_direction (plan->props.script);
	  }
	  ਜੇ (horiz_dir == HB_DIRECTION_LTR)
	    component_extents.x_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) this_lig_component * component_extents.ਚੌੜਾਈ / ਅੰਕ_lig_components);
	  ਨਹੀਂਤਾਂ
	    component_extents.x_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) (ਅੰਕ_lig_components - 1 - this_lig_component) * component_extents.ਚੌੜਾਈ / ਅੰਕ_lig_components);
	  component_extents.ਚੌੜਾਈ /= ਅੰਕ_lig_components;
	}
      }

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      ਜੇ (ਆਖਰੀ_combining_class != this_combining_class)
      {
	ਆਖਰੀ_combining_class = this_combining_class;
	cluster_extents = component_extents;
      }

      position_mark (plan, font, ਬਫਰ, cluster_extents, i, this_combining_class);

      ਬਫਰ->pos[i].x_advance = 0;
      ਬਫਰ->pos[i].y_advance = 0;
      ਬਫਰ->pos[i].x_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (ਬਫਰ->pos[i].x_ਵਿੱਥ_2, x_ਵਿੱਥ_2);
      ਬਫਰ->pos[i].y_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (ਬਫਰ->pos[i].y_ਵਿੱਥ_2, y_ਵਿੱਥ_2);

    } ਨਹੀਂਤਾਂ {
      ਜੇ (HB_DIRECTION_IS_FORWARD (ਬਫਰ->props.direction)) {
	x_ਵਿੱਥ_2 = hb_saturate_sub (x_ਵਿੱਥ_2, ਬਫਰ->pos[i].x_advance);
	y_ਵਿੱਥ_2 = hb_saturate_sub (y_ਵਿੱਥ_2, ਬਫਰ->pos[i].y_advance);
      } ਨਹੀਂਤਾਂ {
	x_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (x_ਵਿੱਥ_2, ਬਫਰ->pos[i].x_advance);
	y_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (y_ਵਿੱਥ_2, ਬਫਰ->pos[i].y_advance);
      }
    }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
position_cluster_impl (ਅਚਲ hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_ਬਫਰ_t  *ਬਫਰ,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ,
		       ਤਾਰਕਿਕ adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
    ਜੇ (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j;
      ਲਈ (j = i + 1; j < ਸਿਰਾ; j++)
      {
        ਜੇ (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  ਜਾਰੀ;
	ਜੇ (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  ਰੋਕੋ;
      }

      position_around_ਅਧਾਰ (plan, font, ਬਫਰ, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

ਸਥਿਰ HB_ALWAYS_INLINE ਖਾਲੀ
position_cluster (ਅਚਲ hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_ਬਫਰ_t  *ਬਫਰ,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ,
		  ਤਾਰਕਿਕ adjust_offsets_when_zeroing)
{
  ਜੇ (ਸਿਰਾ - ਸ਼ੁਰੂ < 2)
    ਵਾਪਸ;

  position_cluster_impl (plan, font, ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ, adjust_offsets_when_zeroing);
}

ਖਾਲੀ
_hb_ot_shape_fallback_mark_position (ਅਚਲ hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_ਬਫਰ_t  *ਬਫਰ,
				     ਤਾਰਕਿਕ adjust_offsets_when_zeroing)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE_FALLBACK
  ਵਾਪਸ;
#ਜੇ_ਅੰਤ

  ਜੇ (!ਬਫਰ->ਸੁਨੇਹਾ (font, "start fallback mark"))
    ਵਾਪਸ;

  hb_ਬਫਰ_assert_gsubgpos_vars (ਬਫਰ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ਗਿਣਤੀ_2; i++)
    ਜੇ (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, ਬਫਰ, ਸ਼ੁਰੂ, i, adjust_offsets_when_zeroing);
      ਸ਼ੁਰੂ = i;
    }
  position_cluster (plan, font, ਬਫਰ, ਸ਼ੁਰੂ, ਗਿਣਤੀ_2, adjust_offsets_when_zeroing);

  (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end fallback mark");
}


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
ਬਣਤਰ hb_ot_shape_fallback_kern_driver_t
{
  hb_ot_shape_fallback_kern_driver_t (hb_font_t   *font_,
				      hb_ਬਫਰ_t *ਬਫਰ) :
    font (font_), direction (ਬਫਰ->props.direction) {}

  hb_position_t get_kerning (hb_codepoint_t ਪਹਿਲਾ, hb_codepoint_t second) ਅਚਲ
  {
    hb_position_t kern = 0;
    font->get_glyph_kerning_for_direction (ਪਹਿਲਾ, second,
					   direction,
					   &kern, &kern);
    ਵਾਪਸ kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#ਜੇ_ਅੰਤ

/* Performs font-assisted kerning. */
ਖਾਲੀ
_hb_ot_shape_fallback_kern (ਅਚਲ hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_ਬਫਰ_t *ਬਫਰ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE_FALLBACK
  ਵਾਪਸ;
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
  ਜੇ (HB_DIRECTION_IS_HORIZONTAL (ਬਫਰ->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    ਵਾਪਸ;

  ਜੇ (!ਬਫਰ->ਸੁਨੇਹਾ (font, "start fallback kern"))
    ਵਾਪਸ;

  ਤਾਰਕਿਕ reverse = HB_DIRECTION_IS_BACKWARD (ਬਫਰ->props.direction);

  ਜੇ (reverse)
    ਬਫਰ->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, ਬਫਰ);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, ਬਫਰ, plan->kern_ਨਕਾਬ, ਝੂਠ);

  ਜੇ (reverse)
    ਬਫਰ->reverse ();

  (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end fallback kern");
#ਜੇ_ਅੰਤ
}


/* Adjusts width of various spaces. */
ਖਾਲੀ
_hb_ot_shape_fallback_spaces (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
			      hb_font_t *font,
			      hb_ਬਫਰ_t  *ਬਫਰ)
{
  hb_glyph_info_t *info = ਬਫਰ->info;
  hb_glyph_position_t *pos = ਬਫਰ->pos;
  ਤਾਰਕਿਕ horizontal = HB_DIRECTION_IS_HORIZONTAL (ਬਫਰ->props.direction);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    ਜੇ (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      ਜੇ (ਬਫਰ->invisible && info[i].codepoint == ਬਫਰ->invisible)
      {
        ਜੇ (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        ਨਹੀਂਤਾਂ
	  pos[i].y_advance = -(int64_t) font->y_scale / 4;
      }

      hb_unicode_funcs_t::space_t space_type = _hb_glyph_info_get_unicode_space_fallback_type (&info[i]);
      hb_codepoint_t glyph;
      ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_unicode_funcs_t t;
      ਚੋਣ (space_type)
      {
	ਹਾਲਤ t::NOT_SPACE: /* Shouldn't happen. */
	ਹਾਲਤ t::SPACE:
	  ਰੋਕੋ;

	ਹਾਲਤ t::SPACE_EM:
	ਹਾਲਤ t::SPACE_EM_2:
	ਹਾਲਤ t::SPACE_EM_3:
	ਹਾਲਤ t::SPACE_EM_4:
	ਹਾਲਤ t::SPACE_EM_5:
	ਹਾਲਤ t::SPACE_EM_6:
	ਹਾਲਤ t::SPACE_EM_16:
	  ਜੇ (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((ਪੂਰਨ) space_type)/2) / (ਪੂਰਨ) space_type;
	  ਨਹੀਂਤਾਂ
	    pos[i].y_advance = -((int64_t) font->y_scale + ((ਪੂਰਨ) space_type)/2) / (ਪੂਰਨ) space_type;
	  ਰੋਕੋ;

	ਹਾਲਤ t::SPACE_4_EM_18:
	  ਜੇ (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  ਨਹੀਂਤਾਂ
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  ਰੋਕੋ;

	ਹਾਲਤ t::SPACE_FIGURE:
	  ਲਈ (ਅੱਖਰ u = '0'; u <= '9'; u++)
	    ਜੇ (font->get_nominal_glyph (u, &glyph))
	    {
	      ਜੇ (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      ਨਹੀਂਤਾਂ
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      ਰੋਕੋ;
	    }
	  ਰੋਕੋ;

	ਹਾਲਤ t::SPACE_PUNCTUATION:
	  ਜੇ (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    ਜੇ (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    ਨਹੀਂਤਾਂ
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  ਰੋਕੋ;

	ਹਾਲਤ t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  ਜੇ (horizontal)
	    pos[i].x_advance /= 2;
	  ਨਹੀਂਤਾਂ
	    pos[i].y_advance /= 2;
	  ਰੋਕੋ;
      }
    }
}


#ਜੇ_ਅੰਤ
