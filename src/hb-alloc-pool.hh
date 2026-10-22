/*
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
 * Author(s): Behdad Esfahbod
 */

#ifndef HB_ALLOC_POOL_HH
#define HB_ALLOC_POOL_HH

#ಅನ್ತರ್ಭಾವ "hb-vector.hh"

/* Memory pool for persistent small- to medium-sized allocations.
 *
 * Some AI musings on this, not necessarily true:
 *
 * This is a very simple implementation, but it's good enough for our
 * purposes.  It's not thread-safe.  It's not very fast.  It's not
 * very memory efficient.  It's not very cache efficient.  It's not
 * very anything efficient.  But it's simple and it works.  And it's
 * good enough for our purposes.  If you need something more
 * sophisticated, use a real allocator.  Or use a real language. */

ಸಂರಚನಾ hb_alloc_pool_t
{
  unsigned ChunkSize = 65536 - 2 * ಪರಿಮಾಣ (ಶೂನ್ಯ *);

  ಶೂನ್ಯ *alloc (size_t size, unsigned alignment = 2 * ಪರಿಮಾಣ (ಶೂನ್ಯ *))
  {
    ಯದಿ (unlikely (chunks.in_error ())) ಪ್ರತ್ಯಾಗಮ nullptr;

    ಪ್ರತಿಜ್ಞಾ (alignment > 0);
    ಪ್ರತಿಜ್ಞಾ (alignment <= 2 * ಪರಿಮಾಣ (ಶೂನ್ಯ *));
    ಪ್ರತಿಜ್ಞಾ ((alignment & (alignment - 1)) == 0); /* power of two */

    ಯದಿ (size > (ChunkSize) / 4)
    {
      /* Big chunk, allocate separately.  */
      hb_vector_t<ವರ್ಣ> chunk;
      ಯದಿ (unlikely (!chunk.resize (size))) ಪ್ರತ್ಯಾಗಮ nullptr;
      ಶೂನ್ಯ *ret = chunk.arrayZ;
      chunks.push (std::move (chunk));
      ಯದಿ (chunks.in_error ()) ಪ್ರತ್ಯಾಗಮ nullptr;
      ಯದಿ (chunks.length > 1)
      {
        // Bring back the previous last chunk to the end, so that
	// we can continue to allocate from it.
	hb_swap (chunks.arrayZ[chunks.length - 1], chunks.arrayZ[chunks.length - 2]);
      }
      ಪ್ರತ್ಯಾಗಮ ret;
    }

    unsigned pad = (unsigned) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));

    // Small chunk, allocate from the last chunk.
    ಯದಿ (current_chunk.length < pad + size)
    {
      chunks.push ();
      ಯದಿ (unlikely (chunks.in_error ())) ಪ್ರತ್ಯಾಗಮ nullptr;
      hb_vector_t<ವರ್ಣ> &chunk = chunks.arrayZ[chunks.length - 1];
      ಯದಿ (unlikely (!chunk.resize (ChunkSize))) ಪ್ರತ್ಯಾಗಮ nullptr;
      current_chunk = chunk;
      pad = (unsigned) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));
    }

    current_chunk += pad;

    ಪ್ರತಿಜ್ಞಾ (current_chunk.length >= size);
    ಶೂನ್ಯ *ret = current_chunk.arrayZ;
    current_chunk += size;
    ಪ್ರತ್ಯಾಗಮ ret;
  }

  ಶೂನ್ಯ discard (ಶೂನ್ಯ *p_, size_t size)
  {
    // Reclaim memory if we can.
    ವರ್ಣ *p = (ವರ್ಣ *) p_;
    ಯದಿ (current_chunk.arrayZ == p + size && current_chunk.backwards_length >= size)
      current_chunk -= size;
  }

  ಗುಪ್ತ:
  hb_vector_t<hb_vector_t<ವರ್ಣ>> chunks;
  hb_array_t<ವರ್ಣ> current_chunk;
};


#endif /* HB_ALLOC_POOL_HH */
