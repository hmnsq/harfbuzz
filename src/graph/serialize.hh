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

#شامل "graph-result.hh"

فضاینام graph {

ساختار overflow_record_t
{
  unsigned parent;
  unsigned child;

  منطقی operator != (ثابت overflow_record_t o) ثابت
  { بازگشت !(*این == o); }

  درونخطی منطقی operator == (ثابت overflow_record_t& o) ثابت
  {
    بازگشت parent == o.parent &&
        child == o.child;
  }

  درونخطی uint32_t hash () ثابت
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (parent);
    current = current * 31 + hb_hash (child);
    بازگشت current;
  }
};

درونخطی
int64_t compute_offset (
    ثابت graph_t& graph,
    unsigned parent_idx,
    ثابت hb_serialize_context_t::object_t::link_t& link)
{
  ثابت auto& parent = graph.vertices_[parent_idx];
  ثابت auto& child = graph.vertices_[link.objidx];
  int64_t offset = 0;
  انتخاب ((hb_serialize_context_t::whence_t) link.whence) {
    حالت hb_serialize_context_t::whence_t::Head:
      offset = child.start - parent.start; بشکن;
    حالت hb_serialize_context_t::whence_t::Tail:
      offset = child.start - parent.end; بشکن;
    حالت hb_serialize_context_t::whence_t::Absolute:
      offset = child.start; بشکن;
  }

  أكد (offset >= link.bias);
  offset -= link.bias;
  بازگشت offset;
}

درونخطی
منطقی is_valid_offset (int64_t offset,
                      ثابت hb_serialize_context_t::object_t::link_t& link)
{
  اگر (unlikely (!link.width))
    // Virtual links can't overflow.
    بازگشت link.is_signed || offset >= 0;

  اگر (link.is_signed)
  {
    اگر (link.width == 4)
      بازگشت offset >= -((int64_t) 1 << 31) && offset < ((int64_t) 1 << 31);
    وگرنه
      بازگشت offset >= -(1 << 15) && offset < (1 << 15);
  }
  وگرنه
  {
    اگر (link.width == 4)
      بازگشت offset >= 0 && offset < ((int64_t) 1 << 32);
    وگرنه اگر (link.width == 3)
      بازگشت offset >= 0 && offset < ((int32_t) 1 << 24);
    وگرنه
      بازگشت offset >= 0 && offset < (1 << 16);
  }
}

/*
 * Will any offsets overflow on graph when it's serialized?
 */
درونخطی graph_result_t<منطقی>
will_overflow (graph_t& graph,
               hb_vector_t<overflow_record_t>* overflows = nullptr)
{
  اگر (overflows) overflows->clear ();
  graph.update_positions ();

  hb_hashmap_t<overflow_record_t, منطقی> record_set;
  ثابت auto& vertices = graph.vertices_;
  برای (unsigned parent_idx : graph.ordering_)
  {
    // Don't need to check virtual links for overflow
    برای (ثابت auto& link : vertices.arrayZ[parent_idx].obj ().real_links)
    {
      int64_t offset = compute_offset (graph, parent_idx, link);
      اگر (likely (is_valid_offset (offset, link)))
        ادامه;

      اگر (!overflows) بازگشت Ok(درست);

      overflow_record_t r;
      r.parent = parent_idx;
      r.child = link.objidx;
      اگر (record_set.has(r)) ادامه; // don't keep duplicate overflows.

      overflows->push (r);
      record_set.set (r, درست);
    }
  }

  TRY (graph_result_t<تهی>::from (record_set, ALLOCATION_FAILURE));
  اگر (overflows)
    TRY (graph_result_t<تهی>::from (*overflows, ALLOCATION_FAILURE));

  بازگشت Ok((منطقی) (overflows && overflows->length));
}

درونخطی
تهی print_overflows (graph_t& graph,
                      ثابت hb_vector_t<overflow_record_t>& overflows)
{
  اگر (!DEBUG_ENABLED(SUBSET_REPACK)) بازگشت;

  auto r = graph.update_parents ();
  اگر (r.is_err()) {
    DEBUG_پیام (SUBSET_REPACK, nullptr, "Unable to print overflows due to error updating parent links: %s",
      to_string(r.error()));
    بازگشت;
  }

  صحیح limit = 10;
  برای (ثابت auto& o : overflows)
  {
    اگر (!limit--) بشکن;
    ثابت auto& parent = graph.vertices_[o.parent];
    ثابت auto& child = graph.vertices_[o.child];
    DEBUG_پیام (SUBSET_REPACK, nullptr,
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
  اگر (overflows.length > 10) {
    DEBUG_پیام (SUBSET_REPACK, nullptr, "  ... plus %u more overflows.", overflows.length - 10);
  }
}

template <typename O> درونخطی تهی
serialize_link_of_type (ثابت hb_serialize_context_t::object_t::link_t& link,
                        نویسه* head,
                        unsigned size,
                        ثابت hb_vector_t<unsigned>& id_map,
                        hb_serialize_context_t* c)
{
  // A link must never write outside the bounds of its parent object.
  اگر (unlikely (link.position + link.width > size))
  {
    c->err (HB_SERIALIZE_ERROR_OTHER);
    بازگشت;
  }

  OT::Offset<O>* offset = reinterpret_cast<OT::Offset<O>*> (head + link.position);
  *offset = 0;
  c->add_link (*offset,
               id_map[link.objidx],
               (hb_serialize_context_t::whence_t) link.whence,
               link.bias);
}

درونخطی
تهی serialize_link (ثابت hb_serialize_context_t::object_t::link_t& link,
                     نویسه* head,
                     unsigned size,
                     ثابت hb_vector_t<unsigned>& id_map,
                     hb_serialize_context_t* c)
{
  انتخاب (link.width)
  {
    حالت 0:
      // Virtual links aren't serialized.
      بازگشت;
    حالت 4:
      اگر (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT32> (link, head, size, id_map, c);
      } وگرنه {
        serialize_link_of_type<OT::HBUINT32> (link, head, size, id_map, c);
      }
      بازگشت;
    حالت 2:
      اگر (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT16> (link, head, size, id_map, c);
      } وگرنه {
        serialize_link_of_type<OT::HBUINT16> (link, head, size, id_map, c);
      }
      بازگشت;
    حالت 3:
      serialize_link_of_type<OT::HBUINT24> (link, head, size, id_map, c);
      بازگشت;
    پیشفرض:
      // Unexpected link width.
      أكد (0);
  }
}

/*
 * serialize graph into the provided serialization buffer.
 */
درونخطی graph_result_t<hb_blob_t*> serialize (ثابت graph_t& graph)
{
  hb_vector_t<نویسه> buffer;
  size_t size = graph.total_size_in_bytes ();

  اگر (!size) بازگشت Ok(hb_blob_get_empty ());

  اگر (unlikely (!buffer.alloc (size))) {
    DEBUG_پیام (SUBSET_REPACK, nullptr, "Unable to allocate output buffer.");
    بازگشت Err(ALLOCATION_FAILURE);
  }
  hb_serialize_context_t c((تهی *) buffer, size);

  c.start_serialize<تهی> ();
  ثابت auto& vertices = graph.vertices_;

  // Objects are placed in the serializer in reverse order since children need
  // to be inserted before their parents.

  // Maps from our obj id's to the id's used during this serialization.
  hb_vector_t<unsigned> id_map;
  اگر (unlikely (!id_map.resize(graph.ordering_.length))) {
    DEBUG_پیام (SUBSET_REPACK, nullptr, "Unable to allocate id_map buffer.");
    بازگشت Err(ALLOCATION_FAILURE);
  }

  برای (صحیح pos = graph.ordering_.length - 1; pos >= 0; pos--) {
    unsigned i = graph.ordering_[pos];
    c.push ();

    auto& v = vertices[i];

    size_t size = v.table_size ();

    نویسه* start = c.allocate_size <نویسه> (size);
    اگر (unlikely (!start)) {
      DEBUG_پیام (SUBSET_REPACK, nullptr, "Buffer out of space.");
      بازگشت Err(ALLOCATION_FAILURE);
    }

    hb_memcpy (start, v.obj ().head, size);

    // Only real links needs to be serialized.
    برای (ثابت auto& link : v.obj ().real_links)
      serialize_link (link, start, size, id_map, &c);

    // All duplications are already encoded in the graph, so don't
    // enable sharing during packing.
    id_map[i] = c.برداشت_pack (نادرست);
  }
  c.end_serialize ();

  اگر (unlikely (c.in_error ())) {
    DEBUG_پیام (SUBSET_REPACK, nullptr, "Error during serialization. Err flag: %d",
               c.errors);
    اگر (c.errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)
      بازگشت Err(OVERFLOW_RESOLUTION_FAILED);
    بازگشت Err(ALLOCATION_FAILURE);
  }

  auto* blob = c.copy_blob ();
  اگر (unlikely (!blob)) بازگشت Err(ALLOCATION_FAILURE);
  بازگشت Ok(blob);
}

} // namespace graph

#endif // GRAPH_SERIALIZE_HH
