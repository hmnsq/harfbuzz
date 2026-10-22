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

#অন্তর্ভাৱ "hb.hh"
#অন্তর্ভাৱ "hb-atomic.hh"
#অন্তর্ভাৱ "hb-algs.hh"


#ifndef HB_DEBUG
#define HB_DEBUG 0
#endif


/*
 * Debug output (needs enabling at compile time.)
 */

স্থির অন্তঃস্থ তার্কিক
_hb_debug (unsigned পূর্ণাঙ্ক level,
	   unsigned পূর্ণাঙ্ক max_level)
{
  ঘূৰাই level < max_level;
}

#define DEBUG_LEVEL_ENABLED(WHAT, LEVEL) (_hb_debug ((LEVEL), HB_DEBUG_##WHAT))
#define DEBUG_ENABLED(WHAT) (DEBUG_LEVEL_ENABLED (WHAT, 0))

স্থির অন্তঃস্থ শূন্য
_hb_print_func (ধ্রুৱ ৱর্ণ *func)
{
  যদি (func)
  {
    unsigned পূর্ণাঙ্ক func_len = strlen (func);
    /* Skip "static" */
    যদি (0 == strncmp (func, "static ", 7))
      func += 7;
    /* Skip "typename" */
    যদি (0 == strncmp (func, "typename ", 9))
      func += 9;
    /* Skip return type */
    ধ্রুৱ ৱর্ণ *space = strchr (func, ' ');
    যদি (space)
      func = space + 1;
    /* Skip parameter list */
    ধ্রুৱ ৱর্ণ *paren = strchr (func, '(');
    যদি (paren)
      func_len = paren - func;
    fprintf (stderr, "%.*s", (পূর্ণাঙ্ক) func_len, func);
  }
}

template <পূর্ণাঙ্ক max_level> স্থির অন্তঃস্থ শূন্য
hb_debug_বাৰ্তা_va (ধ্রুৱ ৱর্ণ *what,
		  ধ্রুৱ শূন্য *obj,
		  ধ্রুৱ ৱর্ণ *func,
		  তার্কিক indented,
		  unsigned পূর্ণাঙ্ক level,
		  পূর্ণাঙ্ক level_dir,
		  ধ্রুৱ ৱর্ণ *message,
		  va_list ap) HB_PRINTF_FUNC(7, 0);
template <পূর্ণাঙ্ক max_level> স্থির অন্তঃস্থ শূন্য
hb_debug_বাৰ্তা_va (ধ্রুৱ ৱর্ণ *what,
		  ধ্রুৱ শূন্য *obj,
		  ধ্রুৱ ৱর্ণ *func,
		  তার্কিক indented,
		  unsigned পূর্ণাঙ্ক level,
		  পূর্ণাঙ্ক level_dir,
		  ধ্রুৱ ৱর্ণ *message,
		  va_list ap)
{
  যদি (!_hb_debug (level, max_level))
    ঘূৰাই;

  fprintf (stderr, "%-10s", what ? what : "");

  যদি (obj)
    fprintf (stderr, "(%*p) ", (পূর্ণাঙ্ক) (2 * পরিমাণ (শূন্য *)), obj);
  অন্যথা
    fprintf (stderr, " %*s  ", (পূর্ণাঙ্ক) (2 * পরিমাণ (শূন্য *)), "");

  যদি (indented) {
#define VBAR	"\342\224\202"	/* U+2502 BOX DRAWINGS LIGHT VERTICAL */
#define VRBAR	"\342\224\234"	/* U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
#define DLBAR	"\342\225\256"	/* U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
#define ULBAR	"\342\225\257"	/* U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
#define LBAR	"\342\225\264"	/* U+2574 BOX DRAWINGS LIGHT LEFT */
    স্থির ধ্রুৱ ৱর্ণ bars[] =
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR;
    fprintf (stderr, "%2u %s" VRBAR "%s",
	     level,
	     bars + পরিমাণ (bars) - 1 - hb_min ((unsigned পূর্ণাঙ্ক) পরিমাণ (bars) - 1, (unsigned পূর্ণাঙ্ক) (পরিমাণ (VBAR) - 1) * level),
	     level_dir ? (level_dir > 0 ? DLBAR : ULBAR) : LBAR);
  } অন্যথা
    fprintf (stderr, "   " VRBAR LBAR);

  _hb_print_func (func);

  যদি (message)
  {
    fprintf (stderr, ": ");
    vfprintf (stderr, message, ap);
  }

  fprintf (stderr, "\n");
}
template <> অন্তঃস্থ শূন্য HB_PRINTF_FUNC(7, 0)
hb_debug_বাৰ্তা_va<0> (ধ্রুৱ ৱর্ণ *what HB_UNUSED,
		     ধ্রুৱ শূন্য *obj HB_UNUSED,
		     ধ্রুৱ ৱর্ণ *func HB_UNUSED,
		     তার্কিক indented HB_UNUSED,
		     unsigned পূর্ণাঙ্ক level HB_UNUSED,
		     পূর্ণাঙ্ক level_dir HB_UNUSED,
		     ধ্রুৱ ৱর্ণ *message HB_UNUSED,
		     va_list ap HB_UNUSED) {}

template <পূর্ণাঙ্ক max_level> স্থির অন্তঃস্থ শূন্য
hb_debug_বাৰ্তা (ধ্রুৱ ৱর্ণ *what,
	       ধ্রুৱ শূন্য *obj,
	       ধ্রুৱ ৱর্ণ *func,
	       তার্কিক indented,
	       unsigned পূর্ণাঙ্ক level,
	       পূর্ণাঙ্ক level_dir,
	       ধ্রুৱ ৱর্ণ *message,
	       ...) HB_PRINTF_FUNC(7, 8);
template <পূর্ণাঙ্ক max_level> স্থির অন্তঃস্থ শূন্য HB_PRINTF_FUNC(7, 8)
hb_debug_বাৰ্তা (ধ্রুৱ ৱর্ণ *what,
	       ধ্রুৱ শূন্য *obj,
	       ধ্রুৱ ৱর্ণ *func,
	       তার্কিক indented,
	       unsigned পূর্ণাঙ্ক level,
	       পূর্ণাঙ্ক level_dir,
	       ধ্রুৱ ৱর্ণ *message,
	       ...)
{
  va_list ap;
  va_start (ap, message);
  hb_debug_বাৰ্তা_va<max_level> (what, obj, func, indented, level, level_dir, message, ap);
  va_end (ap);
}
template <> অন্তঃস্থ শূন্য
hb_debug_বাৰ্তা<0> (ধ্রুৱ ৱর্ণ *what HB_UNUSED,
		  ধ্রুৱ শূন্য *obj HB_UNUSED,
		  ধ্রুৱ ৱর্ণ *func HB_UNUSED,
		  তার্কিক indented HB_UNUSED,
		  unsigned পূর্ণাঙ্ক level HB_UNUSED,
		  পূর্ণাঙ্ক level_dir HB_UNUSED,
		  ধ্রুৱ ৱর্ণ *message HB_UNUSED,
		  ...) HB_PRINTF_FUNC(7, 8);
template <> অন্তঃস্থ শূন্য HB_PRINTF_FUNC(7, 8)
hb_debug_বাৰ্তা<0> (ধ্রুৱ ৱর্ণ *what HB_UNUSED,
		  ধ্রুৱ শূন্য *obj HB_UNUSED,
		  ধ্রুৱ ৱর্ণ *func HB_UNUSED,
		  তার্কিক indented HB_UNUSED,
		  unsigned পূর্ণাঙ্ক level HB_UNUSED,
		  পূর্ণাঙ্ক level_dir HB_UNUSED,
		  ধ্রুৱ ৱর্ণ *message HB_UNUSED,
		  ...) {}

#define DEBUG_বাৰ্তা_LEVEL(WHAT, OBJ, LEVEL, LEVEL_DIR, ...)	hb_debug_বাৰ্তা<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    সঁচা, (LEVEL), (LEVEL_DIR), __VA_ARGS__)
#define DEBUG_বাৰ্তা(WHAT, OBJ, ...)				hb_debug_বাৰ্তা<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    মিছা, 0, 0, __VA_ARGS__)
#define DEBUG_বাৰ্তা_FUNC(WHAT, OBJ, ...)				hb_debug_বাৰ্তা<HB_DEBUG_##WHAT> (#WHAT, (OBJ), HB_FUNC, মিছা, 0, 0, __VA_ARGS__)


/*
 * Printer
 */

template <typename T>
সংরচনা hb_printer_t {
  ধ্রুৱ ৱর্ণ *print (ধ্রুৱ T&) { ঘূৰাই "something"; }
};

template <>
সংরচনা hb_printer_t<তার্কিক> {
  ধ্রুৱ ৱর্ণ *print (তার্কিক v) { ঘূৰাই v ? "true" : "false"; }
};

template <>
সংরচনা hb_printer_t<hb_empty_t> {
  ধ্রুৱ ৱর্ণ *print (hb_empty_t) { ঘূৰাই ""; }
};


/*
 * Trace
 */

template <typename T>
স্থির অন্তঃস্থ শূন্য _hb_warn_no_return (তার্কিক returned)
{
  যদি (unlikely (!returned)) {
    fprintf (stderr, "OUCH, returned with no call to return_trace().  This is a bug, please report.\n");
  }
}
template <>
/*static*/ অন্তঃস্থ শূন্য _hb_warn_no_return<hb_empty_t> (তার্কিক returned HB_UNUSED) {}
template <>
/*static*/ অন্তঃস্থ শূন্য _hb_warn_no_return<শূন্য> (তার্কিক returned HB_UNUSED) {}

template <পূর্ণাঙ্ক max_level, typename ret_t>
সংরচনা hb_auto_trace_t
{
  explicit অন্তঃস্থ hb_auto_trace_t (unsigned পূর্ণাঙ্ক *plevel_,
				   ধ্রুৱ ৱর্ণ *what_,
				   ধ্রুৱ শূন্য *obj_,
				   ধ্রুৱ ৱর্ণ *func,
				   ধ্রুৱ ৱর্ণ *message,
				   ...) HB_PRINTF_FUNC(6, 7)
				   : plevel (plevel_), what (what_), obj (obj_), returned (মিছা)
  {
    যদি (plevel) ++*plevel;

    va_list ap;
    va_start (ap, message);
    hb_debug_বাৰ্তা_va<max_level> (what, obj, func, সঁচা, plevel ? *plevel : 0, +1, message, ap);
    va_end (ap);
  }
  ~hb_auto_trace_t ()
  {
    _hb_warn_no_return<ret_t> (returned);
    যদি (!returned) {
      hb_debug_বাৰ্তা<max_level> (what, obj, nullptr, সঁচা, plevel ? *plevel : 1, -1, " ");
    }
    যদি (plevel) --*plevel;
  }

  template <typename T>
  T ret (T&& v,
	 ধ্রুৱ ৱর্ণ *func = "",
	 unsigned পূর্ণাঙ্ক সাৰি = 0)
  {
    যদি (unlikely (returned)) {
      fprintf (stderr, "OUCH, double calls to return_trace().  This is a bug, please report.\n");
      ঘূৰাই std::forward<T> (v);
    }

    hb_debug_বাৰ্তা<max_level> (what, obj, func, সঁচা, plevel ? *plevel : 1, -1,
			      "return %s (line %u)",
			      hb_printer_t<hb_decay<decltype (v)>>().print (v), সাৰি);
    যদি (plevel) --*plevel;
    plevel = nullptr;
    returned = সঁচা;
    ঘূৰাই std::forward<T> (v);
  }

  গুপ্ত:
  unsigned পূর্ণাঙ্ক *plevel;
  ধ্রুৱ ৱর্ণ *what;
  ধ্রুৱ শূন্য *obj;
  তার্কিক returned;
};
template <typename ret_t> /* Make sure we don't use hb_auto_trace_t when not tracing. */
সংরচনা hb_auto_trace_t<0, ret_t>
{
  explicit অন্তঃস্থ hb_auto_trace_t (unsigned পূর্ণাঙ্ক *plevel_,
				   ধ্রুৱ ৱর্ণ *what_,
				   ধ্রুৱ শূন্য *obj_,
				   ধ্রুৱ ৱর্ণ *func,
				   ধ্রুৱ ৱর্ণ *message,
				   ...) HB_PRINTF_FUNC(6, 7) {}

  template <typename T>
  T ret (T&& v,
	 ধ্রুৱ ৱর্ণ *func HB_UNUSED = nullptr,
	 unsigned পূর্ণাঙ্ক সাৰি HB_UNUSED = 0) { ঘূৰাই std::forward<T> (v); }
};

/* For disabled tracing; optimize out everything.
 * https://github.com/harfbuzz/harfbuzz/pull/605 */
template <typename ret_t>
সংরচনা hb_no_trace_t {
  template <typename T>
  T ret (T&& v,
	 ধ্রুৱ ৱর্ণ *func HB_UNUSED = nullptr,
	 unsigned পূর্ণাঙ্ক সাৰি HB_UNUSED = 0) { ঘূৰাই std::forward<T> (v); }
};

#define return_trace(RET) ঘূৰাই trace.ret (RET, HB_FUNC, __LINE__)


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
#যদি HB_DEBUG_APPLY
#define TRACE_APPLY(এতত্) \
	hb_auto_trace_t<HB_DEBUG_APPLY, তার্কিক> trace \
	(&c->debug_depth, c->get_name (), এতত্, HB_FUNC, \
	 "idx %u gid %u lookup %d", \
	 c->buffer->idx, c->buffer->cur().codepoint, (পূর্ণাঙ্ক) c->lookup_index)
#অন্যথা
#define TRACE_APPLY(এতত্) hb_no_trace_t<তার্কিক> trace
#endif

#ifndef HB_DEBUG_SANITIZE
#define HB_DEBUG_SANITIZE (HB_DEBUG+0)
#endif
#যদি HB_DEBUG_SANITIZE
#define TRACE_SANITIZE(এতত্) \
	hb_auto_trace_t<HB_DEBUG_SANITIZE, তার্কিক> trace \
	(&c->debug_depth, c->get_name (), এতত্, HB_FUNC, \
	 " ")
#অন্যথা
#define TRACE_SANITIZE(এতত্) hb_no_trace_t<তার্কিক> trace
#endif

#ifndef HB_DEBUG_SERIALIZE
#define HB_DEBUG_SERIALIZE (HB_DEBUG+0)
#endif
#যদি HB_DEBUG_SERIALIZE
#define TRACE_SERIALIZE(এতত্) \
	hb_auto_trace_t<HB_DEBUG_SERIALIZE, তার্কিক> trace \
	(&c->debug_depth, "SERIALIZE", c, HB_FUNC, \
	 " ")
#অন্যথা
#define TRACE_SERIALIZE(এতত্) hb_no_trace_t<তার্কিক> trace
#endif

#ifndef HB_DEBUG_SUBSET
#define HB_DEBUG_SUBSET (HB_DEBUG+0)
#endif
#যদি HB_DEBUG_SUBSET
#define TRACE_SUBSET(এতত্) \
  hb_auto_trace_t<HB_DEBUG_SUBSET, তার্কিক> trace \
  (&c->debug_depth, c->get_name (), এতত্, HB_FUNC, \
   " ")
#অন্যথা
#define TRACE_SUBSET(এতত্) hb_no_trace_t<তার্কিক> trace
#endif

#ifndef HB_DEBUG_SUBSET_REPACK
#define HB_DEBUG_SUBSET_REPACK (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_PAINT
#define HB_DEBUG_PAINT (HB_DEBUG+0)
#endif
#যদি HB_DEBUG_PAINT
#define TRACE_PAINT(এতত্) \
  HB_UNUSED hb_auto_trace_t<HB_DEBUG_PAINT, শূন্য> trace \
  (&c->debug_depth, c->get_name (), এতত্, HB_FUNC, \
   " ")
#অন্যথা
#define TRACE_PAINT(এতত্) HB_UNUSED hb_no_trace_t<শূন্য> trace
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
#যদি HB_DEBUG_DISPATCH
#define TRACE_DISPATCH(এতত্, format) \
	hb_auto_trace_t<context_t::max_debug_depth, typename context_t::return_t> trace \
	(&c->debug_depth, c->get_name (), এতত্, HB_FUNC, \
	 "format %u", (unsigned) format)
#অন্যথা
#define TRACE_DISPATCH(এতত্, format) hb_no_trace_t<typename context_t::return_t> trace
#endif


#ifndef HB_BUFFER_MESSAGE_MORE
#define HB_BUFFER_MESSAGE_MORE (HB_DEBUG+0)
#endif


#endif /* HB_DEBUG_HH */
