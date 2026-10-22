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

#اگر_غیر_تعریف_شدہ گراف_MARKBASEPOS_گراف_HH
#تعریف_کرو گراف_MARKBASEPOS_گراف_HH

#شامل "split-helpers.hh"
#شامل "coverage-graph.hh"
#شامل "../OT/Layout/GPOS/MarkBasePos.hh"
#شامل "../OT/Layout/GPOS/PosLookupSubTable.hh"

نامخلا گراف {

ساخت Anchor_میٹرکس : عوامی OT::Layout::GPOS_impl::Anchor_میٹرکس<SmallTypes>
{
  گراف_نتیجہ_t<خالی> sanitize (ثابت گراف_t::راس_t& راس, غیرعلامتی class_گنتی) ثابت
  {
    size_t راس_len = راس.جدول_سائز();
    اگر (unlikely (راس_len < Anchor_میٹرکس::min_سائز)) واپس Err(SANITIZE_FAILURE);
    hb_barrier ();

    اگر (unlikely (راس_len < Anchor_میٹرکس::min_سائز +
                  OT::Offset16::static_سائز * class_گنتی * یہ->صفیں))
      واپس Err(SANITIZE_FAILURE);
    واپس Ok();
  }

  گراف_نتیجہ_t<خالی> shrink (gsubgpos_گراف_context_t& c,
                               غیرعلامتی this_اشاریہ,
                               غیرعلامتی old_class_گنتی,
                               غیرعلامتی new_class_گنتی)
  {
    اگر (unlikely (new_class_گنتی >= old_class_گنتی)) واپس Err(INVALID_دلیل);
    خودکار& v = c.گراف.vertices_[this_اشاریہ];
    ثابت خودکار& o = v.obj ();
    غیرعلامتی بنیاد_گنتی = صفیں;
    v.سیٹ_بفر (o.سر_2,
      o.سر_2 +
      Anchor_میٹرکس::min_سائز +
      OT::Offset16::static_سائز * بنیاد_گنتی * new_class_گنتی);

    // Reposition links into the new indexing scheme.
    برائے (خودکار& link : v.real_links_writer ())
    {
      غیرعلامتی اشاریہ_2 = (link.position - 2) / 2;
      غیرعلامتی بنیاد = اشاریہ_2 / old_class_گنتی;
      غیرعلامتی klass = اشاریہ_2 % old_class_گنتی;
      اگر (unlikely (klass >= new_class_گنتی))
        // should have already been removed
        واپس Err(INVALID_دلیل);

      غیرعلامتی new_اشاریہ = بنیاد * new_class_گنتی + klass;

      link.position = (حرف*) &(یہ->میٹرکس_Z[new_اشاریہ]) - (حرف*) یہ;
    }

    واپس Ok();
  }

  گراف_نتیجہ_t<غیرعلامتی> clone (gsubgpos_گراف_context_t& c,
                                  غیرعلامتی this_اشاریہ,
                                  غیرعلامتی آغاز,
                                  غیرعلامتی انتہا,
                                  غیرعلامتی class_گنتی)
  {
    غیرعلامتی بنیاد_گنتی = صفیں;
    غیرعلامتی new_class_گنتی = انتہا - آغاز;
    غیرعلامتی سائز = Anchor_میٹرکس::min_سائز +
                    OT::Offset16::static_سائز * new_class_گنتی * صفیں;
    TRY_تفویض (غیرعلامتی مفرد_id, c.create_گرہ (سائز));
    Anchor_میٹرکس* مفرد = (Anchor_میٹرکس*) c.گراف.object (مفرد_id).سر_2;
    مفرد->صفیں = بنیاد_گنتی;

    خودکار& v = c.گراف.vertices_[this_اشاریہ];
    ثابت خودکار& o = v.obj ();

    صحیح عدد_links = o.real_links.لمبائی;
    برائے (صحیح i = 0; i < عدد_links; i++)
    {
      ثابت خودکار& link = o.real_links[i];
      غیرعلامتی old_اشاریہ = (link.position - 2) / OT::Offset16::static_سائز;
      غیرعلامتی klass = old_اشاریہ % class_گنتی;
      اگر (klass < آغاز || klass >= انتہا) جاری;

      غیرعلامتی بنیاد = old_اشاریہ / class_گنتی;
      غیرعلامتی new_klass = klass - آغاز;
      غیرعلامتی new_اشاریہ = بنیاد * new_class_گنتی + new_klass;


      غیرعلامتی بچہ_idx = link.objidx;
      TRY (c.گراف.جوڑو_link (&(مفرد->میٹرکس_Z[new_اشاریہ]),
                             مفرد_id,
                             بچہ_idx));

      خودکار& بچہ = c.گراف.vertices_[بچہ_idx];
      بچہ.remove_والد (this_اشاریہ);

      v.remove_real_link_unordered (i);
      عدد_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.ترتیب_دو_real_links ();

    واپس Ok(مفرد_id);
  }
};

ساخت MarkArray : عوامی OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  گراف_نتیجہ_t<خالی> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائز();
    غیرعلامتی min_سائز = MarkArray::min_سائز;
    اگر (unlikely (راس_len < min_سائز)) واپس Err(SANITIZE_FAILURE);
    hb_barrier ();

    اگر (unlikely (راس_len < get_سائز ())) واپس Err(SANITIZE_FAILURE);
    واپس Ok();
  }

  گراف_نتیجہ_t<خالی> shrink (gsubgpos_گراف_context_t& c,
                               ثابت hb_hashmap_t<غیرعلامتی, غیرعلامتی>& mark_array_links,
                               غیرعلامتی this_اشاریہ,
                               غیرعلامتی new_class_گنتی)
  {
    خودکار& v = c.گراف.vertices_[this_اشاریہ];
    ثابت خودکار& o = v.obj ();
    برائے (ثابت خودکار& link : o.real_links)
      c.گراف.vertices_[link.objidx].remove_والد (this_اشاریہ);
    v.clear_real_links ();

    غیرعلامتی new_اشاریہ = 0;
    برائے (ثابت خودکار& ریکارڈ : یہ->iter ())
    {
      غیرعلامتی klass = ریکارڈ.klass;
      اگر (klass >= new_class_گنتی) جاری;

      (*یہ)[new_اشاریہ].klass = klass;
      غیرعلامتی position = (حرف*) &ریکارڈ.markAnchor - (حرف*) یہ;
      غیرعلامتی* objidx;
      اگر (!mark_array_links.has (position, &objidx))
      {
        new_اشاریہ++;
        جاری;
      }

      TRY (c.گراف.جوڑو_link (&(*یہ)[new_اشاریہ].markAnchor, this_اشاریہ, *objidx));
      new_اشاریہ++;
    }

    یہ->len = new_اشاریہ;
    v.سیٹ_بفر (o.سر_2, o.سر_2 + MarkArray::min_سائز +
                  OT::Layout::GPOS_impl::Mark_ریکارڈ<SmallTypes>::static_سائز * new_اشاریہ);

    واپس Ok();
  }

  گراف_نتیجہ_t<غیرعلامتی> clone (gsubgpos_گراف_context_t& c,
                                  غیرعلامتی this_اشاریہ,
                                  ثابت hb_hashmap_t<غیرعلامتی, غیرعلامتی>& pos_مقصد_اشاریہ,
                                  hb_سیٹ_t& marks,
                                  غیرعلامتی آغاز_class)
  {
    غیرعلامتی سائز = MarkArray::min_سائز +
                    OT::Layout::GPOS_impl::Mark_ریکارڈ<SmallTypes>::static_سائز *
                    marks.get_population ();
    TRY_تفویض (غیرعلامتی مفرد_id, c.create_گرہ (سائز));
    MarkArray* مفرد = (MarkArray*) c.گراف.object (مفرد_id).سر_2;
    مفرد->len = marks.get_population ();


    غیرعلامتی i = 0;
    برائے (hb_codepoint_t mark : marks)
    {
      (*مفرد)[i].klass = (*یہ)[mark].klass - آغاز_class;
      غیرعلامتی انحراف_pos = (حرف*) &((*یہ)[mark].markAnchor) - (حرف*) یہ;
      غیرعلامتی* anchor_اشاریہ;
      اگر (pos_مقصد_اشاریہ.has (انحراف_pos, &anchor_اشاریہ))
        TRY (c.گراف.move_بچہ (this_اشاریہ,
                                 &((*یہ)[mark].markAnchor),
                                 مفرد_id,
                                 &((*مفرد)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.گراف.vertices_[this_اشاریہ].ترتیب_دو_real_links ();

    واپس Ok(مفرد_id);
  }
};

ساخت Mark_بنیاد_Pos_Format1 : عوامی OT::Layout::GPOS_impl::Mark_بنیاد_Pos_Format1_2<SmallTypes>
{
  گراف_نتیجہ_t<خالی> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائز ();
    اگر (unlikely (راس_len < Mark_بنیاد_Pos_Format1::static_سائز))
      واپس Err(SANITIZE_FAILURE);
    واپس Ok();
  }

  گراف_نتیجہ_t<hb_سمتیہ_t<غیرعلامتی>> بانٹو_subtables (gsubgpos_گراف_context_t& c,
                                                         غیرعلامتی this_اشاریہ)
  {
    hb_سیٹ_t دیکھا;

    TRY_تفویض (ثابت غیرعلامتی بنیاد_coverage_id, c.گراف.اشاریہ_for_انحراف (this_اشاریہ, &بنیاد_Coverage));

    ثابت غیرعلامتی بنیاد_سائز =
        OT::Layout::GPOS_impl::Mark_بنیاد_Pos_Format1_2<SmallTypes>::min_سائز +
        MarkArray::min_سائز +
        Anchor_میٹرکس::min_سائز +
        c.گراف.vertices_[بنیاد_coverage_id].جدول_سائز ();

    TRY_تفویض (hb_سمتیہ_t<class_info_t> class_مقصد_info, get_class_info (c, this_اشاریہ));

    غیرعلامتی class_گنتی = class_گنتی_2;
    TRY_تفویض (خودکار بنیاد_array, c.گراف.as_جدول<Anchor_میٹرکس> (this_اشاریہ,
                                                          &بنیاد_Array,
                                                          class_گنتی));
    غیرعلامتی بنیاد_گنتی = بنیاد_array.جدول->صفیں;

    غیرعلامتی partial_coverage_سائز = 4;
    غیرعلامتی accumulated = بنیاد_سائز;
    hb_سمتیہ_t<غیرعلامتی> بانٹو_points;

    برائے (غیرعلامتی klass = 0; klass < class_گنتی; klass++)
    {
      class_info_t& info = class_مقصد_info[klass];
      partial_coverage_سائز += OT::HBUINT16::static_سائز * info.marks.get_population ();
      غیرعلامتی accumulated_delta =
          OT::Layout::GPOS_impl::Mark_ریکارڈ<SmallTypes>::static_سائز * info.marks.get_population () +
          OT::Offset16::static_سائز * بنیاد_گنتی;

      برائے (غیرعلامتی objidx : info.بچہ_indices)
        accumulated_delta += c.گراف.ڈھونڈو_subgraph_سائز (objidx, دیکھا).قدر_or (0);

      accumulated += accumulated_delta;
      غیرعلامتی کل = accumulated + partial_coverage_سائز;

      اگر (کل >= (1 << 16))
      {
        بانٹو_points.push (klass);
        TRY (گراف_نتیجہ_t<خالی>::from (بانٹو_points, تخصیص_FAILURE));
        accumulated = بنیاد_سائز + accumulated_delta;
        partial_coverage_سائز = 4 + OT::HBUINT16::static_سائز * info.marks.get_population ();
        دیکھا.clear (); // node sharing isn't allowed between splits.
      }
    }


    TRY_تفویض (ثابت غیرعلامتی mark_array_id, c.گراف.اشاریہ_for_انحراف (this_اشاریہ, &markArray));
    TRY_تفویض (خودکار mark_array_links, c.گراف.vertices_[mark_array_id].position_مقصد_اشاریہ_map ());

    بانٹو_context_t بانٹو_context {
      c,
      یہ,
      this_اشاریہ,
      std::move (class_مقصد_info),
      std::move (mark_array_links),
    };

    واپس actuate_subtable_بانٹو<بانٹو_context_t> (بانٹو_context, بانٹو_points);
  }

 نجی:

  ساخت class_info_t {
    hb_سیٹ_t marks;
    hb_سمتیہ_t<غیرعلامتی> بچہ_indices;
  };

  ساخت بانٹو_context_t {
    gsubgpos_گراف_context_t& c;
    Mark_بنیاد_Pos_Format1* thiz;
    غیرعلامتی this_اشاریہ;
    hb_سمتیہ_t<class_info_t> class_مقصد_info;
    hb_hashmap_t<غیرعلامتی, غیرعلامتی> mark_array_links;

    hb_سیٹ_t marks_for (غیرعلامتی آغاز, غیرعلامتی انتہا)
    {
      hb_سیٹ_t marks;
      برائے (غیرعلامتی klass = آغاز; klass < انتہا; klass++)
      {
        + class_مقصد_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      واپس marks;
    }

    غیرعلامتی original_گنتی_2 ()
    {
      واپس thiz->class_گنتی_2;
    }

    گراف_نتیجہ_t<غیرعلامتی> clone_range (غیرعلامتی آغاز, غیرعلامتی انتہا)
    {
      واپس thiz->clone_range (*یہ, یہ->this_اشاریہ, آغاز, انتہا);
    }

    گراف_نتیجہ_t<خالی> shrink (غیرعلامتی گنتی)
    {
      واپس thiz->shrink (*یہ, یہ->this_اشاریہ, گنتی);
    }
  };

  گراف_نتیجہ_t<hb_سمتیہ_t<class_info_t>> get_class_info (gsubgpos_گراف_context_t& c,
                                                            غیرعلامتی this_اشاریہ)
  {
    hb_سمتیہ_t<class_info_t> class_مقصد_info;

    غیرعلامتی class_گنتی = class_گنتی_2;
    اگر (!class_گنتی) واپس class_مقصد_info;

    اگر (unlikely (!class_مقصد_info.resize (class_گنتی)))
      واپس Err(تخصیص_FAILURE);

    TRY_تفویض (خودکار mark_array, c.گراف.as_جدول<MarkArray> (this_اشاریہ, &markArray));

    غیرعلامتی mark_گنتی = mark_array.جدول->len;
    برائے (غیرعلامتی mark = 0; mark < mark_گنتی; mark++)
    {
      غیرعلامتی klass = (*mark_array.جدول)[mark].get_class ();
      اگر (klass >= class_گنتی) جاری;
      class_مقصد_info[klass].marks.جوڑو (mark);
    }

    برائے (ثابت خودکار& link : mark_array.راس->obj ().real_links)
    {
      غیرعلامتی mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::Mark_ریکارڈ<SmallTypes>::static_سائز;
      غیرعلامتی klass = (*mark_array.جدول)[mark].get_class ();
      اگر (klass >= class_گنتی) جاری;
      class_مقصد_info[klass].بچہ_indices.push (link.objidx);
    }

    TRY_تفویض (غیرعلامتی بنیاد_array_id,
         c.گراف.اشاریہ_for_انحراف (this_اشاریہ, &بنیاد_Array));

    خودکار& بنیاد_array_v = c.گراف.vertices_[بنیاد_array_id];

    برائے (ثابت خودکار& link : بنیاد_array_v.obj ().real_links)
    {
      غیرعلامتی اشاریہ_2 = (link.position - 2) / OT::Offset16::static_سائز;
      غیرعلامتی klass = اشاریہ_2 % class_گنتی;
      class_مقصد_info[klass].بچہ_indices.push (link.objidx);
    }

    واپس class_مقصد_info;
  }

  گراف_نتیجہ_t<خالی> shrink (بانٹو_context_t& sc,
                               غیرعلامتی this_اشاریہ,
                               غیرعلامتی گنتی)
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_اشاریہ,
               گنتی);

    غیرعلامتی old_گنتی = class_گنتی_2;
    اگر (گنتی >= old_گنتی)
      واپس Ok();

    class_گنتی_2 = گنتی;

    TRY_تفویض (خودکار mark_coverage, sc.c.گراف.as_mutable_جدول<Coverage> (this_اشاریہ,
                                                                    &markCoverage));

    hb_سیٹ_t marks = sc.marks_for (0, گنتی);
    خودکار new_coverage =
        + hb_enumerate (mark_coverage.جدول->iter ())
        | hb_filter (marks, hb_پہلا)
        | hb_map_retains_ترتیب_کاری (hb_دوسرا)
        ;
    TRY (Coverage::make_coverage (sc.c, + new_coverage,
                                  mark_coverage.اشاریہ_2,
                                  4 + 2 * marks.get_population ()));


    TRY_تفویض (خودکار بنیاد_array, sc.c.گراف.as_mutable_جدول<Anchor_میٹرکس> (this_اشاریہ,
                                                                     &بنیاد_Array,
                                                                     old_گنتی));

    TRY (بنیاد_array.جدول->shrink (sc.c,
                                   بنیاد_array.اشاریہ_2,
                                   old_گنتی,
                                   گنتی));

    TRY_تفویض (خودکار mark_array, sc.c.گراف.as_mutable_جدول<MarkArray> (this_اشاریہ,
                                                                  &markArray));

    TRY (mark_array.جدول->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.اشاریہ_2,
                                   گنتی));

    واپس Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  گراف_نتیجہ_t<غیرعلامتی> clone_range (بانٹو_context_t& sc,
                                        غیرعلامتی this_اشاریہ,
                                        غیرعلامتی آغاز, غیرعلامتی انتہا) ثابت
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_اشاریہ, آغاز, انتہا);

    گراف_t& گراف = sc.c.گراف;
    غیرعلامتی مفرد_سائز = OT::Layout::GPOS_impl::Mark_بنیاد_Pos_Format1_2<SmallTypes>::static_سائز;

    TRY_تفویض (غیرعلامتی مفرد_id, sc.c.create_گرہ (مفرد_سائز));

    Mark_بنیاد_Pos_Format1* مفرد = (Mark_بنیاد_Pos_Format1*) گراف.object (مفرد_id).سر_2;
    مفرد->format = یہ->format;
    غیرعلامتی new_class_گنتی = انتہا - آغاز;
    مفرد->class_گنتی_2 = new_class_گنتی;

    TRY_تفویض (غیرعلامتی بنیاد_coverage_id,
         گراف.اشاریہ_for_انحراف (sc.this_اشاریہ, &بنیاد_Coverage));

    TRY (گراف.جوڑو_link (&(مفرد->بنیاد_Coverage), مفرد_id, بنیاد_coverage_id));

    TRY_تفویض (خودکار mark_coverage, sc.c.گراف.as_جدول<Coverage> (this_اشاریہ,
                                                            &markCoverage));

    hb_سیٹ_t marks = sc.marks_for (آغاز, انتہا);
    خودکار new_coverage =
        + hb_enumerate (mark_coverage.جدول->iter ())
        | hb_filter (marks, hb_پہلا)
        | hb_map_retains_ترتیب_کاری (hb_دوسرا)
        ;
    TRY (Coverage::جوڑو_coverage (sc.c,
                                 مفرد_id,
                                 2,
                                 + new_coverage,
                                 marks.get_population () * 2 + 4));

    TRY_تفویض (خودکار mark_array,
         گراف.as_mutable_جدول <MarkArray> (sc.this_اشاریہ, &markArray));

    TRY_تفویض (غیرعلامتی new_mark_array,
         mark_array.جدول->clone (sc.c,
                                  mark_array.اشاریہ_2,
                                  sc.mark_array_links,
                                  marks,
                                  آغاز));
    TRY (گراف.جوڑو_link (&(مفرد->markArray), مفرد_id, new_mark_array));

    غیرعلامتی class_گنتی = class_گنتی_2;
    TRY_تفویض (خودکار بنیاد_array,
         گراف.as_mutable_جدول<Anchor_میٹرکس> (sc.this_اشاریہ, &بنیاد_Array, class_گنتی));

    TRY_تفویض (غیرعلامتی new_بنیاد_array,
         بنیاد_array.جدول->clone (sc.c,
                                  بنیاد_array.اشاریہ_2,
                                  آغاز, انتہا, یہ->class_گنتی_2));
    TRY (گراف.جوڑو_link (&(مفرد->بنیاد_Array), مفرد_id, new_بنیاد_array));

    واپس Ok(مفرد_id);
  }
};


ساخت Mark_بنیاد_Pos : عوامی OT::Layout::GPOS_impl::Mark_بنیاد_Pos
{
  گراف_نتیجہ_t<hb_سمتیہ_t<غیرعلامتی>> بانٹو_subtables (gsubgpos_گراف_context_t& c,
                                                         غیرعلامتی this_اشاریہ)
  {
    انتخاب (u.format.v) {
    حالت 1:
      hb_barrier ();
      واپس ((Mark_بنیاد_Pos_Format1*)(&u.format1))->بانٹو_subtables (c, this_اشاریہ);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
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
      واپس ((Mark_بنیاد_Pos_Format1*)(&u.format1))->sanitize (راس);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 2: HB_FALLTHROUGH;
#اگر_ختم
    طےشدہ:
      // We don't handle format 3 and 4 here.
      واپس Err(SANITIZE_FAILURE);
    }
  }
};


}

#اگر_ختم  // GRAPH_MARKBASEPOS_GRAPH_HH
