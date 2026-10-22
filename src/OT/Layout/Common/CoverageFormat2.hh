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

#அந்தர்பாவ "RangeRecord.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ Common {

template <typename Types>
கட்டமைப்பு CoverageFormat2_4
{
  friend கட்டமைப்பு Coverage;
  using CoverageRangeRecord = RangeRecord<Types, typename Types::HBUINT>;

  பொது:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<CoverageRangeRecord>
                rangeRecord;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  பொது:
  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, rangeRecord);

  தனிப்பட்ட:

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned முழு get_coverage (hb_codepoint_t glyph_id) மாறிலி
  {
    மாறிலி CoverageRangeRecord &range = rangeRecord.bsearch (glyph_id);
    திரும்பு likely (range.first <= range.last)
         ? (unsigned முழு) range.value + (glyph_id - range.first)
         : NOT_COVERED;
  }

  unsigned get_population () மாறிலி
  {
    typename Types::large_int ret = 0;
    க்கு (மாறிலி auto &r : rangeRecord)
      ret += r.get_population ();
    திரும்பு ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  தருக்க serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);

    unsigned இலக்கம்_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    க்கு (auto g: glyphs)
    {
      எனில் (last + 1 != g)
        இலக்கம்_ranges++;
      last = g;
    }

    எனில் (unlikely (!rangeRecord.serialize (c, இலக்கம்_ranges))) return_trace (பொய்);
    எனில் (!இலக்கம்_ranges) return_trace (உண்மை);

    unsigned மொத்தம் = 0;
    unsigned range = (unsigned) -1;
    last = (hb_codepoint_t) -2;
    unsigned unsorted = பொய்;
    க்கு (auto g: glyphs)
    {
      எனில் (last + 1 != g)
      {
	எனில் (unlikely (last != (hb_codepoint_t) -2 && last + 1 > g))
	  unsorted = உண்மை;

        range++;
        rangeRecord.arrayZ[range].first = g;
        rangeRecord.arrayZ[range].value = மொத்தம்;
      }
      rangeRecord.arrayZ[range].last = g;
      last = g;
      மொத்தம்++;
    }

    எனில் (unlikely (unsorted))
      rangeRecord.as_array ().qsort (CoverageRangeRecord::cmp_range);

    return_trace (உண்மை);
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    எனில் (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      க்கு (auto g : *glyphs)
        எனில் (get_coverage (g) != NOT_COVERED)
	  திரும்பு உண்மை;
      திரும்பு பொய்;
    }

    திரும்பு hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (மாறிலி CoverageRangeRecord &range) { திரும்பு range.intersects (*glyphs); }));
  }
  தருக்க intersects_coverage (மாறிலி hb_set_t *glyphs, unsigned முழு index) மாறிலி
  {
    auto *range = rangeRecord.as_array ().bsearch (index);
    எனில் (range)
      திரும்பு range->intersects (*glyphs);
    திரும்பு பொய்;
  }

  நிலையான வெறுமை add_intersected_bits (hb_set_t &set, hb_codepoint_t base, uint64_t bits)
  { set.add_bits (base, bits); }

  template <typename IterableOut>
  நிலையான வெறுமை add_intersected_bits (IterableOut&& out, hb_codepoint_t base, uint64_t bits)
  {
    போது (bits)
    {
      out << base + hb_ctz (bits);
      bits &= bits - 1;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  வெறுமை intersect_set (மாறிலி hb_set_t &glyphs, IterableOut&& intersect_glyphs) மாறிலி
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t last = 0;
    க்கு (மாறிலி auto& range : rangeRecord)
    {
      எனில் (unlikely (range.first < last))
        நிறுத்து;
      last = range.last;
      hb_codepoint_t cursor = range.first & ~hb_bit_page_t::ELT_MASK;
      cursor = cursor ? cursor - 1 : HB_SET_மதிப்பு_INVALID;
      uint64_t bits;
      போது (glyphs.next_bits (&cursor, &bits) && cursor <= last)
      {
	எனில் (range.first > cursor)
	  bits &= UINT64_MAX << (range.first - cursor);
	unsigned முழு end = last - cursor;
	எனில் (end < hb_bit_page_t::ELT_BITS - 1)
	  bits &= (uint64_t (1) << (end + 1)) - 1;
	add_intersected_bits (intersect_glyphs, cursor, bits);
      }
    }
  }

  unsigned cost () மாறிலி { திரும்பு hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  தருக்க collect_coverage (set_t *glyphs) மாறிலி
  {
    க்கு (மாறிலி auto& range: rangeRecord)
      எனில் (unlikely (!range.collect_coverage (glyphs)))
        திரும்பு பொய்;
    திரும்பு உண்மை;
  }

  பொது:
  /* Older compilers need this to be public. */
  கட்டமைப்பு iter_t
  {
    வெறுமை init (மாறிலி CoverageFormat2_4 &c_)
    {
      c = &c_;
      coverage = 0;
      i = 0;
      j = c->rangeRecord.len ? c->rangeRecord[0].first : 0;
      எனில் (unlikely (c->rangeRecord[0].first > c->rangeRecord[0].last))
      {
        /* Broken table. Skip. */
        i = c->rangeRecord.len;
        j = 0;
      }
    }
    தருக்க __more__ () மாறிலி { திரும்பு i < c->rangeRecord.len; }
    வெறுமை __next__ ()
    {
      எனில் (j >= c->rangeRecord[i].last)
      {
        i++;
        எனில் (__more__ ())
        {
          unsigned முழு old = coverage;
          j = c->rangeRecord.arrayZ[i].first;
          coverage = c->rangeRecord.arrayZ[i].value;
          எனில் (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->rangeRecord.len;
           j = 0;
           திரும்பு;
          }
        }
        இல்லையெனில்
          j = 0;
        திரும்பு;
      }
      coverage++;
      j++;
    }
    hb_codepoint_t get_glyph () மாறிலி { திரும்பு j; }
    தருக்க operator != (மாறிலி iter_t& o) மாறிலி
    { திரும்பு i != o.i || j != o.j; }
    iter_t __end__ () மாறிலி
    {
      iter_t it;
      it.init (*c);
      it.i = c->rangeRecord.len;
      it.j = 0;
      திரும்பு it;
    }

    தனிப்பட்ட:
    மாறிலி கட்டமைப்பு CoverageFormat2_4 *c;
    unsigned முழு i, coverage;
    hb_codepoint_t j;
  };
  தனிப்பட்ட:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
