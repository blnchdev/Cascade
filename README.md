# Cascade

Cascade is a tool to re-organize your downloads according to user-defined rules stored in [here](https://github.com/blnchdev/Cascade/tree/main/Rules)  

<figure align="center">
  <img src="https://blanche.dev/img/Cascade/UsePreview.gif" width="1000" />
</figure>

### Usage
Cascade is purely CLI, the proper usage is: (<> = Mandatory, [] = Optional)

`Cascade "<DownloadDirectory>" "<TargetDirectory>" [parameters]`
e.g.:
`./Cascade.exe "C:/Users/Blanche/Downloads/" "E:/Downloads/" --reorganize`

Available parameters are:
- --reorganize: Iterates through directories in <TargetDirectory>, which is useful when rules are changed/new ones are added
- --dryrun: Only outputs where items would be moved; does not actually move items

### Output
Cascade will create folders at your target directory, resulting in those:

<figure align="center">
  <img src="https://blanche.dev/img/Cascade/Directories.png" width="1000" />
</figure>

It will also log what it has done, e.g.:
<figure align="center">
  <img src="https://blanche.dev/img/Cascade/Logs.png" width="1000" />
</figure>

### Compiling
#### Requirements
- CMake 4.4 or newer
- GCC/libstdc++ (C++26)
#### Steps
```
git clone https://github.com/blnchdev/Cascade
cd Cascade
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```
The output will be in /build/
