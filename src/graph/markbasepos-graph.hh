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

#ifndef GRAPH_MARKBASEPOS_GRAPH_HH
#define GRAPH_MARKBASEPOS_GRAPH_HH

#include "split-helpers.hh"
#include "coverage-graph.hh"
#include "../OT/Layout/GPOS/MarkBasePos.hh"
#include "../OT/Layout/GPOS/PosLookupSubTable.hh"

namespace graph {

sa_nracanaa AnchorMatrix : public OT::Layout::GPOS_impl::AnchorMatrix<SmallTypes>
{
  graph_result_t<void> sanitize (const graph_t::vertex_t& vertex, unsigned class_count) const
  {
    size_t vertex_len = vertex.table_size();
    གལ་སྲིད (unlikely (vertex_len < AnchorMatrix::min_size)) ལོག Err(SANITIZE_FAILURE);
    hb_barrier ();

    གལ་སྲིད (unlikely (vertex_len < AnchorMatrix::min_size +
                  OT::Offset16::static_size * class_count * this->rows))
      ལོག Err(SANITIZE_FAILURE);
    ལོག Ok();
  }

  graph_result_t<void> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned old_class_count,
                               unsigned new_class_count)
  {
    གལ་སྲིད (unlikely (new_class_count >= old_class_count)) ལོག Err(INVALID_ARGUMENT);
    auto& v = c.graph.vertices_[this_index];
    const auto& o = v.obj ();
    unsigned base_count = rows;
    v.set_buffer (o.head,
      o.head +
      AnchorMatrix::min_size +
      OT::Offset16::static_size * base_count * new_class_count);

    // Reposition links into the new indexing scheme.
    དོན་ལུ (auto& link : v.real_links_writer ())
    {
      unsigned index = (link.position - 2) / 2;
      unsigned base = index / old_class_count;
      unsigned klass = index % old_class_count;
      གལ་སྲིད (unlikely (klass >= new_class_count))
        // should have already been removed
        ལོག Err(INVALID_ARGUMENT);

      unsigned new_index = base * new_class_count + klass;

      link.position = (var_na*) &(this->matrixZ[new_index]) - (var_na*) this;
    }

    ལོག Ok();
  }

  graph_result_t<unsigned> clone (gsubgpos_graph_context_t& c,
                                  unsigned this_index,
                                  unsigned start,
                                  unsigned end,
                                  unsigned class_count)
  {
    unsigned base_count = rows;
    unsigned new_class_count = end - start;
    unsigned size = AnchorMatrix::min_size +
                    OT::Offset16::static_size * new_class_count * rows;
    TRY_ASSIGN (unsigned prime_id, c.create_node (size));
    AnchorMatrix* prime = (AnchorMatrix*) c.graph.object (prime_id).head;
    prime->rows = base_count;

    auto& v = c.graph.vertices_[this_index];
    const auto& o = v.obj ();

    ཧྲིལ num_links = o.real_links.length;
    དོན་ལུ (ཧྲིལ i = 0; i < num_links; i++)
    {
      const auto& link = o.real_links[i];
      unsigned old_index = (link.position - 2) / OT::Offset16::static_size;
      unsigned klass = old_index % class_count;
      གལ་སྲིད (klass < start || klass >= end) continue;

      unsigned base = old_index / class_count;
      unsigned new_klass = klass - start;
      unsigned new_index = base * new_class_count + new_klass;


      unsigned child_idx = link.objidx;
      TRY (c.graph.add_link (&(prime->matrixZ[new_index]),
                             prime_id,
                             child_idx));

      auto& child = c.graph.vertices_[child_idx];
      child.remove_parent (this_index);

      v.remove_real_link_unordered (i);
      num_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.sort_real_links ();

    ལོག Ok(prime_id);
  }
};

sa_nracanaa MarkArray : public OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  graph_result_t<void> sanitize (const graph_t::vertex_t& vertex) const
  {
    size_t vertex_len = vertex.table_size();
    unsigned min_size = MarkArray::min_size;
    གལ་སྲིད (unlikely (vertex_len < min_size)) ལོག Err(SANITIZE_FAILURE);
    hb_barrier ();

    གལ་སྲིད (unlikely (vertex_len < get_size ())) ལོག Err(SANITIZE_FAILURE);
    ལོག Ok();
  }

  graph_result_t<void> shrink (gsubgpos_graph_context_t& c,
                               const hb_hashmap_t<unsigned, unsigned>& mark_array_links,
                               unsigned this_index,
                               unsigned new_class_count)
  {
    auto& v = c.graph.vertices_[this_index];
    const auto& o = v.obj ();
    དོན་ལུ (const auto& link : o.real_links)
      c.graph.vertices_[link.objidx].remove_parent (this_index);
    v.clear_real_links ();

    unsigned new_index = 0;
    དོན་ལུ (const auto& record : this->iter ())
    {
      unsigned klass = record.klass;
      གལ་སྲིད (klass >= new_class_count) continue;

      (*this)[new_index].klass = klass;
      unsigned position = (var_na*) &record.markAnchor - (var_na*) this;
      unsigned* objidx;
      གལ་སྲིད (!mark_array_links.has (position, &objidx))
      {
        new_index++;
        continue;
      }

      TRY (c.graph.add_link (&(*this)[new_index].markAnchor, this_index, *objidx));
      new_index++;
    }

    this->len = new_index;
    v.set_buffer (o.head, o.head + MarkArray::min_size +
                  OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * new_index);

    ལོག Ok();
  }

  graph_result_t<unsigned> clone (gsubgpos_graph_context_t& c,
                                  unsigned this_index,
                                  const hb_hashmap_t<unsigned, unsigned>& pos_to_index,
                                  hb_set_t& marks,
                                  unsigned start_class)
  {
    unsigned size = MarkArray::min_size +
                    OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size *
                    marks.get_population ();
    TRY_ASSIGN (unsigned prime_id, c.create_node (size));
    MarkArray* prime = (MarkArray*) c.graph.object (prime_id).head;
    prime->len = marks.get_population ();


    unsigned i = 0;
    དོན་ལུ (hb_codepoint_t mark : marks)
    {
      (*prime)[i].klass = (*this)[mark].klass - start_class;
      unsigned offset_pos = (var_na*) &((*this)[mark].markAnchor) - (var_na*) this;
      unsigned* anchor_index;
      གལ་སྲིད (pos_to_index.has (offset_pos, &anchor_index))
        TRY (c.graph.move_child (this_index,
                                 &((*this)[mark].markAnchor),
                                 prime_id,
                                 &((*prime)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    ལོག Ok(prime_id);
  }
};

sa_nracanaa MarkBasePosFormat1 : public OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>
{
  graph_result_t<void> sanitize (const graph_t::vertex_t& vertex) const
  {
    size_t vertex_len = vertex.table_size ();
    གལ་སྲིད (unlikely (vertex_len < MarkBasePosFormat1::static_size))
      ལོག Err(SANITIZE_FAILURE);
    ལོག Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (const unsigned base_coverage_id, c.graph.index_for_offset (this_index, &baseCoverage));

    const unsigned base_size =
        OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::min_size +
        MarkArray::min_size +
        AnchorMatrix::min_size +
        c.graph.vertices_[base_coverage_id].table_size ();

    TRY_ASSIGN (hb_vector_t<class_info_t> class_to_info, get_class_info (c, this_index));

    unsigned class_count = classCount;
    TRY_ASSIGN (auto base_array, c.graph.as_table<AnchorMatrix> (this_index,
                                                          &baseArray,
                                                          class_count));
    unsigned base_count = base_array.table->rows;

    unsigned partial_coverage_size = 4;
    unsigned accumulated = base_size;
    hb_vector_t<unsigned> split_points;

    དོན་ལུ (unsigned klass = 0; klass < class_count; klass++)
    {
      class_info_t& info = class_to_info[klass];
      partial_coverage_size += OT::HBUINT16::static_size * info.marks.get_population ();
      unsigned accumulated_delta =
          OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * info.marks.get_population () +
          OT::Offset16::static_size * base_count;

      དོན་ལུ (unsigned objidx : info.child_indices)
        accumulated_delta += c.graph.find_subgraph_size (objidx, visited).value_or (0);

      accumulated += accumulated_delta;
      unsigned total = accumulated + partial_coverage_size;

      གལ་སྲིད (total >= (1 << 16))
      {
        split_points.push (klass);
        TRY (graph_result_t<void>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 4 + OT::HBUINT16::static_size * info.marks.get_population ();
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }


    TRY_ASSIGN (const unsigned mark_array_id, c.graph.index_for_offset (this_index, &markArray));
    TRY_ASSIGN (auto mark_array_links, c.graph.vertices_[mark_array_id].position_to_index_map ());

    split_context_t split_context {
      c,
      this,
      this_index,
      std::move (class_to_info),
      std::move (mark_array_links),
    };

    ལོག actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 private:

  sa_nracanaa class_info_t {
    hb_set_t marks;
    hb_vector_t<unsigned> child_indices;
  };

  sa_nracanaa split_context_t {
    gsubgpos_graph_context_t& c;
    MarkBasePosFormat1* thiz;
    unsigned this_index;
    hb_vector_t<class_info_t> class_to_info;
    hb_hashmap_t<unsigned, unsigned> mark_array_links;

    hb_set_t marks_for (unsigned start, unsigned end)
    {
      hb_set_t marks;
      དོན་ལུ (unsigned klass = start; klass < end; klass++)
      {
        + class_to_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      ལོག marks;
    }

    unsigned original_count ()
    {
      ལོག thiz->classCount;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      ལོག thiz->clone_range (*this, this->this_index, start, end);
    }

    graph_result_t<void> shrink (unsigned count)
    {
      ལོག thiz->shrink (*this, this->this_index, count);
    }
  };

  graph_result_t<hb_vector_t<class_info_t>> get_class_info (gsubgpos_graph_context_t& c,
                                                            unsigned this_index)
  {
    hb_vector_t<class_info_t> class_to_info;

    unsigned class_count = classCount;
    གལ་སྲིད (!class_count) ལོག class_to_info;

    གལ་སྲིད (unlikely (!class_to_info.resize (class_count)))
      ལོག Err(ALLOCATION_FAILURE);

    TRY_ASSIGN (auto mark_array, c.graph.as_table<MarkArray> (this_index, &markArray));

    unsigned mark_count = mark_array.table->len;
    དོན་ལུ (unsigned mark = 0; mark < mark_count; mark++)
    {
      unsigned klass = (*mark_array.table)[mark].get_class ();
      གལ་སྲིད (klass >= class_count) continue;
      class_to_info[klass].marks.add (mark);
    }

    དོན་ལུ (const auto& link : mark_array.vertex->obj ().real_links)
    {
      unsigned mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size;
      unsigned klass = (*mark_array.table)[mark].get_class ();
      གལ་སྲིད (klass >= class_count) continue;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    TRY_ASSIGN (unsigned base_array_id,
         c.graph.index_for_offset (this_index, &baseArray));

    auto& base_array_v = c.graph.vertices_[base_array_id];

    དོན་ལུ (const auto& link : base_array_v.obj ().real_links)
    {
      unsigned index = (link.position - 2) / OT::Offset16::static_size;
      unsigned klass = index % class_count;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    ལོག class_to_info;
  }

  graph_result_t<void> shrink (split_context_t& sc,
                               unsigned this_index,
                               unsigned count)
  {
    DEBUG_MSG (SUBSET_REPACK, nullptr,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_index,
               count);

    unsigned old_count = classCount;
    གལ་སྲིད (count >= old_count)
      ལོག Ok();

    classCount = count;

    TRY_ASSIGN (auto mark_coverage, sc.c.graph.as_mutable_table<Coverage> (this_index,
                                                                    &markCoverage));

    hb_set_t marks = sc.marks_for (0, count);
    auto new_coverage =
        + hb_enumerate (mark_coverage.table->iter ())
        | hb_filter (marks, hb_first)
        | hb_map_retains_sorting (hb_second)
        ;
    TRY (Coverage::make_coverage (sc.c, + new_coverage,
                                  mark_coverage.index,
                                  4 + 2 * marks.get_population ()));


    TRY_ASSIGN (auto base_array, sc.c.graph.as_mutable_table<AnchorMatrix> (this_index,
                                                                     &baseArray,
                                                                     old_count));

    TRY (base_array.table->shrink (sc.c,
                                   base_array.index,
                                   old_count,
                                   count));

    TRY_ASSIGN (auto mark_array, sc.c.graph.as_mutable_table<MarkArray> (this_index,
                                                                  &markArray));

    TRY (mark_array.table->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.index,
                                   count));

    ལོག Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  graph_result_t<unsigned> clone_range (split_context_t& sc,
                                        unsigned this_index,
                                        unsigned start, unsigned end) const
  {
    DEBUG_MSG (SUBSET_REPACK, nullptr,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_index, start, end);

    graph_t& graph = sc.c.graph;
    unsigned prime_size = OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::static_size;

    TRY_ASSIGN (unsigned prime_id, sc.c.create_node (prime_size));

    MarkBasePosFormat1* prime = (MarkBasePosFormat1*) graph.object (prime_id).head;
    prime->format = this->format;
    unsigned new_class_count = end - start;
    prime->classCount = new_class_count;

    TRY_ASSIGN (unsigned base_coverage_id,
         graph.index_for_offset (sc.this_index, &baseCoverage));

    TRY (graph.add_link (&(prime->baseCoverage), prime_id, base_coverage_id));

    TRY_ASSIGN (auto mark_coverage, sc.c.graph.as_table<Coverage> (this_index,
                                                            &markCoverage));

    hb_set_t marks = sc.marks_for (start, end);
    auto new_coverage =
        + hb_enumerate (mark_coverage.table->iter ())
        | hb_filter (marks, hb_first)
        | hb_map_retains_sorting (hb_second)
        ;
    TRY (Coverage::add_coverage (sc.c,
                                 prime_id,
                                 2,
                                 + new_coverage,
                                 marks.get_population () * 2 + 4));

    TRY_ASSIGN (auto mark_array,
         graph.as_mutable_table <MarkArray> (sc.this_index, &markArray));

    TRY_ASSIGN (unsigned new_mark_array,
         mark_array.table->clone (sc.c,
                                  mark_array.index,
                                  sc.mark_array_links,
                                  marks,
                                  start));
    TRY (graph.add_link (&(prime->markArray), prime_id, new_mark_array));

    unsigned class_count = classCount;
    TRY_ASSIGN (auto base_array,
         graph.as_mutable_table<AnchorMatrix> (sc.this_index, &baseArray, class_count));

    TRY_ASSIGN (unsigned new_base_array,
         base_array.table->clone (sc.c,
                                  base_array.index,
                                  start, end, this->classCount));
    TRY (graph.add_link (&(prime->baseArray), prime_id, new_base_array));

    ལོག Ok(prime_id);
  }
};


sa_nracanaa MarkBasePos : public OT::Layout::GPOS_impl::MarkBasePos
{
  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    switch (u.format.v) {
    pak_sa 1:
      hb_barrier ();
      ལོག ((MarkBasePosFormat1*)(&u.format1))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    pak_sa 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
#endif
    default:
      ལོག Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<void> sanitize (const graph_t::vertex_t& vertex) const
  {
    size_t vertex_len = vertex.table_size ();
    གལ་སྲིད (unlikely (vertex_len < u.format.v.get_size ())) ལོག Err(SANITIZE_FAILURE);
    hb_barrier ();

    switch (u.format.v) {
    pak_sa 1:
      hb_barrier ();
      ལོག ((MarkBasePosFormat1*)(&u.format1))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    pak_sa 2: HB_FALLTHROUGH;
#endif
    default:
      // We don't handle format 3 and 4 here.
      ལོག Err(SANITIZE_FAILURE);
    }
  }
};


}

#endif  // GRAPH_MARKBASEPOS_GRAPH_HH
