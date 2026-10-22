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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-unicode.hh"
#ଅନ୍ତର୍ଭାଵ "hb-set-digest.hh"


static_assert ((ଆକାର (hb_glyph_info_t) == 20), "");
static_assert ((ଆକାର (hb_glyph_info_t) == ଆକାର (hb_glyph_position_t)), "");

HB_MARK_AS_FLAG_T (hb_glyph_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_serialize_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_diff_flags_t);

ଗଣନା hb_buffer_scratch_flags_t {
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

ସଂରଚନା hb_buffer_t
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

  ତାର୍କିକ successful; /* Allocations successful */
  ତାର୍କିକ have_output; /* Whether we have an output buffer going on */
  ତାର୍କିକ have_positions; /* Whether we have positions */

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx; /* Cursor into ->info and ->pos arrays */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len; /* Length of ->info and ->pos arrays */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_len; /* Length of ->out_info array if have_output */

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *out_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  ସ୍ଥିର constexpr unsigned CONTEXT_LENGTH = 5u;
  hb_codepoint_t context[2][CONTEXT_LENGTH];
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା context_len[2];

  hb_set_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_bits;
  uint8_t serial;
  uint32_t random_state;
  hb_buffer_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_len; /* Maximum allowed len. */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#ifndef HB_NO_BUFFER_MESSAGE
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*changed_func_t) (hb_buffer_t *buffer, ଶୂନ୍ୟ *user_data);

  hb_buffer_message_func_t message_func;
  ଶୂନ୍ୟ *message_data;
  hb_destroy_func_t message_destroy;
  changed_func_t changed_func;
  ଶୂନ୍ୟ *changed_data;
  unsigned message_depth; /* How deeply are we inside a message callback? */
#ଅନ୍ୟଥା
  ସ୍ଥିର constexpr unsigned message_depth = 0u;
#endif



  /* Methods */

  HB_NODISCARD ତାର୍କିକ in_error () ଅଚଳ { ଫେରନ୍ତୁ !successful; }

  ଶୂନ୍ୟ allocate_var (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = start + ଗଣନା_2;
    ପ୍ରତିଜ୍ଞା (end <= 8);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା bits = (1u<<end) - (1u<<start);
    ପ୍ରତିଜ୍ଞା (0 == (allocated_var_bits & bits));
    allocated_var_bits |= bits;
  }
  ତାର୍କିକ try_allocate_var (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = start + ଗଣନା_2;
    ପ୍ରତିଜ୍ଞା (end <= 8);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା bits = (1u<<end) - (1u<<start);
    ଯଦି (allocated_var_bits & bits)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    allocated_var_bits |= bits;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  ଶୂନ୍ୟ deallocate_var (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = start + ଗଣନା_2;
    ପ୍ରତିଜ୍ଞା (end <= 8);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା bits = (1u<<end) - (1u<<start);
    ପ୍ରତିଜ୍ଞା (bits == (allocated_var_bits & bits));
    allocated_var_bits &= ~bits;
  }
  ଶୂନ୍ୟ assert_var (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = start + ଗଣନା_2;
    ପ୍ରତିଜ୍ଞା (end <= 8);
    HB_UNUSED unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା bits = (1u<<end) - (1u<<start);
    ପ୍ରତିଜ୍ଞା (bits == (allocated_var_bits & bits));
  }
  ଶୂନ୍ୟ deallocate_var_all ()
  {
    allocated_var_bits = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0) { ଫେରନ୍ତୁ info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0) ଅଚଳ { ଫେରନ୍ତୁ info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0) { ଫେରନ୍ତୁ pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0) ଅଚଳ { ଫେରନ୍ତୁ pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { ଫେରନ୍ତୁ out_info[out_len ? out_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () ଅଚଳ { ଫେରନ୍ତୁ out_info[out_len ? out_len - 1 : 0]; }

  template <typename set_t>
  ଶୂନ୍ୟ collect_codepoints (set_t &d) ଅଚଳ
  { d.clear (); d.add_array (&info[0].codepoint, len, ଆକାର (info[0])); }

  ଶୂନ୍ୟ update_digest ()
  {
    digest = hb_set_digest_t ();
    collect_codepoints (digest);
  }

  HB_INTERNAL ଶୂନ୍ୟ similar (ଅଚଳ hb_buffer_t &src);
  HB_INTERNAL ଶୂନ୍ୟ reset ();
  HB_INTERNAL ଶୂନ୍ୟ clear ();

  /* Called around shape() */
  HB_INTERNAL ଶୂନ୍ୟ enter ();
  HB_INTERNAL ଶୂନ୍ୟ leave ();

#ifndef HB_NO_BUFFER_VERIFY
  HB_INTERNAL
#endif
  ତାର୍କିକ verify (hb_buffer_t        *text_buffer,
	       hb_font_t          *font,
	       ଅଚଳ hb_feature_t *features,
	       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା        ଅଙ୍କ_features,
	       ଅଚଳ ଅକ୍ଷର * ଅଚଳ *shapers)
#ifndef HB_NO_BUFFER_VERIFY
  ;
#ଅନ୍ୟଥା
  { ଫେରନ୍ତୁ ସତ୍ୟ; }
#endif

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା backtrack_len () ଅଚଳ { ଫେରନ୍ତୁ have_output ? out_len : idx; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookahead_len () ଅଚଳ { ଫେରନ୍ତୁ len - idx; }
  uint8_t next_serial () { ଫେରନ୍ତୁ ++serial ? serial : ++serial; }

  HB_INTERNAL ଶୂନ୍ୟ add (hb_codepoint_t  codepoint,
			unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    cluster);
  HB_INTERNAL ଶୂନ୍ୟ add_info (ଅଚଳ hb_glyph_info_t &glyph_info);
  HB_INTERNAL ଶୂନ୍ୟ add_info_and_pos (ଅଚଳ hb_glyph_info_t &glyph_info,
				     ଅଚଳ hb_glyph_position_t &glyph_pos);

  ଶୂନ୍ୟ reverse_range (unsigned start, unsigned end)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (start, end);
    ଯଦି (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (start, end);
  }
  ଶୂନ୍ୟ reverse () { reverse_range (0, len); }

  template <typename FuncType>
  ଶୂନ୍ୟ reverse_groups (ଅଚଳ FuncType& group,
		       ତାର୍କିକ merge_clusters = ମିଥ୍ୟା)
  {
    ଯଦି (unlikely (!len))
      ଫେରନ୍ତୁ;

    unsigned start = 0;
    unsigned i;
    ପାଇଁ (i = 1; i < len; i++)
    {
      ଯଦି (!group (info[i - 1], info[i]))
      {
	ଯଦି (merge_clusters)
	  ଏହି->merge_clusters (start, i);
	reverse_range (start, i);
	start = i;
      }
    }
    ଯଦି (merge_clusters)
      ଏହି->merge_clusters (start, i);
    reverse_range (start, i);

    reverse ();
  }

  template <typename FuncType>
  unsigned group_end (unsigned start, ଅଚଳ FuncType& group) ଅଚଳ
  {
    ଯେତେବେଳେ (++start < len && group (info[start - 1], info[start]))
      ;

    ଫେରନ୍ତୁ start;
  }

  ସ୍ଥିର ତାର୍କିକ _cluster_group_func (ଅଚଳ hb_glyph_info_t& a,
				   ଅଚଳ hb_glyph_info_t& b)
  { ଫେରନ୍ତୁ a.cluster == b.cluster; }

  ଶୂନ୍ୟ reverse_clusters () { reverse_groups (_cluster_group_func); }

  HB_INTERNAL ଶୂନ୍ୟ guess_segment_properties ();

  HB_INTERNAL ତାର୍କିକ ସମକାଳ ();
  HB_INTERNAL ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_so_far ();
  HB_INTERNAL ଶୂନ୍ୟ clear_output ();
  HB_INTERNAL ଶୂନ୍ୟ clear_positions ();

  template <typename T>
  HB_NODISCARD ତାର୍କିକ replace_glyphs (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_in,
				    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_out,
				    ଅଚଳ T *glyph_data)
  {
    ଯଦି (unlikely (!make_room_for (ଅଙ୍କ_in, ଅଙ୍କ_out))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପ୍ରତିଜ୍ଞା (idx + ଅଙ୍କ_in <= len);

    merge_clusters (idx, idx + ଅଙ୍କ_in);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &out_info[out_len];
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_out; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_data[i];
      pinfo++;
    }

    idx  += ଅଙ୍କ_in;
    out_len += ଅଙ୍କ_out;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  HB_NODISCARD ତାର୍କିକ replace_glyph (hb_codepoint_t glyph_index)
  { ଫେରନ୍ତୁ replace_glyphs (1, 1, &glyph_index); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD ତାର୍କିକ output_glyph (hb_codepoint_t glyph_index)
  { ଫେରନ୍ତୁ replace_glyphs (0, 1, &glyph_index); }

  HB_NODISCARD ତାର୍କିକ output_info (ଅଚଳ hb_glyph_info_t &glyph_info)
  {
    ଯଦି (unlikely (!make_room_for (0, 1))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    out_info[out_len] = glyph_info;

    out_len++;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  /* Copies glyph at idx to output but doesn't advance idx */
  HB_NODISCARD ତାର୍କିକ copy_glyph ()
  {
    /* Extra copy because cur()'s return can be freed within
     * output_info() call if buffer reallocates. */
    ଫେରନ୍ତୁ output_info (hb_glyph_info_t (cur()));
  }

  /* Copies glyph at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD ତାର୍କିକ next_glyph ()
  {
    ଯଦି (have_output)
    {
      ଯଦି (out_info != info || out_len != idx)
      {
	ଯଦି (unlikely (!ensure (out_len + 1))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	out_info[out_len] = info[idx];
      }
      out_len++;
    }

    idx++;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD ତାର୍କିକ next_glyphs (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
  {
    ଯଦି (have_output)
    {
      ଯଦି (out_info != info || out_len != idx)
      {
	ଯଦି (unlikely (!ensure (out_len + n))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	memmove (out_info + out_len, info + idx, n * ଆକାର (out_info[0]));
      }
      out_len += n;
    }

    idx += n;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  /* Advance idx without copying to output. */
  ଶୂନ୍ୟ skip_glyph () { idx++; }
  ଶୂନ୍ୟ add_masks (hb_mask_t mask)
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < len; j++)
      info[j].mask |= mask;
  }
  HB_INTERNAL ଶୂନ୍ୟ set_masks (hb_mask_t value, hb_mask_t mask,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cluster_start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cluster_end);

  ଶୂନ୍ୟ merge_clusters (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end)
  {
    ଯଦି (end - start < 2)
      ଫେରନ୍ତୁ;
    ଯଦି (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_to_break (start, end);
      ଫେରନ୍ତୁ;
    }
    merge_clusters_impl (start, end);
  }
  ଶୂନ୍ୟ merge_grapheme_clusters (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end)
  {
    ଯଦି (end - start < 2)
      ଫେରନ୍ତୁ;
    ଯଦି (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_to_break (start, end);
      ଫେରନ୍ତୁ;
    }
    merge_clusters_impl (start, end);
  }
  HB_INTERNAL ଶୂନ୍ୟ merge_clusters_impl (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end);
  ଶୂନ୍ୟ merge_out_clusters (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end)
  {
    ଯଦି (end - start < 2)
      ଫେରନ୍ତୁ;
    ଯଦି (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      ଫେରନ୍ତୁ;
    merge_out_clusters_impl (start, end);
  }
  ଶୂନ୍ୟ merge_out_grapheme_clusters (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end)
  {
    ଯଦି (end - start < 2)
      ଫେରନ୍ତୁ;
    ଯଦି (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      ଫେରନ୍ତୁ;
    merge_out_clusters_impl (start, end);
  }
  HB_INTERNAL ଶୂନ୍ୟ merge_out_clusters_impl (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_INTERNAL ଶୂନ୍ୟ delete_glyph ();
  HB_INTERNAL ଶୂନ୍ୟ delete_glyphs_inplace (ତାର୍କିକ (*filter) (ଅଚଳ hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  ଶୂନ୍ୟ _set_glyph_flags_impl (hb_mask_t mask,
			      unsigned start,
			      unsigned end,
			      ତାର୍କିକ interior,
			      ତାର୍କିକ from_out_buffer)
  {
    ଯଦି (!from_out_buffer || !have_output)
    {
      ଯଦି (!interior)
      {
	ପାଇଁ (unsigned i = start; i < end; i++)
	  info[i].mask |= mask;
      }
      ଅନ୍ୟଥା
      {
	unsigned cluster = _infos_find_min_cluster (info, start, end);
	_infos_set_glyph_flags (info, start, end, cluster, mask);
      }
    }
    ଅନ୍ୟଥା
    {
      ପ୍ରତିଜ୍ଞା (start <= out_len);
      ପ୍ରତିଜ୍ଞା (idx <= end);

      ଯଦି (!interior)
      {
	ପାଇଁ (unsigned i = start; i < out_len; i++)
	  out_info[i].mask |= mask;
	ପାଇଁ (unsigned i = idx; i < end; i++)
	  info[i].mask |= mask;
      }
      ଅନ୍ୟଥା
      {
	unsigned cluster = _infos_find_min_cluster (info, idx, end);
	cluster = _infos_find_min_cluster (out_info, start, out_len, cluster);

	_infos_set_glyph_flags (out_info, start, out_len, cluster, mask);
	_infos_set_glyph_flags (info, idx, end, cluster, mask);
      }
    }
  }

  HB_ALWAYS_INLINE
  ଶୂନ୍ୟ _set_glyph_flags (hb_mask_t mask,
			 unsigned start = 0,
			 unsigned end = (unsigned) -1,
			 ତାର୍କିକ interior = ମିଥ୍ୟା,
			 ତାର୍କିକ from_out_buffer = ମିଥ୍ୟା)
  {
    ଯଦି (unlikely (end != (unsigned) -1 && end - start > 255))
      ଫେରନ୍ତୁ;

    end = hb_min (end, len);

    ଯଦି (interior && !from_out_buffer && end - start < 2)
      ଫେରନ୍ତୁ;

    _set_glyph_flags_impl (mask, start, end, interior, from_out_buffer);
  }


  ଶୂନ୍ୟ unsafe_to_break (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = 0, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      ସତ୍ୟ);
  }
  ଶୂନ୍ୟ safe_to_ଭର୍ତ୍ତି_tatweel (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = 0, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = -1)
  {
    ଯଦି ((flags & HB_BUFFER_FLAG_PRODUCE_SAFE_TO_ଭର୍ତ୍ତି_TATWEEL) == 0)
    {
      unsafe_to_break (start, end);
      ଫେରନ୍ତୁ;
    }
    _set_glyph_flags (HB_GLYPH_FLAG_SAFE_TO_ଭର୍ତ୍ତି_TATWEEL,
		      start, end,
		      ସତ୍ୟ);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ଶୂନ୍ୟ unsafe_to_concat (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = 0, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = -1)
  {
    ଯଦି (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      ଫେରନ୍ତୁ;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      ମିଥ୍ୟା);
  }
  ଶୂନ୍ୟ unsafe_to_break_from_outbuffer (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = 0, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      ସତ୍ୟ, ସତ୍ୟ);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ଶୂନ୍ୟ unsafe_to_concat_from_outbuffer (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = 0, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = -1)
  {
    ଯଦି (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      ଫେରନ୍ତୁ;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      ମିଥ୍ୟା, ସତ୍ୟ);
  }


  /* Internal methods */
  HB_NODISCARD HB_INTERNAL ତାର୍କିକ move_to (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i); /* i is output-buffer index. */

  HB_NODISCARD HB_INTERNAL ତାର୍କିକ enlarge (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size);

  HB_NODISCARD ତାର୍କିକ resize (unsigned length)
  {
    ପ୍ରତିଜ୍ଞା (!have_output);
    ଯଦି (unlikely (!ensure (length))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    len = length;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  HB_NODISCARD ତାର୍କିକ ensure (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
  { ଫେରନ୍ତୁ likely (!size || size < allocated) ? ସତ୍ୟ : enlarge (size); }

  HB_NODISCARD ତାର୍କିକ ensure_inplace (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
  { ଫେରନ୍ତୁ likely (!size || size < allocated); }

  ଶୂନ୍ୟ assert_glyphs ()
  {
    ପ୍ରତିଜ୍ଞା ((content_type == HB_BUFFER_CONTENT_TYPE_GLYPHS) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  ଶୂନ୍ୟ assert_unicode ()
  {
    ପ୍ରତିଜ୍ଞା ((content_type == HB_BUFFER_CONTENT_TYPE_UNICODE) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  HB_NODISCARD ତାର୍କିକ ensure_glyphs ()
  {
    ଯଦି (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_GLYPHS))
    {
      ଯଦି (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ପ୍ରତିଜ୍ଞା (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_GLYPHS;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  HB_NODISCARD ତାର୍କିକ ensure_unicode ()
  {
    ଯଦି (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_UNICODE))
    {
      ଯଦି (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ପ୍ରତିଜ୍ଞା (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_UNICODE;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  HB_NODISCARD HB_INTERNAL ତାର୍କିକ make_room_for (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_in, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_out);
  HB_NODISCARD HB_INTERNAL ତାର୍କିକ shift_forward (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2);

  ପ୍ରକାରବ୍ୟାଖ୍ୟା long scratch_buffer_t;
  HB_INTERNAL scratch_buffer_t *get_scratch_buffer (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *size);

  ଶୂନ୍ୟ clear_context (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା side) { context_len[side] = 0; }

  HB_INTERNAL ଶୂନ୍ୟ sort (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end, ପୂର୍ଣ୍ଣସଂଖ୍ୟା(*compar)(ଅଚଳ hb_glyph_info_t *, ଅଚଳ hb_glyph_info_t *));

  ତାର୍କିକ messaging ()
  {
#ifdef HB_NO_BUFFER_MESSAGE
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ unlikely (message_func);
#endif
  }
  ତାର୍କିକ message (hb_font_t *font, ଅଚଳ ଅକ୍ଷର *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#ifdef HB_NO_BUFFER_MESSAGE
    ଫେରନ୍ତୁ ସତ୍ୟ;
#ଅନ୍ୟଥା
    ଯଦି (likely (!messaging ()))
      ଫେରନ୍ତୁ ସତ୍ୟ;

    va_list ap;
    va_start (ap, fmt);
    ତାର୍କିକ ret = message_impl (font, fmt, ap);
    va_end (ap);

    ଫେରନ୍ତୁ ret;
#endif
  }
  HB_INTERNAL ତାର୍କିକ message_impl (hb_font_t *font, ଅଚଳ ଅକ୍ଷର *fmt, va_list ap) HB_PRINTF_FUNC(3, 0);
  HB_INTERNAL ଶୂନ୍ୟ changed ();

  ସ୍ଥିର ଶୂନ୍ୟ
  set_cluster (hb_glyph_info_t &inf, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cluster, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mask = 0)
  {
    ଯଦି (inf.cluster != cluster)
      inf.mask = (inf.mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  ଶୂନ୍ୟ
  _infos_set_glyph_flags (hb_glyph_info_t *infos,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cluster,
			  hb_mask_t mask)
  {
    ଯଦି (unlikely (start == end))
      ଫେରନ୍ତୁ;

    max_ops -= end - start;
    ଯଦି (unlikely (max_ops < 0))
      successful = ମିଥ୍ୟା;

    unsigned cluster_first = infos[start].cluster;
    unsigned cluster_last = infos[end - 1].cluster;

    ଯଦି (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_first && cluster != cluster_last))
    {
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = start; i < end; i++)
	ଯଦି (cluster != infos[i].cluster)
	  infos[i].mask |= mask;
      ଫେରନ୍ତୁ;
    }

    /* Monotone clusters */

    ଯଦି (cluster == cluster_first)
    {
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = end; start < i && infos[i - 1].cluster != cluster_first; i--)
	infos[i - 1].mask |= mask;
    }
    ଅନ୍ୟଥା /* cluster == cluster_last */
    {
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = start; i < end && infos[i].cluster != cluster_last; i++)
	infos[i].mask |= mask;
    }
  }
  unsigned
  _infos_find_min_cluster (ଅଚଳ hb_glyph_info_t *infos,
			   unsigned start, unsigned end,
			   unsigned cluster = UINT_MAX)
  {
    ଯଦି (unlikely (start == end))
      ଫେରନ୍ତୁ cluster;

    ଯଦି (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS)
    {
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = start; i < end; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      ଫେରନ୍ତୁ cluster;
    }

    ଫେରନ୍ତୁ hb_min (cluster, hb_min (infos[start].cluster, infos[end - 1].cluster));
  }

  ଶୂନ୍ୟ clear_glyph_flags (hb_mask_t mask = 0)
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++)
      info[i].mask = (info[i].mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_buffer_t);


#define foreach_group(buffer, start, end, group_func) \
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା \
       ଗଣନା_3 = buffer->len, \
       start = 0, end = ଗଣନା_3 ? buffer->group_end (0, group_func) : 0; \
       start < ଗଣନା_3; \
       start = end, end = buffer->group_end (start, group_func))

#define foreach_cluster(buffer, start, end) \
	foreach_group (buffer, start, end, hb_buffer_t::_cluster_group_func)


#define HB_BUFFER_XALLOCATE_VAR(b, func, var) \
  b->func (offsetof (hb_glyph_info_t, var) - offsetof(hb_glyph_info_t, var1), \
	   ଆକାର (b->info[0].var))
#define HB_BUFFER_ALLOCATE_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, allocate_var,     var ())
#define HB_BUFFER_TRY_ALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, try_allocate_var, var ())
#define HB_BUFFER_DEALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, deallocate_var,   var ())
#define HB_BUFFER_ASSERT_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, assert_var,       var ())


#endif /* HB_BUFFER_HH */
