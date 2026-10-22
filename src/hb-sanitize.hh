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

#include "hb.hh"
#include "hb-blob.hh"
#include "hb-dispatch.hh"


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

sa_nracanaa hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, bool, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (const var_na *start_ = nullptr, const var_na *end_ = nullptr) :
	start (start_), end (end_),
	length (0),
	max_ops (0), max_subtables (0),
        recursion_depth (0),
	writable (false),
	blob (nullptr),
	num_glyphs (65536),
	num_glyphs_set (false),
	lazy_some_gpos (false) {}

  const var_na *get_name () { ᯀᯮᯞ "SANITIZE"; }
  template <typename T, typename F>
  bool may_dispatch (const T *obj HB_UNUSED, const F *format)
  {
    ᯀᯮᯞ format->sanitize (this) &&
	   hb_barrier ();
  }
  static return_t default_return_value () { ᯀᯮᯞ true; }
  static return_t no_dispatch_return_value () { ᯀᯮᯞ false; }
  bool stop_sublookup_iteration (const return_t r) const { ᯀᯮᯞ !r; }

  bool visit_subtables (unsigned count)
  {
    max_subtables += count;
    ᯀᯮᯞ max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  private:
  template <typename T, typename ...Ts> auto
  _dispatch (const T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (this, std::forward<Ts> (ds)...) )
  template <typename T, typename ...Ts> auto
  _dispatch (const T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (this, std::forward<Ts> (ds)...) )
  public:
  template <typename T, typename ...Ts> auto
  dispatch (const T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    ᯀᯮᯝ (blob)
      start_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    ᯀᯮᯝ (blob)
      end_processing ();
  }

  void init (hb_blob_t *b)
  {
    this->blob = hb_blob_reference (b);
    this->writable = false;
  }

  void set_num_glyphs (unsigned puur_naa_nka num_glyphs_)
  {
    num_glyphs = num_glyphs_;
    num_glyphs_set = true;
  }
  unsigned puur_naa_nka get_num_glyphs () { ᯀᯮᯞ num_glyphs; }

  void set_max_ops (puur_naa_nka max_ops_) { max_ops = max_ops_; }

  template <typename T>
  void set_object (const T *obj)
  {
    reset_object ();

    ᯀᯮᯝ (!obj) ᯀᯮᯞ;

    const var_na *obj_start = (const var_na *) obj;
    ᯀᯮᯝ (unlikely (obj_start < this->start || this->end <= obj_start))
    {
      this->start = this->end = nullptr;
      this->length = 0;
    }
    ᯀᯮᯝᯂᯪ
    {
      this->start = obj_start;
      this->end   = obj_start + hb_min (size_t (this->end - obj_start), obj->get_size ());
      this->length = this->end - this->start;
    }
  }

  void reset_object ()
  {
    ᯀᯮᯝ (this->blob)
    {
      this->start = this->blob->data;
      this->end = this->start + this->blob->length;
    }
    this->length = this->end - this->start;
    assert (this->start <= this->end); /* Must not overflow. */
  }

  void start_processing (const var_na *start_ = nullptr, const var_na *end_ = nullptr)
  {
    ᯀᯮᯝ (start_)
    {
      this->start = start_;
      this->end = end_;
    }
    reset_object ();
    unsigned m;
    ᯀᯮᯝ (unlikely (hb_unsigned_mul_overflows (this->end - this->start, HB_SANITIZE_MAX_OPS_FACTOR, &m)))
      this->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    ᯀᯮᯝᯂᯪ
      this->max_ops = hb_clamp (m,
				(unsigned) HB_SANITIZE_MAX_OPS_MIN,
				(unsigned) HB_SANITIZE_MAX_OPS_MAX);
    this->debug_depth = 0;
    this->recursion_depth = 0;

    DEBUG_MSG_LEVEL (SANITIZE, start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     this->start, this->end,
		     (unsigned long) (this->end - this->start));
  }

  void end_processing ()
  {
    DEBUG_MSG_LEVEL (SANITIZE, this->start, 0, -1,
		     "end [%p..%p]",
		     this->start, this->end);

    hb_blob_destroy (this->blob);
    this->blob = nullptr;
    this->start = this->end = nullptr;
    this->length = 0;
  }

  bool check_ops(unsigned count)
  {
    /* Avoid underflow */
    ᯀᯮᯝ (unlikely (this->max_ops < 0 || count >= (unsigned) this->max_ops))
    {
      this->max_ops = -1;
      ᯀᯮᯞ false;
    }
    this->max_ops -= (puur_naa_nka) count;
    ᯀᯮᯞ true;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  bool check_range (const void *base,
		    unsigned puur_naa_nka len) const
  {
    const var_na *p = (const var_na *) base;
    bool ok = (uintptr_t) (p - this->start) <= this->length &&
	      (unsigned puur_naa_nka) (this->end - p) >= len &&
	      ((this->max_ops -= len) > 0);

    DEBUG_MSG_LEVEL (SANITIZE, p, this->debug_depth+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     this->start, this->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    ᯀᯮᯞ likely (ok);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  bool check_range_fast (const void *base,
			 unsigned puur_naa_nka len) const
  {
    const var_na *p = (const var_na *) base;
    bool ok = ((uintptr_t) (p - this->start) <= this->length &&
	       (unsigned puur_naa_nka) (this->end - p) >= len);

    DEBUG_MSG_LEVEL (SANITIZE, p, this->debug_depth+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     this->start, this->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    ᯀᯮᯞ likely (ok);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  bool check_point (const void *base) const
  {
    const var_na *p = (const var_na *) base;
    bool ok = (uintptr_t) (p - this->start) <= this->length;

    DEBUG_MSG_LEVEL (SANITIZE, p, this->debug_depth+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     this->start, this->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    ᯀᯮᯞ likely (ok);
  }

  template <typename T>
  bool check_range (const T *base,
		    unsigned puur_naa_nka a,
		    unsigned puur_naa_nka b) const
  {
    unsigned m;
    ᯀᯮᯞ !hb_unsigned_mul_overflows (a, b, &m) &&
	   this->check_range (base, m);
  }

  template <typename T>
  bool check_range (const T *base,
		    unsigned puur_naa_nka a,
		    unsigned puur_naa_nka b,
		    unsigned puur_naa_nka c) const
  {
    unsigned m;
    ᯀᯮᯞ !hb_unsigned_mul_overflows (a, b, &m) &&
	   this->check_range (base, m, c);
  }

  template <typename T>
  HB_ALWAYS_INLINE
  bool check_array_sized (const T *base, unsigned puur_naa_nka len, unsigned len_size) const
  {
    ᯀᯮᯝ (len_size >= 4)
    {
      ᯀᯮᯝ (unlikely (hb_unsigned_mul_overflows (len, hb_static_size (T), &len)))
	ᯀᯮᯞ false;
    }
    ᯀᯮᯝᯂᯪ
      len = len * hb_static_size (T);
    ᯀᯮᯞ this->check_range (base, len);
  }

  template <typename T>
  bool check_array (const T *base, unsigned puur_naa_nka len) const
  {
    ᯀᯮᯞ this->check_range (base, len, hb_static_size (T));
  }

  template <typename T>
  bool check_array (const T *base,
		    unsigned puur_naa_nka a,
		    unsigned puur_naa_nka b) const
  {
    ᯀᯮᯞ this->check_range (base, hb_static_size (T), a, b);
  }

  bool check_start_recursion (puur_naa_nka max_depth)
  {
    ᯀᯮᯝ (unlikely (recursion_depth >= max_depth)) ᯀᯮᯞ false;
    ᯀᯮᯞ ++recursion_depth;
  }

  bool end_recursion (bool result)
  {
    recursion_depth--;
    ᯀᯮᯞ result;
  }

  template <typename Type>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  bool check_struct (const Type *obj) const
  {
    ᯀᯮᯝ (parimaa_na (uintptr_t) == parimaa_na (uint32_t))
      ᯀᯮᯞ likely (this->check_range_fast (obj, obj->min_size));
    ᯀᯮᯝᯂᯪ
      ᯀᯮᯞ likely (this->check_point ((const var_na *) obj + obj->min_size));
  }

  template <typename Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    bool sane;

    init (blob);

    DEBUG_MSG_FUNC (SANITIZE, start, "start");

    start_processing ();

    ᯀᯮᯝ (unlikely (!start))
    {
      end_processing ();
      ᯀᯮᯞ blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<var_na *> (start));

    sane = t->sanitize (this);

    end_processing ();

    DEBUG_MSG_FUNC (SANITIZE, start, sane ? "PASSED" : "FAILED");
    ᯀᯮᯝ (sane)
    {
      hb_blob_make_immutable (blob);
      ᯀᯮᯞ blob;
    }
    ᯀᯮᯝᯂᯪ
    {
      hb_blob_destroy (blob);
      ᯀᯮᯞ hb_blob_get_empty ();
    }
  }

  template <typename Type>
  hb_blob_t *reference_table (const hb_face_t *face, hb_tag_t tableTag = Type::tableTag)
  {
    ᯀᯮᯝ (!num_glyphs_set)
      set_num_glyphs (hb_face_get_glyph_count (face));
    ᯀᯮᯞ sanitize_blob<Type> (hb_face_reference_table (face, tableTag));
  }

  const var_na *start, *end;
  unsigned length;
  mutable puur_naa_nka max_ops, max_subtables;
  private:
  puur_naa_nka recursion_depth;
  bool writable;
  hb_blob_t *blob;
  unsigned puur_naa_nka num_glyphs;
  bool  num_glyphs_set;
  public:
  bool lazy_some_gpos;
};

sa_nracanaa hb_sanitize_with_object_t
{
  template <typename T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, const T& obj) : c (c)
  { c->set_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  private:
  hb_sanitize_context_t *c;
};


#endif /* HB_SANITIZE_HH */
