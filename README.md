#### Tasks

- [nsst1](https://github.com/nieroku/syspro-cpp/tree/nstt1/nstt1)
- [nsst2](https://github.com/nieroku/syspro-cpp/tree/main/nstt2)
- [nsst3](https://github.com/nieroku/syspro-cpp/tree/nsst3/nstt1)
- [nsst4](https://github.com/nieroku/syspro-cpp/tree/main/nstt1)

#### Build

```sh
meson wrap update-db
meson setup --reconfigure builddir --buildtype debug -Db_lundef=false -Db_sanitize=address,undefined
```

#### Testing

```sh
meson test -C builddir
```
