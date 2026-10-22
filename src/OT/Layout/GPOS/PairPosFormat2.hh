#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#ଅନ୍ତର୍ଭାଵ "ValueFormat.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

HB_INTERNAL ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     ଅଚଳ ମୂଲ୍ୟ_Base *base,
                                     ଅଚଳ Coverage &coverage,
                                     ଅଚଳ ClassDef &class_def1,
                                     ଅଚଳ ClassDef &class_def2,
                                     ଅଚଳ ମୂଲ୍ୟ_Format &ମୂଲ୍ୟ_Format1,
                                     ଅଚଳ ମୂଲ୍ୟ_Format &ମୂଲ୍ୟ_Format2,
                                     unsigned class1_ଗଣନା,
                                     unsigned class2_ଗଣନା,
                                     ଅଚଳ ମୂଲ୍ୟ_Record &values)
{
  ଯଦି (!coverage.intersects (c->glyph_set) ||
      !class_def2.intersects (c->glyph_set)) ଫେରନ୍ତୁ;
  ଯଦି ((!ମୂଲ୍ୟ_Format1.has_device ()) && (!ମୂଲ୍ୟ_Format2.has_device ())) ଫେରନ୍ତୁ;

  hb_set_t klass1_glyphs, klass2_glyphs;
  ଯଦି (!class_def1.collect_coverage (&klass1_glyphs)) ଫେରନ୍ତୁ;
  ଯଦି (!class_def2.collect_coverage (&klass2_glyphs)) ଫେରନ୍ତୁ;

  hb_set_t class1_set, class2_set;
  ପାଇଁ (ଅଚଳ unsigned cp : + c->glyph_set->iter () | hb_filter (coverage))
  {
    ଯଦି (!klass1_glyphs.has (cp)) class1_set.add (0);
    ଅନ୍ୟଥା
    {
      unsigned klass1 = class_def1.get (cp);
      class1_set.add (klass1);
    }
  }

  class2_set.add (0);
  ପାଇଁ (ଅଚଳ unsigned cp : + c->glyph_set->iter () | hb_filter (klass2_glyphs))
  {
    unsigned klass2 = class_def2.get (cp);
    class2_set.add (klass2);
  }

  ଯଦି (class1_set.is_empty ()
      || class2_set.is_empty ()
      || (class2_set.get_population() == 1 && class2_set.has(0)))
    ଫେରନ୍ତୁ;

  unsigned len1 = ମୂଲ୍ୟ_Format1.get_len ();
  unsigned len2 = ମୂଲ୍ୟ_Format2.get_len ();
  ଅଚଳ hb_array_t<ଅଚଳ ମୂଲ୍ୟ> ମୂଲ୍ୟଗୁଡ଼ିକ_array = values.as_array ((unsigned)class1_ଗଣନା * (unsigned) class2_ଗଣନା * (len1 + len2));
  ପାଇଁ (ଅଚଳ unsigned class1_idx : class1_set.iter ())
  {
    ପାଇଁ (ଅଚଳ unsigned class2_idx : class2_set.iter ())
    {
      unsigned start_offset = (class1_idx * (unsigned) class2_ଗଣନା + class2_idx) * (len1 + len2);
      ଯଦି (ମୂଲ୍ୟ_Format1.has_device ())
        ମୂଲ୍ୟ_Format1.collect_variation_indices (c, base, ମୂଲ୍ୟଗୁଡ଼ିକ_array.sub_array (start_offset, len1));

      ଯଦି (ମୂଲ୍ୟ_Format2.has_device ())
        ମୂଲ୍ୟ_Format2.collect_variation_indices (c, base, ମୂଲ୍ୟଗୁଡ଼ିକ_array.sub_array (start_offset+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
ସଂରଚନା pair_pos_class_ମୂଲ୍ୟଗୁଡ଼ିକ_t
{
  ଅଚଳ ମୂଲ୍ୟ_Base *base;
  ଅଚଳ ମୂଲ୍ୟ_Record &values;
  ଅଚଳ ମୂଲ୍ୟ_Format &ମୂଲ୍ୟ_Format1;
  ଅଚଳ ମୂଲ୍ୟ_Format &ମୂଲ୍ୟ_Format2;
  unsigned class1_ଗଣନା;
  unsigned class2_ଗଣନା;

  hb_pair_t<unsigned, unsigned> compute_effective_ମୂଲ୍ୟ_formats (ଅଚଳ hb_map_t &klass1_map,
                                                               ଅଚଳ hb_map_t &klass2_map,
                                                               ତାର୍କିକ strip_hints, ତାର୍କିକ strip_empty,
                                                               ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *varidx_delta_map = nullptr) ଅଚଳ
  {
    unsigned len1 = ମୂଲ୍ୟ_Format1.get_len ();
    unsigned len2 = ମୂଲ୍ୟ_Format2.get_len ();
    unsigned record_size = len1 + len2;

    unsigned format1 = 0;
    unsigned format2 = 0;

    ପାଇଁ (unsigned class1_idx : + hb_range (class1_ଗଣନା) | hb_filter (klass1_map))
    {
      ପାଇଁ (unsigned class2_idx : + hb_range (class2_ଗଣନା) | hb_filter (klass2_map))
      {
        unsigned idx = (class1_idx * class2_ଗଣନା + class2_idx) * record_size;
        format1 = format1 | ମୂଲ୍ୟ_Format1.get_effective_format (&values[idx], strip_hints, strip_empty, base, varidx_delta_map);
        format2 = format2 | ମୂଲ୍ୟ_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_empty, base, varidx_delta_map);
      }

      ଯଦି (!varidx_delta_map &&
          format1 == ମୂଲ୍ୟ_Format1 && format2 == ମୂଲ୍ୟ_Format2)
        ଭାଙ୍ଗ;
    }

    ଫେରନ୍ତୁ hb_pair (format1, format2);
  }

  ତାର୍କିକ serialize (hb_subset_context_t *c,
                  ଅଚଳ hb_map_t &klass1_map,
                  ଅଚଳ hb_map_t &klass2_map,
                  ମୂଲ୍ୟ_Format newFormat1,
                  ମୂଲ୍ୟ_Format newFormat2) ଅଚଳ
  {
    unsigned len1 = ମୂଲ୍ୟ_Format1.get_len ();
    unsigned len2 = ମୂଲ୍ୟ_Format2.get_len ();
    unsigned total_len = len1 + len2;
    hb_vector_t<unsigned> class2_idxs (+ hb_range (class2_ଗଣନା) | hb_filter (klass2_map));
    ଯଦି (unlikely (!c->serializer->propagate_error (class2_idxs)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (unsigned class1_idx : + hb_range (class1_ଗଣନା) | hb_filter (klass1_map))
    {
      ପାଇଁ (unsigned class2_idx : class2_idxs)
      {
        unsigned idx = (class1_idx * class2_ଗଣନା + class2_idx) * total_len;
        ମୂଲ୍ୟ_Format1.copy_ମୂଲ୍ୟଗୁଡ଼ିକ (c->serializer, newFormat1, base, &values[idx], &c->plan->layout_variation_idx_delta_map);
        ମୂଲ୍ୟ_Format2.copy_ମୂଲ୍ୟଗୁଡ଼ିକ (c->serializer, newFormat2, base, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
};

template <typename Types>
ସଂରଚନା PairPosFormat2_4 : ମୂଲ୍ୟ_Base
{
  ସୁରକ୍ଷିତ:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ମୂଲ୍ୟ_Format   ମୂଲ୍ୟ_Format1;           /* ValueRecord definition--for the
                                         * first glyph of the pair--may be zero
                                         * (0) */
  ମୂଲ୍ୟ_Format   ମୂଲ୍ୟ_Format2;           /* ValueRecord definition--for the
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
  HBUINT16      class1_ଗଣନା_2;            /* Number of classes in ClassDef1
                                         * table--includes Class0 */
  HBUINT16      class2_ଗଣନା_2;            /* Number of classes in ClassDef2
                                         * table--includes Class0 */
  ମୂଲ୍ୟ_Record   values;                 /* Matrix of value pairs:
                                         * class1-major, class2-minor,
                                         * Each entry has value1 and value2 */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (10 + 3 * Types::LOffset::static_size, values);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!(c->check_struct (ଏହି)
       && coverage.sanitize (c, ଏହି)
       && classDef1.sanitize (c, ଏହି)
       && classDef2.sanitize (c, ଏହି))) return_trace (ମିଥ୍ୟା);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len1 = ମୂଲ୍ୟ_Format1.get_len ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len2 = ମୂଲ୍ୟ_Format2.get_len ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride = HBUINT16::static_size * (len1 + len2);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) class1_ଗଣନା_2 * (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) class2_ଗଣନା_2;
    return_trace (c->check_range ((ଅଚଳ ଶୂନ୍ୟ *) values,
                                  ଗଣନା_2,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (ମୂଲ୍ୟ_Format1.sanitize_ମୂଲ୍ୟଗୁଡ଼ିକ_stride_unsafe (c, ଏହି, &values[0], ଗଣନା_2, stride) &&
		    ମୂଲ୍ୟ_Format2.sanitize_ମୂଲ୍ୟଗୁଡ଼ିକ_stride_unsafe (c, ଏହି, &values[len1], ଗଣନା_2, stride))));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ଫେରନ୍ତୁ (ଏହି+coverage).intersects (glyphs) &&
           (ଏହି+classDef2).intersects (glyphs);
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ {}
  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    collect_pair_variation_indices (c, ଏହି, ଏହି+coverage,
                                    ଏହି+classDef1, ଏହି+classDef2,
                                    ମୂଲ୍ୟ_Format1, ମୂଲ୍ୟ_Format2,
                                    class1_ଗଣନା_2, class2_ଗଣନା_2, values);
  }

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    ଯଦି (unlikely (!(ଏହି+coverage).collect_coverage (c->ନିବେଶ))) ଫେରନ୍ତୁ;
    ଯଦି (unlikely (!(ଏହି+classDef2).collect_coverage (c->ନିବେଶ))) ଫେରନ୍ତୁ;
  }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+coverage; }

  ସଂରଚନା external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t first;
    hb_ot_layout_mapping_cache_t second;
  };
  ଶୂନ୍ୟ *external_cache_create () ଅଚଳ
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (ଆକାର (external_cache_t));
    ଯଦି (likely (cache))
    {
      cache->coverage.clear ();
      cache->first.clear ();
      cache->second.clear ();
    }
    ଫେରନ୍ତୁ cache;
  }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c, ଶୂନ୍ୟ *external_cache) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#ଅନ୍ୟଥା
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    ଯଦି (index == NOT_COVERED) return_trace (ମିଥ୍ୟା);

    auto &skippy_iter = c->iter_ନିବେଶ;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    ଯଦି (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (ମିଥ୍ୟା);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା klass1 = (ଏହି+classDef1).get_class (buffer->cur().codepoint, cache ? &cache->first : nullptr);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା klass2 = (ଏହି+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint, cache ? &cache->second : nullptr);
#ଅନ୍ୟଥା
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା klass1 = (ଏହି+classDef1).get_class (buffer->cur().codepoint);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା klass2 = (ଏହି+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint);
#endif
    ଯଦି (unlikely (klass1 >= class1_ଗଣନା_2 || klass2 >= class2_ଗଣନା_2))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (ମିଥ୍ୟା);
    }

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len1 = ମୂଲ୍ୟ_Format1.get_len ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len2 = ମୂଲ୍ୟ_Format2.get_len ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା record_len = len1 + len2;

    ଅଚଳ ମୂଲ୍ୟ *v = &values[record_len * (klass1 * class2_ଗଣନା_2 + klass2)];

    ତାର୍କିକ applied_first = ମିଥ୍ୟା, applied_second = ମିଥ୍ୟା;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#ifndef HB_SPLIT_KERN
    ଯଦି (ମିଥ୍ୟା)
#endif
    {
      ଯଦି (!len2)
      {
        ଅଚଳ hb_direction_t dir = buffer->props.direction;
        ଅଚଳ ତାର୍କିକ horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        ଅଚଳ ତାର୍କିକ backward = HB_DIRECTION_IS_BACKWARD (dir);
        unsigned mask = horizontal ? ମୂଲ୍ୟ_Format::xAdvance : ମୂଲ୍ୟ_Format::yAdvance;
        ଯଦି (backward)
          mask |= mask >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        mask |= mask << 4;

        ଯଦି (ମୂଲ୍ୟ_Format1 & ~mask)
          ଯାଅ bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        ଯଦି (ମୂଲ୍ୟ_Format1.apply_ମୂଲ୍ୟ (c, ଏହି, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &buffer->cur_pos().x_advance;
          hb_position_t *dst2 = &buffer->pos[skippy_iter.idx].x_advance;
          unsigned i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          ଯଦି (!backward)
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
            dst2[i + 2] = hb_saturate_add (dst2[i + 2], kern2);
          }
          ଅନ୍ୟଥା
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst1[i + 2] = hb_clamp_to<hb_position_t> ((int64_t) dst1[i + 2] + src[i + 2] - kern2);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
          }

          applied_first = applied_second = kern != 0;
          ଯାଅ success;
        }
        ଯାଅ boring;
      }
    }
    bail:

    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "try kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    applied_first = len1 && ମୂଲ୍ୟ_Format1.apply_ମୂଲ୍ୟ (c, ଏହି, v, buffer->cur_pos());
    applied_second = len2 && ମୂଲ୍ୟ_Format2.apply_ମୂଲ୍ୟ (c, ଏହି, v + len1, buffer->pos[skippy_iter.idx]);

    ଯଦି (applied_first || applied_second)
      ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "kerned glyphs at %u,%u",
			    c->buffer->idx, skippy_iter.idx);
      }

    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    success:
    ଯଦି (applied_first || applied_second)
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    ଅନ୍ୟଥା
    boring:
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);

    ଯଦି (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    }

    buffer->idx = skippy_iter.idx;

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);
    out->format = format;

    hb_map_t klass1_map;
    out->classDef1.serialize_subset (c, classDef1, ଏହି, &klass1_map, ସତ୍ୟ, ସତ୍ୟ, &(ଏହି + coverage));
    out->class1_ଗଣନା_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    out->classDef2.serialize_subset (c, classDef2, ଏହି, &klass2_map, ସତ୍ୟ, ମିଥ୍ୟା);
    out->class2_ଗଣନା_2 = klass2_map.get_population ();

    pair_pos_class_ମୂଲ୍ୟଗୁଡ଼ିକ_t class_ମୂଲ୍ୟଗୁଡ଼ିକ {ଏହି, values, ମୂଲ୍ୟ_Format1, ମୂଲ୍ୟ_Format2,
                                         class1_ଗଣନା_2, class2_ଗଣନା_2};

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (ମୂଲ୍ୟ_Format1, ମୂଲ୍ୟ_Format2);

    ଯଦି (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_ମୂଲ୍ୟଗୁଡ଼ିକ.compute_effective_ମୂଲ୍ୟ_formats (klass1_map, klass2_map, ମିଥ୍ୟା, ମିଥ୍ୟା, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    ଅନ୍ୟଥା ଯଦି (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      ତାର୍କିକ has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      ତାର୍କିକ strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      ଯଦି (has_fvar && !c->plan->has_gdef_varstore)
        strip = ସତ୍ୟ;
      newFormats = class_ମୂଲ୍ୟଗୁଡ଼ିକ.compute_effective_ମୂଲ୍ୟ_formats (klass1_map, klass2_map, strip, ସତ୍ୟ);
    }

    out->ମୂଲ୍ୟ_Format1 = newFormats.first;
    out->ମୂଲ୍ୟ_Format2 = newFormats.second;

    ଯଦି (!class_ମୂଲ୍ୟଗୁଡ଼ିକ.serialize (c, klass1_map, klass2_map,
                                 out->ମୂଲ୍ୟ_Format1, out->ମୂଲ୍ୟ_Format2))
      return_trace (ମିଥ୍ୟା);

    ତାର୍କିକ ret = out->coverage.serialize_subset(c, coverage, ଏହି);
    return_trace (out->class1_ଗଣନା_2 && out->class2_ଗଣନା_2 && ret);
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
