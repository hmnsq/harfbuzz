/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2010,2012  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod, Garret Rieger
 */

#ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
#define OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH

#ଅନ୍ତର୍ଭାଵ "RangeRecord.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ Common {

template <typename Types>
ସଂରଚନା CoverageFormat2_4
{
  friend ସଂରଚନା Coverage;
  using CoverageRangeRecord = RangeRecord<Types, typename Types::HBUINT>;

  ସର୍ବସାଧାରଣ:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<CoverageRangeRecord>
                rangeRecord;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, rangeRecord);

  ବ୍ୟକ୍ତିଗତ:

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_coverage (hb_codepoint_t glyph_id) ଅଚଳ
  {
    ଅଚଳ CoverageRangeRecord &range = rangeRecord.bsearch (glyph_id);
    ଫେରନ୍ତୁ likely (range.first <= range.last)
         ? (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) range.value + (glyph_id - range.first)
         : NOT_COVERED;
  }

  unsigned get_population () ଅଚଳ
  {
    typename Types::large_int ret = 0;
    ପାଇଁ (ଅଚଳ auto &r : rangeRecord)
      ret += r.get_population ();
    ଫେରନ୍ତୁ ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);

    unsigned ଅଙ୍କ_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    ପାଇଁ (auto g: glyphs)
    {
      ଯଦି (last + 1 != g)
        ଅଙ୍କ_ranges++;
      last = g;
    }

    ଯଦି (unlikely (!rangeRecord.serialize (c, ଅଙ୍କ_ranges))) return_trace (ମିଥ୍ୟା);
    ଯଦି (!ଅଙ୍କ_ranges) return_trace (ସତ୍ୟ);

    unsigned ଗଣନା_2 = 0;
    unsigned range = (unsigned) -1;
    last = (hb_codepoint_t) -2;
    unsigned unsorted = ମିଥ୍ୟା;
    ପାଇଁ (auto g: glyphs)
    {
      ଯଦି (last + 1 != g)
      {
	ଯଦି (unlikely (last != (hb_codepoint_t) -2 && last + 1 > g))
	  unsorted = ସତ୍ୟ;

        range++;
        rangeRecord.arrayZ[range].first = g;
        rangeRecord.arrayZ[range].value = ଗଣନା_2;
      }
      rangeRecord.arrayZ[range].last = g;
      last = g;
      ଗଣନା_2++;
    }

    ଯଦି (unlikely (unsorted))
      rangeRecord.as_array ().qsort (CoverageRangeRecord::cmp_range);

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ଯଦି (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      ପାଇଁ (auto g : *glyphs)
        ଯଦି (get_coverage (g) != NOT_COVERED)
	  ଫେରନ୍ତୁ ସତ୍ୟ;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଫେରନ୍ତୁ hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (ଅଚଳ CoverageRangeRecord &range) { ଫେରନ୍ତୁ range.intersects (*glyphs); }));
  }
  ତାର୍କିକ intersects_coverage (ଅଚଳ hb_set_t *glyphs, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index) ଅଚଳ
  {
    auto *range = rangeRecord.as_array ().bsearch (index);
    ଯଦି (range)
      ଫେରନ୍ତୁ range->intersects (*glyphs);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ସ୍ଥିର ଶୂନ୍ୟ add_intersected_bits (hb_set_t &set, hb_codepoint_t base, uint64_t bits)
  { set.add_bits (base, bits); }

  template <typename IterableOut>
  ସ୍ଥିର ଶୂନ୍ୟ add_intersected_bits (IterableOut&& out, hb_codepoint_t base, uint64_t bits)
  {
    ଯେତେବେଳେ (bits)
    {
      out << base + hb_ctz (bits);
      bits &= bits - 1;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  ଶୂନ୍ୟ intersect_set (ଅଚଳ hb_set_t &glyphs, IterableOut&& intersect_glyphs) ଅଚଳ
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t last = 0;
    ପାଇଁ (ଅଚଳ auto& range : rangeRecord)
    {
      ଯଦି (unlikely (range.first < last))
        ଭାଙ୍ଗ;
      last = range.last;
      hb_codepoint_t cursor = range.first & ~hb_bit_page_t::ELT_MASK;
      cursor = cursor ? cursor - 1 : HB_SET_ମୂଲ୍ୟ_INVALID;
      uint64_t bits;
      ଯେତେବେଳେ (glyphs.next_bits (&cursor, &bits) && cursor <= last)
      {
	ଯଦି (range.first > cursor)
	  bits &= UINT64_MAX << (range.first - cursor);
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = last - cursor;
	ଯଦି (end < hb_bit_page_t::ELT_BITS - 1)
	  bits &= (uint64_t (1) << (end + 1)) - 1;
	add_intersected_bits (intersect_glyphs, cursor, bits);
      }
    }
  }

  unsigned cost () ଅଚଳ { ଫେରନ୍ତୁ hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  ତାର୍କିକ collect_coverage (set_t *glyphs) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ auto& range: rangeRecord)
      ଯଦି (unlikely (!range.collect_coverage (glyphs)))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ସର୍ବସାଧାରଣ:
  /* Older compilers need this to be public. */
  ସଂରଚନା iter_t
  {
    ଶୂନ୍ୟ init (ଅଚଳ CoverageFormat2_4 &c_)
    {
      c = &c_;
      coverage = 0;
      i = 0;
      j = c->rangeRecord.len ? c->rangeRecord[0].first : 0;
      ଯଦି (unlikely (c->rangeRecord[0].first > c->rangeRecord[0].last))
      {
        /* Broken table. Skip. */
        i = c->rangeRecord.len;
        j = 0;
      }
    }
    ତାର୍କିକ __more__ () ଅଚଳ { ଫେରନ୍ତୁ i < c->rangeRecord.len; }
    ଶୂନ୍ୟ __next__ ()
    {
      ଯଦି (j >= c->rangeRecord[i].last)
      {
        i++;
        ଯଦି (__more__ ())
        {
          unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା old = coverage;
          j = c->rangeRecord.arrayZ[i].first;
          coverage = c->rangeRecord.arrayZ[i].value;
          ଯଦି (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->rangeRecord.len;
           j = 0;
           ଫେରନ୍ତୁ;
          }
        }
        ଅନ୍ୟଥା
          j = 0;
        ଫେରନ୍ତୁ;
      }
      coverage++;
      j++;
    }
    hb_codepoint_t get_glyph () ଅଚଳ { ଫେରନ୍ତୁ j; }
    ତାର୍କିକ operator != (ଅଚଳ iter_t& o) ଅଚଳ
    { ଫେରନ୍ତୁ i != o.i || j != o.j; }
    iter_t __end__ () ଅଚଳ
    {
      iter_t it;
      it.init (*c);
      it.i = c->rangeRecord.len;
      it.j = 0;
      ଫେରନ୍ତୁ it;
    }

    ବ୍ୟକ୍ତିଗତ:
    ଅଚଳ ସଂରଚନା CoverageFormat2_4 *c;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, coverage;
    hb_codepoint_t j;
  };
  ବ୍ୟକ୍ତିଗତ:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
