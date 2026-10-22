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

#شامل "hb.hh"
#شامل "hb-blob.hh"
#شامل "hb-dispatch.hh"


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

ساختار hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, منطقی, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (ثابت نویسه *start_ = nullptr, ثابت نویسه *end_ = nullptr) :
	start (start_), end (end_),
	length (0),
	max_ops (0), max_subtables (0),
        recursion_depth (0),
	writable (نادرست),
	blob (nullptr),
	رقم_glyphs (65536),
	رقم_glyphs_set (نادرست),
	lazy_some_gpos (نادرست) {}

  ثابت نویسه *get_name () { بازگشت "SANITIZE"; }
  template <typename T, typename F>
  منطقی may_dispatch (ثابت T *obj HB_UNUSED, ثابت F *format)
  {
    بازگشت format->sanitize (این) &&
	   hb_barrier ();
  }
  ایستا return_t default_return_مقدار () { بازگشت درست; }
  ایستا return_t no_dispatch_return_مقدار () { بازگشت نادرست; }
  منطقی stop_sublookup_iteration (ثابت return_t r) ثابت { بازگشت !r; }

  منطقی visit_subtables (unsigned شمار)
  {
    max_subtables += شمار;
    بازگشت max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  خصوصی:
  template <typename T, typename ...Ts> auto
  _dispatch (ثابت T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (این, std::forward<Ts> (ds)...) )
  template <typename T, typename ...Ts> auto
  _dispatch (ثابت T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (این, std::forward<Ts> (ds)...) )
  عمومی:
  template <typename T, typename ...Ts> auto
  dispatch (ثابت T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    اگر (blob)
      start_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    اگر (blob)
      end_processing ();
  }

  تهی init (hb_blob_t *b)
  {
    این->blob = hb_blob_reference (b);
    این->writable = نادرست;
  }

  تهی set_رقم_glyphs (unsigned صحیح رقم_glyphs_2)
  {
    رقم_glyphs = رقم_glyphs_2;
    رقم_glyphs_set = درست;
  }
  unsigned صحیح get_رقم_glyphs () { بازگشت رقم_glyphs; }

  تهی set_max_ops (صحیح max_ops_) { max_ops = max_ops_; }

  template <typename T>
  تهی set_object (ثابت T *obj)
  {
    reset_object ();

    اگر (!obj) بازگشت;

    ثابت نویسه *obj_start = (ثابت نویسه *) obj;
    اگر (unlikely (obj_start < این->start || این->end <= obj_start))
    {
      این->start = این->end = nullptr;
      این->length = 0;
    }
    وگرنه
    {
      این->start = obj_start;
      این->end   = obj_start + hb_min (size_t (این->end - obj_start), obj->get_size ());
      این->length = این->end - این->start;
    }
  }

  تهی reset_object ()
  {
    اگر (این->blob)
    {
      این->start = این->blob->data;
      این->end = این->start + این->blob->length;
    }
    این->length = این->end - این->start;
    أكد (این->start <= این->end); /* Must not overflow. */
  }

  تهی start_processing (ثابت نویسه *start_ = nullptr, ثابت نویسه *end_ = nullptr)
  {
    اگر (start_)
    {
      این->start = start_;
      این->end = end_;
    }
    reset_object ();
    unsigned m;
    اگر (unlikely (hb_unsigned_mul_overflows (این->end - این->start, HB_SANITIZE_MAX_OPS_FACTOR, &m)))
      این->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    وگرنه
      این->max_ops = hb_clamp (m,
				(unsigned) HB_SANITIZE_MAX_OPS_MIN,
				(unsigned) HB_SANITIZE_MAX_OPS_MAX);
    این->debug_depth = 0;
    این->recursion_depth = 0;

    DEBUG_پیام_LEVEL (SANITIZE, start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     این->start, این->end,
		     (unsigned long) (این->end - این->start));
  }

  تهی end_processing ()
  {
    DEBUG_پیام_LEVEL (SANITIZE, این->start, 0, -1,
		     "end [%p..%p]",
		     این->start, این->end);

    hb_blob_destroy (این->blob);
    این->blob = nullptr;
    این->start = این->end = nullptr;
    این->length = 0;
  }

  منطقی check_ops(unsigned شمار)
  {
    /* Avoid underflow */
    اگر (unlikely (این->max_ops < 0 || شمار >= (unsigned) این->max_ops))
    {
      این->max_ops = -1;
      بازگشت نادرست;
    }
    این->max_ops -= (صحیح) شمار;
    بازگشت درست;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  منطقی check_range (ثابت تهی *base,
		    unsigned صحیح len) ثابت
  {
    ثابت نویسه *p = (ثابت نویسه *) base;
    منطقی ok = (uintptr_t) (p - این->start) <= این->length &&
	      (unsigned صحیح) (این->end - p) >= len &&
	      ((این->max_ops -= len) > 0);

    DEBUG_پیام_LEVEL (SANITIZE, p, این->debug_depth+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     این->start, این->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    بازگشت likely (ok);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  منطقی check_range_fast (ثابت تهی *base,
			 unsigned صحیح len) ثابت
  {
    ثابت نویسه *p = (ثابت نویسه *) base;
    منطقی ok = ((uintptr_t) (p - این->start) <= این->length &&
	       (unsigned صحیح) (این->end - p) >= len);

    DEBUG_پیام_LEVEL (SANITIZE, p, این->debug_depth+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     این->start, این->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    بازگشت likely (ok);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  منطقی check_point (ثابت تهی *base) ثابت
  {
    ثابت نویسه *p = (ثابت نویسه *) base;
    منطقی ok = (uintptr_t) (p - این->start) <= این->length;

    DEBUG_پیام_LEVEL (SANITIZE, p, این->debug_depth+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     این->start, این->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    بازگشت likely (ok);
  }

  template <typename T>
  منطقی check_range (ثابت T *base,
		    unsigned صحیح a,
		    unsigned صحیح b) ثابت
  {
    unsigned m;
    بازگشت !hb_unsigned_mul_overflows (a, b, &m) &&
	   این->check_range (base, m);
  }

  template <typename T>
  منطقی check_range (ثابت T *base,
		    unsigned صحیح a,
		    unsigned صحیح b,
		    unsigned صحیح c) ثابت
  {
    unsigned m;
    بازگشت !hb_unsigned_mul_overflows (a, b, &m) &&
	   این->check_range (base, m, c);
  }

  template <typename T>
  HB_ALWAYS_INLINE
  منطقی check_array_sized (ثابت T *base, unsigned صحیح len, unsigned len_size) ثابت
  {
    اگر (len_size >= 4)
    {
      اگر (unlikely (hb_unsigned_mul_overflows (len, hb_static_size (T), &len)))
	بازگشت نادرست;
    }
    وگرنه
      len = len * hb_static_size (T);
    بازگشت این->check_range (base, len);
  }

  template <typename T>
  منطقی check_array (ثابت T *base, unsigned صحیح len) ثابت
  {
    بازگشت این->check_range (base, len, hb_static_size (T));
  }

  template <typename T>
  منطقی check_array (ثابت T *base,
		    unsigned صحیح a,
		    unsigned صحیح b) ثابت
  {
    بازگشت این->check_range (base, hb_static_size (T), a, b);
  }

  منطقی check_start_recursion (صحیح max_depth)
  {
    اگر (unlikely (recursion_depth >= max_depth)) بازگشت نادرست;
    بازگشت ++recursion_depth;
  }

  منطقی end_recursion (منطقی result)
  {
    recursion_depth--;
    بازگشت result;
  }

  template <typename Type>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  منطقی check_struct (ثابت Type *obj) ثابت
  {
    اگر (اندازه (uintptr_t) == اندازه (uint32_t))
      بازگشت likely (این->check_range_fast (obj, obj->min_size));
    وگرنه
      بازگشت likely (این->check_point ((ثابت نویسه *) obj + obj->min_size));
  }

  template <typename Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    منطقی sane;

    init (blob);

    DEBUG_پیام_FUNC (SANITIZE, start, "start");

    start_processing ();

    اگر (unlikely (!start))
    {
      end_processing ();
      بازگشت blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<نویسه *> (start));

    sane = t->sanitize (این);

    end_processing ();

    DEBUG_پیام_FUNC (SANITIZE, start, sane ? "PASSED" : "FAILED");
    اگر (sane)
    {
      hb_blob_make_immutable (blob);
      بازگشت blob;
    }
    وگرنه
    {
      hb_blob_destroy (blob);
      بازگشت hb_blob_get_empty ();
    }
  }

  template <typename Type>
  hb_blob_t *reference_table (ثابت hb_face_t *face, hb_tag_t tableTag = Type::tableTag)
  {
    اگر (!رقم_glyphs_set)
      set_رقم_glyphs (hb_face_get_glyph_شمار (face));
    بازگشت sanitize_blob<Type> (hb_face_reference_table (face, tableTag));
  }

  ثابت نویسه *start, *end;
  unsigned length;
  mutable صحیح max_ops, max_subtables;
  خصوصی:
  صحیح recursion_depth;
  منطقی writable;
  hb_blob_t *blob;
  unsigned صحیح رقم_glyphs;
  منطقی  رقم_glyphs_set;
  عمومی:
  منطقی lazy_some_gpos;
};

ساختار hb_sanitize_with_object_t
{
  template <typename T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, ثابت T& obj) : c (c)
  { c->set_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  خصوصی:
  hb_sanitize_context_t *c;
};


#endif /* HB_SANITIZE_HH */
