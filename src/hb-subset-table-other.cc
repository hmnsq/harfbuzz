#অন্তর্ভাৱ "hb-subset-table.hh"

#অন্তর্ভাৱ "hb-ot-cmap-table.hh"
#অন্তর্ভাৱ "hb-ot-glyf-table.hh"
#অন্তর্ভাৱ "hb-ot-hdmx-table.hh"
#অন্তর্ভাৱ "hb-ot-hhea-table.hh"
#অন্তর্ভাৱ "hb-ot-hmtx-table.hh"
#অন্তর্ভাৱ "hb-ot-maxp-table.hh"
#অন্তর্ভাৱ "hb-ot-os2-table.hh"
#অন্তর্ভাৱ "hb-ot-name-table.hh"
#অন্তর্ভাৱ "hb-ot-post-table.hh"

তার্কিক _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<ৱর্ণ> &buf, hb_tag_t tag, তার্কিক *success)
{
  ৱিকল্প (tag)
  {
  পক্ষ HB_TAG('g','l','y','f'): *success = _hb_subset_table<ধ্রুৱ OT::glyf> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('h','d','m','x'): *success = _hb_subset_table<ধ্রুৱ OT::hdmx> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('n','a','m','e'): *success = _hb_subset_table<ধ্রুৱ OT::name> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('h','h','e','a'): *success = সঁচা; ঘূৰাই সঁচা; /* skip hhea, handled by hmtx */
  পক্ষ HB_TAG('h','m','t','x'): *success = _hb_subset_table<ধ্রুৱ OT::hmtx> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('v','h','e','a'): *success = সঁচা; ঘূৰাই সঁচা; /* skip vhea, handled by vmtx */
  পক্ষ HB_TAG('v','m','t','x'): *success = _hb_subset_table<ধ্রুৱ OT::vmtx> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('m','a','x','p'): *success = _hb_subset_table<ধ্রুৱ OT::maxp> (plan, buf); ঘূৰাই সঁচা;
#ifndef HB_NO_BEYOND_64K
  পক্ষ HB_TAG('G','L','Y','F'): *success = _hb_subset_table<ধ্রুৱ OT::GLYF> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('H','H','E','A'): *success = সঁচা; ঘূৰাই সঁচা; /* skip HHEA, handled by HMTX */
  পক্ষ HB_TAG('H','M','T','X'): *success = _hb_subset_table<ধ্রুৱ OT::HMTX> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('V','H','E','A'): *success = সঁচা; ঘূৰাই সঁচা; /* skip VHEA, handled by VMTX */
  পক্ষ HB_TAG('V','M','T','X'): *success = _hb_subset_table<ধ্রুৱ OT::VMTX> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('M','A','X','P'): *success = _hb_subset_table<ধ্রুৱ OT::MAXP> (plan, buf); ঘূৰাই সঁচা;
#endif
  পক্ষ HB_TAG('l','o','c','a'): *success = সঁচা; ঘূৰাই সঁচা; /* skip loca, handled by glyf */
#ifndef HB_NO_BEYOND_64K
  পক্ষ HB_TAG('L','O','C','A'): *success = সঁচা; ঘূৰাই সঁচা; /* skip LOCA, handled by GLYF */
#endif
  পক্ষ HB_TAG('c','m','a','p'): *success = _hb_subset_table<ধ্রুৱ OT::cmap> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('D','M','A','P'): *success = _hb_subset_table<ধ্রুৱ OT::DMAP> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('O','S','/','2'): *success = _hb_subset_table<ধ্রুৱ OT::OS2 > (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('p','o','s','t'): *success = _hb_subset_table<ধ্রুৱ OT::post> (plan, buf); ঘূৰাই সঁচা;
  }
  ঘূৰাই মিছা;
}
