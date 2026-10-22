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

#अगर_अघोषित HB_DEBUG_HH
#घोषणा HB_DEBUG_HH

#समावेश "hb.hh"
#समावेश "hb-atomic.hh"
#समावेश "hb-algs.hh"


#अगर_अघोषित HB_DEBUG
#घोषणा HB_DEBUG 0
#पूर्ण_अगर


/*
 * Debug output (needs enabling at compile time.)
 */

अटल अंतरभूत bool
_hb_debug (अचिन्हित पूर्णांक level,
	   अचिन्हित पूर्णांक max_level)
{
  वापस level < max_level;
}

#घोषणा DEBUG_LEVEL_ENABLED(WHAT, LEVEL) (_hb_debug ((LEVEL), HB_DEBUG_##WHAT))
#घोषणा DEBUG_ENABLED(WHAT) (DEBUG_LEVEL_ENABLED (WHAT, 0))

अटल अंतरभूत व्योम
_hb_print_func (स्थिर अक्षर *func)
{
  अगर (func)
  {
    अचिन्हित पूर्णांक func_len = म_माप (func);
    /* Skip "static" */
    अगर (0 == म_भेदन (func, "static ", 7))
      func += 7;
    /* Skip "typename" */
    अगर (0 == म_भेदन (func, "typename ", 9))
      func += 9;
    /* Skip return type */
    स्थिर अक्षर *space = म_अक्षर (func, ' ');
    अगर (space)
      func = space + 1;
    /* Skip parameter list */
    स्थिर अक्षर *paren = म_अक्षर (func, '(');
    अगर (paren)
      func_len = paren - func;
    ख_लिखो (मानक_त्रुटि, "%.*s", (पूर्णांक) func_len, func);
  }
}

ढाँचा <पूर्णांक max_level> अटल अंतरभूत व्योम
hb_debug_संदेश_va (स्थिर अक्षर *what,
		  स्थिर व्योम *obj,
		  स्थिर अक्षर *func,
		  bool indented,
		  अचिन्हित पूर्णांक level,
		  पूर्णांक level_dir,
		  स्थिर अक्षर *message,
		  बहु_सूची ap) HB_PRINTF_FUNC(7, 0);
ढाँचा <पूर्णांक max_level> अटल अंतरभूत व्योम
hb_debug_संदेश_va (स्थिर अक्षर *what,
		  स्थिर व्योम *obj,
		  स्थिर अक्षर *func,
		  bool indented,
		  अचिन्हित पूर्णांक level,
		  पूर्णांक level_dir,
		  स्थिर अक्षर *message,
		  बहु_सूची ap)
{
  अगर (!_hb_debug (level, max_level))
    वापस;

  ख_लिखो (मानक_त्रुटि, "%-10s", what ? what : "");

  अगर (obj)
    ख_लिखो (मानक_त्रुटि, "(%*p) ", (पूर्णांक) (2 * माप (व्योम *)), obj);
  अन्यथा
    ख_लिखो (मानक_त्रुटि, " %*s  ", (पूर्णांक) (2 * माप (व्योम *)), "");

  अगर (indented) {
#घोषणा VBAR	"\342\224\202"	/* U+2502 BOX DRAWINGS LIGHT VERTICAL */
#घोषणा VRBAR	"\342\224\234"	/* U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
#घोषणा DLBAR	"\342\225\256"	/* U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
#घोषणा ULBAR	"\342\225\257"	/* U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
#घोषणा LBAR	"\342\225\264"	/* U+2574 BOX DRAWINGS LIGHT LEFT */
    अटल स्थिर अक्षर bars[] =
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR;
    ख_लिखो (मानक_त्रुटि, "%2u %s" VRBAR "%s",
	     level,
	     bars + माप (bars) - 1 - hb_min ((अचिन्हित पूर्णांक) माप (bars) - 1, (अचिन्हित पूर्णांक) (माप (VBAR) - 1) * level),
	     level_dir ? (level_dir > 0 ? DLBAR : ULBAR) : LBAR);
  } अन्यथा
    ख_लिखो (मानक_त्रुटि, "   " VRBAR LBAR);

  _hb_print_func (func);

  अगर (message)
  {
    ख_लिखो (मानक_त्रुटि, ": ");
    भख_लिखो (मानक_त्रुटि, message, ap);
  }

  ख_लिखो (मानक_त्रुटि, "\n");
}
ढाँचा <> अंतरभूत व्योम HB_PRINTF_FUNC(7, 0)
hb_debug_संदेश_va<0> (स्थिर अक्षर *what HB_UNUSED,
		     स्थिर व्योम *obj HB_UNUSED,
		     स्थिर अक्षर *func HB_UNUSED,
		     bool indented HB_UNUSED,
		     अचिन्हित पूर्णांक level HB_UNUSED,
		     पूर्णांक level_dir HB_UNUSED,
		     स्थिर अक्षर *message HB_UNUSED,
		     बहु_सूची ap HB_UNUSED) {}

ढाँचा <पूर्णांक max_level> अटल अंतरभूत व्योम
hb_debug_संदेश (स्थिर अक्षर *what,
	       स्थिर व्योम *obj,
	       स्थिर अक्षर *func,
	       bool indented,
	       अचिन्हित पूर्णांक level,
	       पूर्णांक level_dir,
	       स्थिर अक्षर *message,
	       ...) HB_PRINTF_FUNC(7, 8);
ढाँचा <पूर्णांक max_level> अटल अंतरभूत व्योम HB_PRINTF_FUNC(7, 8)
hb_debug_संदेश (स्थिर अक्षर *what,
	       स्थिर व्योम *obj,
	       स्थिर अक्षर *func,
	       bool indented,
	       अचिन्हित पूर्णांक level,
	       पूर्णांक level_dir,
	       स्थिर अक्षर *message,
	       ...)
{
  बहु_सूची ap;
  बहु_शुरू (ap, message);
  hb_debug_संदेश_va<max_level> (what, obj, func, indented, level, level_dir, message, ap);
  बहु_पूर्ण (ap);
}
ढाँचा <> अंतरभूत व्योम
hb_debug_संदेश<0> (स्थिर अक्षर *what HB_UNUSED,
		  स्थिर व्योम *obj HB_UNUSED,
		  स्थिर अक्षर *func HB_UNUSED,
		  bool indented HB_UNUSED,
		  अचिन्हित पूर्णांक level HB_UNUSED,
		  पूर्णांक level_dir HB_UNUSED,
		  स्थिर अक्षर *message HB_UNUSED,
		  ...) HB_PRINTF_FUNC(7, 8);
ढाँचा <> अंतरभूत व्योम HB_PRINTF_FUNC(7, 8)
hb_debug_संदेश<0> (स्थिर अक्षर *what HB_UNUSED,
		  स्थिर व्योम *obj HB_UNUSED,
		  स्थिर अक्षर *func HB_UNUSED,
		  bool indented HB_UNUSED,
		  अचिन्हित पूर्णांक level HB_UNUSED,
		  पूर्णांक level_dir HB_UNUSED,
		  स्थिर अक्षर *message HB_UNUSED,
		  ...) {}

#घोषणा DEBUG_संदेश_LEVEL(WHAT, OBJ, LEVEL, LEVEL_DIR, ...)	hb_debug_संदेश<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    true, (LEVEL), (LEVEL_DIR), __VA_ARGS__)
#घोषणा DEBUG_संदेश(WHAT, OBJ, ...)				hb_debug_संदेश<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    false, 0, 0, __VA_ARGS__)
#घोषणा DEBUG_संदेश_FUNC(WHAT, OBJ, ...)				hb_debug_संदेश<HB_DEBUG_##WHAT> (#WHAT, (OBJ), HB_FUNC, false, 0, 0, __VA_ARGS__)


/*
 * Printer
 */

ढाँचा <typename T>
काष्ठा hb_printer_t {
  स्थिर अक्षर *print (स्थिर T&) { वापस "something"; }
};

ढाँचा <>
काष्ठा hb_printer_t<bool> {
  स्थिर अक्षर *print (bool v) { वापस v ? "true" : "false"; }
};

ढाँचा <>
काष्ठा hb_printer_t<hb_empty_t> {
  स्थिर अक्षर *print (hb_empty_t) { वापस ""; }
};


/*
 * Trace
 */

ढाँचा <typename T>
अटल अंतरभूत व्योम _hb_warn_no_return (bool returned)
{
  अगर (unlikely (!returned)) {
    ख_लिखो (मानक_त्रुटि, "OUCH, returned with no call to return_trace().  This is a bug, please report.\n");
  }
}
ढाँचा <>
/*static*/ अंतरभूत व्योम _hb_warn_no_return<hb_empty_t> (bool returned HB_UNUSED) {}
ढाँचा <>
/*static*/ अंतरभूत व्योम _hb_warn_no_return<व्योम> (bool returned HB_UNUSED) {}

ढाँचा <पूर्णांक max_level, typename ret_t>
काष्ठा hb_auto_trace_t
{
  explicit अंतरभूत hb_auto_trace_t (अचिन्हित पूर्णांक *plevel_,
				   स्थिर अक्षर *what_,
				   स्थिर व्योम *obj_,
				   स्थिर अक्षर *func,
				   स्थिर अक्षर *message,
				   ...) HB_PRINTF_FUNC(6, 7)
				   : plevel (plevel_), what (what_), obj (obj_), returned (false)
  {
    अगर (plevel) ++*plevel;

    बहु_सूची ap;
    बहु_शुरू (ap, message);
    hb_debug_संदेश_va<max_level> (what, obj, func, true, plevel ? *plevel : 0, +1, message, ap);
    बहु_पूर्ण (ap);
  }
  ~hb_auto_trace_t ()
  {
    _hb_warn_no_return<ret_t> (returned);
    अगर (!returned) {
      hb_debug_संदेश<max_level> (what, obj, nullptr, true, plevel ? *plevel : 1, -1, " ");
    }
    अगर (plevel) --*plevel;
  }

  ढाँचा <typename T>
  T ret (T&& v,
	 स्थिर अक्षर *func = "",
	 अचिन्हित पूर्णांक पंक्ति = 0)
  {
    अगर (unlikely (returned)) {
      ख_लिखो (मानक_त्रुटि, "OUCH, double calls to return_trace().  This is a bug, please report.\n");
      वापस std::forward<T> (v);
    }

    hb_debug_संदेश<max_level> (what, obj, func, true, plevel ? *plevel : 1, -1,
			      "return %s (line %u)",
			      hb_printer_t<hb_decay<decltype (v)>>().print (v), पंक्ति);
    अगर (plevel) --*plevel;
    plevel = nullptr;
    returned = true;
    वापस std::forward<T> (v);
  }

  निजी:
  अचिन्हित पूर्णांक *plevel;
  स्थिर अक्षर *what;
  स्थिर व्योम *obj;
  bool returned;
};
ढाँचा <typename ret_t> /* Make sure we don't use hb_auto_trace_t when not tracing. */
काष्ठा hb_auto_trace_t<0, ret_t>
{
  explicit अंतरभूत hb_auto_trace_t (अचिन्हित पूर्णांक *plevel_,
				   स्थिर अक्षर *what_,
				   स्थिर व्योम *obj_,
				   स्थिर अक्षर *func,
				   स्थिर अक्षर *message,
				   ...) HB_PRINTF_FUNC(6, 7) {}

  ढाँचा <typename T>
  T ret (T&& v,
	 स्थिर अक्षर *func HB_UNUSED = nullptr,
	 अचिन्हित पूर्णांक पंक्ति HB_UNUSED = 0) { वापस std::forward<T> (v); }
};

/* For disabled tracing; optimize out everything.
 * https://github.com/harfbuzz/harfbuzz/pull/605 */
ढाँचा <typename ret_t>
काष्ठा hb_no_trace_t {
  ढाँचा <typename T>
  T ret (T&& v,
	 स्थिर अक्षर *func HB_UNUSED = nullptr,
	 अचिन्हित पूर्णांक पंक्ति HB_UNUSED = 0) { वापस std::forward<T> (v); }
};

#घोषणा return_trace(RET) वापस trace.ret (RET, HB_FUNC, __LINE__)


/*
 * Instances.
 */

#अगर_अघोषित HB_DEBUG_ARABIC
#घोषणा HB_DEBUG_ARABIC (HB_DEBUG+0)
#पूर्ण_अगर

#अगर_अघोषित HB_DEBUG_BLOB
#घोषणा HB_DEBUG_BLOB (HB_DEBUG+0)
#पूर्ण_अगर

#अगर_अघोषित HB_DEBUG_CORETEXT
#घोषणा HB_DEBUG_CORETEXT (HB_DEBUG+0)
#पूर्ण_अगर

#अगर_अघोषित HB_DEBUG_DIRECTWRITE
#घोषणा HB_DEBUG_DIRECTWRITE (HB_DEBUG+0)
#पूर्ण_अगर

#अगर_अघोषित HB_DEBUG_FT
#घोषणा HB_DEBUG_FT (HB_DEBUG+0)
#पूर्ण_अगर

#अगर_अघोषित HB_DEBUG_JUSTIFY
#घोषणा HB_DEBUG_JUSTIFY (HB_DEBUG+0)
#पूर्ण_अगर

#अगर_अघोषित HB_DEBUG_OBJECT
#घोषणा HB_DEBUG_OBJECT (HB_DEBUG+0)
#पूर्ण_अगर

#अगर_अघोषित HB_DEBUG_SHAPE_PLAN
#घोषणा HB_DEBUG_SHAPE_PLAN (HB_DEBUG+0)
#पूर्ण_अगर

#अगर_अघोषित HB_DEBUG_UNISCRIBE
#घोषणा HB_DEBUG_UNISCRIBE (HB_DEBUG+0)
#पूर्ण_अगर

#अगर_अघोषित HB_DEBUG_WASM
#घोषणा HB_DEBUG_WASM (HB_DEBUG+0)
#पूर्ण_अगर

#अगर_अघोषित HB_DEBUG_KBTS
#घोषणा HB_DEBUG_KBTS (HB_DEBUG+0)
#पूर्ण_अगर

#अगर_अघोषित HB_DEBUG_DEPEND
#घोषणा HB_DEBUG_DEPEND (HB_DEBUG+0)
#पूर्ण_अगर

/*
 * With tracing.
 */

#अगर_अघोषित HB_DEBUG_APPLY
#घोषणा HB_DEBUG_APPLY (HB_DEBUG+0)
#पूर्ण_अगर
#अगर HB_DEBUG_APPLY
#घोषणा TRACE_APPLY(यह) \
	hb_auto_trace_t<HB_DEBUG_APPLY, bool> trace \
	(&c->debug_depth, c->get_name (), यह, HB_FUNC, \
	 "idx %u gid %u lookup %d", \
	 c->बफर->idx, c->बफर->cur().codepoint, (पूर्णांक) c->lookup_सूचकांक)
#अन्यथा
#घोषणा TRACE_APPLY(यह) hb_no_trace_t<bool> trace
#पूर्ण_अगर

#अगर_अघोषित HB_DEBUG_SANITIZE
#घोषणा HB_DEBUG_SANITIZE (HB_DEBUG+0)
#पूर्ण_अगर
#अगर HB_DEBUG_SANITIZE
#घोषणा TRACE_SANITIZE(यह) \
	hb_auto_trace_t<HB_DEBUG_SANITIZE, bool> trace \
	(&c->debug_depth, c->get_name (), यह, HB_FUNC, \
	 " ")
#अन्यथा
#घोषणा TRACE_SANITIZE(यह) hb_no_trace_t<bool> trace
#पूर्ण_अगर

#अगर_अघोषित HB_DEBUG_SERIALIZE
#घोषणा HB_DEBUG_SERIALIZE (HB_DEBUG+0)
#पूर्ण_अगर
#अगर HB_DEBUG_SERIALIZE
#घोषणा TRACE_SERIALIZE(यह) \
	hb_auto_trace_t<HB_DEBUG_SERIALIZE, bool> trace \
	(&c->debug_depth, "SERIALIZE", c, HB_FUNC, \
	 " ")
#अन्यथा
#घोषणा TRACE_SERIALIZE(यह) hb_no_trace_t<bool> trace
#पूर्ण_अगर

#अगर_अघोषित HB_DEBUG_SUBSET
#घोषणा HB_DEBUG_SUBSET (HB_DEBUG+0)
#पूर्ण_अगर
#अगर HB_DEBUG_SUBSET
#घोषणा TRACE_SUBSET(यह) \
  hb_auto_trace_t<HB_DEBUG_SUBSET, bool> trace \
  (&c->debug_depth, c->get_name (), यह, HB_FUNC, \
   " ")
#अन्यथा
#घोषणा TRACE_SUBSET(यह) hb_no_trace_t<bool> trace
#पूर्ण_अगर

#अगर_अघोषित HB_DEBUG_SUBSET_REPACK
#घोषणा HB_DEBUG_SUBSET_REPACK (HB_DEBUG+0)
#पूर्ण_अगर

#अगर_अघोषित HB_DEBUG_PAINT
#घोषणा HB_DEBUG_PAINT (HB_DEBUG+0)
#पूर्ण_अगर
#अगर HB_DEBUG_PAINT
#घोषणा TRACE_PAINT(यह) \
  HB_UNUSED hb_auto_trace_t<HB_DEBUG_PAINT, व्योम> trace \
  (&c->debug_depth, c->get_name (), यह, HB_FUNC, \
   " ")
#अन्यथा
#घोषणा TRACE_PAINT(यह) HB_UNUSED hb_no_trace_t<व्योम> trace
#पूर्ण_अगर


#अगर_अघोषित HB_DEBUG_DISPATCH
#घोषणा HB_DEBUG_DISPATCH ( \
	HB_DEBUG_APPLY + \
	HB_DEBUG_SANITIZE + \
	HB_DEBUG_SERIALIZE + \
	HB_DEBUG_SUBSET + \
	HB_DEBUG_PAINT + \
	0)
#पूर्ण_अगर
#अगर HB_DEBUG_DISPATCH
#घोषणा TRACE_DISPATCH(यह, format) \
	hb_auto_trace_t<context_t::max_debug_depth, typename context_t::return_t> trace \
	(&c->debug_depth, c->get_name (), यह, HB_FUNC, \
	 "format %u", (अचिन्हित) format)
#अन्यथा
#घोषणा TRACE_DISPATCH(यह, format) hb_no_trace_t<typename context_t::return_t> trace
#पूर्ण_अगर


#अगर_अघोषित HB_बफर_MESSAGE_MORE
#घोषणा HB_बफर_MESSAGE_MORE (HB_DEBUG+0)
#पूर्ण_अगर


#पूर्ण_अगर /* HB_DEBUG_HH */
