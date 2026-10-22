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

#که_نه_تعریف_شوی HB_بفر_HH
#تعریف_کړه HB_بفر_HH

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
  hb_object_سرلیک_t سرلیک;

  /*
   * Information about how the text in the buffer should be treated.
   */

  hb_unicode_funcs_t *unicode; /* Unicode functions */
  hb_بفر_flags_t flags; /* BOT / EOT / etc. */
  hb_بفر_cluster_level_t cluster_level;
  hb_codepoint_t replacement; /* U+FFFD or something else. */
  hb_codepoint_t invisible; /* 0 or something else. */
  hb_codepoint_t not_وموندل; /* 0 or something else. */
  hb_codepoint_t not_وموندل_variation_selector; /* HB_CODEPOINT_INVALID or something else. */

  /*
   * Buffer contents
   */

  hb_بفر_content_type_t content_type;
  hb_segment_properties_t props; /* Script, language, direction */

  منطقي successful; /* Allocations successful */
  منطقي have_output; /* Whether we have an output buffer going on */
  منطقي have_positions; /* Whether we have positions */

  بینښه صحيح idx; /* Cursor into ->info and ->pos arrays */
  بینښه صحيح len; /* Length of ->info and ->pos arrays */
  بینښه صحيح بهر_len; /* Length of ->out_info array if have_output */

  بینښه صحيح allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *بهر_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  ساكن constexpr بینښه CONTEXT_اوږدتیا = 5u;
  hb_codepoint_t context[2][CONTEXT_اوږدتیا];
  بینښه صحيح context_len[2];

  hb_سېټ_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_بټونه;
  uint8_t serial;
  uint32_t random_state;
  hb_بفر_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  بینښه صحيح max_len; /* Maximum allowed len. */
  صحيح max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#که_نه_تعریف_شوی HB_NO_بفر_پیغام
  تعريفنوع فراغ (*بدل_شوی_func_t) (hb_بفر_t *بفر, فراغ *user_معلومات);

  hb_بفر_پیغام_func_t پیغام_func;
  فراغ *پیغام_معلومات;
  hb_destroy_func_t پیغام_destroy;
  بدل_شوی_func_t بدل_شوی_func;
  فراغ *بدل_شوی_معلومات;
  بینښه پیغام_ژوروالی; /* How deeply are we inside a message callback? */
#وإلا
  ساكن constexpr بینښه پیغام_ژوروالی = 0u;
#د_که_پای



  /* Methods */

  HB_NODISCARD منطقي دننه_error () ثابت { أرجع !successful; }

  فراغ allocate_var (بینښه صحيح پیل, بینښه صحيح شمېر)
  {
    بینښه صحيح پای_ټکی = پیل + شمېر;
    أكد (پای_ټکی <= 8);
    بینښه صحيح بټونه = (1u<<پای_ټکی) - (1u<<پیل);
    أكد (0 == (allocated_var_بټونه & بټونه));
    allocated_var_بټونه |= بټونه;
  }
  منطقي try_allocate_var (بینښه صحيح پیل, بینښه صحيح شمېر)
  {
    بینښه صحيح پای_ټکی = پیل + شمېر;
    أكد (پای_ټکی <= 8);
    بینښه صحيح بټونه = (1u<<پای_ټکی) - (1u<<پیل);
    إذا (allocated_var_بټونه & بټونه)
      أرجع خطأ;
    allocated_var_بټونه |= بټونه;
    أرجع ریښتیا;
  }
  فراغ deallocate_var (بینښه صحيح پیل, بینښه صحيح شمېر)
  {
    بینښه صحيح پای_ټکی = پیل + شمېر;
    أكد (پای_ټکی <= 8);
    بینښه صحيح بټونه = (1u<<پای_ټکی) - (1u<<پیل);
    أكد (بټونه == (allocated_var_بټونه & بټونه));
    allocated_var_بټونه &= ~بټونه;
  }
  فراغ assert_var (بینښه صحيح پیل, بینښه صحيح شمېر)
  {
    بینښه صحيح پای_ټکی = پیل + شمېر;
    أكد (پای_ټکی <= 8);
    HB_ناکارول_شوی بینښه صحيح بټونه = (1u<<پای_ټکی) - (1u<<پیل);
    أكد (بټونه == (allocated_var_بټونه & بټونه));
  }
  فراغ deallocate_var_all ()
  {
    allocated_var_بټونه = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (بینښه صحيح i = 0) { أرجع info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (بینښه صحيح i = 0) ثابت { أرجع info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (بینښه صحيح i = 0) { أرجع pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (بینښه صحيح i = 0) ثابت { أرجع pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { أرجع بهر_info[بهر_len ? بهر_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () ثابت { أرجع بهر_info[بهر_len ? بهر_len - 1 : 0]; }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_codepoints (سېټ_t &d) ثابت
  { d.clear (); d.جمع_کړه_array (&info[0].codepoint, len, حجم (info[0])); }

  فراغ update_digest ()
  {
    digest = hb_سېټ_digest_t ();
    collect_codepoints (digest);
  }

  HB_دننی فراغ similar (ثابت hb_بفر_t &src);
  HB_دننی فراغ reset ();
  HB_دننی فراغ clear ();

  /* Called around shape() */
  HB_دننی فراغ enter ();
  HB_دننی فراغ leave ();

#که_نه_تعریف_شوی HB_NO_بفر_VERIFY
  HB_دننی
#د_که_پای
  منطقي verify (hb_بفر_t        *متن_بفر,
	       hb_font_t          *font,
	       ثابت hb_feature_t *features,
	       بینښه صحيح        عدد_features,
	       ثابت حرف * ثابت *shapers)
#که_نه_تعریف_شوی HB_NO_بفر_VERIFY
  ;
#وإلا
  { أرجع ریښتیا; }
#د_که_پای

  بینښه صحيح backtrack_len () ثابت { أرجع have_output ? بهر_len : idx; }
  بینښه صحيح lookahead_len () ثابت { أرجع len - idx; }
  uint8_t بل_serial () { أرجع ++serial ? serial : ++serial; }

  HB_دننی فراغ جمع_کړه (hb_codepoint_t  codepoint,
			بینښه صحيح    cluster);
  HB_دننی فراغ جمع_کړه_info (ثابت hb_glyph_info_t &glyph_info);
  HB_دننی فراغ جمع_کړه_info_and_pos (ثابت hb_glyph_info_t &glyph_info,
				     ثابت hb_glyph_position_t &glyph_pos);

  فراغ سرچپه_range (بینښه پیل, بینښه پای_ټکی)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (پیل, پای_ټکی);
    إذا (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (پیل, پای_ټکی);
  }
  فراغ reverse () { سرچپه_range (0, len); }

  کينډۍ <ډول_نوم FuncType>
  فراغ سرچپه_groups (ثابت FuncType& ډله,
		       منطقي یوځای_clusters = خطأ)
  {
    إذا (unlikely (!len))
      أرجع;

    بینښه پیل = 0;
    بینښه i;
    لكل (i = 1; i < len; i++)
    {
      إذا (!ډله (info[i - 1], info[i]))
      {
	إذا (یوځای_clusters)
	  هذا->یوځای_clusters (پیل, i);
	سرچپه_range (پیل, i);
	پیل = i;
      }
    }
    إذا (یوځای_clusters)
      هذا->یوځای_clusters (پیل, i);
    سرچپه_range (پیل, i);

    reverse ();
  }

  کينډۍ <ډول_نوم FuncType>
  بینښه ډله_پای_ټکی (بینښه پیل, ثابت FuncType& ډله) ثابت
  {
    طالما (++پیل < len && ډله (info[پیل - 1], info[پیل]))
      ;

    أرجع پیل;
  }

  ساكن منطقي cluster_ډله_func (ثابت hb_glyph_info_t& a,
				   ثابت hb_glyph_info_t& b)
  { أرجع a.cluster == b.cluster; }

  فراغ سرچپه_clusters () { سرچپه_groups (cluster_ډله_func); }

  HB_دننی فراغ guess_segment_properties ();

  HB_دننی منطقي متزامن ();
  HB_دننی صحيح sync_so_far ();
  HB_دننی فراغ clear_output ();
  HB_دننی فراغ clear_positions ();

  کينډۍ <ډول_نوم T>
  HB_NODISCARD منطقي replace_glyphs (بینښه صحيح عدد_دننه,
				    بینښه صحيح عدد_بهر,
				    ثابت T *glyph_معلومات)
  {
    إذا (unlikely (!make_ځای_for (عدد_دننه, عدد_بهر))) أرجع خطأ;

    أكد (idx + عدد_دننه <= len);

    یوځای_clusters (idx, idx + عدد_دننه);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &بهر_info[بهر_len];
    لكل (بینښه صحيح i = 0; i < عدد_بهر; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_معلومات[i];
      pinfo++;
    }

    idx  += عدد_دننه;
    بهر_len += عدد_بهر;
    أرجع ریښتیا;
  }

  HB_NODISCARD منطقي replace_glyph (hb_codepoint_t glyph_شاخص)
  { أرجع replace_glyphs (1, 1, &glyph_شاخص); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD منطقي output_glyph (hb_codepoint_t glyph_شاخص)
  { أرجع replace_glyphs (0, 1, &glyph_شاخص); }

  HB_NODISCARD منطقي output_info (ثابت hb_glyph_info_t &glyph_info)
  {
    إذا (unlikely (!make_ځای_for (0, 1))) أرجع خطأ;

    بهر_info[بهر_len] = glyph_info;

    بهر_len++;
    أرجع ریښتیا;
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
  HB_NODISCARD منطقي بل_glyph ()
  {
    إذا (have_output)
    {
      إذا (بهر_info != info || بهر_len != idx)
      {
	إذا (unlikely (!ensure (بهر_len + 1))) أرجع خطأ;
	بهر_info[بهر_len] = info[idx];
      }
      بهر_len++;
    }

    idx++;
    أرجع ریښتیا;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD منطقي بل_glyphs (بینښه صحيح n)
  {
    إذا (have_output)
    {
      إذا (بهر_info != info || بهر_len != idx)
      {
	إذا (unlikely (!ensure (بهر_len + n))) أرجع خطأ;
	memmove (بهر_info + بهر_len, info + idx, n * حجم (بهر_info[0]));
      }
      بهر_len += n;
    }

    idx += n;
    أرجع ریښتیا;
  }
  /* Advance idx without copying to output. */
  فراغ skip_glyph () { idx++; }
  فراغ جمع_کړه_masks (hb_ماسک_t ماسک)
  {
    لكل (بینښه صحيح j = 0; j < len; j++)
      info[j].ماسک |= ماسک;
  }
  HB_دننی فراغ سېټ_masks (hb_ماسک_t value, hb_ماسک_t ماسک,
			      بینښه صحيح cluster_پیل, بینښه صحيح cluster_پای_ټکی);

  فراغ یوځای_clusters (بینښه صحيح پیل, بینښه صحيح پای_ټکی)
  {
    إذا (پای_ټکی - پیل < 2)
      أرجع;
    إذا (!HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_منزل_break (پیل, پای_ټکی);
      أرجع;
    }
    یوځای_clusters_impl (پیل, پای_ټکی);
  }
  فراغ یوځای_grapheme_clusters (بینښه صحيح پیل, بینښه صحيح پای_ټکی)
  {
    إذا (پای_ټکی - پیل < 2)
      أرجع;
    إذا (!HB_بفر_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_منزل_break (پیل, پای_ټکی);
      أرجع;
    }
    یوځای_clusters_impl (پیل, پای_ټکی);
  }
  HB_دننی فراغ یوځای_clusters_impl (بینښه صحيح پیل, بینښه صحيح پای_ټکی);
  فراغ یوځای_بهر_clusters (بینښه صحيح پیل, بینښه صحيح پای_ټکی)
  {
    إذا (پای_ټکی - پیل < 2)
      أرجع;
    إذا (!HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      أرجع;
    یوځای_بهر_clusters_impl (پیل, پای_ټکی);
  }
  فراغ یوځای_بهر_grapheme_clusters (بینښه صحيح پیل, بینښه صحيح پای_ټکی)
  {
    إذا (پای_ټکی - پیل < 2)
      أرجع;
    إذا (!HB_بفر_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      أرجع;
    یوځای_بهر_clusters_impl (پیل, پای_ټکی);
  }
  HB_دننی فراغ یوځای_بهر_clusters_impl (بینښه صحيح پیل, بینښه صحيح پای_ټکی);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_دننی فراغ delete_glyph ();
  HB_دننی فراغ delete_glyphs_inplace (منطقي (*filter) (ثابت hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  فراغ سېټ_glyph_flags_impl (hb_ماسک_t ماسک,
			      بینښه پیل,
			      بینښه پای_ټکی,
			      منطقي interior,
			      منطقي from_بهر_بفر)
  {
    إذا (!from_بهر_بفر || !have_output)
    {
      إذا (!interior)
      {
	لكل (بینښه i = پیل; i < پای_ټکی; i++)
	  info[i].ماسک |= ماسک;
      }
      وإلا
      {
	بینښه cluster = infos_پیدا_کړه_min_cluster (info, پیل, پای_ټکی);
	infos_سېټ_glyph_flags (info, پیل, پای_ټکی, cluster, ماسک);
      }
    }
    وإلا
    {
      أكد (پیل <= بهر_len);
      أكد (idx <= پای_ټکی);

      إذا (!interior)
      {
	لكل (بینښه i = پیل; i < بهر_len; i++)
	  بهر_info[i].ماسک |= ماسک;
	لكل (بینښه i = idx; i < پای_ټکی; i++)
	  info[i].ماسک |= ماسک;
      }
      وإلا
      {
	بینښه cluster = infos_پیدا_کړه_min_cluster (info, idx, پای_ټکی);
	cluster = infos_پیدا_کړه_min_cluster (بهر_info, پیل, بهر_len, cluster);

	infos_سېټ_glyph_flags (بهر_info, پیل, بهر_len, cluster, ماسک);
	infos_سېټ_glyph_flags (info, idx, پای_ټکی, cluster, ماسک);
      }
    }
  }

  HB_ALWAYS_INLINE
  فراغ سېټ_glyph_flags (hb_ماسک_t ماسک,
			 بینښه پیل = 0,
			 بینښه پای_ټکی = (بینښه) -1,
			 منطقي interior = خطأ,
			 منطقي from_بهر_بفر = خطأ)
  {
    إذا (unlikely (پای_ټکی != (بینښه) -1 && پای_ټکی - پیل > 255))
      أرجع;

    پای_ټکی = hb_min (پای_ټکی, len);

    إذا (interior && !from_بهر_بفر && پای_ټکی - پیل < 2)
      أرجع;

    سېټ_glyph_flags_impl (ماسک, پیل, پای_ټکی, interior, from_بهر_بفر);
  }


  فراغ unsafe_منزل_break (بینښه صحيح پیل = 0, بینښه صحيح پای_ټکی = -1)
  {
    سېټ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_منزل_BREAK | HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT,
		      پیل, پای_ټکی,
		      ریښتیا);
  }
  فراغ خوندي_منزل_ورننباسه_tatweel (بینښه صحيح پیل = 0, بینښه صحيح پای_ټکی = -1)
  {
    إذا ((flags & HB_بفر_FLAG_PRODUCE_خوندي_منزل_ورننباسه_TATWEEL) == 0)
    {
      unsafe_منزل_break (پیل, پای_ټکی);
      أرجع;
    }
    سېټ_glyph_flags (HB_GLYPH_FLAG_خوندي_منزل_ورننباسه_TATWEEL,
		      پیل, پای_ټکی,
		      ریښتیا);
  }
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  فراغ unsafe_منزل_concat (بینښه صحيح پیل = 0, بینښه صحيح پای_ټکی = -1)
  {
    إذا (likely ((flags & HB_بفر_FLAG_PRODUCE_UNSAFE_منزل_CONCAT) == 0))
      أرجع;
    سېټ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT,
		      پیل, پای_ټکی,
		      خطأ);
  }
  فراغ unsafe_منزل_break_from_outbuffer (بینښه صحيح پیل = 0, بینښه صحيح پای_ټکی = -1)
  {
    سېټ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_منزل_BREAK | HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT,
		      پیل, پای_ټکی,
		      ریښتیا, ریښتیا);
  }
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  فراغ unsafe_منزل_concat_from_outbuffer (بینښه صحيح پیل = 0, بینښه صحيح پای_ټکی = -1)
  {
    إذا (likely ((flags & HB_بفر_FLAG_PRODUCE_UNSAFE_منزل_CONCAT) == 0))
      أرجع;
    سېټ_glyph_flags (HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT,
		      پیل, پای_ټکی,
		      خطأ, ریښتیا);
  }


  /* Internal methods */
  HB_NODISCARD HB_دننی منطقي move_منزل (بینښه صحيح i); /* i is output-buffer index. */

  HB_NODISCARD HB_دننی منطقي enlarge (بینښه صحيح اندازه);

  HB_NODISCARD منطقي resize (بینښه اوږدتیا)
  {
    أكد (!have_output);
    إذا (unlikely (!ensure (اوږدتیا))) أرجع خطأ;
    len = اوږدتیا;
    أرجع ریښتیا;
  }
  HB_NODISCARD منطقي ensure (بینښه صحيح اندازه)
  { أرجع likely (!اندازه || اندازه < allocated) ? ریښتیا : enlarge (اندازه); }

  HB_NODISCARD منطقي ensure_inplace (بینښه صحيح اندازه)
  { أرجع likely (!اندازه || اندازه < allocated); }

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
    أرجع ریښتیا;
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
    أرجع ریښتیا;
  }

  HB_NODISCARD HB_دننی منطقي make_ځای_for (بینښه صحيح عدد_دننه, بینښه صحيح عدد_بهر);
  HB_NODISCARD HB_دننی منطقي ښويدنه_forward (بینښه صحيح شمېر);

  تعريفنوع اوږد scratch_بفر_t;
  HB_دننی scratch_بفر_t *get_scratch_بفر (بینښه صحيح *اندازه);

  فراغ clear_context (بینښه صحيح side) { context_len[side] = 0; }

  HB_دننی فراغ sort (بینښه صحيح پیل, بینښه صحيح پای_ټکی, صحيح(*compar)(ثابت hb_glyph_info_t *, ثابت hb_glyph_info_t *));

  منطقي messaging ()
  {
#که_تعریف_شوی HB_NO_بفر_پیغام
    أرجع خطأ;
#وإلا
    أرجع unlikely (پیغام_func);
#د_که_پای
  }
  منطقي پیغام (hb_font_t *font, ثابت حرف *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#که_تعریف_شوی HB_NO_بفر_پیغام
    أرجع ریښتیا;
#وإلا
    إذا (likely (!messaging ()))
      أرجع ریښتیا;

    va_list ap;
    va_پیل (ap, fmt);
    منطقي ret = پیغام_impl (font, fmt, ap);
    va_پای_ټکی (ap);

    أرجع ret;
#د_که_پای
  }
  HB_دننی منطقي پیغام_impl (hb_font_t *font, ثابت حرف *fmt, va_list ap) HB_PRINTF_FUNC(3, 0);
  HB_دننی فراغ بدل_شوی ();

  ساكن فراغ
  سېټ_cluster (hb_glyph_info_t &inf, بینښه صحيح cluster, بینښه صحيح ماسک = 0)
  {
    إذا (inf.cluster != cluster)
      inf.ماسک = (inf.ماسک & ~HB_GLYPH_FLAG_DEFINED) | (ماسک & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  فراغ
  infos_سېټ_glyph_flags (hb_glyph_info_t *infos,
			  بینښه صحيح پیل, بینښه صحيح پای_ټکی,
			  بینښه صحيح cluster,
			  hb_ماسک_t ماسک)
  {
    إذا (unlikely (پیل == پای_ټکی))
      أرجع;

    max_ops -= پای_ټکی - پیل;
    إذا (unlikely (max_ops < 0))
      successful = خطأ;

    بینښه cluster_لومړی = infos[پیل].cluster;
    بینښه cluster_وروستی = infos[پای_ټکی - 1].cluster;

    إذا (cluster_level == HB_بفر_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_لومړی && cluster != cluster_وروستی))
    {
      لكل (بینښه صحيح i = پیل; i < پای_ټکی; i++)
	إذا (cluster != infos[i].cluster)
	  infos[i].ماسک |= ماسک;
      أرجع;
    }

    /* Monotone clusters */

    إذا (cluster == cluster_لومړی)
    {
      لكل (بینښه صحيح i = پای_ټکی; پیل < i && infos[i - 1].cluster != cluster_لومړی; i--)
	infos[i - 1].ماسک |= ماسک;
    }
    وإلا /* cluster == cluster_last */
    {
      لكل (بینښه صحيح i = پیل; i < پای_ټکی && infos[i].cluster != cluster_وروستی; i++)
	infos[i].ماسک |= ماسک;
    }
  }
  بینښه
  infos_پیدا_کړه_min_cluster (ثابت hb_glyph_info_t *infos,
			   بینښه پیل, بینښه پای_ټکی,
			   بینښه cluster = UINT_MAX)
  {
    إذا (unlikely (پیل == پای_ټکی))
      أرجع cluster;

    إذا (cluster_level == HB_بفر_CLUSTER_LEVEL_CHARACTERS)
    {
      لكل (بینښه صحيح i = پیل; i < پای_ټکی; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      أرجع cluster;
    }

    أرجع hb_min (cluster, hb_min (infos[پیل].cluster, infos[پای_ټکی - 1].cluster));
  }

  فراغ clear_glyph_flags (hb_ماسک_t ماسک = 0)
  {
    لكل (بینښه صحيح i = 0; i < len; i++)
      info[i].ماسک = (info[i].ماسک & ~HB_GLYPH_FLAG_DEFINED) | (ماسک & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_بفر_t);


#تعریف_کړه foreach_ډله(بفر, پیل, پای_ټکی, ډله_func) \
  لكل (بینښه صحيح \
       شمېر_2 = بفر->len, \
       پیل = 0, پای_ټکی = شمېر_2 ? بفر->ډله_پای_ټکی (0, ډله_func) : 0; \
       پیل < شمېر_2; \
       پیل = پای_ټکی, پای_ټکی = بفر->ډله_پای_ټکی (پیل, ډله_func))

#تعریف_کړه foreach_cluster(بفر, پیل, پای_ټکی) \
	foreach_ډله (بفر, پیل, پای_ټکی, hb_بفر_t::cluster_ډله_func)


#تعریف_کړه HB_بفر_XALLOCATE_VAR(b, func, var) \
  b->func (offsetof (hb_glyph_info_t, var) - offsetof(hb_glyph_info_t, var1), \
	   حجم (b->info[0].var))
#تعریف_کړه HB_بفر_ALLOCATE_VAR(b, var)		HB_بفر_XALLOCATE_VAR (b, allocate_var,     var ())
#تعریف_کړه HB_بفر_TRY_ALLOCATE_VAR(b, var)	HB_بفر_XALLOCATE_VAR (b, try_allocate_var, var ())
#تعریف_کړه HB_بفر_DEALLOCATE_VAR(b, var)	HB_بفر_XALLOCATE_VAR (b, deallocate_var,   var ())
#تعریف_کړه HB_بفر_ASSERT_VAR(b, var)		HB_بفر_XALLOCATE_VAR (b, assert_var,       var ())


#د_که_پای /* HB_BUFFER_HH */
