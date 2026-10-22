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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-unicode.hh"
#অন্তর্ভাব "hb-set-digest.hh"


static_assert ((পরিমাণ (hb_glyph_info_t) == 20), "");
static_assert ((পরিমাণ (hb_glyph_info_t) == পরিমাণ (hb_glyph_position_t)), "");

HB_MARK_AS_FLAG_T (hb_glyph_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_serialize_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_diff_flags_t);

গণনা hb_buffer_scratch_flags_t {
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

সংরচনা hb_buffer_t
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

  তার্কিক successful; /* Allocations successful */
  তার্কিক have_output; /* Whether we have an output buffer going on */
  তার্কিক have_positions; /* Whether we have positions */

  unsigned পূর্ণাঙ্ক idx; /* Cursor into ->info and ->pos arrays */
  unsigned পূর্ণাঙ্ক len; /* Length of ->info and ->pos arrays */
  unsigned পূর্ণাঙ্ক out_len; /* Length of ->out_info array if have_output */

  unsigned পূর্ণাঙ্ক allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *out_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  স্থির constexpr unsigned CONTEXT_LENGTH = 5u;
  hb_codepoint_t context[2][CONTEXT_LENGTH];
  unsigned পূর্ণাঙ্ক context_len[2];

  hb_set_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_bits;
  uint8_t serial;
  uint32_t random_state;
  hb_buffer_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  unsigned পূর্ণাঙ্ক max_len; /* Maximum allowed len. */
  পূর্ণাঙ্ক max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#ifndef HB_NO_BUFFER_MESSAGE
  প্রকারনির্দেশ শূন্য (*changed_func_t) (hb_buffer_t *buffer, শূন্য *user_data);

  hb_buffer_message_func_t message_func;
  শূন্য *message_data;
  hb_destroy_func_t message_destroy;
  changed_func_t changed_func;
  শূন্য *changed_data;
  unsigned message_depth; /* How deeply are we inside a message callback? */
#অন্যথা
  স্থির constexpr unsigned message_depth = 0u;
#endif



  /* Methods */

  HB_NODISCARD তার্কিক in_error () ধ্রুব { প্রত্যাগম !successful; }

  শূন্য allocate_var (unsigned পূর্ণাঙ্ক start, unsigned পূর্ণাঙ্ক গুনতি)
  {
    unsigned পূর্ণাঙ্ক end = start + গুনতি;
    প্রতিজ্ঞা (end <= 8);
    unsigned পূর্ণাঙ্ক bits = (1u<<end) - (1u<<start);
    প্রতিজ্ঞা (0 == (allocated_var_bits & bits));
    allocated_var_bits |= bits;
  }
  তার্কিক try_allocate_var (unsigned পূর্ণাঙ্ক start, unsigned পূর্ণাঙ্ক গুনতি)
  {
    unsigned পূর্ণাঙ্ক end = start + গুনতি;
    প্রতিজ্ঞা (end <= 8);
    unsigned পূর্ণাঙ্ক bits = (1u<<end) - (1u<<start);
    যদি (allocated_var_bits & bits)
      প্রত্যাগম অসত্য;
    allocated_var_bits |= bits;
    প্রত্যাগম সত্য;
  }
  শূন্য deallocate_var (unsigned পূর্ণাঙ্ক start, unsigned পূর্ণাঙ্ক গুনতি)
  {
    unsigned পূর্ণাঙ্ক end = start + গুনতি;
    প্রতিজ্ঞা (end <= 8);
    unsigned পূর্ণাঙ্ক bits = (1u<<end) - (1u<<start);
    প্রতিজ্ঞা (bits == (allocated_var_bits & bits));
    allocated_var_bits &= ~bits;
  }
  শূন্য assert_var (unsigned পূর্ণাঙ্ক start, unsigned পূর্ণাঙ্ক গুনতি)
  {
    unsigned পূর্ণাঙ্ক end = start + গুনতি;
    প্রতিজ্ঞা (end <= 8);
    HB_UNUSED unsigned পূর্ণাঙ্ক bits = (1u<<end) - (1u<<start);
    প্রতিজ্ঞা (bits == (allocated_var_bits & bits));
  }
  শূন্য deallocate_var_all ()
  {
    allocated_var_bits = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (unsigned পূর্ণাঙ্ক i = 0) { প্রত্যাগম info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (unsigned পূর্ণাঙ্ক i = 0) ধ্রুব { প্রত্যাগম info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (unsigned পূর্ণাঙ্ক i = 0) { প্রত্যাগম pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (unsigned পূর্ণাঙ্ক i = 0) ধ্রুব { প্রত্যাগম pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { প্রত্যাগম out_info[out_len ? out_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () ধ্রুব { প্রত্যাগম out_info[out_len ? out_len - 1 : 0]; }

  template <typename set_t>
  শূন্য collect_codepoints (set_t &d) ধ্রুব
  { d.clear (); d.add_array (&info[0].codepoint, len, পরিমাণ (info[0])); }

  শূন্য update_digest ()
  {
    digest = hb_set_digest_t ();
    collect_codepoints (digest);
  }

  HB_INTERNAL শূন্য similar (ধ্রুব hb_buffer_t &src);
  HB_INTERNAL শূন্য reset ();
  HB_INTERNAL শূন্য clear ();

  /* Called around shape() */
  HB_INTERNAL শূন্য enter ();
  HB_INTERNAL শূন্য leave ();

#ifndef HB_NO_BUFFER_VERIFY
  HB_INTERNAL
#endif
  তার্কিক verify (hb_buffer_t        *text_buffer,
	       hb_font_t          *font,
	       ধ্রুব hb_feature_t *features,
	       unsigned পূর্ণাঙ্ক        অঙ্ক_features,
	       ধ্রুব বর্ণ * ধ্রুব *shapers)
#ifndef HB_NO_BUFFER_VERIFY
  ;
#অন্যথা
  { প্রত্যাগম সত্য; }
#endif

  unsigned পূর্ণাঙ্ক backtrack_len () ধ্রুব { প্রত্যাগম have_output ? out_len : idx; }
  unsigned পূর্ণাঙ্ক lookahead_len () ধ্রুব { প্রত্যাগম len - idx; }
  uint8_t next_serial () { প্রত্যাগম ++serial ? serial : ++serial; }

  HB_INTERNAL শূন্য add (hb_codepoint_t  codepoint,
			unsigned পূর্ণাঙ্ক    cluster);
  HB_INTERNAL শূন্য add_info (ধ্রুব hb_glyph_info_t &glyph_info);
  HB_INTERNAL শূন্য add_info_and_pos (ধ্রুব hb_glyph_info_t &glyph_info,
				     ধ্রুব hb_glyph_position_t &glyph_pos);

  শূন্য reverse_range (unsigned start, unsigned end)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (start, end);
    যদি (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (start, end);
  }
  শূন্য reverse () { reverse_range (0, len); }

  template <typename FuncType>
  শূন্য reverse_groups (ধ্রুব FuncType& group,
		       তার্কিক merge_clusters = অসত্য)
  {
    যদি (unlikely (!len))
      প্রত্যাগম;

    unsigned start = 0;
    unsigned i;
    প্রতি (i = 1; i < len; i++)
    {
      যদি (!group (info[i - 1], info[i]))
      {
	যদি (merge_clusters)
	  এতত্->merge_clusters (start, i);
	reverse_range (start, i);
	start = i;
      }
    }
    যদি (merge_clusters)
      এতত্->merge_clusters (start, i);
    reverse_range (start, i);

    reverse ();
  }

  template <typename FuncType>
  unsigned group_end (unsigned start, ধ্রুব FuncType& group) ধ্রুব
  {
    যাবত্ (++start < len && group (info[start - 1], info[start]))
      ;

    প্রত্যাগম start;
  }

  স্থির তার্কিক _cluster_group_func (ধ্রুব hb_glyph_info_t& a,
				   ধ্রুব hb_glyph_info_t& b)
  { প্রত্যাগম a.cluster == b.cluster; }

  শূন্য reverse_clusters () { reverse_groups (_cluster_group_func); }

  HB_INTERNAL শূন্য guess_segment_properties ();

  HB_INTERNAL তার্কিক সমকাল ();
  HB_INTERNAL পূর্ণাঙ্ক sync_so_far ();
  HB_INTERNAL শূন্য clear_output ();
  HB_INTERNAL শূন্য clear_positions ();

  template <typename T>
  HB_NODISCARD তার্কিক replace_glyphs (unsigned পূর্ণাঙ্ক অঙ্ক_in,
				    unsigned পূর্ণাঙ্ক অঙ্ক_out,
				    ধ্রুব T *glyph_data)
  {
    যদি (unlikely (!make_room_for (অঙ্ক_in, অঙ্ক_out))) প্রত্যাগম অসত্য;

    প্রতিজ্ঞা (idx + অঙ্ক_in <= len);

    merge_clusters (idx, idx + অঙ্ক_in);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &out_info[out_len];
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < অঙ্ক_out; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_data[i];
      pinfo++;
    }

    idx  += অঙ্ক_in;
    out_len += অঙ্ক_out;
    প্রত্যাগম সত্য;
  }

  HB_NODISCARD তার্কিক replace_glyph (hb_codepoint_t glyph_index)
  { প্রত্যাগম replace_glyphs (1, 1, &glyph_index); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD তার্কিক output_glyph (hb_codepoint_t glyph_index)
  { প্রত্যাগম replace_glyphs (0, 1, &glyph_index); }

  HB_NODISCARD তার্কিক output_info (ধ্রুব hb_glyph_info_t &glyph_info)
  {
    যদি (unlikely (!make_room_for (0, 1))) প্রত্যাগম অসত্য;

    out_info[out_len] = glyph_info;

    out_len++;
    প্রত্যাগম সত্য;
  }
  /* Copies glyph at idx to output but doesn't advance idx */
  HB_NODISCARD তার্কিক copy_glyph ()
  {
    /* Extra copy because cur()'s return can be freed within
     * output_info() call if buffer reallocates. */
    প্রত্যাগম output_info (hb_glyph_info_t (cur()));
  }

  /* Copies glyph at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD তার্কিক next_glyph ()
  {
    যদি (have_output)
    {
      যদি (out_info != info || out_len != idx)
      {
	যদি (unlikely (!ensure (out_len + 1))) প্রত্যাগম অসত্য;
	out_info[out_len] = info[idx];
      }
      out_len++;
    }

    idx++;
    প্রত্যাগম সত্য;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD তার্কিক next_glyphs (unsigned পূর্ণাঙ্ক n)
  {
    যদি (have_output)
    {
      যদি (out_info != info || out_len != idx)
      {
	যদি (unlikely (!ensure (out_len + n))) প্রত্যাগম অসত্য;
	memmove (out_info + out_len, info + idx, n * পরিমাণ (out_info[0]));
      }
      out_len += n;
    }

    idx += n;
    প্রত্যাগম সত্য;
  }
  /* Advance idx without copying to output. */
  শূন্য skip_glyph () { idx++; }
  শূন্য add_masks (hb_mask_t mask)
  {
    প্রতি (unsigned পূর্ণাঙ্ক j = 0; j < len; j++)
      info[j].mask |= mask;
  }
  HB_INTERNAL শূন্য set_masks (hb_mask_t value, hb_mask_t mask,
			      unsigned পূর্ণাঙ্ক cluster_start, unsigned পূর্ণাঙ্ক cluster_end);

  শূন্য merge_clusters (unsigned পূর্ণাঙ্ক start, unsigned পূর্ণাঙ্ক end)
  {
    যদি (end - start < 2)
      প্রত্যাগম;
    যদি (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_to_break (start, end);
      প্রত্যাগম;
    }
    merge_clusters_impl (start, end);
  }
  শূন্য merge_grapheme_clusters (unsigned পূর্ণাঙ্ক start, unsigned পূর্ণাঙ্ক end)
  {
    যদি (end - start < 2)
      প্রত্যাগম;
    যদি (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_to_break (start, end);
      প্রত্যাগম;
    }
    merge_clusters_impl (start, end);
  }
  HB_INTERNAL শূন্য merge_clusters_impl (unsigned পূর্ণাঙ্ক start, unsigned পূর্ণাঙ্ক end);
  শূন্য merge_out_clusters (unsigned পূর্ণাঙ্ক start, unsigned পূর্ণাঙ্ক end)
  {
    যদি (end - start < 2)
      প্রত্যাগম;
    যদি (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      প্রত্যাগম;
    merge_out_clusters_impl (start, end);
  }
  শূন্য merge_out_grapheme_clusters (unsigned পূর্ণাঙ্ক start, unsigned পূর্ণাঙ্ক end)
  {
    যদি (end - start < 2)
      প্রত্যাগম;
    যদি (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      প্রত্যাগম;
    merge_out_clusters_impl (start, end);
  }
  HB_INTERNAL শূন্য merge_out_clusters_impl (unsigned পূর্ণাঙ্ক start, unsigned পূর্ণাঙ্ক end);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_INTERNAL শূন্য delete_glyph ();
  HB_INTERNAL শূন্য delete_glyphs_inplace (তার্কিক (*filter) (ধ্রুব hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  শূন্য _set_glyph_flags_impl (hb_mask_t mask,
			      unsigned start,
			      unsigned end,
			      তার্কিক interior,
			      তার্কিক from_out_buffer)
  {
    যদি (!from_out_buffer || !have_output)
    {
      যদি (!interior)
      {
	প্রতি (unsigned i = start; i < end; i++)
	  info[i].mask |= mask;
      }
      অন্যথা
      {
	unsigned cluster = _infos_find_min_cluster (info, start, end);
	_infos_set_glyph_flags (info, start, end, cluster, mask);
      }
    }
    অন্যথা
    {
      প্রতিজ্ঞা (start <= out_len);
      প্রতিজ্ঞা (idx <= end);

      যদি (!interior)
      {
	প্রতি (unsigned i = start; i < out_len; i++)
	  out_info[i].mask |= mask;
	প্রতি (unsigned i = idx; i < end; i++)
	  info[i].mask |= mask;
      }
      অন্যথা
      {
	unsigned cluster = _infos_find_min_cluster (info, idx, end);
	cluster = _infos_find_min_cluster (out_info, start, out_len, cluster);

	_infos_set_glyph_flags (out_info, start, out_len, cluster, mask);
	_infos_set_glyph_flags (info, idx, end, cluster, mask);
      }
    }
  }

  HB_ALWAYS_INLINE
  শূন্য _set_glyph_flags (hb_mask_t mask,
			 unsigned start = 0,
			 unsigned end = (unsigned) -1,
			 তার্কিক interior = অসত্য,
			 তার্কিক from_out_buffer = অসত্য)
  {
    যদি (unlikely (end != (unsigned) -1 && end - start > 255))
      প্রত্যাগম;

    end = hb_min (end, len);

    যদি (interior && !from_out_buffer && end - start < 2)
      প্রত্যাগম;

    _set_glyph_flags_impl (mask, start, end, interior, from_out_buffer);
  }


  শূন্য unsafe_to_break (unsigned পূর্ণাঙ্ক start = 0, unsigned পূর্ণাঙ্ক end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      সত্য);
  }
  শূন্য safe_to_ঢোকাও_tatweel (unsigned পূর্ণাঙ্ক start = 0, unsigned পূর্ণাঙ্ক end = -1)
  {
    যদি ((flags & HB_BUFFER_FLAG_PRODUCE_SAFE_TO_ঢোকাও_TATWEEL) == 0)
    {
      unsafe_to_break (start, end);
      প্রত্যাগম;
    }
    _set_glyph_flags (HB_GLYPH_FLAG_SAFE_TO_ঢোকাও_TATWEEL,
		      start, end,
		      সত্য);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  শূন্য unsafe_to_concat (unsigned পূর্ণাঙ্ক start = 0, unsigned পূর্ণাঙ্ক end = -1)
  {
    যদি (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      প্রত্যাগম;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      অসত্য);
  }
  শূন্য unsafe_to_break_from_outbuffer (unsigned পূর্ণাঙ্ক start = 0, unsigned পূর্ণাঙ্ক end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      সত্য, সত্য);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  শূন্য unsafe_to_concat_from_outbuffer (unsigned পূর্ণাঙ্ক start = 0, unsigned পূর্ণাঙ্ক end = -1)
  {
    যদি (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      প্রত্যাগম;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      অসত্য, সত্য);
  }


  /* Internal methods */
  HB_NODISCARD HB_INTERNAL তার্কিক move_to (unsigned পূর্ণাঙ্ক i); /* i is output-buffer index. */

  HB_NODISCARD HB_INTERNAL তার্কিক enlarge (unsigned পূর্ণাঙ্ক size);

  HB_NODISCARD তার্কিক resize (unsigned length)
  {
    প্রতিজ্ঞা (!have_output);
    যদি (unlikely (!ensure (length))) প্রত্যাগম অসত্য;
    len = length;
    প্রত্যাগম সত্য;
  }
  HB_NODISCARD তার্কিক ensure (unsigned পূর্ণাঙ্ক size)
  { প্রত্যাগম likely (!size || size < allocated) ? সত্য : enlarge (size); }

  HB_NODISCARD তার্কিক ensure_inplace (unsigned পূর্ণাঙ্ক size)
  { প্রত্যাগম likely (!size || size < allocated); }

  শূন্য assert_glyphs ()
  {
    প্রতিজ্ঞা ((content_type == HB_BUFFER_CONTENT_TYPE_GLYPHS) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  শূন্য assert_unicode ()
  {
    প্রতিজ্ঞা ((content_type == HB_BUFFER_CONTENT_TYPE_UNICODE) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  HB_NODISCARD তার্কিক ensure_glyphs ()
  {
    যদি (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_GLYPHS))
    {
      যদি (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	প্রত্যাগম অসত্য;
      প্রতিজ্ঞা (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_GLYPHS;
    }
    প্রত্যাগম সত্য;
  }
  HB_NODISCARD তার্কিক ensure_unicode ()
  {
    যদি (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_UNICODE))
    {
      যদি (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	প্রত্যাগম অসত্য;
      প্রতিজ্ঞা (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_UNICODE;
    }
    প্রত্যাগম সত্য;
  }

  HB_NODISCARD HB_INTERNAL তার্কিক make_room_for (unsigned পূর্ণাঙ্ক অঙ্ক_in, unsigned পূর্ণাঙ্ক অঙ্ক_out);
  HB_NODISCARD HB_INTERNAL তার্কিক shift_forward (unsigned পূর্ণাঙ্ক গুনতি);

  প্রকারনির্দেশ long scratch_buffer_t;
  HB_INTERNAL scratch_buffer_t *get_scratch_buffer (unsigned পূর্ণাঙ্ক *size);

  শূন্য clear_context (unsigned পূর্ণাঙ্ক side) { context_len[side] = 0; }

  HB_INTERNAL শূন্য sort (unsigned পূর্ণাঙ্ক start, unsigned পূর্ণাঙ্ক end, পূর্ণাঙ্ক(*compar)(ধ্রুব hb_glyph_info_t *, ধ্রুব hb_glyph_info_t *));

  তার্কিক messaging ()
  {
#ifdef HB_NO_BUFFER_MESSAGE
    প্রত্যাগম অসত্য;
#অন্যথা
    প্রত্যাগম unlikely (message_func);
#endif
  }
  তার্কিক message (hb_font_t *font, ধ্রুব বর্ণ *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#ifdef HB_NO_BUFFER_MESSAGE
    প্রত্যাগম সত্য;
#অন্যথা
    যদি (likely (!messaging ()))
      প্রত্যাগম সত্য;

    va_list ap;
    va_start (ap, fmt);
    তার্কিক ret = message_impl (font, fmt, ap);
    va_end (ap);

    প্রত্যাগম ret;
#endif
  }
  HB_INTERNAL তার্কিক message_impl (hb_font_t *font, ধ্রুব বর্ণ *fmt, va_list ap) HB_PRINTF_FUNC(3, 0);
  HB_INTERNAL শূন্য changed ();

  স্থির শূন্য
  set_cluster (hb_glyph_info_t &inf, unsigned পূর্ণাঙ্ক cluster, unsigned পূর্ণাঙ্ক mask = 0)
  {
    যদি (inf.cluster != cluster)
      inf.mask = (inf.mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  শূন্য
  _infos_set_glyph_flags (hb_glyph_info_t *infos,
			  unsigned পূর্ণাঙ্ক start, unsigned পূর্ণাঙ্ক end,
			  unsigned পূর্ণাঙ্ক cluster,
			  hb_mask_t mask)
  {
    যদি (unlikely (start == end))
      প্রত্যাগম;

    max_ops -= end - start;
    যদি (unlikely (max_ops < 0))
      successful = অসত্য;

    unsigned cluster_first = infos[start].cluster;
    unsigned cluster_last = infos[end - 1].cluster;

    যদি (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_first && cluster != cluster_last))
    {
      প্রতি (unsigned পূর্ণাঙ্ক i = start; i < end; i++)
	যদি (cluster != infos[i].cluster)
	  infos[i].mask |= mask;
      প্রত্যাগম;
    }

    /* Monotone clusters */

    যদি (cluster == cluster_first)
    {
      প্রতি (unsigned পূর্ণাঙ্ক i = end; start < i && infos[i - 1].cluster != cluster_first; i--)
	infos[i - 1].mask |= mask;
    }
    অন্যথা /* cluster == cluster_last */
    {
      প্রতি (unsigned পূর্ণাঙ্ক i = start; i < end && infos[i].cluster != cluster_last; i++)
	infos[i].mask |= mask;
    }
  }
  unsigned
  _infos_find_min_cluster (ধ্রুব hb_glyph_info_t *infos,
			   unsigned start, unsigned end,
			   unsigned cluster = UINT_MAX)
  {
    যদি (unlikely (start == end))
      প্রত্যাগম cluster;

    যদি (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS)
    {
      প্রতি (unsigned পূর্ণাঙ্ক i = start; i < end; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      প্রত্যাগম cluster;
    }

    প্রত্যাগম hb_min (cluster, hb_min (infos[start].cluster, infos[end - 1].cluster));
  }

  শূন্য clear_glyph_flags (hb_mask_t mask = 0)
  {
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < len; i++)
      info[i].mask = (info[i].mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_buffer_t);


#define foreach_group(buffer, start, end, group_func) \
  প্রতি (unsigned পূর্ণাঙ্ক \
       গুনতি_2 = buffer->len, \
       start = 0, end = গুনতি_2 ? buffer->group_end (0, group_func) : 0; \
       start < গুনতি_2; \
       start = end, end = buffer->group_end (start, group_func))

#define foreach_cluster(buffer, start, end) \
	foreach_group (buffer, start, end, hb_buffer_t::_cluster_group_func)


#define HB_BUFFER_XALLOCATE_VAR(b, func, var) \
  b->func (offsetof (hb_glyph_info_t, var) - offsetof(hb_glyph_info_t, var1), \
	   পরিমাণ (b->info[0].var))
#define HB_BUFFER_ALLOCATE_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, allocate_var,     var ())
#define HB_BUFFER_TRY_ALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, try_allocate_var, var ())
#define HB_BUFFER_DEALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, deallocate_var,   var ())
#define HB_BUFFER_ASSERT_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, assert_var,       var ())


#endif /* HB_BUFFER_HH */
