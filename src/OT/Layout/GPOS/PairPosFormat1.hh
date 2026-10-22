#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#నిర్వచించు OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#అన్తర్భావ "PairSet.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {


మూస <రకం_పేరు Types>
నిర్మాణం PairPosFormat1_3
{
  ఉపయోగించి Pair_సమితి = GPOS_impl::Pair_సమితి<Types>;
  ఉపయోగించి Pair_విలువ_రికార్డు = GPOS_impl::Pair_విలువ_రికార్డు<Types>;

  రక్షిత:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  విలువ_Format   విలువ_Format_2[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  రకం_పేరు Types::మూస Array_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Pair_సమితి>>
                pair_సమితి;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2 + Types::LOffset::static_కొలత + 2 * విలువ_Format::static_కొలత + Types::HBUINT::static_కొలత, pair_సమితి);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);

    ఒకవేళ (!c->check_struct (ఇది)) return_trace (అబద్ధం);
    hb_barrier ();

    గుర్తులేని పూర్ణాంకం len1 = విలువ_Format_2[0].get_len ();
    గుర్తులేని పూర్ణాంకం len2 = విలువ_Format_2[1].get_len ();
    రకం_పేరు Pair_సమితి::sanitize_సంవృతం_t సంవృతం =
    {
      విలువ_Format_2,
      len1,
      Pair_సమితి::get_కొలత (len1, len2)
    };

    return_trace (coverage.sanitize (c, ఇది) && pair_సమితి.sanitize (c, ఇది, &సంవృతం));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    స్వయంచాలక &cov = ఇది+coverage;

    ఒకవేళ (pair_సమితి.len > glyphs->get_population () * hb_బిట్టు_storage ((గుర్తులేని) pair_సమితి.len))
    {
      కొరకు (hb_codepoint_t g : glyphs->iter ())
      {
	గుర్తులేని i = cov.get_coverage (g);
	ఒకవేళ ((ఇది+pair_సమితి[i]).intersects (glyphs, విలువ_Format_2))
	  తిరిగి నిజం;
      }
      తిరిగి అబద్ధం;
    }

    తిరిగి
    + hb_zip (cov, pair_సమితి)
    | hb_filter (*glyphs, hb_మొదటి)
    | hb_map (hb_రెండవ)
    | hb_map ([glyphs, ఇది] (స్థిరరాశి రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Pair_సమితి> &_)
              { తిరిగి (ఇది+_).intersects (glyphs, విలువ_Format_2); })
    | hb_any
    ;
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి {}
  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    ఒకవేళ ((!విలువ_Format_2[0].has_device ()) && (!విలువ_Format_2[1].has_device ())) తిరిగి;

    స్వయంచాలక it =
    + hb_zip (ఇది+coverage, pair_సమితి)
    | hb_filter (c->glyph_సమితి, hb_మొదటి)
    | hb_map (hb_రెండవ)
    ;

    ఒకవేళ (!it) తిరిగి;
    + it
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([&] (స్థిరరాశి Pair_సమితి& _) { _.collect_variation_indices (c, విలువ_Format_2); })
    ;
  }

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!(ఇది+coverage).collect_coverage (c->ఆదానం))) తిరిగి;
    గుర్తులేని పూర్ణాంకం లెక్క = pair_సమితి.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      (ఇది+pair_సమితి[i]).collect_glyphs (c, విలువ_Format_2);
  }

  శూన్య collect_రెండవ_glyphs (hb_సమితి_digest_t *digest) స్థిరరాశి
  {
    గుర్తులేని లెక్క = pair_సమితి.len;
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      (ఇది+pair_సమితి[i]).collect_రెండవ_glyphs (digest, విలువ_Format_2);
  }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి { తిరిగి ఇది+coverage; }

  నిర్మాణం external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_సమితి_digest_t pair_sets[HB_VAR_ARRAY];
  };
  శూన్య *external_cache_create () స్థిరరాశి
  {
    గుర్తులేని లెక్క = pair_సమితి.len;
    గుర్తులేని కొలత = పరిమాణం (external_cache_t) -
		    HB_VAR_ARRAY * పరిమాణం (hb_సమితి_digest_t) +
		    లెక్క * పరిమాణం (hb_సమితి_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (కొలత);
    ఒకవేళ (likely (cache))
    {
      cache->coverage.clear ();
      కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      {
	cache->pair_sets[i].init ();
	(ఇది+pair_సమితి[i]).collect_రెండవ_glyphs (&cache->pair_sets[i], విలువ_Format_2);
      }
    }
    తిరిగి cache;
  }

  తార్కిక apply (hb_ot_apply_context_t *c, శూన్య *external_cache) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    hb_బఫరు_t *బఫరు = c->బఫరు;

#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage  (బఫరు->cur().codepoint, cache ? &cache->coverage : శూన్యసూచిక);
#లేకపోతే
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage  (బఫరు->cur().codepoint);
#అయితే_ముగింపు
    ఒకవేళ (సూచీ_2 == NOT_COVERED) return_trace (అబద్ధం);

    స్వయంచాలక &skippy_iter = c->iter_ఆదానం;
    skippy_iter.reset_fast (బఫరు->idx);
    గుర్తులేని unsafe_గమ్యం;
    ఒకవేళ (unlikely (!skippy_iter.next (&unsafe_గమ్యం)))
    {
      బఫరు->unsafe_గమ్యం_concat (బఫరు->idx, unsafe_గమ్యం);
      return_trace (అబద్ధం);
    }

#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ఒకవేళ (cache &&
	సూచీ_2 < pair_సమితి.len &&
	!cache->pair_sets[సూచీ_2].may_have (బఫరు->info[skippy_iter.idx].codepoint))
    {
      బఫరు->unsafe_గమ్యం_concat (బఫరు->idx, skippy_iter.idx + 1);
      return_trace (అబద్ధం);
    }
#అయితే_ముగింపు

    return_trace ((ఇది+pair_సమితి[సూచీ_2]).apply (c, విలువ_Format_2, skippy_iter.idx));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);

    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
    బహిర్గతం->format = format;

    hb_pair_t<గుర్తులేని, గుర్తులేని> newFormats = hb_pair (విలువ_Format_2[0], విలువ_Format_2[1]);

    ఒకవేళ (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_విలువ_formats (glyphset, అబద్ధం, అబద్ధం, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    లేకపోతే ఒకవేళ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_పట్టిక (c->plan->ఆధారం, HB_ట్యాగ్ ('f','v','a','r'));
      తార్కిక has_fvar = (blob != hb_blob_get_ఖాళీలు ());
      hb_blob_destroy (blob);

      తార్కిక strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      ఒకవేళ (has_fvar && !c->plan->has_gdef_varstore)
        strip = నిజం;
      newFormats = compute_effective_విలువ_formats (glyphset, strip, నిజం);
    }

    బహిర్గతం->విలువ_Format_2[0] = newFormats.మొదటి;
    బహిర్గతం->విలువ_Format_2[1] = newFormats.second;

    hb_sorted_సదిశ_t<hb_codepoint_t> new_coverage;

    + hb_zip (ఇది+coverage, pair_సమితి)
    | hb_filter (glyphset, hb_మొదటి)
    | hb_filter ([ఇది, c, బహిర్గతం] (స్థిరరాశి రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Pair_సమితి>& _)
                 {
                   స్వయంచాలక snap = c->serializer->snapshot ();
                   స్వయంచాలక *o = బహిర్గతం->pair_సమితి.serialize_append (c->serializer);
                   ఒకవేళ (unlikely (!o)) తిరిగి అబద్ధం;
                   తార్కిక ret = o->serialize_subset (c, _, ఇది, విలువ_Format_2, బహిర్గతం->విలువ_Format_2);
                   ఒకవేళ (!ret)
                   {
                     బహిర్గతం->pair_సమితి.తీయి ();
                     c->serializer->revert (snap);
                   }
                   తిరిగి ret;
                 },
                 hb_రెండవ)
    | hb_map (hb_మొదటి)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    బహిర్గతం->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (తార్కిక (new_coverage));
  }


  hb_pair_t<గుర్తులేని, గుర్తులేని> compute_effective_విలువ_formats (స్థిరరాశి hb_సమితి_t& glyphset,
                                                                 తార్కిక strip_hints, తార్కిక strip_ఖాళీలు,
                                                                 స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *varidx_delta_map = శూన్యసూచిక) స్థిరరాశి
  {
    గుర్తులేని format1 = 0;
    గుర్తులేని format2 = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక & _ :
	  + hb_zip (ఇది+coverage, pair_సమితి)
	  | hb_filter (glyphset, hb_మొదటి)
	  | hb_map (hb_రెండవ)
	)
    {
      స్థిరరాశి Pair_సమితి& సమితి = (ఇది + _);
      స్వయంచాలక formats = సమితి.get_విలువలు_view (విలువ_Format_2).compute_effective_విలువ_formats (
          glyphset, విలువ_Format_2, strip_hints, strip_ఖాళీలు, varidx_delta_map);
      format1 |= formats.మొదటి;
      format2 |= formats.second;

      ఒకవేళ (!varidx_delta_map &&
          format1 == విలువ_Format_2[0] && format2 == విలువ_Format_2[1])
        ఆపు;
    }

    తిరిగి hb_pair (format1, format2);
  }
};


}
}
}

#అయితే_ముగింపు  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
