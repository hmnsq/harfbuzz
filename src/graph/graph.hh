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

#अन्तर्भाव "../hb-set.hh"
#अन्तर्भाव "../hb-priority-queue.hh"
#अन्तर्भाव "../hb-serialize.hh"
#अन्तर्भाव "graph-result.hh"

#ifndef GRAPH_GRAPH_HH
#define GRAPH_GRAPH_HH

#define HB_GRAPH_INVALID ((अचिन्हित) -1)

नामाकाश graph {

/**
 * Represents a serialized table in the form of a graph.
 * Provides methods for modifying and reordering the graph.
 */
संरचना graph_t
{
  // TODO(garretrieger): experiment with using scratch vector for visited sets instead of allocating
  // a new hb_set_t each time.

  संरचना vertex_t
  {
    // TODO(garretrieger): have vertex_t gaurantee that head/tail are non null and head < tail
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): have vertex_t gaurantee that all link indexes are valid (ie < vertices_.length)
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): track where the real links are currently sorted, and automatically sort
    // prior to accessing iterator for real links. Avoids needing to manually trigger sorting by
    // downstream users.

    int64_t distance = 0 ;
    अचिन्हित space = 0 ;
    अचिन्हित start = 0;
    अचिन्हित end = 0;
    अचिन्हित priority = 0;

    खाजगी:
    hb_serialize_context_t::object_t obj_;
    अचिन्हित incoming_edges_ = 0;
    अचिन्हित single_parent = HB_GRAPH_INVALID;
    तार्किक has_incoming_virtual_edges_ = असत्य;
    hb_hashmap_t<अचिन्हित, अचिन्हित> parents;
    hb_set_t virtual_parents;
    सार्वजनिक:

    vertex_t ()
    {}

    explicit vertex_t(स्थिरांक hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    स्वतः parents_iter () स्थिरांक HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_parent, single_parent != HB_GRAPH_INVALID),
	parents.किल्ल्या_ref ()
      )
    )

    शून्य set_buffer (अक्षर* head, अक्षर* tail) {
      obj_.head = head;
      obj_.tail = tail;
    }

    शून्य shrink_buffer (माप_प्रकार मोजणी)
    {
      obj_.tail -= मोजणी;
    }

    स्वतः all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    स्वतः real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    स्वतः virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    शून्य sort_real_links () {
      obj_.real_links.क्विक ();
    }

    graph_result_t<शून्य> add_real_link (अचिन्हित width, अचिन्हित obj_idx, अचिन्हित position) {
      स्वतः* link = obj_.real_links.push ();
      TRY (graph_result_t<शून्य>::from (obj_.real_links, ALLOCATION_FAILURE));
      link->width = width;
      link->objidx = obj_idx;
      link->position = position;
      परत Ok();
    }

    graph_result_t<शून्य> add_virtual_link (अचिन्हित obj_idx) {
      स्वतः* link = obj_.virtual_links.push ();
      TRY (graph_result_t<शून्य>::from (obj_.virtual_links, ALLOCATION_FAILURE));
      link->objidx = obj_idx;
      परत Ok();
    }

    graph_result_t<शून्य> add_real_link (स्थिरांक hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      परत graph_result_t<शून्य>::from (obj_.real_links, ALLOCATION_FAILURE);
    }

    graph_result_t<शून्य> set_real_links (hb_vector_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(graph_result_t<शून्य>::from (links, ALLOCATION_FAILURE));
      obj_.real_links = links;
      परत Ok();
    }

    शून्य clear_real_links ()
    {
      obj_.real_links.reset();
    }

    शून्य clear_virtual_links ()
    {
      obj_.virtual_links.reset();
    }

    graph_result_t<शून्य> add_virtual_link (स्थिरांक hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      परत graph_result_t<शून्य>::from (obj_.virtual_links, ALLOCATION_FAILURE);
    }

    स्थिरांक hb_serialize_context_t::object_t& obj() स्थिरांक { परत obj_; }

    ढाँचा<typename T>
    T as () स्थिरांक
    {
      परत reinterpret_cast<T> (obj_.head);
    }

    तार्किक has_incoming_virtual_edges () स्थिरांक
    {
      परत has_incoming_virtual_edges_;
    }

    graph_result_t<शून्य> link_positions_valid (अचिन्हित अंक_objects, तार्किक removed_nil)
    {
      hb_set_t assigned_bytes;
      साठी (स्थिरांक स्वतः& l : obj_.real_links)
      {
        जर (unlikely (l.objidx >= अंक_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_संदेश (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid object index.");
          परत Err(INVALID_ARGUMENT);
        }

        अचिन्हित start = l.position;
        अचिन्हित end = start + l.width - 1;

        जर (unlikely (l.width < 2 || l.width > 4))
        {
          DEBUG_संदेश (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid link width.");
          परत Err(INVALID_ARGUMENT);
        }

        जर (unlikely (end >= table_size ()))
        {
          DEBUG_संदेश (SUBSET_REPACK, nullptr,
                     "Invalid graph. Link position is out of bounds.");
          परत Err(OUT_OF_BOUNDS);
        }

        जर (unlikely (assigned_bytes.intersects (start, end)))
        {
          DEBUG_संदेश (SUBSET_REPACK, nullptr,
                     "Invalid graph. Found offsets whose positions overlap.");
          परत Err(INVALID_ARGUMENT);
        }

        assigned_bytes.add_range (start, end);
      }

      परत graph_result_t<शून्य>::from (assigned_bytes, ALLOCATION_FAILURE);
    }

    शून्य normalize ()
    {
      obj_.real_links.क्विक ();
      साठी (स्थिरांक स्वतः& l : obj_.real_links)
      {
        साठी (अचिन्हित i = 0; i < l.width; i++)
        {
          obj_.head[l.position + i] = 0;
        }
      }
    }

    तार्किक equals (अचिन्हित this_index,
                 अचिन्हित other_index,
                 स्थिरांक vertex_t& other,
                 स्थिरांक graph_t& graph,
                 स्थिरांक graph_t& other_graph,
                 अचिन्हित depth) स्थिरांक
    {
      जर (!(as_bytes () == other.as_bytes ()))
      {
        DEBUG_संदेश (SUBSET_REPACK, nullptr,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_index,
                   (अचिन्हित दीर्घ) table_size (),
                   other_index,
                   (अचिन्हित दीर्घ) other.table_size (),
                   depth);

        स्वतः a = as_bytes ();
        स्वतः b = other.as_bytes ();
        पर्यंत (a || b)
        {
          DEBUG_संदेश (SUBSET_REPACK, nullptr,
                     "  0x%x %s 0x%x", (अचिन्हित) *a, (*a == *b) ? "==" : "!=", (अचिन्हित) *b);
          a++;
          b++;
        }
        परत असत्य;
      }

      परत links_equal (obj_.real_links, other.obj_.real_links, graph, other_graph, depth);
    }

    hb_bytes_t as_bytes () स्थिरांक
    {
      परत hb_bytes_t (obj_.head, table_size ());
    }

    मित्र शून्य swap (vertex_t& a, vertex_t& b)
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

    graph_result_t<hb_hashmap_t<अचिन्हित, अचिन्हित>>
    position_to_index_map () स्थिरांक
    {
      hb_hashmap_t<अचिन्हित, अचिन्हित> result;

      result.alloc (obj_.real_links.length);
      साठी (स्थिरांक स्वतः& l : obj_.real_links) {
        result.set (l.position, l.objidx);
      }

      परत graph_result_t<hb_hashmap_t<अचिन्हित, अचिन्हित>>::from (std::move(result), ALLOCATION_FAILURE);
    }

    तार्किक is_shared () स्थिरांक
    {
      परत parents.get_population () > virtual_parents.get_population () + 1;
    }

    अचिन्हित incoming_edges () स्थिरांक
    {
      जर (HB_DEBUG_SUBSET_REPACK)
       {
	प्रतिज्ञा (incoming_edges_ == (single_parent != HB_GRAPH_INVALID) +
		(parents.मूल्ये_ref () | hb_reduce (hb_add, 0)));
       }
      परत incoming_edges_;
    }

    अचिन्हित incoming_edges_from_parent (अचिन्हित parent_index) स्थिरांक {
      जर (single_parent != HB_GRAPH_INVALID) {
        परत single_parent == parent_index ? 1 : 0;
      }

      अचिन्हित* मोजणी;
      परत  parents.has(parent_index, &मोजणी) ? *मोजणी : 0;
    }

    शून्य reset_parents ()
    {
      incoming_edges_ = 0;
      has_incoming_virtual_edges_ = असत्य;
      single_parent = HB_GRAPH_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    graph_result_t<शून्य> add_parent (अचिन्हित parent_index, तार्किक is_virtual)
    {
      प्रतिज्ञा (parent_index != HB_GRAPH_INVALID);
      has_incoming_virtual_edges_ |= is_virtual;
      जर (is_virtual)
        virtual_parents.add (parent_index);

      जर (incoming_edges_ == 0)
      {
	single_parent = parent_index;
	incoming_edges_ = 1;
	परत to_result();
      }
      अन्यथा जर (single_parent != HB_GRAPH_INVALID)
      {
        प्रतिज्ञा (incoming_edges_ == 1);
	जर (!parents.set (single_parent, 1))
	  परत to_result ();
	single_parent = HB_GRAPH_INVALID;
      }

      अचिन्हित *v;
      जर (parents.has (parent_index, &v))
      {
        (*v)++;
	incoming_edges_++;
      }
      अन्यथा जर (parents.set (parent_index, 1))
	incoming_edges_++;

      परत to_result();
    }

    शून्य remove_parent (अचिन्हित parent_index)
    {
      जर (parent_index == single_parent)
      {
	single_parent = HB_GRAPH_INVALID;
	incoming_edges_--;
	virtual_parents.reset ();
	परत;
      }

      अचिन्हित *v;
      जर (parents.has (parent_index, &v))
      {
	incoming_edges_--;
	जर (*v > 1)
	  (*v)--;
	अन्यथा
        {
	  parents.del (parent_index);
          virtual_parents.del (parent_index);
        }

	जर (incoming_edges_ == 1)
	{
	  single_parent = *parents.किल्ल्या ();
	  parents.reset ();
	}
      }
    }

    शून्य remove_real_link_unordered (अचिन्हित link_array_index) {
      obj_.real_links.remove_unordered (link_array_index);
    }

    शून्य remove_real_link_unordered (अचिन्हित child_index, स्थिरांक शून्य* offset)
    {
      अचिन्हित मोजणी = obj_.real_links.length;
      साठी (अचिन्हित i = 0; i < मोजणी; i++)
      {
        स्वतः& link = obj_.real_links.arrayZ[i];
        जर (link.objidx != child_index)
          सुरूठेवा;

        जर ((obj_.head + link.position) != offset)
          सुरूठेवा;

        obj_.real_links.remove_unordered (i);
        परत;
      }
    }

    graph_result_t<शून्य> remap_parent (अचिन्हित old_index, अचिन्हित new_index)
    {
      जर (single_parent != HB_GRAPH_INVALID)
      {
        जर (single_parent == old_index)
        {
	  single_parent = new_index;
          जर (virtual_parents.has (old_index))
          {
            virtual_parents.del (old_index);
            virtual_parents.add (new_index);
          }
        }
        परत graph_result_t<शून्य>::from(virtual_parents, ALLOCATION_FAILURE);
      }

      स्थिरांक अचिन्हित *pv;
      जर (parents.has (old_index, &pv))
      {
        अचिन्हित v = *pv;
	जर (!parents.set (new_index, v))
          incoming_edges_ -= v;
	parents.del (old_index);

        जर (incoming_edges_ == 1)
	{
	  single_parent = *parents.किल्ल्या ();
	  parents.reset ();
	}
      }

      जर (virtual_parents.has (old_index))
      {
        virtual_parents.del (old_index);
        virtual_parents.add (new_index);
      }

      परत graph_result_t<शून्य>::from(virtual_parents, ALLOCATION_FAILURE);
    }

    तार्किक is_leaf () स्थिरांक
    {
      परत !obj_.real_links.length && !obj_.virtual_links.length;
    }

    तार्किक raise_priority ()
    {
      जर (has_max_priority ()) परत असत्य;
      priority++;
      परत सत्य;
    }

    तार्किक give_max_priority ()
    {
      तार्किक result = असत्य;
      पर्यंत (!has_max_priority()) {
        result = सत्य;
        priority++;
      }
      परत result;
    }

    तार्किक has_max_priority () स्थिरांक {
      परत priority >= 3;
    }

    माप_प्रकार table_size () स्थिरांक {
      परत obj_.tail - obj_.head;
    }

    int64_t modified_distance (अचिन्हित order) स्थिरांक
    {
      // TODO(garretrieger): once priority is high enough, should try
      // setting distance = 0 which will force to sort immediately after
      // it's parent where possible.

      int64_t modified_distance =
          hb_clamp (distance + distance_modifier (), (int64_t) 0,
                    hb_int_max (int64_t) >> 18);
      जर (has_max_priority ()) {
        modified_distance = 0;
      }
      परत (modified_distance << 18) | (0x003FFFF & order);
    }

    int64_t distance_modifier () स्थिरांक
    {
      जर (!priority) परत 0;
      int64_t table_size = obj_.tail - obj_.head;

      जर (priority == 1)
        परत -table_size / 2;

      परत -table_size;
    }

   खाजगी:
    graph_result_t<शून्य> to_result() स्थिरांक {
      जर (unlikely (
        parents.in_error() || virtual_parents.in_error() ||
        obj_.real_links.in_error() || obj_.virtual_links.in_error()))
        परत Err(ALLOCATION_FAILURE);
      परत Ok();
    }

    तार्किक links_equal (स्थिरांक hb_vector_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      स्थिरांक hb_vector_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      स्थिरांक graph_t& graph,
                      स्थिरांक graph_t& other_graph,
                      अचिन्हित depth) स्थिरांक
    {
      स्वतः a = this_links.iter ();
      स्वतः b = other_links.iter ();

      पर्यंत (a && b)
      {
        स्थिरांक स्वतः& link_a = *a;
        स्थिरांक स्वतः& link_b = *b;

        जर (link_a.width != link_b.width ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          परत असत्य;

        जर (!graph.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_graph.vertices_[link_b.objidx], graph, other_graph, depth + 1))
          परत असत्य;

        a++;
        b++;
      }

      जर (तार्किक (a) != तार्किक (b))
        परत असत्य;

      परत सत्य;
    }
  };

  गणना vertex_mutability_t
  {
    Immutable,
    Mutable
  };

  ढाँचा <typename T, vertex_mutability_t mutability = Mutable>
  संरचना vertex_and_table_t
  {
    vertex_and_table_t () : index (0), vertex (nullptr), table (nullptr)
    {}

    ढाँचा <vertex_mutability_t other_mutability>
    vertex_and_table_t (स्थिरांक vertex_and_table_t<T, other_mutability>& o)
      : index (o.index), vertex (o.vertex), table (o.table)
    {}

    अचिन्हित index;
    typename std::conditional<mutability == Immutable, स्थिरांक vertex_t*, vertex_t*>::type vertex;
    typename std::conditional<mutability == Immutable, स्थिरांक T*, T*>::type table;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  ढाँचा<typename T>
  स्थिर graph_result_t<graph_t> create (स्थिरांक T& objects)
  {
    जर (unlikely (objects.length > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_संदेश (SUBSET_REPACK, nullptr,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.length);
      परत Err(LIMIT_EXCEEDED);
    }

    graph_t g;
    g.अंक_roots_for_space.push (1);
    TRY (graph_result_t<शून्य>::from (g.अंक_roots_for_space, ALLOCATION_FAILURE));

    तार्किक removed_nil = असत्य;
    स्थिरांक अचिन्हित मोजणी = objects.length;
    g.vertices_.alloc (मोजणी);
    g.ordering_.resize (मोजणी);
    g.ordering_scratch_.alloc (मोजणी);
    TRY (graph_result_t<शून्य>::from (g.vertices_, ALLOCATION_FAILURE));
    TRY (graph_result_t<शून्य>::from (g.ordering_, ALLOCATION_FAILURE));
    TRY (graph_result_t<शून्य>::from (g.ordering_scratch_, ALLOCATION_FAILURE));

    अचिन्हित order = objects.length;
    अचिन्हित skip = 0;
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      जर (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = सत्य;
        order--;
        g.ordering_.resize (मोजणी - 1);
        skip++;
        सुरूठेवा;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      जर (unlikely (obj.in_error ())) {
        परत Err(ALLOCATION_FAILURE);
      }
      vertex_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (मोजणी, removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      अचिन्हित obj_idx = i - skip;
      g.ordering_[--order] = obj_idx;

      जर (!removed_nil) सुरूठेवा;
      // Fix indices to account for removed nil object.
      साठी (स्वतः& l : v->all_links_writer ()) {
        l.objidx--;
      }
    }

    स्वतः r = g.is_fully_connected();
    जर (r.is_err() && r.error() == ORPHANED_NODES) {
      g.print_orphaned_nodes ();
    }
    TRY(r);

    परत Ok (std::move (g));
  }

  graph_t (स्थिरांक graph_t&) = delete;
  graph_t& चालक = (स्थिरांक graph_t&) = delete;
  graph_t (graph_t&&) = पूर्वनिर्धारित;
  graph_t& चालक = (graph_t&&) = पूर्वनिर्धारित;

  ~graph_t ()
  {
    साठी (अक्षर* b : buffers)
      hb_free (b);
  }

खाजगी:
  graph_t () = पूर्वनिर्धारित;

सार्वजनिक:

  तार्किक चालक== (स्थिरांक graph_t& other) स्थिरांक
  {
    परत मूळ ().equals (मूळ_idx(), other.मूळ_idx(), other.मूळ (), *हे, other, 0);
  }

  शून्य print () स्थिरांक {
    साठी (अचिन्हित id : ordering_)
    {
      स्थिरांक स्वतः& v = vertices_[id];
      म_लिखो("%u: %u [", id, (अचिन्हित पूर्णांक)v.table_size());
      साठी (स्थिरांक स्वतः &l : v.obj().real_links) {
        म_लिखो("%u, ", l.objidx);
      }
      साठी (स्थिरांक स्वतः &l : v.obj().virtual_links) {
        म_लिखो("v%u, ", l.objidx);
      }
      म_लिखो("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  शून्य normalize ()
  {
    साठी (स्वतः& v : vertices_.writer ())
      v.normalize ();
  }

  स्थिरांक vertex_t& मूळ () स्थिरांक
  {
    परत vertices_[मूळ_idx ()];
  }

  अचिन्हित मूळ_idx () स्थिरांक
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    परत ordering_[0];
  }

  स्थिरांक hb_serialize_context_t::object_t& object (अचिन्हित i) स्थिरांक
  {
    परत vertices_[i].obj();
  }

  graph_result_t<शून्य> add_buffer (अक्षर* buffer)
  {
    buffers.push (buffer);
    परत graph_result_t<शून्य>::from (buffers, ALLOCATION_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  ढाँचा<typename T>
  graph_result_t<शून्य> add_link (T* offset,
                                 अचिन्हित parent_id,
                                 अचिन्हित child_id)
  {
    जर (unlikely (parent_id >= vertices_.length || child_id >= vertices_.length))
      परत Err(OUT_OF_BOUNDS);
    स्वतः& v = vertices_[parent_id];
    TRY(v.add_real_link(2, child_id, (अक्षर*) offset - (अक्षर*) v.obj().head));
    परत vertices_[child_id].add_parent (parent_id, असत्य);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  graph_result_t<शून्य> sort_shortest_distance_if_needed ()
  {
    जर (!positions_invalid) परत Ok();
    परत sort_shortest_distance ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  graph_result_t<शून्य> sort_shortest_distance ()
  {
    positions_invalid = सत्य;

    जर (vertices_.length <= 1) {
      // Graph of 1 or less doesn't need sorting.
      परत Ok();
    }

    TRY (update_distances ());

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (vertices_.length);
    TRY (graph_result_t<शून्य>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<अचिन्हित> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.length);
    TRY (graph_result_t<शून्य>::from (new_ordering, ALLOCATION_FAILURE));

    hb_vector_t<अचिन्हित> removed_edges;
    removed_edges.resize (vertices_.length);
    TRY (graph_result_t<शून्य>::from (removed_edges, ALLOCATION_FAILURE));

    TRY (update_parents ());

    queue.insert (मूळ ().modified_distance (0), मूळ_idx ());
    अचिन्हित order = 1;
    अचिन्हित pos = 0;
    पर्यंत (!queue.in_error () && !queue.is_empty ())
    {
      अचिन्हित next_id = queue.काढा_minimum().second;

      जर (unlikely (pos >= new_ordering.length)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_संदेश (SUBSET_REPACK, nullptr, "Invalid graph. Contains cycle.");
        परत Err(CYCLE_DETECTED);
      }
      new_ordering[pos++] = next_id;
      स्थिरांक vertex_t& next = vertices_[next_id];

      साठी (स्थिरांक स्वतः& link : next.obj().all_links ()) {
        जर (unlikely (link.objidx >= vertices_.length))
          परत Err(OUT_OF_BOUNDS);
        removed_edges[link.objidx]++;
        स्थिरांक स्वतः& v = vertices_[link.objidx];
        जर (!(v.incoming_edges () - removed_edges[link.objidx]))
          // Add the order that the links were encountered to the priority.
          // This ensures that ties between priorities objects are broken in a consistent
          // way. More specifically this is set up so that if a set of objects have the same
          // distance they'll be added to the topological order in the order that they are
          // referenced from the parent object.
          queue.insert (v.modified_distance (order++),
                        link.objidx);
      }
    }

    TRY (graph_result_t<शून्य>::from (queue, ALLOCATION_FAILURE));
    TRY (graph_result_t<शून्य>::from (new_ordering, ALLOCATION_FAILURE));

    hb_swap (ordering_, new_ordering);

    जर (unlikely (pos != vertices_.length)) {
      print_orphaned_nodes ();
      परत Err(ORPHANED_NODES);
    }

    परत Ok();
  }

  /*
   * Finds the set of nodes (placed into roots) that should be assigned unique spaces.
   * More specifically this looks for the top most 24 bit or 32 bit links in the graph.
   * Some special casing is done that is specific to the layout of GSUB/GPOS tables.
   */
  graph_result_t<शून्य> find_space_roots (hb_set_t& visited, hb_set_t& roots)
  {
    अचिन्हित मूळ_index = मूळ_idx ();
    साठी (अचिन्हित i : ordering_)
    {
      जर (visited.has (i)) सुरूठेवा;

      // Only real links can form 32 bit spaces
      साठी (स्थिरांक स्वतः& l : vertices_[i].obj().real_links)
      {
        जर (l.is_signed || l.width < 3)
          सुरूठेवा;

        जर (i == मूळ_index && l.width == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          सुरूठेवा;

        जर (l.width >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_set_t sub_roots;
          TRY (find_32bit_roots (l.objidx, sub_roots));
          जर (sub_roots) {
            साठी (अचिन्हित sub_मूळ_idx : sub_roots) {
              roots.add (sub_मूळ_idx);
              TRY (find_subgraph (sub_मूळ_idx, visited));
            }
            सुरूठेवा;
          }
        }

        roots.add (l.objidx);
        TRY (find_subgraph (l.objidx, visited));
      }
    }

    TRY(graph_result_t<शून्य>::from (visited, ALLOCATION_FAILURE));
    TRY(graph_result_t<शून्य>::from (roots, ALLOCATION_FAILURE));
    परत Ok();
  }

  ढाँचा <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Immutable>> as_table (अचिन्हित parent, स्थिरांक शून्य* offset, Ts... ds)
  {
    TRY_ASSIGN (अचिन्हित idx, index_for_offset (parent, offset));
    परत as_table_from_index<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  ढाँचा <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Mutable>> as_mutable_table (अचिन्हित parent, स्थिरांक शून्य* offset, Ts... ds)
  {
    TRY_ASSIGN (स्वतः idx, mutable_index_for_offset (parent, offset));
    परत as_table_from_index<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  ढाँचा <typename T, vertex_mutability_t mutability = Mutable, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, mutability>> as_table_from_index (अचिन्हित index, Ts... ds)
  {
    जर (unlikely (index >= vertices_.length))
      परत Err(OUT_OF_BOUNDS);

    vertex_and_table_t<T, mutability> r;
    r.vertex = (typename std::conditional<mutability == Immutable, स्थिरांक vertex_t*, vertex_t*>::type) &vertices_[index];
    r.table = r.vertex->ढाँचा as<typename std::conditional<mutability == Immutable, स्थिरांक T*, T*>::type> ();
    r.index = index;
    जर (unlikely (!r.table))
      परत Err(INVALID_ARGUMENT);

    TRY(r.table->sanitize (*(r.vertex), std::forward<Ts>(ds)...));
    परत r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  graph_result_t<अचिन्हित> index_for_offset (अचिन्हित गाठ_idx, स्थिरांक शून्य* offset) स्थिरांक
  {
    जर (unlikely (गाठ_idx >= vertices_.length)) परत Err(OUT_OF_BOUNDS);
    स्थिरांक स्वतः& node = object (गाठ_idx);
    जर (unlikely (offset < node.head || offset >= node.tail)) परत Err(OUT_OF_BOUNDS);

    अचिन्हित मोजणी = node.real_links.length;
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      स्थिरांक स्वतः& link = node.real_links.arrayZ[i];
      जर (offset != node.head + link.position)
        सुरूठेवा;
      परत Ok(link.objidx);
    }

    परत Err(INVALID_ARGUMENT);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  graph_result_t<अचिन्हित> mutable_index_for_offset (अचिन्हित गाठ_idx, स्थिरांक शून्य* offset)
  {
    TRY_ASSIGN (अचिन्हित child_idx, index_for_offset (गाठ_idx, offset));
    जर (unlikely (child_idx >= vertices_.length))
      परत Err(OUT_OF_BOUNDS);

    स्वतः& child = vertices_[child_idx];
    साठी (अचिन्हित p : child.parents_iter ())
    {
      जर (p != गाठ_idx) {
        परत duplicate (गाठ_idx, child_idx, सत्य);
      }
    }

    परत Ok(child_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  graph_result_t<तार्किक> assign_spaces ()
  {
    TRY (update_parents ());

    साठी (स्वतः& vertex : vertices_.writer ())
    {
      vertex.space = 0;
      vertex.priority = 0;
    }
    अंक_roots_for_space.resize (1);
    TRY (graph_result_t<शून्य>::from (अंक_roots_for_space, ALLOCATION_FAILURE));
    अंक_roots_for_space[0] = 1;
    distance_invalid = सत्य;
    positions_invalid = सत्य;

    hb_set_t visited;
    hb_set_t roots;
    TRY (find_space_roots (visited, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    visited.invert ();

    जर (!roots) परत Ok(असत्य);

    पर्यंत (roots)
    {
      uint32_t next = HB_SET_मूल्य_INVALID;
      TRY (graph_result_t<शून्य>::from (roots, ALLOCATION_FAILURE));
      जर (!roots.next (&next)) थांबा;

      hb_set_t connected_roots;
      TRY (find_connected_nodes (next, roots, visited, connected_roots));
      TRY (graph_result_t<शून्य>::from (connected_roots, ALLOCATION_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (graph_result_t<शून्य>::from (connected_roots, ALLOCATION_FAILURE));

      अचिन्हित next_space = हे->next_space ();
      जर (unlikely (next_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_संदेश (SUBSET_REPACK, nullptr,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 next_space);
        परत Err(LIMIT_EXCEEDED);
      }
      अंक_roots_for_space.push (0);
      TRY(graph_result_t<शून्य>::from (अंक_roots_for_space, ALLOCATION_FAILURE));

      साठी (अचिन्हित मूळ : connected_roots)
      {
        DEBUG_संदेश (SUBSET_REPACK, nullptr, "Subgraph %u gets space %u", मूळ, next_space);
        vertices_[मूळ].space = next_space;
        अंक_roots_for_space[next_space] = अंक_roots_for_space[next_space] + 1;
        distance_invalid = सत्य;
        positions_invalid = सत्य;
      }

      // TODO(grieger): special case for GSUB/GPOS use extension promotions to move 16 bit space
      //                into the 32 bit space as needed, instead of using isolation.
    }

    परत Ok(सत्य);
  }

  /*
   * Isolates the subgraph of nodes reachable from root. Any links to nodes in the subgraph
   * that originate from outside of the subgraph will be removed by duplicating the linked to
   * object.
   *
   * Indices stored in roots will be updated if any of the roots are duplicated to new indices.
   */
  graph_result_t<तार्किक> isolate_subgraph (hb_set_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_set_t parents;
    साठी (अचिन्हित मूळ_idx : roots)
    {
      जर (unlikely (मूळ_idx >= vertices_.length))
        परत Err(OUT_OF_BOUNDS);
      subgraph.set (मूळ_idx, wide_parents (मूळ_idx, parents));
    }
    साठी (अचिन्हित मूळ_idx : roots)
    {
      TRY (find_subgraph (मूळ_idx, subgraph));
    }
    TRY (graph_result_t<शून्य>::from (subgraph, ALLOCATION_FAILURE));

    hb_map_t index_map;
    तार्किक made_changes = असत्य;
    साठी (स्वतः entry : subgraph.iter ())
    {
      प्रतिज्ञा (entry.first < vertices_.length);
      स्थिरांक स्वतः& node = vertices_[entry.first];
      अचिन्हित subgraph_incoming_edges = entry.second;

      जर (subgraph_incoming_edges < node.incoming_edges ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = सत्य;
        TRY (duplicate_subgraph (entry.first, index_map));
      }
    }

    TRY (graph_result_t<शून्य>::from (index_map, ALLOCATION_FAILURE));

    जर (!made_changes)
      परत Ok(असत्य);

    स्वतः new_subgraph =
        + subgraph.किल्ल्या ()
        | hb_map([&] (uint32_t गाठ_idx) {
          स्थिरांक uint32_t *v;
          जर (index_map.has (गाठ_idx, &v)) परत *v;
          परत गाठ_idx;
        })
        ;

    TRY(remap_obj_indices (index_map, new_subgraph));
    TRY(remap_obj_indices (index_map, parents.iter (), सत्य));

    // Update roots set with new indices as needed.
    साठी (स्वतः next : roots)
    {
      स्थिरांक uint32_t *v;
      जर (index_map.has (next, &v))
      {
        roots.del (next);
        roots.add (*v);
      }
    }

    परत Ok(सत्य);
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
  ढाँचा <typename VisitEdgeFunc>
  graph_result_t<शून्य> traverse_directed_bfs (अचिन्हित start_idx, VisitEdgeFunc&& visit_edge)
  {
    जर (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_संदेश (SUBSET_REPACK, nullptr,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      परत Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<शून्य>::from (ordering_scratch_, ALLOCATION_FAILURE));

    जर (!visit_edge (HB_CODEPOINT_INVALID, nullptr, start_idx, 0))
      परत Ok();

    अचिन्हित head = 0;
    अचिन्हित tail = 0;
    स्वतः& queue = ordering_scratch_;
    queue[tail++] = start_idx;

    अचिन्हित depth = 0;
    पर्यंत (head < tail)
    {
      अचिन्हित level_end = tail;
      पर्यंत (head < level_end)
      {
        अचिन्हित गाठ_idx = queue[head++];
        स्थिरांक स्वतः& v = vertices_[गाठ_idx];

        अचिन्हित अंक_real = v.obj().real_links.length;
        अचिन्हित total_links = अंक_real + v.obj().virtual_links.length;

        साठी (अचिन्हित i = 0; i < total_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          स्थिरांक स्वतः& v = vertices_[गाठ_idx];
          स्थिरांक स्वतः& link = (i < अंक_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - अंक_real];
          अचिन्हित child_idx = link.objidx;

          जर (!visit_edge (गाठ_idx, &link, child_idx, depth + 1))
            सुरूठेवा;

          जर (unlikely (tail >= queue.length))
            परत Err(CYCLE_DETECTED);

          queue[tail++] = child_idx;
        }
      }
      depth++;
    }
    परत Ok();
  }

  graph_result_t<शून्य> find_subgraph (अचिन्हित गाठ_idx, hb_map_t& subgraph)
  {
    परत traverse_directed_bfs (गाठ_idx, [&] (
      अचिन्हित parent,
      स्थिरांक hb_serialize_context_t::object_t::link_t* link,
      अचिन्हित child,
      अचिन्हित depth) {
      जर (depth == 0) परत सत्य;
      hb_codepoint_t *मोजणी;
      जर (subgraph.has (child, &मोजणी))
      {
        (*मोजणी)++;
        परत असत्य;
      }
      subgraph.set (child, 1);
      परत सत्य;
    });
  }

  graph_result_t<शून्य> find_subgraph (अचिन्हित गाठ_idx, hb_set_t& subgraph)
  {
    परत traverse_directed_bfs (गाठ_idx, [&] (
      अचिन्हित parent,
      स्थिरांक hb_serialize_context_t::object_t::link_t* link,
      अचिन्हित child,
      अचिन्हित depth) {
      जर (subgraph.has (child)) परत असत्य;
      subgraph.add (child);
      परत सत्य;
    });
  }

  graph_result_t<माप_प्रकार> find_subgraph_size (अचिन्हित गाठ_idx, hb_set_t& subgraph, अचिन्हित max_depth = HB_GRAPH_INVALID)
  {
    माप_प्रकार size = 0;
    TRY (traverse_directed_bfs (गाठ_idx, [&] (
      अचिन्हित parent,
      स्थिरांक hb_serialize_context_t::object_t::link_t* link,
      अचिन्हित child,
      अचिन्हित depth) {
      जर (subgraph.has (child)) परत असत्य;
      subgraph.add (child);

      स्थिरांक स्वतः& o = vertices_[child].obj();
      size += o.tail - o.head;
      परत depth < max_depth;
    }));
    परत Ok(size);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  graph_result_t<शून्य> find_32bit_roots (अचिन्हित गाठ_idx, hb_set_t& found)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_set_t visited;
    परत traverse_directed_bfs (गाठ_idx, [&] (
      अचिन्हित parent,
      स्थिरांक hb_serialize_context_t::object_t::link_t* link,
      अचिन्हित child,
      अचिन्हित _) {

      जर (link && found.has(parent))
        // Don't traverse from something that's already marked as a root.
        परत असत्य;

      जर (link && !link->is_signed && link->width == 4)
      {
        found.add (link->objidx);
        visited.add (link->objidx);
        परत असत्य;
      }

      जर (visited.has (child)) परत असत्य;
      visited.add (child);
      परत सत्य;
    });
  }

  /*
   * Moves the child of old_parent_idx pointed to by old_offset to a new
   * vertex at the new_offset.
   *
   * Returns the id of the child node that was moved.
   */
  ढाँचा<typename O>
  graph_result_t<अचिन्हित> move_child (अचिन्हित old_parent_idx,
                                       स्थिरांक O* old_offset,
                                       अचिन्हित new_parent_idx,
                                       स्थिरांक O* new_offset)
  {
    distance_invalid = सत्य;
    positions_invalid = सत्य;

    जर (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      परत Err(OUT_OF_BOUNDS);

    स्वतः& old_v = vertices_[old_parent_idx];
    स्वतः& new_v = vertices_[new_parent_idx];

    TRY_ASSIGN (अचिन्हित child_id, index_for_offset (old_parent_idx,
                                                     old_offset));
    जर (unlikely (child_id >= vertices_.length))
      परत Err(OUT_OF_BOUNDS);

    TRY(new_v.add_real_link(O::static_size, child_id, (स्थिरांक अक्षर*) new_offset - (स्थिरांक अक्षर*) new_v.obj().head));

    स्वतः& child = vertices_[child_id];
    TRY(child.add_parent (new_parent_idx, असत्य));

    old_v.remove_real_link_unordered (child_id, old_offset);
    child.remove_parent (old_parent_idx);

    परत Ok(child_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  ढाँचा<typename O>
  graph_result_t<शून्य> move_children (अचिन्हित old_parent_idx,
                                      अचिन्हित old_pos_start,
                                      अचिन्हित old_pos_end,
                                      अचिन्हित new_parent_idx,
                                      अचिन्हित new_pos_start)
  {
    जर (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      परत Err(OUT_OF_BOUNDS);

    distance_invalid = सत्य;
    positions_invalid = सत्य;

    स्वतः& old_v = vertices_[old_parent_idx];
    स्वतः& new_v = vertices_[new_parent_idx];

    hb_vector_t<hb_serialize_context_t::object_t::link_t> old_links;
    साठी (स्थिरांक स्वतः& l : old_v.obj().real_links)
    {
      जर (l.position < old_pos_start || l.position >= old_pos_end)
      {
        old_links.push (l);
        सुरूठेवा;
      }

      अचिन्हित array_pos = l.position - old_pos_start;

      अचिन्हित child_id = l.objidx;
      जर (unlikely (child_id >= vertices_.length))
        परत Err(OUT_OF_BOUNDS);

      TRY(new_v.add_real_link(O::static_size, child_id, new_pos_start + array_pos));

      स्वतः& child = vertices_[child_id];
      TRY(child.add_parent (new_parent_idx, असत्य));
      child.remove_parent (old_parent_idx);
    }

    TRY (graph_result_t<शून्य>::from (old_links, ALLOCATION_FAILURE));
    परत old_v.set_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  graph_result_t<शून्य> duplicate_subgraph (अचिन्हित गाठ_idx, hb_map_t& index_map)
  {
    graph_result_t<शून्य> result = Ok();
    TRY (traverse_directed_bfs (गाठ_idx, [&] (
      अचिन्हित parent,
      स्थिरांक hb_serialize_context_t::object_t::link_t* link,
      अचिन्हित child,
      अचिन्हित _) {
      जर (result.is_err () || index_map.has (child)) परत असत्य;
      स्वतः r = duplicate (child);
      जर (unlikely (r.is_err ()))
      {
        result = Err(r.error ());
        परत असत्य;
      }
      index_map.set (child, *r);
      परत सत्य;
    }));

    परत graph_result_t<शून्य>::from(index_map, ALLOCATION_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  graph_result_t<अचिन्हित> duplicate (अचिन्हित गाठ_idx, तार्किक copy_table = असत्य)
  {
    जर (unlikely (गाठ_idx >= vertices_.length))
      परत Err(OUT_OF_BOUNDS);

    जर (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_संदेश (SUBSET_REPACK, nullptr,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      परत Err(LIMIT_EXCEEDED);
    }

    positions_invalid = सत्य;
    distance_invalid = सत्य;

    स्वतः* clone = vertices_.push ();
    TRY (graph_result_t<शून्य>::from (vertices_, ALLOCATION_FAILURE));

    अचिन्हित clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<शून्य>::from (ordering_, ALLOCATION_FAILURE));

    स्वतः& child = vertices_[गाठ_idx];

    अचिन्हित table_size = child.obj().tail - child.obj().head;
    जर (copy_table && table_size)
    {
      अक्षर* buffer = (अक्षर*) hb_malloc (table_size);
      जर (unlikely (!buffer))
        परत Err(ALLOCATION_FAILURE);
      स्वतः res = add_buffer (buffer);
      जर (unlikely (res.is_err ()))
      {
        hb_free (buffer);
        परत Err(res.error ());
      }
      hb_memcpy (buffer, child.obj().head, table_size);
      clone->set_buffer(buffer, buffer + table_size);
    }
    अन्यथा
    {
      clone->set_buffer(child.obj().head, child.obj().tail);
    }
    clone->distance = child.distance;
    clone->space = child.space;
    clone->reset_parents ();

    साठी (स्थिरांक स्वतः& l : child.obj().real_links)
    {
      TRY(clone->add_real_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, असत्य));
    }

    साठी (स्थिरांक स्वतः& l : child.obj().virtual_links)
    {
      TRY(clone->add_virtual_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, सत्य));
    }

    परत Ok(clone_idx);
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
  graph_result_t<अचिन्हित> duplicate (अचिन्हित parent_idx, अचिन्हित child_idx, तार्किक copy_table = असत्य)
  {
    जर (unlikely (parent_idx >= vertices_.length || child_idx >= vertices_.length))
      परत Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    स्थिरांक स्वतः& child = vertices_[child_idx];
    अचिन्हित links_to_child = child.incoming_edges_from_parent (parent_idx);

    जर (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_संदेश (SUBSET_REPACK, nullptr, "  Not duplicating %u => %u",
                 parent_idx, child_idx);
      परत Err(INVALID_ARGUMENT);
    }

    DEBUG_संदेश (SUBSET_REPACK, nullptr, "  Duplicating %u => %u",
               parent_idx, child_idx);

    TRY_ASSIGN (अचिन्हित clone_idx, duplicate (child_idx, copy_table));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    जर (parent_idx == clone_idx) parent_idx++;

    स्वतः& parent = vertices_[parent_idx];
    अचिन्हित मोजणी = 0;
    अचिन्हित अंक_real = parent.obj().real_links.length;
    साठी (स्वतः& l : parent.all_links_writer ())
    {
      मोजणी++;
      जर (l.objidx != child_idx)
        सुरूठेवा;

      TRY(reassign_link (l, parent_idx, clone_idx, मोजणी > अंक_real));
    }

    परत Ok(clone_idx);
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
  graph_result_t<अचिन्हित> duplicate (स्थिरांक hb_set_t* parents, अचिन्हित child_idx)
  {
    जर (unlikely (!parents || parents->is_empty ()))
      परत Err(INVALID_ARGUMENT);

    जर (unlikely (child_idx >= vertices_.length))
      परत Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    स्थिरांक स्वतः& child = vertices_[child_idx];
    अचिन्हित links_to_child = 0;
    अचिन्हित last_parent = parents->get_max ();
    अचिन्हित first_parent = parents->get_min ();
    साठी (अचिन्हित parent_idx : *parents) {
      जर (unlikely (parent_idx >= vertices_.length))
        परत Err(OUT_OF_BOUNDS);
      links_to_child += child.incoming_edges_from_parent (parent_idx);
    }

    जर (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_संदेश (SUBSET_REPACK, nullptr, "  Not duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);
      परत Err(INVALID_ARGUMENT);
    }

    DEBUG_संदेश (SUBSET_REPACK, nullptr, "  Duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);

    TRY_ASSIGN (अचिन्हित clone_idx, duplicate (child_idx));

    साठी (अचिन्हित parent_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      जर (parent_idx == clone_idx) parent_idx++;
      स्वतः& parent = vertices_[parent_idx];
      अचिन्हित मोजणी = 0;
      अचिन्हित अंक_real = parent.obj().real_links.length;
      साठी (स्वतः& l : parent.all_links_writer ())
      {
        मोजणी++;
        जर (l.objidx != child_idx)
          सुरूठेवा;

        TRY(reassign_link (l, parent_idx, clone_idx, मोजणी > अंक_real));
      }
    }

    परत Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  graph_result_t<अचिन्हित> new_गाठ (अक्षर* head, अक्षर* tail)
  {
    जर (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_संदेश (SUBSET_REPACK, nullptr,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      परत Err(LIMIT_EXCEEDED);
    }

    positions_invalid = सत्य;
    distance_invalid = सत्य;

    स्वतः* clone = vertices_.push ();
    TRY (graph_result_t<शून्य>::from (vertices_, ALLOCATION_FAILURE));

    अचिन्हित clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<शून्य>::from (ordering_, ALLOCATION_FAILURE));

    clone->set_buffer (head, tail);
    clone->distance = 0;
    clone->space = 0;

    परत Ok(clone_idx);
  }

  /*
   * Creates a new child node and remap the old child to it.
   *
   * Returns the index of the newly created child.
   *
   */
  graph_result_t<अचिन्हित> remap_child (अचिन्हित parent_idx, अचिन्हित old_child_idx)
  {
    जर (unlikely (parent_idx >= vertices_.length || old_child_idx >= vertices_.length))
      परत Err(OUT_OF_BOUNDS);

    TRY_ASSIGN (अचिन्हित new_child_idx, duplicate (old_child_idx));

    स्वतः& parent = vertices_[parent_idx];
    साठी (स्वतः& l : parent.real_links_writer ())
    {
      जर (l.objidx != old_child_idx)
        सुरूठेवा;
      TRY(reassign_link (l, parent_idx, new_child_idx, असत्य));
    }

    साठी (स्वतः& l : parent.virtual_links_writer ())
    {
      जर (l.objidx != old_child_idx)
        सुरूठेवा;
      TRY(reassign_link (l, parent_idx, new_child_idx, सत्य));
    }
    परत Ok(new_child_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  graph_result_t<तार्किक> raise_childrens_priority (अचिन्हित parent_idx)
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr, "  Raising priority of all children of %u",
               parent_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    जर (unlikely (parent_idx >= vertices_.length)) परत Err(OUT_OF_BOUNDS);
    तार्किक made_change = असत्य;
    साठी (स्वतः& l : vertices_[parent_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_priority ();
    परत Ok(made_change);
  }

  graph_result_t<शून्य> is_fully_connected ()
  {
    TRY(update_parents());

    जर (unlikely (मूळ().incoming_edges ()))
      // Root cannot have parents.
      परत Err(ORPHANED_NODES);

    साठी (अचिन्हित i = 0; i < मूळ_idx (); i++)
    {
      जर (unlikely (!vertices_[i].incoming_edges ()))
        परत Err(ORPHANED_NODES);
    }
    परत Ok();
  }

#जर 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  शून्य save_fuzzer_seed (hb_tag_t tag) स्थिरांक
  {
    खाता* f = ख_खोलो ("./repacker_fuzzer_seed", "w");
    ख_डालो ((शून्य*) &tag, आकार (tag), 1, f);

    uint16_t अंक_objects = vertices_.length;
    ख_डालो ((शून्य*) &अंक_objects, आकार (अंक_objects), 1, f);

    साठी (स्थिरांक स्वतः& v : vertices_)
    {
      uint16_t blob_size = v.table_size ();
      ख_डालो ((शून्य*) &blob_size, आकार (blob_size), 1, f);
      ख_डालो ((स्थिरांक शून्य*) v.obj.head, blob_size, 1, f);
    }

    uint16_t link_मोजणी = 0;
    साठी (स्थिरांक स्वतः& v : vertices_)
      link_मोजणी += v.obj.real_links.length;

    ख_डालो ((शून्य*) &link_मोजणी, आकार (link_मोजणी), 1, f);

    प्रकारव्याख्या संरचना
    {
      uint16_t parent;
      uint16_t child;
      uint16_t position;
      uint8_t width;
    } link_t;

    साठी (अचिन्हित i = 0; i < vertices_.length; i++)
    {
      साठी (स्थिरांक स्वतः& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.width
        };
        ख_डालो ((शून्य*) &link, आकार (link), 1, f);
      }
    }

    ख_बंद (f);
  }
#endif

  अचिन्हित अंक_roots_for_space_2 (अचिन्हित space) स्थिरांक
  {
    परत अंक_roots_for_space[space];
  }

  अचिन्हित next_space () स्थिरांक
  {
    परत अंक_roots_for_space.length;
  }

  graph_result_t<शून्य> move_to_new_space (स्थिरांक hb_set_t& indices)
  {
    जर (unlikely (अंक_roots_for_space.length >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_संदेश (SUBSET_REPACK, nullptr,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 अंक_roots_for_space.length);
      परत Err(LIMIT_EXCEEDED);
    }
    अंक_roots_for_space.push (0);
    TRY (graph_result_t<शून्य>::from (अंक_roots_for_space, ALLOCATION_FAILURE));
    अचिन्हित new_space = अंक_roots_for_space.length - 1;

    साठी (अचिन्हित index : indices) {
      जर (unlikely (index >= vertices_.length))
        परत Err(OUT_OF_BOUNDS);
      स्वतः& node = vertices_[index];
      अंक_roots_for_space[node.space] = अंक_roots_for_space[node.space] - 1;
      अंक_roots_for_space[new_space] = अंक_roots_for_space[new_space] + 1;
      node.space = new_space;
      distance_invalid = सत्य;
      positions_invalid = सत्य;
    }
    परत Ok();
  }

  अचिन्हित space_for (अचिन्हित index, अचिन्हित* मूळ = nullptr) स्थिरांक
  {
    पर्यंत (सत्य)
    {
      प्रतिज्ञा (index < vertices_.length);
      स्थिरांक स्वतः& node = vertices_[index];
      जर (node.space)
      {
        जर (मूळ != nullptr)
          *मूळ = index;
        परत node.space;
      }

      जर (!node.incoming_edges ())
      {
        जर (मूळ)
          *मूळ = index;
        परत 0;
      }

      index = *node.parents_iter ();
    }
  }

  माप_प्रकार total_size_in_bytes () स्थिरांक {
    माप_प्रकार total_size = 0;
    अचिन्हित मोजणी = vertices_.length;
    साठी (अचिन्हित i = 0; i < मोजणी; i++) {
      माप_प्रकार size = vertices_.arrayZ[i].table_size ();
      total_size += size;
    }
    परत total_size;
  }


 खाजगी:

  शून्य print_orphaned_nodes ()
  {
    जर (!DEBUG_ENABLED(SUBSET_REPACK)) परत;

    DEBUG_संदेश (SUBSET_REPACK, nullptr, "Graph is not fully connected.");

    parents_invalid = सत्य;
    (शून्य) update_parents();

    जर (मूळ().incoming_edges ()) {
      DEBUG_संदेश (SUBSET_REPACK, nullptr, "Root node has incoming edges.");
    }

    साठी (अचिन्हित i = 0; i < vertices_.length; i++)
    {
      स्थिरांक स्वतः& v = vertices_[i];
      जर (!v.incoming_edges ())
        DEBUG_संदेश (SUBSET_REPACK, nullptr, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  अचिन्हित wide_parents (अचिन्हित गाठ_idx, hb_set_t& parents) स्थिरांक
  {
    अचिन्हित मोजणी = 0;
    साठी (अचिन्हित p : vertices_[गाठ_idx].parents_iter ())
    {
      // Only real links can be wide
      साठी (स्थिरांक स्वतः& l : vertices_[p].obj().real_links)
      {
        जर (l.objidx == गाठ_idx
            && (l.width == 3 || l.width == 4)
            && !l.is_signed)
        {
          मोजणी++;
          parents.add (p);
        }
      }
    }
    परत मोजणी;
  }

 सार्वजनिक:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  graph_result_t<शून्य> update_parents ()
  {
    जर (!parents_invalid) परत Ok();

    अचिन्हित मोजणी = vertices_.length;

    साठी (अचिन्हित i = 0; i < मोजणी; i++)
      vertices_.arrayZ[i].reset_parents ();

    साठी (अचिन्हित p = 0; p < मोजणी; p++)
    {
      साठी (स्थिरांक स्वतः& l : vertices_.arrayZ[p].obj().real_links)
      {
        जर (unlikely (l.objidx >= मोजणी)) परत Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, असत्य));
      }

      साठी (स्थिरांक स्वतः& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        जर (unlikely (l.objidx >= मोजणी)) परत Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, सत्य));
      }
    }

    parents_invalid = असत्य;
    परत Ok();
  }

  /*
   * compute the serialized start and end positions for each vertex.
   */
  शून्य update_positions ()
  {
    जर (!positions_invalid) परत;

    अचिन्हित current_pos = 0;
    साठी (अचिन्हित i : ordering_)
    {
      स्वतः& v = vertices_[i];
      v.start = current_pos;
      current_pos += v.obj().tail - v.obj().head;
      v.end = current_pos;
    }

    positions_invalid = असत्य;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  graph_result_t<शून्य> update_distances ()
  {
    जर (!distance_invalid) परत Ok();

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
    अचिन्हित मोजणी = vertices_.length;
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[मूळ_idx ()].distance = 0;

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (मोजणी);
    queue.insert (0, मूळ_idx ());
    TRY (graph_result_t<शून्य>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<तार्किक> visited;
    visited.resize (vertices_.length);
    TRY (graph_result_t<शून्य>::from (visited, ALLOCATION_FAILURE));

    पर्यंत (!queue.in_error () && !queue.is_empty ())
    {
      अचिन्हित next_idx = queue.काढा_minimum ().second;
      जर (visited[next_idx]) सुरूठेवा;
      स्थिरांक स्वतः& next = vertices_[next_idx];
      int64_t next_distance = next.distance;
      visited[next_idx] = सत्य;

      साठी (स्थिरांक स्वतः& link : next.obj().all_links ())
      {
        जर (unlikely (link.objidx >= vertices_.length))
          परत Err(OUT_OF_BOUNDS);

        जर (visited[link.objidx]) सुरूठेवा;

        स्वतः& child_v = vertices_.arrayZ[link.objidx];
        स्थिरांक स्वतः& child = child_v.obj ();
        अचिन्हित link_width = link.width ? link.width : 4; // treat virtual offsets as 32 bits wide
        int64_t child_weight = (child.tail - child.head) +
                               ((int64_t) 1 << (link_width * 8)) * (child_v.space + 1);
        int64_t child_distance = next_distance + child_weight;

        जर (child_distance < child_v.distance)
        {
          child_v.distance = child_distance;
          queue.insert (child_distance, link.objidx);
        }
      }
    }

    TRY (graph_result_t<शून्य>::from (queue, ALLOCATION_FAILURE));
    जर (unlikely (!queue.is_empty ()))
    {
      print_orphaned_nodes ();
      परत Err(ORPHANED_NODES);
    }

    distance_invalid = असत्य;
    परत Ok();
  }

 खाजगी:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  graph_result_t<शून्य> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      अचिन्हित parent_idx,
                      अचिन्हित new_idx,
                      तार्किक is_virtual)
  {
    अचिन्हित old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_parent (parent_idx);
    परत vertices_[new_idx].add_parent (parent_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  ढाँचा<typename Iterator, hb_requires (hb_is_iterator (Iterator))>
  graph_result_t<शून्य> remap_obj_indices (स्थिरांक hb_map_t& id_map,
                                          Iterator subgraph,
                                          तार्किक only_wide = असत्य)
  {
    जर (!id_map) परत Ok();
    साठी (अचिन्हित i : subgraph)
    {
      स्वतः& vertex = vertices_[i];
      अचिन्हित अंक_real = vertex.obj().real_links.length;
      अचिन्हित मोजणी = 0;
      साठी (स्वतः& link : vertex.all_links_writer ())
      {
        मोजणी++;
        स्थिरांक uint32_t *v;
        जर (!id_map.has (link.objidx, &v)) सुरूठेवा;
        जर (only_wide && (link.is_signed || (link.width != 4 && link.width != 3))) सुरूठेवा;

        TRY(reassign_link (link, i, *v, मोजणी > अंक_real));
      }
    }
    परत Ok();
  }

  /*
   * Finds all nodes in targets that are reachable from start_idx, nodes in visited will be skipped.
   * For this search the graph is treated as being undirected.
   *
   * Connected targets will be added to connected and removed from targets. All visited nodes
   * will be added to visited.
   */
  graph_result_t<शून्य> find_connected_nodes (अचिन्हित start_idx,
                                             hb_set_t& targets,
                                             hb_set_t& visited,
                                             hb_set_t& connected)
  {
    TRY (graph_result_t<शून्य>::from (visited, ALLOCATION_FAILURE));
    जर (visited.has (start_idx)) परत Ok();
    जर (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_संदेश (SUBSET_REPACK, nullptr,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      परत Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<शून्य>::from (ordering_scratch_, ALLOCATION_FAILURE));

    स्वतः& चळत = ordering_scratch_;
    अचिन्हित चळत_len = 0;

    graph_result_t<शून्य> res = Ok();
    स्वतः handle_गाठ = [&] (अचिन्हित गाठ_idx) {
      visited.add (गाठ_idx);
      जर (targets.has (गाठ_idx)) {
        targets.del (गाठ_idx);
        connected.add (गाठ_idx);
      }

      जर (unlikely (चळत_len >= चळत.length))
      {
        res = Err(CYCLE_DETECTED);
        परत असत्य;
      }
      चळत[चळत_len++] = गाठ_idx;
      परत सत्य;
    };

    जर (!handle_गाठ (start_idx)) परत res;

    पर्यंत (चळत_len > 0)
    {
      अचिन्हित गाठ_idx = चळत[--चळत_len];
      स्थिरांक स्वतः& v = vertices_[गाठ_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      साठी (स्थिरांक स्वतः& l : v.obj().all_links ())
      {
        अचिन्हित child_idx = l.objidx;
        जर (visited.has (child_idx)) सुरूठेवा;
        जर (!handle_गाठ (child_idx)) परत res;
      }

      साठी (अचिन्हित parent_idx : v.parents_iter ())
      {
        जर (visited.has (parent_idx)) सुरूठेवा;
        जर (!handle_गाठ (parent_idx)) परत res;
      }
    }

    TRY (graph_result_t<शून्य>::from (visited, ALLOCATION_FAILURE));
    TRY (graph_result_t<शून्य>::from (connected, ALLOCATION_FAILURE));

    परत Ok();
  }

 सार्वजनिक:
  // TODO(garretrieger): make private, will need to move most of offset overflow code into graph.
  hb_vector_t<vertex_t> vertices_;

  // Specifies the current topological ordering of this graph
  //
  // ordering_[pos] = obj index
  //
  // specifies that the 'pos'th spot is filled by the object
  // given by obj index.
  hb_vector_t<अचिन्हित> ordering_;
  hb_vector_t<अचिन्हित> ordering_scratch_;

 खाजगी:
  तार्किक parents_invalid = सत्य;
  तार्किक distance_invalid = सत्य;
  तार्किक positions_invalid = सत्य;
  hb_vector_t<अचिन्हित> अंक_roots_for_space;
  hb_vector_t<अक्षर*> buffers;
};

}

#endif  // GRAPH_GRAPH_HH
