/*
 * Copyright © 2019  Facebook, Inc.
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
 * Facebook Author(s): Behdad Esfahbod
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_FREE_POOL_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FREE_POOL_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

/* Memory pool for persistent alloc/free of small objects.
 *
 * Some AI musings on this, not necessarily true:
 *
 * This is a very simple implementation, but it's good enough for our
 * purposes.  It's not thread-safe.  It's not very fast.  It's not
 * very memory efficient.  It's not very cache efficient.  It's not
 * very anything efficient.  But it's simple and it works.  And it's
 * good enough for our purposes.  If you need something more
 * sophisticated, use a real allocator.  Or use a real language. */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਬਿਨਾਨਿਸ਼ਾਨੀ ChunkLen = 32>
ਬਣਤਰ hb_free_pool_t
{
  hb_free_pool_t () : next (ਖਾਲੀ_ਸੂਚਕ) {}
  ~hb_free_pool_t ()
  {
    next = ਖਾਲੀ_ਸੂਚਕ;

    + hb_iter (chunks)
    | hb_apply (hb_free)
    ;
  }

  T* alloc ()
  {
    ਜੇ (unlikely (!next))
    {
      ਜੇ (unlikely (!chunks.alloc (chunks.ਲੰਮਾਈ + 1))) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
      chunk_t *chunk = (chunk_t *) hb_malloc (ਆਕਾਰ (chunk_t));
      ਜੇ (unlikely (!chunk)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
      chunks.push (chunk);
      next = chunk->thread ();
    }

    T* obj = next;
    next = * ((T**) next);

    hb_memset (obj, 0, ਆਕਾਰ (T));

    ਵਾਪਸ obj;
  }

  ਖਾਲੀ release (T* obj)
  {
    * (T**) obj = next;
    next = obj;
  }

  ਨਿਜੀ:

  static_assert (ChunkLen > 1, "");
  static_assert (ਆਕਾਰ (T) >= ਆਕਾਰ (ਖਾਲੀ *), "");
  static_assert (alignof (T) % alignof (ਖਾਲੀ *) == 0, "");

  ਬਣਤਰ chunk_t
  {
    T* thread ()
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ARRAY_ਲੰਮਾਈ (arrayZ) - 1; i++)
	* (T**) &arrayZ[i] = &arrayZ[i + 1];

      * (T**) &arrayZ[ARRAY_ਲੰਮਾਈ (arrayZ) - 1] = ਖਾਲੀ_ਸੂਚਕ;

      ਵਾਪਸ arrayZ;
    }

    T arrayZ[ChunkLen];
  };

  T* next;
  hb_ਸਦਿਸ਼_t<chunk_t *> chunks;
};


#ਜੇ_ਅੰਤ /* HB_FREE_POOL_HH */
