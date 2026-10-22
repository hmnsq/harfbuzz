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

#ifndef OT_LAYOUT_COMMON_COVERAGE_HH
#define OT_LAYOUT_COMMON_COVERAGE_HH

#அந்தர்பாவ "../../../hb-ot-dual.hh"
#அந்தர்பாவ "CoverageFormat1.hh"
#அந்தர்பாவ "CoverageFormat2.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ Common {

template<typename Iterator>
நிலையான உள்வரி வெறுமை Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

கட்டமைப்பு Coverage
{

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT16 v; }        format;         /* Format identifier */
  CoverageFormat1_3<SmallTypes> format1;
  CoverageFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  CoverageFormat1_3<MediumTypes>format3;
  CoverageFormat2_4<MediumTypes>format4;
#endif
  } u;
  பொது:
  DEFINE_SIZE_UNION (2, format.v);

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!u.format.v.sanitize (c)) return_trace (பொய்);
    hb_barrier ();
    தேர்வு (u.format.v)
    {
    நிலை 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    நிலை 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    நிலை 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    இயல்பு:return_trace (உண்மை);
    }
  }

  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) மாறிலி { திரும்பு get (k); }
  தருக்க has (hb_codepoint_t k) மாறிலி { திரும்பு (*இது)[k] != NOT_COVERED; }
  /* Predicate. */
  தருக்க operator () (hb_codepoint_t k) மாறிலி { திரும்பு has (k); }

  unsigned முழு get (hb_codepoint_t k) மாறிலி { திரும்பு get_coverage (k); }
  unsigned முழு get_coverage (hb_codepoint_t glyph_id) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_coverage (glyph_id);
    நிலை 2: hb_barrier (); திரும்பு u.format2.get_coverage (glyph_id);
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); திரும்பு u.format3.get_coverage (glyph_id);
    நிலை 4: hb_barrier (); திரும்பு u.format4.get_coverage (glyph_id);
#endif
    இயல்பு:திரும்பு NOT_COVERED;
    }
  }
  unsigned முழு get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) மாறிலி
  {
    unsigned coverage;
    எனில் (cache && cache->get (glyph_id, &coverage)) திரும்பு coverage < cache->MAX_மதிப்பு ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    எனில் (cache) {
      எனில் (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_மதிப்பு);
      இல்லையெனில் எனில் (likely (coverage < cache->MAX_மதிப்பு))
	cache->set (glyph_id, coverage);
    }
    திரும்பு coverage;
  }

  unsigned முழு get_coverage_இருமம் (hb_codepoint_t glyph_id,
				    hb_ot_layout_இருமம்_cache_t *cache) மாறிலி
  {
    unsigned coverage;
    எனில் (cache && cache->get (glyph_id, &coverage)) திரும்பு coverage < cache->MAX_மதிப்பு ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    எனில் (cache) {
      எனில் (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_மதிப்பு);
      இல்லையெனில்
	cache->set (glyph_id, 0);
    }
    திரும்பு coverage;
  }

  unsigned get_population () மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_population ();
    நிலை 2: hb_barrier (); திரும்பு u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); திரும்பு u.format3.get_population ();
    நிலை 4: hb_barrier (); திரும்பு u.format4.get_population ();
#endif
    இயல்பு:திரும்பு NOT_COVERED;
    }
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  தருக்க serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);

    unsigned மொத்தம் = hb_len (+ glyphs);
    unsigned இலக்கம்_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    தருக்க unsorted = பொய்;
    க்கு (auto g: + glyphs)
    {
      எனில் (last != (hb_codepoint_t) -2 && g < last)
	unsorted = உண்மை;
      எனில் (last + 1 != g)
	இலக்கம்_ranges++;
      last = g;
      எனில் (g > max) max = g;
    }
    u.format.v = !unsorted && மொத்தம் <= இலக்கம்_ranges * 3 ? 1 : 2;

#ifndef HB_NO_BEYOND_64K
    எனில் (max > 0xFFFFu)
      u.format.v += 2;
    எனில் (unlikely (max > 0xFFFFFFu))
#இல்லையெனில்
    எனில் (unlikely (max > 0xFFFFu))
#endif
    {
      c->check_success (பொய், HB_SERIALIZE_ERROR_INT_OVERFLOW);
      return_trace (பொய்);
    }

    தேர்வு (u.format.v)
    {
    நிலை 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    நிலை 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    நிலை 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    இயல்பு:return_trace (பொய்);
    }
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_subset_plan_t *plan = c->plan;
    auto it =
    + iter ()
    | hb_take (plan->source->get_இலக்கம்_glyphs ())
    | hb_map_retains_sorting ([plan] (hb_codepoint_t g) {
	திரும்பு plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { திரும்பு glyph != HB_MAP_மதிப்பு_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_vector_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (தருக்க (glyphs));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    தேர்வு (u.format.v)
    {
    நிலை 1: hb_barrier (); திரும்பு u.format1.intersects (glyphs);
    நிலை 2: hb_barrier (); திரும்பு u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); திரும்பு u.format3.intersects (glyphs);
    நிலை 4: hb_barrier (); திரும்பு u.format4.intersects (glyphs);
#endif
    இயல்பு:திரும்பு பொய்;
    }
  }
  தருக்க intersects_coverage (மாறிலி hb_set_t *glyphs, unsigned முழு index) மாறிலி
  {
    தேர்வு (u.format.v)
    {
    நிலை 1: hb_barrier (); திரும்பு u.format1.intersects_coverage (glyphs, index);
    நிலை 2: hb_barrier (); திரும்பு u.format2.intersects_coverage (glyphs, index);
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); திரும்பு u.format3.intersects_coverage (glyphs, index);
    நிலை 4: hb_barrier (); திரும்பு u.format4.intersects_coverage (glyphs, index);
#endif
    இயல்பு:திரும்பு பொய்;
    }
  }

  unsigned cost () மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.cost ();
    நிலை 2: hb_barrier (); திரும்பு u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); திரும்பு u.format3.cost ();
    நிலை 4: hb_barrier (); திரும்பு u.format4.cost ();
#endif
    இயல்பு:திரும்பு 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  தருக்க collect_coverage (set_t *glyphs) மாறிலி
  {
    தேர்வு (u.format.v)
    {
    நிலை 1: hb_barrier (); திரும்பு u.format1.collect_coverage (glyphs);
    நிலை 2: hb_barrier (); திரும்பு u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); திரும்பு u.format3.collect_coverage (glyphs);
    நிலை 4: hb_barrier (); திரும்பு u.format4.collect_coverage (glyphs);
#endif
    இயல்பு:திரும்பு பொய்;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  வெறுமை intersect_set (மாறிலி hb_set_t &glyphs, IterableOut&& intersect_glyphs) மாறிலி
  {
    தேர்வு (u.format.v)
    {
    நிலை 1: hb_barrier (); திரும்பு u.format1.intersect_set (glyphs, intersect_glyphs);
    நிலை 2: hb_barrier (); திரும்பு u.format2.intersect_set (glyphs, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); திரும்பு u.format3.intersect_set (glyphs, intersect_glyphs);
    நிலை 4: hb_barrier (); திரும்பு u.format4.intersect_set (glyphs, intersect_glyphs);
#endif
    இயல்பு:திரும்பு ;
    }
  }

  கட்டமைப்பு iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    நிலையான constexpr தருக்க is_sorted_iterator = உண்மை;
    iter_t (மாறிலி Coverage &c_ = Null (Coverage))
    {
      hb_memset (இது, 0, அளவு (*இது));
      format = c_.u.format.v;
      தேர்வு (format)
      {
      நிலை 1: hb_barrier (); u.format1.init (c_.u.format1); திரும்பு;
      நிலை 2: hb_barrier (); u.format2.init (c_.u.format2); திரும்பு;
#ifndef HB_NO_BEYOND_64K
      நிலை 3: hb_barrier (); u.format3.init (c_.u.format3); திரும்பு;
      நிலை 4: hb_barrier (); u.format4.init (c_.u.format4); திரும்பு;
#endif
      இயல்பு:                               திரும்பு;
      }
    }
    தருக்க __more__ () மாறிலி
    {
      தேர்வு (format)
      {
      நிலை 1: hb_barrier (); திரும்பு u.format1.__more__ ();
      நிலை 2: hb_barrier (); திரும்பு u.format2.__more__ ();
#ifndef HB_NO_BEYOND_64K
      நிலை 3: hb_barrier (); திரும்பு u.format3.__more__ ();
      நிலை 4: hb_barrier (); திரும்பு u.format4.__more__ ();
#endif
      இயல்பு:திரும்பு பொய்;
      }
    }
    வெறுமை __next__ ()
    {
      தேர்வு (format)
      {
      நிலை 1: hb_barrier (); u.format1.__next__ (); நிறுத்து;
      நிலை 2: hb_barrier (); u.format2.__next__ (); நிறுத்து;
#ifndef HB_NO_BEYOND_64K
      நிலை 3: hb_barrier (); u.format3.__next__ (); நிறுத்து;
      நிலை 4: hb_barrier (); u.format4.__next__ (); நிறுத்து;
#endif
      இயல்பு:                   நிறுத்து;
      }
    }
    வகைவரை hb_codepoint_t __item_t__;
    __item_t__ __item__ () மாறிலி { திரும்பு get_glyph (); }

    hb_codepoint_t get_glyph () மாறிலி
    {
      தேர்வு (format)
      {
      நிலை 1: hb_barrier (); திரும்பு u.format1.get_glyph ();
      நிலை 2: hb_barrier (); திரும்பு u.format2.get_glyph ();
#ifndef HB_NO_BEYOND_64K
      நிலை 3: hb_barrier (); திரும்பு u.format3.get_glyph ();
      நிலை 4: hb_barrier (); திரும்பு u.format4.get_glyph ();
#endif
      இயல்பு:திரும்பு 0;
      }
    }
    தருக்க operator != (மாறிலி iter_t& o) மாறிலி
    {
      எனில் (unlikely (format != o.format)) திரும்பு உண்மை;
      தேர்வு (format)
      {
      நிலை 1: hb_barrier (); திரும்பு u.format1 != o.u.format1;
      நிலை 2: hb_barrier (); திரும்பு u.format2 != o.u.format2;
#ifndef HB_NO_BEYOND_64K
      நிலை 3: hb_barrier (); திரும்பு u.format3 != o.u.format3;
      நிலை 4: hb_barrier (); திரும்பு u.format4 != o.u.format4;
#endif
      இயல்பு:திரும்பு பொய்;
      }
    }
    iter_t __end__ () மாறிலி
    {
      iter_t it;
      it.format = format;
      தேர்வு (format)
      {
      நிலை 1: hb_barrier (); it.u.format1 = u.format1.__end__ (); நிறுத்து;
      நிலை 2: hb_barrier (); it.u.format2 = u.format2.__end__ (); நிறுத்து;
#ifndef HB_NO_BEYOND_64K
      நிலை 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); நிறுத்து;
      நிலை 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); நிறுத்து;
#endif
      இயல்பு: நிறுத்து;
      }
      திரும்பு it;
    }

    தனிப்பட்ட:
    unsigned முழு format;
    ஒன்றிப்பு {
#ifndef HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#endif
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () மாறிலி { திரும்பு iter_t (*இது); }
};

template<typename Iterator>
நிலையான உள்வரி வெறுமை
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->start_embed<Coverage> ()->serialize (c, it); }

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH
