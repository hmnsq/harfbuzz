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

#அந்தர்பாவ "graph.hh"
#அந்தர்பாவ "../hb-ot-layout-gsubgpos.hh"
#அந்தர்பாவ "../OT/Layout/GSUB/ExtensionSubst.hh"
#அந்தர்பாவ "../OT/Layout/GSUB/SubstLookupSubTable.hh"
#அந்தர்பாவ "gsubgpos-context.hh"
#அந்தர்பாவ "pairpos-graph.hh"
#அந்தர்பாவ "markbasepos-graph.hh"
#அந்தர்பாவ "ligature-graph.hh"

#ifndef GRAPH_GSUBGPOS_GRAPH_HH
#define GRAPH_GSUBGPOS_GRAPH_HH

நாமாகாஶ graph {

கட்டமைப்பு Lookup;

template<typename T>
கட்டமைப்பு ExtensionFormat1 : பொது OT::ExtensionFormat1<T>
{
  வெறுமை reset(unsigned type)
  {
    இது->format = 1;
    இது->extensionLookupType = type;
    இது->extensionOffset = 0;
  }

  graph_result_t<வெறுமை> sanitize (மாறிலி graph_t::vertex_t& vertex) மாறிலி
  {
    size_t vertex_len = vertex.table_size ();
    எனில் (unlikely (vertex_len < OT::ExtensionFormat1<T>::static_size))
      திரும்பு Err(SANITIZE_FAILURE);
    திரும்பு Ok();
  }

  unsigned get_lookup_type () மாறிலி
  {
    திரும்பு இது->extensionLookupType;
  }

  graph_result_t<unsigned> get_subtable_index (graph_t& graph, unsigned this_index) மாறிலி
  {
    திரும்பு graph.index_for_offset (this_index, &இது->extensionOffset);
  }
};

கட்டமைப்பு Lookup : பொது OT::Lookup
{
  unsigned எண்_of_subtables () மாறிலி
  {
    திரும்பு subTable.len;
  }

  graph_result_t<வெறுமை> sanitize (மாறிலி graph_t::vertex_t& vertex) மாறிலி
  {
    size_t vertex_len = vertex.table_size ();
    எனில் (unlikely (vertex_len < OT::Lookup::min_size)) திரும்பு Err(SANITIZE_FAILURE);
    hb_barrier ();
    எனில் (unlikely (vertex_len < இது->get_size ())) திரும்பு Err(SANITIZE_FAILURE);
    திரும்பு Ok();
  }

  தருக்க is_extension (hb_tag_t table_tag) மாறிலி
  {
    திரும்பு lookupType == extension_type (table_tag);
  }

  தருக்க use_mark_filtering_set () மாறிலி
  {
    unsigned flag = lookupFlag;
    திரும்பு flag & 0x0010u;
  }

  graph_result_t<வெறுமை> make_extension (gsubgpos_graph_context_t& c,
                                       unsigned this_index)
  {
    unsigned type = lookupType;
    unsigned ext_type = extension_type (c.table_tag);
    எனில் (!ext_type || is_extension (c.table_tag))
    {
      // NOOP
      திரும்பு Ok();
    }

    DEBUG_செய்தி (SUBSET_REPACK, nullptr,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_index);

    hb_set_t seen;
    hb_vector_t<unsigned> subtables;
    க்கு (unsigned i = 0; i < subTable.len; i++)
    {
      TRY_ASSIGN (unsigned subtable_index, c.graph.index_for_offset (this_index, &subTable[i]));
      எனில் (seen.has (subtable_index))
        தொடர்;
      seen.add (subtable_index);
      subtables.push (subtable_index);
    }
    TRY (graph_result_t<வெறுமை>::from (seen, ALLOCATION_FAILURE));
    TRY (graph_result_t<வெறுமை>::from (subtables, ALLOCATION_FAILURE));

    க்கு (unsigned subtable_index : subtables)
      TRY (make_subtable_extension (c, this_index, subtable_index));

    lookupType = ext_type;
    திரும்பு Ok();
  }

  graph_result_t<வெறுமை> split_subtables_if_needed (gsubgpos_graph_context_t& c,
                                                  unsigned this_index)
  {
    unsigned type = lookupType;
    தருக்க is_ext = is_extension (c.table_tag);

    எனில் (c.table_tag != HB_OT_TAG_GPOS && c.table_tag != HB_OT_TAG_GSUB)
      திரும்பு Ok();

    எனில் (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      திரும்பு Ok();

    hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>> all_new_subtables;
    க்கு (unsigned i = 0; i < subTable.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      auto idx_res = c.graph.index_for_offset (this_index, &subTable[i]);
      எனில் (!idx_res.is_ok ()) தொடர்;
      unsigned subtable_index = *idx_res;
      எனில் (is_ext) {
        unsigned ext_subtable_index = subtable_index;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.graph.object (ext_subtable_index).head;
        எனில் (!extension || !extension->sanitize (c.graph.vertices_[ext_subtable_index]).is_ok ())
          தொடர்;

        TRY_ASSIGN (subtable_index, extension->get_subtable_index (c.graph, ext_subtable_index));
        type = extension->get_lookup_type ();
        எனில் (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          தொடர்;
      }

      hb_vector_t<unsigned>* split_result;
      எனில் (c.split_subtables.has (subtable_index, &split_result))
      {
        எனில் (split_result->length == 0)
          தொடர்;
        all_new_subtables.push (hb_pair(i, *split_result));
        TRY (graph_result_t<வெறுமை>::from (all_new_subtables, ALLOCATION_FAILURE));
      }
      இல்லையெனில்
      {
        hb_vector_t<unsigned> new_sub_tables;

        எனில் (c.table_tag == HB_OT_TAG_GPOS) {
          தேர்வு (type)
          {
          நிலை 2:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<PairPos> (c, subtable_index));
              நிறுத்து;
            }
          நிலை 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<MarkBasePos> (c, subtable_index));
              நிறுத்து;
            }
          இயல்பு:
            நிறுத்து;
          }
        } இல்லையெனில் எனில் (c.table_tag == HB_OT_TAG_GSUB) {
          தேர்வு (type)
          {
          நிலை 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<graph::LigatureSubst> (c, subtable_index));
              நிறுத்து;
            }
          இயல்பு:
            நிறுத்து;
          }
        }

        c.split_subtables.set (subtable_index, new_sub_tables);
        TRY (graph_result_t<வெறுமை>::from (c.split_subtables, ALLOCATION_FAILURE));
        எனில் (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (graph_result_t<வெறுமை>::from (all_new_subtables, ALLOCATION_FAILURE));
        }
      }
    }

    எனில் (all_new_subtables) {
      TRY (add_sub_tables (c, this_index, type, all_new_subtables));
    }

    திரும்பு Ok();
  }

  template<typename T>
  graph_result_t<hb_vector_t<unsigned>> split_subtable (gsubgpos_graph_context_t& c,
                                                        unsigned objidx)
  {
    T* sub_table = (T*) c.graph.object (objidx).head;
    எனில் (!sub_table || !sub_table->sanitize (c.graph.vertices_[objidx]).is_ok ())
      திரும்பு Ok(hb_vector_t<unsigned> ());

    திரும்பு sub_table->split_subtables (c, objidx);
  }

  graph_result_t<வெறுமை> add_sub_tables (gsubgpos_graph_context_t& c,
                                       unsigned this_index,
                                       unsigned type,
                                       மாறிலி hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    தருக்க is_ext = is_extension (c.table_tag);
    auto* v = &c.graph.vertices_[this_index];
    fix_existing_subtable_links (c, this_index, subtable_ids);

    unsigned new_subtable_மொத்தம் = 0;
    க்கு (மாறிலி auto& p : subtable_ids)
      new_subtable_மொத்தம் += p.second.length;

    size_t new_size = v->table_size ()
                      + new_subtable_மொத்தம் * OT::Offset16::static_size;
    எழுத்து* buffer = (எழுத்து*) hb_calloc (1, new_size);
    எனில் (unlikely (!buffer)) திரும்பு Err(ALLOCATION_FAILURE);
    auto res = c.add_buffer (buffer);
    எனில் (unlikely (!res.is_ok ()))
    {
      hb_free (buffer);
      திரும்பு res;
    }
    hb_memcpy (buffer, v->obj ().head, v->table_size());

    எனில் (use_mark_filtering_set ())
      hb_memcpy (buffer + new_size - 2, v->obj ().tail - 2, 2);

    v->set_buffer (buffer, buffer + new_size);

    Lookup* new_lookup = (Lookup*) buffer;

    unsigned shift = 0;
    new_lookup->subTable.len = subTable.len + new_subtable_மொத்தம்;
    க்கு (மாறிலி auto& p : subtable_ids)
    {
      unsigned offset_index = p.first + shift + 1;
      shift += p.second.length;

      க்கு (unsigned subtable_id : p.second)
      {
        எனில் (is_ext)
        {
          TRY_ASSIGN (unsigned ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.graph.vertices_[subtable_id].add_parent (ext_id, பொய்));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.graph.vertices_[this_index];
        }

        TRY(v->add_real_link(2, subtable_id,
          (எழுத்து*) &new_lookup->subTable[offset_index++] - (எழுத்து*) new_lookup));
        TRY(c.graph.vertices_[subtable_id].add_parent (this_index, பொய்));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->sort_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.set (this_index, new_lookup);
    TRY (graph_result_t<வெறுமை>::from (c.lookups, ALLOCATION_FAILURE));
    திரும்பு Ok();
  }

  வெறுமை fix_existing_subtable_links (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    மாறிலி hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    auto& v = c.graph.vertices_[this_index];
    unsigned shift = 0;
    க்கு (மாறிலி auto& p : subtable_ids)
    {
      unsigned செருகு_index = p.first + shift;
      unsigned pos_offset = p.second.length * OT::Offset16::static_size;
      unsigned செருகு_offset = Lookup::min_size + செருகு_index * OT::Offset16::static_size;
      shift += p.second.length;

      க்கு (auto& l : v.all_links_writer ())
      {
        எனில் (l.position > செருகு_offset) l.position += pos_offset;
      }
    }
  }

  graph_result_t<unsigned> create_extension_subtable (gsubgpos_graph_context_t& c,
                                                      unsigned subtable_index,
                                                      unsigned type)
  {
    unsigned extension_size = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_size;

    TRY_ASSIGN (unsigned ext_index, c.create_முனை (extension_size));

    auto& ext_vertex = c.graph.vertices_[ext_index];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_vertex.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_vertex.add_real_link (4, subtable_index, 4));

    திரும்பு Ok(ext_index);
  }

  graph_result_t<வெறுமை> make_subtable_extension (gsubgpos_graph_context_t& c,
                                                unsigned lookup_index,
                                                unsigned subtable_index)
  {
    unsigned type = lookupType;
    unsigned ext_index = HB_GRAPH_INVALID;
    unsigned* existing_ext_index = nullptr;
    எனில் (c.subtable_to_extension.has(subtable_index, &existing_ext_index)) {
      ext_index = *existing_ext_index;
    } இல்லையெனில் {
      TRY_ASSIGN (ext_index, create_extension_subtable(c, subtable_index, type));
      c.subtable_to_extension.set(subtable_index, ext_index);
      TRY (graph_result_t<வெறுமை>::from (c.subtable_to_extension, ALLOCATION_FAILURE));
    }

    auto& subtable_vertex = c.graph.vertices_[subtable_index];
    auto& lookup_vertex = c.graph.vertices_[lookup_index];
    unsigned replaced = 0;
    க்கு (auto& l : lookup_vertex.real_links_writer ())
    {
      எனில் (l.objidx == subtable_index) {
        // Change lookup to point at the extension.
        l.objidx = ext_index;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    auto& ext_vertex = c.graph.vertices_[ext_index];
    க்கு (unsigned i = 0; i < replaced; i++)
    {
      subtable_vertex.remove_parent (lookup_index);
      TRY(ext_vertex.add_parent (lookup_index, பொய்));
    }
    எனில் (!existing_ext_index)
      TRY(subtable_vertex.add_parent (ext_index, பொய்));

    திரும்பு Ok();
  }

 தனிப்பட்ட:
  தருக்க is_supported_gsub_type(unsigned type, gsubgpos_graph_context_t& c) மாறிலி {
    திரும்பு (c.table_tag == HB_OT_TAG_GSUB) && (
      type == OT::Layout::GSUB_impl::SubstLookupSubTable::Type::Ligature
    );
  }

  தருக்க is_supported_gpos_type(unsigned type, gsubgpos_graph_context_t& c) மாறிலி {
   திரும்பு (c.table_tag == HB_OT_TAG_GPOS) && (
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::Pair ||
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::MarkBase
    );
  }

  unsigned extension_type (hb_tag_t table_tag) மாறிலி
  {
    தேர்வு (table_tag)
    {
    நிலை HB_OT_TAG_GPOS: திரும்பு 9;
    நிலை HB_OT_TAG_GSUB: திரும்பு 7;
    இயல்பு: திரும்பு 0;
    }
  }
};

template <typename T>
கட்டமைப்பு LookupList : பொது OT::LookupList<T>
{
  graph_result_t<வெறுமை> sanitize (மாறிலி graph_t::vertex_t& vertex) மாறிலி
  {
    size_t vertex_len = vertex.table_size ();
    எனில் (unlikely (vertex_len < OT::LookupList<T>::min_size)) திரும்பு Err(SANITIZE_FAILURE);
    hb_barrier ();
    எனில் (unlikely (vertex_len < OT::LookupList<T>::item_size * இது->len)) திரும்பு Err(SANITIZE_FAILURE);
    திரும்பு Ok();
  }
};

கட்டமைப்பு GSTAR : பொது OT::GSUBGPOS
{
  நிலையான graph_result_t<மாறிலி GSTAR*> graph_to_gstar (graph_t& graph)
  {
    மாறிலி auto& r = graph.வேர் ();

    மாறிலி GSTAR* gstar = (GSTAR*) r.obj ().head;
    எனில் (unlikely (!gstar)) திரும்பு Err(INVALID_ARGUMENT);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    திரும்பு gstar;
  }

  மாறிலி வெறுமை* get_lookup_list_field_offset () மாறிலி
  {
    திரும்பு OT::GSUBGPOS::get_lookup_list_field_offset ();
  }

  graph_result_t<வெறுமை> sanitize (மாறிலி graph_t::vertex_t& vertex) மாறிலி
  {
    size_t len = vertex.table_size ();
    எனில் (unlikely (len < OT::GSUBGPOS::min_size)) திரும்பு Err(SANITIZE_FAILURE);
    hb_barrier ();
    எனில் (unlikely (version.major != 1)) திரும்பு Err(SANITIZE_FAILURE);
    எனில் (unlikely (len < get_size ())) திரும்பு Err(SANITIZE_FAILURE);
    திரும்பு Ok();
  }

  graph_result_t<வெறுமை> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010002u && lookupList2)
      திரும்பு find_lookups<MediumTypes> (graph, lookups);
#endif
    TRY(find_lookups<SmallTypes> (graph, lookups));
    திரும்பு Ok();
  }

  graph_result_t<unsigned> get_lookup_list_index (graph_t& graph) மாறிலி
  {
    மாறிலி வெறுமை* offset = get_lookup_list_field_offset ();
    எனில் (unlikely (!offset)) திரும்பு Err(INVALID_ARGUMENT);
    திரும்பு graph.index_for_offset (graph.வேர்_idx (), offset);
  }

  template<typename Types>
  graph_result_t<வெறுமை> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) மாறிலி
  {
    TRY_ASSIGN (unsigned lookup_list_idx, get_lookup_list_index (graph));
    மாறிலி LookupList<Types>* lookupList =
        (மாறிலி LookupList<Types>*) graph.object (lookup_list_idx).head;
    எனில் (unlikely (!lookupList)) திரும்பு Err(INVALID_ARGUMENT);
    TRY(lookupList->sanitize (graph.vertices_[lookup_list_idx]));

    க்கு (unsigned i = 0; i < lookupList->len; i++)
    {
      TRY_ASSIGN (unsigned lookup_idx, graph.index_for_offset (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) graph.object (lookup_idx).head;
      எனில் (!lookup || !lookup->sanitize (graph.vertices_[lookup_idx]).is_ok ()) தொடர்;
      lookups.set (lookup_idx, lookup);
    }
    திரும்பு Ok();
  }
};




}

#endif  /* GRAPH_GSUBGPOS_GRAPH_HH */
