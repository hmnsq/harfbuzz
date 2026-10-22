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

#ifndef HB_OT_LAYOUT_HH
#define HB_OT_LAYOUT_HH

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-font.hh"
#അന്തര്ഭാവ "hb-buffer.hh"
#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-shape.hh"
#അന്തര്ഭാവ "hb-set-digest.hh"


ഘടന hb_ot_shape_plan_t;


/*
 * kern
 */

HB_INTERNAL തർക്കം
hb_ot_layout_has_kerning (hb_face_t *face);

HB_INTERNAL തർക്കം
hb_ot_layout_has_machine_kerning (hb_face_t *face);

HB_INTERNAL തർക്കം
hb_ot_layout_has_cross_kerning (hb_face_t *face);

HB_INTERNAL ശൂന്യം
hb_ot_layout_kern (സ്ഥിരം hb_ot_shape_plan_t *plan,
		   hb_font_t *font,
		   hb_buffer_t  *buffer);


/* Private API corresponding to hb-ot-layout.h: */

HB_INTERNAL തർക്കം
hb_ot_layout_table_find_feature (hb_face_t    *face,
				 hb_tag_t      table_tag,
				 hb_tag_t      feature_tag,
				 unsigned പൂർണ്ണസംഖ്യ *feature_index);


/*
 * GDEF
 */

എണ്ണൽ hb_ot_layout_glyph_props_flags_t
{
  /* The following three match LookupFlags::Ignore* numbers. */
  HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH	= 0x02u,
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
HB_INTERNAL ശൂന്യം
hb_ot_layout_substitute_start (hb_font_t    *font,
			       hb_buffer_t  *buffer);

HB_INTERNAL ശൂന്യം
hb_ot_layout_substitute_start_with_digest (hb_font_t    *font,
					   hb_buffer_t  *buffer);

നാമാകാശ OT {
  ഘടന hb_ot_apply_context_t;
  ഘടന hb_ot_layout_lookup_accelerator_t;
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {
  ഘടന SubstLookup;
}
}
}

HB_INTERNAL ശൂന്യം
hb_ot_layout_substitute_lookup (OT::hb_ot_apply_context_t *c,
				സ്ഥിരം OT::Layout::GSUB_impl::SubstLookup &lookup,
				സ്ഥിരം OT::hb_ot_layout_lookup_accelerator_t &accel);


/* Should be called before all the position_lookup's are done. */
HB_INTERNAL ശൂന്യം
hb_ot_layout_position_start (hb_font_t    *font,
			     hb_buffer_t  *buffer);

/* Should be called after all the position_lookup's are done, to fini advances. */
HB_INTERNAL ശൂന്യം
hb_ot_layout_position_finish_advances (hb_font_t    *font,
				       hb_buffer_t  *buffer);

/* Should be called after hb_ot_layout_position_finish_advances, to fini offsets. */
HB_INTERNAL ശൂന്യം
hb_ot_layout_position_finish_offsets (hb_font_t    *font,
				      hb_buffer_t  *buffer);


/*
 * Buffer var routines.
 */

/* buffer var allocations, used during the entire shaping process */
#define unicode_props()		var2.u16[0]

/* buffer var allocations, used during the GSUB/GPOS processing */
#define glyph_props()		var1.u16[0] /* GDEF glyph properties */
#define lig_props()		var1.u8[2] /* GSUB/GPOS ligature tracking */
#define syllable()		var1.u8[3] /* GSUB/GPOS shaping boundaries */


/* Loop over syllables. Based on foreach_cluster(). */
#define foreach_syllable(buffer, start, end) \
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ \
       എണ്ണം_2 = buffer->len, \
       start = 0, end = എണ്ണം_2 ? _hb_next_syllable (buffer, 0) : 0; \
       start < എണ്ണം_2; \
       start = end, end = _hb_next_syllable (buffer, start))

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
_hb_next_syllable (hb_buffer_t *buffer, unsigned പൂർണ്ണസംഖ്യ start)
{
  hb_glyph_info_t *info = buffer->info;
  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = start + hb_min (buffer->len - start,
				       (unsigned) HB_MAX_SYLLABLE_LENGTH);

  unsigned പൂർണ്ണസംഖ്യ syllable = info[start].syllable();
  ആകുമ്പോൾ (++start < എണ്ണം && syllable == info[start].syllable())
    ;

  തിരികെ start;
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

എണ്ണൽ hb_unicode_props_flags_t {
  UPROPS_MASK_GEN_CAT	= 0x001Fu,
  UPROPS_MASK_IGNORABLE	= 0x0020u,
  UPROPS_MASK_HIDDEN	= 0x0040u, /* MONGOLIAN FREE VARIATION SELECTOR 1..4, or TAG characters, or CGJ sometimes */
  UPROPS_MASK_CONTINUATION=0x0080u,

  /* If GEN_CAT=FORMAT, top byte masks: */
  UPROPS_MASK_Cf_ZWJ	= 0x0100u,
  UPROPS_MASK_Cf_ZWNJ	= 0x0200u,
  UPROPS_MASK_Cf_VS	= 0x0400u,
  UPROPS_MASK_Cf_AAT_DELETED	= 0x0800u
};
HB_MARK_AS_FLAG_T (hb_unicode_props_flags_t);

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_set_unicode_props (hb_glyph_info_t *info, hb_buffer_t *buffer)
{
  hb_unicode_funcs_t *unicode = buffer->unicode;
  unsigned പൂർണ്ണസംഖ്യ u = info->codepoint;
  unsigned പൂർണ്ണസംഖ്യ gen_cat = (unsigned പൂർണ്ണസംഖ്യ) unicode->general_category (u);
  unsigned പൂർണ്ണസംഖ്യ props = gen_cat;

  എങ്കിൽ (u >= 0x80u)
  {
    എങ്കിൽ (unlikely (unicode->is_default_ignorable (u)))
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES;
      props |=  UPROPS_MASK_IGNORABLE;
      എങ്കിൽ (u == 0x200Cu) props |= UPROPS_MASK_Cf_ZWNJ;
      അല്ലെങ്കിൽ എങ്കിൽ (u == 0x200Du) props |= UPROPS_MASK_Cf_ZWJ;
      /* Mongolian Free Variation Selectors need to be remembered
       * because although we need to hide them like default-ignorables,
       * they need to non-ignorable during shaping.  This is similar to
       * what we do for joiners in Indic-like shapers, but since the
       * FVSes are GC=Mn, we have use a separate bit to remember them.
       * Fixes:
       * https://github.com/harfbuzz/harfbuzz/issues/234 */
      അല്ലെങ്കിൽ എങ്കിൽ (unlikely (hb_in_ranges<hb_codepoint_t> (u, 0x180Bu, 0x180Du, 0x180Fu, 0x180Fu))) props |= UPROPS_MASK_HIDDEN;
      /* TAG characters need similar treatment. Fixes:
       * https://github.com/harfbuzz/harfbuzz/issues/463 */
      അല്ലെങ്കിൽ എങ്കിൽ (unlikely (hb_in_range<hb_codepoint_t> (u, 0xE0020u, 0xE007Fu))) props |= UPROPS_MASK_HIDDEN;
      /* COMBINING GRAPHEME JOINER should not be skipped during GSUB either.
       * https://github.com/harfbuzz/harfbuzz/issues/554 */
      അല്ലെങ്കിൽ എങ്കിൽ (unlikely (u == 0x034Fu))
      {
	buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_CGJ;
	props |= UPROPS_MASK_HIDDEN;
      }
    }

    എങ്കിൽ (unlikely (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (gen_cat)))
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_CONTINUATIONS;
      props |= UPROPS_MASK_CONTINUATION;
      props |= unicode->modified_combining_class (u)<<8;
    }
  }

  info->unicode_props() = props;
}

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_set_general_category (hb_glyph_info_t *info,
				     hb_unicode_general_category_t gen_cat)
{
  /* Clears top-byte. */
  info->unicode_props() = (unsigned പൂർണ്ണസംഖ്യ) gen_cat | (info->unicode_props() & (0xFF & ~UPROPS_MASK_GEN_CAT));
}

സ്ഥിതം വരിയിൽ hb_unicode_general_category_t
_hb_glyph_info_get_general_category (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ (hb_unicode_general_category_t) (info->unicode_props() & UPROPS_MASK_GEN_CAT);
}

സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_is_unicode_mark (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ HB_UNICODE_GENERAL_CATEGORY_IS_MARK (info->unicode_props() & UPROPS_MASK_GEN_CAT);
}
സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_set_modified_combining_class (hb_glyph_info_t *info,
					     unsigned പൂർണ്ണസംഖ്യ modified_class)
{
  എങ്കിൽ (unlikely (!_hb_glyph_info_is_unicode_mark (info)))
    തിരികെ;
  info->unicode_props() = (modified_class<<8) | (info->unicode_props() & 0xFF);
}
സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
_hb_glyph_info_get_modified_combining_class (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ _hb_glyph_info_is_unicode_mark (info) ? info->unicode_props()>>8 : 0;
}
#define info_cc(info) (_hb_glyph_info_get_modified_combining_class (&(info)))

സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_is_unicode_space (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
}
സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_set_unicode_space_fallback_type (hb_glyph_info_t *info, hb_unicode_funcs_t::space_t s)
{
  എങ്കിൽ (unlikely (!_hb_glyph_info_is_unicode_space (info)))
    തിരികെ;
  info->unicode_props() = (((unsigned പൂർണ്ണസംഖ്യ) s)<<8) | (info->unicode_props() & 0xFF);
}
സ്ഥിതം വരിയിൽ hb_unicode_funcs_t::space_t
_hb_glyph_info_get_unicode_space_fallback_type (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ _hb_glyph_info_is_unicode_space (info) ?
	 (hb_unicode_funcs_t::space_t) (info->unicode_props()>>8) :
	 hb_unicode_funcs_t::NOT_SPACE;
}
സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_is_variation_selector (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_FORMAT &&
	 (info->unicode_props() & UPROPS_MASK_Cf_VS);
}
സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_set_variation_selector (hb_glyph_info_t *info, തർക്കം customize)
{
  എങ്കിൽ (customize)
  {
    _hb_glyph_info_set_general_category (info, HB_UNICODE_GENERAL_CATEGORY_FORMAT);
    info->unicode_props() |= UPROPS_MASK_Cf_VS;
  }
  അല്ലെങ്കിൽ
  {
    // Reset to their original condition
    _hb_glyph_info_set_general_category (info, HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK);
  }
}

സ്ഥിതം വരിയിൽ തർക്കം _hb_glyph_info_substituted (സ്ഥിരം hb_glyph_info_t *info);

സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_is_default_ignorable (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ (info->unicode_props() & UPROPS_MASK_IGNORABLE) &&
	 !_hb_glyph_info_substituted (info);
}
സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_set_default_ignorable (hb_glyph_info_t *info)
{
  info->unicode_props() |= UPROPS_MASK_IGNORABLE;
}
സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_clear_default_ignorable (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_MASK_IGNORABLE;
}
സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_is_hidden (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ info->unicode_props() & UPROPS_MASK_HIDDEN;
}
സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_unhide (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_MASK_HIDDEN;
}

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_set_continuation (hb_glyph_info_t *info, hb_buffer_t *buffer)
{
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_CONTINUATIONS;
  info->unicode_props() |= UPROPS_MASK_CONTINUATION;
}
സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_clear_continuation (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_MASK_CONTINUATION;
}
സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_is_continuation (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ info->unicode_props() & UPROPS_MASK_CONTINUATION;
}

സ്ഥിതം വരിയിൽ തർക്കം
_hb_grapheme_group_func (സ്ഥിരം hb_glyph_info_t& a HB_UNUSED,
			 സ്ഥിരം hb_glyph_info_t& b)
{ തിരികെ _hb_glyph_info_is_continuation (&b); }

#define foreach_grapheme(buffer, start, end) \
	foreach_group (buffer, start, end, _hb_grapheme_group_func)

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_ot_layout_reverse_graphemes (hb_buffer_t *buffer)
{
  // MONOTONE_GRAPHEMES was already applied and is taken care of by _hb_grapheme_group_func.
  // So we just check for MONOTONE_CHARACTERS here.
  buffer->reverse_groups (_hb_grapheme_group_func,
			  buffer->cluster_level == HB_BUFFER_CLUSTER_LEVEL_MONOTONE_CHARACTERS);
}

സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_is_unicode_format (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_FORMAT;
}
സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_is_zwnj (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_MASK_Cf_ZWNJ);
}
സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_is_zwj (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_MASK_Cf_ZWJ);
}
സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_is_aat_deleted (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_MASK_Cf_AAT_DELETED);
}
സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_set_aat_deleted (hb_glyph_info_t *info)
{
  _hb_glyph_info_set_general_category (info, HB_UNICODE_GENERAL_CATEGORY_FORMAT);
  info->unicode_props() |= UPROPS_MASK_Cf_AAT_DELETED;
  info->unicode_props() |= UPROPS_MASK_HIDDEN;
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

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_clear_lig_props (hb_glyph_info_t *info)
{
  info->lig_props() = 0;
}

#define IS_LIG_BASE 0x10

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_set_lig_props_for_ligature (hb_glyph_info_t *info,
					   unsigned പൂർണ്ണസംഖ്യ lig_id,
					   unsigned പൂർണ്ണസംഖ്യ lig_അക്കം_comps)
{
  info->lig_props() = (lig_id << 5) | IS_LIG_BASE | (lig_അക്കം_comps & 0x0F);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_set_lig_props_for_mark (hb_glyph_info_t *info,
				       unsigned പൂർണ്ണസംഖ്യ lig_id,
				       unsigned പൂർണ്ണസംഖ്യ lig_comp)
{
  info->lig_props() = (lig_id << 5) | (lig_comp & 0x0F);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_set_lig_props_for_component (hb_glyph_info_t *info, unsigned പൂർണ്ണസംഖ്യ comp)
{
  _hb_glyph_info_set_lig_props_for_mark (info, 0, comp);
}

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
_hb_glyph_info_get_lig_id (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ info->lig_props() >> 5;
}

സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_ligated_internal (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ info->lig_props() & IS_LIG_BASE;
}

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
_hb_glyph_info_get_lig_comp (സ്ഥിരം hb_glyph_info_t *info)
{
  എങ്കിൽ (_hb_glyph_info_ligated_internal (info))
    തിരികെ 0;
  അല്ലെങ്കിൽ
    തിരികെ info->lig_props() & 0x0F;
}

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
hb_glyph_info_get_lig_അക്കം_comps (സ്ഥിരം hb_glyph_info_t *info)
{
  എങ്കിൽ ((info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE) &&
      _hb_glyph_info_ligated_internal (info))
    തിരികെ info->lig_props() & 0x0F;
  അല്ലെങ്കിൽ
    തിരികെ 1;
}

സ്ഥിതം വരിയിൽ uint8_t
_hb_allocate_lig_id (hb_buffer_t *buffer)
{
  uint8_t lig_id = buffer->next_serial () & 0x07;
  എങ്കിൽ (unlikely (!lig_id))
    lig_id = _hb_allocate_lig_id (buffer); /* in case of overflow */
  തിരികെ lig_id;
}

/* glyph_props: */

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_set_glyph_props (hb_glyph_info_t *info, unsigned പൂർണ്ണസംഖ്യ props)
{
  info->glyph_props() = props;
}

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
_hb_glyph_info_get_glyph_props (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ info->glyph_props();
}

സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_is_base_glyph (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH;
}

സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_is_ligature (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
}

സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_is_mark (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_MARK;
}

സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_substituted (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
}

സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_ligated (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATED;
}

സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_multiplied (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
}

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
hb_glyph_info_get_lig_അക്കം_comps_in_ligation (സ്ഥിരം hb_glyph_info_t *info)
{
  /* When a glyph is decomposed by a MultipleSubst and its pieces later become
   * components of a ligature, the pieces belong to the same ligature component
   * as the first piece, matching how MarkBasePos attaches marks only to the
   * first piece.  So the non-first pieces contribute no extra component.
   * https://github.com/harfbuzz/harfbuzz/issues/4969 */
  എങ്കിൽ (_hb_glyph_info_multiplied (info) &&
      _hb_glyph_info_get_lig_comp (info))
    തിരികെ 0;
  തിരികെ hb_glyph_info_get_lig_അക്കം_comps (info);
}

സ്ഥിതം വരിയിൽ തർക്കം
_hb_glyph_info_ligated_and_didnt_multiply (സ്ഥിരം hb_glyph_info_t *info)
{
  തിരികെ _hb_glyph_info_ligated (info) && !_hb_glyph_info_multiplied (info);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_clear_ligated_and_multiplied (hb_glyph_info_t *info)
{
  info->glyph_props() &= ~(HB_OT_LAYOUT_GLYPH_PROPS_LIGATED |
			   HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_glyph_info_clear_substituted (hb_glyph_info_t *info)
{
  info->glyph_props() &= ~(HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED);
}

സ്ഥിതം വരിയിൽ തർക്കം
_hb_clear_substitution_flags (സ്ഥിരം hb_ot_shape_plan_t *plan HB_UNUSED,
			      hb_font_t *font HB_UNUSED,
			      hb_buffer_t *buffer)
{
  hb_glyph_info_t *info = buffer->info;
  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    _hb_glyph_info_clear_substituted (&info[i]);
  തിരികെ അസത്യം;
}


/* Allocation / deallocation. */

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_buffer_allocate_unicode_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, unicode_props);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_buffer_deallocate_unicode_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_DEALLOCATE_VAR (buffer, unicode_props);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_buffer_assert_unicode_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_ASSERT_VAR (buffer, unicode_props);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_buffer_allocate_gsubgpos_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, glyph_props);
  HB_BUFFER_ALLOCATE_VAR (buffer, lig_props);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_buffer_deallocate_gsubgpos_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_DEALLOCATE_VAR (buffer, lig_props);
  HB_BUFFER_DEALLOCATE_VAR (buffer, glyph_props);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_buffer_assert_gsubgpos_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_ASSERT_VAR (buffer, glyph_props);
  HB_BUFFER_ASSERT_VAR (buffer, lig_props);
}

/* Make sure no one directly touches our props... */
#undef unicode_props
#undef lig_props
#undef glyph_props

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_collect_glyph_alternates_add (hb_codepoint_t from,
				  hb_codepoint_t to,
				  hb_map_t *alternate_എണ്ണം,
				  hb_map_t *alternate_glyphs)
{
  hb_codepoint_t zero = 0;
  hb_codepoint_t *i = &zero;
  alternate_എണ്ണം->has (from, &i);
  alternate_glyphs->set (from | (*i << 24), to);
  alternate_എണ്ണം->set (from, *i + 1);
}


#endif /* HB_OT_LAYOUT_HH */
