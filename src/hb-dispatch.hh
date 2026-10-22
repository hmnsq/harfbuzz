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

#ifndef HB_DISPATCH_HH
#define HB_DISPATCH_HH

#அந்தர்பாவ "hb.hh"

/*
 * Dispatch
 */

template <typename Context, typename Return=hb_empty_t, unsigned முழு MaxDebugDepth=0>
கட்டமைப்பு hb_dispatch_context_t
{
  தனிப்பட்ட:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  மாறிலி Context* thiz () மாறிலி { திரும்பு static_cast<மாறிலி Context *> (இது); }
	Context* thiz ()       { திரும்பு static_cast<      Context *> (இது); }
  பொது:
  மாறிலி எழுத்து *get_name () { திரும்பு "UNKNOWN"; }
  நிலையான constexpr unsigned max_debug_depth = MaxDebugDepth;
  வகைவரை Return return_t;
  template <typename T, typename F>
  தருக்க may_dispatch (மாறிலி T *obj HB_UNUSED, மாறிலி F *format HB_UNUSED) { திரும்பு உண்மை; }
  template <typename T, typename ...Ts>
  return_t dispatch (மாறிலி T &obj, Ts&&... ds)
  { திரும்பு obj.dispatch (thiz (), std::forward<Ts> (ds)...); }
  நிலையான return_t no_dispatch_return_மதிப்பு () { திரும்பு Context::default_return_மதிப்பு (); }
  நிலையான தருக்க stop_sublookup_iteration (மாறிலி return_t r HB_UNUSED) { திரும்பு பொய்; }
  unsigned debug_depth = 0;
};


#endif /* HB_DISPATCH_HH */
