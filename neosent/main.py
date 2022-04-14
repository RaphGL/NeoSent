from neosent.sheetwriter import *
from sys import exit
import pkg_resources
import click

@click.group(invoke_without_command=True)
@click.option('--set-res', 'res', default=(3840, 2160), show_default=True, type=(int, int),
              metavar='<height width>', help="the sheet's height and width")  # Needs to be converted into (1280x720)
@click.option('--set-font', 'font', default="data/OpenSansEmoji.ttf",
              metavar='<font_path>', help='the font used')
@click.option('--set-bg', 'bg', default=(8, 8, 8), type=(int, int, int),
              metavar='<R G B>', help='set the background color')  # Needs to be converted to (0, 0, 0) before being consumed
@click.option('--set-fg', 'fg', default=(248, 248, 242), type=(int, int, int),
              metavar='<R G B>', help='set the foreground color')  # Idem
@click.option('--set-font-size', 'font_size', default=240, show_default=True, type=int,
            metavar='<size>', help='set the font size')
@click.argument('filename', type=click.Path(exists=True))
def cli(res, font, bg, fg, font_size, filename):
    font_path = ''
    if font == 'data/OpenSansEmoji.ttf':
        font_path = pkg_resources.resource_filename(__name__, font)
    else:
        font_path = font

    sheet = SheetWriter(res, bg, fg, font_path, font_size, filename)
    sheet.create_sheet()
    print(res, font, bg, fg, filename)
    exit(0)


if __name__ == '__main__':
    cli()
