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

#اگر_تعریف_نشده گراف_تقسیم_HELPERS_HH
#تعریف_کن گراف_تقسیم_HELPERS_HH

#ضمن "graph-result.hh"

نطاق گراف {

قالب<نام_نوع Context>
HB_داخلی
گراف_نتیجه_t<hb_وکتور_t<بیعلامت>> actuate_subtable_تقسیم (Context& تقسیم_context,
                                                              ثابت hb_وکتور_t<بیعلامت>& تقسیم_points)
{
  hb_وکتور_t<بیعلامت> new_objects;
  إذا (!تقسیم_points)
    أرجع Ok(new_objects);

  لكل (بیعلامت i = 0; i < تقسیم_points.درازی; i++)
  {
    بیعلامت شروع = تقسیم_points[i];
    بیعلامت انتها = (i < تقسیم_points.درازی - 1)
                   ? تقسیم_points[i + 1]
                   : تقسیم_context.original_شمار_2 ();
    TRY_تخصیص_بده (بیعلامت id, تقسیم_context.clone_range (شروع, انتها));
    new_objects.push (id);
    TRY (گراف_نتیجه_t<فراغ>::from (new_objects, تخصیص_FAILURE));
  }

  TRY (تقسیم_context.shrink (تقسیم_points[0]));

  أرجع Ok(new_objects);
}

}

#پایان_اگر  // GRAPH_SPLIT_HELPERS_HH
