/*
 * Copyright © 2017  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_AAT_LAYOUT_COMMON_HH
#నిర్వచించు HB_AAT_LAYOUT_COMMON_HH

#అన్తర్భావ "hb-aat-layout.hh"
#అన్తర్భావ "hb-aat-map.hh"
#అన్తర్భావ "hb-ot-layout-common.hh"
#అన్తర్భావ "hb-ot-layout-gdef-table.hh"
#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-cache.hh"
#అన్తర్భావ "hb-bit-set.hh"
#అన్తర్భావ "hb-bit-page.hh"


నామాకాశ OT {
నిర్మాణం GDEF;
};

నామాకాశ AAT {

ఉపయోగించి నామాకాశ OT;

నిర్మాణం ankr;

ఉపయోగించి hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
#నిర్వచించు HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#లేకపోతే
#నిర్వచించు HB_AAT_TRANSITION_INLINE
#అయితే_ముగింపు

నిర్మాణం hb_aat_scratch_t
{
  hb_aat_scratch_t () = అప్రమేయ;
  hb_aat_scratch_t (స్థిరరాశి hb_aat_scratch_t &) = తొలగించు;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    బఫరు_glyph_సమితి.సమితి_relaxed (o.బఫరు_glyph_సమితి.get_relaxed ());
    o.బఫరు_glyph_సమితి.సమితి_relaxed (శూన్యసూచిక);
  }
  hb_aat_scratch_t & క్రియాకారకం = (hb_aat_scratch_t &&o)
  {
    బఫరు_glyph_సమితి.సమితి_relaxed (o.బఫరు_glyph_సమితి.get_relaxed ());
    o.బఫరు_glyph_సమితి.సమితి_relaxed (శూన్యసూచిక);
    తిరిగి *ఇది;
  }
  ~hb_aat_scratch_t ()
  {
    స్వయంచాలక *s = బఫరు_glyph_సమితి.get_relaxed ();
    ఒకవేళ (unlikely (!s))
      తిరిగి;
    s->fini ();
    hb_free (s);
  }

  hb_బిట్టు_సమితి_t *create_బఫరు_glyph_సమితి () స్థిరరాశి
  {
    hb_బిట్టు_సమితి_t *s = బఫరు_glyph_సమితి.get_acquire ();
    ఒకవేళ (s && బఫరు_glyph_సమితి.cmpexch (s, శూన్యసూచిక))
    {
      s->clear ();
      తిరిగి s;
    }

    s = (hb_బిట్టు_సమితి_t *) hb_calloc (1, పరిమాణం (hb_బిట్టు_సమితి_t));
    ఒకవేళ (unlikely (!s))
      తిరిగి శూన్యసూచిక;
    s->init ();

    తిరిగి s;
  }
  శూన్య destroy_బఫరు_glyph_సమితి (hb_బిట్టు_సమితి_t *s) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!s))
      తిరిగి;
    ఒకవేళ (బఫరు_glyph_సమితి.cmpexch (శూన్యసూచిక, s))
      తిరిగి;
    s->fini ();
    hb_free (s);
  }

  మార్చదగిన hb_atomic_t<hb_బిట్టు_సమితి_t *> బఫరు_glyph_సమితి;
};

గణన { DELETED_GLYPH = 0xFFFF };

#నిర్వచించు HB_బఫరు_SCRATCH_FLAG_AAT_HAS_DELETED HB_బఫరు_SCRATCH_FLAG_SHAPER0

నిర్మాణం hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, తార్కిక, HB_DEBUG_APPLY>
{
  స్థిరరాశి అక్షరం *get_name () { తిరిగి "APPLY"; }
  మూస <రకం_పేరు T, రకం_పేరు ...Ts>
  return_t dispatch (స్థిరరాశి T &obj, Ts&&... ds)
  { తిరిగి obj.apply (ఇది, std::forward<Ts> (ds)...); }
  స్థిర return_t default_return_విలువ () { తిరిగి అబద్ధం; }
  తార్కిక stop_sublookup_iteration (return_t r) స్థిరరాశి { తిరిగి r; }

  స్థిరరాశి hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_బఫరు_t *బఫరు;
  hb_sanitize_context_t sanitizer;
  స్థిరరాశి ankr *ankr_పట్టిక;
  స్థిరరాశి OT::GDEF &gdef;
  తార్కిక has_glyph_classes;
  స్థిరరాశి hb_sorted_సదిశ_t<hb_aat_map_t::range_flags_t> *range_flags = శూన్యసూచిక;
  hb_ముసుగు_t subtable_flags = 0;
  తార్కిక బఫరు_is_reversed = అబద్ధం;
  // Caches
  తార్కిక using_బఫరు_glyph_సమితి = అబద్ధం;
  hb_బిట్టు_సమితి_t *బఫరు_glyph_సమితి = శూన్యసూచిక;
  స్థిరరాశి hb_బిట్టు_సమితి_t *మొదటి_సమితి = శూన్యసూచిక;
  స్థిరరాశి hb_బిట్టు_సమితి_t *రెండవ_సమితి = శూన్యసూచిక;
  hb_aat_class_cache_t *machine_class_cache = శూన్యసూచిక;

  /* Unused. For debug tracing only. */
  గుర్తులేని పూర్ణాంకం lookup_సూచీ;

  HB_అంతర్గత hb_aat_apply_context_t (స్థిరరాశి hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_బఫరు_t *బఫరు_2,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_అంతర్గత ~hb_aat_apply_context_t ();

  HB_అంతర్గత శూన్య సమితి_ankr_పట్టిక (స్థిరరాశి AAT::ankr *ankr_పట్టిక_2);

  శూన్య సమితి_lookup_సూచీ (గుర్తులేని పూర్ణాంకం i) { lookup_సూచీ = i; }

  శూన్య తిరగేయి_బఫరు ()
  {
    బఫరు->reverse ();
    బఫరు_is_reversed = !బఫరు_is_reversed;
  }

  శూన్య అమరిక_బఫరు_glyph_సమితి ()
  {
    using_బఫరు_glyph_సమితి = బఫరు->len >= 4 && బఫరు_glyph_సమితి;

    ఒకవేళ (likely (using_బఫరు_glyph_సమితి))
      బఫరు->collect_codepoints (*బఫరు_glyph_సమితి);
  }
  తార్కిక బఫరు_intersects_machine () స్థిరరాశి
  {
    ఒకవేళ (likely (using_బఫరు_glyph_సమితి))
      తిరిగి బఫరు_glyph_సమితి->intersects (*మొదటి_సమితి);

    // Faster for shorter buffers.
    కొరకు (గుర్తులేని i = 0; i < బఫరు->len; i++)
      ఒకవేళ (మొదటి_సమితి->has (బఫరు->info[i].codepoint))
	తిరిగి నిజం;
    తిరిగి అబద్ధం;
  }

  మూస <రకం_పేరు T>
  HB_NODISCARD తార్కిక output_glyphs (గుర్తులేని పూర్ణాంకం లెక్క,
				   స్థిరరాశి T *glyphs)
  {
    ఒకవేళ (likely (using_బఫరు_glyph_సమితి))
      బఫరు_glyph_సమితి->కూడు_array (glyphs, లెక్క);
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      తార్కిక at_చివర_యొక్క_పాఠ్యం = బఫరు->idx == బఫరు->len;
      ఒకవేళ (at_చివర_యొక్క_పాఠ్యం)
	ఒకవేళ (unlikely (!బఫరు->output_glyph (glyphs[i]))) తిరిగి అబద్ధం;

      hb_glyph_info_t &info = at_చివర_యొక్క_పాఠ్యం ? బఫరు->prev () : బఫరు->cur ();
      ఒకవేళ (glyphs[i] == DELETED_GLYPH)
      {
        బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_AAT_HAS_DELETED;
	hb_glyph_info_సమితి_aat_deleted (&info);
      }
      లేకపోతే
      {
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT
	ఒకవేళ (has_glyph_classes)
	  hb_glyph_info_సమితి_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#అయితే_ముగింపు
      }
      ఒకవేళ (!at_చివర_యొక్క_పాఠ్యం)
	ఒకవేళ (unlikely (!బఫరు->output_glyph (glyphs[i]))) తిరిగి అబద్ధం;
    }
    తిరిగి నిజం;
  }

  HB_NODISCARD తార్కిక replace_glyph (hb_codepoint_t glyph)
  {
    ఒకవేళ (glyph == DELETED_GLYPH)
    {
      బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_AAT_HAS_DELETED;
      hb_glyph_info_సమితి_aat_deleted (&బఫరు->cur());
    }

    ఒకవేళ (likely (using_బఫరు_glyph_సమితి))
      బఫరు_glyph_సమితి->కూడు (glyph);
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT
    ఒకవేళ (has_glyph_classes)
      hb_glyph_info_సమితి_glyph_props (&బఫరు->cur(),
				      gdef.get_glyph_props (glyph));
#అయితే_ముగింపు
    తిరిగి బఫరు->replace_glyph (glyph);
  }

  HB_NODISCARD తార్కిక delete_glyph ()
  {
    బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_AAT_HAS_DELETED;
    hb_glyph_info_సమితి_aat_deleted (&బఫరు->cur());
    ఒకవేళ (likely (using_బఫరు_glyph_సమితి))
      బఫరు_glyph_సమితి->కూడు (DELETED_GLYPH);
    తిరిగి బఫరు->replace_glyph (DELETED_GLYPH);
  }

  శూన్య replace_glyph_inplace (గుర్తులేని i, hb_codepoint_t glyph)
  {
    బఫరు->info[i].codepoint = glyph;
    ఒకవేళ (glyph == DELETED_GLYPH)
    {
      బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_AAT_HAS_DELETED;
      hb_glyph_info_సమితి_aat_deleted (&బఫరు->info[i]);
    }
    ఒకవేళ (likely (using_బఫరు_glyph_సమితి))
      బఫరు_glyph_సమితి->కూడు (glyph);
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT
    ఒకవేళ (has_glyph_classes)
      hb_glyph_info_సమితి_glyph_props (&బఫరు->info[i],
				      gdef.get_glyph_props (glyph));
#అయితే_ముగింపు
  }
};


/*
 * Lookup Table
 */

మూస <రకం_పేరు T> నిర్మాణం Lookup;

మూస <రకం_పేరు T>
నిర్మాణం LookupFormat0
{
  మిత్ర నిర్మాణం Lookup<T>;

  అంతరంగ:
  స్థిరరాశి T* get_విలువ (hb_codepoint_t glyph_id, గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  {
    ఒకవేళ (unlikely (glyph_id >= అంకె_glyphs)) తిరిగి శూన్యసూచిక;
    తిరిగి &arrayZ[glyph_id];
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &glyphs, గుర్తులేని అంకె_glyphs) స్థిరరాశి
  {
    glyphs.కూడు_range (0, అంకె_glyphs - 1);
  }
  మూస <రకం_పేరు సమితి_t, రకం_పేరు filter_t>
  శూన్య collect_glyphs_filtered (సమితి_t &glyphs, గుర్తులేని అంకె_glyphs, స్థిరరాశి filter_t &filter) స్థిరరాశి
  {
    కొరకు (గుర్తులేని i = 0; i < అంకె_glyphs; i++)
      ఒకవేళ (filter (arrayZ[i]))
	glyphs.కూడు (i);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (arrayZ.sanitize (c, c->get_అంకె_glyphs ()));
  }
  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (arrayZ.sanitize (c, c->get_అంకె_glyphs (), పునాది));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier--format = 0 */
  Unsized_Array_యొక్క<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  బహిరంగ:
  DEFINE_కొలత_UNBOUNDED (2);
};


మూస <రకం_పేరు T>
నిర్మాణం LookupSegmentSingle
{
  స్థిర constexpr గుర్తులేని Termination_మాట_లెక్క = 2u;

  పూర్ణాంకం cmp (hb_codepoint_t g) స్థిరరాశి
  { తిరిగి g < మొదటి ? -1 : g <= చివరి ? 0 : +1 ; }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &glyphs) స్థిరరాశి
  {
    ఒకవేళ (మొదటి == DELETED_GLYPH) తిరిగి;
    glyphs.కూడు_range (మొదటి, చివరి);
  }
  మూస <రకం_పేరు సమితి_t, రకం_పేరు filter_t>
  శూన్య collect_glyphs_filtered (సమితి_t &glyphs, స్థిరరాశి filter_t &filter) స్థిరరాశి
  {
    ఒకవేళ (మొదటి == DELETED_GLYPH) తిరిగి;
    ఒకవేళ (!filter (value)) తిరిగి;
    glyphs.కూడు_range (మొదటి, చివరి);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && value.sanitize (c));
  }
  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && value.sanitize (c, పునాది));
  }

  HBGlyphID16	చివరి;		/* Last GlyphID in this segment */
  HBGlyphID16	మొదటి;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + T::static_కొలత);
};

మూస <రకం_పేరు T>
నిర్మాణం LookupFormat2
{
  మిత్ర నిర్మాణం Lookup<T>;

  అంతరంగ:
  స్థిరరాశి T* get_విలువ (hb_codepoint_t glyph_id) స్థిరరాశి
  {
    స్థిరరాశి LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    తిరిగి v ? &v->value : శూన్యసూచిక;
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &glyphs) స్థిరరాశి
  {
    గుర్తులేని లెక్క = segments.get_నిడివి ();
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      segments[i].collect_glyphs (glyphs);
  }
  మూస <రకం_పేరు సమితి_t, రకం_పేరు filter_t>
  శూన్య collect_glyphs_filtered (సమితి_t &glyphs, స్థిరరాశి filter_t &filter) స్థిరరాశి
  {
    గుర్తులేని లెక్క = segments.get_నిడివి ();
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (segments.sanitize (c));
  }
  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (segments.sanitize (c, పునాది));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_శోధన_Array_యొక్క<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (8, segments);
};

మూస <రకం_పేరు T>
నిర్మాణం LookupSegmentArray
{
  స్థిర constexpr గుర్తులేని Termination_మాట_లెక్క = 2u;

  స్థిరరాశి T* get_విలువ (hb_codepoint_t glyph_id, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    తిరిగి మొదటి <= glyph_id && glyph_id <= చివరి ? &(పునాది+విలువలు_Z)[glyph_id - మొదటి] : శూన్యసూచిక;
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &glyphs) స్థిరరాశి
  {
    ఒకవేళ (మొదటి == DELETED_GLYPH) తిరిగి;
    glyphs.కూడు_range (మొదటి, చివరి);
  }
  మూస <రకం_పేరు సమితి_t, రకం_పేరు filter_t>
  శూన్య collect_glyphs_filtered (సమితి_t &glyphs, స్థిరరాశి శూన్య *పునాది, స్థిరరాశి filter_t &filter) స్థిరరాశి
  {
    ఒకవేళ (మొదటి == DELETED_GLYPH) తిరిగి;
    స్థిరరాశి స్వయంచాలక &values = పునాది+విలువలు_Z;
    కొరకు (hb_codepoint_t i = మొదటి; i <= చివరి; i++)
      ఒకవేళ (filter (values[i - మొదటి]))
	glyphs.కూడు (i);
  }

  పూర్ణాంకం cmp (hb_codepoint_t g) స్థిరరాశి
  { తిరిగి g < మొదటి ? -1 : g <= చివరి ? 0 : +1; }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  మొదటి <= చివరి &&
		  విలువలు_Z.sanitize (c, పునాది, చివరి - మొదటి + 1));
  }
  మూస <రకం_పేరు ...Ts>
  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది, Ts&&... ds) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  మొదటి <= చివరి &&
		  విలువలు_Z.sanitize (c, పునాది, చివరి - మొదటి + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	చివరి;		/* Last GlyphID in this segment */
  HBGlyphID16	మొదటి;		/* First GlyphID in this segment */
  NNOffset16_గమ్యం<Unsized_Array_యొక్క<T>>
		విలువలు_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};

మూస <రకం_పేరు T>
నిర్మాణం LookupFormat4
{
  మిత్ర నిర్మాణం Lookup<T>;

  అంతరంగ:
  స్థిరరాశి T* get_విలువ (hb_codepoint_t glyph_id) స్థిరరాశి
  {
    స్థిరరాశి LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    తిరిగి v ? v->get_విలువ (glyph_id, ఇది) : శూన్యసూచిక;
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &glyphs) స్థిరరాశి
  {
    గుర్తులేని లెక్క = segments.get_నిడివి ();
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      segments[i].collect_glyphs (glyphs);
  }
  మూస <రకం_పేరు సమితి_t, రకం_పేరు filter_t>
  శూన్య collect_glyphs_filtered (సమితి_t &glyphs, స్థిరరాశి filter_t &filter) స్థిరరాశి
  {
    గుర్తులేని లెక్క = segments.get_నిడివి ();
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      segments[i].collect_glyphs_filtered (glyphs, ఇది, filter);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (segments.sanitize (c, ఇది));
  }
  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (segments.sanitize (c, ఇది, పునాది));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_శోధన_Array_యొక్క<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (8, segments);
};

మూస <రకం_పేరు T>
నిర్మాణం LookupSingle
{
  స్థిర constexpr గుర్తులేని Termination_మాట_లెక్క = 1u;

  పూర్ణాంకం cmp (hb_codepoint_t g) స్థిరరాశి { తిరిగి glyph.cmp (g); }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &glyphs) స్థిరరాశి
  {
    ఒకవేళ (glyph == DELETED_GLYPH) తిరిగి;
    glyphs.కూడు (glyph);
  }
  మూస <రకం_పేరు సమితి_t, రకం_పేరు filter_t>
  శూన్య collect_glyphs_filtered (సమితి_t &glyphs, స్థిరరాశి filter_t &filter) స్థిరరాశి
  {
    ఒకవేళ (glyph == DELETED_GLYPH) తిరిగి;
    ఒకవేళ (!filter (value)) తిరిగి;
    glyphs.కూడు (glyph);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && value.sanitize (c));
  }
  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && value.sanitize (c, పునాది));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  బహిరంగ:
  DEFINE_కొలత_STATIC (2 + T::static_కొలత);
};

మూస <రకం_పేరు T>
నిర్మాణం LookupFormat6
{
  మిత్ర నిర్మాణం Lookup<T>;

  అంతరంగ:
  స్థిరరాశి T* get_విలువ (hb_codepoint_t glyph_id) స్థిరరాశి
  {
    స్థిరరాశి LookupSingle<T> *v = entries.bsearch (glyph_id);
    తిరిగి v ? &v->value : శూన్యసూచిక;
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &glyphs) స్థిరరాశి
  {
    గుర్తులేని లెక్క = entries.get_నిడివి ();
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      entries[i].collect_glyphs (glyphs);
  }
  మూస <రకం_పేరు సమితి_t, రకం_పేరు filter_t>
  శూన్య collect_glyphs_filtered (సమితి_t &glyphs, స్థిరరాశి filter_t &filter) స్థిరరాశి
  {
    గుర్తులేని లెక్క = entries.get_నిడివి ();
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (entries.sanitize (c));
  }
  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (entries.sanitize (c, పునాది));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_శోధన_Array_యొక్క<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (8, entries);
};

మూస <రకం_పేరు T>
నిర్మాణం LookupFormat8
{
  మిత్ర నిర్మాణం Lookup<T>;

  అంతరంగ:
  స్థిరరాశి T* get_విలువ (hb_codepoint_t glyph_id) స్థిరరాశి
  {
    తిరిగి మొదటి_Glyph <= glyph_id && glyph_id - మొదటి_Glyph < glyph_లెక్క_2 ?
	   &విలువ_Array_Z[glyph_id - మొదటి_Glyph] : శూన్యసూచిక;
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &glyphs) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!glyph_లెక్క_2)) తిరిగి;
    ఒకవేళ (మొదటి_Glyph == DELETED_GLYPH) తిరిగి;
    glyphs.కూడు_range (మొదటి_Glyph, మొదటి_Glyph + glyph_లెక్క_2 - 1);
  }
  మూస <రకం_పేరు సమితి_t, రకం_పేరు filter_t>
  శూన్య collect_glyphs_filtered (సమితి_t &glyphs, స్థిరరాశి filter_t &filter) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!glyph_లెక్క_2)) తిరిగి;
    ఒకవేళ (మొదటి_Glyph == DELETED_GLYPH) తిరిగి;
    స్థిరరాశి T *p = విలువ_Array_Z.arrayZ;
    కొరకు (గుర్తులేని i = 0; i < glyph_లెక్క_2; i++)
      ఒకవేళ (filter (p[i]))
	glyphs.కూడు (మొదటి_Glyph + i);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && విలువ_Array_Z.sanitize (c, glyph_లెక్క_2));
  }
  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && విలువ_Array_Z.sanitize (c, glyph_లెక్క_2, పునాది));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	మొదటి_Glyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_లెక్క_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  Unsized_Array_యొక్క<T>
		విలువ_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (6, విలువ_Array_Z);
};

మూస <రకం_పేరు T>
నిర్మాణం LookupFormat10
{
  మిత్ర నిర్మాణం Lookup<T>;

  అంతరంగ:
  స్థిరరాశి రకం_పేరు T::type get_విలువ_or_null (hb_codepoint_t glyph_id) స్థిరరాశి
  {
    ఒకవేళ (!(మొదటి_Glyph <= glyph_id && glyph_id - మొదటి_Glyph < glyph_లెక్క_2))
      తిరిగి Null (T);

    స్థిరరాశి HBUINT8 *p = &విలువ_Array_Z[(glyph_id - మొదటి_Glyph) * విలువ_కొలత];

    గుర్తులేని పూర్ణాంకం v = 0;
    గుర్తులేని పూర్ణాంకం లెక్క = విలువ_కొలత;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      v = (v << 8) | *p++;

    తిరిగి v;
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &glyphs) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!glyph_లెక్క_2)) తిరిగి;
    ఒకవేళ (మొదటి_Glyph == DELETED_GLYPH) తిరిగి;
    glyphs.కూడు_range (మొదటి_Glyph, మొదటి_Glyph + glyph_లెక్క_2 - 1);
  }

  మూస <రకం_పేరు సమితి_t, రకం_పేరు filter_t>
  శూన్య collect_glyphs_filtered (సమితి_t &glyphs, స్థిరరాశి filter_t &filter) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!glyph_లెక్క_2)) తిరిగి;
    ఒకవేళ (మొదటి_Glyph == DELETED_GLYPH) తిరిగి;
    స్థిరరాశి HBUINT8 *p = విలువ_Array_Z.arrayZ;
    కొరకు (గుర్తులేని i = 0; i < glyph_లెక్క_2; i++)
    {
      గుర్తులేని పూర్ణాంకం v = 0;
      గుర్తులేని పూర్ణాంకం లెక్క = విలువ_కొలత;
      కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < లెక్క; j++)
	v = (v << 8) | *p++;
      ఒకవేళ (filter (v))
	glyphs.కూడు (మొదటి_Glyph + i);
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  విలువ_కొలత <= 4 &&
		  విలువ_Array_Z.sanitize (c, glyph_లెక్క_2 * విలువ_కొలత));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	విలువ_కొలత;	/* Byte size of each value. */
  HBGlyphID16	మొదటి_Glyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_లెక్క_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  Unsized_Array_యొక్క<HBUINT8>
		విలువ_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (8, విలువ_Array_Z);
};

మూస <రకం_పేరు T>
నిర్మాణం Lookup
{
  స్థిరరాశి T* get_విలువ (hb_codepoint_t glyph_id, గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 0: hb_barrier (); తిరిగి u.format0.get_విలువ (glyph_id, అంకె_glyphs);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.get_విలువ (glyph_id);
    స్థితి 4: hb_barrier (); తిరిగి u.format4.get_విలువ (glyph_id);
    స్థితి 6: hb_barrier (); తిరిగి u.format6.get_విలువ (glyph_id);
    స్థితి 8: hb_barrier (); తిరిగి u.format8.get_విలువ (glyph_id);
    అప్రమేయ:తిరిగి శూన్యసూచిక;
    }
  }

  స్థిరరాశి రకం_పేరు T::type get_విలువ_or_null (hb_codepoint_t glyph_id, గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
      /* Format 10 cannot return a pointer. */
      స్థితి 10: hb_barrier (); తిరిగి u.format10.get_విలువ_or_null (glyph_id);
      అప్రమేయ:
      స్థిరరాశి T *v = get_విలువ (glyph_id, అంకె_glyphs);
      తిరిగి v ? *v : Null (T);
    }
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &glyphs, గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 0: hb_barrier (); u.format0.collect_glyphs (glyphs, అంకె_glyphs); తిరిగి;
    స్థితి 2: hb_barrier (); u.format2.collect_glyphs (glyphs); తిరిగి;
    స్థితి 4: hb_barrier (); u.format4.collect_glyphs (glyphs); తిరిగి;
    స్థితి 6: hb_barrier (); u.format6.collect_glyphs (glyphs); తిరిగి;
    స్థితి 8: hb_barrier (); u.format8.collect_glyphs (glyphs); తిరిగి;
    స్థితి 10: hb_barrier (); u.format10.collect_glyphs (glyphs); తిరిగి;
    అప్రమేయ:తిరిగి;
    }
  }
  మూస <రకం_పేరు సమితి_t, రకం_పేరు filter_t>
  శూన్య collect_glyphs_filtered (సమితి_t &glyphs, గుర్తులేని అంకె_glyphs, స్థిరరాశి filter_t &filter) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, అంకె_glyphs, filter); తిరిగి;
    స్థితి 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); తిరిగి;
    స్థితి 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); తిరిగి;
    స్థితి 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); తిరిగి;
    స్థితి 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); తిరిగి;
    స్థితి 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); తిరిగి;
    అప్రమేయ:తిరిగి;
    }
  }

  రకం_పేరు T::type get_class (hb_codepoint_t glyph_id,
			      గుర్తులేని పూర్ణాంకం అంకె_glyphs,
			      గుర్తులేని పూర్ణాంకం బహిర్గతం_యొక్క_Range) స్థిరరాశి
  {
    స్థిరరాశి T *v = get_విలువ (glyph_id, అంకె_glyphs);
    తిరిగి v ? *v : బహిర్గతం_యొక్క_Range;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!u.format.v.sanitize (c)) return_trace (అబద్ధం);
    hb_barrier ();
    ఎంపిక (u.format.v) {
    స్థితి 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    స్థితి 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    స్థితి 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    స్థితి 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    స్థితి 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    స్థితి 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    అప్రమేయ:return_trace (నిజం);
    }
  }
  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!u.format.v.sanitize (c)) return_trace (అబద్ధం);
    hb_barrier ();
    ఎంపిక (u.format.v) {
    స్థితి 0: hb_barrier (); return_trace (u.format0.sanitize (c, పునాది));
    స్థితి 2: hb_barrier (); return_trace (u.format2.sanitize (c, పునాది));
    స్థితి 4: hb_barrier (); return_trace (u.format4.sanitize (c, పునాది));
    స్థితి 6: hb_barrier (); return_trace (u.format6.sanitize (c, పునాది));
    స్థితి 8: hb_barrier (); return_trace (u.format8.sanitize (c, పునాది));
    స్థితి 10: return_trace (అబద్ధం); /* We don't support format10 here currently. */
    అప్రమేయ:return_trace (నిజం);
    }
  }

  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT16 v; }	format;		/* Format identifier */
  LookupFormat0<T>	format0;
  LookupFormat2<T>	format2;
  LookupFormat4<T>	format4;
  LookupFormat6<T>	format6;
  LookupFormat8<T>	format8;
  LookupFormat10<T>	format10;
  } u;
  బహిరంగ:
  DEFINE_కొలత_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_బైట్లు_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

మూస <రకం_పేరు T>
నిర్మాణం నమోదు_3
{
  // This doesn't seem like it's ever called.
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    /* Note, we don't recurse-sanitize data because we don't access it.
     * That said, in our DEFINE_SIZE_STATIC we access T::static_size,
     * which ensures that data has a simple sanitize(). To be determined
     * if I need to remove that as well.
     *
     * HOWEVER! Because we are a template, our DEFINE_SIZE_STATIC
     * assertion wouldn't be checked, hence the line below. */
    static_assert (T::static_కొలత, "");

    return_trace (c->check_struct (ఇది));
  }

  బహిరంగ:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		దత్తాంశం;		/* Optional offsets to per-glyph tables. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + T::static_కొలత);
};

మూస <>
నిర్మాణం నమోదు_3<శూన్య>
{
  // This does seem like it's ever called.
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  బహిరంగ:
  HBUINT16	newState;	/* Byte offset from beginning of state table to the new state. */
  HBUINT16	flags;		/* Table specific. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

గణన Class
{
  CLASS_చివర_యొక్క_పాఠ్యం = 0,
  CLASS_బహిర్గతం_యొక్క_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_చివర_యొక్క_పంక్తి = 3,
};

మూస <రకం_పేరు Types, రకం_పేరు Extra>
నిర్మాణం State_పట్టిక
{
  రకనిర్వచనం రకం_పేరు Types::HBUINT HBUINT;
  రకనిర్వచనం రకం_పేరు Types::HBUSHORT HBUSHORT;
  రకనిర్వచనం రకం_పేరు Types::ClassTypeNarrow ClassType;

  గణన State
  {
    STATE_ప్రారంభం_యొక్క_పాఠ్యం = 0,
    STATE_ప్రారంభం_యొక్క_పంక్తి = 1,
  };

  మూస <రకం_పేరు సమితి_t, రకం_పేరు పట్టిక_t>
  శూన్య collect_initial_glyphs (సమితి_t &glyphs, గుర్తులేని అంకె_glyphs, స్థిరరాశి పట్టిక_t &పట్టిక) స్థిరరాశి
  {
    గుర్తులేని అంకె_classes = nClasses;

    ఒకవేళ (unlikely (అంకె_classes > hb_బిట్టు_పుట_t::బిట్లు_2))
    {
      (ఇది+class_పట్టిక).collect_glyphs (glyphs, అంకె_glyphs);
      తిరిగి;
    }

    // Collect all classes going out from the start state.
    hb_బిట్టు_పుట_t filter;

    కొరకు (గుర్తులేని i = 0; i < అంకె_classes; i++)
    {
      స్థిరరాశి స్వయంచాలక &నమోదు_2 = get_నమోదు (STATE_ప్రారంభం_యొక్క_పాఠ్యం, i);
      ఒకవేళ (new_state (నమోదు_2.newState) == STATE_ప్రారంభం_యొక్క_పాఠ్యం &&
	  !పట్టిక.is_action_initiable (నమోదు_2) && !పట్టిక.is_actionable (నమోదు_2))
	కొనసాగు;

      filter.కూడు (i);
    }

    // And glyphs in those classes.

    ఒకవేళ (filter (CLASS_DELETED_GLYPH))
      glyphs.కూడు (DELETED_GLYPH);

    (ఇది+class_పట్టిక).collect_glyphs_filtered (glyphs, అంకె_glyphs, filter);
  }

  పూర్ణాంకం new_state (గుర్తులేని పూర్ణాంకం newState) స్థిరరాశి
  { తిరిగి Types::extended ? newState : ((పూర్ణాంకం) newState - (పూర్ణాంకం) state_Array_పట్టిక) / (పూర్ణాంకం) nClasses; }

  గుర్తులేని పూర్ణాంకం get_class (hb_codepoint_t glyph_id,
			  గుర్తులేని పూర్ణాంకం అంకె_glyphs,
			  hb_aat_class_cache_t *cache = శూన్యసూచిక) స్థిరరాశి
  {
    గుర్తులేని klass;
    ఒకవేళ (cache && cache->get (glyph_id, &klass)) తిరిగి klass;
    ఒకవేళ (unlikely (glyph_id == DELETED_GLYPH)) తిరిగి CLASS_DELETED_GLYPH;
    klass = (ఇది+class_పట్టిక).get_class (glyph_id, అంకె_glyphs, CLASS_బహిర్గతం_యొక్క_BOUNDS);
    ఒకవేళ (cache) cache->సమితి (glyph_id, klass);
    తిరిగి klass;
  }

  స్థిరరాశి నమోదు_3<Extra> *get_entries () స్థిరరాశి
  { తిరిగి (ఇది+నమోదు_పట్టిక).arrayZ; }

  స్థిరరాశి నమోదు_3<Extra> &get_నమోదు (పూర్ణాంకం state, గుర్తులేని పూర్ణాంకం klass) స్థిరరాశి
  {
    గుర్తులేని n_classes = nClasses;
    ఒకవేళ (unlikely (klass >= n_classes))
      klass = CLASS_బహిర్గతం_యొక్క_BOUNDS;

    స్థిరరాశి HBUSHORT *స్థితులు = (ఇది+state_Array_పట్టిక).arrayZ;
    స్థిరరాశి నమోదు_3<Extra> *entries = (ఇది+నమోదు_పట్టిక).arrayZ;

    గుర్తులేని పూర్ణాంకం నమోదు_2 = స్థితులు[state * n_classes + klass];
    DEBUG_సందేశం (APPLY, శూన్యసూచిక, "e%u", నమోదు_2);

    తిరిగి entries[నమోదు_2];
  }

  తార్కిక sanitize (hb_sanitize_context_t *c,
		 గుర్తులేని పూర్ణాంకం *అంకె_entries_బహిర్గతం = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!(c->check_struct (ఇది) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    class_పట్టిక.sanitize (c, ఇది)))) return_trace (అబద్ధం);

    స్థిరరాశి HBUSHORT *స్థితులు = (ఇది+state_Array_పట్టిక).arrayZ;
    స్థిరరాశి నమోదు_3<Extra> *entries = (ఇది+నమోదు_పట్టిక).arrayZ;

    గుర్తులేని పూర్ణాంకం అంకె_classes = nClasses;
    ఒకవేళ (unlikely (hb_unsigned_mul_overflows (అంకె_classes, స్థితులు[0].static_కొలత)))
      return_trace (అబద్ధం);
    గుర్తులేని పూర్ణాంకం అడ్డువరుస_stride = అంకె_classes * స్థితులు[0].static_కొలత;

    /* Apple 'kern' table has this peculiarity:
     *
     * "Because the stateTableOffset in the state table header is (strictly
     * speaking) redundant, some 'kern' tables use it to record an initial
     * state where that should not be StartOfText. To determine if this is
     * done, calculate what the stateTableOffset should be. If it's different
     * from the actual stateTableOffset, use it as the initial state."
     *
     * We implement this by calling the initial state zero, but allow *negative*
     * states if the start state indeed was not the first state.  Since the code
     * is shared, this will also apply to 'mort' table.  The 'kerx' / 'morx'
     * tables are not affected since those address states by index, not offset.
     */

    పూర్ణాంకం min_state = 0;
    పూర్ణాంకం max_state = 0;
    గుర్తులేని పూర్ణాంకం అంకె_entries = 0;

    పూర్ణాంకం state_pos = 0;
    పూర్ణాంకం state_neg = 0;
    గుర్తులేని పూర్ణాంకం నమోదు_2 = 0;
    అయితే (min_state < state_neg || state_pos <= max_state)
    {
      ఒకవేళ (min_state < state_neg)
      {
	/* Negative states. */
	ఒకవేళ (unlikely (hb_unsigned_mul_overflows (min_state, అంకె_classes)))
	  return_trace (అబద్ధం);
	ఒకవేళ (unlikely (!c->check_range (&స్థితులు[min_state * అంకె_classes],
				       -min_state,
				       అడ్డువరుస_stride)))
	  return_trace (అబద్ధం);
	ఒకవేళ ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (అబద్ధం);
	{ /* Sweep new states. */
	  స్థిరరాశి HBUSHORT *stop = &స్థితులు[min_state * అంకె_classes];
	  ఒకవేళ (unlikely (stop > స్థితులు))
	    return_trace (అబద్ధం);
	  కొరకు (స్థిరరాశి HBUSHORT *p = స్థితులు; stop < p; p--)
	    అంకె_entries = hb_max (అంకె_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      ఒకవేళ (state_pos <= max_state)
      {
	/* Positive states. */
	ఒకవేళ (unlikely (!c->check_range (స్థితులు,
				       max_state + 1,
				       అడ్డువరుస_stride)))
	  return_trace (అబద్ధం);
	ఒకవేళ ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (అబద్ధం);
	{ /* Sweep new states. */
	  ఒకవేళ (unlikely (hb_unsigned_mul_overflows ((max_state + 1), అంకె_classes)))
	    return_trace (అబద్ధం);
	  స్థిరరాశి HBUSHORT *stop = &స్థితులు[(max_state + 1) * అంకె_classes];
	  ఒకవేళ (unlikely (stop < స్థితులు))
	    return_trace (అబద్ధం);
	  కొరకు (స్థిరరాశి HBUSHORT *p = &స్థితులు[state_pos * అంకె_classes]; p < stop; p++)
	    అంకె_entries = hb_max (అంకె_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      ఒకవేళ (unlikely (!c->check_array (entries, అంకె_entries)))
	return_trace (అబద్ధం);
      ఒకవేళ ((c->max_ops -= అంకె_entries - నమోదు_2) <= 0)
	return_trace (అబద్ధం);
      { /* Sweep new entries. */
	స్థిరరాశి నమోదు_3<Extra> *stop = &entries[అంకె_entries];
	కొరకు (స్థిరరాశి నమోదు_3<Extra> *p = &entries[నమోదు_2]; p < stop; p++)
	{
	  పూర్ణాంకం newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	నమోదు_2 = అంకె_entries;
      }
    }

    ఒకవేళ (అంకె_entries_బహిర్గతం)
      *అంకె_entries_బహిర్గతం = అంకె_entries;

    return_trace (నిజం);
  }

  రక్షిత:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffset_గమ్యం<ClassType, HBUINT>
		class_పట్టిక;	/* Offset to the class table. */
  NNOffset_గమ్యం<Unsized_Array_యొక్క<HBUSHORT>, HBUINT>
		state_Array_పట్టిక;/* Offset to the state array. */
  NNOffset_గమ్యం<Unsized_Array_యొక్క<నమోదు_3<Extra>>, HBUINT>
		నమోదు_పట్టిక;	/* Offset to the entry array. */

  బహిరంగ:
  DEFINE_కొలత_STATIC (4 * పరిమాణం (HBUINT));
};

మూస <రకం_పేరు HBUCHAR>
నిర్మాణం Class_పట్టిక
{
  గుర్తులేని పూర్ణాంకం get_class (hb_codepoint_t glyph_id, గుర్తులేని పూర్ణాంకం బహిర్గతం_యొక్క_Range) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం i = glyph_id - మొదటి_Glyph;
    తిరిగి i >= classArray.len ? బహిర్గతం_యొక్క_Range : classArray.arrayZ[i];
  }
  గుర్తులేని పూర్ణాంకం get_class (hb_codepoint_t glyph_id,
			  గుర్తులేని పూర్ణాంకం అంకె_glyphs HB_వాడని,
			  గుర్తులేని పూర్ణాంకం బహిర్గతం_యొక్క_Range) స్థిరరాశి
  {
    తిరిగి get_class (glyph_id, బహిర్గతం_యొక్క_Range);
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &glyphs, గుర్తులేని అంకె_glyphs) స్థిరరాశి
  {
    కొరకు (గుర్తులేని i = 0; i < classArray.len; i++)
      ఒకవేళ (classArray.arrayZ[i] != CLASS_బహిర్గతం_యొక్క_BOUNDS)
	glyphs.కూడు (మొదటి_Glyph + i);
  }
  మూస <రకం_పేరు సమితి_t, రకం_పేరు filter_t>
  శూన్య collect_glyphs_filtered (సమితి_t &glyphs, గుర్తులేని అంకె_glyphs, స్థిరరాశి filter_t &filter) స్థిరరాశి
  {
    కొరకు (గుర్తులేని i = 0; i < classArray.len; i++)
      ఒకవేళ (filter (classArray.arrayZ[i]))
	glyphs.కూడు (మొదటి_Glyph + i);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && classArray.sanitize (c));
  }
  రక్షిత:
  HBGlyphID16		మొదటి_Glyph;	/* First glyph index included in the trimmed array. */
  Array16_యొక్క<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, classArray);
};

నిర్మాణం SubtableGlyphCoverage
{
  తార్కిక sanitize (hb_sanitize_context_t *c, గుర్తులేని subtable_లెక్క) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);

    ఒకవేళ (unlikely (!c->check_array (&subtableOffsets, subtable_లెక్క)))
      return_trace (అబద్ధం);

    గుర్తులేని బైట్లు = (c->get_అంకె_glyphs () + CHAR_బిట్టు - 1) / CHAR_బిట్టు;
    కొరకు (గుర్తులేని i = 0; i < subtable_లెక్క; i++)
    {
      uint32_t స్థానభ్రంశం = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      ఒకవేళ (స్థానభ్రంశం == 0 || స్థానభ్రంశం == 0xFFFFFFFF)
        కొనసాగు;
      ఒకవేళ (unlikely (!subtableOffsets[i].sanitize (c, ఇది, బైట్లు)))
        return_trace (అబద్ధం);
    }

    return_trace (నిజం);
  }
  రక్షిత:
  Unsized_Array_యొక్క<NNOffset32_గమ్యం<Unsized_Array_యొక్క<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (0, subtableOffsets);
};

నిర్మాణం ObsoleteTypes
{
  స్థిర constexpr తార్కిక extended = అబద్ధం;
  రకనిర్వచనం HBUINT16 HBUINT;
  రకనిర్వచనం HBUINT8 HBUSHORT;
  రకనిర్వచనం Class_పట్టిక<HBUINT8> ClassTypeNarrow;
  రకనిర్వచనం Class_పట్టిక<HBUINT16> ClassTypeWide;

  మూస <రకం_పేరు T>
  స్థిర పూర్ణాంకం స్థానభ్రంశం_గమ్యం_సూచీ (int64_t స్థానభ్రంశం,
			    స్థిరరాశి శూన్య *పునాది,
			    స్థిరరాశి T *array)
  {
    int64_t array_స్థానభ్రంశం = (స్థిరరాశి అక్షరం *) array - (స్థిరరాశి అక్షరం *) పునాది;
    పూర్ణాంకం bad_సూచీ = గరిష్ఠ_పూర్ణాంకం / T::static_కొలత;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    ఒకవేళ (unlikely (స్థానభ్రంశం < array_స్థానభ్రంశం))
      తిరిగి bad_సూచీ;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t సూచీ_2 = (స్థానభ్రంశం - array_స్థానభ్రంశం) / T::static_కొలత;
    ఒకవేళ (unlikely (సూచీ_2 > bad_సూచీ))
      తిరిగి bad_సూచీ;
    తిరిగి సూచీ_2;
  }
  మూస <రకం_పేరు T>
  స్థిర పూర్ణాంకం byte_స్థానభ్రంశం_గమ్యం_సూచీ (int64_t స్థానభ్రంశం,
				స్థిరరాశి శూన్య *పునాది,
				స్థిరరాశి T *array)
  {
    తిరిగి స్థానభ్రంశం_గమ్యం_సూచీ (స్థానభ్రంశం, పునాది, array);
  }
  మూస <రకం_పేరు T>
  స్థిర పూర్ణాంకం మాట_స్థానభ్రంశం_గమ్యం_సూచీ (int64_t స్థానభ్రంశం,
				స్థిరరాశి శూన్య *పునాది,
				స్థిరరాశి T *array)
  {
    తిరిగి స్థానభ్రంశం_గమ్యం_సూచీ (2 * స్థానభ్రంశం, పునాది, array);
  }
};
నిర్మాణం ExtendedTypes
{
  స్థిర constexpr తార్కిక extended = నిజం;
  రకనిర్వచనం HBUINT32 HBUINT;
  రకనిర్వచనం HBUINT16 HBUSHORT;
  రకనిర్వచనం Lookup<HBUINT16> ClassTypeNarrow;
  రకనిర్వచనం Lookup<HBUINT16> ClassTypeWide;

  మూస <రకం_పేరు T>
  స్థిర గుర్తులేని పూర్ణాంకం స్థానభ్రంశం_గమ్యం_సూచీ (గుర్తులేని పూర్ణాంకం స్థానభ్రంశం,
				     స్థిరరాశి శూన్య *పునాది HB_వాడని,
				     స్థిరరాశి T *array HB_వాడని)
  {
    తిరిగి స్థానభ్రంశం;
  }
  మూస <రకం_పేరు T>
  స్థిర గుర్తులేని పూర్ణాంకం byte_స్థానభ్రంశం_గమ్యం_సూచీ (గుర్తులేని పూర్ణాంకం స్థానభ్రంశం,
					 స్థిరరాశి శూన్య *పునాది HB_వాడని,
					 స్థిరరాశి T *array HB_వాడని)
  {
    తిరిగి స్థానభ్రంశం / 2;
  }
  మూస <రకం_పేరు T>
  స్థిర గుర్తులేని పూర్ణాంకం మాట_స్థానభ్రంశం_గమ్యం_సూచీ (గుర్తులేని పూర్ణాంకం స్థానభ్రంశం,
					 స్థిరరాశి శూన్య *పునాది HB_వాడని,
					 స్థిరరాశి T *array HB_వాడని)
  {
    తిరిగి స్థానభ్రంశం;
  }
};

మూస <రకం_పేరు Types, రకం_పేరు నమోదు_దత్తాంశం, రకం_పేరు Flags>
నిర్మాణం State_పట్టిక_Driver
{
  ఉపయోగించి State_పట్టిక_T = State_పట్టిక<Types, నమోదు_దత్తాంశం>;
  ఉపయోగించి నమోదు_T = నమోదు_3<నమోదు_దత్తాంశం>;

  State_పట్టిక_Driver (స్థిరరాశి State_పట్టిక_T &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      అంకె_glyphs (face_->get_అంకె_glyphs ()) {}

  మూస <రకం_పేరు context_t>
  శూన్య drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_బఫరు_t *బఫరు = ac->బఫరు;

    ఒకవేళ (!c->in_place)
      బఫరు->clear_output ();

    పూర్ణాంకం state = State_పట్టిక_T::STATE_ప్రారంభం_యొక్క_పాఠ్యం;
    // If there's only one range, we already checked the flag.
    స్వయంచాలక *చివరి_range = ac->range_flags && (ac->range_flags->నిడివి > 1) ? &(*ac->range_flags)[0] : శూన్యసూచిక;
    స్థిరరాశి తార్కిక ప్రారంభం_state_సురక్షిత_గమ్యం_break_eot =
      !c->పట్టిక->is_actionable (machine.get_నమోదు (State_పట్టిక_T::STATE_ప్రారంభం_యొక్క_పాఠ్యం, CLASS_చివర_యొక్క_పాఠ్యం));
    కొరకు (బఫరు->idx = 0; బఫరు->successful;)
    {
      గుర్తులేని పూర్ణాంకం klass = likely (బఫరు->idx < బఫరు->len) ?
			   machine.get_class (బఫరు->cur().codepoint, అంకె_glyphs, ac->machine_class_cache) :
			   (గుర్తులేని) CLASS_చివర_యొక్క_పాఠ్యం;
    resume:
      DEBUG_సందేశం (APPLY, శూన్యసూచిక, "c%u at %u", klass, బఫరు->idx);
      స్థిరరాశి నమోదు_T &నమోదు_2 = machine.get_నమోదు (state, klass);
      స్థిరరాశి పూర్ణాంకం తదుపరి_state = machine.new_state (నమోదు_2.newState);

      తార్కిక is_not_epsilon_transition = !(నమోదు_2.flags & Flags::DontAdvance);
      తార్కిక is_not_actionable = !c->పట్టిక->is_actionable (నమోదు_2);

      ఒకవేళ (unlikely (చివరి_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	స్వయంచాలక *range = చివరి_range;
	ఒకవేళ (బఫరు->idx < బఫరు->len)
	{
	  గుర్తులేని cluster = బఫరు->cur().cluster;
	  అయితే (cluster < range->cluster_మొదటి)
	    range--;
	  అయితే (cluster > range->cluster_చివరి)
	    range++;


	  చివరి_range = range;
	}
	ఒకవేళ (!(range->flags & ac->subtable_flags))
	{
	  ఒకవేళ (బఫరు->idx == బఫరు->len)
	    ఆపు;

	  state = State_పట్టిక_T::STATE_ప్రారంభం_యొక్క_పాఠ్యం;
	  (శూన్య) బఫరు->తదుపరి_glyph ();
	  కొనసాగు;
	}
      }
      లేకపోతే
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	తార్కిక is_null_transition = state == State_పట్టిక_T::STATE_ప్రారంభం_యొక్క_పాఠ్యం &&
				  తదుపరి_state == State_పట్టిక_T::STATE_ప్రారంభం_యొక్క_పాఠ్యం &&
				  ప్రారంభం_state_సురక్షిత_గమ్యం_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	ఒకవేళ (is_null_transition)
	{
	  గుర్తులేని old_klass = klass;
	  చేయి
	  {
	    c->transition (బఫరు, ఇది, నమోదు_2);

	    ఒకవేళ (బఫరు->idx == బఫరు->len || !బఫరు->successful)
	      ఆపు;

	    (శూన్య) బఫరు->తదుపరి_glyph ();

	    klass = likely (బఫరు->idx < బఫరు->len) ?
		     machine.get_class (బఫరు->cur().codepoint, అంకె_glyphs, ac->machine_class_cache) :
		     (గుర్తులేని) CLASS_చివర_యొక్క_పాఠ్యం;
	  } అయితే (klass == old_klass);

	  ఒకవేళ (బఫరు->idx == బఫరు->len || !బఫరు->successful)
	    ఆపు;

	  వెళ్ళు resume;
	}
      }

      /* Conditions under which it's guaranteed safe-to-break before current glyph:
       *
       * 1. There was no action in this transition; and
       *
       * 2. If we break before current glyph, the results will be the same. That
       *    is guaranteed if:
       *
       *    2a. We were already in start-of-text state; or
       *
       *    2b. We are epsilon-transitioning to start-of-text state; or
       *
       *    2c. Starting from start-of-text state seeing current glyph:
       *
       *        2c'. There won't be any actions; and
       *
       *        2c". We would end up in the same state that we were going to end up
       *             in now, including whether epsilon-transitioning.
       *
       *    and
       *
       * 3. If we break before current glyph, there won't be any end-of-text action
       *    after previous glyph.
       *
       * This triples the transitions we need to look up, but is worth returning
       * granular unsafe-to-break results. See eg.:
       *
       *   https://github.com/harfbuzz/harfbuzz/issues/2860
       */
      స్థిరరాశి నమోదు_T *wouldbe_నమోదు;
      తార్కిక is_సురక్షిత_గమ్యం_break =
      (
          /* 1. */
          !c->పట్టిక->is_actionable (నమోదు_2) &&

          /* 2. */
          // This one is meh, I know...
	  (
                 state == State_పట్టిక_T::STATE_ప్రారంభం_యొక్క_పాఠ్యం
              || ((నమోదు_2.flags & Flags::DontAdvance) && తదుపరి_state == State_పట్టిక_T::STATE_ప్రారంభం_యొక్క_పాఠ్యం)
              || (
		    /* 2c. */
		    wouldbe_నమోదు = &machine.get_నమోదు(State_పట్టిక_T::STATE_ప్రారంభం_యొక్క_పాఠ్యం, klass)
		    ,
		    /* 2c'. */
		    !c->పట్టిక->is_actionable (*wouldbe_నమోదు) &&
		    /* 2c". */
		    (
		      తదుపరి_state == machine.new_state(wouldbe_నమోదు->newState) &&
		      (నమోదు_2.flags & Flags::DontAdvance) == (wouldbe_నమోదు->flags & Flags::DontAdvance)
		    )
		 )
	  ) &&

          /* 3. */
          !c->పట్టిక->is_actionable (machine.get_నమోదు (state, CLASS_చివర_యొక్క_పాఠ్యం))
      );

      ఒకవేళ (!is_సురక్షిత_గమ్యం_break && బఫరు->backtrack_len () && బఫరు->idx < బఫరు->len)
	బఫరు->unsafe_గమ్యం_break_from_outbuffer (బఫరు->backtrack_len () - 1, బఫరు->idx + 1);

      c->transition (బఫరు, ఇది, నమోదు_2);

      state = తదుపరి_state;
      DEBUG_సందేశం (APPLY, శూన్యసూచిక, "s%d", state);

      ఒకవేళ (బఫరు->idx == బఫరు->len)
	ఆపు;

      ఒకవేళ (is_not_epsilon_transition || బఫరు->max_ops-- <= 0)
	(శూన్య) బఫరు->తదుపరి_glyph ();
    }

    ఒకవేళ (!c->in_place)
      బఫరు->సమకాలీకరణ ();
  }

  బహిరంగ:
  స్థిరరాశి State_పట్టిక_T &machine;
  గుర్తులేని పూర్ణాంకం అంకె_glyphs;
};


} /* namespace AAT */


#అయితే_ముగింపు /* HB_AAT_LAYOUT_COMMON_HH */
