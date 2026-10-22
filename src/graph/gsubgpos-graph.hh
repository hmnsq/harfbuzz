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

#অন্তর্ভাব "graph.hh"
#অন্তর্ভাব "../hb-ot-layout-gsubgpos.hh"
#অন্তর্ভাব "../OT/Layout/GSUB/ExtensionSubst.hh"
#অন্তর্ভাব "../OT/Layout/GSUB/SubstLookupSubTable.hh"
#অন্তর্ভাব "gsubgpos-context.hh"
#অন্তর্ভাব "pairpos-graph.hh"
#অন্তর্ভাব "markbasepos-graph.hh"
#অন্তর্ভাব "ligature-graph.hh"

#ifndef GRAPH_GSUBGPOS_GRAPH_HH
#define GRAPH_GSUBGPOS_GRAPH_HH

নামাকাশ graph {

সংরচনা Lookup;

template<typename T>
সংরচনা ExtensionFormat1 : সার্বজনিক OT::ExtensionFormat1<T>
{
  শূন্য reset(unsigned type)
  {
    এতত্->format = 1;
    এতত্->extensionLookupType = type;
    এতত্->extensionOffset = 0;
  }

  graph_result_t<শূন্য> sanitize (ধ্রুব graph_t::vertex_t& vertex) ধ্রুব
  {
    size_t vertex_len = vertex.table_size ();
    যদি (unlikely (vertex_len < OT::ExtensionFormat1<T>::static_size))
      প্রত্যাগম Err(SANITIZE_FAILURE);
    প্রত্যাগম Ok();
  }

  unsigned get_lookup_type () ধ্রুব
  {
    প্রত্যাগম এতত্->extensionLookupType;
  }

  graph_result_t<unsigned> get_subtable_index (graph_t& graph, unsigned this_index) ধ্রুব
  {
    প্রত্যাগম graph.index_for_offset (this_index, &এতত্->extensionOffset);
  }
};

সংরচনা Lookup : সার্বজনিক OT::Lookup
{
  unsigned সংখ্যা_of_subtables () ধ্রুব
  {
    প্রত্যাগম subTable.len;
  }

  graph_result_t<শূন্য> sanitize (ধ্রুব graph_t::vertex_t& vertex) ধ্রুব
  {
    size_t vertex_len = vertex.table_size ();
    যদি (unlikely (vertex_len < OT::Lookup::min_size)) প্রত্যাগম Err(SANITIZE_FAILURE);
    hb_barrier ();
    যদি (unlikely (vertex_len < এতত্->get_size ())) প্রত্যাগম Err(SANITIZE_FAILURE);
    প্রত্যাগম Ok();
  }

  তার্কিক is_extension (hb_tag_t table_tag) ধ্রুব
  {
    প্রত্যাগম lookupType == extension_type (table_tag);
  }

  তার্কিক use_mark_filtering_set () ধ্রুব
  {
    unsigned flag = lookupFlag;
    প্রত্যাগম flag & 0x0010u;
  }

  graph_result_t<শূন্য> make_extension (gsubgpos_graph_context_t& c,
                                       unsigned this_index)
  {
    unsigned type = lookupType;
    unsigned ext_type = extension_type (c.table_tag);
    যদি (!ext_type || is_extension (c.table_tag))
    {
      // NOOP
      প্রত্যাগম Ok();
    }

    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_index);

    hb_set_t seen;
    hb_vector_t<unsigned> subtables;
    প্রতি (unsigned i = 0; i < subTable.len; i++)
    {
      TRY_ASSIGN (unsigned subtable_index, c.graph.index_for_offset (this_index, &subTable[i]));
      যদি (seen.has (subtable_index))
        অনুবর্ত;
      seen.add (subtable_index);
      subtables.push (subtable_index);
    }
    TRY (graph_result_t<শূন্য>::from (seen, ALLOCATION_FAILURE));
    TRY (graph_result_t<শূন্য>::from (subtables, ALLOCATION_FAILURE));

    প্রতি (unsigned subtable_index : subtables)
      TRY (make_subtable_extension (c, this_index, subtable_index));

    lookupType = ext_type;
    প্রত্যাগম Ok();
  }

  graph_result_t<শূন্য> split_subtables_if_needed (gsubgpos_graph_context_t& c,
                                                  unsigned this_index)
  {
    unsigned type = lookupType;
    তার্কিক is_ext = is_extension (c.table_tag);

    যদি (c.table_tag != HB_OT_TAG_GPOS && c.table_tag != HB_OT_TAG_GSUB)
      প্রত্যাগম Ok();

    যদি (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      প্রত্যাগম Ok();

    hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>> all_new_subtables;
    প্রতি (unsigned i = 0; i < subTable.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      auto idx_res = c.graph.index_for_offset (this_index, &subTable[i]);
      যদি (!idx_res.is_ok ()) অনুবর্ত;
      unsigned subtable_index = *idx_res;
      যদি (is_ext) {
        unsigned ext_subtable_index = subtable_index;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.graph.object (ext_subtable_index).head;
        যদি (!extension || !extension->sanitize (c.graph.vertices_[ext_subtable_index]).is_ok ())
          অনুবর্ত;

        TRY_ASSIGN (subtable_index, extension->get_subtable_index (c.graph, ext_subtable_index));
        type = extension->get_lookup_type ();
        যদি (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          অনুবর্ত;
      }

      hb_vector_t<unsigned>* split_result;
      যদি (c.split_subtables.has (subtable_index, &split_result))
      {
        যদি (split_result->length == 0)
          অনুবর্ত;
        all_new_subtables.push (hb_pair(i, *split_result));
        TRY (graph_result_t<শূন্য>::from (all_new_subtables, ALLOCATION_FAILURE));
      }
      অন্যথা
      {
        hb_vector_t<unsigned> new_sub_tables;

        যদি (c.table_tag == HB_OT_TAG_GPOS) {
          বিকল্প (type)
          {
          পক্ষ 2:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<PairPos> (c, subtable_index));
              ভঙ্গ;
            }
          পক্ষ 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<MarkBasePos> (c, subtable_index));
              ভঙ্গ;
            }
          স্বভাব:
            ভঙ্গ;
          }
        } অন্যথা যদি (c.table_tag == HB_OT_TAG_GSUB) {
          বিকল্প (type)
          {
          পক্ষ 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<graph::LigatureSubst> (c, subtable_index));
              ভঙ্গ;
            }
          স্বভাব:
            ভঙ্গ;
          }
        }

        c.split_subtables.set (subtable_index, new_sub_tables);
        TRY (graph_result_t<শূন্য>::from (c.split_subtables, ALLOCATION_FAILURE));
        যদি (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (graph_result_t<শূন্য>::from (all_new_subtables, ALLOCATION_FAILURE));
        }
      }
    }

    যদি (all_new_subtables) {
      TRY (add_sub_tables (c, this_index, type, all_new_subtables));
    }

    প্রত্যাগম Ok();
  }

  template<typename T>
  graph_result_t<hb_vector_t<unsigned>> split_subtable (gsubgpos_graph_context_t& c,
                                                        unsigned objidx)
  {
    T* sub_table = (T*) c.graph.object (objidx).head;
    যদি (!sub_table || !sub_table->sanitize (c.graph.vertices_[objidx]).is_ok ())
      প্রত্যাগম Ok(hb_vector_t<unsigned> ());

    প্রত্যাগম sub_table->split_subtables (c, objidx);
  }

  graph_result_t<শূন্য> add_sub_tables (gsubgpos_graph_context_t& c,
                                       unsigned this_index,
                                       unsigned type,
                                       ধ্রুব hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    তার্কিক is_ext = is_extension (c.table_tag);
    auto* v = &c.graph.vertices_[this_index];
    fix_existing_subtable_links (c, this_index, subtable_ids);

    unsigned new_subtable_গুনতি = 0;
    প্রতি (ধ্রুব auto& p : subtable_ids)
      new_subtable_গুনতি += p.second.length;

    size_t new_size = v->table_size ()
                      + new_subtable_গুনতি * OT::Offset16::static_size;
    বর্ণ* buffer = (বর্ণ*) hb_calloc (1, new_size);
    যদি (unlikely (!buffer)) প্রত্যাগম Err(ALLOCATION_FAILURE);
    auto res = c.add_buffer (buffer);
    যদি (unlikely (!res.is_ok ()))
    {
      hb_free (buffer);
      প্রত্যাগম res;
    }
    hb_memcpy (buffer, v->obj ().head, v->table_size());

    যদি (use_mark_filtering_set ())
      hb_memcpy (buffer + new_size - 2, v->obj ().tail - 2, 2);

    v->set_buffer (buffer, buffer + new_size);

    Lookup* new_lookup = (Lookup*) buffer;

    unsigned shift = 0;
    new_lookup->subTable.len = subTable.len + new_subtable_গুনতি;
    প্রতি (ধ্রুব auto& p : subtable_ids)
    {
      unsigned offset_index = p.first + shift + 1;
      shift += p.second.length;

      প্রতি (unsigned subtable_id : p.second)
      {
        যদি (is_ext)
        {
          TRY_ASSIGN (unsigned ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.graph.vertices_[subtable_id].add_parent (ext_id, অসত্য));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.graph.vertices_[this_index];
        }

        TRY(v->add_real_link(2, subtable_id,
          (বর্ণ*) &new_lookup->subTable[offset_index++] - (বর্ণ*) new_lookup));
        TRY(c.graph.vertices_[subtable_id].add_parent (this_index, অসত্য));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->sort_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.set (this_index, new_lookup);
    TRY (graph_result_t<শূন্য>::from (c.lookups, ALLOCATION_FAILURE));
    প্রত্যাগম Ok();
  }

  শূন্য fix_existing_subtable_links (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    ধ্রুব hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    auto& v = c.graph.vertices_[this_index];
    unsigned shift = 0;
    প্রতি (ধ্রুব auto& p : subtable_ids)
    {
      unsigned ঢোকাও_index = p.first + shift;
      unsigned pos_offset = p.second.length * OT::Offset16::static_size;
      unsigned ঢোকাও_offset = Lookup::min_size + ঢোকাও_index * OT::Offset16::static_size;
      shift += p.second.length;

      প্রতি (auto& l : v.all_links_writer ())
      {
        যদি (l.position > ঢোকাও_offset) l.position += pos_offset;
      }
    }
  }

  graph_result_t<unsigned> create_extension_subtable (gsubgpos_graph_context_t& c,
                                                      unsigned subtable_index,
                                                      unsigned type)
  {
    unsigned extension_size = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_size;

    TRY_ASSIGN (unsigned ext_index, c.create_গ্রন্থি (extension_size));

    auto& ext_vertex = c.graph.vertices_[ext_index];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_vertex.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_vertex.add_real_link (4, subtable_index, 4));

    প্রত্যাগম Ok(ext_index);
  }

  graph_result_t<শূন্য> make_subtable_extension (gsubgpos_graph_context_t& c,
                                                unsigned lookup_index,
                                                unsigned subtable_index)
  {
    unsigned type = lookupType;
    unsigned ext_index = HB_GRAPH_INVALID;
    unsigned* existing_ext_index = nullptr;
    যদি (c.subtable_to_extension.has(subtable_index, &existing_ext_index)) {
      ext_index = *existing_ext_index;
    } অন্যথা {
      TRY_ASSIGN (ext_index, create_extension_subtable(c, subtable_index, type));
      c.subtable_to_extension.set(subtable_index, ext_index);
      TRY (graph_result_t<শূন্য>::from (c.subtable_to_extension, ALLOCATION_FAILURE));
    }

    auto& subtable_vertex = c.graph.vertices_[subtable_index];
    auto& lookup_vertex = c.graph.vertices_[lookup_index];
    unsigned replaced = 0;
    প্রতি (auto& l : lookup_vertex.real_links_writer ())
    {
      যদি (l.objidx == subtable_index) {
        // Change lookup to point at the extension.
        l.objidx = ext_index;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    auto& ext_vertex = c.graph.vertices_[ext_index];
    প্রতি (unsigned i = 0; i < replaced; i++)
    {
      subtable_vertex.remove_parent (lookup_index);
      TRY(ext_vertex.add_parent (lookup_index, অসত্য));
    }
    যদি (!existing_ext_index)
      TRY(subtable_vertex.add_parent (ext_index, অসত্য));

    প্রত্যাগম Ok();
  }

 গুপ্ত:
  তার্কিক is_supported_gsub_type(unsigned type, gsubgpos_graph_context_t& c) ধ্রুব {
    প্রত্যাগম (c.table_tag == HB_OT_TAG_GSUB) && (
      type == OT::Layout::GSUB_impl::SubstLookupSubTable::Type::Ligature
    );
  }

  তার্কিক is_supported_gpos_type(unsigned type, gsubgpos_graph_context_t& c) ধ্রুব {
   প্রত্যাগম (c.table_tag == HB_OT_TAG_GPOS) && (
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::Pair ||
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::MarkBase
    );
  }

  unsigned extension_type (hb_tag_t table_tag) ধ্রুব
  {
    বিকল্প (table_tag)
    {
    পক্ষ HB_OT_TAG_GPOS: প্রত্যাগম 9;
    পক্ষ HB_OT_TAG_GSUB: প্রত্যাগম 7;
    স্বভাব: প্রত্যাগম 0;
    }
  }
};

template <typename T>
সংরচনা LookupList : সার্বজনিক OT::LookupList<T>
{
  graph_result_t<শূন্য> sanitize (ধ্রুব graph_t::vertex_t& vertex) ধ্রুব
  {
    size_t vertex_len = vertex.table_size ();
    যদি (unlikely (vertex_len < OT::LookupList<T>::min_size)) প্রত্যাগম Err(SANITIZE_FAILURE);
    hb_barrier ();
    যদি (unlikely (vertex_len < OT::LookupList<T>::item_size * এতত্->len)) প্রত্যাগম Err(SANITIZE_FAILURE);
    প্রত্যাগম Ok();
  }
};

সংরচনা GSTAR : সার্বজনিক OT::GSUBGPOS
{
  স্থির graph_result_t<ধ্রুব GSTAR*> graph_to_gstar (graph_t& graph)
  {
    ধ্রুব auto& r = graph.মূল ();

    ধ্রুব GSTAR* gstar = (GSTAR*) r.obj ().head;
    যদি (unlikely (!gstar)) প্রত্যাগম Err(INVALID_ARGUMENT);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    প্রত্যাগম gstar;
  }

  ধ্রুব শূন্য* get_lookup_list_field_offset () ধ্রুব
  {
    প্রত্যাগম OT::GSUBGPOS::get_lookup_list_field_offset ();
  }

  graph_result_t<শূন্য> sanitize (ধ্রুব graph_t::vertex_t& vertex) ধ্রুব
  {
    size_t len = vertex.table_size ();
    যদি (unlikely (len < OT::GSUBGPOS::min_size)) প্রত্যাগম Err(SANITIZE_FAILURE);
    hb_barrier ();
    যদি (unlikely (version.major != 1)) প্রত্যাগম Err(SANITIZE_FAILURE);
    যদি (unlikely (len < get_size ())) প্রত্যাগম Err(SANITIZE_FAILURE);
    প্রত্যাগম Ok();
  }

  graph_result_t<শূন্য> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) ধ্রুব
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010002u && lookupList2)
      প্রত্যাগম find_lookups<MediumTypes> (graph, lookups);
#endif
    TRY(find_lookups<SmallTypes> (graph, lookups));
    প্রত্যাগম Ok();
  }

  graph_result_t<unsigned> get_lookup_list_index (graph_t& graph) ধ্রুব
  {
    ধ্রুব শূন্য* offset = get_lookup_list_field_offset ();
    যদি (unlikely (!offset)) প্রত্যাগম Err(INVALID_ARGUMENT);
    প্রত্যাগম graph.index_for_offset (graph.মূল_idx (), offset);
  }

  template<typename Types>
  graph_result_t<শূন্য> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) ধ্রুব
  {
    TRY_ASSIGN (unsigned lookup_list_idx, get_lookup_list_index (graph));
    ধ্রুব LookupList<Types>* lookupList =
        (ধ্রুব LookupList<Types>*) graph.object (lookup_list_idx).head;
    যদি (unlikely (!lookupList)) প্রত্যাগম Err(INVALID_ARGUMENT);
    TRY(lookupList->sanitize (graph.vertices_[lookup_list_idx]));

    প্রতি (unsigned i = 0; i < lookupList->len; i++)
    {
      TRY_ASSIGN (unsigned lookup_idx, graph.index_for_offset (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) graph.object (lookup_idx).head;
      যদি (!lookup || !lookup->sanitize (graph.vertices_[lookup_idx]).is_ok ()) অনুবর্ত;
      lookups.set (lookup_idx, lookup);
    }
    প্রত্যাগম Ok();
  }
};




}

#endif  /* GRAPH_GSUBGPOS_GRAPH_HH */
