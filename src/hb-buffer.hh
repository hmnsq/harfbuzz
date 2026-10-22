/*
 * Copyright © 1998-2004  David Turner and Werner Lemberg
 * Copyright © 2004,2007,2009,2010  Red Hat, Inc.
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
 * Red Hat Author(s): Owen Taylor, Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਬਫਰ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਬਫਰ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-unicode.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set-digest.hh"


static_assert ((ਆਕਾਰ (hb_glyph_info_t) == 20), "");
static_assert ((ਆਕਾਰ (hb_glyph_info_t) == ਆਕਾਰ (hb_glyph_position_t)), "");

HB_MARK_AS_FLAG_T (hb_glyph_flags_t);
HB_MARK_AS_FLAG_T (hb_ਬਫਰ_flags_t);
HB_MARK_AS_FLAG_T (hb_ਬਫਰ_serialize_flags_t);
HB_MARK_AS_FLAG_T (hb_ਬਫਰ_diff_flags_t);

ਗਿਣਤੀ hb_ਬਫਰ_scratch_flags_t {
  HB_ਬਫਰ_SCRATCH_FLAG_DEFAULT			= 0x00000000u,
  HB_ਬਫਰ_SCRATCH_FLAG_HAS_FRACTION_SLASH		= 0x00000001u,
  HB_ਬਫਰ_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES		= 0x00000002u,
  HB_ਬਫਰ_SCRATCH_FLAG_HAS_SPACE_FALLBACK		= 0x00000004u,
  HB_ਬਫਰ_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT		= 0x00000008u,
  HB_ਬਫਰ_SCRATCH_FLAG_HAS_CGJ			= 0x00000010u,
  HB_ਬਫਰ_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE		= 0x00000020u,
  HB_ਬਫਰ_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK= 0x00000040u,
  HB_ਬਫਰ_SCRATCH_FLAG_HAS_CONTINUATIONS		= 0x00000080u,

  /* Reserved for shapers' internal use. */
  HB_ਬਫਰ_SCRATCH_FLAG_SHAPER0			= 0x01000000u,
  HB_ਬਫਰ_SCRATCH_FLAG_SHAPER1			= 0x02000000u,
  HB_ਬਫਰ_SCRATCH_FLAG_SHAPER2			= 0x04000000u,
  HB_ਬਫਰ_SCRATCH_FLAG_SHAPER3			= 0x08000000u,
};
HB_MARK_AS_FLAG_T (hb_ਬਫਰ_scratch_flags_t);


/*
 * hb_buffer_t
 */

ਬਣਤਰ hb_ਬਫਰ_t
{
  hb_object_ਸਿਰਲੇਖ_t ਸਿਰਲੇਖ;

  /*
   * Information about how the text in the buffer should be treated.
   */

  hb_unicode_funcs_t *unicode; /* Unicode functions */
  hb_ਬਫਰ_flags_t flags; /* BOT / EOT / etc. */
  hb_ਬਫਰ_cluster_level_t cluster_level;
  hb_codepoint_t replacement; /* U+FFFD or something else. */
  hb_codepoint_t invisible; /* 0 or something else. */
  hb_codepoint_t not_ਮਿਲਿਆ; /* 0 or something else. */
  hb_codepoint_t not_ਮਿਲਿਆ_variation_selector; /* HB_CODEPOINT_INVALID or something else. */

  /*
   * Buffer contents
   */

  hb_ਬਫਰ_content_type_t content_type;
  hb_segment_properties_t props; /* Script, language, direction */

  ਤਾਰਕਿਕ successful; /* Allocations successful */
  ਤਾਰਕਿਕ have_output; /* Whether we have an output buffer going on */
  ਤਾਰਕਿਕ have_positions; /* Whether we have positions */

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idx; /* Cursor into ->info and ->pos arrays */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len; /* Length of ->info and ->pos arrays */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਾਹਰਲਾ_len; /* Length of ->out_info array if have_output */

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *ਬਾਹਰਲਾ_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ CONTEXT_ਲੰਮਾਈ = 5u;
  hb_codepoint_t context[2][CONTEXT_ਲੰਮਾਈ];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ context_len[2];

  hb_ਸੈੱਟ_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_ਬਿੱਟ;
  uint8_t serial;
  uint32_t random_state;
  hb_ਬਫਰ_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ max_len; /* Maximum allowed len. */
  ਪੂਰਨ max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ਬਫਰ_ਸੁਨੇਹਾ
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*ਬਦਲਿਆ_func_t) (hb_ਬਫਰ_t *ਬਫਰ, ਖਾਲੀ *user_ਅੰਕੜੇ);

  hb_ਬਫਰ_ਸੁਨੇਹਾ_func_t ਸੁਨੇਹਾ_func;
  ਖਾਲੀ *ਸੁਨੇਹਾ_ਅੰਕੜੇ;
  hb_destroy_func_t ਸੁਨੇਹਾ_destroy;
  ਬਦਲਿਆ_func_t ਬਦਲਿਆ_func;
  ਖਾਲੀ *ਬਦਲਿਆ_ਅੰਕੜੇ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੁਨੇਹਾ_ਡੂੰਘਾਈ; /* How deeply are we inside a message callback? */
#ਨਹੀਂਤਾਂ
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੁਨੇਹਾ_ਡੂੰਘਾਈ = 0u;
#ਜੇ_ਅੰਤ



  /* Methods */

  HB_NODISCARD ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ { ਵਾਪਸ !successful; }

  ਖਾਲੀ allocate_var (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = ਸ਼ੁਰੂ + ਗਿਣਤੀ_2;
    ਪ੍ਰਤਿਜ੍ਞਾ (ਸਿਰਾ <= 8);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਿੱਟ = (1u<<ਸਿਰਾ) - (1u<<ਸ਼ੁਰੂ);
    ਪ੍ਰਤਿਜ੍ਞਾ (0 == (allocated_var_ਬਿੱਟ & ਬਿੱਟ));
    allocated_var_ਬਿੱਟ |= ਬਿੱਟ;
  }
  ਤਾਰਕਿਕ try_allocate_var (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = ਸ਼ੁਰੂ + ਗਿਣਤੀ_2;
    ਪ੍ਰਤਿਜ੍ਞਾ (ਸਿਰਾ <= 8);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਿੱਟ = (1u<<ਸਿਰਾ) - (1u<<ਸ਼ੁਰੂ);
    ਜੇ (allocated_var_ਬਿੱਟ & ਬਿੱਟ)
      ਵਾਪਸ ਝੂਠ;
    allocated_var_ਬਿੱਟ |= ਬਿੱਟ;
    ਵਾਪਸ ਸੱਚ;
  }
  ਖਾਲੀ deallocate_var (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = ਸ਼ੁਰੂ + ਗਿਣਤੀ_2;
    ਪ੍ਰਤਿਜ੍ਞਾ (ਸਿਰਾ <= 8);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਿੱਟ = (1u<<ਸਿਰਾ) - (1u<<ਸ਼ੁਰੂ);
    ਪ੍ਰਤਿਜ੍ਞਾ (ਬਿੱਟ == (allocated_var_ਬਿੱਟ & ਬਿੱਟ));
    allocated_var_ਬਿੱਟ &= ~ਬਿੱਟ;
  }
  ਖਾਲੀ assert_var (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = ਸ਼ੁਰੂ + ਗਿਣਤੀ_2;
    ਪ੍ਰਤਿਜ੍ਞਾ (ਸਿਰਾ <= 8);
    HB_ਅਣਵਰਤਿਆ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਿੱਟ = (1u<<ਸਿਰਾ) - (1u<<ਸ਼ੁਰੂ);
    ਪ੍ਰਤਿਜ੍ਞਾ (ਬਿੱਟ == (allocated_var_ਬਿੱਟ & ਬਿੱਟ));
  }
  ਖਾਲੀ deallocate_var_all ()
  {
    allocated_var_ਬਿੱਟ = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0) { ਵਾਪਸ info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0) ਅਚਲ { ਵਾਪਸ info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0) { ਵਾਪਸ pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0) ਅਚਲ { ਵਾਪਸ pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { ਵਾਪਸ ਬਾਹਰਲਾ_info[ਬਾਹਰਲਾ_len ? ਬਾਹਰਲਾ_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () ਅਚਲ { ਵਾਪਸ ਬਾਹਰਲਾ_info[ਬਾਹਰਲਾ_len ? ਬਾਹਰਲਾ_len - 1 : 0]; }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_codepoints (ਸੈੱਟ_t &d) ਅਚਲ
  { d.clear (); d.ਜੋੜੋ_array (&info[0].codepoint, len, ਆਕਾਰ (info[0])); }

  ਖਾਲੀ update_digest ()
  {
    digest = hb_ਸੈੱਟ_digest_t ();
    collect_codepoints (digest);
  }

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ similar (ਅਚਲ hb_ਬਫਰ_t &src);
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ reset ();
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ clear ();

  /* Called around shape() */
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ enter ();
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ leave ();

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ਬਫਰ_VERIFY
  HB_ਅੰਦਰੂਨੀ
#ਜੇ_ਅੰਤ
  ਤਾਰਕਿਕ verify (hb_ਬਫਰ_t        *ਪਾਠ_ਬਫਰ,
	       hb_font_t          *font,
	       ਅਚਲ hb_feature_t *features,
	       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features,
	       ਅਚਲ ਅੱਖਰ * ਅਚਲ *shapers)
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ਬਫਰ_VERIFY
  ;
#ਨਹੀਂਤਾਂ
  { ਵਾਪਸ ਸੱਚ; }
#ਜੇ_ਅੰਤ

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ backtrack_len () ਅਚਲ { ਵਾਪਸ have_output ? ਬਾਹਰਲਾ_len : idx; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookahead_len () ਅਚਲ { ਵਾਪਸ len - idx; }
  uint8_t ਅਗਲਾ_serial () { ਵਾਪਸ ++serial ? serial : ++serial; }

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ ਜੋੜੋ (hb_codepoint_t  codepoint,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    cluster);
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ ਜੋੜੋ_info (ਅਚਲ hb_glyph_info_t &glyph_info);
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ ਜੋੜੋ_info_and_pos (ਅਚਲ hb_glyph_info_t &glyph_info,
				     ਅਚਲ hb_glyph_position_t &glyph_pos);

  ਖਾਲੀ ਉਲਟ_range (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (ਸ਼ੁਰੂ, ਸਿਰਾ);
    ਜੇ (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (ਸ਼ੁਰੂ, ਸਿਰਾ);
  }
  ਖਾਲੀ reverse () { ਉਲਟ_range (0, len); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ FuncType>
  ਖਾਲੀ ਉਲਟ_groups (ਅਚਲ FuncType& ਸਮੂਹ,
		       ਤਾਰਕਿਕ ਮਿਲਾਓ_clusters = ਝੂਠ)
  {
    ਜੇ (unlikely (!len))
      ਵਾਪਸ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ i;
    ਲਈ (i = 1; i < len; i++)
    {
      ਜੇ (!ਸਮੂਹ (info[i - 1], info[i]))
      {
	ਜੇ (ਮਿਲਾਓ_clusters)
	  ਇਹ->ਮਿਲਾਓ_clusters (ਸ਼ੁਰੂ, i);
	ਉਲਟ_range (ਸ਼ੁਰੂ, i);
	ਸ਼ੁਰੂ = i;
      }
    }
    ਜੇ (ਮਿਲਾਓ_clusters)
      ਇਹ->ਮਿਲਾਓ_clusters (ਸ਼ੁਰੂ, i);
    ਉਲਟ_range (ਸ਼ੁਰੂ, i);

    reverse ();
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ FuncType>
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਮੂਹ_ਸਿਰਾ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਅਚਲ FuncType& ਸਮੂਹ) ਅਚਲ
  {
    ਜਦੋਂ (++ਸ਼ੁਰੂ < len && ਸਮੂਹ (info[ਸ਼ੁਰੂ - 1], info[ਸ਼ੁਰੂ]))
      ;

    ਵਾਪਸ ਸ਼ੁਰੂ;
  }

  ਸਥਿਰ ਤਾਰਕਿਕ cluster_ਸਮੂਹ_func (ਅਚਲ hb_glyph_info_t& a,
				   ਅਚਲ hb_glyph_info_t& b)
  { ਵਾਪਸ a.cluster == b.cluster; }

  ਖਾਲੀ ਉਲਟ_clusters () { ਉਲਟ_groups (cluster_ਸਮੂਹ_func); }

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ guess_segment_properties ();

  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ ਸਮਕਾਲੀ ();
  HB_ਅੰਦਰੂਨੀ ਪੂਰਨ sync_so_far ();
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ clear_output ();
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ clear_positions ();

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  HB_NODISCARD ਤਾਰਕਿਕ replace_glyphs (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_ਵਿੱਚ,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_ਬਾਹਰਲਾ,
				    ਅਚਲ T *glyph_ਅੰਕੜੇ)
  {
    ਜੇ (unlikely (!make_ਗੁੰਜਾਇਸ਼_for (ਅੰਕ_ਵਿੱਚ, ਅੰਕ_ਬਾਹਰਲਾ))) ਵਾਪਸ ਝੂਠ;

    ਪ੍ਰਤਿਜ੍ਞਾ (idx + ਅੰਕ_ਵਿੱਚ <= len);

    ਮਿਲਾਓ_clusters (idx, idx + ਅੰਕ_ਵਿੱਚ);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &ਬਾਹਰਲਾ_info[ਬਾਹਰਲਾ_len];
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_ਬਾਹਰਲਾ; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_ਅੰਕੜੇ[i];
      pinfo++;
    }

    idx  += ਅੰਕ_ਵਿੱਚ;
    ਬਾਹਰਲਾ_len += ਅੰਕ_ਬਾਹਰਲਾ;
    ਵਾਪਸ ਸੱਚ;
  }

  HB_NODISCARD ਤਾਰਕਿਕ replace_glyph (hb_codepoint_t glyph_ਸੂਚਕਾਂਕ)
  { ਵਾਪਸ replace_glyphs (1, 1, &glyph_ਸੂਚਕਾਂਕ); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD ਤਾਰਕਿਕ output_glyph (hb_codepoint_t glyph_ਸੂਚਕਾਂਕ)
  { ਵਾਪਸ replace_glyphs (0, 1, &glyph_ਸੂਚਕਾਂਕ); }

  HB_NODISCARD ਤਾਰਕਿਕ output_info (ਅਚਲ hb_glyph_info_t &glyph_info)
  {
    ਜੇ (unlikely (!make_ਗੁੰਜਾਇਸ਼_for (0, 1))) ਵਾਪਸ ਝੂਠ;

    ਬਾਹਰਲਾ_info[ਬਾਹਰਲਾ_len] = glyph_info;

    ਬਾਹਰਲਾ_len++;
    ਵਾਪਸ ਸੱਚ;
  }
  /* Copies glyph at idx to output but doesn't advance idx */
  HB_NODISCARD ਤਾਰਕਿਕ copy_glyph ()
  {
    /* Extra copy because cur()'s return can be freed within
     * output_info() call if buffer reallocates. */
    ਵਾਪਸ output_info (hb_glyph_info_t (cur()));
  }

  /* Copies glyph at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD ਤਾਰਕਿਕ ਅਗਲਾ_glyph ()
  {
    ਜੇ (have_output)
    {
      ਜੇ (ਬਾਹਰਲਾ_info != info || ਬਾਹਰਲਾ_len != idx)
      {
	ਜੇ (unlikely (!ensure (ਬਾਹਰਲਾ_len + 1))) ਵਾਪਸ ਝੂਠ;
	ਬਾਹਰਲਾ_info[ਬਾਹਰਲਾ_len] = info[idx];
      }
      ਬਾਹਰਲਾ_len++;
    }

    idx++;
    ਵਾਪਸ ਸੱਚ;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD ਤਾਰਕਿਕ ਅਗਲਾ_glyphs (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n)
  {
    ਜੇ (have_output)
    {
      ਜੇ (ਬਾਹਰਲਾ_info != info || ਬਾਹਰਲਾ_len != idx)
      {
	ਜੇ (unlikely (!ensure (ਬਾਹਰਲਾ_len + n))) ਵਾਪਸ ਝੂਠ;
	memmove (ਬਾਹਰਲਾ_info + ਬਾਹਰਲਾ_len, info + idx, n * ਆਕਾਰ (ਬਾਹਰਲਾ_info[0]));
      }
      ਬਾਹਰਲਾ_len += n;
    }

    idx += n;
    ਵਾਪਸ ਸੱਚ;
  }
  /* Advance idx without copying to output. */
  ਖਾਲੀ skip_glyph () { idx++; }
  ਖਾਲੀ ਜੋੜੋ_masks (hb_ਨਕਾਬ_t ਨਕਾਬ)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < len; j++)
      info[j].ਨਕਾਬ |= ਨਕਾਬ;
  }
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ ਸੈੱਟ_masks (hb_ਨਕਾਬ_t value, hb_ਨਕਾਬ_t ਨਕਾਬ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster_ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster_ਸਿਰਾ);

  ਖਾਲੀ ਮਿਲਾਓ_clusters (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
  {
    ਜੇ (ਸਿਰਾ - ਸ਼ੁਰੂ < 2)
      ਵਾਪਸ;
    ਜੇ (!HB_ਬਫਰ_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_ਮੰਜ਼ਿਲ_break (ਸ਼ੁਰੂ, ਸਿਰਾ);
      ਵਾਪਸ;
    }
    ਮਿਲਾਓ_clusters_impl (ਸ਼ੁਰੂ, ਸਿਰਾ);
  }
  ਖਾਲੀ ਮਿਲਾਓ_grapheme_clusters (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
  {
    ਜੇ (ਸਿਰਾ - ਸ਼ੁਰੂ < 2)
      ਵਾਪਸ;
    ਜੇ (!HB_ਬਫਰ_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_ਮੰਜ਼ਿਲ_break (ਸ਼ੁਰੂ, ਸਿਰਾ);
      ਵਾਪਸ;
    }
    ਮਿਲਾਓ_clusters_impl (ਸ਼ੁਰੂ, ਸਿਰਾ);
  }
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ ਮਿਲਾਓ_clusters_impl (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ);
  ਖਾਲੀ ਮਿਲਾਓ_ਬਾਹਰਲਾ_clusters (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
  {
    ਜੇ (ਸਿਰਾ - ਸ਼ੁਰੂ < 2)
      ਵਾਪਸ;
    ਜੇ (!HB_ਬਫਰ_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      ਵਾਪਸ;
    ਮਿਲਾਓ_ਬਾਹਰਲਾ_clusters_impl (ਸ਼ੁਰੂ, ਸਿਰਾ);
  }
  ਖਾਲੀ ਮਿਲਾਓ_ਬਾਹਰਲਾ_grapheme_clusters (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
  {
    ਜੇ (ਸਿਰਾ - ਸ਼ੁਰੂ < 2)
      ਵਾਪਸ;
    ਜੇ (!HB_ਬਫਰ_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      ਵਾਪਸ;
    ਮਿਲਾਓ_ਬਾਹਰਲਾ_clusters_impl (ਸ਼ੁਰੂ, ਸਿਰਾ);
  }
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ ਮਿਲਾਓ_ਬਾਹਰਲਾ_clusters_impl (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ delete_glyph ();
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ delete_glyphs_inplace (ਤਾਰਕਿਕ (*filter) (ਅਚਲ hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  ਖਾਲੀ ਸੈੱਟ_glyph_flags_impl (hb_ਨਕਾਬ_t ਨਕਾਬ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ,
			      ਤਾਰਕਿਕ interior,
			      ਤਾਰਕਿਕ from_ਬਾਹਰਲਾ_ਬਫਰ)
  {
    ਜੇ (!from_ਬਾਹਰਲਾ_ਬਫਰ || !have_output)
    {
      ਜੇ (!interior)
      {
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
	  info[i].ਨਕਾਬ |= ਨਕਾਬ;
      }
      ਨਹੀਂਤਾਂ
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ cluster = infos_ਲੱਭੋ_min_cluster (info, ਸ਼ੁਰੂ, ਸਿਰਾ);
	infos_ਸੈੱਟ_glyph_flags (info, ਸ਼ੁਰੂ, ਸਿਰਾ, cluster, ਨਕਾਬ);
      }
    }
    ਨਹੀਂਤਾਂ
    {
      ਪ੍ਰਤਿਜ੍ਞਾ (ਸ਼ੁਰੂ <= ਬਾਹਰਲਾ_len);
      ਪ੍ਰਤਿਜ੍ਞਾ (idx <= ਸਿਰਾ);

      ਜੇ (!interior)
      {
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਸ਼ੁਰੂ; i < ਬਾਹਰਲਾ_len; i++)
	  ਬਾਹਰਲਾ_info[i].ਨਕਾਬ |= ਨਕਾਬ;
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = idx; i < ਸਿਰਾ; i++)
	  info[i].ਨਕਾਬ |= ਨਕਾਬ;
      }
      ਨਹੀਂਤਾਂ
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ cluster = infos_ਲੱਭੋ_min_cluster (info, idx, ਸਿਰਾ);
	cluster = infos_ਲੱਭੋ_min_cluster (ਬਾਹਰਲਾ_info, ਸ਼ੁਰੂ, ਬਾਹਰਲਾ_len, cluster);

	infos_ਸੈੱਟ_glyph_flags (ਬਾਹਰਲਾ_info, ਸ਼ੁਰੂ, ਬਾਹਰਲਾ_len, cluster, ਨਕਾਬ);
	infos_ਸੈੱਟ_glyph_flags (info, idx, ਸਿਰਾ, cluster, ਨਕਾਬ);
      }
    }
  }

  HB_ALWAYS_INLINE
  ਖਾਲੀ ਸੈੱਟ_glyph_flags (hb_ਨਕਾਬ_t ਨਕਾਬ,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = 0,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ = (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1,
			 ਤਾਰਕਿਕ interior = ਝੂਠ,
			 ਤਾਰਕਿਕ from_ਬਾਹਰਲਾ_ਬਫਰ = ਝੂਠ)
  {
    ਜੇ (unlikely (ਸਿਰਾ != (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1 && ਸਿਰਾ - ਸ਼ੁਰੂ > 255))
      ਵਾਪਸ;

    ਸਿਰਾ = hb_min (ਸਿਰਾ, len);

    ਜੇ (interior && !from_ਬਾਹਰਲਾ_ਬਫਰ && ਸਿਰਾ - ਸ਼ੁਰੂ < 2)
      ਵਾਪਸ;

    ਸੈੱਟ_glyph_flags_impl (ਨਕਾਬ, ਸ਼ੁਰੂ, ਸਿਰਾ, interior, from_ਬਾਹਰਲਾ_ਬਫਰ);
  }


  ਖਾਲੀ unsafe_ਮੰਜ਼ਿਲ_break (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = 0, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = -1)
  {
    ਸੈੱਟ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_ਮੰਜ਼ਿਲ_BREAK | HB_GLYPH_FLAG_UNSAFE_ਮੰਜ਼ਿਲ_CONCAT,
		      ਸ਼ੁਰੂ, ਸਿਰਾ,
		      ਸੱਚ);
  }
  ਖਾਲੀ ਸੁਰੱਖਿਅਤ_ਮੰਜ਼ਿਲ_ਪਾਓ_tatweel (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = 0, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = -1)
  {
    ਜੇ ((flags & HB_ਬਫਰ_FLAG_PRODUCE_ਸੁਰੱਖਿਅਤ_ਮੰਜ਼ਿਲ_ਪਾਓ_TATWEEL) == 0)
    {
      unsafe_ਮੰਜ਼ਿਲ_break (ਸ਼ੁਰੂ, ਸਿਰਾ);
      ਵਾਪਸ;
    }
    ਸੈੱਟ_glyph_flags (HB_GLYPH_FLAG_ਸੁਰੱਖਿਅਤ_ਮੰਜ਼ਿਲ_ਪਾਓ_TATWEEL,
		      ਸ਼ੁਰੂ, ਸਿਰਾ,
		      ਸੱਚ);
  }
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  ਖਾਲੀ unsafe_ਮੰਜ਼ਿਲ_concat (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = 0, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = -1)
  {
    ਜੇ (likely ((flags & HB_ਬਫਰ_FLAG_PRODUCE_UNSAFE_ਮੰਜ਼ਿਲ_CONCAT) == 0))
      ਵਾਪਸ;
    ਸੈੱਟ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_ਮੰਜ਼ਿਲ_CONCAT,
		      ਸ਼ੁਰੂ, ਸਿਰਾ,
		      ਝੂਠ);
  }
  ਖਾਲੀ unsafe_ਮੰਜ਼ਿਲ_break_from_outbuffer (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = 0, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = -1)
  {
    ਸੈੱਟ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_ਮੰਜ਼ਿਲ_BREAK | HB_GLYPH_FLAG_UNSAFE_ਮੰਜ਼ਿਲ_CONCAT,
		      ਸ਼ੁਰੂ, ਸਿਰਾ,
		      ਸੱਚ, ਸੱਚ);
  }
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  ਖਾਲੀ unsafe_ਮੰਜ਼ਿਲ_concat_from_outbuffer (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = 0, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = -1)
  {
    ਜੇ (likely ((flags & HB_ਬਫਰ_FLAG_PRODUCE_UNSAFE_ਮੰਜ਼ਿਲ_CONCAT) == 0))
      ਵਾਪਸ;
    ਸੈੱਟ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_ਮੰਜ਼ਿਲ_CONCAT,
		      ਸ਼ੁਰੂ, ਸਿਰਾ,
		      ਝੂਠ, ਸੱਚ);
  }


  /* Internal methods */
  HB_NODISCARD HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ move_ਮੰਜ਼ਿਲ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i); /* i is output-buffer index. */

  HB_NODISCARD HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ enlarge (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ);

  HB_NODISCARD ਤਾਰਕਿਕ resize (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ)
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (!have_output);
    ਜੇ (unlikely (!ensure (ਲੰਮਾਈ))) ਵਾਪਸ ਝੂਠ;
    len = ਲੰਮਾਈ;
    ਵਾਪਸ ਸੱਚ;
  }
  HB_NODISCARD ਤਾਰਕਿਕ ensure (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ)
  { ਵਾਪਸ likely (!ਮਾਪ || ਮਾਪ < allocated) ? ਸੱਚ : enlarge (ਮਾਪ); }

  HB_NODISCARD ਤਾਰਕਿਕ ensure_inplace (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ)
  { ਵਾਪਸ likely (!ਮਾਪ || ਮਾਪ < allocated); }

  ਖਾਲੀ assert_glyphs ()
  {
    ਪ੍ਰਤਿਜ੍ਞਾ ((content_type == HB_ਬਫਰ_CONTENT_TYPE_GLYPHS) ||
	    (!len && (content_type == HB_ਬਫਰ_CONTENT_TYPE_INVALID)));
  }
  ਖਾਲੀ assert_unicode ()
  {
    ਪ੍ਰਤਿਜ੍ਞਾ ((content_type == HB_ਬਫਰ_CONTENT_TYPE_UNICODE) ||
	    (!len && (content_type == HB_ਬਫਰ_CONTENT_TYPE_INVALID)));
  }
  HB_NODISCARD ਤਾਰਕਿਕ ensure_glyphs ()
  {
    ਜੇ (unlikely (content_type != HB_ਬਫਰ_CONTENT_TYPE_GLYPHS))
    {
      ਜੇ (content_type != HB_ਬਫਰ_CONTENT_TYPE_INVALID)
	ਵਾਪਸ ਝੂਠ;
      ਪ੍ਰਤਿਜ੍ਞਾ (len == 0);
      content_type = HB_ਬਫਰ_CONTENT_TYPE_GLYPHS;
    }
    ਵਾਪਸ ਸੱਚ;
  }
  HB_NODISCARD ਤਾਰਕਿਕ ensure_unicode ()
  {
    ਜੇ (unlikely (content_type != HB_ਬਫਰ_CONTENT_TYPE_UNICODE))
    {
      ਜੇ (content_type != HB_ਬਫਰ_CONTENT_TYPE_INVALID)
	ਵਾਪਸ ਝੂਠ;
      ਪ੍ਰਤਿਜ੍ਞਾ (len == 0);
      content_type = HB_ਬਫਰ_CONTENT_TYPE_UNICODE;
    }
    ਵਾਪਸ ਸੱਚ;
  }

  HB_NODISCARD HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ make_ਗੁੰਜਾਇਸ਼_for (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_ਵਿੱਚ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_ਬਾਹਰਲਾ);
  HB_NODISCARD HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ ਖਿਸਕਾਓ_forward (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2);

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਲੰਮਾ scratch_ਬਫਰ_t;
  HB_ਅੰਦਰੂਨੀ scratch_ਬਫਰ_t *get_scratch_ਬਫਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਮਾਪ);

  ਖਾਲੀ clear_context (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ side) { context_len[side] = 0; }

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ sort (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ, ਪੂਰਨ(*compar)(ਅਚਲ hb_glyph_info_t *, ਅਚਲ hb_glyph_info_t *));

  ਤਾਰਕਿਕ messaging ()
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_ਬਫਰ_ਸੁਨੇਹਾ
    ਵਾਪਸ ਝੂਠ;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ unlikely (ਸੁਨੇਹਾ_func);
#ਜੇ_ਅੰਤ
  }
  ਤਾਰਕਿਕ ਸੁਨੇਹਾ (hb_font_t *font, ਅਚਲ ਅੱਖਰ *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_ਬਫਰ_ਸੁਨੇਹਾ
    ਵਾਪਸ ਸੱਚ;
#ਨਹੀਂਤਾਂ
    ਜੇ (likely (!messaging ()))
      ਵਾਪਸ ਸੱਚ;

    va_list ap;
    va_ਸ਼ੁਰੂ (ap, fmt);
    ਤਾਰਕਿਕ ret = ਸੁਨੇਹਾ_impl (font, fmt, ap);
    va_ਸਿਰਾ (ap);

    ਵਾਪਸ ret;
#ਜੇ_ਅੰਤ
  }
  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ ਸੁਨੇਹਾ_impl (hb_font_t *font, ਅਚਲ ਅੱਖਰ *fmt, va_list ap) HB_PRINTF_FUNC(3, 0);
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ ਬਦਲਿਆ ();

  ਸਥਿਰ ਖਾਲੀ
  ਸੈੱਟ_cluster (hb_glyph_info_t &inf, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਕਾਬ = 0)
  {
    ਜੇ (inf.cluster != cluster)
      inf.ਨਕਾਬ = (inf.ਨਕਾਬ & ~HB_GLYPH_FLAG_DEFINED) | (ਨਕਾਬ & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  ਖਾਲੀ
  infos_ਸੈੱਟ_glyph_flags (hb_glyph_info_t *infos,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster,
			  hb_ਨਕਾਬ_t ਨਕਾਬ)
  {
    ਜੇ (unlikely (ਸ਼ੁਰੂ == ਸਿਰਾ))
      ਵਾਪਸ;

    max_ops -= ਸਿਰਾ - ਸ਼ੁਰੂ;
    ਜੇ (unlikely (max_ops < 0))
      successful = ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ cluster_ਪਹਿਲਾ = infos[ਸ਼ੁਰੂ].cluster;
    ਬਿਨਾਨਿਸ਼ਾਨੀ cluster_ਆਖਰੀ = infos[ਸਿਰਾ - 1].cluster;

    ਜੇ (cluster_level == HB_ਬਫਰ_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_ਪਹਿਲਾ && cluster != cluster_ਆਖਰੀ))
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
	ਜੇ (cluster != infos[i].cluster)
	  infos[i].ਨਕਾਬ |= ਨਕਾਬ;
      ਵਾਪਸ;
    }

    /* Monotone clusters */

    ਜੇ (cluster == cluster_ਪਹਿਲਾ)
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸਿਰਾ; ਸ਼ੁਰੂ < i && infos[i - 1].cluster != cluster_ਪਹਿਲਾ; i--)
	infos[i - 1].ਨਕਾਬ |= ਨਕਾਬ;
    }
    ਨਹੀਂਤਾਂ /* cluster == cluster_last */
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ && infos[i].cluster != cluster_ਆਖਰੀ; i++)
	infos[i].ਨਕਾਬ |= ਨਕਾਬ;
    }
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ
  infos_ਲੱਭੋ_min_cluster (ਅਚਲ hb_glyph_info_t *infos,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ cluster = UINT_MAX)
  {
    ਜੇ (unlikely (ਸ਼ੁਰੂ == ਸਿਰਾ))
      ਵਾਪਸ cluster;

    ਜੇ (cluster_level == HB_ਬਫਰ_CLUSTER_LEVEL_CHARACTERS)
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      ਵਾਪਸ cluster;
    }

    ਵਾਪਸ hb_min (cluster, hb_min (infos[ਸ਼ੁਰੂ].cluster, infos[ਸਿਰਾ - 1].cluster));
  }

  ਖਾਲੀ clear_glyph_flags (hb_ਨਕਾਬ_t ਨਕਾਬ = 0)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < len; i++)
      info[i].ਨਕਾਬ = (info[i].ਨਕਾਬ & ~HB_GLYPH_FLAG_DEFINED) | (ਨਕਾਬ & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_ਬਫਰ_t);


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ foreach_ਸਮੂਹ(ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ, ਸਮੂਹ_func) \
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ \
       ਗਿਣਤੀ_3 = ਬਫਰ->len, \
       ਸ਼ੁਰੂ = 0, ਸਿਰਾ = ਗਿਣਤੀ_3 ? ਬਫਰ->ਸਮੂਹ_ਸਿਰਾ (0, ਸਮੂਹ_func) : 0; \
       ਸ਼ੁਰੂ < ਗਿਣਤੀ_3; \
       ਸ਼ੁਰੂ = ਸਿਰਾ, ਸਿਰਾ = ਬਫਰ->ਸਮੂਹ_ਸਿਰਾ (ਸ਼ੁਰੂ, ਸਮੂਹ_func))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ foreach_cluster(ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ) \
	foreach_ਸਮੂਹ (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ, hb_ਬਫਰ_t::cluster_ਸਮੂਹ_func)


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਬਫਰ_XALLOCATE_VAR(b, func, var) \
  b->func (offsetof (hb_glyph_info_t, var) - offsetof(hb_glyph_info_t, var1), \
	   ਆਕਾਰ (b->info[0].var))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਬਫਰ_ALLOCATE_VAR(b, var)		HB_ਬਫਰ_XALLOCATE_VAR (b, allocate_var,     var ())
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਬਫਰ_TRY_ALLOCATE_VAR(b, var)	HB_ਬਫਰ_XALLOCATE_VAR (b, try_allocate_var, var ())
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਬਫਰ_DEALLOCATE_VAR(b, var)	HB_ਬਫਰ_XALLOCATE_VAR (b, deallocate_var,   var ())
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਬਫਰ_ASSERT_VAR(b, var)		HB_ਬਫਰ_XALLOCATE_VAR (b, assert_var,       var ())


#ਜੇ_ਅੰਤ /* HB_BUFFER_HH */
