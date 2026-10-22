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

#অন্তর্ভাব "hb-vector.hh"

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

কাঠামো hb_alloc_pool_t
{
  unsigned Chunk_আকার = 65536 - 2 * আকার (শূন্য *);

  শূন্য *alloc (size_t আকার_2, unsigned alignment = 2 * আকার (শূন্য *))
  {
    যদি (unlikely (chunks.in_error ())) ফেরত nullptr;

    প্রতিজ্ঞা (alignment > 0);
    প্রতিজ্ঞা (alignment <= 2 * আকার (শূন্য *));
    প্রতিজ্ঞা ((alignment & (alignment - 1)) == 0); /* power of two */

    যদি (আকার_2 > (Chunk_আকার) / 4)
    {
      /* Big chunk, allocate separately.  */
      hb_vector_t<অক্ষর> chunk;
      যদি (unlikely (!chunk.resize (আকার_2))) ফেরত nullptr;
      শূন্য *ret = chunk.arrayZ;
      chunks.push (std::move (chunk));
      যদি (chunks.in_error ()) ফেরত nullptr;
      যদি (chunks.দৈর্ঘ্য > 1)
      {
        // Bring back the previous last chunk to the end, so that
	// we can continue to allocate from it.
	hb_swap (chunks.arrayZ[chunks.দৈর্ঘ্য - 1], chunks.arrayZ[chunks.দৈর্ঘ্য - 2]);
      }
      ফেরত ret;
    }

    unsigned pad = (unsigned) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));

    // Small chunk, allocate from the last chunk.
    যদি (current_chunk.দৈর্ঘ্য < pad + আকার_2)
    {
      chunks.push ();
      যদি (unlikely (chunks.in_error ())) ফেরত nullptr;
      hb_vector_t<অক্ষর> &chunk = chunks.arrayZ[chunks.দৈর্ঘ্য - 1];
      যদি (unlikely (!chunk.resize (Chunk_আকার))) ফেরত nullptr;
      current_chunk = chunk;
      pad = (unsigned) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));
    }

    current_chunk += pad;

    প্রতিজ্ঞা (current_chunk.দৈর্ঘ্য >= আকার_2);
    শূন্য *ret = current_chunk.arrayZ;
    current_chunk += আকার_2;
    ফেরত ret;
  }

  শূন্য discard (শূন্য *p_, size_t আকার_2)
  {
    // Reclaim memory if we can.
    অক্ষর *p = (অক্ষর *) p_;
    যদি (current_chunk.arrayZ == p + আকার_2 && current_chunk.backwards_দৈর্ঘ্য >= আকার_2)
      current_chunk -= আকার_2;
  }

  ব্যক্তিগত:
  hb_vector_t<hb_vector_t<অক্ষর>> chunks;
  hb_array_t<অক্ষর> current_chunk;
};


#endif /* HB_ALLOC_POOL_HH */
