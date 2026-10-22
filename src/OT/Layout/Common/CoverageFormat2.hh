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

#اگر_غیر_تعریف_شدہ OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
#تعریف_کرو OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH

#شامل "RangeRecord.hh"

نامخلا OT {
نامخلا Layout {
نامخلا Common {

سانچہ <قسم_نام Types>
ساخت CoverageFormat2_4
{
  دوست ساخت Coverage;
  استعمال Coverage_Range_ریکارڈ = Range_ریکارڈ<Types, قسم_نام Types::HBUINT>;

  عوامی:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  قسم_نام Types::سانچہ Sorted_Array_کا<Coverage_Range_ریکارڈ>
                range_ریکارڈ;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  عوامی:
  DEFINE_سائز_ARRAY (2 + Types::HBUINT::static_سائز, range_ریکارڈ);

  نجی:

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (range_ریکارڈ.sanitize (c));
  }

  غیرعلامتی صحیح get_coverage (hb_codepoint_t glyph_id) ثابت
  {
    ثابت Coverage_Range_ریکارڈ &range = range_ریکارڈ.bsearch (glyph_id);
    واپس likely (range.پہلا <= range.آخری)
         ? (غیرعلامتی صحیح) range.value + (glyph_id - range.پہلا)
         : NOT_COVERED;
  }

  غیرعلامتی get_population () ثابت
  {
    قسم_نام Types::large_int ret = 0;
    برائے (ثابت خودکار &r : range_ریکارڈ)
      ret += r.get_population ();
    واپس ret > UINT_MAX ? UINT_MAX : (غیرعلامتی) ret;
  }

  سانچہ <قسم_نام Iterator,
      hb_requires (hb_is_sorted_ماخذ_کا (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);

    غیرعلامتی عدد_ranges = 0;
    hb_codepoint_t آخری = (hb_codepoint_t) -2;
    برائے (خودکار g: glyphs)
    {
      اگر (آخری + 1 != g)
        عدد_ranges++;
      آخری = g;
    }

    اگر (unlikely (!range_ریکارڈ.serialize (c, عدد_ranges))) return_trace (جھوٹ);
    اگر (!عدد_ranges) return_trace (سچ);

    غیرعلامتی گنتی = 0;
    غیرعلامتی range = (غیرعلامتی) -1;
    آخری = (hb_codepoint_t) -2;
    غیرعلامتی unsorted = جھوٹ;
    برائے (خودکار g: glyphs)
    {
      اگر (آخری + 1 != g)
      {
	اگر (unlikely (آخری != (hb_codepoint_t) -2 && آخری + 1 > g))
	  unsorted = سچ;

        range++;
        range_ریکارڈ.arrayZ[range].پہلا = g;
        range_ریکارڈ.arrayZ[range].value = گنتی;
      }
      range_ریکارڈ.arrayZ[range].آخری = g;
      آخری = g;
      گنتی++;
    }

    اگر (unlikely (unsorted))
      range_ریکارڈ.as_array ().qsort (Coverage_Range_ریکارڈ::cmp_range);

    return_trace (سچ);
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    اگر (range_ریکارڈ.len > glyphs->get_population () * hb_ثنائی_ہندسہ_storage ((غیرعلامتی) range_ریکارڈ.len))
    {
      برائے (خودکار g : *glyphs)
        اگر (get_coverage (g) != NOT_COVERED)
	  واپس سچ;
      واپس جھوٹ;
    }

    واپس hb_any (+ hb_iter (range_ریکارڈ)
                   | hb_map ([glyphs] (ثابت Coverage_Range_ریکارڈ &range) { واپس range.intersects (*glyphs); }));
  }
  منطقی intersects_coverage (ثابت hb_سیٹ_t *glyphs, غیرعلامتی صحیح اشاریہ_2) ثابت
  {
    خودکار *range = range_ریکارڈ.as_array ().bsearch (اشاریہ_2);
    اگر (range)
      واپس range->intersects (*glyphs);
    واپس جھوٹ;
  }

  ساکن خالی جوڑو_intersected_بٹ (hb_سیٹ_t &سیٹ, hb_codepoint_t بنیاد, uint64_t بٹ)
  { سیٹ.جوڑو_بٹ (بنیاد, بٹ); }

  سانچہ <قسم_نام Iterable_برآمد>
  ساکن خالی جوڑو_intersected_بٹ (Iterable_برآمد&& برآمد, hb_codepoint_t بنیاد, uint64_t بٹ)
  {
    جب (بٹ)
    {
      برآمد << بنیاد + hb_ctz (بٹ);
      بٹ &= بٹ - 1;
    }
  }

  سانچہ <قسم_نام Iterable_برآمد,
	    hb_requires (hb_is_sink_کا (Iterable_برآمد, hb_codepoint_t))>
  خالی intersect_سیٹ (ثابت hb_سیٹ_t &glyphs, Iterable_برآمد&& intersect_glyphs) ثابت
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t آخری = 0;
    برائے (ثابت خودکار& range : range_ریکارڈ)
    {
      اگر (unlikely (range.پہلا < آخری))
        توڑو;
      آخری = range.آخری;
      hb_codepoint_t cursor = range.پہلا & ~hb_ثنائی_ہندسہ_صفحہ_t::ELT_نقاب;
      cursor = cursor ? cursor - 1 : HB_سیٹ_قدر_INVALID;
      uint64_t بٹ;
      جب (glyphs.اگلا_بٹ (&cursor, &بٹ) && cursor <= آخری)
      {
	اگر (range.پہلا > cursor)
	  بٹ &= UINT64_MAX << (range.پہلا - cursor);
	غیرعلامتی صحیح انتہا = آخری - cursor;
	اگر (انتہا < hb_ثنائی_ہندسہ_صفحہ_t::ELT_بٹ - 1)
	  بٹ &= (uint64_t (1) << (انتہا + 1)) - 1;
	جوڑو_intersected_بٹ (intersect_glyphs, cursor, بٹ);
      }
    }
  }

  غیرعلامتی لاگت () ثابت { واپس hb_ثنائی_ہندسہ_storage ((غیرعلامتی) range_ریکارڈ.len); /* bsearch cost */ }

  سانچہ <قسم_نام سیٹ_t>
  منطقی collect_coverage (سیٹ_t *glyphs) ثابت
  {
    برائے (ثابت خودکار& range: range_ریکارڈ)
      اگر (unlikely (!range.collect_coverage (glyphs)))
        واپس جھوٹ;
    واپس سچ;
  }

  عوامی:
  /* Older compilers need this to be public. */
  ساخت iter_t
  {
    خالی init (ثابت CoverageFormat2_4 &c_)
    {
      c = &c_;
      coverage = 0;
      i = 0;
      j = c->range_ریکارڈ.len ? c->range_ریکارڈ[0].پہلا : 0;
      اگر (unlikely (c->range_ریکارڈ[0].پہلا > c->range_ریکارڈ[0].آخری))
      {
        /* Broken table. Skip. */
        i = c->range_ریکارڈ.len;
        j = 0;
      }
    }
    منطقی __more__ () ثابت { واپس i < c->range_ریکارڈ.len; }
    خالی __next__ ()
    {
      اگر (j >= c->range_ریکارڈ[i].آخری)
      {
        i++;
        اگر (__more__ ())
        {
          غیرعلامتی صحیح old = coverage;
          j = c->range_ریکارڈ.arrayZ[i].پہلا;
          coverage = c->range_ریکارڈ.arrayZ[i].value;
          اگر (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->range_ریکارڈ.len;
           j = 0;
           واپس;
          }
        }
        ورنہ
          j = 0;
        واپس;
      }
      coverage++;
      j++;
    }
    hb_codepoint_t get_glyph () ثابت { واپس j; }
    منطقی عامل != (ثابت iter_t& o) ثابت
    { واپس i != o.i || j != o.j; }
    iter_t __end__ () ثابت
    {
      iter_t it;
      it.init (*c);
      it.i = c->range_ریکارڈ.len;
      it.j = 0;
      واپس it;
    }

    نجی:
    ثابت ساخت CoverageFormat2_4 *c;
    غیرعلامتی صحیح i, coverage;
    hb_codepoint_t j;
  };
  نجی:
};

}
}
}

#اگر_ختم  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
