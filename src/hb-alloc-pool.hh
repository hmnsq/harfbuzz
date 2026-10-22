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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ALLOC_POOL_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ALLOC_POOL_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector.hh"

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

ਬਣਤਰ hb_alloc_pool_t
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ Chunk_ਮਾਪ = 65536 - 2 * ਆਕਾਰ (ਖਾਲੀ *);

  ਖਾਲੀ *alloc (size_t ਮਾਪ, ਬਿਨਾਨਿਸ਼ਾਨੀ alignment = 2 * ਆਕਾਰ (ਖਾਲੀ *))
  {
    ਜੇ (unlikely (chunks.ਵਿੱਚ_error ())) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    ਪ੍ਰਤਿਜ੍ਞਾ (alignment > 0);
    ਪ੍ਰਤਿਜ੍ਞਾ (alignment <= 2 * ਆਕਾਰ (ਖਾਲੀ *));
    ਪ੍ਰਤਿਜ੍ਞਾ ((alignment & (alignment - 1)) == 0); /* power of two */

    ਜੇ (ਮਾਪ > (Chunk_ਮਾਪ) / 4)
    {
      /* Big chunk, allocate separately.  */
      hb_ਸਦਿਸ਼_t<ਅੱਖਰ> chunk;
      ਜੇ (unlikely (!chunk.resize (ਮਾਪ))) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
      ਖਾਲੀ *ret = chunk.arrayZ;
      chunks.push (std::move (chunk));
      ਜੇ (chunks.ਵਿੱਚ_error ()) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
      ਜੇ (chunks.ਲੰਮਾਈ > 1)
      {
        // Bring back the previous last chunk to the end, so that
	// we can continue to allocate from it.
	hb_ਅਦਲਾ_ਬਦਲੀ (chunks.arrayZ[chunks.ਲੰਮਾਈ - 1], chunks.arrayZ[chunks.ਲੰਮਾਈ - 2]);
      }
      ਵਾਪਸ ret;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ pad = (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));

    // Small chunk, allocate from the last chunk.
    ਜੇ (current_chunk.ਲੰਮਾਈ < pad + ਮਾਪ)
    {
      chunks.push ();
      ਜੇ (unlikely (chunks.ਵਿੱਚ_error ())) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
      hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &chunk = chunks.arrayZ[chunks.ਲੰਮਾਈ - 1];
      ਜੇ (unlikely (!chunk.resize (Chunk_ਮਾਪ))) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
      current_chunk = chunk;
      pad = (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));
    }

    current_chunk += pad;

    ਪ੍ਰਤਿਜ੍ਞਾ (current_chunk.ਲੰਮਾਈ >= ਮਾਪ);
    ਖਾਲੀ *ret = current_chunk.arrayZ;
    current_chunk += ਮਾਪ;
    ਵਾਪਸ ret;
  }

  ਖਾਲੀ discard (ਖਾਲੀ *p_, size_t ਮਾਪ)
  {
    // Reclaim memory if we can.
    ਅੱਖਰ *p = (ਅੱਖਰ *) p_;
    ਜੇ (current_chunk.arrayZ == p + ਮਾਪ && current_chunk.backwards_ਲੰਮਾਈ >= ਮਾਪ)
      current_chunk -= ਮਾਪ;
  }

  ਨਿਜੀ:
  hb_ਸਦਿਸ਼_t<hb_ਸਦਿਸ਼_t<ਅੱਖਰ>> chunks;
  hb_array_t<ਅੱਖਰ> current_chunk;
};


#ਜੇ_ਅੰਤ /* HB_ALLOC_POOL_HH */
