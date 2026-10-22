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

#جے_بنا_تعریف گراف_MARKBASEPOS_گراف_HH
#تعریف_کرو گراف_MARKBASEPOS_گراف_HH

#ضمن "split-helpers.hh"
#ضمن "coverage-graph.hh"
#ضمن "../OT/Layout/GPOS/MarkBasePos.hh"
#ضمن "../OT/Layout/GPOS/PosLookupSubTable.hh"

نطاق گراف {

بنية Anchor_میٹرکس : عام OT::Layout::GPOS_impl::Anchor_میٹرکس<SmallTypes>
{
  گراف_نتیجہ_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس, بنانشانی class_گنتی) ثابت
  {
    size_t راس_len = راس.جدول_ناپ();
    إذا (unlikely (راس_len < Anchor_میٹرکس::min_ناپ)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (راس_len < Anchor_میٹرکس::min_ناپ +
                  OT::Offset16::static_ناپ * class_گنتی * هذا->صفاں))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتیجہ_t<فراغ> shrink (gsubgpos_گراف_context_t& c,
                               بنانشانی this_اشاریہ,
                               بنانشانی old_class_گنتی,
                               بنانشانی new_class_گنتی)
  {
    إذا (unlikely (new_class_گنتی >= old_class_گنتی)) أرجع Err(INVALID_دلیل);
    خودکار& v = c.گراف.vertices_[this_اشاریہ];
    ثابت خودکار& o = v.obj ();
    بنانشانی ادھار_گنتی = صفاں;
    v.سیٹ_بفر (o.سر_2,
      o.سر_2 +
      Anchor_میٹرکس::min_ناپ +
      OT::Offset16::static_ناپ * ادھار_گنتی * new_class_گنتی);

    // Reposition links into the new indexing scheme.
    لكل (خودکار& link : v.real_links_writer ())
    {
      بنانشانی اشاریہ_2 = (link.position - 2) / 2;
      بنانشانی ادھار = اشاریہ_2 / old_class_گنتی;
      بنانشانی klass = اشاریہ_2 % old_class_گنتی;
      إذا (unlikely (klass >= new_class_گنتی))
        // should have already been removed
        أرجع Err(INVALID_دلیل);

      بنانشانی new_اشاریہ = ادھار * new_class_گنتی + klass;

      link.position = (حرف*) &(هذا->میٹرکس_Z[new_اشاریہ]) - (حرف*) هذا;
    }

    أرجع Ok();
  }

  گراف_نتیجہ_t<بنانشانی> clone (gsubgpos_گراف_context_t& c,
                                  بنانشانی this_اشاریہ,
                                  بنانشانی شروع,
                                  بنانشانی سرا,
                                  بنانشانی class_گنتی)
  {
    بنانشانی ادھار_گنتی = صفاں;
    بنانشانی new_class_گنتی = سرا - شروع;
    بنانشانی ناپ = Anchor_میٹرکس::min_ناپ +
                    OT::Offset16::static_ناپ * new_class_گنتی * صفاں;
    TRY_سونپو (بنانشانی مفرد_id, c.create_گرہ (ناپ));
    Anchor_میٹرکس* مفرد = (Anchor_میٹرکس*) c.گراف.object (مفرد_id).سر_2;
    مفرد->صفاں = ادھار_گنتی;

    خودکار& v = c.گراف.vertices_[this_اشاریہ];
    ثابت خودکار& o = v.obj ();

    صحيح عدد_links = o.real_links.لمائی;
    لكل (صحيح i = 0; i < عدد_links; i++)
    {
      ثابت خودکار& link = o.real_links[i];
      بنانشانی old_اشاریہ = (link.position - 2) / OT::Offset16::static_ناپ;
      بنانشانی klass = old_اشاریہ % class_گنتی;
      إذا (klass < شروع || klass >= سرا) استمر;

      بنانشانی ادھار = old_اشاریہ / class_گنتی;
      بنانشانی new_klass = klass - شروع;
      بنانشانی new_اشاریہ = ادھار * new_class_گنتی + new_klass;


      بنانشانی بچہ_idx = link.objidx;
      TRY (c.گراف.جوڑو_link (&(مفرد->میٹرکس_Z[new_اشاریہ]),
                             مفرد_id,
                             بچہ_idx));

      خودکار& بچہ = c.گراف.vertices_[بچہ_idx];
      بچہ.remove_ماپا (this_اشاریہ);

      v.remove_real_link_unordered (i);
      عدد_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.ترتیب_دیو_real_links ();

    أرجع Ok(مفرد_id);
  }
};

بنية MarkArray : عام OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  گراف_نتیجہ_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_ناپ();
    بنانشانی min_ناپ = MarkArray::min_ناپ;
    إذا (unlikely (راس_len < min_ناپ)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (راس_len < get_ناپ ())) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتیجہ_t<فراغ> shrink (gsubgpos_گراف_context_t& c,
                               ثابت hb_hashmap_t<بنانشانی, بنانشانی>& mark_array_links,
                               بنانشانی this_اشاریہ,
                               بنانشانی new_class_گنتی)
  {
    خودکار& v = c.گراف.vertices_[this_اشاریہ];
    ثابت خودکار& o = v.obj ();
    لكل (ثابت خودکار& link : o.real_links)
      c.گراف.vertices_[link.objidx].remove_ماپا (this_اشاریہ);
    v.clear_real_links ();

    بنانشانی new_اشاریہ = 0;
    لكل (ثابت خودکار& ریکارڈ : هذا->iter ())
    {
      بنانشانی klass = ریکارڈ.klass;
      إذا (klass >= new_class_گنتی) استمر;

      (*هذا)[new_اشاریہ].klass = klass;
      بنانشانی position = (حرف*) &ریکارڈ.markAnchor - (حرف*) هذا;
      بنانشانی* objidx;
      إذا (!mark_array_links.has (position, &objidx))
      {
        new_اشاریہ++;
        استمر;
      }

      TRY (c.گراف.جوڑو_link (&(*هذا)[new_اشاریہ].markAnchor, this_اشاریہ, *objidx));
      new_اشاریہ++;
    }

    هذا->len = new_اشاریہ;
    v.سیٹ_بفر (o.سر_2, o.سر_2 + MarkArray::min_ناپ +
                  OT::Layout::GPOS_impl::Mark_ریکارڈ<SmallTypes>::static_ناپ * new_اشاریہ);

    أرجع Ok();
  }

  گراف_نتیجہ_t<بنانشانی> clone (gsubgpos_گراف_context_t& c,
                                  بنانشانی this_اشاریہ,
                                  ثابت hb_hashmap_t<بنانشانی, بنانشانی>& pos_منزل_اشاریہ,
                                  hb_سیٹ_t& marks,
                                  بنانشانی شروع_class)
  {
    بنانشانی ناپ = MarkArray::min_ناپ +
                    OT::Layout::GPOS_impl::Mark_ریکارڈ<SmallTypes>::static_ناپ *
                    marks.get_population ();
    TRY_سونپو (بنانشانی مفرد_id, c.create_گرہ (ناپ));
    MarkArray* مفرد = (MarkArray*) c.گراف.object (مفرد_id).سر_2;
    مفرد->len = marks.get_population ();


    بنانشانی i = 0;
    لكل (hb_codepoint_t mark : marks)
    {
      (*مفرد)[i].klass = (*هذا)[mark].klass - شروع_class;
      بنانشانی وتھ_pos = (حرف*) &((*هذا)[mark].markAnchor) - (حرف*) هذا;
      بنانشانی* anchor_اشاریہ;
      إذا (pos_منزل_اشاریہ.has (وتھ_pos, &anchor_اشاریہ))
        TRY (c.گراف.move_بچہ (this_اشاریہ,
                                 &((*هذا)[mark].markAnchor),
                                 مفرد_id,
                                 &((*مفرد)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.گراف.vertices_[this_اشاریہ].ترتیب_دیو_real_links ();

    أرجع Ok(مفرد_id);
  }
};

بنية Mark_ادھار_Pos_Format1 : عام OT::Layout::GPOS_impl::Mark_ادھار_Pos_Format1_2<SmallTypes>
{
  گراف_نتیجہ_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_ناپ ();
    إذا (unlikely (راس_len < Mark_ادھار_Pos_Format1::static_ناپ))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتیجہ_t<hb_سمتیہ_t<بنانشانی>> ونڈو_subtables (gsubgpos_گراف_context_t& c,
                                                         بنانشانی this_اشاریہ)
  {
    hb_سیٹ_t ویکھیا;

    TRY_سونپو (ثابت بنانشانی ادھار_coverage_id, c.گراف.اشاریہ_for_وتھ (this_اشاریہ, &ادھار_Coverage));

    ثابت بنانشانی ادھار_ناپ =
        OT::Layout::GPOS_impl::Mark_ادھار_Pos_Format1_2<SmallTypes>::min_ناپ +
        MarkArray::min_ناپ +
        Anchor_میٹرکس::min_ناپ +
        c.گراف.vertices_[ادھار_coverage_id].جدول_ناپ ();

    TRY_سونپو (hb_سمتیہ_t<class_info_t> class_منزل_info, get_class_info (c, this_اشاریہ));

    بنانشانی class_گنتی = class_گنتی_2;
    TRY_سونپو (خودکار ادھار_array, c.گراف.as_جدول<Anchor_میٹرکس> (this_اشاریہ,
                                                          &ادھار_Array,
                                                          class_گنتی));
    بنانشانی ادھار_گنتی = ادھار_array.جدول->صفاں;

    بنانشانی partial_coverage_ناپ = 4;
    بنانشانی accumulated = ادھار_ناپ;
    hb_سمتیہ_t<بنانشانی> ونڈو_points;

    لكل (بنانشانی klass = 0; klass < class_گنتی; klass++)
    {
      class_info_t& info = class_منزل_info[klass];
      partial_coverage_ناپ += OT::HBUINT16::static_ناپ * info.marks.get_population ();
      بنانشانی accumulated_delta =
          OT::Layout::GPOS_impl::Mark_ریکارڈ<SmallTypes>::static_ناپ * info.marks.get_population () +
          OT::Offset16::static_ناپ * ادھار_گنتی;

      لكل (بنانشانی objidx : info.بچہ_indices)
        accumulated_delta += c.گراف.لبھو_subgraph_ناپ (objidx, ویکھیا).قدر_or (0);

      accumulated += accumulated_delta;
      بنانشانی کل = accumulated + partial_coverage_ناپ;

      إذا (کل >= (1 << 16))
      {
        ونڈو_points.push (klass);
        TRY (گراف_نتیجہ_t<فراغ>::from (ونڈو_points, ونڈ_FAILURE));
        accumulated = ادھار_ناپ + accumulated_delta;
        partial_coverage_ناپ = 4 + OT::HBUINT16::static_ناپ * info.marks.get_population ();
        ویکھیا.clear (); // node sharing isn't allowed between splits.
      }
    }


    TRY_سونپو (ثابت بنانشانی mark_array_id, c.گراف.اشاریہ_for_وتھ (this_اشاریہ, &markArray));
    TRY_سونپو (خودکار mark_array_links, c.گراف.vertices_[mark_array_id].position_منزل_اشاریہ_map ());

    ونڈو_context_t ونڈو_context {
      c,
      هذا,
      this_اشاریہ,
      std::move (class_منزل_info),
      std::move (mark_array_links),
    };

    أرجع actuate_subtable_ونڈو<ونڈو_context_t> (ونڈو_context, ونڈو_points);
  }

 خاص:

  بنية class_info_t {
    hb_سیٹ_t marks;
    hb_سمتیہ_t<بنانشانی> بچہ_indices;
  };

  بنية ونڈو_context_t {
    gsubgpos_گراف_context_t& c;
    Mark_ادھار_Pos_Format1* thiz;
    بنانشانی this_اشاریہ;
    hb_سمتیہ_t<class_info_t> class_منزل_info;
    hb_hashmap_t<بنانشانی, بنانشانی> mark_array_links;

    hb_سیٹ_t marks_for (بنانشانی شروع, بنانشانی سرا)
    {
      hb_سیٹ_t marks;
      لكل (بنانشانی klass = شروع; klass < سرا; klass++)
      {
        + class_منزل_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      أرجع marks;
    }

    بنانشانی original_گنتی_2 ()
    {
      أرجع thiz->class_گنتی_2;
    }

    گراف_نتیجہ_t<بنانشانی> clone_range (بنانشانی شروع, بنانشانی سرا)
    {
      أرجع thiz->clone_range (*هذا, هذا->this_اشاریہ, شروع, سرا);
    }

    گراف_نتیجہ_t<فراغ> shrink (بنانشانی گنتی)
    {
      أرجع thiz->shrink (*هذا, هذا->this_اشاریہ, گنتی);
    }
  };

  گراف_نتیجہ_t<hb_سمتیہ_t<class_info_t>> get_class_info (gsubgpos_گراف_context_t& c,
                                                            بنانشانی this_اشاریہ)
  {
    hb_سمتیہ_t<class_info_t> class_منزل_info;

    بنانشانی class_گنتی = class_گنتی_2;
    إذا (!class_گنتی) أرجع class_منزل_info;

    إذا (unlikely (!class_منزل_info.resize (class_گنتی)))
      أرجع Err(ونڈ_FAILURE);

    TRY_سونپو (خودکار mark_array, c.گراف.as_جدول<MarkArray> (this_اشاریہ, &markArray));

    بنانشانی mark_گنتی = mark_array.جدول->len;
    لكل (بنانشانی mark = 0; mark < mark_گنتی; mark++)
    {
      بنانشانی klass = (*mark_array.جدول)[mark].get_class ();
      إذا (klass >= class_گنتی) استمر;
      class_منزل_info[klass].marks.جوڑو (mark);
    }

    لكل (ثابت خودکار& link : mark_array.راس->obj ().real_links)
    {
      بنانشانی mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::Mark_ریکارڈ<SmallTypes>::static_ناپ;
      بنانشانی klass = (*mark_array.جدول)[mark].get_class ();
      إذا (klass >= class_گنتی) استمر;
      class_منزل_info[klass].بچہ_indices.push (link.objidx);
    }

    TRY_سونپو (بنانشانی ادھار_array_id,
         c.گراف.اشاریہ_for_وتھ (this_اشاریہ, &ادھار_Array));

    خودکار& ادھار_array_v = c.گراف.vertices_[ادھار_array_id];

    لكل (ثابت خودکار& link : ادھار_array_v.obj ().real_links)
    {
      بنانشانی اشاریہ_2 = (link.position - 2) / OT::Offset16::static_ناپ;
      بنانشانی klass = اشاریہ_2 % class_گنتی;
      class_منزل_info[klass].بچہ_indices.push (link.objidx);
    }

    أرجع class_منزل_info;
  }

  گراف_نتیجہ_t<فراغ> shrink (ونڈو_context_t& sc,
                               بنانشانی this_اشاریہ,
                               بنانشانی گنتی)
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_اشاریہ,
               گنتی);

    بنانشانی old_گنتی = class_گنتی_2;
    إذا (گنتی >= old_گنتی)
      أرجع Ok();

    class_گنتی_2 = گنتی;

    TRY_سونپو (خودکار mark_coverage, sc.c.گراف.as_mutable_جدول<Coverage> (this_اشاریہ,
                                                                    &markCoverage));

    hb_سیٹ_t marks = sc.marks_for (0, گنتی);
    خودکار new_coverage =
        + hb_enumerate (mark_coverage.جدول->iter ())
        | hb_filter (marks, hb_پہلا)
        | hb_map_retains_چھانٹی (hb_دوجا)
        ;
    TRY (Coverage::make_coverage (sc.c, + new_coverage,
                                  mark_coverage.اشاریہ_2,
                                  4 + 2 * marks.get_population ()));


    TRY_سونپو (خودکار ادھار_array, sc.c.گراف.as_mutable_جدول<Anchor_میٹرکس> (this_اشاریہ,
                                                                     &ادھار_Array,
                                                                     old_گنتی));

    TRY (ادھار_array.جدول->shrink (sc.c,
                                   ادھار_array.اشاریہ_2,
                                   old_گنتی,
                                   گنتی));

    TRY_سونپو (خودکار mark_array, sc.c.گراف.as_mutable_جدول<MarkArray> (this_اشاریہ,
                                                                  &markArray));

    TRY (mark_array.جدول->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.اشاریہ_2,
                                   گنتی));

    أرجع Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  گراف_نتیجہ_t<بنانشانی> clone_range (ونڈو_context_t& sc,
                                        بنانشانی this_اشاریہ,
                                        بنانشانی شروع, بنانشانی سرا) ثابت
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_اشاریہ, شروع, سرا);

    گراف_t& گراف = sc.c.گراف;
    بنانشانی مفرد_ناپ = OT::Layout::GPOS_impl::Mark_ادھار_Pos_Format1_2<SmallTypes>::static_ناپ;

    TRY_سونپو (بنانشانی مفرد_id, sc.c.create_گرہ (مفرد_ناپ));

    Mark_ادھار_Pos_Format1* مفرد = (Mark_ادھار_Pos_Format1*) گراف.object (مفرد_id).سر_2;
    مفرد->format = هذا->format;
    بنانشانی new_class_گنتی = سرا - شروع;
    مفرد->class_گنتی_2 = new_class_گنتی;

    TRY_سونپو (بنانشانی ادھار_coverage_id,
         گراف.اشاریہ_for_وتھ (sc.this_اشاریہ, &ادھار_Coverage));

    TRY (گراف.جوڑو_link (&(مفرد->ادھار_Coverage), مفرد_id, ادھار_coverage_id));

    TRY_سونپو (خودکار mark_coverage, sc.c.گراف.as_جدول<Coverage> (this_اشاریہ,
                                                            &markCoverage));

    hb_سیٹ_t marks = sc.marks_for (شروع, سرا);
    خودکار new_coverage =
        + hb_enumerate (mark_coverage.جدول->iter ())
        | hb_filter (marks, hb_پہلا)
        | hb_map_retains_چھانٹی (hb_دوجا)
        ;
    TRY (Coverage::جوڑو_coverage (sc.c,
                                 مفرد_id,
                                 2,
                                 + new_coverage,
                                 marks.get_population () * 2 + 4));

    TRY_سونپو (خودکار mark_array,
         گراف.as_mutable_جدول <MarkArray> (sc.this_اشاریہ, &markArray));

    TRY_سونپو (بنانشانی new_mark_array,
         mark_array.جدول->clone (sc.c,
                                  mark_array.اشاریہ_2,
                                  sc.mark_array_links,
                                  marks,
                                  شروع));
    TRY (گراف.جوڑو_link (&(مفرد->markArray), مفرد_id, new_mark_array));

    بنانشانی class_گنتی = class_گنتی_2;
    TRY_سونپو (خودکار ادھار_array,
         گراف.as_mutable_جدول<Anchor_میٹرکس> (sc.this_اشاریہ, &ادھار_Array, class_گنتی));

    TRY_سونپو (بنانشانی new_ادھار_array,
         ادھار_array.جدول->clone (sc.c,
                                  ادھار_array.اشاریہ_2,
                                  شروع, سرا, هذا->class_گنتی_2));
    TRY (گراف.جوڑو_link (&(مفرد->ادھار_Array), مفرد_id, new_ادھار_array));

    أرجع Ok(مفرد_id);
  }
};


بنية Mark_ادھار_Pos : عام OT::Layout::GPOS_impl::Mark_ادھار_Pos
{
  گراف_نتیجہ_t<hb_سمتیہ_t<بنانشانی>> ونڈو_subtables (gsubgpos_گراف_context_t& c,
                                                         بنانشانی this_اشاریہ)
  {
    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((Mark_ادھار_Pos_Format1*)(&u.format1))->ونڈو_subtables (c, this_اشاریہ);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
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
      أرجع ((Mark_ادھار_Pos_Format1*)(&u.format1))->sanitize (راس);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 2: HB_FALLTHROUGH;
#جے_انت
    افتراضي:
      // We don't handle format 3 and 4 here.
      أرجع Err(SANITIZE_FAILURE);
    }
  }
};


}

#جے_انت  // GRAPH_MARKBASEPOS_GRAPH_HH
