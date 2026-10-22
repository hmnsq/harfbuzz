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

#അന്തര്ഭാവ "graph.hh"
#അന്തര്ഭാവ "../hb-ot-layout-gsubgpos.hh"

#ifndef GRAPH_GSUBGPOS_CONTEXT_HH
#define GRAPH_GSUBGPOS_CONTEXT_HH

നാമാകാശ graph {

ഘടന Lookup;

ഘടന gsubgpos_graph_context_t
{
  HB_INTERNAL സ്ഥിതം graph_result_t<gsubgpos_graph_context_t> create (hb_tag_t table_tag_,
                                                                      graph_t& graph_);

  hb_tag_t table_tag;
  graph_t& graph;
  unsigned lookup_list_index;
  hb_hashmap_t<unsigned, graph::Lookup*> lookups;
  hb_hashmap_t<unsigned, unsigned> subtable_to_extension;
  hb_hashmap_t<unsigned, hb_vector_t<unsigned>> split_subtables;


  HB_INTERNAL graph_result_t<unsigned> create_കണ്ണി (unsigned size);

  graph_result_t<ശൂന്യം> add_buffer (അക്ഷരം* buffer)
  {
    തിരികെ graph.add_buffer (buffer);
  }

 സ്വകാര്യ:
  HB_INTERNAL gsubgpos_graph_context_t (hb_tag_t table_tag_,
                                        graph_t& graph_);

  HB_INTERNAL unsigned അക്കം_non_ext_subtables ();
};

}

#endif  // GRAPH_GSUBGPOS_CONTEXT
