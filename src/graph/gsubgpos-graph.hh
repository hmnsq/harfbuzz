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

#شامل "graph.hh"
#شامل "../hb-ot-layout-gsubgpos.hh"
#شامل "../OT/Layout/GSUB/ExtensionSubst.hh"
#شامل "../OT/Layout/GSUB/SubstLookupSubTable.hh"
#شامل "gsubgpos-context.hh"
#شامل "pairpos-graph.hh"
#شامل "markbasepos-graph.hh"
#شامل "ligature-graph.hh"

#ifndef GRAPH_GSUBGPOS_GRAPH_HH
#define GRAPH_GSUBGPOS_GRAPH_HH

فضاینام graph {

ساختار Lookup;

template<typename T>
ساختار ExtensionFormat1 : عمومی OT::ExtensionFormat1<T>
{
  تهی reset(unsigned type)
  {
    این->format = 1;
    این->extensionLookupType = type;
    این->extensionOffset = 0;
  }

  graph_result_t<تهی> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size ();
    اگر (unlikely (vertex_len < OT::ExtensionFormat1<T>::static_size))
      بازگشت Err(SANITIZE_FAILURE);
    بازگشت Ok();
  }

  unsigned get_lookup_type () ثابت
  {
    بازگشت این->extensionLookupType;
  }

  graph_result_t<unsigned> get_subtable_index (graph_t& graph, unsigned this_index) ثابت
  {
    بازگشت graph.index_for_offset (this_index, &این->extensionOffset);
  }
};

ساختار Lookup : عمومی OT::Lookup
{
  unsigned عدد_of_subtables () ثابت
  {
    بازگشت subTable.len;
  }

  graph_result_t<تهی> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size ();
    اگر (unlikely (vertex_len < OT::Lookup::min_size)) بازگشت Err(SANITIZE_FAILURE);
    hb_barrier ();
    اگر (unlikely (vertex_len < این->get_size ())) بازگشت Err(SANITIZE_FAILURE);
    بازگشت Ok();
  }

  منطقی is_extension (hb_tag_t table_tag) ثابت
  {
    بازگشت lookupType == extension_type (table_tag);
  }

  منطقی use_mark_filtering_set () ثابت
  {
    unsigned flag = lookupFlag;
    بازگشت flag & 0x0010u;
  }

  graph_result_t<تهی> make_extension (gsubgpos_graph_context_t& c,
                                       unsigned this_index)
  {
    unsigned type = lookupType;
    unsigned ext_type = extension_type (c.table_tag);
    اگر (!ext_type || is_extension (c.table_tag))
    {
      // NOOP
      بازگشت Ok();
    }

    DEBUG_پیام (SUBSET_REPACK, nullptr,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_index);

    hb_set_t seen;
    hb_vector_t<unsigned> subtables;
    برای (unsigned i = 0; i < subTable.len; i++)
    {
      TRY_ASSIGN (unsigned subtable_index, c.graph.index_for_offset (this_index, &subTable[i]));
      اگر (seen.has (subtable_index))
        ادامه;
      seen.add (subtable_index);
      subtables.push (subtable_index);
    }
    TRY (graph_result_t<تهی>::from (seen, ALLOCATION_FAILURE));
    TRY (graph_result_t<تهی>::from (subtables, ALLOCATION_FAILURE));

    برای (unsigned subtable_index : subtables)
      TRY (make_subtable_extension (c, this_index, subtable_index));

    lookupType = ext_type;
    بازگشت Ok();
  }

  graph_result_t<تهی> split_subtables_if_needed (gsubgpos_graph_context_t& c,
                                                  unsigned this_index)
  {
    unsigned type = lookupType;
    منطقی is_ext = is_extension (c.table_tag);

    اگر (c.table_tag != HB_OT_TAG_GPOS && c.table_tag != HB_OT_TAG_GSUB)
      بازگشت Ok();

    اگر (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      بازگشت Ok();

    hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>> all_new_subtables;
    برای (unsigned i = 0; i < subTable.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      auto idx_res = c.graph.index_for_offset (this_index, &subTable[i]);
      اگر (!idx_res.is_ok ()) ادامه;
      unsigned subtable_index = *idx_res;
      اگر (is_ext) {
        unsigned ext_subtable_index = subtable_index;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.graph.object (ext_subtable_index).head;
        اگر (!extension || !extension->sanitize (c.graph.vertices_[ext_subtable_index]).is_ok ())
          ادامه;

        TRY_ASSIGN (subtable_index, extension->get_subtable_index (c.graph, ext_subtable_index));
        type = extension->get_lookup_type ();
        اگر (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          ادامه;
      }

      hb_vector_t<unsigned>* split_result;
      اگر (c.split_subtables.has (subtable_index, &split_result))
      {
        اگر (split_result->length == 0)
          ادامه;
        all_new_subtables.push (hb_pair(i, *split_result));
        TRY (graph_result_t<تهی>::from (all_new_subtables, ALLOCATION_FAILURE));
      }
      وگرنه
      {
        hb_vector_t<unsigned> new_sub_tables;

        اگر (c.table_tag == HB_OT_TAG_GPOS) {
          انتخاب (type)
          {
          حالت 2:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<PairPos> (c, subtable_index));
              بشکن;
            }
          حالت 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<MarkBasePos> (c, subtable_index));
              بشکن;
            }
          پیشفرض:
            بشکن;
          }
        } وگرنه اگر (c.table_tag == HB_OT_TAG_GSUB) {
          انتخاب (type)
          {
          حالت 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<graph::LigatureSubst> (c, subtable_index));
              بشکن;
            }
          پیشفرض:
            بشکن;
          }
        }

        c.split_subtables.set (subtable_index, new_sub_tables);
        TRY (graph_result_t<تهی>::from (c.split_subtables, ALLOCATION_FAILURE));
        اگر (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (graph_result_t<تهی>::from (all_new_subtables, ALLOCATION_FAILURE));
        }
      }
    }

    اگر (all_new_subtables) {
      TRY (add_sub_tables (c, this_index, type, all_new_subtables));
    }

    بازگشت Ok();
  }

  template<typename T>
  graph_result_t<hb_vector_t<unsigned>> split_subtable (gsubgpos_graph_context_t& c,
                                                        unsigned objidx)
  {
    T* sub_table = (T*) c.graph.object (objidx).head;
    اگر (!sub_table || !sub_table->sanitize (c.graph.vertices_[objidx]).is_ok ())
      بازگشت Ok(hb_vector_t<unsigned> ());

    بازگشت sub_table->split_subtables (c, objidx);
  }

  graph_result_t<تهی> add_sub_tables (gsubgpos_graph_context_t& c,
                                       unsigned this_index,
                                       unsigned type,
                                       ثابت hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    منطقی is_ext = is_extension (c.table_tag);
    auto* v = &c.graph.vertices_[this_index];
    fix_existing_subtable_links (c, this_index, subtable_ids);

    unsigned new_subtable_شمار = 0;
    برای (ثابت auto& p : subtable_ids)
      new_subtable_شمار += p.second.length;

    size_t new_size = v->table_size ()
                      + new_subtable_شمار * OT::Offset16::static_size;
    نویسه* buffer = (نویسه*) hb_calloc (1, new_size);
    اگر (unlikely (!buffer)) بازگشت Err(ALLOCATION_FAILURE);
    auto res = c.add_buffer (buffer);
    اگر (unlikely (!res.is_ok ()))
    {
      hb_free (buffer);
      بازگشت res;
    }
    hb_memcpy (buffer, v->obj ().head, v->table_size());

    اگر (use_mark_filtering_set ())
      hb_memcpy (buffer + new_size - 2, v->obj ().tail - 2, 2);

    v->set_buffer (buffer, buffer + new_size);

    Lookup* new_lookup = (Lookup*) buffer;

    unsigned shift = 0;
    new_lookup->subTable.len = subTable.len + new_subtable_شمار;
    برای (ثابت auto& p : subtable_ids)
    {
      unsigned offset_index = p.first + shift + 1;
      shift += p.second.length;

      برای (unsigned subtable_id : p.second)
      {
        اگر (is_ext)
        {
          TRY_ASSIGN (unsigned ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.graph.vertices_[subtable_id].add_parent (ext_id, نادرست));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.graph.vertices_[this_index];
        }

        TRY(v->add_real_link(2, subtable_id,
          (نویسه*) &new_lookup->subTable[offset_index++] - (نویسه*) new_lookup));
        TRY(c.graph.vertices_[subtable_id].add_parent (this_index, نادرست));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->sort_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.set (this_index, new_lookup);
    TRY (graph_result_t<تهی>::from (c.lookups, ALLOCATION_FAILURE));
    بازگشت Ok();
  }

  تهی fix_existing_subtable_links (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    ثابت hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    auto& v = c.graph.vertices_[this_index];
    unsigned shift = 0;
    برای (ثابت auto& p : subtable_ids)
    {
      unsigned درج_index = p.first + shift;
      unsigned pos_offset = p.second.length * OT::Offset16::static_size;
      unsigned درج_offset = Lookup::min_size + درج_index * OT::Offset16::static_size;
      shift += p.second.length;

      برای (auto& l : v.all_links_writer ())
      {
        اگر (l.position > درج_offset) l.position += pos_offset;
      }
    }
  }

  graph_result_t<unsigned> create_extension_subtable (gsubgpos_graph_context_t& c,
                                                      unsigned subtable_index,
                                                      unsigned type)
  {
    unsigned extension_size = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_size;

    TRY_ASSIGN (unsigned ext_index, c.create_گره (extension_size));

    auto& ext_vertex = c.graph.vertices_[ext_index];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_vertex.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_vertex.add_real_link (4, subtable_index, 4));

    بازگشت Ok(ext_index);
  }

  graph_result_t<تهی> make_subtable_extension (gsubgpos_graph_context_t& c,
                                                unsigned lookup_index,
                                                unsigned subtable_index)
  {
    unsigned type = lookupType;
    unsigned ext_index = HB_GRAPH_INVALID;
    unsigned* existing_ext_index = nullptr;
    اگر (c.subtable_to_extension.has(subtable_index, &existing_ext_index)) {
      ext_index = *existing_ext_index;
    } وگرنه {
      TRY_ASSIGN (ext_index, create_extension_subtable(c, subtable_index, type));
      c.subtable_to_extension.set(subtable_index, ext_index);
      TRY (graph_result_t<تهی>::from (c.subtable_to_extension, ALLOCATION_FAILURE));
    }

    auto& subtable_vertex = c.graph.vertices_[subtable_index];
    auto& lookup_vertex = c.graph.vertices_[lookup_index];
    unsigned replaced = 0;
    برای (auto& l : lookup_vertex.real_links_writer ())
    {
      اگر (l.objidx == subtable_index) {
        // Change lookup to point at the extension.
        l.objidx = ext_index;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    auto& ext_vertex = c.graph.vertices_[ext_index];
    برای (unsigned i = 0; i < replaced; i++)
    {
      subtable_vertex.remove_parent (lookup_index);
      TRY(ext_vertex.add_parent (lookup_index, نادرست));
    }
    اگر (!existing_ext_index)
      TRY(subtable_vertex.add_parent (ext_index, نادرست));

    بازگشت Ok();
  }

 خصوصی:
  منطقی is_supported_gsub_type(unsigned type, gsubgpos_graph_context_t& c) ثابت {
    بازگشت (c.table_tag == HB_OT_TAG_GSUB) && (
      type == OT::Layout::GSUB_impl::SubstLookupSubTable::Type::Ligature
    );
  }

  منطقی is_supported_gpos_type(unsigned type, gsubgpos_graph_context_t& c) ثابت {
   بازگشت (c.table_tag == HB_OT_TAG_GPOS) && (
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::Pair ||
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::MarkBase
    );
  }

  unsigned extension_type (hb_tag_t table_tag) ثابت
  {
    انتخاب (table_tag)
    {
    حالت HB_OT_TAG_GPOS: بازگشت 9;
    حالت HB_OT_TAG_GSUB: بازگشت 7;
    پیشفرض: بازگشت 0;
    }
  }
};

template <typename T>
ساختار LookupList : عمومی OT::LookupList<T>
{
  graph_result_t<تهی> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size ();
    اگر (unlikely (vertex_len < OT::LookupList<T>::min_size)) بازگشت Err(SANITIZE_FAILURE);
    hb_barrier ();
    اگر (unlikely (vertex_len < OT::LookupList<T>::item_size * این->len)) بازگشت Err(SANITIZE_FAILURE);
    بازگشت Ok();
  }
};

ساختار GSTAR : عمومی OT::GSUBGPOS
{
  ایستا graph_result_t<ثابت GSTAR*> graph_to_gstar (graph_t& graph)
  {
    ثابت auto& r = graph.ریشه ();

    ثابت GSTAR* gstar = (GSTAR*) r.obj ().head;
    اگر (unlikely (!gstar)) بازگشت Err(INVALID_ARGUMENT);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    بازگشت gstar;
  }

  ثابت تهی* get_lookup_list_field_offset () ثابت
  {
    بازگشت OT::GSUBGPOS::get_lookup_list_field_offset ();
  }

  graph_result_t<تهی> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t len = vertex.table_size ();
    اگر (unlikely (len < OT::GSUBGPOS::min_size)) بازگشت Err(SANITIZE_FAILURE);
    hb_barrier ();
    اگر (unlikely (version.major != 1)) بازگشت Err(SANITIZE_FAILURE);
    اگر (unlikely (len < get_size ())) بازگشت Err(SANITIZE_FAILURE);
    بازگشت Ok();
  }

  graph_result_t<تهی> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010002u && lookupList2)
      بازگشت find_lookups<MediumTypes> (graph, lookups);
#endif
    TRY(find_lookups<SmallTypes> (graph, lookups));
    بازگشت Ok();
  }

  graph_result_t<unsigned> get_lookup_list_index (graph_t& graph) ثابت
  {
    ثابت تهی* offset = get_lookup_list_field_offset ();
    اگر (unlikely (!offset)) بازگشت Err(INVALID_ARGUMENT);
    بازگشت graph.index_for_offset (graph.ریشه_idx (), offset);
  }

  template<typename Types>
  graph_result_t<تهی> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) ثابت
  {
    TRY_ASSIGN (unsigned lookup_list_idx, get_lookup_list_index (graph));
    ثابت LookupList<Types>* lookupList =
        (ثابت LookupList<Types>*) graph.object (lookup_list_idx).head;
    اگر (unlikely (!lookupList)) بازگشت Err(INVALID_ARGUMENT);
    TRY(lookupList->sanitize (graph.vertices_[lookup_list_idx]));

    برای (unsigned i = 0; i < lookupList->len; i++)
    {
      TRY_ASSIGN (unsigned lookup_idx, graph.index_for_offset (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) graph.object (lookup_idx).head;
      اگر (!lookup || !lookup->sanitize (graph.vertices_[lookup_idx]).is_ok ()) ادامه;
      lookups.set (lookup_idx, lookup);
    }
    بازگشت Ok();
  }
};




}

#endif  /* GRAPH_GSUBGPOS_GRAPH_HH */
