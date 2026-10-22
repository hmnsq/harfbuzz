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

#ଅନ୍ତର୍ଭାଵ "graph.hh"
#ଅନ୍ତର୍ଭାଵ "../hb-ot-layout-gsubgpos.hh"
#ଅନ୍ତର୍ଭାଵ "../OT/Layout/GSUB/ExtensionSubst.hh"
#ଅନ୍ତର୍ଭାଵ "../OT/Layout/GSUB/SubstLookupSubTable.hh"
#ଅନ୍ତର୍ଭାଵ "gsubgpos-context.hh"
#ଅନ୍ତର୍ଭାଵ "pairpos-graph.hh"
#ଅନ୍ତର୍ଭାଵ "markbasepos-graph.hh"
#ଅନ୍ତର୍ଭାଵ "ligature-graph.hh"

#ifndef GRAPH_GSUBGPOS_GRAPH_HH
#define GRAPH_GSUBGPOS_GRAPH_HH

ନାମାକାଶ graph {

ସଂରଚନା Lookup;

template<typename T>
ସଂରଚନା ExtensionFormat1 : ସର୍ବସାଧାରଣ OT::ExtensionFormat1<T>
{
  ଶୂନ୍ୟ reset(unsigned type)
  {
    ଏହି->format = 1;
    ଏହି->extensionLookupType = type;
    ଏହି->extensionOffset = 0;
  }

  graph_result_t<ଶୂନ୍ୟ> sanitize (ଅଚଳ graph_t::vertex_t& vertex) ଅଚଳ
  {
    size_t vertex_len = vertex.table_size ();
    ଯଦି (unlikely (vertex_len < OT::ExtensionFormat1<T>::static_size))
      ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    ଫେରନ୍ତୁ Ok();
  }

  unsigned get_lookup_type () ଅଚଳ
  {
    ଫେରନ୍ତୁ ଏହି->extensionLookupType;
  }

  graph_result_t<unsigned> get_subtable_index (graph_t& graph, unsigned this_index) ଅଚଳ
  {
    ଫେରନ୍ତୁ graph.index_for_offset (this_index, &ଏହି->extensionOffset);
  }
};

ସଂରଚନା Lookup : ସର୍ବସାଧାରଣ OT::Lookup
{
  unsigned ସଂଖ୍ୟା_of_subtables () ଅଚଳ
  {
    ଫେରନ୍ତୁ subTable.len;
  }

  graph_result_t<ଶୂନ୍ୟ> sanitize (ଅଚଳ graph_t::vertex_t& vertex) ଅଚଳ
  {
    size_t vertex_len = vertex.table_size ();
    ଯଦି (unlikely (vertex_len < OT::Lookup::min_size)) ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    hb_barrier ();
    ଯଦି (unlikely (vertex_len < ଏହି->get_size ())) ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    ଫେରନ୍ତୁ Ok();
  }

  ତାର୍କିକ is_extension (hb_tag_t table_tag) ଅଚଳ
  {
    ଫେରନ୍ତୁ lookupType == extension_type (table_tag);
  }

  ତାର୍କିକ use_mark_filtering_set () ଅଚଳ
  {
    unsigned flag = lookupFlag;
    ଫେରନ୍ତୁ flag & 0x0010u;
  }

  graph_result_t<ଶୂନ୍ୟ> make_extension (gsubgpos_graph_context_t& c,
                                       unsigned this_index)
  {
    unsigned type = lookupType;
    unsigned ext_type = extension_type (c.table_tag);
    ଯଦି (!ext_type || is_extension (c.table_tag))
    {
      // NOOP
      ଫେରନ୍ତୁ Ok();
    }

    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_index);

    hb_set_t seen;
    hb_vector_t<unsigned> subtables;
    ପାଇଁ (unsigned i = 0; i < subTable.len; i++)
    {
      TRY_ASSIGN (unsigned subtable_index, c.graph.index_for_offset (this_index, &subTable[i]));
      ଯଦି (seen.has (subtable_index))
        ଜାରି;
      seen.add (subtable_index);
      subtables.push (subtable_index);
    }
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (seen, ALLOCATION_FAILURE));
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (subtables, ALLOCATION_FAILURE));

    ପାଇଁ (unsigned subtable_index : subtables)
      TRY (make_subtable_extension (c, this_index, subtable_index));

    lookupType = ext_type;
    ଫେରନ୍ତୁ Ok();
  }

  graph_result_t<ଶୂନ୍ୟ> split_subtables_if_needed (gsubgpos_graph_context_t& c,
                                                  unsigned this_index)
  {
    unsigned type = lookupType;
    ତାର୍କିକ is_ext = is_extension (c.table_tag);

    ଯଦି (c.table_tag != HB_OT_TAG_GPOS && c.table_tag != HB_OT_TAG_GSUB)
      ଫେରନ୍ତୁ Ok();

    ଯଦି (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      ଫେରନ୍ତୁ Ok();

    hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>> all_new_subtables;
    ପାଇଁ (unsigned i = 0; i < subTable.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      auto idx_res = c.graph.index_for_offset (this_index, &subTable[i]);
      ଯଦି (!idx_res.is_ok ()) ଜାରି;
      unsigned subtable_index = *idx_res;
      ଯଦି (is_ext) {
        unsigned ext_subtable_index = subtable_index;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.graph.object (ext_subtable_index).head;
        ଯଦି (!extension || !extension->sanitize (c.graph.vertices_[ext_subtable_index]).is_ok ())
          ଜାରି;

        TRY_ASSIGN (subtable_index, extension->get_subtable_index (c.graph, ext_subtable_index));
        type = extension->get_lookup_type ();
        ଯଦି (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          ଜାରି;
      }

      hb_vector_t<unsigned>* split_result;
      ଯଦି (c.split_subtables.has (subtable_index, &split_result))
      {
        ଯଦି (split_result->length == 0)
          ଜାରି;
        all_new_subtables.push (hb_pair(i, *split_result));
        TRY (graph_result_t<ଶୂନ୍ୟ>::from (all_new_subtables, ALLOCATION_FAILURE));
      }
      ଅନ୍ୟଥା
      {
        hb_vector_t<unsigned> new_sub_tables;

        ଯଦି (c.table_tag == HB_OT_TAG_GPOS) {
          ବାଛ (type)
          {
          ସ୍ଥିତି 2:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<PairPos> (c, subtable_index));
              ଭାଙ୍ଗ;
            }
          ସ୍ଥିତି 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<MarkBasePos> (c, subtable_index));
              ଭାଙ୍ଗ;
            }
          ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ଭାଙ୍ଗ;
          }
        } ଅନ୍ୟଥା ଯଦି (c.table_tag == HB_OT_TAG_GSUB) {
          ବାଛ (type)
          {
          ସ୍ଥିତି 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<graph::LigatureSubst> (c, subtable_index));
              ଭାଙ୍ଗ;
            }
          ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ଭାଙ୍ଗ;
          }
        }

        c.split_subtables.set (subtable_index, new_sub_tables);
        TRY (graph_result_t<ଶୂନ୍ୟ>::from (c.split_subtables, ALLOCATION_FAILURE));
        ଯଦି (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (graph_result_t<ଶୂନ୍ୟ>::from (all_new_subtables, ALLOCATION_FAILURE));
        }
      }
    }

    ଯଦି (all_new_subtables) {
      TRY (add_sub_tables (c, this_index, type, all_new_subtables));
    }

    ଫେରନ୍ତୁ Ok();
  }

  template<typename T>
  graph_result_t<hb_vector_t<unsigned>> split_subtable (gsubgpos_graph_context_t& c,
                                                        unsigned objidx)
  {
    T* sub_table = (T*) c.graph.object (objidx).head;
    ଯଦି (!sub_table || !sub_table->sanitize (c.graph.vertices_[objidx]).is_ok ())
      ଫେରନ୍ତୁ Ok(hb_vector_t<unsigned> ());

    ଫେରନ୍ତୁ sub_table->split_subtables (c, objidx);
  }

  graph_result_t<ଶୂନ୍ୟ> add_sub_tables (gsubgpos_graph_context_t& c,
                                       unsigned this_index,
                                       unsigned type,
                                       ଅଚଳ hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    ତାର୍କିକ is_ext = is_extension (c.table_tag);
    auto* v = &c.graph.vertices_[this_index];
    fix_existing_subtable_links (c, this_index, subtable_ids);

    unsigned new_subtable_ଗଣନା = 0;
    ପାଇଁ (ଅଚଳ auto& p : subtable_ids)
      new_subtable_ଗଣନା += p.second.length;

    size_t new_size = v->table_size ()
                      + new_subtable_ଗଣନା * OT::Offset16::static_size;
    ଅକ୍ଷର* buffer = (ଅକ୍ଷର*) hb_calloc (1, new_size);
    ଯଦି (unlikely (!buffer)) ଫେରନ୍ତୁ Err(ALLOCATION_FAILURE);
    auto res = c.add_buffer (buffer);
    ଯଦି (unlikely (!res.is_ok ()))
    {
      hb_free (buffer);
      ଫେରନ୍ତୁ res;
    }
    hb_memcpy (buffer, v->obj ().head, v->table_size());

    ଯଦି (use_mark_filtering_set ())
      hb_memcpy (buffer + new_size - 2, v->obj ().tail - 2, 2);

    v->set_buffer (buffer, buffer + new_size);

    Lookup* new_lookup = (Lookup*) buffer;

    unsigned shift = 0;
    new_lookup->subTable.len = subTable.len + new_subtable_ଗଣନା;
    ପାଇଁ (ଅଚଳ auto& p : subtable_ids)
    {
      unsigned offset_index = p.first + shift + 1;
      shift += p.second.length;

      ପାଇଁ (unsigned subtable_id : p.second)
      {
        ଯଦି (is_ext)
        {
          TRY_ASSIGN (unsigned ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.graph.vertices_[subtable_id].add_parent (ext_id, ମିଥ୍ୟା));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.graph.vertices_[this_index];
        }

        TRY(v->add_real_link(2, subtable_id,
          (ଅକ୍ଷର*) &new_lookup->subTable[offset_index++] - (ଅକ୍ଷର*) new_lookup));
        TRY(c.graph.vertices_[subtable_id].add_parent (this_index, ମିଥ୍ୟା));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->sort_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.set (this_index, new_lookup);
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (c.lookups, ALLOCATION_FAILURE));
    ଫେରନ୍ତୁ Ok();
  }

  ଶୂନ୍ୟ fix_existing_subtable_links (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    ଅଚଳ hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    auto& v = c.graph.vertices_[this_index];
    unsigned shift = 0;
    ପାଇଁ (ଅଚଳ auto& p : subtable_ids)
    {
      unsigned ଭର୍ତ୍ତି_index = p.first + shift;
      unsigned pos_offset = p.second.length * OT::Offset16::static_size;
      unsigned ଭର୍ତ୍ତି_offset = Lookup::min_size + ଭର୍ତ୍ତି_index * OT::Offset16::static_size;
      shift += p.second.length;

      ପାଇଁ (auto& l : v.all_links_writer ())
      {
        ଯଦି (l.position > ଭର୍ତ୍ତି_offset) l.position += pos_offset;
      }
    }
  }

  graph_result_t<unsigned> create_extension_subtable (gsubgpos_graph_context_t& c,
                                                      unsigned subtable_index,
                                                      unsigned type)
  {
    unsigned extension_size = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_size;

    TRY_ASSIGN (unsigned ext_index, c.create_ଗଣ୍ଠି (extension_size));

    auto& ext_vertex = c.graph.vertices_[ext_index];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_vertex.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_vertex.add_real_link (4, subtable_index, 4));

    ଫେରନ୍ତୁ Ok(ext_index);
  }

  graph_result_t<ଶୂନ୍ୟ> make_subtable_extension (gsubgpos_graph_context_t& c,
                                                unsigned lookup_index,
                                                unsigned subtable_index)
  {
    unsigned type = lookupType;
    unsigned ext_index = HB_GRAPH_INVALID;
    unsigned* existing_ext_index = nullptr;
    ଯଦି (c.subtable_to_extension.has(subtable_index, &existing_ext_index)) {
      ext_index = *existing_ext_index;
    } ଅନ୍ୟଥା {
      TRY_ASSIGN (ext_index, create_extension_subtable(c, subtable_index, type));
      c.subtable_to_extension.set(subtable_index, ext_index);
      TRY (graph_result_t<ଶୂନ୍ୟ>::from (c.subtable_to_extension, ALLOCATION_FAILURE));
    }

    auto& subtable_vertex = c.graph.vertices_[subtable_index];
    auto& lookup_vertex = c.graph.vertices_[lookup_index];
    unsigned replaced = 0;
    ପାଇଁ (auto& l : lookup_vertex.real_links_writer ())
    {
      ଯଦି (l.objidx == subtable_index) {
        // Change lookup to point at the extension.
        l.objidx = ext_index;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    auto& ext_vertex = c.graph.vertices_[ext_index];
    ପାଇଁ (unsigned i = 0; i < replaced; i++)
    {
      subtable_vertex.remove_parent (lookup_index);
      TRY(ext_vertex.add_parent (lookup_index, ମିଥ୍ୟା));
    }
    ଯଦି (!existing_ext_index)
      TRY(subtable_vertex.add_parent (ext_index, ମିଥ୍ୟା));

    ଫେରନ୍ତୁ Ok();
  }

 ବ୍ୟକ୍ତିଗତ:
  ତାର୍କିକ is_supported_gsub_type(unsigned type, gsubgpos_graph_context_t& c) ଅଚଳ {
    ଫେରନ୍ତୁ (c.table_tag == HB_OT_TAG_GSUB) && (
      type == OT::Layout::GSUB_impl::SubstLookupSubTable::Type::Ligature
    );
  }

  ତାର୍କିକ is_supported_gpos_type(unsigned type, gsubgpos_graph_context_t& c) ଅଚଳ {
   ଫେରନ୍ତୁ (c.table_tag == HB_OT_TAG_GPOS) && (
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::Pair ||
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::MarkBase
    );
  }

  unsigned extension_type (hb_tag_t table_tag) ଅଚଳ
  {
    ବାଛ (table_tag)
    {
    ସ୍ଥିତି HB_OT_TAG_GPOS: ଫେରନ୍ତୁ 9;
    ସ୍ଥିତି HB_OT_TAG_GSUB: ଫେରନ୍ତୁ 7;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ 0;
    }
  }
};

template <typename T>
ସଂରଚନା LookupList : ସର୍ବସାଧାରଣ OT::LookupList<T>
{
  graph_result_t<ଶୂନ୍ୟ> sanitize (ଅଚଳ graph_t::vertex_t& vertex) ଅଚଳ
  {
    size_t vertex_len = vertex.table_size ();
    ଯଦି (unlikely (vertex_len < OT::LookupList<T>::min_size)) ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    hb_barrier ();
    ଯଦି (unlikely (vertex_len < OT::LookupList<T>::item_size * ଏହି->len)) ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    ଫେରନ୍ତୁ Ok();
  }
};

ସଂରଚନା GSTAR : ସର୍ବସାଧାରଣ OT::GSUBGPOS
{
  ସ୍ଥିର graph_result_t<ଅଚଳ GSTAR*> graph_to_gstar (graph_t& graph)
  {
    ଅଚଳ auto& r = graph.ମୂଳ ();

    ଅଚଳ GSTAR* gstar = (GSTAR*) r.obj ().head;
    ଯଦି (unlikely (!gstar)) ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    ଫେରନ୍ତୁ gstar;
  }

  ଅଚଳ ଶୂନ୍ୟ* get_lookup_list_field_offset () ଅଚଳ
  {
    ଫେରନ୍ତୁ OT::GSUBGPOS::get_lookup_list_field_offset ();
  }

  graph_result_t<ଶୂନ୍ୟ> sanitize (ଅଚଳ graph_t::vertex_t& vertex) ଅଚଳ
  {
    size_t len = vertex.table_size ();
    ଯଦି (unlikely (len < OT::GSUBGPOS::min_size)) ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    hb_barrier ();
    ଯଦି (unlikely (version.major != 1)) ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    ଯଦି (unlikely (len < get_size ())) ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    ଫେରନ୍ତୁ Ok();
  }

  graph_result_t<ଶୂନ୍ୟ> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010002u && lookupList2)
      ଫେରନ୍ତୁ find_lookups<MediumTypes> (graph, lookups);
#endif
    TRY(find_lookups<SmallTypes> (graph, lookups));
    ଫେରନ୍ତୁ Ok();
  }

  graph_result_t<unsigned> get_lookup_list_index (graph_t& graph) ଅଚଳ
  {
    ଅଚଳ ଶୂନ୍ୟ* offset = get_lookup_list_field_offset ();
    ଯଦି (unlikely (!offset)) ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);
    ଫେରନ୍ତୁ graph.index_for_offset (graph.ମୂଳ_idx (), offset);
  }

  template<typename Types>
  graph_result_t<ଶୂନ୍ୟ> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) ଅଚଳ
  {
    TRY_ASSIGN (unsigned lookup_list_idx, get_lookup_list_index (graph));
    ଅଚଳ LookupList<Types>* lookupList =
        (ଅଚଳ LookupList<Types>*) graph.object (lookup_list_idx).head;
    ଯଦି (unlikely (!lookupList)) ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);
    TRY(lookupList->sanitize (graph.vertices_[lookup_list_idx]));

    ପାଇଁ (unsigned i = 0; i < lookupList->len; i++)
    {
      TRY_ASSIGN (unsigned lookup_idx, graph.index_for_offset (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) graph.object (lookup_idx).head;
      ଯଦି (!lookup || !lookup->sanitize (graph.vertices_[lookup_idx]).is_ok ()) ଜାରି;
      lookups.set (lookup_idx, lookup);
    }
    ଫେରନ୍ତୁ Ok();
  }
};




}

#endif  /* GRAPH_GSUBGPOS_GRAPH_HH */
