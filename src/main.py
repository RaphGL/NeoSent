from sheetwriter import *
from PIL import Image


def main():
    file = parse_file('example.ns')
    sheet = SheetWriter(file, (1280, 720), (0, 0, 0),
                        '/usr/share/fonts/open-sans/OpenSans-Regular.ttf')
    sheet.create_sheet()
    convert_to_pdf()


if __name__ == '__main__':
    main()
