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

#اگر_تعریف_نشده OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
#تعریف_کن OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH

#ضمن "RangeRecord.hh"

نطاق OT {
نطاق Layout {
نطاق Common {

قالب <نام_نوع Types>
بنية CoverageFormat2_4
{
  دوست بنية Coverage;
  استفاده Coverage_Range_رکورد = Range_رکورد<Types, نام_نوع Types::HBUINT>;

  عام:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  نام_نوع Types::قالب Sorted_Array_از<Coverage_Range_رکورد>
                range_رکورد;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::HBUINT::static_اندازه, range_رکورد);

  خاص:

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (range_رکورد.sanitize (c));
  }

  بیعلامت صحيح get_coverage (hb_codepoint_t glyph_id) ثابت
  {
    ثابت Coverage_Range_رکورد &range = range_رکورد.bsearch (glyph_id);
    أرجع likely (range.اولی <= range.آخرین)
         ? (بیعلامت صحيح) range.value + (glyph_id - range.اولی)
         : NOT_COVERED;
  }

  بیعلامت get_population () ثابت
  {
    نام_نوع Types::large_int ret = 0;
    لكل (ثابت خودکار &r : range_رکورد)
      ret += r.get_population ();
    أرجع ret > UINT_MAX ? UINT_MAX : (بیعلامت) ret;
  }

  قالب <نام_نوع Iterator,
      hb_requires (hb_is_sorted_منبع_از (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    بیعلامت رقم_ranges = 0;
    hb_codepoint_t آخرین = (hb_codepoint_t) -2;
    لكل (خودکار g: glyphs)
    {
      إذا (آخرین + 1 != g)
        رقم_ranges++;
      آخرین = g;
    }

    إذا (unlikely (!range_رکورد.serialize (c, رقم_ranges))) return_trace (خطأ);
    إذا (!رقم_ranges) return_trace (درست);

    بیعلامت شمار = 0;
    بیعلامت range = (بیعلامت) -1;
    آخرین = (hb_codepoint_t) -2;
    بیعلامت unsorted = خطأ;
    لكل (خودکار g: glyphs)
    {
      إذا (آخرین + 1 != g)
      {
	إذا (unlikely (آخرین != (hb_codepoint_t) -2 && آخرین + 1 > g))
	  unsorted = درست;

        range++;
        range_رکورد.arrayZ[range].اولی = g;
        range_رکورد.arrayZ[range].value = شمار;
      }
      range_رکورد.arrayZ[range].آخرین = g;
      آخرین = g;
      شمار++;
    }

    إذا (unlikely (unsorted))
      range_رکورد.as_array ().qsort (Coverage_Range_رکورد::cmp_range);

    return_trace (درست);
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    إذا (range_رکورد.len > glyphs->get_population () * hb_بیت_storage ((بیعلامت) range_رکورد.len))
    {
      لكل (خودکار g : *glyphs)
        إذا (get_coverage (g) != NOT_COVERED)
	  أرجع درست;
      أرجع خطأ;
    }

    أرجع hb_any (+ hb_iter (range_رکورد)
                   | hb_map ([glyphs] (ثابت Coverage_Range_رکورد &range) { أرجع range.intersects (*glyphs); }));
  }
  منطقي intersects_coverage (ثابت hb_مجموعه_t *glyphs, بیعلامت صحيح شاخص_2) ثابت
  {
    خودکار *range = range_رکورد.as_array ().bsearch (شاخص_2);
    إذا (range)
      أرجع range->intersects (*glyphs);
    أرجع خطأ;
  }

  ساكن فراغ جمع_کن_intersected_بیتها (hb_مجموعه_t &مجموعه, hb_codepoint_t پایه, uint64_t بیتها)
  { مجموعه.جمع_کن_بیتها (پایه, بیتها); }

  قالب <نام_نوع Iterable_بیرون>
  ساكن فراغ جمع_کن_intersected_بیتها (Iterable_بیرون&& بیرون, hb_codepoint_t پایه, uint64_t بیتها)
  {
    طالما (بیتها)
    {
      بیرون << پایه + hb_ctz (بیتها);
      بیتها &= بیتها - 1;
    }
  }

  قالب <نام_نوع Iterable_بیرون,
	    hb_requires (hb_is_sink_از (Iterable_بیرون, hb_codepoint_t))>
  فراغ intersect_مجموعه (ثابت hb_مجموعه_t &glyphs, Iterable_بیرون&& intersect_glyphs) ثابت
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t آخرین = 0;
    لكل (ثابت خودکار& range : range_رکورد)
    {
      إذا (unlikely (range.اولی < آخرین))
        اكسر;
      آخرین = range.آخرین;
      hb_codepoint_t cursor = range.اولی & ~hb_بیت_صفحه_t::ELT_ماسک;
      cursor = cursor ? cursor - 1 : HB_مجموعه_مقدار_INVALID;
      uint64_t بیتها;
      طالما (glyphs.بعدی_بیتها (&cursor, &بیتها) && cursor <= آخرین)
      {
	إذا (range.اولی > cursor)
	  بیتها &= UINT64_MAX << (range.اولی - cursor);
	بیعلامت صحيح انتها = آخرین - cursor;
	إذا (انتها < hb_بیت_صفحه_t::ELT_بیتها - 1)
	  بیتها &= (uint64_t (1) << (انتها + 1)) - 1;
	جمع_کن_intersected_بیتها (intersect_glyphs, cursor, بیتها);
      }
    }
  }

  بیعلامت هزینه () ثابت { أرجع hb_بیت_storage ((بیعلامت) range_رکورد.len); /* bsearch cost */ }

  قالب <نام_نوع مجموعه_t>
  منطقي collect_coverage (مجموعه_t *glyphs) ثابت
  {
    لكل (ثابت خودکار& range: range_رکورد)
      إذا (unlikely (!range.collect_coverage (glyphs)))
        أرجع خطأ;
    أرجع درست;
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
      j = c->range_رکورد.len ? c->range_رکورد[0].اولی : 0;
      إذا (unlikely (c->range_رکورد[0].اولی > c->range_رکورد[0].آخرین))
      {
        /* Broken table. Skip. */
        i = c->range_رکورد.len;
        j = 0;
      }
    }
    منطقي __more__ () ثابت { أرجع i < c->range_رکورد.len; }
    فراغ __next__ ()
    {
      إذا (j >= c->range_رکورد[i].آخرین)
      {
        i++;
        إذا (__more__ ())
        {
          بیعلامت صحيح old = coverage;
          j = c->range_رکورد.arrayZ[i].اولی;
          coverage = c->range_رکورد.arrayZ[i].value;
          إذا (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->range_رکورد.len;
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
    منطقي عملگر != (ثابت iter_t& o) ثابت
    { أرجع i != o.i || j != o.j; }
    iter_t __end__ () ثابت
    {
      iter_t it;
      it.init (*c);
      it.i = c->range_رکورد.len;
      it.j = 0;
      أرجع it;
    }

    خاص:
    ثابت بنية CoverageFormat2_4 *c;
    بیعلامت صحيح i, coverage;
    hb_codepoint_t j;
  };
  خاص:
};

}
}
}

#پایان_اگر  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
