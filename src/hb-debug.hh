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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-atomic.hh"
#अन्तर्भाव "hb-algs.hh"


#ifndef HB_DEBUG
#define HB_DEBUG 0
#endif


/*
 * Debug output (needs enabling at compile time.)
 */

स्थिर अन्तःस्थ तार्किक
_hb_debug (unsigned पूर्णाङ्क level,
	   unsigned पूर्णाङ्क max_level)
{
  प्रत्यागम level < max_level;
}

#define DEBUG_LEVEL_ENABLED(WHAT, LEVEL) (_hb_debug ((LEVEL), HB_DEBUG_##WHAT))
#define DEBUG_ENABLED(WHAT) (DEBUG_LEVEL_ENABLED (WHAT, 0))

स्थिर अन्तःस्थ शून्य
_hb_print_func (ध्रुव वर्ण *func)
{
  यदि (func)
  {
    unsigned पूर्णाङ्क func_len = strlen (func);
    /* Skip "static" */
    यदि (0 == strncmp (func, "static ", 7))
      func += 7;
    /* Skip "typename" */
    यदि (0 == strncmp (func, "typename ", 9))
      func += 9;
    /* Skip return type */
    ध्रुव वर्ण *space = strchr (func, ' ');
    यदि (space)
      func = space + 1;
    /* Skip parameter list */
    ध्रुव वर्ण *paren = strchr (func, '(');
    यदि (paren)
      func_len = paren - func;
    fprintf (stderr, "%.*s", (पूर्णाङ्क) func_len, func);
  }
}

template <पूर्णाङ्क max_level> स्थिर अन्तःस्थ शून्य
hb_debug_संदेश_va (ध्रुव वर्ण *what,
		  ध्रुव शून्य *obj,
		  ध्रुव वर्ण *func,
		  तार्किक indented,
		  unsigned पूर्णाङ्क level,
		  पूर्णाङ्क level_dir,
		  ध्रुव वर्ण *message,
		  va_list ap) HB_PRINTF_FUNC(7, 0);
template <पूर्णाङ्क max_level> स्थिर अन्तःस्थ शून्य
hb_debug_संदेश_va (ध्रुव वर्ण *what,
		  ध्रुव शून्य *obj,
		  ध्रुव वर्ण *func,
		  तार्किक indented,
		  unsigned पूर्णाङ्क level,
		  पूर्णाङ्क level_dir,
		  ध्रुव वर्ण *message,
		  va_list ap)
{
  यदि (!_hb_debug (level, max_level))
    प्रत्यागम;

  fprintf (stderr, "%-10s", what ? what : "");

  यदि (obj)
    fprintf (stderr, "(%*p) ", (पूर्णाङ्क) (2 * परिमाण (शून्य *)), obj);
  अन्यथा
    fprintf (stderr, " %*s  ", (पूर्णाङ्क) (2 * परिमाण (शून्य *)), "");

  यदि (indented) {
#define VBAR	"\342\224\202"	/* U+2502 BOX DRAWINGS LIGHT VERTICAL */
#define VRBAR	"\342\224\234"	/* U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
#define DLBAR	"\342\225\256"	/* U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
#define ULBAR	"\342\225\257"	/* U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
#define LBAR	"\342\225\264"	/* U+2574 BOX DRAWINGS LIGHT LEFT */
    स्थिर ध्रुव वर्ण bars[] =
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR;
    fprintf (stderr, "%2u %s" VRBAR "%s",
	     level,
	     bars + परिमाण (bars) - 1 - hb_min ((unsigned पूर्णाङ्क) परिमाण (bars) - 1, (unsigned पूर्णाङ्क) (परिमाण (VBAR) - 1) * level),
	     level_dir ? (level_dir > 0 ? DLBAR : ULBAR) : LBAR);
  } अन्यथा
    fprintf (stderr, "   " VRBAR LBAR);

  _hb_print_func (func);

  यदि (message)
  {
    fprintf (stderr, ": ");
    vfprintf (stderr, message, ap);
  }

  fprintf (stderr, "\n");
}
template <> अन्तःस्थ शून्य HB_PRINTF_FUNC(7, 0)
hb_debug_संदेश_va<0> (ध्रुव वर्ण *what HB_UNUSED,
		     ध्रुव शून्य *obj HB_UNUSED,
		     ध्रुव वर्ण *func HB_UNUSED,
		     तार्किक indented HB_UNUSED,
		     unsigned पूर्णाङ्क level HB_UNUSED,
		     पूर्णाङ्क level_dir HB_UNUSED,
		     ध्रुव वर्ण *message HB_UNUSED,
		     va_list ap HB_UNUSED) {}

template <पूर्णाङ्क max_level> स्थिर अन्तःस्थ शून्य
hb_debug_संदेश (ध्रुव वर्ण *what,
	       ध्रुव शून्य *obj,
	       ध्रुव वर्ण *func,
	       तार्किक indented,
	       unsigned पूर्णाङ्क level,
	       पूर्णाङ्क level_dir,
	       ध्रुव वर्ण *message,
	       ...) HB_PRINTF_FUNC(7, 8);
template <पूर्णाङ्क max_level> स्थिर अन्तःस्थ शून्य HB_PRINTF_FUNC(7, 8)
hb_debug_संदेश (ध्रुव वर्ण *what,
	       ध्रुव शून्य *obj,
	       ध्रुव वर्ण *func,
	       तार्किक indented,
	       unsigned पूर्णाङ्क level,
	       पूर्णाङ्क level_dir,
	       ध्रुव वर्ण *message,
	       ...)
{
  va_list ap;
  va_start (ap, message);
  hb_debug_संदेश_va<max_level> (what, obj, func, indented, level, level_dir, message, ap);
  va_end (ap);
}
template <> अन्तःस्थ शून्य
hb_debug_संदेश<0> (ध्रुव वर्ण *what HB_UNUSED,
		  ध्रुव शून्य *obj HB_UNUSED,
		  ध्रुव वर्ण *func HB_UNUSED,
		  तार्किक indented HB_UNUSED,
		  unsigned पूर्णाङ्क level HB_UNUSED,
		  पूर्णाङ्क level_dir HB_UNUSED,
		  ध्रुव वर्ण *message HB_UNUSED,
		  ...) HB_PRINTF_FUNC(7, 8);
template <> अन्तःस्थ शून्य HB_PRINTF_FUNC(7, 8)
hb_debug_संदेश<0> (ध्रुव वर्ण *what HB_UNUSED,
		  ध्रुव शून्य *obj HB_UNUSED,
		  ध्रुव वर्ण *func HB_UNUSED,
		  तार्किक indented HB_UNUSED,
		  unsigned पूर्णाङ्क level HB_UNUSED,
		  पूर्णाङ्क level_dir HB_UNUSED,
		  ध्रुव वर्ण *message HB_UNUSED,
		  ...) {}

#define DEBUG_संदेश_LEVEL(WHAT, OBJ, LEVEL, LEVEL_DIR, ...)	hb_debug_संदेश<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    सत्य, (LEVEL), (LEVEL_DIR), __VA_ARGS__)
#define DEBUG_संदेश(WHAT, OBJ, ...)				hb_debug_संदेश<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    असत्य, 0, 0, __VA_ARGS__)
#define DEBUG_संदेश_FUNC(WHAT, OBJ, ...)				hb_debug_संदेश<HB_DEBUG_##WHAT> (#WHAT, (OBJ), HB_FUNC, असत्य, 0, 0, __VA_ARGS__)


/*
 * Printer
 */

template <typename T>
संरचना hb_printer_t {
  ध्रुव वर्ण *print (ध्रुव T&) { प्रत्यागम "something"; }
};

template <>
संरचना hb_printer_t<तार्किक> {
  ध्रुव वर्ण *print (तार्किक v) { प्रत्यागम v ? "true" : "false"; }
};

template <>
संरचना hb_printer_t<hb_empty_t> {
  ध्रुव वर्ण *print (hb_empty_t) { प्रत्यागम ""; }
};


/*
 * Trace
 */

template <typename T>
स्थिर अन्तःस्थ शून्य _hb_warn_no_return (तार्किक returned)
{
  यदि (unlikely (!returned)) {
    fprintf (stderr, "OUCH, returned with no call to return_trace().  This is a bug, please report.\n");
  }
}
template <>
/*static*/ अन्तःस्थ शून्य _hb_warn_no_return<hb_empty_t> (तार्किक returned HB_UNUSED) {}
template <>
/*static*/ अन्तःस्थ शून्य _hb_warn_no_return<शून्य> (तार्किक returned HB_UNUSED) {}

template <पूर्णाङ्क max_level, typename ret_t>
संरचना hb_auto_trace_t
{
  explicit अन्तःस्थ hb_auto_trace_t (unsigned पूर्णाङ्क *plevel_,
				   ध्रुव वर्ण *what_,
				   ध्रुव शून्य *obj_,
				   ध्रुव वर्ण *func,
				   ध्रुव वर्ण *message,
				   ...) HB_PRINTF_FUNC(6, 7)
				   : plevel (plevel_), what (what_), obj (obj_), returned (असत्य)
  {
    यदि (plevel) ++*plevel;

    va_list ap;
    va_start (ap, message);
    hb_debug_संदेश_va<max_level> (what, obj, func, सत्य, plevel ? *plevel : 0, +1, message, ap);
    va_end (ap);
  }
  ~hb_auto_trace_t ()
  {
    _hb_warn_no_return<ret_t> (returned);
    यदि (!returned) {
      hb_debug_संदेश<max_level> (what, obj, nullptr, सत्य, plevel ? *plevel : 1, -1, " ");
    }
    यदि (plevel) --*plevel;
  }

  template <typename T>
  T ret (T&& v,
	 ध्रुव वर्ण *func = "",
	 unsigned पूर्णाङ्क पंक्ति = 0)
  {
    यदि (unlikely (returned)) {
      fprintf (stderr, "OUCH, double calls to return_trace().  This is a bug, please report.\n");
      प्रत्यागम std::forward<T> (v);
    }

    hb_debug_संदेश<max_level> (what, obj, func, सत्य, plevel ? *plevel : 1, -1,
			      "return %s (line %u)",
			      hb_printer_t<hb_decay<decltype (v)>>().print (v), पंक्ति);
    यदि (plevel) --*plevel;
    plevel = nullptr;
    returned = सत्य;
    प्रत्यागम std::forward<T> (v);
  }

  गुप्त:
  unsigned पूर्णाङ्क *plevel;
  ध्रुव वर्ण *what;
  ध्रुव शून्य *obj;
  तार्किक returned;
};
template <typename ret_t> /* Make sure we don't use hb_auto_trace_t when not tracing. */
संरचना hb_auto_trace_t<0, ret_t>
{
  explicit अन्तःस्थ hb_auto_trace_t (unsigned पूर्णाङ्क *plevel_,
				   ध्रुव वर्ण *what_,
				   ध्रुव शून्य *obj_,
				   ध्रुव वर्ण *func,
				   ध्रुव वर्ण *message,
				   ...) HB_PRINTF_FUNC(6, 7) {}

  template <typename T>
  T ret (T&& v,
	 ध्रुव वर्ण *func HB_UNUSED = nullptr,
	 unsigned पूर्णाङ्क पंक्ति HB_UNUSED = 0) { प्रत्यागम std::forward<T> (v); }
};

/* For disabled tracing; optimize out everything.
 * https://github.com/harfbuzz/harfbuzz/pull/605 */
template <typename ret_t>
संरचना hb_no_trace_t {
  template <typename T>
  T ret (T&& v,
	 ध्रुव वर्ण *func HB_UNUSED = nullptr,
	 unsigned पूर्णाङ्क पंक्ति HB_UNUSED = 0) { प्रत्यागम std::forward<T> (v); }
};

#define return_trace(RET) प्रत्यागम trace.ret (RET, HB_FUNC, __LINE__)


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
#यदि HB_DEBUG_APPLY
#define TRACE_APPLY(एतत्) \
	hb_auto_trace_t<HB_DEBUG_APPLY, तार्किक> trace \
	(&c->debug_depth, c->get_name (), एतत्, HB_FUNC, \
	 "idx %u gid %u lookup %d", \
	 c->buffer->idx, c->buffer->cur().codepoint, (पूर्णाङ्क) c->lookup_index)
#अन्यथा
#define TRACE_APPLY(एतत्) hb_no_trace_t<तार्किक> trace
#endif

#ifndef HB_DEBUG_SANITIZE
#define HB_DEBUG_SANITIZE (HB_DEBUG+0)
#endif
#यदि HB_DEBUG_SANITIZE
#define TRACE_SANITIZE(एतत्) \
	hb_auto_trace_t<HB_DEBUG_SANITIZE, तार्किक> trace \
	(&c->debug_depth, c->get_name (), एतत्, HB_FUNC, \
	 " ")
#अन्यथा
#define TRACE_SANITIZE(एतत्) hb_no_trace_t<तार्किक> trace
#endif

#ifndef HB_DEBUG_SERIALIZE
#define HB_DEBUG_SERIALIZE (HB_DEBUG+0)
#endif
#यदि HB_DEBUG_SERIALIZE
#define TRACE_SERIALIZE(एतत्) \
	hb_auto_trace_t<HB_DEBUG_SERIALIZE, तार्किक> trace \
	(&c->debug_depth, "SERIALIZE", c, HB_FUNC, \
	 " ")
#अन्यथा
#define TRACE_SERIALIZE(एतत्) hb_no_trace_t<तार्किक> trace
#endif

#ifndef HB_DEBUG_SUBSET
#define HB_DEBUG_SUBSET (HB_DEBUG+0)
#endif
#यदि HB_DEBUG_SUBSET
#define TRACE_SUBSET(एतत्) \
  hb_auto_trace_t<HB_DEBUG_SUBSET, तार्किक> trace \
  (&c->debug_depth, c->get_name (), एतत्, HB_FUNC, \
   " ")
#अन्यथा
#define TRACE_SUBSET(एतत्) hb_no_trace_t<तार्किक> trace
#endif

#ifndef HB_DEBUG_SUBSET_REPACK
#define HB_DEBUG_SUBSET_REPACK (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_PAINT
#define HB_DEBUG_PAINT (HB_DEBUG+0)
#endif
#यदि HB_DEBUG_PAINT
#define TRACE_PAINT(एतत्) \
  HB_UNUSED hb_auto_trace_t<HB_DEBUG_PAINT, शून्य> trace \
  (&c->debug_depth, c->get_name (), एतत्, HB_FUNC, \
   " ")
#अन्यथा
#define TRACE_PAINT(एतत्) HB_UNUSED hb_no_trace_t<शून्य> trace
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
#यदि HB_DEBUG_DISPATCH
#define TRACE_DISPATCH(एतत्, format) \
	hb_auto_trace_t<context_t::max_debug_depth, typename context_t::return_t> trace \
	(&c->debug_depth, c->get_name (), एतत्, HB_FUNC, \
	 "format %u", (unsigned) format)
#अन्यथा
#define TRACE_DISPATCH(एतत्, format) hb_no_trace_t<typename context_t::return_t> trace
#endif


#ifndef HB_BUFFER_MESSAGE_MORE
#define HB_BUFFER_MESSAGE_MORE (HB_DEBUG+0)
#endif


#endif /* HB_DEBUG_HH */
