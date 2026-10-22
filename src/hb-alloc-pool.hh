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

#اگر_غیر_تعریف_شدہ HB_ALLOC_POOL_HH
#تعریف_کرو HB_ALLOC_POOL_HH

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

ساخت hb_alloc_pool_t
{
  غیرعلامتی Chunk_سائز = 65536 - 2 * حجم (خالی *);

  خالی *alloc (size_t سائز, غیرعلامتی alignment = 2 * حجم (خالی *))
  {
    اگر (unlikely (chunks.اندر_error ())) واپس خالی_اشاریہ;

    أكد (alignment > 0);
    أكد (alignment <= 2 * حجم (خالی *));
    أكد ((alignment & (alignment - 1)) == 0); /* power of two */

    اگر (سائز > (Chunk_سائز) / 4)
    {
      /* Big chunk, allocate separately.  */
      hb_سمتیہ_t<حرف> chunk;
      اگر (unlikely (!chunk.resize (سائز))) واپس خالی_اشاریہ;
      خالی *ret = chunk.arrayZ;
      chunks.push (std::move (chunk));
      اگر (chunks.اندر_error ()) واپس خالی_اشاریہ;
      اگر (chunks.لمبائی > 1)
      {
        // Bring back the previous last chunk to the end, so that
	// we can continue to allocate from it.
	hb_تبادلہ (chunks.arrayZ[chunks.لمبائی - 1], chunks.arrayZ[chunks.لمبائی - 2]);
      }
      واپس ret;
    }

    غیرعلامتی pad = (غیرعلامتی) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));

    // Small chunk, allocate from the last chunk.
    اگر (current_chunk.لمبائی < pad + سائز)
    {
      chunks.push ();
      اگر (unlikely (chunks.اندر_error ())) واپس خالی_اشاریہ;
      hb_سمتیہ_t<حرف> &chunk = chunks.arrayZ[chunks.لمبائی - 1];
      اگر (unlikely (!chunk.resize (Chunk_سائز))) واپس خالی_اشاریہ;
      current_chunk = chunk;
      pad = (غیرعلامتی) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));
    }

    current_chunk += pad;

    أكد (current_chunk.لمبائی >= سائز);
    خالی *ret = current_chunk.arrayZ;
    current_chunk += سائز;
    واپس ret;
  }

  خالی discard (خالی *p_, size_t سائز)
  {
    // Reclaim memory if we can.
    حرف *p = (حرف *) p_;
    اگر (current_chunk.arrayZ == p + سائز && current_chunk.backwards_لمبائی >= سائز)
      current_chunk -= سائز;
  }

  نجی:
  hb_سمتیہ_t<hb_سمتیہ_t<حرف>> chunks;
  hb_array_t<حرف> current_chunk;
};


#اگر_ختم /* HB_ALLOC_POOL_HH */
