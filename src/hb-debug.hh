/*
 * Copyright © 2017  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#اگر_غیر_تعریف_شدہ HB_DEBUG_HH
#تعریف_کرو HB_DEBUG_HH

#شامل "hb.hh"
#شامل "hb-atomic.hh"
#شامل "hb-algs.hh"


#اگر_غیر_تعریف_شدہ HB_DEBUG
#تعریف_کرو HB_DEBUG 0
#اگر_ختم


/*
 * Debug output (needs enabling at compile time.)
 */

ساکن اندرونی منطقی
_hb_debug (غیرعلامتی صحیح level,
	   غیرعلامتی صحیح max_level)
{
  واپس level < max_level;
}

#تعریف_کرو DEBUG_LEVEL_ENABLED(WHAT, LEVEL) (_hb_debug ((LEVEL), HB_DEBUG_##WHAT))
#تعریف_کرو DEBUG_ENABLED(WHAT) (DEBUG_LEVEL_ENABLED (WHAT, 0))

ساکن اندرونی خالی
_hb_print_func (ثابت حرف *func)
{
  اگر (func)
  {
    غیرعلامتی صحیح func_len = لڑی_لمبائی (func);
    /* Skip "static" */
    اگر (0 == لڑی_موازنہ (func, "static ", 7))
      func += 7;
    /* Skip "typename" */
    اگر (0 == لڑی_موازنہ (func, "typename ", 9))
      func += 9;
    /* Skip return type */
    ثابت حرف *space = strchr (func, ' ');
    اگر (space)
      func = space + 1;
    /* Skip parameter list */
    ثابت حرف *paren = strchr (func, '(');
    اگر (paren)
      func_len = paren - func;
    fprintf (stderr, "%.*s", (صحیح) func_len, func);
  }
}

سانچہ <صحیح max_level> ساکن اندرونی خالی
hb_debug_پیغام_va (ثابت حرف *what,
		  ثابت خالی *obj,
		  ثابت حرف *func,
		  منطقی indented,
		  غیرعلامتی صحیح level,
		  صحیح level_dir,
		  ثابت حرف *پیغام,
		  va_list ap) HB_PRINTF_FUNC(7, 0);
سانچہ <صحیح max_level> ساکن اندرونی خالی
hb_debug_پیغام_va (ثابت حرف *what,
		  ثابت خالی *obj,
		  ثابت حرف *func,
		  منطقی indented,
		  غیرعلامتی صحیح level,
		  صحیح level_dir,
		  ثابت حرف *پیغام,
		  va_list ap)
{
  اگر (!_hb_debug (level, max_level))
    واپس;

  fprintf (stderr, "%-10s", what ? what : "");

  اگر (obj)
    fprintf (stderr, "(%*p) ", (صحیح) (2 * حجم (خالی *)), obj);
  ورنہ
    fprintf (stderr, " %*s  ", (صحیح) (2 * حجم (خالی *)), "");

  اگر (indented) {
#تعریف_کرو VBAR	"\342\224\202"	/* U+2502 BOX DRAWINGS LIGHT VERTICAL */
#تعریف_کرو VRBAR	"\342\224\234"	/* U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
#تعریف_کرو DLBAR	"\342\225\256"	/* U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
#تعریف_کرو ULBAR	"\342\225\257"	/* U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
#تعریف_کرو LBAR	"\342\225\264"	/* U+2574 BOX DRAWINGS LIGHT LEFT */
    ساکن ثابت حرف bars[] =
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR;
    fprintf (stderr, "%2u %s" VRBAR "%s",
	     level,
	     bars + حجم (bars) - 1 - hb_min ((غیرعلامتی صحیح) حجم (bars) - 1, (غیرعلامتی صحیح) (حجم (VBAR) - 1) * level),
	     level_dir ? (level_dir > 0 ? DLBAR : ULBAR) : LBAR);
  } ورنہ
    fprintf (stderr, "   " VRBAR LBAR);

  _hb_print_func (func);

  اگر (پیغام)
  {
    fprintf (stderr, ": ");
    vfprintf (stderr, پیغام, ap);
  }

  fprintf (stderr, "\n");
}
سانچہ <> اندرونی خالی HB_PRINTF_FUNC(7, 0)
hb_debug_پیغام_va<0> (ثابت حرف *what HB_غیر_مستعمل,
		     ثابت خالی *obj HB_غیر_مستعمل,
		     ثابت حرف *func HB_غیر_مستعمل,
		     منطقی indented HB_غیر_مستعمل,
		     غیرعلامتی صحیح level HB_غیر_مستعمل,
		     صحیح level_dir HB_غیر_مستعمل,
		     ثابت حرف *پیغام HB_غیر_مستعمل,
		     va_list ap HB_غیر_مستعمل) {}

سانچہ <صحیح max_level> ساکن اندرونی خالی
hb_debug_پیغام (ثابت حرف *what,
	       ثابت خالی *obj,
	       ثابت حرف *func,
	       منطقی indented,
	       غیرعلامتی صحیح level,
	       صحیح level_dir,
	       ثابت حرف *پیغام,
	       ...) HB_PRINTF_FUNC(7, 8);
سانچہ <صحیح max_level> ساکن اندرونی خالی HB_PRINTF_FUNC(7, 8)
hb_debug_پیغام (ثابت حرف *what,
	       ثابت خالی *obj,
	       ثابت حرف *func,
	       منطقی indented,
	       غیرعلامتی صحیح level,
	       صحیح level_dir,
	       ثابت حرف *پیغام,
	       ...)
{
  va_list ap;
  va_آغاز (ap, پیغام);
  hb_debug_پیغام_va<max_level> (what, obj, func, indented, level, level_dir, پیغام, ap);
  va_انتہا (ap);
}
سانچہ <> اندرونی خالی
hb_debug_پیغام<0> (ثابت حرف *what HB_غیر_مستعمل,
		  ثابت خالی *obj HB_غیر_مستعمل,
		  ثابت حرف *func HB_غیر_مستعمل,
		  منطقی indented HB_غیر_مستعمل,
		  غیرعلامتی صحیح level HB_غیر_مستعمل,
		  صحیح level_dir HB_غیر_مستعمل,
		  ثابت حرف *پیغام HB_غیر_مستعمل,
		  ...) HB_PRINTF_FUNC(7, 8);
سانچہ <> اندرونی خالی HB_PRINTF_FUNC(7, 8)
hb_debug_پیغام<0> (ثابت حرف *what HB_غیر_مستعمل,
		  ثابت خالی *obj HB_غیر_مستعمل,
		  ثابت حرف *func HB_غیر_مستعمل,
		  منطقی indented HB_غیر_مستعمل,
		  غیرعلامتی صحیح level HB_غیر_مستعمل,
		  صحیح level_dir HB_غیر_مستعمل,
		  ثابت حرف *پیغام HB_غیر_مستعمل,
		  ...) {}

#تعریف_کرو DEBUG_پیغام_LEVEL(WHAT, OBJ, LEVEL, LEVEL_DIR, ...)	hb_debug_پیغام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), خالی_اشاریہ,    سچ, (LEVEL), (LEVEL_DIR), __VA_ARGS__)
#تعریف_کرو DEBUG_پیغام(WHAT, OBJ, ...)				hb_debug_پیغام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), خالی_اشاریہ,    جھوٹ, 0, 0, __VA_ARGS__)
#تعریف_کرو DEBUG_پیغام_FUNC(WHAT, OBJ, ...)				hb_debug_پیغام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), HB_FUNC, جھوٹ, 0, 0, __VA_ARGS__)


/*
 * Printer
 */

سانچہ <قسم_نام T>
ساخت hb_printer_t {
  ثابت حرف *print (ثابت T&) { واپس "something"; }
};

سانچہ <>
ساخت hb_printer_t<منطقی> {
  ثابت حرف *print (منطقی v) { واپس v ? "true" : "false"; }
};

سانچہ <>
ساخت hb_printer_t<hb_خالی_خانے_t> {
  ثابت حرف *print (hb_خالی_خانے_t) { واپس ""; }
};


/*
 * Trace
 */

سانچہ <قسم_نام T>
ساکن اندرونی خالی _hb_warn_no_return (منطقی returned)
{
  اگر (unlikely (!returned)) {
    fprintf (stderr, "OUCH, returned with no call to return_trace().  This is a bug, please report.\n");
  }
}
سانچہ <>
/*static*/ اندرونی خالی _hb_warn_no_return<hb_خالی_خانے_t> (منطقی returned HB_غیر_مستعمل) {}
سانچہ <>
/*static*/ اندرونی خالی _hb_warn_no_return<خالی> (منطقی returned HB_غیر_مستعمل) {}

سانچہ <صحیح max_level, قسم_نام ret_t>
ساخت hb_auto_trace_t
{
  صریح اندرونی hb_auto_trace_t (غیرعلامتی صحیح *plevel_,
				   ثابت حرف *what_,
				   ثابت خالی *obj_,
				   ثابت حرف *func,
				   ثابت حرف *پیغام,
				   ...) HB_PRINTF_FUNC(6, 7)
				   : plevel (plevel_), what (what_), obj (obj_), returned (جھوٹ)
  {
    اگر (plevel) ++*plevel;

    va_list ap;
    va_آغاز (ap, پیغام);
    hb_debug_پیغام_va<max_level> (what, obj, func, سچ, plevel ? *plevel : 0, +1, پیغام, ap);
    va_انتہا (ap);
  }
  ~hb_auto_trace_t ()
  {
    _hb_warn_no_return<ret_t> (returned);
    اگر (!returned) {
      hb_debug_پیغام<max_level> (what, obj, خالی_اشاریہ, سچ, plevel ? *plevel : 1, -1, " ");
    }
    اگر (plevel) --*plevel;
  }

  سانچہ <قسم_نام T>
  T ret (T&& v,
	 ثابت حرف *func = "",
	 غیرعلامتی صحیح سطر = 0)
  {
    اگر (unlikely (returned)) {
      fprintf (stderr, "OUCH, double calls to return_trace().  This is a bug, please report.\n");
      واپس std::forward<T> (v);
    }

    hb_debug_پیغام<max_level> (what, obj, func, سچ, plevel ? *plevel : 1, -1,
			      "return %s (line %u)",
			      hb_printer_t<hb_decay<decltype (v)>>().print (v), سطر);
    اگر (plevel) --*plevel;
    plevel = خالی_اشاریہ;
    returned = سچ;
    واپس std::forward<T> (v);
  }

  نجی:
  غیرعلامتی صحیح *plevel;
  ثابت حرف *what;
  ثابت خالی *obj;
  منطقی returned;
};
سانچہ <قسم_نام ret_t> /* Make sure we don't use hb_auto_trace_t when not tracing. */
ساخت hb_auto_trace_t<0, ret_t>
{
  صریح اندرونی hb_auto_trace_t (غیرعلامتی صحیح *plevel_,
				   ثابت حرف *what_,
				   ثابت خالی *obj_,
				   ثابت حرف *func,
				   ثابت حرف *پیغام,
				   ...) HB_PRINTF_FUNC(6, 7) {}

  سانچہ <قسم_نام T>
  T ret (T&& v,
	 ثابت حرف *func HB_غیر_مستعمل = خالی_اشاریہ,
	 غیرعلامتی صحیح سطر HB_غیر_مستعمل = 0) { واپس std::forward<T> (v); }
};

/* For disabled tracing; optimize out everything.
 * https://github.com/harfbuzz/harfbuzz/pull/605 */
سانچہ <قسم_نام ret_t>
ساخت hb_no_trace_t {
  سانچہ <قسم_نام T>
  T ret (T&& v,
	 ثابت حرف *func HB_غیر_مستعمل = خالی_اشاریہ,
	 غیرعلامتی صحیح سطر HB_غیر_مستعمل = 0) { واپس std::forward<T> (v); }
};

#تعریف_کرو return_trace(RET) واپس trace.ret (RET, HB_FUNC, __LINE__)


/*
 * Instances.
 */

#اگر_غیر_تعریف_شدہ HB_DEBUG_ARABIC
#تعریف_کرو HB_DEBUG_ARABIC (HB_DEBUG+0)
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_DEBUG_BLOB
#تعریف_کرو HB_DEBUG_BLOB (HB_DEBUG+0)
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_DEBUG_CORETEXT
#تعریف_کرو HB_DEBUG_CORETEXT (HB_DEBUG+0)
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_DEBUG_DIRECTWRITE
#تعریف_کرو HB_DEBUG_DIRECTWRITE (HB_DEBUG+0)
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_DEBUG_FT
#تعریف_کرو HB_DEBUG_FT (HB_DEBUG+0)
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_DEBUG_JUSTIFY
#تعریف_کرو HB_DEBUG_JUSTIFY (HB_DEBUG+0)
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_DEBUG_OBJECT
#تعریف_کرو HB_DEBUG_OBJECT (HB_DEBUG+0)
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_DEBUG_SHAPE_PLAN
#تعریف_کرو HB_DEBUG_SHAPE_PLAN (HB_DEBUG+0)
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_DEBUG_UNISCRIBE
#تعریف_کرو HB_DEBUG_UNISCRIBE (HB_DEBUG+0)
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_DEBUG_WASM
#تعریف_کرو HB_DEBUG_WASM (HB_DEBUG+0)
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_DEBUG_KBTS
#تعریف_کرو HB_DEBUG_KBTS (HB_DEBUG+0)
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_DEBUG_DEPEND
#تعریف_کرو HB_DEBUG_DEPEND (HB_DEBUG+0)
#اگر_ختم

/*
 * With tracing.
 */

#اگر_غیر_تعریف_شدہ HB_DEBUG_APPLY
#تعریف_کرو HB_DEBUG_APPLY (HB_DEBUG+0)
#اگر_ختم
#اگر HB_DEBUG_APPLY
#تعریف_کرو TRACE_APPLY(یہ) \
	hb_auto_trace_t<HB_DEBUG_APPLY, منطقی> trace \
	(&c->debug_گہرائی, c->get_name (), یہ, HB_FUNC, \
	 "idx %u gid %u lookup %d", \
	 c->بفر->idx, c->بفر->cur().codepoint, (صحیح) c->lookup_اشاریہ)
#ورنہ
#تعریف_کرو TRACE_APPLY(یہ) hb_no_trace_t<منطقی> trace
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_DEBUG_SANITIZE
#تعریف_کرو HB_DEBUG_SANITIZE (HB_DEBUG+0)
#اگر_ختم
#اگر HB_DEBUG_SANITIZE
#تعریف_کرو TRACE_SANITIZE(یہ) \
	hb_auto_trace_t<HB_DEBUG_SANITIZE, منطقی> trace \
	(&c->debug_گہرائی, c->get_name (), یہ, HB_FUNC, \
	 " ")
#ورنہ
#تعریف_کرو TRACE_SANITIZE(یہ) hb_no_trace_t<منطقی> trace
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_DEBUG_SERIALIZE
#تعریف_کرو HB_DEBUG_SERIALIZE (HB_DEBUG+0)
#اگر_ختم
#اگر HB_DEBUG_SERIALIZE
#تعریف_کرو TRACE_SERIALIZE(یہ) \
	hb_auto_trace_t<HB_DEBUG_SERIALIZE, منطقی> trace \
	(&c->debug_گہرائی, "SERIALIZE", c, HB_FUNC, \
	 " ")
#ورنہ
#تعریف_کرو TRACE_SERIALIZE(یہ) hb_no_trace_t<منطقی> trace
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_DEBUG_SUBSET
#تعریف_کرو HB_DEBUG_SUBSET (HB_DEBUG+0)
#اگر_ختم
#اگر HB_DEBUG_SUBSET
#تعریف_کرو TRACE_SUBSET(یہ) \
  hb_auto_trace_t<HB_DEBUG_SUBSET, منطقی> trace \
  (&c->debug_گہرائی, c->get_name (), یہ, HB_FUNC, \
   " ")
#ورنہ
#تعریف_کرو TRACE_SUBSET(یہ) hb_no_trace_t<منطقی> trace
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_DEBUG_SUBSET_REPACK
#تعریف_کرو HB_DEBUG_SUBSET_REPACK (HB_DEBUG+0)
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_DEBUG_PAINT
#تعریف_کرو HB_DEBUG_PAINT (HB_DEBUG+0)
#اگر_ختم
#اگر HB_DEBUG_PAINT
#تعریف_کرو TRACE_PAINT(یہ) \
  HB_غیر_مستعمل hb_auto_trace_t<HB_DEBUG_PAINT, خالی> trace \
  (&c->debug_گہرائی, c->get_name (), یہ, HB_FUNC, \
   " ")
#ورنہ
#تعریف_کرو TRACE_PAINT(یہ) HB_غیر_مستعمل hb_no_trace_t<خالی> trace
#اگر_ختم


#اگر_غیر_تعریف_شدہ HB_DEBUG_DISPATCH
#تعریف_کرو HB_DEBUG_DISPATCH ( \
	HB_DEBUG_APPLY + \
	HB_DEBUG_SANITIZE + \
	HB_DEBUG_SERIALIZE + \
	HB_DEBUG_SUBSET + \
	HB_DEBUG_PAINT + \
	0)
#اگر_ختم
#اگر HB_DEBUG_DISPATCH
#تعریف_کرو TRACE_DISPATCH(یہ, format) \
	hb_auto_trace_t<context_t::max_debug_گہرائی, قسم_نام context_t::return_t> trace \
	(&c->debug_گہرائی, c->get_name (), یہ, HB_FUNC, \
	 "format %u", (غیرعلامتی) format)
#ورنہ
#تعریف_کرو TRACE_DISPATCH(یہ, format) hb_no_trace_t<قسم_نام context_t::return_t> trace
#اگر_ختم


#اگر_غیر_تعریف_شدہ HB_بفر_پیغام_مزید
#تعریف_کرو HB_بفر_پیغام_مزید (HB_DEBUG+0)
#اگر_ختم


#اگر_ختم /* HB_DEBUG_HH */
