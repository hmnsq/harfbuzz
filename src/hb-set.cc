/*
 * Copyright © 2012  Google, Inc.
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

#అన్తర్భావ "hb-set.hh"


/**
 * SECTION:hb-set
 * @title: hb-set
 * @short_description: Objects representing a set of integers
 * @include: hb.h
 *
 * Set objects represent a mathematical set of integer values.  They are
 * used in non-shaping APIs to query certain sets of characters or glyphs,
 * or other integer values.
 **/


/**
 * hb_set_create:
 *
 * Creates a new, initially empty set.
 *
 * Return value: (transfer full): The new #hb_set_t
 *
 * Since: 0.9.2
 **/
hb_సమితి_t *
hb_సమితి_create ()
{
  hb_సమితి_t *సమితి;

  ఒకవేళ (!(సమితి = hb_object_create<hb_సమితి_t> ()))
    తిరిగి hb_సమితి_get_ఖాళీలు ();

  తిరిగి సమితి;
}

/**
 * hb_set_get_empty:
 *
 * Fetches the singleton empty #hb_set_t.
 *
 * Return value: (transfer full): The empty #hb_set_t
 *
 * Since: 0.9.2
 **/
hb_సమితి_t *
hb_సమితి_get_ఖాళీలు ()
{
  తిరిగి const_cast<hb_సమితి_t *> (&Null (hb_సమితి_t));
}

/**
 * hb_set_reference: (skip)
 * @set: A set
 *
 * Increases the reference count on a set.
 *
 * Return value: (transfer full): The set
 *
 * Since: 0.9.2
 **/
hb_సమితి_t *
hb_సమితి_reference (hb_సమితి_t *సమితి)
{
  తిరిగి hb_object_reference (సమితి);
}

/**
 * hb_set_destroy: (skip)
 * @set: A set
 *
 * Decreases the reference count on a set. When
 * the reference count reaches zero, the set is
 * destroyed, freeing all memory.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_సమితి_destroy (hb_సమితి_t *సమితి)
{
  ఒకవేళ (!hb_object_destroy (సమితి)) తిరిగి;

  hb_free (సమితి);
}

/**
 * hb_set_set_user_data: (skip)
 * @set: A set
 * @key: The user-data key to set
 * @data: A pointer to the user data to set
 * @destroy: (nullable): A callback to call when @data is not needed anymore
 * @replace: Whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the specified set.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_సమితి_సమితి_user_దత్తాంశం (hb_సమితి_t           *సమితి,
		      hb_user_దత్తాంశం_కీలకం_t *key,
		      శూన్య *              దత్తాంశం,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace)
{
  తిరిగి hb_object_సమితి_user_దత్తాంశం (సమితి, key, దత్తాంశం, destroy, replace);
}

/**
 * hb_set_get_user_data: (skip)
 * @set: A set
 * @key: The user-data key to query
 *
 * Fetches the user data associated with the specified key,
 * attached to the specified set.
 *
 * Return value: (transfer none): A pointer to the user data
 *
 * Since: 0.9.2
 **/
శూన్య *
hb_సమితి_get_user_దత్తాంశం (స్థిరరాశి hb_సమితి_t     *సమితి,
		      hb_user_దత్తాంశం_కీలకం_t *key)
{
  తిరిగి hb_object_get_user_దత్తాంశం (సమితి, key);
}


/**
 * hb_set_allocation_successful:
 * @set: A set
 *
 * Tests whether memory allocation for a set was successful.
 *
 * Return value: `true` if allocation succeeded, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_సమితి_కేటాయింపు_successful (స్థిరరాశి hb_సమితి_t  *సమితి)
{
  తిరిగి !సమితి->లోపల_error ();
}

/**
 * hb_set_copy:
 * @set: A set
 *
 * Allocate a copy of @set.
 *
 * Return value: (transfer full): Newly-allocated set.
 *
 * Since: 2.8.2
 **/
hb_సమితి_t *
hb_సమితి_copy (స్థిరరాశి hb_సమితి_t *సమితి)
{
  hb_సమితి_t *copy = hb_సమితి_create ();
  ఒకవేళ (unlikely (copy->లోపల_error ()))
    తిరిగి hb_సమితి_get_ఖాళీలు ();

  copy->సమితి (*సమితి);
  తిరిగి copy;
}

/**
 * hb_set_clear:
 * @set: A set
 *
 * Clears out the contents of a set.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_సమితి_clear (hb_సమితి_t *సమితి)
{
  /* Immutable-safe. */
  సమితి->clear ();
}

/**
 * hb_set_is_empty:
 * @set: a set.
 *
 * Tests whether a set is empty (contains no elements).
 *
 * Return value: `true` if @set is empty
 *
 * Since: 0.9.7
 **/
hb_bool_t
hb_సమితి_is_ఖాళీలు (స్థిరరాశి hb_సమితి_t *సమితి)
{
  తిరిగి సమితి->is_empty ();
}

/**
 * hb_set_has:
 * @set: A set
 * @codepoint: The element to query
 *
 * Tests whether @codepoint belongs to @set.
 *
 * Return value: `true` if @codepoint is in @set, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_సమితి_has (స్థిరరాశి hb_సమితి_t *సమితి,
	    hb_codepoint_t  codepoint)
{
  తిరిగి సమితి->has (codepoint);
}

/**
 * hb_set_add:
 * @set: A set
 * @codepoint: The element to add to @set
 *
 * Adds @codepoint to @set.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_సమితి_కూడు (hb_సమితి_t       *సమితి,
	    hb_codepoint_t  codepoint)
{
  /* Immutable-safe. */
  సమితి->కూడు (codepoint);
}

/**
 * hb_set_add_sorted_array:
 * @set: A set
 * @sorted_codepoints: (array length=num_codepoints): Array of codepoints to add
 * @num_codepoints: Length of @sorted_codepoints
 *
 * Adds @num_codepoints codepoints to a set at once.
 * The codepoints array must be in increasing order,
 * with size at least @num_codepoints.
 *
 * Since: 4.1.0
 */
HB_EXTERN శూన్య
hb_సమితి_కూడు_sorted_array (hb_సమితి_t             *సమితి,
		         స్థిరరాశి hb_codepoint_t *sorted_codepoints,
		         గుర్తులేని పూర్ణాంకం          అంకె_codepoints)
{
  /* Immutable-safe. */
  సమితి->కూడు_sorted_array (sorted_codepoints,
		         అంకె_codepoints,
		         పరిమాణం(hb_codepoint_t));
}

/**
 * hb_set_add_range:
 * @set: A set
 * @first: The first element to add to @set
 * @last: The final element to add to @set
 *
 * Adds all of the elements from @first to @last
 * (inclusive) to @set.
 *
 * Since: 0.9.7
 **/
శూన్య
hb_సమితి_కూడు_range (hb_సమితి_t       *సమితి,
		  hb_codepoint_t  మొదటి,
		  hb_codepoint_t  చివరి)
{
  /* Immutable-safe. */
  సమితి->కూడు_range (మొదటి, చివరి);
}

/**
 * hb_set_del:
 * @set: A set
 * @codepoint: Removes @codepoint from @set
 *
 * Removes @codepoint from @set.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_సమితి_del (hb_సమితి_t       *సమితి,
	    hb_codepoint_t  codepoint)
{
  /* Immutable-safe. */
  సమితి->del (codepoint);
}

/**
 * hb_set_del_range:
 * @set: A set
 * @first: The first element to remove from @set
 * @last: The final element to remove from @set
 *
 * Removes all of the elements from @first to @last
 * (inclusive) from @set.
 *
 * If @last is #HB_SET_VALUE_INVALID, then all values
 * greater than or equal to @first are removed.
 *
 * Since: 0.9.7
 **/
శూన్య
hb_సమితి_del_range (hb_సమితి_t       *సమితి,
		  hb_codepoint_t  మొదటి,
		  hb_codepoint_t  చివరి)
{
  /* Immutable-safe. */
  సమితి->del_range (మొదటి, చివరి);
}

/**
 * hb_set_is_equal:
 * @set: A set
 * @other: Another set
 *
 * Tests whether @set and @other are equal (contain the same
 * elements).
 *
 * Return value: `true` if the two sets are equal, `false` otherwise.
 *
 * Since: 0.9.7
 **/
hb_bool_t
hb_సమితి_is_equal (స్థిరరాశి hb_సమితి_t *సమితి,
		 స్థిరరాశి hb_సమితి_t *other)
{
  తిరిగి సమితి->is_equal (*other);
}

/**
 * hb_set_intersects:
 * @set: A set
 * @other: Another set
 *
 * Tests whether @set and @other have any elements in common.
 *
 * Return value: `true` if the two sets intersect, `false` otherwise.
 *
 * Since: 14.4.0
 **/
hb_bool_t
hb_సమితి_intersects (స్థిరరాశి hb_సమితి_t *సమితి,
		   స్థిరరాశి hb_సమితి_t *other)
{
  తిరిగి సమితి->intersects (*other);
}

/**
 * hb_set_hash:
 * @set: A set
 *
 * Creates a hash representing @set.
 *
 * Return value:
 * A hash of @set.
 *
 * Since: 4.4.0
 **/
HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_సమితి_hash (స్థిరరాశి hb_సమితి_t *సమితి)
{
  తిరిగి సమితి->hash ();
}

/**
 * hb_set_is_subset:
 * @set: A set
 * @larger_set: Another set
 *
 * Tests whether @set is a subset of @larger_set.
 *
 * Return value: `true` if the @set is a subset of (or equal to) @larger_set, `false` otherwise.
 *
 * Since: 1.8.1
 **/
hb_bool_t
hb_సమితి_is_subset (స్థిరరాశి hb_సమితి_t *సమితి,
		  స్థిరరాశి hb_సమితి_t *larger_సమితి)
{
  తిరిగి సమితి->is_subset (*larger_సమితి);
}

/**
 * hb_set_set:
 * @set: A set
 * @other: Another set
 *
 * Makes the contents of @set equal to the contents of @other.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_సమితి_సమితి (hb_సమితి_t       *సమితి,
	    స్థిరరాశి hb_సమితి_t *other)
{
  /* Immutable-safe. */
  సమితి->సమితి (*other);
}

/**
 * hb_set_union:
 * @set: A set
 * @other: Another set
 *
 * Makes @set the union of @set and @other.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_సమితి_union (hb_సమితి_t       *సమితి,
	      స్థిరరాశి hb_సమితి_t *other)
{
  /* Immutable-safe. */
  సమితి->union_ (*other);
}

/**
 * hb_set_intersect:
 * @set: A set
 * @other: Another set
 *
 * Makes @set the intersection of @set and @other.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_సమితి_intersect (hb_సమితి_t       *సమితి,
		  స్థిరరాశి hb_సమితి_t *other)
{
  /* Immutable-safe. */
  సమితి->intersect (*other);
}

/**
 * hb_set_subtract:
 * @set: A set
 * @other: Another set
 *
 * Subtracts the contents of @other from @set.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_సమితి_వ్యవకలనం (hb_సమితి_t       *సమితి,
		 స్థిరరాశి hb_సమితి_t *other)
{
  /* Immutable-safe. */
  సమితి->వ్యవకలనం (*other);
}

/**
 * hb_set_symmetric_difference:
 * @set: A set
 * @other: Another set
 *
 * Makes @set the symmetric difference of @set
 * and @other.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_సమితి_సౌష్ఠవ_difference (hb_సమితి_t       *సమితి,
			     స్థిరరాశి hb_సమితి_t *other)
{
  /* Immutable-safe. */
  సమితి->సౌష్ఠవ_difference (*other);
}

/**
 * hb_set_invert:
 * @set: A set
 *
 * Inverts the contents of @set.
 *
 * Since: 3.0.0
 **/
శూన్య
hb_సమితి_invert (hb_సమితి_t *సమితి)
{
  /* Immutable-safe. */
  సమితి->invert ();
}

/**
 * hb_set_is_inverted:
 * @set: A set
 *
 * Returns whether the set is inverted.
 *
 * Return value: `true` if the set is inverted, `false` otherwise
 *
 * Since: 7.0.0
 **/
hb_bool_t
hb_సమితి_is_inverted (స్థిరరాశి hb_సమితి_t *సమితి)
{
  తిరిగి సమితి->is_inverted ();
}

/**
 * hb_set_get_population:
 * @set: A set
 *
 * Returns the number of elements in the set.
 *
 * Return value: The population of @set
 *
 * Since: 0.9.7
 **/
గుర్తులేని పూర్ణాంకం
hb_సమితి_get_population (స్థిరరాశి hb_సమితి_t *సమితి)
{
  తిరిగి సమితి->get_population ();
}

/**
 * hb_set_get_min:
 * @set: A set
 *
 * Finds the smallest element in the set.
 *
 * Return value: minimum of @set, or #HB_SET_VALUE_INVALID if @set is empty.
 *
 * Since: 0.9.7
 **/
hb_codepoint_t
hb_సమితి_get_min (స్థిరరాశి hb_సమితి_t *సమితి)
{
  తిరిగి సమితి->get_min ();
}

/**
 * hb_set_get_max:
 * @set: A set
 *
 * Finds the largest element in the set.
 *
 * Return value: maximum of @set, or #HB_SET_VALUE_INVALID if @set is empty.
 *
 * Since: 0.9.7
 **/
hb_codepoint_t
hb_సమితి_get_max (స్థిరరాశి hb_సమితి_t *సమితి)
{
  తిరిగి సమితి->get_max ();
}

/**
 * hb_set_next:
 * @set: A set
 * @codepoint: (inout): Input = Code point to query
 *             Output = Code point retrieved
 *
 * Fetches the next element in @set that is greater than current value of @codepoint.
 *
 * Set @codepoint to #HB_SET_VALUE_INVALID to get started.
 *
 * Return value: `true` if there was a next value, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_సమితి_తదుపరి (స్థిరరాశి hb_సమితి_t *సమితి,
	     hb_codepoint_t *codepoint)
{
  తిరిగి సమితి->next (codepoint);
}

/**
 * hb_set_previous:
 * @set: A set
 * @codepoint: (inout): Input = Code point to query
 *             Output = Code point retrieved
 *
 * Fetches the previous element in @set that is lower than current value of @codepoint.
 *
 * Set @codepoint to #HB_SET_VALUE_INVALID to get started.
 *
 * Return value: `true` if there was a previous value, `false` otherwise
 *
 * Since: 1.8.0
 **/
hb_bool_t
hb_సమితి_మునుపటి (స్థిరరాశి hb_సమితి_t *సమితి,
		 hb_codepoint_t *codepoint)
{
  తిరిగి సమితి->మునుపటి (codepoint);
}

/**
 * hb_set_next_range:
 * @set: A set
 * @first: (out): The first code point in the range
 * @last: (inout): Input = The current last code point in the range
 *         Output = The last code point in the range
 *
 * Fetches the next consecutive range of elements in @set that
 * are greater than current value of @last.
 *
 * Set @last to #HB_SET_VALUE_INVALID to get started.
 *
 * Return value: `true` if there was a next range, `false` otherwise
 *
 * Since: 0.9.7
 **/
hb_bool_t
hb_సమితి_తదుపరి_range (స్థిరరాశి hb_సమితి_t *సమితి,
		   hb_codepoint_t *మొదటి,
		   hb_codepoint_t *చివరి)
{
  తిరిగి సమితి->తదుపరి_range (మొదటి, చివరి);
}

/**
 * hb_set_previous_range:
 * @set: A set
 * @first: (inout): Input = The current first code point in the range
 *         Output = The first code point in the range
 * @last: (out): The last code point in the range
 *
 * Fetches the previous consecutive range of elements in @set that
 * are greater than current value of @last.
 *
 * Set @first to #HB_SET_VALUE_INVALID to get started.
 *
 * Return value: `true` if there was a previous range, `false` otherwise
 *
 * Since: 1.8.0
 **/
hb_bool_t
hb_సమితి_మునుపటి_range (స్థిరరాశి hb_సమితి_t *సమితి,
		       hb_codepoint_t *మొదటి,
		       hb_codepoint_t *చివరి)
{
  తిరిగి సమితి->మునుపటి_range (మొదటి, చివరి);
}

/**
 * hb_set_next_many:
 * @set: A set
 * @codepoint: Outputting codepoints starting after this one.
 *             Use #HB_SET_VALUE_INVALID to get started.
 * @out: (array length=size): An array of codepoints to write to.
 * @size: The maximum number of codepoints to write out.
 *
 * Finds the next element in @set that is greater than @codepoint. Writes out
 * codepoints to @out, until either the set runs out of elements, or @size
 * codepoints are written, whichever comes first.
 *
 * Return value: the number of values written.
 *
 * Since: 4.2.0
 **/
గుర్తులేని పూర్ణాంకం
hb_సమితి_తదుపరి_many (స్థిరరాశి hb_సమితి_t *సమితి,
		  hb_codepoint_t  codepoint,
		  hb_codepoint_t *బహిర్గతం,
		  గుర్తులేని పూర్ణాంకం    కొలత)
{
  తిరిగి సమితి->తదుపరి_many (codepoint, బహిర్గతం, కొలత);
}
