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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-font.hh"
#अन्तर्भाव "hb-buffer.hh"
#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-shape.hh"
#अन्तर्भाव "hb-set-digest.hh"


संरचना hb_ot_shape_plan_t;


/*
 * kern
 */

HB_INTERNAL तार्किक
hb_ot_layout_has_kerning (hb_face_t *face);

HB_INTERNAL तार्किक
hb_ot_layout_has_machine_kerning (hb_face_t *face);

HB_INTERNAL तार्किक
hb_ot_layout_has_cross_kerning (hb_face_t *face);

HB_INTERNAL शून्य
hb_ot_layout_kern (स्थिरांक hb_ot_shape_plan_t *plan,
		   hb_font_t *font,
		   hb_buffer_t  *buffer);


/* Private API corresponding to hb-ot-layout.h: */

HB_INTERNAL तार्किक
hb_ot_layout_table_find_feature (hb_face_t    *face,
				 hb_tag_t      table_tag,
				 hb_tag_t      feature_tag,
				 अचिन्हित पूर्णांक *feature_index);


/*
 * GDEF
 */

गणना hb_ot_layout_glyph_props_flags_t
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
HB_INTERNAL शून्य
hb_ot_layout_substitute_start (hb_font_t    *font,
			       hb_buffer_t  *buffer);

HB_INTERNAL शून्य
hb_ot_layout_substitute_start_with_digest (hb_font_t    *font,
					   hb_buffer_t  *buffer);

नामाकाश OT {
  संरचना hb_ot_apply_context_t;
  संरचना hb_ot_layout_lookup_accelerator_t;
नामाकाश Layout {
नामाकाश GSUB_impl {
  संरचना SubstLookup;
}
}
}

HB_INTERNAL शून्य
hb_ot_layout_substitute_lookup (OT::hb_ot_apply_context_t *c,
				स्थिरांक OT::Layout::GSUB_impl::SubstLookup &lookup,
				स्थिरांक OT::hb_ot_layout_lookup_accelerator_t &accel);


/* Should be called before all the position_lookup's are done. */
HB_INTERNAL शून्य
hb_ot_layout_position_start (hb_font_t    *font,
			     hb_buffer_t  *buffer);

/* Should be called after all the position_lookup's are done, to fini advances. */
HB_INTERNAL शून्य
hb_ot_layout_position_finish_advances (hb_font_t    *font,
				       hb_buffer_t  *buffer);

/* Should be called after hb_ot_layout_position_finish_advances, to fini offsets. */
HB_INTERNAL शून्य
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
  साठी (अचिन्हित पूर्णांक \
       मोजणी_2 = buffer->len, \
       start = 0, end = मोजणी_2 ? _hb_next_syllable (buffer, 0) : 0; \
       start < मोजणी_2; \
       start = end, end = _hb_next_syllable (buffer, start))

स्थिर ओळीत अचिन्हित पूर्णांक
_hb_next_syllable (hb_buffer_t *buffer, अचिन्हित पूर्णांक start)
{
  hb_glyph_info_t *info = buffer->info;
  अचिन्हित पूर्णांक मोजणी = start + hb_min (buffer->len - start,
				       (अचिन्हित) HB_MAX_SYLLABLE_LENGTH);

  अचिन्हित पूर्णांक syllable = info[start].syllable();
  पर्यंत (++start < मोजणी && syllable == info[start].syllable())
    ;

  परत start;
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

गणना hb_unicode_props_flags_t {
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

स्थिर ओळीत शून्य
_hb_glyph_info_set_unicode_props (hb_glyph_info_t *info, hb_buffer_t *buffer)
{
  hb_unicode_funcs_t *unicode = buffer->unicode;
  अचिन्हित पूर्णांक u = info->codepoint;
  अचिन्हित पूर्णांक gen_cat = (अचिन्हित पूर्णांक) unicode->general_category (u);
  अचिन्हित पूर्णांक props = gen_cat;

  जर (u >= 0x80u)
  {
    जर (unlikely (unicode->is_default_ignorable (u)))
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES;
      props |=  UPROPS_MASK_IGNORABLE;
      जर (u == 0x200Cu) props |= UPROPS_MASK_Cf_ZWNJ;
      अन्यथा जर (u == 0x200Du) props |= UPROPS_MASK_Cf_ZWJ;
      /* Mongolian Free Variation Selectors need to be remembered
       * because although we need to hide them like default-ignorables,
       * they need to non-ignorable during shaping.  This is similar to
       * what we do for joiners in Indic-like shapers, but since the
       * FVSes are GC=Mn, we have use a separate bit to remember them.
       * Fixes:
       * https://github.com/harfbuzz/harfbuzz/issues/234 */
      अन्यथा जर (unlikely (hb_in_ranges<hb_codepoint_t> (u, 0x180Bu, 0x180Du, 0x180Fu, 0x180Fu))) props |= UPROPS_MASK_HIDDEN;
      /* TAG characters need similar treatment. Fixes:
       * https://github.com/harfbuzz/harfbuzz/issues/463 */
      अन्यथा जर (unlikely (hb_in_range<hb_codepoint_t> (u, 0xE0020u, 0xE007Fu))) props |= UPROPS_MASK_HIDDEN;
      /* COMBINING GRAPHEME JOINER should not be skipped during GSUB either.
       * https://github.com/harfbuzz/harfbuzz/issues/554 */
      अन्यथा जर (unlikely (u == 0x034Fu))
      {
	buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_CGJ;
	props |= UPROPS_MASK_HIDDEN;
      }
    }

    जर (unlikely (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (gen_cat)))
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_CONTINUATIONS;
      props |= UPROPS_MASK_CONTINUATION;
      props |= unicode->modified_combining_class (u)<<8;
    }
  }

  info->unicode_props() = props;
}

स्थिर ओळीत शून्य
_hb_glyph_info_set_general_category (hb_glyph_info_t *info,
				     hb_unicode_general_category_t gen_cat)
{
  /* Clears top-byte. */
  info->unicode_props() = (अचिन्हित पूर्णांक) gen_cat | (info->unicode_props() & (0xFF & ~UPROPS_MASK_GEN_CAT));
}

स्थिर ओळीत hb_unicode_general_category_t
_hb_glyph_info_get_general_category (स्थिरांक hb_glyph_info_t *info)
{
  परत (hb_unicode_general_category_t) (info->unicode_props() & UPROPS_MASK_GEN_CAT);
}

स्थिर ओळीत तार्किक
_hb_glyph_info_is_unicode_mark (स्थिरांक hb_glyph_info_t *info)
{
  परत HB_UNICODE_GENERAL_CATEGORY_IS_MARK (info->unicode_props() & UPROPS_MASK_GEN_CAT);
}
स्थिर ओळीत शून्य
_hb_glyph_info_set_modified_combining_class (hb_glyph_info_t *info,
					     अचिन्हित पूर्णांक modified_class)
{
  जर (unlikely (!_hb_glyph_info_is_unicode_mark (info)))
    परत;
  info->unicode_props() = (modified_class<<8) | (info->unicode_props() & 0xFF);
}
स्थिर ओळीत अचिन्हित पूर्णांक
_hb_glyph_info_get_modified_combining_class (स्थिरांक hb_glyph_info_t *info)
{
  परत _hb_glyph_info_is_unicode_mark (info) ? info->unicode_props()>>8 : 0;
}
#define info_cc(info) (_hb_glyph_info_get_modified_combining_class (&(info)))

स्थिर ओळीत तार्किक
_hb_glyph_info_is_unicode_space (स्थिरांक hb_glyph_info_t *info)
{
  परत _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
}
स्थिर ओळीत शून्य
_hb_glyph_info_set_unicode_space_fallback_type (hb_glyph_info_t *info, hb_unicode_funcs_t::space_t s)
{
  जर (unlikely (!_hb_glyph_info_is_unicode_space (info)))
    परत;
  info->unicode_props() = (((अचिन्हित पूर्णांक) s)<<8) | (info->unicode_props() & 0xFF);
}
स्थिर ओळीत hb_unicode_funcs_t::space_t
_hb_glyph_info_get_unicode_space_fallback_type (स्थिरांक hb_glyph_info_t *info)
{
  परत _hb_glyph_info_is_unicode_space (info) ?
	 (hb_unicode_funcs_t::space_t) (info->unicode_props()>>8) :
	 hb_unicode_funcs_t::NOT_SPACE;
}
स्थिर ओळीत तार्किक
_hb_glyph_info_is_variation_selector (स्थिरांक hb_glyph_info_t *info)
{
  परत _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_FORMAT &&
	 (info->unicode_props() & UPROPS_MASK_Cf_VS);
}
स्थिर ओळीत शून्य
_hb_glyph_info_set_variation_selector (hb_glyph_info_t *info, तार्किक customize)
{
  जर (customize)
  {
    _hb_glyph_info_set_general_category (info, HB_UNICODE_GENERAL_CATEGORY_FORMAT);
    info->unicode_props() |= UPROPS_MASK_Cf_VS;
  }
  अन्यथा
  {
    // Reset to their original condition
    _hb_glyph_info_set_general_category (info, HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK);
  }
}

स्थिर ओळीत तार्किक _hb_glyph_info_substituted (स्थिरांक hb_glyph_info_t *info);

स्थिर ओळीत तार्किक
_hb_glyph_info_is_default_ignorable (स्थिरांक hb_glyph_info_t *info)
{
  परत (info->unicode_props() & UPROPS_MASK_IGNORABLE) &&
	 !_hb_glyph_info_substituted (info);
}
स्थिर ओळीत शून्य
_hb_glyph_info_set_default_ignorable (hb_glyph_info_t *info)
{
  info->unicode_props() |= UPROPS_MASK_IGNORABLE;
}
स्थिर ओळीत शून्य
_hb_glyph_info_clear_default_ignorable (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_MASK_IGNORABLE;
}
स्थिर ओळीत तार्किक
_hb_glyph_info_is_hidden (स्थिरांक hb_glyph_info_t *info)
{
  परत info->unicode_props() & UPROPS_MASK_HIDDEN;
}
स्थिर ओळीत शून्य
_hb_glyph_info_unhide (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_MASK_HIDDEN;
}

स्थिर ओळीत शून्य
_hb_glyph_info_set_continuation (hb_glyph_info_t *info, hb_buffer_t *buffer)
{
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_CONTINUATIONS;
  info->unicode_props() |= UPROPS_MASK_CONTINUATION;
}
स्थिर ओळीत शून्य
_hb_glyph_info_clear_continuation (hb_glyph_info_t *info)
{
  info->unicode_props() &= ~ UPROPS_MASK_CONTINUATION;
}
स्थिर ओळीत तार्किक
_hb_glyph_info_is_continuation (स्थिरांक hb_glyph_info_t *info)
{
  परत info->unicode_props() & UPROPS_MASK_CONTINUATION;
}

स्थिर ओळीत तार्किक
_hb_grapheme_group_func (स्थिरांक hb_glyph_info_t& a HB_UNUSED,
			 स्थिरांक hb_glyph_info_t& b)
{ परत _hb_glyph_info_is_continuation (&b); }

#define foreach_grapheme(buffer, start, end) \
	foreach_group (buffer, start, end, _hb_grapheme_group_func)

स्थिर ओळीत शून्य
_hb_ot_layout_reverse_graphemes (hb_buffer_t *buffer)
{
  // MONOTONE_GRAPHEMES was already applied and is taken care of by _hb_grapheme_group_func.
  // So we just check for MONOTONE_CHARACTERS here.
  buffer->reverse_groups (_hb_grapheme_group_func,
			  buffer->cluster_level == HB_BUFFER_CLUSTER_LEVEL_MONOTONE_CHARACTERS);
}

स्थिर ओळीत तार्किक
_hb_glyph_info_is_unicode_format (स्थिरांक hb_glyph_info_t *info)
{
  परत _hb_glyph_info_get_general_category (info) ==
	 HB_UNICODE_GENERAL_CATEGORY_FORMAT;
}
स्थिर ओळीत तार्किक
_hb_glyph_info_is_zwnj (स्थिरांक hb_glyph_info_t *info)
{
  परत _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_MASK_Cf_ZWNJ);
}
स्थिर ओळीत तार्किक
_hb_glyph_info_is_zwj (स्थिरांक hb_glyph_info_t *info)
{
  परत _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_MASK_Cf_ZWJ);
}
स्थिर ओळीत तार्किक
_hb_glyph_info_is_aat_deleted (स्थिरांक hb_glyph_info_t *info)
{
  परत _hb_glyph_info_is_unicode_format (info) && (info->unicode_props() & UPROPS_MASK_Cf_AAT_DELETED);
}
स्थिर ओळीत शून्य
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

स्थिर ओळीत शून्य
_hb_glyph_info_clear_lig_props (hb_glyph_info_t *info)
{
  info->lig_props() = 0;
}

#define IS_LIG_BASE 0x10

स्थिर ओळीत शून्य
_hb_glyph_info_set_lig_props_for_ligature (hb_glyph_info_t *info,
					   अचिन्हित पूर्णांक lig_id,
					   अचिन्हित पूर्णांक lig_अंक_comps)
{
  info->lig_props() = (lig_id << 5) | IS_LIG_BASE | (lig_अंक_comps & 0x0F);
}

स्थिर ओळीत शून्य
_hb_glyph_info_set_lig_props_for_mark (hb_glyph_info_t *info,
				       अचिन्हित पूर्णांक lig_id,
				       अचिन्हित पूर्णांक lig_comp)
{
  info->lig_props() = (lig_id << 5) | (lig_comp & 0x0F);
}

स्थिर ओळीत शून्य
_hb_glyph_info_set_lig_props_for_component (hb_glyph_info_t *info, अचिन्हित पूर्णांक comp)
{
  _hb_glyph_info_set_lig_props_for_mark (info, 0, comp);
}

स्थिर ओळीत अचिन्हित पूर्णांक
_hb_glyph_info_get_lig_id (स्थिरांक hb_glyph_info_t *info)
{
  परत info->lig_props() >> 5;
}

स्थिर ओळीत तार्किक
_hb_glyph_info_ligated_internal (स्थिरांक hb_glyph_info_t *info)
{
  परत info->lig_props() & IS_LIG_BASE;
}

स्थिर ओळीत अचिन्हित पूर्णांक
_hb_glyph_info_get_lig_comp (स्थिरांक hb_glyph_info_t *info)
{
  जर (_hb_glyph_info_ligated_internal (info))
    परत 0;
  अन्यथा
    परत info->lig_props() & 0x0F;
}

स्थिर ओळीत अचिन्हित पूर्णांक
hb_glyph_info_get_lig_अंक_comps (स्थिरांक hb_glyph_info_t *info)
{
  जर ((info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE) &&
      _hb_glyph_info_ligated_internal (info))
    परत info->lig_props() & 0x0F;
  अन्यथा
    परत 1;
}

स्थिर ओळीत uint8_t
_hb_allocate_lig_id (hb_buffer_t *buffer)
{
  uint8_t lig_id = buffer->next_serial () & 0x07;
  जर (unlikely (!lig_id))
    lig_id = _hb_allocate_lig_id (buffer); /* in case of overflow */
  परत lig_id;
}

/* glyph_props: */

स्थिर ओळीत शून्य
_hb_glyph_info_set_glyph_props (hb_glyph_info_t *info, अचिन्हित पूर्णांक props)
{
  info->glyph_props() = props;
}

स्थिर ओळीत अचिन्हित पूर्णांक
_hb_glyph_info_get_glyph_props (स्थिरांक hb_glyph_info_t *info)
{
  परत info->glyph_props();
}

स्थिर ओळीत तार्किक
_hb_glyph_info_is_base_glyph (स्थिरांक hb_glyph_info_t *info)
{
  परत info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH;
}

स्थिर ओळीत तार्किक
_hb_glyph_info_is_ligature (स्थिरांक hb_glyph_info_t *info)
{
  परत info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
}

स्थिर ओळीत तार्किक
_hb_glyph_info_is_mark (स्थिरांक hb_glyph_info_t *info)
{
  परत info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_MARK;
}

स्थिर ओळीत तार्किक
_hb_glyph_info_substituted (स्थिरांक hb_glyph_info_t *info)
{
  परत info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
}

स्थिर ओळीत तार्किक
_hb_glyph_info_ligated (स्थिरांक hb_glyph_info_t *info)
{
  परत info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_LIGATED;
}

स्थिर ओळीत तार्किक
_hb_glyph_info_multiplied (स्थिरांक hb_glyph_info_t *info)
{
  परत info->glyph_props() & HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
}

स्थिर ओळीत अचिन्हित पूर्णांक
hb_glyph_info_get_lig_अंक_comps_in_ligation (स्थिरांक hb_glyph_info_t *info)
{
  /* When a glyph is decomposed by a MultipleSubst and its pieces later become
   * components of a ligature, the pieces belong to the same ligature component
   * as the first piece, matching how MarkBasePos attaches marks only to the
   * first piece.  So the non-first pieces contribute no extra component.
   * https://github.com/harfbuzz/harfbuzz/issues/4969 */
  जर (_hb_glyph_info_multiplied (info) &&
      _hb_glyph_info_get_lig_comp (info))
    परत 0;
  परत hb_glyph_info_get_lig_अंक_comps (info);
}

स्थिर ओळीत तार्किक
_hb_glyph_info_ligated_and_didnt_multiply (स्थिरांक hb_glyph_info_t *info)
{
  परत _hb_glyph_info_ligated (info) && !_hb_glyph_info_multiplied (info);
}

स्थिर ओळीत शून्य
_hb_glyph_info_clear_ligated_and_multiplied (hb_glyph_info_t *info)
{
  info->glyph_props() &= ~(HB_OT_LAYOUT_GLYPH_PROPS_LIGATED |
			   HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED);
}

स्थिर ओळीत शून्य
_hb_glyph_info_clear_substituted (hb_glyph_info_t *info)
{
  info->glyph_props() &= ~(HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED);
}

स्थिर ओळीत तार्किक
_hb_clear_substitution_flags (स्थिरांक hb_ot_shape_plan_t *plan HB_UNUSED,
			      hb_font_t *font HB_UNUSED,
			      hb_buffer_t *buffer)
{
  hb_glyph_info_t *info = buffer->info;
  अचिन्हित पूर्णांक मोजणी = buffer->len;
  साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    _hb_glyph_info_clear_substituted (&info[i]);
  परत असत्य;
}


/* Allocation / deallocation. */

स्थिर ओळीत शून्य
_hb_buffer_allocate_unicode_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, unicode_props);
}

स्थिर ओळीत शून्य
_hb_buffer_deallocate_unicode_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_DEALLOCATE_VAR (buffer, unicode_props);
}

स्थिर ओळीत शून्य
_hb_buffer_assert_unicode_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_ASSERT_VAR (buffer, unicode_props);
}

स्थिर ओळीत शून्य
_hb_buffer_allocate_gsubgpos_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, glyph_props);
  HB_BUFFER_ALLOCATE_VAR (buffer, lig_props);
}

स्थिर ओळीत शून्य
_hb_buffer_deallocate_gsubgpos_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_DEALLOCATE_VAR (buffer, lig_props);
  HB_BUFFER_DEALLOCATE_VAR (buffer, glyph_props);
}

स्थिर ओळीत शून्य
_hb_buffer_assert_gsubgpos_vars (hb_buffer_t *buffer)
{
  HB_BUFFER_ASSERT_VAR (buffer, glyph_props);
  HB_BUFFER_ASSERT_VAR (buffer, lig_props);
}

/* Make sure no one directly touches our props... */
#undef unicode_props
#undef lig_props
#undef glyph_props

स्थिर ओळीत शून्य
_hb_collect_glyph_alternates_add (hb_codepoint_t from,
				  hb_codepoint_t to,
				  hb_map_t *alternate_मोजणी,
				  hb_map_t *alternate_glyphs)
{
  hb_codepoint_t zero = 0;
  hb_codepoint_t *i = &zero;
  alternate_मोजणी->has (from, &i);
  alternate_glyphs->set (from | (*i << 24), to);
  alternate_मोजणी->set (from, *i + 1);
}


#endif /* HB_OT_LAYOUT_HH */
