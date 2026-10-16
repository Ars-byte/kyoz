Wrapper for xbps

<a href="http://www.wtfpl.net/"><img
       src="http://www.wtfpl.net/wp-content/uploads/2012/12/wtfpl-badge-4.png"
       width="80" height="15" alt="WTFPL" /></a>
       
# COMPILE
```
g++ -std=c++17 -O2 -static -s -o kyo kyo.cpp
```
# INSTALL
```
cp kyo ~/.local/bin/ 
```

# Usage: 
kyo {install|update|remove|search|clean} [arguments...]

Commands:
  install, i   <package...>   Install packages
  
  update, u                   Update the system
  
  remove, r    <package...>   Remove packages (with -o)
  
  search, s    <term...>      Search for packages
  
  clean, c                    Clean orphans and obsolete packages

# Example: for install fastfetch run...
  
kyo install fastfetch

or

kyo i fastfetch