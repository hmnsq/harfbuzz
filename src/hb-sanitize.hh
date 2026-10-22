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

#अगर_अघोषित HB_SANITIZE_HH
#घोषणा HB_SANITIZE_HH

#समावेश "hb.hh"
#समावेश "hb-blob.hh"
#समावेश "hb-dispatch.hh"


/*
 * Sanitize
 *
 *
 * === Introduction ===
 *
 * The sanitize machinery is at the core of our zero-cost font loading.  We
 * mmap() font file into memory and create a blob out of it.  Font subtables
 * are returned as a readonly sub-blob of the main font blob.  These table
 * blobs are then sanitized before use, to ensure invalid memory access does
 * not happen.  The toplevel sanitize API use is like, eg. to load the 'head'
 * table:
 *
 *   hb_blob_t *head_blob = hb_sanitize_context_t ().reference_table<OT::head> (face);
 *
 * The blob then can be converted to a head table struct with:
 *
 *   const head *head_table = head_blob->as<head> ();
 *
 * What the reference_table does is, to call hb_face_reference_table() to load
 * the table blob, sanitize it and return either the sanitized blob, or empty
 * blob if sanitization failed.  The blob->as() function returns the null
 * object of its template type argument if the blob is empty.  Otherwise, it
 * just casts the blob contents to the desired type.
 *
 * Sanitizing a blob of data with a type T works as follows (with minor
 * simplification):
 *
 *   - Cast blob content to T*, call sanitize() method of it,
 *   - If sanitize succeeded, return blob.
 *   - Return empty blob otherwise.
 *
 *
 * === The sanitize() contract ===
 *
 * The sanitize() method of each object type shall return `true` if it's safe to
 * call other methods of the object, and `false` otherwise.
 *
 * Note that what sanitize() checks for might align with what the specification
 * describes as valid table data, but does not have to be.  In particular, we
 * do NOT want to be pedantic and concern ourselves with validity checks that
 * are irrelevant to our use of the table.  On the contrary, we want to be
 * lenient with error handling and accept invalid data to the extent that it
 * does not impose extra burden on us.
 *
 * Based on the sanitize contract, one can see that what we check for depends
 * on how we use the data in other table methods.  Ie. if other table methods
 * assume that offsets do NOT point out of the table data block, then that's
 * something sanitize() must check for (GSUB/GPOS/GDEF/etc work this way).  On
 * the other hand, if other methods do such checks themselves, then sanitize()
 * does not have to bother with them (glyf/local work this way).  The choice
 * depends on the table structure and sanitize() performance.  For example, to
 * check glyf/loca offsets in sanitize() would cost O(num-glyphs).  We try hard
 * to avoid such costs during font loading.  By postponing such checks to the
 * actual glyph loading, we reduce the sanitize cost to O(1) and total runtime
 * cost to O(used-glyphs).  As such, this is preferred.
 *
 * The same argument can be made re GSUB/GPOS/GDEF, but there, the table
 * structure is so complicated that by checking all offsets at sanitize() time,
 * we make the code much simpler in other methods, as offsets and referenced
 * objects do not need to be validated at each use site.
 *
 * Note:
 * Sanitize was named so because it used to try to recover from errors by
 * modifying the data to make it valid.  This is no longer the case, as it
 * could make HarfBuzz hallucinate new rules if there was aliasing in the
 * data.  However, the name stuck.  See: https://behdad.github.io/harfbust/
 */

/* This limits sanitizing time on really broken fonts. */
#अगर_अघोषित HB_SANITIZE_MAX_EDITS
#घोषणा HB_SANITIZE_MAX_EDITS 32
#पूर्ण_अगर
#अगर_अघोषित HB_SANITIZE_MAX_OPS_FACTOR
#घोषणा HB_SANITIZE_MAX_OPS_FACTOR 64
#पूर्ण_अगर
#अगर_अघोषित HB_SANITIZE_MAX_OPS_MIN
#घोषणा HB_SANITIZE_MAX_OPS_MIN 16384
#पूर्ण_अगर
#अगर_अघोषित HB_SANITIZE_MAX_OPS_MAX
#घोषणा HB_SANITIZE_MAX_OPS_MAX 0x3FFFFFFF
#पूर्ण_अगर
#अगर_अघोषित HB_SANITIZE_MAX_SUBTABLES
#घोषणा HB_SANITIZE_MAX_SUBTABLES 0x4000
#पूर्ण_अगर

काष्ठा hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, bool, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (स्थिर अक्षर *start_ = nullptr, स्थिर अक्षर *end_ = nullptr) :
	start (start_), end (end_),
	लंबाई (0),
	max_ops (0), max_subtables (0),
        recursion_depth (0),
	writable (false),
	blob (nullptr),
	अंक_glyphs (65536),
	अंक_glyphs_set (false),
	lazy_some_gpos (false) {}

  स्थिर अक्षर *get_name () { वापस "SANITIZE"; }
  ढाँचा <typename T, typename F>
  bool may_dispatch (स्थिर T *obj HB_UNUSED, स्थिर F *format)
  {
    वापस format->sanitize (यह) &&
	   hb_barrier ();
  }
  अटल return_t default_return_मूल्य () { वापस true; }
  अटल return_t no_dispatch_return_मूल्य () { वापस false; }
  bool stop_sublookup_iteration (स्थिर return_t r) स्थिर { वापस !r; }

  bool visit_subtables (अचिन्हित गिनती)
  {
    max_subtables += गिनती;
    वापस max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  निजी:
  ढाँचा <typename T, typename ...Ts> स्वतः
  _dispatch (स्थिर T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (यह, std::forward<Ts> (ds)...) )
  ढाँचा <typename T, typename ...Ts> स्वतः
  _dispatch (स्थिर T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (यह, std::forward<Ts> (ds)...) )
  खुला:
  ढाँचा <typename T, typename ...Ts> स्वतः
  dispatch (स्थिर T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    अगर (blob)
      start_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    अगर (blob)
      end_processing ();
  }

  व्योम init (hb_blob_t *b)
  {
    यह->blob = hb_blob_reference (b);
    यह->writable = false;
  }

  व्योम set_अंक_glyphs (अचिन्हित पूर्णांक अंक_glyphs_2)
  {
    अंक_glyphs = अंक_glyphs_2;
    अंक_glyphs_set = true;
  }
  अचिन्हित पूर्णांक get_अंक_glyphs () { वापस अंक_glyphs; }

  व्योम set_max_ops (पूर्णांक max_ops_) { max_ops = max_ops_; }

  ढाँचा <typename T>
  व्योम set_object (स्थिर T *obj)
  {
    reset_object ();

    अगर (!obj) वापस;

    स्थिर अक्षर *obj_start = (स्थिर अक्षर *) obj;
    अगर (unlikely (obj_start < यह->start || यह->end <= obj_start))
    {
      यह->start = यह->end = nullptr;
      यह->लंबाई = 0;
    }
    अन्यथा
    {
      यह->start = obj_start;
      यह->end   = obj_start + hb_min (माप_प्रकार (यह->end - obj_start), obj->get_आकार ());
      यह->लंबाई = यह->end - यह->start;
    }
  }

  व्योम reset_object ()
  {
    अगर (यह->blob)
    {
      यह->start = यह->blob->तथ्य;
      यह->end = यह->start + यह->blob->लंबाई;
    }
    यह->लंबाई = यह->end - यह->start;
    निश्चित (यह->start <= यह->end); /* Must not overflow. */
  }

  व्योम start_processing (स्थिर अक्षर *start_ = nullptr, स्थिर अक्षर *end_ = nullptr)
  {
    अगर (start_)
    {
      यह->start = start_;
      यह->end = end_;
    }
    reset_object ();
    अचिन्हित m;
    अगर (unlikely (hb_unsigned_mul_overflows (यह->end - यह->start, HB_SANITIZE_MAX_OPS_FACTOR, &m)))
      यह->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    अन्यथा
      यह->max_ops = hb_clamp (m,
				(अचिन्हित) HB_SANITIZE_MAX_OPS_MIN,
				(अचिन्हित) HB_SANITIZE_MAX_OPS_MAX);
    यह->debug_depth = 0;
    यह->recursion_depth = 0;

    DEBUG_संदेश_LEVEL (SANITIZE, start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     यह->start, यह->end,
		     (अचिन्हित दीर्घ) (यह->end - यह->start));
  }

  व्योम end_processing ()
  {
    DEBUG_संदेश_LEVEL (SANITIZE, यह->start, 0, -1,
		     "end [%p..%p]",
		     यह->start, यह->end);

    hb_blob_destroy (यह->blob);
    यह->blob = nullptr;
    यह->start = यह->end = nullptr;
    यह->लंबाई = 0;
  }

  bool check_ops(अचिन्हित गिनती)
  {
    /* Avoid underflow */
    अगर (unlikely (यह->max_ops < 0 || गिनती >= (अचिन्हित) यह->max_ops))
    {
      यह->max_ops = -1;
      वापस false;
    }
    यह->max_ops -= (पूर्णांक) गिनती;
    वापस true;
  }

#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  bool check_range (स्थिर व्योम *base,
		    अचिन्हित पूर्णांक len) स्थिर
  {
    स्थिर अक्षर *p = (स्थिर अक्षर *) base;
    bool ok = (uintptr_t) (p - यह->start) <= यह->लंबाई &&
	      (अचिन्हित पूर्णांक) (यह->end - p) >= len &&
	      ((यह->max_ops -= len) > 0);

    DEBUG_संदेश_LEVEL (SANITIZE, p, यह->debug_depth+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     यह->start, यह->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    वापस likely (ok);
  }
#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  bool check_range_fast (स्थिर व्योम *base,
			 अचिन्हित पूर्णांक len) स्थिर
  {
    स्थिर अक्षर *p = (स्थिर अक्षर *) base;
    bool ok = ((uintptr_t) (p - यह->start) <= यह->लंबाई &&
	       (अचिन्हित पूर्णांक) (यह->end - p) >= len);

    DEBUG_संदेश_LEVEL (SANITIZE, p, यह->debug_depth+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     यह->start, यह->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    वापस likely (ok);
  }

#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  bool check_point (स्थिर व्योम *base) स्थिर
  {
    स्थिर अक्षर *p = (स्थिर अक्षर *) base;
    bool ok = (uintptr_t) (p - यह->start) <= यह->लंबाई;

    DEBUG_संदेश_LEVEL (SANITIZE, p, यह->debug_depth+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     यह->start, यह->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    वापस likely (ok);
  }

  ढाँचा <typename T>
  bool check_range (स्थिर T *base,
		    अचिन्हित पूर्णांक a,
		    अचिन्हित पूर्णांक b) स्थिर
  {
    अचिन्हित m;
    वापस !hb_unsigned_mul_overflows (a, b, &m) &&
	   यह->check_range (base, m);
  }

  ढाँचा <typename T>
  bool check_range (स्थिर T *base,
		    अचिन्हित पूर्णांक a,
		    अचिन्हित पूर्णांक b,
		    अचिन्हित पूर्णांक c) स्थिर
  {
    अचिन्हित m;
    वापस !hb_unsigned_mul_overflows (a, b, &m) &&
	   यह->check_range (base, m, c);
  }

  ढाँचा <typename T>
  HB_ALWAYS_INLINE
  bool check_array_sized (स्थिर T *base, अचिन्हित पूर्णांक len, अचिन्हित len_आकार) स्थिर
  {
    अगर (len_आकार >= 4)
    {
      अगर (unlikely (hb_unsigned_mul_overflows (len, hb_static_आकार (T), &len)))
	वापस false;
    }
    अन्यथा
      len = len * hb_static_आकार (T);
    वापस यह->check_range (base, len);
  }

  ढाँचा <typename T>
  bool check_array (स्थिर T *base, अचिन्हित पूर्णांक len) स्थिर
  {
    वापस यह->check_range (base, len, hb_static_आकार (T));
  }

  ढाँचा <typename T>
  bool check_array (स्थिर T *base,
		    अचिन्हित पूर्णांक a,
		    अचिन्हित पूर्णांक b) स्थिर
  {
    वापस यह->check_range (base, hb_static_आकार (T), a, b);
  }

  bool check_start_recursion (पूर्णांक max_depth)
  {
    अगर (unlikely (recursion_depth >= max_depth)) वापस false;
    वापस ++recursion_depth;
  }

  bool end_recursion (bool परिणाम)
  {
    recursion_depth--;
    वापस परिणाम;
  }

  ढाँचा <typename Type>
#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  bool check_struct (स्थिर Type *obj) स्थिर
  {
    अगर (माप (uintptr_t) == माप (uint32_t))
      वापस likely (यह->check_range_fast (obj, obj->min_आकार));
    अन्यथा
      वापस likely (यह->check_point ((स्थिर अक्षर *) obj + obj->min_आकार));
  }

  ढाँचा <typename Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    bool sane;

    init (blob);

    DEBUG_संदेश_FUNC (SANITIZE, start, "start");

    start_processing ();

    अगर (unlikely (!start))
    {
      end_processing ();
      वापस blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<अक्षर *> (start));

    sane = t->sanitize (यह);

    end_processing ();

    DEBUG_संदेश_FUNC (SANITIZE, start, sane ? "PASSED" : "FAILED");
    अगर (sane)
    {
      hb_blob_make_immutable (blob);
      वापस blob;
    }
    अन्यथा
    {
      hb_blob_destroy (blob);
      वापस hb_blob_get_empty ();
    }
  }

  ढाँचा <typename Type>
  hb_blob_t *reference_table (स्थिर hb_face_t *face, hb_tag_t tableTag = Type::tableTag)
  {
    अगर (!अंक_glyphs_set)
      set_अंक_glyphs (hb_face_get_glyph_गिनती (face));
    वापस sanitize_blob<Type> (hb_face_reference_table (face, tableTag));
  }

  स्थिर अक्षर *start, *end;
  अचिन्हित लंबाई;
  mutable पूर्णांक max_ops, max_subtables;
  निजी:
  पूर्णांक recursion_depth;
  bool writable;
  hb_blob_t *blob;
  अचिन्हित पूर्णांक अंक_glyphs;
  bool  अंक_glyphs_set;
  खुला:
  bool lazy_some_gpos;
};

काष्ठा hb_sanitize_with_object_t
{
  ढाँचा <typename T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, स्थिर T& obj) : c (c)
  { c->set_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  निजी:
  hb_sanitize_context_t *c;
};


#पूर्ण_अगर /* HB_SANITIZE_HH */
