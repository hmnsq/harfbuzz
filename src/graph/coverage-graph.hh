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

#include "graph.hh"
#include "../OT/Layout/Common/Coverage.hh"

#ifndef GRAPH_COVERAGE_GRAPH_HH
#define GRAPH_COVERAGE_GRAPH_HH

namespace graph {

template <typename Types>
static graph_result_t<void> sanitize (
  const OT::Layout::Common::CoverageFormat1_3<Types>* thiz,
  const graph_t::vertex_t& vertex
) {
  size_t vertex_len = vertex.table_size();
  constexpr unsigned min_size = OT::Layout::Common::CoverageFormat1_3<Types>::min_size;
  ако (unlikely (vertex_len < min_size)) врати Err(SANITIZE_FAILURE);
  hb_barrier ();
  ако (unlikely (vertex_len < min_size + thiz->glyphArray.get_size () - thiz->glyphArray.len.get_size ()))
    врати Err(SANITIZE_FAILURE);
  врати Ok();
}

template <typename Types>
static graph_result_t<void> sanitize (
  const OT::Layout::Common::CoverageFormat2_4<Types>* thiz,
  const graph_t::vertex_t& vertex
) {
  size_t vertex_len = vertex.table_size();
  constexpr unsigned min_size = OT::Layout::Common::CoverageFormat2_4<Types>::min_size;
  ако (unlikely (vertex_len < min_size)) врати Err(SANITIZE_FAILURE);
  hb_barrier ();
  ако (unlikely (vertex_len < min_size + thiz->rangeRecord.get_size () - thiz->rangeRecord.len.get_size ()))
    врати Err(SANITIZE_FAILURE);
  врати Ok();
}

struct Coverage : public OT::Layout::Common::Coverage
{
  static graph_result_t<void> clone_coverage (gsubgpos_graph_context_t& c,
                                              unsigned coverage_id,
                                              unsigned new_parent_id,
                                              unsigned link_position,
                                              unsigned start, unsigned end)

  {
    unsigned coverage_size = c.graph.vertices_[coverage_id].table_size ();
    auto& coverage_v = c.graph.vertices_[coverage_id];
    const Coverage* coverage_table = (const Coverage*) coverage_v.obj().head;
    ако (unlikely (!coverage_table))
      врати Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));

    auto new_coverage =
        + hb_zip (coverage_table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          врати p.second >= start && p.second < end;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    врати add_coverage (c, new_parent_id, link_position, new_coverage, coverage_size);
  }

  template<typename It>
  static graph_result_t<void> add_coverage (gsubgpos_graph_context_t& c,
                                            unsigned parent_id,
                                            unsigned link_position,
                                            It glyphs,
                                            unsigned max_size)
  {
    TRY_ASSIGN (unsigned coverage_prime_id, c.graph.new_node (nullptr, nullptr));
    auto& coverage_prime_vertex = c.graph.vertices_[coverage_prime_id];
    TRY (make_coverage (c, glyphs, coverage_prime_id, max_size));

    TRY(c.graph.vertices_[parent_id].add_real_link (SmallTypes::size, coverage_prime_id, link_position));
    TRY(coverage_prime_vertex.add_parent (parent_id, false));

    врати Ok();
  }

  // Filter an existing coverage table to glyphs at indices [start, end) and replace it with the filtered version.
  static graph_result_t<void> filter_coverage (gsubgpos_graph_context_t& c,
                                                unsigned existing_coverage,
                                                unsigned start, unsigned end) {
    unsigned coverage_size = c.graph.vertices_[existing_coverage].table_size ();
    auto& coverage_v = c.graph.vertices_[existing_coverage];
    const Coverage* coverage_table = (const Coverage*) coverage_v.obj().head;
    ако (unlikely (!coverage_table))
      врати Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));

    auto new_coverage =
        + hb_zip (coverage_table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          врати p.second >= start && p.second < end;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    врати make_coverage (c, new_coverage, existing_coverage, coverage_size * 2 + 100);
  }

  // Replace the coverage table at dest obj with one covering 'glyphs'.
  template<typename It>
  static graph_result_t<void> make_coverage (gsubgpos_graph_context_t& c,
                                             It glyphs,
                                             unsigned dest_obj,
                                             unsigned max_size)
  {
    char* buffer = (char*) hb_calloc (1, max_size);
    ако (unlikely (!buffer))
      врати Err(ALLOCATION_FAILURE);

    hb_serialize_context_t serializer (buffer, max_size);
    OT::Layout::Common::Coverage_serialize (&serializer, glyphs);
    serializer.end_serialize ();
    ако (unlikely (serializer.in_error ()))
    {
      hb_free (buffer);
      врати Err(ALLOCATION_FAILURE);
    }

    hb_bytes_t coverage_copy = serializer.copy_bytes ();
    ако (unlikely (!coverage_copy.arrayZ)) {
      hb_free (buffer);
      врати Err(ALLOCATION_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    auto res = c.add_buffer ((char *) coverage_copy.arrayZ);
    ако (unlikely (!res.is_ok ()))
    {
      hb_free (buffer);
      hb_free ((char *) coverage_copy.arrayZ);
      врати res;
    }

    auto& v = c.graph.vertices_[dest_obj];
    char* head = (char *) coverage_copy.arrayZ;
    v.set_buffer (head, head + coverage_copy.length);

    hb_free (buffer);
    врати Ok();
  }

  graph_result_t<void> sanitize (const graph_t::vertex_t& vertex) const
  {
    size_t vertex_len = vertex.table_size ();
    ако (unlikely (vertex_len < OT::Layout::Common::Coverage::min_size)) врати Err(SANITIZE_FAILURE);
    hb_barrier ();
    switch (u.format.v)
    {
    case 1: врати graph::sanitize ((const OT::Layout::Common::CoverageFormat1_3<OT::Layout::SmallTypes>*) this, vertex);
    case 2: врати graph::sanitize ((const OT::Layout::Common::CoverageFormat2_4<OT::Layout::SmallTypes>*) this, vertex);
#ifndef HB_NO_BEYOND_64K
    case 3: врати graph::sanitize ((const OT::Layout::Common::CoverageFormat1_3<OT::Layout::MediumTypes>*) this, vertex);
    case 4: врати graph::sanitize ((const OT::Layout::Common::CoverageFormat2_4<OT::Layout::MediumTypes>*) this, vertex);
#endif
    default: врати Err(SANITIZE_FAILURE);
    }
  }
};


}

#endif  // GRAPH_COVERAGE_GRAPH_HH
