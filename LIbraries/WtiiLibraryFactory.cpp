#include "WtiiLibraryFactory.h"
#include "../Utils.h"
#include <iostream>
#include <map>
#include <dlfcn.h>
#include <csignal>


void WtiiLibraryFactory::LoadLibraries(const std::string& path) {
    // Check whether file exists
    if (access(path.c_str(), F_OK) == -1) {
        std::cerr << "Error loading library: file does not exist" << std::endl;
        return;
    }

    std::string fullpath = Utils::GetAbsolutePath(path);
    lib_handle = dlopen(fullpath.c_str(), RTLD_NOW);

    if (!lib_handle) {
        // handle error, the library could not be loaded
        std::cerr << "Error loading library (dlopen): " << path << std::endl;
        system("pwd");
        std::cerr << dlerror() << std::endl;
    }

    // reset errors
    dlerror();

    createLibrary = (CreateLibraryFn)dlsym(lib_handle, "create");

    // check for errors
    char *dlsym_error = dlerror();

    if (dlsym_error) {
        // handle error, the function could not be loaded
        std::cerr << "Error loading symbol (dlsym): create" << std::endl;
        std::cerr << dlsym_error << std::endl;
    }
}

IWtiiLibrary *WtiiLibraryFactory::createInstance() {
    return createLibrary();
}
