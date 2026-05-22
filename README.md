# Wine Breeze Dark

Small registry theme for making Wine's classic `(No Theme)` appearance use a Breeze Dark-like palette.

## Apply

```sh
wine regedit theme/wine-breeze-dark.reg
winecfg
```

In `winecfg`, use:

- Theme: `(No Theme)`
- WinRT theme: `Dark`

Restart already-running Wine applications after applying the registry file.

## Revert

```sh
wine regedit theme/wine-reset-theme.reg
winecfg
```

Then set the Wine theme and WinRT theme back to your preferred defaults.

## System Color Demo

`syscolor_demo/syscolor_demo.exe` shows Wine system colors, classic controls, common controls, menus, tooltips, status bars, and native scrollbars.

A prebuilt 64-bit Windows executable is included for convenience:

```sh
wine syscolor_demo/syscolor_demo.exe
```

Build with MinGW:

```sh
make -C syscolor_demo
```

Or directly:

```sh
x86_64-w64-mingw32-gcc -O2 -municode syscolor_demo/syscolor_demo.c -o syscolor_demo/syscolor_demo.exe -lcomctl32 -lgdi32 -luser32
```
