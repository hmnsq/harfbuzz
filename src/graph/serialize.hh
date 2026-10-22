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

#ifndef GRAPH_SERIALIZE_HH
#define GRAPH_SERIALIZE_HH

#ಅನ್ತರ್ಭಾವ "graph-result.hh"

ನಾಮಾಕಾಶ graph {

ಸಂರಚನಾ overflow_record_t
{
  unsigned parent;
  unsigned child;

  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ overflow_record_t o) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ !(*ಏತತ್ == o); }

  ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ operator == (ಧ್ರುವ overflow_record_t& o) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ parent == o.parent &&
        child == o.child;
  }

  ಅನ್ತಃಸ್ಥ uint32_t hash () ಧ್ರುವ
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (parent);
    current = current * 31 + hb_hash (child);
    ಪ್ರತ್ಯಾಗಮ current;
  }
};

ಅನ್ತಃಸ್ಥ
int64_t compute_offset (
    ಧ್ರುವ graph_t& graph,
    unsigned parent_idx,
    ಧ್ರುವ hb_serialize_context_t::object_t::link_t& link)
{
  ಧ್ರುವ auto& parent = graph.vertices_[parent_idx];
  ಧ್ರುವ auto& child = graph.vertices_[link.objidx];
  int64_t offset = 0;
  ವಿಕಲ್ಪ ((hb_serialize_context_t::whence_t) link.whence) {
    ಪಕ್ಷ hb_serialize_context_t::whence_t::Head:
      offset = child.start - parent.start; ಭಙ್ಗ;
    ಪಕ್ಷ hb_serialize_context_t::whence_t::Tail:
      offset = child.start - parent.end; ಭಙ್ಗ;
    ಪಕ್ಷ hb_serialize_context_t::whence_t::Absolute:
      offset = child.start; ಭಙ್ಗ;
  }

  ಪ್ರತಿಜ್ಞಾ (offset >= link.bias);
  offset -= link.bias;
  ಪ್ರತ್ಯಾಗಮ offset;
}

ಅನ್ತಃಸ್ಥ
ತಾರ್ಕಿಕ is_valid_offset (int64_t offset,
                      ಧ್ರುವ hb_serialize_context_t::object_t::link_t& link)
{
  ಯದಿ (unlikely (!link.width))
    // Virtual links can't overflow.
    ಪ್ರತ್ಯಾಗಮ link.is_signed || offset >= 0;

  ಯದಿ (link.is_signed)
  {
    ಯದಿ (link.width == 4)
      ಪ್ರತ್ಯಾಗಮ offset >= -((int64_t) 1 << 31) && offset < ((int64_t) 1 << 31);
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ offset >= -(1 << 15) && offset < (1 << 15);
  }
  ಅನ್ಯಥಾ
  {
    ಯದಿ (link.width == 4)
      ಪ್ರತ್ಯಾಗಮ offset >= 0 && offset < ((int64_t) 1 << 32);
    ಅನ್ಯಥಾ ಯದಿ (link.width == 3)
      ಪ್ರತ್ಯಾಗಮ offset >= 0 && offset < ((int32_t) 1 << 24);
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ offset >= 0 && offset < (1 << 16);
  }
}

/*
 * Will any offsets overflow on graph when it's serialized?
 */
ಅನ್ತಃಸ್ಥ graph_result_t<ತಾರ್ಕಿಕ>
will_overflow (graph_t& graph,
               hb_vector_t<overflow_record_t>* overflows = nullptr)
{
  ಯದಿ (overflows) overflows->clear ();
  graph.update_positions ();

  hb_hashmap_t<overflow_record_t, ತಾರ್ಕಿಕ> record_set;
  ಧ್ರುವ auto& vertices = graph.vertices_;
  ಪ್ರತಿ (unsigned parent_idx : graph.ordering_)
  {
    // Don't need to check virtual links for overflow
    ಪ್ರತಿ (ಧ್ರುವ auto& link : vertices.arrayZ[parent_idx].obj ().real_links)
    {
      int64_t offset = compute_offset (graph, parent_idx, link);
      ಯದಿ (likely (is_valid_offset (offset, link)))
        ಅನುವರ್ತ;

      ಯದಿ (!overflows) ಪ್ರತ್ಯಾಗಮ Ok(ಸತ್ಯ);

      overflow_record_t r;
      r.parent = parent_idx;
      r.child = link.objidx;
      ಯದಿ (record_set.has(r)) ಅನುವರ್ತ; // don't keep duplicate overflows.

      overflows->push (r);
      record_set.set (r, ಸತ್ಯ);
    }
  }

  TRY (graph_result_t<ಶೂನ್ಯ>::from (record_set, ALLOCATION_FAILURE));
  ಯದಿ (overflows)
    TRY (graph_result_t<ಶೂನ್ಯ>::from (*overflows, ALLOCATION_FAILURE));

  ಪ್ರತ್ಯಾಗಮ Ok((ತಾರ್ಕಿಕ) (overflows && overflows->length));
}

ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ print_overflows (graph_t& graph,
                      ಧ್ರುವ hb_vector_t<overflow_record_t>& overflows)
{
  ಯದಿ (!DEBUG_ENABLED(SUBSET_REPACK)) ಪ್ರತ್ಯಾಗಮ;

  auto r = graph.update_parents ();
  ಯದಿ (r.is_err()) {
    DEBUG_ಸಂದೇಶ (SUBSET_REPACK, nullptr, "Unable to print overflows due to error updating parent links: %s",
      to_string(r.error()));
    ಪ್ರತ್ಯಾಗಮ;
  }

  ಪೂರ್ಣಾಙ್ಕ limit = 10;
  ಪ್ರತಿ (ಧ್ರುವ auto& o : overflows)
  {
    ಯದಿ (!limit--) ಭಙ್ಗ;
    ಧ್ರುವ auto& parent = graph.vertices_[o.parent];
    ಧ್ರುವ auto& child = graph.vertices_[o.child];
    DEBUG_ಸಂದೇಶ (SUBSET_REPACK, nullptr,
               "  overflow from "
               "%4u (%4u in, %4u out, space %2u) => "
               "%4u (%4u in, %4u out, space %2u)",
               o.parent,
               parent.incoming_edges (),
               parent.obj ().real_links.length + parent.obj ().virtual_links.length,
               graph.space_for (o.parent),
               o.child,
               child.incoming_edges (),
               child.obj ().real_links.length + child.obj ().virtual_links.length,
               graph.space_for (o.child));
  }
  ಯದಿ (overflows.length > 10) {
    DEBUG_ಸಂದೇಶ (SUBSET_REPACK, nullptr, "  ... plus %u more overflows.", overflows.length - 10);
  }
}

template <typename O> ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
serialize_link_of_type (ಧ್ರುವ hb_serialize_context_t::object_t::link_t& link,
                        ವರ್ಣ* head,
                        unsigned size,
                        ಧ್ರುವ hb_vector_t<unsigned>& id_map,
                        hb_serialize_context_t* c)
{
  // A link must never write outside the bounds of its parent object.
  ಯದಿ (unlikely (link.position + link.width > size))
  {
    c->err (HB_SERIALIZE_ERROR_OTHER);
    ಪ್ರತ್ಯಾಗಮ;
  }

  OT::Offset<O>* offset = reinterpret_cast<OT::Offset<O>*> (head + link.position);
  *offset = 0;
  c->add_link (*offset,
               id_map[link.objidx],
               (hb_serialize_context_t::whence_t) link.whence,
               link.bias);
}

ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ serialize_link (ಧ್ರುವ hb_serialize_context_t::object_t::link_t& link,
                     ವರ್ಣ* head,
                     unsigned size,
                     ಧ್ರುವ hb_vector_t<unsigned>& id_map,
                     hb_serialize_context_t* c)
{
  ವಿಕಲ್ಪ (link.width)
  {
    ಪಕ್ಷ 0:
      // Virtual links aren't serialized.
      ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 4:
      ಯದಿ (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT32> (link, head, size, id_map, c);
      } ಅನ್ಯಥಾ {
        serialize_link_of_type<OT::HBUINT32> (link, head, size, id_map, c);
      }
      ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 2:
      ಯದಿ (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT16> (link, head, size, id_map, c);
      } ಅನ್ಯಥಾ {
        serialize_link_of_type<OT::HBUINT16> (link, head, size, id_map, c);
      }
      ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 3:
      serialize_link_of_type<OT::HBUINT24> (link, head, size, id_map, c);
      ಪ್ರತ್ಯಾಗಮ;
    ಸ್ವಭಾವ:
      // Unexpected link width.
      ಪ್ರತಿಜ್ಞಾ (0);
  }
}

/*
 * serialize graph into the provided serialization buffer.
 */
ಅನ್ತಃಸ್ಥ graph_result_t<hb_blob_t*> serialize (ಧ್ರುವ graph_t& graph)
{
  hb_vector_t<ವರ್ಣ> buffer;
  size_t size = graph.total_size_in_bytes ();

  ಯದಿ (!size) ಪ್ರತ್ಯಾಗಮ Ok(hb_blob_get_empty ());

  ಯದಿ (unlikely (!buffer.alloc (size))) {
    DEBUG_ಸಂದೇಶ (SUBSET_REPACK, nullptr, "Unable to allocate output buffer.");
    ಪ್ರತ್ಯಾಗಮ Err(ALLOCATION_FAILURE);
  }
  hb_serialize_context_t c((ಶೂನ್ಯ *) buffer, size);

  c.start_serialize<ಶೂನ್ಯ> ();
  ಧ್ರುವ auto& vertices = graph.vertices_;

  // Objects are placed in the serializer in reverse order since children need
  // to be inserted before their parents.

  // Maps from our obj id's to the id's used during this serialization.
  hb_vector_t<unsigned> id_map;
  ಯದಿ (unlikely (!id_map.resize(graph.ordering_.length))) {
    DEBUG_ಸಂದೇಶ (SUBSET_REPACK, nullptr, "Unable to allocate id_map buffer.");
    ಪ್ರತ್ಯಾಗಮ Err(ALLOCATION_FAILURE);
  }

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ pos = graph.ordering_.length - 1; pos >= 0; pos--) {
    unsigned i = graph.ordering_[pos];
    c.push ();

    auto& v = vertices[i];

    size_t size = v.table_size ();

    ವರ್ಣ* start = c.allocate_size <ವರ್ಣ> (size);
    ಯದಿ (unlikely (!start)) {
      DEBUG_ಸಂದೇಶ (SUBSET_REPACK, nullptr, "Buffer out of space.");
      ಪ್ರತ್ಯಾಗಮ Err(ALLOCATION_FAILURE);
    }

    hb_memcpy (start, v.obj ().head, size);

    // Only real links needs to be serialized.
    ಪ್ರತಿ (ಧ್ರುವ auto& link : v.obj ().real_links)
      serialize_link (link, start, size, id_map, &c);

    // All duplications are already encoded in the graph, so don't
    // enable sharing during packing.
    id_map[i] = c.ತೆಗೆ_pack (ಅಸತ್ಯ);
  }
  c.end_serialize ();

  ಯದಿ (unlikely (c.in_error ())) {
    DEBUG_ಸಂದೇಶ (SUBSET_REPACK, nullptr, "Error during serialization. Err flag: %d",
               c.errors);
    ಯದಿ (c.errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)
      ಪ್ರತ್ಯಾಗಮ Err(OVERFLOW_RESOLUTION_FAILED);
    ಪ್ರತ್ಯಾಗಮ Err(ALLOCATION_FAILURE);
  }

  auto* blob = c.copy_blob ();
  ಯದಿ (unlikely (!blob)) ಪ್ರತ್ಯಾಗಮ Err(ALLOCATION_FAILURE);
  ಪ್ರತ್ಯಾಗಮ Ok(blob);
}

} // namespace graph

#endif // GRAPH_SERIALIZE_HH
