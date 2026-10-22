/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2012,2013  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#اگر_غیر_تعریف_شدہ HB_OT_LAYOUT_HH
#تعریف_کرو HB_OT_LAYOUT_HH

#شامل "hb.hh"

#شامل "hb-font.hh"
#شامل "hb-buffer.hh"
#شامل "hb-open-type.hh"
#شامل "hb-ot-shape.hh"
#شامل "hb-set-digest.hh"


ساخت hb_ot_shape_plan_t;


/*
 * kern
 */

HB_اندرونی منطقی
hb_ot_layout_has_kerning (hb_face_t *face);

HB_اندرونی منطقی
hb_ot_layout_has_machine_kerning (hb_face_t *face);

HB_اندرونی منطقی
hb_ot_layout_has_cross_kerning (hb_face_t *face);

HB_اندرونی خالی
hb_ot_layout_kern (ثابت hb_ot_shape_plan_t *plan,
		   hb_font_t *font,
		   hb_بفر_t  *بفر);


/* Private API corresponding to hb-ot-layout.h: */

HB_اندرونی منطقی
hb_ot_layout_جدول_ڈھونڈو_feature (hb_face_t    *face,
				 hb_لیبل_t      جدول_لیبل_2,
				 hb_لیبل_t      feature_لیبل,
				 غیرعلامتی صحیح *feature_اشاریہ);


/*
 * GDEF
 */

شمار hb_ot_layout_glyph_props_flags_t
{
  /* The following three match LookupFlags::Ignore* numbers. */
  HB_OT_LAYOUT_GLYPH_PROPS_بنیاد_GLYPH	= 0x02u,
  HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE	= 0x04u,
  HB_OT_LAYOUT_GLYPH_PROPS_MARK		= 0x08u,

  /* The following are used internally; not derived from GDEF. */
  HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED	= 0x10u,
  HB_OT_LAYOUT_GLYPH_PROPS_LIGATED	= 0x20u,
  HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED	= 0x40u,

  HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE     = HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED |
					  HB_OT_LAYOUT_GLYPH_PROPS_LIGATED |
					  HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED
};
HB_MARK_AS_FLAG_T (hb_ot_layout_glyph_props_flags_t);


/*
 * GSUB/GPOS
 */


/* Should be called before all the substitute_lookup's are done. */
HB_اندرونی خالی
hb_ot_layout_substitute_آغاز (hb_font_t    *font,
			       hb_بفر_t  *بفر);

HB_اندرونی خالی
hb_ot_layout_substitute_آغاز_with_digest (hb_font_t    *font,
					   hb_بفر_t  *بفر);

نامخلا OT {
  ساخت hb_ot_apply_context_t;
  ساخت hb_ot_layout_lookup_accelerator_t;
نامخلا Layout {
نامخلا GSUB_impl {
  ساخت SubstLookup;
}
}
}

HB_اندرونی خالی
hb_ot_layout_substitute_lookup (OT::hb_ot_apply_context_t *c,
				ثابت OT::Layout::GSUB_impl::SubstLookup &lookup,
				ثابت OT::hb_ot_layout_lookup_accelerator_t &accel);


/* Should be called before all the position_lookup's are done. */
HB_اندرونی خالی
hb_ot_layout_position_آغاز (hb_font_t    *font,
			     hb_بفر_t  *بفر);

/* Should be called after all the position_lookup's are done, to fini advances. */
HB_اندرونی خالی
hb_ot_layout_position_اختتام_advances (hb_font_t    *font,
				       hb_بفر_t  *بفر);

/* Should be called after hb_ot_layout_position_finish_advances, to fini offsets. */
HB_اندرونی خالی
hb_ot_layout_position_اختتام_offsets (hb_font_t    *font,
				      hb_بفر_t  *بفر);


/*
 * Buffer var routines.
 */

/* buffer var allocations, used during the entire shaping process */
#تعریف_کرو unicode_props()		var2.u16[0]

/* buffer var allocations, used during the GSUB/GPOS processing */
#تعریف_کرو glyph_props()		var1.u16[0] /* GDEF glyph properties */
#تعریف_کرو lig_props()		var1.u8[2] /* GSUB/GPOS ligature tracking */
#تعریف_کرو syllable()		var1.u8[3] /* GSUB/GPOS shaping boundaries */


/* Loop over syllables. Based on foreach_cluster(). */
#تعریف_کرو foreach_syllable(بفر, آغاز, انتہا) \
  برائے (غیرعلامتی صحیح \
       گنتی_2 = بفر->len, \
       آغاز = 0, انتہا = گنتی_2 ? hb_اگلا_syllable (بفر, 0) : 0; \
       آغاز < گنتی_2; \
       آغاز = انتہا, انتہا = hb_اگلا_syllable (بفر, آغاز))

ساکن اندرونی غیرعلامتی صحیح
hb_اگلا_syllable (hb_بفر_t *بفر, غیرعلامتی صحیح آغاز)
{
  hb_glyph_info_t *info = بفر->info;
  غیرعلامتی صحیح گنتی = آغاز + hb_min (بفر->len - آغاز,
				       (غیرعلامتی) HB_MAX_SYLLABLE_لمبائی);

  غیرعلامتی صحیح syllable = info[آغاز].syllable();
  جب (++آغاز < گنتی && syllable == info[آغاز].syllable())
    ;

  واپس آغاز;
}


/* unicode_props */

/* Design:
 * unicode_props() is a two-byte number.  The low byte includes:
 * - Modified General_Category: 5 bits.
 * - A bit each for:
 *   * Is it Default_Ignorable(); we have a modified Default_Ignorable().
 *   * Whether it's one of the four Mongolian Free Variation Selectors,
 *     CGJ, or other characters that are hidden but should not be ignored
 *     like most other Default_Ignorable()s do during GSUB matching.
 *   * Whether it's a grapheme continuation.
 *
 * The high-byte has different meanings, switched by the Gen-Cat:
 * - For Mn,Mc,Me: the modified Combining_Class.
 * - For Cf: whether it's ZWJ, ZWNJ, or something else.
 * - For Ws: index of which space character this is, if space fallback
 *   is needed, ie. we don't set this by default, only if asked to.
 *
 * Above I said "modified" General_Category. This is because we need to
 * remember Variation Selectors, and we don't have bits left. So we
 * change their Gen_Cat from Mn to Cf, and use a bit of the high byte to
 * remember them.
 */

شمار hb_unicode_props_flags_t {
  UPROPS_نقاب_GEN_CAT	= 0x001Fu,
  UPROPS_نقاب_IGNORABLE	= 0x0020u,
  UPROPS_نقاب_HIDDEN	= 0x0040u, /* MONGOLIAN FREE VARIATION SELECTOR 1..4, or TAG characters, or CGJ sometimes */
  UPROPS_نقاب_CONTINUATION=0x0080u,

  /* If GEN_CAT=FORMAT, top byte masks: */
  UPROPS_نقاب_Cf_ZWJ	= 0x0100u,
  UPROPS_نقاب_Cf_ZWNJ	= 0x0200u,
  UPROPS_نقاب_Cf_VS	= 0x0400u,
  UPROPS_نقاب_Cf_AAT_DELETED	= 0x0800u
};
HB_MARK_AS_FLAG_T (hb_unicode_props_flags_t);

ساکن اندرونی خالی
hb_glyph_info_سیٹ_unicode_props (hb_glyph_info_t *info, hb_بفر_t *بفر)
{
  hb_unicode_funcs_t *unicode = بفر->unicode;
  غیرعلامتی صحیح u = info->codepoint;
  غیرعلامتی صحیح gen_cat = (غیرعلامتی صحیح) unicode->general_category (u);
  غیرعلامتی صحیح props = gen_cat;

  اگر (u >= 0x80u)
  {
    اگر (unlikely (unicode->is_default_ignorable (u)))
    {
      بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES;
      props |=  UPROPS_نقاب_IGNORABLE;
      اگر (u == 0x200Cu) props |= UPROPS_نقاب_Cf_ZWNJ;
      ورنہ اگر (u == 0x200Du) props |= UPROPS_نقاب_Cf_ZWJ;
      /* Mongolian Free Variation Selectors need to be remembered
       * because although we need to hide them like default-ignorables,
       * they need to non-ignorable during shaping.  This is similar to
       * what we do for joiners in Indic-like shapers, but since the
       * FVSes are GC=Mn, we have use a separate bit to remember them.
       * Fixes:
       * https://github.com/harfbuzz/harfbuzz/issues/234 */
      ورنہ اگر (unlikely (hb_اندر_ranges<hb_codepoint_t> (u, 0x180Bu, 0x180Du, 0x180Fu, 0x180Fu))) props |= UPROPS_نقاب_HIDDEN;
      /* TAG characters need similar treatment. Fixes:
       * https://github.com/harfbuzz/harfbuzz/issues/463 */
      ورنہ اگر (unlikely (hb_اندر_range<hb_codepoint_t> (u, 0xE0020u, 0xE007Fu))) props |= UPROPS_نقاب_HIDDEN;
      /* COMBINING GRAPHEME JOINER should not be skipped during GSUB either.
       * https://github.com/harfbuzz/harfbuzz/issues/554 */
      ورنہ اگر (unlikely (u == 0x034Fu))
      {
	بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_CGJ;
	props |= UPROPS_نقاب_HIDDEN;
      }
    }

    اگر (unlikely (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (gen_cat)))
    {
      بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_CONTINUATIONS;
      props |= UPROPS_نقاب_CONTINUATION;
      props |= unicode->modified_combining_class (u)<<8;
    }
  }

  info->unicode_props() = props;
}

ساکن اندرونی خالی
hb_glyph_info_سیٹ_general_category (hb_glyph_info_t *info,
				     hb_unicode_general_category_t gen_cat)
{
  /* Clears top-byte. */
  info->unicode_props() = (غیرعلامتی صحیح) gen_cat | (info->unicode_props() & (0xFF & ~UPROPS_نقاب_GEN_CAT));
}

ساکن اندرونی hb_unicode_general_category_t
_hb_glyph_info_get_general_category (ثابت hb_glyph_info_t *info)
{
  واپس (hb_unicode_general_category_t) (info->unicode_props() & UPROPS_نقاب_GEN_CAT);
}

ساکن اندرونی منطقی
_hb_glyph_info_is_unicode_mark (ثابت hb_glyph_info_t *info)
{
  واپس HB_UNICODE_GENERAL_CATEGORY_IS_MARK (info->unicode_props() & UPROPS_نقاب_GEN_CAT);
}
ساکن اندرونی خالی
hb_glyph_info_سیٹ_modified_combining_class (hb_glyph_info_t *info,
					     غیرعلامتی صحیح modified_class)
{
  اگر (unlikely (!_hb_glyph_info_is_unicode_mark (info)))
    واپس;
  info->unicode_props() = (modified_class<<8) | (info->unicode_props() & 0xFF);
}
ساکن اندرونی غیرعلامتی صحیح
_hb_glyph_info_get_modified_combining_class (ثابت hb_glyph_info_t *info)
{
  واپس _hb_glyph_info_is_unicode_mark (info) ? info->unicode_props()>>8 : 0;
}
#تعریف_کرو info_cc(info) (_hb_glyph_info_get_modified_combining_class (&(info)))

ساکن اندرونی منطقی
_hb_glyph_info_is_unicode_space (ثابت hb_glyph_info_t *info)
{
  واپس _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
}
ساکن اندرونی خالی
hb_glyph_info_سیٹ_unicode_space_fallback_type (hb_glyph_info_t *info, hb_unicode_funcs_t::space_t s)
{
  اگر (unlikely (!_hb_glyph_info_is_unicode_space (info)))
    واپس;
  info->unicode_props() = (((غیرعلامتی صحیح) s)<<8) | (info->unicode_props() & 0xFF);
}
ساکن اندرونی hb_unicode_funcs_t::space_t
_hb_glyph_info_get_unicode_space_fallback_type (ثابت hb_glyph_info_t *info)
{
  واپس _hb_glyph_info_is_unicode_space (info) ?
	 (hb_unicode_funcs_t::space_t) (info->unicode_props()>>8) :
	 hb_unicode_funcs_t::NOT_SPACE;
}
ساکن اندرونی منطقی
_hb_glyph_info_is_variation_selector (ثابت hb_glyph_info_t *info)
{
  واپس _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_FORMAT &&
	 (info->unicode_props() & UPROPS_نقاب_Cf_VS);
}
ساکن اندرونی خالی
hb_glyph_info_سیٹ_variation_selector (hb_glyph_info_t *info, منطقی customize)
{
  اگر (customize)
  {
    hb_glyph_info_سیٹ_general_category (info, HB_UNICODE_GENERAL_CATEGORY_FORMAT);
    info->unicode_props() |= UPROPS_نقاب_Cf_VS;
  }
  ورنہ
  {
    // Reset to their original condition
    hb_glyph_info_سیٹ_general_category (info, HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK);
  }
}

ساکن اندرونی منطقی _hb_glyph_info_substituted (ثابت hb_glyph_info_t *info);

ساکن اندرونی منطقی
_hb_glyph_info_is_default_ignorable (ثابت hb_glyph_info_t *info)
{
  واپس (info->unicode_props() & UPROPS_نقاب_IGNORABLE) &&
	 !_hb_glyph_info_substituted (info);
}
ساکن اندرونی خالی
hb_glyph_info_سیٹ_default_ignorable (hb_glyph_info_t *info)
{
  info->unicode_props() |= UPROPS_نقاب_IGNORABLE;
}
ساکن اندرونی خالی
_hb_glyph_info_clear_default_ignorable (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_نقاب_IGNORABLE;
}
ساکن اندرونی منطقی
_hb_glyph_info_is_hidden (ثابت hb_glyph_info_t *info)
{
  واپس info->unicode_props() & UPROPS_نقاب_HIDDEN;
}
ساکن اندرونی خالی
_hb_glyph_info_unhide (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_نقاب_HIDDEN;
}

ساکن اندرونی خالی
hb_glyph_info_سیٹ_continuation (hb_glyph_info_t *info, hb_بفر_t *بفر)
{
  بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_CONTINUATIONS;
  info->unicode_props() |= UPROPS_نقاب_CONTINUATION;
}
ساکن اندرونی خالی
_hb_glyph_info_clear_continuation (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_نقاب_CONTINUATION;
}
ساکن اندرونی منطقی
_hb_glyph_info_is_continuation (ثابت hb_glyph_info_t *info)
{
  واپس info->unicode_props() & UPROPS_نقاب_CONTINUATION;
}

ساکن اندرونی منطقی
hb_grapheme_گروہ_func (ثابت hb_glyph_info_t& a HB_غیر_مستعمل,
			 ثابت hb_glyph_info_t& b)
{ واپس _hb_glyph_info_is_continuation (&b); }

#تعریف_کرو foreach_grapheme(بفر, آغاز, انتہا) \
	foreach_گروہ (بفر, آغاز, انتہا, hb_grapheme_گروہ_func)

ساکن اندرونی خالی
hb_ot_layout_الٹ_graphemes (hb_بفر_t *بفر)
{
  // MONOTONE_GRAPHEMES was already applied and is taken care of by _hb_grapheme_group_func.
  // So we just check for MONOTONE_CHARACTERS here.
  بفر->الٹ_groups (hb_grapheme_گروہ_func,
			  بفر->cluster_level == HB_بفر_CLUSTER_LEVEL_MONOTONE_CHARACTERS);
}

ساکن اندرونی منطقی
_hb_glyph_info_is_unicode_format (ثابت hb_glyph_info_t *info)
{
  واپس _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_FORMAT;
}
ساکن اندرونی منطقی
_hb_glyph_info_is_zwnj (ثابت hb_glyph_info_t *info)
{
  واپس _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_نقاب_Cf_ZWNJ);
}
ساکن اندرونی منطقی
_hb_glyph_info_is_zwj (ثابت hb_glyph_info_t *info)
{
  واپس _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_نقاب_Cf_ZWJ);
}
ساکن اندرونی منطقی
_hb_glyph_info_is_aat_deleted (ثابت hb_glyph_info_t *info)
{
  واپس _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_نقاب_Cf_AAT_DELETED);
}
ساکن اندرونی خالی
hb_glyph_info_سیٹ_aat_deleted (hb_glyph_info_t *info)
{
  hb_glyph_info_سیٹ_general_category (info, HB_UNICODE_GENERAL_CATEGORY_FORMAT);
  info->unicode_props() |= UPROPS_نقاب_Cf_AAT_DELETED;
  info->unicode_props() |= UPROPS_نقاب_HIDDEN;
}

/* lig_props: aka lig_id / lig_comp
 *
 * When a ligature is formed:
 *
 *   - The ligature glyph and any marks in between all the same newly allocated
 *     lig_id,
 *   - The ligature glyph will get lig_num_comps set to the number of components
 *   - The marks get lig_comp > 0, reflecting which component of the ligature
 *     they were applied to.
 *   - This is used in GPOS to attach marks to the right component of a ligature
 *     in MarkLigPos,
 *   - Note that when marks are ligated together, much of the above is skipped
 *     and the current lig_id reused.
 *
 * When a multiple-substitution is done:
 *
 *   - All resulting glyphs will have lig_id = 0,
 *   - The resulting glyphs will have lig_comp = 0, 1, 2, ... respectively.
 *   - This is used in GPOS to attach marks to the first component of a
 *     multiple substitution in MarkBasePos.
 *
 * The numbers are also used in GPOS to do mark-to-mark positioning only
 * to marks that belong to the same component of the same ligature.
 */

ساکن اندرونی خالی
_hb_glyph_info_clear_lig_props (hb_glyph_info_t *info)
{
  info->lig_props() = 0;
}

#تعریف_کرو IS_LIG_بنیاد 0x10

ساکن اندرونی خالی
hb_glyph_info_سیٹ_lig_props_for_ligature (hb_glyph_info_t *info,
					   غیرعلامتی صحیح lig_id,
					   غیرعلامتی صحیح lig_عدد_comps)
{
  info->lig_props() = (lig_id << 5) | IS_LIG_بنیاد | (lig_عدد_comps & 0x0F);
}

ساکن اندرونی خالی
hb_glyph_info_سیٹ_lig_props_for_mark (hb_glyph_info_t *info,
				       غیرعلامتی صحیح lig_id,
				       غیرعلامتی صحیح lig_comp)
{
  info->lig_props() = (lig_id << 5) | (lig_comp & 0x0F);
}

ساکن اندرونی خالی
hb_glyph_info_سیٹ_lig_props_for_component (hb_glyph_info_t *info, غیرعلامتی صحیح comp)
{
  hb_glyph_info_سیٹ_lig_props_for_mark (info, 0, comp);
}

ساکن اندرونی غیرعلامتی صحیح
_hb_glyph_info_get_lig_id (ثابت hb_glyph_info_t *info)
{
  واپس info->lig_props() >> 5;
}

ساکن اندرونی منطقی
hb_glyph_info_ligated_اندرونی (ثابت hb_glyph_info_t *info)
{
  واپس info->lig_props() & IS_LIG_بنیاد;
}

ساکن اندرونی غیرعلامتی صحیح
_hb_glyph_info_get_lig_comp (ثابت hb_glyph_info_t *info)
{
  اگر (hb_glyph_info_ligated_اندرونی (info))
    واپس 0;
  ورنہ
    واپس info->lig_props() & 0x0F;
}

ساکن اندرونی غیرعلامتی صحیح
hb_glyph_info_get_lig_عدد_comps (ثابت hb_glyph_info_t *info)
{
  اگر ((info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE) &&
      hb_glyph_info_ligated_اندرونی (info))
    واپس info->lig_props() & 0x0F;
  ورنہ
    واپس 1;
}

ساکن اندرونی uint8_t
_hb_allocate_lig_id (hb_بفر_t *بفر)
{
  uint8_t lig_id = بفر->اگلا_serial () & 0x07;
  اگر (unlikely (!lig_id))
    lig_id = _hb_allocate_lig_id (بفر); /* in case of overflow */
  واپس lig_id;
}

/* glyph_props: */

ساکن اندرونی خالی
hb_glyph_info_سیٹ_glyph_props (hb_glyph_info_t *info, غیرعلامتی صحیح props)
{
  info->glyph_props() = props;
}

ساکن اندرونی غیرعلامتی صحیح
_hb_glyph_info_get_glyph_props (ثابت hb_glyph_info_t *info)
{
  واپس info->glyph_props();
}

ساکن اندرونی منطقی
hb_glyph_info_is_بنیاد_glyph (ثابت hb_glyph_info_t *info)
{
  واپس info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_بنیاد_GLYPH;
}

ساکن اندرونی منطقی
_hb_glyph_info_is_ligature (ثابت hb_glyph_info_t *info)
{
  واپس info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
}

ساکن اندرونی منطقی
_hb_glyph_info_is_mark (ثابت hb_glyph_info_t *info)
{
  واپس info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_MARK;
}

ساکن اندرونی منطقی
_hb_glyph_info_substituted (ثابت hb_glyph_info_t *info)
{
  واپس info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
}

ساکن اندرونی منطقی
_hb_glyph_info_ligated (ثابت hb_glyph_info_t *info)
{
  واپس info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATED;
}

ساکن اندرونی منطقی
_hb_glyph_info_multiplied (ثابت hb_glyph_info_t *info)
{
  واپس info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
}

ساکن اندرونی غیرعلامتی صحیح
hb_glyph_info_get_lig_عدد_comps_اندر_ligation (ثابت hb_glyph_info_t *info)
{
  /* When a glyph is decomposed by a MultipleSubst and its pieces later become
   * components of a ligature, the pieces belong to the same ligature component
   * as the first piece, matching how MarkBasePos attaches marks only to the
   * first piece.  So the non-first pieces contribute no extra component.
   * https://github.com/harfbuzz/harfbuzz/issues/4969 */
  اگر (_hb_glyph_info_multiplied (info) &&
      _hb_glyph_info_get_lig_comp (info))
    واپس 0;
  واپس hb_glyph_info_get_lig_عدد_comps (info);
}

ساکن اندرونی منطقی
_hb_glyph_info_ligated_and_didnt_multiply (ثابت hb_glyph_info_t *info)
{
  واپس _hb_glyph_info_ligated (info) && !_hb_glyph_info_multiplied (info);
}

ساکن اندرونی خالی
_hb_glyph_info_clear_ligated_and_multiplied (hb_glyph_info_t *info)
{
  info->glyph_props() &= ~(HB_OT_LAYOUT_GLYPH_PROPS_LIGATED |
			   HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED);
}

ساکن اندرونی خالی
_hb_glyph_info_clear_substituted (hb_glyph_info_t *info)
{
  info->glyph_props() &= ~(HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED);
}

ساکن اندرونی منطقی
_hb_clear_substitution_flags (ثابت hb_ot_shape_plan_t *plan HB_غیر_مستعمل,
			      hb_font_t *font HB_غیر_مستعمل,
			      hb_بفر_t *بفر)
{
  hb_glyph_info_t *info = بفر->info;
  غیرعلامتی صحیح گنتی = بفر->len;
  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    _hb_glyph_info_clear_substituted (&info[i]);
  واپس جھوٹ;
}


/* Allocation / deallocation. */

ساکن اندرونی خالی
hb_بفر_allocate_unicode_vars (hb_بفر_t *بفر)
{
  HB_بفر_ALLOCATE_VAR (بفر, unicode_props);
}

ساکن اندرونی خالی
hb_بفر_deallocate_unicode_vars (hb_بفر_t *بفر)
{
  HB_بفر_DEALLOCATE_VAR (بفر, unicode_props);
}

ساکن اندرونی خالی
hb_بفر_assert_unicode_vars (hb_بفر_t *بفر)
{
  HB_بفر_ASSERT_VAR (بفر, unicode_props);
}

ساکن اندرونی خالی
hb_بفر_allocate_gsubgpos_vars (hb_بفر_t *بفر)
{
  HB_بفر_ALLOCATE_VAR (بفر, glyph_props);
  HB_بفر_ALLOCATE_VAR (بفر, lig_props);
}

ساکن اندرونی خالی
hb_بفر_deallocate_gsubgpos_vars (hb_بفر_t *بفر)
{
  HB_بفر_DEALLOCATE_VAR (بفر, lig_props);
  HB_بفر_DEALLOCATE_VAR (بفر, glyph_props);
}

ساکن اندرونی خالی
hb_بفر_assert_gsubgpos_vars (hb_بفر_t *بفر)
{
  HB_بفر_ASSERT_VAR (بفر, glyph_props);
  HB_بفر_ASSERT_VAR (بفر, lig_props);
}

/* Make sure no one directly touches our props... */
#تعریف_منسوخ unicode_props
#تعریف_منسوخ lig_props
#تعریف_منسوخ glyph_props

ساکن اندرونی خالی
hb_collect_glyph_alternates_جوڑو (hb_codepoint_t from,
				  hb_codepoint_t to,
				  hb_map_t *alternate_گنتی,
				  hb_map_t *alternate_glyphs)
{
  hb_codepoint_t zero = 0;
  hb_codepoint_t *i = &zero;
  alternate_گنتی->has (from, &i);
  alternate_glyphs->سیٹ (from | (*i << 24), to);
  alternate_گنتی->سیٹ (from, *i + 1);
}


#اگر_ختم /* HB_OT_LAYOUT_HH */
