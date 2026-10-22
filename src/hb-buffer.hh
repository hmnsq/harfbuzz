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

  HB_NODISCARD منطقي in_error () ثابت { إرجاع !successful; }

  فراغ allocate_var (unsigned صحيح start, unsigned صحيح تعداد_2)
  {
    unsigned صحيح end = start + تعداد_2;
    أكد (end <= 8);
    unsigned صحيح bits = (1u<<end) - (1u<<start);
    أكد (0 == (allocated_var_bits & bits));
    allocated_var_bits |= bits;
  }
  منطقي try_allocate_var (unsigned صحيح start, unsigned صحيح تعداد_2)
  {
    unsigned صحيح end = start + تعداد_2;
    أكد (end <= 8);
    unsigned صحيح bits = (1u<<end) - (1u<<start);
    إذا (allocated_var_bits & bits)
      إرجاع خطأ;
    allocated_var_bits |= bits;
    إرجاع true;
  }
  فراغ deallocate_var (unsigned صحيح start, unsigned صحيح تعداد_2)
  {
    unsigned صحيح end = start + تعداد_2;
    أكد (end <= 8);
    unsigned صحيح bits = (1u<<end) - (1u<<start);
    أكد (bits == (allocated_var_bits & bits));
    allocated_var_bits &= ~bits;
  }
  فراغ assert_var (unsigned صحيح start, unsigned صحيح تعداد_2)
  {
    unsigned صحيح end = start + تعداد_2;
    أكد (end <= 8);
    HB_UNUSED unsigned صحيح bits = (1u<<end) - (1u<<start);
    أكد (bits == (allocated_var_bits & bits));
  }
  فراغ deallocate_var_all ()
  {
    allocated_var_bits = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (unsigned صحيح i = 0) { إرجاع info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (unsigned صحيح i = 0) ثابت { إرجاع info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (unsigned صحيح i = 0) { إرجاع pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (unsigned صحيح i = 0) ثابت { إرجاع pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { إرجاع out_info[out_len ? out_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () ثابت { إرجاع out_info[out_len ? out_len - 1 : 0]; }

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
	       unsigned صحيح        رقم_features,
	       ثابت حرف * ثابت *shapers)
#ifndef HB_NO_BUFFER_VERIFY
  ;
#وإلا
  { إرجاع true; }
#endif

  unsigned صحيح backtrack_len () ثابت { إرجاع have_output ? out_len : idx; }
  unsigned صحيح lookahead_len () ثابت { إرجاع len - idx; }
  uint8_t next_serial () { إرجاع ++serial ? serial : ++serial; }

  HB_INTERNAL فراغ add (hb_codepoint_t  codepoint,
			unsigned صحيح    cluster);
  HB_INTERNAL فراغ add_info (ثابت hb_glyph_info_t &glyph_info);
  HB_INTERNAL فراغ add_info_and_pos (ثابت hb_glyph_info_t &glyph_info,
				     ثابت hb_glyph_position_t &glyph_pos);

  فراغ reverse_range (unsigned start, unsigned end)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (start, end);
    إذا (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (start, end);
  }
  فراغ reverse () { reverse_range (0, len); }

  template <typename FuncType>
  فراغ reverse_groups (ثابت FuncType& group,
		       منطقي merge_clusters = خطأ)
  {
    إذا (unlikely (!len))
      إرجاع;

    unsigned start = 0;
    unsigned i;
    لكل (i = 1; i < len; i++)
    {
      إذا (!group (info[i - 1], info[i]))
      {
	إذا (merge_clusters)
	  هذا->merge_clusters (start, i);
	reverse_range (start, i);
	start = i;
      }
    }
    إذا (merge_clusters)
      هذا->merge_clusters (start, i);
    reverse_range (start, i);

    reverse ();
  }

  template <typename FuncType>
  unsigned group_end (unsigned start, ثابت FuncType& group) ثابت
  {
    بينما (++start < len && group (info[start - 1], info[start]))
      ;

    إرجاع start;
  }

  ساكن منطقي _cluster_group_func (ثابت hb_glyph_info_t& a,
				   ثابت hb_glyph_info_t& b)
  { إرجاع a.cluster == b.cluster; }

  فراغ reverse_clusters () { reverse_groups (_cluster_group_func); }

  HB_INTERNAL فراغ guess_segment_properties ();

  HB_INTERNAL منطقي متزامن ();
  HB_INTERNAL صحيح sync_so_far ();
  HB_INTERNAL فراغ clear_output ();
  HB_INTERNAL فراغ clear_positions ();

  template <typename T>
  HB_NODISCARD منطقي replace_glyphs (unsigned صحيح رقم_in,
				    unsigned صحيح رقم_out,
				    ثابت T *glyph_data)
  {
    إذا (unlikely (!make_room_for (رقم_in, رقم_out))) إرجاع خطأ;

    أكد (idx + رقم_in <= len);

    merge_clusters (idx, idx + رقم_in);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &out_info[out_len];
    لكل (unsigned صحيح i = 0; i < رقم_out; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_data[i];
      pinfo++;
    }

    idx  += رقم_in;
    out_len += رقم_out;
    إرجاع true;
  }

  HB_NODISCARD منطقي replace_glyph (hb_codepoint_t glyph_index)
  { إرجاع replace_glyphs (1, 1, &glyph_index); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD منطقي output_glyph (hb_codepoint_t glyph_index)
  { إرجاع replace_glyphs (0, 1, &glyph_index); }

  HB_NODISCARD منطقي output_info (ثابت hb_glyph_info_t &glyph_info)
  {
    إذا (unlikely (!make_room_for (0, 1))) إرجاع خطأ;

    out_info[out_len] = glyph_info;

    out_len++;
    إرجاع true;
  }
  /* Copies glyph at idx to output but doesn't advance idx */
  HB_NODISCARD منطقي copy_glyph ()
  {
    /* Extra copy because cur()'s return can be freed within
     * output_info() call if buffer reallocates. */
    إرجاع output_info (hb_glyph_info_t (cur()));
  }

  /* Copies glyph at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD منطقي next_glyph ()
  {
    إذا (have_output)
    {
      إذا (out_info != info || out_len != idx)
      {
	إذا (unlikely (!ensure (out_len + 1))) إرجاع خطأ;
	out_info[out_len] = info[idx];
      }
      out_len++;
    }

    idx++;
    إرجاع true;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD منطقي next_glyphs (unsigned صحيح n)
  {
    إذا (have_output)
    {
      إذا (out_info != info || out_len != idx)
      {
	إذا (unlikely (!ensure (out_len + n))) إرجاع خطأ;
	memmove (out_info + out_len, info + idx, n * حجم (out_info[0]));
      }
      out_len += n;
    }

    idx += n;
    إرجاع true;
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
    إذا (end - start < 2)
      إرجاع;
    إذا (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_to_break (start, end);
      إرجاع;
    }
    merge_clusters_impl (start, end);
  }
  فراغ merge_grapheme_clusters (unsigned صحيح start, unsigned صحيح end)
  {
    إذا (end - start < 2)
      إرجاع;
    إذا (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_to_break (start, end);
      إرجاع;
    }
    merge_clusters_impl (start, end);
  }
  HB_INTERNAL فراغ merge_clusters_impl (unsigned صحيح start, unsigned صحيح end);
  فراغ merge_out_clusters (unsigned صحيح start, unsigned صحيح end)
  {
    إذا (end - start < 2)
      إرجاع;
    إذا (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      إرجاع;
    merge_out_clusters_impl (start, end);
  }
  فراغ merge_out_grapheme_clusters (unsigned صحيح start, unsigned صحيح end)
  {
    إذا (end - start < 2)
      إرجاع;
    إذا (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      إرجاع;
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
    إذا (!from_out_buffer || !have_output)
    {
      إذا (!interior)
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

      إذا (!interior)
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
    إذا (unlikely (end != (unsigned) -1 && end - start > 255))
      إرجاع;

    end = hb_min (end, len);

    إذا (interior && !from_out_buffer && end - start < 2)
      إرجاع;

    _set_glyph_flags_impl (mask, start, end, interior, from_out_buffer);
  }


  فراغ unsafe_to_break (unsigned صحيح start = 0, unsigned صحيح end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      true);
  }
  فراغ safe_to_أدرج_tatweel (unsigned صحيح start = 0, unsigned صحيح end = -1)
  {
    إذا ((flags & HB_BUFFER_FLAG_PRODUCE_SAFE_TO_أدرج_TATWEEL) == 0)
    {
      unsafe_to_break (start, end);
      إرجاع;
    }
    _set_glyph_flags (HB_GLYPH_FLAG_SAFE_TO_أدرج_TATWEEL,
		      start, end,
		      true);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  فراغ unsafe_to_concat (unsigned صحيح start = 0, unsigned صحيح end = -1)
  {
    إذا (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      إرجاع;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      خطأ);
  }
  فراغ unsafe_to_break_from_outbuffer (unsigned صحيح start = 0, unsigned صحيح end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      true, true);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  فراغ unsafe_to_concat_from_outbuffer (unsigned صحيح start = 0, unsigned صحيح end = -1)
  {
    إذا (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      إرجاع;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      خطأ, true);
  }


  /* Internal methods */
  HB_NODISCARD HB_INTERNAL منطقي move_to (unsigned صحيح i); /* i is output-buffer index. */

  HB_NODISCARD HB_INTERNAL منطقي enlarge (unsigned صحيح size);

  HB_NODISCARD منطقي resize (unsigned length)
  {
    أكد (!have_output);
    إذا (unlikely (!ensure (length))) إرجاع خطأ;
    len = length;
    إرجاع true;
  }
  HB_NODISCARD منطقي ensure (unsigned صحيح size)
  { إرجاع likely (!size || size < allocated) ? true : enlarge (size); }

  HB_NODISCARD منطقي ensure_inplace (unsigned صحيح size)
  { إرجاع likely (!size || size < allocated); }

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
    إذا (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_GLYPHS))
    {
      إذا (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	إرجاع خطأ;
      أكد (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_GLYPHS;
    }
    إرجاع true;
  }
  HB_NODISCARD منطقي ensure_unicode ()
  {
    إذا (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_UNICODE))
    {
      إذا (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	إرجاع خطأ;
      أكد (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_UNICODE;
    }
    إرجاع true;
  }

  HB_NODISCARD HB_INTERNAL منطقي make_room_for (unsigned صحيح رقم_in, unsigned صحيح رقم_out);
  HB_NODISCARD HB_INTERNAL منطقي shift_forward (unsigned صحيح تعداد_2);

  تعريفنوع long scratch_buffer_t;
  HB_INTERNAL scratch_buffer_t *get_scratch_buffer (unsigned صحيح *size);

  فراغ clear_context (unsigned صحيح side) { context_len[side] = 0; }

  HB_INTERNAL فراغ sort (unsigned صحيح start, unsigned صحيح end, صحيح(*compar)(ثابت hb_glyph_info_t *, ثابت hb_glyph_info_t *));

  منطقي messaging ()
  {
#ifdef HB_NO_BUFFER_MESSAGE
    إرجاع خطأ;
#وإلا
    إرجاع unlikely (message_func);
#endif
  }
  منطقي message (hb_font_t *font, ثابت حرف *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#ifdef HB_NO_BUFFER_MESSAGE
    إرجاع true;
#وإلا
    إذا (likely (!messaging ()))
      إرجاع true;

    va_list ap;
    va_start (ap, fmt);
    منطقي ret = message_impl (font, fmt, ap);
    va_end (ap);

    إرجاع ret;
#endif
  }
  HB_INTERNAL منطقي message_impl (hb_font_t *font, ثابت حرف *fmt, va_list ap) HB_PRINTF_FUNC(3, 0);
  HB_INTERNAL فراغ changed ();

  ساكن فراغ
  set_cluster (hb_glyph_info_t &inf, unsigned صحيح cluster, unsigned صحيح mask = 0)
  {
    إذا (inf.cluster != cluster)
      inf.mask = (inf.mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  فراغ
  _infos_set_glyph_flags (hb_glyph_info_t *infos,
			  unsigned صحيح start, unsigned صحيح end,
			  unsigned صحيح cluster,
			  hb_mask_t mask)
  {
    إذا (unlikely (start == end))
      إرجاع;

    max_ops -= end - start;
    إذا (unlikely (max_ops < 0))
      successful = خطأ;

    unsigned cluster_first = infos[start].cluster;
    unsigned cluster_last = infos[end - 1].cluster;

    إذا (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_first && cluster != cluster_last))
    {
      لكل (unsigned صحيح i = start; i < end; i++)
	إذا (cluster != infos[i].cluster)
	  infos[i].mask |= mask;
      إرجاع;
    }

    /* Monotone clusters */

    إذا (cluster == cluster_first)
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
    إذا (unlikely (start == end))
      إرجاع cluster;

    إذا (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS)
    {
      لكل (unsigned صحيح i = start; i < end; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      إرجاع cluster;
    }

    إرجاع hb_min (cluster, hb_min (infos[start].cluster, infos[end - 1].cluster));
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
       تعداد_3 = buffer->len, \
       start = 0, end = تعداد_3 ? buffer->group_end (0, group_func) : 0; \
       start < تعداد_3; \
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
