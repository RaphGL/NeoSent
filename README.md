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

Neosent is a drop-in replacement for Suckless Sent. It reimplements all of the features but instead of depending on X to output presentations it outputs presentations directly to PDF with the goal of making them more portable.

### Built With

- [Python](https://www.python.org/)
- [Pillow](https://pillow.readthedocs.io/en/stable/)

<!-- GETTING STARTED -->

## Getting Started

### Installation

1. Download the `.zip` file
2. Extract it
3. On the extracted folder run:

```sh
pip install --user .
```

5. Add `~/.local/lib/python3.10/site-packages/neosent` to your PATH.

<!-- USAGE EXAMPLES -->

## Usage

NeoSent files usually have a `.ns` extension, but any (or no) extension is allowed as well.

To get your PDF, run:

```sh
$ neosent file.ns
```

All defaults can be changed through the use of flags.
Example:

```sh
$ neosent --set-res 1280 720                 \
          --set-bg 40 42 54                  \
          --set-fg 248 248 242               \
          --set-font "~/.fonts/FreeSans.ttf" \
          --set-font-size 24                 \
          -o "Final Presentation.pdf"        \
          final_presentation.ns
```

To learn more about the flags available, check `neosent --help`.

The syntax is very minimal, to learn it just see this example:

```
NeoSent

@some_image.png

It depends on:
- Python
- Pillow

neosent [FILE]
one slide per paragraph
# This is a comment and it won't appear in the presentation

\ # A backslash alone will create an empty slide

\@FILE.png # Image is escape so it will only display the text

Having problemas? Make an issue!
```

<!-- LICENSE -->

## License

Distributed under GPLv3 License. See [`LICENSE`](https://github.com/RaphGL/NeoSent/blob/main/LICENSE) for more information.

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[contributors-shield]: https://img.shields.io/github/contributors/othneildrew/Best-README-Template.svg?style=for-the-badge
[contributors-url]: https://github.com/othneildrew/Best-README-Template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/othneildrew/Best-README-Template.svg?style=for-the-badge
[forks-url]: https://github.com/othneildrew/Best-README-Template/network/members
[stars-shield]: https://img.shields.io/github/stars/othneildrew/Best-README-Template.svg?style=for-the-badge
[stars-url]: https://github.com/othneildrew/Best-README-Template/stargazers
[issues-shield]: https://img.shields.io/github/issues/othneildrew/Best-README-Template.svg?style=for-the-badge
[issues-url]: https://github.com/othneildrew/Best-README-Template/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: https://github.com/othneildrew/Best-README-Template/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew
[product-screenshot]: images/screenshot.png
