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
#അന്തര്ഭാവ "../OT/Layout/GSUB/ExtensionSubst.hh"
#അന്തര്ഭാവ "../OT/Layout/GSUB/SubstLookupSubTable.hh"
#അന്തര്ഭാവ "gsubgpos-context.hh"
#അന്തര്ഭാവ "pairpos-graph.hh"
#അന്തര്ഭാവ "markbasepos-graph.hh"
#അന്തര്ഭാവ "ligature-graph.hh"

#ifndef GRAPH_GSUBGPOS_GRAPH_HH
#define GRAPH_GSUBGPOS_GRAPH_HH

നാമാകാശ graph {

ഘടന Lookup;

template<typename T>
ഘടന ExtensionFormat1 : പൊതു OT::ExtensionFormat1<T>
{
  ശൂന്യം reset(unsigned type)
  {
    ഇത്->format = 1;
    ഇത്->extensionLookupType = type;
    ഇത്->extensionOffset = 0;
  }

  graph_result_t<ശൂന്യം> sanitize (സ്ഥിരം graph_t::vertex_t& vertex) സ്ഥിരം
  {
    size_t vertex_len = vertex.table_size ();
    എങ്കിൽ (unlikely (vertex_len < OT::ExtensionFormat1<T>::static_size))
      തിരികെ Err(SANITIZE_FAILURE);
    തിരികെ Ok();
  }

  unsigned get_lookup_type () സ്ഥിരം
  {
    തിരികെ ഇത്->extensionLookupType;
  }

  graph_result_t<unsigned> get_subtable_index (graph_t& graph, unsigned this_index) സ്ഥിരം
  {
    തിരികെ graph.index_for_offset (this_index, &ഇത്->extensionOffset);
  }
};

ഘടന Lookup : പൊതു OT::Lookup
{
  unsigned സംഖ്യ_of_subtables () സ്ഥിരം
  {
    തിരികെ subTable.len;
  }

  graph_result_t<ശൂന്യം> sanitize (സ്ഥിരം graph_t::vertex_t& vertex) സ്ഥിരം
  {
    size_t vertex_len = vertex.table_size ();
    എങ്കിൽ (unlikely (vertex_len < OT::Lookup::min_size)) തിരികെ Err(SANITIZE_FAILURE);
    hb_barrier ();
    എങ്കിൽ (unlikely (vertex_len < ഇത്->get_size ())) തിരികെ Err(SANITIZE_FAILURE);
    തിരികെ Ok();
  }

  തർക്കം is_extension (hb_tag_t table_tag) സ്ഥിരം
  {
    തിരികെ lookupType == extension_type (table_tag);
  }

  തർക്കം use_mark_filtering_set () സ്ഥിരം
  {
    unsigned flag = lookupFlag;
    തിരികെ flag & 0x0010u;
  }

  graph_result_t<ശൂന്യം> make_extension (gsubgpos_graph_context_t& c,
                                       unsigned this_index)
  {
    unsigned type = lookupType;
    unsigned ext_type = extension_type (c.table_tag);
    എങ്കിൽ (!ext_type || is_extension (c.table_tag))
    {
      // NOOP
      തിരികെ Ok();
    }

    DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_index);

    hb_set_t seen;
    hb_vector_t<unsigned> subtables;
    വേണ്ടി (unsigned i = 0; i < subTable.len; i++)
    {
      TRY_ASSIGN (unsigned subtable_index, c.graph.index_for_offset (this_index, &subTable[i]));
      എങ്കിൽ (seen.has (subtable_index))
        തുടരുക;
      seen.add (subtable_index);
      subtables.push (subtable_index);
    }
    TRY (graph_result_t<ശൂന്യം>::from (seen, ALLOCATION_FAILURE));
    TRY (graph_result_t<ശൂന്യം>::from (subtables, ALLOCATION_FAILURE));

    വേണ്ടി (unsigned subtable_index : subtables)
      TRY (make_subtable_extension (c, this_index, subtable_index));

    lookupType = ext_type;
    തിരികെ Ok();
  }

  graph_result_t<ശൂന്യം> split_subtables_if_needed (gsubgpos_graph_context_t& c,
                                                  unsigned this_index)
  {
    unsigned type = lookupType;
    തർക്കം is_ext = is_extension (c.table_tag);

    എങ്കിൽ (c.table_tag != HB_OT_TAG_GPOS && c.table_tag != HB_OT_TAG_GSUB)
      തിരികെ Ok();

    എങ്കിൽ (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      തിരികെ Ok();

    hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>> all_new_subtables;
    വേണ്ടി (unsigned i = 0; i < subTable.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      auto idx_res = c.graph.index_for_offset (this_index, &subTable[i]);
      എങ്കിൽ (!idx_res.is_ok ()) തുടരുക;
      unsigned subtable_index = *idx_res;
      എങ്കിൽ (is_ext) {
        unsigned ext_subtable_index = subtable_index;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.graph.object (ext_subtable_index).head;
        എങ്കിൽ (!extension || !extension->sanitize (c.graph.vertices_[ext_subtable_index]).is_ok ())
          തുടരുക;

        TRY_ASSIGN (subtable_index, extension->get_subtable_index (c.graph, ext_subtable_index));
        type = extension->get_lookup_type ();
        എങ്കിൽ (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          തുടരുക;
      }

      hb_vector_t<unsigned>* split_result;
      എങ്കിൽ (c.split_subtables.has (subtable_index, &split_result))
      {
        എങ്കിൽ (split_result->length == 0)
          തുടരുക;
        all_new_subtables.push (hb_pair(i, *split_result));
        TRY (graph_result_t<ശൂന്യം>::from (all_new_subtables, ALLOCATION_FAILURE));
      }
      അല്ലെങ്കിൽ
      {
        hb_vector_t<unsigned> new_sub_tables;

        എങ്കിൽ (c.table_tag == HB_OT_TAG_GPOS) {
          തിരഞ്ഞെടുപ്പ് (type)
          {
          അവസ്ഥ 2:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<PairPos> (c, subtable_index));
              നിർത്തുക;
            }
          അവസ്ഥ 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<MarkBasePos> (c, subtable_index));
              നിർത്തുക;
            }
          സ്ഥിരസ്ഥിതി:
            നിർത്തുക;
          }
        } അല്ലെങ്കിൽ എങ്കിൽ (c.table_tag == HB_OT_TAG_GSUB) {
          തിരഞ്ഞെടുപ്പ് (type)
          {
          അവസ്ഥ 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<graph::LigatureSubst> (c, subtable_index));
              നിർത്തുക;
            }
          സ്ഥിരസ്ഥിതി:
            നിർത്തുക;
          }
        }

        c.split_subtables.set (subtable_index, new_sub_tables);
        TRY (graph_result_t<ശൂന്യം>::from (c.split_subtables, ALLOCATION_FAILURE));
        എങ്കിൽ (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (graph_result_t<ശൂന്യം>::from (all_new_subtables, ALLOCATION_FAILURE));
        }
      }
    }

    എങ്കിൽ (all_new_subtables) {
      TRY (add_sub_tables (c, this_index, type, all_new_subtables));
    }

    തിരികെ Ok();
  }

  template<typename T>
  graph_result_t<hb_vector_t<unsigned>> split_subtable (gsubgpos_graph_context_t& c,
                                                        unsigned objidx)
  {
    T* sub_table = (T*) c.graph.object (objidx).head;
    എങ്കിൽ (!sub_table || !sub_table->sanitize (c.graph.vertices_[objidx]).is_ok ())
      തിരികെ Ok(hb_vector_t<unsigned> ());

    തിരികെ sub_table->split_subtables (c, objidx);
  }

  graph_result_t<ശൂന്യം> add_sub_tables (gsubgpos_graph_context_t& c,
                                       unsigned this_index,
                                       unsigned type,
                                       സ്ഥിരം hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    തർക്കം is_ext = is_extension (c.table_tag);
    auto* v = &c.graph.vertices_[this_index];
    fix_existing_subtable_links (c, this_index, subtable_ids);

    unsigned new_subtable_എണ്ണം = 0;
    വേണ്ടി (സ്ഥിരം auto& p : subtable_ids)
      new_subtable_എണ്ണം += p.second.length;

    size_t new_size = v->table_size ()
                      + new_subtable_എണ്ണം * OT::Offset16::static_size;
    അക്ഷരം* buffer = (അക്ഷരം*) hb_calloc (1, new_size);
    എങ്കിൽ (unlikely (!buffer)) തിരികെ Err(ALLOCATION_FAILURE);
    auto res = c.add_buffer (buffer);
    എങ്കിൽ (unlikely (!res.is_ok ()))
    {
      hb_free (buffer);
      തിരികെ res;
    }
    hb_memcpy (buffer, v->obj ().head, v->table_size());

    എങ്കിൽ (use_mark_filtering_set ())
      hb_memcpy (buffer + new_size - 2, v->obj ().tail - 2, 2);

    v->set_buffer (buffer, buffer + new_size);

    Lookup* new_lookup = (Lookup*) buffer;

    unsigned shift = 0;
    new_lookup->subTable.len = subTable.len + new_subtable_എണ്ണം;
    വേണ്ടി (സ്ഥിരം auto& p : subtable_ids)
    {
      unsigned offset_index = p.first + shift + 1;
      shift += p.second.length;

      വേണ്ടി (unsigned subtable_id : p.second)
      {
        എങ്കിൽ (is_ext)
        {
          TRY_ASSIGN (unsigned ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.graph.vertices_[subtable_id].add_parent (ext_id, അസത്യം));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.graph.vertices_[this_index];
        }

        TRY(v->add_real_link(2, subtable_id,
          (അക്ഷരം*) &new_lookup->subTable[offset_index++] - (അക്ഷരം*) new_lookup));
        TRY(c.graph.vertices_[subtable_id].add_parent (this_index, അസത്യം));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->sort_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.set (this_index, new_lookup);
    TRY (graph_result_t<ശൂന്യം>::from (c.lookups, ALLOCATION_FAILURE));
    തിരികെ Ok();
  }

  ശൂന്യം fix_existing_subtable_links (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    സ്ഥിരം hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    auto& v = c.graph.vertices_[this_index];
    unsigned shift = 0;
    വേണ്ടി (സ്ഥിരം auto& p : subtable_ids)
    {
      unsigned തിരുകുക_index = p.first + shift;
      unsigned pos_offset = p.second.length * OT::Offset16::static_size;
      unsigned തിരുകുക_offset = Lookup::min_size + തിരുകുക_index * OT::Offset16::static_size;
      shift += p.second.length;

      വേണ്ടി (auto& l : v.all_links_writer ())
      {
        എങ്കിൽ (l.position > തിരുകുക_offset) l.position += pos_offset;
      }
    }
  }

  graph_result_t<unsigned> create_extension_subtable (gsubgpos_graph_context_t& c,
                                                      unsigned subtable_index,
                                                      unsigned type)
  {
    unsigned extension_size = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_size;

    TRY_ASSIGN (unsigned ext_index, c.create_കണ്ണി (extension_size));

    auto& ext_vertex = c.graph.vertices_[ext_index];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_vertex.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_vertex.add_real_link (4, subtable_index, 4));

    തിരികെ Ok(ext_index);
  }

  graph_result_t<ശൂന്യം> make_subtable_extension (gsubgpos_graph_context_t& c,
                                                unsigned lookup_index,
                                                unsigned subtable_index)
  {
    unsigned type = lookupType;
    unsigned ext_index = HB_GRAPH_INVALID;
    unsigned* existing_ext_index = nullptr;
    എങ്കിൽ (c.subtable_to_extension.has(subtable_index, &existing_ext_index)) {
      ext_index = *existing_ext_index;
    } അല്ലെങ്കിൽ {
      TRY_ASSIGN (ext_index, create_extension_subtable(c, subtable_index, type));
      c.subtable_to_extension.set(subtable_index, ext_index);
      TRY (graph_result_t<ശൂന്യം>::from (c.subtable_to_extension, ALLOCATION_FAILURE));
    }

    auto& subtable_vertex = c.graph.vertices_[subtable_index];
    auto& lookup_vertex = c.graph.vertices_[lookup_index];
    unsigned replaced = 0;
    വേണ്ടി (auto& l : lookup_vertex.real_links_writer ())
    {
      എങ്കിൽ (l.objidx == subtable_index) {
        // Change lookup to point at the extension.
        l.objidx = ext_index;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    auto& ext_vertex = c.graph.vertices_[ext_index];
    വേണ്ടി (unsigned i = 0; i < replaced; i++)
    {
      subtable_vertex.remove_parent (lookup_index);
      TRY(ext_vertex.add_parent (lookup_index, അസത്യം));
    }
    എങ്കിൽ (!existing_ext_index)
      TRY(subtable_vertex.add_parent (ext_index, അസത്യം));

    തിരികെ Ok();
  }

 സ്വകാര്യ:
  തർക്കം is_supported_gsub_type(unsigned type, gsubgpos_graph_context_t& c) സ്ഥിരം {
    തിരികെ (c.table_tag == HB_OT_TAG_GSUB) && (
      type == OT::Layout::GSUB_impl::SubstLookupSubTable::Type::Ligature
    );
  }

  തർക്കം is_supported_gpos_type(unsigned type, gsubgpos_graph_context_t& c) സ്ഥിരം {
   തിരികെ (c.table_tag == HB_OT_TAG_GPOS) && (
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::Pair ||
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::MarkBase
    );
  }

  unsigned extension_type (hb_tag_t table_tag) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (table_tag)
    {
    അവസ്ഥ HB_OT_TAG_GPOS: തിരികെ 9;
    അവസ്ഥ HB_OT_TAG_GSUB: തിരികെ 7;
    സ്ഥിരസ്ഥിതി: തിരികെ 0;
    }
  }
};

template <typename T>
ഘടന LookupList : പൊതു OT::LookupList<T>
{
  graph_result_t<ശൂന്യം> sanitize (സ്ഥിരം graph_t::vertex_t& vertex) സ്ഥിരം
  {
    size_t vertex_len = vertex.table_size ();
    എങ്കിൽ (unlikely (vertex_len < OT::LookupList<T>::min_size)) തിരികെ Err(SANITIZE_FAILURE);
    hb_barrier ();
    എങ്കിൽ (unlikely (vertex_len < OT::LookupList<T>::item_size * ഇത്->len)) തിരികെ Err(SANITIZE_FAILURE);
    തിരികെ Ok();
  }
};

ഘടന GSTAR : പൊതു OT::GSUBGPOS
{
  സ്ഥിതം graph_result_t<സ്ഥിരം GSTAR*> graph_to_gstar (graph_t& graph)
  {
    സ്ഥിരം auto& r = graph.വേര് ();

    സ്ഥിരം GSTAR* gstar = (GSTAR*) r.obj ().head;
    എങ്കിൽ (unlikely (!gstar)) തിരികെ Err(INVALID_ARGUMENT);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    തിരികെ gstar;
  }

  സ്ഥിരം ശൂന്യം* get_lookup_list_field_offset () സ്ഥിരം
  {
    തിരികെ OT::GSUBGPOS::get_lookup_list_field_offset ();
  }

  graph_result_t<ശൂന്യം> sanitize (സ്ഥിരം graph_t::vertex_t& vertex) സ്ഥിരം
  {
    size_t len = vertex.table_size ();
    എങ്കിൽ (unlikely (len < OT::GSUBGPOS::min_size)) തിരികെ Err(SANITIZE_FAILURE);
    hb_barrier ();
    എങ്കിൽ (unlikely (version.major != 1)) തിരികെ Err(SANITIZE_FAILURE);
    എങ്കിൽ (unlikely (len < get_size ())) തിരികെ Err(SANITIZE_FAILURE);
    തിരികെ Ok();
  }

  graph_result_t<ശൂന്യം> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010002u && lookupList2)
      തിരികെ find_lookups<MediumTypes> (graph, lookups);
#endif
    TRY(find_lookups<SmallTypes> (graph, lookups));
    തിരികെ Ok();
  }

  graph_result_t<unsigned> get_lookup_list_index (graph_t& graph) സ്ഥിരം
  {
    സ്ഥിരം ശൂന്യം* offset = get_lookup_list_field_offset ();
    എങ്കിൽ (unlikely (!offset)) തിരികെ Err(INVALID_ARGUMENT);
    തിരികെ graph.index_for_offset (graph.വേര്_idx (), offset);
  }

  template<typename Types>
  graph_result_t<ശൂന്യം> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) സ്ഥിരം
  {
    TRY_ASSIGN (unsigned lookup_list_idx, get_lookup_list_index (graph));
    സ്ഥിരം LookupList<Types>* lookupList =
        (സ്ഥിരം LookupList<Types>*) graph.object (lookup_list_idx).head;
    എങ്കിൽ (unlikely (!lookupList)) തിരികെ Err(INVALID_ARGUMENT);
    TRY(lookupList->sanitize (graph.vertices_[lookup_list_idx]));

    വേണ്ടി (unsigned i = 0; i < lookupList->len; i++)
    {
      TRY_ASSIGN (unsigned lookup_idx, graph.index_for_offset (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) graph.object (lookup_idx).head;
      എങ്കിൽ (!lookup || !lookup->sanitize (graph.vertices_[lookup_idx]).is_ok ()) തുടരുക;
      lookups.set (lookup_idx, lookup);
    }
    തിരികെ Ok();
  }
};




}

#endif  /* GRAPH_GSUBGPOS_GRAPH_HH */
