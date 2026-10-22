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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_LAYOUT_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_LAYOUT_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-font.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-buffer.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shape.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set-digest.hh"


ਬਣਤਰ hb_ot_shape_plan_t;


/*
 * kern
 */

HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ
hb_ot_layout_has_kerning (hb_face_t *face);

HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ
hb_ot_layout_has_machine_kerning (hb_face_t *face);

HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ
hb_ot_layout_has_cross_kerning (hb_face_t *face);

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
hb_ot_layout_kern (ਅਚਲ hb_ot_shape_plan_t *plan,
		   hb_font_t *font,
		   hb_ਬਫਰ_t  *ਬਫਰ);


/* Private API corresponding to hb-ot-layout.h: */

HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ
hb_ot_layout_ਸਾਰਣੀ_ਲੱਭੋ_feature (hb_face_t    *face,
				 hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				 hb_ਟੈਗ_t      feature_ਟੈਗ,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_ਸੂਚਕਾਂਕ);


/*
 * GDEF
 */

ਗਿਣਤੀ hb_ot_layout_glyph_props_flags_t
{
  /* The following three match LookupFlags::Ignore* numbers. */
  HB_OT_LAYOUT_GLYPH_PROPS_ਅਧਾਰ_GLYPH	= 0x02u,
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
HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
hb_ot_layout_substitute_ਸ਼ੁਰੂ (hb_font_t    *font,
			       hb_ਬਫਰ_t  *ਬਫਰ);

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
hb_ot_layout_substitute_ਸ਼ੁਰੂ_with_digest (hb_font_t    *font,
					   hb_ਬਫਰ_t  *ਬਫਰ);

ਨਾਮਾਕਾਸ਼ OT {
  ਬਣਤਰ hb_ot_apply_context_t;
  ਬਣਤਰ hb_ot_layout_lookup_accelerator_t;
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GSUB_impl {
  ਬਣਤਰ SubstLookup;
}
}
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
hb_ot_layout_substitute_lookup (OT::hb_ot_apply_context_t *c,
				ਅਚਲ OT::Layout::GSUB_impl::SubstLookup &lookup,
				ਅਚਲ OT::hb_ot_layout_lookup_accelerator_t &accel);


/* Should be called before all the position_lookup's are done. */
HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
hb_ot_layout_position_ਸ਼ੁਰੂ (hb_font_t    *font,
			     hb_ਬਫਰ_t  *ਬਫਰ);

/* Should be called after all the position_lookup's are done, to fini advances. */
HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
hb_ot_layout_position_ਅੰਤ_advances (hb_font_t    *font,
				       hb_ਬਫਰ_t  *ਬਫਰ);

/* Should be called after hb_ot_layout_position_finish_advances, to fini offsets. */
HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
hb_ot_layout_position_ਅੰਤ_offsets (hb_font_t    *font,
				      hb_ਬਫਰ_t  *ਬਫਰ);


/*
 * Buffer var routines.
 */

/* buffer var allocations, used during the entire shaping process */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ unicode_props()		var2.u16[0]

/* buffer var allocations, used during the GSUB/GPOS processing */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ glyph_props()		var1.u16[0] /* GDEF glyph properties */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ lig_props()		var1.u8[2] /* GSUB/GPOS ligature tracking */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ syllable()		var1.u8[3] /* GSUB/GPOS shaping boundaries */


/* Loop over syllables. Based on foreach_cluster(). */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ foreach_syllable(ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ) \
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ \
       ਗਿਣਤੀ_3 = ਬਫਰ->len, \
       ਸ਼ੁਰੂ = 0, ਸਿਰਾ = ਗਿਣਤੀ_3 ? hb_ਅਗਲਾ_syllable (ਬਫਰ, 0) : 0; \
       ਸ਼ੁਰੂ < ਗਿਣਤੀ_3; \
       ਸ਼ੁਰੂ = ਸਿਰਾ, ਸਿਰਾ = hb_ਅਗਲਾ_syllable (ਬਫਰ, ਸ਼ੁਰੂ))

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ਅਗਲਾ_syllable (hb_ਬਫਰ_t *ਬਫਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ)
{
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਸ਼ੁਰੂ + hb_min (ਬਫਰ->len - ਸ਼ੁਰੂ,
				       (ਬਿਨਾਨਿਸ਼ਾਨੀ) HB_MAX_SYLLABLE_ਲੰਮਾਈ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ syllable = info[ਸ਼ੁਰੂ].syllable();
  ਜਦੋਂ (++ਸ਼ੁਰੂ < ਗਿਣਤੀ_2 && syllable == info[ਸ਼ੁਰੂ].syllable())
    ;

  ਵਾਪਸ ਸ਼ੁਰੂ;
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

ਗਿਣਤੀ hb_unicode_props_flags_t {
  UPROPS_ਨਕਾਬ_GEN_CAT	= 0x001Fu,
  UPROPS_ਨਕਾਬ_IGNORABLE	= 0x0020u,
  UPROPS_ਨਕਾਬ_HIDDEN	= 0x0040u, /* MONGOLIAN FREE VARIATION SELECTOR 1..4, or TAG characters, or CGJ sometimes */
  UPROPS_ਨਕਾਬ_CONTINUATION=0x0080u,

  /* If GEN_CAT=FORMAT, top byte masks: */
  UPROPS_ਨਕਾਬ_Cf_ZWJ	= 0x0100u,
  UPROPS_ਨਕਾਬ_Cf_ZWNJ	= 0x0200u,
  UPROPS_ਨਕਾਬ_Cf_VS	= 0x0400u,
  UPROPS_ਨਕਾਬ_Cf_AAT_DELETED	= 0x0800u
};
HB_MARK_AS_FLAG_T (hb_unicode_props_flags_t);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_glyph_info_ਸੈੱਟ_unicode_props (hb_glyph_info_t *info, hb_ਬਫਰ_t *ਬਫਰ)
{
  hb_unicode_funcs_t *unicode = ਬਫਰ->unicode;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ u = info->codepoint;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ gen_cat = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) unicode->general_category (u);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ props = gen_cat;

  ਜੇ (u >= 0x80u)
  {
    ਜੇ (unlikely (unicode->is_default_ignorable (u)))
    {
      ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES;
      props |=  UPROPS_ਨਕਾਬ_IGNORABLE;
      ਜੇ (u == 0x200Cu) props |= UPROPS_ਨਕਾਬ_Cf_ZWNJ;
      ਨਹੀਂਤਾਂ ਜੇ (u == 0x200Du) props |= UPROPS_ਨਕਾਬ_Cf_ZWJ;
      /* Mongolian Free Variation Selectors need to be remembered
       * because although we need to hide them like default-ignorables,
       * they need to non-ignorable during shaping.  This is similar to
       * what we do for joiners in Indic-like shapers, but since the
       * FVSes are GC=Mn, we have use a separate bit to remember them.
       * Fixes:
       * https://github.com/harfbuzz/harfbuzz/issues/234 */
      ਨਹੀਂਤਾਂ ਜੇ (unlikely (hb_ਵਿੱਚ_ranges<hb_codepoint_t> (u, 0x180Bu, 0x180Du, 0x180Fu, 0x180Fu))) props |= UPROPS_ਨਕਾਬ_HIDDEN;
      /* TAG characters need similar treatment. Fixes:
       * https://github.com/harfbuzz/harfbuzz/issues/463 */
      ਨਹੀਂਤਾਂ ਜੇ (unlikely (hb_ਵਿੱਚ_range<hb_codepoint_t> (u, 0xE0020u, 0xE007Fu))) props |= UPROPS_ਨਕਾਬ_HIDDEN;
      /* COMBINING GRAPHEME JOINER should not be skipped during GSUB either.
       * https://github.com/harfbuzz/harfbuzz/issues/554 */
      ਨਹੀਂਤਾਂ ਜੇ (unlikely (u == 0x034Fu))
      {
	ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_HAS_CGJ;
	props |= UPROPS_ਨਕਾਬ_HIDDEN;
      }
    }

    ਜੇ (unlikely (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (gen_cat)))
    {
      ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_HAS_CONTINUATIONS;
      props |= UPROPS_ਨਕਾਬ_CONTINUATION;
      props |= unicode->modified_combining_class (u)<<8;
    }
  }

  info->unicode_props() = props;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_glyph_info_ਸੈੱਟ_general_category (hb_glyph_info_t *info,
				     hb_unicode_general_category_t gen_cat)
{
  /* Clears top-byte. */
  info->unicode_props() = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) gen_cat | (info->unicode_props() & (0xFF & ~UPROPS_ਨਕਾਬ_GEN_CAT));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ hb_unicode_general_category_t
_hb_glyph_info_get_general_category (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ (hb_unicode_general_category_t) (info->unicode_props() & UPROPS_ਨਕਾਬ_GEN_CAT);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_glyph_info_is_unicode_mark (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_IS_MARK (info->unicode_props() & UPROPS_ਨਕਾਬ_GEN_CAT);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_glyph_info_ਸੈੱਟ_modified_combining_class (hb_glyph_info_t *info,
					     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ modified_class)
{
  ਜੇ (unlikely (!_hb_glyph_info_is_unicode_mark (info)))
    ਵਾਪਸ;
  info->unicode_props() = (modified_class<<8) | (info->unicode_props() & 0xFF);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
_hb_glyph_info_get_modified_combining_class (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ _hb_glyph_info_is_unicode_mark (info) ? info->unicode_props()>>8 : 0;
}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ info_cc(info) (_hb_glyph_info_get_modified_combining_class (&(info)))

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_glyph_info_is_unicode_space (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_glyph_info_ਸੈੱਟ_unicode_space_fallback_type (hb_glyph_info_t *info, hb_unicode_funcs_t::space_t s)
{
  ਜੇ (unlikely (!_hb_glyph_info_is_unicode_space (info)))
    ਵਾਪਸ;
  info->unicode_props() = (((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) s)<<8) | (info->unicode_props() & 0xFF);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ hb_unicode_funcs_t::space_t
_hb_glyph_info_get_unicode_space_fallback_type (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ _hb_glyph_info_is_unicode_space (info) ?
	 (hb_unicode_funcs_t::space_t) (info->unicode_props()>>8) :
	 hb_unicode_funcs_t::NOT_SPACE;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_glyph_info_is_variation_selector (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_FORMAT &&
	 (info->unicode_props() & UPROPS_ਨਕਾਬ_Cf_VS);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_glyph_info_ਸੈੱਟ_variation_selector (hb_glyph_info_t *info, ਤਾਰਕਿਕ customize)
{
  ਜੇ (customize)
  {
    hb_glyph_info_ਸੈੱਟ_general_category (info, HB_UNICODE_GENERAL_CATEGORY_FORMAT);
    info->unicode_props() |= UPROPS_ਨਕਾਬ_Cf_VS;
  }
  ਨਹੀਂਤਾਂ
  {
    // Reset to their original condition
    hb_glyph_info_ਸੈੱਟ_general_category (info, HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK);
  }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ _hb_glyph_info_substituted (ਅਚਲ hb_glyph_info_t *info);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_glyph_info_is_default_ignorable (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ (info->unicode_props() & UPROPS_ਨਕਾਬ_IGNORABLE) &&
	 !_hb_glyph_info_substituted (info);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_glyph_info_ਸੈੱਟ_default_ignorable (hb_glyph_info_t *info)
{
  info->unicode_props() |= UPROPS_ਨਕਾਬ_IGNORABLE;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
_hb_glyph_info_clear_default_ignorable (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_ਨਕਾਬ_IGNORABLE;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_glyph_info_is_hidden (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ info->unicode_props() & UPROPS_ਨਕਾਬ_HIDDEN;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
_hb_glyph_info_unhide (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_ਨਕਾਬ_HIDDEN;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_glyph_info_ਸੈੱਟ_continuation (hb_glyph_info_t *info, hb_ਬਫਰ_t *ਬਫਰ)
{
  ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_HAS_CONTINUATIONS;
  info->unicode_props() |= UPROPS_ਨਕਾਬ_CONTINUATION;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
_hb_glyph_info_clear_continuation (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_ਨਕਾਬ_CONTINUATION;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_glyph_info_is_continuation (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ info->unicode_props() & UPROPS_ਨਕਾਬ_CONTINUATION;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
hb_grapheme_ਸਮੂਹ_func (ਅਚਲ hb_glyph_info_t& a HB_ਅਣਵਰਤਿਆ,
			 ਅਚਲ hb_glyph_info_t& b)
{ ਵਾਪਸ _hb_glyph_info_is_continuation (&b); }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ foreach_grapheme(ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ) \
	foreach_ਸਮੂਹ (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ, hb_grapheme_ਸਮੂਹ_func)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ot_layout_ਉਲਟ_graphemes (hb_ਬਫਰ_t *ਬਫਰ)
{
  // MONOTONE_GRAPHEMES was already applied and is taken care of by _hb_grapheme_group_func.
  // So we just check for MONOTONE_CHARACTERS here.
  ਬਫਰ->ਉਲਟ_groups (hb_grapheme_ਸਮੂਹ_func,
			  ਬਫਰ->cluster_level == HB_ਬਫਰ_CLUSTER_LEVEL_MONOTONE_CHARACTERS);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_glyph_info_is_unicode_format (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_FORMAT;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_glyph_info_is_zwnj (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_ਨਕਾਬ_Cf_ZWNJ);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_glyph_info_is_zwj (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_ਨਕਾਬ_Cf_ZWJ);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_glyph_info_is_aat_deleted (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_ਨਕਾਬ_Cf_AAT_DELETED);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_glyph_info_ਸੈੱਟ_aat_deleted (hb_glyph_info_t *info)
{
  hb_glyph_info_ਸੈੱਟ_general_category (info, HB_UNICODE_GENERAL_CATEGORY_FORMAT);
  info->unicode_props() |= UPROPS_ਨਕਾਬ_Cf_AAT_DELETED;
  info->unicode_props() |= UPROPS_ਨਕਾਬ_HIDDEN;
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

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
_hb_glyph_info_clear_lig_props (hb_glyph_info_t *info)
{
  info->lig_props() = 0;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IS_LIG_ਅਧਾਰ 0x10

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_glyph_info_ਸੈੱਟ_lig_props_for_ligature (hb_glyph_info_t *info,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lig_id,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lig_ਅੰਕ_comps)
{
  info->lig_props() = (lig_id << 5) | IS_LIG_ਅਧਾਰ | (lig_ਅੰਕ_comps & 0x0F);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_glyph_info_ਸੈੱਟ_lig_props_for_mark (hb_glyph_info_t *info,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lig_id,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lig_comp)
{
  info->lig_props() = (lig_id << 5) | (lig_comp & 0x0F);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_glyph_info_ਸੈੱਟ_lig_props_for_component (hb_glyph_info_t *info, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ comp)
{
  hb_glyph_info_ਸੈੱਟ_lig_props_for_mark (info, 0, comp);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
_hb_glyph_info_get_lig_id (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ info->lig_props() >> 5;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
hb_glyph_info_ligated_ਅੰਦਰੂਨੀ (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ info->lig_props() & IS_LIG_ਅਧਾਰ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
_hb_glyph_info_get_lig_comp (ਅਚਲ hb_glyph_info_t *info)
{
  ਜੇ (hb_glyph_info_ligated_ਅੰਦਰੂਨੀ (info))
    ਵਾਪਸ 0;
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ info->lig_props() & 0x0F;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_glyph_info_get_lig_ਅੰਕ_comps (ਅਚਲ hb_glyph_info_t *info)
{
  ਜੇ ((info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE) &&
      hb_glyph_info_ligated_ਅੰਦਰੂਨੀ (info))
    ਵਾਪਸ info->lig_props() & 0x0F;
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ 1;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ uint8_t
_hb_allocate_lig_id (hb_ਬਫਰ_t *ਬਫਰ)
{
  uint8_t lig_id = ਬਫਰ->ਅਗਲਾ_serial () & 0x07;
  ਜੇ (unlikely (!lig_id))
    lig_id = _hb_allocate_lig_id (ਬਫਰ); /* in case of overflow */
  ਵਾਪਸ lig_id;
}

/* glyph_props: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_glyph_info_ਸੈੱਟ_glyph_props (hb_glyph_info_t *info, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ props)
{
  info->glyph_props() = props;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
_hb_glyph_info_get_glyph_props (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ info->glyph_props();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
hb_glyph_info_is_ਅਧਾਰ_glyph (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_ਅਧਾਰ_GLYPH;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_glyph_info_is_ligature (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_glyph_info_is_mark (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_MARK;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_glyph_info_substituted (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_glyph_info_ligated (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATED;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_glyph_info_multiplied (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_glyph_info_get_lig_ਅੰਕ_comps_ਵਿੱਚ_ligation (ਅਚਲ hb_glyph_info_t *info)
{
  /* When a glyph is decomposed by a MultipleSubst and its pieces later become
   * components of a ligature, the pieces belong to the same ligature component
   * as the first piece, matching how MarkBasePos attaches marks only to the
   * first piece.  So the non-first pieces contribute no extra component.
   * https://github.com/harfbuzz/harfbuzz/issues/4969 */
  ਜੇ (_hb_glyph_info_multiplied (info) &&
      _hb_glyph_info_get_lig_comp (info))
    ਵਾਪਸ 0;
  ਵਾਪਸ hb_glyph_info_get_lig_ਅੰਕ_comps (info);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_glyph_info_ligated_and_didnt_multiply (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ _hb_glyph_info_ligated (info) && !_hb_glyph_info_multiplied (info);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
_hb_glyph_info_clear_ligated_and_multiplied (hb_glyph_info_t *info)
{
  info->glyph_props() &= ~(HB_OT_LAYOUT_GLYPH_PROPS_LIGATED |
			   HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
_hb_glyph_info_clear_substituted (hb_glyph_info_t *info)
{
  info->glyph_props() &= ~(HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_clear_substitution_flags (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
			      hb_font_t *font HB_ਅਣਵਰਤਿਆ,
			      hb_ਬਫਰ_t *ਬਫਰ)
{
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    _hb_glyph_info_clear_substituted (&info[i]);
  ਵਾਪਸ ਝੂਠ;
}


/* Allocation / deallocation. */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ਬਫਰ_allocate_unicode_vars (hb_ਬਫਰ_t *ਬਫਰ)
{
  HB_ਬਫਰ_ALLOCATE_VAR (ਬਫਰ, unicode_props);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ਬਫਰ_deallocate_unicode_vars (hb_ਬਫਰ_t *ਬਫਰ)
{
  HB_ਬਫਰ_DEALLOCATE_VAR (ਬਫਰ, unicode_props);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ਬਫਰ_assert_unicode_vars (hb_ਬਫਰ_t *ਬਫਰ)
{
  HB_ਬਫਰ_ASSERT_VAR (ਬਫਰ, unicode_props);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ਬਫਰ_allocate_gsubgpos_vars (hb_ਬਫਰ_t *ਬਫਰ)
{
  HB_ਬਫਰ_ALLOCATE_VAR (ਬਫਰ, glyph_props);
  HB_ਬਫਰ_ALLOCATE_VAR (ਬਫਰ, lig_props);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ਬਫਰ_deallocate_gsubgpos_vars (hb_ਬਫਰ_t *ਬਫਰ)
{
  HB_ਬਫਰ_DEALLOCATE_VAR (ਬਫਰ, lig_props);
  HB_ਬਫਰ_DEALLOCATE_VAR (ਬਫਰ, glyph_props);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ਬਫਰ_assert_gsubgpos_vars (hb_ਬਫਰ_t *ਬਫਰ)
{
  HB_ਬਫਰ_ASSERT_VAR (ਬਫਰ, glyph_props);
  HB_ਬਫਰ_ASSERT_VAR (ਬਫਰ, lig_props);
}

/* Make sure no one directly touches our props... */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ unicode_props
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ lig_props
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ glyph_props

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_collect_glyph_alternates_ਜੋੜੋ (hb_codepoint_t from,
				  hb_codepoint_t to,
				  hb_map_t *alternate_ਗਿਣਤੀ,
				  hb_map_t *alternate_glyphs)
{
  hb_codepoint_t zero = 0;
  hb_codepoint_t *i = &zero;
  alternate_ਗਿਣਤੀ->has (from, &i);
  alternate_glyphs->ਸੈੱਟ (from | (*i << 24), to);
  alternate_ਗਿਣਤੀ->ਸੈੱਟ (from, *i + 1);
}


#ਜੇ_ਅੰਤ /* HB_OT_LAYOUT_HH */
