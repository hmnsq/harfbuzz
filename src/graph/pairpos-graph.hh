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

#اگر_تعریف_نشده گراف_PAIRPOS_گراف_HH
#تعریف_کن گراف_PAIRPOS_گراف_HH

#ضمن "split-helpers.hh"
#ضمن "coverage-graph.hh"
#ضمن "classdef-graph.hh"
#ضمن "../OT/Layout/GPOS/PairPos.hh"
#ضمن "../OT/Layout/GPOS/PosLookupSubTable.hh"

نطاق گراف {

بنية PairPosFormat1 : عام OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  گراف_نتیجه_t<فراغ> sanitize (ثابت گراف_t::رأس_t& رأس) ثابت
  {
    size_t رأس_len = رأس.جدول_اندازه ();
    بیعلامت min_اندازه = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_اندازه;
    إذا (unlikely (رأس_len < min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (رأس_len < min_اندازه + pair_مجموعه.get_اندازه () - pair_مجموعه.len.get_اندازه()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتیجه_t<hb_وکتور_t<بیعلامت>> تقسیم_subtables (gsubgpos_گراف_context_t& c,
                                                         بیعلامت this_شاخص)
  {
    hb_مجموعه_t دیدهشده;

    TRY_تخصیص_بده (ثابت بیعلامت coverage_id, c.گراف.شاخص_for_جابه_جایی (this_شاخص, &coverage));
    ثابت بیعلامت coverage_اندازه = c.گراف.vertices_[coverage_id].جدول_اندازه ();
    ثابت بیعلامت پایه_اندازه = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_اندازه;

    بیعلامت partial_coverage_اندازه = 4;
    بیعلامت accumulated = پایه_اندازه;
    hb_وکتور_t<بیعلامت> تقسیم_points;
    لكل (بیعلامت i = 0; i < pair_مجموعه.len; i++)
    {
      TRY_تخصیص_بده (بیعلامت pair_مجموعه_شاخص, pair_مجموعه_گراف_شاخص (c, this_شاخص, i));
      بیعلامت accumulated_delta =
          c.گراف.پیدا_کن_subgraph_اندازه (pair_مجموعه_شاخص, دیدهشده).مقدار_or (0) +
          SmallTypes::اندازه; // for PairSet offset.
      partial_coverage_اندازه += OT::HBUINT16::static_اندازه;

      accumulated += accumulated_delta;
      بیعلامت مجموع = accumulated + hb_min (partial_coverage_اندازه, coverage_اندازه);

      إذا (مجموع >= (1 << 16))
      {
        تقسیم_points.push (i);
        TRY (گراف_نتیجه_t<فراغ>::from (تقسیم_points, تخصیص_FAILURE));
        accumulated = پایه_اندازه + accumulated_delta;
        partial_coverage_اندازه = 6;
        دیدهشده.clear (); // node sharing isn't allowed between splits.
      }
    }

    تقسیم_context_t تقسیم_context {
      c,
      هذا,
      this_شاخص,
    };

    أرجع actuate_subtable_تقسیم<تقسیم_context_t> (تقسیم_context, تقسیم_points);
  }

 خاص:

  بنية تقسیم_context_t {
    gsubgpos_گراف_context_t& c;
    PairPosFormat1* thiz;
    بیعلامت this_شاخص;

    بیعلامت original_شمار_2 ()
    {
      أرجع thiz->pair_مجموعه.len;
    }

    گراف_نتیجه_t<بیعلامت> clone_range (بیعلامت شروع, بیعلامت انتها)
    {
      أرجع thiz->clone_range (هذا->c, هذا->this_شاخص, شروع, انتها);
    }

    گراف_نتیجه_t<فراغ> shrink (بیعلامت شمار)
    {
      أرجع thiz->shrink (هذا->c, هذا->this_شاخص, شمار);
    }
  };

  گراف_نتیجه_t<فراغ> shrink (gsubgpos_گراف_context_t& c,
                               بیعلامت this_شاخص,
                               بیعلامت شمار)
  {
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_شاخص,
               شمار);
    بیعلامت old_شمار = pair_مجموعه.len;
    إذا (شمار >= old_شمار)
      أرجع Ok();

    pair_مجموعه.len = شمار;
    c.گراف.vertices_[this_شاخص].shrink_میانگیر ((old_شمار - شمار) * SmallTypes::اندازه);

    TRY_تخصیص_بده (خودکار coverage, c.گراف.as_mutable_جدول<Coverage> (this_شاخص, &هذا->coverage));

    بیعلامت coverage_اندازه = coverage.رأس->جدول_اندازه ();
    خودکار new_coverage =
        + hb_zip (coverage.جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<بیعلامت, بیعلامت> p) {
          أرجع p.second < شمار;
        })
        | hb_map_retains_ترتیبدهی (hb_اولی)
        ;

    أرجع Coverage::make_coverage (c, new_coverage, coverage.شاخص_2, coverage_اندازه);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  گراف_نتیجه_t<بیعلامت> clone_range (gsubgpos_گراف_context_t& c,
                                        بیعلامت this_شاخص,
                                        بیعلامت شروع, بیعلامت انتها) ثابت
  {
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_شاخص, شروع, انتها);

    بیعلامت رقم_pair_sets = انتها - شروع;
    بیعلامت اول_اندازه = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_اندازه
                          + رقم_pair_sets * SmallTypes::اندازه;

    TRY_تخصیص_بده (بیعلامت pair_pos_اول_id, c.create_گره (اول_اندازه));

    PairPosFormat1* pair_pos_اول = (PairPosFormat1*) c.گراف.object (pair_pos_اول_id).سر_2;
    pair_pos_اول->format = هذا->format;
    pair_pos_اول->مقدار_Format_2[0] = هذا->مقدار_Format_2[0];
    pair_pos_اول->مقدار_Format_2[1] = هذا->مقدار_Format_2[1];
    pair_pos_اول->pair_مجموعه.len = رقم_pair_sets;

    لكل (بیعلامت i = شروع; i < انتها; i++)
    {
      TRY (c.گراف.move_فرزند<> (this_شاخص,
                                 &pair_مجموعه[i],
                                 pair_pos_اول_id,
                                 &pair_pos_اول->pair_مجموعه[i - شروع]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.گراف.vertices_[this_شاخص].مرتب_سازی_real_links ();

    TRY_تخصیص_بده (بیعلامت coverage_id, c.گراف.شاخص_for_جابه_جایی (this_شاخص, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_اول_id,
                                   2,
                                   شروع, انتها));

    أرجع Ok(pair_pos_اول_id);
  }



  گراف_نتیجه_t<بیعلامت> pair_مجموعه_گراف_شاخص (gsubgpos_گراف_context_t& c, بیعلامت this_شاخص, بیعلامت i) ثابت
  {
    أرجع c.گراف.شاخص_for_جابه_جایی (this_شاخص, &pair_مجموعه[i]);
  }
};

بنية PairPosFormat2 : عام OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  گراف_نتیجه_t<فراغ> sanitize (ثابت گراف_t::رأس_t& رأس) ثابت
  {
    size_t رأس_len = رأس.جدول_اندازه ();
    بیعلامت min_اندازه = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_اندازه;
    إذا (unlikely (رأس_len < min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    ثابت بیعلامت class1_شمار = class1_شمار_2;
    إذا (unlikely (رأس_len < min_اندازه + class1_شمار * get_class1_رکورد_اندازه ()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتیجه_t<hb_وکتور_t<بیعلامت>> تقسیم_subtables (gsubgpos_گراف_context_t& c,
                                                         بیعلامت this_شاخص)
  {
    ثابت بیعلامت پایه_اندازه = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_اندازه;
    TRY_تخصیص_بده (ثابت بیعلامت class_def_2_اندازه, اندازه_از (c, this_شاخص, &classDef2));
    TRY_تخصیص_بده (ثابت Coverage* coverage, get_coverage (c, this_شاخص));
    TRY_تخصیص_بده (ثابت ClassDef* class_def_1, get_class_def_1 (c, this_شاخص));
    خودکار gid_and_class =
        + coverage->iter ()
        | hb_map_retains_ترتیبدهی ([&] (hb_codepoint_t gid) {
          أرجع hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_تخصیص_بده (class_def_اندازه_estimator_t estimator, class_def_اندازه_estimator_t::create (gid_and_class));

    ثابت بیعلامت class1_شمار = class1_شمار_2;
    ثابت بیعلامت class2_شمار = class2_شمار_2;
    ثابت بیعلامت class1_رکورد_اندازه = get_class1_رکورد_اندازه ();

    ثابت بیعلامت مقدار_1_len = مقدار_Format1.get_len ();
    ثابت بیعلامت مقدار_2_len = مقدار_Format2.get_len ();
    ثابت بیعلامت مجموع_مقدار_len = مقدار_1_len + مقدار_2_len;

    بیعلامت accumulated = پایه_اندازه;
    بیعلامت coverage_اندازه = 4;
    بیعلامت class_def_1_اندازه = 4;
    بیعلامت max_coverage_اندازه = coverage_اندازه;
    بیعلامت max_class_def_1_اندازه = class_def_1_اندازه;

    hb_وکتور_t<بیعلامت> تقسیم_points;

    TRY_تخصیص_بده (خودکار device_tables, get_all_device_tables (c, this_شاخص));
    hb_وکتور_t<بیعلامت> format1_device_جدول_indices = مقدار_Format1.get_device_جدول_indices ();
    hb_وکتور_t<بیعلامت> format2_device_جدول_indices = مقدار_Format2.get_device_جدول_indices ();
    منطقي has_device_tables = منطقي(format1_device_جدول_indices) || منطقي(format2_device_جدول_indices);

    hb_مجموعه_t دیدهشده;
    لكل (بیعلامت i = 0; i < class1_شمار; i++)
    {
      بیعلامت accumulated_delta = class1_رکورد_اندازه;
      class_def_1_اندازه = estimator.جمع_کن_class_def_اندازه (i);
      coverage_اندازه = estimator.coverage_اندازه ();
      max_coverage_اندازه = hb_max (max_coverage_اندازه, coverage_اندازه);
      max_class_def_1_اندازه = hb_max (max_class_def_1_اندازه, class_def_1_اندازه);

      إذا (has_device_tables) {
        لكل (بیعلامت j = 0; j < class2_شمار; j++)
        {
          بیعلامت value1_شاخص = مجموع_مقدار_len * (class2_شمار * i + j);
          بیعلامت value2_شاخص = value1_شاخص + مقدار_1_len;
          accumulated_delta += اندازه_از_مقدار_رکورد_children (c,
                                                        device_tables,
                                                        format1_device_جدول_indices,
                                                        value1_شاخص,
                                                        دیدهشده);
          accumulated_delta += اندازه_از_مقدار_رکورد_children (c,
                                                        device_tables,
                                                        format2_device_جدول_indices,
                                                        value2_شاخص,
                                                        دیدهشده);
        }
      }

      accumulated += accumulated_delta;
      بیعلامت مجموع = accumulated
                       + coverage_اندازه + class_def_1_اندازه + class_def_2_اندازه
                       // The largest object will pack last and can exceed the size limit.
                       - hb_max (hb_max (coverage_اندازه, class_def_1_اندازه), class_def_2_اندازه);
      إذا (مجموع >= (1 << 16))
      {
        تقسیم_points.push (i);
        TRY (گراف_نتیجه_t<فراغ>::from (تقسیم_points, تخصیص_FAILURE));
        // split does not include i, so add the size for i when we reset the size counters.
        accumulated = پایه_اندازه + accumulated_delta;

        estimator.reset();
        class_def_1_اندازه = estimator.جمع_کن_class_def_اندازه(i);
        coverage_اندازه = estimator.coverage_اندازه();
        دیدهشده.clear (); // node sharing isn't allowed between splits.
      }
    }

    تقسیم_context_t تقسیم_context {
      c,
      هذا,
      this_شاخص,
      class1_رکورد_اندازه,
      مجموع_مقدار_len,
      مقدار_1_len,
      مقدار_2_len,
      max_coverage_اندازه,
      max_class_def_1_اندازه,
      device_tables,
      format1_device_جدول_indices,
      format2_device_جدول_indices
    };

    أرجع actuate_subtable_تقسیم<تقسیم_context_t> (تقسیم_context, تقسیم_points);
  }
 خاص:

  بنية تقسیم_context_t
  {
    gsubgpos_گراف_context_t& c;
    PairPosFormat2* thiz;
    بیعلامت this_شاخص;
    بیعلامت class1_رکورد_اندازه;
    بیعلامت مقدار_رکورد_len;
    بیعلامت value1_رکورد_len;
    بیعلامت value2_رکورد_len;
    بیعلامت max_coverage_اندازه;
    بیعلامت max_class_def_اندازه;

    ثابت hb_hashmap_t<بیعلامت, بیعلامت>& device_tables;
    ثابت hb_وکتور_t<بیعلامت>& format1_device_جدول_indices;
    ثابت hb_وکتور_t<بیعلامت>& format2_device_جدول_indices;

    بیعلامت original_شمار_2 ()
    {
      أرجع thiz->class1_شمار_2;
    }

    گراف_نتیجه_t<بیعلامت> clone_range (بیعلامت شروع, بیعلامت انتها)
    {
      أرجع thiz->clone_range (*هذا, شروع, انتها);
    }

    گراف_نتیجه_t<فراغ> shrink (بیعلامت شمار)
    {
      أرجع thiz->shrink (*هذا, شمار);
    }
  };

  size_t get_class1_رکورد_اندازه () ثابت
  {
    ثابت size_t class2_شمار = class2_شمار_2;
    أرجع
        class2_شمار * (مقدار_Format1.get_اندازه () + مقدار_Format2.get_اندازه ());
  }

  گراف_نتیجه_t<بیعلامت> clone_range (تقسیم_context_t& تقسیم_context,
                                        بیعلامت شروع, بیعلامت انتها) ثابت
  {
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", تقسیم_context.this_شاخص, شروع, انتها);

    گراف_t& گراف = تقسیم_context.c.گراف;

    بیعلامت رقم_رکوردها = انتها - شروع;
    بیعلامت اول_اندازه = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_اندازه
                          + رقم_رکوردها * تقسیم_context.class1_رکورد_اندازه;

    TRY_تخصیص_بده (بیعلامت pair_pos_اول_id, تقسیم_context.c.create_گره (اول_اندازه));

    PairPosFormat2* pair_pos_اول =
        (PairPosFormat2*) گراف.object (pair_pos_اول_id).سر_2;
    pair_pos_اول->format = هذا->format;
    pair_pos_اول->مقدار_Format1 = هذا->مقدار_Format1;
    pair_pos_اول->مقدار_Format2 = هذا->مقدار_Format2;
    pair_pos_اول->class1_شمار_2 = رقم_رکوردها;
    pair_pos_اول->class2_شمار_2 = هذا->class2_شمار_2;
    TRY (clone_class1_رکوردها (تقسیم_context,
                               pair_pos_اول_id,
                               شروع,
                               انتها));

    TRY_تخصیص_بده (بیعلامت coverage_id,
         گراف.شاخص_for_جابه_جایی (تقسیم_context.this_شاخص, &coverage));
    TRY_تخصیص_بده (بیعلامت class_def_1_id,
         گراف.شاخص_for_جابه_جایی (تقسیم_context.this_شاخص, &classDef1));

    خودکار& coverage_v = گراف.vertices_[coverage_id];
    خودکار& class_def_1_v = گراف.vertices_[class_def_1_id];
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    ثابت ClassDef* class_def_1_جدول = (ثابت ClassDef*) class_def_1_v.obj().سر_2;
    إذا (unlikely (!coverage_جدول || !class_def_1_جدول))
      أرجع Err(INVALID_آرگومان);
    TRY (coverage_جدول->sanitize (coverage_v));
    TRY (class_def_1_جدول->sanitize (class_def_1_v));

    خودکار klass_map =
    + coverage_جدول->iter ()
    | hb_map_retains_ترتیبدهی ([&] (hb_codepoint_t gid) {
      أرجع hb_codepoint_pair_t (gid, class_def_1_جدول->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      أرجع klass >= شروع && klass < انتها;
    }, hb_دومی)
    | hb_map_retains_ترتیبدهی ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      أرجع hb_codepoint_pair_t (gid_and_class.اولی, gid_and_class.second - شروع);
    })
    ;

    TRY (Coverage::جمع_کن_coverage (تقسیم_context.c,
                                 pair_pos_اول_id,
                                 2,
                                 + klass_map | hb_map_retains_ترتیبدهی (hb_اولی),
                                 تقسیم_context.max_coverage_اندازه));

    // classDef1
    TRY (ClassDef::جمع_کن_class_def (تقسیم_context.c,
                                  pair_pos_اول_id,
                                  8,
                                  + klass_map,
                                  تقسیم_context.max_class_def_اندازه));

    // classDef2
    TRY_تخصیص_بده (بیعلامت class_def_2_id,
         گراف.شاخص_for_جابه_جایی (تقسیم_context.this_شاخص, &classDef2));

    TRY(گراف.vertices_[pair_pos_اول_id].جمع_کن_real_link (SmallTypes::اندازه, class_def_2_id, 10));
    TRY(گراف.vertices_[class_def_2_id].جمع_کن_والد (pair_pos_اول_id, خطأ));

    أرجع Ok(pair_pos_اول_id);
  }

  گراف_نتیجه_t<فراغ> clone_class1_رکوردها (تقسیم_context_t& تقسیم_context,
                                             بیعلامت pair_pos_اول_id,
                                             بیعلامت شروع, بیعلامت انتها) ثابت
  {
    PairPosFormat2* pair_pos_اول =
        (PairPosFormat2*) تقسیم_context.c.گراف.object (pair_pos_اول_id).سر_2;

    حرف* شروع_addr = ((حرف*)&values[0]) + شروع * تقسیم_context.class1_رکورد_اندازه;
    بیعلامت رقم_رکوردها = انتها - شروع;
    hb_memcpy (&pair_pos_اول->values[0],
            شروع_addr,
            رقم_رکوردها * تقسیم_context.class1_رکورد_اندازه);

    إذا (!تقسیم_context.format1_device_جدول_indices
        && !تقسیم_context.format2_device_جدول_indices)
      // No device tables to move over.
      أرجع Ok();

    بیعلامت class2_شمار = class2_شمار_2;
    لكل (بیعلامت i = شروع; i < انتها; i++)
    {
      لكل (بیعلامت j = 0; j < class2_شمار; j++)
      {
        بیعلامت value1_شاخص = تقسیم_context.مقدار_رکورد_len * (class2_شمار * i + j);
        بیعلامت value2_شاخص = value1_شاخص + تقسیم_context.value1_رکورد_len;

        بیعلامت new_value1_شاخص = تقسیم_context.مقدار_رکورد_len * (class2_شمار * (i - شروع) + j);
        بیعلامت new_value2_شاخص = new_value1_شاخص + تقسیم_context.value1_رکورد_len;

        TRY (transfer_device_tables (تقسیم_context,
                                     pair_pos_اول_id,
                                     تقسیم_context.format1_device_جدول_indices,
                                     value1_شاخص,
                                     new_value1_شاخص));

        TRY (transfer_device_tables (تقسیم_context,
                                     pair_pos_اول_id,
                                     تقسیم_context.format2_device_جدول_indices,
                                     value2_شاخص,
                                     new_value2_شاخص));
      }
    }
    أرجع Ok();
  }

  گراف_نتیجه_t<فراغ> transfer_device_tables (تقسیم_context_t& تقسیم_context,
                                               بیعلامت pair_pos_اول_id,
                                               ثابت hb_وکتور_t<بیعلامت>& device_جدول_indices,
                                               بیعلامت old_مقدار_رکورد_شاخص,
                                               بیعلامت new_مقدار_رکورد_شاخص) ثابت
  {
    PairPosFormat2* pair_pos_اول =
        (PairPosFormat2*) تقسیم_context.c.گراف.object (pair_pos_اول_id).سر_2;

    لكل (بیعلامت i : device_جدول_indices)
    {
      OT::Offset16* رکورد = (OT::Offset16*) &values[old_مقدار_رکورد_شاخص + i];
      بیعلامت رکورد_position = ((حرف*) رکورد) - ((حرف*) هذا);
      إذا (!تقسیم_context.device_tables.has (رکورد_position)) استمر;

      TRY (تقسیم_context.c.گراف.move_فرزند (
          تقسیم_context.this_شاخص,
          رکورد,
          pair_pos_اول_id,
          (OT::Offset16*) &pair_pos_اول->values[new_مقدار_رکورد_شاخص + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    تقسیم_context.c.گراف.vertices_[تقسیم_context.this_شاخص].مرتب_سازی_real_links ();

    أرجع Ok();
  }

  گراف_نتیجه_t<فراغ> shrink (تقسیم_context_t& تقسیم_context,
                               بیعلامت شمار)
  {
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               تقسیم_context.this_شاخص,
               شمار);
    بیعلامت old_شمار = class1_شمار_2;
    إذا (شمار >= old_شمار)
      أرجع Ok();

    گراف_t& گراف = تقسیم_context.c.گراف;
    class1_شمار_2 = شمار;
    گراف.vertices_[تقسیم_context.this_شاخص].shrink_میانگیر(
        (old_شمار - شمار) * تقسیم_context.class1_رکورد_اندازه);

    TRY_تخصیص_بده (خودکار coverage,
         گراف.as_mutable_جدول<Coverage> (تقسیم_context.this_شاخص, &هذا->coverage));

    TRY_تخصیص_بده (خودکار class_def_1,
         گراف.as_mutable_جدول<ClassDef> (تقسیم_context.this_شاخص, &classDef1));

    خودکار klass_map =
    + coverage.جدول->iter ()
    | hb_map_retains_ترتیبدهی ([&] (hb_codepoint_t gid) {
      أرجع hb_codepoint_pair_t (gid, class_def_1.جدول->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      أرجع klass < شمار;
    }, hb_دومی)
    ;

    خودکار new_coverage = + klass_map | hb_map_retains_ترتیبدهی (hb_اولی);
    TRY (Coverage::make_coverage (تقسیم_context.c,
                                  + new_coverage,
                                  coverage.شاخص_2,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    أرجع ClassDef::make_class_def (تقسیم_context.c,
                                     + klass_map,
                                     class_def_1.شاخص_2,
                                     class_def_1.رأس->جدول_اندازه ());
  }

  گراف_نتیجه_t<hb_hashmap_t<بیعلامت, بیعلامت>>
  get_all_device_tables (gsubgpos_گراف_context_t& c,
                         بیعلامت this_شاخص) ثابت
  {
    ثابت خودکار& v = c.گراف.vertices_[this_شاخص];
    أرجع v.position_مقصد_شاخص_map ();
  }

  گراف_نتیجه_t<ثابت Coverage*> get_coverage (gsubgpos_گراف_context_t& c,
                                                بیعلامت this_شاخص) ثابت
  {
    TRY_تخصیص_بده (بیعلامت coverage_id, c.گراف.شاخص_for_جابه_جایی (this_شاخص, &coverage));
    خودکار& coverage_v = c.گراف.vertices_[coverage_id];

    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    إذا (unlikely (!coverage_جدول)) أرجع Err(INVALID_آرگومان);
    TRY(coverage_جدول->sanitize (coverage_v));

    أرجع coverage_جدول;
  }

  گراف_نتیجه_t<ثابت ClassDef*> get_class_def_1 (gsubgpos_گراف_context_t& c,
                                                   بیعلامت this_شاخص) ثابت
  {
    TRY_تخصیص_بده (بیعلامت class_def_1_id, c.گراف.شاخص_for_جابه_جایی (this_شاخص, &classDef1));
    خودکار& class_def_1_v = c.گراف.vertices_[class_def_1_id];

    ثابت ClassDef* class_def_1_جدول = (ثابت ClassDef*) class_def_1_v.obj().سر_2;
    إذا (unlikely (!class_def_1_جدول)) أرجع Err(INVALID_آرگومان);
    TRY(class_def_1_جدول->sanitize (class_def_1_v));
    أرجع class_def_1_جدول;
  }

  بیعلامت اندازه_از_مقدار_رکورد_children (gsubgpos_گراف_context_t& c,
                                          ثابت hb_hashmap_t<بیعلامت, بیعلامت>& device_tables,
                                          ثابت hb_وکتور_t<بیعلامت> device_جدول_indices,
                                          بیعلامت مقدار_رکورد_شاخص,
                                          hb_مجموعه_t& دیدهشده)
  {
    بیعلامت اندازه = 0;
    لكل (بیعلامت i : device_جدول_indices)
    {
      OT::Layout::GPOS_impl::مقدار* رکورد = &values[مقدار_رکورد_شاخص + i];
      بیعلامت رکورد_position = ((حرف*) رکورد) - ((حرف*) هذا);
      بیعلامت* obj_idx;
      إذا (!device_tables.has (رکورد_position, &obj_idx)) استمر;
      اندازه += c.گراف.پیدا_کن_subgraph_اندازه (*obj_idx, دیدهشده).مقدار_or (0);
    }
    أرجع اندازه;
  }

  گراف_نتیجه_t<بیعلامت> اندازه_از (gsubgpos_گراف_context_t& c,
                                    بیعلامت this_شاخص,
                                    ثابت فراغ* جابه_جایی) ثابت
  {
    TRY_تخصیص_بده (ثابت بیعلامت id, c.گراف.شاخص_for_جابه_جایی (this_شاخص, جابه_جایی));
    أرجع Ok(c.گراف.vertices_[id].جدول_اندازه ());
  }
};

بنية PairPos : عام OT::Layout::GPOS_impl::PairPos
{
  گراف_نتیجه_t<hb_وکتور_t<بیعلامت>> تقسیم_subtables (gsubgpos_گراف_context_t& c,
                                                         بیعلامت this_شاخص)
  {
    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((PairPosFormat1*)(&u.format1))->تقسیم_subtables (c, this_شاخص);
    حالة 2:
      hb_barrier ();
      أرجع ((PairPosFormat2*)(&u.format2))->تقسیم_subtables (c, this_شاخص);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: HB_FALLTHROUGH;
    حالة 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#پایان_اگر
    افتراضي:
      أرجع Ok(hb_وکتور_t<بیعلامت> ());
    }
  }

  گراف_نتیجه_t<فراغ> sanitize (ثابت گراف_t::رأس_t& رأس) ثابت
  {
    size_t رأس_len = رأس.جدول_اندازه();
    إذا (unlikely (رأس_len < u.format.v.get_اندازه ())) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((PairPosFormat1*)(&u.format1))->sanitize (رأس);
    حالة 2:
      hb_barrier ();
      أرجع ((PairPosFormat2*)(&u.format2))->sanitize (رأس);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: HB_FALLTHROUGH;
    حالة 4: HB_FALLTHROUGH;
#پایان_اگر
    افتراضي:
      // We don't handle format 3 and 4 here.
      أرجع Err(SANITIZE_FAILURE);
    }
  }
};

}

#پایان_اگر  // GRAPH_PAIRPOS_GRAPH_HH
