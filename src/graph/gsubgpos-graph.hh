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

#ضمن "graph.hh"
#ضمن "../hb-ot-layout-gsubgpos.hh"
#ضمن "../OT/Layout/GSUB/ExtensionSubst.hh"
#ضمن "../OT/Layout/GSUB/SubstLookupSubTable.hh"
#ضمن "gsubgpos-context.hh"
#ضمن "pairpos-graph.hh"
#ضمن "markbasepos-graph.hh"
#ضمن "ligature-graph.hh"

#ifndef GRAPH_GSUBGPOS_GRAPH_HH
#define GRAPH_GSUBGPOS_GRAPH_HH

نطاق graph {

بنية Lookup;

template<typename T>
بنية ExtensionFormat1 : عام OT::ExtensionFormat1<T>
{
  فراغ reset(unsigned type)
  {
    هذا->format = 1;
    هذا->extensionLookupType = type;
    هذا->extensionOffset = 0;
  }

  graph_result_t<فراغ> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size ();
    إذا (unlikely (vertex_len < OT::ExtensionFormat1<T>::static_size))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  unsigned get_lookup_type () ثابت
  {
    أرجع هذا->extensionLookupType;
  }

  graph_result_t<unsigned> get_subtable_index (graph_t& graph, unsigned this_index) ثابت
  {
    أرجع graph.index_for_offset (this_index, &هذا->extensionOffset);
  }
};

بنية Lookup : عام OT::Lookup
{
  unsigned لمب_of_subtables () ثابت
  {
    أرجع subTable.len;
  }

  graph_result_t<فراغ> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size ();
    إذا (unlikely (vertex_len < OT::Lookup::min_size)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (vertex_len < هذا->get_size ())) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  منطقي is_extension (hb_tag_t table_tag) ثابت
  {
    أرجع lookupType == extension_type (table_tag);
  }

  منطقي use_mark_filtering_set () ثابت
  {
    unsigned flag = lookupFlag;
    أرجع flag & 0x0010u;
  }

  graph_result_t<فراغ> make_extension (gsubgpos_graph_context_t& c,
                                       unsigned this_index)
  {
    unsigned type = lookupType;
    unsigned ext_type = extension_type (c.table_tag);
    إذا (!ext_type || is_extension (c.table_tag))
    {
      // NOOP
      أرجع Ok();
    }

    DEBUG_سكو (SUBSET_REPACK, nullptr,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_index);

    hb_set_t seen;
    hb_vector_t<unsigned> subtables;
    لكل (unsigned i = 0; i < subTable.len; i++)
    {
      TRY_ASSIGN (unsigned subtable_index, c.graph.index_for_offset (this_index, &subTable[i]));
      إذا (seen.has (subtable_index))
        استمر;
      seen.add (subtable_index);
      subtables.push (subtable_index);
    }
    TRY (graph_result_t<فراغ>::from (seen, ALLOCATION_FAILURE));
    TRY (graph_result_t<فراغ>::from (subtables, ALLOCATION_FAILURE));

    لكل (unsigned subtable_index : subtables)
      TRY (make_subtable_extension (c, this_index, subtable_index));

    lookupType = ext_type;
    أرجع Ok();
  }

  graph_result_t<فراغ> split_subtables_if_needed (gsubgpos_graph_context_t& c,
                                                  unsigned this_index)
  {
    unsigned type = lookupType;
    منطقي is_ext = is_extension (c.table_tag);

    إذا (c.table_tag != HB_OT_TAG_GPOS && c.table_tag != HB_OT_TAG_GSUB)
      أرجع Ok();

    إذا (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      أرجع Ok();

    hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>> all_new_subtables;
    لكل (unsigned i = 0; i < subTable.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      auto idx_res = c.graph.index_for_offset (this_index, &subTable[i]);
      إذا (!idx_res.is_ok ()) استمر;
      unsigned subtable_index = *idx_res;
      إذا (is_ext) {
        unsigned ext_subtable_index = subtable_index;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.graph.object (ext_subtable_index).head;
        إذا (!extension || !extension->sanitize (c.graph.vertices_[ext_subtable_index]).is_ok ())
          استمر;

        TRY_ASSIGN (subtable_index, extension->get_subtable_index (c.graph, ext_subtable_index));
        type = extension->get_lookup_type ();
        إذا (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          استمر;
      }

      hb_vector_t<unsigned>* split_result;
      إذا (c.split_subtables.has (subtable_index, &split_result))
      {
        إذا (split_result->length == 0)
          استمر;
        all_new_subtables.push (hb_pair(i, *split_result));
        TRY (graph_result_t<فراغ>::from (all_new_subtables, ALLOCATION_FAILURE));
      }
      وإلا
      {
        hb_vector_t<unsigned> new_sub_tables;

        إذا (c.table_tag == HB_OT_TAG_GPOS) {
          بدل (type)
          {
          حالة 2:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<PairPos> (c, subtable_index));
              اكسر;
            }
          حالة 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<MarkBasePos> (c, subtable_index));
              اكسر;
            }
          افتراضي:
            اكسر;
          }
        } وإلا إذا (c.table_tag == HB_OT_TAG_GSUB) {
          بدل (type)
          {
          حالة 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<graph::LigatureSubst> (c, subtable_index));
              اكسر;
            }
          افتراضي:
            اكسر;
          }
        }

        c.split_subtables.set (subtable_index, new_sub_tables);
        TRY (graph_result_t<فراغ>::from (c.split_subtables, ALLOCATION_FAILURE));
        إذا (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (graph_result_t<فراغ>::from (all_new_subtables, ALLOCATION_FAILURE));
        }
      }
    }

    إذا (all_new_subtables) {
      TRY (add_sub_tables (c, this_index, type, all_new_subtables));
    }

    أرجع Ok();
  }

  template<typename T>
  graph_result_t<hb_vector_t<unsigned>> split_subtable (gsubgpos_graph_context_t& c,
                                                        unsigned objidx)
  {
    T* sub_table = (T*) c.graph.object (objidx).head;
    إذا (!sub_table || !sub_table->sanitize (c.graph.vertices_[objidx]).is_ok ())
      أرجع Ok(hb_vector_t<unsigned> ());

    أرجع sub_table->split_subtables (c, objidx);
  }

  graph_result_t<فراغ> add_sub_tables (gsubgpos_graph_context_t& c,
                                       unsigned this_index,
                                       unsigned type,
                                       ثابت hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    منطقي is_ext = is_extension (c.table_tag);
    auto* v = &c.graph.vertices_[this_index];
    fix_existing_subtable_links (c, this_index, subtable_ids);

    unsigned new_subtable_كدي = 0;
    لكل (ثابت auto& p : subtable_ids)
      new_subtable_كدي += p.second.length;

    size_t new_size = v->table_size ()
                      + new_subtable_كدي * OT::Offset16::static_size;
    حرف* buffer = (حرف*) hb_calloc (1, new_size);
    إذا (unlikely (!buffer)) أرجع Err(ALLOCATION_FAILURE);
    auto res = c.add_buffer (buffer);
    إذا (unlikely (!res.is_ok ()))
    {
      hb_free (buffer);
      أرجع res;
    }
    hb_memcpy (buffer, v->obj ().head, v->table_size());

    إذا (use_mark_filtering_set ())
      hb_memcpy (buffer + new_size - 2, v->obj ().tail - 2, 2);

    v->set_buffer (buffer, buffer + new_size);

    Lookup* new_lookup = (Lookup*) buffer;

    unsigned shift = 0;
    new_lookup->subTable.len = subTable.len + new_subtable_كدي;
    لكل (ثابت auto& p : subtable_ids)
    {
      unsigned offset_index = p.first + shift + 1;
      shift += p.second.length;

      لكل (unsigned subtable_id : p.second)
      {
        إذا (is_ext)
        {
          TRY_ASSIGN (unsigned ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.graph.vertices_[subtable_id].add_parent (ext_id, خطأ));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.graph.vertices_[this_index];
        }

        TRY(v->add_real_link(2, subtable_id,
          (حرف*) &new_lookup->subTable[offset_index++] - (حرف*) new_lookup));
        TRY(c.graph.vertices_[subtable_id].add_parent (this_index, خطأ));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->sort_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.set (this_index, new_lookup);
    TRY (graph_result_t<فراغ>::from (c.lookups, ALLOCATION_FAILURE));
    أرجع Ok();
  }

  فراغ fix_existing_subtable_links (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    ثابت hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    auto& v = c.graph.vertices_[this_index];
    unsigned shift = 0;
    لكل (ثابت auto& p : subtable_ids)
    {
      unsigned سك_index = p.first + shift;
      unsigned pos_offset = p.second.length * OT::Offset16::static_size;
      unsigned سك_offset = Lookup::min_size + سك_index * OT::Offset16::static_size;
      shift += p.second.length;

      لكل (auto& l : v.all_links_writer ())
      {
        إذا (l.position > سك_offset) l.position += pos_offset;
      }
    }
  }

  graph_result_t<unsigned> create_extension_subtable (gsubgpos_graph_context_t& c,
                                                      unsigned subtable_index,
                                                      unsigned type)
  {
    unsigned extension_size = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_size;

    TRY_ASSIGN (unsigned ext_index, c.create_كلي (extension_size));

    auto& ext_vertex = c.graph.vertices_[ext_index];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_vertex.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_vertex.add_real_link (4, subtable_index, 4));

    أرجع Ok(ext_index);
  }

  graph_result_t<فراغ> make_subtable_extension (gsubgpos_graph_context_t& c,
                                                unsigned lookup_index,
                                                unsigned subtable_index)
  {
    unsigned type = lookupType;
    unsigned ext_index = HB_GRAPH_INVALID;
    unsigned* existing_ext_index = nullptr;
    إذا (c.subtable_to_extension.has(subtable_index, &existing_ext_index)) {
      ext_index = *existing_ext_index;
    } وإلا {
      TRY_ASSIGN (ext_index, create_extension_subtable(c, subtable_index, type));
      c.subtable_to_extension.set(subtable_index, ext_index);
      TRY (graph_result_t<فراغ>::from (c.subtable_to_extension, ALLOCATION_FAILURE));
    }

    auto& subtable_vertex = c.graph.vertices_[subtable_index];
    auto& lookup_vertex = c.graph.vertices_[lookup_index];
    unsigned replaced = 0;
    لكل (auto& l : lookup_vertex.real_links_writer ())
    {
      إذا (l.objidx == subtable_index) {
        // Change lookup to point at the extension.
        l.objidx = ext_index;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    auto& ext_vertex = c.graph.vertices_[ext_index];
    لكل (unsigned i = 0; i < replaced; i++)
    {
      subtable_vertex.remove_parent (lookup_index);
      TRY(ext_vertex.add_parent (lookup_index, خطأ));
    }
    إذا (!existing_ext_index)
      TRY(subtable_vertex.add_parent (ext_index, خطأ));

    أرجع Ok();
  }

 خاص:
  منطقي is_supported_gsub_type(unsigned type, gsubgpos_graph_context_t& c) ثابت {
    أرجع (c.table_tag == HB_OT_TAG_GSUB) && (
      type == OT::Layout::GSUB_impl::SubstLookupSubTable::Type::Ligature
    );
  }

  منطقي is_supported_gpos_type(unsigned type, gsubgpos_graph_context_t& c) ثابت {
   أرجع (c.table_tag == HB_OT_TAG_GPOS) && (
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::Pair ||
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::MarkBase
    );
  }

  unsigned extension_type (hb_tag_t table_tag) ثابت
  {
    بدل (table_tag)
    {
    حالة HB_OT_TAG_GPOS: أرجع 9;
    حالة HB_OT_TAG_GSUB: أرجع 7;
    افتراضي: أرجع 0;
    }
  }
};

template <typename T>
بنية LookupList : عام OT::LookupList<T>
{
  graph_result_t<فراغ> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size ();
    إذا (unlikely (vertex_len < OT::LookupList<T>::min_size)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (vertex_len < OT::LookupList<T>::item_size * هذا->len)) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }
};

بنية GSTAR : عام OT::GSUBGPOS
{
  ساكن graph_result_t<ثابت GSTAR*> graph_to_gstar (graph_t& graph)
  {
    ثابت auto& r = graph.سيو ();

    ثابت GSTAR* gstar = (GSTAR*) r.obj ().head;
    إذا (unlikely (!gstar)) أرجع Err(INVALID_ARGUMENT);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    أرجع gstar;
  }

  ثابت فراغ* get_lookup_list_field_offset () ثابت
  {
    أرجع OT::GSUBGPOS::get_lookup_list_field_offset ();
  }

  graph_result_t<فراغ> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t len = vertex.table_size ();
    إذا (unlikely (len < OT::GSUBGPOS::min_size)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (version.major != 1)) أرجع Err(SANITIZE_FAILURE);
    إذا (unlikely (len < get_size ())) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  graph_result_t<فراغ> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010002u && lookupList2)
      أرجع find_lookups<MediumTypes> (graph, lookups);
#endif
    TRY(find_lookups<SmallTypes> (graph, lookups));
    أرجع Ok();
  }

  graph_result_t<unsigned> get_lookup_list_index (graph_t& graph) ثابت
  {
    ثابت فراغ* offset = get_lookup_list_field_offset ();
    إذا (unlikely (!offset)) أرجع Err(INVALID_ARGUMENT);
    أرجع graph.index_for_offset (graph.سيو_idx (), offset);
  }

  template<typename Types>
  graph_result_t<فراغ> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) ثابت
  {
    TRY_ASSIGN (unsigned lookup_list_idx, get_lookup_list_index (graph));
    ثابت LookupList<Types>* lookupList =
        (ثابت LookupList<Types>*) graph.object (lookup_list_idx).head;
    إذا (unlikely (!lookupList)) أرجع Err(INVALID_ARGUMENT);
    TRY(lookupList->sanitize (graph.vertices_[lookup_list_idx]));

    لكل (unsigned i = 0; i < lookupList->len; i++)
    {
      TRY_ASSIGN (unsigned lookup_idx, graph.index_for_offset (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) graph.object (lookup_idx).head;
      إذا (!lookup || !lookup->sanitize (graph.vertices_[lookup_idx]).is_ok ()) استمر;
      lookups.set (lookup_idx, lookup);
    }
    أرجع Ok();
  }
};




}

#endif  /* GRAPH_GSUBGPOS_GRAPH_HH */
