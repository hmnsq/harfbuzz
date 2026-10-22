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

#که_نه_تعریف_شوی HB_DEBUG_HH
#تعریف_کړه HB_DEBUG_HH

#ضمن "hb.hh"
#ضمن "hb-atomic.hh"
#ضمن "hb-algs.hh"


#که_نه_تعریف_شوی HB_DEBUG
#تعریف_کړه HB_DEBUG 0
#د_که_پای


/*
 * Debug output (needs enabling at compile time.)
 */

ساكن مضمن منطقي
_hb_debug (بینښه صحيح level,
	   بینښه صحيح max_level)
{
  أرجع level < max_level;
}

#تعریف_کړه DEBUG_LEVEL_ENABLED(WHAT, LEVEL) (_hb_debug ((LEVEL), HB_DEBUG_##WHAT))
#تعریف_کړه DEBUG_ENABLED(WHAT) (DEBUG_LEVEL_ENABLED (WHAT, 0))

ساكن مضمن فراغ
_hb_print_func (ثابت حرف *func)
{
  إذا (func)
  {
    بینښه صحيح func_len = مزي_اوږدوالی (func);
    /* Skip "static" */
    إذا (0 == مزي_پرتله (func, "static ", 7))
      func += 7;
    /* Skip "typename" */
    إذا (0 == مزي_پرتله (func, "typename ", 9))
      func += 9;
    /* Skip return type */
    ثابت حرف *space = strchr (func, ' ');
    إذا (space)
      func = space + 1;
    /* Skip parameter list */
    ثابت حرف *paren = strchr (func, '(');
    إذا (paren)
      func_len = paren - func;
    fprintf (stderr, "%.*s", (صحيح) func_len, func);
  }
}

کينډۍ <صحيح max_level> ساكن مضمن فراغ
hb_debug_پیغام_va (ثابت حرف *what,
		  ثابت فراغ *obj,
		  ثابت حرف *func,
		  منطقي indented,
		  بینښه صحيح level,
		  صحيح level_dir,
		  ثابت حرف *پیغام,
		  va_list ap) HB_PRINTF_FUNC(7, 0);
کينډۍ <صحيح max_level> ساكن مضمن فراغ
hb_debug_پیغام_va (ثابت حرف *what,
		  ثابت فراغ *obj,
		  ثابت حرف *func,
		  منطقي indented,
		  بینښه صحيح level,
		  صحيح level_dir,
		  ثابت حرف *پیغام,
		  va_list ap)
{
  إذا (!_hb_debug (level, max_level))
    أرجع;

  fprintf (stderr, "%-10s", what ? what : "");

  إذا (obj)
    fprintf (stderr, "(%*p) ", (صحيح) (2 * حجم (فراغ *)), obj);
  وإلا
    fprintf (stderr, " %*s  ", (صحيح) (2 * حجم (فراغ *)), "");

  إذا (indented) {
#تعریف_کړه VBAR	"\342\224\202"	/* U+2502 BOX DRAWINGS LIGHT VERTICAL */
#تعریف_کړه VRBAR	"\342\224\234"	/* U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
#تعریف_کړه DLBAR	"\342\225\256"	/* U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
#تعریف_کړه ULBAR	"\342\225\257"	/* U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
#تعریف_کړه LBAR	"\342\225\264"	/* U+2574 BOX DRAWINGS LIGHT LEFT */
    ساكن ثابت حرف bars[] =
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR;
    fprintf (stderr, "%2u %s" VRBAR "%s",
	     level,
	     bars + حجم (bars) - 1 - hb_min ((بینښه صحيح) حجم (bars) - 1, (بینښه صحيح) (حجم (VBAR) - 1) * level),
	     level_dir ? (level_dir > 0 ? DLBAR : ULBAR) : LBAR);
  } وإلا
    fprintf (stderr, "   " VRBAR LBAR);

  _hb_print_func (func);

  إذا (پیغام)
  {
    fprintf (stderr, ": ");
    vfprintf (stderr, پیغام, ap);
  }

  fprintf (stderr, "\n");
}
کينډۍ <> مضمن فراغ HB_PRINTF_FUNC(7, 0)
hb_debug_پیغام_va<0> (ثابت حرف *what HB_ناکارول_شوی,
		     ثابت فراغ *obj HB_ناکارول_شوی,
		     ثابت حرف *func HB_ناکارول_شوی,
		     منطقي indented HB_ناکارول_شوی,
		     بینښه صحيح level HB_ناکارول_شوی,
		     صحيح level_dir HB_ناکارول_شوی,
		     ثابت حرف *پیغام HB_ناکارول_شوی,
		     va_list ap HB_ناکارول_شوی) {}

کينډۍ <صحيح max_level> ساكن مضمن فراغ
hb_debug_پیغام (ثابت حرف *what,
	       ثابت فراغ *obj,
	       ثابت حرف *func,
	       منطقي indented,
	       بینښه صحيح level,
	       صحيح level_dir,
	       ثابت حرف *پیغام,
	       ...) HB_PRINTF_FUNC(7, 8);
کينډۍ <صحيح max_level> ساكن مضمن فراغ HB_PRINTF_FUNC(7, 8)
hb_debug_پیغام (ثابت حرف *what,
	       ثابت فراغ *obj,
	       ثابت حرف *func,
	       منطقي indented,
	       بینښه صحيح level,
	       صحيح level_dir,
	       ثابت حرف *پیغام,
	       ...)
{
  va_list ap;
  va_پیل (ap, پیغام);
  hb_debug_پیغام_va<max_level> (what, obj, func, indented, level, level_dir, پیغام, ap);
  va_پای_ټکی (ap);
}
کينډۍ <> مضمن فراغ
hb_debug_پیغام<0> (ثابت حرف *what HB_ناکارول_شوی,
		  ثابت فراغ *obj HB_ناکارول_شوی,
		  ثابت حرف *func HB_ناکارول_شوی,
		  منطقي indented HB_ناکارول_شوی,
		  بینښه صحيح level HB_ناکارول_شوی,
		  صحيح level_dir HB_ناکارول_شوی,
		  ثابت حرف *پیغام HB_ناکارول_شوی,
		  ...) HB_PRINTF_FUNC(7, 8);
کينډۍ <> مضمن فراغ HB_PRINTF_FUNC(7, 8)
hb_debug_پیغام<0> (ثابت حرف *what HB_ناکارول_شوی,
		  ثابت فراغ *obj HB_ناکارول_شوی,
		  ثابت حرف *func HB_ناکارول_شوی,
		  منطقي indented HB_ناکارول_شوی,
		  بینښه صحيح level HB_ناکارول_شوی,
		  صحيح level_dir HB_ناکارول_شوی,
		  ثابت حرف *پیغام HB_ناکارول_شوی,
		  ...) {}

#تعریف_کړه DEBUG_پیغام_LEVEL(WHAT, OBJ, LEVEL, LEVEL_DIR, ...)	hb_debug_پیغام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), تش_اشاره,    ریښتیا, (LEVEL), (LEVEL_DIR), __VA_ARGS__)
#تعریف_کړه DEBUG_پیغام(WHAT, OBJ, ...)				hb_debug_پیغام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), تش_اشاره,    خطأ, 0, 0, __VA_ARGS__)
#تعریف_کړه DEBUG_پیغام_FUNC(WHAT, OBJ, ...)				hb_debug_پیغام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), HB_FUNC, خطأ, 0, 0, __VA_ARGS__)


/*
 * Printer
 */

کينډۍ <ډول_نوم T>
بنية hb_printer_t {
  ثابت حرف *print (ثابت T&) { أرجع "something"; }
};

کينډۍ <>
بنية hb_printer_t<منطقي> {
  ثابت حرف *print (منطقي v) { أرجع v ? "true" : "false"; }
};

کينډۍ <>
بنية hb_printer_t<hb_تشه_t> {
  ثابت حرف *print (hb_تشه_t) { أرجع ""; }
};


/*
 * Trace
 */

کينډۍ <ډول_نوم T>
ساكن مضمن فراغ _hb_warn_no_return (منطقي returned)
{
  إذا (unlikely (!returned)) {
    fprintf (stderr, "OUCH, returned with no call to return_trace().  This is a bug, please report.\n");
  }
}
کينډۍ <>
/*static*/ مضمن فراغ _hb_warn_no_return<hb_تشه_t> (منطقي returned HB_ناکارول_شوی) {}
کينډۍ <>
/*static*/ مضمن فراغ _hb_warn_no_return<فراغ> (منطقي returned HB_ناکارول_شوی) {}

کينډۍ <صحيح max_level, ډول_نوم ret_t>
بنية hb_auto_trace_t
{
  څرګند مضمن hb_auto_trace_t (بینښه صحيح *plevel_,
				   ثابت حرف *what_,
				   ثابت فراغ *obj_,
				   ثابت حرف *func,
				   ثابت حرف *پیغام,
				   ...) HB_PRINTF_FUNC(6, 7)
				   : plevel (plevel_), what (what_), obj (obj_), returned (خطأ)
  {
    إذا (plevel) ++*plevel;

    va_list ap;
    va_پیل (ap, پیغام);
    hb_debug_پیغام_va<max_level> (what, obj, func, ریښتیا, plevel ? *plevel : 0, +1, پیغام, ap);
    va_پای_ټکی (ap);
  }
  ~hb_auto_trace_t ()
  {
    _hb_warn_no_return<ret_t> (returned);
    إذا (!returned) {
      hb_debug_پیغام<max_level> (what, obj, تش_اشاره, ریښتیا, plevel ? *plevel : 1, -1, " ");
    }
    إذا (plevel) --*plevel;
  }

  کينډۍ <ډول_نوم T>
  T ret (T&& v,
	 ثابت حرف *func = "",
	 بینښه صحيح کرښه = 0)
  {
    إذا (unlikely (returned)) {
      fprintf (stderr, "OUCH, double calls to return_trace().  This is a bug, please report.\n");
      أرجع std::forward<T> (v);
    }

    hb_debug_پیغام<max_level> (what, obj, func, ریښتیا, plevel ? *plevel : 1, -1,
			      "return %s (line %u)",
			      hb_printer_t<hb_decay<decltype (v)>>().print (v), کرښه);
    إذا (plevel) --*plevel;
    plevel = تش_اشاره;
    returned = ریښتیا;
    أرجع std::forward<T> (v);
  }

  خاص:
  بینښه صحيح *plevel;
  ثابت حرف *what;
  ثابت فراغ *obj;
  منطقي returned;
};
کينډۍ <ډول_نوم ret_t> /* Make sure we don't use hb_auto_trace_t when not tracing. */
بنية hb_auto_trace_t<0, ret_t>
{
  څرګند مضمن hb_auto_trace_t (بینښه صحيح *plevel_,
				   ثابت حرف *what_,
				   ثابت فراغ *obj_,
				   ثابت حرف *func,
				   ثابت حرف *پیغام,
				   ...) HB_PRINTF_FUNC(6, 7) {}

  کينډۍ <ډول_نوم T>
  T ret (T&& v,
	 ثابت حرف *func HB_ناکارول_شوی = تش_اشاره,
	 بینښه صحيح کرښه HB_ناکارول_شوی = 0) { أرجع std::forward<T> (v); }
};

/* For disabled tracing; optimize out everything.
 * https://github.com/harfbuzz/harfbuzz/pull/605 */
کينډۍ <ډول_نوم ret_t>
بنية hb_no_trace_t {
  کينډۍ <ډول_نوم T>
  T ret (T&& v,
	 ثابت حرف *func HB_ناکارول_شوی = تش_اشاره,
	 بینښه صحيح کرښه HB_ناکارول_شوی = 0) { أرجع std::forward<T> (v); }
};

#تعریف_کړه return_trace(RET) أرجع trace.ret (RET, HB_FUNC, __LINE__)


/*
 * Instances.
 */

#که_نه_تعریف_شوی HB_DEBUG_ARABIC
#تعریف_کړه HB_DEBUG_ARABIC (HB_DEBUG+0)
#د_که_پای

#که_نه_تعریف_شوی HB_DEBUG_BLOB
#تعریف_کړه HB_DEBUG_BLOB (HB_DEBUG+0)
#د_که_پای

#که_نه_تعریف_شوی HB_DEBUG_CORETEXT
#تعریف_کړه HB_DEBUG_CORETEXT (HB_DEBUG+0)
#د_که_پای

#که_نه_تعریف_شوی HB_DEBUG_DIRECTWRITE
#تعریف_کړه HB_DEBUG_DIRECTWRITE (HB_DEBUG+0)
#د_که_پای

#که_نه_تعریف_شوی HB_DEBUG_FT
#تعریف_کړه HB_DEBUG_FT (HB_DEBUG+0)
#د_که_پای

#که_نه_تعریف_شوی HB_DEBUG_JUSTIFY
#تعریف_کړه HB_DEBUG_JUSTIFY (HB_DEBUG+0)
#د_که_پای

#که_نه_تعریف_شوی HB_DEBUG_OBJECT
#تعریف_کړه HB_DEBUG_OBJECT (HB_DEBUG+0)
#د_که_پای

#که_نه_تعریف_شوی HB_DEBUG_SHAPE_PLAN
#تعریف_کړه HB_DEBUG_SHAPE_PLAN (HB_DEBUG+0)
#د_که_پای

#که_نه_تعریف_شوی HB_DEBUG_UNISCRIBE
#تعریف_کړه HB_DEBUG_UNISCRIBE (HB_DEBUG+0)
#د_که_پای

#که_نه_تعریف_شوی HB_DEBUG_WASM
#تعریف_کړه HB_DEBUG_WASM (HB_DEBUG+0)
#د_که_پای

#که_نه_تعریف_شوی HB_DEBUG_KBTS
#تعریف_کړه HB_DEBUG_KBTS (HB_DEBUG+0)
#د_که_پای

#که_نه_تعریف_شوی HB_DEBUG_DEPEND
#تعریف_کړه HB_DEBUG_DEPEND (HB_DEBUG+0)
#د_که_پای

/*
 * With tracing.
 */

#که_نه_تعریف_شوی HB_DEBUG_APPLY
#تعریف_کړه HB_DEBUG_APPLY (HB_DEBUG+0)
#د_که_پای
#إذا HB_DEBUG_APPLY
#تعریف_کړه TRACE_APPLY(هذا) \
	hb_auto_trace_t<HB_DEBUG_APPLY, منطقي> trace \
	(&c->debug_ژوروالی, c->get_name (), هذا, HB_FUNC, \
	 "idx %u gid %u lookup %d", \
	 c->بفر->idx, c->بفر->cur().codepoint, (صحيح) c->lookup_شاخص)
#وإلا
#تعریف_کړه TRACE_APPLY(هذا) hb_no_trace_t<منطقي> trace
#د_که_پای

#که_نه_تعریف_شوی HB_DEBUG_SANITIZE
#تعریف_کړه HB_DEBUG_SANITIZE (HB_DEBUG+0)
#د_که_پای
#إذا HB_DEBUG_SANITIZE
#تعریف_کړه TRACE_SANITIZE(هذا) \
	hb_auto_trace_t<HB_DEBUG_SANITIZE, منطقي> trace \
	(&c->debug_ژوروالی, c->get_name (), هذا, HB_FUNC, \
	 " ")
#وإلا
#تعریف_کړه TRACE_SANITIZE(هذا) hb_no_trace_t<منطقي> trace
#د_که_پای

#که_نه_تعریف_شوی HB_DEBUG_SERIALIZE
#تعریف_کړه HB_DEBUG_SERIALIZE (HB_DEBUG+0)
#د_که_پای
#إذا HB_DEBUG_SERIALIZE
#تعریف_کړه TRACE_SERIALIZE(هذا) \
	hb_auto_trace_t<HB_DEBUG_SERIALIZE, منطقي> trace \
	(&c->debug_ژوروالی, "SERIALIZE", c, HB_FUNC, \
	 " ")
#وإلا
#تعریف_کړه TRACE_SERIALIZE(هذا) hb_no_trace_t<منطقي> trace
#د_که_پای

#که_نه_تعریف_شوی HB_DEBUG_SUBSET
#تعریف_کړه HB_DEBUG_SUBSET (HB_DEBUG+0)
#د_که_پای
#إذا HB_DEBUG_SUBSET
#تعریف_کړه TRACE_SUBSET(هذا) \
  hb_auto_trace_t<HB_DEBUG_SUBSET, منطقي> trace \
  (&c->debug_ژوروالی, c->get_name (), هذا, HB_FUNC, \
   " ")
#وإلا
#تعریف_کړه TRACE_SUBSET(هذا) hb_no_trace_t<منطقي> trace
#د_که_پای

#که_نه_تعریف_شوی HB_DEBUG_SUBSET_REPACK
#تعریف_کړه HB_DEBUG_SUBSET_REPACK (HB_DEBUG+0)
#د_که_پای

#که_نه_تعریف_شوی HB_DEBUG_PAINT
#تعریف_کړه HB_DEBUG_PAINT (HB_DEBUG+0)
#د_که_پای
#إذا HB_DEBUG_PAINT
#تعریف_کړه TRACE_PAINT(هذا) \
  HB_ناکارول_شوی hb_auto_trace_t<HB_DEBUG_PAINT, فراغ> trace \
  (&c->debug_ژوروالی, c->get_name (), هذا, HB_FUNC, \
   " ")
#وإلا
#تعریف_کړه TRACE_PAINT(هذا) HB_ناکارول_شوی hb_no_trace_t<فراغ> trace
#د_که_پای


#که_نه_تعریف_شوی HB_DEBUG_DISPATCH
#تعریف_کړه HB_DEBUG_DISPATCH ( \
	HB_DEBUG_APPLY + \
	HB_DEBUG_SANITIZE + \
	HB_DEBUG_SERIALIZE + \
	HB_DEBUG_SUBSET + \
	HB_DEBUG_PAINT + \
	0)
#د_که_پای
#إذا HB_DEBUG_DISPATCH
#تعریف_کړه TRACE_DISPATCH(هذا, format) \
	hb_auto_trace_t<context_t::max_debug_ژوروالی, ډول_نوم context_t::return_t> trace \
	(&c->debug_ژوروالی, c->get_name (), هذا, HB_FUNC, \
	 "format %u", (بینښه) format)
#وإلا
#تعریف_کړه TRACE_DISPATCH(هذا, format) hb_no_trace_t<ډول_نوم context_t::return_t> trace
#د_که_پای


#که_نه_تعریف_شوی HB_بفر_پیغام_نور
#تعریف_کړه HB_بفر_پیغام_نور (HB_DEBUG+0)
#د_که_پای


#د_که_پای /* HB_DEBUG_HH */
