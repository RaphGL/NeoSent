from neosent.sheetwriter import *
from sys import argv, exit
import pkg_resources


def main():
    font_path = pkg_resources.resource_filename(__name__, 'data/OpenSansEmoji.ttf')
    sheet = SheetWriter((1280, 720), (0, 0, 0), '/usr/share/fonts/gnu-free/FreeSans.ttf')
    sheet.create_sheet()

if __name__ == '__main__':
    main()
