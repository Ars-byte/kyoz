# kyoz

A small command-line wrapper around the [XBPS](https://wiki.voidlinux.org/XBPS) package manager for Void Linux.

<a href="http://www.wtfpl.net/"><img
       src="http://www.wtfpl.net/wp-content/uploads/2012/12/wtfpl-badge-4.png"
       width="80" height="15" alt="WTFPL" /></a>

## Features

- Short, memorable commands for everyday package management
- Same aliases as the long commands (`i`, `u`, `r`, `s`, `c`)
- Single, statically linked binary with no runtime dependencies

## Requirements

- Void Linux
- A C++ compiler (only needed if you build from source)

```sh
sudo xbps-install -S gcc
```

## Build

```sh
g++ -std=c++17 -O2 -static -s -o kyo kyo.cpp
```

## Install

```sh
sudo install -m 755 kyo /usr/local/bin/kyo
```

Or, for the current user only:

```sh
install -Dm 755 kyo ~/.local/bin/kyo
```

You can also grab a prebuilt binary from the [latest release](https://github.com/Ars-byte/kyoz/releases/latest).

## Usage

```
kyo {install|update|remove|search|clean} [arguments...]
```

| Command          | Alias | Arguments     | Description                                  |
| ---------------- | :---: | ------------- | -------------------------------------------- |
| `install`        | `i`   | `<package...>`| Install packages                             |
| `update`         | `u`   | —             | Update the whole system                      |
| `remove`         | `r`   | `<package...>`| Remove packages (and their orphans)          |
| `search`         | `s`   | `<term...>`   | Search for packages                          |
| `clean`          | `c`   | —             | Remove orphans and obsolete packages         |

## Examples

```sh
# Install fastfetch
kyo install fastfetch
kyo i fastfetch

# Update the system
kyo update

# Search for a package
kyo search neovim

# Clean orphaned and obsolete packages
kyo clean
```

## License

Released under the [WTFPL](http://www.wtfpl.net/).
