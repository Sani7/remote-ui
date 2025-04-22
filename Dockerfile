FROM fedora:latest
RUN dnf upgrade -y
RUN dnf install -y gcc g++ ninja cmake git clang-tools-extra
RUN dnf install -y qt6-*
RUN dnf install -y libgpiod-devel
