#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#अन्तर्भाव "ValueFormat.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

HB_INTERNAL ओळीत
शून्य collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     स्थिरांक मूल्य_Base *base,
                                     स्थिरांक Coverage &coverage,
                                     स्थिरांक ClassDef &class_def1,
                                     स्थिरांक ClassDef &class_def2,
                                     स्थिरांक मूल्य_Format &मूल्य_Format1,
                                     स्थिरांक मूल्य_Format &मूल्य_Format2,
                                     अचिन्हित class1_मोजणी,
                                     अचिन्हित class2_मोजणी,
                                     स्थिरांक मूल्य_Record &values)
{
  जर (!coverage.intersects (c->glyph_set) ||
      !class_def2.intersects (c->glyph_set)) परत;
  जर ((!मूल्य_Format1.has_device ()) && (!मूल्य_Format2.has_device ())) परत;

  hb_set_t klass1_glyphs, klass2_glyphs;
  जर (!class_def1.collect_coverage (&klass1_glyphs)) परत;
  जर (!class_def2.collect_coverage (&klass2_glyphs)) परत;

  hb_set_t class1_set, class2_set;
  साठी (स्थिरांक अचिन्हित cp : + c->glyph_set->iter () | hb_filter (coverage))
  {
    जर (!klass1_glyphs.has (cp)) class1_set.add (0);
    अन्यथा
    {
      अचिन्हित klass1 = class_def1.get (cp);
      class1_set.add (klass1);
    }
  }

  class2_set.add (0);
  साठी (स्थिरांक अचिन्हित cp : + c->glyph_set->iter () | hb_filter (klass2_glyphs))
  {
    अचिन्हित klass2 = class_def2.get (cp);
    class2_set.add (klass2);
  }

  जर (class1_set.is_empty ()
      || class2_set.is_empty ()
      || (class2_set.get_population() == 1 && class2_set.has(0)))
    परत;

  अचिन्हित len1 = मूल्य_Format1.get_len ();
  अचिन्हित len2 = मूल्य_Format2.get_len ();
  स्थिरांक hb_array_t<स्थिरांक मूल्य> मूल्ये_array = values.as_array ((अचिन्हित)class1_मोजणी * (अचिन्हित) class2_मोजणी * (len1 + len2));
  साठी (स्थिरांक अचिन्हित class1_idx : class1_set.iter ())
  {
    साठी (स्थिरांक अचिन्हित class2_idx : class2_set.iter ())
    {
      अचिन्हित start_offset = (class1_idx * (अचिन्हित) class2_मोजणी + class2_idx) * (len1 + len2);
      जर (मूल्य_Format1.has_device ())
        मूल्य_Format1.collect_variation_indices (c, base, मूल्ये_array.sub_array (start_offset, len1));

      जर (मूल्य_Format2.has_device ())
        मूल्य_Format2.collect_variation_indices (c, base, मूल्ये_array.sub_array (start_offset+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
संरचना pair_pos_class_मूल्ये_t
{
  स्थिरांक मूल्य_Base *base;
  स्थिरांक मूल्य_Record &values;
  स्थिरांक मूल्य_Format &मूल्य_Format1;
  स्थिरांक मूल्य_Format &मूल्य_Format2;
  अचिन्हित class1_मोजणी;
  अचिन्हित class2_मोजणी;

  hb_pair_t<अचिन्हित, अचिन्हित> compute_effective_मूल्य_formats (स्थिरांक hb_map_t &klass1_map,
                                                               स्थिरांक hb_map_t &klass2_map,
                                                               तार्किक strip_hints, तार्किक strip_empty,
                                                               स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *varidx_delta_map = nullptr) स्थिरांक
  {
    अचिन्हित len1 = मूल्य_Format1.get_len ();
    अचिन्हित len2 = मूल्य_Format2.get_len ();
    अचिन्हित record_size = len1 + len2;

    अचिन्हित format1 = 0;
    अचिन्हित format2 = 0;

    साठी (अचिन्हित class1_idx : + hb_range (class1_मोजणी) | hb_filter (klass1_map))
    {
      साठी (अचिन्हित class2_idx : + hb_range (class2_मोजणी) | hb_filter (klass2_map))
      {
        अचिन्हित idx = (class1_idx * class2_मोजणी + class2_idx) * record_size;
        format1 = format1 | मूल्य_Format1.get_effective_format (&values[idx], strip_hints, strip_empty, base, varidx_delta_map);
        format2 = format2 | मूल्य_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_empty, base, varidx_delta_map);
      }

      जर (!varidx_delta_map &&
          format1 == मूल्य_Format1 && format2 == मूल्य_Format2)
        थांबा;
    }

    परत hb_pair (format1, format2);
  }

  तार्किक serialize (hb_subset_context_t *c,
                  स्थिरांक hb_map_t &klass1_map,
                  स्थिरांक hb_map_t &klass2_map,
                  मूल्य_Format newFormat1,
                  मूल्य_Format newFormat2) स्थिरांक
  {
    अचिन्हित len1 = मूल्य_Format1.get_len ();
    अचिन्हित len2 = मूल्य_Format2.get_len ();
    अचिन्हित total_len = len1 + len2;
    hb_vector_t<अचिन्हित> class2_idxs (+ hb_range (class2_मोजणी) | hb_filter (klass2_map));
    जर (unlikely (!c->serializer->propagate_error (class2_idxs)))
      परत असत्य;
    साठी (अचिन्हित class1_idx : + hb_range (class1_मोजणी) | hb_filter (klass1_map))
    {
      साठी (अचिन्हित class2_idx : class2_idxs)
      {
        अचिन्हित idx = (class1_idx * class2_मोजणी + class2_idx) * total_len;
        मूल्य_Format1.copy_मूल्ये (c->serializer, newFormat1, base, &values[idx], &c->plan->layout_variation_idx_delta_map);
        मूल्य_Format2.copy_मूल्ये (c->serializer, newFormat2, base, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    परत सत्य;
  }
};

ढाँचा <typename Types>
संरचना PairPosFormat2_4 : मूल्य_Base
{
  संरक्षित:
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
  HBUINT16      class1_मोजणी_2;            /* Number of classes in ClassDef1
                                         * table--includes Class0 */
  HBUINT16      class2_मोजणी_2;            /* Number of classes in ClassDef2
                                         * table--includes Class0 */
  मूल्य_Record   values;                 /* Matrix of value pairs:
                                         * class1-major, class2-minor,
                                         * Each entry has value1 and value2 */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (10 + 3 * Types::LOffset::static_size, values);

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!(c->check_struct (हे)
       && coverage.sanitize (c, हे)
       && classDef1.sanitize (c, हे)
       && classDef2.sanitize (c, हे))) return_trace (असत्य);

    अचिन्हित पूर्णांक len1 = मूल्य_Format1.get_len ();
    अचिन्हित पूर्णांक len2 = मूल्य_Format2.get_len ();
    अचिन्हित पूर्णांक stride = HBUINT16::static_size * (len1 + len2);
    अचिन्हित पूर्णांक मोजणी = (अचिन्हित पूर्णांक) class1_मोजणी_2 * (अचिन्हित पूर्णांक) class2_मोजणी_2;
    return_trace (c->check_range ((स्थिरांक शून्य *) values,
                                  मोजणी,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (मूल्य_Format1.sanitize_मूल्ये_stride_unsafe (c, हे, &values[0], मोजणी, stride) &&
		    मूल्य_Format2.sanitize_मूल्ये_stride_unsafe (c, हे, &values[len1], मोजणी, stride))));
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    परत (हे+coverage).intersects (glyphs) &&
           (हे+classDef2).intersects (glyphs);
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक {}
  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक
  {
    collect_pair_variation_indices (c, हे, हे+coverage,
                                    हे+classDef1, हे+classDef2,
                                    मूल्य_Format1, मूल्य_Format2,
                                    class1_मोजणी_2, class2_मोजणी_2, values);
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  {
    जर (unlikely (!(हे+coverage).collect_coverage (c->आदान))) परत;
    जर (unlikely (!(हे+classDef2).collect_coverage (c->आदान))) परत;
  }

  स्थिरांक Coverage &get_coverage () स्थिरांक { परत हे+coverage; }

  संरचना external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t first;
    hb_ot_layout_mapping_cache_t second;
  };
  शून्य *external_cache_create () स्थिरांक
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (आकार (external_cache_t));
    जर (likely (cache))
    {
      cache->coverage.clear ();
      cache->first.clear ();
      cache->second.clear ();
    }
    परत cache;
  }

  तार्किक apply (hb_ot_apply_context_t *c, शून्य *external_cache) स्थिरांक
  {
    TRACE_APPLY (हे);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#अन्यथा
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    जर (index == NOT_COVERED) return_trace (असत्य);

    स्वतः &skippy_iter = c->iter_आदान;
    skippy_iter.reset_fast (buffer->idx);
    अचिन्हित unsafe_to;
    जर (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (असत्य);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    अचिन्हित पूर्णांक klass1 = (हे+classDef1).get_class (buffer->cur().codepoint, cache ? &cache->first : nullptr);
    अचिन्हित पूर्णांक klass2 = (हे+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint, cache ? &cache->second : nullptr);
#अन्यथा
    अचिन्हित पूर्णांक klass1 = (हे+classDef1).get_class (buffer->cur().codepoint);
    अचिन्हित पूर्णांक klass2 = (हे+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint);
#endif
    जर (unlikely (klass1 >= class1_मोजणी_2 || klass2 >= class2_मोजणी_2))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (असत्य);
    }

    अचिन्हित पूर्णांक len1 = मूल्य_Format1.get_len ();
    अचिन्हित पूर्णांक len2 = मूल्य_Format2.get_len ();
    अचिन्हित पूर्णांक record_len = len1 + len2;

    स्थिरांक मूल्य *v = &values[record_len * (klass1 * class2_मोजणी_2 + klass2)];

    तार्किक applied_first = असत्य, applied_second = असत्य;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#ifndef HB_SPLIT_KERN
    जर (असत्य)
#endif
    {
      जर (!len2)
      {
        स्थिरांक hb_direction_t dir = buffer->props.direction;
        स्थिरांक तार्किक horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        स्थिरांक तार्किक backward = HB_DIRECTION_IS_BACKWARD (dir);
        अचिन्हित mask = horizontal ? मूल्य_Format::xAdvance : मूल्य_Format::yAdvance;
        जर (backward)
          mask |= mask >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        mask |= mask << 4;

        जर (मूल्य_Format1 & ~mask)
          जा bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        जर (मूल्य_Format1.apply_मूल्य (c, हे, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &buffer->cur_pos().x_advance;
          hb_position_t *dst2 = &buffer->pos[skippy_iter.idx].x_advance;
          अचिन्हित i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          जर (!backward)
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
          जा success;
        }
        जा boring;
      }
    }
    bail:

    जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "try kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    applied_first = len1 && मूल्य_Format1.apply_मूल्य (c, हे, v, buffer->cur_pos());
    applied_second = len2 && मूल्य_Format2.apply_मूल्य (c, हे, v + len1, buffer->pos[skippy_iter.idx]);

    जर (applied_first || applied_second)
      जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "kerned glyphs at %u,%u",
			    c->buffer->idx, skippy_iter.idx);
      }

    जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    success:
    जर (applied_first || applied_second)
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    अन्यथा
    boring:
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);

    जर (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    }

    buffer->idx = skippy_iter.idx;

    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;

    hb_map_t klass1_map;
    out->classDef1.serialize_subset (c, classDef1, हे, &klass1_map, सत्य, सत्य, &(हे + coverage));
    out->class1_मोजणी_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    out->classDef2.serialize_subset (c, classDef2, हे, &klass2_map, सत्य, असत्य);
    out->class2_मोजणी_2 = klass2_map.get_population ();

    pair_pos_class_मूल्ये_t class_मूल्ये {हे, values, मूल्य_Format1, मूल्य_Format2,
                                         class1_मोजणी_2, class2_मोजणी_2};

    hb_pair_t<अचिन्हित, अचिन्हित> newFormats = hb_pair (मूल्य_Format1, मूल्य_Format2);

    जर (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_मूल्ये.compute_effective_मूल्य_formats (klass1_map, klass2_map, असत्य, असत्य, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    अन्यथा जर (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      तार्किक has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      तार्किक strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      जर (has_fvar && !c->plan->has_gdef_varstore)
        strip = सत्य;
      newFormats = class_मूल्ये.compute_effective_मूल्य_formats (klass1_map, klass2_map, strip, सत्य);
    }

    out->मूल्य_Format1 = newFormats.first;
    out->मूल्य_Format2 = newFormats.second;

    जर (!class_मूल्ये.serialize (c, klass1_map, klass2_map,
                                 out->मूल्य_Format1, out->मूल्य_Format2))
      return_trace (असत्य);

    तार्किक ret = out->coverage.serialize_subset(c, coverage, हे);
    return_trace (out->class1_मोजणी_2 && out->class2_मोजणी_2 && ret);
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
