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
    إذا (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * حجم (صحيح));
    وإلا
      dynamic_coords.extend (coords_);
  }

  صحيح& operator [] (unsigned idx)
  {
    إذا (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      إرجاع Crap(صحيح);

    إذا (length <= ARRAY_LENGTH (static_coords))
    {
      إذا (idx < ARRAY_LENGTH (static_coords))
      {
        بينما (length <= idx)
	  static_coords[length++] = 0;
	إرجاع static_coords[idx];
      }
      وإلا
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    إذا (dynamic_coords.length <= idx)
    {
      إذا (unlikely (!dynamic_coords.resize (idx + 1)))
	إرجاع Crap(صحيح);
      length = idx + 1;
    }
    إرجاع dynamic_coords.arrayZ[idx];
  }

  hb_array_t<صحيح> get_coords ()
  { إرجاع length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  خاص:
  hb_vector_t<صحيح> dynamic_coords;
  unsigned length;
  صحيح static_coords[حجم (فراغ *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
