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

#अगर_अघोषित HB_FREE_POOL_HH
#घोषणा HB_FREE_POOL_HH

#समावेश "hb.hh"

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

ढाँचा <typename T, अचिन्हित ChunkLen = 32>
काष्ठा hb_free_pool_t
{
  hb_free_pool_t () : next (nullptr) {}
  ~hb_free_pool_t ()
  {
    next = nullptr;

    + hb_iter (chunks)
    | hb_apply (hb_free)
    ;
  }

  T* alloc ()
  {
    अगर (unlikely (!next))
    {
      अगर (unlikely (!chunks.alloc (chunks.लंबाई + 1))) वापस nullptr;
      chunk_t *chunk = (chunk_t *) hb_malloc (माप (chunk_t));
      अगर (unlikely (!chunk)) वापस nullptr;
      chunks.push (chunk);
      next = chunk->thread ();
    }

    T* obj = next;
    next = * ((T**) next);

    hb_memset (obj, 0, माप (T));

    वापस obj;
  }

  व्योम release (T* obj)
  {
    * (T**) obj = next;
    next = obj;
  }

  निजी:

  static_assert (ChunkLen > 1, "");
  static_assert (माप (T) >= माप (व्योम *), "");
  static_assert (alignof (T) % alignof (व्योम *) == 0, "");

  काष्ठा chunk_t
  {
    T* thread ()
    {
      क्रम (अचिन्हित i = 0; i < ARRAY_लंबाई (arrayZ) - 1; i++)
	* (T**) &arrayZ[i] = &arrayZ[i + 1];

      * (T**) &arrayZ[ARRAY_लंबाई (arrayZ) - 1] = nullptr;

      वापस arrayZ;
    }

    T arrayZ[ChunkLen];
  };

  T* next;
  hb_vector_t<chunk_t *> chunks;
};


#पूर्ण_अगर /* HB_FREE_POOL_HH */
