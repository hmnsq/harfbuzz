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

#अन्तर्भाव "graph-result.hh"

नामाकाश graph {

संरचना overflow_record_t
{
  unsigned parent;
  unsigned child;

  तार्किक operator != (ध्रुव overflow_record_t o) ध्रुव
  { पटिनिवत्त !(*एतत् == o); }

  अन्तःस्थ तार्किक operator == (ध्रुव overflow_record_t& o) ध्रुव
  {
    पटिनिवत्त parent == o.parent &&
        child == o.child;
  }

  अन्तःस्थ uint32_t hash () ध्रुव
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (parent);
    current = current * 31 + hb_hash (child);
    पटिनिवत्त current;
  }
};

अन्तःस्थ
int64_t compute_offset (
    ध्रुव graph_t& graph,
    unsigned parent_idx,
    ध्रुव hb_serialize_context_t::object_t::link_t& link)
{
  ध्रुव auto& parent = graph.vertices_[parent_idx];
  ध्रुव auto& child = graph.vertices_[link.objidx];
  int64_t offset = 0;
  विकल्प ((hb_serialize_context_t::whence_t) link.whence) {
    पक्ष hb_serialize_context_t::whence_t::Head:
      offset = child.start - parent.start; भङ्ग;
    पक्ष hb_serialize_context_t::whence_t::Tail:
      offset = child.start - parent.end; भङ्ग;
    पक्ष hb_serialize_context_t::whence_t::Absolute:
      offset = child.start; भङ्ग;
  }

  प्रतिज्ञा (offset >= link.bias);
  offset -= link.bias;
  पटिनिवत्त offset;
}

अन्तःस्थ
तार्किक is_valid_offset (int64_t offset,
                      ध्रुव hb_serialize_context_t::object_t::link_t& link)
{
  सचे (unlikely (!link.width))
    // Virtual links can't overflow.
    पटिनिवत्त link.is_signed || offset >= 0;

  सचे (link.is_signed)
  {
    सचे (link.width == 4)
      पटिनिवत्त offset >= -((int64_t) 1 << 31) && offset < ((int64_t) 1 << 31);
    अञ्ञथा
      पटिनिवत्त offset >= -(1 << 15) && offset < (1 << 15);
  }
  अञ्ञथा
  {
    सचे (link.width == 4)
      पटिनिवत्त offset >= 0 && offset < ((int64_t) 1 << 32);
    अञ्ञथा सचे (link.width == 3)
      पटिनिवत्त offset >= 0 && offset < ((int32_t) 1 << 24);
    अञ्ञथा
      पटिनिवत्त offset >= 0 && offset < (1 << 16);
  }
}

/*
 * Will any offsets overflow on graph when it's serialized?
 */
अन्तःस्थ graph_result_t<तार्किक>
will_overflow (graph_t& graph,
               hb_vector_t<overflow_record_t>* overflows = nullptr)
{
  सचे (overflows) overflows->clear ();
  graph.update_positions ();

  hb_hashmap_t<overflow_record_t, तार्किक> record_set;
  ध्रुव auto& vertices = graph.vertices_;
  अत्थाय (unsigned parent_idx : graph.ordering_)
  {
    // Don't need to check virtual links for overflow
    अत्थाय (ध्रुव auto& link : vertices.arrayZ[parent_idx].obj ().real_links)
    {
      int64_t offset = compute_offset (graph, parent_idx, link);
      सचे (likely (is_valid_offset (offset, link)))
        अनुवर्त;

      सचे (!overflows) पटिनिवत्त Ok(सच्च);

      overflow_record_t r;
      r.parent = parent_idx;
      r.child = link.objidx;
      सचे (record_set.has(r)) अनुवर्त; // don't keep duplicate overflows.

      overflows->push (r);
      record_set.set (r, सच्च);
    }
  }

  TRY (graph_result_t<शून्य>::from (record_set, ALLOCATION_FAILURE));
  सचे (overflows)
    TRY (graph_result_t<शून्य>::from (*overflows, ALLOCATION_FAILURE));

  पटिनिवत्त Ok((तार्किक) (overflows && overflows->length));
}

अन्तःस्थ
शून्य print_overflows (graph_t& graph,
                      ध्रुव hb_vector_t<overflow_record_t>& overflows)
{
  सचे (!DEBUG_ENABLED(SUBSET_REPACK)) पटिनिवत्त;

  auto r = graph.update_parents ();
  सचे (r.is_err()) {
    DEBUG_सन्देशः (SUBSET_REPACK, nullptr, "Unable to print overflows due to error updating parent links: %s",
      to_string(r.error()));
    पटिनिवत्त;
  }

  पूर्णाङ्क limit = 10;
  अत्थाय (ध्रुव auto& o : overflows)
  {
    सचे (!limit--) भङ्ग;
    ध्रुव auto& parent = graph.vertices_[o.parent];
    ध्रुव auto& child = graph.vertices_[o.child];
    DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
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
  सचे (overflows.length > 10) {
    DEBUG_सन्देशः (SUBSET_REPACK, nullptr, "  ... plus %u more overflows.", overflows.length - 10);
  }
}

template <typename O> अन्तःस्थ शून्य
serialize_link_of_type (ध्रुव hb_serialize_context_t::object_t::link_t& link,
                        वर्ण* head,
                        unsigned size,
                        ध्रुव hb_vector_t<unsigned>& id_map,
                        hb_serialize_context_t* c)
{
  // A link must never write outside the bounds of its parent object.
  सचे (unlikely (link.position + link.width > size))
  {
    c->err (HB_SERIALIZE_ERROR_OTHER);
    पटिनिवत्त;
  }

  OT::Offset<O>* offset = reinterpret_cast<OT::Offset<O>*> (head + link.position);
  *offset = 0;
  c->add_link (*offset,
               id_map[link.objidx],
               (hb_serialize_context_t::whence_t) link.whence,
               link.bias);
}

अन्तःस्थ
शून्य serialize_link (ध्रुव hb_serialize_context_t::object_t::link_t& link,
                     वर्ण* head,
                     unsigned size,
                     ध्रुव hb_vector_t<unsigned>& id_map,
                     hb_serialize_context_t* c)
{
  विकल्प (link.width)
  {
    पक्ष 0:
      // Virtual links aren't serialized.
      पटिनिवत्त;
    पक्ष 4:
      सचे (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT32> (link, head, size, id_map, c);
      } अञ्ञथा {
        serialize_link_of_type<OT::HBUINT32> (link, head, size, id_map, c);
      }
      पटिनिवत्त;
    पक्ष 2:
      सचे (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT16> (link, head, size, id_map, c);
      } अञ्ञथा {
        serialize_link_of_type<OT::HBUINT16> (link, head, size, id_map, c);
      }
      पटिनिवत्त;
    पक्ष 3:
      serialize_link_of_type<OT::HBUINT24> (link, head, size, id_map, c);
      पटिनिवत्त;
    स्वभाव:
      // Unexpected link width.
      प्रतिज्ञा (0);
  }
}

/*
 * serialize graph into the provided serialization buffer.
 */
अन्तःस्थ graph_result_t<hb_blob_t*> serialize (ध्रुव graph_t& graph)
{
  hb_vector_t<वर्ण> buffer;
  size_t size = graph.total_size_in_bytes ();

  सचे (!size) पटिनिवत्त Ok(hb_blob_get_empty ());

  सचे (unlikely (!buffer.alloc (size))) {
    DEBUG_सन्देशः (SUBSET_REPACK, nullptr, "Unable to allocate output buffer.");
    पटिनिवत्त Err(ALLOCATION_FAILURE);
  }
  hb_serialize_context_t c((शून्य *) buffer, size);

  c.start_serialize<शून्य> ();
  ध्रुव auto& vertices = graph.vertices_;

  // Objects are placed in the serializer in reverse order since children need
  // to be inserted before their parents.

  // Maps from our obj id's to the id's used during this serialization.
  hb_vector_t<unsigned> id_map;
  सचे (unlikely (!id_map.resize(graph.ordering_.length))) {
    DEBUG_सन्देशः (SUBSET_REPACK, nullptr, "Unable to allocate id_map buffer.");
    पटिनिवत्त Err(ALLOCATION_FAILURE);
  }

  अत्थाय (पूर्णाङ्क pos = graph.ordering_.length - 1; pos >= 0; pos--) {
    unsigned i = graph.ordering_[pos];
    c.push ();

    auto& v = vertices[i];

    size_t size = v.table_size ();

    वर्ण* start = c.allocate_size <वर्ण> (size);
    सचे (unlikely (!start)) {
      DEBUG_सन्देशः (SUBSET_REPACK, nullptr, "Buffer out of space.");
      पटिनिवत्त Err(ALLOCATION_FAILURE);
    }

    hb_memcpy (start, v.obj ().head, size);

    // Only real links needs to be serialized.
    अत्थाय (ध्रुव auto& link : v.obj ().real_links)
      serialize_link (link, start, size, id_map, &c);

    // All duplications are already encoded in the graph, so don't
    // enable sharing during packing.
    id_map[i] = c.निष्कासय_pack (मिच्छा);
  }
  c.end_serialize ();

  सचे (unlikely (c.in_error ())) {
    DEBUG_सन्देशः (SUBSET_REPACK, nullptr, "Error during serialization. Err flag: %d",
               c.errors);
    सचे (c.errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)
      पटिनिवत्त Err(OVERFLOW_RESOLUTION_FAILED);
    पटिनिवत्त Err(ALLOCATION_FAILURE);
  }

  auto* blob = c.copy_blob ();
  सचे (unlikely (!blob)) पटिनिवत्त Err(ALLOCATION_FAILURE);
  पटिनिवत्त Ok(blob);
}

} // namespace graph

#endif // GRAPH_SERIALIZE_HH
