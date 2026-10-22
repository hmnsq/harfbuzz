#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#ಅನ್ತರ್ಭಾವ "ValueFormat.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

HB_INTERNAL ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     ಧ್ರುವ ಮೌಲ್ಯ_Base *base,
                                     ಧ್ರುವ Coverage &coverage,
                                     ಧ್ರುವ ClassDef &class_def1,
                                     ಧ್ರುವ ClassDef &class_def2,
                                     ಧ್ರುವ ಮೌಲ್ಯ_Format &ಮೌಲ್ಯ_Format1,
                                     ಧ್ರುವ ಮೌಲ್ಯ_Format &ಮೌಲ್ಯ_Format2,
                                     unsigned class1_ಎಣಿಕೆ,
                                     unsigned class2_ಎಣಿಕೆ,
                                     ಧ್ರುವ ಮೌಲ್ಯ_Record &values)
{
  ಯದಿ (!coverage.intersects (c->glyph_set) ||
      !class_def2.intersects (c->glyph_set)) ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ ((!ಮೌಲ್ಯ_Format1.has_device ()) && (!ಮೌಲ್ಯ_Format2.has_device ())) ಪ್ರತ್ಯಾಗಮ;

  hb_set_t klass1_glyphs, klass2_glyphs;
  ಯದಿ (!class_def1.collect_coverage (&klass1_glyphs)) ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (!class_def2.collect_coverage (&klass2_glyphs)) ಪ್ರತ್ಯಾಗಮ;

  hb_set_t class1_set, class2_set;
  ಪ್ರತಿ (ಧ್ರುವ unsigned cp : + c->glyph_set->iter () | hb_filter (coverage))
  {
    ಯದಿ (!klass1_glyphs.has (cp)) class1_set.add (0);
    ಅನ್ಯಥಾ
    {
      unsigned klass1 = class_def1.get (cp);
      class1_set.add (klass1);
    }
  }

  class2_set.add (0);
  ಪ್ರತಿ (ಧ್ರುವ unsigned cp : + c->glyph_set->iter () | hb_filter (klass2_glyphs))
  {
    unsigned klass2 = class_def2.get (cp);
    class2_set.add (klass2);
  }

  ಯದಿ (class1_set.is_empty ()
      || class2_set.is_empty ()
      || (class2_set.get_population() == 1 && class2_set.has(0)))
    ಪ್ರತ್ಯಾಗಮ;

  unsigned len1 = ಮೌಲ್ಯ_Format1.get_len ();
  unsigned len2 = ಮೌಲ್ಯ_Format2.get_len ();
  ಧ್ರುವ hb_array_t<ಧ್ರುವ ಮೌಲ್ಯ> ಮೌಲ್ಯಗಳು_array = values.as_array ((unsigned)class1_ಎಣಿಕೆ * (unsigned) class2_ಎಣಿಕೆ * (len1 + len2));
  ಪ್ರತಿ (ಧ್ರುವ unsigned class1_idx : class1_set.iter ())
  {
    ಪ್ರತಿ (ಧ್ರುವ unsigned class2_idx : class2_set.iter ())
    {
      unsigned start_offset = (class1_idx * (unsigned) class2_ಎಣಿಕೆ + class2_idx) * (len1 + len2);
      ಯದಿ (ಮೌಲ್ಯ_Format1.has_device ())
        ಮೌಲ್ಯ_Format1.collect_variation_indices (c, base, ಮೌಲ್ಯಗಳು_array.sub_array (start_offset, len1));

      ಯದಿ (ಮೌಲ್ಯ_Format2.has_device ())
        ಮೌಲ್ಯ_Format2.collect_variation_indices (c, base, ಮೌಲ್ಯಗಳು_array.sub_array (start_offset+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
ಸಂರಚನಾ pair_pos_class_ಮೌಲ್ಯಗಳು_t
{
  ಧ್ರುವ ಮೌಲ್ಯ_Base *base;
  ಧ್ರುವ ಮೌಲ್ಯ_Record &values;
  ಧ್ರುವ ಮೌಲ್ಯ_Format &ಮೌಲ್ಯ_Format1;
  ಧ್ರುವ ಮೌಲ್ಯ_Format &ಮೌಲ್ಯ_Format2;
  unsigned class1_ಎಣಿಕೆ;
  unsigned class2_ಎಣಿಕೆ;

  hb_pair_t<unsigned, unsigned> compute_effective_ಮೌಲ್ಯ_formats (ಧ್ರುವ hb_map_t &klass1_map,
                                                               ಧ್ರುವ hb_map_t &klass2_map,
                                                               ತಾರ್ಕಿಕ strip_hints, ತಾರ್ಕಿಕ strip_empty,
                                                               ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *varidx_delta_map = nullptr) ಧ್ರುವ
  {
    unsigned len1 = ಮೌಲ್ಯ_Format1.get_len ();
    unsigned len2 = ಮೌಲ್ಯ_Format2.get_len ();
    unsigned record_size = len1 + len2;

    unsigned format1 = 0;
    unsigned format2 = 0;

    ಪ್ರತಿ (unsigned class1_idx : + hb_range (class1_ಎಣಿಕೆ) | hb_filter (klass1_map))
    {
      ಪ್ರತಿ (unsigned class2_idx : + hb_range (class2_ಎಣಿಕೆ) | hb_filter (klass2_map))
      {
        unsigned idx = (class1_idx * class2_ಎಣಿಕೆ + class2_idx) * record_size;
        format1 = format1 | ಮೌಲ್ಯ_Format1.get_effective_format (&values[idx], strip_hints, strip_empty, base, varidx_delta_map);
        format2 = format2 | ಮೌಲ್ಯ_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_empty, base, varidx_delta_map);
      }

      ಯದಿ (!varidx_delta_map &&
          format1 == ಮೌಲ್ಯ_Format1 && format2 == ಮೌಲ್ಯ_Format2)
        ಭಙ್ಗ;
    }

    ಪ್ರತ್ಯಾಗಮ hb_pair (format1, format2);
  }

  ತಾರ್ಕಿಕ serialize (hb_subset_context_t *c,
                  ಧ್ರುವ hb_map_t &klass1_map,
                  ಧ್ರುವ hb_map_t &klass2_map,
                  ಮೌಲ್ಯ_Format newFormat1,
                  ಮೌಲ್ಯ_Format newFormat2) ಧ್ರುವ
  {
    unsigned len1 = ಮೌಲ್ಯ_Format1.get_len ();
    unsigned len2 = ಮೌಲ್ಯ_Format2.get_len ();
    unsigned total_len = len1 + len2;
    hb_vector_t<unsigned> class2_idxs (+ hb_range (class2_ಎಣಿಕೆ) | hb_filter (klass2_map));
    ಯದಿ (unlikely (!c->serializer->propagate_error (class2_idxs)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (unsigned class1_idx : + hb_range (class1_ಎಣಿಕೆ) | hb_filter (klass1_map))
    {
      ಪ್ರತಿ (unsigned class2_idx : class2_idxs)
      {
        unsigned idx = (class1_idx * class2_ಎಣಿಕೆ + class2_idx) * total_len;
        ಮೌಲ್ಯ_Format1.copy_ಮೌಲ್ಯಗಳು (c->serializer, newFormat1, base, &values[idx], &c->plan->layout_variation_idx_delta_map);
        ಮೌಲ್ಯ_Format2.copy_ಮೌಲ್ಯಗಳು (c->serializer, newFormat2, base, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
};

template <typename Types>
ಸಂರಚನಾ PairPosFormat2_4 : ಮೌಲ್ಯ_Base
{
  ರಕ್ಷಿತ:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ಮೌಲ್ಯ_Format   ಮೌಲ್ಯ_Format1;           /* ValueRecord definition--for the
                                         * first glyph of the pair--may be zero
                                         * (0) */
  ಮೌಲ್ಯ_Format   ಮೌಲ್ಯ_Format2;           /* ValueRecord definition--for the
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
  HBUINT16      class1_ಎಣಿಕೆ_2;            /* Number of classes in ClassDef1
                                         * table--includes Class0 */
  HBUINT16      class2_ಎಣಿಕೆ_2;            /* Number of classes in ClassDef2
                                         * table--includes Class0 */
  ಮೌಲ್ಯ_Record   values;                 /* Matrix of value pairs:
                                         * class1-major, class2-minor,
                                         * Each entry has value1 and value2 */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (10 + 3 * Types::LOffset::static_size, values);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!(c->check_struct (ಏತತ್)
       && coverage.sanitize (c, ಏತತ್)
       && classDef1.sanitize (c, ಏತತ್)
       && classDef2.sanitize (c, ಏತತ್))) return_trace (ಅಸತ್ಯ);

    unsigned ಪೂರ್ಣಾಙ್ಕ len1 = ಮೌಲ್ಯ_Format1.get_len ();
    unsigned ಪೂರ್ಣಾಙ್ಕ len2 = ಮೌಲ್ಯ_Format2.get_len ();
    unsigned ಪೂರ್ಣಾಙ್ಕ stride = HBUINT16::static_size * (len1 + len2);
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = (unsigned ಪೂರ್ಣಾಙ್ಕ) class1_ಎಣಿಕೆ_2 * (unsigned ಪೂರ್ಣಾಙ್ಕ) class2_ಎಣಿಕೆ_2;
    return_trace (c->check_range ((ಧ್ರುವ ಶೂನ್ಯ *) values,
                                  ಎಣಿಕೆ,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (ಮೌಲ್ಯ_Format1.sanitize_ಮೌಲ್ಯಗಳು_stride_unsafe (c, ಏತತ್, &values[0], ಎಣಿಕೆ, stride) &&
		    ಮೌಲ್ಯ_Format2.sanitize_ಮೌಲ್ಯಗಳು_stride_unsafe (c, ಏತತ್, &values[len1], ಎಣಿಕೆ, stride))));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (ಏತತ್+coverage).intersects (glyphs) &&
           (ಏತತ್+classDef2).intersects (glyphs);
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ {}
  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    collect_pair_variation_indices (c, ಏತತ್, ಏತತ್+coverage,
                                    ಏತತ್+classDef1, ಏತತ್+classDef2,
                                    ಮೌಲ್ಯ_Format1, ಮೌಲ್ಯ_Format2,
                                    class1_ಎಣಿಕೆ_2, class2_ಎಣಿಕೆ_2, values);
  }

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!(ಏತತ್+coverage).collect_coverage (c->ಒಳಹರಿವು))) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (unlikely (!(ಏತತ್+classDef2).collect_coverage (c->ಒಳಹರಿವು))) ಪ್ರತ್ಯಾಗಮ;
  }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+coverage; }

  ಸಂರಚನಾ external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t first;
    hb_ot_layout_mapping_cache_t second;
  };
  ಶೂನ್ಯ *external_cache_create () ಧ್ರುವ
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (ಪರಿಮಾಣ (external_cache_t));
    ಯದಿ (likely (cache))
    {
      cache->coverage.clear ();
      cache->first.clear ();
      cache->second.clear ();
    }
    ಪ್ರತ್ಯಾಗಮ cache;
  }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c, ಶೂನ್ಯ *external_cache) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#ಅನ್ಯಥಾ
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    ಯದಿ (index == NOT_COVERED) return_trace (ಅಸತ್ಯ);

    auto &skippy_iter = c->iter_ಒಳಹರಿವು;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    ಯದಿ (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (ಅಸತ್ಯ);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    unsigned ಪೂರ್ಣಾಙ್ಕ klass1 = (ಏತತ್+classDef1).get_class (buffer->cur().codepoint, cache ? &cache->first : nullptr);
    unsigned ಪೂರ್ಣಾಙ್ಕ klass2 = (ಏತತ್+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint, cache ? &cache->second : nullptr);
#ಅನ್ಯಥಾ
    unsigned ಪೂರ್ಣಾಙ್ಕ klass1 = (ಏತತ್+classDef1).get_class (buffer->cur().codepoint);
    unsigned ಪೂರ್ಣಾಙ್ಕ klass2 = (ಏತತ್+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint);
#endif
    ಯದಿ (unlikely (klass1 >= class1_ಎಣಿಕೆ_2 || klass2 >= class2_ಎಣಿಕೆ_2))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (ಅಸತ್ಯ);
    }

    unsigned ಪೂರ್ಣಾಙ್ಕ len1 = ಮೌಲ್ಯ_Format1.get_len ();
    unsigned ಪೂರ್ಣಾಙ್ಕ len2 = ಮೌಲ್ಯ_Format2.get_len ();
    unsigned ಪೂರ್ಣಾಙ್ಕ record_len = len1 + len2;

    ಧ್ರುವ ಮೌಲ್ಯ *v = &values[record_len * (klass1 * class2_ಎಣಿಕೆ_2 + klass2)];

    ತಾರ್ಕಿಕ applied_first = ಅಸತ್ಯ, applied_second = ಅಸತ್ಯ;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#ifndef HB_SPLIT_KERN
    ಯದಿ (ಅಸತ್ಯ)
#endif
    {
      ಯದಿ (!len2)
      {
        ಧ್ರುವ hb_direction_t dir = buffer->props.direction;
        ಧ್ರುವ ತಾರ್ಕಿಕ horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        ಧ್ರುವ ತಾರ್ಕಿಕ backward = HB_DIRECTION_IS_BACKWARD (dir);
        unsigned mask = horizontal ? ಮೌಲ್ಯ_Format::xAdvance : ಮೌಲ್ಯ_Format::yAdvance;
        ಯದಿ (backward)
          mask |= mask >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        mask |= mask << 4;

        ಯದಿ (ಮೌಲ್ಯ_Format1 & ~mask)
          ಗಚ್ಛ bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        ಯದಿ (ಮೌಲ್ಯ_Format1.apply_ಮೌಲ್ಯ (c, ಏತತ್, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &buffer->cur_pos().x_advance;
          hb_position_t *dst2 = &buffer->pos[skippy_iter.idx].x_advance;
          unsigned i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          ಯದಿ (!backward)
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
            dst2[i + 2] = hb_saturate_add (dst2[i + 2], kern2);
          }
          ಅನ್ಯಥಾ
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst1[i + 2] = hb_clamp_to<hb_position_t> ((int64_t) dst1[i + 2] + src[i + 2] - kern2);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
          }

          applied_first = applied_second = kern != 0;
          ಗಚ್ಛ success;
        }
        ಗಚ್ಛ boring;
      }
    }
    bail:

    ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "try kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    applied_first = len1 && ಮೌಲ್ಯ_Format1.apply_ಮೌಲ್ಯ (c, ಏತತ್, v, buffer->cur_pos());
    applied_second = len2 && ಮೌಲ್ಯ_Format2.apply_ಮೌಲ್ಯ (c, ಏತತ್, v + len1, buffer->pos[skippy_iter.idx]);

    ಯದಿ (applied_first || applied_second)
      ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "kerned glyphs at %u,%u",
			    c->buffer->idx, skippy_iter.idx);
      }

    ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    success:
    ಯದಿ (applied_first || applied_second)
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    ಅನ್ಯಥಾ
    boring:
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);

    ಯದಿ (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    }

    buffer->idx = skippy_iter.idx;

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    out->format = format;

    hb_map_t klass1_map;
    out->classDef1.serialize_subset (c, classDef1, ಏತತ್, &klass1_map, ಸತ್ಯ, ಸತ್ಯ, &(ಏತತ್ + coverage));
    out->class1_ಎಣಿಕೆ_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    out->classDef2.serialize_subset (c, classDef2, ಏತತ್, &klass2_map, ಸತ್ಯ, ಅಸತ್ಯ);
    out->class2_ಎಣಿಕೆ_2 = klass2_map.get_population ();

    pair_pos_class_ಮೌಲ್ಯಗಳು_t class_ಮೌಲ್ಯಗಳು {ಏತತ್, values, ಮೌಲ್ಯ_Format1, ಮೌಲ್ಯ_Format2,
                                         class1_ಎಣಿಕೆ_2, class2_ಎಣಿಕೆ_2};

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (ಮೌಲ್ಯ_Format1, ಮೌಲ್ಯ_Format2);

    ಯದಿ (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_ಮೌಲ್ಯಗಳು.compute_effective_ಮೌಲ್ಯ_formats (klass1_map, klass2_map, ಅಸತ್ಯ, ಅಸತ್ಯ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    ಅನ್ಯಥಾ ಯದಿ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      ತಾರ್ಕಿಕ has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      ತಾರ್ಕಿಕ strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      ಯದಿ (has_fvar && !c->plan->has_gdef_varstore)
        strip = ಸತ್ಯ;
      newFormats = class_ಮೌಲ್ಯಗಳು.compute_effective_ಮೌಲ್ಯ_formats (klass1_map, klass2_map, strip, ಸತ್ಯ);
    }

    out->ಮೌಲ್ಯ_Format1 = newFormats.first;
    out->ಮೌಲ್ಯ_Format2 = newFormats.second;

    ಯದಿ (!class_ಮೌಲ್ಯಗಳು.serialize (c, klass1_map, klass2_map,
                                 out->ಮೌಲ್ಯ_Format1, out->ಮೌಲ್ಯ_Format2))
      return_trace (ಅಸತ್ಯ);

    ತಾರ್ಕಿಕ ret = out->coverage.serialize_subset(c, coverage, ಏತತ್);
    return_trace (out->class1_ಎಣಿಕೆ_2 && out->class2_ಎಣಿಕೆ_2 && ret);
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
