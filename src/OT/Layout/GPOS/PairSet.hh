#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_PAIRSET_HH
#నిర్వచించు OT_LAYOUT_GPOS_PAIRSET_HH

#అన్తర్భావ "PairValueRecord.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
నిర్మాణం pair_సమితి_విలువలు_t
{
  స్థిరరాశి విలువ_పునాది *పునాది;
  glyph_దత్తాంశం_t glyphs;
  స్థిరరాశి విలువ *values;
  గుర్తులేని లెక్క;
  గుర్తులేని రికార్డు_కొలత;

  గుర్తులేని get_glyph (గుర్తులేని i) స్థిరరాశి
  {
    glyph_దత్తాంశం_t glyph = glyphs;
    glyph.దత్తాంశం = (స్థిరరాశి అక్షరం *) glyph.దత్తాంశం + i * రికార్డు_కొలత;
    తిరిగి glyph[0];
  }

  స్థిరరాశి విలువ *get_విలువలు (గుర్తులేని i) స్థిరరాశి
  { తిరిగి reinterpret_cast<స్థిరరాశి విలువ *> ((స్థిరరాశి అక్షరం *) values + i * రికార్డు_కొలత); }

  hb_pair_t<గుర్తులేని, గుర్తులేని>
  compute_effective_విలువ_formats (స్థిరరాశి hb_సమితి_t &glyphset,
                                   స్థిరరాశి విలువ_Format విలువ_Formats[2],
                                   తార్కిక strip_hints, తార్కిక strip_ఖాళీలు,
                                   స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *varidx_delta_map) స్థిరరాశి
  {
    గుర్తులేని len1 = విలువ_Formats[0].get_len ();
    గుర్తులేని format1 = 0, format2 = 0;
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (!glyphset.has (get_glyph (i))) కొనసాగు;
      స్థిరరాశి విలువ *v = get_విలువలు (i);
      format1 |= విలువ_Formats[0].get_effective_format (v, strip_hints, strip_ఖాళీలు, పునాది, varidx_delta_map);
      format2 |= విలువ_Formats[1].get_effective_format (v + len1, strip_hints, strip_ఖాళీలు, పునాది, varidx_delta_map);
    }
    తిరిగి hb_pair (format1, format2);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  స్థిరరాశి విలువ_Format విలువ_Formats[2]) స్థిరరాశి
  {
    గుర్తులేని len1 = విలువ_Formats[0].get_len ();
    గుర్తులేని len2 = విలువ_Formats[1].get_len ();
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (!c->glyph_సమితి->has (get_glyph (i))) కొనసాగు;
      స్థిరరాశి hb_array_t<స్థిరరాశి విలువ> v = hb_array (get_విలువలు (i), len1 + len2);
      ఒకవేళ (విలువ_Formats[0].has_device ())
        విలువ_Formats[0].collect_variation_indices (c, పునాది, v.sub_array (0, len1));
      ఒకవేళ (విలువ_Formats[1].has_device ())
        విలువ_Formats[1].collect_variation_indices (c, పునాది, v.sub_array (len1, len2));
    }
  }
};

మూస <రకం_పేరు Types>
నిర్మాణం Pair_సమితి : విలువ_పునాది
{
  మూస <రకం_పేరు Types2>
  మిత్ర నిర్మాణం PairPosFormat1_3;

  ఉపయోగించి Pair_విలువ_రికార్డు = GPOS_impl::Pair_విలువ_రికార్డు<Types>;

  రక్షిత:
  రకం_పేరు Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_విలువ_రికార్డు       మొదటి_Pair_విలువ_రికార్డు;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  బహిరంగ:
  DEFINE_కొలత_MIN (Types::HBUINT::static_కొలత);

  స్థిర size_t get_కొలత (గుర్తులేని len1, గుర్తులేని len2)
  {
    తిరిగి Types::HBGlyphID::static_కొలత + విలువ::static_కొలత * (len1 + len2);
  }
  స్థిర size_t get_కొలత (స్థిరరాశి విలువ_Format విలువ_Formats[2])
  {
    గుర్తులేని len1 = విలువ_Formats[0].get_len ();
    గుర్తులేని len2 = విలువ_Formats[1].get_len ();
    తిరిగి get_కొలత (len1, len2);
  }

  నిర్మాణం sanitize_సంవృతం_t
  {
    స్థిరరాశి విలువ_Format *విలువ_Formats;
    గుర్తులేని పూర్ణాంకం len1; /* valueFormats[0].get_len() */
    size_t stride; /*బైట్లు*/
  };

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి sanitize_సంవృతం_t *సంవృతం) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!(c->check_struct (ఇది) &&
	  hb_barrier () &&
          c->check_range (&మొదటి_Pair_విలువ_రికార్డు,
                          len,
                          సంవృతం->stride))) return_trace (అబద్ధం);
    hb_barrier ();

    గుర్తులేని పూర్ణాంకం లెక్క = len;
    స్థిరరాశి Pair_విలువ_రికార్డు *రికార్డు = &మొదటి_Pair_విలువ_రికార్డు;
    return_trace (c->lazy_some_gpos ||
		  (సంవృతం->విలువ_Formats[0].sanitize_విలువలు_stride_unsafe (c, ఇది, &రికార్డు->values[0], లెక్క, సంవృతం->stride) &&
                   సంవృతం->విలువ_Formats[1].sanitize_విలువలు_stride_unsafe (c, ఇది, &రికార్డు->values[సంవృతం->len1], లెక్క, సంవృతం->stride)));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs,
                   స్థిరరాశి విలువ_Format *విలువ_Formats) స్థిరరాశి
  {
    గుర్తులేని రికార్డు_కొలత = get_కొలత (విలువ_Formats);

    స్థిరరాశి Pair_విలువ_రికార్డు *రికార్డు = &మొదటి_Pair_విలువ_రికార్డు;
    గుర్తులేని పూర్ణాంకం లెక్క = len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (glyphs->has (రికార్డు->రెండవ_Glyph))
        తిరిగి నిజం;
      రికార్డు = &Struct_At_స్థానభ్రంశం<స్థిరరాశి Pair_విలువ_రికార్డు> (రికార్డు, రికార్డు_కొలత);
    }
    తిరిగి అబద్ధం;
  }

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c,
                       స్థిరరాశి విలువ_Format *విలువ_Formats) స్థిరరాశి
  {
    గుర్తులేని రికార్డు_కొలత = get_కొలత (విలువ_Formats);

    స్థిరరాశి Pair_విలువ_రికార్డు *రికార్డు = &మొదటి_Pair_విలువ_రికార్డు;
    c->ఆదానం->కూడు_array (&రికార్డు->రెండవ_Glyph, len, రికార్డు_కొలత);
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_రెండవ_glyphs (సమితి_t *glyphs,
			      స్థిరరాశి విలువ_Format *విలువ_Formats) స్థిరరాశి
  {
    గుర్తులేని రికార్డు_కొలత = get_కొలత (విలువ_Formats);
    స్థిరరాశి Pair_విలువ_రికార్డు *రికార్డు = &మొదటి_Pair_విలువ_రికార్డు;
    glyphs->కూడు_array (&రికార్డు->రెండవ_Glyph, len, రికార్డు_కొలత);
  }

  pair_సమితి_విలువలు_t get_విలువలు_view (స్థిరరాశి విలువ_Format విలువ_Formats[2]) స్థిరరాశి
  {
    తిరిగి {ఇది, &మొదటి_Pair_విలువ_రికార్డు.రెండవ_Glyph, &మొదటి_Pair_విలువ_రికార్డు.values[0],
            (గుర్తులేని) len, (గుర్తులేని) get_కొలత (విలువ_Formats)};
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  స్థిరరాశి విలువ_Format *విలువ_Formats) స్థిరరాశి
  {
    get_విలువలు_view (విలువ_Formats).collect_variation_indices (c, విలువ_Formats);
  }

  తార్కిక apply (hb_ot_apply_context_t *c,
              స్థిరరాశి విలువ_Format *విలువ_Formats,
              గుర్తులేని పూర్ణాంకం pos) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    hb_బఫరు_t *బఫరు = c->బఫరు;
    గుర్తులేని పూర్ణాంకం len1 = విలువ_Formats[0].get_len ();
    గుర్తులేని పూర్ణాంకం len2 = విలువ_Formats[1].get_len ();
    గుర్తులేని రికార్డు_కొలత = get_కొలత (len1, len2);

    స్థిరరాశి Pair_విలువ_రికార్డు *రికార్డు = hb_bsearch (బఫరు->info[pos].codepoint,
                                                &మొదటి_Pair_విలువ_రికార్డు,
                                                (గుర్తులేని) len,
                                                రికార్డు_కొలత);
    ఒకవేళ (రికార్డు)
    {
      ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
      {
	c->బఫరు->సందేశం (c->font,
			    "try kerning glyphs at %u,%u",
			    c->బఫరు->idx, pos);
      }

      తార్కిక applied_మొదటి = len1 && విలువ_Formats[0].apply_విలువ (c, ఇది, &రికార్డు->values[0], బఫరు->cur_pos());
      తార్కిక applied_రెండవ = len2 && విలువ_Formats[1].apply_విలువ (c, ఇది, &రికార్డు->values[len1], బఫరు->pos[pos]);

      ఒకవేళ (applied_మొదటి || applied_రెండవ)
	ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
	{
	  c->బఫరు->సందేశం (c->font,
			      "kerned glyphs at %u,%u",
			      c->బఫరు->idx, pos);
	}

      ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
      {
	c->బఫరు->సందేశం (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->బఫరు->idx, pos);
      }

      ఒకవేళ (applied_మొదటి || applied_రెండవ)
        బఫరు->unsafe_గమ్యం_break (బఫరు->idx, pos + 1);
      లేకపోతే
        /* Even a zero-valued pair record is a concat hazard. */
        బఫరు->unsafe_గమ్యం_concat (బఫరు->idx, pos + 1);

      ఒకవేళ (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      బఫరు->unsafe_గమ్యం_break (బఫరు->idx, pos + 1);
      }

      బఫరు->idx = pos;
      return_trace (నిజం);
    }
    బఫరు->unsafe_గమ్యం_concat (బఫరు->idx, pos + 1);
    return_trace (అబద్ధం);
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి విలువ_Format విలువ_Formats[2],
               స్థిరరాశి విలువ_Format newFormats[2]) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక snap = c->serializer->snapshot ();

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
    బహిర్గతం->len = 0;

    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    గుర్తులేని len1 = విలువ_Formats[0].get_len ();
    గుర్తులేని len2 = విలువ_Formats[1].get_len ();
    గుర్తులేని రికార్డు_కొలత = get_కొలత (len1, len2);

    pair_విలువ_రికార్డు_context_t context =
    {
      ఇది,
      విలువ_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    స్థిరరాశి Pair_విలువ_రికార్డు *రికార్డు = &మొదటి_Pair_విలువ_రికార్డు;
    గుర్తులేని లెక్క = len, అంకె = 0;
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (glyphset.has (రికార్డు->రెండవ_Glyph)
         && రికార్డు->subset (c, &context)) అంకె++;
      రికార్డు = &Struct_At_స్థానభ్రంశం<స్థిరరాశి Pair_విలువ_రికార్డు> (రికార్డు, రికార్డు_కొలత);
    }

    బహిర్గతం->len = అంకె;
    ఒకవేళ (!అంకె) c->serializer->revert (snap);
    return_trace (అంకె);
  }
};


}
}
}

#అయితే_ముగింపు  // OT_LAYOUT_GPOS_PAIRSET_HH
