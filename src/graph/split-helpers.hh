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

#جے_بنا_تعریف گراف_ونڈو_HELPERS_HH
#تعریف_کرو گراف_ونڈو_HELPERS_HH

#ضمن "graph-result.hh"

نطاق گراف {

سانچا<قسم_ناں Context>
HB_اندرونی
گراف_نتیجہ_t<hb_سمتیہ_t<بنانشانی>> actuate_subtable_ونڈو (Context& ونڈو_context,
                                                              ثابت hb_سمتیہ_t<بنانشانی>& ونڈو_points)
{
  hb_سمتیہ_t<بنانشانی> new_objects;
  إذا (!ونڈو_points)
    أرجع Ok(new_objects);

  لكل (بنانشانی i = 0; i < ونڈو_points.لمائی; i++)
  {
    بنانشانی شروع = ونڈو_points[i];
    بنانشانی سرا = (i < ونڈو_points.لمائی - 1)
                   ? ونڈو_points[i + 1]
                   : ونڈو_context.original_گنتی_2 ();
    TRY_سونپو (بنانشانی id, ونڈو_context.clone_range (شروع, سرا));
    new_objects.push (id);
    TRY (گراف_نتیجہ_t<فراغ>::from (new_objects, ونڈ_FAILURE));
  }

  TRY (ونڈو_context.shrink (ونڈو_points[0]));

  أرجع Ok(new_objects);
}

}

#جے_انت  // GRAPH_SPLIT_HELPERS_HH
