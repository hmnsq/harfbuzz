#!/usr/bin/env python3

ਆਯਾਤ sys
ਆਯਾਤ array
ਆਯਾਤ gi
gi.require_ਸੰਸਕਰਣ('HarfBuzz', '0.0')
ਤਃ gi.repository ਆਯਾਤ HarfBuzz ਇਵ hb
ਤਃ gi.repository ਆਯਾਤ GLib

fontdata = open (sys.argv[1], 'rb').ਪਠ ()
ਪਾਠ = sys.argv[2]
# Need to create GLib.Bytes explicitly until this bug is fixed:
# https://bugzilla.gnome.org/show_bug.cgi?id=729541
blob = hb.glib_blob_create (GLib.ਬਾਈਟਾਂ_3.ਨਵਾਂ (fontdata))
face = hb.face_create (blob, 0)
ਹਟਾਓ blob
font = hb.font_create (face)
upem = hb.face_get_upem (face)
ਹਟਾਓ face
hb.font_ਸੈੱਟ_scale (font, upem, upem)
#hb.ft_font_set_funcs (font)
hb.ot_font_ਸੈੱਟ_funcs (font)

buf = hb.ਬਫਰ_create ()
ਵਰਗ Debugger (object):
	ਪਰਿਭਾਸ਼ਾ ਸੁਨੇਹਾ (self, buf, font, ਸੁਨੇਹਾ_2, ਅੰਕੜੇ, _x_what_is_this):
		ਛਾਪੋ (ਸੁਨੇਹਾ_2)
		ਵਾਪਸ ਸੱਚ
debugger = Debugger ()
hb.ਬਫਰ_ਸੈੱਟ_ਸੁਨੇਹਾ_func (buf, debugger.ਸੁਨੇਹਾ, 1, 0)

##
## Add text to buffer
##
#
# See https://github.com/harfbuzz/harfbuzz/pull/271
#
# If you do not care about cluster values reflecting Python
# string indices, then this is quickest way to add text to
# buffer:
# hb.buffer_add_utf8 (buf, text.encode('utf-8'), 0, -1)
# Otherwise, then following handles both narrow and wide
# Python builds (the first item in the array is BOM, so we skip it):
ਜੇ sys.maxunicode == 0x10FFFF:
	hb.ਬਫਰ_ਜੋੜੋ_utf32 (buf, array.array ('I', ਪਾਠ.encode ('utf-32'))[1:], 0, -1)
ਨਹੀਂਤਾਂ:
	hb.ਬਫਰ_ਜੋੜੋ_utf16 (buf, array.array ('H', ਪਾਠ.encode ('utf-16'))[1:], 0, -1)


hb.ਬਫਰ_guess_segment_properties (buf)

hb.shape (font, buf, [])
ਹਟਾਓ font

infos = hb.ਬਫਰ_get_glyph_infos (buf)
positions = hb.ਬਫਰ_get_glyph_positions (buf)

ਲਈ info, pos ਵਿੱਚ zip (infos, positions):
	gid = info.codepoint
	cluster = info.cluster
	x_advance = pos.x_advance
	x_ਵਿੱਥ_2 = pos.x_ਵਿੱਥ_2
	y_ਵਿੱਥ_2 = pos.y_ਵਿੱਥ_2

	ਛਾਪੋ ("gid%d=%d@%d,%d+%d" % (gid, cluster, x_advance, x_ਵਿੱਥ_2, y_ਵਿੱਥ_2))
