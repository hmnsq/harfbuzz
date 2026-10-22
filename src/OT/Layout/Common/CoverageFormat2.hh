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

ढाँचा <typename Types>
संरचना CoverageFormat2_4
{
  मित्र संरचना Coverage;
  using CoverageRangeRecord = RangeRecord<Types, typename Types::HBUINT>;

  सार्वजनिक:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  typename Types::ढाँचा SortedArrayOf<CoverageRangeRecord>
                rangeRecord;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, rangeRecord);

  खाजगी:

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (rangeRecord.sanitize (c));
  }

  अचिन्हित पूर्णांक get_coverage (hb_codepoint_t glyph_id) स्थिरांक
  {
    स्थिरांक CoverageRangeRecord &range = rangeRecord.द्वा_खोज (glyph_id);
    परत likely (range.first <= range.last)
         ? (अचिन्हित पूर्णांक) range.value + (glyph_id - range.first)
         : NOT_COVERED;
  }

  अचिन्हित get_population () स्थिरांक
  {
    typename Types::large_int ret = 0;
    साठी (स्थिरांक स्वतः &r : rangeRecord)
      ret += r.get_population ();
    परत ret > अच_पूर्णांक_उच्च ? अच_पूर्णांक_उच्च : (अचिन्हित) ret;
  }

  ढाँचा <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);

    अचिन्हित अंक_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    साठी (स्वतः g: glyphs)
    {
      जर (last + 1 != g)
        अंक_ranges++;
      last = g;
    }

    जर (unlikely (!rangeRecord.serialize (c, अंक_ranges))) return_trace (असत्य);
    जर (!अंक_ranges) return_trace (सत्य);

    अचिन्हित मोजणी = 0;
    अचिन्हित range = (अचिन्हित) -1;
    last = (hb_codepoint_t) -2;
    अचिन्हित unsorted = असत्य;
    साठी (स्वतः g: glyphs)
    {
      जर (last + 1 != g)
      {
	जर (unlikely (last != (hb_codepoint_t) -2 && last + 1 > g))
	  unsorted = सत्य;

        range++;
        rangeRecord.arrayZ[range].first = g;
        rangeRecord.arrayZ[range].value = मोजणी;
      }
      rangeRecord.arrayZ[range].last = g;
      last = g;
      मोजणी++;
    }

    जर (unlikely (unsorted))
      rangeRecord.as_array ().क्विक (CoverageRangeRecord::cmp_range);

    return_trace (सत्य);
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    जर (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((अचिन्हित) rangeRecord.len))
    {
      साठी (स्वतः g : *glyphs)
        जर (get_coverage (g) != NOT_COVERED)
	  परत सत्य;
      परत असत्य;
    }

    परत hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (स्थिरांक CoverageRangeRecord &range) { परत range.intersects (*glyphs); }));
  }
  तार्किक intersects_coverage (स्थिरांक hb_set_t *glyphs, अचिन्हित पूर्णांक index) स्थिरांक
  {
    स्वतः *range = rangeRecord.as_array ().द्वा_खोज (index);
    जर (range)
      परत range->intersects (*glyphs);
    परत असत्य;
  }

  स्थिर शून्य add_intersected_bits (hb_set_t &set, hb_codepoint_t base, uint64_t bits)
  { set.add_bits (base, bits); }

  ढाँचा <typename IterableOut>
  स्थिर शून्य add_intersected_bits (IterableOut&& out, hb_codepoint_t base, uint64_t bits)
  {
    पर्यंत (bits)
    {
      out << base + hb_ctz (bits);
      bits &= bits - 1;
    }
  }

  ढाँचा <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  शून्य intersect_set (स्थिरांक hb_set_t &glyphs, IterableOut&& intersect_glyphs) स्थिरांक
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t last = 0;
    साठी (स्थिरांक स्वतः& range : rangeRecord)
    {
      जर (unlikely (range.first < last))
        थांबा;
      last = range.last;
      hb_codepoint_t cursor = range.first & ~hb_bit_page_t::ELT_MASK;
      cursor = cursor ? cursor - 1 : HB_SET_मूल्य_INVALID;
      uint64_t bits;
      पर्यंत (glyphs.next_bits (&cursor, &bits) && cursor <= last)
      {
	जर (range.first > cursor)
	  bits &= UINT64_MAX << (range.first - cursor);
	अचिन्हित पूर्णांक end = last - cursor;
	जर (end < hb_bit_page_t::ELT_BITS - 1)
	  bits &= (uint64_t (1) << (end + 1)) - 1;
	add_intersected_bits (intersect_glyphs, cursor, bits);
      }
    }
  }

  अचिन्हित cost () स्थिरांक { परत hb_bit_storage ((अचिन्हित) rangeRecord.len); /* bsearch cost */ }

  ढाँचा <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) स्थिरांक
  {
    साठी (स्थिरांक स्वतः& range: rangeRecord)
      जर (unlikely (!range.collect_coverage (glyphs)))
        परत असत्य;
    परत सत्य;
  }

  सार्वजनिक:
  /* Older compilers need this to be public. */
  संरचना iter_t
  {
    शून्य init (स्थिरांक CoverageFormat2_4 &c_)
    {
      c = &c_;
      coverage = 0;
      i = 0;
      j = c->rangeRecord.len ? c->rangeRecord[0].first : 0;
      जर (unlikely (c->rangeRecord[0].first > c->rangeRecord[0].last))
      {
        /* Broken table. Skip. */
        i = c->rangeRecord.len;
        j = 0;
      }
    }
    तार्किक __more__ () स्थिरांक { परत i < c->rangeRecord.len; }
    शून्य __next__ ()
    {
      जर (j >= c->rangeRecord[i].last)
      {
        i++;
        जर (__more__ ())
        {
          अचिन्हित पूर्णांक old = coverage;
          j = c->rangeRecord.arrayZ[i].first;
          coverage = c->rangeRecord.arrayZ[i].value;
          जर (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->rangeRecord.len;
           j = 0;
           परत;
          }
        }
        अन्यथा
          j = 0;
        परत;
      }
      coverage++;
      j++;
    }
    hb_codepoint_t get_glyph () स्थिरांक { परत j; }
    तार्किक चालक != (स्थिरांक iter_t& o) स्थिरांक
    { परत i != o.i || j != o.j; }
    iter_t __end__ () स्थिरांक
    {
      iter_t it;
      it.init (*c);
      it.i = c->rangeRecord.len;
      it.j = 0;
      परत it;
    }

    खाजगी:
    स्थिरांक संरचना CoverageFormat2_4 *c;
    अचिन्हित पूर्णांक i, coverage;
    hb_codepoint_t j;
  };
  खाजगी:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
