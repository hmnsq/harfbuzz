#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#నిర్వచించు OT_LAYOUT_GPOS_ANCHORMATRIX_HH

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

HB_అంతర్గత అంతఃవరుస
శూన్య collect_ligature_anchor_indexes (గుర్తులేని అడ్డువరుసలు,
                                     గుర్తులేని class_లెక్క,
                                     స్థిరరాశి hb_map_t &klass_mapping,
                                     hb_sorted_సదిశ_t<గుర్తులేని> &indexes)
{
  కొరకు (గుర్తులేని అడ్డువరుస = 0; అడ్డువరుస < అడ్డువరుసలు; అడ్డువరుస++)
    కొరకు (గుర్తులేని నిలువు = 0; నిలువు < class_లెక్క; నిలువు++)
      ఒకవేళ (klass_mapping.has (నిలువు))
        indexes.push (అడ్డువరుస * class_లెక్క + నిలువు);
}

HB_అంతర్గత అంతఃవరుస
శూన్య collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      స్థిరరాశి శూన్య *పునాది,
                                      glyph_దత్తాంశం_t offsets,
                                      hb_array_t<స్థిరరాశి గుర్తులేని> indexes)
{
  కొరకు (గుర్తులేని i : indexes)
  {
    గుర్తులేని స్థానభ్రంశం = offsets[i];
    ఒకవేళ (స్థానభ్రంశం)
      Struct_At_స్థానభ్రంశం<స్థిరరాశి Anchor> (పునాది, స్థానభ్రంశం).collect_variation_indices (c);
  }
}

HB_అంతర్గత అంతఃవరుస
శూన్య collect_anchor_indexes (స్థిరరాశి Coverage &coverage,
                             గుర్తులేని అడ్డువరుసలు,
                             గుర్తులేని class_లెక్క,
                             స్థిరరాశి hb_సమితి_t &glyphset,
                             స్థిరరాశి hb_map_t &klass_mapping,
                             hb_sorted_సదిశ_t<గుర్తులేని> &indexes)
{
  కొరకు (స్థిరరాశి స్వయంచాలక _ : hb_zip (coverage, hb_range (అడ్డువరుసలు)))
    ఒకవేళ (glyphset.has (_.మొదటి))
      కొరకు (గుర్తులేని నిలువు = 0; నిలువు < class_లెక్క; నిలువు++)
        ఒకవేళ (klass_mapping.has (నిలువు))
          indexes.push (_.second * class_లెక్క + నిలువు);
}

HB_అంతర్గత అంతఃవరుస
శూన్య collect_subset_anchor_indexes (స్థిరరాశి Coverage &coverage,
                                    గుర్తులేని అడ్డువరుసలు,
                                    గుర్తులేని class_లెక్క,
                                    glyph_దత్తాంశం_t offsets,
                                    స్థిరరాశి hb_సమితి_t &glyphset,
                                    స్థిరరాశి hb_map_t &glyph_map,
                                    స్థిరరాశి hb_map_t &klass_mapping,
                                    hb_sorted_సదిశ_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_సదిశ_t<గుర్తులేని> &indexes)
{
  కొరకు (స్థిరరాశి స్వయంచాలక _ : hb_zip (coverage, hb_range (అడ్డువరుసలు)))
  {
    ఒకవేళ (!glyphset.has (_.మొదటి)) కొనసాగు;
    గుర్తులేని అడ్డువరుస = _.second;
    తార్కిక non_ఖాళీలు = అబద్ధం;
    కొరకు (గుర్తులేని నిలువు = 0; నిలువు < class_లెక్క; నిలువు++)
      ఒకవేళ (klass_mapping.has (నిలువు) && offsets[అడ్డువరుస * class_లెక్క + నిలువు])
      {
        non_ఖాళీలు = నిజం;
        ఆపు;
      }
    ఒకవేళ (!non_ఖాళీలు) కొనసాగు;

    new_coverage.push (glyph_map.get (_.మొదటి));
    కొరకు (గుర్తులేని నిలువు = 0; నిలువు < class_లెక్క; నిలువు++)
      ఒకవేళ (klass_mapping.has (నిలువు))
        indexes.push (అడ్డువరుస * class_లెక్క + నిలువు);
  }
}

మూస <రకం_పేరు Types, రకం_పేరు అడ్డువరుస_లెక్క=రకం_పేరు Types::HBUINT>
నిర్మాణం Anchor_మాతృక
{
  అడ్డువరుస_లెక్క      అడ్డువరుసలు;                   /* Number of rows */
  Unsized_Array_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Anchor, Anchor_మాతృక>>
                మాతృక_Z;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (పరిమాణం (అడ్డువరుస_లెక్క), మాతృక_Z);

  తార్కిక sanitize (hb_sanitize_context_t *c, గుర్తులేని పూర్ణాంకం నిలువుల_సంఖ్య) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!c->check_struct (ఇది)) return_trace (అబద్ధం);
    hb_barrier ();
    ఒకవేళ (unlikely (hb_unsigned_mul_overflows (అడ్డువరుసలు, నిలువుల_సంఖ్య))) return_trace (అబద్ధం);
    గుర్తులేని పూర్ణాంకం లెక్క = అడ్డువరుసలు * నిలువుల_సంఖ్య;
    ఒకవేళ (!c->check_array (మాతృక_Z.arrayZ, లెక్క)) return_trace (అబద్ధం);

    ఒకవేళ (c->lazy_some_gpos)
      return_trace (నిజం);

    hb_barrier ();
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (!మాతృక_Z[i].sanitize (c, ఇది)) return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  స్థిరరాశి Anchor& get_anchor (hb_ot_apply_context_t *c,
			    గుర్తులేని పూర్ణాంకం అడ్డువరుస, గుర్తులేని పూర్ణాంకం నిలువు,
			    గుర్తులేని పూర్ణాంకం నిలువుల_సంఖ్య, తార్కిక *దొరికింది) స్థిరరాశి
  {
    *దొరికింది = అబద్ధం;
    ఒకవేళ (unlikely (అడ్డువరుస >= అడ్డువరుసలు || నిలువు >= నిలువుల_సంఖ్య)) తిరిగి Null (Anchor);
    స్వయంచాలక &స్థానభ్రంశం = మాతృక_Z[అడ్డువరుస * నిలువుల_సంఖ్య + నిలువు];
    ఒకవేళ (unlikely (!స్థానభ్రంశం.sanitize (&c->sanitizer, ఇది))) తిరిగి Null (Anchor);
    hb_barrier ();
    *దొరికింది = !స్థానభ్రంశం.is_null ();
    తిరిగి ఇది+స్థానభ్రంశం;
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<స్థిరరాశి గుర్తులేని> indexes) స్థిరరాశి
  {
    collect_anchor_variation_indices (c, ఇది, మాతృక_Z.arrayZ, indexes);
  }

  మూస <రకం_పేరు Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  తార్కిక subset (hb_subset_context_t *c,
               గుర్తులేని             అంకె_అడ్డువరుసలు,
               Iterator             సూచీ_iter) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (ఇది);

    ఒకవేళ (!సూచీ_iter) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం)))  return_trace (అబద్ధం);

    బహిర్గతం->అడ్డువరుసలు = అంకె_అడ్డువరుసలు;
    కొరకు (స్థిరరాశి గుర్తులేని i : సూచీ_iter)
    {
      స్వయంచాలక *స్థానభ్రంశం = c->serializer->embed (మాతృక_Z[i]);
      ఒకవేళ (!స్థానభ్రంశం) return_trace (అబద్ధం);
      స్థానభ్రంశం->serialize_subset (c, మాతృక_Z[i], ఇది);
    }

    return_trace (నిజం);
  }

  తార్కిక స్థానభ్రంశం_is_null (గుర్తులేని అడ్డువరుస, గుర్తులేని నిలువు, గుర్తులేని అంకె_నిలువుల_సంఖ్య) స్థిరరాశి
  {
    ఒకవేళ (unlikely (అడ్డువరుస >= అడ్డువరుసలు || నిలువు >= అంకె_నిలువుల_సంఖ్య)) తిరిగి నిజం;
    స్వయంచాలక &స్థానభ్రంశం = మాతృక_Z[అడ్డువరుస * అంకె_నిలువుల_సంఖ్య + నిలువు];
    తిరిగి స్థానభ్రంశం.is_null ();
  }
};


}
}
}

#అయితే_ముగింపు /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
