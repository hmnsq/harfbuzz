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

#جے_بنا_تعریف HB_BLOB_HH
#تعریف_کرو HB_BLOB_HH

#ضمن "hb.hh"


/*
 * hb_blob_t
 */

بنية hb_blob_t
{
  ~hb_blob_t () { destroy_user_انکڑے (); }

  فراغ destroy_user_انکڑے ()
  {
    إذا (destroy)
    {
      destroy (user_انکڑے);
      user_انکڑے = خالی_سوچک;
      destroy = خالی_سوچک;
    }
  }

  فراغ replace_بفر (ثابت حرف       *new_انکڑے,
		       بنانشانی          new_لمائی,
		       hb_حافظہ_منوال_t  new_منوال,
		       فراغ             *new_user_انکڑے,
		       hb_destroy_func_t new_destroy)
  {
    إذا (new_انکڑے != انکڑے)
      destroy_user_انکڑے ();
    انکڑے = new_انکڑے;
    لمائی = new_لمائی;
    منوال = new_منوال;
    user_انکڑے = new_user_انکڑے;
    destroy = new_destroy;
  }

  HB_اندرونی منطقي try_make_writable ();
  HB_اندرونی منطقي try_make_writable_inplace ();
  HB_اندرونی منطقي try_make_writable_inplace_unix ();

  hb_بائٹاں_t as_بائٹاں () ثابت { أرجع hb_بائٹاں_t (انکڑے, لمائی); }
  سانچا <قسم_ناں Type>
  ثابت Type* as () ثابت { أرجع as_بائٹاں ().as<Type> (); }

  عام:
  hb_object_سرخی_t سرخی;

  ثابت حرف *انکڑے = خالی_سوچک;
  بنانشانی صحيح لمائی = 0;
  hb_حافظہ_منوال_t منوال = (hb_حافظہ_منوال_t) 0;

  فراغ *user_انکڑے = خالی_سوچک;
  hb_destroy_func_t destroy = خالی_سوچک;

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

  بنية recycle_انکڑے_t
  {
    حرف *buf;
    بنانشانی capacity;
  };

  ساكن مضمن فراغ recycle_انکڑے_destroy (فراغ *user_انکڑے)
  {
    خودکار *bd = (recycle_انکڑے_t *) user_انکڑے;
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
		   بنانشانی   needed,
		   بنانشانی  *باہرلا_سمرتھا,
		   حرف     **باہرلا_replaced_buf)
  {
    *باہرلا_replaced_buf = خالی_سوچک;

    إذا (recycled && recycled->destroy == recycle_انکڑے_destroy)
    {
      خودکار *bd = (recycle_انکڑے_t *) recycled->user_انکڑے;
      إذا (bd->capacity >= needed)
      {
	*باہرلا_سمرتھا = bd->capacity;
	أرجع bd->buf;
      }
      /* Grow with a 1.5x ramp to amortize repeated growth. */
      بنانشانی alloc_بائٹاں = needed;
      إذا (unlikely (hb_unsigned_جوڑو_overflows (needed, needed / 2,
					       &alloc_بائٹاں)))
	alloc_بائٹاں = needed;
      حرف *new_buf = (حرف *) hb_realloc (bd->buf, alloc_بائٹاں);
      إذا (new_buf)
      {
	bd->buf = new_buf;
	bd->capacity = alloc_بائٹاں;
	*باہرلا_سمرتھا = alloc_بائٹاں;
	أرجع new_buf;
      }
      /* Realloc failed.  Fall through to a fresh hb_malloc and stash
       * the old buf for the caller to free after recycle_finalize. */
      *باہرلا_replaced_buf = bd->buf;
    }

    حرف *buf = (حرف *) hb_malloc (needed);
    إذا (unlikely (!buf))
      أرجع خالی_سوچک;
    *باہرلا_سمرتھا = needed;
    أرجع buf;
  }

  /* Wrap @buf (of @capacity, with @length used) into an hb_blob_t.
   * If @recycled is one of our blobs, update and return it (cheap);
   * otherwise create a new blob.  Pass @replaced_recycled_buf from
   * recycle_acquire(). */
  ساكن مضمن hb_blob_t *
  recycle_finalize (حرف      *buf,
		    بنانشانی   capacity,
		    بنانشانی   لمائی,
		    hb_blob_t *recycled,
		    حرف      *replaced_recycled_buf)
  {
    إذا (recycled && recycled->destroy == recycle_انکڑے_destroy)
    {
      خودکار *bd = (recycle_انکڑے_t *) recycled->user_انکڑے;
      إذا (replaced_recycled_buf && replaced_recycled_buf != buf)
	hb_free (replaced_recycled_buf);
      bd->buf = buf;
      bd->capacity = capacity;
      recycled->انکڑے = (ثابت حرف *) buf;
      recycled->لمائی = لمائی;
      أرجع recycled;
    }

    /* No recycled blob to update -- create a fresh one with our
     * destroy closure so the next recycle round can reuse it. */
    recycle_انکڑے_t *bd = (recycle_انکڑے_t *) hb_malloc (حجم (*bd));
    إذا (unlikely (!bd))
    {
      hb_free (buf);
      أرجع خالی_سوچک;
    }
    bd->buf = buf;
    bd->capacity = capacity;

    أرجع hb_blob_create ((ثابت حرف *) buf, لمائی,
			   HB_حافظہ_منوال_WRITABLE,
			   bd, recycle_انکڑے_destroy);
  }

  /* Discard @buf returned by recycle_acquire without committing to
   * a blob.  Frees @buf if it was a fresh allocation; leaves any
   * recycled buffer untouched. */
  ساكن مضمن فراغ
  recycle_abort (حرف *buf, hb_blob_t *recycled)
  {
    إذا (!buf) أرجع;
    إذا (recycled && recycled->destroy == recycle_انکڑے_destroy)
    {
      خودکار *bd = (recycle_انکڑے_t *) recycled->user_انکڑے;
      إذا (buf == bd->buf) أرجع;  /* owned by the recycled blob */
    }
    hb_free (buf);
  }

  /* Stash @blob in @slot as the recycled output for the next
   * encode.  Destroys any previously stashed blob.  Safe to call
   * with @blob = nullptr or the empty-singleton blob (treated as
   * "drop"). */
  ساكن مضمن فراغ
  recycle_stash (hb_blob_t **تھاں, hb_blob_t *blob)
  {
    hb_blob_destroy (*تھاں);
    *تھاں = خالی_سوچک;
    إذا (!blob || blob == hb_blob_get_خالی_تھاواں ())
      أرجع;
    *تھاں = blob;
  }
};


/*
 * hb_blob_ptr_t
 */

سانچا <قسم_ناں P>
بنية hb_blob_ptr_t
{
  تعريفنوع hb_remove_اشارہ_گر<P> T;

  hb_blob_ptr_t (hb_blob_t *b_ = خالی_سوچک) : b (b_) {}
  hb_blob_t * عامل = (hb_blob_t *b_) { أرجع b = b_; }
  ثابت T * عامل -> () ثابت { أرجع get (); }
  ثابت T & عامل * () ثابت  { أرجع *get (); }
  سانچا <قسم_ناں C> عامل ثابت C * () ثابت { أرجع get (); }
  عامل ثابت حرف * () ثابت { أرجع (ثابت حرف *) get (); }
  ثابت T * get () ثابت { أرجع b->as<T> (); }
  hb_blob_t * get_blob () ثابت { أرجع b.get_raw (); }
  بنانشانی صحيح get_لمائی () ثابت { أرجع b.get ()->لمائی; }
  فراغ destroy () { hb_blob_destroy (b.get_raw ()); b = خالی_سوچک; }

  خاص:
  hb_nonnull_ptr_t<hb_blob_t> b;
};


#جے_انت /* HB_BLOB_HH */
