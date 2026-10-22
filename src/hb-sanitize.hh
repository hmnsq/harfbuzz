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

#ضمن "hb.hh"
#ضمن "hb-blob.hh"
#ضمن "hb-dispatch.hh"


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

بنية hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, منطقي, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (ثابت حرف *start_ = nullptr, ثابت حرف *end_ = nullptr) :
	start (start_), end (end_),
	length (0),
	max_ops (0), max_subtables (0),
        recursion_depth (0),
	writable (خطأ),
	blob (nullptr),
	عدد_glyphs (65536),
	عدد_glyphs_set (خطأ),
	lazy_some_gpos (خطأ) {}

  ثابت حرف *get_name () { أرجع "SANITIZE"; }
  template <typename T, typename F>
  منطقي may_dispatch (ثابت T *obj HB_UNUSED, ثابت F *format)
  {
    أرجع format->sanitize (هذا) &&
	   hb_barrier ();
  }
  ساكن return_t default_return_قدر () { أرجع true; }
  ساكن return_t no_dispatch_return_قدر () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (ثابت return_t r) ثابت { أرجع !r; }

  منطقي visit_subtables (unsigned گنتی)
  {
    max_subtables += گنتی;
    أرجع max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  خاص:
  template <typename T, typename ...Ts> auto
  _dispatch (ثابت T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (هذا, std::forward<Ts> (ds)...) )
  template <typename T, typename ...Ts> auto
  _dispatch (ثابت T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (هذا, std::forward<Ts> (ds)...) )
  عام:
  template <typename T, typename ...Ts> auto
  dispatch (ثابت T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    إذا (blob)
      start_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    إذا (blob)
      end_processing ();
  }

  فراغ init (hb_blob_t *b)
  {
    هذا->blob = hb_blob_reference (b);
    هذا->writable = خطأ;
  }

  فراغ set_عدد_glyphs (unsigned صحيح عدد_glyphs_2)
  {
    عدد_glyphs = عدد_glyphs_2;
    عدد_glyphs_set = true;
  }
  unsigned صحيح get_عدد_glyphs () { أرجع عدد_glyphs; }

  فراغ set_max_ops (صحيح max_ops_) { max_ops = max_ops_; }

  template <typename T>
  فراغ set_object (ثابت T *obj)
  {
    reset_object ();

    إذا (!obj) أرجع;

    ثابت حرف *obj_start = (ثابت حرف *) obj;
    إذا (unlikely (obj_start < هذا->start || هذا->end <= obj_start))
    {
      هذا->start = هذا->end = nullptr;
      هذا->length = 0;
    }
    وإلا
    {
      هذا->start = obj_start;
      هذا->end   = obj_start + hb_min (size_t (هذا->end - obj_start), obj->get_size ());
      هذا->length = هذا->end - هذا->start;
    }
  }

  فراغ reset_object ()
  {
    إذا (هذا->blob)
    {
      هذا->start = هذا->blob->data;
      هذا->end = هذا->start + هذا->blob->length;
    }
    هذا->length = هذا->end - هذا->start;
    أكد (هذا->start <= هذا->end); /* Must not overflow. */
  }

  فراغ start_processing (ثابت حرف *start_ = nullptr, ثابت حرف *end_ = nullptr)
  {
    إذا (start_)
    {
      هذا->start = start_;
      هذا->end = end_;
    }
    reset_object ();
    unsigned m;
    إذا (unlikely (hb_unsigned_mul_overflows (هذا->end - هذا->start, HB_SANITIZE_MAX_OPS_FACTOR, &m)))
      هذا->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    وإلا
      هذا->max_ops = hb_clamp (m,
				(unsigned) HB_SANITIZE_MAX_OPS_MIN,
				(unsigned) HB_SANITIZE_MAX_OPS_MAX);
    هذا->debug_depth = 0;
    هذا->recursion_depth = 0;

    DEBUG_پیغام_LEVEL (SANITIZE, start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     هذا->start, هذا->end,
		     (unsigned long) (هذا->end - هذا->start));
  }

  فراغ end_processing ()
  {
    DEBUG_پیغام_LEVEL (SANITIZE, هذا->start, 0, -1,
		     "end [%p..%p]",
		     هذا->start, هذا->end);

    hb_blob_destroy (هذا->blob);
    هذا->blob = nullptr;
    هذا->start = هذا->end = nullptr;
    هذا->length = 0;
  }

  منطقي check_ops(unsigned گنتی)
  {
    /* Avoid underflow */
    إذا (unlikely (هذا->max_ops < 0 || گنتی >= (unsigned) هذا->max_ops))
    {
      هذا->max_ops = -1;
      أرجع خطأ;
    }
    هذا->max_ops -= (صحيح) گنتی;
    أرجع true;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  منطقي check_range (ثابت فراغ *base,
		    unsigned صحيح len) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) base;
    منطقي ok = (uintptr_t) (p - هذا->start) <= هذا->length &&
	      (unsigned صحيح) (هذا->end - p) >= len &&
	      ((هذا->max_ops -= len) > 0);

    DEBUG_پیغام_LEVEL (SANITIZE, p, هذا->debug_depth+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     هذا->start, هذا->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    أرجع likely (ok);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  منطقي check_range_fast (ثابت فراغ *base,
			 unsigned صحيح len) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) base;
    منطقي ok = ((uintptr_t) (p - هذا->start) <= هذا->length &&
	       (unsigned صحيح) (هذا->end - p) >= len);

    DEBUG_پیغام_LEVEL (SANITIZE, p, هذا->debug_depth+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     هذا->start, هذا->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    أرجع likely (ok);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  منطقي check_point (ثابت فراغ *base) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) base;
    منطقي ok = (uintptr_t) (p - هذا->start) <= هذا->length;

    DEBUG_پیغام_LEVEL (SANITIZE, p, هذا->debug_depth+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     هذا->start, هذا->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    أرجع likely (ok);
  }

  template <typename T>
  منطقي check_range (ثابت T *base,
		    unsigned صحيح a,
		    unsigned صحيح b) ثابت
  {
    unsigned m;
    أرجع !hb_unsigned_mul_overflows (a, b, &m) &&
	   هذا->check_range (base, m);
  }

  template <typename T>
  منطقي check_range (ثابت T *base,
		    unsigned صحيح a,
		    unsigned صحيح b,
		    unsigned صحيح c) ثابت
  {
    unsigned m;
    أرجع !hb_unsigned_mul_overflows (a, b, &m) &&
	   هذا->check_range (base, m, c);
  }

  template <typename T>
  HB_ALWAYS_INLINE
  منطقي check_array_sized (ثابت T *base, unsigned صحيح len, unsigned len_size) ثابت
  {
    إذا (len_size >= 4)
    {
      إذا (unlikely (hb_unsigned_mul_overflows (len, hb_static_size (T), &len)))
	أرجع خطأ;
    }
    وإلا
      len = len * hb_static_size (T);
    أرجع هذا->check_range (base, len);
  }

  template <typename T>
  منطقي check_array (ثابت T *base, unsigned صحيح len) ثابت
  {
    أرجع هذا->check_range (base, len, hb_static_size (T));
  }

  template <typename T>
  منطقي check_array (ثابت T *base,
		    unsigned صحيح a,
		    unsigned صحيح b) ثابت
  {
    أرجع هذا->check_range (base, hb_static_size (T), a, b);
  }

  منطقي check_start_recursion (صحيح max_depth)
  {
    إذا (unlikely (recursion_depth >= max_depth)) أرجع خطأ;
    أرجع ++recursion_depth;
  }

  منطقي end_recursion (منطقي result)
  {
    recursion_depth--;
    أرجع result;
  }

  template <typename Type>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  منطقي check_struct (ثابت Type *obj) ثابت
  {
    إذا (حجم (uintptr_t) == حجم (uint32_t))
      أرجع likely (هذا->check_range_fast (obj, obj->min_size));
    وإلا
      أرجع likely (هذا->check_point ((ثابت حرف *) obj + obj->min_size));
  }

  template <typename Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    منطقي sane;

    init (blob);

    DEBUG_پیغام_FUNC (SANITIZE, start, "start");

    start_processing ();

    إذا (unlikely (!start))
    {
      end_processing ();
      أرجع blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<حرف *> (start));

    sane = t->sanitize (هذا);

    end_processing ();

    DEBUG_پیغام_FUNC (SANITIZE, start, sane ? "PASSED" : "FAILED");
    إذا (sane)
    {
      hb_blob_make_immutable (blob);
      أرجع blob;
    }
    وإلا
    {
      hb_blob_destroy (blob);
      أرجع hb_blob_get_empty ();
    }
  }

  template <typename Type>
  hb_blob_t *reference_table (ثابت hb_face_t *face, hb_tag_t tableTag = Type::tableTag)
  {
    إذا (!عدد_glyphs_set)
      set_عدد_glyphs (hb_face_get_glyph_گنتی (face));
    أرجع sanitize_blob<Type> (hb_face_reference_table (face, tableTag));
  }

  ثابت حرف *start, *end;
  unsigned length;
  mutable صحيح max_ops, max_subtables;
  خاص:
  صحيح recursion_depth;
  منطقي writable;
  hb_blob_t *blob;
  unsigned صحيح عدد_glyphs;
  منطقي  عدد_glyphs_set;
  عام:
  منطقي lazy_some_gpos;
};

بنية hb_sanitize_with_object_t
{
  template <typename T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, ثابت T& obj) : c (c)
  { c->set_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  خاص:
  hb_sanitize_context_t *c;
};


#endif /* HB_SANITIZE_HH */
