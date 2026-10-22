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

#اگر_غیر_تعریف_شدہ گراف_PAIRPOS_گراف_HH
#تعریف_کرو گراف_PAIRPOS_گراف_HH

#شامل "split-helpers.hh"
#شامل "coverage-graph.hh"
#شامل "classdef-graph.hh"
#شامل "../OT/Layout/GPOS/PairPos.hh"
#شامل "../OT/Layout/GPOS/PosLookupSubTable.hh"

نامخلا گراف {

ساخت PairPosFormat1 : عوامی OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  گراف_نتیجہ_t<خالی> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائز ();
    غیرعلامتی min_سائز = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_سائز;
    اگر (unlikely (راس_len < min_سائز)) واپس Err(SANITIZE_FAILURE);
    hb_barrier ();

    اگر (unlikely (راس_len < min_سائز + pair_سیٹ.get_سائز () - pair_سیٹ.len.get_سائز()))
      واپس Err(SANITIZE_FAILURE);
    واپس Ok();
  }

  گراف_نتیجہ_t<hb_سمتیہ_t<غیرعلامتی>> بانٹو_subtables (gsubgpos_گراف_context_t& c,
                                                         غیرعلامتی this_اشاریہ)
  {
    hb_سیٹ_t دیکھا;

    TRY_تفویض (ثابت غیرعلامتی coverage_id, c.گراف.اشاریہ_for_انحراف (this_اشاریہ, &coverage));
    ثابت غیرعلامتی coverage_سائز = c.گراف.vertices_[coverage_id].جدول_سائز ();
    ثابت غیرعلامتی بنیاد_سائز = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_سائز;

    غیرعلامتی partial_coverage_سائز = 4;
    غیرعلامتی accumulated = بنیاد_سائز;
    hb_سمتیہ_t<غیرعلامتی> بانٹو_points;
    برائے (غیرعلامتی i = 0; i < pair_سیٹ.len; i++)
    {
      TRY_تفویض (غیرعلامتی pair_سیٹ_اشاریہ, pair_سیٹ_گراف_اشاریہ (c, this_اشاریہ, i));
      غیرعلامتی accumulated_delta =
          c.گراف.ڈھونڈو_subgraph_سائز (pair_سیٹ_اشاریہ, دیکھا).قدر_or (0) +
          SmallTypes::سائز; // for PairSet offset.
      partial_coverage_سائز += OT::HBUINT16::static_سائز;

      accumulated += accumulated_delta;
      غیرعلامتی کل = accumulated + hb_min (partial_coverage_سائز, coverage_سائز);

      اگر (کل >= (1 << 16))
      {
        بانٹو_points.push (i);
        TRY (گراف_نتیجہ_t<خالی>::from (بانٹو_points, تخصیص_FAILURE));
        accumulated = بنیاد_سائز + accumulated_delta;
        partial_coverage_سائز = 6;
        دیکھا.clear (); // node sharing isn't allowed between splits.
      }
    }

    بانٹو_context_t بانٹو_context {
      c,
      یہ,
      this_اشاریہ,
    };

    واپس actuate_subtable_بانٹو<بانٹو_context_t> (بانٹو_context, بانٹو_points);
  }

 نجی:

  ساخت بانٹو_context_t {
    gsubgpos_گراف_context_t& c;
    PairPosFormat1* thiz;
    غیرعلامتی this_اشاریہ;

    غیرعلامتی original_گنتی_2 ()
    {
      واپس thiz->pair_سیٹ.len;
    }

    گراف_نتیجہ_t<غیرعلامتی> clone_range (غیرعلامتی آغاز, غیرعلامتی انتہا)
    {
      واپس thiz->clone_range (یہ->c, یہ->this_اشاریہ, آغاز, انتہا);
    }

    گراف_نتیجہ_t<خالی> shrink (غیرعلامتی گنتی)
    {
      واپس thiz->shrink (یہ->c, یہ->this_اشاریہ, گنتی);
    }
  };

  گراف_نتیجہ_t<خالی> shrink (gsubgpos_گراف_context_t& c,
                               غیرعلامتی this_اشاریہ,
                               غیرعلامتی گنتی)
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_اشاریہ,
               گنتی);
    غیرعلامتی old_گنتی = pair_سیٹ.len;
    اگر (گنتی >= old_گنتی)
      واپس Ok();

    pair_سیٹ.len = گنتی;
    c.گراف.vertices_[this_اشاریہ].shrink_بفر ((old_گنتی - گنتی) * SmallTypes::سائز);

    TRY_تفویض (خودکار coverage, c.گراف.as_mutable_جدول<Coverage> (this_اشاریہ, &یہ->coverage));

    غیرعلامتی coverage_سائز = coverage.راس->جدول_سائز ();
    خودکار new_coverage =
        + hb_zip (coverage.جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<غیرعلامتی, غیرعلامتی> p) {
          واپس p.second < گنتی;
        })
        | hb_map_retains_ترتیب_کاری (hb_پہلا)
        ;

    واپس Coverage::make_coverage (c, new_coverage, coverage.اشاریہ_2, coverage_سائز);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  گراف_نتیجہ_t<غیرعلامتی> clone_range (gsubgpos_گراف_context_t& c,
                                        غیرعلامتی this_اشاریہ,
                                        غیرعلامتی آغاز, غیرعلامتی انتہا) ثابت
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_اشاریہ, آغاز, انتہا);

    غیرعلامتی عدد_pair_sets = انتہا - آغاز;
    غیرعلامتی مفرد_سائز = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_سائز
                          + عدد_pair_sets * SmallTypes::سائز;

    TRY_تفویض (غیرعلامتی pair_pos_مفرد_id, c.create_گرہ (مفرد_سائز));

    PairPosFormat1* pair_pos_مفرد = (PairPosFormat1*) c.گراف.object (pair_pos_مفرد_id).سر_2;
    pair_pos_مفرد->format = یہ->format;
    pair_pos_مفرد->قدر_Format_2[0] = یہ->قدر_Format_2[0];
    pair_pos_مفرد->قدر_Format_2[1] = یہ->قدر_Format_2[1];
    pair_pos_مفرد->pair_سیٹ.len = عدد_pair_sets;

    برائے (غیرعلامتی i = آغاز; i < انتہا; i++)
    {
      TRY (c.گراف.move_بچہ<> (this_اشاریہ,
                                 &pair_سیٹ[i],
                                 pair_pos_مفرد_id,
                                 &pair_pos_مفرد->pair_سیٹ[i - آغاز]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.گراف.vertices_[this_اشاریہ].ترتیب_دو_real_links ();

    TRY_تفویض (غیرعلامتی coverage_id, c.گراف.اشاریہ_for_انحراف (this_اشاریہ, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_مفرد_id,
                                   2,
                                   آغاز, انتہا));

    واپس Ok(pair_pos_مفرد_id);
  }



  گراف_نتیجہ_t<غیرعلامتی> pair_سیٹ_گراف_اشاریہ (gsubgpos_گراف_context_t& c, غیرعلامتی this_اشاریہ, غیرعلامتی i) ثابت
  {
    واپس c.گراف.اشاریہ_for_انحراف (this_اشاریہ, &pair_سیٹ[i]);
  }
};

ساخت PairPosFormat2 : عوامی OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  گراف_نتیجہ_t<خالی> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائز ();
    غیرعلامتی min_سائز = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_سائز;
    اگر (unlikely (راس_len < min_سائز)) واپس Err(SANITIZE_FAILURE);
    hb_barrier ();

    ثابت غیرعلامتی class1_گنتی = class1_گنتی_2;
    اگر (unlikely (راس_len < min_سائز + class1_گنتی * get_class1_ریکارڈ_سائز ()))
      واپس Err(SANITIZE_FAILURE);
    واپس Ok();
  }

  گراف_نتیجہ_t<hb_سمتیہ_t<غیرعلامتی>> بانٹو_subtables (gsubgpos_گراف_context_t& c,
                                                         غیرعلامتی this_اشاریہ)
  {
    ثابت غیرعلامتی بنیاد_سائز = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_سائز;
    TRY_تفویض (ثابت غیرعلامتی class_def_2_سائز, سائز_کا (c, this_اشاریہ, &classDef2));
    TRY_تفویض (ثابت Coverage* coverage, get_coverage (c, this_اشاریہ));
    TRY_تفویض (ثابت ClassDef* class_def_1, get_class_def_1 (c, this_اشاریہ));
    خودکار gid_and_class =
        + coverage->iter ()
        | hb_map_retains_ترتیب_کاری ([&] (hb_codepoint_t gid) {
          واپس hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_تفویض (class_def_سائز_estimator_t estimator, class_def_سائز_estimator_t::create (gid_and_class));

    ثابت غیرعلامتی class1_گنتی = class1_گنتی_2;
    ثابت غیرعلامتی class2_گنتی = class2_گنتی_2;
    ثابت غیرعلامتی class1_ریکارڈ_سائز = get_class1_ریکارڈ_سائز ();

    ثابت غیرعلامتی قدر_1_len = قدر_Format1.get_len ();
    ثابت غیرعلامتی قدر_2_len = قدر_Format2.get_len ();
    ثابت غیرعلامتی کل_قدر_len = قدر_1_len + قدر_2_len;

    غیرعلامتی accumulated = بنیاد_سائز;
    غیرعلامتی coverage_سائز = 4;
    غیرعلامتی class_def_1_سائز = 4;
    غیرعلامتی max_coverage_سائز = coverage_سائز;
    غیرعلامتی max_class_def_1_سائز = class_def_1_سائز;

    hb_سمتیہ_t<غیرعلامتی> بانٹو_points;

    TRY_تفویض (خودکار device_tables, get_all_device_tables (c, this_اشاریہ));
    hb_سمتیہ_t<غیرعلامتی> format1_device_جدول_indices = قدر_Format1.get_device_جدول_indices ();
    hb_سمتیہ_t<غیرعلامتی> format2_device_جدول_indices = قدر_Format2.get_device_جدول_indices ();
    منطقی has_device_tables = منطقی(format1_device_جدول_indices) || منطقی(format2_device_جدول_indices);

    hb_سیٹ_t دیکھا;
    برائے (غیرعلامتی i = 0; i < class1_گنتی; i++)
    {
      غیرعلامتی accumulated_delta = class1_ریکارڈ_سائز;
      class_def_1_سائز = estimator.جوڑو_class_def_سائز (i);
      coverage_سائز = estimator.coverage_سائز ();
      max_coverage_سائز = hb_max (max_coverage_سائز, coverage_سائز);
      max_class_def_1_سائز = hb_max (max_class_def_1_سائز, class_def_1_سائز);

      اگر (has_device_tables) {
        برائے (غیرعلامتی j = 0; j < class2_گنتی; j++)
        {
          غیرعلامتی value1_اشاریہ = کل_قدر_len * (class2_گنتی * i + j);
          غیرعلامتی value2_اشاریہ = value1_اشاریہ + قدر_1_len;
          accumulated_delta += سائز_کا_قدر_ریکارڈ_children (c,
                                                        device_tables,
                                                        format1_device_جدول_indices,
                                                        value1_اشاریہ,
                                                        دیکھا);
          accumulated_delta += سائز_کا_قدر_ریکارڈ_children (c,
                                                        device_tables,
                                                        format2_device_جدول_indices,
                                                        value2_اشاریہ,
                                                        دیکھا);
        }
      }

      accumulated += accumulated_delta;
      غیرعلامتی کل = accumulated
                       + coverage_سائز + class_def_1_سائز + class_def_2_سائز
                       // The largest object will pack last and can exceed the size limit.
                       - hb_max (hb_max (coverage_سائز, class_def_1_سائز), class_def_2_سائز);
      اگر (کل >= (1 << 16))
      {
        بانٹو_points.push (i);
        TRY (گراف_نتیجہ_t<خالی>::from (بانٹو_points, تخصیص_FAILURE));
        // split does not include i, so add the size for i when we reset the size counters.
        accumulated = بنیاد_سائز + accumulated_delta;

        estimator.reset();
        class_def_1_سائز = estimator.جوڑو_class_def_سائز(i);
        coverage_سائز = estimator.coverage_سائز();
        دیکھا.clear (); // node sharing isn't allowed between splits.
      }
    }

    بانٹو_context_t بانٹو_context {
      c,
      یہ,
      this_اشاریہ,
      class1_ریکارڈ_سائز,
      کل_قدر_len,
      قدر_1_len,
      قدر_2_len,
      max_coverage_سائز,
      max_class_def_1_سائز,
      device_tables,
      format1_device_جدول_indices,
      format2_device_جدول_indices
    };

    واپس actuate_subtable_بانٹو<بانٹو_context_t> (بانٹو_context, بانٹو_points);
  }
 نجی:

  ساخت بانٹو_context_t
  {
    gsubgpos_گراف_context_t& c;
    PairPosFormat2* thiz;
    غیرعلامتی this_اشاریہ;
    غیرعلامتی class1_ریکارڈ_سائز;
    غیرعلامتی قدر_ریکارڈ_len;
    غیرعلامتی value1_ریکارڈ_len;
    غیرعلامتی value2_ریکارڈ_len;
    غیرعلامتی max_coverage_سائز;
    غیرعلامتی max_class_def_سائز;

    ثابت hb_hashmap_t<غیرعلامتی, غیرعلامتی>& device_tables;
    ثابت hb_سمتیہ_t<غیرعلامتی>& format1_device_جدول_indices;
    ثابت hb_سمتیہ_t<غیرعلامتی>& format2_device_جدول_indices;

    غیرعلامتی original_گنتی_2 ()
    {
      واپس thiz->class1_گنتی_2;
    }

    گراف_نتیجہ_t<غیرعلامتی> clone_range (غیرعلامتی آغاز, غیرعلامتی انتہا)
    {
      واپس thiz->clone_range (*یہ, آغاز, انتہا);
    }

    گراف_نتیجہ_t<خالی> shrink (غیرعلامتی گنتی)
    {
      واپس thiz->shrink (*یہ, گنتی);
    }
  };

  size_t get_class1_ریکارڈ_سائز () ثابت
  {
    ثابت size_t class2_گنتی = class2_گنتی_2;
    واپس
        class2_گنتی * (قدر_Format1.get_سائز () + قدر_Format2.get_سائز ());
  }

  گراف_نتیجہ_t<غیرعلامتی> clone_range (بانٹو_context_t& بانٹو_context,
                                        غیرعلامتی آغاز, غیرعلامتی انتہا) ثابت
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", بانٹو_context.this_اشاریہ, آغاز, انتہا);

    گراف_t& گراف = بانٹو_context.c.گراف;

    غیرعلامتی عدد_اندراجات = انتہا - آغاز;
    غیرعلامتی مفرد_سائز = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_سائز
                          + عدد_اندراجات * بانٹو_context.class1_ریکارڈ_سائز;

    TRY_تفویض (غیرعلامتی pair_pos_مفرد_id, بانٹو_context.c.create_گرہ (مفرد_سائز));

    PairPosFormat2* pair_pos_مفرد =
        (PairPosFormat2*) گراف.object (pair_pos_مفرد_id).سر_2;
    pair_pos_مفرد->format = یہ->format;
    pair_pos_مفرد->قدر_Format1 = یہ->قدر_Format1;
    pair_pos_مفرد->قدر_Format2 = یہ->قدر_Format2;
    pair_pos_مفرد->class1_گنتی_2 = عدد_اندراجات;
    pair_pos_مفرد->class2_گنتی_2 = یہ->class2_گنتی_2;
    TRY (clone_class1_اندراجات (بانٹو_context,
                               pair_pos_مفرد_id,
                               آغاز,
                               انتہا));

    TRY_تفویض (غیرعلامتی coverage_id,
         گراف.اشاریہ_for_انحراف (بانٹو_context.this_اشاریہ, &coverage));
    TRY_تفویض (غیرعلامتی class_def_1_id,
         گراف.اشاریہ_for_انحراف (بانٹو_context.this_اشاریہ, &classDef1));

    خودکار& coverage_v = گراف.vertices_[coverage_id];
    خودکار& class_def_1_v = گراف.vertices_[class_def_1_id];
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    ثابت ClassDef* class_def_1_جدول = (ثابت ClassDef*) class_def_1_v.obj().سر_2;
    اگر (unlikely (!coverage_جدول || !class_def_1_جدول))
      واپس Err(INVALID_دلیل);
    TRY (coverage_جدول->sanitize (coverage_v));
    TRY (class_def_1_جدول->sanitize (class_def_1_v));

    خودکار klass_map =
    + coverage_جدول->iter ()
    | hb_map_retains_ترتیب_کاری ([&] (hb_codepoint_t gid) {
      واپس hb_codepoint_pair_t (gid, class_def_1_جدول->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      واپس klass >= آغاز && klass < انتہا;
    }, hb_دوسرا)
    | hb_map_retains_ترتیب_کاری ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      واپس hb_codepoint_pair_t (gid_and_class.پہلا, gid_and_class.second - آغاز);
    })
    ;

    TRY (Coverage::جوڑو_coverage (بانٹو_context.c,
                                 pair_pos_مفرد_id,
                                 2,
                                 + klass_map | hb_map_retains_ترتیب_کاری (hb_پہلا),
                                 بانٹو_context.max_coverage_سائز));

    // classDef1
    TRY (ClassDef::جوڑو_class_def (بانٹو_context.c,
                                  pair_pos_مفرد_id,
                                  8,
                                  + klass_map,
                                  بانٹو_context.max_class_def_سائز));

    // classDef2
    TRY_تفویض (غیرعلامتی class_def_2_id,
         گراف.اشاریہ_for_انحراف (بانٹو_context.this_اشاریہ, &classDef2));

    TRY(گراف.vertices_[pair_pos_مفرد_id].جوڑو_real_link (SmallTypes::سائز, class_def_2_id, 10));
    TRY(گراف.vertices_[class_def_2_id].جوڑو_والد (pair_pos_مفرد_id, جھوٹ));

    واپس Ok(pair_pos_مفرد_id);
  }

  گراف_نتیجہ_t<خالی> clone_class1_اندراجات (بانٹو_context_t& بانٹو_context,
                                             غیرعلامتی pair_pos_مفرد_id,
                                             غیرعلامتی آغاز, غیرعلامتی انتہا) ثابت
  {
    PairPosFormat2* pair_pos_مفرد =
        (PairPosFormat2*) بانٹو_context.c.گراف.object (pair_pos_مفرد_id).سر_2;

    حرف* آغاز_addr = ((حرف*)&values[0]) + آغاز * بانٹو_context.class1_ریکارڈ_سائز;
    غیرعلامتی عدد_اندراجات = انتہا - آغاز;
    hb_memcpy (&pair_pos_مفرد->values[0],
            آغاز_addr,
            عدد_اندراجات * بانٹو_context.class1_ریکارڈ_سائز);

    اگر (!بانٹو_context.format1_device_جدول_indices
        && !بانٹو_context.format2_device_جدول_indices)
      // No device tables to move over.
      واپس Ok();

    غیرعلامتی class2_گنتی = class2_گنتی_2;
    برائے (غیرعلامتی i = آغاز; i < انتہا; i++)
    {
      برائے (غیرعلامتی j = 0; j < class2_گنتی; j++)
      {
        غیرعلامتی value1_اشاریہ = بانٹو_context.قدر_ریکارڈ_len * (class2_گنتی * i + j);
        غیرعلامتی value2_اشاریہ = value1_اشاریہ + بانٹو_context.value1_ریکارڈ_len;

        غیرعلامتی new_value1_اشاریہ = بانٹو_context.قدر_ریکارڈ_len * (class2_گنتی * (i - آغاز) + j);
        غیرعلامتی new_value2_اشاریہ = new_value1_اشاریہ + بانٹو_context.value1_ریکارڈ_len;

        TRY (transfer_device_tables (بانٹو_context,
                                     pair_pos_مفرد_id,
                                     بانٹو_context.format1_device_جدول_indices,
                                     value1_اشاریہ,
                                     new_value1_اشاریہ));

        TRY (transfer_device_tables (بانٹو_context,
                                     pair_pos_مفرد_id,
                                     بانٹو_context.format2_device_جدول_indices,
                                     value2_اشاریہ,
                                     new_value2_اشاریہ));
      }
    }
    واپس Ok();
  }

  گراف_نتیجہ_t<خالی> transfer_device_tables (بانٹو_context_t& بانٹو_context,
                                               غیرعلامتی pair_pos_مفرد_id,
                                               ثابت hb_سمتیہ_t<غیرعلامتی>& device_جدول_indices,
                                               غیرعلامتی old_قدر_ریکارڈ_اشاریہ,
                                               غیرعلامتی new_قدر_ریکارڈ_اشاریہ) ثابت
  {
    PairPosFormat2* pair_pos_مفرد =
        (PairPosFormat2*) بانٹو_context.c.گراف.object (pair_pos_مفرد_id).سر_2;

    برائے (غیرعلامتی i : device_جدول_indices)
    {
      OT::Offset16* ریکارڈ = (OT::Offset16*) &values[old_قدر_ریکارڈ_اشاریہ + i];
      غیرعلامتی ریکارڈ_position = ((حرف*) ریکارڈ) - ((حرف*) یہ);
      اگر (!بانٹو_context.device_tables.has (ریکارڈ_position)) جاری;

      TRY (بانٹو_context.c.گراف.move_بچہ (
          بانٹو_context.this_اشاریہ,
          ریکارڈ,
          pair_pos_مفرد_id,
          (OT::Offset16*) &pair_pos_مفرد->values[new_قدر_ریکارڈ_اشاریہ + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    بانٹو_context.c.گراف.vertices_[بانٹو_context.this_اشاریہ].ترتیب_دو_real_links ();

    واپس Ok();
  }

  گراف_نتیجہ_t<خالی> shrink (بانٹو_context_t& بانٹو_context,
                               غیرعلامتی گنتی)
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               بانٹو_context.this_اشاریہ,
               گنتی);
    غیرعلامتی old_گنتی = class1_گنتی_2;
    اگر (گنتی >= old_گنتی)
      واپس Ok();

    گراف_t& گراف = بانٹو_context.c.گراف;
    class1_گنتی_2 = گنتی;
    گراف.vertices_[بانٹو_context.this_اشاریہ].shrink_بفر(
        (old_گنتی - گنتی) * بانٹو_context.class1_ریکارڈ_سائز);

    TRY_تفویض (خودکار coverage,
         گراف.as_mutable_جدول<Coverage> (بانٹو_context.this_اشاریہ, &یہ->coverage));

    TRY_تفویض (خودکار class_def_1,
         گراف.as_mutable_جدول<ClassDef> (بانٹو_context.this_اشاریہ, &classDef1));

    خودکار klass_map =
    + coverage.جدول->iter ()
    | hb_map_retains_ترتیب_کاری ([&] (hb_codepoint_t gid) {
      واپس hb_codepoint_pair_t (gid, class_def_1.جدول->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      واپس klass < گنتی;
    }, hb_دوسرا)
    ;

    خودکار new_coverage = + klass_map | hb_map_retains_ترتیب_کاری (hb_پہلا);
    TRY (Coverage::make_coverage (بانٹو_context.c,
                                  + new_coverage,
                                  coverage.اشاریہ_2,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    واپس ClassDef::make_class_def (بانٹو_context.c,
                                     + klass_map,
                                     class_def_1.اشاریہ_2,
                                     class_def_1.راس->جدول_سائز ());
  }

  گراف_نتیجہ_t<hb_hashmap_t<غیرعلامتی, غیرعلامتی>>
  get_all_device_tables (gsubgpos_گراف_context_t& c,
                         غیرعلامتی this_اشاریہ) ثابت
  {
    ثابت خودکار& v = c.گراف.vertices_[this_اشاریہ];
    واپس v.position_مقصد_اشاریہ_map ();
  }

  گراف_نتیجہ_t<ثابت Coverage*> get_coverage (gsubgpos_گراف_context_t& c,
                                                غیرعلامتی this_اشاریہ) ثابت
  {
    TRY_تفویض (غیرعلامتی coverage_id, c.گراف.اشاریہ_for_انحراف (this_اشاریہ, &coverage));
    خودکار& coverage_v = c.گراف.vertices_[coverage_id];

    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    اگر (unlikely (!coverage_جدول)) واپس Err(INVALID_دلیل);
    TRY(coverage_جدول->sanitize (coverage_v));

    واپس coverage_جدول;
  }

  گراف_نتیجہ_t<ثابت ClassDef*> get_class_def_1 (gsubgpos_گراف_context_t& c,
                                                   غیرعلامتی this_اشاریہ) ثابت
  {
    TRY_تفویض (غیرعلامتی class_def_1_id, c.گراف.اشاریہ_for_انحراف (this_اشاریہ, &classDef1));
    خودکار& class_def_1_v = c.گراف.vertices_[class_def_1_id];

    ثابت ClassDef* class_def_1_جدول = (ثابت ClassDef*) class_def_1_v.obj().سر_2;
    اگر (unlikely (!class_def_1_جدول)) واپس Err(INVALID_دلیل);
    TRY(class_def_1_جدول->sanitize (class_def_1_v));
    واپس class_def_1_جدول;
  }

  غیرعلامتی سائز_کا_قدر_ریکارڈ_children (gsubgpos_گراف_context_t& c,
                                          ثابت hb_hashmap_t<غیرعلامتی, غیرعلامتی>& device_tables,
                                          ثابت hb_سمتیہ_t<غیرعلامتی> device_جدول_indices,
                                          غیرعلامتی قدر_ریکارڈ_اشاریہ,
                                          hb_سیٹ_t& دیکھا)
  {
    غیرعلامتی سائز = 0;
    برائے (غیرعلامتی i : device_جدول_indices)
    {
      OT::Layout::GPOS_impl::قدر* ریکارڈ = &values[قدر_ریکارڈ_اشاریہ + i];
      غیرعلامتی ریکارڈ_position = ((حرف*) ریکارڈ) - ((حرف*) یہ);
      غیرعلامتی* obj_idx;
      اگر (!device_tables.has (ریکارڈ_position, &obj_idx)) جاری;
      سائز += c.گراف.ڈھونڈو_subgraph_سائز (*obj_idx, دیکھا).قدر_or (0);
    }
    واپس سائز;
  }

  گراف_نتیجہ_t<غیرعلامتی> سائز_کا (gsubgpos_گراف_context_t& c,
                                    غیرعلامتی this_اشاریہ,
                                    ثابت خالی* انحراف) ثابت
  {
    TRY_تفویض (ثابت غیرعلامتی id, c.گراف.اشاریہ_for_انحراف (this_اشاریہ, انحراف));
    واپس Ok(c.گراف.vertices_[id].جدول_سائز ());
  }
};

ساخت PairPos : عوامی OT::Layout::GPOS_impl::PairPos
{
  گراف_نتیجہ_t<hb_سمتیہ_t<غیرعلامتی>> بانٹو_subtables (gsubgpos_گراف_context_t& c,
                                                         غیرعلامتی this_اشاریہ)
  {
    انتخاب (u.format.v) {
    حالت 1:
      hb_barrier ();
      واپس ((PairPosFormat1*)(&u.format1))->بانٹو_subtables (c, this_اشاریہ);
    حالت 2:
      hb_barrier ();
      واپس ((PairPosFormat2*)(&u.format2))->بانٹو_subtables (c, this_اشاریہ);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: HB_FALLTHROUGH;
    حالت 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#اگر_ختم
    طےشدہ:
      واپس Ok(hb_سمتیہ_t<غیرعلامتی> ());
    }
  }

  گراف_نتیجہ_t<خالی> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائز();
    اگر (unlikely (راس_len < u.format.v.get_سائز ())) واپس Err(SANITIZE_FAILURE);
    hb_barrier ();

    انتخاب (u.format.v) {
    حالت 1:
      hb_barrier ();
      واپس ((PairPosFormat1*)(&u.format1))->sanitize (راس);
    حالت 2:
      hb_barrier ();
      واپس ((PairPosFormat2*)(&u.format2))->sanitize (راس);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: HB_FALLTHROUGH;
    حالت 4: HB_FALLTHROUGH;
#اگر_ختم
    طےشدہ:
      // We don't handle format 3 and 4 here.
      واپس Err(SANITIZE_FAILURE);
    }
  }
};

}

#اگر_ختم  // GRAPH_PAIRPOS_GRAPH_HH
