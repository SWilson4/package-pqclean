# package-pqclean/hqc

Use this repository to patch [HQC](https://pqc-hqc.org) code for inclusion in the [PQClean](https://github.com/PQClean/PQClean.git) project.

## Getting patched code

Run `./checkout.sh` to place the upstream code in the `./upstream` directory and patched code in the `./upstream_patched` directory.

## Updating patches

Edit this code as you like, then run `./update_patches.sh` to update the `patches` directory accordingly.

## Packaging code

Run `./package.sh` to produce a `tar.gz` file with your patched code nicely formatted for inclusion in PQClean.
Alternatively, run `./package-docker.sh` to generate the `tar.gz` file in a Docker container and copy it into this directory.

The advantage of using the Docker container is that it should use the same version of astyle that runs in CI,
which is what your code will ultimately be checked against.
