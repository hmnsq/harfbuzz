#که_نه_تعریف_شوی OT_VAR_VARC_COORD_SETTER_HH
#تعریف_کړه OT_VAR_VARC_COORD_SETTER_HH


#ضمن "../../../hb.hh"


نطاق OT {
//namespace Var {


بنية coord_setter_t
{
  coord_setter_t (hb_array_t<ثابت صحيح> coords_)
  {
    اوږدتیا = coords_.اوږدتیا;
    إذا (اوږدتیا <= ARRAY_اوږدتیا (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, اوږدتیا * حجم (صحيح));
    وإلا
      dynamic_coords.extend (coords_);
  }

  صحيح& چلوونکی [] (بینښه idx)
  {
    إذا (unlikely (idx >= HB_VAR_مرکب_MAX_AXES))
      أرجع Crap(صحيح);

    إذا (اوږدتیا <= ARRAY_اوږدتیا (static_coords))
    {
      إذا (idx < ARRAY_اوږدتیا (static_coords))
      {
        طالما (اوږدتیا <= idx)
	  static_coords[اوږدتیا++] = 0;
	أرجع static_coords[idx];
      }
      وإلا
        dynamic_coords.extend (hb_array (static_coords, اوږدتیا));
    }

    إذا (dynamic_coords.اوږدتیا <= idx)
    {
      إذا (unlikely (!dynamic_coords.resize (idx + 1)))
	أرجع Crap(صحيح);
      اوږدتیا = idx + 1;
    }
    أرجع dynamic_coords.arrayZ[idx];
  }

  hb_array_t<صحيح> get_coords ()
  { أرجع اوږدتیا <= ARRAY_اوږدتیا (static_coords) ? hb_array (static_coords, اوږدتیا) : dynamic_coords.as_array (); }

  خاص:
  hb_ویکټور_t<صحيح> dynamic_coords;
  بینښه اوږدتیا;
  صحيح static_coords[حجم (فراغ *) * 8];
};


//} // namespace Var

} // namespace OT

#د_که_پای /* OT_VAR_VARC_COORD_SETTER_HH */
