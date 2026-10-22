
/*
 * Copyright © 2025  Google, Inc.
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

#నిర్వచితం_కాకపోతే లేఖాచిత్రం_LIGATURE_లేఖాచిత్రం_HH
#నిర్వచించు లేఖాచిత్రం_LIGATURE_లేఖాచిత్రం_HH

#అన్తర్భావ "graph.hh"
#అన్తర్భావ "../OT/Layout/GSUB/LigatureSubst.hh"
#అన్తర్భావ "../OT/Layout/GSUB/LigatureSubstFormat1.hh"
#అన్తర్భావ "../OT/Layout/GSUB/LigatureSet.hh"
#అన్తర్భావ "../hb-ot-dual.hh"
#అన్తర్భావ <algorithm>
#అన్తర్భావ <utility>

నామాకాశ లేఖాచిత్రం {

నిర్మాణం Ligature_సమితి : బహిరంగ OT::Layout::GSUB_impl::Ligature_సమితి<SmallTypes>
{
  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం) స్థిరరాశి
  {
    size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత ();
    ఒకవేళ (unlikely (శీర్షం_len < OT::Layout::GSUB_impl::Ligature_సమితి<SmallTypes>::min_కొలత)) తిరిగి Err(SANITIZE_FAILURE);
    hb_barrier ();

    size_t మొత్తం_len = ligature.get_కొలత() + OT::Layout::GSUB_impl::Ligature_సమితి<SmallTypes>::min_కొలత - ligature.len.get_కొలత();
    ఒకవేళ (unlikely (శీర్షం_len < మొత్తం_len)) {
      తిరిగి Err(SANITIZE_FAILURE);
    }
    తిరిగి Ok();
  }
};

నిర్మాణం LigatureSubstFormat1 : బహిరంగ OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>
{
  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం) స్థిరరాశి
  {
    size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత ();
    గుర్తులేని min_కొలత = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_కొలత;
    ఒకవేళ (unlikely (శీర్షం_len < min_కొలత)) తిరిగి Err(SANITIZE_FAILURE);
    hb_barrier ();

    ఒకవేళ (unlikely (శీర్షం_len < min_కొలత + ligature_సమితి.get_కొలత() - ligature_సమితి.len.get_కొలత()))
      తిరిగి Err(SANITIZE_FAILURE);
    తిరిగి Ok();
  }

  లేఖాచిత్రం_ఫలితం_t<hb_సదిశ_t<గుర్తులేని>> విభజించు_subtables (gsubgpos_లేఖాచిత్రం_context_t& c,
                                                         గుర్తులేని this_సూచీ)
  {
    TRY_కేటాయించు (స్వయంచాలక విభజించు_points, compute_విభజించు_points(c, this_సూచీ));
    ఒకవేళ (!విభజించు_points)
      తిరిగి Ok(hb_సదిశ_t<గుర్తులేని> ());

    TRY_కేటాయించు (గుర్తులేని మొత్తం_ligas, మొత్తం_సంఖ్య_ligas(c, this_సూచీ));
    TRY_కేటాయించు (hb_సదిశ_t<గుర్తులేని> counts, liga_counts(c, this_సూచీ));
    విభజించు_context_t విభజించు_context {
      c,
      ఇది,
      this_సూచీ,
      మొత్తం_ligas,
      std::move (counts),
    };
    తిరిగి actuate_subtable_విభజించు<విభజించు_context_t> (విభజించు_context, విభజించు_points);
  }

 అంతరంగ:
  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> మొత్తం_సంఖ్య_ligas(gsubgpos_లేఖాచిత్రం_context_t& c, గుర్తులేని this_సూచీ) స్థిరరాశి {
    గుర్తులేని మొత్తం = 0;
    కొరకు (గుర్తులేని i = 0; i < ligature_సమితి.len; i++)
    {
      TRY_కేటాయించు (స్వయంచాలక liga_సమితి, c.లేఖాచిత్రం.as_పట్టిక<Ligature_సమితి>(this_సూచీ, &ligature_సమితి[i]));
      మొత్తం += liga_సమితి.పట్టిక->ligature.len;
    }
    తిరిగి Ok(మొత్తం);
  }

  లేఖాచిత్రం_ఫలితం_t<hb_సదిశ_t<గుర్తులేని>> liga_counts(gsubgpos_లేఖాచిత్రం_context_t& c, గుర్తులేని this_సూచీ) స్థిరరాశి {
    hb_సదిశ_t<గుర్తులేని> ఫలితం;
    కొరకు (గుర్తులేని i = 0; i < ligature_సమితి.len; i++)
    {
      TRY_కేటాయించు (స్వయంచాలక liga_సమితి, c.లేఖాచిత్రం.as_పట్టిక<Ligature_సమితి>(this_సూచీ, &ligature_సమితి[i]));
      ఫలితం.push(!liga_సమితి.పట్టిక ? 0 : liga_సమితి.పట్టిక->ligature.len);
    }
    తిరిగి లేఖాచిత్రం_ఫలితం_t<hb_సదిశ_t<గుర్తులేని>>::from(std::move(ఫలితం), కేటాయింపు_FAILURE);
  }

  మూస <లేఖాచిత్రం_t::శీర్షం_mutability_t mutability>
  లేఖాచిత్రం_ఫలితం_t<hb_సదిశ_t<గుర్తులేని>> ligature_సూచీ_గమ్యం_object_id(స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_and_పట్టిక_t<Ligature_సమితి, mutability>& liga_సమితి) స్థిరరాశి {
    hb_సదిశ_t<గుర్తులేని> map;
    ఒకవేళ (!map.resize_exact(liga_సమితి.పట్టిక->ligature.len)) తిరిగి Err(కేటాయింపు_FAILURE);

    కొరకు (గుర్తులేని i = 0; i < map.నిడివి; i++) {
      map[i] = HB_లేఖాచిత్రం_INVALID;
    }

    కొరకు (స్థిరరాశి స్వయంచాలక& l : liga_సమితి.శీర్షం->obj ().real_links) {
      ఒకవేళ (l.position < 2) కొనసాగు;
      గుర్తులేని array_సూచీ = (l.position - 2) / 2;
      map[array_సూచీ] = l.objidx;
    }
    తిరిగి map;
  }

  లేఖాచిత్రం_ఫలితం_t<hb_సదిశ_t<గుర్తులేని>> compute_విభజించు_points(gsubgpos_లేఖాచిత్రం_context_t& c,
                                                             గుర్తులేని this_సూచీ) స్థిరరాశి
  {
    // For ligature subst coverage is always packed last, and as a result is where an overflow
    // will happen if there is one, so we can check the estimate length of the
    // LigatureSubstFormat1 -> Coverage offset length which is the sum of all data in the
    // retained sub graph except for the coverage table itself.
    స్థిరరాశి గుర్తులేని పునాది_కొలత = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_కొలత;
    గుర్తులేని accumulated = పునాది_కొలత;

    గుర్తులేని ligature_సూచీ = 0;
    hb_సదిశ_t<గుర్తులేని> విభజించు_points;
    కొరకు (గుర్తులేని i = 0; i < ligature_సమితి.len; i++)
    {
      accumulated += OT::HBUINT16::static_కొలత; // for ligature set offset
      accumulated += OT::Layout::GSUB_impl::Ligature_సమితి<SmallTypes>::min_కొలత; // for ligature set table

      TRY_కేటాయించు (స్వయంచాలక liga_సమితి, c.లేఖాచిత్రం.as_పట్టిక<Ligature_సమితి>(this_సూచీ, &ligature_సమితి[i]));

      // Finding the object id associated with an array index is O(n)
      // so to avoid O(n^2), precompute the mapping by scanning through
      // all links
      TRY_కేటాయించు (స్వయంచాలక సూచీ_గమ్యం_id, ligature_సూచీ_గమ్యం_object_id (liga_సమితి));

      కొరకు (గుర్తులేని j = 0; j < liga_సమితి.పట్టిక->ligature.len; j++)
      {
        స్థిరరాశి గుర్తులేని liga_id = సూచీ_గమ్యం_id[j];
        ఒకవేళ (liga_id == HB_లేఖాచిత్రం_INVALID) కొనసాగు; // no outgoing link, ignore
        స్థిరరాశి గుర్తులేని liga_కొలత = c.లేఖాచిత్రం.vertices_[liga_id].పట్టిక_కొలత ();

        accumulated += OT::HBUINT16::static_కొలత; // for ligature offset
        accumulated += liga_కొలత; // for the ligature table

        ఒకవేళ (accumulated >= (1 << 16))
        {
          విభజించు_points.push(ligature_సూచీ);
          // We're going to split such that the current ligature will be in the new sub table.
          // That means we'll have one ligature subst (base_base), one ligature set, and one liga table
          accumulated = పునాది_కొలత + // for liga subst subtable
            (OT::HBUINT16::static_కొలత * 2) + // for liga set and liga offset
            OT::Layout::GSUB_impl::Ligature_సమితి<SmallTypes>::min_కొలత + // for liga set subtable
            liga_కొలత; // for liga sub table
        }

        ligature_సూచీ++;
      }
    }

    తిరిగి విభజించు_points;
  }

  నిర్మాణం విభజించు_context_t
  {
    gsubgpos_లేఖాచిత్రం_context_t& c;
    LigatureSubstFormat1* thiz;
    గుర్తులేని this_సూచీ;
    గుర్తులేని original_లెక్క;
    hb_సదిశ_t<గుర్తులేని> liga_counts;

    గుర్తులేని original_లెక్క_2 ()
    {
      తిరిగి original_లెక్క;
    }

    లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> clone_range (గుర్తులేని ప్రారంభం, గుర్తులేని చివర)
    {
      తిరిగి thiz->clone_range (c, this_సూచీ, liga_counts, ప్రారంభం, చివర);
    }

    లేఖాచిత్రం_ఫలితం_t<శూన్య> shrink (గుర్తులేని లెక్క)
    {
      తిరిగి thiz->shrink (c, this_సూచీ, original_లెక్క_2(), liga_counts, లెక్క);
    }
  };

  లేఖాచిత్రం_ఫలితం_t<hb_pair_t<గుర్తులేని, Ligature_సమితి*>> new_liga_సమితి(gsubgpos_లేఖాచిత్రం_context_t& c, గుర్తులేని లెక్క) స్థిరరాశి {
    గుర్తులేని ప్రధానసంఖ్య_కొలత = OT::Layout::GSUB_impl::Ligature_సమితి<SmallTypes>::min_కొలత
                          + లెక్క * SmallTypes::కొలత;

    TRY_కేటాయించు (గుర్తులేని ప్రధానసంఖ్య_id, c.create_కణుపు (ప్రధానసంఖ్య_కొలత));

    Ligature_సమితి* ప్రధానసంఖ్య = (Ligature_సమితి*) c.లేఖాచిత్రం.object (ప్రధానసంఖ్య_id).తల_2;
    ప్రధానసంఖ్య->ligature.len = లెక్క;
    తిరిగి Ok(hb_pair(ప్రధానసంఖ్య_id, ప్రధానసంఖ్య));
  }

  శూన్య clear_virtual_links (gsubgpos_లేఖాచిత్రం_context_t& c, గుర్తులేని కణుపు_సూచీ) స్థిరరాశి
  {
    స్వయంచాలక& v  = c.లేఖాచిత్రం.vertices_[కణుపు_సూచీ];
    కొరకు (స్థిరరాశి స్వయంచాలక& l : v.obj ().virtual_links)
    {
      స్వయంచాలక& బిడ్డ = c.లేఖాచిత్రం.vertices_[l.objidx];
      బిడ్డ.remove_మాతృ(కణుపు_సూచీ);
    }
    v.clear_virtual_links ();
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> కూడు_virtual_link(gsubgpos_లేఖాచిత్రం_context_t& c, గుర్తులేని from, గుర్తులేని to) స్థిరరాశి {
    ఒకవేళ (unlikely (from >= c.లేఖాచిత్రం.vertices_.నిడివి || to >= c.లేఖాచిత్రం.vertices_.నిడివి)) తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);
    TRY(c.లేఖాచిత్రం.vertices_[to].కూడు_మాతృ (from, నిజం));
    TRY(c.లేఖాచిత్రం.vertices_[from].కూడు_virtual_link (to));
    తిరిగి Ok();
  }

  hb_pair_t<గుర్తులేని, గుర్తులేని> current_liga_సమితి_bounds (gsubgpos_లేఖాచిత్రం_context_t& c,
                                                         గుర్తులేని liga_సమితి_సూచీ,
                                                         స్థిరరాశి hb_serialize_context_t::object_t& liga_సమితి) స్థిరరాశి
  {
    // Finds the actual liga indices present in the liga set currently. Takes
    // into account those that have been removed by processing.
    గుర్తులేని min_సూచీ = HB_లేఖాచిత్రం_INVALID;
    గుర్తులేని max_సూచీ = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక& l : liga_సమితి.real_links) {
      ఒకవేళ (l.position < 2) కొనసాగు;

      గుర్తులేని liga_సూచీ = (l.position - 2) / 2;
      min_సూచీ = hb_min(min_సూచీ, liga_సూచీ);
      max_సూచీ = hb_max(max_సూచీ, liga_సూచీ);
    }
    తిరిగి hb_pair(min_సూచీ, max_సూచీ + 1);
  }

  శూన్య compact_liga_సమితి (gsubgpos_లేఖాచిత్రం_context_t& c, Ligature_సమితి* పట్టిక, లేఖాచిత్రం_t::శీర్షం_t* v) స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక& obj = v->obj ();
    ఒకవేళ (పట్టిక->ligature.len <= obj.real_links.నిడివి) తిరిగి;

    // compact the remaining linked liga offsets into a continous array and shrink the node as needed.
    గుర్తులేని గమ్యం_remove = పట్టిక->ligature.len - obj.real_links.నిడివి;
    గుర్తులేని new_position = SmallTypes::కొలత;
    v->క్రమీకరించు_real_links (); // for this to work we need to process links in order of position.
    కొరకు (స్వయంచాలక& l : v->real_links_writer ())
    {
      l.position = new_position;
      new_position += SmallTypes::కొలత;
    }

    పట్టిక->ligature.len = obj.real_links.నిడివి;
    v->shrink_బఫరు (గమ్యం_remove * SmallTypes::కొలత);
  }

  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> clone_range (gsubgpos_లేఖాచిత్రం_context_t& c,
                                        గుర్తులేని this_సూచీ,
                                        hb_సదిశ_t<గుర్తులేని> liga_counts,
                                        గుర్తులేని ప్రారంభం, గుర్తులేని చివర) స్థిరరాశి
  {
    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
               "  Cloning LigatureSubstFormat1 (%u) range [%u, %u).", this_సూచీ, ప్రారంభం, చివర);

    // Create an oversized new liga subst, we'll adjust the size down later. We don't know
    // the final size until we process it but we also need it to exist while we're processing
    // so that nodes can be moved to it as needed.
    గుర్తులేని ప్రధానసంఖ్య_కొలత = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_కొలత
                          + ligature_సమితి.get_కొలత() - ligature_సమితి.len.get_కొలత();

    TRY_కేటాయించు (గుర్తులేని liga_subst_ప్రధానసంఖ్య_id, c.create_కణుపు (ప్రధానసంఖ్య_కొలత));

    LigatureSubstFormat1* liga_subst_ప్రధానసంఖ్య = (LigatureSubstFormat1*) c.లేఖాచిత్రం.object (liga_subst_ప్రధానసంఖ్య_id).తల_2;
    liga_subst_ప్రధానసంఖ్య->format = ఇది->format;
    liga_subst_ప్రధానసంఖ్య->ligature_సమితి.len = ఇది->ligature_సమితి.len;

    // Create a place holder coverage prime id since we need to add virtual links to it while
    // generating liga and liga sets. Afterwards it will be updated to have the correct coverage.
    TRY_కేటాయించు (గుర్తులేని coverage_id, c.లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (this_సూచీ, &coverage));
    TRY_కేటాయించు (గుర్తులేని coverage_ప్రధానసంఖ్య_id, c.లేఖాచిత్రం.duplicate(coverage_id));
    స్వయంచాలక& coverage_ప్రధానసంఖ్య_శీర్షం = c.లేఖాచిత్రం.vertices_[coverage_ప్రధానసంఖ్య_id];

    TRY(c.లేఖాచిత్రం.vertices_[liga_subst_ప్రధానసంఖ్య_id].కూడు_real_link (SmallTypes::కొలత, coverage_ప్రధానసంఖ్య_id, 2));
    TRY(coverage_ప్రధానసంఖ్య_శీర్షం.కూడు_మాతృ (liga_subst_ప్రధానసంఖ్య_id, అబద్ధం));

    // Locate all liga sets with ligas between start and end.
    // Clone or move them as needed.
    గుర్తులేని లెక్క = 0;
    గుర్తులేని liga_సమితి_లెక్క = 0;
    గుర్తులేని liga_సమితి_ప్రారంభం = HB_లేఖాచిత్రం_INVALID;
    గుర్తులేని liga_సమితి_చివర = 0; // inclusive
    కొరకు (గుర్తులేని i = 0; i < liga_counts.నిడివి; i++)
    {
      గుర్తులేని అంకె_ligas = liga_counts[i];

      గుర్తులేని current_ప్రారంభం = లెక్క;
      గుర్తులేని current_చివర = లెక్క + అంకె_ligas;

      ఒకవేళ (current_ప్రారంభం >= చివర || ప్రారంభం >= current_చివర) {
        // No intersection, so just skip
        లెక్క += అంకె_ligas;
        కొనసాగు;
      }

      TRY_కేటాయించు (స్వయంచాలక liga_సమితి, c.లేఖాచిత్రం.as_mutable_పట్టిక<Ligature_సమితి>(this_సూచీ, &ligature_సమితి[i]));

      గుర్తులేని liga_సమితి_సూచీ = liga_సమితి.సూచీ_2;
      // Bounds may need to be adjusted if some ligas have been previously removed.
      hb_pair_t<గుర్తులేని, గుర్తులేని> liga_bounds = current_liga_సమితి_bounds(c, liga_సమితి_సూచీ, liga_సమితి.శీర్షం->obj ());
      current_ప్రారంభం = hb_max(లెక్క + liga_bounds.మొదటి, current_ప్రారంభం);
      current_చివర = hb_min(లెక్క + liga_bounds.second, current_చివర);

      గుర్తులేని liga_సమితి_ప్రధానసంఖ్య_id;
      ఒకవేళ (current_ప్రారంభం >= ప్రారంభం && current_చివర <= చివర) {
        // This liga set is fully contined within [start, end)
        // We can move the entire ligaset to the new liga subset object.
        liga_సమితి_చివర = i;
        ఒకవేళ (i < liga_సమితి_ప్రారంభం) liga_సమితి_ప్రారంభం = i;
        TRY_కేటాయించు (liga_సమితి_ప్రధానసంఖ్య_id, c.లేఖాచిత్రం.move_బిడ్డ<> (this_సూచీ,
                              &ligature_సమితి[i],
                              liga_subst_ప్రధానసంఖ్య_id,
                              &liga_subst_ప్రధానసంఖ్య->ligature_సమితి[liga_సమితి_లెక్క++]));
        compact_liga_సమితి (c, liga_సమితి.పట్టిక, liga_సమితి.శీర్షం);
      }
      లేకపోతే
      {
        // This liga set partially overlaps [start, end). We'll need to create
        // a new liga set sub table and move the intersecting ligas to it.
        గుర్తులేని ప్రారంభం_సూచీ = hb_max(ప్రారంభం, current_ప్రారంభం) - లెక్క;
        గుర్తులేని చివర_సూచీ = hb_min(చివర, current_చివర) - లెక్క;
        గుర్తులేని liga_లెక్క = చివర_సూచీ - ప్రారంభం_సూచీ;
        TRY_కేటాయించు (స్వయంచాలక ఫలితం, new_liga_సమితి(c, liga_లెక్క));
        liga_సమితి_ప్రధానసంఖ్య_id = ఫలితం.మొదటి;

        TRY (c.లేఖాచిత్రం.move_children<OT::Offset16>(
          liga_సమితి_సూచీ,
          2 + ప్రారంభం_సూచీ * 2,
          2 + చివర_సూచీ * 2,
          liga_సమితి_ప్రధానసంఖ్య_id,
          2));

        liga_సమితి_చివర = i;
        ఒకవేళ (i < liga_సమితి_ప్రారంభం) liga_సమితి_ప్రారంభం = i;
        TRY (c.లేఖాచిత్రం.కూడు_link(&liga_subst_ప్రధానసంఖ్య->ligature_సమితి[liga_సమితి_లెక్క++], liga_subst_ప్రధానసంఖ్య_id, liga_సమితి_ప్రధానసంఖ్య_id));
      }

      // The new liga and all children set needs to have a virtual link to the new coverage table:
      స్థిరరాశి స్వయంచాలక& liga_సమితి_ప్రధానసంఖ్య = c.లేఖాచిత్రం.vertices_[liga_సమితి_ప్రధానసంఖ్య_id].obj ();
      clear_virtual_links(c, liga_సమితి_ప్రధానసంఖ్య_id);
      TRY(కూడు_virtual_link(c, liga_సమితి_ప్రధానసంఖ్య_id, coverage_ప్రధానసంఖ్య_id));
      కొరకు (స్థిరరాశి స్వయంచాలక& l : liga_సమితి_ప్రధానసంఖ్య.real_links) {
        clear_virtual_links(c, l.objidx);
        TRY(కూడు_virtual_link(c, l.objidx, coverage_ప్రధానసంఖ్య_id));
      }

      లెక్క += అంకె_ligas;
    }

    c.లేఖాచిత్రం.vertices_[liga_subst_ప్రధానసంఖ్య_id].shrink_బఫరు ((liga_subst_ప్రధానసంఖ్య->ligature_సమితి.len - liga_సమితి_లెక్క) * SmallTypes::కొలత);
    liga_subst_ప్రధానసంఖ్య->ligature_సమితి.len = liga_సమితి_లెక్క;

    TRY (Coverage::filter_coverage (c,
                                    coverage_ప్రధానసంఖ్య_id,
                                    liga_సమితి_ప్రారంభం, liga_సమితి_చివర + 1));

    తిరిగి Ok(liga_subst_ప్రధానసంఖ్య_id);
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> shrink (gsubgpos_లేఖాచిత్రం_context_t& c,
                               గుర్తులేని this_సూచీ,
                               గుర్తులేని old_లెక్క,
                               hb_సదిశ_t<గుర్తులేని> liga_counts,
                               గుర్తులేని లెక్క)
  {
    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
               "  Shrinking LigatureSubstFormat1 (%u) to [0, %u).",
               this_సూచీ,
               లెక్క);
    ఒకవేళ (లెక్క >= old_లెక్క)
      తిరిగి Ok();

    hb_సమితి_t retained_indices;
    గుర్తులేని new_liga_సమితి_లెక్క = 0;
    కొరకు (గుర్తులేని i = 0; i < liga_counts.నిడివి; i++)
    {
      TRY_కేటాయించు (స్వయంచాలక liga_సమితి, c.లేఖాచిత్రం.as_mutable_పట్టిక<Ligature_సమితి>(this_సూచీ, &ligature_సమితి[i]));

      // We need the virtual links to coverage removed from all descendants on this liga subst.
      // If any are left when we try to mutate the coverage table later it will be unnessecarily
      // duplicated. Code later on will re-add the virtual links as needed (via retained_indices).
      clear_virtual_links(c, liga_సమితి.సూచీ_2);
      retained_indices.కూడు(liga_సమితి.సూచీ_2);

      TRY_కేటాయించు (స్వయంచాలక సూచీ_గమ్యం_id, ligature_సూచీ_గమ్యం_object_id (liga_సమితి));

      కొరకు (గుర్తులేని i = 0; i < liga_సమితి.పట్టిక->ligature.len; i++) {
        గుర్తులేని liga_సూచీ = సూచీ_గమ్యం_id[i];
        ఒకవేళ (liga_సూచీ != HB_లేఖాచిత్రం_INVALID) {
          clear_virtual_links(c, liga_సూచీ);
          retained_indices.కూడు(liga_సూచీ);
        }
      }

      గుర్తులేని అంకె_ligas = liga_counts[i];
      ఒకవేళ (అంకె_ligas >= లెక్క) {
        // drop the trailing liga's from this set and all subsequent liga sets
        గుర్తులేని అంకె_ligas_గమ్యం_remove = అంకె_ligas - లెక్క;
        new_liga_సమితి_లెక్క = i + 1;
        c.లేఖాచిత్రం.vertices_[liga_సమితి.సూచీ_2].shrink_బఫరు (అంకె_ligas_గమ్యం_remove * SmallTypes::కొలత);
        liga_సమితి.పట్టిక->ligature.len = లెక్క;
        ఆపు;
      } లేకపోతే {
        లెక్క -= అంకె_ligas;
      }
    }

    // Adjust liga set array
    స్వయంచాలక& this_శీర్షం = c.లేఖాచిత్రం.vertices_[this_సూచీ];
    this_శీర్షం.shrink_బఫరు ((ligature_సమితి.len - new_liga_సమితి_లెక్క) * SmallTypes::కొలత);
    ligature_సమితి.len = new_liga_సమితి_లెక్క;

    // Coverage matches the number of liga sets so rebuild as needed
    TRY_కేటాయించు (గుర్తులేని coverage_idx, c.లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (this_సూచీ, &ఇది->coverage));

    స్వయంచాలక& coverage_v = c.లేఖాచిత్రం.vertices_[coverage_idx];
    గుర్తులేని coverage_కొలత = coverage_v.పట్టిక_కొలత ();
    స్థిరరాశి Coverage* coverage_పట్టిక = (స్థిరరాశి Coverage*) coverage_v.obj ().తల_2;

    ఒకవేళ (coverage_v.is_shared ())
    {
      TRY_కేటాయించు (coverage_idx, c.లేఖాచిత్రం.remap_బిడ్డ (this_సూచీ, coverage_idx));
    }

    కొరకు (గుర్తులేని i : retained_indices.iter())
      TRY(కూడు_virtual_link(c, i, coverage_idx));

    స్వయంచాలక new_coverage =
        + hb_zip (coverage_పట్టిక->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<గుర్తులేని, గుర్తులేని> p) {
          తిరిగి p.second < new_liga_సమితి_లెక్క;
        })
        | hb_map_retains_క్రమీకరణ (hb_మొదటి)
        ;

    తిరిగి Coverage::make_coverage (c, new_coverage, coverage_idx, coverage_కొలత);
  }
};

నిర్మాణం LigatureSubst : బహిరంగ OT::Layout::GSUB_impl::LigatureSubst
{

  లేఖాచిత్రం_ఫలితం_t<hb_సదిశ_t<గుర్తులేని>> విభజించు_subtables (gsubgpos_లేఖాచిత్రం_context_t& c,
                                                         గుర్తులేని this_సూచీ)
  {
    ఎంపిక (u.format.v) {
    స్థితి 1:
      hb_barrier ();
      తిరిగి ((LigatureSubstFormat1*)(&u.format1))->విభజించు_subtables (c, this_సూచీ);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 2: HB_FALLTHROUGH;
      // 24-bit LigatureSubst already has widened offsets; leave it unsplit.
#అయితే_ముగింపు
    అప్రమేయ:
      తిరిగి Ok(hb_సదిశ_t<గుర్తులేని> ());
    }
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం) స్థిరరాశి
  {
    size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత ();
    ఒకవేళ (unlikely (శీర్షం_len < u.format.v.get_కొలత ())) తిరిగి Err(SANITIZE_FAILURE);
    hb_barrier ();

    ఎంపిక (u.format.v) {
    స్థితి 1:
      hb_barrier ();
      తిరిగి ((LigatureSubstFormat1*)(&u.format1))->sanitize (శీర్షం);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 2:  HB_FALLTHROUGH;
#అయితే_ముగింపు
    అప్రమేయ:
      // We don't handle format 2 here.
      తిరిగి Err(SANITIZE_FAILURE);
    }
  }
};

}

#అయితే_ముగింపు  // GRAPH_LIGATURE_GRAPH_HH
