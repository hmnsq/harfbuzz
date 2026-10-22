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

#ifndef HB_SANITIZE_HH
#define HB_SANITIZE_HH

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-blob.hh"
#अन्तर्भाव "hb-dispatch.hh"


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
#ifndef HB_SANITIZE_MAX_EDITS
#define HB_SANITIZE_MAX_EDITS 32
#endif
#ifndef HB_SANITIZE_MAX_OPS_FACTOR
#define HB_SANITIZE_MAX_OPS_FACTOR 64
#endif
#ifndef HB_SANITIZE_MAX_OPS_MIN
#define HB_SANITIZE_MAX_OPS_MIN 16384
#endif
#ifndef HB_SANITIZE_MAX_OPS_MAX
#define HB_SANITIZE_MAX_OPS_MAX 0x3FFFFFFF
#endif
#ifndef HB_SANITIZE_MAX_SUBTABLES
#define HB_SANITIZE_MAX_SUBTABLES 0x4000
#endif

संरचना hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, तार्किक, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (स्थिरांक अक्षर *start_ = nullptr, स्थिरांक अक्षर *end_ = nullptr) :
	start (start_), end (end_),
	length (0),
	max_ops (0), max_subtables (0),
        recursion_depth (0),
	writable (असत्य),
	blob (nullptr),
	अंक_glyphs (65536),
	अंक_glyphs_set (असत्य),
	lazy_some_gpos (असत्य) {}

  स्थिरांक अक्षर *get_name () { परत "SANITIZE"; }
  ढाँचा <typename T, typename F>
  तार्किक may_dispatch (स्थिरांक T *obj HB_UNUSED, स्थिरांक F *format)
  {
    परत format->sanitize (हे) &&
	   hb_barrier ();
  }
  स्थिर return_t default_return_मूल्य () { परत सत्य; }
  स्थिर return_t no_dispatch_return_मूल्य () { परत असत्य; }
  तार्किक stop_sublookup_iteration (स्थिरांक return_t r) स्थिरांक { परत !r; }

  तार्किक visit_subtables (अचिन्हित मोजणी)
  {
    max_subtables += मोजणी;
    परत max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  खाजगी:
  ढाँचा <typename T, typename ...Ts> स्वतः
  _dispatch (स्थिरांक T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (हे, std::forward<Ts> (ds)...) )
  ढाँचा <typename T, typename ...Ts> स्वतः
  _dispatch (स्थिरांक T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (हे, std::forward<Ts> (ds)...) )
  सार्वजनिक:
  ढाँचा <typename T, typename ...Ts> स्वतः
  dispatch (स्थिरांक T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    जर (blob)
      start_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    जर (blob)
      end_processing ();
  }

  शून्य init (hb_blob_t *b)
  {
    हे->blob = hb_blob_reference (b);
    हे->writable = असत्य;
  }

  शून्य set_अंक_glyphs (अचिन्हित पूर्णांक अंक_glyphs_2)
  {
    अंक_glyphs = अंक_glyphs_2;
    अंक_glyphs_set = सत्य;
  }
  अचिन्हित पूर्णांक get_अंक_glyphs () { परत अंक_glyphs; }

  शून्य set_max_ops (पूर्णांक max_ops_) { max_ops = max_ops_; }

  ढाँचा <typename T>
  शून्य set_object (स्थिरांक T *obj)
  {
    reset_object ();

    जर (!obj) परत;

    स्थिरांक अक्षर *obj_start = (स्थिरांक अक्षर *) obj;
    जर (unlikely (obj_start < हे->start || हे->end <= obj_start))
    {
      हे->start = हे->end = nullptr;
      हे->length = 0;
    }
    अन्यथा
    {
      हे->start = obj_start;
      हे->end   = obj_start + hb_min (माप_प्रकार (हे->end - obj_start), obj->get_size ());
      हे->length = हे->end - हे->start;
    }
  }

  शून्य reset_object ()
  {
    जर (हे->blob)
    {
      हे->start = हे->blob->data;
      हे->end = हे->start + हे->blob->length;
    }
    हे->length = हे->end - हे->start;
    प्रतिज्ञा (हे->start <= हे->end); /* Must not overflow. */
  }

  शून्य start_processing (स्थिरांक अक्षर *start_ = nullptr, स्थिरांक अक्षर *end_ = nullptr)
  {
    जर (start_)
    {
      हे->start = start_;
      हे->end = end_;
    }
    reset_object ();
    अचिन्हित m;
    जर (unlikely (hb_unsigned_mul_overflows (हे->end - हे->start, HB_SANITIZE_MAX_OPS_FACTOR, &m)))
      हे->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    अन्यथा
      हे->max_ops = hb_clamp (m,
				(अचिन्हित) HB_SANITIZE_MAX_OPS_MIN,
				(अचिन्हित) HB_SANITIZE_MAX_OPS_MAX);
    हे->debug_depth = 0;
    हे->recursion_depth = 0;

    DEBUG_संदेश_LEVEL (SANITIZE, start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     हे->start, हे->end,
		     (अचिन्हित दीर्घ) (हे->end - हे->start));
  }

  शून्य end_processing ()
  {
    DEBUG_संदेश_LEVEL (SANITIZE, हे->start, 0, -1,
		     "end [%p..%p]",
		     हे->start, हे->end);

    hb_blob_destroy (हे->blob);
    हे->blob = nullptr;
    हे->start = हे->end = nullptr;
    हे->length = 0;
  }

  तार्किक check_ops(अचिन्हित मोजणी)
  {
    /* Avoid underflow */
    जर (unlikely (हे->max_ops < 0 || मोजणी >= (अचिन्हित) हे->max_ops))
    {
      हे->max_ops = -1;
      परत असत्य;
    }
    हे->max_ops -= (पूर्णांक) मोजणी;
    परत सत्य;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  तार्किक check_range (स्थिरांक शून्य *base,
		    अचिन्हित पूर्णांक len) स्थिरांक
  {
    स्थिरांक अक्षर *p = (स्थिरांक अक्षर *) base;
    तार्किक ok = (uintptr_t) (p - हे->start) <= हे->length &&
	      (अचिन्हित पूर्णांक) (हे->end - p) >= len &&
	      ((हे->max_ops -= len) > 0);

    DEBUG_संदेश_LEVEL (SANITIZE, p, हे->debug_depth+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     हे->start, हे->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    परत likely (ok);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  तार्किक check_range_fast (स्थिरांक शून्य *base,
			 अचिन्हित पूर्णांक len) स्थिरांक
  {
    स्थिरांक अक्षर *p = (स्थिरांक अक्षर *) base;
    तार्किक ok = ((uintptr_t) (p - हे->start) <= हे->length &&
	       (अचिन्हित पूर्णांक) (हे->end - p) >= len);

    DEBUG_संदेश_LEVEL (SANITIZE, p, हे->debug_depth+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     हे->start, हे->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    परत likely (ok);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  तार्किक check_point (स्थिरांक शून्य *base) स्थिरांक
  {
    स्थिरांक अक्षर *p = (स्थिरांक अक्षर *) base;
    तार्किक ok = (uintptr_t) (p - हे->start) <= हे->length;

    DEBUG_संदेश_LEVEL (SANITIZE, p, हे->debug_depth+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     हे->start, हे->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    परत likely (ok);
  }

  ढाँचा <typename T>
  तार्किक check_range (स्थिरांक T *base,
		    अचिन्हित पूर्णांक a,
		    अचिन्हित पूर्णांक b) स्थिरांक
  {
    अचिन्हित m;
    परत !hb_unsigned_mul_overflows (a, b, &m) &&
	   हे->check_range (base, m);
  }

  ढाँचा <typename T>
  तार्किक check_range (स्थिरांक T *base,
		    अचिन्हित पूर्णांक a,
		    अचिन्हित पूर्णांक b,
		    अचिन्हित पूर्णांक c) स्थिरांक
  {
    अचिन्हित m;
    परत !hb_unsigned_mul_overflows (a, b, &m) &&
	   हे->check_range (base, m, c);
  }

  ढाँचा <typename T>
  HB_ALWAYS_INLINE
  तार्किक check_array_sized (स्थिरांक T *base, अचिन्हित पूर्णांक len, अचिन्हित len_size) स्थिरांक
  {
    जर (len_size >= 4)
    {
      जर (unlikely (hb_unsigned_mul_overflows (len, hb_static_size (T), &len)))
	परत असत्य;
    }
    अन्यथा
      len = len * hb_static_size (T);
    परत हे->check_range (base, len);
  }

  ढाँचा <typename T>
  तार्किक check_array (स्थिरांक T *base, अचिन्हित पूर्णांक len) स्थिरांक
  {
    परत हे->check_range (base, len, hb_static_size (T));
  }

  ढाँचा <typename T>
  तार्किक check_array (स्थिरांक T *base,
		    अचिन्हित पूर्णांक a,
		    अचिन्हित पूर्णांक b) स्थिरांक
  {
    परत हे->check_range (base, hb_static_size (T), a, b);
  }

  तार्किक check_start_recursion (पूर्णांक max_depth)
  {
    जर (unlikely (recursion_depth >= max_depth)) परत असत्य;
    परत ++recursion_depth;
  }

  तार्किक end_recursion (तार्किक result)
  {
    recursion_depth--;
    परत result;
  }

  ढाँचा <typename Type>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  तार्किक check_struct (स्थिरांक Type *obj) स्थिरांक
  {
    जर (आकार (uintptr_t) == आकार (uint32_t))
      परत likely (हे->check_range_fast (obj, obj->min_size));
    अन्यथा
      परत likely (हे->check_point ((स्थिरांक अक्षर *) obj + obj->min_size));
  }

  ढाँचा <typename Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    तार्किक sane;

    init (blob);

    DEBUG_संदेश_FUNC (SANITIZE, start, "start");

    start_processing ();

    जर (unlikely (!start))
    {
      end_processing ();
      परत blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<अक्षर *> (start));

    sane = t->sanitize (हे);

    end_processing ();

    DEBUG_संदेश_FUNC (SANITIZE, start, sane ? "PASSED" : "FAILED");
    जर (sane)
    {
      hb_blob_make_immutable (blob);
      परत blob;
    }
    अन्यथा
    {
      hb_blob_destroy (blob);
      परत hb_blob_get_empty ();
    }
  }

  ढाँचा <typename Type>
  hb_blob_t *reference_table (स्थिरांक hb_face_t *face, hb_tag_t tableTag = Type::tableTag)
  {
    जर (!अंक_glyphs_set)
      set_अंक_glyphs (hb_face_get_glyph_मोजणी (face));
    परत sanitize_blob<Type> (hb_face_reference_table (face, tableTag));
  }

  स्थिरांक अक्षर *start, *end;
  अचिन्हित length;
  mutable पूर्णांक max_ops, max_subtables;
  खाजगी:
  पूर्णांक recursion_depth;
  तार्किक writable;
  hb_blob_t *blob;
  अचिन्हित पूर्णांक अंक_glyphs;
  तार्किक  अंक_glyphs_set;
  सार्वजनिक:
  तार्किक lazy_some_gpos;
};

संरचना hb_sanitize_with_object_t
{
  ढाँचा <typename T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, स्थिरांक T& obj) : c (c)
  { c->set_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  खाजगी:
  hb_sanitize_context_t *c;
};


#endif /* HB_SANITIZE_HH */
