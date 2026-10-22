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

#ਅਨ੍ਤਰ੍ਭਾਵ "../hb-set.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../hb-priority-queue.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../hb-serialize.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "graph-result.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਗ੍ਰਾਫ_ਗ੍ਰਾਫ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗ੍ਰਾਫ_ਗ੍ਰਾਫ_HH

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਗ੍ਰਾਫ_INVALID ((ਬਿਨਾਨਿਸ਼ਾਨੀ) -1)

ਨਾਮਾਕਾਸ਼ ਗ੍ਰਾਫ {

/**
 * Represents a serialized table in the form of a graph.
 * Provides methods for modifying and reordering the graph.
 */
ਬਣਤਰ ਗ੍ਰਾਫ_t
{
  // TODO(garretrieger): experiment with using scratch vector for visited sets instead of allocating
  // a new hb_set_t each time.

  ਬਣਤਰ ਸ਼ੀਰਸ਼_t
  {
    // TODO(garretrieger): have vertex_t gaurantee that head/tail are non null and head < tail
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): have vertex_t gaurantee that all link indexes are valid (ie < vertices_.length)
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): track where the real links are currently sorted, and automatically sort
    // prior to accessing iterator for real links. Avoids needing to manually trigger sorting by
    // downstream users.

    int64_t distance = 0 ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ space = 0 ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਤਰਜੀਹ = 0;

    ਨਿਜੀ:
    hb_serialize_context_t::object_t obj_;
    ਬਿਨਾਨਿਸ਼ਾਨੀ incoming_ਕਿਨਾਰੇ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ single_ਮਾਪਾ = HB_ਗ੍ਰਾਫ_INVALID;
    ਤਾਰਕਿਕ has_incoming_virtual_ਕਿਨਾਰੇ = ਝੂਠ;
    hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> parents;
    hb_ਸੈੱਟ_t virtual_parents;
    ਜਨਤਕ:

    ਸ਼ੀਰਸ਼_t ()
    {}

    ਸਪਸ਼ਟ ਸ਼ੀਰਸ਼_t(ਅਚਲ hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    ਸਵੈਚਾਲਿਤ parents_iter () ਅਚਲ HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_ਮਾਪਾ, single_ਮਾਪਾ != HB_ਗ੍ਰਾਫ_INVALID),
	parents.ਕੁੰਜੀਆਂ_ref ()
      )
    )

    ਖਾਲੀ ਸੈੱਟ_ਬਫਰ (ਅੱਖਰ* ਸਿਰ_2, ਅੱਖਰ* ਪੂਛ) {
      obj_.ਸਿਰ_2 = ਸਿਰ_2;
      obj_.ਪੂਛ = ਪੂਛ;
    }

    ਖਾਲੀ shrink_ਬਫਰ (size_t ਗਿਣਤੀ_2)
    {
      obj_.ਪੂਛ -= ਗਿਣਤੀ_2;
    }

    ਸਵੈਚਾਲਿਤ all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    ਸਵੈਚਾਲਿਤ real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    ਸਵੈਚਾਲਿਤ virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    ਖਾਲੀ ਛਾਂਟੋ_real_links () {
      obj_.real_links.qsort ();
    }

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਜੋੜੋ_real_link (ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ, ਬਿਨਾਨਿਸ਼ਾਨੀ obj_idx, ਬਿਨਾਨਿਸ਼ਾਨੀ position) {
      ਸਵੈਚਾਲਿਤ* link = obj_.real_links.push ();
      TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (obj_.real_links, ਵੰਡ_FAILURE));
      link->ਚੌੜਾਈ = ਚੌੜਾਈ;
      link->objidx = obj_idx;
      link->position = position;
      ਵਾਪਸ Ok();
    }

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਜੋੜੋ_virtual_link (ਬਿਨਾਨਿਸ਼ਾਨੀ obj_idx) {
      ਸਵੈਚਾਲਿਤ* link = obj_.virtual_links.push ();
      TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (obj_.virtual_links, ਵੰਡ_FAILURE));
      link->objidx = obj_idx;
      ਵਾਪਸ Ok();
    }

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਜੋੜੋ_real_link (ਅਚਲ hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      ਵਾਪਸ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (obj_.real_links, ਵੰਡ_FAILURE);
    }

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਸੈੱਟ_real_links (hb_ਸਦਿਸ਼_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (links, ਵੰਡ_FAILURE));
      obj_.real_links = links;
      ਵਾਪਸ Ok();
    }

    ਖਾਲੀ clear_real_links ()
    {
      obj_.real_links.reset();
    }

    ਖਾਲੀ clear_virtual_links ()
    {
      obj_.virtual_links.reset();
    }

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਜੋੜੋ_virtual_link (ਅਚਲ hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      ਵਾਪਸ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (obj_.virtual_links, ਵੰਡ_FAILURE);
    }

    ਅਚਲ hb_serialize_context_t::object_t& obj() ਅਚਲ { ਵਾਪਸ obj_; }

    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
    T as () ਅਚਲ
    {
      ਵਾਪਸ reinterpret_cast<T> (obj_.ਸਿਰ_2);
    }

    ਤਾਰਕਿਕ has_incoming_virtual_ਕਿਨਾਰੇ_2 () ਅਚਲ
    {
      ਵਾਪਸ has_incoming_virtual_ਕਿਨਾਰੇ;
    }

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> link_positions_valid (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_objects, ਤਾਰਕਿਕ removed_nil)
    {
      hb_ਸੈੱਟ_t assigned_ਬਾਈਟਾਂ;
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& l : obj_.real_links)
      {
        ਜੇ (unlikely (l.objidx >= ਅੰਕ_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
                     "Invalid graph. Invalid object index.");
          ਵਾਪਸ Err(INVALID_ਦਲੀਲ);
        }

        ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = l.position;
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ = ਸ਼ੁਰੂ + l.ਚੌੜਾਈ - 1;

        ਜੇ (unlikely (l.ਚੌੜਾਈ < 2 || l.ਚੌੜਾਈ > 4))
        {
          DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
                     "Invalid graph. Invalid link width.");
          ਵਾਪਸ Err(INVALID_ਦਲੀਲ);
        }

        ਜੇ (unlikely (ਸਿਰਾ >= ਸਾਰਣੀ_ਮਾਪ ()))
        {
          DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
                     "Invalid graph. Link position is out of bounds.");
          ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);
        }

        ਜੇ (unlikely (assigned_ਬਾਈਟਾਂ.intersects (ਸ਼ੁਰੂ, ਸਿਰਾ)))
        {
          DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
                     "Invalid graph. Found offsets whose positions overlap.");
          ਵਾਪਸ Err(INVALID_ਦਲੀਲ);
        }

        assigned_ਬਾਈਟਾਂ.ਜੋੜੋ_range (ਸ਼ੁਰੂ, ਸਿਰਾ);
      }

      ਵਾਪਸ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (assigned_ਬਾਈਟਾਂ, ਵੰਡ_FAILURE);
    }

    ਖਾਲੀ normalize ()
    {
      obj_.real_links.qsort ();
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& l : obj_.real_links)
      {
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < l.ਚੌੜਾਈ; i++)
        {
          obj_.ਸਿਰ_2[l.position + i] = 0;
        }
      }
    }

    ਤਾਰਕਿਕ equals (ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ,
                 ਬਿਨਾਨਿਸ਼ਾਨੀ other_ਸੂਚਕਾਂਕ,
                 ਅਚਲ ਸ਼ੀਰਸ਼_t& other,
                 ਅਚਲ ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ,
                 ਅਚਲ ਗ੍ਰਾਫ_t& other_ਗ੍ਰਾਫ,
                 ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ) ਅਚਲ
    {
      ਜੇ (!(as_ਬਾਈਟਾਂ () == other.as_ਬਾਈਟਾਂ ()))
      {
        DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_ਸੂਚਕਾਂਕ,
                   (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) ਸਾਰਣੀ_ਮਾਪ (),
                   other_ਸੂਚਕਾਂਕ,
                   (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) other.ਸਾਰਣੀ_ਮਾਪ (),
                   ਡੂੰਘਾਈ);

        ਸਵੈਚਾਲਿਤ a = as_ਬਾਈਟਾਂ ();
        ਸਵੈਚਾਲਿਤ b = other.as_ਬਾਈਟਾਂ ();
        ਜਦੋਂ (a || b)
        {
          DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
                     "  0x%x %s 0x%x", (ਬਿਨਾਨਿਸ਼ਾਨੀ) *a, (*a == *b) ? "==" : "!=", (ਬਿਨਾਨਿਸ਼ਾਨੀ) *b);
          a++;
          b++;
        }
        ਵਾਪਸ ਝੂਠ;
      }

      ਵਾਪਸ links_equal (obj_.real_links, other.obj_.real_links, ਗ੍ਰਾਫ, other_ਗ੍ਰਾਫ, ਡੂੰਘਾਈ);
    }

    hb_ਬਾਈਟਾਂ_t as_ਬਾਈਟਾਂ () ਅਚਲ
    {
      ਵਾਪਸ hb_ਬਾਈਟਾਂ_t (obj_.ਸਿਰ_2, ਸਾਰਣੀ_ਮਾਪ ());
    }

    ਮਿੱਤਰ ਖਾਲੀ swap (ਸ਼ੀਰਸ਼_t& a, ਸ਼ੀਰਸ਼_t& b)
    {
      hb_ਅਦਲਾ_ਬਦਲੀ (a.obj_, b.obj_);
      hb_ਅਦਲਾ_ਬਦਲੀ (a.distance, b.distance);
      hb_ਅਦਲਾ_ਬਦਲੀ (a.space, b.space);
      hb_ਅਦਲਾ_ਬਦਲੀ (a.single_ਮਾਪਾ, b.single_ਮਾਪਾ);
      hb_ਅਦਲਾ_ਬਦਲੀ (a.parents, b.parents);
      hb_ਅਦਲਾ_ਬਦਲੀ (a.virtual_parents, b.virtual_parents);
      hb_ਅਦਲਾ_ਬਦਲੀ (a.incoming_ਕਿਨਾਰੇ, b.incoming_ਕਿਨਾਰੇ);
      hb_ਅਦਲਾ_ਬਦਲੀ (a.has_incoming_virtual_ਕਿਨਾਰੇ, b.has_incoming_virtual_ਕਿਨਾਰੇ);
      hb_ਅਦਲਾ_ਬਦਲੀ (a.ਸ਼ੁਰੂ, b.ਸ਼ੁਰੂ);
      hb_ਅਦਲਾ_ਬਦਲੀ (a.ਸਿਰਾ, b.ਸਿਰਾ);
      hb_ਅਦਲਾ_ਬਦਲੀ (a.ਤਰਜੀਹ, b.ਤਰਜੀਹ);
    }

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ>>
    position_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ_map () ਅਚਲ
    {
      hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> ਨਤੀਜਾ;

      ਨਤੀਜਾ.alloc (obj_.real_links.ਲੰਮਾਈ);
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& l : obj_.real_links) {
        ਨਤੀਜਾ.ਸੈੱਟ (l.position, l.objidx);
      }

      ਵਾਪਸ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ>>::from (std::move(ਨਤੀਜਾ), ਵੰਡ_FAILURE);
    }

    ਤਾਰਕਿਕ is_shared () ਅਚਲ
    {
      ਵਾਪਸ parents.get_population () > virtual_parents.get_population () + 1;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ incoming_ਕਿਨਾਰੇ_2 () ਅਚਲ
    {
      ਜੇ (HB_DEBUG_SUBSET_REPACK)
       {
	ਪ੍ਰਤਿਜ੍ਞਾ (incoming_ਕਿਨਾਰੇ == (single_ਮਾਪਾ != HB_ਗ੍ਰਾਫ_INVALID) +
		(parents.ਮੁੱਲਾਂ_ref () | hb_reduce (hb_ਜੋੜੋ, 0)));
       }
      ਵਾਪਸ incoming_ਕਿਨਾਰੇ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ incoming_ਕਿਨਾਰੇ_from_ਮਾਪਾ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ_ਸੂਚਕਾਂਕ) ਅਚਲ {
      ਜੇ (single_ਮਾਪਾ != HB_ਗ੍ਰਾਫ_INVALID) {
        ਵਾਪਸ single_ਮਾਪਾ == ਮਾਪਾ_ਸੂਚਕਾਂਕ ? 1 : 0;
      }

      ਬਿਨਾਨਿਸ਼ਾਨੀ* ਗਿਣਤੀ_2;
      ਵਾਪਸ  parents.has(ਮਾਪਾ_ਸੂਚਕਾਂਕ, &ਗਿਣਤੀ_2) ? *ਗਿਣਤੀ_2 : 0;
    }

    ਖਾਲੀ reset_parents ()
    {
      incoming_ਕਿਨਾਰੇ = 0;
      has_incoming_virtual_ਕਿਨਾਰੇ = ਝੂਠ;
      single_ਮਾਪਾ = HB_ਗ੍ਰਾਫ_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਜੋੜੋ_ਮਾਪਾ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ_ਸੂਚਕਾਂਕ, ਤਾਰਕਿਕ is_virtual)
    {
      ਪ੍ਰਤਿਜ੍ਞਾ (ਮਾਪਾ_ਸੂਚਕਾਂਕ != HB_ਗ੍ਰਾਫ_INVALID);
      has_incoming_virtual_ਕਿਨਾਰੇ |= is_virtual;
      ਜੇ (is_virtual)
        virtual_parents.ਜੋੜੋ (ਮਾਪਾ_ਸੂਚਕਾਂਕ);

      ਜੇ (incoming_ਕਿਨਾਰੇ == 0)
      {
	single_ਮਾਪਾ = ਮਾਪਾ_ਸੂਚਕਾਂਕ;
	incoming_ਕਿਨਾਰੇ = 1;
	ਵਾਪਸ ਮੰਜ਼ਿਲ_ਨਤੀਜਾ();
      }
      ਨਹੀਂਤਾਂ ਜੇ (single_ਮਾਪਾ != HB_ਗ੍ਰਾਫ_INVALID)
      {
        ਪ੍ਰਤਿਜ੍ਞਾ (incoming_ਕਿਨਾਰੇ == 1);
	ਜੇ (!parents.ਸੈੱਟ (single_ਮਾਪਾ, 1))
	  ਵਾਪਸ ਮੰਜ਼ਿਲ_ਨਤੀਜਾ ();
	single_ਮਾਪਾ = HB_ਗ੍ਰਾਫ_INVALID;
      }

      ਬਿਨਾਨਿਸ਼ਾਨੀ *v;
      ਜੇ (parents.has (ਮਾਪਾ_ਸੂਚਕਾਂਕ, &v))
      {
        (*v)++;
	incoming_ਕਿਨਾਰੇ++;
      }
      ਨਹੀਂਤਾਂ ਜੇ (parents.ਸੈੱਟ (ਮਾਪਾ_ਸੂਚਕਾਂਕ, 1))
	incoming_ਕਿਨਾਰੇ++;

      ਵਾਪਸ ਮੰਜ਼ਿਲ_ਨਤੀਜਾ();
    }

    ਖਾਲੀ remove_ਮਾਪਾ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ_ਸੂਚਕਾਂਕ)
    {
      ਜੇ (ਮਾਪਾ_ਸੂਚਕਾਂਕ == single_ਮਾਪਾ)
      {
	single_ਮਾਪਾ = HB_ਗ੍ਰਾਫ_INVALID;
	incoming_ਕਿਨਾਰੇ--;
	virtual_parents.reset ();
	ਵਾਪਸ;
      }

      ਬਿਨਾਨਿਸ਼ਾਨੀ *v;
      ਜੇ (parents.has (ਮਾਪਾ_ਸੂਚਕਾਂਕ, &v))
      {
	incoming_ਕਿਨਾਰੇ--;
	ਜੇ (*v > 1)
	  (*v)--;
	ਨਹੀਂਤਾਂ
        {
	  parents.del (ਮਾਪਾ_ਸੂਚਕਾਂਕ);
          virtual_parents.del (ਮਾਪਾ_ਸੂਚਕਾਂਕ);
        }

	ਜੇ (incoming_ਕਿਨਾਰੇ == 1)
	{
	  single_ਮਾਪਾ = *parents.ਕੁੰਜੀਆਂ ();
	  parents.reset ();
	}
      }
    }

    ਖਾਲੀ remove_real_link_unordered (ਬਿਨਾਨਿਸ਼ਾਨੀ link_array_ਸੂਚਕਾਂਕ) {
      obj_.real_links.remove_unordered (link_array_ਸੂਚਕਾਂਕ);
    }

    ਖਾਲੀ remove_real_link_unordered (ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ_ਸੂਚਕਾਂਕ, ਅਚਲ ਖਾਲੀ* ਵਿੱਥ)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = obj_.real_links.ਲੰਮਾਈ;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      {
        ਸਵੈਚਾਲਿਤ& link = obj_.real_links.arrayZ[i];
        ਜੇ (link.objidx != ਬੱਚਾ_ਸੂਚਕਾਂਕ)
          ਜਾਰੀ;

        ਜੇ ((obj_.ਸਿਰ_2 + link.position) != ਵਿੱਥ)
          ਜਾਰੀ;

        obj_.real_links.remove_unordered (i);
        ਵਾਪਸ;
      }
    }

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> remap_ਮਾਪਾ (ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਸੂਚਕਾਂਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਸੂਚਕਾਂਕ)
    {
      ਜੇ (single_ਮਾਪਾ != HB_ਗ੍ਰਾਫ_INVALID)
      {
        ਜੇ (single_ਮਾਪਾ == old_ਸੂਚਕਾਂਕ)
        {
	  single_ਮਾਪਾ = new_ਸੂਚਕਾਂਕ;
          ਜੇ (virtual_parents.has (old_ਸੂਚਕਾਂਕ))
          {
            virtual_parents.del (old_ਸੂਚਕਾਂਕ);
            virtual_parents.ਜੋੜੋ (new_ਸੂਚਕਾਂਕ);
          }
        }
        ਵਾਪਸ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from(virtual_parents, ਵੰਡ_FAILURE);
      }

      ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *pv;
      ਜੇ (parents.has (old_ਸੂਚਕਾਂਕ, &pv))
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ v = *pv;
	ਜੇ (!parents.ਸੈੱਟ (new_ਸੂਚਕਾਂਕ, v))
          incoming_ਕਿਨਾਰੇ -= v;
	parents.del (old_ਸੂਚਕਾਂਕ);

        ਜੇ (incoming_ਕਿਨਾਰੇ == 1)
	{
	  single_ਮਾਪਾ = *parents.ਕੁੰਜੀਆਂ ();
	  parents.reset ();
	}
      }

      ਜੇ (virtual_parents.has (old_ਸੂਚਕਾਂਕ))
      {
        virtual_parents.del (old_ਸੂਚਕਾਂਕ);
        virtual_parents.ਜੋੜੋ (new_ਸੂਚਕਾਂਕ);
      }

      ਵਾਪਸ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from(virtual_parents, ਵੰਡ_FAILURE);
    }

    ਤਾਰਕਿਕ is_ਪੱਤੀ () ਅਚਲ
    {
      ਵਾਪਸ !obj_.real_links.ਲੰਮਾਈ && !obj_.virtual_links.ਲੰਮਾਈ;
    }

    ਤਾਰਕਿਕ raise_ਤਰਜੀਹ ()
    {
      ਜੇ (has_max_ਤਰਜੀਹ ()) ਵਾਪਸ ਝੂਠ;
      ਤਰਜੀਹ++;
      ਵਾਪਸ ਸੱਚ;
    }

    ਤਾਰਕਿਕ give_max_ਤਰਜੀਹ ()
    {
      ਤਾਰਕਿਕ ਨਤੀਜਾ = ਝੂਠ;
      ਜਦੋਂ (!has_max_ਤਰਜੀਹ()) {
        ਨਤੀਜਾ = ਸੱਚ;
        ਤਰਜੀਹ++;
      }
      ਵਾਪਸ ਨਤੀਜਾ;
    }

    ਤਾਰਕਿਕ has_max_ਤਰਜੀਹ () ਅਚਲ {
      ਵਾਪਸ ਤਰਜੀਹ >= 3;
    }

    size_t ਸਾਰਣੀ_ਮਾਪ () ਅਚਲ {
      ਵਾਪਸ obj_.ਪੂਛ - obj_.ਸਿਰ_2;
    }

    int64_t modified_ਦੂਰੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਕ੍ਰਮ) ਅਚਲ
    {
      // TODO(garretrieger): once priority is high enough, should try
      // setting distance = 0 which will force to sort immediately after
      // it's parent where possible.

      int64_t modified_ਦੂਰੀ =
          hb_clamp (distance + ਦੂਰੀ_modifier (), (int64_t) 0,
                    hb_int_max (int64_t) >> 18);
      ਜੇ (has_max_ਤਰਜੀਹ ()) {
        modified_ਦੂਰੀ = 0;
      }
      ਵਾਪਸ (modified_ਦੂਰੀ << 18) | (0x003FFFF & ਕ੍ਰਮ);
    }

    int64_t ਦੂਰੀ_modifier () ਅਚਲ
    {
      ਜੇ (!ਤਰਜੀਹ) ਵਾਪਸ 0;
      int64_t ਸਾਰਣੀ_ਮਾਪ = obj_.ਪੂਛ - obj_.ਸਿਰ_2;

      ਜੇ (ਤਰਜੀਹ == 1)
        ਵਾਪਸ -ਸਾਰਣੀ_ਮਾਪ / 2;

      ਵਾਪਸ -ਸਾਰਣੀ_ਮਾਪ;
    }

   ਨਿਜੀ:
    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਮੰਜ਼ਿਲ_ਨਤੀਜਾ() ਅਚਲ {
      ਜੇ (unlikely (
        parents.ਵਿੱਚ_error() || virtual_parents.ਵਿੱਚ_error() ||
        obj_.real_links.ਵਿੱਚ_error() || obj_.virtual_links.ਵਿੱਚ_error()))
        ਵਾਪਸ Err(ਵੰਡ_FAILURE);
      ਵਾਪਸ Ok();
    }

    ਤਾਰਕਿਕ links_equal (ਅਚਲ hb_ਸਦਿਸ਼_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      ਅਚਲ hb_ਸਦਿਸ਼_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      ਅਚਲ ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ,
                      ਅਚਲ ਗ੍ਰਾਫ_t& other_ਗ੍ਰਾਫ,
                      ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ) ਅਚਲ
    {
      ਸਵੈਚਾਲਿਤ a = this_links.iter ();
      ਸਵੈਚਾਲਿਤ b = other_links.iter ();

      ਜਦੋਂ (a && b)
      {
        ਅਚਲ ਸਵੈਚਾਲਿਤ& link_a = *a;
        ਅਚਲ ਸਵੈਚਾਲਿਤ& link_b = *b;

        ਜੇ (link_a.ਚੌੜਾਈ != link_b.ਚੌੜਾਈ ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          ਵਾਪਸ ਝੂਠ;

        ਜੇ (!ਗ੍ਰਾਫ.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_ਗ੍ਰਾਫ.vertices_[link_b.objidx], ਗ੍ਰਾਫ, other_ਗ੍ਰਾਫ, ਡੂੰਘਾਈ + 1))
          ਵਾਪਸ ਝੂਠ;

        a++;
        b++;
      }

      ਜੇ (ਤਾਰਕਿਕ (a) != ਤਾਰਕਿਕ (b))
        ਵਾਪਸ ਝੂਠ;

      ਵਾਪਸ ਸੱਚ;
    }
  };

  ਗਿਣਤੀ ਸ਼ੀਰਸ਼_mutability_t
  {
    Immutable,
    Mutable
  };

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਸ਼ੀਰਸ਼_mutability_t mutability = Mutable>
  ਬਣਤਰ ਸ਼ੀਰਸ਼_and_ਸਾਰਣੀ_t
  {
    ਸ਼ੀਰਸ਼_and_ਸਾਰਣੀ_t () : ਸੂਚਕਾਂਕ_2 (0), ਸ਼ੀਰਸ਼ (ਖਾਲੀ_ਸੂਚਕ), ਸਾਰਣੀ (ਖਾਲੀ_ਸੂਚਕ)
    {}

    ਸਾਂਚਾ <ਸ਼ੀਰਸ਼_mutability_t other_mutability>
    ਸ਼ੀਰਸ਼_and_ਸਾਰਣੀ_t (ਅਚਲ ਸ਼ੀਰਸ਼_and_ਸਾਰਣੀ_t<T, other_mutability>& o)
      : ਸੂਚਕਾਂਕ_2 (o.ਸੂਚਕਾਂਕ_2), ਸ਼ੀਰਸ਼ (o.ਸ਼ੀਰਸ਼), ਸਾਰਣੀ (o.ਸਾਰਣੀ)
    {}

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2;
    ਕਿਸਮ_ਨਾਂ std::conditional<mutability == Immutable, ਅਚਲ ਸ਼ੀਰਸ਼_t*, ਸ਼ੀਰਸ਼_t*>::type ਸ਼ੀਰਸ਼;
    ਕਿਸਮ_ਨਾਂ std::conditional<mutability == Immutable, ਅਚਲ T*, T*>::type ਸਾਰਣੀ;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਗ੍ਰਾਫ_t> create (ਅਚਲ T& objects)
  {
    ਜੇ (unlikely (objects.ਲੰਮਾਈ > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.ਲੰਮਾਈ);
      ਵਾਪਸ Err(ਹੱਦਬੰਦੀ_EXCEEDED);
    }

    ਗ੍ਰਾਫ_t g;
    g.ਅੰਕ_roots_for_space.push (1);
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (g.ਅੰਕ_roots_for_space, ਵੰਡ_FAILURE));

    ਤਾਰਕਿਕ removed_nil = ਝੂਠ;
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = objects.ਲੰਮਾਈ;
    g.vertices_.alloc (ਗਿਣਤੀ_2);
    g.ordering_.resize (ਗਿਣਤੀ_2);
    g.ordering_scratch_.alloc (ਗਿਣਤੀ_2);
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (g.vertices_, ਵੰਡ_FAILURE));
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (g.ordering_, ਵੰਡ_FAILURE));
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (g.ordering_scratch_, ਵੰਡ_FAILURE));

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕ੍ਰਮ = objects.ਲੰਮਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ skip = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      ਜੇ (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = ਸੱਚ;
        ਕ੍ਰਮ--;
        g.ordering_.resize (ਗਿਣਤੀ_2 - 1);
        skip++;
        ਜਾਰੀ;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      ਜੇ (unlikely (obj.ਵਿੱਚ_error ())) {
        ਵਾਪਸ Err(ਵੰਡ_FAILURE);
      }
      ਸ਼ੀਰਸ਼_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (ਗਿਣਤੀ_2, removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      ਬਿਨਾਨਿਸ਼ਾਨੀ obj_idx = i - skip;
      g.ordering_[--ਕ੍ਰਮ] = obj_idx;

      ਜੇ (!removed_nil) ਜਾਰੀ;
      // Fix indices to account for removed nil object.
      ਲਈ (ਸਵੈਚਾਲਿਤ& l : v->all_links_writer ()) {
        l.objidx--;
      }
    }

    ਸਵੈਚਾਲਿਤ r = g.is_fully_connected();
    ਜੇ (r.is_err() && r.error() == ORPHANED_NODES) {
      g.print_orphaned_nodes ();
    }
    TRY(r);

    ਵਾਪਸ Ok (std::move (g));
  }

  ਗ੍ਰਾਫ_t (ਅਚਲ ਗ੍ਰਾਫ_t&) = ਮਿਟਾਓ;
  ਗ੍ਰਾਫ_t& ਸੰਚਾਲਕ = (ਅਚਲ ਗ੍ਰਾਫ_t&) = ਮਿਟਾਓ;
  ਗ੍ਰਾਫ_t (ਗ੍ਰਾਫ_t&&) = ਮੂਲ;
  ਗ੍ਰਾਫ_t& ਸੰਚਾਲਕ = (ਗ੍ਰਾਫ_t&&) = ਮੂਲ;

  ~ਗ੍ਰਾਫ_t ()
  {
    ਲਈ (ਅੱਖਰ* b : buffers)
      hb_free (b);
  }

ਨਿਜੀ:
  ਗ੍ਰਾਫ_t () = ਮੂਲ;

ਜਨਤਕ:

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ== (ਅਚਲ ਗ੍ਰਾਫ_t& other) ਅਚਲ
  {
    ਵਾਪਸ ਜੜ੍ਹ ().equals (ਜੜ੍ਹ_idx(), other.ਜੜ੍ਹ_idx(), other.ਜੜ੍ਹ (), *ਇਹ, other, 0);
  }

  ਖਾਲੀ print () ਅਚਲ {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ id : ordering_)
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ& v = vertices_[id];
      ਲਿਖੋ("%u: %u [", id, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)v.ਸਾਰਣੀ_ਮਾਪ());
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &l : v.obj().real_links) {
        ਲਿਖੋ("%u, ", l.objidx);
      }
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &l : v.obj().virtual_links) {
        ਲਿਖੋ("v%u, ", l.objidx);
      }
      ਲਿਖੋ("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  ਖਾਲੀ normalize ()
  {
    ਲਈ (ਸਵੈਚਾਲਿਤ& v : vertices_.writer ())
      v.normalize ();
  }

  ਅਚਲ ਸ਼ੀਰਸ਼_t& ਜੜ੍ਹ () ਅਚਲ
  {
    ਵਾਪਸ vertices_[ਜੜ੍ਹ_idx ()];
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੜ੍ਹ_idx () ਅਚਲ
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    ਵਾਪਸ ordering_[0];
  }

  ਅਚਲ hb_serialize_context_t::object_t& object (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ
  {
    ਵਾਪਸ vertices_[i].obj();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਜੋੜੋ_ਬਫਰ (ਅੱਖਰ* ਬਫਰ)
  {
    buffers.push (ਬਫਰ);
    ਵਾਪਸ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (buffers, ਵੰਡ_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਜੋੜੋ_link (T* ਵਿੱਥ,
                                 ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ_id,
                                 ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ_id)
  {
    ਜੇ (unlikely (ਮਾਪਾ_id >= vertices_.ਲੰਮਾਈ || ਬੱਚਾ_id >= vertices_.ਲੰਮਾਈ))
      ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);
    ਸਵੈਚਾਲਿਤ& v = vertices_[ਮਾਪਾ_id];
    TRY(v.ਜੋੜੋ_real_link(2, ਬੱਚਾ_id, (ਅੱਖਰ*) ਵਿੱਥ - (ਅੱਖਰ*) v.obj().ਸਿਰ_2));
    ਵਾਪਸ vertices_[ਬੱਚਾ_id].ਜੋੜੋ_ਮਾਪਾ (ਮਾਪਾ_id, ਝੂਠ);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਛਾਂਟੋ_ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ_ਦੂਰੀ_if_needed ()
  {
    ਜੇ (!positions_invalid) ਵਾਪਸ Ok();
    ਵਾਪਸ ਛਾਂਟੋ_ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ_ਦੂਰੀ ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਛਾਂਟੋ_ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ_ਦੂਰੀ ()
  {
    positions_invalid = ਸੱਚ;

    ਜੇ (vertices_.ਲੰਮਾਈ <= 1) {
      // Graph of 1 or less doesn't need sorting.
      ਵਾਪਸ Ok();
    }

    TRY (update_distances ());

    hb_ਤਰਜੀਹ_ਕਤਾਰ_t<int64_t> ਕਤਾਰ;
    ਕਤਾਰ.alloc (vertices_.ਲੰਮਾਈ);
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਕਤਾਰ, ਵੰਡ_FAILURE));

    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.ਲੰਮਾਈ);
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (new_ordering, ਵੰਡ_FAILURE));

    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> removed_ਕਿਨਾਰੇ;
    removed_ਕਿਨਾਰੇ.resize (vertices_.ਲੰਮਾਈ);
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (removed_ਕਿਨਾਰੇ, ਵੰਡ_FAILURE));

    TRY (update_parents ());

    ਕਤਾਰ.insert (ਜੜ੍ਹ ().modified_ਦੂਰੀ (0), ਜੜ੍ਹ_idx ());
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕ੍ਰਮ = 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ pos = 0;
    ਜਦੋਂ (!ਕਤਾਰ.ਵਿੱਚ_error () && !ਕਤਾਰ.is_empty ())
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਗਲਾ_id = ਕਤਾਰ.ਕੱਢੋ_minimum().second;

      ਜੇ (unlikely (pos >= new_ordering.ਲੰਮਾਈ)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "Invalid graph. Contains cycle.");
        ਵਾਪਸ Err(ਚੱਕਰ_DETECTED);
      }
      new_ordering[pos++] = ਅਗਲਾ_id;
      ਅਚਲ ਸ਼ੀਰਸ਼_t& next = vertices_[ਅਗਲਾ_id];

      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& link : next.obj().all_links ()) {
        ਜੇ (unlikely (link.objidx >= vertices_.ਲੰਮਾਈ))
          ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);
        removed_ਕਿਨਾਰੇ[link.objidx]++;
        ਅਚਲ ਸਵੈਚਾਲਿਤ& v = vertices_[link.objidx];
        ਜੇ (!(v.incoming_ਕਿਨਾਰੇ_2 () - removed_ਕਿਨਾਰੇ[link.objidx]))
          // Add the order that the links were encountered to the priority.
          // This ensures that ties between priorities objects are broken in a consistent
          // way. More specifically this is set up so that if a set of objects have the same
          // distance they'll be added to the topological order in the order that they are
          // referenced from the parent object.
          ਕਤਾਰ.insert (v.modified_ਦੂਰੀ (ਕ੍ਰਮ++),
                        link.objidx);
      }
    }

    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਕਤਾਰ, ਵੰਡ_FAILURE));
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (new_ordering, ਵੰਡ_FAILURE));

    hb_ਅਦਲਾ_ਬਦਲੀ (ordering_, new_ordering);

    ਜੇ (unlikely (pos != vertices_.ਲੰਮਾਈ)) {
      print_orphaned_nodes ();
      ਵਾਪਸ Err(ORPHANED_NODES);
    }

    ਵਾਪਸ Ok();
  }

  /*
   * Finds the set of nodes (placed into roots) that should be assigned unique spaces.
   * More specifically this looks for the top most 24 bit or 32 bit links in the graph.
   * Some special casing is done that is specific to the layout of GSUB/GPOS tables.
   */
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਲੱਭੋ_space_roots (hb_ਸੈੱਟ_t& ਦੇਖਿਆ, hb_ਸੈੱਟ_t& roots)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੜ੍ਹ_ਸੂਚਕਾਂਕ = ਜੜ੍ਹ_idx ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i : ordering_)
    {
      ਜੇ (ਦੇਖਿਆ.has (i)) ਜਾਰੀ;

      // Only real links can form 32 bit spaces
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& l : vertices_[i].obj().real_links)
      {
        ਜੇ (l.is_signed || l.ਚੌੜਾਈ < 3)
          ਜਾਰੀ;

        ਜੇ (i == ਜੜ੍ਹ_ਸੂਚਕਾਂਕ && l.ਚੌੜਾਈ == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          ਜਾਰੀ;

        ਜੇ (l.ਚੌੜਾਈ >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_ਸੈੱਟ_t sub_roots;
          TRY (ਲੱਭੋ_32bit_roots (l.objidx, sub_roots));
          ਜੇ (sub_roots) {
            ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ sub_ਜੜ੍ਹ_idx : sub_roots) {
              roots.ਜੋੜੋ (sub_ਜੜ੍ਹ_idx);
              TRY (ਲੱਭੋ_subgraph (sub_ਜੜ੍ਹ_idx, ਦੇਖਿਆ));
            }
            ਜਾਰੀ;
          }
        }

        roots.ਜੋੜੋ (l.objidx);
        TRY (ਲੱਭੋ_subgraph (l.objidx, ਦੇਖਿਆ));
      }
    }

    TRY(ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਦੇਖਿਆ, ਵੰਡ_FAILURE));
    TRY(ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (roots, ਵੰਡ_FAILURE));
    ਵਾਪਸ Ok();
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਸ਼ੀਰਸ਼_and_ਸਾਰਣੀ_t<T, Immutable>> as_ਸਾਰਣੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ, ਅਚਲ ਖਾਲੀ* ਵਿੱਥ, Ts... ds)
  {
    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ idx, ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (ਮਾਪਾ, ਵਿੱਥ));
    ਵਾਪਸ as_ਸਾਰਣੀ_from_ਸੂਚਕਾਂਕ<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਸ਼ੀਰਸ਼_and_ਸਾਰਣੀ_t<T, Mutable>> as_mutable_ਸਾਰਣੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ, ਅਚਲ ਖਾਲੀ* ਵਿੱਥ, Ts... ds)
  {
    TRY_ਸੌਂਪੋ (ਸਵੈਚਾਲਿਤ idx, mutable_ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (ਮਾਪਾ, ਵਿੱਥ));
    ਵਾਪਸ as_ਸਾਰਣੀ_from_ਸੂਚਕਾਂਕ<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਸ਼ੀਰਸ਼_mutability_t mutability = Mutable, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਸ਼ੀਰਸ਼_and_ਸਾਰਣੀ_t<T, mutability>> as_ਸਾਰਣੀ_from_ਸੂਚਕਾਂਕ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2, Ts... ds)
  {
    ਜੇ (unlikely (ਸੂਚਕਾਂਕ_2 >= vertices_.ਲੰਮਾਈ))
      ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);

    ਸ਼ੀਰਸ਼_and_ਸਾਰਣੀ_t<T, mutability> r;
    r.ਸ਼ੀਰਸ਼ = (ਕਿਸਮ_ਨਾਂ std::conditional<mutability == Immutable, ਅਚਲ ਸ਼ੀਰਸ਼_t*, ਸ਼ੀਰਸ਼_t*>::type) &vertices_[ਸੂਚਕਾਂਕ_2];
    r.ਸਾਰਣੀ = r.ਸ਼ੀਰਸ਼->ਸਾਂਚਾ as<ਕਿਸਮ_ਨਾਂ std::conditional<mutability == Immutable, ਅਚਲ T*, T*>::type> ();
    r.ਸੂਚਕਾਂਕ_2 = ਸੂਚਕਾਂਕ_2;
    ਜੇ (unlikely (!r.ਸਾਰਣੀ))
      ਵਾਪਸ Err(INVALID_ਦਲੀਲ);

    TRY(r.ਸਾਰਣੀ->sanitize (*(r.ਸ਼ੀਰਸ਼), std::forward<Ts>(ds)...));
    ਵਾਪਸ r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗੰਢ_idx, ਅਚਲ ਖਾਲੀ* ਵਿੱਥ) ਅਚਲ
  {
    ਜੇ (unlikely (ਗੰਢ_idx >= vertices_.ਲੰਮਾਈ)) ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);
    ਅਚਲ ਸਵੈਚਾਲਿਤ& node = object (ਗੰਢ_idx);
    ਜੇ (unlikely (ਵਿੱਥ < node.ਸਿਰ_2 || ਵਿੱਥ >= node.ਪੂਛ)) ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = node.real_links.ਲੰਮਾਈ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      ਅਚਲ ਸਵੈਚਾਲਿਤ& link = node.real_links.arrayZ[i];
      ਜੇ (ਵਿੱਥ != node.ਸਿਰ_2 + link.position)
        ਜਾਰੀ;
      ਵਾਪਸ Ok(link.objidx);
    }

    ਵਾਪਸ Err(INVALID_ਦਲੀਲ);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> mutable_ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗੰਢ_idx, ਅਚਲ ਖਾਲੀ* ਵਿੱਥ)
  {
    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ_idx, ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (ਗੰਢ_idx, ਵਿੱਥ));
    ਜੇ (unlikely (ਬੱਚਾ_idx >= vertices_.ਲੰਮਾਈ))
      ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);

    ਸਵੈਚਾਲਿਤ& ਬੱਚਾ = vertices_[ਬੱਚਾ_idx];
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ p : ਬੱਚਾ.parents_iter ())
    {
      ਜੇ (p != ਗੰਢ_idx) {
        ਵਾਪਸ duplicate (ਗੰਢ_idx, ਬੱਚਾ_idx, ਸੱਚ);
      }
    }

    ਵਾਪਸ Ok(ਬੱਚਾ_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਤਾਰਕਿਕ> ਸੌਂਪੋ_spaces ()
  {
    TRY (update_parents ());

    ਲਈ (ਸਵੈਚਾਲਿਤ& ਸ਼ੀਰਸ਼ : vertices_.writer ())
    {
      ਸ਼ੀਰਸ਼.space = 0;
      ਸ਼ੀਰਸ਼.ਤਰਜੀਹ = 0;
    }
    ਅੰਕ_roots_for_space.resize (1);
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਅੰਕ_roots_for_space, ਵੰਡ_FAILURE));
    ਅੰਕ_roots_for_space[0] = 1;
    ਦੂਰੀ_invalid = ਸੱਚ;
    positions_invalid = ਸੱਚ;

    hb_ਸੈੱਟ_t ਦੇਖਿਆ;
    hb_ਸੈੱਟ_t roots;
    TRY (ਲੱਭੋ_space_roots (ਦੇਖਿਆ, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    ਦੇਖਿਆ.invert ();

    ਜੇ (!roots) ਵਾਪਸ Ok(ਝੂਠ);

    ਜਦੋਂ (roots)
    {
      uint32_t next = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
      TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (roots, ਵੰਡ_FAILURE));
      ਜੇ (!roots.next (&next)) ਰੋਕੋ;

      hb_ਸੈੱਟ_t connected_roots;
      TRY (ਲੱਭੋ_connected_nodes (next, roots, ਦੇਖਿਆ, connected_roots));
      TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (connected_roots, ਵੰਡ_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (connected_roots, ਵੰਡ_FAILURE));

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਗਲਾ_space = ਇਹ->ਅਗਲਾ_space ();
      ਜੇ (unlikely (ਅਗਲਾ_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 ਅਗਲਾ_space);
        ਵਾਪਸ Err(ਹੱਦਬੰਦੀ_EXCEEDED);
      }
      ਅੰਕ_roots_for_space.push (0);
      TRY(ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਅੰਕ_roots_for_space, ਵੰਡ_FAILURE));

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੜ੍ਹ : connected_roots)
      {
        DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "Subgraph %u gets space %u", ਜੜ੍ਹ, ਅਗਲਾ_space);
        vertices_[ਜੜ੍ਹ].space = ਅਗਲਾ_space;
        ਅੰਕ_roots_for_space[ਅਗਲਾ_space] = ਅੰਕ_roots_for_space[ਅਗਲਾ_space] + 1;
        ਦੂਰੀ_invalid = ਸੱਚ;
        positions_invalid = ਸੱਚ;
      }

      // TODO(grieger): special case for GSUB/GPOS use extension promotions to move 16 bit space
      //                into the 32 bit space as needed, instead of using isolation.
    }

    ਵਾਪਸ Ok(ਸੱਚ);
  }

  /*
   * Isolates the subgraph of nodes reachable from root. Any links to nodes in the subgraph
   * that originate from outside of the subgraph will be removed by duplicating the linked to
   * object.
   *
   * Indices stored in roots will be updated if any of the roots are duplicated to new indices.
   */
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਤਾਰਕਿਕ> isolate_subgraph (hb_ਸੈੱਟ_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_ਸੈੱਟ_t parents;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੜ੍ਹ_idx : roots)
    {
      ਜੇ (unlikely (ਜੜ੍ਹ_idx >= vertices_.ਲੰਮਾਈ))
        ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);
      subgraph.ਸੈੱਟ (ਜੜ੍ਹ_idx, wide_parents (ਜੜ੍ਹ_idx, parents));
    }
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੜ੍ਹ_idx : roots)
    {
      TRY (ਲੱਭੋ_subgraph (ਜੜ੍ਹ_idx, subgraph));
    }
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (subgraph, ਵੰਡ_FAILURE));

    hb_map_t ਸੂਚਕਾਂਕ_map;
    ਤਾਰਕਿਕ made_changes = ਝੂਠ;
    ਲਈ (ਸਵੈਚਾਲਿਤ ਇੰਦਰਾਜ : subgraph.iter ())
    {
      ਪ੍ਰਤਿਜ੍ਞਾ (ਇੰਦਰਾਜ.ਪਹਿਲਾ < vertices_.ਲੰਮਾਈ);
      ਅਚਲ ਸਵੈਚਾਲਿਤ& node = vertices_[ਇੰਦਰਾਜ.ਪਹਿਲਾ];
      ਬਿਨਾਨਿਸ਼ਾਨੀ subgraph_incoming_ਕਿਨਾਰੇ = ਇੰਦਰਾਜ.second;

      ਜੇ (subgraph_incoming_ਕਿਨਾਰੇ < node.incoming_ਕਿਨਾਰੇ_2 ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = ਸੱਚ;
        TRY (duplicate_subgraph (ਇੰਦਰਾਜ.ਪਹਿਲਾ, ਸੂਚਕਾਂਕ_map));
      }
    }

    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਸੂਚਕਾਂਕ_map, ਵੰਡ_FAILURE));

    ਜੇ (!made_changes)
      ਵਾਪਸ Ok(ਝੂਠ);

    ਸਵੈਚਾਲਿਤ new_subgraph =
        + subgraph.ਕੁੰਜੀਆਂ ()
        | hb_map([&] (uint32_t ਗੰਢ_idx) {
          ਅਚਲ uint32_t *v;
          ਜੇ (ਸੂਚਕਾਂਕ_map.has (ਗੰਢ_idx, &v)) ਵਾਪਸ *v;
          ਵਾਪਸ ਗੰਢ_idx;
        })
        ;

    TRY(remap_obj_indices (ਸੂਚਕਾਂਕ_map, new_subgraph));
    TRY(remap_obj_indices (ਸੂਚਕਾਂਕ_map, parents.iter (), ਸੱਚ));

    // Update roots set with new indices as needed.
    ਲਈ (ਸਵੈਚਾਲਿਤ next : roots)
    {
      ਅਚਲ uint32_t *v;
      ਜੇ (ਸੂਚਕਾਂਕ_map.has (next, &v))
      {
        roots.del (next);
        roots.ਜੋੜੋ (*v);
      }
    }

    ਵਾਪਸ Ok(ਸੱਚ);
  }

  // BFS graph traversal starting at start_idx.
  //
  // The visit_edge function will be called once for the root node and then for each traversed edge
  // with the following signature:
  //
  // bool VisitEdgeFunc(unsigned parent, const link_t* link, unsigned child, unsigned depth)
  //
  // Where a return value of false signals that traversal should not continue into child's outgoing
  // edges. parent/child are the vertex indices. depth starts at 0 for the root node.
  // link will be null when called for entering the root node at the start of the traversal.
  //
  // This traversal does not use a visited set internally, it is the responsibility of visit_edge
  // to track and filter visited nodes if required for the particular traversal.
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਦੇਖੋ_ਕਿਨਾਰਾ_Func>
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> traverse_directed_bfs (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_idx, ਦੇਖੋ_ਕਿਨਾਰਾ_Func&& ਦੇਖੋ_ਕਿਨਾਰਾ)
  {
    ਜੇ (unlikely (ਸ਼ੁਰੂ_idx >= vertices_.ਲੰਮਾਈ))
    {
      DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.ਲੰਮਾਈ);
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ordering_scratch_, ਵੰਡ_FAILURE));

    ਜੇ (!ਦੇਖੋ_ਕਿਨਾਰਾ (HB_CODEPOINT_INVALID, ਖਾਲੀ_ਸੂਚਕ, ਸ਼ੁਰੂ_idx, 0))
      ਵਾਪਸ Ok();

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰ_2 = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਛ = 0;
    ਸਵੈਚਾਲਿਤ& ਕਤਾਰ = ordering_scratch_;
    ਕਤਾਰ[ਪੂਛ++] = ਸ਼ੁਰੂ_idx;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ = 0;
    ਜਦੋਂ (ਸਿਰ_2 < ਪੂਛ)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ level_ਸਿਰਾ = ਪੂਛ;
      ਜਦੋਂ (ਸਿਰ_2 < level_ਸਿਰਾ)
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਗੰਢ_idx = ਕਤਾਰ[ਸਿਰ_2++];
        ਅਚਲ ਸਵੈਚਾਲਿਤ& v = vertices_[ਗੰਢ_idx];

        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_real = v.obj().real_links.ਲੰਮਾਈ;
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_links = ਅੰਕ_real + v.obj().virtual_links.ਲੰਮਾਈ;

        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਕੁੱਲ_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          ਅਚਲ ਸਵੈਚਾਲਿਤ& v = vertices_[ਗੰਢ_idx];
          ਅਚਲ ਸਵੈਚਾਲਿਤ& link = (i < ਅੰਕ_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - ਅੰਕ_real];
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ_idx = link.objidx;

          ਜੇ (!ਦੇਖੋ_ਕਿਨਾਰਾ (ਗੰਢ_idx, &link, ਬੱਚਾ_idx, ਡੂੰਘਾਈ + 1))
            ਜਾਰੀ;

          ਜੇ (unlikely (ਪੂਛ >= ਕਤਾਰ.ਲੰਮਾਈ))
            ਵਾਪਸ Err(ਚੱਕਰ_DETECTED);

          ਕਤਾਰ[ਪੂਛ++] = ਬੱਚਾ_idx;
        }
      }
      ਡੂੰਘਾਈ++;
    }
    ਵਾਪਸ Ok();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਲੱਭੋ_subgraph (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗੰਢ_idx, hb_map_t& subgraph)
  {
    ਵਾਪਸ traverse_directed_bfs (ਗੰਢ_idx, [&] (
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ,
      ਅਚਲ hb_serialize_context_t::object_t::link_t* link,
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ,
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ) {
      ਜੇ (ਡੂੰਘਾਈ == 0) ਵਾਪਸ ਸੱਚ;
      hb_codepoint_t *ਗਿਣਤੀ_2;
      ਜੇ (subgraph.has (ਬੱਚਾ, &ਗਿਣਤੀ_2))
      {
        (*ਗਿਣਤੀ_2)++;
        ਵਾਪਸ ਝੂਠ;
      }
      subgraph.ਸੈੱਟ (ਬੱਚਾ, 1);
      ਵਾਪਸ ਸੱਚ;
    });
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਲੱਭੋ_subgraph (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗੰਢ_idx, hb_ਸੈੱਟ_t& subgraph)
  {
    ਵਾਪਸ traverse_directed_bfs (ਗੰਢ_idx, [&] (
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ,
      ਅਚਲ hb_serialize_context_t::object_t::link_t* link,
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ,
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ) {
      ਜੇ (subgraph.has (ਬੱਚਾ)) ਵਾਪਸ ਝੂਠ;
      subgraph.ਜੋੜੋ (ਬੱਚਾ);
      ਵਾਪਸ ਸੱਚ;
    });
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<size_t> ਲੱਭੋ_subgraph_ਮਾਪ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗੰਢ_idx, hb_ਸੈੱਟ_t& subgraph, ਬਿਨਾਨਿਸ਼ਾਨੀ max_ਡੂੰਘਾਈ = HB_ਗ੍ਰਾਫ_INVALID)
  {
    size_t ਮਾਪ = 0;
    TRY (traverse_directed_bfs (ਗੰਢ_idx, [&] (
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ,
      ਅਚਲ hb_serialize_context_t::object_t::link_t* link,
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ,
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ) {
      ਜੇ (subgraph.has (ਬੱਚਾ)) ਵਾਪਸ ਝੂਠ;
      subgraph.ਜੋੜੋ (ਬੱਚਾ);

      ਅਚਲ ਸਵੈਚਾਲਿਤ& o = vertices_[ਬੱਚਾ].obj();
      ਮਾਪ += o.ਪੂਛ - o.ਸਿਰ_2;
      ਵਾਪਸ ਡੂੰਘਾਈ < max_ਡੂੰਘਾਈ;
    }));
    ਵਾਪਸ Ok(ਮਾਪ);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਲੱਭੋ_32bit_roots (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗੰਢ_idx, hb_ਸੈੱਟ_t& ਮਿਲਿਆ)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_ਸੈੱਟ_t ਦੇਖਿਆ;
    ਵਾਪਸ traverse_directed_bfs (ਗੰਢ_idx, [&] (
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ,
      ਅਚਲ hb_serialize_context_t::object_t::link_t* link,
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ,
      ਬਿਨਾਨਿਸ਼ਾਨੀ _) {

      ਜੇ (link && ਮਿਲਿਆ.has(ਮਾਪਾ))
        // Don't traverse from something that's already marked as a root.
        ਵਾਪਸ ਝੂਠ;

      ਜੇ (link && !link->is_signed && link->ਚੌੜਾਈ == 4)
      {
        ਮਿਲਿਆ.ਜੋੜੋ (link->objidx);
        ਦੇਖਿਆ.ਜੋੜੋ (link->objidx);
        ਵਾਪਸ ਝੂਠ;
      }

      ਜੇ (ਦੇਖਿਆ.has (ਬੱਚਾ)) ਵਾਪਸ ਝੂਠ;
      ਦੇਖਿਆ.ਜੋੜੋ (ਬੱਚਾ);
      ਵਾਪਸ ਸੱਚ;
    });
  }

  /*
   * Moves the child of old_parent_idx pointed to by old_offset to a new
   * vertex at the new_offset.
   *
   * Returns the id of the child node that was moved.
   */
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ O>
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> move_ਬੱਚਾ (ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਮਾਪਾ_idx,
                                       ਅਚਲ O* old_ਵਿੱਥ,
                                       ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਮਾਪਾ_idx,
                                       ਅਚਲ O* new_ਵਿੱਥ)
  {
    ਦੂਰੀ_invalid = ਸੱਚ;
    positions_invalid = ਸੱਚ;

    ਜੇ (unlikely (old_ਮਾਪਾ_idx >= vertices_.ਲੰਮਾਈ ||
                  new_ਮਾਪਾ_idx >= vertices_.ਲੰਮਾਈ))
      ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);

    ਸਵੈਚਾਲਿਤ& old_v = vertices_[old_ਮਾਪਾ_idx];
    ਸਵੈਚਾਲਿਤ& new_v = vertices_[new_ਮਾਪਾ_idx];

    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ_id, ਸੂਚਕਾਂਕ_for_ਵਿੱਥ (old_ਮਾਪਾ_idx,
                                                     old_ਵਿੱਥ));
    ਜੇ (unlikely (ਬੱਚਾ_id >= vertices_.ਲੰਮਾਈ))
      ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);

    TRY(new_v.ਜੋੜੋ_real_link(O::static_ਮਾਪ, ਬੱਚਾ_id, (ਅਚਲ ਅੱਖਰ*) new_ਵਿੱਥ - (ਅਚਲ ਅੱਖਰ*) new_v.obj().ਸਿਰ_2));

    ਸਵੈਚਾਲਿਤ& ਬੱਚਾ = vertices_[ਬੱਚਾ_id];
    TRY(ਬੱਚਾ.ਜੋੜੋ_ਮਾਪਾ (new_ਮਾਪਾ_idx, ਝੂਠ));

    old_v.remove_real_link_unordered (ਬੱਚਾ_id, old_ਵਿੱਥ);
    ਬੱਚਾ.remove_ਮਾਪਾ (old_ਮਾਪਾ_idx);

    ਵਾਪਸ Ok(ਬੱਚਾ_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ O>
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> move_children (ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਮਾਪਾ_idx,
                                      ਬਿਨਾਨਿਸ਼ਾਨੀ old_pos_ਸ਼ੁਰੂ,
                                      ਬਿਨਾਨਿਸ਼ਾਨੀ old_pos_ਸਿਰਾ,
                                      ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਮਾਪਾ_idx,
                                      ਬਿਨਾਨਿਸ਼ਾਨੀ new_pos_ਸ਼ੁਰੂ)
  {
    ਜੇ (unlikely (old_ਮਾਪਾ_idx >= vertices_.ਲੰਮਾਈ ||
                  new_ਮਾਪਾ_idx >= vertices_.ਲੰਮਾਈ))
      ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);

    ਦੂਰੀ_invalid = ਸੱਚ;
    positions_invalid = ਸੱਚ;

    ਸਵੈਚਾਲਿਤ& old_v = vertices_[old_ਮਾਪਾ_idx];
    ਸਵੈਚਾਲਿਤ& new_v = vertices_[new_ਮਾਪਾ_idx];

    hb_ਸਦਿਸ਼_t<hb_serialize_context_t::object_t::link_t> old_links;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& l : old_v.obj().real_links)
    {
      ਜੇ (l.position < old_pos_ਸ਼ੁਰੂ || l.position >= old_pos_ਸਿਰਾ)
      {
        old_links.push (l);
        ਜਾਰੀ;
      }

      ਬਿਨਾਨਿਸ਼ਾਨੀ array_pos = l.position - old_pos_ਸ਼ੁਰੂ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ_id = l.objidx;
      ਜੇ (unlikely (ਬੱਚਾ_id >= vertices_.ਲੰਮਾਈ))
        ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);

      TRY(new_v.ਜੋੜੋ_real_link(O::static_ਮਾਪ, ਬੱਚਾ_id, new_pos_ਸ਼ੁਰੂ + array_pos));

      ਸਵੈਚਾਲਿਤ& ਬੱਚਾ = vertices_[ਬੱਚਾ_id];
      TRY(ਬੱਚਾ.ਜੋੜੋ_ਮਾਪਾ (new_ਮਾਪਾ_idx, ਝੂਠ));
      ਬੱਚਾ.remove_ਮਾਪਾ (old_ਮਾਪਾ_idx);
    }

    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (old_links, ਵੰਡ_FAILURE));
    ਵਾਪਸ old_v.ਸੈੱਟ_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> duplicate_subgraph (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗੰਢ_idx, hb_map_t& ਸੂਚਕਾਂਕ_map)
  {
    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਨਤੀਜਾ = Ok();
    TRY (traverse_directed_bfs (ਗੰਢ_idx, [&] (
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ,
      ਅਚਲ hb_serialize_context_t::object_t::link_t* link,
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ,
      ਬਿਨਾਨਿਸ਼ਾਨੀ _) {
      ਜੇ (ਨਤੀਜਾ.is_err () || ਸੂਚਕਾਂਕ_map.has (ਬੱਚਾ)) ਵਾਪਸ ਝੂਠ;
      ਸਵੈਚਾਲਿਤ r = duplicate (ਬੱਚਾ);
      ਜੇ (unlikely (r.is_err ()))
      {
        ਨਤੀਜਾ = Err(r.error ());
        ਵਾਪਸ ਝੂਠ;
      }
      ਸੂਚਕਾਂਕ_map.ਸੈੱਟ (ਬੱਚਾ, *r);
      ਵਾਪਸ ਸੱਚ;
    }));

    ਵਾਪਸ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from(ਸੂਚਕਾਂਕ_map, ਵੰਡ_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> duplicate (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗੰਢ_idx, ਤਾਰਕਿਕ copy_ਸਾਰਣੀ = ਝੂਠ)
  {
    ਜੇ (unlikely (ਗੰਢ_idx >= vertices_.ਲੰਮਾਈ))
      ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);

    ਜੇ (unlikely (vertices_.ਲੰਮਾਈ >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.ਲੰਮਾਈ);
      ਵਾਪਸ Err(ਹੱਦਬੰਦੀ_EXCEEDED);
    }

    positions_invalid = ਸੱਚ;
    ਦੂਰੀ_invalid = ਸੱਚ;

    ਸਵੈਚਾਲਿਤ* clone = vertices_.push ();
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (vertices_, ਵੰਡ_FAILURE));

    ਬਿਨਾਨਿਸ਼ਾਨੀ clone_idx = vertices_.ਲੰਮਾਈ - 1;
    ordering_.push (clone_idx);
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ordering_, ਵੰਡ_FAILURE));

    ਸਵੈਚਾਲਿਤ& ਬੱਚਾ = vertices_[ਗੰਢ_idx];

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਾਰਣੀ_ਮਾਪ = ਬੱਚਾ.obj().ਪੂਛ - ਬੱਚਾ.obj().ਸਿਰ_2;
    ਜੇ (copy_ਸਾਰਣੀ && ਸਾਰਣੀ_ਮਾਪ)
    {
      ਅੱਖਰ* ਬਫਰ = (ਅੱਖਰ*) hb_malloc (ਸਾਰਣੀ_ਮਾਪ);
      ਜੇ (unlikely (!ਬਫਰ))
        ਵਾਪਸ Err(ਵੰਡ_FAILURE);
      ਸਵੈਚਾਲਿਤ res = ਜੋੜੋ_ਬਫਰ (ਬਫਰ);
      ਜੇ (unlikely (res.is_err ()))
      {
        hb_free (ਬਫਰ);
        ਵਾਪਸ Err(res.error ());
      }
      hb_memcpy (ਬਫਰ, ਬੱਚਾ.obj().ਸਿਰ_2, ਸਾਰਣੀ_ਮਾਪ);
      clone->ਸੈੱਟ_ਬਫਰ(ਬਫਰ, ਬਫਰ + ਸਾਰਣੀ_ਮਾਪ);
    }
    ਨਹੀਂਤਾਂ
    {
      clone->ਸੈੱਟ_ਬਫਰ(ਬੱਚਾ.obj().ਸਿਰ_2, ਬੱਚਾ.obj().ਪੂਛ);
    }
    clone->distance = ਬੱਚਾ.distance;
    clone->space = ਬੱਚਾ.space;
    clone->reset_parents ();

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& l : ਬੱਚਾ.obj().real_links)
    {
      TRY(clone->ਜੋੜੋ_real_link (l));
      TRY(vertices_[l.objidx].ਜੋੜੋ_ਮਾਪਾ (clone_idx, ਝੂਠ));
    }

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& l : ਬੱਚਾ.obj().virtual_links)
    {
      TRY(clone->ਜੋੜੋ_virtual_link (l));
      TRY(vertices_[l.objidx].ਜੋੜੋ_ਮਾਪਾ (clone_idx, ਸੱਚ));
    }

    ਵਾਪਸ Ok(clone_idx);
  }

  /*
   * Creates a copy of child and re-assigns the link from
   * parent to the clone. The copy is a shallow copy, objects
   * linked from child are not duplicated.
   *
   * Returns the index of the newly created duplicate.
   *
   * If the child_idx only has incoming edges from parent_idx,
   * duplication isn't possible and this will return an error.
   */
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> duplicate (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ_idx, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ_idx, ਤਾਰਕਿਕ copy_ਸਾਰਣੀ = ਝੂਠ)
  {
    ਜੇ (unlikely (ਮਾਪਾ_idx >= vertices_.ਲੰਮਾਈ || ਬੱਚਾ_idx >= vertices_.ਲੰਮਾਈ))
      ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);

    TRY (update_parents ());

    ਅਚਲ ਸਵੈਚਾਲਿਤ& ਬੱਚਾ = vertices_[ਬੱਚਾ_idx];
    ਬਿਨਾਨਿਸ਼ਾਨੀ links_ਮੰਜ਼ਿਲ_ਬੱਚਾ = ਬੱਚਾ.incoming_ਕਿਨਾਰੇ_from_ਮਾਪਾ (ਮਾਪਾ_idx);

    ਜੇ (unlikely (ਬੱਚਾ.incoming_ਕਿਨਾਰੇ_2 () <= links_ਮੰਜ਼ਿਲ_ਬੱਚਾ || ਬੱਚਾ.has_incoming_virtual_ਕਿਨਾਰੇ_2 ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "  Not duplicating %u => %u",
                 ਮਾਪਾ_idx, ਬੱਚਾ_idx);
      ਵਾਪਸ Err(INVALID_ਦਲੀਲ);
    }

    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "  Duplicating %u => %u",
               ਮਾਪਾ_idx, ਬੱਚਾ_idx);

    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ clone_idx, duplicate (ਬੱਚਾ_idx, copy_ਸਾਰਣੀ));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    ਜੇ (ਮਾਪਾ_idx == clone_idx) ਮਾਪਾ_idx++;

    ਸਵੈਚਾਲਿਤ& ਮਾਪਾ = vertices_[ਮਾਪਾ_idx];
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_real = ਮਾਪਾ.obj().real_links.ਲੰਮਾਈ;
    ਲਈ (ਸਵੈਚਾਲਿਤ& l : ਮਾਪਾ.all_links_writer ())
    {
      ਗਿਣਤੀ_2++;
      ਜੇ (l.objidx != ਬੱਚਾ_idx)
        ਜਾਰੀ;

      TRY(reassign_link (l, ਮਾਪਾ_idx, clone_idx, ਗਿਣਤੀ_2 > ਅੰਕ_real));
    }

    ਵਾਪਸ Ok(clone_idx);
  }

  /*
   * Creates a copy of child and re-assigns the links from
   * parents to the clone. The copy is a shallow copy, objects
   * linked from child are not duplicated.
   *
   * Returns the index of the newly created duplicate.
   *
   * If the child_idx only has incoming edges from parents,
   * duplication isn't possible or duplication fails and this will
   * return an error.
   */
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> duplicate (ਅਚਲ hb_ਸੈੱਟ_t* parents, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ_idx)
  {
    ਜੇ (unlikely (!parents || parents->is_empty ()))
      ਵਾਪਸ Err(INVALID_ਦਲੀਲ);

    ਜੇ (unlikely (ਬੱਚਾ_idx >= vertices_.ਲੰਮਾਈ))
      ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);

    TRY (update_parents ());

    ਅਚਲ ਸਵੈਚਾਲਿਤ& ਬੱਚਾ = vertices_[ਬੱਚਾ_idx];
    ਬਿਨਾਨਿਸ਼ਾਨੀ links_ਮੰਜ਼ਿਲ_ਬੱਚਾ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਆਖਰੀ_ਮਾਪਾ = parents->get_max ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਹਿਲਾ_ਮਾਪਾ = parents->get_min ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ_idx : *parents) {
      ਜੇ (unlikely (ਮਾਪਾ_idx >= vertices_.ਲੰਮਾਈ))
        ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);
      links_ਮੰਜ਼ਿਲ_ਬੱਚਾ += ਬੱਚਾ.incoming_ਕਿਨਾਰੇ_from_ਮਾਪਾ (ਮਾਪਾ_idx);
    }

    ਜੇ (unlikely (ਬੱਚਾ.incoming_ਕਿਨਾਰੇ_2 () <= links_ਮੰਜ਼ਿਲ_ਬੱਚਾ || ਬੱਚਾ.has_incoming_virtual_ਕਿਨਾਰੇ_2 ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "  Not duplicating %u, ..., %u => %u", ਪਹਿਲਾ_ਮਾਪਾ, ਆਖਰੀ_ਮਾਪਾ, ਬੱਚਾ_idx);
      ਵਾਪਸ Err(INVALID_ਦਲੀਲ);
    }

    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "  Duplicating %u, ..., %u => %u", ਪਹਿਲਾ_ਮਾਪਾ, ਆਖਰੀ_ਮਾਪਾ, ਬੱਚਾ_idx);

    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ clone_idx, duplicate (ਬੱਚਾ_idx));

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      ਜੇ (ਮਾਪਾ_idx == clone_idx) ਮਾਪਾ_idx++;
      ਸਵੈਚਾਲਿਤ& ਮਾਪਾ = vertices_[ਮਾਪਾ_idx];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 0;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_real = ਮਾਪਾ.obj().real_links.ਲੰਮਾਈ;
      ਲਈ (ਸਵੈਚਾਲਿਤ& l : ਮਾਪਾ.all_links_writer ())
      {
        ਗਿਣਤੀ_2++;
        ਜੇ (l.objidx != ਬੱਚਾ_idx)
          ਜਾਰੀ;

        TRY(reassign_link (l, ਮਾਪਾ_idx, clone_idx, ਗਿਣਤੀ_2 > ਅੰਕ_real));
      }
    }

    ਵਾਪਸ Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> new_ਗੰਢ (ਅੱਖਰ* ਸਿਰ_2, ਅੱਖਰ* ਪੂਛ)
  {
    ਜੇ (unlikely (vertices_.ਲੰਮਾਈ >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.ਲੰਮਾਈ);
      ਵਾਪਸ Err(ਹੱਦਬੰਦੀ_EXCEEDED);
    }

    positions_invalid = ਸੱਚ;
    ਦੂਰੀ_invalid = ਸੱਚ;

    ਸਵੈਚਾਲਿਤ* clone = vertices_.push ();
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (vertices_, ਵੰਡ_FAILURE));

    ਬਿਨਾਨਿਸ਼ਾਨੀ clone_idx = vertices_.ਲੰਮਾਈ - 1;
    ordering_.push (clone_idx);
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ordering_, ਵੰਡ_FAILURE));

    clone->ਸੈੱਟ_ਬਫਰ (ਸਿਰ_2, ਪੂਛ);
    clone->distance = 0;
    clone->space = 0;

    ਵਾਪਸ Ok(clone_idx);
  }

  /*
   * Creates a new child node and remap the old child to it.
   *
   * Returns the index of the newly created child.
   *
   */
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> remap_ਬੱਚਾ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ_idx, ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਬੱਚਾ_idx)
  {
    ਜੇ (unlikely (ਮਾਪਾ_idx >= vertices_.ਲੰਮਾਈ || old_ਬੱਚਾ_idx >= vertices_.ਲੰਮਾਈ))
      ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);

    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਬੱਚਾ_idx, duplicate (old_ਬੱਚਾ_idx));

    ਸਵੈਚਾਲਿਤ& ਮਾਪਾ = vertices_[ਮਾਪਾ_idx];
    ਲਈ (ਸਵੈਚਾਲਿਤ& l : ਮਾਪਾ.real_links_writer ())
    {
      ਜੇ (l.objidx != old_ਬੱਚਾ_idx)
        ਜਾਰੀ;
      TRY(reassign_link (l, ਮਾਪਾ_idx, new_ਬੱਚਾ_idx, ਝੂਠ));
    }

    ਲਈ (ਸਵੈਚਾਲਿਤ& l : ਮਾਪਾ.virtual_links_writer ())
    {
      ਜੇ (l.objidx != old_ਬੱਚਾ_idx)
        ਜਾਰੀ;
      TRY(reassign_link (l, ਮਾਪਾ_idx, new_ਬੱਚਾ_idx, ਸੱਚ));
    }
    ਵਾਪਸ Ok(new_ਬੱਚਾ_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਤਾਰਕਿਕ> raise_childrens_ਤਰਜੀਹ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ_idx)
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "  Raising priority of all children of %u",
               ਮਾਪਾ_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    ਜੇ (unlikely (ਮਾਪਾ_idx >= vertices_.ਲੰਮਾਈ)) ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);
    ਤਾਰਕਿਕ made_change = ਝੂਠ;
    ਲਈ (ਸਵੈਚਾਲਿਤ& l : vertices_[ਮਾਪਾ_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_ਤਰਜੀਹ ();
    ਵਾਪਸ Ok(made_change);
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> is_fully_connected ()
  {
    TRY(update_parents());

    ਜੇ (unlikely (ਜੜ੍ਹ().incoming_ਕਿਨਾਰੇ_2 ()))
      // Root cannot have parents.
      ਵਾਪਸ Err(ORPHANED_NODES);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਜੜ੍ਹ_idx (); i++)
    {
      ਜੇ (unlikely (!vertices_[i].incoming_ਕਿਨਾਰੇ_2 ()))
        ਵਾਪਸ Err(ORPHANED_NODES);
    }
    ਵਾਪਸ Ok();
  }

#ਜੇ 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  ਖਾਲੀ save_fuzzer_ਬੀਜ (hb_ਟੈਗ_t ਟੈਗ_2) ਅਚਲ
  {
    FILE* f = fopen ("./repacker_fuzzer_seed", "w");
    fwrite ((ਖਾਲੀ*) &ਟੈਗ_2, ਆਕਾਰ (ਟੈਗ_2), 1, f);

    uint16_t ਅੰਕ_objects = vertices_.ਲੰਮਾਈ;
    fwrite ((ਖਾਲੀ*) &ਅੰਕ_objects, ਆਕਾਰ (ਅੰਕ_objects), 1, f);

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& v : vertices_)
    {
      uint16_t blob_ਮਾਪ = v.ਸਾਰਣੀ_ਮਾਪ ();
      fwrite ((ਖਾਲੀ*) &blob_ਮਾਪ, ਆਕਾਰ (blob_ਮਾਪ), 1, f);
      fwrite ((ਅਚਲ ਖਾਲੀ*) v.obj.ਸਿਰ_2, blob_ਮਾਪ, 1, f);
    }

    uint16_t link_ਗਿਣਤੀ = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& v : vertices_)
      link_ਗਿਣਤੀ += v.obj.real_links.ਲੰਮਾਈ;

    fwrite ((ਖਾਲੀ*) &link_ਗਿਣਤੀ, ਆਕਾਰ (link_ਗਿਣਤੀ), 1, f);

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
    {
      uint16_t ਮਾਪਾ;
      uint16_t ਬੱਚਾ;
      uint16_t position;
      uint8_t ਚੌੜਾਈ;
    } link_t;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < vertices_.ਲੰਮਾਈ; i++)
    {
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.ਚੌੜਾਈ
        };
        fwrite ((ਖਾਲੀ*) &link, ਆਕਾਰ (link), 1, f);
      }
    }

    fclose (f);
  }
#ਜੇ_ਅੰਤ

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_roots_for_space_2 (ਬਿਨਾਨਿਸ਼ਾਨੀ space) ਅਚਲ
  {
    ਵਾਪਸ ਅੰਕ_roots_for_space[space];
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਗਲਾ_space () ਅਚਲ
  {
    ਵਾਪਸ ਅੰਕ_roots_for_space.ਲੰਮਾਈ;
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> move_ਮੰਜ਼ਿਲ_new_space (ਅਚਲ hb_ਸੈੱਟ_t& indices)
  {
    ਜੇ (unlikely (ਅੰਕ_roots_for_space.ਲੰਮਾਈ >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 ਅੰਕ_roots_for_space.ਲੰਮਾਈ);
      ਵਾਪਸ Err(ਹੱਦਬੰਦੀ_EXCEEDED);
    }
    ਅੰਕ_roots_for_space.push (0);
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਅੰਕ_roots_for_space, ਵੰਡ_FAILURE));
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_space = ਅੰਕ_roots_for_space.ਲੰਮਾਈ - 1;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2 : indices) {
      ਜੇ (unlikely (ਸੂਚਕਾਂਕ_2 >= vertices_.ਲੰਮਾਈ))
        ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);
      ਸਵੈਚਾਲਿਤ& node = vertices_[ਸੂਚਕਾਂਕ_2];
      ਅੰਕ_roots_for_space[node.space] = ਅੰਕ_roots_for_space[node.space] - 1;
      ਅੰਕ_roots_for_space[new_space] = ਅੰਕ_roots_for_space[new_space] + 1;
      node.space = new_space;
      ਦੂਰੀ_invalid = ਸੱਚ;
      positions_invalid = ਸੱਚ;
    }
    ਵਾਪਸ Ok();
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ space_for (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ* ਜੜ੍ਹ = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜਦੋਂ (ਸੱਚ)
    {
      ਪ੍ਰਤਿਜ੍ਞਾ (ਸੂਚਕਾਂਕ_2 < vertices_.ਲੰਮਾਈ);
      ਅਚਲ ਸਵੈਚਾਲਿਤ& node = vertices_[ਸੂਚਕਾਂਕ_2];
      ਜੇ (node.space)
      {
        ਜੇ (ਜੜ੍ਹ != ਖਾਲੀ_ਸੂਚਕ)
          *ਜੜ੍ਹ = ਸੂਚਕਾਂਕ_2;
        ਵਾਪਸ node.space;
      }

      ਜੇ (!node.incoming_ਕਿਨਾਰੇ_2 ())
      {
        ਜੇ (ਜੜ੍ਹ)
          *ਜੜ੍ਹ = ਸੂਚਕਾਂਕ_2;
        ਵਾਪਸ 0;
      }

      ਸੂਚਕਾਂਕ_2 = *node.parents_iter ();
    }
  }

  size_t ਕੁੱਲ_ਮਾਪ_ਵਿੱਚ_ਬਾਈਟਾਂ () ਅਚਲ {
    size_t ਕੁੱਲ_ਮਾਪ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = vertices_.ਲੰਮਾਈ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++) {
      size_t ਮਾਪ = vertices_.arrayZ[i].ਸਾਰਣੀ_ਮਾਪ ();
      ਕੁੱਲ_ਮਾਪ += ਮਾਪ;
    }
    ਵਾਪਸ ਕੁੱਲ_ਮਾਪ;
  }


 ਨਿਜੀ:

  ਖਾਲੀ print_orphaned_nodes ()
  {
    ਜੇ (!DEBUG_ENABLED(SUBSET_REPACK)) ਵਾਪਸ;

    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "Graph is not fully connected.");

    parents_invalid = ਸੱਚ;
    (ਖਾਲੀ) update_parents();

    ਜੇ (ਜੜ੍ਹ().incoming_ਕਿਨਾਰੇ_2 ()) {
      DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "Root node has incoming edges.");
    }

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < vertices_.ਲੰਮਾਈ; i++)
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ& v = vertices_[i];
      ਜੇ (!v.incoming_ਕਿਨਾਰੇ_2 ())
        DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  ਬਿਨਾਨਿਸ਼ਾਨੀ wide_parents (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗੰਢ_idx, hb_ਸੈੱਟ_t& parents) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ p : vertices_[ਗੰਢ_idx].parents_iter ())
    {
      // Only real links can be wide
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& l : vertices_[p].obj().real_links)
      {
        ਜੇ (l.objidx == ਗੰਢ_idx
            && (l.ਚੌੜਾਈ == 3 || l.ਚੌੜਾਈ == 4)
            && !l.is_signed)
        {
          ਗਿਣਤੀ_2++;
          parents.ਜੋੜੋ (p);
        }
      }
    }
    ਵਾਪਸ ਗਿਣਤੀ_2;
  }

 ਜਨਤਕ:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> update_parents ()
  {
    ਜੇ (!parents_invalid) ਵਾਪਸ Ok();

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = vertices_.ਲੰਮਾਈ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      vertices_.arrayZ[i].reset_parents ();

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ p = 0; p < ਗਿਣਤੀ_2; p++)
    {
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& l : vertices_.arrayZ[p].obj().real_links)
      {
        ਜੇ (unlikely (l.objidx >= ਗਿਣਤੀ_2)) ਵਾਪਸ Err(INVALID_ਦਲੀਲ);
        TRY(vertices_[l.objidx].ਜੋੜੋ_ਮਾਪਾ (p, ਝੂਠ));
      }

      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        ਜੇ (unlikely (l.objidx >= ਗਿਣਤੀ_2)) ਵਾਪਸ Err(INVALID_ਦਲੀਲ);
        TRY(vertices_[l.objidx].ਜੋੜੋ_ਮਾਪਾ (p, ਸੱਚ));
      }
    }

    parents_invalid = ਝੂਠ;
    ਵਾਪਸ Ok();
  }

  /*
   * compute the serialized start and end positions for each vertex.
   */
  ਖਾਲੀ update_positions ()
  {
    ਜੇ (!positions_invalid) ਵਾਪਸ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ current_pos = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i : ordering_)
    {
      ਸਵੈਚਾਲਿਤ& v = vertices_[i];
      v.ਸ਼ੁਰੂ = current_pos;
      current_pos += v.obj().ਪੂਛ - v.obj().ਸਿਰ_2;
      v.ਸਿਰਾ = current_pos;
    }

    positions_invalid = ਝੂਠ;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> update_distances ()
  {
    ਜੇ (!ਦੂਰੀ_invalid) ਵਾਪਸ Ok();

    // Uses Dijkstra's algorithm to find all of the shortest distances.
    // https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
    //
    // Implementation Note:
    // Since our priority queue doesn't support fast priority decreases
    // we instead just add new entries into the queue when a priority changes.
    // Redundant ones are filtered out later on by the visited set.
    // According to https://www3.cs.stonybrook.edu/~rezaul/papers/TR-07-54.pdf
    // for practical performance this is faster then using a more advanced queue
    // (such as a fibonacci queue) with a fast decrease priority.
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = vertices_.ਲੰਮਾਈ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[ਜੜ੍ਹ_idx ()].distance = 0;

    hb_ਤਰਜੀਹ_ਕਤਾਰ_t<int64_t> ਕਤਾਰ;
    ਕਤਾਰ.alloc (ਗਿਣਤੀ_2);
    ਕਤਾਰ.insert (0, ਜੜ੍ਹ_idx ());
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਕਤਾਰ, ਵੰਡ_FAILURE));

    hb_ਸਦਿਸ਼_t<ਤਾਰਕਿਕ> ਦੇਖਿਆ;
    ਦੇਖਿਆ.resize (vertices_.ਲੰਮਾਈ);
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਦੇਖਿਆ, ਵੰਡ_FAILURE));

    ਜਦੋਂ (!ਕਤਾਰ.ਵਿੱਚ_error () && !ਕਤਾਰ.is_empty ())
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਗਲਾ_idx = ਕਤਾਰ.ਕੱਢੋ_minimum ().second;
      ਜੇ (ਦੇਖਿਆ[ਅਗਲਾ_idx]) ਜਾਰੀ;
      ਅਚਲ ਸਵੈਚਾਲਿਤ& next = vertices_[ਅਗਲਾ_idx];
      int64_t ਅਗਲਾ_ਦੂਰੀ = next.distance;
      ਦੇਖਿਆ[ਅਗਲਾ_idx] = ਸੱਚ;

      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& link : next.obj().all_links ())
      {
        ਜੇ (unlikely (link.objidx >= vertices_.ਲੰਮਾਈ))
          ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);

        ਜੇ (ਦੇਖਿਆ[link.objidx]) ਜਾਰੀ;

        ਸਵੈਚਾਲਿਤ& ਬੱਚਾ_v = vertices_.arrayZ[link.objidx];
        ਅਚਲ ਸਵੈਚਾਲਿਤ& ਬੱਚਾ = ਬੱਚਾ_v.obj ();
        ਬਿਨਾਨਿਸ਼ਾਨੀ link_ਚੌੜਾਈ = link.ਚੌੜਾਈ ? link.ਚੌੜਾਈ : 4; // treat virtual offsets as 32 bits wide
        int64_t ਬੱਚਾ_ਭਾਰ = (ਬੱਚਾ.ਪੂਛ - ਬੱਚਾ.ਸਿਰ_2) +
                               ((int64_t) 1 << (link_ਚੌੜਾਈ * 8)) * (ਬੱਚਾ_v.space + 1);
        int64_t ਬੱਚਾ_ਦੂਰੀ = ਅਗਲਾ_ਦੂਰੀ + ਬੱਚਾ_ਭਾਰ;

        ਜੇ (ਬੱਚਾ_ਦੂਰੀ < ਬੱਚਾ_v.distance)
        {
          ਬੱਚਾ_v.distance = ਬੱਚਾ_ਦੂਰੀ;
          ਕਤਾਰ.insert (ਬੱਚਾ_ਦੂਰੀ, link.objidx);
        }
      }
    }

    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਕਤਾਰ, ਵੰਡ_FAILURE));
    ਜੇ (unlikely (!ਕਤਾਰ.is_empty ()))
    {
      print_orphaned_nodes ();
      ਵਾਪਸ Err(ORPHANED_NODES);
    }

    ਦੂਰੀ_invalid = ਝੂਠ;
    ਵਾਪਸ Ok();
  }

 ਨਿਜੀ:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ_idx,
                      ਬਿਨਾਨਿਸ਼ਾਨੀ new_idx,
                      ਤਾਰਕਿਕ is_virtual)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_ਮਾਪਾ (ਮਾਪਾ_idx);
    ਵਾਪਸ vertices_[new_idx].ਜੋੜੋ_ਮਾਪਾ (ਮਾਪਾ_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator, hb_requires (hb_is_iterator (Iterator))>
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> remap_obj_indices (ਅਚਲ hb_map_t& id_map,
                                          Iterator subgraph,
                                          ਤਾਰਕਿਕ only_wide = ਝੂਠ)
  {
    ਜੇ (!id_map) ਵਾਪਸ Ok();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i : subgraph)
    {
      ਸਵੈਚਾਲਿਤ& ਸ਼ੀਰਸ਼ = vertices_[i];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_real = ਸ਼ੀਰਸ਼.obj().real_links.ਲੰਮਾਈ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 0;
      ਲਈ (ਸਵੈਚਾਲਿਤ& link : ਸ਼ੀਰਸ਼.all_links_writer ())
      {
        ਗਿਣਤੀ_2++;
        ਅਚਲ uint32_t *v;
        ਜੇ (!id_map.has (link.objidx, &v)) ਜਾਰੀ;
        ਜੇ (only_wide && (link.is_signed || (link.ਚੌੜਾਈ != 4 && link.ਚੌੜਾਈ != 3))) ਜਾਰੀ;

        TRY(reassign_link (link, i, *v, ਗਿਣਤੀ_2 > ਅੰਕ_real));
      }
    }
    ਵਾਪਸ Ok();
  }

  /*
   * Finds all nodes in targets that are reachable from start_idx, nodes in visited will be skipped.
   * For this search the graph is treated as being undirected.
   *
   * Connected targets will be added to connected and removed from targets. All visited nodes
   * will be added to visited.
   */
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਲੱਭੋ_connected_nodes (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_idx,
                                             hb_ਸੈੱਟ_t& targets,
                                             hb_ਸੈੱਟ_t& ਦੇਖਿਆ,
                                             hb_ਸੈੱਟ_t& connected)
  {
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਦੇਖਿਆ, ਵੰਡ_FAILURE));
    ਜੇ (ਦੇਖਿਆ.has (ਸ਼ੁਰੂ_idx)) ਵਾਪਸ Ok();
    ਜੇ (unlikely (ਸ਼ੁਰੂ_idx >= vertices_.ਲੰਮਾਈ))
    {
      DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      ਵਾਪਸ Err(ਬਾਹਰਲਾ_ਦਾ_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.ਲੰਮਾਈ);
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ordering_scratch_, ਵੰਡ_FAILURE));

    ਸਵੈਚਾਲਿਤ& ਢੇਰ = ordering_scratch_;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਢੇਰ_len = 0;

    ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> res = Ok();
    ਸਵੈਚਾਲਿਤ handle_ਗੰਢ = [&] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗੰਢ_idx) {
      ਦੇਖਿਆ.ਜੋੜੋ (ਗੰਢ_idx);
      ਜੇ (targets.has (ਗੰਢ_idx)) {
        targets.del (ਗੰਢ_idx);
        connected.ਜੋੜੋ (ਗੰਢ_idx);
      }

      ਜੇ (unlikely (ਢੇਰ_len >= ਢੇਰ.ਲੰਮਾਈ))
      {
        res = Err(ਚੱਕਰ_DETECTED);
        ਵਾਪਸ ਝੂਠ;
      }
      ਢੇਰ[ਢੇਰ_len++] = ਗੰਢ_idx;
      ਵਾਪਸ ਸੱਚ;
    };

    ਜੇ (!handle_ਗੰਢ (ਸ਼ੁਰੂ_idx)) ਵਾਪਸ res;

    ਜਦੋਂ (ਢੇਰ_len > 0)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗੰਢ_idx = ਢੇਰ[--ਢੇਰ_len];
      ਅਚਲ ਸਵੈਚਾਲਿਤ& v = vertices_[ਗੰਢ_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& l : v.obj().all_links ())
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ_idx = l.objidx;
        ਜੇ (ਦੇਖਿਆ.has (ਬੱਚਾ_idx)) ਜਾਰੀ;
        ਜੇ (!handle_ਗੰਢ (ਬੱਚਾ_idx)) ਵਾਪਸ res;
      }

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ_idx : v.parents_iter ())
      {
        ਜੇ (ਦੇਖਿਆ.has (ਮਾਪਾ_idx)) ਜਾਰੀ;
        ਜੇ (!handle_ਗੰਢ (ਮਾਪਾ_idx)) ਵਾਪਸ res;
      }
    }

    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਦੇਖਿਆ, ਵੰਡ_FAILURE));
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (connected, ਵੰਡ_FAILURE));

    ਵਾਪਸ Ok();
  }

 ਜਨਤਕ:
  // TODO(garretrieger): make private, will need to move most of offset overflow code into graph.
  hb_ਸਦਿਸ਼_t<ਸ਼ੀਰਸ਼_t> vertices_;

  // Specifies the current topological ordering of this graph
  //
  // ordering_[pos] = obj index
  //
  // specifies that the 'pos'th spot is filled by the object
  // given by obj index.
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ordering_;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ordering_scratch_;

 ਨਿਜੀ:
  ਤਾਰਕਿਕ parents_invalid = ਸੱਚ;
  ਤਾਰਕਿਕ ਦੂਰੀ_invalid = ਸੱਚ;
  ਤਾਰਕਿਕ positions_invalid = ਸੱਚ;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਅੰਕ_roots_for_space;
  hb_ਸਦਿਸ਼_t<ਅੱਖਰ*> buffers;
};

}

#ਜੇ_ਅੰਤ  // GRAPH_GRAPH_HH
