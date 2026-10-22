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

#جيڪڏهن_وصف_نه_ٿيل گراف_PAIRPOS_گراف_HH
#وصف_ڪريو گراف_PAIRPOS_گراف_HH

#ضمن "split-helpers.hh"
#ضمن "coverage-graph.hh"
#ضمن "classdef-graph.hh"
#ضمن "../OT/Layout/GPOS/PairPos.hh"
#ضمن "../OT/Layout/GPOS/PosLookupSubTable.hh"

نطاق گراف {

بنية PairPosFormat1 : عام OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  گراف_نتيجو_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائيز ();
    بنانشان min_سائيز = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_سائيز;
    إذا (unlikely (راس_len < min_سائيز)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (راس_len < min_سائيز + pair_سيٽ.get_سائيز () - pair_سيٽ.len.get_سائيز()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتيجو_t<hb_ويڪٽر_t<بنانشان>> ورهايو_subtables (gsubgpos_گراف_context_t& c,
                                                         بنانشان this_اشاريو)
  {
    hb_سيٽ_t گهميل;

    TRY_سونپيو (ثابت بنانشان coverage_id, c.گراف.اشاريو_for_وٿي (this_اشاريو, &coverage));
    ثابت بنانشان coverage_سائيز = c.گراف.vertices_[coverage_id].جدول_سائيز ();
    ثابت بنانشان بنياد_سائيز = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_سائيز;

    بنانشان partial_coverage_سائيز = 4;
    بنانشان accumulated = بنياد_سائيز;
    hb_ويڪٽر_t<بنانشان> ورهايو_points;
    لكل (بنانشان i = 0; i < pair_سيٽ.len; i++)
    {
      TRY_سونپيو (بنانشان pair_سيٽ_اشاريو, pair_سيٽ_گراف_اشاريو (c, this_اشاريو, i));
      بنانشان accumulated_delta =
          c.گراف.ڳوليو_subgraph_سائيز (pair_سيٽ_اشاريو, گهميل).قدر_or (0) +
          SmallTypes::سائيز; // for PairSet offset.
      partial_coverage_سائيز += OT::HBUINT16::static_سائيز;

      accumulated += accumulated_delta;
      بنانشان ڪل = accumulated + hb_min (partial_coverage_سائيز, coverage_سائيز);

      إذا (ڪل >= (1 << 16))
      {
        ورهايو_points.push (i);
        TRY (گراف_نتيجو_t<فراغ>::from (ورهايو_points, ورهاست_FAILURE));
        accumulated = بنياد_سائيز + accumulated_delta;
        partial_coverage_سائيز = 6;
        گهميل.clear (); // node sharing isn't allowed between splits.
      }
    }

    ورهايو_context_t ورهايو_context {
      c,
      هذا,
      this_اشاريو,
    };

    أرجع actuate_subtable_ورهايو<ورهايو_context_t> (ورهايو_context, ورهايو_points);
  }

 خاص:

  بنية ورهايو_context_t {
    gsubgpos_گراف_context_t& c;
    PairPosFormat1* thiz;
    بنانشان this_اشاريو;

    بنانشان original_ڳڻپ_2 ()
    {
      أرجع thiz->pair_سيٽ.len;
    }

    گراف_نتيجو_t<بنانشان> clone_range (بنانشان شروعات, بنانشان پڇاڙي)
    {
      أرجع thiz->clone_range (هذا->c, هذا->this_اشاريو, شروعات, پڇاڙي);
    }

    گراف_نتيجو_t<فراغ> shrink (بنانشان ڳڻپ)
    {
      أرجع thiz->shrink (هذا->c, هذا->this_اشاريو, ڳڻپ);
    }
  };

  گراف_نتيجو_t<فراغ> shrink (gsubgpos_گراف_context_t& c,
                               بنانشان this_اشاريو,
                               بنانشان ڳڻپ)
  {
    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_اشاريو,
               ڳڻپ);
    بنانشان old_ڳڻپ = pair_سيٽ.len;
    إذا (ڳڻپ >= old_ڳڻپ)
      أرجع Ok();

    pair_سيٽ.len = ڳڻپ;
    c.گراف.vertices_[this_اشاريو].shrink_بفر ((old_ڳڻپ - ڳڻپ) * SmallTypes::سائيز);

    TRY_سونپيو (خودڪار coverage, c.گراف.as_mutable_جدول<Coverage> (this_اشاريو, &هذا->coverage));

    بنانشان coverage_سائيز = coverage.راس->جدول_سائيز ();
    خودڪار new_coverage =
        + hb_zip (coverage.جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<بنانشان, بنانشان> p) {
          أرجع p.second < ڳڻپ;
        })
        | hb_map_retains_ترتيب_ڏيڻ (hb_پهريون)
        ;

    أرجع Coverage::make_coverage (c, new_coverage, coverage.اشاريو_2, coverage_سائيز);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  گراف_نتيجو_t<بنانشان> clone_range (gsubgpos_گراف_context_t& c,
                                        بنانشان this_اشاريو,
                                        بنانشان شروعات, بنانشان پڇاڙي) ثابت
  {
    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_اشاريو, شروعات, پڇاڙي);

    بنانشان انگ_pair_sets = پڇاڙي - شروعات;
    بنانشان مفرد_سائيز = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_سائيز
                          + انگ_pair_sets * SmallTypes::سائيز;

    TRY_سونپيو (بنانشان pair_pos_مفرد_id, c.create_ڳنڍ (مفرد_سائيز));

    PairPosFormat1* pair_pos_مفرد = (PairPosFormat1*) c.گراف.object (pair_pos_مفرد_id).سر_2;
    pair_pos_مفرد->format = هذا->format;
    pair_pos_مفرد->قدر_Format_2[0] = هذا->قدر_Format_2[0];
    pair_pos_مفرد->قدر_Format_2[1] = هذا->قدر_Format_2[1];
    pair_pos_مفرد->pair_سيٽ.len = انگ_pair_sets;

    لكل (بنانشان i = شروعات; i < پڇاڙي; i++)
    {
      TRY (c.گراف.move_ٻار<> (this_اشاريو,
                                 &pair_سيٽ[i],
                                 pair_pos_مفرد_id,
                                 &pair_pos_مفرد->pair_سيٽ[i - شروعات]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.گراف.vertices_[this_اشاريو].ترتيب_ڏيو_real_links ();

    TRY_سونپيو (بنانشان coverage_id, c.گراف.اشاريو_for_وٿي (this_اشاريو, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_مفرد_id,
                                   2,
                                   شروعات, پڇاڙي));

    أرجع Ok(pair_pos_مفرد_id);
  }



  گراف_نتيجو_t<بنانشان> pair_سيٽ_گراف_اشاريو (gsubgpos_گراف_context_t& c, بنانشان this_اشاريو, بنانشان i) ثابت
  {
    أرجع c.گراف.اشاريو_for_وٿي (this_اشاريو, &pair_سيٽ[i]);
  }
};

بنية PairPosFormat2 : عام OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  گراف_نتيجو_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائيز ();
    بنانشان min_سائيز = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_سائيز;
    إذا (unlikely (راس_len < min_سائيز)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    ثابت بنانشان class1_ڳڻپ = class1_ڳڻپ_2;
    إذا (unlikely (راس_len < min_سائيز + class1_ڳڻپ * get_class1_رڪارڊ_سائيز ()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  گراف_نتيجو_t<hb_ويڪٽر_t<بنانشان>> ورهايو_subtables (gsubgpos_گراف_context_t& c,
                                                         بنانشان this_اشاريو)
  {
    ثابت بنانشان بنياد_سائيز = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_سائيز;
    TRY_سونپيو (ثابت بنانشان class_def_2_سائيز, سائيز_جو (c, this_اشاريو, &classDef2));
    TRY_سونپيو (ثابت Coverage* coverage, get_coverage (c, this_اشاريو));
    TRY_سونپيو (ثابت ClassDef* class_def_1, get_class_def_1 (c, this_اشاريو));
    خودڪار gid_and_class =
        + coverage->iter ()
        | hb_map_retains_ترتيب_ڏيڻ ([&] (hb_codepoint_t gid) {
          أرجع hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_سونپيو (class_def_سائيز_estimator_t estimator, class_def_سائيز_estimator_t::create (gid_and_class));

    ثابت بنانشان class1_ڳڻپ = class1_ڳڻپ_2;
    ثابت بنانشان class2_ڳڻپ = class2_ڳڻپ_2;
    ثابت بنانشان class1_رڪارڊ_سائيز = get_class1_رڪارڊ_سائيز ();

    ثابت بنانشان قدر_1_len = قدر_Format1.get_len ();
    ثابت بنانشان قدر_2_len = قدر_Format2.get_len ();
    ثابت بنانشان ڪل_قدر_len = قدر_1_len + قدر_2_len;

    بنانشان accumulated = بنياد_سائيز;
    بنانشان coverage_سائيز = 4;
    بنانشان class_def_1_سائيز = 4;
    بنانشان max_coverage_سائيز = coverage_سائيز;
    بنانشان max_class_def_1_سائيز = class_def_1_سائيز;

    hb_ويڪٽر_t<بنانشان> ورهايو_points;

    TRY_سونپيو (خودڪار device_tables, get_all_device_tables (c, this_اشاريو));
    hb_ويڪٽر_t<بنانشان> format1_device_جدول_indices = قدر_Format1.get_device_جدول_indices ();
    hb_ويڪٽر_t<بنانشان> format2_device_جدول_indices = قدر_Format2.get_device_جدول_indices ();
    منطقي has_device_tables = منطقي(format1_device_جدول_indices) || منطقي(format2_device_جدول_indices);

    hb_سيٽ_t گهميل;
    لكل (بنانشان i = 0; i < class1_ڳڻپ; i++)
    {
      بنانشان accumulated_delta = class1_رڪارڊ_سائيز;
      class_def_1_سائيز = estimator.جوڙيو_class_def_سائيز (i);
      coverage_سائيز = estimator.coverage_سائيز ();
      max_coverage_سائيز = hb_max (max_coverage_سائيز, coverage_سائيز);
      max_class_def_1_سائيز = hb_max (max_class_def_1_سائيز, class_def_1_سائيز);

      إذا (has_device_tables) {
        لكل (بنانشان j = 0; j < class2_ڳڻپ; j++)
        {
          بنانشان value1_اشاريو = ڪل_قدر_len * (class2_ڳڻپ * i + j);
          بنانشان value2_اشاريو = value1_اشاريو + قدر_1_len;
          accumulated_delta += سائيز_جو_قدر_رڪارڊ_children (c,
                                                        device_tables,
                                                        format1_device_جدول_indices,
                                                        value1_اشاريو,
                                                        گهميل);
          accumulated_delta += سائيز_جو_قدر_رڪارڊ_children (c,
                                                        device_tables,
                                                        format2_device_جدول_indices,
                                                        value2_اشاريو,
                                                        گهميل);
        }
      }

      accumulated += accumulated_delta;
      بنانشان ڪل = accumulated
                       + coverage_سائيز + class_def_1_سائيز + class_def_2_سائيز
                       // The largest object will pack last and can exceed the size limit.
                       - hb_max (hb_max (coverage_سائيز, class_def_1_سائيز), class_def_2_سائيز);
      إذا (ڪل >= (1 << 16))
      {
        ورهايو_points.push (i);
        TRY (گراف_نتيجو_t<فراغ>::from (ورهايو_points, ورهاست_FAILURE));
        // split does not include i, so add the size for i when we reset the size counters.
        accumulated = بنياد_سائيز + accumulated_delta;

        estimator.reset();
        class_def_1_سائيز = estimator.جوڙيو_class_def_سائيز(i);
        coverage_سائيز = estimator.coverage_سائيز();
        گهميل.clear (); // node sharing isn't allowed between splits.
      }
    }

    ورهايو_context_t ورهايو_context {
      c,
      هذا,
      this_اشاريو,
      class1_رڪارڊ_سائيز,
      ڪل_قدر_len,
      قدر_1_len,
      قدر_2_len,
      max_coverage_سائيز,
      max_class_def_1_سائيز,
      device_tables,
      format1_device_جدول_indices,
      format2_device_جدول_indices
    };

    أرجع actuate_subtable_ورهايو<ورهايو_context_t> (ورهايو_context, ورهايو_points);
  }
 خاص:

  بنية ورهايو_context_t
  {
    gsubgpos_گراف_context_t& c;
    PairPosFormat2* thiz;
    بنانشان this_اشاريو;
    بنانشان class1_رڪارڊ_سائيز;
    بنانشان قدر_رڪارڊ_len;
    بنانشان value1_رڪارڊ_len;
    بنانشان value2_رڪارڊ_len;
    بنانشان max_coverage_سائيز;
    بنانشان max_class_def_سائيز;

    ثابت hb_hashmap_t<بنانشان, بنانشان>& device_tables;
    ثابت hb_ويڪٽر_t<بنانشان>& format1_device_جدول_indices;
    ثابت hb_ويڪٽر_t<بنانشان>& format2_device_جدول_indices;

    بنانشان original_ڳڻپ_2 ()
    {
      أرجع thiz->class1_ڳڻپ_2;
    }

    گراف_نتيجو_t<بنانشان> clone_range (بنانشان شروعات, بنانشان پڇاڙي)
    {
      أرجع thiz->clone_range (*هذا, شروعات, پڇاڙي);
    }

    گراف_نتيجو_t<فراغ> shrink (بنانشان ڳڻپ)
    {
      أرجع thiz->shrink (*هذا, ڳڻپ);
    }
  };

  size_t get_class1_رڪارڊ_سائيز () ثابت
  {
    ثابت size_t class2_ڳڻپ = class2_ڳڻپ_2;
    أرجع
        class2_ڳڻپ * (قدر_Format1.get_سائيز () + قدر_Format2.get_سائيز ());
  }

  گراف_نتيجو_t<بنانشان> clone_range (ورهايو_context_t& ورهايو_context,
                                        بنانشان شروعات, بنانشان پڇاڙي) ثابت
  {
    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", ورهايو_context.this_اشاريو, شروعات, پڇاڙي);

    گراف_t& گراف = ورهايو_context.c.گراف;

    بنانشان انگ_رڪارڊون = پڇاڙي - شروعات;
    بنانشان مفرد_سائيز = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_سائيز
                          + انگ_رڪارڊون * ورهايو_context.class1_رڪارڊ_سائيز;

    TRY_سونپيو (بنانشان pair_pos_مفرد_id, ورهايو_context.c.create_ڳنڍ (مفرد_سائيز));

    PairPosFormat2* pair_pos_مفرد =
        (PairPosFormat2*) گراف.object (pair_pos_مفرد_id).سر_2;
    pair_pos_مفرد->format = هذا->format;
    pair_pos_مفرد->قدر_Format1 = هذا->قدر_Format1;
    pair_pos_مفرد->قدر_Format2 = هذا->قدر_Format2;
    pair_pos_مفرد->class1_ڳڻپ_2 = انگ_رڪارڊون;
    pair_pos_مفرد->class2_ڳڻپ_2 = هذا->class2_ڳڻپ_2;
    TRY (clone_class1_رڪارڊون (ورهايو_context,
                               pair_pos_مفرد_id,
                               شروعات,
                               پڇاڙي));

    TRY_سونپيو (بنانشان coverage_id,
         گراف.اشاريو_for_وٿي (ورهايو_context.this_اشاريو, &coverage));
    TRY_سونپيو (بنانشان class_def_1_id,
         گراف.اشاريو_for_وٿي (ورهايو_context.this_اشاريو, &classDef1));

    خودڪار& coverage_v = گراف.vertices_[coverage_id];
    خودڪار& class_def_1_v = گراف.vertices_[class_def_1_id];
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    ثابت ClassDef* class_def_1_جدول = (ثابت ClassDef*) class_def_1_v.obj().سر_2;
    إذا (unlikely (!coverage_جدول || !class_def_1_جدول))
      أرجع Err(INVALID_دليل);
    TRY (coverage_جدول->sanitize (coverage_v));
    TRY (class_def_1_جدول->sanitize (class_def_1_v));

    خودڪار klass_map =
    + coverage_جدول->iter ()
    | hb_map_retains_ترتيب_ڏيڻ ([&] (hb_codepoint_t gid) {
      أرجع hb_codepoint_pair_t (gid, class_def_1_جدول->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      أرجع klass >= شروعات && klass < پڇاڙي;
    }, hb_ٻيو)
    | hb_map_retains_ترتيب_ڏيڻ ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      أرجع hb_codepoint_pair_t (gid_and_class.پهريون, gid_and_class.second - شروعات);
    })
    ;

    TRY (Coverage::جوڙيو_coverage (ورهايو_context.c,
                                 pair_pos_مفرد_id,
                                 2,
                                 + klass_map | hb_map_retains_ترتيب_ڏيڻ (hb_پهريون),
                                 ورهايو_context.max_coverage_سائيز));

    // classDef1
    TRY (ClassDef::جوڙيو_class_def (ورهايو_context.c,
                                  pair_pos_مفرد_id,
                                  8,
                                  + klass_map,
                                  ورهايو_context.max_class_def_سائيز));

    // classDef2
    TRY_سونپيو (بنانشان class_def_2_id,
         گراف.اشاريو_for_وٿي (ورهايو_context.this_اشاريو, &classDef2));

    TRY(گراف.vertices_[pair_pos_مفرد_id].جوڙيو_real_link (SmallTypes::سائيز, class_def_2_id, 10));
    TRY(گراف.vertices_[class_def_2_id].جوڙيو_والد (pair_pos_مفرد_id, خطأ));

    أرجع Ok(pair_pos_مفرد_id);
  }

  گراف_نتيجو_t<فراغ> clone_class1_رڪارڊون (ورهايو_context_t& ورهايو_context,
                                             بنانشان pair_pos_مفرد_id,
                                             بنانشان شروعات, بنانشان پڇاڙي) ثابت
  {
    PairPosFormat2* pair_pos_مفرد =
        (PairPosFormat2*) ورهايو_context.c.گراف.object (pair_pos_مفرد_id).سر_2;

    حرف* شروعات_addr = ((حرف*)&values[0]) + شروعات * ورهايو_context.class1_رڪارڊ_سائيز;
    بنانشان انگ_رڪارڊون = پڇاڙي - شروعات;
    hb_memcpy (&pair_pos_مفرد->values[0],
            شروعات_addr,
            انگ_رڪارڊون * ورهايو_context.class1_رڪارڊ_سائيز);

    إذا (!ورهايو_context.format1_device_جدول_indices
        && !ورهايو_context.format2_device_جدول_indices)
      // No device tables to move over.
      أرجع Ok();

    بنانشان class2_ڳڻپ = class2_ڳڻپ_2;
    لكل (بنانشان i = شروعات; i < پڇاڙي; i++)
    {
      لكل (بنانشان j = 0; j < class2_ڳڻپ; j++)
      {
        بنانشان value1_اشاريو = ورهايو_context.قدر_رڪارڊ_len * (class2_ڳڻپ * i + j);
        بنانشان value2_اشاريو = value1_اشاريو + ورهايو_context.value1_رڪارڊ_len;

        بنانشان new_value1_اشاريو = ورهايو_context.قدر_رڪارڊ_len * (class2_ڳڻپ * (i - شروعات) + j);
        بنانشان new_value2_اشاريو = new_value1_اشاريو + ورهايو_context.value1_رڪارڊ_len;

        TRY (transfer_device_tables (ورهايو_context,
                                     pair_pos_مفرد_id,
                                     ورهايو_context.format1_device_جدول_indices,
                                     value1_اشاريو,
                                     new_value1_اشاريو));

        TRY (transfer_device_tables (ورهايو_context,
                                     pair_pos_مفرد_id,
                                     ورهايو_context.format2_device_جدول_indices,
                                     value2_اشاريو,
                                     new_value2_اشاريو));
      }
    }
    أرجع Ok();
  }

  گراف_نتيجو_t<فراغ> transfer_device_tables (ورهايو_context_t& ورهايو_context,
                                               بنانشان pair_pos_مفرد_id,
                                               ثابت hb_ويڪٽر_t<بنانشان>& device_جدول_indices,
                                               بنانشان old_قدر_رڪارڊ_اشاريو,
                                               بنانشان new_قدر_رڪارڊ_اشاريو) ثابت
  {
    PairPosFormat2* pair_pos_مفرد =
        (PairPosFormat2*) ورهايو_context.c.گراف.object (pair_pos_مفرد_id).سر_2;

    لكل (بنانشان i : device_جدول_indices)
    {
      OT::Offset16* رڪارڊ = (OT::Offset16*) &values[old_قدر_رڪارڊ_اشاريو + i];
      بنانشان رڪارڊ_position = ((حرف*) رڪارڊ) - ((حرف*) هذا);
      إذا (!ورهايو_context.device_tables.has (رڪارڊ_position)) استمر;

      TRY (ورهايو_context.c.گراف.move_ٻار (
          ورهايو_context.this_اشاريو,
          رڪارڊ,
          pair_pos_مفرد_id,
          (OT::Offset16*) &pair_pos_مفرد->values[new_قدر_رڪارڊ_اشاريو + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    ورهايو_context.c.گراف.vertices_[ورهايو_context.this_اشاريو].ترتيب_ڏيو_real_links ();

    أرجع Ok();
  }

  گراف_نتيجو_t<فراغ> shrink (ورهايو_context_t& ورهايو_context,
                               بنانشان ڳڻپ)
  {
    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               ورهايو_context.this_اشاريو,
               ڳڻپ);
    بنانشان old_ڳڻپ = class1_ڳڻپ_2;
    إذا (ڳڻپ >= old_ڳڻپ)
      أرجع Ok();

    گراف_t& گراف = ورهايو_context.c.گراف;
    class1_ڳڻپ_2 = ڳڻپ;
    گراف.vertices_[ورهايو_context.this_اشاريو].shrink_بفر(
        (old_ڳڻپ - ڳڻپ) * ورهايو_context.class1_رڪارڊ_سائيز);

    TRY_سونپيو (خودڪار coverage,
         گراف.as_mutable_جدول<Coverage> (ورهايو_context.this_اشاريو, &هذا->coverage));

    TRY_سونپيو (خودڪار class_def_1,
         گراف.as_mutable_جدول<ClassDef> (ورهايو_context.this_اشاريو, &classDef1));

    خودڪار klass_map =
    + coverage.جدول->iter ()
    | hb_map_retains_ترتيب_ڏيڻ ([&] (hb_codepoint_t gid) {
      أرجع hb_codepoint_pair_t (gid, class_def_1.جدول->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      أرجع klass < ڳڻپ;
    }, hb_ٻيو)
    ;

    خودڪار new_coverage = + klass_map | hb_map_retains_ترتيب_ڏيڻ (hb_پهريون);
    TRY (Coverage::make_coverage (ورهايو_context.c,
                                  + new_coverage,
                                  coverage.اشاريو_2,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    أرجع ClassDef::make_class_def (ورهايو_context.c,
                                     + klass_map,
                                     class_def_1.اشاريو_2,
                                     class_def_1.راس->جدول_سائيز ());
  }

  گراف_نتيجو_t<hb_hashmap_t<بنانشان, بنانشان>>
  get_all_device_tables (gsubgpos_گراف_context_t& c,
                         بنانشان this_اشاريو) ثابت
  {
    ثابت خودڪار& v = c.گراف.vertices_[this_اشاريو];
    أرجع v.position_منزل_اشاريو_map ();
  }

  گراف_نتيجو_t<ثابت Coverage*> get_coverage (gsubgpos_گراف_context_t& c,
                                                بنانشان this_اشاريو) ثابت
  {
    TRY_سونپيو (بنانشان coverage_id, c.گراف.اشاريو_for_وٿي (this_اشاريو, &coverage));
    خودڪار& coverage_v = c.گراف.vertices_[coverage_id];

    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    إذا (unlikely (!coverage_جدول)) أرجع Err(INVALID_دليل);
    TRY(coverage_جدول->sanitize (coverage_v));

    أرجع coverage_جدول;
  }

  گراف_نتيجو_t<ثابت ClassDef*> get_class_def_1 (gsubgpos_گراف_context_t& c,
                                                   بنانشان this_اشاريو) ثابت
  {
    TRY_سونپيو (بنانشان class_def_1_id, c.گراف.اشاريو_for_وٿي (this_اشاريو, &classDef1));
    خودڪار& class_def_1_v = c.گراف.vertices_[class_def_1_id];

    ثابت ClassDef* class_def_1_جدول = (ثابت ClassDef*) class_def_1_v.obj().سر_2;
    إذا (unlikely (!class_def_1_جدول)) أرجع Err(INVALID_دليل);
    TRY(class_def_1_جدول->sanitize (class_def_1_v));
    أرجع class_def_1_جدول;
  }

  بنانشان سائيز_جو_قدر_رڪارڊ_children (gsubgpos_گراف_context_t& c,
                                          ثابت hb_hashmap_t<بنانشان, بنانشان>& device_tables,
                                          ثابت hb_ويڪٽر_t<بنانشان> device_جدول_indices,
                                          بنانشان قدر_رڪارڊ_اشاريو,
                                          hb_سيٽ_t& گهميل)
  {
    بنانشان سائيز = 0;
    لكل (بنانشان i : device_جدول_indices)
    {
      OT::Layout::GPOS_impl::قدر* رڪارڊ = &values[قدر_رڪارڊ_اشاريو + i];
      بنانشان رڪارڊ_position = ((حرف*) رڪارڊ) - ((حرف*) هذا);
      بنانشان* obj_idx;
      إذا (!device_tables.has (رڪارڊ_position, &obj_idx)) استمر;
      سائيز += c.گراف.ڳوليو_subgraph_سائيز (*obj_idx, گهميل).قدر_or (0);
    }
    أرجع سائيز;
  }

  گراف_نتيجو_t<بنانشان> سائيز_جو (gsubgpos_گراف_context_t& c,
                                    بنانشان this_اشاريو,
                                    ثابت فراغ* وٿي) ثابت
  {
    TRY_سونپيو (ثابت بنانشان id, c.گراف.اشاريو_for_وٿي (this_اشاريو, وٿي));
    أرجع Ok(c.گراف.vertices_[id].جدول_سائيز ());
  }
};

بنية PairPos : عام OT::Layout::GPOS_impl::PairPos
{
  گراف_نتيجو_t<hb_ويڪٽر_t<بنانشان>> ورهايو_subtables (gsubgpos_گراف_context_t& c,
                                                         بنانشان this_اشاريو)
  {
    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((PairPosFormat1*)(&u.format1))->ورهايو_subtables (c, this_اشاريو);
    حالة 2:
      hb_barrier ();
      أرجع ((PairPosFormat2*)(&u.format2))->ورهايو_subtables (c, this_اشاريو);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: HB_FALLTHROUGH;
    حالة 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#جيڪڏهن_پورو
    افتراضي:
      أرجع Ok(hb_ويڪٽر_t<بنانشان> ());
    }
  }

  گراف_نتيجو_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائيز();
    إذا (unlikely (راس_len < u.format.v.get_سائيز ())) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((PairPosFormat1*)(&u.format1))->sanitize (راس);
    حالة 2:
      hb_barrier ();
      أرجع ((PairPosFormat2*)(&u.format2))->sanitize (راس);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: HB_FALLTHROUGH;
    حالة 4: HB_FALLTHROUGH;
#جيڪڏهن_پورو
    افتراضي:
      // We don't handle format 3 and 4 here.
      أرجع Err(SANITIZE_FAILURE);
    }
  }
};

}

#جيڪڏهن_پورو  // GRAPH_PAIRPOS_GRAPH_HH
