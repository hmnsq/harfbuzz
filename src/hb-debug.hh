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

#ifndef HB_DEBUG_HH
#define HB_DEBUG_HH

#ضمن "hb.hh"
#ضمن "hb-atomic.hh"
#ضمن "hb-algs.hh"


#ifndef HB_DEBUG
#define HB_DEBUG 0
#endif


/*
 * Debug output (needs enabling at compile time.)
 */

ساكن مضمن منطقي
_hb_debug (unsigned صحيح level,
	   unsigned صحيح max_level)
{
  أرجع level < max_level;
}

#define DEBUG_LEVEL_ENABLED(WHAT, LEVEL) (_hb_debug ((LEVEL), HB_DEBUG_##WHAT))
#define DEBUG_ENABLED(WHAT) (DEBUG_LEVEL_ENABLED (WHAT, 0))

ساكن مضمن فراغ
_hb_print_func (ثابت حرف *func)
{
  إذا (func)
  {
    unsigned صحيح func_len = strlen (func);
    /* Skip "static" */
    إذا (0 == strncmp (func, "static ", 7))
      func += 7;
    /* Skip "typename" */
    إذا (0 == strncmp (func, "typename ", 9))
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

template <صحيح max_level> ساكن مضمن فراغ
hb_debug_پیغام_va (ثابت حرف *what,
		  ثابت فراغ *obj,
		  ثابت حرف *func,
		  منطقي indented,
		  unsigned صحيح level,
		  صحيح level_dir,
		  ثابت حرف *message,
		  va_list ap) HB_PRINTF_FUNC(7, 0);
template <صحيح max_level> ساكن مضمن فراغ
hb_debug_پیغام_va (ثابت حرف *what,
		  ثابت فراغ *obj,
		  ثابت حرف *func,
		  منطقي indented,
		  unsigned صحيح level,
		  صحيح level_dir,
		  ثابت حرف *message,
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
#define VBAR	"\342\224\202"	/* U+2502 BOX DRAWINGS LIGHT VERTICAL */
#define VRBAR	"\342\224\234"	/* U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
#define DLBAR	"\342\225\256"	/* U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
#define ULBAR	"\342\225\257"	/* U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
#define LBAR	"\342\225\264"	/* U+2574 BOX DRAWINGS LIGHT LEFT */
    ساكن ثابت حرف bars[] =
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR;
    fprintf (stderr, "%2u %s" VRBAR "%s",
	     level,
	     bars + حجم (bars) - 1 - hb_min ((unsigned صحيح) حجم (bars) - 1, (unsigned صحيح) (حجم (VBAR) - 1) * level),
	     level_dir ? (level_dir > 0 ? DLBAR : ULBAR) : LBAR);
  } وإلا
    fprintf (stderr, "   " VRBAR LBAR);

  _hb_print_func (func);

  إذا (message)
  {
    fprintf (stderr, ": ");
    vfprintf (stderr, message, ap);
  }

  fprintf (stderr, "\n");
}
template <> مضمن فراغ HB_PRINTF_FUNC(7, 0)
hb_debug_پیغام_va<0> (ثابت حرف *what HB_UNUSED,
		     ثابت فراغ *obj HB_UNUSED,
		     ثابت حرف *func HB_UNUSED,
		     منطقي indented HB_UNUSED,
		     unsigned صحيح level HB_UNUSED,
		     صحيح level_dir HB_UNUSED,
		     ثابت حرف *message HB_UNUSED,
		     va_list ap HB_UNUSED) {}

template <صحيح max_level> ساكن مضمن فراغ
hb_debug_پیغام (ثابت حرف *what,
	       ثابت فراغ *obj,
	       ثابت حرف *func,
	       منطقي indented,
	       unsigned صحيح level,
	       صحيح level_dir,
	       ثابت حرف *message,
	       ...) HB_PRINTF_FUNC(7, 8);
template <صحيح max_level> ساكن مضمن فراغ HB_PRINTF_FUNC(7, 8)
hb_debug_پیغام (ثابت حرف *what,
	       ثابت فراغ *obj,
	       ثابت حرف *func,
	       منطقي indented,
	       unsigned صحيح level,
	       صحيح level_dir,
	       ثابت حرف *message,
	       ...)
{
  va_list ap;
  va_start (ap, message);
  hb_debug_پیغام_va<max_level> (what, obj, func, indented, level, level_dir, message, ap);
  va_end (ap);
}
template <> مضمن فراغ
hb_debug_پیغام<0> (ثابت حرف *what HB_UNUSED,
		  ثابت فراغ *obj HB_UNUSED,
		  ثابت حرف *func HB_UNUSED,
		  منطقي indented HB_UNUSED,
		  unsigned صحيح level HB_UNUSED,
		  صحيح level_dir HB_UNUSED,
		  ثابت حرف *message HB_UNUSED,
		  ...) HB_PRINTF_FUNC(7, 8);
template <> مضمن فراغ HB_PRINTF_FUNC(7, 8)
hb_debug_پیغام<0> (ثابت حرف *what HB_UNUSED,
		  ثابت فراغ *obj HB_UNUSED,
		  ثابت حرف *func HB_UNUSED,
		  منطقي indented HB_UNUSED,
		  unsigned صحيح level HB_UNUSED,
		  صحيح level_dir HB_UNUSED,
		  ثابت حرف *message HB_UNUSED,
		  ...) {}

#define DEBUG_پیغام_LEVEL(WHAT, OBJ, LEVEL, LEVEL_DIR, ...)	hb_debug_پیغام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    true, (LEVEL), (LEVEL_DIR), __VA_ARGS__)
#define DEBUG_پیغام(WHAT, OBJ, ...)				hb_debug_پیغام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    خطأ, 0, 0, __VA_ARGS__)
#define DEBUG_پیغام_FUNC(WHAT, OBJ, ...)				hb_debug_پیغام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), HB_FUNC, خطأ, 0, 0, __VA_ARGS__)


/*
 * Printer
 */

template <typename T>
بنية hb_printer_t {
  ثابت حرف *print (ثابت T&) { أرجع "something"; }
};

template <>
بنية hb_printer_t<منطقي> {
  ثابت حرف *print (منطقي v) { أرجع v ? "true" : "false"; }
};

template <>
بنية hb_printer_t<hb_empty_t> {
  ثابت حرف *print (hb_empty_t) { أرجع ""; }
};


/*
 * Trace
 */

template <typename T>
ساكن مضمن فراغ _hb_warn_no_return (منطقي returned)
{
  إذا (unlikely (!returned)) {
    fprintf (stderr, "OUCH, returned with no call to return_trace().  This is a bug, please report.\n");
  }
}
template <>
/*static*/ مضمن فراغ _hb_warn_no_return<hb_empty_t> (منطقي returned HB_UNUSED) {}
template <>
/*static*/ مضمن فراغ _hb_warn_no_return<فراغ> (منطقي returned HB_UNUSED) {}

template <صحيح max_level, typename ret_t>
بنية hb_auto_trace_t
{
  explicit مضمن hb_auto_trace_t (unsigned صحيح *plevel_,
				   ثابت حرف *what_,
				   ثابت فراغ *obj_,
				   ثابت حرف *func,
				   ثابت حرف *message,
				   ...) HB_PRINTF_FUNC(6, 7)
				   : plevel (plevel_), what (what_), obj (obj_), returned (خطأ)
  {
    إذا (plevel) ++*plevel;

    va_list ap;
    va_start (ap, message);
    hb_debug_پیغام_va<max_level> (what, obj, func, true, plevel ? *plevel : 0, +1, message, ap);
    va_end (ap);
  }
  ~hb_auto_trace_t ()
  {
    _hb_warn_no_return<ret_t> (returned);
    إذا (!returned) {
      hb_debug_پیغام<max_level> (what, obj, nullptr, true, plevel ? *plevel : 1, -1, " ");
    }
    إذا (plevel) --*plevel;
  }

  template <typename T>
  T ret (T&& v,
	 ثابت حرف *func = "",
	 unsigned صحيح سطر = 0)
  {
    إذا (unlikely (returned)) {
      fprintf (stderr, "OUCH, double calls to return_trace().  This is a bug, please report.\n");
      أرجع std::forward<T> (v);
    }

    hb_debug_پیغام<max_level> (what, obj, func, true, plevel ? *plevel : 1, -1,
			      "return %s (line %u)",
			      hb_printer_t<hb_decay<decltype (v)>>().print (v), سطر);
    إذا (plevel) --*plevel;
    plevel = nullptr;
    returned = true;
    أرجع std::forward<T> (v);
  }

  خاص:
  unsigned صحيح *plevel;
  ثابت حرف *what;
  ثابت فراغ *obj;
  منطقي returned;
};
template <typename ret_t> /* Make sure we don't use hb_auto_trace_t when not tracing. */
بنية hb_auto_trace_t<0, ret_t>
{
  explicit مضمن hb_auto_trace_t (unsigned صحيح *plevel_,
				   ثابت حرف *what_,
				   ثابت فراغ *obj_,
				   ثابت حرف *func,
				   ثابت حرف *message,
				   ...) HB_PRINTF_FUNC(6, 7) {}

  template <typename T>
  T ret (T&& v,
	 ثابت حرف *func HB_UNUSED = nullptr,
	 unsigned صحيح سطر HB_UNUSED = 0) { أرجع std::forward<T> (v); }
};

/* For disabled tracing; optimize out everything.
 * https://github.com/harfbuzz/harfbuzz/pull/605 */
template <typename ret_t>
بنية hb_no_trace_t {
  template <typename T>
  T ret (T&& v,
	 ثابت حرف *func HB_UNUSED = nullptr,
	 unsigned صحيح سطر HB_UNUSED = 0) { أرجع std::forward<T> (v); }
};

#define return_trace(RET) أرجع trace.ret (RET, HB_FUNC, __LINE__)


/*
 * Instances.
 */

#ifndef HB_DEBUG_ARABIC
#define HB_DEBUG_ARABIC (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_BLOB
#define HB_DEBUG_BLOB (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_CORETEXT
#define HB_DEBUG_CORETEXT (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_DIRECTWRITE
#define HB_DEBUG_DIRECTWRITE (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_FT
#define HB_DEBUG_FT (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_JUSTIFY
#define HB_DEBUG_JUSTIFY (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_OBJECT
#define HB_DEBUG_OBJECT (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_SHAPE_PLAN
#define HB_DEBUG_SHAPE_PLAN (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_UNISCRIBE
#define HB_DEBUG_UNISCRIBE (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_WASM
#define HB_DEBUG_WASM (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_KBTS
#define HB_DEBUG_KBTS (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_DEPEND
#define HB_DEBUG_DEPEND (HB_DEBUG+0)
#endif

/*
 * With tracing.
 */

#ifndef HB_DEBUG_APPLY
#define HB_DEBUG_APPLY (HB_DEBUG+0)
#endif
#إذا HB_DEBUG_APPLY
#define TRACE_APPLY(هذا) \
	hb_auto_trace_t<HB_DEBUG_APPLY, منطقي> trace \
	(&c->debug_depth, c->get_name (), هذا, HB_FUNC, \
	 "idx %u gid %u lookup %d", \
	 c->buffer->idx, c->buffer->cur().codepoint, (صحيح) c->lookup_index)
#وإلا
#define TRACE_APPLY(هذا) hb_no_trace_t<منطقي> trace
#endif

#ifndef HB_DEBUG_SANITIZE
#define HB_DEBUG_SANITIZE (HB_DEBUG+0)
#endif
#إذا HB_DEBUG_SANITIZE
#define TRACE_SANITIZE(هذا) \
	hb_auto_trace_t<HB_DEBUG_SANITIZE, منطقي> trace \
	(&c->debug_depth, c->get_name (), هذا, HB_FUNC, \
	 " ")
#وإلا
#define TRACE_SANITIZE(هذا) hb_no_trace_t<منطقي> trace
#endif

#ifndef HB_DEBUG_SERIALIZE
#define HB_DEBUG_SERIALIZE (HB_DEBUG+0)
#endif
#إذا HB_DEBUG_SERIALIZE
#define TRACE_SERIALIZE(هذا) \
	hb_auto_trace_t<HB_DEBUG_SERIALIZE, منطقي> trace \
	(&c->debug_depth, "SERIALIZE", c, HB_FUNC, \
	 " ")
#وإلا
#define TRACE_SERIALIZE(هذا) hb_no_trace_t<منطقي> trace
#endif

#ifndef HB_DEBUG_SUBSET
#define HB_DEBUG_SUBSET (HB_DEBUG+0)
#endif
#إذا HB_DEBUG_SUBSET
#define TRACE_SUBSET(هذا) \
  hb_auto_trace_t<HB_DEBUG_SUBSET, منطقي> trace \
  (&c->debug_depth, c->get_name (), هذا, HB_FUNC, \
   " ")
#وإلا
#define TRACE_SUBSET(هذا) hb_no_trace_t<منطقي> trace
#endif

#ifndef HB_DEBUG_SUBSET_REPACK
#define HB_DEBUG_SUBSET_REPACK (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_PAINT
#define HB_DEBUG_PAINT (HB_DEBUG+0)
#endif
#إذا HB_DEBUG_PAINT
#define TRACE_PAINT(هذا) \
  HB_UNUSED hb_auto_trace_t<HB_DEBUG_PAINT, فراغ> trace \
  (&c->debug_depth, c->get_name (), هذا, HB_FUNC, \
   " ")
#وإلا
#define TRACE_PAINT(هذا) HB_UNUSED hb_no_trace_t<فراغ> trace
#endif


#ifndef HB_DEBUG_DISPATCH
#define HB_DEBUG_DISPATCH ( \
	HB_DEBUG_APPLY + \
	HB_DEBUG_SANITIZE + \
	HB_DEBUG_SERIALIZE + \
	HB_DEBUG_SUBSET + \
	HB_DEBUG_PAINT + \
	0)
#endif
#إذا HB_DEBUG_DISPATCH
#define TRACE_DISPATCH(هذا, format) \
	hb_auto_trace_t<context_t::max_debug_depth, typename context_t::return_t> trace \
	(&c->debug_depth, c->get_name (), هذا, HB_FUNC, \
	 "format %u", (unsigned) format)
#وإلا
#define TRACE_DISPATCH(هذا, format) hb_no_trace_t<typename context_t::return_t> trace
#endif


#ifndef HB_BUFFER_MESSAGE_MORE
#define HB_BUFFER_MESSAGE_MORE (HB_DEBUG+0)
#endif


#endif /* HB_DEBUG_HH */
