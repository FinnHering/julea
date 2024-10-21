target "docker-metadata-action" {
  platforms = ["linux/amd64"]
}

variable "BASE_TAG" {
  default = "ghcr.io/finnhering/julea-prebuilt"
}

group "ubuntu" {
  targets = ["ubuntu-spack", "ubuntu-system", "ubuntu-latest"]
}

# Target build docker images with prebuild julea + dependencies using spack method
target "ubuntu-spack" {
  name     = "julea-ubuntu-${versions}-04-spack-${compilers}-${targets}"
  inherits = ["docker-metadata-action"]
  matrix = {
    versions  = ["24", "22", "20"]
    compilers = ["gcc", "clang"]
    targets = ["julea", "julea_dependencies"]
  }
  args = {
    UBUNTU_VERSION       = "${versions}.04"
    JULEA_SPACK_COMPILER = compilers
    CC                   = compilers
  }

  # Add -dependencies to the tag if the target is julea_dependencies
  tags       = ["${BASE_TAG}-spack${targets == "julea_dependencies" ? "-dependencies": ""}:ubuntu-${versions}-04-${compilers}"]
  target     = "julea"
  dockerfile = "Dockerfile.spack"
  cache-from = [
    "type=gha,scope=julea-ubuntu-${versions}-04-spack-${compilers}"
  ]
  cache-to = [
    "type=gha,scope=julea-ubuntu-${versions}-04-spack-${compilers},mode=max"
  ]
}

# Target build docker images with prebuild julea + dependencies using system method
target "ubuntu-system" {
  name     = "julea-ubuntu-${versions}-04-system-${compilers}-${targets}"
  inherits = ["docker-metadata-action"]
  matrix = {
    versions  = ["24", "22"]
    compilers = ["gcc", "clang"]
    targets = ["julea", "julea_dependencies"]
  }
  args = {
    UBUNTU_VERSION = "${versions}.04"
    CC             = compilers
  }
  tags       = ["${BASE_TAG}-system${targets == "julea_dependencies" ? "-dependencies": ""}:ubuntu-${versions}-04-${compilers}"]
  target     = "julea"
  dockerfile = "Dockerfile.system"

  cache-from = [
    "type=gha,scope=julea-ubuntu-${versions}-04-spack-${compilers}"
  ]
  cache-to = [
    "type=gha,scope=julea-ubuntu-${versions}-04-spack-${compilers},mode=max"
  ]
}

# Build latest image
target "ubuntu-latest" {
  inherits = ["docker-metadata-action"]
  args = {
    UBUNTU_VERSION = "24.04"
    CC             = "gcc"
  }
  tags       = ["${BASE_TAG}:ubuntu-latest"]
  target     = "julea_spack"
  dockerfile = "Dockerfile.system"

  cache-from = [
    "type=gha,scope=julea-ubuntu-24-04-spack-gcc"
  ]
  cache-to = [
    "type=gha,scope=julea-ubuntu-24-04-spack-gcc,mode=max"
  ]
}

// # Build dependencies spack
// target "ubuntu-dependencies" {
//   inherits = ["ubuntu-spack"]
//   target   = "julea_dependencies"
//   tags     = ["${BASE_TAG}-spack-dependencies:ubuntu-${versions}-04-${compilers}"]
// }

// # Build dependencies system
// target "ubuntu-dependencies" {
//   inherits = ["ubuntu-system"]
//   target   = "julea_dependencies"
//   tags     = ["${BASE_TAG}-system-dependencies:ubuntu-${versions}-04-${compilers}"]
// }