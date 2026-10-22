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

#अगर_अघोषित HB_ALLOC_POOL_HH
#घोषणा HB_ALLOC_POOL_HH

#समावेश "hb-vector.hh"

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

काष्ठा hb_alloc_pool_t
{
  अचिन्हित Chunk_आकार = 65536 - 2 * माप (व्योम *);

  व्योम *alloc (माप_प्रकार आकार, अचिन्हित alignment = 2 * माप (व्योम *))
  {
    अगर (unlikely (chunks.in_error ())) वापस nullptr;

    निश्चित (alignment > 0);
    निश्चित (alignment <= 2 * माप (व्योम *));
    निश्चित ((alignment & (alignment - 1)) == 0); /* power of two */

    अगर (आकार > (Chunk_आकार) / 4)
    {
      /* Big chunk, allocate separately.  */
      hb_vector_t<अक्षर> chunk;
      अगर (unlikely (!chunk.resize (आकार))) वापस nullptr;
      व्योम *ret = chunk.arrayZ;
      chunks.push (std::move (chunk));
      अगर (chunks.in_error ()) वापस nullptr;
      अगर (chunks.लंबाई > 1)
      {
        // Bring back the previous last chunk to the end, so that
	// we can continue to allocate from it.
	hb_swap (chunks.arrayZ[chunks.लंबाई - 1], chunks.arrayZ[chunks.लंबाई - 2]);
      }
      वापस ret;
    }

    अचिन्हित pad = (अचिन्हित) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));

    // Small chunk, allocate from the last chunk.
    अगर (current_chunk.लंबाई < pad + आकार)
    {
      chunks.push ();
      अगर (unlikely (chunks.in_error ())) वापस nullptr;
      hb_vector_t<अक्षर> &chunk = chunks.arrayZ[chunks.लंबाई - 1];
      अगर (unlikely (!chunk.resize (Chunk_आकार))) वापस nullptr;
      current_chunk = chunk;
      pad = (अचिन्हित) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));
    }

    current_chunk += pad;

    निश्चित (current_chunk.लंबाई >= आकार);
    व्योम *ret = current_chunk.arrayZ;
    current_chunk += आकार;
    वापस ret;
  }

  व्योम discard (व्योम *p_, माप_प्रकार आकार)
  {
    // Reclaim memory if we can.
    अक्षर *p = (अक्षर *) p_;
    अगर (current_chunk.arrayZ == p + आकार && current_chunk.backwards_लंबाई >= आकार)
      current_chunk -= आकार;
  }

  निजी:
  hb_vector_t<hb_vector_t<अक्षर>> chunks;
  hb_array_t<अक्षर> current_chunk;
};


#पूर्ण_अगर /* HB_ALLOC_POOL_HH */
