#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#अन्तर्भाव "PairSet.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {


ढाँचा <typename Types>
संरचना PairPosFormat1_3
{
  using PairSet = GPOS_impl::PairSet<Types>;
  using Pair_मूल्य_Record = GPOS_impl::Pair_मूल्य_Record<Types>;

  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  मूल्य_Format   मूल्य_Format_2[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  typename Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<PairSet>>
                pairSet;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + 2 * मूल्य_Format::static_size + Types::HBUINT::static_size, pairSet);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);

    यदि (!c->check_struct (एतत्)) return_trace (असत्य);
    hb_barrier ();

    अचिन्हित पूर्णाङ्क len1 = मूल्य_Format_2[0].get_len ();
    अचिन्हित पूर्णाङ्क len2 = मूल्य_Format_2[1].get_len ();
    typename PairSet::sanitize_closure_t closure =
    {
      मूल्य_Format_2,
      len1,
      PairSet::get_size (len1, len2)
    };

    return_trace (coverage.sanitize (c, एतत्) && pairSet.sanitize (c, एतत्, &closure));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    स्वतः &cov = एतत्+coverage;

    यदि (pairSet.len > glyphs->get_population () * hb_bit_storage ((अचिन्हित) pairSet.len))
    {
      प्रति (hb_codepoint_t g : glyphs->iter ())
      {
	अचिन्हित i = cov.get_coverage (g);
	यदि ((एतत्+pairSet[i]).intersects (glyphs, मूल्य_Format_2))
	  प्रत्यागम सत्य;
      }
      प्रत्यागम असत्य;
    }

    प्रत्यागम
    + hb_zip (cov, pairSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([glyphs, एतत्] (ध्रुव typename Types::ढाँचा OffsetTo<PairSet> &_)
              { प्रत्यागम (एतत्+_).intersects (glyphs, मूल्य_Format_2); })
    | hb_any
    ;
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव {}
  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    यदि ((!मूल्य_Format_2[0].has_device ()) && (!मूल्य_Format_2[1].has_device ())) प्रत्यागम;

    स्वतः it =
    + hb_zip (एतत्+coverage, pairSet)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    ;

    यदि (!it) प्रत्यागम;
    + it
    | hb_map (hb_add (एतत्))
    | hb_apply ([&] (ध्रुव PairSet& _) { _.collect_variation_indices (c, मूल्य_Format_2); })
    ;
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    यदि (unlikely (!(एतत्+coverage).collect_coverage (c->निवेश))) प्रत्यागम;
    अचिन्हित पूर्णाङ्क गिनती = pairSet.len;
    प्रति (अचिन्हित पूर्णाङ्क i = 0; i < गिनती; i++)
      (एतत्+pairSet[i]).collect_glyphs (c, मूल्य_Format_2);
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) ध्रुव
  {
    अचिन्हित गिनती = pairSet.len;
    प्रति (अचिन्हित i = 0; i < गिनती; i++)
      (एतत्+pairSet[i]).collect_second_glyphs (digest, मूल्य_Format_2);
  }

  ध्रुव Coverage &get_coverage () ध्रुव { प्रत्यागम एतत्+coverage; }

  संरचना external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t pair_sets[HB_VAR_ARRAY];
  };
  शून्य *external_cache_create () ध्रुव
  {
    अचिन्हित गिनती = pairSet.len;
    अचिन्हित size = परिमाण (external_cache_t) -
		    HB_VAR_ARRAY * परिमाण (hb_set_digest_t) +
		    गिनती * परिमाण (hb_set_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    यदि (likely (cache))
    {
      cache->coverage.clear ();
      प्रति (अचिन्हित i = 0; i < गिनती; i++)
      {
	cache->pair_sets[i].init ();
	(एतत्+pairSet[i]).collect_second_glyphs (&cache->pair_sets[i], मूल्य_Format_2);
      }
    }
    प्रत्यागम cache;
  }

  तार्किक apply (hb_ot_apply_context_t *c, शून्य *external_cache) ध्रुव
  {
    TRACE_APPLY (एतत्);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#अन्यथा
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    यदि (index == NOT_COVERED) return_trace (असत्य);

    स्वतः &skippy_iter = c->iter_निवेश;
    skippy_iter.reset_fast (buffer->idx);
    अचिन्हित unsafe_to;
    यदि (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (असत्य);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    यदि (cache &&
	index < pairSet.len &&
	!cache->pair_sets[index].may_have (buffer->info[skippy_iter.idx].codepoint))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (असत्य);
    }
#endif

    return_trace ((एतत्+pairSet[index]).apply (c, मूल्य_Format_2, skippy_iter.idx));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;

    hb_pair_t<अचिन्हित, अचिन्हित> newFormats = hb_pair (मूल्य_Format_2[0], मूल्य_Format_2[1]);

    यदि (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_मूल्य_formats (glyphset, असत्य, असत्य, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    अन्यथा यदि (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      तार्किक has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      तार्किक strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      यदि (has_fvar && !c->plan->has_gdef_varstore)
        strip = सत्य;
      newFormats = compute_effective_मूल्य_formats (glyphset, strip, सत्य);
    }

    out->मूल्य_Format_2[0] = newFormats.first;
    out->मूल्य_Format_2[1] = newFormats.second;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;

    + hb_zip (एतत्+coverage, pairSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter ([एतत्, c, out] (ध्रुव typename Types::ढाँचा OffsetTo<PairSet>& _)
                 {
                   स्वतः snap = c->serializer->snapshot ();
                   स्वतः *o = out->pairSet.serialize_append (c->serializer);
                   यदि (unlikely (!o)) प्रत्यागम असत्य;
                   तार्किक ret = o->serialize_subset (c, _, एतत्, मूल्य_Format_2, out->मूल्य_Format_2);
                   यदि (!ret)
                   {
                     out->pairSet.निकालो ();
                     c->serializer->revert (snap);
                   }
                   प्रत्यागम ret;
                 },
                 hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (तार्किक (new_coverage));
  }


  hb_pair_t<अचिन्हित, अचिन्हित> compute_effective_मूल्य_formats (ध्रुव hb_set_t& glyphset,
                                                                 तार्किक strip_hints, तार्किक strip_empty,
                                                                 ध्रुव hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *varidx_delta_map = nullptr) ध्रुव
  {
    अचिन्हित format1 = 0;
    अचिन्हित format2 = 0;
    प्रति (ध्रुव स्वतः & _ :
	  + hb_zip (एतत्+coverage, pairSet)
	  | hb_filter (glyphset, hb_first)
	  | hb_map (hb_second)
	)
    {
      ध्रुव PairSet& set = (एतत् + _);
      स्वतः formats = set.get_मूल्यावली_view (मूल्य_Format_2).compute_effective_मूल्य_formats (
          glyphset, मूल्य_Format_2, strip_hints, strip_empty, varidx_delta_map);
      format1 |= formats.first;
      format2 |= formats.second;

      यदि (!varidx_delta_map &&
          format1 == मूल्य_Format_2[0] && format2 == मूल्य_Format_2[1])
        भङ्ग;
    }

    प्रत्यागम hb_pair (format1, format2);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
