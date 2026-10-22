#!/usr/bin/env python3

దిగుమతి gi

gi.require_సంస్కరణ("Gtk", "3.0")
తః gi.repository దిగుమతి Gtk, HarfBuzz ఇవ hb


POOL = {}


నిర్వచనం move_గమ్యం_f(funcs, draw_దత్తాంశం, st, గమ్యం_x, గమ్యం_y, user_దత్తాంశం):
    context = POOL[draw_దత్తాంశం]
    context.move_గమ్యం(గమ్యం_x, గమ్యం_y)


నిర్వచనం పంక్తి_గమ్యం_f(funcs, draw_దత్తాంశం, st, గమ్యం_x, గమ్యం_y, user_దత్తాంశం):
    context = POOL[draw_దత్తాంశం]
    context.పంక్తి_గమ్యం(గమ్యం_x, గమ్యం_y)


నిర్వచనం cubic_గమ్యం_f(
    funcs,
    draw_దత్తాంశం,
    st,
    control1_x,
    control1_y,
    control2_x,
    control2_y,
    గమ్యం_x,
    గమ్యం_y,
    user_దత్తాంశం,
):
    context = POOL[draw_దత్తాంశం]
    context.వక్రం_గమ్యం(control1_x, control1_y, control2_x, control2_y, గమ్యం_x, గమ్యం_y)


నిర్వచనం close_మార్గం_f(funcs, draw_దత్తాంశం, st, user_దత్తాంశం):
    context = POOL[draw_దత్తాంశం]
    context.close_మార్గం()


DFUNCS = hb.draw_funcs_create()
hb.draw_funcs_సమితి_move_గమ్యం_func(DFUNCS, move_గమ్యం_f, ఏమీలేదు)
hb.draw_funcs_సమితి_పంక్తి_గమ్యం_func(DFUNCS, పంక్తి_గమ్యం_f, ఏమీలేదు)
hb.draw_funcs_సమితి_cubic_గమ్యం_func(DFUNCS, cubic_గమ్యం_f, ఏమీలేదు)
hb.draw_funcs_సమితి_close_మార్గం_func(DFUNCS, close_మార్గం_f, ఏమీలేదు)


నిర్వచనం నెట్టు_transform_f(funcs, paint_దత్తాంశం, xx, yx, xy, yy, dx, dy, user_దత్తాంశం):
    లేపు NotImplementedError


నిర్వచనం తీయి_transform_f(funcs, paint_దత్తాంశం, user_దత్తాంశం):
    లేపు NotImplementedError


నిర్వచనం color_f(funcs, paint_దత్తాంశం, is_foreground, color, user_దత్తాంశం):
    context = POOL[paint_దత్తాంశం]
    r = hb.color_get_red(color) / 255
    g = hb.color_get_green(color) / 255
    b = hb.color_get_blue(color) / 255
    a = hb.color_get_alpha(color) / 255
    context.సమితి_ఆధారం_rgba(r, g, b, a)
    context.paint()


నిర్వచనం నెట్టు_clip_rectangle_f(funcs, paint_దత్తాంశం, xmin, ymin, xmax, ymax, user_దత్తాంశం):
    context = POOL[paint_దత్తాంశం]
    context.save()
    context.rectangle(xmin, ymin, xmax, ymax)
    context.clip()


నిర్వచనం నెట్టు_clip_glyph_f(funcs, paint_దత్తాంశం, glyph, font, user_దత్తాంశం):
    context = POOL[paint_దత్తాంశం]
    context.save()
    context.new_మార్గం()
    hb.font_draw_glyph(font, glyph, DFUNCS, paint_దత్తాంశం)
    context.close_మార్గం()
    context.clip()


నిర్వచనం తీయి_clip_f(funcs, paint_దత్తాంశం, user_దత్తాంశం):
    context = POOL[paint_దత్తాంశం]
    context.restore()


నిర్వచనం నెట్టు_సమూహం_f(funcs, paint_దత్తాంశం, user_దత్తాంశం):
    లేపు NotImplementedError


నిర్వచనం తీయి_సమూహం_f(funcs, paint_దత్తాంశం, బహుళకం, user_దత్తాంశం):
    లేపు NotImplementedError


PFUNCS = hb.paint_funcs_create()
hb.paint_funcs_సమితి_నెట్టు_transform_func(PFUNCS, నెట్టు_transform_f, ఏమీలేదు)
hb.paint_funcs_సమితి_తీయి_transform_func(PFUNCS, తీయి_transform_f, ఏమీలేదు)
hb.paint_funcs_సమితి_color_func(PFUNCS, color_f, ఏమీలేదు)
hb.paint_funcs_సమితి_నెట్టు_clip_glyph_func(PFUNCS, నెట్టు_clip_glyph_f, ఏమీలేదు)
hb.paint_funcs_సమితి_నెట్టు_clip_rectangle_func(PFUNCS, నెట్టు_clip_rectangle_f, ఏమీలేదు)
hb.paint_funcs_సమితి_తీయి_clip_func(PFUNCS, తీయి_clip_f, ఏమీలేదు)
hb.paint_funcs_సమితి_నెట్టు_సమూహం_func(PFUNCS, నెట్టు_సమూహం_f, ఏమీలేదు)
hb.paint_funcs_సమితి_తీయి_సమూహం_func(PFUNCS, తీయి_సమూహం_f, ఏమీలేదు)


నిర్వచనం makebuffer(పదాలు):
    buf = hb.బఫరు_create()

    పాఠ్యం = " ".join(పదాలు)
    hb.బఫరు_కూడు_codepoints(buf, [ord(c) కొరకు c లో పాఠ్యం], 0, పొడవు(పాఠ్యం))

    hb.బఫరు_guess_segment_properties(buf)

    తిరిగి buf


నిర్వచనం justify(face, పదాలు, advance, లక్ష్యం_advance):
    font = hb.font_create(face)
    buf = makebuffer(పదాలు)

    wiggle = 5
    shrink = లక్ష్యం_advance - wiggle < advance
    expand = లక్ష్యం_advance + wiggle > advance

    ret, advance, ట్యాగ్_2, విలువ_2 = hb.shape_justify(
        font,
        buf,
        ఏమీలేదు,
        ఏమీలేదు,
        లక్ష్యం_advance,
        లక్ష్యం_advance,
        advance,
    )

    ఒకవేళ కాదు ret:
        తిరిగి అబద్ధం, buf, ఏమీలేదు

    ఒకవేళ ట్యాగ్_2:
        variation = hb.variation_t()
        variation.ట్యాగ్_2 = ట్యాగ్_2
        variation.విలువ_2 = విలువ_2
    లేకపోతే:
        variation = ఏమీలేదు

    ఒకవేళ shrink మరియు advance > లక్ష్యం_advance + wiggle:
        తిరిగి అబద్ధం, buf, variation
    ఒకవేళ expand మరియు advance < లక్ష్యం_advance - wiggle:
        తిరిగి అబద్ధం, buf, variation

    తిరిగి నిజం, buf, variation


నిర్వచనం shape(face, పదాలు):
    font = hb.font_create(face)
    buf = makebuffer(పదాలు)
    hb.shape(font, buf)
    positions = hb.బఫరు_get_glyph_positions(buf)
    advance = కూడిక(p.x_advance కొరకు p లో positions)
    తిరిగి buf, advance


నిర్వచనం typeset(face, పాఠ్యం, లక్ష్యం_advance):
    పంక్తులు = []
    పదాలు = []
    కొరకు మాట_2 లో పాఠ్యం.విభజించు():
        పదాలు.append(మాట_2)
        buf, advance = shape(face, పదాలు)
        ఒకవేళ advance > లక్ష్యం_advance:
            # Shrink
            ret, buf, variation = justify(face, పదాలు, advance, లక్ష్యం_advance)
            ఒకవేళ ret:
                పంక్తులు.append((buf, variation))
                పదాలు = []
            # If if fails, pop the last word and shrink, and hope for the best.
            # A too short line is better than too long.
            లేదావేళ పొడవు(పదాలు) > 1:
                పదాలు.తీయి()
                _, buf, variation = justify(face, పదాలు, advance, లక్ష్యం_advance)
                పంక్తులు.append((buf, variation))
                పదాలు = [మాట_2]
            # But if it is one word, meh.
            లేకపోతే:
                పంక్తులు.append((buf, variation))
                పదాలు = []

    # Justify last line
    ఒకవేళ పదాలు:
        _, buf, variation = justify(face, పదాలు, advance, లక్ష్యం_advance)
        పంక్తులు.append((buf, variation))

    తిరిగి పంక్తులు


నిర్వచనం render(face, పాఠ్యం, context, వెడల్పు, ఎత్తు, fontsize):
    font = hb.font_create(face)

    margin = fontsize * 2
    scale = fontsize / hb.face_get_upem(face)
    లక్ష్యం_advance = (వెడల్పు - (margin * 2)) / scale

    పంక్తులు = typeset(face, పాఠ్యం, లక్ష్యం_advance)

    _, extents = hb.font_get_h_extents(font)
    lineheight = extents.ascender - extents.descender + extents.పంక్తి_gap
    lineheight *= scale

    context.save()
    context.translate(0, margin)
    context.సమితి_font_కొలత(12)
    context.సమితి_ఆధారం_rgb(1, 0, 0)
    కొరకు buf, variation లో పంక్తులు:
        rtl = hb.బఫరు_get_direction(buf) == hb.direction_t.RTL
        ఒకవేళ rtl:
            hb.బఫరు_తిరగేయి(buf)
        infos = hb.బఫరు_get_glyph_infos(buf)
        positions = hb.బఫరు_get_glyph_positions(buf)
        advance = కూడిక(p.x_advance కొరకు p లో positions)

        context.translate(0, lineheight)
        context.save()

        context.save()
        context.move_గమ్యం(0, -20)
        ఒకవేళ variation:
            ట్యాగ్_2 = hb.ట్యాగ్_గమ్యం_string(variation.ట్యాగ్_2).decode("ascii")
            context.show_పాఠ్యం(f" {tag}={variation.value:g}")
        context.move_గమ్యం(0, 0)
        context.show_పాఠ్యం(f" {advance:g}/{target_advance:g}")
        context.restore()

        ఒకవేళ variation:
            hb.font_సమితి_variations(font, [variation])

        context.translate(margin, 0)
        context.scale(scale, -scale)

        ఒకవేళ rtl:
            context.translate(లక్ష్యం_advance, 0)

        కొరకు info, pos లో zip(infos, positions):
            ఒకవేళ rtl:
                context.translate(-pos.x_advance, pos.y_advance)
            context.save()
            context.translate(pos.x_స్థానభ్రంశం_2, pos.y_స్థానభ్రంశం_2)
            hb.font_paint_glyph(font, info.codepoint, PFUNCS, id(context), 0, 0x0000FF)
            context.restore()
            ఒకవేళ కాదు rtl:
                context.translate(+pos.x_advance, pos.y_advance)

        context.restore()
    context.restore()


నిర్వచనం ప్రధాన(fontpath, textpath):
    fontsize = 70

    blob = hb.blob_create_from_file(fontpath)
    face = hb.face_create(blob, 0)

    తోడుగా open(textpath) ఇవ f:
        పాఠ్యం = f.పఠ()

    నిర్వచనం on_draw(da, context):
        alloc = da.get_కేటాయింపు()
        POOL[id(context)] = context
        render(face, పాఠ్యం, context, alloc.వెడల్పు, alloc.ఎత్తు, fontsize)
        తీసివేయి POOL[id(context)]

    drawingarea = Gtk.DrawingArea()
    drawingarea.connect("draw", on_draw)

    win = Gtk.కిటికీ()
    win.connect("destroy", Gtk.main_quit)
    win.సమితి_default_కొలత(1000, 700)
    win.కూడు(drawingarea)

    win.show_all()
    Gtk.ప్రధాన()


ఒకవేళ __name__ == "__main__":
    దిగుమతి argparse

    parser = argparse.వాదం_Parser(description="HarfBuzz justification demo.")
    parser.కూడు_వాదం("fontfile", help="font file")
    parser.కూడు_వాదం("textfile", help="text")
    వాదనలు = parser.parse_వాదనలు()
    ప్రధాన(వాదనలు.fontfile, వాదనలు.textfile)
