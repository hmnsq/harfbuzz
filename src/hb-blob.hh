/*
 * Copyright © 2009  Red Hat, Inc.
 * Copyright © 2018  Google, Inc.
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

#ifndef HB_BLOB_HH
#define HB_BLOB_HH

#অন্তর্ভাব "hb.hh"


/*
 * hb_blob_t
 */

কাঠামো hb_blob_t
{
  ~hb_blob_t () { destroy_user_তথ্য (); }

  শূন্য destroy_user_তথ্য ()
  {
    যদি (destroy)
    {
      destroy (user_তথ্য);
      user_তথ্য = nullptr;
      destroy = nullptr;
    }
  }

  শূন্য replace_বাফার (ধ্রুবক অক্ষর       *new_তথ্য,
		       unsigned          new_দৈর্ঘ্য,
		       hb_memory_mode_t  new_mode,
		       শূন্য             *new_user_তথ্য,
		       hb_destroy_func_t new_destroy)
  {
    যদি (new_তথ্য != তথ্য)
      destroy_user_তথ্য ();
    তথ্য = new_তথ্য;
    দৈর্ঘ্য = new_দৈর্ঘ্য;
    mode = new_mode;
    user_তথ্য = new_user_তথ্য;
    destroy = new_destroy;
  }

  HB_INTERNAL যৌক্তিক try_make_writable ();
  HB_INTERNAL যৌক্তিক try_make_writable_inplace ();
  HB_INTERNAL যৌক্তিক try_make_writable_inplace_unix ();

  hb_bytes_t as_bytes () ধ্রুবক { ফেরত hb_bytes_t (তথ্য, দৈর্ঘ্য); }
  template <typename Type>
  ধ্রুবক Type* as () ধ্রুবক { ফেরত as_bytes ().as<Type> (); }

  সর্বজনীন:
  hb_object_header_t header;

  ধ্রুবক অক্ষর *তথ্য = nullptr;
  unsigned পূর্ণসংখ্যা দৈর্ঘ্য = 0;
  hb_memory_mode_t mode = (hb_memory_mode_t) 0;

  শূন্য *user_তথ্য = nullptr;
  hb_destroy_func_t destroy = nullptr;

  /*
   * Blob-recycling helpers.
   *
   * Encoders that produce a sized byte buffer and wrap it as an
   * hb_blob_t can amortize malloc/blob-allocation across repeated
   * renders by handing the output blob back via a recycle slot.  On
   * the next encode these helpers reuse (or realloc) the buffer and
   * the same hb_blob_t handle is returned, skipping malloc/free
   * and blob-handle churn across glyph-by-glyph encoding loops.
   *
   * Blobs managed by this machinery are identified by the address
   * of recycle_data_destroy.
   */

  কাঠামো recycle_তথ্য_t
  {
    অক্ষর *buf;
    unsigned capacity;
  };

  স্থির অন্তঃরেখ শূন্য recycle_তথ্য_destroy (শূন্য *user_তথ্য)
  {
    auto *bd = (recycle_তথ্য_t *) user_তথ্য;
    hb_free (bd->buf);
    hb_free (bd);
  }

  /* Acquire a buffer of at least @needed bytes.  If @recycled is
   * one of our blobs, reuse its buffer (or realloc it).
   * *@out_capacity receives the actual capacity (>= @needed).
   * *@out_replaced_buf is set to the recycled buf when realloc
   * fails and a fresh buffer was allocated instead -- the caller
   * must hb_free() that buf after recycle_finalize() runs.  Returns
   * nullptr on allocation failure. */
  স্থির অন্তঃরেখ অক্ষর *
  recycle_acquire (hb_blob_t *recycled,
		   unsigned   needed,
		   unsigned  *out_capacity,
		   অক্ষর     **out_replaced_buf)
  {
    *out_replaced_buf = nullptr;

    যদি (recycled && recycled->destroy == recycle_তথ্য_destroy)
    {
      auto *bd = (recycle_তথ্য_t *) recycled->user_তথ্য;
      যদি (bd->capacity >= needed)
      {
	*out_capacity = bd->capacity;
	ফেরত bd->buf;
      }
      /* Grow with a 1.5x ramp to amortize repeated growth. */
      unsigned alloc_bytes = needed;
      যদি (unlikely (hb_unsigned_add_overflows (needed, needed / 2,
					       &alloc_bytes)))
	alloc_bytes = needed;
      অক্ষর *new_buf = (অক্ষর *) hb_realloc (bd->buf, alloc_bytes);
      যদি (new_buf)
      {
	bd->buf = new_buf;
	bd->capacity = alloc_bytes;
	*out_capacity = alloc_bytes;
	ফেরত new_buf;
      }
      /* Realloc failed.  Fall through to a fresh hb_malloc and stash
       * the old buf for the caller to free after recycle_finalize. */
      *out_replaced_buf = bd->buf;
    }

    অক্ষর *buf = (অক্ষর *) hb_malloc (needed);
    যদি (unlikely (!buf))
      ফেরত nullptr;
    *out_capacity = needed;
    ফেরত buf;
  }

  /* Wrap @buf (of @capacity, with @length used) into an hb_blob_t.
   * If @recycled is one of our blobs, update and return it (cheap);
   * otherwise create a new blob.  Pass @replaced_recycled_buf from
   * recycle_acquire(). */
  স্থির অন্তঃরেখ hb_blob_t *
  recycle_finalize (অক্ষর      *buf,
		    unsigned   capacity,
		    unsigned   দৈর্ঘ্য,
		    hb_blob_t *recycled,
		    অক্ষর      *replaced_recycled_buf)
  {
    যদি (recycled && recycled->destroy == recycle_তথ্য_destroy)
    {
      auto *bd = (recycle_তথ্য_t *) recycled->user_তথ্য;
      যদি (replaced_recycled_buf && replaced_recycled_buf != buf)
	hb_free (replaced_recycled_buf);
      bd->buf = buf;
      bd->capacity = capacity;
      recycled->তথ্য = (ধ্রুবক অক্ষর *) buf;
      recycled->দৈর্ঘ্য = দৈর্ঘ্য;
      ফেরত recycled;
    }

    /* No recycled blob to update -- create a fresh one with our
     * destroy closure so the next recycle round can reuse it. */
    recycle_তথ্য_t *bd = (recycle_তথ্য_t *) hb_malloc (আকার (*bd));
    যদি (unlikely (!bd))
    {
      hb_free (buf);
      ফেরত nullptr;
    }
    bd->buf = buf;
    bd->capacity = capacity;

    ফেরত hb_blob_create ((ধ্রুবক অক্ষর *) buf, দৈর্ঘ্য,
			   HB_MEMORY_MODE_WRITABLE,
			   bd, recycle_তথ্য_destroy);
  }

  /* Discard @buf returned by recycle_acquire without committing to
   * a blob.  Frees @buf if it was a fresh allocation; leaves any
   * recycled buffer untouched. */
  স্থির অন্তঃরেখ শূন্য
  recycle_abort (অক্ষর *buf, hb_blob_t *recycled)
  {
    যদি (!buf) ফেরত;
    যদি (recycled && recycled->destroy == recycle_তথ্য_destroy)
    {
      auto *bd = (recycle_তথ্য_t *) recycled->user_তথ্য;
      যদি (buf == bd->buf) ফেরত;  /* owned by the recycled blob */
    }
    hb_free (buf);
  }

  /* Stash @blob in @slot as the recycled output for the next
   * encode.  Destroys any previously stashed blob.  Safe to call
   * with @blob = nullptr or the empty-singleton blob (treated as
   * "drop"). */
  স্থির অন্তঃরেখ শূন্য
  recycle_stash (hb_blob_t **slot, hb_blob_t *blob)
  {
    hb_blob_destroy (*slot);
    *slot = nullptr;
    যদি (!blob || blob == hb_blob_get_empty ())
      ফেরত;
    *slot = blob;
  }
};


/*
 * hb_blob_ptr_t
 */

template <typename P>
কাঠামো hb_blob_ptr_t
{
  প্রকারলেখ hb_remove_pointer<P> T;

  hb_blob_ptr_t (hb_blob_t *b_ = nullptr) : b (b_) {}
  hb_blob_t * operator = (hb_blob_t *b_) { ফেরত b = b_; }
  ধ্রুবক T * operator -> () ধ্রুবক { ফেরত get (); }
  ধ্রুবক T & operator * () ধ্রুবক  { ফেরত *get (); }
  template <typename C> operator ধ্রুবক C * () ধ্রুবক { ফেরত get (); }
  operator ধ্রুবক অক্ষর * () ধ্রুবক { ফেরত (ধ্রুবক অক্ষর *) get (); }
  ধ্রুবক T * get () ধ্রুবক { ফেরত b->as<T> (); }
  hb_blob_t * get_blob () ধ্রুবক { ফেরত b.get_raw (); }
  unsigned পূর্ণসংখ্যা get_দৈর্ঘ্য () ধ্রুবক { ফেরত b.get ()->দৈর্ঘ্য; }
  শূন্য destroy () { hb_blob_destroy (b.get_raw ()); b = nullptr; }

  ব্যক্তিগত:
  hb_nonnull_ptr_t<hb_blob_t> b;
};


#endif /* HB_BLOB_HH */
