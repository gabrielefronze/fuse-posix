# Rucio `FUSE-posix` interface

This repository carries the best attempt at making the Rucio namespace `posix`-mountable.

It is based on `C` code (for the custom `FUSE` specialization) and on `C++11` for utilities and wrappers.

This tool is intended as an **alpha** version unless explicitly said and has to be considered WIP.

Please note that the first target is to get a **read-only file system**.

## Access pattern
The Rucio file catalog is much flatter than that of a usual `POSIX` filesystems and as such its representation has to be structured according to that:

- the root of the mount is intended to be a `cvmfs`-like "`/rucio`";
- the first level of the tree (namely `/rucio/*`) should be filled with all the available scopes;
- each scope should appear as a directory filled with its DiDs;
- file DiDs will appear as files;
- container and dataset DiDs will appear as directories;
- datasets and containers might include already represented DiDs: a routine to handle such loops should be present.

## DONEs
- Curl-REST wrapper based on `libcurl` has been implemented in the form of `GET` and `POST` methods, for the moment. The `GET` method has been tested widely and appears to be consistent with the output of the `curl` CLI.
- The curl-REST wrapper uses a singleton-equipped `curl` instance to limit memory fingerprint.
- Rucio-API wrappers, based on Rucio's RESTFul interface, are being developed. At the moment access token retrieval through userpass authentication and scopes listing are implemented and tested.
- Fuse mounting works!
- Rucio-API is now backed with cache, to reduce proactively the number of cURL operations.
- Introduced runtime parsing of configuration file (`settings.json`?) to replace `globals.cpp`. The syntax is clear from the included example file.
- Introduced log-level based logging using submodule `fastlog`[link](https://github.com/gabrielefronze/fastlog).
  As a side note three log levels (ERROR > INFO > DEBUG, in order of reversed gravity and direct verbosity) have been introduced and are usable at runtime:
  
  - `-v` = INFO
  - `-vv` = DEBUG
  - `<no opt>` = ERROR (default)
  
  For example `rucio-fuse-main -v` invokes the software with INFO log level, `rucio-fuse-main -vv` with DEBUG level and so on.

## TODOs
- Implement the read Fuse operation: ***at the moment the file contents are dummy***!
- Introduce certificate-base authentication mechanism.
- Fix Mac OS X mouting issues
- Surely a lot more...

## How to build
To build the software please run:

```[shell]
./build.sh
```

This has been tested on CentOS7 ~~and Mac OS X Mojave 10.14.6~~.
Mac OS X special files created by the OS FS service generate a lot of issues which should be dealt with and prevent the Fuse module from correct mounting under Mac.

To complete the build `libcurl-devel`, `fuse-libs` and `fuse-devel` packages (or equivalent) must be present:
`cmake` will try to locate them for you and trigger some build messages if unable to do so.
Please note that `cmake` version 3 or greater is needed.
