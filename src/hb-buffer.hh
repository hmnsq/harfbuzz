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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-unicode.hh"
#अन्तर्भाव "hb-set-digest.hh"


static_assert ((परिमाण (hb_glyph_info_t) == 20), "");
static_assert ((परिमाण (hb_glyph_info_t) == परिमाण (hb_glyph_position_t)), "");

HB_MARK_AS_FLAG_T (hb_glyph_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_serialize_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_diff_flags_t);

गणना hb_buffer_scratch_flags_t {
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

संरचना hb_buffer_t
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

  तार्किक successful; /* Allocations successful */
  तार्किक have_output; /* Whether we have an output buffer going on */
  तार्किक have_positions; /* Whether we have positions */

  अचिन्हित पूर्णाङ्क idx; /* Cursor into ->info and ->pos arrays */
  अचिन्हित पूर्णाङ्क len; /* Length of ->info and ->pos arrays */
  अचिन्हित पूर्णाङ्क out_len; /* Length of ->out_info array if have_output */

  अचिन्हित पूर्णाङ्क allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *out_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  स्थिर constexpr अचिन्हित CONTEXT_LENGTH = 5u;
  hb_codepoint_t context[2][CONTEXT_LENGTH];
  अचिन्हित पूर्णाङ्क context_len[2];

  hb_set_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_bits;
  uint8_t serial;
  uint32_t random_state;
  hb_buffer_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  अचिन्हित पूर्णाङ्क max_len; /* Maximum allowed len. */
  पूर्णाङ्क max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#ifndef HB_NO_BUFFER_MESSAGE
  प्रकारनिर्देश शून्य (*changed_func_t) (hb_buffer_t *buffer, शून्य *user_data);

  hb_buffer_message_func_t message_func;
  शून्य *message_data;
  hb_destroy_func_t message_destroy;
  changed_func_t changed_func;
  शून्य *changed_data;
  अचिन्हित message_depth; /* How deeply are we inside a message callback? */
#अन्यथा
  स्थिर constexpr अचिन्हित message_depth = 0u;
#endif



  /* Methods */

  HB_NODISCARD तार्किक in_error () ध्रुव { प्रत्यागम !successful; }

  शून्य allocate_var (अचिन्हित पूर्णाङ्क start, अचिन्हित पूर्णाङ्क गिनती)
  {
    अचिन्हित पूर्णाङ्क end = start + गिनती;
    प्रतिज्ञा (end <= 8);
    अचिन्हित पूर्णाङ्क bits = (1u<<end) - (1u<<start);
    प्रतिज्ञा (0 == (allocated_var_bits & bits));
    allocated_var_bits |= bits;
  }
  तार्किक try_allocate_var (अचिन्हित पूर्णाङ्क start, अचिन्हित पूर्णाङ्क गिनती)
  {
    अचिन्हित पूर्णाङ्क end = start + गिनती;
    प्रतिज्ञा (end <= 8);
    अचिन्हित पूर्णाङ्क bits = (1u<<end) - (1u<<start);
    यदि (allocated_var_bits & bits)
      प्रत्यागम असत्य;
    allocated_var_bits |= bits;
    प्रत्यागम सत्य;
  }
  शून्य deallocate_var (अचिन्हित पूर्णाङ्क start, अचिन्हित पूर्णाङ्क गिनती)
  {
    अचिन्हित पूर्णाङ्क end = start + गिनती;
    प्रतिज्ञा (end <= 8);
    अचिन्हित पूर्णाङ्क bits = (1u<<end) - (1u<<start);
    प्रतिज्ञा (bits == (allocated_var_bits & bits));
    allocated_var_bits &= ~bits;
  }
  शून्य assert_var (अचिन्हित पूर्णाङ्क start, अचिन्हित पूर्णाङ्क गिनती)
  {
    अचिन्हित पूर्णाङ्क end = start + गिनती;
    प्रतिज्ञा (end <= 8);
    HB_UNUSED अचिन्हित पूर्णाङ्क bits = (1u<<end) - (1u<<start);
    प्रतिज्ञा (bits == (allocated_var_bits & bits));
  }
  शून्य deallocate_var_all ()
  {
    allocated_var_bits = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (अचिन्हित पूर्णाङ्क i = 0) { प्रत्यागम info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (अचिन्हित पूर्णाङ्क i = 0) ध्रुव { प्रत्यागम info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (अचिन्हित पूर्णाङ्क i = 0) { प्रत्यागम pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (अचिन्हित पूर्णाङ्क i = 0) ध्रुव { प्रत्यागम pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { प्रत्यागम out_info[out_len ? out_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () ध्रुव { प्रत्यागम out_info[out_len ? out_len - 1 : 0]; }

  ढाँचा <typename set_t>
  शून्य collect_codepoints (set_t &d) ध्रुव
  { d.clear (); d.add_array (&info[0].codepoint, len, परिमाण (info[0])); }

  शून्य update_digest ()
  {
    digest = hb_set_digest_t ();
    collect_codepoints (digest);
  }

  HB_INTERNAL शून्य similar (ध्रुव hb_buffer_t &src);
  HB_INTERNAL शून्य reset ();
  HB_INTERNAL शून्य clear ();

  /* Called around shape() */
  HB_INTERNAL शून्य enter ();
  HB_INTERNAL शून्य leave ();

#ifndef HB_NO_BUFFER_VERIFY
  HB_INTERNAL
#endif
  तार्किक verify (hb_buffer_t        *text_buffer,
	       hb_font_t          *font,
	       ध्रुव hb_feature_t *features,
	       अचिन्हित पूर्णाङ्क        अंक_features,
	       ध्रुव वर्ण * ध्रुव *shapers)
#ifndef HB_NO_BUFFER_VERIFY
  ;
#अन्यथा
  { प्रत्यागम सत्य; }
#endif

  अचिन्हित पूर्णाङ्क backtrack_len () ध्रुव { प्रत्यागम have_output ? out_len : idx; }
  अचिन्हित पूर्णाङ्क lookahead_len () ध्रुव { प्रत्यागम len - idx; }
  uint8_t next_serial () { प्रत्यागम ++serial ? serial : ++serial; }

  HB_INTERNAL शून्य add (hb_codepoint_t  codepoint,
			अचिन्हित पूर्णाङ्क    cluster);
  HB_INTERNAL शून्य add_info (ध्रुव hb_glyph_info_t &glyph_info);
  HB_INTERNAL शून्य add_info_and_pos (ध्रुव hb_glyph_info_t &glyph_info,
				     ध्रुव hb_glyph_position_t &glyph_pos);

  शून्य reverse_range (अचिन्हित start, अचिन्हित end)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (start, end);
    यदि (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (start, end);
  }
  शून्य reverse () { reverse_range (0, len); }

  ढाँचा <typename FuncType>
  शून्य reverse_groups (ध्रुव FuncType& group,
		       तार्किक merge_clusters = असत्य)
  {
    यदि (unlikely (!len))
      प्रत्यागम;

    अचिन्हित start = 0;
    अचिन्हित i;
    प्रति (i = 1; i < len; i++)
    {
      यदि (!group (info[i - 1], info[i]))
      {
	यदि (merge_clusters)
	  एतत्->merge_clusters (start, i);
	reverse_range (start, i);
	start = i;
      }
    }
    यदि (merge_clusters)
      एतत्->merge_clusters (start, i);
    reverse_range (start, i);

    reverse ();
  }

  ढाँचा <typename FuncType>
  अचिन्हित group_end (अचिन्हित start, ध्रुव FuncType& group) ध्रुव
  {
    यावत् (++start < len && group (info[start - 1], info[start]))
      ;

    प्रत्यागम start;
  }

  स्थिर तार्किक _cluster_group_func (ध्रुव hb_glyph_info_t& a,
				   ध्रुव hb_glyph_info_t& b)
  { प्रत्यागम a.cluster == b.cluster; }

  शून्य reverse_clusters () { reverse_groups (_cluster_group_func); }

  HB_INTERNAL शून्य guess_segment_properties ();

  HB_INTERNAL तार्किक समकाल ();
  HB_INTERNAL पूर्णाङ्क sync_so_far ();
  HB_INTERNAL शून्य clear_output ();
  HB_INTERNAL शून्य clear_positions ();

  ढाँचा <typename T>
  HB_NODISCARD तार्किक replace_glyphs (अचिन्हित पूर्णाङ्क अंक_in,
				    अचिन्हित पूर्णाङ्क अंक_out,
				    ध्रुव T *glyph_data)
  {
    यदि (unlikely (!make_room_for (अंक_in, अंक_out))) प्रत्यागम असत्य;

    प्रतिज्ञा (idx + अंक_in <= len);

    merge_clusters (idx, idx + अंक_in);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &out_info[out_len];
    प्रति (अचिन्हित पूर्णाङ्क i = 0; i < अंक_out; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_data[i];
      pinfo++;
    }

    idx  += अंक_in;
    out_len += अंक_out;
    प्रत्यागम सत्य;
  }

  HB_NODISCARD तार्किक replace_glyph (hb_codepoint_t glyph_index)
  { प्रत्यागम replace_glyphs (1, 1, &glyph_index); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD तार्किक output_glyph (hb_codepoint_t glyph_index)
  { प्रत्यागम replace_glyphs (0, 1, &glyph_index); }

  HB_NODISCARD तार्किक output_info (ध्रुव hb_glyph_info_t &glyph_info)
  {
    यदि (unlikely (!make_room_for (0, 1))) प्रत्यागम असत्य;

    out_info[out_len] = glyph_info;

    out_len++;
    प्रत्यागम सत्य;
  }
  /* Copies glyph at idx to output but doesn't advance idx */
  HB_NODISCARD तार्किक copy_glyph ()
  {
    /* Extra copy because cur()'s return can be freed within
     * output_info() call if buffer reallocates. */
    प्रत्यागम output_info (hb_glyph_info_t (cur()));
  }

  /* Copies glyph at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD तार्किक next_glyph ()
  {
    यदि (have_output)
    {
      यदि (out_info != info || out_len != idx)
      {
	यदि (unlikely (!ensure (out_len + 1))) प्रत्यागम असत्य;
	out_info[out_len] = info[idx];
      }
      out_len++;
    }

    idx++;
    प्रत्यागम सत्य;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD तार्किक next_glyphs (अचिन्हित पूर्णाङ्क n)
  {
    यदि (have_output)
    {
      यदि (out_info != info || out_len != idx)
      {
	यदि (unlikely (!ensure (out_len + n))) प्रत्यागम असत्य;
	स_हटाओ (out_info + out_len, info + idx, n * परिमाण (out_info[0]));
      }
      out_len += n;
    }

    idx += n;
    प्रत्यागम सत्य;
  }
  /* Advance idx without copying to output. */
  शून्य skip_glyph () { idx++; }
  शून्य add_masks (hb_mask_t mask)
  {
    प्रति (अचिन्हित पूर्णाङ्क j = 0; j < len; j++)
      info[j].mask |= mask;
  }
  HB_INTERNAL शून्य set_masks (hb_mask_t value, hb_mask_t mask,
			      अचिन्हित पूर्णाङ्क cluster_start, अचिन्हित पूर्णाङ्क cluster_end);

  शून्य merge_clusters (अचिन्हित पूर्णाङ्क start, अचिन्हित पूर्णाङ्क end)
  {
    यदि (end - start < 2)
      प्रत्यागम;
    यदि (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_to_break (start, end);
      प्रत्यागम;
    }
    merge_clusters_impl (start, end);
  }
  शून्य merge_grapheme_clusters (अचिन्हित पूर्णाङ्क start, अचिन्हित पूर्णाङ्क end)
  {
    यदि (end - start < 2)
      प्रत्यागम;
    यदि (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_to_break (start, end);
      प्रत्यागम;
    }
    merge_clusters_impl (start, end);
  }
  HB_INTERNAL शून्य merge_clusters_impl (अचिन्हित पूर्णाङ्क start, अचिन्हित पूर्णाङ्क end);
  शून्य merge_out_clusters (अचिन्हित पूर्णाङ्क start, अचिन्हित पूर्णाङ्क end)
  {
    यदि (end - start < 2)
      प्रत्यागम;
    यदि (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      प्रत्यागम;
    merge_out_clusters_impl (start, end);
  }
  शून्य merge_out_grapheme_clusters (अचिन्हित पूर्णाङ्क start, अचिन्हित पूर्णाङ्क end)
  {
    यदि (end - start < 2)
      प्रत्यागम;
    यदि (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      प्रत्यागम;
    merge_out_clusters_impl (start, end);
  }
  HB_INTERNAL शून्य merge_out_clusters_impl (अचिन्हित पूर्णाङ्क start, अचिन्हित पूर्णाङ्क end);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_INTERNAL शून्य delete_glyph ();
  HB_INTERNAL शून्य delete_glyphs_inplace (तार्किक (*filter) (ध्रुव hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  शून्य _set_glyph_flags_impl (hb_mask_t mask,
			      अचिन्हित start,
			      अचिन्हित end,
			      तार्किक interior,
			      तार्किक from_out_buffer)
  {
    यदि (!from_out_buffer || !have_output)
    {
      यदि (!interior)
      {
	प्रति (अचिन्हित i = start; i < end; i++)
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
      प्रतिज्ञा (start <= out_len);
      प्रतिज्ञा (idx <= end);

      यदि (!interior)
      {
	प्रति (अचिन्हित i = start; i < out_len; i++)
	  out_info[i].mask |= mask;
	प्रति (अचिन्हित i = idx; i < end; i++)
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
  शून्य _set_glyph_flags (hb_mask_t mask,
			 अचिन्हित start = 0,
			 अचिन्हित end = (अचिन्हित) -1,
			 तार्किक interior = असत्य,
			 तार्किक from_out_buffer = असत्य)
  {
    यदि (unlikely (end != (अचिन्हित) -1 && end - start > 255))
      प्रत्यागम;

    end = hb_min (end, len);

    यदि (interior && !from_out_buffer && end - start < 2)
      प्रत्यागम;

    _set_glyph_flags_impl (mask, start, end, interior, from_out_buffer);
  }


  शून्य unsafe_to_break (अचिन्हित पूर्णाङ्क start = 0, अचिन्हित पूर्णाङ्क end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      सत्य);
  }
  शून्य safe_to_जोड़ो_tatweel (अचिन्हित पूर्णाङ्क start = 0, अचिन्हित पूर्णाङ्क end = -1)
  {
    यदि ((flags & HB_BUFFER_FLAG_PRODUCE_SAFE_TO_जोड़ो_TATWEEL) == 0)
    {
      unsafe_to_break (start, end);
      प्रत्यागम;
    }
    _set_glyph_flags (HB_GLYPH_FLAG_SAFE_TO_जोड़ो_TATWEEL,
		      start, end,
		      सत्य);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  शून्य unsafe_to_concat (अचिन्हित पूर्णाङ्क start = 0, अचिन्हित पूर्णाङ्क end = -1)
  {
    यदि (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      प्रत्यागम;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      असत्य);
  }
  शून्य unsafe_to_break_from_outbuffer (अचिन्हित पूर्णाङ्क start = 0, अचिन्हित पूर्णाङ्क end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      सत्य, सत्य);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  शून्य unsafe_to_concat_from_outbuffer (अचिन्हित पूर्णाङ्क start = 0, अचिन्हित पूर्णाङ्क end = -1)
  {
    यदि (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      प्रत्यागम;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      असत्य, सत्य);
  }


  /* Internal methods */
  HB_NODISCARD HB_INTERNAL तार्किक move_to (अचिन्हित पूर्णाङ्क i); /* i is output-buffer index. */

  HB_NODISCARD HB_INTERNAL तार्किक enlarge (अचिन्हित पूर्णाङ्क size);

  HB_NODISCARD तार्किक resize (अचिन्हित length)
  {
    प्रतिज्ञा (!have_output);
    यदि (unlikely (!ensure (length))) प्रत्यागम असत्य;
    len = length;
    प्रत्यागम सत्य;
  }
  HB_NODISCARD तार्किक ensure (अचिन्हित पूर्णाङ्क size)
  { प्रत्यागम likely (!size || size < allocated) ? सत्य : enlarge (size); }

  HB_NODISCARD तार्किक ensure_inplace (अचिन्हित पूर्णाङ्क size)
  { प्रत्यागम likely (!size || size < allocated); }

  शून्य assert_glyphs ()
  {
    प्रतिज्ञा ((content_type == HB_BUFFER_CONTENT_TYPE_GLYPHS) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  शून्य assert_unicode ()
  {
    प्रतिज्ञा ((content_type == HB_BUFFER_CONTENT_TYPE_UNICODE) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  HB_NODISCARD तार्किक ensure_glyphs ()
  {
    यदि (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_GLYPHS))
    {
      यदि (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	प्रत्यागम असत्य;
      प्रतिज्ञा (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_GLYPHS;
    }
    प्रत्यागम सत्य;
  }
  HB_NODISCARD तार्किक ensure_unicode ()
  {
    यदि (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_UNICODE))
    {
      यदि (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	प्रत्यागम असत्य;
      प्रतिज्ञा (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_UNICODE;
    }
    प्रत्यागम सत्य;
  }

  HB_NODISCARD HB_INTERNAL तार्किक make_room_for (अचिन्हित पूर्णाङ्क अंक_in, अचिन्हित पूर्णाङ्क अंक_out);
  HB_NODISCARD HB_INTERNAL तार्किक shift_forward (अचिन्हित पूर्णाङ्क गिनती);

  प्रकारनिर्देश दीर्घ scratch_buffer_t;
  HB_INTERNAL scratch_buffer_t *get_scratch_buffer (अचिन्हित पूर्णाङ्क *size);

  शून्य clear_context (अचिन्हित पूर्णाङ्क side) { context_len[side] = 0; }

  HB_INTERNAL शून्य sort (अचिन्हित पूर्णाङ्क start, अचिन्हित पूर्णाङ्क end, पूर्णाङ्क(*compar)(ध्रुव hb_glyph_info_t *, ध्रुव hb_glyph_info_t *));

  तार्किक messaging ()
  {
#ifdef HB_NO_BUFFER_MESSAGE
    प्रत्यागम असत्य;
#अन्यथा
    प्रत्यागम unlikely (message_func);
#endif
  }
  तार्किक message (hb_font_t *font, ध्रुव वर्ण *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#ifdef HB_NO_BUFFER_MESSAGE
    प्रत्यागम सत्य;
#अन्यथा
    यदि (likely (!messaging ()))
      प्रत्यागम सत्य;

    बहु_सूची ap;
    बहु_शुरू (ap, fmt);
    तार्किक ret = message_impl (font, fmt, ap);
    बहु_पूर्ण (ap);

    प्रत्यागम ret;
#endif
  }
  HB_INTERNAL तार्किक message_impl (hb_font_t *font, ध्रुव वर्ण *fmt, बहु_सूची ap) HB_PRINTF_FUNC(3, 0);
  HB_INTERNAL शून्य changed ();

  स्थिर शून्य
  set_cluster (hb_glyph_info_t &inf, अचिन्हित पूर्णाङ्क cluster, अचिन्हित पूर्णाङ्क mask = 0)
  {
    यदि (inf.cluster != cluster)
      inf.mask = (inf.mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  शून्य
  _infos_set_glyph_flags (hb_glyph_info_t *infos,
			  अचिन्हित पूर्णाङ्क start, अचिन्हित पूर्णाङ्क end,
			  अचिन्हित पूर्णाङ्क cluster,
			  hb_mask_t mask)
  {
    यदि (unlikely (start == end))
      प्रत्यागम;

    max_ops -= end - start;
    यदि (unlikely (max_ops < 0))
      successful = असत्य;

    अचिन्हित cluster_first = infos[start].cluster;
    अचिन्हित cluster_last = infos[end - 1].cluster;

    यदि (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_first && cluster != cluster_last))
    {
      प्रति (अचिन्हित पूर्णाङ्क i = start; i < end; i++)
	यदि (cluster != infos[i].cluster)
	  infos[i].mask |= mask;
      प्रत्यागम;
    }

    /* Monotone clusters */

    यदि (cluster == cluster_first)
    {
      प्रति (अचिन्हित पूर्णाङ्क i = end; start < i && infos[i - 1].cluster != cluster_first; i--)
	infos[i - 1].mask |= mask;
    }
    अन्यथा /* cluster == cluster_last */
    {
      प्रति (अचिन्हित पूर्णाङ्क i = start; i < end && infos[i].cluster != cluster_last; i++)
	infos[i].mask |= mask;
    }
  }
  अचिन्हित
  _infos_find_min_cluster (ध्रुव hb_glyph_info_t *infos,
			   अचिन्हित start, अचिन्हित end,
			   अचिन्हित cluster = अच_पूर्णांक_उच्च)
  {
    यदि (unlikely (start == end))
      प्रत्यागम cluster;

    यदि (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS)
    {
      प्रति (अचिन्हित पूर्णाङ्क i = start; i < end; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      प्रत्यागम cluster;
    }

    प्रत्यागम hb_min (cluster, hb_min (infos[start].cluster, infos[end - 1].cluster));
  }

  शून्य clear_glyph_flags (hb_mask_t mask = 0)
  {
    प्रति (अचिन्हित पूर्णाङ्क i = 0; i < len; i++)
      info[i].mask = (info[i].mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_buffer_t);


#define foreach_group(buffer, start, end, group_func) \
  प्रति (अचिन्हित पूर्णाङ्क \
       गिनती_2 = buffer->len, \
       start = 0, end = गिनती_2 ? buffer->group_end (0, group_func) : 0; \
       start < गिनती_2; \
       start = end, end = buffer->group_end (start, group_func))

#define foreach_cluster(buffer, start, end) \
	foreach_group (buffer, start, end, hb_buffer_t::_cluster_group_func)


#define HB_BUFFER_XALLOCATE_VAR(b, func, var) \
  b->func (दुरत्व (hb_glyph_info_t, var) - दुरत्व(hb_glyph_info_t, var1), \
	   परिमाण (b->info[0].var))
#define HB_BUFFER_ALLOCATE_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, allocate_var,     var ())
#define HB_BUFFER_TRY_ALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, try_allocate_var, var ())
#define HB_BUFFER_DEALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, deallocate_var,   var ())
#define HB_BUFFER_ASSERT_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, assert_var,       var ())


#endif /* HB_BUFFER_HH */
