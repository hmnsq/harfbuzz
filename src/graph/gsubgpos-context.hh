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

#اگر_تعریف_نشده گراف_GSUBGPOS_CONTEXT_HH
#تعریف_کن گراف_GSUBGPOS_CONTEXT_HH

نطاق گراف {

بنية Lookup;

بنية gsubgpos_گراف_context_t
{
  HB_داخلی ساكن گراف_نتیجه_t<gsubgpos_گراف_context_t> create (hb_برچسب_t جدول_برچسب_3,
                                                                      گراف_t& گراف_2);

  hb_برچسب_t جدول_برچسب_2;
  گراف_t& گراف;
  بیعلامت lookup_list_شاخص;
  hb_hashmap_t<بیعلامت, گراف::Lookup*> lookups;
  hb_hashmap_t<بیعلامت, بیعلامت> subtable_مقصد_extension;
  hb_hashmap_t<بیعلامت, hb_وکتور_t<بیعلامت>> تقسیم_subtables;


  HB_داخلی گراف_نتیجه_t<بیعلامت> create_گره (بیعلامت اندازه);

  گراف_نتیجه_t<فراغ> جمع_کن_میانگیر (حرف* میانگیر)
  {
    أرجع گراف.جمع_کن_میانگیر (میانگیر);
  }

 خاص:
  HB_داخلی gsubgpos_گراف_context_t (hb_برچسب_t جدول_برچسب_3,
                                        گراف_t& گراف_2);

  HB_داخلی بیعلامت رقم_non_ext_subtables ();
};

}

#پایان_اگر  // GRAPH_GSUBGPOS_CONTEXT
