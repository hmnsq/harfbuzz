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

#اگر_غیر_تعریف_شدہ HB_بفر_HH
#تعریف_کرو HB_بفر_HH

#شامل "hb.hh"
#شامل "hb-unicode.hh"
#شامل "hb-set-digest.hh"


static_assert ((حجم (hb_glyph_info_t) == 20), "");
static_assert ((حجم (hb_glyph_info_t) == حجم (hb_glyph_position_t)), "");

HB_MARK_AS_FLAG_T (hb_glyph_flags_t);
HB_MARK_AS_FLAG_T (hb_بفر_flags_t);
HB_MARK_AS_FLAG_T (hb_بفر_serialize_flags_t);
HB_MARK_AS_FLAG_T (hb_بفر_diff_flags_t);

شمار hb_بفر_scratch_flags_t {
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

ساخت hb_بفر_t
{
  hb_object_سرخی_t سرخی;

  /*
   * Information about how the text in the buffer should be treated.
   */

  hb_unicode_funcs_t *unicode; /* Unicode functions */
  hb_بفر_flags_t flags; /* BOT / EOT / etc. */
  hb_بفر_cluster_level_t cluster_level;
  hb_codepoint_t replacement; /* U+FFFD or something else. */
  hb_codepoint_t invisible; /* 0 or something else. */
  hb_codepoint_t not_ملا; /* 0 or something else. */
  hb_codepoint_t not_ملا_variation_selector; /* HB_CODEPOINT_INVALID or something else. */

  /*
   * Buffer contents
   */

  hb_بفر_content_type_t content_type;
  hb_segment_properties_t props; /* Script, language, direction */

  منطقی successful; /* Allocations successful */
  منطقی have_output; /* Whether we have an output buffer going on */
  منطقی have_positions; /* Whether we have positions */

  غیرعلامتی صحیح idx; /* Cursor into ->info and ->pos arrays */
  غیرعلامتی صحیح len; /* Length of ->info and ->pos arrays */
  غیرعلامتی صحیح برآمد_len; /* Length of ->out_info array if have_output */

  غیرعلامتی صحیح allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *برآمد_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  ساکن constexpr غیرعلامتی CONTEXT_لمبائی = 5u;
  hb_codepoint_t context[2][CONTEXT_لمبائی];
  غیرعلامتی صحیح context_len[2];

  hb_سیٹ_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_بٹ;
  uint8_t serial;
  uint32_t random_state;
  hb_بفر_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  غیرعلامتی صحیح max_len; /* Maximum allowed len. */
  صحیح max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#اگر_غیر_تعریف_شدہ HB_NO_بفر_پیغام
  قسمتعریف خالی (*بدلا_func_t) (hb_بفر_t *بفر, خالی *user_ڈیٹا);

  hb_بفر_پیغام_func_t پیغام_func;
  خالی *پیغام_ڈیٹا;
  hb_destroy_func_t پیغام_destroy;
  بدلا_func_t بدلا_func;
  خالی *بدلا_ڈیٹا;
  غیرعلامتی پیغام_گہرائی; /* How deeply are we inside a message callback? */
#ورنہ
  ساکن constexpr غیرعلامتی پیغام_گہرائی = 0u;
#اگر_ختم



  /* Methods */

  HB_NODISCARD منطقی اندر_error () ثابت { واپس !successful; }

  خالی allocate_var (غیرعلامتی صحیح آغاز, غیرعلامتی صحیح گنتی)
  {
    غیرعلامتی صحیح انتہا = آغاز + گنتی;
    أكد (انتہا <= 8);
    غیرعلامتی صحیح بٹ = (1u<<انتہا) - (1u<<آغاز);
    أكد (0 == (allocated_var_بٹ & بٹ));
    allocated_var_بٹ |= بٹ;
  }
  منطقی try_allocate_var (غیرعلامتی صحیح آغاز, غیرعلامتی صحیح گنتی)
  {
    غیرعلامتی صحیح انتہا = آغاز + گنتی;
    أكد (انتہا <= 8);
    غیرعلامتی صحیح بٹ = (1u<<انتہا) - (1u<<آغاز);
    اگر (allocated_var_بٹ & بٹ)
      واپس جھوٹ;
    allocated_var_بٹ |= بٹ;
    واپس سچ;
  }
  خالی deallocate_var (غیرعلامتی صحیح آغاز, غیرعلامتی صحیح گنتی)
  {
    غیرعلامتی صحیح انتہا = آغاز + گنتی;
    أكد (انتہا <= 8);
    غیرعلامتی صحیح بٹ = (1u<<انتہا) - (1u<<آغاز);
    أكد (بٹ == (allocated_var_بٹ & بٹ));
    allocated_var_بٹ &= ~بٹ;
  }
  خالی assert_var (غیرعلامتی صحیح آغاز, غیرعلامتی صحیح گنتی)
  {
    غیرعلامتی صحیح انتہا = آغاز + گنتی;
    أكد (انتہا <= 8);
    HB_غیر_مستعمل غیرعلامتی صحیح بٹ = (1u<<انتہا) - (1u<<آغاز);
    أكد (بٹ == (allocated_var_بٹ & بٹ));
  }
  خالی deallocate_var_all ()
  {
    allocated_var_بٹ = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (غیرعلامتی صحیح i = 0) { واپس info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (غیرعلامتی صحیح i = 0) ثابت { واپس info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (غیرعلامتی صحیح i = 0) { واپس pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (غیرعلامتی صحیح i = 0) ثابت { واپس pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { واپس برآمد_info[برآمد_len ? برآمد_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () ثابت { واپس برآمد_info[برآمد_len ? برآمد_len - 1 : 0]; }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_codepoints (سیٹ_t &d) ثابت
  { d.clear (); d.جوڑو_array (&info[0].codepoint, len, حجم (info[0])); }

  خالی update_digest ()
  {
    digest = hb_سیٹ_digest_t ();
    collect_codepoints (digest);
  }

  HB_اندرونی خالی similar (ثابت hb_بفر_t &src);
  HB_اندرونی خالی reset ();
  HB_اندرونی خالی clear ();

  /* Called around shape() */
  HB_اندرونی خالی enter ();
  HB_اندرونی خالی leave ();

#اگر_غیر_تعریف_شدہ HB_NO_بفر_VERIFY
  HB_اندرونی
#اگر_ختم
  منطقی verify (hb_بفر_t        *متن_بفر,
	       hb_font_t          *font,
	       ثابت hb_feature_t *features,
	       غیرعلامتی صحیح        عدد_features,
	       ثابت حرف * ثابت *shapers)
#اگر_غیر_تعریف_شدہ HB_NO_بفر_VERIFY
  ;
#ورنہ
  { واپس سچ; }
#اگر_ختم

  غیرعلامتی صحیح backtrack_len () ثابت { واپس have_output ? برآمد_len : idx; }
  غیرعلامتی صحیح lookahead_len () ثابت { واپس len - idx; }
  uint8_t اگلا_serial () { واپس ++serial ? serial : ++serial; }

  HB_اندرونی خالی جوڑو (hb_codepoint_t  codepoint,
			غیرعلامتی صحیح    cluster);
  HB_اندرونی خالی جوڑو_info (ثابت hb_glyph_info_t &glyph_info);
  HB_اندرونی خالی جوڑو_info_and_pos (ثابت hb_glyph_info_t &glyph_info,
				     ثابت hb_glyph_position_t &glyph_pos);

  خالی الٹ_range (غیرعلامتی آغاز, غیرعلامتی انتہا)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (آغاز, انتہا);
    اگر (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (آغاز, انتہا);
  }
  خالی reverse () { الٹ_range (0, len); }

  سانچہ <قسم_نام FuncType>
  خالی الٹ_groups (ثابت FuncType& گروہ,
		       منطقی ملاؤ_clusters = جھوٹ)
  {
    اگر (unlikely (!len))
      واپس;

    غیرعلامتی آغاز = 0;
    غیرعلامتی i;
    برائے (i = 1; i < len; i++)
    {
      اگر (!گروہ (info[i - 1], info[i]))
      {
	اگر (ملاؤ_clusters)
	  یہ->ملاؤ_clusters (آغاز, i);
	الٹ_range (آغاز, i);
	آغاز = i;
      }
    }
    اگر (ملاؤ_clusters)
      یہ->ملاؤ_clusters (آغاز, i);
    الٹ_range (آغاز, i);

    reverse ();
  }

  سانچہ <قسم_نام FuncType>
  غیرعلامتی گروہ_انتہا (غیرعلامتی آغاز, ثابت FuncType& گروہ) ثابت
  {
    جب (++آغاز < len && گروہ (info[آغاز - 1], info[آغاز]))
      ;

    واپس آغاز;
  }

  ساکن منطقی cluster_گروہ_func (ثابت hb_glyph_info_t& a,
				   ثابت hb_glyph_info_t& b)
  { واپس a.cluster == b.cluster; }

  خالی الٹ_clusters () { الٹ_groups (cluster_گروہ_func); }

  HB_اندرونی خالی guess_segment_properties ();

  HB_اندرونی منطقی ہمآہنگ ();
  HB_اندرونی صحیح sync_so_far ();
  HB_اندرونی خالی clear_output ();
  HB_اندرونی خالی clear_positions ();

  سانچہ <قسم_نام T>
  HB_NODISCARD منطقی replace_glyphs (غیرعلامتی صحیح عدد_اندر,
				    غیرعلامتی صحیح عدد_برآمد,
				    ثابت T *glyph_ڈیٹا)
  {
    اگر (unlikely (!make_گنجائش_for (عدد_اندر, عدد_برآمد))) واپس جھوٹ;

    أكد (idx + عدد_اندر <= len);

    ملاؤ_clusters (idx, idx + عدد_اندر);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &برآمد_info[برآمد_len];
    برائے (غیرعلامتی صحیح i = 0; i < عدد_برآمد; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_ڈیٹا[i];
      pinfo++;
    }

    idx  += عدد_اندر;
    برآمد_len += عدد_برآمد;
    واپس سچ;
  }

  HB_NODISCARD منطقی replace_glyph (hb_codepoint_t glyph_اشاریہ)
  { واپس replace_glyphs (1, 1, &glyph_اشاریہ); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD منطقی output_glyph (hb_codepoint_t glyph_اشاریہ)
  { واپس replace_glyphs (0, 1, &glyph_اشاریہ); }

  HB_NODISCARD منطقی output_info (ثابت hb_glyph_info_t &glyph_info)
  {
    اگر (unlikely (!make_گنجائش_for (0, 1))) واپس جھوٹ;

    برآمد_info[برآمد_len] = glyph_info;

    برآمد_len++;
    واپس سچ;
  }
  /* Copies glyph at idx to output but doesn't advance idx */
  HB_NODISCARD منطقی copy_glyph ()
  {
    /* Extra copy because cur()'s return can be freed within
     * output_info() call if buffer reallocates. */
    واپس output_info (hb_glyph_info_t (cur()));
  }

  /* Copies glyph at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD منطقی اگلا_glyph ()
  {
    اگر (have_output)
    {
      اگر (برآمد_info != info || برآمد_len != idx)
      {
	اگر (unlikely (!ensure (برآمد_len + 1))) واپس جھوٹ;
	برآمد_info[برآمد_len] = info[idx];
      }
      برآمد_len++;
    }

    idx++;
    واپس سچ;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD منطقی اگلا_glyphs (غیرعلامتی صحیح n)
  {
    اگر (have_output)
    {
      اگر (برآمد_info != info || برآمد_len != idx)
      {
	اگر (unlikely (!ensure (برآمد_len + n))) واپس جھوٹ;
	memmove (برآمد_info + برآمد_len, info + idx, n * حجم (برآمد_info[0]));
      }
      برآمد_len += n;
    }

    idx += n;
    واپس سچ;
  }
  /* Advance idx without copying to output. */
  خالی skip_glyph () { idx++; }
  خالی جوڑو_masks (hb_نقاب_t نقاب)
  {
    برائے (غیرعلامتی صحیح j = 0; j < len; j++)
      info[j].نقاب |= نقاب;
  }
  HB_اندرونی خالی سیٹ_masks (hb_نقاب_t value, hb_نقاب_t نقاب,
			      غیرعلامتی صحیح cluster_آغاز, غیرعلامتی صحیح cluster_انتہا);

  خالی ملاؤ_clusters (غیرعلامتی صحیح آغاز, غیرعلامتی صحیح انتہا)
  {
    اگر (انتہا - آغاز < 2)
      واپس;
    اگر (!HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_مقصد_break (آغاز, انتہا);
      واپس;
    }
    ملاؤ_clusters_impl (آغاز, انتہا);
  }
  خالی ملاؤ_grapheme_clusters (غیرعلامتی صحیح آغاز, غیرعلامتی صحیح انتہا)
  {
    اگر (انتہا - آغاز < 2)
      واپس;
    اگر (!HB_بفر_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_مقصد_break (آغاز, انتہا);
      واپس;
    }
    ملاؤ_clusters_impl (آغاز, انتہا);
  }
  HB_اندرونی خالی ملاؤ_clusters_impl (غیرعلامتی صحیح آغاز, غیرعلامتی صحیح انتہا);
  خالی ملاؤ_برآمد_clusters (غیرعلامتی صحیح آغاز, غیرعلامتی صحیح انتہا)
  {
    اگر (انتہا - آغاز < 2)
      واپس;
    اگر (!HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      واپس;
    ملاؤ_برآمد_clusters_impl (آغاز, انتہا);
  }
  خالی ملاؤ_برآمد_grapheme_clusters (غیرعلامتی صحیح آغاز, غیرعلامتی صحیح انتہا)
  {
    اگر (انتہا - آغاز < 2)
      واپس;
    اگر (!HB_بفر_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      واپس;
    ملاؤ_برآمد_clusters_impl (آغاز, انتہا);
  }
  HB_اندرونی خالی ملاؤ_برآمد_clusters_impl (غیرعلامتی صحیح آغاز, غیرعلامتی صحیح انتہا);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_اندرونی خالی delete_glyph ();
  HB_اندرونی خالی delete_glyphs_inplace (منطقی (*filter) (ثابت hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  خالی سیٹ_glyph_flags_impl (hb_نقاب_t نقاب,
			      غیرعلامتی آغاز,
			      غیرعلامتی انتہا,
			      منطقی interior,
			      منطقی from_برآمد_بفر)
  {
    اگر (!from_برآمد_بفر || !have_output)
    {
      اگر (!interior)
      {
	برائے (غیرعلامتی i = آغاز; i < انتہا; i++)
	  info[i].نقاب |= نقاب;
      }
      ورنہ
      {
	غیرعلامتی cluster = infos_ڈھونڈو_min_cluster (info, آغاز, انتہا);
	infos_سیٹ_glyph_flags (info, آغاز, انتہا, cluster, نقاب);
      }
    }
    ورنہ
    {
      أكد (آغاز <= برآمد_len);
      أكد (idx <= انتہا);

      اگر (!interior)
      {
	برائے (غیرعلامتی i = آغاز; i < برآمد_len; i++)
	  برآمد_info[i].نقاب |= نقاب;
	برائے (غیرعلامتی i = idx; i < انتہا; i++)
	  info[i].نقاب |= نقاب;
      }
      ورنہ
      {
	غیرعلامتی cluster = infos_ڈھونڈو_min_cluster (info, idx, انتہا);
	cluster = infos_ڈھونڈو_min_cluster (برآمد_info, آغاز, برآمد_len, cluster);

	infos_سیٹ_glyph_flags (برآمد_info, آغاز, برآمد_len, cluster, نقاب);
	infos_سیٹ_glyph_flags (info, idx, انتہا, cluster, نقاب);
      }
    }
  }

  HB_ALWAYS_INLINE
  خالی سیٹ_glyph_flags (hb_نقاب_t نقاب,
			 غیرعلامتی آغاز = 0,
			 غیرعلامتی انتہا = (غیرعلامتی) -1,
			 منطقی interior = جھوٹ,
			 منطقی from_برآمد_بفر = جھوٹ)
  {
    اگر (unlikely (انتہا != (غیرعلامتی) -1 && انتہا - آغاز > 255))
      واپس;

    انتہا = hb_min (انتہا, len);

    اگر (interior && !from_برآمد_بفر && انتہا - آغاز < 2)
      واپس;

    سیٹ_glyph_flags_impl (نقاب, آغاز, انتہا, interior, from_برآمد_بفر);
  }


  خالی unsafe_مقصد_break (غیرعلامتی صحیح آغاز = 0, غیرعلامتی صحیح انتہا = -1)
  {
    سیٹ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_مقصد_BREAK | HB_GLYPH_FLAG_UNSAFE_مقصد_CONCAT,
		      آغاز, انتہا,
		      سچ);
  }
  خالی محفوظ_مقصد_داخل_کرو_tatweel (غیرعلامتی صحیح آغاز = 0, غیرعلامتی صحیح انتہا = -1)
  {
    اگر ((flags & HB_بفر_FLAG_PRODUCE_محفوظ_مقصد_داخل_کرو_TATWEEL) == 0)
    {
      unsafe_مقصد_break (آغاز, انتہا);
      واپس;
    }
    سیٹ_glyph_flags (HB_GLYPH_FLAG_محفوظ_مقصد_داخل_کرو_TATWEEL,
		      آغاز, انتہا,
		      سچ);
  }
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  خالی unsafe_مقصد_concat (غیرعلامتی صحیح آغاز = 0, غیرعلامتی صحیح انتہا = -1)
  {
    اگر (likely ((flags & HB_بفر_FLAG_PRODUCE_UNSAFE_مقصد_CONCAT) == 0))
      واپس;
    سیٹ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_مقصد_CONCAT,
		      آغاز, انتہا,
		      جھوٹ);
  }
  خالی unsafe_مقصد_break_from_outbuffer (غیرعلامتی صحیح آغاز = 0, غیرعلامتی صحیح انتہا = -1)
  {
    سیٹ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_مقصد_BREAK | HB_GLYPH_FLAG_UNSAFE_مقصد_CONCAT,
		      آغاز, انتہا,
		      سچ, سچ);
  }
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  خالی unsafe_مقصد_concat_from_outbuffer (غیرعلامتی صحیح آغاز = 0, غیرعلامتی صحیح انتہا = -1)
  {
    اگر (likely ((flags & HB_بفر_FLAG_PRODUCE_UNSAFE_مقصد_CONCAT) == 0))
      واپس;
    سیٹ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_مقصد_CONCAT,
		      آغاز, انتہا,
		      جھوٹ, سچ);
  }


  /* Internal methods */
  HB_NODISCARD HB_اندرونی منطقی move_مقصد (غیرعلامتی صحیح i); /* i is output-buffer index. */

  HB_NODISCARD HB_اندرونی منطقی enlarge (غیرعلامتی صحیح سائز);

  HB_NODISCARD منطقی resize (غیرعلامتی لمبائی)
  {
    أكد (!have_output);
    اگر (unlikely (!ensure (لمبائی))) واپس جھوٹ;
    len = لمبائی;
    واپس سچ;
  }
  HB_NODISCARD منطقی ensure (غیرعلامتی صحیح سائز)
  { واپس likely (!سائز || سائز < allocated) ? سچ : enlarge (سائز); }

  HB_NODISCARD منطقی ensure_inplace (غیرعلامتی صحیح سائز)
  { واپس likely (!سائز || سائز < allocated); }

  خالی assert_glyphs ()
  {
    أكد ((content_type == HB_بفر_CONTENT_TYPE_GLYPHS) ||
	    (!len && (content_type == HB_بفر_CONTENT_TYPE_INVALID)));
  }
  خالی assert_unicode ()
  {
    أكد ((content_type == HB_بفر_CONTENT_TYPE_UNICODE) ||
	    (!len && (content_type == HB_بفر_CONTENT_TYPE_INVALID)));
  }
  HB_NODISCARD منطقی ensure_glyphs ()
  {
    اگر (unlikely (content_type != HB_بفر_CONTENT_TYPE_GLYPHS))
    {
      اگر (content_type != HB_بفر_CONTENT_TYPE_INVALID)
	واپس جھوٹ;
      أكد (len == 0);
      content_type = HB_بفر_CONTENT_TYPE_GLYPHS;
    }
    واپس سچ;
  }
  HB_NODISCARD منطقی ensure_unicode ()
  {
    اگر (unlikely (content_type != HB_بفر_CONTENT_TYPE_UNICODE))
    {
      اگر (content_type != HB_بفر_CONTENT_TYPE_INVALID)
	واپس جھوٹ;
      أكد (len == 0);
      content_type = HB_بفر_CONTENT_TYPE_UNICODE;
    }
    واپس سچ;
  }

  HB_NODISCARD HB_اندرونی منطقی make_گنجائش_for (غیرعلامتی صحیح عدد_اندر, غیرعلامتی صحیح عدد_برآمد);
  HB_NODISCARD HB_اندرونی منطقی سرکاؤ_forward (غیرعلامتی صحیح گنتی);

  قسمتعریف طویل scratch_بفر_t;
  HB_اندرونی scratch_بفر_t *get_scratch_بفر (غیرعلامتی صحیح *سائز);

  خالی clear_context (غیرعلامتی صحیح side) { context_len[side] = 0; }

  HB_اندرونی خالی sort (غیرعلامتی صحیح آغاز, غیرعلامتی صحیح انتہا, صحیح(*compar)(ثابت hb_glyph_info_t *, ثابت hb_glyph_info_t *));

  منطقی messaging ()
  {
#اگر_تعریف_شدہ HB_NO_بفر_پیغام
    واپس جھوٹ;
#ورنہ
    واپس unlikely (پیغام_func);
#اگر_ختم
  }
  منطقی پیغام (hb_font_t *font, ثابت حرف *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#اگر_تعریف_شدہ HB_NO_بفر_پیغام
    واپس سچ;
#ورنہ
    اگر (likely (!messaging ()))
      واپس سچ;

    va_list ap;
    va_آغاز (ap, fmt);
    منطقی ret = پیغام_impl (font, fmt, ap);
    va_انتہا (ap);

    واپس ret;
#اگر_ختم
  }
  HB_اندرونی منطقی پیغام_impl (hb_font_t *font, ثابت حرف *fmt, va_list ap) HB_PRINTF_FUNC(3, 0);
  HB_اندرونی خالی بدلا ();

  ساکن خالی
  سیٹ_cluster (hb_glyph_info_t &inf, غیرعلامتی صحیح cluster, غیرعلامتی صحیح نقاب = 0)
  {
    اگر (inf.cluster != cluster)
      inf.نقاب = (inf.نقاب & ~HB_GLYPH_FLAG_DEFINED) | (نقاب & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  خالی
  infos_سیٹ_glyph_flags (hb_glyph_info_t *infos,
			  غیرعلامتی صحیح آغاز, غیرعلامتی صحیح انتہا,
			  غیرعلامتی صحیح cluster,
			  hb_نقاب_t نقاب)
  {
    اگر (unlikely (آغاز == انتہا))
      واپس;

    max_ops -= انتہا - آغاز;
    اگر (unlikely (max_ops < 0))
      successful = جھوٹ;

    غیرعلامتی cluster_پہلا = infos[آغاز].cluster;
    غیرعلامتی cluster_آخری = infos[انتہا - 1].cluster;

    اگر (cluster_level == HB_بفر_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_پہلا && cluster != cluster_آخری))
    {
      برائے (غیرعلامتی صحیح i = آغاز; i < انتہا; i++)
	اگر (cluster != infos[i].cluster)
	  infos[i].نقاب |= نقاب;
      واپس;
    }

    /* Monotone clusters */

    اگر (cluster == cluster_پہلا)
    {
      برائے (غیرعلامتی صحیح i = انتہا; آغاز < i && infos[i - 1].cluster != cluster_پہلا; i--)
	infos[i - 1].نقاب |= نقاب;
    }
    ورنہ /* cluster == cluster_last */
    {
      برائے (غیرعلامتی صحیح i = آغاز; i < انتہا && infos[i].cluster != cluster_آخری; i++)
	infos[i].نقاب |= نقاب;
    }
  }
  غیرعلامتی
  infos_ڈھونڈو_min_cluster (ثابت hb_glyph_info_t *infos,
			   غیرعلامتی آغاز, غیرعلامتی انتہا,
			   غیرعلامتی cluster = UINT_MAX)
  {
    اگر (unlikely (آغاز == انتہا))
      واپس cluster;

    اگر (cluster_level == HB_بفر_CLUSTER_LEVEL_CHARACTERS)
    {
      برائے (غیرعلامتی صحیح i = آغاز; i < انتہا; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      واپس cluster;
    }

    واپس hb_min (cluster, hb_min (infos[آغاز].cluster, infos[انتہا - 1].cluster));
  }

  خالی clear_glyph_flags (hb_نقاب_t نقاب = 0)
  {
    برائے (غیرعلامتی صحیح i = 0; i < len; i++)
      info[i].نقاب = (info[i].نقاب & ~HB_GLYPH_FLAG_DEFINED) | (نقاب & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_بفر_t);


#تعریف_کرو foreach_گروہ(بفر, آغاز, انتہا, گروہ_func) \
  برائے (غیرعلامتی صحیح \
       گنتی_2 = بفر->len, \
       آغاز = 0, انتہا = گنتی_2 ? بفر->گروہ_انتہا (0, گروہ_func) : 0; \
       آغاز < گنتی_2; \
       آغاز = انتہا, انتہا = بفر->گروہ_انتہا (آغاز, گروہ_func))

#تعریف_کرو foreach_cluster(بفر, آغاز, انتہا) \
	foreach_گروہ (بفر, آغاز, انتہا, hb_بفر_t::cluster_گروہ_func)


#تعریف_کرو HB_بفر_XALLOCATE_VAR(b, func, var) \
  b->func (offsetof (hb_glyph_info_t, var) - offsetof(hb_glyph_info_t, var1), \
	   حجم (b->info[0].var))
#تعریف_کرو HB_بفر_ALLOCATE_VAR(b, var)		HB_بفر_XALLOCATE_VAR (b, allocate_var,     var ())
#تعریف_کرو HB_بفر_TRY_ALLOCATE_VAR(b, var)	HB_بفر_XALLOCATE_VAR (b, try_allocate_var, var ())
#تعریف_کرو HB_بفر_DEALLOCATE_VAR(b, var)	HB_بفر_XALLOCATE_VAR (b, deallocate_var,   var ())
#تعریف_کرو HB_بفر_ASSERT_VAR(b, var)		HB_بفر_XALLOCATE_VAR (b, assert_var,       var ())


#اگر_ختم /* HB_BUFFER_HH */
