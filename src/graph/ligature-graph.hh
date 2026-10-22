
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

#جے_بنا_تعریف گراف_LIGATURE_گراف_HH
#تعریف_کرو گراف_LIGATURE_گراف_HH

#ضمن "graph.hh"
#ضمن "../OT/Layout/GSUB/LigatureSubst.hh"
#ضمن "../OT/Layout/GSUB/LigatureSubstFormat1.hh"
#ضمن "../OT/Layout/GSUB/LigatureSet.hh"
#ضمن "../hb-ot-dual.hh"
#ضمن <algorithm>
#ضمن <utility>

نطاق گراف {

بنية Ligature_سیٹ : عام OT::Layout::GSUB_impl::Ligature_سیٹ<SmallTypes>
{
  گراف_نتیجہ_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_ناپ ();
    إذا (unlikely (راس_len < OT::Layout::GSUB_impl::Ligature_سیٹ<SmallTypes>::min_ناپ)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    size_t کل_len = ligature.get_ناپ() + OT::Layout::GSUB_impl::Ligature_سیٹ<SmallTypes>::min_ناپ - ligature.len.get_ناپ();
    إذا (unlikely (راس_len < کل_len)) {
      أرجع Err(SANITIZE_FAILURE);
    }
    أرجع Ok();
  }
};

بنية LigatureSubstFormat1 : عام OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>
{
  گراف_نتیجہ_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_ناپ ();
    بنانشانی min_ناپ = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_ناپ;
    إذا (unlikely (راس_len < min_ناپ)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (راس_len < min_ناپ + ligature_سیٹ.get_ناپ() - ligature_سیٹ.len.get_ناپ()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتیجہ_t<hb_سمتیہ_t<بنانشانی>> ونڈو_subtables (gsubgpos_گراف_context_t& c,
                                                         بنانشانی this_اشاریہ)
  {
    TRY_سونپو (خودکار ونڈو_points, compute_ونڈو_points(c, this_اشاریہ));
    إذا (!ونڈو_points)
      أرجع Ok(hb_سمتیہ_t<بنانشانی> ());

    TRY_سونپو (بنانشانی کل_ligas, کل_نمبر_ligas(c, this_اشاریہ));
    TRY_سونپو (hb_سمتیہ_t<بنانشانی> counts, liga_counts(c, this_اشاریہ));
    ونڈو_context_t ونڈو_context {
      c,
      هذا,
      this_اشاریہ,
      کل_ligas,
      std::move (counts),
    };
    أرجع actuate_subtable_ونڈو<ونڈو_context_t> (ونڈو_context, ونڈو_points);
  }

 خاص:
  گراف_نتیجہ_t<بنانشانی> کل_نمبر_ligas(gsubgpos_گراف_context_t& c, بنانشانی this_اشاریہ) ثابت {
    بنانشانی کل = 0;
    لكل (بنانشانی i = 0; i < ligature_سیٹ.len; i++)
    {
      TRY_سونپو (خودکار liga_سیٹ, c.گراف.as_جدول<Ligature_سیٹ>(this_اشاریہ, &ligature_سیٹ[i]));
      کل += liga_سیٹ.جدول->ligature.len;
    }
    أرجع Ok(کل);
  }

  گراف_نتیجہ_t<hb_سمتیہ_t<بنانشانی>> liga_counts(gsubgpos_گراف_context_t& c, بنانشانی this_اشاریہ) ثابت {
    hb_سمتیہ_t<بنانشانی> نتیجہ;
    لكل (بنانشانی i = 0; i < ligature_سیٹ.len; i++)
    {
      TRY_سونپو (خودکار liga_سیٹ, c.گراف.as_جدول<Ligature_سیٹ>(this_اشاریہ, &ligature_سیٹ[i]));
      نتیجہ.push(!liga_سیٹ.جدول ? 0 : liga_سیٹ.جدول->ligature.len);
    }
    أرجع گراف_نتیجہ_t<hb_سمتیہ_t<بنانشانی>>::from(std::move(نتیجہ), ونڈ_FAILURE);
  }

  سانچا <گراف_t::راس_mutability_t mutability>
  گراف_نتیجہ_t<hb_سمتیہ_t<بنانشانی>> ligature_اشاریہ_منزل_object_id(ثابت گراف_t::راس_and_جدول_t<Ligature_سیٹ, mutability>& liga_سیٹ) ثابت {
    hb_سمتیہ_t<بنانشانی> map;
    إذا (!map.resize_exact(liga_سیٹ.جدول->ligature.len)) أرجع Err(ونڈ_FAILURE);

    لكل (بنانشانی i = 0; i < map.لمائی; i++) {
      map[i] = HB_گراف_INVALID;
    }

    لكل (ثابت خودکار& l : liga_سیٹ.راس->obj ().real_links) {
      إذا (l.position < 2) استمر;
      بنانشانی array_اشاریہ = (l.position - 2) / 2;
      map[array_اشاریہ] = l.objidx;
    }
    أرجع map;
  }

  گراف_نتیجہ_t<hb_سمتیہ_t<بنانشانی>> compute_ونڈو_points(gsubgpos_گراف_context_t& c,
                                                             بنانشانی this_اشاریہ) ثابت
  {
    // For ligature subst coverage is always packed last, and as a result is where an overflow
    // will happen if there is one, so we can check the estimate length of the
    // LigatureSubstFormat1 -> Coverage offset length which is the sum of all data in the
    // retained sub graph except for the coverage table itself.
    ثابت بنانشانی ادھار_ناپ = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_ناپ;
    بنانشانی accumulated = ادھار_ناپ;

    بنانشانی ligature_اشاریہ = 0;
    hb_سمتیہ_t<بنانشانی> ونڈو_points;
    لكل (بنانشانی i = 0; i < ligature_سیٹ.len; i++)
    {
      accumulated += OT::HBUINT16::static_ناپ; // for ligature set offset
      accumulated += OT::Layout::GSUB_impl::Ligature_سیٹ<SmallTypes>::min_ناپ; // for ligature set table

      TRY_سونپو (خودکار liga_سیٹ, c.گراف.as_جدول<Ligature_سیٹ>(this_اشاریہ, &ligature_سیٹ[i]));

      // Finding the object id associated with an array index is O(n)
      // so to avoid O(n^2), precompute the mapping by scanning through
      // all links
      TRY_سونپو (خودکار اشاریہ_منزل_id, ligature_اشاریہ_منزل_object_id (liga_سیٹ));

      لكل (بنانشانی j = 0; j < liga_سیٹ.جدول->ligature.len; j++)
      {
        ثابت بنانشانی liga_id = اشاریہ_منزل_id[j];
        إذا (liga_id == HB_گراف_INVALID) استمر; // no outgoing link, ignore
        ثابت بنانشانی liga_ناپ = c.گراف.vertices_[liga_id].جدول_ناپ ();

        accumulated += OT::HBUINT16::static_ناپ; // for ligature offset
        accumulated += liga_ناپ; // for the ligature table

        إذا (accumulated >= (1 << 16))
        {
          ونڈو_points.push(ligature_اشاریہ);
          // We're going to split such that the current ligature will be in the new sub table.
          // That means we'll have one ligature subst (base_base), one ligature set, and one liga table
          accumulated = ادھار_ناپ + // for liga subst subtable
            (OT::HBUINT16::static_ناپ * 2) + // for liga set and liga offset
            OT::Layout::GSUB_impl::Ligature_سیٹ<SmallTypes>::min_ناپ + // for liga set subtable
            liga_ناپ; // for liga sub table
        }

        ligature_اشاریہ++;
      }
    }

    أرجع ونڈو_points;
  }

  بنية ونڈو_context_t
  {
    gsubgpos_گراف_context_t& c;
    LigatureSubstFormat1* thiz;
    بنانشانی this_اشاریہ;
    بنانشانی original_گنتی;
    hb_سمتیہ_t<بنانشانی> liga_counts;

    بنانشانی original_گنتی_2 ()
    {
      أرجع original_گنتی;
    }

    گراف_نتیجہ_t<بنانشانی> clone_range (بنانشانی شروع, بنانشانی سرا)
    {
      أرجع thiz->clone_range (c, this_اشاریہ, liga_counts, شروع, سرا);
    }

    گراف_نتیجہ_t<فراغ> shrink (بنانشانی گنتی)
    {
      أرجع thiz->shrink (c, this_اشاریہ, original_گنتی_2(), liga_counts, گنتی);
    }
  };

  گراف_نتیجہ_t<hb_pair_t<بنانشانی, Ligature_سیٹ*>> new_liga_سیٹ(gsubgpos_گراف_context_t& c, بنانشانی گنتی) ثابت {
    بنانشانی مفرد_ناپ = OT::Layout::GSUB_impl::Ligature_سیٹ<SmallTypes>::min_ناپ
                          + گنتی * SmallTypes::ناپ;

    TRY_سونپو (بنانشانی مفرد_id, c.create_گرہ (مفرد_ناپ));

    Ligature_سیٹ* مفرد = (Ligature_سیٹ*) c.گراف.object (مفرد_id).سر_2;
    مفرد->ligature.len = گنتی;
    أرجع Ok(hb_pair(مفرد_id, مفرد));
  }

  فراغ clear_virtual_links (gsubgpos_گراف_context_t& c, بنانشانی گرہ_اشاریہ) ثابت
  {
    خودکار& v  = c.گراف.vertices_[گرہ_اشاریہ];
    لكل (ثابت خودکار& l : v.obj ().virtual_links)
    {
      خودکار& بچہ = c.گراف.vertices_[l.objidx];
      بچہ.remove_ماپا(گرہ_اشاریہ);
    }
    v.clear_virtual_links ();
  }

  گراف_نتیجہ_t<فراغ> جوڑو_virtual_link(gsubgpos_گراف_context_t& c, بنانشانی from, بنانشانی to) ثابت {
    إذا (unlikely (from >= c.گراف.vertices_.لمائی || to >= c.گراف.vertices_.لمائی)) أرجع Err(باہرلا_دا_BOUNDS);
    TRY(c.گراف.vertices_[to].جوڑو_ماپا (from, سچ));
    TRY(c.گراف.vertices_[from].جوڑو_virtual_link (to));
    أرجع Ok();
  }

  hb_pair_t<بنانشانی, بنانشانی> current_liga_سیٹ_bounds (gsubgpos_گراف_context_t& c,
                                                         بنانشانی liga_سیٹ_اشاریہ,
                                                         ثابت hb_serialize_context_t::object_t& liga_سیٹ) ثابت
  {
    // Finds the actual liga indices present in the liga set currently. Takes
    // into account those that have been removed by processing.
    بنانشانی min_اشاریہ = HB_گراف_INVALID;
    بنانشانی max_اشاریہ = 0;
    لكل (ثابت خودکار& l : liga_سیٹ.real_links) {
      إذا (l.position < 2) استمر;

      بنانشانی liga_اشاریہ = (l.position - 2) / 2;
      min_اشاریہ = hb_min(min_اشاریہ, liga_اشاریہ);
      max_اشاریہ = hb_max(max_اشاریہ, liga_اشاریہ);
    }
    أرجع hb_pair(min_اشاریہ, max_اشاریہ + 1);
  }

  فراغ compact_liga_سیٹ (gsubgpos_گراف_context_t& c, Ligature_سیٹ* جدول, گراف_t::راس_t* v) ثابت
  {
    ثابت خودکار& obj = v->obj ();
    إذا (جدول->ligature.len <= obj.real_links.لمائی) أرجع;

    // compact the remaining linked liga offsets into a continous array and shrink the node as needed.
    بنانشانی منزل_remove = جدول->ligature.len - obj.real_links.لمائی;
    بنانشانی new_position = SmallTypes::ناپ;
    v->ترتیب_دیو_real_links (); // for this to work we need to process links in order of position.
    لكل (خودکار& l : v->real_links_writer ())
    {
      l.position = new_position;
      new_position += SmallTypes::ناپ;
    }

    جدول->ligature.len = obj.real_links.لمائی;
    v->shrink_بفر (منزل_remove * SmallTypes::ناپ);
  }

  گراف_نتیجہ_t<بنانشانی> clone_range (gsubgpos_گراف_context_t& c,
                                        بنانشانی this_اشاریہ,
                                        hb_سمتیہ_t<بنانشانی> liga_counts,
                                        بنانشانی شروع, بنانشانی سرا) ثابت
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
               "  Cloning LigatureSubstFormat1 (%u) range [%u, %u).", this_اشاریہ, شروع, سرا);

    // Create an oversized new liga subst, we'll adjust the size down later. We don't know
    // the final size until we process it but we also need it to exist while we're processing
    // so that nodes can be moved to it as needed.
    بنانشانی مفرد_ناپ = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_ناپ
                          + ligature_سیٹ.get_ناپ() - ligature_سیٹ.len.get_ناپ();

    TRY_سونپو (بنانشانی liga_subst_مفرد_id, c.create_گرہ (مفرد_ناپ));

    LigatureSubstFormat1* liga_subst_مفرد = (LigatureSubstFormat1*) c.گراف.object (liga_subst_مفرد_id).سر_2;
    liga_subst_مفرد->format = هذا->format;
    liga_subst_مفرد->ligature_سیٹ.len = هذا->ligature_سیٹ.len;

    // Create a place holder coverage prime id since we need to add virtual links to it while
    // generating liga and liga sets. Afterwards it will be updated to have the correct coverage.
    TRY_سونپو (بنانشانی coverage_id, c.گراف.اشاریہ_for_وتھ (this_اشاریہ, &coverage));
    TRY_سونپو (بنانشانی coverage_مفرد_id, c.گراف.duplicate(coverage_id));
    خودکار& coverage_مفرد_راس = c.گراف.vertices_[coverage_مفرد_id];

    TRY(c.گراف.vertices_[liga_subst_مفرد_id].جوڑو_real_link (SmallTypes::ناپ, coverage_مفرد_id, 2));
    TRY(coverage_مفرد_راس.جوڑو_ماپا (liga_subst_مفرد_id, خطأ));

    // Locate all liga sets with ligas between start and end.
    // Clone or move them as needed.
    بنانشانی گنتی = 0;
    بنانشانی liga_سیٹ_گنتی = 0;
    بنانشانی liga_سیٹ_شروع = HB_گراف_INVALID;
    بنانشانی liga_سیٹ_سرا = 0; // inclusive
    لكل (بنانشانی i = 0; i < liga_counts.لمائی; i++)
    {
      بنانشانی عدد_ligas = liga_counts[i];

      بنانشانی current_شروع = گنتی;
      بنانشانی current_سرا = گنتی + عدد_ligas;

      إذا (current_شروع >= سرا || شروع >= current_سرا) {
        // No intersection, so just skip
        گنتی += عدد_ligas;
        استمر;
      }

      TRY_سونپو (خودکار liga_سیٹ, c.گراف.as_mutable_جدول<Ligature_سیٹ>(this_اشاریہ, &ligature_سیٹ[i]));

      بنانشانی liga_سیٹ_اشاریہ = liga_سیٹ.اشاریہ_2;
      // Bounds may need to be adjusted if some ligas have been previously removed.
      hb_pair_t<بنانشانی, بنانشانی> liga_bounds = current_liga_سیٹ_bounds(c, liga_سیٹ_اشاریہ, liga_سیٹ.راس->obj ());
      current_شروع = hb_max(گنتی + liga_bounds.پہلا, current_شروع);
      current_سرا = hb_min(گنتی + liga_bounds.second, current_سرا);

      بنانشانی liga_سیٹ_مفرد_id;
      إذا (current_شروع >= شروع && current_سرا <= سرا) {
        // This liga set is fully contined within [start, end)
        // We can move the entire ligaset to the new liga subset object.
        liga_سیٹ_سرا = i;
        إذا (i < liga_سیٹ_شروع) liga_سیٹ_شروع = i;
        TRY_سونپو (liga_سیٹ_مفرد_id, c.گراف.move_بچہ<> (this_اشاریہ,
                              &ligature_سیٹ[i],
                              liga_subst_مفرد_id,
                              &liga_subst_مفرد->ligature_سیٹ[liga_سیٹ_گنتی++]));
        compact_liga_سیٹ (c, liga_سیٹ.جدول, liga_سیٹ.راس);
      }
      وإلا
      {
        // This liga set partially overlaps [start, end). We'll need to create
        // a new liga set sub table and move the intersecting ligas to it.
        بنانشانی شروع_اشاریہ = hb_max(شروع, current_شروع) - گنتی;
        بنانشانی سرا_اشاریہ = hb_min(سرا, current_سرا) - گنتی;
        بنانشانی liga_گنتی = سرا_اشاریہ - شروع_اشاریہ;
        TRY_سونپو (خودکار نتیجہ, new_liga_سیٹ(c, liga_گنتی));
        liga_سیٹ_مفرد_id = نتیجہ.پہلا;

        TRY (c.گراف.move_children<OT::Offset16>(
          liga_سیٹ_اشاریہ,
          2 + شروع_اشاریہ * 2,
          2 + سرا_اشاریہ * 2,
          liga_سیٹ_مفرد_id,
          2));

        liga_سیٹ_سرا = i;
        إذا (i < liga_سیٹ_شروع) liga_سیٹ_شروع = i;
        TRY (c.گراف.جوڑو_link(&liga_subst_مفرد->ligature_سیٹ[liga_سیٹ_گنتی++], liga_subst_مفرد_id, liga_سیٹ_مفرد_id));
      }

      // The new liga and all children set needs to have a virtual link to the new coverage table:
      ثابت خودکار& liga_سیٹ_مفرد = c.گراف.vertices_[liga_سیٹ_مفرد_id].obj ();
      clear_virtual_links(c, liga_سیٹ_مفرد_id);
      TRY(جوڑو_virtual_link(c, liga_سیٹ_مفرد_id, coverage_مفرد_id));
      لكل (ثابت خودکار& l : liga_سیٹ_مفرد.real_links) {
        clear_virtual_links(c, l.objidx);
        TRY(جوڑو_virtual_link(c, l.objidx, coverage_مفرد_id));
      }

      گنتی += عدد_ligas;
    }

    c.گراف.vertices_[liga_subst_مفرد_id].shrink_بفر ((liga_subst_مفرد->ligature_سیٹ.len - liga_سیٹ_گنتی) * SmallTypes::ناپ);
    liga_subst_مفرد->ligature_سیٹ.len = liga_سیٹ_گنتی;

    TRY (Coverage::filter_coverage (c,
                                    coverage_مفرد_id,
                                    liga_سیٹ_شروع, liga_سیٹ_سرا + 1));

    أرجع Ok(liga_subst_مفرد_id);
  }

  گراف_نتیجہ_t<فراغ> shrink (gsubgpos_گراف_context_t& c,
                               بنانشانی this_اشاریہ,
                               بنانشانی old_گنتی,
                               hb_سمتیہ_t<بنانشانی> liga_counts,
                               بنانشانی گنتی)
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
               "  Shrinking LigatureSubstFormat1 (%u) to [0, %u).",
               this_اشاریہ,
               گنتی);
    إذا (گنتی >= old_گنتی)
      أرجع Ok();

    hb_سیٹ_t retained_indices;
    بنانشانی new_liga_سیٹ_گنتی = 0;
    لكل (بنانشانی i = 0; i < liga_counts.لمائی; i++)
    {
      TRY_سونپو (خودکار liga_سیٹ, c.گراف.as_mutable_جدول<Ligature_سیٹ>(this_اشاریہ, &ligature_سیٹ[i]));

      // We need the virtual links to coverage removed from all descendants on this liga subst.
      // If any are left when we try to mutate the coverage table later it will be unnessecarily
      // duplicated. Code later on will re-add the virtual links as needed (via retained_indices).
      clear_virtual_links(c, liga_سیٹ.اشاریہ_2);
      retained_indices.جوڑو(liga_سیٹ.اشاریہ_2);

      TRY_سونپو (خودکار اشاریہ_منزل_id, ligature_اشاریہ_منزل_object_id (liga_سیٹ));

      لكل (بنانشانی i = 0; i < liga_سیٹ.جدول->ligature.len; i++) {
        بنانشانی liga_اشاریہ = اشاریہ_منزل_id[i];
        إذا (liga_اشاریہ != HB_گراف_INVALID) {
          clear_virtual_links(c, liga_اشاریہ);
          retained_indices.جوڑو(liga_اشاریہ);
        }
      }

      بنانشانی عدد_ligas = liga_counts[i];
      إذا (عدد_ligas >= گنتی) {
        // drop the trailing liga's from this set and all subsequent liga sets
        بنانشانی عدد_ligas_منزل_remove = عدد_ligas - گنتی;
        new_liga_سیٹ_گنتی = i + 1;
        c.گراف.vertices_[liga_سیٹ.اشاریہ_2].shrink_بفر (عدد_ligas_منزل_remove * SmallTypes::ناپ);
        liga_سیٹ.جدول->ligature.len = گنتی;
        اكسر;
      } وإلا {
        گنتی -= عدد_ligas;
      }
    }

    // Adjust liga set array
    خودکار& this_راس = c.گراف.vertices_[this_اشاریہ];
    this_راس.shrink_بفر ((ligature_سیٹ.len - new_liga_سیٹ_گنتی) * SmallTypes::ناپ);
    ligature_سیٹ.len = new_liga_سیٹ_گنتی;

    // Coverage matches the number of liga sets so rebuild as needed
    TRY_سونپو (بنانشانی coverage_idx, c.گراف.اشاریہ_for_وتھ (this_اشاریہ, &هذا->coverage));

    خودکار& coverage_v = c.گراف.vertices_[coverage_idx];
    بنانشانی coverage_ناپ = coverage_v.جدول_ناپ ();
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj ().سر_2;

    إذا (coverage_v.is_shared ())
    {
      TRY_سونپو (coverage_idx, c.گراف.remap_بچہ (this_اشاریہ, coverage_idx));
    }

    لكل (بنانشانی i : retained_indices.iter())
      TRY(جوڑو_virtual_link(c, i, coverage_idx));

    خودکار new_coverage =
        + hb_zip (coverage_جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<بنانشانی, بنانشانی> p) {
          أرجع p.second < new_liga_سیٹ_گنتی;
        })
        | hb_map_retains_چھانٹی (hb_پہلا)
        ;

    أرجع Coverage::make_coverage (c, new_coverage, coverage_idx, coverage_ناپ);
  }
};

بنية LigatureSubst : عام OT::Layout::GSUB_impl::LigatureSubst
{

  گراف_نتیجہ_t<hb_سمتیہ_t<بنانشانی>> ونڈو_subtables (gsubgpos_گراف_context_t& c,
                                                         بنانشانی this_اشاریہ)
  {
    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((LigatureSubstFormat1*)(&u.format1))->ونڈو_subtables (c, this_اشاریہ);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 2: HB_FALLTHROUGH;
      // 24-bit LigatureSubst already has widened offsets; leave it unsplit.
#جے_انت
    افتراضي:
      أرجع Ok(hb_سمتیہ_t<بنانشانی> ());
    }
  }

  گراف_نتیجہ_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_ناپ ();
    إذا (unlikely (راس_len < u.format.v.get_ناپ ())) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((LigatureSubstFormat1*)(&u.format1))->sanitize (راس);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 2:  HB_FALLTHROUGH;
#جے_انت
    افتراضي:
      // We don't handle format 2 here.
      أرجع Err(SANITIZE_FAILURE);
    }
  }
};

}

#جے_انت  // GRAPH_LIGATURE_GRAPH_HH
