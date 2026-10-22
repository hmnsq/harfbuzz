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

#અન્તર્ભાવ "graph.hh"
#અન્તર્ભાવ "../OT/Layout/Common/Coverage.hh"

#ifndef GRAPH_COVERAGE_GRAPH_HH
#define GRAPH_COVERAGE_GRAPH_HH

નામાકાશ graph {

template <typename Types>
સ્થિર graph_result_t<શૂન્ય> sanitize (
  અચલ OT::Layout::Common::CoverageFormat1_3<Types>* thiz,
  અચલ graph_t::vertex_t& vertex
) {
  size_t vertex_len = vertex.table_size();
  constexpr unsigned min_size = OT::Layout::Common::CoverageFormat1_3<Types>::min_size;
  જો (unlikely (vertex_len < min_size)) પાછા Err(SANITIZE_FAILURE);
  hb_barrier ();
  જો (unlikely (vertex_len < min_size + thiz->glyphArray.get_size () - thiz->glyphArray.len.get_size ()))
    પાછા Err(SANITIZE_FAILURE);
  પાછા Ok();
}

template <typename Types>
સ્થિર graph_result_t<શૂન્ય> sanitize (
  અચલ OT::Layout::Common::CoverageFormat2_4<Types>* thiz,
  અચલ graph_t::vertex_t& vertex
) {
  size_t vertex_len = vertex.table_size();
  constexpr unsigned min_size = OT::Layout::Common::CoverageFormat2_4<Types>::min_size;
  જો (unlikely (vertex_len < min_size)) પાછા Err(SANITIZE_FAILURE);
  hb_barrier ();
  જો (unlikely (vertex_len < min_size + thiz->rangeRecord.get_size () - thiz->rangeRecord.len.get_size ()))
    પાછા Err(SANITIZE_FAILURE);
  પાછા Ok();
}

સંરચના Coverage : જાહેર OT::Layout::Common::Coverage
{
  સ્થિર graph_result_t<શૂન્ય> clone_coverage (gsubgpos_graph_context_t& c,
                                              unsigned coverage_id,
                                              unsigned new_parent_id,
                                              unsigned link_position,
                                              unsigned start, unsigned end)

  {
    unsigned coverage_size = c.graph.vertices_[coverage_id].table_size ();
    auto& coverage_v = c.graph.vertices_[coverage_id];
    અચલ Coverage* coverage_table = (અચલ Coverage*) coverage_v.obj().head;
    જો (unlikely (!coverage_table))
      પાછા Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));

    auto new_coverage =
        + hb_zip (coverage_table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          પાછા p.second >= start && p.second < end;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    પાછા add_coverage (c, new_parent_id, link_position, new_coverage, coverage_size);
  }

  template<typename It>
  સ્થિર graph_result_t<શૂન્ય> add_coverage (gsubgpos_graph_context_t& c,
                                            unsigned parent_id,
                                            unsigned link_position,
                                            It glyphs,
                                            unsigned max_size)
  {
    TRY_ASSIGN (unsigned coverage_અવિભાજ્ય_id, c.graph.new_ગાંઠ (nullptr, nullptr));
    auto& coverage_અવિભાજ્ય_vertex = c.graph.vertices_[coverage_અવિભાજ્ય_id];
    TRY (make_coverage (c, glyphs, coverage_અવિભાજ્ય_id, max_size));

    TRY(c.graph.vertices_[parent_id].add_real_link (SmallTypes::size, coverage_અવિભાજ્ય_id, link_position));
    TRY(coverage_અવિભાજ્ય_vertex.add_parent (parent_id, ખોટું));

    પાછા Ok();
  }

  // Filter an existing coverage table to glyphs at indices [start, end) and replace it with the filtered version.
  સ્થિર graph_result_t<શૂન્ય> filter_coverage (gsubgpos_graph_context_t& c,
                                                unsigned existing_coverage,
                                                unsigned start, unsigned end) {
    unsigned coverage_size = c.graph.vertices_[existing_coverage].table_size ();
    auto& coverage_v = c.graph.vertices_[existing_coverage];
    અચલ Coverage* coverage_table = (અચલ Coverage*) coverage_v.obj().head;
    જો (unlikely (!coverage_table))
      પાછા Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));

    auto new_coverage =
        + hb_zip (coverage_table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          પાછા p.second >= start && p.second < end;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    પાછા make_coverage (c, new_coverage, existing_coverage, coverage_size * 2 + 100);
  }

  // Replace the coverage table at dest obj with one covering 'glyphs'.
  template<typename It>
  સ્થિર graph_result_t<શૂન્ય> make_coverage (gsubgpos_graph_context_t& c,
                                             It glyphs,
                                             unsigned dest_obj,
                                             unsigned max_size)
  {
    અક્ષર* buffer = (અક્ષર*) hb_calloc (1, max_size);
    જો (unlikely (!buffer))
      પાછા Err(ALLOCATION_FAILURE);

    hb_serialize_context_t serializer (buffer, max_size);
    OT::Layout::Common::Coverage_serialize (&serializer, glyphs);
    serializer.end_serialize ();
    જો (unlikely (serializer.in_error ()))
    {
      hb_free (buffer);
      પાછા Err(ALLOCATION_FAILURE);
    }

    hb_bytes_t coverage_copy = serializer.copy_bytes ();
    જો (unlikely (!coverage_copy.arrayZ)) {
      hb_free (buffer);
      પાછા Err(ALLOCATION_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    auto res = c.add_buffer ((અક્ષર *) coverage_copy.arrayZ);
    જો (unlikely (!res.is_ok ()))
    {
      hb_free (buffer);
      hb_free ((અક્ષર *) coverage_copy.arrayZ);
      પાછા res;
    }

    auto& v = c.graph.vertices_[dest_obj];
    અક્ષર* head = (અક્ષર *) coverage_copy.arrayZ;
    v.set_buffer (head, head + coverage_copy.length);

    hb_free (buffer);
    પાછા Ok();
  }

  graph_result_t<શૂન્ય> sanitize (અચલ graph_t::vertex_t& vertex) અચલ
  {
    size_t vertex_len = vertex.table_size ();
    જો (unlikely (vertex_len < OT::Layout::Common::Coverage::min_size)) પાછા Err(SANITIZE_FAILURE);
    hb_barrier ();
    પસંદગી (u.format.v)
    {
    સ્થિતિ 1: પાછા graph::sanitize ((અચલ OT::Layout::Common::CoverageFormat1_3<OT::Layout::SmallTypes>*) આ, vertex);
    સ્થિતિ 2: પાછા graph::sanitize ((અચલ OT::Layout::Common::CoverageFormat2_4<OT::Layout::SmallTypes>*) આ, vertex);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: પાછા graph::sanitize ((અચલ OT::Layout::Common::CoverageFormat1_3<OT::Layout::MediumTypes>*) આ, vertex);
    સ્થિતિ 4: પાછા graph::sanitize ((અચલ OT::Layout::Common::CoverageFormat2_4<OT::Layout::MediumTypes>*) આ, vertex);
#endif
    મૂળભૂત: પાછા Err(SANITIZE_FAILURE);
    }
  }
};


}

#endif  // GRAPH_COVERAGE_GRAPH_HH
