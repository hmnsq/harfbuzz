#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#ضمن "../../../hb.hh"


نطاق OT {
//namespace Var {


بنية coord_setter_t
{
  coord_setter_t (hb_array_t<ثابت صحيح> coords_)
  {
    length = coords_.length;
    𐡄𐡍 (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * حجم (صحيح));
    وإلا
      dynamic_coords.extend (coords_);
  }

  صحيح& operator [] (unsigned idx)
  {
    𐡄𐡍 (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      أرجع Crap(صحيح);

    𐡄𐡍 (length <= ARRAY_LENGTH (static_coords))
    {
      𐡄𐡍 (idx < ARRAY_LENGTH (static_coords))
      {
        طالما (length <= idx)
	  static_coords[length++] = 0;
	أرجع static_coords[idx];
      }
      وإلا
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    𐡄𐡍 (dynamic_coords.length <= idx)
    {
      𐡄𐡍 (unlikely (!dynamic_coords.resize (idx + 1)))
	أرجع Crap(صحيح);
      length = idx + 1;
    }
    أرجع dynamic_coords.arrayZ[idx];
  }

  hb_array_t<صحيح> get_coords ()
  { أرجع length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  خاص:
  hb_vector_t<صحيح> dynamic_coords;
  unsigned length;
  صحيح static_coords[حجم (فراغ *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
