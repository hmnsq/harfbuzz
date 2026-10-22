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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-font.hh"
#ଅନ୍ତର୍ଭାଵ "hb-buffer.hh"
#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shape.hh"
#ଅନ୍ତର୍ଭାଵ "hb-set-digest.hh"


ସଂରଚନା hb_ot_shape_plan_t;


/*
 * kern
 */

HB_INTERNAL ତାର୍କିକ
hb_ot_layout_has_kerning (hb_face_t *face);

HB_INTERNAL ତାର୍କିକ
hb_ot_layout_has_machine_kerning (hb_face_t *face);

HB_INTERNAL ତାର୍କିକ
hb_ot_layout_has_cross_kerning (hb_face_t *face);

HB_INTERNAL ଶୂନ୍ୟ
hb_ot_layout_kern (ଅଚଳ hb_ot_shape_plan_t *plan,
		   hb_font_t *font,
		   hb_buffer_t  *buffer);


/* Private API corresponding to hb-ot-layout.h: */

HB_INTERNAL ତାର୍କିକ
hb_ot_layout_table_find_feature (hb_face_t    *face,
				 hb_tag_t      table_tag,
				 hb_tag_t      feature_tag,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *feature_index);


/*
 * GDEF
 */

ଗଣନା hb_ot_layout_glyph_props_flags_t
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
HB_INTERNAL ଶୂନ୍ୟ
hb_ot_layout_substitute_start (hb_font_t    *font,
			       hb_buffer_t  *buffer);

HB_INTERNAL ଶୂନ୍ୟ
hb_ot_layout_substitute_start_with_digest (hb_font_t    *font,
					   hb_buffer_t  *buffer);

ନାମାକାଶ OT {
  ସଂରଚନା hb_ot_apply_context_t;
  ସଂରଚନା hb_ot_layout_lookup_accelerator_t;
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {
  ସଂରଚନା SubstLookup;
}
}
}

HB_INTERNAL ଶୂନ୍ୟ
hb_ot_layout_substitute_lookup (OT::hb_ot_apply_context_t *c,
				ଅଚଳ OT::Layout::GSUB_impl::SubstLookup &lookup,
				ଅଚଳ OT::hb_ot_layout_lookup_accelerator_t &accel);


/* Should be called before all the position_lookup's are done. */
HB_INTERNAL ଶୂନ୍ୟ
hb_ot_layout_position_start (hb_font_t    *font,
			     hb_buffer_t  *buffer);

/* Should be called after all the position_lookup's are done, to fini advances. */
HB_INTERNAL ଶୂନ୍ୟ
hb_ot_layout_position_finish_advances (hb_font_t    *font,
				       hb_buffer_t  *buffer);

/* Should be called after hb_ot_layout_position_finish_advances, to fini offsets. */
HB_INTERNAL ଶୂନ୍ୟ
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
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା \
       ଗଣନା_3 = buffer->len, \
       start = 0, end = ଗଣନା_3 ? _hb_next_syllable (buffer, 0) : 0; \
       start < ଗଣନା_3; \
       start = end, end = _hb_next_syllable (buffer, start))

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
_hb_next_syllable (hb_buffer_t *buffer, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start)
{
  hb_glyph_info_t *info = buffer->info;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = start + hb_min (buffer->len - start,
				       (unsigned) HB_MAX_SYLLABLE_LENGTH);

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା syllable = info[start].syllable();
  ଯେତେବେଳେ (++start < ଗଣନା_2 && syllable == info[start].syllable())
    ;

  ଫେରନ୍ତୁ start;
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

ଗଣନା hb_unicode_props_flags_t {
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

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_glyph_info_set_unicode_props (hb_glyph_info_t *info, hb_buffer_t *buffer)
{
  hb_unicode_funcs_t *unicode = buffer->unicode;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା u = info->codepoint;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା gen_cat = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) unicode->general_category (u);
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା props = gen_cat;

  ଯଦି (u >= 0x80u)
  {
    ଯଦି (unlikely (unicode->is_default_ignorable (u)))
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES;
      props |=  UPROPS_MASK_IGNORABLE;
      ଯଦି (u == 0x200Cu) props |= UPROPS_MASK_Cf_ZWNJ;
      ଅନ୍ୟଥା ଯଦି (u == 0x200Du) props |= UPROPS_MASK_Cf_ZWJ;
      /* Mongolian Free Variation Selectors need to be remembered
       * because although we need to hide them like default-ignorables,
       * they need to non-ignorable during shaping.  This is similar to
       * what we do for joiners in Indic-like shapers, but since the
       * FVSes are GC=Mn, we have use a separate bit to remember them.
       * Fixes:
       * https://github.com/harfbuzz/harfbuzz/issues/234 */
      ଅନ୍ୟଥା ଯଦି (unlikely (hb_in_ranges<hb_codepoint_t> (u, 0x180Bu, 0x180Du, 0x180Fu, 0x180Fu))) props |= UPROPS_MASK_HIDDEN;
      /* TAG characters need similar treatment. Fixes:
       * https://github.com/harfbuzz/harfbuzz/issues/463 */
      ଅନ୍ୟଥା ଯଦି (unlikely (hb_in_range<hb_codepoint_t> (u, 0xE0020u, 0xE007Fu))) props |= UPROPS_MASK_HIDDEN;
      /* COMBINING GRAPHEME JOINER should not be skipped during GSUB either.
       * https://github.com/harfbuzz/harfbuzz/issues/554 */
      ଅନ୍ୟଥା ଯଦି (unlikely (u == 0x034Fu))
      {
	buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_CGJ;
	props |= UPROPS_MASK_HIDDEN;
      }
    }

    ଯଦି (unlikely (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (gen_cat)))
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_CONTINUATIONS;
      props |= UPROPS_MASK_CONTINUATION;
      props |= unicode->modified_combining_class (u)<<8;
    }
  }

  info->unicode_props() = props;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_glyph_info_set_general_category (hb_glyph_info_t *info,
				     hb_unicode_general_category_t gen_cat)
{
  /* Clears top-byte. */
  info->unicode_props() = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) gen_cat | (info->unicode_props() & (0xFF & ~UPROPS_MASK_GEN_CAT));
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_unicode_general_category_t
_hb_glyph_info_get_general_category (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ (hb_unicode_general_category_t) (info->unicode_props() & UPROPS_MASK_GEN_CAT);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_is_unicode_mark (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_IS_MARK (info->unicode_props() & UPROPS_MASK_GEN_CAT);
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_glyph_info_set_modified_combining_class (hb_glyph_info_t *info,
					     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା modified_class)
{
  ଯଦି (unlikely (!_hb_glyph_info_is_unicode_mark (info)))
    ଫେରନ୍ତୁ;
  info->unicode_props() = (modified_class<<8) | (info->unicode_props() & 0xFF);
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
_hb_glyph_info_get_modified_combining_class (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ _hb_glyph_info_is_unicode_mark (info) ? info->unicode_props()>>8 : 0;
}
#define info_cc(info) (_hb_glyph_info_get_modified_combining_class (&(info)))

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_is_unicode_space (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_glyph_info_set_unicode_space_fallback_type (hb_glyph_info_t *info, hb_unicode_funcs_t::space_t s)
{
  ଯଦି (unlikely (!_hb_glyph_info_is_unicode_space (info)))
    ଫେରନ୍ତୁ;
  info->unicode_props() = (((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) s)<<8) | (info->unicode_props() & 0xFF);
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_unicode_funcs_t::space_t
_hb_glyph_info_get_unicode_space_fallback_type (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ _hb_glyph_info_is_unicode_space (info) ?
	 (hb_unicode_funcs_t::space_t) (info->unicode_props()>>8) :
	 hb_unicode_funcs_t::NOT_SPACE;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_is_variation_selector (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_FORMAT &&
	 (info->unicode_props() & UPROPS_MASK_Cf_VS);
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_glyph_info_set_variation_selector (hb_glyph_info_t *info, ତାର୍କିକ customize)
{
  ଯଦି (customize)
  {
    _hb_glyph_info_set_general_category (info, HB_UNICODE_GENERAL_CATEGORY_FORMAT);
    info->unicode_props() |= UPROPS_MASK_Cf_VS;
  }
  ଅନ୍ୟଥା
  {
    // Reset to their original condition
    _hb_glyph_info_set_general_category (info, HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK);
  }
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ _hb_glyph_info_substituted (ଅଚଳ hb_glyph_info_t *info);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_is_default_ignorable (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ (info->unicode_props() & UPROPS_MASK_IGNORABLE) &&
	 !_hb_glyph_info_substituted (info);
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_glyph_info_set_default_ignorable (hb_glyph_info_t *info)
{
  info->unicode_props() |= UPROPS_MASK_IGNORABLE;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_glyph_info_clear_default_ignorable (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_MASK_IGNORABLE;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_is_hidden (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ info->unicode_props() & UPROPS_MASK_HIDDEN;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_glyph_info_unhide (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_MASK_HIDDEN;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_glyph_info_set_continuation (hb_glyph_info_t *info, hb_buffer_t *buffer)
{
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_CONTINUATIONS;
  info->unicode_props() |= UPROPS_MASK_CONTINUATION;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_glyph_info_clear_continuation (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_MASK_CONTINUATION;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_is_continuation (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ info->unicode_props() & UPROPS_MASK_CONTINUATION;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_grapheme_group_func (ଅଚଳ hb_glyph_info_t& a HB_UNUSED,
			 ଅଚଳ hb_glyph_info_t& b)
{ ଫେରନ୍ତୁ _hb_glyph_info_is_continuation (&b); }

#define foreach_grapheme(buffer, start, end) \
	foreach_group (buffer, start, end, _hb_grapheme_group_func)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_ot_layout_reverse_graphemes (hb_buffer_t *buffer)
{
  // MONOTONE_GRAPHEMES was already applied and is taken care of by _hb_grapheme_group_func.
  // So we just check for MONOTONE_CHARACTERS here.
  buffer->reverse_groups (_hb_grapheme_group_func,
			  buffer->cluster_level == HB_BUFFER_CLUSTER_LEVEL_MONOTONE_CHARACTERS);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_is_unicode_format (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_FORMAT;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_is_zwnj (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_MASK_Cf_ZWNJ);
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_is_zwj (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_MASK_Cf_ZWJ);
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_is_aat_deleted (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_MASK_Cf_AAT_DELETED);
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
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

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_glyph_info_clear_lig_props (hb_glyph_info_t *info)
{
  info->lig_props() = 0;
}

#define IS_LIG_BASE 0x10

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_glyph_info_set_lig_props_for_ligature (hb_glyph_info_t *info,
					   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lig_id,
					   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lig_ଅଙ୍କ_comps)
{
  info->lig_props() = (lig_id << 5) | IS_LIG_BASE | (lig_ଅଙ୍କ_comps & 0x0F);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_glyph_info_set_lig_props_for_mark (hb_glyph_info_t *info,
				       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lig_id,
				       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lig_comp)
{
  info->lig_props() = (lig_id << 5) | (lig_comp & 0x0F);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_glyph_info_set_lig_props_for_component (hb_glyph_info_t *info, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା comp)
{
  _hb_glyph_info_set_lig_props_for_mark (info, 0, comp);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
_hb_glyph_info_get_lig_id (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ info->lig_props() >> 5;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_ligated_internal (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ info->lig_props() & IS_LIG_BASE;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
_hb_glyph_info_get_lig_comp (ଅଚଳ hb_glyph_info_t *info)
{
  ଯଦି (_hb_glyph_info_ligated_internal (info))
    ଫେରନ୍ତୁ 0;
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ info->lig_props() & 0x0F;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_glyph_info_get_lig_ଅଙ୍କ_comps (ଅଚଳ hb_glyph_info_t *info)
{
  ଯଦି ((info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE) &&
      _hb_glyph_info_ligated_internal (info))
    ଫେରନ୍ତୁ info->lig_props() & 0x0F;
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ uint8_t
_hb_allocate_lig_id (hb_buffer_t *buffer)
{
  uint8_t lig_id = buffer->next_serial () & 0x07;
  ଯଦି (unlikely (!lig_id))
    lig_id = _hb_allocate_lig_id (buffer); /* in case of overflow */
  ଫେରନ୍ତୁ lig_id;
}

/* glyph_props: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_glyph_info_set_glyph_props (hb_glyph_info_t *info, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା props)
{
  info->glyph_props() = props;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
_hb_glyph_info_get_glyph_props (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ info->glyph_props();
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_is_base_glyph (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_is_ligature (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_is_mark (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_MARK;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_substituted (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_ligated (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATED;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_multiplied (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_glyph_info_get_lig_ଅଙ୍କ_comps_in_ligation (ଅଚଳ hb_glyph_info_t *info)
{
  /* When a glyph is decomposed by a MultipleSubst and its pieces later become
   * components of a ligature, the pieces belong to the same ligature component
   * as the first piece, matching how MarkBasePos attaches marks only to the
   * first piece.  So the non-first pieces contribute no extra component.
   * https://github.com/harfbuzz/harfbuzz/issues/4969 */
  ଯଦି (_hb_glyph_info_multiplied (info) &&
      _hb_glyph_info_get_lig_comp (info))
    ଫେରନ୍ତୁ 0;
  ଫେରନ୍ତୁ hb_glyph_info_get_lig_ଅଙ୍କ_comps (info);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_glyph_info_ligated_and_didnt_multiply (ଅଚଳ hb_glyph_info_t *info)
{
  ଫେରନ୍ତୁ _hb_glyph_info_ligated (info) && !_hb_glyph_info_multiplied (info);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_glyph_info_clear_ligated_and_multiplied (hb_glyph_info_t *info)
{
  info->glyph_props() &= ~(HB_OT_LAYOUT_GLYPH_PROPS_LIGATED |
			   HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_glyph_info_clear_substituted (hb_glyph_info_t *info)
{
  info->glyph_props() &= ~(HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_clear_substitution_flags (ଅଚଳ hb_ot_shape_plan_t *plan HB_UNUSED,
			      hb_font_t *font HB_UNUSED,
			      hb_buffer_t *buffer)
{
  hb_glyph_info_t *info = buffer->info;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = buffer->len;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    _hb_glyph_info_clear_substituted (&info[i]);
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}


/* Allocation / deallocation. */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_buffer_allocate_unicode_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, unicode_props);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_buffer_deallocate_unicode_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_DEALLOCATE_VAR (buffer, unicode_props);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_buffer_assert_unicode_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_ASSERT_VAR (buffer, unicode_props);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_buffer_allocate_gsubgpos_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, glyph_props);
  HB_BUFFER_ALLOCATE_VAR (buffer, lig_props);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_buffer_deallocate_gsubgpos_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_DEALLOCATE_VAR (buffer, lig_props);
  HB_BUFFER_DEALLOCATE_VAR (buffer, glyph_props);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_buffer_assert_gsubgpos_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_ASSERT_VAR (buffer, glyph_props);
  HB_BUFFER_ASSERT_VAR (buffer, lig_props);
}

/* Make sure no one directly touches our props... */
#undef unicode_props
#undef lig_props
#undef glyph_props

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_collect_glyph_alternates_add (hb_codepoint_t from,
				  hb_codepoint_t to,
				  hb_map_t *alternate_ଗଣନା,
				  hb_map_t *alternate_glyphs)
{
  hb_codepoint_t zero = 0;
  hb_codepoint_t *i = &zero;
  alternate_ଗଣନା->has (from, &i);
  alternate_glyphs->set (from | (*i << 24), to);
  alternate_ଗଣନା->set (from, *i + 1);
}


#endif /* HB_OT_LAYOUT_HH */
