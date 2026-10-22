/*
 * Copyright © 2022  Google, Inc.
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
 * Google Author(s): Garret Rieger
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਗ੍ਰਾਫ_SERIALIZE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗ੍ਰਾਫ_SERIALIZE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "graph-result.hh"

ਨਾਮਾਕਾਸ਼ ਗ੍ਰਾਫ {

ਬਣਤਰ ਉਛਾਲ_ਰਿਕਾਰਡ_t
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ;

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ ਉਛਾਲ_ਰਿਕਾਰਡ_t o) ਅਚਲ
  { ਵਾਪਸ !(*ਇਹ == o); }

  ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ ਉਛਾਲ_ਰਿਕਾਰਡ_t& o) ਅਚਲ
  {
    ਵਾਪਸ ਮਾਪਾ == o.ਮਾਪਾ &&
        ਬੱਚਾ == o.ਬੱਚਾ;
  }

  ਸਤਰਅੰਦਰ uint32_t hash () ਅਚਲ
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (ਮਾਪਾ);
    current = current * 31 + hb_hash (ਬੱਚਾ);
    ਵਾਪਸ current;
  }
};

ਸਤਰਅੰਦਰ
int64_t compute_ਵਿੱਥ (
    ਅਚਲ ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ,
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ_idx,
    ਅਚਲ hb_serialize_context_t::object_t::link_t& link)
{
  ਅਚਲ ਸਵੈਚਾਲਿਤ& ਮਾਪਾ = ਗ੍ਰਾਫ.vertices_[ਮਾਪਾ_idx];
  ਅਚਲ ਸਵੈਚਾਲਿਤ& ਬੱਚਾ = ਗ੍ਰਾਫ.vertices_[link.objidx];
  int64_t ਵਿੱਥ = 0;
  ਚੋਣ ((hb_serialize_context_t::whence_t) link.whence) {
    ਹਾਲਤ hb_serialize_context_t::whence_t::ਸਿਰ:
      ਵਿੱਥ = ਬੱਚਾ.ਸ਼ੁਰੂ - ਮਾਪਾ.ਸ਼ੁਰੂ; ਰੋਕੋ;
    ਹਾਲਤ hb_serialize_context_t::whence_t::ਪੂਛ_2:
      ਵਿੱਥ = ਬੱਚਾ.ਸ਼ੁਰੂ - ਮਾਪਾ.ਸਿਰਾ; ਰੋਕੋ;
    ਹਾਲਤ hb_serialize_context_t::whence_t::Absolute:
      ਵਿੱਥ = ਬੱਚਾ.ਸ਼ੁਰੂ; ਰੋਕੋ;
  }

  ਪ੍ਰਤਿਜ੍ਞਾ (ਵਿੱਥ >= link.bias);
  ਵਿੱਥ -= link.bias;
  ਵਾਪਸ ਵਿੱਥ;
}

ਸਤਰਅੰਦਰ
ਤਾਰਕਿਕ is_valid_ਵਿੱਥ (int64_t ਵਿੱਥ,
                      ਅਚਲ hb_serialize_context_t::object_t::link_t& link)
{
  ਜੇ (unlikely (!link.ਚੌੜਾਈ))
    // Virtual links can't overflow.
    ਵਾਪਸ link.is_signed || ਵਿੱਥ >= 0;

  ਜੇ (link.is_signed)
  {
    ਜੇ (link.ਚੌੜਾਈ == 4)
      ਵਾਪਸ ਵਿੱਥ >= -((int64_t) 1 << 31) && ਵਿੱਥ < ((int64_t) 1 << 31);
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ ਵਿੱਥ >= -(1 << 15) && ਵਿੱਥ < (1 << 15);
  }
  ਨਹੀਂਤਾਂ
  {
    ਜੇ (link.ਚੌੜਾਈ == 4)
      ਵਾਪਸ ਵਿੱਥ >= 0 && ਵਿੱਥ < ((int64_t) 1 << 32);
    ਨਹੀਂਤਾਂ ਜੇ (link.ਚੌੜਾਈ == 3)
      ਵਾਪਸ ਵਿੱਥ >= 0 && ਵਿੱਥ < ((int32_t) 1 << 24);
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ ਵਿੱਥ >= 0 && ਵਿੱਥ < (1 << 16);
  }
}

/*
 * Will any offsets overflow on graph when it's serialized?
 */
ਸਤਰਅੰਦਰ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਤਾਰਕਿਕ>
will_ਉਛਾਲ (ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ,
               hb_ਸਦਿਸ਼_t<ਉਛਾਲ_ਰਿਕਾਰਡ_t>* overflows = ਖਾਲੀ_ਸੂਚਕ)
{
  ਜੇ (overflows) overflows->clear ();
  ਗ੍ਰਾਫ.update_positions ();

  hb_hashmap_t<ਉਛਾਲ_ਰਿਕਾਰਡ_t, ਤਾਰਕਿਕ> ਰਿਕਾਰਡ_ਸੈੱਟ;
  ਅਚਲ ਸਵੈਚਾਲਿਤ& vertices = ਗ੍ਰਾਫ.vertices_;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ_idx : ਗ੍ਰਾਫ.ordering_)
  {
    // Don't need to check virtual links for overflow
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& link : vertices.arrayZ[ਮਾਪਾ_idx].obj ().real_links)
    {
      int64_t ਵਿੱਥ = compute_ਵਿੱਥ (ਗ੍ਰਾਫ, ਮਾਪਾ_idx, link);
      ਜੇ (likely (is_valid_ਵਿੱਥ (ਵਿੱਥ, link)))
        ਜਾਰੀ;

      ਜੇ (!overflows) ਵਾਪਸ Ok(ਸੱਚ);

      ਉਛਾਲ_ਰਿਕਾਰਡ_t r;
      r.ਮਾਪਾ = ਮਾਪਾ_idx;
      r.ਬੱਚਾ = link.objidx;
      ਜੇ (ਰਿਕਾਰਡ_ਸੈੱਟ.has(r)) ਜਾਰੀ; // don't keep duplicate overflows.

      overflows->push (r);
      ਰਿਕਾਰਡ_ਸੈੱਟ.ਸੈੱਟ (r, ਸੱਚ);
    }
  }

  TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਰਿਕਾਰਡ_ਸੈੱਟ, ਵੰਡ_FAILURE));
  ਜੇ (overflows)
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (*overflows, ਵੰਡ_FAILURE));

  ਵਾਪਸ Ok((ਤਾਰਕਿਕ) (overflows && overflows->ਲੰਮਾਈ));
}

ਸਤਰਅੰਦਰ
ਖਾਲੀ print_overflows (ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ,
                      ਅਚਲ hb_ਸਦਿਸ਼_t<ਉਛਾਲ_ਰਿਕਾਰਡ_t>& overflows)
{
  ਜੇ (!DEBUG_ENABLED(SUBSET_REPACK)) ਵਾਪਸ;

  ਸਵੈਚਾਲਿਤ r = ਗ੍ਰਾਫ.update_parents ();
  ਜੇ (r.is_err()) {
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "Unable to print overflows due to error updating parent links: %s",
      to_string(r.error()));
    ਵਾਪਸ;
  }

  ਪੂਰਨ ਹੱਦਬੰਦੀ = 10;
  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& o : overflows)
  {
    ਜੇ (!ਹੱਦਬੰਦੀ--) ਰੋਕੋ;
    ਅਚਲ ਸਵੈਚਾਲਿਤ& ਮਾਪਾ = ਗ੍ਰਾਫ.vertices_[o.ਮਾਪਾ];
    ਅਚਲ ਸਵੈਚਾਲਿਤ& ਬੱਚਾ = ਗ੍ਰਾਫ.vertices_[o.ਬੱਚਾ];
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
               "  overflow from "
               "%4u (%4u in, %4u out, space %2u) => "
               "%4u (%4u in, %4u out, space %2u)",
               o.ਮਾਪਾ,
               ਮਾਪਾ.incoming_ਕਿਨਾਰੇ_2 (),
               ਮਾਪਾ.obj ().real_links.ਲੰਮਾਈ + ਮਾਪਾ.obj ().virtual_links.ਲੰਮਾਈ,
               ਗ੍ਰਾਫ.space_for (o.ਮਾਪਾ),
               o.ਬੱਚਾ,
               ਬੱਚਾ.incoming_ਕਿਨਾਰੇ_2 (),
               ਬੱਚਾ.obj ().real_links.ਲੰਮਾਈ + ਬੱਚਾ.obj ().virtual_links.ਲੰਮਾਈ,
               ਗ੍ਰਾਫ.space_for (o.ਬੱਚਾ));
  }
  ਜੇ (overflows.ਲੰਮਾਈ > 10) {
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "  ... plus %u more overflows.", overflows.ਲੰਮਾਈ - 10);
  }
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ O> ਸਤਰਅੰਦਰ ਖਾਲੀ
serialize_link_ਦਾ_type (ਅਚਲ hb_serialize_context_t::object_t::link_t& link,
                        ਅੱਖਰ* ਸਿਰ_2,
                        ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ,
                        ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>& id_map,
                        hb_serialize_context_t* c)
{
  // A link must never write outside the bounds of its parent object.
  ਜੇ (unlikely (link.position + link.ਚੌੜਾਈ > ਮਾਪ))
  {
    c->err (HB_SERIALIZE_ERROR_OTHER);
    ਵਾਪਸ;
  }

  OT::ਵਿੱਥ_2<O>* ਵਿੱਥ = reinterpret_cast<OT::ਵਿੱਥ_2<O>*> (ਸਿਰ_2 + link.position);
  *ਵਿੱਥ = 0;
  c->ਜੋੜੋ_link (*ਵਿੱਥ,
               id_map[link.objidx],
               (hb_serialize_context_t::whence_t) link.whence,
               link.bias);
}

ਸਤਰਅੰਦਰ
ਖਾਲੀ serialize_link (ਅਚਲ hb_serialize_context_t::object_t::link_t& link,
                     ਅੱਖਰ* ਸਿਰ_2,
                     ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ,
                     ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>& id_map,
                     hb_serialize_context_t* c)
{
  ਚੋਣ (link.ਚੌੜਾਈ)
  {
    ਹਾਲਤ 0:
      // Virtual links aren't serialized.
      ਵਾਪਸ;
    ਹਾਲਤ 4:
      ਜੇ (link.is_signed)
      {
        serialize_link_ਦਾ_type<OT::HBINT32> (link, ਸਿਰ_2, ਮਾਪ, id_map, c);
      } ਨਹੀਂਤਾਂ {
        serialize_link_ਦਾ_type<OT::HBUINT32> (link, ਸਿਰ_2, ਮਾਪ, id_map, c);
      }
      ਵਾਪਸ;
    ਹਾਲਤ 2:
      ਜੇ (link.is_signed)
      {
        serialize_link_ਦਾ_type<OT::HBINT16> (link, ਸਿਰ_2, ਮਾਪ, id_map, c);
      } ਨਹੀਂਤਾਂ {
        serialize_link_ਦਾ_type<OT::HBUINT16> (link, ਸਿਰ_2, ਮਾਪ, id_map, c);
      }
      ਵਾਪਸ;
    ਹਾਲਤ 3:
      serialize_link_ਦਾ_type<OT::HBUINT24> (link, ਸਿਰ_2, ਮਾਪ, id_map, c);
      ਵਾਪਸ;
    ਮੂਲ:
      // Unexpected link width.
      ਪ੍ਰਤਿਜ੍ਞਾ (0);
  }
}

/*
 * serialize graph into the provided serialization buffer.
 */
ਸਤਰਅੰਦਰ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_blob_t*> serialize (ਅਚਲ ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ)
{
  hb_ਸਦਿਸ਼_t<ਅੱਖਰ> ਬਫਰ;
  size_t ਮਾਪ = ਗ੍ਰਾਫ.ਕੁੱਲ_ਮਾਪ_ਵਿੱਚ_ਬਾਈਟਾਂ ();

  ਜੇ (!ਮਾਪ) ਵਾਪਸ Ok(hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ());

  ਜੇ (unlikely (!ਬਫਰ.alloc (ਮਾਪ))) {
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "Unable to allocate output buffer.");
    ਵਾਪਸ Err(ਵੰਡ_FAILURE);
  }
  hb_serialize_context_t c((ਖਾਲੀ *) ਬਫਰ, ਮਾਪ);

  c.ਸ਼ੁਰੂ_serialize<ਖਾਲੀ> ();
  ਅਚਲ ਸਵੈਚਾਲਿਤ& vertices = ਗ੍ਰਾਫ.vertices_;

  // Objects are placed in the serializer in reverse order since children need
  // to be inserted before their parents.

  // Maps from our obj id's to the id's used during this serialization.
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> id_map;
  ਜੇ (unlikely (!id_map.resize(ਗ੍ਰਾਫ.ordering_.ਲੰਮਾਈ))) {
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "Unable to allocate id_map buffer.");
    ਵਾਪਸ Err(ਵੰਡ_FAILURE);
  }

  ਲਈ (ਪੂਰਨ pos = ਗ੍ਰਾਫ.ordering_.ਲੰਮਾਈ - 1; pos >= 0; pos--) {
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਗ੍ਰਾਫ.ordering_[pos];
    c.push ();

    ਸਵੈਚਾਲਿਤ& v = vertices[i];

    size_t ਮਾਪ = v.ਸਾਰਣੀ_ਮਾਪ ();

    ਅੱਖਰ* ਸ਼ੁਰੂ = c.allocate_ਮਾਪ <ਅੱਖਰ> (ਮਾਪ);
    ਜੇ (unlikely (!ਸ਼ੁਰੂ)) {
      DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "Buffer out of space.");
      ਵਾਪਸ Err(ਵੰਡ_FAILURE);
    }

    hb_memcpy (ਸ਼ੁਰੂ, v.obj ().ਸਿਰ_2, ਮਾਪ);

    // Only real links needs to be serialized.
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& link : v.obj ().real_links)
      serialize_link (link, ਸ਼ੁਰੂ, ਮਾਪ, id_map, &c);

    // All duplications are already encoded in the graph, so don't
    // enable sharing during packing.
    id_map[i] = c.ਕੱਢੋ_pack (ਝੂਠ);
  }
  c.ਸਿਰਾ_serialize ();

  ਜੇ (unlikely (c.ਵਿੱਚ_error ())) {
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "Error during serialization. Err flag: %d",
               c.errors);
    ਜੇ (c.errors & HB_SERIALIZE_ERROR_ਵਿੱਥ_ਉਛਾਲ)
      ਵਾਪਸ Err(ਉਛਾਲ_RESOLUTION_FAILED);
    ਵਾਪਸ Err(ਵੰਡ_FAILURE);
  }

  ਸਵੈਚਾਲਿਤ* blob = c.copy_blob ();
  ਜੇ (unlikely (!blob)) ਵਾਪਸ Err(ਵੰਡ_FAILURE);
  ਵਾਪਸ Ok(blob);
}

} // namespace graph

#ਜੇ_ਅੰਤ // GRAPH_SERIALIZE_HH
