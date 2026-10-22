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

#அந்தர்பாவ "../hb-set.hh"
#அந்தர்பாவ "../hb-priority-queue.hh"
#அந்தர்பாவ "../hb-serialize.hh"
#அந்தர்பாவ "graph-result.hh"

#ifndef GRAPH_GRAPH_HH
#define GRAPH_GRAPH_HH

#define HB_GRAPH_INVALID ((unsigned) -1)

நாமாகாஶ graph {

/**
 * Represents a serialized table in the form of a graph.
 * Provides methods for modifying and reordering the graph.
 */
கட்டமைப்பு graph_t
{
  // TODO(garretrieger): experiment with using scratch vector for visited sets instead of allocating
  // a new hb_set_t each time.

  கட்டமைப்பு vertex_t
  {
    // TODO(garretrieger): have vertex_t gaurantee that head/tail are non null and head < tail
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): have vertex_t gaurantee that all link indexes are valid (ie < vertices_.length)
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): track where the real links are currently sorted, and automatically sort
    // prior to accessing iterator for real links. Avoids needing to manually trigger sorting by
    // downstream users.

    int64_t distance = 0 ;
    unsigned space = 0 ;
    unsigned start = 0;
    unsigned end = 0;
    unsigned priority = 0;

    தனிப்பட்ட:
    hb_serialize_context_t::object_t obj_;
    unsigned incoming_edges_ = 0;
    unsigned single_parent = HB_GRAPH_INVALID;
    தருக்க has_incoming_virtual_edges_ = பொய்;
    hb_hashmap_t<unsigned, unsigned> parents;
    hb_set_t virtual_parents;
    பொது:

    vertex_t ()
    {}

    explicit vertex_t(மாறிலி hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    auto parents_iter () மாறிலி HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_parent, single_parent != HB_GRAPH_INVALID),
	parents.சாவிகள்_ref ()
      )
    )

    வெறுமை set_buffer (எழுத்து* head, எழுத்து* tail) {
      obj_.head = head;
      obj_.tail = tail;
    }

    வெறுமை shrink_buffer (size_t மொத்தம்)
    {
      obj_.tail -= மொத்தம்;
    }

    auto all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    auto real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    auto virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    வெறுமை sort_real_links () {
      obj_.real_links.qsort ();
    }

    graph_result_t<வெறுமை> add_real_link (unsigned width, unsigned obj_idx, unsigned position) {
      auto* link = obj_.real_links.push ();
      TRY (graph_result_t<வெறுமை>::from (obj_.real_links, ALLOCATION_FAILURE));
      link->width = width;
      link->objidx = obj_idx;
      link->position = position;
      திரும்பு Ok();
    }

    graph_result_t<வெறுமை> add_virtual_link (unsigned obj_idx) {
      auto* link = obj_.virtual_links.push ();
      TRY (graph_result_t<வெறுமை>::from (obj_.virtual_links, ALLOCATION_FAILURE));
      link->objidx = obj_idx;
      திரும்பு Ok();
    }

    graph_result_t<வெறுமை> add_real_link (மாறிலி hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      திரும்பு graph_result_t<வெறுமை>::from (obj_.real_links, ALLOCATION_FAILURE);
    }

    graph_result_t<வெறுமை> set_real_links (hb_vector_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(graph_result_t<வெறுமை>::from (links, ALLOCATION_FAILURE));
      obj_.real_links = links;
      திரும்பு Ok();
    }

    வெறுமை clear_real_links ()
    {
      obj_.real_links.reset();
    }

    வெறுமை clear_virtual_links ()
    {
      obj_.virtual_links.reset();
    }

    graph_result_t<வெறுமை> add_virtual_link (மாறிலி hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      திரும்பு graph_result_t<வெறுமை>::from (obj_.virtual_links, ALLOCATION_FAILURE);
    }

    மாறிலி hb_serialize_context_t::object_t& obj() மாறிலி { திரும்பு obj_; }

    template<typename T>
    T as () மாறிலி
    {
      திரும்பு reinterpret_cast<T> (obj_.head);
    }

    தருக்க has_incoming_virtual_edges () மாறிலி
    {
      திரும்பு has_incoming_virtual_edges_;
    }

    graph_result_t<வெறுமை> link_positions_valid (unsigned இலக்கம்_objects, தருக்க removed_nil)
    {
      hb_set_t assigned_bytes;
      க்கு (மாறிலி auto& l : obj_.real_links)
      {
        எனில் (unlikely (l.objidx >= இலக்கம்_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_செய்தி (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid object index.");
          திரும்பு Err(INVALID_ARGUMENT);
        }

        unsigned start = l.position;
        unsigned end = start + l.width - 1;

        எனில் (unlikely (l.width < 2 || l.width > 4))
        {
          DEBUG_செய்தி (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid link width.");
          திரும்பு Err(INVALID_ARGUMENT);
        }

        எனில் (unlikely (end >= table_size ()))
        {
          DEBUG_செய்தி (SUBSET_REPACK, nullptr,
                     "Invalid graph. Link position is out of bounds.");
          திரும்பு Err(OUT_OF_BOUNDS);
        }

        எனில் (unlikely (assigned_bytes.intersects (start, end)))
        {
          DEBUG_செய்தி (SUBSET_REPACK, nullptr,
                     "Invalid graph. Found offsets whose positions overlap.");
          திரும்பு Err(INVALID_ARGUMENT);
        }

        assigned_bytes.add_range (start, end);
      }

      திரும்பு graph_result_t<வெறுமை>::from (assigned_bytes, ALLOCATION_FAILURE);
    }

    வெறுமை normalize ()
    {
      obj_.real_links.qsort ();
      க்கு (மாறிலி auto& l : obj_.real_links)
      {
        க்கு (unsigned i = 0; i < l.width; i++)
        {
          obj_.head[l.position + i] = 0;
        }
      }
    }

    தருக்க equals (unsigned this_index,
                 unsigned other_index,
                 மாறிலி vertex_t& other,
                 மாறிலி graph_t& graph,
                 மாறிலி graph_t& other_graph,
                 unsigned depth) மாறிலி
    {
      எனில் (!(as_bytes () == other.as_bytes ()))
      {
        DEBUG_செய்தி (SUBSET_REPACK, nullptr,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_index,
                   (unsigned long) table_size (),
                   other_index,
                   (unsigned long) other.table_size (),
                   depth);

        auto a = as_bytes ();
        auto b = other.as_bytes ();
        போது (a || b)
        {
          DEBUG_செய்தி (SUBSET_REPACK, nullptr,
                     "  0x%x %s 0x%x", (unsigned) *a, (*a == *b) ? "==" : "!=", (unsigned) *b);
          a++;
          b++;
        }
        திரும்பு பொய்;
      }

      திரும்பு links_equal (obj_.real_links, other.obj_.real_links, graph, other_graph, depth);
    }

    hb_bytes_t as_bytes () மாறிலி
    {
      திரும்பு hb_bytes_t (obj_.head, table_size ());
    }

    friend வெறுமை swap (vertex_t& a, vertex_t& b)
    {
      hb_swap (a.obj_, b.obj_);
      hb_swap (a.distance, b.distance);
      hb_swap (a.space, b.space);
      hb_swap (a.single_parent, b.single_parent);
      hb_swap (a.parents, b.parents);
      hb_swap (a.virtual_parents, b.virtual_parents);
      hb_swap (a.incoming_edges_, b.incoming_edges_);
      hb_swap (a.has_incoming_virtual_edges_, b.has_incoming_virtual_edges_);
      hb_swap (a.start, b.start);
      hb_swap (a.end, b.end);
      hb_swap (a.priority, b.priority);
    }

    graph_result_t<hb_hashmap_t<unsigned, unsigned>>
    position_to_index_map () மாறிலி
    {
      hb_hashmap_t<unsigned, unsigned> result;

      result.alloc (obj_.real_links.length);
      க்கு (மாறிலி auto& l : obj_.real_links) {
        result.set (l.position, l.objidx);
      }

      திரும்பு graph_result_t<hb_hashmap_t<unsigned, unsigned>>::from (std::move(result), ALLOCATION_FAILURE);
    }

    தருக்க is_shared () மாறிலி
    {
      திரும்பு parents.get_population () > virtual_parents.get_population () + 1;
    }

    unsigned incoming_edges () மாறிலி
    {
      எனில் (HB_DEBUG_SUBSET_REPACK)
       {
	ப்ரதிஜ்ஞா (incoming_edges_ == (single_parent != HB_GRAPH_INVALID) +
		(parents.மதிப்புகள்_ref () | hb_reduce (hb_add, 0)));
       }
      திரும்பு incoming_edges_;
    }

    unsigned incoming_edges_from_parent (unsigned parent_index) மாறிலி {
      எனில் (single_parent != HB_GRAPH_INVALID) {
        திரும்பு single_parent == parent_index ? 1 : 0;
      }

      unsigned* மொத்தம்;
      திரும்பு  parents.has(parent_index, &மொத்தம்) ? *மொத்தம் : 0;
    }

    வெறுமை reset_parents ()
    {
      incoming_edges_ = 0;
      has_incoming_virtual_edges_ = பொய்;
      single_parent = HB_GRAPH_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    graph_result_t<வெறுமை> add_parent (unsigned parent_index, தருக்க is_virtual)
    {
      ப்ரதிஜ்ஞா (parent_index != HB_GRAPH_INVALID);
      has_incoming_virtual_edges_ |= is_virtual;
      எனில் (is_virtual)
        virtual_parents.add (parent_index);

      எனில் (incoming_edges_ == 0)
      {
	single_parent = parent_index;
	incoming_edges_ = 1;
	திரும்பு to_result();
      }
      இல்லையெனில் எனில் (single_parent != HB_GRAPH_INVALID)
      {
        ப்ரதிஜ்ஞா (incoming_edges_ == 1);
	எனில் (!parents.set (single_parent, 1))
	  திரும்பு to_result ();
	single_parent = HB_GRAPH_INVALID;
      }

      unsigned *v;
      எனில் (parents.has (parent_index, &v))
      {
        (*v)++;
	incoming_edges_++;
      }
      இல்லையெனில் எனில் (parents.set (parent_index, 1))
	incoming_edges_++;

      திரும்பு to_result();
    }

    வெறுமை remove_parent (unsigned parent_index)
    {
      எனில் (parent_index == single_parent)
      {
	single_parent = HB_GRAPH_INVALID;
	incoming_edges_--;
	virtual_parents.reset ();
	திரும்பு;
      }

      unsigned *v;
      எனில் (parents.has (parent_index, &v))
      {
	incoming_edges_--;
	எனில் (*v > 1)
	  (*v)--;
	இல்லையெனில்
        {
	  parents.del (parent_index);
          virtual_parents.del (parent_index);
        }

	எனில் (incoming_edges_ == 1)
	{
	  single_parent = *parents.சாவிகள் ();
	  parents.reset ();
	}
      }
    }

    வெறுமை remove_real_link_unordered (unsigned link_array_index) {
      obj_.real_links.remove_unordered (link_array_index);
    }

    வெறுமை remove_real_link_unordered (unsigned child_index, மாறிலி வெறுமை* offset)
    {
      unsigned மொத்தம் = obj_.real_links.length;
      க்கு (unsigned i = 0; i < மொத்தம்; i++)
      {
        auto& link = obj_.real_links.arrayZ[i];
        எனில் (link.objidx != child_index)
          தொடர்;

        எனில் ((obj_.head + link.position) != offset)
          தொடர்;

        obj_.real_links.remove_unordered (i);
        திரும்பு;
      }
    }

    graph_result_t<வெறுமை> remap_parent (unsigned old_index, unsigned new_index)
    {
      எனில் (single_parent != HB_GRAPH_INVALID)
      {
        எனில் (single_parent == old_index)
        {
	  single_parent = new_index;
          எனில் (virtual_parents.has (old_index))
          {
            virtual_parents.del (old_index);
            virtual_parents.add (new_index);
          }
        }
        திரும்பு graph_result_t<வெறுமை>::from(virtual_parents, ALLOCATION_FAILURE);
      }

      மாறிலி unsigned *pv;
      எனில் (parents.has (old_index, &pv))
      {
        unsigned v = *pv;
	எனில் (!parents.set (new_index, v))
          incoming_edges_ -= v;
	parents.del (old_index);

        எனில் (incoming_edges_ == 1)
	{
	  single_parent = *parents.சாவிகள் ();
	  parents.reset ();
	}
      }

      எனில் (virtual_parents.has (old_index))
      {
        virtual_parents.del (old_index);
        virtual_parents.add (new_index);
      }

      திரும்பு graph_result_t<வெறுமை>::from(virtual_parents, ALLOCATION_FAILURE);
    }

    தருக்க is_leaf () மாறிலி
    {
      திரும்பு !obj_.real_links.length && !obj_.virtual_links.length;
    }

    தருக்க raise_priority ()
    {
      எனில் (has_max_priority ()) திரும்பு பொய்;
      priority++;
      திரும்பு உண்மை;
    }

    தருக்க give_max_priority ()
    {
      தருக்க result = பொய்;
      போது (!has_max_priority()) {
        result = உண்மை;
        priority++;
      }
      திரும்பு result;
    }

    தருக்க has_max_priority () மாறிலி {
      திரும்பு priority >= 3;
    }

    size_t table_size () மாறிலி {
      திரும்பு obj_.tail - obj_.head;
    }

    int64_t modified_distance (unsigned order) மாறிலி
    {
      // TODO(garretrieger): once priority is high enough, should try
      // setting distance = 0 which will force to sort immediately after
      // it's parent where possible.

      int64_t modified_distance =
          hb_clamp (distance + distance_modifier (), (int64_t) 0,
                    hb_int_max (int64_t) >> 18);
      எனில் (has_max_priority ()) {
        modified_distance = 0;
      }
      திரும்பு (modified_distance << 18) | (0x003FFFF & order);
    }

    int64_t distance_modifier () மாறிலி
    {
      எனில் (!priority) திரும்பு 0;
      int64_t table_size = obj_.tail - obj_.head;

      எனில் (priority == 1)
        திரும்பு -table_size / 2;

      திரும்பு -table_size;
    }

   தனிப்பட்ட:
    graph_result_t<வெறுமை> to_result() மாறிலி {
      எனில் (unlikely (
        parents.in_error() || virtual_parents.in_error() ||
        obj_.real_links.in_error() || obj_.virtual_links.in_error()))
        திரும்பு Err(ALLOCATION_FAILURE);
      திரும்பு Ok();
    }

    தருக்க links_equal (மாறிலி hb_vector_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      மாறிலி hb_vector_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      மாறிலி graph_t& graph,
                      மாறிலி graph_t& other_graph,
                      unsigned depth) மாறிலி
    {
      auto a = this_links.iter ();
      auto b = other_links.iter ();

      போது (a && b)
      {
        மாறிலி auto& link_a = *a;
        மாறிலி auto& link_b = *b;

        எனில் (link_a.width != link_b.width ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          திரும்பு பொய்;

        எனில் (!graph.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_graph.vertices_[link_b.objidx], graph, other_graph, depth + 1))
          திரும்பு பொய்;

        a++;
        b++;
      }

      எனில் (தருக்க (a) != தருக்க (b))
        திரும்பு பொய்;

      திரும்பு உண்மை;
    }
  };

  எண்ணிக்கை vertex_mutability_t
  {
    Immutable,
    Mutable
  };

  template <typename T, vertex_mutability_t mutability = Mutable>
  கட்டமைப்பு vertex_and_table_t
  {
    vertex_and_table_t () : index (0), vertex (nullptr), table (nullptr)
    {}

    template <vertex_mutability_t other_mutability>
    vertex_and_table_t (மாறிலி vertex_and_table_t<T, other_mutability>& o)
      : index (o.index), vertex (o.vertex), table (o.table)
    {}

    unsigned index;
    typename std::conditional<mutability == Immutable, மாறிலி vertex_t*, vertex_t*>::type vertex;
    typename std::conditional<mutability == Immutable, மாறிலி T*, T*>::type table;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  template<typename T>
  நிலையான graph_result_t<graph_t> create (மாறிலி T& objects)
  {
    எனில் (unlikely (objects.length > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_செய்தி (SUBSET_REPACK, nullptr,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.length);
      திரும்பு Err(LIMIT_EXCEEDED);
    }

    graph_t g;
    g.இலக்கம்_roots_for_space.push (1);
    TRY (graph_result_t<வெறுமை>::from (g.இலக்கம்_roots_for_space, ALLOCATION_FAILURE));

    தருக்க removed_nil = பொய்;
    மாறிலி unsigned மொத்தம் = objects.length;
    g.vertices_.alloc (மொத்தம்);
    g.ordering_.resize (மொத்தம்);
    g.ordering_scratch_.alloc (மொத்தம்);
    TRY (graph_result_t<வெறுமை>::from (g.vertices_, ALLOCATION_FAILURE));
    TRY (graph_result_t<வெறுமை>::from (g.ordering_, ALLOCATION_FAILURE));
    TRY (graph_result_t<வெறுமை>::from (g.ordering_scratch_, ALLOCATION_FAILURE));

    unsigned order = objects.length;
    unsigned skip = 0;
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      எனில் (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = உண்மை;
        order--;
        g.ordering_.resize (மொத்தம் - 1);
        skip++;
        தொடர்;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      எனில் (unlikely (obj.in_error ())) {
        திரும்பு Err(ALLOCATION_FAILURE);
      }
      vertex_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (மொத்தம், removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      unsigned obj_idx = i - skip;
      g.ordering_[--order] = obj_idx;

      எனில் (!removed_nil) தொடர்;
      // Fix indices to account for removed nil object.
      க்கு (auto& l : v->all_links_writer ()) {
        l.objidx--;
      }
    }

    auto r = g.is_fully_connected();
    எனில் (r.is_err() && r.error() == ORPHANED_NODES) {
      g.print_orphaned_nodes ();
    }
    TRY(r);

    திரும்பு Ok (std::move (g));
  }

  graph_t (மாறிலி graph_t&) = delete;
  graph_t& operator = (மாறிலி graph_t&) = delete;
  graph_t (graph_t&&) = இயல்பு;
  graph_t& operator = (graph_t&&) = இயல்பு;

  ~graph_t ()
  {
    க்கு (எழுத்து* b : buffers)
      hb_free (b);
  }

தனிப்பட்ட:
  graph_t () = இயல்பு;

பொது:

  தருக்க operator== (மாறிலி graph_t& other) மாறிலி
  {
    திரும்பு வேர் ().equals (வேர்_idx(), other.வேர்_idx(), other.வேர் (), *இது, other, 0);
  }

  வெறுமை print () மாறிலி {
    க்கு (unsigned id : ordering_)
    {
      மாறிலி auto& v = vertices_[id];
      printf("%u: %u [", id, (unsigned முழு)v.table_size());
      க்கு (மாறிலி auto &l : v.obj().real_links) {
        printf("%u, ", l.objidx);
      }
      க்கு (மாறிலி auto &l : v.obj().virtual_links) {
        printf("v%u, ", l.objidx);
      }
      printf("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  வெறுமை normalize ()
  {
    க்கு (auto& v : vertices_.writer ())
      v.normalize ();
  }

  மாறிலி vertex_t& வேர் () மாறிலி
  {
    திரும்பு vertices_[வேர்_idx ()];
  }

  unsigned வேர்_idx () மாறிலி
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    திரும்பு ordering_[0];
  }

  மாறிலி hb_serialize_context_t::object_t& object (unsigned i) மாறிலி
  {
    திரும்பு vertices_[i].obj();
  }

  graph_result_t<வெறுமை> add_buffer (எழுத்து* buffer)
  {
    buffers.push (buffer);
    திரும்பு graph_result_t<வெறுமை>::from (buffers, ALLOCATION_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  template<typename T>
  graph_result_t<வெறுமை> add_link (T* offset,
                                 unsigned parent_id,
                                 unsigned child_id)
  {
    எனில் (unlikely (parent_id >= vertices_.length || child_id >= vertices_.length))
      திரும்பு Err(OUT_OF_BOUNDS);
    auto& v = vertices_[parent_id];
    TRY(v.add_real_link(2, child_id, (எழுத்து*) offset - (எழுத்து*) v.obj().head));
    திரும்பு vertices_[child_id].add_parent (parent_id, பொய்);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  graph_result_t<வெறுமை> sort_shortest_distance_if_needed ()
  {
    எனில் (!positions_invalid) திரும்பு Ok();
    திரும்பு sort_shortest_distance ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  graph_result_t<வெறுமை> sort_shortest_distance ()
  {
    positions_invalid = உண்மை;

    எனில் (vertices_.length <= 1) {
      // Graph of 1 or less doesn't need sorting.
      திரும்பு Ok();
    }

    TRY (update_distances ());

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (vertices_.length);
    TRY (graph_result_t<வெறுமை>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.length);
    TRY (graph_result_t<வெறுமை>::from (new_ordering, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> removed_edges;
    removed_edges.resize (vertices_.length);
    TRY (graph_result_t<வெறுமை>::from (removed_edges, ALLOCATION_FAILURE));

    TRY (update_parents ());

    queue.insert (வேர் ().modified_distance (0), வேர்_idx ());
    unsigned order = 1;
    unsigned pos = 0;
    போது (!queue.in_error () && !queue.is_empty ())
    {
      unsigned next_id = queue.எடு_minimum().second;

      எனில் (unlikely (pos >= new_ordering.length)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_செய்தி (SUBSET_REPACK, nullptr, "Invalid graph. Contains cycle.");
        திரும்பு Err(CYCLE_DETECTED);
      }
      new_ordering[pos++] = next_id;
      மாறிலி vertex_t& next = vertices_[next_id];

      க்கு (மாறிலி auto& link : next.obj().all_links ()) {
        எனில் (unlikely (link.objidx >= vertices_.length))
          திரும்பு Err(OUT_OF_BOUNDS);
        removed_edges[link.objidx]++;
        மாறிலி auto& v = vertices_[link.objidx];
        எனில் (!(v.incoming_edges () - removed_edges[link.objidx]))
          // Add the order that the links were encountered to the priority.
          // This ensures that ties between priorities objects are broken in a consistent
          // way. More specifically this is set up so that if a set of objects have the same
          // distance they'll be added to the topological order in the order that they are
          // referenced from the parent object.
          queue.insert (v.modified_distance (order++),
                        link.objidx);
      }
    }

    TRY (graph_result_t<வெறுமை>::from (queue, ALLOCATION_FAILURE));
    TRY (graph_result_t<வெறுமை>::from (new_ordering, ALLOCATION_FAILURE));

    hb_swap (ordering_, new_ordering);

    எனில் (unlikely (pos != vertices_.length)) {
      print_orphaned_nodes ();
      திரும்பு Err(ORPHANED_NODES);
    }

    திரும்பு Ok();
  }

  /*
   * Finds the set of nodes (placed into roots) that should be assigned unique spaces.
   * More specifically this looks for the top most 24 bit or 32 bit links in the graph.
   * Some special casing is done that is specific to the layout of GSUB/GPOS tables.
   */
  graph_result_t<வெறுமை> find_space_roots (hb_set_t& visited, hb_set_t& roots)
  {
    unsigned வேர்_index = வேர்_idx ();
    க்கு (unsigned i : ordering_)
    {
      எனில் (visited.has (i)) தொடர்;

      // Only real links can form 32 bit spaces
      க்கு (மாறிலி auto& l : vertices_[i].obj().real_links)
      {
        எனில் (l.is_signed || l.width < 3)
          தொடர்;

        எனில் (i == வேர்_index && l.width == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          தொடர்;

        எனில் (l.width >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_set_t sub_roots;
          TRY (find_32bit_roots (l.objidx, sub_roots));
          எனில் (sub_roots) {
            க்கு (unsigned sub_வேர்_idx : sub_roots) {
              roots.add (sub_வேர்_idx);
              TRY (find_subgraph (sub_வேர்_idx, visited));
            }
            தொடர்;
          }
        }

        roots.add (l.objidx);
        TRY (find_subgraph (l.objidx, visited));
      }
    }

    TRY(graph_result_t<வெறுமை>::from (visited, ALLOCATION_FAILURE));
    TRY(graph_result_t<வெறுமை>::from (roots, ALLOCATION_FAILURE));
    திரும்பு Ok();
  }

  template <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Immutable>> as_table (unsigned parent, மாறிலி வெறுமை* offset, Ts... ds)
  {
    TRY_ASSIGN (unsigned idx, index_for_offset (parent, offset));
    திரும்பு as_table_from_index<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Mutable>> as_mutable_table (unsigned parent, மாறிலி வெறுமை* offset, Ts... ds)
  {
    TRY_ASSIGN (auto idx, mutable_index_for_offset (parent, offset));
    திரும்பு as_table_from_index<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, vertex_mutability_t mutability = Mutable, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, mutability>> as_table_from_index (unsigned index, Ts... ds)
  {
    எனில் (unlikely (index >= vertices_.length))
      திரும்பு Err(OUT_OF_BOUNDS);

    vertex_and_table_t<T, mutability> r;
    r.vertex = (typename std::conditional<mutability == Immutable, மாறிலி vertex_t*, vertex_t*>::type) &vertices_[index];
    r.table = r.vertex->template as<typename std::conditional<mutability == Immutable, மாறிலி T*, T*>::type> ();
    r.index = index;
    எனில் (unlikely (!r.table))
      திரும்பு Err(INVALID_ARGUMENT);

    TRY(r.table->sanitize (*(r.vertex), std::forward<Ts>(ds)...));
    திரும்பு r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  graph_result_t<unsigned> index_for_offset (unsigned முனை_idx, மாறிலி வெறுமை* offset) மாறிலி
  {
    எனில் (unlikely (முனை_idx >= vertices_.length)) திரும்பு Err(OUT_OF_BOUNDS);
    மாறிலி auto& node = object (முனை_idx);
    எனில் (unlikely (offset < node.head || offset >= node.tail)) திரும்பு Err(OUT_OF_BOUNDS);

    unsigned மொத்தம் = node.real_links.length;
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      மாறிலி auto& link = node.real_links.arrayZ[i];
      எனில் (offset != node.head + link.position)
        தொடர்;
      திரும்பு Ok(link.objidx);
    }

    திரும்பு Err(INVALID_ARGUMENT);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  graph_result_t<unsigned> mutable_index_for_offset (unsigned முனை_idx, மாறிலி வெறுமை* offset)
  {
    TRY_ASSIGN (unsigned child_idx, index_for_offset (முனை_idx, offset));
    எனில் (unlikely (child_idx >= vertices_.length))
      திரும்பு Err(OUT_OF_BOUNDS);

    auto& child = vertices_[child_idx];
    க்கு (unsigned p : child.parents_iter ())
    {
      எனில் (p != முனை_idx) {
        திரும்பு duplicate (முனை_idx, child_idx, உண்மை);
      }
    }

    திரும்பு Ok(child_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  graph_result_t<தருக்க> assign_spaces ()
  {
    TRY (update_parents ());

    க்கு (auto& vertex : vertices_.writer ())
    {
      vertex.space = 0;
      vertex.priority = 0;
    }
    இலக்கம்_roots_for_space.resize (1);
    TRY (graph_result_t<வெறுமை>::from (இலக்கம்_roots_for_space, ALLOCATION_FAILURE));
    இலக்கம்_roots_for_space[0] = 1;
    distance_invalid = உண்மை;
    positions_invalid = உண்மை;

    hb_set_t visited;
    hb_set_t roots;
    TRY (find_space_roots (visited, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    visited.invert ();

    எனில் (!roots) திரும்பு Ok(பொய்);

    போது (roots)
    {
      uint32_t next = HB_SET_மதிப்பு_INVALID;
      TRY (graph_result_t<வெறுமை>::from (roots, ALLOCATION_FAILURE));
      எனில் (!roots.next (&next)) நிறுத்து;

      hb_set_t connected_roots;
      TRY (find_connected_nodes (next, roots, visited, connected_roots));
      TRY (graph_result_t<வெறுமை>::from (connected_roots, ALLOCATION_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (graph_result_t<வெறுமை>::from (connected_roots, ALLOCATION_FAILURE));

      unsigned next_space = இது->next_space ();
      எனில் (unlikely (next_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_செய்தி (SUBSET_REPACK, nullptr,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 next_space);
        திரும்பு Err(LIMIT_EXCEEDED);
      }
      இலக்கம்_roots_for_space.push (0);
      TRY(graph_result_t<வெறுமை>::from (இலக்கம்_roots_for_space, ALLOCATION_FAILURE));

      க்கு (unsigned வேர் : connected_roots)
      {
        DEBUG_செய்தி (SUBSET_REPACK, nullptr, "Subgraph %u gets space %u", வேர், next_space);
        vertices_[வேர்].space = next_space;
        இலக்கம்_roots_for_space[next_space] = இலக்கம்_roots_for_space[next_space] + 1;
        distance_invalid = உண்மை;
        positions_invalid = உண்மை;
      }

      // TODO(grieger): special case for GSUB/GPOS use extension promotions to move 16 bit space
      //                into the 32 bit space as needed, instead of using isolation.
    }

    திரும்பு Ok(உண்மை);
  }

  /*
   * Isolates the subgraph of nodes reachable from root. Any links to nodes in the subgraph
   * that originate from outside of the subgraph will be removed by duplicating the linked to
   * object.
   *
   * Indices stored in roots will be updated if any of the roots are duplicated to new indices.
   */
  graph_result_t<தருக்க> isolate_subgraph (hb_set_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_set_t parents;
    க்கு (unsigned வேர்_idx : roots)
    {
      எனில் (unlikely (வேர்_idx >= vertices_.length))
        திரும்பு Err(OUT_OF_BOUNDS);
      subgraph.set (வேர்_idx, wide_parents (வேர்_idx, parents));
    }
    க்கு (unsigned வேர்_idx : roots)
    {
      TRY (find_subgraph (வேர்_idx, subgraph));
    }
    TRY (graph_result_t<வெறுமை>::from (subgraph, ALLOCATION_FAILURE));

    hb_map_t index_map;
    தருக்க made_changes = பொய்;
    க்கு (auto entry : subgraph.iter ())
    {
      ப்ரதிஜ்ஞா (entry.first < vertices_.length);
      மாறிலி auto& node = vertices_[entry.first];
      unsigned subgraph_incoming_edges = entry.second;

      எனில் (subgraph_incoming_edges < node.incoming_edges ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = உண்மை;
        TRY (duplicate_subgraph (entry.first, index_map));
      }
    }

    TRY (graph_result_t<வெறுமை>::from (index_map, ALLOCATION_FAILURE));

    எனில் (!made_changes)
      திரும்பு Ok(பொய்);

    auto new_subgraph =
        + subgraph.சாவிகள் ()
        | hb_map([&] (uint32_t முனை_idx) {
          மாறிலி uint32_t *v;
          எனில் (index_map.has (முனை_idx, &v)) திரும்பு *v;
          திரும்பு முனை_idx;
        })
        ;

    TRY(remap_obj_indices (index_map, new_subgraph));
    TRY(remap_obj_indices (index_map, parents.iter (), உண்மை));

    // Update roots set with new indices as needed.
    க்கு (auto next : roots)
    {
      மாறிலி uint32_t *v;
      எனில் (index_map.has (next, &v))
      {
        roots.del (next);
        roots.add (*v);
      }
    }

    திரும்பு Ok(உண்மை);
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
  template <typename VisitEdgeFunc>
  graph_result_t<வெறுமை> traverse_directed_bfs (unsigned start_idx, VisitEdgeFunc&& visit_edge)
  {
    எனில் (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_செய்தி (SUBSET_REPACK, nullptr,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      திரும்பு Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<வெறுமை>::from (ordering_scratch_, ALLOCATION_FAILURE));

    எனில் (!visit_edge (HB_CODEPOINT_INVALID, nullptr, start_idx, 0))
      திரும்பு Ok();

    unsigned head = 0;
    unsigned tail = 0;
    auto& queue = ordering_scratch_;
    queue[tail++] = start_idx;

    unsigned depth = 0;
    போது (head < tail)
    {
      unsigned level_end = tail;
      போது (head < level_end)
      {
        unsigned முனை_idx = queue[head++];
        மாறிலி auto& v = vertices_[முனை_idx];

        unsigned இலக்கம்_real = v.obj().real_links.length;
        unsigned total_links = இலக்கம்_real + v.obj().virtual_links.length;

        க்கு (unsigned i = 0; i < total_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          மாறிலி auto& v = vertices_[முனை_idx];
          மாறிலி auto& link = (i < இலக்கம்_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - இலக்கம்_real];
          unsigned child_idx = link.objidx;

          எனில் (!visit_edge (முனை_idx, &link, child_idx, depth + 1))
            தொடர்;

          எனில் (unlikely (tail >= queue.length))
            திரும்பு Err(CYCLE_DETECTED);

          queue[tail++] = child_idx;
        }
      }
      depth++;
    }
    திரும்பு Ok();
  }

  graph_result_t<வெறுமை> find_subgraph (unsigned முனை_idx, hb_map_t& subgraph)
  {
    திரும்பு traverse_directed_bfs (முனை_idx, [&] (
      unsigned parent,
      மாறிலி hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      எனில் (depth == 0) திரும்பு உண்மை;
      hb_codepoint_t *மொத்தம்;
      எனில் (subgraph.has (child, &மொத்தம்))
      {
        (*மொத்தம்)++;
        திரும்பு பொய்;
      }
      subgraph.set (child, 1);
      திரும்பு உண்மை;
    });
  }

  graph_result_t<வெறுமை> find_subgraph (unsigned முனை_idx, hb_set_t& subgraph)
  {
    திரும்பு traverse_directed_bfs (முனை_idx, [&] (
      unsigned parent,
      மாறிலி hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      எனில் (subgraph.has (child)) திரும்பு பொய்;
      subgraph.add (child);
      திரும்பு உண்மை;
    });
  }

  graph_result_t<size_t> find_subgraph_size (unsigned முனை_idx, hb_set_t& subgraph, unsigned max_depth = HB_GRAPH_INVALID)
  {
    size_t size = 0;
    TRY (traverse_directed_bfs (முனை_idx, [&] (
      unsigned parent,
      மாறிலி hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      எனில் (subgraph.has (child)) திரும்பு பொய்;
      subgraph.add (child);

      மாறிலி auto& o = vertices_[child].obj();
      size += o.tail - o.head;
      திரும்பு depth < max_depth;
    }));
    திரும்பு Ok(size);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  graph_result_t<வெறுமை> find_32bit_roots (unsigned முனை_idx, hb_set_t& found)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_set_t visited;
    திரும்பு traverse_directed_bfs (முனை_idx, [&] (
      unsigned parent,
      மாறிலி hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {

      எனில் (link && found.has(parent))
        // Don't traverse from something that's already marked as a root.
        திரும்பு பொய்;

      எனில் (link && !link->is_signed && link->width == 4)
      {
        found.add (link->objidx);
        visited.add (link->objidx);
        திரும்பு பொய்;
      }

      எனில் (visited.has (child)) திரும்பு பொய்;
      visited.add (child);
      திரும்பு உண்மை;
    });
  }

  /*
   * Moves the child of old_parent_idx pointed to by old_offset to a new
   * vertex at the new_offset.
   *
   * Returns the id of the child node that was moved.
   */
  template<typename O>
  graph_result_t<unsigned> move_child (unsigned old_parent_idx,
                                       மாறிலி O* old_offset,
                                       unsigned new_parent_idx,
                                       மாறிலி O* new_offset)
  {
    distance_invalid = உண்மை;
    positions_invalid = உண்மை;

    எனில் (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      திரும்பு Err(OUT_OF_BOUNDS);

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    TRY_ASSIGN (unsigned child_id, index_for_offset (old_parent_idx,
                                                     old_offset));
    எனில் (unlikely (child_id >= vertices_.length))
      திரும்பு Err(OUT_OF_BOUNDS);

    TRY(new_v.add_real_link(O::static_size, child_id, (மாறிலி எழுத்து*) new_offset - (மாறிலி எழுத்து*) new_v.obj().head));

    auto& child = vertices_[child_id];
    TRY(child.add_parent (new_parent_idx, பொய்));

    old_v.remove_real_link_unordered (child_id, old_offset);
    child.remove_parent (old_parent_idx);

    திரும்பு Ok(child_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  template<typename O>
  graph_result_t<வெறுமை> move_children (unsigned old_parent_idx,
                                      unsigned old_pos_start,
                                      unsigned old_pos_end,
                                      unsigned new_parent_idx,
                                      unsigned new_pos_start)
  {
    எனில் (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      திரும்பு Err(OUT_OF_BOUNDS);

    distance_invalid = உண்மை;
    positions_invalid = உண்மை;

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    hb_vector_t<hb_serialize_context_t::object_t::link_t> old_links;
    க்கு (மாறிலி auto& l : old_v.obj().real_links)
    {
      எனில் (l.position < old_pos_start || l.position >= old_pos_end)
      {
        old_links.push (l);
        தொடர்;
      }

      unsigned array_pos = l.position - old_pos_start;

      unsigned child_id = l.objidx;
      எனில் (unlikely (child_id >= vertices_.length))
        திரும்பு Err(OUT_OF_BOUNDS);

      TRY(new_v.add_real_link(O::static_size, child_id, new_pos_start + array_pos));

      auto& child = vertices_[child_id];
      TRY(child.add_parent (new_parent_idx, பொய்));
      child.remove_parent (old_parent_idx);
    }

    TRY (graph_result_t<வெறுமை>::from (old_links, ALLOCATION_FAILURE));
    திரும்பு old_v.set_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  graph_result_t<வெறுமை> duplicate_subgraph (unsigned முனை_idx, hb_map_t& index_map)
  {
    graph_result_t<வெறுமை> result = Ok();
    TRY (traverse_directed_bfs (முனை_idx, [&] (
      unsigned parent,
      மாறிலி hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {
      எனில் (result.is_err () || index_map.has (child)) திரும்பு பொய்;
      auto r = duplicate (child);
      எனில் (unlikely (r.is_err ()))
      {
        result = Err(r.error ());
        திரும்பு பொய்;
      }
      index_map.set (child, *r);
      திரும்பு உண்மை;
    }));

    திரும்பு graph_result_t<வெறுமை>::from(index_map, ALLOCATION_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  graph_result_t<unsigned> duplicate (unsigned முனை_idx, தருக்க copy_table = பொய்)
  {
    எனில் (unlikely (முனை_idx >= vertices_.length))
      திரும்பு Err(OUT_OF_BOUNDS);

    எனில் (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_செய்தி (SUBSET_REPACK, nullptr,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      திரும்பு Err(LIMIT_EXCEEDED);
    }

    positions_invalid = உண்மை;
    distance_invalid = உண்மை;

    auto* clone = vertices_.push ();
    TRY (graph_result_t<வெறுமை>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<வெறுமை>::from (ordering_, ALLOCATION_FAILURE));

    auto& child = vertices_[முனை_idx];

    unsigned table_size = child.obj().tail - child.obj().head;
    எனில் (copy_table && table_size)
    {
      எழுத்து* buffer = (எழுத்து*) hb_malloc (table_size);
      எனில் (unlikely (!buffer))
        திரும்பு Err(ALLOCATION_FAILURE);
      auto res = add_buffer (buffer);
      எனில் (unlikely (res.is_err ()))
      {
        hb_free (buffer);
        திரும்பு Err(res.error ());
      }
      hb_memcpy (buffer, child.obj().head, table_size);
      clone->set_buffer(buffer, buffer + table_size);
    }
    இல்லையெனில்
    {
      clone->set_buffer(child.obj().head, child.obj().tail);
    }
    clone->distance = child.distance;
    clone->space = child.space;
    clone->reset_parents ();

    க்கு (மாறிலி auto& l : child.obj().real_links)
    {
      TRY(clone->add_real_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, பொய்));
    }

    க்கு (மாறிலி auto& l : child.obj().virtual_links)
    {
      TRY(clone->add_virtual_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, உண்மை));
    }

    திரும்பு Ok(clone_idx);
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
  graph_result_t<unsigned> duplicate (unsigned parent_idx, unsigned child_idx, தருக்க copy_table = பொய்)
  {
    எனில் (unlikely (parent_idx >= vertices_.length || child_idx >= vertices_.length))
      திரும்பு Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    மாறிலி auto& child = vertices_[child_idx];
    unsigned links_to_child = child.incoming_edges_from_parent (parent_idx);

    எனில் (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_செய்தி (SUBSET_REPACK, nullptr, "  Not duplicating %u => %u",
                 parent_idx, child_idx);
      திரும்பு Err(INVALID_ARGUMENT);
    }

    DEBUG_செய்தி (SUBSET_REPACK, nullptr, "  Duplicating %u => %u",
               parent_idx, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx, copy_table));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    எனில் (parent_idx == clone_idx) parent_idx++;

    auto& parent = vertices_[parent_idx];
    unsigned மொத்தம் = 0;
    unsigned இலக்கம்_real = parent.obj().real_links.length;
    க்கு (auto& l : parent.all_links_writer ())
    {
      மொத்தம்++;
      எனில் (l.objidx != child_idx)
        தொடர்;

      TRY(reassign_link (l, parent_idx, clone_idx, மொத்தம் > இலக்கம்_real));
    }

    திரும்பு Ok(clone_idx);
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
  graph_result_t<unsigned> duplicate (மாறிலி hb_set_t* parents, unsigned child_idx)
  {
    எனில் (unlikely (!parents || parents->is_empty ()))
      திரும்பு Err(INVALID_ARGUMENT);

    எனில் (unlikely (child_idx >= vertices_.length))
      திரும்பு Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    மாறிலி auto& child = vertices_[child_idx];
    unsigned links_to_child = 0;
    unsigned last_parent = parents->get_max ();
    unsigned first_parent = parents->get_min ();
    க்கு (unsigned parent_idx : *parents) {
      எனில் (unlikely (parent_idx >= vertices_.length))
        திரும்பு Err(OUT_OF_BOUNDS);
      links_to_child += child.incoming_edges_from_parent (parent_idx);
    }

    எனில் (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_செய்தி (SUBSET_REPACK, nullptr, "  Not duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);
      திரும்பு Err(INVALID_ARGUMENT);
    }

    DEBUG_செய்தி (SUBSET_REPACK, nullptr, "  Duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx));

    க்கு (unsigned parent_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      எனில் (parent_idx == clone_idx) parent_idx++;
      auto& parent = vertices_[parent_idx];
      unsigned மொத்தம் = 0;
      unsigned இலக்கம்_real = parent.obj().real_links.length;
      க்கு (auto& l : parent.all_links_writer ())
      {
        மொத்தம்++;
        எனில் (l.objidx != child_idx)
          தொடர்;

        TRY(reassign_link (l, parent_idx, clone_idx, மொத்தம் > இலக்கம்_real));
      }
    }

    திரும்பு Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  graph_result_t<unsigned> new_முனை (எழுத்து* head, எழுத்து* tail)
  {
    எனில் (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_செய்தி (SUBSET_REPACK, nullptr,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      திரும்பு Err(LIMIT_EXCEEDED);
    }

    positions_invalid = உண்மை;
    distance_invalid = உண்மை;

    auto* clone = vertices_.push ();
    TRY (graph_result_t<வெறுமை>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<வெறுமை>::from (ordering_, ALLOCATION_FAILURE));

    clone->set_buffer (head, tail);
    clone->distance = 0;
    clone->space = 0;

    திரும்பு Ok(clone_idx);
  }

  /*
   * Creates a new child node and remap the old child to it.
   *
   * Returns the index of the newly created child.
   *
   */
  graph_result_t<unsigned> remap_child (unsigned parent_idx, unsigned old_child_idx)
  {
    எனில் (unlikely (parent_idx >= vertices_.length || old_child_idx >= vertices_.length))
      திரும்பு Err(OUT_OF_BOUNDS);

    TRY_ASSIGN (unsigned new_child_idx, duplicate (old_child_idx));

    auto& parent = vertices_[parent_idx];
    க்கு (auto& l : parent.real_links_writer ())
    {
      எனில் (l.objidx != old_child_idx)
        தொடர்;
      TRY(reassign_link (l, parent_idx, new_child_idx, பொய்));
    }

    க்கு (auto& l : parent.virtual_links_writer ())
    {
      எனில் (l.objidx != old_child_idx)
        தொடர்;
      TRY(reassign_link (l, parent_idx, new_child_idx, உண்மை));
    }
    திரும்பு Ok(new_child_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  graph_result_t<தருக்க> raise_childrens_priority (unsigned parent_idx)
  {
    DEBUG_செய்தி (SUBSET_REPACK, nullptr, "  Raising priority of all children of %u",
               parent_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    எனில் (unlikely (parent_idx >= vertices_.length)) திரும்பு Err(OUT_OF_BOUNDS);
    தருக்க made_change = பொய்;
    க்கு (auto& l : vertices_[parent_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_priority ();
    திரும்பு Ok(made_change);
  }

  graph_result_t<வெறுமை> is_fully_connected ()
  {
    TRY(update_parents());

    எனில் (unlikely (வேர்().incoming_edges ()))
      // Root cannot have parents.
      திரும்பு Err(ORPHANED_NODES);

    க்கு (unsigned i = 0; i < வேர்_idx (); i++)
    {
      எனில் (unlikely (!vertices_[i].incoming_edges ()))
        திரும்பு Err(ORPHANED_NODES);
    }
    திரும்பு Ok();
  }

#எனில் 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  வெறுமை save_fuzzer_seed (hb_tag_t tag) மாறிலி
  {
    FILE* f = fopen ("./repacker_fuzzer_seed", "w");
    fwrite ((வெறுமை*) &tag, அளவு (tag), 1, f);

    uint16_t இலக்கம்_objects = vertices_.length;
    fwrite ((வெறுமை*) &இலக்கம்_objects, அளவு (இலக்கம்_objects), 1, f);

    க்கு (மாறிலி auto& v : vertices_)
    {
      uint16_t blob_size = v.table_size ();
      fwrite ((வெறுமை*) &blob_size, அளவு (blob_size), 1, f);
      fwrite ((மாறிலி வெறுமை*) v.obj.head, blob_size, 1, f);
    }

    uint16_t link_மொத்தம் = 0;
    க்கு (மாறிலி auto& v : vertices_)
      link_மொத்தம் += v.obj.real_links.length;

    fwrite ((வெறுமை*) &link_மொத்தம், அளவு (link_மொத்தம்), 1, f);

    வகைவரை கட்டமைப்பு
    {
      uint16_t parent;
      uint16_t child;
      uint16_t position;
      uint8_t width;
    } link_t;

    க்கு (unsigned i = 0; i < vertices_.length; i++)
    {
      க்கு (மாறிலி auto& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.width
        };
        fwrite ((வெறுமை*) &link, அளவு (link), 1, f);
      }
    }

    fclose (f);
  }
#endif

  unsigned இலக்கம்_roots_for_space_2 (unsigned space) மாறிலி
  {
    திரும்பு இலக்கம்_roots_for_space[space];
  }

  unsigned next_space () மாறிலி
  {
    திரும்பு இலக்கம்_roots_for_space.length;
  }

  graph_result_t<வெறுமை> move_to_new_space (மாறிலி hb_set_t& indices)
  {
    எனில் (unlikely (இலக்கம்_roots_for_space.length >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_செய்தி (SUBSET_REPACK, nullptr,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 இலக்கம்_roots_for_space.length);
      திரும்பு Err(LIMIT_EXCEEDED);
    }
    இலக்கம்_roots_for_space.push (0);
    TRY (graph_result_t<வெறுமை>::from (இலக்கம்_roots_for_space, ALLOCATION_FAILURE));
    unsigned new_space = இலக்கம்_roots_for_space.length - 1;

    க்கு (unsigned index : indices) {
      எனில் (unlikely (index >= vertices_.length))
        திரும்பு Err(OUT_OF_BOUNDS);
      auto& node = vertices_[index];
      இலக்கம்_roots_for_space[node.space] = இலக்கம்_roots_for_space[node.space] - 1;
      இலக்கம்_roots_for_space[new_space] = இலக்கம்_roots_for_space[new_space] + 1;
      node.space = new_space;
      distance_invalid = உண்மை;
      positions_invalid = உண்மை;
    }
    திரும்பு Ok();
  }

  unsigned space_for (unsigned index, unsigned* வேர் = nullptr) மாறிலி
  {
    போது (உண்மை)
    {
      ப்ரதிஜ்ஞா (index < vertices_.length);
      மாறிலி auto& node = vertices_[index];
      எனில் (node.space)
      {
        எனில் (வேர் != nullptr)
          *வேர் = index;
        திரும்பு node.space;
      }

      எனில் (!node.incoming_edges ())
      {
        எனில் (வேர்)
          *வேர் = index;
        திரும்பு 0;
      }

      index = *node.parents_iter ();
    }
  }

  size_t total_size_in_bytes () மாறிலி {
    size_t total_size = 0;
    unsigned மொத்தம் = vertices_.length;
    க்கு (unsigned i = 0; i < மொத்தம்; i++) {
      size_t size = vertices_.arrayZ[i].table_size ();
      total_size += size;
    }
    திரும்பு total_size;
  }


 தனிப்பட்ட:

  வெறுமை print_orphaned_nodes ()
  {
    எனில் (!DEBUG_ENABLED(SUBSET_REPACK)) திரும்பு;

    DEBUG_செய்தி (SUBSET_REPACK, nullptr, "Graph is not fully connected.");

    parents_invalid = உண்மை;
    (வெறுமை) update_parents();

    எனில் (வேர்().incoming_edges ()) {
      DEBUG_செய்தி (SUBSET_REPACK, nullptr, "Root node has incoming edges.");
    }

    க்கு (unsigned i = 0; i < vertices_.length; i++)
    {
      மாறிலி auto& v = vertices_[i];
      எனில் (!v.incoming_edges ())
        DEBUG_செய்தி (SUBSET_REPACK, nullptr, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  unsigned wide_parents (unsigned முனை_idx, hb_set_t& parents) மாறிலி
  {
    unsigned மொத்தம் = 0;
    க்கு (unsigned p : vertices_[முனை_idx].parents_iter ())
    {
      // Only real links can be wide
      க்கு (மாறிலி auto& l : vertices_[p].obj().real_links)
      {
        எனில் (l.objidx == முனை_idx
            && (l.width == 3 || l.width == 4)
            && !l.is_signed)
        {
          மொத்தம்++;
          parents.add (p);
        }
      }
    }
    திரும்பு மொத்தம்;
  }

 பொது:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  graph_result_t<வெறுமை> update_parents ()
  {
    எனில் (!parents_invalid) திரும்பு Ok();

    unsigned மொத்தம் = vertices_.length;

    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      vertices_.arrayZ[i].reset_parents ();

    க்கு (unsigned p = 0; p < மொத்தம்; p++)
    {
      க்கு (மாறிலி auto& l : vertices_.arrayZ[p].obj().real_links)
      {
        எனில் (unlikely (l.objidx >= மொத்தம்)) திரும்பு Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, பொய்));
      }

      க்கு (மாறிலி auto& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        எனில் (unlikely (l.objidx >= மொத்தம்)) திரும்பு Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, உண்மை));
      }
    }

    parents_invalid = பொய்;
    திரும்பு Ok();
  }

  /*
   * compute the serialized start and end positions for each vertex.
   */
  வெறுமை update_positions ()
  {
    எனில் (!positions_invalid) திரும்பு;

    unsigned current_pos = 0;
    க்கு (unsigned i : ordering_)
    {
      auto& v = vertices_[i];
      v.start = current_pos;
      current_pos += v.obj().tail - v.obj().head;
      v.end = current_pos;
    }

    positions_invalid = பொய்;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  graph_result_t<வெறுமை> update_distances ()
  {
    எனில் (!distance_invalid) திரும்பு Ok();

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
    unsigned மொத்தம் = vertices_.length;
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[வேர்_idx ()].distance = 0;

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (மொத்தம்);
    queue.insert (0, வேர்_idx ());
    TRY (graph_result_t<வெறுமை>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<தருக்க> visited;
    visited.resize (vertices_.length);
    TRY (graph_result_t<வெறுமை>::from (visited, ALLOCATION_FAILURE));

    போது (!queue.in_error () && !queue.is_empty ())
    {
      unsigned next_idx = queue.எடு_minimum ().second;
      எனில் (visited[next_idx]) தொடர்;
      மாறிலி auto& next = vertices_[next_idx];
      int64_t next_distance = next.distance;
      visited[next_idx] = உண்மை;

      க்கு (மாறிலி auto& link : next.obj().all_links ())
      {
        எனில் (unlikely (link.objidx >= vertices_.length))
          திரும்பு Err(OUT_OF_BOUNDS);

        எனில் (visited[link.objidx]) தொடர்;

        auto& child_v = vertices_.arrayZ[link.objidx];
        மாறிலி auto& child = child_v.obj ();
        unsigned link_width = link.width ? link.width : 4; // treat virtual offsets as 32 bits wide
        int64_t child_weight = (child.tail - child.head) +
                               ((int64_t) 1 << (link_width * 8)) * (child_v.space + 1);
        int64_t child_distance = next_distance + child_weight;

        எனில் (child_distance < child_v.distance)
        {
          child_v.distance = child_distance;
          queue.insert (child_distance, link.objidx);
        }
      }
    }

    TRY (graph_result_t<வெறுமை>::from (queue, ALLOCATION_FAILURE));
    எனில் (unlikely (!queue.is_empty ()))
    {
      print_orphaned_nodes ();
      திரும்பு Err(ORPHANED_NODES);
    }

    distance_invalid = பொய்;
    திரும்பு Ok();
  }

 தனிப்பட்ட:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  graph_result_t<வெறுமை> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      unsigned parent_idx,
                      unsigned new_idx,
                      தருக்க is_virtual)
  {
    unsigned old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_parent (parent_idx);
    திரும்பு vertices_[new_idx].add_parent (parent_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  template<typename Iterator, hb_requires (hb_is_iterator (Iterator))>
  graph_result_t<வெறுமை> remap_obj_indices (மாறிலி hb_map_t& id_map,
                                          Iterator subgraph,
                                          தருக்க only_wide = பொய்)
  {
    எனில் (!id_map) திரும்பு Ok();
    க்கு (unsigned i : subgraph)
    {
      auto& vertex = vertices_[i];
      unsigned இலக்கம்_real = vertex.obj().real_links.length;
      unsigned மொத்தம் = 0;
      க்கு (auto& link : vertex.all_links_writer ())
      {
        மொத்தம்++;
        மாறிலி uint32_t *v;
        எனில் (!id_map.has (link.objidx, &v)) தொடர்;
        எனில் (only_wide && (link.is_signed || (link.width != 4 && link.width != 3))) தொடர்;

        TRY(reassign_link (link, i, *v, மொத்தம் > இலக்கம்_real));
      }
    }
    திரும்பு Ok();
  }

  /*
   * Finds all nodes in targets that are reachable from start_idx, nodes in visited will be skipped.
   * For this search the graph is treated as being undirected.
   *
   * Connected targets will be added to connected and removed from targets. All visited nodes
   * will be added to visited.
   */
  graph_result_t<வெறுமை> find_connected_nodes (unsigned start_idx,
                                             hb_set_t& targets,
                                             hb_set_t& visited,
                                             hb_set_t& connected)
  {
    TRY (graph_result_t<வெறுமை>::from (visited, ALLOCATION_FAILURE));
    எனில் (visited.has (start_idx)) திரும்பு Ok();
    எனில் (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_செய்தி (SUBSET_REPACK, nullptr,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      திரும்பு Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<வெறுமை>::from (ordering_scratch_, ALLOCATION_FAILURE));

    auto& அடுக்கு = ordering_scratch_;
    unsigned அடுக்கு_len = 0;

    graph_result_t<வெறுமை> res = Ok();
    auto handle_முனை = [&] (unsigned முனை_idx) {
      visited.add (முனை_idx);
      எனில் (targets.has (முனை_idx)) {
        targets.del (முனை_idx);
        connected.add (முனை_idx);
      }

      எனில் (unlikely (அடுக்கு_len >= அடுக்கு.length))
      {
        res = Err(CYCLE_DETECTED);
        திரும்பு பொய்;
      }
      அடுக்கு[அடுக்கு_len++] = முனை_idx;
      திரும்பு உண்மை;
    };

    எனில் (!handle_முனை (start_idx)) திரும்பு res;

    போது (அடுக்கு_len > 0)
    {
      unsigned முனை_idx = அடுக்கு[--அடுக்கு_len];
      மாறிலி auto& v = vertices_[முனை_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      க்கு (மாறிலி auto& l : v.obj().all_links ())
      {
        unsigned child_idx = l.objidx;
        எனில் (visited.has (child_idx)) தொடர்;
        எனில் (!handle_முனை (child_idx)) திரும்பு res;
      }

      க்கு (unsigned parent_idx : v.parents_iter ())
      {
        எனில் (visited.has (parent_idx)) தொடர்;
        எனில் (!handle_முனை (parent_idx)) திரும்பு res;
      }
    }

    TRY (graph_result_t<வெறுமை>::from (visited, ALLOCATION_FAILURE));
    TRY (graph_result_t<வெறுமை>::from (connected, ALLOCATION_FAILURE));

    திரும்பு Ok();
  }

 பொது:
  // TODO(garretrieger): make private, will need to move most of offset overflow code into graph.
  hb_vector_t<vertex_t> vertices_;

  // Specifies the current topological ordering of this graph
  //
  // ordering_[pos] = obj index
  //
  // specifies that the 'pos'th spot is filled by the object
  // given by obj index.
  hb_vector_t<unsigned> ordering_;
  hb_vector_t<unsigned> ordering_scratch_;

 தனிப்பட்ட:
  தருக்க parents_invalid = உண்மை;
  தருக்க distance_invalid = உண்மை;
  தருக்க positions_invalid = உண்மை;
  hb_vector_t<unsigned> இலக்கம்_roots_for_space;
  hb_vector_t<எழுத்து*> buffers;
};

}

#endif  // GRAPH_GRAPH_HH
