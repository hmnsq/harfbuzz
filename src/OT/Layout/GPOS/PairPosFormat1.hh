#अगर_अघोषित OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#घोषणा OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#समावेश "PairSet.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {


ढाँचा <typename Types>
काष्ठा PairPosFormat1_3
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
  खुला:
  DEFINE_आकार_ARRAY (2 + Types::LOffset::static_आकार + 2 * मूल्य_Format::static_आकार + Types::HBUINT::static_आकार, pairSet);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);

    अगर (!c->check_struct (यह)) return_trace (false);
    hb_barrier ();

    अचिन्हित पूर्णांक len1 = मूल्य_Format_2[0].get_len ();
    अचिन्हित पूर्णांक len2 = मूल्य_Format_2[1].get_len ();
    typename PairSet::sanitize_closure_t closure =
    {
      मूल्य_Format_2,
      len1,
      PairSet::get_आकार (len1, len2)
    };

    return_trace (coverage.sanitize (c, यह) && pairSet.sanitize (c, यह, &closure));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    स्वतः &cov = यह+coverage;

    अगर (pairSet.len > glyphs->get_population () * hb_bit_storage ((अचिन्हित) pairSet.len))
    {
      क्रम (hb_codepoint_t g : glyphs->iter ())
      {
	अचिन्हित i = cov.get_coverage (g);
	अगर ((यह+pairSet[i]).intersects (glyphs, मूल्य_Format_2))
	  वापस true;
      }
      वापस false;
    }

    वापस
    + hb_zip (cov, pairSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([glyphs, यह] (स्थिर typename Types::ढाँचा OffsetTo<PairSet> &_)
              { वापस (यह+_).intersects (glyphs, मूल्य_Format_2); })
    | hb_any
    ;
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर {}
  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    अगर ((!मूल्य_Format_2[0].has_device ()) && (!मूल्य_Format_2[1].has_device ())) वापस;

    स्वतः it =
    + hb_zip (यह+coverage, pairSet)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    ;

    अगर (!it) वापस;
    + it
    | hb_map (hb_add (यह))
    | hb_apply ([&] (स्थिर PairSet& _) { _.collect_variation_indices (c, मूल्य_Format_2); })
    ;
  }

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    अगर (unlikely (!(यह+coverage).collect_coverage (c->निवेश))) वापस;
    अचिन्हित पूर्णांक गिनती = pairSet.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      (यह+pairSet[i]).collect_glyphs (c, मूल्य_Format_2);
  }

  व्योम collect_second_glyphs (hb_set_digest_t *digest) स्थिर
  {
    अचिन्हित गिनती = pairSet.len;
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      (यह+pairSet[i]).collect_second_glyphs (digest, मूल्य_Format_2);
  }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+coverage; }

  काष्ठा external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t pair_sets[HB_VAR_ARRAY];
  };
  व्योम *external_cache_create () स्थिर
  {
    अचिन्हित गिनती = pairSet.len;
    अचिन्हित आकार = माप (external_cache_t) -
		    HB_VAR_ARRAY * माप (hb_set_digest_t) +
		    गिनती * माप (hb_set_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (आकार);
    अगर (likely (cache))
    {
      cache->coverage.सफा ();
      क्रम (अचिन्हित i = 0; i < गिनती; i++)
      {
	cache->pair_sets[i].init ();
	(यह+pairSet[i]).collect_second_glyphs (&cache->pair_sets[i], मूल्य_Format_2);
      }
    }
    वापस cache;
  }

  bool apply (hb_ot_apply_context_t *c, व्योम *external_cache) स्थिर
  {
    TRACE_APPLY (यह);

    hb_बफर_t *बफर = c->बफर;

#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage  (बफर->cur().codepoint, cache ? &cache->coverage : nullptr);
#अन्यथा
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage  (बफर->cur().codepoint);
#पूर्ण_अगर
    अगर (सूचकांक_2 == NOT_COVERED) return_trace (false);

    स्वतः &skippy_iter = c->iter_निवेश;
    skippy_iter.reset_fast (बफर->idx);
    अचिन्हित unsafe_to;
    अगर (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      बफर->unsafe_to_concat (बफर->idx, unsafe_to);
      return_trace (false);
    }

#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    अगर (cache &&
	सूचकांक_2 < pairSet.len &&
	!cache->pair_sets[सूचकांक_2].may_have (बफर->info[skippy_iter.idx].codepoint))
    {
      बफर->unsafe_to_concat (बफर->idx, skippy_iter.idx + 1);
      return_trace (false);
    }
#पूर्ण_अगर

    return_trace ((यह+pairSet[सूचकांक_2]).apply (c, मूल्य_Format_2, skippy_iter.idx));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);

    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->format = format;

    hb_pair_t<अचिन्हित, अचिन्हित> newFormats = hb_pair (मूल्य_Format_2[0], मूल्य_Format_2[1]);

    अगर (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_मूल्य_formats (glyphset, false, false, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    अन्यथा अगर (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      bool has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      bool strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      अगर (has_fvar && !c->plan->has_gdef_varstore)
        strip = true;
      newFormats = compute_effective_मूल्य_formats (glyphset, strip, true);
    }

    out->मूल्य_Format_2[0] = newFormats.first;
    out->मूल्य_Format_2[1] = newFormats.second;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;

    + hb_zip (यह+coverage, pairSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter ([यह, c, out] (स्थिर typename Types::ढाँचा OffsetTo<PairSet>& _)
                 {
                   स्वतः snap = c->serializer->snapshot ();
                   स्वतः *o = out->pairSet.serialize_append (c->serializer);
                   अगर (unlikely (!o)) वापस false;
                   bool ret = o->serialize_subset (c, _, यह, मूल्य_Format_2, out->मूल्य_Format_2);
                   अगर (!ret)
                   {
                     out->pairSet.निकालो ();
                     c->serializer->revert (snap);
                   }
                   वापस ret;
                 },
                 hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (bool (new_coverage));
  }


  hb_pair_t<अचिन्हित, अचिन्हित> compute_effective_मूल्य_formats (स्थिर hb_set_t& glyphset,
                                                                 bool strip_hints, bool strip_empty,
                                                                 स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *varidx_delta_map = nullptr) स्थिर
  {
    अचिन्हित format1 = 0;
    अचिन्हित format2 = 0;
    क्रम (स्थिर स्वतः & _ :
	  + hb_zip (यह+coverage, pairSet)
	  | hb_filter (glyphset, hb_first)
	  | hb_map (hb_second)
	)
    {
      स्थिर PairSet& set = (यह + _);
      स्वतः formats = set.get_मूल्यावली_view (मूल्य_Format_2).compute_effective_मूल्य_formats (
          glyphset, मूल्य_Format_2, strip_hints, strip_empty, varidx_delta_map);
      format1 |= formats.first;
      format2 |= formats.second;

      अगर (!varidx_delta_map &&
          format1 == मूल्य_Format_2[0] && format2 == मूल्य_Format_2[1])
        अवरोध;
    }

    वापस hb_pair (format1, format2);
  }
};


}
}
}

#पूर्ण_अगर  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
