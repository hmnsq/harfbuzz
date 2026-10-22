#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#ضمن "ValueFormat.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

HB_INTERNAL مضمن
فراغ collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     ثابت ValueBase *base,
                                     ثابت Coverage &coverage,
                                     ثابت ClassDef &class_def1,
                                     ثابت ClassDef &class_def2,
                                     ثابت ValueFormat &valueFormat1,
                                     ثابت ValueFormat &valueFormat2,
                                     unsigned class1_count,
                                     unsigned class2_count,
                                     ثابت ValueRecord &values)
{
  ܐܢ (!coverage.intersects (c->glyph_set) ||
      !class_def2.intersects (c->glyph_set)) ܦܢܐ;
  ܐܢ ((!valueFormat1.has_device ()) && (!valueFormat2.has_device ())) ܦܢܐ;

  hb_set_t klass1_glyphs, klass2_glyphs;
  ܐܢ (!class_def1.collect_coverage (&klass1_glyphs)) ܦܢܐ;
  ܐܢ (!class_def2.collect_coverage (&klass2_glyphs)) ܦܢܐ;

  hb_set_t class1_set, class2_set;
  لكل (ثابت unsigned cp : + c->glyph_set->iter () | hb_filter (coverage))
  {
    ܐܢ (!klass1_glyphs.has (cp)) class1_set.add (0);
    ܐܚܪܢܐ
    {
      unsigned klass1 = class_def1.get (cp);
      class1_set.add (klass1);
    }
  }

  class2_set.add (0);
  لكل (ثابت unsigned cp : + c->glyph_set->iter () | hb_filter (klass2_glyphs))
  {
    unsigned klass2 = class_def2.get (cp);
    class2_set.add (klass2);
  }

  ܐܢ (class1_set.is_empty ()
      || class2_set.is_empty ()
      || (class2_set.get_population() == 1 && class2_set.has(0)))
    ܦܢܐ;

  unsigned len1 = valueFormat1.get_len ();
  unsigned len2 = valueFormat2.get_len ();
  ثابت hb_array_t<ثابت Value> values_array = values.as_array ((unsigned)class1_count * (unsigned) class2_count * (len1 + len2));
  لكل (ثابت unsigned class1_idx : class1_set.iter ())
  {
    لكل (ثابت unsigned class2_idx : class2_set.iter ())
    {
      unsigned start_offset = (class1_idx * (unsigned) class2_count + class2_idx) * (len1 + len2);
      ܐܢ (valueFormat1.has_device ())
        valueFormat1.collect_variation_indices (c, base, values_array.sub_array (start_offset, len1));

      ܐܢ (valueFormat2.has_device ())
        valueFormat2.collect_variation_indices (c, base, values_array.sub_array (start_offset+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
بنية pair_pos_class_values_t
{
  ثابت ValueBase *base;
  ثابت ValueRecord &values;
  ثابت ValueFormat &valueFormat1;
  ثابت ValueFormat &valueFormat2;
  unsigned class1_count;
  unsigned class2_count;

  hb_pair_t<unsigned, unsigned> compute_effective_value_formats (ثابت hb_map_t &klass1_map,
                                                               ثابت hb_map_t &klass2_map,
                                                               منطقي strip_hints, منطقي strip_empty,
                                                               ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *varidx_delta_map = nullptr) ثابت
  {
    unsigned len1 = valueFormat1.get_len ();
    unsigned len2 = valueFormat2.get_len ();
    unsigned record_size = len1 + len2;

    unsigned format1 = 0;
    unsigned format2 = 0;

    لكل (unsigned class1_idx : + hb_range (class1_count) | hb_filter (klass1_map))
    {
      لكل (unsigned class2_idx : + hb_range (class2_count) | hb_filter (klass2_map))
      {
        unsigned idx = (class1_idx * class2_count + class2_idx) * record_size;
        format1 = format1 | valueFormat1.get_effective_format (&values[idx], strip_hints, strip_empty, base, varidx_delta_map);
        format2 = format2 | valueFormat2.get_effective_format (&values[idx + len1], strip_hints, strip_empty, base, varidx_delta_map);
      }

      ܐܢ (!varidx_delta_map &&
          format1 == valueFormat1 && format2 == valueFormat2)
        اكسر;
    }

    ܦܢܐ hb_pair (format1, format2);
  }

  منطقي serialize (hb_subset_context_t *c,
                  ثابت hb_map_t &klass1_map,
                  ثابت hb_map_t &klass2_map,
                  ValueFormat newFormat1,
                  ValueFormat newFormat2) ثابت
  {
    unsigned len1 = valueFormat1.get_len ();
    unsigned len2 = valueFormat2.get_len ();
    unsigned total_len = len1 + len2;
    hb_vector_t<unsigned> class2_idxs (+ hb_range (class2_count) | hb_filter (klass2_map));
    ܐܢ (unlikely (!c->serializer->propagate_error (class2_idxs)))
      ܦܢܐ ܕܓܠܐ;
    لكل (unsigned class1_idx : + hb_range (class1_count) | hb_filter (klass1_map))
    {
      لكل (unsigned class2_idx : class2_idxs)
      {
        unsigned idx = (class1_idx * class2_count + class2_idx) * total_len;
        valueFormat1.copy_values (c->serializer, newFormat1, base, &values[idx], &c->plan->layout_variation_idx_delta_map);
        valueFormat2.copy_values (c->serializer, newFormat2, base, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    ܦܢܐ ܫܪܪܐ;
  }
};

template <typename Types>
بنية PairPosFormat2_4 : ValueBase
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ValueFormat   valueFormat1;           /* ValueRecord definition--for the
                                         * first glyph of the pair--may be zero
                                         * (0) */
  ValueFormat   valueFormat2;           /* ValueRecord definition--for the
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
  HBUINT16      class1Count;            /* Number of classes in ClassDef1
                                         * table--includes Class0 */
  HBUINT16      class2Count;            /* Number of classes in ClassDef2
                                         * table--includes Class0 */
  ValueRecord   values;                 /* Matrix of value pairs:
                                         * class1-major, class2-minor,
                                         * Each entry has value1 and value2 */
  عام:
  DEFINE_SIZE_ARRAY (10 + 3 * Types::LOffset::static_size, values);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (!(c->check_struct (هذا)
       && coverage.sanitize (c, هذا)
       && classDef1.sanitize (c, هذا)
       && classDef2.sanitize (c, هذا))) return_trace (ܕܓܠܐ);

    unsigned صحيح len1 = valueFormat1.get_len ();
    unsigned صحيح len2 = valueFormat2.get_len ();
    unsigned صحيح stride = HBUINT16::static_size * (len1 + len2);
    unsigned صحيح count = (unsigned صحيح) class1Count * (unsigned صحيح) class2Count;
    return_trace (c->check_range ((ثابت فراغ *) values,
                                  count,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (valueFormat1.sanitize_values_stride_unsafe (c, هذا, &values[0], count, stride) &&
		    valueFormat2.sanitize_values_stride_unsafe (c, هذا, &values[len1], count, stride))));
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    ܦܢܐ (هذا+coverage).intersects (glyphs) &&
           (هذا+classDef2).intersects (glyphs);
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    collect_pair_variation_indices (c, هذا, هذا+coverage,
                                    هذا+classDef1, هذا+classDef2,
                                    valueFormat1, valueFormat2,
                                    class1Count, class2Count, values);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    ܐܢ (unlikely (!(هذا+coverage).collect_coverage (c->input))) ܦܢܐ;
    ܐܢ (unlikely (!(هذا+classDef2).collect_coverage (c->input))) ܦܢܐ;
  }

  ثابت Coverage &get_coverage () ثابت { ܦܢܐ هذا+coverage; }

  بنية external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t first;
    hb_ot_layout_mapping_cache_t second;
  };
  فراغ *external_cache_create () ثابت
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (حجم (external_cache_t));
    ܐܢ (likely (cache))
    {
      cache->coverage.clear ();
      cache->first.clear ();
      cache->second.clear ();
    }
    ܦܢܐ cache;
  }

  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned صحيح index = (هذا+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#ܐܚܪܢܐ
    unsigned صحيح index = (هذا+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    ܐܢ (index == NOT_COVERED) return_trace (ܕܓܠܐ);

    auto &skippy_iter = c->iter_input;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    ܐܢ (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (ܕܓܠܐ);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    unsigned صحيح klass1 = (هذا+classDef1).get_class (buffer->cur().codepoint, cache ? &cache->first : nullptr);
    unsigned صحيح klass2 = (هذا+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint, cache ? &cache->second : nullptr);
#ܐܚܪܢܐ
    unsigned صحيح klass1 = (هذا+classDef1).get_class (buffer->cur().codepoint);
    unsigned صحيح klass2 = (هذا+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint);
#endif
    ܐܢ (unlikely (klass1 >= class1Count || klass2 >= class2Count))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (ܕܓܠܐ);
    }

    unsigned صحيح len1 = valueFormat1.get_len ();
    unsigned صحيح len2 = valueFormat2.get_len ();
    unsigned صحيح record_len = len1 + len2;

    ثابت Value *v = &values[record_len * (klass1 * class2Count + klass2)];

    منطقي applied_first = ܕܓܠܐ, applied_second = ܕܓܠܐ;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#ifndef HB_SPLIT_KERN
    ܐܢ (ܕܓܠܐ)
#endif
    {
      ܐܢ (!len2)
      {
        ثابت hb_direction_t dir = buffer->props.direction;
        ثابت منطقي horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        ثابت منطقي backward = HB_DIRECTION_IS_BACKWARD (dir);
        unsigned mask = horizontal ? ValueFormat::xAdvance : ValueFormat::yAdvance;
        ܐܢ (backward)
          mask |= mask >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        mask |= mask << 4;

        ܐܢ (valueFormat1 & ~mask)
          اذهب bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        ܐܢ (valueFormat1.apply_value (c, هذا, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &buffer->cur_pos().x_advance;
          hb_position_t *dst2 = &buffer->pos[skippy_iter.idx].x_advance;
          unsigned i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          ܐܢ (!backward)
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
            dst2[i + 2] = hb_saturate_add (dst2[i + 2], kern2);
          }
          ܐܚܪܢܐ
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst1[i + 2] = hb_clamp_to<hb_position_t> ((int64_t) dst1[i + 2] + src[i + 2] - kern2);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
          }

          applied_first = applied_second = kern != 0;
          اذهب success;
        }
        اذهب boring;
      }
    }
    bail:

    ܐܢ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "try kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    applied_first = len1 && valueFormat1.apply_value (c, هذا, v, buffer->cur_pos());
    applied_second = len2 && valueFormat2.apply_value (c, هذا, v + len1, buffer->pos[skippy_iter.idx]);

    ܐܢ (applied_first || applied_second)
      ܐܢ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "kerned glyphs at %u,%u",
			    c->buffer->idx, skippy_iter.idx);
      }

    ܐܢ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    success:
    ܐܢ (applied_first || applied_second)
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    ܐܚܪܢܐ
    boring:
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);

    ܐܢ (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    }

    buffer->idx = skippy_iter.idx;

    return_trace (ܫܪܪܐ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    ܐܢ (unlikely (!c->serializer->extend_min (out))) return_trace (ܕܓܠܐ);
    out->format = format;

    hb_map_t klass1_map;
    out->classDef1.serialize_subset (c, classDef1, هذا, &klass1_map, ܫܪܪܐ, ܫܪܪܐ, &(هذا + coverage));
    out->class1Count = klass1_map.get_population ();

    hb_map_t klass2_map;
    out->classDef2.serialize_subset (c, classDef2, هذا, &klass2_map, ܫܪܪܐ, ܕܓܠܐ);
    out->class2Count = klass2_map.get_population ();

    pair_pos_class_values_t class_values {هذا, values, valueFormat1, valueFormat2,
                                         class1Count, class2Count};

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (valueFormat1, valueFormat2);

    ܐܢ (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_values.compute_effective_value_formats (klass1_map, klass2_map, ܕܓܠܐ, ܕܓܠܐ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    ܐܚܪܢܐ ܐܢ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      منطقي has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      منطقي strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      ܐܢ (has_fvar && !c->plan->has_gdef_varstore)
        strip = ܫܪܪܐ;
      newFormats = class_values.compute_effective_value_formats (klass1_map, klass2_map, strip, ܫܪܪܐ);
    }

    out->valueFormat1 = newFormats.first;
    out->valueFormat2 = newFormats.second;

    ܐܢ (!class_values.serialize (c, klass1_map, klass2_map,
                                 out->valueFormat1, out->valueFormat2))
      return_trace (ܕܓܠܐ);

    منطقي ret = out->coverage.serialize_subset(c, coverage, هذا);
    return_trace (out->class1Count && out->class2Count && ret);
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
