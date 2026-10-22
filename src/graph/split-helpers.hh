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

#جيڪڏهن_وصف_نه_ٿيل گراف_ورهايو_HELPERS_HH
#وصف_ڪريو گراف_ورهايو_HELPERS_HH

#ضمن "graph-result.hh"

نطاق گراف {

سانچو<قسم_نالو Context>
HB_اندروني
گراف_نتيجو_t<hb_ويڪٽر_t<بنانشان>> actuate_subtable_ورهايو (Context& ورهايو_context,
                                                              ثابت hb_ويڪٽر_t<بنانشان>& ورهايو_points)
{
  hb_ويڪٽر_t<بنانشان> new_objects;
  إذا (!ورهايو_points)
    أرجع Ok(new_objects);

  لكل (بنانشان i = 0; i < ورهايو_points.ڊيگهائي; i++)
  {
    بنانشان شروعات = ورهايو_points[i];
    بنانشان پڇاڙي = (i < ورهايو_points.ڊيگهائي - 1)
                   ? ورهايو_points[i + 1]
                   : ورهايو_context.original_ڳڻپ_2 ();
    TRY_سونپيو (بنانشان id, ورهايو_context.clone_range (شروعات, پڇاڙي));
    new_objects.push (id);
    TRY (گراف_نتيجو_t<فراغ>::from (new_objects, ورهاست_FAILURE));
  }

  TRY (ورهايو_context.shrink (ورهايو_points[0]));

  أرجع Ok(new_objects);
}

}

#جيڪڏهن_پورو  // GRAPH_SPLIT_HELPERS_HH
