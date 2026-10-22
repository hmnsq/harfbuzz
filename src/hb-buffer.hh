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

#شامل "hb.hh"
#شامل "hb-unicode.hh"
#شامل "hb-set-digest.hh"


static_assert ((اندازه (hb_glyph_info_t) == 20), "");
static_assert ((اندازه (hb_glyph_info_t) == اندازه (hb_glyph_position_t)), "");

HB_MARK_AS_FLAG_T (hb_glyph_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_serialize_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_diff_flags_t);

شمارش hb_buffer_scratch_flags_t {
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

ساختار hb_buffer_t
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

  منطقی successful; /* Allocations successful */
  منطقی have_output; /* Whether we have an output buffer going on */
  منطقی have_positions; /* Whether we have positions */

  unsigned صحیح idx; /* Cursor into ->info and ->pos arrays */
  unsigned صحیح len; /* Length of ->info and ->pos arrays */
  unsigned صحیح out_len; /* Length of ->out_info array if have_output */

  unsigned صحیح allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *out_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  ایستا constexpr unsigned CONTEXT_LENGTH = 5u;
  hb_codepoint_t context[2][CONTEXT_LENGTH];
  unsigned صحیح context_len[2];

  hb_set_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_bits;
  uint8_t serial;
  uint32_t random_state;
  hb_buffer_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  unsigned صحیح max_len; /* Maximum allowed len. */
  صحیح max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#ifndef HB_NO_BUFFER_MESSAGE
  تعریفنوع تهی (*changed_func_t) (hb_buffer_t *buffer, تهی *user_data);

  hb_buffer_message_func_t message_func;
  تهی *message_data;
  hb_destroy_func_t message_destroy;
  changed_func_t changed_func;
  تهی *changed_data;
  unsigned message_depth; /* How deeply are we inside a message callback? */
#وگرنه
  ایستا constexpr unsigned message_depth = 0u;
#endif



  /* Methods */

  HB_NODISCARD منطقی in_error () ثابت { بازگشت !successful; }

  تهی allocate_var (unsigned صحیح start, unsigned صحیح شمار)
  {
    unsigned صحیح end = start + شمار;
    أكد (end <= 8);
    unsigned صحیح bits = (1u<<end) - (1u<<start);
    أكد (0 == (allocated_var_bits & bits));
    allocated_var_bits |= bits;
  }
  منطقی try_allocate_var (unsigned صحیح start, unsigned صحیح شمار)
  {
    unsigned صحیح end = start + شمار;
    أكد (end <= 8);
    unsigned صحیح bits = (1u<<end) - (1u<<start);
    اگر (allocated_var_bits & bits)
      بازگشت نادرست;
    allocated_var_bits |= bits;
    بازگشت درست;
  }
  تهی deallocate_var (unsigned صحیح start, unsigned صحیح شمار)
  {
    unsigned صحیح end = start + شمار;
    أكد (end <= 8);
    unsigned صحیح bits = (1u<<end) - (1u<<start);
    أكد (bits == (allocated_var_bits & bits));
    allocated_var_bits &= ~bits;
  }
  تهی assert_var (unsigned صحیح start, unsigned صحیح شمار)
  {
    unsigned صحیح end = start + شمار;
    أكد (end <= 8);
    HB_UNUSED unsigned صحیح bits = (1u<<end) - (1u<<start);
    أكد (bits == (allocated_var_bits & bits));
  }
  تهی deallocate_var_all ()
  {
    allocated_var_bits = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (unsigned صحیح i = 0) { بازگشت info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (unsigned صحیح i = 0) ثابت { بازگشت info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (unsigned صحیح i = 0) { بازگشت pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (unsigned صحیح i = 0) ثابت { بازگشت pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { بازگشت out_info[out_len ? out_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () ثابت { بازگشت out_info[out_len ? out_len - 1 : 0]; }

  template <typename set_t>
  تهی collect_codepoints (set_t &d) ثابت
  { d.clear (); d.add_array (&info[0].codepoint, len, اندازه (info[0])); }

  تهی update_digest ()
  {
    digest = hb_set_digest_t ();
    collect_codepoints (digest);
  }

  HB_INTERNAL تهی similar (ثابت hb_buffer_t &src);
  HB_INTERNAL تهی reset ();
  HB_INTERNAL تهی clear ();

  /* Called around shape() */
  HB_INTERNAL تهی enter ();
  HB_INTERNAL تهی leave ();

#ifndef HB_NO_BUFFER_VERIFY
  HB_INTERNAL
#endif
  منطقی verify (hb_buffer_t        *text_buffer,
	       hb_font_t          *font,
	       ثابت hb_feature_t *features,
	       unsigned صحیح        رقم_features,
	       ثابت نویسه * ثابت *shapers)
#ifndef HB_NO_BUFFER_VERIFY
  ;
#وگرنه
  { بازگشت درست; }
#endif

  unsigned صحیح backtrack_len () ثابت { بازگشت have_output ? out_len : idx; }
  unsigned صحیح lookahead_len () ثابت { بازگشت len - idx; }
  uint8_t next_serial () { بازگشت ++serial ? serial : ++serial; }

  HB_INTERNAL تهی add (hb_codepoint_t  codepoint,
			unsigned صحیح    cluster);
  HB_INTERNAL تهی add_info (ثابت hb_glyph_info_t &glyph_info);
  HB_INTERNAL تهی add_info_and_pos (ثابت hb_glyph_info_t &glyph_info,
				     ثابت hb_glyph_position_t &glyph_pos);

  تهی reverse_range (unsigned start, unsigned end)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (start, end);
    اگر (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (start, end);
  }
  تهی reverse () { reverse_range (0, len); }

  template <typename FuncType>
  تهی reverse_groups (ثابت FuncType& group,
		       منطقی merge_clusters = نادرست)
  {
    اگر (unlikely (!len))
      بازگشت;

    unsigned start = 0;
    unsigned i;
    برای (i = 1; i < len; i++)
    {
      اگر (!group (info[i - 1], info[i]))
      {
	اگر (merge_clusters)
	  این->merge_clusters (start, i);
	reverse_range (start, i);
	start = i;
      }
    }
    اگر (merge_clusters)
      این->merge_clusters (start, i);
    reverse_range (start, i);

    reverse ();
  }

  template <typename FuncType>
  unsigned group_end (unsigned start, ثابت FuncType& group) ثابت
  {
    وقتی (++start < len && group (info[start - 1], info[start]))
      ;

    بازگشت start;
  }

  ایستا منطقی _cluster_group_func (ثابت hb_glyph_info_t& a,
				   ثابت hb_glyph_info_t& b)
  { بازگشت a.cluster == b.cluster; }

  تهی reverse_clusters () { reverse_groups (_cluster_group_func); }

  HB_INTERNAL تهی guess_segment_properties ();

  HB_INTERNAL منطقی همگام ();
  HB_INTERNAL صحیح sync_so_far ();
  HB_INTERNAL تهی clear_output ();
  HB_INTERNAL تهی clear_positions ();

  template <typename T>
  HB_NODISCARD منطقی replace_glyphs (unsigned صحیح رقم_in,
				    unsigned صحیح رقم_out,
				    ثابت T *glyph_data)
  {
    اگر (unlikely (!make_room_for (رقم_in, رقم_out))) بازگشت نادرست;

    أكد (idx + رقم_in <= len);

    merge_clusters (idx, idx + رقم_in);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &out_info[out_len];
    برای (unsigned صحیح i = 0; i < رقم_out; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_data[i];
      pinfo++;
    }

    idx  += رقم_in;
    out_len += رقم_out;
    بازگشت درست;
  }

  HB_NODISCARD منطقی replace_glyph (hb_codepoint_t glyph_index)
  { بازگشت replace_glyphs (1, 1, &glyph_index); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD منطقی output_glyph (hb_codepoint_t glyph_index)
  { بازگشت replace_glyphs (0, 1, &glyph_index); }

  HB_NODISCARD منطقی output_info (ثابت hb_glyph_info_t &glyph_info)
  {
    اگر (unlikely (!make_room_for (0, 1))) بازگشت نادرست;

    out_info[out_len] = glyph_info;

    out_len++;
    بازگشت درست;
  }
  /* Copies glyph at idx to output but doesn't advance idx */
  HB_NODISCARD منطقی copy_glyph ()
  {
    /* Extra copy because cur()'s return can be freed within
     * output_info() call if buffer reallocates. */
    بازگشت output_info (hb_glyph_info_t (cur()));
  }

  /* Copies glyph at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD منطقی next_glyph ()
  {
    اگر (have_output)
    {
      اگر (out_info != info || out_len != idx)
      {
	اگر (unlikely (!ensure (out_len + 1))) بازگشت نادرست;
	out_info[out_len] = info[idx];
      }
      out_len++;
    }

    idx++;
    بازگشت درست;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD منطقی next_glyphs (unsigned صحیح n)
  {
    اگر (have_output)
    {
      اگر (out_info != info || out_len != idx)
      {
	اگر (unlikely (!ensure (out_len + n))) بازگشت نادرست;
	memmove (out_info + out_len, info + idx, n * اندازه (out_info[0]));
      }
      out_len += n;
    }

    idx += n;
    بازگشت درست;
  }
  /* Advance idx without copying to output. */
  تهی skip_glyph () { idx++; }
  تهی add_masks (hb_mask_t mask)
  {
    برای (unsigned صحیح j = 0; j < len; j++)
      info[j].mask |= mask;
  }
  HB_INTERNAL تهی set_masks (hb_mask_t value, hb_mask_t mask,
			      unsigned صحیح cluster_start, unsigned صحیح cluster_end);

  تهی merge_clusters (unsigned صحیح start, unsigned صحیح end)
  {
    اگر (end - start < 2)
      بازگشت;
    اگر (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_to_break (start, end);
      بازگشت;
    }
    merge_clusters_impl (start, end);
  }
  تهی merge_grapheme_clusters (unsigned صحیح start, unsigned صحیح end)
  {
    اگر (end - start < 2)
      بازگشت;
    اگر (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_to_break (start, end);
      بازگشت;
    }
    merge_clusters_impl (start, end);
  }
  HB_INTERNAL تهی merge_clusters_impl (unsigned صحیح start, unsigned صحیح end);
  تهی merge_out_clusters (unsigned صحیح start, unsigned صحیح end)
  {
    اگر (end - start < 2)
      بازگشت;
    اگر (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      بازگشت;
    merge_out_clusters_impl (start, end);
  }
  تهی merge_out_grapheme_clusters (unsigned صحیح start, unsigned صحیح end)
  {
    اگر (end - start < 2)
      بازگشت;
    اگر (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      بازگشت;
    merge_out_clusters_impl (start, end);
  }
  HB_INTERNAL تهی merge_out_clusters_impl (unsigned صحیح start, unsigned صحیح end);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_INTERNAL تهی delete_glyph ();
  HB_INTERNAL تهی delete_glyphs_inplace (منطقی (*filter) (ثابت hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  تهی _set_glyph_flags_impl (hb_mask_t mask,
			      unsigned start,
			      unsigned end,
			      منطقی interior,
			      منطقی from_out_buffer)
  {
    اگر (!from_out_buffer || !have_output)
    {
      اگر (!interior)
      {
	برای (unsigned i = start; i < end; i++)
	  info[i].mask |= mask;
      }
      وگرنه
      {
	unsigned cluster = _infos_find_min_cluster (info, start, end);
	_infos_set_glyph_flags (info, start, end, cluster, mask);
      }
    }
    وگرنه
    {
      أكد (start <= out_len);
      أكد (idx <= end);

      اگر (!interior)
      {
	برای (unsigned i = start; i < out_len; i++)
	  out_info[i].mask |= mask;
	برای (unsigned i = idx; i < end; i++)
	  info[i].mask |= mask;
      }
      وگرنه
      {
	unsigned cluster = _infos_find_min_cluster (info, idx, end);
	cluster = _infos_find_min_cluster (out_info, start, out_len, cluster);

	_infos_set_glyph_flags (out_info, start, out_len, cluster, mask);
	_infos_set_glyph_flags (info, idx, end, cluster, mask);
      }
    }
  }

  HB_ALWAYS_INLINE
  تهی _set_glyph_flags (hb_mask_t mask,
			 unsigned start = 0,
			 unsigned end = (unsigned) -1,
			 منطقی interior = نادرست,
			 منطقی from_out_buffer = نادرست)
  {
    اگر (unlikely (end != (unsigned) -1 && end - start > 255))
      بازگشت;

    end = hb_min (end, len);

    اگر (interior && !from_out_buffer && end - start < 2)
      بازگشت;

    _set_glyph_flags_impl (mask, start, end, interior, from_out_buffer);
  }


  تهی unsafe_to_break (unsigned صحیح start = 0, unsigned صحیح end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      درست);
  }
  تهی safe_to_درج_tatweel (unsigned صحیح start = 0, unsigned صحیح end = -1)
  {
    اگر ((flags & HB_BUFFER_FLAG_PRODUCE_SAFE_TO_درج_TATWEEL) == 0)
    {
      unsafe_to_break (start, end);
      بازگشت;
    }
    _set_glyph_flags (HB_GLYPH_FLAG_SAFE_TO_درج_TATWEEL,
		      start, end,
		      درست);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  تهی unsafe_to_concat (unsigned صحیح start = 0, unsigned صحیح end = -1)
  {
    اگر (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      بازگشت;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      نادرست);
  }
  تهی unsafe_to_break_from_outbuffer (unsigned صحیح start = 0, unsigned صحیح end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      درست, درست);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  تهی unsafe_to_concat_from_outbuffer (unsigned صحیح start = 0, unsigned صحیح end = -1)
  {
    اگر (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      بازگشت;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      نادرست, درست);
  }


  /* Internal methods */
  HB_NODISCARD HB_INTERNAL منطقی move_to (unsigned صحیح i); /* i is output-buffer index. */

  HB_NODISCARD HB_INTERNAL منطقی enlarge (unsigned صحیح size);

  HB_NODISCARD منطقی resize (unsigned length)
  {
    أكد (!have_output);
    اگر (unlikely (!ensure (length))) بازگشت نادرست;
    len = length;
    بازگشت درست;
  }
  HB_NODISCARD منطقی ensure (unsigned صحیح size)
  { بازگشت likely (!size || size < allocated) ? درست : enlarge (size); }

  HB_NODISCARD منطقی ensure_inplace (unsigned صحیح size)
  { بازگشت likely (!size || size < allocated); }

  تهی assert_glyphs ()
  {
    أكد ((content_type == HB_BUFFER_CONTENT_TYPE_GLYPHS) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  تهی assert_unicode ()
  {
    أكد ((content_type == HB_BUFFER_CONTENT_TYPE_UNICODE) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  HB_NODISCARD منطقی ensure_glyphs ()
  {
    اگر (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_GLYPHS))
    {
      اگر (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	بازگشت نادرست;
      أكد (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_GLYPHS;
    }
    بازگشت درست;
  }
  HB_NODISCARD منطقی ensure_unicode ()
  {
    اگر (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_UNICODE))
    {
      اگر (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	بازگشت نادرست;
      أكد (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_UNICODE;
    }
    بازگشت درست;
  }

  HB_NODISCARD HB_INTERNAL منطقی make_room_for (unsigned صحیح رقم_in, unsigned صحیح رقم_out);
  HB_NODISCARD HB_INTERNAL منطقی shift_forward (unsigned صحیح شمار);

  تعریفنوع long scratch_buffer_t;
  HB_INTERNAL scratch_buffer_t *get_scratch_buffer (unsigned صحیح *size);

  تهی clear_context (unsigned صحیح side) { context_len[side] = 0; }

  HB_INTERNAL تهی sort (unsigned صحیح start, unsigned صحیح end, صحیح(*compar)(ثابت hb_glyph_info_t *, ثابت hb_glyph_info_t *));

  منطقی messaging ()
  {
#ifdef HB_NO_BUFFER_MESSAGE
    بازگشت نادرست;
#وگرنه
    بازگشت unlikely (message_func);
#endif
  }
  منطقی message (hb_font_t *font, ثابت نویسه *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#ifdef HB_NO_BUFFER_MESSAGE
    بازگشت درست;
#وگرنه
    اگر (likely (!messaging ()))
      بازگشت درست;

    va_list ap;
    va_start (ap, fmt);
    منطقی ret = message_impl (font, fmt, ap);
    va_end (ap);

    بازگشت ret;
#endif
  }
  HB_INTERNAL منطقی message_impl (hb_font_t *font, ثابت نویسه *fmt, va_list ap) HB_PRINTF_FUNC(3, 0);
  HB_INTERNAL تهی changed ();

  ایستا تهی
  set_cluster (hb_glyph_info_t &inf, unsigned صحیح cluster, unsigned صحیح mask = 0)
  {
    اگر (inf.cluster != cluster)
      inf.mask = (inf.mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  تهی
  _infos_set_glyph_flags (hb_glyph_info_t *infos,
			  unsigned صحیح start, unsigned صحیح end,
			  unsigned صحیح cluster,
			  hb_mask_t mask)
  {
    اگر (unlikely (start == end))
      بازگشت;

    max_ops -= end - start;
    اگر (unlikely (max_ops < 0))
      successful = نادرست;

    unsigned cluster_first = infos[start].cluster;
    unsigned cluster_last = infos[end - 1].cluster;

    اگر (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_first && cluster != cluster_last))
    {
      برای (unsigned صحیح i = start; i < end; i++)
	اگر (cluster != infos[i].cluster)
	  infos[i].mask |= mask;
      بازگشت;
    }

    /* Monotone clusters */

    اگر (cluster == cluster_first)
    {
      برای (unsigned صحیح i = end; start < i && infos[i - 1].cluster != cluster_first; i--)
	infos[i - 1].mask |= mask;
    }
    وگرنه /* cluster == cluster_last */
    {
      برای (unsigned صحیح i = start; i < end && infos[i].cluster != cluster_last; i++)
	infos[i].mask |= mask;
    }
  }
  unsigned
  _infos_find_min_cluster (ثابت hb_glyph_info_t *infos,
			   unsigned start, unsigned end,
			   unsigned cluster = UINT_MAX)
  {
    اگر (unlikely (start == end))
      بازگشت cluster;

    اگر (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS)
    {
      برای (unsigned صحیح i = start; i < end; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      بازگشت cluster;
    }

    بازگشت hb_min (cluster, hb_min (infos[start].cluster, infos[end - 1].cluster));
  }

  تهی clear_glyph_flags (hb_mask_t mask = 0)
  {
    برای (unsigned صحیح i = 0; i < len; i++)
      info[i].mask = (info[i].mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_buffer_t);


#define foreach_group(buffer, start, end, group_func) \
  برای (unsigned صحیح \
       شمار_2 = buffer->len, \
       start = 0, end = شمار_2 ? buffer->group_end (0, group_func) : 0; \
       start < شمار_2; \
       start = end, end = buffer->group_end (start, group_func))

#define foreach_cluster(buffer, start, end) \
	foreach_group (buffer, start, end, hb_buffer_t::_cluster_group_func)


#define HB_BUFFER_XALLOCATE_VAR(b, func, var) \
  b->func (offsetof (hb_glyph_info_t, var) - offsetof(hb_glyph_info_t, var1), \
	   اندازه (b->info[0].var))
#define HB_BUFFER_ALLOCATE_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, allocate_var,     var ())
#define HB_BUFFER_TRY_ALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, try_allocate_var, var ())
#define HB_BUFFER_DEALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, deallocate_var,   var ())
#define HB_BUFFER_ASSERT_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, assert_var,       var ())


#endif /* HB_BUFFER_HH */
