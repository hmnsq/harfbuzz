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

#ਅਨ੍ਤਰ੍ਭਾਵ "graph.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../hb-ot-layout-gsubgpos.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../OT/Layout/GSUB/ExtensionSubst.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../OT/Layout/GSUB/SubstLookupSubTable.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "gsubgpos-context.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "pairpos-graph.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "markbasepos-graph.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "ligature-graph.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਗ੍ਰਾਫ_GSUBGPOS_ਗ੍ਰਾਫ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗ੍ਰਾਫ_GSUBGPOS_ਗ੍ਰਾਫ_HH

ਨਾਮਾਕਾਸ਼ ਗ੍ਰਾਫ {

ਬਣਤਰ Lookup;

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ ExtensionFormat1 : ਜਨਤਕ OT::ExtensionFormat1<T>
{
  ਖਾਲੀ reset(ਬਿਨਾਨਿਸ਼ਾਨੀ type)
  {
    ਇਹ->format = 1;
    ਇਹ->extensionLookupType = type;
    ਇਹ->extension_ਵਿੱਥ = 0;
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼) ਅਚਲ
  {
    size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ ();
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < OT::ExtensionFormat1<T>::static_ਮਾਪ))
      ਵਾਪਸ Err(SANITIZE_FAILURE);
    ਵਾਪਸ Ok();
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_lookup_type () ਅਚਲ
  {
    ਵਾਪਸ ਇਹ->extensionLookupType;
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> get_subtable_ਸੂਚਕਾਂਕ (ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ, ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਵਾਪਸ ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (this_ਸੂਚਕਾਂਕ, &ਇਹ->extension_ਵਿੱਥ);
  }
};

ਬਣਤਰ Lookup : ਜਨਤਕ OT::Lookup
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੰਖਿਆ_ਦਾ_subtables () ਅਚਲ
  {
    ਵਾਪਸ sub_ਸਾਰਣੀ.len;
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼) ਅਚਲ
  {
    size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ ();
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < OT::Lookup::min_ਮਾਪ)) ਵਾਪਸ Err(SANITIZE_FAILURE);
    hb_barrier ();
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < ਇਹ->get_ਮਾਪ ())) ਵਾਪਸ Err(SANITIZE_FAILURE);
    ਵਾਪਸ Ok();
  }

  ਤਾਰਕਿਕ is_extension (hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2) ਅਚਲ
  {
    ਵਾਪਸ lookupType == extension_type (ਸਾਰਣੀ_ਟੈਗ_2);
  }

  ਤਾਰਕਿਕ use_mark_filtering_ਸੈੱਟ () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ flag = lookupFlag;
    ਵਾਪਸ flag & 0x0010u;
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> make_extension (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                       ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ type = lookupType;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ext_type = extension_type (c.ਸਾਰਣੀ_ਟੈਗ_2);
    ਜੇ (!ext_type || is_extension (c.ਸਾਰਣੀ_ਟੈਗ_2))
    {
      // NOOP
      ਵਾਪਸ Ok();
    }

    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
               "Promoting lookup type %u (obj %u) to extension.",
               type,
               this_ਸੂਚਕਾਂਕ);

    hb_ਸੈੱਟ_t ਵੇਖਿਆ;
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> subtables;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < sub_ਸਾਰਣੀ.len; i++)
    {
      TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ subtable_ਸੂਚਕਾਂਕ, c.ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (this_ਸੂਚਕਾਂਕ, &sub_ਸਾਰਣੀ[i]));
      ਜੇ (ਵੇਖਿਆ.has (subtable_ਸੂਚਕਾਂਕ))
        ਜਾਰੀ;
      ਵੇਖਿਆ.ਜੋੜੋ (subtable_ਸੂਚਕਾਂਕ);
      subtables.push (subtable_ਸੂਚਕਾਂਕ);
    }
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਵੇਖਿਆ, ਵੰਡ_FAILURE));
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (subtables, ਵੰਡ_FAILURE));

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ subtable_ਸੂਚਕਾਂਕ : subtables)
      TRY (make_subtable_extension (c, this_ਸੂਚਕਾਂਕ, subtable_ਸੂਚਕਾਂਕ));

    lookupType = ext_type;
    ਵਾਪਸ Ok();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਵੰਡੋ_subtables_if_needed (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                                  ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ type = lookupType;
    ਤਾਰਕਿਕ is_ext = is_extension (c.ਸਾਰਣੀ_ਟੈਗ_2);

    ਜੇ (c.ਸਾਰਣੀ_ਟੈਗ_2 != HB_OT_ਟੈਗ_GPOS && c.ਸਾਰਣੀ_ਟੈਗ_2 != HB_OT_ਟੈਗ_GSUB)
      ਵਾਪਸ Ok();

    ਜੇ (!is_ext && !is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
      ਵਾਪਸ Ok();

    hb_ਸਦਿਸ਼_t<hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>>> all_new_subtables;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < sub_ਸਾਰਣੀ.len; i++)
    {
      // This assumes we are working with something like a GSUB/GPOS table, if
      // sanitization fails we ignore and skip splitting for the particular sub table.
      ਸਵੈਚਾਲਿਤ idx_res = c.ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (this_ਸੂਚਕਾਂਕ, &sub_ਸਾਰਣੀ[i]);
      ਜੇ (!idx_res.is_ok ()) ਜਾਰੀ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ subtable_ਸੂਚਕਾਂਕ = *idx_res;
      ਜੇ (is_ext) {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ext_subtable_ਸੂਚਕਾਂਕ = subtable_ਸੂਚਕਾਂਕ;
        ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
            (ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*)
            c.ਗ੍ਰਾਫ.object (ext_subtable_ਸੂਚਕਾਂਕ).ਸਿਰ_2;
        ਜੇ (!extension || !extension->sanitize (c.ਗ੍ਰਾਫ.vertices_[ext_subtable_ਸੂਚਕਾਂਕ]).is_ok ())
          ਜਾਰੀ;

        TRY_ਸੌਂਪੋ (subtable_ਸੂਚਕਾਂਕ, extension->get_subtable_ਸੂਚਕਾਂਕ (c.ਗ੍ਰਾਫ, ext_subtable_ਸੂਚਕਾਂਕ));
        type = extension->get_lookup_type ();
        ਜੇ (!is_supported_gpos_type(type, c) && !is_supported_gsub_type(type, c))
          ਜਾਰੀ;
      }

      hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>* ਵੰਡੋ_ਨਤੀਜਾ;
      ਜੇ (c.ਵੰਡੋ_subtables.has (subtable_ਸੂਚਕਾਂਕ, &ਵੰਡੋ_ਨਤੀਜਾ))
      {
        ਜੇ (ਵੰਡੋ_ਨਤੀਜਾ->ਲੰਮਾਈ == 0)
          ਜਾਰੀ;
        all_new_subtables.push (hb_pair(i, *ਵੰਡੋ_ਨਤੀਜਾ));
        TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (all_new_subtables, ਵੰਡ_FAILURE));
      }
      ਨਹੀਂਤਾਂ
      {
        hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> new_sub_tables;

        ਜੇ (c.ਸਾਰਣੀ_ਟੈਗ_2 == HB_OT_ਟੈਗ_GPOS) {
          ਚੋਣ (type)
          {
          ਹਾਲਤ 2:
            {
              TRY_ਸੌਂਪੋ (new_sub_tables, ਵੰਡੋ_subtable<PairPos> (c, subtable_ਸੂਚਕਾਂਕ));
              ਰੋਕੋ;
            }
          ਹਾਲਤ 4:
            {
              TRY_ਸੌਂਪੋ (new_sub_tables, ਵੰਡੋ_subtable<Mark_ਅਧਾਰ_Pos> (c, subtable_ਸੂਚਕਾਂਕ));
              ਰੋਕੋ;
            }
          ਮੂਲ:
            ਰੋਕੋ;
          }
        } ਨਹੀਂਤਾਂ ਜੇ (c.ਸਾਰਣੀ_ਟੈਗ_2 == HB_OT_ਟੈਗ_GSUB) {
          ਚੋਣ (type)
          {
          ਹਾਲਤ 4:
            {
              TRY_ਸੌਂਪੋ (new_sub_tables, ਵੰਡੋ_subtable<ਗ੍ਰਾਫ::LigatureSubst> (c, subtable_ਸੂਚਕਾਂਕ));
              ਰੋਕੋ;
            }
          ਮੂਲ:
            ਰੋਕੋ;
          }
        }

        c.ਵੰਡੋ_subtables.ਸੈੱਟ (subtable_ਸੂਚਕਾਂਕ, new_sub_tables);
        TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (c.ਵੰਡੋ_subtables, ਵੰਡ_FAILURE));
        ਜੇ (new_sub_tables) {
          all_new_subtables.push (hb_pair (i, std::move (new_sub_tables)));
          TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (all_new_subtables, ਵੰਡ_FAILURE));
        }
      }
    }

    ਜੇ (all_new_subtables) {
      TRY (ਜੋੜੋ_sub_tables (c, this_ਸੂਚਕਾਂਕ, type, all_new_subtables));
    }

    ਵਾਪਸ Ok();
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>> ਵੰਡੋ_subtable (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                                        ਬਿਨਾਨਿਸ਼ਾਨੀ objidx)
  {
    T* sub_ਸਾਰਣੀ_2 = (T*) c.ਗ੍ਰਾਫ.object (objidx).ਸਿਰ_2;
    ਜੇ (!sub_ਸਾਰਣੀ_2 || !sub_ਸਾਰਣੀ_2->sanitize (c.ਗ੍ਰਾਫ.vertices_[objidx]).is_ok ())
      ਵਾਪਸ Ok(hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ());

    ਵਾਪਸ sub_ਸਾਰਣੀ_2->ਵੰਡੋ_subtables (c, objidx);
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਜੋੜੋ_sub_tables (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                       ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ,
                                       ਬਿਨਾਨਿਸ਼ਾਨੀ type,
                                       ਅਚਲ hb_ਸਦਿਸ਼_t<hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>>>& subtable_ids)
  {
    ਤਾਰਕਿਕ is_ext = is_extension (c.ਸਾਰਣੀ_ਟੈਗ_2);
    ਸਵੈਚਾਲਿਤ* v = &c.ਗ੍ਰਾਫ.vertices_[this_ਸੂਚਕਾਂਕ];
    fix_existing_subtable_links (c, this_ਸੂਚਕਾਂਕ, subtable_ids);

    ਬਿਨਾਨਿਸ਼ਾਨੀ new_subtable_ਗਿਣਤੀ = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& p : subtable_ids)
      new_subtable_ਗਿਣਤੀ += p.second.ਲੰਮਾਈ;

    size_t new_ਮਾਪ = v->ਸਾਰਣੀ_ਮਾਪ ()
                      + new_subtable_ਗਿਣਤੀ * OT::Offset16::static_ਮਾਪ;
    ਅੱਖਰ* ਬਫਰ = (ਅੱਖਰ*) hb_calloc (1, new_ਮਾਪ);
    ਜੇ (unlikely (!ਬਫਰ)) ਵਾਪਸ Err(ਵੰਡ_FAILURE);
    ਸਵੈਚਾਲਿਤ res = c.ਜੋੜੋ_ਬਫਰ (ਬਫਰ);
    ਜੇ (unlikely (!res.is_ok ()))
    {
      hb_free (ਬਫਰ);
      ਵਾਪਸ res;
    }
    hb_memcpy (ਬਫਰ, v->obj ().ਸਿਰ_2, v->ਸਾਰਣੀ_ਮਾਪ());

    ਜੇ (use_mark_filtering_ਸੈੱਟ ())
      hb_memcpy (ਬਫਰ + new_ਮਾਪ - 2, v->obj ().ਪੂਛ - 2, 2);

    v->ਸੈੱਟ_ਬਫਰ (ਬਫਰ, ਬਫਰ + new_ਮਾਪ);

    Lookup* new_lookup = (Lookup*) ਬਫਰ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਖਿਸਕਾਓ = 0;
    new_lookup->sub_ਸਾਰਣੀ.len = sub_ਸਾਰਣੀ.len + new_subtable_ਗਿਣਤੀ;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& p : subtable_ids)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ_ਸੂਚਕਾਂਕ = p.ਪਹਿਲਾ + ਖਿਸਕਾਓ + 1;
      ਖਿਸਕਾਓ += p.second.ਲੰਮਾਈ;

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ subtable_id : p.second)
      {
        ਜੇ (is_ext)
        {
          TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ ext_id, create_extension_subtable (c, subtable_id, type));
          TRY(c.ਗ੍ਰਾਫ.vertices_[subtable_id].ਜੋੜੋ_ਮਾਪਾ (ext_id, ਝੂਠ));
          subtable_id = ext_id;
          // the reference to v may have changed on adding a node, so reassign it.
          v = &c.ਗ੍ਰਾਫ.vertices_[this_ਸੂਚਕਾਂਕ];
        }

        TRY(v->ਜੋੜੋ_real_link(2, subtable_id,
          (ਅੱਖਰ*) &new_lookup->sub_ਸਾਰਣੀ[ਵਿੱਥ_ਸੂਚਕਾਂਕ++] - (ਅੱਖਰ*) new_lookup));
        TRY(c.ਗ੍ਰਾਫ.vertices_[subtable_id].ਜੋੜੋ_ਮਾਪਾ (this_ਸੂਚਕਾਂਕ, ਝੂਠ));
      }
    }

    // Repacker sort order depends on link order, which we've messed up so resort it.
    v->ਛਾਂਟੋ_real_links ();

    // The head location of the lookup has changed, invalidating the lookups map entry
    // in the context. Update the map.
    c.lookups.ਸੈੱਟ (this_ਸੂਚਕਾਂਕ, new_lookup);
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (c.lookups, ਵੰਡ_FAILURE));
    ਵਾਪਸ Ok();
  }

  ਖਾਲੀ fix_existing_subtable_links (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                    ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ,
                                    ਅਚਲ hb_ਸਦਿਸ਼_t<hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>>>& subtable_ids)
  {
    ਸਵੈਚਾਲਿਤ& v = c.ਗ੍ਰਾਫ.vertices_[this_ਸੂਚਕਾਂਕ];
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਖਿਸਕਾਓ = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& p : subtable_ids)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਾਓ_ਸੂਚਕਾਂਕ = p.ਪਹਿਲਾ + ਖਿਸਕਾਓ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ pos_ਵਿੱਥ = p.second.ਲੰਮਾਈ * OT::Offset16::static_ਮਾਪ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਾਓ_ਵਿੱਥ = Lookup::min_ਮਾਪ + ਪਾਓ_ਸੂਚਕਾਂਕ * OT::Offset16::static_ਮਾਪ;
      ਖਿਸਕਾਓ += p.second.ਲੰਮਾਈ;

      ਲਈ (ਸਵੈਚਾਲਿਤ& l : v.all_links_writer ())
      {
        ਜੇ (l.position > ਪਾਓ_ਵਿੱਥ) l.position += pos_ਵਿੱਥ;
      }
    }
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> create_extension_subtable (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                                      ਬਿਨਾਨਿਸ਼ਾਨੀ subtable_ਸੂਚਕਾਂਕ,
                                                      ਬਿਨਾਨਿਸ਼ਾਨੀ type)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ extension_ਮਾਪ = OT::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>::static_ਮਾਪ;

    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ ext_ਸੂਚਕਾਂਕ, c.create_ਗੰਢ (extension_ਮਾਪ));

    ਸਵੈਚਾਲਿਤ& ext_ਸ਼ੀਰਸ਼ = c.ਗ੍ਰਾਫ.vertices_[ext_ਸੂਚਕਾਂਕ];
    ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>* extension =
        ext_ਸ਼ੀਰਸ਼.as<ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
    extension->reset (type);

    // Make extension point at the subtable.
    TRY(ext_ਸ਼ੀਰਸ਼.ਜੋੜੋ_real_link (4, subtable_ਸੂਚਕਾਂਕ, 4));

    ਵਾਪਸ Ok(ext_ਸੂਚਕਾਂਕ);
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> make_subtable_extension (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                                ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ,
                                                ਬਿਨਾਨਿਸ਼ਾਨੀ subtable_ਸੂਚਕਾਂਕ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ type = lookupType;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ext_ਸੂਚਕਾਂਕ = HB_ਗ੍ਰਾਫ_INVALID;
    ਬਿਨਾਨਿਸ਼ਾਨੀ* existing_ext_ਸੂਚਕਾਂਕ = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (c.subtable_ਮੰਜ਼ਿਲ_extension.has(subtable_ਸੂਚਕਾਂਕ, &existing_ext_ਸੂਚਕਾਂਕ)) {
      ext_ਸੂਚਕਾਂਕ = *existing_ext_ਸੂਚਕਾਂਕ;
    } ਨਹੀਂਤਾਂ {
      TRY_ਸੌਂਪੋ (ext_ਸੂਚਕਾਂਕ, create_extension_subtable(c, subtable_ਸੂਚਕਾਂਕ, type));
      c.subtable_ਮੰਜ਼ਿਲ_extension.ਸੈੱਟ(subtable_ਸੂਚਕਾਂਕ, ext_ਸੂਚਕਾਂਕ);
      TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (c.subtable_ਮੰਜ਼ਿਲ_extension, ਵੰਡ_FAILURE));
    }

    ਸਵੈਚਾਲਿਤ& subtable_ਸ਼ੀਰਸ਼ = c.ਗ੍ਰਾਫ.vertices_[subtable_ਸੂਚਕਾਂਕ];
    ਸਵੈਚਾਲਿਤ& lookup_ਸ਼ੀਰਸ਼ = c.ਗ੍ਰਾਫ.vertices_[lookup_ਸੂਚਕਾਂਕ];
    ਬਿਨਾਨਿਸ਼ਾਨੀ replaced = 0;
    ਲਈ (ਸਵੈਚਾਲਿਤ& l : lookup_ਸ਼ੀਰਸ਼.real_links_writer ())
    {
      ਜੇ (l.objidx == subtable_ਸੂਚਕਾਂਕ) {
        // Change lookup to point at the extension.
        l.objidx = ext_ਸੂਚਕਾਂਕ;
        replaced++;
      }
    }

    // Make extension point at the subtable.
    ਸਵੈਚਾਲਿਤ& ext_ਸ਼ੀਰਸ਼ = c.ਗ੍ਰਾਫ.vertices_[ext_ਸੂਚਕਾਂਕ];
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < replaced; i++)
    {
      subtable_ਸ਼ੀਰਸ਼.remove_ਮਾਪਾ (lookup_ਸੂਚਕਾਂਕ);
      TRY(ext_ਸ਼ੀਰਸ਼.ਜੋੜੋ_ਮਾਪਾ (lookup_ਸੂਚਕਾਂਕ, ਝੂਠ));
    }
    ਜੇ (!existing_ext_ਸੂਚਕਾਂਕ)
      TRY(subtable_ਸ਼ੀਰਸ਼.ਜੋੜੋ_ਮਾਪਾ (ext_ਸੂਚਕਾਂਕ, ਝੂਠ));

    ਵਾਪਸ Ok();
  }

 ਨਿਜੀ:
  ਤਾਰਕਿਕ is_supported_gsub_type(ਬਿਨਾਨਿਸ਼ਾਨੀ type, gsubgpos_ਗ੍ਰਾਫ_context_t& c) ਅਚਲ {
    ਵਾਪਸ (c.ਸਾਰਣੀ_ਟੈਗ_2 == HB_OT_ਟੈਗ_GSUB) && (
      type == OT::Layout::GSUB_impl::Subst_Lookup_Sub_ਸਾਰਣੀ::Type::Ligature
    );
  }

  ਤਾਰਕਿਕ is_supported_gpos_type(ਬਿਨਾਨਿਸ਼ਾਨੀ type, gsubgpos_ਗ੍ਰਾਫ_context_t& c) ਅਚਲ {
   ਵਾਪਸ (c.ਸਾਰਣੀ_ਟੈਗ_2 == HB_OT_ਟੈਗ_GPOS) && (
      type == OT::Layout::GPOS_impl::Pos_Lookup_Sub_ਸਾਰਣੀ::Type::Pair ||
      type == OT::Layout::GPOS_impl::Pos_Lookup_Sub_ਸਾਰਣੀ::Type::Mark_ਅਧਾਰ
    );
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ extension_type (hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2) ਅਚਲ
  {
    ਚੋਣ (ਸਾਰਣੀ_ਟੈਗ_2)
    {
    ਹਾਲਤ HB_OT_ਟੈਗ_GPOS: ਵਾਪਸ 9;
    ਹਾਲਤ HB_OT_ਟੈਗ_GSUB: ਵਾਪਸ 7;
    ਮੂਲ: ਵਾਪਸ 0;
    }
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ LookupList : ਜਨਤਕ OT::LookupList<T>
{
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼) ਅਚਲ
  {
    size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ ();
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < OT::LookupList<T>::min_ਮਾਪ)) ਵਾਪਸ Err(SANITIZE_FAILURE);
    hb_barrier ();
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < OT::LookupList<T>::ਵਸਤੂ_ਮਾਪ * ਇਹ->len)) ਵਾਪਸ Err(SANITIZE_FAILURE);
    ਵਾਪਸ Ok();
  }
};

ਬਣਤਰ GSTAR : ਜਨਤਕ OT::GSUBGPOS
{
  ਸਥਿਰ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਅਚਲ GSTAR*> ਗ੍ਰਾਫ_ਮੰਜ਼ਿਲ_gstar (ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ)
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ& r = ਗ੍ਰਾਫ.ਜੜ੍ਹ ();

    ਅਚਲ GSTAR* gstar = (GSTAR*) r.obj ().ਸਿਰ_2;
    ਜੇ (unlikely (!gstar)) ਵਾਪਸ Err(INVALID_ਦਲੀਲ);
    TRY (gstar->sanitize (r));
    hb_barrier ();

    ਵਾਪਸ gstar;
  }

  ਅਚਲ ਖਾਲੀ* get_lookup_list_ਖੇਤਰ_ਵਿੱਥ () ਅਚਲ
  {
    ਵਾਪਸ OT::GSUBGPOS::get_lookup_list_ਖੇਤਰ_ਵਿੱਥ ();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼) ਅਚਲ
  {
    size_t len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ ();
    ਜੇ (unlikely (len < OT::GSUBGPOS::min_ਮਾਪ)) ਵਾਪਸ Err(SANITIZE_FAILURE);
    hb_barrier ();
    ਜੇ (unlikely (ਸੰਸਕਰਣ.major != 1)) ਵਾਪਸ Err(SANITIZE_FAILURE);
    ਜੇ (unlikely (len < get_ਮਾਪ ())) ਵਾਪਸ Err(SANITIZE_FAILURE);
    ਵਾਪਸ Ok();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਲੱਭੋ_lookups (ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ,
                                     hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, Lookup*>& lookups /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010002u && lookupList2)
      ਵਾਪਸ ਲੱਭੋ_lookups<MediumTypes> (ਗ੍ਰਾਫ, lookups);
#ਜੇ_ਅੰਤ
    TRY(ਲੱਭੋ_lookups<SmallTypes> (ਗ੍ਰਾਫ, lookups));
    ਵਾਪਸ Ok();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> get_lookup_list_ਸੂਚਕਾਂਕ (ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ) ਅਚਲ
  {
    ਅਚਲ ਖਾਲੀ* ਵਿੱਥ = get_lookup_list_ਖੇਤਰ_ਵਿੱਥ ();
    ਜੇ (unlikely (!ਵਿੱਥ)) ਵਾਪਸ Err(INVALID_ਦਲੀਲ);
    ਵਾਪਸ ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (ਗ੍ਰਾਫ.ਜੜ੍ਹ_idx (), ਵਿੱਥ);
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Types>
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਲੱਭੋ_lookups (ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ,
                                     hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, Lookup*>& lookups /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_list_idx, get_lookup_list_ਸੂਚਕਾਂਕ (ਗ੍ਰਾਫ));
    ਅਚਲ LookupList<Types>* lookupList =
        (ਅਚਲ LookupList<Types>*) ਗ੍ਰਾਫ.object (lookup_list_idx).ਸਿਰ_2;
    ਜੇ (unlikely (!lookupList)) ਵਾਪਸ Err(INVALID_ਦਲੀਲ);
    TRY(lookupList->sanitize (ਗ੍ਰਾਫ.vertices_[lookup_list_idx]));

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < lookupList->len; i++)
    {
      TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_idx, ਗ੍ਰਾਫ.ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (lookup_list_idx, &(lookupList->arrayZ[i])));
      Lookup* lookup = (Lookup*) ਗ੍ਰਾਫ.object (lookup_idx).ਸਿਰ_2;
      ਜੇ (!lookup || !lookup->sanitize (ਗ੍ਰਾਫ.vertices_[lookup_idx]).is_ok ()) ਜਾਰੀ;
      lookups.ਸੈੱਟ (lookup_idx, lookup);
    }
    ਵਾਪਸ Ok();
  }
};




}

#ਜੇ_ਅੰਤ  /* GRAPH_GSUBGPOS_GRAPH_HH */
