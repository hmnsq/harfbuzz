#که_نه_تعریف_شوی OT_GLYF_GLYF_HELPERS_HH
#تعریف_کړه OT_GLYF_GLYF_HELPERS_HH


#ضمن "../../hb-open-type.hh"
#ضمن "../../hb-subset-plan.hh"

#ضمن "loca.hh"


نطاق OT {
نطاق glyf_impl {


کينډۍ<ډول_نوم Iterator_دننه, ډول_نوم Type_بهر,
	 hb_requires (hb_is_سرچینه_د (Iterator_دننه, بینښه صحيح))>
ساكن فراغ
_write_loca (Iterator_دننه&& it,
	     ثابت hb_sorted_ویکټور_t<hb_codepoint_pair_t> new_منزل_old_gid_list,
	     منطقي short_offsets,
	     Type_بهر *dest,
	     بینښه عدد_offsets)
{
  بینښه ښی_ښويدنه = short_offsets ? 1 : 0;
  بینښه دننه_واټن = 0;
  Type_بهر value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t وروستی = 0;
  لكل (خپلکاری _ : new_منزل_old_gid_list)
  {
    hb_codepoint_t gid = _.لومړی;
    لكل (; وروستی < gid; وروستی++)
    {
      DEBUG_پیغام (SUBSET, تش_اشاره, "loca entry empty offset %u", دننه_واټن);
      *dest++ = value;
    }

    بینښه padded_اندازه = *it++;
    دننه_واټن += padded_اندازه;
    DEBUG_پیغام (SUBSET, تش_اشاره, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, دننه_واټن, padded_اندازه);
    value = دننه_واټن >> ښی_ښويدنه;
    *dest++ = value;

    وروستی++; // Skip over gid
  }
  بینښه عدد_glyphs = عدد_offsets - 1;
  لكل (; وروستی < عدد_glyphs; وروستی++)
  {
    DEBUG_پیغام (SUBSET, تش_اشاره, "loca entry empty offset %u", دننه_واټن);
    *dest++ = value;
  }
}

ساكن منطقي
جمع_کړه_سر_and_سېټ_loca_نسخه (hb_subset_plan_t *plan, منطقي use_short_loca)
{
  hb_blob_t *سر_blob = hb_sanitize_context_t ().reference_جدول<سر_2> (plan->سرچینه);
  hb_blob_t *سر_اولي_blob = hb_blob_copy_writable_or_fail (سر_blob);
  hb_blob_destroy (سر_blob);

  إذا (unlikely (!سر_اولي_blob))
    أرجع خطأ;

  سر_2 *سر_اولي = (سر_2 *) hb_blob_get_معلومات_writable (سر_اولي_blob, تش_اشاره);
  سر_اولي->شاخص_منزل_Loc_Format = use_short_loca ? 0 : 1;
  إذا (plan->normalized_coords)
  {
    سر_اولي->xMin = plan->سر_maxp_info.xMin;
    سر_اولي->xMax = plan->سر_maxp_info.xMax;
    سر_اولي->yMin = plan->سر_maxp_info.yMin;
    سر_اولي->yMax = plan->سر_maxp_info.yMax;

    بینښه orig_flag = سر_اولي->flags;
    إذا (plan->سر_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    وإلا
      orig_flag &= ~(1 << 1);
    سر_اولي->flags = orig_flag;
  }
  منطقي success = plan->جمع_کړه_جدول (HB_OT_نښان_سر, سر_اولي_blob);

  hb_blob_destroy (سر_اولي_blob);
  أرجع success;
}

کينډۍ<ډول_نوم Iterator,
	 hb_requires (hb_is_سرچینه_د (Iterator, بینښه صحيح))>
ساكن منطقي
جمع_کړه_loca_and_سر (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    منطقي use_short_loca,
		    منطقي extended)
{
  بینښه عدد_offsets = c->plan->عدد_output_glyphs () + 1;
  بینښه ثبت_اندازه = use_short_loca ? 2 : 4;

  حرف *loca_اولي_معلومات = (حرف *) hb_malloc2 (عدد_offsets, ثبت_اندازه);

  إذا (unlikely (!loca_اولي_معلومات)) أرجع خطأ;

  DEBUG_پیغام (SUBSET, تش_اشاره, "loca entry_size %u num_offsets %u size %u",
	     ثبت_اندازه, عدد_offsets, ثبت_اندازه * عدد_offsets);

  إذا (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_منزل_old_gid_list, ریښتیا, (HBUINT16 *) loca_اولي_معلومات, عدد_offsets);
  وإلا
    _write_loca (padded_offsets, c->plan->new_منزل_old_gid_list, خطأ, (HBUINT32 *) loca_اولي_معلومات, عدد_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_اولي_معلومات,
					 ثبت_اندازه * عدد_offsets,
					 HB_حافظه_ډېرپېښ_WRITABLE,
					 loca_اولي_معلومات,
					 hb_free);

  منطقي پایله = c->plan->جمع_کړه_جدول (extended ? HB_OT_نښان_LOCA : HB_OT_نښان_loca, loca_blob)
	     && جمع_کړه_سر_and_سېټ_loca_نسخه (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  أرجع پایله;
}


} /* namespace glyf_impl */
} /* namespace OT */


#د_که_پای /* OT_GLYF_GLYF_HELPERS_HH */
