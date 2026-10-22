#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#அந்தர்பாவ "ValueFormat.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

HB_INTERNAL உள்வரி
வெறுமை collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     மாறிலி மதிப்பு_Base *base,
                                     மாறிலி Coverage &coverage,
                                     மாறிலி ClassDef &class_def1,
                                     மாறிலி ClassDef &class_def2,
                                     மாறிலி மதிப்பு_Format &மதிப்பு_Format1,
                                     மாறிலி மதிப்பு_Format &மதிப்பு_Format2,
                                     unsigned class1_மொத்தம்,
                                     unsigned class2_மொத்தம்,
                                     மாறிலி மதிப்பு_Record &values)
{
  எனில் (!coverage.intersects (c->glyph_set) ||
      !class_def2.intersects (c->glyph_set)) திரும்பு;
  எனில் ((!மதிப்பு_Format1.has_device ()) && (!மதிப்பு_Format2.has_device ())) திரும்பு;

  hb_set_t klass1_glyphs, klass2_glyphs;
  எனில் (!class_def1.collect_coverage (&klass1_glyphs)) திரும்பு;
  எனில் (!class_def2.collect_coverage (&klass2_glyphs)) திரும்பு;

  hb_set_t class1_set, class2_set;
  க்கு (மாறிலி unsigned cp : + c->glyph_set->iter () | hb_filter (coverage))
  {
    எனில் (!klass1_glyphs.has (cp)) class1_set.add (0);
    இல்லையெனில்
    {
      unsigned klass1 = class_def1.get (cp);
      class1_set.add (klass1);
    }
  }

  class2_set.add (0);
  க்கு (மாறிலி unsigned cp : + c->glyph_set->iter () | hb_filter (klass2_glyphs))
  {
    unsigned klass2 = class_def2.get (cp);
    class2_set.add (klass2);
  }

  எனில் (class1_set.is_empty ()
      || class2_set.is_empty ()
      || (class2_set.get_population() == 1 && class2_set.has(0)))
    திரும்பு;

  unsigned len1 = மதிப்பு_Format1.get_len ();
  unsigned len2 = மதிப்பு_Format2.get_len ();
  மாறிலி hb_array_t<மாறிலி மதிப்பு> மதிப்புகள்_array = values.as_array ((unsigned)class1_மொத்தம் * (unsigned) class2_மொத்தம் * (len1 + len2));
  க்கு (மாறிலி unsigned class1_idx : class1_set.iter ())
  {
    க்கு (மாறிலி unsigned class2_idx : class2_set.iter ())
    {
      unsigned start_offset = (class1_idx * (unsigned) class2_மொத்தம் + class2_idx) * (len1 + len2);
      எனில் (மதிப்பு_Format1.has_device ())
        மதிப்பு_Format1.collect_variation_indices (c, base, மதிப்புகள்_array.sub_array (start_offset, len1));

      எனில் (மதிப்பு_Format2.has_device ())
        மதிப்பு_Format2.collect_variation_indices (c, base, மதிப்புகள்_array.sub_array (start_offset+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
கட்டமைப்பு pair_pos_class_மதிப்புகள்_t
{
  மாறிலி மதிப்பு_Base *base;
  மாறிலி மதிப்பு_Record &values;
  மாறிலி மதிப்பு_Format &மதிப்பு_Format1;
  மாறிலி மதிப்பு_Format &மதிப்பு_Format2;
  unsigned class1_மொத்தம்;
  unsigned class2_மொத்தம்;

  hb_pair_t<unsigned, unsigned> compute_effective_மதிப்பு_formats (மாறிலி hb_map_t &klass1_map,
                                                               மாறிலி hb_map_t &klass2_map,
                                                               தருக்க strip_hints, தருக்க strip_empty,
                                                               மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *varidx_delta_map = nullptr) மாறிலி
  {
    unsigned len1 = மதிப்பு_Format1.get_len ();
    unsigned len2 = மதிப்பு_Format2.get_len ();
    unsigned record_size = len1 + len2;

    unsigned format1 = 0;
    unsigned format2 = 0;

    க்கு (unsigned class1_idx : + hb_range (class1_மொத்தம்) | hb_filter (klass1_map))
    {
      க்கு (unsigned class2_idx : + hb_range (class2_மொத்தம்) | hb_filter (klass2_map))
      {
        unsigned idx = (class1_idx * class2_மொத்தம் + class2_idx) * record_size;
        format1 = format1 | மதிப்பு_Format1.get_effective_format (&values[idx], strip_hints, strip_empty, base, varidx_delta_map);
        format2 = format2 | மதிப்பு_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_empty, base, varidx_delta_map);
      }

      எனில் (!varidx_delta_map &&
          format1 == மதிப்பு_Format1 && format2 == மதிப்பு_Format2)
        நிறுத்து;
    }

    திரும்பு hb_pair (format1, format2);
  }

  தருக்க serialize (hb_subset_context_t *c,
                  மாறிலி hb_map_t &klass1_map,
                  மாறிலி hb_map_t &klass2_map,
                  மதிப்பு_Format newFormat1,
                  மதிப்பு_Format newFormat2) மாறிலி
  {
    unsigned len1 = மதிப்பு_Format1.get_len ();
    unsigned len2 = மதிப்பு_Format2.get_len ();
    unsigned total_len = len1 + len2;
    hb_vector_t<unsigned> class2_idxs (+ hb_range (class2_மொத்தம்) | hb_filter (klass2_map));
    எனில் (unlikely (!c->serializer->propagate_error (class2_idxs)))
      திரும்பு பொய்;
    க்கு (unsigned class1_idx : + hb_range (class1_மொத்தம்) | hb_filter (klass1_map))
    {
      க்கு (unsigned class2_idx : class2_idxs)
      {
        unsigned idx = (class1_idx * class2_மொத்தம் + class2_idx) * total_len;
        மதிப்பு_Format1.copy_மதிப்புகள் (c->serializer, newFormat1, base, &values[idx], &c->plan->layout_variation_idx_delta_map);
        மதிப்பு_Format2.copy_மதிப்புகள் (c->serializer, newFormat2, base, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    திரும்பு உண்மை;
  }
};

template <typename Types>
கட்டமைப்பு PairPosFormat2_4 : மதிப்பு_Base
{
  பாதுகாக்கப்பட்ட:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  மதிப்பு_Format   மதிப்பு_Format1;           /* ValueRecord definition--for the
                                         * first glyph of the pair--may be zero
                                         * (0) */
  மதிப்பு_Format   மதிப்பு_Format2;           /* ValueRecord definition--for the
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
  HBUINT16      class1_மொத்தம்_2;            /* Number of classes in ClassDef1
                                         * table--includes Class0 */
  HBUINT16      class2_மொத்தம்_2;            /* Number of classes in ClassDef2
                                         * table--includes Class0 */
  மதிப்பு_Record   values;                 /* Matrix of value pairs:
                                         * class1-major, class2-minor,
                                         * Each entry has value1 and value2 */
  பொது:
  DEFINE_SIZE_ARRAY (10 + 3 * Types::LOffset::static_size, values);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!(c->check_struct (இது)
       && coverage.sanitize (c, இது)
       && classDef1.sanitize (c, இது)
       && classDef2.sanitize (c, இது))) return_trace (பொய்);

    unsigned முழு len1 = மதிப்பு_Format1.get_len ();
    unsigned முழு len2 = மதிப்பு_Format2.get_len ();
    unsigned முழு stride = HBUINT16::static_size * (len1 + len2);
    unsigned முழு மொத்தம் = (unsigned முழு) class1_மொத்தம்_2 * (unsigned முழு) class2_மொத்தம்_2;
    return_trace (c->check_range ((மாறிலி வெறுமை *) values,
                                  மொத்தம்,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (மதிப்பு_Format1.sanitize_மதிப்புகள்_stride_unsafe (c, இது, &values[0], மொத்தம், stride) &&
		    மதிப்பு_Format2.sanitize_மதிப்புகள்_stride_unsafe (c, இது, &values[len1], மொத்தம், stride))));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    திரும்பு (இது+coverage).intersects (glyphs) &&
           (இது+classDef2).intersects (glyphs);
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி {}
  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    collect_pair_variation_indices (c, இது, இது+coverage,
                                    இது+classDef1, இது+classDef2,
                                    மதிப்பு_Format1, மதிப்பு_Format2,
                                    class1_மொத்தம்_2, class2_மொத்தம்_2, values);
  }

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    எனில் (unlikely (!(இது+coverage).collect_coverage (c->உள்ளீடு))) திரும்பு;
    எனில் (unlikely (!(இது+classDef2).collect_coverage (c->உள்ளீடு))) திரும்பு;
  }

  மாறிலி Coverage &get_coverage () மாறிலி { திரும்பு இது+coverage; }

  கட்டமைப்பு external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t first;
    hb_ot_layout_mapping_cache_t second;
  };
  வெறுமை *external_cache_create () மாறிலி
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (அளவு (external_cache_t));
    எனில் (likely (cache))
    {
      cache->coverage.clear ();
      cache->first.clear ();
      cache->second.clear ();
    }
    திரும்பு cache;
  }

  தருக்க apply (hb_ot_apply_context_t *c, வெறுமை *external_cache) மாறிலி
  {
    TRACE_APPLY (இது);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned முழு index = (இது+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#இல்லையெனில்
    unsigned முழு index = (இது+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    எனில் (index == NOT_COVERED) return_trace (பொய்);

    auto &skippy_iter = c->iter_உள்ளீடு;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    எனில் (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (பொய்);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    unsigned முழு klass1 = (இது+classDef1).get_class (buffer->cur().codepoint, cache ? &cache->first : nullptr);
    unsigned முழு klass2 = (இது+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint, cache ? &cache->second : nullptr);
#இல்லையெனில்
    unsigned முழு klass1 = (இது+classDef1).get_class (buffer->cur().codepoint);
    unsigned முழு klass2 = (இது+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint);
#endif
    எனில் (unlikely (klass1 >= class1_மொத்தம்_2 || klass2 >= class2_மொத்தம்_2))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (பொய்);
    }

    unsigned முழு len1 = மதிப்பு_Format1.get_len ();
    unsigned முழு len2 = மதிப்பு_Format2.get_len ();
    unsigned முழு record_len = len1 + len2;

    மாறிலி மதிப்பு *v = &values[record_len * (klass1 * class2_மொத்தம்_2 + klass2)];

    தருக்க applied_first = பொய், applied_second = பொய்;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#ifndef HB_SPLIT_KERN
    எனில் (பொய்)
#endif
    {
      எனில் (!len2)
      {
        மாறிலி hb_direction_t dir = buffer->props.direction;
        மாறிலி தருக்க horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        மாறிலி தருக்க backward = HB_DIRECTION_IS_BACKWARD (dir);
        unsigned mask = horizontal ? மதிப்பு_Format::xAdvance : மதிப்பு_Format::yAdvance;
        எனில் (backward)
          mask |= mask >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        mask |= mask << 4;

        எனில் (மதிப்பு_Format1 & ~mask)
          செல் bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        எனில் (மதிப்பு_Format1.apply_மதிப்பு (c, இது, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &buffer->cur_pos().x_advance;
          hb_position_t *dst2 = &buffer->pos[skippy_iter.idx].x_advance;
          unsigned i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          எனில் (!backward)
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
            dst2[i + 2] = hb_saturate_add (dst2[i + 2], kern2);
          }
          இல்லையெனில்
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst1[i + 2] = hb_clamp_to<hb_position_t> ((int64_t) dst1[i + 2] + src[i + 2] - kern2);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
          }

          applied_first = applied_second = kern != 0;
          செல் success;
        }
        செல் boring;
      }
    }
    bail:

    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "try kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    applied_first = len1 && மதிப்பு_Format1.apply_மதிப்பு (c, இது, v, buffer->cur_pos());
    applied_second = len2 && மதிப்பு_Format2.apply_மதிப்பு (c, இது, v + len1, buffer->pos[skippy_iter.idx]);

    எனில் (applied_first || applied_second)
      எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "kerned glyphs at %u,%u",
			    c->buffer->idx, skippy_iter.idx);
      }

    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    success:
    எனில் (applied_first || applied_second)
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    இல்லையெனில்
    boring:
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);

    எனில் (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    }

    buffer->idx = skippy_iter.idx;

    return_trace (உண்மை);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);
    out->format = format;

    hb_map_t klass1_map;
    out->classDef1.serialize_subset (c, classDef1, இது, &klass1_map, உண்மை, உண்மை, &(இது + coverage));
    out->class1_மொத்தம்_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    out->classDef2.serialize_subset (c, classDef2, இது, &klass2_map, உண்மை, பொய்);
    out->class2_மொத்தம்_2 = klass2_map.get_population ();

    pair_pos_class_மதிப்புகள்_t class_மதிப்புகள் {இது, values, மதிப்பு_Format1, மதிப்பு_Format2,
                                         class1_மொத்தம்_2, class2_மொத்தம்_2};

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (மதிப்பு_Format1, மதிப்பு_Format2);

    எனில் (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_மதிப்புகள்.compute_effective_மதிப்பு_formats (klass1_map, klass2_map, பொய், பொய், &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    இல்லையெனில் எனில் (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      தருக்க has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      தருக்க strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      எனில் (has_fvar && !c->plan->has_gdef_varstore)
        strip = உண்மை;
      newFormats = class_மதிப்புகள்.compute_effective_மதிப்பு_formats (klass1_map, klass2_map, strip, உண்மை);
    }

    out->மதிப்பு_Format1 = newFormats.first;
    out->மதிப்பு_Format2 = newFormats.second;

    எனில் (!class_மதிப்புகள்.serialize (c, klass1_map, klass2_map,
                                 out->மதிப்பு_Format1, out->மதிப்பு_Format2))
      return_trace (பொய்);

    தருக்க ret = out->coverage.serialize_subset(c, coverage, இது);
    return_trace (out->class1_மொத்தம்_2 && out->class2_மொத்தம்_2 && ret);
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
