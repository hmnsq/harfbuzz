
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

#جيڪڏهن_وصف_نه_ٿيل گراف_LIGATURE_گراف_HH
#وصف_ڪريو گراف_LIGATURE_گراف_HH

#ضمن "graph.hh"
#ضمن "../OT/Layout/GSUB/LigatureSubst.hh"
#ضمن "../OT/Layout/GSUB/LigatureSubstFormat1.hh"
#ضمن "../OT/Layout/GSUB/LigatureSet.hh"
#ضمن "../hb-ot-dual.hh"
#ضمن <algorithm>
#ضمن <utility>

نطاق گراف {

بنية Ligature_سيٽ : عام OT::Layout::GSUB_impl::Ligature_سيٽ<SmallTypes>
{
  گراف_نتيجو_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائيز ();
    إذا (unlikely (راس_len < OT::Layout::GSUB_impl::Ligature_سيٽ<SmallTypes>::min_سائيز)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    size_t ڪل_len = ligature.get_سائيز() + OT::Layout::GSUB_impl::Ligature_سيٽ<SmallTypes>::min_سائيز - ligature.len.get_سائيز();
    إذا (unlikely (راس_len < ڪل_len)) {
      أرجع Err(SANITIZE_FAILURE);
    }
    أرجع Ok();
  }
};

بنية LigatureSubstFormat1 : عام OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>
{
  گراف_نتيجو_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائيز ();
    بنانشان min_سائيز = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_سائيز;
    إذا (unlikely (راس_len < min_سائيز)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (راس_len < min_سائيز + ligature_سيٽ.get_سائيز() - ligature_سيٽ.len.get_سائيز()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتيجو_t<hb_ويڪٽر_t<بنانشان>> ورهايو_subtables (gsubgpos_گراف_context_t& c,
                                                         بنانشان this_اشاريو)
  {
    TRY_سونپيو (خودڪار ورهايو_points, compute_ورهايو_points(c, this_اشاريو));
    إذا (!ورهايو_points)
      أرجع Ok(hb_ويڪٽر_t<بنانشان> ());

    TRY_سونپيو (بنانشان ڪل_ligas, ڪل_عدد_ligas(c, this_اشاريو));
    TRY_سونپيو (hb_ويڪٽر_t<بنانشان> counts, liga_counts(c, this_اشاريو));
    ورهايو_context_t ورهايو_context {
      c,
      هذا,
      this_اشاريو,
      ڪل_ligas,
      std::move (counts),
    };
    أرجع actuate_subtable_ورهايو<ورهايو_context_t> (ورهايو_context, ورهايو_points);
  }

 خاص:
  گراف_نتيجو_t<بنانشان> ڪل_عدد_ligas(gsubgpos_گراف_context_t& c, بنانشان this_اشاريو) ثابت {
    بنانشان ڪل = 0;
    لكل (بنانشان i = 0; i < ligature_سيٽ.len; i++)
    {
      TRY_سونپيو (خودڪار liga_سيٽ, c.گراف.as_جدول<Ligature_سيٽ>(this_اشاريو, &ligature_سيٽ[i]));
      ڪل += liga_سيٽ.جدول->ligature.len;
    }
    أرجع Ok(ڪل);
  }

  گراف_نتيجو_t<hb_ويڪٽر_t<بنانشان>> liga_counts(gsubgpos_گراف_context_t& c, بنانشان this_اشاريو) ثابت {
    hb_ويڪٽر_t<بنانشان> نتيجو;
    لكل (بنانشان i = 0; i < ligature_سيٽ.len; i++)
    {
      TRY_سونپيو (خودڪار liga_سيٽ, c.گراف.as_جدول<Ligature_سيٽ>(this_اشاريو, &ligature_سيٽ[i]));
      نتيجو.push(!liga_سيٽ.جدول ? 0 : liga_سيٽ.جدول->ligature.len);
    }
    أرجع گراف_نتيجو_t<hb_ويڪٽر_t<بنانشان>>::from(std::move(نتيجو), ورهاست_FAILURE);
  }

  سانچو <گراف_t::راس_mutability_t mutability>
  گراف_نتيجو_t<hb_ويڪٽر_t<بنانشان>> ligature_اشاريو_منزل_object_id(ثابت گراف_t::راس_and_جدول_t<Ligature_سيٽ, mutability>& liga_سيٽ) ثابت {
    hb_ويڪٽر_t<بنانشان> map;
    إذا (!map.resize_exact(liga_سيٽ.جدول->ligature.len)) أرجع Err(ورهاست_FAILURE);

    لكل (بنانشان i = 0; i < map.ڊيگهائي; i++) {
      map[i] = HB_گراف_INVALID;
    }

    لكل (ثابت خودڪار& l : liga_سيٽ.راس->obj ().real_links) {
      إذا (l.position < 2) استمر;
      بنانشان array_اشاريو = (l.position - 2) / 2;
      map[array_اشاريو] = l.objidx;
    }
    أرجع map;
  }

  گراف_نتيجو_t<hb_ويڪٽر_t<بنانشان>> compute_ورهايو_points(gsubgpos_گراف_context_t& c,
                                                             بنانشان this_اشاريو) ثابت
  {
    // For ligature subst coverage is always packed last, and as a result is where an overflow
    // will happen if there is one, so we can check the estimate length of the
    // LigatureSubstFormat1 -> Coverage offset length which is the sum of all data in the
    // retained sub graph except for the coverage table itself.
    ثابت بنانشان بنياد_سائيز = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_سائيز;
    بنانشان accumulated = بنياد_سائيز;

    بنانشان ligature_اشاريو = 0;
    hb_ويڪٽر_t<بنانشان> ورهايو_points;
    لكل (بنانشان i = 0; i < ligature_سيٽ.len; i++)
    {
      accumulated += OT::HBUINT16::static_سائيز; // for ligature set offset
      accumulated += OT::Layout::GSUB_impl::Ligature_سيٽ<SmallTypes>::min_سائيز; // for ligature set table

      TRY_سونپيو (خودڪار liga_سيٽ, c.گراف.as_جدول<Ligature_سيٽ>(this_اشاريو, &ligature_سيٽ[i]));

      // Finding the object id associated with an array index is O(n)
      // so to avoid O(n^2), precompute the mapping by scanning through
      // all links
      TRY_سونپيو (خودڪار اشاريو_منزل_id, ligature_اشاريو_منزل_object_id (liga_سيٽ));

      لكل (بنانشان j = 0; j < liga_سيٽ.جدول->ligature.len; j++)
      {
        ثابت بنانشان liga_id = اشاريو_منزل_id[j];
        إذا (liga_id == HB_گراف_INVALID) استمر; // no outgoing link, ignore
        ثابت بنانشان liga_سائيز = c.گراف.vertices_[liga_id].جدول_سائيز ();

        accumulated += OT::HBUINT16::static_سائيز; // for ligature offset
        accumulated += liga_سائيز; // for the ligature table

        إذا (accumulated >= (1 << 16))
        {
          ورهايو_points.push(ligature_اشاريو);
          // We're going to split such that the current ligature will be in the new sub table.
          // That means we'll have one ligature subst (base_base), one ligature set, and one liga table
          accumulated = بنياد_سائيز + // for liga subst subtable
            (OT::HBUINT16::static_سائيز * 2) + // for liga set and liga offset
            OT::Layout::GSUB_impl::Ligature_سيٽ<SmallTypes>::min_سائيز + // for liga set subtable
            liga_سائيز; // for liga sub table
        }

        ligature_اشاريو++;
      }
    }

    أرجع ورهايو_points;
  }

  بنية ورهايو_context_t
  {
    gsubgpos_گراف_context_t& c;
    LigatureSubstFormat1* thiz;
    بنانشان this_اشاريو;
    بنانشان original_ڳڻپ;
    hb_ويڪٽر_t<بنانشان> liga_counts;

    بنانشان original_ڳڻپ_2 ()
    {
      أرجع original_ڳڻپ;
    }

    گراف_نتيجو_t<بنانشان> clone_range (بنانشان شروعات, بنانشان پڇاڙي)
    {
      أرجع thiz->clone_range (c, this_اشاريو, liga_counts, شروعات, پڇاڙي);
    }

    گراف_نتيجو_t<فراغ> shrink (بنانشان ڳڻپ)
    {
      أرجع thiz->shrink (c, this_اشاريو, original_ڳڻپ_2(), liga_counts, ڳڻپ);
    }
  };

  گراف_نتيجو_t<hb_pair_t<بنانشان, Ligature_سيٽ*>> new_liga_سيٽ(gsubgpos_گراف_context_t& c, بنانشان ڳڻپ) ثابت {
    بنانشان مفرد_سائيز = OT::Layout::GSUB_impl::Ligature_سيٽ<SmallTypes>::min_سائيز
                          + ڳڻپ * SmallTypes::سائيز;

    TRY_سونپيو (بنانشان مفرد_id, c.create_ڳنڍ (مفرد_سائيز));

    Ligature_سيٽ* مفرد = (Ligature_سيٽ*) c.گراف.object (مفرد_id).سر_2;
    مفرد->ligature.len = ڳڻپ;
    أرجع Ok(hb_pair(مفرد_id, مفرد));
  }

  فراغ clear_virtual_links (gsubgpos_گراف_context_t& c, بنانشان ڳنڍ_اشاريو) ثابت
  {
    خودڪار& v  = c.گراف.vertices_[ڳنڍ_اشاريو];
    لكل (ثابت خودڪار& l : v.obj ().virtual_links)
    {
      خودڪار& ٻار = c.گراف.vertices_[l.objidx];
      ٻار.remove_والد(ڳنڍ_اشاريو);
    }
    v.clear_virtual_links ();
  }

  گراف_نتيجو_t<فراغ> جوڙيو_virtual_link(gsubgpos_گراف_context_t& c, بنانشان from, بنانشان to) ثابت {
    إذا (unlikely (from >= c.گراف.vertices_.ڊيگهائي || to >= c.گراف.vertices_.ڊيگهائي)) أرجع Err(ٻاهريون_جو_BOUNDS);
    TRY(c.گراف.vertices_[to].جوڙيو_والد (from, سچ));
    TRY(c.گراف.vertices_[from].جوڙيو_virtual_link (to));
    أرجع Ok();
  }

  hb_pair_t<بنانشان, بنانشان> current_liga_سيٽ_bounds (gsubgpos_گراف_context_t& c,
                                                         بنانشان liga_سيٽ_اشاريو,
                                                         ثابت hb_serialize_context_t::object_t& liga_سيٽ) ثابت
  {
    // Finds the actual liga indices present in the liga set currently. Takes
    // into account those that have been removed by processing.
    بنانشان min_اشاريو = HB_گراف_INVALID;
    بنانشان max_اشاريو = 0;
    لكل (ثابت خودڪار& l : liga_سيٽ.real_links) {
      إذا (l.position < 2) استمر;

      بنانشان liga_اشاريو = (l.position - 2) / 2;
      min_اشاريو = hb_min(min_اشاريو, liga_اشاريو);
      max_اشاريو = hb_max(max_اشاريو, liga_اشاريو);
    }
    أرجع hb_pair(min_اشاريو, max_اشاريو + 1);
  }

  فراغ compact_liga_سيٽ (gsubgpos_گراف_context_t& c, Ligature_سيٽ* جدول, گراف_t::راس_t* v) ثابت
  {
    ثابت خودڪار& obj = v->obj ();
    إذا (جدول->ligature.len <= obj.real_links.ڊيگهائي) أرجع;

    // compact the remaining linked liga offsets into a continous array and shrink the node as needed.
    بنانشان منزل_remove = جدول->ligature.len - obj.real_links.ڊيگهائي;
    بنانشان new_position = SmallTypes::سائيز;
    v->ترتيب_ڏيو_real_links (); // for this to work we need to process links in order of position.
    لكل (خودڪار& l : v->real_links_writer ())
    {
      l.position = new_position;
      new_position += SmallTypes::سائيز;
    }

    جدول->ligature.len = obj.real_links.ڊيگهائي;
    v->shrink_بفر (منزل_remove * SmallTypes::سائيز);
  }

  گراف_نتيجو_t<بنانشان> clone_range (gsubgpos_گراف_context_t& c,
                                        بنانشان this_اشاريو,
                                        hb_ويڪٽر_t<بنانشان> liga_counts,
                                        بنانشان شروعات, بنانشان پڇاڙي) ثابت
  {
    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
               "  Cloning LigatureSubstFormat1 (%u) range [%u, %u).", this_اشاريو, شروعات, پڇاڙي);

    // Create an oversized new liga subst, we'll adjust the size down later. We don't know
    // the final size until we process it but we also need it to exist while we're processing
    // so that nodes can be moved to it as needed.
    بنانشان مفرد_سائيز = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_سائيز
                          + ligature_سيٽ.get_سائيز() - ligature_سيٽ.len.get_سائيز();

    TRY_سونپيو (بنانشان liga_subst_مفرد_id, c.create_ڳنڍ (مفرد_سائيز));

    LigatureSubstFormat1* liga_subst_مفرد = (LigatureSubstFormat1*) c.گراف.object (liga_subst_مفرد_id).سر_2;
    liga_subst_مفرد->format = هذا->format;
    liga_subst_مفرد->ligature_سيٽ.len = هذا->ligature_سيٽ.len;

    // Create a place holder coverage prime id since we need to add virtual links to it while
    // generating liga and liga sets. Afterwards it will be updated to have the correct coverage.
    TRY_سونپيو (بنانشان coverage_id, c.گراف.اشاريو_for_وٿي (this_اشاريو, &coverage));
    TRY_سونپيو (بنانشان coverage_مفرد_id, c.گراف.duplicate(coverage_id));
    خودڪار& coverage_مفرد_راس = c.گراف.vertices_[coverage_مفرد_id];

    TRY(c.گراف.vertices_[liga_subst_مفرد_id].جوڙيو_real_link (SmallTypes::سائيز, coverage_مفرد_id, 2));
    TRY(coverage_مفرد_راس.جوڙيو_والد (liga_subst_مفرد_id, خطأ));

    // Locate all liga sets with ligas between start and end.
    // Clone or move them as needed.
    بنانشان ڳڻپ = 0;
    بنانشان liga_سيٽ_ڳڻپ = 0;
    بنانشان liga_سيٽ_شروعات = HB_گراف_INVALID;
    بنانشان liga_سيٽ_پڇاڙي = 0; // inclusive
    لكل (بنانشان i = 0; i < liga_counts.ڊيگهائي; i++)
    {
      بنانشان انگ_ligas = liga_counts[i];

      بنانشان current_شروعات = ڳڻپ;
      بنانشان current_پڇاڙي = ڳڻپ + انگ_ligas;

      إذا (current_شروعات >= پڇاڙي || شروعات >= current_پڇاڙي) {
        // No intersection, so just skip
        ڳڻپ += انگ_ligas;
        استمر;
      }

      TRY_سونپيو (خودڪار liga_سيٽ, c.گراف.as_mutable_جدول<Ligature_سيٽ>(this_اشاريو, &ligature_سيٽ[i]));

      بنانشان liga_سيٽ_اشاريو = liga_سيٽ.اشاريو_2;
      // Bounds may need to be adjusted if some ligas have been previously removed.
      hb_pair_t<بنانشان, بنانشان> liga_bounds = current_liga_سيٽ_bounds(c, liga_سيٽ_اشاريو, liga_سيٽ.راس->obj ());
      current_شروعات = hb_max(ڳڻپ + liga_bounds.پهريون, current_شروعات);
      current_پڇاڙي = hb_min(ڳڻپ + liga_bounds.second, current_پڇاڙي);

      بنانشان liga_سيٽ_مفرد_id;
      إذا (current_شروعات >= شروعات && current_پڇاڙي <= پڇاڙي) {
        // This liga set is fully contined within [start, end)
        // We can move the entire ligaset to the new liga subset object.
        liga_سيٽ_پڇاڙي = i;
        إذا (i < liga_سيٽ_شروعات) liga_سيٽ_شروعات = i;
        TRY_سونپيو (liga_سيٽ_مفرد_id, c.گراف.move_ٻار<> (this_اشاريو,
                              &ligature_سيٽ[i],
                              liga_subst_مفرد_id,
                              &liga_subst_مفرد->ligature_سيٽ[liga_سيٽ_ڳڻپ++]));
        compact_liga_سيٽ (c, liga_سيٽ.جدول, liga_سيٽ.راس);
      }
      وإلا
      {
        // This liga set partially overlaps [start, end). We'll need to create
        // a new liga set sub table and move the intersecting ligas to it.
        بنانشان شروعات_اشاريو = hb_max(شروعات, current_شروعات) - ڳڻپ;
        بنانشان پڇاڙي_اشاريو = hb_min(پڇاڙي, current_پڇاڙي) - ڳڻپ;
        بنانشان liga_ڳڻپ = پڇاڙي_اشاريو - شروعات_اشاريو;
        TRY_سونپيو (خودڪار نتيجو, new_liga_سيٽ(c, liga_ڳڻپ));
        liga_سيٽ_مفرد_id = نتيجو.پهريون;

        TRY (c.گراف.move_children<OT::Offset16>(
          liga_سيٽ_اشاريو,
          2 + شروعات_اشاريو * 2,
          2 + پڇاڙي_اشاريو * 2,
          liga_سيٽ_مفرد_id,
          2));

        liga_سيٽ_پڇاڙي = i;
        إذا (i < liga_سيٽ_شروعات) liga_سيٽ_شروعات = i;
        TRY (c.گراف.جوڙيو_link(&liga_subst_مفرد->ligature_سيٽ[liga_سيٽ_ڳڻپ++], liga_subst_مفرد_id, liga_سيٽ_مفرد_id));
      }

      // The new liga and all children set needs to have a virtual link to the new coverage table:
      ثابت خودڪار& liga_سيٽ_مفرد = c.گراف.vertices_[liga_سيٽ_مفرد_id].obj ();
      clear_virtual_links(c, liga_سيٽ_مفرد_id);
      TRY(جوڙيو_virtual_link(c, liga_سيٽ_مفرد_id, coverage_مفرد_id));
      لكل (ثابت خودڪار& l : liga_سيٽ_مفرد.real_links) {
        clear_virtual_links(c, l.objidx);
        TRY(جوڙيو_virtual_link(c, l.objidx, coverage_مفرد_id));
      }

      ڳڻپ += انگ_ligas;
    }

    c.گراف.vertices_[liga_subst_مفرد_id].shrink_بفر ((liga_subst_مفرد->ligature_سيٽ.len - liga_سيٽ_ڳڻپ) * SmallTypes::سائيز);
    liga_subst_مفرد->ligature_سيٽ.len = liga_سيٽ_ڳڻپ;

    TRY (Coverage::filter_coverage (c,
                                    coverage_مفرد_id,
                                    liga_سيٽ_شروعات, liga_سيٽ_پڇاڙي + 1));

    أرجع Ok(liga_subst_مفرد_id);
  }

  گراف_نتيجو_t<فراغ> shrink (gsubgpos_گراف_context_t& c,
                               بنانشان this_اشاريو,
                               بنانشان old_ڳڻپ,
                               hb_ويڪٽر_t<بنانشان> liga_counts,
                               بنانشان ڳڻپ)
  {
    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
               "  Shrinking LigatureSubstFormat1 (%u) to [0, %u).",
               this_اشاريو,
               ڳڻپ);
    إذا (ڳڻپ >= old_ڳڻپ)
      أرجع Ok();

    hb_سيٽ_t retained_indices;
    بنانشان new_liga_سيٽ_ڳڻپ = 0;
    لكل (بنانشان i = 0; i < liga_counts.ڊيگهائي; i++)
    {
      TRY_سونپيو (خودڪار liga_سيٽ, c.گراف.as_mutable_جدول<Ligature_سيٽ>(this_اشاريو, &ligature_سيٽ[i]));

      // We need the virtual links to coverage removed from all descendants on this liga subst.
      // If any are left when we try to mutate the coverage table later it will be unnessecarily
      // duplicated. Code later on will re-add the virtual links as needed (via retained_indices).
      clear_virtual_links(c, liga_سيٽ.اشاريو_2);
      retained_indices.جوڙيو(liga_سيٽ.اشاريو_2);

      TRY_سونپيو (خودڪار اشاريو_منزل_id, ligature_اشاريو_منزل_object_id (liga_سيٽ));

      لكل (بنانشان i = 0; i < liga_سيٽ.جدول->ligature.len; i++) {
        بنانشان liga_اشاريو = اشاريو_منزل_id[i];
        إذا (liga_اشاريو != HB_گراف_INVALID) {
          clear_virtual_links(c, liga_اشاريو);
          retained_indices.جوڙيو(liga_اشاريو);
        }
      }

      بنانشان انگ_ligas = liga_counts[i];
      إذا (انگ_ligas >= ڳڻپ) {
        // drop the trailing liga's from this set and all subsequent liga sets
        بنانشان انگ_ligas_منزل_remove = انگ_ligas - ڳڻپ;
        new_liga_سيٽ_ڳڻپ = i + 1;
        c.گراف.vertices_[liga_سيٽ.اشاريو_2].shrink_بفر (انگ_ligas_منزل_remove * SmallTypes::سائيز);
        liga_سيٽ.جدول->ligature.len = ڳڻپ;
        اكسر;
      } وإلا {
        ڳڻپ -= انگ_ligas;
      }
    }

    // Adjust liga set array
    خودڪار& this_راس = c.گراف.vertices_[this_اشاريو];
    this_راس.shrink_بفر ((ligature_سيٽ.len - new_liga_سيٽ_ڳڻپ) * SmallTypes::سائيز);
    ligature_سيٽ.len = new_liga_سيٽ_ڳڻپ;

    // Coverage matches the number of liga sets so rebuild as needed
    TRY_سونپيو (بنانشان coverage_idx, c.گراف.اشاريو_for_وٿي (this_اشاريو, &هذا->coverage));

    خودڪار& coverage_v = c.گراف.vertices_[coverage_idx];
    بنانشان coverage_سائيز = coverage_v.جدول_سائيز ();
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj ().سر_2;

    إذا (coverage_v.is_shared ())
    {
      TRY_سونپيو (coverage_idx, c.گراف.remap_ٻار (this_اشاريو, coverage_idx));
    }

    لكل (بنانشان i : retained_indices.iter())
      TRY(جوڙيو_virtual_link(c, i, coverage_idx));

    خودڪار new_coverage =
        + hb_zip (coverage_جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<بنانشان, بنانشان> p) {
          أرجع p.second < new_liga_سيٽ_ڳڻپ;
        })
        | hb_map_retains_ترتيب_ڏيڻ (hb_پهريون)
        ;

    أرجع Coverage::make_coverage (c, new_coverage, coverage_idx, coverage_سائيز);
  }
};

بنية LigatureSubst : عام OT::Layout::GSUB_impl::LigatureSubst
{

  گراف_نتيجو_t<hb_ويڪٽر_t<بنانشان>> ورهايو_subtables (gsubgpos_گراف_context_t& c,
                                                         بنانشان this_اشاريو)
  {
    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((LigatureSubstFormat1*)(&u.format1))->ورهايو_subtables (c, this_اشاريو);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 2: HB_FALLTHROUGH;
      // 24-bit LigatureSubst already has widened offsets; leave it unsplit.
#جيڪڏهن_پورو
    افتراضي:
      أرجع Ok(hb_ويڪٽر_t<بنانشان> ());
    }
  }

  گراف_نتيجو_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائيز ();
    إذا (unlikely (راس_len < u.format.v.get_سائيز ())) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((LigatureSubstFormat1*)(&u.format1))->sanitize (راس);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 2:  HB_FALLTHROUGH;
#جيڪڏهن_پورو
    افتراضي:
      // We don't handle format 2 here.
      أرجع Err(SANITIZE_FAILURE);
    }
  }
};

}

#جيڪڏهن_پورو  // GRAPH_LIGATURE_GRAPH_HH
