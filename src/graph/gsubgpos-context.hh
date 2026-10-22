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

#ضمن "graph.hh"
#ضمن "../hb-ot-layout-gsubgpos.hh"

#جيڪڏهن_وصف_نه_ٿيل گراف_GSUBGPOS_CONTEXT_HH
#وصف_ڪريو گراف_GSUBGPOS_CONTEXT_HH

نطاق گراف {

بنية Lookup;

بنية gsubgpos_گراف_context_t
{
  HB_اندروني ساكن گراف_نتيجو_t<gsubgpos_گراف_context_t> create (hb_ٽيگ_t جدول_ٽيگ_3,
                                                                      گراف_t& گراف_2);

  hb_ٽيگ_t جدول_ٽيگ_2;
  گراف_t& گراف;
  بنانشان lookup_list_اشاريو;
  hb_hashmap_t<بنانشان, گراف::Lookup*> lookups;
  hb_hashmap_t<بنانشان, بنانشان> subtable_منزل_extension;
  hb_hashmap_t<بنانشان, hb_ويڪٽر_t<بنانشان>> ورهايو_subtables;


  HB_اندروني گراف_نتيجو_t<بنانشان> create_ڳنڍ (بنانشان سائيز);

  گراف_نتيجو_t<فراغ> جوڙيو_بفر (حرف* بفر)
  {
    أرجع گراف.جوڙيو_بفر (بفر);
  }

 خاص:
  HB_اندروني gsubgpos_گراف_context_t (hb_ٽيگ_t جدول_ٽيگ_3,
                                        گراف_t& گراف_2);

  HB_اندروني بنانشان انگ_non_ext_subtables ();
};

}

#جيڪڏهن_پورو  // GRAPH_GSUBGPOS_CONTEXT
