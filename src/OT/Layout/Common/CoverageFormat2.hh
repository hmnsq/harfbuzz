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

#که_نه_تعریف_شوی OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
#تعریف_کړه OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH

#ضمن "RangeRecord.hh"

نطاق OT {
نطاق Layout {
نطاق Common {

کينډۍ <ډول_نوم Types>
بنية CoverageFormat2_4
{
  ملګری بنية Coverage;
  کارول Coverage_Range_ریکارډ = Range_ریکارډ<Types, ډول_نوم Types::HBUINT>;

  عام:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  ډول_نوم Types::کينډۍ Sorted_Array_د<Coverage_Range_ریکارډ>
                range_ریکارډ;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::HBUINT::static_اندازه, range_ریکارډ);

  خاص:

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (range_ریکارډ.sanitize (c));
  }

  بینښه صحيح get_coverage (hb_codepoint_t glyph_id) ثابت
  {
    ثابت Coverage_Range_ریکارډ &range = range_ریکارډ.bsearch (glyph_id);
    أرجع likely (range.لومړی <= range.وروستی)
         ? (بینښه صحيح) range.value + (glyph_id - range.لومړی)
         : NOT_COVERED;
  }

  بینښه get_population () ثابت
  {
    ډول_نوم Types::large_int ret = 0;
    لكل (ثابت خپلکاری &r : range_ریکارډ)
      ret += r.get_population ();
    أرجع ret > UINT_MAX ? UINT_MAX : (بینښه) ret;
  }

  کينډۍ <ډول_نوم Iterator,
      hb_requires (hb_is_sorted_سرچینه_د (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    بینښه عدد_ranges = 0;
    hb_codepoint_t وروستی = (hb_codepoint_t) -2;
    لكل (خپلکاری g: glyphs)
    {
      إذا (وروستی + 1 != g)
        عدد_ranges++;
      وروستی = g;
    }

    إذا (unlikely (!range_ریکارډ.serialize (c, عدد_ranges))) return_trace (خطأ);
    إذا (!عدد_ranges) return_trace (ریښتیا);

    بینښه شمېر = 0;
    بینښه range = (بینښه) -1;
    وروستی = (hb_codepoint_t) -2;
    بینښه unsorted = خطأ;
    لكل (خپلکاری g: glyphs)
    {
      إذا (وروستی + 1 != g)
      {
	إذا (unlikely (وروستی != (hb_codepoint_t) -2 && وروستی + 1 > g))
	  unsorted = ریښتیا;

        range++;
        range_ریکارډ.arrayZ[range].لومړی = g;
        range_ریکارډ.arrayZ[range].value = شمېر;
      }
      range_ریکارډ.arrayZ[range].وروستی = g;
      وروستی = g;
      شمېر++;
    }

    إذا (unlikely (unsorted))
      range_ریکارډ.as_array ().qsort (Coverage_Range_ریکارډ::cmp_range);

    return_trace (ریښتیا);
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    إذا (range_ریکارډ.len > glyphs->get_population () * hb_بټ_storage ((بینښه) range_ریکارډ.len))
    {
      لكل (خپلکاری g : *glyphs)
        إذا (get_coverage (g) != NOT_COVERED)
	  أرجع ریښتیا;
      أرجع خطأ;
    }

    أرجع hb_any (+ hb_iter (range_ریکارډ)
                   | hb_map ([glyphs] (ثابت Coverage_Range_ریکارډ &range) { أرجع range.intersects (*glyphs); }));
  }
  منطقي intersects_coverage (ثابت hb_سېټ_t *glyphs, بینښه صحيح شاخص_2) ثابت
  {
    خپلکاری *range = range_ریکارډ.as_array ().bsearch (شاخص_2);
    إذا (range)
      أرجع range->intersects (*glyphs);
    أرجع خطأ;
  }

  ساكن فراغ جمع_کړه_intersected_بټونه (hb_سېټ_t &سېټ, hb_codepoint_t بنسټ, uint64_t بټونه)
  { سېټ.جمع_کړه_بټونه (بنسټ, بټونه); }

  کينډۍ <ډول_نوم Iterable_بهر>
  ساكن فراغ جمع_کړه_intersected_بټونه (Iterable_بهر&& بهر, hb_codepoint_t بنسټ, uint64_t بټونه)
  {
    طالما (بټونه)
    {
      بهر << بنسټ + hb_ctz (بټونه);
      بټونه &= بټونه - 1;
    }
  }

  کينډۍ <ډول_نوم Iterable_بهر,
	    hb_requires (hb_is_sink_د (Iterable_بهر, hb_codepoint_t))>
  فراغ intersect_سېټ (ثابت hb_سېټ_t &glyphs, Iterable_بهر&& intersect_glyphs) ثابت
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t وروستی = 0;
    لكل (ثابت خپلکاری& range : range_ریکارډ)
    {
      إذا (unlikely (range.لومړی < وروستی))
        اكسر;
      وروستی = range.وروستی;
      hb_codepoint_t cursor = range.لومړی & ~hb_بټ_پاڼه_t::ELT_ماسک;
      cursor = cursor ? cursor - 1 : HB_سېټ_ارزښت_INVALID;
      uint64_t بټونه;
      طالما (glyphs.بل_بټونه (&cursor, &بټونه) && cursor <= وروستی)
      {
	إذا (range.لومړی > cursor)
	  بټونه &= UINT64_MAX << (range.لومړی - cursor);
	بینښه صحيح پای_ټکی = وروستی - cursor;
	إذا (پای_ټکی < hb_بټ_پاڼه_t::ELT_بټونه - 1)
	  بټونه &= (uint64_t (1) << (پای_ټکی + 1)) - 1;
	جمع_کړه_intersected_بټونه (intersect_glyphs, cursor, بټونه);
      }
    }
  }

  بینښه لګښت () ثابت { أرجع hb_بټ_storage ((بینښه) range_ریکارډ.len); /* bsearch cost */ }

  کينډۍ <ډول_نوم سېټ_t>
  منطقي collect_coverage (سېټ_t *glyphs) ثابت
  {
    لكل (ثابت خپلکاری& range: range_ریکارډ)
      إذا (unlikely (!range.collect_coverage (glyphs)))
        أرجع خطأ;
    أرجع ریښتیا;
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
      j = c->range_ریکارډ.len ? c->range_ریکارډ[0].لومړی : 0;
      إذا (unlikely (c->range_ریکارډ[0].لومړی > c->range_ریکارډ[0].وروستی))
      {
        /* Broken table. Skip. */
        i = c->range_ریکارډ.len;
        j = 0;
      }
    }
    منطقي __more__ () ثابت { أرجع i < c->range_ریکارډ.len; }
    فراغ __next__ ()
    {
      إذا (j >= c->range_ریکارډ[i].وروستی)
      {
        i++;
        إذا (__more__ ())
        {
          بینښه صحيح old = coverage;
          j = c->range_ریکارډ.arrayZ[i].لومړی;
          coverage = c->range_ریکارډ.arrayZ[i].value;
          إذا (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->range_ریکارډ.len;
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
    منطقي چلوونکی != (ثابت iter_t& o) ثابت
    { أرجع i != o.i || j != o.j; }
    iter_t __end__ () ثابت
    {
      iter_t it;
      it.init (*c);
      it.i = c->range_ریکارډ.len;
      it.j = 0;
      أرجع it;
    }

    خاص:
    ثابت بنية CoverageFormat2_4 *c;
    بینښه صحيح i, coverage;
    hb_codepoint_t j;
  };
  خاص:
};

}
}
}

#د_که_پای  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
