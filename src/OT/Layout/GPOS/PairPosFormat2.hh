#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#अन्तर्भाव "ValueFormat.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

HB_INTERNAL अन्तःस्थ
शून्य collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     ध्रुव मानम्_Base *base,
                                     ध्रुव Coverage &coverage,
                                     ध्रुव ClassDef &class_def1,
                                     ध्रुव ClassDef &class_def2,
                                     ध्रुव मानम्_Format &मानम्_Format1,
                                     ध्रुव मानम्_Format &मानम्_Format2,
                                     unsigned class1_गणनम्,
                                     unsigned class2_गणनम्,
                                     ध्रुव मानम्_Record &values)
{
  सचे (!coverage.intersects (c->glyph_set) ||
      !class_def2.intersects (c->glyph_set)) पटिनिवत्त;
  सचे ((!मानम्_Format1.has_device ()) && (!मानम्_Format2.has_device ())) पटिनिवत्त;

  hb_set_t klass1_glyphs, klass2_glyphs;
  सचे (!class_def1.collect_coverage (&klass1_glyphs)) पटिनिवत्त;
  सचे (!class_def2.collect_coverage (&klass2_glyphs)) पटिनिवत्त;

  hb_set_t class1_set, class2_set;
  अत्थाय (ध्रुव unsigned cp : + c->glyph_set->iter () | hb_filter (coverage))
  {
    सचे (!klass1_glyphs.has (cp)) class1_set.add (0);
    अञ्ञथा
    {
      unsigned klass1 = class_def1.get (cp);
      class1_set.add (klass1);
    }
  }

  class2_set.add (0);
  अत्थाय (ध्रुव unsigned cp : + c->glyph_set->iter () | hb_filter (klass2_glyphs))
  {
    unsigned klass2 = class_def2.get (cp);
    class2_set.add (klass2);
  }

  सचे (class1_set.is_empty ()
      || class2_set.is_empty ()
      || (class2_set.get_population() == 1 && class2_set.has(0)))
    पटिनिवत्त;

  unsigned len1 = मानम्_Format1.get_len ();
  unsigned len2 = मानम्_Format2.get_len ();
  ध्रुव hb_array_t<ध्रुव मानम्> मानानि_array = values.as_array ((unsigned)class1_गणनम् * (unsigned) class2_गणनम् * (len1 + len2));
  अत्थाय (ध्रुव unsigned class1_idx : class1_set.iter ())
  {
    अत्थाय (ध्रुव unsigned class2_idx : class2_set.iter ())
    {
      unsigned start_offset = (class1_idx * (unsigned) class2_गणनम् + class2_idx) * (len1 + len2);
      सचे (मानम्_Format1.has_device ())
        मानम्_Format1.collect_variation_indices (c, base, मानानि_array.sub_array (start_offset, len1));

      सचे (मानम्_Format2.has_device ())
        मानम्_Format2.collect_variation_indices (c, base, मानानि_array.sub_array (start_offset+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
संरचना pair_pos_class_मानानि_t
{
  ध्रुव मानम्_Base *base;
  ध्रुव मानम्_Record &values;
  ध्रुव मानम्_Format &मानम्_Format1;
  ध्रुव मानम्_Format &मानम्_Format2;
  unsigned class1_गणनम्;
  unsigned class2_गणनम्;

  hb_pair_t<unsigned, unsigned> compute_effective_मानम्_formats (ध्रुव hb_map_t &klass1_map,
                                                               ध्रुव hb_map_t &klass2_map,
                                                               तार्किक strip_hints, तार्किक strip_empty,
                                                               ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *varidx_delta_map = nullptr) ध्रुव
  {
    unsigned len1 = मानम्_Format1.get_len ();
    unsigned len2 = मानम्_Format2.get_len ();
    unsigned record_size = len1 + len2;

    unsigned format1 = 0;
    unsigned format2 = 0;

    अत्थाय (unsigned class1_idx : + hb_range (class1_गणनम्) | hb_filter (klass1_map))
    {
      अत्थाय (unsigned class2_idx : + hb_range (class2_गणनम्) | hb_filter (klass2_map))
      {
        unsigned idx = (class1_idx * class2_गणनम् + class2_idx) * record_size;
        format1 = format1 | मानम्_Format1.get_effective_format (&values[idx], strip_hints, strip_empty, base, varidx_delta_map);
        format2 = format2 | मानम्_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_empty, base, varidx_delta_map);
      }

      सचे (!varidx_delta_map &&
          format1 == मानम्_Format1 && format2 == मानम्_Format2)
        भङ्ग;
    }

    पटिनिवत्त hb_pair (format1, format2);
  }

  तार्किक serialize (hb_subset_context_t *c,
                  ध्रुव hb_map_t &klass1_map,
                  ध्रुव hb_map_t &klass2_map,
                  मानम्_Format newFormat1,
                  मानम्_Format newFormat2) ध्रुव
  {
    unsigned len1 = मानम्_Format1.get_len ();
    unsigned len2 = मानम्_Format2.get_len ();
    unsigned total_len = len1 + len2;
    hb_vector_t<unsigned> class2_idxs (+ hb_range (class2_गणनम्) | hb_filter (klass2_map));
    सचे (unlikely (!c->serializer->propagate_error (class2_idxs)))
      पटिनिवत्त मिच्छा;
    अत्थाय (unsigned class1_idx : + hb_range (class1_गणनम्) | hb_filter (klass1_map))
    {
      अत्थाय (unsigned class2_idx : class2_idxs)
      {
        unsigned idx = (class1_idx * class2_गणनम् + class2_idx) * total_len;
        मानम्_Format1.copy_मानानि (c->serializer, newFormat1, base, &values[idx], &c->plan->layout_variation_idx_delta_map);
        मानम्_Format2.copy_मानानि (c->serializer, newFormat2, base, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    पटिनिवत्त सच्च;
  }
};

template <typename Types>
संरचना PairPosFormat2_4 : मानम्_Base
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  मानम्_Format   मानम्_Format1;           /* ValueRecord definition--for the
                                         * first glyph of the pair--may be zero
                                         * (0) */
  मानम्_Format   मानम्_Format2;           /* ValueRecord definition--for the
                                         * second glyph of the pair--may be
                                         * zero (0) */
  typename Types::template LOffsetTo<ClassDef>
                classDef1;              /* Offset to ClassDef table--from
                                         * beginning of PairPos subtable--for
                                         * the first glyph of the pair */
  typename Types::template LOffsetTo<ClassDef>
                classDef2;              /* Offset to ClassDef table--from
                                         * beginning of PairPos subtable--for
                                         * the second glyph of the pair */
  HBUINT16      class1_गणनम्_2;            /* Number of classes in ClassDef1
                                         * table--includes Class0 */
  HBUINT16      class2_गणनम्_2;            /* Number of classes in ClassDef2
                                         * table--includes Class0 */
  मानम्_Record   values;                 /* Matrix of value pairs:
                                         * class1-major, class2-minor,
                                         * Each entry has value1 and value2 */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (10 + 3 * Types::LOffset::static_size, values);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (!(c->check_struct (एतत्)
       && coverage.sanitize (c, एतत्)
       && classDef1.sanitize (c, एतत्)
       && classDef2.sanitize (c, एतत्))) return_trace (मिच्छा);

    unsigned पूर्णाङ्क len1 = मानम्_Format1.get_len ();
    unsigned पूर्णाङ्क len2 = मानम्_Format2.get_len ();
    unsigned पूर्णाङ्क stride = HBUINT16::static_size * (len1 + len2);
    unsigned पूर्णाङ्क गणनम् = (unsigned पूर्णाङ्क) class1_गणनम्_2 * (unsigned पूर्णाङ्क) class2_गणनम्_2;
    return_trace (c->check_range ((ध्रुव शून्य *) values,
                                  गणनम्,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (मानम्_Format1.sanitize_मानानि_stride_unsafe (c, एतत्, &values[0], गणनम्, stride) &&
		    मानम्_Format2.sanitize_मानानि_stride_unsafe (c, एतत्, &values[len1], गणनम्, stride))));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    पटिनिवत्त (एतत्+coverage).intersects (glyphs) &&
           (एतत्+classDef2).intersects (glyphs);
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव {}
  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    collect_pair_variation_indices (c, एतत्, एतत्+coverage,
                                    एतत्+classDef1, एतत्+classDef2,
                                    मानम्_Format1, मानम्_Format2,
                                    class1_गणनम्_2, class2_गणनम्_2, values);
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    सचे (unlikely (!(एतत्+coverage).collect_coverage (c->निवेशः))) पटिनिवत्त;
    सचे (unlikely (!(एतत्+classDef2).collect_coverage (c->निवेशः))) पटिनिवत्त;
  }

  ध्रुव Coverage &get_coverage () ध्रुव { पटिनिवत्त एतत्+coverage; }

  संरचना external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t first;
    hb_ot_layout_mapping_cache_t second;
  };
  शून्य *external_cache_create () ध्रुव
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (परिमाण (external_cache_t));
    सचे (likely (cache))
    {
      cache->coverage.clear ();
      cache->first.clear ();
      cache->second.clear ();
    }
    पटिनिवत्त cache;
  }

  तार्किक apply (hb_ot_apply_context_t *c, शून्य *external_cache) ध्रुव
  {
    TRACE_APPLY (एतत्);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#अञ्ञथा
    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    सचे (index == NOT_COVERED) return_trace (मिच्छा);

    auto &skippy_iter = c->iter_निवेशः;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    सचे (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (मिच्छा);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    unsigned पूर्णाङ्क klass1 = (एतत्+classDef1).get_class (buffer->cur().codepoint, cache ? &cache->first : nullptr);
    unsigned पूर्णाङ्क klass2 = (एतत्+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint, cache ? &cache->second : nullptr);
#अञ्ञथा
    unsigned पूर्णाङ्क klass1 = (एतत्+classDef1).get_class (buffer->cur().codepoint);
    unsigned पूर्णाङ्क klass2 = (एतत्+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint);
#endif
    सचे (unlikely (klass1 >= class1_गणनम्_2 || klass2 >= class2_गणनम्_2))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (मिच्छा);
    }

    unsigned पूर्णाङ्क len1 = मानम्_Format1.get_len ();
    unsigned पूर्णाङ्क len2 = मानम्_Format2.get_len ();
    unsigned पूर्णाङ्क record_len = len1 + len2;

    ध्रुव मानम् *v = &values[record_len * (klass1 * class2_गणनम्_2 + klass2)];

    तार्किक applied_first = मिच्छा, applied_second = मिच्छा;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#ifndef HB_SPLIT_KERN
    सचे (मिच्छा)
#endif
    {
      सचे (!len2)
      {
        ध्रुव hb_direction_t dir = buffer->props.direction;
        ध्रुव तार्किक horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        ध्रुव तार्किक backward = HB_DIRECTION_IS_BACKWARD (dir);
        unsigned mask = horizontal ? मानम्_Format::xAdvance : मानम्_Format::yAdvance;
        सचे (backward)
          mask |= mask >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        mask |= mask << 4;

        सचे (मानम्_Format1 & ~mask)
          गच्छ bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        सचे (मानम्_Format1.apply_मानम् (c, एतत्, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &buffer->cur_pos().x_advance;
          hb_position_t *dst2 = &buffer->pos[skippy_iter.idx].x_advance;
          unsigned i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          सचे (!backward)
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
            dst2[i + 2] = hb_saturate_add (dst2[i + 2], kern2);
          }
          अञ्ञथा
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst1[i + 2] = hb_clamp_to<hb_position_t> ((int64_t) dst1[i + 2] + src[i + 2] - kern2);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
          }

          applied_first = applied_second = kern != 0;
          गच्छ success;
        }
        गच्छ boring;
      }
    }
    bail:

    सचे (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "try kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    applied_first = len1 && मानम्_Format1.apply_मानम् (c, एतत्, v, buffer->cur_pos());
    applied_second = len2 && मानम्_Format2.apply_मानम् (c, एतत्, v + len1, buffer->pos[skippy_iter.idx]);

    सचे (applied_first || applied_second)
      सचे (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "kerned glyphs at %u,%u",
			    c->buffer->idx, skippy_iter.idx);
      }

    सचे (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    success:
    सचे (applied_first || applied_second)
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    अञ्ञथा
    boring:
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);

    सचे (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    }

    buffer->idx = skippy_iter.idx;

    return_trace (सच्च);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    सचे (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छा);
    out->format = format;

    hb_map_t klass1_map;
    out->classDef1.serialize_subset (c, classDef1, एतत्, &klass1_map, सच्च, सच्च, &(एतत् + coverage));
    out->class1_गणनम्_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    out->classDef2.serialize_subset (c, classDef2, एतत्, &klass2_map, सच्च, मिच्छा);
    out->class2_गणनम्_2 = klass2_map.get_population ();

    pair_pos_class_मानानि_t class_मानानि {एतत्, values, मानम्_Format1, मानम्_Format2,
                                         class1_गणनम्_2, class2_गणनम्_2};

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (मानम्_Format1, मानम्_Format2);

    सचे (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_मानानि.compute_effective_मानम्_formats (klass1_map, klass2_map, मिच्छा, मिच्छा, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    अञ्ञथा सचे (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      तार्किक has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      तार्किक strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      सचे (has_fvar && !c->plan->has_gdef_varstore)
        strip = सच्च;
      newFormats = class_मानानि.compute_effective_मानम्_formats (klass1_map, klass2_map, strip, सच्च);
    }

    out->मानम्_Format1 = newFormats.first;
    out->मानम्_Format2 = newFormats.second;

    सचे (!class_मानानि.serialize (c, klass1_map, klass2_map,
                                 out->मानम्_Format1, out->मानम्_Format2))
      return_trace (मिच्छा);

    तार्किक ret = out->coverage.serialize_subset(c, coverage, एतत्);
    return_trace (out->class1_गणनम्_2 && out->class2_गणनम्_2 && ret);
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
