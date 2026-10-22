#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "ValueFormat.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

HB_ਅੰਦਰੂਨੀ ਸਤਰਅੰਦਰ
ਖਾਲੀ collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *ਅਧਾਰ,
                                     ਅਚਲ Coverage &coverage,
                                     ਅਚਲ ClassDef &class_def1,
                                     ਅਚਲ ClassDef &class_def2,
                                     ਅਚਲ ਮੁੱਲ_Format &ਮੁੱਲ_Format1,
                                     ਅਚਲ ਮੁੱਲ_Format &ਮੁੱਲ_Format2,
                                     ਬਿਨਾਨਿਸ਼ਾਨੀ class1_ਗਿਣਤੀ,
                                     ਬਿਨਾਨਿਸ਼ਾਨੀ class2_ਗਿਣਤੀ,
                                     ਅਚਲ ਮੁੱਲ_ਰਿਕਾਰਡ &values)
{
  ਜੇ (!coverage.intersects (c->glyph_ਸੈੱਟ) ||
      !class_def2.intersects (c->glyph_ਸੈੱਟ)) ਵਾਪਸ;
  ਜੇ ((!ਮੁੱਲ_Format1.has_device ()) && (!ਮੁੱਲ_Format2.has_device ())) ਵਾਪਸ;

  hb_ਸੈੱਟ_t klass1_glyphs, klass2_glyphs;
  ਜੇ (!class_def1.collect_coverage (&klass1_glyphs)) ਵਾਪਸ;
  ਜੇ (!class_def2.collect_coverage (&klass2_glyphs)) ਵਾਪਸ;

  hb_ਸੈੱਟ_t class1_ਸੈੱਟ, class2_ਸੈੱਟ;
  ਲਈ (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ cp : + c->glyph_ਸੈੱਟ->iter () | hb_filter (coverage))
  {
    ਜੇ (!klass1_glyphs.has (cp)) class1_ਸੈੱਟ.ਜੋੜੋ (0);
    ਨਹੀਂਤਾਂ
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ klass1 = class_def1.get (cp);
      class1_ਸੈੱਟ.ਜੋੜੋ (klass1);
    }
  }

  class2_ਸੈੱਟ.ਜੋੜੋ (0);
  ਲਈ (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ cp : + c->glyph_ਸੈੱਟ->iter () | hb_filter (klass2_glyphs))
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ klass2 = class_def2.get (cp);
    class2_ਸੈੱਟ.ਜੋੜੋ (klass2);
  }

  ਜੇ (class1_ਸੈੱਟ.is_empty ()
      || class2_ਸੈੱਟ.is_empty ()
      || (class2_ਸੈੱਟ.get_population() == 1 && class2_ਸੈੱਟ.has(0)))
    ਵਾਪਸ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ len1 = ਮੁੱਲ_Format1.get_len ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ len2 = ਮੁੱਲ_Format2.get_len ();
  ਅਚਲ hb_array_t<ਅਚਲ ਮੁੱਲ> ਮੁੱਲਾਂ_array = values.as_array ((ਬਿਨਾਨਿਸ਼ਾਨੀ)class1_ਗਿਣਤੀ * (ਬਿਨਾਨਿਸ਼ਾਨੀ) class2_ਗਿਣਤੀ * (len1 + len2));
  ਲਈ (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ class1_idx : class1_ਸੈੱਟ.iter ())
  {
    ਲਈ (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ class2_idx : class2_ਸੈੱਟ.iter ())
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_ਵਿੱਥ = (class1_idx * (ਬਿਨਾਨਿਸ਼ਾਨੀ) class2_ਗਿਣਤੀ + class2_idx) * (len1 + len2);
      ਜੇ (ਮੁੱਲ_Format1.has_device ())
        ਮੁੱਲ_Format1.collect_variation_indices (c, ਅਧਾਰ, ਮੁੱਲਾਂ_array.sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, len1));

      ਜੇ (ਮੁੱਲ_Format2.has_device ())
        ਮੁੱਲ_Format2.collect_variation_indices (c, ਅਧਾਰ, ਮੁੱਲਾਂ_array.sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
ਬਣਤਰ pair_pos_class_ਮੁੱਲਾਂ_t
{
  ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *ਅਧਾਰ;
  ਅਚਲ ਮੁੱਲ_ਰਿਕਾਰਡ &values;
  ਅਚਲ ਮੁੱਲ_Format &ਮੁੱਲ_Format1;
  ਅਚਲ ਮੁੱਲ_Format &ਮੁੱਲ_Format2;
  ਬਿਨਾਨਿਸ਼ਾਨੀ class1_ਗਿਣਤੀ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ class2_ਗਿਣਤੀ;

  hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> compute_effective_ਮੁੱਲ_formats (ਅਚਲ hb_map_t &klass1_map,
                                                               ਅਚਲ hb_map_t &klass2_map,
                                                               ਤਾਰਕਿਕ strip_hints, ਤਾਰਕਿਕ strip_ਖਾਲੀ_ਥਾਵਾਂ,
                                                               ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *varidx_delta_map = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ len1 = ਮੁੱਲ_Format1.get_len ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ len2 = ਮੁੱਲ_Format2.get_len ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਰਿਕਾਰਡ_ਮਾਪ = len1 + len2;

    ਬਿਨਾਨਿਸ਼ਾਨੀ format1 = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ format2 = 0;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ class1_idx : + hb_range (class1_ਗਿਣਤੀ) | hb_filter (klass1_map))
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ class2_idx : + hb_range (class2_ਗਿਣਤੀ) | hb_filter (klass2_map))
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ idx = (class1_idx * class2_ਗਿਣਤੀ + class2_idx) * ਰਿਕਾਰਡ_ਮਾਪ;
        format1 = format1 | ਮੁੱਲ_Format1.get_effective_format (&values[idx], strip_hints, strip_ਖਾਲੀ_ਥਾਵਾਂ, ਅਧਾਰ, varidx_delta_map);
        format2 = format2 | ਮੁੱਲ_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_ਖਾਲੀ_ਥਾਵਾਂ, ਅਧਾਰ, varidx_delta_map);
      }

      ਜੇ (!varidx_delta_map &&
          format1 == ਮੁੱਲ_Format1 && format2 == ਮੁੱਲ_Format2)
        ਰੋਕੋ;
    }

    ਵਾਪਸ hb_pair (format1, format2);
  }

  ਤਾਰਕਿਕ serialize (hb_subset_context_t *c,
                  ਅਚਲ hb_map_t &klass1_map,
                  ਅਚਲ hb_map_t &klass2_map,
                  ਮੁੱਲ_Format newFormat1,
                  ਮੁੱਲ_Format newFormat2) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ len1 = ਮੁੱਲ_Format1.get_len ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ len2 = ਮੁੱਲ_Format2.get_len ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_len = len1 + len2;
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> class2_idxs (+ hb_range (class2_ਗਿਣਤੀ) | hb_filter (klass2_map));
    ਜੇ (unlikely (!c->serializer->propagate_error (class2_idxs)))
      ਵਾਪਸ ਝੂਠ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ class1_idx : + hb_range (class1_ਗਿਣਤੀ) | hb_filter (klass1_map))
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ class2_idx : class2_idxs)
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ idx = (class1_idx * class2_ਗਿਣਤੀ + class2_idx) * ਕੁੱਲ_len;
        ਮੁੱਲ_Format1.copy_ਮੁੱਲਾਂ (c->serializer, newFormat1, ਅਧਾਰ, &values[idx], &c->plan->layout_variation_idx_delta_map);
        ਮੁੱਲ_Format2.copy_ਮੁੱਲਾਂ (c->serializer, newFormat2, ਅਧਾਰ, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    ਵਾਪਸ ਸੱਚ;
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ PairPosFormat2_4 : ਮੁੱਲ_ਅਧਾਰ
{
  ਸੁਰੱਖਿਅਤ:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ਮੁੱਲ_Format   ਮੁੱਲ_Format1;           /* ValueRecord definition--for the
                                         * first glyph of the pair--may be zero
                                         * (0) */
  ਮੁੱਲ_Format   ਮੁੱਲ_Format2;           /* ValueRecord definition--for the
                                         * second glyph of the pair--may be
                                         * zero (0) */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<ClassDef>
                classDef1;              /* Offset to ClassDef table--from
                                         * beginning of PairPos subtable--for
                                         * the first glyph of the pair */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<ClassDef>
                classDef2;              /* Offset to ClassDef table--from
                                         * beginning of PairPos subtable--for
                                         * the second glyph of the pair */
  HBUINT16      class1_ਗਿਣਤੀ_2;            /* Number of classes in ClassDef1
                                         * table--includes Class0 */
  HBUINT16      class2_ਗਿਣਤੀ_2;            /* Number of classes in ClassDef2
                                         * table--includes Class0 */
  ਮੁੱਲ_ਰਿਕਾਰਡ   values;                 /* Matrix of value pairs:
                                         * class1-major, class2-minor,
                                         * Each entry has value1 and value2 */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (10 + 3 * Types::LOffset::static_ਮਾਪ, values);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!(c->check_struct (ਇਹ)
       && coverage.sanitize (c, ਇਹ)
       && classDef1.sanitize (c, ਇਹ)
       && classDef2.sanitize (c, ਇਹ))) return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len1 = ਮੁੱਲ_Format1.get_len ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len2 = ਮੁੱਲ_Format2.get_len ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stride = HBUINT16::static_ਮਾਪ * (len1 + len2);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) class1_ਗਿਣਤੀ_2 * (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) class2_ਗਿਣਤੀ_2;
    return_trace (c->check_range ((ਅਚਲ ਖਾਲੀ *) values,
                                  ਗਿਣਤੀ_2,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (ਮੁੱਲ_Format1.sanitize_ਮੁੱਲਾਂ_stride_unsafe (c, ਇਹ, &values[0], ਗਿਣਤੀ_2, stride) &&
		    ਮੁੱਲ_Format2.sanitize_ਮੁੱਲਾਂ_stride_unsafe (c, ਇਹ, &values[len1], ਗਿਣਤੀ_2, stride))));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਵਾਪਸ (ਇਹ+coverage).intersects (glyphs) &&
           (ਇਹ+classDef2).intersects (glyphs);
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ {}
  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    collect_pair_variation_indices (c, ਇਹ, ਇਹ+coverage,
                                    ਇਹ+classDef1, ਇਹ+classDef2,
                                    ਮੁੱਲ_Format1, ਮੁੱਲ_Format2,
                                    class1_ਗਿਣਤੀ_2, class2_ਗਿਣਤੀ_2, values);
  }

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    ਜੇ (unlikely (!(ਇਹ+coverage).collect_coverage (c->ਨਿਵੇਸ਼))) ਵਾਪਸ;
    ਜੇ (unlikely (!(ਇਹ+classDef2).collect_coverage (c->ਨਿਵੇਸ਼))) ਵਾਪਸ;
  }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+coverage; }

  ਬਣਤਰ external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t ਪਹਿਲਾ;
    hb_ot_layout_mapping_cache_t second;
  };
  ਖਾਲੀ *external_cache_create () ਅਚਲ
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (ਆਕਾਰ (external_cache_t));
    ਜੇ (likely (cache))
    {
      cache->coverage.clear ();
      cache->ਪਹਿਲਾ.clear ();
      cache->second.clear ();
    }
    ਵਾਪਸ cache;
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c, ਖਾਲੀ *external_cache) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage  (ਬਫਰ->cur().codepoint, cache ? &cache->coverage : ਖਾਲੀ_ਸੂਚਕ);
#ਨਹੀਂਤਾਂ
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage  (ਬਫਰ->cur().codepoint);
#ਜੇ_ਅੰਤ
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED) return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ &skippy_iter = c->iter_ਨਿਵੇਸ਼;
    skippy_iter.reset_fast (ਬਫਰ->idx);
    ਬਿਨਾਨਿਸ਼ਾਨੀ unsafe_ਮੰਜ਼ਿਲ;
    ਜੇ (unlikely (!skippy_iter.next (&unsafe_ਮੰਜ਼ਿਲ)))
    {
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (ਬਫਰ->idx, unsafe_ਮੰਜ਼ਿਲ);
      return_trace (ਝੂਠ);
    }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ klass1 = (ਇਹ+classDef1).get_class (ਬਫਰ->cur().codepoint, cache ? &cache->ਪਹਿਲਾ : ਖਾਲੀ_ਸੂਚਕ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ klass2 = (ਇਹ+classDef2).get_class (ਬਫਰ->info[skippy_iter.idx].codepoint, cache ? &cache->second : ਖਾਲੀ_ਸੂਚਕ);
#ਨਹੀਂਤਾਂ
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ klass1 = (ਇਹ+classDef1).get_class (ਬਫਰ->cur().codepoint);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ klass2 = (ਇਹ+classDef2).get_class (ਬਫਰ->info[skippy_iter.idx].codepoint);
#ਜੇ_ਅੰਤ
    ਜੇ (unlikely (klass1 >= class1_ਗਿਣਤੀ_2 || klass2 >= class2_ਗਿਣਤੀ_2))
    {
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (ਬਫਰ->idx, skippy_iter.idx + 1);
      return_trace (ਝੂਠ);
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len1 = ਮੁੱਲ_Format1.get_len ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len2 = ਮੁੱਲ_Format2.get_len ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਰਿਕਾਰਡ_len = len1 + len2;

    ਅਚਲ ਮੁੱਲ *v = &values[ਰਿਕਾਰਡ_len * (klass1 * class2_ਗਿਣਤੀ_2 + klass2)];

    ਤਾਰਕਿਕ applied_ਪਹਿਲਾ = ਝੂਠ, applied_ਦੂਜਾ = ਝੂਠ;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਵੰਡੋ_KERN
    ਜੇ (ਝੂਠ)
#ਜੇ_ਅੰਤ
    {
      ਜੇ (!len2)
      {
        ਅਚਲ hb_direction_t dir = ਬਫਰ->props.direction;
        ਅਚਲ ਤਾਰਕਿਕ horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        ਅਚਲ ਤਾਰਕਿਕ backward = HB_DIRECTION_IS_BACKWARD (dir);
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਕਾਬ = horizontal ? ਮੁੱਲ_Format::xAdvance : ਮੁੱਲ_Format::yAdvance;
        ਜੇ (backward)
          ਨਕਾਬ |= ਨਕਾਬ >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        ਨਕਾਬ |= ਨਕਾਬ << 4;

        ਜੇ (ਮੁੱਲ_Format1 & ~ਨਕਾਬ)
          ਜਾਓ bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        ਜੇ (ਮੁੱਲ_Format1.apply_ਮੁੱਲ (c, ਇਹ, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &ਬਫਰ->cur_pos().x_advance;
          hb_position_t *dst2 = &ਬਫਰ->pos[skippy_iter.idx].x_advance;
          ਬਿਨਾਨਿਸ਼ਾਨੀ i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          ਜੇ (!backward)
          {
            dst1[i] = hb_saturate_ਜੋੜੋ (dst1[i], kern1);
            dst2[i] = hb_saturate_ਜੋੜੋ (dst2[i], kern2);
            dst2[i + 2] = hb_saturate_ਜੋੜੋ (dst2[i + 2], kern2);
          }
          ਨਹੀਂਤਾਂ
          {
            dst1[i] = hb_saturate_ਜੋੜੋ (dst1[i], kern1);
            dst1[i + 2] = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) dst1[i + 2] + src[i + 2] - kern2);
            dst2[i] = hb_saturate_ਜੋੜੋ (dst2[i], kern2);
          }

          applied_ਪਹਿਲਾ = applied_ਦੂਜਾ = kern != 0;
          ਜਾਓ success;
        }
        ਜਾਓ boring;
      }
    }
    bail:

    ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
    {
      c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "try kerning glyphs at %u,%u",
			  c->ਬਫਰ->idx, skippy_iter.idx);
    }

    applied_ਪਹਿਲਾ = len1 && ਮੁੱਲ_Format1.apply_ਮੁੱਲ (c, ਇਹ, v, ਬਫਰ->cur_pos());
    applied_ਦੂਜਾ = len2 && ਮੁੱਲ_Format2.apply_ਮੁੱਲ (c, ਇਹ, v + len1, ਬਫਰ->pos[skippy_iter.idx]);

    ਜੇ (applied_ਪਹਿਲਾ || applied_ਦੂਜਾ)
      ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
      {
	c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			    "kerned glyphs at %u,%u",
			    c->ਬਫਰ->idx, skippy_iter.idx);
      }

    ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
    {
      c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->ਬਫਰ->idx, skippy_iter.idx);
    }

    success:
    ਜੇ (applied_ਪਹਿਲਾ || applied_ਦੂਜਾ)
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (ਬਫਰ->idx, skippy_iter.idx + 1);
    ਨਹੀਂਤਾਂ
    boring:
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (ਬਫਰ->idx, skippy_iter.idx + 1);

    ਜੇ (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (ਬਫਰ->idx, skippy_iter.idx + 1);
    }

    ਬਫਰ->idx = skippy_iter.idx;

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->format = format;

    hb_map_t klass1_map;
    ਬਾਹਰਲਾ->classDef1.serialize_subset (c, classDef1, ਇਹ, &klass1_map, ਸੱਚ, ਸੱਚ, &(ਇਹ + coverage));
    ਬਾਹਰਲਾ->class1_ਗਿਣਤੀ_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    ਬਾਹਰਲਾ->classDef2.serialize_subset (c, classDef2, ਇਹ, &klass2_map, ਸੱਚ, ਝੂਠ);
    ਬਾਹਰਲਾ->class2_ਗਿਣਤੀ_2 = klass2_map.get_population ();

    pair_pos_class_ਮੁੱਲਾਂ_t class_ਮੁੱਲਾਂ {ਇਹ, values, ਮੁੱਲ_Format1, ਮੁੱਲ_Format2,
                                         class1_ਗਿਣਤੀ_2, class2_ਗਿਣਤੀ_2};

    hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> newFormats = hb_pair (ਮੁੱਲ_Format1, ਮੁੱਲ_Format2);

    ਜੇ (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_ਮੁੱਲਾਂ.compute_effective_ਮੁੱਲ_formats (klass1_map, klass2_map, ਝੂਠ, ਝੂਠ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    ਨਹੀਂਤਾਂ ਜੇ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_ਸਾਰਣੀ (c->plan->ਸਰੋਤ, HB_ਟੈਗ ('f','v','a','r'));
      ਤਾਰਕਿਕ has_fvar = (blob != hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ());
      hb_blob_destroy (blob);

      ਤਾਰਕਿਕ strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      ਜੇ (has_fvar && !c->plan->has_gdef_varstore)
        strip = ਸੱਚ;
      newFormats = class_ਮੁੱਲਾਂ.compute_effective_ਮੁੱਲ_formats (klass1_map, klass2_map, strip, ਸੱਚ);
    }

    ਬਾਹਰਲਾ->ਮੁੱਲ_Format1 = newFormats.ਪਹਿਲਾ;
    ਬਾਹਰਲਾ->ਮੁੱਲ_Format2 = newFormats.second;

    ਜੇ (!class_ਮੁੱਲਾਂ.serialize (c, klass1_map, klass2_map,
                                 ਬਾਹਰਲਾ->ਮੁੱਲ_Format1, ਬਾਹਰਲਾ->ਮੁੱਲ_Format2))
      return_trace (ਝੂਠ);

    ਤਾਰਕਿਕ ret = ਬਾਹਰਲਾ->coverage.serialize_subset(c, coverage, ਇਹ);
    return_trace (ਬਾਹਰਲਾ->class1_ਗਿਣਤੀ_2 && ਬਾਹਰਲਾ->class2_ਗਿਣਤੀ_2 && ret);
  }
};

}
}
}

#ਜੇ_ਅੰਤ  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
