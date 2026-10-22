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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਗ੍ਰਾਫ_PAIRPOS_ਗ੍ਰਾਫ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗ੍ਰਾਫ_PAIRPOS_ਗ੍ਰਾਫ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "split-helpers.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "coverage-graph.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "classdef-graph.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../OT/Layout/GPOS/PairPos.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../OT/Layout/GPOS/PosLookupSubTable.hh"

ਨਾਮਾਕਾਸ਼ ਗ੍ਰਾਫ {

ਬਣਤਰ PairPosFormat1 : ਜਨਤਕ OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼) ਅਚਲ
  {
    size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ min_ਮਾਪ = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_ਮਾਪ;
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < min_ਮਾਪ)) ਵਾਪਸ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < min_ਮਾਪ + pair_ਸੈੱਟ.get_ਮਾਪ () - pair_ਸੈੱਟ.len.get_ਮਾਪ()))
      ਵਾਪਸ Err(SANITIZE_FAILURE);
    ਵਾਪਸ Ok();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>> ਵੰਡੋ_subtables (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                                         ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ)
  {
    hb_ਸੈੱਟ_t ਦੇਖਿਆ;

    TRY_ਸੌਂਪੋ (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_id, c.ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (this_ਸੂਚਕਾਂਕ, &coverage));
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_ਮਾਪ = c.ਗ੍ਰਾਫ.vertices_[coverage_id].ਸਾਰਣੀ_ਮਾਪ ();
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ_ਮਾਪ = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_ਮਾਪ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ partial_coverage_ਮਾਪ = 4;
    ਬਿਨਾਨਿਸ਼ਾਨੀ accumulated = ਅਧਾਰ_ਮਾਪ;
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਵੰਡੋ_points;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < pair_ਸੈੱਟ.len; i++)
    {
      TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ pair_ਸੈੱਟ_ਸੂਚਕਾਂਕ, pair_ਸੈੱਟ_ਗ੍ਰਾਫ_ਸੂਚਕਾਂਕ (c, this_ਸੂਚਕਾਂਕ, i));
      ਬਿਨਾਨਿਸ਼ਾਨੀ accumulated_delta =
          c.ਗ੍ਰਾਫ.ਲੱਭੋ_subgraph_ਮਾਪ (pair_ਸੈੱਟ_ਸੂਚਕਾਂਕ, ਦੇਖਿਆ).ਮੁੱਲ_or (0) +
          SmallTypes::ਮਾਪ; // for PairSet offset.
      partial_coverage_ਮਾਪ += OT::HBUINT16::static_ਮਾਪ;

      accumulated += accumulated_delta;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ = accumulated + hb_min (partial_coverage_ਮਾਪ, coverage_ਮਾਪ);

      ਜੇ (ਕੁੱਲ >= (1 << 16))
      {
        ਵੰਡੋ_points.push (i);
        TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਵੰਡੋ_points, ਵੰਡ_FAILURE));
        accumulated = ਅਧਾਰ_ਮਾਪ + accumulated_delta;
        partial_coverage_ਮਾਪ = 6;
        ਦੇਖਿਆ.clear (); // node sharing isn't allowed between splits.
      }
    }

    ਵੰਡੋ_context_t ਵੰਡੋ_context {
      c,
      ਇਹ,
      this_ਸੂਚਕਾਂਕ,
    };

    ਵਾਪਸ actuate_subtable_ਵੰਡੋ<ਵੰਡੋ_context_t> (ਵੰਡੋ_context, ਵੰਡੋ_points);
  }

 ਨਿਜੀ:

  ਬਣਤਰ ਵੰਡੋ_context_t {
    gsubgpos_ਗ੍ਰਾਫ_context_t& c;
    PairPosFormat1* thiz;
    ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ original_ਗਿਣਤੀ_2 ()
    {
      ਵਾਪਸ thiz->pair_ਸੈੱਟ.len;
    }

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> clone_range (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ)
    {
      ਵਾਪਸ thiz->clone_range (ਇਹ->c, ਇਹ->this_ਸੂਚਕਾਂਕ, ਸ਼ੁਰੂ, ਸਿਰਾ);
    }

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> shrink (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2)
    {
      ਵਾਪਸ thiz->shrink (ਇਹ->c, ਇਹ->this_ਸੂਚਕਾਂਕ, ਗਿਣਤੀ_2);
    }
  };

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> shrink (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2)
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_ਸੂਚਕਾਂਕ,
               ਗਿਣਤੀ_2);
    ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਗਿਣਤੀ = pair_ਸੈੱਟ.len;
    ਜੇ (ਗਿਣਤੀ_2 >= old_ਗਿਣਤੀ)
      ਵਾਪਸ Ok();

    pair_ਸੈੱਟ.len = ਗਿਣਤੀ_2;
    c.ਗ੍ਰਾਫ.vertices_[this_ਸੂਚਕਾਂਕ].shrink_ਬਫਰ ((old_ਗਿਣਤੀ - ਗਿਣਤੀ_2) * SmallTypes::ਮਾਪ);

    TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ coverage, c.ਗ੍ਰਾਫ.as_mutable_ਸਾਰਣੀ<Coverage> (this_ਸੂਚਕਾਂਕ, &ਇਹ->coverage));

    ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_ਮਾਪ = coverage.ਸ਼ੀਰਸ਼->ਸਾਰਣੀ_ਮਾਪ ();
    ਸਵੈਚਾਲਿਤ new_coverage =
        + hb_zip (coverage.ਸਾਰਣੀ->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> p) {
          ਵਾਪਸ p.second < ਗਿਣਤੀ_2;
        })
        | hb_map_retains_ਛਾਂਟੀ (hb_ਪਹਿਲਾ)
        ;

    ਵਾਪਸ Coverage::make_coverage (c, new_coverage, coverage.ਸੂਚਕਾਂਕ_2, coverage_ਮਾਪ);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> clone_range (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                        ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ,
                                        ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ) ਅਚਲ
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_ਸੂਚਕਾਂਕ, ਸ਼ੁਰੂ, ਸਿਰਾ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_pair_sets = ਸਿਰਾ - ਸ਼ੁਰੂ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਭਾਜ_ਮਾਪ = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_ਮਾਪ
                          + ਅੰਕ_pair_sets * SmallTypes::ਮਾਪ;

    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ pair_pos_ਅਭਾਜ_id, c.create_ਗੰਢ (ਅਭਾਜ_ਮਾਪ));

    PairPosFormat1* pair_pos_ਅਭਾਜ = (PairPosFormat1*) c.ਗ੍ਰਾਫ.object (pair_pos_ਅਭਾਜ_id).ਸਿਰ_2;
    pair_pos_ਅਭਾਜ->format = ਇਹ->format;
    pair_pos_ਅਭਾਜ->ਮੁੱਲ_Format_2[0] = ਇਹ->ਮੁੱਲ_Format_2[0];
    pair_pos_ਅਭਾਜ->ਮੁੱਲ_Format_2[1] = ਇਹ->ਮੁੱਲ_Format_2[1];
    pair_pos_ਅਭਾਜ->pair_ਸੈੱਟ.len = ਅੰਕ_pair_sets;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
    {
      TRY (c.ਗ੍ਰਾਫ.move_ਬੱਚਾ<> (this_ਸੂਚਕਾਂਕ,
                                 &pair_ਸੈੱਟ[i],
                                 pair_pos_ਅਭਾਜ_id,
                                 &pair_pos_ਅਭਾਜ->pair_ਸੈੱਟ[i - ਸ਼ੁਰੂ]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.ਗ੍ਰਾਫ.vertices_[this_ਸੂਚਕਾਂਕ].ਛਾਂਟੋ_real_links ();

    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_id, c.ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (this_ਸੂਚਕਾਂਕ, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_ਅਭਾਜ_id,
                                   2,
                                   ਸ਼ੁਰੂ, ਸਿਰਾ));

    ਵਾਪਸ Ok(pair_pos_ਅਭਾਜ_id);
  }



  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> pair_ਸੈੱਟ_ਗ੍ਰਾਫ_ਸੂਚਕਾਂਕ (gsubgpos_ਗ੍ਰਾਫ_context_t& c, ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ
  {
    ਵਾਪਸ c.ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (this_ਸੂਚਕਾਂਕ, &pair_ਸੈੱਟ[i]);
  }
};

ਬਣਤਰ PairPosFormat2 : ਜਨਤਕ OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼) ਅਚਲ
  {
    size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ min_ਮਾਪ = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_ਮਾਪ;
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < min_ਮਾਪ)) ਵਾਪਸ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ class1_ਗਿਣਤੀ = class1_ਗਿਣਤੀ_2;
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < min_ਮਾਪ + class1_ਗਿਣਤੀ * get_class1_ਰਿਕਾਰਡ_ਮਾਪ ()))
      ਵਾਪਸ Err(SANITIZE_FAILURE);
    ਵਾਪਸ Ok();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>> ਵੰਡੋ_subtables (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                                         ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ)
  {
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ_ਮਾਪ = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_ਮਾਪ;
    TRY_ਸੌਂਪੋ (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ class_def_2_ਮਾਪ, ਮਾਪ_ਦਾ (c, this_ਸੂਚਕਾਂਕ, &classDef2));
    TRY_ਸੌਂਪੋ (ਅਚਲ Coverage* coverage, get_coverage (c, this_ਸੂਚਕਾਂਕ));
    TRY_ਸੌਂਪੋ (ਅਚਲ ClassDef* class_def_1, get_class_def_1 (c, this_ਸੂਚਕਾਂਕ));
    ਸਵੈਚਾਲਿਤ gid_and_class =
        + coverage->iter ()
        | hb_map_retains_ਛਾਂਟੀ ([&] (hb_codepoint_t gid) {
          ਵਾਪਸ hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_ਸੌਂਪੋ (class_def_ਮਾਪ_estimator_t estimator, class_def_ਮਾਪ_estimator_t::create (gid_and_class));

    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ class1_ਗਿਣਤੀ = class1_ਗਿਣਤੀ_2;
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ class2_ਗਿਣਤੀ = class2_ਗਿਣਤੀ_2;
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ class1_ਰਿਕਾਰਡ_ਮਾਪ = get_class1_ਰਿਕਾਰਡ_ਮਾਪ ();

    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੁੱਲ_1_len = ਮੁੱਲ_Format1.get_len ();
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੁੱਲ_2_len = ਮੁੱਲ_Format2.get_len ();
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ਮੁੱਲ_len = ਮੁੱਲ_1_len + ਮੁੱਲ_2_len;

    ਬਿਨਾਨਿਸ਼ਾਨੀ accumulated = ਅਧਾਰ_ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_ਮਾਪ = 4;
    ਬਿਨਾਨਿਸ਼ਾਨੀ class_def_1_ਮਾਪ = 4;
    ਬਿਨਾਨਿਸ਼ਾਨੀ max_coverage_ਮਾਪ = coverage_ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ max_class_def_1_ਮਾਪ = class_def_1_ਮਾਪ;

    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਵੰਡੋ_points;

    TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ device_tables, get_all_device_tables (c, this_ਸੂਚਕਾਂਕ));
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> format1_device_ਸਾਰਣੀ_indices = ਮੁੱਲ_Format1.get_device_ਸਾਰਣੀ_indices ();
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> format2_device_ਸਾਰਣੀ_indices = ਮੁੱਲ_Format2.get_device_ਸਾਰਣੀ_indices ();
    ਤਾਰਕਿਕ has_device_tables = ਤਾਰਕਿਕ(format1_device_ਸਾਰਣੀ_indices) || ਤਾਰਕਿਕ(format2_device_ਸਾਰਣੀ_indices);

    hb_ਸੈੱਟ_t ਦੇਖਿਆ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < class1_ਗਿਣਤੀ; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ accumulated_delta = class1_ਰਿਕਾਰਡ_ਮਾਪ;
      class_def_1_ਮਾਪ = estimator.ਜੋੜੋ_class_def_ਮਾਪ (i);
      coverage_ਮਾਪ = estimator.coverage_ਮਾਪ ();
      max_coverage_ਮਾਪ = hb_max (max_coverage_ਮਾਪ, coverage_ਮਾਪ);
      max_class_def_1_ਮਾਪ = hb_max (max_class_def_1_ਮਾਪ, class_def_1_ਮਾਪ);

      ਜੇ (has_device_tables) {
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < class2_ਗਿਣਤੀ; j++)
        {
          ਬਿਨਾਨਿਸ਼ਾਨੀ value1_ਸੂਚਕਾਂਕ = ਕੁੱਲ_ਮੁੱਲ_len * (class2_ਗਿਣਤੀ * i + j);
          ਬਿਨਾਨਿਸ਼ਾਨੀ value2_ਸੂਚਕਾਂਕ = value1_ਸੂਚਕਾਂਕ + ਮੁੱਲ_1_len;
          accumulated_delta += ਮਾਪ_ਦਾ_ਮੁੱਲ_ਰਿਕਾਰਡ_children (c,
                                                        device_tables,
                                                        format1_device_ਸਾਰਣੀ_indices,
                                                        value1_ਸੂਚਕਾਂਕ,
                                                        ਦੇਖਿਆ);
          accumulated_delta += ਮਾਪ_ਦਾ_ਮੁੱਲ_ਰਿਕਾਰਡ_children (c,
                                                        device_tables,
                                                        format2_device_ਸਾਰਣੀ_indices,
                                                        value2_ਸੂਚਕਾਂਕ,
                                                        ਦੇਖਿਆ);
        }
      }

      accumulated += accumulated_delta;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ = accumulated
                       + coverage_ਮਾਪ + class_def_1_ਮਾਪ + class_def_2_ਮਾਪ
                       // The largest object will pack last and can exceed the size limit.
                       - hb_max (hb_max (coverage_ਮਾਪ, class_def_1_ਮਾਪ), class_def_2_ਮਾਪ);
      ਜੇ (ਕੁੱਲ >= (1 << 16))
      {
        ਵੰਡੋ_points.push (i);
        TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਵੰਡੋ_points, ਵੰਡ_FAILURE));
        // split does not include i, so add the size for i when we reset the size counters.
        accumulated = ਅਧਾਰ_ਮਾਪ + accumulated_delta;

        estimator.reset();
        class_def_1_ਮਾਪ = estimator.ਜੋੜੋ_class_def_ਮਾਪ(i);
        coverage_ਮਾਪ = estimator.coverage_ਮਾਪ();
        ਦੇਖਿਆ.clear (); // node sharing isn't allowed between splits.
      }
    }

    ਵੰਡੋ_context_t ਵੰਡੋ_context {
      c,
      ਇਹ,
      this_ਸੂਚਕਾਂਕ,
      class1_ਰਿਕਾਰਡ_ਮਾਪ,
      ਕੁੱਲ_ਮੁੱਲ_len,
      ਮੁੱਲ_1_len,
      ਮੁੱਲ_2_len,
      max_coverage_ਮਾਪ,
      max_class_def_1_ਮਾਪ,
      device_tables,
      format1_device_ਸਾਰਣੀ_indices,
      format2_device_ਸਾਰਣੀ_indices
    };

    ਵਾਪਸ actuate_subtable_ਵੰਡੋ<ਵੰਡੋ_context_t> (ਵੰਡੋ_context, ਵੰਡੋ_points);
  }
 ਨਿਜੀ:

  ਬਣਤਰ ਵੰਡੋ_context_t
  {
    gsubgpos_ਗ੍ਰਾਫ_context_t& c;
    PairPosFormat2* thiz;
    ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ class1_ਰਿਕਾਰਡ_ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੁੱਲ_ਰਿਕਾਰਡ_len;
    ਬਿਨਾਨਿਸ਼ਾਨੀ value1_ਰਿਕਾਰਡ_len;
    ਬਿਨਾਨਿਸ਼ਾਨੀ value2_ਰਿਕਾਰਡ_len;
    ਬਿਨਾਨਿਸ਼ਾਨੀ max_coverage_ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ max_class_def_ਮਾਪ;

    ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ>& device_tables;
    ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>& format1_device_ਸਾਰਣੀ_indices;
    ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>& format2_device_ਸਾਰਣੀ_indices;

    ਬਿਨਾਨਿਸ਼ਾਨੀ original_ਗਿਣਤੀ_2 ()
    {
      ਵਾਪਸ thiz->class1_ਗਿਣਤੀ_2;
    }

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> clone_range (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ)
    {
      ਵਾਪਸ thiz->clone_range (*ਇਹ, ਸ਼ੁਰੂ, ਸਿਰਾ);
    }

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> shrink (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2)
    {
      ਵਾਪਸ thiz->shrink (*ਇਹ, ਗਿਣਤੀ_2);
    }
  };

  size_t get_class1_ਰਿਕਾਰਡ_ਮਾਪ () ਅਚਲ
  {
    ਅਚਲ size_t class2_ਗਿਣਤੀ = class2_ਗਿਣਤੀ_2;
    ਵਾਪਸ
        class2_ਗਿਣਤੀ * (ਮੁੱਲ_Format1.get_ਮਾਪ () + ਮੁੱਲ_Format2.get_ਮਾਪ ());
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> clone_range (ਵੰਡੋ_context_t& ਵੰਡੋ_context,
                                        ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ) ਅਚਲ
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", ਵੰਡੋ_context.this_ਸੂਚਕਾਂਕ, ਸ਼ੁਰੂ, ਸਿਰਾ);

    ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ = ਵੰਡੋ_context.c.ਗ੍ਰਾਫ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਰਿਕਾਰਡਾਂ = ਸਿਰਾ - ਸ਼ੁਰੂ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਭਾਜ_ਮਾਪ = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_ਮਾਪ
                          + ਅੰਕ_ਰਿਕਾਰਡਾਂ * ਵੰਡੋ_context.class1_ਰਿਕਾਰਡ_ਮਾਪ;

    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ pair_pos_ਅਭਾਜ_id, ਵੰਡੋ_context.c.create_ਗੰਢ (ਅਭਾਜ_ਮਾਪ));

    PairPosFormat2* pair_pos_ਅਭਾਜ =
        (PairPosFormat2*) ਗ੍ਰਾਫ.object (pair_pos_ਅਭਾਜ_id).ਸਿਰ_2;
    pair_pos_ਅਭਾਜ->format = ਇਹ->format;
    pair_pos_ਅਭਾਜ->ਮੁੱਲ_Format1 = ਇਹ->ਮੁੱਲ_Format1;
    pair_pos_ਅਭਾਜ->ਮੁੱਲ_Format2 = ਇਹ->ਮੁੱਲ_Format2;
    pair_pos_ਅਭਾਜ->class1_ਗਿਣਤੀ_2 = ਅੰਕ_ਰਿਕਾਰਡਾਂ;
    pair_pos_ਅਭਾਜ->class2_ਗਿਣਤੀ_2 = ਇਹ->class2_ਗਿਣਤੀ_2;
    TRY (clone_class1_ਰਿਕਾਰਡਾਂ (ਵੰਡੋ_context,
                               pair_pos_ਅਭਾਜ_id,
                               ਸ਼ੁਰੂ,
                               ਸਿਰਾ));

    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_id,
         ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (ਵੰਡੋ_context.this_ਸੂਚਕਾਂਕ, &coverage));
    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ class_def_1_id,
         ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (ਵੰਡੋ_context.this_ਸੂਚਕਾਂਕ, &classDef1));

    ਸਵੈਚਾਲਿਤ& coverage_v = ਗ੍ਰਾਫ.vertices_[coverage_id];
    ਸਵੈਚਾਲਿਤ& class_def_1_v = ਗ੍ਰਾਫ.vertices_[class_def_1_id];
    ਅਚਲ Coverage* coverage_ਸਾਰਣੀ = (ਅਚਲ Coverage*) coverage_v.obj().ਸਿਰ_2;
    ਅਚਲ ClassDef* class_def_1_ਸਾਰਣੀ = (ਅਚਲ ClassDef*) class_def_1_v.obj().ਸਿਰ_2;
    ਜੇ (unlikely (!coverage_ਸਾਰਣੀ || !class_def_1_ਸਾਰਣੀ))
      ਵਾਪਸ Err(INVALID_ਦਲੀਲ);
    TRY (coverage_ਸਾਰਣੀ->sanitize (coverage_v));
    TRY (class_def_1_ਸਾਰਣੀ->sanitize (class_def_1_v));

    ਸਵੈਚਾਲਿਤ klass_map =
    + coverage_ਸਾਰਣੀ->iter ()
    | hb_map_retains_ਛਾਂਟੀ ([&] (hb_codepoint_t gid) {
      ਵਾਪਸ hb_codepoint_pair_t (gid, class_def_1_ਸਾਰਣੀ->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      ਵਾਪਸ klass >= ਸ਼ੁਰੂ && klass < ਸਿਰਾ;
    }, hb_ਦੂਜਾ)
    | hb_map_retains_ਛਾਂਟੀ ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      ਵਾਪਸ hb_codepoint_pair_t (gid_and_class.ਪਹਿਲਾ, gid_and_class.second - ਸ਼ੁਰੂ);
    })
    ;

    TRY (Coverage::ਜੋੜੋ_coverage (ਵੰਡੋ_context.c,
                                 pair_pos_ਅਭਾਜ_id,
                                 2,
                                 + klass_map | hb_map_retains_ਛਾਂਟੀ (hb_ਪਹਿਲਾ),
                                 ਵੰਡੋ_context.max_coverage_ਮਾਪ));

    // classDef1
    TRY (ClassDef::ਜੋੜੋ_class_def (ਵੰਡੋ_context.c,
                                  pair_pos_ਅਭਾਜ_id,
                                  8,
                                  + klass_map,
                                  ਵੰਡੋ_context.max_class_def_ਮਾਪ));

    // classDef2
    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ class_def_2_id,
         ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (ਵੰਡੋ_context.this_ਸੂਚਕਾਂਕ, &classDef2));

    TRY(ਗ੍ਰਾਫ.vertices_[pair_pos_ਅਭਾਜ_id].ਜੋੜੋ_real_link (SmallTypes::ਮਾਪ, class_def_2_id, 10));
    TRY(ਗ੍ਰਾਫ.vertices_[class_def_2_id].ਜੋੜੋ_ਮਾਪਾ (pair_pos_ਅਭਾਜ_id, ਝੂਠ));

    ਵਾਪਸ Ok(pair_pos_ਅਭਾਜ_id);
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> clone_class1_ਰਿਕਾਰਡਾਂ (ਵੰਡੋ_context_t& ਵੰਡੋ_context,
                                             ਬਿਨਾਨਿਸ਼ਾਨੀ pair_pos_ਅਭਾਜ_id,
                                             ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ) ਅਚਲ
  {
    PairPosFormat2* pair_pos_ਅਭਾਜ =
        (PairPosFormat2*) ਵੰਡੋ_context.c.ਗ੍ਰਾਫ.object (pair_pos_ਅਭਾਜ_id).ਸਿਰ_2;

    ਅੱਖਰ* ਸ਼ੁਰੂ_addr = ((ਅੱਖਰ*)&values[0]) + ਸ਼ੁਰੂ * ਵੰਡੋ_context.class1_ਰਿਕਾਰਡ_ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਰਿਕਾਰਡਾਂ = ਸਿਰਾ - ਸ਼ੁਰੂ;
    hb_memcpy (&pair_pos_ਅਭਾਜ->values[0],
            ਸ਼ੁਰੂ_addr,
            ਅੰਕ_ਰਿਕਾਰਡਾਂ * ਵੰਡੋ_context.class1_ਰਿਕਾਰਡ_ਮਾਪ);

    ਜੇ (!ਵੰਡੋ_context.format1_device_ਸਾਰਣੀ_indices
        && !ਵੰਡੋ_context.format2_device_ਸਾਰਣੀ_indices)
      // No device tables to move over.
      ਵਾਪਸ Ok();

    ਬਿਨਾਨਿਸ਼ਾਨੀ class2_ਗਿਣਤੀ = class2_ਗਿਣਤੀ_2;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < class2_ਗਿਣਤੀ; j++)
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ value1_ਸੂਚਕਾਂਕ = ਵੰਡੋ_context.ਮੁੱਲ_ਰਿਕਾਰਡ_len * (class2_ਗਿਣਤੀ * i + j);
        ਬਿਨਾਨਿਸ਼ਾਨੀ value2_ਸੂਚਕਾਂਕ = value1_ਸੂਚਕਾਂਕ + ਵੰਡੋ_context.value1_ਰਿਕਾਰਡ_len;

        ਬਿਨਾਨਿਸ਼ਾਨੀ new_value1_ਸੂਚਕਾਂਕ = ਵੰਡੋ_context.ਮੁੱਲ_ਰਿਕਾਰਡ_len * (class2_ਗਿਣਤੀ * (i - ਸ਼ੁਰੂ) + j);
        ਬਿਨਾਨਿਸ਼ਾਨੀ new_value2_ਸੂਚਕਾਂਕ = new_value1_ਸੂਚਕਾਂਕ + ਵੰਡੋ_context.value1_ਰਿਕਾਰਡ_len;

        TRY (transfer_device_tables (ਵੰਡੋ_context,
                                     pair_pos_ਅਭਾਜ_id,
                                     ਵੰਡੋ_context.format1_device_ਸਾਰਣੀ_indices,
                                     value1_ਸੂਚਕਾਂਕ,
                                     new_value1_ਸੂਚਕਾਂਕ));

        TRY (transfer_device_tables (ਵੰਡੋ_context,
                                     pair_pos_ਅਭਾਜ_id,
                                     ਵੰਡੋ_context.format2_device_ਸਾਰਣੀ_indices,
                                     value2_ਸੂਚਕਾਂਕ,
                                     new_value2_ਸੂਚਕਾਂਕ));
      }
    }
    ਵਾਪਸ Ok();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> transfer_device_tables (ਵੰਡੋ_context_t& ਵੰਡੋ_context,
                                               ਬਿਨਾਨਿਸ਼ਾਨੀ pair_pos_ਅਭਾਜ_id,
                                               ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>& device_ਸਾਰਣੀ_indices,
                                               ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਮੁੱਲ_ਰਿਕਾਰਡ_ਸੂਚਕਾਂਕ,
                                               ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਮੁੱਲ_ਰਿਕਾਰਡ_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    PairPosFormat2* pair_pos_ਅਭਾਜ =
        (PairPosFormat2*) ਵੰਡੋ_context.c.ਗ੍ਰਾਫ.object (pair_pos_ਅਭਾਜ_id).ਸਿਰ_2;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i : device_ਸਾਰਣੀ_indices)
    {
      OT::Offset16* ਰਿਕਾਰਡ = (OT::Offset16*) &values[old_ਮੁੱਲ_ਰਿਕਾਰਡ_ਸੂਚਕਾਂਕ + i];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਰਿਕਾਰਡ_position = ((ਅੱਖਰ*) ਰਿਕਾਰਡ) - ((ਅੱਖਰ*) ਇਹ);
      ਜੇ (!ਵੰਡੋ_context.device_tables.has (ਰਿਕਾਰਡ_position)) ਜਾਰੀ;

      TRY (ਵੰਡੋ_context.c.ਗ੍ਰਾਫ.move_ਬੱਚਾ (
          ਵੰਡੋ_context.this_ਸੂਚਕਾਂਕ,
          ਰਿਕਾਰਡ,
          pair_pos_ਅਭਾਜ_id,
          (OT::Offset16*) &pair_pos_ਅਭਾਜ->values[new_ਮੁੱਲ_ਰਿਕਾਰਡ_ਸੂਚਕਾਂਕ + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    ਵੰਡੋ_context.c.ਗ੍ਰਾਫ.vertices_[ਵੰਡੋ_context.this_ਸੂਚਕਾਂਕ].ਛਾਂਟੋ_real_links ();

    ਵਾਪਸ Ok();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> shrink (ਵੰਡੋ_context_t& ਵੰਡੋ_context,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2)
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               ਵੰਡੋ_context.this_ਸੂਚਕਾਂਕ,
               ਗਿਣਤੀ_2);
    ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਗਿਣਤੀ = class1_ਗਿਣਤੀ_2;
    ਜੇ (ਗਿਣਤੀ_2 >= old_ਗਿਣਤੀ)
      ਵਾਪਸ Ok();

    ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ = ਵੰਡੋ_context.c.ਗ੍ਰਾਫ;
    class1_ਗਿਣਤੀ_2 = ਗਿਣਤੀ_2;
    ਗ੍ਰਾਫ.vertices_[ਵੰਡੋ_context.this_ਸੂਚਕਾਂਕ].shrink_ਬਫਰ(
        (old_ਗਿਣਤੀ - ਗਿਣਤੀ_2) * ਵੰਡੋ_context.class1_ਰਿਕਾਰਡ_ਮਾਪ);

    TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ coverage,
         ਗ੍ਰਾਫ.as_mutable_ਸਾਰਣੀ<Coverage> (ਵੰਡੋ_context.this_ਸੂਚਕਾਂਕ, &ਇਹ->coverage));

    TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ class_def_1,
         ਗ੍ਰਾਫ.as_mutable_ਸਾਰਣੀ<ClassDef> (ਵੰਡੋ_context.this_ਸੂਚਕਾਂਕ, &classDef1));

    ਸਵੈਚਾਲਿਤ klass_map =
    + coverage.ਸਾਰਣੀ->iter ()
    | hb_map_retains_ਛਾਂਟੀ ([&] (hb_codepoint_t gid) {
      ਵਾਪਸ hb_codepoint_pair_t (gid, class_def_1.ਸਾਰਣੀ->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      ਵਾਪਸ klass < ਗਿਣਤੀ_2;
    }, hb_ਦੂਜਾ)
    ;

    ਸਵੈਚਾਲਿਤ new_coverage = + klass_map | hb_map_retains_ਛਾਂਟੀ (hb_ਪਹਿਲਾ);
    TRY (Coverage::make_coverage (ਵੰਡੋ_context.c,
                                  + new_coverage,
                                  coverage.ਸੂਚਕਾਂਕ_2,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    ਵਾਪਸ ClassDef::make_class_def (ਵੰਡੋ_context.c,
                                     + klass_map,
                                     class_def_1.ਸੂਚਕਾਂਕ_2,
                                     class_def_1.ਸ਼ੀਰਸ਼->ਸਾਰਣੀ_ਮਾਪ ());
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ>>
  get_all_device_tables (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                         ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ& v = c.ਗ੍ਰਾਫ.vertices_[this_ਸੂਚਕਾਂਕ];
    ਵਾਪਸ v.position_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ_map ();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਅਚਲ Coverage*> get_coverage (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                                ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_id, c.ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (this_ਸੂਚਕਾਂਕ, &coverage));
    ਸਵੈਚਾਲਿਤ& coverage_v = c.ਗ੍ਰਾਫ.vertices_[coverage_id];

    ਅਚਲ Coverage* coverage_ਸਾਰਣੀ = (ਅਚਲ Coverage*) coverage_v.obj().ਸਿਰ_2;
    ਜੇ (unlikely (!coverage_ਸਾਰਣੀ)) ਵਾਪਸ Err(INVALID_ਦਲੀਲ);
    TRY(coverage_ਸਾਰਣੀ->sanitize (coverage_v));

    ਵਾਪਸ coverage_ਸਾਰਣੀ;
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਅਚਲ ClassDef*> get_class_def_1 (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                                   ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ class_def_1_id, c.ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (this_ਸੂਚਕਾਂਕ, &classDef1));
    ਸਵੈਚਾਲਿਤ& class_def_1_v = c.ਗ੍ਰਾਫ.vertices_[class_def_1_id];

    ਅਚਲ ClassDef* class_def_1_ਸਾਰਣੀ = (ਅਚਲ ClassDef*) class_def_1_v.obj().ਸਿਰ_2;
    ਜੇ (unlikely (!class_def_1_ਸਾਰਣੀ)) ਵਾਪਸ Err(INVALID_ਦਲੀਲ);
    TRY(class_def_1_ਸਾਰਣੀ->sanitize (class_def_1_v));
    ਵਾਪਸ class_def_1_ਸਾਰਣੀ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ_ਦਾ_ਮੁੱਲ_ਰਿਕਾਰਡ_children (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                          ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ>& device_tables,
                                          ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> device_ਸਾਰਣੀ_indices,
                                          ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੁੱਲ_ਰਿਕਾਰਡ_ਸੂਚਕਾਂਕ,
                                          hb_ਸੈੱਟ_t& ਦੇਖਿਆ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i : device_ਸਾਰਣੀ_indices)
    {
      OT::Layout::GPOS_impl::ਮੁੱਲ* ਰਿਕਾਰਡ = &values[ਮੁੱਲ_ਰਿਕਾਰਡ_ਸੂਚਕਾਂਕ + i];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਰਿਕਾਰਡ_position = ((ਅੱਖਰ*) ਰਿਕਾਰਡ) - ((ਅੱਖਰ*) ਇਹ);
      ਬਿਨਾਨਿਸ਼ਾਨੀ* obj_idx;
      ਜੇ (!device_tables.has (ਰਿਕਾਰਡ_position, &obj_idx)) ਜਾਰੀ;
      ਮਾਪ += c.ਗ੍ਰਾਫ.ਲੱਭੋ_subgraph_ਮਾਪ (*obj_idx, ਦੇਖਿਆ).ਮੁੱਲ_or (0);
    }
    ਵਾਪਸ ਮਾਪ;
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਮਾਪ_ਦਾ (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                    ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ,
                                    ਅਚਲ ਖਾਲੀ* ਵਿੱਥ) ਅਚਲ
  {
    TRY_ਸੌਂਪੋ (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ id, c.ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (this_ਸੂਚਕਾਂਕ, ਵਿੱਥ));
    ਵਾਪਸ Ok(c.ਗ੍ਰਾਫ.vertices_[id].ਸਾਰਣੀ_ਮਾਪ ());
  }
};

ਬਣਤਰ PairPos : ਜਨਤਕ OT::Layout::GPOS_impl::PairPos
{
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>> ਵੰਡੋ_subtables (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                                         ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ)
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1:
      hb_barrier ();
      ਵਾਪਸ ((PairPosFormat1*)(&u.format1))->ਵੰਡੋ_subtables (c, this_ਸੂਚਕਾਂਕ);
    ਹਾਲਤ 2:
      hb_barrier ();
      ਵਾਪਸ ((PairPosFormat2*)(&u.format2))->ਵੰਡੋ_subtables (c, this_ਸੂਚਕਾਂਕ);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: HB_FALLTHROUGH;
    ਹਾਲਤ 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#ਜੇ_ਅੰਤ
    ਮੂਲ:
      ਵਾਪਸ Ok(hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ());
    }
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼) ਅਚਲ
  {
    size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ();
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < u.format.v.get_ਮਾਪ ())) ਵਾਪਸ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1:
      hb_barrier ();
      ਵਾਪਸ ((PairPosFormat1*)(&u.format1))->sanitize (ਸ਼ੀਰਸ਼);
    ਹਾਲਤ 2:
      hb_barrier ();
      ਵਾਪਸ ((PairPosFormat2*)(&u.format2))->sanitize (ਸ਼ੀਰਸ਼);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: HB_FALLTHROUGH;
    ਹਾਲਤ 4: HB_FALLTHROUGH;
#ਜੇ_ਅੰਤ
    ਮੂਲ:
      // We don't handle format 3 and 4 here.
      ਵਾਪਸ Err(SANITIZE_FAILURE);
    }
  }
};

}

#ਜੇ_ਅੰਤ  // GRAPH_PAIRPOS_GRAPH_HH
