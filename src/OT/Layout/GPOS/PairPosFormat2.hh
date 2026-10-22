#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#অন্তর্ভাব "ValueFormat.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

HB_INTERNAL অন্তঃরেখ
শূন্য collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     ধ্রুবক মান_Base *base,
                                     ধ্রুবক Coverage &coverage,
                                     ধ্রুবক ClassDef &class_def1,
                                     ধ্রুবক ClassDef &class_def2,
                                     ধ্রুবক মান_Format &মান_Format1,
                                     ধ্রুবক মান_Format &মান_Format2,
                                     unsigned class1_গুনতি,
                                     unsigned class2_গুনতি,
                                     ধ্রুবক মান_Record &values)
{
  যদি (!coverage.intersects (c->glyph_set) ||
      !class_def2.intersects (c->glyph_set)) ফেরত;
  যদি ((!মান_Format1.has_device ()) && (!মান_Format2.has_device ())) ফেরত;

  hb_set_t klass1_glyphs, klass2_glyphs;
  যদি (!class_def1.collect_coverage (&klass1_glyphs)) ফেরত;
  যদি (!class_def2.collect_coverage (&klass2_glyphs)) ফেরত;

  hb_set_t class1_set, class2_set;
  জন্য (ধ্রুবক unsigned cp : + c->glyph_set->iter () | hb_filter (coverage))
  {
    যদি (!klass1_glyphs.has (cp)) class1_set.add (0);
    অন্যথা
    {
      unsigned klass1 = class_def1.get (cp);
      class1_set.add (klass1);
    }
  }

  class2_set.add (0);
  জন্য (ধ্রুবক unsigned cp : + c->glyph_set->iter () | hb_filter (klass2_glyphs))
  {
    unsigned klass2 = class_def2.get (cp);
    class2_set.add (klass2);
  }

  যদি (class1_set.is_empty ()
      || class2_set.is_empty ()
      || (class2_set.get_population() == 1 && class2_set.has(0)))
    ফেরত;

  unsigned len1 = মান_Format1.get_len ();
  unsigned len2 = মান_Format2.get_len ();
  ধ্রুবক hb_array_t<ধ্রুবক মান> মানসমূহ_array = values.as_array ((unsigned)class1_গুনতি * (unsigned) class2_গুনতি * (len1 + len2));
  জন্য (ধ্রুবক unsigned class1_idx : class1_set.iter ())
  {
    জন্য (ধ্রুবক unsigned class2_idx : class2_set.iter ())
    {
      unsigned start_offset = (class1_idx * (unsigned) class2_গুনতি + class2_idx) * (len1 + len2);
      যদি (মান_Format1.has_device ())
        মান_Format1.collect_variation_indices (c, base, মানসমূহ_array.sub_array (start_offset, len1));

      যদি (মান_Format2.has_device ())
        মান_Format2.collect_variation_indices (c, base, মানসমূহ_array.sub_array (start_offset+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
কাঠামো pair_pos_class_মানসমূহ_t
{
  ধ্রুবক মান_Base *base;
  ধ্রুবক মান_Record &values;
  ধ্রুবক মান_Format &মান_Format1;
  ধ্রুবক মান_Format &মান_Format2;
  unsigned class1_গুনতি;
  unsigned class2_গুনতি;

  hb_pair_t<unsigned, unsigned> compute_effective_মান_formats (ধ্রুবক hb_map_t &klass1_map,
                                                               ধ্রুবক hb_map_t &klass2_map,
                                                               যৌক্তিক strip_hints, যৌক্তিক strip_empty,
                                                               ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *varidx_delta_map = nullptr) ধ্রুবক
  {
    unsigned len1 = মান_Format1.get_len ();
    unsigned len2 = মান_Format2.get_len ();
    unsigned record_আকার = len1 + len2;

    unsigned format1 = 0;
    unsigned format2 = 0;

    জন্য (unsigned class1_idx : + hb_range (class1_গুনতি) | hb_filter (klass1_map))
    {
      জন্য (unsigned class2_idx : + hb_range (class2_গুনতি) | hb_filter (klass2_map))
      {
        unsigned idx = (class1_idx * class2_গুনতি + class2_idx) * record_আকার;
        format1 = format1 | মান_Format1.get_effective_format (&values[idx], strip_hints, strip_empty, base, varidx_delta_map);
        format2 = format2 | মান_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_empty, base, varidx_delta_map);
      }

      যদি (!varidx_delta_map &&
          format1 == মান_Format1 && format2 == মান_Format2)
        বিরতি;
    }

    ফেরত hb_pair (format1, format2);
  }

  যৌক্তিক serialize (hb_subset_context_t *c,
                  ধ্রুবক hb_map_t &klass1_map,
                  ধ্রুবক hb_map_t &klass2_map,
                  মান_Format newFormat1,
                  মান_Format newFormat2) ধ্রুবক
  {
    unsigned len1 = মান_Format1.get_len ();
    unsigned len2 = মান_Format2.get_len ();
    unsigned যোগ_len = len1 + len2;
    hb_vector_t<unsigned> class2_idxs (+ hb_range (class2_গুনতি) | hb_filter (klass2_map));
    যদি (unlikely (!c->serializer->propagate_error (class2_idxs)))
      ফেরত মিথ্যা;
    জন্য (unsigned class1_idx : + hb_range (class1_গুনতি) | hb_filter (klass1_map))
    {
      জন্য (unsigned class2_idx : class2_idxs)
      {
        unsigned idx = (class1_idx * class2_গুনতি + class2_idx) * যোগ_len;
        মান_Format1.copy_মানসমূহ (c->serializer, newFormat1, base, &values[idx], &c->plan->layout_variation_idx_delta_map);
        মান_Format2.copy_মানসমূহ (c->serializer, newFormat2, base, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    ফেরত সত্য;
  }
};

template <typename Types>
কাঠামো PairPosFormat2_4 : মান_Base
{
  সুরক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  মান_Format   মান_Format1;           /* ValueRecord definition--for the
                                         * first glyph of the pair--may be zero
                                         * (0) */
  মান_Format   মান_Format2;           /* ValueRecord definition--for the
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
  HBUINT16      class1_গুনতি_2;            /* Number of classes in ClassDef1
                                         * table--includes Class0 */
  HBUINT16      class2_গুনতি_2;            /* Number of classes in ClassDef2
                                         * table--includes Class0 */
  মান_Record   values;                 /* Matrix of value pairs:
                                         * class1-major, class2-minor,
                                         * Each entry has value1 and value2 */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (10 + 3 * Types::LOffset::static_আকার, values);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!(c->check_struct (এই)
       && coverage.sanitize (c, এই)
       && classDef1.sanitize (c, এই)
       && classDef2.sanitize (c, এই))) return_trace (মিথ্যা);

    unsigned পূর্ণসংখ্যা len1 = মান_Format1.get_len ();
    unsigned পূর্ণসংখ্যা len2 = মান_Format2.get_len ();
    unsigned পূর্ণসংখ্যা stride = HBUINT16::static_আকার * (len1 + len2);
    unsigned পূর্ণসংখ্যা গুনতি = (unsigned পূর্ণসংখ্যা) class1_গুনতি_2 * (unsigned পূর্ণসংখ্যা) class2_গুনতি_2;
    return_trace (c->check_range ((ধ্রুবক শূন্য *) values,
                                  গুনতি,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (মান_Format1.sanitize_মানসমূহ_stride_unsafe (c, এই, &values[0], গুনতি, stride) &&
		    মান_Format2.sanitize_মানসমূহ_stride_unsafe (c, এই, &values[len1], গুনতি, stride))));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    ফেরত (এই+coverage).intersects (glyphs) &&
           (এই+classDef2).intersects (glyphs);
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক {}
  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    collect_pair_variation_indices (c, এই, এই+coverage,
                                    এই+classDef1, এই+classDef2,
                                    মান_Format1, মান_Format2,
                                    class1_গুনতি_2, class2_গুনতি_2, values);
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    যদি (unlikely (!(এই+coverage).collect_coverage (c->প্রবেশ))) ফেরত;
    যদি (unlikely (!(এই+classDef2).collect_coverage (c->প্রবেশ))) ফেরত;
  }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক { ফেরত এই+coverage; }

  কাঠামো external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t first;
    hb_ot_layout_mapping_cache_t second;
  };
  শূন্য *external_cache_create () ধ্রুবক
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (আকার (external_cache_t));
    যদি (likely (cache))
    {
      cache->coverage.clear ();
      cache->first.clear ();
      cache->second.clear ();
    }
    ফেরত cache;
  }

  যৌক্তিক apply (hb_ot_apply_context_t *c, শূন্য *external_cache) ধ্রুবক
  {
    TRACE_APPLY (এই);

    hb_বাফার_t *বাফার = c->বাফার;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage  (বাফার->cur().codepoint, cache ? &cache->coverage : nullptr);
#অন্যথা
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage  (বাফার->cur().codepoint);
#endif
    যদি (সূচক_2 == NOT_COVERED) return_trace (মিথ্যা);

    auto &skippy_iter = c->iter_প্রবেশ;
    skippy_iter.reset_fast (বাফার->idx);
    unsigned unsafe_to;
    যদি (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      বাফার->unsafe_to_concat (বাফার->idx, unsafe_to);
      return_trace (মিথ্যা);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    unsigned পূর্ণসংখ্যা klass1 = (এই+classDef1).get_class (বাফার->cur().codepoint, cache ? &cache->first : nullptr);
    unsigned পূর্ণসংখ্যা klass2 = (এই+classDef2).get_class (বাফার->info[skippy_iter.idx].codepoint, cache ? &cache->second : nullptr);
#অন্যথা
    unsigned পূর্ণসংখ্যা klass1 = (এই+classDef1).get_class (বাফার->cur().codepoint);
    unsigned পূর্ণসংখ্যা klass2 = (এই+classDef2).get_class (বাফার->info[skippy_iter.idx].codepoint);
#endif
    যদি (unlikely (klass1 >= class1_গুনতি_2 || klass2 >= class2_গুনতি_2))
    {
      বাফার->unsafe_to_concat (বাফার->idx, skippy_iter.idx + 1);
      return_trace (মিথ্যা);
    }

    unsigned পূর্ণসংখ্যা len1 = মান_Format1.get_len ();
    unsigned পূর্ণসংখ্যা len2 = মান_Format2.get_len ();
    unsigned পূর্ণসংখ্যা record_len = len1 + len2;

    ধ্রুবক মান *v = &values[record_len * (klass1 * class2_গুনতি_2 + klass2)];

    যৌক্তিক applied_first = মিথ্যা, applied_second = মিথ্যা;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#ifndef HB_SPLIT_KERN
    যদি (মিথ্যা)
#endif
    {
      যদি (!len2)
      {
        ধ্রুবক hb_direction_t dir = বাফার->props.direction;
        ধ্রুবক যৌক্তিক horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        ধ্রুবক যৌক্তিক backward = HB_DIRECTION_IS_BACKWARD (dir);
        unsigned mask = horizontal ? মান_Format::xAdvance : মান_Format::yAdvance;
        যদি (backward)
          mask |= mask >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        mask |= mask << 4;

        যদি (মান_Format1 & ~mask)
          যাও bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        যদি (মান_Format1.apply_মান (c, এই, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &বাফার->cur_pos().x_advance;
          hb_position_t *dst2 = &বাফার->pos[skippy_iter.idx].x_advance;
          unsigned i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          যদি (!backward)
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
            dst2[i + 2] = hb_saturate_add (dst2[i + 2], kern2);
          }
          অন্যথা
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst1[i + 2] = hb_clamp_to<hb_position_t> ((int64_t) dst1[i + 2] + src[i + 2] - kern2);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
          }

          applied_first = applied_second = kern != 0;
          যাও success;
        }
        যাও boring;
      }
    }
    bail:

    যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
    {
      c->বাফার->message (c->font,
			  "try kerning glyphs at %u,%u",
			  c->বাফার->idx, skippy_iter.idx);
    }

    applied_first = len1 && মান_Format1.apply_মান (c, এই, v, বাফার->cur_pos());
    applied_second = len2 && মান_Format2.apply_মান (c, এই, v + len1, বাফার->pos[skippy_iter.idx]);

    যদি (applied_first || applied_second)
      যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
      {
	c->বাফার->message (c->font,
			    "kerned glyphs at %u,%u",
			    c->বাফার->idx, skippy_iter.idx);
      }

    যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
    {
      c->বাফার->message (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->বাফার->idx, skippy_iter.idx);
    }

    success:
    যদি (applied_first || applied_second)
      বাফার->unsafe_to_break (বাফার->idx, skippy_iter.idx + 1);
    অন্যথা
    boring:
      বাফার->unsafe_to_concat (বাফার->idx, skippy_iter.idx + 1);

    যদি (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      বাফার->unsafe_to_break (বাফার->idx, skippy_iter.idx + 1);
    }

    বাফার->idx = skippy_iter.idx;

    return_trace (সত্য);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);
    out->format = format;

    hb_map_t klass1_map;
    out->classDef1.serialize_subset (c, classDef1, এই, &klass1_map, সত্য, সত্য, &(এই + coverage));
    out->class1_গুনতি_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    out->classDef2.serialize_subset (c, classDef2, এই, &klass2_map, সত্য, মিথ্যা);
    out->class2_গুনতি_2 = klass2_map.get_population ();

    pair_pos_class_মানসমূহ_t class_মানসমূহ {এই, values, মান_Format1, মান_Format2,
                                         class1_গুনতি_2, class2_গুনতি_2};

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (মান_Format1, মান_Format2);

    যদি (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_মানসমূহ.compute_effective_মান_formats (klass1_map, klass2_map, মিথ্যা, মিথ্যা, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    অন্যথা যদি (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      যৌক্তিক has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      যৌক্তিক strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      যদি (has_fvar && !c->plan->has_gdef_varstore)
        strip = সত্য;
      newFormats = class_মানসমূহ.compute_effective_মান_formats (klass1_map, klass2_map, strip, সত্য);
    }

    out->মান_Format1 = newFormats.first;
    out->মান_Format2 = newFormats.second;

    যদি (!class_মানসমূহ.serialize (c, klass1_map, klass2_map,
                                 out->মান_Format1, out->মান_Format2))
      return_trace (মিথ্যা);

    যৌক্তিক ret = out->coverage.serialize_subset(c, coverage, এই);
    return_trace (out->class1_গুনতি_2 && out->class2_গুনতি_2 && ret);
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
