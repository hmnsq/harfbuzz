#!/usr/bin/env python3

ഇറക്കുമതി gi

gi.require_version("Gtk", "3.0")
തഃ gi.repository ഇറക്കുമതി Gtk, HarfBuzz ഇവ hb


POOL = {}


def move_to_f(funcs, draw_data, st, to_x, to_y, user_data):
    context = POOL[draw_data]
    context.move_to(to_x, to_y)


def വരി_to_f(funcs, draw_data, st, to_x, to_y, user_data):
    context = POOL[draw_data]
    context.വരി_to(to_x, to_y)


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
hb.draw_funcs_set_വരി_to_func(DFUNCS, വരി_to_f, None)
hb.draw_funcs_set_cubic_to_func(DFUNCS, cubic_to_f, None)
hb.draw_funcs_set_close_path_func(DFUNCS, close_path_f, None)


def തള്ളുക_transform_f(funcs, paint_data, xx, yx, xy, yy, dx, dy, user_data):
    ഉയർത്തുക NotImplementedError


def എടുക്കുക_transform_f(funcs, paint_data, user_data):
    ഉയർത്തുക NotImplementedError


def color_f(funcs, paint_data, is_foreground, color, user_data):
    context = POOL[paint_data]
    r = hb.color_get_red(color) / 255
    g = hb.color_get_green(color) / 255
    b = hb.color_get_blue(color) / 255
    a = hb.color_get_alpha(color) / 255
    context.set_source_rgba(r, g, b, a)
    context.paint()


def തള്ളുക_clip_rectangle_f(funcs, paint_data, xmin, ymin, xmax, ymax, user_data):
    context = POOL[paint_data]
    context.save()
    context.rectangle(xmin, ymin, xmax, ymax)
    context.clip()


def തള്ളുക_clip_glyph_f(funcs, paint_data, glyph, font, user_data):
    context = POOL[paint_data]
    context.save()
    context.new_path()
    hb.font_draw_glyph(font, glyph, DFUNCS, paint_data)
    context.close_path()
    context.clip()


def എടുക്കുക_clip_f(funcs, paint_data, user_data):
    context = POOL[paint_data]
    context.restore()


def തള്ളുക_group_f(funcs, paint_data, user_data):
    ഉയർത്തുക NotImplementedError


def എടുക്കുക_group_f(funcs, paint_data, mode, user_data):
    ഉയർത്തുക NotImplementedError


PFUNCS = hb.paint_funcs_create()
hb.paint_funcs_set_തള്ളുക_transform_func(PFUNCS, തള്ളുക_transform_f, None)
hb.paint_funcs_set_എടുക്കുക_transform_func(PFUNCS, എടുക്കുക_transform_f, None)
hb.paint_funcs_set_color_func(PFUNCS, color_f, None)
hb.paint_funcs_set_തള്ളുക_clip_glyph_func(PFUNCS, തള്ളുക_clip_glyph_f, None)
hb.paint_funcs_set_തള്ളുക_clip_rectangle_func(PFUNCS, തള്ളുക_clip_rectangle_f, None)
hb.paint_funcs_set_എടുക്കുക_clip_func(PFUNCS, എടുക്കുക_clip_f, None)
hb.paint_funcs_set_തള്ളുക_group_func(PFUNCS, തള്ളുക_group_f, None)
hb.paint_funcs_set_എടുക്കുക_group_func(PFUNCS, എടുക്കുക_group_f, None)


def makebuffer(വാക്കുകൾ):
    buf = hb.buffer_create()

    text = " ".join(വാക്കുകൾ)
    hb.buffer_add_codepoints(buf, [ord(c) വേണ്ടി c ഇൽ text], 0, len(text))

    hb.buffer_guess_segment_properties(buf)

    തിരികെ buf


def justify(face, വാക്കുകൾ, advance, target_advance):
    font = hb.font_create(face)
    buf = makebuffer(വാക്കുകൾ)

    wiggle = 5
    shrink = target_advance - wiggle < advance
    expand = target_advance + wiggle > advance

    ret, advance, tag, മൂല്യം_2 = hb.shape_justify(
        font,
        buf,
        None,
        None,
        target_advance,
        target_advance,
        advance,
    )

    എങ്കിൽ അല്ല ret:
        തിരികെ അസത്യം, buf, None

    എങ്കിൽ tag:
        variation = hb.variation_t()
        variation.tag = tag
        variation.മൂല്യം_2 = മൂല്യം_2
    അല്ലെങ്കിൽ:
        variation = None

    എങ്കിൽ shrink ഒപ്പം advance > target_advance + wiggle:
        തിരികെ അസത്യം, buf, variation
    എങ്കിൽ expand ഒപ്പം advance < target_advance - wiggle:
        തിരികെ അസത്യം, buf, variation

    തിരികെ സത്യം, buf, variation


def shape(face, വാക്കുകൾ):
    font = hb.font_create(face)
    buf = makebuffer(വാക്കുകൾ)
    hb.shape(font, buf)
    positions = hb.buffer_get_glyph_positions(buf)
    advance = sum(p.x_advance വേണ്ടി p ഇൽ positions)
    തിരികെ buf, advance


def typeset(face, text, target_advance):
    വരികൾ = []
    വാക്കുകൾ = []
    വേണ്ടി word ഇൽ text.split():
        വാക്കുകൾ.append(word)
        buf, advance = shape(face, വാക്കുകൾ)
        എങ്കിൽ advance > target_advance:
            # Shrink
            ret, buf, variation = justify(face, വാക്കുകൾ, advance, target_advance)
            എങ്കിൽ ret:
                വരികൾ.append((buf, variation))
                വാക്കുകൾ = []
            # If if fails, pop the last word and shrink, and hope for the best.
            # A too short line is better than too long.
            അല്ലെങ്കിലെങ്കിൽ len(വാക്കുകൾ) > 1:
                വാക്കുകൾ.എടുക്കുക()
                _, buf, variation = justify(face, വാക്കുകൾ, advance, target_advance)
                വരികൾ.append((buf, variation))
                വാക്കുകൾ = [word]
            # But if it is one word, meh.
            അല്ലെങ്കിൽ:
                വരികൾ.append((buf, variation))
                വാക്കുകൾ = []

    # Justify last line
    എങ്കിൽ വാക്കുകൾ:
        _, buf, variation = justify(face, വാക്കുകൾ, advance, target_advance)
        വരികൾ.append((buf, variation))

    തിരികെ വരികൾ


def render(face, text, context, width, height, fontsize):
    font = hb.font_create(face)

    margin = fontsize * 2
    scale = fontsize / hb.face_get_upem(face)
    target_advance = (width - (margin * 2)) / scale

    വരികൾ = typeset(face, text, target_advance)

    _, extents = hb.font_get_h_extents(font)
    lineheight = extents.ascender - extents.descender + extents.വരി_gap
    lineheight *= scale

    context.save()
    context.translate(0, margin)
    context.set_font_size(12)
    context.set_source_rgb(1, 0, 0)
    വേണ്ടി buf, variation ഇൽ വരികൾ:
        rtl = hb.buffer_get_direction(buf) == hb.direction_t.RTL
        എങ്കിൽ rtl:
            hb.buffer_reverse(buf)
        infos = hb.buffer_get_glyph_infos(buf)
        positions = hb.buffer_get_glyph_positions(buf)
        advance = sum(p.x_advance വേണ്ടി p ഇൽ positions)

        context.translate(0, lineheight)
        context.save()

        context.save()
        context.move_to(0, -20)
        എങ്കിൽ variation:
            tag = hb.tag_to_string(variation.tag).decode("ascii")
            context.show_text(f" {tag}={variation.value:g}")
        context.move_to(0, 0)
        context.show_text(f" {advance:g}/{target_advance:g}")
        context.restore()

        എങ്കിൽ variation:
            hb.font_set_variations(font, [variation])

        context.translate(margin, 0)
        context.scale(scale, -scale)

        എങ്കിൽ rtl:
            context.translate(target_advance, 0)

        വേണ്ടി info, pos ഇൽ zip(infos, positions):
            എങ്കിൽ rtl:
                context.translate(-pos.x_advance, pos.y_advance)
            context.save()
            context.translate(pos.x_offset, pos.y_offset)
            hb.font_paint_glyph(font, info.codepoint, PFUNCS, id(context), 0, 0x0000FF)
            context.restore()
            എങ്കിൽ അല്ല rtl:
                context.translate(+pos.x_advance, pos.y_advance)

        context.restore()
    context.restore()


def പ്രധാന(fontpath, textpath):
    fontsize = 70

    blob = hb.blob_create_from_file(fontpath)
    face = hb.face_create(blob, 0)

    with open(textpath) ഇവ f:
        text = f.പഠ()

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
    Gtk.പ്രധാന()


എങ്കിൽ __name__ == "__main__":
    ഇറക്കുമതി argparse

    parser = argparse.ArgumentParser(description="HarfBuzz justification demo.")
    parser.add_argument("fontfile", help="font file")
    parser.add_argument("textfile", help="text")
    args = parser.parse_args()
    പ്രധാന(args.fontfile, args.textfile)
