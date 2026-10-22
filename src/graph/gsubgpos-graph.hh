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

#અન્તર્ભાવ "graph.hh"
#અન્તર્ભાવ "../hb-ot-layout-gsubgpos.hh"
#અન્તર્ભાવ "../OT/Layout/GSUB/ExtensionSubst.hh"
#અન્તર્ભાવ "../OT/Layout/GSUB/SubstLookupSubTable.hh"
#અન્તર્ભાવ "gsubgpos-context.hh"
#અન્તર્ભાવ "pairpos-graph.hh"
#અન્તર્ભાવ "markbasepos-graph.hh"
#અન્તર્ભાવ "ligature-graph.hh"

#ifndef GRAPH_GSUBGPOS_GRAPH_HH
#define GRAPH_GSUBGPOS_GRAPH_HH

નામાકાશ graph {

સંરચના Lookup;

template<typename T>
સંરચના ExtensionFormat1 : જાહેર OT::ExtensionFormat1<T>
{
  શૂન્ય reset(unsigned type)
  {
    આ->format = 1;
    આ->extensionLookupType = type;
    આ->extensionOffset = 0;
  }

  graph_result_t<શૂન્ય> sanitize (અચલ graph_t::vertex_t& vertex) અચલ
  {
    size_t vertex_len = vertex.table_size ();
    જો (unlikely (vertex_len < OT::ExtensionFormat1<T>::static_size))
      પાછા Err(SANITIZE_FAILURE);
    પાછા Ok();
  }

  unsigned get_lookup_type () અચલ
  {
    પાછા આ->extensionLookupType;
  }

  graph_result_t<unsigned> get_subtable_index (graph_t& graph, unsigned this_index) અચલ
  {
    પાછા graph.index_for_offset (this_index, &આ->extensionOffset);
  }
};

સંરચના Lookup : જાહેર OT::Lookup
{
  unsigned સંખ્યા_of_subtables () અચલ
  {
    પાછા subTable.len;
  }

  graph_result_t<શૂન્ય> sanitize (અચલ graph_t::vertex_t& vertex) અચલ
  {
    size_t vertex_len = vertex.table_size ();
    જો (unlikely (vertex_len < OT::Lookup::min_size)) પાછા Err(SANITIZE_FAILURE);
    hb_barrier ();
    જો (unlikely (vertex_len < આ->get_size ())) પાછા Err(SANITIZE_FAILURE);
    પાછા Ok();
  }

  તાર્કિક is_extension (hb_tag_t table_tag) અચલ
  {
    પાછા lookupType == extension_type (table_tag);
  }

  તાર્કિક use_mark_filtering_set () અચલ
  {
    unsigned flag = lookupFlag;
    પાછા flag & 0x0010u;
  }

  graph_result_t<શૂન્ય> make_extension (gsubgpos_graph_context_t& c,
                                       unsigned this_index)
  {
    unsigned type = lookupType;
    unsigned ext_type = extension_type (c.table_tag);
    જો (!ext_type || is_extension (c.table_tag))
    {
      // NOOP
      પાછા Ok();
    }

    DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_index);

    hb_set_t seen;
    hb_vector_t<unsigned> subtables;
    માટે (unsigned i = 0; i < subTable.len; i++)
    {
      TRY_ASSIGN (unsigned subtable_index, c.graph.index_for_offset (this_index, &subTable[i]));
      જો (seen.has (subtable_index))
        ચાલુ;
      seen.add (subtable_index);
      subtables.push (subtable_index);
    }
    TRY (graph_result_t<શૂન્ય>::from (seen, ALLOCATION_FAILURE));
    TRY (graph_result_t<શૂન્ય>::from (subtables, ALLOCATION_FAILURE));

    માટે (unsigned subtable_index : subtables)
      TRY (make_subtable_extension (c, this_index, subtable_index));

    lookupType = ext_type;
    પાછા Ok();
  }

  graph_result_t<શૂન્ય> split_subtables_if_needed (gsubgpos_graph_context_t& c,
                                                  unsigned this_index)
  {
    unsigned type = lookupType;
    તાર્કિક is_ext = is_extension (c.table_tag);

    જો (c.table_tag != HB_OT_TAG_GPOS && c.table_tag != HB_OT_TAG_GSUB)
      પાછા Ok();

    જો (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      પાછા Ok();

    hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>> all_new_subtables;
    માટે (unsigned i = 0; i < subTable.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      auto idx_res = c.graph.index_for_offset (this_index, &subTable[i]);
      જો (!idx_res.is_ok ()) ચાલુ;
      unsigned subtable_index = *idx_res;
      જો (is_ext) {
        unsigned ext_subtable_index = subtable_index;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.graph.object (ext_subtable_index).head;
        જો (!extension || !extension->sanitize (c.graph.vertices_[ext_subtable_index]).is_ok ())
          ચાલુ;

        TRY_ASSIGN (subtable_index, extension->get_subtable_index (c.graph, ext_subtable_index));
        type = extension->get_lookup_type ();
        જો (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          ચાલુ;
      }

      hb_vector_t<unsigned>* split_result;
      જો (c.split_subtables.has (subtable_index, &split_result))
      {
        જો (split_result->length == 0)
          ચાલુ;
        all_new_subtables.push (hb_pair(i, *split_result));
        TRY (graph_result_t<શૂન્ય>::from (all_new_subtables, ALLOCATION_FAILURE));
      }
      નહીંતર
      {
        hb_vector_t<unsigned> new_sub_tables;

        જો (c.table_tag == HB_OT_TAG_GPOS) {
          પસંદગી (type)
          {
          સ્થિતિ 2:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<PairPos> (c, subtable_index));
              તોડો;
            }
          સ્થિતિ 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<MarkBasePos> (c, subtable_index));
              તોડો;
            }
          મૂળભૂત:
            તોડો;
          }
        } નહીંતર જો (c.table_tag == HB_OT_TAG_GSUB) {
          પસંદગી (type)
          {
          સ્થિતિ 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<graph::LigatureSubst> (c, subtable_index));
              તોડો;
            }
          મૂળભૂત:
            તોડો;
          }
        }

        c.split_subtables.set (subtable_index, new_sub_tables);
        TRY (graph_result_t<શૂન્ય>::from (c.split_subtables, ALLOCATION_FAILURE));
        જો (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (graph_result_t<શૂન્ય>::from (all_new_subtables, ALLOCATION_FAILURE));
        }
      }
    }

    જો (all_new_subtables) {
      TRY (add_sub_tables (c, this_index, type, all_new_subtables));
    }

    પાછા Ok();
  }

  template<typename T>
  graph_result_t<hb_vector_t<unsigned>> split_subtable (gsubgpos_graph_context_t& c,
                                                        unsigned objidx)
  {
    T* sub_table = (T*) c.graph.object (objidx).head;
    જો (!sub_table || !sub_table->sanitize (c.graph.vertices_[objidx]).is_ok ())
      પાછા Ok(hb_vector_t<unsigned> ());

    પાછા sub_table->split_subtables (c, objidx);
  }

  graph_result_t<શૂન્ય> add_sub_tables (gsubgpos_graph_context_t& c,
                                       unsigned this_index,
                                       unsigned type,
                                       અચલ hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    તાર્કિક is_ext = is_extension (c.table_tag);
    auto* v = &c.graph.vertices_[this_index];
    fix_existing_subtable_links (c, this_index, subtable_ids);

    unsigned new_subtable_ગણતરી = 0;
    માટે (અચલ auto& p : subtable_ids)
      new_subtable_ગણતરી += p.second.length;

    size_t new_size = v->table_size ()
                      + new_subtable_ગણતરી * OT::Offset16::static_size;
    અક્ષર* buffer = (અક્ષર*) hb_calloc (1, new_size);
    જો (unlikely (!buffer)) પાછા Err(ALLOCATION_FAILURE);
    auto res = c.add_buffer (buffer);
    જો (unlikely (!res.is_ok ()))
    {
      hb_free (buffer);
      પાછા res;
    }
    hb_memcpy (buffer, v->obj ().head, v->table_size());

    જો (use_mark_filtering_set ())
      hb_memcpy (buffer + new_size - 2, v->obj ().tail - 2, 2);

    v->set_buffer (buffer, buffer + new_size);

    Lookup* new_lookup = (Lookup*) buffer;

    unsigned shift = 0;
    new_lookup->subTable.len = subTable.len + new_subtable_ગણતરી;
    માટે (અચલ auto& p : subtable_ids)
    {
      unsigned offset_index = p.first + shift + 1;
      shift += p.second.length;

      માટે (unsigned subtable_id : p.second)
      {
        જો (is_ext)
        {
          TRY_ASSIGN (unsigned ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.graph.vertices_[subtable_id].add_parent (ext_id, ખોટું));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.graph.vertices_[this_index];
        }

        TRY(v->add_real_link(2, subtable_id,
          (અક્ષર*) &new_lookup->subTable[offset_index++] - (અક્ષર*) new_lookup));
        TRY(c.graph.vertices_[subtable_id].add_parent (this_index, ખોટું));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->sort_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.set (this_index, new_lookup);
    TRY (graph_result_t<શૂન્ય>::from (c.lookups, ALLOCATION_FAILURE));
    પાછા Ok();
  }

  શૂન્ય fix_existing_subtable_links (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    અચલ hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    auto& v = c.graph.vertices_[this_index];
    unsigned shift = 0;
    માટે (અચલ auto& p : subtable_ids)
    {
      unsigned ઉમેરો_index = p.first + shift;
      unsigned pos_offset = p.second.length * OT::Offset16::static_size;
      unsigned ઉમેરો_offset = Lookup::min_size + ઉમેરો_index * OT::Offset16::static_size;
      shift += p.second.length;

      માટે (auto& l : v.all_links_writer ())
      {
        જો (l.position > ઉમેરો_offset) l.position += pos_offset;
      }
    }
  }

  graph_result_t<unsigned> create_extension_subtable (gsubgpos_graph_context_t& c,
                                                      unsigned subtable_index,
                                                      unsigned type)
  {
    unsigned extension_size = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_size;

    TRY_ASSIGN (unsigned ext_index, c.create_ગાંઠ (extension_size));

    auto& ext_vertex = c.graph.vertices_[ext_index];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_vertex.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_vertex.add_real_link (4, subtable_index, 4));

    પાછા Ok(ext_index);
  }

  graph_result_t<શૂન્ય> make_subtable_extension (gsubgpos_graph_context_t& c,
                                                unsigned lookup_index,
                                                unsigned subtable_index)
  {
    unsigned type = lookupType;
    unsigned ext_index = HB_GRAPH_INVALID;
    unsigned* existing_ext_index = nullptr;
    જો (c.subtable_to_extension.has(subtable_index, &existing_ext_index)) {
      ext_index = *existing_ext_index;
    } નહીંતર {
      TRY_ASSIGN (ext_index, create_extension_subtable(c, subtable_index, type));
      c.subtable_to_extension.set(subtable_index, ext_index);
      TRY (graph_result_t<શૂન્ય>::from (c.subtable_to_extension, ALLOCATION_FAILURE));
    }

    auto& subtable_vertex = c.graph.vertices_[subtable_index];
    auto& lookup_vertex = c.graph.vertices_[lookup_index];
    unsigned replaced = 0;
    માટે (auto& l : lookup_vertex.real_links_writer ())
    {
      જો (l.objidx == subtable_index) {
        // Change lookup to point at the extension.
        l.objidx = ext_index;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    auto& ext_vertex = c.graph.vertices_[ext_index];
    માટે (unsigned i = 0; i < replaced; i++)
    {
      subtable_vertex.remove_parent (lookup_index);
      TRY(ext_vertex.add_parent (lookup_index, ખોટું));
    }
    જો (!existing_ext_index)
      TRY(subtable_vertex.add_parent (ext_index, ખોટું));

    પાછા Ok();
  }

 ખાનગી:
  તાર્કિક is_supported_gsub_type(unsigned type, gsubgpos_graph_context_t& c) અચલ {
    પાછા (c.table_tag == HB_OT_TAG_GSUB) && (
      type == OT::Layout::GSUB_impl::SubstLookupSubTable::Type::Ligature
    );
  }

  તાર્કિક is_supported_gpos_type(unsigned type, gsubgpos_graph_context_t& c) અચલ {
   પાછા (c.table_tag == HB_OT_TAG_GPOS) && (
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::Pair ||
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::MarkBase
    );
  }

  unsigned extension_type (hb_tag_t table_tag) અચલ
  {
    પસંદગી (table_tag)
    {
    સ્થિતિ HB_OT_TAG_GPOS: પાછા 9;
    સ્થિતિ HB_OT_TAG_GSUB: પાછા 7;
    મૂળભૂત: પાછા 0;
    }
  }
};

template <typename T>
સંરચના LookupList : જાહેર OT::LookupList<T>
{
  graph_result_t<શૂન્ય> sanitize (અચલ graph_t::vertex_t& vertex) અચલ
  {
    size_t vertex_len = vertex.table_size ();
    જો (unlikely (vertex_len < OT::LookupList<T>::min_size)) પાછા Err(SANITIZE_FAILURE);
    hb_barrier ();
    જો (unlikely (vertex_len < OT::LookupList<T>::item_size * આ->len)) પાછા Err(SANITIZE_FAILURE);
    પાછા Ok();
  }
};

સંરચના GSTAR : જાહેર OT::GSUBGPOS
{
  સ્થિર graph_result_t<અચલ GSTAR*> graph_to_gstar (graph_t& graph)
  {
    અચલ auto& r = graph.મૂળ ();

    અચલ GSTAR* gstar = (GSTAR*) r.obj ().head;
    જો (unlikely (!gstar)) પાછા Err(INVALID_ARGUMENT);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    પાછા gstar;
  }

  અચલ શૂન્ય* get_lookup_list_field_offset () અચલ
  {
    પાછા OT::GSUBGPOS::get_lookup_list_field_offset ();
  }

  graph_result_t<શૂન્ય> sanitize (અચલ graph_t::vertex_t& vertex) અચલ
  {
    size_t len = vertex.table_size ();
    જો (unlikely (len < OT::GSUBGPOS::min_size)) પાછા Err(SANITIZE_FAILURE);
    hb_barrier ();
    જો (unlikely (version.major != 1)) પાછા Err(SANITIZE_FAILURE);
    જો (unlikely (len < get_size ())) પાછા Err(SANITIZE_FAILURE);
    પાછા Ok();
  }

  graph_result_t<શૂન્ય> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010002u && lookupList2)
      પાછા find_lookups<MediumTypes> (graph, lookups);
#endif
    TRY(find_lookups<SmallTypes> (graph, lookups));
    પાછા Ok();
  }

  graph_result_t<unsigned> get_lookup_list_index (graph_t& graph) અચલ
  {
    અચલ શૂન્ય* offset = get_lookup_list_field_offset ();
    જો (unlikely (!offset)) પાછા Err(INVALID_ARGUMENT);
    પાછા graph.index_for_offset (graph.મૂળ_idx (), offset);
  }

  template<typename Types>
  graph_result_t<શૂન્ય> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) અચલ
  {
    TRY_ASSIGN (unsigned lookup_list_idx, get_lookup_list_index (graph));
    અચલ LookupList<Types>* lookupList =
        (અચલ LookupList<Types>*) graph.object (lookup_list_idx).head;
    જો (unlikely (!lookupList)) પાછા Err(INVALID_ARGUMENT);
    TRY(lookupList->sanitize (graph.vertices_[lookup_list_idx]));

    માટે (unsigned i = 0; i < lookupList->len; i++)
    {
      TRY_ASSIGN (unsigned lookup_idx, graph.index_for_offset (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) graph.object (lookup_idx).head;
      જો (!lookup || !lookup->sanitize (graph.vertices_[lookup_idx]).is_ok ()) ચાલુ;
      lookups.set (lookup_idx, lookup);
    }
    પાછા Ok();
  }
};




}

#endif  /* GRAPH_GSUBGPOS_GRAPH_HH */
