#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#وصف_ڪريو OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#ضمن "ValueFormat.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

HB_اندروني مضمن
فراغ collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     ثابت قدر_بنياد *بنياد,
                                     ثابت Coverage &coverage,
                                     ثابت ClassDef &class_def1,
                                     ثابت ClassDef &class_def2,
                                     ثابت قدر_Format &قدر_Format1,
                                     ثابت قدر_Format &قدر_Format2,
                                     بنانشان class1_ڳڻپ,
                                     بنانشان class2_ڳڻپ,
                                     ثابت قدر_رڪارڊ &values)
{
  إذا (!coverage.intersects (c->glyph_سيٽ) ||
      !class_def2.intersects (c->glyph_سيٽ)) أرجع;
  إذا ((!قدر_Format1.has_device ()) && (!قدر_Format2.has_device ())) أرجع;

  hb_سيٽ_t klass1_glyphs, klass2_glyphs;
  إذا (!class_def1.collect_coverage (&klass1_glyphs)) أرجع;
  إذا (!class_def2.collect_coverage (&klass2_glyphs)) أرجع;

  hb_سيٽ_t class1_سيٽ, class2_سيٽ;
  لكل (ثابت بنانشان cp : + c->glyph_سيٽ->iter () | hb_filter (coverage))
  {
    إذا (!klass1_glyphs.has (cp)) class1_سيٽ.جوڙيو (0);
    وإلا
    {
      بنانشان klass1 = class_def1.get (cp);
      class1_سيٽ.جوڙيو (klass1);
    }
  }

  class2_سيٽ.جوڙيو (0);
  لكل (ثابت بنانشان cp : + c->glyph_سيٽ->iter () | hb_filter (klass2_glyphs))
  {
    بنانشان klass2 = class_def2.get (cp);
    class2_سيٽ.جوڙيو (klass2);
  }

  إذا (class1_سيٽ.is_empty ()
      || class2_سيٽ.is_empty ()
      || (class2_سيٽ.get_population() == 1 && class2_سيٽ.has(0)))
    أرجع;

  بنانشان len1 = قدر_Format1.get_len ();
  بنانشان len2 = قدر_Format2.get_len ();
  ثابت hb_array_t<ثابت قدر> قدرون_array = values.as_array ((بنانشان)class1_ڳڻپ * (بنانشان) class2_ڳڻپ * (len1 + len2));
  لكل (ثابت بنانشان class1_idx : class1_سيٽ.iter ())
  {
    لكل (ثابت بنانشان class2_idx : class2_سيٽ.iter ())
    {
      بنانشان شروعات_وٿي = (class1_idx * (بنانشان) class2_ڳڻپ + class2_idx) * (len1 + len2);
      إذا (قدر_Format1.has_device ())
        قدر_Format1.collect_variation_indices (c, بنياد, قدرون_array.sub_array (شروعات_وٿي, len1));

      إذا (قدر_Format2.has_device ())
        قدر_Format2.collect_variation_indices (c, بنياد, قدرون_array.sub_array (شروعات_وٿي+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
بنية pair_pos_class_قدرون_t
{
  ثابت قدر_بنياد *بنياد;
  ثابت قدر_رڪارڊ &values;
  ثابت قدر_Format &قدر_Format1;
  ثابت قدر_Format &قدر_Format2;
  بنانشان class1_ڳڻپ;
  بنانشان class2_ڳڻپ;

  hb_pair_t<بنانشان, بنانشان> compute_effective_قدر_formats (ثابت hb_map_t &klass1_map,
                                                               ثابت hb_map_t &klass2_map,
                                                               منطقي strip_hints, منطقي strip_خالي_جايون,
                                                               ثابت hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> *varidx_delta_map = خالي_اشارو) ثابت
  {
    بنانشان len1 = قدر_Format1.get_len ();
    بنانشان len2 = قدر_Format2.get_len ();
    بنانشان رڪارڊ_سائيز = len1 + len2;

    بنانشان format1 = 0;
    بنانشان format2 = 0;

    لكل (بنانشان class1_idx : + hb_range (class1_ڳڻپ) | hb_filter (klass1_map))
    {
      لكل (بنانشان class2_idx : + hb_range (class2_ڳڻپ) | hb_filter (klass2_map))
      {
        بنانشان idx = (class1_idx * class2_ڳڻپ + class2_idx) * رڪارڊ_سائيز;
        format1 = format1 | قدر_Format1.get_effective_format (&values[idx], strip_hints, strip_خالي_جايون, بنياد, varidx_delta_map);
        format2 = format2 | قدر_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_خالي_جايون, بنياد, varidx_delta_map);
      }

      إذا (!varidx_delta_map &&
          format1 == قدر_Format1 && format2 == قدر_Format2)
        اكسر;
    }

    أرجع hb_pair (format1, format2);
  }

  منطقي serialize (hb_subset_context_t *c,
                  ثابت hb_map_t &klass1_map,
                  ثابت hb_map_t &klass2_map,
                  قدر_Format newFormat1,
                  قدر_Format newFormat2) ثابت
  {
    بنانشان len1 = قدر_Format1.get_len ();
    بنانشان len2 = قدر_Format2.get_len ();
    بنانشان ڪل_len = len1 + len2;
    hb_ويڪٽر_t<بنانشان> class2_idxs (+ hb_range (class2_ڳڻپ) | hb_filter (klass2_map));
    إذا (unlikely (!c->serializer->propagate_error (class2_idxs)))
      أرجع خطأ;
    لكل (بنانشان class1_idx : + hb_range (class1_ڳڻپ) | hb_filter (klass1_map))
    {
      لكل (بنانشان class2_idx : class2_idxs)
      {
        بنانشان idx = (class1_idx * class2_ڳڻپ + class2_idx) * ڪل_len;
        قدر_Format1.copy_قدرون (c->serializer, newFormat1, بنياد, &values[idx], &c->plan->layout_variation_idx_delta_map);
        قدر_Format2.copy_قدرون (c->serializer, newFormat2, بنياد, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    أرجع سچ;
  }
};

سانچو <قسم_نالو Types>
بنية PairPosFormat2_4 : قدر_بنياد
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  قدر_Format   قدر_Format1;           /* ValueRecord definition--for the
                                         * first glyph of the pair--may be zero
                                         * (0) */
  قدر_Format   قدر_Format2;           /* ValueRecord definition--for the
                                         * second glyph of the pair--may be
                                         * zero (0) */
  قسم_نالو Types::سانچو LOffset_منزل<ClassDef>
                classDef1;              /* Offset to ClassDef table--from
                                         * beginning of PairPos subtable--for
                                         * the first glyph of the pair */
  قسم_نالو Types::سانچو LOffset_منزل<ClassDef>
                classDef2;              /* Offset to ClassDef table--from
                                         * beginning of PairPos subtable--for
                                         * the second glyph of the pair */
  HBUINT16      class1_ڳڻپ_2;            /* Number of classes in ClassDef1
                                         * table--includes Class0 */
  HBUINT16      class2_ڳڻپ_2;            /* Number of classes in ClassDef2
                                         * table--includes Class0 */
  قدر_رڪارڊ   values;                 /* Matrix of value pairs:
                                         * class1-major, class2-minor,
                                         * Each entry has value1 and value2 */
  عام:
  DEFINE_سائيز_ARRAY (10 + 3 * Types::LOffset::static_سائيز, values);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(c->check_struct (هذا)
       && coverage.sanitize (c, هذا)
       && classDef1.sanitize (c, هذا)
       && classDef2.sanitize (c, هذا))) return_trace (خطأ);

    بنانشان صحيح len1 = قدر_Format1.get_len ();
    بنانشان صحيح len2 = قدر_Format2.get_len ();
    بنانشان صحيح stride = HBUINT16::static_سائيز * (len1 + len2);
    بنانشان صحيح ڳڻپ = (بنانشان صحيح) class1_ڳڻپ_2 * (بنانشان صحيح) class2_ڳڻپ_2;
    return_trace (c->check_range ((ثابت فراغ *) values,
                                  ڳڻپ,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (قدر_Format1.sanitize_قدرون_stride_unsafe (c, هذا, &values[0], ڳڻپ, stride) &&
		    قدر_Format2.sanitize_قدرون_stride_unsafe (c, هذا, &values[len1], ڳڻپ, stride))));
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    أرجع (هذا+coverage).intersects (glyphs) &&
           (هذا+classDef2).intersects (glyphs);
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    collect_pair_variation_indices (c, هذا, هذا+coverage,
                                    هذا+classDef1, هذا+classDef2,
                                    قدر_Format1, قدر_Format2,
                                    class1_ڳڻپ_2, class2_ڳڻپ_2, values);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->داخلا_2))) أرجع;
    إذا (unlikely (!(هذا+classDef2).collect_coverage (c->داخلا_2))) أرجع;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  بنية external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t پهريون;
    hb_ot_layout_mapping_cache_t second;
  };
  فراغ *external_cache_create () ثابت
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (حجم (external_cache_t));
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      cache->پهريون.clear ();
      cache->second.clear ();
    }
    أرجع cache;
  }

  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);

    hb_بفر_t *بفر = c->بفر;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage  (بفر->cur().codepoint, cache ? &cache->coverage : خالي_اشارو);
#وإلا
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage  (بفر->cur().codepoint);
#جيڪڏهن_پورو
    إذا (اشاريو_2 == NOT_COVERED) return_trace (خطأ);

    خودڪار &skippy_iter = c->iter_داخلا;
    skippy_iter.reset_fast (بفر->idx);
    بنانشان unsafe_منزل;
    إذا (unlikely (!skippy_iter.next (&unsafe_منزل)))
    {
      بفر->unsafe_منزل_concat (بفر->idx, unsafe_منزل);
      return_trace (خطأ);
    }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    بنانشان صحيح klass1 = (هذا+classDef1).get_class (بفر->cur().codepoint, cache ? &cache->پهريون : خالي_اشارو);
    بنانشان صحيح klass2 = (هذا+classDef2).get_class (بفر->info[skippy_iter.idx].codepoint, cache ? &cache->second : خالي_اشارو);
#وإلا
    بنانشان صحيح klass1 = (هذا+classDef1).get_class (بفر->cur().codepoint);
    بنانشان صحيح klass2 = (هذا+classDef2).get_class (بفر->info[skippy_iter.idx].codepoint);
#جيڪڏهن_پورو
    إذا (unlikely (klass1 >= class1_ڳڻپ_2 || klass2 >= class2_ڳڻپ_2))
    {
      بفر->unsafe_منزل_concat (بفر->idx, skippy_iter.idx + 1);
      return_trace (خطأ);
    }

    بنانشان صحيح len1 = قدر_Format1.get_len ();
    بنانشان صحيح len2 = قدر_Format2.get_len ();
    بنانشان صحيح رڪارڊ_len = len1 + len2;

    ثابت قدر *v = &values[رڪارڊ_len * (klass1 * class2_ڳڻپ_2 + klass2)];

    منطقي applied_پهريون = خطأ, applied_ٻيو = خطأ;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#جيڪڏهن_وصف_نه_ٿيل HB_ورهايو_KERN
    إذا (خطأ)
#جيڪڏهن_پورو
    {
      إذا (!len2)
      {
        ثابت hb_direction_t dir = بفر->props.direction;
        ثابت منطقي horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        ثابت منطقي backward = HB_DIRECTION_IS_BACKWARD (dir);
        بنانشان نقاب = horizontal ? قدر_Format::xAdvance : قدر_Format::yAdvance;
        إذا (backward)
          نقاب |= نقاب >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        نقاب |= نقاب << 4;

        إذا (قدر_Format1 & ~نقاب)
          اذهب bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        إذا (قدر_Format1.apply_قدر (c, هذا, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &بفر->cur_pos().x_advance;
          hb_position_t *dst2 = &بفر->pos[skippy_iter.idx].x_advance;
          بنانشان i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          إذا (!backward)
          {
            dst1[i] = hb_saturate_جوڙيو (dst1[i], kern1);
            dst2[i] = hb_saturate_جوڙيو (dst2[i], kern2);
            dst2[i + 2] = hb_saturate_جوڙيو (dst2[i + 2], kern2);
          }
          وإلا
          {
            dst1[i] = hb_saturate_جوڙيو (dst1[i], kern1);
            dst1[i + 2] = hb_clamp_منزل<hb_position_t> ((int64_t) dst1[i + 2] + src[i + 2] - kern2);
            dst2[i] = hb_saturate_جوڙيو (dst2[i], kern2);
          }

          applied_پهريون = applied_ٻيو = kern != 0;
          اذهب success;
        }
        اذهب boring;
      }
    }
    bail:

    إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
    {
      c->بفر->پيغام (c->font,
			  "try kerning glyphs at %u,%u",
			  c->بفر->idx, skippy_iter.idx);
    }

    applied_پهريون = len1 && قدر_Format1.apply_قدر (c, هذا, v, بفر->cur_pos());
    applied_ٻيو = len2 && قدر_Format2.apply_قدر (c, هذا, v + len1, بفر->pos[skippy_iter.idx]);

    إذا (applied_پهريون || applied_ٻيو)
      إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
      {
	c->بفر->پيغام (c->font,
			    "kerned glyphs at %u,%u",
			    c->بفر->idx, skippy_iter.idx);
      }

    إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
    {
      c->بفر->پيغام (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->بفر->idx, skippy_iter.idx);
    }

    success:
    إذا (applied_پهريون || applied_ٻيو)
      بفر->unsafe_منزل_break (بفر->idx, skippy_iter.idx + 1);
    وإلا
    boring:
      بفر->unsafe_منزل_concat (بفر->idx, skippy_iter.idx + 1);

    إذا (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      بفر->unsafe_منزل_break (بفر->idx, skippy_iter.idx + 1);
    }

    بفر->idx = skippy_iter.idx;

    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);
    ٻاهريون->format = format;

    hb_map_t klass1_map;
    ٻاهريون->classDef1.serialize_subset (c, classDef1, هذا, &klass1_map, سچ, سچ, &(هذا + coverage));
    ٻاهريون->class1_ڳڻپ_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    ٻاهريون->classDef2.serialize_subset (c, classDef2, هذا, &klass2_map, سچ, خطأ);
    ٻاهريون->class2_ڳڻپ_2 = klass2_map.get_population ();

    pair_pos_class_قدرون_t class_قدرون {هذا, values, قدر_Format1, قدر_Format2,
                                         class1_ڳڻپ_2, class2_ڳڻپ_2};

    hb_pair_t<بنانشان, بنانشان> newFormats = hb_pair (قدر_Format1, قدر_Format2);

    إذا (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_قدرون.compute_effective_قدر_formats (klass1_map, klass2_map, خطأ, خطأ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    وإلا إذا (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_جدول (c->plan->ذريعو, HB_ٽيگ ('f','v','a','r'));
      منطقي has_fvar = (blob != hb_blob_get_خالي_جايون ());
      hb_blob_destroy (blob);

      منطقي strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      إذا (has_fvar && !c->plan->has_gdef_varstore)
        strip = سچ;
      newFormats = class_قدرون.compute_effective_قدر_formats (klass1_map, klass2_map, strip, سچ);
    }

    ٻاهريون->قدر_Format1 = newFormats.پهريون;
    ٻاهريون->قدر_Format2 = newFormats.second;

    إذا (!class_قدرون.serialize (c, klass1_map, klass2_map,
                                 ٻاهريون->قدر_Format1, ٻاهريون->قدر_Format2))
      return_trace (خطأ);

    منطقي ret = ٻاهريون->coverage.serialize_subset(c, coverage, هذا);
    return_trace (ٻاهريون->class1_ڳڻپ_2 && ٻاهريون->class2_ڳڻپ_2 && ret);
  }
};

}
}
}

#جيڪڏهن_پورو  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
