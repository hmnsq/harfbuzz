
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

#اگر_تعریف_نشده گراف_LIGATURE_گراف_HH
#تعریف_کن گراف_LIGATURE_گراف_HH

#ضمن "graph.hh"
#ضمن "../OT/Layout/GSUB/LigatureSubst.hh"
#ضمن "../OT/Layout/GSUB/LigatureSubstFormat1.hh"
#ضمن "../OT/Layout/GSUB/LigatureSet.hh"
#ضمن "../hb-ot-dual.hh"
#ضمن <algorithm>
#ضمن <utility>

نطاق گراف {

بنية Ligature_مجموعه : عام OT::Layout::GSUB_impl::Ligature_مجموعه<SmallTypes>
{
  گراف_نتیجه_t<فراغ> sanitize (ثابت گراف_t::رأس_t& رأس) ثابت
  {
    size_t رأس_len = رأس.جدول_اندازه ();
    إذا (unlikely (رأس_len < OT::Layout::GSUB_impl::Ligature_مجموعه<SmallTypes>::min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    size_t مجموع_len = ligature.get_اندازه() + OT::Layout::GSUB_impl::Ligature_مجموعه<SmallTypes>::min_اندازه - ligature.len.get_اندازه();
    إذا (unlikely (رأس_len < مجموع_len)) {
      أرجع Err(SANITIZE_FAILURE);
    }
    أرجع Ok();
  }
};

بنية LigatureSubstFormat1 : عام OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>
{
  گراف_نتیجه_t<فراغ> sanitize (ثابت گراف_t::رأس_t& رأس) ثابت
  {
    size_t رأس_len = رأس.جدول_اندازه ();
    بیعلامت min_اندازه = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_اندازه;
    إذا (unlikely (رأس_len < min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (رأس_len < min_اندازه + ligature_مجموعه.get_اندازه() - ligature_مجموعه.len.get_اندازه()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتیجه_t<hb_وکتور_t<بیعلامت>> تقسیم_subtables (gsubgpos_گراف_context_t& c,
                                                         بیعلامت this_شاخص)
  {
    TRY_تخصیص_بده (خودکار تقسیم_points, compute_تقسیم_points(c, this_شاخص));
    إذا (!تقسیم_points)
      أرجع Ok(hb_وکتور_t<بیعلامت> ());

    TRY_تخصیص_بده (بیعلامت مجموع_ligas, مجموع_عدد_ligas(c, this_شاخص));
    TRY_تخصیص_بده (hb_وکتور_t<بیعلامت> counts, liga_counts(c, this_شاخص));
    تقسیم_context_t تقسیم_context {
      c,
      هذا,
      this_شاخص,
      مجموع_ligas,
      std::move (counts),
    };
    أرجع actuate_subtable_تقسیم<تقسیم_context_t> (تقسیم_context, تقسیم_points);
  }

 خاص:
  گراف_نتیجه_t<بیعلامت> مجموع_عدد_ligas(gsubgpos_گراف_context_t& c, بیعلامت this_شاخص) ثابت {
    بیعلامت مجموع = 0;
    لكل (بیعلامت i = 0; i < ligature_مجموعه.len; i++)
    {
      TRY_تخصیص_بده (خودکار liga_مجموعه, c.گراف.as_جدول<Ligature_مجموعه>(this_شاخص, &ligature_مجموعه[i]));
      مجموع += liga_مجموعه.جدول->ligature.len;
    }
    أرجع Ok(مجموع);
  }

  گراف_نتیجه_t<hb_وکتور_t<بیعلامت>> liga_counts(gsubgpos_گراف_context_t& c, بیعلامت this_شاخص) ثابت {
    hb_وکتور_t<بیعلامت> نتیجه;
    لكل (بیعلامت i = 0; i < ligature_مجموعه.len; i++)
    {
      TRY_تخصیص_بده (خودکار liga_مجموعه, c.گراف.as_جدول<Ligature_مجموعه>(this_شاخص, &ligature_مجموعه[i]));
      نتیجه.push(!liga_مجموعه.جدول ? 0 : liga_مجموعه.جدول->ligature.len);
    }
    أرجع گراف_نتیجه_t<hb_وکتور_t<بیعلامت>>::from(std::move(نتیجه), تخصیص_FAILURE);
  }

  قالب <گراف_t::رأس_mutability_t mutability>
  گراف_نتیجه_t<hb_وکتور_t<بیعلامت>> ligature_شاخص_مقصد_object_id(ثابت گراف_t::رأس_and_جدول_t<Ligature_مجموعه, mutability>& liga_مجموعه) ثابت {
    hb_وکتور_t<بیعلامت> map;
    إذا (!map.resize_exact(liga_مجموعه.جدول->ligature.len)) أرجع Err(تخصیص_FAILURE);

    لكل (بیعلامت i = 0; i < map.درازی; i++) {
      map[i] = HB_گراف_INVALID;
    }

    لكل (ثابت خودکار& l : liga_مجموعه.رأس->obj ().real_links) {
      إذا (l.position < 2) استمر;
      بیعلامت array_شاخص = (l.position - 2) / 2;
      map[array_شاخص] = l.objidx;
    }
    أرجع map;
  }

  گراف_نتیجه_t<hb_وکتور_t<بیعلامت>> compute_تقسیم_points(gsubgpos_گراف_context_t& c,
                                                             بیعلامت this_شاخص) ثابت
  {
    // For ligature subst coverage is always packed last, and as a result is where an overflow
    // will happen if there is one, so we can check the estimate length of the
    // LigatureSubstFormat1 -> Coverage offset length which is the sum of all data in the
    // retained sub graph except for the coverage table itself.
    ثابت بیعلامت پایه_اندازه = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_اندازه;
    بیعلامت accumulated = پایه_اندازه;

    بیعلامت ligature_شاخص = 0;
    hb_وکتور_t<بیعلامت> تقسیم_points;
    لكل (بیعلامت i = 0; i < ligature_مجموعه.len; i++)
    {
      accumulated += OT::HBUINT16::static_اندازه; // for ligature set offset
      accumulated += OT::Layout::GSUB_impl::Ligature_مجموعه<SmallTypes>::min_اندازه; // for ligature set table

      TRY_تخصیص_بده (خودکار liga_مجموعه, c.گراف.as_جدول<Ligature_مجموعه>(this_شاخص, &ligature_مجموعه[i]));

      // Finding the object id associated with an array index is O(n)
      // so to avoid O(n^2), precompute the mapping by scanning through
      // all links
      TRY_تخصیص_بده (خودکار شاخص_مقصد_id, ligature_شاخص_مقصد_object_id (liga_مجموعه));

      لكل (بیعلامت j = 0; j < liga_مجموعه.جدول->ligature.len; j++)
      {
        ثابت بیعلامت liga_id = شاخص_مقصد_id[j];
        إذا (liga_id == HB_گراف_INVALID) استمر; // no outgoing link, ignore
        ثابت بیعلامت liga_اندازه = c.گراف.vertices_[liga_id].جدول_اندازه ();

        accumulated += OT::HBUINT16::static_اندازه; // for ligature offset
        accumulated += liga_اندازه; // for the ligature table

        إذا (accumulated >= (1 << 16))
        {
          تقسیم_points.push(ligature_شاخص);
          // We're going to split such that the current ligature will be in the new sub table.
          // That means we'll have one ligature subst (base_base), one ligature set, and one liga table
          accumulated = پایه_اندازه + // for liga subst subtable
            (OT::HBUINT16::static_اندازه * 2) + // for liga set and liga offset
            OT::Layout::GSUB_impl::Ligature_مجموعه<SmallTypes>::min_اندازه + // for liga set subtable
            liga_اندازه; // for liga sub table
        }

        ligature_شاخص++;
      }
    }

    أرجع تقسیم_points;
  }

  بنية تقسیم_context_t
  {
    gsubgpos_گراف_context_t& c;
    LigatureSubstFormat1* thiz;
    بیعلامت this_شاخص;
    بیعلامت original_شمار;
    hb_وکتور_t<بیعلامت> liga_counts;

    بیعلامت original_شمار_2 ()
    {
      أرجع original_شمار;
    }

    گراف_نتیجه_t<بیعلامت> clone_range (بیعلامت شروع, بیعلامت انتها)
    {
      أرجع thiz->clone_range (c, this_شاخص, liga_counts, شروع, انتها);
    }

    گراف_نتیجه_t<فراغ> shrink (بیعلامت شمار)
    {
      أرجع thiz->shrink (c, this_شاخص, original_شمار_2(), liga_counts, شمار);
    }
  };

  گراف_نتیجه_t<hb_pair_t<بیعلامت, Ligature_مجموعه*>> new_liga_مجموعه(gsubgpos_گراف_context_t& c, بیعلامت شمار) ثابت {
    بیعلامت اول_اندازه = OT::Layout::GSUB_impl::Ligature_مجموعه<SmallTypes>::min_اندازه
                          + شمار * SmallTypes::اندازه;

    TRY_تخصیص_بده (بیعلامت اول_id, c.create_گره (اول_اندازه));

    Ligature_مجموعه* اول = (Ligature_مجموعه*) c.گراف.object (اول_id).سر_2;
    اول->ligature.len = شمار;
    أرجع Ok(hb_pair(اول_id, اول));
  }

  فراغ clear_virtual_links (gsubgpos_گراف_context_t& c, بیعلامت گره_شاخص) ثابت
  {
    خودکار& v  = c.گراف.vertices_[گره_شاخص];
    لكل (ثابت خودکار& l : v.obj ().virtual_links)
    {
      خودکار& فرزند = c.گراف.vertices_[l.objidx];
      فرزند.remove_والد(گره_شاخص);
    }
    v.clear_virtual_links ();
  }

  گراف_نتیجه_t<فراغ> جمع_کن_virtual_link(gsubgpos_گراف_context_t& c, بیعلامت from, بیعلامت to) ثابت {
    إذا (unlikely (from >= c.گراف.vertices_.درازی || to >= c.گراف.vertices_.درازی)) أرجع Err(بیرون_از_BOUNDS);
    TRY(c.گراف.vertices_[to].جمع_کن_والد (from, درست));
    TRY(c.گراف.vertices_[from].جمع_کن_virtual_link (to));
    أرجع Ok();
  }

  hb_pair_t<بیعلامت, بیعلامت> current_liga_مجموعه_bounds (gsubgpos_گراف_context_t& c,
                                                         بیعلامت liga_مجموعه_شاخص,
                                                         ثابت hb_serialize_context_t::object_t& liga_مجموعه) ثابت
  {
    // Finds the actual liga indices present in the liga set currently. Takes
    // into account those that have been removed by processing.
    بیعلامت min_شاخص = HB_گراف_INVALID;
    بیعلامت max_شاخص = 0;
    لكل (ثابت خودکار& l : liga_مجموعه.real_links) {
      إذا (l.position < 2) استمر;

      بیعلامت liga_شاخص = (l.position - 2) / 2;
      min_شاخص = hb_min(min_شاخص, liga_شاخص);
      max_شاخص = hb_max(max_شاخص, liga_شاخص);
    }
    أرجع hb_pair(min_شاخص, max_شاخص + 1);
  }

  فراغ compact_liga_مجموعه (gsubgpos_گراف_context_t& c, Ligature_مجموعه* جدول, گراف_t::رأس_t* v) ثابت
  {
    ثابت خودکار& obj = v->obj ();
    إذا (جدول->ligature.len <= obj.real_links.درازی) أرجع;

    // compact the remaining linked liga offsets into a continous array and shrink the node as needed.
    بیعلامت مقصد_remove = جدول->ligature.len - obj.real_links.درازی;
    بیعلامت new_position = SmallTypes::اندازه;
    v->مرتب_سازی_real_links (); // for this to work we need to process links in order of position.
    لكل (خودکار& l : v->real_links_writer ())
    {
      l.position = new_position;
      new_position += SmallTypes::اندازه;
    }

    جدول->ligature.len = obj.real_links.درازی;
    v->shrink_میانگیر (مقصد_remove * SmallTypes::اندازه);
  }

  گراف_نتیجه_t<بیعلامت> clone_range (gsubgpos_گراف_context_t& c,
                                        بیعلامت this_شاخص,
                                        hb_وکتور_t<بیعلامت> liga_counts,
                                        بیعلامت شروع, بیعلامت انتها) ثابت
  {
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
               "  Cloning LigatureSubstFormat1 (%u) range [%u, %u).", this_شاخص, شروع, انتها);

    // Create an oversized new liga subst, we'll adjust the size down later. We don't know
    // the final size until we process it but we also need it to exist while we're processing
    // so that nodes can be moved to it as needed.
    بیعلامت اول_اندازه = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_اندازه
                          + ligature_مجموعه.get_اندازه() - ligature_مجموعه.len.get_اندازه();

    TRY_تخصیص_بده (بیعلامت liga_subst_اول_id, c.create_گره (اول_اندازه));

    LigatureSubstFormat1* liga_subst_اول = (LigatureSubstFormat1*) c.گراف.object (liga_subst_اول_id).سر_2;
    liga_subst_اول->format = هذا->format;
    liga_subst_اول->ligature_مجموعه.len = هذا->ligature_مجموعه.len;

    // Create a place holder coverage prime id since we need to add virtual links to it while
    // generating liga and liga sets. Afterwards it will be updated to have the correct coverage.
    TRY_تخصیص_بده (بیعلامت coverage_id, c.گراف.شاخص_for_جابه_جایی (this_شاخص, &coverage));
    TRY_تخصیص_بده (بیعلامت coverage_اول_id, c.گراف.duplicate(coverage_id));
    خودکار& coverage_اول_رأس = c.گراف.vertices_[coverage_اول_id];

    TRY(c.گراف.vertices_[liga_subst_اول_id].جمع_کن_real_link (SmallTypes::اندازه, coverage_اول_id, 2));
    TRY(coverage_اول_رأس.جمع_کن_والد (liga_subst_اول_id, خطأ));

    // Locate all liga sets with ligas between start and end.
    // Clone or move them as needed.
    بیعلامت شمار = 0;
    بیعلامت liga_مجموعه_شمار = 0;
    بیعلامت liga_مجموعه_شروع = HB_گراف_INVALID;
    بیعلامت liga_مجموعه_انتها = 0; // inclusive
    لكل (بیعلامت i = 0; i < liga_counts.درازی; i++)
    {
      بیعلامت رقم_ligas = liga_counts[i];

      بیعلامت current_شروع = شمار;
      بیعلامت current_انتها = شمار + رقم_ligas;

      إذا (current_شروع >= انتها || شروع >= current_انتها) {
        // No intersection, so just skip
        شمار += رقم_ligas;
        استمر;
      }

      TRY_تخصیص_بده (خودکار liga_مجموعه, c.گراف.as_mutable_جدول<Ligature_مجموعه>(this_شاخص, &ligature_مجموعه[i]));

      بیعلامت liga_مجموعه_شاخص = liga_مجموعه.شاخص_2;
      // Bounds may need to be adjusted if some ligas have been previously removed.
      hb_pair_t<بیعلامت, بیعلامت> liga_bounds = current_liga_مجموعه_bounds(c, liga_مجموعه_شاخص, liga_مجموعه.رأس->obj ());
      current_شروع = hb_max(شمار + liga_bounds.اولی, current_شروع);
      current_انتها = hb_min(شمار + liga_bounds.second, current_انتها);

      بیعلامت liga_مجموعه_اول_id;
      إذا (current_شروع >= شروع && current_انتها <= انتها) {
        // This liga set is fully contined within [start, end)
        // We can move the entire ligaset to the new liga subset object.
        liga_مجموعه_انتها = i;
        إذا (i < liga_مجموعه_شروع) liga_مجموعه_شروع = i;
        TRY_تخصیص_بده (liga_مجموعه_اول_id, c.گراف.move_فرزند<> (this_شاخص,
                              &ligature_مجموعه[i],
                              liga_subst_اول_id,
                              &liga_subst_اول->ligature_مجموعه[liga_مجموعه_شمار++]));
        compact_liga_مجموعه (c, liga_مجموعه.جدول, liga_مجموعه.رأس);
      }
      وإلا
      {
        // This liga set partially overlaps [start, end). We'll need to create
        // a new liga set sub table and move the intersecting ligas to it.
        بیعلامت شروع_شاخص = hb_max(شروع, current_شروع) - شمار;
        بیعلامت انتها_شاخص = hb_min(انتها, current_انتها) - شمار;
        بیعلامت liga_شمار = انتها_شاخص - شروع_شاخص;
        TRY_تخصیص_بده (خودکار نتیجه, new_liga_مجموعه(c, liga_شمار));
        liga_مجموعه_اول_id = نتیجه.اولی;

        TRY (c.گراف.move_children<OT::Offset16>(
          liga_مجموعه_شاخص,
          2 + شروع_شاخص * 2,
          2 + انتها_شاخص * 2,
          liga_مجموعه_اول_id,
          2));

        liga_مجموعه_انتها = i;
        إذا (i < liga_مجموعه_شروع) liga_مجموعه_شروع = i;
        TRY (c.گراف.جمع_کن_link(&liga_subst_اول->ligature_مجموعه[liga_مجموعه_شمار++], liga_subst_اول_id, liga_مجموعه_اول_id));
      }

      // The new liga and all children set needs to have a virtual link to the new coverage table:
      ثابت خودکار& liga_مجموعه_اول = c.گراف.vertices_[liga_مجموعه_اول_id].obj ();
      clear_virtual_links(c, liga_مجموعه_اول_id);
      TRY(جمع_کن_virtual_link(c, liga_مجموعه_اول_id, coverage_اول_id));
      لكل (ثابت خودکار& l : liga_مجموعه_اول.real_links) {
        clear_virtual_links(c, l.objidx);
        TRY(جمع_کن_virtual_link(c, l.objidx, coverage_اول_id));
      }

      شمار += رقم_ligas;
    }

    c.گراف.vertices_[liga_subst_اول_id].shrink_میانگیر ((liga_subst_اول->ligature_مجموعه.len - liga_مجموعه_شمار) * SmallTypes::اندازه);
    liga_subst_اول->ligature_مجموعه.len = liga_مجموعه_شمار;

    TRY (Coverage::filter_coverage (c,
                                    coverage_اول_id,
                                    liga_مجموعه_شروع, liga_مجموعه_انتها + 1));

    أرجع Ok(liga_subst_اول_id);
  }

  گراف_نتیجه_t<فراغ> shrink (gsubgpos_گراف_context_t& c,
                               بیعلامت this_شاخص,
                               بیعلامت old_شمار,
                               hb_وکتور_t<بیعلامت> liga_counts,
                               بیعلامت شمار)
  {
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
               "  Shrinking LigatureSubstFormat1 (%u) to [0, %u).",
               this_شاخص,
               شمار);
    إذا (شمار >= old_شمار)
      أرجع Ok();

    hb_مجموعه_t retained_indices;
    بیعلامت new_liga_مجموعه_شمار = 0;
    لكل (بیعلامت i = 0; i < liga_counts.درازی; i++)
    {
      TRY_تخصیص_بده (خودکار liga_مجموعه, c.گراف.as_mutable_جدول<Ligature_مجموعه>(this_شاخص, &ligature_مجموعه[i]));

      // We need the virtual links to coverage removed from all descendants on this liga subst.
      // If any are left when we try to mutate the coverage table later it will be unnessecarily
      // duplicated. Code later on will re-add the virtual links as needed (via retained_indices).
      clear_virtual_links(c, liga_مجموعه.شاخص_2);
      retained_indices.جمع_کن(liga_مجموعه.شاخص_2);

      TRY_تخصیص_بده (خودکار شاخص_مقصد_id, ligature_شاخص_مقصد_object_id (liga_مجموعه));

      لكل (بیعلامت i = 0; i < liga_مجموعه.جدول->ligature.len; i++) {
        بیعلامت liga_شاخص = شاخص_مقصد_id[i];
        إذا (liga_شاخص != HB_گراف_INVALID) {
          clear_virtual_links(c, liga_شاخص);
          retained_indices.جمع_کن(liga_شاخص);
        }
      }

      بیعلامت رقم_ligas = liga_counts[i];
      إذا (رقم_ligas >= شمار) {
        // drop the trailing liga's from this set and all subsequent liga sets
        بیعلامت رقم_ligas_مقصد_remove = رقم_ligas - شمار;
        new_liga_مجموعه_شمار = i + 1;
        c.گراف.vertices_[liga_مجموعه.شاخص_2].shrink_میانگیر (رقم_ligas_مقصد_remove * SmallTypes::اندازه);
        liga_مجموعه.جدول->ligature.len = شمار;
        اكسر;
      } وإلا {
        شمار -= رقم_ligas;
      }
    }

    // Adjust liga set array
    خودکار& this_رأس = c.گراف.vertices_[this_شاخص];
    this_رأس.shrink_میانگیر ((ligature_مجموعه.len - new_liga_مجموعه_شمار) * SmallTypes::اندازه);
    ligature_مجموعه.len = new_liga_مجموعه_شمار;

    // Coverage matches the number of liga sets so rebuild as needed
    TRY_تخصیص_بده (بیعلامت coverage_idx, c.گراف.شاخص_for_جابه_جایی (this_شاخص, &هذا->coverage));

    خودکار& coverage_v = c.گراف.vertices_[coverage_idx];
    بیعلامت coverage_اندازه = coverage_v.جدول_اندازه ();
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj ().سر_2;

    إذا (coverage_v.is_shared ())
    {
      TRY_تخصیص_بده (coverage_idx, c.گراف.remap_فرزند (this_شاخص, coverage_idx));
    }

    لكل (بیعلامت i : retained_indices.iter())
      TRY(جمع_کن_virtual_link(c, i, coverage_idx));

    خودکار new_coverage =
        + hb_zip (coverage_جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<بیعلامت, بیعلامت> p) {
          أرجع p.second < new_liga_مجموعه_شمار;
        })
        | hb_map_retains_ترتیبدهی (hb_اولی)
        ;

    أرجع Coverage::make_coverage (c, new_coverage, coverage_idx, coverage_اندازه);
  }
};

بنية LigatureSubst : عام OT::Layout::GSUB_impl::LigatureSubst
{

  گراف_نتیجه_t<hb_وکتور_t<بیعلامت>> تقسیم_subtables (gsubgpos_گراف_context_t& c,
                                                         بیعلامت this_شاخص)
  {
    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((LigatureSubstFormat1*)(&u.format1))->تقسیم_subtables (c, this_شاخص);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 2: HB_FALLTHROUGH;
      // 24-bit LigatureSubst already has widened offsets; leave it unsplit.
#پایان_اگر
    افتراضي:
      أرجع Ok(hb_وکتور_t<بیعلامت> ());
    }
  }

  گراف_نتیجه_t<فراغ> sanitize (ثابت گراف_t::رأس_t& رأس) ثابت
  {
    size_t رأس_len = رأس.جدول_اندازه ();
    إذا (unlikely (رأس_len < u.format.v.get_اندازه ())) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((LigatureSubstFormat1*)(&u.format1))->sanitize (رأس);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 2:  HB_FALLTHROUGH;
#پایان_اگر
    افتراضي:
      // We don't handle format 2 here.
      أرجع Err(SANITIZE_FAILURE);
    }
  }
};

}

#پایان_اگر  // GRAPH_LIGATURE_GRAPH_HH
