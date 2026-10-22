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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-unicode.hh"
#અન્તર્ભાવ "hb-set-digest.hh"


static_assert ((કદ (hb_glyph_info_t) == 20), "");
static_assert ((કદ (hb_glyph_info_t) == કદ (hb_glyph_position_t)), "");

HB_MARK_AS_FLAG_T (hb_glyph_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_serialize_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_diff_flags_t);

ગણના hb_buffer_scratch_flags_t {
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

સંરચના hb_buffer_t
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

  તાર્કિક successful; /* Allocations successful */
  તાર્કિક have_output; /* Whether we have an output buffer going on */
  તાર્કિક have_positions; /* Whether we have positions */

  unsigned પૂર્ણાંક idx; /* Cursor into ->info and ->pos arrays */
  unsigned પૂર્ણાંક len; /* Length of ->info and ->pos arrays */
  unsigned પૂર્ણાંક out_len; /* Length of ->out_info array if have_output */

  unsigned પૂર્ણાંક allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *out_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  સ્થિર constexpr unsigned CONTEXT_LENGTH = 5u;
  hb_codepoint_t context[2][CONTEXT_LENGTH];
  unsigned પૂર્ણાંક context_len[2];

  hb_set_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_bits;
  uint8_t serial;
  uint32_t random_state;
  hb_buffer_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  unsigned પૂર્ણાંક max_len; /* Maximum allowed len. */
  પૂર્ણાંક max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#ifndef HB_NO_BUFFER_MESSAGE
  પ્રકારવ્યાખ્યા શૂન્ય (*changed_func_t) (hb_buffer_t *buffer, શૂન્ય *user_data);

  hb_buffer_message_func_t message_func;
  શૂન્ય *message_data;
  hb_destroy_func_t message_destroy;
  changed_func_t changed_func;
  શૂન્ય *changed_data;
  unsigned message_depth; /* How deeply are we inside a message callback? */
#નહીંતર
  સ્થિર constexpr unsigned message_depth = 0u;
#endif



  /* Methods */

  HB_NODISCARD તાર્કિક in_error () અચલ { પાછા !successful; }

  શૂન્ય allocate_var (unsigned પૂર્ણાંક start, unsigned પૂર્ણાંક ગણતરી)
  {
    unsigned પૂર્ણાંક end = start + ગણતરી;
    પ્રતિજ્ઞા (end <= 8);
    unsigned પૂર્ણાંક bits = (1u<<end) - (1u<<start);
    પ્રતિજ્ઞા (0 == (allocated_var_bits & bits));
    allocated_var_bits |= bits;
  }
  તાર્કિક try_allocate_var (unsigned પૂર્ણાંક start, unsigned પૂર્ણાંક ગણતરી)
  {
    unsigned પૂર્ણાંક end = start + ગણતરી;
    પ્રતિજ્ઞા (end <= 8);
    unsigned પૂર્ણાંક bits = (1u<<end) - (1u<<start);
    જો (allocated_var_bits & bits)
      પાછા ખોટું;
    allocated_var_bits |= bits;
    પાછા સાચું;
  }
  શૂન્ય deallocate_var (unsigned પૂર્ણાંક start, unsigned પૂર્ણાંક ગણતરી)
  {
    unsigned પૂર્ણાંક end = start + ગણતરી;
    પ્રતિજ્ઞા (end <= 8);
    unsigned પૂર્ણાંક bits = (1u<<end) - (1u<<start);
    પ્રતિજ્ઞા (bits == (allocated_var_bits & bits));
    allocated_var_bits &= ~bits;
  }
  શૂન્ય assert_var (unsigned પૂર્ણાંક start, unsigned પૂર્ણાંક ગણતરી)
  {
    unsigned પૂર્ણાંક end = start + ગણતરી;
    પ્રતિજ્ઞા (end <= 8);
    HB_UNUSED unsigned પૂર્ણાંક bits = (1u<<end) - (1u<<start);
    પ્રતિજ્ઞા (bits == (allocated_var_bits & bits));
  }
  શૂન્ય deallocate_var_all ()
  {
    allocated_var_bits = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (unsigned પૂર્ણાંક i = 0) { પાછા info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (unsigned પૂર્ણાંક i = 0) અચલ { પાછા info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (unsigned પૂર્ણાંક i = 0) { પાછા pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (unsigned પૂર્ણાંક i = 0) અચલ { પાછા pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { પાછા out_info[out_len ? out_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () અચલ { પાછા out_info[out_len ? out_len - 1 : 0]; }

  template <typename set_t>
  શૂન્ય collect_codepoints (set_t &d) અચલ
  { d.clear (); d.add_array (&info[0].codepoint, len, કદ (info[0])); }

  શૂન્ય update_digest ()
  {
    digest = hb_set_digest_t ();
    collect_codepoints (digest);
  }

  HB_INTERNAL શૂન્ય similar (અચલ hb_buffer_t &src);
  HB_INTERNAL શૂન્ય reset ();
  HB_INTERNAL શૂન્ય clear ();

  /* Called around shape() */
  HB_INTERNAL શૂન્ય enter ();
  HB_INTERNAL શૂન્ય leave ();

#ifndef HB_NO_BUFFER_VERIFY
  HB_INTERNAL
#endif
  તાર્કિક verify (hb_buffer_t        *text_buffer,
	       hb_font_t          *font,
	       અચલ hb_feature_t *features,
	       unsigned પૂર્ણાંક        અંક_features,
	       અચલ અક્ષર * અચલ *shapers)
#ifndef HB_NO_BUFFER_VERIFY
  ;
#નહીંતર
  { પાછા સાચું; }
#endif

  unsigned પૂર્ણાંક backtrack_len () અચલ { પાછા have_output ? out_len : idx; }
  unsigned પૂર્ણાંક lookahead_len () અચલ { પાછા len - idx; }
  uint8_t next_serial () { પાછા ++serial ? serial : ++serial; }

  HB_INTERNAL શૂન્ય add (hb_codepoint_t  codepoint,
			unsigned પૂર્ણાંક    cluster);
  HB_INTERNAL શૂન્ય add_info (અચલ hb_glyph_info_t &glyph_info);
  HB_INTERNAL શૂન્ય add_info_and_pos (અચલ hb_glyph_info_t &glyph_info,
				     અચલ hb_glyph_position_t &glyph_pos);

  શૂન્ય reverse_range (unsigned start, unsigned end)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (start, end);
    જો (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (start, end);
  }
  શૂન્ય reverse () { reverse_range (0, len); }

  template <typename FuncType>
  શૂન્ય reverse_groups (અચલ FuncType& group,
		       તાર્કિક merge_clusters = ખોટું)
  {
    જો (unlikely (!len))
      પાછા;

    unsigned start = 0;
    unsigned i;
    માટે (i = 1; i < len; i++)
    {
      જો (!group (info[i - 1], info[i]))
      {
	જો (merge_clusters)
	  આ->merge_clusters (start, i);
	reverse_range (start, i);
	start = i;
      }
    }
    જો (merge_clusters)
      આ->merge_clusters (start, i);
    reverse_range (start, i);

    reverse ();
  }

  template <typename FuncType>
  unsigned group_end (unsigned start, અચલ FuncType& group) અચલ
  {
    જ્યારે (++start < len && group (info[start - 1], info[start]))
      ;

    પાછા start;
  }

  સ્થિર તાર્કિક _cluster_group_func (અચલ hb_glyph_info_t& a,
				   અચલ hb_glyph_info_t& b)
  { પાછા a.cluster == b.cluster; }

  શૂન્ય reverse_clusters () { reverse_groups (_cluster_group_func); }

  HB_INTERNAL શૂન્ય guess_segment_properties ();

  HB_INTERNAL તાર્કિક સમકાલિક ();
  HB_INTERNAL પૂર્ણાંક sync_so_far ();
  HB_INTERNAL શૂન્ય clear_output ();
  HB_INTERNAL શૂન્ય clear_positions ();

  template <typename T>
  HB_NODISCARD તાર્કિક replace_glyphs (unsigned પૂર્ણાંક અંક_in,
				    unsigned પૂર્ણાંક અંક_out,
				    અચલ T *glyph_data)
  {
    જો (unlikely (!make_room_for (અંક_in, અંક_out))) પાછા ખોટું;

    પ્રતિજ્ઞા (idx + અંક_in <= len);

    merge_clusters (idx, idx + અંક_in);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &out_info[out_len];
    માટે (unsigned પૂર્ણાંક i = 0; i < અંક_out; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_data[i];
      pinfo++;
    }

    idx  += અંક_in;
    out_len += અંક_out;
    પાછા સાચું;
  }

  HB_NODISCARD તાર્કિક replace_glyph (hb_codepoint_t glyph_index)
  { પાછા replace_glyphs (1, 1, &glyph_index); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD તાર્કિક output_glyph (hb_codepoint_t glyph_index)
  { પાછા replace_glyphs (0, 1, &glyph_index); }

  HB_NODISCARD તાર્કિક output_info (અચલ hb_glyph_info_t &glyph_info)
  {
    જો (unlikely (!make_room_for (0, 1))) પાછા ખોટું;

    out_info[out_len] = glyph_info;

    out_len++;
    પાછા સાચું;
  }
  /* Copies glyph at idx to output but doesn't advance idx */
  HB_NODISCARD તાર્કિક copy_glyph ()
  {
    /* Extra copy because cur()'s return can be freed within
     * output_info() call if buffer reallocates. */
    પાછા output_info (hb_glyph_info_t (cur()));
  }

  /* Copies glyph at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD તાર્કિક next_glyph ()
  {
    જો (have_output)
    {
      જો (out_info != info || out_len != idx)
      {
	જો (unlikely (!ensure (out_len + 1))) પાછા ખોટું;
	out_info[out_len] = info[idx];
      }
      out_len++;
    }

    idx++;
    પાછા સાચું;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD તાર્કિક next_glyphs (unsigned પૂર્ણાંક n)
  {
    જો (have_output)
    {
      જો (out_info != info || out_len != idx)
      {
	જો (unlikely (!ensure (out_len + n))) પાછા ખોટું;
	memmove (out_info + out_len, info + idx, n * કદ (out_info[0]));
      }
      out_len += n;
    }

    idx += n;
    પાછા સાચું;
  }
  /* Advance idx without copying to output. */
  શૂન્ય skip_glyph () { idx++; }
  શૂન્ય add_masks (hb_mask_t mask)
  {
    માટે (unsigned પૂર્ણાંક j = 0; j < len; j++)
      info[j].mask |= mask;
  }
  HB_INTERNAL શૂન્ય set_masks (hb_mask_t value, hb_mask_t mask,
			      unsigned પૂર્ણાંક cluster_start, unsigned પૂર્ણાંક cluster_end);

  શૂન્ય merge_clusters (unsigned પૂર્ણાંક start, unsigned પૂર્ણાંક end)
  {
    જો (end - start < 2)
      પાછા;
    જો (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_to_break (start, end);
      પાછા;
    }
    merge_clusters_impl (start, end);
  }
  શૂન્ય merge_grapheme_clusters (unsigned પૂર્ણાંક start, unsigned પૂર્ણાંક end)
  {
    જો (end - start < 2)
      પાછા;
    જો (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_to_break (start, end);
      પાછા;
    }
    merge_clusters_impl (start, end);
  }
  HB_INTERNAL શૂન્ય merge_clusters_impl (unsigned પૂર્ણાંક start, unsigned પૂર્ણાંક end);
  શૂન્ય merge_out_clusters (unsigned પૂર્ણાંક start, unsigned પૂર્ણાંક end)
  {
    જો (end - start < 2)
      પાછા;
    જો (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      પાછા;
    merge_out_clusters_impl (start, end);
  }
  શૂન્ય merge_out_grapheme_clusters (unsigned પૂર્ણાંક start, unsigned પૂર્ણાંક end)
  {
    જો (end - start < 2)
      પાછા;
    જો (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      પાછા;
    merge_out_clusters_impl (start, end);
  }
  HB_INTERNAL શૂન્ય merge_out_clusters_impl (unsigned પૂર્ણાંક start, unsigned પૂર્ણાંક end);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_INTERNAL શૂન્ય delete_glyph ();
  HB_INTERNAL શૂન્ય delete_glyphs_inplace (તાર્કિક (*filter) (અચલ hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  શૂન્ય _set_glyph_flags_impl (hb_mask_t mask,
			      unsigned start,
			      unsigned end,
			      તાર્કિક interior,
			      તાર્કિક from_out_buffer)
  {
    જો (!from_out_buffer || !have_output)
    {
      જો (!interior)
      {
	માટે (unsigned i = start; i < end; i++)
	  info[i].mask |= mask;
      }
      નહીંતર
      {
	unsigned cluster = _infos_find_min_cluster (info, start, end);
	_infos_set_glyph_flags (info, start, end, cluster, mask);
      }
    }
    નહીંતર
    {
      પ્રતિજ્ઞા (start <= out_len);
      પ્રતિજ્ઞા (idx <= end);

      જો (!interior)
      {
	માટે (unsigned i = start; i < out_len; i++)
	  out_info[i].mask |= mask;
	માટે (unsigned i = idx; i < end; i++)
	  info[i].mask |= mask;
      }
      નહીંતર
      {
	unsigned cluster = _infos_find_min_cluster (info, idx, end);
	cluster = _infos_find_min_cluster (out_info, start, out_len, cluster);

	_infos_set_glyph_flags (out_info, start, out_len, cluster, mask);
	_infos_set_glyph_flags (info, idx, end, cluster, mask);
      }
    }
  }

  HB_ALWAYS_INLINE
  શૂન્ય _set_glyph_flags (hb_mask_t mask,
			 unsigned start = 0,
			 unsigned end = (unsigned) -1,
			 તાર્કિક interior = ખોટું,
			 તાર્કિક from_out_buffer = ખોટું)
  {
    જો (unlikely (end != (unsigned) -1 && end - start > 255))
      પાછા;

    end = hb_min (end, len);

    જો (interior && !from_out_buffer && end - start < 2)
      પાછા;

    _set_glyph_flags_impl (mask, start, end, interior, from_out_buffer);
  }


  શૂન્ય unsafe_to_break (unsigned પૂર્ણાંક start = 0, unsigned પૂર્ણાંક end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      સાચું);
  }
  શૂન્ય safe_to_ઉમેરો_tatweel (unsigned પૂર્ણાંક start = 0, unsigned પૂર્ણાંક end = -1)
  {
    જો ((flags & HB_BUFFER_FLAG_PRODUCE_SAFE_TO_ઉમેરો_TATWEEL) == 0)
    {
      unsafe_to_break (start, end);
      પાછા;
    }
    _set_glyph_flags (HB_GLYPH_FLAG_SAFE_TO_ઉમેરો_TATWEEL,
		      start, end,
		      સાચું);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  શૂન્ય unsafe_to_concat (unsigned પૂર્ણાંક start = 0, unsigned પૂર્ણાંક end = -1)
  {
    જો (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      પાછા;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      ખોટું);
  }
  શૂન્ય unsafe_to_break_from_outbuffer (unsigned પૂર્ણાંક start = 0, unsigned પૂર્ણાંક end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      સાચું, સાચું);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  શૂન્ય unsafe_to_concat_from_outbuffer (unsigned પૂર્ણાંક start = 0, unsigned પૂર્ણાંક end = -1)
  {
    જો (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      પાછા;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      ખોટું, સાચું);
  }


  /* Internal methods */
  HB_NODISCARD HB_INTERNAL તાર્કિક move_to (unsigned પૂર્ણાંક i); /* i is output-buffer index. */

  HB_NODISCARD HB_INTERNAL તાર્કિક enlarge (unsigned પૂર્ણાંક size);

  HB_NODISCARD તાર્કિક resize (unsigned length)
  {
    પ્રતિજ્ઞા (!have_output);
    જો (unlikely (!ensure (length))) પાછા ખોટું;
    len = length;
    પાછા સાચું;
  }
  HB_NODISCARD તાર્કિક ensure (unsigned પૂર્ણાંક size)
  { પાછા likely (!size || size < allocated) ? સાચું : enlarge (size); }

  HB_NODISCARD તાર્કિક ensure_inplace (unsigned પૂર્ણાંક size)
  { પાછા likely (!size || size < allocated); }

  શૂન્ય assert_glyphs ()
  {
    પ્રતિજ્ઞા ((content_type == HB_BUFFER_CONTENT_TYPE_GLYPHS) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  શૂન્ય assert_unicode ()
  {
    પ્રતિજ્ઞા ((content_type == HB_BUFFER_CONTENT_TYPE_UNICODE) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  HB_NODISCARD તાર્કિક ensure_glyphs ()
  {
    જો (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_GLYPHS))
    {
      જો (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	પાછા ખોટું;
      પ્રતિજ્ઞા (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_GLYPHS;
    }
    પાછા સાચું;
  }
  HB_NODISCARD તાર્કિક ensure_unicode ()
  {
    જો (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_UNICODE))
    {
      જો (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	પાછા ખોટું;
      પ્રતિજ્ઞા (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_UNICODE;
    }
    પાછા સાચું;
  }

  HB_NODISCARD HB_INTERNAL તાર્કિક make_room_for (unsigned પૂર્ણાંક અંક_in, unsigned પૂર્ણાંક અંક_out);
  HB_NODISCARD HB_INTERNAL તાર્કિક shift_forward (unsigned પૂર્ણાંક ગણતરી);

  પ્રકારવ્યાખ્યા long scratch_buffer_t;
  HB_INTERNAL scratch_buffer_t *get_scratch_buffer (unsigned પૂર્ણાંક *size);

  શૂન્ય clear_context (unsigned પૂર્ણાંક side) { context_len[side] = 0; }

  HB_INTERNAL શૂન્ય sort (unsigned પૂર્ણાંક start, unsigned પૂર્ણાંક end, પૂર્ણાંક(*compar)(અચલ hb_glyph_info_t *, અચલ hb_glyph_info_t *));

  તાર્કિક messaging ()
  {
#ifdef HB_NO_BUFFER_MESSAGE
    પાછા ખોટું;
#નહીંતર
    પાછા unlikely (message_func);
#endif
  }
  તાર્કિક message (hb_font_t *font, અચલ અક્ષર *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#ifdef HB_NO_BUFFER_MESSAGE
    પાછા સાચું;
#નહીંતર
    જો (likely (!messaging ()))
      પાછા સાચું;

    va_list ap;
    va_start (ap, fmt);
    તાર્કિક ret = message_impl (font, fmt, ap);
    va_end (ap);

    પાછા ret;
#endif
  }
  HB_INTERNAL તાર્કિક message_impl (hb_font_t *font, અચલ અક્ષર *fmt, va_list ap) HB_PRINTF_FUNC(3, 0);
  HB_INTERNAL શૂન્ય changed ();

  સ્થિર શૂન્ય
  set_cluster (hb_glyph_info_t &inf, unsigned પૂર્ણાંક cluster, unsigned પૂર્ણાંક mask = 0)
  {
    જો (inf.cluster != cluster)
      inf.mask = (inf.mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  શૂન્ય
  _infos_set_glyph_flags (hb_glyph_info_t *infos,
			  unsigned પૂર્ણાંક start, unsigned પૂર્ણાંક end,
			  unsigned પૂર્ણાંક cluster,
			  hb_mask_t mask)
  {
    જો (unlikely (start == end))
      પાછા;

    max_ops -= end - start;
    જો (unlikely (max_ops < 0))
      successful = ખોટું;

    unsigned cluster_first = infos[start].cluster;
    unsigned cluster_last = infos[end - 1].cluster;

    જો (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_first && cluster != cluster_last))
    {
      માટે (unsigned પૂર્ણાંક i = start; i < end; i++)
	જો (cluster != infos[i].cluster)
	  infos[i].mask |= mask;
      પાછા;
    }

    /* Monotone clusters */

    જો (cluster == cluster_first)
    {
      માટે (unsigned પૂર્ણાંક i = end; start < i && infos[i - 1].cluster != cluster_first; i--)
	infos[i - 1].mask |= mask;
    }
    નહીંતર /* cluster == cluster_last */
    {
      માટે (unsigned પૂર્ણાંક i = start; i < end && infos[i].cluster != cluster_last; i++)
	infos[i].mask |= mask;
    }
  }
  unsigned
  _infos_find_min_cluster (અચલ hb_glyph_info_t *infos,
			   unsigned start, unsigned end,
			   unsigned cluster = UINT_MAX)
  {
    જો (unlikely (start == end))
      પાછા cluster;

    જો (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS)
    {
      માટે (unsigned પૂર્ણાંક i = start; i < end; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      પાછા cluster;
    }

    પાછા hb_min (cluster, hb_min (infos[start].cluster, infos[end - 1].cluster));
  }

  શૂન્ય clear_glyph_flags (hb_mask_t mask = 0)
  {
    માટે (unsigned પૂર્ણાંક i = 0; i < len; i++)
      info[i].mask = (info[i].mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_buffer_t);


#define foreach_group(buffer, start, end, group_func) \
  માટે (unsigned પૂર્ણાંક \
       ગણતરી_2 = buffer->len, \
       start = 0, end = ગણતરી_2 ? buffer->group_end (0, group_func) : 0; \
       start < ગણતરી_2; \
       start = end, end = buffer->group_end (start, group_func))

#define foreach_cluster(buffer, start, end) \
	foreach_group (buffer, start, end, hb_buffer_t::_cluster_group_func)


#define HB_BUFFER_XALLOCATE_VAR(b, func, var) \
  b->func (offsetof (hb_glyph_info_t, var) - offsetof(hb_glyph_info_t, var1), \
	   કદ (b->info[0].var))
#define HB_BUFFER_ALLOCATE_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, allocate_var,     var ())
#define HB_BUFFER_TRY_ALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, try_allocate_var, var ())
#define HB_BUFFER_DEALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, deallocate_var,   var ())
#define HB_BUFFER_ASSERT_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, assert_var,       var ())


#endif /* HB_BUFFER_HH */
