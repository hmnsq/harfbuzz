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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "RangeRecord.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ Common {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ CoverageFormat2_4
{
  ਮਿੱਤਰ ਬਣਤਰ Coverage;
  ਵਰਤੋਂ Coverage_Range_ਰਿਕਾਰਡ = Range_ਰਿਕਾਰਡ<Types, ਕਿਸਮ_ਨਾਂ Types::HBUINT>;

  ਜਨਤਕ:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Sorted_Array_ਦਾ<Coverage_Range_ਰਿਕਾਰਡ>
                range_ਰਿਕਾਰਡ;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2 + Types::HBUINT::static_ਮਾਪ, range_ਰਿਕਾਰਡ);

  ਨਿਜੀ:

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (range_ਰਿਕਾਰਡ.sanitize (c));
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_coverage (hb_codepoint_t glyph_id) ਅਚਲ
  {
    ਅਚਲ Coverage_Range_ਰਿਕਾਰਡ &range = range_ਰਿਕਾਰਡ.bsearch (glyph_id);
    ਵਾਪਸ likely (range.ਪਹਿਲਾ <= range.ਆਖਰੀ)
         ? (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) range.value + (glyph_id - range.ਪਹਿਲਾ)
         : NOT_COVERED;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_population () ਅਚਲ
  {
    ਕਿਸਮ_ਨਾਂ Types::large_int ret = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &r : range_ਰਿਕਾਰਡ)
      ret += r.get_population ();
    ਵਾਪਸ ret > UINT_MAX ? UINT_MAX : (ਬਿਨਾਨਿਸ਼ਾਨੀ) ret;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
      hb_requires (hb_is_sorted_ਸਰੋਤ_ਦਾ (Iterator, hb_codepoint_t))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ranges = 0;
    hb_codepoint_t ਆਖਰੀ = (hb_codepoint_t) -2;
    ਲਈ (ਸਵੈਚਾਲਿਤ g: glyphs)
    {
      ਜੇ (ਆਖਰੀ + 1 != g)
        ਅੰਕ_ranges++;
      ਆਖਰੀ = g;
    }

    ਜੇ (unlikely (!range_ਰਿਕਾਰਡ.serialize (c, ਅੰਕ_ranges))) return_trace (ਝੂਠ);
    ਜੇ (!ਅੰਕ_ranges) return_trace (ਸੱਚ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ range = (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1;
    ਆਖਰੀ = (hb_codepoint_t) -2;
    ਬਿਨਾਨਿਸ਼ਾਨੀ unsorted = ਝੂਠ;
    ਲਈ (ਸਵੈਚਾਲਿਤ g: glyphs)
    {
      ਜੇ (ਆਖਰੀ + 1 != g)
      {
	ਜੇ (unlikely (ਆਖਰੀ != (hb_codepoint_t) -2 && ਆਖਰੀ + 1 > g))
	  unsorted = ਸੱਚ;

        range++;
        range_ਰਿਕਾਰਡ.arrayZ[range].ਪਹਿਲਾ = g;
        range_ਰਿਕਾਰਡ.arrayZ[range].value = ਗਿਣਤੀ_2;
      }
      range_ਰਿਕਾਰਡ.arrayZ[range].ਆਖਰੀ = g;
      ਆਖਰੀ = g;
      ਗਿਣਤੀ_2++;
    }

    ਜੇ (unlikely (unsorted))
      range_ਰਿਕਾਰਡ.as_array ().qsort (Coverage_Range_ਰਿਕਾਰਡ::cmp_range);

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਜੇ (range_ਰਿਕਾਰਡ.len > glyphs->get_population () * hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage ((ਬਿਨਾਨਿਸ਼ਾਨੀ) range_ਰਿਕਾਰਡ.len))
    {
      ਲਈ (ਸਵੈਚਾਲਿਤ g : *glyphs)
        ਜੇ (get_coverage (g) != NOT_COVERED)
	  ਵਾਪਸ ਸੱਚ;
      ਵਾਪਸ ਝੂਠ;
    }

    ਵਾਪਸ hb_any (+ hb_iter (range_ਰਿਕਾਰਡ)
                   | hb_map ([glyphs] (ਅਚਲ Coverage_Range_ਰਿਕਾਰਡ &range) { ਵਾਪਸ range.intersects (*glyphs); }));
  }
  ਤਾਰਕਿਕ intersects_coverage (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2) ਅਚਲ
  {
    ਸਵੈਚਾਲਿਤ *range = range_ਰਿਕਾਰਡ.as_array ().bsearch (ਸੂਚਕਾਂਕ_2);
    ਜੇ (range)
      ਵਾਪਸ range->intersects (*glyphs);
    ਵਾਪਸ ਝੂਠ;
  }

  ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_intersected_ਬਿੱਟ (hb_ਸੈੱਟ_t &ਸੈੱਟ, hb_codepoint_t ਅਧਾਰ, uint64_t ਬਿੱਟ)
  { ਸੈੱਟ.ਜੋੜੋ_ਬਿੱਟ (ਅਧਾਰ, ਬਿੱਟ); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable_ਬਾਹਰਲਾ>
  ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_intersected_ਬਿੱਟ (Iterable_ਬਾਹਰਲਾ&& ਬਾਹਰਲਾ, hb_codepoint_t ਅਧਾਰ, uint64_t ਬਿੱਟ)
  {
    ਜਦੋਂ (ਬਿੱਟ)
    {
      ਬਾਹਰਲਾ << ਅਧਾਰ + hb_ctz (ਬਿੱਟ);
      ਬਿੱਟ &= ਬਿੱਟ - 1;
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable_ਬਾਹਰਲਾ,
	    hb_requires (hb_is_sink_ਦਾ (Iterable_ਬਾਹਰਲਾ, hb_codepoint_t))>
  ਖਾਲੀ intersect_ਸੈੱਟ (ਅਚਲ hb_ਸੈੱਟ_t &glyphs, Iterable_ਬਾਹਰਲਾ&& intersect_glyphs) ਅਚਲ
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t ਆਖਰੀ = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& range : range_ਰਿਕਾਰਡ)
    {
      ਜੇ (unlikely (range.ਪਹਿਲਾ < ਆਖਰੀ))
        ਰੋਕੋ;
      ਆਖਰੀ = range.ਆਖਰੀ;
      hb_codepoint_t cursor = range.ਪਹਿਲਾ & ~hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ELT_ਨਕਾਬ;
      cursor = cursor ? cursor - 1 : HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
      uint64_t ਬਿੱਟ;
      ਜਦੋਂ (glyphs.ਅਗਲਾ_ਬਿੱਟ (&cursor, &ਬਿੱਟ) && cursor <= ਆਖਰੀ)
      {
	ਜੇ (range.ਪਹਿਲਾ > cursor)
	  ਬਿੱਟ &= UINT64_MAX << (range.ਪਹਿਲਾ - cursor);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = ਆਖਰੀ - cursor;
	ਜੇ (ਸਿਰਾ < hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ELT_ਬਿੱਟ - 1)
	  ਬਿੱਟ &= (uint64_t (1) << (ਸਿਰਾ + 1)) - 1;
	ਜੋੜੋ_intersected_ਬਿੱਟ (intersect_glyphs, cursor, ਬਿੱਟ);
      }
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲਾਗਤ () ਅਚਲ { ਵਾਪਸ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage ((ਬਿਨਾਨਿਸ਼ਾਨੀ) range_ਰਿਕਾਰਡ.len); /* bsearch cost */ }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਤਾਰਕਿਕ collect_coverage (ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& range: range_ਰਿਕਾਰਡ)
      ਜੇ (unlikely (!range.collect_coverage (glyphs)))
        ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
  }

  ਜਨਤਕ:
  /* Older compilers need this to be public. */
  ਬਣਤਰ iter_t
  {
    ਖਾਲੀ init (ਅਚਲ CoverageFormat2_4 &c_)
    {
      c = &c_;
      coverage = 0;
      i = 0;
      j = c->range_ਰਿਕਾਰਡ.len ? c->range_ਰਿਕਾਰਡ[0].ਪਹਿਲਾ : 0;
      ਜੇ (unlikely (c->range_ਰਿਕਾਰਡ[0].ਪਹਿਲਾ > c->range_ਰਿਕਾਰਡ[0].ਆਖਰੀ))
      {
        /* Broken table. Skip. */
        i = c->range_ਰਿਕਾਰਡ.len;
        j = 0;
      }
    }
    ਤਾਰਕਿਕ __more__ () ਅਚਲ { ਵਾਪਸ i < c->range_ਰਿਕਾਰਡ.len; }
    ਖਾਲੀ __next__ ()
    {
      ਜੇ (j >= c->range_ਰਿਕਾਰਡ[i].ਆਖਰੀ)
      {
        i++;
        ਜੇ (__more__ ())
        {
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ old = coverage;
          j = c->range_ਰਿਕਾਰਡ.arrayZ[i].ਪਹਿਲਾ;
          coverage = c->range_ਰਿਕਾਰਡ.arrayZ[i].value;
          ਜੇ (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->range_ਰਿਕਾਰਡ.len;
           j = 0;
           ਵਾਪਸ;
          }
        }
        ਨਹੀਂਤਾਂ
          j = 0;
        ਵਾਪਸ;
      }
      coverage++;
      j++;
    }
    hb_codepoint_t get_glyph () ਅਚਲ { ਵਾਪਸ j; }
    ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ iter_t& o) ਅਚਲ
    { ਵਾਪਸ i != o.i || j != o.j; }
    iter_t __end__ () ਅਚਲ
    {
      iter_t it;
      it.init (*c);
      it.i = c->range_ਰਿਕਾਰਡ.len;
      it.j = 0;
      ਵਾਪਸ it;
    }

    ਨਿਜੀ:
    ਅਚਲ ਬਣਤਰ CoverageFormat2_4 *c;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i, coverage;
    hb_codepoint_t j;
  };
  ਨਿਜੀ:
};

}
}
}

#ਜੇ_ਅੰਤ  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
