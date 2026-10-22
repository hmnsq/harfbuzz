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

#جے_بنا_تعریف HB_ALLOC_POOL_HH
#تعریف_کرو HB_ALLOC_POOL_HH

#ضمن "hb-vector.hh"

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

بنية hb_alloc_pool_t
{
  بنانشانی Chunk_ناپ = 65536 - 2 * حجم (فراغ *);

  فراغ *alloc (size_t ناپ, بنانشانی alignment = 2 * حجم (فراغ *))
  {
    إذا (unlikely (chunks.وچ_error ())) أرجع خالی_سوچک;

    أكد (alignment > 0);
    أكد (alignment <= 2 * حجم (فراغ *));
    أكد ((alignment & (alignment - 1)) == 0); /* power of two */

    إذا (ناپ > (Chunk_ناپ) / 4)
    {
      /* Big chunk, allocate separately.  */
      hb_سمتیہ_t<حرف> chunk;
      إذا (unlikely (!chunk.resize (ناپ))) أرجع خالی_سوچک;
      فراغ *ret = chunk.arrayZ;
      chunks.push (std::move (chunk));
      إذا (chunks.وچ_error ()) أرجع خالی_سوچک;
      إذا (chunks.لمائی > 1)
      {
        // Bring back the previous last chunk to the end, so that
	// we can continue to allocate from it.
	hb_وٹاندرا (chunks.arrayZ[chunks.لمائی - 1], chunks.arrayZ[chunks.لمائی - 2]);
      }
      أرجع ret;
    }

    بنانشانی pad = (بنانشانی) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));

    // Small chunk, allocate from the last chunk.
    إذا (current_chunk.لمائی < pad + ناپ)
    {
      chunks.push ();
      إذا (unlikely (chunks.وچ_error ())) أرجع خالی_سوچک;
      hb_سمتیہ_t<حرف> &chunk = chunks.arrayZ[chunks.لمائی - 1];
      إذا (unlikely (!chunk.resize (Chunk_ناپ))) أرجع خالی_سوچک;
      current_chunk = chunk;
      pad = (بنانشانی) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));
    }

    current_chunk += pad;

    أكد (current_chunk.لمائی >= ناپ);
    فراغ *ret = current_chunk.arrayZ;
    current_chunk += ناپ;
    أرجع ret;
  }

  فراغ discard (فراغ *p_, size_t ناپ)
  {
    // Reclaim memory if we can.
    حرف *p = (حرف *) p_;
    إذا (current_chunk.arrayZ == p + ناپ && current_chunk.backwards_لمائی >= ناپ)
      current_chunk -= ناپ;
  }

  خاص:
  hb_سمتیہ_t<hb_سمتیہ_t<حرف>> chunks;
  hb_array_t<حرف> current_chunk;
};


#جے_انت /* HB_ALLOC_POOL_HH */
