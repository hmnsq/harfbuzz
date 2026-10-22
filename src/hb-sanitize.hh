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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-blob.hh"
#অন্তর্ভাব "hb-dispatch.hh"


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

কাঠামো hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, যৌক্তিক, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (ধ্রুবক অক্ষর *start_ = nullptr, ধ্রুবক অক্ষর *end_ = nullptr) :
	start (start_), end (end_),
	দৈর্ঘ্য (0),
	max_ops (0), max_subtables (0),
        recursion_depth (0),
	writable (মিথ্যা),
	blob (nullptr),
	অঙ্ক_glyphs (65536),
	অঙ্ক_glyphs_set (মিথ্যা),
	lazy_some_gpos (মিথ্যা) {}

  ধ্রুবক অক্ষর *get_name () { ফেরত "SANITIZE"; }
  template <typename T, typename F>
  যৌক্তিক may_dispatch (ধ্রুবক T *obj HB_UNUSED, ধ্রুবক F *format)
  {
    ফেরত format->sanitize (এই) &&
	   hb_barrier ();
  }
  স্থির return_t default_return_মান () { ফেরত সত্য; }
  স্থির return_t no_dispatch_return_মান () { ফেরত মিথ্যা; }
  যৌক্তিক stop_sublookup_iteration (ধ্রুবক return_t r) ধ্রুবক { ফেরত !r; }

  যৌক্তিক visit_subtables (unsigned গুনতি)
  {
    max_subtables += গুনতি;
    ফেরত max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  ব্যক্তিগত:
  template <typename T, typename ...Ts> auto
  _dispatch (ধ্রুবক T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (এই, std::forward<Ts> (ds)...) )
  template <typename T, typename ...Ts> auto
  _dispatch (ধ্রুবক T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (এই, std::forward<Ts> (ds)...) )
  সর্বজনীন:
  template <typename T, typename ...Ts> auto
  dispatch (ধ্রুবক T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    যদি (blob)
      start_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    যদি (blob)
      end_processing ();
  }

  শূন্য init (hb_blob_t *b)
  {
    এই->blob = hb_blob_reference (b);
    এই->writable = মিথ্যা;
  }

  শূন্য set_অঙ্ক_glyphs (unsigned পূর্ণসংখ্যা অঙ্ক_glyphs_2)
  {
    অঙ্ক_glyphs = অঙ্ক_glyphs_2;
    অঙ্ক_glyphs_set = সত্য;
  }
  unsigned পূর্ণসংখ্যা get_অঙ্ক_glyphs () { ফেরত অঙ্ক_glyphs; }

  শূন্য set_max_ops (পূর্ণসংখ্যা max_ops_) { max_ops = max_ops_; }

  template <typename T>
  শূন্য set_object (ধ্রুবক T *obj)
  {
    reset_object ();

    যদি (!obj) ফেরত;

    ধ্রুবক অক্ষর *obj_start = (ধ্রুবক অক্ষর *) obj;
    যদি (unlikely (obj_start < এই->start || এই->end <= obj_start))
    {
      এই->start = এই->end = nullptr;
      এই->দৈর্ঘ্য = 0;
    }
    অন্যথা
    {
      এই->start = obj_start;
      এই->end   = obj_start + hb_min (size_t (এই->end - obj_start), obj->get_আকার ());
      এই->দৈর্ঘ্য = এই->end - এই->start;
    }
  }

  শূন্য reset_object ()
  {
    যদি (এই->blob)
    {
      এই->start = এই->blob->তথ্য;
      এই->end = এই->start + এই->blob->দৈর্ঘ্য;
    }
    এই->দৈর্ঘ্য = এই->end - এই->start;
    প্রতিজ্ঞা (এই->start <= এই->end); /* Must not overflow. */
  }

  শূন্য start_processing (ধ্রুবক অক্ষর *start_ = nullptr, ধ্রুবক অক্ষর *end_ = nullptr)
  {
    যদি (start_)
    {
      এই->start = start_;
      এই->end = end_;
    }
    reset_object ();
    unsigned m;
    যদি (unlikely (hb_unsigned_mul_overflows (এই->end - এই->start, HB_SANITIZE_MAX_OPS_FACTOR, &m)))
      এই->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    অন্যথা
      এই->max_ops = hb_clamp (m,
				(unsigned) HB_SANITIZE_MAX_OPS_MIN,
				(unsigned) HB_SANITIZE_MAX_OPS_MAX);
    এই->debug_depth = 0;
    এই->recursion_depth = 0;

    DEBUG_বার্তা_LEVEL (SANITIZE, start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     এই->start, এই->end,
		     (unsigned long) (এই->end - এই->start));
  }

  শূন্য end_processing ()
  {
    DEBUG_বার্তা_LEVEL (SANITIZE, এই->start, 0, -1,
		     "end [%p..%p]",
		     এই->start, এই->end);

    hb_blob_destroy (এই->blob);
    এই->blob = nullptr;
    এই->start = এই->end = nullptr;
    এই->দৈর্ঘ্য = 0;
  }

  যৌক্তিক check_ops(unsigned গুনতি)
  {
    /* Avoid underflow */
    যদি (unlikely (এই->max_ops < 0 || গুনতি >= (unsigned) এই->max_ops))
    {
      এই->max_ops = -1;
      ফেরত মিথ্যা;
    }
    এই->max_ops -= (পূর্ণসংখ্যা) গুনতি;
    ফেরত সত্য;
  }

#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  যৌক্তিক check_range (ধ্রুবক শূন্য *base,
		    unsigned পূর্ণসংখ্যা len) ধ্রুবক
  {
    ধ্রুবক অক্ষর *p = (ধ্রুবক অক্ষর *) base;
    যৌক্তিক ok = (uintptr_t) (p - এই->start) <= এই->দৈর্ঘ্য &&
	      (unsigned পূর্ণসংখ্যা) (এই->end - p) >= len &&
	      ((এই->max_ops -= len) > 0);

    DEBUG_বার্তা_LEVEL (SANITIZE, p, এই->debug_depth+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     এই->start, এই->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    ফেরত likely (ok);
  }
#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  যৌক্তিক check_range_fast (ধ্রুবক শূন্য *base,
			 unsigned পূর্ণসংখ্যা len) ধ্রুবক
  {
    ধ্রুবক অক্ষর *p = (ধ্রুবক অক্ষর *) base;
    যৌক্তিক ok = ((uintptr_t) (p - এই->start) <= এই->দৈর্ঘ্য &&
	       (unsigned পূর্ণসংখ্যা) (এই->end - p) >= len);

    DEBUG_বার্তা_LEVEL (SANITIZE, p, এই->debug_depth+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     এই->start, এই->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    ফেরত likely (ok);
  }

#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  যৌক্তিক check_point (ধ্রুবক শূন্য *base) ধ্রুবক
  {
    ধ্রুবক অক্ষর *p = (ধ্রুবক অক্ষর *) base;
    যৌক্তিক ok = (uintptr_t) (p - এই->start) <= এই->দৈর্ঘ্য;

    DEBUG_বার্তা_LEVEL (SANITIZE, p, এই->debug_depth+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     এই->start, এই->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    ফেরত likely (ok);
  }

  template <typename T>
  যৌক্তিক check_range (ধ্রুবক T *base,
		    unsigned পূর্ণসংখ্যা a,
		    unsigned পূর্ণসংখ্যা b) ধ্রুবক
  {
    unsigned m;
    ফেরত !hb_unsigned_mul_overflows (a, b, &m) &&
	   এই->check_range (base, m);
  }

  template <typename T>
  যৌক্তিক check_range (ধ্রুবক T *base,
		    unsigned পূর্ণসংখ্যা a,
		    unsigned পূর্ণসংখ্যা b,
		    unsigned পূর্ণসংখ্যা c) ধ্রুবক
  {
    unsigned m;
    ফেরত !hb_unsigned_mul_overflows (a, b, &m) &&
	   এই->check_range (base, m, c);
  }

  template <typename T>
  HB_ALWAYS_INLINE
  যৌক্তিক check_array_sized (ধ্রুবক T *base, unsigned পূর্ণসংখ্যা len, unsigned len_আকার) ধ্রুবক
  {
    যদি (len_আকার >= 4)
    {
      যদি (unlikely (hb_unsigned_mul_overflows (len, hb_static_আকার (T), &len)))
	ফেরত মিথ্যা;
    }
    অন্যথা
      len = len * hb_static_আকার (T);
    ফেরত এই->check_range (base, len);
  }

  template <typename T>
  যৌক্তিক check_array (ধ্রুবক T *base, unsigned পূর্ণসংখ্যা len) ধ্রুবক
  {
    ফেরত এই->check_range (base, len, hb_static_আকার (T));
  }

  template <typename T>
  যৌক্তিক check_array (ধ্রুবক T *base,
		    unsigned পূর্ণসংখ্যা a,
		    unsigned পূর্ণসংখ্যা b) ধ্রুবক
  {
    ফেরত এই->check_range (base, hb_static_আকার (T), a, b);
  }

  যৌক্তিক check_start_recursion (পূর্ণসংখ্যা max_depth)
  {
    যদি (unlikely (recursion_depth >= max_depth)) ফেরত মিথ্যা;
    ফেরত ++recursion_depth;
  }

  যৌক্তিক end_recursion (যৌক্তিক ফলাফল)
  {
    recursion_depth--;
    ফেরত ফলাফল;
  }

  template <typename Type>
#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  যৌক্তিক check_struct (ধ্রুবক Type *obj) ধ্রুবক
  {
    যদি (আকার (uintptr_t) == আকার (uint32_t))
      ফেরত likely (এই->check_range_fast (obj, obj->min_আকার));
    অন্যথা
      ফেরত likely (এই->check_point ((ধ্রুবক অক্ষর *) obj + obj->min_আকার));
  }

  template <typename Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    যৌক্তিক sane;

    init (blob);

    DEBUG_বার্তা_FUNC (SANITIZE, start, "start");

    start_processing ();

    যদি (unlikely (!start))
    {
      end_processing ();
      ফেরত blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<অক্ষর *> (start));

    sane = t->sanitize (এই);

    end_processing ();

    DEBUG_বার্তা_FUNC (SANITIZE, start, sane ? "PASSED" : "FAILED");
    যদি (sane)
    {
      hb_blob_make_immutable (blob);
      ফেরত blob;
    }
    অন্যথা
    {
      hb_blob_destroy (blob);
      ফেরত hb_blob_get_empty ();
    }
  }

  template <typename Type>
  hb_blob_t *reference_table (ধ্রুবক hb_face_t *face, hb_tag_t tableTag = Type::tableTag)
  {
    যদি (!অঙ্ক_glyphs_set)
      set_অঙ্ক_glyphs (hb_face_get_glyph_গুনতি (face));
    ফেরত sanitize_blob<Type> (hb_face_reference_table (face, tableTag));
  }

  ধ্রুবক অক্ষর *start, *end;
  unsigned দৈর্ঘ্য;
  mutable পূর্ণসংখ্যা max_ops, max_subtables;
  ব্যক্তিগত:
  পূর্ণসংখ্যা recursion_depth;
  যৌক্তিক writable;
  hb_blob_t *blob;
  unsigned পূর্ণসংখ্যা অঙ্ক_glyphs;
  যৌক্তিক  অঙ্ক_glyphs_set;
  সর্বজনীন:
  যৌক্তিক lazy_some_gpos;
};

কাঠামো hb_sanitize_with_object_t
{
  template <typename T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, ধ্রুবক T& obj) : c (c)
  { c->set_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  ব্যক্তিগত:
  hb_sanitize_context_t *c;
};


#endif /* HB_SANITIZE_HH */
