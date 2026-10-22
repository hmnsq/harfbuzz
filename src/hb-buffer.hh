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

#నిర్వచితం_కాకపోతే HB_బఫరు_HH
#నిర్వచించు HB_బఫరు_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-unicode.hh"
#అన్తర్భావ "hb-set-digest.hh"


static_assert ((పరిమాణం (hb_glyph_info_t) == 20), "");
static_assert ((పరిమాణం (hb_glyph_info_t) == పరిమాణం (hb_glyph_position_t)), "");

HB_MARK_AS_FLAG_T (hb_glyph_flags_t);
HB_MARK_AS_FLAG_T (hb_బఫరు_flags_t);
HB_MARK_AS_FLAG_T (hb_బఫరు_serialize_flags_t);
HB_MARK_AS_FLAG_T (hb_బఫరు_diff_flags_t);

గణన hb_బఫరు_scratch_flags_t {
  HB_బఫరు_SCRATCH_FLAG_DEFAULT			= 0x00000000u,
  HB_బఫరు_SCRATCH_FLAG_HAS_FRACTION_SLASH		= 0x00000001u,
  HB_బఫరు_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES		= 0x00000002u,
  HB_బఫరు_SCRATCH_FLAG_HAS_SPACE_FALLBACK		= 0x00000004u,
  HB_బఫరు_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT		= 0x00000008u,
  HB_బఫరు_SCRATCH_FLAG_HAS_CGJ			= 0x00000010u,
  HB_బఫరు_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE		= 0x00000020u,
  HB_బఫరు_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK= 0x00000040u,
  HB_బఫరు_SCRATCH_FLAG_HAS_CONTINUATIONS		= 0x00000080u,

  /* Reserved for shapers' internal use. */
  HB_బఫరు_SCRATCH_FLAG_SHAPER0			= 0x01000000u,
  HB_బఫరు_SCRATCH_FLAG_SHAPER1			= 0x02000000u,
  HB_బఫరు_SCRATCH_FLAG_SHAPER2			= 0x04000000u,
  HB_బఫరు_SCRATCH_FLAG_SHAPER3			= 0x08000000u,
};
HB_MARK_AS_FLAG_T (hb_బఫరు_scratch_flags_t);


/*
 * hb_buffer_t
 */

నిర్మాణం hb_బఫరు_t
{
  hb_object_శీర్షిక_t శీర్షిక;

  /*
   * Information about how the text in the buffer should be treated.
   */

  hb_unicode_funcs_t *unicode; /* Unicode functions */
  hb_బఫరు_flags_t flags; /* BOT / EOT / etc. */
  hb_బఫరు_cluster_level_t cluster_level;
  hb_codepoint_t replacement; /* U+FFFD or something else. */
  hb_codepoint_t invisible; /* 0 or something else. */
  hb_codepoint_t not_దొరికింది; /* 0 or something else. */
  hb_codepoint_t not_దొరికింది_variation_selector; /* HB_CODEPOINT_INVALID or something else. */

  /*
   * Buffer contents
   */

  hb_బఫరు_content_type_t content_type;
  hb_segment_properties_t props; /* Script, language, direction */

  తార్కిక successful; /* Allocations successful */
  తార్కిక have_output; /* Whether we have an output buffer going on */
  తార్కిక have_positions; /* Whether we have positions */

  గుర్తులేని పూర్ణాంకం idx; /* Cursor into ->info and ->pos arrays */
  గుర్తులేని పూర్ణాంకం len; /* Length of ->info and ->pos arrays */
  గుర్తులేని పూర్ణాంకం బహిర్గతం_len; /* Length of ->out_info array if have_output */

  గుర్తులేని పూర్ణాంకం allocated; /* Length of allocated arrays */
  hb_glyph_info_t     *info;
  hb_glyph_info_t     *బహిర్గతం_info;
  hb_glyph_position_t *pos;

  /* Text before / after the main buffer contents.
   * Always in Unicode, and ordered outward.
   * Index 0 is for "pre-context", 1 for "post-context". */
  స్థిర constexpr గుర్తులేని CONTEXT_నిడివి = 5u;
  hb_codepoint_t context[2][CONTEXT_నిడివి];
  గుర్తులేని పూర్ణాంకం context_len[2];

  hb_సమితి_digest_t digest; /* Manually updated sometimes */

  /*
   * Managed by enter / leave
   */

  uint8_t allocated_var_బిట్లు;
  uint8_t serial;
  uint32_t random_state;
  hb_బఫరు_scratch_flags_t scratch_flags; /* Have space-fallback, etc. */
  గుర్తులేని పూర్ణాంకం max_len; /* Maximum allowed len. */
  పూర్ణాంకం max_ops; /* Maximum allowed operations. */
  /* The bits here reflect current allocations of the bytes in glyph_info_t's var1 and var2. */


  /*
   * Messaging callback
   */

#నిర్వచితం_కాకపోతే HB_NO_బఫరు_సందేశం
  రకనిర్వచనం శూన్య (*మారింది_func_t) (hb_బఫరు_t *బఫరు, శూన్య *user_దత్తాంశం);

  hb_బఫరు_సందేశం_func_t సందేశం_func;
  శూన్య *సందేశం_దత్తాంశం;
  hb_destroy_func_t సందేశం_destroy;
  మారింది_func_t మారింది_func;
  శూన్య *మారింది_దత్తాంశం;
  గుర్తులేని సందేశం_లోతు; /* How deeply are we inside a message callback? */
#లేకపోతే
  స్థిర constexpr గుర్తులేని సందేశం_లోతు = 0u;
#అయితే_ముగింపు



  /* Methods */

  HB_NODISCARD తార్కిక లోపల_error () స్థిరరాశి { తిరిగి !successful; }

  శూన్య allocate_var (గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం లెక్క)
  {
    గుర్తులేని పూర్ణాంకం చివర = ప్రారంభం + లెక్క;
    ప్రతిజ్ఞా (చివర <= 8);
    గుర్తులేని పూర్ణాంకం బిట్లు = (1u<<చివర) - (1u<<ప్రారంభం);
    ప్రతిజ్ఞా (0 == (allocated_var_బిట్లు & బిట్లు));
    allocated_var_బిట్లు |= బిట్లు;
  }
  తార్కిక try_allocate_var (గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం లెక్క)
  {
    గుర్తులేని పూర్ణాంకం చివర = ప్రారంభం + లెక్క;
    ప్రతిజ్ఞా (చివర <= 8);
    గుర్తులేని పూర్ణాంకం బిట్లు = (1u<<చివర) - (1u<<ప్రారంభం);
    ఒకవేళ (allocated_var_బిట్లు & బిట్లు)
      తిరిగి అబద్ధం;
    allocated_var_బిట్లు |= బిట్లు;
    తిరిగి నిజం;
  }
  శూన్య deallocate_var (గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం లెక్క)
  {
    గుర్తులేని పూర్ణాంకం చివర = ప్రారంభం + లెక్క;
    ప్రతిజ్ఞా (చివర <= 8);
    గుర్తులేని పూర్ణాంకం బిట్లు = (1u<<చివర) - (1u<<ప్రారంభం);
    ప్రతిజ్ఞా (బిట్లు == (allocated_var_బిట్లు & బిట్లు));
    allocated_var_బిట్లు &= ~బిట్లు;
  }
  శూన్య assert_var (గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం లెక్క)
  {
    గుర్తులేని పూర్ణాంకం చివర = ప్రారంభం + లెక్క;
    ప్రతిజ్ఞా (చివర <= 8);
    HB_వాడని గుర్తులేని పూర్ణాంకం బిట్లు = (1u<<చివర) - (1u<<ప్రారంభం);
    ప్రతిజ్ఞా (బిట్లు == (allocated_var_బిట్లు & బిట్లు));
  }
  శూన్య deallocate_var_all ()
  {
    allocated_var_బిట్లు = 0;
  }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &cur (గుర్తులేని పూర్ణాంకం i = 0) { తిరిగి info[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t cur (గుర్తులేని పూర్ణాంకం i = 0) స్థిరరాశి { తిరిగి info[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_position_t &cur_pos (గుర్తులేని పూర్ణాంకం i = 0) { తిరిగి pos[idx + i]; }
  HB_ALWAYS_INLINE
  hb_glyph_position_t cur_pos (గుర్తులేని పూర్ణాంకం i = 0) స్థిరరాశి { తిరిగి pos[idx + i]; }

  HB_ALWAYS_INLINE
  hb_glyph_info_t &prev ()      { తిరిగి బహిర్గతం_info[బహిర్గతం_len ? బహిర్గతం_len - 1 : 0]; }
  HB_ALWAYS_INLINE
  hb_glyph_info_t prev () స్థిరరాశి { తిరిగి బహిర్గతం_info[బహిర్గతం_len ? బహిర్గతం_len - 1 : 0]; }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_codepoints (సమితి_t &d) స్థిరరాశి
  { d.clear (); d.కూడు_array (&info[0].codepoint, len, పరిమాణం (info[0])); }

  శూన్య update_digest ()
  {
    digest = hb_సమితి_digest_t ();
    collect_codepoints (digest);
  }

  HB_అంతర్గత శూన్య similar (స్థిరరాశి hb_బఫరు_t &src);
  HB_అంతర్గత శూన్య reset ();
  HB_అంతర్గత శూన్య clear ();

  /* Called around shape() */
  HB_అంతర్గత శూన్య enter ();
  HB_అంతర్గత శూన్య leave ();

#నిర్వచితం_కాకపోతే HB_NO_బఫరు_VERIFY
  HB_అంతర్గత
#అయితే_ముగింపు
  తార్కిక verify (hb_బఫరు_t        *పాఠ్యం_బఫరు,
	       hb_font_t          *font,
	       స్థిరరాశి hb_feature_t *features,
	       గుర్తులేని పూర్ణాంకం        అంకె_features,
	       స్థిరరాశి అక్షరం * స్థిరరాశి *shapers)
#నిర్వచితం_కాకపోతే HB_NO_బఫరు_VERIFY
  ;
#లేకపోతే
  { తిరిగి నిజం; }
#అయితే_ముగింపు

  గుర్తులేని పూర్ణాంకం backtrack_len () స్థిరరాశి { తిరిగి have_output ? బహిర్గతం_len : idx; }
  గుర్తులేని పూర్ణాంకం lookahead_len () స్థిరరాశి { తిరిగి len - idx; }
  uint8_t తదుపరి_serial () { తిరిగి ++serial ? serial : ++serial; }

  HB_అంతర్గత శూన్య కూడు (hb_codepoint_t  codepoint,
			గుర్తులేని పూర్ణాంకం    cluster);
  HB_అంతర్గత శూన్య కూడు_info (స్థిరరాశి hb_glyph_info_t &glyph_info);
  HB_అంతర్గత శూన్య కూడు_info_and_pos (స్థిరరాశి hb_glyph_info_t &glyph_info,
				     స్థిరరాశి hb_glyph_position_t &glyph_pos);

  శూన్య తిరగేయి_range (గుర్తులేని ప్రారంభం, గుర్తులేని చివర)
  {
    hb_array_t<hb_glyph_info_t> (info, len).reverse (ప్రారంభం, చివర);
    ఒకవేళ (have_positions)
      hb_array_t<hb_glyph_position_t> (pos, len).reverse (ప్రారంభం, చివర);
  }
  శూన్య reverse () { తిరగేయి_range (0, len); }

  మూస <రకం_పేరు FuncType>
  శూన్య తిరగేయి_groups (స్థిరరాశి FuncType& సమూహం,
		       తార్కిక విలీనం_clusters = అబద్ధం)
  {
    ఒకవేళ (unlikely (!len))
      తిరిగి;

    గుర్తులేని ప్రారంభం = 0;
    గుర్తులేని i;
    కొరకు (i = 1; i < len; i++)
    {
      ఒకవేళ (!సమూహం (info[i - 1], info[i]))
      {
	ఒకవేళ (విలీనం_clusters)
	  ఇది->విలీనం_clusters (ప్రారంభం, i);
	తిరగేయి_range (ప్రారంభం, i);
	ప్రారంభం = i;
      }
    }
    ఒకవేళ (విలీనం_clusters)
      ఇది->విలీనం_clusters (ప్రారంభం, i);
    తిరగేయి_range (ప్రారంభం, i);

    reverse ();
  }

  మూస <రకం_పేరు FuncType>
  గుర్తులేని సమూహం_చివర (గుర్తులేని ప్రారంభం, స్థిరరాశి FuncType& సమూహం) స్థిరరాశి
  {
    అయితే (++ప్రారంభం < len && సమూహం (info[ప్రారంభం - 1], info[ప్రారంభం]))
      ;

    తిరిగి ప్రారంభం;
  }

  స్థిర తార్కిక cluster_సమూహం_func (స్థిరరాశి hb_glyph_info_t& a,
				   స్థిరరాశి hb_glyph_info_t& b)
  { తిరిగి a.cluster == b.cluster; }

  శూన్య తిరగేయి_clusters () { తిరగేయి_groups (cluster_సమూహం_func); }

  HB_అంతర్గత శూన్య guess_segment_properties ();

  HB_అంతర్గత తార్కిక సమకాలీకరణ ();
  HB_అంతర్గత పూర్ణాంకం sync_so_far ();
  HB_అంతర్గత శూన్య clear_output ();
  HB_అంతర్గత శూన్య clear_positions ();

  మూస <రకం_పేరు T>
  HB_NODISCARD తార్కిక replace_glyphs (గుర్తులేని పూర్ణాంకం అంకె_లోపల,
				    గుర్తులేని పూర్ణాంకం అంకె_బహిర్గతం,
				    స్థిరరాశి T *glyph_దత్తాంశం)
  {
    ఒకవేళ (unlikely (!make_స్థలం_for (అంకె_లోపల, అంకె_బహిర్గతం))) తిరిగి అబద్ధం;

    ప్రతిజ్ఞా (idx + అంకె_లోపల <= len);

    విలీనం_clusters (idx, idx + అంకె_లోపల);

    hb_glyph_info_t &orig_info = idx < len ? cur() : prev();

    hb_glyph_info_t *pinfo = &బహిర్గతం_info[బహిర్గతం_len];
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_బహిర్గతం; i++)
    {
      *pinfo = orig_info;
      pinfo->codepoint = glyph_దత్తాంశం[i];
      pinfo++;
    }

    idx  += అంకె_లోపల;
    బహిర్గతం_len += అంకె_బహిర్గతం;
    తిరిగి నిజం;
  }

  HB_NODISCARD తార్కిక replace_glyph (hb_codepoint_t glyph_సూచీ)
  { తిరిగి replace_glyphs (1, 1, &glyph_సూచీ); }

  /* Makes a copy of the glyph at idx to output and replace glyph_index */
  HB_NODISCARD తార్కిక output_glyph (hb_codepoint_t glyph_సూచీ)
  { తిరిగి replace_glyphs (0, 1, &glyph_సూచీ); }

  HB_NODISCARD తార్కిక output_info (స్థిరరాశి hb_glyph_info_t &glyph_info)
  {
    ఒకవేళ (unlikely (!make_స్థలం_for (0, 1))) తిరిగి అబద్ధం;

    బహిర్గతం_info[బహిర్గతం_len] = glyph_info;

    బహిర్గతం_len++;
    తిరిగి నిజం;
  }
  /* Copies glyph at idx to output but doesn't advance idx */
  HB_NODISCARD తార్కిక copy_glyph ()
  {
    /* Extra copy because cur()'s return can be freed within
     * output_info() call if buffer reallocates. */
    తిరిగి output_info (hb_glyph_info_t (cur()));
  }

  /* Copies glyph at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD తార్కిక తదుపరి_glyph ()
  {
    ఒకవేళ (have_output)
    {
      ఒకవేళ (బహిర్గతం_info != info || బహిర్గతం_len != idx)
      {
	ఒకవేళ (unlikely (!ensure (బహిర్గతం_len + 1))) తిరిగి అబద్ధం;
	బహిర్గతం_info[బహిర్గతం_len] = info[idx];
      }
      బహిర్గతం_len++;
    }

    idx++;
    తిరిగి నిజం;
  }
  /* Copies n glyphs at idx to output and advance idx.
   * If there's no output, just advance idx. */
  HB_NODISCARD తార్కిక తదుపరి_glyphs (గుర్తులేని పూర్ణాంకం n)
  {
    ఒకవేళ (have_output)
    {
      ఒకవేళ (బహిర్గతం_info != info || బహిర్గతం_len != idx)
      {
	ఒకవేళ (unlikely (!ensure (బహిర్గతం_len + n))) తిరిగి అబద్ధం;
	memmove (బహిర్గతం_info + బహిర్గతం_len, info + idx, n * పరిమాణం (బహిర్గతం_info[0]));
      }
      బహిర్గతం_len += n;
    }

    idx += n;
    తిరిగి నిజం;
  }
  /* Advance idx without copying to output. */
  శూన్య skip_glyph () { idx++; }
  శూన్య కూడు_masks (hb_ముసుగు_t ముసుగు)
  {
    కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < len; j++)
      info[j].ముసుగు |= ముసుగు;
  }
  HB_అంతర్గత శూన్య సమితి_masks (hb_ముసుగు_t value, hb_ముసుగు_t ముసుగు,
			      గుర్తులేని పూర్ణాంకం cluster_ప్రారంభం, గుర్తులేని పూర్ణాంకం cluster_చివర);

  శూన్య విలీనం_clusters (గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం చివర)
  {
    ఒకవేళ (చివర - ప్రారంభం < 2)
      తిరిగి;
    ఒకవేళ (!HB_బఫరు_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
    {
      unsafe_గమ్యం_break (ప్రారంభం, చివర);
      తిరిగి;
    }
    విలీనం_clusters_impl (ప్రారంభం, చివర);
  }
  శూన్య విలీనం_grapheme_clusters (గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం చివర)
  {
    ఒకవేళ (చివర - ప్రారంభం < 2)
      తిరిగి;
    ఒకవేళ (!HB_బఫరు_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
    {
      unsafe_గమ్యం_break (ప్రారంభం, చివర);
      తిరిగి;
    }
    విలీనం_clusters_impl (ప్రారంభం, చివర);
  }
  HB_అంతర్గత శూన్య విలీనం_clusters_impl (గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం చివర);
  శూన్య విలీనం_బహిర్గతం_clusters (గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం చివర)
  {
    ఒకవేళ (చివర - ప్రారంభం < 2)
      తిరిగి;
    ఒకవేళ (!HB_బఫరు_CLUSTER_LEVEL_IS_MONOTONE (cluster_level))
      తిరిగి;
    విలీనం_బహిర్గతం_clusters_impl (ప్రారంభం, చివర);
  }
  శూన్య విలీనం_బహిర్గతం_grapheme_clusters (గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం చివర)
  {
    ఒకవేళ (చివర - ప్రారంభం < 2)
      తిరిగి;
    ఒకవేళ (!HB_బఫరు_CLUSTER_LEVEL_IS_GRAPHEMES (cluster_level))
      తిరిగి;
    విలీనం_బహిర్గతం_clusters_impl (ప్రారంభం, చివర);
  }
  HB_అంతర్గత శూన్య విలీనం_బహిర్గతం_clusters_impl (గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం చివర);
  /* Merge clusters for deleting current glyph, and skip it. */
  HB_అంతర్గత శూన్య delete_glyph ();
  HB_అంతర్గత శూన్య delete_glyphs_inplace (తార్కిక (*filter) (స్థిరరాశి hb_glyph_info_t *info));



  /* Adds glyph flags in mask to infos with clusters between start and end.
   * The start index will be from out-buffer if from_out_buffer is true.
   * If interior is true, then the cluster having the minimum value is skipped. */
  శూన్య సమితి_glyph_flags_impl (hb_ముసుగు_t ముసుగు,
			      గుర్తులేని ప్రారంభం,
			      గుర్తులేని చివర,
			      తార్కిక interior,
			      తార్కిక from_బహిర్గతం_బఫరు)
  {
    ఒకవేళ (!from_బహిర్గతం_బఫరు || !have_output)
    {
      ఒకవేళ (!interior)
      {
	కొరకు (గుర్తులేని i = ప్రారంభం; i < చివర; i++)
	  info[i].ముసుగు |= ముసుగు;
      }
      లేకపోతే
      {
	గుర్తులేని cluster = infos_కనుగొను_min_cluster (info, ప్రారంభం, చివర);
	infos_సమితి_glyph_flags (info, ప్రారంభం, చివర, cluster, ముసుగు);
      }
    }
    లేకపోతే
    {
      ప్రతిజ్ఞా (ప్రారంభం <= బహిర్గతం_len);
      ప్రతిజ్ఞా (idx <= చివర);

      ఒకవేళ (!interior)
      {
	కొరకు (గుర్తులేని i = ప్రారంభం; i < బహిర్గతం_len; i++)
	  బహిర్గతం_info[i].ముసుగు |= ముసుగు;
	కొరకు (గుర్తులేని i = idx; i < చివర; i++)
	  info[i].ముసుగు |= ముసుగు;
      }
      లేకపోతే
      {
	గుర్తులేని cluster = infos_కనుగొను_min_cluster (info, idx, చివర);
	cluster = infos_కనుగొను_min_cluster (బహిర్గతం_info, ప్రారంభం, బహిర్గతం_len, cluster);

	infos_సమితి_glyph_flags (బహిర్గతం_info, ప్రారంభం, బహిర్గతం_len, cluster, ముసుగు);
	infos_సమితి_glyph_flags (info, idx, చివర, cluster, ముసుగు);
      }
    }
  }

  HB_ALWAYS_INLINE
  శూన్య సమితి_glyph_flags (hb_ముసుగు_t ముసుగు,
			 గుర్తులేని ప్రారంభం = 0,
			 గుర్తులేని చివర = (గుర్తులేని) -1,
			 తార్కిక interior = అబద్ధం,
			 తార్కిక from_బహిర్గతం_బఫరు = అబద్ధం)
  {
    ఒకవేళ (unlikely (చివర != (గుర్తులేని) -1 && చివర - ప్రారంభం > 255))
      తిరిగి;

    చివర = hb_min (చివర, len);

    ఒకవేళ (interior && !from_బహిర్గతం_బఫరు && చివర - ప్రారంభం < 2)
      తిరిగి;

    సమితి_glyph_flags_impl (ముసుగు, ప్రారంభం, చివర, interior, from_బహిర్గతం_బఫరు);
  }


  శూన్య unsafe_గమ్యం_break (గుర్తులేని పూర్ణాంకం ప్రారంభం = 0, గుర్తులేని పూర్ణాంకం చివర = -1)
  {
    సమితి_glyph_flags (HB_GLYPH_FLAG_UNSAFE_గమ్యం_BREAK | HB_GLYPH_FLAG_UNSAFE_గమ్యం_CONCAT,
		      ప్రారంభం, చివర,
		      నిజం);
  }
  శూన్య సురక్షిత_గమ్యం_చొప్పించు_tatweel (గుర్తులేని పూర్ణాంకం ప్రారంభం = 0, గుర్తులేని పూర్ణాంకం చివర = -1)
  {
    ఒకవేళ ((flags & HB_బఫరు_FLAG_PRODUCE_సురక్షిత_గమ్యం_చొప్పించు_TATWEEL) == 0)
    {
      unsafe_గమ్యం_break (ప్రారంభం, చివర);
      తిరిగి;
    }
    సమితి_glyph_flags (HB_GLYPH_FLAG_సురక్షిత_గమ్యం_చొప్పించు_TATWEEL,
		      ప్రారంభం, చివర,
		      నిజం);
  }
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  శూన్య unsafe_గమ్యం_concat (గుర్తులేని పూర్ణాంకం ప్రారంభం = 0, గుర్తులేని పూర్ణాంకం చివర = -1)
  {
    ఒకవేళ (likely ((flags & HB_బఫరు_FLAG_PRODUCE_UNSAFE_గమ్యం_CONCAT) == 0))
      తిరిగి;
    సమితి_glyph_flags (HB_GLYPH_FLAG_UNSAFE_గమ్యం_CONCAT,
		      ప్రారంభం, చివర,
		      అబద్ధం);
  }
  శూన్య unsafe_గమ్యం_break_from_outbuffer (గుర్తులేని పూర్ణాంకం ప్రారంభం = 0, గుర్తులేని పూర్ణాంకం చివర = -1)
  {
    సమితి_glyph_flags (HB_GLYPH_FLAG_UNSAFE_గమ్యం_BREAK | HB_GLYPH_FLAG_UNSAFE_గమ్యం_CONCAT,
		      ప్రారంభం, చివర,
		      నిజం, నిజం);
  }
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  శూన్య unsafe_గమ్యం_concat_from_outbuffer (గుర్తులేని పూర్ణాంకం ప్రారంభం = 0, గుర్తులేని పూర్ణాంకం చివర = -1)
  {
    ఒకవేళ (likely ((flags & HB_బఫరు_FLAG_PRODUCE_UNSAFE_గమ్యం_CONCAT) == 0))
      తిరిగి;
    సమితి_glyph_flags (HB_GLYPH_FLAG_UNSAFE_గమ్యం_CONCAT,
		      ప్రారంభం, చివర,
		      అబద్ధం, నిజం);
  }


  /* Internal methods */
  HB_NODISCARD HB_అంతర్గత తార్కిక move_గమ్యం (గుర్తులేని పూర్ణాంకం i); /* i is output-buffer index. */

  HB_NODISCARD HB_అంతర్గత తార్కిక enlarge (గుర్తులేని పూర్ణాంకం కొలత);

  HB_NODISCARD తార్కిక resize (గుర్తులేని నిడివి)
  {
    ప్రతిజ్ఞా (!have_output);
    ఒకవేళ (unlikely (!ensure (నిడివి))) తిరిగి అబద్ధం;
    len = నిడివి;
    తిరిగి నిజం;
  }
  HB_NODISCARD తార్కిక ensure (గుర్తులేని పూర్ణాంకం కొలత)
  { తిరిగి likely (!కొలత || కొలత < allocated) ? నిజం : enlarge (కొలత); }

  HB_NODISCARD తార్కిక ensure_inplace (గుర్తులేని పూర్ణాంకం కొలత)
  { తిరిగి likely (!కొలత || కొలత < allocated); }

  శూన్య assert_glyphs ()
  {
    ప్రతిజ్ఞా ((content_type == HB_బఫరు_CONTENT_TYPE_GLYPHS) ||
	    (!len && (content_type == HB_బఫరు_CONTENT_TYPE_INVALID)));
  }
  శూన్య assert_unicode ()
  {
    ప్రతిజ్ఞా ((content_type == HB_బఫరు_CONTENT_TYPE_UNICODE) ||
	    (!len && (content_type == HB_బఫరు_CONTENT_TYPE_INVALID)));
  }
  HB_NODISCARD తార్కిక ensure_glyphs ()
  {
    ఒకవేళ (unlikely (content_type != HB_బఫరు_CONTENT_TYPE_GLYPHS))
    {
      ఒకవేళ (content_type != HB_బఫరు_CONTENT_TYPE_INVALID)
	తిరిగి అబద్ధం;
      ప్రతిజ్ఞా (len == 0);
      content_type = HB_బఫరు_CONTENT_TYPE_GLYPHS;
    }
    తిరిగి నిజం;
  }
  HB_NODISCARD తార్కిక ensure_unicode ()
  {
    ఒకవేళ (unlikely (content_type != HB_బఫరు_CONTENT_TYPE_UNICODE))
    {
      ఒకవేళ (content_type != HB_బఫరు_CONTENT_TYPE_INVALID)
	తిరిగి అబద్ధం;
      ప్రతిజ్ఞా (len == 0);
      content_type = HB_బఫరు_CONTENT_TYPE_UNICODE;
    }
    తిరిగి నిజం;
  }

  HB_NODISCARD HB_అంతర్గత తార్కిక make_స్థలం_for (గుర్తులేని పూర్ణాంకం అంకె_లోపల, గుర్తులేని పూర్ణాంకం అంకె_బహిర్గతం);
  HB_NODISCARD HB_అంతర్గత తార్కిక జరుపు_forward (గుర్తులేని పూర్ణాంకం లెక్క);

  రకనిర్వచనం దీర్ఘ scratch_బఫరు_t;
  HB_అంతర్గత scratch_బఫరు_t *get_scratch_బఫరు (గుర్తులేని పూర్ణాంకం *కొలత);

  శూన్య clear_context (గుర్తులేని పూర్ణాంకం side) { context_len[side] = 0; }

  HB_అంతర్గత శూన్య sort (గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం చివర, పూర్ణాంకం(*compar)(స్థిరరాశి hb_glyph_info_t *, స్థిరరాశి hb_glyph_info_t *));

  తార్కిక messaging ()
  {
#నిర్వచితమైతే HB_NO_బఫరు_సందేశం
    తిరిగి అబద్ధం;
#లేకపోతే
    తిరిగి unlikely (సందేశం_func);
#అయితే_ముగింపు
  }
  తార్కిక సందేశం (hb_font_t *font, స్థిరరాశి అక్షరం *fmt, ...) HB_PRINTF_FUNC(3, 4)
  {
#నిర్వచితమైతే HB_NO_బఫరు_సందేశం
    తిరిగి నిజం;
#లేకపోతే
    ఒకవేళ (likely (!messaging ()))
      తిరిగి నిజం;

    va_list ap;
    va_ప్రారంభం (ap, fmt);
    తార్కిక ret = సందేశం_impl (font, fmt, ap);
    va_చివర (ap);

    తిరిగి ret;
#అయితే_ముగింపు
  }
  HB_అంతర్గత తార్కిక సందేశం_impl (hb_font_t *font, స్థిరరాశి అక్షరం *fmt, va_list ap) HB_PRINTF_FUNC(3, 0);
  HB_అంతర్గత శూన్య మారింది ();

  స్థిర శూన్య
  సమితి_cluster (hb_glyph_info_t &inf, గుర్తులేని పూర్ణాంకం cluster, గుర్తులేని పూర్ణాంకం ముసుగు = 0)
  {
    ఒకవేళ (inf.cluster != cluster)
      inf.ముసుగు = (inf.ముసుగు & ~HB_GLYPH_FLAG_DEFINED) | (ముసుగు & HB_GLYPH_FLAG_DEFINED);
    inf.cluster = cluster;
  }
  శూన్య
  infos_సమితి_glyph_flags (hb_glyph_info_t *infos,
			  గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం చివర,
			  గుర్తులేని పూర్ణాంకం cluster,
			  hb_ముసుగు_t ముసుగు)
  {
    ఒకవేళ (unlikely (ప్రారంభం == చివర))
      తిరిగి;

    max_ops -= చివర - ప్రారంభం;
    ఒకవేళ (unlikely (max_ops < 0))
      successful = అబద్ధం;

    గుర్తులేని cluster_మొదటి = infos[ప్రారంభం].cluster;
    గుర్తులేని cluster_చివరి = infos[చివర - 1].cluster;

    ఒకవేళ (cluster_level == HB_బఫరు_CLUSTER_LEVEL_CHARACTERS ||
	(cluster != cluster_మొదటి && cluster != cluster_చివరి))
    {
      కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
	ఒకవేళ (cluster != infos[i].cluster)
	  infos[i].ముసుగు |= ముసుగు;
      తిరిగి;
    }

    /* Monotone clusters */

    ఒకవేళ (cluster == cluster_మొదటి)
    {
      కొరకు (గుర్తులేని పూర్ణాంకం i = చివర; ప్రారంభం < i && infos[i - 1].cluster != cluster_మొదటి; i--)
	infos[i - 1].ముసుగు |= ముసుగు;
    }
    లేకపోతే /* cluster == cluster_last */
    {
      కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర && infos[i].cluster != cluster_చివరి; i++)
	infos[i].ముసుగు |= ముసుగు;
    }
  }
  గుర్తులేని
  infos_కనుగొను_min_cluster (స్థిరరాశి hb_glyph_info_t *infos,
			   గుర్తులేని ప్రారంభం, గుర్తులేని చివర,
			   గుర్తులేని cluster = UINT_MAX)
  {
    ఒకవేళ (unlikely (ప్రారంభం == చివర))
      తిరిగి cluster;

    ఒకవేళ (cluster_level == HB_బఫరు_CLUSTER_LEVEL_CHARACTERS)
    {
      కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
	cluster = hb_min (cluster, infos[i].cluster);
      తిరిగి cluster;
    }

    తిరిగి hb_min (cluster, hb_min (infos[ప్రారంభం].cluster, infos[చివర - 1].cluster));
  }

  శూన్య clear_glyph_flags (hb_ముసుగు_t ముసుగు = 0)
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < len; i++)
      info[i].ముసుగు = (info[i].ముసుగు & ~HB_GLYPH_FLAG_DEFINED) | (ముసుగు & HB_GLYPH_FLAG_DEFINED);
  }
};
DECLARE_NULL_INSTANCE (hb_బఫరు_t);


#నిర్వచించు foreach_సమూహం(బఫరు, ప్రారంభం, చివర, సమూహం_func) \
  కొరకు (గుర్తులేని పూర్ణాంకం \
       లెక్క_2 = బఫరు->len, \
       ప్రారంభం = 0, చివర = లెక్క_2 ? బఫరు->సమూహం_చివర (0, సమూహం_func) : 0; \
       ప్రారంభం < లెక్క_2; \
       ప్రారంభం = చివర, చివర = బఫరు->సమూహం_చివర (ప్రారంభం, సమూహం_func))

#నిర్వచించు foreach_cluster(బఫరు, ప్రారంభం, చివర) \
	foreach_సమూహం (బఫరు, ప్రారంభం, చివర, hb_బఫరు_t::cluster_సమూహం_func)


#నిర్వచించు HB_బఫరు_XALLOCATE_VAR(b, func, var) \
  b->func (offsetof (hb_glyph_info_t, var) - offsetof(hb_glyph_info_t, var1), \
	   పరిమాణం (b->info[0].var))
#నిర్వచించు HB_బఫరు_ALLOCATE_VAR(b, var)		HB_బఫరు_XALLOCATE_VAR (b, allocate_var,     var ())
#నిర్వచించు HB_బఫరు_TRY_ALLOCATE_VAR(b, var)	HB_బఫరు_XALLOCATE_VAR (b, try_allocate_var, var ())
#నిర్వచించు HB_బఫరు_DEALLOCATE_VAR(b, var)	HB_బఫరు_XALLOCATE_VAR (b, deallocate_var,   var ())
#నిర్వచించు HB_బఫరు_ASSERT_VAR(b, var)		HB_బఫరు_XALLOCATE_VAR (b, assert_var,       var ())


#అయితే_ముగింపు /* HB_BUFFER_HH */
