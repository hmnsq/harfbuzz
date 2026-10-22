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

#समावेश "graph.hh"
#समावेश "../OT/Layout/Common/Coverage.hh"

#अगर_अघोषित GRAPH_COVERAGE_GRAPH_HH
#घोषणा GRAPH_COVERAGE_GRAPH_HH

namespace graph {

ढाँचा <typename Types>
अटल graph_परिणाम_t<व्योम> sanitize (
  स्थिर OT::Layout::Common::CoverageFormat1_3<Types>* thiz,
  स्थिर graph_t::vertex_t& vertex
) {
  माप_प्रकार vertex_len = vertex.table_आकार();
  constexpr अचिन्हित min_आकार = OT::Layout::Common::CoverageFormat1_3<Types>::min_आकार;
  अगर (unlikely (vertex_len < min_आकार)) वापस Err(SANITIZE_FAILURE);
  hb_barrier ();
  अगर (unlikely (vertex_len < min_आकार + thiz->glyphArray.get_आकार () - thiz->glyphArray.len.get_आकार ()))
    वापस Err(SANITIZE_FAILURE);
  वापस Ok();
}

ढाँचा <typename Types>
अटल graph_परिणाम_t<व्योम> sanitize (
  स्थिर OT::Layout::Common::CoverageFormat2_4<Types>* thiz,
  स्थिर graph_t::vertex_t& vertex
) {
  माप_प्रकार vertex_len = vertex.table_आकार();
  constexpr अचिन्हित min_आकार = OT::Layout::Common::CoverageFormat2_4<Types>::min_आकार;
  अगर (unlikely (vertex_len < min_आकार)) वापस Err(SANITIZE_FAILURE);
  hb_barrier ();
  अगर (unlikely (vertex_len < min_आकार + thiz->rangeRecord.get_आकार () - thiz->rangeRecord.len.get_आकार ()))
    वापस Err(SANITIZE_FAILURE);
  वापस Ok();
}

काष्ठा Coverage : खुला OT::Layout::Common::Coverage
{
  अटल graph_परिणाम_t<व्योम> clone_coverage (gsubgpos_graph_context_t& c,
                                              अचिन्हित coverage_id,
                                              अचिन्हित new_parent_id,
                                              अचिन्हित link_position,
                                              अचिन्हित start, अचिन्हित end)

  {
    अचिन्हित coverage_आकार = c.graph.vertices_[coverage_id].table_आकार ();
    स्वतः& coverage_v = c.graph.vertices_[coverage_id];
    स्थिर Coverage* coverage_table = (स्थिर Coverage*) coverage_v.obj().शीर्ष_2;
    अगर (unlikely (!coverage_table))
      वापस Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));

    स्वतः new_coverage =
        + hb_zip (coverage_table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<अचिन्हित, अचिन्हित> p) {
          वापस p.second >= start && p.second < end;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    वापस add_coverage (c, new_parent_id, link_position, new_coverage, coverage_आकार);
  }

  ढाँचा<typename It>
  अटल graph_परिणाम_t<व्योम> add_coverage (gsubgpos_graph_context_t& c,
                                            अचिन्हित parent_id,
                                            अचिन्हित link_position,
                                            It glyphs,
                                            अचिन्हित max_size)
  {
    TRY_ASSIGN (अचिन्हित coverage_अभाज्य_id, c.graph.new_गाँठ (nullptr, nullptr));
    स्वतः& coverage_अभाज्य_vertex = c.graph.vertices_[coverage_अभाज्य_id];
    TRY (make_coverage (c, glyphs, coverage_अभाज्य_id, max_size));

    TRY(c.graph.vertices_[parent_id].add_real_link (SmallTypes::आकार, coverage_अभाज्य_id, link_position));
    TRY(coverage_अभाज्य_vertex.add_parent (parent_id, false));

    वापस Ok();
  }

  // Filter an existing coverage table to glyphs at indices [start, end) and replace it with the filtered version.
  अटल graph_परिणाम_t<व्योम> filter_coverage (gsubgpos_graph_context_t& c,
                                                अचिन्हित existing_coverage,
                                                अचिन्हित start, अचिन्हित end) {
    अचिन्हित coverage_आकार = c.graph.vertices_[existing_coverage].table_आकार ();
    स्वतः& coverage_v = c.graph.vertices_[existing_coverage];
    स्थिर Coverage* coverage_table = (स्थिर Coverage*) coverage_v.obj().शीर्ष_2;
    अगर (unlikely (!coverage_table))
      वापस Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));

    स्वतः new_coverage =
        + hb_zip (coverage_table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<अचिन्हित, अचिन्हित> p) {
          वापस p.second >= start && p.second < end;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    वापस make_coverage (c, new_coverage, existing_coverage, coverage_आकार * 2 + 100);
  }

  // Replace the coverage table at dest obj with one covering 'glyphs'.
  ढाँचा<typename It>
  अटल graph_परिणाम_t<व्योम> make_coverage (gsubgpos_graph_context_t& c,
                                             It glyphs,
                                             अचिन्हित dest_obj,
                                             अचिन्हित max_size)
  {
    अक्षर* बफर = (अक्षर*) hb_calloc (1, max_size);
    अगर (unlikely (!बफर))
      वापस Err(ALLOCATION_FAILURE);

    hb_serialize_context_t serializer (बफर, max_size);
    OT::Layout::Common::Coverage_serialize (&serializer, glyphs);
    serializer.end_serialize ();
    अगर (unlikely (serializer.in_error ()))
    {
      hb_free (बफर);
      वापस Err(ALLOCATION_FAILURE);
    }

    hb_bytes_t coverage_copy = serializer.copy_bytes ();
    अगर (unlikely (!coverage_copy.arrayZ)) {
      hb_free (बफर);
      वापस Err(ALLOCATION_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    स्वतः res = c.add_बफर ((अक्षर *) coverage_copy.arrayZ);
    अगर (unlikely (!res.is_ok ()))
    {
      hb_free (बफर);
      hb_free ((अक्षर *) coverage_copy.arrayZ);
      वापस res;
    }

    स्वतः& v = c.graph.vertices_[dest_obj];
    अक्षर* शीर्ष_2 = (अक्षर *) coverage_copy.arrayZ;
    v.set_बफर (शीर्ष_2, शीर्ष_2 + coverage_copy.लंबाई);

    hb_free (बफर);
    वापस Ok();
  }

  graph_परिणाम_t<व्योम> sanitize (स्थिर graph_t::vertex_t& vertex) स्थिर
  {
    माप_प्रकार vertex_len = vertex.table_आकार ();
    अगर (unlikely (vertex_len < OT::Layout::Common::Coverage::min_आकार)) वापस Err(SANITIZE_FAILURE);
    hb_barrier ();
    चयन (u.format.v)
    {
    हाल 1: वापस graph::sanitize ((स्थिर OT::Layout::Common::CoverageFormat1_3<OT::Layout::SmallTypes>*) यह, vertex);
    हाल 2: वापस graph::sanitize ((स्थिर OT::Layout::Common::CoverageFormat2_4<OT::Layout::SmallTypes>*) यह, vertex);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: वापस graph::sanitize ((स्थिर OT::Layout::Common::CoverageFormat1_3<OT::Layout::MediumTypes>*) यह, vertex);
    हाल 4: वापस graph::sanitize ((स्थिर OT::Layout::Common::CoverageFormat2_4<OT::Layout::MediumTypes>*) यह, vertex);
#पूर्ण_अगर
    शेष: वापस Err(SANITIZE_FAILURE);
    }
  }
};


}

#पूर्ण_अगर  // GRAPH_COVERAGE_GRAPH_HH
