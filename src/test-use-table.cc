#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-use-table.hh"

ਪੂਰਨ ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਜੇ (argc != 2)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ u = 0; u < 0x10FFFFu; u++)
      ਲਿਖੋ ("U+%04X %d\n", u, hb_use_get_category (u));
    ਵਾਪਸ 0;
  }

  hb_codepoint_t u;
  ਲੜੀ_ਤੋਂ_ਪੜ੍ਹੋ (argv[1], "%x", &u);

  ਲਿਖੋ ("%d\n", hb_use_get_category (u));

  ਵਾਪਸ 0;
}
