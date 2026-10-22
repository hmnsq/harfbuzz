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

#अन्तर्भाव "RangeRecord.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश Common {

template <typename Types>
संरचना CoverageFormat2_4
{
  friend संरचना Coverage;
  using CoverageRangeRecord = RangeRecord<Types, typename Types::HBUINT>;

  सार्वजनिक:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<CoverageRangeRecord>
                rangeRecord;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, rangeRecord);

  गुप्त:

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned पूर्णाङ्क get_coverage (hb_codepoint_t glyph_id) ध्रुव
  {
    ध्रुव CoverageRangeRecord &range = rangeRecord.bsearch (glyph_id);
    प्रत्यागम likely (range.first <= range.last)
         ? (unsigned पूर्णाङ्क) range.value + (glyph_id - range.first)
         : NOT_COVERED;
  }

  unsigned get_population () ध्रुव
  {
    typename Types::large_int ret = 0;
    प्रति (ध्रुव auto &r : rangeRecord)
      ret += r.get_population ();
    प्रत्यागम ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);

    unsigned अंक_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    प्रति (auto g: glyphs)
    {
      यदि (last + 1 != g)
        अंक_ranges++;
      last = g;
    }

    यदि (unlikely (!rangeRecord.serialize (c, अंक_ranges))) return_trace (असत्य);
    यदि (!अंक_ranges) return_trace (सत्य);

    unsigned गिनती = 0;
    unsigned range = (unsigned) -1;
    last = (hb_codepoint_t) -2;
    unsigned unsorted = असत्य;
    प्रति (auto g: glyphs)
    {
      यदि (last + 1 != g)
      {
	यदि (unlikely (last != (hb_codepoint_t) -2 && last + 1 > g))
	  unsorted = सत्य;

        range++;
        rangeRecord.arrayZ[range].first = g;
        rangeRecord.arrayZ[range].value = गिनती;
      }
      rangeRecord.arrayZ[range].last = g;
      last = g;
      गिनती++;
    }

    यदि (unlikely (unsorted))
      rangeRecord.as_array ().qsort (CoverageRangeRecord::cmp_range);

    return_trace (सत्य);
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    यदि (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      प्रति (auto g : *glyphs)
        यदि (get_coverage (g) != NOT_COVERED)
	  प्रत्यागम सत्य;
      प्रत्यागम असत्य;
    }

    प्रत्यागम hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (ध्रुव CoverageRangeRecord &range) { प्रत्यागम range.intersects (*glyphs); }));
  }
  तार्किक intersects_coverage (ध्रुव hb_set_t *glyphs, unsigned पूर्णाङ्क index) ध्रुव
  {
    auto *range = rangeRecord.as_array ().bsearch (index);
    यदि (range)
      प्रत्यागम range->intersects (*glyphs);
    प्रत्यागम असत्य;
  }

  स्थिर शून्य add_intersected_bits (hb_set_t &set, hb_codepoint_t base, uint64_t bits)
  { set.add_bits (base, bits); }

  template <typename IterableOut>
  स्थिर शून्य add_intersected_bits (IterableOut&& out, hb_codepoint_t base, uint64_t bits)
  {
    यावत् (bits)
    {
      out << base + hb_ctz (bits);
      bits &= bits - 1;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  शून्य intersect_set (ध्रुव hb_set_t &glyphs, IterableOut&& intersect_glyphs) ध्रुव
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t last = 0;
    प्रति (ध्रुव auto& range : rangeRecord)
    {
      यदि (unlikely (range.first < last))
        भङ्ग;
      last = range.last;
      hb_codepoint_t cursor = range.first & ~hb_bit_page_t::ELT_MASK;
      cursor = cursor ? cursor - 1 : HB_SET_मूल्य_INVALID;
      uint64_t bits;
      यावत् (glyphs.next_bits (&cursor, &bits) && cursor <= last)
      {
	यदि (range.first > cursor)
	  bits &= UINT64_MAX << (range.first - cursor);
	unsigned पूर्णाङ्क end = last - cursor;
	यदि (end < hb_bit_page_t::ELT_BITS - 1)
	  bits &= (uint64_t (1) << (end + 1)) - 1;
	add_intersected_bits (intersect_glyphs, cursor, bits);
      }
    }
  }

  unsigned cost () ध्रुव { प्रत्यागम hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) ध्रुव
  {
    प्रति (ध्रुव auto& range: rangeRecord)
      यदि (unlikely (!range.collect_coverage (glyphs)))
        प्रत्यागम असत्य;
    प्रत्यागम सत्य;
  }

  सार्वजनिक:
  /* Older compilers need this to be public. */
  संरचना iter_t
  {
    शून्य init (ध्रुव CoverageFormat2_4 &c_)
    {
      c = &c_;
      coverage = 0;
      i = 0;
      j = c->rangeRecord.len ? c->rangeRecord[0].first : 0;
      यदि (unlikely (c->rangeRecord[0].first > c->rangeRecord[0].last))
      {
        /* Broken table. Skip. */
        i = c->rangeRecord.len;
        j = 0;
      }
    }
    तार्किक __more__ () ध्रुव { प्रत्यागम i < c->rangeRecord.len; }
    शून्य __next__ ()
    {
      यदि (j >= c->rangeRecord[i].last)
      {
        i++;
        यदि (__more__ ())
        {
          unsigned पूर्णाङ्क old = coverage;
          j = c->rangeRecord.arrayZ[i].first;
          coverage = c->rangeRecord.arrayZ[i].value;
          यदि (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->rangeRecord.len;
           j = 0;
           प्रत्यागम;
          }
        }
        अन्यथा
          j = 0;
        प्रत्यागम;
      }
      coverage++;
      j++;
    }
    hb_codepoint_t get_glyph () ध्रुव { प्रत्यागम j; }
    तार्किक operator != (ध्रुव iter_t& o) ध्रुव
    { प्रत्यागम i != o.i || j != o.j; }
    iter_t __end__ () ध्रुव
    {
      iter_t it;
      it.init (*c);
      it.i = c->rangeRecord.len;
      it.j = 0;
      प्रत्यागम it;
    }

    गुप्त:
    ध्रुव संरचना CoverageFormat2_4 *c;
    unsigned पूर्णाङ्क i, coverage;
    hb_codepoint_t j;
  };
  गुप्त:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
