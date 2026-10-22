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

#অন্তর্ভাব "gsubgpos-graph.hh"

নামাকাশ graph {

graph_ফলাফল_t<gsubgpos_graph_context_t> gsubgpos_graph_context_t::create (hb_tag_t table_tag_,
                                                                           graph_t& graph_)
{
  gsubgpos_graph_context_t context (table_tag_, graph_);

  যদি (unlikely (table_tag_ != HB_OT_TAG_GPOS
                && table_tag_ != HB_OT_TAG_GSUB))
    ফেরত Err(INVALID_ARGUMENT);

  TRY_ASSIGN (ধ্রুবক GSTAR* gstar, graph::GSTAR::graph_to_gstar (context.graph));
  TRY(gstar->find_lookups (context.graph, context.lookups));
  TRY_ASSIGN (context.lookup_list_সূচক, gstar->get_lookup_list_সূচক (context.graph));

  ফেরত context;
}

gsubgpos_graph_context_t::gsubgpos_graph_context_t (hb_tag_t table_tag_,
                                                    graph_t& graph_)
    : table_tag (table_tag_),
      graph (graph_),
      lookup_list_সূচক (0),
      lookups ()
{}

graph_ফলাফল_t<unsigned> gsubgpos_graph_context_t::create_গ্রন্থি (unsigned আকার_2)
{
  অক্ষর* বাফার = (অক্ষর*) hb_calloc (1, আকার_2);
  যদি (unlikely (!বাফার))
    ফেরত Err(ALLOCATION_FAILURE);

  auto res = add_বাফার (বাফার);
  যদি (unlikely (!res.is_ok ())) {
    // Allocation did not get stored for freeing later.
    hb_free (বাফার);
    ফেরত Err(res.error ());
  }

  ফেরত graph.new_গ্রন্থি (বাফার, বাফার + আকার_2);
}

unsigned gsubgpos_graph_context_t::অঙ্ক_non_ext_subtables ()  {
  unsigned গুনতি = 0;
  জন্য (auto l : lookups.values ())
  {
    যদি (l->is_extension (table_tag)) চালু;
    গুনতি += l->সংখ্যা_of_subtables ();
  }
  ফেরত গুনতি;
}

}
