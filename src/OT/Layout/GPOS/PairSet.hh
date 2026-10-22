#अगर_अघोषित OT_LAYOUT_GPOS_PAIRSET_HH
#घोषणा OT_LAYOUT_GPOS_PAIRSET_HH

#समावेश "PairValueRecord.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
काष्ठा pair_set_मूल्यावली_t
{
  स्थिर मूल्य_Base *base;
  glyph_तथ्य_t glyphs;
  स्थिर मूल्य *values;
  अचिन्हित गिनती;
  अचिन्हित record_आकार;

  अचिन्हित get_glyph (अचिन्हित i) स्थिर
  {
    glyph_तथ्य_t glyph = glyphs;
    glyph.तथ्य = (स्थिर अक्षर *) glyph.तथ्य + i * record_आकार;
    वापस glyph[0];
  }

  स्थिर मूल्य *get_मूल्यावली (अचिन्हित i) स्थिर
  { वापस reinterpret_cast<स्थिर मूल्य *> ((स्थिर अक्षर *) values + i * record_आकार); }

  hb_pair_t<अचिन्हित, अचिन्हित>
  compute_effective_मूल्य_formats (स्थिर hb_set_t &glyphset,
                                   स्थिर मूल्य_Format मूल्य_Formats[2],
                                   bool strip_hints, bool strip_empty,
                                   स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *varidx_delta_map) स्थिर
  {
    अचिन्हित len1 = मूल्य_Formats[0].get_len ();
    अचिन्हित format1 = 0, format2 = 0;
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      अगर (!glyphset.has (get_glyph (i))) जारी;
      स्थिर मूल्य *v = get_मूल्यावली (i);
      format1 |= मूल्य_Formats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= मूल्य_Formats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    वापस hb_pair (format1, format2);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  स्थिर मूल्य_Format मूल्य_Formats[2]) स्थिर
  {
    अचिन्हित len1 = मूल्य_Formats[0].get_len ();
    अचिन्हित len2 = मूल्य_Formats[1].get_len ();
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      अगर (!c->glyph_set->has (get_glyph (i))) जारी;
      स्थिर hb_array_t<स्थिर मूल्य> v = hb_array (get_मूल्यावली (i), len1 + len2);
      अगर (मूल्य_Formats[0].has_device ())
        मूल्य_Formats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      अगर (मूल्य_Formats[1].has_device ())
        मूल्य_Formats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

ढाँचा <typename Types>
काष्ठा PairSet : मूल्य_Base
{
  ढाँचा <typename Types2>
  मित्र काष्ठा PairPosFormat1_3;

  using Pair_मूल्य_Record = GPOS_impl::Pair_मूल्य_Record<Types>;

  रक्षित:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_मूल्य_Record       first_Pair_मूल्य_Record;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  खुला:
  DEFINE_आकार_MIN (Types::HBUINT::static_आकार);

  अटल माप_प्रकार get_आकार (अचिन्हित len1, अचिन्हित len2)
  {
    वापस Types::HBGlyphID::static_आकार + मूल्य::static_आकार * (len1 + len2);
  }
  अटल माप_प्रकार get_आकार (स्थिर मूल्य_Format मूल्य_Formats[2])
  {
    अचिन्हित len1 = मूल्य_Formats[0].get_len ();
    अचिन्हित len2 = मूल्य_Formats[1].get_len ();
    वापस get_आकार (len1, len2);
  }

  काष्ठा sanitize_closure_t
  {
    स्थिर मूल्य_Format *मूल्य_Formats;
    अचिन्हित पूर्णांक len1; /* valueFormats[0].get_len() */
    माप_प्रकार stride; /* bytes */
  };

  bool sanitize (hb_sanitize_context_t *c, स्थिर sanitize_closure_t *closure) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!(c->check_struct (यह) &&
	  hb_barrier () &&
          c->check_range (&first_Pair_मूल्य_Record,
                          len,
                          closure->stride))) return_trace (false);
    hb_barrier ();

    अचिन्हित पूर्णांक गिनती = len;
    स्थिर Pair_मूल्य_Record *record = &first_Pair_मूल्य_Record;
    return_trace (c->lazy_some_gpos ||
		  (closure->मूल्य_Formats[0].sanitize_मूल्यावली_stride_unsafe (c, यह, &record->values[0], गिनती, closure->stride) &&
                   closure->मूल्य_Formats[1].sanitize_मूल्यावली_stride_unsafe (c, यह, &record->values[closure->len1], गिनती, closure->stride)));
  }

  bool intersects (स्थिर hb_set_t *glyphs,
                   स्थिर मूल्य_Format *मूल्य_Formats) स्थिर
  {
    अचिन्हित record_आकार = get_आकार (मूल्य_Formats);

    स्थिर Pair_मूल्य_Record *record = &first_Pair_मूल्य_Record;
    अचिन्हित पूर्णांक गिनती = len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      अगर (glyphs->has (record->secondGlyph))
        वापस true;
      record = &StructAtOffset<स्थिर Pair_मूल्य_Record> (record, record_आकार);
    }
    वापस false;
  }

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c,
                       स्थिर मूल्य_Format *मूल्य_Formats) स्थिर
  {
    अचिन्हित record_आकार = get_आकार (मूल्य_Formats);

    स्थिर Pair_मूल्य_Record *record = &first_Pair_मूल्य_Record;
    c->निवेश->add_array (&record->secondGlyph, len, record_आकार);
  }

  ढाँचा <typename set_t>
  व्योम collect_second_glyphs (set_t *glyphs,
			      स्थिर मूल्य_Format *मूल्य_Formats) स्थिर
  {
    अचिन्हित record_आकार = get_आकार (मूल्य_Formats);
    स्थिर Pair_मूल्य_Record *record = &first_Pair_मूल्य_Record;
    glyphs->add_array (&record->secondGlyph, len, record_आकार);
  }

  pair_set_मूल्यावली_t get_मूल्यावली_view (स्थिर मूल्य_Format मूल्य_Formats[2]) स्थिर
  {
    वापस {यह, &first_Pair_मूल्य_Record.secondGlyph, &first_Pair_मूल्य_Record.values[0],
            (अचिन्हित) len, (अचिन्हित) get_आकार (मूल्य_Formats)};
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  स्थिर मूल्य_Format *मूल्य_Formats) स्थिर
  {
    get_मूल्यावली_view (मूल्य_Formats).collect_variation_indices (c, मूल्य_Formats);
  }

  bool apply (hb_ot_apply_context_t *c,
              स्थिर मूल्य_Format *मूल्य_Formats,
              अचिन्हित पूर्णांक pos) स्थिर
  {
    TRACE_APPLY (यह);
    hb_बफर_t *बफर = c->बफर;
    अचिन्हित पूर्णांक len1 = मूल्य_Formats[0].get_len ();
    अचिन्हित पूर्णांक len2 = मूल्य_Formats[1].get_len ();
    अचिन्हित record_आकार = get_आकार (len1, len2);

    स्थिर Pair_मूल्य_Record *record = hb_bsearch (बफर->info[pos].codepoint,
                                                &first_Pair_मूल्य_Record,
                                                (अचिन्हित) len,
                                                record_आकार);
    अगर (record)
    {
      अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
      {
	c->बफर->message (c->font,
			    "try kerning glyphs at %u,%u",
			    c->बफर->idx, pos);
      }

      bool applied_first = len1 && मूल्य_Formats[0].apply_मूल्य (c, यह, &record->values[0], बफर->cur_pos());
      bool applied_second = len2 && मूल्य_Formats[1].apply_मूल्य (c, यह, &record->values[len1], बफर->pos[pos]);

      अगर (applied_first || applied_second)
	अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
	{
	  c->बफर->message (c->font,
			      "kerned glyphs at %u,%u",
			      c->बफर->idx, pos);
	}

      अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
      {
	c->बफर->message (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->बफर->idx, pos);
      }

      अगर (applied_first || applied_second)
        बफर->unsafe_to_break (बफर->idx, pos + 1);
      अन्यथा
        /* Even a zero-valued pair record is a concat hazard. */
        बफर->unsafe_to_concat (बफर->idx, pos + 1);

      अगर (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      बफर->unsafe_to_break (बफर->idx, pos + 1);
      }

      बफर->idx = pos;
      return_trace (true);
    }
    बफर->unsafe_to_concat (बफर->idx, pos + 1);
    return_trace (false);
  }

  bool subset (hb_subset_context_t *c,
               स्थिर मूल्य_Format मूल्य_Formats[2],
               स्थिर मूल्य_Format newFormats[2]) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः snap = c->serializer->snapshot ();

    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->len = 0;

    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    अचिन्हित len1 = मूल्य_Formats[0].get_len ();
    अचिन्हित len2 = मूल्य_Formats[1].get_len ();
    अचिन्हित record_आकार = get_आकार (len1, len2);

    pair_मूल्य_record_context_t context =
    {
      यह,
      मूल्य_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    स्थिर Pair_मूल्य_Record *record = &first_Pair_मूल्य_Record;
    अचिन्हित गिनती = len, अंक = 0;
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      अगर (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) अंक++;
      record = &StructAtOffset<स्थिर Pair_मूल्य_Record> (record, record_आकार);
    }

    out->len = अंक;
    अगर (!अंक) c->serializer->revert (snap);
    return_trace (अंक);
  }
};


}
}
}

#पूर्ण_अगर  // OT_LAYOUT_GPOS_PAIRSET_HH
