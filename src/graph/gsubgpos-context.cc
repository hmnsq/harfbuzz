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

#अन्तर्भाव "gsubgpos-graph.hh"

नामाकाश graph {

graph_result_t<gsubgpos_graph_context_t> gsubgpos_graph_context_t::create (hb_tag_t table_tag_,
                                                                           graph_t& graph_)
{
  gsubgpos_graph_context_t context (table_tag_, graph_);

  यदि (unlikely (table_tag_ != HB_OT_TAG_GPOS
                && table_tag_ != HB_OT_TAG_GSUB))
    फर्काऊ Err(INVALID_ARGUMENT);

  TRY_ASSIGN (अचर GSTAR* gstar, graph::GSTAR::graph_to_gstar (context.graph));
  TRY(gstar->find_lookups (context.graph, context.lookups));
  TRY_ASSIGN (context.lookup_list_index, gstar->get_lookup_list_index (context.graph));

  फर्काऊ context;
}

gsubgpos_graph_context_t::gsubgpos_graph_context_t (hb_tag_t table_tag_,
                                                    graph_t& graph_)
    : table_tag (table_tag_),
      graph (graph_),
      lookup_list_index (0),
      lookups ()
{}

graph_result_t<अचिन्हित> gsubgpos_graph_context_t::create_गाँठो (अचिन्हित size)
{
  वर्ण* buffer = (वर्ण*) hb_calloc (1, size);
  यदि (unlikely (!buffer))
    फर्काऊ Err(ALLOCATION_FAILURE);

  स्वतः res = add_buffer (buffer);
  यदि (unlikely (!res.is_ok ())) {
    // Allocation did not get stored for freeing later.
    hb_free (buffer);
    फर्काऊ Err(res.error ());
  }

  फर्काऊ graph.new_गाँठो (buffer, buffer + size);
}

अचिन्हित gsubgpos_graph_context_t::अङ्क_non_ext_subtables ()  {
  अचिन्हित गन्ती = 0;
  लागि (स्वतः l : lookups.values ())
  {
    यदि (l->is_extension (table_tag)) जारी;
    गन्ती += l->सङ्ख्या_of_subtables ();
  }
  फर्काऊ गन्ती;
}

}
