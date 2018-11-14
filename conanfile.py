from conans import ConanFile, CMake
from pathlib import Path
import os

homedir = Path.home()
conan_dir = os.path.join(homedir, ".conan")
conan_bin_dir = os.path.join(conan_dir, "bin")
if not os.path.exists(conan_bin_dir):
    os.mkdir(conan_bin_dir)


class ElvesConan(ConanFile):
    name = "elves"
    version = "0.1.0"
    license = "MIT"
    author = "kaiyin keenzhong@qq.com"
    url = "https://github.com/elves.git"
    description = "Inspect ELF files"
    topics = ("elf", "binary")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    requires = "libelf/0.8.13@bincrafters/stable"
    generators = "cmake"
    exports_sources = "src/*"

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/hello %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.h", dst="include", src="src")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)
        self.copy("*", src="bin", dst="bin", keep_path=False)

    def deploy(self):
        self.copy("*", src=bin, dst=conan_bin_dir)

    def package_info(self):
        self.do_package_info()
        if self.options.system:
            self.cpp_info.includedirs = ["/usr/include"]
            self.cpp_info.libdirs = ["/usr/lib/x86_64-linux-gnu/libbsd.so"]
            if len(str(self.options.root)) != 0:
                self.cpp_info.includedirs.append(
                    str(self.options.root) + "/include")
                self.cpp_info.libdirs.append(str(self.options.root) + "/lib")
