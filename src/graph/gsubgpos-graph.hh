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

ढाँचा<typename T>
संरचना ExtensionFormat1 : सार्वजनिक OT::ExtensionFormat1<T>
{
  शून्य reset(अचिन्हित type)
  {
    एतत्->format = 1;
    एतत्->extensionLookupType = type;
    एतत्->extensionOffset = 0;
  }

  graph_result_t<शून्य> sanitize (ध्रुव graph_t::vertex_t& vertex) ध्रुव
  {
    माप_प्रकार vertex_len = vertex.table_size ();
    यदि (unlikely (vertex_len < OT::ExtensionFormat1<T>::static_size))
      प्रत्यागम Err(SANITIZE_FAILURE);
    प्रत्यागम Ok();
  }

  अचिन्हित get_lookup_type () ध्रुव
  {
    प्रत्यागम एतत्->extensionLookupType;
  }

  graph_result_t<अचिन्हित> get_subtable_index (graph_t& graph, अचिन्हित this_index) ध्रुव
  {
    प्रत्यागम graph.index_for_offset (this_index, &एतत्->extensionOffset);
  }
};

संरचना Lookup : सार्वजनिक OT::Lookup
{
  अचिन्हित संख्या_of_subtables () ध्रुव
  {
    प्रत्यागम subTable.len;
  }

  graph_result_t<शून्य> sanitize (ध्रुव graph_t::vertex_t& vertex) ध्रुव
  {
    माप_प्रकार vertex_len = vertex.table_size ();
    यदि (unlikely (vertex_len < OT::Lookup::min_size)) प्रत्यागम Err(SANITIZE_FAILURE);
    hb_barrier ();
    यदि (unlikely (vertex_len < एतत्->get_size ())) प्रत्यागम Err(SANITIZE_FAILURE);
    प्रत्यागम Ok();
  }

  तार्किक is_extension (hb_tag_t table_tag) ध्रुव
  {
    प्रत्यागम lookupType == extension_type (table_tag);
  }

  तार्किक use_mark_filtering_set () ध्रुव
  {
    अचिन्हित flag = lookupFlag;
    प्रत्यागम flag & 0x0010u;
  }

  graph_result_t<शून्य> make_extension (gsubgpos_graph_context_t& c,
                                       अचिन्हित this_index)
  {
    अचिन्हित type = lookupType;
    अचिन्हित ext_type = extension_type (c.table_tag);
    यदि (!ext_type || is_extension (c.table_tag))
    {
      // NOOP
      प्रत्यागम Ok();
    }

    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_index);

    hb_set_t seen;
    hb_vector_t<अचिन्हित> subtables;
    प्रति (अचिन्हित i = 0; i < subTable.len; i++)
    {
      TRY_ASSIGN (अचिन्हित subtable_index, c.graph.index_for_offset (this_index, &subTable[i]));
      यदि (seen.has (subtable_index))
        अनुवर्त;
      seen.add (subtable_index);
      subtables.push (subtable_index);
    }
    TRY (graph_result_t<शून्य>::from (seen, ALLOCATION_FAILURE));
    TRY (graph_result_t<शून्य>::from (subtables, ALLOCATION_FAILURE));

    प्रति (अचिन्हित subtable_index : subtables)
      TRY (make_subtable_extension (c, this_index, subtable_index));

    lookupType = ext_type;
    प्रत्यागम Ok();
  }

  graph_result_t<शून्य> split_subtables_if_needed (gsubgpos_graph_context_t& c,
                                                  अचिन्हित this_index)
  {
    अचिन्हित type = lookupType;
    तार्किक is_ext = is_extension (c.table_tag);

    यदि (c.table_tag != HB_OT_TAG_GPOS && c.table_tag != HB_OT_TAG_GSUB)
      प्रत्यागम Ok();

    यदि (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      प्रत्यागम Ok();

    hb_vector_t<hb_pair_t<अचिन्हित, hb_vector_t<अचिन्हित>>> all_new_subtables;
    प्रति (अचिन्हित i = 0; i < subTable.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      स्वतः idx_res = c.graph.index_for_offset (this_index, &subTable[i]);
      यदि (!idx_res.is_ok ()) अनुवर्त;
      अचिन्हित subtable_index = *idx_res;
      यदि (is_ext) {
        अचिन्हित ext_subtable_index = subtable_index;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.graph.object (ext_subtable_index).head;
        यदि (!extension || !extension->sanitize (c.graph.vertices_[ext_subtable_index]).is_ok ())
          अनुवर्त;

        TRY_ASSIGN (subtable_index, extension->get_subtable_index (c.graph, ext_subtable_index));
        type = extension->get_lookup_type ();
        यदि (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          अनुवर्त;
      }

      hb_vector_t<अचिन्हित>* split_result;
      यदि (c.split_subtables.has (subtable_index, &split_result))
      {
        यदि (split_result->length == 0)
          अनुवर्त;
        all_new_subtables.push (hb_pair(i, *split_result));
        TRY (graph_result_t<शून्य>::from (all_new_subtables, ALLOCATION_FAILURE));
      }
      अन्यथा
      {
        hb_vector_t<अचिन्हित> new_sub_tables;

        यदि (c.table_tag == HB_OT_TAG_GPOS) {
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
        } अन्यथा यदि (c.table_tag == HB_OT_TAG_GSUB) {
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
        यदि (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (graph_result_t<शून्य>::from (all_new_subtables, ALLOCATION_FAILURE));
        }
      }
    }

    यदि (all_new_subtables) {
      TRY (add_sub_tables (c, this_index, type, all_new_subtables));
    }

    प्रत्यागम Ok();
  }

  ढाँचा<typename T>
  graph_result_t<hb_vector_t<अचिन्हित>> split_subtable (gsubgpos_graph_context_t& c,
                                                        अचिन्हित objidx)
  {
    T* sub_table = (T*) c.graph.object (objidx).head;
    यदि (!sub_table || !sub_table->sanitize (c.graph.vertices_[objidx]).is_ok ())
      प्रत्यागम Ok(hb_vector_t<अचिन्हित> ());

    प्रत्यागम sub_table->split_subtables (c, objidx);
  }

  graph_result_t<शून्य> add_sub_tables (gsubgpos_graph_context_t& c,
                                       अचिन्हित this_index,
                                       अचिन्हित type,
                                       ध्रुव hb_vector_t<hb_pair_t<अचिन्हित, hb_vector_t<अचिन्हित>>>& subtable_ids)
  {
    तार्किक is_ext = is_extension (c.table_tag);
    स्वतः* v = &c.graph.vertices_[this_index];
    fix_existing_subtable_links (c, this_index, subtable_ids);

    अचिन्हित new_subtable_गिनती = 0;
    प्रति (ध्रुव स्वतः& p : subtable_ids)
      new_subtable_गिनती += p.second.length;

    माप_प्रकार new_size = v->table_size ()
                      + new_subtable_गिनती * OT::Offset16::static_size;
    वर्ण* buffer = (वर्ण*) hb_calloc (1, new_size);
    यदि (unlikely (!buffer)) प्रत्यागम Err(ALLOCATION_FAILURE);
    स्वतः res = c.add_buffer (buffer);
    यदि (unlikely (!res.is_ok ()))
    {
      hb_free (buffer);
      प्रत्यागम res;
    }
    hb_memcpy (buffer, v->obj ().head, v->table_size());

    यदि (use_mark_filtering_set ())
      hb_memcpy (buffer + new_size - 2, v->obj ().tail - 2, 2);

    v->set_buffer (buffer, buffer + new_size);

    Lookup* new_lookup = (Lookup*) buffer;

    अचिन्हित shift = 0;
    new_lookup->subTable.len = subTable.len + new_subtable_गिनती;
    प्रति (ध्रुव स्वतः& p : subtable_ids)
    {
      अचिन्हित offset_index = p.first + shift + 1;
      shift += p.second.length;

      प्रति (अचिन्हित subtable_id : p.second)
      {
        यदि (is_ext)
        {
          TRY_ASSIGN (अचिन्हित ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.graph.vertices_[subtable_id].add_parent (ext_id, असत्य));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.graph.vertices_[this_index];
        }

        TRY(v->add_real_link(2, subtable_id,
          (वर्ण*) &new_lookup->subTable[offset_index++] - (वर्ण*) new_lookup));
        TRY(c.graph.vertices_[subtable_id].add_parent (this_index, असत्य));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->sort_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.set (this_index, new_lookup);
    TRY (graph_result_t<शून्य>::from (c.lookups, ALLOCATION_FAILURE));
    प्रत्यागम Ok();
  }

  शून्य fix_existing_subtable_links (gsubgpos_graph_context_t& c,
                                    अचिन्हित this_index,
                                    ध्रुव hb_vector_t<hb_pair_t<अचिन्हित, hb_vector_t<अचिन्हित>>>& subtable_ids)
  {
    स्वतः& v = c.graph.vertices_[this_index];
    अचिन्हित shift = 0;
    प्रति (ध्रुव स्वतः& p : subtable_ids)
    {
      अचिन्हित जोड़ो_index = p.first + shift;
      अचिन्हित pos_offset = p.second.length * OT::Offset16::static_size;
      अचिन्हित जोड़ो_offset = Lookup::min_size + जोड़ो_index * OT::Offset16::static_size;
      shift += p.second.length;

      प्रति (स्वतः& l : v.all_links_writer ())
      {
        यदि (l.position > जोड़ो_offset) l.position += pos_offset;
      }
    }
  }

  graph_result_t<अचिन्हित> create_extension_subtable (gsubgpos_graph_context_t& c,
                                                      अचिन्हित subtable_index,
                                                      अचिन्हित type)
  {
    अचिन्हित extension_size = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_size;

    TRY_ASSIGN (अचिन्हित ext_index, c.create_गाँठ (extension_size));

    स्वतः& ext_vertex = c.graph.vertices_[ext_index];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_vertex.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_vertex.add_real_link (4, subtable_index, 4));

    प्रत्यागम Ok(ext_index);
  }

  graph_result_t<शून्य> make_subtable_extension (gsubgpos_graph_context_t& c,
                                                अचिन्हित lookup_index,
                                                अचिन्हित subtable_index)
  {
    अचिन्हित type = lookupType;
    अचिन्हित ext_index = HB_GRAPH_INVALID;
    अचिन्हित* existing_ext_index = nullptr;
    यदि (c.subtable_to_extension.has(subtable_index, &existing_ext_index)) {
      ext_index = *existing_ext_index;
    } अन्यथा {
      TRY_ASSIGN (ext_index, create_extension_subtable(c, subtable_index, type));
      c.subtable_to_extension.set(subtable_index, ext_index);
      TRY (graph_result_t<शून्य>::from (c.subtable_to_extension, ALLOCATION_FAILURE));
    }

    स्वतः& subtable_vertex = c.graph.vertices_[subtable_index];
    स्वतः& lookup_vertex = c.graph.vertices_[lookup_index];
    अचिन्हित replaced = 0;
    प्रति (स्वतः& l : lookup_vertex.real_links_writer ())
    {
      यदि (l.objidx == subtable_index) {
        // Change lookup to point at the extension.
        l.objidx = ext_index;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    स्वतः& ext_vertex = c.graph.vertices_[ext_index];
    प्रति (अचिन्हित i = 0; i < replaced; i++)
    {
      subtable_vertex.remove_parent (lookup_index);
      TRY(ext_vertex.add_parent (lookup_index, असत्य));
    }
    यदि (!existing_ext_index)
      TRY(subtable_vertex.add_parent (ext_index, असत्य));

    प्रत्यागम Ok();
  }

 गुप्त:
  तार्किक is_supported_gsub_type(अचिन्हित type, gsubgpos_graph_context_t& c) ध्रुव {
    प्रत्यागम (c.table_tag == HB_OT_TAG_GSUB) && (
      type == OT::Layout::GSUB_impl::SubstLookupSubTable::Type::Ligature
    );
  }

  तार्किक is_supported_gpos_type(अचिन्हित type, gsubgpos_graph_context_t& c) ध्रुव {
   प्रत्यागम (c.table_tag == HB_OT_TAG_GPOS) && (
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::Pair ||
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::MarkBase
    );
  }

  अचिन्हित extension_type (hb_tag_t table_tag) ध्रुव
  {
    विकल्प (table_tag)
    {
    पक्ष HB_OT_TAG_GPOS: प्रत्यागम 9;
    पक्ष HB_OT_TAG_GSUB: प्रत्यागम 7;
    स्वभाव: प्रत्यागम 0;
    }
  }
};

ढाँचा <typename T>
संरचना LookupList : सार्वजनिक OT::LookupList<T>
{
  graph_result_t<शून्य> sanitize (ध्रुव graph_t::vertex_t& vertex) ध्रुव
  {
    माप_प्रकार vertex_len = vertex.table_size ();
    यदि (unlikely (vertex_len < OT::LookupList<T>::min_size)) प्रत्यागम Err(SANITIZE_FAILURE);
    hb_barrier ();
    यदि (unlikely (vertex_len < OT::LookupList<T>::item_size * एतत्->len)) प्रत्यागम Err(SANITIZE_FAILURE);
    प्रत्यागम Ok();
  }
};

संरचना GSTAR : सार्वजनिक OT::GSUBGPOS
{
  स्थिर graph_result_t<ध्रुव GSTAR*> graph_to_gstar (graph_t& graph)
  {
    ध्रुव स्वतः& r = graph.मूल ();

    ध्रुव GSTAR* gstar = (GSTAR*) r.obj ().head;
    यदि (unlikely (!gstar)) प्रत्यागम Err(INVALID_ARGUMENT);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    प्रत्यागम gstar;
  }

  ध्रुव शून्य* get_lookup_list_field_offset () ध्रुव
  {
    प्रत्यागम OT::GSUBGPOS::get_lookup_list_field_offset ();
  }

  graph_result_t<शून्य> sanitize (ध्रुव graph_t::vertex_t& vertex) ध्रुव
  {
    माप_प्रकार len = vertex.table_size ();
    यदि (unlikely (len < OT::GSUBGPOS::min_size)) प्रत्यागम Err(SANITIZE_FAILURE);
    hb_barrier ();
    यदि (unlikely (version.major != 1)) प्रत्यागम Err(SANITIZE_FAILURE);
    यदि (unlikely (len < get_size ())) प्रत्यागम Err(SANITIZE_FAILURE);
    प्रत्यागम Ok();
  }

  graph_result_t<शून्य> find_lookups (graph_t& graph,
                                     hb_hashmap_t<अचिन्हित, Lookup*>& lookups /* OUT */) ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010002u && lookupList2)
      प्रत्यागम find_lookups<MediumTypes> (graph, lookups);
#endif
    TRY(find_lookups<SmallTypes> (graph, lookups));
    प्रत्यागम Ok();
  }

  graph_result_t<अचिन्हित> get_lookup_list_index (graph_t& graph) ध्रुव
  {
    ध्रुव शून्य* offset = get_lookup_list_field_offset ();
    यदि (unlikely (!offset)) प्रत्यागम Err(INVALID_ARGUMENT);
    प्रत्यागम graph.index_for_offset (graph.मूल_idx (), offset);
  }

  ढाँचा<typename Types>
  graph_result_t<शून्य> find_lookups (graph_t& graph,
                                     hb_hashmap_t<अचिन्हित, Lookup*>& lookups /* OUT */) ध्रुव
  {
    TRY_ASSIGN (अचिन्हित lookup_list_idx, get_lookup_list_index (graph));
    ध्रुव LookupList<Types>* lookupList =
        (ध्रुव LookupList<Types>*) graph.object (lookup_list_idx).head;
    यदि (unlikely (!lookupList)) प्रत्यागम Err(INVALID_ARGUMENT);
    TRY(lookupList->sanitize (graph.vertices_[lookup_list_idx]));

    प्रति (अचिन्हित i = 0; i < lookupList->len; i++)
    {
      TRY_ASSIGN (अचिन्हित lookup_idx, graph.index_for_offset (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) graph.object (lookup_idx).head;
      यदि (!lookup || !lookup->sanitize (graph.vertices_[lookup_idx]).is_ok ()) अनुवर्त;
      lookups.set (lookup_idx, lookup);
    }
    प्रत्यागम Ok();
  }
};




}

#endif  /* GRAPH_GSUBGPOS_GRAPH_HH */
