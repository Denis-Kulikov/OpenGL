#include <dynamic_compiler/dynamic_compiler.hpp>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>
#include <clang/Tooling/Tooling.h>
#include <memory>
#include <iostream>

using namespace llvm;
using namespace llvm::orc;

struct DynamicCompiler::Impl {
    std::unique_ptr<LLJIT> jit;

    Impl() {
        InitializeNativeTarget();
        InitializeNativeTargetAsmPrinter();

        llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);

        auto jitExpected = LLJITBuilder().create();
        if (!jitExpected) {
            llvm::errs() << "Failed to create JIT: " << toString(jitExpected.takeError()) << "\n";
            exit(1);
        }
        
        jit = std::move(*jitExpected);

        auto DLSG = DynamicLibrarySearchGenerator::GetForCurrentProcess(
            jit->getDataLayout().getGlobalPrefix()
        );

        if (!DLSG) {
            llvm::errs() << "Failed to create symbol generator: "
                        << toString(DLSG.takeError()) << "\n";
            exit(1);
        }

        jit->getMainJITDylib().addGenerator(std::move(*DLSG));
    }

    bool addModule(std::unique_ptr<llvm::Module> M, std::unique_ptr<llvm::LLVMContext> Ctx) {
        auto TSM = ThreadSafeModule(std::move(M), std::move(Ctx));
        if (auto err = jit->addIRModule(std::move(TSM))) {
            llvm::errs() << "Failed to add module: " << toString(std::move(err)) << "\n";
            return false;
        }
        return true;
    }

    void* lookup(const std::string &name) {
        auto sym = jit->lookup(name);
        if (!sym) {
            llvm::errs() << "Symbol lookup failed: " << toString(sym.takeError()) << "\n";
            return nullptr;
        }

        return sym->toPtr<void*>();
    }
};

DynamicCompiler::DynamicCompiler() : impl(new Impl()) {}

DynamicCompiler::~DynamicCompiler() {
    delete impl;
}

bool DynamicCompiler::compile(const std::string &filename) {
    const std::string objectFile = "temp_dynamic_code.o";

    // Компиляция через clang++
    #ifdef _WIN32
        std::string command = "clang++ -O0 -c " + filename + " -o " + objectFile;
        command += " -Iinclude";  // Для директории include
        command += " -Iassimp/include";  // Для Assimp
        command += " -Ibuild/_deps/bullet-src/src";  // Для Bullet
        command += " -I\"C:/Program Files/LLVM/include\"";  // Для LLVM на Windows
        command += " -Ibuild/_deps/stb_image-src";  // Для stb_image
        command += " -Ibuild/_deps/glm-src";  // Для glm
    #else
        std::string command = "clang++ -O0 -fPIC -c " + filename + " -o " + objectFile;
        command += " -Iinclude";  // Для директории include
        command += " -I/usr/include/bullet";  // Для Bullet на Linux
        command += " -Ibuild/_deps/stb_image-src";  // Для stb_image
        command += " -Ibuild/_deps/glm-src";  // Для glm
    #endif

    if (std::system(command.c_str()) != 0) {
        std::cerr << "clang++ compilation failed\n";
        return false;
    }

    // Загрузка объектного файла
    auto bufferOrErr = llvm::MemoryBuffer::getFile(objectFile);
    if (!bufferOrErr) {
        llvm::errs() << "Failed to read compiled object file\n";
        return false;
    }

    auto objBuffer = std::move(*bufferOrErr);
    auto obj = llvm::object::ObjectFile::createObjectFile(objBuffer->getMemBufferRef());
    if (!obj) {
        llvm::errs() << "Failed to parse object file.\n";
        return false;
    }

    if (auto err = impl->jit->addObjectFile(std::move(objBuffer))) {
        llvm::errs() << "Failed to add object file to JIT: " << toString(std::move(err)) << "\n";
        return false;
    }

    return true;
}

void* DynamicCompiler::getSymbol(const std::string &symbolName) {
    return impl->lookup(symbolName);
}
