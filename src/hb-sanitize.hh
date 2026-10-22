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

#অন্তর্ভাৱ "hb.hh"
#অন্তর্ভাৱ "hb-blob.hh"
#অন্তর্ভাৱ "hb-dispatch.hh"


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

সংরচনা hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, তার্কিক, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (ধ্রুৱ ৱর্ণ *start_ = nullptr, ধ্রুৱ ৱর্ণ *end_ = nullptr) :
	start (start_), end (end_),
	length (0),
	max_ops (0), max_subtables (0),
        recursion_depth (0),
	writable (মিছা),
	blob (nullptr),
	অঙ্ক_glyphs (65536),
	অঙ্ক_glyphs_set (মিছা),
	lazy_some_gpos (মিছা) {}

  ধ্রুৱ ৱর্ণ *get_name () { ঘূৰাই "SANITIZE"; }
  template <typename T, typename F>
  তার্কিক may_dispatch (ধ্রুৱ T *obj HB_UNUSED, ধ্রুৱ F *format)
  {
    ঘূৰাই format->sanitize (এতত্) &&
	   hb_barrier ();
  }
  স্থির return_t default_return_মান () { ঘূৰাই সঁচা; }
  স্থির return_t no_dispatch_return_মান () { ঘূৰাই মিছা; }
  তার্কিক stop_sublookup_iteration (ধ্রুৱ return_t r) ধ্রুৱ { ঘূৰাই !r; }

  তার্কিক visit_subtables (unsigned গুনতি)
  {
    max_subtables += গুনতি;
    ঘূৰাই max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  গুপ্ত:
  template <typename T, typename ...Ts> auto
  _dispatch (ধ্রুৱ T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (এতত্, std::forward<Ts> (ds)...) )
  template <typename T, typename ...Ts> auto
  _dispatch (ধ্রুৱ T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (এতত্, std::forward<Ts> (ds)...) )
  সার্ৱজনিক:
  template <typename T, typename ...Ts> auto
  dispatch (ধ্রুৱ T &obj, Ts&&... ds) HB_AUTO_RETURN
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
    এতত্->blob = hb_blob_reference (b);
    এতত্->writable = মিছা;
  }

  শূন্য set_অঙ্ক_glyphs (unsigned পূর্ণাঙ্ক অঙ্ক_glyphs_2)
  {
    অঙ্ক_glyphs = অঙ্ক_glyphs_2;
    অঙ্ক_glyphs_set = সঁচা;
  }
  unsigned পূর্ণাঙ্ক get_অঙ্ক_glyphs () { ঘূৰাই অঙ্ক_glyphs; }

  শূন্য set_max_ops (পূর্ণাঙ্ক max_ops_) { max_ops = max_ops_; }

  template <typename T>
  শূন্য set_object (ধ্রুৱ T *obj)
  {
    reset_object ();

    যদি (!obj) ঘূৰাই;

    ধ্রুৱ ৱর্ণ *obj_start = (ধ্রুৱ ৱর্ণ *) obj;
    যদি (unlikely (obj_start < এতত্->start || এতত্->end <= obj_start))
    {
      এতত্->start = এতত্->end = nullptr;
      এতত্->length = 0;
    }
    অন্যথা
    {
      এতত্->start = obj_start;
      এতত্->end   = obj_start + hb_min (size_t (এতত্->end - obj_start), obj->get_size ());
      এতত্->length = এতত্->end - এতত্->start;
    }
  }

  শূন্য reset_object ()
  {
    যদি (এতত্->blob)
    {
      এতত্->start = এতত্->blob->data;
      এতত্->end = এতত্->start + এতত্->blob->length;
    }
    এতত্->length = এতত্->end - এতত্->start;
    প্রতিজ্ঞা (এতত্->start <= এতত্->end); /* Must not overflow. */
  }

  শূন্য start_processing (ধ্রুৱ ৱর্ণ *start_ = nullptr, ধ্রুৱ ৱর্ণ *end_ = nullptr)
  {
    যদি (start_)
    {
      এতত্->start = start_;
      এতত্->end = end_;
    }
    reset_object ();
    unsigned m;
    যদি (unlikely (hb_unsigned_mul_overflows (এতত্->end - এতত্->start, HB_SANITIZE_MAX_OPS_FACTOR, &m)))
      এতত্->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    অন্যথা
      এতত্->max_ops = hb_clamp (m,
				(unsigned) HB_SANITIZE_MAX_OPS_MIN,
				(unsigned) HB_SANITIZE_MAX_OPS_MAX);
    এতত্->debug_depth = 0;
    এতত্->recursion_depth = 0;

    DEBUG_বাৰ্তা_LEVEL (SANITIZE, start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     এতত্->start, এতত্->end,
		     (unsigned long) (এতত্->end - এতত্->start));
  }

  শূন্য end_processing ()
  {
    DEBUG_বাৰ্তা_LEVEL (SANITIZE, এতত্->start, 0, -1,
		     "end [%p..%p]",
		     এতত্->start, এতত্->end);

    hb_blob_destroy (এতত্->blob);
    এতত্->blob = nullptr;
    এতত্->start = এতত্->end = nullptr;
    এতত্->length = 0;
  }

  তার্কিক check_ops(unsigned গুনতি)
  {
    /* Avoid underflow */
    যদি (unlikely (এতত্->max_ops < 0 || গুনতি >= (unsigned) এতত্->max_ops))
    {
      এতত্->max_ops = -1;
      ঘূৰাই মিছা;
    }
    এতত্->max_ops -= (পূর্ণাঙ্ক) গুনতি;
    ঘূৰাই সঁচা;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  তার্কিক check_range (ধ্রুৱ শূন্য *base,
		    unsigned পূর্ণাঙ্ক len) ধ্রুৱ
  {
    ধ্রুৱ ৱর্ণ *p = (ধ্রুৱ ৱর্ণ *) base;
    তার্কিক ok = (uintptr_t) (p - এতত্->start) <= এতত্->length &&
	      (unsigned পূর্ণাঙ্ক) (এতত্->end - p) >= len &&
	      ((এতত্->max_ops -= len) > 0);

    DEBUG_বাৰ্তা_LEVEL (SANITIZE, p, এতত্->debug_depth+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     এতত্->start, এতত্->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    ঘূৰাই likely (ok);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  তার্কিক check_range_fast (ধ্রুৱ শূন্য *base,
			 unsigned পূর্ণাঙ্ক len) ধ্রুৱ
  {
    ধ্রুৱ ৱর্ণ *p = (ধ্রুৱ ৱর্ণ *) base;
    তার্কিক ok = ((uintptr_t) (p - এতত্->start) <= এতত্->length &&
	       (unsigned পূর্ণাঙ্ক) (এতত্->end - p) >= len);

    DEBUG_বাৰ্তা_LEVEL (SANITIZE, p, এতত্->debug_depth+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     এতত্->start, এতত্->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    ঘূৰাই likely (ok);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  তার্কিক check_point (ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    ধ্রুৱ ৱর্ণ *p = (ধ্রুৱ ৱর্ণ *) base;
    তার্কিক ok = (uintptr_t) (p - এতত্->start) <= এতত্->length;

    DEBUG_বাৰ্তা_LEVEL (SANITIZE, p, এতত্->debug_depth+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     এতত্->start, এতত্->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    ঘূৰাই likely (ok);
  }

  template <typename T>
  তার্কিক check_range (ধ্রুৱ T *base,
		    unsigned পূর্ণাঙ্ক a,
		    unsigned পূর্ণাঙ্ক b) ধ্রুৱ
  {
    unsigned m;
    ঘূৰাই !hb_unsigned_mul_overflows (a, b, &m) &&
	   এতত্->check_range (base, m);
  }

  template <typename T>
  তার্কিক check_range (ধ্রুৱ T *base,
		    unsigned পূর্ণাঙ্ক a,
		    unsigned পূর্ণাঙ্ক b,
		    unsigned পূর্ণাঙ্ক c) ধ্রুৱ
  {
    unsigned m;
    ঘূৰাই !hb_unsigned_mul_overflows (a, b, &m) &&
	   এতত্->check_range (base, m, c);
  }

  template <typename T>
  HB_ALWAYS_INLINE
  তার্কিক check_array_sized (ধ্রুৱ T *base, unsigned পূর্ণাঙ্ক len, unsigned len_size) ধ্রুৱ
  {
    যদি (len_size >= 4)
    {
      যদি (unlikely (hb_unsigned_mul_overflows (len, hb_static_size (T), &len)))
	ঘূৰাই মিছা;
    }
    অন্যথা
      len = len * hb_static_size (T);
    ঘূৰাই এতত্->check_range (base, len);
  }

  template <typename T>
  তার্কিক check_array (ধ্রুৱ T *base, unsigned পূর্ণাঙ্ক len) ধ্রুৱ
  {
    ঘূৰাই এতত্->check_range (base, len, hb_static_size (T));
  }

  template <typename T>
  তার্কিক check_array (ধ্রুৱ T *base,
		    unsigned পূর্ণাঙ্ক a,
		    unsigned পূর্ণাঙ্ক b) ধ্রুৱ
  {
    ঘূৰাই এতত্->check_range (base, hb_static_size (T), a, b);
  }

  তার্কিক check_start_recursion (পূর্ণাঙ্ক max_depth)
  {
    যদি (unlikely (recursion_depth >= max_depth)) ঘূৰাই মিছা;
    ঘূৰাই ++recursion_depth;
  }

  তার্কিক end_recursion (তার্কিক result)
  {
    recursion_depth--;
    ঘূৰাই result;
  }

  template <typename Type>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  তার্কিক check_struct (ধ্রুৱ Type *obj) ধ্রুৱ
  {
    যদি (পরিমাণ (uintptr_t) == পরিমাণ (uint32_t))
      ঘূৰাই likely (এতত্->check_range_fast (obj, obj->min_size));
    অন্যথা
      ঘূৰাই likely (এতত্->check_point ((ধ্রুৱ ৱর্ণ *) obj + obj->min_size));
  }

  template <typename Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    তার্কিক sane;

    init (blob);

    DEBUG_বাৰ্তা_FUNC (SANITIZE, start, "start");

    start_processing ();

    যদি (unlikely (!start))
    {
      end_processing ();
      ঘূৰাই blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<ৱর্ণ *> (start));

    sane = t->sanitize (এতত্);

    end_processing ();

    DEBUG_বাৰ্তা_FUNC (SANITIZE, start, sane ? "PASSED" : "FAILED");
    যদি (sane)
    {
      hb_blob_make_immutable (blob);
      ঘূৰাই blob;
    }
    অন্যথা
    {
      hb_blob_destroy (blob);
      ঘূৰাই hb_blob_get_empty ();
    }
  }

  template <typename Type>
  hb_blob_t *reference_table (ধ্রুৱ hb_face_t *face, hb_tag_t tableTag = Type::tableTag)
  {
    যদি (!অঙ্ক_glyphs_set)
      set_অঙ্ক_glyphs (hb_face_get_glyph_গুনতি (face));
    ঘূৰাই sanitize_blob<Type> (hb_face_reference_table (face, tableTag));
  }

  ধ্রুৱ ৱর্ণ *start, *end;
  unsigned length;
  mutable পূর্ণাঙ্ক max_ops, max_subtables;
  গুপ্ত:
  পূর্ণাঙ্ক recursion_depth;
  তার্কিক writable;
  hb_blob_t *blob;
  unsigned পূর্ণাঙ্ক অঙ্ক_glyphs;
  তার্কিক  অঙ্ক_glyphs_set;
  সার্ৱজনিক:
  তার্কিক lazy_some_gpos;
};

সংরচনা hb_sanitize_with_object_t
{
  template <typename T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, ধ্রুৱ T& obj) : c (c)
  { c->set_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  গুপ্ত:
  hb_sanitize_context_t *c;
};


#endif /* HB_SANITIZE_HH */
