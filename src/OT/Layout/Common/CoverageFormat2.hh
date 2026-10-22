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

#অন্তর্ভাৱ "RangeRecord.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ Common {

template <typename Types>
সংরচনা CoverageFormat2_4
{
  friend সংরচনা Coverage;
  using CoverageRangeRecord = RangeRecord<Types, typename Types::HBUINT>;

  সার্ৱজনিক:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<CoverageRangeRecord>
                rangeRecord;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, rangeRecord);

  গুপ্ত:

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned পূর্ণাঙ্ক get_coverage (hb_codepoint_t glyph_id) ধ্রুৱ
  {
    ধ্রুৱ CoverageRangeRecord &range = rangeRecord.bsearch (glyph_id);
    ঘূৰাই likely (range.first <= range.last)
         ? (unsigned পূর্ণাঙ্ক) range.value + (glyph_id - range.first)
         : NOT_COVERED;
  }

  unsigned get_population () ধ্রুৱ
  {
    typename Types::large_int ret = 0;
    বাবে (ধ্রুৱ auto &r : rangeRecord)
      ret += r.get_population ();
    ঘূৰাই ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  তার্কিক serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);

    unsigned অঙ্ক_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    বাবে (auto g: glyphs)
    {
      যদি (last + 1 != g)
        অঙ্ক_ranges++;
      last = g;
    }

    যদি (unlikely (!rangeRecord.serialize (c, অঙ্ক_ranges))) return_trace (মিছা);
    যদি (!অঙ্ক_ranges) return_trace (সঁচা);

    unsigned গুনতি = 0;
    unsigned range = (unsigned) -1;
    last = (hb_codepoint_t) -2;
    unsigned unsorted = মিছা;
    বাবে (auto g: glyphs)
    {
      যদি (last + 1 != g)
      {
	যদি (unlikely (last != (hb_codepoint_t) -2 && last + 1 > g))
	  unsorted = সঁচা;

        range++;
        rangeRecord.arrayZ[range].first = g;
        rangeRecord.arrayZ[range].value = গুনতি;
      }
      rangeRecord.arrayZ[range].last = g;
      last = g;
      গুনতি++;
    }

    যদি (unlikely (unsorted))
      rangeRecord.as_array ().qsort (CoverageRangeRecord::cmp_range);

    return_trace (সঁচা);
  }

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  {
    যদি (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      বাবে (auto g : *glyphs)
        যদি (get_coverage (g) != NOT_COVERED)
	  ঘূৰাই সঁচা;
      ঘূৰাই মিছা;
    }

    ঘূৰাই hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (ধ্রুৱ CoverageRangeRecord &range) { ঘূৰাই range.intersects (*glyphs); }));
  }
  তার্কিক intersects_coverage (ধ্রুৱ hb_set_t *glyphs, unsigned পূর্ণাঙ্ক index) ধ্রুৱ
  {
    auto *range = rangeRecord.as_array ().bsearch (index);
    যদি (range)
      ঘূৰাই range->intersects (*glyphs);
    ঘূৰাই মিছা;
  }

  স্থির শূন্য add_intersected_bits (hb_set_t &set, hb_codepoint_t base, uint64_t bits)
  { set.add_bits (base, bits); }

  template <typename IterableOut>
  স্থির শূন্য add_intersected_bits (IterableOut&& out, hb_codepoint_t base, uint64_t bits)
  {
    যেতিয়ালৈ (bits)
    {
      out << base + hb_ctz (bits);
      bits &= bits - 1;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  শূন্য intersect_set (ধ্রুৱ hb_set_t &glyphs, IterableOut&& intersect_glyphs) ধ্রুৱ
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t last = 0;
    বাবে (ধ্রুৱ auto& range : rangeRecord)
    {
      যদি (unlikely (range.first < last))
        ভঙ্গ;
      last = range.last;
      hb_codepoint_t cursor = range.first & ~hb_bit_page_t::ELT_MASK;
      cursor = cursor ? cursor - 1 : HB_SET_মান_INVALID;
      uint64_t bits;
      যেতিয়ালৈ (glyphs.next_bits (&cursor, &bits) && cursor <= last)
      {
	যদি (range.first > cursor)
	  bits &= UINT64_MAX << (range.first - cursor);
	unsigned পূর্ণাঙ্ক end = last - cursor;
	যদি (end < hb_bit_page_t::ELT_BITS - 1)
	  bits &= (uint64_t (1) << (end + 1)) - 1;
	add_intersected_bits (intersect_glyphs, cursor, bits);
      }
    }
  }

  unsigned cost () ধ্রুৱ { ঘূৰাই hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  তার্কিক collect_coverage (set_t *glyphs) ধ্রুৱ
  {
    বাবে (ধ্রুৱ auto& range: rangeRecord)
      যদি (unlikely (!range.collect_coverage (glyphs)))
        ঘূৰাই মিছা;
    ঘূৰাই সঁচা;
  }

  সার্ৱজনিক:
  /* Older compilers need this to be public. */
  সংরচনা iter_t
  {
    শূন্য init (ধ্রুৱ CoverageFormat2_4 &c_)
    {
      c = &c_;
      coverage = 0;
      i = 0;
      j = c->rangeRecord.len ? c->rangeRecord[0].first : 0;
      যদি (unlikely (c->rangeRecord[0].first > c->rangeRecord[0].last))
      {
        /* Broken table. Skip. */
        i = c->rangeRecord.len;
        j = 0;
      }
    }
    তার্কিক __more__ () ধ্রুৱ { ঘূৰাই i < c->rangeRecord.len; }
    শূন্য __next__ ()
    {
      যদি (j >= c->rangeRecord[i].last)
      {
        i++;
        যদি (__more__ ())
        {
          unsigned পূর্ণাঙ্ক old = coverage;
          j = c->rangeRecord.arrayZ[i].first;
          coverage = c->rangeRecord.arrayZ[i].value;
          যদি (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->rangeRecord.len;
           j = 0;
           ঘূৰাই;
          }
        }
        অন্যথা
          j = 0;
        ঘূৰাই;
      }
      coverage++;
      j++;
    }
    hb_codepoint_t get_glyph () ধ্রুৱ { ঘূৰাই j; }
    তার্কিক operator != (ধ্রুৱ iter_t& o) ধ্রুৱ
    { ঘূৰাই i != o.i || j != o.j; }
    iter_t __end__ () ধ্রুৱ
    {
      iter_t it;
      it.init (*c);
      it.i = c->rangeRecord.len;
      it.j = 0;
      ঘূৰাই it;
    }

    গুপ্ত:
    ধ্রুৱ সংরচনা CoverageFormat2_4 *c;
    unsigned পূর্ণাঙ্ক i, coverage;
    hb_codepoint_t j;
  };
  গুপ্ত:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
