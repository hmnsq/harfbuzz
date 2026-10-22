#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#அந்தர்பாவ "../../../hb.hh"


நாமாகாஶ OT {
//namespace Var {


கட்டமைப்பு coord_setter_t
{
  coord_setter_t (hb_array_t<மாறிலி முழு> coords_)
  {
    length = coords_.length;
    எனில் (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * அளவு (முழு));
    இல்லையெனில்
      dynamic_coords.extend (coords_);
  }

  முழு& operator [] (unsigned idx)
  {
    எனில் (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      திரும்பு Crap(முழு);

    எனில் (length <= ARRAY_LENGTH (static_coords))
    {
      எனில் (idx < ARRAY_LENGTH (static_coords))
      {
        போது (length <= idx)
	  static_coords[length++] = 0;
	திரும்பு static_coords[idx];
      }
      இல்லையெனில்
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    எனில் (dynamic_coords.length <= idx)
    {
      எனில் (unlikely (!dynamic_coords.resize (idx + 1)))
	திரும்பு Crap(முழு);
      length = idx + 1;
    }
    திரும்பு dynamic_coords.arrayZ[idx];
  }

  hb_array_t<முழு> get_coords ()
  { திரும்பு length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  தனிப்பட்ட:
  hb_vector_t<முழு> dynamic_coords;
  unsigned length;
  முழு static_coords[அளவு (வெறுமை *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
