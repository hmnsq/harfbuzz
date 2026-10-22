#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#অন্তর্ভাব "PairValueRecord.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
কাঠামো pair_set_মানসমূহ_t
{
  ধ্রুবক মান_Base *base;
  glyph_তথ্য_t glyphs;
  ধ্রুবক মান *values;
  unsigned গুনতি;
  unsigned record_আকার;

  unsigned get_glyph (unsigned i) ধ্রুবক
  {
    glyph_তথ্য_t glyph = glyphs;
    glyph.তথ্য = (ধ্রুবক অক্ষর *) glyph.তথ্য + i * record_আকার;
    ফেরত glyph[0];
  }

  ধ্রুবক মান *get_মানসমূহ (unsigned i) ধ্রুবক
  { ফেরত reinterpret_cast<ধ্রুবক মান *> ((ধ্রুবক অক্ষর *) values + i * record_আকার); }

  hb_pair_t<unsigned, unsigned>
  compute_effective_মান_formats (ধ্রুবক hb_set_t &glyphset,
                                   ধ্রুবক মান_Format মান_Formats[2],
                                   যৌক্তিক strip_hints, যৌক্তিক strip_empty,
                                   ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *varidx_delta_map) ধ্রুবক
  {
    unsigned len1 = মান_Formats[0].get_len ();
    unsigned format1 = 0, format2 = 0;
    জন্য (unsigned i = 0; i < গুনতি; i++)
    {
      যদি (!glyphset.has (get_glyph (i))) চালু;
      ধ্রুবক মান *v = get_মানসমূহ (i);
      format1 |= মান_Formats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= মান_Formats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    ফেরত hb_pair (format1, format2);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ধ্রুবক মান_Format মান_Formats[2]) ধ্রুবক
  {
    unsigned len1 = মান_Formats[0].get_len ();
    unsigned len2 = মান_Formats[1].get_len ();
    জন্য (unsigned i = 0; i < গুনতি; i++)
    {
      যদি (!c->glyph_set->has (get_glyph (i))) চালু;
      ধ্রুবক hb_array_t<ধ্রুবক মান> v = hb_array (get_মানসমূহ (i), len1 + len2);
      যদি (মান_Formats[0].has_device ())
        মান_Formats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      যদি (মান_Formats[1].has_device ())
        মান_Formats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

template <typename Types>
কাঠামো PairSet : মান_Base
{
  template <typename Types2>
  friend কাঠামো PairPosFormat1_3;

  using Pair_মান_Record = GPOS_impl::Pair_মান_Record<Types>;

  সুরক্ষিত:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_মান_Record       first_Pair_মান_Record;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  সর্বজনীন:
  DEFINE_আকার_MIN (Types::HBUINT::static_আকার);

  স্থির size_t get_আকার (unsigned len1, unsigned len2)
  {
    ফেরত Types::HBGlyphID::static_আকার + মান::static_আকার * (len1 + len2);
  }
  স্থির size_t get_আকার (ধ্রুবক মান_Format মান_Formats[2])
  {
    unsigned len1 = মান_Formats[0].get_len ();
    unsigned len2 = মান_Formats[1].get_len ();
    ফেরত get_আকার (len1, len2);
  }

  কাঠামো sanitize_closure_t
  {
    ধ্রুবক মান_Format *মান_Formats;
    unsigned পূর্ণসংখ্যা len1; /* valueFormats[0].get_len() */
    size_t stride; /* bytes */
  };

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক sanitize_closure_t *closure) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!(c->check_struct (এই) &&
	  hb_barrier () &&
          c->check_range (&first_Pair_মান_Record,
                          len,
                          closure->stride))) return_trace (মিথ্যা);
    hb_barrier ();

    unsigned পূর্ণসংখ্যা গুনতি = len;
    ধ্রুবক Pair_মান_Record *record = &first_Pair_মান_Record;
    return_trace (c->lazy_some_gpos ||
		  (closure->মান_Formats[0].sanitize_মানসমূহ_stride_unsafe (c, এই, &record->values[0], গুনতি, closure->stride) &&
                   closure->মান_Formats[1].sanitize_মানসমূহ_stride_unsafe (c, এই, &record->values[closure->len1], গুনতি, closure->stride)));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs,
                   ধ্রুবক মান_Format *মান_Formats) ধ্রুবক
  {
    unsigned record_আকার = get_আকার (মান_Formats);

    ধ্রুবক Pair_মান_Record *record = &first_Pair_মান_Record;
    unsigned পূর্ণসংখ্যা গুনতি = len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যদি (glyphs->has (record->secondGlyph))
        ফেরত সত্য;
      record = &StructAtOffset<ধ্রুবক Pair_মান_Record> (record, record_আকার);
    }
    ফেরত মিথ্যা;
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c,
                       ধ্রুবক মান_Format *মান_Formats) ধ্রুবক
  {
    unsigned record_আকার = get_আকার (মান_Formats);

    ধ্রুবক Pair_মান_Record *record = &first_Pair_মান_Record;
    c->প্রবেশ->add_array (&record->secondGlyph, len, record_আকার);
  }

  template <typename set_t>
  শূন্য collect_second_glyphs (set_t *glyphs,
			      ধ্রুবক মান_Format *মান_Formats) ধ্রুবক
  {
    unsigned record_আকার = get_আকার (মান_Formats);
    ধ্রুবক Pair_মান_Record *record = &first_Pair_মান_Record;
    glyphs->add_array (&record->secondGlyph, len, record_আকার);
  }

  pair_set_মানসমূহ_t get_মানসমূহ_view (ধ্রুবক মান_Format মান_Formats[2]) ধ্রুবক
  {
    ফেরত {এই, &first_Pair_মান_Record.secondGlyph, &first_Pair_মান_Record.values[0],
            (unsigned) len, (unsigned) get_আকার (মান_Formats)};
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ধ্রুবক মান_Format *মান_Formats) ধ্রুবক
  {
    get_মানসমূহ_view (মান_Formats).collect_variation_indices (c, মান_Formats);
  }

  যৌক্তিক apply (hb_ot_apply_context_t *c,
              ধ্রুবক মান_Format *মান_Formats,
              unsigned পূর্ণসংখ্যা pos) ধ্রুবক
  {
    TRACE_APPLY (এই);
    hb_বাফার_t *বাফার = c->বাফার;
    unsigned পূর্ণসংখ্যা len1 = মান_Formats[0].get_len ();
    unsigned পূর্ণসংখ্যা len2 = মান_Formats[1].get_len ();
    unsigned record_আকার = get_আকার (len1, len2);

    ধ্রুবক Pair_মান_Record *record = hb_bsearch (বাফার->info[pos].codepoint,
                                                &first_Pair_মান_Record,
                                                (unsigned) len,
                                                record_আকার);
    যদি (record)
    {
      যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
      {
	c->বাফার->message (c->font,
			    "try kerning glyphs at %u,%u",
			    c->বাফার->idx, pos);
      }

      যৌক্তিক applied_first = len1 && মান_Formats[0].apply_মান (c, এই, &record->values[0], বাফার->cur_pos());
      যৌক্তিক applied_second = len2 && মান_Formats[1].apply_মান (c, এই, &record->values[len1], বাফার->pos[pos]);

      যদি (applied_first || applied_second)
	যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
	{
	  c->বাফার->message (c->font,
			      "kerned glyphs at %u,%u",
			      c->বাফার->idx, pos);
	}

      যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
      {
	c->বাফার->message (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->বাফার->idx, pos);
      }

      যদি (applied_first || applied_second)
        বাফার->unsafe_to_break (বাফার->idx, pos + 1);
      অন্যথা
        /* Even a zero-valued pair record is a concat hazard. */
        বাফার->unsafe_to_concat (বাফার->idx, pos + 1);

      যদি (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      বাফার->unsafe_to_break (বাফার->idx, pos + 1);
      }

      বাফার->idx = pos;
      return_trace (সত্য);
    }
    বাফার->unsafe_to_concat (বাফার->idx, pos + 1);
    return_trace (মিথ্যা);
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক মান_Format মান_Formats[2],
               ধ্রুবক মান_Format newFormats[2]) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto snap = c->serializer->snapshot ();

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);
    out->len = 0;

    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned len1 = মান_Formats[0].get_len ();
    unsigned len2 = মান_Formats[1].get_len ();
    unsigned record_আকার = get_আকার (len1, len2);

    pair_মান_record_context_t context =
    {
      এই,
      মান_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ধ্রুবক Pair_মান_Record *record = &first_Pair_মান_Record;
    unsigned গুনতি = len, অঙ্ক = 0;
    জন্য (unsigned i = 0; i < গুনতি; i++)
    {
      যদি (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) অঙ্ক++;
      record = &StructAtOffset<ধ্রুবক Pair_মান_Record> (record, record_আকার);
    }

    out->len = অঙ্ক;
    যদি (!অঙ্ক) c->serializer->revert (snap);
    return_trace (অঙ্ক);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
