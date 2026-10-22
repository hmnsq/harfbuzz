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

#که_نه_تعریف_شوی ګراف_GSUBGPOS_CONTEXT_HH
#تعریف_کړه ګراف_GSUBGPOS_CONTEXT_HH

نطاق ګراف {

بنية Lookup;

بنية gsubgpos_ګراف_context_t
{
  HB_دننی ساكن ګراف_پایله_t<gsubgpos_ګراف_context_t> create (hb_نښان_t جدول_نښان_3,
                                                                      ګراف_t& ګراف_2);

  hb_نښان_t جدول_نښان_2;
  ګراف_t& ګراف;
  بینښه lookup_list_شاخص;
  hb_hashmap_t<بینښه, ګراف::Lookup*> lookups;
  hb_hashmap_t<بینښه, بینښه> subtable_منزل_extension;
  hb_hashmap_t<بینښه, hb_ویکټور_t<بینښه>> وېشل_subtables;


  HB_دننی ګراف_پایله_t<بینښه> create_غوټه (بینښه اندازه);

  ګراف_پایله_t<فراغ> جمع_کړه_بفر (حرف* بفر)
  {
    أرجع ګراف.جمع_کړه_بفر (بفر);
  }

 خاص:
  HB_دننی gsubgpos_ګراف_context_t (hb_نښان_t جدول_نښان_3,
                                        ګراف_t& ګراف_2);

  HB_دننی بینښه عدد_non_ext_subtables ();
};

}

#د_که_پای  // GRAPH_GSUBGPOS_CONTEXT
