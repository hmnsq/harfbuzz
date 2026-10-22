
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

#اگر_غیر_تعریف_شدہ گراف_LIGATURE_گراف_HH
#تعریف_کرو گراف_LIGATURE_گراف_HH

#شامل "graph.hh"
#شامل "../OT/Layout/GSUB/LigatureSubst.hh"
#شامل "../OT/Layout/GSUB/LigatureSubstFormat1.hh"
#شامل "../OT/Layout/GSUB/LigatureSet.hh"
#شامل "../hb-ot-dual.hh"
#شامل <algorithm>
#شامل <utility>

نامخلا گراف {

ساخت Ligature_سیٹ : عوامی OT::Layout::GSUB_impl::Ligature_سیٹ<SmallTypes>
{
  گراف_نتیجہ_t<خالی> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائز ();
    اگر (unlikely (راس_len < OT::Layout::GSUB_impl::Ligature_سیٹ<SmallTypes>::min_سائز)) واپس Err(SANITIZE_FAILURE);
    hb_barrier ();

    size_t کل_len = ligature.get_سائز() + OT::Layout::GSUB_impl::Ligature_سیٹ<SmallTypes>::min_سائز - ligature.len.get_سائز();
    اگر (unlikely (راس_len < کل_len)) {
      واپس Err(SANITIZE_FAILURE);
    }
    واپس Ok();
  }
};

ساخت LigatureSubstFormat1 : عوامی OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>
{
  گراف_نتیجہ_t<خالی> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائز ();
    غیرعلامتی min_سائز = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_سائز;
    اگر (unlikely (راس_len < min_سائز)) واپس Err(SANITIZE_FAILURE);
    hb_barrier ();

    اگر (unlikely (راس_len < min_سائز + ligature_سیٹ.get_سائز() - ligature_سیٹ.len.get_سائز()))
      واپس Err(SANITIZE_FAILURE);
    واپس Ok();
  }

  گراف_نتیجہ_t<hb_سمتیہ_t<غیرعلامتی>> بانٹو_subtables (gsubgpos_گراف_context_t& c,
                                                         غیرعلامتی this_اشاریہ)
  {
    TRY_تفویض (خودکار بانٹو_points, compute_بانٹو_points(c, this_اشاریہ));
    اگر (!بانٹو_points)
      واپس Ok(hb_سمتیہ_t<غیرعلامتی> ());

    TRY_تفویض (غیرعلامتی کل_ligas, کل_نمبر_ligas(c, this_اشاریہ));
    TRY_تفویض (hb_سمتیہ_t<غیرعلامتی> counts, liga_counts(c, this_اشاریہ));
    بانٹو_context_t بانٹو_context {
      c,
      یہ,
      this_اشاریہ,
      کل_ligas,
      std::move (counts),
    };
    واپس actuate_subtable_بانٹو<بانٹو_context_t> (بانٹو_context, بانٹو_points);
  }

 نجی:
  گراف_نتیجہ_t<غیرعلامتی> کل_نمبر_ligas(gsubgpos_گراف_context_t& c, غیرعلامتی this_اشاریہ) ثابت {
    غیرعلامتی کل = 0;
    برائے (غیرعلامتی i = 0; i < ligature_سیٹ.len; i++)
    {
      TRY_تفویض (خودکار liga_سیٹ, c.گراف.as_جدول<Ligature_سیٹ>(this_اشاریہ, &ligature_سیٹ[i]));
      کل += liga_سیٹ.جدول->ligature.len;
    }
    واپس Ok(کل);
  }

  گراف_نتیجہ_t<hb_سمتیہ_t<غیرعلامتی>> liga_counts(gsubgpos_گراف_context_t& c, غیرعلامتی this_اشاریہ) ثابت {
    hb_سمتیہ_t<غیرعلامتی> نتیجہ;
    برائے (غیرعلامتی i = 0; i < ligature_سیٹ.len; i++)
    {
      TRY_تفویض (خودکار liga_سیٹ, c.گراف.as_جدول<Ligature_سیٹ>(this_اشاریہ, &ligature_سیٹ[i]));
      نتیجہ.push(!liga_سیٹ.جدول ? 0 : liga_سیٹ.جدول->ligature.len);
    }
    واپس گراف_نتیجہ_t<hb_سمتیہ_t<غیرعلامتی>>::from(std::move(نتیجہ), تخصیص_FAILURE);
  }

  سانچہ <گراف_t::راس_mutability_t mutability>
  گراف_نتیجہ_t<hb_سمتیہ_t<غیرعلامتی>> ligature_اشاریہ_مقصد_object_id(ثابت گراف_t::راس_and_جدول_t<Ligature_سیٹ, mutability>& liga_سیٹ) ثابت {
    hb_سمتیہ_t<غیرعلامتی> map;
    اگر (!map.resize_exact(liga_سیٹ.جدول->ligature.len)) واپس Err(تخصیص_FAILURE);

    برائے (غیرعلامتی i = 0; i < map.لمبائی; i++) {
      map[i] = HB_گراف_INVALID;
    }

    برائے (ثابت خودکار& l : liga_سیٹ.راس->obj ().real_links) {
      اگر (l.position < 2) جاری;
      غیرعلامتی array_اشاریہ = (l.position - 2) / 2;
      map[array_اشاریہ] = l.objidx;
    }
    واپس map;
  }

  گراف_نتیجہ_t<hb_سمتیہ_t<غیرعلامتی>> compute_بانٹو_points(gsubgpos_گراف_context_t& c,
                                                             غیرعلامتی this_اشاریہ) ثابت
  {
    // For ligature subst coverage is always packed last, and as a result is where an overflow
    // will happen if there is one, so we can check the estimate length of the
    // LigatureSubstFormat1 -> Coverage offset length which is the sum of all data in the
    // retained sub graph except for the coverage table itself.
    ثابت غیرعلامتی بنیاد_سائز = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_سائز;
    غیرعلامتی accumulated = بنیاد_سائز;

    غیرعلامتی ligature_اشاریہ = 0;
    hb_سمتیہ_t<غیرعلامتی> بانٹو_points;
    برائے (غیرعلامتی i = 0; i < ligature_سیٹ.len; i++)
    {
      accumulated += OT::HBUINT16::static_سائز; // for ligature set offset
      accumulated += OT::Layout::GSUB_impl::Ligature_سیٹ<SmallTypes>::min_سائز; // for ligature set table

      TRY_تفویض (خودکار liga_سیٹ, c.گراف.as_جدول<Ligature_سیٹ>(this_اشاریہ, &ligature_سیٹ[i]));

      // Finding the object id associated with an array index is O(n)
      // so to avoid O(n^2), precompute the mapping by scanning through
      // all links
      TRY_تفویض (خودکار اشاریہ_مقصد_id, ligature_اشاریہ_مقصد_object_id (liga_سیٹ));

      برائے (غیرعلامتی j = 0; j < liga_سیٹ.جدول->ligature.len; j++)
      {
        ثابت غیرعلامتی liga_id = اشاریہ_مقصد_id[j];
        اگر (liga_id == HB_گراف_INVALID) جاری; // no outgoing link, ignore
        ثابت غیرعلامتی liga_سائز = c.گراف.vertices_[liga_id].جدول_سائز ();

        accumulated += OT::HBUINT16::static_سائز; // for ligature offset
        accumulated += liga_سائز; // for the ligature table

        اگر (accumulated >= (1 << 16))
        {
          بانٹو_points.push(ligature_اشاریہ);
          // We're going to split such that the current ligature will be in the new sub table.
          // That means we'll have one ligature subst (base_base), one ligature set, and one liga table
          accumulated = بنیاد_سائز + // for liga subst subtable
            (OT::HBUINT16::static_سائز * 2) + // for liga set and liga offset
            OT::Layout::GSUB_impl::Ligature_سیٹ<SmallTypes>::min_سائز + // for liga set subtable
            liga_سائز; // for liga sub table
        }

        ligature_اشاریہ++;
      }
    }

    واپس بانٹو_points;
  }

  ساخت بانٹو_context_t
  {
    gsubgpos_گراف_context_t& c;
    LigatureSubstFormat1* thiz;
    غیرعلامتی this_اشاریہ;
    غیرعلامتی original_گنتی;
    hb_سمتیہ_t<غیرعلامتی> liga_counts;

    غیرعلامتی original_گنتی_2 ()
    {
      واپس original_گنتی;
    }

    گراف_نتیجہ_t<غیرعلامتی> clone_range (غیرعلامتی آغاز, غیرعلامتی انتہا)
    {
      واپس thiz->clone_range (c, this_اشاریہ, liga_counts, آغاز, انتہا);
    }

    گراف_نتیجہ_t<خالی> shrink (غیرعلامتی گنتی)
    {
      واپس thiz->shrink (c, this_اشاریہ, original_گنتی_2(), liga_counts, گنتی);
    }
  };

  گراف_نتیجہ_t<hb_pair_t<غیرعلامتی, Ligature_سیٹ*>> new_liga_سیٹ(gsubgpos_گراف_context_t& c, غیرعلامتی گنتی) ثابت {
    غیرعلامتی مفرد_سائز = OT::Layout::GSUB_impl::Ligature_سیٹ<SmallTypes>::min_سائز
                          + گنتی * SmallTypes::سائز;

    TRY_تفویض (غیرعلامتی مفرد_id, c.create_گرہ (مفرد_سائز));

    Ligature_سیٹ* مفرد = (Ligature_سیٹ*) c.گراف.object (مفرد_id).سر_2;
    مفرد->ligature.len = گنتی;
    واپس Ok(hb_pair(مفرد_id, مفرد));
  }

  خالی clear_virtual_links (gsubgpos_گراف_context_t& c, غیرعلامتی گرہ_اشاریہ) ثابت
  {
    خودکار& v  = c.گراف.vertices_[گرہ_اشاریہ];
    برائے (ثابت خودکار& l : v.obj ().virtual_links)
    {
      خودکار& بچہ = c.گراف.vertices_[l.objidx];
      بچہ.remove_والد(گرہ_اشاریہ);
    }
    v.clear_virtual_links ();
  }

  گراف_نتیجہ_t<خالی> جوڑو_virtual_link(gsubgpos_گراف_context_t& c, غیرعلامتی from, غیرعلامتی to) ثابت {
    اگر (unlikely (from >= c.گراف.vertices_.لمبائی || to >= c.گراف.vertices_.لمبائی)) واپس Err(برآمد_کا_BOUNDS);
    TRY(c.گراف.vertices_[to].جوڑو_والد (from, سچ));
    TRY(c.گراف.vertices_[from].جوڑو_virtual_link (to));
    واپس Ok();
  }

  hb_pair_t<غیرعلامتی, غیرعلامتی> current_liga_سیٹ_bounds (gsubgpos_گراف_context_t& c,
                                                         غیرعلامتی liga_سیٹ_اشاریہ,
                                                         ثابت hb_serialize_context_t::object_t& liga_سیٹ) ثابت
  {
    // Finds the actual liga indices present in the liga set currently. Takes
    // into account those that have been removed by processing.
    غیرعلامتی min_اشاریہ = HB_گراف_INVALID;
    غیرعلامتی max_اشاریہ = 0;
    برائے (ثابت خودکار& l : liga_سیٹ.real_links) {
      اگر (l.position < 2) جاری;

      غیرعلامتی liga_اشاریہ = (l.position - 2) / 2;
      min_اشاریہ = hb_min(min_اشاریہ, liga_اشاریہ);
      max_اشاریہ = hb_max(max_اشاریہ, liga_اشاریہ);
    }
    واپس hb_pair(min_اشاریہ, max_اشاریہ + 1);
  }

  خالی compact_liga_سیٹ (gsubgpos_گراف_context_t& c, Ligature_سیٹ* جدول, گراف_t::راس_t* v) ثابت
  {
    ثابت خودکار& obj = v->obj ();
    اگر (جدول->ligature.len <= obj.real_links.لمبائی) واپس;

    // compact the remaining linked liga offsets into a continous array and shrink the node as needed.
    غیرعلامتی مقصد_remove = جدول->ligature.len - obj.real_links.لمبائی;
    غیرعلامتی new_position = SmallTypes::سائز;
    v->ترتیب_دو_real_links (); // for this to work we need to process links in order of position.
    برائے (خودکار& l : v->real_links_writer ())
    {
      l.position = new_position;
      new_position += SmallTypes::سائز;
    }

    جدول->ligature.len = obj.real_links.لمبائی;
    v->shrink_بفر (مقصد_remove * SmallTypes::سائز);
  }

  گراف_نتیجہ_t<غیرعلامتی> clone_range (gsubgpos_گراف_context_t& c,
                                        غیرعلامتی this_اشاریہ,
                                        hb_سمتیہ_t<غیرعلامتی> liga_counts,
                                        غیرعلامتی آغاز, غیرعلامتی انتہا) ثابت
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
               "  Cloning LigatureSubstFormat1 (%u) range [%u, %u).", this_اشاریہ, آغاز, انتہا);

    // Create an oversized new liga subst, we'll adjust the size down later. We don't know
    // the final size until we process it but we also need it to exist while we're processing
    // so that nodes can be moved to it as needed.
    غیرعلامتی مفرد_سائز = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_سائز
                          + ligature_سیٹ.get_سائز() - ligature_سیٹ.len.get_سائز();

    TRY_تفویض (غیرعلامتی liga_subst_مفرد_id, c.create_گرہ (مفرد_سائز));

    LigatureSubstFormat1* liga_subst_مفرد = (LigatureSubstFormat1*) c.گراف.object (liga_subst_مفرد_id).سر_2;
    liga_subst_مفرد->format = یہ->format;
    liga_subst_مفرد->ligature_سیٹ.len = یہ->ligature_سیٹ.len;

    // Create a place holder coverage prime id since we need to add virtual links to it while
    // generating liga and liga sets. Afterwards it will be updated to have the correct coverage.
    TRY_تفویض (غیرعلامتی coverage_id, c.گراف.اشاریہ_for_انحراف (this_اشاریہ, &coverage));
    TRY_تفویض (غیرعلامتی coverage_مفرد_id, c.گراف.duplicate(coverage_id));
    خودکار& coverage_مفرد_راس = c.گراف.vertices_[coverage_مفرد_id];

    TRY(c.گراف.vertices_[liga_subst_مفرد_id].جوڑو_real_link (SmallTypes::سائز, coverage_مفرد_id, 2));
    TRY(coverage_مفرد_راس.جوڑو_والد (liga_subst_مفرد_id, جھوٹ));

    // Locate all liga sets with ligas between start and end.
    // Clone or move them as needed.
    غیرعلامتی گنتی = 0;
    غیرعلامتی liga_سیٹ_گنتی = 0;
    غیرعلامتی liga_سیٹ_آغاز = HB_گراف_INVALID;
    غیرعلامتی liga_سیٹ_انتہا = 0; // inclusive
    برائے (غیرعلامتی i = 0; i < liga_counts.لمبائی; i++)
    {
      غیرعلامتی عدد_ligas = liga_counts[i];

      غیرعلامتی current_آغاز = گنتی;
      غیرعلامتی current_انتہا = گنتی + عدد_ligas;

      اگر (current_آغاز >= انتہا || آغاز >= current_انتہا) {
        // No intersection, so just skip
        گنتی += عدد_ligas;
        جاری;
      }

      TRY_تفویض (خودکار liga_سیٹ, c.گراف.as_mutable_جدول<Ligature_سیٹ>(this_اشاریہ, &ligature_سیٹ[i]));

      غیرعلامتی liga_سیٹ_اشاریہ = liga_سیٹ.اشاریہ_2;
      // Bounds may need to be adjusted if some ligas have been previously removed.
      hb_pair_t<غیرعلامتی, غیرعلامتی> liga_bounds = current_liga_سیٹ_bounds(c, liga_سیٹ_اشاریہ, liga_سیٹ.راس->obj ());
      current_آغاز = hb_max(گنتی + liga_bounds.پہلا, current_آغاز);
      current_انتہا = hb_min(گنتی + liga_bounds.second, current_انتہا);

      غیرعلامتی liga_سیٹ_مفرد_id;
      اگر (current_آغاز >= آغاز && current_انتہا <= انتہا) {
        // This liga set is fully contined within [start, end)
        // We can move the entire ligaset to the new liga subset object.
        liga_سیٹ_انتہا = i;
        اگر (i < liga_سیٹ_آغاز) liga_سیٹ_آغاز = i;
        TRY_تفویض (liga_سیٹ_مفرد_id, c.گراف.move_بچہ<> (this_اشاریہ,
                              &ligature_سیٹ[i],
                              liga_subst_مفرد_id,
                              &liga_subst_مفرد->ligature_سیٹ[liga_سیٹ_گنتی++]));
        compact_liga_سیٹ (c, liga_سیٹ.جدول, liga_سیٹ.راس);
      }
      ورنہ
      {
        // This liga set partially overlaps [start, end). We'll need to create
        // a new liga set sub table and move the intersecting ligas to it.
        غیرعلامتی آغاز_اشاریہ = hb_max(آغاز, current_آغاز) - گنتی;
        غیرعلامتی انتہا_اشاریہ = hb_min(انتہا, current_انتہا) - گنتی;
        غیرعلامتی liga_گنتی = انتہا_اشاریہ - آغاز_اشاریہ;
        TRY_تفویض (خودکار نتیجہ, new_liga_سیٹ(c, liga_گنتی));
        liga_سیٹ_مفرد_id = نتیجہ.پہلا;

        TRY (c.گراف.move_children<OT::Offset16>(
          liga_سیٹ_اشاریہ,
          2 + آغاز_اشاریہ * 2,
          2 + انتہا_اشاریہ * 2,
          liga_سیٹ_مفرد_id,
          2));

        liga_سیٹ_انتہا = i;
        اگر (i < liga_سیٹ_آغاز) liga_سیٹ_آغاز = i;
        TRY (c.گراف.جوڑو_link(&liga_subst_مفرد->ligature_سیٹ[liga_سیٹ_گنتی++], liga_subst_مفرد_id, liga_سیٹ_مفرد_id));
      }

      // The new liga and all children set needs to have a virtual link to the new coverage table:
      ثابت خودکار& liga_سیٹ_مفرد = c.گراف.vertices_[liga_سیٹ_مفرد_id].obj ();
      clear_virtual_links(c, liga_سیٹ_مفرد_id);
      TRY(جوڑو_virtual_link(c, liga_سیٹ_مفرد_id, coverage_مفرد_id));
      برائے (ثابت خودکار& l : liga_سیٹ_مفرد.real_links) {
        clear_virtual_links(c, l.objidx);
        TRY(جوڑو_virtual_link(c, l.objidx, coverage_مفرد_id));
      }

      گنتی += عدد_ligas;
    }

    c.گراف.vertices_[liga_subst_مفرد_id].shrink_بفر ((liga_subst_مفرد->ligature_سیٹ.len - liga_سیٹ_گنتی) * SmallTypes::سائز);
    liga_subst_مفرد->ligature_سیٹ.len = liga_سیٹ_گنتی;

    TRY (Coverage::filter_coverage (c,
                                    coverage_مفرد_id,
                                    liga_سیٹ_آغاز, liga_سیٹ_انتہا + 1));

    واپس Ok(liga_subst_مفرد_id);
  }

  گراف_نتیجہ_t<خالی> shrink (gsubgpos_گراف_context_t& c,
                               غیرعلامتی this_اشاریہ,
                               غیرعلامتی old_گنتی,
                               hb_سمتیہ_t<غیرعلامتی> liga_counts,
                               غیرعلامتی گنتی)
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
               "  Shrinking LigatureSubstFormat1 (%u) to [0, %u).",
               this_اشاریہ,
               گنتی);
    اگر (گنتی >= old_گنتی)
      واپس Ok();

    hb_سیٹ_t retained_indices;
    غیرعلامتی new_liga_سیٹ_گنتی = 0;
    برائے (غیرعلامتی i = 0; i < liga_counts.لمبائی; i++)
    {
      TRY_تفویض (خودکار liga_سیٹ, c.گراف.as_mutable_جدول<Ligature_سیٹ>(this_اشاریہ, &ligature_سیٹ[i]));

      // We need the virtual links to coverage removed from all descendants on this liga subst.
      // If any are left when we try to mutate the coverage table later it will be unnessecarily
      // duplicated. Code later on will re-add the virtual links as needed (via retained_indices).
      clear_virtual_links(c, liga_سیٹ.اشاریہ_2);
      retained_indices.جوڑو(liga_سیٹ.اشاریہ_2);

      TRY_تفویض (خودکار اشاریہ_مقصد_id, ligature_اشاریہ_مقصد_object_id (liga_سیٹ));

      برائے (غیرعلامتی i = 0; i < liga_سیٹ.جدول->ligature.len; i++) {
        غیرعلامتی liga_اشاریہ = اشاریہ_مقصد_id[i];
        اگر (liga_اشاریہ != HB_گراف_INVALID) {
          clear_virtual_links(c, liga_اشاریہ);
          retained_indices.جوڑو(liga_اشاریہ);
        }
      }

      غیرعلامتی عدد_ligas = liga_counts[i];
      اگر (عدد_ligas >= گنتی) {
        // drop the trailing liga's from this set and all subsequent liga sets
        غیرعلامتی عدد_ligas_مقصد_remove = عدد_ligas - گنتی;
        new_liga_سیٹ_گنتی = i + 1;
        c.گراف.vertices_[liga_سیٹ.اشاریہ_2].shrink_بفر (عدد_ligas_مقصد_remove * SmallTypes::سائز);
        liga_سیٹ.جدول->ligature.len = گنتی;
        توڑو;
      } ورنہ {
        گنتی -= عدد_ligas;
      }
    }

    // Adjust liga set array
    خودکار& this_راس = c.گراف.vertices_[this_اشاریہ];
    this_راس.shrink_بفر ((ligature_سیٹ.len - new_liga_سیٹ_گنتی) * SmallTypes::سائز);
    ligature_سیٹ.len = new_liga_سیٹ_گنتی;

    // Coverage matches the number of liga sets so rebuild as needed
    TRY_تفویض (غیرعلامتی coverage_idx, c.گراف.اشاریہ_for_انحراف (this_اشاریہ, &یہ->coverage));

    خودکار& coverage_v = c.گراف.vertices_[coverage_idx];
    غیرعلامتی coverage_سائز = coverage_v.جدول_سائز ();
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj ().سر_2;

    اگر (coverage_v.is_shared ())
    {
      TRY_تفویض (coverage_idx, c.گراف.remap_بچہ (this_اشاریہ, coverage_idx));
    }

    برائے (غیرعلامتی i : retained_indices.iter())
      TRY(جوڑو_virtual_link(c, i, coverage_idx));

    خودکار new_coverage =
        + hb_zip (coverage_جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<غیرعلامتی, غیرعلامتی> p) {
          واپس p.second < new_liga_سیٹ_گنتی;
        })
        | hb_map_retains_ترتیب_کاری (hb_پہلا)
        ;

    واپس Coverage::make_coverage (c, new_coverage, coverage_idx, coverage_سائز);
  }
};

ساخت LigatureSubst : عوامی OT::Layout::GSUB_impl::LigatureSubst
{

  گراف_نتیجہ_t<hb_سمتیہ_t<غیرعلامتی>> بانٹو_subtables (gsubgpos_گراف_context_t& c,
                                                         غیرعلامتی this_اشاریہ)
  {
    انتخاب (u.format.v) {
    حالت 1:
      hb_barrier ();
      واپس ((LigatureSubstFormat1*)(&u.format1))->بانٹو_subtables (c, this_اشاریہ);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 2: HB_FALLTHROUGH;
      // 24-bit LigatureSubst already has widened offsets; leave it unsplit.
#اگر_ختم
    طےشدہ:
      واپس Ok(hb_سمتیہ_t<غیرعلامتی> ());
    }
  }

  گراف_نتیجہ_t<خالی> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائز ();
    اگر (unlikely (راس_len < u.format.v.get_سائز ())) واپس Err(SANITIZE_FAILURE);
    hb_barrier ();

    انتخاب (u.format.v) {
    حالت 1:
      hb_barrier ();
      واپس ((LigatureSubstFormat1*)(&u.format1))->sanitize (راس);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 2:  HB_FALLTHROUGH;
#اگر_ختم
    طےشدہ:
      // We don't handle format 2 here.
      واپس Err(SANITIZE_FAILURE);
    }
  }
};

}

#اگر_ختم  // GRAPH_LIGATURE_GRAPH_HH
