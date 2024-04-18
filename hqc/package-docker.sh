#!/bin/bash

set -eu

function cleanup_image {
  docker rmi ${image}
}

function cleanup_container {
  docker rm ${container}
}

image=pqci${RANDOM}
container=pqcc${RANDOM}

docker build --no-cache --tag ${image} .
trap cleanup_image EXIT
docker run --name ${container} ${image} 
trap "cleanup_container;cleanup_image" EXIT
docker cp ${container}:/home/pqcuser/package-pqclean/hqc/pqclean-hqc-$(date -u +"%Y%m%d").tar.gz .
