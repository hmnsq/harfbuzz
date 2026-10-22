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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_BLOB_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_BLOB_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"


/*
 * hb_blob_t
 */

ਬਣਤਰ hb_blob_t
{
  ~hb_blob_t () { destroy_user_ਅੰਕੜੇ (); }

  ਖਾਲੀ destroy_user_ਅੰਕੜੇ ()
  {
    ਜੇ (destroy)
    {
      destroy (user_ਅੰਕੜੇ);
      user_ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
      destroy = ਖਾਲੀ_ਸੂਚਕ;
    }
  }

  ਖਾਲੀ replace_ਬਫਰ (ਅਚਲ ਅੱਖਰ       *new_ਅੰਕੜੇ,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ          new_ਲੰਮਾਈ,
		       hb_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_t  new_ਬਹੁਲਕ,
		       ਖਾਲੀ             *new_user_ਅੰਕੜੇ,
		       hb_destroy_func_t new_destroy)
  {
    ਜੇ (new_ਅੰਕੜੇ != ਅੰਕੜੇ)
      destroy_user_ਅੰਕੜੇ ();
    ਅੰਕੜੇ = new_ਅੰਕੜੇ;
    ਲੰਮਾਈ = new_ਲੰਮਾਈ;
    ਬਹੁਲਕ = new_ਬਹੁਲਕ;
    user_ਅੰਕੜੇ = new_user_ਅੰਕੜੇ;
    destroy = new_destroy;
  }

  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ try_make_writable ();
  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ try_make_writable_inplace ();
  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ try_make_writable_inplace_unix ();

  hb_ਬਾਈਟਾਂ_t as_ਬਾਈਟਾਂ () ਅਚਲ { ਵਾਪਸ hb_ਬਾਈਟਾਂ_t (ਅੰਕੜੇ, ਲੰਮਾਈ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  ਅਚਲ Type* as () ਅਚਲ { ਵਾਪਸ as_ਬਾਈਟਾਂ ().as<Type> (); }

  ਜਨਤਕ:
  hb_object_ਸਿਰਲੇਖ_t ਸਿਰਲੇਖ;

  ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ = 0;
  hb_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_t ਬਹੁਲਕ = (hb_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_t) 0;

  ਖਾਲੀ *user_ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
  hb_destroy_func_t destroy = ਖਾਲੀ_ਸੂਚਕ;

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

  ਬਣਤਰ recycle_ਅੰਕੜੇ_t
  {
    ਅੱਖਰ *buf;
    ਬਿਨਾਨਿਸ਼ਾਨੀ capacity;
  };

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ recycle_ਅੰਕੜੇ_destroy (ਖਾਲੀ *user_ਅੰਕੜੇ)
  {
    ਸਵੈਚਾਲਿਤ *bd = (recycle_ਅੰਕੜੇ_t *) user_ਅੰਕੜੇ;
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
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਅੱਖਰ *
  recycle_acquire (hb_blob_t *recycled,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ   needed,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ  *ਬਾਹਰਲਾ_ਸਮਰੱਥਾ,
		   ਅੱਖਰ     **ਬਾਹਰਲਾ_replaced_buf)
  {
    *ਬਾਹਰਲਾ_replaced_buf = ਖਾਲੀ_ਸੂਚਕ;

    ਜੇ (recycled && recycled->destroy == recycle_ਅੰਕੜੇ_destroy)
    {
      ਸਵੈਚਾਲਿਤ *bd = (recycle_ਅੰਕੜੇ_t *) recycled->user_ਅੰਕੜੇ;
      ਜੇ (bd->capacity >= needed)
      {
	*ਬਾਹਰਲਾ_ਸਮਰੱਥਾ = bd->capacity;
	ਵਾਪਸ bd->buf;
      }
      /* Grow with a 1.5x ramp to amortize repeated growth. */
      ਬਿਨਾਨਿਸ਼ਾਨੀ alloc_ਬਾਈਟਾਂ = needed;
      ਜੇ (unlikely (hb_unsigned_ਜੋੜੋ_overflows (needed, needed / 2,
					       &alloc_ਬਾਈਟਾਂ)))
	alloc_ਬਾਈਟਾਂ = needed;
      ਅੱਖਰ *new_buf = (ਅੱਖਰ *) hb_realloc (bd->buf, alloc_ਬਾਈਟਾਂ);
      ਜੇ (new_buf)
      {
	bd->buf = new_buf;
	bd->capacity = alloc_ਬਾਈਟਾਂ;
	*ਬਾਹਰਲਾ_ਸਮਰੱਥਾ = alloc_ਬਾਈਟਾਂ;
	ਵਾਪਸ new_buf;
      }
      /* Realloc failed.  Fall through to a fresh hb_malloc and stash
       * the old buf for the caller to free after recycle_finalize. */
      *ਬਾਹਰਲਾ_replaced_buf = bd->buf;
    }

    ਅੱਖਰ *buf = (ਅੱਖਰ *) hb_malloc (needed);
    ਜੇ (unlikely (!buf))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    *ਬਾਹਰਲਾ_ਸਮਰੱਥਾ = needed;
    ਵਾਪਸ buf;
  }

  /* Wrap @buf (of @capacity, with @length used) into an hb_blob_t.
   * If @recycled is one of our blobs, update and return it (cheap);
   * otherwise create a new blob.  Pass @replaced_recycled_buf from
   * recycle_acquire(). */
  ਸਥਿਰ ਸਤਰਅੰਦਰ hb_blob_t *
  recycle_finalize (ਅੱਖਰ      *buf,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ   capacity,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ   ਲੰਮਾਈ,
		    hb_blob_t *recycled,
		    ਅੱਖਰ      *replaced_recycled_buf)
  {
    ਜੇ (recycled && recycled->destroy == recycle_ਅੰਕੜੇ_destroy)
    {
      ਸਵੈਚਾਲਿਤ *bd = (recycle_ਅੰਕੜੇ_t *) recycled->user_ਅੰਕੜੇ;
      ਜੇ (replaced_recycled_buf && replaced_recycled_buf != buf)
	hb_free (replaced_recycled_buf);
      bd->buf = buf;
      bd->capacity = capacity;
      recycled->ਅੰਕੜੇ = (ਅਚਲ ਅੱਖਰ *) buf;
      recycled->ਲੰਮਾਈ = ਲੰਮਾਈ;
      ਵਾਪਸ recycled;
    }

    /* No recycled blob to update -- create a fresh one with our
     * destroy closure so the next recycle round can reuse it. */
    recycle_ਅੰਕੜੇ_t *bd = (recycle_ਅੰਕੜੇ_t *) hb_malloc (ਆਕਾਰ (*bd));
    ਜੇ (unlikely (!bd))
    {
      hb_free (buf);
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
    bd->buf = buf;
    bd->capacity = capacity;

    ਵਾਪਸ hb_blob_create ((ਅਚਲ ਅੱਖਰ *) buf, ਲੰਮਾਈ,
			   HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE,
			   bd, recycle_ਅੰਕੜੇ_destroy);
  }

  /* Discard @buf returned by recycle_acquire without committing to
   * a blob.  Frees @buf if it was a fresh allocation; leaves any
   * recycled buffer untouched. */
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
  recycle_abort (ਅੱਖਰ *buf, hb_blob_t *recycled)
  {
    ਜੇ (!buf) ਵਾਪਸ;
    ਜੇ (recycled && recycled->destroy == recycle_ਅੰਕੜੇ_destroy)
    {
      ਸਵੈਚਾਲਿਤ *bd = (recycle_ਅੰਕੜੇ_t *) recycled->user_ਅੰਕੜੇ;
      ਜੇ (buf == bd->buf) ਵਾਪਸ;  /* owned by the recycled blob */
    }
    hb_free (buf);
  }

  /* Stash @blob in @slot as the recycled output for the next
   * encode.  Destroys any previously stashed blob.  Safe to call
   * with @blob = nullptr or the empty-singleton blob (treated as
   * "drop"). */
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
  recycle_stash (hb_blob_t **ਥਾਂ, hb_blob_t *blob)
  {
    hb_blob_destroy (*ਥਾਂ);
    *ਥਾਂ = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (!blob || blob == hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ())
      ਵਾਪਸ;
    *ਥਾਂ = blob;
  }
};


/*
 * hb_blob_ptr_t
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ P>
ਬਣਤਰ hb_blob_ptr_t
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_remove_ਸੂਚਕ<P> T;

  hb_blob_ptr_t (hb_blob_t *b_ = ਖਾਲੀ_ਸੂਚਕ) : b (b_) {}
  hb_blob_t * ਸੰਚਾਲਕ = (hb_blob_t *b_) { ਵਾਪਸ b = b_; }
  ਅਚਲ T * ਸੰਚਾਲਕ -> () ਅਚਲ { ਵਾਪਸ get (); }
  ਅਚਲ T & ਸੰਚਾਲਕ * () ਅਚਲ  { ਵਾਪਸ *get (); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ C> ਸੰਚਾਲਕ ਅਚਲ C * () ਅਚਲ { ਵਾਪਸ get (); }
  ਸੰਚਾਲਕ ਅਚਲ ਅੱਖਰ * () ਅਚਲ { ਵਾਪਸ (ਅਚਲ ਅੱਖਰ *) get (); }
  ਅਚਲ T * get () ਅਚਲ { ਵਾਪਸ b->as<T> (); }
  hb_blob_t * get_blob () ਅਚਲ { ਵਾਪਸ b.get_raw (); }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਲੰਮਾਈ () ਅਚਲ { ਵਾਪਸ b.get ()->ਲੰਮਾਈ; }
  ਖਾਲੀ destroy () { hb_blob_destroy (b.get_raw ()); b = ਖਾਲੀ_ਸੂਚਕ; }

  ਨਿਜੀ:
  hb_nonnull_ptr_t<hb_blob_t> b;
};


#ਜੇ_ਅੰਤ /* HB_BLOB_HH */
