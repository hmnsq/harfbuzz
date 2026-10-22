/*
 * Copyright © 2007,2008,2009,2010  Red Hat, Inc.
 * Copyright © 2012,2018  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 * Facebook Author(s): Behdad Esfahbod
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SERIALIZE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SERIALIZE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-blob.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-map.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-free-pool.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-serialize.h"

/*
 * Serialize
 */

ਗਿਣਤੀ hb_serialize_error_t {
  HB_SERIALIZE_ERROR_NONE =            0x00000000u,
  HB_SERIALIZE_ERROR_OTHER =           0x00000001u,
  HB_SERIALIZE_ERROR_ਵਿੱਥ_ਉਛਾਲ = 0x00000002u,
  HB_SERIALIZE_ERROR_ਬਾਹਰਲਾ_ਦਾ_ਗੁੰਜਾਇਸ਼ =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_ਉਛਾਲ =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_ਉਛਾਲ =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

ਬਣਤਰ hb_serialize_context_t
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ objidx_t;

  ਗਿਣਤੀ whence_t {
     ਸਿਰ,	/* Relative to the current object head (default). */
     ਪੂਛ_2,	/* Relative to the current object tail after packed. */
     Absolute	/* Absolute: from the start of the serialize buffer. */
   };



  ਬਣਤਰ object_t
  {
    ਖਾਲੀ fini () {
      real_links.fini ();
      virtual_links.fini ();
    }

    object_t () = ਮੂਲ;

    object_t (ਅਚਲ hb_subset_serialize_object_t &o)
    {
      ਸਿਰ_2 = o.ਸਿਰ_2;
      ਪੂਛ = o.ਪੂਛ;
      next = ਖਾਲੀ_ਸੂਚਕ;
      real_links.alloc_exact (o.ਅੰਕ_real_links);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0 ; i < o.ਅੰਕ_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.ਅੰਕ_virtual_links);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < o.ਅੰਕ_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ
    {
      ਵਾਪਸ real_links.ਵਿੱਚ_error() || virtual_links.ਵਿੱਚ_error();
    }

    ਤਾਰਕਿਕ ਜੋੜੋ_virtual_link (objidx_t objidx)
    {
      ਜੇ (!objidx)
        ਵਾਪਸ ਝੂਠ;

      ਸਵੈਚਾਲਿਤ& link = *virtual_links.push ();
      ਜੇ (virtual_links.ਵਿੱਚ_error ())
        ਵਾਪਸ ਝੂਠ;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      ਵਾਪਸ ਸੱਚ;
    }

    ਮਿੱਤਰ ਖਾਲੀ swap (object_t& a, object_t& b) noexcept
    {
      hb_ਅਦਲਾ_ਬਦਲੀ (a.ਸਿਰ_2, b.ਸਿਰ_2);
      hb_ਅਦਲਾ_ਬਦਲੀ (a.ਪੂਛ, b.ਪੂਛ);
      hb_ਅਦਲਾ_ਬਦਲੀ (a.next, b.next);
      hb_ਅਦਲਾ_ਬਦਲੀ (a.real_links, b.real_links);
      hb_ਅਦਲਾ_ਬਦਲੀ (a.virtual_links, b.virtual_links);
    }

    ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ object_t &o) ਅਚਲ
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      ਵਾਪਸ (ਪੂਛ - ਸਿਰ_2 == o.ਪੂਛ - o.ਸਿਰ_2)
	  && (real_links.ਲੰਮਾਈ == o.real_links.ਲੰਮਾਈ)
	  && 0 == hb_memcmp (ਸਿਰ_2, o.ਸਿਰ_2, ਪੂਛ - ਸਿਰ_2)
	  && real_links.as_ਬਾਈਟਾਂ () == o.real_links.as_ਬਾਈਟਾਂ ();
    }
    uint32_t hash () ਅਚਲ
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      ਵਾਪਸ hb_ਬਾਈਟਾਂ_t (ਸਿਰ_2, hb_min (128, ਪੂਛ - ਸਿਰ_2)).hash () ^
          real_links.as_ਬਾਈਟਾਂ ().hash ();
    }

    ਬਣਤਰ link_t
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ: 3;
      ਬਿਨਾਨਿਸ਼ਾਨੀ is_signed: 1;
      ਬਿਨਾਨਿਸ਼ਾਨੀ whence: 2;
      ਬਿਨਾਨਿਸ਼ਾਨੀ bias : 26;
      ਬਿਨਾਨਿਸ਼ਾਨੀ position;
      objidx_t objidx;

      link_t () = ਮੂਲ;

      link_t (ਅਚਲ hb_subset_serialize_link_t &o)
      {
        ਚੌੜਾਈ = o.ਚੌੜਾਈ;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ* a, ਅਚਲ ਖਾਲੀ* b)
      {
        ਪੂਰਨ cmp = ((ਅਚਲ link_t*)a)->position - ((ਅਚਲ link_t*)b)->position;
        ਜੇ (cmp) ਵਾਪਸ cmp;

        ਵਾਪਸ ((ਅਚਲ link_t*)a)->objidx - ((ਅਚਲ link_t*)b)->objidx;
      }
    };

    ਅੱਖਰ *ਸਿਰ_2;
    ਅੱਖਰ *ਪੂਛ;
    hb_ਸਦਿਸ਼_t<link_t> real_links;
    hb_ਸਦਿਸ਼_t<link_t> virtual_links;
    object_t *next;

    ਸਵੈਚਾਲਿਤ all_links () ਅਚਲ HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    ਸਵੈਚਾਲਿਤ all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  ਬਣਤਰ snapshot_t
  {
    ਅੱਖਰ *ਸਿਰ_2;
    ਅੱਖਰ *ਪੂਛ;
    object_t *current; // Just for sanity check
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_real_links;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    ਵਾਪਸ snapshot_t {
      ਸਿਰ_2, ਪੂਛ, current,
      current ? current->real_links.ਲੰਮਾਈ : 0,
      current ? current->virtual_links.ਲੰਮਾਈ : 0,
      errors
     };
  }

  hb_serialize_context_t (ਖਾਲੀ *ਸ਼ੁਰੂ_2, size_t ਮਾਪ) :
    ਸ਼ੁਰੂ ((ਅੱਖਰ *) ਸ਼ੁਰੂ_2),
    ਸਿਰਾ (ਸ਼ੁਰੂ + ਮਾਪ),
    current (ਖਾਲੀ_ਸੂਚਕ)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  ਖਾਲੀ fini ()
  {
    ਲਈ (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    ਇਹ->packed_map.fini ();

    ਜਦੋਂ (current)
    {
      ਸਵੈਚਾਲਿਤ *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ { ਵਾਪਸ ਤਾਰਕਿਕ (errors); }

  ਤਾਰਕਿਕ successful () ਅਚਲ { ਵਾਪਸ !ਤਾਰਕਿਕ (errors); }

  HB_NODISCARD ਤਾਰਕਿਕ ran_ਬਾਹਰਲਾ_ਦਾ_ਗੁੰਜਾਇਸ਼ () ਅਚਲ { ਵਾਪਸ errors & HB_SERIALIZE_ERROR_ਬਾਹਰਲਾ_ਦਾ_ਗੁੰਜਾਇਸ਼; }
  HB_NODISCARD ਤਾਰਕਿਕ ਵਿੱਥ_ਉਛਾਲ () ਅਚਲ { ਵਾਪਸ errors & HB_SERIALIZE_ERROR_ਵਿੱਥ_ਉਛਾਲ; }
  HB_NODISCARD ਤਾਰਕਿਕ only_ਵਿੱਥ_ਉਛਾਲ () ਅਚਲ { ਵਾਪਸ errors == HB_SERIALIZE_ERROR_ਵਿੱਥ_ਉਛਾਲ; }
  HB_NODISCARD ਤਾਰਕਿਕ only_ਉਛਾਲ () ਅਚਲ
  {
    ਵਾਪਸ errors == HB_SERIALIZE_ERROR_ਵਿੱਥ_ਉਛਾਲ
        || errors == HB_SERIALIZE_ERROR_INT_ਉਛਾਲ
        || errors == HB_SERIALIZE_ERROR_ARRAY_ਉਛਾਲ;
  }

  ਖਾਲੀ reset (ਖਾਲੀ *ਸ਼ੁਰੂ_2, size_t ਮਾਪ)
  {
    ਸ਼ੁਰੂ = (ਅੱਖਰ*) ਸ਼ੁਰੂ_2;
    ਸਿਰਾ = ਸ਼ੁਰੂ + ਮਾਪ;
    reset ();
    current = ਖਾਲੀ_ਸੂਚਕ;
  }

  ਖਾਲੀ reset ()
  {
    ਇਹ->errors = HB_SERIALIZE_ERROR_NONE;
    ਇਹ->ਸਿਰ_2 = ਇਹ->ਸ਼ੁਰੂ;
    ਇਹ->ਪੂਛ = ਇਹ->ਸਿਰਾ;
    ਇਹ->zerocopy = ਖਾਲੀ_ਸੂਚਕ;
    ਇਹ->debug_ਡੂੰਘਾਈ = 0;

    fini ();
    ਇਹ->packed.push (ਖਾਲੀ_ਸੂਚਕ);
    ਇਹ->packed_map.init ();
  }

  ਤਾਰਕਿਕ check_success (ਤਾਰਕਿਕ success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    ਵਾਪਸ successful ()
        && (success || err (err_type));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T1, ਕਿਸਮ_ਨਾਂ T2>
  ਤਾਰਕਿਕ check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    ਜੇ ((ਲੰਮਾ ਲੰਮਾ) v1 != (ਲੰਮਾ ਲੰਮਾ) v2)
    {
      ਵਾਪਸ err (err_type);
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T1, ਕਿਸਮ_ਨਾਂ T2>
  ਤਾਰਕਿਕ check_ਸੌਂਪੋ (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { ਵਾਪਸ check_equal (v1 = v2, v2, err_type); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਤਾਰਕਿਕ propagate_error (T &&obj)
  { ਵਾਪਸ check_success (!hb_deref (obj).ਵਿੱਚ_error ()); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T1, ਕਿਸਮ_ਨਾਂ... Ts> ਤਾਰਕਿਕ propagate_error (T1 &&o1, Ts&&... os)
  { ਵਾਪਸ propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type=ਅੱਖਰ>
  __attribute__((returns_nonnull))
  Type *ਸ਼ੁਰੂ_serialize ()
  {
    DEBUG_ਸੁਨੇਹਾ_LEVEL (SERIALIZE, ਇਹ->ਸ਼ੁਰੂ, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     ਇਹ->ਸ਼ੁਰੂ, ਇਹ->ਸਿਰਾ,
		     (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) (ਇਹ->ਸਿਰਾ - ਇਹ->ਸ਼ੁਰੂ));

    ਪ੍ਰਤਿਜ੍ਞਾ (!current);
    ਵਾਪਸ push<Type> ();
  }
  ਖਾਲੀ ਸਿਰਾ_serialize ()
  {
    DEBUG_ਸੁਨੇਹਾ_LEVEL (SERIALIZE, ਇਹ->ਸ਼ੁਰੂ, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     ਇਹ->ਸ਼ੁਰੂ, ਇਹ->ਸਿਰਾ,
		     (ਬਿਨਾਨਿਸ਼ਾਨੀ) (ਇਹ->ਸਿਰ_2 - ਇਹ->ਸ਼ੁਰੂ),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    ਜੇ (unlikely (!current)) ਵਾਪਸ;
    ਜੇ (unlikely (ਵਿੱਚ_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      ਜੇ (ਵਿੱਥ_ਉਛਾਲ ()) err (HB_SERIALIZE_ERROR_OTHER);
      ਵਾਪਸ;
    }

    ਪ੍ਰਤਿਜ੍ਞਾ (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    ਜੇ (packed.ਲੰਮਾਈ <= 1)
      ਵਾਪਸ;

    ਕੱਢੋ_pack (ਝੂਠ);

    resolve_links ();
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type = ਖਾਲੀ>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ ਸ਼ੁਰੂ_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    ਜੇ (unlikely (!obj))
      check_success (ਝੂਠ);
    ਨਹੀਂਤਾਂ
    {
      obj->ਸਿਰ_2 = ਸਿਰ_2;
      obj->ਪੂਛ = ਪੂਛ;
      obj->next = current;
      current = obj;
    }
    ਵਾਪਸ ਸ਼ੁਰੂ_embed<Type> ();
  }
  ਖਾਲੀ ਕੱਢੋ_discard ()
  {
    object_t *obj = current;
    ਜੇ (unlikely (!obj)) ਵਾਪਸ;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    ਜੇ (unlikely (ਵਿੱਚ_error() && !only_ਉਛਾਲ ())) ਵਾਪਸ;

    current = current->next;
    revert (zerocopy ? zerocopy : obj->ਸਿਰ_2, obj->ਪੂਛ);
    zerocopy = ਖਾਲੀ_ਸੂਚਕ;
    obj->fini ();
    object_pool.release (obj);
  }

  /* Set share to false when an object is unlikely shareable with others
   * so not worth an attempt, or a contiguous table is serialized as
   * multiple consecutive objects in the reverse order so can't be shared.
   */
  objidx_t ਕੱਢੋ_pack (ਤਾਰਕਿਕ share=ਸੱਚ)
  {
    object_t *obj = current;
    ਜੇ (unlikely (!obj)) ਵਾਪਸ 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    ਜੇ (unlikely (ਵਿੱਚ_error()  && !only_ਉਛਾਲ ())) ਵਾਪਸ 0;

    current = current->next;
    obj->ਪੂਛ = ਸਿਰ_2;
    obj->next = ਖਾਲੀ_ਸੂਚਕ;
    ਪ੍ਰਤਿਜ੍ਞਾ (obj->ਸਿਰ_2 <= obj->ਪੂਛ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ len = obj->ਪੂਛ - obj->ਸਿਰ_2;
    ਸਿਰ_2 = zerocopy ? zerocopy : obj->ਸਿਰ_2; /* Rewind head. */
    ਤਾਰਕਿਕ was_zerocopy = zerocopy;
    zerocopy = ਖਾਲੀ_ਸੂਚਕ;

    ਜੇ (!len)
    {
      ਪ੍ਰਤਿਜ੍ਞਾ (!obj->real_links.ਲੰਮਾਈ);
      ਪ੍ਰਤਿਜ੍ਞਾ (!obj->virtual_links.ਲੰਮਾਈ);
      ਵਾਪਸ 0;
    }

    objidx_t objidx;
    uint32_t hash = 0;
    ਜੇ (share)
    {
      hash = hb_hash (obj);
      objidx = packed_map.get_with_hash (obj, hash);
      ਜੇ (objidx)
      {
        ਮਿਲਾਓ_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	ਵਾਪਸ objidx;
      }
    }

    ਪੂਛ -= len;
    ਜੇ (was_zerocopy)
      ਪ੍ਰਤਿਜ੍ਞਾ (ਪੂਛ == obj->ਸਿਰ_2);
    ਨਹੀਂਤਾਂ
      memmove (ਪੂਛ, obj->ਸਿਰ_2, len);

    obj->ਸਿਰ_2 = ਪੂਛ;
    obj->ਪੂਛ = ਪੂਛ + len;

    packed.push (obj);

    ਜੇ (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      ਵਾਪਸ 0;
    }

    objidx = packed.ਲੰਮਾਈ - 1;

    ਜੇ (share) packed_map.ਸੈੱਟ_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    ਵਾਪਸ objidx;
  }

  ਖਾਲੀ revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    ਜੇ (unlikely (ਵਿੱਚ_error () && !only_ਉਛਾਲ ())) ਵਾਪਸ;
    ਪ੍ਰਤਿਜ੍ਞਾ (snap.current == current);
    ਜੇ (current)
    {
      current->real_links.shrink (snap.ਅੰਕ_real_links);
      current->virtual_links.shrink (snap.ਅੰਕ_virtual_links);
    }
    errors = snap.errors;
    revert (snap.ਸਿਰ_2, snap.ਪੂਛ);
  }

  ਖਾਲੀ revert (ਅੱਖਰ *snap_ਸਿਰ,
	       ਅੱਖਰ *snap_ਪੂਛ)
  {
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ;
    ਪ੍ਰਤਿਜ੍ਞਾ (snap_ਸਿਰ <= ਸਿਰ_2);
    ਪ੍ਰਤਿਜ੍ਞਾ (ਪੂਛ <= snap_ਪੂਛ);
    ਸਿਰ_2 = snap_ਸਿਰ;
    ਪੂਛ = snap_ਪੂਛ;
    discard_stale_objects ();
  }

  ਖਾਲੀ discard_stale_objects ()
  {
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ;
    ਜਦੋਂ (packed.ਲੰਮਾਈ > 1 &&
	   packed.ਪੂਛ ()->ਸਿਰ_2 < ਪੂਛ)
    {
      object_t *obj = packed.ਪੂਛ ();
      packed_map.del (obj);
      ਪ੍ਰਤਿਜ੍ਞਾ (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.ਕੱਢੋ ();
    }
    ਜੇ (packed.ਲੰਮਾਈ > 1)
      ਪ੍ਰਤਿਜ੍ਞਾ (packed.ਪੂਛ ()->ਸਿਰ_2 == ਪੂਛ);
  }

  // Adds a virtual link from the current object to objidx. A virtual link is not associated with
  // an actual offset field. They are solely used to enforce ordering constraints between objects.
  // Adding a virtual link from object a to object b will ensure that object b is always packed after
  // object a in the final serialized order.
  //
  // This is useful in certain situations where there needs to be a specific ordering in the
  // final serialization. Such as when platform bugs require certain orderings, or to provide
  //  guidance to the repacker for better offset overflow resolution.
  ਖਾਲੀ ਜੋੜੋ_virtual_link (objidx_t objidx)
  {
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ;

    ਜੇ (!objidx)
      ਵਾਪਸ;

    ਪ੍ਰਤਿਜ੍ਞਾ (current);

    ਜੇ (!current->ਜੋੜੋ_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t ਆਖਰੀ_added_ਬੱਚਾ_ਸੂਚਕਾਂਕ() ਅਚਲ {
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ (objidx_t) -1;

    ਪ੍ਰਤਿਜ੍ਞਾ (current);
    ਜੇ (!ਤਾਰਕਿਕ(current->real_links)) {
      ਵਾਪਸ (objidx_t) -1;
    }

    ਵਾਪਸ current->real_links[current->real_links.ਲੰਮਾਈ - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  ਖਾਲੀ repack_ਆਖਰੀ(objidx_t objidx) {
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ;

    ਜੇ (!objidx)
      ਵਾਪਸ;

    ਪ੍ਰਤਿਜ੍ਞਾ (current);
    ਲਈ (ਸਵੈਚਾਲਿਤ& l : current->real_links) {
      ਜੇ (l.objidx == objidx) {
        ਜਾਰੀ;
      }

      packed[l.objidx]->ਜੋੜੋ_virtual_link(objidx);
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਖਾਲੀ ਜੋੜੋ_link (T &ofs, objidx_t objidx,
		 whence_t whence = ਸਿਰ,
		 ਬਿਨਾਨਿਸ਼ਾਨੀ bias = 0)
  {
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ;

    ਜੇ (!objidx)
      ਵਾਪਸ;

    ਪ੍ਰਤਿਜ੍ਞਾ (current);
    ਪ੍ਰਤਿਜ੍ਞਾ (current->ਸਿਰ_2 <= (ਅਚਲ ਅੱਖਰ *) &ofs);

    ਸਵੈਚਾਲਿਤ& link = *current->real_links.push ();
    ਜੇ (current->real_links.ਵਿੱਚ_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.ਚੌੜਾਈ = ਆਕਾਰ (T);
    link.objidx = objidx;
    ਜੇ (unlikely (!ਆਕਾਰ (T)))
    {
      // This link is not associated with an actual offset and exists merely to enforce
      // an ordering constraint.
      link.is_signed = 0;
      link.whence = 0;
      link.position = 0;
      link.bias = 0;
      ਵਾਪਸ;
    }

    link.is_signed = std::is_signed<hb_unwrap_type (T)>::value;
    link.whence = (ਬਿਨਾਨਿਸ਼ਾਨੀ) whence;
    link.position = (ਅਚਲ ਅੱਖਰ *) &ofs - current->ਸਿਰ_2;
    link.bias = bias;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੰਜ਼ਿਲ_bias (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ 0;
    ਜੇ (!ਅਧਾਰ) ਵਾਪਸ 0;
    ਪ੍ਰਤਿਜ੍ਞਾ (current);
    ਪ੍ਰਤਿਜ੍ਞਾ (current->ਸਿਰ_2 <= (ਅਚਲ ਅੱਖਰ *) ਅਧਾਰ);
    ਵਾਪਸ (ਅਚਲ ਅੱਖਰ *) ਅਧਾਰ - current->ਸਿਰ_2;
  }

  ਖਾਲੀ resolve_links ()
  {
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ;

    ਪ੍ਰਤਿਜ੍ਞਾ (!current);
    ਪ੍ਰਤਿਜ੍ਞਾ (packed.ਲੰਮਾਈ > 1);

    ਲਈ (ਅਚਲ object_t* ਮਾਪਾ : ++hb_iter (packed))
      ਲਈ (ਅਚਲ object_t::link_t &link : ਮਾਪਾ->real_links)
      {
	ਅਚਲ object_t* ਬੱਚਾ = packed[link.objidx];
	ਜੇ (unlikely (!ਬੱਚਾ)) { err (HB_SERIALIZE_ERROR_OTHER); ਵਾਪਸ; }
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ = 0;
	ਚੋਣ ((whence_t) link.whence) {
	ਹਾਲਤ ਸਿਰ:     ਵਿੱਥ = ਬੱਚਾ->ਸਿਰ_2 - ਮਾਪਾ->ਸਿਰ_2; ਰੋਕੋ;
	ਹਾਲਤ ਪੂਛ_2:     ਵਿੱਥ = ਬੱਚਾ->ਸਿਰ_2 - ਮਾਪਾ->ਪੂਛ; ਰੋਕੋ;
	ਹਾਲਤ Absolute: ਵਿੱਥ = (ਸਿਰ_2 - ਸ਼ੁਰੂ) + (ਬੱਚਾ->ਸਿਰ_2 - ਪੂਛ); ਰੋਕੋ;
	}

	ਪ੍ਰਤਿਜ੍ਞਾ (ਵਿੱਥ >= link.bias);
	ਵਿੱਥ -= link.bias;
	ਜੇ (link.is_signed)
	{
	  ਪ੍ਰਤਿਜ੍ਞਾ (link.ਚੌੜਾਈ == 2 || link.ਚੌੜਾਈ == 4);
	  ਜੇ (link.ਚੌੜਾਈ == 4)
	    ਸੌਂਪੋ_ਵਿੱਥ<int32_t> (ਮਾਪਾ, link, ਵਿੱਥ);
	  ਨਹੀਂਤਾਂ
	    ਸੌਂਪੋ_ਵਿੱਥ<int16_t> (ਮਾਪਾ, link, ਵਿੱਥ);
	}
	ਨਹੀਂਤਾਂ
	{
	  ਪ੍ਰਤਿਜ੍ਞਾ (link.ਚੌੜਾਈ == 2 || link.ਚੌੜਾਈ == 3 || link.ਚੌੜਾਈ == 4);
	  ਜੇ (link.ਚੌੜਾਈ == 4)
	    ਸੌਂਪੋ_ਵਿੱਥ<uint32_t> (ਮਾਪਾ, link, ਵਿੱਥ);
	  ਨਹੀਂਤਾਂ ਜੇ (link.ਚੌੜਾਈ == 3)
	    ਸੌਂਪੋ_ਵਿੱਥ<uint32_t, 3> (ਮਾਪਾ, link, ਵਿੱਥ);
	  ਨਹੀਂਤਾਂ
	    ਸੌਂਪੋ_ਵਿੱਥ<uint16_t> (ਮਾਪਾ, link, ਵਿੱਥ);
	}
      }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ () ਅਚਲ
  {
    ਜੇ (unlikely (!current)) ਵਾਪਸ 0;
    ਵਾਪਸ ਇਹ->ਸਿਰ_2 - current->ਸਿਰ_2;
  }

  ਖਾਲੀ align (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ alignment)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ l = ਲੰਮਾਈ () % alignment;
    ਜੇ (l)
      (ਖਾਲੀ) allocate_ਮਾਪ<ਖਾਲੀ> (alignment - l);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type = ਖਾਲੀ>
  __attribute__((returns_nonnull))
  Type *ਸ਼ੁਰੂ_embed (ਅਚਲ Type *obj HB_ਅਣਵਰਤਿਆ = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  { ਵਾਪਸ reinterpret_cast<Type *> (ਇਹ->ਸਿਰ_2); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  __attribute__((returns_nonnull))
  Type *ਸ਼ੁਰੂ_embed (ਅਚਲ Type &obj) ਅਚਲ
  { ਵਾਪਸ ਸ਼ੁਰੂ_embed (std::addressof (obj)); }

  ਤਾਰਕਿਕ err (hb_serialize_error_t err_type)
  {
    ਵਾਪਸ !ਤਾਰਕਿਕ ((errors = (errors | err_type)));
  }

  ਤਾਰਕਿਕ ਸ਼ੁਰੂ_zerocopy (size_t ਮਾਪ)
  {
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;

    ਜੇ (unlikely (ਮਾਪ > ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ || ਇਹ->ਪੂਛ - ਇਹ->ਸਿਰ_2 < ptrdiff_t (ਮਾਪ)))
    {
      err (HB_SERIALIZE_ERROR_ਬਾਹਰਲਾ_ਦਾ_ਗੁੰਜਾਇਸ਼);
      ਵਾਪਸ ਝੂਠ;
    }

    ਪ੍ਰਤਿਜ੍ਞਾ (!ਇਹ->zerocopy);
    ਇਹ->zerocopy = ਇਹ->ਸਿਰ_2;

    ਪ੍ਰਤਿਜ੍ਞਾ (ਇਹ->current->ਸਿਰ_2 == ਇਹ->ਸਿਰ_2);
    ਇਹ->current->ਸਿਰ_2 = ਇਹ->current->ਪੂਛ = ਇਹ->ਸਿਰ_2 = ਇਹ->ਪੂਛ - ਮਾਪ;
    ਵਾਪਸ ਸੱਚ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  HB_NODISCARD
  Type *allocate_ਮਾਪ (size_t ਮਾਪ, ਤਾਰਕਿਕ clear = ਸੱਚ)
  {
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    ਜੇ (unlikely (ਮਾਪ > ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ || ਇਹ->ਪੂਛ - ਇਹ->ਸਿਰ_2 < ptrdiff_t (ਮਾਪ)))
    {
      err (HB_SERIALIZE_ERROR_ਬਾਹਰਲਾ_ਦਾ_ਗੁੰਜਾਇਸ਼);
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
    ਜੇ (clear)
      hb_memset (ਇਹ->ਸਿਰ_2, 0, ਮਾਪ);
    ਅੱਖਰ *ret = ਇਹ->ਸਿਰ_2;
    ਇਹ->ਸਿਰ_2 += ਮਾਪ;
    ਵਾਪਸ reinterpret_cast<Type *> (ret);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  Type *allocate_min ()
  { ਵਾਪਸ ਇਹ->allocate_ਮਾਪ<Type> (Type::min_ਮਾਪ); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  HB_NODISCARD
  Type *embed (ਅਚਲ Type *obj)
  {
    size_t ਮਾਪ = obj->get_ਮਾਪ ();
    Type *ret = ਇਹ->allocate_ਮਾਪ<Type> (ਮਾਪ, ਝੂਠ);
    ਜੇ (unlikely (!ret)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    hb_memcpy (ret, obj, ਮਾਪ);
    ਵਾਪਸ ret;
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  HB_NODISCARD
  Type *embed (ਅਚਲ Type &obj)
  { ਵਾਪਸ embed (std::addressof (obj)); }
  ਅੱਖਰ *embed (ਅਚਲ ਅੱਖਰ *obj, ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ)
  {
    ਅੱਖਰ *ret = ਇਹ->allocate_ਮਾਪ<ਅੱਖਰ> (ਮਾਪ, ਝੂਠ);
    ਜੇ (unlikely (!ret)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    hb_memcpy (ret, obj, ਮਾਪ);
    ਵਾਪਸ ret;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  _copy (ਅਚਲ Type &src, hb_ਤਰਜੀਹ<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (ਇਹ, std::forward<Ts> (ds)...))

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type> ਸਵੈਚਾਲਿਤ
  _copy (ਅਚਲ Type &src, hb_ਤਰਜੀਹ<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = ਇਹ->allocate_ਮਾਪ<Type> (ਆਕਾਰ (Type));
    ਜੇ (unlikely (!ret)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    *ret = src;
    ਵਾਪਸ ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ...Ts>
  Type *copy (ਅਚਲ Type &src, Ts&&... ds)
  { ਵਾਪਸ _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ...Ts>
  Type *copy (ਅਚਲ Type *src, Ts&&... ds)
  { ਵਾਪਸ copy (*src, std::forward<Ts> (ds)...); }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   ਕਿਸਮ_ਨਾਂ ...Ts>
  ਖਾਲੀ copy_all (Iterator it, Ts&&... ds)
  { ਲਈ (decltype (*it) _ : it) copy (_, ds...); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  hb_serialize_context_t& ਸੰਚਾਲਕ << (ਅਚਲ Type &obj) & { embed (obj); ਵਾਪਸ *ਇਹ; }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  Type *extend_ਮਾਪ (Type *obj, size_t ਮਾਪ, ਤਾਰਕਿਕ clear = ਸੱਚ)
  {
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (unlikely (ਮਾਪ >= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)) { err (HB_SERIALIZE_ERROR_OTHER); ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ; }

    ਪ੍ਰਤਿਜ੍ਞਾ (ਇਹ->ਸ਼ੁਰੂ <= (ਅੱਖਰ *) obj);
    ਪ੍ਰਤਿਜ੍ਞਾ ((ਅੱਖਰ *) obj <= ਇਹ->ਸਿਰ_2);
    ਪ੍ਰਤਿਜ੍ਞਾ ((size_t) (ਇਹ->ਸਿਰ_2 - (ਅੱਖਰ *) obj) <= ਮਾਪ);
    ਜੇ (unlikely (((ਅੱਖਰ *) obj + ਮਾਪ < (ਅੱਖਰ *) obj) ||
		  !ਇਹ->allocate_ਮਾਪ<Type> (((ਅੱਖਰ *) obj) + ਮਾਪ - ਇਹ->ਸਿਰ_2, clear))) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    ਵਾਪਸ reinterpret_cast<Type *> (obj);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  Type *extend_ਮਾਪ (Type &obj, size_t ਮਾਪ, ਤਾਰਕਿਕ clear = ਸੱਚ)
  { ਵਾਪਸ extend_ਮਾਪ (std::addressof (obj), ਮਾਪ, clear); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  Type *extend_min (Type *obj) { ਵਾਪਸ extend_ਮਾਪ (obj, obj->min_ਮਾਪ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  Type *extend_min (Type &obj) { ਵਾਪਸ extend_min (std::addressof (obj)); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { ਵਾਪਸ extend_ਮਾਪ (obj, obj->get_ਮਾਪ (std::forward<Ts> (ds)...)); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { ਵਾਪਸ extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_ਬਾਈਟਾਂ_t copy_ਬਾਈਟਾਂ () ਅਚਲ
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (successful ());
    /* Copy both items from head side and tail side... */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len = (ਇਹ->ਸਿਰ_2 - ਇਹ->ਸ਼ੁਰੂ)
		     + (ਇਹ->ਸਿਰਾ  - ਇਹ->ਪੂਛ);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    ਜੇ (!len) ਵਾਪਸ hb_ਬਾਈਟਾਂ_t ();

    ਅੱਖਰ *p = (ਅੱਖਰ *) hb_malloc (len);
    ਜੇ (unlikely (!p)) ਵਾਪਸ hb_ਬਾਈਟਾਂ_t ();

    hb_memcpy (p, ਇਹ->ਸ਼ੁਰੂ, ਇਹ->ਸਿਰ_2 - ਇਹ->ਸ਼ੁਰੂ);
    hb_memcpy (p + (ਇਹ->ਸਿਰ_2 - ਇਹ->ਸ਼ੁਰੂ), ਇਹ->ਪੂਛ, ਇਹ->ਸਿਰਾ - ਇਹ->ਪੂਛ);
    ਵਾਪਸ hb_ਬਾਈਟਾਂ_t (p, len);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  Type *copy () ਅਚਲ
  { ਵਾਪਸ reinterpret_cast<Type *> ((ਅੱਖਰ *) copy_ਬਾਈਟਾਂ ().arrayZ); }
  hb_blob_t *copy_blob () ਅਚਲ
  {
    hb_ਬਾਈਟਾਂ_t b = copy_ਬਾਈਟਾਂ ();
    ਵਾਪਸ hb_blob_create (b.arrayZ, b.ਲੰਮਾਈ,
			   HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE,
			   (ਅੱਖਰ *) b.arrayZ, hb_free);
  }

  ਅਚਲ hb_ਸਦਿਸ਼_t<object_t *>& object_ਗ੍ਰਾਫ() ਅਚਲ
  { ਵਾਪਸ packed; }

  ਨਿਜੀ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ_2 = ਆਕਾਰ (T)>
  ਖਾਲੀ ਸੌਂਪੋ_ਵਿੱਥ (ਅਚਲ object_t* ਮਾਪਾ, ਅਚਲ object_t::link_t &link, ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ)
  {
    // XXX We should stop assuming big-endian!
    ਸਵੈਚਾਲਿਤ &off = * ((HBInt<ਸੱਚ, T, ਮਾਪ_2> *) (ਮਾਪਾ->ਸਿਰ_2 + link.position));
    ਪ੍ਰਤਿਜ੍ਞਾ (0 == off);
    check_ਸੌਂਪੋ (off, ਵਿੱਥ, HB_SERIALIZE_ERROR_ਵਿੱਥ_ਉਛਾਲ);
  }

  ਜਨਤਕ:
  ਅੱਖਰ *ਸ਼ੁਰੂ, *ਸਿਰ_2, *ਪੂਛ, *ਸਿਰਾ, *zerocopy;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ debug_ਡੂੰਘਾਈ;
  hb_serialize_error_t errors;

  ਨਿਜੀ:

  ਖਾਲੀ ਮਿਲਾਓ_virtual_links (ਅਚਲ object_t* from, objidx_t ਮੰਜ਼ਿਲ_idx) {
    object_t* to = packed[ਮੰਜ਼ਿਲ_idx];
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& l : from->virtual_links) {
      to->virtual_links.push (l);
    }
  }

  /* Object memory pool. */
  hb_free_pool_t<object_t> object_pool;

  /* Stack of currently under construction objects. */
  object_t *current;

  /* Stack of packed objects.  Object 0 is always nil object. */
  hb_ਸਦਿਸ਼_t<object_t *> packed;

  /* Map view of packed objects. */
  hb_hashmap_t<ਅਚਲ object_t *, objidx_t> packed_map;
};

#ਜੇ_ਅੰਤ /* HB_SERIALIZE_HH */
