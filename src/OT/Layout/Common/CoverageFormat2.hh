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

#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
#وصف_ڪريو OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH

#ضمن "RangeRecord.hh"

نطاق OT {
نطاق Layout {
نطاق Common {

سانچو <قسم_نالو Types>
بنية CoverageFormat2_4
{
  دوست بنية Coverage;
  استعمال Coverage_Range_رڪارڊ = Range_رڪارڊ<Types, قسم_نالو Types::HBUINT>;

  عام:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  قسم_نالو Types::سانچو Sorted_Array_جو<Coverage_Range_رڪارڊ>
                range_رڪارڊ;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  عام:
  DEFINE_سائيز_ARRAY (2 + Types::HBUINT::static_سائيز, range_رڪارڊ);

  خاص:

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (range_رڪارڊ.sanitize (c));
  }

  بنانشان صحيح get_coverage (hb_codepoint_t glyph_id) ثابت
  {
    ثابت Coverage_Range_رڪارڊ &range = range_رڪارڊ.bsearch (glyph_id);
    أرجع likely (range.پهريون <= range.آخري)
         ? (بنانشان صحيح) range.value + (glyph_id - range.پهريون)
         : NOT_COVERED;
  }

  بنانشان get_population () ثابت
  {
    قسم_نالو Types::large_int ret = 0;
    لكل (ثابت خودڪار &r : range_رڪارڊ)
      ret += r.get_population ();
    أرجع ret > UINT_MAX ? UINT_MAX : (بنانشان) ret;
  }

  سانچو <قسم_نالو Iterator,
      hb_requires (hb_is_sorted_ذريعو_جو (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    بنانشان انگ_ranges = 0;
    hb_codepoint_t آخري = (hb_codepoint_t) -2;
    لكل (خودڪار g: glyphs)
    {
      إذا (آخري + 1 != g)
        انگ_ranges++;
      آخري = g;
    }

    إذا (unlikely (!range_رڪارڊ.serialize (c, انگ_ranges))) return_trace (خطأ);
    إذا (!انگ_ranges) return_trace (سچ);

    بنانشان ڳڻپ = 0;
    بنانشان range = (بنانشان) -1;
    آخري = (hb_codepoint_t) -2;
    بنانشان unsorted = خطأ;
    لكل (خودڪار g: glyphs)
    {
      إذا (آخري + 1 != g)
      {
	إذا (unlikely (آخري != (hb_codepoint_t) -2 && آخري + 1 > g))
	  unsorted = سچ;

        range++;
        range_رڪارڊ.arrayZ[range].پهريون = g;
        range_رڪارڊ.arrayZ[range].value = ڳڻپ;
      }
      range_رڪارڊ.arrayZ[range].آخري = g;
      آخري = g;
      ڳڻپ++;
    }

    إذا (unlikely (unsorted))
      range_رڪارڊ.as_array ().qsort (Coverage_Range_رڪارڊ::cmp_range);

    return_trace (سچ);
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    إذا (range_رڪارڊ.len > glyphs->get_population () * hb_ٻٽو_انگ_storage ((بنانشان) range_رڪارڊ.len))
    {
      لكل (خودڪار g : *glyphs)
        إذا (get_coverage (g) != NOT_COVERED)
	  أرجع سچ;
      أرجع خطأ;
    }

    أرجع hb_any (+ hb_iter (range_رڪارڊ)
                   | hb_map ([glyphs] (ثابت Coverage_Range_رڪارڊ &range) { أرجع range.intersects (*glyphs); }));
  }
  منطقي intersects_coverage (ثابت hb_سيٽ_t *glyphs, بنانشان صحيح اشاريو_2) ثابت
  {
    خودڪار *range = range_رڪارڊ.as_array ().bsearch (اشاريو_2);
    إذا (range)
      أرجع range->intersects (*glyphs);
    أرجع خطأ;
  }

  ساكن فراغ جوڙيو_intersected_بٽ (hb_سيٽ_t &سيٽ, hb_codepoint_t بنياد, uint64_t بٽ)
  { سيٽ.جوڙيو_بٽ (بنياد, بٽ); }

  سانچو <قسم_نالو Iterable_ٻاهريون>
  ساكن فراغ جوڙيو_intersected_بٽ (Iterable_ٻاهريون&& ٻاهريون, hb_codepoint_t بنياد, uint64_t بٽ)
  {
    طالما (بٽ)
    {
      ٻاهريون << بنياد + hb_ctz (بٽ);
      بٽ &= بٽ - 1;
    }
  }

  سانچو <قسم_نالو Iterable_ٻاهريون,
	    hb_requires (hb_is_sink_جو (Iterable_ٻاهريون, hb_codepoint_t))>
  فراغ intersect_سيٽ (ثابت hb_سيٽ_t &glyphs, Iterable_ٻاهريون&& intersect_glyphs) ثابت
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t آخري = 0;
    لكل (ثابت خودڪار& range : range_رڪارڊ)
    {
      إذا (unlikely (range.پهريون < آخري))
        اكسر;
      آخري = range.آخري;
      hb_codepoint_t cursor = range.پهريون & ~hb_ٻٽو_انگ_صفحو_t::ELT_نقاب;
      cursor = cursor ? cursor - 1 : HB_سيٽ_قدر_INVALID;
      uint64_t بٽ;
      طالما (glyphs.ايندڙ_بٽ (&cursor, &بٽ) && cursor <= آخري)
      {
	إذا (range.پهريون > cursor)
	  بٽ &= UINT64_MAX << (range.پهريون - cursor);
	بنانشان صحيح پڇاڙي = آخري - cursor;
	إذا (پڇاڙي < hb_ٻٽو_انگ_صفحو_t::ELT_بٽ - 1)
	  بٽ &= (uint64_t (1) << (پڇاڙي + 1)) - 1;
	جوڙيو_intersected_بٽ (intersect_glyphs, cursor, بٽ);
      }
    }
  }

  بنانشان لاڳت () ثابت { أرجع hb_ٻٽو_انگ_storage ((بنانشان) range_رڪارڊ.len); /* bsearch cost */ }

  سانچو <قسم_نالو سيٽ_t>
  منطقي collect_coverage (سيٽ_t *glyphs) ثابت
  {
    لكل (ثابت خودڪار& range: range_رڪارڊ)
      إذا (unlikely (!range.collect_coverage (glyphs)))
        أرجع خطأ;
    أرجع سچ;
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
      j = c->range_رڪارڊ.len ? c->range_رڪارڊ[0].پهريون : 0;
      إذا (unlikely (c->range_رڪارڊ[0].پهريون > c->range_رڪارڊ[0].آخري))
      {
        /* Broken table. Skip. */
        i = c->range_رڪارڊ.len;
        j = 0;
      }
    }
    منطقي __more__ () ثابت { أرجع i < c->range_رڪارڊ.len; }
    فراغ __next__ ()
    {
      إذا (j >= c->range_رڪارڊ[i].آخري)
      {
        i++;
        إذا (__more__ ())
        {
          بنانشان صحيح old = coverage;
          j = c->range_رڪارڊ.arrayZ[i].پهريون;
          coverage = c->range_رڪارڊ.arrayZ[i].value;
          إذا (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->range_رڪارڊ.len;
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
    منطقي عامل != (ثابت iter_t& o) ثابت
    { أرجع i != o.i || j != o.j; }
    iter_t __end__ () ثابت
    {
      iter_t it;
      it.init (*c);
      it.i = c->range_رڪارڊ.len;
      it.j = 0;
      أرجع it;
    }

    خاص:
    ثابت بنية CoverageFormat2_4 *c;
    بنانشان صحيح i, coverage;
    hb_codepoint_t j;
  };
  خاص:
};

}
}
}

#جيڪڏهن_پورو  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
