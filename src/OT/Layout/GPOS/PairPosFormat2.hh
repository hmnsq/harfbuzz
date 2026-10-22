#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#تعریف_کرو OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#شامل "ValueFormat.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

HB_اندرونی اندرونی
خالی collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     ثابت قدر_بنیاد *بنیاد,
                                     ثابت Coverage &coverage,
                                     ثابت ClassDef &class_def1,
                                     ثابت ClassDef &class_def2,
                                     ثابت قدر_Format &قدر_Format1,
                                     ثابت قدر_Format &قدر_Format2,
                                     غیرعلامتی class1_گنتی,
                                     غیرعلامتی class2_گنتی,
                                     ثابت قدر_ریکارڈ &values)
{
  اگر (!coverage.intersects (c->glyph_سیٹ) ||
      !class_def2.intersects (c->glyph_سیٹ)) واپس;
  اگر ((!قدر_Format1.has_device ()) && (!قدر_Format2.has_device ())) واپس;

  hb_سیٹ_t klass1_glyphs, klass2_glyphs;
  اگر (!class_def1.collect_coverage (&klass1_glyphs)) واپس;
  اگر (!class_def2.collect_coverage (&klass2_glyphs)) واپس;

  hb_سیٹ_t class1_سیٹ, class2_سیٹ;
  برائے (ثابت غیرعلامتی cp : + c->glyph_سیٹ->iter () | hb_filter (coverage))
  {
    اگر (!klass1_glyphs.has (cp)) class1_سیٹ.جوڑو (0);
    ورنہ
    {
      غیرعلامتی klass1 = class_def1.get (cp);
      class1_سیٹ.جوڑو (klass1);
    }
  }

  class2_سیٹ.جوڑو (0);
  برائے (ثابت غیرعلامتی cp : + c->glyph_سیٹ->iter () | hb_filter (klass2_glyphs))
  {
    غیرعلامتی klass2 = class_def2.get (cp);
    class2_سیٹ.جوڑو (klass2);
  }

  اگر (class1_سیٹ.is_empty ()
      || class2_سیٹ.is_empty ()
      || (class2_سیٹ.get_population() == 1 && class2_سیٹ.has(0)))
    واپس;

  غیرعلامتی len1 = قدر_Format1.get_len ();
  غیرعلامتی len2 = قدر_Format2.get_len ();
  ثابت hb_array_t<ثابت قدر> اقدار_array = values.as_array ((غیرعلامتی)class1_گنتی * (غیرعلامتی) class2_گنتی * (len1 + len2));
  برائے (ثابت غیرعلامتی class1_idx : class1_سیٹ.iter ())
  {
    برائے (ثابت غیرعلامتی class2_idx : class2_سیٹ.iter ())
    {
      غیرعلامتی آغاز_انحراف = (class1_idx * (غیرعلامتی) class2_گنتی + class2_idx) * (len1 + len2);
      اگر (قدر_Format1.has_device ())
        قدر_Format1.collect_variation_indices (c, بنیاد, اقدار_array.sub_array (آغاز_انحراف, len1));

      اگر (قدر_Format2.has_device ())
        قدر_Format2.collect_variation_indices (c, بنیاد, اقدار_array.sub_array (آغاز_انحراف+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
ساخت pair_pos_class_اقدار_t
{
  ثابت قدر_بنیاد *بنیاد;
  ثابت قدر_ریکارڈ &values;
  ثابت قدر_Format &قدر_Format1;
  ثابت قدر_Format &قدر_Format2;
  غیرعلامتی class1_گنتی;
  غیرعلامتی class2_گنتی;

  hb_pair_t<غیرعلامتی, غیرعلامتی> compute_effective_قدر_formats (ثابت hb_map_t &klass1_map,
                                                               ثابت hb_map_t &klass2_map,
                                                               منطقی strip_hints, منطقی strip_خالی_خانے,
                                                               ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *varidx_delta_map = خالی_اشاریہ) ثابت
  {
    غیرعلامتی len1 = قدر_Format1.get_len ();
    غیرعلامتی len2 = قدر_Format2.get_len ();
    غیرعلامتی ریکارڈ_سائز = len1 + len2;

    غیرعلامتی format1 = 0;
    غیرعلامتی format2 = 0;

    برائے (غیرعلامتی class1_idx : + hb_range (class1_گنتی) | hb_filter (klass1_map))
    {
      برائے (غیرعلامتی class2_idx : + hb_range (class2_گنتی) | hb_filter (klass2_map))
      {
        غیرعلامتی idx = (class1_idx * class2_گنتی + class2_idx) * ریکارڈ_سائز;
        format1 = format1 | قدر_Format1.get_effective_format (&values[idx], strip_hints, strip_خالی_خانے, بنیاد, varidx_delta_map);
        format2 = format2 | قدر_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_خالی_خانے, بنیاد, varidx_delta_map);
      }

      اگر (!varidx_delta_map &&
          format1 == قدر_Format1 && format2 == قدر_Format2)
        توڑو;
    }

    واپس hb_pair (format1, format2);
  }

  منطقی serialize (hb_subset_context_t *c,
                  ثابت hb_map_t &klass1_map,
                  ثابت hb_map_t &klass2_map,
                  قدر_Format newFormat1,
                  قدر_Format newFormat2) ثابت
  {
    غیرعلامتی len1 = قدر_Format1.get_len ();
    غیرعلامتی len2 = قدر_Format2.get_len ();
    غیرعلامتی کل_len = len1 + len2;
    hb_سمتیہ_t<غیرعلامتی> class2_idxs (+ hb_range (class2_گنتی) | hb_filter (klass2_map));
    اگر (unlikely (!c->serializer->propagate_error (class2_idxs)))
      واپس جھوٹ;
    برائے (غیرعلامتی class1_idx : + hb_range (class1_گنتی) | hb_filter (klass1_map))
    {
      برائے (غیرعلامتی class2_idx : class2_idxs)
      {
        غیرعلامتی idx = (class1_idx * class2_گنتی + class2_idx) * کل_len;
        قدر_Format1.copy_اقدار (c->serializer, newFormat1, بنیاد, &values[idx], &c->plan->layout_variation_idx_delta_map);
        قدر_Format2.copy_اقدار (c->serializer, newFormat2, بنیاد, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    واپس سچ;
  }
};

سانچہ <قسم_نام Types>
ساخت PairPosFormat2_4 : قدر_بنیاد
{
  محفوظ:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  قدر_Format   قدر_Format1;           /* ValueRecord definition--for the
                                         * first glyph of the pair--may be zero
                                         * (0) */
  قدر_Format   قدر_Format2;           /* ValueRecord definition--for the
                                         * second glyph of the pair--may be
                                         * zero (0) */
  قسم_نام Types::سانچہ LOffset_مقصد<ClassDef>
                classDef1;              /* Offset to ClassDef table--from
                                         * beginning of PairPos subtable--for
                                         * the first glyph of the pair */
  قسم_نام Types::سانچہ LOffset_مقصد<ClassDef>
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
  عوامی:
  DEFINE_سائز_ARRAY (10 + 3 * Types::LOffset::static_سائز, values);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!(c->check_struct (یہ)
       && coverage.sanitize (c, یہ)
       && classDef1.sanitize (c, یہ)
       && classDef2.sanitize (c, یہ))) return_trace (جھوٹ);

    غیرعلامتی صحیح len1 = قدر_Format1.get_len ();
    غیرعلامتی صحیح len2 = قدر_Format2.get_len ();
    غیرعلامتی صحیح stride = HBUINT16::static_سائز * (len1 + len2);
    غیرعلامتی صحیح گنتی = (غیرعلامتی صحیح) class1_گنتی_2 * (غیرعلامتی صحیح) class2_گنتی_2;
    return_trace (c->check_range ((ثابت خالی *) values,
                                  گنتی,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (قدر_Format1.sanitize_اقدار_stride_unsafe (c, یہ, &values[0], گنتی, stride) &&
		    قدر_Format2.sanitize_اقدار_stride_unsafe (c, یہ, &values[len1], گنتی, stride))));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    واپس (یہ+coverage).intersects (glyphs) &&
           (یہ+classDef2).intersects (glyphs);
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت {}
  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    collect_pair_variation_indices (c, یہ, یہ+coverage,
                                    یہ+classDef1, یہ+classDef2,
                                    قدر_Format1, قدر_Format2,
                                    class1_گنتی_2, class2_گنتی_2, values);
  }

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(یہ+coverage).collect_coverage (c->داخلہ))) واپس;
    اگر (unlikely (!(یہ+classDef2).collect_coverage (c->داخلہ))) واپس;
  }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+coverage; }

  ساخت external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t پہلا;
    hb_ot_layout_mapping_cache_t second;
  };
  خالی *external_cache_create () ثابت
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (حجم (external_cache_t));
    اگر (likely (cache))
    {
      cache->coverage.clear ();
      cache->پہلا.clear ();
      cache->second.clear ();
    }
    واپس cache;
  }

  منطقی apply (hb_ot_apply_context_t *c, خالی *external_cache) ثابت
  {
    TRACE_APPLY (یہ);

    hb_بفر_t *بفر = c->بفر;

#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage  (بفر->cur().codepoint, cache ? &cache->coverage : خالی_اشاریہ);
#ورنہ
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage  (بفر->cur().codepoint);
#اگر_ختم
    اگر (اشاریہ_2 == NOT_COVERED) return_trace (جھوٹ);

    خودکار &skippy_iter = c->iter_داخلہ;
    skippy_iter.reset_fast (بفر->idx);
    غیرعلامتی unsafe_مقصد;
    اگر (unlikely (!skippy_iter.next (&unsafe_مقصد)))
    {
      بفر->unsafe_مقصد_concat (بفر->idx, unsafe_مقصد);
      return_trace (جھوٹ);
    }

#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    غیرعلامتی صحیح klass1 = (یہ+classDef1).get_class (بفر->cur().codepoint, cache ? &cache->پہلا : خالی_اشاریہ);
    غیرعلامتی صحیح klass2 = (یہ+classDef2).get_class (بفر->info[skippy_iter.idx].codepoint, cache ? &cache->second : خالی_اشاریہ);
#ورنہ
    غیرعلامتی صحیح klass1 = (یہ+classDef1).get_class (بفر->cur().codepoint);
    غیرعلامتی صحیح klass2 = (یہ+classDef2).get_class (بفر->info[skippy_iter.idx].codepoint);
#اگر_ختم
    اگر (unlikely (klass1 >= class1_گنتی_2 || klass2 >= class2_گنتی_2))
    {
      بفر->unsafe_مقصد_concat (بفر->idx, skippy_iter.idx + 1);
      return_trace (جھوٹ);
    }

    غیرعلامتی صحیح len1 = قدر_Format1.get_len ();
    غیرعلامتی صحیح len2 = قدر_Format2.get_len ();
    غیرعلامتی صحیح ریکارڈ_len = len1 + len2;

    ثابت قدر *v = &values[ریکارڈ_len * (klass1 * class2_گنتی_2 + klass2)];

    منطقی applied_پہلا = جھوٹ, applied_دوسرا = جھوٹ;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#اگر_غیر_تعریف_شدہ HB_بانٹو_KERN
    اگر (جھوٹ)
#اگر_ختم
    {
      اگر (!len2)
      {
        ثابت hb_direction_t dir = بفر->props.direction;
        ثابت منطقی horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        ثابت منطقی backward = HB_DIRECTION_IS_BACKWARD (dir);
        غیرعلامتی نقاب = horizontal ? قدر_Format::xAdvance : قدر_Format::yAdvance;
        اگر (backward)
          نقاب |= نقاب >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        نقاب |= نقاب << 4;

        اگر (قدر_Format1 & ~نقاب)
          جاؤ bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        اگر (قدر_Format1.apply_قدر (c, یہ, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &بفر->cur_pos().x_advance;
          hb_position_t *dst2 = &بفر->pos[skippy_iter.idx].x_advance;
          غیرعلامتی i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          اگر (!backward)
          {
            dst1[i] = hb_saturate_جوڑو (dst1[i], kern1);
            dst2[i] = hb_saturate_جوڑو (dst2[i], kern2);
            dst2[i + 2] = hb_saturate_جوڑو (dst2[i + 2], kern2);
          }
          ورنہ
          {
            dst1[i] = hb_saturate_جوڑو (dst1[i], kern1);
            dst1[i + 2] = hb_clamp_مقصد<hb_position_t> ((int64_t) dst1[i + 2] + src[i + 2] - kern2);
            dst2[i] = hb_saturate_جوڑو (dst2[i], kern2);
          }

          applied_پہلا = applied_دوسرا = kern != 0;
          جاؤ success;
        }
        جاؤ boring;
      }
    }
    bail:

    اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
    {
      c->بفر->پیغام (c->font,
			  "try kerning glyphs at %u,%u",
			  c->بفر->idx, skippy_iter.idx);
    }

    applied_پہلا = len1 && قدر_Format1.apply_قدر (c, یہ, v, بفر->cur_pos());
    applied_دوسرا = len2 && قدر_Format2.apply_قدر (c, یہ, v + len1, بفر->pos[skippy_iter.idx]);

    اگر (applied_پہلا || applied_دوسرا)
      اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
      {
	c->بفر->پیغام (c->font,
			    "kerned glyphs at %u,%u",
			    c->بفر->idx, skippy_iter.idx);
      }

    اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
    {
      c->بفر->پیغام (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->بفر->idx, skippy_iter.idx);
    }

    success:
    اگر (applied_پہلا || applied_دوسرا)
      بفر->unsafe_مقصد_break (بفر->idx, skippy_iter.idx + 1);
    ورنہ
    boring:
      بفر->unsafe_مقصد_concat (بفر->idx, skippy_iter.idx + 1);

    اگر (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      بفر->unsafe_مقصد_break (بفر->idx, skippy_iter.idx + 1);
    }

    بفر->idx = skippy_iter.idx;

    return_trace (سچ);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);
    برآمد->format = format;

    hb_map_t klass1_map;
    برآمد->classDef1.serialize_subset (c, classDef1, یہ, &klass1_map, سچ, سچ, &(یہ + coverage));
    برآمد->class1_گنتی_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    برآمد->classDef2.serialize_subset (c, classDef2, یہ, &klass2_map, سچ, جھوٹ);
    برآمد->class2_گنتی_2 = klass2_map.get_population ();

    pair_pos_class_اقدار_t class_اقدار {یہ, values, قدر_Format1, قدر_Format2,
                                         class1_گنتی_2, class2_گنتی_2};

    hb_pair_t<غیرعلامتی, غیرعلامتی> newFormats = hb_pair (قدر_Format1, قدر_Format2);

    اگر (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_اقدار.compute_effective_قدر_formats (klass1_map, klass2_map, جھوٹ, جھوٹ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    ورنہ اگر (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_جدول (c->plan->ماخذ, HB_لیبل ('f','v','a','r'));
      منطقی has_fvar = (blob != hb_blob_get_خالی_خانے ());
      hb_blob_destroy (blob);

      منطقی strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      اگر (has_fvar && !c->plan->has_gdef_varstore)
        strip = سچ;
      newFormats = class_اقدار.compute_effective_قدر_formats (klass1_map, klass2_map, strip, سچ);
    }

    برآمد->قدر_Format1 = newFormats.پہلا;
    برآمد->قدر_Format2 = newFormats.second;

    اگر (!class_اقدار.serialize (c, klass1_map, klass2_map,
                                 برآمد->قدر_Format1, برآمد->قدر_Format2))
      return_trace (جھوٹ);

    منطقی ret = برآمد->coverage.serialize_subset(c, coverage, یہ);
    return_trace (برآمد->class1_گنتی_2 && برآمد->class2_گنتی_2 && ret);
  }
};

}
}
}

#اگر_ختم  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
