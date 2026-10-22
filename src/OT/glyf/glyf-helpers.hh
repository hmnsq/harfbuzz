#جے_بنا_تعریف OT_GLYF_GLYF_HELPERS_HH
#تعریف_کرو OT_GLYF_GLYF_HELPERS_HH


#ضمن "../../hb-open-type.hh"
#ضمن "../../hb-subset-plan.hh"

#ضمن "loca.hh"


نطاق OT {
نطاق glyf_impl {


سانچا<قسم_ناں Iterator_وچ, قسم_ناں Type_باہرلا,
	 hb_requires (hb_is_سروت_دا (Iterator_وچ, بنانشانی صحيح))>
ساكن فراغ
_write_loca (Iterator_وچ&& it,
	     ثابت hb_sorted_سمتیہ_t<hb_codepoint_pair_t> new_منزل_old_gid_list,
	     منطقي short_offsets,
	     Type_باہرلا *dest,
	     بنانشانی عدد_offsets)
{
  بنانشانی دایاں_کھسکاؤ = short_offsets ? 1 : 0;
  بنانشانی وتھ = 0;
  Type_باہرلا value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t آخری = 0;
  لكل (خودکار _ : new_منزل_old_gid_list)
  {
    hb_codepoint_t gid = _.پہلا;
    لكل (; آخری < gid; آخری++)
    {
      DEBUG_پیغام (SUBSET, خالی_سوچک, "loca entry empty offset %u", وتھ);
      *dest++ = value;
    }

    بنانشانی padded_ناپ = *it++;
    وتھ += padded_ناپ;
    DEBUG_پیغام (SUBSET, خالی_سوچک, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, وتھ, padded_ناپ);
    value = وتھ >> دایاں_کھسکاؤ;
    *dest++ = value;

    آخری++; // Skip over gid
  }
  بنانشانی عدد_glyphs = عدد_offsets - 1;
  لكل (; آخری < عدد_glyphs; آخری++)
  {
    DEBUG_پیغام (SUBSET, خالی_سوچک, "loca entry empty offset %u", وتھ);
    *dest++ = value;
  }
}

ساكن منطقي
جوڑو_سر_and_سیٹ_loca_نسخہ (hb_subset_plan_t *plan, منطقي use_short_loca)
{
  hb_blob_t *سر_blob = hb_sanitize_context_t ().reference_جدول<سر_2> (plan->سروت);
  hb_blob_t *سر_مفرد_blob = hb_blob_copy_writable_or_fail (سر_blob);
  hb_blob_destroy (سر_blob);

  إذا (unlikely (!سر_مفرد_blob))
    أرجع خطأ;

  سر_2 *سر_مفرد = (سر_2 *) hb_blob_get_انکڑے_writable (سر_مفرد_blob, خالی_سوچک);
  سر_مفرد->اشاریہ_منزل_Loc_Format = use_short_loca ? 0 : 1;
  إذا (plan->normalized_coords)
  {
    سر_مفرد->xMin = plan->سر_maxp_info.xMin;
    سر_مفرد->xMax = plan->سر_maxp_info.xMax;
    سر_مفرد->yMin = plan->سر_maxp_info.yMin;
    سر_مفرد->yMax = plan->سر_maxp_info.yMax;

    بنانشانی orig_flag = سر_مفرد->flags;
    إذا (plan->سر_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    وإلا
      orig_flag &= ~(1 << 1);
    سر_مفرد->flags = orig_flag;
  }
  منطقي success = plan->جوڑو_جدول (HB_OT_ٹیگ_سر, سر_مفرد_blob);

  hb_blob_destroy (سر_مفرد_blob);
  أرجع success;
}

سانچا<قسم_ناں Iterator,
	 hb_requires (hb_is_سروت_دا (Iterator, بنانشانی صحيح))>
ساكن منطقي
جوڑو_loca_and_سر (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    منطقي use_short_loca,
		    منطقي extended)
{
  بنانشانی عدد_offsets = c->plan->عدد_output_glyphs () + 1;
  بنانشانی اندراج_ناپ = use_short_loca ? 2 : 4;

  حرف *loca_مفرد_انکڑے = (حرف *) hb_malloc2 (عدد_offsets, اندراج_ناپ);

  إذا (unlikely (!loca_مفرد_انکڑے)) أرجع خطأ;

  DEBUG_پیغام (SUBSET, خالی_سوچک, "loca entry_size %u num_offsets %u size %u",
	     اندراج_ناپ, عدد_offsets, اندراج_ناپ * عدد_offsets);

  إذا (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_منزل_old_gid_list, سچ, (HBUINT16 *) loca_مفرد_انکڑے, عدد_offsets);
  وإلا
    _write_loca (padded_offsets, c->plan->new_منزل_old_gid_list, خطأ, (HBUINT32 *) loca_مفرد_انکڑے, عدد_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_مفرد_انکڑے,
					 اندراج_ناپ * عدد_offsets,
					 HB_حافظہ_منوال_WRITABLE,
					 loca_مفرد_انکڑے,
					 hb_free);

  منطقي نتیجہ = c->plan->جوڑو_جدول (extended ? HB_OT_ٹیگ_LOCA : HB_OT_ٹیگ_loca, loca_blob)
	     && جوڑو_سر_and_سیٹ_loca_نسخہ (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  أرجع نتیجہ;
}


} /* namespace glyf_impl */
} /* namespace OT */


#جے_انت /* OT_GLYF_GLYF_HELPERS_HH */
