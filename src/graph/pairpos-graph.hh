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

#که_نه_تعریف_شوی ګراف_PAIRPOS_ګراف_HH
#تعریف_کړه ګراف_PAIRPOS_ګراف_HH

#ضمن "split-helpers.hh"
#ضمن "coverage-graph.hh"
#ضمن "classdef-graph.hh"
#ضمن "../OT/Layout/GPOS/PairPos.hh"
#ضمن "../OT/Layout/GPOS/PosLookupSubTable.hh"

نطاق ګراف {

بنية PairPosFormat1 : عام OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  ګراف_پایله_t<فراغ> sanitize (ثابت ګراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_اندازه ();
    بینښه min_اندازه = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_اندازه;
    إذا (unlikely (راس_len < min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (راس_len < min_اندازه + pair_سېټ.get_اندازه () - pair_سېټ.len.get_اندازه()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  ګراف_پایله_t<hb_ویکټور_t<بینښه>> وېشل_subtables (gsubgpos_ګراف_context_t& c,
                                                         بینښه this_شاخص)
  {
    hb_سېټ_t لیدل_شوی;

    TRY_ټاکل (ثابت بینښه coverage_id, c.ګراف.شاخص_for_دننه_واټن (this_شاخص, &coverage));
    ثابت بینښه coverage_اندازه = c.ګراف.vertices_[coverage_id].جدول_اندازه ();
    ثابت بینښه بنسټ_اندازه = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_اندازه;

    بینښه partial_coverage_اندازه = 4;
    بینښه accumulated = بنسټ_اندازه;
    hb_ویکټور_t<بینښه> وېشل_points;
    لكل (بینښه i = 0; i < pair_سېټ.len; i++)
    {
      TRY_ټاکل (بینښه pair_سېټ_شاخص, pair_سېټ_ګراف_شاخص (c, this_شاخص, i));
      بینښه accumulated_delta =
          c.ګراف.پیدا_کړه_subgraph_اندازه (pair_سېټ_شاخص, لیدل_شوی).ارزښت_or (0) +
          SmallTypes::اندازه; // for PairSet offset.
      partial_coverage_اندازه += OT::HBUINT16::static_اندازه;

      accumulated += accumulated_delta;
      بینښه ټول = accumulated + hb_min (partial_coverage_اندازه, coverage_اندازه);

      إذا (ټول >= (1 << 16))
      {
        وېشل_points.push (i);
        TRY (ګراف_پایله_t<فراغ>::from (وېشل_points, ویش_FAILURE));
        accumulated = بنسټ_اندازه + accumulated_delta;
        partial_coverage_اندازه = 6;
        لیدل_شوی.clear (); // node sharing isn't allowed between splits.
      }
    }

    وېشل_context_t وېشل_context {
      c,
      هذا,
      this_شاخص,
    };

    أرجع actuate_subtable_وېشل<وېشل_context_t> (وېشل_context, وېشل_points);
  }

 خاص:

  بنية وېشل_context_t {
    gsubgpos_ګراف_context_t& c;
    PairPosFormat1* thiz;
    بینښه this_شاخص;

    بینښه original_شمېر_2 ()
    {
      أرجع thiz->pair_سېټ.len;
    }

    ګراف_پایله_t<بینښه> clone_range (بینښه پیل, بینښه پای_ټکی)
    {
      أرجع thiz->clone_range (هذا->c, هذا->this_شاخص, پیل, پای_ټکی);
    }

    ګراف_پایله_t<فراغ> shrink (بینښه شمېر)
    {
      أرجع thiz->shrink (هذا->c, هذا->this_شاخص, شمېر);
    }
  };

  ګراف_پایله_t<فراغ> shrink (gsubgpos_ګراف_context_t& c,
                               بینښه this_شاخص,
                               بینښه شمېر)
  {
    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_شاخص,
               شمېر);
    بینښه old_شمېر = pair_سېټ.len;
    إذا (شمېر >= old_شمېر)
      أرجع Ok();

    pair_سېټ.len = شمېر;
    c.ګراف.vertices_[this_شاخص].shrink_بفر ((old_شمېر - شمېر) * SmallTypes::اندازه);

    TRY_ټاکل (خپلکاری coverage, c.ګراف.as_mutable_جدول<Coverage> (this_شاخص, &هذا->coverage));

    بینښه coverage_اندازه = coverage.راس->جدول_اندازه ();
    خپلکاری new_coverage =
        + hb_zip (coverage.جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<بینښه, بینښه> p) {
          أرجع p.second < شمېر;
        })
        | hb_map_retains_ترتیبونه (hb_لومړی)
        ;

    أرجع Coverage::make_coverage (c, new_coverage, coverage.شاخص_2, coverage_اندازه);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  ګراف_پایله_t<بینښه> clone_range (gsubgpos_ګراف_context_t& c,
                                        بینښه this_شاخص,
                                        بینښه پیل, بینښه پای_ټکی) ثابت
  {
    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_شاخص, پیل, پای_ټکی);

    بینښه عدد_pair_sets = پای_ټکی - پیل;
    بینښه اولي_اندازه = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_اندازه
                          + عدد_pair_sets * SmallTypes::اندازه;

    TRY_ټاکل (بینښه pair_pos_اولي_id, c.create_غوټه (اولي_اندازه));

    PairPosFormat1* pair_pos_اولي = (PairPosFormat1*) c.ګراف.object (pair_pos_اولي_id).سر_2;
    pair_pos_اولي->format = هذا->format;
    pair_pos_اولي->ارزښت_Format_2[0] = هذا->ارزښت_Format_2[0];
    pair_pos_اولي->ارزښت_Format_2[1] = هذا->ارزښت_Format_2[1];
    pair_pos_اولي->pair_سېټ.len = عدد_pair_sets;

    لكل (بینښه i = پیل; i < پای_ټکی; i++)
    {
      TRY (c.ګراف.move_ماشوم<> (this_شاخص,
                                 &pair_سېټ[i],
                                 pair_pos_اولي_id,
                                 &pair_pos_اولي->pair_سېټ[i - پیل]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.ګراف.vertices_[this_شاخص].ترتیبول_real_links ();

    TRY_ټاکل (بینښه coverage_id, c.ګراف.شاخص_for_دننه_واټن (this_شاخص, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_اولي_id,
                                   2,
                                   پیل, پای_ټکی));

    أرجع Ok(pair_pos_اولي_id);
  }



  ګراف_پایله_t<بینښه> pair_سېټ_ګراف_شاخص (gsubgpos_ګراف_context_t& c, بینښه this_شاخص, بینښه i) ثابت
  {
    أرجع c.ګراف.شاخص_for_دننه_واټن (this_شاخص, &pair_سېټ[i]);
  }
};

بنية PairPosFormat2 : عام OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  ګراف_پایله_t<فراغ> sanitize (ثابت ګراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_اندازه ();
    بینښه min_اندازه = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_اندازه;
    إذا (unlikely (راس_len < min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    ثابت بینښه class1_شمېر = class1_شمېر_2;
    إذا (unlikely (راس_len < min_اندازه + class1_شمېر * get_class1_ریکارډ_اندازه ()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  ګراف_پایله_t<hb_ویکټور_t<بینښه>> وېشل_subtables (gsubgpos_ګراف_context_t& c,
                                                         بینښه this_شاخص)
  {
    ثابت بینښه بنسټ_اندازه = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_اندازه;
    TRY_ټاکل (ثابت بینښه class_def_2_اندازه, اندازه_د (c, this_شاخص, &classDef2));
    TRY_ټاکل (ثابت Coverage* coverage, get_coverage (c, this_شاخص));
    TRY_ټاکل (ثابت ClassDef* class_def_1, get_class_def_1 (c, this_شاخص));
    خپلکاری gid_and_class =
        + coverage->iter ()
        | hb_map_retains_ترتیبونه ([&] (hb_codepoint_t gid) {
          أرجع hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_ټاکل (class_def_اندازه_estimator_t estimator, class_def_اندازه_estimator_t::create (gid_and_class));

    ثابت بینښه class1_شمېر = class1_شمېر_2;
    ثابت بینښه class2_شمېر = class2_شمېر_2;
    ثابت بینښه class1_ریکارډ_اندازه = get_class1_ریکارډ_اندازه ();

    ثابت بینښه ارزښت_1_len = ارزښت_Format1.get_len ();
    ثابت بینښه ارزښت_2_len = ارزښت_Format2.get_len ();
    ثابت بینښه ټول_ارزښت_len = ارزښت_1_len + ارزښت_2_len;

    بینښه accumulated = بنسټ_اندازه;
    بینښه coverage_اندازه = 4;
    بینښه class_def_1_اندازه = 4;
    بینښه max_coverage_اندازه = coverage_اندازه;
    بینښه max_class_def_1_اندازه = class_def_1_اندازه;

    hb_ویکټور_t<بینښه> وېشل_points;

    TRY_ټاکل (خپلکاری device_tables, get_all_device_tables (c, this_شاخص));
    hb_ویکټور_t<بینښه> format1_device_جدول_indices = ارزښت_Format1.get_device_جدول_indices ();
    hb_ویکټور_t<بینښه> format2_device_جدول_indices = ارزښت_Format2.get_device_جدول_indices ();
    منطقي has_device_tables = منطقي(format1_device_جدول_indices) || منطقي(format2_device_جدول_indices);

    hb_سېټ_t لیدل_شوی;
    لكل (بینښه i = 0; i < class1_شمېر; i++)
    {
      بینښه accumulated_delta = class1_ریکارډ_اندازه;
      class_def_1_اندازه = estimator.جمع_کړه_class_def_اندازه (i);
      coverage_اندازه = estimator.coverage_اندازه ();
      max_coverage_اندازه = hb_max (max_coverage_اندازه, coverage_اندازه);
      max_class_def_1_اندازه = hb_max (max_class_def_1_اندازه, class_def_1_اندازه);

      إذا (has_device_tables) {
        لكل (بینښه j = 0; j < class2_شمېر; j++)
        {
          بینښه value1_شاخص = ټول_ارزښت_len * (class2_شمېر * i + j);
          بینښه value2_شاخص = value1_شاخص + ارزښت_1_len;
          accumulated_delta += اندازه_د_ارزښت_ریکارډ_children (c,
                                                        device_tables,
                                                        format1_device_جدول_indices,
                                                        value1_شاخص,
                                                        لیدل_شوی);
          accumulated_delta += اندازه_د_ارزښت_ریکارډ_children (c,
                                                        device_tables,
                                                        format2_device_جدول_indices,
                                                        value2_شاخص,
                                                        لیدل_شوی);
        }
      }

      accumulated += accumulated_delta;
      بینښه ټول = accumulated
                       + coverage_اندازه + class_def_1_اندازه + class_def_2_اندازه
                       // The largest object will pack last and can exceed the size limit.
                       - hb_max (hb_max (coverage_اندازه, class_def_1_اندازه), class_def_2_اندازه);
      إذا (ټول >= (1 << 16))
      {
        وېشل_points.push (i);
        TRY (ګراف_پایله_t<فراغ>::from (وېشل_points, ویش_FAILURE));
        // split does not include i, so add the size for i when we reset the size counters.
        accumulated = بنسټ_اندازه + accumulated_delta;

        estimator.reset();
        class_def_1_اندازه = estimator.جمع_کړه_class_def_اندازه(i);
        coverage_اندازه = estimator.coverage_اندازه();
        لیدل_شوی.clear (); // node sharing isn't allowed between splits.
      }
    }

    وېشل_context_t وېشل_context {
      c,
      هذا,
      this_شاخص,
      class1_ریکارډ_اندازه,
      ټول_ارزښت_len,
      ارزښت_1_len,
      ارزښت_2_len,
      max_coverage_اندازه,
      max_class_def_1_اندازه,
      device_tables,
      format1_device_جدول_indices,
      format2_device_جدول_indices
    };

    أرجع actuate_subtable_وېشل<وېشل_context_t> (وېشل_context, وېشل_points);
  }
 خاص:

  بنية وېشل_context_t
  {
    gsubgpos_ګراف_context_t& c;
    PairPosFormat2* thiz;
    بینښه this_شاخص;
    بینښه class1_ریکارډ_اندازه;
    بینښه ارزښت_ریکارډ_len;
    بینښه value1_ریکارډ_len;
    بینښه value2_ریکارډ_len;
    بینښه max_coverage_اندازه;
    بینښه max_class_def_اندازه;

    ثابت hb_hashmap_t<بینښه, بینښه>& device_tables;
    ثابت hb_ویکټور_t<بینښه>& format1_device_جدول_indices;
    ثابت hb_ویکټور_t<بینښه>& format2_device_جدول_indices;

    بینښه original_شمېر_2 ()
    {
      أرجع thiz->class1_شمېر_2;
    }

    ګراف_پایله_t<بینښه> clone_range (بینښه پیل, بینښه پای_ټکی)
    {
      أرجع thiz->clone_range (*هذا, پیل, پای_ټکی);
    }

    ګراف_پایله_t<فراغ> shrink (بینښه شمېر)
    {
      أرجع thiz->shrink (*هذا, شمېر);
    }
  };

  size_t get_class1_ریکارډ_اندازه () ثابت
  {
    ثابت size_t class2_شمېر = class2_شمېر_2;
    أرجع
        class2_شمېر * (ارزښت_Format1.get_اندازه () + ارزښت_Format2.get_اندازه ());
  }

  ګراف_پایله_t<بینښه> clone_range (وېشل_context_t& وېشل_context,
                                        بینښه پیل, بینښه پای_ټکی) ثابت
  {
    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", وېشل_context.this_شاخص, پیل, پای_ټکی);

    ګراف_t& ګراف = وېشل_context.c.ګراف;

    بینښه عدد_ریکارډونه = پای_ټکی - پیل;
    بینښه اولي_اندازه = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_اندازه
                          + عدد_ریکارډونه * وېشل_context.class1_ریکارډ_اندازه;

    TRY_ټاکل (بینښه pair_pos_اولي_id, وېشل_context.c.create_غوټه (اولي_اندازه));

    PairPosFormat2* pair_pos_اولي =
        (PairPosFormat2*) ګراف.object (pair_pos_اولي_id).سر_2;
    pair_pos_اولي->format = هذا->format;
    pair_pos_اولي->ارزښت_Format1 = هذا->ارزښت_Format1;
    pair_pos_اولي->ارزښت_Format2 = هذا->ارزښت_Format2;
    pair_pos_اولي->class1_شمېر_2 = عدد_ریکارډونه;
    pair_pos_اولي->class2_شمېر_2 = هذا->class2_شمېر_2;
    TRY (clone_class1_ریکارډونه (وېشل_context,
                               pair_pos_اولي_id,
                               پیل,
                               پای_ټکی));

    TRY_ټاکل (بینښه coverage_id,
         ګراف.شاخص_for_دننه_واټن (وېشل_context.this_شاخص, &coverage));
    TRY_ټاکل (بینښه class_def_1_id,
         ګراف.شاخص_for_دننه_واټن (وېشل_context.this_شاخص, &classDef1));

    خپلکاری& coverage_v = ګراف.vertices_[coverage_id];
    خپلکاری& class_def_1_v = ګراف.vertices_[class_def_1_id];
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    ثابت ClassDef* class_def_1_جدول = (ثابت ClassDef*) class_def_1_v.obj().سر_2;
    إذا (unlikely (!coverage_جدول || !class_def_1_جدول))
      أرجع Err(INVALID_دلیل);
    TRY (coverage_جدول->sanitize (coverage_v));
    TRY (class_def_1_جدول->sanitize (class_def_1_v));

    خپلکاری klass_map =
    + coverage_جدول->iter ()
    | hb_map_retains_ترتیبونه ([&] (hb_codepoint_t gid) {
      أرجع hb_codepoint_pair_t (gid, class_def_1_جدول->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      أرجع klass >= پیل && klass < پای_ټکی;
    }, hb_دوهم)
    | hb_map_retains_ترتیبونه ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      أرجع hb_codepoint_pair_t (gid_and_class.لومړی, gid_and_class.second - پیل);
    })
    ;

    TRY (Coverage::جمع_کړه_coverage (وېشل_context.c,
                                 pair_pos_اولي_id,
                                 2,
                                 + klass_map | hb_map_retains_ترتیبونه (hb_لومړی),
                                 وېشل_context.max_coverage_اندازه));

    // classDef1
    TRY (ClassDef::جمع_کړه_class_def (وېشل_context.c,
                                  pair_pos_اولي_id,
                                  8,
                                  + klass_map,
                                  وېشل_context.max_class_def_اندازه));

    // classDef2
    TRY_ټاکل (بینښه class_def_2_id,
         ګراف.شاخص_for_دننه_واټن (وېشل_context.this_شاخص, &classDef2));

    TRY(ګراف.vertices_[pair_pos_اولي_id].جمع_کړه_real_link (SmallTypes::اندازه, class_def_2_id, 10));
    TRY(ګراف.vertices_[class_def_2_id].جمع_کړه_پلار (pair_pos_اولي_id, خطأ));

    أرجع Ok(pair_pos_اولي_id);
  }

  ګراف_پایله_t<فراغ> clone_class1_ریکارډونه (وېشل_context_t& وېشل_context,
                                             بینښه pair_pos_اولي_id,
                                             بینښه پیل, بینښه پای_ټکی) ثابت
  {
    PairPosFormat2* pair_pos_اولي =
        (PairPosFormat2*) وېشل_context.c.ګراف.object (pair_pos_اولي_id).سر_2;

    حرف* پیل_addr = ((حرف*)&values[0]) + پیل * وېشل_context.class1_ریکارډ_اندازه;
    بینښه عدد_ریکارډونه = پای_ټکی - پیل;
    hb_memcpy (&pair_pos_اولي->values[0],
            پیل_addr,
            عدد_ریکارډونه * وېشل_context.class1_ریکارډ_اندازه);

    إذا (!وېشل_context.format1_device_جدول_indices
        && !وېشل_context.format2_device_جدول_indices)
      // No device tables to move over.
      أرجع Ok();

    بینښه class2_شمېر = class2_شمېر_2;
    لكل (بینښه i = پیل; i < پای_ټکی; i++)
    {
      لكل (بینښه j = 0; j < class2_شمېر; j++)
      {
        بینښه value1_شاخص = وېشل_context.ارزښت_ریکارډ_len * (class2_شمېر * i + j);
        بینښه value2_شاخص = value1_شاخص + وېشل_context.value1_ریکارډ_len;

        بینښه new_value1_شاخص = وېشل_context.ارزښت_ریکارډ_len * (class2_شمېر * (i - پیل) + j);
        بینښه new_value2_شاخص = new_value1_شاخص + وېشل_context.value1_ریکارډ_len;

        TRY (transfer_device_tables (وېشل_context,
                                     pair_pos_اولي_id,
                                     وېشل_context.format1_device_جدول_indices,
                                     value1_شاخص,
                                     new_value1_شاخص));

        TRY (transfer_device_tables (وېشل_context,
                                     pair_pos_اولي_id,
                                     وېشل_context.format2_device_جدول_indices,
                                     value2_شاخص,
                                     new_value2_شاخص));
      }
    }
    أرجع Ok();
  }

  ګراف_پایله_t<فراغ> transfer_device_tables (وېشل_context_t& وېشل_context,
                                               بینښه pair_pos_اولي_id,
                                               ثابت hb_ویکټور_t<بینښه>& device_جدول_indices,
                                               بینښه old_ارزښت_ریکارډ_شاخص,
                                               بینښه new_ارزښت_ریکارډ_شاخص) ثابت
  {
    PairPosFormat2* pair_pos_اولي =
        (PairPosFormat2*) وېشل_context.c.ګراف.object (pair_pos_اولي_id).سر_2;

    لكل (بینښه i : device_جدول_indices)
    {
      OT::Offset16* ریکارډ = (OT::Offset16*) &values[old_ارزښت_ریکارډ_شاخص + i];
      بینښه ریکارډ_position = ((حرف*) ریکارډ) - ((حرف*) هذا);
      إذا (!وېشل_context.device_tables.has (ریکارډ_position)) استمر;

      TRY (وېشل_context.c.ګراف.move_ماشوم (
          وېشل_context.this_شاخص,
          ریکارډ,
          pair_pos_اولي_id,
          (OT::Offset16*) &pair_pos_اولي->values[new_ارزښت_ریکارډ_شاخص + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    وېشل_context.c.ګراف.vertices_[وېشل_context.this_شاخص].ترتیبول_real_links ();

    أرجع Ok();
  }

  ګراف_پایله_t<فراغ> shrink (وېشل_context_t& وېشل_context,
                               بینښه شمېر)
  {
    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               وېشل_context.this_شاخص,
               شمېر);
    بینښه old_شمېر = class1_شمېر_2;
    إذا (شمېر >= old_شمېر)
      أرجع Ok();

    ګراف_t& ګراف = وېشل_context.c.ګراف;
    class1_شمېر_2 = شمېر;
    ګراف.vertices_[وېشل_context.this_شاخص].shrink_بفر(
        (old_شمېر - شمېر) * وېشل_context.class1_ریکارډ_اندازه);

    TRY_ټاکل (خپلکاری coverage,
         ګراف.as_mutable_جدول<Coverage> (وېشل_context.this_شاخص, &هذا->coverage));

    TRY_ټاکل (خپلکاری class_def_1,
         ګراف.as_mutable_جدول<ClassDef> (وېشل_context.this_شاخص, &classDef1));

    خپلکاری klass_map =
    + coverage.جدول->iter ()
    | hb_map_retains_ترتیبونه ([&] (hb_codepoint_t gid) {
      أرجع hb_codepoint_pair_t (gid, class_def_1.جدول->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      أرجع klass < شمېر;
    }, hb_دوهم)
    ;

    خپلکاری new_coverage = + klass_map | hb_map_retains_ترتیبونه (hb_لومړی);
    TRY (Coverage::make_coverage (وېشل_context.c,
                                  + new_coverage,
                                  coverage.شاخص_2,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    أرجع ClassDef::make_class_def (وېشل_context.c,
                                     + klass_map,
                                     class_def_1.شاخص_2,
                                     class_def_1.راس->جدول_اندازه ());
  }

  ګراف_پایله_t<hb_hashmap_t<بینښه, بینښه>>
  get_all_device_tables (gsubgpos_ګراف_context_t& c,
                         بینښه this_شاخص) ثابت
  {
    ثابت خپلکاری& v = c.ګراف.vertices_[this_شاخص];
    أرجع v.position_منزل_شاخص_map ();
  }

  ګراف_پایله_t<ثابت Coverage*> get_coverage (gsubgpos_ګراف_context_t& c,
                                                بینښه this_شاخص) ثابت
  {
    TRY_ټاکل (بینښه coverage_id, c.ګراف.شاخص_for_دننه_واټن (this_شاخص, &coverage));
    خپلکاری& coverage_v = c.ګراف.vertices_[coverage_id];

    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    إذا (unlikely (!coverage_جدول)) أرجع Err(INVALID_دلیل);
    TRY(coverage_جدول->sanitize (coverage_v));

    أرجع coverage_جدول;
  }

  ګراف_پایله_t<ثابت ClassDef*> get_class_def_1 (gsubgpos_ګراف_context_t& c,
                                                   بینښه this_شاخص) ثابت
  {
    TRY_ټاکل (بینښه class_def_1_id, c.ګراف.شاخص_for_دننه_واټن (this_شاخص, &classDef1));
    خپلکاری& class_def_1_v = c.ګراف.vertices_[class_def_1_id];

    ثابت ClassDef* class_def_1_جدول = (ثابت ClassDef*) class_def_1_v.obj().سر_2;
    إذا (unlikely (!class_def_1_جدول)) أرجع Err(INVALID_دلیل);
    TRY(class_def_1_جدول->sanitize (class_def_1_v));
    أرجع class_def_1_جدول;
  }

  بینښه اندازه_د_ارزښت_ریکارډ_children (gsubgpos_ګراف_context_t& c,
                                          ثابت hb_hashmap_t<بینښه, بینښه>& device_tables,
                                          ثابت hb_ویکټور_t<بینښه> device_جدول_indices,
                                          بینښه ارزښت_ریکارډ_شاخص,
                                          hb_سېټ_t& لیدل_شوی)
  {
    بینښه اندازه = 0;
    لكل (بینښه i : device_جدول_indices)
    {
      OT::Layout::GPOS_impl::ارزښت* ریکارډ = &values[ارزښت_ریکارډ_شاخص + i];
      بینښه ریکارډ_position = ((حرف*) ریکارډ) - ((حرف*) هذا);
      بینښه* obj_idx;
      إذا (!device_tables.has (ریکارډ_position, &obj_idx)) استمر;
      اندازه += c.ګراف.پیدا_کړه_subgraph_اندازه (*obj_idx, لیدل_شوی).ارزښت_or (0);
    }
    أرجع اندازه;
  }

  ګراف_پایله_t<بینښه> اندازه_د (gsubgpos_ګراف_context_t& c,
                                    بینښه this_شاخص,
                                    ثابت فراغ* دننه_واټن) ثابت
  {
    TRY_ټاکل (ثابت بینښه id, c.ګراف.شاخص_for_دننه_واټن (this_شاخص, دننه_واټن));
    أرجع Ok(c.ګراف.vertices_[id].جدول_اندازه ());
  }
};

بنية PairPos : عام OT::Layout::GPOS_impl::PairPos
{
  ګراف_پایله_t<hb_ویکټور_t<بینښه>> وېشل_subtables (gsubgpos_ګراف_context_t& c,
                                                         بینښه this_شاخص)
  {
    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((PairPosFormat1*)(&u.format1))->وېشل_subtables (c, this_شاخص);
    حالة 2:
      hb_barrier ();
      أرجع ((PairPosFormat2*)(&u.format2))->وېشل_subtables (c, this_شاخص);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: HB_FALLTHROUGH;
    حالة 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#د_که_پای
    افتراضي:
      أرجع Ok(hb_ویکټور_t<بینښه> ());
    }
  }

  ګراف_پایله_t<فراغ> sanitize (ثابت ګراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_اندازه();
    إذا (unlikely (راس_len < u.format.v.get_اندازه ())) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((PairPosFormat1*)(&u.format1))->sanitize (راس);
    حالة 2:
      hb_barrier ();
      أرجع ((PairPosFormat2*)(&u.format2))->sanitize (راس);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: HB_FALLTHROUGH;
    حالة 4: HB_FALLTHROUGH;
#د_که_پای
    افتراضي:
      // We don't handle format 3 and 4 here.
      أرجع Err(SANITIZE_FAILURE);
    }
  }
};

}

#د_که_پای  // GRAPH_PAIRPOS_GRAPH_HH
