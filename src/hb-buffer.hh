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

#ifndef HB_BUFFER_HH
#define HB_BUFFER_HH

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-unicode.hh"
#ಅನ್ತರ್ಭಾವ "hb-set-digest.hh"


static_assert ((ಪರಿಮಾಣ (hb_glyph_info_t) == 20), "");
static_assert ((ಪರಿಮಾಣ (hb_glyph_info_t) == ಪರಿಮಾಣ (hb_glyph_position_t)), "");

HB_MARK_AS_FLAG_T (hb_glyph_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_serialize_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_diff_flags_t);

ಗಣನಾ hb_buffer_scratch_flags_t {
  HB_BUFFER_SCRATCH_FLAG_DEFAULT			= 0x00000000u,
  HB_BUFFER_SCRATCH_FLAG_HAS_FRACTION_SLASH		= 0x00000001u,
  HB_BUFFER_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES		= 0x00000002u,
  HB_BUFFER_SCRATCH_FLAG_HAS_SPACE_FALLBACK		= 0x00000004u,
  HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT		= 0x00000008u,
  HB_BUFFER_SCRATCH_FLAG_HAS_CGJ			= 0x00000010u,
  HB_BUFFER_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE		= 0x00000020u,
  HB_BUFFER_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK= 0x00000040u,
  HB_BUFFER_SCRATCH_FLAG_HAS_CONTINUATIONS		= 0x00000080u,

  /* Reserved for shapers' internal use. */
  HB_BUFFER_SCRATCH_FLAG_SHAPER0			= 0x01000000u,
  HB_BUFFER_SCRATCH_FLAG_SHAPER1			= 0x02000000u,
  HB_BUFFER_SCRATCH_FLAG_SHAPER2			= 0x04000000u,
  HB_BUFFER_SCRATCH_FLAG_SHAPER3			= 0x08000000u,
};
HB_MARK_AS_FLAG_T (hb_buffer_scratch_flags_t);


/*
 * hb_buffer_t
 */

ಸಂರಚನಾ hb_buffer_t
{
  hb_object_header_t header;

  /*
   * Information about how the text in the buffer should be treated.
   */

  hb_unicode_funcs_t *unicode; /* Unicode functions */
  hb_buffer_flags_t flags; /* BOT / EOT / etc. */
  hb_buffer_cluster_level_t cluster_level;
  hb_codepoint_t replacement; /* U+FFFD or something else. */
  hb_codepoint_t invisible; /* 0 or something else. */
  hb_codepoint_t not_found; /* 0 or something else. */
  hb_codepoint_t not_found_variation_selector; /* HB_CODEPOINT_INVALID or something else. */

  /*
   * Buffer contents
   */

  hb_buffer_content_type_t content_type;
  hb_segment_properties_t props; /* Script, language, direction */

  ತಾರ್ಕಿಕ successful; /* Allocations successful */
  ತಾರ್ಕಿಕ have_output; /* Whether we have an output buffer going on */
  ತಾರ್ಕಿಕ have_positions; /* Whether we have positions */

  unsigned ಪೂರ್ಣಾಙ್ಕ idx; /* Cursor into ->info and ->pos arrays */
  unsigned ಪೂರ್ಣಾಙ್ಕ len; /* Length of ->info and ->pos arrays */
  unsigned ಪೂರ್ಣಾಙ್ಕ out_len; /* Length of ->out_info array if have_output */

  unsigned ಪೂರ್ಣಾಙ್ಕ allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *out_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  ಸ್ಥಿರ constexpr unsigned CONTEXT_LENGTH = 5u;
  hb_codepoint_t context[2][CONTEXT_LENGTH];
  unsigned ಪೂರ್ಣಾಙ್ಕ context_len[2];

  hb_set_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_bits;
  uint8_t serial;
  uint32_t random_state;
  hb_buffer_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  unsigned ಪೂರ್ಣಾಙ್ಕ max_len; /* Maximum allowed len. */
  ಪೂರ್ಣಾಙ್ಕ max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#ifndef HB_NO_BUFFER_MESSAGE
  ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*changed_func_t) (hb_buffer_t *buffer, ಶೂನ್ಯ *user_data);

  hb_buffer_message_func_t message_func;
  ಶೂನ್ಯ *message_data;
  hb_destroy_func_t message_destroy;
  changed_func_t changed_func;
  ಶೂನ್ಯ *changed_data;
  unsigned message_depth; /* How deeply are we inside a message callback? */
#ಅನ್ಯಥಾ
  ಸ್ಥಿರ constexpr unsigned message_depth = 0u;
#endif



  /* Methods */

  HB_NODISCARD ತಾರ್ಕಿಕ in_error () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !successful; }

  ಶೂನ್ಯ allocate_var (unsigned ಪೂರ್ಣಾಙ್ಕ start, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ end = start + ಎಣಿಕೆ;
    ಪ್ರತಿಜ್ಞಾ (end <= 8);
    unsigned ಪೂರ್ಣಾಙ್ಕ bits = (1u<<end) - (1u<<start);
    ಪ್ರತಿಜ್ಞಾ (0 == (allocated_var_bits & bits));
    allocated_var_bits |= bits;
  }
  ತಾರ್ಕಿಕ try_allocate_var (unsigned ಪೂರ್ಣಾಙ್ಕ start, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ end = start + ಎಣಿಕೆ;
    ಪ್ರತಿಜ್ಞಾ (end <= 8);
    unsigned ಪೂರ್ಣಾಙ್ಕ bits = (1u<<end) - (1u<<start);
    ಯದಿ (allocated_var_bits & bits)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    allocated_var_bits |= bits;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  ಶೂನ್ಯ deallocate_var (unsigned ಪೂರ್ಣಾಙ್ಕ start, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ end = start + ಎಣಿಕೆ;
    ಪ್ರತಿಜ್ಞಾ (end <= 8);
    unsigned ಪೂರ್ಣಾಙ್ಕ bits = (1u<<end) - (1u<<start);
    ಪ್ರತಿಜ್ಞಾ (bits == (allocated_var_bits & bits));
    allocated_var_bits &= ~bits;
  }
  ಶೂನ್ಯ assert_var (unsigned ಪೂರ್ಣಾಙ್ಕ start, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ end = start + ಎಣಿಕೆ;
    ಪ್ರತಿಜ್ಞಾ (end <= 8);
    HB_UNUSED unsigned ಪೂರ್ಣಾಙ್ಕ bits = (1u<<end) - (1u<<start);
    ಪ್ರತಿಜ್ಞಾ (bits == (allocated_var_bits & bits));
  }
  ಶೂನ್ಯ deallocate_var_all ()
  {
    allocated_var_bits = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0) { ಪ್ರತ್ಯಾಗಮ info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0) { ಪ್ರತ್ಯಾಗಮ pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { ಪ್ರತ್ಯಾಗಮ out_info[out_len ? out_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ out_info[out_len ? out_len - 1 : 0]; }

  template <typename set_t>
  ಶೂನ್ಯ collect_codepoints (set_t &d) ಧ್ರುವ
  { d.clear (); d.add_array (&info[0].codepoint, len, ಪರಿಮಾಣ (info[0])); }

  ಶೂನ್ಯ update_digest ()
  {
    digest = hb_set_digest_t ();
    collect_codepoints (digest);
  }

  HB_INTERNAL ಶೂನ್ಯ similar (ಧ್ರುವ hb_buffer_t &src);
  HB_INTERNAL ಶೂನ್ಯ reset ();
  HB_INTERNAL ಶೂನ್ಯ clear ();

  /* Called around shape() */
  HB_INTERNAL ಶೂನ್ಯ enter ();
  HB_INTERNAL ಶೂನ್ಯ leave ();

#ifndef HB_NO_BUFFER_VERIFY
  HB_INTERNAL
#endif
  ತಾರ್ಕಿಕ verify (hb_buffer_t        *text_buffer,
	       hb_font_t          *font,
	       ಧ್ರುವ hb_feature_t *features,
	       unsigned ಪೂರ್ಣಾಙ್ಕ        ಅಂಕೆ_features,
	       ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ *shapers)
#ifndef HB_NO_BUFFER_VERIFY
  ;
#ಅನ್ಯಥಾ
  { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }
#endif

  unsigned ಪೂರ್ಣಾಙ್ಕ backtrack_len () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ have_output ? out_len : idx; }
  unsigned ಪೂರ್ಣಾಙ್ಕ lookahead_len () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ len - idx; }
  uint8_t next_serial () { ಪ್ರತ್ಯಾಗಮ ++serial ? serial : ++serial; }

  HB_INTERNAL ಶೂನ್ಯ add (hb_codepoint_t  codepoint,
			unsigned ಪೂರ್ಣಾಙ್ಕ    cluster);
  HB_INTERNAL ಶೂನ್ಯ add_info (ಧ್ರುವ hb_glyph_info_t &glyph_info);
  HB_INTERNAL ಶೂನ್ಯ add_info_and_pos (ಧ್ರುವ hb_glyph_info_t &glyph_info,
				     ಧ್ರುವ hb_glyph_position_t &glyph_pos);

  ಶೂನ್ಯ reverse_range (unsigned start, unsigned end)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (start, end);
    ಯದಿ (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (start, end);
  }
  ಶೂನ್ಯ reverse () { reverse_range (0, len); }

  template <typename FuncType>
  ಶೂನ್ಯ reverse_groups (ಧ್ರುವ FuncType& group,
		       ತಾರ್ಕಿಕ merge_clusters = ಅಸತ್ಯ)
  {
    ಯದಿ (unlikely (!len))
      ಪ್ರತ್ಯಾಗಮ;

    unsigned start = 0;
    unsigned i;
    ಪ್ರತಿ (i = 1; i < len; i++)
    {
      ಯದಿ (!group (info[i - 1], info[i]))
      {
	ಯದಿ (merge_clusters)
	  ಏತತ್->merge_clusters (start, i);
	reverse_range (start, i);
	start = i;
      }
    }
    ಯದಿ (merge_clusters)
      ಏತತ್->merge_clusters (start, i);
    reverse_range (start, i);

    reverse ();
  }

  template <typename FuncType>
  unsigned group_end (unsigned start, ಧ್ರುವ FuncType& group) ಧ್ರುವ
  {
    ಯಾವತ್ (++start < len && group (info[start - 1], info[start]))
      ;

    ಪ್ರತ್ಯಾಗಮ start;
  }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ _cluster_group_func (ಧ್ರುವ hb_glyph_info_t& a,
				   ಧ್ರುವ hb_glyph_info_t& b)
  { ಪ್ರತ್ಯಾಗಮ a.cluster == b.cluster; }

  ಶೂನ್ಯ reverse_clusters () { reverse_groups (_cluster_group_func); }

  HB_INTERNAL ಶೂನ್ಯ guess_segment_properties ();

  HB_INTERNAL ತಾರ್ಕಿಕ ಸಮಕಾಲ ();
  HB_INTERNAL ಪೂರ್ಣಾಙ್ಕ sync_so_far ();
  HB_INTERNAL ಶೂನ್ಯ clear_output ();
  HB_INTERNAL ಶೂನ್ಯ clear_positions ();

  template <typename T>
  HB_NODISCARD ತಾರ್ಕಿಕ replace_glyphs (unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_in,
				    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_out,
				    ಧ್ರುವ T *glyph_data)
  {
    ಯದಿ (unlikely (!make_room_for (ಅಂಕೆ_in, ಅಂಕೆ_out))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿಜ್ಞಾ (idx + ಅಂಕೆ_in <= len);

    merge_clusters (idx, idx + ಅಂಕೆ_in);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &out_info[out_len];
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_out; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_data[i];
      pinfo++;
    }

    idx  += ಅಂಕೆ_in;
    out_len += ಅಂಕೆ_out;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  HB_NODISCARD ತಾರ್ಕಿಕ replace_glyph (hb_codepoint_t glyph_index)
  { ಪ್ರತ್ಯಾಗಮ replace_glyphs (1, 1, &glyph_index); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD ತಾರ್ಕಿಕ output_glyph (hb_codepoint_t glyph_index)
  { ಪ್ರತ್ಯಾಗಮ replace_glyphs (0, 1, &glyph_index); }

  HB_NODISCARD ತಾರ್ಕಿಕ output_info (ಧ್ರುವ hb_glyph_info_t &glyph_info)
  {
    ಯದಿ (unlikely (!make_room_for (0, 1))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    out_info[out_len] = glyph_info;

    out_len++;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  /* Copies glyph at idx to output but doesn't advance idx */
  HB_NODISCARD ತಾರ್ಕಿಕ copy_glyph ()
  {
    /* Extra copy because cur()'s return can be freed within
     * output_info() call if buffer reallocates. */
    ಪ್ರತ್ಯಾಗಮ output_info (hb_glyph_info_t (cur()));
  }

  /* Copies glyph at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD ತಾರ್ಕಿಕ next_glyph ()
  {
    ಯದಿ (have_output)
    {
      ಯದಿ (out_info != info || out_len != idx)
      {
	ಯದಿ (unlikely (!ensure (out_len + 1))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	out_info[out_len] = info[idx];
      }
      out_len++;
    }

    idx++;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD ತಾರ್ಕಿಕ next_glyphs (unsigned ಪೂರ್ಣಾಙ್ಕ n)
  {
    ಯದಿ (have_output)
    {
      ಯದಿ (out_info != info || out_len != idx)
      {
	ಯದಿ (unlikely (!ensure (out_len + n))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	memmove (out_info + out_len, info + idx, n * ಪರಿಮಾಣ (out_info[0]));
      }
      out_len += n;
    }

    idx += n;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  /* Advance idx without copying to output. */
  ಶೂನ್ಯ skip_glyph () { idx++; }
  ಶೂನ್ಯ add_masks (hb_mask_t mask)
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = 0; j < len; j++)
      info[j].mask |= mask;
  }
  HB_INTERNAL ಶೂನ್ಯ set_masks (hb_mask_t value, hb_mask_t mask,
			      unsigned ಪೂರ್ಣಾಙ್ಕ cluster_start, unsigned ಪೂರ್ಣಾಙ್ಕ cluster_end);

  ಶೂನ್ಯ merge_clusters (unsigned ಪೂರ್ಣಾಙ್ಕ start, unsigned ಪೂರ್ಣಾಙ್ಕ end)
  {
    ಯದಿ (end - start < 2)
      ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_to_break (start, end);
      ಪ್ರತ್ಯಾಗಮ;
    }
    merge_clusters_impl (start, end);
  }
  ಶೂನ್ಯ merge_grapheme_clusters (unsigned ಪೂರ್ಣಾಙ್ಕ start, unsigned ಪೂರ್ಣಾಙ್ಕ end)
  {
    ಯದಿ (end - start < 2)
      ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_to_break (start, end);
      ಪ್ರತ್ಯಾಗಮ;
    }
    merge_clusters_impl (start, end);
  }
  HB_INTERNAL ಶೂನ್ಯ merge_clusters_impl (unsigned ಪೂರ್ಣಾಙ್ಕ start, unsigned ಪೂರ್ಣಾಙ್ಕ end);
  ಶೂನ್ಯ merge_out_clusters (unsigned ಪೂರ್ಣಾಙ್ಕ start, unsigned ಪೂರ್ಣಾಙ್ಕ end)
  {
    ಯದಿ (end - start < 2)
      ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      ಪ್ರತ್ಯಾಗಮ;
    merge_out_clusters_impl (start, end);
  }
  ಶೂನ್ಯ merge_out_grapheme_clusters (unsigned ಪೂರ್ಣಾಙ್ಕ start, unsigned ಪೂರ್ಣಾಙ್ಕ end)
  {
    ಯದಿ (end - start < 2)
      ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      ಪ್ರತ್ಯಾಗಮ;
    merge_out_clusters_impl (start, end);
  }
  HB_INTERNAL ಶೂನ್ಯ merge_out_clusters_impl (unsigned ಪೂರ್ಣಾಙ್ಕ start, unsigned ಪೂರ್ಣಾಙ್ಕ end);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_INTERNAL ಶೂನ್ಯ delete_glyph ();
  HB_INTERNAL ಶೂನ್ಯ delete_glyphs_inplace (ತಾರ್ಕಿಕ (*filter) (ಧ್ರುವ hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  ಶೂನ್ಯ _set_glyph_flags_impl (hb_mask_t mask,
			      unsigned start,
			      unsigned end,
			      ತಾರ್ಕಿಕ interior,
			      ತಾರ್ಕಿಕ from_out_buffer)
  {
    ಯದಿ (!from_out_buffer || !have_output)
    {
      ಯದಿ (!interior)
      {
	ಪ್ರತಿ (unsigned i = start; i < end; i++)
	  info[i].mask |= mask;
      }
      ಅನ್ಯಥಾ
      {
	unsigned cluster = _infos_find_min_cluster (info, start, end);
	_infos_set_glyph_flags (info, start, end, cluster, mask);
      }
    }
    ಅನ್ಯಥಾ
    {
      ಪ್ರತಿಜ್ಞಾ (start <= out_len);
      ಪ್ರತಿಜ್ಞಾ (idx <= end);

      ಯದಿ (!interior)
      {
	ಪ್ರತಿ (unsigned i = start; i < out_len; i++)
	  out_info[i].mask |= mask;
	ಪ್ರತಿ (unsigned i = idx; i < end; i++)
	  info[i].mask |= mask;
      }
      ಅನ್ಯಥಾ
      {
	unsigned cluster = _infos_find_min_cluster (info, idx, end);
	cluster = _infos_find_min_cluster (out_info, start, out_len, cluster);

	_infos_set_glyph_flags (out_info, start, out_len, cluster, mask);
	_infos_set_glyph_flags (info, idx, end, cluster, mask);
      }
    }
  }

  HB_ALWAYS_INLINE
  ಶೂನ್ಯ _set_glyph_flags (hb_mask_t mask,
			 unsigned start = 0,
			 unsigned end = (unsigned) -1,
			 ತಾರ್ಕಿಕ interior = ಅಸತ್ಯ,
			 ತಾರ್ಕಿಕ from_out_buffer = ಅಸತ್ಯ)
  {
    ಯದಿ (unlikely (end != (unsigned) -1 && end - start > 255))
      ಪ್ರತ್ಯಾಗಮ;

    end = hb_min (end, len);

    ಯದಿ (interior && !from_out_buffer && end - start < 2)
      ಪ್ರತ್ಯಾಗಮ;

    _set_glyph_flags_impl (mask, start, end, interior, from_out_buffer);
  }


  ಶೂನ್ಯ unsafe_to_break (unsigned ಪೂರ್ಣಾಙ್ಕ start = 0, unsigned ಪೂರ್ಣಾಙ್ಕ end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      ಸತ್ಯ);
  }
  ಶೂನ್ಯ safe_to_ಸೇರಿಸು_tatweel (unsigned ಪೂರ್ಣಾಙ್ಕ start = 0, unsigned ಪೂರ್ಣಾಙ್ಕ end = -1)
  {
    ಯದಿ ((flags & HB_BUFFER_FLAG_PRODUCE_SAFE_TO_ಸೇರಿಸು_TATWEEL) == 0)
    {
      unsafe_to_break (start, end);
      ಪ್ರತ್ಯಾಗಮ;
    }
    _set_glyph_flags (HB_GLYPH_FLAG_SAFE_TO_ಸೇರಿಸು_TATWEEL,
		      start, end,
		      ಸತ್ಯ);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ಶೂನ್ಯ unsafe_to_concat (unsigned ಪೂರ್ಣಾಙ್ಕ start = 0, unsigned ಪೂರ್ಣಾಙ್ಕ end = -1)
  {
    ಯದಿ (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      ಪ್ರತ್ಯಾಗಮ;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      ಅಸತ್ಯ);
  }
  ಶೂನ್ಯ unsafe_to_break_from_outbuffer (unsigned ಪೂರ್ಣಾಙ್ಕ start = 0, unsigned ಪೂರ್ಣಾಙ್ಕ end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      ಸತ್ಯ, ಸತ್ಯ);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ಶೂನ್ಯ unsafe_to_concat_from_outbuffer (unsigned ಪೂರ್ಣಾಙ್ಕ start = 0, unsigned ಪೂರ್ಣಾಙ್ಕ end = -1)
  {
    ಯದಿ (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      ಪ್ರತ್ಯಾಗಮ;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      ಅಸತ್ಯ, ಸತ್ಯ);
  }


  /* Internal methods */
  HB_NODISCARD HB_INTERNAL ತಾರ್ಕಿಕ move_to (unsigned ಪೂರ್ಣಾಙ್ಕ i); /* i is output-buffer index. */

  HB_NODISCARD HB_INTERNAL ತಾರ್ಕಿಕ enlarge (unsigned ಪೂರ್ಣಾಙ್ಕ size);

  HB_NODISCARD ತಾರ್ಕಿಕ resize (unsigned length)
  {
    ಪ್ರತಿಜ್ಞಾ (!have_output);
    ಯದಿ (unlikely (!ensure (length))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    len = length;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  HB_NODISCARD ತಾರ್ಕಿಕ ensure (unsigned ಪೂರ್ಣಾಙ್ಕ size)
  { ಪ್ರತ್ಯಾಗಮ likely (!size || size < allocated) ? ಸತ್ಯ : enlarge (size); }

  HB_NODISCARD ತಾರ್ಕಿಕ ensure_inplace (unsigned ಪೂರ್ಣಾಙ್ಕ size)
  { ಪ್ರತ್ಯಾಗಮ likely (!size || size < allocated); }

  ಶೂನ್ಯ assert_glyphs ()
  {
    ಪ್ರತಿಜ್ಞಾ ((content_type == HB_BUFFER_CONTENT_TYPE_GLYPHS) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  ಶೂನ್ಯ assert_unicode ()
  {
    ಪ್ರತಿಜ್ಞಾ ((content_type == HB_BUFFER_CONTENT_TYPE_UNICODE) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  HB_NODISCARD ತಾರ್ಕಿಕ ensure_glyphs ()
  {
    ಯದಿ (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_GLYPHS))
    {
      ಯದಿ (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಪ್ರತಿಜ್ಞಾ (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_GLYPHS;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  HB_NODISCARD ತಾರ್ಕಿಕ ensure_unicode ()
  {
    ಯದಿ (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_UNICODE))
    {
      ಯದಿ (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಪ್ರತಿಜ್ಞಾ (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_UNICODE;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  HB_NODISCARD HB_INTERNAL ತಾರ್ಕಿಕ make_room_for (unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_in, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_out);
  HB_NODISCARD HB_INTERNAL ತಾರ್ಕಿಕ shift_forward (unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ);

  ಪ್ರಕಾರನಿರ್ದೇಶ long scratch_buffer_t;
  HB_INTERNAL scratch_buffer_t *get_scratch_buffer (unsigned ಪೂರ್ಣಾಙ್ಕ *size);

  ಶೂನ್ಯ clear_context (unsigned ಪೂರ್ಣಾಙ್ಕ side) { context_len[side] = 0; }

  HB_INTERNAL ಶೂನ್ಯ sort (unsigned ಪೂರ್ಣಾಙ್ಕ start, unsigned ಪೂರ್ಣಾಙ್ಕ end, ಪೂರ್ಣಾಙ್ಕ(*compar)(ಧ್ರುವ hb_glyph_info_t *, ಧ್ರುವ hb_glyph_info_t *));

  ತಾರ್ಕಿಕ messaging ()
  {
#ifdef HB_NO_BUFFER_MESSAGE
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ unlikely (message_func);
#endif
  }
  ತಾರ್ಕಿಕ message (hb_font_t *font, ಧ್ರುವ ವರ್ಣ *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#ifdef HB_NO_BUFFER_MESSAGE
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#ಅನ್ಯಥಾ
    ಯದಿ (likely (!messaging ()))
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    va_list ap;
    va_start (ap, fmt);
    ತಾರ್ಕಿಕ ret = message_impl (font, fmt, ap);
    va_end (ap);

    ಪ್ರತ್ಯಾಗಮ ret;
#endif
  }
  HB_INTERNAL ತಾರ್ಕಿಕ message_impl (hb_font_t *font, ಧ್ರುವ ವರ್ಣ *fmt, va_list ap) HB_PRINTF_FUNC(3, 0);
  HB_INTERNAL ಶೂನ್ಯ changed ();

  ಸ್ಥಿರ ಶೂನ್ಯ
  set_cluster (hb_glyph_info_t &inf, unsigned ಪೂರ್ಣಾಙ್ಕ cluster, unsigned ಪೂರ್ಣಾಙ್ಕ mask = 0)
  {
    ಯದಿ (inf.cluster != cluster)
      inf.mask = (inf.mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  ಶೂನ್ಯ
  _infos_set_glyph_flags (hb_glyph_info_t *infos,
			  unsigned ಪೂರ್ಣಾಙ್ಕ start, unsigned ಪೂರ್ಣಾಙ್ಕ end,
			  unsigned ಪೂರ್ಣಾಙ್ಕ cluster,
			  hb_mask_t mask)
  {
    ಯದಿ (unlikely (start == end))
      ಪ್ರತ್ಯಾಗಮ;

    max_ops -= end - start;
    ಯದಿ (unlikely (max_ops < 0))
      successful = ಅಸತ್ಯ;

    unsigned cluster_first = infos[start].cluster;
    unsigned cluster_last = infos[end - 1].cluster;

    ಯದಿ (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_first && cluster != cluster_last))
    {
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = start; i < end; i++)
	ಯದಿ (cluster != infos[i].cluster)
	  infos[i].mask |= mask;
      ಪ್ರತ್ಯಾಗಮ;
    }

    /* Monotone clusters */

    ಯದಿ (cluster == cluster_first)
    {
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = end; start < i && infos[i - 1].cluster != cluster_first; i--)
	infos[i - 1].mask |= mask;
    }
    ಅನ್ಯಥಾ /* cluster == cluster_last */
    {
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = start; i < end && infos[i].cluster != cluster_last; i++)
	infos[i].mask |= mask;
    }
  }
  unsigned
  _infos_find_min_cluster (ಧ್ರುವ hb_glyph_info_t *infos,
			   unsigned start, unsigned end,
			   unsigned cluster = UINT_MAX)
  {
    ಯದಿ (unlikely (start == end))
      ಪ್ರತ್ಯಾಗಮ cluster;

    ಯದಿ (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS)
    {
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = start; i < end; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      ಪ್ರತ್ಯಾಗಮ cluster;
    }

    ಪ್ರತ್ಯಾಗಮ hb_min (cluster, hb_min (infos[start].cluster, infos[end - 1].cluster));
  }

  ಶೂನ್ಯ clear_glyph_flags (hb_mask_t mask = 0)
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++)
      info[i].mask = (info[i].mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_buffer_t);


#define foreach_group(buffer, start, end, group_func) \
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ \
       ಎಣಿಕೆ_2 = buffer->len, \
       start = 0, end = ಎಣಿಕೆ_2 ? buffer->group_end (0, group_func) : 0; \
       start < ಎಣಿಕೆ_2; \
       start = end, end = buffer->group_end (start, group_func))

#define foreach_cluster(buffer, start, end) \
	foreach_group (buffer, start, end, hb_buffer_t::_cluster_group_func)


#define HB_BUFFER_XALLOCATE_VAR(b, func, var) \
  b->func (offsetof (hb_glyph_info_t, var) - offsetof(hb_glyph_info_t, var1), \
	   ಪರಿಮಾಣ (b->info[0].var))
#define HB_BUFFER_ALLOCATE_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, allocate_var,     var ())
#define HB_BUFFER_TRY_ALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, try_allocate_var, var ())
#define HB_BUFFER_DEALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, deallocate_var,   var ())
#define HB_BUFFER_ASSERT_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, assert_var,       var ())


#endif /* HB_BUFFER_HH */
