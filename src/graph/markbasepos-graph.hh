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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਗ੍ਰਾਫ_MARKBASEPOS_ਗ੍ਰਾਫ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗ੍ਰਾਫ_MARKBASEPOS_ਗ੍ਰਾਫ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "split-helpers.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "coverage-graph.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../OT/Layout/GPOS/MarkBasePos.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../OT/Layout/GPOS/PosLookupSubTable.hh"

ਨਾਮਾਕਾਸ਼ ਗ੍ਰਾਫ {

ਬਣਤਰ Anchor_ਮੈਟ੍ਰਿਕਸ : ਜਨਤਕ OT::Layout::GPOS_impl::Anchor_ਮੈਟ੍ਰਿਕਸ<SmallTypes>
{
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼, ਬਿਨਾਨਿਸ਼ਾਨੀ class_ਗਿਣਤੀ) ਅਚਲ
  {
    size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ();
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < Anchor_ਮੈਟ੍ਰਿਕਸ::min_ਮਾਪ)) ਵਾਪਸ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < Anchor_ਮੈਟ੍ਰਿਕਸ::min_ਮਾਪ +
                  OT::Offset16::static_ਮਾਪ * class_ਗਿਣਤੀ * ਇਹ->ਪੰਕਤੀਆਂ))
      ਵਾਪਸ Err(SANITIZE_FAILURE);
    ਵਾਪਸ Ok();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> shrink (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ old_class_ਗਿਣਤੀ,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ new_class_ਗਿਣਤੀ)
  {
    ਜੇ (unlikely (new_class_ਗਿਣਤੀ >= old_class_ਗਿਣਤੀ)) ਵਾਪਸ Err(INVALID_ਦਲੀਲ);
    ਸਵੈਚਾਲਿਤ& v = c.ਗ੍ਰਾਫ.vertices_[this_ਸੂਚਕਾਂਕ];
    ਅਚਲ ਸਵੈਚਾਲਿਤ& o = v.obj ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ_ਗਿਣਤੀ = ਪੰਕਤੀਆਂ;
    v.ਸੈੱਟ_ਬਫਰ (o.ਸਿਰ_2,
      o.ਸਿਰ_2 +
      Anchor_ਮੈਟ੍ਰਿਕਸ::min_ਮਾਪ +
      OT::Offset16::static_ਮਾਪ * ਅਧਾਰ_ਗਿਣਤੀ * new_class_ਗਿਣਤੀ);

    // Reposition links into the new indexing scheme.
    ਲਈ (ਸਵੈਚਾਲਿਤ& link : v.real_links_writer ())
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2 = (link.position - 2) / 2;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ = ਸੂਚਕਾਂਕ_2 / old_class_ਗਿਣਤੀ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ klass = ਸੂਚਕਾਂਕ_2 % old_class_ਗਿਣਤੀ;
      ਜੇ (unlikely (klass >= new_class_ਗਿਣਤੀ))
        // should have already been removed
        ਵਾਪਸ Err(INVALID_ਦਲੀਲ);

      ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਸੂਚਕਾਂਕ = ਅਧਾਰ * new_class_ਗਿਣਤੀ + klass;

      link.position = (ਅੱਖਰ*) &(ਇਹ->ਮੈਟ੍ਰਿਕਸ_Z[new_ਸੂਚਕਾਂਕ]) - (ਅੱਖਰ*) ਇਹ;
    }

    ਵਾਪਸ Ok();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> clone (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ class_ਗਿਣਤੀ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ_ਗਿਣਤੀ = ਪੰਕਤੀਆਂ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_class_ਗਿਣਤੀ = ਸਿਰਾ - ਸ਼ੁਰੂ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = Anchor_ਮੈਟ੍ਰਿਕਸ::min_ਮਾਪ +
                    OT::Offset16::static_ਮਾਪ * new_class_ਗਿਣਤੀ * ਪੰਕਤੀਆਂ;
    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਭਾਜ_id, c.create_ਗੰਢ (ਮਾਪ));
    Anchor_ਮੈਟ੍ਰਿਕਸ* ਅਭਾਜ = (Anchor_ਮੈਟ੍ਰਿਕਸ*) c.ਗ੍ਰਾਫ.object (ਅਭਾਜ_id).ਸਿਰ_2;
    ਅਭਾਜ->ਪੰਕਤੀਆਂ = ਅਧਾਰ_ਗਿਣਤੀ;

    ਸਵੈਚਾਲਿਤ& v = c.ਗ੍ਰਾਫ.vertices_[this_ਸੂਚਕਾਂਕ];
    ਅਚਲ ਸਵੈਚਾਲਿਤ& o = v.obj ();

    ਪੂਰਨ ਅੰਕ_links = o.real_links.ਲੰਮਾਈ;
    ਲਈ (ਪੂਰਨ i = 0; i < ਅੰਕ_links; i++)
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ& link = o.real_links[i];
      ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਸੂਚਕਾਂਕ = (link.position - 2) / OT::Offset16::static_ਮਾਪ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ klass = old_ਸੂਚਕਾਂਕ % class_ਗਿਣਤੀ;
      ਜੇ (klass < ਸ਼ੁਰੂ || klass >= ਸਿਰਾ) ਜਾਰੀ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ = old_ਸੂਚਕਾਂਕ / class_ਗਿਣਤੀ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ new_klass = klass - ਸ਼ੁਰੂ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਸੂਚਕਾਂਕ = ਅਧਾਰ * new_class_ਗਿਣਤੀ + new_klass;


      ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ_idx = link.objidx;
      TRY (c.ਗ੍ਰਾਫ.ਜੋੜੋ_link (&(ਅਭਾਜ->ਮੈਟ੍ਰਿਕਸ_Z[new_ਸੂਚਕਾਂਕ]),
                             ਅਭਾਜ_id,
                             ਬੱਚਾ_idx));

      ਸਵੈਚਾਲਿਤ& ਬੱਚਾ = c.ਗ੍ਰਾਫ.vertices_[ਬੱਚਾ_idx];
      ਬੱਚਾ.remove_ਮਾਪਾ (this_ਸੂਚਕਾਂਕ);

      v.remove_real_link_unordered (i);
      ਅੰਕ_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.ਛਾਂਟੋ_real_links ();

    ਵਾਪਸ Ok(ਅਭਾਜ_id);
  }
};

ਬਣਤਰ MarkArray : ਜਨਤਕ OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼) ਅਚਲ
  {
    size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ min_ਮਾਪ = MarkArray::min_ਮਾਪ;
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < min_ਮਾਪ)) ਵਾਪਸ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < get_ਮਾਪ ())) ਵਾਪਸ Err(SANITIZE_FAILURE);
    ਵਾਪਸ Ok();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> shrink (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                               ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ>& mark_array_links,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ new_class_ਗਿਣਤੀ)
  {
    ਸਵੈਚਾਲਿਤ& v = c.ਗ੍ਰਾਫ.vertices_[this_ਸੂਚਕਾਂਕ];
    ਅਚਲ ਸਵੈਚਾਲਿਤ& o = v.obj ();
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& link : o.real_links)
      c.ਗ੍ਰਾਫ.vertices_[link.objidx].remove_ਮਾਪਾ (this_ਸੂਚਕਾਂਕ);
    v.clear_real_links ();

    ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਸੂਚਕਾਂਕ = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& ਰਿਕਾਰਡ : ਇਹ->iter ())
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ klass = ਰਿਕਾਰਡ.klass;
      ਜੇ (klass >= new_class_ਗਿਣਤੀ) ਜਾਰੀ;

      (*ਇਹ)[new_ਸੂਚਕਾਂਕ].klass = klass;
      ਬਿਨਾਨਿਸ਼ਾਨੀ position = (ਅੱਖਰ*) &ਰਿਕਾਰਡ.markAnchor - (ਅੱਖਰ*) ਇਹ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ* objidx;
      ਜੇ (!mark_array_links.has (position, &objidx))
      {
        new_ਸੂਚਕਾਂਕ++;
        ਜਾਰੀ;
      }

      TRY (c.ਗ੍ਰਾਫ.ਜੋੜੋ_link (&(*ਇਹ)[new_ਸੂਚਕਾਂਕ].markAnchor, this_ਸੂਚਕਾਂਕ, *objidx));
      new_ਸੂਚਕਾਂਕ++;
    }

    ਇਹ->len = new_ਸੂਚਕਾਂਕ;
    v.ਸੈੱਟ_ਬਫਰ (o.ਸਿਰ_2, o.ਸਿਰ_2 + MarkArray::min_ਮਾਪ +
                  OT::Layout::GPOS_impl::Mark_ਰਿਕਾਰਡ<SmallTypes>::static_ਮਾਪ * new_ਸੂਚਕਾਂਕ);

    ਵਾਪਸ Ok();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> clone (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ,
                                  ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ>& pos_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ,
                                  hb_ਸੈੱਟ_t& marks,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_class)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = MarkArray::min_ਮਾਪ +
                    OT::Layout::GPOS_impl::Mark_ਰਿਕਾਰਡ<SmallTypes>::static_ਮਾਪ *
                    marks.get_population ();
    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਭਾਜ_id, c.create_ਗੰਢ (ਮਾਪ));
    MarkArray* ਅਭਾਜ = (MarkArray*) c.ਗ੍ਰਾਫ.object (ਅਭਾਜ_id).ਸਿਰ_2;
    ਅਭਾਜ->len = marks.get_population ();


    ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
    ਲਈ (hb_codepoint_t mark : marks)
    {
      (*ਅਭਾਜ)[i].klass = (*ਇਹ)[mark].klass - ਸ਼ੁਰੂ_class;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ_pos = (ਅੱਖਰ*) &((*ਇਹ)[mark].markAnchor) - (ਅੱਖਰ*) ਇਹ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ* anchor_ਸੂਚਕਾਂਕ;
      ਜੇ (pos_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ.has (ਵਿੱਥ_pos, &anchor_ਸੂਚਕਾਂਕ))
        TRY (c.ਗ੍ਰਾਫ.move_ਬੱਚਾ (this_ਸੂਚਕਾਂਕ,
                                 &((*ਇਹ)[mark].markAnchor),
                                 ਅਭਾਜ_id,
                                 &((*ਅਭਾਜ)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.ਗ੍ਰਾਫ.vertices_[this_ਸੂਚਕਾਂਕ].ਛਾਂਟੋ_real_links ();

    ਵਾਪਸ Ok(ਅਭਾਜ_id);
  }
};

ਬਣਤਰ Mark_ਅਧਾਰ_Pos_Format1 : ਜਨਤਕ OT::Layout::GPOS_impl::Mark_ਅਧਾਰ_Pos_Format1_2<SmallTypes>
{
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼) ਅਚਲ
  {
    size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ ();
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < Mark_ਅਧਾਰ_Pos_Format1::static_ਮਾਪ))
      ਵਾਪਸ Err(SANITIZE_FAILURE);
    ਵਾਪਸ Ok();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>> ਵੰਡੋ_subtables (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                                         ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ)
  {
    hb_ਸੈੱਟ_t ਦੇਖਿਆ;

    TRY_ਸੌਂਪੋ (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ_coverage_id, c.ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (this_ਸੂਚਕਾਂਕ, &ਅਧਾਰ_Coverage));

    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ_ਮਾਪ =
        OT::Layout::GPOS_impl::Mark_ਅਧਾਰ_Pos_Format1_2<SmallTypes>::min_ਮਾਪ +
        MarkArray::min_ਮਾਪ +
        Anchor_ਮੈਟ੍ਰਿਕਸ::min_ਮਾਪ +
        c.ਗ੍ਰਾਫ.vertices_[ਅਧਾਰ_coverage_id].ਸਾਰਣੀ_ਮਾਪ ();

    TRY_ਸੌਂਪੋ (hb_ਸਦਿਸ਼_t<class_info_t> class_ਮੰਜ਼ਿਲ_info, get_class_info (c, this_ਸੂਚਕਾਂਕ));

    ਬਿਨਾਨਿਸ਼ਾਨੀ class_ਗਿਣਤੀ = class_ਗਿਣਤੀ_2;
    TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ ਅਧਾਰ_array, c.ਗ੍ਰਾਫ.as_ਸਾਰਣੀ<Anchor_ਮੈਟ੍ਰਿਕਸ> (this_ਸੂਚਕਾਂਕ,
                                                          &ਅਧਾਰ_Array,
                                                          class_ਗਿਣਤੀ));
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ_ਗਿਣਤੀ = ਅਧਾਰ_array.ਸਾਰਣੀ->ਪੰਕਤੀਆਂ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ partial_coverage_ਮਾਪ = 4;
    ਬਿਨਾਨਿਸ਼ਾਨੀ accumulated = ਅਧਾਰ_ਮਾਪ;
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਵੰਡੋ_points;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ klass = 0; klass < class_ਗਿਣਤੀ; klass++)
    {
      class_info_t& info = class_ਮੰਜ਼ਿਲ_info[klass];
      partial_coverage_ਮਾਪ += OT::HBUINT16::static_ਮਾਪ * info.marks.get_population ();
      ਬਿਨਾਨਿਸ਼ਾਨੀ accumulated_delta =
          OT::Layout::GPOS_impl::Mark_ਰਿਕਾਰਡ<SmallTypes>::static_ਮਾਪ * info.marks.get_population () +
          OT::Offset16::static_ਮਾਪ * ਅਧਾਰ_ਗਿਣਤੀ;

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ objidx : info.ਬੱਚਾ_indices)
        accumulated_delta += c.ਗ੍ਰਾਫ.ਲੱਭੋ_subgraph_ਮਾਪ (objidx, ਦੇਖਿਆ).ਮੁੱਲ_or (0);

      accumulated += accumulated_delta;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ = accumulated + partial_coverage_ਮਾਪ;

      ਜੇ (ਕੁੱਲ >= (1 << 16))
      {
        ਵੰਡੋ_points.push (klass);
        TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਵੰਡੋ_points, ਵੰਡ_FAILURE));
        accumulated = ਅਧਾਰ_ਮਾਪ + accumulated_delta;
        partial_coverage_ਮਾਪ = 4 + OT::HBUINT16::static_ਮਾਪ * info.marks.get_population ();
        ਦੇਖਿਆ.clear (); // node sharing isn't allowed between splits.
      }
    }


    TRY_ਸੌਂਪੋ (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ mark_array_id, c.ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (this_ਸੂਚਕਾਂਕ, &markArray));
    TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ mark_array_links, c.ਗ੍ਰਾਫ.vertices_[mark_array_id].position_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ_map ());

    ਵੰਡੋ_context_t ਵੰਡੋ_context {
      c,
      ਇਹ,
      this_ਸੂਚਕਾਂਕ,
      std::move (class_ਮੰਜ਼ਿਲ_info),
      std::move (mark_array_links),
    };

    ਵਾਪਸ actuate_subtable_ਵੰਡੋ<ਵੰਡੋ_context_t> (ਵੰਡੋ_context, ਵੰਡੋ_points);
  }

 ਨਿਜੀ:

  ਬਣਤਰ class_info_t {
    hb_ਸੈੱਟ_t marks;
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਬੱਚਾ_indices;
  };

  ਬਣਤਰ ਵੰਡੋ_context_t {
    gsubgpos_ਗ੍ਰਾਫ_context_t& c;
    Mark_ਅਧਾਰ_Pos_Format1* thiz;
    ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ;
    hb_ਸਦਿਸ਼_t<class_info_t> class_ਮੰਜ਼ਿਲ_info;
    hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> mark_array_links;

    hb_ਸੈੱਟ_t marks_for (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ)
    {
      hb_ਸੈੱਟ_t marks;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ klass = ਸ਼ੁਰੂ; klass < ਸਿਰਾ; klass++)
      {
        + class_ਮੰਜ਼ਿਲ_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      ਵਾਪਸ marks;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ original_ਗਿਣਤੀ_2 ()
    {
      ਵਾਪਸ thiz->class_ਗਿਣਤੀ_2;
    }

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> clone_range (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ)
    {
      ਵਾਪਸ thiz->clone_range (*ਇਹ, ਇਹ->this_ਸੂਚਕਾਂਕ, ਸ਼ੁਰੂ, ਸਿਰਾ);
    }

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> shrink (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2)
    {
      ਵਾਪਸ thiz->shrink (*ਇਹ, ਇਹ->this_ਸੂਚਕਾਂਕ, ਗਿਣਤੀ_2);
    }
  };

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_ਸਦਿਸ਼_t<class_info_t>> get_class_info (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                                            ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ)
  {
    hb_ਸਦਿਸ਼_t<class_info_t> class_ਮੰਜ਼ਿਲ_info;

    ਬਿਨਾਨਿਸ਼ਾਨੀ class_ਗਿਣਤੀ = class_ਗਿਣਤੀ_2;
    ਜੇ (!class_ਗਿਣਤੀ) ਵਾਪਸ class_ਮੰਜ਼ਿਲ_info;

    ਜੇ (unlikely (!class_ਮੰਜ਼ਿਲ_info.resize (class_ਗਿਣਤੀ)))
      ਵਾਪਸ Err(ਵੰਡ_FAILURE);

    TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ mark_array, c.ਗ੍ਰਾਫ.as_ਸਾਰਣੀ<MarkArray> (this_ਸੂਚਕਾਂਕ, &markArray));

    ਬਿਨਾਨਿਸ਼ਾਨੀ mark_ਗਿਣਤੀ = mark_array.ਸਾਰਣੀ->len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ mark = 0; mark < mark_ਗਿਣਤੀ; mark++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ klass = (*mark_array.ਸਾਰਣੀ)[mark].get_class ();
      ਜੇ (klass >= class_ਗਿਣਤੀ) ਜਾਰੀ;
      class_ਮੰਜ਼ਿਲ_info[klass].marks.ਜੋੜੋ (mark);
    }

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& link : mark_array.ਸ਼ੀਰਸ਼->obj ().real_links)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::Mark_ਰਿਕਾਰਡ<SmallTypes>::static_ਮਾਪ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ klass = (*mark_array.ਸਾਰਣੀ)[mark].get_class ();
      ਜੇ (klass >= class_ਗਿਣਤੀ) ਜਾਰੀ;
      class_ਮੰਜ਼ਿਲ_info[klass].ਬੱਚਾ_indices.push (link.objidx);
    }

    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ_array_id,
         c.ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (this_ਸੂਚਕਾਂਕ, &ਅਧਾਰ_Array));

    ਸਵੈਚਾਲਿਤ& ਅਧਾਰ_array_v = c.ਗ੍ਰਾਫ.vertices_[ਅਧਾਰ_array_id];

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& link : ਅਧਾਰ_array_v.obj ().real_links)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2 = (link.position - 2) / OT::Offset16::static_ਮਾਪ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ klass = ਸੂਚਕਾਂਕ_2 % class_ਗਿਣਤੀ;
      class_ਮੰਜ਼ਿਲ_info[klass].ਬੱਚਾ_indices.push (link.objidx);
    }

    ਵਾਪਸ class_ਮੰਜ਼ਿਲ_info;
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> shrink (ਵੰਡੋ_context_t& sc,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2)
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_ਸੂਚਕਾਂਕ,
               ਗਿਣਤੀ_2);

    ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਗਿਣਤੀ = class_ਗਿਣਤੀ_2;
    ਜੇ (ਗਿਣਤੀ_2 >= old_ਗਿਣਤੀ)
      ਵਾਪਸ Ok();

    class_ਗਿਣਤੀ_2 = ਗਿਣਤੀ_2;

    TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ mark_coverage, sc.c.ਗ੍ਰਾਫ.as_mutable_ਸਾਰਣੀ<Coverage> (this_ਸੂਚਕਾਂਕ,
                                                                    &markCoverage));

    hb_ਸੈੱਟ_t marks = sc.marks_for (0, ਗਿਣਤੀ_2);
    ਸਵੈਚਾਲਿਤ new_coverage =
        + hb_enumerate (mark_coverage.ਸਾਰਣੀ->iter ())
        | hb_filter (marks, hb_ਪਹਿਲਾ)
        | hb_map_retains_ਛਾਂਟੀ (hb_ਦੂਜਾ)
        ;
    TRY (Coverage::make_coverage (sc.c, + new_coverage,
                                  mark_coverage.ਸੂਚਕਾਂਕ_2,
                                  4 + 2 * marks.get_population ()));


    TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ ਅਧਾਰ_array, sc.c.ਗ੍ਰਾਫ.as_mutable_ਸਾਰਣੀ<Anchor_ਮੈਟ੍ਰਿਕਸ> (this_ਸੂਚਕਾਂਕ,
                                                                     &ਅਧਾਰ_Array,
                                                                     old_ਗਿਣਤੀ));

    TRY (ਅਧਾਰ_array.ਸਾਰਣੀ->shrink (sc.c,
                                   ਅਧਾਰ_array.ਸੂਚਕਾਂਕ_2,
                                   old_ਗਿਣਤੀ,
                                   ਗਿਣਤੀ_2));

    TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ mark_array, sc.c.ਗ੍ਰਾਫ.as_mutable_ਸਾਰਣੀ<MarkArray> (this_ਸੂਚਕਾਂਕ,
                                                                  &markArray));

    TRY (mark_array.ਸਾਰਣੀ->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.ਸੂਚਕਾਂਕ_2,
                                   ਗਿਣਤੀ_2));

    ਵਾਪਸ Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> clone_range (ਵੰਡੋ_context_t& sc,
                                        ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ,
                                        ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ) ਅਚਲ
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_ਸੂਚਕਾਂਕ, ਸ਼ੁਰੂ, ਸਿਰਾ);

    ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ = sc.c.ਗ੍ਰਾਫ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਭਾਜ_ਮਾਪ = OT::Layout::GPOS_impl::Mark_ਅਧਾਰ_Pos_Format1_2<SmallTypes>::static_ਮਾਪ;

    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਭਾਜ_id, sc.c.create_ਗੰਢ (ਅਭਾਜ_ਮਾਪ));

    Mark_ਅਧਾਰ_Pos_Format1* ਅਭਾਜ = (Mark_ਅਧਾਰ_Pos_Format1*) ਗ੍ਰਾਫ.object (ਅਭਾਜ_id).ਸਿਰ_2;
    ਅਭਾਜ->format = ਇਹ->format;
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_class_ਗਿਣਤੀ = ਸਿਰਾ - ਸ਼ੁਰੂ;
    ਅਭਾਜ->class_ਗਿਣਤੀ_2 = new_class_ਗਿਣਤੀ;

    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ_coverage_id,
         ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (sc.this_ਸੂਚਕਾਂਕ, &ਅਧਾਰ_Coverage));

    TRY (ਗ੍ਰਾਫ.ਜੋੜੋ_link (&(ਅਭਾਜ->ਅਧਾਰ_Coverage), ਅਭਾਜ_id, ਅਧਾਰ_coverage_id));

    TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ mark_coverage, sc.c.ਗ੍ਰਾਫ.as_ਸਾਰਣੀ<Coverage> (this_ਸੂਚਕਾਂਕ,
                                                            &markCoverage));

    hb_ਸੈੱਟ_t marks = sc.marks_for (ਸ਼ੁਰੂ, ਸਿਰਾ);
    ਸਵੈਚਾਲਿਤ new_coverage =
        + hb_enumerate (mark_coverage.ਸਾਰਣੀ->iter ())
        | hb_filter (marks, hb_ਪਹਿਲਾ)
        | hb_map_retains_ਛਾਂਟੀ (hb_ਦੂਜਾ)
        ;
    TRY (Coverage::ਜੋੜੋ_coverage (sc.c,
                                 ਅਭਾਜ_id,
                                 2,
                                 + new_coverage,
                                 marks.get_population () * 2 + 4));

    TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ mark_array,
         ਗ੍ਰਾਫ.as_mutable_ਸਾਰਣੀ <MarkArray> (sc.this_ਸੂਚਕਾਂਕ, &markArray));

    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ new_mark_array,
         mark_array.ਸਾਰਣੀ->clone (sc.c,
                                  mark_array.ਸੂਚਕਾਂਕ_2,
                                  sc.mark_array_links,
                                  marks,
                                  ਸ਼ੁਰੂ));
    TRY (ਗ੍ਰਾਫ.ਜੋੜੋ_link (&(ਅਭਾਜ->markArray), ਅਭਾਜ_id, new_mark_array));

    ਬਿਨਾਨਿਸ਼ਾਨੀ class_ਗਿਣਤੀ = class_ਗਿਣਤੀ_2;
    TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ ਅਧਾਰ_array,
         ਗ੍ਰਾਫ.as_mutable_ਸਾਰਣੀ<Anchor_ਮੈਟ੍ਰਿਕਸ> (sc.this_ਸੂਚਕਾਂਕ, &ਅਧਾਰ_Array, class_ਗਿਣਤੀ));

    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਅਧਾਰ_array,
         ਅਧਾਰ_array.ਸਾਰਣੀ->clone (sc.c,
                                  ਅਧਾਰ_array.ਸੂਚਕਾਂਕ_2,
                                  ਸ਼ੁਰੂ, ਸਿਰਾ, ਇਹ->class_ਗਿਣਤੀ_2));
    TRY (ਗ੍ਰਾਫ.ਜੋੜੋ_link (&(ਅਭਾਜ->ਅਧਾਰ_Array), ਅਭਾਜ_id, new_ਅਧਾਰ_array));

    ਵਾਪਸ Ok(ਅਭਾਜ_id);
  }
};


ਬਣਤਰ Mark_ਅਧਾਰ_Pos : ਜਨਤਕ OT::Layout::GPOS_impl::Mark_ਅਧਾਰ_Pos
{
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>> ਵੰਡੋ_subtables (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                                         ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ)
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1:
      hb_barrier ();
      ਵਾਪਸ ((Mark_ਅਧਾਰ_Pos_Format1*)(&u.format1))->ਵੰਡੋ_subtables (c, this_ਸੂਚਕਾਂਕ);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
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
      ਵਾਪਸ ((Mark_ਅਧਾਰ_Pos_Format1*)(&u.format1))->sanitize (ਸ਼ੀਰਸ਼);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 2: HB_FALLTHROUGH;
#ਜੇ_ਅੰਤ
    ਮੂਲ:
      // We don't handle format 3 and 4 here.
      ਵਾਪਸ Err(SANITIZE_FAILURE);
    }
  }
};


}

#ਜੇ_ਅੰਤ  // GRAPH_MARKBASEPOS_GRAPH_HH
