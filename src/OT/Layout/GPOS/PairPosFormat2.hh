#अगर_अघोषित OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#घोषणा OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#समावेश "ValueFormat.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

HB_INTERNAL अंतरभूत
व्योम collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     स्थिर मूल्य_Base *base,
                                     स्थिर Coverage &coverage,
                                     स्थिर ClassDef &class_def1,
                                     स्थिर ClassDef &class_def2,
                                     स्थिर मूल्य_Format &मूल्य_Format1,
                                     स्थिर मूल्य_Format &मूल्य_Format2,
                                     अचिन्हित class1_गिनती,
                                     अचिन्हित class2_गिनती,
                                     स्थिर मूल्य_Record &values)
{
  अगर (!coverage.intersects (c->glyph_set) ||
      !class_def2.intersects (c->glyph_set)) वापस;
  अगर ((!मूल्य_Format1.has_device ()) && (!मूल्य_Format2.has_device ())) वापस;

  hb_set_t klass1_glyphs, klass2_glyphs;
  अगर (!class_def1.collect_coverage (&klass1_glyphs)) वापस;
  अगर (!class_def2.collect_coverage (&klass2_glyphs)) वापस;

  hb_set_t class1_set, class2_set;
  क्रम (स्थिर अचिन्हित cp : + c->glyph_set->iter () | hb_filter (coverage))
  {
    अगर (!klass1_glyphs.has (cp)) class1_set.add (0);
    अन्यथा
    {
      अचिन्हित klass1 = class_def1.लो (cp);
      class1_set.add (klass1);
    }
  }

  class2_set.add (0);
  क्रम (स्थिर अचिन्हित cp : + c->glyph_set->iter () | hb_filter (klass2_glyphs))
  {
    अचिन्हित klass2 = class_def2.लो (cp);
    class2_set.add (klass2);
  }

  अगर (class1_set.is_empty ()
      || class2_set.is_empty ()
      || (class2_set.get_population() == 1 && class2_set.has(0)))
    वापस;

  अचिन्हित len1 = मूल्य_Format1.get_len ();
  अचिन्हित len2 = मूल्य_Format2.get_len ();
  स्थिर hb_array_t<स्थिर मूल्य> मूल्यावली_array = values.as_array ((अचिन्हित)class1_गिनती * (अचिन्हित) class2_गिनती * (len1 + len2));
  क्रम (स्थिर अचिन्हित class1_idx : class1_set.iter ())
  {
    क्रम (स्थिर अचिन्हित class2_idx : class2_set.iter ())
    {
      अचिन्हित start_offset = (class1_idx * (अचिन्हित) class2_गिनती + class2_idx) * (len1 + len2);
      अगर (मूल्य_Format1.has_device ())
        मूल्य_Format1.collect_variation_indices (c, base, मूल्यावली_array.sub_array (start_offset, len1));

      अगर (मूल्य_Format2.has_device ())
        मूल्य_Format2.collect_variation_indices (c, base, मूल्यावली_array.sub_array (start_offset+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
काष्ठा pair_pos_class_मूल्यावली_t
{
  स्थिर मूल्य_Base *base;
  स्थिर मूल्य_Record &values;
  स्थिर मूल्य_Format &मूल्य_Format1;
  स्थिर मूल्य_Format &मूल्य_Format2;
  अचिन्हित class1_गिनती;
  अचिन्हित class2_गिनती;

  hb_pair_t<अचिन्हित, अचिन्हित> compute_effective_मूल्य_formats (स्थिर hb_map_t &klass1_map,
                                                               स्थिर hb_map_t &klass2_map,
                                                               bool strip_hints, bool strip_empty,
                                                               स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *varidx_delta_map = nullptr) स्थिर
  {
    अचिन्हित len1 = मूल्य_Format1.get_len ();
    अचिन्हित len2 = मूल्य_Format2.get_len ();
    अचिन्हित record_आकार = len1 + len2;

    अचिन्हित format1 = 0;
    अचिन्हित format2 = 0;

    क्रम (अचिन्हित class1_idx : + hb_range (class1_गिनती) | hb_filter (klass1_map))
    {
      क्रम (अचिन्हित class2_idx : + hb_range (class2_गिनती) | hb_filter (klass2_map))
      {
        अचिन्हित idx = (class1_idx * class2_गिनती + class2_idx) * record_आकार;
        format1 = format1 | मूल्य_Format1.get_effective_format (&values[idx], strip_hints, strip_empty, base, varidx_delta_map);
        format2 = format2 | मूल्य_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_empty, base, varidx_delta_map);
      }

      अगर (!varidx_delta_map &&
          format1 == मूल्य_Format1 && format2 == मूल्य_Format2)
        अवरोध;
    }

    वापस hb_pair (format1, format2);
  }

  bool serialize (hb_subset_context_t *c,
                  स्थिर hb_map_t &klass1_map,
                  स्थिर hb_map_t &klass2_map,
                  मूल्य_Format newFormat1,
                  मूल्य_Format newFormat2) स्थिर
  {
    अचिन्हित len1 = मूल्य_Format1.get_len ();
    अचिन्हित len2 = मूल्य_Format2.get_len ();
    अचिन्हित योग_len = len1 + len2;
    hb_vector_t<अचिन्हित> class2_idxs (+ hb_range (class2_गिनती) | hb_filter (klass2_map));
    अगर (unlikely (!c->serializer->propagate_error (class2_idxs)))
      वापस false;
    क्रम (अचिन्हित class1_idx : + hb_range (class1_गिनती) | hb_filter (klass1_map))
    {
      क्रम (अचिन्हित class2_idx : class2_idxs)
      {
        अचिन्हित idx = (class1_idx * class2_गिनती + class2_idx) * योग_len;
        मूल्य_Format1.copy_मूल्यावली (c->serializer, newFormat1, base, &values[idx], &c->plan->layout_variation_idx_delta_map);
        मूल्य_Format2.copy_मूल्यावली (c->serializer, newFormat2, base, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    वापस true;
  }
};

ढाँचा <typename Types>
काष्ठा PairPosFormat2_4 : मूल्य_Base
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  मूल्य_Format   मूल्य_Format1;           /* ValueRecord definition--for the
                                         * first glyph of the pair--may be zero
                                         * (0) */
  मूल्य_Format   मूल्य_Format2;           /* ValueRecord definition--for the
                                         * second glyph of the pair--may be
                                         * zero (0) */
  typename Types::ढाँचा LOffsetTo<ClassDef>
                classDef1;              /* Offset to ClassDef table--from
                                         * beginning of PairPos subtable--for
                                         * the first glyph of the pair */
  typename Types::ढाँचा LOffsetTo<ClassDef>
                classDef2;              /* Offset to ClassDef table--from
                                         * beginning of PairPos subtable--for
                                         * the second glyph of the pair */
  HBUINT16      class1_गिनती_2;            /* Number of classes in ClassDef1
                                         * table--includes Class0 */
  HBUINT16      class2_गिनती_2;            /* Number of classes in ClassDef2
                                         * table--includes Class0 */
  मूल्य_Record   values;                 /* Matrix of value pairs:
                                         * class1-major, class2-minor,
                                         * Each entry has value1 and value2 */
  खुला:
  DEFINE_आकार_ARRAY (10 + 3 * Types::LOffset::static_आकार, values);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!(c->check_struct (यह)
       && coverage.sanitize (c, यह)
       && classDef1.sanitize (c, यह)
       && classDef2.sanitize (c, यह))) return_trace (false);

    अचिन्हित पूर्णांक len1 = मूल्य_Format1.get_len ();
    अचिन्हित पूर्णांक len2 = मूल्य_Format2.get_len ();
    अचिन्हित पूर्णांक stride = HBUINT16::static_आकार * (len1 + len2);
    अचिन्हित पूर्णांक गिनती = (अचिन्हित पूर्णांक) class1_गिनती_2 * (अचिन्हित पूर्णांक) class2_गिनती_2;
    return_trace (c->check_range ((स्थिर व्योम *) values,
                                  गिनती,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (मूल्य_Format1.sanitize_मूल्यावली_stride_unsafe (c, यह, &values[0], गिनती, stride) &&
		    मूल्य_Format2.sanitize_मूल्यावली_stride_unsafe (c, यह, &values[len1], गिनती, stride))));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    वापस (यह+coverage).intersects (glyphs) &&
           (यह+classDef2).intersects (glyphs);
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर {}
  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    collect_pair_variation_indices (c, यह, यह+coverage,
                                    यह+classDef1, यह+classDef2,
                                    मूल्य_Format1, मूल्य_Format2,
                                    class1_गिनती_2, class2_गिनती_2, values);
  }

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    अगर (unlikely (!(यह+coverage).collect_coverage (c->निवेश))) वापस;
    अगर (unlikely (!(यह+classDef2).collect_coverage (c->निवेश))) वापस;
  }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+coverage; }

  काष्ठा external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t first;
    hb_ot_layout_mapping_cache_t second;
  };
  व्योम *external_cache_create () स्थिर
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (माप (external_cache_t));
    अगर (likely (cache))
    {
      cache->coverage.सफा ();
      cache->first.सफा ();
      cache->second.सफा ();
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
    अचिन्हित पूर्णांक klass1 = (यह+classDef1).get_class (बफर->cur().codepoint, cache ? &cache->first : nullptr);
    अचिन्हित पूर्णांक klass2 = (यह+classDef2).get_class (बफर->info[skippy_iter.idx].codepoint, cache ? &cache->second : nullptr);
#अन्यथा
    अचिन्हित पूर्णांक klass1 = (यह+classDef1).get_class (बफर->cur().codepoint);
    अचिन्हित पूर्णांक klass2 = (यह+classDef2).get_class (बफर->info[skippy_iter.idx].codepoint);
#पूर्ण_अगर
    अगर (unlikely (klass1 >= class1_गिनती_2 || klass2 >= class2_गिनती_2))
    {
      बफर->unsafe_to_concat (बफर->idx, skippy_iter.idx + 1);
      return_trace (false);
    }

    अचिन्हित पूर्णांक len1 = मूल्य_Format1.get_len ();
    अचिन्हित पूर्णांक len2 = मूल्य_Format2.get_len ();
    अचिन्हित पूर्णांक record_len = len1 + len2;

    स्थिर मूल्य *v = &values[record_len * (klass1 * class2_गिनती_2 + klass2)];

    bool applied_first = false, applied_second = false;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#अगर_अघोषित HB_SPLIT_KERN
    अगर (false)
#पूर्ण_अगर
    {
      अगर (!len2)
      {
        स्थिर hb_direction_t dir = बफर->props.direction;
        स्थिर bool horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        स्थिर bool backward = HB_DIRECTION_IS_BACKWARD (dir);
        अचिन्हित mask = horizontal ? मूल्य_Format::xAdvance : मूल्य_Format::yAdvance;
        अगर (backward)
          mask |= mask >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        mask |= mask << 4;

        अगर (मूल्य_Format1 & ~mask)
          जाओ bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        अगर (मूल्य_Format1.apply_मूल्य (c, यह, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &बफर->cur_pos().x_advance;
          hb_position_t *dst2 = &बफर->pos[skippy_iter.idx].x_advance;
          अचिन्हित i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          अगर (!backward)
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
            dst2[i + 2] = hb_saturate_add (dst2[i + 2], kern2);
          }
          अन्यथा
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst1[i + 2] = hb_clamp_to<hb_position_t> ((int64_t) dst1[i + 2] + src[i + 2] - kern2);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
          }

          applied_first = applied_second = kern != 0;
          जाओ success;
        }
        जाओ boring;
      }
    }
    bail:

    अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
    {
      c->बफर->message (c->font,
			  "try kerning glyphs at %u,%u",
			  c->बफर->idx, skippy_iter.idx);
    }

    applied_first = len1 && मूल्य_Format1.apply_मूल्य (c, यह, v, बफर->cur_pos());
    applied_second = len2 && मूल्य_Format2.apply_मूल्य (c, यह, v + len1, बफर->pos[skippy_iter.idx]);

    अगर (applied_first || applied_second)
      अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
      {
	c->बफर->message (c->font,
			    "kerned glyphs at %u,%u",
			    c->बफर->idx, skippy_iter.idx);
      }

    अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
    {
      c->बफर->message (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->बफर->idx, skippy_iter.idx);
    }

    success:
    अगर (applied_first || applied_second)
      बफर->unsafe_to_break (बफर->idx, skippy_iter.idx + 1);
    अन्यथा
    boring:
      बफर->unsafe_to_concat (बफर->idx, skippy_iter.idx + 1);

    अगर (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      बफर->unsafe_to_break (बफर->idx, skippy_iter.idx + 1);
    }

    बफर->idx = skippy_iter.idx;

    return_trace (true);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->format = format;

    hb_map_t klass1_map;
    out->classDef1.serialize_subset (c, classDef1, यह, &klass1_map, true, true, &(यह + coverage));
    out->class1_गिनती_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    out->classDef2.serialize_subset (c, classDef2, यह, &klass2_map, true, false);
    out->class2_गिनती_2 = klass2_map.get_population ();

    pair_pos_class_मूल्यावली_t class_मूल्यावली {यह, values, मूल्य_Format1, मूल्य_Format2,
                                         class1_गिनती_2, class2_गिनती_2};

    hb_pair_t<अचिन्हित, अचिन्हित> newFormats = hb_pair (मूल्य_Format1, मूल्य_Format2);

    अगर (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_मूल्यावली.compute_effective_मूल्य_formats (klass1_map, klass2_map, false, false, &c->plan->layout_variation_idx_delta_map);
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
      newFormats = class_मूल्यावली.compute_effective_मूल्य_formats (klass1_map, klass2_map, strip, true);
    }

    out->मूल्य_Format1 = newFormats.first;
    out->मूल्य_Format2 = newFormats.second;

    अगर (!class_मूल्यावली.serialize (c, klass1_map, klass2_map,
                                 out->मूल्य_Format1, out->मूल्य_Format2))
      return_trace (false);

    bool ret = out->coverage.serialize_subset(c, coverage, यह);
    return_trace (out->class1_गिनती_2 && out->class2_गिनती_2 && ret);
  }
};

}
}
}

#पूर्ण_अगर  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
