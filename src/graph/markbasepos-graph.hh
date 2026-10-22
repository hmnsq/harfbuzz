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

#که_نه_تعریف_شوی ګراف_MARKBASEPOS_ګراف_HH
#تعریف_کړه ګراف_MARKBASEPOS_ګراف_HH

#ضمن "split-helpers.hh"
#ضمن "coverage-graph.hh"
#ضمن "../OT/Layout/GPOS/MarkBasePos.hh"
#ضمن "../OT/Layout/GPOS/PosLookupSubTable.hh"

نطاق ګراف {

بنية Anchor_ماتریکس : عام OT::Layout::GPOS_impl::Anchor_ماتریکس<SmallTypes>
{
  ګراف_پایله_t<فراغ> sanitize (ثابت ګراف_t::راس_t& راس, بینښه class_شمېر) ثابت
  {
    size_t راس_len = راس.جدول_اندازه();
    إذا (unlikely (راس_len < Anchor_ماتریکس::min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (راس_len < Anchor_ماتریکس::min_اندازه +
                  OT::Offset16::static_اندازه * class_شمېر * هذا->لیکې))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  ګراف_پایله_t<فراغ> shrink (gsubgpos_ګراف_context_t& c,
                               بینښه this_شاخص,
                               بینښه old_class_شمېر,
                               بینښه new_class_شمېر)
  {
    إذا (unlikely (new_class_شمېر >= old_class_شمېر)) أرجع Err(INVALID_دلیل);
    خپلکاری& v = c.ګراف.vertices_[this_شاخص];
    ثابت خپلکاری& o = v.obj ();
    بینښه بنسټ_شمېر = لیکې;
    v.سېټ_بفر (o.سر_2,
      o.سر_2 +
      Anchor_ماتریکس::min_اندازه +
      OT::Offset16::static_اندازه * بنسټ_شمېر * new_class_شمېر);

    // Reposition links into the new indexing scheme.
    لكل (خپلکاری& link : v.real_links_writer ())
    {
      بینښه شاخص_2 = (link.position - 2) / 2;
      بینښه بنسټ = شاخص_2 / old_class_شمېر;
      بینښه klass = شاخص_2 % old_class_شمېر;
      إذا (unlikely (klass >= new_class_شمېر))
        // should have already been removed
        أرجع Err(INVALID_دلیل);

      بینښه new_شاخص = بنسټ * new_class_شمېر + klass;

      link.position = (حرف*) &(هذا->ماتریکس_Z[new_شاخص]) - (حرف*) هذا;
    }

    أرجع Ok();
  }

  ګراف_پایله_t<بینښه> clone (gsubgpos_ګراف_context_t& c,
                                  بینښه this_شاخص,
                                  بینښه پیل,
                                  بینښه پای_ټکی,
                                  بینښه class_شمېر)
  {
    بینښه بنسټ_شمېر = لیکې;
    بینښه new_class_شمېر = پای_ټکی - پیل;
    بینښه اندازه = Anchor_ماتریکس::min_اندازه +
                    OT::Offset16::static_اندازه * new_class_شمېر * لیکې;
    TRY_ټاکل (بینښه اولي_id, c.create_غوټه (اندازه));
    Anchor_ماتریکس* اولي = (Anchor_ماتریکس*) c.ګراف.object (اولي_id).سر_2;
    اولي->لیکې = بنسټ_شمېر;

    خپلکاری& v = c.ګراف.vertices_[this_شاخص];
    ثابت خپلکاری& o = v.obj ();

    صحيح عدد_links = o.real_links.اوږدتیا;
    لكل (صحيح i = 0; i < عدد_links; i++)
    {
      ثابت خپلکاری& link = o.real_links[i];
      بینښه old_شاخص = (link.position - 2) / OT::Offset16::static_اندازه;
      بینښه klass = old_شاخص % class_شمېر;
      إذا (klass < پیل || klass >= پای_ټکی) استمر;

      بینښه بنسټ = old_شاخص / class_شمېر;
      بینښه new_klass = klass - پیل;
      بینښه new_شاخص = بنسټ * new_class_شمېر + new_klass;


      بینښه ماشوم_idx = link.objidx;
      TRY (c.ګراف.جمع_کړه_link (&(اولي->ماتریکس_Z[new_شاخص]),
                             اولي_id,
                             ماشوم_idx));

      خپلکاری& ماشوم = c.ګراف.vertices_[ماشوم_idx];
      ماشوم.remove_پلار (this_شاخص);

      v.remove_real_link_unordered (i);
      عدد_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.ترتیبول_real_links ();

    أرجع Ok(اولي_id);
  }
};

بنية MarkArray : عام OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  ګراف_پایله_t<فراغ> sanitize (ثابت ګراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_اندازه();
    بینښه min_اندازه = MarkArray::min_اندازه;
    إذا (unlikely (راس_len < min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (راس_len < get_اندازه ())) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  ګراف_پایله_t<فراغ> shrink (gsubgpos_ګراف_context_t& c,
                               ثابت hb_hashmap_t<بینښه, بینښه>& mark_array_links,
                               بینښه this_شاخص,
                               بینښه new_class_شمېر)
  {
    خپلکاری& v = c.ګراف.vertices_[this_شاخص];
    ثابت خپلکاری& o = v.obj ();
    لكل (ثابت خپلکاری& link : o.real_links)
      c.ګراف.vertices_[link.objidx].remove_پلار (this_شاخص);
    v.clear_real_links ();

    بینښه new_شاخص = 0;
    لكل (ثابت خپلکاری& ریکارډ : هذا->iter ())
    {
      بینښه klass = ریکارډ.klass;
      إذا (klass >= new_class_شمېر) استمر;

      (*هذا)[new_شاخص].klass = klass;
      بینښه position = (حرف*) &ریکارډ.markAnchor - (حرف*) هذا;
      بینښه* objidx;
      إذا (!mark_array_links.has (position, &objidx))
      {
        new_شاخص++;
        استمر;
      }

      TRY (c.ګراف.جمع_کړه_link (&(*هذا)[new_شاخص].markAnchor, this_شاخص, *objidx));
      new_شاخص++;
    }

    هذا->len = new_شاخص;
    v.سېټ_بفر (o.سر_2, o.سر_2 + MarkArray::min_اندازه +
                  OT::Layout::GPOS_impl::Mark_ریکارډ<SmallTypes>::static_اندازه * new_شاخص);

    أرجع Ok();
  }

  ګراف_پایله_t<بینښه> clone (gsubgpos_ګراف_context_t& c,
                                  بینښه this_شاخص,
                                  ثابت hb_hashmap_t<بینښه, بینښه>& pos_منزل_شاخص,
                                  hb_سېټ_t& marks,
                                  بینښه پیل_class)
  {
    بینښه اندازه = MarkArray::min_اندازه +
                    OT::Layout::GPOS_impl::Mark_ریکارډ<SmallTypes>::static_اندازه *
                    marks.get_population ();
    TRY_ټاکل (بینښه اولي_id, c.create_غوټه (اندازه));
    MarkArray* اولي = (MarkArray*) c.ګراف.object (اولي_id).سر_2;
    اولي->len = marks.get_population ();


    بینښه i = 0;
    لكل (hb_codepoint_t mark : marks)
    {
      (*اولي)[i].klass = (*هذا)[mark].klass - پیل_class;
      بینښه دننه_واټن_pos = (حرف*) &((*هذا)[mark].markAnchor) - (حرف*) هذا;
      بینښه* anchor_شاخص;
      إذا (pos_منزل_شاخص.has (دننه_واټن_pos, &anchor_شاخص))
        TRY (c.ګراف.move_ماشوم (this_شاخص,
                                 &((*هذا)[mark].markAnchor),
                                 اولي_id,
                                 &((*اولي)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.ګراف.vertices_[this_شاخص].ترتیبول_real_links ();

    أرجع Ok(اولي_id);
  }
};

بنية Mark_بنسټ_Pos_Format1 : عام OT::Layout::GPOS_impl::Mark_بنسټ_Pos_Format1_2<SmallTypes>
{
  ګراف_پایله_t<فراغ> sanitize (ثابت ګراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_اندازه ();
    إذا (unlikely (راس_len < Mark_بنسټ_Pos_Format1::static_اندازه))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  ګراف_پایله_t<hb_ویکټور_t<بینښه>> وېشل_subtables (gsubgpos_ګراف_context_t& c,
                                                         بینښه this_شاخص)
  {
    hb_سېټ_t لیدل_شوی;

    TRY_ټاکل (ثابت بینښه بنسټ_coverage_id, c.ګراف.شاخص_for_دننه_واټن (this_شاخص, &بنسټ_Coverage));

    ثابت بینښه بنسټ_اندازه =
        OT::Layout::GPOS_impl::Mark_بنسټ_Pos_Format1_2<SmallTypes>::min_اندازه +
        MarkArray::min_اندازه +
        Anchor_ماتریکس::min_اندازه +
        c.ګراف.vertices_[بنسټ_coverage_id].جدول_اندازه ();

    TRY_ټاکل (hb_ویکټور_t<class_info_t> class_منزل_info, get_class_info (c, this_شاخص));

    بینښه class_شمېر = class_شمېر_2;
    TRY_ټاکل (خپلکاری بنسټ_array, c.ګراف.as_جدول<Anchor_ماتریکس> (this_شاخص,
                                                          &بنسټ_Array,
                                                          class_شمېر));
    بینښه بنسټ_شمېر = بنسټ_array.جدول->لیکې;

    بینښه partial_coverage_اندازه = 4;
    بینښه accumulated = بنسټ_اندازه;
    hb_ویکټور_t<بینښه> وېشل_points;

    لكل (بینښه klass = 0; klass < class_شمېر; klass++)
    {
      class_info_t& info = class_منزل_info[klass];
      partial_coverage_اندازه += OT::HBUINT16::static_اندازه * info.marks.get_population ();
      بینښه accumulated_delta =
          OT::Layout::GPOS_impl::Mark_ریکارډ<SmallTypes>::static_اندازه * info.marks.get_population () +
          OT::Offset16::static_اندازه * بنسټ_شمېر;

      لكل (بینښه objidx : info.ماشوم_indices)
        accumulated_delta += c.ګراف.پیدا_کړه_subgraph_اندازه (objidx, لیدل_شوی).ارزښت_or (0);

      accumulated += accumulated_delta;
      بینښه ټول = accumulated + partial_coverage_اندازه;

      إذا (ټول >= (1 << 16))
      {
        وېشل_points.push (klass);
        TRY (ګراف_پایله_t<فراغ>::from (وېشل_points, ویش_FAILURE));
        accumulated = بنسټ_اندازه + accumulated_delta;
        partial_coverage_اندازه = 4 + OT::HBUINT16::static_اندازه * info.marks.get_population ();
        لیدل_شوی.clear (); // node sharing isn't allowed between splits.
      }
    }


    TRY_ټاکل (ثابت بینښه mark_array_id, c.ګراف.شاخص_for_دننه_واټن (this_شاخص, &markArray));
    TRY_ټاکل (خپلکاری mark_array_links, c.ګراف.vertices_[mark_array_id].position_منزل_شاخص_map ());

    وېشل_context_t وېشل_context {
      c,
      هذا,
      this_شاخص,
      std::move (class_منزل_info),
      std::move (mark_array_links),
    };

    أرجع actuate_subtable_وېشل<وېشل_context_t> (وېشل_context, وېشل_points);
  }

 خاص:

  بنية class_info_t {
    hb_سېټ_t marks;
    hb_ویکټور_t<بینښه> ماشوم_indices;
  };

  بنية وېشل_context_t {
    gsubgpos_ګراف_context_t& c;
    Mark_بنسټ_Pos_Format1* thiz;
    بینښه this_شاخص;
    hb_ویکټور_t<class_info_t> class_منزل_info;
    hb_hashmap_t<بینښه, بینښه> mark_array_links;

    hb_سېټ_t marks_for (بینښه پیل, بینښه پای_ټکی)
    {
      hb_سېټ_t marks;
      لكل (بینښه klass = پیل; klass < پای_ټکی; klass++)
      {
        + class_منزل_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      أرجع marks;
    }

    بینښه original_شمېر_2 ()
    {
      أرجع thiz->class_شمېر_2;
    }

    ګراف_پایله_t<بینښه> clone_range (بینښه پیل, بینښه پای_ټکی)
    {
      أرجع thiz->clone_range (*هذا, هذا->this_شاخص, پیل, پای_ټکی);
    }

    ګراف_پایله_t<فراغ> shrink (بینښه شمېر)
    {
      أرجع thiz->shrink (*هذا, هذا->this_شاخص, شمېر);
    }
  };

  ګراف_پایله_t<hb_ویکټور_t<class_info_t>> get_class_info (gsubgpos_ګراف_context_t& c,
                                                            بینښه this_شاخص)
  {
    hb_ویکټور_t<class_info_t> class_منزل_info;

    بینښه class_شمېر = class_شمېر_2;
    إذا (!class_شمېر) أرجع class_منزل_info;

    إذا (unlikely (!class_منزل_info.resize (class_شمېر)))
      أرجع Err(ویش_FAILURE);

    TRY_ټاکل (خپلکاری mark_array, c.ګراف.as_جدول<MarkArray> (this_شاخص, &markArray));

    بینښه mark_شمېر = mark_array.جدول->len;
    لكل (بینښه mark = 0; mark < mark_شمېر; mark++)
    {
      بینښه klass = (*mark_array.جدول)[mark].get_class ();
      إذا (klass >= class_شمېر) استمر;
      class_منزل_info[klass].marks.جمع_کړه (mark);
    }

    لكل (ثابت خپلکاری& link : mark_array.راس->obj ().real_links)
    {
      بینښه mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::Mark_ریکارډ<SmallTypes>::static_اندازه;
      بینښه klass = (*mark_array.جدول)[mark].get_class ();
      إذا (klass >= class_شمېر) استمر;
      class_منزل_info[klass].ماشوم_indices.push (link.objidx);
    }

    TRY_ټاکل (بینښه بنسټ_array_id,
         c.ګراف.شاخص_for_دننه_واټن (this_شاخص, &بنسټ_Array));

    خپلکاری& بنسټ_array_v = c.ګراف.vertices_[بنسټ_array_id];

    لكل (ثابت خپلکاری& link : بنسټ_array_v.obj ().real_links)
    {
      بینښه شاخص_2 = (link.position - 2) / OT::Offset16::static_اندازه;
      بینښه klass = شاخص_2 % class_شمېر;
      class_منزل_info[klass].ماشوم_indices.push (link.objidx);
    }

    أرجع class_منزل_info;
  }

  ګراف_پایله_t<فراغ> shrink (وېشل_context_t& sc,
                               بینښه this_شاخص,
                               بینښه شمېر)
  {
    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_شاخص,
               شمېر);

    بینښه old_شمېر = class_شمېر_2;
    إذا (شمېر >= old_شمېر)
      أرجع Ok();

    class_شمېر_2 = شمېر;

    TRY_ټاکل (خپلکاری mark_coverage, sc.c.ګراف.as_mutable_جدول<Coverage> (this_شاخص,
                                                                    &markCoverage));

    hb_سېټ_t marks = sc.marks_for (0, شمېر);
    خپلکاری new_coverage =
        + hb_enumerate (mark_coverage.جدول->iter ())
        | hb_filter (marks, hb_لومړی)
        | hb_map_retains_ترتیبونه (hb_دوهم)
        ;
    TRY (Coverage::make_coverage (sc.c, + new_coverage,
                                  mark_coverage.شاخص_2,
                                  4 + 2 * marks.get_population ()));


    TRY_ټاکل (خپلکاری بنسټ_array, sc.c.ګراف.as_mutable_جدول<Anchor_ماتریکس> (this_شاخص,
                                                                     &بنسټ_Array,
                                                                     old_شمېر));

    TRY (بنسټ_array.جدول->shrink (sc.c,
                                   بنسټ_array.شاخص_2,
                                   old_شمېر,
                                   شمېر));

    TRY_ټاکل (خپلکاری mark_array, sc.c.ګراف.as_mutable_جدول<MarkArray> (this_شاخص,
                                                                  &markArray));

    TRY (mark_array.جدول->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.شاخص_2,
                                   شمېر));

    أرجع Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  ګراف_پایله_t<بینښه> clone_range (وېشل_context_t& sc,
                                        بینښه this_شاخص,
                                        بینښه پیل, بینښه پای_ټکی) ثابت
  {
    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_شاخص, پیل, پای_ټکی);

    ګراف_t& ګراف = sc.c.ګراف;
    بینښه اولي_اندازه = OT::Layout::GPOS_impl::Mark_بنسټ_Pos_Format1_2<SmallTypes>::static_اندازه;

    TRY_ټاکل (بینښه اولي_id, sc.c.create_غوټه (اولي_اندازه));

    Mark_بنسټ_Pos_Format1* اولي = (Mark_بنسټ_Pos_Format1*) ګراف.object (اولي_id).سر_2;
    اولي->format = هذا->format;
    بینښه new_class_شمېر = پای_ټکی - پیل;
    اولي->class_شمېر_2 = new_class_شمېر;

    TRY_ټاکل (بینښه بنسټ_coverage_id,
         ګراف.شاخص_for_دننه_واټن (sc.this_شاخص, &بنسټ_Coverage));

    TRY (ګراف.جمع_کړه_link (&(اولي->بنسټ_Coverage), اولي_id, بنسټ_coverage_id));

    TRY_ټاکل (خپلکاری mark_coverage, sc.c.ګراف.as_جدول<Coverage> (this_شاخص,
                                                            &markCoverage));

    hb_سېټ_t marks = sc.marks_for (پیل, پای_ټکی);
    خپلکاری new_coverage =
        + hb_enumerate (mark_coverage.جدول->iter ())
        | hb_filter (marks, hb_لومړی)
        | hb_map_retains_ترتیبونه (hb_دوهم)
        ;
    TRY (Coverage::جمع_کړه_coverage (sc.c,
                                 اولي_id,
                                 2,
                                 + new_coverage,
                                 marks.get_population () * 2 + 4));

    TRY_ټاکل (خپلکاری mark_array,
         ګراف.as_mutable_جدول <MarkArray> (sc.this_شاخص, &markArray));

    TRY_ټاکل (بینښه new_mark_array,
         mark_array.جدول->clone (sc.c,
                                  mark_array.شاخص_2,
                                  sc.mark_array_links,
                                  marks,
                                  پیل));
    TRY (ګراف.جمع_کړه_link (&(اولي->markArray), اولي_id, new_mark_array));

    بینښه class_شمېر = class_شمېر_2;
    TRY_ټاکل (خپلکاری بنسټ_array,
         ګراف.as_mutable_جدول<Anchor_ماتریکس> (sc.this_شاخص, &بنسټ_Array, class_شمېر));

    TRY_ټاکل (بینښه new_بنسټ_array,
         بنسټ_array.جدول->clone (sc.c,
                                  بنسټ_array.شاخص_2,
                                  پیل, پای_ټکی, هذا->class_شمېر_2));
    TRY (ګراف.جمع_کړه_link (&(اولي->بنسټ_Array), اولي_id, new_بنسټ_array));

    أرجع Ok(اولي_id);
  }
};


بنية Mark_بنسټ_Pos : عام OT::Layout::GPOS_impl::Mark_بنسټ_Pos
{
  ګراف_پایله_t<hb_ویکټور_t<بینښه>> وېشل_subtables (gsubgpos_ګراف_context_t& c,
                                                         بینښه this_شاخص)
  {
    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((Mark_بنسټ_Pos_Format1*)(&u.format1))->وېشل_subtables (c, this_شاخص);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
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
      أرجع ((Mark_بنسټ_Pos_Format1*)(&u.format1))->sanitize (راس);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 2: HB_FALLTHROUGH;
#د_که_پای
    افتراضي:
      // We don't handle format 3 and 4 here.
      أرجع Err(SANITIZE_FAILURE);
    }
  }
};


}

#د_که_پای  // GRAPH_MARKBASEPOS_GRAPH_HH
