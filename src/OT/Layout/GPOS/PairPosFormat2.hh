#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#અન્તર્ભાવ "ValueFormat.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {

HB_INTERNAL અંતઃરેખ
શૂન્ય collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     અચલ કિંમત_Base *base,
                                     અચલ Coverage &coverage,
                                     અચલ ClassDef &class_def1,
                                     અચલ ClassDef &class_def2,
                                     અચલ કિંમત_Format &કિંમત_Format1,
                                     અચલ કિંમત_Format &કિંમત_Format2,
                                     unsigned class1_ગણતરી,
                                     unsigned class2_ગણતરી,
                                     અચલ કિંમત_Record &values)
{
  જો (!coverage.intersects (c->glyph_set) ||
      !class_def2.intersects (c->glyph_set)) પાછા;
  જો ((!કિંમત_Format1.has_device ()) && (!કિંમત_Format2.has_device ())) પાછા;

  hb_set_t klass1_glyphs, klass2_glyphs;
  જો (!class_def1.collect_coverage (&klass1_glyphs)) પાછા;
  જો (!class_def2.collect_coverage (&klass2_glyphs)) પાછા;

  hb_set_t class1_set, class2_set;
  માટે (અચલ unsigned cp : + c->glyph_set->iter () | hb_filter (coverage))
  {
    જો (!klass1_glyphs.has (cp)) class1_set.add (0);
    નહીંતર
    {
      unsigned klass1 = class_def1.get (cp);
      class1_set.add (klass1);
    }
  }

  class2_set.add (0);
  માટે (અચલ unsigned cp : + c->glyph_set->iter () | hb_filter (klass2_glyphs))
  {
    unsigned klass2 = class_def2.get (cp);
    class2_set.add (klass2);
  }

  જો (class1_set.is_empty ()
      || class2_set.is_empty ()
      || (class2_set.get_population() == 1 && class2_set.has(0)))
    પાછા;

  unsigned len1 = કિંમત_Format1.get_len ();
  unsigned len2 = કિંમત_Format2.get_len ();
  અચલ hb_array_t<અચલ કિંમત> કિંમતો_array = values.as_array ((unsigned)class1_ગણતરી * (unsigned) class2_ગણતરી * (len1 + len2));
  માટે (અચલ unsigned class1_idx : class1_set.iter ())
  {
    માટે (અચલ unsigned class2_idx : class2_set.iter ())
    {
      unsigned start_offset = (class1_idx * (unsigned) class2_ગણતરી + class2_idx) * (len1 + len2);
      જો (કિંમત_Format1.has_device ())
        કિંમત_Format1.collect_variation_indices (c, base, કિંમતો_array.sub_array (start_offset, len1));

      જો (કિંમત_Format2.has_device ())
        કિંમત_Format2.collect_variation_indices (c, base, કિંમતો_array.sub_array (start_offset+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
સંરચના pair_pos_class_કિંમતો_t
{
  અચલ કિંમત_Base *base;
  અચલ કિંમત_Record &values;
  અચલ કિંમત_Format &કિંમત_Format1;
  અચલ કિંમત_Format &કિંમત_Format2;
  unsigned class1_ગણતરી;
  unsigned class2_ગણતરી;

  hb_pair_t<unsigned, unsigned> compute_effective_કિંમત_formats (અચલ hb_map_t &klass1_map,
                                                               અચલ hb_map_t &klass2_map,
                                                               તાર્કિક strip_hints, તાર્કિક strip_empty,
                                                               અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *varidx_delta_map = nullptr) અચલ
  {
    unsigned len1 = કિંમત_Format1.get_len ();
    unsigned len2 = કિંમત_Format2.get_len ();
    unsigned record_size = len1 + len2;

    unsigned format1 = 0;
    unsigned format2 = 0;

    માટે (unsigned class1_idx : + hb_range (class1_ગણતરી) | hb_filter (klass1_map))
    {
      માટે (unsigned class2_idx : + hb_range (class2_ગણતરી) | hb_filter (klass2_map))
      {
        unsigned idx = (class1_idx * class2_ગણતરી + class2_idx) * record_size;
        format1 = format1 | કિંમત_Format1.get_effective_format (&values[idx], strip_hints, strip_empty, base, varidx_delta_map);
        format2 = format2 | કિંમત_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_empty, base, varidx_delta_map);
      }

      જો (!varidx_delta_map &&
          format1 == કિંમત_Format1 && format2 == કિંમત_Format2)
        તોડો;
    }

    પાછા hb_pair (format1, format2);
  }

  તાર્કિક serialize (hb_subset_context_t *c,
                  અચલ hb_map_t &klass1_map,
                  અચલ hb_map_t &klass2_map,
                  કિંમત_Format newFormat1,
                  કિંમત_Format newFormat2) અચલ
  {
    unsigned len1 = કિંમત_Format1.get_len ();
    unsigned len2 = કિંમત_Format2.get_len ();
    unsigned total_len = len1 + len2;
    hb_vector_t<unsigned> class2_idxs (+ hb_range (class2_ગણતરી) | hb_filter (klass2_map));
    જો (unlikely (!c->serializer->propagate_error (class2_idxs)))
      પાછા ખોટું;
    માટે (unsigned class1_idx : + hb_range (class1_ગણતરી) | hb_filter (klass1_map))
    {
      માટે (unsigned class2_idx : class2_idxs)
      {
        unsigned idx = (class1_idx * class2_ગણતરી + class2_idx) * total_len;
        કિંમત_Format1.copy_કિંમતો (c->serializer, newFormat1, base, &values[idx], &c->plan->layout_variation_idx_delta_map);
        કિંમત_Format2.copy_કિંમતો (c->serializer, newFormat2, base, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    પાછા સાચું;
  }
};

template <typename Types>
સંરચના PairPosFormat2_4 : કિંમત_Base
{
  સુરક્ષિત:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  કિંમત_Format   કિંમત_Format1;           /* ValueRecord definition--for the
                                         * first glyph of the pair--may be zero
                                         * (0) */
  કિંમત_Format   કિંમત_Format2;           /* ValueRecord definition--for the
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
  HBUINT16      class1_ગણતરી_2;            /* Number of classes in ClassDef1
                                         * table--includes Class0 */
  HBUINT16      class2_ગણતરી_2;            /* Number of classes in ClassDef2
                                         * table--includes Class0 */
  કિંમત_Record   values;                 /* Matrix of value pairs:
                                         * class1-major, class2-minor,
                                         * Each entry has value1 and value2 */
  જાહેર:
  DEFINE_SIZE_ARRAY (10 + 3 * Types::LOffset::static_size, values);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!(c->check_struct (આ)
       && coverage.sanitize (c, આ)
       && classDef1.sanitize (c, આ)
       && classDef2.sanitize (c, આ))) return_trace (ખોટું);

    unsigned પૂર્ણાંક len1 = કિંમત_Format1.get_len ();
    unsigned પૂર્ણાંક len2 = કિંમત_Format2.get_len ();
    unsigned પૂર્ણાંક stride = HBUINT16::static_size * (len1 + len2);
    unsigned પૂર્ણાંક ગણતરી = (unsigned પૂર્ણાંક) class1_ગણતરી_2 * (unsigned પૂર્ણાંક) class2_ગણતરી_2;
    return_trace (c->check_range ((અચલ શૂન્ય *) values,
                                  ગણતરી,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (કિંમત_Format1.sanitize_કિંમતો_stride_unsafe (c, આ, &values[0], ગણતરી, stride) &&
		    કિંમત_Format2.sanitize_કિંમતો_stride_unsafe (c, આ, &values[len1], ગણતરી, stride))));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    પાછા (આ+coverage).intersects (glyphs) &&
           (આ+classDef2).intersects (glyphs);
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ {}
  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    collect_pair_variation_indices (c, આ, આ+coverage,
                                    આ+classDef1, આ+classDef2,
                                    કિંમત_Format1, કિંમત_Format2,
                                    class1_ગણતરી_2, class2_ગણતરી_2, values);
  }

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    જો (unlikely (!(આ+coverage).collect_coverage (c->આગત))) પાછા;
    જો (unlikely (!(આ+classDef2).collect_coverage (c->આગત))) પાછા;
  }

  અચલ Coverage &get_coverage () અચલ { પાછા આ+coverage; }

  સંરચના external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t first;
    hb_ot_layout_mapping_cache_t second;
  };
  શૂન્ય *external_cache_create () અચલ
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (કદ (external_cache_t));
    જો (likely (cache))
    {
      cache->coverage.clear ();
      cache->first.clear ();
      cache->second.clear ();
    }
    પાછા cache;
  }

  તાર્કિક apply (hb_ot_apply_context_t *c, શૂન્ય *external_cache) અચલ
  {
    TRACE_APPLY (આ);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#નહીંતર
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    જો (index == NOT_COVERED) return_trace (ખોટું);

    auto &skippy_iter = c->iter_આગત;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    જો (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (ખોટું);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    unsigned પૂર્ણાંક klass1 = (આ+classDef1).get_class (buffer->cur().codepoint, cache ? &cache->first : nullptr);
    unsigned પૂર્ણાંક klass2 = (આ+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint, cache ? &cache->second : nullptr);
#નહીંતર
    unsigned પૂર્ણાંક klass1 = (આ+classDef1).get_class (buffer->cur().codepoint);
    unsigned પૂર્ણાંક klass2 = (આ+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint);
#endif
    જો (unlikely (klass1 >= class1_ગણતરી_2 || klass2 >= class2_ગણતરી_2))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (ખોટું);
    }

    unsigned પૂર્ણાંક len1 = કિંમત_Format1.get_len ();
    unsigned પૂર્ણાંક len2 = કિંમત_Format2.get_len ();
    unsigned પૂર્ણાંક record_len = len1 + len2;

    અચલ કિંમત *v = &values[record_len * (klass1 * class2_ગણતરી_2 + klass2)];

    તાર્કિક applied_first = ખોટું, applied_second = ખોટું;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#ifndef HB_SPLIT_KERN
    જો (ખોટું)
#endif
    {
      જો (!len2)
      {
        અચલ hb_direction_t dir = buffer->props.direction;
        અચલ તાર્કિક horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        અચલ તાર્કિક backward = HB_DIRECTION_IS_BACKWARD (dir);
        unsigned mask = horizontal ? કિંમત_Format::xAdvance : કિંમત_Format::yAdvance;
        જો (backward)
          mask |= mask >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        mask |= mask << 4;

        જો (કિંમત_Format1 & ~mask)
          જાઓ bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        જો (કિંમત_Format1.apply_કિંમત (c, આ, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &buffer->cur_pos().x_advance;
          hb_position_t *dst2 = &buffer->pos[skippy_iter.idx].x_advance;
          unsigned i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          જો (!backward)
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
            dst2[i + 2] = hb_saturate_add (dst2[i + 2], kern2);
          }
          નહીંતર
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst1[i + 2] = hb_clamp_to<hb_position_t> ((int64_t) dst1[i + 2] + src[i + 2] - kern2);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
          }

          applied_first = applied_second = kern != 0;
          જાઓ success;
        }
        જાઓ boring;
      }
    }
    bail:

    જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "try kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    applied_first = len1 && કિંમત_Format1.apply_કિંમત (c, આ, v, buffer->cur_pos());
    applied_second = len2 && કિંમત_Format2.apply_કિંમત (c, આ, v + len1, buffer->pos[skippy_iter.idx]);

    જો (applied_first || applied_second)
      જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "kerned glyphs at %u,%u",
			    c->buffer->idx, skippy_iter.idx);
      }

    જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    success:
    જો (applied_first || applied_second)
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    નહીંતર
    boring:
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);

    જો (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    }

    buffer->idx = skippy_iter.idx;

    return_trace (સાચું);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);
    out->format = format;

    hb_map_t klass1_map;
    out->classDef1.serialize_subset (c, classDef1, આ, &klass1_map, સાચું, સાચું, &(આ + coverage));
    out->class1_ગણતરી_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    out->classDef2.serialize_subset (c, classDef2, આ, &klass2_map, સાચું, ખોટું);
    out->class2_ગણતરી_2 = klass2_map.get_population ();

    pair_pos_class_કિંમતો_t class_કિંમતો {આ, values, કિંમત_Format1, કિંમત_Format2,
                                         class1_ગણતરી_2, class2_ગણતરી_2};

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (કિંમત_Format1, કિંમત_Format2);

    જો (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_કિંમતો.compute_effective_કિંમત_formats (klass1_map, klass2_map, ખોટું, ખોટું, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    નહીંતર જો (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      તાર્કિક has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      તાર્કિક strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      જો (has_fvar && !c->plan->has_gdef_varstore)
        strip = સાચું;
      newFormats = class_કિંમતો.compute_effective_કિંમત_formats (klass1_map, klass2_map, strip, સાચું);
    }

    out->કિંમત_Format1 = newFormats.first;
    out->કિંમત_Format2 = newFormats.second;

    જો (!class_કિંમતો.serialize (c, klass1_map, klass2_map,
                                 out->કિંમત_Format1, out->કિંમત_Format2))
      return_trace (ખોટું);

    તાર્કિક ret = out->coverage.serialize_subset(c, coverage, આ);
    return_trace (out->class1_ગણતરી_2 && out->class2_ગણતરી_2 && ret);
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
