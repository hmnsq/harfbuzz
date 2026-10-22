#اگر_غیر_تعریف_شدہ OT_GLYF_GLYF_HELPERS_HH
#تعریف_کرو OT_GLYF_GLYF_HELPERS_HH


#شامل "../../hb-open-type.hh"
#شامل "../../hb-subset-plan.hh"

#شامل "loca.hh"


نامخلا OT {
نامخلا glyf_impl {


سانچہ<قسم_نام Iterator_اندر, قسم_نام Type_برآمد,
	 hb_requires (hb_is_ماخذ_کا (Iterator_اندر, غیرعلامتی صحیح))>
ساکن خالی
_write_loca (Iterator_اندر&& it,
	     ثابت hb_sorted_سمتیہ_t<hb_codepoint_pair_t> new_مقصد_old_gid_list,
	     منطقی short_offsets,
	     Type_برآمد *dest,
	     غیرعلامتی عدد_offsets)
{
  غیرعلامتی دایاں_سرکاؤ = short_offsets ? 1 : 0;
  غیرعلامتی انحراف = 0;
  Type_برآمد value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t آخری = 0;
  برائے (خودکار _ : new_مقصد_old_gid_list)
  {
    hb_codepoint_t gid = _.پہلا;
    برائے (; آخری < gid; آخری++)
    {
      DEBUG_پیغام (SUBSET, خالی_اشاریہ, "loca entry empty offset %u", انحراف);
      *dest++ = value;
    }

    غیرعلامتی padded_سائز = *it++;
    انحراف += padded_سائز;
    DEBUG_پیغام (SUBSET, خالی_اشاریہ, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, انحراف, padded_سائز);
    value = انحراف >> دایاں_سرکاؤ;
    *dest++ = value;

    آخری++; // Skip over gid
  }
  غیرعلامتی عدد_glyphs = عدد_offsets - 1;
  برائے (; آخری < عدد_glyphs; آخری++)
  {
    DEBUG_پیغام (SUBSET, خالی_اشاریہ, "loca entry empty offset %u", انحراف);
    *dest++ = value;
  }
}

ساکن منطقی
جوڑو_سر_and_سیٹ_loca_نسخہ (hb_subset_plan_t *plan, منطقی use_short_loca)
{
  hb_blob_t *سر_blob = hb_sanitize_context_t ().reference_جدول<سر_2> (plan->ماخذ);
  hb_blob_t *سر_مفرد_blob = hb_blob_copy_writable_or_fail (سر_blob);
  hb_blob_destroy (سر_blob);

  اگر (unlikely (!سر_مفرد_blob))
    واپس جھوٹ;

  سر_2 *سر_مفرد = (سر_2 *) hb_blob_get_ڈیٹا_writable (سر_مفرد_blob, خالی_اشاریہ);
  سر_مفرد->اشاریہ_مقصد_Loc_Format = use_short_loca ? 0 : 1;
  اگر (plan->normalized_coords)
  {
    سر_مفرد->xMin = plan->سر_maxp_info.xMin;
    سر_مفرد->xMax = plan->سر_maxp_info.xMax;
    سر_مفرد->yMin = plan->سر_maxp_info.yMin;
    سر_مفرد->yMax = plan->سر_maxp_info.yMax;

    غیرعلامتی orig_flag = سر_مفرد->flags;
    اگر (plan->سر_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    ورنہ
      orig_flag &= ~(1 << 1);
    سر_مفرد->flags = orig_flag;
  }
  منطقی success = plan->جوڑو_جدول (HB_OT_لیبل_سر, سر_مفرد_blob);

  hb_blob_destroy (سر_مفرد_blob);
  واپس success;
}

سانچہ<قسم_نام Iterator,
	 hb_requires (hb_is_ماخذ_کا (Iterator, غیرعلامتی صحیح))>
ساکن منطقی
جوڑو_loca_and_سر (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    منطقی use_short_loca,
		    منطقی extended)
{
  غیرعلامتی عدد_offsets = c->plan->عدد_output_glyphs () + 1;
  غیرعلامتی اندراج_سائز = use_short_loca ? 2 : 4;

  حرف *loca_مفرد_ڈیٹا = (حرف *) hb_malloc2 (عدد_offsets, اندراج_سائز);

  اگر (unlikely (!loca_مفرد_ڈیٹا)) واپس جھوٹ;

  DEBUG_پیغام (SUBSET, خالی_اشاریہ, "loca entry_size %u num_offsets %u size %u",
	     اندراج_سائز, عدد_offsets, اندراج_سائز * عدد_offsets);

  اگر (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_مقصد_old_gid_list, سچ, (HBUINT16 *) loca_مفرد_ڈیٹا, عدد_offsets);
  ورنہ
    _write_loca (padded_offsets, c->plan->new_مقصد_old_gid_list, جھوٹ, (HBUINT32 *) loca_مفرد_ڈیٹا, عدد_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_مفرد_ڈیٹا,
					 اندراج_سائز * عدد_offsets,
					 HB_حافظہ_منوال_WRITABLE,
					 loca_مفرد_ڈیٹا,
					 hb_free);

  منطقی نتیجہ = c->plan->جوڑو_جدول (extended ? HB_OT_لیبل_LOCA : HB_OT_لیبل_loca, loca_blob)
	     && جوڑو_سر_and_سیٹ_loca_نسخہ (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  واپس نتیجہ;
}


} /* namespace glyf_impl */
} /* namespace OT */


#اگر_ختم /* OT_GLYF_GLYF_HELPERS_HH */
