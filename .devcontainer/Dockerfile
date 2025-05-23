ARG VARIANT=42
FROM fedora:${VARIANT}

RUN dnf update -y

RUN dnf install -y \
    git \
    gcc \
    gcc-c++ \
    clang \
    cmake \
    ninja-build \
    SDL3-devel \
    # vcpkg deps
    curl zip unzip pkgconfig

ENV VCPKG_ROOT=/usr/local/vcpkg
ENV PATH="${PATH}:${VCPKG_ROOT}"

RUN git clone https://github.com/microsoft/vcpkg.git --depth=1 ${VCPKG_ROOT} \
    && "${VCPKG_ROOT}"/bootstrap-vcpkg.sh -disableMetrics

# where's awk???
RUN dnf install -y gawk
