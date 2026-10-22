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

#جيڪڏهن_وصف_نه_ٿيل HB_بفر_HH
#وصف_ڪريو HB_بفر_HH

#ضمن "hb.hh"
#ضمن "hb-unicode.hh"
#ضمن "hb-set-digest.hh"


static_assert ((حجم (hb_glyph_info_t) == 20), "");
static_assert ((حجم (hb_glyph_info_t) == حجم (hb_glyph_position_t)), "");

HB_MARK_AS_FLAG_T (hb_glyph_flags_t);
HB_MARK_AS_FLAG_T (hb_بفر_flags_t);
HB_MARK_AS_FLAG_T (hb_بفر_serialize_flags_t);
HB_MARK_AS_FLAG_T (hb_بفر_diff_flags_t);

تعداد hb_بفر_scratch_flags_t {
  HB_بفر_SCRATCH_FLAG_DEFAULT			= 0x00000000u,
  HB_بفر_SCRATCH_FLAG_HAS_FRACTION_SLASH		= 0x00000001u,
  HB_بفر_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES		= 0x00000002u,
  HB_بفر_SCRATCH_FLAG_HAS_SPACE_FALLBACK		= 0x00000004u,
  HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT		= 0x00000008u,
  HB_بفر_SCRATCH_FLAG_HAS_CGJ			= 0x00000010u,
  HB_بفر_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE		= 0x00000020u,
  HB_بفر_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK= 0x00000040u,
  HB_بفر_SCRATCH_FLAG_HAS_CONTINUATIONS		= 0x00000080u,

  /* Reserved for shapers' internal use. */
  HB_بفر_SCRATCH_FLAG_SHAPER0			= 0x01000000u,
  HB_بفر_SCRATCH_FLAG_SHAPER1			= 0x02000000u,
  HB_بفر_SCRATCH_FLAG_SHAPER2			= 0x04000000u,
  HB_بفر_SCRATCH_FLAG_SHAPER3			= 0x08000000u,
};
HB_MARK_AS_FLAG_T (hb_بفر_scratch_flags_t);


/*
 * hb_buffer_t
 */

بنية hb_بفر_t
{
  hb_object_سرو_t سرو;

  /*
   * Information about how the text in the buffer should be treated.
   */

  hb_unicode_funcs_t *unicode; /* Unicode functions */
  hb_بفر_flags_t flags; /* BOT / EOT / etc. */
  hb_بفر_cluster_level_t cluster_level;
  hb_codepoint_t replacement; /* U+FFFD or something else. */
  hb_codepoint_t invisible; /* 0 or something else. */
  hb_codepoint_t not_مليو; /* 0 or something else. */
  hb_codepoint_t not_مليو_variation_selector; /* HB_CODEPOINT_INVALID or something else. */

  /*
   * Buffer contents
   */

  hb_بفر_content_type_t content_type;
  hb_segment_properties_t props; /* Script, language, direction */

  منطقي successful; /* Allocations successful */
  منطقي have_output; /* Whether we have an output buffer going on */
  منطقي have_positions; /* Whether we have positions */

  بنانشان صحيح idx; /* Cursor into ->info and ->pos arrays */
  بنانشان صحيح len; /* Length of ->info and ->pos arrays */
  بنانشان صحيح ٻاهريون_len; /* Length of ->out_info array if have_output */

  بنانشان صحيح allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *ٻاهريون_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  ساكن constexpr بنانشان CONTEXT_ڊيگهائي = 5u;
  hb_codepoint_t context[2][CONTEXT_ڊيگهائي];
  بنانشان صحيح context_len[2];

  hb_سيٽ_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_بٽ;
  uint8_t serial;
  uint32_t random_state;
  hb_بفر_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  بنانشان صحيح max_len; /* Maximum allowed len. */
  صحيح max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#جيڪڏهن_وصف_نه_ٿيل HB_NO_بفر_پيغام
  تعريفنوع فراغ (*بدليو_func_t) (hb_بفر_t *بفر, فراغ *user_معلومات);

  hb_بفر_پيغام_func_t پيغام_func;
  فراغ *پيغام_معلومات;
  hb_destroy_func_t پيغام_destroy;
  بدليو_func_t بدليو_func;
  فراغ *بدليو_معلومات;
  بنانشان پيغام_اونهائي; /* How deeply are we inside a message callback? */
#وإلا
  ساكن constexpr بنانشان پيغام_اونهائي = 0u;
#جيڪڏهن_پورو



  /* Methods */

  HB_NODISCARD منطقي اندريون_error () ثابت { أرجع !successful; }

  فراغ allocate_var (بنانشان صحيح شروعات, بنانشان صحيح ڳڻپ)
  {
    بنانشان صحيح پڇاڙي = شروعات + ڳڻپ;
    أكد (پڇاڙي <= 8);
    بنانشان صحيح بٽ = (1u<<پڇاڙي) - (1u<<شروعات);
    أكد (0 == (allocated_var_بٽ & بٽ));
    allocated_var_بٽ |= بٽ;
  }
  منطقي try_allocate_var (بنانشان صحيح شروعات, بنانشان صحيح ڳڻپ)
  {
    بنانشان صحيح پڇاڙي = شروعات + ڳڻپ;
    أكد (پڇاڙي <= 8);
    بنانشان صحيح بٽ = (1u<<پڇاڙي) - (1u<<شروعات);
    إذا (allocated_var_بٽ & بٽ)
      أرجع خطأ;
    allocated_var_بٽ |= بٽ;
    أرجع سچ;
  }
  فراغ deallocate_var (بنانشان صحيح شروعات, بنانشان صحيح ڳڻپ)
  {
    بنانشان صحيح پڇاڙي = شروعات + ڳڻپ;
    أكد (پڇاڙي <= 8);
    بنانشان صحيح بٽ = (1u<<پڇاڙي) - (1u<<شروعات);
    أكد (بٽ == (allocated_var_بٽ & بٽ));
    allocated_var_بٽ &= ~بٽ;
  }
  فراغ assert_var (بنانشان صحيح شروعات, بنانشان صحيح ڳڻپ)
  {
    بنانشان صحيح پڇاڙي = شروعات + ڳڻپ;
    أكد (پڇاڙي <= 8);
    HB_اڻ_استعمال_ٿيل بنانشان صحيح بٽ = (1u<<پڇاڙي) - (1u<<شروعات);
    أكد (بٽ == (allocated_var_بٽ & بٽ));
  }
  فراغ deallocate_var_all ()
  {
    allocated_var_بٽ = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (بنانشان صحيح i = 0) { أرجع info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (بنانشان صحيح i = 0) ثابت { أرجع info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (بنانشان صحيح i = 0) { أرجع pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (بنانشان صحيح i = 0) ثابت { أرجع pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { أرجع ٻاهريون_info[ٻاهريون_len ? ٻاهريون_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () ثابت { أرجع ٻاهريون_info[ٻاهريون_len ? ٻاهريون_len - 1 : 0]; }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_codepoints (سيٽ_t &d) ثابت
  { d.clear (); d.جوڙيو_array (&info[0].codepoint, len, حجم (info[0])); }

  فراغ update_digest ()
  {
    digest = hb_سيٽ_digest_t ();
    collect_codepoints (digest);
  }

  HB_اندروني فراغ similar (ثابت hb_بفر_t &src);
  HB_اندروني فراغ reset ();
  HB_اندروني فراغ clear ();

  /* Called around shape() */
  HB_اندروني فراغ enter ();
  HB_اندروني فراغ leave ();

#جيڪڏهن_وصف_نه_ٿيل HB_NO_بفر_VERIFY
  HB_اندروني
#جيڪڏهن_پورو
  منطقي verify (hb_بفر_t        *متن_بفر,
	       hb_font_t          *font,
	       ثابت hb_feature_t *features,
	       بنانشان صحيح        انگ_features,
	       ثابت حرف * ثابت *shapers)
#جيڪڏهن_وصف_نه_ٿيل HB_NO_بفر_VERIFY
  ;
#وإلا
  { أرجع سچ; }
#جيڪڏهن_پورو

  بنانشان صحيح backtrack_len () ثابت { أرجع have_output ? ٻاهريون_len : idx; }
  بنانشان صحيح lookahead_len () ثابت { أرجع len - idx; }
  uint8_t ايندڙ_serial () { أرجع ++serial ? serial : ++serial; }

  HB_اندروني فراغ جوڙيو (hb_codepoint_t  codepoint,
			بنانشان صحيح    cluster);
  HB_اندروني فراغ جوڙيو_info (ثابت hb_glyph_info_t &glyph_info);
  HB_اندروني فراغ جوڙيو_info_and_pos (ثابت hb_glyph_info_t &glyph_info,
				     ثابت hb_glyph_position_t &glyph_pos);

  فراغ ابتو_range (بنانشان شروعات, بنانشان پڇاڙي)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (شروعات, پڇاڙي);
    إذا (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (شروعات, پڇاڙي);
  }
  فراغ reverse () { ابتو_range (0, len); }

  سانچو <قسم_نالو FuncType>
  فراغ ابتو_groups (ثابت FuncType& گروهه,
		       منطقي ملايو_clusters = خطأ)
  {
    إذا (unlikely (!len))
      أرجع;

    بنانشان شروعات = 0;
    بنانشان i;
    لكل (i = 1; i < len; i++)
    {
      إذا (!گروهه (info[i - 1], info[i]))
      {
	إذا (ملايو_clusters)
	  هذا->ملايو_clusters (شروعات, i);
	ابتو_range (شروعات, i);
	شروعات = i;
      }
    }
    إذا (ملايو_clusters)
      هذا->ملايو_clusters (شروعات, i);
    ابتو_range (شروعات, i);

    reverse ();
  }

  سانچو <قسم_نالو FuncType>
  بنانشان گروهه_پڇاڙي (بنانشان شروعات, ثابت FuncType& گروهه) ثابت
  {
    طالما (++شروعات < len && گروهه (info[شروعات - 1], info[شروعات]))
      ;

    أرجع شروعات;
  }

  ساكن منطقي cluster_گروهه_func (ثابت hb_glyph_info_t& a,
				   ثابت hb_glyph_info_t& b)
  { أرجع a.cluster == b.cluster; }

  فراغ ابتو_clusters () { ابتو_groups (cluster_گروهه_func); }

  HB_اندروني فراغ guess_segment_properties ();

  HB_اندروني منطقي متزامن ();
  HB_اندروني صحيح sync_so_far ();
  HB_اندروني فراغ clear_output ();
  HB_اندروني فراغ clear_positions ();

  سانچو <قسم_نالو T>
  HB_NODISCARD منطقي replace_glyphs (بنانشان صحيح انگ_اندريون,
				    بنانشان صحيح انگ_ٻاهريون,
				    ثابت T *glyph_معلومات)
  {
    إذا (unlikely (!make_گنجائش_for (انگ_اندريون, انگ_ٻاهريون))) أرجع خطأ;

    أكد (idx + انگ_اندريون <= len);

    ملايو_clusters (idx, idx + انگ_اندريون);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &ٻاهريون_info[ٻاهريون_len];
    لكل (بنانشان صحيح i = 0; i < انگ_ٻاهريون; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_معلومات[i];
      pinfo++;
    }

    idx  += انگ_اندريون;
    ٻاهريون_len += انگ_ٻاهريون;
    أرجع سچ;
  }

  HB_NODISCARD منطقي replace_glyph (hb_codepoint_t glyph_اشاريو)
  { أرجع replace_glyphs (1, 1, &glyph_اشاريو); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD منطقي output_glyph (hb_codepoint_t glyph_اشاريو)
  { أرجع replace_glyphs (0, 1, &glyph_اشاريو); }

  HB_NODISCARD منطقي output_info (ثابت hb_glyph_info_t &glyph_info)
  {
    إذا (unlikely (!make_گنجائش_for (0, 1))) أرجع خطأ;

    ٻاهريون_info[ٻاهريون_len] = glyph_info;

    ٻاهريون_len++;
    أرجع سچ;
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
  HB_NODISCARD منطقي ايندڙ_glyph ()
  {
    إذا (have_output)
    {
      إذا (ٻاهريون_info != info || ٻاهريون_len != idx)
      {
	إذا (unlikely (!ensure (ٻاهريون_len + 1))) أرجع خطأ;
	ٻاهريون_info[ٻاهريون_len] = info[idx];
      }
      ٻاهريون_len++;
    }

    idx++;
    أرجع سچ;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD منطقي ايندڙ_glyphs (بنانشان صحيح n)
  {
    إذا (have_output)
    {
      إذا (ٻاهريون_info != info || ٻاهريون_len != idx)
      {
	إذا (unlikely (!ensure (ٻاهريون_len + n))) أرجع خطأ;
	memmove (ٻاهريون_info + ٻاهريون_len, info + idx, n * حجم (ٻاهريون_info[0]));
      }
      ٻاهريون_len += n;
    }

    idx += n;
    أرجع سچ;
  }
  /* Advance idx without copying to output. */
  فراغ skip_glyph () { idx++; }
  فراغ جوڙيو_masks (hb_نقاب_t نقاب)
  {
    لكل (بنانشان صحيح j = 0; j < len; j++)
      info[j].نقاب |= نقاب;
  }
  HB_اندروني فراغ سيٽ_masks (hb_نقاب_t value, hb_نقاب_t نقاب,
			      بنانشان صحيح cluster_شروعات, بنانشان صحيح cluster_پڇاڙي);

  فراغ ملايو_clusters (بنانشان صحيح شروعات, بنانشان صحيح پڇاڙي)
  {
    إذا (پڇاڙي - شروعات < 2)
      أرجع;
    إذا (!HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_منزل_break (شروعات, پڇاڙي);
      أرجع;
    }
    ملايو_clusters_impl (شروعات, پڇاڙي);
  }
  فراغ ملايو_grapheme_clusters (بنانشان صحيح شروعات, بنانشان صحيح پڇاڙي)
  {
    إذا (پڇاڙي - شروعات < 2)
      أرجع;
    إذا (!HB_بفر_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_منزل_break (شروعات, پڇاڙي);
      أرجع;
    }
    ملايو_clusters_impl (شروعات, پڇاڙي);
  }
  HB_اندروني فراغ ملايو_clusters_impl (بنانشان صحيح شروعات, بنانشان صحيح پڇاڙي);
  فراغ ملايو_ٻاهريون_clusters (بنانشان صحيح شروعات, بنانشان صحيح پڇاڙي)
  {
    إذا (پڇاڙي - شروعات < 2)
      أرجع;
    إذا (!HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      أرجع;
    ملايو_ٻاهريون_clusters_impl (شروعات, پڇاڙي);
  }
  فراغ ملايو_ٻاهريون_grapheme_clusters (بنانشان صحيح شروعات, بنانشان صحيح پڇاڙي)
  {
    إذا (پڇاڙي - شروعات < 2)
      أرجع;
    إذا (!HB_بفر_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      أرجع;
    ملايو_ٻاهريون_clusters_impl (شروعات, پڇاڙي);
  }
  HB_اندروني فراغ ملايو_ٻاهريون_clusters_impl (بنانشان صحيح شروعات, بنانشان صحيح پڇاڙي);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_اندروني فراغ delete_glyph ();
  HB_اندروني فراغ delete_glyphs_inplace (منطقي (*filter) (ثابت hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  فراغ سيٽ_glyph_flags_impl (hb_نقاب_t نقاب,
			      بنانشان شروعات,
			      بنانشان پڇاڙي,
			      منطقي interior,
			      منطقي from_ٻاهريون_بفر)
  {
    إذا (!from_ٻاهريون_بفر || !have_output)
    {
      إذا (!interior)
      {
	لكل (بنانشان i = شروعات; i < پڇاڙي; i++)
	  info[i].نقاب |= نقاب;
      }
      وإلا
      {
	بنانشان cluster = infos_ڳوليو_min_cluster (info, شروعات, پڇاڙي);
	infos_سيٽ_glyph_flags (info, شروعات, پڇاڙي, cluster, نقاب);
      }
    }
    وإلا
    {
      أكد (شروعات <= ٻاهريون_len);
      أكد (idx <= پڇاڙي);

      إذا (!interior)
      {
	لكل (بنانشان i = شروعات; i < ٻاهريون_len; i++)
	  ٻاهريون_info[i].نقاب |= نقاب;
	لكل (بنانشان i = idx; i < پڇاڙي; i++)
	  info[i].نقاب |= نقاب;
      }
      وإلا
      {
	بنانشان cluster = infos_ڳوليو_min_cluster (info, idx, پڇاڙي);
	cluster = infos_ڳوليو_min_cluster (ٻاهريون_info, شروعات, ٻاهريون_len, cluster);

	infos_سيٽ_glyph_flags (ٻاهريون_info, شروعات, ٻاهريون_len, cluster, نقاب);
	infos_سيٽ_glyph_flags (info, idx, پڇاڙي, cluster, نقاب);
      }
    }
  }

  HB_ALWAYS_INLINE
  فراغ سيٽ_glyph_flags (hb_نقاب_t نقاب,
			 بنانشان شروعات = 0,
			 بنانشان پڇاڙي = (بنانشان) -1,
			 منطقي interior = خطأ,
			 منطقي from_ٻاهريون_بفر = خطأ)
  {
    إذا (unlikely (پڇاڙي != (بنانشان) -1 && پڇاڙي - شروعات > 255))
      أرجع;

    پڇاڙي = hb_min (پڇاڙي, len);

    إذا (interior && !from_ٻاهريون_بفر && پڇاڙي - شروعات < 2)
      أرجع;

    سيٽ_glyph_flags_impl (نقاب, شروعات, پڇاڙي, interior, from_ٻاهريون_بفر);
  }


  فراغ unsafe_منزل_break (بنانشان صحيح شروعات = 0, بنانشان صحيح پڇاڙي = -1)
  {
    سيٽ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_منزل_BREAK | HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT,
		      شروعات, پڇاڙي,
		      سچ);
  }
  فراغ محفوظ_منزل_وجهو_tatweel (بنانشان صحيح شروعات = 0, بنانشان صحيح پڇاڙي = -1)
  {
    إذا ((flags & HB_بفر_FLAG_PRODUCE_محفوظ_منزل_وجهو_TATWEEL) == 0)
    {
      unsafe_منزل_break (شروعات, پڇاڙي);
      أرجع;
    }
    سيٽ_glyph_flags (HB_GLYPH_FLAG_محفوظ_منزل_وجهو_TATWEEL,
		      شروعات, پڇاڙي,
		      سچ);
  }
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
  فراغ unsafe_منزل_concat (بنانشان صحيح شروعات = 0, بنانشان صحيح پڇاڙي = -1)
  {
    إذا (likely ((flags & HB_بفر_FLAG_PRODUCE_UNSAFE_منزل_CONCAT) == 0))
      أرجع;
    سيٽ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT,
		      شروعات, پڇاڙي,
		      خطأ);
  }
  فراغ unsafe_منزل_break_from_outbuffer (بنانشان صحيح شروعات = 0, بنانشان صحيح پڇاڙي = -1)
  {
    سيٽ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_منزل_BREAK | HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT,
		      شروعات, پڇاڙي,
		      سچ, سچ);
  }
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
  فراغ unsafe_منزل_concat_from_outbuffer (بنانشان صحيح شروعات = 0, بنانشان صحيح پڇاڙي = -1)
  {
    إذا (likely ((flags & HB_بفر_FLAG_PRODUCE_UNSAFE_منزل_CONCAT) == 0))
      أرجع;
    سيٽ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT,
		      شروعات, پڇاڙي,
		      خطأ, سچ);
  }


  /* Internal methods */
  HB_NODISCARD HB_اندروني منطقي move_منزل (بنانشان صحيح i); /* i is output-buffer index. */

  HB_NODISCARD HB_اندروني منطقي enlarge (بنانشان صحيح سائيز);

  HB_NODISCARD منطقي resize (بنانشان ڊيگهائي)
  {
    أكد (!have_output);
    إذا (unlikely (!ensure (ڊيگهائي))) أرجع خطأ;
    len = ڊيگهائي;
    أرجع سچ;
  }
  HB_NODISCARD منطقي ensure (بنانشان صحيح سائيز)
  { أرجع likely (!سائيز || سائيز < allocated) ? سچ : enlarge (سائيز); }

  HB_NODISCARD منطقي ensure_inplace (بنانشان صحيح سائيز)
  { أرجع likely (!سائيز || سائيز < allocated); }

  فراغ assert_glyphs ()
  {
    أكد ((content_type == HB_بفر_CONTENT_TYPE_GLYPHS) ||
	    (!len && (content_type == HB_بفر_CONTENT_TYPE_INVALID)));
  }
  فراغ assert_unicode ()
  {
    أكد ((content_type == HB_بفر_CONTENT_TYPE_UNICODE) ||
	    (!len && (content_type == HB_بفر_CONTENT_TYPE_INVALID)));
  }
  HB_NODISCARD منطقي ensure_glyphs ()
  {
    إذا (unlikely (content_type != HB_بفر_CONTENT_TYPE_GLYPHS))
    {
      إذا (content_type != HB_بفر_CONTENT_TYPE_INVALID)
	أرجع خطأ;
      أكد (len == 0);
      content_type = HB_بفر_CONTENT_TYPE_GLYPHS;
    }
    أرجع سچ;
  }
  HB_NODISCARD منطقي ensure_unicode ()
  {
    إذا (unlikely (content_type != HB_بفر_CONTENT_TYPE_UNICODE))
    {
      إذا (content_type != HB_بفر_CONTENT_TYPE_INVALID)
	أرجع خطأ;
      أكد (len == 0);
      content_type = HB_بفر_CONTENT_TYPE_UNICODE;
    }
    أرجع سچ;
  }

  HB_NODISCARD HB_اندروني منطقي make_گنجائش_for (بنانشان صحيح انگ_اندريون, بنانشان صحيح انگ_ٻاهريون);
  HB_NODISCARD HB_اندروني منطقي سرڪايو_forward (بنانشان صحيح ڳڻپ);

  تعريفنوع ڊگهو scratch_بفر_t;
  HB_اندروني scratch_بفر_t *get_scratch_بفر (بنانشان صحيح *سائيز);

  فراغ clear_context (بنانشان صحيح side) { context_len[side] = 0; }

  HB_اندروني فراغ sort (بنانشان صحيح شروعات, بنانشان صحيح پڇاڙي, صحيح(*compar)(ثابت hb_glyph_info_t *, ثابت hb_glyph_info_t *));

  منطقي messaging ()
  {
#جيڪڏهن_وصف_ٿيل HB_NO_بفر_پيغام
    أرجع خطأ;
#وإلا
    أرجع unlikely (پيغام_func);
#جيڪڏهن_پورو
  }
  منطقي پيغام (hb_font_t *font, ثابت حرف *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#جيڪڏهن_وصف_ٿيل HB_NO_بفر_پيغام
    أرجع سچ;
#وإلا
    إذا (likely (!messaging ()))
      أرجع سچ;

    va_list ap;
    va_شروعات (ap, fmt);
    منطقي ret = پيغام_impl (font, fmt, ap);
    va_پڇاڙي (ap);

    أرجع ret;
#جيڪڏهن_پورو
  }
  HB_اندروني منطقي پيغام_impl (hb_font_t *font, ثابت حرف *fmt, va_list ap) HB_PRINTF_FUNC(3, 0);
  HB_اندروني فراغ بدليو ();

  ساكن فراغ
  سيٽ_cluster (hb_glyph_info_t &inf, بنانشان صحيح cluster, بنانشان صحيح نقاب = 0)
  {
    إذا (inf.cluster != cluster)
      inf.نقاب = (inf.نقاب & ~HB_GLYPH_FLAG_DEFINED) | (نقاب & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  فراغ
  infos_سيٽ_glyph_flags (hb_glyph_info_t *infos,
			  بنانشان صحيح شروعات, بنانشان صحيح پڇاڙي,
			  بنانشان صحيح cluster,
			  hb_نقاب_t نقاب)
  {
    إذا (unlikely (شروعات == پڇاڙي))
      أرجع;

    max_ops -= پڇاڙي - شروعات;
    إذا (unlikely (max_ops < 0))
      successful = خطأ;

    بنانشان cluster_پهريون = infos[شروعات].cluster;
    بنانشان cluster_آخري = infos[پڇاڙي - 1].cluster;

    إذا (cluster_level == HB_بفر_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_پهريون && cluster != cluster_آخري))
    {
      لكل (بنانشان صحيح i = شروعات; i < پڇاڙي; i++)
	إذا (cluster != infos[i].cluster)
	  infos[i].نقاب |= نقاب;
      أرجع;
    }

    /* Monotone clusters */

    إذا (cluster == cluster_پهريون)
    {
      لكل (بنانشان صحيح i = پڇاڙي; شروعات < i && infos[i - 1].cluster != cluster_پهريون; i--)
	infos[i - 1].نقاب |= نقاب;
    }
    وإلا /* cluster == cluster_last */
    {
      لكل (بنانشان صحيح i = شروعات; i < پڇاڙي && infos[i].cluster != cluster_آخري; i++)
	infos[i].نقاب |= نقاب;
    }
  }
  بنانشان
  infos_ڳوليو_min_cluster (ثابت hb_glyph_info_t *infos,
			   بنانشان شروعات, بنانشان پڇاڙي,
			   بنانشان cluster = UINT_MAX)
  {
    إذا (unlikely (شروعات == پڇاڙي))
      أرجع cluster;

    إذا (cluster_level == HB_بفر_CLUSTER_LEVEL_CHARACTERS)
    {
      لكل (بنانشان صحيح i = شروعات; i < پڇاڙي; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      أرجع cluster;
    }

    أرجع hb_min (cluster, hb_min (infos[شروعات].cluster, infos[پڇاڙي - 1].cluster));
  }

  فراغ clear_glyph_flags (hb_نقاب_t نقاب = 0)
  {
    لكل (بنانشان صحيح i = 0; i < len; i++)
      info[i].نقاب = (info[i].نقاب & ~HB_GLYPH_FLAG_DEFINED) | (نقاب & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_بفر_t);


#وصف_ڪريو foreach_گروهه(بفر, شروعات, پڇاڙي, گروهه_func) \
  لكل (بنانشان صحيح \
       ڳڻپ_2 = بفر->len, \
       شروعات = 0, پڇاڙي = ڳڻپ_2 ? بفر->گروهه_پڇاڙي (0, گروهه_func) : 0; \
       شروعات < ڳڻپ_2; \
       شروعات = پڇاڙي, پڇاڙي = بفر->گروهه_پڇاڙي (شروعات, گروهه_func))

#وصف_ڪريو foreach_cluster(بفر, شروعات, پڇاڙي) \
	foreach_گروهه (بفر, شروعات, پڇاڙي, hb_بفر_t::cluster_گروهه_func)


#وصف_ڪريو HB_بفر_XALLOCATE_VAR(b, func, var) \
  b->func (offsetof (hb_glyph_info_t, var) - offsetof(hb_glyph_info_t, var1), \
	   حجم (b->info[0].var))
#وصف_ڪريو HB_بفر_ALLOCATE_VAR(b, var)		HB_بفر_XALLOCATE_VAR (b, allocate_var,     var ())
#وصف_ڪريو HB_بفر_TRY_ALLOCATE_VAR(b, var)	HB_بفر_XALLOCATE_VAR (b, try_allocate_var, var ())
#وصف_ڪريو HB_بفر_DEALLOCATE_VAR(b, var)	HB_بفر_XALLOCATE_VAR (b, deallocate_var,   var ())
#وصف_ڪريو HB_بفر_ASSERT_VAR(b, var)		HB_بفر_XALLOCATE_VAR (b, assert_var,       var ())


#جيڪڏهن_پورو /* HB_BUFFER_HH */
