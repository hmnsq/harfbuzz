#!/usr/bin/env python3

ਆਯਾਤ gi

gi.require_ਸੰਸਕਰਣ("Gtk", "3.0")
ਤਃ gi.repository ਆਯਾਤ Gtk, HarfBuzz ਇਵ hb


POOL = {}


ਪਰਿਭਾਸ਼ਾ move_ਮੰਜ਼ਿਲ_f(funcs, draw_ਅੰਕੜੇ, st, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y, user_ਅੰਕੜੇ):
    context = POOL[draw_ਅੰਕੜੇ]
    context.move_ਮੰਜ਼ਿਲ(ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y)


ਪਰਿਭਾਸ਼ਾ ਸਤਰ_ਮੰਜ਼ਿਲ_f(funcs, draw_ਅੰਕੜੇ, st, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y, user_ਅੰਕੜੇ):
    context = POOL[draw_ਅੰਕੜੇ]
    context.ਸਤਰ_ਮੰਜ਼ਿਲ(ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y)


ਪਰਿਭਾਸ਼ਾ cubic_ਮੰਜ਼ਿਲ_f(
    funcs,
    draw_ਅੰਕੜੇ,
    st,
    control1_x,
    control1_y,
    control2_x,
    control2_y,
    ਮੰਜ਼ਿਲ_x,
    ਮੰਜ਼ਿਲ_y,
    user_ਅੰਕੜੇ,
):
    context = POOL[draw_ਅੰਕੜੇ]
    context.ਵਕਰ_ਮੰਜ਼ਿਲ(control1_x, control1_y, control2_x, control2_y, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y)


ਪਰਿਭਾਸ਼ਾ close_ਰਸਤਾ_f(funcs, draw_ਅੰਕੜੇ, st, user_ਅੰਕੜੇ):
    context = POOL[draw_ਅੰਕੜੇ]
    context.close_ਰਸਤਾ()


DFUNCS = hb.draw_funcs_create()
hb.draw_funcs_ਸੈੱਟ_move_ਮੰਜ਼ਿਲ_func(DFUNCS, move_ਮੰਜ਼ਿਲ_f, ਕੁਝ_ਨਹੀਂ)
hb.draw_funcs_ਸੈੱਟ_ਸਤਰ_ਮੰਜ਼ਿਲ_func(DFUNCS, ਸਤਰ_ਮੰਜ਼ਿਲ_f, ਕੁਝ_ਨਹੀਂ)
hb.draw_funcs_ਸੈੱਟ_cubic_ਮੰਜ਼ਿਲ_func(DFUNCS, cubic_ਮੰਜ਼ਿਲ_f, ਕੁਝ_ਨਹੀਂ)
hb.draw_funcs_ਸੈੱਟ_close_ਰਸਤਾ_func(DFUNCS, close_ਰਸਤਾ_f, ਕੁਝ_ਨਹੀਂ)


ਪਰਿਭਾਸ਼ਾ ਧੱਕੋ_transform_f(funcs, paint_ਅੰਕੜੇ, xx, yx, xy, yy, dx, dy, user_ਅੰਕੜੇ):
    ਉਠਾਓ NotImplementedError


ਪਰਿਭਾਸ਼ਾ ਕੱਢੋ_transform_f(funcs, paint_ਅੰਕੜੇ, user_ਅੰਕੜੇ):
    ਉਠਾਓ NotImplementedError


ਪਰਿਭਾਸ਼ਾ color_f(funcs, paint_ਅੰਕੜੇ, is_foreground, color, user_ਅੰਕੜੇ):
    context = POOL[paint_ਅੰਕੜੇ]
    r = hb.color_get_red(color) / 255
    g = hb.color_get_green(color) / 255
    b = hb.color_get_blue(color) / 255
    a = hb.color_get_alpha(color) / 255
    context.ਸੈੱਟ_ਸਰੋਤ_rgba(r, g, b, a)
    context.paint()


ਪਰਿਭਾਸ਼ਾ ਧੱਕੋ_clip_rectangle_f(funcs, paint_ਅੰਕੜੇ, xmin, ymin, xmax, ymax, user_ਅੰਕੜੇ):
    context = POOL[paint_ਅੰਕੜੇ]
    context.save()
    context.rectangle(xmin, ymin, xmax, ymax)
    context.clip()


ਪਰਿਭਾਸ਼ਾ ਧੱਕੋ_clip_glyph_f(funcs, paint_ਅੰਕੜੇ, glyph, font, user_ਅੰਕੜੇ):
    context = POOL[paint_ਅੰਕੜੇ]
    context.save()
    context.new_ਰਸਤਾ()
    hb.font_draw_glyph(font, glyph, DFUNCS, paint_ਅੰਕੜੇ)
    context.close_ਰਸਤਾ()
    context.clip()


ਪਰਿਭਾਸ਼ਾ ਕੱਢੋ_clip_f(funcs, paint_ਅੰਕੜੇ, user_ਅੰਕੜੇ):
    context = POOL[paint_ਅੰਕੜੇ]
    context.restore()


ਪਰਿਭਾਸ਼ਾ ਧੱਕੋ_ਸਮੂਹ_f(funcs, paint_ਅੰਕੜੇ, user_ਅੰਕੜੇ):
    ਉਠਾਓ NotImplementedError


ਪਰਿਭਾਸ਼ਾ ਕੱਢੋ_ਸਮੂਹ_f(funcs, paint_ਅੰਕੜੇ, ਬਹੁਲਕ, user_ਅੰਕੜੇ):
    ਉਠਾਓ NotImplementedError


PFUNCS = hb.paint_funcs_create()
hb.paint_funcs_ਸੈੱਟ_ਧੱਕੋ_transform_func(PFUNCS, ਧੱਕੋ_transform_f, ਕੁਝ_ਨਹੀਂ)
hb.paint_funcs_ਸੈੱਟ_ਕੱਢੋ_transform_func(PFUNCS, ਕੱਢੋ_transform_f, ਕੁਝ_ਨਹੀਂ)
hb.paint_funcs_ਸੈੱਟ_color_func(PFUNCS, color_f, ਕੁਝ_ਨਹੀਂ)
hb.paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_glyph_func(PFUNCS, ਧੱਕੋ_clip_glyph_f, ਕੁਝ_ਨਹੀਂ)
hb.paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_rectangle_func(PFUNCS, ਧੱਕੋ_clip_rectangle_f, ਕੁਝ_ਨਹੀਂ)
hb.paint_funcs_ਸੈੱਟ_ਕੱਢੋ_clip_func(PFUNCS, ਕੱਢੋ_clip_f, ਕੁਝ_ਨਹੀਂ)
hb.paint_funcs_ਸੈੱਟ_ਧੱਕੋ_ਸਮੂਹ_func(PFUNCS, ਧੱਕੋ_ਸਮੂਹ_f, ਕੁਝ_ਨਹੀਂ)
hb.paint_funcs_ਸੈੱਟ_ਕੱਢੋ_ਸਮੂਹ_func(PFUNCS, ਕੱਢੋ_ਸਮੂਹ_f, ਕੁਝ_ਨਹੀਂ)


ਪਰਿਭਾਸ਼ਾ makebuffer(ਸ਼ਬਦ_2):
    buf = hb.ਬਫਰ_create()

    ਪਾਠ = " ".join(ਸ਼ਬਦ_2)
    hb.ਬਫਰ_ਜੋੜੋ_codepoints(buf, [ord(c) ਲਈ c ਵਿੱਚ ਪਾਠ], 0, ਲੰਬਾਈ(ਪਾਠ))

    hb.ਬਫਰ_guess_segment_properties(buf)

    ਵਾਪਸ buf


ਪਰਿਭਾਸ਼ਾ justify(face, ਸ਼ਬਦ_2, advance, ਨਿਸ਼ਾਨਾ_advance):
    font = hb.font_create(face)
    buf = makebuffer(ਸ਼ਬਦ_2)

    wiggle = 5
    shrink = ਨਿਸ਼ਾਨਾ_advance - wiggle < advance
    expand = ਨਿਸ਼ਾਨਾ_advance + wiggle > advance

    ret, advance, ਟੈਗ_2, ਮੁੱਲ_2 = hb.shape_justify(
        font,
        buf,
        ਕੁਝ_ਨਹੀਂ,
        ਕੁਝ_ਨਹੀਂ,
        ਨਿਸ਼ਾਨਾ_advance,
        ਨਿਸ਼ਾਨਾ_advance,
        advance,
    )

    ਜੇ ਨਹੀਂ ret:
        ਵਾਪਸ ਝੂਠ, buf, ਕੁਝ_ਨਹੀਂ

    ਜੇ ਟੈਗ_2:
        variation = hb.variation_t()
        variation.ਟੈਗ_2 = ਟੈਗ_2
        variation.ਮੁੱਲ_2 = ਮੁੱਲ_2
    ਨਹੀਂਤਾਂ:
        variation = ਕੁਝ_ਨਹੀਂ

    ਜੇ shrink ਅਤੇ advance > ਨਿਸ਼ਾਨਾ_advance + wiggle:
        ਵਾਪਸ ਝੂਠ, buf, variation
    ਜੇ expand ਅਤੇ advance < ਨਿਸ਼ਾਨਾ_advance - wiggle:
        ਵਾਪਸ ਝੂਠ, buf, variation

    ਵਾਪਸ ਸੱਚ, buf, variation


ਪਰਿਭਾਸ਼ਾ shape(face, ਸ਼ਬਦ_2):
    font = hb.font_create(face)
    buf = makebuffer(ਸ਼ਬਦ_2)
    hb.shape(font, buf)
    positions = hb.ਬਫਰ_get_glyph_positions(buf)
    advance = ਜੋੜ(p.x_advance ਲਈ p ਵਿੱਚ positions)
    ਵਾਪਸ buf, advance


ਪਰਿਭਾਸ਼ਾ typeset(face, ਪਾਠ, ਨਿਸ਼ਾਨਾ_advance):
    ਸਤਰਾਂ = []
    ਸ਼ਬਦ_2 = []
    ਲਈ ਸ਼ਬਦ_3 ਵਿੱਚ ਪਾਠ.ਵੰਡੋ():
        ਸ਼ਬਦ_2.append(ਸ਼ਬਦ_3)
        buf, advance = shape(face, ਸ਼ਬਦ_2)
        ਜੇ advance > ਨਿਸ਼ਾਨਾ_advance:
            # Shrink
            ret, buf, variation = justify(face, ਸ਼ਬਦ_2, advance, ਨਿਸ਼ਾਨਾ_advance)
            ਜੇ ret:
                ਸਤਰਾਂ.append((buf, variation))
                ਸ਼ਬਦ_2 = []
            # If if fails, pop the last word and shrink, and hope for the best.
            # A too short line is better than too long.
            ਜਾਂਜੇ ਲੰਬਾਈ(ਸ਼ਬਦ_2) > 1:
                ਸ਼ਬਦ_2.ਕੱਢੋ()
                _, buf, variation = justify(face, ਸ਼ਬਦ_2, advance, ਨਿਸ਼ਾਨਾ_advance)
                ਸਤਰਾਂ.append((buf, variation))
                ਸ਼ਬਦ_2 = [ਸ਼ਬਦ_3]
            # But if it is one word, meh.
            ਨਹੀਂਤਾਂ:
                ਸਤਰਾਂ.append((buf, variation))
                ਸ਼ਬਦ_2 = []

    # Justify last line
    ਜੇ ਸ਼ਬਦ_2:
        _, buf, variation = justify(face, ਸ਼ਬਦ_2, advance, ਨਿਸ਼ਾਨਾ_advance)
        ਸਤਰਾਂ.append((buf, variation))

    ਵਾਪਸ ਸਤਰਾਂ


ਪਰਿਭਾਸ਼ਾ render(face, ਪਾਠ, context, ਚੌੜਾਈ, ਉਚਾਈ, fontsize):
    font = hb.font_create(face)

    margin = fontsize * 2
    scale = fontsize / hb.face_get_upem(face)
    ਨਿਸ਼ਾਨਾ_advance = (ਚੌੜਾਈ - (margin * 2)) / scale

    ਸਤਰਾਂ = typeset(face, ਪਾਠ, ਨਿਸ਼ਾਨਾ_advance)

    _, extents = hb.font_get_h_extents(font)
    lineheight = extents.ascender - extents.descender + extents.ਸਤਰ_gap
    lineheight *= scale

    context.save()
    context.translate(0, margin)
    context.ਸੈੱਟ_font_ਮਾਪ(12)
    context.ਸੈੱਟ_ਸਰੋਤ_rgb(1, 0, 0)
    ਲਈ buf, variation ਵਿੱਚ ਸਤਰਾਂ:
        rtl = hb.ਬਫਰ_get_direction(buf) == hb.direction_t.RTL
        ਜੇ rtl:
            hb.ਬਫਰ_ਉਲਟ(buf)
        infos = hb.ਬਫਰ_get_glyph_infos(buf)
        positions = hb.ਬਫਰ_get_glyph_positions(buf)
        advance = ਜੋੜ(p.x_advance ਲਈ p ਵਿੱਚ positions)

        context.translate(0, lineheight)
        context.save()

        context.save()
        context.move_ਮੰਜ਼ਿਲ(0, -20)
        ਜੇ variation:
            ਟੈਗ_2 = hb.ਟੈਗ_ਮੰਜ਼ਿਲ_string(variation.ਟੈਗ_2).decode("ascii")
            context.show_ਪਾਠ(f" {tag}={variation.value:g}")
        context.move_ਮੰਜ਼ਿਲ(0, 0)
        context.show_ਪਾਠ(f" {advance:g}/{target_advance:g}")
        context.restore()

        ਜੇ variation:
            hb.font_ਸੈੱਟ_variations(font, [variation])

        context.translate(margin, 0)
        context.scale(scale, -scale)

        ਜੇ rtl:
            context.translate(ਨਿਸ਼ਾਨਾ_advance, 0)

        ਲਈ info, pos ਵਿੱਚ zip(infos, positions):
            ਜੇ rtl:
                context.translate(-pos.x_advance, pos.y_advance)
            context.save()
            context.translate(pos.x_ਵਿੱਥ_2, pos.y_ਵਿੱਥ_2)
            hb.font_paint_glyph(font, info.codepoint, PFUNCS, id(context), 0, 0x0000FF)
            context.restore()
            ਜੇ ਨਹੀਂ rtl:
                context.translate(+pos.x_advance, pos.y_advance)

        context.restore()
    context.restore()


ਪਰਿਭਾਸ਼ਾ ਮੁੱਖ(fontpath, textpath):
    fontsize = 70

    blob = hb.blob_create_from_file(fontpath)
    face = hb.face_create(blob, 0)

    ਨਾਲ open(textpath) ਇਵ f:
        ਪਾਠ = f.ਪਠ()

    ਪਰਿਭਾਸ਼ਾ on_draw(da, context):
        alloc = da.get_ਵੰਡ()
        POOL[id(context)] = context
        render(face, ਪਾਠ, context, alloc.ਚੌੜਾਈ, alloc.ਉਚਾਈ, fontsize)
        ਹਟਾਓ POOL[id(context)]

    drawingarea = Gtk.DrawingArea()
    drawingarea.connect("draw", on_draw)

    win = Gtk.ਖਿੜਕੀ()
    win.connect("destroy", Gtk.main_quit)
    win.ਸੈੱਟ_default_ਮਾਪ(1000, 700)
    win.ਜੋੜੋ(drawingarea)

    win.show_all()
    Gtk.ਮੁੱਖ()


ਜੇ __name__ == "__main__":
    ਆਯਾਤ argparse

    parser = argparse.ਦਲੀਲ_Parser(description="HarfBuzz justification demo.")
    parser.ਜੋੜੋ_ਦਲੀਲ("fontfile", help="font file")
    parser.ਜੋੜੋ_ਦਲੀਲ("textfile", help="text")
    ਦਲੀਲਾਂ = parser.parse_ਦਲੀਲਾਂ()
    ਮੁੱਖ(ਦਲੀਲਾਂ.fontfile, ਦਲੀਲਾਂ.textfile)
