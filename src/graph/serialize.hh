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

#অন্তর্ভাব "graph-result.hh"

নামাকাশ graph {

সংরচনা overflow_record_t
{
  unsigned parent;
  unsigned child;

  তার্কিক operator != (ধ্রুব overflow_record_t o) ধ্রুব
  { প্রত্যাগম !(*এতত্ == o); }

  অন্তঃস্থ তার্কিক operator == (ধ্রুব overflow_record_t& o) ধ্রুব
  {
    প্রত্যাগম parent == o.parent &&
        child == o.child;
  }

  অন্তঃস্থ uint32_t hash () ধ্রুব
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (parent);
    current = current * 31 + hb_hash (child);
    প্রত্যাগম current;
  }
};

অন্তঃস্থ
int64_t compute_offset (
    ধ্রুব graph_t& graph,
    unsigned parent_idx,
    ধ্রুব hb_serialize_context_t::object_t::link_t& link)
{
  ধ্রুব auto& parent = graph.vertices_[parent_idx];
  ধ্রুব auto& child = graph.vertices_[link.objidx];
  int64_t offset = 0;
  বিকল্প ((hb_serialize_context_t::whence_t) link.whence) {
    পক্ষ hb_serialize_context_t::whence_t::Head:
      offset = child.start - parent.start; ভঙ্গ;
    পক্ষ hb_serialize_context_t::whence_t::Tail:
      offset = child.start - parent.end; ভঙ্গ;
    পক্ষ hb_serialize_context_t::whence_t::Absolute:
      offset = child.start; ভঙ্গ;
  }

  প্রতিজ্ঞা (offset >= link.bias);
  offset -= link.bias;
  প্রত্যাগম offset;
}

অন্তঃস্থ
তার্কিক is_valid_offset (int64_t offset,
                      ধ্রুব hb_serialize_context_t::object_t::link_t& link)
{
  যদি (unlikely (!link.width))
    // Virtual links can't overflow.
    প্রত্যাগম link.is_signed || offset >= 0;

  যদি (link.is_signed)
  {
    যদি (link.width == 4)
      প্রত্যাগম offset >= -((int64_t) 1 << 31) && offset < ((int64_t) 1 << 31);
    অন্যথা
      প্রত্যাগম offset >= -(1 << 15) && offset < (1 << 15);
  }
  অন্যথা
  {
    যদি (link.width == 4)
      প্রত্যাগম offset >= 0 && offset < ((int64_t) 1 << 32);
    অন্যথা যদি (link.width == 3)
      প্রত্যাগম offset >= 0 && offset < ((int32_t) 1 << 24);
    অন্যথা
      প্রত্যাগম offset >= 0 && offset < (1 << 16);
  }
}

/*
 * Will any offsets overflow on graph when it's serialized?
 */
অন্তঃস্থ graph_result_t<তার্কিক>
will_overflow (graph_t& graph,
               hb_vector_t<overflow_record_t>* overflows = nullptr)
{
  যদি (overflows) overflows->clear ();
  graph.update_positions ();

  hb_hashmap_t<overflow_record_t, তার্কিক> record_set;
  ধ্রুব auto& vertices = graph.vertices_;
  প্রতি (unsigned parent_idx : graph.ordering_)
  {
    // Don't need to check virtual links for overflow
    প্রতি (ধ্রুব auto& link : vertices.arrayZ[parent_idx].obj ().real_links)
    {
      int64_t offset = compute_offset (graph, parent_idx, link);
      যদি (likely (is_valid_offset (offset, link)))
        অনুবর্ত;

      যদি (!overflows) প্রত্যাগম Ok(সত্য);

      overflow_record_t r;
      r.parent = parent_idx;
      r.child = link.objidx;
      যদি (record_set.has(r)) অনুবর্ত; // don't keep duplicate overflows.

      overflows->push (r);
      record_set.set (r, সত্য);
    }
  }

  TRY (graph_result_t<শূন্য>::from (record_set, ALLOCATION_FAILURE));
  যদি (overflows)
    TRY (graph_result_t<শূন্য>::from (*overflows, ALLOCATION_FAILURE));

  প্রত্যাগম Ok((তার্কিক) (overflows && overflows->length));
}

অন্তঃস্থ
শূন্য print_overflows (graph_t& graph,
                      ধ্রুব hb_vector_t<overflow_record_t>& overflows)
{
  যদি (!DEBUG_ENABLED(SUBSET_REPACK)) প্রত্যাগম;

  auto r = graph.update_parents ();
  যদি (r.is_err()) {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr, "Unable to print overflows due to error updating parent links: %s",
      to_string(r.error()));
    প্রত্যাগম;
  }

  পূর্ণাঙ্ক limit = 10;
  প্রতি (ধ্রুব auto& o : overflows)
  {
    যদি (!limit--) ভঙ্গ;
    ধ্রুব auto& parent = graph.vertices_[o.parent];
    ধ্রুব auto& child = graph.vertices_[o.child];
    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
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
  যদি (overflows.length > 10) {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr, "  ... plus %u more overflows.", overflows.length - 10);
  }
}

template <typename O> অন্তঃস্থ শূন্য
serialize_link_of_type (ধ্রুব hb_serialize_context_t::object_t::link_t& link,
                        বর্ণ* head,
                        unsigned size,
                        ধ্রুব hb_vector_t<unsigned>& id_map,
                        hb_serialize_context_t* c)
{
  // A link must never write outside the bounds of its parent object.
  যদি (unlikely (link.position + link.width > size))
  {
    c->err (HB_SERIALIZE_ERROR_OTHER);
    প্রত্যাগম;
  }

  OT::Offset<O>* offset = reinterpret_cast<OT::Offset<O>*> (head + link.position);
  *offset = 0;
  c->add_link (*offset,
               id_map[link.objidx],
               (hb_serialize_context_t::whence_t) link.whence,
               link.bias);
}

অন্তঃস্থ
শূন্য serialize_link (ধ্রুব hb_serialize_context_t::object_t::link_t& link,
                     বর্ণ* head,
                     unsigned size,
                     ধ্রুব hb_vector_t<unsigned>& id_map,
                     hb_serialize_context_t* c)
{
  বিকল্প (link.width)
  {
    পক্ষ 0:
      // Virtual links aren't serialized.
      প্রত্যাগম;
    পক্ষ 4:
      যদি (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT32> (link, head, size, id_map, c);
      } অন্যথা {
        serialize_link_of_type<OT::HBUINT32> (link, head, size, id_map, c);
      }
      প্রত্যাগম;
    পক্ষ 2:
      যদি (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT16> (link, head, size, id_map, c);
      } অন্যথা {
        serialize_link_of_type<OT::HBUINT16> (link, head, size, id_map, c);
      }
      প্রত্যাগম;
    পক্ষ 3:
      serialize_link_of_type<OT::HBUINT24> (link, head, size, id_map, c);
      প্রত্যাগম;
    স্বভাব:
      // Unexpected link width.
      প্রতিজ্ঞা (0);
  }
}

/*
 * serialize graph into the provided serialization buffer.
 */
অন্তঃস্থ graph_result_t<hb_blob_t*> serialize (ধ্রুব graph_t& graph)
{
  hb_vector_t<বর্ণ> buffer;
  size_t size = graph.total_size_in_bytes ();

  যদি (!size) প্রত্যাগম Ok(hb_blob_get_empty ());

  যদি (unlikely (!buffer.alloc (size))) {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr, "Unable to allocate output buffer.");
    প্রত্যাগম Err(ALLOCATION_FAILURE);
  }
  hb_serialize_context_t c((শূন্য *) buffer, size);

  c.start_serialize<শূন্য> ();
  ধ্রুব auto& vertices = graph.vertices_;

  // Objects are placed in the serializer in reverse order since children need
  // to be inserted before their parents.

  // Maps from our obj id's to the id's used during this serialization.
  hb_vector_t<unsigned> id_map;
  যদি (unlikely (!id_map.resize(graph.ordering_.length))) {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr, "Unable to allocate id_map buffer.");
    প্রত্যাগম Err(ALLOCATION_FAILURE);
  }

  প্রতি (পূর্ণাঙ্ক pos = graph.ordering_.length - 1; pos >= 0; pos--) {
    unsigned i = graph.ordering_[pos];
    c.push ();

    auto& v = vertices[i];

    size_t size = v.table_size ();

    বর্ণ* start = c.allocate_size <বর্ণ> (size);
    যদি (unlikely (!start)) {
      DEBUG_বার্তা (SUBSET_REPACK, nullptr, "Buffer out of space.");
      প্রত্যাগম Err(ALLOCATION_FAILURE);
    }

    hb_memcpy (start, v.obj ().head, size);

    // Only real links needs to be serialized.
    প্রতি (ধ্রুব auto& link : v.obj ().real_links)
      serialize_link (link, start, size, id_map, &c);

    // All duplications are already encoded in the graph, so don't
    // enable sharing during packing.
    id_map[i] = c.বের_করো_pack (অসত্য);
  }
  c.end_serialize ();

  যদি (unlikely (c.in_error ())) {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr, "Error during serialization. Err flag: %d",
               c.errors);
    যদি (c.errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)
      প্রত্যাগম Err(OVERFLOW_RESOLUTION_FAILED);
    প্রত্যাগম Err(ALLOCATION_FAILURE);
  }

  auto* blob = c.copy_blob ();
  যদি (unlikely (!blob)) প্রত্যাগম Err(ALLOCATION_FAILURE);
  প্রত্যাগম Ok(blob);
}

} // namespace graph

#endif // GRAPH_SERIALIZE_HH
