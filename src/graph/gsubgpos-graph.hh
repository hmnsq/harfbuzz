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
    हे->format = 1;
    हे->extensionLookupType = type;
    हे->extensionOffset = 0;
  }

  graph_result_t<शून्य> sanitize (स्थिरांक graph_t::vertex_t& vertex) स्थिरांक
  {
    माप_प्रकार vertex_len = vertex.table_size ();
    जर (unlikely (vertex_len < OT::ExtensionFormat1<T>::static_size))
      परत Err(SANITIZE_FAILURE);
    परत Ok();
  }

  अचिन्हित get_lookup_type () स्थिरांक
  {
    परत हे->extensionLookupType;
  }

  graph_result_t<अचिन्हित> get_subtable_index (graph_t& graph, अचिन्हित this_index) स्थिरांक
  {
    परत graph.index_for_offset (this_index, &हे->extensionOffset);
  }
};

संरचना Lookup : सार्वजनिक OT::Lookup
{
  अचिन्हित संख्या_of_subtables () स्थिरांक
  {
    परत subTable.len;
  }

  graph_result_t<शून्य> sanitize (स्थिरांक graph_t::vertex_t& vertex) स्थिरांक
  {
    माप_प्रकार vertex_len = vertex.table_size ();
    जर (unlikely (vertex_len < OT::Lookup::min_size)) परत Err(SANITIZE_FAILURE);
    hb_barrier ();
    जर (unlikely (vertex_len < हे->get_size ())) परत Err(SANITIZE_FAILURE);
    परत Ok();
  }

  तार्किक is_extension (hb_tag_t table_tag) स्थिरांक
  {
    परत lookupType == extension_type (table_tag);
  }

  तार्किक use_mark_filtering_set () स्थिरांक
  {
    अचिन्हित flag = lookupFlag;
    परत flag & 0x0010u;
  }

  graph_result_t<शून्य> make_extension (gsubgpos_graph_context_t& c,
                                       अचिन्हित this_index)
  {
    अचिन्हित type = lookupType;
    अचिन्हित ext_type = extension_type (c.table_tag);
    जर (!ext_type || is_extension (c.table_tag))
    {
      // NOOP
      परत Ok();
    }

    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_index);

    hb_set_t seen;
    hb_vector_t<अचिन्हित> subtables;
    साठी (अचिन्हित i = 0; i < subTable.len; i++)
    {
      TRY_ASSIGN (अचिन्हित subtable_index, c.graph.index_for_offset (this_index, &subTable[i]));
      जर (seen.has (subtable_index))
        सुरूठेवा;
      seen.add (subtable_index);
      subtables.push (subtable_index);
    }
    TRY (graph_result_t<शून्य>::from (seen, ALLOCATION_FAILURE));
    TRY (graph_result_t<शून्य>::from (subtables, ALLOCATION_FAILURE));

    साठी (अचिन्हित subtable_index : subtables)
      TRY (make_subtable_extension (c, this_index, subtable_index));

    lookupType = ext_type;
    परत Ok();
  }

  graph_result_t<शून्य> split_subtables_if_needed (gsubgpos_graph_context_t& c,
                                                  अचिन्हित this_index)
  {
    अचिन्हित type = lookupType;
    तार्किक is_ext = is_extension (c.table_tag);

    जर (c.table_tag != HB_OT_TAG_GPOS && c.table_tag != HB_OT_TAG_GSUB)
      परत Ok();

    जर (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      परत Ok();

    hb_vector_t<hb_pair_t<अचिन्हित, hb_vector_t<अचिन्हित>>> all_new_subtables;
    साठी (अचिन्हित i = 0; i < subTable.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      स्वतः idx_res = c.graph.index_for_offset (this_index, &subTable[i]);
      जर (!idx_res.is_ok ()) सुरूठेवा;
      अचिन्हित subtable_index = *idx_res;
      जर (is_ext) {
        अचिन्हित ext_subtable_index = subtable_index;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.graph.object (ext_subtable_index).head;
        जर (!extension || !extension->sanitize (c.graph.vertices_[ext_subtable_index]).is_ok ())
          सुरूठेवा;

        TRY_ASSIGN (subtable_index, extension->get_subtable_index (c.graph, ext_subtable_index));
        type = extension->get_lookup_type ();
        जर (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          सुरूठेवा;
      }

      hb_vector_t<अचिन्हित>* split_result;
      जर (c.split_subtables.has (subtable_index, &split_result))
      {
        जर (split_result->length == 0)
          सुरूठेवा;
        all_new_subtables.push (hb_pair(i, *split_result));
        TRY (graph_result_t<शून्य>::from (all_new_subtables, ALLOCATION_FAILURE));
      }
      अन्यथा
      {
        hb_vector_t<अचिन्हित> new_sub_tables;

        जर (c.table_tag == HB_OT_TAG_GPOS) {
          निवड (type)
          {
          स्थिती 2:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<PairPos> (c, subtable_index));
              थांबा;
            }
          स्थिती 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<MarkBasePos> (c, subtable_index));
              थांबा;
            }
          पूर्वनिर्धारित:
            थांबा;
          }
        } अन्यथा जर (c.table_tag == HB_OT_TAG_GSUB) {
          निवड (type)
          {
          स्थिती 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<graph::LigatureSubst> (c, subtable_index));
              थांबा;
            }
          पूर्वनिर्धारित:
            थांबा;
          }
        }

        c.split_subtables.set (subtable_index, new_sub_tables);
        TRY (graph_result_t<शून्य>::from (c.split_subtables, ALLOCATION_FAILURE));
        जर (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (graph_result_t<शून्य>::from (all_new_subtables, ALLOCATION_FAILURE));
        }
      }
    }

    जर (all_new_subtables) {
      TRY (add_sub_tables (c, this_index, type, all_new_subtables));
    }

    परत Ok();
  }

  ढाँचा<typename T>
  graph_result_t<hb_vector_t<अचिन्हित>> split_subtable (gsubgpos_graph_context_t& c,
                                                        अचिन्हित objidx)
  {
    T* sub_table = (T*) c.graph.object (objidx).head;
    जर (!sub_table || !sub_table->sanitize (c.graph.vertices_[objidx]).is_ok ())
      परत Ok(hb_vector_t<अचिन्हित> ());

    परत sub_table->split_subtables (c, objidx);
  }

  graph_result_t<शून्य> add_sub_tables (gsubgpos_graph_context_t& c,
                                       अचिन्हित this_index,
                                       अचिन्हित type,
                                       स्थिरांक hb_vector_t<hb_pair_t<अचिन्हित, hb_vector_t<अचिन्हित>>>& subtable_ids)
  {
    तार्किक is_ext = is_extension (c.table_tag);
    स्वतः* v = &c.graph.vertices_[this_index];
    fix_existing_subtable_links (c, this_index, subtable_ids);

    अचिन्हित new_subtable_मोजणी = 0;
    साठी (स्थिरांक स्वतः& p : subtable_ids)
      new_subtable_मोजणी += p.second.length;

    माप_प्रकार new_size = v->table_size ()
                      + new_subtable_मोजणी * OT::Offset16::static_size;
    अक्षर* buffer = (अक्षर*) hb_calloc (1, new_size);
    जर (unlikely (!buffer)) परत Err(ALLOCATION_FAILURE);
    स्वतः res = c.add_buffer (buffer);
    जर (unlikely (!res.is_ok ()))
    {
      hb_free (buffer);
      परत res;
    }
    hb_memcpy (buffer, v->obj ().head, v->table_size());

    जर (use_mark_filtering_set ())
      hb_memcpy (buffer + new_size - 2, v->obj ().tail - 2, 2);

    v->set_buffer (buffer, buffer + new_size);

    Lookup* new_lookup = (Lookup*) buffer;

    अचिन्हित shift = 0;
    new_lookup->subTable.len = subTable.len + new_subtable_मोजणी;
    साठी (स्थिरांक स्वतः& p : subtable_ids)
    {
      अचिन्हित offset_index = p.first + shift + 1;
      shift += p.second.length;

      साठी (अचिन्हित subtable_id : p.second)
      {
        जर (is_ext)
        {
          TRY_ASSIGN (अचिन्हित ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.graph.vertices_[subtable_id].add_parent (ext_id, असत्य));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.graph.vertices_[this_index];
        }

        TRY(v->add_real_link(2, subtable_id,
          (अक्षर*) &new_lookup->subTable[offset_index++] - (अक्षर*) new_lookup));
        TRY(c.graph.vertices_[subtable_id].add_parent (this_index, असत्य));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->sort_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.set (this_index, new_lookup);
    TRY (graph_result_t<शून्य>::from (c.lookups, ALLOCATION_FAILURE));
    परत Ok();
  }

  शून्य fix_existing_subtable_links (gsubgpos_graph_context_t& c,
                                    अचिन्हित this_index,
                                    स्थिरांक hb_vector_t<hb_pair_t<अचिन्हित, hb_vector_t<अचिन्हित>>>& subtable_ids)
  {
    स्वतः& v = c.graph.vertices_[this_index];
    अचिन्हित shift = 0;
    साठी (स्थिरांक स्वतः& p : subtable_ids)
    {
      अचिन्हित घाला_index = p.first + shift;
      अचिन्हित pos_offset = p.second.length * OT::Offset16::static_size;
      अचिन्हित घाला_offset = Lookup::min_size + घाला_index * OT::Offset16::static_size;
      shift += p.second.length;

      साठी (स्वतः& l : v.all_links_writer ())
      {
        जर (l.position > घाला_offset) l.position += pos_offset;
      }
    }
  }

  graph_result_t<अचिन्हित> create_extension_subtable (gsubgpos_graph_context_t& c,
                                                      अचिन्हित subtable_index,
                                                      अचिन्हित type)
  {
    अचिन्हित extension_size = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_size;

    TRY_ASSIGN (अचिन्हित ext_index, c.create_गाठ (extension_size));

    स्वतः& ext_vertex = c.graph.vertices_[ext_index];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_vertex.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_vertex.add_real_link (4, subtable_index, 4));

    परत Ok(ext_index);
  }

  graph_result_t<शून्य> make_subtable_extension (gsubgpos_graph_context_t& c,
                                                अचिन्हित lookup_index,
                                                अचिन्हित subtable_index)
  {
    अचिन्हित type = lookupType;
    अचिन्हित ext_index = HB_GRAPH_INVALID;
    अचिन्हित* existing_ext_index = nullptr;
    जर (c.subtable_to_extension.has(subtable_index, &existing_ext_index)) {
      ext_index = *existing_ext_index;
    } अन्यथा {
      TRY_ASSIGN (ext_index, create_extension_subtable(c, subtable_index, type));
      c.subtable_to_extension.set(subtable_index, ext_index);
      TRY (graph_result_t<शून्य>::from (c.subtable_to_extension, ALLOCATION_FAILURE));
    }

    स्वतः& subtable_vertex = c.graph.vertices_[subtable_index];
    स्वतः& lookup_vertex = c.graph.vertices_[lookup_index];
    अचिन्हित replaced = 0;
    साठी (स्वतः& l : lookup_vertex.real_links_writer ())
    {
      जर (l.objidx == subtable_index) {
        // Change lookup to point at the extension.
        l.objidx = ext_index;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    स्वतः& ext_vertex = c.graph.vertices_[ext_index];
    साठी (अचिन्हित i = 0; i < replaced; i++)
    {
      subtable_vertex.remove_parent (lookup_index);
      TRY(ext_vertex.add_parent (lookup_index, असत्य));
    }
    जर (!existing_ext_index)
      TRY(subtable_vertex.add_parent (ext_index, असत्य));

    परत Ok();
  }

 खाजगी:
  तार्किक is_supported_gsub_type(अचिन्हित type, gsubgpos_graph_context_t& c) स्थिरांक {
    परत (c.table_tag == HB_OT_TAG_GSUB) && (
      type == OT::Layout::GSUB_impl::SubstLookupSubTable::Type::Ligature
    );
  }

  तार्किक is_supported_gpos_type(अचिन्हित type, gsubgpos_graph_context_t& c) स्थिरांक {
   परत (c.table_tag == HB_OT_TAG_GPOS) && (
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::Pair ||
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::MarkBase
    );
  }

  अचिन्हित extension_type (hb_tag_t table_tag) स्थिरांक
  {
    निवड (table_tag)
    {
    स्थिती HB_OT_TAG_GPOS: परत 9;
    स्थिती HB_OT_TAG_GSUB: परत 7;
    पूर्वनिर्धारित: परत 0;
    }
  }
};

ढाँचा <typename T>
संरचना LookupList : सार्वजनिक OT::LookupList<T>
{
  graph_result_t<शून्य> sanitize (स्थिरांक graph_t::vertex_t& vertex) स्थिरांक
  {
    माप_प्रकार vertex_len = vertex.table_size ();
    जर (unlikely (vertex_len < OT::LookupList<T>::min_size)) परत Err(SANITIZE_FAILURE);
    hb_barrier ();
    जर (unlikely (vertex_len < OT::LookupList<T>::item_size * हे->len)) परत Err(SANITIZE_FAILURE);
    परत Ok();
  }
};

संरचना GSTAR : सार्वजनिक OT::GSUBGPOS
{
  स्थिर graph_result_t<स्थिरांक GSTAR*> graph_to_gstar (graph_t& graph)
  {
    स्थिरांक स्वतः& r = graph.मूळ ();

    स्थिरांक GSTAR* gstar = (GSTAR*) r.obj ().head;
    जर (unlikely (!gstar)) परत Err(INVALID_ARGUMENT);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    परत gstar;
  }

  स्थिरांक शून्य* get_lookup_list_field_offset () स्थिरांक
  {
    परत OT::GSUBGPOS::get_lookup_list_field_offset ();
  }

  graph_result_t<शून्य> sanitize (स्थिरांक graph_t::vertex_t& vertex) स्थिरांक
  {
    माप_प्रकार len = vertex.table_size ();
    जर (unlikely (len < OT::GSUBGPOS::min_size)) परत Err(SANITIZE_FAILURE);
    hb_barrier ();
    जर (unlikely (version.major != 1)) परत Err(SANITIZE_FAILURE);
    जर (unlikely (len < get_size ())) परत Err(SANITIZE_FAILURE);
    परत Ok();
  }

  graph_result_t<शून्य> find_lookups (graph_t& graph,
                                     hb_hashmap_t<अचिन्हित, Lookup*>& lookups /* OUT */) स्थिरांक
  {
#ifndef HB_NO_BEYOND_64K
    जर (version.to_int () >= 0x00010002u && lookupList2)
      परत find_lookups<MediumTypes> (graph, lookups);
#endif
    TRY(find_lookups<SmallTypes> (graph, lookups));
    परत Ok();
  }

  graph_result_t<अचिन्हित> get_lookup_list_index (graph_t& graph) स्थिरांक
  {
    स्थिरांक शून्य* offset = get_lookup_list_field_offset ();
    जर (unlikely (!offset)) परत Err(INVALID_ARGUMENT);
    परत graph.index_for_offset (graph.मूळ_idx (), offset);
  }

  ढाँचा<typename Types>
  graph_result_t<शून्य> find_lookups (graph_t& graph,
                                     hb_hashmap_t<अचिन्हित, Lookup*>& lookups /* OUT */) स्थिरांक
  {
    TRY_ASSIGN (अचिन्हित lookup_list_idx, get_lookup_list_index (graph));
    स्थिरांक LookupList<Types>* lookupList =
        (स्थिरांक LookupList<Types>*) graph.object (lookup_list_idx).head;
    जर (unlikely (!lookupList)) परत Err(INVALID_ARGUMENT);
    TRY(lookupList->sanitize (graph.vertices_[lookup_list_idx]));

    साठी (अचिन्हित i = 0; i < lookupList->len; i++)
    {
      TRY_ASSIGN (अचिन्हित lookup_idx, graph.index_for_offset (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) graph.object (lookup_idx).head;
      जर (!lookup || !lookup->sanitize (graph.vertices_[lookup_idx]).is_ok ()) सुरूठेवा;
      lookups.set (lookup_idx, lookup);
    }
    परत Ok();
  }
};




}

#endif  /* GRAPH_GSUBGPOS_GRAPH_HH */
