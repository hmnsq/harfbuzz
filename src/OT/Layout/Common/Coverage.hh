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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_COMMON_COVERAGE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_COMMON_COVERAGE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-ot-dual.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "CoverageFormat1.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "CoverageFormat2.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ Common {

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

ਬਣਤਰ Coverage
{

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT16 v; }        format;         /* Format identifier */
  CoverageFormat1_3<SmallTypes> format1;
  CoverageFormat2_4<SmallTypes> format2;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  CoverageFormat1_3<MediumTypes>format3;
  CoverageFormat2_4<MediumTypes>format4;
#ਜੇ_ਅੰਤ
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNION (2, format.v);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!u.format.v.sanitize (c)) return_trace (ਝੂਠ);
    hb_barrier ();
    ਚੋਣ (u.format.v)
    {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ਹਾਲਤ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ਹਾਲਤ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#ਜੇ_ਅੰਤ
    ਮੂਲ:return_trace (ਸੱਚ);
    }
  }

  /* Has interface. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੰਚਾਲਕ [] (hb_codepoint_t k) ਅਚਲ { ਵਾਪਸ get (k); }
  ਤਾਰਕਿਕ has (hb_codepoint_t k) ਅਚਲ { ਵਾਪਸ (*ਇਹ)[k] != NOT_COVERED; }
  /* Predicate. */
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ () (hb_codepoint_t k) ਅਚਲ { ਵਾਪਸ has (k); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get (hb_codepoint_t k) ਅਚਲ { ਵਾਪਸ get_coverage (k); }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_coverage (hb_codepoint_t glyph_id) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_coverage (glyph_id);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.get_coverage (glyph_id);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.get_coverage (glyph_id);
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.get_coverage (glyph_id);
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ NOT_COVERED;
    }
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ coverage;
    ਜੇ (cache && cache->get (glyph_id, &coverage)) ਵਾਪਸ coverage < cache->MAX_ਮੁੱਲ ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    ਜੇ (cache) {
      ਜੇ (coverage == NOT_COVERED)
	cache->ਸੈੱਟ (glyph_id, cache->MAX_ਮੁੱਲ);
      ਨਹੀਂਤਾਂ ਜੇ (likely (coverage < cache->MAX_ਮੁੱਲ))
	cache->ਸੈੱਟ (glyph_id, coverage);
    }
    ਵਾਪਸ coverage;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_coverage_ਦੁਆਧਾਰੀ (hb_codepoint_t glyph_id,
				    hb_ot_layout_ਦੁਆਧਾਰੀ_cache_t *cache) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ coverage;
    ਜੇ (cache && cache->get (glyph_id, &coverage)) ਵਾਪਸ coverage < cache->MAX_ਮੁੱਲ ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    ਜੇ (cache) {
      ਜੇ (coverage == NOT_COVERED)
	cache->ਸੈੱਟ (glyph_id, cache->MAX_ਮੁੱਲ);
      ਨਹੀਂਤਾਂ
	cache->ਸੈੱਟ (glyph_id, 0);
    }
    ਵਾਪਸ coverage;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_population () ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_population ();
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.get_population ();
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.get_population ();
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.get_population ();
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ NOT_COVERED;
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
      hb_requires (hb_is_sorted_ਸਰੋਤ_ਦਾ (Iterator, hb_codepoint_t))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = hb_len (+ glyphs);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ranges = 0;
    hb_codepoint_t ਆਖਰੀ = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    ਤਾਰਕਿਕ unsorted = ਝੂਠ;
    ਲਈ (ਸਵੈਚਾਲਿਤ g: + glyphs)
    {
      ਜੇ (ਆਖਰੀ != (hb_codepoint_t) -2 && g < ਆਖਰੀ)
	unsorted = ਸੱਚ;
      ਜੇ (ਆਖਰੀ + 1 != g)
	ਅੰਕ_ranges++;
      ਆਖਰੀ = g;
      ਜੇ (g > max) max = g;
    }
    u.format.v = !unsorted && ਗਿਣਤੀ_2 <= ਅੰਕ_ranges * 3 ? 1 : 2;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (max > 0xFFFFu)
      u.format.v += 2;
    ਜੇ (unlikely (max > 0xFFFFFFu))
#ਨਹੀਂਤਾਂ
    ਜੇ (unlikely (max > 0xFFFFu))
#ਜੇ_ਅੰਤ
    {
      c->check_success (ਝੂਠ, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ);
      return_trace (ਝੂਠ);
    }

    ਚੋਣ (u.format.v)
    {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    ਹਾਲਤ 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    ਹਾਲਤ 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#ਜੇ_ਅੰਤ
    ਮੂਲ:return_trace (ਝੂਠ);
    }
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_subset_plan_t *plan = c->plan;
    ਸਵੈਚਾਲਿਤ it =
    + iter ()
    | hb_take (plan->ਸਰੋਤ->get_ਅੰਕ_glyphs ())
    | hb_map_retains_ਛਾਂਟੀ ([plan] (hb_codepoint_t g) {
	ਵਾਪਸ plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { ਵਾਪਸ glyph != HB_MAP_ਮੁੱਲ_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (ਤਾਰਕਿਕ (glyphs));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਚੋਣ (u.format.v)
    {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.intersects (glyphs);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.intersects (glyphs);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.intersects (glyphs);
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.intersects (glyphs);
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ ਝੂਠ;
    }
  }
  ਤਾਰਕਿਕ intersects_coverage (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2) ਅਚਲ
  {
    ਚੋਣ (u.format.v)
    {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.intersects_coverage (glyphs, ਸੂਚਕਾਂਕ_2);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.intersects_coverage (glyphs, ਸੂਚਕਾਂਕ_2);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.intersects_coverage (glyphs, ਸੂਚਕਾਂਕ_2);
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.intersects_coverage (glyphs, ਸੂਚਕਾਂਕ_2);
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ ਝੂਠ;
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲਾਗਤ () ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.ਲਾਗਤ ();
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.ਲਾਗਤ ();
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.ਲਾਗਤ ();
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.ਲਾਗਤ ();
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਤਾਰਕਿਕ collect_coverage (ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਚੋਣ (u.format.v)
    {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.collect_coverage (glyphs);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.collect_coverage (glyphs);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.collect_coverage (glyphs);
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.collect_coverage (glyphs);
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ ਝੂਠ;
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable_ਬਾਹਰਲਾ,
	    hb_requires (hb_is_sink_ਦਾ (Iterable_ਬਾਹਰਲਾ, hb_codepoint_t))>
  ਖਾਲੀ intersect_ਸੈੱਟ (ਅਚਲ hb_ਸੈੱਟ_t &glyphs, Iterable_ਬਾਹਰਲਾ&& intersect_glyphs) ਅਚਲ
  {
    ਚੋਣ (u.format.v)
    {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.intersect_ਸੈੱਟ (glyphs, intersect_glyphs);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.intersect_ਸੈੱਟ (glyphs, intersect_glyphs);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.intersect_ਸੈੱਟ (glyphs, intersect_glyphs);
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.intersect_ਸੈੱਟ (glyphs, intersect_glyphs);
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ ;
    }
  }

  ਬਣਤਰ iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_sorted_iterator = ਸੱਚ;
    iter_t (ਅਚਲ Coverage &c_ = Null (Coverage))
    {
      hb_memset (ਇਹ, 0, ਆਕਾਰ (*ਇਹ));
      format = c_.u.format.v;
      ਚੋਣ (format)
      {
      ਹਾਲਤ 1: hb_barrier (); u.format1.init (c_.u.format1); ਵਾਪਸ;
      ਹਾਲਤ 2: hb_barrier (); u.format2.init (c_.u.format2); ਵਾਪਸ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
      ਹਾਲਤ 3: hb_barrier (); u.format3.init (c_.u.format3); ਵਾਪਸ;
      ਹਾਲਤ 4: hb_barrier (); u.format4.init (c_.u.format4); ਵਾਪਸ;
#ਜੇ_ਅੰਤ
      ਮੂਲ:                               ਵਾਪਸ;
      }
    }
    ਤਾਰਕਿਕ __more__ () ਅਚਲ
    {
      ਚੋਣ (format)
      {
      ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.__more__ ();
      ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.__more__ ();
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
      ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.__more__ ();
      ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.__more__ ();
#ਜੇ_ਅੰਤ
      ਮੂਲ:ਵਾਪਸ ਝੂਠ;
      }
    }
    ਖਾਲੀ __next__ ()
    {
      ਚੋਣ (format)
      {
      ਹਾਲਤ 1: hb_barrier (); u.format1.__next__ (); ਰੋਕੋ;
      ਹਾਲਤ 2: hb_barrier (); u.format2.__next__ (); ਰੋਕੋ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
      ਹਾਲਤ 3: hb_barrier (); u.format3.__next__ (); ਰੋਕੋ;
      ਹਾਲਤ 4: hb_barrier (); u.format4.__next__ (); ਰੋਕੋ;
#ਜੇ_ਅੰਤ
      ਮੂਲ:                   ਰੋਕੋ;
      }
    }
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_codepoint_t __item_t__;
    __item_t__ __item__ () ਅਚਲ { ਵਾਪਸ get_glyph (); }

    hb_codepoint_t get_glyph () ਅਚਲ
    {
      ਚੋਣ (format)
      {
      ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_glyph ();
      ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.get_glyph ();
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
      ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.get_glyph ();
      ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.get_glyph ();
#ਜੇ_ਅੰਤ
      ਮੂਲ:ਵਾਪਸ 0;
      }
    }
    ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ iter_t& o) ਅਚਲ
    {
      ਜੇ (unlikely (format != o.format)) ਵਾਪਸ ਸੱਚ;
      ਚੋਣ (format)
      {
      ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1 != o.u.format1;
      ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2 != o.u.format2;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
      ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3 != o.u.format3;
      ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4 != o.u.format4;
#ਜੇ_ਅੰਤ
      ਮੂਲ:ਵਾਪਸ ਝੂਠ;
      }
    }
    iter_t __end__ () ਅਚਲ
    {
      iter_t it;
      it.format = format;
      ਚੋਣ (format)
      {
      ਹਾਲਤ 1: hb_barrier (); it.u.format1 = u.format1.__end__ (); ਰੋਕੋ;
      ਹਾਲਤ 2: hb_barrier (); it.u.format2 = u.format2.__end__ (); ਰੋਕੋ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
      ਹਾਲਤ 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); ਰੋਕੋ;
      ਹਾਲਤ 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); ਰੋਕੋ;
#ਜੇ_ਅੰਤ
      ਮੂਲ: ਰੋਕੋ;
      }
      ਵਾਪਸ it;
    }

    ਨਿਜੀ:
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ format;
    ਸੰਘ {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#ਜੇ_ਅੰਤ
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () ਅਚਲ { ਵਾਪਸ iter_t (*ਇਹ); }
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->ਸ਼ੁਰੂ_embed<Coverage> ()->serialize (c, it); }

}
}
}

#ਜੇ_ਅੰਤ  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH
