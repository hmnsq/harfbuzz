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

#اگر_غیر_تعریف_شدہ HB_BLOB_HH
#تعریف_کرو HB_BLOB_HH

#شامل "hb.hh"


/*
 * hb_blob_t
 */

ساخت hb_blob_t
{
  ~hb_blob_t () { destroy_user_ڈیٹا (); }

  خالی destroy_user_ڈیٹا ()
  {
    اگر (destroy)
    {
      destroy (user_ڈیٹا);
      user_ڈیٹا = خالی_اشاریہ;
      destroy = خالی_اشاریہ;
    }
  }

  خالی replace_بفر (ثابت حرف       *new_ڈیٹا,
		       غیرعلامتی          new_لمبائی,
		       hb_حافظہ_منوال_t  new_منوال,
		       خالی             *new_user_ڈیٹا,
		       hb_destroy_func_t new_destroy)
  {
    اگر (new_ڈیٹا != ڈیٹا)
      destroy_user_ڈیٹا ();
    ڈیٹا = new_ڈیٹا;
    لمبائی = new_لمبائی;
    منوال = new_منوال;
    user_ڈیٹا = new_user_ڈیٹا;
    destroy = new_destroy;
  }

  HB_اندرونی منطقی try_make_writable ();
  HB_اندرونی منطقی try_make_writable_inplace ();
  HB_اندرونی منطقی try_make_writable_inplace_unix ();

  hb_بائٹیں_t as_بائٹیں () ثابت { واپس hb_بائٹیں_t (ڈیٹا, لمبائی); }
  سانچہ <قسم_نام Type>
  ثابت Type* as () ثابت { واپس as_بائٹیں ().as<Type> (); }

  عوامی:
  hb_object_سرخی_t سرخی;

  ثابت حرف *ڈیٹا = خالی_اشاریہ;
  غیرعلامتی صحیح لمبائی = 0;
  hb_حافظہ_منوال_t منوال = (hb_حافظہ_منوال_t) 0;

  خالی *user_ڈیٹا = خالی_اشاریہ;
  hb_destroy_func_t destroy = خالی_اشاریہ;

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

  ساخت recycle_ڈیٹا_t
  {
    حرف *buf;
    غیرعلامتی capacity;
  };

  ساکن اندرونی خالی recycle_ڈیٹا_destroy (خالی *user_ڈیٹا)
  {
    خودکار *bd = (recycle_ڈیٹا_t *) user_ڈیٹا;
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
  ساکن اندرونی حرف *
  recycle_acquire (hb_blob_t *recycled,
		   غیرعلامتی   needed,
		   غیرعلامتی  *برآمد_استعداد,
		   حرف     **برآمد_replaced_buf)
  {
    *برآمد_replaced_buf = خالی_اشاریہ;

    اگر (recycled && recycled->destroy == recycle_ڈیٹا_destroy)
    {
      خودکار *bd = (recycle_ڈیٹا_t *) recycled->user_ڈیٹا;
      اگر (bd->capacity >= needed)
      {
	*برآمد_استعداد = bd->capacity;
	واپس bd->buf;
      }
      /* Grow with a 1.5x ramp to amortize repeated growth. */
      غیرعلامتی alloc_بائٹیں = needed;
      اگر (unlikely (hb_unsigned_جوڑو_overflows (needed, needed / 2,
					       &alloc_بائٹیں)))
	alloc_بائٹیں = needed;
      حرف *new_buf = (حرف *) hb_realloc (bd->buf, alloc_بائٹیں);
      اگر (new_buf)
      {
	bd->buf = new_buf;
	bd->capacity = alloc_بائٹیں;
	*برآمد_استعداد = alloc_بائٹیں;
	واپس new_buf;
      }
      /* Realloc failed.  Fall through to a fresh hb_malloc and stash
       * the old buf for the caller to free after recycle_finalize. */
      *برآمد_replaced_buf = bd->buf;
    }

    حرف *buf = (حرف *) hb_malloc (needed);
    اگر (unlikely (!buf))
      واپس خالی_اشاریہ;
    *برآمد_استعداد = needed;
    واپس buf;
  }

  /* Wrap @buf (of @capacity, with @length used) into an hb_blob_t.
   * If @recycled is one of our blobs, update and return it (cheap);
   * otherwise create a new blob.  Pass @replaced_recycled_buf from
   * recycle_acquire(). */
  ساکن اندرونی hb_blob_t *
  recycle_finalize (حرف      *buf,
		    غیرعلامتی   capacity,
		    غیرعلامتی   لمبائی,
		    hb_blob_t *recycled,
		    حرف      *replaced_recycled_buf)
  {
    اگر (recycled && recycled->destroy == recycle_ڈیٹا_destroy)
    {
      خودکار *bd = (recycle_ڈیٹا_t *) recycled->user_ڈیٹا;
      اگر (replaced_recycled_buf && replaced_recycled_buf != buf)
	hb_free (replaced_recycled_buf);
      bd->buf = buf;
      bd->capacity = capacity;
      recycled->ڈیٹا = (ثابت حرف *) buf;
      recycled->لمبائی = لمبائی;
      واپس recycled;
    }

    /* No recycled blob to update -- create a fresh one with our
     * destroy closure so the next recycle round can reuse it. */
    recycle_ڈیٹا_t *bd = (recycle_ڈیٹا_t *) hb_malloc (حجم (*bd));
    اگر (unlikely (!bd))
    {
      hb_free (buf);
      واپس خالی_اشاریہ;
    }
    bd->buf = buf;
    bd->capacity = capacity;

    واپس hb_blob_create ((ثابت حرف *) buf, لمبائی,
			   HB_حافظہ_منوال_WRITABLE,
			   bd, recycle_ڈیٹا_destroy);
  }

  /* Discard @buf returned by recycle_acquire without committing to
   * a blob.  Frees @buf if it was a fresh allocation; leaves any
   * recycled buffer untouched. */
  ساکن اندرونی خالی
  recycle_abort (حرف *buf, hb_blob_t *recycled)
  {
    اگر (!buf) واپس;
    اگر (recycled && recycled->destroy == recycle_ڈیٹا_destroy)
    {
      خودکار *bd = (recycle_ڈیٹا_t *) recycled->user_ڈیٹا;
      اگر (buf == bd->buf) واپس;  /* owned by the recycled blob */
    }
    hb_free (buf);
  }

  /* Stash @blob in @slot as the recycled output for the next
   * encode.  Destroys any previously stashed blob.  Safe to call
   * with @blob = nullptr or the empty-singleton blob (treated as
   * "drop"). */
  ساکن اندرونی خالی
  recycle_stash (hb_blob_t **خانہ, hb_blob_t *blob)
  {
    hb_blob_destroy (*خانہ);
    *خانہ = خالی_اشاریہ;
    اگر (!blob || blob == hb_blob_get_خالی_خانے ())
      واپس;
    *خانہ = blob;
  }
};


/*
 * hb_blob_ptr_t
 */

سانچہ <قسم_نام P>
ساخت hb_blob_ptr_t
{
  قسمتعریف hb_remove_اشارہ_گر<P> T;

  hb_blob_ptr_t (hb_blob_t *b_ = خالی_اشاریہ) : b (b_) {}
  hb_blob_t * عامل = (hb_blob_t *b_) { واپس b = b_; }
  ثابت T * عامل -> () ثابت { واپس get (); }
  ثابت T & عامل * () ثابت  { واپس *get (); }
  سانچہ <قسم_نام C> عامل ثابت C * () ثابت { واپس get (); }
  عامل ثابت حرف * () ثابت { واپس (ثابت حرف *) get (); }
  ثابت T * get () ثابت { واپس b->as<T> (); }
  hb_blob_t * get_blob () ثابت { واپس b.get_raw (); }
  غیرعلامتی صحیح get_لمبائی () ثابت { واپس b.get ()->لمبائی; }
  خالی destroy () { hb_blob_destroy (b.get_raw ()); b = خالی_اشاریہ; }

  نجی:
  hb_nonnull_ptr_t<hb_blob_t> b;
};


#اگر_ختم /* HB_BLOB_HH */
