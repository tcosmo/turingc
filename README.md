# TuringC

TuringC is a Turing machine simulator written in C.

## Dependencies

We use the SDL (v2), a powerful graphics, game and media library.

We use conan to manage our dependencies, in particular SDL: [https://docs.conan.io/en/latest/getting_started.html](https://docs.conan.io/en/latest/getting_started.html).

You can install conan using python (>3):

```bash
python3 -m pip install conan
```

SDL is not available on the standard conan repository, you have to add the `bincrafters` repository:

```bash
conan remote add bincrafters https://bincrafters.jfrog.io/artifactory/api/conan/public-conan
conan config set general.revisions_enabled=1
```

## Build

```bash
git clone
cd turingc
mkdir build
conan install ..
cmake ..
make
(cd bin && ./turingc ">11110000110011111101")
```
