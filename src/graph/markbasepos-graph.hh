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

#جيڪڏهن_وصف_نه_ٿيل گراف_MARKBASEPOS_گراف_HH
#وصف_ڪريو گراف_MARKBASEPOS_گراف_HH

#ضمن "split-helpers.hh"
#ضمن "coverage-graph.hh"
#ضمن "../OT/Layout/GPOS/MarkBasePos.hh"
#ضمن "../OT/Layout/GPOS/PosLookupSubTable.hh"

نطاق گراف {

بنية Anchor_ميٽرڪس : عام OT::Layout::GPOS_impl::Anchor_ميٽرڪس<SmallTypes>
{
  گراف_نتيجو_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس, بنانشان class_ڳڻپ) ثابت
  {
    size_t راس_len = راس.جدول_سائيز();
    إذا (unlikely (راس_len < Anchor_ميٽرڪس::min_سائيز)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (راس_len < Anchor_ميٽرڪس::min_سائيز +
                  OT::Offset16::static_سائيز * class_ڳڻپ * هذا->صفون))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتيجو_t<فراغ> shrink (gsubgpos_گراف_context_t& c,
                               بنانشان this_اشاريو,
                               بنانشان old_class_ڳڻپ,
                               بنانشان new_class_ڳڻپ)
  {
    إذا (unlikely (new_class_ڳڻپ >= old_class_ڳڻپ)) أرجع Err(INVALID_دليل);
    خودڪار& v = c.گراف.vertices_[this_اشاريو];
    ثابت خودڪار& o = v.obj ();
    بنانشان بنياد_ڳڻپ = صفون;
    v.سيٽ_بفر (o.سر_2,
      o.سر_2 +
      Anchor_ميٽرڪس::min_سائيز +
      OT::Offset16::static_سائيز * بنياد_ڳڻپ * new_class_ڳڻپ);

    // Reposition links into the new indexing scheme.
    لكل (خودڪار& link : v.real_links_writer ())
    {
      بنانشان اشاريو_2 = (link.position - 2) / 2;
      بنانشان بنياد = اشاريو_2 / old_class_ڳڻپ;
      بنانشان klass = اشاريو_2 % old_class_ڳڻپ;
      إذا (unlikely (klass >= new_class_ڳڻپ))
        // should have already been removed
        أرجع Err(INVALID_دليل);

      بنانشان new_اشاريو = بنياد * new_class_ڳڻپ + klass;

      link.position = (حرف*) &(هذا->ميٽرڪس_Z[new_اشاريو]) - (حرف*) هذا;
    }

    أرجع Ok();
  }

  گراف_نتيجو_t<بنانشان> clone (gsubgpos_گراف_context_t& c,
                                  بنانشان this_اشاريو,
                                  بنانشان شروعات,
                                  بنانشان پڇاڙي,
                                  بنانشان class_ڳڻپ)
  {
    بنانشان بنياد_ڳڻپ = صفون;
    بنانشان new_class_ڳڻپ = پڇاڙي - شروعات;
    بنانشان سائيز = Anchor_ميٽرڪس::min_سائيز +
                    OT::Offset16::static_سائيز * new_class_ڳڻپ * صفون;
    TRY_سونپيو (بنانشان مفرد_id, c.create_ڳنڍ (سائيز));
    Anchor_ميٽرڪس* مفرد = (Anchor_ميٽرڪس*) c.گراف.object (مفرد_id).سر_2;
    مفرد->صفون = بنياد_ڳڻپ;

    خودڪار& v = c.گراف.vertices_[this_اشاريو];
    ثابت خودڪار& o = v.obj ();

    صحيح انگ_links = o.real_links.ڊيگهائي;
    لكل (صحيح i = 0; i < انگ_links; i++)
    {
      ثابت خودڪار& link = o.real_links[i];
      بنانشان old_اشاريو = (link.position - 2) / OT::Offset16::static_سائيز;
      بنانشان klass = old_اشاريو % class_ڳڻپ;
      إذا (klass < شروعات || klass >= پڇاڙي) استمر;

      بنانشان بنياد = old_اشاريو / class_ڳڻپ;
      بنانشان new_klass = klass - شروعات;
      بنانشان new_اشاريو = بنياد * new_class_ڳڻپ + new_klass;


      بنانشان ٻار_idx = link.objidx;
      TRY (c.گراف.جوڙيو_link (&(مفرد->ميٽرڪس_Z[new_اشاريو]),
                             مفرد_id,
                             ٻار_idx));

      خودڪار& ٻار = c.گراف.vertices_[ٻار_idx];
      ٻار.remove_والد (this_اشاريو);

      v.remove_real_link_unordered (i);
      انگ_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.ترتيب_ڏيو_real_links ();

    أرجع Ok(مفرد_id);
  }
};

بنية MarkArray : عام OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  گراف_نتيجو_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائيز();
    بنانشان min_سائيز = MarkArray::min_سائيز;
    إذا (unlikely (راس_len < min_سائيز)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (راس_len < get_سائيز ())) أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتيجو_t<فراغ> shrink (gsubgpos_گراف_context_t& c,
                               ثابت hb_hashmap_t<بنانشان, بنانشان>& mark_array_links,
                               بنانشان this_اشاريو,
                               بنانشان new_class_ڳڻپ)
  {
    خودڪار& v = c.گراف.vertices_[this_اشاريو];
    ثابت خودڪار& o = v.obj ();
    لكل (ثابت خودڪار& link : o.real_links)
      c.گراف.vertices_[link.objidx].remove_والد (this_اشاريو);
    v.clear_real_links ();

    بنانشان new_اشاريو = 0;
    لكل (ثابت خودڪار& رڪارڊ : هذا->iter ())
    {
      بنانشان klass = رڪارڊ.klass;
      إذا (klass >= new_class_ڳڻپ) استمر;

      (*هذا)[new_اشاريو].klass = klass;
      بنانشان position = (حرف*) &رڪارڊ.markAnchor - (حرف*) هذا;
      بنانشان* objidx;
      إذا (!mark_array_links.has (position, &objidx))
      {
        new_اشاريو++;
        استمر;
      }

      TRY (c.گراف.جوڙيو_link (&(*هذا)[new_اشاريو].markAnchor, this_اشاريو, *objidx));
      new_اشاريو++;
    }

    هذا->len = new_اشاريو;
    v.سيٽ_بفر (o.سر_2, o.سر_2 + MarkArray::min_سائيز +
                  OT::Layout::GPOS_impl::Mark_رڪارڊ<SmallTypes>::static_سائيز * new_اشاريو);

    أرجع Ok();
  }

  گراف_نتيجو_t<بنانشان> clone (gsubgpos_گراف_context_t& c,
                                  بنانشان this_اشاريو,
                                  ثابت hb_hashmap_t<بنانشان, بنانشان>& pos_منزل_اشاريو,
                                  hb_سيٽ_t& marks,
                                  بنانشان شروعات_class)
  {
    بنانشان سائيز = MarkArray::min_سائيز +
                    OT::Layout::GPOS_impl::Mark_رڪارڊ<SmallTypes>::static_سائيز *
                    marks.get_population ();
    TRY_سونپيو (بنانشان مفرد_id, c.create_ڳنڍ (سائيز));
    MarkArray* مفرد = (MarkArray*) c.گراف.object (مفرد_id).سر_2;
    مفرد->len = marks.get_population ();


    بنانشان i = 0;
    لكل (hb_codepoint_t mark : marks)
    {
      (*مفرد)[i].klass = (*هذا)[mark].klass - شروعات_class;
      بنانشان وٿي_pos = (حرف*) &((*هذا)[mark].markAnchor) - (حرف*) هذا;
      بنانشان* anchor_اشاريو;
      إذا (pos_منزل_اشاريو.has (وٿي_pos, &anchor_اشاريو))
        TRY (c.گراف.move_ٻار (this_اشاريو,
                                 &((*هذا)[mark].markAnchor),
                                 مفرد_id,
                                 &((*مفرد)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.گراف.vertices_[this_اشاريو].ترتيب_ڏيو_real_links ();

    أرجع Ok(مفرد_id);
  }
};

بنية Mark_بنياد_Pos_Format1 : عام OT::Layout::GPOS_impl::Mark_بنياد_Pos_Format1_2<SmallTypes>
{
  گراف_نتيجو_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائيز ();
    إذا (unlikely (راس_len < Mark_بنياد_Pos_Format1::static_سائيز))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتيجو_t<hb_ويڪٽر_t<بنانشان>> ورهايو_subtables (gsubgpos_گراف_context_t& c,
                                                         بنانشان this_اشاريو)
  {
    hb_سيٽ_t گهميل;

    TRY_سونپيو (ثابت بنانشان بنياد_coverage_id, c.گراف.اشاريو_for_وٿي (this_اشاريو, &بنياد_Coverage));

    ثابت بنانشان بنياد_سائيز =
        OT::Layout::GPOS_impl::Mark_بنياد_Pos_Format1_2<SmallTypes>::min_سائيز +
        MarkArray::min_سائيز +
        Anchor_ميٽرڪس::min_سائيز +
        c.گراف.vertices_[بنياد_coverage_id].جدول_سائيز ();

    TRY_سونپيو (hb_ويڪٽر_t<class_info_t> class_منزل_info, get_class_info (c, this_اشاريو));

    بنانشان class_ڳڻپ = class_ڳڻپ_2;
    TRY_سونپيو (خودڪار بنياد_array, c.گراف.as_جدول<Anchor_ميٽرڪس> (this_اشاريو,
                                                          &بنياد_Array,
                                                          class_ڳڻپ));
    بنانشان بنياد_ڳڻپ = بنياد_array.جدول->صفون;

    بنانشان partial_coverage_سائيز = 4;
    بنانشان accumulated = بنياد_سائيز;
    hb_ويڪٽر_t<بنانشان> ورهايو_points;

    لكل (بنانشان klass = 0; klass < class_ڳڻپ; klass++)
    {
      class_info_t& info = class_منزل_info[klass];
      partial_coverage_سائيز += OT::HBUINT16::static_سائيز * info.marks.get_population ();
      بنانشان accumulated_delta =
          OT::Layout::GPOS_impl::Mark_رڪارڊ<SmallTypes>::static_سائيز * info.marks.get_population () +
          OT::Offset16::static_سائيز * بنياد_ڳڻپ;

      لكل (بنانشان objidx : info.ٻار_indices)
        accumulated_delta += c.گراف.ڳوليو_subgraph_سائيز (objidx, گهميل).قدر_or (0);

      accumulated += accumulated_delta;
      بنانشان ڪل = accumulated + partial_coverage_سائيز;

      إذا (ڪل >= (1 << 16))
      {
        ورهايو_points.push (klass);
        TRY (گراف_نتيجو_t<فراغ>::from (ورهايو_points, ورهاست_FAILURE));
        accumulated = بنياد_سائيز + accumulated_delta;
        partial_coverage_سائيز = 4 + OT::HBUINT16::static_سائيز * info.marks.get_population ();
        گهميل.clear (); // node sharing isn't allowed between splits.
      }
    }


    TRY_سونپيو (ثابت بنانشان mark_array_id, c.گراف.اشاريو_for_وٿي (this_اشاريو, &markArray));
    TRY_سونپيو (خودڪار mark_array_links, c.گراف.vertices_[mark_array_id].position_منزل_اشاريو_map ());

    ورهايو_context_t ورهايو_context {
      c,
      هذا,
      this_اشاريو,
      std::move (class_منزل_info),
      std::move (mark_array_links),
    };

    أرجع actuate_subtable_ورهايو<ورهايو_context_t> (ورهايو_context, ورهايو_points);
  }

 خاص:

  بنية class_info_t {
    hb_سيٽ_t marks;
    hb_ويڪٽر_t<بنانشان> ٻار_indices;
  };

  بنية ورهايو_context_t {
    gsubgpos_گراف_context_t& c;
    Mark_بنياد_Pos_Format1* thiz;
    بنانشان this_اشاريو;
    hb_ويڪٽر_t<class_info_t> class_منزل_info;
    hb_hashmap_t<بنانشان, بنانشان> mark_array_links;

    hb_سيٽ_t marks_for (بنانشان شروعات, بنانشان پڇاڙي)
    {
      hb_سيٽ_t marks;
      لكل (بنانشان klass = شروعات; klass < پڇاڙي; klass++)
      {
        + class_منزل_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      أرجع marks;
    }

    بنانشان original_ڳڻپ_2 ()
    {
      أرجع thiz->class_ڳڻپ_2;
    }

    گراف_نتيجو_t<بنانشان> clone_range (بنانشان شروعات, بنانشان پڇاڙي)
    {
      أرجع thiz->clone_range (*هذا, هذا->this_اشاريو, شروعات, پڇاڙي);
    }

    گراف_نتيجو_t<فراغ> shrink (بنانشان ڳڻپ)
    {
      أرجع thiz->shrink (*هذا, هذا->this_اشاريو, ڳڻپ);
    }
  };

  گراف_نتيجو_t<hb_ويڪٽر_t<class_info_t>> get_class_info (gsubgpos_گراف_context_t& c,
                                                            بنانشان this_اشاريو)
  {
    hb_ويڪٽر_t<class_info_t> class_منزل_info;

    بنانشان class_ڳڻپ = class_ڳڻپ_2;
    إذا (!class_ڳڻپ) أرجع class_منزل_info;

    إذا (unlikely (!class_منزل_info.resize (class_ڳڻپ)))
      أرجع Err(ورهاست_FAILURE);

    TRY_سونپيو (خودڪار mark_array, c.گراف.as_جدول<MarkArray> (this_اشاريو, &markArray));

    بنانشان mark_ڳڻپ = mark_array.جدول->len;
    لكل (بنانشان mark = 0; mark < mark_ڳڻپ; mark++)
    {
      بنانشان klass = (*mark_array.جدول)[mark].get_class ();
      إذا (klass >= class_ڳڻپ) استمر;
      class_منزل_info[klass].marks.جوڙيو (mark);
    }

    لكل (ثابت خودڪار& link : mark_array.راس->obj ().real_links)
    {
      بنانشان mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::Mark_رڪارڊ<SmallTypes>::static_سائيز;
      بنانشان klass = (*mark_array.جدول)[mark].get_class ();
      إذا (klass >= class_ڳڻپ) استمر;
      class_منزل_info[klass].ٻار_indices.push (link.objidx);
    }

    TRY_سونپيو (بنانشان بنياد_array_id,
         c.گراف.اشاريو_for_وٿي (this_اشاريو, &بنياد_Array));

    خودڪار& بنياد_array_v = c.گراف.vertices_[بنياد_array_id];

    لكل (ثابت خودڪار& link : بنياد_array_v.obj ().real_links)
    {
      بنانشان اشاريو_2 = (link.position - 2) / OT::Offset16::static_سائيز;
      بنانشان klass = اشاريو_2 % class_ڳڻپ;
      class_منزل_info[klass].ٻار_indices.push (link.objidx);
    }

    أرجع class_منزل_info;
  }

  گراف_نتيجو_t<فراغ> shrink (ورهايو_context_t& sc,
                               بنانشان this_اشاريو,
                               بنانشان ڳڻپ)
  {
    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_اشاريو,
               ڳڻپ);

    بنانشان old_ڳڻپ = class_ڳڻپ_2;
    إذا (ڳڻپ >= old_ڳڻپ)
      أرجع Ok();

    class_ڳڻپ_2 = ڳڻپ;

    TRY_سونپيو (خودڪار mark_coverage, sc.c.گراف.as_mutable_جدول<Coverage> (this_اشاريو,
                                                                    &markCoverage));

    hb_سيٽ_t marks = sc.marks_for (0, ڳڻپ);
    خودڪار new_coverage =
        + hb_enumerate (mark_coverage.جدول->iter ())
        | hb_filter (marks, hb_پهريون)
        | hb_map_retains_ترتيب_ڏيڻ (hb_ٻيو)
        ;
    TRY (Coverage::make_coverage (sc.c, + new_coverage,
                                  mark_coverage.اشاريو_2,
                                  4 + 2 * marks.get_population ()));


    TRY_سونپيو (خودڪار بنياد_array, sc.c.گراف.as_mutable_جدول<Anchor_ميٽرڪس> (this_اشاريو,
                                                                     &بنياد_Array,
                                                                     old_ڳڻپ));

    TRY (بنياد_array.جدول->shrink (sc.c,
                                   بنياد_array.اشاريو_2,
                                   old_ڳڻپ,
                                   ڳڻپ));

    TRY_سونپيو (خودڪار mark_array, sc.c.گراف.as_mutable_جدول<MarkArray> (this_اشاريو,
                                                                  &markArray));

    TRY (mark_array.جدول->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.اشاريو_2,
                                   ڳڻپ));

    أرجع Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  گراف_نتيجو_t<بنانشان> clone_range (ورهايو_context_t& sc,
                                        بنانشان this_اشاريو,
                                        بنانشان شروعات, بنانشان پڇاڙي) ثابت
  {
    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_اشاريو, شروعات, پڇاڙي);

    گراف_t& گراف = sc.c.گراف;
    بنانشان مفرد_سائيز = OT::Layout::GPOS_impl::Mark_بنياد_Pos_Format1_2<SmallTypes>::static_سائيز;

    TRY_سونپيو (بنانشان مفرد_id, sc.c.create_ڳنڍ (مفرد_سائيز));

    Mark_بنياد_Pos_Format1* مفرد = (Mark_بنياد_Pos_Format1*) گراف.object (مفرد_id).سر_2;
    مفرد->format = هذا->format;
    بنانشان new_class_ڳڻپ = پڇاڙي - شروعات;
    مفرد->class_ڳڻپ_2 = new_class_ڳڻپ;

    TRY_سونپيو (بنانشان بنياد_coverage_id,
         گراف.اشاريو_for_وٿي (sc.this_اشاريو, &بنياد_Coverage));

    TRY (گراف.جوڙيو_link (&(مفرد->بنياد_Coverage), مفرد_id, بنياد_coverage_id));

    TRY_سونپيو (خودڪار mark_coverage, sc.c.گراف.as_جدول<Coverage> (this_اشاريو,
                                                            &markCoverage));

    hb_سيٽ_t marks = sc.marks_for (شروعات, پڇاڙي);
    خودڪار new_coverage =
        + hb_enumerate (mark_coverage.جدول->iter ())
        | hb_filter (marks, hb_پهريون)
        | hb_map_retains_ترتيب_ڏيڻ (hb_ٻيو)
        ;
    TRY (Coverage::جوڙيو_coverage (sc.c,
                                 مفرد_id,
                                 2,
                                 + new_coverage,
                                 marks.get_population () * 2 + 4));

    TRY_سونپيو (خودڪار mark_array,
         گراف.as_mutable_جدول <MarkArray> (sc.this_اشاريو, &markArray));

    TRY_سونپيو (بنانشان new_mark_array,
         mark_array.جدول->clone (sc.c,
                                  mark_array.اشاريو_2,
                                  sc.mark_array_links,
                                  marks,
                                  شروعات));
    TRY (گراف.جوڙيو_link (&(مفرد->markArray), مفرد_id, new_mark_array));

    بنانشان class_ڳڻپ = class_ڳڻپ_2;
    TRY_سونپيو (خودڪار بنياد_array,
         گراف.as_mutable_جدول<Anchor_ميٽرڪس> (sc.this_اشاريو, &بنياد_Array, class_ڳڻپ));

    TRY_سونپيو (بنانشان new_بنياد_array,
         بنياد_array.جدول->clone (sc.c,
                                  بنياد_array.اشاريو_2,
                                  شروعات, پڇاڙي, هذا->class_ڳڻپ_2));
    TRY (گراف.جوڙيو_link (&(مفرد->بنياد_Array), مفرد_id, new_بنياد_array));

    أرجع Ok(مفرد_id);
  }
};


بنية Mark_بنياد_Pos : عام OT::Layout::GPOS_impl::Mark_بنياد_Pos
{
  گراف_نتيجو_t<hb_ويڪٽر_t<بنانشان>> ورهايو_subtables (gsubgpos_گراف_context_t& c,
                                                         بنانشان this_اشاريو)
  {
    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((Mark_بنياد_Pos_Format1*)(&u.format1))->ورهايو_subtables (c, this_اشاريو);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
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
      أرجع ((Mark_بنياد_Pos_Format1*)(&u.format1))->sanitize (راس);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 2: HB_FALLTHROUGH;
#جيڪڏهن_پورو
    افتراضي:
      // We don't handle format 3 and 4 here.
      أرجع Err(SANITIZE_FAILURE);
    }
  }
};


}

#جيڪڏهن_پورو  // GRAPH_MARKBASEPOS_GRAPH_HH
