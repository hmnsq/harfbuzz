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
#समावेश "../hb-ot-layout-gsubgpos.hh"

#अगर_अघोषित GRAPH_GSUBGPOS_CONTEXT_HH
#घोषणा GRAPH_GSUBGPOS_CONTEXT_HH

namespace graph {

काष्ठा Lookup;

काष्ठा gsubgpos_graph_context_t
{
  HB_INTERNAL अटल graph_परिणाम_t<gsubgpos_graph_context_t> create (hb_tag_t table_tag_,
                                                                      graph_t& graph_);

  hb_tag_t table_tag;
  graph_t& graph;
  अचिन्हित lookup_list_सूचकांक;
  hb_hashmap_t<अचिन्हित, graph::Lookup*> lookups;
  hb_hashmap_t<अचिन्हित, अचिन्हित> subtable_to_extension;
  hb_hashmap_t<अचिन्हित, hb_vector_t<अचिन्हित>> split_subtables;


  HB_INTERNAL graph_परिणाम_t<अचिन्हित> create_गाँठ (अचिन्हित आकार);

  graph_परिणाम_t<व्योम> add_बफर (अक्षर* बफर)
  {
    वापस graph.add_बफर (बफर);
  }

 निजी:
  HB_INTERNAL gsubgpos_graph_context_t (hb_tag_t table_tag_,
                                        graph_t& graph_);

  HB_INTERNAL अचिन्हित अंक_non_ext_subtables ();
};

}

#पूर्ण_अगर  // GRAPH_GSUBGPOS_CONTEXT
