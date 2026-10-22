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

#अगर_अघोषित OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
#घोषणा OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH

#समावेश "RangeRecord.hh"

namespace OT {
namespace Layout {
namespace Common {

ढाँचा <typename Types>
काष्ठा CoverageFormat2_4
{
  मित्र काष्ठा Coverage;
  using CoverageRangeRecord = RangeRecord<Types, typename Types::HBUINT>;

  खुला:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  typename Types::ढाँचा SortedArrayOf<CoverageRangeRecord>
                rangeRecord;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  खुला:
  DEFINE_आकार_ARRAY (2 + Types::HBUINT::static_आकार, rangeRecord);

  निजी:

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (rangeRecord.sanitize (c));
  }

  अचिन्हित पूर्णांक get_coverage (hb_codepoint_t glyph_id) स्थिर
  {
    स्थिर CoverageRangeRecord &range = rangeRecord.द्वा_खोज (glyph_id);
    वापस likely (range.first <= range.last)
         ? (अचिन्हित पूर्णांक) range.value + (glyph_id - range.first)
         : NOT_COVERED;
  }

  अचिन्हित get_population () स्थिर
  {
    typename Types::large_int ret = 0;
    क्रम (स्थिर स्वतः &r : rangeRecord)
      ret += r.get_population ();
    वापस ret > अच_पूर्णांक_उच्च ? अच_पूर्णांक_उच्च : (अचिन्हित) ret;
  }

  ढाँचा <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);

    अचिन्हित अंक_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    क्रम (स्वतः g: glyphs)
    {
      अगर (last + 1 != g)
        अंक_ranges++;
      last = g;
    }

    अगर (unlikely (!rangeRecord.serialize (c, अंक_ranges))) return_trace (false);
    अगर (!अंक_ranges) return_trace (true);

    अचिन्हित गिनती = 0;
    अचिन्हित range = (अचिन्हित) -1;
    last = (hb_codepoint_t) -2;
    अचिन्हित unsorted = false;
    क्रम (स्वतः g: glyphs)
    {
      अगर (last + 1 != g)
      {
	अगर (unlikely (last != (hb_codepoint_t) -2 && last + 1 > g))
	  unsorted = true;

        range++;
        rangeRecord.arrayZ[range].first = g;
        rangeRecord.arrayZ[range].value = गिनती;
      }
      rangeRecord.arrayZ[range].last = g;
      last = g;
      गिनती++;
    }

    अगर (unlikely (unsorted))
      rangeRecord.as_array ().क्विक (CoverageRangeRecord::cmp_range);

    return_trace (true);
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    अगर (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((अचिन्हित) rangeRecord.len))
    {
      क्रम (स्वतः g : *glyphs)
        अगर (get_coverage (g) != NOT_COVERED)
	  वापस true;
      वापस false;
    }

    वापस hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (स्थिर CoverageRangeRecord &range) { वापस range.intersects (*glyphs); }));
  }
  bool intersects_coverage (स्थिर hb_set_t *glyphs, अचिन्हित पूर्णांक सूचकांक_2) स्थिर
  {
    स्वतः *range = rangeRecord.as_array ().द्वा_खोज (सूचकांक_2);
    अगर (range)
      वापस range->intersects (*glyphs);
    वापस false;
  }

  अटल व्योम add_intersected_bits (hb_set_t &set, hb_codepoint_t base, uint64_t bits)
  { set.add_bits (base, bits); }

  ढाँचा <typename IterableOut>
  अटल व्योम add_intersected_bits (IterableOut&& out, hb_codepoint_t base, uint64_t bits)
  {
    जबतक (bits)
    {
      out << base + hb_ctz (bits);
      bits &= bits - 1;
    }
  }

  ढाँचा <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  व्योम intersect_set (स्थिर hb_set_t &glyphs, IterableOut&& intersect_glyphs) स्थिर
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t last = 0;
    क्रम (स्थिर स्वतः& range : rangeRecord)
    {
      अगर (unlikely (range.first < last))
        अवरोध;
      last = range.last;
      hb_codepoint_t cursor = range.first & ~hb_bit_page_t::ELT_MASK;
      cursor = cursor ? cursor - 1 : HB_SET_मूल्य_INVALID;
      uint64_t bits;
      जबतक (glyphs.अग्र_bits (&cursor, &bits) && cursor <= last)
      {
	अगर (range.first > cursor)
	  bits &= UINT64_MAX << (range.first - cursor);
	अचिन्हित पूर्णांक end = last - cursor;
	अगर (end < hb_bit_page_t::ELT_BITS - 1)
	  bits &= (uint64_t (1) << (end + 1)) - 1;
	add_intersected_bits (intersect_glyphs, cursor, bits);
      }
    }
  }

  अचिन्हित cost () स्थिर { वापस hb_bit_storage ((अचिन्हित) rangeRecord.len); /* bsearch cost */ }

  ढाँचा <typename set_t>
  bool collect_coverage (set_t *glyphs) स्थिर
  {
    क्रम (स्थिर स्वतः& range: rangeRecord)
      अगर (unlikely (!range.collect_coverage (glyphs)))
        वापस false;
    वापस true;
  }

  खुला:
  /* Older compilers need this to be public. */
  काष्ठा iter_t
  {
    व्योम init (स्थिर CoverageFormat2_4 &c_)
    {
      c = &c_;
      coverage = 0;
      i = 0;
      j = c->rangeRecord.len ? c->rangeRecord[0].first : 0;
      अगर (unlikely (c->rangeRecord[0].first > c->rangeRecord[0].last))
      {
        /* Broken table. Skip. */
        i = c->rangeRecord.len;
        j = 0;
      }
    }
    bool __more__ () स्थिर { वापस i < c->rangeRecord.len; }
    व्योम __next__ ()
    {
      अगर (j >= c->rangeRecord[i].last)
      {
        i++;
        अगर (__more__ ())
        {
          अचिन्हित पूर्णांक old = coverage;
          j = c->rangeRecord.arrayZ[i].first;
          coverage = c->rangeRecord.arrayZ[i].value;
          अगर (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->rangeRecord.len;
           j = 0;
           वापस;
          }
        }
        अन्यथा
          j = 0;
        वापस;
      }
      coverage++;
      j++;
    }
    hb_codepoint_t get_glyph () स्थिर { वापस j; }
    bool चालक != (स्थिर iter_t& o) स्थिर
    { वापस i != o.i || j != o.j; }
    iter_t __end__ () स्थिर
    {
      iter_t it;
      it.init (*c);
      it.i = c->rangeRecord.len;
      it.j = 0;
      वापस it;
    }

    निजी:
    स्थिर काष्ठा CoverageFormat2_4 *c;
    अचिन्हित पूर्णांक i, coverage;
    hb_codepoint_t j;
  };
  निजी:
};

}
}
}

#पूर्ण_अगर  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
