<p align="center">
<img src="https://images-wixmp-ed30a86b8c4ca887773594c2.wixmp.com/f/726a3088-35cc-46c1-b047-a1658cf6767c/de0o1wn-3b3505ca-63ed-4a0b-a239-65c845e84033.png?token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJ1cm46YXBwOiIsImlzcyI6InVybjphcHA6Iiwib2JqIjpbW3sicGF0aCI6IlwvZlwvNzI2YTMwODgtMzVjYy00NmMxLWIwNDctYTE2NThjZjY3NjdjXC9kZTBvMXduLTNiMzUwNWNhLTYzZWQtNGEwYi1hMjM5LTY1Yzg0NWU4NDAzMy5wbmcifV1dLCJhdWQiOlsidXJuOnNlcnZpY2U6ZmlsZS5kb3dubG9hZCJdfQ.h2LwdJ7W_ZRyMvdH7yCTziDHq8nuOJCMDssDkAhV2hg" width="350" height="350">
</p>
<img src="https://img.shields.io/badge/Language-C-purple"> <img src="https://img.shields.io/badge/OS-linux-red"> <img src="https://img.shields.io/badge/OS-OpenBSD-red"> <img src="https://img.shields.io/badge/OS-FreeBSD-red"> <img src="https://img.shields.io/badge/Doc-man-purple">

# Regexops

regexops - a small tool for printing lines that match Regex patterns


## Requirements
```
make
a Compiler like gcc, clang, tcc
a C standard library like glibc
```

## Installation


```sh
git clone https://github.com/siruidops/regexops.git regexops/
cd regexops
make
sudo make install
# doas make install
```

## Usage

```sh
regexops [REGEX_PATTERN] [FILE]
```

## Documentation
There is a man page for it in the address man/regexops.1.gz and also after installation you can see it with the following command

```sh
man regexops
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[BSD 2-Clause](https://opensource.org/licenses/BSD-2-Clause)
