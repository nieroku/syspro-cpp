#### Build

```sh
meson wrap update-db
meson setup --reconfigure builddir --buildtype debug -Db_sanitize=address,undefined
```

#### Testing

```sh
meson test -C builddir
```
