/*
 * Copyright © 2022  Google, Inc.
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
 * Google Author(s): Garret Rieger
 */

#شامل "graph.hh"
#شامل "../hb-ot-layout-common.hh"
#شامل "graph-result.hh"

#ifndef GRAPH_CLASSDEF_GRAPH_HH
#define GRAPH_CLASSDEF_GRAPH_HH

فضاینام graph {

template <typename Types>
ساختار ClassDefFormat1_3 : عمومی OT::ClassDefFormat1_3<Types>
{
  graph_result_t<تهی> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size();
    constexpr unsigned min_size = OT::ClassDefFormat1_3<Types>::min_size;
    اگر (unlikely (vertex_len < min_size)) بازگشت Err(SANITIZE_FAILURE);
    hb_barrier ();
    اگر (unlikely (vertex_len < min_size + این->class_مقدار.get_size () - این->class_مقدار.len.get_size ()))
      بازگشت Err(SANITIZE_FAILURE);
    بازگشت Ok();
  }
};

template <typename Types>
ساختار ClassDefFormat2_4 : عمومی OT::ClassDefFormat2_4<Types>
{
  graph_result_t<تهی> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size();
    constexpr unsigned min_size = OT::ClassDefFormat2_4<Types>::min_size;
    اگر (unlikely (vertex_len < min_size)) بازگشت Err(SANITIZE_FAILURE);
    hb_barrier ();
    اگر (unlikely (vertex_len < min_size + این->rangeRecord.get_size () - این->rangeRecord.len.get_size ()))
      بازگشت Err(SANITIZE_FAILURE);
    بازگشت Ok();
  }
};

ساختار ClassDef : عمومی OT::ClassDef
{
  template<typename It>
  ایستا graph_result_t<تهی> add_class_def (gsubgpos_graph_context_t& c,
                                             unsigned parent_id,
                                             unsigned link_position,
                                             It glyph_and_class,
                                             unsigned max_size)
  {
    TRY_ASSIGN (unsigned class_def_اول_id, c.graph.new_گره (nullptr, nullptr));
    auto& class_def_اول_vertex = c.graph.vertices_[class_def_اول_id];
    TRY (make_class_def (c, glyph_and_class, class_def_اول_id, max_size));

    TRY(c.graph.vertices_[parent_id].add_real_link (SmallTypes::size, class_def_اول_id, link_position));
    TRY(class_def_اول_vertex.add_parent (parent_id, نادرست));

    بازگشت Ok();
  }

  template<typename It>
  ایستا graph_result_t<تهی> make_class_def (gsubgpos_graph_context_t& c,
                                              It glyph_and_class,
                                              unsigned dest_obj,
                                              unsigned max_size)
  {
    نویسه* buffer = (نویسه*) hb_calloc (1, max_size);
    اگر (unlikely (!buffer))
      بازگشت Err(ALLOCATION_FAILURE);

    hb_serialize_context_t serializer (buffer, max_size);
    OT::ClassDef_serialize (&serializer, glyph_and_class);
    serializer.end_serialize ();
    اگر (unlikely (serializer.in_error ()))
    {
      hb_free (buffer);
      بازگشت Err(ALLOCATION_FAILURE);
    }

    hb_bytes_t class_def_copy = serializer.copy_bytes ();
    اگر (unlikely (!class_def_copy.arrayZ))
    {
      hb_free (buffer);
      بازگشت Err(ALLOCATION_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    auto res = c.add_buffer ((نویسه *) class_def_copy.arrayZ);
    اگر (unlikely (!res.is_ok ()))
    {
      hb_free (buffer);
      hb_free ((نویسه *) class_def_copy.arrayZ);
      بازگشت res;
    }


    auto& v = c.graph.vertices_[dest_obj];
    نویسه* head = (نویسه *) class_def_copy.arrayZ;
    v.set_buffer (head, head + class_def_copy.length);

    hb_free (buffer);
    بازگشت Ok();
  }

  graph_result_t<تهی> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size();
    اگر (unlikely (vertex_len < OT::ClassDef::min_size)) بازگشت Err(SANITIZE_FAILURE);
    hb_barrier ();
    انتخاب (u.format.v)
    {
    حالت 1: بازگشت ((ClassDefFormat1_3<SmallTypes>*)این)->sanitize (vertex);
    حالت 2: بازگشت ((ClassDefFormat2_4<SmallTypes>*)این)->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    حالت 3: بازگشت ((ClassDefFormat1_3<MediumTypes>*)این)->sanitize (vertex);
    حالت 4: بازگشت ((ClassDefFormat2_4<MediumTypes>*)این)->sanitize (vertex);
#endif
    پیشفرض: بازگشت Err(SANITIZE_FAILURE);
    }
  }
};


ساختار class_def_size_estimator_t
{
  template<typename It>
  ایستا graph_result_t<class_def_size_estimator_t> create (It glyph_and_class)
  {
    class_def_size_estimator_t estimator;
    estimator.reset();
    برای (auto p : + glyph_and_class)
    {
      unsigned gid = p.first;
      unsigned klass = p.second;

      hb_set_t* glyphs;
      اگر (estimator.glyphs_per_class.has (klass, &glyphs) && glyphs) {
        glyphs->add (gid);
        ادامه;
      }

      hb_set_t new_glyphs;
      new_glyphs.add (gid);
      estimator.glyphs_per_class.set (klass, std::move (new_glyphs));
    }

    TRY(estimator.to_result ());

    برای (unsigned klass : estimator.glyphs_per_class.کلیدها ())
    {
      اگر (!klass) ادامه; // class 0 doesn't get encoded.

      ثابت hb_set_t& glyphs = estimator.glyphs_per_class.get (klass);
      hb_codepoint_t start = HB_SET_مقدار_INVALID;
      hb_codepoint_t end = HB_SET_مقدار_INVALID;

      unsigned شمار = 0;
      وقتی (glyphs.next_range (&start, &end))
        شمار++;

      estimator.رقم_ranges_per_class.set (klass, شمار);
    }

    TRY(estimator.to_result ());
    بازگشت estimator;
  }

  تهی reset() {
    included_glyphs.clear();
    included_class_def_glyphs.clear();
    included_classes.clear();
    class_def_range_شمار = 0;
    class_max = 0;
    coverage_size_val = compute_coverage_size ();
  }

  // Size of coverage for all glyphs added via 'add_class_def_size'.
  unsigned coverage_size () ثابت
  {
    بازگشت coverage_size_val;
  }

  // Compute the new size of the ClassDef table if all glyphs associated with 'klass' were added.
  unsigned add_class_def_size (unsigned klass)
  {
    اگر (!included_classes.has(klass)) {
      hb_set_t* glyphs = nullptr;
      اگر (glyphs_per_class.has(klass, &glyphs)) {
        unsigned رقم_glyphs = included_glyphs.get_population();
        included_glyphs.union_(*glyphs);
        اگر (klass)
          included_class_def_glyphs.union_(*glyphs);
        اگر (رقم_glyphs != included_glyphs.get_population())
          coverage_size_val = compute_coverage_size ();
      }

      اگر (klass)
      {
        class_def_range_شمار += رقم_ranges_per_class.get (klass);
        اگر (glyphs && klass > class_max)
          class_max = klass;
      }
      included_classes.add(klass);
    }

    بازگشت class_def_size ();
  }

  unsigned رقم_glyph_ranges (ثابت hb_set_t &glyphs) ثابت {
    hb_codepoint_t start = HB_SET_مقدار_INVALID;
    hb_codepoint_t end = HB_SET_مقدار_INVALID;

    unsigned شمار = 0;
    وقتی (glyphs.next_range (&start, &end)) {
        شمار++;
    }
    بازگشت شمار;
  }

 خصوصی:
  graph_result_t<تهی> to_result() ثابت
  {
    TRY (graph_result_t<تهی>::from (رقم_ranges_per_class, ALLOCATION_FAILURE));
    TRY (graph_result_t<تهی>::from (glyphs_per_class, ALLOCATION_FAILURE));
    TRY (graph_result_t<تهی>::from (included_classes, ALLOCATION_FAILURE));
    TRY (graph_result_t<تهی>::from (included_glyphs, ALLOCATION_FAILURE));
    TRY (graph_result_t<تهی>::from (included_class_def_glyphs, ALLOCATION_FAILURE));

    برای (ثابت hb_set_t& s : glyphs_per_class.values ())
    {
          TRY (graph_result_t<تهی>::from (s, ALLOCATION_FAILURE));
    }
    بازگشت Ok();
  }

  class_def_size_estimator_t ()
      : رقم_ranges_per_class (), glyphs_per_class () {}

  unsigned compute_coverage_size () ثابت
  {
    unsigned glyph_size = glyph_id_size (included_glyphs);
    unsigned coverage_base_size = OT::HBUINT16::static_size + glyph_size;
    unsigned range_size = 3 * glyph_size;
    unsigned format1_size = coverage_base_size + glyph_size * included_glyphs.get_population();
    unsigned format2_size = coverage_base_size + range_size * رقم_glyph_ranges (included_glyphs);
    بازگشت hb_min(format1_size, format2_size);
  }

  unsigned glyph_id_size (ثابت hb_set_t &glyphs) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (!glyphs.is_empty () && glyphs.get_max () > 0xFFFFu)
      بازگشت OT::Layout::MediumTypes::size;
#endif
    بازگشت OT::Layout::SmallTypes::size;
  }

  unsigned class_def_size () ثابت
  {
    اگر (included_class_def_glyphs.is_empty ())
      بازگشت OT::HBUINT16::static_size + OT::Layout::SmallTypes::size;

    unsigned best_size = UINT_MAX;
    hb_codepoint_t glyph_max = included_class_def_glyphs.get_max ();
    unsigned glyph_span = glyph_max - included_class_def_glyphs.get_min () + 1;

    اگر (glyph_max <= 0xFFFFu && glyph_span <= 0xFFFFu && class_max <= 0xFFFFu)
      best_size = hb_min (best_size, 6 + 2 * glyph_span);
    اگر (glyph_max <= 0xFFFFu && class_def_range_شمار <= 0xFFFFu && class_max <= 0xFFFFu)
      best_size = hb_min (best_size, 4 + 6 * class_def_range_شمار);
#ifndef HB_NO_BEYOND_64K
    اگر (glyph_max <= 0xFFFFFFu && glyph_span <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
      best_size = hb_min (best_size, 8 + 3 * glyph_span);
    اگر (glyph_max <= 0xFFFFFFu && class_def_range_شمار <= 0xFFFFFFu && class_max <= 0xFFFFu)
      best_size = hb_min (best_size, 5 + 8 * class_def_range_شمار);
#endif
    بازگشت best_size;
  }

  hb_hashmap_t<unsigned, unsigned> رقم_ranges_per_class;
  hb_hashmap_t<unsigned, hb_set_t> glyphs_per_class;
  hb_set_t included_classes;
  hb_set_t included_glyphs;
  hb_set_t included_class_def_glyphs;
  unsigned class_def_range_شمار;
  unsigned class_max;
  unsigned coverage_size_val;
};


}

#endif  // GRAPH_CLASSDEF_GRAPH_HH
