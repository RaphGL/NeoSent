from sheetwriter import *
from sys import argv


def main():
    if len(argv) == 1 or argv[0] == 'help' or argv[0] == "-h" or argv[0] == '--help':
        print("Usage: neosent FILE")
    try:
        file = parse_file(argv[1])
    except IndexError:
        exit(1)
    except FileNotFoundError:
        print(f'No such file or directory: {argv[1]}')
        exit(1)

    sheet = SheetWriter(file, (1280, 720), (0, 0, 0),
                        '/usr/share/fonts/open-sans/OpenSans-Regular.ttf')
    sheet.create_sheet()
    convert_to_pdf()


if __name__ == '__main__':
    main()
