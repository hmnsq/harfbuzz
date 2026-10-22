/*
 * Copyright © 2007,2008,2009,2010  Red Hat, Inc.
 * Copyright © 2012,2018  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#اگر_غیر_تعریف_شدہ HB_DISPATCH_HH
#تعریف_کرو HB_DISPATCH_HH

#شامل "hb.hh"

/*
 * Dispatch
 */

سانچہ <قسم_نام Context, قسم_نام Return=hb_خالی_خانے_t, غیرعلامتی صحیح Max_Debug_گہرائی=0>
ساخت hb_dispatch_context_t
{
  نجی:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت Context* thiz () ثابت { واپس static_cast<ثابت Context *> (یہ); }
	Context* thiz ()       { واپس static_cast<      Context *> (یہ); }
  عوامی:
  ثابت حرف *get_name () { واپس "UNKNOWN"; }
  ساکن constexpr غیرعلامتی max_debug_گہرائی = Max_Debug_گہرائی;
  قسمتعریف Return return_t;
  سانچہ <قسم_نام T, قسم_نام F>
  منطقی may_dispatch (ثابت T *obj HB_غیر_مستعمل, ثابت F *format HB_غیر_مستعمل) { واپس سچ; }
  سانچہ <قسم_نام T, قسم_نام ...Ts>
  return_t dispatch (ثابت T &obj, Ts&&... ds)
  { واپس obj.dispatch (thiz (), std::forward<Ts> (ds)...); }
  ساکن return_t no_dispatch_return_قدر () { واپس Context::default_return_قدر (); }
  ساکن منطقی stop_sublookup_iteration (ثابت return_t r HB_غیر_مستعمل) { واپس جھوٹ; }
  غیرعلامتی debug_گہرائی = 0;
};


#اگر_ختم /* HB_DISPATCH_HH */
