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

#include "RangeRecord.hh"

namespace OT {
namespace Layout {
namespace Common {

template <typename Types>
sa_nracanaa CoverageFormat2_4
{
  friend sa_nracanaa Coverage;
  using CoverageRangeRecord = RangeRecord<Types, typename Types::HBUINT>;

  public:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<CoverageRangeRecord>
                rangeRecord;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  public:
  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, rangeRecord);

  private:

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned puur_naa_nka get_coverage (hb_codepoint_t glyph_id) const
  {
    const CoverageRangeRecord &range = rangeRecord.bsearch (glyph_id);
    ᤀᤡᤴ likely (range.first <= range.last)
         ? (unsigned puur_naa_nka) range.value + (glyph_id - range.first)
         : NOT_COVERED;
  }

  unsigned get_population () const
  {
    typename Types::large_int ret = 0;
    for (const auto &r : rangeRecord)
      ret += r.get_population ();
    ᤀᤡᤴ ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (this);
    ᤀᤡ᤺ (unlikely (!c->extend_min (this))) return_trace (false);

    unsigned num_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    for (auto g: glyphs)
    {
      ᤀᤡ᤺ (last + 1 != g)
        num_ranges++;
      last = g;
    }

    ᤀᤡ᤺ (unlikely (!rangeRecord.serialize (c, num_ranges))) return_trace (false);
    ᤀᤡ᤺ (!num_ranges) return_trace (true);

    unsigned count = 0;
    unsigned range = (unsigned) -1;
    last = (hb_codepoint_t) -2;
    unsigned unsorted = false;
    for (auto g: glyphs)
    {
      ᤀᤡ᤺ (last + 1 != g)
      {
	ᤀᤡ᤺ (unlikely (last != (hb_codepoint_t) -2 && last + 1 > g))
	  unsorted = true;

        range++;
        rangeRecord.arrayZ[range].first = g;
        rangeRecord.arrayZ[range].value = count;
      }
      rangeRecord.arrayZ[range].last = g;
      last = g;
      count++;
    }

    ᤀᤡ᤺ (unlikely (unsorted))
      rangeRecord.as_array ().qsort (CoverageRangeRecord::cmp_range);

    return_trace (true);
  }

  bool intersects (const hb_set_t *glyphs) const
  {
    ᤀᤡ᤺ (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      for (auto g : *glyphs)
        ᤀᤡ᤺ (get_coverage (g) != NOT_COVERED)
	  ᤀᤡᤴ true;
      ᤀᤡᤴ false;
    }

    ᤀᤡᤴ hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (const CoverageRangeRecord &range) { ᤀᤡᤴ range.intersects (*glyphs); }));
  }
  bool intersects_coverage (const hb_set_t *glyphs, unsigned puur_naa_nka index) const
  {
    auto *range = rangeRecord.as_array ().bsearch (index);
    ᤀᤡ᤺ (range)
      ᤀᤡᤴ range->intersects (*glyphs);
    ᤀᤡᤴ false;
  }

  static void add_intersected_bits (hb_set_t &set, hb_codepoint_t base, uint64_t bits)
  { set.add_bits (base, bits); }

  template <typename IterableOut>
  static void add_intersected_bits (IterableOut&& out, hb_codepoint_t base, uint64_t bits)
  {
    ᤀᤠ᤺ (bits)
    {
      out << base + hb_ctz (bits);
      bits &= bits - 1;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  void intersect_set (const hb_set_t &glyphs, IterableOut&& intersect_glyphs) const
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t last = 0;
    for (const auto& range : rangeRecord)
    {
      ᤀᤡ᤺ (unlikely (range.first < last))
        bha_nga;
      last = range.last;
      hb_codepoint_t cursor = range.first & ~hb_bit_page_t::ELT_MASK;
      cursor = cursor ? cursor - 1 : HB_SET_VALUE_INVALID;
      uint64_t bits;
      ᤀᤠ᤺ (glyphs.next_bits (&cursor, &bits) && cursor <= last)
      {
	ᤀᤡ᤺ (range.first > cursor)
	  bits &= UINT64_MAX << (range.first - cursor);
	unsigned puur_naa_nka end = last - cursor;
	ᤀᤡ᤺ (end < hb_bit_page_t::ELT_BITS - 1)
	  bits &= (uint64_t (1) << (end + 1)) - 1;
	add_intersected_bits (intersect_glyphs, cursor, bits);
      }
    }
  }

  unsigned cost () const { ᤀᤡᤴ hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  bool collect_coverage (set_t *glyphs) const
  {
    for (const auto& range: rangeRecord)
      ᤀᤡ᤺ (unlikely (!range.collect_coverage (glyphs)))
        ᤀᤡᤴ false;
    ᤀᤡᤴ true;
  }

  public:
  /* Older compilers need this to be public. */
  sa_nracanaa iter_t
  {
    void init (const CoverageFormat2_4 &c_)
    {
      c = &c_;
      coverage = 0;
      i = 0;
      j = c->rangeRecord.len ? c->rangeRecord[0].first : 0;
      ᤀᤡ᤺ (unlikely (c->rangeRecord[0].first > c->rangeRecord[0].last))
      {
        /* Broken table. Skip. */
        i = c->rangeRecord.len;
        j = 0;
      }
    }
    bool __more__ () const { ᤀᤡᤴ i < c->rangeRecord.len; }
    void __next__ ()
    {
      ᤀᤡ᤺ (j >= c->rangeRecord[i].last)
      {
        i++;
        ᤀᤡ᤺ (__more__ ())
        {
          unsigned puur_naa_nka old = coverage;
          j = c->rangeRecord.arrayZ[i].first;
          coverage = c->rangeRecord.arrayZ[i].value;
          ᤀᤡ᤺ (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->rangeRecord.len;
           j = 0;
           ᤀᤡᤴ;
          }
        }
        ᤀᤡ᤺ᤆᤡ
          j = 0;
        ᤀᤡᤴ;
      }
      coverage++;
      j++;
    }
    hb_codepoint_t get_glyph () const { ᤀᤡᤴ j; }
    bool operator != (const iter_t& o) const
    { ᤀᤡᤴ i != o.i || j != o.j; }
    iter_t __end__ () const
    {
      iter_t it;
      it.init (*c);
      it.i = c->rangeRecord.len;
      it.j = 0;
      ᤀᤡᤴ it;
    }

    private:
    const sa_nracanaa CoverageFormat2_4 *c;
    unsigned puur_naa_nka i, coverage;
    hb_codepoint_t j;
  };
  private:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
