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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-atomic.hh"
#ଅନ୍ତର୍ଭାଵ "hb-algs.hh"


#ifndef HB_DEBUG
#define HB_DEBUG 0
#endif


/*
 * Debug output (needs enabling at compile time.)
 */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_debug (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା level,
	   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_level)
{
  ଫେରନ୍ତୁ level < max_level;
}

#define DEBUG_LEVEL_ENABLED(WHAT, LEVEL) (_hb_debug ((LEVEL), HB_DEBUG_##WHAT))
#define DEBUG_ENABLED(WHAT) (DEBUG_LEVEL_ENABLED (WHAT, 0))

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_hb_print_func (ଅଚଳ ଅକ୍ଷର *func)
{
  ଯଦି (func)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା func_len = strlen (func);
    /* Skip "static" */
    ଯଦି (0 == strncmp (func, "static ", 7))
      func += 7;
    /* Skip "typename" */
    ଯଦି (0 == strncmp (func, "typename ", 9))
      func += 9;
    /* Skip return type */
    ଅଚଳ ଅକ୍ଷର *space = strchr (func, ' ');
    ଯଦି (space)
      func = space + 1;
    /* Skip parameter list */
    ଅଚଳ ଅକ୍ଷର *paren = strchr (func, '(');
    ଯଦି (paren)
      func_len = paren - func;
    fprintf (stderr, "%.*s", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) func_len, func);
  }
}

template <ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_level> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_debug_ସନ୍ଦେଶ_va (ଅଚଳ ଅକ୍ଷର *what,
		  ଅଚଳ ଶୂନ୍ୟ *obj,
		  ଅଚଳ ଅକ୍ଷର *func,
		  ତାର୍କିକ indented,
		  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା level,
		  ପୂର୍ଣ୍ଣସଂଖ୍ୟା level_dir,
		  ଅଚଳ ଅକ୍ଷର *message,
		  va_list ap) HB_PRINTF_FUNC(7, 0);
template <ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_level> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_debug_ସନ୍ଦେଶ_va (ଅଚଳ ଅକ୍ଷର *what,
		  ଅଚଳ ଶୂନ୍ୟ *obj,
		  ଅଚଳ ଅକ୍ଷର *func,
		  ତାର୍କିକ indented,
		  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା level,
		  ପୂର୍ଣ୍ଣସଂଖ୍ୟା level_dir,
		  ଅଚଳ ଅକ୍ଷର *message,
		  va_list ap)
{
  ଯଦି (!_hb_debug (level, max_level))
    ଫେରନ୍ତୁ;

  fprintf (stderr, "%-10s", what ? what : "");

  ଯଦି (obj)
    fprintf (stderr, "(%*p) ", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (2 * ଆକାର (ଶୂନ୍ୟ *)), obj);
  ଅନ୍ୟଥା
    fprintf (stderr, " %*s  ", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (2 * ଆକାର (ଶୂନ୍ୟ *)), "");

  ଯଦି (indented) {
#define VBAR	"\342\224\202"	/* U+2502 BOX DRAWINGS LIGHT VERTICAL */
#define VRBAR	"\342\224\234"	/* U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
#define DLBAR	"\342\225\256"	/* U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
#define ULBAR	"\342\225\257"	/* U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
#define LBAR	"\342\225\264"	/* U+2574 BOX DRAWINGS LIGHT LEFT */
    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର bars[] =
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR;
    fprintf (stderr, "%2u %s" VRBAR "%s",
	     level,
	     bars + ଆକାର (bars) - 1 - hb_min ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (bars) - 1, (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (ଆକାର (VBAR) - 1) * level),
	     level_dir ? (level_dir > 0 ? DLBAR : ULBAR) : LBAR);
  } ଅନ୍ୟଥା
    fprintf (stderr, "   " VRBAR LBAR);

  _hb_print_func (func);

  ଯଦି (message)
  {
    fprintf (stderr, ": ");
    vfprintf (stderr, message, ap);
  }

  fprintf (stderr, "\n");
}
template <> ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ HB_PRINTF_FUNC(7, 0)
hb_debug_ସନ୍ଦେଶ_va<0> (ଅଚଳ ଅକ୍ଷର *what HB_UNUSED,
		     ଅଚଳ ଶୂନ୍ୟ *obj HB_UNUSED,
		     ଅଚଳ ଅକ୍ଷର *func HB_UNUSED,
		     ତାର୍କିକ indented HB_UNUSED,
		     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା level HB_UNUSED,
		     ପୂର୍ଣ୍ଣସଂଖ୍ୟା level_dir HB_UNUSED,
		     ଅଚଳ ଅକ୍ଷର *message HB_UNUSED,
		     va_list ap HB_UNUSED) {}

template <ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_level> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_debug_ସନ୍ଦେଶ (ଅଚଳ ଅକ୍ଷର *what,
	       ଅଚଳ ଶୂନ୍ୟ *obj,
	       ଅଚଳ ଅକ୍ଷର *func,
	       ତାର୍କିକ indented,
	       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା level,
	       ପୂର୍ଣ୍ଣସଂଖ୍ୟା level_dir,
	       ଅଚଳ ଅକ୍ଷର *message,
	       ...) HB_PRINTF_FUNC(7, 8);
template <ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_level> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ HB_PRINTF_FUNC(7, 8)
hb_debug_ସନ୍ଦେଶ (ଅଚଳ ଅକ୍ଷର *what,
	       ଅଚଳ ଶୂନ୍ୟ *obj,
	       ଅଚଳ ଅକ୍ଷର *func,
	       ତାର୍କିକ indented,
	       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା level,
	       ପୂର୍ଣ୍ଣସଂଖ୍ୟା level_dir,
	       ଅଚଳ ଅକ୍ଷର *message,
	       ...)
{
  va_list ap;
  va_start (ap, message);
  hb_debug_ସନ୍ଦେଶ_va<max_level> (what, obj, func, indented, level, level_dir, message, ap);
  va_end (ap);
}
template <> ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_debug_ସନ୍ଦେଶ<0> (ଅଚଳ ଅକ୍ଷର *what HB_UNUSED,
		  ଅଚଳ ଶୂନ୍ୟ *obj HB_UNUSED,
		  ଅଚଳ ଅକ୍ଷର *func HB_UNUSED,
		  ତାର୍କିକ indented HB_UNUSED,
		  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା level HB_UNUSED,
		  ପୂର୍ଣ୍ଣସଂଖ୍ୟା level_dir HB_UNUSED,
		  ଅଚଳ ଅକ୍ଷର *message HB_UNUSED,
		  ...) HB_PRINTF_FUNC(7, 8);
template <> ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ HB_PRINTF_FUNC(7, 8)
hb_debug_ସନ୍ଦେଶ<0> (ଅଚଳ ଅକ୍ଷର *what HB_UNUSED,
		  ଅଚଳ ଶୂନ୍ୟ *obj HB_UNUSED,
		  ଅଚଳ ଅକ୍ଷର *func HB_UNUSED,
		  ତାର୍କିକ indented HB_UNUSED,
		  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା level HB_UNUSED,
		  ପୂର୍ଣ୍ଣସଂଖ୍ୟା level_dir HB_UNUSED,
		  ଅଚଳ ଅକ୍ଷର *message HB_UNUSED,
		  ...) {}

#define DEBUG_ସନ୍ଦେଶ_LEVEL(WHAT, OBJ, LEVEL, LEVEL_DIR, ...)	hb_debug_ସନ୍ଦେଶ<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    ସତ୍ୟ, (LEVEL), (LEVEL_DIR), __VA_ARGS__)
#define DEBUG_ସନ୍ଦେଶ(WHAT, OBJ, ...)				hb_debug_ସନ୍ଦେଶ<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    ମିଥ୍ୟା, 0, 0, __VA_ARGS__)
#define DEBUG_ସନ୍ଦେଶ_FUNC(WHAT, OBJ, ...)				hb_debug_ସନ୍ଦେଶ<HB_DEBUG_##WHAT> (#WHAT, (OBJ), HB_FUNC, ମିଥ୍ୟା, 0, 0, __VA_ARGS__)


/*
 * Printer
 */

template <typename T>
ସଂରଚନା hb_printer_t {
  ଅଚଳ ଅକ୍ଷର *print (ଅଚଳ T&) { ଫେରନ୍ତୁ "something"; }
};

template <>
ସଂରଚନା hb_printer_t<ତାର୍କିକ> {
  ଅଚଳ ଅକ୍ଷର *print (ତାର୍କିକ v) { ଫେରନ୍ତୁ v ? "true" : "false"; }
};

template <>
ସଂରଚନା hb_printer_t<hb_empty_t> {
  ଅଚଳ ଅକ୍ଷର *print (hb_empty_t) { ଫେରନ୍ତୁ ""; }
};


/*
 * Trace
 */

template <typename T>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ _hb_warn_no_return (ତାର୍କିକ returned)
{
  ଯଦି (unlikely (!returned)) {
    fprintf (stderr, "OUCH, returned with no call to return_trace().  This is a bug, please report.\n");
  }
}
template <>
/*static*/ ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ _hb_warn_no_return<hb_empty_t> (ତାର୍କିକ returned HB_UNUSED) {}
template <>
/*static*/ ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ _hb_warn_no_return<ଶୂନ୍ୟ> (ତାର୍କିକ returned HB_UNUSED) {}

template <ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_level, typename ret_t>
ସଂରଚନା hb_auto_trace_t
{
  explicit ଧାଡ଼ିଭିତରେ hb_auto_trace_t (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *plevel_,
				   ଅଚଳ ଅକ୍ଷର *what_,
				   ଅଚଳ ଶୂନ୍ୟ *obj_,
				   ଅଚଳ ଅକ୍ଷର *func,
				   ଅଚଳ ଅକ୍ଷର *message,
				   ...) HB_PRINTF_FUNC(6, 7)
				   : plevel (plevel_), what (what_), obj (obj_), returned (ମିଥ୍ୟା)
  {
    ଯଦି (plevel) ++*plevel;

    va_list ap;
    va_start (ap, message);
    hb_debug_ସନ୍ଦେଶ_va<max_level> (what, obj, func, ସତ୍ୟ, plevel ? *plevel : 0, +1, message, ap);
    va_end (ap);
  }
  ~hb_auto_trace_t ()
  {
    _hb_warn_no_return<ret_t> (returned);
    ଯଦି (!returned) {
      hb_debug_ସନ୍ଦେଶ<max_level> (what, obj, nullptr, ସତ୍ୟ, plevel ? *plevel : 1, -1, " ");
    }
    ଯଦି (plevel) --*plevel;
  }

  template <typename T>
  T ret (T&& v,
	 ଅଚଳ ଅକ୍ଷର *func = "",
	 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି = 0)
  {
    ଯଦି (unlikely (returned)) {
      fprintf (stderr, "OUCH, double calls to return_trace().  This is a bug, please report.\n");
      ଫେରନ୍ତୁ std::forward<T> (v);
    }

    hb_debug_ସନ୍ଦେଶ<max_level> (what, obj, func, ସତ୍ୟ, plevel ? *plevel : 1, -1,
			      "return %s (line %u)",
			      hb_printer_t<hb_decay<decltype (v)>>().print (v), ଧାଡ଼ି);
    ଯଦି (plevel) --*plevel;
    plevel = nullptr;
    returned = ସତ୍ୟ;
    ଫେରନ୍ତୁ std::forward<T> (v);
  }

  ବ୍ୟକ୍ତିଗତ:
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *plevel;
  ଅଚଳ ଅକ୍ଷର *what;
  ଅଚଳ ଶୂନ୍ୟ *obj;
  ତାର୍କିକ returned;
};
template <typename ret_t> /* Make sure we don't use hb_auto_trace_t when not tracing. */
ସଂରଚନା hb_auto_trace_t<0, ret_t>
{
  explicit ଧାଡ଼ିଭିତରେ hb_auto_trace_t (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *plevel_,
				   ଅଚଳ ଅକ୍ଷର *what_,
				   ଅଚଳ ଶୂନ୍ୟ *obj_,
				   ଅଚଳ ଅକ୍ଷର *func,
				   ଅଚଳ ଅକ୍ଷର *message,
				   ...) HB_PRINTF_FUNC(6, 7) {}

  template <typename T>
  T ret (T&& v,
	 ଅଚଳ ଅକ୍ଷର *func HB_UNUSED = nullptr,
	 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି HB_UNUSED = 0) { ଫେରନ୍ତୁ std::forward<T> (v); }
};

/* For disabled tracing; optimize out everything.
 * https://github.com/harfbuzz/harfbuzz/pull/605 */
template <typename ret_t>
ସଂରଚନା hb_no_trace_t {
  template <typename T>
  T ret (T&& v,
	 ଅଚଳ ଅକ୍ଷର *func HB_UNUSED = nullptr,
	 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି HB_UNUSED = 0) { ଫେରନ୍ତୁ std::forward<T> (v); }
};

#define return_trace(RET) ଫେରନ୍ତୁ trace.ret (RET, HB_FUNC, __LINE__)


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
#ଯଦି HB_DEBUG_APPLY
#define TRACE_APPLY(ଏହି) \
	hb_auto_trace_t<HB_DEBUG_APPLY, ତାର୍କିକ> trace \
	(&c->debug_depth, c->get_name (), ଏହି, HB_FUNC, \
	 "idx %u gid %u lookup %d", \
	 c->buffer->idx, c->buffer->cur().codepoint, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) c->lookup_index)
#ଅନ୍ୟଥା
#define TRACE_APPLY(ଏହି) hb_no_trace_t<ତାର୍କିକ> trace
#endif

#ifndef HB_DEBUG_SANITIZE
#define HB_DEBUG_SANITIZE (HB_DEBUG+0)
#endif
#ଯଦି HB_DEBUG_SANITIZE
#define TRACE_SANITIZE(ଏହି) \
	hb_auto_trace_t<HB_DEBUG_SANITIZE, ତାର୍କିକ> trace \
	(&c->debug_depth, c->get_name (), ଏହି, HB_FUNC, \
	 " ")
#ଅନ୍ୟଥା
#define TRACE_SANITIZE(ଏହି) hb_no_trace_t<ତାର୍କିକ> trace
#endif

#ifndef HB_DEBUG_SERIALIZE
#define HB_DEBUG_SERIALIZE (HB_DEBUG+0)
#endif
#ଯଦି HB_DEBUG_SERIALIZE
#define TRACE_SERIALIZE(ଏହି) \
	hb_auto_trace_t<HB_DEBUG_SERIALIZE, ତାର୍କିକ> trace \
	(&c->debug_depth, "SERIALIZE", c, HB_FUNC, \
	 " ")
#ଅନ୍ୟଥା
#define TRACE_SERIALIZE(ଏହି) hb_no_trace_t<ତାର୍କିକ> trace
#endif

#ifndef HB_DEBUG_SUBSET
#define HB_DEBUG_SUBSET (HB_DEBUG+0)
#endif
#ଯଦି HB_DEBUG_SUBSET
#define TRACE_SUBSET(ଏହି) \
  hb_auto_trace_t<HB_DEBUG_SUBSET, ତାର୍କିକ> trace \
  (&c->debug_depth, c->get_name (), ଏହି, HB_FUNC, \
   " ")
#ଅନ୍ୟଥା
#define TRACE_SUBSET(ଏହି) hb_no_trace_t<ତାର୍କିକ> trace
#endif

#ifndef HB_DEBUG_SUBSET_REPACK
#define HB_DEBUG_SUBSET_REPACK (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_PAINT
#define HB_DEBUG_PAINT (HB_DEBUG+0)
#endif
#ଯଦି HB_DEBUG_PAINT
#define TRACE_PAINT(ଏହି) \
  HB_UNUSED hb_auto_trace_t<HB_DEBUG_PAINT, ଶୂନ୍ୟ> trace \
  (&c->debug_depth, c->get_name (), ଏହି, HB_FUNC, \
   " ")
#ଅନ୍ୟଥା
#define TRACE_PAINT(ଏହି) HB_UNUSED hb_no_trace_t<ଶୂନ୍ୟ> trace
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
#ଯଦି HB_DEBUG_DISPATCH
#define TRACE_DISPATCH(ଏହି, format) \
	hb_auto_trace_t<context_t::max_debug_depth, typename context_t::return_t> trace \
	(&c->debug_depth, c->get_name (), ଏହି, HB_FUNC, \
	 "format %u", (unsigned) format)
#ଅନ୍ୟଥା
#define TRACE_DISPATCH(ଏହି, format) hb_no_trace_t<typename context_t::return_t> trace
#endif


#ifndef HB_BUFFER_MESSAGE_MORE
#define HB_BUFFER_MESSAGE_MORE (HB_DEBUG+0)
#endif


#endif /* HB_DEBUG_HH */
