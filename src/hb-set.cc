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

#ضمن "hb-set.hh"


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
hb_مجموعه_t *
hb_مجموعه_create ()
{
  hb_مجموعه_t *مجموعه;

  إذا (!(مجموعه = hb_object_create<hb_مجموعه_t> ()))
    أرجع hb_مجموعه_get_خالی ();

  أرجع مجموعه;
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
hb_مجموعه_t *
hb_مجموعه_get_خالی ()
{
  أرجع const_cast<hb_مجموعه_t *> (&Null (hb_مجموعه_t));
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
hb_مجموعه_t *
hb_مجموعه_reference (hb_مجموعه_t *مجموعه)
{
  أرجع hb_object_reference (مجموعه);
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
فراغ
hb_مجموعه_destroy (hb_مجموعه_t *مجموعه)
{
  إذا (!hb_object_destroy (مجموعه)) أرجع;

  hb_free (مجموعه);
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
hb_مجموعه_مجموعه_user_داده (hb_مجموعه_t           *مجموعه,
		      hb_user_داده_کلید_t *key,
		      فراغ *              داده,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace)
{
  أرجع hb_object_مجموعه_user_داده (مجموعه, key, داده, destroy, replace);
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
فراغ *
hb_مجموعه_get_user_داده (ثابت hb_مجموعه_t     *مجموعه,
		      hb_user_داده_کلید_t *key)
{
  أرجع hb_object_get_user_داده (مجموعه, key);
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
hb_مجموعه_تخصیص_successful (ثابت hb_مجموعه_t  *مجموعه)
{
  أرجع !مجموعه->درون_error ();
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
hb_مجموعه_t *
hb_مجموعه_copy (ثابت hb_مجموعه_t *مجموعه)
{
  hb_مجموعه_t *copy = hb_مجموعه_create ();
  إذا (unlikely (copy->درون_error ()))
    أرجع hb_مجموعه_get_خالی ();

  copy->مجموعه (*مجموعه);
  أرجع copy;
}

/**
 * hb_set_clear:
 * @set: A set
 *
 * Clears out the contents of a set.
 *
 * Since: 0.9.2
 **/
فراغ
hb_مجموعه_clear (hb_مجموعه_t *مجموعه)
{
  /* Immutable-safe. */
  مجموعه->clear ();
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
hb_مجموعه_is_خالی (ثابت hb_مجموعه_t *مجموعه)
{
  أرجع مجموعه->is_empty ();
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
hb_مجموعه_has (ثابت hb_مجموعه_t *مجموعه,
	    hb_codepoint_t  codepoint)
{
  أرجع مجموعه->has (codepoint);
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
فراغ
hb_مجموعه_جمع_کن (hb_مجموعه_t       *مجموعه,
	    hb_codepoint_t  codepoint)
{
  /* Immutable-safe. */
  مجموعه->جمع_کن (codepoint);
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
HB_EXTERN فراغ
hb_مجموعه_جمع_کن_sorted_array (hb_مجموعه_t             *مجموعه,
		         ثابت hb_codepoint_t *sorted_codepoints,
		         بیعلامت صحيح          رقم_codepoints)
{
  /* Immutable-safe. */
  مجموعه->جمع_کن_sorted_array (sorted_codepoints,
		         رقم_codepoints,
		         حجم(hb_codepoint_t));
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
فراغ
hb_مجموعه_جمع_کن_range (hb_مجموعه_t       *مجموعه,
		  hb_codepoint_t  اولی,
		  hb_codepoint_t  آخرین)
{
  /* Immutable-safe. */
  مجموعه->جمع_کن_range (اولی, آخرین);
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
فراغ
hb_مجموعه_del (hb_مجموعه_t       *مجموعه,
	    hb_codepoint_t  codepoint)
{
  /* Immutable-safe. */
  مجموعه->del (codepoint);
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
فراغ
hb_مجموعه_del_range (hb_مجموعه_t       *مجموعه,
		  hb_codepoint_t  اولی,
		  hb_codepoint_t  آخرین)
{
  /* Immutable-safe. */
  مجموعه->del_range (اولی, آخرین);
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
hb_مجموعه_is_equal (ثابت hb_مجموعه_t *مجموعه,
		 ثابت hb_مجموعه_t *other)
{
  أرجع مجموعه->is_equal (*other);
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
hb_مجموعه_intersects (ثابت hb_مجموعه_t *مجموعه,
		   ثابت hb_مجموعه_t *other)
{
  أرجع مجموعه->intersects (*other);
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
HB_EXTERN بیعلامت صحيح
hb_مجموعه_hash (ثابت hb_مجموعه_t *مجموعه)
{
  أرجع مجموعه->hash ();
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
hb_مجموعه_is_subset (ثابت hb_مجموعه_t *مجموعه,
		  ثابت hb_مجموعه_t *larger_مجموعه)
{
  أرجع مجموعه->is_subset (*larger_مجموعه);
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
فراغ
hb_مجموعه_مجموعه (hb_مجموعه_t       *مجموعه,
	    ثابت hb_مجموعه_t *other)
{
  /* Immutable-safe. */
  مجموعه->مجموعه (*other);
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
فراغ
hb_مجموعه_union (hb_مجموعه_t       *مجموعه,
	      ثابت hb_مجموعه_t *other)
{
  /* Immutable-safe. */
  مجموعه->union_ (*other);
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
فراغ
hb_مجموعه_intersect (hb_مجموعه_t       *مجموعه,
		  ثابت hb_مجموعه_t *other)
{
  /* Immutable-safe. */
  مجموعه->intersect (*other);
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
فراغ
hb_مجموعه_تفریق (hb_مجموعه_t       *مجموعه,
		 ثابت hb_مجموعه_t *other)
{
  /* Immutable-safe. */
  مجموعه->تفریق (*other);
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
فراغ
hb_مجموعه_متقارن_difference (hb_مجموعه_t       *مجموعه,
			     ثابت hb_مجموعه_t *other)
{
  /* Immutable-safe. */
  مجموعه->متقارن_difference (*other);
}

/**
 * hb_set_invert:
 * @set: A set
 *
 * Inverts the contents of @set.
 *
 * Since: 3.0.0
 **/
فراغ
hb_مجموعه_invert (hb_مجموعه_t *مجموعه)
{
  /* Immutable-safe. */
  مجموعه->invert ();
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
hb_مجموعه_is_inverted (ثابت hb_مجموعه_t *مجموعه)
{
  أرجع مجموعه->is_inverted ();
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
بیعلامت صحيح
hb_مجموعه_get_population (ثابت hb_مجموعه_t *مجموعه)
{
  أرجع مجموعه->get_population ();
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
hb_مجموعه_get_min (ثابت hb_مجموعه_t *مجموعه)
{
  أرجع مجموعه->get_min ();
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
hb_مجموعه_get_max (ثابت hb_مجموعه_t *مجموعه)
{
  أرجع مجموعه->get_max ();
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
hb_مجموعه_بعدی (ثابت hb_مجموعه_t *مجموعه,
	     hb_codepoint_t *codepoint)
{
  أرجع مجموعه->next (codepoint);
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
hb_مجموعه_قبلی (ثابت hb_مجموعه_t *مجموعه,
		 hb_codepoint_t *codepoint)
{
  أرجع مجموعه->قبلی (codepoint);
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
hb_مجموعه_بعدی_range (ثابت hb_مجموعه_t *مجموعه,
		   hb_codepoint_t *اولی,
		   hb_codepoint_t *آخرین)
{
  أرجع مجموعه->بعدی_range (اولی, آخرین);
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
hb_مجموعه_قبلی_range (ثابت hb_مجموعه_t *مجموعه,
		       hb_codepoint_t *اولی,
		       hb_codepoint_t *آخرین)
{
  أرجع مجموعه->قبلی_range (اولی, آخرین);
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
بیعلامت صحيح
hb_مجموعه_بعدی_many (ثابت hb_مجموعه_t *مجموعه,
		  hb_codepoint_t  codepoint,
		  hb_codepoint_t *بیرون,
		  بیعلامت صحيح    اندازه)
{
  أرجع مجموعه->بعدی_many (codepoint, بیرون, اندازه);
}
