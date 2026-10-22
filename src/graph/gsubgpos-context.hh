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

#شامل "graph.hh"
#شامل "../hb-ot-layout-gsubgpos.hh"

#اگر_غیر_تعریف_شدہ گراف_GSUBGPOS_CONTEXT_HH
#تعریف_کرو گراف_GSUBGPOS_CONTEXT_HH

نامخلا گراف {

ساخت Lookup;

ساخت gsubgpos_گراف_context_t
{
  HB_اندرونی ساکن گراف_نتیجہ_t<gsubgpos_گراف_context_t> create (hb_لیبل_t جدول_لیبل_3,
                                                                      گراف_t& گراف_2);

  hb_لیبل_t جدول_لیبل_2;
  گراف_t& گراف;
  غیرعلامتی lookup_list_اشاریہ;
  hb_hashmap_t<غیرعلامتی, گراف::Lookup*> lookups;
  hb_hashmap_t<غیرعلامتی, غیرعلامتی> subtable_مقصد_extension;
  hb_hashmap_t<غیرعلامتی, hb_سمتیہ_t<غیرعلامتی>> بانٹو_subtables;


  HB_اندرونی گراف_نتیجہ_t<غیرعلامتی> create_گرہ (غیرعلامتی سائز);

  گراف_نتیجہ_t<خالی> جوڑو_بفر (حرف* بفر)
  {
    واپس گراف.جوڑو_بفر (بفر);
  }

 نجی:
  HB_اندرونی gsubgpos_گراف_context_t (hb_لیبل_t جدول_لیبل_3,
                                        گراف_t& گراف_2);

  HB_اندرونی غیرعلامتی عدد_non_ext_subtables ();
};

}

#اگر_ختم  // GRAPH_GSUBGPOS_CONTEXT
