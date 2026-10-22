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
  hb_sanitize_context_t (ध्रुव वर्ण *start_ = nullptr, ध्रुव वर्ण *end_ = nullptr) :
	start (start_), end (end_),
	length (0),
	max_ops (0), max_subtables (0),
        recursion_depth (0),
	writable (मिच्छा),
	blob (nullptr),
	अङ्कः_glyphs (65536),
	अङ्कः_glyphs_set (मिच्छा),
	lazy_some_gpos (मिच्छा) {}

  ध्रुव वर्ण *get_name () { पटिनिवत्त "SANITIZE"; }
  template <typename T, typename F>
  तार्किक may_dispatch (ध्रुव T *obj HB_UNUSED, ध्रुव F *format)
  {
    पटिनिवत्त format->sanitize (एतत्) &&
	   hb_barrier ();
  }
  स्थिर return_t default_return_मानम् () { पटिनिवत्त सच्च; }
  स्थिर return_t no_dispatch_return_मानम् () { पटिनिवत्त मिच्छा; }
  तार्किक stop_sublookup_iteration (ध्रुव return_t r) ध्रुव { पटिनिवत्त !r; }

  तार्किक visit_subtables (unsigned गणनम्)
  {
    max_subtables += गणनम्;
    पटिनिवत्त max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  गुप्त:
  template <typename T, typename ...Ts> auto
  _dispatch (ध्रुव T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (एतत्, std::forward<Ts> (ds)...) )
  template <typename T, typename ...Ts> auto
  _dispatch (ध्रुव T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (एतत्, std::forward<Ts> (ds)...) )
  सार्वजनिक:
  template <typename T, typename ...Ts> auto
  dispatch (ध्रुव T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    सचे (blob)
      start_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    सचे (blob)
      end_processing ();
  }

  शून्य init (hb_blob_t *b)
  {
    एतत्->blob = hb_blob_reference (b);
    एतत्->writable = मिच्छा;
  }

  शून्य set_अङ्कः_glyphs (unsigned पूर्णाङ्क अङ्कः_glyphs_2)
  {
    अङ्कः_glyphs = अङ्कः_glyphs_2;
    अङ्कः_glyphs_set = सच्च;
  }
  unsigned पूर्णाङ्क get_अङ्कः_glyphs () { पटिनिवत्त अङ्कः_glyphs; }

  शून्य set_max_ops (पूर्णाङ्क max_ops_) { max_ops = max_ops_; }

  template <typename T>
  शून्य set_object (ध्रुव T *obj)
  {
    reset_object ();

    सचे (!obj) पटिनिवत्त;

    ध्रुव वर्ण *obj_start = (ध्रुव वर्ण *) obj;
    सचे (unlikely (obj_start < एतत्->start || एतत्->end <= obj_start))
    {
      एतत्->start = एतत्->end = nullptr;
      एतत्->length = 0;
    }
    अञ्ञथा
    {
      एतत्->start = obj_start;
      एतत्->end   = obj_start + hb_min (size_t (एतत्->end - obj_start), obj->get_size ());
      एतत्->length = एतत्->end - एतत्->start;
    }
  }

  शून्य reset_object ()
  {
    सचे (एतत्->blob)
    {
      एतत्->start = एतत्->blob->data;
      एतत्->end = एतत्->start + एतत्->blob->length;
    }
    एतत्->length = एतत्->end - एतत्->start;
    प्रतिज्ञा (एतत्->start <= एतत्->end); /* Must not overflow. */
  }

  शून्य start_processing (ध्रुव वर्ण *start_ = nullptr, ध्रुव वर्ण *end_ = nullptr)
  {
    सचे (start_)
    {
      एतत्->start = start_;
      एतत्->end = end_;
    }
    reset_object ();
    unsigned m;
    सचे (unlikely (hb_unsigned_mul_overflows (एतत्->end - एतत्->start, HB_SANITIZE_MAX_OPS_FACTOR, &m)))
      एतत्->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    अञ्ञथा
      एतत्->max_ops = hb_clamp (m,
				(unsigned) HB_SANITIZE_MAX_OPS_MIN,
				(unsigned) HB_SANITIZE_MAX_OPS_MAX);
    एतत्->debug_depth = 0;
    एतत्->recursion_depth = 0;

    DEBUG_सन्देशः_LEVEL (SANITIZE, start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     एतत्->start, एतत्->end,
		     (unsigned long) (एतत्->end - एतत्->start));
  }

  शून्य end_processing ()
  {
    DEBUG_सन्देशः_LEVEL (SANITIZE, एतत्->start, 0, -1,
		     "end [%p..%p]",
		     एतत्->start, एतत्->end);

    hb_blob_destroy (एतत्->blob);
    एतत्->blob = nullptr;
    एतत्->start = एतत्->end = nullptr;
    एतत्->length = 0;
  }

  तार्किक check_ops(unsigned गणनम्)
  {
    /* Avoid underflow */
    सचे (unlikely (एतत्->max_ops < 0 || गणनम् >= (unsigned) एतत्->max_ops))
    {
      एतत्->max_ops = -1;
      पटिनिवत्त मिच्छा;
    }
    एतत्->max_ops -= (पूर्णाङ्क) गणनम्;
    पटिनिवत्त सच्च;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  तार्किक check_range (ध्रुव शून्य *base,
		    unsigned पूर्णाङ्क len) ध्रुव
  {
    ध्रुव वर्ण *p = (ध्रुव वर्ण *) base;
    तार्किक ok = (uintptr_t) (p - एतत्->start) <= एतत्->length &&
	      (unsigned पूर्णाङ्क) (एतत्->end - p) >= len &&
	      ((एतत्->max_ops -= len) > 0);

    DEBUG_सन्देशः_LEVEL (SANITIZE, p, एतत्->debug_depth+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     एतत्->start, एतत्->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    पटिनिवत्त likely (ok);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  तार्किक check_range_fast (ध्रुव शून्य *base,
			 unsigned पूर्णाङ्क len) ध्रुव
  {
    ध्रुव वर्ण *p = (ध्रुव वर्ण *) base;
    तार्किक ok = ((uintptr_t) (p - एतत्->start) <= एतत्->length &&
	       (unsigned पूर्णाङ्क) (एतत्->end - p) >= len);

    DEBUG_सन्देशः_LEVEL (SANITIZE, p, एतत्->debug_depth+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     एतत्->start, एतत्->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    पटिनिवत्त likely (ok);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  तार्किक check_point (ध्रुव शून्य *base) ध्रुव
  {
    ध्रुव वर्ण *p = (ध्रुव वर्ण *) base;
    तार्किक ok = (uintptr_t) (p - एतत्->start) <= एतत्->length;

    DEBUG_सन्देशः_LEVEL (SANITIZE, p, एतत्->debug_depth+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     एतत्->start, एतत्->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    पटिनिवत्त likely (ok);
  }

  template <typename T>
  तार्किक check_range (ध्रुव T *base,
		    unsigned पूर्णाङ्क a,
		    unsigned पूर्णाङ्क b) ध्रुव
  {
    unsigned m;
    पटिनिवत्त !hb_unsigned_mul_overflows (a, b, &m) &&
	   एतत्->check_range (base, m);
  }

  template <typename T>
  तार्किक check_range (ध्रुव T *base,
		    unsigned पूर्णाङ्क a,
		    unsigned पूर्णाङ्क b,
		    unsigned पूर्णाङ्क c) ध्रुव
  {
    unsigned m;
    पटिनिवत्त !hb_unsigned_mul_overflows (a, b, &m) &&
	   एतत्->check_range (base, m, c);
  }

  template <typename T>
  HB_ALWAYS_INLINE
  तार्किक check_array_sized (ध्रुव T *base, unsigned पूर्णाङ्क len, unsigned len_size) ध्रुव
  {
    सचे (len_size >= 4)
    {
      सचे (unlikely (hb_unsigned_mul_overflows (len, hb_static_size (T), &len)))
	पटिनिवत्त मिच्छा;
    }
    अञ्ञथा
      len = len * hb_static_size (T);
    पटिनिवत्त एतत्->check_range (base, len);
  }

  template <typename T>
  तार्किक check_array (ध्रुव T *base, unsigned पूर्णाङ्क len) ध्रुव
  {
    पटिनिवत्त एतत्->check_range (base, len, hb_static_size (T));
  }

  template <typename T>
  तार्किक check_array (ध्रुव T *base,
		    unsigned पूर्णाङ्क a,
		    unsigned पूर्णाङ्क b) ध्रुव
  {
    पटिनिवत्त एतत्->check_range (base, hb_static_size (T), a, b);
  }

  तार्किक check_start_recursion (पूर्णाङ्क max_depth)
  {
    सचे (unlikely (recursion_depth >= max_depth)) पटिनिवत्त मिच्छा;
    पटिनिवत्त ++recursion_depth;
  }

  तार्किक end_recursion (तार्किक result)
  {
    recursion_depth--;
    पटिनिवत्त result;
  }

  template <typename Type>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  तार्किक check_struct (ध्रुव Type *obj) ध्रुव
  {
    सचे (परिमाण (uintptr_t) == परिमाण (uint32_t))
      पटिनिवत्त likely (एतत्->check_range_fast (obj, obj->min_size));
    अञ्ञथा
      पटिनिवत्त likely (एतत्->check_point ((ध्रुव वर्ण *) obj + obj->min_size));
  }

  template <typename Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    तार्किक sane;

    init (blob);

    DEBUG_सन्देशः_FUNC (SANITIZE, start, "start");

    start_processing ();

    सचे (unlikely (!start))
    {
      end_processing ();
      पटिनिवत्त blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<वर्ण *> (start));

    sane = t->sanitize (एतत्);

    end_processing ();

    DEBUG_सन्देशः_FUNC (SANITIZE, start, sane ? "PASSED" : "FAILED");
    सचे (sane)
    {
      hb_blob_make_immutable (blob);
      पटिनिवत्त blob;
    }
    अञ्ञथा
    {
      hb_blob_destroy (blob);
      पटिनिवत्त hb_blob_get_empty ();
    }
  }

  template <typename Type>
  hb_blob_t *reference_table (ध्रुव hb_face_t *face, hb_tag_t tableTag = Type::tableTag)
  {
    सचे (!अङ्कः_glyphs_set)
      set_अङ्कः_glyphs (hb_face_get_glyph_गणनम् (face));
    पटिनिवत्त sanitize_blob<Type> (hb_face_reference_table (face, tableTag));
  }

  ध्रुव वर्ण *start, *end;
  unsigned length;
  mutable पूर्णाङ्क max_ops, max_subtables;
  गुप्त:
  पूर्णाङ्क recursion_depth;
  तार्किक writable;
  hb_blob_t *blob;
  unsigned पूर्णाङ्क अङ्कः_glyphs;
  तार्किक  अङ्कः_glyphs_set;
  सार्वजनिक:
  तार्किक lazy_some_gpos;
};

संरचना hb_sanitize_with_object_t
{
  template <typename T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, ध्रुव T& obj) : c (c)
  { c->set_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  गुप्त:
  hb_sanitize_context_t *c;
};


#endif /* HB_SANITIZE_HH */
