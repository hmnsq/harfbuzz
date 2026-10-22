#اگر_تعریف_نشده OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
#تعریف_کن OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH

#ضمن "ValueFormat.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

HB_داخلی مضمن
فراغ collect_pair_variation_indices (hb_collect_variation_indices_context_t *c,
                                     ثابت مقدار_پایه *پایه,
                                     ثابت Coverage &coverage,
                                     ثابت ClassDef &class_def1,
                                     ثابت ClassDef &class_def2,
                                     ثابت مقدار_Format &مقدار_Format1,
                                     ثابت مقدار_Format &مقدار_Format2,
                                     بیعلامت class1_شمار,
                                     بیعلامت class2_شمار,
                                     ثابت مقدار_رکورد &values)
{
  إذا (!coverage.intersects (c->glyph_مجموعه) ||
      !class_def2.intersects (c->glyph_مجموعه)) أرجع;
  إذا ((!مقدار_Format1.has_device ()) && (!مقدار_Format2.has_device ())) أرجع;

  hb_مجموعه_t klass1_glyphs, klass2_glyphs;
  إذا (!class_def1.collect_coverage (&klass1_glyphs)) أرجع;
  إذا (!class_def2.collect_coverage (&klass2_glyphs)) أرجع;

  hb_مجموعه_t class1_مجموعه, class2_مجموعه;
  لكل (ثابت بیعلامت cp : + c->glyph_مجموعه->iter () | hb_filter (coverage))
  {
    إذا (!klass1_glyphs.has (cp)) class1_مجموعه.جمع_کن (0);
    وإلا
    {
      بیعلامت klass1 = class_def1.get (cp);
      class1_مجموعه.جمع_کن (klass1);
    }
  }

  class2_مجموعه.جمع_کن (0);
  لكل (ثابت بیعلامت cp : + c->glyph_مجموعه->iter () | hb_filter (klass2_glyphs))
  {
    بیعلامت klass2 = class_def2.get (cp);
    class2_مجموعه.جمع_کن (klass2);
  }

  إذا (class1_مجموعه.is_empty ()
      || class2_مجموعه.is_empty ()
      || (class2_مجموعه.get_population() == 1 && class2_مجموعه.has(0)))
    أرجع;

  بیعلامت len1 = مقدار_Format1.get_len ();
  بیعلامت len2 = مقدار_Format2.get_len ();
  ثابت hb_array_t<ثابت مقدار> مقادیر_array = values.as_array ((بیعلامت)class1_شمار * (بیعلامت) class2_شمار * (len1 + len2));
  لكل (ثابت بیعلامت class1_idx : class1_مجموعه.iter ())
  {
    لكل (ثابت بیعلامت class2_idx : class2_مجموعه.iter ())
    {
      بیعلامت شروع_جابه_جایی = (class1_idx * (بیعلامت) class2_شمار + class2_idx) * (len1 + len2);
      إذا (مقدار_Format1.has_device ())
        مقدار_Format1.collect_variation_indices (c, پایه, مقادیر_array.sub_array (شروع_جابه_جایی, len1));

      إذا (مقدار_Format2.has_device ())
        مقدار_Format2.collect_variation_indices (c, پایه, مقادیر_array.sub_array (شروع_جابه_جایی+len1, len2));
    }
  }
}

/* Keep class-matrix traversal independent of the source table width. */
بنية pair_pos_class_مقادیر_t
{
  ثابت مقدار_پایه *پایه;
  ثابت مقدار_رکورد &values;
  ثابت مقدار_Format &مقدار_Format1;
  ثابت مقدار_Format &مقدار_Format2;
  بیعلامت class1_شمار;
  بیعلامت class2_شمار;

  hb_pair_t<بیعلامت, بیعلامت> compute_effective_مقدار_formats (ثابت hb_map_t &klass1_map,
                                                               ثابت hb_map_t &klass2_map,
                                                               منطقي strip_hints, منطقي strip_خالی,
                                                               ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *varidx_delta_map = اشاره_تهی) ثابت
  {
    بیعلامت len1 = مقدار_Format1.get_len ();
    بیعلامت len2 = مقدار_Format2.get_len ();
    بیعلامت رکورد_اندازه = len1 + len2;

    بیعلامت format1 = 0;
    بیعلامت format2 = 0;

    لكل (بیعلامت class1_idx : + hb_range (class1_شمار) | hb_filter (klass1_map))
    {
      لكل (بیعلامت class2_idx : + hb_range (class2_شمار) | hb_filter (klass2_map))
      {
        بیعلامت idx = (class1_idx * class2_شمار + class2_idx) * رکورد_اندازه;
        format1 = format1 | مقدار_Format1.get_effective_format (&values[idx], strip_hints, strip_خالی, پایه, varidx_delta_map);
        format2 = format2 | مقدار_Format2.get_effective_format (&values[idx + len1], strip_hints, strip_خالی, پایه, varidx_delta_map);
      }

      إذا (!varidx_delta_map &&
          format1 == مقدار_Format1 && format2 == مقدار_Format2)
        اكسر;
    }

    أرجع hb_pair (format1, format2);
  }

  منطقي serialize (hb_subset_context_t *c,
                  ثابت hb_map_t &klass1_map,
                  ثابت hb_map_t &klass2_map,
                  مقدار_Format newFormat1,
                  مقدار_Format newFormat2) ثابت
  {
    بیعلامت len1 = مقدار_Format1.get_len ();
    بیعلامت len2 = مقدار_Format2.get_len ();
    بیعلامت مجموع_len = len1 + len2;
    hb_وکتور_t<بیعلامت> class2_idxs (+ hb_range (class2_شمار) | hb_filter (klass2_map));
    إذا (unlikely (!c->serializer->propagate_error (class2_idxs)))
      أرجع خطأ;
    لكل (بیعلامت class1_idx : + hb_range (class1_شمار) | hb_filter (klass1_map))
    {
      لكل (بیعلامت class2_idx : class2_idxs)
      {
        بیعلامت idx = (class1_idx * class2_شمار + class2_idx) * مجموع_len;
        مقدار_Format1.copy_مقادیر (c->serializer, newFormat1, پایه, &values[idx], &c->plan->layout_variation_idx_delta_map);
        مقدار_Format2.copy_مقادیر (c->serializer, newFormat2, پایه, &values[idx + len1], &c->plan->layout_variation_idx_delta_map);
      }
    }

    أرجع درست;
  }
};

قالب <نام_نوع Types>
بنية PairPosFormat2_4 : مقدار_پایه
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  مقدار_Format   مقدار_Format1;           /* ValueRecord definition--for the
                                         * first glyph of the pair--may be zero
                                         * (0) */
  مقدار_Format   مقدار_Format2;           /* ValueRecord definition--for the
                                         * second glyph of the pair--may be
                                         * zero (0) */
  نام_نوع Types::قالب LOffset_مقصد<ClassDef>
                classDef1;              /* Offset to ClassDef table--from
                                         * beginning of PairPos subtable--for
                                         * the first glyph of the pair */
  نام_نوع Types::قالب LOffset_مقصد<ClassDef>
                classDef2;              /* Offset to ClassDef table--from
                                         * beginning of PairPos subtable--for
                                         * the second glyph of the pair */
  HBUINT16      class1_شمار_2;            /* Number of classes in ClassDef1
                                         * table--includes Class0 */
  HBUINT16      class2_شمار_2;            /* Number of classes in ClassDef2
                                         * table--includes Class0 */
  مقدار_رکورد   values;                 /* Matrix of value pairs:
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

    بیعلامت صحيح len1 = مقدار_Format1.get_len ();
    بیعلامت صحيح len2 = مقدار_Format2.get_len ();
    بیعلامت صحيح stride = HBUINT16::static_اندازه * (len1 + len2);
    بیعلامت صحيح شمار = (بیعلامت صحيح) class1_شمار_2 * (بیعلامت صحيح) class2_شمار_2;
    return_trace (c->check_range ((ثابت فراغ *) values,
                                  شمار,
                                  stride) &&
		  (c->lazy_some_gpos ||
		   (مقدار_Format1.sanitize_مقادیر_stride_unsafe (c, هذا, &values[0], شمار, stride) &&
		    مقدار_Format2.sanitize_مقادیر_stride_unsafe (c, هذا, &values[len1], شمار, stride))));
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    أرجع (هذا+coverage).intersects (glyphs) &&
           (هذا+classDef2).intersects (glyphs);
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    collect_pair_variation_indices (c, هذا, هذا+coverage,
                                    هذا+classDef1, هذا+classDef2,
                                    مقدار_Format1, مقدار_Format2,
                                    class1_شمار_2, class2_شمار_2, values);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->درون_داد))) أرجع;
    إذا (unlikely (!(هذا+classDef2).collect_coverage (c->درون_داد))) أرجع;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  بنية external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ot_layout_mapping_cache_t اولی;
    hb_ot_layout_mapping_cache_t second;
  };
  فراغ *external_cache_create () ثابت
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (حجم (external_cache_t));
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      cache->اولی.clear ();
      cache->second.clear ();
    }
    أرجع cache;
  }

  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);

    hb_میانگیر_t *میانگیر = c->میانگیر;

#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage  (میانگیر->cur().codepoint, cache ? &cache->coverage : اشاره_تهی);
#وإلا
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage  (میانگیر->cur().codepoint);
#پایان_اگر
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    خودکار &skippy_iter = c->iter_درون_داد;
    skippy_iter.reset_fast (میانگیر->idx);
    بیعلامت unsafe_مقصد;
    إذا (unlikely (!skippy_iter.next (&unsafe_مقصد)))
    {
      میانگیر->unsafe_مقصد_concat (میانگیر->idx, unsafe_مقصد);
      return_trace (خطأ);
    }

#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    بیعلامت صحيح klass1 = (هذا+classDef1).get_class (میانگیر->cur().codepoint, cache ? &cache->اولی : اشاره_تهی);
    بیعلامت صحيح klass2 = (هذا+classDef2).get_class (میانگیر->info[skippy_iter.idx].codepoint, cache ? &cache->second : اشاره_تهی);
#وإلا
    بیعلامت صحيح klass1 = (هذا+classDef1).get_class (میانگیر->cur().codepoint);
    بیعلامت صحيح klass2 = (هذا+classDef2).get_class (میانگیر->info[skippy_iter.idx].codepoint);
#پایان_اگر
    إذا (unlikely (klass1 >= class1_شمار_2 || klass2 >= class2_شمار_2))
    {
      میانگیر->unsafe_مقصد_concat (میانگیر->idx, skippy_iter.idx + 1);
      return_trace (خطأ);
    }

    بیعلامت صحيح len1 = مقدار_Format1.get_len ();
    بیعلامت صحيح len2 = مقدار_Format2.get_len ();
    بیعلامت صحيح رکورد_len = len1 + len2;

    ثابت مقدار *v = &values[رکورد_len * (klass1 * class2_شمار_2 + klass2)];

    منطقي applied_اولی = خطأ, applied_دومی = خطأ;


    /* Isolate simple kerning and apply it half to each side.
     * Results in better cursor positioning / underline drawing.
     *
     * Disabled, because causes issues... :-(
     * https://github.com/harfbuzz/harfbuzz/issues/3408
     * https://github.com/harfbuzz/harfbuzz/pull/3235#issuecomment-1029814978
     */
#اگر_تعریف_نشده HB_تقسیم_KERN
    إذا (خطأ)
#پایان_اگر
    {
      إذا (!len2)
      {
        ثابت hb_direction_t dir = میانگیر->props.direction;
        ثابت منطقي horizontal = HB_DIRECTION_IS_HORIZONTAL (dir);
        ثابت منطقي backward = HB_DIRECTION_IS_BACKWARD (dir);
        بیعلامت ماسک = horizontal ? مقدار_Format::xAdvance : مقدار_Format::yAdvance;
        إذا (backward)
          ماسک |= ماسک >> 2; /* Add eg. xPlacement in RTL. */
        /* Add Devices. */
        ماسک |= ماسک << 4;

        إذا (مقدار_Format1 & ~ماسک)
          اذهب bail;

        /* Is simple kern. Apply value on an empty position slot,
         * then split it between sides. */

        hb_glyph_position_t pos{};
        إذا (مقدار_Format1.apply_مقدار (c, هذا, v, pos))
        {
          hb_position_t *src  = &pos.x_advance;
          hb_position_t *dst1 = &میانگیر->cur_pos().x_advance;
          hb_position_t *dst2 = &میانگیر->pos[skippy_iter.idx].x_advance;
          بیعلامت i = horizontal ? 0 : 1;

          hb_position_t kern  = src[i];
          hb_position_t kern1 = kern >> 1;
          hb_position_t kern2 = kern - kern1;

          إذا (!backward)
          {
            dst1[i] = hb_saturate_جمع_کن (dst1[i], kern1);
            dst2[i] = hb_saturate_جمع_کن (dst2[i], kern2);
            dst2[i + 2] = hb_saturate_جمع_کن (dst2[i + 2], kern2);
          }
          وإلا
          {
            dst1[i] = hb_saturate_جمع_کن (dst1[i], kern1);
            dst1[i + 2] = hb_clamp_مقصد<hb_position_t> ((int64_t) dst1[i + 2] + src[i + 2] - kern2);
            dst2[i] = hb_saturate_جمع_کن (dst2[i], kern2);
          }

          applied_اولی = applied_دومی = kern != 0;
          اذهب success;
        }
        اذهب boring;
      }
    }
    bail:

    إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
    {
      c->میانگیر->پیام (c->font,
			  "try kerning glyphs at %u,%u",
			  c->میانگیر->idx, skippy_iter.idx);
    }

    applied_اولی = len1 && مقدار_Format1.apply_مقدار (c, هذا, v, میانگیر->cur_pos());
    applied_دومی = len2 && مقدار_Format2.apply_مقدار (c, هذا, v + len1, میانگیر->pos[skippy_iter.idx]);

    إذا (applied_اولی || applied_دومی)
      إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
      {
	c->میانگیر->پیام (c->font,
			    "kerned glyphs at %u,%u",
			    c->میانگیر->idx, skippy_iter.idx);
      }

    إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
    {
      c->میانگیر->پیام (c->font,
			  "tried kerning glyphs at %u,%u",
			  c->میانگیر->idx, skippy_iter.idx);
    }

    success:
    إذا (applied_اولی || applied_دومی)
      میانگیر->unsafe_مقصد_break (میانگیر->idx, skippy_iter.idx + 1);
    وإلا
    boring:
      میانگیر->unsafe_مقصد_concat (میانگیر->idx, skippy_iter.idx + 1);

    إذا (len2)
    {
      skippy_iter.idx++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      میانگیر->unsafe_مقصد_break (میانگیر->idx, skippy_iter.idx + 1);
    }

    میانگیر->idx = skippy_iter.idx;

    return_trace (درست);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    بیرون->format = format;

    hb_map_t klass1_map;
    بیرون->classDef1.serialize_subset (c, classDef1, هذا, &klass1_map, درست, درست, &(هذا + coverage));
    بیرون->class1_شمار_2 = klass1_map.get_population ();

    hb_map_t klass2_map;
    بیرون->classDef2.serialize_subset (c, classDef2, هذا, &klass2_map, درست, خطأ);
    بیرون->class2_شمار_2 = klass2_map.get_population ();

    pair_pos_class_مقادیر_t class_مقادیر {هذا, values, مقدار_Format1, مقدار_Format2,
                                         class1_شمار_2, class2_شمار_2};

    hb_pair_t<بیعلامت, بیعلامت> newFormats = hb_pair (مقدار_Format1, مقدار_Format2);

    إذا (c->plan->normalized_coords)
    {
      /* in case of full instancing, all var device flags will be dropped so no
       * need to strip hints here */
      newFormats = class_مقادیر.compute_effective_مقدار_formats (klass1_map, klass2_map, خطأ, خطأ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    وإلا إذا (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_جدول (c->plan->منبع, HB_برچسب ('f','v','a','r'));
      منطقي has_fvar = (blob != hb_blob_get_خالی ());
      hb_blob_destroy (blob);

      منطقي strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      إذا (has_fvar && !c->plan->has_gdef_varstore)
        strip = درست;
      newFormats = class_مقادیر.compute_effective_مقدار_formats (klass1_map, klass2_map, strip, درست);
    }

    بیرون->مقدار_Format1 = newFormats.اولی;
    بیرون->مقدار_Format2 = newFormats.second;

    إذا (!class_مقادیر.serialize (c, klass1_map, klass2_map,
                                 بیرون->مقدار_Format1, بیرون->مقدار_Format2))
      return_trace (خطأ);

    منطقي ret = بیرون->coverage.serialize_subset(c, coverage, هذا);
    return_trace (بیرون->class1_شمار_2 && بیرون->class2_شمار_2 && ret);
  }
};

}
}
}

#پایان_اگر  // OT_LAYOUT_GPOS_PAIRPOSFORMAT2_HH
