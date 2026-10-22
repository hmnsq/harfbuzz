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

#include "graph.hh"
#include "../hb-ot-layout-gsubgpos.hh"
#include "../OT/Layout/GSUB/ExtensionSubst.hh"
#include "../OT/Layout/GSUB/SubstLookupSubTable.hh"
#include "gsubgpos-context.hh"
#include "pairpos-graph.hh"
#include "markbasepos-graph.hh"
#include "ligature-graph.hh"

#ifndef GRAPH_GSUBGPOS_GRAPH_HH
#define GRAPH_GSUBGPOS_GRAPH_HH

namespace graph {

sa_nracanaa Lookup;

template<typename T>
sa_nracanaa ExtensionFormat1 : public OT::ExtensionFormat1<T>
{
  void reset(unsigned type)
  {
    this->format = 1;
    this->extensionLookupType = type;
    this->extensionOffset = 0;
  }

  graph_result_t<void> sanitize (const graph_t::vertex_t& vertex) const
  {
    size_t vertex_len = vertex.table_size ();
    ᯀᯮᯝ (unlikely (vertex_len < OT::ExtensionFormat1<T>::static_size))
      ᯀᯮᯞ Err(SANITIZE_FAILURE);
    ᯀᯮᯞ Ok();
  }

  unsigned get_lookup_type () const
  {
    ᯀᯮᯞ this->extensionLookupType;
  }

  graph_result_t<unsigned> get_subtable_index (graph_t& graph, unsigned this_index) const
  {
    ᯀᯮᯞ graph.index_for_offset (this_index, &this->extensionOffset);
  }
};

sa_nracanaa Lookup : public OT::Lookup
{
  unsigned number_of_subtables () const
  {
    ᯀᯮᯞ subTable.len;
  }

  graph_result_t<void> sanitize (const graph_t::vertex_t& vertex) const
  {
    size_t vertex_len = vertex.table_size ();
    ᯀᯮᯝ (unlikely (vertex_len < OT::Lookup::min_size)) ᯀᯮᯞ Err(SANITIZE_FAILURE);
    hb_barrier ();
    ᯀᯮᯝ (unlikely (vertex_len < this->get_size ())) ᯀᯮᯞ Err(SANITIZE_FAILURE);
    ᯀᯮᯞ Ok();
  }

  bool is_extension (hb_tag_t table_tag) const
  {
    ᯀᯮᯞ lookupType == extension_type (table_tag);
  }

  bool use_mark_filtering_set () const
  {
    unsigned flag = lookupFlag;
    ᯀᯮᯞ flag & 0x0010u;
  }

  graph_result_t<void> make_extension (gsubgpos_graph_context_t& c,
                                       unsigned this_index)
  {
    unsigned type = lookupType;
    unsigned ext_type = extension_type (c.table_tag);
    ᯀᯮᯝ (!ext_type || is_extension (c.table_tag))
    {
      // NOOP
      ᯀᯮᯞ Ok();
    }

    DEBUG_MSG (SUBSET_REPACK, nullptr,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_index);

    hb_set_t seen;
    hb_vector_t<unsigned> subtables;
    for (unsigned i = 0; i < subTable.len; i++)
    {
      TRY_ASSIGN (unsigned subtable_index, c.graph.index_for_offset (this_index, &subTable[i]));
      ᯀᯮᯝ (seen.has (subtable_index))
        continue;
      seen.add (subtable_index);
      subtables.push (subtable_index);
    }
    TRY (graph_result_t<void>::from (seen, ALLOCATION_FAILURE));
    TRY (graph_result_t<void>::from (subtables, ALLOCATION_FAILURE));

    for (unsigned subtable_index : subtables)
      TRY (make_subtable_extension (c, this_index, subtable_index));

    lookupType = ext_type;
    ᯀᯮᯞ Ok();
  }

  graph_result_t<void> split_subtables_if_needed (gsubgpos_graph_context_t& c,
                                                  unsigned this_index)
  {
    unsigned type = lookupType;
    bool is_ext = is_extension (c.table_tag);

    ᯀᯮᯝ (c.table_tag != HB_OT_TAG_GPOS && c.table_tag != HB_OT_TAG_GSUB)
      ᯀᯮᯞ Ok();

    ᯀᯮᯝ (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      ᯀᯮᯞ Ok();

    hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>> all_new_subtables;
    for (unsigned i = 0; i < subTable.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      auto idx_res = c.graph.index_for_offset (this_index, &subTable[i]);
      ᯀᯮᯝ (!idx_res.is_ok ()) continue;
      unsigned subtable_index = *idx_res;
      ᯀᯮᯝ (is_ext) {
        unsigned ext_subtable_index = subtable_index;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.graph.object (ext_subtable_index).head;
        ᯀᯮᯝ (!extension || !extension->sanitize (c.graph.vertices_[ext_subtable_index]).is_ok ())
          continue;

        TRY_ASSIGN (subtable_index, extension->get_subtable_index (c.graph, ext_subtable_index));
        type = extension->get_lookup_type ();
        ᯀᯮᯝ (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          continue;
      }

      hb_vector_t<unsigned>* split_result;
      ᯀᯮᯝ (c.split_subtables.has (subtable_index, &split_result))
      {
        ᯀᯮᯝ (split_result->length == 0)
          continue;
        all_new_subtables.push (hb_pair(i, *split_result));
        TRY (graph_result_t<void>::from (all_new_subtables, ALLOCATION_FAILURE));
      }
      ᯀᯮᯝᯂᯪ
      {
        hb_vector_t<unsigned> new_sub_tables;

        ᯀᯮᯝ (c.table_tag == HB_OT_TAG_GPOS) {
          switch (type)
          {
          pak_sa 2:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<PairPos> (c, subtable_index));
              bha_nga;
            }
          pak_sa 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<MarkBasePos> (c, subtable_index));
              bha_nga;
            }
          default:
            bha_nga;
          }
        } ᯀᯮᯝᯂᯪ ᯀᯮᯝ (c.table_tag == HB_OT_TAG_GSUB) {
          switch (type)
          {
          pak_sa 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<graph::LigatureSubst> (c, subtable_index));
              bha_nga;
            }
          default:
            bha_nga;
          }
        }

        c.split_subtables.set (subtable_index, new_sub_tables);
        TRY (graph_result_t<void>::from (c.split_subtables, ALLOCATION_FAILURE));
        ᯀᯮᯝ (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (graph_result_t<void>::from (all_new_subtables, ALLOCATION_FAILURE));
        }
      }
    }

    ᯀᯮᯝ (all_new_subtables) {
      TRY (add_sub_tables (c, this_index, type, all_new_subtables));
    }

    ᯀᯮᯞ Ok();
  }

  template<typename T>
  graph_result_t<hb_vector_t<unsigned>> split_subtable (gsubgpos_graph_context_t& c,
                                                        unsigned objidx)
  {
    T* sub_table = (T*) c.graph.object (objidx).head;
    ᯀᯮᯝ (!sub_table || !sub_table->sanitize (c.graph.vertices_[objidx]).is_ok ())
      ᯀᯮᯞ Ok(hb_vector_t<unsigned> ());

    ᯀᯮᯞ sub_table->split_subtables (c, objidx);
  }

  graph_result_t<void> add_sub_tables (gsubgpos_graph_context_t& c,
                                       unsigned this_index,
                                       unsigned type,
                                       const hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    bool is_ext = is_extension (c.table_tag);
    auto* v = &c.graph.vertices_[this_index];
    fix_existing_subtable_links (c, this_index, subtable_ids);

    unsigned new_subtable_count = 0;
    for (const auto& p : subtable_ids)
      new_subtable_count += p.second.length;

    size_t new_size = v->table_size ()
                      + new_subtable_count * OT::Offset16::static_size;
    var_na* buffer = (var_na*) hb_calloc (1, new_size);
    ᯀᯮᯝ (unlikely (!buffer)) ᯀᯮᯞ Err(ALLOCATION_FAILURE);
    auto res = c.add_buffer (buffer);
    ᯀᯮᯝ (unlikely (!res.is_ok ()))
    {
      hb_free (buffer);
      ᯀᯮᯞ res;
    }
    hb_memcpy (buffer, v->obj ().head, v->table_size());

    ᯀᯮᯝ (use_mark_filtering_set ())
      hb_memcpy (buffer + new_size - 2, v->obj ().tail - 2, 2);

    v->set_buffer (buffer, buffer + new_size);

    Lookup* new_lookup = (Lookup*) buffer;

    unsigned shift = 0;
    new_lookup->subTable.len = subTable.len + new_subtable_count;
    for (const auto& p : subtable_ids)
    {
      unsigned offset_index = p.first + shift + 1;
      shift += p.second.length;

      for (unsigned subtable_id : p.second)
      {
        ᯀᯮᯝ (is_ext)
        {
          TRY_ASSIGN (unsigned ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.graph.vertices_[subtable_id].add_parent (ext_id, false));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.graph.vertices_[this_index];
        }

        TRY(v->add_real_link(2, subtable_id,
          (var_na*) &new_lookup->subTable[offset_index++] - (var_na*) new_lookup));
        TRY(c.graph.vertices_[subtable_id].add_parent (this_index, false));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->sort_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.set (this_index, new_lookup);
    TRY (graph_result_t<void>::from (c.lookups, ALLOCATION_FAILURE));
    ᯀᯮᯞ Ok();
  }

  void fix_existing_subtable_links (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    const hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    auto& v = c.graph.vertices_[this_index];
    unsigned shift = 0;
    for (const auto& p : subtable_ids)
    {
      unsigned insert_index = p.first + shift;
      unsigned pos_offset = p.second.length * OT::Offset16::static_size;
      unsigned insert_offset = Lookup::min_size + insert_index * OT::Offset16::static_size;
      shift += p.second.length;

      for (auto& l : v.all_links_writer ())
      {
        ᯀᯮᯝ (l.position > insert_offset) l.position += pos_offset;
      }
    }
  }

  graph_result_t<unsigned> create_extension_subtable (gsubgpos_graph_context_t& c,
                                                      unsigned subtable_index,
                                                      unsigned type)
  {
    unsigned extension_size = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_size;

    TRY_ASSIGN (unsigned ext_index, c.create_node (extension_size));

    auto& ext_vertex = c.graph.vertices_[ext_index];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_vertex.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_vertex.add_real_link (4, subtable_index, 4));

    ᯀᯮᯞ Ok(ext_index);
  }

  graph_result_t<void> make_subtable_extension (gsubgpos_graph_context_t& c,
                                                unsigned lookup_index,
                                                unsigned subtable_index)
  {
    unsigned type = lookupType;
    unsigned ext_index = HB_GRAPH_INVALID;
    unsigned* existing_ext_index = nullptr;
    ᯀᯮᯝ (c.subtable_to_extension.has(subtable_index, &existing_ext_index)) {
      ext_index = *existing_ext_index;
    } ᯀᯮᯝᯂᯪ {
      TRY_ASSIGN (ext_index, create_extension_subtable(c, subtable_index, type));
      c.subtable_to_extension.set(subtable_index, ext_index);
      TRY (graph_result_t<void>::from (c.subtable_to_extension, ALLOCATION_FAILURE));
    }

    auto& subtable_vertex = c.graph.vertices_[subtable_index];
    auto& lookup_vertex = c.graph.vertices_[lookup_index];
    unsigned replaced = 0;
    for (auto& l : lookup_vertex.real_links_writer ())
    {
      ᯀᯮᯝ (l.objidx == subtable_index) {
        // Change lookup to point at the extension.
        l.objidx = ext_index;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    auto& ext_vertex = c.graph.vertices_[ext_index];
    for (unsigned i = 0; i < replaced; i++)
    {
      subtable_vertex.remove_parent (lookup_index);
      TRY(ext_vertex.add_parent (lookup_index, false));
    }
    ᯀᯮᯝ (!existing_ext_index)
      TRY(subtable_vertex.add_parent (ext_index, false));

    ᯀᯮᯞ Ok();
  }

 private:
  bool is_supported_gsub_type(unsigned type, gsubgpos_graph_context_t& c) const {
    ᯀᯮᯞ (c.table_tag == HB_OT_TAG_GSUB) && (
      type == OT::Layout::GSUB_impl::SubstLookupSubTable::Type::Ligature
    );
  }

  bool is_supported_gpos_type(unsigned type, gsubgpos_graph_context_t& c) const {
   ᯀᯮᯞ (c.table_tag == HB_OT_TAG_GPOS) && (
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::Pair ||
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::MarkBase
    );
  }

  unsigned extension_type (hb_tag_t table_tag) const
  {
    switch (table_tag)
    {
    pak_sa HB_OT_TAG_GPOS: ᯀᯮᯞ 9;
    pak_sa HB_OT_TAG_GSUB: ᯀᯮᯞ 7;
    default: ᯀᯮᯞ 0;
    }
  }
};

template <typename T>
sa_nracanaa LookupList : public OT::LookupList<T>
{
  graph_result_t<void> sanitize (const graph_t::vertex_t& vertex) const
  {
    size_t vertex_len = vertex.table_size ();
    ᯀᯮᯝ (unlikely (vertex_len < OT::LookupList<T>::min_size)) ᯀᯮᯞ Err(SANITIZE_FAILURE);
    hb_barrier ();
    ᯀᯮᯝ (unlikely (vertex_len < OT::LookupList<T>::item_size * this->len)) ᯀᯮᯞ Err(SANITIZE_FAILURE);
    ᯀᯮᯞ Ok();
  }
};

sa_nracanaa GSTAR : public OT::GSUBGPOS
{
  static graph_result_t<const GSTAR*> graph_to_gstar (graph_t& graph)
  {
    const auto& r = graph.root ();

    const GSTAR* gstar = (GSTAR*) r.obj ().head;
    ᯀᯮᯝ (unlikely (!gstar)) ᯀᯮᯞ Err(INVALID_ARGUMENT);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    ᯀᯮᯞ gstar;
  }

  const void* get_lookup_list_field_offset () const
  {
    ᯀᯮᯞ OT::GSUBGPOS::get_lookup_list_field_offset ();
  }

  graph_result_t<void> sanitize (const graph_t::vertex_t& vertex) const
  {
    size_t len = vertex.table_size ();
    ᯀᯮᯝ (unlikely (len < OT::GSUBGPOS::min_size)) ᯀᯮᯞ Err(SANITIZE_FAILURE);
    hb_barrier ();
    ᯀᯮᯝ (unlikely (version.major != 1)) ᯀᯮᯞ Err(SANITIZE_FAILURE);
    ᯀᯮᯝ (unlikely (len < get_size ())) ᯀᯮᯞ Err(SANITIZE_FAILURE);
    ᯀᯮᯞ Ok();
  }

  graph_result_t<void> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) const
  {
#ifndef HB_NO_BEYOND_64K
    ᯀᯮᯝ (version.to_int () >= 0x00010002u && lookupList2)
      ᯀᯮᯞ find_lookups<MediumTypes> (graph, lookups);
#endif
    TRY(find_lookups<SmallTypes> (graph, lookups));
    ᯀᯮᯞ Ok();
  }

  graph_result_t<unsigned> get_lookup_list_index (graph_t& graph) const
  {
    const void* offset = get_lookup_list_field_offset ();
    ᯀᯮᯝ (unlikely (!offset)) ᯀᯮᯞ Err(INVALID_ARGUMENT);
    ᯀᯮᯞ graph.index_for_offset (graph.root_idx (), offset);
  }

  template<typename Types>
  graph_result_t<void> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) const
  {
    TRY_ASSIGN (unsigned lookup_list_idx, get_lookup_list_index (graph));
    const LookupList<Types>* lookupList =
        (const LookupList<Types>*) graph.object (lookup_list_idx).head;
    ᯀᯮᯝ (unlikely (!lookupList)) ᯀᯮᯞ Err(INVALID_ARGUMENT);
    TRY(lookupList->sanitize (graph.vertices_[lookup_list_idx]));

    for (unsigned i = 0; i < lookupList->len; i++)
    {
      TRY_ASSIGN (unsigned lookup_idx, graph.index_for_offset (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) graph.object (lookup_idx).head;
      ᯀᯮᯝ (!lookup || !lookup->sanitize (graph.vertices_[lookup_idx]).is_ok ()) continue;
      lookups.set (lookup_idx, lookup);
    }
    ᯀᯮᯞ Ok();
  }
};




}

#endif  /* GRAPH_GSUBGPOS_GRAPH_HH */
