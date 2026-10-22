#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_VAR_VARC_COORD_SETTER_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_VAR_VARC_COORD_SETTER_HH


#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb.hh"


ਨਾਮਾਕਾਸ਼ OT {
//namespace Var {


ਬਣਤਰ coord_setter_t
{
  coord_setter_t (hb_array_t<ਅਚਲ ਪੂਰਨ> coords_)
  {
    ਲੰਮਾਈ = coords_.ਲੰਮਾਈ;
    ਜੇ (ਲੰਮਾਈ <= ARRAY_ਲੰਮਾਈ (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, ਲੰਮਾਈ * ਆਕਾਰ (ਪੂਰਨ));
    ਨਹੀਂਤਾਂ
      dynamic_coords.extend (coords_);
  }

  ਪੂਰਨ& ਸੰਚਾਲਕ [] (ਬਿਨਾਨਿਸ਼ਾਨੀ idx)
  {
    ਜੇ (unlikely (idx >= HB_VAR_ਸੰਯੁਕਤ_MAX_AXES))
      ਵਾਪਸ Crap(ਪੂਰਨ);

    ਜੇ (ਲੰਮਾਈ <= ARRAY_ਲੰਮਾਈ (static_coords))
    {
      ਜੇ (idx < ARRAY_ਲੰਮਾਈ (static_coords))
      {
        ਜਦੋਂ (ਲੰਮਾਈ <= idx)
	  static_coords[ਲੰਮਾਈ++] = 0;
	ਵਾਪਸ static_coords[idx];
      }
      ਨਹੀਂਤਾਂ
        dynamic_coords.extend (hb_array (static_coords, ਲੰਮਾਈ));
    }

    ਜੇ (dynamic_coords.ਲੰਮਾਈ <= idx)
    {
      ਜੇ (unlikely (!dynamic_coords.resize (idx + 1)))
	ਵਾਪਸ Crap(ਪੂਰਨ);
      ਲੰਮਾਈ = idx + 1;
    }
    ਵਾਪਸ dynamic_coords.arrayZ[idx];
  }

  hb_array_t<ਪੂਰਨ> get_coords ()
  { ਵਾਪਸ ਲੰਮਾਈ <= ARRAY_ਲੰਮਾਈ (static_coords) ? hb_array (static_coords, ਲੰਮਾਈ) : dynamic_coords.as_array (); }

  ਨਿਜੀ:
  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> dynamic_coords;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ;
  ਪੂਰਨ static_coords[ਆਕਾਰ (ਖਾਲੀ *) * 8];
};


//} // namespace Var

} // namespace OT

#ਜੇ_ਅੰਤ /* OT_VAR_VARC_COORD_SETTER_HH */
