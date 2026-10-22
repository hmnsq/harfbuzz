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

#అన్తర్భావ "graph.hh"
#అన్తర్భావ "../hb-ot-layout-gsubgpos.hh"
#అన్తర్భావ "../OT/Layout/GSUB/ExtensionSubst.hh"
#అన్తర్భావ "../OT/Layout/GSUB/SubstLookupSubTable.hh"
#అన్తర్భావ "gsubgpos-context.hh"
#అన్తర్భావ "pairpos-graph.hh"
#అన్తర్భావ "markbasepos-graph.hh"
#అన్తర్భావ "ligature-graph.hh"

#నిర్వచితం_కాకపోతే లేఖాచిత్రం_GSUBGPOS_లేఖాచిత్రం_HH
#నిర్వచించు లేఖాచిత్రం_GSUBGPOS_లేఖాచిత్రం_HH

నామాకాశ లేఖాచిత్రం {

నిర్మాణం Lookup;

మూస<రకం_పేరు T>
నిర్మాణం ExtensionFormat1 : బహిరంగ OT::ExtensionFormat1<T>
{
  శూన్య reset(గుర్తులేని type)
  {
    ఇది->format = 1;
    ఇది->extensionLookupType = type;
    ఇది->extension_స్థానభ్రంశం = 0;
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం) స్థిరరాశి
  {
    size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత ();
    ఒకవేళ (unlikely (శీర్షం_len < OT::ExtensionFormat1<T>::static_కొలత))
      తిరిగి Err(SANITIZE_FAILURE);
    తిరిగి Ok();
  }

  గుర్తులేని get_lookup_type () స్థిరరాశి
  {
    తిరిగి ఇది->extensionLookupType;
  }

  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> get_subtable_సూచీ (లేఖాచిత్రం_t& లేఖాచిత్రం, గుర్తులేని this_సూచీ) స్థిరరాశి
  {
    తిరిగి లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (this_సూచీ, &ఇది->extension_స్థానభ్రంశం);
  }
};

నిర్మాణం Lookup : బహిరంగ OT::Lookup
{
  గుర్తులేని సంఖ్య_యొక్క_subtables () స్థిరరాశి
  {
    తిరిగి sub_పట్టిక.len;
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం) స్థిరరాశి
  {
    size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత ();
    ఒకవేళ (unlikely (శీర్షం_len < OT::Lookup::min_కొలత)) తిరిగి Err(SANITIZE_FAILURE);
    hb_barrier ();
    ఒకవేళ (unlikely (శీర్షం_len < ఇది->get_కొలత ())) తిరిగి Err(SANITIZE_FAILURE);
    తిరిగి Ok();
  }

  తార్కిక is_extension (hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2) స్థిరరాశి
  {
    తిరిగి lookupType == extension_type (పట్టిక_ట్యాగ్_2);
  }

  తార్కిక use_mark_filtering_సమితి () స్థిరరాశి
  {
    గుర్తులేని flag = lookupFlag;
    తిరిగి flag & 0x0010u;
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> make_extension (gsubgpos_లేఖాచిత్రం_context_t& c,
                                       గుర్తులేని this_సూచీ)
  {
    గుర్తులేని type = lookupType;
    గుర్తులేని ext_type = extension_type (c.పట్టిక_ట్యాగ్_2);
    ఒకవేళ (!ext_type || is_extension (c.పట్టిక_ట్యాగ్_2))
    {
      // NOOP
      తిరిగి Ok();
    }

    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_సూచీ);

    hb_సమితి_t చూసిన;
    hb_సదిశ_t<గుర్తులేని> subtables;
    కొరకు (గుర్తులేని i = 0; i < sub_పట్టిక.len; i++)
    {
      TRY_కేటాయించు (గుర్తులేని subtable_సూచీ, c.లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (this_సూచీ, &sub_పట్టిక[i]));
      ఒకవేళ (చూసిన.has (subtable_సూచీ))
        కొనసాగు;
      చూసిన.కూడు (subtable_సూచీ);
      subtables.push (subtable_సూచీ);
    }
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (చూసిన, కేటాయింపు_FAILURE));
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (subtables, కేటాయింపు_FAILURE));

    కొరకు (గుర్తులేని subtable_సూచీ : subtables)
      TRY (make_subtable_extension (c, this_సూచీ, subtable_సూచీ));

    lookupType = ext_type;
    తిరిగి Ok();
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> విభజించు_subtables_if_needed (gsubgpos_లేఖాచిత్రం_context_t& c,
                                                  గుర్తులేని this_సూచీ)
  {
    గుర్తులేని type = lookupType;
    తార్కిక is_ext = is_extension (c.పట్టిక_ట్యాగ్_2);

    ఒకవేళ (c.పట్టిక_ట్యాగ్_2 != HB_OT_ట్యాగ్_GPOS && c.పట్టిక_ట్యాగ్_2 != HB_OT_ట్యాగ్_GSUB)
      తిరిగి Ok();

    ఒకవేళ (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      తిరిగి Ok();

    hb_సదిశ_t<hb_pair_t<గుర్తులేని, hb_సదిశ_t<గుర్తులేని>>> all_new_subtables;
    కొరకు (గుర్తులేని i = 0; i < sub_పట్టిక.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      స్వయంచాలక idx_res = c.లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (this_సూచీ, &sub_పట్టిక[i]);
      ఒకవేళ (!idx_res.is_ok ()) కొనసాగు;
      గుర్తులేని subtable_సూచీ = *idx_res;
      ఒకవేళ (is_ext) {
        గుర్తులేని ext_subtable_సూచీ = subtable_సూచీ;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.లేఖాచిత్రం.object (ext_subtable_సూచీ).తల_2;
        ఒకవేళ (!extension || !extension->sanitize (c.లేఖాచిత్రం.vertices_[ext_subtable_సూచీ]).is_ok ())
          కొనసాగు;

        TRY_కేటాయించు (subtable_సూచీ, extension->get_subtable_సూచీ (c.లేఖాచిత్రం, ext_subtable_సూచీ));
        type = extension->get_lookup_type ();
        ఒకవేళ (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          కొనసాగు;
      }

      hb_సదిశ_t<గుర్తులేని>* విభజించు_ఫలితం;
      ఒకవేళ (c.విభజించు_subtables.has (subtable_సూచీ, &విభజించు_ఫలితం))
      {
        ఒకవేళ (విభజించు_ఫలితం->నిడివి == 0)
          కొనసాగు;
        all_new_subtables.push (hb_pair(i, *విభజించు_ఫలితం));
        TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (all_new_subtables, కేటాయింపు_FAILURE));
      }
      లేకపోతే
      {
        hb_సదిశ_t<గుర్తులేని> new_sub_tables;

        ఒకవేళ (c.పట్టిక_ట్యాగ్_2 == HB_OT_ట్యాగ్_GPOS) {
          ఎంపిక (type)
          {
          స్థితి 2:
            {
              TRY_కేటాయించు (new_sub_tables, విభజించు_subtable<PairPos> (c, subtable_సూచీ));
              ఆపు;
            }
          స్థితి 4:
            {
              TRY_కేటాయించు (new_sub_tables, విభజించు_subtable<Mark_పునాది_Pos> (c, subtable_సూచీ));
              ఆపు;
            }
          అప్రమేయ:
            ఆపు;
          }
        } లేకపోతే ఒకవేళ (c.పట్టిక_ట్యాగ్_2 == HB_OT_ట్యాగ్_GSUB) {
          ఎంపిక (type)
          {
          స్థితి 4:
            {
              TRY_కేటాయించు (new_sub_tables, విభజించు_subtable<లేఖాచిత్రం::LigatureSubst> (c, subtable_సూచీ));
              ఆపు;
            }
          అప్రమేయ:
            ఆపు;
          }
        }

        c.విభజించు_subtables.సమితి (subtable_సూచీ, new_sub_tables);
        TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (c.విభజించు_subtables, కేటాయింపు_FAILURE));
        ఒకవేళ (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (all_new_subtables, కేటాయింపు_FAILURE));
        }
      }
    }

    ఒకవేళ (all_new_subtables) {
      TRY (కూడు_sub_tables (c, this_సూచీ, type, all_new_subtables));
    }

    తిరిగి Ok();
  }

  మూస<రకం_పేరు T>
  లేఖాచిత్రం_ఫలితం_t<hb_సదిశ_t<గుర్తులేని>> విభజించు_subtable (gsubgpos_లేఖాచిత్రం_context_t& c,
                                                        గుర్తులేని objidx)
  {
    T* sub_పట్టిక_2 = (T*) c.లేఖాచిత్రం.object (objidx).తల_2;
    ఒకవేళ (!sub_పట్టిక_2 || !sub_పట్టిక_2->sanitize (c.లేఖాచిత్రం.vertices_[objidx]).is_ok ())
      తిరిగి Ok(hb_సదిశ_t<గుర్తులేని> ());

    తిరిగి sub_పట్టిక_2->విభజించు_subtables (c, objidx);
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> కూడు_sub_tables (gsubgpos_లేఖాచిత్రం_context_t& c,
                                       గుర్తులేని this_సూచీ,
                                       గుర్తులేని type,
                                       స్థిరరాశి hb_సదిశ_t<hb_pair_t<గుర్తులేని, hb_సదిశ_t<గుర్తులేని>>>& subtable_ids)
  {
    తార్కిక is_ext = is_extension (c.పట్టిక_ట్యాగ్_2);
    స్వయంచాలక* v = &c.లేఖాచిత్రం.vertices_[this_సూచీ];
    fix_existing_subtable_links (c, this_సూచీ, subtable_ids);

    గుర్తులేని new_subtable_లెక్క = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక& p : subtable_ids)
      new_subtable_లెక్క += p.second.నిడివి;

    size_t new_కొలత = v->పట్టిక_కొలత ()
                      + new_subtable_లెక్క * OT::Offset16::static_కొలత;
    అక్షరం* బఫరు = (అక్షరం*) hb_calloc (1, new_కొలత);
    ఒకవేళ (unlikely (!బఫరు)) తిరిగి Err(కేటాయింపు_FAILURE);
    స్వయంచాలక res = c.కూడు_బఫరు (బఫరు);
    ఒకవేళ (unlikely (!res.is_ok ()))
    {
      hb_free (బఫరు);
      తిరిగి res;
    }
    hb_memcpy (బఫరు, v->obj ().తల_2, v->పట్టిక_కొలత());

    ఒకవేళ (use_mark_filtering_సమితి ())
      hb_memcpy (బఫరు + new_కొలత - 2, v->obj ().తోక - 2, 2);

    v->సమితి_బఫరు (బఫరు, బఫరు + new_కొలత);

    Lookup* new_lookup = (Lookup*) బఫరు;

    గుర్తులేని జరుపు = 0;
    new_lookup->sub_పట్టిక.len = sub_పట్టిక.len + new_subtable_లెక్క;
    కొరకు (స్థిరరాశి స్వయంచాలక& p : subtable_ids)
    {
      గుర్తులేని స్థానభ్రంశం_సూచీ = p.మొదటి + జరుపు + 1;
      జరుపు += p.second.నిడివి;

      కొరకు (గుర్తులేని subtable_id : p.second)
      {
        ఒకవేళ (is_ext)
        {
          TRY_కేటాయించు (గుర్తులేని ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.లేఖాచిత్రం.vertices_[subtable_id].కూడు_మాతృ (ext_id, అబద్ధం));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.లేఖాచిత్రం.vertices_[this_సూచీ];
        }

        TRY(v->కూడు_real_link(2, subtable_id,
          (అక్షరం*) &new_lookup->sub_పట్టిక[స్థానభ్రంశం_సూచీ++] - (అక్షరం*) new_lookup));
        TRY(c.లేఖాచిత్రం.vertices_[subtable_id].కూడు_మాతృ (this_సూచీ, అబద్ధం));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->క్రమీకరించు_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.సమితి (this_సూచీ, new_lookup);
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (c.lookups, కేటాయింపు_FAILURE));
    తిరిగి Ok();
  }

  శూన్య fix_existing_subtable_links (gsubgpos_లేఖాచిత్రం_context_t& c,
                                    గుర్తులేని this_సూచీ,
                                    స్థిరరాశి hb_సదిశ_t<hb_pair_t<గుర్తులేని, hb_సదిశ_t<గుర్తులేని>>>& subtable_ids)
  {
    స్వయంచాలక& v = c.లేఖాచిత్రం.vertices_[this_సూచీ];
    గుర్తులేని జరుపు = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక& p : subtable_ids)
    {
      గుర్తులేని చొప్పించు_సూచీ = p.మొదటి + జరుపు;
      గుర్తులేని pos_స్థానభ్రంశం = p.second.నిడివి * OT::Offset16::static_కొలత;
      గుర్తులేని చొప్పించు_స్థానభ్రంశం = Lookup::min_కొలత + చొప్పించు_సూచీ * OT::Offset16::static_కొలత;
      జరుపు += p.second.నిడివి;

      కొరకు (స్వయంచాలక& l : v.all_links_writer ())
      {
        ఒకవేళ (l.position > చొప్పించు_స్థానభ్రంశం) l.position += pos_స్థానభ్రంశం;
      }
    }
  }

  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> create_extension_subtable (gsubgpos_లేఖాచిత్రం_context_t& c,
                                                      గుర్తులేని subtable_సూచీ,
                                                      గుర్తులేని type)
  {
    గుర్తులేని extension_కొలత = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_కొలత;

    TRY_కేటాయించు (గుర్తులేని ext_సూచీ, c.create_కణుపు (extension_కొలత));

    స్వయంచాలక& ext_శీర్షం = c.లేఖాచిత్రం.vertices_[ext_సూచీ];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_శీర్షం.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_శీర్షం.కూడు_real_link (4, subtable_సూచీ, 4));

    తిరిగి Ok(ext_సూచీ);
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> make_subtable_extension (gsubgpos_లేఖాచిత్రం_context_t& c,
                                                గుర్తులేని lookup_సూచీ,
                                                గుర్తులేని subtable_సూచీ)
  {
    గుర్తులేని type = lookupType;
    గుర్తులేని ext_సూచీ = HB_లేఖాచిత్రం_INVALID;
    గుర్తులేని* existing_ext_సూచీ = శూన్యసూచిక;
    ఒకవేళ (c.subtable_గమ్యం_extension.has(subtable_సూచీ, &existing_ext_సూచీ)) {
      ext_సూచీ = *existing_ext_సూచీ;
    } లేకపోతే {
      TRY_కేటాయించు (ext_సూచీ, create_extension_subtable(c, subtable_సూచీ, type));
      c.subtable_గమ్యం_extension.సమితి(subtable_సూచీ, ext_సూచీ);
      TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (c.subtable_గమ్యం_extension, కేటాయింపు_FAILURE));
    }

    స్వయంచాలక& subtable_శీర్షం = c.లేఖాచిత్రం.vertices_[subtable_సూచీ];
    స్వయంచాలక& lookup_శీర్షం = c.లేఖాచిత్రం.vertices_[lookup_సూచీ];
    గుర్తులేని replaced = 0;
    కొరకు (స్వయంచాలక& l : lookup_శీర్షం.real_links_writer ())
    {
      ఒకవేళ (l.objidx == subtable_సూచీ) {
        // Change lookup to point at the extension.
        l.objidx = ext_సూచీ;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    స్వయంచాలక& ext_శీర్షం = c.లేఖాచిత్రం.vertices_[ext_సూచీ];
    కొరకు (గుర్తులేని i = 0; i < replaced; i++)
    {
      subtable_శీర్షం.remove_మాతృ (lookup_సూచీ);
      TRY(ext_శీర్షం.కూడు_మాతృ (lookup_సూచీ, అబద్ధం));
    }
    ఒకవేళ (!existing_ext_సూచీ)
      TRY(subtable_శీర్షం.కూడు_మాతృ (ext_సూచీ, అబద్ధం));

    తిరిగి Ok();
  }

 అంతరంగ:
  తార్కిక is_supported_gsub_type(గుర్తులేని type, gsubgpos_లేఖాచిత్రం_context_t& c) స్థిరరాశి {
    తిరిగి (c.పట్టిక_ట్యాగ్_2 == HB_OT_ట్యాగ్_GSUB) && (
      type == OT::Layout::GSUB_impl::Subst_Lookup_Sub_పట్టిక::Type::Ligature
    );
  }

  తార్కిక is_supported_gpos_type(గుర్తులేని type, gsubgpos_లేఖాచిత్రం_context_t& c) స్థిరరాశి {
   తిరిగి (c.పట్టిక_ట్యాగ్_2 == HB_OT_ట్యాగ్_GPOS) && (
      type == OT::Layout::GPOS_impl::Pos_Lookup_Sub_పట్టిక::Type::Pair ||
      type == OT::Layout::GPOS_impl::Pos_Lookup_Sub_పట్టిక::Type::Mark_పునాది
    );
  }

  గుర్తులేని extension_type (hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2) స్థిరరాశి
  {
    ఎంపిక (పట్టిక_ట్యాగ్_2)
    {
    స్థితి HB_OT_ట్యాగ్_GPOS: తిరిగి 9;
    స్థితి HB_OT_ట్యాగ్_GSUB: తిరిగి 7;
    అప్రమేయ: తిరిగి 0;
    }
  }
};

మూస <రకం_పేరు T>
నిర్మాణం LookupList : బహిరంగ OT::LookupList<T>
{
  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం) స్థిరరాశి
  {
    size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత ();
    ఒకవేళ (unlikely (శీర్షం_len < OT::LookupList<T>::min_కొలత)) తిరిగి Err(SANITIZE_FAILURE);
    hb_barrier ();
    ఒకవేళ (unlikely (శీర్షం_len < OT::LookupList<T>::అంశం_కొలత * ఇది->len)) తిరిగి Err(SANITIZE_FAILURE);
    తిరిగి Ok();
  }
};

నిర్మాణం GSTAR : బహిరంగ OT::GSUBGPOS
{
  స్థిర లేఖాచిత్రం_ఫలితం_t<స్థిరరాశి GSTAR*> లేఖాచిత్రం_గమ్యం_gstar (లేఖాచిత్రం_t& లేఖాచిత్రం)
  {
    స్థిరరాశి స్వయంచాలక& r = లేఖాచిత్రం.మూలం ();

    స్థిరరాశి GSTAR* gstar = (GSTAR*) r.obj ().తల_2;
    ఒకవేళ (unlikely (!gstar)) తిరిగి Err(INVALID_వాదం);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    తిరిగి gstar;
  }

  స్థిరరాశి శూన్య* get_lookup_list_క్షేత్రం_స్థానభ్రంశం () స్థిరరాశి
  {
    తిరిగి OT::GSUBGPOS::get_lookup_list_క్షేత్రం_స్థానభ్రంశం ();
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం) స్థిరరాశి
  {
    size_t len = శీర్షం.పట్టిక_కొలత ();
    ఒకవేళ (unlikely (len < OT::GSUBGPOS::min_కొలత)) తిరిగి Err(SANITIZE_FAILURE);
    hb_barrier ();
    ఒకవేళ (unlikely (సంస్కరణ.major != 1)) తిరిగి Err(SANITIZE_FAILURE);
    ఒకవేళ (unlikely (len < get_కొలత ())) తిరిగి Err(SANITIZE_FAILURE);
    తిరిగి Ok();
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> కనుగొను_lookups (లేఖాచిత్రం_t& లేఖాచిత్రం,
                                     hb_hashmap_t<గుర్తులేని, Lookup*>& lookups /*బహిర్గతం*/) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010002u && lookupList2)
      తిరిగి కనుగొను_lookups<MediumTypes> (లేఖాచిత్రం, lookups);
#అయితే_ముగింపు
    TRY(కనుగొను_lookups<SmallTypes> (లేఖాచిత్రం, lookups));
    తిరిగి Ok();
  }

  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> get_lookup_list_సూచీ (లేఖాచిత్రం_t& లేఖాచిత్రం) స్థిరరాశి
  {
    స్థిరరాశి శూన్య* స్థానభ్రంశం = get_lookup_list_క్షేత్రం_స్థానభ్రంశం ();
    ఒకవేళ (unlikely (!స్థానభ్రంశం)) తిరిగి Err(INVALID_వాదం);
    తిరిగి లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (లేఖాచిత్రం.మూలం_idx (), స్థానభ్రంశం);
  }

  మూస<రకం_పేరు Types>
  లేఖాచిత్రం_ఫలితం_t<శూన్య> కనుగొను_lookups (లేఖాచిత్రం_t& లేఖాచిత్రం,
                                     hb_hashmap_t<గుర్తులేని, Lookup*>& lookups /*బహిర్గతం*/) స్థిరరాశి
  {
    TRY_కేటాయించు (గుర్తులేని lookup_list_idx, get_lookup_list_సూచీ (లేఖాచిత్రం));
    స్థిరరాశి LookupList<Types>* lookupList =
        (స్థిరరాశి LookupList<Types>*) లేఖాచిత్రం.object (lookup_list_idx).తల_2;
    ఒకవేళ (unlikely (!lookupList)) తిరిగి Err(INVALID_వాదం);
    TRY(lookupList->sanitize (లేఖాచిత్రం.vertices_[lookup_list_idx]));

    కొరకు (గుర్తులేని i = 0; i < lookupList->len; i++)
    {
      TRY_కేటాయించు (గుర్తులేని lookup_idx, లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) లేఖాచిత్రం.object (lookup_idx).తల_2;
      ఒకవేళ (!lookup || !lookup->sanitize (లేఖాచిత్రం.vertices_[lookup_idx]).is_ok ()) కొనసాగు;
      lookups.సమితి (lookup_idx, lookup);
    }
    తిరిగి Ok();
  }
};




}

#అయితే_ముగింపు  /* GRAPH_GSUBGPOS_GRAPH_HH */
