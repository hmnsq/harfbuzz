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

#ضمن "RangeRecord.hh"

نطاق OT {
نطاق Layout {
نطاق Common {

template <typename Types>
بنية CoverageFormat2_4
{
  friend بنية Coverage;
  using CoverageRangeRecord = RangeRecord<Types, typename Types::HBUINT>;

  عام:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<CoverageRangeRecord>
                rangeRecord;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  عام:
  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, rangeRecord);

  خاص:

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned صحيح get_coverage (hb_codepoint_t glyph_id) ثابت
  {
    ثابت CoverageRangeRecord &range = rangeRecord.bsearch (glyph_id);
    أرجع likely (range.first <= range.last)
         ? (unsigned صحيح) range.value + (glyph_id - range.first)
         : NOT_COVERED;
  }

  unsigned get_population () ثابت
  {
    typename Types::large_int ret = 0;
    لكل (ثابت auto &r : rangeRecord)
      ret += r.get_population ();
    أرجع ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (هذا);
    𐤀𐤌 (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    unsigned num_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    لكل (auto g: glyphs)
    {
      𐤀𐤌 (last + 1 != g)
        num_ranges++;
      last = g;
    }

    𐤀𐤌 (unlikely (!rangeRecord.serialize (c, num_ranges))) return_trace (خطأ);
    𐤀𐤌 (!num_ranges) return_trace (true);

    unsigned count = 0;
    unsigned range = (unsigned) -1;
    last = (hb_codepoint_t) -2;
    unsigned unsorted = خطأ;
    لكل (auto g: glyphs)
    {
      𐤀𐤌 (last + 1 != g)
      {
	𐤀𐤌 (unlikely (last != (hb_codepoint_t) -2 && last + 1 > g))
	  unsorted = true;

        range++;
        rangeRecord.arrayZ[range].first = g;
        rangeRecord.arrayZ[range].value = count;
      }
      rangeRecord.arrayZ[range].last = g;
      last = g;
      count++;
    }

    𐤀𐤌 (unlikely (unsorted))
      rangeRecord.as_array ().qsort (CoverageRangeRecord::cmp_range);

    return_trace (true);
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    𐤀𐤌 (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      لكل (auto g : *glyphs)
        𐤀𐤌 (get_coverage (g) != NOT_COVERED)
	  أرجع true;
      أرجع خطأ;
    }

    أرجع hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (ثابت CoverageRangeRecord &range) { أرجع range.intersects (*glyphs); }));
  }
  منطقي intersects_coverage (ثابت hb_set_t *glyphs, unsigned صحيح index) ثابت
  {
    auto *range = rangeRecord.as_array ().bsearch (index);
    𐤀𐤌 (range)
      أرجع range->intersects (*glyphs);
    أرجع خطأ;
  }

  ساكن فراغ add_intersected_bits (hb_set_t &set, hb_codepoint_t base, uint64_t bits)
  { set.add_bits (base, bits); }

  template <typename IterableOut>
  ساكن فراغ add_intersected_bits (IterableOut&& out, hb_codepoint_t base, uint64_t bits)
  {
    طالما (bits)
    {
      out << base + hb_ctz (bits);
      bits &= bits - 1;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  فراغ intersect_set (ثابت hb_set_t &glyphs, IterableOut&& intersect_glyphs) ثابت
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t last = 0;
    لكل (ثابت auto& range : rangeRecord)
    {
      𐤀𐤌 (unlikely (range.first < last))
        اكسر;
      last = range.last;
      hb_codepoint_t cursor = range.first & ~hb_bit_page_t::ELT_MASK;
      cursor = cursor ? cursor - 1 : HB_SET_VALUE_INVALID;
      uint64_t bits;
      طالما (glyphs.next_bits (&cursor, &bits) && cursor <= last)
      {
	𐤀𐤌 (range.first > cursor)
	  bits &= UINT64_MAX << (range.first - cursor);
	unsigned صحيح end = last - cursor;
	𐤀𐤌 (end < hb_bit_page_t::ELT_BITS - 1)
	  bits &= (uint64_t (1) << (end + 1)) - 1;
	add_intersected_bits (intersect_glyphs, cursor, bits);
      }
    }
  }

  unsigned cost () ثابت { أرجع hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  منطقي collect_coverage (set_t *glyphs) ثابت
  {
    لكل (ثابت auto& range: rangeRecord)
      𐤀𐤌 (unlikely (!range.collect_coverage (glyphs)))
        أرجع خطأ;
    أرجع true;
  }

  عام:
  /* Older compilers need this to be public. */
  بنية iter_t
  {
    فراغ init (ثابت CoverageFormat2_4 &c_)
    {
      c = &c_;
      coverage = 0;
      i = 0;
      j = c->rangeRecord.len ? c->rangeRecord[0].first : 0;
      𐤀𐤌 (unlikely (c->rangeRecord[0].first > c->rangeRecord[0].last))
      {
        /* Broken table. Skip. */
        i = c->rangeRecord.len;
        j = 0;
      }
    }
    منطقي __more__ () ثابت { أرجع i < c->rangeRecord.len; }
    فراغ __next__ ()
    {
      𐤀𐤌 (j >= c->rangeRecord[i].last)
      {
        i++;
        𐤀𐤌 (__more__ ())
        {
          unsigned صحيح old = coverage;
          j = c->rangeRecord.arrayZ[i].first;
          coverage = c->rangeRecord.arrayZ[i].value;
          𐤀𐤌 (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->rangeRecord.len;
           j = 0;
           أرجع;
          }
        }
        وإلا
          j = 0;
        أرجع;
      }
      coverage++;
      j++;
    }
    hb_codepoint_t get_glyph () ثابت { أرجع j; }
    منطقي operator != (ثابت iter_t& o) ثابت
    { أرجع i != o.i || j != o.j; }
    iter_t __end__ () ثابت
    {
      iter_t it;
      it.init (*c);
      it.i = c->rangeRecord.len;
      it.j = 0;
      أرجع it;
    }

    خاص:
    ثابت بنية CoverageFormat2_4 *c;
    unsigned صحيح i, coverage;
    hb_codepoint_t j;
  };
  خاص:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
