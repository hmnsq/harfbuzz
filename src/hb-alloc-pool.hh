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

#شامل "hb-vector.hh"

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

ساختار hb_alloc_pool_t
{
  unsigned ChunkSize = 65536 - 2 * اندازه (تهی *);

  تهی *alloc (size_t size, unsigned alignment = 2 * اندازه (تهی *))
  {
    اگر (unlikely (chunks.in_error ())) بازگشت nullptr;

    أكد (alignment > 0);
    أكد (alignment <= 2 * اندازه (تهی *));
    أكد ((alignment & (alignment - 1)) == 0); /* power of two */

    اگر (size > (ChunkSize) / 4)
    {
      /* Big chunk, allocate separately.  */
      hb_vector_t<نویسه> chunk;
      اگر (unlikely (!chunk.resize (size))) بازگشت nullptr;
      تهی *ret = chunk.arrayZ;
      chunks.push (std::move (chunk));
      اگر (chunks.in_error ()) بازگشت nullptr;
      اگر (chunks.length > 1)
      {
        // Bring back the previous last chunk to the end, so that
	// we can continue to allocate from it.
	hb_swap (chunks.arrayZ[chunks.length - 1], chunks.arrayZ[chunks.length - 2]);
      }
      بازگشت ret;
    }

    unsigned pad = (unsigned) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));

    // Small chunk, allocate from the last chunk.
    اگر (current_chunk.length < pad + size)
    {
      chunks.push ();
      اگر (unlikely (chunks.in_error ())) بازگشت nullptr;
      hb_vector_t<نویسه> &chunk = chunks.arrayZ[chunks.length - 1];
      اگر (unlikely (!chunk.resize (ChunkSize))) بازگشت nullptr;
      current_chunk = chunk;
      pad = (unsigned) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));
    }

    current_chunk += pad;

    أكد (current_chunk.length >= size);
    تهی *ret = current_chunk.arrayZ;
    current_chunk += size;
    بازگشت ret;
  }

  تهی discard (تهی *p_, size_t size)
  {
    // Reclaim memory if we can.
    نویسه *p = (نویسه *) p_;
    اگر (current_chunk.arrayZ == p + size && current_chunk.backwards_length >= size)
      current_chunk -= size;
  }

  خصوصی:
  hb_vector_t<hb_vector_t<نویسه>> chunks;
  hb_array_t<نویسه> current_chunk;
};


#endif /* HB_ALLOC_POOL_HH */
