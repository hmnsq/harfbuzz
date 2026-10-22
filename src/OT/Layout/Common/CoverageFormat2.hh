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

#شامل "RangeRecord.hh"

فضاینام OT {
فضاینام Layout {
فضاینام Common {

template <typename Types>
ساختار CoverageFormat2_4
{
  friend ساختار Coverage;
  using CoverageRangeRecord = RangeRecord<Types, typename Types::HBUINT>;

  عمومی:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<CoverageRangeRecord>
                rangeRecord;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  عمومی:
  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, rangeRecord);

  خصوصی:

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned صحیح get_coverage (hb_codepoint_t glyph_id) ثابت
  {
    ثابت CoverageRangeRecord &range = rangeRecord.bsearch (glyph_id);
    بازگشت likely (range.first <= range.last)
         ? (unsigned صحیح) range.value + (glyph_id - range.first)
         : NOT_COVERED;
  }

  unsigned get_population () ثابت
  {
    typename Types::large_int ret = 0;
    برای (ثابت auto &r : rangeRecord)
      ret += r.get_population ();
    بازگشت ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);

    unsigned رقم_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    برای (auto g: glyphs)
    {
      اگر (last + 1 != g)
        رقم_ranges++;
      last = g;
    }

    اگر (unlikely (!rangeRecord.serialize (c, رقم_ranges))) return_trace (نادرست);
    اگر (!رقم_ranges) return_trace (درست);

    unsigned شمار = 0;
    unsigned range = (unsigned) -1;
    last = (hb_codepoint_t) -2;
    unsigned unsorted = نادرست;
    برای (auto g: glyphs)
    {
      اگر (last + 1 != g)
      {
	اگر (unlikely (last != (hb_codepoint_t) -2 && last + 1 > g))
	  unsorted = درست;

        range++;
        rangeRecord.arrayZ[range].first = g;
        rangeRecord.arrayZ[range].value = شمار;
      }
      rangeRecord.arrayZ[range].last = g;
      last = g;
      شمار++;
    }

    اگر (unlikely (unsorted))
      rangeRecord.as_array ().qsort (CoverageRangeRecord::cmp_range);

    return_trace (درست);
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    اگر (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      برای (auto g : *glyphs)
        اگر (get_coverage (g) != NOT_COVERED)
	  بازگشت درست;
      بازگشت نادرست;
    }

    بازگشت hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (ثابت CoverageRangeRecord &range) { بازگشت range.intersects (*glyphs); }));
  }
  منطقی intersects_coverage (ثابت hb_set_t *glyphs, unsigned صحیح index) ثابت
  {
    auto *range = rangeRecord.as_array ().bsearch (index);
    اگر (range)
      بازگشت range->intersects (*glyphs);
    بازگشت نادرست;
  }

  ایستا تهی add_intersected_bits (hb_set_t &set, hb_codepoint_t base, uint64_t bits)
  { set.add_bits (base, bits); }

  template <typename IterableOut>
  ایستا تهی add_intersected_bits (IterableOut&& out, hb_codepoint_t base, uint64_t bits)
  {
    وقتی (bits)
    {
      out << base + hb_ctz (bits);
      bits &= bits - 1;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  تهی intersect_set (ثابت hb_set_t &glyphs, IterableOut&& intersect_glyphs) ثابت
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t last = 0;
    برای (ثابت auto& range : rangeRecord)
    {
      اگر (unlikely (range.first < last))
        بشکن;
      last = range.last;
      hb_codepoint_t cursor = range.first & ~hb_bit_page_t::ELT_MASK;
      cursor = cursor ? cursor - 1 : HB_SET_مقدار_INVALID;
      uint64_t bits;
      وقتی (glyphs.next_bits (&cursor, &bits) && cursor <= last)
      {
	اگر (range.first > cursor)
	  bits &= UINT64_MAX << (range.first - cursor);
	unsigned صحیح end = last - cursor;
	اگر (end < hb_bit_page_t::ELT_BITS - 1)
	  bits &= (uint64_t (1) << (end + 1)) - 1;
	add_intersected_bits (intersect_glyphs, cursor, bits);
      }
    }
  }

  unsigned cost () ثابت { بازگشت hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  منطقی collect_coverage (set_t *glyphs) ثابت
  {
    برای (ثابت auto& range: rangeRecord)
      اگر (unlikely (!range.collect_coverage (glyphs)))
        بازگشت نادرست;
    بازگشت درست;
  }

  عمومی:
  /* Older compilers need this to be public. */
  ساختار iter_t
  {
    تهی init (ثابت CoverageFormat2_4 &c_)
    {
      c = &c_;
      coverage = 0;
      i = 0;
      j = c->rangeRecord.len ? c->rangeRecord[0].first : 0;
      اگر (unlikely (c->rangeRecord[0].first > c->rangeRecord[0].last))
      {
        /* Broken table. Skip. */
        i = c->rangeRecord.len;
        j = 0;
      }
    }
    منطقی __more__ () ثابت { بازگشت i < c->rangeRecord.len; }
    تهی __next__ ()
    {
      اگر (j >= c->rangeRecord[i].last)
      {
        i++;
        اگر (__more__ ())
        {
          unsigned صحیح old = coverage;
          j = c->rangeRecord.arrayZ[i].first;
          coverage = c->rangeRecord.arrayZ[i].value;
          اگر (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->rangeRecord.len;
           j = 0;
           بازگشت;
          }
        }
        وگرنه
          j = 0;
        بازگشت;
      }
      coverage++;
      j++;
    }
    hb_codepoint_t get_glyph () ثابت { بازگشت j; }
    منطقی operator != (ثابت iter_t& o) ثابت
    { بازگشت i != o.i || j != o.j; }
    iter_t __end__ () ثابت
    {
      iter_t it;
      it.init (*c);
      it.i = c->rangeRecord.len;
      it.j = 0;
      بازگشت it;
    }

    خصوصی:
    ثابت ساختار CoverageFormat2_4 *c;
    unsigned صحیح i, coverage;
    hb_codepoint_t j;
  };
  خصوصی:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
