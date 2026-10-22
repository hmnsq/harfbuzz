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
    אם (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * حجم (صحيح));
    אחרת
      dynamic_coords.extend (coords_);
  }

  صحيح& operator [] (unsigned idx)
  {
    אם (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      החזר Crap(صحيح);

    אם (length <= ARRAY_LENGTH (static_coords))
    {
      אם (idx < ARRAY_LENGTH (static_coords))
      {
        כלעוד (length <= idx)
	  static_coords[length++] = 0;
	החזר static_coords[idx];
      }
      אחרת
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    אם (dynamic_coords.length <= idx)
    {
      אם (unlikely (!dynamic_coords.resize (idx + 1)))
	החזר Crap(صحيح);
      length = idx + 1;
    }
    החזר dynamic_coords.arrayZ[idx];
  }

  hb_array_t<صحيح> get_coords ()
  { החזר length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  خاص:
  hb_vector_t<صحيح> dynamic_coords;
  unsigned length;
  صحيح static_coords[حجم (فراغ *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
