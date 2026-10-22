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

#که_نه_تعریف_شوی ګراف_وېشل_HELPERS_HH
#تعریف_کړه ګراف_وېشل_HELPERS_HH

#ضمن "graph-result.hh"

نطاق ګراف {

کينډۍ<ډول_نوم Context>
HB_دننی
ګراف_پایله_t<hb_ویکټور_t<بینښه>> actuate_subtable_وېشل (Context& وېشل_context,
                                                              ثابت hb_ویکټور_t<بینښه>& وېشل_points)
{
  hb_ویکټور_t<بینښه> new_objects;
  إذا (!وېشل_points)
    أرجع Ok(new_objects);

  لكل (بینښه i = 0; i < وېشل_points.اوږدتیا; i++)
  {
    بینښه پیل = وېشل_points[i];
    بینښه پای_ټکی = (i < وېشل_points.اوږدتیا - 1)
                   ? وېشل_points[i + 1]
                   : وېشل_context.original_شمېر_2 ();
    TRY_ټاکل (بینښه id, وېشل_context.clone_range (پیل, پای_ټکی));
    new_objects.push (id);
    TRY (ګراف_پایله_t<فراغ>::from (new_objects, ویش_FAILURE));
  }

  TRY (وېشل_context.shrink (وېشل_points[0]));

  أرجع Ok(new_objects);
}

}

#د_که_پای  // GRAPH_SPLIT_HELPERS_HH
