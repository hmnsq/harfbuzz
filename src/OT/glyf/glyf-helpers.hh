#جيڪڏهن_وصف_نه_ٿيل OT_GLYF_GLYF_HELPERS_HH
#وصف_ڪريو OT_GLYF_GLYF_HELPERS_HH


#ضمن "../../hb-open-type.hh"
#ضمن "../../hb-subset-plan.hh"

#ضمن "loca.hh"


نطاق OT {
نطاق glyf_impl {


سانچو<قسم_نالو Iterator_اندريون, قسم_نالو Type_ٻاهريون,
	 hb_requires (hb_is_ذريعو_جو (Iterator_اندريون, بنانشان صحيح))>
ساكن فراغ
_write_loca (Iterator_اندريون&& it,
	     ثابت hb_sorted_ويڪٽر_t<hb_codepoint_pair_t> new_منزل_old_gid_list,
	     منطقي short_offsets,
	     Type_ٻاهريون *dest,
	     بنانشان انگ_offsets)
{
  بنانشان ساڄو_سرڪايو = short_offsets ? 1 : 0;
  بنانشان وٿي = 0;
  Type_ٻاهريون value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t آخري = 0;
  لكل (خودڪار _ : new_منزل_old_gid_list)
  {
    hb_codepoint_t gid = _.پهريون;
    لكل (; آخري < gid; آخري++)
    {
      DEBUG_نياپو (SUBSET, خالي_اشارو, "loca entry empty offset %u", وٿي);
      *dest++ = value;
    }

    بنانشان padded_سائيز = *it++;
    وٿي += padded_سائيز;
    DEBUG_نياپو (SUBSET, خالي_اشارو, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, وٿي, padded_سائيز);
    value = وٿي >> ساڄو_سرڪايو;
    *dest++ = value;

    آخري++; // Skip over gid
  }
  بنانشان انگ_glyphs = انگ_offsets - 1;
  لكل (; آخري < انگ_glyphs; آخري++)
  {
    DEBUG_نياپو (SUBSET, خالي_اشارو, "loca entry empty offset %u", وٿي);
    *dest++ = value;
  }
}

ساكن منطقي
جوڙيو_سر_and_سيٽ_loca_نسخو (hb_subset_plan_t *plan, منطقي use_short_loca)
{
  hb_blob_t *سر_blob = hb_sanitize_context_t ().reference_جدول<سر_2> (plan->ذريعو);
  hb_blob_t *سر_مفرد_blob = hb_blob_copy_writable_or_fail (سر_blob);
  hb_blob_destroy (سر_blob);

  إذا (unlikely (!سر_مفرد_blob))
    أرجع خطأ;

  سر_2 *سر_مفرد = (سر_2 *) hb_blob_get_معلومات_writable (سر_مفرد_blob, خالي_اشارو);
  سر_مفرد->اشاريو_منزل_Loc_Format = use_short_loca ? 0 : 1;
  إذا (plan->normalized_coords)
  {
    سر_مفرد->xMin = plan->سر_maxp_info.xMin;
    سر_مفرد->xMax = plan->سر_maxp_info.xMax;
    سر_مفرد->yMin = plan->سر_maxp_info.yMin;
    سر_مفرد->yMax = plan->سر_maxp_info.yMax;

    بنانشان orig_flag = سر_مفرد->flags;
    إذا (plan->سر_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    وإلا
      orig_flag &= ~(1 << 1);
    سر_مفرد->flags = orig_flag;
  }
  منطقي success = plan->جوڙيو_جدول (HB_OT_ٽيگ_سر, سر_مفرد_blob);

  hb_blob_destroy (سر_مفرد_blob);
  أرجع success;
}

سانچو<قسم_نالو Iterator,
	 hb_requires (hb_is_ذريعو_جو (Iterator, بنانشان صحيح))>
ساكن منطقي
جوڙيو_loca_and_سر (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    منطقي use_short_loca,
		    منطقي extended)
{
  بنانشان انگ_offsets = c->plan->انگ_output_glyphs () + 1;
  بنانشان داخلا_سائيز = use_short_loca ? 2 : 4;

  حرف *loca_مفرد_معلومات = (حرف *) hb_malloc2 (انگ_offsets, داخلا_سائيز);

  إذا (unlikely (!loca_مفرد_معلومات)) أرجع خطأ;

  DEBUG_نياپو (SUBSET, خالي_اشارو, "loca entry_size %u num_offsets %u size %u",
	     داخلا_سائيز, انگ_offsets, داخلا_سائيز * انگ_offsets);

  إذا (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_منزل_old_gid_list, سچ, (HBUINT16 *) loca_مفرد_معلومات, انگ_offsets);
  وإلا
    _write_loca (padded_offsets, c->plan->new_منزل_old_gid_list, خطأ, (HBUINT32 *) loca_مفرد_معلومات, انگ_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_مفرد_معلومات,
					 داخلا_سائيز * انگ_offsets,
					 HB_يادگيري_گهڻائي_WRITABLE,
					 loca_مفرد_معلومات,
					 hb_free);

  منطقي نتيجو = c->plan->جوڙيو_جدول (extended ? HB_OT_ٽيگ_LOCA : HB_OT_ٽيگ_loca, loca_blob)
	     && جوڙيو_سر_and_سيٽ_loca_نسخو (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  أرجع نتيجو;
}


} /* namespace glyf_impl */
} /* namespace OT */


#جيڪڏهن_پورو /* OT_GLYF_GLYF_HELPERS_HH */
