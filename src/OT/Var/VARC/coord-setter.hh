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
    ܐܢ (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * حجم (صحيح));
    ܐܚܪܢܐ
      dynamic_coords.extend (coords_);
  }

  صحيح& operator [] (unsigned idx)
  {
    ܐܢ (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      ܦܢܐ Crap(صحيح);

    ܐܢ (length <= ARRAY_LENGTH (static_coords))
    {
      ܐܢ (idx < ARRAY_LENGTH (static_coords))
      {
        ܟܡܐ (length <= idx)
	  static_coords[length++] = 0;
	ܦܢܐ static_coords[idx];
      }
      ܐܚܪܢܐ
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    ܐܢ (dynamic_coords.length <= idx)
    {
      ܐܢ (unlikely (!dynamic_coords.resize (idx + 1)))
	ܦܢܐ Crap(صحيح);
      length = idx + 1;
    }
    ܦܢܐ dynamic_coords.arrayZ[idx];
  }

  hb_array_t<صحيح> get_coords ()
  { ܦܢܐ length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  خاص:
  hb_vector_t<صحيح> dynamic_coords;
  unsigned length;
  صحيح static_coords[حجم (فراغ *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
