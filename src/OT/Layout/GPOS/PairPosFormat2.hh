#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#അന്തര്ഭാവ "ValueFormat.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

HB_INTERNAL വരിയിൽ
ശൂന്യം collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     സ്ഥിരം മൂല്യം_Base *base,
                                     സ്ഥിരം Coverage &coverage,
                                     സ്ഥിരം ClassDef &class_def1,
                                     സ്ഥിരം ClassDef &class_def2,
                                     സ്ഥിരം മൂല്യം_Format &മൂല്യം_Format1,
                                     സ്ഥിരം മൂല്യം_Format &മൂല്യം_Format2,
                                     unsigned class1_എണ്ണം,
                                     unsigned class2_എണ്ണം,
                                     സ്ഥിരം മൂല്യം_Record &values)
{
  എങ്കിൽ (!coverage.intersects (c->glyph_set) ||
      !class_def2.intersects (c->glyph_set)) തിരികെ;
  എങ്കിൽ ((!മൂല്യം_Format1.has_device ()) && (!മൂല്യം_Format2.has_device ())) തിരികെ;

  hb_set_t klass1_glyphs, klass2_glyphs;
  എങ്കിൽ (!class_def1.collect_coverage (&klass1_glyphs)) തിരികെ;
  എങ്കിൽ (!class_def2.collect_coverage (&klass2_glyphs)) തിരികെ;

  hb_set_t class1_set, class2_set;
  വേണ്ടി (സ്ഥിരം unsigned cp : + c->glyph_set->iter () | hb_filter (coverage))
  {
    എങ്കിൽ (!klass1_glyphs.has (cp)) class1_set.add (0);
    അല്ലെങ്കിൽ
    {
      unsigned klass1 = class_def1.get (cp);
      class1_set.add (klass1);
    }
  }

  class2_set.add (0);
  വേണ്ടി (സ്ഥിരം unsigned cp : + c->glyph_set->iter () | hb_filter (klass2_glyphs))
  {
    unsigned klass2 = class_def2.get (cp);
    class2_set.add (klass2);
  }

  എങ്കിൽ (class1_set.is_empty ()
      || class2_set.is_empty ()
      || (class2_set.get_population() == 1 && class2_set.has(0)))
    തിരികെ;

  unsigned len1 = മൂല്യം_Format1.get_len ();
  unsigned len2 = മൂല്യം_Format2.get_len ();
  സ്ഥിരം hb_array_t<സ്ഥിരം മൂല്യം> മൂല്യങ്ങൾ_array = values.as_array ((unsigned)class1_എണ്ണം * (unsigned) class2_എണ്ണം * (len1 + len2));
  വേണ്ടി (സ്ഥിരം unsigned class1_idx : class1_set.iter ())
  {
    വേണ്ടി (സ്ഥിരം unsigned class2_idx : class2_set.iter ())
    {
      unsigned start_offset = (class1_idx * (unsigned) class2_എണ്ണം + class2_idx) * (len1 + len2);
      എങ്കിൽ (മൂല്യം_Format1.has_device ())
        മൂല്യം_Format1.collect_variation_indices (c, base, മൂല്യങ്ങൾ_array.sub_array (start_offset, len1));

      എങ്കിൽ (മൂല്യം_Format2.has_device ())
        മൂല്യം_Format2.collect_variation_indices (c, base, മൂല്യങ്ങൾ_array.sub_array (start_offset+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
ഘടന pair_pos_class_മൂല്യങ്ങൾ_t
{
  സ്ഥിരം മൂല്യം_Base *base;
  സ്ഥിരം മൂല്യം_Record &values;
  സ്ഥിരം മൂല്യം_Format &മൂല്യം_Format1;
  സ്ഥിരം മൂല്യം_Format &മൂല്യം_Format2;
  unsigned class1_എണ്ണം;
  unsigned class2_എണ്ണം;

  hb_pair_t<unsigned, unsigned> compute_effective_മൂല്യം_formats (സ്ഥിരം hb_map_t &klass1_map,
                                                               സ്ഥിരം hb_map_t &klass2_map,
                                                               തർക്കം strip_hints, തർക്കം strip_empty,
                                                               സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *varidx_delta_map = nullptr) സ്ഥിരം
  {
    unsigned len1 = മൂല്യം_Format1.get_len ();
    unsigned len2 = മൂല്യം_Format2.get_len ();
    unsigned record_size = len1 + len2;

    unsigned format1 = 0;
    unsigned format2 = 0;

    വേണ്ടി (unsigned class1_idx : + hb_range (class1_എണ്ണം) | hb_filter (klass1_map))
    {
      വേണ്ടി (unsigned class2_idx : + hb_range (class2_എണ്ണം) | hb_filter (klass2_map))
      {
        unsigned idx = (class1_idx * class2_എണ്ണം + class2_idx) * record_size;
        format1 = format1 | മൂല്യം_Format1.get_effective_format (&values[idx], strip_hints, strip_empty, base, varidx_delta_map);
        format2 = format2 | മൂല്യം_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_empty, base, varidx_delta_map);
      }

      എങ്കിൽ (!varidx_delta_map &&
          format1 == മൂല്യം_Format1 && format2 == മൂല്യം_Format2)
        നിർത്തുക;
    }

    തിരികെ hb_pair (format1, format2);
  }

  തർക്കം serialize (hb_subset_context_t *c,
                  സ്ഥിരം hb_map_t &klass1_map,
                  സ്ഥിരം hb_map_t &klass2_map,
                  മൂല്യം_Format newFormat1,
                  മൂല്യം_Format newFormat2) സ്ഥിരം
  {
    unsigned len1 = മൂല്യം_Format1.get_len ();
    unsigned len2 = മൂല്യം_Format2.get_len ();
    unsigned total_len = len1 + len2;
    hb_vector_t<unsigned> class2_idxs (+ hb_range (class2_എണ്ണം) | hb_filter (klass2_map));
    എങ്കിൽ (unlikely (!c->serializer->propagate_error (class2_idxs)))
      തിരികെ അസത്യം;
    വേണ്ടി (unsigned class1_idx : + hb_range (class1_എണ്ണം) | hb_filter (klass1_map))
    {
      വേണ്ടി (unsigned class2_idx : class2_idxs)
      {
        unsigned idx = (class1_idx * class2_എണ്ണം + class2_idx) * total_len;
        മൂല്യം_Format1.copy_മൂല്യങ്ങൾ (c->serializer, newFormat1, base, &values[idx], &c->plan->layout_variation_idx_delta_map);
        മൂല്യം_Format2.copy_മൂല്യങ്ങൾ (c->serializer, newFormat2, base, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    തിരികെ സത്യം;
  }
};

template <typename Types>
ഘടന PairPosFormat2_4 : മൂല്യം_Base
{
  സംരക്ഷിത:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  മൂല്യം_Format   മൂല്യം_Format1;           /* ValueRecord definition--for the
                                         * first glyph of the pair--may be zero
                                         * (0) */
  മൂല്യം_Format   മൂല്യം_Format2;           /* ValueRecord definition--for the
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
  HBUINT16      class1_എണ്ണം_2;            /* Number of classes in ClassDef1
                                         * table--includes Class0 */
  HBUINT16      class2_എണ്ണം_2;            /* Number of classes in ClassDef2
                                         * table--includes Class0 */
  മൂല്യം_Record   values;                 /* Matrix of value pairs:
                                         * class1-major, class2-minor,
                                         * Each entry has value1 and value2 */
  പൊതു:
  DEFINE_SIZE_ARRAY (10 + 3 * Types::LOffset::static_size, values);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!(c->check_struct (ഇത്)
       && coverage.sanitize (c, ഇത്)
       && classDef1.sanitize (c, ഇത്)
       && classDef2.sanitize (c, ഇത്))) return_trace (അസത്യം);

    unsigned പൂർണ്ണസംഖ്യ len1 = മൂല്യം_Format1.get_len ();
    unsigned പൂർണ്ണസംഖ്യ len2 = മൂല്യം_Format2.get_len ();
    unsigned പൂർണ്ണസംഖ്യ stride = HBUINT16::static_size * (len1 + len2);
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = (unsigned പൂർണ്ണസംഖ്യ) class1_എണ്ണം_2 * (unsigned പൂർണ്ണസംഖ്യ) class2_എണ്ണം_2;
    return_trace (c->check_range ((സ്ഥിരം ശൂന്യം *) values,
                                  എണ്ണം,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (മൂല്യം_Format1.sanitize_മൂല്യങ്ങൾ_stride_unsafe (c, ഇത്, &values[0], എണ്ണം, stride) &&
		    മൂല്യം_Format2.sanitize_മൂല്യങ്ങൾ_stride_unsafe (c, ഇത്, &values[len1], എണ്ണം, stride))));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    തിരികെ (ഇത്+coverage).intersects (glyphs) &&
           (ഇത്+classDef2).intersects (glyphs);
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം {}
  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    collect_pair_variation_indices (c, ഇത്, ഇത്+coverage,
                                    ഇത്+classDef1, ഇത്+classDef2,
                                    മൂല്യം_Format1, മൂല്യം_Format2,
                                    class1_എണ്ണം_2, class2_എണ്ണം_2, values);
  }

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!(ഇത്+coverage).collect_coverage (c->നിവേശം))) തിരികെ;
    എങ്കിൽ (unlikely (!(ഇത്+classDef2).collect_coverage (c->നിവേശം))) തിരികെ;
  }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം { തിരികെ ഇത്+coverage; }

  ഘടന external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t first;
    hb_ot_layout_mapping_cache_t second;
  };
  ശൂന്യം *external_cache_create () സ്ഥിരം
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (വലുപ്പം (external_cache_t));
    എങ്കിൽ (likely (cache))
    {
      cache->coverage.clear ();
      cache->first.clear ();
      cache->second.clear ();
    }
    തിരികെ cache;
  }

  തർക്കം apply (hb_ot_apply_context_t *c, ശൂന്യം *external_cache) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#അല്ലെങ്കിൽ
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    എങ്കിൽ (index == NOT_COVERED) return_trace (അസത്യം);

    auto &skippy_iter = c->iter_നിവേശം;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    എങ്കിൽ (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (അസത്യം);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    unsigned പൂർണ്ണസംഖ്യ klass1 = (ഇത്+classDef1).get_class (buffer->cur().codepoint, cache ? &cache->first : nullptr);
    unsigned പൂർണ്ണസംഖ്യ klass2 = (ഇത്+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint, cache ? &cache->second : nullptr);
#അല്ലെങ്കിൽ
    unsigned പൂർണ്ണസംഖ്യ klass1 = (ഇത്+classDef1).get_class (buffer->cur().codepoint);
    unsigned പൂർണ്ണസംഖ്യ klass2 = (ഇത്+classDef2).get_class (buffer->info[skippy_iter.idx].codepoint);
#endif
    എങ്കിൽ (unlikely (klass1 >= class1_എണ്ണം_2 || klass2 >= class2_എണ്ണം_2))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (അസത്യം);
    }

    unsigned പൂർണ്ണസംഖ്യ len1 = മൂല്യം_Format1.get_len ();
    unsigned പൂർണ്ണസംഖ്യ len2 = മൂല്യം_Format2.get_len ();
    unsigned പൂർണ്ണസംഖ്യ record_len = len1 + len2;

    സ്ഥിരം മൂല്യം *v = &values[record_len * (klass1 * class2_എണ്ണം_2 + klass2)];

    തർക്കം applied_first = അസത്യം, applied_second = അസത്യം;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#ifndef HB_SPLIT_KERN
    എങ്കിൽ (അസത്യം)
#endif
    {
      എങ്കിൽ (!len2)
      {
        സ്ഥിരം hb_direction_t dir = buffer->props.direction;
        സ്ഥിരം തർക്കം horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        സ്ഥിരം തർക്കം backward = HB_DIRECTION_IS_BACKWARD (dir);
        unsigned mask = horizontal ? മൂല്യം_Format::xAdvance : മൂല്യം_Format::yAdvance;
        എങ്കിൽ (backward)
          mask |= mask >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        mask |= mask << 4;

        എങ്കിൽ (മൂല്യം_Format1 & ~mask)
          പോകുക bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        എങ്കിൽ (മൂല്യം_Format1.apply_മൂല്യം (c, ഇത്, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &buffer->cur_pos().x_advance;
          hb_position_t *dst2 = &buffer->pos[skippy_iter.idx].x_advance;
          unsigned i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          എങ്കിൽ (!backward)
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
            dst2[i + 2] = hb_saturate_add (dst2[i + 2], kern2);
          }
          അല്ലെങ്കിൽ
          {
            dst1[i] = hb_saturate_add (dst1[i], kern1);
            dst1[i + 2] = hb_clamp_to<hb_position_t> ((int64_t) dst1[i + 2] + src[i + 2] - kern2);
            dst2[i] = hb_saturate_add (dst2[i], kern2);
          }

          applied_first = applied_second = kern != 0;
          പോകുക success;
        }
        പോകുക boring;
      }
    }
    bail:

    എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "try kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    applied_first = len1 && മൂല്യം_Format1.apply_മൂല്യം (c, ഇത്, v, buffer->cur_pos());
    applied_second = len2 && മൂല്യം_Format2.apply_മൂല്യം (c, ഇത്, v + len1, buffer->pos[skippy_iter.idx]);

    എങ്കിൽ (applied_first || applied_second)
      എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "kerned glyphs at %u,%u",
			    c->buffer->idx, skippy_iter.idx);
      }

    എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->buffer->idx, skippy_iter.idx);
    }

    success:
    എങ്കിൽ (applied_first || applied_second)
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    അല്ലെങ്കിൽ
    boring:
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);

    എങ്കിൽ (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, skippy_iter.idx + 1);
    }

    buffer->idx = skippy_iter.idx;

    return_trace (സത്യം);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);
    out->format = format;

    hb_map_t klass1_map;
    out->classDef1.serialize_subset (c, classDef1, ഇത്, &klass1_map, സത്യം, സത്യം, &(ഇത് + coverage));
    out->class1_എണ്ണം_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    out->classDef2.serialize_subset (c, classDef2, ഇത്, &klass2_map, സത്യം, അസത്യം);
    out->class2_എണ്ണം_2 = klass2_map.get_population ();

    pair_pos_class_മൂല്യങ്ങൾ_t class_മൂല്യങ്ങൾ {ഇത്, values, മൂല്യം_Format1, മൂല്യം_Format2,
                                         class1_എണ്ണം_2, class2_എണ്ണം_2};

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (മൂല്യം_Format1, മൂല്യം_Format2);

    എങ്കിൽ (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_മൂല്യങ്ങൾ.compute_effective_മൂല്യം_formats (klass1_map, klass2_map, അസത്യം, അസത്യം, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    അല്ലെങ്കിൽ എങ്കിൽ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      തർക്കം has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      തർക്കം strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      എങ്കിൽ (has_fvar && !c->plan->has_gdef_varstore)
        strip = സത്യം;
      newFormats = class_മൂല്യങ്ങൾ.compute_effective_മൂല്യം_formats (klass1_map, klass2_map, strip, സത്യം);
    }

    out->മൂല്യം_Format1 = newFormats.first;
    out->മൂല്യം_Format2 = newFormats.second;

    എങ്കിൽ (!class_മൂല്യങ്ങൾ.serialize (c, klass1_map, klass2_map,
                                 out->മൂല്യം_Format1, out->മൂല്യം_Format2))
      return_trace (അസത്യം);

    തർക്കം ret = out->coverage.serialize_subset(c, coverage, ഇത്);
    return_trace (out->class1_എണ്ണം_2 && out->class2_എണ്ണം_2 && ret);
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
