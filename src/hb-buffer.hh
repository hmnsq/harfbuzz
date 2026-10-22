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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-unicode.hh"
#அந்தர்பாவ "hb-set-digest.hh"


static_assert ((அளவு (hb_glyph_info_t) == 20), "");
static_assert ((அளவு (hb_glyph_info_t) == அளவு (hb_glyph_position_t)), "");

HB_MARK_AS_FLAG_T (hb_glyph_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_serialize_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_diff_flags_t);

எண்ணிக்கை hb_buffer_scratch_flags_t {
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

கட்டமைப்பு hb_buffer_t
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

  தருக்க successful; /* Allocations successful */
  தருக்க have_output; /* Whether we have an output buffer going on */
  தருக்க have_positions; /* Whether we have positions */

  unsigned முழு idx; /* Cursor into ->info and ->pos arrays */
  unsigned முழு len; /* Length of ->info and ->pos arrays */
  unsigned முழு out_len; /* Length of ->out_info array if have_output */

  unsigned முழு allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *out_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  நிலையான constexpr unsigned CONTEXT_LENGTH = 5u;
  hb_codepoint_t context[2][CONTEXT_LENGTH];
  unsigned முழு context_len[2];

  hb_set_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_bits;
  uint8_t serial;
  uint32_t random_state;
  hb_buffer_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  unsigned முழு max_len; /* Maximum allowed len. */
  முழு max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#ifndef HB_NO_BUFFER_MESSAGE
  வகைவரை வெறுமை (*changed_func_t) (hb_buffer_t *buffer, வெறுமை *user_data);

  hb_buffer_message_func_t message_func;
  வெறுமை *message_data;
  hb_destroy_func_t message_destroy;
  changed_func_t changed_func;
  வெறுமை *changed_data;
  unsigned message_depth; /* How deeply are we inside a message callback? */
#இல்லையெனில்
  நிலையான constexpr unsigned message_depth = 0u;
#endif



  /* Methods */

  HB_NODISCARD தருக்க in_error () மாறிலி { திரும்பு !successful; }

  வெறுமை allocate_var (unsigned முழு start, unsigned முழு மொத்தம்)
  {
    unsigned முழு end = start + மொத்தம்;
    ப்ரதிஜ்ஞா (end <= 8);
    unsigned முழு bits = (1u<<end) - (1u<<start);
    ப்ரதிஜ்ஞா (0 == (allocated_var_bits & bits));
    allocated_var_bits |= bits;
  }
  தருக்க try_allocate_var (unsigned முழு start, unsigned முழு மொத்தம்)
  {
    unsigned முழு end = start + மொத்தம்;
    ப்ரதிஜ்ஞா (end <= 8);
    unsigned முழு bits = (1u<<end) - (1u<<start);
    எனில் (allocated_var_bits & bits)
      திரும்பு பொய்;
    allocated_var_bits |= bits;
    திரும்பு உண்மை;
  }
  வெறுமை deallocate_var (unsigned முழு start, unsigned முழு மொத்தம்)
  {
    unsigned முழு end = start + மொத்தம்;
    ப்ரதிஜ்ஞா (end <= 8);
    unsigned முழு bits = (1u<<end) - (1u<<start);
    ப்ரதிஜ்ஞா (bits == (allocated_var_bits & bits));
    allocated_var_bits &= ~bits;
  }
  வெறுமை assert_var (unsigned முழு start, unsigned முழு மொத்தம்)
  {
    unsigned முழு end = start + மொத்தம்;
    ப்ரதிஜ்ஞா (end <= 8);
    HB_UNUSED unsigned முழு bits = (1u<<end) - (1u<<start);
    ப்ரதிஜ்ஞா (bits == (allocated_var_bits & bits));
  }
  வெறுமை deallocate_var_all ()
  {
    allocated_var_bits = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (unsigned முழு i = 0) { திரும்பு info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (unsigned முழு i = 0) மாறிலி { திரும்பு info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (unsigned முழு i = 0) { திரும்பு pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (unsigned முழு i = 0) மாறிலி { திரும்பு pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { திரும்பு out_info[out_len ? out_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () மாறிலி { திரும்பு out_info[out_len ? out_len - 1 : 0]; }

  template <typename set_t>
  வெறுமை collect_codepoints (set_t &d) மாறிலி
  { d.clear (); d.add_array (&info[0].codepoint, len, அளவு (info[0])); }

  வெறுமை update_digest ()
  {
    digest = hb_set_digest_t ();
    collect_codepoints (digest);
  }

  HB_INTERNAL வெறுமை similar (மாறிலி hb_buffer_t &src);
  HB_INTERNAL வெறுமை reset ();
  HB_INTERNAL வெறுமை clear ();

  /* Called around shape() */
  HB_INTERNAL வெறுமை enter ();
  HB_INTERNAL வெறுமை leave ();

#ifndef HB_NO_BUFFER_VERIFY
  HB_INTERNAL
#endif
  தருக்க verify (hb_buffer_t        *text_buffer,
	       hb_font_t          *font,
	       மாறிலி hb_feature_t *features,
	       unsigned முழு        இலக்கம்_features,
	       மாறிலி எழுத்து * மாறிலி *shapers)
#ifndef HB_NO_BUFFER_VERIFY
  ;
#இல்லையெனில்
  { திரும்பு உண்மை; }
#endif

  unsigned முழு backtrack_len () மாறிலி { திரும்பு have_output ? out_len : idx; }
  unsigned முழு lookahead_len () மாறிலி { திரும்பு len - idx; }
  uint8_t next_serial () { திரும்பு ++serial ? serial : ++serial; }

  HB_INTERNAL வெறுமை add (hb_codepoint_t  codepoint,
			unsigned முழு    cluster);
  HB_INTERNAL வெறுமை add_info (மாறிலி hb_glyph_info_t &glyph_info);
  HB_INTERNAL வெறுமை add_info_and_pos (மாறிலி hb_glyph_info_t &glyph_info,
				     மாறிலி hb_glyph_position_t &glyph_pos);

  வெறுமை reverse_range (unsigned start, unsigned end)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (start, end);
    எனில் (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (start, end);
  }
  வெறுமை reverse () { reverse_range (0, len); }

  template <typename FuncType>
  வெறுமை reverse_groups (மாறிலி FuncType& group,
		       தருக்க merge_clusters = பொய்)
  {
    எனில் (unlikely (!len))
      திரும்பு;

    unsigned start = 0;
    unsigned i;
    க்கு (i = 1; i < len; i++)
    {
      எனில் (!group (info[i - 1], info[i]))
      {
	எனில் (merge_clusters)
	  இது->merge_clusters (start, i);
	reverse_range (start, i);
	start = i;
      }
    }
    எனில் (merge_clusters)
      இது->merge_clusters (start, i);
    reverse_range (start, i);

    reverse ();
  }

  template <typename FuncType>
  unsigned group_end (unsigned start, மாறிலி FuncType& group) மாறிலி
  {
    போது (++start < len && group (info[start - 1], info[start]))
      ;

    திரும்பு start;
  }

  நிலையான தருக்க _cluster_group_func (மாறிலி hb_glyph_info_t& a,
				   மாறிலி hb_glyph_info_t& b)
  { திரும்பு a.cluster == b.cluster; }

  வெறுமை reverse_clusters () { reverse_groups (_cluster_group_func); }

  HB_INTERNAL வெறுமை guess_segment_properties ();

  HB_INTERNAL தருக்க ஒத்திசை ();
  HB_INTERNAL முழு sync_so_far ();
  HB_INTERNAL வெறுமை clear_output ();
  HB_INTERNAL வெறுமை clear_positions ();

  template <typename T>
  HB_NODISCARD தருக்க replace_glyphs (unsigned முழு இலக்கம்_in,
				    unsigned முழு இலக்கம்_out,
				    மாறிலி T *glyph_data)
  {
    எனில் (unlikely (!make_room_for (இலக்கம்_in, இலக்கம்_out))) திரும்பு பொய்;

    ப்ரதிஜ்ஞா (idx + இலக்கம்_in <= len);

    merge_clusters (idx, idx + இலக்கம்_in);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &out_info[out_len];
    க்கு (unsigned முழு i = 0; i < இலக்கம்_out; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_data[i];
      pinfo++;
    }

    idx  += இலக்கம்_in;
    out_len += இலக்கம்_out;
    திரும்பு உண்மை;
  }

  HB_NODISCARD தருக்க replace_glyph (hb_codepoint_t glyph_index)
  { திரும்பு replace_glyphs (1, 1, &glyph_index); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD தருக்க output_glyph (hb_codepoint_t glyph_index)
  { திரும்பு replace_glyphs (0, 1, &glyph_index); }

  HB_NODISCARD தருக்க output_info (மாறிலி hb_glyph_info_t &glyph_info)
  {
    எனில் (unlikely (!make_room_for (0, 1))) திரும்பு பொய்;

    out_info[out_len] = glyph_info;

    out_len++;
    திரும்பு உண்மை;
  }
  /* Copies glyph at idx to output but doesn't advance idx */
  HB_NODISCARD தருக்க copy_glyph ()
  {
    /* Extra copy because cur()'s return can be freed within
     * output_info() call if buffer reallocates. */
    திரும்பு output_info (hb_glyph_info_t (cur()));
  }

  /* Copies glyph at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD தருக்க next_glyph ()
  {
    எனில் (have_output)
    {
      எனில் (out_info != info || out_len != idx)
      {
	எனில் (unlikely (!ensure (out_len + 1))) திரும்பு பொய்;
	out_info[out_len] = info[idx];
      }
      out_len++;
    }

    idx++;
    திரும்பு உண்மை;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD தருக்க next_glyphs (unsigned முழு n)
  {
    எனில் (have_output)
    {
      எனில் (out_info != info || out_len != idx)
      {
	எனில் (unlikely (!ensure (out_len + n))) திரும்பு பொய்;
	memmove (out_info + out_len, info + idx, n * அளவு (out_info[0]));
      }
      out_len += n;
    }

    idx += n;
    திரும்பு உண்மை;
  }
  /* Advance idx without copying to output. */
  வெறுமை skip_glyph () { idx++; }
  வெறுமை add_masks (hb_mask_t mask)
  {
    க்கு (unsigned முழு j = 0; j < len; j++)
      info[j].mask |= mask;
  }
  HB_INTERNAL வெறுமை set_masks (hb_mask_t value, hb_mask_t mask,
			      unsigned முழு cluster_start, unsigned முழு cluster_end);

  வெறுமை merge_clusters (unsigned முழு start, unsigned முழு end)
  {
    எனில் (end - start < 2)
      திரும்பு;
    எனில் (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_to_break (start, end);
      திரும்பு;
    }
    merge_clusters_impl (start, end);
  }
  வெறுமை merge_grapheme_clusters (unsigned முழு start, unsigned முழு end)
  {
    எனில் (end - start < 2)
      திரும்பு;
    எனில் (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_to_break (start, end);
      திரும்பு;
    }
    merge_clusters_impl (start, end);
  }
  HB_INTERNAL வெறுமை merge_clusters_impl (unsigned முழு start, unsigned முழு end);
  வெறுமை merge_out_clusters (unsigned முழு start, unsigned முழு end)
  {
    எனில் (end - start < 2)
      திரும்பு;
    எனில் (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      திரும்பு;
    merge_out_clusters_impl (start, end);
  }
  வெறுமை merge_out_grapheme_clusters (unsigned முழு start, unsigned முழு end)
  {
    எனில் (end - start < 2)
      திரும்பு;
    எனில் (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      திரும்பு;
    merge_out_clusters_impl (start, end);
  }
  HB_INTERNAL வெறுமை merge_out_clusters_impl (unsigned முழு start, unsigned முழு end);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_INTERNAL வெறுமை delete_glyph ();
  HB_INTERNAL வெறுமை delete_glyphs_inplace (தருக்க (*filter) (மாறிலி hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  வெறுமை _set_glyph_flags_impl (hb_mask_t mask,
			      unsigned start,
			      unsigned end,
			      தருக்க interior,
			      தருக்க from_out_buffer)
  {
    எனில் (!from_out_buffer || !have_output)
    {
      எனில் (!interior)
      {
	க்கு (unsigned i = start; i < end; i++)
	  info[i].mask |= mask;
      }
      இல்லையெனில்
      {
	unsigned cluster = _infos_find_min_cluster (info, start, end);
	_infos_set_glyph_flags (info, start, end, cluster, mask);
      }
    }
    இல்லையெனில்
    {
      ப்ரதிஜ்ஞா (start <= out_len);
      ப்ரதிஜ்ஞா (idx <= end);

      எனில் (!interior)
      {
	க்கு (unsigned i = start; i < out_len; i++)
	  out_info[i].mask |= mask;
	க்கு (unsigned i = idx; i < end; i++)
	  info[i].mask |= mask;
      }
      இல்லையெனில்
      {
	unsigned cluster = _infos_find_min_cluster (info, idx, end);
	cluster = _infos_find_min_cluster (out_info, start, out_len, cluster);

	_infos_set_glyph_flags (out_info, start, out_len, cluster, mask);
	_infos_set_glyph_flags (info, idx, end, cluster, mask);
      }
    }
  }

  HB_ALWAYS_INLINE
  வெறுமை _set_glyph_flags (hb_mask_t mask,
			 unsigned start = 0,
			 unsigned end = (unsigned) -1,
			 தருக்க interior = பொய்,
			 தருக்க from_out_buffer = பொய்)
  {
    எனில் (unlikely (end != (unsigned) -1 && end - start > 255))
      திரும்பு;

    end = hb_min (end, len);

    எனில் (interior && !from_out_buffer && end - start < 2)
      திரும்பு;

    _set_glyph_flags_impl (mask, start, end, interior, from_out_buffer);
  }


  வெறுமை unsafe_to_break (unsigned முழு start = 0, unsigned முழு end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      உண்மை);
  }
  வெறுமை safe_to_செருகு_tatweel (unsigned முழு start = 0, unsigned முழு end = -1)
  {
    எனில் ((flags & HB_BUFFER_FLAG_PRODUCE_SAFE_TO_செருகு_TATWEEL) == 0)
    {
      unsafe_to_break (start, end);
      திரும்பு;
    }
    _set_glyph_flags (HB_GLYPH_FLAG_SAFE_TO_செருகு_TATWEEL,
		      start, end,
		      உண்மை);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  வெறுமை unsafe_to_concat (unsigned முழு start = 0, unsigned முழு end = -1)
  {
    எனில் (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      திரும்பு;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      பொய்);
  }
  வெறுமை unsafe_to_break_from_outbuffer (unsigned முழு start = 0, unsigned முழு end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      உண்மை, உண்மை);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  வெறுமை unsafe_to_concat_from_outbuffer (unsigned முழு start = 0, unsigned முழு end = -1)
  {
    எனில் (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      திரும்பு;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      பொய், உண்மை);
  }


  /* Internal methods */
  HB_NODISCARD HB_INTERNAL தருக்க move_to (unsigned முழு i); /* i is output-buffer index. */

  HB_NODISCARD HB_INTERNAL தருக்க enlarge (unsigned முழு size);

  HB_NODISCARD தருக்க resize (unsigned length)
  {
    ப்ரதிஜ்ஞா (!have_output);
    எனில் (unlikely (!ensure (length))) திரும்பு பொய்;
    len = length;
    திரும்பு உண்மை;
  }
  HB_NODISCARD தருக்க ensure (unsigned முழு size)
  { திரும்பு likely (!size || size < allocated) ? உண்மை : enlarge (size); }

  HB_NODISCARD தருக்க ensure_inplace (unsigned முழு size)
  { திரும்பு likely (!size || size < allocated); }

  வெறுமை assert_glyphs ()
  {
    ப்ரதிஜ்ஞா ((content_type == HB_BUFFER_CONTENT_TYPE_GLYPHS) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  வெறுமை assert_unicode ()
  {
    ப்ரதிஜ்ஞா ((content_type == HB_BUFFER_CONTENT_TYPE_UNICODE) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  HB_NODISCARD தருக்க ensure_glyphs ()
  {
    எனில் (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_GLYPHS))
    {
      எனில் (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	திரும்பு பொய்;
      ப்ரதிஜ்ஞா (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_GLYPHS;
    }
    திரும்பு உண்மை;
  }
  HB_NODISCARD தருக்க ensure_unicode ()
  {
    எனில் (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_UNICODE))
    {
      எனில் (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	திரும்பு பொய்;
      ப்ரதிஜ்ஞா (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_UNICODE;
    }
    திரும்பு உண்மை;
  }

  HB_NODISCARD HB_INTERNAL தருக்க make_room_for (unsigned முழு இலக்கம்_in, unsigned முழு இலக்கம்_out);
  HB_NODISCARD HB_INTERNAL தருக்க shift_forward (unsigned முழு மொத்தம்);

  வகைவரை long scratch_buffer_t;
  HB_INTERNAL scratch_buffer_t *get_scratch_buffer (unsigned முழு *size);

  வெறுமை clear_context (unsigned முழு side) { context_len[side] = 0; }

  HB_INTERNAL வெறுமை sort (unsigned முழு start, unsigned முழு end, முழு(*compar)(மாறிலி hb_glyph_info_t *, மாறிலி hb_glyph_info_t *));

  தருக்க messaging ()
  {
#ifdef HB_NO_BUFFER_MESSAGE
    திரும்பு பொய்;
#இல்லையெனில்
    திரும்பு unlikely (message_func);
#endif
  }
  தருக்க message (hb_font_t *font, மாறிலி எழுத்து *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#ifdef HB_NO_BUFFER_MESSAGE
    திரும்பு உண்மை;
#இல்லையெனில்
    எனில் (likely (!messaging ()))
      திரும்பு உண்மை;

    va_list ap;
    va_start (ap, fmt);
    தருக்க ret = message_impl (font, fmt, ap);
    va_end (ap);

    திரும்பு ret;
#endif
  }
  HB_INTERNAL தருக்க message_impl (hb_font_t *font, மாறிலி எழுத்து *fmt, va_list ap) HB_PRINTF_FUNC(3, 0);
  HB_INTERNAL வெறுமை changed ();

  நிலையான வெறுமை
  set_cluster (hb_glyph_info_t &inf, unsigned முழு cluster, unsigned முழு mask = 0)
  {
    எனில் (inf.cluster != cluster)
      inf.mask = (inf.mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  வெறுமை
  _infos_set_glyph_flags (hb_glyph_info_t *infos,
			  unsigned முழு start, unsigned முழு end,
			  unsigned முழு cluster,
			  hb_mask_t mask)
  {
    எனில் (unlikely (start == end))
      திரும்பு;

    max_ops -= end - start;
    எனில் (unlikely (max_ops < 0))
      successful = பொய்;

    unsigned cluster_first = infos[start].cluster;
    unsigned cluster_last = infos[end - 1].cluster;

    எனில் (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_first && cluster != cluster_last))
    {
      க்கு (unsigned முழு i = start; i < end; i++)
	எனில் (cluster != infos[i].cluster)
	  infos[i].mask |= mask;
      திரும்பு;
    }

    /* Monotone clusters */

    எனில் (cluster == cluster_first)
    {
      க்கு (unsigned முழு i = end; start < i && infos[i - 1].cluster != cluster_first; i--)
	infos[i - 1].mask |= mask;
    }
    இல்லையெனில் /* cluster == cluster_last */
    {
      க்கு (unsigned முழு i = start; i < end && infos[i].cluster != cluster_last; i++)
	infos[i].mask |= mask;
    }
  }
  unsigned
  _infos_find_min_cluster (மாறிலி hb_glyph_info_t *infos,
			   unsigned start, unsigned end,
			   unsigned cluster = UINT_MAX)
  {
    எனில் (unlikely (start == end))
      திரும்பு cluster;

    எனில் (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS)
    {
      க்கு (unsigned முழு i = start; i < end; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      திரும்பு cluster;
    }

    திரும்பு hb_min (cluster, hb_min (infos[start].cluster, infos[end - 1].cluster));
  }

  வெறுமை clear_glyph_flags (hb_mask_t mask = 0)
  {
    க்கு (unsigned முழு i = 0; i < len; i++)
      info[i].mask = (info[i].mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_buffer_t);


#define foreach_group(buffer, start, end, group_func) \
  க்கு (unsigned முழு \
       மொத்தம்_2 = buffer->len, \
       start = 0, end = மொத்தம்_2 ? buffer->group_end (0, group_func) : 0; \
       start < மொத்தம்_2; \
       start = end, end = buffer->group_end (start, group_func))

#define foreach_cluster(buffer, start, end) \
	foreach_group (buffer, start, end, hb_buffer_t::_cluster_group_func)


#define HB_BUFFER_XALLOCATE_VAR(b, func, var) \
  b->func (offsetof (hb_glyph_info_t, var) - offsetof(hb_glyph_info_t, var1), \
	   அளவு (b->info[0].var))
#define HB_BUFFER_ALLOCATE_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, allocate_var,     var ())
#define HB_BUFFER_TRY_ALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, try_allocate_var, var ())
#define HB_BUFFER_DEALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, deallocate_var,   var ())
#define HB_BUFFER_ASSERT_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, assert_var,       var ())


#endif /* HB_BUFFER_HH */
