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

#ಅನ್ತರ್ಭಾವ "RangeRecord.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ Common {

template <typename Types>
ಸಂರಚನಾ CoverageFormat2_4
{
  friend ಸಂರಚನಾ Coverage;
  using CoverageRangeRecord = RangeRecord<Types, typename Types::HBUINT>;

  ಸಾರ್ವಜನಿಕ:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<CoverageRangeRecord>
                rangeRecord;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, rangeRecord);

  ಗುಪ್ತ:

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_coverage (hb_codepoint_t glyph_id) ಧ್ರುವ
  {
    ಧ್ರುವ CoverageRangeRecord &range = rangeRecord.bsearch (glyph_id);
    ಪ್ರತ್ಯಾಗಮ likely (range.first <= range.last)
         ? (unsigned ಪೂರ್ಣಾಙ್ಕ) range.value + (glyph_id - range.first)
         : NOT_COVERED;
  }

  unsigned get_population () ಧ್ರುವ
  {
    typename Types::large_int ret = 0;
    ಪ್ರತಿ (ಧ್ರುವ auto &r : rangeRecord)
      ret += r.get_population ();
    ಪ್ರತ್ಯಾಗಮ ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);

    unsigned ಅಂಕೆ_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    ಪ್ರತಿ (auto g: glyphs)
    {
      ಯದಿ (last + 1 != g)
        ಅಂಕೆ_ranges++;
      last = g;
    }

    ಯದಿ (unlikely (!rangeRecord.serialize (c, ಅಂಕೆ_ranges))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!ಅಂಕೆ_ranges) return_trace (ಸತ್ಯ);

    unsigned ಎಣಿಕೆ = 0;
    unsigned range = (unsigned) -1;
    last = (hb_codepoint_t) -2;
    unsigned unsorted = ಅಸತ್ಯ;
    ಪ್ರತಿ (auto g: glyphs)
    {
      ಯದಿ (last + 1 != g)
      {
	ಯದಿ (unlikely (last != (hb_codepoint_t) -2 && last + 1 > g))
	  unsorted = ಸತ್ಯ;

        range++;
        rangeRecord.arrayZ[range].first = g;
        rangeRecord.arrayZ[range].value = ಎಣಿಕೆ;
      }
      rangeRecord.arrayZ[range].last = g;
      last = g;
      ಎಣಿಕೆ++;
    }

    ಯದಿ (unlikely (unsorted))
      rangeRecord.as_array ().qsort (CoverageRangeRecord::cmp_range);

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ಯದಿ (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      ಪ್ರತಿ (auto g : *glyphs)
        ಯದಿ (get_coverage (g) != NOT_COVERED)
	  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಪ್ರತ್ಯಾಗಮ hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (ಧ್ರುವ CoverageRangeRecord &range) { ಪ್ರತ್ಯಾಗಮ range.intersects (*glyphs); }));
  }
  ತಾರ್ಕಿಕ intersects_coverage (ಧ್ರುವ hb_set_t *glyphs, unsigned ಪೂರ್ಣಾಙ್ಕ index) ಧ್ರುವ
  {
    auto *range = rangeRecord.as_array ().bsearch (index);
    ಯದಿ (range)
      ಪ್ರತ್ಯಾಗಮ range->intersects (*glyphs);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಸ್ಥಿರ ಶೂನ್ಯ add_intersected_bits (hb_set_t &set, hb_codepoint_t base, uint64_t bits)
  { set.add_bits (base, bits); }

  template <typename IterableOut>
  ಸ್ಥಿರ ಶೂನ್ಯ add_intersected_bits (IterableOut&& out, hb_codepoint_t base, uint64_t bits)
  {
    ಯಾವತ್ (bits)
    {
      out << base + hb_ctz (bits);
      bits &= bits - 1;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  ಶೂನ್ಯ intersect_set (ಧ್ರುವ hb_set_t &glyphs, IterableOut&& intersect_glyphs) ಧ್ರುವ
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t last = 0;
    ಪ್ರತಿ (ಧ್ರುವ auto& range : rangeRecord)
    {
      ಯದಿ (unlikely (range.first < last))
        ಭಙ್ಗ;
      last = range.last;
      hb_codepoint_t cursor = range.first & ~hb_bit_page_t::ELT_MASK;
      cursor = cursor ? cursor - 1 : HB_SET_ಮೌಲ್ಯ_INVALID;
      uint64_t bits;
      ಯಾವತ್ (glyphs.next_bits (&cursor, &bits) && cursor <= last)
      {
	ಯದಿ (range.first > cursor)
	  bits &= UINT64_MAX << (range.first - cursor);
	unsigned ಪೂರ್ಣಾಙ್ಕ end = last - cursor;
	ಯದಿ (end < hb_bit_page_t::ELT_BITS - 1)
	  bits &= (uint64_t (1) << (end + 1)) - 1;
	add_intersected_bits (intersect_glyphs, cursor, bits);
      }
    }
  }

  unsigned cost () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  ತಾರ್ಕಿಕ collect_coverage (set_t *glyphs) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ auto& range: rangeRecord)
      ಯದಿ (unlikely (!range.collect_coverage (glyphs)))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಸಾರ್ವಜನಿಕ:
  /* Older compilers need this to be public. */
  ಸಂರಚನಾ iter_t
  {
    ಶೂನ್ಯ init (ಧ್ರುವ CoverageFormat2_4 &c_)
    {
      c = &c_;
      coverage = 0;
      i = 0;
      j = c->rangeRecord.len ? c->rangeRecord[0].first : 0;
      ಯದಿ (unlikely (c->rangeRecord[0].first > c->rangeRecord[0].last))
      {
        /* Broken table. Skip. */
        i = c->rangeRecord.len;
        j = 0;
      }
    }
    ತಾರ್ಕಿಕ __more__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ i < c->rangeRecord.len; }
    ಶೂನ್ಯ __next__ ()
    {
      ಯದಿ (j >= c->rangeRecord[i].last)
      {
        i++;
        ಯದಿ (__more__ ())
        {
          unsigned ಪೂರ್ಣಾಙ್ಕ old = coverage;
          j = c->rangeRecord.arrayZ[i].first;
          coverage = c->rangeRecord.arrayZ[i].value;
          ಯದಿ (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->rangeRecord.len;
           j = 0;
           ಪ್ರತ್ಯಾಗಮ;
          }
        }
        ಅನ್ಯಥಾ
          j = 0;
        ಪ್ರತ್ಯಾಗಮ;
      }
      coverage++;
      j++;
    }
    hb_codepoint_t get_glyph () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ j; }
    ತಾರ್ಕಿಕ operator != (ಧ್ರುವ iter_t& o) ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ i != o.i || j != o.j; }
    iter_t __end__ () ಧ್ರುವ
    {
      iter_t it;
      it.init (*c);
      it.i = c->rangeRecord.len;
      it.j = 0;
      ಪ್ರತ್ಯಾಗಮ it;
    }

    ಗುಪ್ತ:
    ಧ್ರುವ ಸಂರಚನಾ CoverageFormat2_4 *c;
    unsigned ಪೂರ್ಣಾಙ್ಕ i, coverage;
    hb_codepoint_t j;
  };
  ಗುಪ್ತ:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
