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

#অন্তর্ভাৱ "hb.hh"


/*
 * hb_blob_t
 */

সংরচনা hb_blob_t
{
  ~hb_blob_t () { destroy_user_data (); }

  শূন্য destroy_user_data ()
  {
    যদি (destroy)
    {
      destroy (user_data);
      user_data = nullptr;
      destroy = nullptr;
    }
  }

  শূন্য replace_buffer (ধ্রুৱ ৱর্ণ       *new_data,
		       unsigned          new_length,
		       hb_memory_mode_t  new_mode,
		       শূন্য             *new_user_data,
		       hb_destroy_func_t new_destroy)
  {
    যদি (new_data != data)
      destroy_user_data ();
    data = new_data;
    length = new_length;
    mode = new_mode;
    user_data = new_user_data;
    destroy = new_destroy;
  }

  HB_INTERNAL তার্কিক try_make_writable ();
  HB_INTERNAL তার্কিক try_make_writable_inplace ();
  HB_INTERNAL তার্কিক try_make_writable_inplace_unix ();

  hb_bytes_t as_bytes () ধ্রুৱ { ঘূৰাই hb_bytes_t (data, length); }
  template <typename Type>
  ধ্রুৱ Type* as () ধ্রুৱ { ঘূৰাই as_bytes ().as<Type> (); }

  সার্ৱজনিক:
  hb_object_header_t header;

  ধ্রুৱ ৱর্ণ *data = nullptr;
  unsigned পূর্ণাঙ্ক length = 0;
  hb_memory_mode_t mode = (hb_memory_mode_t) 0;

  শূন্য *user_data = nullptr;
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

  সংরচনা recycle_data_t
  {
    ৱর্ণ *buf;
    unsigned capacity;
  };

  স্থির অন্তঃস্থ শূন্য recycle_data_destroy (শূন্য *user_data)
  {
    auto *bd = (recycle_data_t *) user_data;
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
  স্থির অন্তঃস্থ ৱর্ণ *
  recycle_acquire (hb_blob_t *recycled,
		   unsigned   needed,
		   unsigned  *out_capacity,
		   ৱর্ণ     **out_replaced_buf)
  {
    *out_replaced_buf = nullptr;

    যদি (recycled && recycled->destroy == recycle_data_destroy)
    {
      auto *bd = (recycle_data_t *) recycled->user_data;
      যদি (bd->capacity >= needed)
      {
	*out_capacity = bd->capacity;
	ঘূৰাই bd->buf;
      }
      /* Grow with a 1.5x ramp to amortize repeated growth. */
      unsigned alloc_bytes = needed;
      যদি (unlikely (hb_unsigned_add_overflows (needed, needed / 2,
					       &alloc_bytes)))
	alloc_bytes = needed;
      ৱর্ণ *new_buf = (ৱর্ণ *) hb_realloc (bd->buf, alloc_bytes);
      যদি (new_buf)
      {
	bd->buf = new_buf;
	bd->capacity = alloc_bytes;
	*out_capacity = alloc_bytes;
	ঘূৰাই new_buf;
      }
      /* Realloc failed.  Fall through to a fresh hb_malloc and stash
       * the old buf for the caller to free after recycle_finalize. */
      *out_replaced_buf = bd->buf;
    }

    ৱর্ণ *buf = (ৱর্ণ *) hb_malloc (needed);
    যদি (unlikely (!buf))
      ঘূৰাই nullptr;
    *out_capacity = needed;
    ঘূৰাই buf;
  }

  /* Wrap @buf (of @capacity, with @length used) into an hb_blob_t.
   * If @recycled is one of our blobs, update and return it (cheap);
   * otherwise create a new blob.  Pass @replaced_recycled_buf from
   * recycle_acquire(). */
  স্থির অন্তঃস্থ hb_blob_t *
  recycle_finalize (ৱর্ণ      *buf,
		    unsigned   capacity,
		    unsigned   length,
		    hb_blob_t *recycled,
		    ৱর্ণ      *replaced_recycled_buf)
  {
    যদি (recycled && recycled->destroy == recycle_data_destroy)
    {
      auto *bd = (recycle_data_t *) recycled->user_data;
      যদি (replaced_recycled_buf && replaced_recycled_buf != buf)
	hb_free (replaced_recycled_buf);
      bd->buf = buf;
      bd->capacity = capacity;
      recycled->data = (ধ্রুৱ ৱর্ণ *) buf;
      recycled->length = length;
      ঘূৰাই recycled;
    }

    /* No recycled blob to update -- create a fresh one with our
     * destroy closure so the next recycle round can reuse it. */
    recycle_data_t *bd = (recycle_data_t *) hb_malloc (পরিমাণ (*bd));
    যদি (unlikely (!bd))
    {
      hb_free (buf);
      ঘূৰাই nullptr;
    }
    bd->buf = buf;
    bd->capacity = capacity;

    ঘূৰাই hb_blob_create ((ধ্রুৱ ৱর্ণ *) buf, length,
			   HB_MEMORY_MODE_WRITABLE,
			   bd, recycle_data_destroy);
  }

  /* Discard @buf returned by recycle_acquire without committing to
   * a blob.  Frees @buf if it was a fresh allocation; leaves any
   * recycled buffer untouched. */
  স্থির অন্তঃস্থ শূন্য
  recycle_abort (ৱর্ণ *buf, hb_blob_t *recycled)
  {
    যদি (!buf) ঘূৰাই;
    যদি (recycled && recycled->destroy == recycle_data_destroy)
    {
      auto *bd = (recycle_data_t *) recycled->user_data;
      যদি (buf == bd->buf) ঘূৰাই;  /* owned by the recycled blob */
    }
    hb_free (buf);
  }

  /* Stash @blob in @slot as the recycled output for the next
   * encode.  Destroys any previously stashed blob.  Safe to call
   * with @blob = nullptr or the empty-singleton blob (treated as
   * "drop"). */
  স্থির অন্তঃস্থ শূন্য
  recycle_stash (hb_blob_t **slot, hb_blob_t *blob)
  {
    hb_blob_destroy (*slot);
    *slot = nullptr;
    যদি (!blob || blob == hb_blob_get_empty ())
      ঘূৰাই;
    *slot = blob;
  }
};


/*
 * hb_blob_ptr_t
 */

template <typename P>
সংরচনা hb_blob_ptr_t
{
  প্রকারনির্দেশ hb_remove_pointer<P> T;

  hb_blob_ptr_t (hb_blob_t *b_ = nullptr) : b (b_) {}
  hb_blob_t * operator = (hb_blob_t *b_) { ঘূৰাই b = b_; }
  ধ্রুৱ T * operator -> () ধ্রুৱ { ঘূৰাই get (); }
  ধ্রুৱ T & operator * () ধ্রুৱ  { ঘূৰাই *get (); }
  template <typename C> operator ধ্রুৱ C * () ধ্রুৱ { ঘূৰাই get (); }
  operator ধ্রুৱ ৱর্ণ * () ধ্রুৱ { ঘূৰাই (ধ্রুৱ ৱর্ণ *) get (); }
  ধ্রুৱ T * get () ধ্রুৱ { ঘূৰাই b->as<T> (); }
  hb_blob_t * get_blob () ধ্রুৱ { ঘূৰাই b.get_raw (); }
  unsigned পূর্ণাঙ্ক get_length () ধ্রুৱ { ঘূৰাই b.get ()->length; }
  শূন্য destroy () { hb_blob_destroy (b.get_raw ()); b = nullptr; }

  গুপ্ত:
  hb_nonnull_ptr_t<hb_blob_t> b;
};


#endif /* HB_BLOB_HH */
