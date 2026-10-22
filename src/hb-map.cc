/*
 * Copyright © 2018  Google, Inc.
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

#شامل "hb-map.hh"


/**
 * SECTION:hb-map
 * @title: hb-map
 * @short_description: Object representing integer to integer mapping
 * @include: hb.h
 *
 * Map objects are integer-to-integer hash-maps.  Currently they are
 * not used in the HarfBuzz public API, but are provided for client's
 * use if desired.
 **/


/**
 * hb_map_create:
 *
 * Creates a new, initially empty map.
 *
 * Return value: (transfer full): The new #hb_map_t
 *
 * Since: 1.7.7
 **/
hb_map_t *
hb_map_create ()
{
  hb_map_t *map;

  اگر (!(map = hb_object_create<hb_map_t> ()))
    بازگشت hb_map_get_empty ();

  بازگشت map;
}

/**
 * hb_map_get_empty:
 *
 * Fetches the singleton empty #hb_map_t.
 *
 * Return value: (transfer full): The empty #hb_map_t
 *
 * Since: 1.7.7
 **/
hb_map_t *
hb_map_get_empty ()
{
  بازگشت const_cast<hb_map_t *> (&Null (hb_map_t));
}

/**
 * hb_map_reference: (skip)
 * @map: A map
 *
 * Increases the reference count on a map.
 *
 * Return value: (transfer full): The map
 *
 * Since: 1.7.7
 **/
hb_map_t *
hb_map_reference (hb_map_t *map)
{
  بازگشت hb_object_reference (map);
}

/**
 * hb_map_destroy: (skip)
 * @map: A map
 *
 * Decreases the reference count on a map. When
 * the reference count reaches zero, the map is
 * destroyed, freeing all memory.
 *
 * Since: 1.7.7
 **/
تهی
hb_map_destroy (hb_map_t *map)
{
  اگر (!hb_object_destroy (map)) بازگشت;

  hb_free (map);
}

/**
 * hb_map_set_user_data: (skip)
 * @map: A map
 * @key: The user-data key to set
 * @data: A pointer to the user data to set
 * @destroy: (nullable): A callback to call when @data is not needed anymore
 * @replace: Whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the specified map.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 1.7.7
 **/
hb_bool_t
hb_map_set_user_data (hb_map_t           *map,
		      hb_user_data_کلید_t *key,
		      تهی *              data,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace)
{
  بازگشت hb_object_set_user_data (map, key, data, destroy, replace);
}

/**
 * hb_map_get_user_data: (skip)
 * @map: A map
 * @key: The user-data key to query
 *
 * Fetches the user data associated with the specified key,
 * attached to the specified map.
 *
 * Return value: (transfer none): A pointer to the user data
 *
 * Since: 1.7.7
 **/
تهی *
hb_map_get_user_data (ثابت hb_map_t     *map,
		      hb_user_data_کلید_t *key)
{
  بازگشت hb_object_get_user_data (map, key);
}


/**
 * hb_map_allocation_successful:
 * @map: A map
 *
 * Tests whether memory allocation for a set was successful.
 *
 * Return value: `true` if allocation succeeded, `false` otherwise
 *
 * Since: 1.7.7
 **/
hb_bool_t
hb_map_allocation_successful (ثابت hb_map_t  *map)
{
  بازگشت map->successful;
}

/**
 * hb_map_copy:
 * @map: A map
 *
 * Allocate a copy of @map.
 *
 * Return value: (transfer full): Newly-allocated map.
 *
 * Since: 4.4.0
 **/
hb_map_t *
hb_map_copy (ثابت hb_map_t *map)
{
  hb_map_t *copy = hb_map_create ();
  اگر (unlikely (copy->in_error ()))
    بازگشت hb_map_get_empty ();

  *copy = *map;
  بازگشت copy;
}

/**
 * hb_map_set:
 * @map: A map
 * @key: The key to store in the map
 * @value: The value to store for @key
 *
 * Stores @key:@value in the map.
 *
 * Since: 1.7.7
 **/
تهی
hb_map_set (hb_map_t       *map,
	    hb_codepoint_t  key,
	    hb_codepoint_t  value)
{
  /* Immutable-safe. */
  map->set (key, value);
}

/**
 * hb_map_get:
 * @map: A map
 * @key: The key to query
 *
 * Fetches the value stored for @key in @map.
 *
 * Since: 1.7.7
 **/
hb_codepoint_t
hb_map_get (ثابت hb_map_t *map,
	    hb_codepoint_t  key)
{
  بازگشت map->get (key);
}

/**
 * hb_map_del:
 * @map: A map
 * @key: The key to delete
 *
 * Removes @key and its stored value from @map.
 *
 * Since: 1.7.7
 **/
تهی
hb_map_del (hb_map_t       *map,
	    hb_codepoint_t  key)
{
  /* Immutable-safe. */
  map->del (key);
}

/**
 * hb_map_has:
 * @map: A map
 * @key: The key to query
 *
 * Tests whether @key is an element of @map.
 *
 * Return value: `true` if @key is found in @map, `false` otherwise
 *
 * Since: 1.7.7
 **/
hb_bool_t
hb_map_has (ثابت hb_map_t *map,
	    hb_codepoint_t  key)
{
  بازگشت map->has (key);
}


/**
 * hb_map_clear:
 * @map: A map
 *
 * Clears out the contents of @map.
 *
 * Since: 1.7.7
 **/
تهی
hb_map_clear (hb_map_t *map)
{
  بازگشت map->clear ();
}

/**
 * hb_map_is_empty:
 * @map: A map
 *
 * Tests whether @map is empty (contains no elements).
 *
 * Return value: `true` if @map is empty
 *
 * Since: 1.7.7
 **/
hb_bool_t
hb_map_is_empty (ثابت hb_map_t *map)
{
  بازگشت map->is_empty ();
}

/**
 * hb_map_get_population:
 * @map: A map
 *
 * Returns the number of key-value pairs in the map.
 *
 * Return value: The population of @map
 *
 * Since: 1.7.7
 **/
unsigned صحیح
hb_map_get_population (ثابت hb_map_t *map)
{
  بازگشت map->get_population ();
}

/**
 * hb_map_is_equal:
 * @map: A map
 * @other: Another map
 *
 * Tests whether @map and @other are equal (contain the same
 * elements).
 *
 * Return value: `true` if the two maps are equal, `false` otherwise.
 *
 * Since: 4.3.0
 **/
hb_bool_t
hb_map_is_equal (ثابت hb_map_t *map,
		 ثابت hb_map_t *other)
{
  بازگشت map->is_equal (*other);
}

/**
 * hb_map_hash:
 * @map: A map
 *
 * Creates a hash representing @map.
 *
 * Return value:
 * A hash of @map.
 *
 * Since: 4.4.0
 **/
unsigned صحیح
hb_map_hash (ثابت hb_map_t *map)
{
  بازگشت map->hash ();
}

/**
 * hb_map_update:
 * @map: A map
 * @other: Another map
 *
 * Add the contents of @other to @map.
 *
 * Since: 7.0.0
 **/
HB_EXTERN تهی
hb_map_update (hb_map_t *map,
	       ثابت hb_map_t *other)
{
  map->update (*other);
}

/**
 * hb_map_next:
 * @map: A map
 * @idx: (inout): Iterator internal state
 * @key: (out): Key retrieved
 * @value: (out): Value retrieved
 *
 * Fetches the next key/value pair in @map.
 *
 * Set @idx to -1 to get started.
 *
 * If the map is modified during iteration, the behavior is undefined.
 *
 * The order in which the key/values are returned is undefined.
 *
 * Return value: `true` if there was a next value, `false` otherwise
 *
 * Since: 7.0.0
 **/
hb_bool_t
hb_map_next (ثابت hb_map_t *map,
	     صحیح *idx,
	     hb_codepoint_t *key,
	     hb_codepoint_t *value)
{
  بازگشت map->next (idx, key, value);
}

/**
 * hb_map_keys:
 * @map: A map
 * @keys: A set
 *
 * Add the keys of @map to @keys.
 *
 * Since: 7.0.0
 **/
تهی
hb_map_کلیدها (ثابت hb_map_t *map,
	     hb_set_t *کلیدها)
{
  hb_copy (map->کلیدها() , *کلیدها);
}

/**
 * hb_map_values:
 * @map: A map
 * @values: A set
 *
 * Add the values of @map to @values.
 *
 * Since: 7.0.0
 **/
تهی
hb_map_مقادیر (ثابت hb_map_t *map,
	       hb_set_t *values)
{
  hb_copy (map->values() , *values);
}
