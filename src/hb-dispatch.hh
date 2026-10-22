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

#अगर_अघोषित HB_DISPATCH_HH
#घोषणा HB_DISPATCH_HH

#समावेश "hb.hh"

/*
 * Dispatch
 */

ढाँचा <typename Context, typename Return=hb_empty_t, अचिन्हित पूर्णांक MaxDebugDepth=0>
काष्ठा hb_dispatch_context_t
{
  निजी:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  स्थिर Context* thiz () स्थिर { वापस static_cast<स्थिर Context *> (यह); }
	Context* thiz ()       { वापस static_cast<      Context *> (यह); }
  खुला:
  स्थिर अक्षर *get_name () { वापस "UNKNOWN"; }
  अटल constexpr अचिन्हित max_debug_depth = MaxDebugDepth;
  प्रकार Return return_t;
  ढाँचा <typename T, typename F>
  bool may_dispatch (स्थिर T *obj HB_UNUSED, स्थिर F *format HB_UNUSED) { वापस true; }
  ढाँचा <typename T, typename ...Ts>
  return_t dispatch (स्थिर T &obj, Ts&&... ds)
  { वापस obj.dispatch (thiz (), std::forward<Ts> (ds)...); }
  अटल return_t no_dispatch_return_मूल्य () { वापस Context::default_return_मूल्य (); }
  अटल bool stop_sublookup_iteration (स्थिर return_t r HB_UNUSED) { वापस false; }
  अचिन्हित debug_depth = 0;
};


#पूर्ण_अगर /* HB_DISPATCH_HH */
