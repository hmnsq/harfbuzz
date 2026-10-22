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

#include "hb.hh"
#include "hb-unicode.hh"
#include "hb-set-digest.hh"


static_assert ((sizeof (hb_glyph_info_t) == 20), "");
static_assert ((sizeof (hb_glyph_info_t) == sizeof (hb_glyph_position_t)), "");

HB_MARK_AS_FLAG_T (hb_glyph_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_serialize_flags_t);
HB_MARK_AS_FLAG_T (hb_buffer_diff_flags_t);

enum hb_buffer_scratch_flags_t {
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

struct hb_buffer_t
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

  bool successful; /* Allocations successful */
  bool have_output; /* Whether we have an output buffer going on */
  bool have_positions; /* Whether we have positions */

  unsigned 整数 idx; /* Cursor into ->info and ->pos arrays */
  unsigned 整数 len; /* Length of ->info and ->pos arrays */
  unsigned 整数 out_len; /* Length of ->out_info array if have_output */

  unsigned 整数 allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *out_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  static constexpr unsigned CONTEXT_LENGTH = 5u;
  hb_codepoint_t context[2][CONTEXT_LENGTH];
  unsigned 整数 context_len[2];

  hb_set_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_bits;
  uint8_t serial;
  uint32_t random_state;
  hb_buffer_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  unsigned 整数 max_len; /* Maximum allowed len. */
  整数 max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#ifndef HB_NO_BUFFER_MESSAGE
  typedef void (*changed_func_t) (hb_buffer_t *buffer, void *user_data);

  hb_buffer_message_func_t message_func;
  void *message_data;
  hb_destroy_func_t message_destroy;
  changed_func_t changed_func;
  void *changed_data;
  unsigned message_depth; /* How deeply are we inside a message callback? */
#否则
  static constexpr unsigned message_depth = 0u;
#endif



  /* Methods */

  HB_NODISCARD bool in_error () const { 戻る !successful; }

  void allocate_var (unsigned 整数 start, unsigned 整数 count)
  {
    unsigned 整数 end = start + count;
    assert (end <= 8);
    unsigned 整数 bits = (1u<<end) - (1u<<start);
    assert (0 == (allocated_var_bits & bits));
    allocated_var_bits |= bits;
  }
  bool try_allocate_var (unsigned 整数 start, unsigned 整数 count)
  {
    unsigned 整数 end = start + count;
    assert (end <= 8);
    unsigned 整数 bits = (1u<<end) - (1u<<start);
    もし (allocated_var_bits & bits)
      戻る false;
    allocated_var_bits |= bits;
    戻る true;
  }
  void deallocate_var (unsigned 整数 start, unsigned 整数 count)
  {
    unsigned 整数 end = start + count;
    assert (end <= 8);
    unsigned 整数 bits = (1u<<end) - (1u<<start);
    assert (bits == (allocated_var_bits & bits));
    allocated_var_bits &= ~bits;
  }
  void assert_var (unsigned 整数 start, unsigned 整数 count)
  {
    unsigned 整数 end = start + count;
    assert (end <= 8);
    HB_UNUSED unsigned 整数 bits = (1u<<end) - (1u<<start);
    assert (bits == (allocated_var_bits & bits));
  }
  void deallocate_var_all ()
  {
    allocated_var_bits = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (unsigned 整数 i = 0) { 戻る info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (unsigned 整数 i = 0) const { 戻る info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (unsigned 整数 i = 0) { 戻る pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (unsigned 整数 i = 0) const { 戻る pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { 戻る out_info[out_len ? out_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () const { 戻る out_info[out_len ? out_len - 1 : 0]; }

  template <typename set_t>
  void collect_codepoints (set_t &d) const
  { d.clear (); d.add_array (&info[0].codepoint, len, sizeof (info[0])); }

  void update_digest ()
  {
    digest = hb_set_digest_t ();
    collect_codepoints (digest);
  }

  HB_INTERNAL void similar (const hb_buffer_t &src);
  HB_INTERNAL void reset ();
  HB_INTERNAL void clear ();

  /* Called around shape() */
  HB_INTERNAL void enter ();
  HB_INTERNAL void leave ();

#ifndef HB_NO_BUFFER_VERIFY
  HB_INTERNAL
#endif
  bool verify (hb_buffer_t        *text_buffer,
	       hb_font_t          *font,
	       const hb_feature_t *features,
	       unsigned 整数        num_features,
	       const char * const *shapers)
#ifndef HB_NO_BUFFER_VERIFY
  ;
#否则
  { 戻る true; }
#endif

  unsigned 整数 backtrack_len () const { 戻る have_output ? out_len : idx; }
  unsigned 整数 lookahead_len () const { 戻る len - idx; }
  uint8_t next_serial () { 戻る ++serial ? serial : ++serial; }

  HB_INTERNAL void add (hb_codepoint_t  codepoint,
			unsigned 整数    cluster);
  HB_INTERNAL void add_info (const hb_glyph_info_t &glyph_info);
  HB_INTERNAL void add_info_and_pos (const hb_glyph_info_t &glyph_info,
				     const hb_glyph_position_t &glyph_pos);

  void reverse_range (unsigned start, unsigned end)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (start, end);
    もし (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (start, end);
  }
  void reverse () { reverse_range (0, len); }

  template <typename FuncType>
  void reverse_groups (const FuncType& group,
		       bool merge_clusters = false)
  {
    もし (unlikely (!len))
      戻る;

    unsigned start = 0;
    unsigned i;
    ために (i = 1; i < len; i++)
    {
      もし (!group (info[i - 1], info[i]))
      {
	もし (merge_clusters)
	  this->merge_clusters (start, i);
	reverse_range (start, i);
	start = i;
      }
    }
    もし (merge_clusters)
      this->merge_clusters (start, i);
    reverse_range (start, i);

    reverse ();
  }

  template <typename FuncType>
  unsigned group_end (unsigned start, const FuncType& group) const
  {
    ながら (++start < len && group (info[start - 1], info[start]))
      ;

    戻る start;
  }

  static bool _cluster_group_func (const hb_glyph_info_t& a,
				   const hb_glyph_info_t& b)
  { 戻る a.cluster == b.cluster; }

  void reverse_clusters () { reverse_groups (_cluster_group_func); }

  HB_INTERNAL void guess_segment_properties ();

  HB_INTERNAL bool sync ();
  HB_INTERNAL 整数 sync_so_far ();
  HB_INTERNAL void clear_output ();
  HB_INTERNAL void clear_positions ();

  template <typename T>
  HB_NODISCARD bool replace_glyphs (unsigned 整数 num_in,
				    unsigned 整数 num_out,
				    const T *glyph_data)
  {
    もし (unlikely (!make_room_for (num_in, num_out))) 戻る false;

    assert (idx + num_in <= len);

    merge_clusters (idx, idx + num_in);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &out_info[out_len];
    ために (unsigned 整数 i = 0; i < num_out; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_data[i];
      pinfo++;
    }

    idx  += num_in;
    out_len += num_out;
    戻る true;
  }

  HB_NODISCARD bool replace_glyph (hb_codepoint_t glyph_index)
  { 戻る replace_glyphs (1, 1, &glyph_index); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD bool output_glyph (hb_codepoint_t glyph_index)
  { 戻る replace_glyphs (0, 1, &glyph_index); }

  HB_NODISCARD bool output_info (const hb_glyph_info_t &glyph_info)
  {
    もし (unlikely (!make_room_for (0, 1))) 戻る false;

    out_info[out_len] = glyph_info;

    out_len++;
    戻る true;
  }
  /* Copies glyph at idx to output but doesn't advance idx */
  HB_NODISCARD bool copy_glyph ()
  {
    /* Extra copy because cur()'s return can be freed within
     * output_info() call if buffer reallocates. */
    戻る output_info (hb_glyph_info_t (cur()));
  }

  /* Copies glyph at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD bool next_glyph ()
  {
    もし (have_output)
    {
      もし (out_info != info || out_len != idx)
      {
	もし (unlikely (!ensure (out_len + 1))) 戻る false;
	out_info[out_len] = info[idx];
      }
      out_len++;
    }

    idx++;
    戻る true;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD bool next_glyphs (unsigned 整数 n)
  {
    もし (have_output)
    {
      もし (out_info != info || out_len != idx)
      {
	もし (unlikely (!ensure (out_len + n))) 戻る false;
	memmove (out_info + out_len, info + idx, n * sizeof (out_info[0]));
      }
      out_len += n;
    }

    idx += n;
    戻る true;
  }
  /* Advance idx without copying to output. */
  void skip_glyph () { idx++; }
  void add_masks (hb_mask_t mask)
  {
    ために (unsigned 整数 j = 0; j < len; j++)
      info[j].mask |= mask;
  }
  HB_INTERNAL void set_masks (hb_mask_t value, hb_mask_t mask,
			      unsigned 整数 cluster_start, unsigned 整数 cluster_end);

  void merge_clusters (unsigned 整数 start, unsigned 整数 end)
  {
    もし (end - start < 2)
      戻る;
    もし (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_to_break (start, end);
      戻る;
    }
    merge_clusters_impl (start, end);
  }
  void merge_grapheme_clusters (unsigned 整数 start, unsigned 整数 end)
  {
    もし (end - start < 2)
      戻る;
    もし (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_to_break (start, end);
      戻る;
    }
    merge_clusters_impl (start, end);
  }
  HB_INTERNAL void merge_clusters_impl (unsigned 整数 start, unsigned 整数 end);
  void merge_out_clusters (unsigned 整数 start, unsigned 整数 end)
  {
    もし (end - start < 2)
      戻る;
    もし (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      戻る;
    merge_out_clusters_impl (start, end);
  }
  void merge_out_grapheme_clusters (unsigned 整数 start, unsigned 整数 end)
  {
    もし (end - start < 2)
      戻る;
    もし (!HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      戻る;
    merge_out_clusters_impl (start, end);
  }
  HB_INTERNAL void merge_out_clusters_impl (unsigned 整数 start, unsigned 整数 end);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_INTERNAL void delete_glyph ();
  HB_INTERNAL void delete_glyphs_inplace (bool (*filter) (const hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  void _set_glyph_flags_impl (hb_mask_t mask,
			      unsigned start,
			      unsigned end,
			      bool interior,
			      bool from_out_buffer)
  {
    もし (!from_out_buffer || !have_output)
    {
      もし (!interior)
      {
	ために (unsigned i = start; i < end; i++)
	  info[i].mask |= mask;
      }
      否则
      {
	unsigned cluster = _infos_find_min_cluster (info, start, end);
	_infos_set_glyph_flags (info, start, end, cluster, mask);
      }
    }
    否则
    {
      assert (start <= out_len);
      assert (idx <= end);

      もし (!interior)
      {
	ために (unsigned i = start; i < out_len; i++)
	  out_info[i].mask |= mask;
	ために (unsigned i = idx; i < end; i++)
	  info[i].mask |= mask;
      }
      否则
      {
	unsigned cluster = _infos_find_min_cluster (info, idx, end);
	cluster = _infos_find_min_cluster (out_info, start, out_len, cluster);

	_infos_set_glyph_flags (out_info, start, out_len, cluster, mask);
	_infos_set_glyph_flags (info, idx, end, cluster, mask);
      }
    }
  }

  HB_ALWAYS_INLINE
  void _set_glyph_flags (hb_mask_t mask,
			 unsigned start = 0,
			 unsigned end = (unsigned) -1,
			 bool interior = false,
			 bool from_out_buffer = false)
  {
    もし (unlikely (end != (unsigned) -1 && end - start > 255))
      戻る;

    end = hb_min (end, len);

    もし (interior && !from_out_buffer && end - start < 2)
      戻る;

    _set_glyph_flags_impl (mask, start, end, interior, from_out_buffer);
  }


  void unsafe_to_break (unsigned 整数 start = 0, unsigned 整数 end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      true);
  }
  void safe_to_insert_tatweel (unsigned 整数 start = 0, unsigned 整数 end = -1)
  {
    もし ((flags & HB_BUFFER_FLAG_PRODUCE_SAFE_TO_INSERT_TATWEEL) == 0)
    {
      unsafe_to_break (start, end);
      戻る;
    }
    _set_glyph_flags (HB_GLYPH_FLAG_SAFE_TO_INSERT_TATWEEL,
		      start, end,
		      true);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  void unsafe_to_concat (unsigned 整数 start = 0, unsigned 整数 end = -1)
  {
    もし (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      戻る;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      false);
  }
  void unsafe_to_break_from_outbuffer (unsigned 整数 start = 0, unsigned 整数 end = -1)
  {
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      true, true);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  void unsafe_to_concat_from_outbuffer (unsigned 整数 start = 0, unsigned 整数 end = -1)
  {
    もし (likely ((flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0))
      戻る;
    _set_glyph_flags (HB_GLYPH_FLAG_UNSAFE_TO_CONCAT,
		      start, end,
		      false, true);
  }


  /* Internal methods */
  HB_NODISCARD HB_INTERNAL bool move_to (unsigned 整数 i); /* i is output-buffer index. */

  HB_NODISCARD HB_INTERNAL bool enlarge (unsigned 整数 size);

  HB_NODISCARD bool resize (unsigned length)
  {
    assert (!have_output);
    もし (unlikely (!ensure (length))) 戻る false;
    len = length;
    戻る true;
  }
  HB_NODISCARD bool ensure (unsigned 整数 size)
  { 戻る likely (!size || size < allocated) ? true : enlarge (size); }

  HB_NODISCARD bool ensure_inplace (unsigned 整数 size)
  { 戻る likely (!size || size < allocated); }

  void assert_glyphs ()
  {
    assert ((content_type == HB_BUFFER_CONTENT_TYPE_GLYPHS) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  void assert_unicode ()
  {
    assert ((content_type == HB_BUFFER_CONTENT_TYPE_UNICODE) ||
	    (!len && (content_type == HB_BUFFER_CONTENT_TYPE_INVALID)));
  }
  HB_NODISCARD bool ensure_glyphs ()
  {
    もし (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_GLYPHS))
    {
      もし (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	戻る false;
      assert (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_GLYPHS;
    }
    戻る true;
  }
  HB_NODISCARD bool ensure_unicode ()
  {
    もし (unlikely (content_type != HB_BUFFER_CONTENT_TYPE_UNICODE))
    {
      もし (content_type != HB_BUFFER_CONTENT_TYPE_INVALID)
	戻る false;
      assert (len == 0);
      content_type = HB_BUFFER_CONTENT_TYPE_UNICODE;
    }
    戻る true;
  }

  HB_NODISCARD HB_INTERNAL bool make_room_for (unsigned 整数 num_in, unsigned 整数 num_out);
  HB_NODISCARD HB_INTERNAL bool shift_forward (unsigned 整数 count);

  typedef long scratch_buffer_t;
  HB_INTERNAL scratch_buffer_t *get_scratch_buffer (unsigned 整数 *size);

  void clear_context (unsigned 整数 side) { context_len[side] = 0; }

  HB_INTERNAL void sort (unsigned 整数 start, unsigned 整数 end, 整数(*compar)(const hb_glyph_info_t *, const hb_glyph_info_t *));

  bool messaging ()
  {
#ifdef HB_NO_BUFFER_MESSAGE
    戻る false;
#否则
    戻る unlikely (message_func);
#endif
  }
  bool message (hb_font_t *font, const char *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#ifdef HB_NO_BUFFER_MESSAGE
    戻る true;
#否则
    もし (likely (!messaging ()))
      戻る true;

    va_list ap;
    va_start (ap, fmt);
    bool ret = message_impl (font, fmt, ap);
    va_end (ap);

    戻る ret;
#endif
  }
  HB_INTERNAL bool message_impl (hb_font_t *font, const char *fmt, va_list ap) HB_PRINTF_FUNC(3, 0);
  HB_INTERNAL void changed ();

  static void
  set_cluster (hb_glyph_info_t &inf, unsigned 整数 cluster, unsigned 整数 mask = 0)
  {
    もし (inf.cluster != cluster)
      inf.mask = (inf.mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  void
  _infos_set_glyph_flags (hb_glyph_info_t *infos,
			  unsigned 整数 start, unsigned 整数 end,
			  unsigned 整数 cluster,
			  hb_mask_t mask)
  {
    もし (unlikely (start == end))
      戻る;

    max_ops -= end - start;
    もし (unlikely (max_ops < 0))
      successful = false;

    unsigned cluster_first = infos[start].cluster;
    unsigned cluster_last = infos[end - 1].cluster;

    もし (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_first && cluster != cluster_last))
    {
      ために (unsigned 整数 i = start; i < end; i++)
	もし (cluster != infos[i].cluster)
	  infos[i].mask |= mask;
      戻る;
    }

    /* Monotone clusters */

    もし (cluster == cluster_first)
    {
      ために (unsigned 整数 i = end; start < i && infos[i - 1].cluster != cluster_first; i--)
	infos[i - 1].mask |= mask;
    }
    否则 /* cluster == cluster_last */
    {
      ために (unsigned 整数 i = start; i < end && infos[i].cluster != cluster_last; i++)
	infos[i].mask |= mask;
    }
  }
  unsigned
  _infos_find_min_cluster (const hb_glyph_info_t *infos,
			   unsigned start, unsigned end,
			   unsigned cluster = UINT_MAX)
  {
    もし (unlikely (start == end))
      戻る cluster;

    もし (cluster_level == HB_BUFFER_CLUSTER_LEVEL_CHARACTERS)
    {
      ために (unsigned 整数 i = start; i < end; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      戻る cluster;
    }

    戻る hb_min (cluster, hb_min (infos[start].cluster, infos[end - 1].cluster));
  }

  void clear_glyph_flags (hb_mask_t mask = 0)
  {
    ために (unsigned 整数 i = 0; i < len; i++)
      info[i].mask = (info[i].mask & ~HB_GLYPH_FLAG_DEFINED) | (mask & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_buffer_t);


#define foreach_group(buffer, start, end, group_func) \
  ために (unsigned 整数 \
       _count = buffer->len, \
       start = 0, end = _count ? buffer->group_end (0, group_func) : 0; \
       start < _count; \
       start = end, end = buffer->group_end (start, group_func))

#define foreach_cluster(buffer, start, end) \
	foreach_group (buffer, start, end, hb_buffer_t::_cluster_group_func)


#define HB_BUFFER_XALLOCATE_VAR(b, func, var) \
  b->func (offsetof (hb_glyph_info_t, var) - offsetof(hb_glyph_info_t, var1), \
	   sizeof (b->info[0].var))
#define HB_BUFFER_ALLOCATE_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, allocate_var,     var ())
#define HB_BUFFER_TRY_ALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, try_allocate_var, var ())
#define HB_BUFFER_DEALLOCATE_VAR(b, var)	HB_BUFFER_XALLOCATE_VAR (b, deallocate_var,   var ())
#define HB_BUFFER_ASSERT_VAR(b, var)		HB_BUFFER_XALLOCATE_VAR (b, assert_var,       var ())


#endif /* HB_BUFFER_HH */
