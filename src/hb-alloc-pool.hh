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

#اگر_تعریف_نشده HB_ALLOC_POOL_HH
#تعریف_کن HB_ALLOC_POOL_HH

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
  بیعلامت Chunk_اندازه = 65536 - 2 * حجم (فراغ *);

  فراغ *alloc (size_t اندازه, بیعلامت alignment = 2 * حجم (فراغ *))
  {
    إذا (unlikely (chunks.درون_error ())) أرجع اشاره_تهی;

    أكد (alignment > 0);
    أكد (alignment <= 2 * حجم (فراغ *));
    أكد ((alignment & (alignment - 1)) == 0); /* power of two */

    إذا (اندازه > (Chunk_اندازه) / 4)
    {
      /* Big chunk, allocate separately.  */
      hb_وکتور_t<حرف> chunk;
      إذا (unlikely (!chunk.resize (اندازه))) أرجع اشاره_تهی;
      فراغ *ret = chunk.arrayZ;
      chunks.push (std::move (chunk));
      إذا (chunks.درون_error ()) أرجع اشاره_تهی;
      إذا (chunks.درازی > 1)
      {
        // Bring back the previous last chunk to the end, so that
	// we can continue to allocate from it.
	hb_جابجا (chunks.arrayZ[chunks.درازی - 1], chunks.arrayZ[chunks.درازی - 2]);
      }
      أرجع ret;
    }

    بیعلامت pad = (بیعلامت) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));

    // Small chunk, allocate from the last chunk.
    إذا (current_chunk.درازی < pad + اندازه)
    {
      chunks.push ();
      إذا (unlikely (chunks.درون_error ())) أرجع اشاره_تهی;
      hb_وکتور_t<حرف> &chunk = chunks.arrayZ[chunks.درازی - 1];
      إذا (unlikely (!chunk.resize (Chunk_اندازه))) أرجع اشاره_تهی;
      current_chunk = chunk;
      pad = (بیعلامت) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));
    }

    current_chunk += pad;

    أكد (current_chunk.درازی >= اندازه);
    فراغ *ret = current_chunk.arrayZ;
    current_chunk += اندازه;
    أرجع ret;
  }

  فراغ discard (فراغ *p_, size_t اندازه)
  {
    // Reclaim memory if we can.
    حرف *p = (حرف *) p_;
    إذا (current_chunk.arrayZ == p + اندازه && current_chunk.backwards_درازی >= اندازه)
      current_chunk -= اندازه;
  }

  خاص:
  hb_وکتور_t<hb_وکتور_t<حرف>> chunks;
  hb_array_t<حرف> current_chunk;
};


#پایان_اگر /* HB_ALLOC_POOL_HH */
