#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#شامل "ValueFormat.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

HB_INTERNAL درونخطی
تهی collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     ثابت مقدار_Base *base,
                                     ثابت Coverage &coverage,
                                     ثابت ClassDef &class_def1,
                                     ثابت ClassDef &class_def2,
                                     ثابت مقدار_Format &مقدار_Format1,
                                     ثابت مقدار_Format &مقدار_Format2,
                                     unsigned class1_شمار,
                                     unsigned class2_شمار,
                                     ثابت مقدار_Record &values)
{
  اگر (!coverage.intersects (c->glyph_set) ||
      !class_def2.intersects (c->glyph_set)) بازگشت;
  اگر ((!مقدار_Format1.has_device ()) && (!مقدار_Format2.has_device ())) بازگشت;

  hb_set_t klass1_glyphs, klass2_glyphs;
  اگر (!class_def1.collect_coverage (&klass1_glyphs)) بازگشت;
  اگر (!class_def2.collect_coverage (&klass2_glyphs)) بازگشت;

  hb_set_t class1_set, class2_set;
  برای (ثابت unsigned cp : + c->glyph_set->iter () | hb_filter (coverage))
  {
    اگر (!klass1_glyphs.has (cp)) class1_set.add (0);
    وگرنه
    {
      unsigned klass1 = class_def1.get (cp);
      class1_set.add (klass1);
    }
  }

  class2_set.add (0);
  برای (ثابت unsigned cp : + c->glyph_set->iter () | hb_filter (klass2_glyphs))
  {
    unsigned klass2 = class_def2.get (cp);
    class2_set.add (klass2);
  }

  اگر (class1_set.is_empty ()
      || class2_set.is_empty ()
      || (class2_set.get_population() == 1 && class2_set.has(0)))
    بازگشت;

  unsigned len1 = مقدار_Format1.get_len ();
  unsigned len2 = مقدار_Format2.get_len ();
  ثابت hb_array_t<ثابت مقدار> مقادیر_array = values.as_array ((unsigned)class1_شمار * (unsigned) class2_شمار * (len1 + len2));
  برای (ثابت unsigned class1_idx : class1_set.iter ())
  {
    برای (ثابت unsigned class2_idx : class2_set.iter ())
    {
      unsigned start_offset = (class1_idx * (unsigned) class2_شمار + class2_idx) * (len1 + len2);
      اگر (مقدار_Format1.has_device ())
        مقدار_Format1.collect_variation_indices (c, base, مقادیر_array.sub_array (start_offset, len1));

      اگر (مقدار_Format2.has_device ())
        مقدار_Format2.collect_variation_indices (c, base, مقادیر_array.sub_array (start_offset+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
ساختار pair_pos_class_مقادیر_t
{
  ثابت مقدار_Base *base;
  ثابت مقدار_Record &values;
  ثابت مقدار_Format &مقدار_Format1;
  ثابت مقدار_Format &مقدار_Format2;
  unsigned class1_شمار;
  unsigned class2_شمار;

  hb_pair_t<unsigned, unsigned> compute_effective_مقدار_formats (ثابت hb_map_t &klass1_map,
                                                               ثابت hb_map_t &klass2_map,
                                                               منطقی strip_hints, منطقی strip_empty,
                                                               ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *varidx_delta_map = nullptr) ثابت
  {
    unsigned len1 = مقدار_Format1.get_len ();
    unsigned len2 = مقدار_Format2.get_len ();
    unsigned record_size = len1 + len2;

    unsigned format1 = 0;
    unsigned format2 = 0;

    برای (unsigned class1_idx : + hb_range (class1_شمار) | hb_filter (klass1_map))
    {
      برای (unsigned class2_idx : + hb_range (class2_شمار) | hb_filter (klass2_map))
      {
        unsigned idx = (class1_idx * class2_شمار + class2_idx) * record_size;
        format1 = format1 | مقدار_Format1.get_effective_format (&values[idx], strip_hints, strip_empty, base, varidx_delta_map);
        format2 = format2 | مقدار_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_empty, base, varidx_delta_map);
      }

      اگر (!varidx_delta_map &&
          format1 == مقدار_Format1 && format2 == مقدار_Format2)
        بشکن;
    }

    بازگشت hb_pair (format1, format2);
  }

  منطقی serialize (hb_subset_context_t *c,
                  ثابت hb_map_t &klass1_map,
                  ثابت hb_map_t &klass2_map,
                  مقدار_Format newFormat1,
                  مقدار_Format newFormat2) ثابت
  {
    unsigned len1 = مقدار_Format1.get_len ();
    unsigned len2 = مقدار_Format2.get_len ();
    unsigned total_len = len1 + len2;
    hb_vector_t<unsigned> class2_idxs (+ hb_range (class2_شمار) | hb_filter (klass2_map));
    اگر (unlikely (!c->serializer->propagate_error (class2_idxs)))
      بازگشت نادرست;
    برای (unsigned class1_idx : + hb_range (class1_شمار) | hb_filter (klass1_map))
    {
      برای (unsigned class2_idx : class2_idxs)
      {
        unsigned idx = (class1_idx * class2_شمار + class2_idx) * total_len;
        مقدار_Format1.copy_مقادیر (c->serializer, newFormat1, base, &values[idx], &c->plan->layout_variation_idx_delta_map);
        مقدار_Format2.copy_مقادیر (c->serializer, newFormat2, base, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    بازگشت درست;
  }
};

template <typename Types>
ساختار PairPosFormat2_4 : مقدار_Base
{
  محافظتشده:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  مقدار_Format   مقدار_Format1;           /* ValueRecord definition--for the
                                         * first glyph of the pair--may be zero
                                         * (0) */
  مقدار_Format   مقدار_Format2;           /* ValueRecord definition--for the
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
  HBUINT16      class1_شمار_2;            /* Number of classes in ClassDef1
                                         * table--includes Class0 */
  HBUINT16      class2_شمار_2;            /* Number of classes in ClassDef2
                                         * table--includes Class0 */
  مقدار_Record   values;                 /* Matrix of value pairs:
                                         * class1-major, class2-minor,
                                         * Each entry has value1 and value2 */
  عمومی:
  DEFINE_SIZE_ARRAY (10 + 3 * Types::LOffset::static_size, values);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!(c->check_struct (این)
       && coverage.sanitize (c, این)
       && classDef1.sanitize (c, این)
       && classDef2.sanitize (c, این))) return_trace (نادرست);

    unsigned صحیح len1 = مقدار_Format1.get_len ();
    unsigned صحیح len2 = مقدار_Format2.get_len ();
    unsigned صحیح stride = HBUINT16::static_size * (len1 + len2);
    unsigned صحیح شمار = (unsigned صحیح) class1_شمار_2 * (unsigned صحیح) class2_شمار_2;
    return_trace (c->check_range ((ثابت تهی *) values,
                                  شمار,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (مقدار_Format1.sanitize_مقادیر_stride_unsafe (c, این, &values[0], شمار, stride) &&
		    مقدار_Format2.sanitize_مقادیر_stride_unsafe (c, این, &values[len1], شمار, stride))));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    بازگشت (این+coverage).intersects (glyphs) &&
           (این+classDef2).intersects (glyphs);
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت {}
  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    collect_pair_variation_indices (c, این, این+coverage,
                                    این+classDef1, این+classDef2,
                                    مقدار_Format1, مقدار_Format2,
                                    class1_شمار_2, class2_شمار_2, values);
  }

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(این+coverage).collect_coverage (c->درون_داد))) بازگشت;
    اگر (unlikely (!(این+classDef2).collect_coverage (c->درون_داد))) بازگشت;
  }

  ثابت Coverage &get_coverage () ثابت { بازگشت این+coverage; }

  ساختار external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t first;
    hb_ot_layout_mapping_cache_t second;
  };
  تهی *external_cache_create () ثابت
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (اندازه (external_cache_t));
    اگر (likely (cache))
    {
      cache->coverage.clear ();
      cache->first.clear ();
      cache->second.clear ();
    }
    بازگشت cache;
  }

  منطقی apply (hb_ot_apply_context_t *c, تهی *external_cache) ثابت
  {
    TRACE_APPLY (این);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned صحیح index = (این+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#وگرنه
    unsigned صحیح index = (این+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    اگر (index == NOT_COVERED) return_trace (نادرست);

    auto &skippy_iter = c->iter_درون_داد;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    اگر (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (نادرست);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    unsigned صحیح klass1 = (این+classDef1).get_class (buffer->cur().codepoint, cache ? &cache->first : nullptr);
    unsigned صحیح klass2 = (این+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint, cache ? &cache->second : nullptr);
#وگرنه
    unsigned صحیح klass1 = (این+classDef1).get_class (buffer->cur().codepoint);
    unsigned صحیح klass2 = (این+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint);
#endif
    اگر (unlikely (klass1 >= class1_شمار_2 || klass2 >= class2_شمار_2))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (نادرست);
    }

    unsigned صحیح len1 = مقدار_Format1.get_len ();
    unsigned صحیح len2 = مقدار_Format2.get_len ();
    unsigned صحیح record_len = len1 + len2;

    ثابت مقدار *v = &values[record_len * (klass1 * class2_شمار_2 + klass2)];

    منطقی applied_first = نادرست, applied_second = نادرست;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#ifndef HB_SPLIT_KERN
    اگر (نادرست)
#endif
    {
      اگر (!len2)
      {
        ثابت hb_direction_t dir = buffer->props.direction;
        ثابت منطقی horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        ثابت منطقی backward = HB_DIRECTION_IS_BACKWARD (dir);
        unsigned mask = horizontal ? مقدار_Format::xAdvance : مقدار_Format::yAdvance;
        اگر (backward)
          mask |= mask >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        mask |= mask << 4;

        اگر (مقدار_Format1 & ~mask)
          برو bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        اگر (مقدار_Format1.apply_مقدار (c, این, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &buffer->cur_pos().x_advance;
          hb_position_t *dst2 = &buffer->pos[skippy_iter.idx].x_advance;
          unsigned i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          اگر (!backward)
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
            dst2[i + 2] = hb_saturate_add (dst2[i + 2], kern2);
          }
          وگرنه
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst1[i + 2] = hb_clamp_to<hb_position_t> ((int64_t) dst1[i + 2] + src[i + 2] - kern2);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
          }

          applied_first = applied_second = kern != 0;
          برو success;
        }
        برو boring;
      }
    }
    bail:

    اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "try kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    applied_first = len1 && مقدار_Format1.apply_مقدار (c, این, v, buffer->cur_pos());
    applied_second = len2 && مقدار_Format2.apply_مقدار (c, این, v + len1, buffer->pos[skippy_iter.idx]);

    اگر (applied_first || applied_second)
      اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "kerned glyphs at %u,%u",
			    c->buffer->idx, skippy_iter.idx);
      }

    اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    success:
    اگر (applied_first || applied_second)
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    وگرنه
    boring:
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);

    اگر (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    }

    buffer->idx = skippy_iter.idx;

    return_trace (درست);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);
    out->format = format;

    hb_map_t klass1_map;
    out->classDef1.serialize_subset (c, classDef1, این, &klass1_map, درست, درست, &(این + coverage));
    out->class1_شمار_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    out->classDef2.serialize_subset (c, classDef2, این, &klass2_map, درست, نادرست);
    out->class2_شمار_2 = klass2_map.get_population ();

    pair_pos_class_مقادیر_t class_مقادیر {این, values, مقدار_Format1, مقدار_Format2,
                                         class1_شمار_2, class2_شمار_2};

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (مقدار_Format1, مقدار_Format2);

    اگر (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_مقادیر.compute_effective_مقدار_formats (klass1_map, klass2_map, نادرست, نادرست, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    وگرنه اگر (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      منطقی has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      منطقی strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      اگر (has_fvar && !c->plan->has_gdef_varstore)
        strip = درست;
      newFormats = class_مقادیر.compute_effective_مقدار_formats (klass1_map, klass2_map, strip, درست);
    }

    out->مقدار_Format1 = newFormats.first;
    out->مقدار_Format2 = newFormats.second;

    اگر (!class_مقادیر.serialize (c, klass1_map, klass2_map,
                                 out->مقدار_Format1, out->مقدار_Format2))
      return_trace (نادرست);

    منطقی ret = out->coverage.serialize_subset(c, coverage, این);
    return_trace (out->class1_شمار_2 && out->class2_شمار_2 && ret);
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
