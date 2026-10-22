#অন্তর্ভাব "hb-subset-table.hh"

#অন্তর্ভাব "hb-ot-cmap-table.hh"
#অন্তর্ভাব "hb-ot-glyf-table.hh"
#অন্তর্ভাব "hb-ot-hdmx-table.hh"
#অন্তর্ভাব "hb-ot-hhea-table.hh"
#অন্তর্ভাব "hb-ot-hmtx-table.hh"
#অন্তর্ভাব "hb-ot-maxp-table.hh"
#অন্তর্ভাব "hb-ot-os2-table.hh"
#অন্তর্ভাব "hb-ot-name-table.hh"
#অন্তর্ভাব "hb-ot-post-table.hh"

তার্কিক _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<বর্ণ> &buf, hb_tag_t tag, তার্কিক *success)
{
  বিকল্প (tag)
  {
  পক্ষ HB_TAG('g','l','y','f'): *success = _hb_subset_table<ধ্রুব OT::glyf> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('h','d','m','x'): *success = _hb_subset_table<ধ্রুব OT::hdmx> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('n','a','m','e'): *success = _hb_subset_table<ধ্রুব OT::name> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('h','h','e','a'): *success = সত্য; প্রত্যাগম সত্য; /* skip hhea, handled by hmtx */
  পক্ষ HB_TAG('h','m','t','x'): *success = _hb_subset_table<ধ্রুব OT::hmtx> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('v','h','e','a'): *success = সত্য; প্রত্যাগম সত্য; /* skip vhea, handled by vmtx */
  পক্ষ HB_TAG('v','m','t','x'): *success = _hb_subset_table<ধ্রুব OT::vmtx> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('m','a','x','p'): *success = _hb_subset_table<ধ্রুব OT::maxp> (plan, buf); প্রত্যাগম সত্য;
#ifndef HB_NO_BEYOND_64K
  পক্ষ HB_TAG('G','L','Y','F'): *success = _hb_subset_table<ধ্রুব OT::GLYF> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('H','H','E','A'): *success = সত্য; প্রত্যাগম সত্য; /* skip HHEA, handled by HMTX */
  পক্ষ HB_TAG('H','M','T','X'): *success = _hb_subset_table<ধ্রুব OT::HMTX> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('V','H','E','A'): *success = সত্য; প্রত্যাগম সত্য; /* skip VHEA, handled by VMTX */
  পক্ষ HB_TAG('V','M','T','X'): *success = _hb_subset_table<ধ্রুব OT::VMTX> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('M','A','X','P'): *success = _hb_subset_table<ধ্রুব OT::MAXP> (plan, buf); প্রত্যাগম সত্য;
#endif
  পক্ষ HB_TAG('l','o','c','a'): *success = সত্য; প্রত্যাগম সত্য; /* skip loca, handled by glyf */
#ifndef HB_NO_BEYOND_64K
  পক্ষ HB_TAG('L','O','C','A'): *success = সত্য; প্রত্যাগম সত্য; /* skip LOCA, handled by GLYF */
#endif
  পক্ষ HB_TAG('c','m','a','p'): *success = _hb_subset_table<ধ্রুব OT::cmap> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('D','M','A','P'): *success = _hb_subset_table<ধ্রুব OT::DMAP> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('O','S','/','2'): *success = _hb_subset_table<ধ্রুব OT::OS2 > (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('p','o','s','t'): *success = _hb_subset_table<ধ্রুব OT::post> (plan, buf); প্রত্যাগম সত্য;
  }
  প্রত্যাগম অসত্য;
}
