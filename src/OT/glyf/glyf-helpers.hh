#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_GLYF_GLYF_HELPERS_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_GLYF_GLYF_HELPERS_HH


#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-subset-plan.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "loca.hh"


ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ glyf_impl {


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator_ਵਿੱਚ, ਕਿਸਮ_ਨਾਂ Type_ਬਾਹਰਲਾ,
	 hb_requires (hb_is_ਸਰੋਤ_ਦਾ (Iterator_ਵਿੱਚ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ))>
ਸਥਿਰ ਖਾਲੀ
_write_loca (Iterator_ਵਿੱਚ&& it,
	     ਅਚਲ hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_pair_t> new_ਮੰਜ਼ਿਲ_old_gid_list,
	     ਤਾਰਕਿਕ short_offsets,
	     Type_ਬਾਹਰਲਾ *dest,
	     ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_offsets)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੱਜਾ_ਖਿਸਕਾਓ = short_offsets ? 1 : 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ = 0;
  Type_ਬਾਹਰਲਾ value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t ਆਖਰੀ = 0;
  ਲਈ (ਸਵੈਚਾਲਿਤ _ : new_ਮੰਜ਼ਿਲ_old_gid_list)
  {
    hb_codepoint_t gid = _.ਪਹਿਲਾ;
    ਲਈ (; ਆਖਰੀ < gid; ਆਖਰੀ++)
    {
      DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "loca entry empty offset %u", ਵਿੱਥ);
      *dest++ = value;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ padded_ਮਾਪ = *it++;
    ਵਿੱਥ += padded_ਮਾਪ;
    DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, ਵਿੱਥ, padded_ਮਾਪ);
    value = ਵਿੱਥ >> ਸੱਜਾ_ਖਿਸਕਾਓ;
    *dest++ = value;

    ਆਖਰੀ++; // Skip over gid
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs = ਅੰਕ_offsets - 1;
  ਲਈ (; ਆਖਰੀ < ਅੰਕ_glyphs; ਆਖਰੀ++)
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "loca entry empty offset %u", ਵਿੱਥ);
    *dest++ = value;
  }
}

ਸਥਿਰ ਤਾਰਕਿਕ
ਜੋੜੋ_ਸਿਰ_and_ਸੈੱਟ_loca_ਸੰਸਕਰਣ (hb_subset_plan_t *plan, ਤਾਰਕਿਕ use_short_loca)
{
  hb_blob_t *ਸਿਰ_blob = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<ਸਿਰ_2> (plan->ਸਰੋਤ);
  hb_blob_t *ਸਿਰ_ਅਭਾਜ_blob = hb_blob_copy_writable_or_fail (ਸਿਰ_blob);
  hb_blob_destroy (ਸਿਰ_blob);

  ਜੇ (unlikely (!ਸਿਰ_ਅਭਾਜ_blob))
    ਵਾਪਸ ਝੂਠ;

  ਸਿਰ_2 *ਸਿਰ_ਅਭਾਜ = (ਸਿਰ_2 *) hb_blob_get_ਅੰਕੜੇ_writable (ਸਿਰ_ਅਭਾਜ_blob, ਖਾਲੀ_ਸੂਚਕ);
  ਸਿਰ_ਅਭਾਜ->ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Loc_Format = use_short_loca ? 0 : 1;
  ਜੇ (plan->normalized_coords)
  {
    ਸਿਰ_ਅਭਾਜ->xMin = plan->ਸਿਰ_maxp_info.xMin;
    ਸਿਰ_ਅਭਾਜ->xMax = plan->ਸਿਰ_maxp_info.xMax;
    ਸਿਰ_ਅਭਾਜ->yMin = plan->ਸਿਰ_maxp_info.yMin;
    ਸਿਰ_ਅਭਾਜ->yMax = plan->ਸਿਰ_maxp_info.yMax;

    ਬਿਨਾਨਿਸ਼ਾਨੀ orig_flag = ਸਿਰ_ਅਭਾਜ->flags;
    ਜੇ (plan->ਸਿਰ_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    ਨਹੀਂਤਾਂ
      orig_flag &= ~(1 << 1);
    ਸਿਰ_ਅਭਾਜ->flags = orig_flag;
  }
  ਤਾਰਕਿਕ success = plan->ਜੋੜੋ_ਸਾਰਣੀ (HB_OT_ਟੈਗ_ਸਿਰ, ਸਿਰ_ਅਭਾਜ_blob);

  hb_blob_destroy (ਸਿਰ_ਅਭਾਜ_blob);
  ਵਾਪਸ success;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
	 hb_requires (hb_is_ਸਰੋਤ_ਦਾ (Iterator, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ))>
ਸਥਿਰ ਤਾਰਕਿਕ
ਜੋੜੋ_loca_and_ਸਿਰ (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    ਤਾਰਕਿਕ use_short_loca,
		    ਤਾਰਕਿਕ extended)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_offsets = c->plan->ਅੰਕ_output_glyphs () + 1;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਇੰਦਰਾਜ_ਮਾਪ = use_short_loca ? 2 : 4;

  ਅੱਖਰ *loca_ਅਭਾਜ_ਅੰਕੜੇ = (ਅੱਖਰ *) hb_malloc2 (ਅੰਕ_offsets, ਇੰਦਰਾਜ_ਮਾਪ);

  ਜੇ (unlikely (!loca_ਅਭਾਜ_ਅੰਕੜੇ)) ਵਾਪਸ ਝੂਠ;

  DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "loca entry_size %u num_offsets %u size %u",
	     ਇੰਦਰਾਜ_ਮਾਪ, ਅੰਕ_offsets, ਇੰਦਰਾਜ_ਮਾਪ * ਅੰਕ_offsets);

  ਜੇ (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_ਮੰਜ਼ਿਲ_old_gid_list, ਸੱਚ, (HBUINT16 *) loca_ਅਭਾਜ_ਅੰਕੜੇ, ਅੰਕ_offsets);
  ਨਹੀਂਤਾਂ
    _write_loca (padded_offsets, c->plan->new_ਮੰਜ਼ਿਲ_old_gid_list, ਝੂਠ, (HBUINT32 *) loca_ਅਭਾਜ_ਅੰਕੜੇ, ਅੰਕ_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_ਅਭਾਜ_ਅੰਕੜੇ,
					 ਇੰਦਰਾਜ_ਮਾਪ * ਅੰਕ_offsets,
					 HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE,
					 loca_ਅਭਾਜ_ਅੰਕੜੇ,
					 hb_free);

  ਤਾਰਕਿਕ ਨਤੀਜਾ = c->plan->ਜੋੜੋ_ਸਾਰਣੀ (extended ? HB_OT_ਟੈਗ_LOCA : HB_OT_ਟੈਗ_loca, loca_blob)
	     && ਜੋੜੋ_ਸਿਰ_and_ਸੈੱਟ_loca_ਸੰਸਕਰਣ (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  ਵਾਪਸ ਨਤੀਜਾ;
}


} /* namespace glyf_impl */
} /* namespace OT */


#ਜੇ_ਅੰਤ /* OT_GLYF_GLYF_HELPERS_HH */
