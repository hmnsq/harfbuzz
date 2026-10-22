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

#నిర్వచితం_కాకపోతే HB_OT_LAYOUT_HH
#నిర్వచించు HB_OT_LAYOUT_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-font.hh"
#అన్తర్భావ "hb-buffer.hh"
#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-shape.hh"
#అన్తర్భావ "hb-set-digest.hh"


నిర్మాణం hb_ot_shape_plan_t;


/*
 * kern
 */

HB_అంతర్గత తార్కిక
hb_ot_layout_has_kerning (hb_face_t *face);

HB_అంతర్గత తార్కిక
hb_ot_layout_has_machine_kerning (hb_face_t *face);

HB_అంతర్గత తార్కిక
hb_ot_layout_has_cross_kerning (hb_face_t *face);

HB_అంతర్గత శూన్య
hb_ot_layout_kern (స్థిరరాశి hb_ot_shape_plan_t *plan,
		   hb_font_t *font,
		   hb_బఫరు_t  *బఫరు);


/* Private API corresponding to hb-ot-layout.h: */

HB_అంతర్గత తార్కిక
hb_ot_layout_పట్టిక_కనుగొను_feature (hb_face_t    *face,
				 hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
				 hb_ట్యాగ్_t      feature_ట్యాగ్,
				 గుర్తులేని పూర్ణాంకం *feature_సూచీ);


/*
 * GDEF
 */

గణన hb_ot_layout_glyph_props_flags_t
{
  /* The following three match LookupFlags::Ignore* numbers. */
  HB_OT_LAYOUT_GLYPH_PROPS_పునాది_GLYPH	= 0x02u,
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
HB_అంతర్గత శూన్య
hb_ot_layout_substitute_ప్రారంభం (hb_font_t    *font,
			       hb_బఫరు_t  *బఫరు);

HB_అంతర్గత శూన్య
hb_ot_layout_substitute_ప్రారంభం_with_digest (hb_font_t    *font,
					   hb_బఫరు_t  *బఫరు);

నామాకాశ OT {
  నిర్మాణం hb_ot_apply_context_t;
  నిర్మాణం hb_ot_layout_lookup_accelerator_t;
నామాకాశ Layout {
నామాకాశ GSUB_impl {
  నిర్మాణం SubstLookup;
}
}
}

HB_అంతర్గత శూన్య
hb_ot_layout_substitute_lookup (OT::hb_ot_apply_context_t *c,
				స్థిరరాశి OT::Layout::GSUB_impl::SubstLookup &lookup,
				స్థిరరాశి OT::hb_ot_layout_lookup_accelerator_t &accel);


/* Should be called before all the position_lookup's are done. */
HB_అంతర్గత శూన్య
hb_ot_layout_position_ప్రారంభం (hb_font_t    *font,
			     hb_బఫరు_t  *బఫరు);

/* Should be called after all the position_lookup's are done, to fini advances. */
HB_అంతర్గత శూన్య
hb_ot_layout_position_ముగింపు_advances (hb_font_t    *font,
				       hb_బఫరు_t  *బఫరు);

/* Should be called after hb_ot_layout_position_finish_advances, to fini offsets. */
HB_అంతర్గత శూన్య
hb_ot_layout_position_ముగింపు_offsets (hb_font_t    *font,
				      hb_బఫరు_t  *బఫరు);


/*
 * Buffer var routines.
 */

/* buffer var allocations, used during the entire shaping process */
#నిర్వచించు unicode_props()		var2.u16[0]

/* buffer var allocations, used during the GSUB/GPOS processing */
#నిర్వచించు glyph_props()		var1.u16[0] /* GDEF glyph properties */
#నిర్వచించు lig_props()		var1.u8[2] /* GSUB/GPOS ligature tracking */
#నిర్వచించు syllable()		var1.u8[3] /* GSUB/GPOS shaping boundaries */


/* Loop over syllables. Based on foreach_cluster(). */
#నిర్వచించు foreach_syllable(బఫరు, ప్రారంభం, చివర) \
  కొరకు (గుర్తులేని పూర్ణాంకం \
       లెక్క_2 = బఫరు->len, \
       ప్రారంభం = 0, చివర = లెక్క_2 ? hb_తదుపరి_syllable (బఫరు, 0) : 0; \
       ప్రారంభం < లెక్క_2; \
       ప్రారంభం = చివర, చివర = hb_తదుపరి_syllable (బఫరు, ప్రారంభం))

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
hb_తదుపరి_syllable (hb_బఫరు_t *బఫరు, గుర్తులేని పూర్ణాంకం ప్రారంభం)
{
  hb_glyph_info_t *info = బఫరు->info;
  గుర్తులేని పూర్ణాంకం లెక్క = ప్రారంభం + hb_min (బఫరు->len - ప్రారంభం,
				       (గుర్తులేని) HB_MAX_SYLLABLE_నిడివి);

  గుర్తులేని పూర్ణాంకం syllable = info[ప్రారంభం].syllable();
  అయితే (++ప్రారంభం < లెక్క && syllable == info[ప్రారంభం].syllable())
    ;

  తిరిగి ప్రారంభం;
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

గణన hb_unicode_props_flags_t {
  UPROPS_ముసుగు_GEN_CAT	= 0x001Fu,
  UPROPS_ముసుగు_IGNORABLE	= 0x0020u,
  UPROPS_ముసుగు_HIDDEN	= 0x0040u, /* MONGOLIAN FREE VARIATION SELECTOR 1..4, or TAG characters, or CGJ sometimes */
  UPROPS_ముసుగు_CONTINUATION=0x0080u,

  /* If GEN_CAT=FORMAT, top byte masks: */
  UPROPS_ముసుగు_Cf_ZWJ	= 0x0100u,
  UPROPS_ముసుగు_Cf_ZWNJ	= 0x0200u,
  UPROPS_ముసుగు_Cf_VS	= 0x0400u,
  UPROPS_ముసుగు_Cf_AAT_DELETED	= 0x0800u
};
HB_MARK_AS_FLAG_T (hb_unicode_props_flags_t);

స్థిర అంతఃవరుస శూన్య
hb_glyph_info_సమితి_unicode_props (hb_glyph_info_t *info, hb_బఫరు_t *బఫరు)
{
  hb_unicode_funcs_t *unicode = బఫరు->unicode;
  గుర్తులేని పూర్ణాంకం u = info->codepoint;
  గుర్తులేని పూర్ణాంకం gen_cat = (గుర్తులేని పూర్ణాంకం) unicode->general_category (u);
  గుర్తులేని పూర్ణాంకం props = gen_cat;

  ఒకవేళ (u >= 0x80u)
  {
    ఒకవేళ (unlikely (unicode->is_default_ignorable (u)))
    {
      బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES;
      props |=  UPROPS_ముసుగు_IGNORABLE;
      ఒకవేళ (u == 0x200Cu) props |= UPROPS_ముసుగు_Cf_ZWNJ;
      లేకపోతే ఒకవేళ (u == 0x200Du) props |= UPROPS_ముసుగు_Cf_ZWJ;
      /* Mongolian Free Variation Selectors need to be remembered
       * because although we need to hide them like default-ignorables,
       * they need to non-ignorable during shaping.  This is similar to
       * what we do for joiners in Indic-like shapers, but since the
       * FVSes are GC=Mn, we have use a separate bit to remember them.
       * Fixes:
       * https://github.com/harfbuzz/harfbuzz/issues/234 */
      లేకపోతే ఒకవేళ (unlikely (hb_లోపల_ranges<hb_codepoint_t> (u, 0x180Bu, 0x180Du, 0x180Fu, 0x180Fu))) props |= UPROPS_ముసుగు_HIDDEN;
      /* TAG characters need similar treatment. Fixes:
       * https://github.com/harfbuzz/harfbuzz/issues/463 */
      లేకపోతే ఒకవేళ (unlikely (hb_లోపల_range<hb_codepoint_t> (u, 0xE0020u, 0xE007Fu))) props |= UPROPS_ముసుగు_HIDDEN;
      /* COMBINING GRAPHEME JOINER should not be skipped during GSUB either.
       * https://github.com/harfbuzz/harfbuzz/issues/554 */
      లేకపోతే ఒకవేళ (unlikely (u == 0x034Fu))
      {
	బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_HAS_CGJ;
	props |= UPROPS_ముసుగు_HIDDEN;
      }
    }

    ఒకవేళ (unlikely (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (gen_cat)))
    {
      బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_HAS_CONTINUATIONS;
      props |= UPROPS_ముసుగు_CONTINUATION;
      props |= unicode->modified_combining_class (u)<<8;
    }
  }

  info->unicode_props() = props;
}

స్థిర అంతఃవరుస శూన్య
hb_glyph_info_సమితి_general_category (hb_glyph_info_t *info,
				     hb_unicode_general_category_t gen_cat)
{
  /* Clears top-byte. */
  info->unicode_props() = (గుర్తులేని పూర్ణాంకం) gen_cat | (info->unicode_props() & (0xFF & ~UPROPS_ముసుగు_GEN_CAT));
}

స్థిర అంతఃవరుస hb_unicode_general_category_t
_hb_glyph_info_get_general_category (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి (hb_unicode_general_category_t) (info->unicode_props() & UPROPS_ముసుగు_GEN_CAT);
}

స్థిర అంతఃవరుస తార్కిక
_hb_glyph_info_is_unicode_mark (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి HB_UNICODE_GENERAL_CATEGORY_IS_MARK (info->unicode_props() & UPROPS_ముసుగు_GEN_CAT);
}
స్థిర అంతఃవరుస శూన్య
hb_glyph_info_సమితి_modified_combining_class (hb_glyph_info_t *info,
					     గుర్తులేని పూర్ణాంకం modified_class)
{
  ఒకవేళ (unlikely (!_hb_glyph_info_is_unicode_mark (info)))
    తిరిగి;
  info->unicode_props() = (modified_class<<8) | (info->unicode_props() & 0xFF);
}
స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
_hb_glyph_info_get_modified_combining_class (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి _hb_glyph_info_is_unicode_mark (info) ? info->unicode_props()>>8 : 0;
}
#నిర్వచించు info_cc(info) (_hb_glyph_info_get_modified_combining_class (&(info)))

స్థిర అంతఃవరుస తార్కిక
_hb_glyph_info_is_unicode_space (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
}
స్థిర అంతఃవరుస శూన్య
hb_glyph_info_సమితి_unicode_space_fallback_type (hb_glyph_info_t *info, hb_unicode_funcs_t::space_t s)
{
  ఒకవేళ (unlikely (!_hb_glyph_info_is_unicode_space (info)))
    తిరిగి;
  info->unicode_props() = (((గుర్తులేని పూర్ణాంకం) s)<<8) | (info->unicode_props() & 0xFF);
}
స్థిర అంతఃవరుస hb_unicode_funcs_t::space_t
_hb_glyph_info_get_unicode_space_fallback_type (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి _hb_glyph_info_is_unicode_space (info) ?
	 (hb_unicode_funcs_t::space_t) (info->unicode_props()>>8) :
	 hb_unicode_funcs_t::NOT_SPACE;
}
స్థిర అంతఃవరుస తార్కిక
_hb_glyph_info_is_variation_selector (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_FORMAT &&
	 (info->unicode_props() & UPROPS_ముసుగు_Cf_VS);
}
స్థిర అంతఃవరుస శూన్య
hb_glyph_info_సమితి_variation_selector (hb_glyph_info_t *info, తార్కిక customize)
{
  ఒకవేళ (customize)
  {
    hb_glyph_info_సమితి_general_category (info, HB_UNICODE_GENERAL_CATEGORY_FORMAT);
    info->unicode_props() |= UPROPS_ముసుగు_Cf_VS;
  }
  లేకపోతే
  {
    // Reset to their original condition
    hb_glyph_info_సమితి_general_category (info, HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK);
  }
}

స్థిర అంతఃవరుస తార్కిక _hb_glyph_info_substituted (స్థిరరాశి hb_glyph_info_t *info);

స్థిర అంతఃవరుస తార్కిక
_hb_glyph_info_is_default_ignorable (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి (info->unicode_props() & UPROPS_ముసుగు_IGNORABLE) &&
	 !_hb_glyph_info_substituted (info);
}
స్థిర అంతఃవరుస శూన్య
hb_glyph_info_సమితి_default_ignorable (hb_glyph_info_t *info)
{
  info->unicode_props() |= UPROPS_ముసుగు_IGNORABLE;
}
స్థిర అంతఃవరుస శూన్య
_hb_glyph_info_clear_default_ignorable (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_ముసుగు_IGNORABLE;
}
స్థిర అంతఃవరుస తార్కిక
_hb_glyph_info_is_hidden (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి info->unicode_props() & UPROPS_ముసుగు_HIDDEN;
}
స్థిర అంతఃవరుస శూన్య
_hb_glyph_info_unhide (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_ముసుగు_HIDDEN;
}

స్థిర అంతఃవరుస శూన్య
hb_glyph_info_సమితి_continuation (hb_glyph_info_t *info, hb_బఫరు_t *బఫరు)
{
  బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_HAS_CONTINUATIONS;
  info->unicode_props() |= UPROPS_ముసుగు_CONTINUATION;
}
స్థిర అంతఃవరుస శూన్య
_hb_glyph_info_clear_continuation (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_ముసుగు_CONTINUATION;
}
స్థిర అంతఃవరుస తార్కిక
_hb_glyph_info_is_continuation (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి info->unicode_props() & UPROPS_ముసుగు_CONTINUATION;
}

స్థిర అంతఃవరుస తార్కిక
hb_grapheme_సమూహం_func (స్థిరరాశి hb_glyph_info_t& a HB_వాడని,
			 స్థిరరాశి hb_glyph_info_t& b)
{ తిరిగి _hb_glyph_info_is_continuation (&b); }

#నిర్వచించు foreach_grapheme(బఫరు, ప్రారంభం, చివర) \
	foreach_సమూహం (బఫరు, ప్రారంభం, చివర, hb_grapheme_సమూహం_func)

స్థిర అంతఃవరుస శూన్య
hb_ot_layout_తిరగేయి_graphemes (hb_బఫరు_t *బఫరు)
{
  // MONOTONE_GRAPHEMES was already applied and is taken care of by _hb_grapheme_group_func.
  // So we just check for MONOTONE_CHARACTERS here.
  బఫరు->తిరగేయి_groups (hb_grapheme_సమూహం_func,
			  బఫరు->cluster_level == HB_బఫరు_CLUSTER_LEVEL_MONOTONE_CHARACTERS);
}

స్థిర అంతఃవరుస తార్కిక
_hb_glyph_info_is_unicode_format (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_FORMAT;
}
స్థిర అంతఃవరుస తార్కిక
_hb_glyph_info_is_zwnj (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_ముసుగు_Cf_ZWNJ);
}
స్థిర అంతఃవరుస తార్కిక
_hb_glyph_info_is_zwj (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_ముసుగు_Cf_ZWJ);
}
స్థిర అంతఃవరుస తార్కిక
_hb_glyph_info_is_aat_deleted (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_ముసుగు_Cf_AAT_DELETED);
}
స్థిర అంతఃవరుస శూన్య
hb_glyph_info_సమితి_aat_deleted (hb_glyph_info_t *info)
{
  hb_glyph_info_సమితి_general_category (info, HB_UNICODE_GENERAL_CATEGORY_FORMAT);
  info->unicode_props() |= UPROPS_ముసుగు_Cf_AAT_DELETED;
  info->unicode_props() |= UPROPS_ముసుగు_HIDDEN;
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

స్థిర అంతఃవరుస శూన్య
_hb_glyph_info_clear_lig_props (hb_glyph_info_t *info)
{
  info->lig_props() = 0;
}

#నిర్వచించు IS_LIG_పునాది 0x10

స్థిర అంతఃవరుస శూన్య
hb_glyph_info_సమితి_lig_props_for_ligature (hb_glyph_info_t *info,
					   గుర్తులేని పూర్ణాంకం lig_id,
					   గుర్తులేని పూర్ణాంకం lig_అంకె_comps)
{
  info->lig_props() = (lig_id << 5) | IS_LIG_పునాది | (lig_అంకె_comps & 0x0F);
}

స్థిర అంతఃవరుస శూన్య
hb_glyph_info_సమితి_lig_props_for_mark (hb_glyph_info_t *info,
				       గుర్తులేని పూర్ణాంకం lig_id,
				       గుర్తులేని పూర్ణాంకం lig_comp)
{
  info->lig_props() = (lig_id << 5) | (lig_comp & 0x0F);
}

స్థిర అంతఃవరుస శూన్య
hb_glyph_info_సమితి_lig_props_for_component (hb_glyph_info_t *info, గుర్తులేని పూర్ణాంకం comp)
{
  hb_glyph_info_సమితి_lig_props_for_mark (info, 0, comp);
}

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
_hb_glyph_info_get_lig_id (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి info->lig_props() >> 5;
}

స్థిర అంతఃవరుస తార్కిక
hb_glyph_info_ligated_అంతర్గత (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి info->lig_props() & IS_LIG_పునాది;
}

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
_hb_glyph_info_get_lig_comp (స్థిరరాశి hb_glyph_info_t *info)
{
  ఒకవేళ (hb_glyph_info_ligated_అంతర్గత (info))
    తిరిగి 0;
  లేకపోతే
    తిరిగి info->lig_props() & 0x0F;
}

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
hb_glyph_info_get_lig_అంకె_comps (స్థిరరాశి hb_glyph_info_t *info)
{
  ఒకవేళ ((info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE) &&
      hb_glyph_info_ligated_అంతర్గత (info))
    తిరిగి info->lig_props() & 0x0F;
  లేకపోతే
    తిరిగి 1;
}

స్థిర అంతఃవరుస uint8_t
_hb_allocate_lig_id (hb_బఫరు_t *బఫరు)
{
  uint8_t lig_id = బఫరు->తదుపరి_serial () & 0x07;
  ఒకవేళ (unlikely (!lig_id))
    lig_id = _hb_allocate_lig_id (బఫరు); /* in case of overflow */
  తిరిగి lig_id;
}

/* glyph_props: */

స్థిర అంతఃవరుస శూన్య
hb_glyph_info_సమితి_glyph_props (hb_glyph_info_t *info, గుర్తులేని పూర్ణాంకం props)
{
  info->glyph_props() = props;
}

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
_hb_glyph_info_get_glyph_props (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి info->glyph_props();
}

స్థిర అంతఃవరుస తార్కిక
hb_glyph_info_is_పునాది_glyph (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_పునాది_GLYPH;
}

స్థిర అంతఃవరుస తార్కిక
_hb_glyph_info_is_ligature (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
}

స్థిర అంతఃవరుస తార్కిక
_hb_glyph_info_is_mark (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_MARK;
}

స్థిర అంతఃవరుస తార్కిక
_hb_glyph_info_substituted (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
}

స్థిర అంతఃవరుస తార్కిక
_hb_glyph_info_ligated (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATED;
}

స్థిర అంతఃవరుస తార్కిక
_hb_glyph_info_multiplied (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
}

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
hb_glyph_info_get_lig_అంకె_comps_లోపల_ligation (స్థిరరాశి hb_glyph_info_t *info)
{
  /* When a glyph is decomposed by a MultipleSubst and its pieces later become
   * components of a ligature, the pieces belong to the same ligature component
   * as the first piece, matching how MarkBasePos attaches marks only to the
   * first piece.  So the non-first pieces contribute no extra component.
   * https://github.com/harfbuzz/harfbuzz/issues/4969 */
  ఒకవేళ (_hb_glyph_info_multiplied (info) &&
      _hb_glyph_info_get_lig_comp (info))
    తిరిగి 0;
  తిరిగి hb_glyph_info_get_lig_అంకె_comps (info);
}

స్థిర అంతఃవరుస తార్కిక
_hb_glyph_info_ligated_and_didnt_multiply (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి _hb_glyph_info_ligated (info) && !_hb_glyph_info_multiplied (info);
}

స్థిర అంతఃవరుస శూన్య
_hb_glyph_info_clear_ligated_and_multiplied (hb_glyph_info_t *info)
{
  info->glyph_props() &= ~(HB_OT_LAYOUT_GLYPH_PROPS_LIGATED |
			   HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED);
}

స్థిర అంతఃవరుస శూన్య
_hb_glyph_info_clear_substituted (hb_glyph_info_t *info)
{
  info->glyph_props() &= ~(HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED);
}

స్థిర అంతఃవరుస తార్కిక
_hb_clear_substitution_flags (స్థిరరాశి hb_ot_shape_plan_t *plan HB_వాడని,
			      hb_font_t *font HB_వాడని,
			      hb_బఫరు_t *బఫరు)
{
  hb_glyph_info_t *info = బఫరు->info;
  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    _hb_glyph_info_clear_substituted (&info[i]);
  తిరిగి అబద్ధం;
}


/* Allocation / deallocation. */

స్థిర అంతఃవరుస శూన్య
hb_బఫరు_allocate_unicode_vars (hb_బఫరు_t *బఫరు)
{
  HB_బఫరు_ALLOCATE_VAR (బఫరు, unicode_props);
}

స్థిర అంతఃవరుస శూన్య
hb_బఫరు_deallocate_unicode_vars (hb_బఫరు_t *బఫరు)
{
  HB_బఫరు_DEALLOCATE_VAR (బఫరు, unicode_props);
}

స్థిర అంతఃవరుస శూన్య
hb_బఫరు_assert_unicode_vars (hb_బఫరు_t *బఫరు)
{
  HB_బఫరు_ASSERT_VAR (బఫరు, unicode_props);
}

స్థిర అంతఃవరుస శూన్య
hb_బఫరు_allocate_gsubgpos_vars (hb_బఫరు_t *బఫరు)
{
  HB_బఫరు_ALLOCATE_VAR (బఫరు, glyph_props);
  HB_బఫరు_ALLOCATE_VAR (బఫరు, lig_props);
}

స్థిర అంతఃవరుస శూన్య
hb_బఫరు_deallocate_gsubgpos_vars (hb_బఫరు_t *బఫరు)
{
  HB_బఫరు_DEALLOCATE_VAR (బఫరు, lig_props);
  HB_బఫరు_DEALLOCATE_VAR (బఫరు, glyph_props);
}

స్థిర అంతఃవరుస శూన్య
hb_బఫరు_assert_gsubgpos_vars (hb_బఫరు_t *బఫరు)
{
  HB_బఫరు_ASSERT_VAR (బఫరు, glyph_props);
  HB_బఫరు_ASSERT_VAR (బఫరు, lig_props);
}

/* Make sure no one directly touches our props... */
#నిర్వచనం_రద్దు unicode_props
#నిర్వచనం_రద్దు lig_props
#నిర్వచనం_రద్దు glyph_props

స్థిర అంతఃవరుస శూన్య
hb_collect_glyph_alternates_కూడు (hb_codepoint_t from,
				  hb_codepoint_t to,
				  hb_map_t *alternate_లెక్క,
				  hb_map_t *alternate_glyphs)
{
  hb_codepoint_t zero = 0;
  hb_codepoint_t *i = &zero;
  alternate_లెక్క->has (from, &i);
  alternate_glyphs->సమితి (from | (*i << 24), to);
  alternate_లెక్క->సమితి (from, *i + 1);
}


#అయితే_ముగింపు /* HB_OT_LAYOUT_HH */
