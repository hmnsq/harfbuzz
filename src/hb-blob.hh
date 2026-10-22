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

#اگر_تعریف_نشده HB_BLOB_HH
#تعریف_کن HB_BLOB_HH

#ضمن "hb.hh"


/*
 * hb_blob_t
 */

بنية hb_blob_t
{
  ~hb_blob_t () { destroy_user_داده (); }

  فراغ destroy_user_داده ()
  {
    إذا (destroy)
    {
      destroy (user_داده);
      user_داده = اشاره_تهی;
      destroy = اشاره_تهی;
    }
  }

  فراغ replace_میانگیر (ثابت حرف       *new_داده,
		       بیعلامت          new_درازی,
		       hb_حافظه_نما_t  new_نما,
		       فراغ             *new_user_داده,
		       hb_destroy_func_t new_destroy)
  {
    إذا (new_داده != داده)
      destroy_user_داده ();
    داده = new_داده;
    درازی = new_درازی;
    نما = new_نما;
    user_داده = new_user_داده;
    destroy = new_destroy;
  }

  HB_داخلی منطقي try_make_writable ();
  HB_داخلی منطقي try_make_writable_inplace ();
  HB_داخلی منطقي try_make_writable_inplace_unix ();

  hb_بایتها_t as_بایتها () ثابت { أرجع hb_بایتها_t (داده, درازی); }
  قالب <نام_نوع Type>
  ثابت Type* as () ثابت { أرجع as_بایتها ().as<Type> (); }

  عام:
  hb_object_سرآیند_t سرآیند;

  ثابت حرف *داده = اشاره_تهی;
  بیعلامت صحيح درازی = 0;
  hb_حافظه_نما_t نما = (hb_حافظه_نما_t) 0;

  فراغ *user_داده = اشاره_تهی;
  hb_destroy_func_t destroy = اشاره_تهی;

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

  بنية recycle_داده_t
  {
    حرف *buf;
    بیعلامت capacity;
  };

  ساكن مضمن فراغ recycle_داده_destroy (فراغ *user_داده)
  {
    خودکار *bd = (recycle_داده_t *) user_داده;
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
  ساكن مضمن حرف *
  recycle_acquire (hb_blob_t *recycled,
		   بیعلامت   needed,
		   بیعلامت  *بیرون_ظرفیت,
		   حرف     **بیرون_replaced_buf)
  {
    *بیرون_replaced_buf = اشاره_تهی;

    إذا (recycled && recycled->destroy == recycle_داده_destroy)
    {
      خودکار *bd = (recycle_داده_t *) recycled->user_داده;
      إذا (bd->capacity >= needed)
      {
	*بیرون_ظرفیت = bd->capacity;
	أرجع bd->buf;
      }
      /* Grow with a 1.5x ramp to amortize repeated growth. */
      بیعلامت alloc_بایتها = needed;
      إذا (unlikely (hb_unsigned_جمع_کن_overflows (needed, needed / 2,
					       &alloc_بایتها)))
	alloc_بایتها = needed;
      حرف *new_buf = (حرف *) hb_realloc (bd->buf, alloc_بایتها);
      إذا (new_buf)
      {
	bd->buf = new_buf;
	bd->capacity = alloc_بایتها;
	*بیرون_ظرفیت = alloc_بایتها;
	أرجع new_buf;
      }
      /* Realloc failed.  Fall through to a fresh hb_malloc and stash
       * the old buf for the caller to free after recycle_finalize. */
      *بیرون_replaced_buf = bd->buf;
    }

    حرف *buf = (حرف *) hb_malloc (needed);
    إذا (unlikely (!buf))
      أرجع اشاره_تهی;
    *بیرون_ظرفیت = needed;
    أرجع buf;
  }

  /* Wrap @buf (of @capacity, with @length used) into an hb_blob_t.
   * If @recycled is one of our blobs, update and return it (cheap);
   * otherwise create a new blob.  Pass @replaced_recycled_buf from
   * recycle_acquire(). */
  ساكن مضمن hb_blob_t *
  recycle_finalize (حرف      *buf,
		    بیعلامت   capacity,
		    بیعلامت   درازی,
		    hb_blob_t *recycled,
		    حرف      *replaced_recycled_buf)
  {
    إذا (recycled && recycled->destroy == recycle_داده_destroy)
    {
      خودکار *bd = (recycle_داده_t *) recycled->user_داده;
      إذا (replaced_recycled_buf && replaced_recycled_buf != buf)
	hb_free (replaced_recycled_buf);
      bd->buf = buf;
      bd->capacity = capacity;
      recycled->داده = (ثابت حرف *) buf;
      recycled->درازی = درازی;
      أرجع recycled;
    }

    /* No recycled blob to update -- create a fresh one with our
     * destroy closure so the next recycle round can reuse it. */
    recycle_داده_t *bd = (recycle_داده_t *) hb_malloc (حجم (*bd));
    إذا (unlikely (!bd))
    {
      hb_free (buf);
      أرجع اشاره_تهی;
    }
    bd->buf = buf;
    bd->capacity = capacity;

    أرجع hb_blob_create ((ثابت حرف *) buf, درازی,
			   HB_حافظه_نما_WRITABLE,
			   bd, recycle_داده_destroy);
  }

  /* Discard @buf returned by recycle_acquire without committing to
   * a blob.  Frees @buf if it was a fresh allocation; leaves any
   * recycled buffer untouched. */
  ساكن مضمن فراغ
  recycle_abort (حرف *buf, hb_blob_t *recycled)
  {
    إذا (!buf) أرجع;
    إذا (recycled && recycled->destroy == recycle_داده_destroy)
    {
      خودکار *bd = (recycle_داده_t *) recycled->user_داده;
      إذا (buf == bd->buf) أرجع;  /* owned by the recycled blob */
    }
    hb_free (buf);
  }

  /* Stash @blob in @slot as the recycled output for the next
   * encode.  Destroys any previously stashed blob.  Safe to call
   * with @blob = nullptr or the empty-singleton blob (treated as
   * "drop"). */
  ساكن مضمن فراغ
  recycle_stash (hb_blob_t **جایگاه, hb_blob_t *blob)
  {
    hb_blob_destroy (*جایگاه);
    *جایگاه = اشاره_تهی;
    إذا (!blob || blob == hb_blob_get_خالی ())
      أرجع;
    *جایگاه = blob;
  }
};


/*
 * hb_blob_ptr_t
 */

قالب <نام_نوع P>
بنية hb_blob_ptr_t
{
  تعريفنوع hb_remove_اشاره_گر<P> T;

  hb_blob_ptr_t (hb_blob_t *b_ = اشاره_تهی) : b (b_) {}
  hb_blob_t * عملگر = (hb_blob_t *b_) { أرجع b = b_; }
  ثابت T * عملگر -> () ثابت { أرجع get (); }
  ثابت T & عملگر * () ثابت  { أرجع *get (); }
  قالب <نام_نوع C> عملگر ثابت C * () ثابت { أرجع get (); }
  عملگر ثابت حرف * () ثابت { أرجع (ثابت حرف *) get (); }
  ثابت T * get () ثابت { أرجع b->as<T> (); }
  hb_blob_t * get_blob () ثابت { أرجع b.get_raw (); }
  بیعلامت صحيح get_درازی () ثابت { أرجع b.get ()->درازی; }
  فراغ destroy () { hb_blob_destroy (b.get_raw ()); b = اشاره_تهی; }

  خاص:
  hb_nonnull_ptr_t<hb_blob_t> b;
};


#پایان_اگر /* HB_BLOB_HH */
