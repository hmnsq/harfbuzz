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

#جے_بنا_تعریف HB_بفر_HH
#تعریف_کرو HB_بفر_HH

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
  hb_object_سرخی_t سرخی;

  /*
   * Information about how the text in the buffer should be treated.
   */

  hb_unicode_funcs_t *unicode; /* Unicode functions */
  hb_بفر_flags_t flags; /* BOT / EOT / etc. */
  hb_بفر_cluster_level_t cluster_level;
  hb_codepoint_t replacement; /* U+FFFD or something else. */
  hb_codepoint_t invisible; /* 0 or something else. */
  hb_codepoint_t not_لبھیا; /* 0 or something else. */
  hb_codepoint_t not_لبھیا_variation_selector; /* HB_CODEPOINT_INVALID or something else. */

  /*
   * Buffer contents
   */

  hb_بفر_content_type_t content_type;
  hb_segment_properties_t props; /* Script, language, direction */

  منطقي successful; /* Allocations successful */
  منطقي have_output; /* Whether we have an output buffer going on */
  منطقي have_positions; /* Whether we have positions */

  بنانشانی صحيح idx; /* Cursor into ->info and ->pos arrays */
  بنانشانی صحيح len; /* Length of ->info and ->pos arrays */
  بنانشانی صحيح باہرلا_len; /* Length of ->out_info array if have_output */

  بنانشانی صحيح allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *باہرلا_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  ساكن constexpr بنانشانی CONTEXT_لمائی = 5u;
  hb_codepoint_t context[2][CONTEXT_لمائی];
  بنانشانی صحيح context_len[2];

  hb_سیٹ_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_بٹ;
  uint8_t serial;
  uint32_t random_state;
  hb_بفر_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  بنانشانی صحيح max_len; /* Maximum allowed len. */
  صحيح max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#جے_بنا_تعریف HB_NO_بفر_سنیہا
  تعريفنوع فراغ (*بدلیا_func_t) (hb_بفر_t *بفر, فراغ *user_انکڑے);

  hb_بفر_سنیہا_func_t سنیہا_func;
  فراغ *سنیہا_انکڑے;
  hb_destroy_func_t سنیہا_destroy;
  بدلیا_func_t بدلیا_func;
  فراغ *بدلیا_انکڑے;
  بنانشانی سنیہا_ڈونگھائی; /* How deeply are we inside a message callback? */
#وإلا
  ساكن constexpr بنانشانی سنیہا_ڈونگھائی = 0u;
#جے_انت



  /* Methods */

  HB_NODISCARD منطقي وچ_error () ثابت { أرجع !successful; }

  فراغ allocate_var (بنانشانی صحيح شروع, بنانشانی صحيح گنتی)
  {
    بنانشانی صحيح سرا = شروع + گنتی;
    أكد (سرا <= 8);
    بنانشانی صحيح بٹ = (1u<<سرا) - (1u<<شروع);
    أكد (0 == (allocated_var_بٹ & بٹ));
    allocated_var_بٹ |= بٹ;
  }
  منطقي try_allocate_var (بنانشانی صحيح شروع, بنانشانی صحيح گنتی)
  {
    بنانشانی صحيح سرا = شروع + گنتی;
    أكد (سرا <= 8);
    بنانشانی صحيح بٹ = (1u<<سرا) - (1u<<شروع);
    إذا (allocated_var_بٹ & بٹ)
      أرجع خطأ;
    allocated_var_بٹ |= بٹ;
    أرجع سچ;
  }
  فراغ deallocate_var (بنانشانی صحيح شروع, بنانشانی صحيح گنتی)
  {
    بنانشانی صحيح سرا = شروع + گنتی;
    أكد (سرا <= 8);
    بنانشانی صحيح بٹ = (1u<<سرا) - (1u<<شروع);
    أكد (بٹ == (allocated_var_بٹ & بٹ));
    allocated_var_بٹ &= ~بٹ;
  }
  فراغ assert_var (بنانشانی صحيح شروع, بنانشانی صحيح گنتی)
  {
    بنانشانی صحيح سرا = شروع + گنتی;
    أكد (سرا <= 8);
    HB_بن_ورتیا بنانشانی صحيح بٹ = (1u<<سرا) - (1u<<شروع);
    أكد (بٹ == (allocated_var_بٹ & بٹ));
  }
  فراغ deallocate_var_all ()
  {
    allocated_var_بٹ = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (بنانشانی صحيح i = 0) { أرجع info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (بنانشانی صحيح i = 0) ثابت { أرجع info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (بنانشانی صحيح i = 0) { أرجع pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (بنانشانی صحيح i = 0) ثابت { أرجع pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { أرجع باہرلا_info[باہرلا_len ? باہرلا_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () ثابت { أرجع باہرلا_info[باہرلا_len ? باہرلا_len - 1 : 0]; }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_codepoints (سیٹ_t &d) ثابت
  { d.clear (); d.جوڑو_array (&info[0].codepoint, len, حجم (info[0])); }

  فراغ update_digest ()
  {
    digest = hb_سیٹ_digest_t ();
    collect_codepoints (digest);
  }

  HB_اندرونی فراغ similar (ثابت hb_بفر_t &src);
  HB_اندرونی فراغ reset ();
  HB_اندرونی فراغ clear ();

  /* Called around shape() */
  HB_اندرونی فراغ enter ();
  HB_اندرونی فراغ leave ();

#جے_بنا_تعریف HB_NO_بفر_VERIFY
  HB_اندرونی
#جے_انت
  منطقي verify (hb_بفر_t        *متن_بفر,
	       hb_font_t          *font,
	       ثابت hb_feature_t *features,
	       بنانشانی صحيح        عدد_features,
	       ثابت حرف * ثابت *shapers)
#جے_بنا_تعریف HB_NO_بفر_VERIFY
  ;
#وإلا
  { أرجع سچ; }
#جے_انت

  بنانشانی صحيح backtrack_len () ثابت { أرجع have_output ? باہرلا_len : idx; }
  بنانشانی صحيح lookahead_len () ثابت { أرجع len - idx; }
  uint8_t اگلا_serial () { أرجع ++serial ? serial : ++serial; }

  HB_اندرونی فراغ جوڑو (hb_codepoint_t  codepoint,
			بنانشانی صحيح    cluster);
  HB_اندرونی فراغ جوڑو_info (ثابت hb_glyph_info_t &glyph_info);
  HB_اندرونی فراغ جوڑو_info_and_pos (ثابت hb_glyph_info_t &glyph_info,
				     ثابت hb_glyph_position_t &glyph_pos);

  فراغ الٹ_range (بنانشانی شروع, بنانشانی سرا)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (شروع, سرا);
    إذا (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (شروع, سرا);
  }
  فراغ reverse () { الٹ_range (0, len); }

  سانچا <قسم_ناں FuncType>
  فراغ الٹ_groups (ثابت FuncType& گروہ,
		       منطقي ملاؤ_clusters = خطأ)
  {
    إذا (unlikely (!len))
      أرجع;

    بنانشانی شروع = 0;
    بنانشانی i;
    لكل (i = 1; i < len; i++)
    {
      إذا (!گروہ (info[i - 1], info[i]))
      {
	إذا (ملاؤ_clusters)
	  هذا->ملاؤ_clusters (شروع, i);
	الٹ_range (شروع, i);
	شروع = i;
      }
    }
    إذا (ملاؤ_clusters)
      هذا->ملاؤ_clusters (شروع, i);
    الٹ_range (شروع, i);

    reverse ();
  }

  سانچا <قسم_ناں FuncType>
  بنانشانی گروہ_سرا (بنانشانی شروع, ثابت FuncType& گروہ) ثابت
  {
    طالما (++شروع < len && گروہ (info[شروع - 1], info[شروع]))
      ;

    أرجع شروع;
  }

  ساكن منطقي cluster_گروہ_func (ثابت hb_glyph_info_t& a,
				   ثابت hb_glyph_info_t& b)
  { أرجع a.cluster == b.cluster; }

  فراغ الٹ_clusters () { الٹ_groups (cluster_گروہ_func); }

  HB_اندرونی فراغ guess_segment_properties ();

  HB_اندرونی منطقي متزامن ();
  HB_اندرونی صحيح sync_so_far ();
  HB_اندرونی فراغ clear_output ();
  HB_اندرونی فراغ clear_positions ();

  سانچا <قسم_ناں T>
  HB_NODISCARD منطقي replace_glyphs (بنانشانی صحيح عدد_وچ,
				    بنانشانی صحيح عدد_باہرلا,
				    ثابت T *glyph_انکڑے)
  {
    إذا (unlikely (!make_گنجائش_for (عدد_وچ, عدد_باہرلا))) أرجع خطأ;

    أكد (idx + عدد_وچ <= len);

    ملاؤ_clusters (idx, idx + عدد_وچ);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &باہرلا_info[باہرلا_len];
    لكل (بنانشانی صحيح i = 0; i < عدد_باہرلا; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_انکڑے[i];
      pinfo++;
    }

    idx  += عدد_وچ;
    باہرلا_len += عدد_باہرلا;
    أرجع سچ;
  }

  HB_NODISCARD منطقي replace_glyph (hb_codepoint_t glyph_اشاریہ)
  { أرجع replace_glyphs (1, 1, &glyph_اشاریہ); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD منطقي output_glyph (hb_codepoint_t glyph_اشاریہ)
  { أرجع replace_glyphs (0, 1, &glyph_اشاریہ); }

  HB_NODISCARD منطقي output_info (ثابت hb_glyph_info_t &glyph_info)
  {
    إذا (unlikely (!make_گنجائش_for (0, 1))) أرجع خطأ;

    باہرلا_info[باہرلا_len] = glyph_info;

    باہرلا_len++;
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
  HB_NODISCARD منطقي اگلا_glyph ()
  {
    إذا (have_output)
    {
      إذا (باہرلا_info != info || باہرلا_len != idx)
      {
	إذا (unlikely (!ensure (باہرلا_len + 1))) أرجع خطأ;
	باہرلا_info[باہرلا_len] = info[idx];
      }
      باہرلا_len++;
    }

    idx++;
    أرجع سچ;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD منطقي اگلا_glyphs (بنانشانی صحيح n)
  {
    إذا (have_output)
    {
      إذا (باہرلا_info != info || باہرلا_len != idx)
      {
	إذا (unlikely (!ensure (باہرلا_len + n))) أرجع خطأ;
	memmove (باہرلا_info + باہرلا_len, info + idx, n * حجم (باہرلا_info[0]));
      }
      باہرلا_len += n;
    }

    idx += n;
    أرجع سچ;
  }
  /* Advance idx without copying to output. */
  فراغ skip_glyph () { idx++; }
  فراغ جوڑو_masks (hb_نقاب_t نقاب)
  {
    لكل (بنانشانی صحيح j = 0; j < len; j++)
      info[j].نقاب |= نقاب;
  }
  HB_اندرونی فراغ سیٹ_masks (hb_نقاب_t value, hb_نقاب_t نقاب,
			      بنانشانی صحيح cluster_شروع, بنانشانی صحيح cluster_سرا);

  فراغ ملاؤ_clusters (بنانشانی صحيح شروع, بنانشانی صحيح سرا)
  {
    إذا (سرا - شروع < 2)
      أرجع;
    إذا (!HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_منزل_break (شروع, سرا);
      أرجع;
    }
    ملاؤ_clusters_impl (شروع, سرا);
  }
  فراغ ملاؤ_grapheme_clusters (بنانشانی صحيح شروع, بنانشانی صحيح سرا)
  {
    إذا (سرا - شروع < 2)
      أرجع;
    إذا (!HB_بفر_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_منزل_break (شروع, سرا);
      أرجع;
    }
    ملاؤ_clusters_impl (شروع, سرا);
  }
  HB_اندرونی فراغ ملاؤ_clusters_impl (بنانشانی صحيح شروع, بنانشانی صحيح سرا);
  فراغ ملاؤ_باہرلا_clusters (بنانشانی صحيح شروع, بنانشانی صحيح سرا)
  {
    إذا (سرا - شروع < 2)
      أرجع;
    إذا (!HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      أرجع;
    ملاؤ_باہرلا_clusters_impl (شروع, سرا);
  }
  فراغ ملاؤ_باہرلا_grapheme_clusters (بنانشانی صحيح شروع, بنانشانی صحيح سرا)
  {
    إذا (سرا - شروع < 2)
      أرجع;
    إذا (!HB_بفر_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      أرجع;
    ملاؤ_باہرلا_clusters_impl (شروع, سرا);
  }
  HB_اندرونی فراغ ملاؤ_باہرلا_clusters_impl (بنانشانی صحيح شروع, بنانشانی صحيح سرا);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_اندرونی فراغ delete_glyph ();
  HB_اندرونی فراغ delete_glyphs_inplace (منطقي (*filter) (ثابت hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  فراغ سیٹ_glyph_flags_impl (hb_نقاب_t نقاب,
			      بنانشانی شروع,
			      بنانشانی سرا,
			      منطقي interior,
			      منطقي from_باہرلا_بفر)
  {
    إذا (!from_باہرلا_بفر || !have_output)
    {
      إذا (!interior)
      {
	لكل (بنانشانی i = شروع; i < سرا; i++)
	  info[i].نقاب |= نقاب;
      }
      وإلا
      {
	بنانشانی cluster = infos_لبھو_min_cluster (info, شروع, سرا);
	infos_سیٹ_glyph_flags (info, شروع, سرا, cluster, نقاب);
      }
    }
    وإلا
    {
      أكد (شروع <= باہرلا_len);
      أكد (idx <= سرا);

      إذا (!interior)
      {
	لكل (بنانشانی i = شروع; i < باہرلا_len; i++)
	  باہرلا_info[i].نقاب |= نقاب;
	لكل (بنانشانی i = idx; i < سرا; i++)
	  info[i].نقاب |= نقاب;
      }
      وإلا
      {
	بنانشانی cluster = infos_لبھو_min_cluster (info, idx, سرا);
	cluster = infos_لبھو_min_cluster (باہرلا_info, شروع, باہرلا_len, cluster);

	infos_سیٹ_glyph_flags (باہرلا_info, شروع, باہرلا_len, cluster, نقاب);
	infos_سیٹ_glyph_flags (info, idx, سرا, cluster, نقاب);
      }
    }
  }

  HB_ALWAYS_INLINE
  فراغ سیٹ_glyph_flags (hb_نقاب_t نقاب,
			 بنانشانی شروع = 0,
			 بنانشانی سرا = (بنانشانی) -1,
			 منطقي interior = خطأ,
			 منطقي from_باہرلا_بفر = خطأ)
  {
    إذا (unlikely (سرا != (بنانشانی) -1 && سرا - شروع > 255))
      أرجع;

    سرا = hb_min (سرا, len);

    إذا (interior && !from_باہرلا_بفر && سرا - شروع < 2)
      أرجع;

    سیٹ_glyph_flags_impl (نقاب, شروع, سرا, interior, from_باہرلا_بفر);
  }


  فراغ unsafe_منزل_break (بنانشانی صحيح شروع = 0, بنانشانی صحيح سرا = -1)
  {
    سیٹ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_منزل_BREAK | HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT,
		      شروع, سرا,
		      سچ);
  }
  فراغ محفوظ_منزل_داخل_کرو_tatweel (بنانشانی صحيح شروع = 0, بنانشانی صحيح سرا = -1)
  {
    إذا ((flags & HB_بفر_FLAG_PRODUCE_محفوظ_منزل_داخل_کرو_TATWEEL) == 0)
    {
      unsafe_منزل_break (شروع, سرا);
      أرجع;
    }
    سیٹ_glyph_flags (HB_GLYPH_FLAG_محفوظ_منزل_داخل_کرو_TATWEEL,
		      شروع, سرا,
		      سچ);
  }
#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  فراغ unsafe_منزل_concat (بنانشانی صحيح شروع = 0, بنانشانی صحيح سرا = -1)
  {
    إذا (likely ((flags & HB_بفر_FLAG_PRODUCE_UNSAFE_منزل_CONCAT) == 0))
      أرجع;
    سیٹ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT,
		      شروع, سرا,
		      خطأ);
  }
  فراغ unsafe_منزل_break_from_outbuffer (بنانشانی صحيح شروع = 0, بنانشانی صحيح سرا = -1)
  {
    سیٹ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_منزل_BREAK | HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT,
		      شروع, سرا,
		      سچ, سچ);
  }
#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  فراغ unsafe_منزل_concat_from_outbuffer (بنانشانی صحيح شروع = 0, بنانشانی صحيح سرا = -1)
  {
    إذا (likely ((flags & HB_بفر_FLAG_PRODUCE_UNSAFE_منزل_CONCAT) == 0))
      أرجع;
    سیٹ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT,
		      شروع, سرا,
		      خطأ, سچ);
  }


  /* Internal methods */
  HB_NODISCARD HB_اندرونی منطقي move_منزل (بنانشانی صحيح i); /* i is output-buffer index. */

  HB_NODISCARD HB_اندرونی منطقي enlarge (بنانشانی صحيح ناپ);

  HB_NODISCARD منطقي resize (بنانشانی لمائی)
  {
    أكد (!have_output);
    إذا (unlikely (!ensure (لمائی))) أرجع خطأ;
    len = لمائی;
    أرجع سچ;
  }
  HB_NODISCARD منطقي ensure (بنانشانی صحيح ناپ)
  { أرجع likely (!ناپ || ناپ < allocated) ? سچ : enlarge (ناپ); }

  HB_NODISCARD منطقي ensure_inplace (بنانشانی صحيح ناپ)
  { أرجع likely (!ناپ || ناپ < allocated); }

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

  HB_NODISCARD HB_اندرونی منطقي make_گنجائش_for (بنانشانی صحيح عدد_وچ, بنانشانی صحيح عدد_باہرلا);
  HB_NODISCARD HB_اندرونی منطقي کھسکاؤ_forward (بنانشانی صحيح گنتی);

  تعريفنوع لما scratch_بفر_t;
  HB_اندرونی scratch_بفر_t *get_scratch_بفر (بنانشانی صحيح *ناپ);

  فراغ clear_context (بنانشانی صحيح side) { context_len[side] = 0; }

  HB_اندرونی فراغ sort (بنانشانی صحيح شروع, بنانشانی صحيح سرا, صحيح(*compar)(ثابت hb_glyph_info_t *, ثابت hb_glyph_info_t *));

  منطقي messaging ()
  {
#جے_تعریف_شدہ HB_NO_بفر_سنیہا
    أرجع خطأ;
#وإلا
    أرجع unlikely (سنیہا_func);
#جے_انت
  }
  منطقي سنیہا (hb_font_t *font, ثابت حرف *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#جے_تعریف_شدہ HB_NO_بفر_سنیہا
    أرجع سچ;
#وإلا
    إذا (likely (!messaging ()))
      أرجع سچ;

    va_list ap;
    va_شروع (ap, fmt);
    منطقي ret = سنیہا_impl (font, fmt, ap);
    va_سرا (ap);

    أرجع ret;
#جے_انت
  }
  HB_اندرونی منطقي سنیہا_impl (hb_font_t *font, ثابت حرف *fmt, va_list ap) HB_PRINTF_FUNC(3, 0);
  HB_اندرونی فراغ بدلیا ();

  ساكن فراغ
  سیٹ_cluster (hb_glyph_info_t &inf, بنانشانی صحيح cluster, بنانشانی صحيح نقاب = 0)
  {
    إذا (inf.cluster != cluster)
      inf.نقاب = (inf.نقاب & ~HB_GLYPH_FLAG_DEFINED) | (نقاب & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  فراغ
  infos_سیٹ_glyph_flags (hb_glyph_info_t *infos,
			  بنانشانی صحيح شروع, بنانشانی صحيح سرا,
			  بنانشانی صحيح cluster,
			  hb_نقاب_t نقاب)
  {
    إذا (unlikely (شروع == سرا))
      أرجع;

    max_ops -= سرا - شروع;
    إذا (unlikely (max_ops < 0))
      successful = خطأ;

    بنانشانی cluster_پہلا = infos[شروع].cluster;
    بنانشانی cluster_آخری = infos[سرا - 1].cluster;

    إذا (cluster_level == HB_بفر_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_پہلا && cluster != cluster_آخری))
    {
      لكل (بنانشانی صحيح i = شروع; i < سرا; i++)
	إذا (cluster != infos[i].cluster)
	  infos[i].نقاب |= نقاب;
      أرجع;
    }

    /* Monotone clusters */

    إذا (cluster == cluster_پہلا)
    {
      لكل (بنانشانی صحيح i = سرا; شروع < i && infos[i - 1].cluster != cluster_پہلا; i--)
	infos[i - 1].نقاب |= نقاب;
    }
    وإلا /* cluster == cluster_last */
    {
      لكل (بنانشانی صحيح i = شروع; i < سرا && infos[i].cluster != cluster_آخری; i++)
	infos[i].نقاب |= نقاب;
    }
  }
  بنانشانی
  infos_لبھو_min_cluster (ثابت hb_glyph_info_t *infos,
			   بنانشانی شروع, بنانشانی سرا,
			   بنانشانی cluster = UINT_MAX)
  {
    إذا (unlikely (شروع == سرا))
      أرجع cluster;

    إذا (cluster_level == HB_بفر_CLUSTER_LEVEL_CHARACTERS)
    {
      لكل (بنانشانی صحيح i = شروع; i < سرا; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      أرجع cluster;
    }

    أرجع hb_min (cluster, hb_min (infos[شروع].cluster, infos[سرا - 1].cluster));
  }

  فراغ clear_glyph_flags (hb_نقاب_t نقاب = 0)
  {
    لكل (بنانشانی صحيح i = 0; i < len; i++)
      info[i].نقاب = (info[i].نقاب & ~HB_GLYPH_FLAG_DEFINED) | (نقاب & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_بفر_t);


#تعریف_کرو foreach_گروہ(بفر, شروع, سرا, گروہ_func) \
  لكل (بنانشانی صحيح \
       گنتی_2 = بفر->len, \
       شروع = 0, سرا = گنتی_2 ? بفر->گروہ_سرا (0, گروہ_func) : 0; \
       شروع < گنتی_2; \
       شروع = سرا, سرا = بفر->گروہ_سرا (شروع, گروہ_func))

#تعریف_کرو foreach_cluster(بفر, شروع, سرا) \
	foreach_گروہ (بفر, شروع, سرا, hb_بفر_t::cluster_گروہ_func)


#تعریف_کرو HB_بفر_XALLOCATE_VAR(b, func, var) \
  b->func (offsetof (hb_glyph_info_t, var) - offsetof(hb_glyph_info_t, var1), \
	   حجم (b->info[0].var))
#تعریف_کرو HB_بفر_ALLOCATE_VAR(b, var)		HB_بفر_XALLOCATE_VAR (b, allocate_var,     var ())
#تعریف_کرو HB_بفر_TRY_ALLOCATE_VAR(b, var)	HB_بفر_XALLOCATE_VAR (b, try_allocate_var, var ())
#تعریف_کرو HB_بفر_DEALLOCATE_VAR(b, var)	HB_بفر_XALLOCATE_VAR (b, deallocate_var,   var ())
#تعریف_کرو HB_بفر_ASSERT_VAR(b, var)		HB_بفر_XALLOCATE_VAR (b, assert_var,       var ())


#جے_انت /* HB_BUFFER_HH */
