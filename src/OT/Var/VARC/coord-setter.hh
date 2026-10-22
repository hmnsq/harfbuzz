#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#include "../../../hb.hh"


namespace OT {
//namespace Var {


sa_nracanaa coord_setter_t
{
  coord_setter_t (hb_array_t<const ຈຳນວນເຕັມ> coords_)
  {
    length = coords_.length;
    ຖ້າ (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * parimaa_na (ຈຳນວນເຕັມ));
    ບໍ່ດັ່ງນັ້ນ
      dynamic_coords.extend (coords_);
  }

  ຈຳນວນເຕັມ& operator [] (unsigned idx)
  {
    ຖ້າ (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      ສົ່ງຄືນ Crap(ຈຳນວນເຕັມ);

    ຖ້າ (length <= ARRAY_LENGTH (static_coords))
    {
      ຖ້າ (idx < ARRAY_LENGTH (static_coords))
      {
        ໃນຂະນະທີ່ (length <= idx)
	  static_coords[length++] = 0;
	ສົ່ງຄືນ static_coords[idx];
      }
      ບໍ່ດັ່ງນັ້ນ
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    ຖ້າ (dynamic_coords.length <= idx)
    {
      ຖ້າ (unlikely (!dynamic_coords.resize (idx + 1)))
	ສົ່ງຄືນ Crap(ຈຳນວນເຕັມ);
      length = idx + 1;
    }
    ສົ່ງຄືນ dynamic_coords.arrayZ[idx];
  }

  hb_array_t<ຈຳນວນເຕັມ> get_coords ()
  { ສົ່ງຄືນ length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  private:
  hb_vector_t<ຈຳນວນເຕັມ> dynamic_coords;
  unsigned length;
  ຈຳນວນເຕັມ static_coords[parimaa_na (void *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
