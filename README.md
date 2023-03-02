<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/RaphGL/NeoSent">
    <img src="logo.png" alt="Logo" height="80">
  </a>

  <h3 align="center">Suckful Sent</h3>
  <p align="center">
    <br />
    <a href="https://github.com/RaphGL/NeoSent"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    ·
    <a href="https://github.com/RaphGL/NeoSent/issues">Report Bug</a>
    ·
    <a href="https://github.com/RaphGL/NeoSent/issues">Request Feature</a>
  </p>
</p>

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li>
    <a href="#usage">Usage</a>
      <ul>
        <li><a href="#changing-font-and-colors">Changing font and colors</a></li>
        <li><a href="#keybindings">Keybindings</a></li>
      </ul>
    </li>
    <li><a href="#license">License</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->

Neosent is a drop-in replacement for Suckless Sent.

It provides everything you need out of the box instead of relying on patches and editing header files.

<center>

| Features                          | Implemented |
| --------------------------------- | :---------: |
| PNG, JPG, AVIF                    |      ✅      |
| Command-line options              |      ✅      |
| Inverted colors and toggling them |      ✅      |
| Progress bar                      |      ✅      |
| Fullscreen mode                   |      ✅      |
| PDF generation                    |      ❌      |

</center>

### Built With

- C
- [SDL2](https://www.libsdl.org/)

<!-- GETTING STARTED -->

## Getting Started

Make sure you have the SDL2, SDL2_ttf and SDL2_image libraries installed.

On Arch:

```sh
sudo pacman -S sdl2 sdl2_image sdl2_ttf
```

### Installation

#### Manually
1. Clone the project
2. Run:

```sh
make
```
3. Move the `nsent` file to your `$PATH`.

#### AUR
```sh
paru -S neosent-git
```

<!-- USAGE EXAMPLES -->

## Usage

NeoSent files usually have a `.ns` extension, but any (or no) extension is allowed as well.

```sh
$ nsent file.ns
```

Some attributes can be changed to fit the user's preference.

The syntax is very minimal, to learn it just see this example:

```
NeoSent

@some_image.png

It depends on:
- SDL2

neosent [FILE]
one slide per paragraph
# This is a comment and it won't appear in the presentation

\ # A backslash alone will create an empty slide

# both of these will be shown instead of interpreted
\@FILE.png 
\# Some comment?

Having problems? Make an issue!
```

### Changing font and colors

All changeable attributes are available through flags and environment variables. 
Check `nsent -h` to know what flags are available. 
The following environment variables are available: 
- `NS_FG_COLOR` - changes text color
- `NS_BG_COLOR` - changes the background color
- `NS_FONT` - changes the preferred font, if not found it will fallback to a system default

Fonts are matched to the system's. Just roughly name the font you want and neosent will fetch it. Fonts have to be installed on the system to work.

```sh
nsent -f "JetbrainsMono" presentation.ns
nsent -f "Ubuntu" presentation.ns
```

Colors are represented as RBG values. Passing them to the program:
```sh
nsent -t 0 -b 0xFFFFFF presentation.ns # black text and white background
nsent -t 0xeb4034 -b 0x2e22a1 presentation.ns # red text and blue background
```

### Keybindings

<center>

| Previous page | Next page     |
| ------------- | ------------- |
| `Left Arrow`  | `Right Arrow` |
| `Up Arrow`    | `Down Arrow`  |
| `Page Up`     | `Page Down`   |
| `h`           | `l`           |
| `k`           | `j`           |
| `p`           | `n`           |

Miscellaneous keybindings:

| Function                  | Keybind        |
| ------------------------- | -------------- |
| Go to first and last page | `Home` & `End` |
| Invert colors             | `i`            |
| Go fullscreen             | `f` / `F11`    |
| Quit                      | `q`            |

</center>

<!-- LICENSE -->

## License

Distributed under GPLv3 License. See [`LICENSE`](https://github.com/RaphGL/NeoSent/blob/main/LICENSE) for more information.
