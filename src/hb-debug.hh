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

#شامل "hb.hh"
#شامل "hb-atomic.hh"
#شامل "hb-algs.hh"


#ifndef HB_DEBUG
#define HB_DEBUG 0
#endif


/*
 * Debug output (needs enabling at compile time.)
 */

ایستا درونخطی منطقی
_hb_debug (unsigned صحیح level,
	   unsigned صحیح max_level)
{
  بازگشت level < max_level;
}

#define DEBUG_LEVEL_ENABLED(WHAT, LEVEL) (_hb_debug ((LEVEL), HB_DEBUG_##WHAT))
#define DEBUG_ENABLED(WHAT) (DEBUG_LEVEL_ENABLED (WHAT, 0))

ایستا درونخطی تهی
_hb_print_func (ثابت نویسه *func)
{
  اگر (func)
  {
    unsigned صحیح func_len = strlen (func);
    /* Skip "static" */
    اگر (0 == strncmp (func, "static ", 7))
      func += 7;
    /* Skip "typename" */
    اگر (0 == strncmp (func, "typename ", 9))
      func += 9;
    /* Skip return type */
    ثابت نویسه *space = strchr (func, ' ');
    اگر (space)
      func = space + 1;
    /* Skip parameter list */
    ثابت نویسه *paren = strchr (func, '(');
    اگر (paren)
      func_len = paren - func;
    fprintf (stderr, "%.*s", (صحیح) func_len, func);
  }
}

template <صحیح max_level> ایستا درونخطی تهی
hb_debug_پیام_va (ثابت نویسه *what,
		  ثابت تهی *obj,
		  ثابت نویسه *func,
		  منطقی indented,
		  unsigned صحیح level,
		  صحیح level_dir,
		  ثابت نویسه *message,
		  va_list ap) HB_PRINTF_FUNC(7, 0);
template <صحیح max_level> ایستا درونخطی تهی
hb_debug_پیام_va (ثابت نویسه *what,
		  ثابت تهی *obj,
		  ثابت نویسه *func,
		  منطقی indented,
		  unsigned صحیح level,
		  صحیح level_dir,
		  ثابت نویسه *message,
		  va_list ap)
{
  اگر (!_hb_debug (level, max_level))
    بازگشت;

  fprintf (stderr, "%-10s", what ? what : "");

  اگر (obj)
    fprintf (stderr, "(%*p) ", (صحیح) (2 * اندازه (تهی *)), obj);
  وگرنه
    fprintf (stderr, " %*s  ", (صحیح) (2 * اندازه (تهی *)), "");

  اگر (indented) {
#define VBAR	"\342\224\202"	/* U+2502 BOX DRAWINGS LIGHT VERTICAL */
#define VRBAR	"\342\224\234"	/* U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
#define DLBAR	"\342\225\256"	/* U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
#define ULBAR	"\342\225\257"	/* U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
#define LBAR	"\342\225\264"	/* U+2574 BOX DRAWINGS LIGHT LEFT */
    ایستا ثابت نویسه bars[] =
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR;
    fprintf (stderr, "%2u %s" VRBAR "%s",
	     level,
	     bars + اندازه (bars) - 1 - hb_min ((unsigned صحیح) اندازه (bars) - 1, (unsigned صحیح) (اندازه (VBAR) - 1) * level),
	     level_dir ? (level_dir > 0 ? DLBAR : ULBAR) : LBAR);
  } وگرنه
    fprintf (stderr, "   " VRBAR LBAR);

  _hb_print_func (func);

  اگر (message)
  {
    fprintf (stderr, ": ");
    vfprintf (stderr, message, ap);
  }

  fprintf (stderr, "\n");
}
template <> درونخطی تهی HB_PRINTF_FUNC(7, 0)
hb_debug_پیام_va<0> (ثابت نویسه *what HB_UNUSED,
		     ثابت تهی *obj HB_UNUSED,
		     ثابت نویسه *func HB_UNUSED,
		     منطقی indented HB_UNUSED,
		     unsigned صحیح level HB_UNUSED,
		     صحیح level_dir HB_UNUSED,
		     ثابت نویسه *message HB_UNUSED,
		     va_list ap HB_UNUSED) {}

template <صحیح max_level> ایستا درونخطی تهی
hb_debug_پیام (ثابت نویسه *what,
	       ثابت تهی *obj,
	       ثابت نویسه *func,
	       منطقی indented,
	       unsigned صحیح level,
	       صحیح level_dir,
	       ثابت نویسه *message,
	       ...) HB_PRINTF_FUNC(7, 8);
template <صحیح max_level> ایستا درونخطی تهی HB_PRINTF_FUNC(7, 8)
hb_debug_پیام (ثابت نویسه *what,
	       ثابت تهی *obj,
	       ثابت نویسه *func,
	       منطقی indented,
	       unsigned صحیح level,
	       صحیح level_dir,
	       ثابت نویسه *message,
	       ...)
{
  va_list ap;
  va_start (ap, message);
  hb_debug_پیام_va<max_level> (what, obj, func, indented, level, level_dir, message, ap);
  va_end (ap);
}
template <> درونخطی تهی
hb_debug_پیام<0> (ثابت نویسه *what HB_UNUSED,
		  ثابت تهی *obj HB_UNUSED,
		  ثابت نویسه *func HB_UNUSED,
		  منطقی indented HB_UNUSED,
		  unsigned صحیح level HB_UNUSED,
		  صحیح level_dir HB_UNUSED,
		  ثابت نویسه *message HB_UNUSED,
		  ...) HB_PRINTF_FUNC(7, 8);
template <> درونخطی تهی HB_PRINTF_FUNC(7, 8)
hb_debug_پیام<0> (ثابت نویسه *what HB_UNUSED,
		  ثابت تهی *obj HB_UNUSED,
		  ثابت نویسه *func HB_UNUSED,
		  منطقی indented HB_UNUSED,
		  unsigned صحیح level HB_UNUSED,
		  صحیح level_dir HB_UNUSED,
		  ثابت نویسه *message HB_UNUSED,
		  ...) {}

#define DEBUG_پیام_LEVEL(WHAT, OBJ, LEVEL, LEVEL_DIR, ...)	hb_debug_پیام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    درست, (LEVEL), (LEVEL_DIR), __VA_ARGS__)
#define DEBUG_پیام(WHAT, OBJ, ...)				hb_debug_پیام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    نادرست, 0, 0, __VA_ARGS__)
#define DEBUG_پیام_FUNC(WHAT, OBJ, ...)				hb_debug_پیام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), HB_FUNC, نادرست, 0, 0, __VA_ARGS__)


/*
 * Printer
 */

template <typename T>
ساختار hb_printer_t {
  ثابت نویسه *print (ثابت T&) { بازگشت "something"; }
};

template <>
ساختار hb_printer_t<منطقی> {
  ثابت نویسه *print (منطقی v) { بازگشت v ? "true" : "false"; }
};

template <>
ساختار hb_printer_t<hb_empty_t> {
  ثابت نویسه *print (hb_empty_t) { بازگشت ""; }
};


/*
 * Trace
 */

template <typename T>
ایستا درونخطی تهی _hb_warn_no_return (منطقی returned)
{
  اگر (unlikely (!returned)) {
    fprintf (stderr, "OUCH, returned with no call to return_trace().  This is a bug, please report.\n");
  }
}
template <>
/*static*/ درونخطی تهی _hb_warn_no_return<hb_empty_t> (منطقی returned HB_UNUSED) {}
template <>
/*static*/ درونخطی تهی _hb_warn_no_return<تهی> (منطقی returned HB_UNUSED) {}

template <صحیح max_level, typename ret_t>
ساختار hb_auto_trace_t
{
  explicit درونخطی hb_auto_trace_t (unsigned صحیح *plevel_,
				   ثابت نویسه *what_,
				   ثابت تهی *obj_,
				   ثابت نویسه *func,
				   ثابت نویسه *message,
				   ...) HB_PRINTF_FUNC(6, 7)
				   : plevel (plevel_), what (what_), obj (obj_), returned (نادرست)
  {
    اگر (plevel) ++*plevel;

    va_list ap;
    va_start (ap, message);
    hb_debug_پیام_va<max_level> (what, obj, func, درست, plevel ? *plevel : 0, +1, message, ap);
    va_end (ap);
  }
  ~hb_auto_trace_t ()
  {
    _hb_warn_no_return<ret_t> (returned);
    اگر (!returned) {
      hb_debug_پیام<max_level> (what, obj, nullptr, درست, plevel ? *plevel : 1, -1, " ");
    }
    اگر (plevel) --*plevel;
  }

  template <typename T>
  T ret (T&& v,
	 ثابت نویسه *func = "",
	 unsigned صحیح سطر = 0)
  {
    اگر (unlikely (returned)) {
      fprintf (stderr, "OUCH, double calls to return_trace().  This is a bug, please report.\n");
      بازگشت std::forward<T> (v);
    }

    hb_debug_پیام<max_level> (what, obj, func, درست, plevel ? *plevel : 1, -1,
			      "return %s (line %u)",
			      hb_printer_t<hb_decay<decltype (v)>>().print (v), سطر);
    اگر (plevel) --*plevel;
    plevel = nullptr;
    returned = درست;
    بازگشت std::forward<T> (v);
  }

  خصوصی:
  unsigned صحیح *plevel;
  ثابت نویسه *what;
  ثابت تهی *obj;
  منطقی returned;
};
template <typename ret_t> /* Make sure we don't use hb_auto_trace_t when not tracing. */
ساختار hb_auto_trace_t<0, ret_t>
{
  explicit درونخطی hb_auto_trace_t (unsigned صحیح *plevel_,
				   ثابت نویسه *what_,
				   ثابت تهی *obj_,
				   ثابت نویسه *func,
				   ثابت نویسه *message,
				   ...) HB_PRINTF_FUNC(6, 7) {}

  template <typename T>
  T ret (T&& v,
	 ثابت نویسه *func HB_UNUSED = nullptr,
	 unsigned صحیح سطر HB_UNUSED = 0) { بازگشت std::forward<T> (v); }
};

/* For disabled tracing; optimize out everything.
 * https://github.com/harfbuzz/harfbuzz/pull/605 */
template <typename ret_t>
ساختار hb_no_trace_t {
  template <typename T>
  T ret (T&& v,
	 ثابت نویسه *func HB_UNUSED = nullptr,
	 unsigned صحیح سطر HB_UNUSED = 0) { بازگشت std::forward<T> (v); }
};

#define return_trace(RET) بازگشت trace.ret (RET, HB_FUNC, __LINE__)


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
#اگر HB_DEBUG_APPLY
#define TRACE_APPLY(این) \
	hb_auto_trace_t<HB_DEBUG_APPLY, منطقی> trace \
	(&c->debug_depth, c->get_name (), این, HB_FUNC, \
	 "idx %u gid %u lookup %d", \
	 c->buffer->idx, c->buffer->cur().codepoint, (صحیح) c->lookup_index)
#وگرنه
#define TRACE_APPLY(این) hb_no_trace_t<منطقی> trace
#endif

#ifndef HB_DEBUG_SANITIZE
#define HB_DEBUG_SANITIZE (HB_DEBUG+0)
#endif
#اگر HB_DEBUG_SANITIZE
#define TRACE_SANITIZE(این) \
	hb_auto_trace_t<HB_DEBUG_SANITIZE, منطقی> trace \
	(&c->debug_depth, c->get_name (), این, HB_FUNC, \
	 " ")
#وگرنه
#define TRACE_SANITIZE(این) hb_no_trace_t<منطقی> trace
#endif

#ifndef HB_DEBUG_SERIALIZE
#define HB_DEBUG_SERIALIZE (HB_DEBUG+0)
#endif
#اگر HB_DEBUG_SERIALIZE
#define TRACE_SERIALIZE(این) \
	hb_auto_trace_t<HB_DEBUG_SERIALIZE, منطقی> trace \
	(&c->debug_depth, "SERIALIZE", c, HB_FUNC, \
	 " ")
#وگرنه
#define TRACE_SERIALIZE(این) hb_no_trace_t<منطقی> trace
#endif

#ifndef HB_DEBUG_SUBSET
#define HB_DEBUG_SUBSET (HB_DEBUG+0)
#endif
#اگر HB_DEBUG_SUBSET
#define TRACE_SUBSET(این) \
  hb_auto_trace_t<HB_DEBUG_SUBSET, منطقی> trace \
  (&c->debug_depth, c->get_name (), این, HB_FUNC, \
   " ")
#وگرنه
#define TRACE_SUBSET(این) hb_no_trace_t<منطقی> trace
#endif

#ifndef HB_DEBUG_SUBSET_REPACK
#define HB_DEBUG_SUBSET_REPACK (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_PAINT
#define HB_DEBUG_PAINT (HB_DEBUG+0)
#endif
#اگر HB_DEBUG_PAINT
#define TRACE_PAINT(این) \
  HB_UNUSED hb_auto_trace_t<HB_DEBUG_PAINT, تهی> trace \
  (&c->debug_depth, c->get_name (), این, HB_FUNC, \
   " ")
#وگرنه
#define TRACE_PAINT(این) HB_UNUSED hb_no_trace_t<تهی> trace
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
#اگر HB_DEBUG_DISPATCH
#define TRACE_DISPATCH(این, format) \
	hb_auto_trace_t<context_t::max_debug_depth, typename context_t::return_t> trace \
	(&c->debug_depth, c->get_name (), این, HB_FUNC, \
	 "format %u", (unsigned) format)
#وگرنه
#define TRACE_DISPATCH(این, format) hb_no_trace_t<typename context_t::return_t> trace
#endif


#ifndef HB_BUFFER_MESSAGE_MORE
#define HB_BUFFER_MESSAGE_MORE (HB_DEBUG+0)
#endif


#endif /* HB_DEBUG_HH */
