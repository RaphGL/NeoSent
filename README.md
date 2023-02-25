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
    <li><a href="#usage">Usage</a></li>
    <li><a href="#license">License</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->

Neosent is a drop-in replacement for Suckless Sent.

It provide everything you need out of the box instead of relying on patches and editing header files.

Supports:

- PNG, JPG, AVIF    ✅
- TTF and OTF fonts ✅

### Built With

- C
- [SDL2](https://www.libsdl.org/)

<!-- GETTING STARTED -->

## Getting Started

Make sure you the SDL2, SDL2_ttf and SDL2_image libraries installed.

On Arch:

```sh
sudo pacman -S sdl2 sdl2_image sdl2_ttf
```

### Installation

1. Download the `.zip` file
2. Extract it
3. On the extracted folder run:

```sh
make
```

5. Move the `nsent` file to your `$PATH`.

<!-- USAGE EXAMPLES -->

## Usage

NeoSent files usually have a `.ns` extension, but any (or no) extension is allowed as well.

```sh
$ nsent file.ns
```

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

\@FILE.png # Image is escape so it will only display the text

Having problems? Make an issue!
```

<!-- LICENSE -->

## License

Distributed under GPLv3 License. See [`LICENSE`](https://github.com/RaphGL/NeoSent/blob/main/LICENSE) for more information.
