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

#اگر_تعریف_نشده گراف_MARKBASEPOS_گراف_HH
#تعریف_کن گراف_MARKBASEPOS_گراف_HH

#ضمن "split-helpers.hh"
#ضمن "coverage-graph.hh"
#ضمن "../OT/Layout/GPOS/MarkBasePos.hh"
#ضمن "../OT/Layout/GPOS/PosLookupSubTable.hh"

نطاق گراف {

بنية Anchor_ماتریس : عام OT::Layout::GPOS_impl::Anchor_ماتریس<SmallTypes>
{
  گراف_نتیجه_t<فراغ> sanitize (ثابت گراف_t::رأس_t& رأس, بیعلامت class_شمار) ثابت
  {
    size_t رأس_len = رأس.جدول_اندازه();
    إذا (unlikely (رأس_len < Anchor_ماتریس::min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (رأس_len < Anchor_ماتریس::min_اندازه +
                  OT::Offset16::static_اندازه * class_شمار * هذا->ردیفها))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتیجه_t<فراغ> shrink (gsubgpos_گراف_context_t& c,
                               بیعلامت this_شاخص,
                               بیعلامت old_class_شمار,
                               بیعلامت new_class_شمار)
  {
    إذا (unlikely (new_class_شمار >= old_class_شمار)) أرجع Err(INVALID_آرگومان);
    خودکار& v = c.گراف.vertices_[this_شاخص];
    ثابت خودکار& o = v.obj ();
    بیعلامت پایه_شمار = ردیفها;
    v.مجموعه_میانگیر (o.سر_2,
      o.سر_2 +
      Anchor_ماتریس::min_اندازه +
      OT::Offset16::static_اندازه * پایه_شمار * new_class_شمار);

    // Reposition links into the new indexing scheme.
    لكل (خودکار& link : v.real_links_writer ())
    {
      بیعلامت شاخص_2 = (link.position - 2) / 2;
      بیعلامت پایه = شاخص_2 / old_class_شمار;
      بیعلامت klass = شاخص_2 % old_class_شمار;
      إذا (unlikely (klass >= new_class_شمار))
        // should have already been removed
        أرجع Err(INVALID_آرگومان);

      بیعلامت new_شاخص = پایه * new_class_شمار + klass;

      link.position = (حرف*) &(هذا->ماتریس_Z[new_شاخص]) - (حرف*) هذا;
    }

    أرجع Ok();
  }

  گراف_نتیجه_t<بیعلامت> clone (gsubgpos_گراف_context_t& c,
                                  بیعلامت this_شاخص,
                                  بیعلامت شروع,
                                  بیعلامت انتها,
                                  بیعلامت class_شمار)
  {
    بیعلامت پایه_شمار = ردیفها;
    بیعلامت new_class_شمار = انتها - شروع;
    بیعلامت اندازه = Anchor_ماتریس::min_اندازه +
                    OT::Offset16::static_اندازه * new_class_شمار * ردیفها;
    TRY_تخصیص_بده (بیعلامت اول_id, c.create_گره (اندازه));
    Anchor_ماتریس* اول = (Anchor_ماتریس*) c.گراف.object (اول_id).سر_2;
    اول->ردیفها = پایه_شمار;

    خودکار& v = c.گراف.vertices_[this_شاخص];
    ثابت خودکار& o = v.obj ();

    صحيح رقم_links = o.real_links.درازی;
    لكل (صحيح i = 0; i < رقم_links; i++)
    {
      ثابت خودکار& link = o.real_links[i];
      بیعلامت old_شاخص = (link.position - 2) / OT::Offset16::static_اندازه;
      بیعلامت klass = old_شاخص % class_شمار;
      إذا (klass < شروع || klass >= انتها) استمر;

      بیعلامت پایه = old_شاخص / class_شمار;
      بیعلامت new_klass = klass - شروع;
      بیعلامت new_شاخص = پایه * new_class_شمار + new_klass;


      بیعلامت فرزند_idx = link.objidx;
      TRY (c.گراف.جمع_کن_link (&(اول->ماتریس_Z[new_شاخص]),
                             اول_id,
                             فرزند_idx));

      خودکار& فرزند = c.گراف.vertices_[فرزند_idx];
      فرزند.remove_والد (this_شاخص);

      v.remove_real_link_unordered (i);
      رقم_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.مرتب_سازی_real_links ();

    أرجع Ok(اول_id);
  }
};

بنية MarkArray : عام OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  گراف_نتیجه_t<فراغ> sanitize (ثابت گراف_t::رأس_t& رأس) ثابت
  {
    size_t رأس_len = رأس.جدول_اندازه();
    بیعلامت min_اندازه = MarkArray::min_اندازه;
    إذا (unlikely (رأس_len < min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (رأس_len < get_اندازه ())) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتیجه_t<فراغ> shrink (gsubgpos_گراف_context_t& c,
                               ثابت hb_hashmap_t<بیعلامت, بیعلامت>& mark_array_links,
                               بیعلامت this_شاخص,
                               بیعلامت new_class_شمار)
  {
    خودکار& v = c.گراف.vertices_[this_شاخص];
    ثابت خودکار& o = v.obj ();
    لكل (ثابت خودکار& link : o.real_links)
      c.گراف.vertices_[link.objidx].remove_والد (this_شاخص);
    v.clear_real_links ();

    بیعلامت new_شاخص = 0;
    لكل (ثابت خودکار& رکورد : هذا->iter ())
    {
      بیعلامت klass = رکورد.klass;
      إذا (klass >= new_class_شمار) استمر;

      (*هذا)[new_شاخص].klass = klass;
      بیعلامت position = (حرف*) &رکورد.markAnchor - (حرف*) هذا;
      بیعلامت* objidx;
      إذا (!mark_array_links.has (position, &objidx))
      {
        new_شاخص++;
        استمر;
      }

      TRY (c.گراف.جمع_کن_link (&(*هذا)[new_شاخص].markAnchor, this_شاخص, *objidx));
      new_شاخص++;
    }

    هذا->len = new_شاخص;
    v.مجموعه_میانگیر (o.سر_2, o.سر_2 + MarkArray::min_اندازه +
                  OT::Layout::GPOS_impl::Mark_رکورد<SmallTypes>::static_اندازه * new_شاخص);

    أرجع Ok();
  }

  گراف_نتیجه_t<بیعلامت> clone (gsubgpos_گراف_context_t& c,
                                  بیعلامت this_شاخص,
                                  ثابت hb_hashmap_t<بیعلامت, بیعلامت>& pos_مقصد_شاخص,
                                  hb_مجموعه_t& marks,
                                  بیعلامت شروع_class)
  {
    بیعلامت اندازه = MarkArray::min_اندازه +
                    OT::Layout::GPOS_impl::Mark_رکورد<SmallTypes>::static_اندازه *
                    marks.get_population ();
    TRY_تخصیص_بده (بیعلامت اول_id, c.create_گره (اندازه));
    MarkArray* اول = (MarkArray*) c.گراف.object (اول_id).سر_2;
    اول->len = marks.get_population ();


    بیعلامت i = 0;
    لكل (hb_codepoint_t mark : marks)
    {
      (*اول)[i].klass = (*هذا)[mark].klass - شروع_class;
      بیعلامت جابه_جایی_pos = (حرف*) &((*هذا)[mark].markAnchor) - (حرف*) هذا;
      بیعلامت* anchor_شاخص;
      إذا (pos_مقصد_شاخص.has (جابه_جایی_pos, &anchor_شاخص))
        TRY (c.گراف.move_فرزند (this_شاخص,
                                 &((*هذا)[mark].markAnchor),
                                 اول_id,
                                 &((*اول)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.گراف.vertices_[this_شاخص].مرتب_سازی_real_links ();

    أرجع Ok(اول_id);
  }
};

بنية Mark_پایه_Pos_Format1 : عام OT::Layout::GPOS_impl::Mark_پایه_Pos_Format1_2<SmallTypes>
{
  گراف_نتیجه_t<فراغ> sanitize (ثابت گراف_t::رأس_t& رأس) ثابت
  {
    size_t رأس_len = رأس.جدول_اندازه ();
    إذا (unlikely (رأس_len < Mark_پایه_Pos_Format1::static_اندازه))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتیجه_t<hb_وکتور_t<بیعلامت>> تقسیم_subtables (gsubgpos_گراف_context_t& c,
                                                         بیعلامت this_شاخص)
  {
    hb_مجموعه_t دیدهشده;

    TRY_تخصیص_بده (ثابت بیعلامت پایه_coverage_id, c.گراف.شاخص_for_جابه_جایی (this_شاخص, &پایه_Coverage));

    ثابت بیعلامت پایه_اندازه =
        OT::Layout::GPOS_impl::Mark_پایه_Pos_Format1_2<SmallTypes>::min_اندازه +
        MarkArray::min_اندازه +
        Anchor_ماتریس::min_اندازه +
        c.گراف.vertices_[پایه_coverage_id].جدول_اندازه ();

    TRY_تخصیص_بده (hb_وکتور_t<class_info_t> class_مقصد_info, get_class_info (c, this_شاخص));

    بیعلامت class_شمار = class_شمار_2;
    TRY_تخصیص_بده (خودکار پایه_array, c.گراف.as_جدول<Anchor_ماتریس> (this_شاخص,
                                                          &پایه_Array,
                                                          class_شمار));
    بیعلامت پایه_شمار = پایه_array.جدول->ردیفها;

    بیعلامت partial_coverage_اندازه = 4;
    بیعلامت accumulated = پایه_اندازه;
    hb_وکتور_t<بیعلامت> تقسیم_points;

    لكل (بیعلامت klass = 0; klass < class_شمار; klass++)
    {
      class_info_t& info = class_مقصد_info[klass];
      partial_coverage_اندازه += OT::HBUINT16::static_اندازه * info.marks.get_population ();
      بیعلامت accumulated_delta =
          OT::Layout::GPOS_impl::Mark_رکورد<SmallTypes>::static_اندازه * info.marks.get_population () +
          OT::Offset16::static_اندازه * پایه_شمار;

      لكل (بیعلامت objidx : info.فرزند_indices)
        accumulated_delta += c.گراف.پیدا_کن_subgraph_اندازه (objidx, دیدهشده).مقدار_or (0);

      accumulated += accumulated_delta;
      بیعلامت مجموع = accumulated + partial_coverage_اندازه;

      إذا (مجموع >= (1 << 16))
      {
        تقسیم_points.push (klass);
        TRY (گراف_نتیجه_t<فراغ>::from (تقسیم_points, تخصیص_FAILURE));
        accumulated = پایه_اندازه + accumulated_delta;
        partial_coverage_اندازه = 4 + OT::HBUINT16::static_اندازه * info.marks.get_population ();
        دیدهشده.clear (); // node sharing isn't allowed between splits.
      }
    }


    TRY_تخصیص_بده (ثابت بیعلامت mark_array_id, c.گراف.شاخص_for_جابه_جایی (this_شاخص, &markArray));
    TRY_تخصیص_بده (خودکار mark_array_links, c.گراف.vertices_[mark_array_id].position_مقصد_شاخص_map ());

    تقسیم_context_t تقسیم_context {
      c,
      هذا,
      this_شاخص,
      std::move (class_مقصد_info),
      std::move (mark_array_links),
    };

    أرجع actuate_subtable_تقسیم<تقسیم_context_t> (تقسیم_context, تقسیم_points);
  }

 خاص:

  بنية class_info_t {
    hb_مجموعه_t marks;
    hb_وکتور_t<بیعلامت> فرزند_indices;
  };

  بنية تقسیم_context_t {
    gsubgpos_گراف_context_t& c;
    Mark_پایه_Pos_Format1* thiz;
    بیعلامت this_شاخص;
    hb_وکتور_t<class_info_t> class_مقصد_info;
    hb_hashmap_t<بیعلامت, بیعلامت> mark_array_links;

    hb_مجموعه_t marks_for (بیعلامت شروع, بیعلامت انتها)
    {
      hb_مجموعه_t marks;
      لكل (بیعلامت klass = شروع; klass < انتها; klass++)
      {
        + class_مقصد_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      أرجع marks;
    }

    بیعلامت original_شمار_2 ()
    {
      أرجع thiz->class_شمار_2;
    }

    گراف_نتیجه_t<بیعلامت> clone_range (بیعلامت شروع, بیعلامت انتها)
    {
      أرجع thiz->clone_range (*هذا, هذا->this_شاخص, شروع, انتها);
    }

    گراف_نتیجه_t<فراغ> shrink (بیعلامت شمار)
    {
      أرجع thiz->shrink (*هذا, هذا->this_شاخص, شمار);
    }
  };

  گراف_نتیجه_t<hb_وکتور_t<class_info_t>> get_class_info (gsubgpos_گراف_context_t& c,
                                                            بیعلامت this_شاخص)
  {
    hb_وکتور_t<class_info_t> class_مقصد_info;

    بیعلامت class_شمار = class_شمار_2;
    إذا (!class_شمار) أرجع class_مقصد_info;

    إذا (unlikely (!class_مقصد_info.resize (class_شمار)))
      أرجع Err(تخصیص_FAILURE);

    TRY_تخصیص_بده (خودکار mark_array, c.گراف.as_جدول<MarkArray> (this_شاخص, &markArray));

    بیعلامت mark_شمار = mark_array.جدول->len;
    لكل (بیعلامت mark = 0; mark < mark_شمار; mark++)
    {
      بیعلامت klass = (*mark_array.جدول)[mark].get_class ();
      إذا (klass >= class_شمار) استمر;
      class_مقصد_info[klass].marks.جمع_کن (mark);
    }

    لكل (ثابت خودکار& link : mark_array.رأس->obj ().real_links)
    {
      بیعلامت mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::Mark_رکورد<SmallTypes>::static_اندازه;
      بیعلامت klass = (*mark_array.جدول)[mark].get_class ();
      إذا (klass >= class_شمار) استمر;
      class_مقصد_info[klass].فرزند_indices.push (link.objidx);
    }

    TRY_تخصیص_بده (بیعلامت پایه_array_id,
         c.گراف.شاخص_for_جابه_جایی (this_شاخص, &پایه_Array));

    خودکار& پایه_array_v = c.گراف.vertices_[پایه_array_id];

    لكل (ثابت خودکار& link : پایه_array_v.obj ().real_links)
    {
      بیعلامت شاخص_2 = (link.position - 2) / OT::Offset16::static_اندازه;
      بیعلامت klass = شاخص_2 % class_شمار;
      class_مقصد_info[klass].فرزند_indices.push (link.objidx);
    }

    أرجع class_مقصد_info;
  }

  گراف_نتیجه_t<فراغ> shrink (تقسیم_context_t& sc,
                               بیعلامت this_شاخص,
                               بیعلامت شمار)
  {
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_شاخص,
               شمار);

    بیعلامت old_شمار = class_شمار_2;
    إذا (شمار >= old_شمار)
      أرجع Ok();

    class_شمار_2 = شمار;

    TRY_تخصیص_بده (خودکار mark_coverage, sc.c.گراف.as_mutable_جدول<Coverage> (this_شاخص,
                                                                    &markCoverage));

    hb_مجموعه_t marks = sc.marks_for (0, شمار);
    خودکار new_coverage =
        + hb_enumerate (mark_coverage.جدول->iter ())
        | hb_filter (marks, hb_اولی)
        | hb_map_retains_ترتیبدهی (hb_دومی)
        ;
    TRY (Coverage::make_coverage (sc.c, + new_coverage,
                                  mark_coverage.شاخص_2,
                                  4 + 2 * marks.get_population ()));


    TRY_تخصیص_بده (خودکار پایه_array, sc.c.گراف.as_mutable_جدول<Anchor_ماتریس> (this_شاخص,
                                                                     &پایه_Array,
                                                                     old_شمار));

    TRY (پایه_array.جدول->shrink (sc.c,
                                   پایه_array.شاخص_2,
                                   old_شمار,
                                   شمار));

    TRY_تخصیص_بده (خودکار mark_array, sc.c.گراف.as_mutable_جدول<MarkArray> (this_شاخص,
                                                                  &markArray));

    TRY (mark_array.جدول->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.شاخص_2,
                                   شمار));

    أرجع Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  گراف_نتیجه_t<بیعلامت> clone_range (تقسیم_context_t& sc,
                                        بیعلامت this_شاخص,
                                        بیعلامت شروع, بیعلامت انتها) ثابت
  {
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_شاخص, شروع, انتها);

    گراف_t& گراف = sc.c.گراف;
    بیعلامت اول_اندازه = OT::Layout::GPOS_impl::Mark_پایه_Pos_Format1_2<SmallTypes>::static_اندازه;

    TRY_تخصیص_بده (بیعلامت اول_id, sc.c.create_گره (اول_اندازه));

    Mark_پایه_Pos_Format1* اول = (Mark_پایه_Pos_Format1*) گراف.object (اول_id).سر_2;
    اول->format = هذا->format;
    بیعلامت new_class_شمار = انتها - شروع;
    اول->class_شمار_2 = new_class_شمار;

    TRY_تخصیص_بده (بیعلامت پایه_coverage_id,
         گراف.شاخص_for_جابه_جایی (sc.this_شاخص, &پایه_Coverage));

    TRY (گراف.جمع_کن_link (&(اول->پایه_Coverage), اول_id, پایه_coverage_id));

    TRY_تخصیص_بده (خودکار mark_coverage, sc.c.گراف.as_جدول<Coverage> (this_شاخص,
                                                            &markCoverage));

    hb_مجموعه_t marks = sc.marks_for (شروع, انتها);
    خودکار new_coverage =
        + hb_enumerate (mark_coverage.جدول->iter ())
        | hb_filter (marks, hb_اولی)
        | hb_map_retains_ترتیبدهی (hb_دومی)
        ;
    TRY (Coverage::جمع_کن_coverage (sc.c,
                                 اول_id,
                                 2,
                                 + new_coverage,
                                 marks.get_population () * 2 + 4));

    TRY_تخصیص_بده (خودکار mark_array,
         گراف.as_mutable_جدول <MarkArray> (sc.this_شاخص, &markArray));

    TRY_تخصیص_بده (بیعلامت new_mark_array,
         mark_array.جدول->clone (sc.c,
                                  mark_array.شاخص_2,
                                  sc.mark_array_links,
                                  marks,
                                  شروع));
    TRY (گراف.جمع_کن_link (&(اول->markArray), اول_id, new_mark_array));

    بیعلامت class_شمار = class_شمار_2;
    TRY_تخصیص_بده (خودکار پایه_array,
         گراف.as_mutable_جدول<Anchor_ماتریس> (sc.this_شاخص, &پایه_Array, class_شمار));

    TRY_تخصیص_بده (بیعلامت new_پایه_array,
         پایه_array.جدول->clone (sc.c,
                                  پایه_array.شاخص_2,
                                  شروع, انتها, هذا->class_شمار_2));
    TRY (گراف.جمع_کن_link (&(اول->پایه_Array), اول_id, new_پایه_array));

    أرجع Ok(اول_id);
  }
};


بنية Mark_پایه_Pos : عام OT::Layout::GPOS_impl::Mark_پایه_Pos
{
  گراف_نتیجه_t<hb_وکتور_t<بیعلامت>> تقسیم_subtables (gsubgpos_گراف_context_t& c,
                                                         بیعلامت this_شاخص)
  {
    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((Mark_پایه_Pos_Format1*)(&u.format1))->تقسیم_subtables (c, this_شاخص);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
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
      أرجع ((Mark_پایه_Pos_Format1*)(&u.format1))->sanitize (رأس);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 2: HB_FALLTHROUGH;
#پایان_اگر
    افتراضي:
      // We don't handle format 3 and 4 here.
      أرجع Err(SANITIZE_FAILURE);
    }
  }
};


}

#پایان_اگر  // GRAPH_MARKBASEPOS_GRAPH_HH
