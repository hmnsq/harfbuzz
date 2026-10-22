/*
 * Copyright © 2017  Google, Inc.
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

#ifndef HB_AAT_LAYOUT_COMMON_HH
#define HB_AAT_LAYOUT_COMMON_HH

#include "hb-aat-layout.hh"
#include "hb-aat-map.hh"
#include "hb-ot-layout-common.hh"
#include "hb-ot-layout-gdef-table.hh"
#include "hb-open-type.hh"
#include "hb-cache.hh"
#include "hb-bit-set.hh"
#include "hb-bit-page.hh"


namespace OT {
struct GDEF;
};

namespace AAT {

using namespace OT;

struct ankr;

using hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#ifndef HB_OPTIMIZE_SIZE
#define HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#아니면
#define HB_AAT_TRANSITION_INLINE
#endif

struct hb_aat_scratch_t
{
  hb_aat_scratch_t () = default;
  hb_aat_scratch_t (const hb_aat_scratch_t &) = delete;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
  }
  hb_aat_scratch_t & operator = (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
    반환 *this;
  }
  ~hb_aat_scratch_t ()
  {
    auto *s = buffer_glyph_set.get_relaxed ();
    만약 (unlikely (!s))
      반환;
    s->fini ();
    hb_free (s);
  }

  hb_bit_set_t *create_buffer_glyph_set () const
  {
    hb_bit_set_t *s = buffer_glyph_set.get_acquire ();
    만약 (s && buffer_glyph_set.cmpexch (s, nullptr))
    {
      s->clear ();
      반환 s;
    }

    s = (hb_bit_set_t *) hb_calloc (1, sizeof (hb_bit_set_t));
    만약 (unlikely (!s))
      반환 nullptr;
    s->init ();

    반환 s;
  }
  void destroy_buffer_glyph_set (hb_bit_set_t *s) const
  {
    만약 (unlikely (!s))
      반환;
    만약 (buffer_glyph_set.cmpexch (nullptr, s))
      반환;
    s->fini ();
    hb_free (s);
  }

  mutable hb_atomic_t<hb_bit_set_t *> buffer_glyph_set;
};

enum { DELETED_GLYPH = 0xFFFF };

#define HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED HB_BUFFER_SCRATCH_FLAG_SHAPER0

struct hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, bool, HB_DEBUG_APPLY>
{
  const char *get_name () { 반환 "APPLY"; }
  template <typename T, typename ...Ts>
  return_t dispatch (const T &obj, Ts&&... ds)
  { 반환 obj.apply (this, std::forward<Ts> (ds)...); }
  static return_t default_return_value () { 반환 false; }
  bool stop_sublookup_iteration (return_t r) const { 반환 r; }

  const hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  const ankr *ankr_table;
  const OT::GDEF &gdef;
  bool has_glyph_classes;
  const hb_sorted_vector_t<hb_aat_map_t::range_flags_t> *range_flags = nullptr;
  hb_mask_t subtable_flags = 0;
  bool buffer_is_reversed = false;
  // Caches
  bool using_buffer_glyph_set = false;
  hb_bit_set_t *buffer_glyph_set = nullptr;
  const hb_bit_set_t *first_set = nullptr;
  const hb_bit_set_t *second_set = nullptr;
  hb_aat_class_cache_t *machine_class_cache = nullptr;

  /* Unused. For debug tracing only. */
  unsigned 정수 lookup_index;

  HB_INTERNAL hb_aat_apply_context_t (const hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_buffer_t *buffer_,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_INTERNAL ~hb_aat_apply_context_t ();

  HB_INTERNAL void set_ankr_table (const AAT::ankr *ankr_table_);

  void set_lookup_index (unsigned 정수 i) { lookup_index = i; }

  void reverse_buffer ()
  {
    buffer->reverse ();
    buffer_is_reversed = !buffer_is_reversed;
  }

  void setup_buffer_glyph_set ()
  {
    using_buffer_glyph_set = buffer->len >= 4 && buffer_glyph_set;

    만약 (likely (using_buffer_glyph_set))
      buffer->collect_codepoints (*buffer_glyph_set);
  }
  bool buffer_intersects_machine () const
  {
    만약 (likely (using_buffer_glyph_set))
      반환 buffer_glyph_set->intersects (*first_set);

    // Faster for shorter buffers.
    위해 (unsigned i = 0; i < buffer->len; i++)
      만약 (first_set->has (buffer->info[i].codepoint))
	반환 true;
    반환 false;
  }

  template <typename T>
  HB_NODISCARD bool output_glyphs (unsigned 정수 count,
				   const T *glyphs)
  {
    만약 (likely (using_buffer_glyph_set))
      buffer_glyph_set->add_array (glyphs, count);
    위해 (unsigned 정수 i = 0; i < count; i++)
    {
      bool at_end_of_text = buffer->idx == buffer->len;
      만약 (at_end_of_text)
	만약 (unlikely (!buffer->output_glyph (glyphs[i]))) 반환 false;

      hb_glyph_info_t &info = at_end_of_text ? buffer->prev () : buffer->cur ();
      만약 (glyphs[i] == DELETED_GLYPH)
      {
        buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
	_hb_glyph_info_set_aat_deleted (&info);
      }
      아니면
      {
#ifndef HB_NO_OT_LAYOUT
	만약 (has_glyph_classes)
	  _hb_glyph_info_set_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#endif
      }
      만약 (!at_end_of_text)
	만약 (unlikely (!buffer->output_glyph (glyphs[i]))) 반환 false;
    }
    반환 true;
  }

  HB_NODISCARD bool replace_glyph (hb_codepoint_t glyph)
  {
    만약 (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->cur());
    }

    만약 (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    만약 (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->cur(),
				      gdef.get_glyph_props (glyph));
#endif
    반환 buffer->replace_glyph (glyph);
  }

  HB_NODISCARD bool delete_glyph ()
  {
    buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
    _hb_glyph_info_set_aat_deleted (&buffer->cur());
    만약 (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (DELETED_GLYPH);
    반환 buffer->replace_glyph (DELETED_GLYPH);
  }

  void replace_glyph_inplace (unsigned i, hb_codepoint_t glyph)
  {
    buffer->info[i].codepoint = glyph;
    만약 (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->info[i]);
    }
    만약 (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    만약 (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->info[i],
				      gdef.get_glyph_props (glyph));
#endif
  }
};


/*
 * Lookup Table
 */

template <typename T> struct Lookup;

template <typename T>
struct LookupFormat0
{
  friend struct Lookup<T>;

  private:
  const T* get_value (hb_codepoint_t glyph_id, unsigned 정수 num_glyphs) const
  {
    만약 (unlikely (glyph_id >= num_glyphs)) 반환 nullptr;
    반환 &arrayZ[glyph_id];
  }

  template <typename set_t>
  void collect_glyphs (set_t &glyphs, unsigned num_glyphs) const
  {
    glyphs.add_range (0, num_glyphs - 1);
  }
  template <typename set_t, typename filter_t>
  void collect_glyphs_filtered (set_t &glyphs, unsigned num_glyphs, const filter_t &filter) const
  {
    위해 (unsigned i = 0; i < num_glyphs; i++)
      만약 (filter (arrayZ[i]))
	glyphs.add (i);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (arrayZ.sanitize (c, c->get_num_glyphs ()));
  }
  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (arrayZ.sanitize (c, c->get_num_glyphs (), base));
  }

  protected:
  HBUINT16	format;		/* Format identifier--format = 0 */
  UnsizedArrayOf<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  public:
  DEFINE_SIZE_UNBOUNDED (2);
};


template <typename T>
struct LookupSegmentSingle
{
  static constexpr unsigned TerminationWordCount = 2u;

  정수 cmp (hb_codepoint_t g) const
  { 반환 g < first ? -1 : g <= last ? 0 : +1 ; }

  template <typename set_t>
  void collect_glyphs (set_t &glyphs) const
  {
    만약 (first == DELETED_GLYPH) 반환;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  void collect_glyphs_filtered (set_t &glyphs, const filter_t &filter) const
  {
    만약 (first == DELETED_GLYPH) 반환;
    만약 (!filter (value)) 반환;
    glyphs.add_range (first, last);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && value.sanitize (c));
  }
  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && value.sanitize (c, base));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  public:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <typename T>
struct LookupFormat2
{
  friend struct Lookup<T>;

  private:
  const T* get_value (hb_codepoint_t glyph_id) const
  {
    const LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    반환 v ? &v->value : nullptr;
  }

  template <typename set_t>
  void collect_glyphs (set_t &glyphs) const
  {
    unsigned count = segments.get_length ();
    위해 (unsigned 정수 i = 0; i < count; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  void collect_glyphs_filtered (set_t &glyphs, const filter_t &filter) const
  {
    unsigned count = segments.get_length ();
    위해 (unsigned 정수 i = 0; i < count; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (segments.sanitize (c));
  }
  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (segments.sanitize (c, base));
  }

  protected:
  HBUINT16	format;		/* Format identifier--format = 2 */
  VarSizedBinSearchArrayOf<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  public:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
struct LookupSegmentArray
{
  static constexpr unsigned TerminationWordCount = 2u;

  const T* get_value (hb_codepoint_t glyph_id, const void *base) const
  {
    반환 first <= glyph_id && glyph_id <= last ? &(base+valuesZ)[glyph_id - first] : nullptr;
  }

  template <typename set_t>
  void collect_glyphs (set_t &glyphs) const
  {
    만약 (first == DELETED_GLYPH) 반환;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  void collect_glyphs_filtered (set_t &glyphs, const void *base, const filter_t &filter) const
  {
    만약 (first == DELETED_GLYPH) 반환;
    const auto &values = base+valuesZ;
    위해 (hb_codepoint_t i = first; i <= last; i++)
      만약 (filter (values[i - first]))
	glyphs.add (i);
  }

  정수 cmp (hb_codepoint_t g) const
  { 반환 g < first ? -1 : g <= last ? 0 : +1; }

  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  first <= last &&
		  valuesZ.sanitize (c, base, last - first + 1));
  }
  template <typename ...Ts>
  bool sanitize (hb_sanitize_context_t *c, const void *base, Ts&&... ds) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  first <= last &&
		  valuesZ.sanitize (c, base, last - first + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  NNOffset16To<UnsizedArrayOf<T>>
		valuesZ;	/* A 16-bit offset from the start of
				 * the table to the data. */
  public:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
struct LookupFormat4
{
  friend struct Lookup<T>;

  private:
  const T* get_value (hb_codepoint_t glyph_id) const
  {
    const LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    반환 v ? v->get_value (glyph_id, this) : nullptr;
  }

  template <typename set_t>
  void collect_glyphs (set_t &glyphs) const
  {
    unsigned count = segments.get_length ();
    위해 (unsigned i = 0; i < count; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  void collect_glyphs_filtered (set_t &glyphs, const filter_t &filter) const
  {
    unsigned count = segments.get_length ();
    위해 (unsigned i = 0; i < count; i++)
      segments[i].collect_glyphs_filtered (glyphs, this, filter);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (segments.sanitize (c, this));
  }
  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (segments.sanitize (c, this, base));
  }

  protected:
  HBUINT16	format;		/* Format identifier--format = 4 */
  VarSizedBinSearchArrayOf<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  public:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
struct LookupSingle
{
  static constexpr unsigned TerminationWordCount = 1u;

  정수 cmp (hb_codepoint_t g) const { 반환 glyph.cmp (g); }

  template <typename set_t>
  void collect_glyphs (set_t &glyphs) const
  {
    만약 (glyph == DELETED_GLYPH) 반환;
    glyphs.add (glyph);
  }
  template <typename set_t, typename filter_t>
  void collect_glyphs_filtered (set_t &glyphs, const filter_t &filter) const
  {
    만약 (glyph == DELETED_GLYPH) 반환;
    만약 (!filter (value)) 반환;
    glyphs.add (glyph);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && value.sanitize (c));
  }
  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && value.sanitize (c, base));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  public:
  DEFINE_SIZE_STATIC (2 + T::static_size);
};

template <typename T>
struct LookupFormat6
{
  friend struct Lookup<T>;

  private:
  const T* get_value (hb_codepoint_t glyph_id) const
  {
    const LookupSingle<T> *v = entries.bsearch (glyph_id);
    반환 v ? &v->value : nullptr;
  }

  template <typename set_t>
  void collect_glyphs (set_t &glyphs) const
  {
    unsigned count = entries.get_length ();
    위해 (unsigned i = 0; i < count; i++)
      entries[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  void collect_glyphs_filtered (set_t &glyphs, const filter_t &filter) const
  {
    unsigned count = entries.get_length ();
    위해 (unsigned i = 0; i < count; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (entries.sanitize (c));
  }
  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (entries.sanitize (c, base));
  }

  protected:
  HBUINT16	format;		/* Format identifier--format = 6 */
  VarSizedBinSearchArrayOf<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  public:
  DEFINE_SIZE_ARRAY (8, entries);
};

template <typename T>
struct LookupFormat8
{
  friend struct Lookup<T>;

  private:
  const T* get_value (hb_codepoint_t glyph_id) const
  {
    반환 firstGlyph <= glyph_id && glyph_id - firstGlyph < glyphCount ?
	   &valueArrayZ[glyph_id - firstGlyph] : nullptr;
  }

  template <typename set_t>
  void collect_glyphs (set_t &glyphs) const
  {
    만약 (unlikely (!glyphCount)) 반환;
    만약 (firstGlyph == DELETED_GLYPH) 반환;
    glyphs.add_range (firstGlyph, firstGlyph + glyphCount - 1);
  }
  template <typename set_t, typename filter_t>
  void collect_glyphs_filtered (set_t &glyphs, const filter_t &filter) const
  {
    만약 (unlikely (!glyphCount)) 반환;
    만약 (firstGlyph == DELETED_GLYPH) 반환;
    const T *p = valueArrayZ.arrayZ;
    위해 (unsigned i = 0; i < glyphCount; i++)
      만약 (filter (p[i]))
	glyphs.add (firstGlyph + i);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && valueArrayZ.sanitize (c, glyphCount));
  }
  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && valueArrayZ.sanitize (c, glyphCount, base));
  }

  protected:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyphCount;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<T>
		valueArrayZ;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  public:
  DEFINE_SIZE_ARRAY (6, valueArrayZ);
};

template <typename T>
struct LookupFormat10
{
  friend struct Lookup<T>;

  private:
  const typename T::type get_value_or_null (hb_codepoint_t glyph_id) const
  {
    만약 (!(firstGlyph <= glyph_id && glyph_id - firstGlyph < glyphCount))
      반환 Null (T);

    const HBUINT8 *p = &valueArrayZ[(glyph_id - firstGlyph) * valueSize];

    unsigned 정수 v = 0;
    unsigned 정수 count = valueSize;
    위해 (unsigned 정수 i = 0; i < count; i++)
      v = (v << 8) | *p++;

    반환 v;
  }

  template <typename set_t>
  void collect_glyphs (set_t &glyphs) const
  {
    만약 (unlikely (!glyphCount)) 반환;
    만약 (firstGlyph == DELETED_GLYPH) 반환;
    glyphs.add_range (firstGlyph, firstGlyph + glyphCount - 1);
  }

  template <typename set_t, typename filter_t>
  void collect_glyphs_filtered (set_t &glyphs, const filter_t &filter) const
  {
    만약 (unlikely (!glyphCount)) 반환;
    만약 (firstGlyph == DELETED_GLYPH) 반환;
    const HBUINT8 *p = valueArrayZ.arrayZ;
    위해 (unsigned i = 0; i < glyphCount; i++)
    {
      unsigned 정수 v = 0;
      unsigned 정수 count = valueSize;
      위해 (unsigned 정수 j = 0; j < count; j++)
	v = (v << 8) | *p++;
      만약 (filter (v))
	glyphs.add (firstGlyph + i);
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  valueSize <= 4 &&
		  valueArrayZ.sanitize (c, glyphCount * valueSize));
  }

  protected:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	valueSize;	/* Byte size of each value. */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyphCount;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<HBUINT8>
		valueArrayZ;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  public:
  DEFINE_SIZE_ARRAY (8, valueArrayZ);
};

template <typename T>
struct Lookup
{
  const T* get_value (hb_codepoint_t glyph_id, unsigned 정수 num_glyphs) const
  {
    switch (u.format.v) {
    case 0: hb_barrier (); 반환 u.format0.get_value (glyph_id, num_glyphs);
    case 2: hb_barrier (); 반환 u.format2.get_value (glyph_id);
    case 4: hb_barrier (); 반환 u.format4.get_value (glyph_id);
    case 6: hb_barrier (); 반환 u.format6.get_value (glyph_id);
    case 8: hb_barrier (); 반환 u.format8.get_value (glyph_id);
    default:반환 nullptr;
    }
  }

  const typename T::type get_value_or_null (hb_codepoint_t glyph_id, unsigned 정수 num_glyphs) const
  {
    switch (u.format.v) {
      /* Format 10 cannot return a pointer. */
      case 10: hb_barrier (); 반환 u.format10.get_value_or_null (glyph_id);
      default:
      const T *v = get_value (glyph_id, num_glyphs);
      반환 v ? *v : Null (T);
    }
  }

  template <typename set_t>
  void collect_glyphs (set_t &glyphs, unsigned 정수 num_glyphs) const
  {
    switch (u.format.v) {
    case 0: hb_barrier (); u.format0.collect_glyphs (glyphs, num_glyphs); 반환;
    case 2: hb_barrier (); u.format2.collect_glyphs (glyphs); 반환;
    case 4: hb_barrier (); u.format4.collect_glyphs (glyphs); 반환;
    case 6: hb_barrier (); u.format6.collect_glyphs (glyphs); 반환;
    case 8: hb_barrier (); u.format8.collect_glyphs (glyphs); 반환;
    case 10: hb_barrier (); u.format10.collect_glyphs (glyphs); 반환;
    default:반환;
    }
  }
  template <typename set_t, typename filter_t>
  void collect_glyphs_filtered (set_t &glyphs, unsigned num_glyphs, const filter_t &filter) const
  {
    switch (u.format.v) {
    case 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, num_glyphs, filter); 반환;
    case 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); 반환;
    case 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); 반환;
    case 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); 반환;
    case 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); 반환;
    case 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); 반환;
    default:반환;
    }
  }

  typename T::type get_class (hb_codepoint_t glyph_id,
			      unsigned 정수 num_glyphs,
			      unsigned 정수 outOfRange) const
  {
    const T *v = get_value (glyph_id, num_glyphs);
    반환 v ? *v : outOfRange;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    만약 (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    switch (u.format.v) {
    case 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    case 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    case 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    case 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    case 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    case 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    default:return_trace (true);
    }
  }
  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    만약 (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    switch (u.format.v) {
    case 0: hb_barrier (); return_trace (u.format0.sanitize (c, base));
    case 2: hb_barrier (); return_trace (u.format2.sanitize (c, base));
    case 4: hb_barrier (); return_trace (u.format4.sanitize (c, base));
    case 6: hb_barrier (); return_trace (u.format6.sanitize (c, base));
    case 8: hb_barrier (); return_trace (u.format8.sanitize (c, base));
    case 10: return_trace (false); /* We don't support format10 here currently. */
    default:return_trace (true);
    }
  }

  protected:
  union {
  struct { HBUINT16 v; }	format;		/* Format identifier */
  LookupFormat0<T>	format0;
  LookupFormat2<T>	format2;
  LookupFormat4<T>	format4;
  LookupFormat6<T>	format6;
  LookupFormat8<T>	format8;
  LookupFormat10<T>	format10;
  } u;
  public:
  DEFINE_SIZE_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

template <typename T>
struct Entry
{
  // This doesn't seem like it's ever called.
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    /* Note, we don't recurse-sanitize data because we don't access it.
     * That said, in our DEFINE_SIZE_STATIC we access T::static_size,
     * which ensures that data has a simple sanitize(). To be determined
     * if I need to remove that as well.
     *
     * HOWEVER! Because we are a template, our DEFINE_SIZE_STATIC
     * assertion wouldn't be checked, hence the line below. */
    static_assert (T::static_size, "");

    return_trace (c->check_struct (this));
  }

  public:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		data;		/* Optional offsets to per-glyph tables. */
  public:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <>
struct Entry<void>
{
  // This does seem like it's ever called.
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  public:
  HBUINT16	newState;	/* Byte offset from beginning of state table to the new state. */
  HBUINT16	flags;		/* Table specific. */
  public:
  DEFINE_SIZE_STATIC (4);
};

enum Class
{
  CLASS_END_OF_TEXT = 0,
  CLASS_OUT_OF_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_END_OF_LINE = 3,
};

template <typename Types, typename Extra>
struct StateTable
{
  typedef typename Types::HBUINT HBUINT;
  typedef typename Types::HBUSHORT HBUSHORT;
  typedef typename Types::ClassTypeNarrow ClassType;

  enum State
  {
    STATE_START_OF_TEXT = 0,
    STATE_START_OF_LINE = 1,
  };

  template <typename set_t, typename table_t>
  void collect_initial_glyphs (set_t &glyphs, unsigned num_glyphs, const table_t &table) const
  {
    unsigned num_classes = nClasses;

    만약 (unlikely (num_classes > hb_bit_page_t::BITS))
    {
      (this+classTable).collect_glyphs (glyphs, num_glyphs);
      반환;
    }

    // Collect all classes going out from the start state.
    hb_bit_page_t filter;

    위해 (unsigned i = 0; i < num_classes; i++)
    {
      const auto &entry = get_entry (STATE_START_OF_TEXT, i);
      만약 (new_state (entry.newState) == STATE_START_OF_TEXT &&
	  !table.is_action_initiable (entry) && !table.is_actionable (entry))
	continue;

      filter.add (i);
    }

    // And glyphs in those classes.

    만약 (filter (CLASS_DELETED_GLYPH))
      glyphs.add (DELETED_GLYPH);

    (this+classTable).collect_glyphs_filtered (glyphs, num_glyphs, filter);
  }

  정수 new_state (unsigned 정수 newState) const
  { 반환 Types::extended ? newState : ((정수) newState - (정수) stateArrayTable) / (정수) nClasses; }

  unsigned 정수 get_class (hb_codepoint_t glyph_id,
			  unsigned 정수 num_glyphs,
			  hb_aat_class_cache_t *cache = nullptr) const
  {
    unsigned klass;
    만약 (cache && cache->get (glyph_id, &klass)) 반환 klass;
    만약 (unlikely (glyph_id == DELETED_GLYPH)) 반환 CLASS_DELETED_GLYPH;
    klass = (this+classTable).get_class (glyph_id, num_glyphs, CLASS_OUT_OF_BOUNDS);
    만약 (cache) cache->set (glyph_id, klass);
    반환 klass;
  }

  const Entry<Extra> *get_entries () const
  { 반환 (this+entryTable).arrayZ; }

  const Entry<Extra> &get_entry (정수 state, unsigned 정수 klass) const
  {
    unsigned n_classes = nClasses;
    만약 (unlikely (klass >= n_classes))
      klass = CLASS_OUT_OF_BOUNDS;

    const HBUSHORT *states = (this+stateArrayTable).arrayZ;
    const Entry<Extra> *entries = (this+entryTable).arrayZ;

    unsigned 정수 entry = states[state * n_classes + klass];
    DEBUG_MSG (APPLY, nullptr, "e%u", entry);

    반환 entries[entry];
  }

  bool sanitize (hb_sanitize_context_t *c,
		 unsigned 정수 *num_entries_out = nullptr) const
  {
    TRACE_SANITIZE (this);
    만약 (unlikely (!(c->check_struct (this) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    classTable.sanitize (c, this)))) return_trace (false);

    const HBUSHORT *states = (this+stateArrayTable).arrayZ;
    const Entry<Extra> *entries = (this+entryTable).arrayZ;

    unsigned 정수 num_classes = nClasses;
    만약 (unlikely (hb_unsigned_mul_overflows (num_classes, states[0].static_size)))
      return_trace (false);
    unsigned 정수 row_stride = num_classes * states[0].static_size;

    /* Apple 'kern' table has this peculiarity:
     *
     * "Because the stateTableOffset in the state table header is (strictly
     * speaking) redundant, some 'kern' tables use it to record an initial
     * state where that should not be StartOfText. To determine if this is
     * done, calculate what the stateTableOffset should be. If it's different
     * from the actual stateTableOffset, use it as the initial state."
     *
     * We implement this by calling the initial state zero, but allow *negative*
     * states if the start state indeed was not the first state.  Since the code
     * is shared, this will also apply to 'mort' table.  The 'kerx' / 'morx'
     * tables are not affected since those address states by index, not offset.
     */

    정수 min_state = 0;
    정수 max_state = 0;
    unsigned 정수 num_entries = 0;

    정수 state_pos = 0;
    정수 state_neg = 0;
    unsigned 정수 entry = 0;
    동안 (min_state < state_neg || state_pos <= max_state)
    {
      만약 (min_state < state_neg)
      {
	/* Negative states. */
	만약 (unlikely (hb_unsigned_mul_overflows (min_state, num_classes)))
	  return_trace (false);
	만약 (unlikely (!c->check_range (&states[min_state * num_classes],
				       -min_state,
				       row_stride)))
	  return_trace (false);
	만약 ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (false);
	{ /* Sweep new states. */
	  const HBUSHORT *stop = &states[min_state * num_classes];
	  만약 (unlikely (stop > states))
	    return_trace (false);
	  위해 (const HBUSHORT *p = states; stop < p; p--)
	    num_entries = hb_max (num_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      만약 (state_pos <= max_state)
      {
	/* Positive states. */
	만약 (unlikely (!c->check_range (states,
				       max_state + 1,
				       row_stride)))
	  return_trace (false);
	만약 ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (false);
	{ /* Sweep new states. */
	  만약 (unlikely (hb_unsigned_mul_overflows ((max_state + 1), num_classes)))
	    return_trace (false);
	  const HBUSHORT *stop = &states[(max_state + 1) * num_classes];
	  만약 (unlikely (stop < states))
	    return_trace (false);
	  위해 (const HBUSHORT *p = &states[state_pos * num_classes]; p < stop; p++)
	    num_entries = hb_max (num_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      만약 (unlikely (!c->check_array (entries, num_entries)))
	return_trace (false);
      만약 ((c->max_ops -= num_entries - entry) <= 0)
	return_trace (false);
      { /* Sweep new entries. */
	const Entry<Extra> *stop = &entries[num_entries];
	위해 (const Entry<Extra> *p = &entries[entry]; p < stop; p++)
	{
	  정수 newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	entry = num_entries;
      }
    }

    만약 (num_entries_out)
      *num_entries_out = num_entries;

    return_trace (true);
  }

  protected:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffsetTo<ClassType, HBUINT>
		classTable;	/* Offset to the class table. */
  NNOffsetTo<UnsizedArrayOf<HBUSHORT>, HBUINT>
		stateArrayTable;/* Offset to the state array. */
  NNOffsetTo<UnsizedArrayOf<Entry<Extra>>, HBUINT>
		entryTable;	/* Offset to the entry array. */

  public:
  DEFINE_SIZE_STATIC (4 * sizeof (HBUINT));
};

template <typename HBUCHAR>
struct ClassTable
{
  unsigned 정수 get_class (hb_codepoint_t glyph_id, unsigned 정수 outOfRange) const
  {
    unsigned 정수 i = glyph_id - firstGlyph;
    반환 i >= classArray.len ? outOfRange : classArray.arrayZ[i];
  }
  unsigned 정수 get_class (hb_codepoint_t glyph_id,
			  unsigned 정수 num_glyphs HB_UNUSED,
			  unsigned 정수 outOfRange) const
  {
    반환 get_class (glyph_id, outOfRange);
  }

  template <typename set_t>
  void collect_glyphs (set_t &glyphs, unsigned num_glyphs) const
  {
    위해 (unsigned i = 0; i < classArray.len; i++)
      만약 (classArray.arrayZ[i] != CLASS_OUT_OF_BOUNDS)
	glyphs.add (firstGlyph + i);
  }
  template <typename set_t, typename filter_t>
  void collect_glyphs_filtered (set_t &glyphs, unsigned num_glyphs, const filter_t &filter) const
  {
    위해 (unsigned i = 0; i < classArray.len; i++)
      만약 (filter (classArray.arrayZ[i]))
	glyphs.add (firstGlyph + i);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && classArray.sanitize (c));
  }
  protected:
  HBGlyphID16		firstGlyph;	/* First glyph index included in the trimmed array. */
  Array16Of<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  public:
  DEFINE_SIZE_ARRAY (4, classArray);
};

struct SubtableGlyphCoverage
{
  bool sanitize (hb_sanitize_context_t *c, unsigned subtable_count) const
  {
    TRACE_SANITIZE (this);

    만약 (unlikely (!c->check_array (&subtableOffsets, subtable_count)))
      return_trace (false);

    unsigned bytes = (c->get_num_glyphs () + CHAR_BIT - 1) / CHAR_BIT;
    위해 (unsigned i = 0; i < subtable_count; i++)
    {
      uint32_t offset = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      만약 (offset == 0 || offset == 0xFFFFFFFF)
        continue;
      만약 (unlikely (!subtableOffsets[i].sanitize (c, this, bytes)))
        return_trace (false);
    }

    return_trace (true);
  }
  protected:
  UnsizedArrayOf<NNOffset32To<UnsizedArrayOf<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  public:
  DEFINE_SIZE_ARRAY (0, subtableOffsets);
};

struct ObsoleteTypes
{
  static constexpr bool extended = false;
  typedef HBUINT16 HBUINT;
  typedef HBUINT8 HBUSHORT;
  typedef ClassTable<HBUINT8> ClassTypeNarrow;
  typedef ClassTable<HBUINT16> ClassTypeWide;

  template <typename T>
  static 정수 offsetToIndex (int64_t offset,
			    const void *base,
			    const T *array)
  {
    int64_t array_offset = (const char *) array - (const char *) base;
    정수 bad_index = INT_MAX / T::static_size;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    만약 (unlikely (offset < array_offset))
      반환 bad_index;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t index = (offset - array_offset) / T::static_size;
    만약 (unlikely (index > bad_index))
      반환 bad_index;
    반환 index;
  }
  template <typename T>
  static 정수 byteOffsetToIndex (int64_t offset,
				const void *base,
				const T *array)
  {
    반환 offsetToIndex (offset, base, array);
  }
  template <typename T>
  static 정수 wordOffsetToIndex (int64_t offset,
				const void *base,
				const T *array)
  {
    반환 offsetToIndex (2 * offset, base, array);
  }
};
struct ExtendedTypes
{
  static constexpr bool extended = true;
  typedef HBUINT32 HBUINT;
  typedef HBUINT16 HBUSHORT;
  typedef Lookup<HBUINT16> ClassTypeNarrow;
  typedef Lookup<HBUINT16> ClassTypeWide;

  template <typename T>
  static unsigned 정수 offsetToIndex (unsigned 정수 offset,
				     const void *base HB_UNUSED,
				     const T *array HB_UNUSED)
  {
    반환 offset;
  }
  template <typename T>
  static unsigned 정수 byteOffsetToIndex (unsigned 정수 offset,
					 const void *base HB_UNUSED,
					 const T *array HB_UNUSED)
  {
    반환 offset / 2;
  }
  template <typename T>
  static unsigned 정수 wordOffsetToIndex (unsigned 정수 offset,
					 const void *base HB_UNUSED,
					 const T *array HB_UNUSED)
  {
    반환 offset;
  }
};

template <typename Types, typename EntryData, typename Flags>
struct StateTableDriver
{
  using StateTableT = StateTable<Types, EntryData>;
  using EntryT = Entry<EntryData>;

  StateTableDriver (const StateTableT &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      num_glyphs (face_->get_num_glyphs ()) {}

  template <typename context_t>
  void drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_buffer_t *buffer = ac->buffer;

    만약 (!c->in_place)
      buffer->clear_output ();

    정수 state = StateTableT::STATE_START_OF_TEXT;
    // If there's only one range, we already checked the flag.
    auto *last_range = ac->range_flags && (ac->range_flags->length > 1) ? &(*ac->range_flags)[0] : nullptr;
    const bool start_state_safe_to_break_eot =
      !c->table->is_actionable (machine.get_entry (StateTableT::STATE_START_OF_TEXT, CLASS_END_OF_TEXT));
    위해 (buffer->idx = 0; buffer->successful;)
    {
      unsigned 정수 klass = likely (buffer->idx < buffer->len) ?
			   machine.get_class (buffer->cur().codepoint, num_glyphs, ac->machine_class_cache) :
			   (unsigned) CLASS_END_OF_TEXT;
    resume:
      DEBUG_MSG (APPLY, nullptr, "c%u at %u", klass, buffer->idx);
      const EntryT &entry = machine.get_entry (state, klass);
      const 정수 next_state = machine.new_state (entry.newState);

      bool is_not_epsilon_transition = !(entry.flags & Flags::DontAdvance);
      bool is_not_actionable = !c->table->is_actionable (entry);

      만약 (unlikely (last_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	auto *range = last_range;
	만약 (buffer->idx < buffer->len)
	{
	  unsigned cluster = buffer->cur().cluster;
	  동안 (cluster < range->cluster_first)
	    range--;
	  동안 (cluster > range->cluster_last)
	    range++;


	  last_range = range;
	}
	만약 (!(range->flags & ac->subtable_flags))
	{
	  만약 (buffer->idx == buffer->len)
	    break;

	  state = StateTableT::STATE_START_OF_TEXT;
	  (void) buffer->next_glyph ();
	  continue;
	}
      }
      아니면
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	bool is_null_transition = state == StateTableT::STATE_START_OF_TEXT &&
				  next_state == StateTableT::STATE_START_OF_TEXT &&
				  start_state_safe_to_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	만약 (is_null_transition)
	{
	  unsigned old_klass = klass;
	  do
	  {
	    c->transition (buffer, this, entry);

	    만약 (buffer->idx == buffer->len || !buffer->successful)
	      break;

	    (void) buffer->next_glyph ();

	    klass = likely (buffer->idx < buffer->len) ?
		     machine.get_class (buffer->cur().codepoint, num_glyphs, ac->machine_class_cache) :
		     (unsigned) CLASS_END_OF_TEXT;
	  } 동안 (klass == old_klass);

	  만약 (buffer->idx == buffer->len || !buffer->successful)
	    break;

	  goto resume;
	}
      }

      /* Conditions under which it's guaranteed safe-to-break before current glyph:
       *
       * 1. There was no action in this transition; and
       *
       * 2. If we break before current glyph, the results will be the same. That
       *    is guaranteed if:
       *
       *    2a. We were already in start-of-text state; or
       *
       *    2b. We are epsilon-transitioning to start-of-text state; or
       *
       *    2c. Starting from start-of-text state seeing current glyph:
       *
       *        2c'. There won't be any actions; and
       *
       *        2c". We would end up in the same state that we were going to end up
       *             in now, including whether epsilon-transitioning.
       *
       *    and
       *
       * 3. If we break before current glyph, there won't be any end-of-text action
       *    after previous glyph.
       *
       * This triples the transitions we need to look up, but is worth returning
       * granular unsafe-to-break results. See eg.:
       *
       *   https://github.com/harfbuzz/harfbuzz/issues/2860
       */
      const EntryT *wouldbe_entry;
      bool is_safe_to_break =
      (
          /* 1. */
          !c->table->is_actionable (entry) &&

          /* 2. */
          // This one is meh, I know...
	  (
                 state == StateTableT::STATE_START_OF_TEXT
              || ((entry.flags & Flags::DontAdvance) && next_state == StateTableT::STATE_START_OF_TEXT)
              || (
		    /* 2c. */
		    wouldbe_entry = &machine.get_entry(StateTableT::STATE_START_OF_TEXT, klass)
		    ,
		    /* 2c'. */
		    !c->table->is_actionable (*wouldbe_entry) &&
		    /* 2c". */
		    (
		      next_state == machine.new_state(wouldbe_entry->newState) &&
		      (entry.flags & Flags::DontAdvance) == (wouldbe_entry->flags & Flags::DontAdvance)
		    )
		 )
	  ) &&

          /* 3. */
          !c->table->is_actionable (machine.get_entry (state, CLASS_END_OF_TEXT))
      );

      만약 (!is_safe_to_break && buffer->backtrack_len () && buffer->idx < buffer->len)
	buffer->unsafe_to_break_from_outbuffer (buffer->backtrack_len () - 1, buffer->idx + 1);

      c->transition (buffer, this, entry);

      state = next_state;
      DEBUG_MSG (APPLY, nullptr, "s%d", state);

      만약 (buffer->idx == buffer->len)
	break;

      만약 (is_not_epsilon_transition || buffer->max_ops-- <= 0)
	(void) buffer->next_glyph ();
    }

    만약 (!c->in_place)
      buffer->sync ();
  }

  public:
  const StateTableT &machine;
  unsigned 정수 num_glyphs;
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_COMMON_HH */
