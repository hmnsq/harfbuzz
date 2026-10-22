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

কাঠামো Lookup;

template<typename T>
কাঠামো ExtensionFormat1 : সর্বজনীন OT::ExtensionFormat1<T>
{
  শূন্য reset(unsigned type)
  {
    এই->format = 1;
    এই->extensionLookupType = type;
    এই->extensionOffset = 0;
  }

  graph_ফলাফল_t<শূন্য> sanitize (ধ্রুবক graph_t::vertex_t& vertex) ধ্রুবক
  {
    size_t vertex_len = vertex.table_আকার ();
    যদি (unlikely (vertex_len < OT::ExtensionFormat1<T>::static_আকার))
      ফেরত Err(SANITIZE_FAILURE);
    ফেরত Ok();
  }

  unsigned get_lookup_type () ধ্রুবক
  {
    ফেরত এই->extensionLookupType;
  }

  graph_ফলাফল_t<unsigned> get_subtable_সূচক (graph_t& graph, unsigned this_সূচক) ধ্রুবক
  {
    ফেরত graph.সূচক_for_offset (this_সূচক, &এই->extensionOffset);
  }
};

কাঠামো Lookup : সর্বজনীন OT::Lookup
{
  unsigned সংখ্যা_of_subtables () ধ্রুবক
  {
    ফেরত subTable.len;
  }

  graph_ফলাফল_t<শূন্য> sanitize (ধ্রুবক graph_t::vertex_t& vertex) ধ্রুবক
  {
    size_t vertex_len = vertex.table_আকার ();
    যদি (unlikely (vertex_len < OT::Lookup::min_আকার)) ফেরত Err(SANITIZE_FAILURE);
    hb_barrier ();
    যদি (unlikely (vertex_len < এই->get_আকার ())) ফেরত Err(SANITIZE_FAILURE);
    ফেরত Ok();
  }

  যৌক্তিক is_extension (hb_tag_t table_tag) ধ্রুবক
  {
    ফেরত lookupType == extension_type (table_tag);
  }

  যৌক্তিক use_mark_filtering_set () ধ্রুবক
  {
    unsigned flag = lookupFlag;
    ফেরত flag & 0x0010u;
  }

  graph_ফলাফল_t<শূন্য> make_extension (gsubgpos_graph_context_t& c,
                                       unsigned this_সূচক)
  {
    unsigned type = lookupType;
    unsigned ext_type = extension_type (c.table_tag);
    যদি (!ext_type || is_extension (c.table_tag))
    {
      // NOOP
      ফেরত Ok();
    }

    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_সূচক);

    hb_set_t seen;
    hb_vector_t<unsigned> subtables;
    জন্য (unsigned i = 0; i < subTable.len; i++)
    {
      TRY_ASSIGN (unsigned subtable_সূচক, c.graph.সূচক_for_offset (this_সূচক, &subTable[i]));
      যদি (seen.has (subtable_সূচক))
        চালু;
      seen.add (subtable_সূচক);
      subtables.push (subtable_সূচক);
    }
    TRY (graph_ফলাফল_t<শূন্য>::from (seen, ALLOCATION_FAILURE));
    TRY (graph_ফলাফল_t<শূন্য>::from (subtables, ALLOCATION_FAILURE));

    জন্য (unsigned subtable_সূচক : subtables)
      TRY (make_subtable_extension (c, this_সূচক, subtable_সূচক));

    lookupType = ext_type;
    ফেরত Ok();
  }

  graph_ফলাফল_t<শূন্য> split_subtables_if_needed (gsubgpos_graph_context_t& c,
                                                  unsigned this_সূচক)
  {
    unsigned type = lookupType;
    যৌক্তিক is_ext = is_extension (c.table_tag);

    যদি (c.table_tag != HB_OT_TAG_GPOS && c.table_tag != HB_OT_TAG_GSUB)
      ফেরত Ok();

    যদি (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      ফেরত Ok();

    hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>> all_new_subtables;
    জন্য (unsigned i = 0; i < subTable.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      auto idx_res = c.graph.সূচক_for_offset (this_সূচক, &subTable[i]);
      যদি (!idx_res.is_ok ()) চালু;
      unsigned subtable_সূচক = *idx_res;
      যদি (is_ext) {
        unsigned ext_subtable_সূচক = subtable_সূচক;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.graph.object (ext_subtable_সূচক).শীর্ষ_2;
        যদি (!extension || !extension->sanitize (c.graph.vertices_[ext_subtable_সূচক]).is_ok ())
          চালু;

        TRY_ASSIGN (subtable_সূচক, extension->get_subtable_সূচক (c.graph, ext_subtable_সূচক));
        type = extension->get_lookup_type ();
        যদি (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          চালু;
      }

      hb_vector_t<unsigned>* split_ফলাফল;
      যদি (c.split_subtables.has (subtable_সূচক, &split_ফলাফল))
      {
        যদি (split_ফলাফল->দৈর্ঘ্য == 0)
          চালু;
        all_new_subtables.push (hb_pair(i, *split_ফলাফল));
        TRY (graph_ফলাফল_t<শূন্য>::from (all_new_subtables, ALLOCATION_FAILURE));
      }
      অন্যথা
      {
        hb_vector_t<unsigned> new_sub_tables;

        যদি (c.table_tag == HB_OT_TAG_GPOS) {
          নির্বাচন (type)
          {
          ক্ষেত্র 2:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<PairPos> (c, subtable_সূচক));
              বিরতি;
            }
          ক্ষেত্র 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<MarkBasePos> (c, subtable_সূচক));
              বিরতি;
            }
          পূর্বনির্ধারিত:
            বিরতি;
          }
        } অন্যথা যদি (c.table_tag == HB_OT_TAG_GSUB) {
          নির্বাচন (type)
          {
          ক্ষেত্র 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<graph::LigatureSubst> (c, subtable_সূচক));
              বিরতি;
            }
          পূর্বনির্ধারিত:
            বিরতি;
          }
        }

        c.split_subtables.set (subtable_সূচক, new_sub_tables);
        TRY (graph_ফলাফল_t<শূন্য>::from (c.split_subtables, ALLOCATION_FAILURE));
        যদি (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (graph_ফলাফল_t<শূন্য>::from (all_new_subtables, ALLOCATION_FAILURE));
        }
      }
    }

    যদি (all_new_subtables) {
      TRY (add_sub_tables (c, this_সূচক, type, all_new_subtables));
    }

    ফেরত Ok();
  }

  template<typename T>
  graph_ফলাফল_t<hb_vector_t<unsigned>> split_subtable (gsubgpos_graph_context_t& c,
                                                        unsigned objidx)
  {
    T* sub_table = (T*) c.graph.object (objidx).শীর্ষ_2;
    যদি (!sub_table || !sub_table->sanitize (c.graph.vertices_[objidx]).is_ok ())
      ফেরত Ok(hb_vector_t<unsigned> ());

    ফেরত sub_table->split_subtables (c, objidx);
  }

  graph_ফলাফল_t<শূন্য> add_sub_tables (gsubgpos_graph_context_t& c,
                                       unsigned this_সূচক,
                                       unsigned type,
                                       ধ্রুবক hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    যৌক্তিক is_ext = is_extension (c.table_tag);
    auto* v = &c.graph.vertices_[this_সূচক];
    fix_existing_subtable_links (c, this_সূচক, subtable_ids);

    unsigned new_subtable_গুনতি = 0;
    জন্য (ধ্রুবক auto& p : subtable_ids)
      new_subtable_গুনতি += p.second.দৈর্ঘ্য;

    size_t new_আকার = v->table_আকার ()
                      + new_subtable_গুনতি * OT::Offset16::static_আকার;
    অক্ষর* বাফার = (অক্ষর*) hb_calloc (1, new_আকার);
    যদি (unlikely (!বাফার)) ফেরত Err(ALLOCATION_FAILURE);
    auto res = c.add_বাফার (বাফার);
    যদি (unlikely (!res.is_ok ()))
    {
      hb_free (বাফার);
      ফেরত res;
    }
    hb_memcpy (বাফার, v->obj ().শীর্ষ_2, v->table_আকার());

    যদি (use_mark_filtering_set ())
      hb_memcpy (বাফার + new_আকার - 2, v->obj ().লেজ - 2, 2);

    v->set_বাফার (বাফার, বাফার + new_আকার);

    Lookup* new_lookup = (Lookup*) বাফার;

    unsigned shift = 0;
    new_lookup->subTable.len = subTable.len + new_subtable_গুনতি;
    জন্য (ধ্রুবক auto& p : subtable_ids)
    {
      unsigned offset_সূচক = p.first + shift + 1;
      shift += p.second.দৈর্ঘ্য;

      জন্য (unsigned subtable_id : p.second)
      {
        যদি (is_ext)
        {
          TRY_ASSIGN (unsigned ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.graph.vertices_[subtable_id].add_parent (ext_id, মিথ্যা));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.graph.vertices_[this_সূচক];
        }

        TRY(v->add_real_link(2, subtable_id,
          (অক্ষর*) &new_lookup->subTable[offset_সূচক++] - (অক্ষর*) new_lookup));
        TRY(c.graph.vertices_[subtable_id].add_parent (this_সূচক, মিথ্যা));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->sort_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.set (this_সূচক, new_lookup);
    TRY (graph_ফলাফল_t<শূন্য>::from (c.lookups, ALLOCATION_FAILURE));
    ফেরত Ok();
  }

  শূন্য fix_existing_subtable_links (gsubgpos_graph_context_t& c,
                                    unsigned this_সূচক,
                                    ধ্রুবক hb_vector_t<hb_pair_t<unsigned, hb_vector_t<unsigned>>>& subtable_ids)
  {
    auto& v = c.graph.vertices_[this_সূচক];
    unsigned shift = 0;
    জন্য (ধ্রুবক auto& p : subtable_ids)
    {
      unsigned ঢোকাও_সূচক = p.first + shift;
      unsigned pos_offset = p.second.দৈর্ঘ্য * OT::Offset16::static_আকার;
      unsigned ঢোকাও_offset = Lookup::min_আকার + ঢোকাও_সূচক * OT::Offset16::static_আকার;
      shift += p.second.দৈর্ঘ্য;

      জন্য (auto& l : v.all_links_writer ())
      {
        যদি (l.position > ঢোকাও_offset) l.position += pos_offset;
      }
    }
  }

  graph_ফলাফল_t<unsigned> create_extension_subtable (gsubgpos_graph_context_t& c,
                                                      unsigned subtable_সূচক,
                                                      unsigned type)
  {
    unsigned extension_আকার = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_আকার;

    TRY_ASSIGN (unsigned ext_সূচক, c.create_গ্রন্থি (extension_আকার));

    auto& ext_vertex = c.graph.vertices_[ext_সূচক];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_vertex.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_vertex.add_real_link (4, subtable_সূচক, 4));

    ফেরত Ok(ext_সূচক);
  }

  graph_ফলাফল_t<শূন্য> make_subtable_extension (gsubgpos_graph_context_t& c,
                                                unsigned lookup_সূচক,
                                                unsigned subtable_সূচক)
  {
    unsigned type = lookupType;
    unsigned ext_সূচক = HB_GRAPH_INVALID;
    unsigned* existing_ext_সূচক = nullptr;
    যদি (c.subtable_to_extension.has(subtable_সূচক, &existing_ext_সূচক)) {
      ext_সূচক = *existing_ext_সূচক;
    } অন্যথা {
      TRY_ASSIGN (ext_সূচক, create_extension_subtable(c, subtable_সূচক, type));
      c.subtable_to_extension.set(subtable_সূচক, ext_সূচক);
      TRY (graph_ফলাফল_t<শূন্য>::from (c.subtable_to_extension, ALLOCATION_FAILURE));
    }

    auto& subtable_vertex = c.graph.vertices_[subtable_সূচক];
    auto& lookup_vertex = c.graph.vertices_[lookup_সূচক];
    unsigned replaced = 0;
    জন্য (auto& l : lookup_vertex.real_links_writer ())
    {
      যদি (l.objidx == subtable_সূচক) {
        // Change lookup to point at the extension.
        l.objidx = ext_সূচক;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    auto& ext_vertex = c.graph.vertices_[ext_সূচক];
    জন্য (unsigned i = 0; i < replaced; i++)
    {
      subtable_vertex.remove_parent (lookup_সূচক);
      TRY(ext_vertex.add_parent (lookup_সূচক, মিথ্যা));
    }
    যদি (!existing_ext_সূচক)
      TRY(subtable_vertex.add_parent (ext_সূচক, মিথ্যা));

    ফেরত Ok();
  }

 ব্যক্তিগত:
  যৌক্তিক is_supported_gsub_type(unsigned type, gsubgpos_graph_context_t& c) ধ্রুবক {
    ফেরত (c.table_tag == HB_OT_TAG_GSUB) && (
      type == OT::Layout::GSUB_impl::SubstLookupSubTable::Type::Ligature
    );
  }

  যৌক্তিক is_supported_gpos_type(unsigned type, gsubgpos_graph_context_t& c) ধ্রুবক {
   ফেরত (c.table_tag == HB_OT_TAG_GPOS) && (
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::Pair ||
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::MarkBase
    );
  }

  unsigned extension_type (hb_tag_t table_tag) ধ্রুবক
  {
    নির্বাচন (table_tag)
    {
    ক্ষেত্র HB_OT_TAG_GPOS: ফেরত 9;
    ক্ষেত্র HB_OT_TAG_GSUB: ফেরত 7;
    পূর্বনির্ধারিত: ফেরত 0;
    }
  }
};

template <typename T>
কাঠামো LookupList : সর্বজনীন OT::LookupList<T>
{
  graph_ফলাফল_t<শূন্য> sanitize (ধ্রুবক graph_t::vertex_t& vertex) ধ্রুবক
  {
    size_t vertex_len = vertex.table_আকার ();
    যদি (unlikely (vertex_len < OT::LookupList<T>::min_আকার)) ফেরত Err(SANITIZE_FAILURE);
    hb_barrier ();
    যদি (unlikely (vertex_len < OT::LookupList<T>::item_আকার * এই->len)) ফেরত Err(SANITIZE_FAILURE);
    ফেরত Ok();
  }
};

কাঠামো GSTAR : সর্বজনীন OT::GSUBGPOS
{
  স্থির graph_ফলাফল_t<ধ্রুবক GSTAR*> graph_to_gstar (graph_t& graph)
  {
    ধ্রুবক auto& r = graph.মূল ();

    ধ্রুবক GSTAR* gstar = (GSTAR*) r.obj ().শীর্ষ_2;
    যদি (unlikely (!gstar)) ফেরত Err(INVALID_ARGUMENT);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    ফেরত gstar;
  }

  ধ্রুবক শূন্য* get_lookup_list_field_offset () ধ্রুবক
  {
    ফেরত OT::GSUBGPOS::get_lookup_list_field_offset ();
  }

  graph_ফলাফল_t<শূন্য> sanitize (ধ্রুবক graph_t::vertex_t& vertex) ধ্রুবক
  {
    size_t len = vertex.table_আকার ();
    যদি (unlikely (len < OT::GSUBGPOS::min_আকার)) ফেরত Err(SANITIZE_FAILURE);
    hb_barrier ();
    যদি (unlikely (version.major != 1)) ফেরত Err(SANITIZE_FAILURE);
    যদি (unlikely (len < get_আকার ())) ফেরত Err(SANITIZE_FAILURE);
    ফেরত Ok();
  }

  graph_ফলাফল_t<শূন্য> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010002u && lookupList2)
      ফেরত find_lookups<MediumTypes> (graph, lookups);
#endif
    TRY(find_lookups<SmallTypes> (graph, lookups));
    ফেরত Ok();
  }

  graph_ফলাফল_t<unsigned> get_lookup_list_সূচক (graph_t& graph) ধ্রুবক
  {
    ধ্রুবক শূন্য* offset = get_lookup_list_field_offset ();
    যদি (unlikely (!offset)) ফেরত Err(INVALID_ARGUMENT);
    ফেরত graph.সূচক_for_offset (graph.মূল_idx (), offset);
  }

  template<typename Types>
  graph_ফলাফল_t<শূন্য> find_lookups (graph_t& graph,
                                     hb_hashmap_t<unsigned, Lookup*>& lookups /* OUT */) ধ্রুবক
  {
    TRY_ASSIGN (unsigned lookup_list_idx, get_lookup_list_সূচক (graph));
    ধ্রুবক LookupList<Types>* lookupList =
        (ধ্রুবক LookupList<Types>*) graph.object (lookup_list_idx).শীর্ষ_2;
    যদি (unlikely (!lookupList)) ফেরত Err(INVALID_ARGUMENT);
    TRY(lookupList->sanitize (graph.vertices_[lookup_list_idx]));

    জন্য (unsigned i = 0; i < lookupList->len; i++)
    {
      TRY_ASSIGN (unsigned lookup_idx, graph.সূচক_for_offset (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) graph.object (lookup_idx).শীর্ষ_2;
      যদি (!lookup || !lookup->sanitize (graph.vertices_[lookup_idx]).is_ok ()) চালু;
      lookups.set (lookup_idx, lookup);
    }
    ফেরত Ok();
  }
};




}

#endif  /* GRAPH_GSUBGPOS_GRAPH_HH */
