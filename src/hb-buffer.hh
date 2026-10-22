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

#अगर_अघोषित HB_बफर_HH
#घोषणा HB_बफर_HH

#समावेश "hb.hh"
#समावेश "hb-unicode.hh"
#समावेश "hb-set-digest.hh"


static_assert ((माप (hb_glyph_info_t) == 20), "");
static_assert ((माप (hb_glyph_info_t) == माप (hb_glyph_position_t)), "");

HB_MARK_AS_FLAG_T (hb_glyph_flags_t);
HB_MARK_AS_FLAG_T (hb_बफर_flags_t);
HB_MARK_AS_FLAG_T (hb_बफर_serialize_flags_t);
HB_MARK_AS_FLAG_T (hb_बफर_diff_flags_t);

क्रमागत hb_बफर_scratch_flags_t {
  HB_बफर_SCRATCH_FLAG_DEFAULT			= 0x00000000u,
  HB_बफर_SCRATCH_FLAG_HAS_FRACTION_SLASH		= 0x00000001u,
  HB_बफर_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES		= 0x00000002u,
  HB_बफर_SCRATCH_FLAG_HAS_SPACE_FALLBACK		= 0x00000004u,
  HB_बफर_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT		= 0x00000008u,
  HB_बफर_SCRATCH_FLAG_HAS_CGJ			= 0x00000010u,
  HB_बफर_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE		= 0x00000020u,
  HB_बफर_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK= 0x00000040u,
  HB_बफर_SCRATCH_FLAG_HAS_CONTINUATIONS		= 0x00000080u,

  /* Reserved for shapers' internal use. */
  HB_बफर_SCRATCH_FLAG_SHAPER0			= 0x01000000u,
  HB_बफर_SCRATCH_FLAG_SHAPER1			= 0x02000000u,
  HB_बफर_SCRATCH_FLAG_SHAPER2			= 0x04000000u,
  HB_बफर_SCRATCH_FLAG_SHAPER3			= 0x08000000u,
};
HB_MARK_AS_FLAG_T (hb_बफर_scratch_flags_t);


/*
 * hb_buffer_t
 */

काष्ठा hb_बफर_t
{
  hb_object_header_t header;

  /*
   * Information about how the text in the buffer should be treated.
   */

  hb_unicode_funcs_t *unicode; /* Unicode functions */
  hb_बफर_flags_t flags; /* BOT / EOT / etc. */
  hb_बफर_cluster_level_t cluster_level;
  hb_codepoint_t replacement; /* U+FFFD or something else. */
  hb_codepoint_t invisible; /* 0 or something else. */
  hb_codepoint_t not_found; /* 0 or something else. */
  hb_codepoint_t not_found_variation_selector; /* HB_CODEPOINT_INVALID or something else. */

  /*
   * Buffer contents
   */

  hb_बफर_content_type_t content_type;
  hb_segment_properties_t props; /* Script, language, direction */

  bool successful; /* Allocations successful */
  bool have_output; /* Whether we have an output buffer going on */
  bool have_positions; /* Whether we have positions */

  अचिन्हित पूर्णांक idx; /* Cursor into ->info and ->pos arrays */
  अचिन्हित पूर्णांक len; /* Length of ->info and ->pos arrays */
  अचिन्हित पूर्णांक out_len; /* Length of ->out_info array if have_output */

  अचिन्हित पूर्णांक allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *out_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  अटल constexpr अचिन्हित CONTEXT_लंबाई = 5u;
  hb_codepoint_t context[2][CONTEXT_लंबाई];
  अचिन्हित पूर्णांक context_len[2];

  hb_set_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_bits;
  uint8_t serial;
  uint32_t random_state;
  hb_बफर_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  अचिन्हित पूर्णांक max_len; /* Maximum allowed len. */
  पूर्णांक max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#अगर_अघोषित HB_NO_बफर_MESSAGE
  प्रकार व्योम (*changed_func_t) (hb_बफर_t *बफर, व्योम *user_तथ्य);

  hb_बफर_message_func_t message_func;
  व्योम *message_तथ्य;
  hb_destroy_func_t message_destroy;
  changed_func_t changed_func;
  व्योम *changed_तथ्य;
  अचिन्हित message_depth; /* How deeply are we inside a message callback? */
#अन्यथा
  अटल constexpr अचिन्हित message_depth = 0u;
#पूर्ण_अगर



  /* Methods */

  HB_NODISCARD bool in_error () स्थिर { वापस !successful; }

  व्योम allocate_var (अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक गिनती)
  {
    अचिन्हित पूर्णांक end = start + गिनती;
    निश्चित (end <= 8);
    अचिन्हित पूर्णांक bits = (1u<<end) - (1u<<start);
    निश्चित (0 == (allocated_var_bits & bits));
    allocated_var_bits |= bits;
  }
  bool try_allocate_var (अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक गिनती)
  {
    अचिन्हित पूर्णांक end = start + गिनती;
    निश्चित (end <= 8);
    अचिन्हित पूर्णांक bits = (1u<<end) - (1u<<start);
    अगर (allocated_var_bits & bits)
      वापस false;
    allocated_var_bits |= bits;
    वापस true;
  }
  व्योम deallocate_var (अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक गिनती)
  {
    अचिन्हित पूर्णांक end = start + गिनती;
    निश्चित (end <= 8);
    अचिन्हित पूर्णांक bits = (1u<<end) - (1u<<start);
    निश्चित (bits == (allocated_var_bits & bits));
    allocated_var_bits &= ~bits;
  }
  व्योम assert_var (अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक गिनती)
  {
    अचिन्हित पूर्णांक end = start + गिनती;
    निश्चित (end <= 8);
    HB_UNUSED अचिन्हित पूर्णांक bits = (1u<<end) - (1u<<start);
    निश्चित (bits == (allocated_var_bits & bits));
  }
  व्योम deallocate_var_all ()
  {
    allocated_var_bits = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (अचिन्हित पूर्णांक i = 0) { वापस info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (अचिन्हित पूर्णांक i = 0) स्थिर { वापस info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (अचिन्हित पूर्णांक i = 0) { वापस pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (अचिन्हित पूर्णांक i = 0) स्थिर { वापस pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { वापस out_info[out_len ? out_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () स्थिर { वापस out_info[out_len ? out_len - 1 : 0]; }

  ढाँचा <typename set_t>
  व्योम collect_codepoints (set_t &d) स्थिर
  { d.सफा (); d.add_array (&info[0].codepoint, len, माप (info[0])); }

  व्योम update_digest ()
  {
    digest = hb_set_digest_t ();
    collect_codepoints (digest);
  }

  HB_INTERNAL व्योम similar (स्थिर hb_बफर_t &src);
  HB_INTERNAL व्योम reset ();
  HB_INTERNAL व्योम सफा ();

  /* Called around shape() */
  HB_INTERNAL व्योम enter ();
  HB_INTERNAL व्योम leave ();

#अगर_अघोषित HB_NO_बफर_VERIFY
  HB_INTERNAL
#पूर्ण_अगर
  bool verify (hb_बफर_t        *text_बफर,
	       hb_font_t          *font,
	       स्थिर hb_feature_t *features,
	       अचिन्हित पूर्णांक        अंक_features,
	       स्थिर अक्षर * स्थिर *shapers)
#अगर_अघोषित HB_NO_बफर_VERIFY
  ;
#अन्यथा
  { वापस true; }
#पूर्ण_अगर

  अचिन्हित पूर्णांक backtrack_len () स्थिर { वापस have_output ? out_len : idx; }
  अचिन्हित पूर्णांक lookahead_len () स्थिर { वापस len - idx; }
  uint8_t अग्र_serial () { वापस ++serial ? serial : ++serial; }

  HB_INTERNAL व्योम add (hb_codepoint_t  codepoint,
			अचिन्हित पूर्णांक    cluster);
  HB_INTERNAL व्योम add_info (स्थिर hb_glyph_info_t &glyph_info);
  HB_INTERNAL व्योम add_info_and_pos (स्थिर hb_glyph_info_t &glyph_info,
				     स्थिर hb_glyph_position_t &glyph_pos);

  व्योम reverse_range (अचिन्हित start, अचिन्हित end)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (start, end);
    अगर (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (start, end);
  }
  व्योम reverse () { reverse_range (0, len); }

  ढाँचा <typename FuncType>
  व्योम reverse_groups (स्थिर FuncType& group,
		       bool merge_clusters = false)
  {
    अगर (unlikely (!len))
      वापस;

    अचिन्हित start = 0;
    अचिन्हित i;
    क्रम (i = 1; i < len; i++)
    {
      अगर (!group (info[i - 1], info[i]))
      {
	अगर (merge_clusters)
	  यह->merge_clusters (start, i);
	reverse_range (start, i);
	start = i;
      }
    }
    अगर (merge_clusters)
      यह->merge_clusters (start, i);
    reverse_range (start, i);

    reverse ();
  }

  ढाँचा <typename FuncType>
  अचिन्हित group_end (अचिन्हित start, स्थिर FuncType& group) स्थिर
  {
    जबतक (++start < len && group (info[start - 1], info[start]))
      ;

    वापस start;
  }

  अटल bool _cluster_group_func (स्थिर hb_glyph_info_t& a,
				   स्थिर hb_glyph_info_t& b)
  { वापस a.cluster == b.cluster; }

  व्योम reverse_clusters () { reverse_groups (_cluster_group_func); }

  HB_INTERNAL व्योम guess_segment_properties ();

  HB_INTERNAL bool sync ();
  HB_INTERNAL पूर्णांक sync_so_far ();
  HB_INTERNAL व्योम clear_output ();
  HB_INTERNAL व्योम clear_positions ();

  ढाँचा <typename T>
  HB_NODISCARD bool replace_glyphs (अचिन्हित पूर्णांक अंक_in,
				    अचिन्हित पूर्णांक अंक_out,
				    स्थिर T *glyph_तथ्य)
  {
    अगर (unlikely (!make_room_for (अंक_in, अंक_out))) वापस false;

    निश्चित (idx + अंक_in <= len);

    merge_clusters (idx, idx + अंक_in);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &out_info[out_len];
    क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_out; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_तथ्य[i];
      pinfo++;
    }

    idx  += अंक_in;
    out_len += अंक_out;
    वापस true;
  }

  HB_NODISCARD bool replace_glyph (hb_codepoint_t glyph_सूचकांक)
  { वापस replace_glyphs (1, 1, &glyph_सूचकांक); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD bool output_glyph (hb_codepoint_t glyph_सूचकांक)
  { वापस replace_glyphs (0, 1, &glyph_सूचकांक); }

  HB_NODISCARD bool output_info (स्थिर hb_glyph_info_t &glyph_info)
  {
    अगर (unlikely (!make_room_for (0, 1))) वापस false;

    out_info[out_len] = glyph_info;

    out_len++;
    वापस true;
  }
  /* Copies glyph at idx to output but doesn't advance idx */
  HB_NODISCARD bool copy_glyph ()
  {
    /* Extra copy because cur()'s return can be freed within
     * output_info() call if buffer reallocates. */
    वापस output_info (hb_glyph_info_t (cur()));
  }

  /* Copies glyph at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD bool अग्र_glyph ()
  {
    अगर (have_output)
    {
      अगर (out_info != info || out_len != idx)
      {
	अगर (unlikely (!ensure (out_len + 1))) वापस false;
	out_info[out_len] = info[idx];
      }
      out_len++;
    }

    idx++;
    वापस true;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD bool अग्र_glyphs (अचिन्हित पूर्णांक n)
  {
    अगर (have_output)
    {
      अगर (out_info != info || out_len != idx)
      {
	अगर (unlikely (!ensure (out_len + n))) वापस false;
	स_हटाओ (out_info + out_len, info + idx, n * माप (out_info[0]));
      }
      out_len += n;
    }

    idx += n;
    वापस true;
  }
  /* Advance idx without copying to output. */
  व्योम skip_glyph () { idx++; }
  व्योम add_masks (hb_mask_t mask)
  {
    क्रम (अचिन्हित पूर्णांक j = 0; j < len; j++)
      info[j].mask |= mask;
  }
  HB_INTERNAL व्योम set_masks (hb_mask_t value, hb_mask_t mask,
			      अचिन्हित पूर्णांक cluster_start, अचिन्हित पूर्णांक cluster_end);

  व्योम merge_clusters (अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक end)
  {
    अगर (end - start < 2)
      वापस;
    अगर (!HB_बफर_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_to_break (start, end);
      वापस;
    }
    merge_clusters_impl (start, end);
  }
  व्योम merge_grapheme_clusters (अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक end)
  {
    अगर (end - start < 2)
      वापस;
    अगर (!HB_बफर_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_to_break (start, end);
      वापस;
    }
    merge_clusters_impl (start, end);
  }
  HB_INTERNAL व्योम merge_clusters_impl (अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक end);
  व्योम merge_out_clusters (अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक end)
  {
    अगर (end - start < 2)
      वापस;
    अगर (!HB_बफर_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      वापस;
    merge_out_clusters_impl (start, end);
  }
  व्योम merge_out_grapheme_clusters (अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक end)
  {
    अगर (end - start < 2)
      वापस;
    अगर (!HB_बफर_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      वापस;
    merge_out_clusters_impl (start, end);
  }
  HB_INTERNAL व्योम merge_out_clusters_impl (अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक end);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_INTERNAL व्योम delete_glyph ();
  HB_INTERNAL व्योम delete_glyphs_inplace (bool (*filter) (स्थिर hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  व्योम _set_glyph_flags_impl (hb_mask_t mask,
			      अचिन्हित start,
			      अचिन्हित end,
			      bool interior,
			      bool from_out_बफर)
  {
    अगर (!from_out_बफर || !have_output)
    {
      अगर (!interior)
      {
	क्रम (अचिन्हित i = start; i < end; i++)
	  info[i].mask |= mask;
      }
      अन्यथा
      {
	अचिन्हित cluster = _infos_find_min_cluster (info, start, end);
	_infos_set_glyph_flags (info, start, end, cluster, mask);
      }
    }
    अन्यथा
    {
      निश्चित (start <= out_len);
      निश्चित (idx <= end);

      अगर (!interior)
      {
	क्रम (अचिन्हित i = start; i < out_len; i++)
	  out_info[i].mask |= mask;
	क्रम (अचिन्हित i = idx; i < end; i++)
	  info[i].mask |= mask;
      }
      अन्यथा
      {
	अचिन्हित cluster = _infos_find_min_cluster (info, idx, end);
	cluster = _infos_find_min_cluster (out_info, start, out_len, cluster);

	_infos_set_glyph_flags (out_info, start, out_len, cluster, mask);
	_infos_set_glyph_flags (info, idx, end, cluster, mask);
      }
    }
  }

  HB_ALWAYS_INLINE
  व्योम _set_glyph_flags (hb_mask_t mask,
			 अचिन्हित start = 0,
			 अचिन्हित end = (अचिन्हित) -1,
			 bool interior = false,
			 bool from_out_बफर = false)
  {
    अगर (unlikely (end != (अचिन्हित) -1 && end - start > 255))
      वापस;

    end = hb_min (end, len);

    अगर (interior && !from_out_बफर && end - start < 2)
      वापस;

    _set_glyph_flags_impl (mask, start, end, interior, from_out_बफर);
  }


  व्योम unsafe_to_break (अचिन्हित पूर्णांक start = 0, अचिन्हित पूर्णांक end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      true);
  }
  व्योम safe_to_जोड़ो_tatweel (अचिन्हित पूर्णांक start = 0, अचिन्हित पूर्णांक end = -1)
  {
    अगर ((flags & HB_बफर_FLAG_PRODUCE_SAFE_TO_जोड़ो_TATWEEL) == 0)
    {
      unsafe_to_break (start, end);
      वापस;
    }
    _set_glyph_flags (HB_GLYPH_FLAG_SAFE_TO_जोड़ो_TATWEEL,
		      start, end,
		      true);
  }
#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  व्योम unsafe_to_concat (अचिन्हित पूर्णांक start = 0, अचिन्हित पूर्णांक end = -1)
  {
    अगर (likely ((flags & HB_बफर_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      वापस;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      false);
  }
  व्योम unsafe_to_break_from_outbuffer (अचिन्हित पूर्णांक start = 0, अचिन्हित पूर्णांक end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      true, true);
  }
#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  व्योम unsafe_to_concat_from_outbuffer (अचिन्हित पूर्णांक start = 0, अचिन्हित पूर्णांक end = -1)
  {
    अगर (likely ((flags & HB_बफर_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      वापस;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      false, true);
  }


  /* Internal methods */
  HB_NODISCARD HB_INTERNAL bool move_to (अचिन्हित पूर्णांक i); /* i is output-buffer index. */

  HB_NODISCARD HB_INTERNAL bool enlarge (अचिन्हित पूर्णांक आकार);

  HB_NODISCARD bool resize (अचिन्हित लंबाई)
  {
    निश्चित (!have_output);
    अगर (unlikely (!ensure (लंबाई))) वापस false;
    len = लंबाई;
    वापस true;
  }
  HB_NODISCARD bool ensure (अचिन्हित पूर्णांक आकार)
  { वापस likely (!आकार || आकार < allocated) ? true : enlarge (आकार); }

  HB_NODISCARD bool ensure_inplace (अचिन्हित पूर्णांक आकार)
  { वापस likely (!आकार || आकार < allocated); }

  व्योम assert_glyphs ()
  {
    निश्चित ((content_type == HB_बफर_CONTENT_TYPE_GLYPHS) ||
	    (!len && (content_type == HB_बफर_CONTENT_TYPE_INVALID)));
  }
  व्योम assert_unicode ()
  {
    निश्चित ((content_type == HB_बफर_CONTENT_TYPE_UNICODE) ||
	    (!len && (content_type == HB_बफर_CONTENT_TYPE_INVALID)));
  }
  HB_NODISCARD bool ensure_glyphs ()
  {
    अगर (unlikely (content_type != HB_बफर_CONTENT_TYPE_GLYPHS))
    {
      अगर (content_type != HB_बफर_CONTENT_TYPE_INVALID)
	वापस false;
      निश्चित (len == 0);
      content_type = HB_बफर_CONTENT_TYPE_GLYPHS;
    }
    वापस true;
  }
  HB_NODISCARD bool ensure_unicode ()
  {
    अगर (unlikely (content_type != HB_बफर_CONTENT_TYPE_UNICODE))
    {
      अगर (content_type != HB_बफर_CONTENT_TYPE_INVALID)
	वापस false;
      निश्चित (len == 0);
      content_type = HB_बफर_CONTENT_TYPE_UNICODE;
    }
    वापस true;
  }

  HB_NODISCARD HB_INTERNAL bool make_room_for (अचिन्हित पूर्णांक अंक_in, अचिन्हित पूर्णांक अंक_out);
  HB_NODISCARD HB_INTERNAL bool shift_forward (अचिन्हित पूर्णांक गिनती);

  प्रकार दीर्घ scratch_बफर_t;
  HB_INTERNAL scratch_बफर_t *get_scratch_बफर (अचिन्हित पूर्णांक *आकार);

  व्योम clear_context (अचिन्हित पूर्णांक side) { context_len[side] = 0; }

  HB_INTERNAL व्योम sort (अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक end, पूर्णांक(*compar)(स्थिर hb_glyph_info_t *, स्थिर hb_glyph_info_t *));

  bool messaging ()
  {
#अगर_घोषित HB_NO_बफर_MESSAGE
    वापस false;
#अन्यथा
    वापस unlikely (message_func);
#पूर्ण_अगर
  }
  bool message (hb_font_t *font, स्थिर अक्षर *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#अगर_घोषित HB_NO_बफर_MESSAGE
    वापस true;
#अन्यथा
    अगर (likely (!messaging ()))
      वापस true;

    बहु_सूची ap;
    बहु_शुरू (ap, fmt);
    bool ret = message_impl (font, fmt, ap);
    बहु_पूर्ण (ap);

    वापस ret;
#पूर्ण_अगर
  }
  HB_INTERNAL bool message_impl (hb_font_t *font, स्थिर अक्षर *fmt, बहु_सूची ap) HB_PRINTF_FUNC(3, 0);
  HB_INTERNAL व्योम changed ();

  अटल व्योम
  set_cluster (hb_glyph_info_t &inf, अचिन्हित पूर्णांक cluster, अचिन्हित पूर्णांक mask = 0)
  {
    अगर (inf.cluster != cluster)
      inf.mask = (inf.mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  व्योम
  _infos_set_glyph_flags (hb_glyph_info_t *infos,
			  अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक end,
			  अचिन्हित पूर्णांक cluster,
			  hb_mask_t mask)
  {
    अगर (unlikely (start == end))
      वापस;

    max_ops -= end - start;
    अगर (unlikely (max_ops < 0))
      successful = false;

    अचिन्हित cluster_first = infos[start].cluster;
    अचिन्हित cluster_last = infos[end - 1].cluster;

    अगर (cluster_level == HB_बफर_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_first && cluster != cluster_last))
    {
      क्रम (अचिन्हित पूर्णांक i = start; i < end; i++)
	अगर (cluster != infos[i].cluster)
	  infos[i].mask |= mask;
      वापस;
    }

    /* Monotone clusters */

    अगर (cluster == cluster_first)
    {
      क्रम (अचिन्हित पूर्णांक i = end; start < i && infos[i - 1].cluster != cluster_first; i--)
	infos[i - 1].mask |= mask;
    }
    अन्यथा /* cluster == cluster_last */
    {
      क्रम (अचिन्हित पूर्णांक i = start; i < end && infos[i].cluster != cluster_last; i++)
	infos[i].mask |= mask;
    }
  }
  अचिन्हित
  _infos_find_min_cluster (स्थिर hb_glyph_info_t *infos,
			   अचिन्हित start, अचिन्हित end,
			   अचिन्हित cluster = अच_पूर्णांक_उच्च)
  {
    अगर (unlikely (start == end))
      वापस cluster;

    अगर (cluster_level == HB_बफर_CLUSTER_LEVEL_CHARACTERS)
    {
      क्रम (अचिन्हित पूर्णांक i = start; i < end; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      वापस cluster;
    }

    वापस hb_min (cluster, hb_min (infos[start].cluster, infos[end - 1].cluster));
  }

  व्योम clear_glyph_flags (hb_mask_t mask = 0)
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < len; i++)
      info[i].mask = (info[i].mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_बफर_t);


#घोषणा foreach_group(बफर, start, end, group_func) \
  क्रम (अचिन्हित पूर्णांक \
       गिनती_2 = बफर->len, \
       start = 0, end = गिनती_2 ? बफर->group_end (0, group_func) : 0; \
       start < गिनती_2; \
       start = end, end = बफर->group_end (start, group_func))

#घोषणा foreach_cluster(बफर, start, end) \
	foreach_group (बफर, start, end, hb_बफर_t::_cluster_group_func)


#घोषणा HB_बफर_XALLOCATE_VAR(b, func, var) \
  b->func (दुरत्व (hb_glyph_info_t, var) - दुरत्व(hb_glyph_info_t, var1), \
	   माप (b->info[0].var))
#घोषणा HB_बफर_ALLOCATE_VAR(b, var)		HB_बफर_XALLOCATE_VAR (b, allocate_var,     var ())
#घोषणा HB_बफर_TRY_ALLOCATE_VAR(b, var)	HB_बफर_XALLOCATE_VAR (b, try_allocate_var, var ())
#घोषणा HB_बफर_DEALLOCATE_VAR(b, var)	HB_बफर_XALLOCATE_VAR (b, deallocate_var,   var ())
#घोषणा HB_बफर_ASSERT_VAR(b, var)		HB_बफर_XALLOCATE_VAR (b, assert_var,       var ())


#पूर्ण_अगर /* HB_BUFFER_HH */
