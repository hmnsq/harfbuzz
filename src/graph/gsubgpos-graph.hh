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

#अन्तर्भाव "graph.hh"
#अन्तर्भाव "../hb-ot-layout-gsubgpos.hh"
#अन्तर्भाव "../OT/Layout/GSUB/ExtensionSubst.hh"
#अन्तर्भाव "../OT/Layout/GSUB/SubstLookupSubTable.hh"
#अन्तर्भाव "gsubgpos-context.hh"
#अन्तर्भाव "pairpos-graph.hh"
#अन्तर्भाव "markbasepos-graph.hh"
#अन्तर्भाव "ligature-graph.hh"

#ifndef GRAPH_GSUBGPOS_GRAPH_HH
#define GRAPH_GSUBGPOS_GRAPH_HH

नामाकाश graph {

संरचना Lookup;

template<typename T>
संरचना ExtensionFormat1 : सार्वजनिक OT::ExtensionFormat1<T>
{
  शून्य reset(unsigned type)
  {
    एतत्->format = 1;
    एतत्->extensionLookupType = type;
    एतत्->extensionOffset = 0;
  }

  graph_result_t<शून्य> sanitize (ध्रुव graph_t::vertex_t& vertex) ध्रुव
  {
    size_t vertex_len = vertex.table_size ();
    सचे (unlikely (vertex_len < OT::ExtensionFormat1<T>::static_size))
      पटिनिवत्त Err(SANITIZE_FAILURE);
    पटिनिवत्त Ok();
  }

  unsigned get_lookup_type () ध्रुव
  {
    पटिनिवत्त एतत्->extensionLookupType;
  }

  graph_result_t<unsigned> get_subtable_index (graph_t& graph, unsigned this_index) ध्रुव
  {
    पटिनिवत्त graph.index_for_offset (this_index, &एतत्->extensionOffset);
  }
};

संरचना Lookup : सार्वजनिक OT::Lookup
{
  unsigned सङ्ख्या_of_subtables () ध्रुव
  {
    पटिनिवत्त subTable.len;
  }

  graph_result_t<शून्य> sanitize (ध्रुव graph_t::vertex_t& vertex) ध्रुव
  {
    size_t vertex_len = vertex.table_size ();
    सचे (unlikely (vertex_len < OT::Lookup::min_size)) पटिनिवत्त Err(SANITIZE_FAILURE);
    hb_barrier ();
    सचे (unlikely (vertex_len < एतत्->get_size ())) पटिनिवत्त Err(SANITIZE_FAILURE);
    पटिनिवत्त Ok();
  }

  तार्किक is_extension (hb_tag_t table_tag) ध्रुव
  {
    पटिनिवत्त lookupType == extension_type (table_tag);
  }

  तार्किक use_mark_filtering_set () ध्रुव
  {
    unsigned flag = lookupFlag;
    पटिनिवत्त flag & 0x0010u;
  }

  graph_result_t<शून्य> make_extension (gsubgpos_graph_context_t& c,
                                       unsigned this_index)
  {
    unsigned type = lookupType;
    unsigned ext_type = extension_type (c.table_tag);
    सचे (!ext_type || is_extension (c.table_tag))
    {
      // NOOP
      पटिनिवत्त Ok();
    }

    DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_index);

    hb_set_t seen;
    hb_vector_t<unsigned> subtables;
    अत्थाय (unsigned i = 0; i < subTable.len; i++)
    {
      TRY_ASSIGN (unsigned subtable_index, c.graph.index_for_offset (this_index, &subTable[i]));
      सचे (seen.has (subtable_index))
        अनुवर्त;
      seen.add (subtable_index);
      subtables.push (subtable_index);
    }
    TRY (graph_result_t<शून्य>::from (seen, ALLOCATION_FAILURE));
    TRY (graph_result_t<शून्य>::from (subtables, ALLOCATION_FAILURE));

    अत्थाय (unsigned subtable_index : subtables)
      TRY (make_subtable_extension (c, this_index, subtable_index));

    lookupType = ext_type;
    पटिनिवत्त Ok();
  }

  graph_result_t<शून्य> split_subtables_if_needed (gsubgpos_graph_context_t& c,
                                                  unsigned this_index)
  {
    unsigned type = lookupType;
    तार्किक is_ext = is_extension (c.table_tag);

    सचे (c.table_tag != HB_OT_TAG_GPOS && c.table_tag != HB_OT_TAG_GSUB)
      पटिनिवत्त Ok();

    सचे (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      पटिनिवत्त Ok();

    hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>> all_new_subtables;
    अत्थाय (unsigned i = 0; i < subTable.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      auto idx_res = c.graph.index_for_offset (this_index, &subTable[i]);
      सचे (!idx_res.is_ok ()) अनुवर्त;
      unsigned subtable_index = *idx_res;
      सचे (is_ext) {
        unsigned ext_subtable_index = subtable_index;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.graph.object (ext_subtable_index).head;
        सचे (!extension || !extension->sanitize (c.graph.vertices_[ext_subtable_index]).is_ok ())
          अनुवर्त;

        TRY_ASSIGN (subtable_index, extension->get_subtable_index (c.graph, ext_subtable_index));
        type = extension->get_lookup_type ();
        सचे (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          अनुवर्त;
      }

      hb_vector_t<unsigned>* split_result;
      सचे (c.split_subtables.has (subtable_index, &split_result))
      {
        सचे (split_result->length == 0)
          अनुवर्त;
        all_new_subtables.push (hb_pair(i, *split_result));
        TRY (graph_result_t<शून्य>::from (all_new_subtables, ALLOCATION_FAILURE));
      }
      अञ्ञथा
      {
        hb_vector_t<unsigned> new_sub_tables;

        सचे (c.table_tag == HB_OT_TAG_GPOS) {
          विकल्प (type)
          {
          पक्ष 2:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<PairPos> (c, subtable_index));
              भङ्ग;
            }
          पक्ष 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<MarkBasePos> (c, subtable_index));
              भङ्ग;
            }
          स्वभाव:
            भङ्ग;
          }
        } अञ्ञथा सचे (c.table_tag == HB_OT_TAG_GSUB) {
          विकल्प (type)
          {
          पक्ष 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<graph::LigatureSubst> (c, subtable_index));
              भङ्ग;
            }
          स्वभाव:
            भङ्ग;
          }
        }

        c.split_subtables.set (subtable_index, new_sub_tables);
        TRY (graph_result_t<शून्य>::from (c.split_subtables, ALLOCATION_FAILURE));
        सचे (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (graph_result_t<शून्य>::from (all_new_subtables, ALLOCATION_FAILURE));
        }
      }
    }

    सचे (all_new_subtables) {
      TRY (add_sub_tables (c, this_index, type, all_new_subtables));
    }

    पटिनिवत्त Ok();
  }

  template<typename T>
  graph_result_t<hb_vector_t<unsigned>> split_subtable (gsubgpos_graph_context_t& c,
                                                        unsigned objidx)
  {
    T* sub_table = (T*) c.graph.object (objidx).head;
    सचे (!sub_table || !sub_table->sanitize (c.graph.vertices_[objidx]).is_ok ())
      पटिनिवत्त Ok(hb_vector_t<unsigned> ());

    पटिनिवत्त sub_table->split_subtables (c, objidx);
  }

  graph_result_t<शून्य> add_sub_tables (gsubgpos_graph_context_t& c,
                                       unsigned this_index,
                                       unsigned type,
                                       ध्रुव hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    तार्किक is_ext = is_extension (c.table_tag);
    auto* v = &c.graph.vertices_[this_index];
    fix_existing_subtable_links (c, this_index, subtable_ids);

    unsigned new_subtable_गणनम् = 0;
    अत्थाय (ध्रुव auto& p : subtable_ids)
      new_subtable_गणनम् += p.second.length;

    size_t new_size = v->table_size ()
                      + new_subtable_गणनम् * OT::Offset16::static_size;
    वर्ण* buffer = (वर्ण*) hb_calloc (1, new_size);
    सचे (unlikely (!buffer)) पटिनिवत्त Err(ALLOCATION_FAILURE);
    auto res = c.add_buffer (buffer);
    सचे (unlikely (!res.is_ok ()))
    {
      hb_free (buffer);
      पटिनिवत्त res;
    }
    hb_memcpy (buffer, v->obj ().head, v->table_size());

    सचे (use_mark_filtering_set ())
      hb_memcpy (buffer + new_size - 2, v->obj ().tail - 2, 2);

    v->set_buffer (buffer, buffer + new_size);

    Lookup* new_lookup = (Lookup*) buffer;

    unsigned shift = 0;
    new_lookup->subTable.len = subTable.len + new_subtable_गणनम्;
    अत्थाय (ध्रुव auto& p : subtable_ids)
    {
      unsigned offset_index = p.first + shift + 1;
      shift += p.second.length;

      अत्थाय (unsigned subtable_id : p.second)
      {
        सचे (is_ext)
        {
          TRY_ASSIGN (unsigned ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.graph.vertices_[subtable_id].add_parent (ext_id, मिच्छा));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.graph.vertices_[this_index];
        }

        TRY(v->add_real_link(2, subtable_id,
          (वर्ण*) &new_lookup->subTable[offset_index++] - (वर्ण*) new_lookup));
        TRY(c.graph.vertices_[subtable_id].add_parent (this_index, मिच्छा));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->sort_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.set (this_index, new_lookup);
    TRY (graph_result_t<शून्य>::from (c.lookups, ALLOCATION_FAILURE));
    पटिनिवत्त Ok();
  }

  शून्य fix_existing_subtable_links (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    ध्रुव hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    auto& v = c.graph.vertices_[this_index];
    unsigned shift = 0;
    अत्थाय (ध्रुव auto& p : subtable_ids)
    {
      unsigned निवेशय_index = p.first + shift;
      unsigned pos_offset = p.second.length * OT::Offset16::static_size;
      unsigned निवेशय_offset = Lookup::min_size + निवेशय_index * OT::Offset16::static_size;
      shift += p.second.length;

      अत्थाय (auto& l : v.all_links_writer ())
      {
        सचे (l.position > निवेशय_offset) l.position += pos_offset;
      }
    }
  }

  graph_result_t<unsigned> create_extension_subtable (gsubgpos_graph_context_t& c,
                                                      unsigned subtable_index,
                                                      unsigned type)
  {
    unsigned extension_size = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_size;

    TRY_ASSIGN (unsigned ext_index, c.create_ग्रन्थिः (extension_size));

    auto& ext_vertex = c.graph.vertices_[ext_index];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_vertex.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_vertex.add_real_link (4, subtable_index, 4));

    पटिनिवत्त Ok(ext_index);
  }

  graph_result_t<शून्य> make_subtable_extension (gsubgpos_graph_context_t& c,
                                                unsigned lookup_index,
                                                unsigned subtable_index)
  {
    unsigned type = lookupType;
    unsigned ext_index = HB_GRAPH_INVALID;
    unsigned* existing_ext_index = nullptr;
    सचे (c.subtable_to_extension.has(subtable_index, &existing_ext_index)) {
      ext_index = *existing_ext_index;
    } अञ्ञथा {
      TRY_ASSIGN (ext_index, create_extension_subtable(c, subtable_index, type));
      c.subtable_to_extension.set(subtable_index, ext_index);
      TRY (graph_result_t<शून्य>::from (c.subtable_to_extension, ALLOCATION_FAILURE));
    }

    auto& subtable_vertex = c.graph.vertices_[subtable_index];
    auto& lookup_vertex = c.graph.vertices_[lookup_index];
    unsigned replaced = 0;
    अत्थाय (auto& l : lookup_vertex.real_links_writer ())
    {
      सचे (l.objidx == subtable_index) {
        // Change lookup to point at the extension.
        l.objidx = ext_index;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    auto& ext_vertex = c.graph.vertices_[ext_index];
    अत्थाय (unsigned i = 0; i < replaced; i++)
    {
      subtable_vertex.remove_parent (lookup_index);
      TRY(ext_vertex.add_parent (lookup_index, मिच्छा));
    }
    सचे (!existing_ext_index)
      TRY(subtable_vertex.add_parent (ext_index, मिच्छा));

    पटिनिवत्त Ok();
  }

 गुप्त:
  तार्किक is_supported_gsub_type(unsigned type, gsubgpos_graph_context_t& c) ध्रुव {
    पटिनिवत्त (c.table_tag == HB_OT_TAG_GSUB) && (
      type == OT::Layout::GSUB_impl::SubstLookupSubTable::Type::Ligature
    );
  }

  तार्किक is_supported_gpos_type(unsigned type, gsubgpos_graph_context_t& c) ध्रुव {
   पटिनिवत्त (c.table_tag == HB_OT_TAG_GPOS) && (
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::Pair ||
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::MarkBase
    );
  }

  unsigned extension_type (hb_tag_t table_tag) ध्रुव
  {
    विकल्प (table_tag)
    {
    पक्ष HB_OT_TAG_GPOS: पटिनिवत्त 9;
    पक्ष HB_OT_TAG_GSUB: पटिनिवत्त 7;
    स्वभाव: पटिनिवत्त 0;
    }
  }
};

template <typename T>
संरचना LookupList : सार्वजनिक OT::LookupList<T>
{
  graph_result_t<शून्य> sanitize (ध्रुव graph_t::vertex_t& vertex) ध्रुव
  {
    size_t vertex_len = vertex.table_size ();
    सचे (unlikely (vertex_len < OT::LookupList<T>::min_size)) पटिनिवत्त Err(SANITIZE_FAILURE);
    hb_barrier ();
    सचे (unlikely (vertex_len < OT::LookupList<T>::item_size * एतत्->len)) पटिनिवत्त Err(SANITIZE_FAILURE);
    पटिनिवत्त Ok();
  }
};

संरचना GSTAR : सार्वजनिक OT::GSUBGPOS
{
  स्थिर graph_result_t<ध्रुव GSTAR*> graph_to_gstar (graph_t& graph)
  {
    ध्रुव auto& r = graph.मूलम् ();

    ध्रुव GSTAR* gstar = (GSTAR*) r.obj ().head;
    सचे (unlikely (!gstar)) पटिनिवत्त Err(INVALID_ARGUMENT);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    पटिनिवत्त gstar;
  }

  ध्रुव शून्य* get_lookup_list_field_offset () ध्रुव
  {
    पटिनिवत्त OT::GSUBGPOS::get_lookup_list_field_offset ();
  }

  graph_result_t<शून्य> sanitize (ध्रुव graph_t::vertex_t& vertex) ध्रुव
  {
    size_t len = vertex.table_size ();
    सचे (unlikely (len < OT::GSUBGPOS::min_size)) पटिनिवत्त Err(SANITIZE_FAILURE);
    hb_barrier ();
    सचे (unlikely (version.major != 1)) पटिनिवत्त Err(SANITIZE_FAILURE);
    सचे (unlikely (len < get_size ())) पटिनिवत्त Err(SANITIZE_FAILURE);
    पटिनिवत्त Ok();
  }

  graph_result_t<शून्य> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    सचे (version.to_int () >= 0x00010002u && lookupList2)
      पटिनिवत्त find_lookups<MediumTypes> (graph, lookups);
#endif
    TRY(find_lookups<SmallTypes> (graph, lookups));
    पटिनिवत्त Ok();
  }

  graph_result_t<unsigned> get_lookup_list_index (graph_t& graph) ध्रुव
  {
    ध्रुव शून्य* offset = get_lookup_list_field_offset ();
    सचे (unlikely (!offset)) पटिनिवत्त Err(INVALID_ARGUMENT);
    पटिनिवत्त graph.index_for_offset (graph.मूलम्_idx (), offset);
  }

  template<typename Types>
  graph_result_t<शून्य> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) ध्रुव
  {
    TRY_ASSIGN (unsigned lookup_list_idx, get_lookup_list_index (graph));
    ध्रुव LookupList<Types>* lookupList =
        (ध्रुव LookupList<Types>*) graph.object (lookup_list_idx).head;
    सचे (unlikely (!lookupList)) पटिनिवत्त Err(INVALID_ARGUMENT);
    TRY(lookupList->sanitize (graph.vertices_[lookup_list_idx]));

    अत्थाय (unsigned i = 0; i < lookupList->len; i++)
    {
      TRY_ASSIGN (unsigned lookup_idx, graph.index_for_offset (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) graph.object (lookup_idx).head;
      सचे (!lookup || !lookup->sanitize (graph.vertices_[lookup_idx]).is_ok ()) अनुवर्त;
      lookups.set (lookup_idx, lookup);
    }
    पटिनिवत्त Ok();
  }
};




}

#endif  /* GRAPH_GSUBGPOS_GRAPH_HH */
