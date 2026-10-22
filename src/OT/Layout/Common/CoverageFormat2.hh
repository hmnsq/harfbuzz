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

#അന്തര്ഭാവ "RangeRecord.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ Common {

template <typename Types>
ഘടന CoverageFormat2_4
{
  friend ഘടന Coverage;
  using CoverageRangeRecord = RangeRecord<Types, typename Types::HBUINT>;

  പൊതു:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<CoverageRangeRecord>
                rangeRecord;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  പൊതു:
  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, rangeRecord);

  സ്വകാര്യ:

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned പൂർണ്ണസംഖ്യ get_coverage (hb_codepoint_t glyph_id) സ്ഥിരം
  {
    സ്ഥിരം CoverageRangeRecord &range = rangeRecord.bsearch (glyph_id);
    തിരികെ likely (range.first <= range.last)
         ? (unsigned പൂർണ്ണസംഖ്യ) range.value + (glyph_id - range.first)
         : NOT_COVERED;
  }

  unsigned get_population () സ്ഥിരം
  {
    typename Types::large_int ret = 0;
    വേണ്ടി (സ്ഥിരം auto &r : rangeRecord)
      ret += r.get_population ();
    തിരികെ ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  തർക്കം serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);

    unsigned അക്കം_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    വേണ്ടി (auto g: glyphs)
    {
      എങ്കിൽ (last + 1 != g)
        അക്കം_ranges++;
      last = g;
    }

    എങ്കിൽ (unlikely (!rangeRecord.serialize (c, അക്കം_ranges))) return_trace (അസത്യം);
    എങ്കിൽ (!അക്കം_ranges) return_trace (സത്യം);

    unsigned എണ്ണം = 0;
    unsigned range = (unsigned) -1;
    last = (hb_codepoint_t) -2;
    unsigned unsorted = അസത്യം;
    വേണ്ടി (auto g: glyphs)
    {
      എങ്കിൽ (last + 1 != g)
      {
	എങ്കിൽ (unlikely (last != (hb_codepoint_t) -2 && last + 1 > g))
	  unsorted = സത്യം;

        range++;
        rangeRecord.arrayZ[range].first = g;
        rangeRecord.arrayZ[range].value = എണ്ണം;
      }
      rangeRecord.arrayZ[range].last = g;
      last = g;
      എണ്ണം++;
    }

    എങ്കിൽ (unlikely (unsorted))
      rangeRecord.as_array ().qsort (CoverageRangeRecord::cmp_range);

    return_trace (സത്യം);
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    എങ്കിൽ (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      വേണ്ടി (auto g : *glyphs)
        എങ്കിൽ (get_coverage (g) != NOT_COVERED)
	  തിരികെ സത്യം;
      തിരികെ അസത്യം;
    }

    തിരികെ hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (സ്ഥിരം CoverageRangeRecord &range) { തിരികെ range.intersects (*glyphs); }));
  }
  തർക്കം intersects_coverage (സ്ഥിരം hb_set_t *glyphs, unsigned പൂർണ്ണസംഖ്യ index) സ്ഥിരം
  {
    auto *range = rangeRecord.as_array ().bsearch (index);
    എങ്കിൽ (range)
      തിരികെ range->intersects (*glyphs);
    തിരികെ അസത്യം;
  }

  സ്ഥിതം ശൂന്യം add_intersected_bits (hb_set_t &set, hb_codepoint_t base, uint64_t bits)
  { set.add_bits (base, bits); }

  template <typename IterableOut>
  സ്ഥിതം ശൂന്യം add_intersected_bits (IterableOut&& out, hb_codepoint_t base, uint64_t bits)
  {
    ആകുമ്പോൾ (bits)
    {
      out << base + hb_ctz (bits);
      bits &= bits - 1;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  ശൂന്യം intersect_set (സ്ഥിരം hb_set_t &glyphs, IterableOut&& intersect_glyphs) സ്ഥിരം
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t last = 0;
    വേണ്ടി (സ്ഥിരം auto& range : rangeRecord)
    {
      എങ്കിൽ (unlikely (range.first < last))
        നിർത്തുക;
      last = range.last;
      hb_codepoint_t cursor = range.first & ~hb_bit_page_t::ELT_MASK;
      cursor = cursor ? cursor - 1 : HB_SET_മൂല്യം_INVALID;
      uint64_t bits;
      ആകുമ്പോൾ (glyphs.next_bits (&cursor, &bits) && cursor <= last)
      {
	എങ്കിൽ (range.first > cursor)
	  bits &= UINT64_MAX << (range.first - cursor);
	unsigned പൂർണ്ണസംഖ്യ end = last - cursor;
	എങ്കിൽ (end < hb_bit_page_t::ELT_BITS - 1)
	  bits &= (uint64_t (1) << (end + 1)) - 1;
	add_intersected_bits (intersect_glyphs, cursor, bits);
      }
    }
  }

  unsigned cost () സ്ഥിരം { തിരികെ hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  തർക്കം collect_coverage (set_t *glyphs) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം auto& range: rangeRecord)
      എങ്കിൽ (unlikely (!range.collect_coverage (glyphs)))
        തിരികെ അസത്യം;
    തിരികെ സത്യം;
  }

  പൊതു:
  /* Older compilers need this to be public. */
  ഘടന iter_t
  {
    ശൂന്യം init (സ്ഥിരം CoverageFormat2_4 &c_)
    {
      c = &c_;
      coverage = 0;
      i = 0;
      j = c->rangeRecord.len ? c->rangeRecord[0].first : 0;
      എങ്കിൽ (unlikely (c->rangeRecord[0].first > c->rangeRecord[0].last))
      {
        /* Broken table. Skip. */
        i = c->rangeRecord.len;
        j = 0;
      }
    }
    തർക്കം __more__ () സ്ഥിരം { തിരികെ i < c->rangeRecord.len; }
    ശൂന്യം __next__ ()
    {
      എങ്കിൽ (j >= c->rangeRecord[i].last)
      {
        i++;
        എങ്കിൽ (__more__ ())
        {
          unsigned പൂർണ്ണസംഖ്യ old = coverage;
          j = c->rangeRecord.arrayZ[i].first;
          coverage = c->rangeRecord.arrayZ[i].value;
          എങ്കിൽ (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->rangeRecord.len;
           j = 0;
           തിരികെ;
          }
        }
        അല്ലെങ്കിൽ
          j = 0;
        തിരികെ;
      }
      coverage++;
      j++;
    }
    hb_codepoint_t get_glyph () സ്ഥിരം { തിരികെ j; }
    തർക്കം operator != (സ്ഥിരം iter_t& o) സ്ഥിരം
    { തിരികെ i != o.i || j != o.j; }
    iter_t __end__ () സ്ഥിരം
    {
      iter_t it;
      it.init (*c);
      it.i = c->rangeRecord.len;
      it.j = 0;
      തിരികെ it;
    }

    സ്വകാര്യ:
    സ്ഥിരം ഘടന CoverageFormat2_4 *c;
    unsigned പൂർണ്ണസംഖ്യ i, coverage;
    hb_codepoint_t j;
  };
  സ്വകാര്യ:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
