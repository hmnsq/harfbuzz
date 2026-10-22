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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.hh"


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
hb_ਸੈੱਟ_t *
hb_ਸੈੱਟ_create ()
{
  hb_ਸੈੱਟ_t *ਸੈੱਟ;

  ਜੇ (!(ਸੈੱਟ = hb_object_create<hb_ਸੈੱਟ_t> ()))
    ਵਾਪਸ hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  ਵਾਪਸ ਸੈੱਟ;
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
hb_ਸੈੱਟ_t *
hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ()
{
  ਵਾਪਸ const_cast<hb_ਸੈੱਟ_t *> (&Null (hb_ਸੈੱਟ_t));
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
hb_ਸੈੱਟ_t *
hb_ਸੈੱਟ_reference (hb_ਸੈੱਟ_t *ਸੈੱਟ)
{
  ਵਾਪਸ hb_object_reference (ਸੈੱਟ);
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
ਖਾਲੀ
hb_ਸੈੱਟ_destroy (hb_ਸੈੱਟ_t *ਸੈੱਟ)
{
  ਜੇ (!hb_object_destroy (ਸੈੱਟ)) ਵਾਪਸ;

  hb_free (ਸੈੱਟ);
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
hb_ਸੈੱਟ_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_ਸੈੱਟ_t           *ਸੈੱਟ,
		      hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
		      ਖਾਲੀ *              ਅੰਕੜੇ,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace)
{
  ਵਾਪਸ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (ਸੈੱਟ, key, ਅੰਕੜੇ, destroy, replace);
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
ਖਾਲੀ *
hb_ਸੈੱਟ_get_user_ਅੰਕੜੇ (ਅਚਲ hb_ਸੈੱਟ_t     *ਸੈੱਟ,
		      hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key)
{
  ਵਾਪਸ hb_object_get_user_ਅੰਕੜੇ (ਸੈੱਟ, key);
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
hb_ਸੈੱਟ_ਵੰਡ_successful (ਅਚਲ hb_ਸੈੱਟ_t  *ਸੈੱਟ)
{
  ਵਾਪਸ !ਸੈੱਟ->ਵਿੱਚ_error ();
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
hb_ਸੈੱਟ_t *
hb_ਸੈੱਟ_copy (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ)
{
  hb_ਸੈੱਟ_t *copy = hb_ਸੈੱਟ_create ();
  ਜੇ (unlikely (copy->ਵਿੱਚ_error ()))
    ਵਾਪਸ hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  copy->ਸੈੱਟ (*ਸੈੱਟ);
  ਵਾਪਸ copy;
}

/**
 * hb_set_clear:
 * @set: A set
 *
 * Clears out the contents of a set.
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_ਸੈੱਟ_clear (hb_ਸੈੱਟ_t *ਸੈੱਟ)
{
  /* Immutable-safe. */
  ਸੈੱਟ->clear ();
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
hb_ਸੈੱਟ_is_ਖਾਲੀ_ਥਾਵਾਂ (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ)
{
  ਵਾਪਸ ਸੈੱਟ->is_empty ();
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
hb_ਸੈੱਟ_has (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
	    hb_codepoint_t  codepoint)
{
  ਵਾਪਸ ਸੈੱਟ->has (codepoint);
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
ਖਾਲੀ
hb_ਸੈੱਟ_ਜੋੜੋ (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
	    hb_codepoint_t  codepoint)
{
  /* Immutable-safe. */
  ਸੈੱਟ->ਜੋੜੋ (codepoint);
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
HB_EXTERN ਖਾਲੀ
hb_ਸੈੱਟ_ਜੋੜੋ_sorted_array (hb_ਸੈੱਟ_t             *ਸੈੱਟ,
		         ਅਚਲ hb_codepoint_t *sorted_codepoints,
		         ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          ਅੰਕ_codepoints)
{
  /* Immutable-safe. */
  ਸੈੱਟ->ਜੋੜੋ_sorted_array (sorted_codepoints,
		         ਅੰਕ_codepoints,
		         ਆਕਾਰ(hb_codepoint_t));
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
ਖਾਲੀ
hb_ਸੈੱਟ_ਜੋੜੋ_range (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
		  hb_codepoint_t  ਪਹਿਲਾ,
		  hb_codepoint_t  ਆਖਰੀ)
{
  /* Immutable-safe. */
  ਸੈੱਟ->ਜੋੜੋ_range (ਪਹਿਲਾ, ਆਖਰੀ);
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
ਖਾਲੀ
hb_ਸੈੱਟ_del (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
	    hb_codepoint_t  codepoint)
{
  /* Immutable-safe. */
  ਸੈੱਟ->del (codepoint);
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
ਖਾਲੀ
hb_ਸੈੱਟ_del_range (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
		  hb_codepoint_t  ਪਹਿਲਾ,
		  hb_codepoint_t  ਆਖਰੀ)
{
  /* Immutable-safe. */
  ਸੈੱਟ->del_range (ਪਹਿਲਾ, ਆਖਰੀ);
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
hb_ਸੈੱਟ_is_equal (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
		 ਅਚਲ hb_ਸੈੱਟ_t *other)
{
  ਵਾਪਸ ਸੈੱਟ->is_equal (*other);
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
hb_ਸੈੱਟ_intersects (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
		   ਅਚਲ hb_ਸੈੱਟ_t *other)
{
  ਵਾਪਸ ਸੈੱਟ->intersects (*other);
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
HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ਸੈੱਟ_hash (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ)
{
  ਵਾਪਸ ਸੈੱਟ->hash ();
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
hb_ਸੈੱਟ_is_subset (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
		  ਅਚਲ hb_ਸੈੱਟ_t *larger_ਸੈੱਟ)
{
  ਵਾਪਸ ਸੈੱਟ->is_subset (*larger_ਸੈੱਟ);
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
ਖਾਲੀ
hb_ਸੈੱਟ_ਸੈੱਟ (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
	    ਅਚਲ hb_ਸੈੱਟ_t *other)
{
  /* Immutable-safe. */
  ਸੈੱਟ->ਸੈੱਟ (*other);
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
ਖਾਲੀ
hb_ਸੈੱਟ_union (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
	      ਅਚਲ hb_ਸੈੱਟ_t *other)
{
  /* Immutable-safe. */
  ਸੈੱਟ->union_ (*other);
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
ਖਾਲੀ
hb_ਸੈੱਟ_intersect (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
		  ਅਚਲ hb_ਸੈੱਟ_t *other)
{
  /* Immutable-safe. */
  ਸੈੱਟ->intersect (*other);
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
ਖਾਲੀ
hb_ਸੈੱਟ_ਘਟਾਓ (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
		 ਅਚਲ hb_ਸੈੱਟ_t *other)
{
  /* Immutable-safe. */
  ਸੈੱਟ->ਘਟਾਓ (*other);
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
ਖਾਲੀ
hb_ਸੈੱਟ_ਸਮਮਿਤ_difference (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
			     ਅਚਲ hb_ਸੈੱਟ_t *other)
{
  /* Immutable-safe. */
  ਸੈੱਟ->ਸਮਮਿਤ_difference (*other);
}

/**
 * hb_set_invert:
 * @set: A set
 *
 * Inverts the contents of @set.
 *
 * Since: 3.0.0
 **/
ਖਾਲੀ
hb_ਸੈੱਟ_invert (hb_ਸੈੱਟ_t *ਸੈੱਟ)
{
  /* Immutable-safe. */
  ਸੈੱਟ->invert ();
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
hb_ਸੈੱਟ_is_inverted (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ)
{
  ਵਾਪਸ ਸੈੱਟ->is_inverted ();
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ਸੈੱਟ_get_population (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ)
{
  ਵਾਪਸ ਸੈੱਟ->get_population ();
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
hb_ਸੈੱਟ_get_min (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ)
{
  ਵਾਪਸ ਸੈੱਟ->get_min ();
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
hb_ਸੈੱਟ_get_max (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ)
{
  ਵਾਪਸ ਸੈੱਟ->get_max ();
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
hb_ਸੈੱਟ_ਅਗਲਾ (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
	     hb_codepoint_t *codepoint)
{
  ਵਾਪਸ ਸੈੱਟ->next (codepoint);
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
hb_ਸੈੱਟ_ਪਿਛਲਾ (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
		 hb_codepoint_t *codepoint)
{
  ਵਾਪਸ ਸੈੱਟ->ਪਿਛਲਾ (codepoint);
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
hb_ਸੈੱਟ_ਅਗਲਾ_range (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
		   hb_codepoint_t *ਪਹਿਲਾ,
		   hb_codepoint_t *ਆਖਰੀ)
{
  ਵਾਪਸ ਸੈੱਟ->ਅਗਲਾ_range (ਪਹਿਲਾ, ਆਖਰੀ);
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
hb_ਸੈੱਟ_ਪਿਛਲਾ_range (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
		       hb_codepoint_t *ਪਹਿਲਾ,
		       hb_codepoint_t *ਆਖਰੀ)
{
  ਵਾਪਸ ਸੈੱਟ->ਪਿਛਲਾ_range (ਪਹਿਲਾ, ਆਖਰੀ);
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ਸੈੱਟ_ਅਗਲਾ_many (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
		  hb_codepoint_t  codepoint,
		  hb_codepoint_t *ਬਾਹਰਲਾ,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਮਾਪ)
{
  ਵਾਪਸ ਸੈੱਟ->ਅਗਲਾ_many (codepoint, ਬਾਹਰਲਾ, ਮਾਪ);
}
