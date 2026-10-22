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

#جے_بنا_تعریف گراف_PAIRPOS_گراف_HH
#تعریف_کرو گراف_PAIRPOS_گراف_HH

#ضمن "split-helpers.hh"
#ضمن "coverage-graph.hh"
#ضمن "classdef-graph.hh"
#ضمن "../OT/Layout/GPOS/PairPos.hh"
#ضمن "../OT/Layout/GPOS/PosLookupSubTable.hh"

نطاق گراف {

بنية PairPosFormat1 : عام OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  گراف_نتیجہ_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_ناپ ();
    بنانشانی min_ناپ = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_ناپ;
    إذا (unlikely (راس_len < min_ناپ)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (راس_len < min_ناپ + pair_سیٹ.get_ناپ () - pair_سیٹ.len.get_ناپ()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتیجہ_t<hb_سمتیہ_t<بنانشانی>> ونڈو_subtables (gsubgpos_گراف_context_t& c,
                                                         بنانشانی this_اشاریہ)
  {
    hb_سیٹ_t ویکھیا;

    TRY_سونپو (ثابت بنانشانی coverage_id, c.گراف.اشاریہ_for_وتھ (this_اشاریہ, &coverage));
    ثابت بنانشانی coverage_ناپ = c.گراف.vertices_[coverage_id].جدول_ناپ ();
    ثابت بنانشانی ادھار_ناپ = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_ناپ;

    بنانشانی partial_coverage_ناپ = 4;
    بنانشانی accumulated = ادھار_ناپ;
    hb_سمتیہ_t<بنانشانی> ونڈو_points;
    لكل (بنانشانی i = 0; i < pair_سیٹ.len; i++)
    {
      TRY_سونپو (بنانشانی pair_سیٹ_اشاریہ, pair_سیٹ_گراف_اشاریہ (c, this_اشاریہ, i));
      بنانشانی accumulated_delta =
          c.گراف.لبھو_subgraph_ناپ (pair_سیٹ_اشاریہ, ویکھیا).قدر_or (0) +
          SmallTypes::ناپ; // for PairSet offset.
      partial_coverage_ناپ += OT::HBUINT16::static_ناپ;

      accumulated += accumulated_delta;
      بنانشانی کل = accumulated + hb_min (partial_coverage_ناپ, coverage_ناپ);

      إذا (کل >= (1 << 16))
      {
        ونڈو_points.push (i);
        TRY (گراف_نتیجہ_t<فراغ>::from (ونڈو_points, ونڈ_FAILURE));
        accumulated = ادھار_ناپ + accumulated_delta;
        partial_coverage_ناپ = 6;
        ویکھیا.clear (); // node sharing isn't allowed between splits.
      }
    }

    ونڈو_context_t ونڈو_context {
      c,
      هذا,
      this_اشاریہ,
    };

    أرجع actuate_subtable_ونڈو<ونڈو_context_t> (ونڈو_context, ونڈو_points);
  }

 خاص:

  بنية ونڈو_context_t {
    gsubgpos_گراف_context_t& c;
    PairPosFormat1* thiz;
    بنانشانی this_اشاریہ;

    بنانشانی original_گنتی_2 ()
    {
      أرجع thiz->pair_سیٹ.len;
    }

    گراف_نتیجہ_t<بنانشانی> clone_range (بنانشانی شروع, بنانشانی سرا)
    {
      أرجع thiz->clone_range (هذا->c, هذا->this_اشاریہ, شروع, سرا);
    }

    گراف_نتیجہ_t<فراغ> shrink (بنانشانی گنتی)
    {
      أرجع thiz->shrink (هذا->c, هذا->this_اشاریہ, گنتی);
    }
  };

  گراف_نتیجہ_t<فراغ> shrink (gsubgpos_گراف_context_t& c,
                               بنانشانی this_اشاریہ,
                               بنانشانی گنتی)
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_اشاریہ,
               گنتی);
    بنانشانی old_گنتی = pair_سیٹ.len;
    إذا (گنتی >= old_گنتی)
      أرجع Ok();

    pair_سیٹ.len = گنتی;
    c.گراف.vertices_[this_اشاریہ].shrink_بفر ((old_گنتی - گنتی) * SmallTypes::ناپ);

    TRY_سونپو (خودکار coverage, c.گراف.as_mutable_جدول<Coverage> (this_اشاریہ, &هذا->coverage));

    بنانشانی coverage_ناپ = coverage.راس->جدول_ناپ ();
    خودکار new_coverage =
        + hb_zip (coverage.جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<بنانشانی, بنانشانی> p) {
          أرجع p.second < گنتی;
        })
        | hb_map_retains_چھانٹی (hb_پہلا)
        ;

    أرجع Coverage::make_coverage (c, new_coverage, coverage.اشاریہ_2, coverage_ناپ);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  گراف_نتیجہ_t<بنانشانی> clone_range (gsubgpos_گراف_context_t& c,
                                        بنانشانی this_اشاریہ,
                                        بنانشانی شروع, بنانشانی سرا) ثابت
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_اشاریہ, شروع, سرا);

    بنانشانی عدد_pair_sets = سرا - شروع;
    بنانشانی مفرد_ناپ = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_ناپ
                          + عدد_pair_sets * SmallTypes::ناپ;

    TRY_سونپو (بنانشانی pair_pos_مفرد_id, c.create_گرہ (مفرد_ناپ));

    PairPosFormat1* pair_pos_مفرد = (PairPosFormat1*) c.گراف.object (pair_pos_مفرد_id).سر_2;
    pair_pos_مفرد->format = هذا->format;
    pair_pos_مفرد->قدر_Format_2[0] = هذا->قدر_Format_2[0];
    pair_pos_مفرد->قدر_Format_2[1] = هذا->قدر_Format_2[1];
    pair_pos_مفرد->pair_سیٹ.len = عدد_pair_sets;

    لكل (بنانشانی i = شروع; i < سرا; i++)
    {
      TRY (c.گراف.move_بچہ<> (this_اشاریہ,
                                 &pair_سیٹ[i],
                                 pair_pos_مفرد_id,
                                 &pair_pos_مفرد->pair_سیٹ[i - شروع]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.گراف.vertices_[this_اشاریہ].ترتیب_دیو_real_links ();

    TRY_سونپو (بنانشانی coverage_id, c.گراف.اشاریہ_for_وتھ (this_اشاریہ, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_مفرد_id,
                                   2,
                                   شروع, سرا));

    أرجع Ok(pair_pos_مفرد_id);
  }



  گراف_نتیجہ_t<بنانشانی> pair_سیٹ_گراف_اشاریہ (gsubgpos_گراف_context_t& c, بنانشانی this_اشاریہ, بنانشانی i) ثابت
  {
    أرجع c.گراف.اشاریہ_for_وتھ (this_اشاریہ, &pair_سیٹ[i]);
  }
};

بنية PairPosFormat2 : عام OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  گراف_نتیجہ_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_ناپ ();
    بنانشانی min_ناپ = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_ناپ;
    إذا (unlikely (راس_len < min_ناپ)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    ثابت بنانشانی class1_گنتی = class1_گنتی_2;
    إذا (unlikely (راس_len < min_ناپ + class1_گنتی * get_class1_ریکارڈ_ناپ ()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتیجہ_t<hb_سمتیہ_t<بنانشانی>> ونڈو_subtables (gsubgpos_گراف_context_t& c,
                                                         بنانشانی this_اشاریہ)
  {
    ثابت بنانشانی ادھار_ناپ = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_ناپ;
    TRY_سونپو (ثابت بنانشانی class_def_2_ناپ, ناپ_دا (c, this_اشاریہ, &classDef2));
    TRY_سونپو (ثابت Coverage* coverage, get_coverage (c, this_اشاریہ));
    TRY_سونپو (ثابت ClassDef* class_def_1, get_class_def_1 (c, this_اشاریہ));
    خودکار gid_and_class =
        + coverage->iter ()
        | hb_map_retains_چھانٹی ([&] (hb_codepoint_t gid) {
          أرجع hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_سونپو (class_def_ناپ_estimator_t estimator, class_def_ناپ_estimator_t::create (gid_and_class));

    ثابت بنانشانی class1_گنتی = class1_گنتی_2;
    ثابت بنانشانی class2_گنتی = class2_گنتی_2;
    ثابت بنانشانی class1_ریکارڈ_ناپ = get_class1_ریکارڈ_ناپ ();

    ثابت بنانشانی قدر_1_len = قدر_Format1.get_len ();
    ثابت بنانشانی قدر_2_len = قدر_Format2.get_len ();
    ثابت بنانشانی کل_قدر_len = قدر_1_len + قدر_2_len;

    بنانشانی accumulated = ادھار_ناپ;
    بنانشانی coverage_ناپ = 4;
    بنانشانی class_def_1_ناپ = 4;
    بنانشانی max_coverage_ناپ = coverage_ناپ;
    بنانشانی max_class_def_1_ناپ = class_def_1_ناپ;

    hb_سمتیہ_t<بنانشانی> ونڈو_points;

    TRY_سونپو (خودکار device_tables, get_all_device_tables (c, this_اشاریہ));
    hb_سمتیہ_t<بنانشانی> format1_device_جدول_indices = قدر_Format1.get_device_جدول_indices ();
    hb_سمتیہ_t<بنانشانی> format2_device_جدول_indices = قدر_Format2.get_device_جدول_indices ();
    منطقي has_device_tables = منطقي(format1_device_جدول_indices) || منطقي(format2_device_جدول_indices);

    hb_سیٹ_t ویکھیا;
    لكل (بنانشانی i = 0; i < class1_گنتی; i++)
    {
      بنانشانی accumulated_delta = class1_ریکارڈ_ناپ;
      class_def_1_ناپ = estimator.جوڑو_class_def_ناپ (i);
      coverage_ناپ = estimator.coverage_ناپ ();
      max_coverage_ناپ = hb_max (max_coverage_ناپ, coverage_ناپ);
      max_class_def_1_ناپ = hb_max (max_class_def_1_ناپ, class_def_1_ناپ);

      إذا (has_device_tables) {
        لكل (بنانشانی j = 0; j < class2_گنتی; j++)
        {
          بنانشانی value1_اشاریہ = کل_قدر_len * (class2_گنتی * i + j);
          بنانشانی value2_اشاریہ = value1_اشاریہ + قدر_1_len;
          accumulated_delta += ناپ_دا_قدر_ریکارڈ_children (c,
                                                        device_tables,
                                                        format1_device_جدول_indices,
                                                        value1_اشاریہ,
                                                        ویکھیا);
          accumulated_delta += ناپ_دا_قدر_ریکارڈ_children (c,
                                                        device_tables,
                                                        format2_device_جدول_indices,
                                                        value2_اشاریہ,
                                                        ویکھیا);
        }
      }

      accumulated += accumulated_delta;
      بنانشانی کل = accumulated
                       + coverage_ناپ + class_def_1_ناپ + class_def_2_ناپ
                       // The largest object will pack last and can exceed the size limit.
                       - hb_max (hb_max (coverage_ناپ, class_def_1_ناپ), class_def_2_ناپ);
      إذا (کل >= (1 << 16))
      {
        ونڈو_points.push (i);
        TRY (گراف_نتیجہ_t<فراغ>::from (ونڈو_points, ونڈ_FAILURE));
        // split does not include i, so add the size for i when we reset the size counters.
        accumulated = ادھار_ناپ + accumulated_delta;

        estimator.reset();
        class_def_1_ناپ = estimator.جوڑو_class_def_ناپ(i);
        coverage_ناپ = estimator.coverage_ناپ();
        ویکھیا.clear (); // node sharing isn't allowed between splits.
      }
    }

    ونڈو_context_t ونڈو_context {
      c,
      هذا,
      this_اشاریہ,
      class1_ریکارڈ_ناپ,
      کل_قدر_len,
      قدر_1_len,
      قدر_2_len,
      max_coverage_ناپ,
      max_class_def_1_ناپ,
      device_tables,
      format1_device_جدول_indices,
      format2_device_جدول_indices
    };

    أرجع actuate_subtable_ونڈو<ونڈو_context_t> (ونڈو_context, ونڈو_points);
  }
 خاص:

  بنية ونڈو_context_t
  {
    gsubgpos_گراف_context_t& c;
    PairPosFormat2* thiz;
    بنانشانی this_اشاریہ;
    بنانشانی class1_ریکارڈ_ناپ;
    بنانشانی قدر_ریکارڈ_len;
    بنانشانی value1_ریکارڈ_len;
    بنانشانی value2_ریکارڈ_len;
    بنانشانی max_coverage_ناپ;
    بنانشانی max_class_def_ناپ;

    ثابت hb_hashmap_t<بنانشانی, بنانشانی>& device_tables;
    ثابت hb_سمتیہ_t<بنانشانی>& format1_device_جدول_indices;
    ثابت hb_سمتیہ_t<بنانشانی>& format2_device_جدول_indices;

    بنانشانی original_گنتی_2 ()
    {
      أرجع thiz->class1_گنتی_2;
    }

    گراف_نتیجہ_t<بنانشانی> clone_range (بنانشانی شروع, بنانشانی سرا)
    {
      أرجع thiz->clone_range (*هذا, شروع, سرا);
    }

    گراف_نتیجہ_t<فراغ> shrink (بنانشانی گنتی)
    {
      أرجع thiz->shrink (*هذا, گنتی);
    }
  };

  size_t get_class1_ریکارڈ_ناپ () ثابت
  {
    ثابت size_t class2_گنتی = class2_گنتی_2;
    أرجع
        class2_گنتی * (قدر_Format1.get_ناپ () + قدر_Format2.get_ناپ ());
  }

  گراف_نتیجہ_t<بنانشانی> clone_range (ونڈو_context_t& ونڈو_context,
                                        بنانشانی شروع, بنانشانی سرا) ثابت
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", ونڈو_context.this_اشاریہ, شروع, سرا);

    گراف_t& گراف = ونڈو_context.c.گراف;

    بنانشانی عدد_ریکارڈاں = سرا - شروع;
    بنانشانی مفرد_ناپ = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_ناپ
                          + عدد_ریکارڈاں * ونڈو_context.class1_ریکارڈ_ناپ;

    TRY_سونپو (بنانشانی pair_pos_مفرد_id, ونڈو_context.c.create_گرہ (مفرد_ناپ));

    PairPosFormat2* pair_pos_مفرد =
        (PairPosFormat2*) گراف.object (pair_pos_مفرد_id).سر_2;
    pair_pos_مفرد->format = هذا->format;
    pair_pos_مفرد->قدر_Format1 = هذا->قدر_Format1;
    pair_pos_مفرد->قدر_Format2 = هذا->قدر_Format2;
    pair_pos_مفرد->class1_گنتی_2 = عدد_ریکارڈاں;
    pair_pos_مفرد->class2_گنتی_2 = هذا->class2_گنتی_2;
    TRY (clone_class1_ریکارڈاں (ونڈو_context,
                               pair_pos_مفرد_id,
                               شروع,
                               سرا));

    TRY_سونپو (بنانشانی coverage_id,
         گراف.اشاریہ_for_وتھ (ونڈو_context.this_اشاریہ, &coverage));
    TRY_سونپو (بنانشانی class_def_1_id,
         گراف.اشاریہ_for_وتھ (ونڈو_context.this_اشاریہ, &classDef1));

    خودکار& coverage_v = گراف.vertices_[coverage_id];
    خودکار& class_def_1_v = گراف.vertices_[class_def_1_id];
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    ثابت ClassDef* class_def_1_جدول = (ثابت ClassDef*) class_def_1_v.obj().سر_2;
    إذا (unlikely (!coverage_جدول || !class_def_1_جدول))
      أرجع Err(INVALID_دلیل);
    TRY (coverage_جدول->sanitize (coverage_v));
    TRY (class_def_1_جدول->sanitize (class_def_1_v));

    خودکار klass_map =
    + coverage_جدول->iter ()
    | hb_map_retains_چھانٹی ([&] (hb_codepoint_t gid) {
      أرجع hb_codepoint_pair_t (gid, class_def_1_جدول->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      أرجع klass >= شروع && klass < سرا;
    }, hb_دوجا)
    | hb_map_retains_چھانٹی ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      أرجع hb_codepoint_pair_t (gid_and_class.پہلا, gid_and_class.second - شروع);
    })
    ;

    TRY (Coverage::جوڑو_coverage (ونڈو_context.c,
                                 pair_pos_مفرد_id,
                                 2,
                                 + klass_map | hb_map_retains_چھانٹی (hb_پہلا),
                                 ونڈو_context.max_coverage_ناپ));

    // classDef1
    TRY (ClassDef::جوڑو_class_def (ونڈو_context.c,
                                  pair_pos_مفرد_id,
                                  8,
                                  + klass_map,
                                  ونڈو_context.max_class_def_ناپ));

    // classDef2
    TRY_سونپو (بنانشانی class_def_2_id,
         گراف.اشاریہ_for_وتھ (ونڈو_context.this_اشاریہ, &classDef2));

    TRY(گراف.vertices_[pair_pos_مفرد_id].جوڑو_real_link (SmallTypes::ناپ, class_def_2_id, 10));
    TRY(گراف.vertices_[class_def_2_id].جوڑو_ماپا (pair_pos_مفرد_id, خطأ));

    أرجع Ok(pair_pos_مفرد_id);
  }

  گراف_نتیجہ_t<فراغ> clone_class1_ریکارڈاں (ونڈو_context_t& ونڈو_context,
                                             بنانشانی pair_pos_مفرد_id,
                                             بنانشانی شروع, بنانشانی سرا) ثابت
  {
    PairPosFormat2* pair_pos_مفرد =
        (PairPosFormat2*) ونڈو_context.c.گراف.object (pair_pos_مفرد_id).سر_2;

    حرف* شروع_addr = ((حرف*)&values[0]) + شروع * ونڈو_context.class1_ریکارڈ_ناپ;
    بنانشانی عدد_ریکارڈاں = سرا - شروع;
    hb_memcpy (&pair_pos_مفرد->values[0],
            شروع_addr,
            عدد_ریکارڈاں * ونڈو_context.class1_ریکارڈ_ناپ);

    إذا (!ونڈو_context.format1_device_جدول_indices
        && !ونڈو_context.format2_device_جدول_indices)
      // No device tables to move over.
      أرجع Ok();

    بنانشانی class2_گنتی = class2_گنتی_2;
    لكل (بنانشانی i = شروع; i < سرا; i++)
    {
      لكل (بنانشانی j = 0; j < class2_گنتی; j++)
      {
        بنانشانی value1_اشاریہ = ونڈو_context.قدر_ریکارڈ_len * (class2_گنتی * i + j);
        بنانشانی value2_اشاریہ = value1_اشاریہ + ونڈو_context.value1_ریکارڈ_len;

        بنانشانی new_value1_اشاریہ = ونڈو_context.قدر_ریکارڈ_len * (class2_گنتی * (i - شروع) + j);
        بنانشانی new_value2_اشاریہ = new_value1_اشاریہ + ونڈو_context.value1_ریکارڈ_len;

        TRY (transfer_device_tables (ونڈو_context,
                                     pair_pos_مفرد_id,
                                     ونڈو_context.format1_device_جدول_indices,
                                     value1_اشاریہ,
                                     new_value1_اشاریہ));

        TRY (transfer_device_tables (ونڈو_context,
                                     pair_pos_مفرد_id,
                                     ونڈو_context.format2_device_جدول_indices,
                                     value2_اشاریہ,
                                     new_value2_اشاریہ));
      }
    }
    أرجع Ok();
  }

  گراف_نتیجہ_t<فراغ> transfer_device_tables (ونڈو_context_t& ونڈو_context,
                                               بنانشانی pair_pos_مفرد_id,
                                               ثابت hb_سمتیہ_t<بنانشانی>& device_جدول_indices,
                                               بنانشانی old_قدر_ریکارڈ_اشاریہ,
                                               بنانشانی new_قدر_ریکارڈ_اشاریہ) ثابت
  {
    PairPosFormat2* pair_pos_مفرد =
        (PairPosFormat2*) ونڈو_context.c.گراف.object (pair_pos_مفرد_id).سر_2;

    لكل (بنانشانی i : device_جدول_indices)
    {
      OT::Offset16* ریکارڈ = (OT::Offset16*) &values[old_قدر_ریکارڈ_اشاریہ + i];
      بنانشانی ریکارڈ_position = ((حرف*) ریکارڈ) - ((حرف*) هذا);
      إذا (!ونڈو_context.device_tables.has (ریکارڈ_position)) استمر;

      TRY (ونڈو_context.c.گراف.move_بچہ (
          ونڈو_context.this_اشاریہ,
          ریکارڈ,
          pair_pos_مفرد_id,
          (OT::Offset16*) &pair_pos_مفرد->values[new_قدر_ریکارڈ_اشاریہ + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    ونڈو_context.c.گراف.vertices_[ونڈو_context.this_اشاریہ].ترتیب_دیو_real_links ();

    أرجع Ok();
  }

  گراف_نتیجہ_t<فراغ> shrink (ونڈو_context_t& ونڈو_context,
                               بنانشانی گنتی)
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               ونڈو_context.this_اشاریہ,
               گنتی);
    بنانشانی old_گنتی = class1_گنتی_2;
    إذا (گنتی >= old_گنتی)
      أرجع Ok();

    گراف_t& گراف = ونڈو_context.c.گراف;
    class1_گنتی_2 = گنتی;
    گراف.vertices_[ونڈو_context.this_اشاریہ].shrink_بفر(
        (old_گنتی - گنتی) * ونڈو_context.class1_ریکارڈ_ناپ);

    TRY_سونپو (خودکار coverage,
         گراف.as_mutable_جدول<Coverage> (ونڈو_context.this_اشاریہ, &هذا->coverage));

    TRY_سونپو (خودکار class_def_1,
         گراف.as_mutable_جدول<ClassDef> (ونڈو_context.this_اشاریہ, &classDef1));

    خودکار klass_map =
    + coverage.جدول->iter ()
    | hb_map_retains_چھانٹی ([&] (hb_codepoint_t gid) {
      أرجع hb_codepoint_pair_t (gid, class_def_1.جدول->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      أرجع klass < گنتی;
    }, hb_دوجا)
    ;

    خودکار new_coverage = + klass_map | hb_map_retains_چھانٹی (hb_پہلا);
    TRY (Coverage::make_coverage (ونڈو_context.c,
                                  + new_coverage,
                                  coverage.اشاریہ_2,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    أرجع ClassDef::make_class_def (ونڈو_context.c,
                                     + klass_map,
                                     class_def_1.اشاریہ_2,
                                     class_def_1.راس->جدول_ناپ ());
  }

  گراف_نتیجہ_t<hb_hashmap_t<بنانشانی, بنانشانی>>
  get_all_device_tables (gsubgpos_گراف_context_t& c,
                         بنانشانی this_اشاریہ) ثابت
  {
    ثابت خودکار& v = c.گراف.vertices_[this_اشاریہ];
    أرجع v.position_منزل_اشاریہ_map ();
  }

  گراف_نتیجہ_t<ثابت Coverage*> get_coverage (gsubgpos_گراف_context_t& c,
                                                بنانشانی this_اشاریہ) ثابت
  {
    TRY_سونپو (بنانشانی coverage_id, c.گراف.اشاریہ_for_وتھ (this_اشاریہ, &coverage));
    خودکار& coverage_v = c.گراف.vertices_[coverage_id];

    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    إذا (unlikely (!coverage_جدول)) أرجع Err(INVALID_دلیل);
    TRY(coverage_جدول->sanitize (coverage_v));

    أرجع coverage_جدول;
  }

  گراف_نتیجہ_t<ثابت ClassDef*> get_class_def_1 (gsubgpos_گراف_context_t& c,
                                                   بنانشانی this_اشاریہ) ثابت
  {
    TRY_سونپو (بنانشانی class_def_1_id, c.گراف.اشاریہ_for_وتھ (this_اشاریہ, &classDef1));
    خودکار& class_def_1_v = c.گراف.vertices_[class_def_1_id];

    ثابت ClassDef* class_def_1_جدول = (ثابت ClassDef*) class_def_1_v.obj().سر_2;
    إذا (unlikely (!class_def_1_جدول)) أرجع Err(INVALID_دلیل);
    TRY(class_def_1_جدول->sanitize (class_def_1_v));
    أرجع class_def_1_جدول;
  }

  بنانشانی ناپ_دا_قدر_ریکارڈ_children (gsubgpos_گراف_context_t& c,
                                          ثابت hb_hashmap_t<بنانشانی, بنانشانی>& device_tables,
                                          ثابت hb_سمتیہ_t<بنانشانی> device_جدول_indices,
                                          بنانشانی قدر_ریکارڈ_اشاریہ,
                                          hb_سیٹ_t& ویکھیا)
  {
    بنانشانی ناپ = 0;
    لكل (بنانشانی i : device_جدول_indices)
    {
      OT::Layout::GPOS_impl::قدر* ریکارڈ = &values[قدر_ریکارڈ_اشاریہ + i];
      بنانشانی ریکارڈ_position = ((حرف*) ریکارڈ) - ((حرف*) هذا);
      بنانشانی* obj_idx;
      إذا (!device_tables.has (ریکارڈ_position, &obj_idx)) استمر;
      ناپ += c.گراف.لبھو_subgraph_ناپ (*obj_idx, ویکھیا).قدر_or (0);
    }
    أرجع ناپ;
  }

  گراف_نتیجہ_t<بنانشانی> ناپ_دا (gsubgpos_گراف_context_t& c,
                                    بنانشانی this_اشاریہ,
                                    ثابت فراغ* وتھ) ثابت
  {
    TRY_سونپو (ثابت بنانشانی id, c.گراف.اشاریہ_for_وتھ (this_اشاریہ, وتھ));
    أرجع Ok(c.گراف.vertices_[id].جدول_ناپ ());
  }
};

بنية PairPos : عام OT::Layout::GPOS_impl::PairPos
{
  گراف_نتیجہ_t<hb_سمتیہ_t<بنانشانی>> ونڈو_subtables (gsubgpos_گراف_context_t& c,
                                                         بنانشانی this_اشاریہ)
  {
    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((PairPosFormat1*)(&u.format1))->ونڈو_subtables (c, this_اشاریہ);
    حالة 2:
      hb_barrier ();
      أرجع ((PairPosFormat2*)(&u.format2))->ونڈو_subtables (c, this_اشاریہ);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: HB_FALLTHROUGH;
    حالة 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#جے_انت
    افتراضي:
      أرجع Ok(hb_سمتیہ_t<بنانشانی> ());
    }
  }

  گراف_نتیجہ_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_ناپ();
    إذا (unlikely (راس_len < u.format.v.get_ناپ ())) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((PairPosFormat1*)(&u.format1))->sanitize (راس);
    حالة 2:
      hb_barrier ();
      أرجع ((PairPosFormat2*)(&u.format2))->sanitize (راس);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: HB_FALLTHROUGH;
    حالة 4: HB_FALLTHROUGH;
#جے_انت
    افتراضي:
      // We don't handle format 3 and 4 here.
      أرجع Err(SANITIZE_FAILURE);
    }
  }
};

}

#جے_انت  // GRAPH_PAIRPOS_GRAPH_HH
