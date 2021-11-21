from sheetwriter import *
from sys import argv, exit


def main():
    if len(argv) == 1:
        print("Usage: neosent FILE")
    try:
        file = parse_file(argv[1])
    except IndexError:
        exit(1)
    except FileNotFoundError:
        print(f'No such file or directory: {argv[1]}')
        exit(1)

    sheet = SheetWriter(file, (1280, 720), (0, 0, 0), './OpenSansEmoji.ttf')
    sheet.create_sheet()
    convert_to_pdf()


if __name__ == '__main__':
    main()
