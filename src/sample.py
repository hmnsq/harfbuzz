#!/usr/bin/env python3

దిగుమతి sys
దిగుమతి array
దిగుమతి gi
gi.require_సంస్కరణ('HarfBuzz', '0.0')
తః gi.repository దిగుమతి HarfBuzz ఇవ hb
తః gi.repository దిగుమతి GLib

fontdata = open (sys.argv[1], 'rb').పఠ ()
పాఠ్యం = sys.argv[2]
# Need to create GLib.Bytes explicitly until this bug is fixed:
# https://bugzilla.gnome.org/show_bug.cgi?id=729541
blob = hb.glib_blob_create (GLib.బైట్లు_3.కొత్త (fontdata))
face = hb.face_create (blob, 0)
తీసివేయి blob
font = hb.font_create (face)
upem = hb.face_get_upem (face)
తీసివేయి face
hb.font_సమితి_scale (font, upem, upem)
#hb.ft_font_set_funcs (font)
hb.ot_font_సమితి_funcs (font)

buf = hb.బఫరు_create ()
తరగతి Debugger (object):
	నిర్వచనం సందేశం (self, buf, font, సందేశం_2, దత్తాంశం, _x_what_is_this):
		ముద్రించు (సందేశం_2)
		తిరిగి నిజం
debugger = Debugger ()
hb.బఫరు_సమితి_సందేశం_func (buf, debugger.సందేశం, 1, 0)

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
ఒకవేళ sys.maxunicode == 0x10FFFF:
	hb.బఫరు_కూడు_utf32 (buf, array.array ('I', పాఠ్యం.encode ('utf-32'))[1:], 0, -1)
లేకపోతే:
	hb.బఫరు_కూడు_utf16 (buf, array.array ('H', పాఠ్యం.encode ('utf-16'))[1:], 0, -1)


hb.బఫరు_guess_segment_properties (buf)

hb.shape (font, buf, [])
తీసివేయి font

infos = hb.బఫరు_get_glyph_infos (buf)
positions = hb.బఫరు_get_glyph_positions (buf)

కొరకు info, pos లో zip (infos, positions):
	gid = info.codepoint
	cluster = info.cluster
	x_advance = pos.x_advance
	x_స్థానభ్రంశం_2 = pos.x_స్థానభ్రంశం_2
	y_స్థానభ్రంశం_2 = pos.y_స్థానభ్రంశం_2

	ముద్రించు ("gid%d=%d@%d,%d+%d" % (gid, cluster, x_advance, x_స్థానభ్రంశం_2, y_స్థానభ్రంశం_2))
