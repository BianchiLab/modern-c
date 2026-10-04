FROM ubuntu:26.04

# Modern C, 3rd edition (Gustedt) — lab environment (Ubuntu 26.04 LTS rebuild)
#
# Ubuntu 26.04 LTS "Resolute Raccoon" ships GCC 15.2 by default (confirmed
# via Canonical's own release announcement), same major version this
# container originally targeted on Fedora — the C23 story is unchanged:
# gcc 15+ defaults to -std=gnu23, matching this edition's C23 focus with
# no extra flags needed, though being explicit with -std=c23/-std=gnu23
# is still good practice.
#
#   - build-essential: Ubuntu's standard meta-package pulling in gcc, g++,
#     make, libc-dev — the apt equivalent of Fedora's separate
#     gcc/gcc-c++/glibc-devel/make packages.
#   - clang: Appendix B discusses compiler differences directly; having
#     both compilers lets you run those comparisons.
#   - gdb / valgrind: same debugging role as the original setup.
#   - Threads (ch.20) + Atomics (ch.21) need -pthread at link time;
#     build-essential's libc6-dev already includes pthread support, no
#     separate package needed on Ubuntu (this matches how it worked on
#     Fedora too — glibc-devel already covered it there).
#   - linux-tools-generic: Ubuntu's package for `perf`, relevant to ch.16
#     (Performance) — remember perf inside Docker needs --cap-add=SYS_ADMIN
#     and --security-opt seccomp=unconfined at `docker run` time to
#     actually work, not just install (same lesson learned the hard way
#     in The Art of Efficient Programs container).
RUN apt-get update && apt-get install -y \
        build-essential \
        clang \
        gdb \
        valgrind \
        binutils \
        git \
        wget \
        tar \
        gzip \
        diffutils \
        patch \
        man-db \
        manpages-dev \
        vim \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
CMD ["/bin/bash"]