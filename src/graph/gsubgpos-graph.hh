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

#समावेश "graph.hh"
#समावेश "../hb-ot-layout-gsubgpos.hh"
#समावेश "../OT/Layout/GSUB/ExtensionSubst.hh"
#समावेश "../OT/Layout/GSUB/SubstLookupSubTable.hh"
#समावेश "gsubgpos-context.hh"
#समावेश "pairpos-graph.hh"
#समावेश "markbasepos-graph.hh"
#समावेश "ligature-graph.hh"

#अगर_अघोषित GRAPH_GSUBGPOS_GRAPH_HH
#घोषणा GRAPH_GSUBGPOS_GRAPH_HH

namespace graph {

काष्ठा Lookup;

ढाँचा<typename T>
काष्ठा ExtensionFormat1 : खुला OT::ExtensionFormat1<T>
{
  व्योम reset(अचिन्हित type)
  {
    यह->format = 1;
    यह->extensionLookupType = type;
    यह->extensionOffset = 0;
  }

  graph_परिणाम_t<व्योम> sanitize (स्थिर graph_t::vertex_t& vertex) स्थिर
  {
    माप_प्रकार vertex_len = vertex.table_आकार ();
    अगर (unlikely (vertex_len < OT::ExtensionFormat1<T>::static_आकार))
      वापस Err(SANITIZE_FAILURE);
    वापस Ok();
  }

  अचिन्हित get_lookup_type () स्थिर
  {
    वापस यह->extensionLookupType;
  }

  graph_परिणाम_t<अचिन्हित> get_subtable_सूचकांक (graph_t& graph, अचिन्हित this_सूचकांक) स्थिर
  {
    वापस graph.सूचकांक_for_offset (this_सूचकांक, &यह->extensionOffset);
  }
};

काष्ठा Lookup : खुला OT::Lookup
{
  अचिन्हित संख्या_of_subtables () स्थिर
  {
    वापस subTable.len;
  }

  graph_परिणाम_t<व्योम> sanitize (स्थिर graph_t::vertex_t& vertex) स्थिर
  {
    माप_प्रकार vertex_len = vertex.table_आकार ();
    अगर (unlikely (vertex_len < OT::Lookup::min_आकार)) वापस Err(SANITIZE_FAILURE);
    hb_barrier ();
    अगर (unlikely (vertex_len < यह->get_आकार ())) वापस Err(SANITIZE_FAILURE);
    वापस Ok();
  }

  bool is_extension (hb_tag_t table_tag) स्थिर
  {
    वापस lookupType == extension_type (table_tag);
  }

  bool use_mark_filtering_set () स्थिर
  {
    अचिन्हित flag = lookupFlag;
    वापस flag & 0x0010u;
  }

  graph_परिणाम_t<व्योम> make_extension (gsubgpos_graph_context_t& c,
                                       अचिन्हित this_सूचकांक)
  {
    अचिन्हित type = lookupType;
    अचिन्हित ext_type = extension_type (c.table_tag);
    अगर (!ext_type || is_extension (c.table_tag))
    {
      // NOOP
      वापस Ok();
    }

    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_सूचकांक);

    hb_set_t seen;
    hb_vector_t<अचिन्हित> subtables;
    क्रम (अचिन्हित i = 0; i < subTable.len; i++)
    {
      TRY_ASSIGN (अचिन्हित subtable_सूचकांक, c.graph.सूचकांक_for_offset (this_सूचकांक, &subTable[i]));
      अगर (seen.has (subtable_सूचकांक))
        जारी;
      seen.add (subtable_सूचकांक);
      subtables.push (subtable_सूचकांक);
    }
    TRY (graph_परिणाम_t<व्योम>::from (seen, ALLOCATION_FAILURE));
    TRY (graph_परिणाम_t<व्योम>::from (subtables, ALLOCATION_FAILURE));

    क्रम (अचिन्हित subtable_सूचकांक : subtables)
      TRY (make_subtable_extension (c, this_सूचकांक, subtable_सूचकांक));

    lookupType = ext_type;
    वापस Ok();
  }

  graph_परिणाम_t<व्योम> split_subtables_if_needed (gsubgpos_graph_context_t& c,
                                                  अचिन्हित this_सूचकांक)
  {
    अचिन्हित type = lookupType;
    bool is_ext = is_extension (c.table_tag);

    अगर (c.table_tag != HB_OT_TAG_GPOS && c.table_tag != HB_OT_TAG_GSUB)
      वापस Ok();

    अगर (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      वापस Ok();

    hb_vector_t<hb_pair_t<अचिन्हित, hb_vector_t<अचिन्हित>>> all_new_subtables;
    क्रम (अचिन्हित i = 0; i < subTable.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      स्वतः idx_res = c.graph.सूचकांक_for_offset (this_सूचकांक, &subTable[i]);
      अगर (!idx_res.is_ok ()) जारी;
      अचिन्हित subtable_सूचकांक = *idx_res;
      अगर (is_ext) {
        अचिन्हित ext_subtable_सूचकांक = subtable_सूचकांक;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.graph.object (ext_subtable_सूचकांक).शीर्ष_2;
        अगर (!extension || !extension->sanitize (c.graph.vertices_[ext_subtable_सूचकांक]).is_ok ())
          जारी;

        TRY_ASSIGN (subtable_सूचकांक, extension->get_subtable_सूचकांक (c.graph, ext_subtable_सूचकांक));
        type = extension->get_lookup_type ();
        अगर (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          जारी;
      }

      hb_vector_t<अचिन्हित>* split_परिणाम;
      अगर (c.split_subtables.has (subtable_सूचकांक, &split_परिणाम))
      {
        अगर (split_परिणाम->लंबाई == 0)
          जारी;
        all_new_subtables.push (hb_pair(i, *split_परिणाम));
        TRY (graph_परिणाम_t<व्योम>::from (all_new_subtables, ALLOCATION_FAILURE));
      }
      अन्यथा
      {
        hb_vector_t<अचिन्हित> new_sub_tables;

        अगर (c.table_tag == HB_OT_TAG_GPOS) {
          चयन (type)
          {
          हाल 2:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<PairPos> (c, subtable_सूचकांक));
              अवरोध;
            }
          हाल 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<MarkBasePos> (c, subtable_सूचकांक));
              अवरोध;
            }
          शेष:
            अवरोध;
          }
        } अन्यथा अगर (c.table_tag == HB_OT_TAG_GSUB) {
          चयन (type)
          {
          हाल 4:
            {
              TRY_ASSIGN (new_sub_tables, split_subtable<graph::LigatureSubst> (c, subtable_सूचकांक));
              अवरोध;
            }
          शेष:
            अवरोध;
          }
        }

        c.split_subtables.set (subtable_सूचकांक, new_sub_tables);
        TRY (graph_परिणाम_t<व्योम>::from (c.split_subtables, ALLOCATION_FAILURE));
        अगर (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (graph_परिणाम_t<व्योम>::from (all_new_subtables, ALLOCATION_FAILURE));
        }
      }
    }

    अगर (all_new_subtables) {
      TRY (add_sub_tables (c, this_सूचकांक, type, all_new_subtables));
    }

    वापस Ok();
  }

  ढाँचा<typename T>
  graph_परिणाम_t<hb_vector_t<अचिन्हित>> split_subtable (gsubgpos_graph_context_t& c,
                                                        अचिन्हित objidx)
  {
    T* sub_table = (T*) c.graph.object (objidx).शीर्ष_2;
    अगर (!sub_table || !sub_table->sanitize (c.graph.vertices_[objidx]).is_ok ())
      वापस Ok(hb_vector_t<अचिन्हित> ());

    वापस sub_table->split_subtables (c, objidx);
  }

  graph_परिणाम_t<व्योम> add_sub_tables (gsubgpos_graph_context_t& c,
                                       अचिन्हित this_सूचकांक,
                                       अचिन्हित type,
                                       स्थिर hb_vector_t<hb_pair_t<अचिन्हित, hb_vector_t<अचिन्हित>>>& subtable_ids)
  {
    bool is_ext = is_extension (c.table_tag);
    स्वतः* v = &c.graph.vertices_[this_सूचकांक];
    fix_existing_subtable_links (c, this_सूचकांक, subtable_ids);

    अचिन्हित new_subtable_गिनती = 0;
    क्रम (स्थिर स्वतः& p : subtable_ids)
      new_subtable_गिनती += p.second.लंबाई;

    माप_प्रकार new_आकार = v->table_आकार ()
                      + new_subtable_गिनती * OT::Offset16::static_आकार;
    अक्षर* बफर = (अक्षर*) hb_calloc (1, new_आकार);
    अगर (unlikely (!बफर)) वापस Err(ALLOCATION_FAILURE);
    स्वतः res = c.add_बफर (बफर);
    अगर (unlikely (!res.is_ok ()))
    {
      hb_free (बफर);
      वापस res;
    }
    hb_memcpy (बफर, v->obj ().शीर्ष_2, v->table_आकार());

    अगर (use_mark_filtering_set ())
      hb_memcpy (बफर + new_आकार - 2, v->obj ().पुच्छ - 2, 2);

    v->set_बफर (बफर, बफर + new_आकार);

    Lookup* new_lookup = (Lookup*) बफर;

    अचिन्हित shift = 0;
    new_lookup->subTable.len = subTable.len + new_subtable_गिनती;
    क्रम (स्थिर स्वतः& p : subtable_ids)
    {
      अचिन्हित offset_सूचकांक = p.first + shift + 1;
      shift += p.second.लंबाई;

      क्रम (अचिन्हित subtable_id : p.second)
      {
        अगर (is_ext)
        {
          TRY_ASSIGN (अचिन्हित ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.graph.vertices_[subtable_id].add_parent (ext_id, false));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.graph.vertices_[this_सूचकांक];
        }

        TRY(v->add_real_link(2, subtable_id,
          (अक्षर*) &new_lookup->subTable[offset_सूचकांक++] - (अक्षर*) new_lookup));
        TRY(c.graph.vertices_[subtable_id].add_parent (this_सूचकांक, false));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->sort_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.set (this_सूचकांक, new_lookup);
    TRY (graph_परिणाम_t<व्योम>::from (c.lookups, ALLOCATION_FAILURE));
    वापस Ok();
  }

  व्योम fix_existing_subtable_links (gsubgpos_graph_context_t& c,
                                    अचिन्हित this_सूचकांक,
                                    स्थिर hb_vector_t<hb_pair_t<अचिन्हित, hb_vector_t<अचिन्हित>>>& subtable_ids)
  {
    स्वतः& v = c.graph.vertices_[this_सूचकांक];
    अचिन्हित shift = 0;
    क्रम (स्थिर स्वतः& p : subtable_ids)
    {
      अचिन्हित जोड़ो_सूचकांक = p.first + shift;
      अचिन्हित pos_offset = p.second.लंबाई * OT::Offset16::static_आकार;
      अचिन्हित जोड़ो_offset = Lookup::min_आकार + जोड़ो_सूचकांक * OT::Offset16::static_आकार;
      shift += p.second.लंबाई;

      क्रम (स्वतः& l : v.all_links_writer ())
      {
        अगर (l.position > जोड़ो_offset) l.position += pos_offset;
      }
    }
  }

  graph_परिणाम_t<अचिन्हित> create_extension_subtable (gsubgpos_graph_context_t& c,
                                                      अचिन्हित subtable_सूचकांक,
                                                      अचिन्हित type)
  {
    अचिन्हित extension_आकार = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_आकार;

    TRY_ASSIGN (अचिन्हित ext_सूचकांक, c.create_गाँठ (extension_आकार));

    स्वतः& ext_vertex = c.graph.vertices_[ext_सूचकांक];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_vertex.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_vertex.add_real_link (4, subtable_सूचकांक, 4));

    वापस Ok(ext_सूचकांक);
  }

  graph_परिणाम_t<व्योम> make_subtable_extension (gsubgpos_graph_context_t& c,
                                                अचिन्हित lookup_सूचकांक,
                                                अचिन्हित subtable_सूचकांक)
  {
    अचिन्हित type = lookupType;
    अचिन्हित ext_सूचकांक = HB_GRAPH_INVALID;
    अचिन्हित* existing_ext_सूचकांक = nullptr;
    अगर (c.subtable_to_extension.has(subtable_सूचकांक, &existing_ext_सूचकांक)) {
      ext_सूचकांक = *existing_ext_सूचकांक;
    } अन्यथा {
      TRY_ASSIGN (ext_सूचकांक, create_extension_subtable(c, subtable_सूचकांक, type));
      c.subtable_to_extension.set(subtable_सूचकांक, ext_सूचकांक);
      TRY (graph_परिणाम_t<व्योम>::from (c.subtable_to_extension, ALLOCATION_FAILURE));
    }

    स्वतः& subtable_vertex = c.graph.vertices_[subtable_सूचकांक];
    स्वतः& lookup_vertex = c.graph.vertices_[lookup_सूचकांक];
    अचिन्हित replaced = 0;
    क्रम (स्वतः& l : lookup_vertex.real_links_writer ())
    {
      अगर (l.objidx == subtable_सूचकांक) {
        // Change lookup to point at the extension.
        l.objidx = ext_सूचकांक;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    स्वतः& ext_vertex = c.graph.vertices_[ext_सूचकांक];
    क्रम (अचिन्हित i = 0; i < replaced; i++)
    {
      subtable_vertex.remove_parent (lookup_सूचकांक);
      TRY(ext_vertex.add_parent (lookup_सूचकांक, false));
    }
    अगर (!existing_ext_सूचकांक)
      TRY(subtable_vertex.add_parent (ext_सूचकांक, false));

    वापस Ok();
  }

 निजी:
  bool is_supported_gsub_type(अचिन्हित type, gsubgpos_graph_context_t& c) स्थिर {
    वापस (c.table_tag == HB_OT_TAG_GSUB) && (
      type == OT::Layout::GSUB_impl::SubstLookupSubTable::Type::Ligature
    );
  }

  bool is_supported_gpos_type(अचिन्हित type, gsubgpos_graph_context_t& c) स्थिर {
   वापस (c.table_tag == HB_OT_TAG_GPOS) && (
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::Pair ||
      type == OT::Layout::GPOS_impl::PosLookupSubTable::Type::MarkBase
    );
  }

  अचिन्हित extension_type (hb_tag_t table_tag) स्थिर
  {
    चयन (table_tag)
    {
    हाल HB_OT_TAG_GPOS: वापस 9;
    हाल HB_OT_TAG_GSUB: वापस 7;
    शेष: वापस 0;
    }
  }
};

ढाँचा <typename T>
काष्ठा LookupList : खुला OT::LookupList<T>
{
  graph_परिणाम_t<व्योम> sanitize (स्थिर graph_t::vertex_t& vertex) स्थिर
  {
    माप_प्रकार vertex_len = vertex.table_आकार ();
    अगर (unlikely (vertex_len < OT::LookupList<T>::min_आकार)) वापस Err(SANITIZE_FAILURE);
    hb_barrier ();
    अगर (unlikely (vertex_len < OT::LookupList<T>::item_आकार * यह->len)) वापस Err(SANITIZE_FAILURE);
    वापस Ok();
  }
};

काष्ठा GSTAR : खुला OT::GSUBGPOS
{
  अटल graph_परिणाम_t<स्थिर GSTAR*> graph_to_gstar (graph_t& graph)
  {
    स्थिर स्वतः& r = graph.मूल ();

    स्थिर GSTAR* gstar = (GSTAR*) r.obj ().शीर्ष_2;
    अगर (unlikely (!gstar)) वापस Err(INVALID_ARGUMENT);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    वापस gstar;
  }

  स्थिर व्योम* get_lookup_list_field_offset () स्थिर
  {
    वापस OT::GSUBGPOS::get_lookup_list_field_offset ();
  }

  graph_परिणाम_t<व्योम> sanitize (स्थिर graph_t::vertex_t& vertex) स्थिर
  {
    माप_प्रकार len = vertex.table_आकार ();
    अगर (unlikely (len < OT::GSUBGPOS::min_आकार)) वापस Err(SANITIZE_FAILURE);
    hb_barrier ();
    अगर (unlikely (version.major != 1)) वापस Err(SANITIZE_FAILURE);
    अगर (unlikely (len < get_आकार ())) वापस Err(SANITIZE_FAILURE);
    वापस Ok();
  }

  graph_परिणाम_t<व्योम> find_lookups (graph_t& graph,
                                     hb_hashmap_t<अचिन्हित, Lookup*>& lookups /* OUT */) स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010002u && lookupList2)
      वापस find_lookups<MediumTypes> (graph, lookups);
#पूर्ण_अगर
    TRY(find_lookups<SmallTypes> (graph, lookups));
    वापस Ok();
  }

  graph_परिणाम_t<अचिन्हित> get_lookup_list_सूचकांक (graph_t& graph) स्थिर
  {
    स्थिर व्योम* offset = get_lookup_list_field_offset ();
    अगर (unlikely (!offset)) वापस Err(INVALID_ARGUMENT);
    वापस graph.सूचकांक_for_offset (graph.मूल_idx (), offset);
  }

  ढाँचा<typename Types>
  graph_परिणाम_t<व्योम> find_lookups (graph_t& graph,
                                     hb_hashmap_t<अचिन्हित, Lookup*>& lookups /* OUT */) स्थिर
  {
    TRY_ASSIGN (अचिन्हित lookup_list_idx, get_lookup_list_सूचकांक (graph));
    स्थिर LookupList<Types>* lookupList =
        (स्थिर LookupList<Types>*) graph.object (lookup_list_idx).शीर्ष_2;
    अगर (unlikely (!lookupList)) वापस Err(INVALID_ARGUMENT);
    TRY(lookupList->sanitize (graph.vertices_[lookup_list_idx]));

    क्रम (अचिन्हित i = 0; i < lookupList->len; i++)
    {
      TRY_ASSIGN (अचिन्हित lookup_idx, graph.सूचकांक_for_offset (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) graph.object (lookup_idx).शीर्ष_2;
      अगर (!lookup || !lookup->sanitize (graph.vertices_[lookup_idx]).is_ok ()) जारी;
      lookups.set (lookup_idx, lookup);
    }
    वापस Ok();
  }
};




}

#पूर्ण_अगर  /* GRAPH_GSUBGPOS_GRAPH_HH */
