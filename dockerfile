FROM ubuntu:24.04 AS builder

# Install essential tools first (including wget and gpg)
RUN apt update && apt install -y \
    build-essential \
    curl \
    wget \
    unzip \
    gnupg \
    software-properties-common \
    lsb-release \ 
    autoconf \
    libtool \
    pkg-config

# Add LLVM APT key (modern method)
RUN wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key | gpg --dearmor -o /usr/share/keyrings/llvm-archive-keyring.gpg

# Add LLVM repo for Ubuntu 24.04 (noble) + Clang 20
RUN echo "deb [signed-by=/usr/share/keyrings/llvm-archive-keyring.gpg] http://apt.llvm.org/noble/ llvm-toolchain-noble-20 main" \
    > /etc/apt/sources.list.d/llvm.list

# Now install Clang 20 and deps
RUN apt update && apt install -y \
    clang-20 \
    clang-format-20 \
    clang-tidy-20 \
    libc++-20-dev \
    libc++abi-20-dev \
    libglfw3-dev \
    libvulkan-dev \
    libvulkan1 \
    mesa-vulkan-drivers \
    openjdk-17-jdk \
    protobuf-compiler \
    git \
    wmctrl \
    cmake

# Set Clang 20 as compiler
ENV CC=clang-20
ENV CXX=clang++-20

# Install latest protobuf from GitHub
RUN cd /tmp && \
    curl -L -o protoc.zip https://github.com/protocolbuffers/protobuf/releases/download/v33.4/protoc-33.4-linux-x86_64.zip && \
    unzip -o protoc.zip -d /usr/local && \
    rm protoc.zip && \
    protoc --version

# Install Bazelisk
RUN curl -L https://github.com/bazelbuild/bazelisk/releases/download/v1.28.0/bazelisk-linux-amd64 -o /usr/local/bin/bazelisk \
    && chmod +x /usr/local/bin/bazelisk \
    && ln -s /usr/local/bin/bazelisk /usr/local/bin/bazel

# WORKDIR /workspace
# COPY . .

# # Generate compile_commands.json
# RUN bazel run @hedron_compile_commands//:refresh_all

# # Build
# RUN bazel build //:main 
# #--verbose_failures

# # Runtime stage
# FROM ubuntu:24.04
# RUN apt update && apt install -y libglfw3 libvulkan1
# COPY --from=builder /workspace/bazel-bin/main /main

# # Export stage - only contains the binary, compile_commands, and headers
# FROM ubuntu:24.04 AS export
# COPY --from=builder /workspace/bazel-bin/main /bin/main
# COPY --from=builder /workspace/compile_commands.json /build/compile_commands.json
# COPY --from=builder /root/.cache/bazel/_bazel_root/*/external/+_repo_rules+imgui/*.h /build/include/imgui/
# COPY --from=builder /root/.cache/bazel/_bazel_root/*/external/+_repo_rules+imgui/backends/*.h /build/include/imgui/backends/