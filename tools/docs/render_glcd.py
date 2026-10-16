#!/usr/bin/env python3
"""Render MenuOS screens as SVG pictures of the 128x64 KS0108 GLCD.

The frames come from the real menu code: the host test binary dumps every
screen it checks (``test_menu --dump DIR``), and this script draws them the
way hw/driver/disp/hwdisp.cpp does in GRAPH_DISPLAY mode, using the very
5x7 font from the GLCD library.

    make ARCH=host build/host-native/test_menu
    build/host-native/test_menu --dump /tmp/frames
    python3 tools/docs/render_glcd.py /tmp/frames docs/img
"""
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
FONT_H = os.path.join(ROOT, "menuos/hw/driver/disp/glcd/fonts/SystemFont5x7.h")

# hwdisp.cpp geometry
LCD_W, LCD_H = 128, 64
XINDENT, YINDENT = 6, 10
FONT_W, FONT_H_PX = 6, 8

PIXEL = 4            # SVG units per LCD pixel
GAP = 0.7            # gap between pixels, gives the dot-matrix look
BEZEL = 18

LCD_BG = "#b3cc45"
LCD_OFF = "#a5bf3c"
LCD_ON = "#18210b"
BEZEL_FILL = "#20242b"
BEZEL_EDGE = "#3a404a"


def load_font():
    src = open(FONT_H, encoding="latin-1").read()
    body = src[src.index("System5x7[]"):]
    nums = [int(x, 16) for x in re.findall(r"0x([0-9A-Fa-f]+)", body.split("};")[0])]
    first, count = nums[4], nums[5]
    data = nums[6:]
    font = {}
    for i in range(count):
        font[chr(first + i)] = data[i * 5:(i + 1) * 5]
    return font


def parse_frame(text):
    """Parse a terminal frame (see termdisp.h) into rows, selection, cursor."""
    lines = [l for l in text.splitlines() if not l.startswith("#")]
    rows, selected, cursor = [], [], None
    body = lines[1:]
    cursor_row = None
    for line in body:
        if line.startswith("+"):
            break
        selected.append(line[0] == ">")
        if line[0] == "*":
            cursor_row = len(rows)
        rows.append(line[1:-1])
    tail = body[len(rows) + 1:]
    if tail and "^" in tail[0]:
        marks = tail[0][1:]
        cursor = (cursor_row, marks.index("^"), marks.count("^"))
    return rows, selected, cursor


def render_pixels(rows, selected, cursor, font):
    px = [[0] * LCD_W for _ in range(LCD_H)]

    def put(x, y, v=1):
        if 0 <= x < LCD_W and 0 <= y < LCD_H:
            px[y][x] = v

    for r, text in enumerate(rows):
        for c, ch in enumerate(text):
            cols = font.get(ch, font[" "])
            x0 = XINDENT + 1 + c * FONT_W
            y0 = YINDENT + 1 + r * FONT_H_PX
            for dx, bits in enumerate(cols):
                for dy in range(7):
                    if bits >> dy & 1:
                        put(x0 + dx, y0 + dy)
        if selected[r]:  # GLCD.InvertRect(XINDENT, YINDENT + row*8, LCDWIDTH - XINDENT, 8)
            for y in range(YINDENT + r * FONT_H_PX, YINDENT + (r + 1) * FONT_H_PX + 1):
                for x in range(XINDENT, LCD_W):
                    if 0 <= y < LCD_H:
                        px[y][x] ^= 1
    return px


def draw_cursor(px, row, col, length):
    # GLCD.DrawRect(XINDENT + col*6, YINDENT + row*8, length*6, 8)
    x0, y0 = XINDENT + col * FONT_W, YINDENT + row * FONT_H_PX
    x1, y1 = x0 + length * FONT_W, y0 + FONT_H_PX
    for x in range(x0, x1 + 1):
        for y in (y0, y1):
            if 0 <= x < LCD_W and 0 <= y < LCD_H:
                px[y][x] = 1
    for y in range(y0, y1 + 1):
        for x in (x0, x1):
            if 0 <= x < LCD_W and 0 <= y < LCD_H:
                px[y][x] = 1


_uid = [0]


def lcd_group(px, ox, oy, scale=1.0):
    """SVG group with bezel + pixels at offset (ox, oy).

    Lit pixels are merged into horizontal runs; a grid pattern drawn on top
    in the panel colour brings back the dot-matrix gaps. Keeps files small.
    """
    _uid[0] += 1
    pid = f"dots{_uid[0]}"
    w = LCD_W * PIXEL
    h = LCD_H * PIXEL
    out = [f'<g transform="translate({ox},{oy}) scale({scale:g})">']
    out.append(f'<defs><pattern id="{pid}" x="{BEZEL}" y="{BEZEL}" width="{PIXEL}" height="{PIXEL}" '
               f'patternUnits="userSpaceOnUse"><path d="M{PIXEL - GAP:g} 0h{GAP:g}v{PIXEL}h-{GAP:g}z'
               f'M0 {PIXEL - GAP:g}h{PIXEL}v{GAP:g}h-{PIXEL}z" fill="{LCD_BG}"/></pattern></defs>')
    out.append(f'<rect x="0" y="0" width="{w + 2 * BEZEL}" height="{h + 2 * BEZEL}" rx="10" '
               f'fill="{BEZEL_FILL}" stroke="{BEZEL_EDGE}" stroke-width="2"/>')
    out.append(f'<rect x="{BEZEL}" y="{BEZEL}" width="{w}" height="{h}" fill="{LCD_OFF}"/>')
    runs = []
    for y in range(LCD_H):
        x = 0
        while x < LCD_W:
            if px[y][x]:
                x0 = x
                while x < LCD_W and px[y][x]:
                    x += 1
                runs.append(f"M{BEZEL + x0 * PIXEL} {BEZEL + y * PIXEL}h{(x - x0) * PIXEL}v{PIXEL}h-{(x - x0) * PIXEL}z")
            else:
                x += 1
    out.append(f'<path fill="{LCD_ON}" d="{"".join(runs)}"/>')
    out.append(f'<rect x="{BEZEL}" y="{BEZEL}" width="{w}" height="{h}" fill="url(#{pid})"/>')
    out.append("</g>")
    return "\n".join(out)


def screen(frame_file, font):
    rows, selected, cursor = parse_frame(open(frame_file).read())
    px = render_pixels(rows, selected, None, font)
    if cursor is not None:
        draw_cursor(px, *cursor)
    return px


CAPTION_CSS = """
  <style>
    .cap { font: 600 21px -apple-system, 'Segoe UI', Helvetica, Arial, sans-serif; fill: #24292f; }
    .sub { font: 17px -apple-system, 'Segoe UI', Helvetica, Arial, sans-serif; fill: #57606a; }
    @media (prefers-color-scheme: dark) {
      .cap { fill: #e6edf3; }
      .sub { fill: #8d96a0; }
    }
  </style>"""


def write_gallery(items, out_path, font, cols=2):
    lw = LCD_W * PIXEL + 2 * BEZEL
    lh = LCD_H * PIXEL + 2 * BEZEL
    pad_x, pad_y, cap_h = 28, 20, 70
    rows_n = (len(items) + cols - 1) // cols
    W = cols * lw + (cols + 1) * pad_x
    H = rows_n * (lh + cap_h) + (rows_n + 1) * pad_y - pad_y // 2
    parts = [f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {W} {H}" width="{W}" height="{H}" '
             f'role="img" aria-label="MenuOS screens on a 128x64 GLCD">', CAPTION_CSS]
    for i, (frame, title, sub) in enumerate(items):
        cx = pad_x + (i % cols) * (lw + pad_x)
        cy = pad_y + (i // cols) * (lh + cap_h + pad_y)
        parts.append(lcd_group(screen(frame, font), cx, cy))
        parts.append(f'<text class="cap" x="{cx + 4}" y="{cy + lh + 30}">{title}</text>')
        parts.append(f'<text class="sub" x="{cx + 4}" y="{cy + lh + 55}">{sub}</text>')
    parts.append("</svg>")
    open(out_path, "w").write("\n".join(parts) + "\n")


def write_single(frame, out_path, font):
    lw = LCD_W * PIXEL + 2 * BEZEL
    lh = LCD_H * PIXEL + 2 * BEZEL
    parts = [f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {lw} {lh}" width="{lw}" height="{lh}" '
             f'role="img" aria-label="MenuOS on a 128x64 GLCD">',
             lcd_group(screen(frame, font), 0, 0), "</svg>"]
    open(out_path, "w").write("\n".join(parts) + "\n")


BANNER_CSS = """
  <style>
    .bg { fill: #f6f8fa; stroke: #d0d7de; }
    .title { font: 800 54px 'JetBrains Mono', 'SFMono-Regular', Menlo, Consolas, monospace; fill: #1f2328; letter-spacing: -1px; }
    .ver { font: 700 54px 'JetBrains Mono', 'SFMono-Regular', Menlo, Consolas, monospace; fill: #7a9a1c; }
    .tag { font: 18px -apple-system, 'Segoe UI', Helvetica, Arial, sans-serif; fill: #424a53; }
    .chip { fill: #ffffff; stroke: #d0d7de; }
    .chipt { font: 600 13px 'SFMono-Regular', Menlo, Consolas, monospace; fill: #424a53; }
    .tree { stroke: #7a9a1c; stroke-width: 2; fill: none; }
    .node { fill: #7a9a1c; }
    @media (prefers-color-scheme: dark) {
      .bg { fill: #161b22; stroke: #30363d; }
      .title { fill: #e6edf3; }
      .ver { fill: #b3cc45; }
      .tag { fill: #9198a1; }
      .chip { fill: #0d1117; stroke: #30363d; }
      .chipt { fill: #c9d1d9; }
      .tree { stroke: #b3cc45; }
      .node { fill: #b3cc45; }
    }
  </style>"""


def write_banner(frame, out_path, font):
    k = 0.62
    lw = (LCD_W * PIXEL + 2 * BEZEL) * k
    lh = (LCD_H * PIXEL + 2 * BEZEL) * k
    W, H = 1000, 250
    lx, ly = W - lw - 40, (H - lh) / 2
    chips = ["AVR", "MSP430", "host sim", "C++", "BSD-2"]
    parts = [f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {W} {H}" width="{W}" height="{H}" '
             f'role="img" aria-label="MenuOS v1: tree-type menu for small microcontrollers">', BANNER_CSS,
             f'<rect class="bg" x="1" y="1" width="{W - 2}" height="{H - 2}" rx="16"/>']
    # a small menu tree glyph
    tx, ty = 44, 62
    parts.append(f'<path class="tree" d="M{tx} {ty}v62M{tx} {ty + 22}h18M{tx} {ty + 62}h18M{tx + 30} {ty + 62}v26M{tx + 30} {ty + 88}h18"/>')
    for cx, cy in [(tx, ty), (tx + 24, ty + 22), (tx + 24, ty + 62), (tx + 54, ty + 88)]:
        parts.append(f'<rect class="node" x="{cx - 6}" y="{cy - 6}" width="12" height="12" rx="3"/>')
    parts.append(f'<text x="112" y="100"><tspan class="title">MenuOS</tspan><tspan class="ver" dx="14">v1</tspan></text>')
    parts.append('<text class="tag" x="114" y="138">Tree-type menu engine for small microcontrollers:</text>')
    parts.append('<text class="tag" x="114" y="162">folders, apps and editable configs from one const table.</text>')
    x = 114
    for c in chips:
        w = 16 + 8.6 * len(c)
        parts.append(f'<rect class="chip" x="{x}" y="188" width="{w:.0f}" height="28" rx="14"/>')
        parts.append(f'<text class="chipt" x="{x + w / 2:.0f}" y="207" text-anchor="middle">{c}</text>')
        x += w + 10
    parts.append(lcd_group(screen(frame, font), f"{lx:.1f}", f"{ly:.1f}", k))
    parts.append("</svg>")
    open(out_path, "w").write("\n".join(parts) + "\n")


def main():
    if len(sys.argv) != 3:
        print(__doc__)
        sys.exit(1)
    frames, out = sys.argv[1], sys.argv[2]
    os.makedirs(out, exist_ok=True)
    font = load_font()
    f = lambda n: os.path.join(frames, f"{n:02d}.txt")
    write_gallery([
        (f(4), "Folder", "RIGHT opens it, RETURN goes back"),
        (f(6), "Dynamic folder", "one file shown as N numbered copies"),
        (f(10), "Config editor", "LEFT/RIGHT pick a digit, UP/DOWN change it"),
        (f(15), "Application", "sample app redraws itself every second"),
    ], os.path.join(out, "screens.svg"), font)
    write_banner(f(2), os.path.join(out, "banner.svg"), font)
    print("written:", os.path.join(out, "screens.svg"), os.path.join(out, "banner.svg"))


if __name__ == "__main__":
    main()
