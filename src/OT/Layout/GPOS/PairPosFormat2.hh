#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#অন্তর্ভাৱ "ValueFormat.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

HB_INTERNAL অন্তঃস্থ
শূন্য collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     ধ্রুৱ মান_Base *base,
                                     ধ্রুৱ Coverage &coverage,
                                     ধ্রুৱ ClassDef &class_def1,
                                     ধ্রুৱ ClassDef &class_def2,
                                     ধ্রুৱ মান_Format &মান_Format1,
                                     ধ্রুৱ মান_Format &মান_Format2,
                                     unsigned class1_গুনতি,
                                     unsigned class2_গুনতি,
                                     ধ্রুৱ মান_Record &values)
{
  যদি (!coverage.intersects (c->glyph_set) ||
      !class_def2.intersects (c->glyph_set)) ঘূৰাই;
  যদি ((!মান_Format1.has_device ()) && (!মান_Format2.has_device ())) ঘূৰাই;

  hb_set_t klass1_glyphs, klass2_glyphs;
  যদি (!class_def1.collect_coverage (&klass1_glyphs)) ঘূৰাই;
  যদি (!class_def2.collect_coverage (&klass2_glyphs)) ঘূৰাই;

  hb_set_t class1_set, class2_set;
  বাবে (ধ্রুৱ unsigned cp : + c->glyph_set->iter () | hb_filter (coverage))
  {
    যদি (!klass1_glyphs.has (cp)) class1_set.add (0);
    অন্যথা
    {
      unsigned klass1 = class_def1.get (cp);
      class1_set.add (klass1);
    }
  }

  class2_set.add (0);
  বাবে (ধ্রুৱ unsigned cp : + c->glyph_set->iter () | hb_filter (klass2_glyphs))
  {
    unsigned klass2 = class_def2.get (cp);
    class2_set.add (klass2);
  }

  যদি (class1_set.is_empty ()
      || class2_set.is_empty ()
      || (class2_set.get_population() == 1 && class2_set.has(0)))
    ঘূৰাই;

  unsigned len1 = মান_Format1.get_len ();
  unsigned len2 = মান_Format2.get_len ();
  ধ্রুৱ hb_array_t<ধ্রুৱ মান> মানসমূহ_array = values.as_array ((unsigned)class1_গুনতি * (unsigned) class2_গুনতি * (len1 + len2));
  বাবে (ধ্রুৱ unsigned class1_idx : class1_set.iter ())
  {
    বাবে (ধ্রুৱ unsigned class2_idx : class2_set.iter ())
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
সংরচনা pair_pos_class_মানসমূহ_t
{
  ধ্রুৱ মান_Base *base;
  ধ্রুৱ মান_Record &values;
  ধ্রুৱ মান_Format &মান_Format1;
  ধ্রুৱ মান_Format &মান_Format2;
  unsigned class1_গুনতি;
  unsigned class2_গুনতি;

  hb_pair_t<unsigned, unsigned> compute_effective_মান_formats (ধ্রুৱ hb_map_t &klass1_map,
                                                               ধ্রুৱ hb_map_t &klass2_map,
                                                               তার্কিক strip_hints, তার্কিক strip_empty,
                                                               ধ্রুৱ hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *varidx_delta_map = nullptr) ধ্রুৱ
  {
    unsigned len1 = মান_Format1.get_len ();
    unsigned len2 = মান_Format2.get_len ();
    unsigned record_size = len1 + len2;

    unsigned format1 = 0;
    unsigned format2 = 0;

    বাবে (unsigned class1_idx : + hb_range (class1_গুনতি) | hb_filter (klass1_map))
    {
      বাবে (unsigned class2_idx : + hb_range (class2_গুনতি) | hb_filter (klass2_map))
      {
        unsigned idx = (class1_idx * class2_গুনতি + class2_idx) * record_size;
        format1 = format1 | মান_Format1.get_effective_format (&values[idx], strip_hints, strip_empty, base, varidx_delta_map);
        format2 = format2 | মান_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_empty, base, varidx_delta_map);
      }

      যদি (!varidx_delta_map &&
          format1 == মান_Format1 && format2 == মান_Format2)
        ভঙ্গ;
    }

    ঘূৰাই hb_pair (format1, format2);
  }

  তার্কিক serialize (hb_subset_context_t *c,
                  ধ্রুৱ hb_map_t &klass1_map,
                  ধ্রুৱ hb_map_t &klass2_map,
                  মান_Format newFormat1,
                  মান_Format newFormat2) ধ্রুৱ
  {
    unsigned len1 = মান_Format1.get_len ();
    unsigned len2 = মান_Format2.get_len ();
    unsigned total_len = len1 + len2;
    hb_vector_t<unsigned> class2_idxs (+ hb_range (class2_গুনতি) | hb_filter (klass2_map));
    যদি (unlikely (!c->serializer->propagate_error (class2_idxs)))
      ঘূৰাই মিছা;
    বাবে (unsigned class1_idx : + hb_range (class1_গুনতি) | hb_filter (klass1_map))
    {
      বাবে (unsigned class2_idx : class2_idxs)
      {
        unsigned idx = (class1_idx * class2_গুনতি + class2_idx) * total_len;
        মান_Format1.copy_মানসমূহ (c->serializer, newFormat1, base, &values[idx], &c->plan->layout_variation_idx_delta_map);
        মান_Format2.copy_মানসমূহ (c->serializer, newFormat2, base, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    ঘূৰাই সঁচা;
  }
};

template <typename Types>
সংরচনা PairPosFormat2_4 : মান_Base
{
  রক্ষিত:
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
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (10 + 3 * Types::LOffset::static_size, values);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (!(c->check_struct (এতত্)
       && coverage.sanitize (c, এতত্)
       && classDef1.sanitize (c, এতত্)
       && classDef2.sanitize (c, এতত্))) return_trace (মিছা);

    unsigned পূর্ণাঙ্ক len1 = মান_Format1.get_len ();
    unsigned পূর্ণাঙ্ক len2 = মান_Format2.get_len ();
    unsigned পূর্ণাঙ্ক stride = HBUINT16::static_size * (len1 + len2);
    unsigned পূর্ণাঙ্ক গুনতি = (unsigned পূর্ণাঙ্ক) class1_গুনতি_2 * (unsigned পূর্ণাঙ্ক) class2_গুনতি_2;
    return_trace (c->check_range ((ধ্রুৱ শূন্য *) values,
                                  গুনতি,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (মান_Format1.sanitize_মানসমূহ_stride_unsafe (c, এতত্, &values[0], গুনতি, stride) &&
		    মান_Format2.sanitize_মানসমূহ_stride_unsafe (c, এতত্, &values[len1], গুনতি, stride))));
  }

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  {
    ঘূৰাই (এতত্+coverage).intersects (glyphs) &&
           (এতত্+classDef2).intersects (glyphs);
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুৱ {}
  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুৱ
  {
    collect_pair_variation_indices (c, এতত্, এতত্+coverage,
                                    এতত্+classDef1, এতত্+classDef2,
                                    মান_Format1, মান_Format2,
                                    class1_গুনতি_2, class2_গুনতি_2, values);
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  {
    যদি (unlikely (!(এতত্+coverage).collect_coverage (c->প্ৰবেশ))) ঘূৰাই;
    যদি (unlikely (!(এতত্+classDef2).collect_coverage (c->প্ৰবেশ))) ঘূৰাই;
  }

  ধ্রুৱ Coverage &get_coverage () ধ্রুৱ { ঘূৰাই এতত্+coverage; }

  সংরচনা external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t first;
    hb_ot_layout_mapping_cache_t second;
  };
  শূন্য *external_cache_create () ধ্রুৱ
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (পরিমাণ (external_cache_t));
    যদি (likely (cache))
    {
      cache->coverage.clear ();
      cache->first.clear ();
      cache->second.clear ();
    }
    ঘূৰাই cache;
  }

  তার্কিক apply (hb_ot_apply_context_t *c, শূন্য *external_cache) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#অন্যথা
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    যদি (index == NOT_COVERED) return_trace (মিছা);

    auto &skippy_iter = c->iter_প্ৰবেশ;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    যদি (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (মিছা);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    unsigned পূর্ণাঙ্ক klass1 = (এতত্+classDef1).get_class (buffer->cur().codepoint, cache ? &cache->first : nullptr);
    unsigned পূর্ণাঙ্ক klass2 = (এতত্+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint, cache ? &cache->second : nullptr);
#অন্যথা
    unsigned পূর্ণাঙ্ক klass1 = (এতত্+classDef1).get_class (buffer->cur().codepoint);
    unsigned পূর্ণাঙ্ক klass2 = (এতত্+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint);
#endif
    যদি (unlikely (klass1 >= class1_গুনতি_2 || klass2 >= class2_গুনতি_2))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (মিছা);
    }

    unsigned পূর্ণাঙ্ক len1 = মান_Format1.get_len ();
    unsigned পূর্ণাঙ্ক len2 = মান_Format2.get_len ();
    unsigned পূর্ণাঙ্ক record_len = len1 + len2;

    ধ্রুৱ মান *v = &values[record_len * (klass1 * class2_গুনতি_2 + klass2)];

    তার্কিক applied_first = মিছা, applied_second = মিছা;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#ifndef HB_SPLIT_KERN
    যদি (মিছা)
#endif
    {
      যদি (!len2)
      {
        ধ্রুৱ hb_direction_t dir = buffer->props.direction;
        ধ্রুৱ তার্কিক horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        ধ্রুৱ তার্কিক backward = HB_DIRECTION_IS_BACKWARD (dir);
        unsigned mask = horizontal ? মান_Format::xAdvance : মান_Format::yAdvance;
        যদি (backward)
          mask |= mask >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        mask |= mask << 4;

        যদি (মান_Format1 & ~mask)
          গচ্ছ bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        যদি (মান_Format1.apply_মান (c, এতত্, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &buffer->cur_pos().x_advance;
          hb_position_t *dst2 = &buffer->pos[skippy_iter.idx].x_advance;
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
          গচ্ছ success;
        }
        গচ্ছ boring;
      }
    }
    bail:

    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "try kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    applied_first = len1 && মান_Format1.apply_মান (c, এতত্, v, buffer->cur_pos());
    applied_second = len2 && মান_Format2.apply_মান (c, এতত্, v + len1, buffer->pos[skippy_iter.idx]);

    যদি (applied_first || applied_second)
      যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "kerned glyphs at %u,%u",
			    c->buffer->idx, skippy_iter.idx);
      }

    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    success:
    যদি (applied_first || applied_second)
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    অন্যথা
    boring:
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);

    যদি (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    }

    buffer->idx = skippy_iter.idx;

    return_trace (সঁচা);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);
    out->format = format;

    hb_map_t klass1_map;
    out->classDef1.serialize_subset (c, classDef1, এতত্, &klass1_map, সঁচা, সঁচা, &(এতত্ + coverage));
    out->class1_গুনতি_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    out->classDef2.serialize_subset (c, classDef2, এতত্, &klass2_map, সঁচা, মিছা);
    out->class2_গুনতি_2 = klass2_map.get_population ();

    pair_pos_class_মানসমূহ_t class_মানসমূহ {এতত্, values, মান_Format1, মান_Format2,
                                         class1_গুনতি_2, class2_গুনতি_2};

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (মান_Format1, মান_Format2);

    যদি (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_মানসমূহ.compute_effective_মান_formats (klass1_map, klass2_map, মিছা, মিছা, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    অন্যথা যদি (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      তার্কিক has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      তার্কিক strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      যদি (has_fvar && !c->plan->has_gdef_varstore)
        strip = সঁচা;
      newFormats = class_মানসমূহ.compute_effective_মান_formats (klass1_map, klass2_map, strip, সঁচা);
    }

    out->মান_Format1 = newFormats.first;
    out->মান_Format2 = newFormats.second;

    যদি (!class_মানসমূহ.serialize (c, klass1_map, klass2_map,
                                 out->মান_Format1, out->মান_Format2))
      return_trace (মিছা);

    তার্কিক ret = out->coverage.serialize_subset(c, coverage, এতত্);
    return_trace (out->class1_গুনতি_2 && out->class2_গুনতি_2 && ret);
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
