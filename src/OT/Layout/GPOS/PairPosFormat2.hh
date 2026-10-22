#جے_بنا_تعریف OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#تعریف_کرو OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#ضمن "ValueFormat.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

HB_اندرونی مضمن
فراغ collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     ثابت قدر_ادھار *ادھار,
                                     ثابت Coverage &coverage,
                                     ثابت ClassDef &class_def1,
                                     ثابت ClassDef &class_def2,
                                     ثابت قدر_Format &قدر_Format1,
                                     ثابت قدر_Format &قدر_Format2,
                                     بنانشانی class1_گنتی,
                                     بنانشانی class2_گنتی,
                                     ثابت قدر_ریکارڈ &values)
{
  إذا (!coverage.intersects (c->glyph_سیٹ) ||
      !class_def2.intersects (c->glyph_سیٹ)) أرجع;
  إذا ((!قدر_Format1.has_device ()) && (!قدر_Format2.has_device ())) أرجع;

  hb_سیٹ_t klass1_glyphs, klass2_glyphs;
  إذا (!class_def1.collect_coverage (&klass1_glyphs)) أرجع;
  إذا (!class_def2.collect_coverage (&klass2_glyphs)) أرجع;

  hb_سیٹ_t class1_سیٹ, class2_سیٹ;
  لكل (ثابت بنانشانی cp : + c->glyph_سیٹ->iter () | hb_filter (coverage))
  {
    إذا (!klass1_glyphs.has (cp)) class1_سیٹ.جوڑو (0);
    وإلا
    {
      بنانشانی klass1 = class_def1.get (cp);
      class1_سیٹ.جوڑو (klass1);
    }
  }

  class2_سیٹ.جوڑو (0);
  لكل (ثابت بنانشانی cp : + c->glyph_سیٹ->iter () | hb_filter (klass2_glyphs))
  {
    بنانشانی klass2 = class_def2.get (cp);
    class2_سیٹ.جوڑو (klass2);
  }

  إذا (class1_سیٹ.is_empty ()
      || class2_سیٹ.is_empty ()
      || (class2_سیٹ.get_population() == 1 && class2_سیٹ.has(0)))
    أرجع;

  بنانشانی len1 = قدر_Format1.get_len ();
  بنانشانی len2 = قدر_Format2.get_len ();
  ثابت hb_array_t<ثابت قدر> اقدار_array = values.as_array ((بنانشانی)class1_گنتی * (بنانشانی) class2_گنتی * (len1 + len2));
  لكل (ثابت بنانشانی class1_idx : class1_سیٹ.iter ())
  {
    لكل (ثابت بنانشانی class2_idx : class2_سیٹ.iter ())
    {
      بنانشانی شروع_وتھ = (class1_idx * (بنانشانی) class2_گنتی + class2_idx) * (len1 + len2);
      إذا (قدر_Format1.has_device ())
        قدر_Format1.collect_variation_indices (c, ادھار, اقدار_array.sub_array (شروع_وتھ, len1));

      إذا (قدر_Format2.has_device ())
        قدر_Format2.collect_variation_indices (c, ادھار, اقدار_array.sub_array (شروع_وتھ+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
بنية pair_pos_class_اقدار_t
{
  ثابت قدر_ادھار *ادھار;
  ثابت قدر_ریکارڈ &values;
  ثابت قدر_Format &قدر_Format1;
  ثابت قدر_Format &قدر_Format2;
  بنانشانی class1_گنتی;
  بنانشانی class2_گنتی;

  hb_pair_t<بنانشانی, بنانشانی> compute_effective_قدر_formats (ثابت hb_map_t &klass1_map,
                                                               ثابت hb_map_t &klass2_map,
                                                               منطقي strip_hints, منطقي strip_خالی_تھاواں,
                                                               ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *varidx_delta_map = خالی_سوچک) ثابت
  {
    بنانشانی len1 = قدر_Format1.get_len ();
    بنانشانی len2 = قدر_Format2.get_len ();
    بنانشانی ریکارڈ_ناپ = len1 + len2;

    بنانشانی format1 = 0;
    بنانشانی format2 = 0;

    لكل (بنانشانی class1_idx : + hb_range (class1_گنتی) | hb_filter (klass1_map))
    {
      لكل (بنانشانی class2_idx : + hb_range (class2_گنتی) | hb_filter (klass2_map))
      {
        بنانشانی idx = (class1_idx * class2_گنتی + class2_idx) * ریکارڈ_ناپ;
        format1 = format1 | قدر_Format1.get_effective_format (&values[idx], strip_hints, strip_خالی_تھاواں, ادھار, varidx_delta_map);
        format2 = format2 | قدر_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_خالی_تھاواں, ادھار, varidx_delta_map);
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
    بنانشانی len1 = قدر_Format1.get_len ();
    بنانشانی len2 = قدر_Format2.get_len ();
    بنانشانی کل_len = len1 + len2;
    hb_سمتیہ_t<بنانشانی> class2_idxs (+ hb_range (class2_گنتی) | hb_filter (klass2_map));
    إذا (unlikely (!c->serializer->propagate_error (class2_idxs)))
      أرجع خطأ;
    لكل (بنانشانی class1_idx : + hb_range (class1_گنتی) | hb_filter (klass1_map))
    {
      لكل (بنانشانی class2_idx : class2_idxs)
      {
        بنانشانی idx = (class1_idx * class2_گنتی + class2_idx) * کل_len;
        قدر_Format1.copy_اقدار (c->serializer, newFormat1, ادھار, &values[idx], &c->plan->layout_variation_idx_delta_map);
        قدر_Format2.copy_اقدار (c->serializer, newFormat2, ادھار, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    أرجع سچ;
  }
};

سانچا <قسم_ناں Types>
بنية PairPosFormat2_4 : قدر_ادھار
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  قدر_Format   قدر_Format1;           /* ValueRecord definition--for the
                                         * first glyph of the pair--may be zero
                                         * (0) */
  قدر_Format   قدر_Format2;           /* ValueRecord definition--for the
                                         * second glyph of the pair--may be
                                         * zero (0) */
  قسم_ناں Types::سانچا LOffset_منزل<ClassDef>
                classDef1;              /* Offset to ClassDef table--from
                                         * beginning of PairPos subtable--for
                                         * the first glyph of the pair */
  قسم_ناں Types::سانچا LOffset_منزل<ClassDef>
                classDef2;              /* Offset to ClassDef table--from
                                         * beginning of PairPos subtable--for
                                         * the second glyph of the pair */
  HBUINT16      class1_گنتی_2;            /* Number of classes in ClassDef1
                                         * table--includes Class0 */
  HBUINT16      class2_گنتی_2;            /* Number of classes in ClassDef2
                                         * table--includes Class0 */
  قدر_ریکارڈ   values;                 /* Matrix of value pairs:
                                         * class1-major, class2-minor,
                                         * Each entry has value1 and value2 */
  عام:
  DEFINE_ناپ_ARRAY (10 + 3 * Types::LOffset::static_ناپ, values);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(c->check_struct (هذا)
       && coverage.sanitize (c, هذا)
       && classDef1.sanitize (c, هذا)
       && classDef2.sanitize (c, هذا))) return_trace (خطأ);

    بنانشانی صحيح len1 = قدر_Format1.get_len ();
    بنانشانی صحيح len2 = قدر_Format2.get_len ();
    بنانشانی صحيح stride = HBUINT16::static_ناپ * (len1 + len2);
    بنانشانی صحيح گنتی = (بنانشانی صحيح) class1_گنتی_2 * (بنانشانی صحيح) class2_گنتی_2;
    return_trace (c->check_range ((ثابت فراغ *) values,
                                  گنتی,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (قدر_Format1.sanitize_اقدار_stride_unsafe (c, هذا, &values[0], گنتی, stride) &&
		    قدر_Format2.sanitize_اقدار_stride_unsafe (c, هذا, &values[len1], گنتی, stride))));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    أرجع (هذا+coverage).intersects (glyphs) &&
           (هذا+classDef2).intersects (glyphs);
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    collect_pair_variation_indices (c, هذا, هذا+coverage,
                                    هذا+classDef1, هذا+classDef2,
                                    قدر_Format1, قدر_Format2,
                                    class1_گنتی_2, class2_گنتی_2, values);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->داخلہ))) أرجع;
    إذا (unlikely (!(هذا+classDef2).collect_coverage (c->داخلہ))) أرجع;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  بنية external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t پہلا;
    hb_ot_layout_mapping_cache_t second;
  };
  فراغ *external_cache_create () ثابت
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (حجم (external_cache_t));
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      cache->پہلا.clear ();
      cache->second.clear ();
    }
    أرجع cache;
  }

  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);

    hb_بفر_t *بفر = c->بفر;

#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage  (بفر->cur().codepoint, cache ? &cache->coverage : خالی_سوچک);
#وإلا
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage  (بفر->cur().codepoint);
#جے_انت
    إذا (اشاریہ_2 == NOT_COVERED) return_trace (خطأ);

    خودکار &skippy_iter = c->iter_داخلہ;
    skippy_iter.reset_fast (بفر->idx);
    بنانشانی unsafe_منزل;
    إذا (unlikely (!skippy_iter.next (&unsafe_منزل)))
    {
      بفر->unsafe_منزل_concat (بفر->idx, unsafe_منزل);
      return_trace (خطأ);
    }

#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    بنانشانی صحيح klass1 = (هذا+classDef1).get_class (بفر->cur().codepoint, cache ? &cache->پہلا : خالی_سوچک);
    بنانشانی صحيح klass2 = (هذا+classDef2).get_class (بفر->info[skippy_iter.idx].codepoint, cache ? &cache->second : خالی_سوچک);
#وإلا
    بنانشانی صحيح klass1 = (هذا+classDef1).get_class (بفر->cur().codepoint);
    بنانشانی صحيح klass2 = (هذا+classDef2).get_class (بفر->info[skippy_iter.idx].codepoint);
#جے_انت
    إذا (unlikely (klass1 >= class1_گنتی_2 || klass2 >= class2_گنتی_2))
    {
      بفر->unsafe_منزل_concat (بفر->idx, skippy_iter.idx + 1);
      return_trace (خطأ);
    }

    بنانشانی صحيح len1 = قدر_Format1.get_len ();
    بنانشانی صحيح len2 = قدر_Format2.get_len ();
    بنانشانی صحيح ریکارڈ_len = len1 + len2;

    ثابت قدر *v = &values[ریکارڈ_len * (klass1 * class2_گنتی_2 + klass2)];

    منطقي applied_پہلا = خطأ, applied_دوجا = خطأ;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#جے_بنا_تعریف HB_ونڈو_KERN
    إذا (خطأ)
#جے_انت
    {
      إذا (!len2)
      {
        ثابت hb_direction_t dir = بفر->props.direction;
        ثابت منطقي horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        ثابت منطقي backward = HB_DIRECTION_IS_BACKWARD (dir);
        بنانشانی نقاب = horizontal ? قدر_Format::xAdvance : قدر_Format::yAdvance;
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
          بنانشانی i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          إذا (!backward)
          {
            dst1[i] = hb_saturate_جوڑو (dst1[i], kern1);
            dst2[i] = hb_saturate_جوڑو (dst2[i], kern2);
            dst2[i + 2] = hb_saturate_جوڑو (dst2[i + 2], kern2);
          }
          وإلا
          {
            dst1[i] = hb_saturate_جوڑو (dst1[i], kern1);
            dst1[i + 2] = hb_clamp_منزل<hb_position_t> ((int64_t) dst1[i + 2] + src[i + 2] - kern2);
            dst2[i] = hb_saturate_جوڑو (dst2[i], kern2);
          }

          applied_پہلا = applied_دوجا = kern != 0;
          اذهب success;
        }
        اذهب boring;
      }
    }
    bail:

    إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
    {
      c->بفر->سنیہا (c->font,
			  "try kerning glyphs at %u,%u",
			  c->بفر->idx, skippy_iter.idx);
    }

    applied_پہلا = len1 && قدر_Format1.apply_قدر (c, هذا, v, بفر->cur_pos());
    applied_دوجا = len2 && قدر_Format2.apply_قدر (c, هذا, v + len1, بفر->pos[skippy_iter.idx]);

    إذا (applied_پہلا || applied_دوجا)
      إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
      {
	c->بفر->سنیہا (c->font,
			    "kerned glyphs at %u,%u",
			    c->بفر->idx, skippy_iter.idx);
      }

    إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
    {
      c->بفر->سنیہا (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->بفر->idx, skippy_iter.idx);
    }

    success:
    إذا (applied_پہلا || applied_دوجا)
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
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    باہرلا->format = format;

    hb_map_t klass1_map;
    باہرلا->classDef1.serialize_subset (c, classDef1, هذا, &klass1_map, سچ, سچ, &(هذا + coverage));
    باہرلا->class1_گنتی_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    باہرلا->classDef2.serialize_subset (c, classDef2, هذا, &klass2_map, سچ, خطأ);
    باہرلا->class2_گنتی_2 = klass2_map.get_population ();

    pair_pos_class_اقدار_t class_اقدار {هذا, values, قدر_Format1, قدر_Format2,
                                         class1_گنتی_2, class2_گنتی_2};

    hb_pair_t<بنانشانی, بنانشانی> newFormats = hb_pair (قدر_Format1, قدر_Format2);

    إذا (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_اقدار.compute_effective_قدر_formats (klass1_map, klass2_map, خطأ, خطأ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    وإلا إذا (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_جدول (c->plan->سروت, HB_ٹیگ ('f','v','a','r'));
      منطقي has_fvar = (blob != hb_blob_get_خالی_تھاواں ());
      hb_blob_destroy (blob);

      منطقي strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      إذا (has_fvar && !c->plan->has_gdef_varstore)
        strip = سچ;
      newFormats = class_اقدار.compute_effective_قدر_formats (klass1_map, klass2_map, strip, سچ);
    }

    باہرلا->قدر_Format1 = newFormats.پہلا;
    باہرلا->قدر_Format2 = newFormats.second;

    إذا (!class_اقدار.serialize (c, klass1_map, klass2_map,
                                 باہرلا->قدر_Format1, باہرلا->قدر_Format2))
      return_trace (خطأ);

    منطقي ret = باہرلا->coverage.serialize_subset(c, coverage, هذا);
    return_trace (باہرلا->class1_گنتی_2 && باہرلا->class2_گنتی_2 && ret);
  }
};

}
}
}

#جے_انت  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
