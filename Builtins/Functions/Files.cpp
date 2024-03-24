#include <fstream>
#include <filesystem>
#include "Files.h"
#include "../../Utils.h"
#include "../../ParsedScripts/Values/ArrayValue.h"
#include "../../Interpreter/Interpreter.h"

Value *Files::ReadFileAsText(std::vector<Value *> args) {
    auto* path = dynamic_cast<Constant*>(args[0]);

    if (path == nullptr) {
        throw_err("Not constant val");
    }

    std::ifstream file(path->Value);
    std::string lines;
    std::string line;
    while (std::getline(file, line)) {
        lines += line + "\n";
    }
    file.close();

    lines.pop_back();
    return new Constant(lines, "string");
}

Value *Files::WriteTextToFile(std::vector<Value *> args) {
    auto* path = dynamic_cast<Constant*>(args[0]);
    auto* text = dynamic_cast<Constant*>(args[1]);

    if (path == nullptr || text == nullptr) {
        throw_err("Not constant val");
    }

    std::ofstream file(path->Value);
    file << text->Value;
    file.close();

    return new Constant("null", "null");
}

Value *Files::FileExists(std::vector<Value *> args) {
    auto* path = dynamic_cast<Constant*>(args[0]);

    if (path == nullptr) {
        throw_err("Not constant val");
    }

    std::ifstream file(path->Value);
    return new Constant(file.good() ? "true" : "false", "bool");
}

Value *Files::DeleteFile(std::vector<Value *> args) {
    auto* path = dynamic_cast<Constant*>(args[0]);

    if (path == nullptr) {
        throw_err("Not constant val");
    }

    std::remove(path->Value.c_str());
    return new Constant("null", "null");
}

Value *Files::GetFiles(std::vector<Value *> args) {
    auto* path = dynamic_cast<Constant*>(args[0]);

    if (path == nullptr) {
        throw_err("Not constant val");
    }

    std::vector<std::string> files;
    for (const auto & entry : std::filesystem::directory_iterator(path->Value)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().string());
        }
    }

    auto* arr = new ArrayValue();
    for (const auto & file : files) {
        arr->Values.push_back(new Constant(file, "string"));
    }

    return arr;
}

Value *Files::GetDirectories(std::vector<Value *> args) {
    auto* path = dynamic_cast<Constant*>(args[0]);

    if (path == nullptr) {
        throw_err("Not constant val");
    }

    std::vector<std::string> files;
    for (const auto & entry : std::filesystem::directory_iterator(path->Value)) {
        if (entry.is_directory()) {
            files.push_back(entry.path().string());
        }
    }

    auto* arr = new ArrayValue();
    for (const auto & file : files) {
        arr->Values.push_back(new Constant(file, "string"));
    }

    return arr;
}

Value *Files::CreateDirectory(std::vector<Value *> args) {
    auto* path = dynamic_cast<Constant*>(args[0]);

    if (path == nullptr) {
        throw_err("Not constant val");
    }

    std::filesystem::create_directory(path->Value);
    return new Constant("null", "null");
}

Value *Files::DeleteDirectory(std::vector<Value *> args) {
    auto* path = dynamic_cast<Constant*>(args[0]);

    if (path == nullptr) {
        throw_err("Not constant val");
    }

    std::filesystem::remove_all(path->Value);
    return new Constant("null", "null");
}

Value *Files::DirectoryExists(std::vector<Value *> args) {
    auto* path = dynamic_cast<Constant*>(args[0]);

    if (path == nullptr) {
        throw_err("Not constant val");
    }

    return new Constant(std::filesystem::exists(path->Value) ? "true" : "false", "bool");
}

Value *Files::GetCurrentDirectory(const std::vector<Value *>& args) {
    return new Constant(std::filesystem::current_path().string(), "string");
}

Value *Files::SetCurrentDirectory(const std::vector<Value *> &args) {
    auto* path = dynamic_cast<Constant*>(args[0]);

    if (path == nullptr) {
        throw_err("Not constant val");
    }

    std::filesystem::current_path(path->Value);
    return new Constant("null", "null");
}

// Returns the full path from a relative path, not tested, probably inconsistent with C# implementation
Value *Files::GetFullPath(const std::vector<Value *> &args) {
    RealReference* ar = Interpreter::ResolveValue(args[0]);
    auto constant = dynamic_cast<Constant*>(ar);
    if (constant == nullptr) {
        throw_err("Not constant val");
    }
    if (constant->ObjectType != "string") {
        throw_err("Not string val");
    }

    return new Constant(Utils::GetAbsolutePath(constant->Value), "string");
}
