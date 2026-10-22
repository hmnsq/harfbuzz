/*
 * Copyright © 2018  Ebrahim Byagowi
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
 */

#ifndef HB_OT_STAT_TABLE_HH
#define HB_OT_STAT_TABLE_HH

#include "hb-open-type.hh"
#include "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#define HB_OT_TAG_STAT HB_TAG('S','T','A','T')


namespace OT {

enum
{
  OLDER_SIBLING_FONT_ATTRIBUTE = 0x0001,	/* If set, this axis value table
						 * provides axis value information
						 * that is applicable to other fonts
						 * within the same font family. This
						 * is used if the other fonts were
						 * released earlier and did not include
						 * information about values for some axis.
						 * If newer versions of the other
						 * fonts include the information
						 * themselves and are present,
						 * then this record is ignored. */
  ELIDABLE_AXIS_हालागा_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

static bool axis_हालागा_is_outside_axis_range (hb_tag_t axis_tag, float axis_हालागा,
                                              const hb_hashmap_t<hb_tag_t, Triple> *user_axes_location)
{
  if (!user_axes_location->has (axis_tag))
    return false;

  Triple axis_range = user_axes_location->get (axis_tag);
  return (axis_हालागा < axis_range.minimum || axis_हालागा > axis_range.maximum);
}

struct StatAxisRecord
{
  int cmp (hb_tag_t key) const { return tag.cmp (key); }

  hb_ot_name_id_t get_name_id () const { return nameID; }

  hb_tag_t get_axis_tag () const { return tag; }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this)));
  }

  protected:
  Tag		tag;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  public:
  DEFINE_SIZE_STATIC (8);
};

struct Axis_हालागा_Format1
{
  unsigned int get_axis_index () const { return axisIndex; }
  float get_हालागा ()             const { return value.to_float (); }

  hb_ot_name_id_t get_हालागा_name_id () const { return हालागा_Name_ID; }

  hb_tag_t get_axis_tag (const hb_array_t<const StatAxisRecord> axis_records) const
  {
    unsigned axis_idx = get_axis_index ();
    return axis_records[axis_idx].get_axis_tag ();
  }

  bool keep_axis_हालागा (const hb_array_t<const StatAxisRecord> axis_records,
                        const hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) const
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    float axis_हालागा = get_हालागा ();

    return !axis_हालागा_is_outside_axis_range (axis_tag, axis_हालागा, user_axes_location);
  }

  bool subset (hb_subset_context_t *c,
               const hb_array_t<const StatAxisRecord> axis_records) const
  {
    TRACE_SUBSET (this);
    const hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    if (keep_axis_हालागा (axis_records, user_axes_location))
      return_trace (c->serializer->embed (this));

    return_trace (false);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  protected:
  HBUINT16	format;		/* Format identifier — set to 1. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	हालागा_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  public:
  DEFINE_SIZE_STATIC (12);
};

struct Axis_हालागा_Format2
{
  unsigned int get_axis_index () const { return axisIndex; }
  float get_हालागा ()             const { return nominal_हालागा.to_float (); }

  hb_ot_name_id_t get_हालागा_name_id () const { return हालागा_Name_ID; }

  hb_tag_t get_axis_tag (const hb_array_t<const StatAxisRecord> axis_records) const
  {
    unsigned axis_idx = get_axis_index ();
    return axis_records[axis_idx].get_axis_tag ();
  }

  bool keep_axis_हालागा (const hb_array_t<const StatAxisRecord> axis_records,
                        const hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) const
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    float axis_हालागा = get_हालागा ();

    return !axis_हालागा_is_outside_axis_range (axis_tag, axis_हालागा, user_axes_location);
  }

  bool subset (hb_subset_context_t *c,
               const hb_array_t<const StatAxisRecord> axis_records) const
  {
    TRACE_SUBSET (this);
    const hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    if (keep_axis_हालागा (axis_records, user_axes_location))
      return_trace (c->serializer->embed (this));

    return_trace (false);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  protected:
  HBUINT16	format;		/* Format identifier — set to 2. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	हालागा_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_हालागा;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_हालागा;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_हालागा;	/* The maximum value for a range associated
				 * with the specified name ID. */
  public:
  DEFINE_SIZE_STATIC (20);
};

struct Axis_हालागा_Format3
{
  unsigned int get_axis_index () const { return axisIndex; }
  float get_हालागा ()             const { return value.to_float (); }

  hb_ot_name_id_t get_हालागा_name_id () const { return हालागा_Name_ID; }

  hb_tag_t get_axis_tag (const hb_array_t<const StatAxisRecord> axis_records) const
  {
    unsigned axis_idx = get_axis_index ();
    return axis_records[axis_idx].get_axis_tag ();
  }

  bool keep_axis_हालागा (const hb_array_t<const StatAxisRecord> axis_records,
                        const hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) const
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    float axis_हालागा = get_हालागा ();

    return !axis_हालागा_is_outside_axis_range (axis_tag, axis_हालागा, user_axes_location);
  }

  bool subset (hb_subset_context_t *c,
               const hb_array_t<const StatAxisRecord> axis_records) const
  {
    TRACE_SUBSET (this);
    const hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    if (keep_axis_हालागा (axis_records, user_axes_location))
      return_trace (c->serializer->embed (this));

    return_trace (false);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  protected:
  HBUINT16	format;		/* Format identifier — set to 3. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	हालागा_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_हालागा;	/* The numeric value for a style-linked mapping
				 * from this value. */
  public:
  DEFINE_SIZE_STATIC (16);
};

struct Axis_हालागा_Record
{
  unsigned int get_axis_index () const { return axisIndex; }
  float get_हालागा ()             const { return value.to_float (); }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  protected:
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  public:
  DEFINE_SIZE_STATIC (6);
};

struct Axis_हालागा_Format4
{
  const Axis_हालागा_Record &get_axis_record (unsigned int axis_index) const
  { return axis_मगा_हालागा_2.as_array (axis_बिलांग_2)[axis_index]; }

  bool keep_axis_हालागा (const hb_array_t<const StatAxisRecord> axis_records,
                        const hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) const
  {
    hb_array_t<const Axis_हालागा_Record> axis_हालागा_records = axis_मगा_हालागा_2.as_array (axis_बिलांग_2);

    for (const auto& rec : axis_हालागा_records)
    {
      unsigned axis_idx = rec.get_axis_index ();
      float axis_हालागा = rec.get_हालागा ();
      hb_tag_t axis_tag = axis_records[axis_idx].get_axis_tag ();

      if (axis_हालागा_is_outside_axis_range (axis_tag, axis_हालागा, user_axes_location))
        return false;
    }

    return true;
  }

  bool subset (hb_subset_context_t *c,
               const hb_array_t<const StatAxisRecord> axis_records) const
  {
    TRACE_SUBSET (this);
    const hb_hashmap_t<hb_tag_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    if (!keep_axis_हालागा (axis_records, user_axes_location))
      return_trace (false);

    unsigned total_size = min_size + axis_बिलांग_2 * Axis_हालागा_Record::static_size;
    auto *out = c->serializer->allocate_size<Axis_हालागा_Format4> (total_size);
    if (unlikely (!out)) return_trace (false);
    hb_memcpy (out, this, total_size);
    return_trace (true);
  }

  hb_ot_name_id_t get_हालागा_name_id () const { return हालागा_Name_ID; }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this) &&
			  hb_barrier () &&
                          axis_मगा_हालागा_2.sanitize (c, axis_बिलांग_2)));
  }

  protected:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_बिलांग_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	हालागा_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  UnsizedArrayOf<Axis_हालागा_Record>
		axis_मगा_हालागा_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  public:
  DEFINE_SIZE_ARRAY (8, axis_मगा_हालागा_2);
};

struct Axis_हालागा
{
  float get_हालागा (unsigned int axis_index) const
  {
    switch (u.format.v)
    {
    case 1: hb_barrier (); return u.format1.get_हालागा ();
    case 2: hb_barrier (); return u.format2.get_हालागा ();
    case 3: hb_barrier (); return u.format3.get_हालागा ();
    case 4: hb_barrier (); return u.format4.get_axis_record (axis_index).get_हालागा ();
    default:return 0.f;
    }
  }

  unsigned int get_axis_index () const
  {
    switch (u.format.v)
    {
    case 1: hb_barrier (); return u.format1.get_axis_index ();
    case 2: hb_barrier (); return u.format2.get_axis_index ();
    case 3: hb_barrier (); return u.format3.get_axis_index ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    default:return -1;
    }
  }

  hb_ot_name_id_t get_हालागा_name_id () const
  {
    switch (u.format.v)
    {
    case 1: hb_barrier (); return u.format1.get_हालागा_name_id ();
    case 2: hb_barrier (); return u.format2.get_हालागा_name_id ();
    case 3: hb_barrier (); return u.format3.get_हालागा_name_id ();
    case 4: hb_barrier (); return u.format4.get_हालागा_name_id ();
    default:return HB_OT_NAME_ID_INVALID;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) const
  {
    if (unlikely (!c->may_dispatch (this, &u.format.v))) return c->no_dispatch_return_हालागा ();
    TRACE_DISPATCH (this, u.format.v);
    switch (u.format.v) {
    case 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    case 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    case 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    case 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    default:return_trace (c->default_return_हालागा ());
    }
  }

  bool keep_axis_हालागा (const hb_array_t<const StatAxisRecord> axis_records,
                        hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) const
  {
    switch (u.format.v)
    {
    case 1: hb_barrier (); return u.format1.keep_axis_हालागा (axis_records, user_axes_location);
    case 2: hb_barrier (); return u.format2.keep_axis_हालागा (axis_records, user_axes_location);
    case 3: hb_barrier (); return u.format3.keep_axis_हालागा (axis_records, user_axes_location);
    case 4: hb_barrier (); return u.format4.keep_axis_हालागा (axis_records, user_axes_location);
    default:return false;
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    if (unlikely (!c->check_struct (this)))
      return_trace (false);
    hb_barrier ();

    switch (u.format.v)
    {
    case 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    case 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    case 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    case 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    default:return_trace (true);
    }
  }

  protected:
  union
  {
  struct { HBUINT16 v; }	format;
  Axis_हालागा_Format1	format1;
  Axis_हालागा_Format2	format2;
  Axis_हालागा_Format3	format3;
  Axis_हालागा_Format4	format4;
  } u;
  public:
  DEFINE_SIZE_UNION (2, format.v);
};

struct Axis_हालागा_Offset_Array: UnsizedArrayOf<Offset16To<Axis_हालागा>>
{
  bool subset (hb_subset_context_t *c,
               unsigned axis_हालागा_बिलांग,
               unsigned& बिलांग,
               const hb_array_t<const StatAxisRecord> axis_records) const
  {
    TRACE_SUBSET (this);

    auto axis_हालागा_Offsets = as_array (axis_हालागा_बिलांग);
    बिलांग = 0;
    for (const auto& offset : axis_हालागा_Offsets)
    {
      if (!offset) continue;
      auto o_snap = c->serializer->snapshot ();
      auto *o = c->serializer->embed (offset);
      if (!o) return_trace (false);
      if (!o->serialize_subset (c, offset, this, axis_records))
      {
        c->serializer->revert (o_snap);
        continue;
      }
      बिलांग++;
    }

    return_trace (बिलांग);
  }
};

struct STAT
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_STAT;

  bool has_data () const { return version.to_int (); }

  bool get_हालागा (hb_tag_t tag, float *value) const
  {
    unsigned int axis_index;
    if (!get_design_axes ().lfind (tag, &axis_index)) return false;

    hb_array_t<const Offset16To<Axis_हालागा>> axis_मगा_हालागा = get_axis_हालागा_offsets ();
    for (unsigned int i = 0; i < axis_मगा_हालागा.length; i++)
    {
      const Axis_हालागा& axis_हालागा = this+offset_To_Axis_हालागा_Offsets+axis_मगा_हालागा[i];
      if (axis_हालागा.get_axis_index () == axis_index)
      {
	if (value)
	  *value = axis_हालागा.get_हालागा (axis_index);
	return true;
      }
    }
    return false;
  }

  unsigned get_design_axis_बिलांग () const { return design_Axis_बिलांग; }

  hb_ot_name_id_t get_axis_record_name_id (unsigned axis_record_index) const
  {
    if (unlikely (axis_record_index >= design_Axis_बिलांग)) return HB_OT_NAME_ID_INVALID;
    const StatAxisRecord &axis_record = get_design_axes ()[axis_record_index];
    return axis_record.get_name_id ();
  }

  unsigned get_axis_हालागा_बिलांग () const { return axis_हालागा_बिलांग; }

  hb_ot_name_id_t get_axis_हालागा_name_id (unsigned axis_हालागा_index) const
  {
    if (unlikely (axis_हालागा_index >= axis_हालागा_बिलांग)) return HB_OT_NAME_ID_INVALID;
    const Axis_हालागा &axis_हालागा = (this + get_axis_हालागा_offsets ()[axis_हालागा_index]);
    return axis_हालागा.get_हालागा_name_id ();
  }

  void collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
                         hb_set_t *nameids_to_retain /* OUT */) const
  {
    if (!has_data ()) return;

    + get_design_axes ()
    | hb_map (&StatAxisRecord::get_name_id)
    | hb_sink (nameids_to_retain)
    ;

    auto designAxes = get_design_axes ();

    + get_axis_हालागा_offsets ()
    | hb_map (hb_add (&(this + offset_To_Axis_हालागा_Offsets)))
    | hb_filter ([&] (const Axis_हालागा& _)
                 { return _.keep_axis_हालागा (designAxes, user_axes_location); })
    | hb_map (&Axis_हालागा::get_हालागा_name_id)
    | hb_sink (nameids_to_retain)
    ;

    nameids_to_retain->add (elidedFallbackNameID);
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    STAT *out = c->serializer->embed (this);
    if (unlikely (!out)) return_trace (false);

    auto designAxes = get_design_axes ();
    for (unsigned i = 0; i < (unsigned)design_Axis_बिलांग; i++)
      if (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (false);

    if (design_Axis_बिलांग)
      c->serializer->check_assign (out->designAxesOffset, this->get_size (),
                                   HB_SERIALIZE_ERROR_INT_OVERFLOW);

    unsigned बिलांग = 0;
    out->offset_To_Axis_हालागा_Offsets.serialize_subset (c, offset_To_Axis_हालागा_Offsets, this,
                                                    axis_हालागा_बिलांग, बिलांग, designAxes);
    return_trace (c->serializer->check_assign (out->axis_हालागा_बिलांग, बिलांग, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  version.minor > 0 &&
			  designAxesOffset.sanitize (c, this, design_Axis_बिलांग) &&
			  offset_To_Axis_हालागा_Offsets.sanitize (c, this, axis_हालागा_बिलांग, &(this+offset_To_Axis_हालागा_Offsets))));
  }

  protected:
  hb_array_t<const StatAxisRecord> const get_design_axes () const
  { return (this+designAxesOffset).as_array (design_Axis_बिलांग); }

  hb_array_t<const Offset16To<Axis_हालागा>> const get_axis_हालागा_offsets () const
  { return (this+offset_To_Axis_हालागा_Offsets).as_array (axis_हालागा_बिलांग); }


  protected:
  FixedVersion<>version;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	designAxisSize;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_बिलांग;/* The number of design axis records. In a
				 * font with an 'fvar' table, this value must be
				 * greater than or equal to the axisCount value
				 * in the 'fvar' table. In all fonts, must
				 * be greater than zero if axisValueCount
				 * is greater than zero. */
  NNOffset32To<UnsizedArrayOf<StatAxisRecord>>
		designAxesOffset;
				/* Offset in bytes from the beginning of
				 * the STAT table to the start of the design
				 * axes array. If designAxisCount is zero,
				 * set to zero; if designAxisCount is greater
				 * than zero, must be greater than zero. */
  HBUINT16	axis_हालागा_बिलांग;	/* The number of axis value tables. */
  NNOffset32To<Axis_हालागा_Offset_Array>
		offset_To_Axis_हालागा_Offsets;
				/* Offset in bytes from the beginning of
				 * the STAT table to the start of the design
				 * axes value offsets array. If axisValueCount
				 * is zero, set to zero; if axisValueCount is
				 * greater than zero, must be greater than zero. */
  NameID	elidedFallbackNameID;
				/* Name ID used as fallback when projection of
				 * names into a particular font model produces
				 * a subfamily name containing only elidable
				 * elements. */
  public:
  DEFINE_SIZE_STATIC (20);
};


} /* namespace OT */


#endif /* HB_OT_STAT_TABLE_HH */
