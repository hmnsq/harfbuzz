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

#اگر_تعریف_نشده HB_میانگیر_HH
#تعریف_کن HB_میانگیر_HH

#ضمن "hb.hh"
#ضمن "hb-unicode.hh"
#ضمن "hb-set-digest.hh"


static_assert ((حجم (hb_glyph_info_t) == 20), "");
static_assert ((حجم (hb_glyph_info_t) == حجم (hb_glyph_position_t)), "");

HB_MARK_AS_FLAG_T (hb_glyph_flags_t);
HB_MARK_AS_FLAG_T (hb_میانگیر_flags_t);
HB_MARK_AS_FLAG_T (hb_میانگیر_serialize_flags_t);
HB_MARK_AS_FLAG_T (hb_میانگیر_diff_flags_t);

تعداد hb_میانگیر_scratch_flags_t {
  HB_میانگیر_SCRATCH_FLAG_DEFAULT			= 0x00000000u,
  HB_میانگیر_SCRATCH_FLAG_HAS_FRACTION_SLASH		= 0x00000001u,
  HB_میانگیر_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES		= 0x00000002u,
  HB_میانگیر_SCRATCH_FLAG_HAS_SPACE_FALLBACK		= 0x00000004u,
  HB_میانگیر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT		= 0x00000008u,
  HB_میانگیر_SCRATCH_FLAG_HAS_CGJ			= 0x00000010u,
  HB_میانگیر_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE		= 0x00000020u,
  HB_میانگیر_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK= 0x00000040u,
  HB_میانگیر_SCRATCH_FLAG_HAS_CONTINUATIONS		= 0x00000080u,

  /* Reserved for shapers' internal use. */
  HB_میانگیر_SCRATCH_FLAG_SHAPER0			= 0x01000000u,
  HB_میانگیر_SCRATCH_FLAG_SHAPER1			= 0x02000000u,
  HB_میانگیر_SCRATCH_FLAG_SHAPER2			= 0x04000000u,
  HB_میانگیر_SCRATCH_FLAG_SHAPER3			= 0x08000000u,
};
HB_MARK_AS_FLAG_T (hb_میانگیر_scratch_flags_t);


/*
 * hb_buffer_t
 */

بنية hb_میانگیر_t
{
  hb_object_سرآیند_t سرآیند;

  /*
   * Information about how the text in the buffer should be treated.
   */

  hb_unicode_funcs_t *unicode; /* Unicode functions */
  hb_میانگیر_flags_t flags; /* BOT / EOT / etc. */
  hb_میانگیر_cluster_level_t cluster_level;
  hb_codepoint_t replacement; /* U+FFFD or something else. */
  hb_codepoint_t invisible; /* 0 or something else. */
  hb_codepoint_t not_یافت; /* 0 or something else. */
  hb_codepoint_t not_یافت_variation_selector; /* HB_CODEPOINT_INVALID or something else. */

  /*
   * Buffer contents
   */

  hb_میانگیر_content_type_t content_type;
  hb_segment_properties_t props; /* Script, language, direction */

  منطقي successful; /* Allocations successful */
  منطقي have_output; /* Whether we have an output buffer going on */
  منطقي have_positions; /* Whether we have positions */

  بیعلامت صحيح idx; /* Cursor into ->info and ->pos arrays */
  بیعلامت صحيح len; /* Length of ->info and ->pos arrays */
  بیعلامت صحيح بیرون_len; /* Length of ->out_info array if have_output */

  بیعلامت صحيح allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *بیرون_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  ساكن constexpr بیعلامت CONTEXT_درازی = 5u;
  hb_codepoint_t context[2][CONTEXT_درازی];
  بیعلامت صحيح context_len[2];

  hb_مجموعه_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_بیتها;
  uint8_t serial;
  uint32_t random_state;
  hb_میانگیر_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  بیعلامت صحيح max_len; /* Maximum allowed len. */
  صحيح max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#اگر_تعریف_نشده HB_NO_میانگیر_پیام
  تعريفنوع فراغ (*تغییریافته_func_t) (hb_میانگیر_t *میانگیر, فراغ *user_داده);

  hb_میانگیر_پیام_func_t پیام_func;
  فراغ *پیام_داده;
  hb_destroy_func_t پیام_destroy;
  تغییریافته_func_t تغییریافته_func;
  فراغ *تغییریافته_داده;
  بیعلامت پیام_عمق; /* How deeply are we inside a message callback? */
#وإلا
  ساكن constexpr بیعلامت پیام_عمق = 0u;
#پایان_اگر



  /* Methods */

  HB_NODISCARD منطقي درون_error () ثابت { أرجع !successful; }

  فراغ allocate_var (بیعلامت صحيح شروع, بیعلامت صحيح شمار)
  {
    بیعلامت صحيح انتها = شروع + شمار;
    أكد (انتها <= 8);
    بیعلامت صحيح بیتها = (1u<<انتها) - (1u<<شروع);
    أكد (0 == (allocated_var_بیتها & بیتها));
    allocated_var_بیتها |= بیتها;
  }
  منطقي try_allocate_var (بیعلامت صحيح شروع, بیعلامت صحيح شمار)
  {
    بیعلامت صحيح انتها = شروع + شمار;
    أكد (انتها <= 8);
    بیعلامت صحيح بیتها = (1u<<انتها) - (1u<<شروع);
    إذا (allocated_var_بیتها & بیتها)
      أرجع خطأ;
    allocated_var_بیتها |= بیتها;
    أرجع درست;
  }
  فراغ deallocate_var (بیعلامت صحيح شروع, بیعلامت صحيح شمار)
  {
    بیعلامت صحيح انتها = شروع + شمار;
    أكد (انتها <= 8);
    بیعلامت صحيح بیتها = (1u<<انتها) - (1u<<شروع);
    أكد (بیتها == (allocated_var_بیتها & بیتها));
    allocated_var_بیتها &= ~بیتها;
  }
  فراغ assert_var (بیعلامت صحيح شروع, بیعلامت صحيح شمار)
  {
    بیعلامت صحيح انتها = شروع + شمار;
    أكد (انتها <= 8);
    HB_استفادهنشده بیعلامت صحيح بیتها = (1u<<انتها) - (1u<<شروع);
    أكد (بیتها == (allocated_var_بیتها & بیتها));
  }
  فراغ deallocate_var_all ()
  {
    allocated_var_بیتها = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (بیعلامت صحيح i = 0) { أرجع info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (بیعلامت صحيح i = 0) ثابت { أرجع info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (بیعلامت صحيح i = 0) { أرجع pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (بیعلامت صحيح i = 0) ثابت { أرجع pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { أرجع بیرون_info[بیرون_len ? بیرون_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () ثابت { أرجع بیرون_info[بیرون_len ? بیرون_len - 1 : 0]; }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_codepoints (مجموعه_t &d) ثابت
  { d.clear (); d.جمع_کن_array (&info[0].codepoint, len, حجم (info[0])); }

  فراغ update_digest ()
  {
    digest = hb_مجموعه_digest_t ();
    collect_codepoints (digest);
  }

  HB_داخلی فراغ similar (ثابت hb_میانگیر_t &src);
  HB_داخلی فراغ reset ();
  HB_داخلی فراغ clear ();

  /* Called around shape() */
  HB_داخلی فراغ enter ();
  HB_داخلی فراغ leave ();

#اگر_تعریف_نشده HB_NO_میانگیر_VERIFY
  HB_داخلی
#پایان_اگر
  منطقي verify (hb_میانگیر_t        *متن_میانگیر,
	       hb_font_t          *font,
	       ثابت hb_feature_t *features,
	       بیعلامت صحيح        رقم_features,
	       ثابت حرف * ثابت *shapers)
#اگر_تعریف_نشده HB_NO_میانگیر_VERIFY
  ;
#وإلا
  { أرجع درست; }
#پایان_اگر

  بیعلامت صحيح backtrack_len () ثابت { أرجع have_output ? بیرون_len : idx; }
  بیعلامت صحيح lookahead_len () ثابت { أرجع len - idx; }
  uint8_t بعدی_serial () { أرجع ++serial ? serial : ++serial; }

  HB_داخلی فراغ جمع_کن (hb_codepoint_t  codepoint,
			بیعلامت صحيح    cluster);
  HB_داخلی فراغ جمع_کن_info (ثابت hb_glyph_info_t &glyph_info);
  HB_داخلی فراغ جمع_کن_info_and_pos (ثابت hb_glyph_info_t &glyph_info,
				     ثابت hb_glyph_position_t &glyph_pos);

  فراغ معکوس_range (بیعلامت شروع, بیعلامت انتها)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (شروع, انتها);
    إذا (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (شروع, انتها);
  }
  فراغ reverse () { معکوس_range (0, len); }

  قالب <نام_نوع FuncType>
  فراغ معکوس_groups (ثابت FuncType& گروه,
		       منطقي ادغام_clusters = خطأ)
  {
    إذا (unlikely (!len))
      أرجع;

    بیعلامت شروع = 0;
    بیعلامت i;
    لكل (i = 1; i < len; i++)
    {
      إذا (!گروه (info[i - 1], info[i]))
      {
	إذا (ادغام_clusters)
	  هذا->ادغام_clusters (شروع, i);
	معکوس_range (شروع, i);
	شروع = i;
      }
    }
    إذا (ادغام_clusters)
      هذا->ادغام_clusters (شروع, i);
    معکوس_range (شروع, i);

    reverse ();
  }

  قالب <نام_نوع FuncType>
  بیعلامت گروه_انتها (بیعلامت شروع, ثابت FuncType& گروه) ثابت
  {
    طالما (++شروع < len && گروه (info[شروع - 1], info[شروع]))
      ;

    أرجع شروع;
  }

  ساكن منطقي cluster_گروه_func (ثابت hb_glyph_info_t& a,
				   ثابت hb_glyph_info_t& b)
  { أرجع a.cluster == b.cluster; }

  فراغ معکوس_clusters () { معکوس_groups (cluster_گروه_func); }

  HB_داخلی فراغ guess_segment_properties ();

  HB_داخلی منطقي متزامن ();
  HB_داخلی صحيح sync_so_far ();
  HB_داخلی فراغ clear_output ();
  HB_داخلی فراغ clear_positions ();

  قالب <نام_نوع T>
  HB_NODISCARD منطقي replace_glyphs (بیعلامت صحيح رقم_درون,
				    بیعلامت صحيح رقم_بیرون,
				    ثابت T *glyph_داده)
  {
    إذا (unlikely (!make_جا_for (رقم_درون, رقم_بیرون))) أرجع خطأ;

    أكد (idx + رقم_درون <= len);

    ادغام_clusters (idx, idx + رقم_درون);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &بیرون_info[بیرون_len];
    لكل (بیعلامت صحيح i = 0; i < رقم_بیرون; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_داده[i];
      pinfo++;
    }

    idx  += رقم_درون;
    بیرون_len += رقم_بیرون;
    أرجع درست;
  }

  HB_NODISCARD منطقي replace_glyph (hb_codepoint_t glyph_شاخص)
  { أرجع replace_glyphs (1, 1, &glyph_شاخص); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD منطقي output_glyph (hb_codepoint_t glyph_شاخص)
  { أرجع replace_glyphs (0, 1, &glyph_شاخص); }

  HB_NODISCARD منطقي output_info (ثابت hb_glyph_info_t &glyph_info)
  {
    إذا (unlikely (!make_جا_for (0, 1))) أرجع خطأ;

    بیرون_info[بیرون_len] = glyph_info;

    بیرون_len++;
    أرجع درست;
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
  HB_NODISCARD منطقي بعدی_glyph ()
  {
    إذا (have_output)
    {
      إذا (بیرون_info != info || بیرون_len != idx)
      {
	إذا (unlikely (!ensure (بیرون_len + 1))) أرجع خطأ;
	بیرون_info[بیرون_len] = info[idx];
      }
      بیرون_len++;
    }

    idx++;
    أرجع درست;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD منطقي بعدی_glyphs (بیعلامت صحيح n)
  {
    إذا (have_output)
    {
      إذا (بیرون_info != info || بیرون_len != idx)
      {
	إذا (unlikely (!ensure (بیرون_len + n))) أرجع خطأ;
	memmove (بیرون_info + بیرون_len, info + idx, n * حجم (بیرون_info[0]));
      }
      بیرون_len += n;
    }

    idx += n;
    أرجع درست;
  }
  /* Advance idx without copying to output. */
  فراغ skip_glyph () { idx++; }
  فراغ جمع_کن_masks (hb_ماسک_t ماسک)
  {
    لكل (بیعلامت صحيح j = 0; j < len; j++)
      info[j].ماسک |= ماسک;
  }
  HB_داخلی فراغ مجموعه_masks (hb_ماسک_t value, hb_ماسک_t ماسک,
			      بیعلامت صحيح cluster_شروع, بیعلامت صحيح cluster_انتها);

  فراغ ادغام_clusters (بیعلامت صحيح شروع, بیعلامت صحيح انتها)
  {
    إذا (انتها - شروع < 2)
      أرجع;
    إذا (!HB_میانگیر_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_مقصد_break (شروع, انتها);
      أرجع;
    }
    ادغام_clusters_impl (شروع, انتها);
  }
  فراغ ادغام_grapheme_clusters (بیعلامت صحيح شروع, بیعلامت صحيح انتها)
  {
    إذا (انتها - شروع < 2)
      أرجع;
    إذا (!HB_میانگیر_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_مقصد_break (شروع, انتها);
      أرجع;
    }
    ادغام_clusters_impl (شروع, انتها);
  }
  HB_داخلی فراغ ادغام_clusters_impl (بیعلامت صحيح شروع, بیعلامت صحيح انتها);
  فراغ ادغام_بیرون_clusters (بیعلامت صحيح شروع, بیعلامت صحيح انتها)
  {
    إذا (انتها - شروع < 2)
      أرجع;
    إذا (!HB_میانگیر_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      أرجع;
    ادغام_بیرون_clusters_impl (شروع, انتها);
  }
  فراغ ادغام_بیرون_grapheme_clusters (بیعلامت صحيح شروع, بیعلامت صحيح انتها)
  {
    إذا (انتها - شروع < 2)
      أرجع;
    إذا (!HB_میانگیر_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      أرجع;
    ادغام_بیرون_clusters_impl (شروع, انتها);
  }
  HB_داخلی فراغ ادغام_بیرون_clusters_impl (بیعلامت صحيح شروع, بیعلامت صحيح انتها);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_داخلی فراغ delete_glyph ();
  HB_داخلی فراغ delete_glyphs_inplace (منطقي (*filter) (ثابت hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  فراغ مجموعه_glyph_flags_impl (hb_ماسک_t ماسک,
			      بیعلامت شروع,
			      بیعلامت انتها,
			      منطقي interior,
			      منطقي from_بیرون_میانگیر)
  {
    إذا (!from_بیرون_میانگیر || !have_output)
    {
      إذا (!interior)
      {
	لكل (بیعلامت i = شروع; i < انتها; i++)
	  info[i].ماسک |= ماسک;
      }
      وإلا
      {
	بیعلامت cluster = infos_پیدا_کن_min_cluster (info, شروع, انتها);
	infos_مجموعه_glyph_flags (info, شروع, انتها, cluster, ماسک);
      }
    }
    وإلا
    {
      أكد (شروع <= بیرون_len);
      أكد (idx <= انتها);

      إذا (!interior)
      {
	لكل (بیعلامت i = شروع; i < بیرون_len; i++)
	  بیرون_info[i].ماسک |= ماسک;
	لكل (بیعلامت i = idx; i < انتها; i++)
	  info[i].ماسک |= ماسک;
      }
      وإلا
      {
	بیعلامت cluster = infos_پیدا_کن_min_cluster (info, idx, انتها);
	cluster = infos_پیدا_کن_min_cluster (بیرون_info, شروع, بیرون_len, cluster);

	infos_مجموعه_glyph_flags (بیرون_info, شروع, بیرون_len, cluster, ماسک);
	infos_مجموعه_glyph_flags (info, idx, انتها, cluster, ماسک);
      }
    }
  }

  HB_ALWAYS_INLINE
  فراغ مجموعه_glyph_flags (hb_ماسک_t ماسک,
			 بیعلامت شروع = 0,
			 بیعلامت انتها = (بیعلامت) -1,
			 منطقي interior = خطأ,
			 منطقي from_بیرون_میانگیر = خطأ)
  {
    إذا (unlikely (انتها != (بیعلامت) -1 && انتها - شروع > 255))
      أرجع;

    انتها = hb_min (انتها, len);

    إذا (interior && !from_بیرون_میانگیر && انتها - شروع < 2)
      أرجع;

    مجموعه_glyph_flags_impl (ماسک, شروع, انتها, interior, from_بیرون_میانگیر);
  }


  فراغ unsafe_مقصد_break (بیعلامت صحيح شروع = 0, بیعلامت صحيح انتها = -1)
  {
    مجموعه_glyph_flags (HB_GLYPH_FLAG_UNSAFE_مقصد_BREAK | HB_GLYPH_FLAG_UNSAFE_مقصد_CONCAT,
		      شروع, انتها,
		      درست);
  }
  فراغ امن_مقصد_درج_tatweel (بیعلامت صحيح شروع = 0, بیعلامت صحيح انتها = -1)
  {
    إذا ((flags & HB_میانگیر_FLAG_PRODUCE_امن_مقصد_درج_TATWEEL) == 0)
    {
      unsafe_مقصد_break (شروع, انتها);
      أرجع;
    }
    مجموعه_glyph_flags (HB_GLYPH_FLAG_امن_مقصد_درج_TATWEEL,
		      شروع, انتها,
		      درست);
  }
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  فراغ unsafe_مقصد_concat (بیعلامت صحيح شروع = 0, بیعلامت صحيح انتها = -1)
  {
    إذا (likely ((flags & HB_میانگیر_FLAG_PRODUCE_UNSAFE_مقصد_CONCAT) == 0))
      أرجع;
    مجموعه_glyph_flags (HB_GLYPH_FLAG_UNSAFE_مقصد_CONCAT,
		      شروع, انتها,
		      خطأ);
  }
  فراغ unsafe_مقصد_break_from_outbuffer (بیعلامت صحيح شروع = 0, بیعلامت صحيح انتها = -1)
  {
    مجموعه_glyph_flags (HB_GLYPH_FLAG_UNSAFE_مقصد_BREAK | HB_GLYPH_FLAG_UNSAFE_مقصد_CONCAT,
		      شروع, انتها,
		      درست, درست);
  }
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  فراغ unsafe_مقصد_concat_from_outbuffer (بیعلامت صحيح شروع = 0, بیعلامت صحيح انتها = -1)
  {
    إذا (likely ((flags & HB_میانگیر_FLAG_PRODUCE_UNSAFE_مقصد_CONCAT) == 0))
      أرجع;
    مجموعه_glyph_flags (HB_GLYPH_FLAG_UNSAFE_مقصد_CONCAT,
		      شروع, انتها,
		      خطأ, درست);
  }


  /* Internal methods */
  HB_NODISCARD HB_داخلی منطقي move_مقصد (بیعلامت صحيح i); /* i is output-buffer index. */

  HB_NODISCARD HB_داخلی منطقي enlarge (بیعلامت صحيح اندازه);

  HB_NODISCARD منطقي resize (بیعلامت درازی)
  {
    أكد (!have_output);
    إذا (unlikely (!ensure (درازی))) أرجع خطأ;
    len = درازی;
    أرجع درست;
  }
  HB_NODISCARD منطقي ensure (بیعلامت صحيح اندازه)
  { أرجع likely (!اندازه || اندازه < allocated) ? درست : enlarge (اندازه); }

  HB_NODISCARD منطقي ensure_inplace (بیعلامت صحيح اندازه)
  { أرجع likely (!اندازه || اندازه < allocated); }

  فراغ assert_glyphs ()
  {
    أكد ((content_type == HB_میانگیر_CONTENT_TYPE_GLYPHS) ||
	    (!len && (content_type == HB_میانگیر_CONTENT_TYPE_INVALID)));
  }
  فراغ assert_unicode ()
  {
    أكد ((content_type == HB_میانگیر_CONTENT_TYPE_UNICODE) ||
	    (!len && (content_type == HB_میانگیر_CONTENT_TYPE_INVALID)));
  }
  HB_NODISCARD منطقي ensure_glyphs ()
  {
    إذا (unlikely (content_type != HB_میانگیر_CONTENT_TYPE_GLYPHS))
    {
      إذا (content_type != HB_میانگیر_CONTENT_TYPE_INVALID)
	أرجع خطأ;
      أكد (len == 0);
      content_type = HB_میانگیر_CONTENT_TYPE_GLYPHS;
    }
    أرجع درست;
  }
  HB_NODISCARD منطقي ensure_unicode ()
  {
    إذا (unlikely (content_type != HB_میانگیر_CONTENT_TYPE_UNICODE))
    {
      إذا (content_type != HB_میانگیر_CONTENT_TYPE_INVALID)
	أرجع خطأ;
      أكد (len == 0);
      content_type = HB_میانگیر_CONTENT_TYPE_UNICODE;
    }
    أرجع درست;
  }

  HB_NODISCARD HB_داخلی منطقي make_جا_for (بیعلامت صحيح رقم_درون, بیعلامت صحيح رقم_بیرون);
  HB_NODISCARD HB_داخلی منطقي لغزش_forward (بیعلامت صحيح شمار);

  تعريفنوع دراز scratch_میانگیر_t;
  HB_داخلی scratch_میانگیر_t *get_scratch_میانگیر (بیعلامت صحيح *اندازه);

  فراغ clear_context (بیعلامت صحيح side) { context_len[side] = 0; }

  HB_داخلی فراغ sort (بیعلامت صحيح شروع, بیعلامت صحيح انتها, صحيح(*compar)(ثابت hb_glyph_info_t *, ثابت hb_glyph_info_t *));

  منطقي messaging ()
  {
#اگر_تعریف_شده HB_NO_میانگیر_پیام
    أرجع خطأ;
#وإلا
    أرجع unlikely (پیام_func);
#پایان_اگر
  }
  منطقي پیام (hb_font_t *font, ثابت حرف *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#اگر_تعریف_شده HB_NO_میانگیر_پیام
    أرجع درست;
#وإلا
    إذا (likely (!messaging ()))
      أرجع درست;

    va_list ap;
    va_شروع (ap, fmt);
    منطقي ret = پیام_impl (font, fmt, ap);
    va_انتها (ap);

    أرجع ret;
#پایان_اگر
  }
  HB_داخلی منطقي پیام_impl (hb_font_t *font, ثابت حرف *fmt, va_list ap) HB_PRINTF_FUNC(3, 0);
  HB_داخلی فراغ تغییریافته ();

  ساكن فراغ
  مجموعه_cluster (hb_glyph_info_t &inf, بیعلامت صحيح cluster, بیعلامت صحيح ماسک = 0)
  {
    إذا (inf.cluster != cluster)
      inf.ماسک = (inf.ماسک & ~HB_GLYPH_FLAG_DEFINED) | (ماسک & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  فراغ
  infos_مجموعه_glyph_flags (hb_glyph_info_t *infos,
			  بیعلامت صحيح شروع, بیعلامت صحيح انتها,
			  بیعلامت صحيح cluster,
			  hb_ماسک_t ماسک)
  {
    إذا (unlikely (شروع == انتها))
      أرجع;

    max_ops -= انتها - شروع;
    إذا (unlikely (max_ops < 0))
      successful = خطأ;

    بیعلامت cluster_اولی = infos[شروع].cluster;
    بیعلامت cluster_آخرین = infos[انتها - 1].cluster;

    إذا (cluster_level == HB_میانگیر_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_اولی && cluster != cluster_آخرین))
    {
      لكل (بیعلامت صحيح i = شروع; i < انتها; i++)
	إذا (cluster != infos[i].cluster)
	  infos[i].ماسک |= ماسک;
      أرجع;
    }

    /* Monotone clusters */

    إذا (cluster == cluster_اولی)
    {
      لكل (بیعلامت صحيح i = انتها; شروع < i && infos[i - 1].cluster != cluster_اولی; i--)
	infos[i - 1].ماسک |= ماسک;
    }
    وإلا /* cluster == cluster_last */
    {
      لكل (بیعلامت صحيح i = شروع; i < انتها && infos[i].cluster != cluster_آخرین; i++)
	infos[i].ماسک |= ماسک;
    }
  }
  بیعلامت
  infos_پیدا_کن_min_cluster (ثابت hb_glyph_info_t *infos,
			   بیعلامت شروع, بیعلامت انتها,
			   بیعلامت cluster = UINT_MAX)
  {
    إذا (unlikely (شروع == انتها))
      أرجع cluster;

    إذا (cluster_level == HB_میانگیر_CLUSTER_LEVEL_CHARACTERS)
    {
      لكل (بیعلامت صحيح i = شروع; i < انتها; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      أرجع cluster;
    }

    أرجع hb_min (cluster, hb_min (infos[شروع].cluster, infos[انتها - 1].cluster));
  }

  فراغ clear_glyph_flags (hb_ماسک_t ماسک = 0)
  {
    لكل (بیعلامت صحيح i = 0; i < len; i++)
      info[i].ماسک = (info[i].ماسک & ~HB_GLYPH_FLAG_DEFINED) | (ماسک & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_میانگیر_t);


#تعریف_کن foreach_گروه(میانگیر, شروع, انتها, گروه_func) \
  لكل (بیعلامت صحيح \
       شمار_2 = میانگیر->len, \
       شروع = 0, انتها = شمار_2 ? میانگیر->گروه_انتها (0, گروه_func) : 0; \
       شروع < شمار_2; \
       شروع = انتها, انتها = میانگیر->گروه_انتها (شروع, گروه_func))

#تعریف_کن foreach_cluster(میانگیر, شروع, انتها) \
	foreach_گروه (میانگیر, شروع, انتها, hb_میانگیر_t::cluster_گروه_func)


#تعریف_کن HB_میانگیر_XALLOCATE_VAR(b, func, var) \
  b->func (offsetof (hb_glyph_info_t, var) - offsetof(hb_glyph_info_t, var1), \
	   حجم (b->info[0].var))
#تعریف_کن HB_میانگیر_ALLOCATE_VAR(b, var)		HB_میانگیر_XALLOCATE_VAR (b, allocate_var,     var ())
#تعریف_کن HB_میانگیر_TRY_ALLOCATE_VAR(b, var)	HB_میانگیر_XALLOCATE_VAR (b, try_allocate_var, var ())
#تعریف_کن HB_میانگیر_DEALLOCATE_VAR(b, var)	HB_میانگیر_XALLOCATE_VAR (b, deallocate_var,   var ())
#تعریف_کن HB_میانگیر_ASSERT_VAR(b, var)		HB_میانگیر_XALLOCATE_VAR (b, assert_var,       var ())


#پایان_اگر /* HB_BUFFER_HH */
