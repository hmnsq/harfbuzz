
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

#که_نه_تعریف_شوی ګراف_LIGATURE_ګراف_HH
#تعریف_کړه ګراف_LIGATURE_ګراف_HH

#ضمن "graph.hh"
#ضمن "../OT/Layout/GSUB/LigatureSubst.hh"
#ضمن "../OT/Layout/GSUB/LigatureSubstFormat1.hh"
#ضمن "../OT/Layout/GSUB/LigatureSet.hh"
#ضمن "../hb-ot-dual.hh"
#ضمن <algorithm>
#ضمن <utility>

نطاق ګراف {

بنية Ligature_سېټ : عام OT::Layout::GSUB_impl::Ligature_سېټ<SmallTypes>
{
  ګراف_پایله_t<فراغ> sanitize (ثابت ګراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_اندازه ();
    إذا (unlikely (راس_len < OT::Layout::GSUB_impl::Ligature_سېټ<SmallTypes>::min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    size_t ټول_len = ligature.get_اندازه() + OT::Layout::GSUB_impl::Ligature_سېټ<SmallTypes>::min_اندازه - ligature.len.get_اندازه();
    إذا (unlikely (راس_len < ټول_len)) {
      أرجع Err(SANITIZE_FAILURE);
    }
    أرجع Ok();
  }
};

بنية LigatureSubstFormat1 : عام OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>
{
  ګراف_پایله_t<فراغ> sanitize (ثابت ګراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_اندازه ();
    بینښه min_اندازه = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_اندازه;
    إذا (unlikely (راس_len < min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (راس_len < min_اندازه + ligature_سېټ.get_اندازه() - ligature_سېټ.len.get_اندازه()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  ګراف_پایله_t<hb_ویکټور_t<بینښه>> وېشل_subtables (gsubgpos_ګراف_context_t& c,
                                                         بینښه this_شاخص)
  {
    TRY_ټاکل (خپلکاری وېشل_points, compute_وېشل_points(c, this_شاخص));
    إذا (!وېشل_points)
      أرجع Ok(hb_ویکټور_t<بینښه> ());

    TRY_ټاکل (بینښه ټول_ligas, ټول_شمېره_ligas(c, this_شاخص));
    TRY_ټاکل (hb_ویکټور_t<بینښه> counts, liga_counts(c, this_شاخص));
    وېشل_context_t وېشل_context {
      c,
      هذا,
      this_شاخص,
      ټول_ligas,
      std::move (counts),
    };
    أرجع actuate_subtable_وېشل<وېشل_context_t> (وېشل_context, وېشل_points);
  }

 خاص:
  ګراف_پایله_t<بینښه> ټول_شمېره_ligas(gsubgpos_ګراف_context_t& c, بینښه this_شاخص) ثابت {
    بینښه ټول = 0;
    لكل (بینښه i = 0; i < ligature_سېټ.len; i++)
    {
      TRY_ټاکل (خپلکاری liga_سېټ, c.ګراف.as_جدول<Ligature_سېټ>(this_شاخص, &ligature_سېټ[i]));
      ټول += liga_سېټ.جدول->ligature.len;
    }
    أرجع Ok(ټول);
  }

  ګراف_پایله_t<hb_ویکټور_t<بینښه>> liga_counts(gsubgpos_ګراف_context_t& c, بینښه this_شاخص) ثابت {
    hb_ویکټور_t<بینښه> پایله;
    لكل (بینښه i = 0; i < ligature_سېټ.len; i++)
    {
      TRY_ټاکل (خپلکاری liga_سېټ, c.ګراف.as_جدول<Ligature_سېټ>(this_شاخص, &ligature_سېټ[i]));
      پایله.push(!liga_سېټ.جدول ? 0 : liga_سېټ.جدول->ligature.len);
    }
    أرجع ګراف_پایله_t<hb_ویکټور_t<بینښه>>::from(std::move(پایله), ویش_FAILURE);
  }

  کينډۍ <ګراف_t::راس_mutability_t mutability>
  ګراف_پایله_t<hb_ویکټور_t<بینښه>> ligature_شاخص_منزل_object_id(ثابت ګراف_t::راس_and_جدول_t<Ligature_سېټ, mutability>& liga_سېټ) ثابت {
    hb_ویکټور_t<بینښه> map;
    إذا (!map.resize_exact(liga_سېټ.جدول->ligature.len)) أرجع Err(ویش_FAILURE);

    لكل (بینښه i = 0; i < map.اوږدتیا; i++) {
      map[i] = HB_ګراف_INVALID;
    }

    لكل (ثابت خپلکاری& l : liga_سېټ.راس->obj ().real_links) {
      إذا (l.position < 2) استمر;
      بینښه array_شاخص = (l.position - 2) / 2;
      map[array_شاخص] = l.objidx;
    }
    أرجع map;
  }

  ګراف_پایله_t<hb_ویکټور_t<بینښه>> compute_وېشل_points(gsubgpos_ګراف_context_t& c,
                                                             بینښه this_شاخص) ثابت
  {
    // For ligature subst coverage is always packed last, and as a result is where an overflow
    // will happen if there is one, so we can check the estimate length of the
    // LigatureSubstFormat1 -> Coverage offset length which is the sum of all data in the
    // retained sub graph except for the coverage table itself.
    ثابت بینښه بنسټ_اندازه = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_اندازه;
    بینښه accumulated = بنسټ_اندازه;

    بینښه ligature_شاخص = 0;
    hb_ویکټور_t<بینښه> وېشل_points;
    لكل (بینښه i = 0; i < ligature_سېټ.len; i++)
    {
      accumulated += OT::HBUINT16::static_اندازه; // for ligature set offset
      accumulated += OT::Layout::GSUB_impl::Ligature_سېټ<SmallTypes>::min_اندازه; // for ligature set table

      TRY_ټاکل (خپلکاری liga_سېټ, c.ګراف.as_جدول<Ligature_سېټ>(this_شاخص, &ligature_سېټ[i]));

      // Finding the object id associated with an array index is O(n)
      // so to avoid O(n^2), precompute the mapping by scanning through
      // all links
      TRY_ټاکل (خپلکاری شاخص_منزل_id, ligature_شاخص_منزل_object_id (liga_سېټ));

      لكل (بینښه j = 0; j < liga_سېټ.جدول->ligature.len; j++)
      {
        ثابت بینښه liga_id = شاخص_منزل_id[j];
        إذا (liga_id == HB_ګراف_INVALID) استمر; // no outgoing link, ignore
        ثابت بینښه liga_اندازه = c.ګراف.vertices_[liga_id].جدول_اندازه ();

        accumulated += OT::HBUINT16::static_اندازه; // for ligature offset
        accumulated += liga_اندازه; // for the ligature table

        إذا (accumulated >= (1 << 16))
        {
          وېشل_points.push(ligature_شاخص);
          // We're going to split such that the current ligature will be in the new sub table.
          // That means we'll have one ligature subst (base_base), one ligature set, and one liga table
          accumulated = بنسټ_اندازه + // for liga subst subtable
            (OT::HBUINT16::static_اندازه * 2) + // for liga set and liga offset
            OT::Layout::GSUB_impl::Ligature_سېټ<SmallTypes>::min_اندازه + // for liga set subtable
            liga_اندازه; // for liga sub table
        }

        ligature_شاخص++;
      }
    }

    أرجع وېشل_points;
  }

  بنية وېشل_context_t
  {
    gsubgpos_ګراف_context_t& c;
    LigatureSubstFormat1* thiz;
    بینښه this_شاخص;
    بینښه original_شمېر;
    hb_ویکټور_t<بینښه> liga_counts;

    بینښه original_شمېر_2 ()
    {
      أرجع original_شمېر;
    }

    ګراف_پایله_t<بینښه> clone_range (بینښه پیل, بینښه پای_ټکی)
    {
      أرجع thiz->clone_range (c, this_شاخص, liga_counts, پیل, پای_ټکی);
    }

    ګراف_پایله_t<فراغ> shrink (بینښه شمېر)
    {
      أرجع thiz->shrink (c, this_شاخص, original_شمېر_2(), liga_counts, شمېر);
    }
  };

  ګراف_پایله_t<hb_pair_t<بینښه, Ligature_سېټ*>> new_liga_سېټ(gsubgpos_ګراف_context_t& c, بینښه شمېر) ثابت {
    بینښه اولي_اندازه = OT::Layout::GSUB_impl::Ligature_سېټ<SmallTypes>::min_اندازه
                          + شمېر * SmallTypes::اندازه;

    TRY_ټاکل (بینښه اولي_id, c.create_غوټه (اولي_اندازه));

    Ligature_سېټ* اولي = (Ligature_سېټ*) c.ګراف.object (اولي_id).سر_2;
    اولي->ligature.len = شمېر;
    أرجع Ok(hb_pair(اولي_id, اولي));
  }

  فراغ clear_virtual_links (gsubgpos_ګراف_context_t& c, بینښه غوټه_شاخص) ثابت
  {
    خپلکاری& v  = c.ګراف.vertices_[غوټه_شاخص];
    لكل (ثابت خپلکاری& l : v.obj ().virtual_links)
    {
      خپلکاری& ماشوم = c.ګراف.vertices_[l.objidx];
      ماشوم.remove_پلار(غوټه_شاخص);
    }
    v.clear_virtual_links ();
  }

  ګراف_پایله_t<فراغ> جمع_کړه_virtual_link(gsubgpos_ګراف_context_t& c, بینښه from, بینښه to) ثابت {
    إذا (unlikely (from >= c.ګراف.vertices_.اوږدتیا || to >= c.ګراف.vertices_.اوږدتیا)) أرجع Err(بهر_د_BOUNDS);
    TRY(c.ګراف.vertices_[to].جمع_کړه_پلار (from, ریښتیا));
    TRY(c.ګراف.vertices_[from].جمع_کړه_virtual_link (to));
    أرجع Ok();
  }

  hb_pair_t<بینښه, بینښه> current_liga_سېټ_bounds (gsubgpos_ګراف_context_t& c,
                                                         بینښه liga_سېټ_شاخص,
                                                         ثابت hb_serialize_context_t::object_t& liga_سېټ) ثابت
  {
    // Finds the actual liga indices present in the liga set currently. Takes
    // into account those that have been removed by processing.
    بینښه min_شاخص = HB_ګراف_INVALID;
    بینښه max_شاخص = 0;
    لكل (ثابت خپلکاری& l : liga_سېټ.real_links) {
      إذا (l.position < 2) استمر;

      بینښه liga_شاخص = (l.position - 2) / 2;
      min_شاخص = hb_min(min_شاخص, liga_شاخص);
      max_شاخص = hb_max(max_شاخص, liga_شاخص);
    }
    أرجع hb_pair(min_شاخص, max_شاخص + 1);
  }

  فراغ compact_liga_سېټ (gsubgpos_ګراف_context_t& c, Ligature_سېټ* جدول, ګراف_t::راس_t* v) ثابت
  {
    ثابت خپلکاری& obj = v->obj ();
    إذا (جدول->ligature.len <= obj.real_links.اوږدتیا) أرجع;

    // compact the remaining linked liga offsets into a continous array and shrink the node as needed.
    بینښه منزل_remove = جدول->ligature.len - obj.real_links.اوږدتیا;
    بینښه new_position = SmallTypes::اندازه;
    v->ترتیبول_real_links (); // for this to work we need to process links in order of position.
    لكل (خپلکاری& l : v->real_links_writer ())
    {
      l.position = new_position;
      new_position += SmallTypes::اندازه;
    }

    جدول->ligature.len = obj.real_links.اوږدتیا;
    v->shrink_بفر (منزل_remove * SmallTypes::اندازه);
  }

  ګراف_پایله_t<بینښه> clone_range (gsubgpos_ګراف_context_t& c,
                                        بینښه this_شاخص,
                                        hb_ویکټور_t<بینښه> liga_counts,
                                        بینښه پیل, بینښه پای_ټکی) ثابت
  {
    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
               "  Cloning LigatureSubstFormat1 (%u) range [%u, %u).", this_شاخص, پیل, پای_ټکی);

    // Create an oversized new liga subst, we'll adjust the size down later. We don't know
    // the final size until we process it but we also need it to exist while we're processing
    // so that nodes can be moved to it as needed.
    بینښه اولي_اندازه = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_اندازه
                          + ligature_سېټ.get_اندازه() - ligature_سېټ.len.get_اندازه();

    TRY_ټاکل (بینښه liga_subst_اولي_id, c.create_غوټه (اولي_اندازه));

    LigatureSubstFormat1* liga_subst_اولي = (LigatureSubstFormat1*) c.ګراف.object (liga_subst_اولي_id).سر_2;
    liga_subst_اولي->format = هذا->format;
    liga_subst_اولي->ligature_سېټ.len = هذا->ligature_سېټ.len;

    // Create a place holder coverage prime id since we need to add virtual links to it while
    // generating liga and liga sets. Afterwards it will be updated to have the correct coverage.
    TRY_ټاکل (بینښه coverage_id, c.ګراف.شاخص_for_دننه_واټن (this_شاخص, &coverage));
    TRY_ټاکل (بینښه coverage_اولي_id, c.ګراف.duplicate(coverage_id));
    خپلکاری& coverage_اولي_راس = c.ګراف.vertices_[coverage_اولي_id];

    TRY(c.ګراف.vertices_[liga_subst_اولي_id].جمع_کړه_real_link (SmallTypes::اندازه, coverage_اولي_id, 2));
    TRY(coverage_اولي_راس.جمع_کړه_پلار (liga_subst_اولي_id, خطأ));

    // Locate all liga sets with ligas between start and end.
    // Clone or move them as needed.
    بینښه شمېر = 0;
    بینښه liga_سېټ_شمېر = 0;
    بینښه liga_سېټ_پیل = HB_ګراف_INVALID;
    بینښه liga_سېټ_پای_ټکی = 0; // inclusive
    لكل (بینښه i = 0; i < liga_counts.اوږدتیا; i++)
    {
      بینښه عدد_ligas = liga_counts[i];

      بینښه current_پیل = شمېر;
      بینښه current_پای_ټکی = شمېر + عدد_ligas;

      إذا (current_پیل >= پای_ټکی || پیل >= current_پای_ټکی) {
        // No intersection, so just skip
        شمېر += عدد_ligas;
        استمر;
      }

      TRY_ټاکل (خپلکاری liga_سېټ, c.ګراف.as_mutable_جدول<Ligature_سېټ>(this_شاخص, &ligature_سېټ[i]));

      بینښه liga_سېټ_شاخص = liga_سېټ.شاخص_2;
      // Bounds may need to be adjusted if some ligas have been previously removed.
      hb_pair_t<بینښه, بینښه> liga_bounds = current_liga_سېټ_bounds(c, liga_سېټ_شاخص, liga_سېټ.راس->obj ());
      current_پیل = hb_max(شمېر + liga_bounds.لومړی, current_پیل);
      current_پای_ټکی = hb_min(شمېر + liga_bounds.second, current_پای_ټکی);

      بینښه liga_سېټ_اولي_id;
      إذا (current_پیل >= پیل && current_پای_ټکی <= پای_ټکی) {
        // This liga set is fully contined within [start, end)
        // We can move the entire ligaset to the new liga subset object.
        liga_سېټ_پای_ټکی = i;
        إذا (i < liga_سېټ_پیل) liga_سېټ_پیل = i;
        TRY_ټاکل (liga_سېټ_اولي_id, c.ګراف.move_ماشوم<> (this_شاخص,
                              &ligature_سېټ[i],
                              liga_subst_اولي_id,
                              &liga_subst_اولي->ligature_سېټ[liga_سېټ_شمېر++]));
        compact_liga_سېټ (c, liga_سېټ.جدول, liga_سېټ.راس);
      }
      وإلا
      {
        // This liga set partially overlaps [start, end). We'll need to create
        // a new liga set sub table and move the intersecting ligas to it.
        بینښه پیل_شاخص = hb_max(پیل, current_پیل) - شمېر;
        بینښه پای_ټکی_شاخص = hb_min(پای_ټکی, current_پای_ټکی) - شمېر;
        بینښه liga_شمېر = پای_ټکی_شاخص - پیل_شاخص;
        TRY_ټاکل (خپلکاری پایله, new_liga_سېټ(c, liga_شمېر));
        liga_سېټ_اولي_id = پایله.لومړی;

        TRY (c.ګراف.move_children<OT::Offset16>(
          liga_سېټ_شاخص,
          2 + پیل_شاخص * 2,
          2 + پای_ټکی_شاخص * 2,
          liga_سېټ_اولي_id,
          2));

        liga_سېټ_پای_ټکی = i;
        إذا (i < liga_سېټ_پیل) liga_سېټ_پیل = i;
        TRY (c.ګراف.جمع_کړه_link(&liga_subst_اولي->ligature_سېټ[liga_سېټ_شمېر++], liga_subst_اولي_id, liga_سېټ_اولي_id));
      }

      // The new liga and all children set needs to have a virtual link to the new coverage table:
      ثابت خپلکاری& liga_سېټ_اولي = c.ګراف.vertices_[liga_سېټ_اولي_id].obj ();
      clear_virtual_links(c, liga_سېټ_اولي_id);
      TRY(جمع_کړه_virtual_link(c, liga_سېټ_اولي_id, coverage_اولي_id));
      لكل (ثابت خپلکاری& l : liga_سېټ_اولي.real_links) {
        clear_virtual_links(c, l.objidx);
        TRY(جمع_کړه_virtual_link(c, l.objidx, coverage_اولي_id));
      }

      شمېر += عدد_ligas;
    }

    c.ګراف.vertices_[liga_subst_اولي_id].shrink_بفر ((liga_subst_اولي->ligature_سېټ.len - liga_سېټ_شمېر) * SmallTypes::اندازه);
    liga_subst_اولي->ligature_سېټ.len = liga_سېټ_شمېر;

    TRY (Coverage::filter_coverage (c,
                                    coverage_اولي_id,
                                    liga_سېټ_پیل, liga_سېټ_پای_ټکی + 1));

    أرجع Ok(liga_subst_اولي_id);
  }

  ګراف_پایله_t<فراغ> shrink (gsubgpos_ګراف_context_t& c,
                               بینښه this_شاخص,
                               بینښه old_شمېر,
                               hb_ویکټور_t<بینښه> liga_counts,
                               بینښه شمېر)
  {
    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
               "  Shrinking LigatureSubstFormat1 (%u) to [0, %u).",
               this_شاخص,
               شمېر);
    إذا (شمېر >= old_شمېر)
      أرجع Ok();

    hb_سېټ_t retained_indices;
    بینښه new_liga_سېټ_شمېر = 0;
    لكل (بینښه i = 0; i < liga_counts.اوږدتیا; i++)
    {
      TRY_ټاکل (خپلکاری liga_سېټ, c.ګراف.as_mutable_جدول<Ligature_سېټ>(this_شاخص, &ligature_سېټ[i]));

      // We need the virtual links to coverage removed from all descendants on this liga subst.
      // If any are left when we try to mutate the coverage table later it will be unnessecarily
      // duplicated. Code later on will re-add the virtual links as needed (via retained_indices).
      clear_virtual_links(c, liga_سېټ.شاخص_2);
      retained_indices.جمع_کړه(liga_سېټ.شاخص_2);

      TRY_ټاکل (خپلکاری شاخص_منزل_id, ligature_شاخص_منزل_object_id (liga_سېټ));

      لكل (بینښه i = 0; i < liga_سېټ.جدول->ligature.len; i++) {
        بینښه liga_شاخص = شاخص_منزل_id[i];
        إذا (liga_شاخص != HB_ګراف_INVALID) {
          clear_virtual_links(c, liga_شاخص);
          retained_indices.جمع_کړه(liga_شاخص);
        }
      }

      بینښه عدد_ligas = liga_counts[i];
      إذا (عدد_ligas >= شمېر) {
        // drop the trailing liga's from this set and all subsequent liga sets
        بینښه عدد_ligas_منزل_remove = عدد_ligas - شمېر;
        new_liga_سېټ_شمېر = i + 1;
        c.ګراف.vertices_[liga_سېټ.شاخص_2].shrink_بفر (عدد_ligas_منزل_remove * SmallTypes::اندازه);
        liga_سېټ.جدول->ligature.len = شمېر;
        اكسر;
      } وإلا {
        شمېر -= عدد_ligas;
      }
    }

    // Adjust liga set array
    خپلکاری& this_راس = c.ګراف.vertices_[this_شاخص];
    this_راس.shrink_بفر ((ligature_سېټ.len - new_liga_سېټ_شمېر) * SmallTypes::اندازه);
    ligature_سېټ.len = new_liga_سېټ_شمېر;

    // Coverage matches the number of liga sets so rebuild as needed
    TRY_ټاکل (بینښه coverage_idx, c.ګراف.شاخص_for_دننه_واټن (this_شاخص, &هذا->coverage));

    خپلکاری& coverage_v = c.ګراف.vertices_[coverage_idx];
    بینښه coverage_اندازه = coverage_v.جدول_اندازه ();
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj ().سر_2;

    إذا (coverage_v.is_shared ())
    {
      TRY_ټاکل (coverage_idx, c.ګراف.remap_ماشوم (this_شاخص, coverage_idx));
    }

    لكل (بینښه i : retained_indices.iter())
      TRY(جمع_کړه_virtual_link(c, i, coverage_idx));

    خپلکاری new_coverage =
        + hb_zip (coverage_جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<بینښه, بینښه> p) {
          أرجع p.second < new_liga_سېټ_شمېر;
        })
        | hb_map_retains_ترتیبونه (hb_لومړی)
        ;

    أرجع Coverage::make_coverage (c, new_coverage, coverage_idx, coverage_اندازه);
  }
};

بنية LigatureSubst : عام OT::Layout::GSUB_impl::LigatureSubst
{

  ګراف_پایله_t<hb_ویکټور_t<بینښه>> وېشل_subtables (gsubgpos_ګراف_context_t& c,
                                                         بینښه this_شاخص)
  {
    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((LigatureSubstFormat1*)(&u.format1))->وېشل_subtables (c, this_شاخص);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 2: HB_FALLTHROUGH;
      // 24-bit LigatureSubst already has widened offsets; leave it unsplit.
#د_که_پای
    افتراضي:
      أرجع Ok(hb_ویکټور_t<بینښه> ());
    }
  }

  ګراف_پایله_t<فراغ> sanitize (ثابت ګراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_اندازه ();
    إذا (unlikely (راس_len < u.format.v.get_اندازه ())) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((LigatureSubstFormat1*)(&u.format1))->sanitize (راس);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 2:  HB_FALLTHROUGH;
#د_که_پای
    افتراضي:
      // We don't handle format 2 here.
      أرجع Err(SANITIZE_FAILURE);
    }
  }
};

}

#د_که_پای  // GRAPH_LIGATURE_GRAPH_HH
