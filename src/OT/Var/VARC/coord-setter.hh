#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#અન્તર્ભાવ "../../../hb.hh"


નામાકાશ OT {
//namespace Var {


સંરચના coord_setter_t
{
  coord_setter_t (hb_array_t<અચલ પૂર્ણાંક> coords_)
  {
    length = coords_.length;
    જો (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * કદ (પૂર્ણાંક));
    નહીંતર
      dynamic_coords.extend (coords_);
  }

  પૂર્ણાંક& operator [] (unsigned idx)
  {
    જો (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      પાછા Crap(પૂર્ણાંક);

    જો (length <= ARRAY_LENGTH (static_coords))
    {
      જો (idx < ARRAY_LENGTH (static_coords))
      {
        જ્યારે (length <= idx)
	  static_coords[length++] = 0;
	પાછા static_coords[idx];
      }
      નહીંતર
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    જો (dynamic_coords.length <= idx)
    {
      જો (unlikely (!dynamic_coords.resize (idx + 1)))
	પાછા Crap(પૂર્ણાંક);
      length = idx + 1;
    }
    પાછા dynamic_coords.arrayZ[idx];
  }

  hb_array_t<પૂર્ણાંક> get_coords ()
  { પાછા length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  ખાનગી:
  hb_vector_t<પૂર્ણાંક> dynamic_coords;
  unsigned length;
  પૂર્ણાંક static_coords[કદ (શૂન્ય *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
