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

#ifndef HB_বাফার_HH
#define HB_বাফার_HH

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-unicode.hh"
#অন্তর্ভাব "hb-set-digest.hh"


static_assert ((আকার (hb_glyph_info_t) == 20), "");
static_assert ((আকার (hb_glyph_info_t) == আকার (hb_glyph_position_t)), "");

HB_MARK_AS_FLAG_T (hb_glyph_flags_t);
HB_MARK_AS_FLAG_T (hb_বাফার_flags_t);
HB_MARK_AS_FLAG_T (hb_বাফার_serialize_flags_t);
HB_MARK_AS_FLAG_T (hb_বাফার_diff_flags_t);

গণনা hb_বাফার_scratch_flags_t {
  HB_বাফার_SCRATCH_FLAG_DEFAULT			= 0x00000000u,
  HB_বাফার_SCRATCH_FLAG_HAS_FRACTION_SLASH		= 0x00000001u,
  HB_বাফার_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES		= 0x00000002u,
  HB_বাফার_SCRATCH_FLAG_HAS_SPACE_FALLBACK		= 0x00000004u,
  HB_বাফার_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT		= 0x00000008u,
  HB_বাফার_SCRATCH_FLAG_HAS_CGJ			= 0x00000010u,
  HB_বাফার_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE		= 0x00000020u,
  HB_বাফার_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK= 0x00000040u,
  HB_বাফার_SCRATCH_FLAG_HAS_CONTINUATIONS		= 0x00000080u,

  /* Reserved for shapers' internal use. */
  HB_বাফার_SCRATCH_FLAG_SHAPER0			= 0x01000000u,
  HB_বাফার_SCRATCH_FLAG_SHAPER1			= 0x02000000u,
  HB_বাফার_SCRATCH_FLAG_SHAPER2			= 0x04000000u,
  HB_বাফার_SCRATCH_FLAG_SHAPER3			= 0x08000000u,
};
HB_MARK_AS_FLAG_T (hb_বাফার_scratch_flags_t);


/*
 * hb_buffer_t
 */

কাঠামো hb_বাফার_t
{
  hb_object_header_t header;

  /*
   * Information about how the text in the buffer should be treated.
   */

  hb_unicode_funcs_t *unicode; /* Unicode functions */
  hb_বাফার_flags_t flags; /* BOT / EOT / etc. */
  hb_বাফার_cluster_level_t cluster_level;
  hb_codepoint_t replacement; /* U+FFFD or something else. */
  hb_codepoint_t invisible; /* 0 or something else. */
  hb_codepoint_t not_found; /* 0 or something else. */
  hb_codepoint_t not_found_variation_selector; /* HB_CODEPOINT_INVALID or something else. */

  /*
   * Buffer contents
   */

  hb_বাফার_content_type_t content_type;
  hb_segment_properties_t props; /* Script, language, direction */

  যৌক্তিক successful; /* Allocations successful */
  যৌক্তিক have_output; /* Whether we have an output buffer going on */
  যৌক্তিক have_positions; /* Whether we have positions */

  unsigned পূর্ণসংখ্যা idx; /* Cursor into ->info and ->pos arrays */
  unsigned পূর্ণসংখ্যা len; /* Length of ->info and ->pos arrays */
  unsigned পূর্ণসংখ্যা out_len; /* Length of ->out_info array if have_output */

  unsigned পূর্ণসংখ্যা allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *out_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  স্থির constexpr unsigned CONTEXT_দৈর্ঘ্য = 5u;
  hb_codepoint_t context[2][CONTEXT_দৈর্ঘ্য];
  unsigned পূর্ণসংখ্যা context_len[2];

  hb_set_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_bits;
  uint8_t serial;
  uint32_t random_state;
  hb_বাফার_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  unsigned পূর্ণসংখ্যা max_len; /* Maximum allowed len. */
  পূর্ণসংখ্যা max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#ifndef HB_NO_বাফার_MESSAGE
  প্রকারলেখ শূন্য (*changed_func_t) (hb_বাফার_t *বাফার, শূন্য *user_তথ্য);

  hb_বাফার_message_func_t message_func;
  শূন্য *message_তথ্য;
  hb_destroy_func_t message_destroy;
  changed_func_t changed_func;
  শূন্য *changed_তথ্য;
  unsigned message_depth; /* How deeply are we inside a message callback? */
#অন্যথা
  স্থির constexpr unsigned message_depth = 0u;
#endif



  /* Methods */

  HB_NODISCARD যৌক্তিক in_error () ধ্রুবক { ফেরত !successful; }

  শূন্য allocate_var (unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা গুনতি)
  {
    unsigned পূর্ণসংখ্যা end = start + গুনতি;
    প্রতিজ্ঞা (end <= 8);
    unsigned পূর্ণসংখ্যা bits = (1u<<end) - (1u<<start);
    প্রতিজ্ঞা (0 == (allocated_var_bits & bits));
    allocated_var_bits |= bits;
  }
  যৌক্তিক try_allocate_var (unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা গুনতি)
  {
    unsigned পূর্ণসংখ্যা end = start + গুনতি;
    প্রতিজ্ঞা (end <= 8);
    unsigned পূর্ণসংখ্যা bits = (1u<<end) - (1u<<start);
    যদি (allocated_var_bits & bits)
      ফেরত মিথ্যা;
    allocated_var_bits |= bits;
    ফেরত সত্য;
  }
  শূন্য deallocate_var (unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা গুনতি)
  {
    unsigned পূর্ণসংখ্যা end = start + গুনতি;
    প্রতিজ্ঞা (end <= 8);
    unsigned পূর্ণসংখ্যা bits = (1u<<end) - (1u<<start);
    প্রতিজ্ঞা (bits == (allocated_var_bits & bits));
    allocated_var_bits &= ~bits;
  }
  শূন্য assert_var (unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা গুনতি)
  {
    unsigned পূর্ণসংখ্যা end = start + গুনতি;
    প্রতিজ্ঞা (end <= 8);
    HB_UNUSED unsigned পূর্ণসংখ্যা bits = (1u<<end) - (1u<<start);
    প্রতিজ্ঞা (bits == (allocated_var_bits & bits));
  }
  শূন্য deallocate_var_all ()
  {
    allocated_var_bits = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (unsigned পূর্ণসংখ্যা i = 0) { ফেরত info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (unsigned পূর্ণসংখ্যা i = 0) ধ্রুবক { ফেরত info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (unsigned পূর্ণসংখ্যা i = 0) { ফেরত pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (unsigned পূর্ণসংখ্যা i = 0) ধ্রুবক { ফেরত pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { ফেরত out_info[out_len ? out_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () ধ্রুবক { ফেরত out_info[out_len ? out_len - 1 : 0]; }

  template <typename set_t>
  শূন্য collect_codepoints (set_t &d) ধ্রুবক
  { d.clear (); d.add_array (&info[0].codepoint, len, আকার (info[0])); }

  শূন্য update_digest ()
  {
    digest = hb_set_digest_t ();
    collect_codepoints (digest);
  }

  HB_INTERNAL শূন্য similar (ধ্রুবক hb_বাফার_t &src);
  HB_INTERNAL শূন্য reset ();
  HB_INTERNAL শূন্য clear ();

  /* Called around shape() */
  HB_INTERNAL শূন্য enter ();
  HB_INTERNAL শূন্য leave ();

#ifndef HB_NO_বাফার_VERIFY
  HB_INTERNAL
#endif
  যৌক্তিক verify (hb_বাফার_t        *text_বাফার,
	       hb_font_t          *font,
	       ধ্রুবক hb_feature_t *features,
	       unsigned পূর্ণসংখ্যা        অঙ্ক_features,
	       ধ্রুবক অক্ষর * ধ্রুবক *shapers)
#ifndef HB_NO_বাফার_VERIFY
  ;
#অন্যথা
  { ফেরত সত্য; }
#endif

  unsigned পূর্ণসংখ্যা backtrack_len () ধ্রুবক { ফেরত have_output ? out_len : idx; }
  unsigned পূর্ণসংখ্যা lookahead_len () ধ্রুবক { ফেরত len - idx; }
  uint8_t পরবর্তী_serial () { ফেরত ++serial ? serial : ++serial; }

  HB_INTERNAL শূন্য add (hb_codepoint_t  codepoint,
			unsigned পূর্ণসংখ্যা    cluster);
  HB_INTERNAL শূন্য add_info (ধ্রুবক hb_glyph_info_t &glyph_info);
  HB_INTERNAL শূন্য add_info_and_pos (ধ্রুবক hb_glyph_info_t &glyph_info,
				     ধ্রুবক hb_glyph_position_t &glyph_pos);

  শূন্য reverse_range (unsigned start, unsigned end)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (start, end);
    যদি (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (start, end);
  }
  শূন্য reverse () { reverse_range (0, len); }

  template <typename FuncType>
  শূন্য reverse_groups (ধ্রুবক FuncType& group,
		       যৌক্তিক merge_clusters = মিথ্যা)
  {
    যদি (unlikely (!len))
      ফেরত;

    unsigned start = 0;
    unsigned i;
    জন্য (i = 1; i < len; i++)
    {
      যদি (!group (info[i - 1], info[i]))
      {
	যদি (merge_clusters)
	  এই->merge_clusters (start, i);
	reverse_range (start, i);
	start = i;
      }
    }
    যদি (merge_clusters)
      এই->merge_clusters (start, i);
    reverse_range (start, i);

    reverse ();
  }

  template <typename FuncType>
  unsigned group_end (unsigned start, ধ্রুবক FuncType& group) ধ্রুবক
  {
    যখন (++start < len && group (info[start - 1], info[start]))
      ;

    ফেরত start;
  }

  স্থির যৌক্তিক _cluster_group_func (ধ্রুবক hb_glyph_info_t& a,
				   ধ্রুবক hb_glyph_info_t& b)
  { ফেরত a.cluster == b.cluster; }

  শূন্য reverse_clusters () { reverse_groups (_cluster_group_func); }

  HB_INTERNAL শূন্য guess_segment_properties ();

  HB_INTERNAL যৌক্তিক সমলয় ();
  HB_INTERNAL পূর্ণসংখ্যা sync_so_far ();
  HB_INTERNAL শূন্য clear_output ();
  HB_INTERNAL শূন্য clear_positions ();

  template <typename T>
  HB_NODISCARD যৌক্তিক replace_glyphs (unsigned পূর্ণসংখ্যা অঙ্ক_in,
				    unsigned পূর্ণসংখ্যা অঙ্ক_out,
				    ধ্রুবক T *glyph_তথ্য)
  {
    যদি (unlikely (!make_room_for (অঙ্ক_in, অঙ্ক_out))) ফেরত মিথ্যা;

    প্রতিজ্ঞা (idx + অঙ্ক_in <= len);

    merge_clusters (idx, idx + অঙ্ক_in);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &out_info[out_len];
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_out; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_তথ্য[i];
      pinfo++;
    }

    idx  += অঙ্ক_in;
    out_len += অঙ্ক_out;
    ফেরত সত্য;
  }

  HB_NODISCARD যৌক্তিক replace_glyph (hb_codepoint_t glyph_সূচক)
  { ফেরত replace_glyphs (1, 1, &glyph_সূচক); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD যৌক্তিক output_glyph (hb_codepoint_t glyph_সূচক)
  { ফেরত replace_glyphs (0, 1, &glyph_সূচক); }

  HB_NODISCARD যৌক্তিক output_info (ধ্রুবক hb_glyph_info_t &glyph_info)
  {
    যদি (unlikely (!make_room_for (0, 1))) ফেরত মিথ্যা;

    out_info[out_len] = glyph_info;

    out_len++;
    ফেরত সত্য;
  }
  /* Copies glyph at idx to output but doesn't advance idx */
  HB_NODISCARD যৌক্তিক copy_glyph ()
  {
    /* Extra copy because cur()'s return can be freed within
     * output_info() call if buffer reallocates. */
    ফেরত output_info (hb_glyph_info_t (cur()));
  }

  /* Copies glyph at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD যৌক্তিক পরবর্তী_glyph ()
  {
    যদি (have_output)
    {
      যদি (out_info != info || out_len != idx)
      {
	যদি (unlikely (!ensure (out_len + 1))) ফেরত মিথ্যা;
	out_info[out_len] = info[idx];
      }
      out_len++;
    }

    idx++;
    ফেরত সত্য;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD যৌক্তিক পরবর্তী_glyphs (unsigned পূর্ণসংখ্যা n)
  {
    যদি (have_output)
    {
      যদি (out_info != info || out_len != idx)
      {
	যদি (unlikely (!ensure (out_len + n))) ফেরত মিথ্যা;
	memmove (out_info + out_len, info + idx, n * আকার (out_info[0]));
      }
      out_len += n;
    }

    idx += n;
    ফেরত সত্য;
  }
  /* Advance idx without copying to output. */
  শূন্য skip_glyph () { idx++; }
  শূন্য add_masks (hb_mask_t mask)
  {
    জন্য (unsigned পূর্ণসংখ্যা j = 0; j < len; j++)
      info[j].mask |= mask;
  }
  HB_INTERNAL শূন্য set_masks (hb_mask_t value, hb_mask_t mask,
			      unsigned পূর্ণসংখ্যা cluster_start, unsigned পূর্ণসংখ্যা cluster_end);

  শূন্য merge_clusters (unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা end)
  {
    যদি (end - start < 2)
      ফেরত;
    যদি (!HB_বাফার_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_to_break (start, end);
      ফেরত;
    }
    merge_clusters_impl (start, end);
  }
  শূন্য merge_grapheme_clusters (unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা end)
  {
    যদি (end - start < 2)
      ফেরত;
    যদি (!HB_বাফার_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_to_break (start, end);
      ফেরত;
    }
    merge_clusters_impl (start, end);
  }
  HB_INTERNAL শূন্য merge_clusters_impl (unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা end);
  শূন্য merge_out_clusters (unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা end)
  {
    যদি (end - start < 2)
      ফেরত;
    যদি (!HB_বাফার_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      ফেরত;
    merge_out_clusters_impl (start, end);
  }
  শূন্য merge_out_grapheme_clusters (unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা end)
  {
    যদি (end - start < 2)
      ফেরত;
    যদি (!HB_বাফার_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      ফেরত;
    merge_out_clusters_impl (start, end);
  }
  HB_INTERNAL শূন্য merge_out_clusters_impl (unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা end);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_INTERNAL শূন্য delete_glyph ();
  HB_INTERNAL শূন্য delete_glyphs_inplace (যৌক্তিক (*filter) (ধ্রুবক hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  শূন্য _set_glyph_flags_impl (hb_mask_t mask,
			      unsigned start,
			      unsigned end,
			      যৌক্তিক interior,
			      যৌক্তিক from_out_বাফার)
  {
    যদি (!from_out_বাফার || !have_output)
    {
      যদি (!interior)
      {
	জন্য (unsigned i = start; i < end; i++)
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
	জন্য (unsigned i = start; i < out_len; i++)
	  out_info[i].mask |= mask;
	জন্য (unsigned i = idx; i < end; i++)
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
			 যৌক্তিক interior = মিথ্যা,
			 যৌক্তিক from_out_বাফার = মিথ্যা)
  {
    যদি (unlikely (end != (unsigned) -1 && end - start > 255))
      ফেরত;

    end = hb_min (end, len);

    যদি (interior && !from_out_বাফার && end - start < 2)
      ফেরত;

    _set_glyph_flags_impl (mask, start, end, interior, from_out_বাফার);
  }


  শূন্য unsafe_to_break (unsigned পূর্ণসংখ্যা start = 0, unsigned পূর্ণসংখ্যা end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      সত্য);
  }
  শূন্য safe_to_ঢোকাও_tatweel (unsigned পূর্ণসংখ্যা start = 0, unsigned পূর্ণসংখ্যা end = -1)
  {
    যদি ((flags & HB_বাফার_FLAG_PRODUCE_SAFE_TO_ঢোকাও_TATWEEL) == 0)
    {
      unsafe_to_break (start, end);
      ফেরত;
    }
    _set_glyph_flags (HB_GLYPH_FLAG_SAFE_TO_ঢোকাও_TATWEEL,
		      start, end,
		      সত্য);
  }
#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  শূন্য unsafe_to_concat (unsigned পূর্ণসংখ্যা start = 0, unsigned পূর্ণসংখ্যা end = -1)
  {
    যদি (likely ((flags & HB_বাফার_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      ফেরত;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      মিথ্যা);
  }
  শূন্য unsafe_to_break_from_outbuffer (unsigned পূর্ণসংখ্যা start = 0, unsigned পূর্ণসংখ্যা end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      সত্য, সত্য);
  }
#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  শূন্য unsafe_to_concat_from_outbuffer (unsigned পূর্ণসংখ্যা start = 0, unsigned পূর্ণসংখ্যা end = -1)
  {
    যদি (likely ((flags & HB_বাফার_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      ফেরত;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      মিথ্যা, সত্য);
  }


  /* Internal methods */
  HB_NODISCARD HB_INTERNAL যৌক্তিক move_to (unsigned পূর্ণসংখ্যা i); /* i is output-buffer index. */

  HB_NODISCARD HB_INTERNAL যৌক্তিক enlarge (unsigned পূর্ণসংখ্যা আকার_2);

  HB_NODISCARD যৌক্তিক resize (unsigned দৈর্ঘ্য)
  {
    প্রতিজ্ঞা (!have_output);
    যদি (unlikely (!ensure (দৈর্ঘ্য))) ফেরত মিথ্যা;
    len = দৈর্ঘ্য;
    ফেরত সত্য;
  }
  HB_NODISCARD যৌক্তিক ensure (unsigned পূর্ণসংখ্যা আকার_2)
  { ফেরত likely (!আকার_2 || আকার_2 < allocated) ? সত্য : enlarge (আকার_2); }

  HB_NODISCARD যৌক্তিক ensure_inplace (unsigned পূর্ণসংখ্যা আকার_2)
  { ফেরত likely (!আকার_2 || আকার_2 < allocated); }

  শূন্য assert_glyphs ()
  {
    প্রতিজ্ঞা ((content_type == HB_বাফার_CONTENT_TYPE_GLYPHS) ||
	    (!len && (content_type == HB_বাফার_CONTENT_TYPE_INVALID)));
  }
  শূন্য assert_unicode ()
  {
    প্রতিজ্ঞা ((content_type == HB_বাফার_CONTENT_TYPE_UNICODE) ||
	    (!len && (content_type == HB_বাফার_CONTENT_TYPE_INVALID)));
  }
  HB_NODISCARD যৌক্তিক ensure_glyphs ()
  {
    যদি (unlikely (content_type != HB_বাফার_CONTENT_TYPE_GLYPHS))
    {
      যদি (content_type != HB_বাফার_CONTENT_TYPE_INVALID)
	ফেরত মিথ্যা;
      প্রতিজ্ঞা (len == 0);
      content_type = HB_বাফার_CONTENT_TYPE_GLYPHS;
    }
    ফেরত সত্য;
  }
  HB_NODISCARD যৌক্তিক ensure_unicode ()
  {
    যদি (unlikely (content_type != HB_বাফার_CONTENT_TYPE_UNICODE))
    {
      যদি (content_type != HB_বাফার_CONTENT_TYPE_INVALID)
	ফেরত মিথ্যা;
      প্রতিজ্ঞা (len == 0);
      content_type = HB_বাফার_CONTENT_TYPE_UNICODE;
    }
    ফেরত সত্য;
  }

  HB_NODISCARD HB_INTERNAL যৌক্তিক make_room_for (unsigned পূর্ণসংখ্যা অঙ্ক_in, unsigned পূর্ণসংখ্যা অঙ্ক_out);
  HB_NODISCARD HB_INTERNAL যৌক্তিক shift_forward (unsigned পূর্ণসংখ্যা গুনতি);

  প্রকারলেখ long scratch_বাফার_t;
  HB_INTERNAL scratch_বাফার_t *get_scratch_বাফার (unsigned পূর্ণসংখ্যা *আকার_2);

  শূন্য clear_context (unsigned পূর্ণসংখ্যা side) { context_len[side] = 0; }

  HB_INTERNAL শূন্য sort (unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা end, পূর্ণসংখ্যা(*compar)(ধ্রুবক hb_glyph_info_t *, ধ্রুবক hb_glyph_info_t *));

  যৌক্তিক messaging ()
  {
#ifdef HB_NO_বাফার_MESSAGE
    ফেরত মিথ্যা;
#অন্যথা
    ফেরত unlikely (message_func);
#endif
  }
  যৌক্তিক message (hb_font_t *font, ধ্রুবক অক্ষর *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#ifdef HB_NO_বাফার_MESSAGE
    ফেরত সত্য;
#অন্যথা
    যদি (likely (!messaging ()))
      ফেরত সত্য;

    va_list ap;
    va_start (ap, fmt);
    যৌক্তিক ret = message_impl (font, fmt, ap);
    va_end (ap);

    ফেরত ret;
#endif
  }
  HB_INTERNAL যৌক্তিক message_impl (hb_font_t *font, ধ্রুবক অক্ষর *fmt, va_list ap) HB_PRINTF_FUNC(3, 0);
  HB_INTERNAL শূন্য changed ();

  স্থির শূন্য
  set_cluster (hb_glyph_info_t &inf, unsigned পূর্ণসংখ্যা cluster, unsigned পূর্ণসংখ্যা mask = 0)
  {
    যদি (inf.cluster != cluster)
      inf.mask = (inf.mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  শূন্য
  _infos_set_glyph_flags (hb_glyph_info_t *infos,
			  unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা end,
			  unsigned পূর্ণসংখ্যা cluster,
			  hb_mask_t mask)
  {
    যদি (unlikely (start == end))
      ফেরত;

    max_ops -= end - start;
    যদি (unlikely (max_ops < 0))
      successful = মিথ্যা;

    unsigned cluster_first = infos[start].cluster;
    unsigned cluster_last = infos[end - 1].cluster;

    যদি (cluster_level == HB_বাফার_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_first && cluster != cluster_last))
    {
      জন্য (unsigned পূর্ণসংখ্যা i = start; i < end; i++)
	যদি (cluster != infos[i].cluster)
	  infos[i].mask |= mask;
      ফেরত;
    }

    /* Monotone clusters */

    যদি (cluster == cluster_first)
    {
      জন্য (unsigned পূর্ণসংখ্যা i = end; start < i && infos[i - 1].cluster != cluster_first; i--)
	infos[i - 1].mask |= mask;
    }
    অন্যথা /* cluster == cluster_last */
    {
      জন্য (unsigned পূর্ণসংখ্যা i = start; i < end && infos[i].cluster != cluster_last; i++)
	infos[i].mask |= mask;
    }
  }
  unsigned
  _infos_find_min_cluster (ধ্রুবক hb_glyph_info_t *infos,
			   unsigned start, unsigned end,
			   unsigned cluster = UINT_MAX)
  {
    যদি (unlikely (start == end))
      ফেরত cluster;

    যদি (cluster_level == HB_বাফার_CLUSTER_LEVEL_CHARACTERS)
    {
      জন্য (unsigned পূর্ণসংখ্যা i = start; i < end; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      ফেরত cluster;
    }

    ফেরত hb_min (cluster, hb_min (infos[start].cluster, infos[end - 1].cluster));
  }

  শূন্য clear_glyph_flags (hb_mask_t mask = 0)
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < len; i++)
      info[i].mask = (info[i].mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_বাফার_t);


#define foreach_group(বাফার, start, end, group_func) \
  জন্য (unsigned পূর্ণসংখ্যা \
       গুনতি_2 = বাফার->len, \
       start = 0, end = গুনতি_2 ? বাফার->group_end (0, group_func) : 0; \
       start < গুনতি_2; \
       start = end, end = বাফার->group_end (start, group_func))

#define foreach_cluster(বাফার, start, end) \
	foreach_group (বাফার, start, end, hb_বাফার_t::_cluster_group_func)


#define HB_বাফার_XALLOCATE_VAR(b, func, var) \
  b->func (offsetof (hb_glyph_info_t, var) - offsetof(hb_glyph_info_t, var1), \
	   আকার (b->info[0].var))
#define HB_বাফার_ALLOCATE_VAR(b, var)		HB_বাফার_XALLOCATE_VAR (b, allocate_var,     var ())
#define HB_বাফার_TRY_ALLOCATE_VAR(b, var)	HB_বাফার_XALLOCATE_VAR (b, try_allocate_var, var ())
#define HB_বাফার_DEALLOCATE_VAR(b, var)	HB_বাফার_XALLOCATE_VAR (b, deallocate_var,   var ())
#define HB_বাফার_ASSERT_VAR(b, var)		HB_বাফার_XALLOCATE_VAR (b, assert_var,       var ())


#endif /* HB_BUFFER_HH */
