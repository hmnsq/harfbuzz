
/*
 * Copyright © 2025  Google, Inc.
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਗ੍ਰਾਫ_LIGATURE_ਗ੍ਰਾਫ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗ੍ਰਾਫ_LIGATURE_ਗ੍ਰਾਫ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "graph.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../OT/Layout/GSUB/LigatureSubst.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../OT/Layout/GSUB/LigatureSubstFormat1.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../OT/Layout/GSUB/LigatureSet.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../hb-ot-dual.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ <algorithm>
#ਅਨ੍ਤਰ੍ਭਾਵ <utility>

ਨਾਮਾਕਾਸ਼ ਗ੍ਰਾਫ {

ਬਣਤਰ Ligature_ਸੈੱਟ : ਜਨਤਕ OT::Layout::GSUB_impl::Ligature_ਸੈੱਟ<SmallTypes>
{
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼) ਅਚਲ
  {
    size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ ();
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < OT::Layout::GSUB_impl::Ligature_ਸੈੱਟ<SmallTypes>::min_ਮਾਪ)) ਵਾਪਸ Err(SANITIZE_FAILURE);
    hb_barrier ();

    size_t ਕੁੱਲ_len = ligature.get_ਮਾਪ() + OT::Layout::GSUB_impl::Ligature_ਸੈੱਟ<SmallTypes>::min_ਮਾਪ - ligature.len.get_ਮਾਪ();
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < ਕੁੱਲ_len)) {
      ਵਾਪਸ Err(SANITIZE_FAILURE);
    }
    ਵਾਪਸ Ok();
  }
};

ਬਣਤਰ LigatureSubstFormat1 : ਜਨਤਕ OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>
{
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼) ਅਚਲ
  {
    size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ min_ਮਾਪ = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_ਮਾਪ;
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < min_ਮਾਪ)) ਵਾਪਸ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < min_ਮਾਪ + ligature_ਸੈੱਟ.get_ਮਾਪ() - ligature_ਸੈੱਟ.len.get_ਮਾਪ()))
      ਵਾਪਸ Err(SANITIZE_FAILURE);
    ਵਾਪਸ Ok();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>> ਵੰਡੋ_subtables (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                                         ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ)
  {
    TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ ਵੰਡੋ_points, compute_ਵੰਡੋ_points(c, this_ਸੂਚਕਾਂਕ));
    ਜੇ (!ਵੰਡੋ_points)
      ਵਾਪਸ Ok(hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ());

    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ligas, ਕੁੱਲ_ਸੰਖਿਆ_ligas(c, this_ਸੂਚਕਾਂਕ));
    TRY_ਸੌਂਪੋ (hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> counts, liga_counts(c, this_ਸੂਚਕਾਂਕ));
    ਵੰਡੋ_context_t ਵੰਡੋ_context {
      c,
      ਇਹ,
      this_ਸੂਚਕਾਂਕ,
      ਕੁੱਲ_ligas,
      std::move (counts),
    };
    ਵਾਪਸ actuate_subtable_ਵੰਡੋ<ਵੰਡੋ_context_t> (ਵੰਡੋ_context, ਵੰਡੋ_points);
  }

 ਨਿਜੀ:
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਕੁੱਲ_ਸੰਖਿਆ_ligas(gsubgpos_ਗ੍ਰਾਫ_context_t& c, ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ) ਅਚਲ {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ligature_ਸੈੱਟ.len; i++)
    {
      TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ liga_ਸੈੱਟ, c.ਗ੍ਰਾਫ.as_ਸਾਰਣੀ<Ligature_ਸੈੱਟ>(this_ਸੂਚਕਾਂਕ, &ligature_ਸੈੱਟ[i]));
      ਕੁੱਲ += liga_ਸੈੱਟ.ਸਾਰਣੀ->ligature.len;
    }
    ਵਾਪਸ Ok(ਕੁੱਲ);
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>> liga_counts(gsubgpos_ਗ੍ਰਾਫ_context_t& c, ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ) ਅਚਲ {
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਨਤੀਜਾ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ligature_ਸੈੱਟ.len; i++)
    {
      TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ liga_ਸੈੱਟ, c.ਗ੍ਰਾਫ.as_ਸਾਰਣੀ<Ligature_ਸੈੱਟ>(this_ਸੂਚਕਾਂਕ, &ligature_ਸੈੱਟ[i]));
      ਨਤੀਜਾ.push(!liga_ਸੈੱਟ.ਸਾਰਣੀ ? 0 : liga_ਸੈੱਟ.ਸਾਰਣੀ->ligature.len);
    }
    ਵਾਪਸ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>>::from(std::move(ਨਤੀਜਾ), ਵੰਡ_FAILURE);
  }

  ਸਾਂਚਾ <ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_mutability_t mutability>
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>> ligature_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_object_id(ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_and_ਸਾਰਣੀ_t<Ligature_ਸੈੱਟ, mutability>& liga_ਸੈੱਟ) ਅਚਲ {
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> map;
    ਜੇ (!map.resize_exact(liga_ਸੈੱਟ.ਸਾਰਣੀ->ligature.len)) ਵਾਪਸ Err(ਵੰਡ_FAILURE);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < map.ਲੰਮਾਈ; i++) {
      map[i] = HB_ਗ੍ਰਾਫ_INVALID;
    }

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& l : liga_ਸੈੱਟ.ਸ਼ੀਰਸ਼->obj ().real_links) {
      ਜੇ (l.position < 2) ਜਾਰੀ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ array_ਸੂਚਕਾਂਕ = (l.position - 2) / 2;
      map[array_ਸੂਚਕਾਂਕ] = l.objidx;
    }
    ਵਾਪਸ map;
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>> compute_ਵੰਡੋ_points(gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                                             ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    // For ligature subst coverage is always packed last, and as a result is where an overflow
    // will happen if there is one, so we can check the estimate length of the
    // LigatureSubstFormat1 -> Coverage offset length which is the sum of all data in the
    // retained sub graph except for the coverage table itself.
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ_ਮਾਪ = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ accumulated = ਅਧਾਰ_ਮਾਪ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ligature_ਸੂਚਕਾਂਕ = 0;
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਵੰਡੋ_points;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ligature_ਸੈੱਟ.len; i++)
    {
      accumulated += OT::HBUINT16::static_ਮਾਪ; // for ligature set offset
      accumulated += OT::Layout::GSUB_impl::Ligature_ਸੈੱਟ<SmallTypes>::min_ਮਾਪ; // for ligature set table

      TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ liga_ਸੈੱਟ, c.ਗ੍ਰਾਫ.as_ਸਾਰਣੀ<Ligature_ਸੈੱਟ>(this_ਸੂਚਕਾਂਕ, &ligature_ਸੈੱਟ[i]));

      // Finding the object id associated with an array index is O(n)
      // so to avoid O(n^2), precompute the mapping by scanning through
      // all links
      TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_id, ligature_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_object_id (liga_ਸੈੱਟ));

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < liga_ਸੈੱਟ.ਸਾਰਣੀ->ligature.len; j++)
      {
        ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ liga_id = ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_id[j];
        ਜੇ (liga_id == HB_ਗ੍ਰਾਫ_INVALID) ਜਾਰੀ; // no outgoing link, ignore
        ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਮਾਪ = c.ਗ੍ਰਾਫ.vertices_[liga_id].ਸਾਰਣੀ_ਮਾਪ ();

        accumulated += OT::HBUINT16::static_ਮਾਪ; // for ligature offset
        accumulated += liga_ਮਾਪ; // for the ligature table

        ਜੇ (accumulated >= (1 << 16))
        {
          ਵੰਡੋ_points.push(ligature_ਸੂਚਕਾਂਕ);
          // We're going to split such that the current ligature will be in the new sub table.
          // That means we'll have one ligature subst (base_base), one ligature set, and one liga table
          accumulated = ਅਧਾਰ_ਮਾਪ + // for liga subst subtable
            (OT::HBUINT16::static_ਮਾਪ * 2) + // for liga set and liga offset
            OT::Layout::GSUB_impl::Ligature_ਸੈੱਟ<SmallTypes>::min_ਮਾਪ + // for liga set subtable
            liga_ਮਾਪ; // for liga sub table
        }

        ligature_ਸੂਚਕਾਂਕ++;
      }
    }

    ਵਾਪਸ ਵੰਡੋ_points;
  }

  ਬਣਤਰ ਵੰਡੋ_context_t
  {
    gsubgpos_ਗ੍ਰਾਫ_context_t& c;
    LigatureSubstFormat1* thiz;
    ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ original_ਗਿਣਤੀ;
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> liga_counts;

    ਬਿਨਾਨਿਸ਼ਾਨੀ original_ਗਿਣਤੀ_2 ()
    {
      ਵਾਪਸ original_ਗਿਣਤੀ;
    }

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> clone_range (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ)
    {
      ਵਾਪਸ thiz->clone_range (c, this_ਸੂਚਕਾਂਕ, liga_counts, ਸ਼ੁਰੂ, ਸਿਰਾ);
    }

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> shrink (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2)
    {
      ਵਾਪਸ thiz->shrink (c, this_ਸੂਚਕਾਂਕ, original_ਗਿਣਤੀ_2(), liga_counts, ਗਿਣਤੀ_2);
    }
  };

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, Ligature_ਸੈੱਟ*>> new_liga_ਸੈੱਟ(gsubgpos_ਗ੍ਰਾਫ_context_t& c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2) ਅਚਲ {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਭਾਜ_ਮਾਪ = OT::Layout::GSUB_impl::Ligature_ਸੈੱਟ<SmallTypes>::min_ਮਾਪ
                          + ਗਿਣਤੀ_2 * SmallTypes::ਮਾਪ;

    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਭਾਜ_id, c.create_ਗੰਢ (ਅਭਾਜ_ਮਾਪ));

    Ligature_ਸੈੱਟ* ਅਭਾਜ = (Ligature_ਸੈੱਟ*) c.ਗ੍ਰਾਫ.object (ਅਭਾਜ_id).ਸਿਰ_2;
    ਅਭਾਜ->ligature.len = ਗਿਣਤੀ_2;
    ਵਾਪਸ Ok(hb_pair(ਅਭਾਜ_id, ਅਭਾਜ));
  }

  ਖਾਲੀ clear_virtual_links (gsubgpos_ਗ੍ਰਾਫ_context_t& c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਗੰਢ_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਸਵੈਚਾਲਿਤ& v  = c.ਗ੍ਰਾਫ.vertices_[ਗੰਢ_ਸੂਚਕਾਂਕ];
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& l : v.obj ().virtual_links)
    {
      ਸਵੈਚਾਲਿਤ& ਬੱਚਾ = c.ਗ੍ਰਾਫ.vertices_[l.objidx];
      ਬੱਚਾ.remove_ਮਾਪਾ(ਗੰਢ_ਸੂਚਕਾਂਕ);
    }
    v.clear_virtual_links ();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਜੋੜੋ_virtual_link(gsubgpos_ਗ੍ਰਾਫ_context_t& c, ਬਿਨਾਨਿਸ਼ਾਨੀ from, ਬਿਨਾਨਿਸ਼ਾਨੀ to) ਅਚਲ {
    ਜੇ (unlikely (from >= c.ਗ੍ਰਾਫ.vertices_.ਲੰਮਾਈ || to >= c.ਗ੍ਰਾਫ.vertices_.ਲੰਮਾਈ)) ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);
    TRY(c.ਗ੍ਰਾਫ.vertices_[to].ਜੋੜੋ_ਮਾਪਾ (from, ਸੱਚ));
    TRY(c.ਗ੍ਰਾਫ.vertices_[from].ਜੋੜੋ_virtual_link (to));
    ਵਾਪਸ Ok();
  }

  hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> current_liga_ਸੈੱਟ_bounds (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                                         ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਸੈੱਟ_ਸੂਚਕਾਂਕ,
                                                         ਅਚਲ hb_serialize_context_t::object_t& liga_ਸੈੱਟ) ਅਚਲ
  {
    // Finds the actual liga indices present in the liga set currently. Takes
    // into account those that have been removed by processing.
    ਬਿਨਾਨਿਸ਼ਾਨੀ min_ਸੂਚਕਾਂਕ = HB_ਗ੍ਰਾਫ_INVALID;
    ਬਿਨਾਨਿਸ਼ਾਨੀ max_ਸੂਚਕਾਂਕ = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& l : liga_ਸੈੱਟ.real_links) {
      ਜੇ (l.position < 2) ਜਾਰੀ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਸੂਚਕਾਂਕ = (l.position - 2) / 2;
      min_ਸੂਚਕਾਂਕ = hb_min(min_ਸੂਚਕਾਂਕ, liga_ਸੂਚਕਾਂਕ);
      max_ਸੂਚਕਾਂਕ = hb_max(max_ਸੂਚਕਾਂਕ, liga_ਸੂਚਕਾਂਕ);
    }
    ਵਾਪਸ hb_pair(min_ਸੂਚਕਾਂਕ, max_ਸੂਚਕਾਂਕ + 1);
  }

  ਖਾਲੀ compact_liga_ਸੈੱਟ (gsubgpos_ਗ੍ਰਾਫ_context_t& c, Ligature_ਸੈੱਟ* ਸਾਰਣੀ, ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t* v) ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ& obj = v->obj ();
    ਜੇ (ਸਾਰਣੀ->ligature.len <= obj.real_links.ਲੰਮਾਈ) ਵਾਪਸ;

    // compact the remaining linked liga offsets into a continous array and shrink the node as needed.
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੰਜ਼ਿਲ_remove = ਸਾਰਣੀ->ligature.len - obj.real_links.ਲੰਮਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_position = SmallTypes::ਮਾਪ;
    v->ਛਾਂਟੋ_real_links (); // for this to work we need to process links in order of position.
    ਲਈ (ਸਵੈਚਾਲਿਤ& l : v->real_links_writer ())
    {
      l.position = new_position;
      new_position += SmallTypes::ਮਾਪ;
    }

    ਸਾਰਣੀ->ligature.len = obj.real_links.ਲੰਮਾਈ;
    v->shrink_ਬਫਰ (ਮੰਜ਼ਿਲ_remove * SmallTypes::ਮਾਪ);
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> clone_range (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                        ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ,
                                        hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> liga_counts,
                                        ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ) ਅਚਲ
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
               "  Cloning LigatureSubstFormat1 (%u) range [%u, %u).", this_ਸੂਚਕਾਂਕ, ਸ਼ੁਰੂ, ਸਿਰਾ);

    // Create an oversized new liga subst, we'll adjust the size down later. We don't know
    // the final size until we process it but we also need it to exist while we're processing
    // so that nodes can be moved to it as needed.
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਭਾਜ_ਮਾਪ = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_ਮਾਪ
                          + ligature_ਸੈੱਟ.get_ਮਾਪ() - ligature_ਸੈੱਟ.len.get_ਮਾਪ();

    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ liga_subst_ਅਭਾਜ_id, c.create_ਗੰਢ (ਅਭਾਜ_ਮਾਪ));

    LigatureSubstFormat1* liga_subst_ਅਭਾਜ = (LigatureSubstFormat1*) c.ਗ੍ਰਾਫ.object (liga_subst_ਅਭਾਜ_id).ਸਿਰ_2;
    liga_subst_ਅਭਾਜ->format = ਇਹ->format;
    liga_subst_ਅਭਾਜ->ligature_ਸੈੱਟ.len = ਇਹ->ligature_ਸੈੱਟ.len;

    // Create a place holder coverage prime id since we need to add virtual links to it while
    // generating liga and liga sets. Afterwards it will be updated to have the correct coverage.
    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_id, c.ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (this_ਸੂਚਕਾਂਕ, &coverage));
    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_ਅਭਾਜ_id, c.ਗ੍ਰਾਫ.duplicate(coverage_id));
    ਸਵੈਚਾਲਿਤ& coverage_ਅਭਾਜ_ਸ਼ੀਰਸ਼ = c.ਗ੍ਰਾਫ.vertices_[coverage_ਅਭਾਜ_id];

    TRY(c.ਗ੍ਰਾਫ.vertices_[liga_subst_ਅਭਾਜ_id].ਜੋੜੋ_real_link (SmallTypes::ਮਾਪ, coverage_ਅਭਾਜ_id, 2));
    TRY(coverage_ਅਭਾਜ_ਸ਼ੀਰਸ਼.ਜੋੜੋ_ਮਾਪਾ (liga_subst_ਅਭਾਜ_id, ਝੂਠ));

    // Locate all liga sets with ligas between start and end.
    // Clone or move them as needed.
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਸੈੱਟ_ਗਿਣਤੀ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਸੈੱਟ_ਸ਼ੁਰੂ = HB_ਗ੍ਰਾਫ_INVALID;
    ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਸੈੱਟ_ਸਿਰਾ = 0; // inclusive
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < liga_counts.ਲੰਮਾਈ; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ligas = liga_counts[i];

      ਬਿਨਾਨਿਸ਼ਾਨੀ current_ਸ਼ੁਰੂ = ਗਿਣਤੀ_2;
      ਬਿਨਾਨਿਸ਼ਾਨੀ current_ਸਿਰਾ = ਗਿਣਤੀ_2 + ਅੰਕ_ligas;

      ਜੇ (current_ਸ਼ੁਰੂ >= ਸਿਰਾ || ਸ਼ੁਰੂ >= current_ਸਿਰਾ) {
        // No intersection, so just skip
        ਗਿਣਤੀ_2 += ਅੰਕ_ligas;
        ਜਾਰੀ;
      }

      TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ liga_ਸੈੱਟ, c.ਗ੍ਰਾਫ.as_mutable_ਸਾਰਣੀ<Ligature_ਸੈੱਟ>(this_ਸੂਚਕਾਂਕ, &ligature_ਸੈੱਟ[i]));

      ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਸੈੱਟ_ਸੂਚਕਾਂਕ = liga_ਸੈੱਟ.ਸੂਚਕਾਂਕ_2;
      // Bounds may need to be adjusted if some ligas have been previously removed.
      hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> liga_bounds = current_liga_ਸੈੱਟ_bounds(c, liga_ਸੈੱਟ_ਸੂਚਕਾਂਕ, liga_ਸੈੱਟ.ਸ਼ੀਰਸ਼->obj ());
      current_ਸ਼ੁਰੂ = hb_max(ਗਿਣਤੀ_2 + liga_bounds.ਪਹਿਲਾ, current_ਸ਼ੁਰੂ);
      current_ਸਿਰਾ = hb_min(ਗਿਣਤੀ_2 + liga_bounds.second, current_ਸਿਰਾ);

      ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਸੈੱਟ_ਅਭਾਜ_id;
      ਜੇ (current_ਸ਼ੁਰੂ >= ਸ਼ੁਰੂ && current_ਸਿਰਾ <= ਸਿਰਾ) {
        // This liga set is fully contined within [start, end)
        // We can move the entire ligaset to the new liga subset object.
        liga_ਸੈੱਟ_ਸਿਰਾ = i;
        ਜੇ (i < liga_ਸੈੱਟ_ਸ਼ੁਰੂ) liga_ਸੈੱਟ_ਸ਼ੁਰੂ = i;
        TRY_ਸੌਂਪੋ (liga_ਸੈੱਟ_ਅਭਾਜ_id, c.ਗ੍ਰਾਫ.move_ਬੱਚਾ<> (this_ਸੂਚਕਾਂਕ,
                              &ligature_ਸੈੱਟ[i],
                              liga_subst_ਅਭਾਜ_id,
                              &liga_subst_ਅਭਾਜ->ligature_ਸੈੱਟ[liga_ਸੈੱਟ_ਗਿਣਤੀ++]));
        compact_liga_ਸੈੱਟ (c, liga_ਸੈੱਟ.ਸਾਰਣੀ, liga_ਸੈੱਟ.ਸ਼ੀਰਸ਼);
      }
      ਨਹੀਂਤਾਂ
      {
        // This liga set partially overlaps [start, end). We'll need to create
        // a new liga set sub table and move the intersecting ligas to it.
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ = hb_max(ਸ਼ੁਰੂ, current_ਸ਼ੁਰੂ) - ਗਿਣਤੀ_2;
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਸੂਚਕਾਂਕ = hb_min(ਸਿਰਾ, current_ਸਿਰਾ) - ਗਿਣਤੀ_2;
        ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਗਿਣਤੀ = ਸਿਰਾ_ਸੂਚਕਾਂਕ - ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ;
        TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ ਨਤੀਜਾ, new_liga_ਸੈੱਟ(c, liga_ਗਿਣਤੀ));
        liga_ਸੈੱਟ_ਅਭਾਜ_id = ਨਤੀਜਾ.ਪਹਿਲਾ;

        TRY (c.ਗ੍ਰਾਫ.move_children<OT::Offset16>(
          liga_ਸੈੱਟ_ਸੂਚਕਾਂਕ,
          2 + ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ * 2,
          2 + ਸਿਰਾ_ਸੂਚਕਾਂਕ * 2,
          liga_ਸੈੱਟ_ਅਭਾਜ_id,
          2));

        liga_ਸੈੱਟ_ਸਿਰਾ = i;
        ਜੇ (i < liga_ਸੈੱਟ_ਸ਼ੁਰੂ) liga_ਸੈੱਟ_ਸ਼ੁਰੂ = i;
        TRY (c.ਗ੍ਰਾਫ.ਜੋੜੋ_link(&liga_subst_ਅਭਾਜ->ligature_ਸੈੱਟ[liga_ਸੈੱਟ_ਗਿਣਤੀ++], liga_subst_ਅਭਾਜ_id, liga_ਸੈੱਟ_ਅਭਾਜ_id));
      }

      // The new liga and all children set needs to have a virtual link to the new coverage table:
      ਅਚਲ ਸਵੈਚਾਲਿਤ& liga_ਸੈੱਟ_ਅਭਾਜ = c.ਗ੍ਰਾਫ.vertices_[liga_ਸੈੱਟ_ਅਭਾਜ_id].obj ();
      clear_virtual_links(c, liga_ਸੈੱਟ_ਅਭਾਜ_id);
      TRY(ਜੋੜੋ_virtual_link(c, liga_ਸੈੱਟ_ਅਭਾਜ_id, coverage_ਅਭਾਜ_id));
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& l : liga_ਸੈੱਟ_ਅਭਾਜ.real_links) {
        clear_virtual_links(c, l.objidx);
        TRY(ਜੋੜੋ_virtual_link(c, l.objidx, coverage_ਅਭਾਜ_id));
      }

      ਗਿਣਤੀ_2 += ਅੰਕ_ligas;
    }

    c.ਗ੍ਰਾਫ.vertices_[liga_subst_ਅਭਾਜ_id].shrink_ਬਫਰ ((liga_subst_ਅਭਾਜ->ligature_ਸੈੱਟ.len - liga_ਸੈੱਟ_ਗਿਣਤੀ) * SmallTypes::ਮਾਪ);
    liga_subst_ਅਭਾਜ->ligature_ਸੈੱਟ.len = liga_ਸੈੱਟ_ਗਿਣਤੀ;

    TRY (Coverage::filter_coverage (c,
                                    coverage_ਅਭਾਜ_id,
                                    liga_ਸੈੱਟ_ਸ਼ੁਰੂ, liga_ਸੈੱਟ_ਸਿਰਾ + 1));

    ਵਾਪਸ Ok(liga_subst_ਅਭਾਜ_id);
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> shrink (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਗਿਣਤੀ,
                               hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> liga_counts,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2)
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
               "  Shrinking LigatureSubstFormat1 (%u) to [0, %u).",
               this_ਸੂਚਕਾਂਕ,
               ਗਿਣਤੀ_2);
    ਜੇ (ਗਿਣਤੀ_2 >= old_ਗਿਣਤੀ)
      ਵਾਪਸ Ok();

    hb_ਸੈੱਟ_t retained_indices;
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_liga_ਸੈੱਟ_ਗਿਣਤੀ = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < liga_counts.ਲੰਮਾਈ; i++)
    {
      TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ liga_ਸੈੱਟ, c.ਗ੍ਰਾਫ.as_mutable_ਸਾਰਣੀ<Ligature_ਸੈੱਟ>(this_ਸੂਚਕਾਂਕ, &ligature_ਸੈੱਟ[i]));

      // We need the virtual links to coverage removed from all descendants on this liga subst.
      // If any are left when we try to mutate the coverage table later it will be unnessecarily
      // duplicated. Code later on will re-add the virtual links as needed (via retained_indices).
      clear_virtual_links(c, liga_ਸੈੱਟ.ਸੂਚਕਾਂਕ_2);
      retained_indices.ਜੋੜੋ(liga_ਸੈੱਟ.ਸੂਚਕਾਂਕ_2);

      TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_id, ligature_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_object_id (liga_ਸੈੱਟ));

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < liga_ਸੈੱਟ.ਸਾਰਣੀ->ligature.len; i++) {
        ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_id[i];
        ਜੇ (liga_ਸੂਚਕਾਂਕ != HB_ਗ੍ਰਾਫ_INVALID) {
          clear_virtual_links(c, liga_ਸੂਚਕਾਂਕ);
          retained_indices.ਜੋੜੋ(liga_ਸੂਚਕਾਂਕ);
        }
      }

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ligas = liga_counts[i];
      ਜੇ (ਅੰਕ_ligas >= ਗਿਣਤੀ_2) {
        // drop the trailing liga's from this set and all subsequent liga sets
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ligas_ਮੰਜ਼ਿਲ_remove = ਅੰਕ_ligas - ਗਿਣਤੀ_2;
        new_liga_ਸੈੱਟ_ਗਿਣਤੀ = i + 1;
        c.ਗ੍ਰਾਫ.vertices_[liga_ਸੈੱਟ.ਸੂਚਕਾਂਕ_2].shrink_ਬਫਰ (ਅੰਕ_ligas_ਮੰਜ਼ਿਲ_remove * SmallTypes::ਮਾਪ);
        liga_ਸੈੱਟ.ਸਾਰਣੀ->ligature.len = ਗਿਣਤੀ_2;
        ਰੋਕੋ;
      } ਨਹੀਂਤਾਂ {
        ਗਿਣਤੀ_2 -= ਅੰਕ_ligas;
      }
    }

    // Adjust liga set array
    ਸਵੈਚਾਲਿਤ& this_ਸ਼ੀਰਸ਼ = c.ਗ੍ਰਾਫ.vertices_[this_ਸੂਚਕਾਂਕ];
    this_ਸ਼ੀਰਸ਼.shrink_ਬਫਰ ((ligature_ਸੈੱਟ.len - new_liga_ਸੈੱਟ_ਗਿਣਤੀ) * SmallTypes::ਮਾਪ);
    ligature_ਸੈੱਟ.len = new_liga_ਸੈੱਟ_ਗਿਣਤੀ;

    // Coverage matches the number of liga sets so rebuild as needed
    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_idx, c.ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (this_ਸੂਚਕਾਂਕ, &ਇਹ->coverage));

    ਸਵੈਚਾਲਿਤ& coverage_v = c.ਗ੍ਰਾਫ.vertices_[coverage_idx];
    ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_ਮਾਪ = coverage_v.ਸਾਰਣੀ_ਮਾਪ ();
    ਅਚਲ Coverage* coverage_ਸਾਰਣੀ = (ਅਚਲ Coverage*) coverage_v.obj ().ਸਿਰ_2;

    ਜੇ (coverage_v.is_shared ())
    {
      TRY_ਸੌਂਪੋ (coverage_idx, c.ਗ੍ਰਾਫ.remap_ਬੱਚਾ (this_ਸੂਚਕਾਂਕ, coverage_idx));
    }

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i : retained_indices.iter())
      TRY(ਜੋੜੋ_virtual_link(c, i, coverage_idx));

    ਸਵੈਚਾਲਿਤ new_coverage =
        + hb_zip (coverage_ਸਾਰਣੀ->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> p) {
          ਵਾਪਸ p.second < new_liga_ਸੈੱਟ_ਗਿਣਤੀ;
        })
        | hb_map_retains_ਛਾਂਟੀ (hb_ਪਹਿਲਾ)
        ;

    ਵਾਪਸ Coverage::make_coverage (c, new_coverage, coverage_idx, coverage_ਮਾਪ);
  }
};

ਬਣਤਰ LigatureSubst : ਜਨਤਕ OT::Layout::GSUB_impl::LigatureSubst
{

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>> ਵੰਡੋ_subtables (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                                         ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ)
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1:
      hb_barrier ();
      ਵਾਪਸ ((LigatureSubstFormat1*)(&u.format1))->ਵੰਡੋ_subtables (c, this_ਸੂਚਕਾਂਕ);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 2: HB_FALLTHROUGH;
      // 24-bit LigatureSubst already has widened offsets; leave it unsplit.
#ਜੇ_ਅੰਤ
    ਮੂਲ:
      ਵਾਪਸ Ok(hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ());
    }
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼) ਅਚਲ
  {
    size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ ();
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < u.format.v.get_ਮਾਪ ())) ਵਾਪਸ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1:
      hb_barrier ();
      ਵਾਪਸ ((LigatureSubstFormat1*)(&u.format1))->sanitize (ਸ਼ੀਰਸ਼);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 2:  HB_FALLTHROUGH;
#ਜੇ_ਅੰਤ
    ਮੂਲ:
      // We don't handle format 2 here.
      ਵਾਪਸ Err(SANITIZE_FAILURE);
    }
  }
};

}

#ਜੇ_ਅੰਤ  // GRAPH_LIGATURE_GRAPH_HH
