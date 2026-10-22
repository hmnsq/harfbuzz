#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#నిర్వచించు OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#అన్తర్భావ "ValueFormat.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

HB_అంతర్గత అంతఃవరుస
శూన్య collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     స్థిరరాశి విలువ_పునాది *పునాది,
                                     స్థిరరాశి Coverage &coverage,
                                     స్థిరరాశి ClassDef &class_def1,
                                     స్థిరరాశి ClassDef &class_def2,
                                     స్థిరరాశి విలువ_Format &విలువ_Format1,
                                     స్థిరరాశి విలువ_Format &విలువ_Format2,
                                     గుర్తులేని class1_లెక్క,
                                     గుర్తులేని class2_లెక్క,
                                     స్థిరరాశి విలువ_రికార్డు &values)
{
  ఒకవేళ (!coverage.intersects (c->glyph_సమితి) ||
      !class_def2.intersects (c->glyph_సమితి)) తిరిగి;
  ఒకవేళ ((!విలువ_Format1.has_device ()) && (!విలువ_Format2.has_device ())) తిరిగి;

  hb_సమితి_t klass1_glyphs, klass2_glyphs;
  ఒకవేళ (!class_def1.collect_coverage (&klass1_glyphs)) తిరిగి;
  ఒకవేళ (!class_def2.collect_coverage (&klass2_glyphs)) తిరిగి;

  hb_సమితి_t class1_సమితి, class2_సమితి;
  కొరకు (స్థిరరాశి గుర్తులేని cp : + c->glyph_సమితి->iter () | hb_filter (coverage))
  {
    ఒకవేళ (!klass1_glyphs.has (cp)) class1_సమితి.కూడు (0);
    లేకపోతే
    {
      గుర్తులేని klass1 = class_def1.get (cp);
      class1_సమితి.కూడు (klass1);
    }
  }

  class2_సమితి.కూడు (0);
  కొరకు (స్థిరరాశి గుర్తులేని cp : + c->glyph_సమితి->iter () | hb_filter (klass2_glyphs))
  {
    గుర్తులేని klass2 = class_def2.get (cp);
    class2_సమితి.కూడు (klass2);
  }

  ఒకవేళ (class1_సమితి.is_empty ()
      || class2_సమితి.is_empty ()
      || (class2_సమితి.get_population() == 1 && class2_సమితి.has(0)))
    తిరిగి;

  గుర్తులేని len1 = విలువ_Format1.get_len ();
  గుర్తులేని len2 = విలువ_Format2.get_len ();
  స్థిరరాశి hb_array_t<స్థిరరాశి విలువ> విలువలు_array = values.as_array ((గుర్తులేని)class1_లెక్క * (గుర్తులేని) class2_లెక్క * (len1 + len2));
  కొరకు (స్థిరరాశి గుర్తులేని class1_idx : class1_సమితి.iter ())
  {
    కొరకు (స్థిరరాశి గుర్తులేని class2_idx : class2_సమితి.iter ())
    {
      గుర్తులేని ప్రారంభం_స్థానభ్రంశం = (class1_idx * (గుర్తులేని) class2_లెక్క + class2_idx) * (len1 + len2);
      ఒకవేళ (విలువ_Format1.has_device ())
        విలువ_Format1.collect_variation_indices (c, పునాది, విలువలు_array.sub_array (ప్రారంభం_స్థానభ్రంశం, len1));

      ఒకవేళ (విలువ_Format2.has_device ())
        విలువ_Format2.collect_variation_indices (c, పునాది, విలువలు_array.sub_array (ప్రారంభం_స్థానభ్రంశం+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
నిర్మాణం pair_pos_class_విలువలు_t
{
  స్థిరరాశి విలువ_పునాది *పునాది;
  స్థిరరాశి విలువ_రికార్డు &values;
  స్థిరరాశి విలువ_Format &విలువ_Format1;
  స్థిరరాశి విలువ_Format &విలువ_Format2;
  గుర్తులేని class1_లెక్క;
  గుర్తులేని class2_లెక్క;

  hb_pair_t<గుర్తులేని, గుర్తులేని> compute_effective_విలువ_formats (స్థిరరాశి hb_map_t &klass1_map,
                                                               స్థిరరాశి hb_map_t &klass2_map,
                                                               తార్కిక strip_hints, తార్కిక strip_ఖాళీలు,
                                                               స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *varidx_delta_map = శూన్యసూచిక) స్థిరరాశి
  {
    గుర్తులేని len1 = విలువ_Format1.get_len ();
    గుర్తులేని len2 = విలువ_Format2.get_len ();
    గుర్తులేని రికార్డు_కొలత = len1 + len2;

    గుర్తులేని format1 = 0;
    గుర్తులేని format2 = 0;

    కొరకు (గుర్తులేని class1_idx : + hb_range (class1_లెక్క) | hb_filter (klass1_map))
    {
      కొరకు (గుర్తులేని class2_idx : + hb_range (class2_లెక్క) | hb_filter (klass2_map))
      {
        గుర్తులేని idx = (class1_idx * class2_లెక్క + class2_idx) * రికార్డు_కొలత;
        format1 = format1 | విలువ_Format1.get_effective_format (&values[idx], strip_hints, strip_ఖాళీలు, పునాది, varidx_delta_map);
        format2 = format2 | విలువ_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_ఖాళీలు, పునాది, varidx_delta_map);
      }

      ఒకవేళ (!varidx_delta_map &&
          format1 == విలువ_Format1 && format2 == విలువ_Format2)
        ఆపు;
    }

    తిరిగి hb_pair (format1, format2);
  }

  తార్కిక serialize (hb_subset_context_t *c,
                  స్థిరరాశి hb_map_t &klass1_map,
                  స్థిరరాశి hb_map_t &klass2_map,
                  విలువ_Format newFormat1,
                  విలువ_Format newFormat2) స్థిరరాశి
  {
    గుర్తులేని len1 = విలువ_Format1.get_len ();
    గుర్తులేని len2 = విలువ_Format2.get_len ();
    గుర్తులేని మొత్తం_len = len1 + len2;
    hb_సదిశ_t<గుర్తులేని> class2_idxs (+ hb_range (class2_లెక్క) | hb_filter (klass2_map));
    ఒకవేళ (unlikely (!c->serializer->propagate_error (class2_idxs)))
      తిరిగి అబద్ధం;
    కొరకు (గుర్తులేని class1_idx : + hb_range (class1_లెక్క) | hb_filter (klass1_map))
    {
      కొరకు (గుర్తులేని class2_idx : class2_idxs)
      {
        గుర్తులేని idx = (class1_idx * class2_లెక్క + class2_idx) * మొత్తం_len;
        విలువ_Format1.copy_విలువలు (c->serializer, newFormat1, పునాది, &values[idx], &c->plan->layout_variation_idx_delta_map);
        విలువ_Format2.copy_విలువలు (c->serializer, newFormat2, పునాది, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    తిరిగి నిజం;
  }
};

మూస <రకం_పేరు Types>
నిర్మాణం PairPosFormat2_4 : విలువ_పునాది
{
  రక్షిత:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  విలువ_Format   విలువ_Format1;           /* ValueRecord definition--for the
                                         * first glyph of the pair--may be zero
                                         * (0) */
  విలువ_Format   విలువ_Format2;           /* ValueRecord definition--for the
                                         * second glyph of the pair--may be
                                         * zero (0) */
  రకం_పేరు Types::మూస LOffset_గమ్యం<ClassDef>
                classDef1;              /* Offset to ClassDef table--from
                                         * beginning of PairPos subtable--for
                                         * the first glyph of the pair */
  రకం_పేరు Types::మూస LOffset_గమ్యం<ClassDef>
                classDef2;              /* Offset to ClassDef table--from
                                         * beginning of PairPos subtable--for
                                         * the second glyph of the pair */
  HBUINT16      class1_లెక్క_2;            /* Number of classes in ClassDef1
                                         * table--includes Class0 */
  HBUINT16      class2_లెక్క_2;            /* Number of classes in ClassDef2
                                         * table--includes Class0 */
  విలువ_రికార్డు   values;                 /* Matrix of value pairs:
                                         * class1-major, class2-minor,
                                         * Each entry has value1 and value2 */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (10 + 3 * Types::LOffset::static_కొలత, values);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!(c->check_struct (ఇది)
       && coverage.sanitize (c, ఇది)
       && classDef1.sanitize (c, ఇది)
       && classDef2.sanitize (c, ఇది))) return_trace (అబద్ధం);

    గుర్తులేని పూర్ణాంకం len1 = విలువ_Format1.get_len ();
    గుర్తులేని పూర్ణాంకం len2 = విలువ_Format2.get_len ();
    గుర్తులేని పూర్ణాంకం stride = HBUINT16::static_కొలత * (len1 + len2);
    గుర్తులేని పూర్ణాంకం లెక్క = (గుర్తులేని పూర్ణాంకం) class1_లెక్క_2 * (గుర్తులేని పూర్ణాంకం) class2_లెక్క_2;
    return_trace (c->check_range ((స్థిరరాశి శూన్య *) values,
                                  లెక్క,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (విలువ_Format1.sanitize_విలువలు_stride_unsafe (c, ఇది, &values[0], లెక్క, stride) &&
		    విలువ_Format2.sanitize_విలువలు_stride_unsafe (c, ఇది, &values[len1], లెక్క, stride))));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    తిరిగి (ఇది+coverage).intersects (glyphs) &&
           (ఇది+classDef2).intersects (glyphs);
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి {}
  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    collect_pair_variation_indices (c, ఇది, ఇది+coverage,
                                    ఇది+classDef1, ఇది+classDef2,
                                    విలువ_Format1, విలువ_Format2,
                                    class1_లెక్క_2, class2_లెక్క_2, values);
  }

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!(ఇది+coverage).collect_coverage (c->ఆదానం))) తిరిగి;
    ఒకవేళ (unlikely (!(ఇది+classDef2).collect_coverage (c->ఆదానం))) తిరిగి;
  }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి { తిరిగి ఇది+coverage; }

  నిర్మాణం external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t మొదటి;
    hb_ot_layout_mapping_cache_t second;
  };
  శూన్య *external_cache_create () స్థిరరాశి
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (పరిమాణం (external_cache_t));
    ఒకవేళ (likely (cache))
    {
      cache->coverage.clear ();
      cache->మొదటి.clear ();
      cache->second.clear ();
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
    గుర్తులేని పూర్ణాంకం klass1 = (ఇది+classDef1).get_class (బఫరు->cur().codepoint, cache ? &cache->మొదటి : శూన్యసూచిక);
    గుర్తులేని పూర్ణాంకం klass2 = (ఇది+classDef2).get_class (బఫరు->info[skippy_iter.idx].codepoint, cache ? &cache->second : శూన్యసూచిక);
#లేకపోతే
    గుర్తులేని పూర్ణాంకం klass1 = (ఇది+classDef1).get_class (బఫరు->cur().codepoint);
    గుర్తులేని పూర్ణాంకం klass2 = (ఇది+classDef2).get_class (బఫరు->info[skippy_iter.idx].codepoint);
#అయితే_ముగింపు
    ఒకవేళ (unlikely (klass1 >= class1_లెక్క_2 || klass2 >= class2_లెక్క_2))
    {
      బఫరు->unsafe_గమ్యం_concat (బఫరు->idx, skippy_iter.idx + 1);
      return_trace (అబద్ధం);
    }

    గుర్తులేని పూర్ణాంకం len1 = విలువ_Format1.get_len ();
    గుర్తులేని పూర్ణాంకం len2 = విలువ_Format2.get_len ();
    గుర్తులేని పూర్ణాంకం రికార్డు_len = len1 + len2;

    స్థిరరాశి విలువ *v = &values[రికార్డు_len * (klass1 * class2_లెక్క_2 + klass2)];

    తార్కిక applied_మొదటి = అబద్ధం, applied_రెండవ = అబద్ధం;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#నిర్వచితం_కాకపోతే HB_విభజించు_KERN
    ఒకవేళ (అబద్ధం)
#అయితే_ముగింపు
    {
      ఒకవేళ (!len2)
      {
        స్థిరరాశి hb_direction_t dir = బఫరు->props.direction;
        స్థిరరాశి తార్కిక horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        స్థిరరాశి తార్కిక backward = HB_DIRECTION_IS_BACKWARD (dir);
        గుర్తులేని ముసుగు = horizontal ? విలువ_Format::xAdvance : విలువ_Format::yAdvance;
        ఒకవేళ (backward)
          ముసుగు |= ముసుగు >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        ముసుగు |= ముసుగు << 4;

        ఒకవేళ (విలువ_Format1 & ~ముసుగు)
          వెళ్ళు bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        ఒకవేళ (విలువ_Format1.apply_విలువ (c, ఇది, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &బఫరు->cur_pos().x_advance;
          hb_position_t *dst2 = &బఫరు->pos[skippy_iter.idx].x_advance;
          గుర్తులేని i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          ఒకవేళ (!backward)
          {
            dst1[i] = hb_saturate_కూడు (dst1[i], kern1);
            dst2[i] = hb_saturate_కూడు (dst2[i], kern2);
            dst2[i + 2] = hb_saturate_కూడు (dst2[i + 2], kern2);
          }
          లేకపోతే
          {
            dst1[i] = hb_saturate_కూడు (dst1[i], kern1);
            dst1[i + 2] = hb_clamp_గమ్యం<hb_position_t> ((int64_t) dst1[i + 2] + src[i + 2] - kern2);
            dst2[i] = hb_saturate_కూడు (dst2[i], kern2);
          }

          applied_మొదటి = applied_రెండవ = kern != 0;
          వెళ్ళు success;
        }
        వెళ్ళు boring;
      }
    }
    bail:

    ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
    {
      c->బఫరు->సందేశం (c->font,
			  "try kerning glyphs at %u,%u",
			  c->బఫరు->idx, skippy_iter.idx);
    }

    applied_మొదటి = len1 && విలువ_Format1.apply_విలువ (c, ఇది, v, బఫరు->cur_pos());
    applied_రెండవ = len2 && విలువ_Format2.apply_విలువ (c, ఇది, v + len1, బఫరు->pos[skippy_iter.idx]);

    ఒకవేళ (applied_మొదటి || applied_రెండవ)
      ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
      {
	c->బఫరు->సందేశం (c->font,
			    "kerned glyphs at %u,%u",
			    c->బఫరు->idx, skippy_iter.idx);
      }

    ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
    {
      c->బఫరు->సందేశం (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->బఫరు->idx, skippy_iter.idx);
    }

    success:
    ఒకవేళ (applied_మొదటి || applied_రెండవ)
      బఫరు->unsafe_గమ్యం_break (బఫరు->idx, skippy_iter.idx + 1);
    లేకపోతే
    boring:
      బఫరు->unsafe_గమ్యం_concat (బఫరు->idx, skippy_iter.idx + 1);

    ఒకవేళ (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      బఫరు->unsafe_గమ్యం_break (బఫరు->idx, skippy_iter.idx + 1);
    }

    బఫరు->idx = skippy_iter.idx;

    return_trace (నిజం);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
    బహిర్గతం->format = format;

    hb_map_t klass1_map;
    బహిర్గతం->classDef1.serialize_subset (c, classDef1, ఇది, &klass1_map, నిజం, నిజం, &(ఇది + coverage));
    బహిర్గతం->class1_లెక్క_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    బహిర్గతం->classDef2.serialize_subset (c, classDef2, ఇది, &klass2_map, నిజం, అబద్ధం);
    బహిర్గతం->class2_లెక్క_2 = klass2_map.get_population ();

    pair_pos_class_విలువలు_t class_విలువలు {ఇది, values, విలువ_Format1, విలువ_Format2,
                                         class1_లెక్క_2, class2_లెక్క_2};

    hb_pair_t<గుర్తులేని, గుర్తులేని> newFormats = hb_pair (విలువ_Format1, విలువ_Format2);

    ఒకవేళ (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_విలువలు.compute_effective_విలువ_formats (klass1_map, klass2_map, అబద్ధం, అబద్ధం, &c->plan->layout_variation_idx_delta_map);
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
      newFormats = class_విలువలు.compute_effective_విలువ_formats (klass1_map, klass2_map, strip, నిజం);
    }

    బహిర్గతం->విలువ_Format1 = newFormats.మొదటి;
    బహిర్గతం->విలువ_Format2 = newFormats.second;

    ఒకవేళ (!class_విలువలు.serialize (c, klass1_map, klass2_map,
                                 బహిర్గతం->విలువ_Format1, బహిర్గతం->విలువ_Format2))
      return_trace (అబద్ధం);

    తార్కిక ret = బహిర్గతం->coverage.serialize_subset(c, coverage, ఇది);
    return_trace (బహిర్గతం->class1_లెక్క_2 && బహిర్గతం->class2_లెక్క_2 && ret);
  }
};

}
}
}

#అయితే_ముగింపు  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
