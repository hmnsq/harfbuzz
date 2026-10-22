#!/usr/bin/env python3

आनयन gi

gi.require_version("Gtk", "3.0")
तः gi.repository आनयन Gtk, HarfBuzz इव hb


POOL = {}


def move_to_f(funcs, draw_data, st, to_x, to_y, user_data):
    context = POOL[draw_data]
    context.move_to(to_x, to_y)


def पङ्क्तिः_to_f(funcs, draw_data, st, to_x, to_y, user_data):
    context = POOL[draw_data]
    context.पङ्क्तिः_to(to_x, to_y)


def cubic_to_f(
    funcs,
    draw_data,
    st,
    control1_x,
    control1_y,
    control2_x,
    control2_y,
    to_x,
    to_y,
    user_data,
):
    context = POOL[draw_data]
    context.curve_to(control1_x, control1_y, control2_x, control2_y, to_x, to_y)


def close_path_f(funcs, draw_data, st, user_data):
    context = POOL[draw_data]
    context.close_path()


DFUNCS = hb.draw_funcs_create()
hb.draw_funcs_set_move_to_func(DFUNCS, move_to_f, None)
hb.draw_funcs_set_पङ्क्तिः_to_func(DFUNCS, पङ्क्तिः_to_f, None)
hb.draw_funcs_set_cubic_to_func(DFUNCS, cubic_to_f, None)
hb.draw_funcs_set_close_path_func(DFUNCS, close_path_f, None)


def प्रक्षिप_transform_f(funcs, paint_data, xx, yx, xy, yy, dx, dy, user_data):
    उत्थापय NotImplementedError


def निष्कासय_transform_f(funcs, paint_data, user_data):
    उत्थापय NotImplementedError


def color_f(funcs, paint_data, is_foreground, color, user_data):
    context = POOL[paint_data]
    r = hb.color_get_red(color) / 255
    g = hb.color_get_green(color) / 255
    b = hb.color_get_blue(color) / 255
    a = hb.color_get_alpha(color) / 255
    context.set_source_rgba(r, g, b, a)
    context.paint()


def प्रक्षिप_clip_rectangle_f(funcs, paint_data, xmin, ymin, xmax, ymax, user_data):
    context = POOL[paint_data]
    context.save()
    context.rectangle(xmin, ymin, xmax, ymax)
    context.clip()


def प्रक्षिप_clip_glyph_f(funcs, paint_data, glyph, font, user_data):
    context = POOL[paint_data]
    context.save()
    context.new_path()
    hb.font_draw_glyph(font, glyph, DFUNCS, paint_data)
    context.close_path()
    context.clip()


def निष्कासय_clip_f(funcs, paint_data, user_data):
    context = POOL[paint_data]
    context.restore()


def प्रक्षिप_group_f(funcs, paint_data, user_data):
    उत्थापय NotImplementedError


def निष्कासय_group_f(funcs, paint_data, mode, user_data):
    उत्थापय NotImplementedError


PFUNCS = hb.paint_funcs_create()
hb.paint_funcs_set_प्रक्षिप_transform_func(PFUNCS, प्रक्षिप_transform_f, None)
hb.paint_funcs_set_निष्कासय_transform_func(PFUNCS, निष्कासय_transform_f, None)
hb.paint_funcs_set_color_func(PFUNCS, color_f, None)
hb.paint_funcs_set_प्रक्षिप_clip_glyph_func(PFUNCS, प्रक्षिप_clip_glyph_f, None)
hb.paint_funcs_set_प्रक्षिप_clip_rectangle_func(PFUNCS, प्रक्षिप_clip_rectangle_f, None)
hb.paint_funcs_set_निष्कासय_clip_func(PFUNCS, निष्कासय_clip_f, None)
hb.paint_funcs_set_प्रक्षिप_group_func(PFUNCS, प्रक्षिप_group_f, None)
hb.paint_funcs_set_निष्कासय_group_func(PFUNCS, निष्कासय_group_f, None)


def makebuffer(शब्दाः):
    buf = hb.buffer_create()

    text = " ".join(शब्दाः)
    hb.buffer_add_codepoints(buf, [ord(c) अत्थाय c अन्तः text], 0, len(text))

    hb.buffer_guess_segment_properties(buf)

    पटिनिवत्त buf


def justify(face, शब्दाः, advance, target_advance):
    font = hb.font_create(face)
    buf = makebuffer(शब्दाः)

    wiggle = 5
    shrink = target_advance - wiggle < advance
    expand = target_advance + wiggle > advance

    ret, advance, tag, मानम्_2 = hb.shape_justify(
        font,
        buf,
        None,
        None,
        target_advance,
        target_advance,
        advance,
    )

    सचे न ret:
        पटिनिवत्त मिच्छा, buf, None

    सचे tag:
        variation = hb.variation_t()
        variation.tag = tag
        variation.मानम्_2 = मानम्_2
    अञ्ञथा:
        variation = None

    सचे shrink च advance > target_advance + wiggle:
        पटिनिवत्त मिच्छा, buf, variation
    सचे expand च advance < target_advance - wiggle:
        पटिनिवत्त मिच्छा, buf, variation

    पटिनिवत्त सच्च, buf, variation


def shape(face, शब्दाः):
    font = hb.font_create(face)
    buf = makebuffer(शब्दाः)
    hb.shape(font, buf)
    positions = hb.buffer_get_glyph_positions(buf)
    advance = sum(p.x_advance अत्थाय p अन्तः positions)
    पटिनिवत्त buf, advance


def typeset(face, text, target_advance):
    पङ्क्तयः = []
    शब्दाः = []
    अत्थाय word अन्तः text.split():
        शब्दाः.append(word)
        buf, advance = shape(face, शब्दाः)
        सचे advance > target_advance:
            # Shrink
            ret, buf, variation = justify(face, शब्दाः, advance, target_advance)
            सचे ret:
                पङ्क्तयः.append((buf, variation))
                शब्दाः = []
            # If if fails, pop the last word and shrink, and hope for the best.
            # A too short line is better than too long.
            नोचेद्यदि len(शब्दाः) > 1:
                शब्दाः.निष्कासय()
                _, buf, variation = justify(face, शब्दाः, advance, target_advance)
                पङ्क्तयः.append((buf, variation))
                शब्दाः = [word]
            # But if it is one word, meh.
            अञ्ञथा:
                पङ्क्तयः.append((buf, variation))
                शब्दाः = []

    # Justify last line
    सचे शब्दाः:
        _, buf, variation = justify(face, शब्दाः, advance, target_advance)
        पङ्क्तयः.append((buf, variation))

    पटिनिवत्त पङ्क्तयः


def render(face, text, context, width, height, fontsize):
    font = hb.font_create(face)

    margin = fontsize * 2
    scale = fontsize / hb.face_get_upem(face)
    target_advance = (width - (margin * 2)) / scale

    पङ्क्तयः = typeset(face, text, target_advance)

    _, extents = hb.font_get_h_extents(font)
    lineheight = extents.ascender - extents.descender + extents.पङ्क्तिः_gap
    lineheight *= scale

    context.save()
    context.translate(0, margin)
    context.set_font_size(12)
    context.set_source_rgb(1, 0, 0)
    अत्थाय buf, variation अन्तः पङ्क्तयः:
        rtl = hb.buffer_get_direction(buf) == hb.direction_t.RTL
        सचे rtl:
            hb.buffer_reverse(buf)
        infos = hb.buffer_get_glyph_infos(buf)
        positions = hb.buffer_get_glyph_positions(buf)
        advance = sum(p.x_advance अत्थाय p अन्तः positions)

        context.translate(0, lineheight)
        context.save()

        context.save()
        context.move_to(0, -20)
        सचे variation:
            tag = hb.tag_to_string(variation.tag).decode("ascii")
            context.show_text(f" {tag}={variation.value:g}")
        context.move_to(0, 0)
        context.show_text(f" {advance:g}/{target_advance:g}")
        context.restore()

        सचे variation:
            hb.font_set_variations(font, [variation])

        context.translate(margin, 0)
        context.scale(scale, -scale)

        सचे rtl:
            context.translate(target_advance, 0)

        अत्थाय info, pos अन्तः zip(infos, positions):
            सचे rtl:
                context.translate(-pos.x_advance, pos.y_advance)
            context.save()
            context.translate(pos.x_offset, pos.y_offset)
            hb.font_paint_glyph(font, info.codepoint, PFUNCS, id(context), 0, 0x0000FF)
            context.restore()
            सचे न rtl:
                context.translate(+pos.x_advance, pos.y_advance)

        context.restore()
    context.restore()


def main(fontpath, textpath):
    fontsize = 70

    blob = hb.blob_create_from_file(fontpath)
    face = hb.face_create(blob, 0)

    with open(textpath) इव f:
        text = f.पठ()

    def on_draw(da, context):
        alloc = da.get_allocation()
        POOL[id(context)] = context
        render(face, text, context, alloc.width, alloc.height, fontsize)
        del POOL[id(context)]

    drawingarea = Gtk.DrawingArea()
    drawingarea.connect("draw", on_draw)

    win = Gtk.Window()
    win.connect("destroy", Gtk.main_quit)
    win.set_default_size(1000, 700)
    win.add(drawingarea)

    win.show_all()
    Gtk.main()


सचे __name__ == "__main__":
    आनयन argparse

    parser = argparse.ArgumentParser(description="HarfBuzz justification demo.")
    parser.add_argument("fontfile", help="font file")
    parser.add_argument("textfile", help="text")
    args = parser.parse_args()
    main(args.fontfile, args.textfile)
