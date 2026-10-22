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

#ଅନ୍ତର୍ଭାଵ "hb-vector.hh"

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

ସଂରଚନା hb_alloc_pool_t
{
  unsigned ChunkSize = 65536 - 2 * ଆକାର (ଶୂନ୍ୟ *);

  ଶୂନ୍ୟ *alloc (size_t size, unsigned alignment = 2 * ଆକାର (ଶୂନ୍ୟ *))
  {
    ଯଦି (unlikely (chunks.in_error ())) ଫେରନ୍ତୁ nullptr;

    ପ୍ରତିଜ୍ଞା (alignment > 0);
    ପ୍ରତିଜ୍ଞା (alignment <= 2 * ଆକାର (ଶୂନ୍ୟ *));
    ପ୍ରତିଜ୍ଞା ((alignment & (alignment - 1)) == 0); /* power of two */

    ଯଦି (size > (ChunkSize) / 4)
    {
      /* Big chunk, allocate separately.  */
      hb_vector_t<ଅକ୍ଷର> chunk;
      ଯଦି (unlikely (!chunk.resize (size))) ଫେରନ୍ତୁ nullptr;
      ଶୂନ୍ୟ *ret = chunk.arrayZ;
      chunks.push (std::move (chunk));
      ଯଦି (chunks.in_error ()) ଫେରନ୍ତୁ nullptr;
      ଯଦି (chunks.length > 1)
      {
        // Bring back the previous last chunk to the end, so that
	// we can continue to allocate from it.
	hb_swap (chunks.arrayZ[chunks.length - 1], chunks.arrayZ[chunks.length - 2]);
      }
      ଫେରନ୍ତୁ ret;
    }

    unsigned pad = (unsigned) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));

    // Small chunk, allocate from the last chunk.
    ଯଦି (current_chunk.length < pad + size)
    {
      chunks.push ();
      ଯଦି (unlikely (chunks.in_error ())) ଫେରନ୍ତୁ nullptr;
      hb_vector_t<ଅକ୍ଷର> &chunk = chunks.arrayZ[chunks.length - 1];
      ଯଦି (unlikely (!chunk.resize (ChunkSize))) ଫେରନ୍ତୁ nullptr;
      current_chunk = chunk;
      pad = (unsigned) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));
    }

    current_chunk += pad;

    ପ୍ରତିଜ୍ଞା (current_chunk.length >= size);
    ଶୂନ୍ୟ *ret = current_chunk.arrayZ;
    current_chunk += size;
    ଫେରନ୍ତୁ ret;
  }

  ଶୂନ୍ୟ discard (ଶୂନ୍ୟ *p_, size_t size)
  {
    // Reclaim memory if we can.
    ଅକ୍ଷର *p = (ଅକ୍ଷର *) p_;
    ଯଦି (current_chunk.arrayZ == p + size && current_chunk.backwards_length >= size)
      current_chunk -= size;
  }

  ବ୍ୟକ୍ତିଗତ:
  hb_vector_t<hb_vector_t<ଅକ୍ଷର>> chunks;
  hb_array_t<ଅକ୍ଷର> current_chunk;
};


#endif /* HB_ALLOC_POOL_HH */
