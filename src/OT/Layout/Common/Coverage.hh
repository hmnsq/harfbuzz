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

#include "../../../hb-ot-dual.hh"
#include "CoverageFormat1.hh"
#include "CoverageFormat2.hh"

namespace OT {
namespace Layout {
namespace Common {

template<typename Iterator>
static anta_hstha void Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

sa_nracanaa Coverage
{

  rak_sita:
  sa_ngha {
  sa_nracanaa { HBUINT16 v; }        format;         /* Format identifier */
  CoverageFormat1_3<SmallTypes> format1;
  CoverageFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  CoverageFormat1_3<MediumTypes>format3;
  CoverageFormat2_4<MediumTypes>format4;
#endif
  } u;
  public:
  DEFINE_SIZE_UNION (2, format.v);

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    གལ་སྲིད (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    switch (u.format.v)
    {
    pak_sa 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    pak_sa 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    pak_sa 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    default:return_trace (true);
    }
  }

  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) const { ལོག get (k); }
  bool has (hb_codepoint_t k) const { ལོག (*this)[k] != NOT_COVERED; }
  /* Predicate. */
  bool operator () (hb_codepoint_t k) const { ལོག has (k); }

  unsigned ཧྲིལ get (hb_codepoint_t k) const { ལོག get_coverage (k); }
  unsigned ཧྲིལ get_coverage (hb_codepoint_t glyph_id) const
  {
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); ལོག u.format1.get_coverage (glyph_id);
    pak_sa 2: hb_barrier (); ལོག u.format2.get_coverage (glyph_id);
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); ལོག u.format3.get_coverage (glyph_id);
    pak_sa 4: hb_barrier (); ལོག u.format4.get_coverage (glyph_id);
#endif
    default:ལོག NOT_COVERED;
    }
  }
  unsigned ཧྲིལ get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) const
  {
    unsigned coverage;
    གལ་སྲིད (cache && cache->get (glyph_id, &coverage)) ལོག coverage < cache->MAX_VALUE ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    གལ་སྲིད (cache) {
      གལ་སྲིད (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_VALUE);
      དེ་མིན གལ་སྲིད (likely (coverage < cache->MAX_VALUE))
	cache->set (glyph_id, coverage);
    }
    ལོག coverage;
  }

  unsigned ཧྲིལ get_coverage_binary (hb_codepoint_t glyph_id,
				    hb_ot_layout_binary_cache_t *cache) const
  {
    unsigned coverage;
    གལ་སྲིད (cache && cache->get (glyph_id, &coverage)) ལོག coverage < cache->MAX_VALUE ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    གལ་སྲིད (cache) {
      གལ་སྲིད (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_VALUE);
      དེ་མིན
	cache->set (glyph_id, 0);
    }
    ལོག coverage;
  }

  unsigned get_population () const
  {
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); ལོག u.format1.get_population ();
    pak_sa 2: hb_barrier (); ལོག u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); ལོག u.format3.get_population ();
    pak_sa 4: hb_barrier (); ལོག u.format4.get_population ();
#endif
    default:ལོག NOT_COVERED;
    }
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (this);
    གལ་སྲིད (unlikely (!c->extend_min (this))) return_trace (false);

    unsigned count = hb_len (+ glyphs);
    unsigned num_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    bool unsorted = false;
    དོན་ལུ (auto g: + glyphs)
    {
      གལ་སྲིད (last != (hb_codepoint_t) -2 && g < last)
	unsorted = true;
      གལ་སྲིད (last + 1 != g)
	num_ranges++;
      last = g;
      གལ་སྲིད (g > max) max = g;
    }
    u.format.v = !unsorted && count <= num_ranges * 3 ? 1 : 2;

#ifndef HB_NO_BEYOND_64K
    གལ་སྲིད (max > 0xFFFFu)
      u.format.v += 2;
    གལ་སྲིད (unlikely (max > 0xFFFFFFu))
#དེ་མིན
    གལ་སྲིད (unlikely (max > 0xFFFFu))
#endif
    {
      c->check_success (false, HB_SERIALIZE_ERROR_INT_OVERFLOW);
      return_trace (false);
    }

    switch (u.format.v)
    {
    pak_sa 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    pak_sa 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    pak_sa 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    default:return_trace (false);
    }
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    const hb_subset_plan_t *plan = c->plan;
    auto it =
    + iter ()
    | hb_take (plan->source->get_num_glyphs ())
    | hb_map_retains_sorting ([plan] (hb_codepoint_t g) {
	ལོག plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { ལོག glyph != HB_MAP_VALUE_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_vector_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (bool (glyphs));
  }

  bool intersects (const hb_set_t *glyphs) const
  {
    switch (u.format.v)
    {
    pak_sa 1: hb_barrier (); ལོག u.format1.intersects (glyphs);
    pak_sa 2: hb_barrier (); ལོག u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); ལོག u.format3.intersects (glyphs);
    pak_sa 4: hb_barrier (); ལོག u.format4.intersects (glyphs);
#endif
    default:ལོག false;
    }
  }
  bool intersects_coverage (const hb_set_t *glyphs, unsigned ཧྲིལ index) const
  {
    switch (u.format.v)
    {
    pak_sa 1: hb_barrier (); ལོག u.format1.intersects_coverage (glyphs, index);
    pak_sa 2: hb_barrier (); ལོག u.format2.intersects_coverage (glyphs, index);
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); ལོག u.format3.intersects_coverage (glyphs, index);
    pak_sa 4: hb_barrier (); ལོག u.format4.intersects_coverage (glyphs, index);
#endif
    default:ལོག false;
    }
  }

  unsigned cost () const
  {
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); ལོག u.format1.cost ();
    pak_sa 2: hb_barrier (); ལོག u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); ལོག u.format3.cost ();
    pak_sa 4: hb_barrier (); ལོག u.format4.cost ();
#endif
    default:ལོག 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  bool collect_coverage (set_t *glyphs) const
  {
    switch (u.format.v)
    {
    pak_sa 1: hb_barrier (); ལོག u.format1.collect_coverage (glyphs);
    pak_sa 2: hb_barrier (); ལོག u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); ལོག u.format3.collect_coverage (glyphs);
    pak_sa 4: hb_barrier (); ལོག u.format4.collect_coverage (glyphs);
#endif
    default:ལོག false;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  void intersect_set (const hb_set_t &glyphs, IterableOut&& intersect_glyphs) const
  {
    switch (u.format.v)
    {
    pak_sa 1: hb_barrier (); ལོག u.format1.intersect_set (glyphs, intersect_glyphs);
    pak_sa 2: hb_barrier (); ལོག u.format2.intersect_set (glyphs, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); ལོག u.format3.intersect_set (glyphs, intersect_glyphs);
    pak_sa 4: hb_barrier (); ལོག u.format4.intersect_set (glyphs, intersect_glyphs);
#endif
    default:ལོག ;
    }
  }

  sa_nracanaa iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    static constexpr bool is_sorted_iterator = true;
    iter_t (const Coverage &c_ = Null (Coverage))
    {
      hb_memset (this, 0, parimaa_na (*this));
      format = c_.u.format.v;
      switch (format)
      {
      pak_sa 1: hb_barrier (); u.format1.init (c_.u.format1); ལོག;
      pak_sa 2: hb_barrier (); u.format2.init (c_.u.format2); ལོག;
#ifndef HB_NO_BEYOND_64K
      pak_sa 3: hb_barrier (); u.format3.init (c_.u.format3); ལོག;
      pak_sa 4: hb_barrier (); u.format4.init (c_.u.format4); ལོག;
#endif
      default:                               ལོག;
      }
    }
    bool __more__ () const
    {
      switch (format)
      {
      pak_sa 1: hb_barrier (); ལོག u.format1.__more__ ();
      pak_sa 2: hb_barrier (); ལོག u.format2.__more__ ();
#ifndef HB_NO_BEYOND_64K
      pak_sa 3: hb_barrier (); ལོག u.format3.__more__ ();
      pak_sa 4: hb_barrier (); ལོག u.format4.__more__ ();
#endif
      default:ལོག false;
      }
    }
    void __next__ ()
    {
      switch (format)
      {
      pak_sa 1: hb_barrier (); u.format1.__next__ (); bha_nga;
      pak_sa 2: hb_barrier (); u.format2.__next__ (); bha_nga;
#ifndef HB_NO_BEYOND_64K
      pak_sa 3: hb_barrier (); u.format3.__next__ (); bha_nga;
      pak_sa 4: hb_barrier (); u.format4.__next__ (); bha_nga;
#endif
      default:                   bha_nga;
      }
    }
    typedef hb_codepoint_t __item_t__;
    __item_t__ __item__ () const { ལོག get_glyph (); }

    hb_codepoint_t get_glyph () const
    {
      switch (format)
      {
      pak_sa 1: hb_barrier (); ལོག u.format1.get_glyph ();
      pak_sa 2: hb_barrier (); ལོག u.format2.get_glyph ();
#ifndef HB_NO_BEYOND_64K
      pak_sa 3: hb_barrier (); ལོག u.format3.get_glyph ();
      pak_sa 4: hb_barrier (); ལོག u.format4.get_glyph ();
#endif
      default:ལོག 0;
      }
    }
    bool operator != (const iter_t& o) const
    {
      གལ་སྲིད (unlikely (format != o.format)) ལོག true;
      switch (format)
      {
      pak_sa 1: hb_barrier (); ལོག u.format1 != o.u.format1;
      pak_sa 2: hb_barrier (); ལོག u.format2 != o.u.format2;
#ifndef HB_NO_BEYOND_64K
      pak_sa 3: hb_barrier (); ལོག u.format3 != o.u.format3;
      pak_sa 4: hb_barrier (); ལོག u.format4 != o.u.format4;
#endif
      default:ལོག false;
      }
    }
    iter_t __end__ () const
    {
      iter_t it;
      it.format = format;
      switch (format)
      {
      pak_sa 1: hb_barrier (); it.u.format1 = u.format1.__end__ (); bha_nga;
      pak_sa 2: hb_barrier (); it.u.format2 = u.format2.__end__ (); bha_nga;
#ifndef HB_NO_BEYOND_64K
      pak_sa 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); bha_nga;
      pak_sa 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); bha_nga;
#endif
      default: bha_nga;
      }
      ལོག it;
    }

    private:
    unsigned ཧྲིལ format;
    sa_ngha {
#ifndef HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#endif
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () const { ལོག iter_t (*this); }
};

template<typename Iterator>
static anta_hstha void
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->start_embed<Coverage> ()->serialize (c, it); }

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH
