#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#अन्तर्भाव "ValueFormat.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

HB_INTERNAL अन्तःस्थ
शून्य collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     ध्रुव मूल्य_Base *base,
                                     ध्रुव Coverage &coverage,
                                     ध्रुव ClassDef &class_def1,
                                     ध्रुव ClassDef &class_def2,
                                     ध्रुव मूल्य_Format &मूल्य_Format1,
                                     ध्रुव मूल्य_Format &मूल्य_Format2,
                                     अचिन्हित class1_गिनती,
                                     अचिन्हित class2_गिनती,
                                     ध्रुव मूल्य_Record &values)
{
  यदि (!coverage.intersects (c->glyph_set) ||
      !class_def2.intersects (c->glyph_set)) प्रत्यागम;
  यदि ((!मूल्य_Format1.has_device ()) && (!मूल्य_Format2.has_device ())) प्रत्यागम;

  hb_set_t klass1_glyphs, klass2_glyphs;
  यदि (!class_def1.collect_coverage (&klass1_glyphs)) प्रत्यागम;
  यदि (!class_def2.collect_coverage (&klass2_glyphs)) प्रत्यागम;

  hb_set_t class1_set, class2_set;
  प्रति (ध्रुव अचिन्हित cp : + c->glyph_set->iter () | hb_filter (coverage))
  {
    यदि (!klass1_glyphs.has (cp)) class1_set.add (0);
    अन्यथा
    {
      अचिन्हित klass1 = class_def1.get (cp);
      class1_set.add (klass1);
    }
  }

  class2_set.add (0);
  प्रति (ध्रुव अचिन्हित cp : + c->glyph_set->iter () | hb_filter (klass2_glyphs))
  {
    अचिन्हित klass2 = class_def2.get (cp);
    class2_set.add (klass2);
  }

  यदि (class1_set.is_empty ()
      || class2_set.is_empty ()
      || (class2_set.get_population() == 1 && class2_set.has(0)))
    प्रत्यागम;

  अचिन्हित len1 = मूल्य_Format1.get_len ();
  अचिन्हित len2 = मूल्य_Format2.get_len ();
  ध्रुव hb_array_t<ध्रुव मूल्य> मूल्यावली_array = values.as_array ((अचिन्हित)class1_गिनती * (अचिन्हित) class2_गिनती * (len1 + len2));
  प्रति (ध्रुव अचिन्हित class1_idx : class1_set.iter ())
  {
    प्रति (ध्रुव अचिन्हित class2_idx : class2_set.iter ())
    {
      अचिन्हित start_offset = (class1_idx * (अचिन्हित) class2_गिनती + class2_idx) * (len1 + len2);
      यदि (मूल्य_Format1.has_device ())
        मूल्य_Format1.collect_variation_indices (c, base, मूल्यावली_array.sub_array (start_offset, len1));

      यदि (मूल्य_Format2.has_device ())
        मूल्य_Format2.collect_variation_indices (c, base, मूल्यावली_array.sub_array (start_offset+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
संरचना pair_pos_class_मूल्यावली_t
{
  ध्रुव मूल्य_Base *base;
  ध्रुव मूल्य_Record &values;
  ध्रुव मूल्य_Format &मूल्य_Format1;
  ध्रुव मूल्य_Format &मूल्य_Format2;
  अचिन्हित class1_गिनती;
  अचिन्हित class2_गिनती;

  hb_pair_t<अचिन्हित, अचिन्हित> compute_effective_मूल्य_formats (ध्रुव hb_map_t &klass1_map,
                                                               ध्रुव hb_map_t &klass2_map,
                                                               तार्किक strip_hints, तार्किक strip_empty,
                                                               ध्रुव hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *varidx_delta_map = nullptr) ध्रुव
  {
    अचिन्हित len1 = मूल्य_Format1.get_len ();
    अचिन्हित len2 = मूल्य_Format2.get_len ();
    अचिन्हित record_size = len1 + len2;

    अचिन्हित format1 = 0;
    अचिन्हित format2 = 0;

    प्रति (अचिन्हित class1_idx : + hb_range (class1_गिनती) | hb_filter (klass1_map))
    {
      प्रति (अचिन्हित class2_idx : + hb_range (class2_गिनती) | hb_filter (klass2_map))
      {
        अचिन्हित idx = (class1_idx * class2_गिनती + class2_idx) * record_size;
        format1 = format1 | मूल्य_Format1.get_effective_format (&values[idx], strip_hints, strip_empty, base, varidx_delta_map);
        format2 = format2 | मूल्य_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_empty, base, varidx_delta_map);
      }

      यदि (!varidx_delta_map &&
          format1 == मूल्य_Format1 && format2 == मूल्य_Format2)
        भङ्ग;
    }

    प्रत्यागम hb_pair (format1, format2);
  }

  तार्किक serialize (hb_subset_context_t *c,
                  ध्रुव hb_map_t &klass1_map,
                  ध्रुव hb_map_t &klass2_map,
                  मूल्य_Format newFormat1,
                  मूल्य_Format newFormat2) ध्रुव
  {
    अचिन्हित len1 = मूल्य_Format1.get_len ();
    अचिन्हित len2 = मूल्य_Format2.get_len ();
    अचिन्हित total_len = len1 + len2;
    hb_vector_t<अचिन्हित> class2_idxs (+ hb_range (class2_गिनती) | hb_filter (klass2_map));
    यदि (unlikely (!c->serializer->propagate_error (class2_idxs)))
      प्रत्यागम असत्य;
    प्रति (अचिन्हित class1_idx : + hb_range (class1_गिनती) | hb_filter (klass1_map))
    {
      प्रति (अचिन्हित class2_idx : class2_idxs)
      {
        अचिन्हित idx = (class1_idx * class2_गिनती + class2_idx) * total_len;
        मूल्य_Format1.copy_मूल्यावली (c->serializer, newFormat1, base, &values[idx], &c->plan->layout_variation_idx_delta_map);
        मूल्य_Format2.copy_मूल्यावली (c->serializer, newFormat2, base, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    प्रत्यागम सत्य;
  }
};

ढाँचा <typename Types>
संरचना PairPosFormat2_4 : मूल्य_Base
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
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (10 + 3 * Types::LOffset::static_size, values);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    यदि (!(c->check_struct (एतत्)
       && coverage.sanitize (c, एतत्)
       && classDef1.sanitize (c, एतत्)
       && classDef2.sanitize (c, एतत्))) return_trace (असत्य);

    अचिन्हित पूर्णाङ्क len1 = मूल्य_Format1.get_len ();
    अचिन्हित पूर्णाङ्क len2 = मूल्य_Format2.get_len ();
    अचिन्हित पूर्णाङ्क stride = HBUINT16::static_size * (len1 + len2);
    अचिन्हित पूर्णाङ्क गिनती = (अचिन्हित पूर्णाङ्क) class1_गिनती_2 * (अचिन्हित पूर्णाङ्क) class2_गिनती_2;
    return_trace (c->check_range ((ध्रुव शून्य *) values,
                                  गिनती,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (मूल्य_Format1.sanitize_मूल्यावली_stride_unsafe (c, एतत्, &values[0], गिनती, stride) &&
		    मूल्य_Format2.sanitize_मूल्यावली_stride_unsafe (c, एतत्, &values[len1], गिनती, stride))));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    प्रत्यागम (एतत्+coverage).intersects (glyphs) &&
           (एतत्+classDef2).intersects (glyphs);
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव {}
  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    collect_pair_variation_indices (c, एतत्, एतत्+coverage,
                                    एतत्+classDef1, एतत्+classDef2,
                                    मूल्य_Format1, मूल्य_Format2,
                                    class1_गिनती_2, class2_गिनती_2, values);
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    यदि (unlikely (!(एतत्+coverage).collect_coverage (c->निवेश))) प्रत्यागम;
    यदि (unlikely (!(एतत्+classDef2).collect_coverage (c->निवेश))) प्रत्यागम;
  }

  ध्रुव Coverage &get_coverage () ध्रुव { प्रत्यागम एतत्+coverage; }

  संरचना external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t first;
    hb_ot_layout_mapping_cache_t second;
  };
  शून्य *external_cache_create () ध्रुव
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (परिमाण (external_cache_t));
    यदि (likely (cache))
    {
      cache->coverage.clear ();
      cache->first.clear ();
      cache->second.clear ();
    }
    प्रत्यागम cache;
  }

  तार्किक apply (hb_ot_apply_context_t *c, शून्य *external_cache) ध्रुव
  {
    TRACE_APPLY (एतत्);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#अन्यथा
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    यदि (index == NOT_COVERED) return_trace (असत्य);

    स्वतः &skippy_iter = c->iter_निवेश;
    skippy_iter.reset_fast (buffer->idx);
    अचिन्हित unsafe_to;
    यदि (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (असत्य);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    अचिन्हित पूर्णाङ्क klass1 = (एतत्+classDef1).get_class (buffer->cur().codepoint, cache ? &cache->first : nullptr);
    अचिन्हित पूर्णाङ्क klass2 = (एतत्+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint, cache ? &cache->second : nullptr);
#अन्यथा
    अचिन्हित पूर्णाङ्क klass1 = (एतत्+classDef1).get_class (buffer->cur().codepoint);
    अचिन्हित पूर्णाङ्क klass2 = (एतत्+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint);
#endif
    यदि (unlikely (klass1 >= class1_गिनती_2 || klass2 >= class2_गिनती_2))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (असत्य);
    }

    अचिन्हित पूर्णाङ्क len1 = मूल्य_Format1.get_len ();
    अचिन्हित पूर्णाङ्क len2 = मूल्य_Format2.get_len ();
    अचिन्हित पूर्णाङ्क record_len = len1 + len2;

    ध्रुव मूल्य *v = &values[record_len * (klass1 * class2_गिनती_2 + klass2)];

    तार्किक applied_first = असत्य, applied_second = असत्य;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#ifndef HB_SPLIT_KERN
    यदि (असत्य)
#endif
    {
      यदि (!len2)
      {
        ध्रुव hb_direction_t dir = buffer->props.direction;
        ध्रुव तार्किक horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        ध्रुव तार्किक backward = HB_DIRECTION_IS_BACKWARD (dir);
        अचिन्हित mask = horizontal ? मूल्य_Format::xAdvance : मूल्य_Format::yAdvance;
        यदि (backward)
          mask |= mask >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        mask |= mask << 4;

        यदि (मूल्य_Format1 & ~mask)
          गच्छ bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        यदि (मूल्य_Format1.apply_मूल्य (c, एतत्, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &buffer->cur_pos().x_advance;
          hb_position_t *dst2 = &buffer->pos[skippy_iter.idx].x_advance;
          अचिन्हित i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          यदि (!backward)
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
          गच्छ success;
        }
        गच्छ boring;
      }
    }
    bail:

    यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "try kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    applied_first = len1 && मूल्य_Format1.apply_मूल्य (c, एतत्, v, buffer->cur_pos());
    applied_second = len2 && मूल्य_Format2.apply_मूल्य (c, एतत्, v + len1, buffer->pos[skippy_iter.idx]);

    यदि (applied_first || applied_second)
      यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "kerned glyphs at %u,%u",
			    c->buffer->idx, skippy_iter.idx);
      }

    यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    success:
    यदि (applied_first || applied_second)
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    अन्यथा
    boring:
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);

    यदि (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    }

    buffer->idx = skippy_iter.idx;

    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;

    hb_map_t klass1_map;
    out->classDef1.serialize_subset (c, classDef1, एतत्, &klass1_map, सत्य, सत्य, &(एतत् + coverage));
    out->class1_गिनती_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    out->classDef2.serialize_subset (c, classDef2, एतत्, &klass2_map, सत्य, असत्य);
    out->class2_गिनती_2 = klass2_map.get_population ();

    pair_pos_class_मूल्यावली_t class_मूल्यावली {एतत्, values, मूल्य_Format1, मूल्य_Format2,
                                         class1_गिनती_2, class2_गिनती_2};

    hb_pair_t<अचिन्हित, अचिन्हित> newFormats = hb_pair (मूल्य_Format1, मूल्य_Format2);

    यदि (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_मूल्यावली.compute_effective_मूल्य_formats (klass1_map, klass2_map, असत्य, असत्य, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    अन्यथा यदि (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      तार्किक has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      तार्किक strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      यदि (has_fvar && !c->plan->has_gdef_varstore)
        strip = सत्य;
      newFormats = class_मूल्यावली.compute_effective_मूल्य_formats (klass1_map, klass2_map, strip, सत्य);
    }

    out->मूल्य_Format1 = newFormats.first;
    out->मूल्य_Format2 = newFormats.second;

    यदि (!class_मूल्यावली.serialize (c, klass1_map, klass2_map,
                                 out->मूल्य_Format1, out->मूल्य_Format2))
      return_trace (असत्य);

    तार्किक ret = out->coverage.serialize_subset(c, coverage, एतत्);
    return_trace (out->class1_गिनती_2 && out->class2_गिनती_2 && ret);
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
