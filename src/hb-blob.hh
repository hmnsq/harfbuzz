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

#جيڪڏهن_وصف_نه_ٿيل HB_BLOB_HH
#وصف_ڪريو HB_BLOB_HH

#ضمن "hb.hh"


/*
 * hb_blob_t
 */

بنية hb_blob_t
{
  ~hb_blob_t () { destroy_user_معلومات (); }

  فراغ destroy_user_معلومات ()
  {
    إذا (destroy)
    {
      destroy (user_معلومات);
      user_معلومات = خالي_اشارو;
      destroy = خالي_اشارو;
    }
  }

  فراغ replace_بفر (ثابت حرف       *new_معلومات,
		       بنانشان          new_ڊيگهائي,
		       hb_يادگيري_گهڻائي_t  new_گهڻائي,
		       فراغ             *new_user_معلومات,
		       hb_destroy_func_t new_destroy)
  {
    إذا (new_معلومات != معلومات)
      destroy_user_معلومات ();
    معلومات = new_معلومات;
    ڊيگهائي = new_ڊيگهائي;
    گهڻائي = new_گهڻائي;
    user_معلومات = new_user_معلومات;
    destroy = new_destroy;
  }

  HB_اندروني منطقي try_make_writable ();
  HB_اندروني منطقي try_make_writable_inplace ();
  HB_اندروني منطقي try_make_writable_inplace_unix ();

  hb_بائيٽون_t as_بائيٽون () ثابت { أرجع hb_بائيٽون_t (معلومات, ڊيگهائي); }
  سانچو <قسم_نالو Type>
  ثابت Type* as () ثابت { أرجع as_بائيٽون ().as<Type> (); }

  عام:
  hb_object_سرو_t سرو;

  ثابت حرف *معلومات = خالي_اشارو;
  بنانشان صحيح ڊيگهائي = 0;
  hb_يادگيري_گهڻائي_t گهڻائي = (hb_يادگيري_گهڻائي_t) 0;

  فراغ *user_معلومات = خالي_اشارو;
  hb_destroy_func_t destroy = خالي_اشارو;

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

  بنية recycle_معلومات_t
  {
    حرف *buf;
    بنانشان capacity;
  };

  ساكن مضمن فراغ recycle_معلومات_destroy (فراغ *user_معلومات)
  {
    خودڪار *bd = (recycle_معلومات_t *) user_معلومات;
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
		   بنانشان   needed,
		   بنانشان  *ٻاهريون_صلاحيت,
		   حرف     **ٻاهريون_replaced_buf)
  {
    *ٻاهريون_replaced_buf = خالي_اشارو;

    إذا (recycled && recycled->destroy == recycle_معلومات_destroy)
    {
      خودڪار *bd = (recycle_معلومات_t *) recycled->user_معلومات;
      إذا (bd->capacity >= needed)
      {
	*ٻاهريون_صلاحيت = bd->capacity;
	أرجع bd->buf;
      }
      /* Grow with a 1.5x ramp to amortize repeated growth. */
      بنانشان alloc_بائيٽون = needed;
      إذا (unlikely (hb_unsigned_جوڙيو_overflows (needed, needed / 2,
					       &alloc_بائيٽون)))
	alloc_بائيٽون = needed;
      حرف *new_buf = (حرف *) hb_realloc (bd->buf, alloc_بائيٽون);
      إذا (new_buf)
      {
	bd->buf = new_buf;
	bd->capacity = alloc_بائيٽون;
	*ٻاهريون_صلاحيت = alloc_بائيٽون;
	أرجع new_buf;
      }
      /* Realloc failed.  Fall through to a fresh hb_malloc and stash
       * the old buf for the caller to free after recycle_finalize. */
      *ٻاهريون_replaced_buf = bd->buf;
    }

    حرف *buf = (حرف *) hb_malloc (needed);
    إذا (unlikely (!buf))
      أرجع خالي_اشارو;
    *ٻاهريون_صلاحيت = needed;
    أرجع buf;
  }

  /* Wrap @buf (of @capacity, with @length used) into an hb_blob_t.
   * If @recycled is one of our blobs, update and return it (cheap);
   * otherwise create a new blob.  Pass @replaced_recycled_buf from
   * recycle_acquire(). */
  ساكن مضمن hb_blob_t *
  recycle_finalize (حرف      *buf,
		    بنانشان   capacity,
		    بنانشان   ڊيگهائي,
		    hb_blob_t *recycled,
		    حرف      *replaced_recycled_buf)
  {
    إذا (recycled && recycled->destroy == recycle_معلومات_destroy)
    {
      خودڪار *bd = (recycle_معلومات_t *) recycled->user_معلومات;
      إذا (replaced_recycled_buf && replaced_recycled_buf != buf)
	hb_free (replaced_recycled_buf);
      bd->buf = buf;
      bd->capacity = capacity;
      recycled->معلومات = (ثابت حرف *) buf;
      recycled->ڊيگهائي = ڊيگهائي;
      أرجع recycled;
    }

    /* No recycled blob to update -- create a fresh one with our
     * destroy closure so the next recycle round can reuse it. */
    recycle_معلومات_t *bd = (recycle_معلومات_t *) hb_malloc (حجم (*bd));
    إذا (unlikely (!bd))
    {
      hb_free (buf);
      أرجع خالي_اشارو;
    }
    bd->buf = buf;
    bd->capacity = capacity;

    أرجع hb_blob_create ((ثابت حرف *) buf, ڊيگهائي,
			   HB_يادگيري_گهڻائي_WRITABLE,
			   bd, recycle_معلومات_destroy);
  }

  /* Discard @buf returned by recycle_acquire without committing to
   * a blob.  Frees @buf if it was a fresh allocation; leaves any
   * recycled buffer untouched. */
  ساكن مضمن فراغ
  recycle_abort (حرف *buf, hb_blob_t *recycled)
  {
    إذا (!buf) أرجع;
    إذا (recycled && recycled->destroy == recycle_معلومات_destroy)
    {
      خودڪار *bd = (recycle_معلومات_t *) recycled->user_معلومات;
      إذا (buf == bd->buf) أرجع;  /* owned by the recycled blob */
    }
    hb_free (buf);
  }

  /* Stash @blob in @slot as the recycled output for the next
   * encode.  Destroys any previously stashed blob.  Safe to call
   * with @blob = nullptr or the empty-singleton blob (treated as
   * "drop"). */
  ساكن مضمن فراغ
  recycle_stash (hb_blob_t **جڳهه, hb_blob_t *blob)
  {
    hb_blob_destroy (*جڳهه);
    *جڳهه = خالي_اشارو;
    إذا (!blob || blob == hb_blob_get_خالي_جايون ())
      أرجع;
    *جڳهه = blob;
  }
};


/*
 * hb_blob_ptr_t
 */

سانچو <قسم_نالو P>
بنية hb_blob_ptr_t
{
  تعريفنوع hb_remove_اشارو<P> T;

  hb_blob_ptr_t (hb_blob_t *b_ = خالي_اشارو) : b (b_) {}
  hb_blob_t * عامل = (hb_blob_t *b_) { أرجع b = b_; }
  ثابت T * عامل -> () ثابت { أرجع get (); }
  ثابت T & عامل * () ثابت  { أرجع *get (); }
  سانچو <قسم_نالو C> عامل ثابت C * () ثابت { أرجع get (); }
  عامل ثابت حرف * () ثابت { أرجع (ثابت حرف *) get (); }
  ثابت T * get () ثابت { أرجع b->as<T> (); }
  hb_blob_t * get_blob () ثابت { أرجع b.get_raw (); }
  بنانشان صحيح get_ڊيگهائي () ثابت { أرجع b.get ()->ڊيگهائي; }
  فراغ destroy () { hb_blob_destroy (b.get_raw ()); b = خالي_اشارو; }

  خاص:
  hb_nonnull_ptr_t<hb_blob_t> b;
};


#جيڪڏهن_پورو /* HB_BLOB_HH */
