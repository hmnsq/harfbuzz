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

#અન્તર્ભાવ "hb.hh"

/*
 * Dispatch
 */

template <typename Context, typename Return=hb_empty_t, unsigned પૂર્ણાંક MaxDebugDepth=0>
સંરચના hb_dispatch_context_t
{
  ખાનગી:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  અચલ Context* thiz () અચલ { પાછા static_cast<અચલ Context *> (આ); }
	Context* thiz ()       { પાછા static_cast<      Context *> (આ); }
  જાહેર:
  અચલ અક્ષર *get_name () { પાછા "UNKNOWN"; }
  સ્થિર constexpr unsigned max_debug_depth = MaxDebugDepth;
  પ્રકારવ્યાખ્યા Return return_t;
  template <typename T, typename F>
  તાર્કિક may_dispatch (અચલ T *obj HB_UNUSED, અચલ F *format HB_UNUSED) { પાછા સાચું; }
  template <typename T, typename ...Ts>
  return_t dispatch (અચલ T &obj, Ts&&... ds)
  { પાછા obj.dispatch (thiz (), std::forward<Ts> (ds)...); }
  સ્થિર return_t no_dispatch_return_કિંમત () { પાછા Context::default_return_કિંમત (); }
  સ્થિર તાર્કિક stop_sublookup_iteration (અચલ return_t r HB_UNUSED) { પાછા ખોટું; }
  unsigned debug_depth = 0;
};


#endif /* HB_DISPATCH_HH */
