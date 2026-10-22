/*
 * Copyright © 2012,2017  Google, Inc.
 * Copyright © 2021 Behdad Esfahbod
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
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_BIT_SET_HH
#define HB_BIT_SET_HH

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-bit-page.hh"

संरचना hb_bit_set_t
{
  hb_bit_set_t () = पूर्वनिर्धारित;
  ~hb_bit_set_t () = पूर्वनिर्धारित;

  hb_bit_set_t (स्थिरांक hb_bit_set_t& other) : hb_bit_set_t () { set (other, सत्य); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*हे, other); }
  hb_bit_set_t& चालक= (स्थिरांक hb_bit_set_t& other) { set (other); परत *हे; }
  hb_bit_set_t& चालक= (hb_bit_set_t&& other)  noexcept { hb_swap (*हे, other); परत *हे; }
  मित्र शून्य swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    जर (likely (!a.successful || !b.successful))
      परत;
    hb_swap (a.population, b.population);
    hb_swap (a.last_page_lookup, b.last_page_lookup);
    hb_swap (a.page_map, b.page_map);
    hb_swap (a.pages, b.pages);
  }

  शून्य init ()
  {
    successful = सत्य;
    population = 0;
    last_page_lookup = 0;
    page_map.init ();
    pages.init ();
  }
  शून्य fini ()
  {
    page_map.fini ();
    pages.fini ();
  }

  using page_t = hb_bit_page_t;
  संरचना page_map_t
  {
    पूर्णांक cmp (स्थिरांक page_map_t &o) स्थिरांक { परत cmp (o.major); }
    पूर्णांक cmp (uint32_t o_major) स्थिरांक { परत (पूर्णांक) o_major - (पूर्णांक) major; }

    uint32_t major;
    uint32_t index;
  };

  तार्किक successful = सत्य; /* Allocations successful */
  mutable अचिन्हित पूर्णांक population = 0;
  mutable hb_atomic_t<अचिन्हित> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  शून्य err () { जर (successful) successful = असत्य; } /* TODO Remove */
  तार्किक in_error () स्थिरांक { परत !successful; }

  तार्किक resize (अचिन्हित पूर्णांक मोजणी, तार्किक clear = सत्य, तार्किक exact_size = असत्य)
  {
    जर (unlikely (!successful)) परत असत्य;

    जर (pages.length < मोजणी && (अचिन्हित) pages.allocated < मोजणी && मोजणी <= 2)
      exact_size = सत्य; // Most sets are small and local

    जर (unlikely (!pages.resize_full (मोजणी, clear, exact_size) ||
	!page_map.resize_full (मोजणी, clear, असत्य)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = असत्य;
      परत असत्य;
    }
    परत सत्य;
  }

  शून्य alloc (अचिन्हित sz)
  {
    sz >>= (page_t::PAGE_BITS_LOG_2 - 1);
    pages.alloc (sz);
    page_map.alloc (sz);
  }

  hb_bit_set_t& reset ()
  {
    successful = सत्य;
    clear ();
    परत *हे;
  }

  शून्य clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    जर (!pages.length && !population) परत;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  तार्किक is_empty () स्थिरांक
  {
    अचिन्हित पूर्णांक मोजणी = pages.length;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (!pages[i].is_empty ())
	परत असत्य;
    परत सत्य;
  }
  explicit चालक तार्किक () स्थिरांक { परत !is_empty (); }

  uint32_t hash () स्थिरांक
  {
    uint32_t h = 0;
    साठी (स्वतः &map : page_map)
    {
      स्वतः &page = pages.arrayZ[map.index];
      जर (unlikely (page.is_empty ())) सुरूठेवा;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    परत h;
  }

  खाजगी:
  शून्य dirty () { population = अच_पूर्णांक_उच्च; }
  सार्वजनिक:

  शून्य add (hb_codepoint_t g)
  {
    जर (unlikely (!successful)) परत;
    जर (unlikely (g == INVALID)) परत;
    dirty ();
    page_t *page = page_for (g, सत्य); जर (unlikely (!page)) परत;
    page->add (g);
  }
  शून्य add_bits (hb_codepoint_t g, uint64_t bits)
  {
    जर (unlikely (!successful) || unlikely (!bits)) परत;
    प्रतिज्ञा (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, सत्य); जर (unlikely (!page)) परत;
    page->add_bits (g, bits);
  }
  तार्किक add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    जर (unlikely (!successful)) परत सत्य; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    जर (unlikely (a > b || a == INVALID || b == INVALID)) परत असत्य;
    dirty ();
    अचिन्हित पूर्णांक ma = get_major (a);
    अचिन्हित पूर्णांक mb = get_major (b);
    जर (ma == mb)
    {
      page_t *page = page_for (a, सत्य); जर (unlikely (!page)) परत असत्य;
      page->add_range (a, b);
    }
    अन्यथा
    {
      page_t *page = page_for (a, सत्य); जर (unlikely (!page)) परत असत्य;
      page->add_range (a, major_start (ma + 1) - 1);

      साठी (अचिन्हित पूर्णांक m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), सत्य); जर (unlikely (!page)) परत असत्य;
	page->init1 ();
      }

      page = page_for (b, सत्य); जर (unlikely (!page)) परत असत्य;
      page->add_range (major_start (mb), b);
    }
    परत सत्य;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  ढाँचा<typename Type>
  स्थिर ओळीत स्थिरांक Type& StructAtOffsetUnaligned(स्थिरांक शून्य *P, अचिन्हित पूर्णांक offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    परत * reinterpret_cast<स्थिरांक Type*> ((स्थिरांक अक्षर *) P + offset);
#pragma GCC diagnostic काढा
  }

  ढाँचा <typename T>
  शून्य set_array (तार्किक v, स्थिरांक T *array, अचिन्हित पूर्णांक मोजणी, अचिन्हित पूर्णांक stride=आकार(T))
  {
    जर (unlikely (!successful)) परत;
    जर (!मोजणी) परत;
    dirty ();
    hb_codepoint_t g = *array;
    पर्यंत (मोजणी)
    {
      अचिन्हित पूर्णांक m = get_major (g);
      page_t *page = page_for (g, v); जर (unlikely (v && !page)) परत;
      अचिन्हित पूर्णांक start = major_start (m);
      अचिन्हित पूर्णांक end = major_start (m + 1);
      करा
      {
        जर (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	मोजणी--;
      }
      पर्यंत (मोजणी && (g = *array, start <= g && g < end));
    }
  }

  ढाँचा <typename T>
  शून्य add_array (स्थिरांक T *array, अचिन्हित पूर्णांक मोजणी, अचिन्हित पूर्णांक stride=आकार(T))
  { set_array (सत्य, array, मोजणी, stride); }
  ढाँचा <typename T>
  शून्य add_array (स्थिरांक hb_array_t<स्थिरांक T>& arr) { add_array (&arr, arr.len ()); }

  ढाँचा <typename T>
  शून्य del_array (स्थिरांक T *array, अचिन्हित पूर्णांक मोजणी, अचिन्हित पूर्णांक stride=आकार(T))
  { set_array (असत्य, array, मोजणी, stride); }
  ढाँचा <typename T>
  शून्य del_array (स्थिरांक hb_array_t<स्थिरांक T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ढाँचा <typename T>
  तार्किक set_sorted_array (तार्किक v, स्थिरांक T *array, अचिन्हित पूर्णांक मोजणी, अचिन्हित पूर्णांक stride=आकार(T))
  {
    जर (unlikely (!successful)) परत सत्य; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    जर (unlikely (!मोजणी)) परत सत्य;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    पर्यंत (मोजणी)
    {
      अचिन्हित पूर्णांक m = get_major (g);
      page_t *page = page_for (g, v); जर (unlikely (v && !page)) परत असत्य;
      अचिन्हित पूर्णांक end = major_start (m + 1);
      करा
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	जर (g < last_g) परत असत्य;
	last_g = g;

        जर (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	मोजणी--;
      }
      पर्यंत (मोजणी && (g = *array, g < end));
    }
    परत सत्य;
  }

  ढाँचा <typename T>
  तार्किक add_sorted_array (स्थिरांक T *array, अचिन्हित पूर्णांक मोजणी, अचिन्हित पूर्णांक stride=आकार(T))
  { परत set_sorted_array (सत्य, array, मोजणी, stride); }
  ढाँचा <typename T>
  तार्किक add_sorted_array (स्थिरांक hb_sorted_array_t<स्थिरांक T>& arr) { परत add_sorted_array (&arr, arr.len ()); }

  ढाँचा <typename T>
  तार्किक del_sorted_array (स्थिरांक T *array, अचिन्हित पूर्णांक मोजणी, अचिन्हित पूर्णांक stride=आकार(T))
  { परत set_sorted_array (असत्य, array, मोजणी, stride); }
  ढाँचा <typename T>
  तार्किक del_sorted_array (स्थिरांक hb_sorted_array_t<स्थिरांक T>& arr) { परत del_sorted_array (&arr, arr.len ()); }

  शून्य del (hb_codepoint_t g)
  {
    जर (unlikely (!successful)) परत;
    page_t *page = page_for (g);
    जर (!page)
      परत;
    dirty ();
    page->del (g);
  }
  शून्य del_bits (hb_codepoint_t g, uint64_t bits)
  {
    जर (unlikely (!successful) || unlikely (!bits)) परत;
    प्रतिज्ञा (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    जर (!page)
      परत;
    dirty ();
    page->del_bits (g, bits);
  }

  खाजगी:
  शून्य del_pages (पूर्णांक ds, पूर्णांक de)
  {
    जर (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<अचिन्हित> compact_workspace;
      जर (unlikely (!allocate_compact_workspace (compact_workspace))) परत;

      अचिन्हित पूर्णांक write_index = 0;
      साठी (अचिन्हित पूर्णांक i = 0; i < page_map.length; i++)
      {
	पूर्णांक m = (पूर्णांक) page_map.arrayZ[i].major;
	जर (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  सार्वजनिक:
  शून्य del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    जर (unlikely (!successful)) परत;
    जर (unlikely (a > b || a == INVALID)) परत;
    dirty ();
    अचिन्हित पूर्णांक ma = get_major (a);
    अचिन्हित पूर्णांक mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    पूर्णांक ds = (a == major_start (ma))? (पूर्णांक) ma: (पूर्णांक) (ma + 1);
    पूर्णांक de = (b + 1 == major_start (mb + 1))? (पूर्णांक) mb: ((पूर्णांक) mb - 1);
    जर (ds > de || (पूर्णांक) ma < ds)
    {
      page_t *page = page_for (a);
      जर (page)
      {
	जर (ma == mb)
	  page->del_range (a, b);
	अन्यथा
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    जर (de < (पूर्णांक) mb && ma != mb)
    {
      page_t *page = page_for (b);
      जर (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  तार्किक get (hb_codepoint_t g) स्थिरांक
  {
    स्थिरांक page_t *page = page_for (g);
    जर (!page)
      परत असत्य;
    परत page->get (g);
  }
  तार्किक may_have (hb_codepoint_t g) स्थिरांक { परत get (g); }

  /* Has interface. */
  तार्किक चालक [] (hb_codepoint_t k) स्थिरांक { परत get (k); }
  तार्किक has (hb_codepoint_t k) स्थिरांक { परत (*हे)[k]; }
  /* Predicate. */
  तार्किक चालक () (hb_codepoint_t k) स्थिरांक { परत has (k); }

  /* Sink interface. */
  hb_bit_set_t& चालक << (hb_codepoint_t v)
  { add (v); परत *हे; }
  hb_bit_set_t& चालक << (स्थिरांक hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); परत *हे; }

  तार्किक intersects (स्थिरांक hb_bit_set_t &other) स्थिरांक
  {
    अचिन्हित पूर्णांक na = pages.length;
    अचिन्हित पूर्णांक nb = other.pages.length;

    अचिन्हित पूर्णांक a = 0, b = 0;
    साठी (; a < na && b < nb; )
    {
      जर (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	जर (page_at (a).intersects (other.page_at (b)))
	  परत सत्य;
	a++;
	b++;
      }
      अन्यथा जर (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      अन्यथा
	b++;
    }
    परत असत्य;
  }
  तार्किक may_intersect (स्थिरांक hb_bit_set_t &other) स्थिरांक
  { परत intersects (other); }

  तार्किक intersects (hb_codepoint_t first, hb_codepoint_t last) स्थिरांक
  {
    hb_codepoint_t c = first - 1;
    परत next (&c) && c <= last;
  }
  शून्य set (स्थिरांक hb_bit_set_t &other, तार्किक exact_size = असत्य)
  {
    जर (unlikely (!successful)) परत;
    अचिन्हित पूर्णांक मोजणी = other.pages.length;
    जर (unlikely (!resize  (मोजणी, असत्य, exact_size)))
      परत;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  तार्किक is_equal (स्थिरांक hb_bit_set_t &other) स्थिरांक
  {
    जर (has_population () && other.has_population () &&
	population != other.population)
      परत असत्य;

    अचिन्हित पूर्णांक na = pages.length;
    अचिन्हित पूर्णांक nb = other.pages.length;

    अचिन्हित पूर्णांक a = 0, b = 0;
    साठी (; a < na && b < nb; )
    {
      जर (page_at (a).is_empty ()) { a++; सुरूठेवा; }
      जर (other.page_at (b).is_empty ()) { b++; सुरूठेवा; }
      जर (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	परत असत्य;
      a++;
      b++;
    }
    साठी (; a < na; a++)
      जर (!page_at (a).is_empty ()) { परत असत्य; }
    साठी (; b < nb; b++)
      जर (!other.page_at (b).is_empty ()) { परत असत्य; }

    परत सत्य;
  }

  तार्किक is_subset (स्थिरांक hb_bit_set_t &larger_set) स्थिरांक
  {
    जर (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      परत असत्य;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    पर्यंत (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      स्वतः sp = page_at (spi);

      जर (spm < lpm) {
        जर (!sp.is_empty ())
          परत असत्य;
        spi++;
        सुरूठेवा;
      }

      जर (lpm < spm) {
        lpi++;
        सुरूठेवा;
      }

      स्वतः lp = larger_set.page_at (lpi);
      जर (!sp.is_subset (lp))
        परत असत्य;

      spi++;
      lpi++;
    }

    पर्यंत (spi < page_map.length)
      जर (!page_at (spi++).is_empty ())
        परत असत्य;

    परत सत्य;
  }

  खाजगी:
  तार्किक allocate_compact_workspace (hb_vector_t<अचिन्हित>& workspace)
  {
    जर (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = असत्य;
      परत असत्य;
    }

    परत सत्य;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  शून्य compact (hb_vector_t<अचिन्हित>& workspace,
                अचिन्हित पूर्णांक length)
  {
    प्रतिज्ञा(workspace.length == pages.length);
    hb_vector_t<अचिन्हित>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    साठी (अचिन्हित i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  शून्य compact_pages (स्थिरांक hb_vector_t<अचिन्हित>& old_index_to_page_map_index)
  {
    अचिन्हित पूर्णांक write_index = 0;
    साठी (अचिन्हित पूर्णांक i = 0; i < pages.length; i++)
    {
      जर (old_index_to_page_map_index[i] == 0xFFFFFFFF) सुरूठेवा;

      जर (write_index < i)
	pages[write_index] = pages[i];

      page_map[old_index_to_page_map_index[i]].index = write_index;
      write_index++;
    }
  }
  सार्वजनिक:

  शून्य process_ (hb_bit_page_t::vector_t (*op) (स्थिरांक hb_bit_page_t::vector_t &, स्थिरांक hb_bit_page_t::vector_t &),
		 तार्किक passthru_डावा, तार्किक passthru_उजवा,
		 स्थिरांक hb_bit_set_t &other,
		 hb_vector_t<अचिन्हित> *workspace = nullptr)
  {
    जर (unlikely (!successful)) परत;

    dirty ();

    अचिन्हित पूर्णांक na = pages.length;
    अचिन्हित पूर्णांक nb = other.pages.length;
    अचिन्हित पूर्णांक next_page = na;

    अचिन्हित पूर्णांक मोजणी = 0, new_मोजणी = 0;
    अचिन्हित पूर्णांक a = 0, b = 0;
    अचिन्हित पूर्णांक write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<अचिन्हित> local_workspace;
    hb_vector_t<अचिन्हित> &compact_workspace = workspace ? *workspace : local_workspace;
    जर (!passthru_डावा)
    {
      तार्किक allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      जर (unlikely (!allocated))
      {
	successful = असत्य;
	परत;
      }
    }

    साठी (; a < na && b < nb; )
    {
      जर (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	जर (!passthru_डावा)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  जर (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	मोजणी++;
	a++;
	b++;
      }
      अन्यथा जर (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	जर (passthru_डावा)
	  मोजणी++;
	a++;
      }
      अन्यथा
      {
	जर (passthru_उजवा)
	  मोजणी++;
	b++;
      }
    }
    जर (passthru_डावा)
      मोजणी += na - a;
    जर (passthru_उजवा)
      मोजणी += nb - b;

    जर (!passthru_डावा)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    जर (unlikely (!resize (मोजणी)))
      परत;

    new_मोजणी = मोजणी;

    /* Process in-place backward. */
    a = na;
    b = nb;
    साठी (; a && b; )
    {
      जर (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	मोजणी--;
	page_map.arrayZ[मोजणी] = page_map.arrayZ[a];
	page_at (मोजणी).v = op (page_at (a).v, other.page_at (b).v);
	page_at (मोजणी).dirty ();
      }
      अन्यथा जर (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	जर (passthru_डावा)
	{
	  मोजणी--;
	  page_map.arrayZ[मोजणी] = page_map.arrayZ[a];
	}
      }
      अन्यथा
      {
	b--;
	जर (passthru_उजवा)
	{
	  मोजणी--;
	  page_map.arrayZ[मोजणी].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[मोजणी].index = next_page++;
	  page_at (मोजणी) = other.page_at (b);
	}
      }
    }
    जर (passthru_डावा)
      पर्यंत (a)
      {
	a--;
	मोजणी--;
	page_map.arrayZ[मोजणी] = page_map.arrayZ[a];
      }
    जर (passthru_उजवा)
      पर्यंत (b)
      {
	b--;
	मोजणी--;
	page_map.arrayZ[मोजणी].major = other.page_map.arrayZ[b].major;
	page_map.arrayZ[मोजणी].index = next_page++;
	page_at (मोजणी) = other.page_at (b);
      }
    प्रतिज्ञा (!मोजणी);
    resize (new_मोजणी);
  }
  ढाँचा <typename Op>
  स्थिर hb_bit_page_t::vector_t
  op_ (स्थिरांक hb_bit_page_t::vector_t &a, स्थिरांक hb_bit_page_t::vector_t &b)
  { परत Op{} (a, b); }
  ढाँचा <typename Op>
  शून्य process (स्थिरांक Op& op,
		स्थिरांक hb_bit_set_t &other,
		hb_vector_t<अचिन्हित> *workspace = nullptr)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  शून्य union_ (स्थिरांक hb_bit_set_t &other) { process (hb_bitwise_or, other); }
  शून्य intersect (स्थिरांक hb_bit_set_t &other,
		  hb_vector_t<अचिन्हित> *workspace = nullptr)
  { process (hb_bitwise_and, other, workspace); }
  शून्य subtract (स्थिरांक hb_bit_set_t &other) { process (hb_bitwise_gt, other); }
  शून्य symmetric_difference (स्थिरांक hb_bit_set_t &other) { process (hb_bitwise_xor, other); }

  तार्किक next (hb_codepoint_t *codepoint) स्थिरांक
  {
    जर (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      परत *codepoint != INVALID;
    }

    स्थिरांक स्वतः* page_map_array = page_map.arrayZ;
    अचिन्हित पूर्णांक major = get_major (*codepoint);
    अचिन्हित पूर्णांक i = last_page_lookup;

    जर (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      जर (i >= page_map.length) {
        *codepoint = INVALID;
        परत असत्य;
      }
      last_page_lookup = i;
    }

    स्थिरांक स्वतः* pages_array = pages.arrayZ;
    स्थिरांक page_map_t &current = page_map_array[i];
    जर (likely (current.major == major))
    {
      जर (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        परत सत्य;
      }
      i++;
    }

    साठी (; i < page_map.length; i++)
    {
      स्थिरांक page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      जर (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	परत सत्य;
      }
    }
    *codepoint = INVALID;
    परत असत्य;
  }
  तार्किक next_bits (hb_codepoint_t *codepoint, uint64_t *bits) स्थिरांक
  {
    static_assert (page_t::ELT_BITS == 64, "");

    अचिन्हित पूर्णांक i = 0;
    अचिन्हित पूर्णांक elt = 0;
    जर (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      जर (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        जा done;
      base += page_t::ELT_BITS;

      अचिन्हित पूर्णांक major = get_major (base);
      i = last_page_lookup;
      जर (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	जर (i >= page_map.length)
	  जा done;
      }
      जर (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    साठी (; i < page_map.length; i++, elt = 0)
    {
      स्थिरांक page_map_t &map = page_map.arrayZ[i];
      स्थिरांक page_t &page = pages.arrayZ[map.index];
      साठी (; elt < page_t::len (); elt++)
	जर (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  परत सत्य;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    परत असत्य;
  }
  तार्किक previous (hb_codepoint_t *codepoint) स्थिरांक
  {
    जर (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      परत *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    अचिन्हित पूर्णांक i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    जर (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      जर (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	परत सत्य;
      }
    }
    i--;
    साठी (; (पूर्णांक) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      जर (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	परत सत्य;
      }
    }
    *codepoint = INVALID;
    परत असत्य;
  }
  तार्किक next_range (hb_codepoint_t *first, hb_codepoint_t *last) स्थिरांक
  {
    hb_codepoint_t i;

    i = *last;
    जर (!next (&i))
    {
      *last = *first = INVALID;
      परत असत्य;
    }

    *last = *first = i;

    स्थिरांक स्वतः* page_map_array = page_map.arrayZ;
    स्थिरांक स्वतः* pages_array = pages.arrayZ;
    अचिन्हित पूर्णांक major = get_major (*last);
    अचिन्हित पूर्णांक p_index = last_page_lookup;

    जर (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      प्रतिज्ञा(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    अचिन्हित पूर्णांक rem = page_remainder (*last);
    अचिन्हित पूर्णांक elt = rem / page_t::ELT_BITS;
    अचिन्हित पूर्णांक bit = rem & page_t::ELT_MASK;

    स्थिरांक page_t *page = &pages_array[page_map_array[p_index].index];
    अचिन्हित पूर्णांक run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    जर (run < page_t::ELT_BITS - bit)
      परत सत्य;

    elt++;

    पर्यंत (सत्य)
    {
      साठी (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        जर (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          सुरूठेवा;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        परत सत्य;
      }

      p_index++;


      जर (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        परत सत्य;

      स्थिरांक स्वतः& entry = page_map_array[p_index];
      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = 0;
    }
  }

  तार्किक previous_range (hb_codepoint_t *first, hb_codepoint_t *last) स्थिरांक
  {
    hb_codepoint_t i;

    i = *first;
    जर (!previous (&i))
    {
      *last = *first = INVALID;
      परत असत्य;
    }

    *last = *first = i;

    स्थिरांक स्वतः* page_map_array = page_map.arrayZ;
    स्थिरांक स्वतः* pages_array = pages.arrayZ;
    अचिन्हित पूर्णांक major = get_major (*first);
    अचिन्हित पूर्णांक p_index = last_page_lookup;

    जर (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      प्रतिज्ञा(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    अचिन्हित पूर्णांक rem = page_remainder (*first);
    पूर्णांक elt = rem / page_t::ELT_BITS;
    अचिन्हित पूर्णांक bit = rem & page_t::ELT_MASK;

    स्थिरांक page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    अचिन्हित पूर्णांक run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    जर (run < bit + 1)
      परत सत्य;

    elt--;

    पर्यंत (सत्य)
    {
      साठी (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        जर (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          सुरूठेवा;
        }
        uint64_t inv = ~w;
        अचिन्हित पूर्णांक zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        परत सत्य;
      }


      जर (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        परत सत्य;

      p_index--;
      स्थिरांक स्वतः& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  अचिन्हित पूर्णांक next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  अचिन्हित पूर्णांक    size) स्थिरांक
  {
    // By default, start at the first bit of the first page of values.
    अचिन्हित पूर्णांक start_page = 0;
    अचिन्हित पूर्णांक start_page_मूल्य = 0;
    जर (unlikely (codepoint != INVALID))
    {
      स्थिरांक स्वतः* page_map_array = page_map.arrayZ;
      अचिन्हित पूर्णांक major = get_major (codepoint);
      अचिन्हित पूर्णांक i = last_page_lookup;
      जर (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	जर (i >= page_map.length)
	  परत 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      जर (page_map_array[i].major == major)
      {
        start_page_मूल्य = page_remainder (codepoint + 1);
        जर (unlikely (start_page_मूल्य == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_मूल्य = 0;
        }
      }
    }

    अचिन्हित पूर्णांक initial_size = size;
    साठी (अचिन्हित पूर्णांक i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      अचिन्हित पूर्णांक n = pages[page_map.arrayZ[i].index].लिख (base, start_page_मूल्य, out, size);
      out += n;
      size -= n;
      start_page_मूल्य = 0;
    }
    परत initial_size - size;
  }

  अचिन्हित पूर्णांक next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   अचिन्हित पूर्णांक    size) स्थिरांक
  {
    अचिन्हित पूर्णांक initial_size = size;
    // By default, start at the first bit of the first page of values.
    अचिन्हित पूर्णांक start_page = 0;
    अचिन्हित पूर्णांक start_page_मूल्य = 0;
    जर (unlikely (codepoint != INVALID))
    {
      स्थिरांक स्वतः* page_map_array = page_map.arrayZ;
      अचिन्हित पूर्णांक major = get_major (codepoint);
      अचिन्हित पूर्णांक i = last_page_lookup;
      जर (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        जर (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          पर्यंत (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          परत initial_size - size;
        }
      }
      start_page = i;
      जर (page_map_array[i].major == major)
      {
        start_page_मूल्य = page_remainder (codepoint + 1);
        जर (unlikely (start_page_मूल्य == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_मूल्य = 0;
        }
      }
    }

    hb_codepoint_t next_मूल्य = codepoint + 1;
    साठी (अचिन्हित पूर्णांक i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      अचिन्हित पूर्णांक n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_मूल्य, out, size, &next_मूल्य);
      out += n;
      size -= n;
      start_page_मूल्य = 0;
    }
    पर्यंत (next_मूल्य < HB_SET_मूल्य_INVALID && size) {
      *out++ = next_मूल्य++;
      size--;
    }
    परत initial_size - size;
  }

  तार्किक has_population () स्थिरांक { परत population != अच_पूर्णांक_उच्च; }
  अचिन्हित पूर्णांक get_population () स्थिरांक
  {
    जर (has_population ())
      परत population;

    अचिन्हित पूर्णांक काढा = 0;
    अचिन्हित पूर्णांक मोजणी = pages.length;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      काढा += pages[i].get_population ();

    population = काढा;
    परत काढा;
  }
  तार्किक get_singleton (hb_codepoint_t *codepoint) स्थिरांक
  {
    जर (has_population ())
    {
      जर (population != 1) परत असत्य;
      *codepoint = get_min ();
      परत सत्य;
    }

    hb_codepoint_t singleton = INVALID;
    साठी (स्थिरांक स्वतः &map : page_map)
    {
      स्थिरांक स्वतः &page = pages.arrayZ[map.index];
      साठी (अचिन्हित i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	जर (!bits) सुरूठेवा;
	जर (singleton != INVALID || (bits & (bits - 1)))
	  परत असत्य;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    जर (singleton == INVALID)
    {
      population = 0;
      परत असत्य;
    }
    population = 1;
    *codepoint = singleton;
    परत सत्य;
  }
  hb_codepoint_t get_min () स्थिरांक
  {
    अचिन्हित मोजणी = pages.length;
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
    {
      स्थिरांक स्वतः& map = page_map.arrayZ[i];
      स्थिरांक स्वतः& page = pages.arrayZ[map.index];

      जर (!page.is_empty ())
	परत map.major * page_t::PAGE_BITS + page.get_min ();
    }
    परत INVALID;
  }
  hb_codepoint_t get_max () स्थिरांक
  {
    अचिन्हित मोजणी = pages.length;
    साठी (चिन्हित i = मोजणी - 1; i >= 0; i--)
    {
      स्थिरांक स्वतः& map = page_map.arrayZ[(अचिन्हित) i];
      स्थिरांक स्वतः& page = pages.arrayZ[map.index];

      जर (!page.is_empty ())
	परत map.major * page_t::PAGE_BITS + page.get_max ();
    }
    परत INVALID;
  }

  स्थिर constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  संरचना iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    स्थिर constexpr तार्किक is_sorted_iterator = सत्य;
    स्थिर constexpr तार्किक has_fast_len = सत्य;
    iter_t (स्थिरांक hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    तार्किक init = सत्य) : s (&s_), v (INVALID), l(0)
    {
      जर (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    प्रकारव्याख्या hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () स्थिरांक { परत v; }
    तार्किक __more__ () स्थिरांक { परत v != INVALID; }
    शून्य __next__ () { s->next (&v); जर (l) l--; }
    शून्य __prev__ () { s->previous (&v); }
    अचिन्हित __len__ () स्थिरांक { परत l; }
    iter_t end () स्थिरांक { परत iter_t (*s, असत्य); }
    तार्किक चालक != (स्थिरांक iter_t& o) स्थिरांक
    { परत v != o.v; }

    संरक्षित:
    स्थिरांक hb_bit_set_t *s;
    hb_codepoint_t v;
    अचिन्हित l;
  };
  iter_t iter () स्थिरांक { परत iter_t (*हे); }
  चालक iter_t () स्थिरांक { परत iter (); }

  संरक्षित:

  page_t *page_for (hb_codepoint_t g, तार्किक insert = असत्य)
  {
    अचिन्हित major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    अचिन्हित i = last_page_lookup;
    जर (likely (i < page_map.length))
    {
      स्वतः &cached_page = page_map.arrayZ[i];
      जर (cached_page.major == major)
	परत &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    जर (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      जर (!insert)
        परत nullptr;

      जर (unlikely (!resize (pages.length + 1)))
	परत nullptr;

      pages.arrayZ[map.index].init0 ();
      स_हटाओ (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    परत &pages.arrayZ[page_map.arrayZ[i].index];
  }
  स्थिरांक page_t *page_for (hb_codepoint_t g) स्थिरांक
  {
    अचिन्हित major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    अचिन्हित i = last_page_lookup;
    जर (likely (i < page_map.length))
    {
      स्वतः &cached_page = page_map.arrayZ[i];
      जर (cached_page.major == major)
	परत &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    जर (!page_map.bfind (key, &i))
      परत nullptr;

    last_page_lookup = i;
    परत &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (अचिन्हित पूर्णांक i)
  {
    प्रतिज्ञा (i < page_map.length);
    परत pages.arrayZ[page_map.arrayZ[i].index];
  }
  स्थिरांक page_t &page_at (अचिन्हित पूर्णांक i) स्थिरांक
  {
    प्रतिज्ञा (i < page_map.length);
    परत pages.arrayZ[page_map.arrayZ[i].index];
  }
  अचिन्हित पूर्णांक get_major (hb_codepoint_t g) स्थिरांक { परत g >> page_t::PAGE_BITS_LOG_2; }
  अचिन्हित पूर्णांक page_remainder (hb_codepoint_t g) स्थिरांक { परत g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (अचिन्हित पूर्णांक major) स्थिरांक { परत major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
