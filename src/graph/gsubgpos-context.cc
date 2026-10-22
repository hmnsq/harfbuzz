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

#شامل "gsubgpos-graph.hh"

فضاینام graph {

graph_result_t<gsubgpos_graph_context_t> gsubgpos_graph_context_t::create (hb_tag_t table_tag_,
                                                                           graph_t& graph_)
{
  gsubgpos_graph_context_t context (table_tag_, graph_);

  اگر (unlikely (table_tag_ != HB_OT_TAG_GPOS
                && table_tag_ != HB_OT_TAG_GSUB))
    بازگشت Err(INVALID_ARGUMENT);

  TRY_ASSIGN (ثابت GSTAR* gstar, graph::GSTAR::graph_to_gstar (context.graph));
  TRY(gstar->find_lookups (context.graph, context.lookups));
  TRY_ASSIGN (context.lookup_list_index, gstar->get_lookup_list_index (context.graph));

  بازگشت context;
}

gsubgpos_graph_context_t::gsubgpos_graph_context_t (hb_tag_t table_tag_,
                                                    graph_t& graph_)
    : table_tag (table_tag_),
      graph (graph_),
      lookup_list_index (0),
      lookups ()
{}

graph_result_t<unsigned> gsubgpos_graph_context_t::create_گره (unsigned size)
{
  نویسه* buffer = (نویسه*) hb_calloc (1, size);
  اگر (unlikely (!buffer))
    بازگشت Err(ALLOCATION_FAILURE);

  auto res = add_buffer (buffer);
  اگر (unlikely (!res.is_ok ())) {
    // Allocation did not get stored for freeing later.
    hb_free (buffer);
    بازگشت Err(res.error ());
  }

  بازگشت graph.new_گره (buffer, buffer + size);
}

unsigned gsubgpos_graph_context_t::رقم_non_ext_subtables ()  {
  unsigned شمار = 0;
  برای (auto l : lookups.values ())
  {
    اگر (l->is_extension (table_tag)) ادامه;
    شمار += l->عدد_of_subtables ();
  }
  بازگشت شمار;
}

}
