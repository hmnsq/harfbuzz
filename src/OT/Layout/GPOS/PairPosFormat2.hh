#که_نه_تعریف_شوی OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#تعریف_کړه OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#ضمن "ValueFormat.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

HB_دننی مضمن
فراغ collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     ثابت ارزښت_بنسټ *بنسټ,
                                     ثابت Coverage &coverage,
                                     ثابت ClassDef &class_def1,
                                     ثابت ClassDef &class_def2,
                                     ثابت ارزښت_Format &ارزښت_Format1,
                                     ثابت ارزښت_Format &ارزښت_Format2,
                                     بینښه class1_شمېر,
                                     بینښه class2_شمېر,
                                     ثابت ارزښت_ریکارډ &values)
{
  إذا (!coverage.intersects (c->glyph_سېټ) ||
      !class_def2.intersects (c->glyph_سېټ)) أرجع;
  إذا ((!ارزښت_Format1.has_device ()) && (!ارزښت_Format2.has_device ())) أرجع;

  hb_سېټ_t klass1_glyphs, klass2_glyphs;
  إذا (!class_def1.collect_coverage (&klass1_glyphs)) أرجع;
  إذا (!class_def2.collect_coverage (&klass2_glyphs)) أرجع;

  hb_سېټ_t class1_سېټ, class2_سېټ;
  لكل (ثابت بینښه cp : + c->glyph_سېټ->iter () | hb_filter (coverage))
  {
    إذا (!klass1_glyphs.has (cp)) class1_سېټ.جمع_کړه (0);
    وإلا
    {
      بینښه klass1 = class_def1.get (cp);
      class1_سېټ.جمع_کړه (klass1);
    }
  }

  class2_سېټ.جمع_کړه (0);
  لكل (ثابت بینښه cp : + c->glyph_سېټ->iter () | hb_filter (klass2_glyphs))
  {
    بینښه klass2 = class_def2.get (cp);
    class2_سېټ.جمع_کړه (klass2);
  }

  إذا (class1_سېټ.is_empty ()
      || class2_سېټ.is_empty ()
      || (class2_سېټ.get_population() == 1 && class2_سېټ.has(0)))
    أرجع;

  بینښه len1 = ارزښت_Format1.get_len ();
  بینښه len2 = ارزښت_Format2.get_len ();
  ثابت hb_array_t<ثابت ارزښت> ارزښتونه_array = values.as_array ((بینښه)class1_شمېر * (بینښه) class2_شمېر * (len1 + len2));
  لكل (ثابت بینښه class1_idx : class1_سېټ.iter ())
  {
    لكل (ثابت بینښه class2_idx : class2_سېټ.iter ())
    {
      بینښه پیل_دننه_واټن = (class1_idx * (بینښه) class2_شمېر + class2_idx) * (len1 + len2);
      إذا (ارزښت_Format1.has_device ())
        ارزښت_Format1.collect_variation_indices (c, بنسټ, ارزښتونه_array.sub_array (پیل_دننه_واټن, len1));

      إذا (ارزښت_Format2.has_device ())
        ارزښت_Format2.collect_variation_indices (c, بنسټ, ارزښتونه_array.sub_array (پیل_دننه_واټن+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
بنية pair_pos_class_ارزښتونه_t
{
  ثابت ارزښت_بنسټ *بنسټ;
  ثابت ارزښت_ریکارډ &values;
  ثابت ارزښت_Format &ارزښت_Format1;
  ثابت ارزښت_Format &ارزښت_Format2;
  بینښه class1_شمېر;
  بینښه class2_شمېر;

  hb_pair_t<بینښه, بینښه> compute_effective_ارزښت_formats (ثابت hb_map_t &klass1_map,
                                                               ثابت hb_map_t &klass2_map,
                                                               منطقي strip_hints, منطقي strip_تشه,
                                                               ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *varidx_delta_map = تش_اشاره) ثابت
  {
    بینښه len1 = ارزښت_Format1.get_len ();
    بینښه len2 = ارزښت_Format2.get_len ();
    بینښه ریکارډ_اندازه = len1 + len2;

    بینښه format1 = 0;
    بینښه format2 = 0;

    لكل (بینښه class1_idx : + hb_range (class1_شمېر) | hb_filter (klass1_map))
    {
      لكل (بینښه class2_idx : + hb_range (class2_شمېر) | hb_filter (klass2_map))
      {
        بینښه idx = (class1_idx * class2_شمېر + class2_idx) * ریکارډ_اندازه;
        format1 = format1 | ارزښت_Format1.get_effective_format (&values[idx], strip_hints, strip_تشه, بنسټ, varidx_delta_map);
        format2 = format2 | ارزښت_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_تشه, بنسټ, varidx_delta_map);
      }

      إذا (!varidx_delta_map &&
          format1 == ارزښت_Format1 && format2 == ارزښت_Format2)
        اكسر;
    }

    أرجع hb_pair (format1, format2);
  }

  منطقي serialize (hb_subset_context_t *c,
                  ثابت hb_map_t &klass1_map,
                  ثابت hb_map_t &klass2_map,
                  ارزښت_Format newFormat1,
                  ارزښت_Format newFormat2) ثابت
  {
    بینښه len1 = ارزښت_Format1.get_len ();
    بینښه len2 = ارزښت_Format2.get_len ();
    بینښه ټول_len = len1 + len2;
    hb_ویکټور_t<بینښه> class2_idxs (+ hb_range (class2_شمېر) | hb_filter (klass2_map));
    إذا (unlikely (!c->serializer->propagate_error (class2_idxs)))
      أرجع خطأ;
    لكل (بینښه class1_idx : + hb_range (class1_شمېر) | hb_filter (klass1_map))
    {
      لكل (بینښه class2_idx : class2_idxs)
      {
        بینښه idx = (class1_idx * class2_شمېر + class2_idx) * ټول_len;
        ارزښت_Format1.copy_ارزښتونه (c->serializer, newFormat1, بنسټ, &values[idx], &c->plan->layout_variation_idx_delta_map);
        ارزښت_Format2.copy_ارزښتونه (c->serializer, newFormat2, بنسټ, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    أرجع ریښتیا;
  }
};

کينډۍ <ډول_نوم Types>
بنية PairPosFormat2_4 : ارزښت_بنسټ
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ارزښت_Format   ارزښت_Format1;           /* ValueRecord definition--for the
                                         * first glyph of the pair--may be zero
                                         * (0) */
  ارزښت_Format   ارزښت_Format2;           /* ValueRecord definition--for the
                                         * second glyph of the pair--may be
                                         * zero (0) */
  ډول_نوم Types::کينډۍ LOffset_منزل<ClassDef>
                classDef1;              /* Offset to ClassDef table--from
                                         * beginning of PairPos subtable--for
                                         * the first glyph of the pair */
  ډول_نوم Types::کينډۍ LOffset_منزل<ClassDef>
                classDef2;              /* Offset to ClassDef table--from
                                         * beginning of PairPos subtable--for
                                         * the second glyph of the pair */
  HBUINT16      class1_شمېر_2;            /* Number of classes in ClassDef1
                                         * table--includes Class0 */
  HBUINT16      class2_شمېر_2;            /* Number of classes in ClassDef2
                                         * table--includes Class0 */
  ارزښت_ریکارډ   values;                 /* Matrix of value pairs:
                                         * class1-major, class2-minor,
                                         * Each entry has value1 and value2 */
  عام:
  DEFINE_اندازه_ARRAY (10 + 3 * Types::LOffset::static_اندازه, values);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(c->check_struct (هذا)
       && coverage.sanitize (c, هذا)
       && classDef1.sanitize (c, هذا)
       && classDef2.sanitize (c, هذا))) return_trace (خطأ);

    بینښه صحيح len1 = ارزښت_Format1.get_len ();
    بینښه صحيح len2 = ارزښت_Format2.get_len ();
    بینښه صحيح stride = HBUINT16::static_اندازه * (len1 + len2);
    بینښه صحيح شمېر = (بینښه صحيح) class1_شمېر_2 * (بینښه صحيح) class2_شمېر_2;
    return_trace (c->check_range ((ثابت فراغ *) values,
                                  شمېر,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (ارزښت_Format1.sanitize_ارزښتونه_stride_unsafe (c, هذا, &values[0], شمېر, stride) &&
		    ارزښت_Format2.sanitize_ارزښتونه_stride_unsafe (c, هذا, &values[len1], شمېر, stride))));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    أرجع (هذا+coverage).intersects (glyphs) &&
           (هذا+classDef2).intersects (glyphs);
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    collect_pair_variation_indices (c, هذا, هذا+coverage,
                                    هذا+classDef1, هذا+classDef2,
                                    ارزښت_Format1, ارزښت_Format2,
                                    class1_شمېر_2, class2_شمېر_2, values);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->ننوت))) أرجع;
    إذا (unlikely (!(هذا+classDef2).collect_coverage (c->ننوت))) أرجع;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  بنية external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t لومړی;
    hb_ot_layout_mapping_cache_t second;
  };
  فراغ *external_cache_create () ثابت
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (حجم (external_cache_t));
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      cache->لومړی.clear ();
      cache->second.clear ();
    }
    أرجع cache;
  }

  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);

    hb_بفر_t *بفر = c->بفر;

#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage  (بفر->cur().codepoint, cache ? &cache->coverage : تش_اشاره);
#وإلا
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage  (بفر->cur().codepoint);
#د_که_پای
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    خپلکاری &skippy_iter = c->iter_ننوت;
    skippy_iter.reset_fast (بفر->idx);
    بینښه unsafe_منزل;
    إذا (unlikely (!skippy_iter.next (&unsafe_منزل)))
    {
      بفر->unsafe_منزل_concat (بفر->idx, unsafe_منزل);
      return_trace (خطأ);
    }

#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    بینښه صحيح klass1 = (هذا+classDef1).get_class (بفر->cur().codepoint, cache ? &cache->لومړی : تش_اشاره);
    بینښه صحيح klass2 = (هذا+classDef2).get_class (بفر->info[skippy_iter.idx].codepoint, cache ? &cache->second : تش_اشاره);
#وإلا
    بینښه صحيح klass1 = (هذا+classDef1).get_class (بفر->cur().codepoint);
    بینښه صحيح klass2 = (هذا+classDef2).get_class (بفر->info[skippy_iter.idx].codepoint);
#د_که_پای
    إذا (unlikely (klass1 >= class1_شمېر_2 || klass2 >= class2_شمېر_2))
    {
      بفر->unsafe_منزل_concat (بفر->idx, skippy_iter.idx + 1);
      return_trace (خطأ);
    }

    بینښه صحيح len1 = ارزښت_Format1.get_len ();
    بینښه صحيح len2 = ارزښت_Format2.get_len ();
    بینښه صحيح ریکارډ_len = len1 + len2;

    ثابت ارزښت *v = &values[ریکارډ_len * (klass1 * class2_شمېر_2 + klass2)];

    منطقي applied_لومړی = خطأ, applied_دوهم = خطأ;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#که_نه_تعریف_شوی HB_وېشل_KERN
    إذا (خطأ)
#د_که_پای
    {
      إذا (!len2)
      {
        ثابت hb_direction_t dir = بفر->props.direction;
        ثابت منطقي horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        ثابت منطقي backward = HB_DIRECTION_IS_BACKWARD (dir);
        بینښه ماسک = horizontal ? ارزښت_Format::xAdvance : ارزښت_Format::yAdvance;
        إذا (backward)
          ماسک |= ماسک >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        ماسک |= ماسک << 4;

        إذا (ارزښت_Format1 & ~ماسک)
          اذهب bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        إذا (ارزښت_Format1.apply_ارزښت (c, هذا, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &بفر->cur_pos().x_advance;
          hb_position_t *dst2 = &بفر->pos[skippy_iter.idx].x_advance;
          بینښه i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          إذا (!backward)
          {
            dst1[i] = hb_saturate_جمع_کړه (dst1[i], kern1);
            dst2[i] = hb_saturate_جمع_کړه (dst2[i], kern2);
            dst2[i + 2] = hb_saturate_جمع_کړه (dst2[i + 2], kern2);
          }
          وإلا
          {
            dst1[i] = hb_saturate_جمع_کړه (dst1[i], kern1);
            dst1[i + 2] = hb_clamp_منزل<hb_position_t> ((int64_t) dst1[i + 2] + src[i + 2] - kern2);
            dst2[i] = hb_saturate_جمع_کړه (dst2[i], kern2);
          }

          applied_لومړی = applied_دوهم = kern != 0;
          اذهب success;
        }
        اذهب boring;
      }
    }
    bail:

    إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
    {
      c->بفر->پیغام (c->font,
			  "try kerning glyphs at %u,%u",
			  c->بفر->idx, skippy_iter.idx);
    }

    applied_لومړی = len1 && ارزښت_Format1.apply_ارزښت (c, هذا, v, بفر->cur_pos());
    applied_دوهم = len2 && ارزښت_Format2.apply_ارزښت (c, هذا, v + len1, بفر->pos[skippy_iter.idx]);

    إذا (applied_لومړی || applied_دوهم)
      إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
      {
	c->بفر->پیغام (c->font,
			    "kerned glyphs at %u,%u",
			    c->بفر->idx, skippy_iter.idx);
      }

    إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
    {
      c->بفر->پیغام (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->بفر->idx, skippy_iter.idx);
    }

    success:
    إذا (applied_لومړی || applied_دوهم)
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

    return_trace (ریښتیا);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    بهر->format = format;

    hb_map_t klass1_map;
    بهر->classDef1.serialize_subset (c, classDef1, هذا, &klass1_map, ریښتیا, ریښتیا, &(هذا + coverage));
    بهر->class1_شمېر_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    بهر->classDef2.serialize_subset (c, classDef2, هذا, &klass2_map, ریښتیا, خطأ);
    بهر->class2_شمېر_2 = klass2_map.get_population ();

    pair_pos_class_ارزښتونه_t class_ارزښتونه {هذا, values, ارزښت_Format1, ارزښت_Format2,
                                         class1_شمېر_2, class2_شمېر_2};

    hb_pair_t<بینښه, بینښه> newFormats = hb_pair (ارزښت_Format1, ارزښت_Format2);

    إذا (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_ارزښتونه.compute_effective_ارزښت_formats (klass1_map, klass2_map, خطأ, خطأ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    وإلا إذا (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_جدول (c->plan->سرچینه, HB_نښان ('f','v','a','r'));
      منطقي has_fvar = (blob != hb_blob_get_تشه ());
      hb_blob_destroy (blob);

      منطقي strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      إذا (has_fvar && !c->plan->has_gdef_varstore)
        strip = ریښتیا;
      newFormats = class_ارزښتونه.compute_effective_ارزښت_formats (klass1_map, klass2_map, strip, ریښتیا);
    }

    بهر->ارزښت_Format1 = newFormats.لومړی;
    بهر->ارزښت_Format2 = newFormats.second;

    إذا (!class_ارزښتونه.serialize (c, klass1_map, klass2_map,
                                 بهر->ارزښت_Format1, بهر->ارزښت_Format2))
      return_trace (خطأ);

    منطقي ret = بهر->coverage.serialize_subset(c, coverage, هذا);
    return_trace (بهر->class1_شمېر_2 && بهر->class2_شمېر_2 && ret);
  }
};

}
}
}

#د_که_پای  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
