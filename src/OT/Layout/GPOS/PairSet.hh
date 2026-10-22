#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#include "PairValueRecord.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
struct pair_set_मगा_हालागा_t
{
  const हालागा_Base *base;
  glyph_data_t glyphs;
  const हालागा *values;
  unsigned बिलांग;
  unsigned record_size;

  unsigned get_glyph (unsigned i) const
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (const char *) glyph.data + i * record_size;
    return glyph[0];
  }

  const हालागा *get_मगा_हालागा (unsigned i) const
  { return reinterpret_cast<const हालागा *> ((const char *) values + i * record_size); }

  hb_pair_t<unsigned, unsigned>
  compute_effective_हालागा_formats (const hb_set_t &glyphset,
                                   const हालागा_Format हालागा_Formats[2],
                                   bool strip_hints, bool strip_empty,
                                   const hb_hashmap_t<unsigned, hb_pair_t<unsigned, int>> *varidx_delta_map) const
  {
    unsigned len1 = हालागा_Formats[0].get_len ();
    unsigned format1 = 0, format2 = 0;
    for (unsigned i = 0; i < बिलांग; i++)
    {
      if (!glyphset.has (get_glyph (i))) continue;
      const हालागा *v = get_मगा_हालागा (i);
      format1 |= हालागा_Formats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= हालागा_Formats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    return hb_pair (format1, format2);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  const हालागा_Format हालागा_Formats[2]) const
  {
    unsigned len1 = हालागा_Formats[0].get_len ();
    unsigned len2 = हालागा_Formats[1].get_len ();
    for (unsigned i = 0; i < बिलांग; i++)
    {
      if (!c->glyph_set->has (get_glyph (i))) continue;
      const hb_array_t<const हालागा> v = hb_array (get_मगा_हालागा (i), len1 + len2);
      if (हालागा_Formats[0].has_device ())
        हालागा_Formats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      if (हालागा_Formats[1].has_device ())
        हालागा_Formats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

template <typename Types>
struct PairSet : हालागा_Base
{
  template <typename Types2>
  friend struct PairPosFormat1_3;

  using Pair_हालागा_Record = GPOS_impl::Pair_हालागा_Record<Types>;

  protected:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_हालागा_Record       first_Pair_हालागा_Record;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  public:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  static size_t get_size (unsigned len1, unsigned len2)
  {
    return Types::HBGlyphID::static_size + हालागा::static_size * (len1 + len2);
  }
  static size_t get_size (const हालागा_Format हालागा_Formats[2])
  {
    unsigned len1 = हालागा_Formats[0].get_len ();
    unsigned len2 = हालागा_Formats[1].get_len ();
    return get_size (len1, len2);
  }

  struct sanitize_closure_t
  {
    const हालागा_Format *हालागा_Formats;
    unsigned int len1; /* valueFormats[0].get_len() */
    size_t stride; /* bytes */
  };

  bool sanitize (hb_sanitize_context_t *c, const sanitize_closure_t *closure) const
  {
    TRACE_SANITIZE (this);
    if (!(c->check_struct (this) &&
	  hb_barrier () &&
          c->check_range (&first_Pair_हालागा_Record,
                          len,
                          closure->stride))) return_trace (false);
    hb_barrier ();

    unsigned int बिलांग = len;
    const Pair_हालागा_Record *record = &first_Pair_हालागा_Record;
    return_trace (c->lazy_some_gpos ||
		  (closure->हालागा_Formats[0].sanitize_मगा_हालागा_stride_unsafe (c, this, &record->values[0], बिलांग, closure->stride) &&
                   closure->हालागा_Formats[1].sanitize_मगा_हालागा_stride_unsafe (c, this, &record->values[closure->len1], बिलांग, closure->stride)));
  }

  bool intersects (const hb_set_t *glyphs,
                   const हालागा_Format *हालागा_Formats) const
  {
    unsigned record_size = get_size (हालागा_Formats);

    const Pair_हालागा_Record *record = &first_Pair_हालागा_Record;
    unsigned int बिलांग = len;
    for (unsigned int i = 0; i < बिलांग; i++)
    {
      if (glyphs->has (record->secondGlyph))
        return true;
      record = &StructAtOffset<const Pair_हालागा_Record> (record, record_size);
    }
    return false;
  }

  void collect_glyphs (hb_collect_glyphs_context_t *c,
                       const हालागा_Format *हालागा_Formats) const
  {
    unsigned record_size = get_size (हालागा_Formats);

    const Pair_हालागा_Record *record = &first_Pair_हालागा_Record;
    c->पासोक->add_array (&record->secondGlyph, len, record_size);
  }

  template <typename set_t>
  void collect_second_glyphs (set_t *glyphs,
			      const हालागा_Format *हालागा_Formats) const
  {
    unsigned record_size = get_size (हालागा_Formats);
    const Pair_हालागा_Record *record = &first_Pair_हालागा_Record;
    glyphs->add_array (&record->secondGlyph, len, record_size);
  }

  pair_set_मगा_हालागा_t get_मगा_हालागा_view (const हालागा_Format हालागा_Formats[2]) const
  {
    return {this, &first_Pair_हालागा_Record.secondGlyph, &first_Pair_हालागा_Record.values[0],
            (unsigned) len, (unsigned) get_size (हालागा_Formats)};
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  const हालागा_Format *हालागा_Formats) const
  {
    get_मगा_हालागा_view (हालागा_Formats).collect_variation_indices (c, हालागा_Formats);
  }

  bool apply (hb_ot_apply_context_t *c,
              const हालागा_Format *हालागा_Formats,
              unsigned int pos) const
  {
    TRACE_APPLY (this);
    hb_buffer_t *buffer = c->buffer;
    unsigned int len1 = हालागा_Formats[0].get_len ();
    unsigned int len2 = हालागा_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    const Pair_हालागा_Record *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &first_Pair_हालागा_Record,
                                                (unsigned) len,
                                                record_size);
    if (record)
    {
      if (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "try kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      bool applied_first = len1 && हालागा_Formats[0].apply_हालागा (c, this, &record->values[0], buffer->cur_pos());
      bool applied_second = len2 && हालागा_Formats[1].apply_हालागा (c, this, &record->values[len1], buffer->pos[pos]);

      if (applied_first || applied_second)
	if (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
	{
	  c->buffer->message (c->font,
			      "kerned glyphs at %u,%u",
			      c->buffer->idx, pos);
	}

      if (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      if (applied_first || applied_second)
        buffer->unsafe_to_break (buffer->idx, pos + 1);
      else
        /* Even a zero-valued pair record is a concat hazard. */
        buffer->unsafe_to_concat (buffer->idx, pos + 1);

      if (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, pos + 1);
      }

      buffer->idx = pos;
      return_trace (true);
    }
    buffer->unsafe_to_concat (buffer->idx, pos + 1);
    return_trace (false);
  }

  bool subset (hb_subset_context_t *c,
               const हालागा_Format हालागा_Formats[2],
               const हालागा_Format newFormats[2]) const
  {
    TRACE_SUBSET (this);
    auto snap = c->serializer->snapshot ();

    auto *out = c->serializer->start_embed (*this);
    if (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->len = 0;

    const hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    const hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned len1 = हालागा_Formats[0].get_len ();
    unsigned len2 = हालागा_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    pair_हालागा_record_context_t context =
    {
      this,
      हालागा_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    const Pair_हालागा_Record *record = &first_Pair_हालागा_Record;
    unsigned बिलांग = len, नुमेरो = 0;
    for (unsigned i = 0; i < बिलांग; i++)
    {
      if (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) नुमेरो++;
      record = &StructAtOffset<const Pair_हालागा_Record> (record, record_size);
    }

    out->len = नुमेरो;
    if (!नुमेरो) c->serializer->revert (snap);
    return_trace (नुमेरो);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
