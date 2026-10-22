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

#ضمن "hb.hh"
#ضمن "hb-unicode.hh"
#ضمن "hb-set-digest.hh"


static_assert ((حجم (hb_glyph_info_t) == 20), "");
static_assert ((حجم (hb_glyph_info_t) == حجم (hb_glyph_position_t)), "");

HB_MARK_AS_FLAG_T (hb_glyph_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_serialize_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_diff_flags_t);

تعداد hb_buffer_scratch_flags_t {
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

بنية hb_buffer_t
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

  منطقي successful; /* Allocations successful */
  منطقي have_output; /* Whether we have an output buffer going on */
  منطقي have_positions; /* Whether we have positions */

  unsigned صحيح idx; /* Cursor into ->info and ->pos arrays */
  unsigned صحيح len; /* Length of ->info and ->pos arrays */
  unsigned صحيح out_len; /* Length of ->out_info array if have_output */

  unsigned صحيح allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *out_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  ساكن constexpr unsigned CONTEXT_LENGTH = 5u;
  hb_codepoint_t context[2][CONTEXT_LENGTH];
  unsigned صحيح context_len[2];

  hb_set_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_bits;
  uint8_t serial;
  uint32_t random_state;
  hb_buffer_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  unsigned صحيح max_len; /* Maximum allowed len. */
  صحيح max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#ifndef HB_NO_BUFFER_MESSAGE
  تعريفنوع فراغ (*changed_func_t) (hb_buffer_t *buffer, فراغ *user_data);

  hb_buffer_message_func_t message_func;
  فراغ *message_data;
  hb_destroy_func_t message_destroy;
  changed_func_t changed_func;
  فراغ *changed_data;
  unsigned message_depth; /* How deeply are we inside a message callback? */
#وإلا
  ساكن constexpr unsigned message_depth = 0u;
#endif



  /* Methods */

  HB_NODISCARD منطقي in_error () ثابت { أرجع !successful; }

  فراغ allocate_var (unsigned صحيح start, unsigned صحيح count)
  {
    unsigned صحيح end = start + count;
    أكد (end <= 8);
    unsigned صحيح bits = (1u<<end) - (1u<<start);
    أكد (0 == (allocated_var_bits & bits));
    allocated_var_bits |= bits;
  }
  منطقي try_allocate_var (unsigned صحيح start, unsigned صحيح count)
  {
    unsigned صحيح end = start + count;
    أكد (end <= 8);
    unsigned صحيح bits = (1u<<end) - (1u<<start);
    𐡄𐡍 (allocated_var_bits & bits)
      أرجع خطأ;
    allocated_var_bits |= bits;
    أرجع 𐡒𐡔𐡈;
  }
  فراغ deallocate_var (unsigned صحيح start, unsigned صحيح count)
  {
    unsigned صحيح end = start + count;
    أكد (end <= 8);
    unsigned صحيح bits = (1u<<end) - (1u<<start);
    أكد (bits == (allocated_var_bits & bits));
    allocated_var_bits &= ~bits;
  }
  فراغ assert_var (unsigned صحيح start, unsigned صحيح count)
  {
    unsigned صحيح end = start + count;
    أكد (end <= 8);
    HB_UNUSED unsigned صحيح bits = (1u<<end) - (1u<<start);
    أكد (bits == (allocated_var_bits & bits));
  }
  فراغ deallocate_var_all ()
  {
    allocated_var_bits = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (unsigned صحيح i = 0) { أرجع info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (unsigned صحيح i = 0) ثابت { أرجع info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (unsigned صحيح i = 0) { أرجع pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (unsigned صحيح i = 0) ثابت { أرجع pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { أرجع out_info[out_len ? out_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () ثابت { أرجع out_info[out_len ? out_len - 1 : 0]; }

  template <typename set_t>
  فراغ collect_codepoints (set_t &d) ثابت
  { d.clear (); d.add_array (&info[0].codepoint, len, حجم (info[0])); }

  فراغ update_digest ()
  {
    digest = hb_set_digest_t ();
    collect_codepoints (digest);
  }

  HB_INTERNAL فراغ similar (ثابت hb_buffer_t &src);
  HB_INTERNAL فراغ reset ();
  HB_INTERNAL فراغ clear ();

  /* Called around shape() */
  HB_INTERNAL فراغ enter ();
  HB_INTERNAL فراغ leave ();

#ifndef HB_NO_BUFFER_VERIFY
  HB_INTERNAL
#endif
  منطقي verify (hb_buffer_t        *text_buffer,
	       hb_font_t          *font,
	       ثابت hb_feature_t *features,
	       unsigned صحيح        num_features,
	       ثابت حرف * ثابت *shapers)
#ifndef HB_NO_BUFFER_VERIFY
  ;
#وإلا
  { أرجع 𐡒𐡔𐡈; }
#endif

  unsigned صحيح backtrack_len () ثابت { أرجع have_output ? out_len : idx; }
  unsigned صحيح lookahead_len () ثابت { أرجع len - idx; }
  uint8_t next_serial () { أرجع ++serial ? serial : ++serial; }

  HB_INTERNAL فراغ add (hb_codepoint_t  codepoint,
			unsigned صحيح    cluster);
  HB_INTERNAL فراغ add_info (ثابت hb_glyph_info_t &glyph_info);
  HB_INTERNAL فراغ add_info_and_pos (ثابت hb_glyph_info_t &glyph_info,
				     ثابت hb_glyph_position_t &glyph_pos);

  فراغ reverse_range (unsigned start, unsigned end)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (start, end);
    𐡄𐡍 (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (start, end);
  }
  فراغ reverse () { reverse_range (0, len); }

  template <typename FuncType>
  فراغ reverse_groups (ثابت FuncType& group,
		       منطقي merge_clusters = خطأ)
  {
    𐡄𐡍 (unlikely (!len))
      أرجع;

    unsigned start = 0;
    unsigned i;
    لكل (i = 1; i < len; i++)
    {
      𐡄𐡍 (!group (info[i - 1], info[i]))
      {
	𐡄𐡍 (merge_clusters)
	  هذا->merge_clusters (start, i);
	reverse_range (start, i);
	start = i;
      }
    }
    𐡄𐡍 (merge_clusters)
      هذا->merge_clusters (start, i);
    reverse_range (start, i);

    reverse ();
  }

  template <typename FuncType>
  unsigned group_end (unsigned start, ثابت FuncType& group) ثابت
  {
    طالما (++start < len && group (info[start - 1], info[start]))
      ;

    أرجع start;
  }

  ساكن منطقي _cluster_group_func (ثابت hb_glyph_info_t& a,
				   ثابت hb_glyph_info_t& b)
  { أرجع a.cluster == b.cluster; }

  فراغ reverse_clusters () { reverse_groups (_cluster_group_func); }

  HB_INTERNAL فراغ guess_segment_properties ();

  HB_INTERNAL منطقي متزامن ();
  HB_INTERNAL صحيح sync_so_far ();
  HB_INTERNAL فراغ clear_output ();
  HB_INTERNAL فراغ clear_positions ();

  template <typename T>
  HB_NODISCARD منطقي replace_glyphs (unsigned صحيح num_in,
				    unsigned صحيح num_out,
				    ثابت T *glyph_data)
  {
    𐡄𐡍 (unlikely (!make_room_for (num_in, num_out))) أرجع خطأ;

    أكد (idx + num_in <= len);

    merge_clusters (idx, idx + num_in);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &out_info[out_len];
    لكل (unsigned صحيح i = 0; i < num_out; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_data[i];
      pinfo++;
    }

    idx  += num_in;
    out_len += num_out;
    أرجع 𐡒𐡔𐡈;
  }

  HB_NODISCARD منطقي replace_glyph (hb_codepoint_t glyph_index)
  { أرجع replace_glyphs (1, 1, &glyph_index); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD منطقي output_glyph (hb_codepoint_t glyph_index)
  { أرجع replace_glyphs (0, 1, &glyph_index); }

  HB_NODISCARD منطقي output_info (ثابت hb_glyph_info_t &glyph_info)
  {
    𐡄𐡍 (unlikely (!make_room_for (0, 1))) أرجع خطأ;

    out_info[out_len] = glyph_info;

    out_len++;
    أرجع 𐡒𐡔𐡈;
  }
  /* Copies glyph at idx to output but doesn't advance idx */
  HB_NODISCARD منطقي copy_glyph ()
  {
    /* Extra copy because cur()'s return can be freed within
     * output_info() call if buffer reallocates. */
    أرجع output_info (hb_glyph_info_t (cur()));
  }

  /* Copies glyph at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD منطقي next_glyph ()
  {
    𐡄𐡍 (have_output)
    {
      𐡄𐡍 (out_info != info || out_len != idx)
      {
	𐡄𐡍 (unlikely (!ensure (out_len + 1))) أرجع خطأ;
	out_info[out_len] = info[idx];
      }
      out_len++;
    }

    idx++;
    أرجع 𐡒𐡔𐡈;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD منطقي next_glyphs (unsigned صحيح n)
  {
    𐡄𐡍 (have_output)
    {
      𐡄𐡍 (out_info != info || out_len != idx)
      {
	𐡄𐡍 (unlikely (!ensure (out_len + n))) أرجع خطأ;
	memmove (out_info + out_len, info + idx, n * حجم (out_info[0]));
      }
      out_len += n;
    }

    idx += n;
    أرجع 𐡒𐡔𐡈;
  }
  /* Advance idx without copying to output. */
  فراغ skip_glyph () { idx++; }
  فراغ add_masks (hb_mask_t mask)
  {
    لكل (unsigned صحيح j = 0; j < len; j++)
      info[j].mask |= mask;
  }
  HB_INTERNAL فراغ set_masks (hb_mask_t value, hb_mask_t mask,
			      unsigned صحيح cluster_start, unsigned صحيح cluster_end);

  فراغ merge_clusters (unsigned صحيح start, unsigned صحيح end)
  {
    𐡄𐡍 (end - start < 2)
      أرجع;
    𐡄𐡍 (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_to_break (start, end);
      أرجع;
    }
    merge_clusters_impl (start, end);
  }
  فراغ merge_grapheme_clusters (unsigned صحيح start, unsigned صحيح end)
  {
    𐡄𐡍 (end - start < 2)
      أرجع;
    𐡄𐡍 (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_to_break (start, end);
      أرجع;
    }
    merge_clusters_impl (start, end);
  }
  HB_INTERNAL فراغ merge_clusters_impl (unsigned صحيح start, unsigned صحيح end);
  فراغ merge_out_clusters (unsigned صحيح start, unsigned صحيح end)
  {
    𐡄𐡍 (end - start < 2)
      أرجع;
    𐡄𐡍 (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      أرجع;
    merge_out_clusters_impl (start, end);
  }
  فراغ merge_out_grapheme_clusters (unsigned صحيح start, unsigned صحيح end)
  {
    𐡄𐡍 (end - start < 2)
      أرجع;
    𐡄𐡍 (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      أرجع;
    merge_out_clusters_impl (start, end);
  }
  HB_INTERNAL فراغ merge_out_clusters_impl (unsigned صحيح start, unsigned صحيح end);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_INTERNAL فراغ delete_glyph ();
  HB_INTERNAL فراغ delete_glyphs_inplace (منطقي (*filter) (ثابت hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  فراغ _set_glyph_flags_impl (hb_mask_t mask,
			      unsigned start,
			      unsigned end,
			      منطقي interior,
			      منطقي from_out_buffer)
  {
    𐡄𐡍 (!from_out_buffer || !have_output)
    {
      𐡄𐡍 (!interior)
      {
	لكل (unsigned i = start; i < end; i++)
	  info[i].mask |= mask;
      }
      وإلا
      {
	unsigned cluster = _infos_find_min_cluster (info, start, end);
	_infos_set_glyph_flags (info, start, end, cluster, mask);
      }
    }
    وإلا
    {
      أكد (start <= out_len);
      أكد (idx <= end);

      𐡄𐡍 (!interior)
      {
	لكل (unsigned i = start; i < out_len; i++)
	  out_info[i].mask |= mask;
	لكل (unsigned i = idx; i < end; i++)
	  info[i].mask |= mask;
      }
      وإلا
      {
	unsigned cluster = _infos_find_min_cluster (info, idx, end);
	cluster = _infos_find_min_cluster (out_info, start, out_len, cluster);

	_infos_set_glyph_flags (out_info, start, out_len, cluster, mask);
	_infos_set_glyph_flags (info, idx, end, cluster, mask);
      }
    }
  }

  HB_ALWAYS_INLINE
  فراغ _set_glyph_flags (hb_mask_t mask,
			 unsigned start = 0,
			 unsigned end = (unsigned) -1,
			 منطقي interior = خطأ,
			 منطقي from_out_buffer = خطأ)
  {
    𐡄𐡍 (unlikely (end != (unsigned) -1 && end - start > 255))
      أرجع;

    end = hb_min (end, len);

    𐡄𐡍 (interior && !from_out_buffer && end - start < 2)
      أرجع;

    _set_glyph_flags_impl (mask, start, end, interior, from_out_buffer);
  }


  فراغ unsafe_to_break (unsigned صحيح start = 0, unsigned صحيح end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      𐡒𐡔𐡈);
  }
  فراغ safe_to_insert_tatweel (unsigned صحيح start = 0, unsigned صحيح end = -1)
  {
    𐡄𐡍 ((flags & HB_BUFFER_FLAG_PRODUCE_SAFE_TO_INSERT_TATWEEL) == 0)
    {
      unsafe_to_break (start, end);
      أرجع;
    }
    _set_glyph_flags (HB_GLYPH_FLAG_SAFE_TO_INSERT_TATWEEL,
		      start, end,
		      𐡒𐡔𐡈);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  فراغ unsafe_to_concat (unsigned صحيح start = 0, unsigned صحيح end = -1)
  {
    𐡄𐡍 (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      أرجع;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      خطأ);
  }
  فراغ unsafe_to_break_from_outbuffer (unsigned صحيح start = 0, unsigned صحيح end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      𐡒𐡔𐡈, 𐡒𐡔𐡈);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  فراغ unsafe_to_concat_from_outbuffer (unsigned صحيح start = 0, unsigned صحيح end = -1)
  {
    𐡄𐡍 (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      أرجع;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      خطأ, 𐡒𐡔𐡈);
  }


  /* Internal methods */
  HB_NODISCARD HB_INTERNAL منطقي move_to (unsigned صحيح i); /* i is output-buffer index. */

  HB_NODISCARD HB_INTERNAL منطقي enlarge (unsigned صحيح size);

  HB_NODISCARD منطقي resize (unsigned length)
  {
    أكد (!have_output);
    𐡄𐡍 (unlikely (!ensure (length))) أرجع خطأ;
    len = length;
    أرجع 𐡒𐡔𐡈;
  }
  HB_NODISCARD منطقي ensure (unsigned صحيح size)
  { أرجع likely (!size || size < allocated) ? 𐡒𐡔𐡈 : enlarge (size); }

  HB_NODISCARD منطقي ensure_inplace (unsigned صحيح size)
  { أرجع likely (!size || size < allocated); }

  فراغ assert_glyphs ()
  {
    أكد ((content_type == HB_BUFFER_CONTENT_TYPE_GLYPHS) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  فراغ assert_unicode ()
  {
    أكد ((content_type == HB_BUFFER_CONTENT_TYPE_UNICODE) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  HB_NODISCARD منطقي ensure_glyphs ()
  {
    𐡄𐡍 (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_GLYPHS))
    {
      𐡄𐡍 (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	أرجع خطأ;
      أكد (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_GLYPHS;
    }
    أرجع 𐡒𐡔𐡈;
  }
  HB_NODISCARD منطقي ensure_unicode ()
  {
    𐡄𐡍 (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_UNICODE))
    {
      𐡄𐡍 (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	أرجع خطأ;
      أكد (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_UNICODE;
    }
    أرجع 𐡒𐡔𐡈;
  }

  HB_NODISCARD HB_INTERNAL منطقي make_room_for (unsigned صحيح num_in, unsigned صحيح num_out);
  HB_NODISCARD HB_INTERNAL منطقي shift_forward (unsigned صحيح count);

  تعريفنوع long scratch_buffer_t;
  HB_INTERNAL scratch_buffer_t *get_scratch_buffer (unsigned صحيح *size);

  فراغ clear_context (unsigned صحيح side) { context_len[side] = 0; }

  HB_INTERNAL فراغ sort (unsigned صحيح start, unsigned صحيح end, صحيح(*compar)(ثابت hb_glyph_info_t *, ثابت hb_glyph_info_t *));

  منطقي messaging ()
  {
#ifdef HB_NO_BUFFER_MESSAGE
    أرجع خطأ;
#وإلا
    أرجع unlikely (message_func);
#endif
  }
  منطقي message (hb_font_t *font, ثابت حرف *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#ifdef HB_NO_BUFFER_MESSAGE
    أرجع 𐡒𐡔𐡈;
#وإلا
    𐡄𐡍 (likely (!messaging ()))
      أرجع 𐡒𐡔𐡈;

    va_list ap;
    va_start (ap, fmt);
    منطقي ret = message_impl (font, fmt, ap);
    va_end (ap);

    أرجع ret;
#endif
  }
  HB_INTERNAL منطقي message_impl (hb_font_t *font, ثابت حرف *fmt, va_list ap) HB_PRINTF_FUNC(3, 0);
  HB_INTERNAL فراغ changed ();

  ساكن فراغ
  set_cluster (hb_glyph_info_t &inf, unsigned صحيح cluster, unsigned صحيح mask = 0)
  {
    𐡄𐡍 (inf.cluster != cluster)
      inf.mask = (inf.mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  فراغ
  _infos_set_glyph_flags (hb_glyph_info_t *infos,
			  unsigned صحيح start, unsigned صحيح end,
			  unsigned صحيح cluster,
			  hb_mask_t mask)
  {
    𐡄𐡍 (unlikely (start == end))
      أرجع;

    max_ops -= end - start;
    𐡄𐡍 (unlikely (max_ops < 0))
      successful = خطأ;

    unsigned cluster_first = infos[start].cluster;
    unsigned cluster_last = infos[end - 1].cluster;

    𐡄𐡍 (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_first && cluster != cluster_last))
    {
      لكل (unsigned صحيح i = start; i < end; i++)
	𐡄𐡍 (cluster != infos[i].cluster)
	  infos[i].mask |= mask;
      أرجع;
    }

    /* Monotone clusters */

    𐡄𐡍 (cluster == cluster_first)
    {
      لكل (unsigned صحيح i = end; start < i && infos[i - 1].cluster != cluster_first; i--)
	infos[i - 1].mask |= mask;
    }
    وإلا /* cluster == cluster_last */
    {
      لكل (unsigned صحيح i = start; i < end && infos[i].cluster != cluster_last; i++)
	infos[i].mask |= mask;
    }
  }
  unsigned
  _infos_find_min_cluster (ثابت hb_glyph_info_t *infos,
			   unsigned start, unsigned end,
			   unsigned cluster = UINT_MAX)
  {
    𐡄𐡍 (unlikely (start == end))
      أرجع cluster;

    𐡄𐡍 (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS)
    {
      لكل (unsigned صحيح i = start; i < end; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      أرجع cluster;
    }

    أرجع hb_min (cluster, hb_min (infos[start].cluster, infos[end - 1].cluster));
  }

  فراغ clear_glyph_flags (hb_mask_t mask = 0)
  {
    لكل (unsigned صحيح i = 0; i < len; i++)
      info[i].mask = (info[i].mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_buffer_t);


#define foreach_group(buffer, start, end, group_func) \
  لكل (unsigned صحيح \
       _count = buffer->len, \
       start = 0, end = _count ? buffer->group_end (0, group_func) : 0; \
       start < _count; \
       start = end, end = buffer->group_end (start, group_func))

#define foreach_cluster(buffer, start, end) \
	foreach_group (buffer, start, end, hb_buffer_t::_cluster_group_func)


#define HB_BUFFER_XALLOCATE_VAR(b, func, var) \
  b->func (offsetof (hb_glyph_info_t, var) - offsetof(hb_glyph_info_t, var1), \
	   حجم (b->info[0].var))
#define HB_BUFFER_ALLOCATE_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, allocate_var,     var ())
#define HB_BUFFER_TRY_ALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, try_allocate_var, var ())
#define HB_BUFFER_DEALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, deallocate_var,   var ())
#define HB_BUFFER_ASSERT_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, assert_var,       var ())


#endif /* HB_BUFFER_HH */
