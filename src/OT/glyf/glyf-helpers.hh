#اگر_تعریف_نشده OT_GLYF_GLYF_HELPERS_HH
#تعریف_کن OT_GLYF_GLYF_HELPERS_HH


#ضمن "../../hb-open-type.hh"
#ضمن "../../hb-subset-plan.hh"

#ضمن "loca.hh"


نطاق OT {
نطاق glyf_impl {


قالب<نام_نوع Iterator_درون, نام_نوع Type_بیرون,
	 hb_requires (hb_is_منبع_از (Iterator_درون, بیعلامت صحيح))>
ساكن فراغ
_write_loca (Iterator_درون&& it,
	     ثابت hb_sorted_وکتور_t<hb_codepoint_pair_t> new_مقصد_old_gid_list,
	     منطقي short_offsets,
	     Type_بیرون *dest,
	     بیعلامت رقم_offsets)
{
  بیعلامت راست_لغزش = short_offsets ? 1 : 0;
  بیعلامت جابه_جایی = 0;
  Type_بیرون value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t آخرین = 0;
  لكل (خودکار _ : new_مقصد_old_gid_list)
  {
    hb_codepoint_t gid = _.اولی;
    لكل (; آخرین < gid; آخرین++)
    {
      DEBUG_پیام (SUBSET, اشاره_تهی, "loca entry empty offset %u", جابه_جایی);
      *dest++ = value;
    }

    بیعلامت padded_اندازه = *it++;
    جابه_جایی += padded_اندازه;
    DEBUG_پیام (SUBSET, اشاره_تهی, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, جابه_جایی, padded_اندازه);
    value = جابه_جایی >> راست_لغزش;
    *dest++ = value;

    آخرین++; // Skip over gid
  }
  بیعلامت رقم_glyphs = رقم_offsets - 1;
  لكل (; آخرین < رقم_glyphs; آخرین++)
  {
    DEBUG_پیام (SUBSET, اشاره_تهی, "loca entry empty offset %u", جابه_جایی);
    *dest++ = value;
  }
}

ساكن منطقي
جمع_کن_سر_and_مجموعه_loca_نسخه (hb_subset_plan_t *plan, منطقي use_short_loca)
{
  hb_blob_t *سر_blob = hb_sanitize_context_t ().reference_جدول<سر_2> (plan->منبع);
  hb_blob_t *سر_اول_blob = hb_blob_copy_writable_or_fail (سر_blob);
  hb_blob_destroy (سر_blob);

  إذا (unlikely (!سر_اول_blob))
    أرجع خطأ;

  سر_2 *سر_اول = (سر_2 *) hb_blob_get_داده_writable (سر_اول_blob, اشاره_تهی);
  سر_اول->شاخص_مقصد_Loc_Format = use_short_loca ? 0 : 1;
  إذا (plan->normalized_coords)
  {
    سر_اول->xMin = plan->سر_maxp_info.xMin;
    سر_اول->xMax = plan->سر_maxp_info.xMax;
    سر_اول->yMin = plan->سر_maxp_info.yMin;
    سر_اول->yMax = plan->سر_maxp_info.yMax;

    بیعلامت orig_flag = سر_اول->flags;
    إذا (plan->سر_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    وإلا
      orig_flag &= ~(1 << 1);
    سر_اول->flags = orig_flag;
  }
  منطقي success = plan->جمع_کن_جدول (HB_OT_برچسب_سر, سر_اول_blob);

  hb_blob_destroy (سر_اول_blob);
  أرجع success;
}

قالب<نام_نوع Iterator,
	 hb_requires (hb_is_منبع_از (Iterator, بیعلامت صحيح))>
ساكن منطقي
جمع_کن_loca_and_سر (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    منطقي use_short_loca,
		    منطقي extended)
{
  بیعلامت رقم_offsets = c->plan->رقم_output_glyphs () + 1;
  بیعلامت مدخل_اندازه = use_short_loca ? 2 : 4;

  حرف *loca_اول_داده = (حرف *) hb_malloc2 (رقم_offsets, مدخل_اندازه);

  إذا (unlikely (!loca_اول_داده)) أرجع خطأ;

  DEBUG_پیام (SUBSET, اشاره_تهی, "loca entry_size %u num_offsets %u size %u",
	     مدخل_اندازه, رقم_offsets, مدخل_اندازه * رقم_offsets);

  إذا (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_مقصد_old_gid_list, درست, (HBUINT16 *) loca_اول_داده, رقم_offsets);
  وإلا
    _write_loca (padded_offsets, c->plan->new_مقصد_old_gid_list, خطأ, (HBUINT32 *) loca_اول_داده, رقم_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_اول_داده,
					 مدخل_اندازه * رقم_offsets,
					 HB_حافظه_نما_WRITABLE,
					 loca_اول_داده,
					 hb_free);

  منطقي نتیجه = c->plan->جمع_کن_جدول (extended ? HB_OT_برچسب_LOCA : HB_OT_برچسب_loca, loca_blob)
	     && جمع_کن_سر_and_مجموعه_loca_نسخه (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  أرجع نتیجه;
}


} /* namespace glyf_impl */
} /* namespace OT */


#پایان_اگر /* OT_GLYF_GLYF_HELPERS_HH */
