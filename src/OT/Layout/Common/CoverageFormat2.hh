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

#جے_بنا_تعریف OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
#تعریف_کرو OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH

#ضمن "RangeRecord.hh"

نطاق OT {
نطاق Layout {
نطاق Common {

سانچا <قسم_ناں Types>
بنية CoverageFormat2_4
{
  یار بنية Coverage;
  ورتوں Coverage_Range_ریکارڈ = Range_ریکارڈ<Types, قسم_ناں Types::HBUINT>;

  عام:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  قسم_ناں Types::سانچا Sorted_Array_دا<Coverage_Range_ریکارڈ>
                range_ریکارڈ;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  عام:
  DEFINE_ناپ_ARRAY (2 + Types::HBUINT::static_ناپ, range_ریکارڈ);

  خاص:

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (range_ریکارڈ.sanitize (c));
  }

  بنانشانی صحيح get_coverage (hb_codepoint_t glyph_id) ثابت
  {
    ثابت Coverage_Range_ریکارڈ &range = range_ریکارڈ.bsearch (glyph_id);
    أرجع likely (range.پہلا <= range.آخری)
         ? (بنانشانی صحيح) range.value + (glyph_id - range.پہلا)
         : NOT_COVERED;
  }

  بنانشانی get_population () ثابت
  {
    قسم_ناں Types::large_int ret = 0;
    لكل (ثابت خودکار &r : range_ریکارڈ)
      ret += r.get_population ();
    أرجع ret > UINT_MAX ? UINT_MAX : (بنانشانی) ret;
  }

  سانچا <قسم_ناں Iterator,
      hb_requires (hb_is_sorted_سروت_دا (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    بنانشانی عدد_ranges = 0;
    hb_codepoint_t آخری = (hb_codepoint_t) -2;
    لكل (خودکار g: glyphs)
    {
      إذا (آخری + 1 != g)
        عدد_ranges++;
      آخری = g;
    }

    إذا (unlikely (!range_ریکارڈ.serialize (c, عدد_ranges))) return_trace (خطأ);
    إذا (!عدد_ranges) return_trace (سچ);

    بنانشانی گنتی = 0;
    بنانشانی range = (بنانشانی) -1;
    آخری = (hb_codepoint_t) -2;
    بنانشانی unsorted = خطأ;
    لكل (خودکار g: glyphs)
    {
      إذا (آخری + 1 != g)
      {
	إذا (unlikely (آخری != (hb_codepoint_t) -2 && آخری + 1 > g))
	  unsorted = سچ;

        range++;
        range_ریکارڈ.arrayZ[range].پہلا = g;
        range_ریکارڈ.arrayZ[range].value = گنتی;
      }
      range_ریکارڈ.arrayZ[range].آخری = g;
      آخری = g;
      گنتی++;
    }

    إذا (unlikely (unsorted))
      range_ریکارڈ.as_array ().qsort (Coverage_Range_ریکارڈ::cmp_range);

    return_trace (سچ);
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    إذا (range_ریکارڈ.len > glyphs->get_population () * hb_ثنائی_ہندسہ_storage ((بنانشانی) range_ریکارڈ.len))
    {
      لكل (خودکار g : *glyphs)
        إذا (get_coverage (g) != NOT_COVERED)
	  أرجع سچ;
      أرجع خطأ;
    }

    أرجع hb_any (+ hb_iter (range_ریکارڈ)
                   | hb_map ([glyphs] (ثابت Coverage_Range_ریکارڈ &range) { أرجع range.intersects (*glyphs); }));
  }
  منطقي intersects_coverage (ثابت hb_سیٹ_t *glyphs, بنانشانی صحيح اشاریہ_2) ثابت
  {
    خودکار *range = range_ریکارڈ.as_array ().bsearch (اشاریہ_2);
    إذا (range)
      أرجع range->intersects (*glyphs);
    أرجع خطأ;
  }

  ساكن فراغ جوڑو_intersected_بٹ (hb_سیٹ_t &سیٹ, hb_codepoint_t ادھار, uint64_t بٹ)
  { سیٹ.جوڑو_بٹ (ادھار, بٹ); }

  سانچا <قسم_ناں Iterable_باہرلا>
  ساكن فراغ جوڑو_intersected_بٹ (Iterable_باہرلا&& باہرلا, hb_codepoint_t ادھار, uint64_t بٹ)
  {
    طالما (بٹ)
    {
      باہرلا << ادھار + hb_ctz (بٹ);
      بٹ &= بٹ - 1;
    }
  }

  سانچا <قسم_ناں Iterable_باہرلا,
	    hb_requires (hb_is_sink_دا (Iterable_باہرلا, hb_codepoint_t))>
  فراغ intersect_سیٹ (ثابت hb_سیٹ_t &glyphs, Iterable_باہرلا&& intersect_glyphs) ثابت
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t آخری = 0;
    لكل (ثابت خودکار& range : range_ریکارڈ)
    {
      إذا (unlikely (range.پہلا < آخری))
        اكسر;
      آخری = range.آخری;
      hb_codepoint_t cursor = range.پہلا & ~hb_ثنائی_ہندسہ_ورقہ_t::ELT_نقاب;
      cursor = cursor ? cursor - 1 : HB_سیٹ_قدر_INVALID;
      uint64_t بٹ;
      طالما (glyphs.اگلا_بٹ (&cursor, &بٹ) && cursor <= آخری)
      {
	إذا (range.پہلا > cursor)
	  بٹ &= UINT64_MAX << (range.پہلا - cursor);
	بنانشانی صحيح سرا = آخری - cursor;
	إذا (سرا < hb_ثنائی_ہندسہ_ورقہ_t::ELT_بٹ - 1)
	  بٹ &= (uint64_t (1) << (سرا + 1)) - 1;
	جوڑو_intersected_بٹ (intersect_glyphs, cursor, بٹ);
      }
    }
  }

  بنانشانی لاگت () ثابت { أرجع hb_ثنائی_ہندسہ_storage ((بنانشانی) range_ریکارڈ.len); /* bsearch cost */ }

  سانچا <قسم_ناں سیٹ_t>
  منطقي collect_coverage (سیٹ_t *glyphs) ثابت
  {
    لكل (ثابت خودکار& range: range_ریکارڈ)
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
      j = c->range_ریکارڈ.len ? c->range_ریکارڈ[0].پہلا : 0;
      إذا (unlikely (c->range_ریکارڈ[0].پہلا > c->range_ریکارڈ[0].آخری))
      {
        /* Broken table. Skip. */
        i = c->range_ریکارڈ.len;
        j = 0;
      }
    }
    منطقي __more__ () ثابت { أرجع i < c->range_ریکارڈ.len; }
    فراغ __next__ ()
    {
      إذا (j >= c->range_ریکارڈ[i].آخری)
      {
        i++;
        إذا (__more__ ())
        {
          بنانشانی صحيح old = coverage;
          j = c->range_ریکارڈ.arrayZ[i].پہلا;
          coverage = c->range_ریکارڈ.arrayZ[i].value;
          إذا (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->range_ریکارڈ.len;
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
      it.i = c->range_ریکارڈ.len;
      it.j = 0;
      أرجع it;
    }

    خاص:
    ثابت بنية CoverageFormat2_4 *c;
    بنانشانی صحيح i, coverage;
    hb_codepoint_t j;
  };
  خاص:
};

}
}
}

#جے_انت  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
