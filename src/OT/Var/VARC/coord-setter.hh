#اگر_تعریف_نشده OT_VAR_VARC_COORD_SETTER_HH
#تعریف_کن OT_VAR_VARC_COORD_SETTER_HH


#ضمن "../../../hb.hh"


نطاق OT {
//namespace Var {


بنية coord_setter_t
{
  coord_setter_t (hb_array_t<ثابت صحيح> coords_)
  {
    درازی = coords_.درازی;
    إذا (درازی <= ARRAY_درازی (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, درازی * حجم (صحيح));
    وإلا
      dynamic_coords.extend (coords_);
  }

  صحيح& عملگر [] (بیعلامت idx)
  {
    إذا (unlikely (idx >= HB_VAR_مرکب_MAX_AXES))
      أرجع Crap(صحيح);

    إذا (درازی <= ARRAY_درازی (static_coords))
    {
      إذا (idx < ARRAY_درازی (static_coords))
      {
        طالما (درازی <= idx)
	  static_coords[درازی++] = 0;
	أرجع static_coords[idx];
      }
      وإلا
        dynamic_coords.extend (hb_array (static_coords, درازی));
    }

    إذا (dynamic_coords.درازی <= idx)
    {
      إذا (unlikely (!dynamic_coords.resize (idx + 1)))
	أرجع Crap(صحيح);
      درازی = idx + 1;
    }
    أرجع dynamic_coords.arrayZ[idx];
  }

  hb_array_t<صحيح> get_coords ()
  { أرجع درازی <= ARRAY_درازی (static_coords) ? hb_array (static_coords, درازی) : dynamic_coords.as_array (); }

  خاص:
  hb_وکتور_t<صحيح> dynamic_coords;
  بیعلامت درازی;
  صحيح static_coords[حجم (فراغ *) * 8];
};


//} // namespace Var

} // namespace OT

#پایان_اگر /* OT_VAR_VARC_COORD_SETTER_HH */
