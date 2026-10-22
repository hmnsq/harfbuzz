#!/usr/bin/env python3

ଆମଦାନୀ gi

gi.require_version("Gtk", "3.0")
ତଃ gi.repository ଆମଦାନୀ Gtk, HarfBuzz ଇଵ hb


POOL = {}


def move_to_f(funcs, draw_data, st, to_x, to_y, user_data):
    context = POOL[draw_data]
    context.move_to(to_x, to_y)


def ଧାଡ଼ି_to_f(funcs, draw_data, st, to_x, to_y, user_data):
    context = POOL[draw_data]
    context.ଧାଡ଼ି_to(to_x, to_y)


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
hb.draw_funcs_set_ଧାଡ଼ି_to_func(DFUNCS, ଧାଡ଼ି_to_f, None)
hb.draw_funcs_set_cubic_to_func(DFUNCS, cubic_to_f, None)
hb.draw_funcs_set_close_path_func(DFUNCS, close_path_f, None)


def ଠେଲ_transform_f(funcs, paint_data, xx, yx, xy, yy, dx, dy, user_data):
    ଉଠାଅ NotImplementedError


def କାଢ଼_transform_f(funcs, paint_data, user_data):
    ଉଠାଅ NotImplementedError


def color_f(funcs, paint_data, is_foreground, color, user_data):
    context = POOL[paint_data]
    r = hb.color_get_red(color) / 255
    g = hb.color_get_green(color) / 255
    b = hb.color_get_blue(color) / 255
    a = hb.color_get_alpha(color) / 255
    context.set_source_rgba(r, g, b, a)
    context.paint()


def ଠେଲ_clip_rectangle_f(funcs, paint_data, xmin, ymin, xmax, ymax, user_data):
    context = POOL[paint_data]
    context.save()
    context.rectangle(xmin, ymin, xmax, ymax)
    context.clip()


def ଠେଲ_clip_glyph_f(funcs, paint_data, glyph, font, user_data):
    context = POOL[paint_data]
    context.save()
    context.new_path()
    hb.font_draw_glyph(font, glyph, DFUNCS, paint_data)
    context.close_path()
    context.clip()


def କାଢ଼_clip_f(funcs, paint_data, user_data):
    context = POOL[paint_data]
    context.restore()


def ଠେଲ_group_f(funcs, paint_data, user_data):
    ଉଠାଅ NotImplementedError


def କାଢ଼_group_f(funcs, paint_data, mode, user_data):
    ଉଠାଅ NotImplementedError


PFUNCS = hb.paint_funcs_create()
hb.paint_funcs_set_ଠେଲ_transform_func(PFUNCS, ଠେଲ_transform_f, None)
hb.paint_funcs_set_କାଢ଼_transform_func(PFUNCS, କାଢ଼_transform_f, None)
hb.paint_funcs_set_color_func(PFUNCS, color_f, None)
hb.paint_funcs_set_ଠେଲ_clip_glyph_func(PFUNCS, ଠେଲ_clip_glyph_f, None)
hb.paint_funcs_set_ଠେଲ_clip_rectangle_func(PFUNCS, ଠେଲ_clip_rectangle_f, None)
hb.paint_funcs_set_କାଢ଼_clip_func(PFUNCS, କାଢ଼_clip_f, None)
hb.paint_funcs_set_ଠେଲ_group_func(PFUNCS, ଠେଲ_group_f, None)
hb.paint_funcs_set_କାଢ଼_group_func(PFUNCS, କାଢ଼_group_f, None)


def makebuffer(ଶବ୍ଦଗୁଡ଼ିକ):
    buf = hb.buffer_create()

    text = " ".join(ଶବ୍ଦଗୁଡ଼ିକ)
    hb.buffer_add_codepoints(buf, [ord(c) ପାଇଁ c ଭିତରେ text], 0, len(text))

    hb.buffer_guess_segment_properties(buf)

    ଫେରନ୍ତୁ buf


def justify(face, ଶବ୍ଦଗୁଡ଼ିକ, advance, target_advance):
    font = hb.font_create(face)
    buf = makebuffer(ଶବ୍ଦଗୁଡ଼ିକ)

    wiggle = 5
    shrink = target_advance - wiggle < advance
    expand = target_advance + wiggle > advance

    ret, advance, tag, ମୂଲ୍ୟ_2 = hb.shape_justify(
        font,
        buf,
        None,
        None,
        target_advance,
        target_advance,
        advance,
    )

    ଯଦି ନୁହେଁ ret:
        ଫେରନ୍ତୁ ମିଥ୍ୟା, buf, None

    ଯଦି tag:
        variation = hb.variation_t()
        variation.tag = tag
        variation.ମୂଲ୍ୟ_2 = ମୂଲ୍ୟ_2
    ଅନ୍ୟଥା:
        variation = None

    ଯଦି shrink ଏବଂ advance > target_advance + wiggle:
        ଫେରନ୍ତୁ ମିଥ୍ୟା, buf, variation
    ଯଦି expand ଏବଂ advance < target_advance - wiggle:
        ଫେରନ୍ତୁ ମିଥ୍ୟା, buf, variation

    ଫେରନ୍ତୁ ସତ୍ୟ, buf, variation


def shape(face, ଶବ୍ଦଗୁଡ଼ିକ):
    font = hb.font_create(face)
    buf = makebuffer(ଶବ୍ଦଗୁଡ଼ିକ)
    hb.shape(font, buf)
    positions = hb.buffer_get_glyph_positions(buf)
    advance = sum(p.x_advance ପାଇଁ p ଭିତରେ positions)
    ଫେରନ୍ତୁ buf, advance


def typeset(face, text, target_advance):
    ଧାଡ଼ିଗୁଡ଼ିକ = []
    ଶବ୍ଦଗୁଡ଼ିକ = []
    ପାଇଁ word ଭିତରେ text.split():
        ଶବ୍ଦଗୁଡ଼ିକ.append(word)
        buf, advance = shape(face, ଶବ୍ଦଗୁଡ଼ିକ)
        ଯଦି advance > target_advance:
            # Shrink
            ret, buf, variation = justify(face, ଶବ୍ଦଗୁଡ଼ିକ, advance, target_advance)
            ଯଦି ret:
                ଧାଡ଼ିଗୁଡ଼ିକ.append((buf, variation))
                ଶବ୍ଦଗୁଡ଼ିକ = []
            # If if fails, pop the last word and shrink, and hope for the best.
            # A too short line is better than too long.
            ନଚେତଯଦି len(ଶବ୍ଦଗୁଡ଼ିକ) > 1:
                ଶବ୍ଦଗୁଡ଼ିକ.କାଢ଼()
                _, buf, variation = justify(face, ଶବ୍ଦଗୁଡ଼ିକ, advance, target_advance)
                ଧାଡ଼ିଗୁଡ଼ିକ.append((buf, variation))
                ଶବ୍ଦଗୁଡ଼ିକ = [word]
            # But if it is one word, meh.
            ଅନ୍ୟଥା:
                ଧାଡ଼ିଗୁଡ଼ିକ.append((buf, variation))
                ଶବ୍ଦଗୁଡ଼ିକ = []

    # Justify last line
    ଯଦି ଶବ୍ଦଗୁଡ଼ିକ:
        _, buf, variation = justify(face, ଶବ୍ଦଗୁଡ଼ିକ, advance, target_advance)
        ଧାଡ଼ିଗୁଡ଼ିକ.append((buf, variation))

    ଫେରନ୍ତୁ ଧାଡ଼ିଗୁଡ଼ିକ


def render(face, text, context, width, height, fontsize):
    font = hb.font_create(face)

    margin = fontsize * 2
    scale = fontsize / hb.face_get_upem(face)
    target_advance = (width - (margin * 2)) / scale

    ଧାଡ଼ିଗୁଡ଼ିକ = typeset(face, text, target_advance)

    _, extents = hb.font_get_h_extents(font)
    lineheight = extents.ascender - extents.descender + extents.ଧାଡ଼ି_gap
    lineheight *= scale

    context.save()
    context.translate(0, margin)
    context.set_font_size(12)
    context.set_source_rgb(1, 0, 0)
    ପାଇଁ buf, variation ଭିତରେ ଧାଡ଼ିଗୁଡ଼ିକ:
        rtl = hb.buffer_get_direction(buf) == hb.direction_t.RTL
        ଯଦି rtl:
            hb.buffer_reverse(buf)
        infos = hb.buffer_get_glyph_infos(buf)
        positions = hb.buffer_get_glyph_positions(buf)
        advance = sum(p.x_advance ପାଇଁ p ଭିତରେ positions)

        context.translate(0, lineheight)
        context.save()

        context.save()
        context.move_to(0, -20)
        ଯଦି variation:
            tag = hb.tag_to_string(variation.tag).decode("ascii")
            context.show_text(f" {tag}={variation.value:g}")
        context.move_to(0, 0)
        context.show_text(f" {advance:g}/{target_advance:g}")
        context.restore()

        ଯଦି variation:
            hb.font_set_variations(font, [variation])

        context.translate(margin, 0)
        context.scale(scale, -scale)

        ଯଦି rtl:
            context.translate(target_advance, 0)

        ପାଇଁ info, pos ଭିତରେ zip(infos, positions):
            ଯଦି rtl:
                context.translate(-pos.x_advance, pos.y_advance)
            context.save()
            context.translate(pos.x_offset, pos.y_offset)
            hb.font_paint_glyph(font, info.codepoint, PFUNCS, id(context), 0, 0x0000FF)
            context.restore()
            ଯଦି ନୁହେଁ rtl:
                context.translate(+pos.x_advance, pos.y_advance)

        context.restore()
    context.restore()


def ମୁଖ୍ୟ(fontpath, textpath):
    fontsize = 70

    blob = hb.blob_create_from_file(fontpath)
    face = hb.face_create(blob, 0)

    with open(textpath) ଇଵ f:
        text = f.ପଠ()

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
    Gtk.ମୁଖ୍ୟ()


ଯଦି __name__ == "__main__":
    ଆମଦାନୀ argparse

    parser = argparse.ArgumentParser(description="HarfBuzz justification demo.")
    parser.add_argument("fontfile", help="font file")
    parser.add_argument("textfile", help="text")
    args = parser.parse_args()
    ମୁଖ୍ୟ(args.fontfile, args.textfile)
