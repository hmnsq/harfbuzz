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

#اگر_غیر_تعریف_شدہ گراف_بانٹو_HELPERS_HH
#تعریف_کرو گراف_بانٹو_HELPERS_HH

#شامل "graph-result.hh"

نامخلا گراف {

سانچہ<قسم_نام Context>
HB_اندرونی
گراف_نتیجہ_t<hb_سمتیہ_t<غیرعلامتی>> actuate_subtable_بانٹو (Context& بانٹو_context,
                                                              ثابت hb_سمتیہ_t<غیرعلامتی>& بانٹو_points)
{
  hb_سمتیہ_t<غیرعلامتی> new_objects;
  اگر (!بانٹو_points)
    واپس Ok(new_objects);

  برائے (غیرعلامتی i = 0; i < بانٹو_points.لمبائی; i++)
  {
    غیرعلامتی آغاز = بانٹو_points[i];
    غیرعلامتی انتہا = (i < بانٹو_points.لمبائی - 1)
                   ? بانٹو_points[i + 1]
                   : بانٹو_context.original_گنتی_2 ();
    TRY_تفویض (غیرعلامتی id, بانٹو_context.clone_range (آغاز, انتہا));
    new_objects.push (id);
    TRY (گراف_نتیجہ_t<خالی>::from (new_objects, تخصیص_FAILURE));
  }

  TRY (بانٹو_context.shrink (بانٹو_points[0]));

  واپس Ok(new_objects);
}

}

#اگر_ختم  // GRAPH_SPLIT_HELPERS_HH
