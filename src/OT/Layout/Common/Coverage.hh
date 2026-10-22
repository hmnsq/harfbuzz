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

#अगर_अघोषित OT_LAYOUT_COMMON_COVERAGE_HH
#घोषणा OT_LAYOUT_COMMON_COVERAGE_HH

#समावेश "../../../hb-ot-dual.hh"
#समावेश "CoverageFormat1.hh"
#समावेश "CoverageFormat2.hh"

namespace OT {
namespace Layout {
namespace Common {

ढाँचा<typename Iterator>
अटल अंतरभूत व्योम Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

काष्ठा Coverage
{

  रक्षित:
  जोड़ {
  काष्ठा { HBUINT16 v; }        format;         /* Format identifier */
  CoverageFormat1_3<SmallTypes> format1;
  CoverageFormat2_4<SmallTypes> format2;
#अगर_अघोषित HB_NO_BEYOND_64K
  CoverageFormat1_3<MediumTypes>format3;
  CoverageFormat2_4<MediumTypes>format4;
#पूर्ण_अगर
  } u;
  खुला:
  DEFINE_आकार_UNION (2, format.v);

#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    चयन (u.format.v)
    {
    हाल 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    हाल 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    हाल 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#पूर्ण_अगर
    शेष:return_trace (true);
    }
  }

  /* Has interface. */
  अचिन्हित चालक [] (hb_codepoint_t k) स्थिर { वापस लो (k); }
  bool has (hb_codepoint_t k) स्थिर { वापस (*यह)[k] != NOT_COVERED; }
  /* Predicate. */
  bool चालक () (hb_codepoint_t k) स्थिर { वापस has (k); }

  अचिन्हित पूर्णांक लो (hb_codepoint_t k) स्थिर { वापस get_coverage (k); }
  अचिन्हित पूर्णांक get_coverage (hb_codepoint_t glyph_id) स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.get_coverage (glyph_id);
    हाल 2: hb_barrier (); वापस u.format2.get_coverage (glyph_id);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); वापस u.format3.get_coverage (glyph_id);
    हाल 4: hb_barrier (); वापस u.format4.get_coverage (glyph_id);
#पूर्ण_अगर
    शेष:वापस NOT_COVERED;
    }
  }
  अचिन्हित पूर्णांक get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) स्थिर
  {
    अचिन्हित coverage;
    अगर (cache && cache->लो (glyph_id, &coverage)) वापस coverage < cache->MAX_मूल्य ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    अगर (cache) {
      अगर (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_मूल्य);
      अन्यथा अगर (likely (coverage < cache->MAX_मूल्य))
	cache->set (glyph_id, coverage);
    }
    वापस coverage;
  }

  अचिन्हित पूर्णांक get_coverage_द्विआधारी (hb_codepoint_t glyph_id,
				    hb_ot_layout_द्विआधारी_cache_t *cache) स्थिर
  {
    अचिन्हित coverage;
    अगर (cache && cache->लो (glyph_id, &coverage)) वापस coverage < cache->MAX_मूल्य ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    अगर (cache) {
      अगर (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_मूल्य);
      अन्यथा
	cache->set (glyph_id, 0);
    }
    वापस coverage;
  }

  अचिन्हित get_population () स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.get_population ();
    हाल 2: hb_barrier (); वापस u.format2.get_population ();
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); वापस u.format3.get_population ();
    हाल 4: hb_barrier (); वापस u.format4.get_population ();
#पूर्ण_अगर
    शेष:वापस NOT_COVERED;
    }
  }

  ढाँचा <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);

    अचिन्हित गिनती = hb_len (+ glyphs);
    अचिन्हित अंक_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    bool unsorted = false;
    क्रम (स्वतः g: + glyphs)
    {
      अगर (last != (hb_codepoint_t) -2 && g < last)
	unsorted = true;
      अगर (last + 1 != g)
	अंक_ranges++;
      last = g;
      अगर (g > max) max = g;
    }
    u.format.v = !unsorted && गिनती <= अंक_ranges * 3 ? 1 : 2;

#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (max > 0xFFFFu)
      u.format.v += 2;
    अगर (unlikely (max > 0xFFFFFFu))
#अन्यथा
    अगर (unlikely (max > 0xFFFFu))
#पूर्ण_अगर
    {
      c->check_success (false, HB_SERIALIZE_ERROR_INT_OVERFLOW);
      return_trace (false);
    }

    चयन (u.format.v)
    {
    हाल 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    हाल 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    हाल 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#पूर्ण_अगर
    शेष:return_trace (false);
    }
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_subset_plan_t *plan = c->plan;
    स्वतः it =
    + iter ()
    | hb_take (plan->source->get_अंक_glyphs ())
    | hb_map_retains_sorting ([plan] (hb_codepoint_t g) {
	वापस plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { वापस glyph != HB_MAP_मूल्य_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_vector_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (bool (glyphs));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    चयन (u.format.v)
    {
    हाल 1: hb_barrier (); वापस u.format1.intersects (glyphs);
    हाल 2: hb_barrier (); वापस u.format2.intersects (glyphs);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); वापस u.format3.intersects (glyphs);
    हाल 4: hb_barrier (); वापस u.format4.intersects (glyphs);
#पूर्ण_अगर
    शेष:वापस false;
    }
  }
  bool intersects_coverage (स्थिर hb_set_t *glyphs, अचिन्हित पूर्णांक सूचकांक_2) स्थिर
  {
    चयन (u.format.v)
    {
    हाल 1: hb_barrier (); वापस u.format1.intersects_coverage (glyphs, सूचकांक_2);
    हाल 2: hb_barrier (); वापस u.format2.intersects_coverage (glyphs, सूचकांक_2);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); वापस u.format3.intersects_coverage (glyphs, सूचकांक_2);
    हाल 4: hb_barrier (); वापस u.format4.intersects_coverage (glyphs, सूचकांक_2);
#पूर्ण_अगर
    शेष:वापस false;
    }
  }

  अचिन्हित cost () स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.cost ();
    हाल 2: hb_barrier (); वापस u.format2.cost ();
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); वापस u.format3.cost ();
    हाल 4: hb_barrier (); वापस u.format4.cost ();
#पूर्ण_अगर
    शेष:वापस 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ढाँचा <typename set_t>
  bool collect_coverage (set_t *glyphs) स्थिर
  {
    चयन (u.format.v)
    {
    हाल 1: hb_barrier (); वापस u.format1.collect_coverage (glyphs);
    हाल 2: hb_barrier (); वापस u.format2.collect_coverage (glyphs);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); वापस u.format3.collect_coverage (glyphs);
    हाल 4: hb_barrier (); वापस u.format4.collect_coverage (glyphs);
#पूर्ण_अगर
    शेष:वापस false;
    }
  }

  ढाँचा <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  व्योम intersect_set (स्थिर hb_set_t &glyphs, IterableOut&& intersect_glyphs) स्थिर
  {
    चयन (u.format.v)
    {
    हाल 1: hb_barrier (); वापस u.format1.intersect_set (glyphs, intersect_glyphs);
    हाल 2: hb_barrier (); वापस u.format2.intersect_set (glyphs, intersect_glyphs);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); वापस u.format3.intersect_set (glyphs, intersect_glyphs);
    हाल 4: hb_barrier (); वापस u.format4.intersect_set (glyphs, intersect_glyphs);
#पूर्ण_अगर
    शेष:वापस ;
    }
  }

  काष्ठा iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    अटल constexpr bool is_sorted_iterator = true;
    iter_t (स्थिर Coverage &c_ = Null (Coverage))
    {
      hb_memset (यह, 0, माप (*यह));
      format = c_.u.format.v;
      चयन (format)
      {
      हाल 1: hb_barrier (); u.format1.init (c_.u.format1); वापस;
      हाल 2: hb_barrier (); u.format2.init (c_.u.format2); वापस;
#अगर_अघोषित HB_NO_BEYOND_64K
      हाल 3: hb_barrier (); u.format3.init (c_.u.format3); वापस;
      हाल 4: hb_barrier (); u.format4.init (c_.u.format4); वापस;
#पूर्ण_अगर
      शेष:                               वापस;
      }
    }
    bool __more__ () स्थिर
    {
      चयन (format)
      {
      हाल 1: hb_barrier (); वापस u.format1.__more__ ();
      हाल 2: hb_barrier (); वापस u.format2.__more__ ();
#अगर_अघोषित HB_NO_BEYOND_64K
      हाल 3: hb_barrier (); वापस u.format3.__more__ ();
      हाल 4: hb_barrier (); वापस u.format4.__more__ ();
#पूर्ण_अगर
      शेष:वापस false;
      }
    }
    व्योम __next__ ()
    {
      चयन (format)
      {
      हाल 1: hb_barrier (); u.format1.__next__ (); अवरोध;
      हाल 2: hb_barrier (); u.format2.__next__ (); अवरोध;
#अगर_अघोषित HB_NO_BEYOND_64K
      हाल 3: hb_barrier (); u.format3.__next__ (); अवरोध;
      हाल 4: hb_barrier (); u.format4.__next__ (); अवरोध;
#पूर्ण_अगर
      शेष:                   अवरोध;
      }
    }
    प्रकार hb_codepoint_t __item_t__;
    __item_t__ __item__ () स्थिर { वापस get_glyph (); }

    hb_codepoint_t get_glyph () स्थिर
    {
      चयन (format)
      {
      हाल 1: hb_barrier (); वापस u.format1.get_glyph ();
      हाल 2: hb_barrier (); वापस u.format2.get_glyph ();
#अगर_अघोषित HB_NO_BEYOND_64K
      हाल 3: hb_barrier (); वापस u.format3.get_glyph ();
      हाल 4: hb_barrier (); वापस u.format4.get_glyph ();
#पूर्ण_अगर
      शेष:वापस 0;
      }
    }
    bool चालक != (स्थिर iter_t& o) स्थिर
    {
      अगर (unlikely (format != o.format)) वापस true;
      चयन (format)
      {
      हाल 1: hb_barrier (); वापस u.format1 != o.u.format1;
      हाल 2: hb_barrier (); वापस u.format2 != o.u.format2;
#अगर_अघोषित HB_NO_BEYOND_64K
      हाल 3: hb_barrier (); वापस u.format3 != o.u.format3;
      हाल 4: hb_barrier (); वापस u.format4 != o.u.format4;
#पूर्ण_अगर
      शेष:वापस false;
      }
    }
    iter_t __end__ () स्थिर
    {
      iter_t it;
      it.format = format;
      चयन (format)
      {
      हाल 1: hb_barrier (); it.u.format1 = u.format1.__end__ (); अवरोध;
      हाल 2: hb_barrier (); it.u.format2 = u.format2.__end__ (); अवरोध;
#अगर_अघोषित HB_NO_BEYOND_64K
      हाल 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); अवरोध;
      हाल 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); अवरोध;
#पूर्ण_अगर
      शेष: अवरोध;
      }
      वापस it;
    }

    निजी:
    अचिन्हित पूर्णांक format;
    जोड़ {
#अगर_अघोषित HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#पूर्ण_अगर
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () स्थिर { वापस iter_t (*यह); }
};

ढाँचा<typename Iterator>
अटल अंतरभूत व्योम
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->start_embed<Coverage> ()->serialize (c, it); }

}
}
}

#पूर्ण_अगर  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH
