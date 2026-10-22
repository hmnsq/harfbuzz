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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-blob.hh"
#ଅନ୍ତର୍ଭାଵ "hb-dispatch.hh"


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

ସଂରଚନା hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, ତାର୍କିକ, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (ଅଚଳ ଅକ୍ଷର *start_ = nullptr, ଅଚଳ ଅକ୍ଷର *end_ = nullptr) :
	start (start_), end (end_),
	length (0),
	max_ops (0), max_subtables (0),
        recursion_depth (0),
	writable (ମିଥ୍ୟା),
	blob (nullptr),
	ଅଙ୍କ_glyphs (65536),
	ଅଙ୍କ_glyphs_set (ମିଥ୍ୟା),
	lazy_some_gpos (ମିଥ୍ୟା) {}

  ଅଚଳ ଅକ୍ଷର *get_name () { ଫେରନ୍ତୁ "SANITIZE"; }
  template <typename T, typename F>
  ତାର୍କିକ may_dispatch (ଅଚଳ T *obj HB_UNUSED, ଅଚଳ F *format)
  {
    ଫେରନ୍ତୁ format->sanitize (ଏହି) &&
	   hb_barrier ();
  }
  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ ସତ୍ୟ; }
  ସ୍ଥିର return_t no_dispatch_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
  ତାର୍କିକ stop_sublookup_iteration (ଅଚଳ return_t r) ଅଚଳ { ଫେରନ୍ତୁ !r; }

  ତାର୍କିକ visit_subtables (unsigned ଗଣନା_2)
  {
    max_subtables += ଗଣନା_2;
    ଫେରନ୍ତୁ max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  ବ୍ୟକ୍ତିଗତ:
  template <typename T, typename ...Ts> auto
  _dispatch (ଅଚଳ T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (ଏହି, std::forward<Ts> (ds)...) )
  template <typename T, typename ...Ts> auto
  _dispatch (ଅଚଳ T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (ଏହି, std::forward<Ts> (ds)...) )
  ସର୍ବସାଧାରଣ:
  template <typename T, typename ...Ts> auto
  dispatch (ଅଚଳ T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    ଯଦି (blob)
      start_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    ଯଦି (blob)
      end_processing ();
  }

  ଶୂନ୍ୟ init (hb_blob_t *b)
  {
    ଏହି->blob = hb_blob_reference (b);
    ଏହି->writable = ମିଥ୍ୟା;
  }

  ଶୂନ୍ୟ set_ଅଙ୍କ_glyphs (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs_2)
  {
    ଅଙ୍କ_glyphs = ଅଙ୍କ_glyphs_2;
    ଅଙ୍କ_glyphs_set = ସତ୍ୟ;
  }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ଅଙ୍କ_glyphs () { ଫେରନ୍ତୁ ଅଙ୍କ_glyphs; }

  ଶୂନ୍ୟ set_max_ops (ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_ops_) { max_ops = max_ops_; }

  template <typename T>
  ଶୂନ୍ୟ set_object (ଅଚଳ T *obj)
  {
    reset_object ();

    ଯଦି (!obj) ଫେରନ୍ତୁ;

    ଅଚଳ ଅକ୍ଷର *obj_start = (ଅଚଳ ଅକ୍ଷର *) obj;
    ଯଦି (unlikely (obj_start < ଏହି->start || ଏହି->end <= obj_start))
    {
      ଏହି->start = ଏହି->end = nullptr;
      ଏହି->length = 0;
    }
    ଅନ୍ୟଥା
    {
      ଏହି->start = obj_start;
      ଏହି->end   = obj_start + hb_min (size_t (ଏହି->end - obj_start), obj->get_size ());
      ଏହି->length = ଏହି->end - ଏହି->start;
    }
  }

  ଶୂନ୍ୟ reset_object ()
  {
    ଯଦି (ଏହି->blob)
    {
      ଏହି->start = ଏହି->blob->data;
      ଏହି->end = ଏହି->start + ଏହି->blob->length;
    }
    ଏହି->length = ଏହି->end - ଏହି->start;
    ପ୍ରତିଜ୍ଞା (ଏହି->start <= ଏହି->end); /* Must not overflow. */
  }

  ଶୂନ୍ୟ start_processing (ଅଚଳ ଅକ୍ଷର *start_ = nullptr, ଅଚଳ ଅକ୍ଷର *end_ = nullptr)
  {
    ଯଦି (start_)
    {
      ଏହି->start = start_;
      ଏହି->end = end_;
    }
    reset_object ();
    unsigned m;
    ଯଦି (unlikely (hb_unsigned_mul_overflows (ଏହି->end - ଏହି->start, HB_SANITIZE_MAX_OPS_FACTOR, &m)))
      ଏହି->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    ଅନ୍ୟଥା
      ଏହି->max_ops = hb_clamp (m,
				(unsigned) HB_SANITIZE_MAX_OPS_MIN,
				(unsigned) HB_SANITIZE_MAX_OPS_MAX);
    ଏହି->debug_depth = 0;
    ଏହି->recursion_depth = 0;

    DEBUG_ସନ୍ଦେଶ_LEVEL (SANITIZE, start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     ଏହି->start, ଏହି->end,
		     (unsigned long) (ଏହି->end - ଏହି->start));
  }

  ଶୂନ୍ୟ end_processing ()
  {
    DEBUG_ସନ୍ଦେଶ_LEVEL (SANITIZE, ଏହି->start, 0, -1,
		     "end [%p..%p]",
		     ଏହି->start, ଏହି->end);

    hb_blob_destroy (ଏହି->blob);
    ଏହି->blob = nullptr;
    ଏହି->start = ଏହି->end = nullptr;
    ଏହି->length = 0;
  }

  ତାର୍କିକ check_ops(unsigned ଗଣନା_2)
  {
    /* Avoid underflow */
    ଯଦି (unlikely (ଏହି->max_ops < 0 || ଗଣନା_2 >= (unsigned) ଏହି->max_ops))
    {
      ଏହି->max_ops = -1;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଏହି->max_ops -= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଗଣନା_2;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ତାର୍କିକ check_range (ଅଚଳ ଶୂନ୍ୟ *base,
		    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) ଅଚଳ
  {
    ଅଚଳ ଅକ୍ଷର *p = (ଅଚଳ ଅକ୍ଷର *) base;
    ତାର୍କିକ ok = (uintptr_t) (p - ଏହି->start) <= ଏହି->length &&
	      (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (ଏହି->end - p) >= len &&
	      ((ଏହି->max_ops -= len) > 0);

    DEBUG_ସନ୍ଦେଶ_LEVEL (SANITIZE, p, ଏହି->debug_depth+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     ଏହି->start, ଏହି->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    ଫେରନ୍ତୁ likely (ok);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ତାର୍କିକ check_range_fast (ଅଚଳ ଶୂନ୍ୟ *base,
			 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) ଅଚଳ
  {
    ଅଚଳ ଅକ୍ଷର *p = (ଅଚଳ ଅକ୍ଷର *) base;
    ତାର୍କିକ ok = ((uintptr_t) (p - ଏହି->start) <= ଏହି->length &&
	       (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (ଏହି->end - p) >= len);

    DEBUG_ସନ୍ଦେଶ_LEVEL (SANITIZE, p, ଏହି->debug_depth+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     ଏହି->start, ଏହି->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    ଫେରନ୍ତୁ likely (ok);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ତାର୍କିକ check_point (ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    ଅଚଳ ଅକ୍ଷର *p = (ଅଚଳ ଅକ୍ଷର *) base;
    ତାର୍କିକ ok = (uintptr_t) (p - ଏହି->start) <= ଏହି->length;

    DEBUG_ସନ୍ଦେଶ_LEVEL (SANITIZE, p, ଏହି->debug_depth+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     ଏହି->start, ଏହି->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    ଫେରନ୍ତୁ likely (ok);
  }

  template <typename T>
  ତାର୍କିକ check_range (ଅଚଳ T *base,
		    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା a,
		    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା b) ଅଚଳ
  {
    unsigned m;
    ଫେରନ୍ତୁ !hb_unsigned_mul_overflows (a, b, &m) &&
	   ଏହି->check_range (base, m);
  }

  template <typename T>
  ତାର୍କିକ check_range (ଅଚଳ T *base,
		    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା a,
		    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା b,
		    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା c) ଅଚଳ
  {
    unsigned m;
    ଫେରନ୍ତୁ !hb_unsigned_mul_overflows (a, b, &m) &&
	   ଏହି->check_range (base, m, c);
  }

  template <typename T>
  HB_ALWAYS_INLINE
  ତାର୍କିକ check_array_sized (ଅଚଳ T *base, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, unsigned len_size) ଅଚଳ
  {
    ଯଦି (len_size >= 4)
    {
      ଯଦି (unlikely (hb_unsigned_mul_overflows (len, hb_static_size (T), &len)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଅନ୍ୟଥା
      len = len * hb_static_size (T);
    ଫେରନ୍ତୁ ଏହି->check_range (base, len);
  }

  template <typename T>
  ତାର୍କିକ check_array (ଅଚଳ T *base, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) ଅଚଳ
  {
    ଫେରନ୍ତୁ ଏହି->check_range (base, len, hb_static_size (T));
  }

  template <typename T>
  ତାର୍କିକ check_array (ଅଚଳ T *base,
		    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା a,
		    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା b) ଅଚଳ
  {
    ଫେରନ୍ତୁ ଏହି->check_range (base, hb_static_size (T), a, b);
  }

  ତାର୍କିକ check_start_recursion (ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_depth)
  {
    ଯଦି (unlikely (recursion_depth >= max_depth)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ++recursion_depth;
  }

  ତାର୍କିକ end_recursion (ତାର୍କିକ result)
  {
    recursion_depth--;
    ଫେରନ୍ତୁ result;
  }

  template <typename Type>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ତାର୍କିକ check_struct (ଅଚଳ Type *obj) ଅଚଳ
  {
    ଯଦି (ଆକାର (uintptr_t) == ଆକାର (uint32_t))
      ଫେରନ୍ତୁ likely (ଏହି->check_range_fast (obj, obj->min_size));
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ likely (ଏହି->check_point ((ଅଚଳ ଅକ୍ଷର *) obj + obj->min_size));
  }

  template <typename Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    ତାର୍କିକ sane;

    init (blob);

    DEBUG_ସନ୍ଦେଶ_FUNC (SANITIZE, start, "start");

    start_processing ();

    ଯଦି (unlikely (!start))
    {
      end_processing ();
      ଫେରନ୍ତୁ blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<ଅକ୍ଷର *> (start));

    sane = t->sanitize (ଏହି);

    end_processing ();

    DEBUG_ସନ୍ଦେଶ_FUNC (SANITIZE, start, sane ? "PASSED" : "FAILED");
    ଯଦି (sane)
    {
      hb_blob_make_immutable (blob);
      ଫେରନ୍ତୁ blob;
    }
    ଅନ୍ୟଥା
    {
      hb_blob_destroy (blob);
      ଫେରନ୍ତୁ hb_blob_get_empty ();
    }
  }

  template <typename Type>
  hb_blob_t *reference_table (ଅଚଳ hb_face_t *face, hb_tag_t tableTag = Type::tableTag)
  {
    ଯଦି (!ଅଙ୍କ_glyphs_set)
      set_ଅଙ୍କ_glyphs (hb_face_get_glyph_ଗଣନା (face));
    ଫେରନ୍ତୁ sanitize_blob<Type> (hb_face_reference_table (face, tableTag));
  }

  ଅଚଳ ଅକ୍ଷର *start, *end;
  unsigned length;
  mutable ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_ops, max_subtables;
  ବ୍ୟକ୍ତିଗତ:
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା recursion_depth;
  ତାର୍କିକ writable;
  hb_blob_t *blob;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs;
  ତାର୍କିକ  ଅଙ୍କ_glyphs_set;
  ସର୍ବସାଧାରଣ:
  ତାର୍କିକ lazy_some_gpos;
};

ସଂରଚନା hb_sanitize_with_object_t
{
  template <typename T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, ଅଚଳ T& obj) : c (c)
  { c->set_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  ବ୍ୟକ୍ତିଗତ:
  hb_sanitize_context_t *c;
};


#endif /* HB_SANITIZE_HH */
