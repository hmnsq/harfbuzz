/*
 * Copyright © 2007,2008,2009,2010  Red Hat, Inc.
 * Copyright © 2010,2012  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_OT_LAYOUT_GSUBGPOS_HH
#నిర్వచించు HB_OT_LAYOUT_GSUBGPOS_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-buffer.hh"
#అన్తర్భావ "hb-map.hh"
#అన్తర్భావ "hb-set.hh"
#అన్తర్భావ "hb-ot-map.hh"
#అన్తర్భావ "hb-ot-layout-common.hh"
#అన్తర్భావ "hb-ot-layout-gdef-table.hh"
#అన్తర్భావ "hb-depend-data.hh"


నామాకాశ OT {


నిర్మాణం hb_intersects_context_t :
       hb_dispatch_context_t<hb_intersects_context_t, తార్కిక>
{
  మూస <రకం_పేరు T>
  return_t dispatch (స్థిరరాశి T &obj) { తిరిగి obj.intersects (ఇది->glyphs); }
  స్థిర return_t default_return_విలువ () { తిరిగి అబద్ధం; }
  తార్కిక stop_sublookup_iteration (return_t r) స్థిరరాశి { తిరిగి r; }

  స్థిరరాశి hb_సమితి_t *glyphs;

  hb_intersects_context_t (స్థిరరాశి hb_సమితి_t *glyphs_) :
                            glyphs (glyphs_) {}
};

నిర్మాణం hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, తార్కిక>
{
  మూస <రకం_పేరు T>
  return_t dispatch (స్థిరరాశి T &obj) { తిరిగి obj.may_have_non_1to1 (); }
  స్థిర return_t default_return_విలువ () { తిరిగి అబద్ధం; }
  తార్కిక stop_sublookup_iteration (return_t r) స్థిరరాశి { తిరిగి r; }
};

నిర్మాణం hb_సంవృతం_context_t :
       hb_dispatch_context_t<hb_సంవృతం_context_t>
{
  రకనిర్వచనం return_t (*recurse_func_t) (hb_సంవృతం_context_t *c, గుర్తులేని lookup_సూచీ, hb_సమితి_t *covered_seq_indicies, గుర్తులేని seq_సూచీ, గుర్తులేని చివర_సూచీ);
  మూస <రకం_పేరు T>
  return_t dispatch (స్థిరరాశి T &obj) { obj.సంవృతం (ఇది); తిరిగి hb_ఖాళీలు_t (); }
  స్థిర return_t default_return_విలువ () { తిరిగి hb_ఖాళీలు_t (); }
  శూన్య recurse (గుర్తులేని lookup_సూచీ, hb_సమితి_t *covered_seq_indicies, గుర్తులేని seq_సూచీ, గుర్తులేని చివర_సూచీ)
  {
    ఒకవేళ (unlikely (nesting_level_ఎడమ == 0 || !recurse_func))
      తిరిగి;

    nesting_level_ఎడమ--;
    recurse_func (ఇది, lookup_సూచీ, covered_seq_indicies, seq_సూచీ, చివర_సూచీ);
    nesting_level_ఎడమ++;
  }

  శూన్య reset_lookup_సందర్శించు_లెక్క ()
  { lookup_లెక్క = 0; }

  తార్కిక lookup_హద్దు_exceeded ()
  { తిరిగి lookup_లెక్క > HB_MAX_LOOKUP_సందర్శించు_లెక్క; }

  తార్కిక should_సందర్శించు_lookup (గుర్తులేని పూర్ణాంకం lookup_సూచీ)
  {
    ఒకవేళ (lookup_లెక్క++ > HB_MAX_LOOKUP_సందర్శించు_లెక్క)
      తిరిగి అబద్ధం;

    ఒకవేళ (is_lookup_పూర్తి (lookup_సూచీ))
      తిరిగి అబద్ధం;

    తిరిగి నిజం;
  }

  తార్కిక is_lookup_పూర్తి (గుర్తులేని పూర్ణాంకం lookup_సూచీ)
  {
    ఒకవేళ (unlikely (పూర్తి_lookups_glyph_లెక్క->లోపల_error () ||
		  పూర్తి_lookups_glyph_సమితి->లోపల_error ()))
      తిరిగి నిజం;

    /* Have we visited this lookup with the current set of glyphs? */
    గుర్తులేని తీయి = glyphs->get_population ();
    ఒకవేళ (unlikely (తీయి == HB_MAP_విలువ_INVALID)) // Avoid sentinel interference.
      తీయి--;
    ఒకవేళ (పూర్తి_lookups_glyph_లెక్క->get (lookup_సూచీ) != తీయి)
    {
      పూర్తి_lookups_glyph_లెక్క->సమితి (lookup_సూచీ, తీయి);

      ఒకవేళ (!పూర్తి_lookups_glyph_సమితి->has (lookup_సూచీ))
      {
	ఒకవేళ (unlikely (!పూర్తి_lookups_glyph_సమితి->సమితి (lookup_సూచీ, hb::unique_ptr<hb_సమితి_t> {hb_సమితి_create ()})))
	  తిరిగి నిజం;
      }

      పూర్తి_lookups_glyph_సమితి->get (lookup_సూచీ)->clear ();
    }

    hb_సమితి_t *covered_glyph_సమితి = పూర్తి_lookups_glyph_సమితి->get (lookup_సూచీ);
    ఒకవేళ (unlikely (covered_glyph_సమితి->లోపల_error ()))
      తిరిగి నిజం;
    ఒకవేళ (మాతృ_చురుకైన_glyphs ().is_subset (*covered_glyph_సమితి))
      తిరిగి నిజం;

    covered_glyph_సమితి->union_ (మాతృ_చురుకైన_glyphs ());
    తిరిగి అబద్ధం;
  }

  స్థిరరాశి hb_సమితి_t& మునుపటి_మాతృ_చురుకైన_glyphs () {
    ఒకవేళ (చురుకైన_glyphs_దొంతర.నిడివి <= 1)
      తిరిగి *glyphs;

    తిరిగి చురుకైన_glyphs_దొంతర[చురుకైన_glyphs_దొంతర.నిడివి - 2];
  }

  స్థిరరాశి hb_సమితి_t& మాతృ_చురుకైన_glyphs ()
  {
    ఒకవేళ (!చురుకైన_glyphs_దొంతర)
      తిరిగి *glyphs;

    తిరిగి చురుకైన_glyphs_దొంతర.తోక ();
  }

  hb_సమితి_t* నెట్టు_cur_చురుకైన_glyphs ()
  {
    hb_సమితి_t *s = చురుకైన_glyphs_దొంతర.push ();
    ఒకవేళ (unlikely (చురుకైన_glyphs_దొంతర.లోపల_error ()))
      తిరిగి శూన్యసూచిక;
    తిరిగి s;
  }

  తార్కిక తీయి_cur_పూర్తి_glyphs ()
  {
    ఒకవేళ (!చురుకైన_glyphs_దొంతర)
      తిరిగి అబద్ధం;

    చురుకైన_glyphs_దొంతర.తీయి ();
    తిరిగి నిజం;
  }

  hb_face_t *face;
  hb_సమితి_t *glyphs;
  hb_సమితి_t output[1];
  hb_సదిశ_t<hb_సమితి_t> చురుకైన_glyphs_దొంతర;
  recurse_func_t recurse_func = శూన్యసూచిక;
  గుర్తులేని పూర్ణాంకం nesting_level_ఎడమ;

  hb_సంవృతం_context_t (hb_face_t *face_,
			hb_సమితి_t *glyphs_,
			hb_map_t *పూర్తి_lookups_glyph_లెక్క_2,
			hb_hashmap_t<గుర్తులేని, hb::unique_ptr<hb_సమితి_t>> *పూర్తి_lookups_glyph_సమితి_2,
			గుర్తులేని పూర్ణాంకం nesting_level_ఎడమ_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_ఎడమ (nesting_level_ఎడమ_2),
			  పూర్తి_lookups_glyph_లెక్క (పూర్తి_lookups_glyph_లెక్క_2),
			  పూర్తి_lookups_glyph_సమితి (పూర్తి_lookups_glyph_సమితి_2)
  {}

  ~hb_సంవృతం_context_t () { flush (); }

  శూన్య సమితి_recurse_func (recurse_func_t func) { recurse_func = func; }

  శూన్య flush ()
  {
    output->del_range (face->get_అంకె_glyphs (), HB_సమితి_విలువ_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->clear ();
    చురుకైన_glyphs_దొంతర.తీయి ();
    చురుకైన_glyphs_దొంతర.reset ();
  }

  అంతరంగ:
  hb_map_t *పూర్తి_lookups_glyph_లెక్క;
  hb_hashmap_t<గుర్తులేని, hb::unique_ptr<hb_సమితి_t>> *పూర్తి_lookups_glyph_సమితి;
  గుర్తులేని పూర్ణాంకం lookup_లెక్క = 0;
};



నిర్మాణం hb_సంవృతం_lookups_context_t :
       hb_dispatch_context_t<hb_సంవృతం_lookups_context_t>
{
  రకనిర్వచనం return_t (*recurse_func_t) (hb_సంవృతం_lookups_context_t *c, గుర్తులేని lookup_సూచీ);
  మూస <రకం_పేరు T>
  return_t dispatch (స్థిరరాశి T &obj) { obj.సంవృతం_lookups (ఇది); తిరిగి hb_ఖాళీలు_t (); }
  స్థిర return_t default_return_విలువ () { తిరిగి hb_ఖాళీలు_t (); }
  శూన్య recurse (గుర్తులేని lookup_సూచీ)
  {
    ఒకవేళ (unlikely (nesting_level_ఎడమ == 0 || !recurse_func))
      తిరిగి;

    /* Return if new lookup was recursed to before. */
    ఒకవేళ (lookup_హద్దు_exceeded ()
        || సందర్శించిన_lookups->లోపల_error ()
        || సందర్శించిన_lookups->has (lookup_సూచీ))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      తిరిగి;

    nesting_level_ఎడమ--;
    recurse_func (ఇది, lookup_సూచీ);
    nesting_level_ఎడమ++;
  }

  శూన్య సమితి_lookup_సందర్శించిన (గుర్తులేని lookup_సూచీ)
  { సందర్శించిన_lookups->కూడు (lookup_సూచీ); }

  శూన్య సమితి_lookup_inactive (గుర్తులేని lookup_సూచీ)
  { inactive_lookups->కూడు (lookup_సూచీ); }

  తార్కిక lookup_హద్దు_exceeded ()
  {
    తార్కిక ret = lookup_లెక్క > HB_MAX_LOOKUP_సందర్శించు_లెక్క;
    ఒకవేళ (ret)
      DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "lookup visit count limit exceeded in lookup closure!");
    తిరిగి ret; }

  తార్కిక is_lookup_సందర్శించిన (గుర్తులేని lookup_సూచీ)
  {
    ఒకవేళ (unlikely (lookup_లెక్క++ > HB_MAX_LOOKUP_సందర్శించు_లెక్క))
    {
      DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_లెక్క, lookup_సూచీ);
      తిరిగి నిజం;
    }

    ఒకవేళ (unlikely (సందర్శించిన_lookups->లోపల_error ()))
      తిరిగి నిజం;

    తిరిగి సందర్శించిన_lookups->has (lookup_సూచీ);
  }

  hb_face_t *face;
  స్థిరరాశి hb_సమితి_t *glyphs;
  recurse_func_t recurse_func;
  గుర్తులేని పూర్ణాంకం nesting_level_ఎడమ;

  hb_సంవృతం_lookups_context_t (hb_face_t *face_,
				స్థిరరాశి hb_సమితి_t *glyphs_,
				hb_సమితి_t *సందర్శించిన_lookups_2,
				hb_సమితి_t *inactive_lookups_,
				గుర్తులేని nesting_level_ఎడమ_2 = HB_MAX_NESTING_LEVEL) :
				face (face_),
				glyphs (glyphs_),
				recurse_func (శూన్యసూచిక),
				nesting_level_ఎడమ (nesting_level_ఎడమ_2),
				సందర్శించిన_lookups (సందర్శించిన_lookups_2),
				inactive_lookups (inactive_lookups_),
				lookup_లెక్క (0) {}

  శూన్య సమితి_recurse_func (recurse_func_t func) { recurse_func = func; }

  అంతరంగ:
  hb_సమితి_t *సందర్శించిన_lookups;
  hb_సమితి_t *inactive_lookups;
  గుర్తులేని పూర్ణాంకం lookup_లెక్క;
};

నిర్మాణం hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, తార్కిక>
{
  మూస <రకం_పేరు T>
  return_t dispatch (స్థిరరాశి T &obj) { తిరిగి obj.would_apply (ఇది); }
  స్థిర return_t default_return_విలువ () { తిరిగి అబద్ధం; }
  తార్కిక stop_sublookup_iteration (return_t r) స్థిరరాశి { తిరిగి r; }

  hb_face_t *face;
  స్థిరరాశి hb_codepoint_t *glyphs;
  గుర్తులేని పూర్ణాంకం len;
  తార్కిక zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    స్థిరరాశి hb_codepoint_t *glyphs_,
			    గుర్తులేని పూర్ణాంకం len_,
			    తార్కిక zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

నిర్మాణం hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  రకనిర్వచనం return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, గుర్తులేని పూర్ణాంకం lookup_సూచీ);
  మూస <రకం_పేరు T>
  return_t dispatch (స్థిరరాశి T &obj) { obj.collect_glyphs (ఇది); తిరిగి hb_ఖాళీలు_t (); }
  స్థిర return_t default_return_విలువ () { తిరిగి hb_ఖాళీలు_t (); }
  శూన్య recurse (గుర్తులేని పూర్ణాంకం lookup_సూచీ)
  {
    ఒకవేళ (unlikely (nesting_level_ఎడమ == 0 || !recurse_func))
      తిరిగి;

    /* Note that GPOS sets recurse_func to nullptr already, so it doesn't get
     * past the previous check.  For GSUB, we only want to collect the output
     * glyphs in the recursion.  If output is not requested, we can go home now.
     *
     * Note further, that the above is not exactly correct.  A recursed lookup
     * is allowed to match input that is not matched in the context, but that's
     * not how most fonts are built.  It's possible to relax that and recurse
     * with all sets here if it proves to be an issue.
     */

    ఒకవేళ (output == hb_సమితి_get_ఖాళీలు ())
      తిరిగి;

    /* Return if new lookup was recursed to before. */
    ఒకవేళ (recursed_lookups->has (lookup_సూచీ))
      తిరిగి;

    hb_సమితి_t *old_before = before;
    hb_సమితి_t *old_ఆదానం  = ఆదానం;
    hb_సమితి_t *old_after  = after;
    before = ఆదానం = after = hb_సమితి_get_ఖాళీలు ();

    nesting_level_ఎడమ--;
    recurse_func (ఇది, lookup_సూచీ);
    nesting_level_ఎడమ++;

    before = old_before;
    ఆదానం  = old_ఆదానం;
    after  = old_after;

    recursed_lookups->కూడు (lookup_సూచీ);
  }

  hb_face_t *face;
  hb_సమితి_t *before;
  hb_సమితి_t *ఆదానం;
  hb_సమితి_t *after;
  hb_సమితి_t *output;
  recurse_func_t recurse_func;
  hb_సమితి_t *recursed_lookups;
  గుర్తులేని పూర్ణాంకం nesting_level_ఎడమ;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_సమితి_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_సమితి_t  *glyphs_ఆదానం,  /* OUT.  May be NULL */
			       hb_సమితి_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_సమితి_t  *glyphs_output, /* OUT.  May be NULL */
			       గుర్తులేని పూర్ణాంకం nesting_level_ఎడమ_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_సమితి_get_ఖాళీలు ()),
			      ఆదానం  (glyphs_ఆదానం  ? glyphs_ఆదానం  : hb_సమితి_get_ఖాళీలు ()),
			      after  (glyphs_after  ? glyphs_after  : hb_సమితి_get_ఖాళీలు ()),
			      output (glyphs_output ? glyphs_output : hb_సమితి_get_ఖాళీలు ()),
			      recurse_func (శూన్యసూచిక),
			      recursed_lookups (hb_సమితి_create ()),
			      nesting_level_ఎడమ (nesting_level_ఎడమ_2) {}
  ~hb_collect_glyphs_context_t () { hb_సమితి_destroy (recursed_lookups); }

  శూన్య సమితి_recurse_func (recurse_func_t func) { recurse_func = func; }
};

నిర్మాణం hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  గణన glyph_సమితి_type_t
  {
    GLYPH_సమితి_CLASS,
    GLYPH_సమితి_COVERAGE,
    GLYPH_సమితి_SINGLE
  };

  స్థిర constexpr hb_codepoint_t SINGLE_GLYPH_చురుకైన_FLAG = 0x80000000u;

  నిర్మాణం glyph_సమితి_cache_కీలకం_t
  {
    glyph_సమితి_cache_కీలకం_t () = అప్రమేయ;
    glyph_సమితి_cache_కీలకం_t (స్థిరరాశి శూన్య *దత్తాంశం_2, గుర్తులేని విలువ_3, glyph_సమితి_type_t type_)
      : దత్తాంశం (దత్తాంశం_2), value (విలువ_3), type (type_) {}

    తార్కిక క్రియాకారకం == (స్థిరరాశి glyph_సమితి_cache_కీలకం_t &o) స్థిరరాశి
    { తిరిగి దత్తాంశం == o.దత్తాంశం && value == o.value && type == o.type; }

    uint32_t hash () స్థిరరాశి
    {
      uint32_t current = hb_hash ((uintptr_t) దత్తాంశం);
      current = current * 31 + hb_hash (value);
      తిరిగి current * 31 + hb_hash ((గుర్తులేని) type);
    }

    స్థిరరాశి శూన్య *దత్తాంశం = శూన్యసూచిక;
    గుర్తులేని value = 0;
    glyph_సమితి_type_t type = GLYPH_సమితి_CLASS;
  };

  నిర్మాణం context_సమితి_cache_కీలకం_t
  {
    context_సమితి_cache_కీలకం_t () = అప్రమేయ;
    context_సమితి_cache_కీలకం_t (స్థిరరాశి శూన్య *నియమం,
			     స్థిరరాశి శూన్య *రికార్డు_3,
			     స్థిరరాశి శూన్య *దత్తాంశం_2,
			     hb_codepoint_t చురుకైన_glyphs,
			     గుర్తులేని విలువ_3,
			     గుర్తులేని format_)
      : నియమం_2 (నియమం), రికార్డు (రికార్డు_3), దత్తాంశం (దత్తాంశం_2), చురుకైన_glyphs_2 (చురుకైన_glyphs),
	value (విలువ_3), format (format_) {}

    తార్కిక క్రియాకారకం == (స్థిరరాశి context_సమితి_cache_కీలకం_t &o) స్థిరరాశి
    {
      తిరిగి నియమం_2 == o.నియమం_2 &&
	     రికార్డు == o.రికార్డు &&
	     దత్తాంశం == o.దత్తాంశం &&
	     చురుకైన_glyphs_2 == o.చురుకైన_glyphs_2 &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () స్థిరరాశి
    {
      uint32_t current = hb_hash ((uintptr_t) నియమం_2);
      current = current * 31 + hb_hash ((uintptr_t) రికార్డు);
      current = current * 31 + hb_hash ((uintptr_t) దత్తాంశం);
      current = current * 31 + hb_hash (చురుకైన_glyphs_2);
      current = current * 31 + hb_hash (value);
      తిరిగి current * 31 + hb_hash (format);
    }

    స్థిరరాశి శూన్య *నియమం_2 = శూన్యసూచిక;
    స్థిరరాశి శూన్య *రికార్డు = శూన్యసూచిక;
    స్థిరరాశి శూన్య *దత్తాంశం = శూన్యసూచిక;
    hb_codepoint_t చురుకైన_glyphs_2 = HB_CODEPOINT_INVALID;
    గుర్తులేని value = 0;
    గుర్తులేని format = 0;
  };

  నిర్మాణం recurse_కీలకం_t
  {
    recurse_కీలకం_t () = అప్రమేయ;
    recurse_కీలకం_t (గుర్తులేని lookup_సూచీ_3,
                   hb_codepoint_t చురుకైన_glyphs,
                   hb_codepoint_t recurse_మార్గం,
                   hb_codepoint_t context_సమితి_2)
      : lookup_సూచీ (lookup_సూచీ_3),
        చురుకైన_glyphs_2 (చురుకైన_glyphs),
        recurse_మార్గం_2 (recurse_మార్గం),
        context_సమితి (context_సమితి_2) {}

    తార్కిక క్రియాకారకం == (స్థిరరాశి recurse_కీలకం_t &o) స్థిరరాశి
    {
      తిరిగి lookup_సూచీ == o.lookup_సూచీ &&
             చురుకైన_glyphs_2 == o.చురుకైన_glyphs_2 &&
             recurse_మార్గం_2 == o.recurse_మార్గం_2 &&
             context_సమితి == o.context_సమితి;
    }

    uint32_t hash () స్థిరరాశి
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_సూచీ)) * 16777619;
      current = (current ^ hb_hash (చురుకైన_glyphs_2)) * 16777619;
      current = (current ^ hb_hash (recurse_మార్గం_2)) * 16777619;
      current = (current ^ hb_hash (context_సమితి)) * 16777619;
      తిరిగి current;
    }

    గుర్తులేని lookup_సూచీ = 0;
    hb_codepoint_t చురుకైన_glyphs_2 = 0;
    hb_codepoint_t recurse_మార్గం_2 = 0;
    hb_codepoint_t context_సమితి = HB_CODEPOINT_INVALID;
  };

  రకనిర్వచనం return_t (*recurse_func_t) (hb_depend_context_t *c, గుర్తులేని lookup_సూచీ, hb_బిట్టు_పుట_t *covered_seq_indices, గుర్తులేని seq_సూచీ, గుర్తులేని చివర_సూచీ);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  మూస <రకం_పేరు T>
  return_t dispatch (స్థిరరాశి T &obj) { obj.depend (ఇది); తిరిగి hb_ఖాళీలు_t (); }
  స్థిర return_t default_return_విలువ () { తిరిగి hb_ఖాళీలు_t (); }

  శూన్య recurse (గుర్తులేని lookup_idx, hb_బిట్టు_పుట_t *covered_seq_indices, గుర్తులేని seq_సూచీ, గుర్తులేని చివర_సూచీ)
  {
    /* Cycle detection using call-stack semantics: block re-entry of a
     * lookup that is currently on the execution stack, but allow it again
     * once it has returned.
     *
     * A flat "visited" set would be wrong: the same lookup legitimately
     * appears more than once when a rule iterates over multiple sequence
     * positions — each such call is independent and must be processed.
     *
     * Cutting an in-progress cycle is safe because the active glyph set
     * can only stay the same or shrink as we recurse.  If we re-encounter
     * lookup L while it is still on the stack, L is already executing
     * with a context at least as broad, so any edges a re-entry would add
     * are a subset of what L is already in the process of registering.
     *
     * lookups_seen is pre-seeded with the top-level lookup index in
     * GSUB_accelerator_t::depend before the direct depend() call, so
     * that indirect cycles of the form A→B→A are also caught here. */
    ఒకవేళ (lookups_చూసిన.has (lookup_idx)) తిరిగి;
    lookups_చూసిన.కూడు (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t మాతృ_మార్గం = recurse_మార్గం_2;
    uint64_t మార్గం_కీలకం = (uint64_t) మాతృ_మార్గం << 32 | lookup_idx;
    hb_codepoint_t *మార్గం = శూన్యసూచిక;
    hb_codepoint_t మార్గం_సూచీ;
    ఒకవేళ (!recurse_మార్గం_గమ్యం_సూచీ.has (మార్గం_కీలకం, &మార్గం))
    {
      మార్గం_సూచీ = recurse_మార్గం_గమ్యం_సూచీ.get_population () + 1;
      ఒకవేళ (unlikely (!recurse_మార్గం_గమ్యం_సూచీ.సమితి (మార్గం_కీలకం, మార్గం_సూచీ)))
      {
	depend_దత్తాంశం->fail ();
	lookups_చూసిన.del (lookup_idx);
	తిరిగి;
      }
    }
    లేకపోతే
      మార్గం_సూచీ = *మార్గం;
    recurse_మార్గం_2 = మార్గం_సూచీ;
    recurse_func (ఇది, lookup_idx, covered_seq_indices, seq_సూచీ, చివర_సూచీ);
    recurse_మార్గం_2 = మాతృ_మార్గం;
    lookups_చూసిన.del (lookup_idx);
  }

  నిర్మాణం చురుకైన_glyphs_చట్రం_t
  {
    hb_సమితి_t glyphs;
    hb_codepoint_t చురుకైన_idx = HB_CODEPOINT_INVALID;
    తార్కిక use_మూలం = అబద్ధం;
  };

  స్థిరరాశి hb_సమితి_t& మాతృ_చురుకైన_glyphs ()
  {
    ఒకవేళ (!చురుకైన_glyphs_దొంతర_లోతు)
      తిరిగి *glyphs;

    స్థిరరాశి చురుకైన_glyphs_చట్రం_t &చట్రం = చురుకైన_glyphs_దొంతర[చురుకైన_glyphs_దొంతర_లోతు - 1];
    తిరిగి చట్రం.use_మూలం ? *glyphs : చట్రం.glyphs;
  }

  hb_సమితి_t* నెట్టు_cur_చురుకైన_glyphs (hb_codepoint_t చురుకైన_idx = HB_CODEPOINT_INVALID)
  {
    ఒకవేళ (చురుకైన_glyphs_దొంతర_లోతు == చురుకైన_glyphs_దొంతర.నిడివి &&
	unlikely (!చురుకైన_glyphs_దొంతర.నెట్టు_or_fail ()))
      తిరిగి శూన్యసూచిక;

    చురుకైన_glyphs_చట్రం_t &చట్రం = చురుకైన_glyphs_దొంతర[చురుకైన_glyphs_దొంతర_లోతు++];
    చట్రం.glyphs.clear ();
    చట్రం.చురుకైన_idx = చురుకైన_idx;
    చట్రం.use_మూలం = అబద్ధం;
    తిరిగి &చట్రం.glyphs;
  }

  తార్కిక నెట్టు_మూలం_చురుకైన_glyphs ()
  {
    ఒకవేళ (చురుకైన_glyphs_దొంతర_లోతు == చురుకైన_glyphs_దొంతర.నిడివి &&
	unlikely (!చురుకైన_glyphs_దొంతర.నెట్టు_or_fail ()))
      తిరిగి అబద్ధం;

    చురుకైన_glyphs_దొంతర[చురుకైన_glyphs_దొంతర_లోతు++].use_మూలం = నిజం;
    తిరిగి నిజం;
  }

  తార్కిక తీయి_cur_పూర్తి_glyphs ()
  {
    ఒకవేళ (!చురుకైన_glyphs_దొంతర_లోతు)
      తిరిగి అబద్ధం;

    చురుకైన_glyphs_దొంతర_లోతు--;
    తిరిగి నిజం;
  }

  తార్కిక get_మాతృ_చురుకైన_glyphs_సూచీ (hb_codepoint_t *చురుకైన_idx)
  {
    ఒకవేళ (!చురుకైన_glyphs_దొంతర_లోతు ||
	చురుకైన_glyphs_దొంతర[చురుకైన_glyphs_దొంతర_లోతు - 1].use_మూలం)
    {
      *చురుకైన_idx = HB_CODEPOINT_INVALID;
      తిరిగి నిజం;
    }

    చురుకైన_glyphs_చట్రం_t &చట్రం = చురుకైన_glyphs_దొంతర[చురుకైన_glyphs_దొంతర_లోతు - 1];
    *చురుకైన_idx = చట్రం.చురుకైన_idx;
    ఒకవేళ (*చురుకైన_idx != HB_CODEPOINT_INVALID)
      తిరిగి నిజం;

    hb_codepoint_t glyph;
    ఒకవేళ (చట్రం.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_చురుకైన_FLAG))
      *చురుకైన_idx = glyph | SINGLE_GLYPH_చురుకైన_FLAG;
    లేకపోతే ఒకవేళ (!కనుగొను_or_create_recurse_సమితి (చట్రం.glyphs, చురుకైన_idx))
      తిరిగి అబద్ధం;

    చట్రం.చురుకైన_idx = *చురుకైన_idx;
    తిరిగి నిజం;
  }

  తార్కిక get_context_సమితి_cache_కీలకం (స్థిరరాశి శూన్య *నియమం_2,
				  స్థిరరాశి శూన్య *రికార్డు,
				  స్థిరరాశి శూన్య *దత్తాంశం,
				  గుర్తులేని value,
				  గుర్తులేని format,
				  context_సమితి_cache_కీలకం_t *key)
  {
    hb_codepoint_t చురుకైన_idx;
    ఒకవేళ (!get_మాతృ_చురుకైన_glyphs_సూచీ (&చురుకైన_idx))
      తిరిగి అబద్ధం;
    *key = context_సమితి_cache_కీలకం_t (నియమం_2, రికార్డు, దత్తాంశం, చురుకైన_idx, value, format);
    తిరిగి నిజం;
  }

  తార్కిక get_recurse_కీలకం (గుర్తులేని lookup_idx, recurse_కీలకం_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t చురుకైన_idx;
    ఒకవేళ (!get_మాతృ_చురుకైన_glyphs_సూచీ (&చురుకైన_idx))
      తిరిగి అబద్ధం;

    *key = recurse_కీలకం_t (lookup_idx,
                          చురుకైన_idx,
                          recurse_మార్గం_2,
                          depend_దత్తాంశం->current_context_సమితి_సూచీ);
    తిరిగి !completed_recursions.has (*key);
  }

  శూన్య ముగింపు_recurse (స్థిరరాశి recurse_కీలకం_t &key)
  { depend_దత్తాంశం->check_success (completed_recursions.సమితి (key, నిజం)); }

  శూన్య reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_మార్గం_గమ్యం_సూచీ.clear ();
    recurse_మార్గం_2 = 0;
  }

  తార్కిక కనుగొను_or_create_recurse_సమితి (స్థిరరాశి hb_సమితి_t &సమితి, hb_codepoint_t *సూచీ_2)
  {
    hb_codepoint_t *existing = శూన్యసూచిక;
    ఒకవేళ (recurse_సమితి_గమ్యం_సూచీ.has (&సమితి, &existing))
    {
      *సూచీ_2 = *existing;
      తిరిగి నిజం;
    }

    hb_సమితి_t *copy = hb_object_create<hb_సమితి_t> ();
    ఒకవేళ (unlikely (!copy))
      తిరిగి depend_దత్తాంశం->fail ();
    copy->సమితి (సమితి);
    ఒకవేళ (unlikely (copy->లోపల_error ()))
    {
      hb_సమితి_destroy (copy);
      తిరిగి depend_దత్తాంశం->fail ();
    }

    *సూచీ_2 = recurse_sets.నిడివి;
    ఒకవేళ (unlikely (*సూచీ_2 & SINGLE_GLYPH_చురుకైన_FLAG))
    {
      hb_సమితి_destroy (copy);
      తిరిగి depend_దత్తాంశం->fail ();
    }
    ఒకవేళ (unlikely (!recurse_sets.నెట్టు_or_fail (hb::unique_ptr<hb_సమితి_t> {copy})))
      తిరిగి depend_దత్తాంశం->fail ();
    ఒకవేళ (unlikely (!recurse_సమితి_గమ్యం_సూచీ.సమితి (recurse_sets[*సూచీ_2].get (), *సూచీ_2)))
      తిరిగి depend_దత్తాంశం->fail ();
    తిరిగి నిజం;
  }

  స్థిరరాశి hb_సమితి_t *కనుగొను_or_create_glyph_సమితి (స్థిరరాశి శూన్య *దత్తాంశం,
                                             గుర్తులేని value,
                                             glyph_సమితి_type_t type);
  hb_codepoint_t కనుగొను_or_create_cached_context_సమితి (స్థిరరాశి hb_సమితి_t *సమితి);

  నిర్మాణం depend_scratch_t
  {
    శూన్య clear ()
    {
      preliminary_context.clear ();
      position_scratch.clear ();
      pos_glyphs.clear ();
      position_context.clear ();
      disjunctive_indices.clear ();
      filtered_disjunctive_indices.clear ();
      backtrack_sets.clear ();
      lookahead_sets.clear ();
      ఆదానం_position_glyphs.clear ();
      cached_context_sets.clear ();
      compact_workspace.clear ();
    }

    hb_సమితి_t preliminary_context;
    hb_సమితి_t position_scratch;
    hb_సమితి_t pos_glyphs;
    hb_సమితి_t position_context;
    hb_సమితి_t disjunctive_indices;
    hb_సమితి_t filtered_disjunctive_indices;
    hb_సదిశ_t<స్థిరరాశి hb_సమితి_t *> backtrack_sets;
    hb_సదిశ_t<స్థిరరాశి hb_సమితి_t *> lookahead_sets;
    hb_సదిశ_t<స్థిరరాశి hb_సమితి_t *> ఆదానం_position_glyphs;
    hb_సదిశ_t<hb_codepoint_t> cached_context_sets;
    hb_సదిశ_t<గుర్తులేని> compact_workspace;
    depend_scratch_t *next = శూన్యసూచిక;
  };

  depend_scratch_t *acquire_depend_scratch ()
  {
    depend_scratch_t *scratch = depend_scratch_pool;
    ఒకవేళ (scratch)
      depend_scratch_pool = scratch->next;
    లేకపోతే
    {
      scratch = (depend_scratch_t *) hb_malloc (పరిమాణం (depend_scratch_t));
      ఒకవేళ (unlikely (!scratch))
      {
        depend_దత్తాంశం->fail ();
        తిరిగి శూన్యసూచిక;
      }
      కొత్త (scratch) depend_scratch_t ();
    }

    scratch->next = శూన్యసూచిక;
    scratch->clear ();
    తిరిగి scratch;
  }

  శూన్య release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_దత్తాంశం_builder_t *depend_దత్తాంశం;
  hb_face_t *face;
  hb_సమితి_t *glyphs;
  hb_సదిశ_t<చురుకైన_glyphs_చట్రం_t> చురుకైన_glyphs_దొంతర;
  గుర్తులేని చురుకైన_glyphs_దొంతర_లోతు = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_సూచీ = HB_CODEPOINT_INVALID;
  hb_బిట్టు_సమితి_t lookups_చూసిన;
  hb_సదిశ_t<hb::unique_ptr<hb_సమితి_t>> recurse_sets;
  hb_hashmap_t<స్థిరరాశి hb_సమితి_t *, hb_codepoint_t> recurse_సమితి_గమ్యం_సూచీ;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_మార్గం_గమ్యం_సూచీ;
  hb_codepoint_t recurse_మార్గం_2 = 0;
  hb_hashmap_t<recurse_కీలకం_t, తార్కిక> completed_recursions;
  hb_సదిశ_t<hb::unique_ptr<hb_సమితి_t>> glyph_sets;
  hb_hashmap_t<glyph_సమితి_cache_కీలకం_t, hb_codepoint_t> glyph_సమితి_గమ్యం_సూచీ;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_సమితి_indices;
  hb_hashmap_t<context_సమితి_cache_కీలకం_t, hb_codepoint_t> context_సమితి_cache;
  depend_scratch_t *depend_scratch_pool = శూన్యసూచిక;

  hb_depend_context_t (hb_depend_దత్తాంశం_builder_t *depend_దత్తాంశం_2,
                       hb_face_t *face_,
                       hb_సమితి_t *glyphs_)
                      : depend_దత్తాంశం (depend_దత్తాంశం_2),
                        face(face_),
                        glyphs (glyphs_),
                        recurse_func (శూన్యసూచిక) {}
  ~hb_depend_context_t ()
  {
    అయితే (depend_scratch_pool)
    {
      depend_scratch_t *scratch = depend_scratch_pool;
      depend_scratch_pool = scratch->next;
      scratch->~depend_scratch_t ();
      hb_free (scratch);
    }
  }
  శూన్య సమితి_recurse_func (recurse_func_t func) { recurse_func = func; }
};

మూస <రకం_పేరు సమితి_t>
నిర్మాణం hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<సమితి_t>, స్థిరరాశి Coverage &>
{
  రకనిర్వచనం స్థిరరాశి Coverage &return_t; // Stoopid that we have to dupe this here.
  మూస <రకం_పేరు T>
  return_t dispatch (స్థిరరాశి T &obj) { తిరిగి obj.get_coverage (); }
  స్థిర return_t default_return_విలువ () { తిరిగి Null (Coverage); }
  తార్కిక stop_sublookup_iteration (return_t r) స్థిరరాశి
  {
    r.collect_coverage (సమితి);
    తిరిగి అబద్ధం;
  }

  hb_collect_coverage_context_t (సమితి_t *సమితి_2) :
				   సమితి (సమితి_2) {}

  సమితి_t *సమితి;
};

నిర్మాణం matcher_t
{
  రకనిర్వచనం తార్కిక (*సరిపోలిక_func_t) (hb_glyph_info_t &info, గుర్తులేని value, స్థిరరాశి శూన్య *దత్తాంశం);

  మూస <రకం_పేరు context_t>
  శూన్య init (స్థిరరాశి context_t *c, తార్కిక context_సరిపోలిక = అబద్ధం)
  {
    సమితి_సరిపోలిక_func (శూన్యసూచిక, శూన్యసూచిక);
    lookup_props = c->lookup_props;
    /* Ignore ZWNJ if we are matching GPOS, or matching GSUB context and asked to. */
    ignore_zwnj = c->పట్టిక_సూచీ == 1 || (context_సరిపోలిక && c->auto_zwnj);
    /* Ignore ZWJ if we are matching context, or asked to. */
    ignore_zwj = context_సరిపోలిక || c->auto_zwj;
    /* Ignore hidden glyphs (like CGJ) during GPOS. */
    ignore_hidden = c->పట్టిక_సూచీ == 1;
    ముసుగు = context_సరిపోలిక ? -1 : c->lookup_ముసుగు;
    /* Per syllable matching is only for GSUB. */
    చొప్పున_syllable = c->పట్టిక_సూచీ == 0 && c->చొప్పున_syllable;
    syllable = 0;
  }

  శూన్య సమితి_సరిపోలిక_func (సరిపోలిక_func_t సరిపోలిక_func,
		       స్థిరరాశి శూన్య *సరిపోలిక_దత్తాంశం)
  { సరిపోలిక_func_2 = సరిపోలిక_func; సరిపోలిక_దత్తాంశం_2 = సరిపోలిక_దత్తాంశం; }

  గణన may_సరిపోలిక_t {
    సరిపోలిక_NO,
    సరిపోలిక_YES,
    సరిపోలిక_MAYBE
  };

#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  may_సరిపోలిక_t may_సరిపోలిక (hb_glyph_info_t &info,
			 hb_codepoint_t glyph_దత్తాంశం) స్థిరరాశి
  {
    ఒకవేళ (!(info.ముసుగు & ముసుగు) ||
	(చొప్పున_syllable && syllable && syllable != info.syllable ()))
      తిరిగి సరిపోలిక_NO;

    ఒకవేళ (సరిపోలిక_func_2)
      తిరిగి సరిపోలిక_func_2 (info, glyph_దత్తాంశం, సరిపోలిక_దత్తాంశం_2) ? సరిపోలిక_YES : సరిపోలిక_NO;

    తిరిగి సరిపోలిక_MAYBE;
  }

  గణన may_skip_t {
    SKIP_NO,
    SKIP_YES,
    SKIP_MAYBE
  };

  మూస <రకం_పేరు context_t>
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  may_skip_t may_skip (స్థిరరాశి context_t *c,
		       స్థిరరాశి hb_glyph_info_t &info) స్థిరరాశి
  {
    ఒకవేళ (!c->check_glyph_property (&info, lookup_props))
      తిరిగి SKIP_YES;

    ఒకవేళ (unlikely (_hb_glyph_info_is_default_ignorable (&info) &&
		  (ignore_zwnj || !_hb_glyph_info_is_zwnj (&info)) &&
		  (ignore_zwj || !_hb_glyph_info_is_zwj (&info)) &&
		  (ignore_hidden || !_hb_glyph_info_is_hidden (&info))))
      తిరిగి SKIP_MAYBE;

    తిరిగి SKIP_NO;
  }

  బహిరంగ:
  గుర్తులేని పూర్ణాంకం lookup_props = 0;
  hb_ముసుగు_t ముసుగు = -1;
  తార్కిక ignore_zwnj = అబద్ధం;
  తార్కిక ignore_zwj = అబద్ధం;
  తార్కిక ignore_hidden = అబద్ధం;
  తార్కిక చొప్పున_syllable = అబద్ధం;
  uint8_t syllable = 0;
  సరిపోలిక_func_t సరిపోలిక_func_2 = శూన్యసూచిక;
  స్థిరరాశి శూన్య *సరిపోలిక_దత్తాంశం_2 = శూన్యసూచిక;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
నిర్మాణం glyph_దత్తాంశం_t
{
  మూస <రకం_పేరు T>
  glyph_దత్తాంశం_t (స్థిరరాశి T *దత్తాంశం_2) : దత్తాంశం (దత్తాంశం_2)
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
                              , wide (పరిమాణం (T) == 3)
#అయితే_ముగింపు
  {
    static_assert (పరిమాణం (T) == 2 || పరిమాణం (T) == 3, "invalid glyph data size");
  }

  గుర్తులేని క్రియాకారకం [] (గుర్తులేని i) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (wide)
      తిరిగి ((స్థిరరాశి HBUINT24 *) దత్తాంశం)[i];
#అయితే_ముగింపు
    తిరిగి ((స్థిరరాశి HBUINT16 *) దత్తాంశం)[i];
  }

  స్థిరరాశి శూన్య *దత్తాంశం;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  తార్కిక wide;
#అయితే_ముగింపు
};

మూస <రకం_పేరు context_t>
నిర్మాణం skipping_iterator_t
{
  శూన్య init (context_t *c_, తార్కిక context_సరిపోలిక = అబద్ధం)
  {
    c = c_;
    చివర = c->బఫరు->len;
    సరిపోలిక_glyph_data16 = శూన్యసూచిక;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    సరిపోలిక_glyph_data24 = శూన్యసూచిక;
#అయితే_ముగింపు
    matcher.init (c, context_సరిపోలిక);
  }
  శూన్య సమితి_lookup_props (గుర్తులేని పూర్ణాంకం lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  శూన్య సమితి_సరిపోలిక_func (matcher_t::సరిపోలిక_func_t సరిపోలిక_func,
		       స్థిరరాశి శూన్య *సరిపోలిక_దత్తాంశం)
  {
    matcher.సమితి_సరిపోలిక_func (సరిపోలిక_func, సరిపోలిక_దత్తాంశం);
  }
  శూన్య సమితి_glyph_దత్తాంశం (glyph_దత్తాంశం_t glyph_దత్తాంశం)
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    సరిపోలిక_glyph_data16 = glyph_దత్తాంశం.wide ? శూన్యసూచిక : (స్థిరరాశి HBUINT16 *) glyph_దత్తాంశం.దత్తాంశం;
    సరిపోలిక_glyph_data24 = glyph_దత్తాంశం.wide ? (స్థిరరాశి HBUINT24 *) glyph_దత్తాంశం.దత్తాంశం : శూన్యసూచిక;
#లేకపోతే
    సరిపోలిక_glyph_data16 = (స్థిరరాశి HBUINT16 *) glyph_దత్తాంశం.దత్తాంశం;
#అయితే_ముగింపు
  }

#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  శూన్య reset (గుర్తులేని పూర్ణాంకం ప్రారంభం_సూచీ_2)
  {
    // For GSUB forward iterator
    idx = ప్రారంభం_సూచీ_2;
    చివర = c->బఫరు->len;
    matcher.syllable = c->బఫరు->cur().syllable();
  }
  శూన్య reset_వెనుక (గుర్తులేని పూర్ణాంకం ప్రారంభం_సూచీ_2, తార్కిక from_బహిర్గతం_బఫరు = అబద్ధం)
  {
    // For GSUB backward iterator
    idx = ప్రారంభం_సూచీ_2;
    matcher.syllable = c->బఫరు->cur().syllable();
  }

#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  శూన్య reset_fast (గుర్తులేని పూర్ణాంకం ప్రారంభం_సూచీ_2)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = ప్రారంభం_సూచీ_2;
  }

#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  matcher_t::may_skip_t may_skip (స్థిరరాశి hb_glyph_info_t &info) స్థిరరాశి
  { తిరిగి matcher.may_skip (c, info); }

  గణన సరిపోలిక_t {
    సరిపోలిక_2,
    NOT_సరిపోలిక,
    SKIP
  };

#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  సరిపోలిక_t సరిపోలిక (hb_glyph_info_t &info)
  {
    matcher_t::may_skip_t skip = matcher.may_skip (c, info);
    ఒకవేళ (unlikely (skip == matcher_t::SKIP_YES))
      తిరిగి SKIP;

    matcher_t::may_సరిపోలిక_t సరిపోలిక = matcher.may_సరిపోలిక (info, get_glyph_దత్తాంశం ());
    ఒకవేళ (సరిపోలిక == matcher_t::సరిపోలిక_YES ||
	(సరిపోలిక == matcher_t::సరిపోలిక_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      తిరిగి సరిపోలిక_2;

    ఒకవేళ (skip == matcher_t::SKIP_NO)
      తిరిగి NOT_సరిపోలిక;

    తిరిగి SKIP;
  }

  తార్కిక next (గుర్తులేని *unsafe_గమ్యం = శూన్యసూచిక)
  {
    స్వయంచాలక *info = c->బఫరు->info;
    స్థిరరాశి గుర్తుగల stop = (గుర్తుగల) చివర - 1;
    అయితే ((గుర్తుగల) idx < stop)
    {
      idx++;
      ఎంపిక (సరిపోలిక (info[idx]))
      {
	స్థితి సరిపోలిక_2:
	{
	  advance_glyph_దత్తాంశం ();
	  తిరిగి నిజం;
	}
	స్థితి NOT_సరిపోలిక:
	{
	  ఒకవేళ (unsafe_గమ్యం)
	    *unsafe_గమ్యం = idx + 1;
	  తిరిగి అబద్ధం;
	}
	స్థితి SKIP:
	  కొనసాగు;
      }
    }
    ఒకవేళ (unsafe_గమ్యం)
      *unsafe_గమ్యం = చివర;
    తిరిగి అబద్ధం;
  }
  తార్కిక prev (గుర్తులేని *unsafe_from = శూన్యసూచిక)
  {
    స్వయంచాలక *బహిర్గతం_info = c->బఫరు->బహిర్గతం_info;
    స్థిరరాశి గుర్తులేని stop = 0;
    అయితే (idx > stop)
    {
      idx--;
      ఎంపిక (సరిపోలిక (బహిర్గతం_info[idx]))
      {
	స్థితి సరిపోలిక_2:
	{
	  advance_glyph_దత్తాంశం ();
	  తిరిగి నిజం;
	}
	స్థితి NOT_సరిపోలిక:
	{
	  ఒకవేళ (unsafe_from)
	    *unsafe_from = hb_max (1u, idx) - 1u;
	  తిరిగి అబద్ధం;
	}
	స్థితి SKIP:
	  కొనసాగు;
      }
    }
    ఒకవేళ (unsafe_from)
      *unsafe_from = 0;
    తిరిగి అబద్ధం;
  }

  HB_ALWAYS_INLINE
  hb_codepoint_t
  get_glyph_దత్తాంశం ()
  {
    ఒకవేళ (సరిపోలిక_glyph_data16) తిరిగి *సరిపోలిక_glyph_data16;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    లేకపోతే
    ఒకవేళ (సరిపోలిక_glyph_data24) తిరిగి *సరిపోలిక_glyph_data24;
#అయితే_ముగింపు
    తిరిగి 0;
  }
  HB_ALWAYS_INLINE
  శూన్య
  advance_glyph_దత్తాంశం ()
  {
    ఒకవేళ (సరిపోలిక_glyph_data16) సరిపోలిక_glyph_data16++;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    లేకపోతే
    ఒకవేళ (సరిపోలిక_glyph_data24) సరిపోలిక_glyph_data24++;
#అయితే_ముగింపు
  }

  గుర్తులేని పూర్ణాంకం idx;
  రక్షిత:
  context_t *c;
  matcher_t matcher;
  స్థిరరాశి HBUINT16 *సరిపోలిక_glyph_data16;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  స్థిరరాశి HBUINT24 *సరిపోలిక_glyph_data24;
#అయితే_ముగింపు

  గుర్తులేని పూర్ణాంకం చివర;
};

నిర్మాణం hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, తార్కిక, HB_DEBUG_APPLY>
{
  స్థిరరాశి అక్షరం *get_name () { తిరిగి "APPLY"; }
  రకనిర్వచనం return_t (*recurse_func_t) (hb_ot_apply_context_t *c, గుర్తులేని పూర్ణాంకం lookup_సూచీ);

  మూస <రకం_పేరు T>
  స్థిర అంతఃవరుస స్వయంచాలక apply_ (స్థిరరాశి T &obj, hb_ot_apply_context_t *c, hb_ప్రాధాన్యత<1>) HB_RETURN (return_t, obj.apply (c, శూన్యసూచిక) )
  మూస <రకం_పేరు T>
  స్థిర అంతఃవరుస స్వయంచాలక apply_ (స్థిరరాశి T &obj, hb_ot_apply_context_t *c, hb_ప్రాధాన్యత<0>) HB_RETURN (return_t, obj.apply (c) )
  మూస <రకం_పేరు T>
  return_t dispatch (స్థిరరాశి T &obj) { తిరిగి apply_(obj, ఇది, hb_prioritize); }

  స్థిర return_t default_return_విలువ () { తిరిగి అబద్ధం; }
  తార్కిక stop_sublookup_iteration (return_t r) స్థిరరాశి { తిరిగి r; }
  return_t recurse (గుర్తులేని పూర్ణాంకం sub_lookup_సూచీ)
  {
    ప్రతిజ్ఞా (recurse_func);
    ఒకవేళ (unlikely (nesting_level_ఎడమ == 0))
    {
      బఫరు->successful = అబద్ధం;
      తిరిగి default_return_విలువ ();
    }

    బఫరు->max_ops--;
    ఒకవేళ (unlikely (బఫరు->max_ops < 0))
    {
      బఫరు->successful = అబద్ధం;
      తిరిగి default_return_విలువ ();
    }

    nesting_level_ఎడమ--;
    తార్కిక ret = recurse_func (ఇది, sub_lookup_సూచీ);
    nesting_level_ఎడమ++;
    తిరిగి ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_ఆదానం, iter_context;

  గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_బఫరు_t *బఫరు;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = శూన్యసూచిక;
  స్థిరరాశి GDEF &gdef;
  స్థిరరాశి GDEF::accelerator_t &gdef_accel;
  స్థిరరాశి అంశం_Variation_Store &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_ముసుగు_t lookup_ముసుగు = 1;
  గుర్తులేని పూర్ణాంకం lookup_సూచీ = (గుర్తులేని) -1;
  గుర్తులేని పూర్ణాంకం lookup_props = 0;
  గుర్తులేని పూర్ణాంకం nesting_level_ఎడమ = HB_MAX_NESTING_LEVEL;

  తార్కిక has_glyph_classes;
  తార్కిక auto_zwnj = నిజం;
  తార్కిక auto_zwj = నిజం;
  తార్కిక చొప్పున_syllable = అబద్ధం;
  తార్కిక random = అబద్ధం;
  గుర్తులేని new_syllables = (గుర్తులేని) -1;

  గుర్తుగల చివరి_పునాది = -1; // GPOS uses
  గుర్తులేని చివరి_పునాది_until = 0; // GPOS uses

  hb_సదిశ_t<uint32_t> సరిపోలిక_positions;
  uint32_t దొంతర_సరిపోలిక_positions[8];
#నిర్వచితం_కాకపోతే HB_NO_బఫరు_సందేశం
  hb_బఫరు_t::మారింది_func_t orig_మారింది_func = శూన్యసూచిక;
  శూన్య *orig_మారింది_దత్తాంశం = శూన్యసూచిక;
#అయితే_ముగింపు

  hb_ot_apply_context_t (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ_2,
			 hb_font_t *font_,
			 hb_బఫరు_t *బఫరు_2,
			 hb_blob_t *పట్టిక_blob_2,
			 hb_scalar_cache_t *var_store_cache_ = శూన్యసూచిక) :
			పట్టిక_సూచీ (పట్టిక_సూచీ_2),
			font (font_), face (font->face), బఫరు (బఫరు_2),
			sanitizer (పట్టిక_blob_2),
			gdef (
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT
			      *face->పట్టిక.GDEF->పట్టిక
#లేకపోతే
			      Null (GDEF)
#అయితే_ముగింపు
			     ),
			gdef_accel (
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT
			      *face->పట్టిక.GDEF
#లేకపోతే
			      Null (GDEF::accelerator_t)
#అయితే_ముగింపు
			     ),
			var_store (gdef.get_var_store ()),
			var_store_cache (var_store_cache_),
			direction (బఫరు_2->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    సరిపోలిక_positions.సమితి_storage (దొంతర_సరిపోలిక_positions);

#నిర్వచితం_కాకపోతే HB_NO_బఫరు_సందేశం
    ఒకవేళ (బఫరు->messaging ())
    {
      ప్రతిజ్ఞా (బఫరు->మారింది_func == శూన్యసూచిక ||
	      బఫరు->మారింది_func == బఫరు_మారింది_trampoline);
      orig_మారింది_func = బఫరు->మారింది_func;
      orig_మారింది_దత్తాంశం = బఫరు->మారింది_దత్తాంశం;
      బఫరు->మారింది_func = బఫరు_మారింది_trampoline;
      బఫరు->మారింది_దత్తాంశం = ఇది;
    }
#అయితే_ముగింపు
  }
  ~hb_ot_apply_context_t ()
  {
#నిర్వచితం_కాకపోతే HB_NO_బఫరు_సందేశం
    ఒకవేళ (బఫరు->messaging ())
    {
      ప్రతిజ్ఞా (బఫరు->మారింది_func == బఫరు_మారింది_trampoline);
      ప్రతిజ్ఞా (బఫరు->మారింది_దత్తాంశం == ఇది);
      బఫరు->మారింది_func = orig_మారింది_func;
      బఫరు->మారింది_దత్తాంశం = orig_మారింది_దత్తాంశం;
    }
#అయితే_ముగింపు
  }

  శూన్య init_iters ()
  {
    iter_ఆదానం.init (ఇది, అబద్ధం);
    iter_context.init (ఇది, నిజం);
  }

  శూన్య సమితి_lookup_ముసుగు (hb_ముసుగు_t ముసుగు, తార్కిక init = నిజం) { lookup_ముసుగు = ముసుగు; చివరి_పునాది = -1; చివరి_పునాది_until = 0; ఒకవేళ (init) init_iters (); }
  శూన్య సమితి_auto_zwj (తార్కిక auto_zwj_, తార్కిక init = నిజం) { auto_zwj = auto_zwj_; ఒకవేళ (init) init_iters (); }
  శూన్య సమితి_auto_zwnj (తార్కిక auto_zwnj_, తార్కిక init = నిజం) { auto_zwnj = auto_zwnj_; ఒకవేళ (init) init_iters (); }
  శూన్య సమితి_చొప్పున_syllable (తార్కిక చొప్పున_syllable_2, తార్కిక init = నిజం) { చొప్పున_syllable = చొప్పున_syllable_2; ఒకవేళ (init) init_iters (); }
  శూన్య సమితి_random (తార్కిక random_) { random = random_; }
  శూన్య సమితి_recurse_func (recurse_func_t func) { recurse_func = func; }
  శూన్య సమితి_lookup_సూచీ (గుర్తులేని పూర్ణాంకం lookup_సూచీ_3) { lookup_సూచీ = lookup_సూచీ_3; }
  శూన్య సమితి_lookup_props (గుర్తులేని పూర్ణాంకం lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_సంఖ్య ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    బఫరు->random_state = బఫరు->random_state * 48271 % 2147483647;
    తిరిగి బఫరు->random_state;
  }

  స్థిర శూన్య బఫరు_మారింది_trampoline (hb_బఫరు_t *బఫరు HB_వాడని, శూన్య *user_దత్తాంశం)
  {
    ((hb_ot_apply_context_t *) user_దత్తాంశం)->బఫరు_మారింది ();
  }
  శూన్య బఫరు_మారింది ()
  {
    బఫరు->update_digest ();
    ఒకవేళ (likely (has_glyph_classes))
      కొరకు (గుర్తులేని i = 0; i < బఫరు->len; i++)
	సమితి_glyph_class_props (బఫరు->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  తార్కిక సరిపోలిక_properties_mark (స్థిరరాశి hb_glyph_info_t *info,
			      గుర్తులేని పూర్ణాంకం    glyph_props,
			      గుర్తులేని పూర్ణాంకం    సరిపోలిక_props) స్థిరరాశి
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    ఒకవేళ (సరిపోలిక_props & LookupFlag::Use_Mark_Filtering_సమితి)
    {
      గుర్తులేని సమితి_సూచీ = సరిపోలిక_props >> 16;
      తిరిగి gdef_accel.mark_సమితి_covers (సమితి_సూచీ, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    ఒకవేళ (సరిపోలిక_props & LookupFlag::MarkAttachmentType)
      తిరిగి (సరిపోలిక_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    తిరిగి నిజం;
  }

#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  తార్కిక check_glyph_property (స్థిరరాశి hb_glyph_info_t *info,
			     గుర్తులేని సరిపోలిక_props) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    ఒకవేళ (glyph_props & సరిపోలిక_props & LookupFlag::IgnoreFlags)
      తిరిగి అబద్ధం;

    ఒకవేళ (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      తిరిగి సరిపోలిక_properties_mark (info, glyph_props, సరిపోలిక_props);

    తిరిగి నిజం;
  }

  శూన్య సమితి_glyph_class (hb_codepoint_t glyph_సూచీ,
			  గుర్తులేని పూర్ణాంకం class_guess = 0,
			  తార్కిక ligature = అబద్ధం,
			  తార్కిక component = అబద్ధం)
  {
    బఫరు->digest.కూడు (glyph_సూచీ);

    ఒకవేళ (new_syllables != (గుర్తులేని) -1)
      బఫరు->cur().syllable() = new_syllables;

    గుర్తులేని పూర్ణాంకం props = _hb_glyph_info_get_glyph_props (&బఫరు->cur());
    props |= HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
    ఒకవేళ (ligature)
    {
      props |= HB_OT_LAYOUT_GLYPH_PROPS_LIGATED;
      /* In the only place that the MULTIPLIED bit is used, Uniscribe
       * seems to only care about the "last" transformation between
       * Ligature and Multiple substitutions.  Ie. if you ligate, expand,
       * and ligate again, it forgives the multiplication and acts as
       * if only ligation happened.  As such, clear MULTIPLIED bit.
       */
      props &= ~HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
    }
    ఒకవేళ (component)
      props |= HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
    ఒకవేళ (likely (has_glyph_classes))
    {
      సమితి_glyph_class_props (బఫరు->cur(), props, glyph_సూచీ);
    }
    లేకపోతే ఒకవేళ (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      hb_glyph_info_సమితి_glyph_props (&బఫరు->cur(), props | class_guess);
    }
    లేకపోతే
      hb_glyph_info_సమితి_glyph_props (&బఫరు->cur(), props);
  }
  శూన్య సమితి_glyph_class_props (hb_glyph_info_t &info,
			       గుర్తులేని పూర్ణాంకం props,
			       hb_codepoint_t glyph_సూచీ)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    hb_glyph_info_సమితి_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_సూచీ));
  }
  శూన్య సమితి_glyph_class_props (hb_glyph_info_t &info)
  {
    సమితి_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  శూన్య replace_glyph (hb_codepoint_t glyph_సూచీ)
  {
    సమితి_glyph_class (glyph_సూచీ);
    (శూన్య) బఫరు->replace_glyph (glyph_సూచీ);
  }
  శూన్య replace_glyph_inplace (hb_codepoint_t glyph_సూచీ)
  {
    సమితి_glyph_class (glyph_సూచీ);
    బఫరు->cur().codepoint = glyph_సూచీ;
  }
  శూన్య replace_glyph_with_ligature (hb_codepoint_t glyph_సూచీ,
				    గుర్తులేని పూర్ణాంకం class_guess)
  {
    సమితి_glyph_class (glyph_సూచీ, class_guess, నిజం);
    (శూన్య) బఫరు->replace_glyph (glyph_సూచీ);
  }
  శూన్య output_glyph_for_component (hb_codepoint_t glyph_సూచీ,
				   గుర్తులేని పూర్ణాంకం class_guess)
  {
    సమితి_glyph_class (glyph_సూచీ, class_guess, అబద్ధం, నిజం);
    (శూన్య) బఫరు->output_glyph (glyph_సూచీ);
  }
};

గణన తరగతి hb_ot_subtable_cache_op_t
{
  ENTER,
  LEAVE,
};

నిర్మాణం hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  మూస <రకం_పేరు T>
  స్థిర అంతఃవరుస స్వయంచాలక collect_రెండవ_glyphs_2 (స్థిరరాశి T &obj,
					     hb_సమితి_digest_t *digest,
					     hb_ప్రాధాన్యత<1>) HB_RETURN (శూన్య, obj.collect_రెండవ_glyphs (digest) )
  మూస <రకం_పేరు T>
  స్థిర అంతఃవరుస శూన్య collect_రెండవ_glyphs_2 (స్థిరరాశి T &obj HB_వాడని,
					     hb_సమితి_digest_t *digest,
					     hb_ప్రాధాన్యత<0>)
  { *digest = hb_సమితి_digest_t::నిండిన (); }

  మూస <రకం_పేరు T>
  స్థిర అంతఃవరుస స్వయంచాలక apply_ (స్థిరరాశి T *obj, hb_ot_apply_context_t *c, శూన్య *external_cache, hb_ప్రాధాన్యత<1>) HB_RETURN (తార్కిక, obj->apply (c, external_cache) )
  మూస <రకం_పేరు T>
  స్థిర అంతఃవరుస స్వయంచాలక apply_ (స్థిరరాశి T *obj, hb_ot_apply_context_t *c, శూన్య *external_cache, hb_ప్రాధాన్యత<0>) HB_RETURN (తార్కిక, obj->apply (c) )
  మూస <రకం_పేరు T>
  స్థిర అంతఃవరుస తార్కిక apply_గమ్యం (స్థిరరాశి శూన్య *obj, hb_ot_apply_context_t *c, శూన్య *external_cache)
  {
    స్థిరరాశి T *typed_obj = (స్థిరరాశి T *) obj;
    తిరిగి apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
  మూస <రకం_పేరు T>
  స్థిర అంతఃవరుస స్వయంచాలక apply_cached_ (స్థిరరాశి T *obj, hb_ot_apply_context_t *c, శూన్య *external_cache, hb_ప్రాధాన్యత<2>) HB_RETURN (తార్కిక, obj->apply_cached (c, external_cache) )
  మూస <రకం_పేరు T>
  స్థిర అంతఃవరుస స్వయంచాలక apply_cached_ (స్థిరరాశి T *obj, hb_ot_apply_context_t *c, శూన్య *external_cache, hb_ప్రాధాన్యత<1>) HB_RETURN (తార్కిక, obj->apply (c, external_cache) )
  మూస <రకం_పేరు T>
  స్థిర అంతఃవరుస స్వయంచాలక apply_cached_ (స్థిరరాశి T *obj, hb_ot_apply_context_t *c, శూన్య *external_cache, hb_ప్రాధాన్యత<0>) HB_RETURN (తార్కిక, obj->apply (c) )
  మూస <రకం_పేరు T>
  స్థిర అంతఃవరుస తార్కిక apply_cached_గమ్యం (స్థిరరాశి శూన్య *obj, hb_ot_apply_context_t *c, శూన్య *external_cache)
  {
    స్థిరరాశి T *typed_obj = (స్థిరరాశి T *) obj;
    తిరిగి apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  మూస <రకం_పేరు T>
  స్థిర అంతఃవరుస స్వయంచాలక cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_ప్రాధాన్యత<1>) HB_RETURN (తార్కిక, T::cache_func (c, op) )
  మూస <రకం_పేరు T=శూన్య>
  స్థిర అంతఃవరుస తార్కిక cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_వాడని,
				  hb_ప్రాధాన్యత<0>) { తిరిగి అబద్ధం; }
  మూస <రకం_పేరు Type>
  స్థిర అంతఃవరుస తార్కిక cache_func_గమ్యం (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    తిరిగి cache_func_<Type> (c, op, hb_prioritize);
  }
#అయితే_ముగింపు

  రకనిర్వచనం తార్కిక (*hb_apply_func_t) (స్థిరరాశి శూన్య *obj, hb_ot_apply_context_t *c, శూన్య *external_cache);
  రకనిర్వచనం తార్కిక (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  నిర్మాణం hb_applicable_t
  {
    మిత్ర నిర్మాణం hb_accelerate_subtables_context_t;
    మిత్ర నిర్మాణం hb_ot_layout_lookup_accelerator_t;

    మూస <రకం_పేరు T>
    శూన్య init (స్థిరరాశి T &obj_,
	       hb_apply_func_t apply_func_
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
	       , hb_apply_func_t apply_cached_func_
	       , hb_cache_func_t cache_func_
	       , శూన్య *external_cache_
#అయితే_ముగింపు
		)
    {
      obj = &obj_;
      apply_func = apply_func_;
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
      apply_cached_func = apply_cached_func_;
      cache_func = cache_func_;
      external_cache = external_cache_;
#అయితే_ముగింపు
      digest.init ();
      obj_.get_coverage ().collect_coverage (&digest);
    }

#నిర్వచితమైతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
    {
      తిరిగి digest.may_have (c->బఫరు->cur().codepoint) && apply_func (obj, c, శూన్యసూచిక);
    }
    తార్కిక apply_no_digest (hb_ot_apply_context_t *c) స్థిరరాశి
    {
      తిరిగి apply_func (obj, c, శూన్యసూచిక);
    }
#లేకపోతే
    తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
    {
      తిరిగి digest.may_have (c->బఫరు->cur().codepoint) && apply_func (obj, c, external_cache);
    }
    తార్కిక apply_cached (hb_ot_apply_context_t *c) స్థిరరాశి
    {
      తిరిగి digest.may_have (c->బఫరు->cur().codepoint) &&  apply_cached_func (obj, c, external_cache);
    }
    తార్కిక apply_no_digest (hb_ot_apply_context_t *c) స్థిరరాశి
    {
      తిరిగి apply_func (obj, c, external_cache);
    }
    తార్కిక apply_cached_no_digest (hb_ot_apply_context_t *c) స్థిరరాశి
    {
      తిరిగి apply_cached_func (obj, c, external_cache);
    }

    తార్కిక cache_enter (hb_ot_apply_context_t *c) స్థిరరాశి
    {
      తిరిగి cache_func (c, hb_ot_subtable_cache_op_t::ENTER);
    }
    శూన్య cache_leave (hb_ot_apply_context_t *c) స్థిరరాశి
    {
      cache_func (c, hb_ot_subtable_cache_op_t::LEAVE);
    }
#అయితే_ముగింపు

    అంతరంగ:
    స్థిరరాశి శూన్య *obj;
    hb_apply_func_t apply_func;
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    hb_apply_func_t apply_cached_func;
    hb_cache_func_t cache_func;
    శూన్య *external_cache;
#అయితే_ముగింపు
    hb_సమితి_digest_t digest;
  };

#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
  మూస <రకం_పేరు T>
  స్వయంచాలక cache_ఖర్చు (స్థిరరాశి T &obj, hb_ప్రాధాన్యత<1>) HB_AUTO_RETURN ( obj.cache_ఖర్చు () )
  మూస <రకం_పేరు T>
  స్వయంచాలక cache_ఖర్చు (స్థిరరాశి T &obj, hb_ప్రాధాన్యత<0>) HB_AUTO_RETURN ( 0u )

  మూస <రకం_పేరు T>
  స్వయంచాలక external_cache_create (స్థిరరాశి T &obj, hb_ప్రాధాన్యత<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  మూస <రకం_పేరు T>
  స్వయంచాలక external_cache_create (స్థిరరాశి T &obj, hb_ప్రాధాన్యత<0>) HB_AUTO_RETURN ( శూన్యసూచిక )
#అయితే_ముగింపు

  /* Dispatch interface. */
  మూస <రకం_పేరు T>
  return_t dispatch (స్థిరరాశి T &obj)
  {
    collect_రెండవ_glyphs_2 (obj, &digest_రెండవ, hb_prioritize);

#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    శూన్య *external_cache = శూన్యసూచిక;
    ఒకవేళ (i < 8)
      external_cache = external_cache_create (obj, hb_prioritize);
#అయితే_ముగింపు

    hb_applicable_t *నమోదు_2 = &array[i++];

    నమోదు_2->init (obj,
		 apply_గమ్యం<T>
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
		 , apply_cached_గమ్యం<T>
		 , cache_func_గమ్యం<T>
		 , external_cache
#అయితే_ముగింపు
		 );

#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    /* Cache handling
     *
     * We allow one subtable from each lookup to use a cache. The assumption
     * being that multiple subtables of the same lookup cannot use a cache
     * because the resources they would use will collide.  As such, we ask
     * each subtable to tell us how much it costs (which a cache would avoid),
     * and we allocate the cache opportunity to the costliest subtable.
     */
    గుర్తులేని ఖర్చు = cache_ఖర్చు (obj, hb_prioritize);
    ఒకవేళ (ఖర్చు > subtable_cache_user_ఖర్చు)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_ఖర్చు = ఖర్చు;
    }
#అయితే_ముగింపు

    తిరిగి hb_ఖాళీలు_t ();
  }
  స్థిర return_t default_return_విలువ () { తిరిగి hb_ఖాళీలు_t (); }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_) :
				     array (array_) {}

  hb_applicable_t *array;
  గుర్తులేని i = 0;
  hb_సమితి_digest_t digest_రెండవ;

#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
  గుర్తులేని subtable_cache_user_idx = (గుర్తులేని) -1;
  గుర్తులేని subtable_cache_user_ఖర్చు = 0;
#అయితే_ముగింపు
};


రకనిర్వచనం తార్కిక (*intersects_func_t) (స్థిరరాశి hb_సమితి_t *glyphs, గుర్తులేని value, స్థిరరాశి శూన్య *దత్తాంశం, శూన్య *cache);
రకనిర్వచనం శూన్య (*intersected_glyphs_func_t) (స్థిరరాశి hb_సమితి_t *glyphs, స్థిరరాశి శూన్య *దత్తాంశం, గుర్తులేని value, hb_సమితి_t *intersected_glyphs, శూన్య *cache);
రకనిర్వచనం శూన్య (*collect_glyphs_func_t) (hb_సమితి_t *glyphs, గుర్తులేని value, స్థిరరాశి శూన్య *దత్తాంశం);
రకనిర్వచనం తార్కిక (*సరిపోలిక_func_t) (hb_glyph_info_t &info, గుర్తులేని value, స్థిరరాశి శూన్య *దత్తాంశం);
రకనిర్వచనం గుర్తులేని (*get_విలువ_func_t) (hb_glyph_info_t &info, స్థిరరాశి శూన్య *దత్తాంశం);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
నిర్మాణం hb_ot_layout_ruleset_digest_t
{
  శూన్య init () { ముసుగు = 0; }
  శూన్య సమితి_నిండిన () { ముసుగు = (uint64_t) -1; }
  శూన్య కూడు (గుర్తులేని value) { ముసుగు |= (uint64_t) 1 << (value & 63); }
  తార్కిక may_have (గుర్తులేని value) స్థిరరాశి { తిరిగి ముసుగు & ((uint64_t) 1 << (value & 63)); }

  uint64_t ముసుగు;
};

నిర్మాణం Context_సంవృతం_Funcs
{
  intersects_func_t intersects;
  intersected_glyphs_func_t intersected_glyphs;
};
నిర్మాణం ContextCollectGlyphsFuncs
{
  collect_glyphs_func_t collect;
};
నిర్మాణం ContextApplyFuncs
{
  సరిపోలిక_func_t సరిపోలిక;
};
నిర్మాణం ChainContextApplyFuncs
{
  సరిపోలిక_func_t సరిపోలిక[3];
};


స్థిర అంతఃవరుస తార్కిక intersects_glyph (స్థిరరాశి hb_సమితి_t *glyphs, గుర్తులేని value, స్థిరరాశి శూన్య *దత్తాంశం HB_వాడని, శూన్య *cache HB_వాడని)
{
  తిరిగి glyphs->has (value);
}
స్థిర అంతఃవరుస తార్కిక intersects_class (స్థిరరాశి hb_సమితి_t *glyphs, గుర్తులేని value, స్థిరరాశి శూన్య *దత్తాంశం, శూన్య *cache)
{
  స్థిరరాశి ClassDef &class_def = *reinterpret_cast<స్థిరరాశి ClassDef *>(దత్తాంశం);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  ఒకవేళ (map->has (value, &cached_v))
    తిరిగి *cached_v;

  తార్కిక v = class_def.intersects_class (glyphs, value);
  map->సమితి (value, v);

  తిరిగి v;
}
స్థిర అంతఃవరుస తార్కిక intersects_coverage (స్థిరరాశి hb_సమితి_t *glyphs, గుర్తులేని value, స్థిరరాశి శూన్య *దత్తాంశం, శూన్య *cache HB_వాడని)
{
  Offset32_గమ్యం<Coverage> coverage;
  coverage = value;
  తిరిగి (దత్తాంశం+coverage).intersects (glyphs);
}


స్థిర అంతఃవరుస శూన్య intersected_glyph (స్థిరరాశి hb_సమితి_t *glyphs HB_వాడని, స్థిరరాశి శూన్య *దత్తాంశం, గుర్తులేని value, hb_సమితి_t *intersected_glyphs, HB_వాడని శూన్య *cache)
{
  గుర్తులేని g = reinterpret_cast<స్థిరరాశి HBUINT16 *>(దత్తాంశం)[value];
  intersected_glyphs->కూడు (g);
}

ఉపయోగించి intersected_class_cache_t = hb_hashmap_t<గుర్తులేని, hb_సమితి_t>;

స్థిర అంతఃవరుస శూన్య intersected_class_glyphs (స్థిరరాశి hb_సమితి_t *glyphs, స్థిరరాశి శూన్య *దత్తాంశం, గుర్తులేని value, hb_సమితి_t *intersected_glyphs, శూన్య *cache)
{
  స్థిరరాశి ClassDef &class_def = *reinterpret_cast<స్థిరరాశి ClassDef *>(దత్తాంశం);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_సమితి_t *cached_v;
  ఒకవేళ (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    తిరిగి;
  }

  hb_సమితి_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->సమితి (value, std::move (v));
}

స్థిర అంతఃవరుస శూన్య intersected_coverage_glyphs (స్థిరరాశి hb_సమితి_t *glyphs, స్థిరరాశి శూన్య *దత్తాంశం, గుర్తులేని value, hb_సమితి_t *intersected_glyphs, HB_వాడని శూన్య *cache)
{
  Offset32_గమ్యం<Coverage> coverage;
  coverage = value;
  (దత్తాంశం+coverage).intersect_సమితి (*glyphs, *intersected_glyphs);
}


స్థిర అంతఃవరుస తార్కిక array_is_subset_యొక్క (స్థిరరాశి hb_సమితి_t *glyphs,
				       గుర్తులేని పూర్ణాంకం లెక్క,
				       glyph_దత్తాంశం_t values,
				       intersects_func_t intersects_func,
				       స్థిరరాశి శూన్య *intersects_దత్తాంశం,
				       శూన్య *cache)
{
  కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    ఒకవేళ (!intersects_func (glyphs, values[i], intersects_దత్తాంశం, cache)) తిరిగి అబద్ధం;
  తిరిగి నిజం;
}


స్థిర అంతఃవరుస శూన్య collect_glyph (hb_సమితి_t *glyphs, గుర్తులేని value, స్థిరరాశి శూన్య *దత్తాంశం HB_వాడని)
{
  glyphs->కూడు (value);
}
స్థిర అంతఃవరుస శూన్య collect_class (hb_సమితి_t *glyphs, గుర్తులేని value, స్థిరరాశి శూన్య *దత్తాంశం)
{
  స్థిరరాశి ClassDef &class_def = *reinterpret_cast<స్థిరరాశి ClassDef *>(దత్తాంశం);
  class_def.collect_class (glyphs, value);
}
స్థిర అంతఃవరుస శూన్య collect_coverage (hb_సమితి_t *glyphs, గుర్తులేని value, స్థిరరాశి శూన్య *దత్తాంశం)
{
  Offset32_గమ్యం<Coverage> coverage;
  coverage = value;
  (దత్తాంశం+coverage).collect_coverage (glyphs);
}

అంతఃవరుస స్థిరరాశి hb_సమితి_t *
hb_depend_context_t::కనుగొను_or_create_glyph_సమితి (స్థిరరాశి శూన్య *దత్తాంశం,
                                                గుర్తులేని value,
                                                glyph_సమితి_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_సమితి_cache_కీలకం_t key (దత్తాంశం, value, type);
  hb_codepoint_t *existing = శూన్యసూచిక;
  ఒకవేళ (glyph_సమితి_గమ్యం_సూచీ.has (key, &existing))
    తిరిగి glyph_sets[*existing].get ();

  hb_సమితి_t *సమితి = hb_object_create<hb_సమితి_t> ();
  ఒకవేళ (unlikely (!సమితి))
  {
    depend_దత్తాంశం->fail ();
    తిరిగి శూన్యసూచిక;
  }

  ఒకవేళ (type == GLYPH_సమితి_COVERAGE)
    collect_coverage (సమితి, value, దత్తాంశం);
  లేకపోతే ఒకవేళ (type == GLYPH_సమితి_CLASS)
    collect_class (సమితి, value, దత్తాంశం);
  లేకపోతే
    సమితి->కూడు (value);
  ఒకవేళ (unlikely (సమితి->లోపల_error ()))
  {
    hb_సమితి_destroy (సమితి);
    depend_దత్తాంశం->fail ();
    తిరిగి శూన్యసూచిక;
  }

  hb_codepoint_t సూచీ_2 = glyph_sets.నిడివి;
  ఒకవేళ (unlikely (!glyph_sets.నెట్టు_or_fail (hb::unique_ptr<hb_సమితి_t> {సమితి})))
  {
    depend_దత్తాంశం->fail ();
    తిరిగి శూన్యసూచిక;
  }
  ఒకవేళ (unlikely (!glyph_సమితి_గమ్యం_సూచీ.సమితి (key, సూచీ_2)))
  {
    depend_దత్తాంశం->fail ();
    తిరిగి శూన్యసూచిక;
  }
  తిరిగి glyph_sets[సూచీ_2].get ();
}

అంతఃవరుస hb_codepoint_t
hb_depend_context_t::కనుగొను_or_create_cached_context_సమితి (స్థిరరాశి hb_సమితి_t *సమితి)
{
  uintptr_t key = (uintptr_t) సమితి;
  hb_codepoint_t *existing = శూన్యసూచిక;
  ఒకవేళ (cached_context_సమితి_indices.has (key, &existing))
    తిరిగి *existing;

  hb_codepoint_t సూచీ_2 = depend_దత్తాంశం->కనుగొను_or_create_context_సమితి (*సమితి);
  ఒకవేళ (unlikely (సూచీ_2 == HB_CODEPOINT_INVALID))
    తిరిగి HB_CODEPOINT_INVALID;
  ఒకవేళ (unlikely (!cached_context_సమితి_indices.సమితి (key, సూచీ_2)))
    తిరిగి depend_దత్తాంశం->fail_invalid ();
  తిరిగి సూచీ_2;
}

స్థిర అంతఃవరుస శూన్య collect_array (hb_collect_glyphs_context_t *c HB_వాడని,
				  hb_సమితి_t *glyphs,
				  గుర్తులేని పూర్ణాంకం లెక్క,
				  glyph_దత్తాంశం_t values,
				  collect_glyphs_func_t collect_func,
				  స్థిరరాశి శూన్య *collect_దత్తాంశం)
{
  కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    collect_func (glyphs, values[i], collect_దత్తాంశం);
}


స్థిర అంతఃవరుస తార్కిక సరిపోలిక_always (hb_glyph_info_t &info HB_వాడని, గుర్తులేని value HB_వాడని, స్థిరరాశి శూన్య *దత్తాంశం HB_వాడని)
{
  తిరిగి నిజం;
}
స్థిర అంతఃవరుస తార్కిక సరిపోలిక_glyph (hb_glyph_info_t &info, గుర్తులేని value, స్థిరరాశి శూన్య *దత్తాంశం HB_వాడని)
{
  తిరిగి info.codepoint == value;
}
స్థిర అంతఃవరుస తార్కిక సరిపోలిక_class (hb_glyph_info_t &info, గుర్తులేని value, స్థిరరాశి శూన్య *దత్తాంశం)
{
  స్థిరరాశి ClassDef &class_def = *reinterpret_cast<స్థిరరాశి ClassDef *>(దత్తాంశం);
  తిరిగి class_def.get_class (info.codepoint) == value;
}
స్థిర అంతఃవరుస గుర్తులేని get_class_విలువ (hb_glyph_info_t &info, స్థిరరాశి శూన్య *దత్తాంశం)
{
  స్థిరరాశి ClassDef &class_def = *reinterpret_cast<స్థిరరాశి ClassDef *> (దత్తాంశం);
  తిరిగి class_def.get_class (info.codepoint);
}
నిర్మాణం సరిపోలిక_class_cache_దత్తాంశం_t
{
  స్థిరరాశి ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
స్థిర అంతఃవరుస తార్కిక సరిపోలిక_class_cache (hb_glyph_info_t &info, గుర్తులేని value, స్థిరరాశి శూన్య *దత్తాంశం)
{
  స్థిరరాశి స్వయంచాలక &cache_దత్తాంశం = *reinterpret_cast<స్థిరరాశి సరిపోలిక_class_cache_దత్తాంశం_t *> (దత్తాంశం);
  తిరిగి cache_దత్తాంశం.class_def->get_class (info.codepoint, cache_దత్తాంశం.cache) == value;
}
స్థిర అంతఃవరుస గుర్తులేని get_class_cache_విలువ (hb_glyph_info_t &info, స్థిరరాశి శూన్య *దత్తాంశం)
{
  స్థిరరాశి స్వయంచాలక &cache_దత్తాంశం = *reinterpret_cast<స్థిరరాశి సరిపోలిక_class_cache_దత్తాంశం_t *> (దత్తాంశం);
  తిరిగి cache_దత్తాంశం.class_def->get_class (info.codepoint, cache_దత్తాంశం.cache);
}
స్థిర అంతఃవరుస గుర్తులేని get_class_cached (స్థిరరాశి ClassDef &class_def, hb_glyph_info_t &info)
{
  గుర్తులేని klass = info.syllable();
  ఒకవేళ (klass < 255)
    తిరిగి klass;
  klass = class_def.get_class (info.codepoint);
  ఒకవేళ (likely (klass < 255))
    info.syllable() = klass;
  తిరిగి klass;
}
స్థిర అంతఃవరుస తార్కిక సరిపోలిక_class_cached (hb_glyph_info_t &info, గుర్తులేని value, స్థిరరాశి శూన్య *దత్తాంశం)
{
  స్థిరరాశి ClassDef &class_def = *reinterpret_cast<స్థిరరాశి ClassDef *>(దత్తాంశం);
  తిరిగి get_class_cached (class_def, info) == value;
}
స్థిర అంతఃవరుస గుర్తులేని get_class_cached_విలువ (hb_glyph_info_t &info, స్థిరరాశి శూన్య *దత్తాంశం)
{
  స్థిరరాశి ClassDef &class_def = *reinterpret_cast<స్థిరరాశి ClassDef *> (దత్తాంశం);
  తిరిగి get_class_cached (class_def, info);
}
స్థిర అంతఃవరుస గుర్తులేని get_class_cached1 (స్థిరరాశి ClassDef &class_def, hb_glyph_info_t &info)
{
  గుర్తులేని klass = info.syllable() & 0x0F;
  ఒకవేళ (klass < 15)
    తిరిగి klass;
  klass = class_def.get_class (info.codepoint);
  ఒకవేళ (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  తిరిగి klass;
}
స్థిర అంతఃవరుస తార్కిక సరిపోలిక_class_cached1 (hb_glyph_info_t &info, గుర్తులేని value, స్థిరరాశి శూన్య *దత్తాంశం)
{
  స్థిరరాశి ClassDef &class_def = *reinterpret_cast<స్థిరరాశి ClassDef *>(దత్తాంశం);
  తిరిగి get_class_cached1 (class_def, info) == value;
}
స్థిర అంతఃవరుస గుర్తులేని get_class_cached2 (స్థిరరాశి ClassDef &class_def, hb_glyph_info_t &info)
{
  గుర్తులేని klass = (info.syllable() & 0xF0) >> 4;
  ఒకవేళ (klass < 15)
    తిరిగి klass;
  klass = class_def.get_class (info.codepoint);
  ఒకవేళ (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  తిరిగి klass;
}
స్థిర అంతఃవరుస తార్కిక సరిపోలిక_class_cached2 (hb_glyph_info_t &info, గుర్తులేని value, స్థిరరాశి శూన్య *దత్తాంశం)
{
  స్థిరరాశి ClassDef &class_def = *reinterpret_cast<స్థిరరాశి ClassDef *>(దత్తాంశం);
  తిరిగి get_class_cached2 (class_def, info) == value;
}
స్థిర అంతఃవరుస గుర్తులేని get_class_cached2_విలువ (hb_glyph_info_t &info, స్థిరరాశి శూన్య *దత్తాంశం)
{
  స్థిరరాశి ClassDef &class_def = *reinterpret_cast<స్థిరరాశి ClassDef *> (దత్తాంశం);
  తిరిగి get_class_cached2 (class_def, info);
}
స్థిర అంతఃవరుస తార్కిక సరిపోలిక_coverage (hb_glyph_info_t &info, గుర్తులేని value, స్థిరరాశి శూన్య *దత్తాంశం)
{
  Offset32_గమ్యం<Coverage> coverage;
  coverage = value;
  తిరిగి (దత్తాంశం+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

స్థిర అంతఃవరుస తార్కిక would_సరిపోలిక_ఆదానం (hb_would_apply_context_t *c,
				      గుర్తులేని పూర్ణాంకం లెక్క, /* Including the first glyph (not matched) */
				      glyph_దత్తాంశం_t ఆదానం, /* Array of input values--start with second glyph */
				      సరిపోలిక_func_t సరిపోలిక_func_2,
				      స్థిరరాశి శూన్య *సరిపోలిక_దత్తాంశం_2)
{
  ఒకవేళ (లెక్క != c->len)
    తిరిగి అబద్ధం;

  కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < లెక్క; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    ఒకవేళ (likely (!సరిపోలిక_func_2 (info, ఆదానం[i - 1], సరిపోలిక_దత్తాంశం_2)))
      తిరిగి అబద్ధం;
  }

  తిరిగి నిజం;
}
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
HB_ALWAYS_INLINE
#లేకపోతే
అంతఃవరుస
#అయితే_ముగింపు
స్థిర తార్కిక సరిపోలిక_ఆదానం (hb_ot_apply_context_t *c,
			 గుర్తులేని పూర్ణాంకం లెక్క, /* Including the first glyph (not matched) */
			 glyph_దత్తాంశం_t ఆదానం, /* Array of input values--start with second glyph */
			 సరిపోలిక_func_t సరిపోలిక_func_2,
			 స్థిరరాశి శూన్య *సరిపోలిక_దత్తాంశం_2,
			 గుర్తులేని పూర్ణాంకం *చివర_position,
			 గుర్తులేని పూర్ణాంకం *p_మొత్తం_component_లెక్క = శూన్యసూచిక)
{
  TRACE_APPLY (శూన్యసూచిక);

  hb_బఫరు_t *బఫరు = c->బఫరు;

  ఒకవేళ (లెక్క == 1)
  {
    *చివర_position = బఫరు->idx + 1;
    c->సరిపోలిక_positions[0] = బఫరు->idx;
    ఒకవేళ (p_మొత్తం_component_లెక్క)
      *p_మొత్తం_component_లెక్క = hb_glyph_info_get_lig_అంకె_comps (&బఫరు->cur());
    return_trace (నిజం);
  }

  ఒకవేళ (unlikely (లెక్క > HB_MAX_CONTEXT_నిడివి)) return_trace (అబద్ధం);

  స్వయంచాలక &skippy_iter = c->iter_ఆదానం;
  skippy_iter.reset (బఫరు->idx);
  skippy_iter.సమితి_సరిపోలిక_func (సరిపోలిక_func_2, సరిపోలిక_దత్తాంశం_2);
  skippy_iter.సమితి_glyph_దత్తాంశం (ఆదానం);

  /*
   * This is perhaps the trickiest part of OpenType...  Remarks:
   *
   * - If all components of the ligature were marks, we call this a mark ligature.
   *
   * - If there is no GDEF, and the ligature is NOT a mark ligature, we categorize
   *   it as a ligature glyph.
   *
   * - Ligatures cannot be formed across glyphs attached to different components
   *   of previous ligatures.  Eg. the sequence is LAM,SHADDA,LAM,FATHA,HEH, and
   *   LAM,LAM,HEH form a ligature, leaving SHADDA,FATHA next to eachother.
   *   However, it would be wrong to ligate that SHADDA,FATHA sequence.
   *   There are a couple of exceptions to this:
   *
   *   o If a ligature tries ligating with marks that belong to it itself, go ahead,
   *     assuming that the font designer knows what they are doing (otherwise it can
   *     break Indic stuff when a matra wants to ligate with a conjunct,
   *
   *   o If two marks want to ligate and they belong to different components of the
   *     same ligature glyph, and said ligature glyph is to be ignored according to
   *     mark-filtering rules, then allow.
   *     https://github.com/harfbuzz/harfbuzz/issues/545
   */

  గుర్తులేని పూర్ణాంకం మొత్తం_component_లెక్క = 0;

  గుర్తులేని పూర్ణాంకం మొదటి_lig_id = _hb_glyph_info_get_lig_id (&బఫరు->cur());
  గుర్తులేని పూర్ణాంకం మొదటి_lig_comp = _hb_glyph_info_get_lig_comp (&బఫరు->cur());

  గణన {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < లెక్క; i++)
  {
    గుర్తులేని unsafe_గమ్యం;
    ఒకవేళ (!skippy_iter.next (&unsafe_గమ్యం))
    {
      *చివర_position = unsafe_గమ్యం;
      return_trace (అబద్ధం);
    }

    ఒకవేళ (unlikely (i + 1 > c->సరిపోలిక_positions.నిడివి &&
		  !c->సరిపోలిక_positions.resize_dirty  (i + 1)))
      return_trace (అబద్ధం);
    c->సరిపోలిక_positions.arrayZ[i] = skippy_iter.idx;

    గుర్తులేని పూర్ణాంకం this_lig_id = _hb_glyph_info_get_lig_id (&బఫరు->info[skippy_iter.idx]);
    గుర్తులేని పూర్ణాంకం this_lig_comp = _hb_glyph_info_get_lig_comp (&బఫరు->info[skippy_iter.idx]);

    ఒకవేళ (మొదటి_lig_id && మొదటి_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      ఒకవేళ (మొదటి_lig_id != this_lig_id || మొదటి_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	ఒకవేళ (ligbase == LIGBASE_NOT_CHECKED)
	{
	  తార్కిక దొరికింది = అబద్ధం;
	  స్థిరరాశి స్వయంచాలక *బహిర్గతం = బఫరు->బహిర్గతం_info;
	  గుర్తులేని పూర్ణాంకం j = బఫరు->బహిర్గతం_len;
	  అయితే (j && _hb_glyph_info_get_lig_id (&బహిర్గతం[j - 1]) == మొదటి_lig_id)
	  {
	    ఒకవేళ (_hb_glyph_info_get_lig_comp (&బహిర్గతం[j - 1]) == 0)
	    {
	      j--;
	      దొరికింది = నిజం;
	      ఆపు;
	    }
	    j--;
	  }

	  ఒకవేళ (దొరికింది && skippy_iter.may_skip (బహిర్గతం[j]) == matcher_t::SKIP_YES)
	    ligbase = LIGBASE_MAY_SKIP;
	  లేకపోతే
	    ligbase = LIGBASE_MAY_NOT_SKIP;
	}

	ఒకవేళ (ligbase == LIGBASE_MAY_NOT_SKIP)
	  return_trace (అబద్ధం);
      }
    }
    లేకపోతే
    {
      /* If first component was NOT attached to a previous ligature component,
       * all subsequent components should also NOT be attached to any ligature
       * component, unless they are attached to the first component itself! */
      ఒకవేళ (this_lig_id && this_lig_comp && (this_lig_id != మొదటి_lig_id))
	return_trace (అబద్ధం);
    }

    మొత్తం_component_లెక్క += hb_glyph_info_get_lig_అంకె_comps_లోపల_ligation (&బఫరు->info[skippy_iter.idx]);
  }

  *చివర_position = skippy_iter.idx + 1;

  ఒకవేళ (p_మొత్తం_component_లెక్క)
  {
    మొత్తం_component_లెక్క += hb_glyph_info_get_lig_అంకె_comps (&బఫరు->cur());
    *p_మొత్తం_component_లెక్క = మొత్తం_component_లెక్క;
  }

  c->సరిపోలిక_positions.arrayZ[0] = బఫరు->idx;

  return_trace (నిజం);
}
స్థిర అంతఃవరుస తార్కిక ligate_ఆదానం (hb_ot_apply_context_t *c,
				 గుర్తులేని పూర్ణాంకం లెక్క, /* Including the first glyph */
				 గుర్తులేని పూర్ణాంకం సరిపోలిక_చివర,
				 hb_codepoint_t lig_glyph,
				 గుర్తులేని పూర్ణాంకం మొత్తం_component_లెక్క)
{
  TRACE_APPLY (శూన్యసూచిక);

  hb_బఫరు_t *బఫరు = c->బఫరు;

  బఫరు->విలీనం_clusters (బఫరు->idx, సరిపోలిక_చివర);

  /* - If a base and one or more marks ligate, consider that as a base, NOT
   *   ligature, such that all following marks can still attach to it.
   *   https://github.com/harfbuzz/harfbuzz/issues/1109
   *
   * - If all components of the ligature were marks, we call this a mark ligature.
   *   If it *is* a mark ligature, we don't allocate a new ligature id, and leave
   *   the ligature to keep its old ligature id.  This will allow it to attach to
   *   a base ligature in GPOS.  Eg. if the sequence is: LAM,LAM,SHADDA,FATHA,HEH,
   *   and LAM,LAM,HEH for a ligature, they will leave SHADDA and FATHA with a
   *   ligature id and component value of 2.  Then if SHADDA,FATHA form a ligature
   *   later, we don't want them to lose their ligature id/component, otherwise
   *   GPOS will fail to correctly position the mark ligature on top of the
   *   LAM,LAM,HEH ligature.  See:
   *     https://bugzilla.gnome.org/show_bug.cgi?id=676343
   *
   * - If a ligature is formed of components that some of which are also ligatures
   *   themselves, and those ligature components had marks attached to *their*
   *   components, we have to attach the marks to the new ligature component
   *   positions!  Now *that*'s tricky!  And these marks may be following the
   *   last component of the whole sequence, so we should loop forward looking
   *   for them and update them.
   *
   *   Eg. the sequence is LAM,LAM,SHADDA,FATHA,HEH, and the font first forms a
   *   'calt' ligature of LAM,HEH, leaving the SHADDA and FATHA with a ligature
   *   id and component == 1.  Now, during 'liga', the LAM and the LAM-HEH ligature
   *   form a LAM-LAM-HEH ligature.  We need to reassign the SHADDA and FATHA to
   *   the new ligature with a component value of 2.
   *
   *   This in fact happened to a font...  See:
   *   https://bugzilla.gnome.org/show_bug.cgi?id=437633
   */

  తార్కిక is_పునాది_ligature = hb_glyph_info_is_పునాది_glyph (&బఫరు->info[c->సరిపోలిక_positions.arrayZ[0]]);
  తార్కిక is_mark_ligature = _hb_glyph_info_is_mark (&బఫరు->info[c->సరిపోలిక_positions.arrayZ[0]]);
  కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < లెక్క; i++)
    ఒకవేళ (!_hb_glyph_info_is_mark (&బఫరు->info[c->సరిపోలిక_positions.arrayZ[i]]))
    {
      is_పునాది_ligature = అబద్ధం;
      is_mark_ligature = అబద్ధం;
      ఆపు;
    }
  తార్కిక is_ligature = !is_పునాది_ligature && !is_mark_ligature;

  గుర్తులేని పూర్ణాంకం klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  గుర్తులేని పూర్ణాంకం lig_id = is_ligature ? _hb_allocate_lig_id (బఫరు) : 0;
  గుర్తులేని పూర్ణాంకం చివరి_lig_id = _hb_glyph_info_get_lig_id (&బఫరు->cur());
  గుర్తులేని పూర్ణాంకం చివరి_అంకె_components = hb_glyph_info_get_lig_అంకె_comps (&బఫరు->cur());
  గుర్తులేని పూర్ణాంకం components_so_far = చివరి_అంకె_components;

  ఒకవేళ (is_ligature)
  {
    hb_glyph_info_సమితి_lig_props_for_ligature (&బఫరు->cur(), lig_id, మొత్తం_component_లెక్క);
    ఒకవేళ (_hb_glyph_info_get_general_category (&బఫరు->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      hb_glyph_info_సమితి_general_category (&బఫరు->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < లెక్క; i++)
  {
    అయితే (బఫరు->idx < c->సరిపోలిక_positions.arrayZ[i] && బఫరు->successful)
    {
      ఒకవేళ (is_ligature)
      {
	గుర్తులేని పూర్ణాంకం this_comp = _hb_glyph_info_get_lig_comp (&బఫరు->cur());
	ఒకవేళ (this_comp == 0)
	  this_comp = చివరి_అంకె_components;
	ప్రతిజ్ఞా (components_so_far >= చివరి_అంకె_components);
	గుర్తులేని పూర్ణాంకం new_lig_comp = components_so_far - చివరి_అంకె_components +
				    hb_min (this_comp, చివరి_అంకె_components);
	  hb_glyph_info_సమితి_lig_props_for_mark (&బఫరు->cur(), lig_id, new_lig_comp);
      }
      (శూన్య) బఫరు->తదుపరి_glyph ();
    }

    చివరి_lig_id = _hb_glyph_info_get_lig_id (&బఫరు->cur());
    చివరి_అంకె_components = hb_glyph_info_get_lig_అంకె_comps_లోపల_ligation (&బఫరు->cur());
    components_so_far += చివరి_అంకె_components;

    /* Skip the base glyph */
    బఫరు->idx++;
  }

  ఒకవేళ (!is_mark_ligature && చివరి_lig_id)
  {
    /* Re-adjust components for any marks following. */
    కొరకు (గుర్తులేని i = బఫరు->idx; i < బఫరు->len; ++i)
    {
      ఒకవేళ (చివరి_lig_id != _hb_glyph_info_get_lig_id (&బఫరు->info[i])) ఆపు;

      గుర్తులేని this_comp = _hb_glyph_info_get_lig_comp (&బఫరు->info[i]);
      ఒకవేళ (!this_comp) ఆపు;

      ప్రతిజ్ఞా (components_so_far >= చివరి_అంకె_components);
      గుర్తులేని new_lig_comp = components_so_far - చివరి_అంకె_components +
			      hb_min (this_comp, చివరి_అంకె_components);
      hb_glyph_info_సమితి_lig_props_for_mark (&బఫరు->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (నిజం);
}

#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
HB_ALWAYS_INLINE
#లేకపోతే
అంతఃవరుస
#అయితే_ముగింపు
స్థిర తార్కిక సరిపోలిక_backtrack (hb_ot_apply_context_t *c,
			     గుర్తులేని పూర్ణాంకం లెక్క,
			     glyph_దత్తాంశం_t backtrack,
			     సరిపోలిక_func_t సరిపోలిక_func_2,
			     స్థిరరాశి శూన్య *సరిపోలిక_దత్తాంశం_2,
			     గుర్తులేని పూర్ణాంకం *సరిపోలిక_ప్రారంభం)
{
  TRACE_APPLY (శూన్యసూచిక);

  ఒకవేళ (!లెక్క)
  {
    *సరిపోలిక_ప్రారంభం = c->బఫరు->backtrack_len ();
    return_trace (నిజం);
  }

  స్వయంచాలక &skippy_iter = c->iter_context;
  skippy_iter.reset_వెనుక (c->బఫరు->backtrack_len ());
  skippy_iter.సమితి_సరిపోలిక_func (సరిపోలిక_func_2, సరిపోలిక_దత్తాంశం_2);
  skippy_iter.సమితి_glyph_దత్తాంశం (backtrack);

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    గుర్తులేని unsafe_from;
    ఒకవేళ (!skippy_iter.prev (&unsafe_from))
    {
      *సరిపోలిక_ప్రారంభం = unsafe_from;
      return_trace (అబద్ధం);
    }
  }

  *సరిపోలిక_ప్రారంభం = skippy_iter.idx;
  return_trace (నిజం);
}

#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
HB_ALWAYS_INLINE
#లేకపోతే
అంతఃవరుస
#అయితే_ముగింపు
స్థిర తార్కిక సరిపోలిక_lookahead (hb_ot_apply_context_t *c,
			     గుర్తులేని పూర్ణాంకం లెక్క,
			     glyph_దత్తాంశం_t lookahead,
			     సరిపోలిక_func_t సరిపోలిక_func_2,
			     స్థిరరాశి శూన్య *సరిపోలిక_దత్తాంశం_2,
			     గుర్తులేని పూర్ణాంకం ప్రారంభం_సూచీ,
			     గుర్తులేని పూర్ణాంకం *చివర_సూచీ)
{
  TRACE_APPLY (శూన్యసూచిక);

  ఒకవేళ (!లెక్క)
  {
    *చివర_సూచీ = ప్రారంభం_సూచీ;
    return_trace (నిజం);
  }

  స్వయంచాలక &skippy_iter = c->iter_context;
  ప్రతిజ్ఞా (ప్రారంభం_సూచీ >= 1);
  skippy_iter.reset (ప్రారంభం_సూచీ - 1);
  skippy_iter.సమితి_సరిపోలిక_func (సరిపోలిక_func_2, సరిపోలిక_దత్తాంశం_2);
  skippy_iter.సమితి_glyph_దత్తాంశం (lookahead);

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    గుర్తులేని unsafe_గమ్యం;
    ఒకవేళ (!skippy_iter.next (&unsafe_గమ్యం))
    {
      *చివర_సూచీ = unsafe_గమ్యం;
      return_trace (అబద్ధం);
    }
  }

  *చివర_సూచీ = skippy_iter.idx + 1;
  return_trace (నిజం);
}



నిర్మాణం Lookup_రికార్డు
{
  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి hb_map_t         *lookup_map) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక *బహిర్గతం = c->embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    return_trace (c->check_కేటాయించు (బహిర్గతం->lookup_List_సూచీ, lookup_map->get (lookup_List_సూచీ), HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  HBUINT16	sequence_సూచీ;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookup_List_సూచీ;	/* Lookup to apply to that
					 * position--zero--based */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

స్థిర అంతఃవరుస గుర్తులేని serialize_lookuprecord_array (hb_serialize_context_t *c,
						     స్థిరరాశి hb_array_t<స్థిరరాశి Lookup_రికార్డు> lookup_రికార్డులు,
						     స్థిరరాశి hb_map_t *lookup_map)
{
  గుర్తులేని లెక్క = 0;
  కొరకు (స్థిరరాశి Lookup_రికార్డు& r : lookup_రికార్డులు)
  {
    ఒకవేళ (!lookup_map->has (r.lookup_List_సూచీ))
      కొనసాగు;

    ఒకవేళ (!r.serialize (c, lookup_map))
      తిరిగి 0;

    లెక్క++;
  }
  తిరిగి లెక్క;
}

గణన ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

స్థిర అంతఃవరుస స్థిరరాశి hb_సమితి_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			స్థిరరాశి శూన్య *దత్తాంశం,
			గుర్తులేని value,
			తార్కిక filter_class_by_మాతృ,
			hb_సమితి_t *scratch,
			hb_సదిశ_t<గుర్తులేని> *compact_workspace = శూన్యసూచిక)
{
  ఎంపిక (context_format)
  {
    స్థితి ContextFormat::SimpleContext:
      తిరిగి c->కనుగొను_or_create_glyph_సమితి (శూన్యసూచిక, value,
                                           hb_depend_context_t::GLYPH_సమితి_SINGLE);
    స్థితి ContextFormat::ClassBasedContext:
    {
      స్థిరరాశి hb_సమితి_t *class_సమితి = c->కనుగొను_or_create_glyph_సమితి (
        దత్తాంశం, value, hb_depend_context_t::GLYPH_సమితి_CLASS);
      ఒకవేళ (unlikely (!class_సమితి))
	తిరిగి శూన్యసూచిక;
      ఒకవేళ (!filter_class_by_మాతృ)
	తిరిగి class_సమితి;
      scratch->సమితి (*class_సమితి);
      scratch->intersect (c->మాతృ_చురుకైన_glyphs (), compact_workspace);
      ఆపు;
    }
    స్థితి ContextFormat::CoverageBasedContext:
      తిరిగి c->కనుగొను_or_create_glyph_సమితి (
        దత్తాంశం, value, hb_depend_context_t::GLYPH_సమితి_COVERAGE);
  }

  ఒకవేళ (unlikely (scratch->లోపల_error ()))
  {
    c->depend_దత్తాంశం->fail ();
    తిరిగి శూన్యసూచిక;
  }
  తిరిగి scratch;
}

స్థిర అంతఃవరుస శూన్య context_సంవృతం_recurse_lookups (hb_సంవృతం_context_t *c,
					     గుర్తులేని ఆదానం_లెక్క, glyph_దత్తాంశం_t ఆదానం,
					     గుర్తులేని lookup_లెక్క_2,
					     స్థిరరాశి Lookup_రికార్డు lookup_రికార్డు[] /* Array of LookupRecords--in design order */,
					     గుర్తులేని value,
					     ContextFormat context_format,
					     స్థిరరాశి శూన్య *దత్తాంశం,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     శూన్య *cache)
{
  hb_సమితి_t covered_seq_indicies;
  hb_సమితి_t pos_glyphs;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < lookup_లెక్క_2; i++)
  {
    గుర్తులేని seq_సూచీ_2 = lookup_రికార్డు[i].sequence_సూచీ;
    ఒకవేళ (seq_సూచీ_2 >= ఆదానం_లెక్క) కొనసాగు;

    తార్కిక has_pos_glyphs = అబద్ధం;

    ఒకవేళ (!covered_seq_indicies.has (seq_సూచీ_2))
    {
      has_pos_glyphs = నిజం;
      pos_glyphs.clear ();
      ఒకవేళ (seq_సూచీ_2 == 0)
      {
        ఎంపిక (context_format) {
        స్థితి ContextFormat::SimpleContext:
          pos_glyphs.కూడు (value);
          ఆపు;
        స్థితి ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->మాతృ_చురుకైన_glyphs (), దత్తాంశం, value, &pos_glyphs, cache);
          ఆపు;
        స్థితి ContextFormat::CoverageBasedContext:
          pos_glyphs.సమితి (c->మాతృ_చురుకైన_glyphs ());
          ఆపు;
        }
      }
      లేకపోతే
      {
        స్థిరరాశి శూన్య *ఆదానం_దత్తాంశం = ఆదానం.దత్తాంశం;
        గుర్తులేని ఆదానం_విలువ = seq_సూచీ_2 - 1;
        ఒకవేళ (context_format != ContextFormat::SimpleContext)
        {
          ఆదానం_దత్తాంశం = దత్తాంశం;
          ఆదానం_విలువ = ఆదానం[seq_సూచీ_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, ఆదానం_దత్తాంశం, ఆదానం_విలువ, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.కూడు (seq_సూచీ_2);
    hb_సమితి_t *cur_చురుకైన_glyphs = c->నెట్టు_cur_చురుకైన_glyphs ();
    ఒకవేళ (unlikely (!cur_చురుకైన_glyphs))
      తిరిగి;
    ఒకవేళ (has_pos_glyphs) {
      *cur_చురుకైన_glyphs = std::move (pos_glyphs);
    } లేకపోతే {
      *cur_చురుకైన_glyphs = *c->glyphs;
    }

    గుర్తులేని చివర_సూచీ_2 = ఆదానం_లెక్క;
    ఒకవేళ (context_format == ContextFormat::CoverageBasedContext)
      చివర_సూచీ_2 += 1;

    c->recurse (lookup_రికార్డు[i].lookup_List_సూచీ, &covered_seq_indicies, seq_సూచీ_2, చివర_సూచీ_2);

    c->తీయి_cur_పూర్తి_glyphs ();
  }
}

స్థిర తార్కిక
context_depend_sets_are_cached (hb_depend_context_t *c,
				గుర్తులేని ఆదానం_లెక్క_2,
				గుర్తులేని lookup_లెక్క,
				స్థిరరాశి Lookup_రికార్డు lookup_రికార్డులు_2[],
				స్థిరరాశి శూన్య *నియమం_2,
				గుర్తులేని value,
				ContextFormat context_format,
				స్థిరరాశి శూన్య *దత్తాంశం,
				hb_సదిశ_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t చురుకైన_idx = HB_CODEPOINT_INVALID;
  తార్కిక have_చురుకైన_idx = అబద్ధం;
  కొరకు (గుర్తులేని i = 0; i < lookup_లెక్క; i++)
  {
    ఒకవేళ (lookup_రికార్డులు_2[i].sequence_సూచీ >= ఆదానం_లెక్క_2)
      కొనసాగు;

    ఒకవేళ (!have_చురుకైన_idx)
    {
      ఒకవేళ (unlikely (!c->get_మాతృ_చురుకైన_glyphs_సూచీ (&చురుకైన_idx)))
	తిరిగి అబద్ధం;
      have_చురుకైన_idx = నిజం;
    }

    hb_depend_context_t::context_సమితి_cache_కీలకం_t key (
      నియమం_2, &lookup_రికార్డులు_2[i], దత్తాంశం, చురుకైన_idx, value, context_format);
    hb_codepoint_t *cached_context_సమితి = శూన్యసూచిక;
    ఒకవేళ (!c->context_సమితి_cache.has (key, &cached_context_సమితి))
      తిరిగి అబద్ధం;
    ఒకవేళ (unlikely (!cached_context_sets->నెట్టు_or_fail (*cached_context_సమితి)))
      తిరిగి c->depend_దత్తాంశం->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  తిరిగి have_చురుకైన_idx;
}

స్థిర అంతఃవరుస శూన్య context_depend_recurse_lookups (hb_depend_context_t *c,
					     గుర్తులేని ఆదానం_లెక్క, glyph_దత్తాంశం_t ఆదానం,
					     గుర్తులేని lookup_లెక్క_2,
					     స్థిరరాశి Lookup_రికార్డు lookup_రికార్డు[] /* Array of LookupRecords--in design order */,
					     స్థిరరాశి శూన్య *నియమం_2,
					     గుర్తులేని value,
					     ContextFormat context_format,
					     స్థిరరాశి శూన్య *దత్తాంశం,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     శూన్య *cache,
					     స్థిరరాశి hb_సమితి_t &preliminary_context,
					     స్థిరరాశి hb_సదిశ_t<స్థిరరాశి hb_సమితి_t *> *ఆదానం_position_glyphs,
					     స్థిరరాశి hb_సదిశ_t<hb_codepoint_t> *cached_context_sets,
					     hb_depend_context_t::depend_scratch_t *scratch)
{
  /* For depend graph extraction, we filter active glyphs by InputCoverage constraints
   * (same as closure), but we do NOT do sequential accumulation of outputs.
   *
   * - Position filtering (InputCoverage): Required. The contextual rule can only match
   *   glyphs in its InputCoverage. Edges for other glyphs are spurious.
   * - Sequential accumulation: Not done. We want ALL possible edges, so we don't
   *   limit later lookups based on outputs from earlier lookups in the same rule. */

  /* Sequence positions are tracked in one allocation-free bit page below.
   * A rule longer than HB_MAX_CONTEXT_LENGTH never applies during shaping
   * (the apply path rejects it), so skip it here instead of running past
   * the fixed page in add_range(). */
  ఒకవేళ (unlikely (ఆదానం_లెక్క > HB_MAX_CONTEXT_నిడివి))
    తిరిగి;

  hb_బిట్టు_పుట_t covered_seq_indices;
  hb_సమితి_t &pos_glyphs = scratch->pos_glyphs;
  hb_సమితి_t &position_context = scratch->position_context;
  hb_సమితి_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_సమితి_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  గుర్తులేని cached_context_సూచీ = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < lookup_లెక్క_2; i++)
  {
    గుర్తులేని seq_సూచీ_2 = lookup_రికార్డు[i].sequence_సూచీ;
    ఒకవేళ (seq_సూచీ_2 >= ఆదానం_లెక్క)
      కొనసాగు;

    hb_depend_context_t::context_సమితి_cache_కీలకం_t context_cache_కీలకం;
    ఒకవేళ (unlikely (!c->get_context_సమితి_cache_కీలకం (నియమం_2, &lookup_రికార్డు[i], దత్తాంశం, value,
						 context_format, &context_cache_కీలకం)))
      తిరిగి;

    hb_codepoint_t context_సమితి_idx;
    hb_codepoint_t *cached_context_సమితి_idx;
    ఒకవేళ (cached_context_sets)
      context_సమితి_idx = (*cached_context_sets)[cached_context_సూచీ++];
    లేకపోతే ఒకవేళ (c->context_సమితి_cache.has (context_cache_కీలకం, &cached_context_సమితి_idx))
      context_సమితి_idx = *cached_context_సమితి_idx;
    లేకపోతే
    {
      ఒకవేళ (unlikely (!ఆదానం_position_glyphs))
      {
	c->depend_దత్తాంశం->fail ();
	తిరిగి;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_సమితి_విలువ_INVALID;
      అయితే (preliminary_context.next (&elem)) {
	ఒకవేళ (!(elem & HB_DEPEND_CONTEXT_సమితి_FLAG)) {
	  /* Direct glyph */
	  position_context.కూడు (elem);
	} లేకపోతే {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.కూడు (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      కొరకు (గుర్తులేని j = 0; j < ఆదానం_position_glyphs->నిడివి; j++) {
	ఒకవేళ (j != seq_సూచీ_2 && (*ఆదానం_position_glyphs)[j]->get_population () == 1) {
	  position_context.కూడు ((*ఆదానం_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_సమితి_విలువ_INVALID;
      అయితే (disjunctive_indices.next (&elem)) {
	hb_codepoint_t సమితి_idx = elem & 0x7FFFFFFF;
	స్థిరరాశి hb_సమితి_t *original_సమితి = c->depend_దత్తాంశం->get_సమితి_from_సూచీ (సమితి_idx);
	ఒకవేళ (unlikely (!original_సమితి)) కొనసాగు;

	/* Dependency context sets are never inverted. */
	ఒకవేళ (!original_సమితి->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.కూడు (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      కొరకు (గుర్తులేని j = 0; j < ఆదానం_position_glyphs->నిడివి; j++) {
	ఒకవేళ (j != seq_సూచీ_2 && (*ఆదానం_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  ఒకవేళ (!(*ఆదానం_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    తార్కిక cached_సమితి = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_సమితి
	      ? c->కనుగొను_or_create_cached_context_సమితి ((*ఆదానం_position_glyphs)[j])
	      : c->depend_దత్తాంశం->కనుగొను_or_create_context_సమితి (*(*ఆదానం_position_glyphs)[j]);
	    ఒకవేళ (unlikely (idx == HB_CODEPOINT_INVALID))
	      తిరిగి;
	    filtered_disjunctive_indices.కూడు (HB_DEPEND_CONTEXT_సమితి_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      ఒకవేళ (unlikely (position_context.లోపల_error () ||
		    filtered_disjunctive_indices.లోపల_error ()))
      {
	c->depend_దత్తాంశం->fail ();
	తిరిగి;
      }

      context_సమితి_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_దత్తాంశం->కనుగొను_or_create_context_సమితి (position_context);
      ఒకవేళ (unlikely (!position_context.is_empty () &&
		    context_సమితి_idx == HB_CODEPOINT_INVALID))
	తిరిగి;

      ఒకవేళ (unlikely (!c->context_సమితి_cache.సమితి (context_cache_కీలకం, context_సమితి_idx)))
      {
	c->depend_దత్తాంశం->fail ();
	తిరిగి;
      }
    }

    /* Save outer context to restore after this lookup. When a contextual lookup
     * calls another contextual lookup, we want each lookup in this rule to see
     * the same outer context, not context from a sibling lookup. */
    hb_codepoint_t saved_context_సమితి_సూచీ = c->depend_దత్తాంశం->current_context_సమితి_సూచీ;
    hb_subset_depend_అంచు_flags_t saved_అంచు_flags = c->depend_దత్తాంశం->current_అంచు_flags;

    /* Detect nested contextual: if we already have a context set, we're inside
     * an outer contextual rule that called this one. The outer context will be
     * lost, so edges created here may over-approximate. */
    తార్కిక nested_contextual = (saved_context_సమితి_సూచీ != HB_CODEPOINT_INVALID);

    /* Set for this position's edges */
    c->depend_దత్తాంశం->current_context_సమితి_సూచీ = context_సమితి_idx;

    /* Mark edges from multi-position rules (inputCount > 1) as potentially
     * over-approximate. Depend extraction records edges based on what glyphs
     * COULD be at each position per the static input coverage/class. But during
     * closure computation, lookups within the rule are applied sequentially:
     * lookups at earlier positions may transform later positions, and multiple
     * lookups at the same position may interact (one produces a glyph, another
     * immediately consumes it as an "intermediate"). So a glyph matching the
     * coverage might not persist at that position when closure traverses. */
    c->depend_దత్తాంశం->current_అంచు_flags = HB_SUBSET_DEPEND_అంచు_FLAG_NONE;
    ఒకవేళ (ఆదానం_లెక్క > 1) {
      c->depend_దత్తాంశం->current_అంచు_flags |= HB_SUBSET_DEPEND_అంచు_FLAG_FROM_CONTEXT_POSITION;
    }
    ఒకవేళ (nested_contextual) {
      c->depend_దత్తాంశం->current_అంచు_flags |= HB_SUBSET_DEPEND_అంచు_FLAG_FROM_NESTED_CONTEXT;
    }

    తార్కిక has_pos_glyphs = అబద్ధం;

    ఒకవేళ (!covered_seq_indices.has (seq_సూచీ_2))
    {
      has_pos_glyphs = నిజం;
      pos_glyphs.clear ();
      ఒకవేళ (seq_సూచీ_2 == 0)
      {
        ఎంపిక (context_format) {
        స్థితి ContextFormat::SimpleContext:
          pos_glyphs.కూడు (value);
          ఆపు;
        స్థితి ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->మాతృ_చురుకైన_glyphs (), దత్తాంశం, value, &pos_glyphs, cache);
          ఆపు;
        స్థితి ContextFormat::CoverageBasedContext:
          pos_glyphs.సమితి (c->మాతృ_చురుకైన_glyphs ());
          ఆపు;
        }
      }
      లేకపోతే
      {
        స్థిరరాశి శూన్య *ఆదానం_దత్తాంశం = ఆదానం.దత్తాంశం;
        గుర్తులేని ఆదానం_విలువ = seq_సూచీ_2 - 1;
        ఒకవేళ (context_format != ContextFormat::SimpleContext)
        {
          ఆదానం_దత్తాంశం = దత్తాంశం;
          ఆదానం_విలువ = ఆదానం[seq_సూచీ_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, ఆదానం_దత్తాంశం, ఆదానం_విలువ, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.కూడు (seq_సూచీ_2);
    ఒకవేళ (has_pos_glyphs)
    {
      hb_codepoint_t చురుకైన_idx = HB_CODEPOINT_INVALID;
      ఒకవేళ (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seq_సూచీ_2 ? pos_glyphs.get_min () : value;
	ఒకవేళ (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_చురుకైన_FLAG))
	  చురుకైన_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_చురుకైన_FLAG;
      }
      hb_సమితి_t *cur_చురుకైన_glyphs = c->నెట్టు_cur_చురుకైన_glyphs (చురుకైన_idx);
      ఒకవేళ (unlikely (!cur_చురుకైన_glyphs))
      {
	c->depend_దత్తాంశం->current_context_సమితి_సూచీ = saved_context_సమితి_సూచీ;
	c->depend_దత్తాంశం->current_అంచు_flags = saved_అంచు_flags;
	తిరిగి;
      }
      *cur_చురుకైన_glyphs = std::move (pos_glyphs);
    }
    లేకపోతే ఒకవేళ (unlikely (!c->నెట్టు_మూలం_చురుకైన_glyphs ()))
    {
      c->depend_దత్తాంశం->current_context_సమితి_సూచీ = saved_context_సమితి_సూచీ;
      c->depend_దత్తాంశం->current_అంచు_flags = saved_అంచు_flags;
      తిరిగి;
    }

    గుర్తులేని చివర_సూచీ_2 = ఆదానం_లెక్క;
    ఒకవేళ (context_format == ContextFormat::CoverageBasedContext)
      చివర_సూచీ_2 += 1;

    c->recurse (lookup_రికార్డు[i].lookup_List_సూచీ, &covered_seq_indices, seq_సూచీ_2, చివర_సూచీ_2);
    c->తీయి_cur_పూర్తి_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_దత్తాంశం->current_context_సమితి_సూచీ = saved_context_సమితి_సూచీ;
    c->depend_దత్తాంశం->current_అంచు_flags = saved_అంచు_flags;
  }
}

మూస <రకం_పేరు context_t>
స్థిర అంతఃవరుస శూన్య recurse_lookups (context_t *c,
                                    గుర్తులేని పూర్ణాంకం lookup_లెక్క_2,
                                    స్థిరరాశి Lookup_రికార్డు lookup_రికార్డు[] /* Array of LookupRecords--in design order */)
{
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < lookup_లెక్క_2; i++)
    c->recurse (lookup_రికార్డు[i].lookup_List_సూచీ);
}

స్థిర అంతఃవరుస శూన్య apply_lookup (hb_ot_apply_context_t *c,
				 గుర్తులేని పూర్ణాంకం లెక్క, /* Including the first glyph */
				 గుర్తులేని పూర్ణాంకం lookup_లెక్క_2,
				 స్థిరరాశి Lookup_రికార్డు lookup_రికార్డు[], /* Array of LookupRecords--in design order */
				 గుర్తులేని పూర్ణాంకం సరిపోలిక_చివర)
{
  hb_బఫరు_t *బఫరు = c->బఫరు;
  పూర్ణాంకం చివర;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    గుర్తులేని పూర్ణాంకం bl = బఫరు->backtrack_len ();
    చివర = bl + సరిపోలిక_చివర - బఫరు->idx;

    పూర్ణాంకం delta = bl - బఫరు->idx;
    /* Convert positions to new indexing. */
    కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < లెక్క; j++)
      c->సరిపోలిక_positions.arrayZ[j] += delta;
  }

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < lookup_లెక్క_2 && బఫరు->successful; i++)
  {
    గుర్తులేని పూర్ణాంకం idx = lookup_రికార్డు[i].sequence_సూచీ;
    ఒకవేళ (idx >= లెక్క)
      కొనసాగు;

    గుర్తులేని పూర్ణాంకం orig_len = బఫరు->backtrack_len () + బఫరు->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    ఒకవేళ (unlikely (c->సరిపోలిక_positions.arrayZ[idx] >= orig_len))
      కొనసాగు;

    ఒకవేళ (unlikely (!బఫరు->move_గమ్యం (c->సరిపోలిక_positions.arrayZ[idx])))
      ఆపు;

    ఒకవేళ (unlikely (బఫరు->max_ops <= 0))
      ఆపు;

    ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
    {
      ఒకవేళ (బఫరు->have_output)
        c->బఫరు->sync_so_far ();
      c->బఫరు->సందేశం (c->font,
			  "start recursing to lookup %u at %u",
			  (గుర్తులేని) lookup_రికార్డు[i].lookup_List_సూచీ,
			  బఫరు->idx);
    }

    తార్కిక ret = c->recurse (lookup_రికార్డు[i].lookup_List_సూచీ);

    ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
    {
      ఒకవేళ (బఫరు->have_output)
        c->బఫరు->sync_so_far ();
      c->బఫరు->సందేశం (c->font,
			  "end recursing to lookup %u",
			  (గుర్తులేని) lookup_రికార్డు[i].lookup_List_సూచీ);
    }

    ఒకవేళ (!ret)
      కొనసాగు;

    గుర్తులేని పూర్ణాంకం new_len = బఫరు->backtrack_len () + బఫరు->lookahead_len ();
    పూర్ణాంకం delta = new_len - orig_len;

    ఒకవేళ (!delta)
      కొనసాగు;

    /* Recursed lookup changed buffer len.  Adjust.
     *
     * TODO:
     *
     * Right now, if buffer length increased by n, we assume n new glyphs
     * were added right after the current position, and if buffer length
     * was decreased by n, we assume n match positions after the current
     * one where removed.  The former (buffer length increased) case is
     * fine, but the decrease case can be improved in at least two ways,
     * both of which are significant:
     *
     *   - If recursed-to lookup is MultipleSubst and buffer length
     *     decreased, then it's current match position that was deleted,
     *     NOT the one after it.
     *
     *   - If buffer length was decreased by n, it does not necessarily
     *     mean that n match positions where removed, as there recursed-to
     *     lookup might had a different LookupFlag.  Here's a constructed
     *     case of that:
     *     https://github.com/harfbuzz/harfbuzz/discussions/3538
     *
     * It should be possible to construct tests for both of these cases.
     */

    చివర += delta;
    ఒకవేళ (చివర < పూర్ణాంకం (c->సరిపోలిక_positions.arrayZ[idx]))
    {
      /* End might end up being smaller than match_positions.arrayZ[idx] if the recursed
       * lookup ended up removing many items.
       * Just never rewind end beyond start of current position, since that is
       * not possible in the recursed lookup.  Also adjust delta as such.
       *
       * https://bugs.chromium.org/p/chromium/issues/detail?id=659496
       * https://github.com/harfbuzz/harfbuzz/issues/1611
       */
      delta += c->సరిపోలిక_positions.arrayZ[idx] - చివర;
      చివర = c->సరిపోలిక_positions.arrayZ[idx];
    }

    గుర్తులేని పూర్ణాంకం next = idx + 1; /* next now is the position after the recursed lookup. */

    ఒకవేళ (delta > 0)
    {
      ఒకవేళ (unlikely (delta + లెక్క > HB_MAX_CONTEXT_నిడివి))
	ఆపు;
      ఒకవేళ (unlikely (లెక్క + delta > c->సరిపోలిక_positions.నిడివి &&
		    !c->సరిపోలిక_positions.resize_dirty  (లెక్క + delta)))
        తిరిగి;
    }
    లేకపోతే
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (పూర్ణాంకం) next - (పూర్ణాంకం) లెక్క);
      next -= delta;
    }

    /* Shift! */
    memmove (c->సరిపోలిక_positions + next + delta, c->సరిపోలిక_positions + next,
	     (లెక్క - next) * పరిమాణం (c->సరిపోలిక_positions.arrayZ[0]));
    next += delta;
    లెక్క += delta;

    /* Fill in new entries. */
    కొరకు (గుర్తులేని పూర్ణాంకం j = idx + 1; j < next; j++)
      c->సరిపోలిక_positions.arrayZ[j] = c->సరిపోలిక_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    కొరకు (; next < లెక్క; next++)
      c->సరిపోలిక_positions.arrayZ[next] += delta;
  }

  ప్రతిజ్ఞా (చివర >= 0);
  (శూన్య) బఫరు->move_గమ్యం (చివర);
}



/* Contextual lookups */

నిర్మాణం Context_సంవృతం_Lookup_Context
{
  Context_సంవృతం_Funcs funcs;
  ContextFormat context_format;
  స్థిరరాశి శూన్య *intersects_దత్తాంశం;
  శూన్య *intersects_cache;
  శూన్య *intersected_glyphs_cache;
};

రకనిర్వచనం Context_సంవృతం_Lookup_Context ContextDependLookupContext;

నిర్మాణం ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  స్థిరరాశి శూన్య *collect_దత్తాంశం;
};

నిర్మాణం ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  స్థిరరాశి శూన్య *సరిపోలిక_దత్తాంశం_2;
  get_విలువ_func_t get_విలువ;
};

స్థిర అంతఃవరుస తార్కిక context_intersects (స్థిరరాశి hb_సమితి_t *glyphs,
				       గుర్తులేని పూర్ణాంకం ఆదానం_లెక్క, /* Including the first glyph (not matched) */
				       glyph_దత్తాంశం_t ఆదానం, /* Array of input values--start with second glyph */
				       Context_సంవృతం_Lookup_Context &lookup_context)
{
  తిరిగి array_is_subset_యొక్క (glyphs,
			     ఆదానం_లెక్క ? ఆదానం_లెక్క - 1 : 0, ఆదానం,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_దత్తాంశం,
			     lookup_context.intersects_cache);
}

స్థిర అంతఃవరుస శూన్య context_సంవృతం_lookup (hb_సంవృతం_context_t *c,
					   గుర్తులేని పూర్ణాంకం ఆదానం_లెక్క, /* Including the first glyph (not matched) */
					   glyph_దత్తాంశం_t ఆదానం, /* Array of input values--start with second glyph */
					   గుర్తులేని పూర్ణాంకం lookup_లెక్క_2,
					   స్థిరరాశి Lookup_రికార్డు lookup_రికార్డు[],
					   గుర్తులేని value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   Context_సంవృతం_Lookup_Context &lookup_context)
{
  ఒకవేళ (context_intersects (c->glyphs,
			  ఆదానం_లెక్క, ఆదానం,
			  lookup_context))
    context_సంవృతం_recurse_lookups (c,
				     ఆదానం_లెక్క, ఆదానం,
				     lookup_లెక్క_2, lookup_రికార్డు,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_దత్తాంశం,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

స్థిర అంతఃవరుస శూన్య context_depend_lookup (hb_depend_context_t *c,
					  గుర్తులేని పూర్ణాంకం ఆదానం_లెక్క, /* Including the first glyph (not matched) */
					  glyph_దత్తాంశం_t ఆదానం, /* Array of input values--start with second glyph */
					  గుర్తులేని పూర్ణాంకం lookup_లెక్క_2,
					  స్థిరరాశి Lookup_రికార్డు lookup_రికార్డు[],
					  స్థిరరాశి శూన్య *నియమం_2,
					  గుర్తులేని value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  ఒకవేళ (!context_intersects (c->glyphs,
			   ఆదానం_లెక్క, ఆదానం,
			   lookup_context))
    తిరిగి;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  ఒకవేళ (unlikely (!scratch))
    తిరిగి;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  తార్కిక context_sets_cached = context_depend_sets_are_cached (
    c, ఆదానం_లెక్క, lookup_లెక్క_2, lookup_రికార్డు, నియమం_2, value,
    lookup_context.context_format, lookup_context.intersects_దత్తాంశం,
    &scratch->cached_context_sets);
  ఒకవేళ (unlikely (!c->depend_దత్తాంశం->successful))
    తిరిగి;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_సమితి_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_సమితి_t &position_scratch = scratch->position_scratch;
  hb_సదిశ_t<స్థిరరాశి hb_సమితి_t *> &ఆదానం_position_glyphs = scratch->ఆదానం_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  ఒకవేళ (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    స్థిరరాశి hb_సమితి_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_దత్తాంశం, value,
      నిజం, &position_scratch, &scratch->compact_workspace);
    ఒకవేళ (unlikely (!pos0_glyphs ||
		  !ఆదానం_position_glyphs.నెట్టు_or_fail (pos0_glyphs)))
    {
      c->depend_దత్తాంశం->fail ();
      తిరిగి;
    }

    /* Positions 1+ (Input array) */
    కొరకు (గుర్తులేని i = 0; i + 1 < ఆదానం_లెక్క; i++)
    {
      స్థిరరాశి hb_సమితి_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_దత్తాంశం,
	ఆదానం[i], అబద్ధం, &position_scratch);
      ఒకవేళ (unlikely (!pos_glyphs ||
		    !ఆదానం_position_glyphs.నెట్టు_or_fail (pos_glyphs)))
      {
	c->depend_దత్తాంశం->fail ();
	తిరిగి;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  ఆదానం_లెక్క, ఆదానం,
				  lookup_లెక్క_2, lookup_రికార్డు,
				  నియమం_2,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_దత్తాంశం,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? శూన్యసూచిక : &ఆదానం_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : శూన్యసూచిక,
				  scratch);
}

స్థిర అంతఃవరుస శూన్య context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  గుర్తులేని పూర్ణాంకం ఆదానం_లెక్క, /* Including the first glyph (not matched) */
						  glyph_దత్తాంశం_t ఆదానం, /* Array of input values--start with second glyph */
						  గుర్తులేని పూర్ణాంకం lookup_లెక్క_2,
						  స్థిరరాశి Lookup_రికార్డు lookup_రికార్డు[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->ఆదానం,
		 ఆదానం_లెక్క ? ఆదానం_లెక్క - 1 : 0, ఆదానం,
		 lookup_context.funcs.collect, lookup_context.collect_దత్తాంశం);
  recurse_lookups (c,
		   lookup_లెక్క_2, lookup_రికార్డు);
}

స్థిర అంతఃవరుస తార్కిక context_would_apply_lookup (hb_would_apply_context_t *c,
					       గుర్తులేని పూర్ణాంకం ఆదానం_లెక్క, /* Including the first glyph (not matched) */
					       glyph_దత్తాంశం_t ఆదానం, /* Array of input values--start with second glyph */
					       గుర్తులేని పూర్ణాంకం lookup_లెక్క_2 HB_వాడని,
					       స్థిరరాశి Lookup_రికార్డు lookup_రికార్డు[] HB_వాడని,
					       స్థిరరాశి ContextApplyLookupContext &lookup_context)
{
  తిరిగి would_సరిపోలిక_ఆదానం (c,
			    ఆదానం_లెక్క, ఆదానం,
			    lookup_context.funcs.సరిపోలిక, lookup_context.సరిపోలిక_దత్తాంశం_2);
}

స్థిర అంతఃవరుస తార్కిక context_apply_lookup (hb_ot_apply_context_t *c,
					 గుర్తులేని పూర్ణాంకం ఆదానం_లెక్క, /* Including the first glyph (not matched) */
					 glyph_దత్తాంశం_t ఆదానం, /* Array of input values--start with second glyph */
					 గుర్తులేని పూర్ణాంకం lookup_లెక్క_2,
					 స్థిరరాశి Lookup_రికార్డు lookup_రికార్డు[],
					 స్థిరరాశి ContextApplyLookupContext &lookup_context)
{
  ఒకవేళ (unlikely (ఆదానం_లెక్క > HB_MAX_CONTEXT_నిడివి)) తిరిగి అబద్ధం;

  గుర్తులేని సరిపోలిక_చివర = 0;
  తార్కిక ret = అబద్ధం;
  ఒకవేళ (సరిపోలిక_ఆదానం (c,
		   ఆదానం_లెక్క, ఆదానం,
		   lookup_context.funcs.సరిపోలిక, lookup_context.సరిపోలిక_దత్తాంశం_2,
		   &సరిపోలిక_చివర))
  {
    c->బఫరు->unsafe_గమ్యం_break (c->బఫరు->idx, సరిపోలిక_చివర);
    apply_lookup (c,
		  ఆదానం_లెక్క,
		  lookup_లెక్క_2, lookup_రికార్డు,
		  సరిపోలిక_చివర);
    ret = నిజం;
  }
  లేకపోతే
  {
    c->బఫరు->unsafe_గమ్యం_concat (c->బఫరు->idx, సరిపోలిక_చివర);
    ret = అబద్ధం;
  }

  తిరిగి ret;
}

స్థిర అంతఃవరుస తార్కిక context_cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
{
  ఎంపిక (op)
  {
    స్థితి hb_ot_subtable_cache_op_t::ENTER:
    {
      ఒకవేళ (!HB_బఫరు_TRY_ALLOCATE_VAR (c->బఫరు, syllable))
	తిరిగి అబద్ధం;
      స్వయంచాలక &info = c->బఫరు->info;
      గుర్తులేని లెక్క = c->బఫరు->len;
      కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      తిరిగి నిజం;
    }
    స్థితి hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (గుర్తులేని) -1;
      HB_బఫరు_DEALLOCATE_VAR (c->బఫరు, syllable);
      ఆపు;
    }
  }
  తిరిగి అబద్ధం;
}

మూస <రకం_పేరు Types>
నిర్మాణం నియమం_3
{
  మూస <రకం_పేరు సమితి_Types, రకం_పేరు నియమం_Type>
  మిత్ర నిర్మాణం నియమం_సమితి_యొక్క;

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs, Context_సంవృతం_Lookup_Context &lookup_context) స్థిరరాశి
  {
    తిరిగి context_intersects (glyphs,
			       ఆదానం_లెక్క, ఆదానం_Z.arrayZ,
			       lookup_context);
  }

  శూన్య depend (hb_depend_context_t *c, గుర్తులేని value, ContextDependLookupContext &lookup_context) స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక &lookup_రికార్డు = StructAfter<Unsized_Array_యొక్క<Lookup_రికార్డు>>
					   (ఆదానం_Z.as_array ((ఆదానం_లెక్క ? ఆదానం_లెక్క - 1 : 0)));
    context_depend_lookup (c,
			   ఆదానం_లెక్క, ఆదానం_Z.arrayZ,
			   lookup_లెక్క_2, lookup_రికార్డు.arrayZ,
			   ఇది,
			   value, lookup_context);
  }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c, గుర్తులేని value, Context_సంవృతం_Lookup_Context &lookup_context) స్థిరరాశి
  {
    ఒకవేళ (unlikely (c->lookup_హద్దు_exceeded ())) తిరిగి;

    స్థిరరాశి స్వయంచాలక &lookup_రికార్డు = StructAfter<Unsized_Array_యొక్క<Lookup_రికార్డు>>
					   (ఆదానం_Z.as_array ((ఆదానం_లెక్క ? ఆదానం_లెక్క - 1 : 0)));
    context_సంవృతం_lookup (c,
			    ఆదానం_లెక్క, ఆదానం_Z.arrayZ,
			    lookup_లెక్క_2, lookup_రికార్డు.arrayZ,
			    value, lookup_context);
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c,
                        Context_సంవృతం_Lookup_Context &lookup_context) స్థిరరాశి
  {
    ఒకవేళ (unlikely (c->lookup_హద్దు_exceeded ())) తిరిగి;
    ఒకవేళ (!intersects (c->glyphs, lookup_context)) తిరిగి;

    స్థిరరాశి స్వయంచాలక &lookup_రికార్డు = StructAfter<Unsized_Array_యొక్క<Lookup_రికార్డు>>
					   (ఆదానం_Z.as_array (ఆదానం_లెక్క ? ఆదానం_లెక్క - 1 : 0));
    recurse_lookups (c, lookup_లెక్క_2, lookup_రికార్డు.arrayZ);
  }

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక &lookup_రికార్డు = StructAfter<Unsized_Array_యొక్క<Lookup_రికార్డు>>
					   (ఆదానం_Z.as_array (ఆదానం_లెక్క ? ఆదానం_లెక్క - 1 : 0));
    context_collect_glyphs_lookup (c,
				   ఆదానం_లెక్క, ఆదానం_Z.arrayZ,
				   lookup_లెక్క_2, lookup_రికార్డు.arrayZ,
				   lookup_context);
  }

  శూన్య collect_రెండవ_glyphs (hb_సమితి_digest_t *digest,
			      స్థిరరాశి ClassDef *class_def = శూన్యసూచిక) స్థిరరాశి
  {
    ఒకవేళ (ఆదానం_లెక్క <= 1)
    {
      *digest = hb_సమితి_digest_t::నిండిన ();
      తిరిగి;
    }

    గుర్తులేని second = ఆదానం_Z.arrayZ[0];
    ఒకవేళ (!class_def)
      digest->కూడు (second);
    లేకపోతే ఒకవేళ (!second)
      *digest = hb_సమితి_digest_t::నిండిన ();
    లేకపోతే
      class_def->collect_class (digest, second);
  }

  తార్కిక would_apply (hb_would_apply_context_t *c,
		    స్థిరరాశి ContextApplyLookupContext &lookup_context) స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక &lookup_రికార్డు = StructAfter<Unsized_Array_యొక్క<Lookup_రికార్డు>>
					   (ఆదానం_Z.as_array (ఆదానం_లెక్క ? ఆదానం_లెక్క - 1 : 0));
    తిరిగి context_would_apply_lookup (c,
				       ఆదానం_లెక్క, ఆదానం_Z.arrayZ,
				       lookup_లెక్క_2, lookup_రికార్డు.arrayZ,
				       lookup_context);
  }

  తార్కిక apply (hb_ot_apply_context_t *c,
	      స్థిరరాశి ContextApplyLookupContext &lookup_context) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    స్థిరరాశి స్వయంచాలక &lookup_రికార్డు = StructAfter<Unsized_Array_యొక్క<Lookup_రికార్డు>>
					   (ఆదానం_Z.as_array (ఆదానం_లెక్క ? ఆదానం_లెక్క - 1 : 0));
    return_trace (context_apply_lookup (c, ఆదానం_లెక్క, ఆదానం_Z.arrayZ, lookup_లెక్క_2, lookup_రికార్డు.arrayZ, lookup_context));
  }

  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి hb_map_t *ఆదానం_mapping, /* old->new glyphid or class mapping */
		  స్థిరరాశి hb_map_t *lookup_map) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక *బహిర్గతం = c->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    బహిర్గతం->ఆదానం_లెక్క = ఆదానం_లెక్క;
    స్థిరరాశి స్వయంచాలక ఆదానం = ఆదానం_Z.as_array (ఆదానం_లెక్క - 1);
    కొరకు (స్థిరరాశి స్వయంచాలక org : ఆదానం)
    {
      రకం_పేరు Types::HBUINT d;
      d = ఆదానం_mapping->get (org);
      c->copy (d);
    }

    స్థిరరాశి స్వయంచాలక &lookup_రికార్డు = StructAfter<Unsized_Array_యొక్క<Lookup_రికార్డు>>
					   (ఆదానం_Z.as_array ((ఆదానం_లెక్క ? ఆదానం_లెక్క - 1 : 0)));

    గుర్తులేని లెక్క = serialize_lookuprecord_array (c, lookup_రికార్డు.as_array (lookup_లెక్క_2), lookup_map);
    return_trace (c->check_కేటాయించు (బహిర్గతం->lookup_లెక్క_2, లెక్క, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  తార్కిక subset (hb_subset_context_t *c,
	       స్థిరరాశి hb_map_t *lookup_map,
	       స్థిరరాశి hb_map_t *klass_map = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (unlikely (!ఆదానం_లెక్క)) return_trace (అబద్ధం);
    స్థిరరాశి స్వయంచాలక ఆదానం = ఆదానం_Z.as_array (ఆదానం_లెక్క - 1);

    స్థిరరాశి hb_map_t *mapping = klass_map == శూన్యసూచిక ? c->plan->glyph_map : klass_map;
    ఒకవేళ (!hb_all (ఆదానం, mapping)) return_trace (అబద్ధం);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  బహిరంగ:
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  c->check_range (ఆదానం_Z.arrayZ,
				  ఆదానం_Z.అంశం_కొలత * (ఆదానం_లెక్క ? ఆదానం_లెక్క - 1 : 0) +
				  Lookup_రికార్డు::static_కొలత * lookup_లెక్క_2));
  }

  రక్షిత:
  HBUINT16	ఆదానం_లెక్క;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_లెక్క_2;		/* Number of LookupRecords */
  Unsized_Array_యొక్క<రకం_పేరు Types::HBUINT>
		ఆదానం_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, ఆదానం_Z);
};

మూస <రకం_పేరు Types, రకం_పేరు నియమం_Type>
నిర్మాణం నియమం_సమితి_యొక్క
{
  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs,
		   Context_సంవృతం_Lookup_Context &lookup_context) స్థిరరాశి
  {
    తిరిగి
    + hb_iter (నియమం_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_map ([&] (స్థిరరాశి నియమం_Type &_) { తిరిగి _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  శూన్య collect_మొదటి_ఆదానం_classes (hb_ot_layout_ruleset_digest_t *digest) స్థిరరాశి
  {
    digest->init ();
    కొరకు (గుర్తులేని i = 0; i < నియమం_2.len; i++)
    {
      స్థిరరాశి నియమం_Type &r = ఇది+నియమం_2.arrayZ[i];
      ఒకవేళ (r.ఆదానం_లెక్క <= 1)
      {
	digest->సమితి_నిండిన ();
	తిరిగి;
      }
      digest->కూడు (r.ఆదానం_Z.arrayZ[0]);
    }
  }

  శూన్య depend (hb_depend_context_t *c, గుర్తులేని value, ContextDependLookupContext &lookup_context) స్థిరరాశి
  {
    + hb_iter (నియమం_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([&] (స్థిరరాశి నియమం_Type &_) { _.depend (c, value, lookup_context); })
    ;
  }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c, గుర్తులేని value,
		Context_సంవృతం_Lookup_Context &lookup_context) స్థిరరాశి
  {
    ఒకవేళ (unlikely (c->lookup_హద్దు_exceeded ())) తిరిగి;

    తిరిగి
    + hb_iter (నియమం_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([&] (స్థిరరాశి నియమం_Type &_) { _.సంవృతం (c, value, lookup_context); })
    ;
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c,
                        Context_సంవృతం_Lookup_Context &lookup_context) స్థిరరాశి
  {
    ఒకవేళ (unlikely (c->lookup_హద్దు_exceeded ())) తిరిగి;
    + hb_iter (నియమం_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([&] (స్థిరరాశి నియమం_Type &_) { _.సంవృతం_lookups (c, lookup_context); })
    ;
  }

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) స్థిరరాశి
  {
    తిరిగి
    + hb_iter (నియమం_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([&] (స్థిరరాశి నియమం_Type &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  శూన్య collect_రెండవ_glyphs (hb_సమితి_digest_t *digest,
			      స్థిరరాశి ClassDef *class_def = శూన్యసూచిక) స్థిరరాశి
  {
    + hb_iter (నియమం_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([=] (స్థిరరాశి నియమం_Type &_) { _.collect_రెండవ_glyphs (digest, class_def); })
    ;
  }

  తార్కిక would_apply (hb_would_apply_context_t *c,
		    స్థిరరాశి ContextApplyLookupContext &lookup_context) స్థిరరాశి
  {
    తిరిగి
    + hb_iter (నియమం_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_map ([&] (స్థిరరాశి నియమం_Type &_) { తిరిగి _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  తార్కిక apply (hb_ot_apply_context_t *c,
	      స్థిరరాశి ContextApplyLookupContext &lookup_context,
	      స్థిరరాశి hb_ot_layout_ruleset_digest_t *digest = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    గుర్తులేని అంకె_నియమాలు = నియమం_2.len;

#నిర్వచితం_కాకపోతే HB_NO_OT_RULESETS_FAST_మార్గం
    ఒకవేళ (HB_OPTIMIZE_కొలత_VAL || అంకె_నియమాలు <= 4)
#అయితే_ముగింపు
    {
    slow:
      return_trace (
      + hb_iter (నియమం_2)
      | hb_map (hb_కూడు (ఇది))
      | hb_map ([&] (స్థిరరాశి నియమం_Type &_) { తిరిగి _.apply (c, lookup_context); })
      | hb_any
      )
      ;
    }

    /* This version is optimized for speed by matching the first & second
     * components of the rule here, instead of calling into the matching code.
     *
     * Replicated from LigatureSet::apply(). */

    /* We use the iter_context instead of iter_input, to avoid skipping
     * default-ignorables and such.
     *
     * Related: https://github.com/harfbuzz/harfbuzz/issues/4813
     */
    స్వయంచాలక &skippy_iter = c->iter_context;
    skippy_iter.reset (c->బఫరు->idx);
    skippy_iter.సమితి_సరిపోలిక_func (సరిపోలిక_always, శూన్యసూచిక);
    skippy_iter.సమితి_glyph_దత్తాంశం ((HBUINT16 *) శూన్యసూచిక);
    గుర్తులేని unsafe_గమ్యం = (గుర్తులేని) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *మొదటి = శూన్యసూచిక, *second = శూన్యసూచిక;
    తార్కిక matched = skippy_iter.next (&unsafe_to1);
    ఒకవేళ (likely (matched))
    {
      ఒకవేళ (skippy_iter.may_skip (c->బఫరు->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        వెళ్ళు slow;
      }

      మొదటి = &c->బఫరు->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      ఒకవేళ (digest && lookup_context.get_విలువ &&
	  !digest->may_have (lookup_context.get_విలువ (*మొదటి, lookup_context.సరిపోలిక_దత్తాంశం_2)))
	{
	  c->బఫరు->unsafe_గమ్యం_concat (c->బఫరు->idx, unsafe_to1);
	  return_trace (అబద్ధం);
	}
    }
    లేకపోతే
    {
      తార్కిక unsafe_గమ్యం_concat = అబద్ధం;
      తార్కిక ret =
      + hb_iter (నియమం_2)
      | hb_map (hb_కూడు (ఇది))
      | hb_filter ([&] (స్థిరరాశి నియమం_Type &_)
		   {
		     ఒకవేళ (_.ఆదానం_లెక్క <= 1) తిరిగి నిజం;
		     unsafe_గమ్యం_concat = నిజం;
		     తిరిగి అబద్ధం;
		   })
      | hb_map ([&] (స్థిరరాశి నియమం_Type &_) { తిరిగి _.apply (c, lookup_context); })
      | hb_any
      ;
      ఒకవేళ (unsafe_గమ్యం_concat)
	c->బఫరు->unsafe_గమ్యం_concat (c->బఫరు->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    ఒకవేళ (likely (matched))
    {
      ఒకవేళ (skippy_iter.may_skip (c->బఫరు->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        వెళ్ళు slow;
      }

      second = &c->బఫరు->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    స్వయంచాలక సరిపోలిక_ఆదానం = lookup_context.funcs.సరిపోలిక;
    స్వయంచాలక *ఆదానం_దత్తాంశం = lookup_context.సరిపోలిక_దత్తాంశం_2;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_నియమాలు; i++)
    {
      స్థిరరాశి స్వయంచాలక &r = ఇది+నియమం_2.arrayZ[i];

      స్థిరరాశి స్వయంచాలక &ఆదానం = r.ఆదానం_Z;

      ఒకవేళ (r.ఆదానం_లెక్క <= 1 ||
	  (!సరిపోలిక_ఆదానం ||
	   సరిపోలిక_ఆదానం (*మొదటి, ఆదానం.arrayZ[0], ఆదానం_దత్తాంశం)))
      {
        ఒకవేళ (!second ||
	    (r.ఆదానం_లెక్క <= 2 ||
	     (!సరిపోలిక_ఆదానం ||
	      సరిపోలిక_ఆదానం (*second, ఆదానం.arrayZ[1], ఆదానం_దత్తాంశం)))
	   )
	{
	  ఒకవేళ (r.apply (c, lookup_context))
	  {
	    ఒకవేళ (unsafe_గమ్యం != (గుర్తులేని) -1)
	      c->బఫరు->unsafe_గమ్యం_concat (c->బఫరు->idx, unsafe_గమ్యం);
	    return_trace (నిజం);
	  }
	}
	లేకపోతే
	  unsafe_గమ్యం = unsafe_to2;
      }
      లేకపోతే
      {
	ఒకవేళ (unsafe_గమ్యం == (గుర్తులేని) -1)
	  unsafe_గమ్యం = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	కొరకు (; i + 1 < అంకె_నియమాలు; i++)
	{
	  స్థిరరాశి స్వయంచాలక &r2 = ఇది+నియమం_2.arrayZ[i + 1];
	  స్థిరరాశి స్వయంచాలక &input2 = r2.ఆదానం_Z;
	  ఒకవేళ (r2.ఆదానం_లెక్క <= 1 || input2.arrayZ[0] != ఆదానం.arrayZ[0])
	    ఆపు;
	}
      }
    }
    ఒకవేళ (likely (unsafe_గమ్యం != (గుర్తులేని) -1))
      c->బఫరు->unsafe_గమ్యం_concat (c->బఫరు->idx, unsafe_గమ్యం);

    return_trace (అబద్ధం);
  }

  తార్కిక subset (hb_subset_context_t *c,
	       స్థిరరాశి hb_map_t *lookup_map,
	       స్థిరరాశి hb_map_t *klass_map = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);

    స్వయంచాలక snap = c->serializer->snapshot ();
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    కొరకు (స్థిరరాశి స్వయంచాలక& _ : నియమం_2)
    {
      ఒకవేళ (!_) కొనసాగు;
      స్వయంచాలక o_snap = c->serializer->snapshot ();
      స్వయంచాలక *o = బహిర్గతం->నియమం_2.serialize_append (c->serializer);
      ఒకవేళ (unlikely (!o)) కొనసాగు;

      ఒకవేళ (!o->serialize_subset (c, _, ఇది, lookup_map, klass_map))
      {
	బహిర్గతం->నియమం_2.తీయి ();
	c->serializer->revert (o_snap);
      }
    }

    తార్కిక ret = తార్కిక (బహిర్గతం->నియమం_2);
    ఒకవేళ (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (నియమం_2.sanitize (c, ఇది));
  }

  రక్షిత:
  Array16_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<నియమం_Type>>
		నియమం_2;			/* Array of Rule tables
					 * ordered by preference */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2, నియమం_2);
};

మూస <రకం_పేరు Types>
నిర్మాణం నియమం_సమితి : నియమం_సమితి_యొక్క<SmallTypes, నియమం_3<Types>> {};

మూస <రకం_పేరు Types>
నిర్మాణం Class_నియమం_సమితి : నియమం_సమితి_యొక్క<Types, నియమం_3<SmallTypes>> {};


మూస <రకం_పేరు Types>
నిర్మాణం ContextFormat1_4
{
  ఉపయోగించి నియమం_సమితి = OT::నియమం_సమితి<Types>;

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    నిర్మాణం Context_సంవృతం_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      శూన్యసూచిక
    };

    తిరిగి
    + hb_zip (ఇది+coverage, నియమం_సమితి_2)
    | hb_filter (*glyphs, hb_మొదటి)
    | hb_map (hb_రెండవ)
    | hb_map (hb_కూడు (ఇది))
    | hb_map ([&] (స్థిరరాశి నియమం_సమితి &_) { తిరిగి _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  తార్కిక may_have_non_1to1 () స్థిరరాశి
  { తిరిగి నిజం; }

  శూన్య depend (hb_depend_context_t *c) స్థిరరాశి
  {
    hb_సమితి_t* cur_చురుకైన_glyphs = c->నెట్టు_cur_చురుకైన_glyphs ();
    ఒకవేళ (unlikely (!cur_చురుకైన_glyphs)) తిరిగి;
    get_coverage ().intersect_సమితి (*c->glyphs, *cur_చురుకైన_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      శూన్యసూచిక,
      శూన్యసూచిక,  /* intersects_cache */
      శూన్యసూచిక   /* intersected_glyphs_cache */
    };

    + hb_zip (ఇది+coverage, hb_range ((గుర్తులేని) నియమం_సమితి_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      తిరిగి c->మాతృ_చురుకైన_glyphs ().has (_);
    }, hb_మొదటి)
    | hb_map ([&](స్థిరరాశి hb_pair_t<hb_codepoint_t, గుర్తులేని> _) { తిరిగి hb_pair_t<గుర్తులేని, స్థిరరాశి నియమం_సమితి&> (_.మొదటి, ఇది+నియమం_సమితి_2[_.second]); })
    | hb_apply ([&] (స్థిరరాశి hb_pair_t<గుర్తులేని, స్థిరరాశి నియమం_సమితి&>& _) { _.second.depend (c, _.మొదటి, lookup_context); })
    ;

    c->తీయి_cur_పూర్తి_glyphs ();
  }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c) స్థిరరాశి
  {
    hb_సమితి_t* cur_చురుకైన_glyphs = c->నెట్టు_cur_చురుకైన_glyphs ();
    ఒకవేళ (unlikely (!cur_చురుకైన_glyphs)) తిరిగి;
    get_coverage ().intersect_సమితి (c->మునుపటి_మాతృ_చురుకైన_glyphs (), *cur_చురుకైన_glyphs);

    నిర్మాణం Context_సంవృతం_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      శూన్యసూచిక
    };

    + hb_zip (ఇది+coverage, hb_range ((గుర్తులేని) నియమం_సమితి_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      తిరిగి c->మునుపటి_మాతృ_చురుకైన_glyphs ().has (_);
    }, hb_మొదటి)
    | hb_map ([&](స్థిరరాశి hb_pair_t<hb_codepoint_t, గుర్తులేని> _) { తిరిగి hb_pair_t<గుర్తులేని, స్థిరరాశి నియమం_సమితి&> (_.మొదటి, ఇది+నియమం_సమితి_2[_.second]); })
    | hb_apply ([&] (స్థిరరాశి hb_pair_t<గుర్తులేని, స్థిరరాశి నియమం_సమితి&>& _) { _.second.సంవృతం (c, _.మొదటి, lookup_context); })
    ;

    c->తీయి_cur_పూర్తి_glyphs ();
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి
  {
    నిర్మాణం Context_సంవృతం_Lookup_Context lookup_context = {
      {intersects_glyph, శూన్యసూచిక},
      ContextFormat::SimpleContext,
      శూన్యసూచిక
    };

    + hb_zip (ఇది+coverage, నియమం_సమితి_2)
    | hb_filter (*c->glyphs, hb_మొదటి)
    | hb_map (hb_రెండవ)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([&] (స్థిరరాశి నియమం_సమితి &_) { _.సంవృతం_lookups (c, lookup_context); })
    ;
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి {}

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    (ఇది+coverage).collect_coverage (c->ఆదానం);

    నిర్మాణం ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      శూన్యసూచిక
    };

    + hb_iter (నియమం_సమితి_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([&] (స్థిరరాశి నియమం_సమితి &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  శూన్య collect_రెండవ_glyphs (hb_సమితి_digest_t *digest) స్థిరరాశి
  {
    + hb_iter (నియమం_సమితి_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([digest] (స్థిరరాశి నియమం_సమితి &_) { _.collect_రెండవ_glyphs (digest); })
    ;
  }

  తార్కిక would_apply (hb_would_apply_context_t *c) స్థిరరాశి
  {
    స్థిరరాశి నియమం_సమితి &నియమం_సమితి_3 = ఇది+నియమం_సమితి_2[(ఇది+coverage).get_coverage (c->glyphs[0])];
    నిర్మాణం ContextApplyLookupContext lookup_context = {
      {సరిపోలిక_glyph},
      శూన్యసూచిక
    };
    తిరిగి నియమం_సమితి_3.would_apply (c, lookup_context);
  }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి { తిరిగి ఇది+coverage; }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage (c->బఫరు->cur().codepoint);
    ఒకవేళ (likely (సూచీ_2 == NOT_COVERED))
      return_trace (అబద్ధం);

    స్థిరరాశి నియమం_సమితి &నియమం_సమితి_3 = ఇది+నియమం_సమితి_2[సూచీ_2];
    నిర్మాణం ContextApplyLookupContext lookup_context = {
      {సరిపోలిక_glyph},
      శూన్యసూచిక
    };
    return_trace (నియమం_సమితి_3.apply (c, lookup_context));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
    బహిర్గతం->format = format;

    స్థిరరాశి hb_map_t *lookup_map = c->పట్టిక_ట్యాగ్_2 == HB_OT_ట్యాగ్_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_సదిశ_t<hb_codepoint_t> new_coverage;
    + hb_zip (ఇది+coverage, నియమం_సమితి_2)
    | hb_filter (glyphset, hb_మొదటి)
    | hb_filter (subset_స్థానభ్రంశం_array (c, బహిర్గతం->నియమం_సమితి_2, ఇది, lookup_map), hb_రెండవ)
    | hb_map (hb_మొదటి)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    బహిర్గతం->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (తార్కిక (new_coverage));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (coverage.sanitize (c, ఇది) && నియమం_సమితి_2.sanitize (c, ఇది));
  }

  రక్షిత:
  HBUINT16	format;			/* Format identifier--format = 1 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  రకం_పేరు Types::మూస Array_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<నియమం_సమితి>>
		నియమం_సమితి_2;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2 + Types::LOffset::static_కొలత + Types::HBUINT::static_కొలత, నియమం_సమితి_2);
};


మూస <రకం_పేరు Types>
నిర్మాణం ContextFormat2_5
{
  ఉపయోగించి నియమం_సమితి = OT::Class_నియమం_సమితి<Types>;

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    ఒకవేళ (!(ఇది+coverage).intersects (glyphs))
      తిరిగి అబద్ధం;

    స్థిరరాశి ClassDef &class_def = ఇది+classDef;

    hb_map_t cache;
    నిర్మాణం Context_సంవృతం_Lookup_Context lookup_context = {
      {intersects_class, శూన్యసూచిక},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_సమితి_t retained_coverage_glyphs;
    (ఇది+coverage).intersect_సమితి (*glyphs, retained_coverage_glyphs);

    hb_సమితి_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    తిరిగి
    + hb_iter (నియమం_సమితి_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_enumerate
    | hb_map ([&] (స్థిరరాశి hb_pair_t<గుర్తులేని, స్థిరరాశి నియమం_సమితి &> p)
	      { తిరిగి class_def.intersects_class (glyphs, p.మొదటి) &&
		       coverage_glyph_classes.has (p.మొదటి) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  తార్కిక may_have_non_1to1 () స్థిరరాశి
  { తిరిగి నిజం; }

  శూన్య depend (hb_depend_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (!(ఇది+coverage).intersects (c->glyphs))
      తిరిగి;

    hb_సమితి_t* cur_చురుకైన_glyphs = c->నెట్టు_cur_చురుకైన_glyphs ();
    ఒకవేళ (unlikely (!cur_చురుకైన_glyphs)) తిరిగి;
    get_coverage ().intersect_సమితి (*c->glyphs, *cur_చురుకైన_glyphs);

    స్థిరరాశి ClassDef &class_def = ఇది+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    ContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (నియమం_సమితి_2)
    | hb_filter ([&] (గుర్తులేని _)
    { తిరిగి class_def.intersects_class (&c->మాతృ_చురుకైన_glyphs (), _); },
		 hb_మొదటి)
    | hb_apply ([&] (స్థిరరాశి hb_pair_t<గుర్తులేని, స్థిరరాశి రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<నియమం_సమితి>&> _)
                {
                  స్థిరరాశి నియమం_సమితి& నియమం_సమితి_3 = ఇది+_.second;
                  నియమం_సమితి_3.depend (c, _.మొదటి, lookup_context);
                })
    ;

    c->తీయి_cur_పూర్తి_glyphs ();
  }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (!(ఇది+coverage).intersects (c->glyphs))
      తిరిగి;

    hb_సమితి_t* cur_చురుకైన_glyphs = c->నెట్టు_cur_చురుకైన_glyphs ();
    ఒకవేళ (unlikely (!cur_చురుకైన_glyphs)) తిరిగి;
    get_coverage ().intersect_సమితి (c->మునుపటి_మాతృ_చురుకైన_glyphs (),
				   *cur_చురుకైన_glyphs);

    స్థిరరాశి ClassDef &class_def = ఇది+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    నిర్మాణం Context_సంవృతం_Lookup_Context lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (నియమం_సమితి_2)
    | hb_filter ([&] (గుర్తులేని _)
    { తిరిగి class_def.intersects_class (&c->మాతృ_చురుకైన_glyphs (), _); },
		 hb_మొదటి)
    | hb_apply ([&] (స్థిరరాశి hb_pair_t<గుర్తులేని, స్థిరరాశి రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<నియమం_సమితి>&> _)
                {
                  స్థిరరాశి నియమం_సమితి& నియమం_సమితి_3 = ఇది+_.second;
                  నియమం_సమితి_3.సంవృతం (c, _.మొదటి, lookup_context);
                })
    ;

    c->తీయి_cur_పూర్తి_glyphs ();
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (!(ఇది+coverage).intersects (c->glyphs))
      తిరిగి;

    స్థిరరాశి ClassDef &class_def = ఇది+classDef;

    hb_map_t cache;
    నిర్మాణం Context_సంవృతం_Lookup_Context lookup_context = {
      {intersects_class, శూన్యసూచిక},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (నియమం_సమితి_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_enumerate
    | hb_filter ([&] (స్థిరరాశి hb_pair_t<గుర్తులేని, స్థిరరాశి నియమం_సమితి &> p)
    { తిరిగి class_def.intersects_class (c->glyphs, p.మొదటి); })
    | hb_map (hb_రెండవ)
    | hb_apply ([&] (స్థిరరాశి నియమం_సమితి & _)
    { _.సంవృతం_lookups (c, lookup_context); });
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి {}

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    (ఇది+coverage).collect_coverage (c->ఆదానం);

    స్థిరరాశి ClassDef &class_def = ఇది+classDef;
    నిర్మాణం ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (నియమం_సమితి_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([&] (స్థిరరాశి నియమం_సమితి &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  శూన్య collect_రెండవ_glyphs (hb_సమితి_digest_t *digest) స్థిరరాశి
  {
    స్థిరరాశి ClassDef &class_def = ఇది+classDef;
    + hb_iter (నియమం_సమితి_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([=, &class_def] (స్థిరరాశి నియమం_సమితి &_) { _.collect_రెండవ_glyphs (digest, &class_def); })
    ;
  }

  తార్కిక would_apply (hb_would_apply_context_t *c) స్థిరరాశి
  {
    స్థిరరాశి ClassDef &class_def = ఇది+classDef;
    గుర్తులేని పూర్ణాంకం సూచీ_2 = class_def.get_class (c->glyphs[0]);
    స్థిరరాశి నియమం_సమితి &నియమం_సమితి_3 = ఇది+నియమం_సమితి_2[సూచీ_2];
    నిర్మాణం ContextApplyLookupContext lookup_context = {
      {సరిపోలిక_class},
      &class_def
    };
    తిరిగి నియమం_సమితి_3.would_apply (c, lookup_context);
  }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి { తిరిగి ఇది+coverage; }

  గుర్తులేని cache_ఖర్చు () స్థిరరాశి
  {
    తిరిగి (ఇది+classDef).ఖర్చు ();
  }
  స్థిర తార్కిక cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    తిరిగి context_cache_func (c, op);
  }

  నిర్మాణం external_cache_t
  {
    hb_ot_layout_ద్విమాన_cache_t coverage;
    hb_ot_layout_ruleset_digest_t నియమం_sets[HB_VAR_ARRAY];
  };
  శూన్య *external_cache_create () స్థిరరాశి
  {
    గుర్తులేని లెక్క = నియమం_సమితి_2.len;
    గుర్తులేని కొలత = పరిమాణం (external_cache_t) -
		    HB_VAR_ARRAY * పరిమాణం (hb_ot_layout_ruleset_digest_t) +
		    లెక్క * పరిమాణం (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (కొలత);
    ఒకవేళ (likely (cache))
    {
      cache->coverage.clear ();
      కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
	(ఇది+నియమం_సమితి_2[i]).collect_మొదటి_ఆదానం_classes (&cache->నియమం_sets[i]);
    }
    తిరిగి cache;
  }
  తార్కిక apply_cached (hb_ot_apply_context_t *c, శూన్య *external_cache) స్థిరరాశి { తిరిగి _apply (c, నిజం, external_cache); }
  తార్కిక apply (hb_ot_apply_context_t *c, శూన్య *external_cache) స్థిరరాశి { తిరిగి _apply (c, అబద్ధం, external_cache); }
  తార్కిక _apply (hb_ot_apply_context_t *c, తార్కిక cached, శూన్య *external_cache) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage_ద్విమాన (c->బఫరు->cur().codepoint, cache ? &cache->coverage : శూన్యసూచిక);
#లేకపోతే
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage (c->బఫరు->cur().codepoint);
#అయితే_ముగింపు
    ఒకవేళ (సూచీ_2 == NOT_COVERED) return_trace (అబద్ధం);

    స్థిరరాశి ClassDef &class_def = ఇది+classDef;

    నిర్మాణం ContextApplyLookupContext lookup_context = {
      {cached ? సరిపోలిక_class_cached : సరిపోలిక_class},
      &class_def,
      cached ? get_class_cached_విలువ : get_class_విలువ
    };

    సూచీ_2 = cached ? get_class_cached (class_def, c->బఫరు->cur()) : class_def.get_class (c->బఫరు->cur().codepoint);
    స్థిరరాశి నియమం_సమితి &నియమం_సమితి_3 = ఇది+నియమం_సమితి_2[సూచీ_2];
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    స్థిరరాశి hb_ot_layout_ruleset_digest_t *digest = cache && సూచీ_2 < నియమం_సమితి_2.len ? &cache->నియమం_sets[సూచీ_2] : శూన్యసూచిక;
#లేకపోతే
    స్థిరరాశి hb_ot_layout_ruleset_digest_t *digest = శూన్యసూచిక;
#అయితే_ముగింపు
    return_trace (నియమం_సమితి_3.apply (c, lookup_context, digest));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
    బహిర్గతం->format = format;
    ఒకవేళ (unlikely (!బహిర్గతం->coverage.serialize_subset (c, coverage, ఇది)))
      return_trace (అబద్ధం);

    hb_map_t klass_map;
    బహిర్గతం->classDef.serialize_subset (c, classDef, ఇది, &klass_map);

    స్థిరరాశి hb_సమితి_t* glyphset = c->plan->glyphset_gsub ();
    hb_సమితి_t retained_coverage_glyphs;
    (ఇది+coverage).intersect_సమితి (*glyphset, retained_coverage_glyphs);

    hb_సమితి_t coverage_glyph_classes;
    (ఇది+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    స్థిరరాశి hb_map_t *lookup_map = c->పట్టిక_ట్యాగ్_2 == HB_OT_ట్యాగ్_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    తార్కిక ret = నిజం;
    పూర్ణాంకం non_zero_సూచీ = -1, సూచీ_2 = 0;
    స్వయంచాలక snapshot = c->serializer->snapshot();
    కొరకు (స్థిరరాశి స్వయంచాలక& _ : + hb_enumerate (నియమం_సమితి_2)
			 | hb_filter (klass_map, hb_మొదటి))
    {
      స్వయంచాలక *o = బహిర్గతం->నియమం_సమితి_2.serialize_append (c->serializer);
      ఒకవేళ (unlikely (!o))
      {
	ret = అబద్ధం;
	ఆపు;
      }

      ఒకవేళ (coverage_glyph_classes.has (_.మొదటి) &&
	  o->serialize_subset (c, _.second, ఇది, lookup_map, &klass_map)) {
	non_zero_సూచీ = సూచీ_2;
        snapshot = c->serializer->snapshot();
      }

      సూచీ_2++;
    }

    ఒకవేళ (!ret || non_zero_సూచీ == -1) return_trace (అబద్ధం);

    //prune empty trailing ruleSets
    --సూచీ_2;
    అయితే (సూచీ_2 > non_zero_సూచీ)
    {
      బహిర్గతం->నియమం_సమితి_2.తీయి ();
      సూచీ_2--;
    }
    c->serializer->revert (snapshot);

    return_trace (తార్కిక (బహిర్గతం->నియమం_సమితి_2));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (coverage.sanitize (c, ఇది) && classDef.sanitize (c, ఇది) && నియమం_సమితి_2.sanitize (c, ఇది));
  }

  రక్షిత:
  HBUINT16	format;			/* Format identifier--format = 2 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  రకం_పేరు Types::మూస LOffset_గమ్యం<ClassDef>
		classDef;		/* Offset to glyph ClassDef table--from
					 * beginning of table */
  రకం_పేరు Types::మూస Array_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<నియమం_సమితి>>
		నియమం_సమితి_2;		/* Array of RuleSet tables
					 * ordered by class */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2 + 2 * Types::LOffset::static_కొలత + Types::HBUINT::static_కొలత, నియమం_సమితి_2);
};


మూస <రకం_పేరు Types>
నిర్మాణం ContextFormat3_6
{
  ఉపయోగించి నియమం_సమితి = OT::నియమం_సమితి<SmallTypes>;

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    ఒకవేళ (!(ఇది+coverageZ[0]).intersects (glyphs))
      తిరిగి అబద్ధం;

    నిర్మాణం Context_సంవృతం_Lookup_Context lookup_context = {
      {intersects_coverage, శూన్యసూచిక},
      ContextFormat::CoverageBasedContext,
      ఇది
    };
    తిరిగి context_intersects (glyphs,
			       glyph_లెక్క_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  తార్కిక may_have_non_1to1 () స్థిరరాశి
  { తిరిగి నిజం; }

  శూన్య depend (hb_depend_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (!(ఇది+coverageZ[0]).intersects (c->glyphs))
      తిరిగి;

    hb_సమితి_t* cur_చురుకైన_glyphs = c->నెట్టు_cur_చురుకైన_glyphs ();
    ఒకవేళ (unlikely (!cur_చురుకైన_glyphs)) తిరిగి;
    get_coverage ().intersect_సమితి (*c->glyphs, *cur_చురుకైన_glyphs);

    స్థిరరాశి Lookup_రికార్డు *lookup_రికార్డు = &StructAfter<Lookup_రికార్డు> (coverageZ.as_array (glyph_లెక్క_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      ఇది,
      శూన్యసూచిక,  /* intersects_cache */
      శూన్యసూచిక   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_లెక్క_2, (స్థిరరాశి HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_లెక్క_2, lookup_రికార్డు,
			   ఇది,
			   coverageZ[0], lookup_context);

    c->తీయి_cur_పూర్తి_glyphs ();
  }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (!(ఇది+coverageZ[0]).intersects (c->glyphs))
      తిరిగి;

    hb_సమితి_t* cur_చురుకైన_glyphs = c->నెట్టు_cur_చురుకైన_glyphs ();
    ఒకవేళ (unlikely (!cur_చురుకైన_glyphs)) తిరిగి;
    get_coverage ().intersect_సమితి (c->మునుపటి_మాతృ_చురుకైన_glyphs (),
				   *cur_చురుకైన_glyphs);

    స్థిరరాశి Lookup_రికార్డు *lookup_రికార్డు = &StructAfter<Lookup_రికార్డు> (coverageZ.as_array (glyph_లెక్క_2));
    నిర్మాణం Context_సంవృతం_Lookup_Context lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      ఇది
    };
    context_సంవృతం_lookup (c,
			    glyph_లెక్క_2, coverageZ.arrayZ + 1,
			    lookup_లెక్క_2, lookup_రికార్డు,
			    0, lookup_context);

    c->తీయి_cur_పూర్తి_glyphs ();
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (!intersects (c->glyphs))
      తిరిగి;
    స్థిరరాశి Lookup_రికార్డు *lookup_రికార్డు = &StructAfter<Lookup_రికార్డు> (coverageZ.as_array (glyph_లెక్క_2));
    recurse_lookups (c, lookup_లెక్క_2, lookup_రికార్డు);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి {}

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    (ఇది+coverageZ[0]).collect_coverage (c->ఆదానం);

    స్థిరరాశి Lookup_రికార్డు *lookup_రికార్డు = &StructAfter<Lookup_రికార్డు> (coverageZ.as_array (glyph_లెక్క_2));
    నిర్మాణం ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      ఇది
    };

    context_collect_glyphs_lookup (c,
				   glyph_లెక్క_2, coverageZ.arrayZ + 1,
				   lookup_లెక్క_2, lookup_రికార్డు,
				   lookup_context);
  }

  శూన్య collect_రెండవ_glyphs (hb_సమితి_digest_t *digest) స్థిరరాశి
  {
    ఒకవేళ (glyph_లెక్క_2 <= 1)
      *digest = hb_సమితి_digest_t::నిండిన ();
    లేకపోతే
      (ఇది+coverageZ[1]).collect_coverage (digest);
  }

  తార్కిక would_apply (hb_would_apply_context_t *c) స్థిరరాశి
  {
    స్థిరరాశి Lookup_రికార్డు *lookup_రికార్డు = &StructAfter<Lookup_రికార్డు> (coverageZ.as_array (glyph_లెక్క_2));
    నిర్మాణం ContextApplyLookupContext lookup_context = {
      {సరిపోలిక_coverage},
      ఇది
    };
    తిరిగి context_would_apply_lookup (c,
				       glyph_లెక్క_2, coverageZ.arrayZ + 1,
				       lookup_లెక్క_2, lookup_రికార్డు,
				       lookup_context);
  }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి { తిరిగి ఇది+coverageZ[0]; }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverageZ[0]).get_coverage (c->బఫరు->cur().codepoint);
    ఒకవేళ (సూచీ_2 == NOT_COVERED) return_trace (అబద్ధం);

    స్థిరరాశి Lookup_రికార్డు *lookup_రికార్డు = &StructAfter<Lookup_రికార్డు> (coverageZ.as_array (glyph_లెక్క_2));
    నిర్మాణం ContextApplyLookupContext lookup_context = {
      {సరిపోలిక_coverage},
      ఇది
    };
    return_trace (context_apply_lookup (c,
					glyph_లెక్క_2,
					coverageZ.arrayZ + 1,
					lookup_లెక్క_2, lookup_రికార్డు,
					lookup_context));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    బహిర్గతం->format = format;
    బహిర్గతం->glyph_లెక్క_2 = glyph_లెక్క_2;

    స్వయంచాలక coverages = coverageZ.as_array (glyph_లెక్క_2);

    కొరకు (స్థిరరాశి స్వయంచాలక& స్థానభ్రంశం : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      స్వయంచాలక *o = c->serializer->allocate_కొలత<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Coverage>> (Types::స్థానభ్రంశం_2::static_కొలత);
      ఒకవేళ (unlikely (!o)) return_trace (అబద్ధం);
      ఒకవేళ (!o->serialize_subset (c, స్థానభ్రంశం, ఇది)) return_trace (అబద్ధం);
    }

    స్థిరరాశి స్వయంచాలక& lookup_రికార్డు = StructAfter<Unsized_Array_యొక్క<Lookup_రికార్డు>> (coverageZ.as_array (glyph_లెక్క_2));
    స్థిరరాశి hb_map_t *lookup_map = c->పట్టిక_ట్యాగ్_2 == HB_OT_ట్యాగ్_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    గుర్తులేని లెక్క = serialize_lookuprecord_array (c->serializer, lookup_రికార్డు.as_array (lookup_లెక్క_2), lookup_map);
    return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->lookup_లెక్క_2, లెక్క, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!c->check_struct (ఇది))) return_trace (అబద్ధం);
    hb_barrier ();
    గుర్తులేని పూర్ణాంకం లెక్క = glyph_లెక్క_2;
    ఒకవేళ (unlikely (!లెక్క)) return_trace (అబద్ధం); /* We want to access coverageZ[0] freely. */
    ఒకవేళ (unlikely (!c->check_array (coverageZ.arrayZ, లెక్క))) return_trace (అబద్ధం);
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (unlikely (!coverageZ[i].sanitize (c, ఇది))) return_trace (అబద్ధం);
    స్థిరరాశి Lookup_రికార్డు *lookup_రికార్డు = &StructAfter<Lookup_రికార్డు> (coverageZ.as_array (glyph_లెక్క_2));
    return_trace (likely (c->check_array (lookup_రికార్డు, lookup_లెక్క_2)));
  }

  రక్షిత:
  HBUINT16	format;			/* Format identifier--format = 3 */
  HBUINT16	glyph_లెక్క_2;		/* Number of glyphs in the input glyph
					 * sequence */
  HBUINT16	lookup_లెక్క_2;		/* Number of LookupRecords */
  Unsized_Array_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Coverage>>
		coverageZ;		/* Array of offsets to Coverage
					 * table in glyph sequence order */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (6, coverageZ);
};

నిర్మాణం Context
{
  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, Ts&&... ds) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!c->may_dispatch (ఇది, &u.format.v))) తిరిగి c->no_dispatch_return_విలువ ();
    TRACE_DISPATCH (ఇది, u.format.v);
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    స్థితి 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    స్థితి 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    స్థితి 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    స్థితి 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#అయితే_ముగింపు
    అప్రమేయ:return_trace (c->default_return_విలువ ());
    }
  }

  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT16 v; }	format;		/* Format identifier */
  ContextFormat1_4<SmallTypes>	format1;
  ContextFormat2_5<SmallTypes>	format2;
  ContextFormat3_6<SmallTypes>	format3;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  ContextFormat1_4<MediumTypes>	format4;
  ContextFormat2_5<MediumTypes>	format5;
  ContextFormat3_6<MediumTypes>	format6;
#అయితే_ముగింపు
  } u;
};


/* Chaining Contextual lookups */

నిర్మాణం Chain_Context_సంవృతం_Lookup_Context
{
  Context_సంవృతం_Funcs funcs;
  ContextFormat context_format;
  స్థిరరాశి శూన్య *intersects_దత్తాంశం[3];
  శూన్య *intersects_cache[3];
  శూన్య *intersected_glyphs_cache;
};

రకనిర్వచనం Chain_Context_సంవృతం_Lookup_Context ChainContextDependLookupContext;

నిర్మాణం ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  స్థిరరాశి శూన్య *collect_దత్తాంశం[3];
};

నిర్మాణం ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  స్థిరరాశి శూన్య *సరిపోలిక_దత్తాంశం_2[3];
  get_విలువ_func_t get_ఆదానం_విలువ;
};

స్థిర అంతఃవరుస తార్కిక chain_context_intersects (స్థిరరాశి hb_సమితి_t *glyphs,
					     గుర్తులేని పూర్ణాంకం backtrack_లెక్క,
					     glyph_దత్తాంశం_t backtrack,
					     గుర్తులేని పూర్ణాంకం ఆదానం_లెక్క, /* Including the first glyph (not matched) */
					     glyph_దత్తాంశం_t ఆదానం, /* Array of input values--start with second glyph */
					     గుర్తులేని పూర్ణాంకం lookahead_లెక్క,
					     glyph_దత్తాంశం_t lookahead,
					     Chain_Context_సంవృతం_Lookup_Context &lookup_context)
{
  తార్కిక bt_ok = array_is_subset_యొక్క (glyphs,
			     backtrack_లెక్క, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_దత్తాంశం[0],
			     lookup_context.intersects_cache[0]);
  తార్కిక లోపల_ok = array_is_subset_యొక్క (glyphs,
			     ఆదానం_లెక్క ? ఆదానం_లెక్క - 1 : 0, ఆదానం,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_దత్తాంశం[1],
			     lookup_context.intersects_cache[1]);
  తార్కిక la_ok = array_is_subset_యొక్క (glyphs,
			     lookahead_లెక్క, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_దత్తాంశం[2],
			     lookup_context.intersects_cache[2]);

  తిరిగి bt_ok && లోపల_ok && la_ok;
}

స్థిర అంతఃవరుస శూన్య chain_context_సంవృతం_lookup (hb_సంవృతం_context_t *c,
						 గుర్తులేని పూర్ణాంకం backtrack_లెక్క,
						 glyph_దత్తాంశం_t backtrack,
						 గుర్తులేని పూర్ణాంకం ఆదానం_లెక్క, /* Including the first glyph (not matched) */
						 glyph_దత్తాంశం_t ఆదానం, /* Array of input values--start with second glyph */
						 గుర్తులేని పూర్ణాంకం lookahead_లెక్క,
						 glyph_దత్తాంశం_t lookahead,
						 గుర్తులేని పూర్ణాంకం lookup_లెక్క_2,
						 స్థిరరాశి Lookup_రికార్డు lookup_రికార్డు[],
						 గుర్తులేని value,
						 Chain_Context_సంవృతం_Lookup_Context &lookup_context)
{
  ఒకవేళ (chain_context_intersects (c->glyphs,
				backtrack_లెక్క, backtrack,
				ఆదానం_లెక్క, ఆదానం,
				lookahead_లెక్క, lookahead,
				lookup_context))
    context_సంవృతం_recurse_lookups (c,
		     ఆదానం_లెక్క, ఆదానం,
		     lookup_లెక్క_2, lookup_రికార్డు,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_దత్తాంశం[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

స్థిర అంతఃవరుస శూన్య chain_context_depend_lookup (hb_depend_context_t *c,
						గుర్తులేని పూర్ణాంకం backtrack_లెక్క,
						glyph_దత్తాంశం_t backtrack,
						గుర్తులేని పూర్ణాంకం ఆదానం_లెక్క, /* Including the first glyph (not matched) */
						glyph_దత్తాంశం_t ఆదానం, /* Array of input values--start with second glyph */
						గుర్తులేని పూర్ణాంకం lookahead_లెక్క,
						glyph_దత్తాంశం_t lookahead,
						గుర్తులేని పూర్ణాంకం lookup_లెక్క_2,
						స్థిరరాశి Lookup_రికార్డు lookup_రికార్డు[],
						స్థిరరాశి శూన్య *నియమం_2,
						గుర్తులేని value,
						ChainContextDependLookupContext &lookup_context)
{
  ఒకవేళ (!chain_context_intersects (c->glyphs,
				 backtrack_లెక్క, backtrack,
				 ఆదానం_లెక్క, ఆదానం,
				 lookahead_లెక్క, lookahead,
				 lookup_context))
    తిరిగి;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  ఒకవేళ (unlikely (!scratch))
    తిరిగి;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  తార్కిక context_sets_cached = context_depend_sets_are_cached (
    c, ఆదానం_లెక్క, lookup_లెక్క_2, lookup_రికార్డు, నియమం_2, value,
    lookup_context.context_format, lookup_context.intersects_దత్తాంశం[1],
    &scratch->cached_context_sets);
  ఒకవేళ (unlikely (!c->depend_దత్తాంశం->successful))
    తిరిగి;
  ఒకవేళ (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    ఆదానం_లెక్క, ఆదానం,
				    lookup_లెక్క_2, lookup_రికార్డు,
				    నియమం_2,
				    value,
				    lookup_context.context_format,
				    lookup_context.intersects_దత్తాంశం[1],
				    lookup_context.funcs.intersected_glyphs,
				    lookup_context.intersected_glyphs_cache,
				    scratch->preliminary_context,
				    శూన్యసూచిక,
				    &scratch->cached_context_sets,
				    scratch);
    తిరిగి;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_సమితి_t &position_scratch = scratch->position_scratch;
  hb_సదిశ_t<స్థిరరాశి hb_సమితి_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_సదిశ_t<స్థిరరాశి hb_సమితి_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  కొరకు (గుర్తులేని i = 0; i < backtrack_లెక్క; i++)
  {
    స్థిరరాశి hb_సమితి_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_దత్తాంశం[0],
      backtrack[i], అబద్ధం, &position_scratch);
    ఒకవేళ (unlikely (!pos_glyphs))
      తిరిగి;
    ఒకవేళ (!pos_glyphs->is_empty ())
    {
      ఒకవేళ (unlikely (!backtrack_sets.నెట్టు_or_fail (pos_glyphs)))
      {
        c->depend_దత్తాంశం->fail ();
        తిరిగి;
      }
    }
  }

  /* Extract lookahead glyphs */
  కొరకు (గుర్తులేని i = 0; i < lookahead_లెక్క; i++)
  {
    స్థిరరాశి hb_సమితి_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_దత్తాంశం[2],
      lookahead[i], అబద్ధం, &position_scratch);
    ఒకవేళ (unlikely (!pos_glyphs))
      తిరిగి;
    ఒకవేళ (!pos_glyphs->is_empty ())
    {
      ఒకవేళ (unlikely (!lookahead_sets.నెట్టు_or_fail (pos_glyphs)))
      {
        c->depend_దత్తాంశం->fail ();
        తిరిగి;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_సమితి_t &preliminary_context = scratch->preliminary_context;
  కొరకు (స్థిరరాశి hb_సమితి_t *వెనుక_సమితి : backtrack_sets)
  {
    ఒకవేళ (వెనుక_సమితి->get_population () == 1) {
      preliminary_context.కూడు (వెనుక_సమితి->get_min ());  // Direct glyph
    } లేకపోతే ఒకవేళ (వెనుక_సమితి->get_population () > 1) {
      hb_codepoint_t సమితి_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_దత్తాంశం->కనుగొను_or_create_context_సమితి (*వెనుక_సమితి)
        : c->కనుగొను_or_create_cached_context_సమితి (వెనుక_సమితి);
      ఒకవేళ (unlikely (సమితి_idx == HB_CODEPOINT_INVALID))
        తిరిగి;
      preliminary_context.కూడు (HB_DEPEND_CONTEXT_సమితి_FLAG | సమితి_idx);  // Encoded set reference
    }
  }
  కొరకు (స్థిరరాశి hb_సమితి_t *look_సమితి : lookahead_sets)
  {
    ఒకవేళ (look_సమితి->get_population () == 1) {
      preliminary_context.కూడు (look_సమితి->get_min ());  // Direct glyph
    } లేకపోతే ఒకవేళ (look_సమితి->get_population () > 1) {
      hb_codepoint_t సమితి_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_దత్తాంశం->కనుగొను_or_create_context_సమితి (*look_సమితి)
        : c->కనుగొను_or_create_cached_context_సమితి (look_సమితి);
      ఒకవేళ (unlikely (సమితి_idx == HB_CODEPOINT_INVALID))
        తిరిగి;
      preliminary_context.కూడు (HB_DEPEND_CONTEXT_సమితి_FLAG | సమితి_idx);  // Encoded set reference
    }
  }
  ఒకవేళ (unlikely (preliminary_context.లోపల_error ()))
  {
    c->depend_దత్తాంశం->fail ();
    తిరిగి;
  }

  /* Build glyph sets for ALL input positions */
  hb_సదిశ_t<స్థిరరాశి hb_సమితి_t *> &ఆదానం_position_glyphs = scratch->ఆదానం_position_glyphs;

  ఒకవేళ (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    స్థిరరాశి hb_సమితి_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_దత్తాంశం[1], value,
      నిజం, &position_scratch, &scratch->compact_workspace);
    ఒకవేళ (unlikely (!pos0_glyphs ||
		  !ఆదానం_position_glyphs.నెట్టు_or_fail (pos0_glyphs)))
    {
      c->depend_దత్తాంశం->fail ();
      తిరిగి;
    }

    /* Positions 1+ (Input array) */
    కొరకు (గుర్తులేని i = 0; i + 1 < ఆదానం_లెక్క; i++)
    {
      స్థిరరాశి hb_సమితి_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_దత్తాంశం[1],
	ఆదానం[i], అబద్ధం, &position_scratch);
      ఒకవేళ (unlikely (!pos_glyphs ||
		    !ఆదానం_position_glyphs.నెట్టు_or_fail (pos_glyphs)))
      {
	c->depend_దత్తాంశం->fail ();
	తిరిగి;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  ఆదానం_లెక్క, ఆదానం,
				  lookup_లెక్క_2, lookup_రికార్డు,
				  నియమం_2,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_దత్తాంశం[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? శూన్యసూచిక : &ఆదానం_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : శూన్యసూచిక,
				  scratch);
}

స్థిర అంతఃవరుస శూన్య chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							గుర్తులేని పూర్ణాంకం backtrack_లెక్క,
							glyph_దత్తాంశం_t backtrack,
							గుర్తులేని పూర్ణాంకం ఆదానం_లెక్క, /* Including the first glyph (not matched) */
							glyph_దత్తాంశం_t ఆదానం, /* Array of input values--start with second glyph */
							గుర్తులేని పూర్ణాంకం lookahead_లెక్క,
							glyph_దత్తాంశం_t lookahead,
							గుర్తులేని పూర్ణాంకం lookup_లెక్క_2,
							స్థిరరాశి Lookup_రికార్డు lookup_రికార్డు[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_లెక్క, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_దత్తాంశం[0]);
  collect_array (c, c->ఆదానం,
		 ఆదానం_లెక్క ? ఆదానం_లెక్క - 1 : 0, ఆదానం,
		 lookup_context.funcs.collect, lookup_context.collect_దత్తాంశం[1]);
  collect_array (c, c->after,
		 lookahead_లెక్క, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_దత్తాంశం[2]);
  recurse_lookups (c,
		   lookup_లెక్క_2, lookup_రికార్డు);
}

స్థిర అంతఃవరుస తార్కిక chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     గుర్తులేని పూర్ణాంకం backtrack_లెక్క,
						     glyph_దత్తాంశం_t backtrack HB_వాడని,
						     గుర్తులేని పూర్ణాంకం ఆదానం_లెక్క, /* Including the first glyph (not matched) */
						     glyph_దత్తాంశం_t ఆదానం, /* Array of input values--start with second glyph */
						     గుర్తులేని పూర్ణాంకం lookahead_లెక్క,
						     glyph_దత్తాంశం_t lookahead HB_వాడని,
						     గుర్తులేని పూర్ణాంకం lookup_లెక్క_2 HB_వాడని,
						     స్థిరరాశి Lookup_రికార్డు lookup_రికార్డు[] HB_వాడని,
						     స్థిరరాశి ChainContextApplyLookupContext &lookup_context)
{
  తిరిగి (c->zero_context ? !backtrack_లెక్క && !lookahead_లెక్క : నిజం)
      && would_సరిపోలిక_ఆదానం (c,
			    ఆదానం_లెక్క, ఆదానం,
			    lookup_context.funcs.సరిపోలిక[1], lookup_context.సరిపోలిక_దత్తాంశం_2[1]);
}

స్థిర అంతఃవరుస తార్కిక chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       గుర్తులేని పూర్ణాంకం backtrack_లెక్క,
					       glyph_దత్తాంశం_t backtrack,
					       గుర్తులేని పూర్ణాంకం ఆదానం_లెక్క, /* Including the first glyph (not matched) */
					       glyph_దత్తాంశం_t ఆదానం, /* Array of input values--start with second glyph */
					       గుర్తులేని పూర్ణాంకం lookahead_లెక్క,
					       glyph_దత్తాంశం_t lookahead,
					       గుర్తులేని పూర్ణాంకం lookup_లెక్క_2,
					       స్థిరరాశి Lookup_రికార్డు lookup_రికార్డు[],
					       స్థిరరాశి ChainContextApplyLookupContext &lookup_context)
{
  ఒకవేళ (unlikely (ఆదానం_లెక్క > HB_MAX_CONTEXT_నిడివి)) తిరిగి అబద్ధం;

  గుర్తులేని ప్రారంభం_సూచీ = c->బఫరు->బహిర్గతం_len;
  గుర్తులేని చివర_సూచీ = c->బఫరు->idx;
  గుర్తులేని సరిపోలిక_చివర = 0;
  తార్కిక ret = నిజం;
  ఒకవేళ (!(సరిపోలిక_ఆదానం (c,
		     ఆదానం_లెక్క, ఆదానం,
		     lookup_context.funcs.సరిపోలిక[1], lookup_context.సరిపోలిక_దత్తాంశం_2[1],
		     &సరిపోలిక_చివర) && (చివర_సూచీ = సరిపోలిక_చివర)
       && సరిపోలిక_lookahead (c,
			   lookahead_లెక్క, lookahead,
			   lookup_context.funcs.సరిపోలిక[2], lookup_context.సరిపోలిక_దత్తాంశం_2[2],
			   సరిపోలిక_చివర, &చివర_సూచీ)))
  {
    c->బఫరు->unsafe_గమ్యం_concat (c->బఫరు->idx, చివర_సూచీ);
    తిరిగి అబద్ధం;
  }

  ఒకవేళ (!సరిపోలిక_backtrack (c,
			backtrack_లెక్క, backtrack,
			lookup_context.funcs.సరిపోలిక[0], lookup_context.సరిపోలిక_దత్తాంశం_2[0],
			&ప్రారంభం_సూచీ))
  {
    c->బఫరు->unsafe_గమ్యం_concat_from_outbuffer (ప్రారంభం_సూచీ, చివర_సూచీ);
    తిరిగి అబద్ధం;
  }

  c->బఫరు->unsafe_గమ్యం_break_from_outbuffer (ప్రారంభం_సూచీ, చివర_సూచీ);
  apply_lookup (c,
		ఆదానం_లెక్క,
		lookup_లెక్క_2, lookup_రికార్డు,
		సరిపోలిక_చివర);

  తిరిగి ret;
}

మూస <రకం_పేరు Types>
నిర్మాణం Chain_నియమం
{
  మూస <రకం_పేరు సమితి_Types, రకం_పేరు Chain_నియమం_Type>
  మిత్ర నిర్మాణం Chain_నియమం_సమితి_యొక్క;

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs, Chain_Context_సంవృతం_Lookup_Context &lookup_context) స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);
    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    తిరిగి chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     ఆదానం.lenP1, ఆదానం.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  శూన్య depend (hb_depend_context_t *c, గుర్తులేని value, ChainContextDependLookupContext &lookup_context) స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);
    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    స్థిరరాశి స్వయంచాలక &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 ఆదానం.lenP1, ఆదానం.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 ఇది,
				 value,
				 lookup_context);
  }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c, గుర్తులేని value,
		Chain_Context_సంవృతం_Lookup_Context &lookup_context) స్థిరరాశి
  {
    ఒకవేళ (unlikely (c->lookup_హద్దు_exceeded ())) తిరిగి;

    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);
    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    స్థిరరాశి స్వయంచాలక &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_సంవృతం_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  ఆదానం.lenP1, ఆదానం.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c,
                        Chain_Context_సంవృతం_Lookup_Context &lookup_context) స్థిరరాశి
  {
    ఒకవేళ (unlikely (c->lookup_హద్దు_exceeded ())) తిరిగి;
    ఒకవేళ (!intersects (c->glyphs, lookup_context)) తిరిగి;

    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);
    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    స్థిరరాశి స్వయంచాలక &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);
    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    స్థిరరాశి స్వయంచాలక &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 ఆదానం.lenP1, ఆదానం.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  శూన్య collect_రెండవ_glyphs (hb_సమితి_digest_t *digest,
			      స్థిరరాశి ClassDef *ఆదానం_class_def = శూన్యసూచిక,
			      స్థిరరాశి ClassDef *lookahead_class_def = శూన్యసూచిక) స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);
    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);

    గుర్తులేని second;
    స్థిరరాశి ClassDef *class_def;
    ఒకవేళ (ఆదానం.lenP1 > 1)
    {
      second = ఆదానం.arrayZ[0];
      class_def = ఆదానం_class_def;
    }
    లేకపోతే ఒకవేళ (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    లేకపోతే
    {
      *digest = hb_సమితి_digest_t::నిండిన ();
      తిరిగి;
    }

    ఒకవేళ (!class_def)
      digest->కూడు (second);
    లేకపోతే ఒకవేళ (!second)
      *digest = hb_సమితి_digest_t::నిండిన ();
    లేకపోతే
      class_def->collect_class (digest, second);
  }

  తార్కిక would_apply (hb_would_apply_context_t *c,
		    స్థిరరాశి ChainContextApplyLookupContext &lookup_context) స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);
    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    స్థిరరాశి స్వయంచాలక &lookup = StructAfter<decltype (lookupX)> (lookahead);
    తిరిగి chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     ఆదానం.lenP1, ఆదానం.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  తార్కిక apply (hb_ot_apply_context_t *c,
	      స్థిరరాశి ChainContextApplyLookupContext &lookup_context) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);
    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    స్థిరరాశి స్వయంచాలక &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      ఆదానం.lenP1, ఆదానం.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  మూస<రకం_పేరు Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  శూన్య serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) స్థిరరాశి
  {
    c->copy (len);
    కొరకు (స్థిరరాశి స్వయంచాలక g : it)
    {
      రకం_పేరు Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి hb_map_t *lookup_map,
		  స్థిరరాశి hb_map_t *backtrack_map,
		  స్థిరరాశి hb_map_t *ఆదానం_map = శూన్యసూచిక,
		  స్థిరరాశి hb_map_t *lookahead_map = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);

    స్థిరరాశి hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);
    ఒకవేళ (ఆదానం_map) mapping = ఆదానం_map;
    serialize_array (c, ఆదానం.lenP1, + ఆదానం.iter ()
				     | hb_map (mapping));

    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    ఒకవేళ (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    స్థిరరాశి స్వయంచాలక &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_లెక్క_2 = c->embed (&(lookup.len));
    ఒకవేళ (!lookup_లెక్క_2) return_trace (అబద్ధం);

    గుర్తులేని లెక్క = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_కేటాయించు (*lookup_లెక్క_2, లెక్క, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  తార్కిక subset (hb_subset_context_t *c,
	       స్థిరరాశి hb_map_t *lookup_map,
	       స్థిరరాశి hb_map_t *backtrack_map = శూన్యసూచిక,
	       స్థిరరాశి hb_map_t *ఆదానం_map = శూన్యసూచిక,
	       స్థిరరాశి hb_map_t *lookahead_map = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);

    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);
    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);

    ఒకవేళ (!backtrack_map)
    {
      స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
      ఒకవేళ (!hb_all (backtrack, glyphset) ||
	  !hb_all (ఆదానం, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (అబద్ధం);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    లేకపోతే
    {
      ఒకవేళ (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (ఆదానం, ఆదానం_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (అబద్ధం);

      serialize (c->serializer, lookup_map, backtrack_map, ఆదానం_map, lookahead_map);
    }

    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    /* Hyper-optimized sanitized because this is really hot. */
    ఒకవేళ (unlikely (!backtrack.len.sanitize (c))) return_trace (అబద్ధం);
    hb_barrier ();
    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);
    ఒకవేళ (unlikely (!ఆదానం.lenP1.sanitize (c))) return_trace (అబద్ధం);
    hb_barrier ();
    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    ఒకవేళ (unlikely (!lookahead.len.sanitize (c))) return_trace (అబద్ధం);
    hb_barrier ();
    స్థిరరాశి స్వయంచాలక &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  రక్షిత:
  Array16_యొక్క<రకం_పేరు Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  Headless_Array16_యొక్క<రకం_పేరు Types::HBUINT>
		ఆదానం_X;			/* Array of input values (start with
					 * second glyph) */
  Array16_యొక్క<రకం_పేరు Types::HBUINT>
		lookaheadX;		/* Array of lookahead values's (to be
					 * matched after the input sequence) */
  Array16_యొక్క<Lookup_రికార్డు>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  బహిరంగ:
  DEFINE_కొలత_MIN (8);
};

మూస <రకం_పేరు Types, రకం_పేరు Chain_నియమం_Type>
నిర్మాణం Chain_నియమం_సమితి_యొక్క
{
  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs, Chain_Context_సంవృతం_Lookup_Context &lookup_context) స్థిరరాశి
  {
    తిరిగి
    + hb_iter (నియమం_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_map ([&] (స్థిరరాశి Chain_నియమం_Type &_) { తిరిగి _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  శూన్య collect_మొదటి_ఆదానం_classes (hb_ot_layout_ruleset_digest_t *digest) స్థిరరాశి
  {
    digest->init ();
    కొరకు (గుర్తులేని i = 0; i < నియమం_2.len; i++)
    {
      స్థిరరాశి Chain_నియమం_Type &r = ఇది+నియమం_2.arrayZ[i];
      స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (r.ఆదానం_X)> (r.backtrack);
      ఒకవేళ (ఆదానం.lenP1 <= 1)
      {
	digest->సమితి_నిండిన ();
	తిరిగి;
      }
      digest->కూడు (ఆదానం.arrayZ[0]);
    }
  }
  శూన్య depend (hb_depend_context_t *c, గుర్తులేని value, ChainContextDependLookupContext &lookup_context) స్థిరరాశి
  {
    + hb_iter (నియమం_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([&] (స్థిరరాశి Chain_నియమం_Type &_) { _.depend (c, value, lookup_context); })
    ;
  }
  శూన్య సంవృతం (hb_సంవృతం_context_t *c, గుర్తులేని value, Chain_Context_సంవృతం_Lookup_Context &lookup_context) స్థిరరాశి
  {
    ఒకవేళ (unlikely (c->lookup_హద్దు_exceeded ())) తిరిగి;

    తిరిగి
    + hb_iter (నియమం_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([&] (స్థిరరాశి Chain_నియమం_Type &_) { _.సంవృతం (c, value, lookup_context); })
    ;
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c,
                        Chain_Context_సంవృతం_Lookup_Context &lookup_context) స్థిరరాశి
  {
    ఒకవేళ (unlikely (c->lookup_హద్దు_exceeded ())) తిరిగి;

    + hb_iter (నియమం_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([&] (స్థిరరాశి Chain_నియమం_Type &_) { _.సంవృతం_lookups (c, lookup_context); })
    ;
  }

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) స్థిరరాశి
  {
    తిరిగి
    + hb_iter (నియమం_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([&] (స్థిరరాశి Chain_నియమం_Type &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  శూన్య collect_రెండవ_glyphs (hb_సమితి_digest_t *digest,
			      స్థిరరాశి ClassDef *ఆదానం_class_def = శూన్యసూచిక,
			      స్థిరరాశి ClassDef *lookahead_class_def = శూన్యసూచిక) స్థిరరాశి
  {
    + hb_iter (నియమం_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([=] (స్థిరరాశి Chain_నియమం_Type &_)
		{ _.collect_రెండవ_glyphs (digest, ఆదానం_class_def, lookahead_class_def); })
    ;
  }

  తార్కిక would_apply (hb_would_apply_context_t *c,
		    స్థిరరాశి ChainContextApplyLookupContext &lookup_context) స్థిరరాశి
  {
    తిరిగి
    + hb_iter (నియమం_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_map ([&] (స్థిరరాశి Chain_నియమం_Type &_) { తిరిగి _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  తార్కిక apply (hb_ot_apply_context_t *c,
	      స్థిరరాశి ChainContextApplyLookupContext &lookup_context,
	      స్థిరరాశి hb_ot_layout_ruleset_digest_t *digest = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    గుర్తులేని అంకె_నియమాలు = నియమం_2.len;

#నిర్వచితం_కాకపోతే HB_NO_OT_RULESETS_FAST_మార్గం
    ఒకవేళ (HB_OPTIMIZE_కొలత_VAL || అంకె_నియమాలు <= 4)
#అయితే_ముగింపు
    {
    slow:
      return_trace (
      + hb_iter (నియమం_2)
      | hb_map (hb_కూడు (ఇది))
      | hb_map ([&] (స్థిరరాశి Chain_నియమం_Type &_) { తిరిగి _.apply (c, lookup_context); })
      | hb_any
      )
      ;
    }

    /* This version is optimized for speed by matching the first & second
     * components of the rule here, instead of calling into the matching code.
     *
     * Replicated from LigatureSet::apply(). */

    /* We use the iter_context instead of iter_input, to avoid skipping
     * default-ignorables and such.
     *
     * Related: https://github.com/harfbuzz/harfbuzz/issues/4813
     */
    స్వయంచాలక &skippy_iter = c->iter_context;
    skippy_iter.reset (c->బఫరు->idx);
    skippy_iter.సమితి_సరిపోలిక_func (సరిపోలిక_always, శూన్యసూచిక);
    skippy_iter.సమితి_glyph_దత్తాంశం ((HBUINT16 *) శూన్యసూచిక);
    గుర్తులేని unsafe_గమ్యం = (గుర్తులేని) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *మొదటి = శూన్యసూచిక, *second = శూన్యసూచిక;
    తార్కిక matched = skippy_iter.next (&unsafe_to1);
    ఒకవేళ (likely (matched))
    {
      ఒకవేళ (skippy_iter.may_skip (c->బఫరు->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        వెళ్ళు slow;
      }

      మొదటి = &c->బఫరు->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      ఒకవేళ (digest && lookup_context.get_ఆదానం_విలువ &&
	  !digest->may_have (lookup_context.get_ఆదానం_విలువ (*మొదటి, lookup_context.సరిపోలిక_దత్తాంశం_2[1])))
	{
	  c->బఫరు->unsafe_గమ్యం_concat (c->బఫరు->idx, unsafe_to1);
	  return_trace (అబద్ధం);
	}
    }
    లేకపోతే
    {
      తార్కిక unsafe_గమ్యం_concat = అబద్ధం;
      తార్కిక ret =
      + hb_iter (నియమం_2)
      | hb_map (hb_కూడు (ఇది))
      | hb_filter ([&] (స్థిరరాశి Chain_నియమం_Type &_)
		   {
		     స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (_.ఆదానం_X)> (_.backtrack);
		     స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (_.lookaheadX)> (ఆదానం);
		     ఒకవేళ (ఆదానం.lenP1 <= 1 && lookahead.len == 0) తిరిగి నిజం;
		     unsafe_గమ్యం_concat = నిజం;
		     తిరిగి అబద్ధం;
		   })
      | hb_map ([&] (స్థిరరాశి Chain_నియమం_Type &_) { తిరిగి _.apply (c, lookup_context); })
      | hb_any
      ;
      ఒకవేళ (unsafe_గమ్యం_concat)
	c->బఫరు->unsafe_గమ్యం_concat (c->బఫరు->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    ఒకవేళ (likely (matched))
    {
      ఒకవేళ (skippy_iter.may_skip (c->బఫరు->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        వెళ్ళు slow;
      }

      second = &c->బఫరు->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    స్వయంచాలక సరిపోలిక_ఆదానం = lookup_context.funcs.సరిపోలిక[1];
    స్వయంచాలక సరిపోలిక_lookahead = lookup_context.funcs.సరిపోలిక[2];
    స్వయంచాలక *ఆదానం_దత్తాంశం = lookup_context.సరిపోలిక_దత్తాంశం_2[1];
    స్వయంచాలక *lookahead_దత్తాంశం = lookup_context.సరిపోలిక_దత్తాంశం_2[2];
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_నియమాలు; i++)
    {
      స్థిరరాశి స్వయంచాలక &r = ఇది+నియమం_2.arrayZ[i];

      స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (r.ఆదానం_X)> (r.backtrack);
      స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (r.lookaheadX)> (ఆదానం);

      గుర్తులేని lenP1 = ఆదానం.lenP1;
      ఒకవేళ (lenP1 > 1 ?
	   (!సరిపోలిక_ఆదానం ||
	    సరిపోలిక_ఆదానం (*మొదటి, ఆదానం.arrayZ[0], ఆదానం_దత్తాంశం))
	  :
	   (!lookahead.len || !సరిపోలిక_lookahead ||
	    సరిపోలిక_lookahead (*మొదటి, lookahead.arrayZ[0], lookahead_దత్తాంశం)))
      {
	lenP1 = hb_max (lenP1, 1u);
        ఒకవేళ (!second ||
	    (lenP1 > 2 ?
	     (!సరిపోలిక_ఆదానం ||
	      సరిపోలిక_ఆదానం (*second, ఆదానం.arrayZ[1], ఆదానం_దత్తాంశం))
	     :
	     (lookahead.len <= 2 - lenP1 || !సరిపోలిక_lookahead ||
	      సరిపోలిక_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_దత్తాంశం))))
	{
	  ఒకవేళ (r.apply (c, lookup_context))
	  {
	    ఒకవేళ (unsafe_గమ్యం != (గుర్తులేని) -1)
	      c->బఫరు->unsafe_గమ్యం_concat (c->బఫరు->idx, unsafe_గమ్యం);
	    return_trace (నిజం);
	  }
	}
	లేకపోతే
	  unsafe_గమ్యం = unsafe_to2;
      }
      లేకపోతే
      {
	ఒకవేళ (unsafe_గమ్యం == (గుర్తులేని) -1)
	  unsafe_గమ్యం = unsafe_to1;

	ఒకవేళ (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  కొరకు (; i + 1 < అంకె_నియమాలు; i++)
	  {
	    స్థిరరాశి స్వయంచాలక &r2 = ఇది+నియమం_2.arrayZ[i + 1];
	    స్థిరరాశి స్వయంచాలక &input2 = StructAfter<decltype (r2.ఆదానం_X)> (r2.backtrack);
	    ఒకవేళ (input2.lenP1 <= 1 || input2.arrayZ[0] != ఆదానం.arrayZ[0])
	      ఆపు;
	  }
	}
      }
    }
    ఒకవేళ (likely (unsafe_గమ్యం != (గుర్తులేని) -1))
      c->బఫరు->unsafe_గమ్యం_concat (c->బఫరు->idx, unsafe_గమ్యం);

    return_trace (అబద్ధం);
  }

  తార్కిక subset (hb_subset_context_t *c,
	       స్థిరరాశి hb_map_t *lookup_map,
	       స్థిరరాశి hb_map_t *backtrack_klass_map = శూన్యసూచిక,
	       స్థిరరాశి hb_map_t *ఆదానం_klass_map = శూన్యసూచిక,
	       స్థిరరాశి hb_map_t *lookahead_klass_map = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);

    స్వయంచాలక snap = c->serializer->snapshot ();
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    కొరకు (స్థిరరాశి స్వయంచాలక& _ : నియమం_2)
    {
      ఒకవేళ (!_) కొనసాగు;
      స్వయంచాలక o_snap = c->serializer->snapshot ();
      స్వయంచాలక *o = బహిర్గతం->నియమం_2.serialize_append (c->serializer);
      ఒకవేళ (unlikely (!o)) కొనసాగు;

      ఒకవేళ (!o->serialize_subset (c, _, ఇది,
				lookup_map,
				backtrack_klass_map,
				ఆదానం_klass_map,
				lookahead_klass_map))
      {
	బహిర్గతం->నియమం_2.తీయి ();
	c->serializer->revert (o_snap);
      }
    }

    తార్కిక ret = తార్కిక (బహిర్గతం->నియమం_2);
    ఒకవేళ (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (నియమం_2.sanitize (c, ఇది));
  }

  రక్షిత:
  Array16_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Chain_నియమం_Type>>
		నియమం_2;			/* Array of ChainRule tables
					 * ordered by preference */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2, నియమం_2);
};

మూస <రకం_పేరు Types>
నిర్మాణం Chain_నియమం_సమితి : Chain_నియమం_సమితి_యొక్క<Types, Chain_నియమం<Types>> {};

static_assert (పరిమాణం (Chain_నియమం_సమితి<MediumTypes>) ==
	       HBUINT16::static_కొలత + Offset24::static_కొలత, "");

మూస <రకం_పేరు Types>
నిర్మాణం Chain_Class_నియమం_సమితి : Chain_నియమం_సమితి_యొక్క<Types, Chain_నియమం<SmallTypes>> {};

మూస <రకం_పేరు Types>
నిర్మాణం ChainContextFormat1_4
{
  ఉపయోగించి Chain_నియమం_సమితి = OT::Chain_నియమం_సమితి<Types>;

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    నిర్మాణం Chain_Context_సంవృతం_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {శూన్యసూచిక, శూన్యసూచిక, శూన్యసూచిక}
    };

    తిరిగి
    + hb_zip (ఇది+coverage, నియమం_సమితి_2)
    | hb_filter (*glyphs, hb_మొదటి)
    | hb_map (hb_రెండవ)
    | hb_map (hb_కూడు (ఇది))
    | hb_map ([&] (స్థిరరాశి Chain_నియమం_సమితి &_) { తిరిగి _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  తార్కిక may_have_non_1to1 () స్థిరరాశి
  { తిరిగి నిజం; }

  శూన్య depend (hb_depend_context_t *c) స్థిరరాశి
  {
    hb_సమితి_t* cur_చురుకైన_glyphs = c->నెట్టు_cur_చురుకైన_glyphs ();
    ఒకవేళ (unlikely (!cur_చురుకైన_glyphs)) తిరిగి;
    get_coverage ().intersect_సమితి (*c->glyphs, *cur_చురుకైన_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {శూన్యసూచిక, శూన్యసూచిక, శూన్యసూచిక},
      {శూన్యసూచిక, శూన్యసూచిక, శూన్యసూచిక},  /* intersects_cache */
      శూన్యసూచిక                        /* intersected_glyphs_cache */
    };

    + hb_zip (ఇది+coverage, hb_range ((గుర్తులేని) నియమం_సమితి_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      తిరిగి c->మాతృ_చురుకైన_glyphs ().has (_);
    }, hb_మొదటి)
    | hb_map ([&](స్థిరరాశి hb_pair_t<hb_codepoint_t, గుర్తులేని> _) { తిరిగి hb_pair_t<గుర్తులేని, స్థిరరాశి Chain_నియమం_సమితి&> (_.మొదటి, ఇది+నియమం_సమితి_2[_.second]); })
    | hb_apply ([&] (స్థిరరాశి hb_pair_t<గుర్తులేని, స్థిరరాశి Chain_నియమం_సమితి&>& _) { _.second.depend (c, _.మొదటి, lookup_context); })
    ;

    c->తీయి_cur_పూర్తి_glyphs ();
  }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c) స్థిరరాశి
  {
    hb_సమితి_t* cur_చురుకైన_glyphs = c->నెట్టు_cur_చురుకైన_glyphs ();
    ఒకవేళ (unlikely (!cur_చురుకైన_glyphs)) తిరిగి;
    get_coverage ().intersect_సమితి (c->మునుపటి_మాతృ_చురుకైన_glyphs (),
				   *cur_చురుకైన_glyphs);

    నిర్మాణం Chain_Context_సంవృతం_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {శూన్యసూచిక, శూన్యసూచిక, శూన్యసూచిక}
    };

    + hb_zip (ఇది+coverage, hb_range ((గుర్తులేని) నియమం_సమితి_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      తిరిగి c->మునుపటి_మాతృ_చురుకైన_glyphs ().has (_);
    }, hb_మొదటి)
    | hb_map ([&](స్థిరరాశి hb_pair_t<hb_codepoint_t, గుర్తులేని> _) { తిరిగి hb_pair_t<గుర్తులేని, స్థిరరాశి Chain_నియమం_సమితి&> (_.మొదటి, ఇది+నియమం_సమితి_2[_.second]); })
    | hb_apply ([&] (స్థిరరాశి hb_pair_t<గుర్తులేని, స్థిరరాశి Chain_నియమం_సమితి&>& _) { _.second.సంవృతం (c, _.మొదటి, lookup_context); })
    ;

    c->తీయి_cur_పూర్తి_glyphs ();
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి
  {
    నిర్మాణం Chain_Context_సంవృతం_Lookup_Context lookup_context = {
      {intersects_glyph, శూన్యసూచిక},
      ContextFormat::SimpleContext,
      {శూన్యసూచిక, శూన్యసూచిక, శూన్యసూచిక}
    };

    + hb_zip (ఇది+coverage, నియమం_సమితి_2)
    | hb_filter (*c->glyphs, hb_మొదటి)
    | hb_map (hb_రెండవ)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([&] (స్థిరరాశి Chain_నియమం_సమితి &_) { _.సంవృతం_lookups (c, lookup_context); })
    ;
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి {}

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    (ఇది+coverage).collect_coverage (c->ఆదానం);

    నిర్మాణం ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {శూన్యసూచిక, శూన్యసూచిక, శూన్యసూచిక}
    };

    + hb_iter (నియమం_సమితి_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([&] (స్థిరరాశి Chain_నియమం_సమితి &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  శూన్య collect_రెండవ_glyphs (hb_సమితి_digest_t *digest) స్థిరరాశి
  {
    + hb_iter (నియమం_సమితి_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([digest] (స్థిరరాశి Chain_నియమం_సమితి &_) { _.collect_రెండవ_glyphs (digest); })
    ;
  }

  తార్కిక would_apply (hb_would_apply_context_t *c) స్థిరరాశి
  {
    స్థిరరాశి Chain_నియమం_సమితి &నియమం_సమితి_3 = ఇది+నియమం_సమితి_2[(ఇది+coverage).get_coverage (c->glyphs[0])];
    నిర్మాణం ChainContextApplyLookupContext lookup_context = {
      {{సరిపోలిక_glyph, సరిపోలిక_glyph, సరిపోలిక_glyph}},
      {శూన్యసూచిక, శూన్యసూచిక, శూన్యసూచిక}
    };
    తిరిగి నియమం_సమితి_3.would_apply (c, lookup_context);
  }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి { తిరిగి ఇది+coverage; }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage (c->బఫరు->cur().codepoint);
    ఒకవేళ (సూచీ_2 == NOT_COVERED) return_trace (అబద్ధం);

    స్థిరరాశి Chain_నియమం_సమితి &నియమం_సమితి_3 = ఇది+నియమం_సమితి_2[సూచీ_2];
    నిర్మాణం ChainContextApplyLookupContext lookup_context = {
      {{సరిపోలిక_glyph, సరిపోలిక_glyph, సరిపోలిక_glyph}},
      {శూన్యసూచిక, శూన్యసూచిక, శూన్యసూచిక}
    };
    return_trace (నియమం_సమితి_3.apply (c, lookup_context));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
    బహిర్గతం->format = format;

    స్థిరరాశి hb_map_t *lookup_map = c->పట్టిక_ట్యాగ్_2 == HB_OT_ట్యాగ్_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_సదిశ_t<hb_codepoint_t> new_coverage;
    + hb_zip (ఇది+coverage, నియమం_సమితి_2)
    | hb_filter (glyphset, hb_మొదటి)
    | hb_filter (subset_స్థానభ్రంశం_array (c, బహిర్గతం->నియమం_సమితి_2, ఇది, lookup_map), hb_రెండవ)
    | hb_map (hb_మొదటి)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    బహిర్గతం->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (తార్కిక (new_coverage));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (coverage.sanitize (c, ఇది) && నియమం_సమితి_2.sanitize (c, ఇది));
  }

  రక్షిత:
  HBUINT16	format;			/* Format identifier--format = 1 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  రకం_పేరు Types::మూస Array_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Chain_నియమం_సమితి>>
		నియమం_సమితి_2;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2 + Types::LOffset::static_కొలత + Types::HBUINT::static_కొలత, నియమం_సమితి_2);
};

మూస <రకం_పేరు Types>
నిర్మాణం ChainContextFormat2_5
{
  ఉపయోగించి Chain_నియమం_సమితి = OT::Chain_Class_నియమం_సమితి<Types>;

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    ఒకవేళ (!(ఇది+coverage).intersects (glyphs))
      తిరిగి అబద్ధం;

    స్థిరరాశి ClassDef &backtrack_class_def = ఇది+backtrackClassDef;
    స్థిరరాశి ClassDef &ఆదానం_class_def = ఇది+ఆదానం_Class_Def;
    స్థిరరాశి ClassDef &lookahead_class_def = ఇది+lookaheadClassDef;

    hb_map_t caches[3] = {};
    నిర్మాణం Chain_Context_సంవృతం_Lookup_Context lookup_context = {
      {intersects_class, శూన్యసూచిక},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ఆదానం_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_సమితి_t retained_coverage_glyphs;
    (ఇది+coverage).intersect_సమితి (*glyphs, retained_coverage_glyphs);

    hb_సమితి_t coverage_glyph_classes;
    ఆదానం_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    తిరిగి
    + hb_iter (నియమం_సమితి_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_enumerate
    | hb_map ([&] (స్థిరరాశి hb_pair_t<గుర్తులేని, స్థిరరాశి Chain_నియమం_సమితి &> p)
	      { తిరిగి ఆదానం_class_def.intersects_class (glyphs, p.మొదటి) &&
		       coverage_glyph_classes.has (p.మొదటి) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  తార్కిక may_have_non_1to1 () స్థిరరాశి
  { తిరిగి నిజం; }

  శూన్య depend (hb_depend_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (!(ఇది+coverage).intersects (c->glyphs))
      తిరిగి;

    hb_సమితి_t* cur_చురుకైన_glyphs = c->నెట్టు_cur_చురుకైన_glyphs ();
    ఒకవేళ (unlikely (!cur_చురుకైన_glyphs)) తిరిగి;
    get_coverage ().intersect_సమితి (*c->glyphs, *cur_చురుకైన_glyphs);

    స్థిరరాశి ClassDef &backtrack_class_def = ఇది+backtrackClassDef;
    స్థిరరాశి ClassDef &ఆదానం_class_def = ఇది+ఆదానం_Class_Def;
    స్థిరరాశి ClassDef &lookahead_class_def = ఇది+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ChainContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ఆదానం_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (నియమం_సమితి_2)
    | hb_filter ([&] (గుర్తులేని _)
    { తిరిగి ఆదానం_class_def.intersects_class (&c->మాతృ_చురుకైన_glyphs (), _); },
		 hb_మొదటి)
    | hb_apply ([&] (స్థిరరాశి hb_pair_t<గుర్తులేని, స్థిరరాశి రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Chain_నియమం_సమితి>&> _)
                {
                  స్థిరరాశి Chain_నియమం_సమితి& chainrule_సమితి = ఇది+_.second;
                  chainrule_సమితి.depend (c, _.మొదటి, lookup_context);
                })
    ;

    c->తీయి_cur_పూర్తి_glyphs ();
  }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (!(ఇది+coverage).intersects (c->glyphs))
      తిరిగి;

    hb_సమితి_t* cur_చురుకైన_glyphs = c->నెట్టు_cur_చురుకైన_glyphs ();
    ఒకవేళ (unlikely (!cur_చురుకైన_glyphs)) తిరిగి;
    get_coverage ().intersect_సమితి (c->మునుపటి_మాతృ_చురుకైన_glyphs (),
				   *cur_చురుకైన_glyphs);

    స్థిరరాశి ClassDef &backtrack_class_def = ఇది+backtrackClassDef;
    స్థిరరాశి ClassDef &ఆదానం_class_def = ఇది+ఆదానం_Class_Def;
    స్థిరరాశి ClassDef &lookahead_class_def = ఇది+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    నిర్మాణం Chain_Context_సంవృతం_Lookup_Context lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ఆదానం_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (నియమం_సమితి_2)
    | hb_filter ([&] (గుర్తులేని _)
    { తిరిగి ఆదానం_class_def.intersects_class (&c->మాతృ_చురుకైన_glyphs (), _); },
		 hb_మొదటి)
    | hb_apply ([&] (స్థిరరాశి hb_pair_t<గుర్తులేని, స్థిరరాశి రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Chain_నియమం_సమితి>&> _)
                {
                  స్థిరరాశి Chain_నియమం_సమితి& chainrule_సమితి = ఇది+_.second;
                  chainrule_సమితి.సంవృతం (c, _.మొదటి, lookup_context);
                })
    ;

    c->తీయి_cur_పూర్తి_glyphs ();
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (!(ఇది+coverage).intersects (c->glyphs))
      తిరిగి;

    స్థిరరాశి ClassDef &backtrack_class_def = ఇది+backtrackClassDef;
    స్థిరరాశి ClassDef &ఆదానం_class_def = ఇది+ఆదానం_Class_Def;
    స్థిరరాశి ClassDef &lookahead_class_def = ఇది+lookaheadClassDef;

    hb_map_t caches[3] = {};
    నిర్మాణం Chain_Context_సంవృతం_Lookup_Context lookup_context = {
      {intersects_class, శూన్యసూచిక},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ఆదానం_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (నియమం_సమితి_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_enumerate
    | hb_filter([&] (గుర్తులేని klass)
    { తిరిగి ఆదానం_class_def.intersects_class (c->glyphs, klass); }, hb_మొదటి)
    | hb_map (hb_రెండవ)
    | hb_apply ([&] (స్థిరరాశి Chain_నియమం_సమితి &_)
    { _.సంవృతం_lookups (c, lookup_context); })
    ;
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి {}

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    (ఇది+coverage).collect_coverage (c->ఆదానం);

    స్థిరరాశి ClassDef &backtrack_class_def = ఇది+backtrackClassDef;
    స్థిరరాశి ClassDef &ఆదానం_class_def = ఇది+ఆదానం_Class_Def;
    స్థిరరాశి ClassDef &lookahead_class_def = ఇది+lookaheadClassDef;

    నిర్మాణం ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &ఆదానం_class_def,
       &lookahead_class_def}
    };

    + hb_iter (నియమం_సమితి_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([&] (స్థిరరాశి Chain_నియమం_సమితి &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  శూన్య collect_రెండవ_glyphs (hb_సమితి_digest_t *digest) స్థిరరాశి
  {
    స్థిరరాశి ClassDef &ఆదానం_class_def = ఇది+ఆదానం_Class_Def;
    స్థిరరాశి ClassDef &lookahead_class_def = ఇది+lookaheadClassDef;
    + hb_iter (నియమం_సమితి_2)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([digest, &ఆదానం_class_def, &lookahead_class_def] (స్థిరరాశి Chain_నియమం_సమితి &_)
		{ _.collect_రెండవ_glyphs (digest, &ఆదానం_class_def, &lookahead_class_def); })
    ;
  }

  తార్కిక would_apply (hb_would_apply_context_t *c) స్థిరరాశి
  {
    స్థిరరాశి ClassDef &backtrack_class_def = ఇది+backtrackClassDef;
    స్థిరరాశి ClassDef &ఆదానం_class_def = ఇది+ఆదానం_Class_Def;
    స్థిరరాశి ClassDef &lookahead_class_def = ఇది+lookaheadClassDef;

    గుర్తులేని పూర్ణాంకం సూచీ_2 = ఆదానం_class_def.get_class (c->glyphs[0]);
    స్థిరరాశి Chain_నియమం_సమితి &నియమం_సమితి_3 = ఇది+నియమం_సమితి_2[సూచీ_2];
    నిర్మాణం ChainContextApplyLookupContext lookup_context = {
      {{సరిపోలిక_class, సరిపోలిక_class, సరిపోలిక_class}},
      {&backtrack_class_def,
       &ఆదానం_class_def,
       &lookahead_class_def}
    };
    తిరిగి నియమం_సమితి_3.would_apply (c, lookup_context);
  }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి { తిరిగి ఇది+coverage; }

  గుర్తులేని cache_ఖర్చు () స్థిరరాశి
  {
    తిరిగి (ఇది+ఆదానం_Class_Def).ఖర్చు () + (ఇది+lookaheadClassDef).ఖర్చు ();
  }
  స్థిర తార్కిక cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    తిరిగి context_cache_func (c, op);
  }

  నిర్మాణం external_cache_t
  {
    hb_ot_layout_ద్విమాన_cache_t coverage;
    hb_ot_layout_mapping_cache_t ఆదానం_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t నియమం_sets[HB_VAR_ARRAY];
  };
  శూన్య *external_cache_create () స్థిరరాశి
  {
    గుర్తులేని లెక్క = నియమం_సమితి_2.len;
    గుర్తులేని కొలత = పరిమాణం (external_cache_t) -
		    HB_VAR_ARRAY * పరిమాణం (hb_ot_layout_ruleset_digest_t) +
		    లెక్క * పరిమాణం (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (కొలత);
    ఒకవేళ (likely (cache))
    {
      cache->coverage.clear ();
      cache->ఆదానం_class.clear ();
      cache->lookahead_class.clear ();
      కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
	(ఇది+నియమం_సమితి_2[i]).collect_మొదటి_ఆదానం_classes (&cache->నియమం_sets[i]);
    }
    తిరిగి cache;
  }
  తార్కిక apply_cached (hb_ot_apply_context_t *c, శూన్య *external_cache) స్థిరరాశి { తిరిగి _apply (c, నిజం, external_cache); }
  తార్కిక apply (hb_ot_apply_context_t *c, శూన్య *external_cache) స్థిరరాశి { తిరిగి _apply (c, అబద్ధం, external_cache); }
  తార్కిక _apply (hb_ot_apply_context_t *c, తార్కిక cached, శూన్య *external_cache) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage_ద్విమాన (c->బఫరు->cur().codepoint, cache ? &cache->coverage : శూన్యసూచిక);
#లేకపోతే
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage (c->బఫరు->cur().codepoint);
#అయితే_ముగింపు
    ఒకవేళ (సూచీ_2 == NOT_COVERED) return_trace (అబద్ధం);

    స్థిరరాశి ClassDef &backtrack_class_def = ఇది+backtrackClassDef;
    స్థిరరాశి ClassDef &ఆదానం_class_def = ఇది+ఆదానం_Class_Def;
    స్థిరరాశి ClassDef &lookahead_class_def = ఇది+lookaheadClassDef;

#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    సరిపోలిక_class_cache_దత్తాంశం_t ఆదానం_cache_దత్తాంశం = {
      &ఆదానం_class_def,
      cache ? &cache->ఆదానం_class : శూన్యసూచిక
    };
    సరిపోలిక_class_cache_దత్తాంశం_t lookahead_cache_దత్తాంశం = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : శూన్యసూచిక
    };
    స్థిరరాశి శూన్య *ఆదానం_సరిపోలిక_దత్తాంశం = cached
      ? static_cast<స్థిరరాశి శూన్య *> (&ఆదానం_class_def)
      : static_cast<స్థిరరాశి శూన్య *> (&ఆదానం_cache_దత్తాంశం);
    స్థిరరాశి శూన్య *lookahead_సరిపోలిక_దత్తాంశం = cached
      ? static_cast<స్థిరరాశి శూన్య *> (&lookahead_class_def)
      : static_cast<స్థిరరాశి శూన్య *> (&lookahead_cache_దత్తాంశం);
    తార్కిక backtrack_ఉపయోగిస్తుంది_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    స్థిరరాశి శూన్య *backtrack_సరిపోలిక_దత్తాంశం = backtrack_ఉపయోగిస్తుంది_lookahead_cache
      ? static_cast<స్థిరరాశి శూన్య *> (&lookahead_cache_దత్తాంశం)
      : static_cast<స్థిరరాశి శూన్య *> (&backtrack_class_def);
#అయితే_ముగింపు

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    నిర్మాణం ChainContextApplyLookupContext lookup_context = {
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? సరిపోలిక_class_cached1 :
        backtrack_ఉపయోగిస్తుంది_lookahead_cache ? సరిపోలిక_class_cache : సరిపోలిక_class,
        cached ? సరిపోలిక_class_cached2 : సరిపోలిక_class_cache,
        cached ? సరిపోలిక_class_cached1 : సరిపోలిక_class_cache}},
      {backtrack_సరిపోలిక_దత్తాంశం,
       ఆదానం_సరిపోలిక_దత్తాంశం,
       lookahead_సరిపోలిక_దత్తాంశం},
      cached ? get_class_cached2_విలువ : get_class_cache_విలువ
#లేకపోతే
      {{cached && &backtrack_class_def == &lookahead_class_def ? సరిపోలిక_class_cached1 : సరిపోలిక_class,
        cached ? సరిపోలిక_class_cached2 : సరిపోలిక_class,
        cached ? సరిపోలిక_class_cached1 : సరిపోలిక_class}},
      {&backtrack_class_def,
       &ఆదానం_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_విలువ : get_class_విలువ
#అయితే_ముగింపు
    };

#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    సూచీ_2 = cached
         ? get_class_cached2 (ఆదానం_class_def, c->బఫరు->cur())
         : ఆదానం_class_def.get_class (c->బఫరు->cur().codepoint, ఆదానం_cache_దత్తాంశం.cache);
#లేకపోతే
    సూచీ_2 = cached
         ? get_class_cached2 (ఆదానం_class_def, c->బఫరు->cur())
          : ఆదానం_class_def.get_class (c->బఫరు->cur().codepoint);
#అయితే_ముగింపు
    స్థిరరాశి Chain_నియమం_సమితి &నియమం_సమితి_3 = ఇది+నియమం_సమితి_2[సూచీ_2];
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    స్థిరరాశి hb_ot_layout_ruleset_digest_t *digest = cache && సూచీ_2 < నియమం_సమితి_2.len ? &cache->నియమం_sets[సూచీ_2] : శూన్యసూచిక;
#లేకపోతే
    స్థిరరాశి hb_ot_layout_ruleset_digest_t *digest = శూన్యసూచిక;
#అయితే_ముగింపు
    return_trace (నియమం_సమితి_3.apply (c, lookup_context, digest));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
    బహిర్గతం->format = format;
    బహిర్గతం->coverage.serialize_subset (c, coverage, ఇది);

    hb_map_t backtrack_klass_map;
    hb_map_t ఆదానం_klass_map;
    hb_map_t lookahead_klass_map;

    బహిర్గతం->backtrackClassDef.serialize_subset (c, backtrackClassDef, ఇది, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    బహిర్గతం->ఆదానం_Class_Def.serialize_subset (c, ఆదానం_Class_Def, ఇది, &ఆదానం_klass_map);
    బహిర్గతం->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, ఇది, &lookahead_klass_map);

    ఒకవేళ (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   ఆదానం_klass_map,
						   lookahead_klass_map)))
      return_trace (అబద్ధం);

    స్థిరరాశి hb_సమితి_t* glyphset = c->plan->glyphset_gsub ();
    hb_సమితి_t retained_coverage_glyphs;
    (ఇది+coverage).intersect_సమితి (*glyphset, retained_coverage_glyphs);

    hb_సమితి_t coverage_glyph_classes;
    (ఇది+ఆదానం_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    పూర్ణాంకం non_zero_సూచీ = -1, సూచీ_2 = 0;
    తార్కిక ret = నిజం;
    స్థిరరాశి hb_map_t *lookup_map = c->పట్టిక_ట్యాగ్_2 == HB_OT_ట్యాగ్_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    స్వయంచాలక చివరి_non_zero = c->serializer->snapshot ();
    కొరకు (స్థిరరాశి స్వయంచాలక& _ : + hb_enumerate (నియమం_సమితి_2)
			 | hb_filter (ఆదానం_klass_map, hb_మొదటి))
    {
      స్వయంచాలక *o = బహిర్గతం->నియమం_సమితి_2.serialize_append (c->serializer);
      ఒకవేళ (unlikely (!o))
      {
	ret = అబద్ధం;
	ఆపు;
      }
      ఒకవేళ (coverage_glyph_classes.has (_.మొదటి) &&
          o->serialize_subset (c, _.second, ఇది,
			       lookup_map,
			       &backtrack_klass_map,
			       &ఆదానం_klass_map,
			       &lookahead_klass_map))
      {
        చివరి_non_zero = c->serializer->snapshot ();
	non_zero_సూచీ = సూచీ_2;
      }

      సూచీ_2++;
    }

    ఒకవేళ (!ret || non_zero_సూచీ == -1) return_trace (అబద్ధం);

    // prune empty trailing ruleSets
    ఒకవేళ (సూచీ_2 > non_zero_సూచీ) {
      c->serializer->revert (చివరి_non_zero);
      బహిర్గతం->నియమం_సమితి_2.len = non_zero_సూచీ + 1;
    }

    return_trace (తార్కిక (బహిర్గతం->నియమం_సమితి_2));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (coverage.sanitize (c, ఇది) &&
		  backtrackClassDef.sanitize (c, ఇది) &&
		  ఆదానం_Class_Def.sanitize (c, ఇది) &&
		  lookaheadClassDef.sanitize (c, ఇది) &&
		  నియమం_సమితి_2.sanitize (c, ఇది));
  }

  రక్షిత:
  HBUINT16	format;			/* Format identifier--format = 2 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  రకం_పేరు Types::మూస LOffset_గమ్యం<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  రకం_పేరు Types::మూస LOffset_గమ్యం<ClassDef>
		ఆదానం_Class_Def;		/* Offset to glyph ClassDef
					 * table containing input sequence
					 * data--from beginning of table */
  రకం_పేరు Types::మూస LOffset_గమ్యం<ClassDef>
		lookaheadClassDef;	/* Offset to glyph ClassDef table
					 * containing lookahead sequence
					 * data--from beginning of table */
  Array16_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Chain_నియమం_సమితి>>
		నియమం_సమితి_2;		/* Array of ChainRuleSet tables
					 * ordered by class */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (4 + 4 * Types::LOffset::static_కొలత, నియమం_సమితి_2);
};

మూస <రకం_పేరు Types>
నిర్మాణం ChainContextFormat3
{
  ఉపయోగించి నియమం_సమితి = OT::నియమం_సమితి<SmallTypes>;

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);

    ఒకవేళ (!(ఇది+ఆదానం[0]).intersects (glyphs))
      తిరిగి అబద్ధం;

    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    నిర్మాణం Chain_Context_సంవృతం_Lookup_Context lookup_context = {
      {intersects_coverage, శూన్యసూచిక},
      ContextFormat::CoverageBasedContext,
      {ఇది, ఇది, ఇది}
    };
    తిరిగి chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     ఆదానం.len, ఆదానం.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  తార్కిక may_have_non_1to1 () స్థిరరాశి
  { తిరిగి నిజం; }

  శూన్య depend (hb_depend_context_t *c) స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);

    ఒకవేళ (!(ఇది+ఆదానం[0]).intersects (c->glyphs))
      తిరిగి;

    hb_సమితి_t* cur_చురుకైన_glyphs = c->నెట్టు_cur_చురుకైన_glyphs ();
    ఒకవేళ (unlikely (!cur_చురుకైన_glyphs))
      తిరిగి;
    get_coverage ().intersect_సమితి (*c->glyphs, *cur_చురుకైన_glyphs);

    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    స్థిరరాశి స్వయంచాలక &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {ఇది, ఇది, ఇది},
      {శూన్యసూచిక, శూన్యసూచిక, శూన్యసూచిక},  /* intersects_cache */
      శూన్యసూచిక                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (స్థిరరాశి HBUINT16 *) backtrack.arrayZ,
				 ఆదానం.len, (స్థిరరాశి HBUINT16 *) ఆదానం.arrayZ + 1,
				 lookahead.len, (స్థిరరాశి HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 ఇది,
				 ఆదానం[0], lookup_context);

    c->తీయి_cur_పూర్తి_glyphs ();
  }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c) స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);

    ఒకవేళ (!(ఇది+ఆదానం[0]).intersects (c->glyphs))
      తిరిగి;

    hb_సమితి_t* cur_చురుకైన_glyphs = c->నెట్టు_cur_చురుకైన_glyphs ();
    ఒకవేళ (unlikely (!cur_చురుకైన_glyphs))
      తిరిగి;
    get_coverage ().intersect_సమితి (c->మునుపటి_మాతృ_చురుకైన_glyphs (),
				   *cur_చురుకైన_glyphs);

    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    స్థిరరాశి స్వయంచాలక &lookup = StructAfter<decltype (lookupX)> (lookahead);
    నిర్మాణం Chain_Context_సంవృతం_Lookup_Context lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {ఇది, ఇది, ఇది}
    };
    chain_context_సంవృతం_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  ఆదానం.len, ఆదానం.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->తీయి_cur_పూర్తి_glyphs ();
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (!intersects (c->glyphs))
      తిరిగి;

    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);
    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    స్థిరరాశి స్వయంచాలక &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి {}

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);

    (ఇది+ఆదానం[0]).collect_coverage (c->ఆదానం);

    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    స్థిరరాశి స్వయంచాలక &lookup = StructAfter<decltype (lookupX)> (lookahead);

    నిర్మాణం ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {ఇది, ఇది, ఇది}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 ఆదానం.len, ఆదానం.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  శూన్య collect_రెండవ_glyphs (hb_సమితి_digest_t *digest) స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);
    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    ఒకవేళ (ఆదానం.len > 1)
      (ఇది+ఆదానం[1]).collect_coverage (digest);
    లేకపోతే ఒకవేళ (lookahead.len)
      (ఇది+lookahead[0]).collect_coverage (digest);
    లేకపోతే
      *digest = hb_సమితి_digest_t::నిండిన ();
  }

  తార్కిక would_apply (hb_would_apply_context_t *c) స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);
    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    స్థిరరాశి స్వయంచాలక &lookup = StructAfter<decltype (lookupX)> (lookahead);
    నిర్మాణం ChainContextApplyLookupContext lookup_context = {
      {{సరిపోలిక_coverage, సరిపోలిక_coverage, సరిపోలిక_coverage}},
      {ఇది, ఇది, ఇది}
    };
    తిరిగి chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     ఆదానం.len, ఆదానం.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);
    తిరిగి ఇది+ఆదానం[0];
  }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);

    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+ఆదానం[0]).get_coverage (c->బఫరు->cur().codepoint);
    ఒకవేళ (సూచీ_2 == NOT_COVERED) return_trace (అబద్ధం);

    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    స్థిరరాశి స్వయంచాలక &lookup = StructAfter<decltype (lookupX)> (lookahead);
    నిర్మాణం ChainContextApplyLookupContext lookup_context = {
      {{సరిపోలిక_coverage, సరిపోలిక_coverage, సరిపోలిక_coverage}},
      {ఇది, ఇది, ఇది}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      ఆదానం.len, ఆదానం.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  మూస<రకం_పేరు Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  తార్కిక serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, స్థిరరాశి శూన్య* పునాది) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed<Array16_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Coverage>>> ();
    ఒకవేళ (unlikely (!బహిర్గతం->serialize (c->serializer, 0u))) return_trace (అబద్ధం);

    కొరకు (స్వయంచాలక& స్థానభ్రంశం : it) {
      స్వయంచాలక *o = బహిర్గతం->serialize_append (c->serializer);
      ఒకవేళ (unlikely (!o) || !o->serialize_subset (c, స్థానభ్రంశం, పునాది))
        return_trace (అబద్ధం);
    }

    return_trace (నిజం);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);

    ఒకవేళ (unlikely (!c->serializer->embed (ఇది->format))) return_trace (అబద్ధం);

    ఒకవేళ (!serialize_coverage_offsets (c, backtrack.iter (), ఇది))
      return_trace (అబద్ధం);

    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);
    ఒకవేళ (!serialize_coverage_offsets (c, ఆదానం.iter (), ఇది))
      return_trace (అబద్ధం);

    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    ఒకవేళ (!serialize_coverage_offsets (c, lookahead.iter (), ఇది))
      return_trace (అబద్ధం);

    స్థిరరాశి స్వయంచాలక &lookup = StructAfter<decltype (lookupX)> (lookahead);
    స్థిరరాశి hb_map_t *lookup_map = c->పట్టిక_ట్యాగ్_2 == HB_OT_ట్యాగ్_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_లెక్క_2 = c->serializer->copy<HBUINT16> (lookup.len);
    ఒకవేళ (!lookup_లెక్క_2) return_trace (అబద్ధం);

    గుర్తులేని లెక్క = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_కేటాయించు (*lookup_లెక్క_2, లెక్క, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!backtrack.sanitize (c, ఇది))) return_trace (అబద్ధం);
    hb_barrier ();
    స్థిరరాశి స్వయంచాలక &ఆదానం = StructAfter<decltype (ఆదానం_X)> (backtrack);
    ఒకవేళ (unlikely (!ఆదానం.sanitize (c, ఇది))) return_trace (అబద్ధం);
    hb_barrier ();
    ఒకవేళ (unlikely (!ఆదానం.len)) return_trace (అబద్ధం); /* To be consistent with Context. */
    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (ఆదానం);
    ఒకవేళ (unlikely (!lookahead.sanitize (c, ఇది))) return_trace (అబద్ధం);
    hb_barrier ();
    స్థిరరాశి స్వయంచాలక &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  రక్షిత:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Coverage>>
		ఆదానం_X		;	/* Array of coverage
					 * tables in input sequence, in glyph
					 * sequence order */
  Array16_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Coverage>>
		lookaheadX;		/* Array of coverage tables
					 * in lookahead sequence, in glyph
					 * sequence order */
  Array16_యొక్క<Lookup_రికార్డు>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  బహిరంగ:
  DEFINE_కొలత_MIN (10);
};

నిర్మాణం ChainContext
{
  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, Ts&&... ds) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!c->may_dispatch (ఇది, &u.format.v))) తిరిగి c->no_dispatch_return_విలువ ();
    TRACE_DISPATCH (ఇది, u.format.v);
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    స్థితి 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    స్థితి 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    స్థితి 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#అయితే_ముగింపు
    అప్రమేయ:return_trace (c->default_return_విలువ ());
    }
  }

  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT16 v; }		format;	/* Format identifier */
  ChainContextFormat1_4<SmallTypes>	format1;
  ChainContextFormat2_5<SmallTypes>	format2;
  ChainContextFormat3<SmallTypes>	format3;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  ChainContextFormat1_4<MediumTypes>	format4;
  ChainContextFormat2_5<MediumTypes>	format5;
  /* The beyond-64k spec adds sequence-context format 6, but no
   * corresponding chained-context coverage format. */
#అయితే_ముగింపు
  } u;
};


మూస <రకం_పేరు T>
నిర్మాణం ExtensionFormat1
{
  గుర్తులేని పూర్ణాంకం get_type () స్థిరరాశి { తిరిగి extensionLookupType; }

  మూస <రకం_పేరు X>
  స్థిరరాశి X& get_subtable () స్థిరరాశి
  { తిరిగి ఇది + reinterpret_cast<స్థిరరాశి Offset32_గమ్యం<రకం_పేరు T::Sub_పట్టిక> &> (extension_స్థానభ్రంశం); }

  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, Ts&&... ds) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!c->may_dispatch (ఇది, ఇది))) తిరిగి c->no_dispatch_return_విలువ ();
    TRACE_DISPATCH (ఇది, format);
    return_trace (get_subtable<రకం_పేరు T::Sub_పట్టిక> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  { dispatch (c); }

  /* This is called from may_dispatch() above with hb_sanitize_context_t. */
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  extensionLookupType != T::Sub_పట్టిక::Extension);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    బహిర్గతం->format = format;
    బహిర్గతం->extensionLookupType = extensionLookupType;

    స్థిరరాశి స్వయంచాలక& src_స్థానభ్రంశం =
        reinterpret_cast<స్థిరరాశి Offset32_గమ్యం<రకం_పేరు T::Sub_పట్టిక> &> (extension_స్థానభ్రంశం);
    స్వయంచాలక& dest_స్థానభ్రంశం =
        reinterpret_cast<Offset32_గమ్యం<రకం_పేరు T::Sub_పట్టిక> &> (బహిర్గతం->extension_స్థానభ్రంశం);

    return_trace (dest_స్థానభ్రంశం.serialize_subset (c, src_స్థానభ్రంశం, ఇది, get_type ()));
  }

  రక్షిత:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extension_స్థానభ్రంశం;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

మూస <రకం_పేరు T>
నిర్మాణం Extension
{
  గుర్తులేని పూర్ణాంకం get_type () స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_type ();
    అప్రమేయ:తిరిగి 0;
    }
  }
  మూస <రకం_పేరు X>
  స్థిరరాశి X& get_subtable () స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.మూస get_subtable<రకం_పేరు T::Sub_పట్టిక> ();
    అప్రమేయ:తిరిగి Null (రకం_పేరు T::Sub_పట్టిక);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  మూస <రకం_పేరు ...Ts>
  రకం_పేరు hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.subset (c);
    అప్రమేయ: తిరిగి c->default_return_విలువ ();
    }
  }

  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, Ts&&... ds) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!c->may_dispatch (ఇది, &u.format.v))) తిరిగి c->no_dispatch_return_విలువ ();
    TRACE_DISPATCH (ఇది, u.format.v);
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    అప్రమేయ:return_trace (c->default_return_విలువ ());
    }
  }

  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT16 v; }	format;		/* Format identifier */
  ExtensionFormat1<T>	format1;
  } u;
};


/*
 * GSUB/GPOS Common
 */

నిర్మాణం hb_ot_layout_lookup_accelerator_t
{
  మూస <రకం_పేరు TLookup>
  స్థిర hb_ot_layout_lookup_accelerator_t *create (స్థిరరాశి TLookup &lookup)
  {
    గుర్తులేని లెక్క = lookup.get_subtable_లెక్క ();

    గుర్తులేని కొలత = పరిమాణం (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * పరిమాణం (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    లెక్క * పరిమాణం (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    స్వయంచాలక *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, కొలత);
    ఒకవేళ (unlikely (!thiz))
      తిరిగి శూన్యసూచిక;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    అయితే (లెక్క && !thiz->subtables[లెక్క - 1].apply_func)
      లెక్క--;

    thiz->digest.init ();
    కొరకు (స్వయంచాలక& subtable : hb_iter (thiz->subtables, లెక్క))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_రెండవ = c_accelerate_subtables.digest_రెండవ;

    thiz->లెక్క = లెక్క;

#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      ఒకవేళ (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#అయితే_ముగింపు

    తిరిగి thiz;
  }

  శూన్య fini ()
  {
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      hb_free (subtables[i].external_cache);
#అయితే_ముగింపు
  }

  తార్కిక may_have (hb_codepoint_t g) స్థిరరాశి
  { తిరిగి digest.may_have (g); }

#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  తార్కిక apply (hb_ot_apply_context_t *c, తార్కిక use_cache) స్థిరరాశి
  {
    ఒకవేళ (లెక్క == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
      ఒకవేళ (use_cache)
	తిరిగి subtables[0].apply_cached_no_digest (c);
#అయితే_ముగింపు
      తిరిగి subtables[0].apply_no_digest (c);
    }
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ఒకవేళ (use_cache)
    {
      తిరిగి
      + hb_iter (hb_iter (subtables, లెక్క))
      | hb_map ([&c] (స్థిరరాశి hb_accelerate_subtables_context_t::hb_applicable_t &_) { తిరిగి _.apply_cached (c); })
      | hb_any
      ;
    }
    లేకపోతే
#అయితే_ముగింపు
    {
      తిరిగి
      + hb_iter (hb_iter (subtables, లెక్క))
      | hb_map ([&c] (స్థిరరాశి hb_accelerate_subtables_context_t::hb_applicable_t &_) { తిరిగి _.apply (c); })
      | hb_any
      ;
    }
    తిరిగి అబద్ధం;
  }

  తార్కిక cache_enter (hb_ot_apply_context_t *c) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    తిరిగి subtable_cache_user_idx != (గుర్తులేని) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#లేకపోతే
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
  }
  శూన్య cache_leave (hb_ot_apply_context_t *c) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#అయితే_ముగింపు
  }


  hb_సమితి_digest_t digest;
  hb_సమితి_digest_t digest_రెండవ;
  అంతరంగ:
  గుర్తులేని లెక్క = 0; /* Number of subtables in the array. */
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
  గుర్తులేని subtable_cache_user_idx = (గుర్తులేని) -1;
#అయితే_ముగింపు
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

నిర్మాణం GSUBGPOS
{
  ఉపయోగించి LookupListT = LookupList<SmallTypes>;
  ఉపయోగించి LookupList2T = LookupList<MediumTypes>;
  మూస <రకం_పేరు TLookup>
  ఉపయోగించి TLookupList = List16_యొక్క_స్థానభ్రంశం_గమ్యం<TLookup, రకం_పేరు SmallTypes::HBLUINT>;
  మూస <రకం_పేరు TLookup>
  ఉపయోగించి TLookupList2 = List16_యొక్క_స్థానభ్రంశం_గమ్యం<TLookup, రకం_పేరు MediumTypes::HBLUINT>;
  మూస <రకం_పేరు TLookup>
  ఉపయోగించి TLookup_స్థానభ్రంశం_List = Lookup_స్థానభ్రంశం_List<TLookup, రకం_పేరు SmallTypes::HBLUINT>;
  మూస <రకం_పేరు TLookup>
  ఉపయోగించి TLookup_స్థానభ్రంశం_List2 = Lookup_స్థానభ్రంశం_List<TLookup, రకం_పేరు MediumTypes::HBLUINT>;

  size_t get_కొలత () స్థిరరాశి
  {
    ఒకవేళ (సంస్కరణ.major != 1) తిరిగి సంస్కరణ.static_కొలత;

    తిరిగి min_కొలత +
	   (సంస్కరణ.గమ్యం_int () >= 0x00010001u ? featureVars.static_కొలత : 0) +
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
	   (సంస్కరణ.గమ్యం_int () >= 0x00010002u ? 3 * Offset32::static_కొలత : 0)
#లేకపోతే
	   0
#అయితే_ముగింపు
	   ;
  }

  స్థిరరాశి ScriptList &get_script_list () స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010002u && scriptList2) తిరిగి ఇది + scriptList2;
#అయితే_ముగింపు
    తిరిగి ఇది + scriptList;
  }
  స్థిరరాశి FeatureList &get_feature_list () స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010002u && featureList2) తిరిగి ఇది + featureList2;
#అయితే_ముగింపు
    తిరిగి ఇది + featureList;
  }
  మూస <రకం_పేరు TLookup>
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!సంస్కరణ.sanitize (c) ||
		  సంస్కరణ.major != 1 ||
		  !c->check_range (ఇది, get_కొలత ())))
      return_trace (అబద్ధం);
    hb_barrier ();

    తార్కిక script_list_ok = scriptList.sanitize (c, ఇది);
    తార్కిక feature_list_ok = featureList.sanitize (c, ఇది);
    తార్కిక lookup_list_ok = typed_lookup_list_స్థానభ్రంశం<TLookup> ().sanitize (c, ఇది);

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010002u)
    {
      ఒకవేళ (scriptList2) script_list_ok = scriptList2.sanitize (c, ఇది);
      ఒకవేళ (featureList2) feature_list_ok = featureList2.sanitize (c, ఇది);
      ఒకవేళ (lookupList2) lookup_list_ok = typed_lookup_list2_స్థానభ్రంశం<TLookup> ().sanitize (c, ఇది);
    }
#అయితే_ముగింపు

    ఒకవేళ (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (అబద్ధం);

#నిర్వచితం_కాకపోతే HB_NO_VAR
    ఒకవేళ (unlikely (!(సంస్కరణ.గమ్యం_int () < 0x00010001u ||
		    featureVars.sanitize (c, ఇది, &get_feature_list ()))))
      return_trace (అబద్ధం);
#అయితే_ముగింపు

    return_trace (నిజం);
  }

  మూస <రకం_పేరు TLookup>
  తార్కిక subset (hb_subset_layout_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (సంస్కరణ.major != 1) return_trace (అబద్ధం);

    స్వయంచాలక *బహిర్గతం = c->subset_context->serializer->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!c->subset_context->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    బహిర్గతం->సంస్కరణ = సంస్కరణ;

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010002u)
    {
      ఒకవేళ (unlikely (!c->subset_context->serializer->extend_కొలత (బహిర్గతం, get_కొలత ())))
        return_trace (అబద్ధం);

      బహిర్గతం->scriptList = 0;
      బహిర్గతం->featureList = 0;
      బహిర్గతం->lookupList = 0;

      ఒకవేళ (lookupList2)
	serialize_subset_స్థానభ్రంశం (c->subset_context, బహిర్గతం->lookupList2,
				 subset_lookup_list2_స్థానభ్రంశం<TLookup> (),
				 ఇది, c);
      లేకపోతే
	serialize_subset_స్థానభ్రంశం (c->subset_context, బహిర్గతం->lookupList2,
				 subset_lookup_list_స్థానభ్రంశం<TLookup> (),
				 ఇది, c);

      ఒకవేళ (featureList2)
	serialize_subset_స్థానభ్రంశం (c->subset_context, బహిర్గతం->featureList2,
				 subset_feature_list2_స్థానభ్రంశం (),
				 ఇది, c);
      లేకపోతే
	serialize_subset_స్థానభ్రంశం (c->subset_context, బహిర్గతం->featureList2,
				 subset_feature_list_స్థానభ్రంశం (),
				 ఇది, c);

      ఒకవేళ (scriptList2)
	serialize_subset_స్థానభ్రంశం (c->subset_context, బహిర్గతం->scriptList2, scriptList2, ఇది, c);
      లేకపోతే
	serialize_subset_స్థానభ్రంశం (c->subset_context, బహిర్గతం->scriptList2, scriptList, ఇది, c);
    }
    లేకపోతే
#అయితే_ముగింపు
    {
      బహిర్గతం->subset_lookup_list_స్థానభ్రంశం<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_స్థానభ్రంశం<TLookup> (),
			     ఇది,
			     c);

      బహిర్గతం->subset_feature_list_స్థానభ్రంశం ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_స్థానభ్రంశం (),
			     ఇది,
			     c);

      బహిర్గతం->scriptList.serialize_subset (c->subset_context,
					scriptList,
					ఇది,
					c);
    }

#నిర్వచితం_కాకపోతే HB_NO_VAR
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010001u)
    {
      స్వయంచాలక snapshot = c->subset_context->serializer->snapshot ();
      ఒకవేళ (సంస్కరణ.గమ్యం_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&బహిర్గతం->featureVars))
        return_trace (అబద్ధం);

      // if all axes are pinned all feature vars are dropped.
      తార్కిక ret = !c->subset_context->plan->all_axes_pinned
                 && బహిర్గతం->featureVars.serialize_subset (c->subset_context, featureVars, ఇది, c);
      ఒకవేళ (!ret)
      {
        బహిర్గతం->featureVars = 0;
	ఒకవేళ (సంస్కరణ.గమ్యం_int () < 0x00010002u)
	{
	  c->subset_context->serializer->revert (snapshot);
	  బహిర్గతం->సంస్కరణ.major = 1;
	  బహిర్గతం->సంస్కరణ.minor = 0;
	}
      }
    }
#అయితే_ముగింపు

    return_trace (నిజం);
  }

  గుర్తులేని పూర్ణాంకం get_lookup_లెక్క () స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010002u && lookupList2)
      తిరిగి (ఇది + lookupList2).len;
#అయితే_ముగింపు
    తిరిగి (ఇది + lookupList).len;
  }
  స్థిరరాశి Lookup& get_lookup (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010002u && lookupList2)
      తిరిగి (ఇది + lookupList2)[i];
#అయితే_ముగింపు
    తిరిగి (ఇది + lookupList)[i];
  }
  స్థిరరాశి FeatureVariations &get_feature_variations () స్థిరరాశి
  {
    తిరిగి (సంస్కరణ.గమ్యం_int () >= 0x00010001u && hb_barrier ()) ? ఇది+featureVars : Null (FeatureVariations);
  }

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ.గమ్యం_int (); }
  గుర్తులేని పూర్ణాంకం get_script_లెక్క () స్థిరరాశి
  { తిరిగి get_script_list ().len; }
  స్థిరరాశి ట్యాగ్& get_script_ట్యాగ్ (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి get_script_list ().get_ట్యాగ్ (i); }
  గుర్తులేని పూర్ణాంకం get_script_ట్యాగులు (గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
				గుర్తులేని పూర్ణాంకం *script_లెక్క /* IN/OUT */,
				hb_ట్యాగ్_t     *script_ట్యాగులు /*బహిర్గతం*/) స్థిరరాశి
  { తిరిగి get_script_list ().get_ట్యాగులు (ప్రారంభం_స్థానభ్రంశం, script_లెక్క, script_ట్యాగులు); }
  స్థిరరాశి Script& get_script (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి get_script_list ()[i]; }
  తార్కిక కనుగొను_script_సూచీ (hb_ట్యాగ్_t ట్యాగ్_2, గుర్తులేని పూర్ణాంకం *సూచీ_2) స్థిరరాశి
  { తిరిగి get_script_list ().కనుగొను_సూచీ (ట్యాగ్_2, సూచీ_2); }

  గుర్తులేని పూర్ణాంకం get_feature_లెక్క () స్థిరరాశి
  { తిరిగి get_feature_list ().len; }
  hb_ట్యాగ్_t get_feature_ట్యాగ్ (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి i == సూచీ::NOT_దొరికింది_సూచీ ? HB_ట్యాగ్_NONE : get_feature_list ().get_ట్యాగ్ (i); }
  గుర్తులేని పూర్ణాంకం get_feature_ట్యాగులు (గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
				 గుర్తులేని పూర్ణాంకం *feature_లెక్క_2 /* IN/OUT */,
				 hb_ట్యాగ్_t     *feature_ట్యాగులు /*బహిర్గతం*/) స్థిరరాశి
  { తిరిగి get_feature_list ().get_ట్యాగులు (ప్రారంభం_స్థానభ్రంశం, feature_లెక్క_2, feature_ట్యాగులు); }
  స్థిరరాశి Feature& get_feature (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి get_feature_list ()[i]; }
  తార్కిక కనుగొను_feature_సూచీ (hb_ట్యాగ్_t ట్యాగ్_2, గుర్తులేని పూర్ణాంకం *సూచీ_2) స్థిరరాశి
  { తిరిగి get_feature_list ().కనుగొను_సూచీ (ట్యాగ్_2, సూచీ_2); }

  తార్కిక కనుగొను_variations_సూచీ (స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం అంకె_coords,
			      గుర్తులేని పూర్ణాంకం *సూచీ_2,
			      అంశం_Var_Store_Instancer *instancer) స్థిరరాశి
  {
#నిర్వచితమైతే HB_NO_VAR
    *సూచీ_2 = FeatureVariations::NOT_దొరికింది_సూచీ;
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
    తిరిగి get_feature_variations ().కనుగొను_సూచీ (coords, అంకె_coords, సూచీ_2, instancer);
  }
  తార్కిక get_feature_variations_state (స్థిరరాశి పూర్ణాంకం *coords,
				     గుర్తులేని పూర్ణాంకం అంకె_coords,
				     గుర్తులేని పూర్ణాంకం *సూచీ_2,
				     అంశం_Var_Store_Instancer *instancer,
				     hb_సదిశ_t<గుర్తులేని> *lookup_conditions) స్థిరరాశి
  {
    lookup_conditions->reset ();
#నిర్వచితమైతే HB_NO_VAR
    *సూచీ_2 = FeatureVariations::NOT_దొరికింది_సూచీ;
    తిరిగి నిజం;
#లేకపోతే
    కనుగొను_variations_సూచీ (coords, అంకె_coords, సూచీ_2, instancer);
    తిరిగి get_feature_variations ().collect_lookup_conditions (
	coords,
	అంకె_coords,
	instancer,
	lookup_conditions);
#అయితే_ముగింపు
  }
  తార్కిక get_lookup_variations (గుర్తులేని feature_సూచీ,
                              గుర్తులేని variations_సూచీ,
                              hb_sorted_array_t<స్థిరరాశి గుర్తులేని> conditions,
                              hb_సమితి_t *lookup_indexes) స్థిరరాశి
  {
#నిర్వచితమైతే HB_NO_VAR
    తిరిగి అబద్ధం;
#లేకపోతే
    తిరిగి get_feature_variations ().get_lookup_variations (
	feature_సూచీ, get_feature_variation (feature_సూచీ, variations_సూచీ),
	conditions, lookup_indexes);
#అయితే_ముగింపు
  }
  స్థిరరాశి Feature& get_feature_variation (గుర్తులేని పూర్ణాంకం feature_సూచీ,
					గుర్తులేని పూర్ణాంకం variations_సూచీ) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_VAR
    ఒకవేళ (FeatureVariations::NOT_దొరికింది_సూచీ != variations_సూచీ &&
	సంస్కరణ.గమ్యం_int () >= 0x00010001u)
    {
      స్థిరరాశి Feature *feature = get_feature_variations ().కనుగొను_substitute (variations_సూచీ,
									  feature_సూచీ);
      ఒకవేళ (feature)
	తిరిగి *feature;
    }
#అయితే_ముగింపు
    తిరిగి get_feature (feature_సూచీ);
  }

  శూన్య feature_variation_collect_lookups (స్థిరరాశి hb_సమితి_t *feature_indexes,
					  స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb::shared_ptr<hb_సమితి_t>> *feature_రికార్డు_cond_idx_map,
					  hb_సమితి_t       *lookup_indexes /*బహిర్గతం*/) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_రికార్డు_cond_idx_map, lookup_indexes);
#అయితే_ముగింపు
  }

  శూన్య collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#అయితే_ముగింపు
  }

#నిర్వచితం_కాకపోతే HB_NO_VAR
  శూన్య collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) స్థిరరాశి
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#అయితే_ముగింపు

  మూస <రకం_పేరు TLookup>
  శూన్య సంవృతం_lookups (hb_face_t      *face,
			స్థిరరాశి hb_సమితి_t *glyphs,
			hb_సమితి_t       *lookup_indexes /* IN/OUT */) స్థిరరాశి
  {
    hb_సమితి_t సందర్శించిన_lookups, inactive_lookups;
    hb_సంవృతం_lookups_context_t c (face, glyphs, &సందర్శించిన_lookups, &inactive_lookups);

    c.సమితి_recurse_func (TLookup::మూస dispatch_recurse_func<hb_సంవృతం_lookups_context_t>);

    కొరకు (గుర్తులేని lookup_సూచీ : *lookup_indexes)
      reinterpret_cast<స్థిరరాశి TLookup &> (get_lookup (lookup_సూచీ)).సంవృతం_lookups (&c, lookup_సూచీ);

    hb_సమితి_union (lookup_indexes, &సందర్శించిన_lookups);
    hb_సమితి_వ్యవకలనం (lookup_indexes, &inactive_lookups);
  }

  శూన్య prune_langsys (స్థిరరాశి hb_map_t *duplicate_feature_map,
                      స్థిరరాశి hb_సమితి_t *layout_scripts,
                      hb_hashmap_t<గుర్తులేని, hb::unique_ptr<hb_సమితి_t>> *script_langsys_map,
                      hb_సమితి_t       *new_feature_indexes /*బహిర్గతం*/) స్థిరరాశి
  {
    hb_prune_langsys_context_t c (ఇది, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    గుర్తులేని లెక్క = get_script_లెక్క ();
    కొరకు (గుర్తులేని script_సూచీ = 0; script_సూచీ < లెక్క; script_సూచీ++)
    {
      స్థిరరాశి ట్యాగ్& ట్యాగ్_2 = get_script_ట్యాగ్ (script_సూచీ);
      ఒకవేళ (!layout_scripts->has (ట్యాగ్_2)) కొనసాగు;
      స్థిరరాశి Script& s = get_script (script_సూచీ);
      s.prune_langsys (&c, script_సూచీ);
    }
  }

  శూన్య prune_features (స్థిరరాశి hb_map_t *lookup_indices, /*లోపల*/
		       స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb::shared_ptr<hb_సమితి_t>> *feature_రికార్డు_cond_idx_map, /*లోపల*/
		       స్థిరరాశి hb_hashmap_t<గుర్తులేని, స్థిరరాశి Feature*> *feature_substitutes_map, /*లోపల*/
		       hb_సమితి_t       *feature_indices /* IN/OUT */) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_VAR
    // This is the set of feature indices which have alternate versions defined
    // if the FeatureVariation's table and the alternate version(s) intersect the
    // set of lookup indices.
    hb_సమితి_t alternate_feature_indices;
    get_feature_variations ().సంవృతం_features (lookup_indices, feature_రికార్డు_cond_idx_map, &alternate_feature_indices);
    ఒకవేళ (unlikely (alternate_feature_indices.లోపల_error()))
    {
      feature_indices->err ();
      తిరిగి;
    }
#అయితే_ముగింపు

    కొరకు (గుర్తులేని i : hb_iter (feature_indices))
    {
      hb_ట్యాగ్_t ట్యాగ్_2 =  get_feature_ట్యాగ్ (i);
      ఒకవేళ (ట్యాగ్_2 == HB_ట్యాగ్ ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        కొనసాగు;


      స్థిరరాశి Feature *f = &(get_feature (i));
      స్థిరరాశి Feature** p = శూన్యసూచిక;
      ఒకవేళ (feature_substitutes_map->has (i, &p))
        f = *p;

      ఒకవేళ (!f->featureParams.is_null () &&
          ట్యాగ్_2 == HB_ట్యాగ్ ('s', 'i', 'z', 'e'))
        కొనసాగు;

      ఒకవేళ (!f->intersects_lookup_indexes (lookup_indices)
#నిర్వచితం_కాకపోతే HB_NO_VAR
          && !alternate_feature_indices.has (i)
#అయితే_ముగింపు
	  )
	feature_indices->del (i);
    }
  }

  శూన్య collect_name_ids (స్థిరరాశి hb_map_t *feature_సూచీ_map,
                         hb_సమితి_t *nameids_గమ్యం_retain, /*బహిర్గతం*/
                         స్థిరరాశి hb_hashmap_t<గుర్తులేని, స్థిరరాశి Feature*> *feature_substitutes_map,
                         స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb::shared_ptr<hb_సమితి_t>> *feature_రికార్డు_cond_idx_map HB_వాడని,
                         స్థిరరాశి hb_సమితి_t *catch_all_features,
                         తార్కిక all_axes_pinned HB_వాడని) స్థిరరాశి
  {
    గుర్తులేని లెక్క = get_feature_లెక్క ();
    కొరకు (గుర్తులేని i = 0 ; i < లెక్క; i++)
    {
      ఒకవేళ (!feature_సూచీ_map->has (i)) కొనసాగు;
      hb_ట్యాగ్_t ట్యాగ్_2 = get_feature_ట్యాగ్ (i);
      స్థిరరాశి Feature *feature = &get_feature (i);
      స్థిరరాశి Feature **substitute;
      ఒకవేళ (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        ఒకవేళ (catch_all_features->has (i))
          get_feature (i).collect_name_ids (ట్యాగ్_2, nameids_గమ్యం_retain);
      }
      feature->collect_name_ids (ట్యాగ్_2, nameids_గమ్యం_retain);
    }
#నిర్వచితం_కాకపోతే HB_NO_VAR
    ఒకవేళ (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_సూచీ_map, feature_రికార్డు_cond_idx_map,
                                                  [ఇది] (గుర్తులేని i) { తిరిగి get_feature_ట్యాగ్ (i); },
                                                  nameids_గమ్యం_retain);
#అయితే_ముగింపు
  }

  మూస <రకం_పేరు T>
  నిర్మాణం accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      sc.lazy_some_gpos = నిజం;
      ఇది->పట్టిక = sc.reference_పట్టిక<T> (face);

      ఒకవేళ (unlikely (ఇది->పట్టిక->is_blocklisted (ఇది->పట్టిక.get_blob (), face)))
      {
	hb_blob_destroy (ఇది->పట్టిక.get_blob ());
	ఇది->పట్టిక = hb_blob_get_ఖాళీలు ();
      }

      ఇది->lookup_లెక్క = పట్టిక->get_lookup_లెక్క ();

      ఇది->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (ఇది->lookup_లెక్క, పరిమాణం (*accels));
      ఒకవేళ (unlikely (!ఇది->accels))
      {
	ఇది->lookup_లెక్క = 0;
	ఇది->పట్టిక.destroy ();
	ఇది->పట్టిక = hb_blob_get_ఖాళీలు ();
      }
    }
    ~accelerator_t ()
    {
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ఇది->lookup_లెక్క; i++)
      {
	స్వయంచాలక *accel = ఇది->accels[i].get_relaxed ();
	ఒకవేళ (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (ఇది->accels);
      ఇది->పట్టిక.destroy ();
    }

    hb_blob_t *get_blob () స్థిరరాశి { తిరిగి పట్టిక.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (గుర్తులేని lookup_సూచీ) స్థిరరాశి
    {
      ఒకవేళ (unlikely (lookup_సూచీ >= lookup_లెక్క)) తిరిగి శూన్యసూచిక;

    retry:
      స్వయంచాలక *accel = accels[lookup_సూచీ].get_acquire ();
      ఒకవేళ (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (పట్టిక->get_lookup (lookup_సూచీ));
	ఒకవేళ (unlikely (!accel))
	  తిరిగి శూన్యసూచిక;

	ఒకవేళ (unlikely (!accels[lookup_సూచీ].cmpexch (శూన్యసూచిక, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  వెళ్ళు retry;
	}
      }

      తిరిగి accel;
    }

    hb_blob_ptr_t<T> పట్టిక;
    గుర్తులేని పూర్ణాంకం lookup_లెక్క;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  రక్షిత:
  మూస <రకం_పేరు TLookup>
  స్థిరరాశి Offset16_గమ్యం<TLookupList<TLookup>>& typed_lookup_list_స్థానభ్రంశం () స్థిరరాశి
  { తిరిగి reinterpret_cast<స్థిరరాశి Offset16_గమ్యం<TLookupList<TLookup>>&> (lookupList); }

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  మూస <రకం_పేరు TLookup>
  స్థిరరాశి Offset32_గమ్యం<TLookupList2<TLookup>>& typed_lookup_list2_స్థానభ్రంశం () స్థిరరాశి
  { తిరిగి reinterpret_cast<స్థిరరాశి Offset32_గమ్యం<TLookupList2<TLookup>>&> (lookupList2); }
#అయితే_ముగింపు

  మూస <రకం_పేరు TLookup>
  స్థిరరాశి Offset16_గమ్యం<TLookup_స్థానభ్రంశం_List<TLookup>>& subset_lookup_list_స్థానభ్రంశం () స్థిరరాశి
  { తిరిగి reinterpret_cast<స్థిరరాశి Offset16_గమ్యం<TLookup_స్థానభ్రంశం_List<TLookup>>&> (lookupList); }
  మూస <రకం_పేరు TLookup>
  Offset16_గమ్యం<TLookup_స్థానభ్రంశం_List<TLookup>>& subset_lookup_list_స్థానభ్రంశం ()
  { తిరిగి reinterpret_cast<Offset16_గమ్యం<TLookup_స్థానభ్రంశం_List<TLookup>>&> (lookupList); }

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  మూస <రకం_పేరు TLookup>
  స్థిరరాశి Offset32_గమ్యం<TLookup_స్థానభ్రంశం_List2<TLookup>>& subset_lookup_list2_స్థానభ్రంశం () స్థిరరాశి
  { తిరిగి reinterpret_cast<స్థిరరాశి Offset32_గమ్యం<TLookup_స్థానభ్రంశం_List2<TLookup>>&> (lookupList2); }
#అయితే_ముగింపు

  స్థిరరాశి Offset16_గమ్యం<రికార్డు_List_యొక్క_Feature>& subset_feature_list_స్థానభ్రంశం () స్థిరరాశి
  { తిరిగి reinterpret_cast<స్థిరరాశి Offset16_గమ్యం<రికార్డు_List_యొక్క_Feature>&> (featureList); }
  Offset16_గమ్యం<రికార్డు_List_యొక్క_Feature>& subset_feature_list_స్థానభ్రంశం ()
  { తిరిగి reinterpret_cast<Offset16_గమ్యం<రికార్డు_List_యొక్క_Feature>&> (featureList); }

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  స్థిరరాశి Offset32_గమ్యం<రికార్డు_List_యొక్క_Feature>& subset_feature_list2_స్థానభ్రంశం () స్థిరరాశి
  { తిరిగి reinterpret_cast<స్థిరరాశి Offset32_గమ్యం<రికార్డు_List_యొక్క_Feature>&> (featureList2); }
#అయితే_ముగింపు

  స్థిరరాశి శూన్య* get_lookup_list_క్షేత్రం_స్థానభ్రంశం () స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010002u && lookupList2) తిరిగి &lookupList2;
#అయితే_ముగింపు
    తిరిగి &lookupList;
  }

  మూస <రకం_పేరు స్థానభ్రంశం_బహిర్గతం, రకం_పేరు స్థానభ్రంశం_లోపల, రకం_పేరు పునాది_3, రకం_పేరు ...Ts>
  స్థిర తార్కిక serialize_subset_స్థానభ్రంశం (hb_subset_context_t *c,
				       స్థానభ్రంశం_బహిర్గతం& బహిర్గతం,
				       స్థిరరాశి స్థానభ్రంశం_లోపల& లోపల,
				       స్థిరరాశి పునాది_3 *పునాది,
				       Ts&&... ds)
  {
    బహిర్గతం = 0;
    ఒకవేళ (లోపల.is_null ()) తిరిగి అబద్ధం;

    స్వయంచాలక *s = c->serializer;
    s->push ();
    తార్కిక ret = c->dispatch (పునాది+లోపల, std::forward<Ts> (ds)...);
    ఒకవేళ (ret) s->కూడు_link (బహిర్గతం, s->తీయి_pack ());
    లేకపోతే s->తీయి_discard ();
    తిరిగి ret;
  }

  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version of the GSUB/GPOS table--initially set
				 * to 0x00010000u */
  Offset16_గమ్యం<ScriptList>
		scriptList;	/* Offset to ScriptList table */
  Offset16_గమ్యం<FeatureList>
		featureList;	/* Offset to FeatureList table */
  Offset16_గమ్యం<LookupListT>
		lookupList;	/* Offset to LookupList table */
  Offset32_గమ్యం<FeatureVariations>
		featureVars;	/* Offset to Feature Variations table--from
				 * beginning of table (may be NULL).
				 * Introduced in version 0x00010001. */
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  Offset32_గమ్యం<ScriptList>
		scriptList2;	/* 32-bit offset to ScriptList table.
				 * Introduced in version 0x00010002. */
  Offset32_గమ్యం<FeatureList>
		featureList2;	/* 32-bit offset to FeatureList table.
				 * Introduced in version 0x00010002. */
  Offset32_గమ్యం<LookupList2T>
		lookupList2;	/* 32-bit offset to LookupList2 table.
				 * Introduced in version 0x00010002. */
#అయితే_ముగింపు
  బహిరంగ:
  DEFINE_కొలత_MIN (10);
};


} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_LAYOUT_GSUBGPOS_HH */
