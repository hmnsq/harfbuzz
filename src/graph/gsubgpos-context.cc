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

#समावेश "gsubgpos-graph.hh"

namespace graph {

graph_परिणाम_t<gsubgpos_graph_context_t> gsubgpos_graph_context_t::create (hb_tag_t table_tag_,
                                                                           graph_t& graph_)
{
  gsubgpos_graph_context_t context (table_tag_, graph_);

  अगर (unlikely (table_tag_ != HB_OT_TAG_GPOS
                && table_tag_ != HB_OT_TAG_GSUB))
    वापस Err(INVALID_ARGUMENT);

  TRY_ASSIGN (स्थिर GSTAR* gstar, graph::GSTAR::graph_to_gstar (context.graph));
  TRY(gstar->find_lookups (context.graph, context.lookups));
  TRY_ASSIGN (context.lookup_list_सूचकांक, gstar->get_lookup_list_सूचकांक (context.graph));

  वापस context;
}

gsubgpos_graph_context_t::gsubgpos_graph_context_t (hb_tag_t table_tag_,
                                                    graph_t& graph_)
    : table_tag (table_tag_),
      graph (graph_),
      lookup_list_सूचकांक (0),
      lookups ()
{}

graph_परिणाम_t<अचिन्हित> gsubgpos_graph_context_t::create_गाँठ (अचिन्हित आकार)
{
  अक्षर* बफर = (अक्षर*) hb_calloc (1, आकार);
  अगर (unlikely (!बफर))
    वापस Err(ALLOCATION_FAILURE);

  स्वतः res = add_बफर (बफर);
  अगर (unlikely (!res.is_ok ())) {
    // Allocation did not get stored for freeing later.
    hb_free (बफर);
    वापस Err(res.error ());
  }

  वापस graph.new_गाँठ (बफर, बफर + आकार);
}

अचिन्हित gsubgpos_graph_context_t::अंक_non_ext_subtables ()  {
  अचिन्हित गिनती = 0;
  क्रम (स्वतः l : lookups.values ())
  {
    अगर (l->is_extension (table_tag)) जारी;
    गिनती += l->संख्या_of_subtables ();
  }
  वापस गिनती;
}

}
