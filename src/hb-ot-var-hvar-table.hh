/*
 * Copyright © 2017  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_VAR_HVAR_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_VAR_HVAR_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-common.hh"

ਨਾਮਾਕਾਸ਼ OT {


ਬਣਤਰ ਸੂਚਕਾਂਕ_map_subset_plan_t
{
  ਗਿਣਤੀ ਸੂਚਕਾਂਕ_map_ਸੂਚਕਾਂਕ_t {
    ADV_ਸੂਚਕਾਂਕ,
    LSB_ਸੂਚਕਾਂਕ,	/* dual as TSB */
    RSB_ਸੂਚਕਾਂਕ,	/* dual as BSB */
    VORG_ਸੂਚਕਾਂਕ
  };

  ਖਾਲੀ init (ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map  *ਸੂਚਕਾਂਕ_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_ਸਦਿਸ਼_t<hb_ਸੈੱਟ_t *> &inner_sets,
	     ਅਚਲ hb_subset_plan_t  *plan,
	     ਤਾਰਕਿਕ bypass_ਖਾਲੀ_ਥਾਵਾਂ = ਸੱਚ)
  {
    map_ਗਿਣਤੀ = 0;
    ਭਰਿਆ_map_ਗਿਣਤੀ = plan->new_ਮੰਜ਼ਿਲ_old_gid_list.ਲੰਮਾਈ ?
		     plan->new_ਮੰਜ਼ਿਲ_old_gid_list.ਪੂਛ ().ਪਹਿਲਾ + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_ਮੰਜ਼ਿਲ_implicit = !bypass_ਖਾਲੀ_ਥਾਵਾਂ && !ਸੂਚਕਾਂਕ_map;
    max_inners.init ();
    output_map.init ();

    ਜੇ (bypass_ਖਾਲੀ_ਥਾਵਾਂ && (!ਸੂਚਕਾਂਕ_map || !ਸੂਚਕਾਂਕ_map->get_map_ਗਿਣਤੀ ())) ਵਾਪਸ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	ਆਖਰੀ_val = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)-1;
    hb_codepoint_t	ਆਖਰੀ_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.ਲੰਮਾਈ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < inner_sets.ਲੰਮਾਈ; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    ਸਵੈਚਾਲਿਤ &new_ਮੰਜ਼ਿਲ_old_gid_list = plan->new_ਮੰਜ਼ਿਲ_old_gid_list;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = new_ਮੰਜ਼ਿਲ_old_gid_list.ਲੰਮਾਈ;
    ਜੇ (!ਸੂਚਕਾਂਕ_map)
    {
      map_ਗਿਣਤੀ = new_ਮੰਜ਼ਿਲ_old_gid_list.ਪੂਛ ().ਪਹਿਲਾ + 1;
    }
    ਨਹੀਂਤਾਂ
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = ਗਿਣਤੀ_2; j; j--)
      {
        hb_codepoint_t gid = new_ਮੰਜ਼ਿਲ_old_gid_list.arrayZ[j - 1].ਪਹਿਲਾ;
        hb_codepoint_t old_gid = new_ਮੰਜ਼ਿਲ_old_gid_list.arrayZ[j - 1].second;
  
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ v = ਸੂਚਕਾਂਕ_map->map (old_gid);
        ਜੇ (ਆਖਰੀ_gid == HB_CODEPOINT_INVALID)
        {
  	ਆਖਰੀ_val = v;
  	ਆਖਰੀ_gid = gid;
  	ਜਾਰੀ;
        }
        ਜੇ (v != ਆਖਰੀ_val)
  	ਰੋਕੋ;
  
        ਆਖਰੀ_gid = gid;
      }
  
      ਜੇ (unlikely (ਆਖਰੀ_gid == (hb_codepoint_t)-1)) ਵਾਪਸ;
      map_ਗਿਣਤੀ = ਆਖਰੀ_gid + 1;
    }

    ਲਈ (ਸਵੈਚਾਲਿਤ _ : plan->new_ਮੰਜ਼ਿਲ_old_gid_list)
    {
      hb_codepoint_t gid = _.ਪਹਿਲਾ;
      ਜੇ (gid >= map_ਗਿਣਤੀ) ਰੋਕੋ;

      hb_codepoint_t old_gid = _.second;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ v = ਸੂਚਕਾਂਕ_map ? ਸੂਚਕਾਂਕ_map->map (old_gid): old_gid;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ outer = v >> 16;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inner = v & 0xFFFF;
      outer_map.ਜੋੜੋ (outer);
      ਜੇ (inner > max_inners[outer]) max_inners[outer] = inner;
      ਜੇ (outer >= inner_sets.ਲੰਮਾਈ) ਵਾਪਸ;
      inner_sets[outer]->ਜੋੜੋ (inner);
    }
  }

  ਖਾਲੀ fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  ਖਾਲੀ remap (ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map *ਨਿਵੇਸ਼_map,
	      ਅਚਲ hb_inc_bimap_t &outer_map,
	      ਅਚਲ hb_ਸਦਿਸ਼_t<hb_inc_bimap_t> &inner_maps,
	      ਅਚਲ hb_subset_plan_t *plan)
  {
    outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = 1;
    inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = 1;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < max_inners.ਲੰਮਾਈ; i++)
    {
      ਜੇ (inner_maps[i].get_population () == 0) ਜਾਰੀ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = (max_inners[i]==0)? 1: hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (inner_maps[i][max_inners[i]]);
      ਜੇ (ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ > inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ) inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ;
    }

    ਜੇ (unlikely (!output_map.resize (map_ਗਿਣਤੀ))) ਵਾਪਸ;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &_ : plan->new_ਮੰਜ਼ਿਲ_old_gid_list)
    {
      hb_codepoint_t new_gid = _.ਪਹਿਲਾ;
      hb_codepoint_t old_gid = _.second;

      ਜੇ (unlikely (new_gid >= map_ਗਿਣਤੀ)) ਰੋਕੋ;

      uint32_t v = ਨਿਵੇਸ਼_map? ਨਿਵੇਸ਼_map->map (old_gid) : old_gid;
      ਬਿਨਾਨਿਸ਼ਾਨੀ outer = v >> 16;
      ਬਿਨਾਨਿਸ਼ਾਨੀ new_outer = outer_map[outer];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = (new_outer == 0) ? 1 : hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (new_outer);
      outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = hb_max (ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ, outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  ਤਾਰਕਿਕ remap_after_instantiation (ਅਚਲ hb_subset_plan_t *plan,
                                  ਅਚਲ hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = 1;
    inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = 1;

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &_ : plan->new_ਮੰਜ਼ਿਲ_old_gid_list)
    {
      hb_codepoint_t new_gid = _.ਪਹਿਲਾ;
      ਜੇ (unlikely (new_gid >= map_ਗਿਣਤੀ)) ਰੋਕੋ;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      ਜੇ (!varidx_map.has (v, &new_varidx))
        ਵਾਪਸ ਝੂਠ;

      output_map.arrayZ[new_gid] = *new_varidx;

      ਬਿਨਾਨਿਸ਼ਾਨੀ outer = (*new_varidx) >> 16;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = (outer == 0) ? 1 : hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (outer);
      outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = hb_max (ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ, outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ);

      ਬਿਨਾਨਿਸ਼ਾਨੀ inner = (*new_varidx) & 0xFFFF;
      ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = (inner == 0) ? 1 : hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (inner);
      inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = hb_max (ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ, inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ);
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਚੌੜਾਈ ()           ਅਚਲ { ਵਾਪਸ ((outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ + inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ + 7) / 8); }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_map_ਗਿਣਤੀ ()       ਅਚਲ { ਵਾਪਸ map_ਗਿਣਤੀ; }

  size_t get_ਮਾਪ () ਅਚਲ
  {
    ਜੇ (!map_ਗਿਣਤੀ) ਵਾਪਸ 0;
    ਵਾਪਸ hb_unsigned_mul_ਜੋੜੋ_saturate (get_ਚੌੜਾਈ (), map_ਗਿਣਤੀ,
					 Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map::min_ਮਾਪ);
  }

  ਤਾਰਕਿਕ is_ਪਛਾਣ_ਤੱਤ () ਅਚਲ
  {
    ਜੇ (!output_map) ਵਾਪਸ ਸੱਚ;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    ਜੇ (!can_drop_ਮੰਜ਼ਿਲ_implicit || map_ਗਿਣਤੀ != ਭਰਿਆ_map_ਗਿਣਤੀ) ਵਾਪਸ ਝੂਠ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < map_ਗਿਣਤੀ; i++)
      ਜੇ (output_map.arrayZ[i] != i) ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
  }
  hb_array_t<ਅਚਲ uint32_t> get_output_map () ਅਚਲ { ਵਾਪਸ output_map.as_array (); }

  ਸੁਰੱਖਿਅਤ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ map_ਗਿਣਤੀ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਭਰਿਆ_map_ਗਿਣਤੀ;
  ਤਾਰਕਿਕ can_drop_ਮੰਜ਼ਿਲ_implicit;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> max_inners;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ;
  hb_ਸਦਿਸ਼_t<uint32_t> output_map;
};

ਬਣਤਰ hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), ਸੂਚਕਾਂਕ_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  ਖਾਲੀ init (ਅਚਲ hb_array_t<ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map *> &ਸੂਚਕਾਂਕ_maps,
	     ਅਚਲ ਵਸਤੂ_Variation_Store &_var_store,
	     ਅਚਲ hb_subset_plan_t *plan)
  {
    ਸੂਚਕਾਂਕ_map_plans.resize (ਸੂਚਕਾਂਕ_maps.ਲੰਮਾਈ);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_ਸਾਰਣੀ_ਗਿਣਤੀ ());
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < inner_sets.ਲੰਮਾਈ; i++)
      inner_sets[i] = hb_ਸੈੱਟ_create ();
    adv_ਸੈੱਟ = hb_ਸੈੱਟ_create ();

    inner_maps.resize (var_store->get_sub_ਸਾਰਣੀ_ਗਿਣਤੀ ());

    ਜੇ (unlikely (!ਸੂਚਕਾਂਕ_map_plans.ਲੰਮਾਈ || !inner_sets.ਲੰਮਾਈ || !inner_maps.ਲੰਮਾਈ)) ਵਾਪਸ;

    ਤਾਰਕਿਕ retain_adv_map = ਝੂਠ;
    ਸੂਚਕਾਂਕ_map_plans[0].init (ਸੂਚਕਾਂਕ_maps[0], outer_map, inner_sets, plan, ਝੂਠ);
    ਜੇ (!ਸੂਚਕਾਂਕ_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.ਜੋੜੋ (0);
      ਲਈ (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->ਜੋੜੋ (old_gid);
      hb_ਸੈੱਟ_union (adv_ਸੈੱਟ, inner_sets[0]);
    }

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ਸੂਚਕਾਂਕ_maps.ਲੰਮਾਈ; i++)
      ਸੂਚਕਾਂਕ_map_plans[i].init (ਸੂਚਕਾਂਕ_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    ਜੇ (retain_adv_map)
    {
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &_ : plan->new_ਮੰਜ਼ਿਲ_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].ਜੋੜੋ (old_gid);
      }
    }
    ਨਹੀਂਤਾਂ
    {
      inner_maps[0].ਜੋੜੋ_ਸੈੱਟ (adv_ਸੈੱਟ);
      hb_ਸੈੱਟ_ਘਟਾਓ (inner_sets[0], adv_ਸੈੱਟ);
      inner_maps[0].ਜੋੜੋ_ਸੈੱਟ (inner_sets[0]);
    }

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < inner_maps.ਲੰਮਾਈ; i++)
      inner_maps[i].ਜੋੜੋ_ਸੈੱਟ (inner_sets[i]);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਸੂਚਕਾਂਕ_maps.ਲੰਮਾਈ; i++)
      ਸੂਚਕਾਂਕ_map_plans[i].remap (ਸੂਚਕਾਂਕ_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  ਤਾਰਕਿਕ remap_ਸੂਚਕਾਂਕ_map_plans (ਅਚਲ hb_subset_plan_t *plan,
                              ਅਚਲ hb_map_t& varidx_map)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਸੂਚਕਾਂਕ_map_plans.ਲੰਮਾਈ; i++)
      ਜੇ (!ਸੂਚਕਾਂਕ_map_plans[i].remap_after_instantiation (plan, varidx_map))
        ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
  }

  ਖਾਲੀ fini ()
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < inner_sets.ਲੰਮਾਈ; i++)
      hb_ਸੈੱਟ_destroy (inner_sets[i]);
    hb_ਸੈੱਟ_destroy (adv_ਸੈੱਟ);
    inner_maps.fini ();
    ਸੂਚਕਾਂਕ_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_ਸਦਿਸ਼_t<hb_inc_bimap_t> inner_maps;
  hb_ਸਦਿਸ਼_t<ਸੂਚਕਾਂਕ_map_subset_plan_t> ਸੂਚਕਾਂਕ_map_plans;
  ਅਚਲ ਵਸਤੂ_Variation_Store *var_store;

  ਸੁਰੱਖਿਅਤ:
  hb_ਸਦਿਸ਼_t<hb_ਸੈੱਟ_t *> inner_sets;
  hb_ਸੈੱਟ_t *adv_ਸੈੱਟ;
};

/*
 * HVAR -- Horizontal Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hvar
 * VVAR -- Vertical Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vvar
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_HVAR HB_ਟੈਗ('H','V','A','R')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_VVAR HB_ਟੈਗ('V','V','A','R')

ਬਣਤਰ HVARVVAR
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t HVARTag = HB_OT_ਟੈਗ_HVAR;
  ਸਥਿਰ constexpr hb_ਟੈਗ_t VVARTag = HB_OT_ਟੈਗ_VVAR;

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ.major != 0; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ਸੰਸਕਰਣ.sanitize (c) &&
		  hb_barrier () &&
		  likely (ਸੰਸਕਰਣ.major == 1) &&
		  varStore.sanitize (c, ਇਹ) &&
		  advMap.sanitize (c, ਇਹ) &&
		  lsbMap.sanitize (c, ਇਹ) &&
		  rsbMap.sanitize (c, ਇਹ));
  }

  ਅਚਲ ਵਸਤੂ_Variation_Store& get_var_store () ਅਚਲ
  { ਵਾਪਸ ਇਹ+varStore; }

  ਖਾਲੀ listup_ਸੂਚਕਾਂਕ_maps (hb_ਸਦਿਸ਼_t<ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map *> &ਸੂਚਕਾਂਕ_maps) ਅਚਲ
  {
    ਜੇ (advMap) ਸੂਚਕਾਂਕ_maps.push (&(ਇਹ+advMap));
    ਨਹੀਂਤਾਂ ਸੂਚਕਾਂਕ_maps.push (ਖਾਲੀ_ਸੂਚਕ);

    ਜੇ (lsbMap) ਸੂਚਕਾਂਕ_maps.push (&(ਇਹ+lsbMap));
    ਨਹੀਂਤਾਂ ਸੂਚਕਾਂਕ_maps.push (ਖਾਲੀ_ਸੂਚਕ);

    ਜੇ (rsbMap) ਸੂਚਕਾਂਕ_maps.push (&(ਇਹ+rsbMap));
    ਨਹੀਂਤਾਂ ਸੂਚਕਾਂਕ_maps.push (ਖਾਲੀ_ਸੂਚਕ);
  }

  ਤਾਰਕਿਕ serialize_ਸੂਚਕਾਂਕ_maps (hb_serialize_context_t *c,
			     ਅਚਲ hb_array_t<ਸੂਚਕਾਂਕ_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (im_plans[ਸੂਚਕਾਂਕ_map_subset_plan_t::ADV_ਸੂਚਕਾਂਕ].is_ਪਛਾਣ_ਤੱਤ ())
      advMap = 0;
    ਨਹੀਂਤਾਂ ਜੇ (unlikely (!advMap.serialize_serialize (c, im_plans[ਸੂਚਕਾਂਕ_map_subset_plan_t::ADV_ਸੂਚਕਾਂਕ])))
      return_trace (ਝੂਠ);
    ਜੇ (im_plans[ਸੂਚਕਾਂਕ_map_subset_plan_t::LSB_ਸੂਚਕਾਂਕ].is_ਪਛਾਣ_ਤੱਤ ())
      lsbMap = 0;
    ਨਹੀਂਤਾਂ ਜੇ (unlikely (!lsbMap.serialize_serialize (c, im_plans[ਸੂਚਕਾਂਕ_map_subset_plan_t::LSB_ਸੂਚਕਾਂਕ])))
      return_trace (ਝੂਠ);
    ਜੇ (im_plans[ਸੂਚਕਾਂਕ_map_subset_plan_t::RSB_ਸੂਚਕਾਂਕ].is_ਪਛਾਣ_ਤੱਤ ())
      rsbMap = 0;
    ਨਹੀਂਤਾਂ ਜੇ (unlikely (!rsbMap.serialize_serialize (c, im_plans[ਸੂਚਕਾਂਕ_map_subset_plan_t::RSB_ਸੂਚਕਾਂਕ])))
      return_trace (ਝੂਠ);

    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ _subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (c->plan->all_axes_pinned)
      return_trace (ਝੂਠ);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_ਸਦਿਸ਼_t<ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map *>
				ਸੂਚਕਾਂਕ_maps;

    ((T*)ਇਹ)->listup_ਸੂਚਕਾਂਕ_maps (ਸੂਚਕਾਂਕ_maps);
    hvar_plan.init (ਸੂਚਕਾਂਕ_maps.as_array (), ਇਹ+varStore, c->plan);

    T *ਬਾਹਰਲਾ = c->serializer->allocate_min<T> ();
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.major = 1;
    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.minor = 0;

    ਜੇ (c->plan->normalized_coords)
    {
      ਵਸਤੂ_variations_t ਵਸਤੂ_vars;
      ਜੇ (!ਵਸਤੂ_vars.instantiate (ਇਹ+varStore, c->plan,
                                  advMap == 0 ? ਝੂਠ : ਸੱਚ,
                                  ਝੂਠ, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (ਝੂਠ);

      ਜੇ (!ਬਾਹਰਲਾ->varStore.serialize_serialize (c->serializer,
                                              ਵਸਤੂ_vars.has_long_ਸ਼ਬਦ (),
                                              c->plan->axis_ਟੈਗ_ਸੂਚੀ,
                                              ਵਸਤੂ_vars.get_region_list (),
                                              ਵਸਤੂ_vars.get_vardata_encodings ()))
        return_trace (ਝੂਠ);

      /* if varstore is optimized, remap output_map */
      ਜੇ (advMap)
      {
        ਜੇ (!hvar_plan.remap_ਸੂਚਕਾਂਕ_map_plans (c->plan, ਵਸਤੂ_vars.get_varidx_map ()))
          return_trace (ਝੂਠ);
      }
    }
    ਨਹੀਂਤਾਂ
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_ਸੈੱਟ_t dead_regions;
      ਜੇ (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      ਜੇ (unlikely (!ਬਾਹਰਲਾ->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : ਖਾਲੀ_ਸੂਚਕ)))
      return_trace (ਝੂਠ);
    }

    return_trace (ਬਾਹਰਲਾ->T::serialize_ਸੂਚਕਾਂਕ_maps (c->serializer,
						hvar_plan.ਸੂਚਕਾਂਕ_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  ਦਸ਼ਮਲਵ get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_ਗਿਣਤੀ,
				    hb_scalar_cache_t *store_cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    ਜੇ (!advMap && glyph > 0xFFFFu) ਵਾਪਸ 0.f;
    uint32_t varidx = (ਇਹ+advMap).map (glyph);
    ਵਾਪਸ (ਇਹ+varStore).get_delta (varidx,
				      coords, coord_ਗਿਣਤੀ,
				      store_cache);
  }

  ਜਨਤਕ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  Offset32_ਮੰਜ਼ਿਲ<ਵਸਤੂ_Variation_Store>
		varStore;	/* Offset to item variation store table. */
  Offset32_ਮੰਜ਼ਿਲ<Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map>
		advMap;		/* Offset to advance var-idx mapping. */
  Offset32_ਮੰਜ਼ਿਲ<Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map>
		lsbMap;		/* Offset to lsb/tsb var-idx mapping. */
  Offset32_ਮੰਜ਼ਿਲ<Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map>
		rsbMap;		/* Offset to rsb/bsb var-idx mapping. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (20);
};

ਬਣਤਰ HVAR : HVARVVAR {
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_HVAR;
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ { ਵਾਪਸ HVARVVAR::_subset<HVAR> (c); }
};
ਬਣਤਰ VVAR : HVARVVAR {
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_VVAR;

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (static_cast<ਅਚਲ HVARVVAR *> (ਇਹ)->sanitize (c) &&
		  vorgMap.sanitize (c, ਇਹ));
  }

  ਖਾਲੀ listup_ਸੂਚਕਾਂਕ_maps (hb_ਸਦਿਸ਼_t<ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map *> &ਸੂਚਕਾਂਕ_maps) ਅਚਲ
  {
    HVARVVAR::listup_ਸੂਚਕਾਂਕ_maps (ਸੂਚਕਾਂਕ_maps);
    ਜੇ (vorgMap) ਸੂਚਕਾਂਕ_maps.push (&(ਇਹ+vorgMap));
    ਨਹੀਂਤਾਂ ਸੂਚਕਾਂਕ_maps.push (ਖਾਲੀ_ਸੂਚਕ);
  }

  ਤਾਰਕਿਕ serialize_ਸੂਚਕਾਂਕ_maps (hb_serialize_context_t *c,
			     ਅਚਲ hb_array_t<ਸੂਚਕਾਂਕ_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!HVARVVAR::serialize_ਸੂਚਕਾਂਕ_maps (c, im_plans)))
      return_trace (ਝੂਠ);
    ਜੇ (!im_plans[ਸੂਚਕਾਂਕ_map_subset_plan_t::VORG_ਸੂਚਕਾਂਕ].get_map_ਗਿਣਤੀ ())
      vorgMap = 0;
    ਨਹੀਂਤਾਂ ਜੇ (unlikely (!vorgMap.serialize_serialize (c, im_plans[ਸੂਚਕਾਂਕ_map_subset_plan_t::VORG_ਸੂਚਕਾਂਕ])))
      return_trace (ਝੂਠ);

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ { ਵਾਪਸ HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  ਦਸ਼ਮਲਵ get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_ਗਿਣਤੀ,
				 hb_scalar_cache_t *store_cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜੇ (!vorgMap) ਵਾਪਸ 0.f;
    uint32_t varidx = (ਇਹ+vorgMap).map (glyph);
    ਵਾਪਸ (ਇਹ+varStore).get_delta (varidx,
				      coords, coord_ਗਿਣਤੀ,
				      store_cache);
  }

  ਸੁਰੱਖਿਅਤ:
  Offset32_ਮੰਜ਼ਿਲ<Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (24);
};

} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_VAR_HVAR_TABLE_HH */
