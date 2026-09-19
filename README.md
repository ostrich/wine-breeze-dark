# Wine Breeze Dark

Small registry theme for making Wine's classic `(No Theme)` appearance use a Breeze Dark-like palette.

## Download

[Download the latest release](https://github.com/ostrich/wine-breeze-dark/releases/latest) and extract the ZIP. It contains:

```text
syscolor-demo.exe
wine-breeze-dark.reg
wine-reset-theme.reg
README.md
LICENSE
```

## Apply

```sh
wine regedit wine-breeze-dark.reg
winecfg
```

In `winecfg`, use:

- Theme: `(No Theme)`
- WinRT theme: `Dark`

Restart already-running Wine applications after applying the registry file.

## Revert

```sh
wine regedit wine-reset-theme.reg
winecfg
```

Then set the Wine theme and WinRT theme back to your preferred defaults.

## System Color Demo

`syscolor-demo.exe` shows Wine system colors, classic controls, common controls, menus, tooltips, status bars, and native scrollbars.

Run the demo with:

```sh
wine syscolor-demo.exe
```

Build with MinGW:

```sh
make
```

Or directly:

```sh
x86_64-w64-mingw32-gcc -O2 -Wall -Wextra -municode -mwindows syscolor-demo.c -o syscolor-demo.exe -lcomctl32 -lgdi32 -luser32
x86_64-w64-mingw32-strip syscolor-demo.exe
```

The generated executable is ignored by Git. Tagged releases build and package it automatically with both registry files, this README, and the license.

## License

MIT. See [LICENSE](LICENSE).
