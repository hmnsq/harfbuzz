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

#অন্তর্ভাব "RangeRecord.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ Common {

template <typename Types>
কাঠামো CoverageFormat2_4
{
  friend কাঠামো Coverage;
  using CoverageRangeRecord = RangeRecord<Types, typename Types::HBUINT>;

  সর্বজনীন:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<CoverageRangeRecord>
                rangeRecord;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2 + Types::HBUINT::static_আকার, rangeRecord);

  ব্যক্তিগত:

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned পূর্ণসংখ্যা get_coverage (hb_codepoint_t glyph_id) ধ্রুবক
  {
    ধ্রুবক CoverageRangeRecord &range = rangeRecord.bsearch (glyph_id);
    ফেরত likely (range.first <= range.last)
         ? (unsigned পূর্ণসংখ্যা) range.value + (glyph_id - range.first)
         : NOT_COVERED;
  }

  unsigned get_population () ধ্রুবক
  {
    typename Types::large_int ret = 0;
    জন্য (ধ্রুবক auto &r : rangeRecord)
      ret += r.get_population ();
    ফেরত ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  যৌক্তিক serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);

    unsigned অঙ্ক_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    জন্য (auto g: glyphs)
    {
      যদি (last + 1 != g)
        অঙ্ক_ranges++;
      last = g;
    }

    যদি (unlikely (!rangeRecord.serialize (c, অঙ্ক_ranges))) return_trace (মিথ্যা);
    যদি (!অঙ্ক_ranges) return_trace (সত্য);

    unsigned গুনতি = 0;
    unsigned range = (unsigned) -1;
    last = (hb_codepoint_t) -2;
    unsigned unsorted = মিথ্যা;
    জন্য (auto g: glyphs)
    {
      যদি (last + 1 != g)
      {
	যদি (unlikely (last != (hb_codepoint_t) -2 && last + 1 > g))
	  unsorted = সত্য;

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

    return_trace (সত্য);
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    যদি (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      জন্য (auto g : *glyphs)
        যদি (get_coverage (g) != NOT_COVERED)
	  ফেরত সত্য;
      ফেরত মিথ্যা;
    }

    ফেরত hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (ধ্রুবক CoverageRangeRecord &range) { ফেরত range.intersects (*glyphs); }));
  }
  যৌক্তিক intersects_coverage (ধ্রুবক hb_set_t *glyphs, unsigned পূর্ণসংখ্যা সূচক_2) ধ্রুবক
  {
    auto *range = rangeRecord.as_array ().bsearch (সূচক_2);
    যদি (range)
      ফেরত range->intersects (*glyphs);
    ফেরত মিথ্যা;
  }

  স্থির শূন্য add_intersected_bits (hb_set_t &set, hb_codepoint_t base, uint64_t bits)
  { set.add_bits (base, bits); }

  template <typename IterableOut>
  স্থির শূন্য add_intersected_bits (IterableOut&& out, hb_codepoint_t base, uint64_t bits)
  {
    যখন (bits)
    {
      out << base + hb_ctz (bits);
      bits &= bits - 1;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  শূন্য intersect_set (ধ্রুবক hb_set_t &glyphs, IterableOut&& intersect_glyphs) ধ্রুবক
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t last = 0;
    জন্য (ধ্রুবক auto& range : rangeRecord)
    {
      যদি (unlikely (range.first < last))
        বিরতি;
      last = range.last;
      hb_codepoint_t cursor = range.first & ~hb_bit_page_t::ELT_MASK;
      cursor = cursor ? cursor - 1 : HB_SET_মান_INVALID;
      uint64_t bits;
      যখন (glyphs.পরবর্তী_bits (&cursor, &bits) && cursor <= last)
      {
	যদি (range.first > cursor)
	  bits &= UINT64_MAX << (range.first - cursor);
	unsigned পূর্ণসংখ্যা end = last - cursor;
	যদি (end < hb_bit_page_t::ELT_BITS - 1)
	  bits &= (uint64_t (1) << (end + 1)) - 1;
	add_intersected_bits (intersect_glyphs, cursor, bits);
      }
    }
  }

  unsigned cost () ধ্রুবক { ফেরত hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  যৌক্তিক collect_coverage (set_t *glyphs) ধ্রুবক
  {
    জন্য (ধ্রুবক auto& range: rangeRecord)
      যদি (unlikely (!range.collect_coverage (glyphs)))
        ফেরত মিথ্যা;
    ফেরত সত্য;
  }

  সর্বজনীন:
  /* Older compilers need this to be public. */
  কাঠামো iter_t
  {
    শূন্য init (ধ্রুবক CoverageFormat2_4 &c_)
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
    যৌক্তিক __more__ () ধ্রুবক { ফেরত i < c->rangeRecord.len; }
    শূন্য __next__ ()
    {
      যদি (j >= c->rangeRecord[i].last)
      {
        i++;
        যদি (__more__ ())
        {
          unsigned পূর্ণসংখ্যা old = coverage;
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
           ফেরত;
          }
        }
        অন্যথা
          j = 0;
        ফেরত;
      }
      coverage++;
      j++;
    }
    hb_codepoint_t get_glyph () ধ্রুবক { ফেরত j; }
    যৌক্তিক operator != (ধ্রুবক iter_t& o) ধ্রুবক
    { ফেরত i != o.i || j != o.j; }
    iter_t __end__ () ধ্রুবক
    {
      iter_t it;
      it.init (*c);
      it.i = c->rangeRecord.len;
      it.j = 0;
      ফেরত it;
    }

    ব্যক্তিগত:
    ধ্রুবক কাঠামো CoverageFormat2_4 *c;
    unsigned পূর্ণসংখ্যা i, coverage;
    hb_codepoint_t j;
  };
  ব্যক্তিগত:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
