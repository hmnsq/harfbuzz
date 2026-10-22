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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_STAT_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_STAT_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_STAT HB_ਟੈਗ('S','T','A','T')


ਨਾਮਾਕਾਸ਼ OT {

ਗਿਣਤੀ
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
  ELIDABLE_AXIS_ਮੁੱਲ_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

ਸਥਿਰ ਤਾਰਕਿਕ axis_ਮੁੱਲ_is_outside_axis_range (hb_ਟੈਗ_t axis_ਟੈਗ_2, ਦਸ਼ਮਲਵ axis_ਮੁੱਲ,
                                              ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple> *user_axes_location)
{
  ਜੇ (!user_axes_location->has (axis_ਟੈਗ_2))
    ਵਾਪਸ ਝੂਠ;

  Triple axis_range = user_axes_location->get (axis_ਟੈਗ_2);
  ਵਾਪਸ (axis_ਮੁੱਲ < axis_range.minimum || axis_ਮੁੱਲ > axis_range.ਵੱਧ_ਤੋਂ_ਵੱਧ);
}

ਬਣਤਰ Stat_Axis_ਰਿਕਾਰਡ
{
  ਪੂਰਨ cmp (hb_ਟੈਗ_t key) ਅਚਲ { ਵਾਪਸ ਟੈਗ_2.cmp (key); }

  hb_ot_name_id_t get_name_id () ਅਚਲ { ਵਾਪਸ nameID; }

  hb_ਟੈਗ_t get_axis_ਟੈਗ () ਅਚਲ { ਵਾਪਸ ਟੈਗ_2; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ)));
  }

  ਸੁਰੱਖਿਅਤ:
  ਟੈਗ		ਟੈਗ_2;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ Axis_ਮੁੱਲ_Format1
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_axis_ਸੂਚਕਾਂਕ () ਅਚਲ { ਵਾਪਸ axis_ਸੂਚਕਾਂਕ_2; }
  ਦਸ਼ਮਲਵ get_ਮੁੱਲ ()             ਅਚਲ { ਵਾਪਸ value.ਮੰਜ਼ਿਲ_float (); }

  hb_ot_name_id_t get_ਮੁੱਲ_name_id () ਅਚਲ { ਵਾਪਸ ਮੁੱਲ_Name_ID; }

  hb_ਟੈਗ_t get_axis_ਟੈਗ (ਅਚਲ hb_array_t<ਅਚਲ Stat_Axis_ਰਿਕਾਰਡ> axis_ਰਿਕਾਰਡਾਂ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ axis_idx = get_axis_ਸੂਚਕਾਂਕ ();
    ਵਾਪਸ axis_ਰਿਕਾਰਡਾਂ[axis_idx].get_axis_ਟੈਗ ();
  }

  ਤਾਰਕਿਕ keep_axis_ਮੁੱਲ (ਅਚਲ hb_array_t<ਅਚਲ Stat_Axis_ਰਿਕਾਰਡ> axis_ਰਿਕਾਰਡਾਂ,
                        ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple> *user_axes_location) ਅਚਲ
  {
    hb_ਟੈਗ_t axis_ਟੈਗ_2 = get_axis_ਟੈਗ (axis_ਰਿਕਾਰਡਾਂ);
    ਦਸ਼ਮਲਵ axis_ਮੁੱਲ = get_ਮੁੱਲ ();

    ਵਾਪਸ !axis_ਮੁੱਲ_is_outside_axis_range (axis_ਟੈਗ_2, axis_ਮੁੱਲ, user_axes_location);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ hb_array_t<ਅਚਲ Stat_Axis_ਰਿਕਾਰਡ> axis_ਰਿਕਾਰਡਾਂ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    ਜੇ (keep_axis_ਮੁੱਲ (axis_ਰਿਕਾਰਡਾਂ, user_axes_location))
      return_trace (c->serializer->embed (ਇਹ));

    return_trace (ਝੂਠ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier — set to 1. */
  HBUINT16	axis_ਸੂਚਕਾਂਕ_2;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	ਮੁੱਲ_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (12);
};

ਬਣਤਰ Axis_ਮੁੱਲ_Format2
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_axis_ਸੂਚਕਾਂਕ () ਅਚਲ { ਵਾਪਸ axis_ਸੂਚਕਾਂਕ_2; }
  ਦਸ਼ਮਲਵ get_ਮੁੱਲ ()             ਅਚਲ { ਵਾਪਸ nominal_ਮੁੱਲ.ਮੰਜ਼ਿਲ_float (); }

  hb_ot_name_id_t get_ਮੁੱਲ_name_id () ਅਚਲ { ਵਾਪਸ ਮੁੱਲ_Name_ID; }

  hb_ਟੈਗ_t get_axis_ਟੈਗ (ਅਚਲ hb_array_t<ਅਚਲ Stat_Axis_ਰਿਕਾਰਡ> axis_ਰਿਕਾਰਡਾਂ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ axis_idx = get_axis_ਸੂਚਕਾਂਕ ();
    ਵਾਪਸ axis_ਰਿਕਾਰਡਾਂ[axis_idx].get_axis_ਟੈਗ ();
  }

  ਤਾਰਕਿਕ keep_axis_ਮੁੱਲ (ਅਚਲ hb_array_t<ਅਚਲ Stat_Axis_ਰਿਕਾਰਡ> axis_ਰਿਕਾਰਡਾਂ,
                        ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple> *user_axes_location) ਅਚਲ
  {
    hb_ਟੈਗ_t axis_ਟੈਗ_2 = get_axis_ਟੈਗ (axis_ਰਿਕਾਰਡਾਂ);
    ਦਸ਼ਮਲਵ axis_ਮੁੱਲ = get_ਮੁੱਲ ();

    ਵਾਪਸ !axis_ਮੁੱਲ_is_outside_axis_range (axis_ਟੈਗ_2, axis_ਮੁੱਲ, user_axes_location);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ hb_array_t<ਅਚਲ Stat_Axis_ਰਿਕਾਰਡ> axis_ਰਿਕਾਰਡਾਂ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    ਜੇ (keep_axis_ਮੁੱਲ (axis_ਰਿਕਾਰਡਾਂ, user_axes_location))
      return_trace (c->serializer->embed (ਇਹ));

    return_trace (ਝੂਠ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier — set to 2. */
  HBUINT16	axis_ਸੂਚਕਾਂਕ_2;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	ਮੁੱਲ_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_ਮੁੱਲ;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_ਮੁੱਲ;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_ਮੁੱਲ;	/* The maximum value for a range associated
				 * with the specified name ID. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (20);
};

ਬਣਤਰ Axis_ਮੁੱਲ_Format3
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_axis_ਸੂਚਕਾਂਕ () ਅਚਲ { ਵਾਪਸ axis_ਸੂਚਕਾਂਕ_2; }
  ਦਸ਼ਮਲਵ get_ਮੁੱਲ ()             ਅਚਲ { ਵਾਪਸ value.ਮੰਜ਼ਿਲ_float (); }

  hb_ot_name_id_t get_ਮੁੱਲ_name_id () ਅਚਲ { ਵਾਪਸ ਮੁੱਲ_Name_ID; }

  hb_ਟੈਗ_t get_axis_ਟੈਗ (ਅਚਲ hb_array_t<ਅਚਲ Stat_Axis_ਰਿਕਾਰਡ> axis_ਰਿਕਾਰਡਾਂ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ axis_idx = get_axis_ਸੂਚਕਾਂਕ ();
    ਵਾਪਸ axis_ਰਿਕਾਰਡਾਂ[axis_idx].get_axis_ਟੈਗ ();
  }

  ਤਾਰਕਿਕ keep_axis_ਮੁੱਲ (ਅਚਲ hb_array_t<ਅਚਲ Stat_Axis_ਰਿਕਾਰਡ> axis_ਰਿਕਾਰਡਾਂ,
                        ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple> *user_axes_location) ਅਚਲ
  {
    hb_ਟੈਗ_t axis_ਟੈਗ_2 = get_axis_ਟੈਗ (axis_ਰਿਕਾਰਡਾਂ);
    ਦਸ਼ਮਲਵ axis_ਮੁੱਲ = get_ਮੁੱਲ ();

    ਵਾਪਸ !axis_ਮੁੱਲ_is_outside_axis_range (axis_ਟੈਗ_2, axis_ਮੁੱਲ, user_axes_location);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ hb_array_t<ਅਚਲ Stat_Axis_ਰਿਕਾਰਡ> axis_ਰਿਕਾਰਡਾਂ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    ਜੇ (keep_axis_ਮੁੱਲ (axis_ਰਿਕਾਰਡਾਂ, user_axes_location))
      return_trace (c->serializer->embed (ਇਹ));

    return_trace (ਝੂਠ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier — set to 3. */
  HBUINT16	axis_ਸੂਚਕਾਂਕ_2;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	ਮੁੱਲ_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_ਮੁੱਲ;	/* The numeric value for a style-linked mapping
				 * from this value. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (16);
};

ਬਣਤਰ Axis_ਮੁੱਲ_ਰਿਕਾਰਡ
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_axis_ਸੂਚਕਾਂਕ () ਅਚਲ { ਵਾਪਸ axis_ਸੂਚਕਾਂਕ_2; }
  ਦਸ਼ਮਲਵ get_ਮੁੱਲ ()             ਅਚਲ { ਵਾਪਸ value.ਮੰਜ਼ਿਲ_float (); }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	axis_ਸੂਚਕਾਂਕ_2;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};

ਬਣਤਰ Axis_ਮੁੱਲ_Format4
{
  ਅਚਲ Axis_ਮੁੱਲ_ਰਿਕਾਰਡ &get_axis_ਰਿਕਾਰਡ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ axis_ਸੂਚਕਾਂਕ) ਅਚਲ
  { ਵਾਪਸ axis_ਮੁੱਲਾਂ_2.as_array (axis_ਗਿਣਤੀ_2)[axis_ਸੂਚਕਾਂਕ]; }

  ਤਾਰਕਿਕ keep_axis_ਮੁੱਲ (ਅਚਲ hb_array_t<ਅਚਲ Stat_Axis_ਰਿਕਾਰਡ> axis_ਰਿਕਾਰਡਾਂ,
                        ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple> *user_axes_location) ਅਚਲ
  {
    hb_array_t<ਅਚਲ Axis_ਮੁੱਲ_ਰਿਕਾਰਡ> axis_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ = axis_ਮੁੱਲਾਂ_2.as_array (axis_ਗਿਣਤੀ_2);

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& rec : axis_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ axis_idx = rec.get_axis_ਸੂਚਕਾਂਕ ();
      ਦਸ਼ਮਲਵ axis_ਮੁੱਲ = rec.get_ਮੁੱਲ ();
      hb_ਟੈਗ_t axis_ਟੈਗ_2 = axis_ਰਿਕਾਰਡਾਂ[axis_idx].get_axis_ਟੈਗ ();

      ਜੇ (axis_ਮੁੱਲ_is_outside_axis_range (axis_ਟੈਗ_2, axis_ਮੁੱਲ, user_axes_location))
        ਵਾਪਸ ਝੂਠ;
    }

    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ hb_array_t<ਅਚਲ Stat_Axis_ਰਿਕਾਰਡ> axis_ਰਿਕਾਰਡਾਂ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    ਜੇ (!keep_axis_ਮੁੱਲ (axis_ਰਿਕਾਰਡਾਂ, user_axes_location))
      return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ਮਾਪ = min_ਮਾਪ + axis_ਗਿਣਤੀ_2 * Axis_ਮੁੱਲ_ਰਿਕਾਰਡ::static_ਮਾਪ;
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->allocate_ਮਾਪ<Axis_ਮੁੱਲ_Format4> (ਕੁੱਲ_ਮਾਪ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);
    hb_memcpy (ਬਾਹਰਲਾ, ਇਹ, ਕੁੱਲ_ਮਾਪ);
    return_trace (ਸੱਚ);
  }

  hb_ot_name_id_t get_ਮੁੱਲ_name_id () ਅਚਲ { ਵਾਪਸ ਮੁੱਲ_Name_ID; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  hb_barrier () &&
                          axis_ਮੁੱਲਾਂ_2.sanitize (c, axis_ਗਿਣਤੀ_2)));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_ਗਿਣਤੀ_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	ਮੁੱਲ_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  Unsized_Array_ਦਾ<Axis_ਮੁੱਲ_ਰਿਕਾਰਡ>
		axis_ਮੁੱਲਾਂ_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (8, axis_ਮੁੱਲਾਂ_2);
};

ਬਣਤਰ Axis_ਮੁੱਲ
{
  ਦਸ਼ਮਲਵ get_ਮੁੱਲ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ axis_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਚੋਣ (u.format.v)
    {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_ਮੁੱਲ ();
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.get_ਮੁੱਲ ();
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.get_ਮੁੱਲ ();
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.get_axis_ਰਿਕਾਰਡ (axis_ਸੂਚਕਾਂਕ).get_ਮੁੱਲ ();
    ਮੂਲ:ਵਾਪਸ 0.f;
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_axis_ਸੂਚਕਾਂਕ () ਅਚਲ
  {
    ਚੋਣ (u.format.v)
    {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_axis_ਸੂਚਕਾਂਕ ();
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.get_axis_ਸੂਚਕਾਂਕ ();
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.get_axis_ਸੂਚਕਾਂਕ ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    ਮੂਲ:ਵਾਪਸ -1;
    }
  }

  hb_ot_name_id_t get_ਮੁੱਲ_name_id () ਅਚਲ
  {
    ਚੋਣ (u.format.v)
    {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_ਮੁੱਲ_name_id ();
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.get_ਮੁੱਲ_name_id ();
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.get_ਮੁੱਲ_name_id ();
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.get_ਮੁੱਲ_name_id ();
    ਮੂਲ:ਵਾਪਸ HB_OT_NAME_ID_INVALID;
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਜੇ (unlikely (!c->may_dispatch (ਇਹ, &u.format.v))) ਵਾਪਸ c->no_dispatch_return_ਮੁੱਲ ();
    TRACE_DISPATCH (ਇਹ, u.format.v);
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ਹਾਲਤ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ਹਾਲਤ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    ਹਾਲਤ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ਮੂਲ:return_trace (c->default_return_ਮੁੱਲ ());
    }
  }

  ਤਾਰਕਿਕ keep_axis_ਮੁੱਲ (ਅਚਲ hb_array_t<ਅਚਲ Stat_Axis_ਰਿਕਾਰਡ> axis_ਰਿਕਾਰਡਾਂ,
                        hb_hashmap_t<hb_ਟੈਗ_t, Triple> *user_axes_location) ਅਚਲ
  {
    ਚੋਣ (u.format.v)
    {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.keep_axis_ਮੁੱਲ (axis_ਰਿਕਾਰਡਾਂ, user_axes_location);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.keep_axis_ਮੁੱਲ (axis_ਰਿਕਾਰਡਾਂ, user_axes_location);
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.keep_axis_ਮੁੱਲ (axis_ਰਿਕਾਰਡਾਂ, user_axes_location);
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.keep_axis_ਮੁੱਲ (axis_ਰਿਕਾਰਡਾਂ, user_axes_location);
    ਮੂਲ:ਵਾਪਸ ਝੂਠ;
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!c->check_struct (ਇਹ)))
      return_trace (ਝੂਠ);
    hb_barrier ();

    ਚੋਣ (u.format.v)
    {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ਹਾਲਤ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ਹਾਲਤ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ਹਾਲਤ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    ਮੂਲ:return_trace (ਸੱਚ);
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ
  {
  ਬਣਤਰ { HBUINT16 v; }	format;
  Axis_ਮੁੱਲ_Format1	format1;
  Axis_ਮੁੱਲ_Format2	format2;
  Axis_ਮੁੱਲ_Format3	format3;
  Axis_ਮੁੱਲ_Format4	format4;
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNION (2, format.v);
};

ਬਣਤਰ Axis_ਮੁੱਲ_ਵਿੱਥ_Array: Unsized_Array_ਦਾ<Offset16_ਮੰਜ਼ਿਲ<Axis_ਮੁੱਲ>>
{
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਮੁੱਲ_ਗਿਣਤੀ,
               ਬਿਨਾਨਿਸ਼ਾਨੀ& ਗਿਣਤੀ_2,
               ਅਚਲ hb_array_t<ਅਚਲ Stat_Axis_ਰਿਕਾਰਡ> axis_ਰਿਕਾਰਡਾਂ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);

    ਸਵੈਚਾਲਿਤ axis_ਮੁੱਲ_Offsets = as_array (axis_ਮੁੱਲ_ਗਿਣਤੀ);
    ਗਿਣਤੀ_2 = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& ਵਿੱਥ : axis_ਮੁੱਲ_Offsets)
    {
      ਜੇ (!ਵਿੱਥ) ਜਾਰੀ;
      ਸਵੈਚਾਲਿਤ o_snap = c->serializer->snapshot ();
      ਸਵੈਚਾਲਿਤ *o = c->serializer->embed (ਵਿੱਥ);
      ਜੇ (!o) return_trace (ਝੂਠ);
      ਜੇ (!o->serialize_subset (c, ਵਿੱਥ, ਇਹ, axis_ਰਿਕਾਰਡਾਂ))
      {
        c->serializer->revert (o_snap);
        ਜਾਰੀ;
      }
      ਗਿਣਤੀ_2++;
    }

    return_trace (ਗਿਣਤੀ_2);
  }
};

ਬਣਤਰ STAT
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_STAT;

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int (); }

  ਤਾਰਕਿਕ get_ਮੁੱਲ (hb_ਟੈਗ_t ਟੈਗ_2, ਦਸ਼ਮਲਵ *value) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ axis_ਸੂਚਕਾਂਕ;
    ਜੇ (!get_design_axes ().lfind (ਟੈਗ_2, &axis_ਸੂਚਕਾਂਕ)) ਵਾਪਸ ਝੂਠ;

    hb_array_t<ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<Axis_ਮੁੱਲ>> axis_ਮੁੱਲਾਂ = get_axis_ਮੁੱਲ_offsets ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < axis_ਮੁੱਲਾਂ.ਲੰਮਾਈ; i++)
    {
      ਅਚਲ Axis_ਮੁੱਲ& axis_ਮੁੱਲ = ਇਹ+ਵਿੱਥ_ਮੰਜ਼ਿਲ_Axis_ਮੁੱਲ_Offsets+axis_ਮੁੱਲਾਂ[i];
      ਜੇ (axis_ਮੁੱਲ.get_axis_ਸੂਚਕਾਂਕ () == axis_ਸੂਚਕਾਂਕ)
      {
	ਜੇ (value)
	  *value = axis_ਮੁੱਲ.get_ਮੁੱਲ (axis_ਸੂਚਕਾਂਕ);
	ਵਾਪਸ ਸੱਚ;
      }
    }
    ਵਾਪਸ ਝੂਠ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_design_axis_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ design_Axis_ਗਿਣਤੀ; }

  hb_ot_name_id_t get_axis_ਰਿਕਾਰਡ_name_id (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਰਿਕਾਰਡ_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਜੇ (unlikely (axis_ਰਿਕਾਰਡ_ਸੂਚਕਾਂਕ >= design_Axis_ਗਿਣਤੀ)) ਵਾਪਸ HB_OT_NAME_ID_INVALID;
    ਅਚਲ Stat_Axis_ਰਿਕਾਰਡ &axis_ਰਿਕਾਰਡ = get_design_axes ()[axis_ਰਿਕਾਰਡ_ਸੂਚਕਾਂਕ];
    ਵਾਪਸ axis_ਰਿਕਾਰਡ.get_name_id ();
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_axis_ਮੁੱਲ_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ axis_ਮੁੱਲ_ਗਿਣਤੀ; }

  hb_ot_name_id_t get_axis_ਮੁੱਲ_name_id (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਮੁੱਲ_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਜੇ (unlikely (axis_ਮੁੱਲ_ਸੂਚਕਾਂਕ >= axis_ਮੁੱਲ_ਗਿਣਤੀ)) ਵਾਪਸ HB_OT_NAME_ID_INVALID;
    ਅਚਲ Axis_ਮੁੱਲ &axis_ਮੁੱਲ = (ਇਹ + get_axis_ਮੁੱਲ_offsets ()[axis_ਮੁੱਲ_ਸੂਚਕਾਂਕ]);
    ਵਾਪਸ axis_ਮੁੱਲ.get_ਮੁੱਲ_name_id ();
  }

  ਖਾਲੀ collect_name_ids (hb_hashmap_t<hb_ਟੈਗ_t, Triple> *user_axes_location,
                         hb_ਸੈੱਟ_t *nameids_ਮੰਜ਼ਿਲ_retain /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (!has_ਅੰਕੜੇ ()) ਵਾਪਸ;

    + get_design_axes ()
    | hb_map (&Stat_Axis_ਰਿਕਾਰਡ::get_name_id)
    | hb_sink (nameids_ਮੰਜ਼ਿਲ_retain)
    ;

    ਸਵੈਚਾਲਿਤ designAxes = get_design_axes ();

    + get_axis_ਮੁੱਲ_offsets ()
    | hb_map (hb_ਜੋੜੋ (&(ਇਹ + ਵਿੱਥ_ਮੰਜ਼ਿਲ_Axis_ਮੁੱਲ_Offsets)))
    | hb_filter ([&] (ਅਚਲ Axis_ਮੁੱਲ& _)
                 { ਵਾਪਸ _.keep_axis_ਮੁੱਲ (designAxes, user_axes_location); })
    | hb_map (&Axis_ਮੁੱਲ::get_ਮੁੱਲ_name_id)
    | hb_sink (nameids_ਮੰਜ਼ਿਲ_retain)
    ;

    nameids_ਮੰਜ਼ਿਲ_retain->ਜੋੜੋ (elidedFallbackNameID);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    STAT *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ designAxes = get_design_axes ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < (ਬਿਨਾਨਿਸ਼ਾਨੀ)design_Axis_ਗਿਣਤੀ; i++)
      ਜੇ (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (ਝੂਠ);

    ਜੇ (design_Axis_ਗਿਣਤੀ)
      c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->design_Axes_ਵਿੱਥ, ਇਹ->get_ਮਾਪ (),
                                   HB_SERIALIZE_ERROR_INT_ਉਛਾਲ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 0;
    ਬਾਹਰਲਾ->ਵਿੱਥ_ਮੰਜ਼ਿਲ_Axis_ਮੁੱਲ_Offsets.serialize_subset (c, ਵਿੱਥ_ਮੰਜ਼ਿਲ_Axis_ਮੁੱਲ_Offsets, ਇਹ,
                                                    axis_ਮੁੱਲ_ਗਿਣਤੀ, ਗਿਣਤੀ_2, designAxes);
    return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->axis_ਮੁੱਲ_ਗਿਣਤੀ, ਗਿਣਤੀ_2, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  hb_barrier () &&
			  ਸੰਸਕਰਣ.major == 1 &&
			  ਸੰਸਕਰਣ.minor > 0 &&
			  design_Axes_ਵਿੱਥ.sanitize (c, ਇਹ, design_Axis_ਗਿਣਤੀ) &&
			  ਵਿੱਥ_ਮੰਜ਼ਿਲ_Axis_ਮੁੱਲ_Offsets.sanitize (c, ਇਹ, axis_ਮੁੱਲ_ਗਿਣਤੀ, &(ਇਹ+ਵਿੱਥ_ਮੰਜ਼ਿਲ_Axis_ਮੁੱਲ_Offsets))));
  }

  ਸੁਰੱਖਿਅਤ:
  hb_array_t<ਅਚਲ Stat_Axis_ਰਿਕਾਰਡ> ਅਚਲ get_design_axes () ਅਚਲ
  { ਵਾਪਸ (ਇਹ+design_Axes_ਵਿੱਥ).as_array (design_Axis_ਗਿਣਤੀ); }

  hb_array_t<ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<Axis_ਮੁੱਲ>> ਅਚਲ get_axis_ਮੁੱਲ_offsets () ਅਚਲ
  { ਵਾਪਸ (ਇਹ+ਵਿੱਥ_ਮੰਜ਼ਿਲ_Axis_ਮੁੱਲ_Offsets).as_array (axis_ਮੁੱਲ_ਗਿਣਤੀ); }


  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	design_Axis_ਮਾਪ;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_ਗਿਣਤੀ;/* The number of design axis records. In a
				 * font with an 'fvar' table, this value must be
				 * greater than or equal to the axisCount value
				 * in the 'fvar' table. In all fonts, must
				 * be greater than zero if axisValueCount
				 * is greater than zero. */
  NNOffset32_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<Stat_Axis_ਰਿਕਾਰਡ>>
		design_Axes_ਵਿੱਥ;
				/* Offset in bytes from the beginning of
				 * the STAT table to the start of the design
				 * axes array. If designAxisCount is zero,
				 * set to zero; if designAxisCount is greater
				 * than zero, must be greater than zero. */
  HBUINT16	axis_ਮੁੱਲ_ਗਿਣਤੀ;	/* The number of axis value tables. */
  NNOffset32_ਮੰਜ਼ਿਲ<Axis_ਮੁੱਲ_ਵਿੱਥ_Array>
		ਵਿੱਥ_ਮੰਜ਼ਿਲ_Axis_ਮੁੱਲ_Offsets;
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
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (20);
};


} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_STAT_TABLE_HH */
