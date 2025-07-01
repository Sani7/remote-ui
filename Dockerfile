FROM fedora:latest
RUN dnf upgrade -y
RUN dnf install -y gcc g++ ninja cmake git clang-tools-extra
RUN dnf install -y qt6-*-devel --exclude=qt6-doc --exclude=qt6-doc-devel --exclude=qt6-qtwebengine* --exclude=qt6-qtwebview*
RUN dnf install -y libgpiod-devel
