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
  hb_bit_set_t () = स्वभाव;
  ~hb_bit_set_t () = स्वभाव;

  hb_bit_set_t (ध्रुव hb_bit_set_t& other) : hb_bit_set_t () { set (other, सत्य); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*एतत्, other); }
  hb_bit_set_t& चालक= (ध्रुव hb_bit_set_t& other) { set (other); प्रत्यागम *एतत्; }
  hb_bit_set_t& चालक= (hb_bit_set_t&& other)  noexcept { hb_swap (*एतत्, other); प्रत्यागम *एतत्; }
  मित्र शून्य swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    यदि (likely (!a.successful || !b.successful))
      प्रत्यागम;
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
    पूर्णाङ्क cmp (ध्रुव page_map_t &o) ध्रुव { प्रत्यागम cmp (o.major); }
    पूर्णाङ्क cmp (uint32_t o_major) ध्रुव { प्रत्यागम (पूर्णाङ्क) o_major - (पूर्णाङ्क) major; }

    uint32_t major;
    uint32_t index;
  };

  तार्किक successful = सत्य; /* Allocations successful */
  mutable अचिन्हित पूर्णाङ्क population = 0;
  mutable hb_atomic_t<अचिन्हित> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  शून्य err () { यदि (successful) successful = असत्य; } /* TODO Remove */
  तार्किक in_error () ध्रुव { प्रत्यागम !successful; }

  तार्किक resize (अचिन्हित पूर्णाङ्क गिनती, तार्किक clear = सत्य, तार्किक exact_size = असत्य)
  {
    यदि (unlikely (!successful)) प्रत्यागम असत्य;

    यदि (pages.length < गिनती && (अचिन्हित) pages.allocated < गिनती && गिनती <= 2)
      exact_size = सत्य; // Most sets are small and local

    यदि (unlikely (!pages.resize_full (गिनती, clear, exact_size) ||
	!page_map.resize_full (गिनती, clear, असत्य)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = असत्य;
      प्रत्यागम असत्य;
    }
    प्रत्यागम सत्य;
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
    प्रत्यागम *एतत्;
  }

  शून्य clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    यदि (!pages.length && !population) प्रत्यागम;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  तार्किक is_empty () ध्रुव
  {
    अचिन्हित पूर्णाङ्क गिनती = pages.length;
    प्रति (अचिन्हित पूर्णाङ्क i = 0; i < गिनती; i++)
      यदि (!pages[i].is_empty ())
	प्रत्यागम असत्य;
    प्रत्यागम सत्य;
  }
  explicit चालक तार्किक () ध्रुव { प्रत्यागम !is_empty (); }

  uint32_t hash () ध्रुव
  {
    uint32_t h = 0;
    प्रति (स्वतः &map : page_map)
    {
      स्वतः &page = pages.arrayZ[map.index];
      यदि (unlikely (page.is_empty ())) अनुवर्त;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    प्रत्यागम h;
  }

  गुप्त:
  शून्य dirty () { population = अच_पूर्णांक_उच्च; }
  सार्वजनिक:

  शून्य add (hb_codepoint_t g)
  {
    यदि (unlikely (!successful)) प्रत्यागम;
    यदि (unlikely (g == INVALID)) प्रत्यागम;
    dirty ();
    page_t *page = page_for (g, सत्य); यदि (unlikely (!page)) प्रत्यागम;
    page->add (g);
  }
  शून्य add_bits (hb_codepoint_t g, uint64_t bits)
  {
    यदि (unlikely (!successful) || unlikely (!bits)) प्रत्यागम;
    प्रतिज्ञा (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, सत्य); यदि (unlikely (!page)) प्रत्यागम;
    page->add_bits (g, bits);
  }
  तार्किक add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    यदि (unlikely (!successful)) प्रत्यागम सत्य; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    यदि (unlikely (a > b || a == INVALID || b == INVALID)) प्रत्यागम असत्य;
    dirty ();
    अचिन्हित पूर्णाङ्क ma = get_major (a);
    अचिन्हित पूर्णाङ्क mb = get_major (b);
    यदि (ma == mb)
    {
      page_t *page = page_for (a, सत्य); यदि (unlikely (!page)) प्रत्यागम असत्य;
      page->add_range (a, b);
    }
    अन्यथा
    {
      page_t *page = page_for (a, सत्य); यदि (unlikely (!page)) प्रत्यागम असत्य;
      page->add_range (a, major_start (ma + 1) - 1);

      प्रति (अचिन्हित पूर्णाङ्क m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), सत्य); यदि (unlikely (!page)) प्रत्यागम असत्य;
	page->init1 ();
      }

      page = page_for (b, सत्य); यदि (unlikely (!page)) प्रत्यागम असत्य;
      page->add_range (major_start (mb), b);
    }
    प्रत्यागम सत्य;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  ढाँचा<typename Type>
  स्थिर अन्तःस्थ ध्रुव Type& StructAtOffsetUnaligned(ध्रुव शून्य *P, अचिन्हित पूर्णाङ्क offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    प्रत्यागम * reinterpret_cast<ध्रुव Type*> ((ध्रुव वर्ण *) P + offset);
#pragma GCC diagnostic निकालो
  }

  ढाँचा <typename T>
  शून्य set_array (तार्किक v, ध्रुव T *array, अचिन्हित पूर्णाङ्क गिनती, अचिन्हित पूर्णाङ्क stride=परिमाण(T))
  {
    यदि (unlikely (!successful)) प्रत्यागम;
    यदि (!गिनती) प्रत्यागम;
    dirty ();
    hb_codepoint_t g = *array;
    यावत् (गिनती)
    {
      अचिन्हित पूर्णाङ्क m = get_major (g);
      page_t *page = page_for (g, v); यदि (unlikely (v && !page)) प्रत्यागम;
      अचिन्हित पूर्णाङ्क start = major_start (m);
      अचिन्हित पूर्णाङ्क end = major_start (m + 1);
      कुरु
      {
        यदि (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	गिनती--;
      }
      यावत् (गिनती && (g = *array, start <= g && g < end));
    }
  }

  ढाँचा <typename T>
  शून्य add_array (ध्रुव T *array, अचिन्हित पूर्णाङ्क गिनती, अचिन्हित पूर्णाङ्क stride=परिमाण(T))
  { set_array (सत्य, array, गिनती, stride); }
  ढाँचा <typename T>
  शून्य add_array (ध्रुव hb_array_t<ध्रुव T>& arr) { add_array (&arr, arr.len ()); }

  ढाँचा <typename T>
  शून्य del_array (ध्रुव T *array, अचिन्हित पूर्णाङ्क गिनती, अचिन्हित पूर्णाङ्क stride=परिमाण(T))
  { set_array (असत्य, array, गिनती, stride); }
  ढाँचा <typename T>
  शून्य del_array (ध्रुव hb_array_t<ध्रुव T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ढाँचा <typename T>
  तार्किक set_sorted_array (तार्किक v, ध्रुव T *array, अचिन्हित पूर्णाङ्क गिनती, अचिन्हित पूर्णाङ्क stride=परिमाण(T))
  {
    यदि (unlikely (!successful)) प्रत्यागम सत्य; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    यदि (unlikely (!गिनती)) प्रत्यागम सत्य;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    यावत् (गिनती)
    {
      अचिन्हित पूर्णाङ्क m = get_major (g);
      page_t *page = page_for (g, v); यदि (unlikely (v && !page)) प्रत्यागम असत्य;
      अचिन्हित पूर्णाङ्क end = major_start (m + 1);
      कुरु
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	यदि (g < last_g) प्रत्यागम असत्य;
	last_g = g;

        यदि (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	गिनती--;
      }
      यावत् (गिनती && (g = *array, g < end));
    }
    प्रत्यागम सत्य;
  }

  ढाँचा <typename T>
  तार्किक add_sorted_array (ध्रुव T *array, अचिन्हित पूर्णाङ्क गिनती, अचिन्हित पूर्णाङ्क stride=परिमाण(T))
  { प्रत्यागम set_sorted_array (सत्य, array, गिनती, stride); }
  ढाँचा <typename T>
  तार्किक add_sorted_array (ध्रुव hb_sorted_array_t<ध्रुव T>& arr) { प्रत्यागम add_sorted_array (&arr, arr.len ()); }

  ढाँचा <typename T>
  तार्किक del_sorted_array (ध्रुव T *array, अचिन्हित पूर्णाङ्क गिनती, अचिन्हित पूर्णाङ्क stride=परिमाण(T))
  { प्रत्यागम set_sorted_array (असत्य, array, गिनती, stride); }
  ढाँचा <typename T>
  तार्किक del_sorted_array (ध्रुव hb_sorted_array_t<ध्रुव T>& arr) { प्रत्यागम del_sorted_array (&arr, arr.len ()); }

  शून्य del (hb_codepoint_t g)
  {
    यदि (unlikely (!successful)) प्रत्यागम;
    page_t *page = page_for (g);
    यदि (!page)
      प्रत्यागम;
    dirty ();
    page->del (g);
  }
  शून्य del_bits (hb_codepoint_t g, uint64_t bits)
  {
    यदि (unlikely (!successful) || unlikely (!bits)) प्रत्यागम;
    प्रतिज्ञा (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    यदि (!page)
      प्रत्यागम;
    dirty ();
    page->del_bits (g, bits);
  }

  गुप्त:
  शून्य del_pages (पूर्णाङ्क ds, पूर्णाङ्क de)
  {
    यदि (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<अचिन्हित> compact_workspace;
      यदि (unlikely (!allocate_compact_workspace (compact_workspace))) प्रत्यागम;

      अचिन्हित पूर्णाङ्क write_index = 0;
      प्रति (अचिन्हित पूर्णाङ्क i = 0; i < page_map.length; i++)
      {
	पूर्णाङ्क m = (पूर्णाङ्क) page_map.arrayZ[i].major;
	यदि (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  सार्वजनिक:
  शून्य del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    यदि (unlikely (!successful)) प्रत्यागम;
    यदि (unlikely (a > b || a == INVALID)) प्रत्यागम;
    dirty ();
    अचिन्हित पूर्णाङ्क ma = get_major (a);
    अचिन्हित पूर्णाङ्क mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    पूर्णाङ्क ds = (a == major_start (ma))? (पूर्णाङ्क) ma: (पूर्णाङ्क) (ma + 1);
    पूर्णाङ्क de = (b + 1 == major_start (mb + 1))? (पूर्णाङ्क) mb: ((पूर्णाङ्क) mb - 1);
    यदि (ds > de || (पूर्णाङ्क) ma < ds)
    {
      page_t *page = page_for (a);
      यदि (page)
      {
	यदि (ma == mb)
	  page->del_range (a, b);
	अन्यथा
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    यदि (de < (पूर्णाङ्क) mb && ma != mb)
    {
      page_t *page = page_for (b);
      यदि (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  तार्किक get (hb_codepoint_t g) ध्रुव
  {
    ध्रुव page_t *page = page_for (g);
    यदि (!page)
      प्रत्यागम असत्य;
    प्रत्यागम page->get (g);
  }
  तार्किक may_have (hb_codepoint_t g) ध्रुव { प्रत्यागम get (g); }

  /* Has interface. */
  तार्किक चालक [] (hb_codepoint_t k) ध्रुव { प्रत्यागम get (k); }
  तार्किक has (hb_codepoint_t k) ध्रुव { प्रत्यागम (*एतत्)[k]; }
  /* Predicate. */
  तार्किक चालक () (hb_codepoint_t k) ध्रुव { प्रत्यागम has (k); }

  /* Sink interface. */
  hb_bit_set_t& चालक << (hb_codepoint_t v)
  { add (v); प्रत्यागम *एतत्; }
  hb_bit_set_t& चालक << (ध्रुव hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); प्रत्यागम *एतत्; }

  तार्किक intersects (ध्रुव hb_bit_set_t &other) ध्रुव
  {
    अचिन्हित पूर्णाङ्क na = pages.length;
    अचिन्हित पूर्णाङ्क nb = other.pages.length;

    अचिन्हित पूर्णाङ्क a = 0, b = 0;
    प्रति (; a < na && b < nb; )
    {
      यदि (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	यदि (page_at (a).intersects (other.page_at (b)))
	  प्रत्यागम सत्य;
	a++;
	b++;
      }
      अन्यथा यदि (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      अन्यथा
	b++;
    }
    प्रत्यागम असत्य;
  }
  तार्किक may_intersect (ध्रुव hb_bit_set_t &other) ध्रुव
  { प्रत्यागम intersects (other); }

  तार्किक intersects (hb_codepoint_t first, hb_codepoint_t last) ध्रुव
  {
    hb_codepoint_t c = first - 1;
    प्रत्यागम next (&c) && c <= last;
  }
  शून्य set (ध्रुव hb_bit_set_t &other, तार्किक exact_size = असत्य)
  {
    यदि (unlikely (!successful)) प्रत्यागम;
    अचिन्हित पूर्णाङ्क गिनती = other.pages.length;
    यदि (unlikely (!resize  (गिनती, असत्य, exact_size)))
      प्रत्यागम;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  तार्किक is_equal (ध्रुव hb_bit_set_t &other) ध्रुव
  {
    यदि (has_population () && other.has_population () &&
	population != other.population)
      प्रत्यागम असत्य;

    अचिन्हित पूर्णाङ्क na = pages.length;
    अचिन्हित पूर्णाङ्क nb = other.pages.length;

    अचिन्हित पूर्णाङ्क a = 0, b = 0;
    प्रति (; a < na && b < nb; )
    {
      यदि (page_at (a).is_empty ()) { a++; अनुवर्त; }
      यदि (other.page_at (b).is_empty ()) { b++; अनुवर्त; }
      यदि (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	प्रत्यागम असत्य;
      a++;
      b++;
    }
    प्रति (; a < na; a++)
      यदि (!page_at (a).is_empty ()) { प्रत्यागम असत्य; }
    प्रति (; b < nb; b++)
      यदि (!other.page_at (b).is_empty ()) { प्रत्यागम असत्य; }

    प्रत्यागम सत्य;
  }

  तार्किक is_subset (ध्रुव hb_bit_set_t &larger_set) ध्रुव
  {
    यदि (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      प्रत्यागम असत्य;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    यावत् (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      स्वतः sp = page_at (spi);

      यदि (spm < lpm) {
        यदि (!sp.is_empty ())
          प्रत्यागम असत्य;
        spi++;
        अनुवर्त;
      }

      यदि (lpm < spm) {
        lpi++;
        अनुवर्त;
      }

      स्वतः lp = larger_set.page_at (lpi);
      यदि (!sp.is_subset (lp))
        प्रत्यागम असत्य;

      spi++;
      lpi++;
    }

    यावत् (spi < page_map.length)
      यदि (!page_at (spi++).is_empty ())
        प्रत्यागम असत्य;

    प्रत्यागम सत्य;
  }

  गुप्त:
  तार्किक allocate_compact_workspace (hb_vector_t<अचिन्हित>& workspace)
  {
    यदि (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = असत्य;
      प्रत्यागम असत्य;
    }

    प्रत्यागम सत्य;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  शून्य compact (hb_vector_t<अचिन्हित>& workspace,
                अचिन्हित पूर्णाङ्क length)
  {
    प्रतिज्ञा(workspace.length == pages.length);
    hb_vector_t<अचिन्हित>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    प्रति (अचिन्हित i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  शून्य compact_pages (ध्रुव hb_vector_t<अचिन्हित>& old_index_to_page_map_index)
  {
    अचिन्हित पूर्णाङ्क write_index = 0;
    प्रति (अचिन्हित पूर्णाङ्क i = 0; i < pages.length; i++)
    {
      यदि (old_index_to_page_map_index[i] == 0xFFFFFFFF) अनुवर्त;

      यदि (write_index < i)
	pages[write_index] = pages[i];

      page_map[old_index_to_page_map_index[i]].index = write_index;
      write_index++;
    }
  }
  सार्वजनिक:

  शून्य process_ (hb_bit_page_t::vector_t (*op) (ध्रुव hb_bit_page_t::vector_t &, ध्रुव hb_bit_page_t::vector_t &),
		 तार्किक passthru_बायाँ, तार्किक passthru_दायाँ,
		 ध्रुव hb_bit_set_t &other,
		 hb_vector_t<अचिन्हित> *workspace = nullptr)
  {
    यदि (unlikely (!successful)) प्रत्यागम;

    dirty ();

    अचिन्हित पूर्णाङ्क na = pages.length;
    अचिन्हित पूर्णाङ्क nb = other.pages.length;
    अचिन्हित पूर्णाङ्क next_page = na;

    अचिन्हित पूर्णाङ्क गिनती = 0, new_गिनती = 0;
    अचिन्हित पूर्णाङ्क a = 0, b = 0;
    अचिन्हित पूर्णाङ्क write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<अचिन्हित> local_workspace;
    hb_vector_t<अचिन्हित> &compact_workspace = workspace ? *workspace : local_workspace;
    यदि (!passthru_बायाँ)
    {
      तार्किक allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      यदि (unlikely (!allocated))
      {
	successful = असत्य;
	प्रत्यागम;
      }
    }

    प्रति (; a < na && b < nb; )
    {
      यदि (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	यदि (!passthru_बायाँ)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  यदि (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	गिनती++;
	a++;
	b++;
      }
      अन्यथा यदि (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	यदि (passthru_बायाँ)
	  गिनती++;
	a++;
      }
      अन्यथा
      {
	यदि (passthru_दायाँ)
	  गिनती++;
	b++;
      }
    }
    यदि (passthru_बायाँ)
      गिनती += na - a;
    यदि (passthru_दायाँ)
      गिनती += nb - b;

    यदि (!passthru_बायाँ)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    यदि (unlikely (!resize (गिनती)))
      प्रत्यागम;

    new_गिनती = गिनती;

    /* Process in-place backward. */
    a = na;
    b = nb;
    प्रति (; a && b; )
    {
      यदि (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	गिनती--;
	page_map.arrayZ[गिनती] = page_map.arrayZ[a];
	page_at (गिनती).v = op (page_at (a).v, other.page_at (b).v);
	page_at (गिनती).dirty ();
      }
      अन्यथा यदि (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	यदि (passthru_बायाँ)
	{
	  गिनती--;
	  page_map.arrayZ[गिनती] = page_map.arrayZ[a];
	}
      }
      अन्यथा
      {
	b--;
	यदि (passthru_दायाँ)
	{
	  गिनती--;
	  page_map.arrayZ[गिनती].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[गिनती].index = next_page++;
	  page_at (गिनती) = other.page_at (b);
	}
      }
    }
    यदि (passthru_बायाँ)
      यावत् (a)
      {
	a--;
	गिनती--;
	page_map.arrayZ[गिनती] = page_map.arrayZ[a];
      }
    यदि (passthru_दायाँ)
      यावत् (b)
      {
	b--;
	गिनती--;
	page_map.arrayZ[गिनती].major = other.page_map.arrayZ[b].major;
	page_map.arrayZ[गिनती].index = next_page++;
	page_at (गिनती) = other.page_at (b);
      }
    प्रतिज्ञा (!गिनती);
    resize (new_गिनती);
  }
  ढाँचा <typename Op>
  स्थिर hb_bit_page_t::vector_t
  op_ (ध्रुव hb_bit_page_t::vector_t &a, ध्रुव hb_bit_page_t::vector_t &b)
  { प्रत्यागम Op{} (a, b); }
  ढाँचा <typename Op>
  शून्य process (ध्रुव Op& op,
		ध्रुव hb_bit_set_t &other,
		hb_vector_t<अचिन्हित> *workspace = nullptr)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  शून्य union_ (ध्रुव hb_bit_set_t &other) { process (hb_bitwise_or, other); }
  शून्य intersect (ध्रुव hb_bit_set_t &other,
		  hb_vector_t<अचिन्हित> *workspace = nullptr)
  { process (hb_bitwise_and, other, workspace); }
  शून्य subtract (ध्रुव hb_bit_set_t &other) { process (hb_bitwise_gt, other); }
  शून्य symmetric_difference (ध्रुव hb_bit_set_t &other) { process (hb_bitwise_xor, other); }

  तार्किक next (hb_codepoint_t *codepoint) ध्रुव
  {
    यदि (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      प्रत्यागम *codepoint != INVALID;
    }

    ध्रुव स्वतः* page_map_array = page_map.arrayZ;
    अचिन्हित पूर्णाङ्क major = get_major (*codepoint);
    अचिन्हित पूर्णाङ्क i = last_page_lookup;

    यदि (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      यदि (i >= page_map.length) {
        *codepoint = INVALID;
        प्रत्यागम असत्य;
      }
      last_page_lookup = i;
    }

    ध्रुव स्वतः* pages_array = pages.arrayZ;
    ध्रुव page_map_t &current = page_map_array[i];
    यदि (likely (current.major == major))
    {
      यदि (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        प्रत्यागम सत्य;
      }
      i++;
    }

    प्रति (; i < page_map.length; i++)
    {
      ध्रुव page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      यदि (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	प्रत्यागम सत्य;
      }
    }
    *codepoint = INVALID;
    प्रत्यागम असत्य;
  }
  तार्किक next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ध्रुव
  {
    static_assert (page_t::ELT_BITS == 64, "");

    अचिन्हित पूर्णाङ्क i = 0;
    अचिन्हित पूर्णाङ्क elt = 0;
    यदि (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      यदि (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        गच्छ done;
      base += page_t::ELT_BITS;

      अचिन्हित पूर्णाङ्क major = get_major (base);
      i = last_page_lookup;
      यदि (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	यदि (i >= page_map.length)
	  गच्छ done;
      }
      यदि (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    प्रति (; i < page_map.length; i++, elt = 0)
    {
      ध्रुव page_map_t &map = page_map.arrayZ[i];
      ध्रुव page_t &page = pages.arrayZ[map.index];
      प्रति (; elt < page_t::len (); elt++)
	यदि (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  प्रत्यागम सत्य;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    प्रत्यागम असत्य;
  }
  तार्किक previous (hb_codepoint_t *codepoint) ध्रुव
  {
    यदि (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      प्रत्यागम *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    अचिन्हित पूर्णाङ्क i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    यदि (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      यदि (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	प्रत्यागम सत्य;
      }
    }
    i--;
    प्रति (; (पूर्णाङ्क) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      यदि (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	प्रत्यागम सत्य;
      }
    }
    *codepoint = INVALID;
    प्रत्यागम असत्य;
  }
  तार्किक next_range (hb_codepoint_t *first, hb_codepoint_t *last) ध्रुव
  {
    hb_codepoint_t i;

    i = *last;
    यदि (!next (&i))
    {
      *last = *first = INVALID;
      प्रत्यागम असत्य;
    }

    *last = *first = i;

    ध्रुव स्वतः* page_map_array = page_map.arrayZ;
    ध्रुव स्वतः* pages_array = pages.arrayZ;
    अचिन्हित पूर्णाङ्क major = get_major (*last);
    अचिन्हित पूर्णाङ्क p_index = last_page_lookup;

    यदि (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      प्रतिज्ञा(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    अचिन्हित पूर्णाङ्क rem = page_remainder (*last);
    अचिन्हित पूर्णाङ्क elt = rem / page_t::ELT_BITS;
    अचिन्हित पूर्णाङ्क bit = rem & page_t::ELT_MASK;

    ध्रुव page_t *page = &pages_array[page_map_array[p_index].index];
    अचिन्हित पूर्णाङ्क run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    यदि (run < page_t::ELT_BITS - bit)
      प्रत्यागम सत्य;

    elt++;

    यावत् (सत्य)
    {
      प्रति (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        यदि (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          अनुवर्त;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        प्रत्यागम सत्य;
      }

      p_index++;


      यदि (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        प्रत्यागम सत्य;

      ध्रुव स्वतः& entry = page_map_array[p_index];
      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = 0;
    }
  }

  तार्किक previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ध्रुव
  {
    hb_codepoint_t i;

    i = *first;
    यदि (!previous (&i))
    {
      *last = *first = INVALID;
      प्रत्यागम असत्य;
    }

    *last = *first = i;

    ध्रुव स्वतः* page_map_array = page_map.arrayZ;
    ध्रुव स्वतः* pages_array = pages.arrayZ;
    अचिन्हित पूर्णाङ्क major = get_major (*first);
    अचिन्हित पूर्णाङ्क p_index = last_page_lookup;

    यदि (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      प्रतिज्ञा(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    अचिन्हित पूर्णाङ्क rem = page_remainder (*first);
    पूर्णाङ्क elt = rem / page_t::ELT_BITS;
    अचिन्हित पूर्णाङ्क bit = rem & page_t::ELT_MASK;

    ध्रुव page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    अचिन्हित पूर्णाङ्क run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    यदि (run < bit + 1)
      प्रत्यागम सत्य;

    elt--;

    यावत् (सत्य)
    {
      प्रति (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        यदि (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          अनुवर्त;
        }
        uint64_t inv = ~w;
        अचिन्हित पूर्णाङ्क zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        प्रत्यागम सत्य;
      }


      यदि (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        प्रत्यागम सत्य;

      p_index--;
      ध्रुव स्वतः& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  अचिन्हित पूर्णाङ्क next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  अचिन्हित पूर्णाङ्क    size) ध्रुव
  {
    // By default, start at the first bit of the first page of values.
    अचिन्हित पूर्णाङ्क start_page = 0;
    अचिन्हित पूर्णाङ्क start_page_मूल्य = 0;
    यदि (unlikely (codepoint != INVALID))
    {
      ध्रुव स्वतः* page_map_array = page_map.arrayZ;
      अचिन्हित पूर्णाङ्क major = get_major (codepoint);
      अचिन्हित पूर्णाङ्क i = last_page_lookup;
      यदि (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	यदि (i >= page_map.length)
	  प्रत्यागम 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      यदि (page_map_array[i].major == major)
      {
        start_page_मूल्य = page_remainder (codepoint + 1);
        यदि (unlikely (start_page_मूल्य == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_मूल्य = 0;
        }
      }
    }

    अचिन्हित पूर्णाङ्क initial_size = size;
    प्रति (अचिन्हित पूर्णाङ्क i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      अचिन्हित पूर्णाङ्क n = pages[page_map.arrayZ[i].index].लिख (base, start_page_मूल्य, out, size);
      out += n;
      size -= n;
      start_page_मूल्य = 0;
    }
    प्रत्यागम initial_size - size;
  }

  अचिन्हित पूर्णाङ्क next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   अचिन्हित पूर्णाङ्क    size) ध्रुव
  {
    अचिन्हित पूर्णाङ्क initial_size = size;
    // By default, start at the first bit of the first page of values.
    अचिन्हित पूर्णाङ्क start_page = 0;
    अचिन्हित पूर्णाङ्क start_page_मूल्य = 0;
    यदि (unlikely (codepoint != INVALID))
    {
      ध्रुव स्वतः* page_map_array = page_map.arrayZ;
      अचिन्हित पूर्णाङ्क major = get_major (codepoint);
      अचिन्हित पूर्णाङ्क i = last_page_lookup;
      यदि (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        यदि (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          यावत् (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          प्रत्यागम initial_size - size;
        }
      }
      start_page = i;
      यदि (page_map_array[i].major == major)
      {
        start_page_मूल्य = page_remainder (codepoint + 1);
        यदि (unlikely (start_page_मूल्य == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_मूल्य = 0;
        }
      }
    }

    hb_codepoint_t next_मूल्य = codepoint + 1;
    प्रति (अचिन्हित पूर्णाङ्क i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      अचिन्हित पूर्णाङ्क n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_मूल्य, out, size, &next_मूल्य);
      out += n;
      size -= n;
      start_page_मूल्य = 0;
    }
    यावत् (next_मूल्य < HB_SET_मूल्य_INVALID && size) {
      *out++ = next_मूल्य++;
      size--;
    }
    प्रत्यागम initial_size - size;
  }

  तार्किक has_population () ध्रुव { प्रत्यागम population != अच_पूर्णांक_उच्च; }
  अचिन्हित पूर्णाङ्क get_population () ध्रुव
  {
    यदि (has_population ())
      प्रत्यागम population;

    अचिन्हित पूर्णाङ्क निकालो = 0;
    अचिन्हित पूर्णाङ्क गिनती = pages.length;
    प्रति (अचिन्हित पूर्णाङ्क i = 0; i < गिनती; i++)
      निकालो += pages[i].get_population ();

    population = निकालो;
    प्रत्यागम निकालो;
  }
  तार्किक get_singleton (hb_codepoint_t *codepoint) ध्रुव
  {
    यदि (has_population ())
    {
      यदि (population != 1) प्रत्यागम असत्य;
      *codepoint = get_min ();
      प्रत्यागम सत्य;
    }

    hb_codepoint_t singleton = INVALID;
    प्रति (ध्रुव स्वतः &map : page_map)
    {
      ध्रुव स्वतः &page = pages.arrayZ[map.index];
      प्रति (अचिन्हित i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	यदि (!bits) अनुवर्त;
	यदि (singleton != INVALID || (bits & (bits - 1)))
	  प्रत्यागम असत्य;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    यदि (singleton == INVALID)
    {
      population = 0;
      प्रत्यागम असत्य;
    }
    population = 1;
    *codepoint = singleton;
    प्रत्यागम सत्य;
  }
  hb_codepoint_t get_min () ध्रुव
  {
    अचिन्हित गिनती = pages.length;
    प्रति (अचिन्हित i = 0; i < गिनती; i++)
    {
      ध्रुव स्वतः& map = page_map.arrayZ[i];
      ध्रुव स्वतः& page = pages.arrayZ[map.index];

      यदि (!page.is_empty ())
	प्रत्यागम map.major * page_t::PAGE_BITS + page.get_min ();
    }
    प्रत्यागम INVALID;
  }
  hb_codepoint_t get_max () ध्रुव
  {
    अचिन्हित गिनती = pages.length;
    प्रति (चिन्हित i = गिनती - 1; i >= 0; i--)
    {
      ध्रुव स्वतः& map = page_map.arrayZ[(अचिन्हित) i];
      ध्रुव स्वतः& page = pages.arrayZ[map.index];

      यदि (!page.is_empty ())
	प्रत्यागम map.major * page_t::PAGE_BITS + page.get_max ();
    }
    प्रत्यागम INVALID;
  }

  स्थिर constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  संरचना iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    स्थिर constexpr तार्किक is_sorted_iterator = सत्य;
    स्थिर constexpr तार्किक has_fast_len = सत्य;
    iter_t (ध्रुव hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    तार्किक init = सत्य) : s (&s_), v (INVALID), l(0)
    {
      यदि (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    प्रकारनिर्देश hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ध्रुव { प्रत्यागम v; }
    तार्किक __more__ () ध्रुव { प्रत्यागम v != INVALID; }
    शून्य __next__ () { s->next (&v); यदि (l) l--; }
    शून्य __prev__ () { s->previous (&v); }
    अचिन्हित __len__ () ध्रुव { प्रत्यागम l; }
    iter_t end () ध्रुव { प्रत्यागम iter_t (*s, असत्य); }
    तार्किक चालक != (ध्रुव iter_t& o) ध्रुव
    { प्रत्यागम v != o.v; }

    रक्षित:
    ध्रुव hb_bit_set_t *s;
    hb_codepoint_t v;
    अचिन्हित l;
  };
  iter_t iter () ध्रुव { प्रत्यागम iter_t (*एतत्); }
  चालक iter_t () ध्रुव { प्रत्यागम iter (); }

  रक्षित:

  page_t *page_for (hb_codepoint_t g, तार्किक insert = असत्य)
  {
    अचिन्हित major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    अचिन्हित i = last_page_lookup;
    यदि (likely (i < page_map.length))
    {
      स्वतः &cached_page = page_map.arrayZ[i];
      यदि (cached_page.major == major)
	प्रत्यागम &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    यदि (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      यदि (!insert)
        प्रत्यागम nullptr;

      यदि (unlikely (!resize (pages.length + 1)))
	प्रत्यागम nullptr;

      pages.arrayZ[map.index].init0 ();
      स_हटाओ (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    प्रत्यागम &pages.arrayZ[page_map.arrayZ[i].index];
  }
  ध्रुव page_t *page_for (hb_codepoint_t g) ध्रुव
  {
    अचिन्हित major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    अचिन्हित i = last_page_lookup;
    यदि (likely (i < page_map.length))
    {
      स्वतः &cached_page = page_map.arrayZ[i];
      यदि (cached_page.major == major)
	प्रत्यागम &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    यदि (!page_map.bfind (key, &i))
      प्रत्यागम nullptr;

    last_page_lookup = i;
    प्रत्यागम &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (अचिन्हित पूर्णाङ्क i)
  {
    प्रतिज्ञा (i < page_map.length);
    प्रत्यागम pages.arrayZ[page_map.arrayZ[i].index];
  }
  ध्रुव page_t &page_at (अचिन्हित पूर्णाङ्क i) ध्रुव
  {
    प्रतिज्ञा (i < page_map.length);
    प्रत्यागम pages.arrayZ[page_map.arrayZ[i].index];
  }
  अचिन्हित पूर्णाङ्क get_major (hb_codepoint_t g) ध्रुव { प्रत्यागम g >> page_t::PAGE_BITS_LOG_2; }
  अचिन्हित पूर्णाङ्क page_remainder (hb_codepoint_t g) ध्रुव { प्रत्यागम g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (अचिन्हित पूर्णाङ्क major) ध्रुव { प्रत्यागम major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
