#include <stdexcept>
#include "Convert.h"
#include "../../Utils.h"
#include "../../ParsedScripts/Values/Constant.h"

Value *Convert::ToInt(std::vector<Value *> args) {
    auto* constant = dynamic_cast<Constant*>(args[0]);

    if (constant == nullptr) {
        throw_err("Not constant val");
    }

    int val;
    try {
        val = std::stoi(constant->Value);
    } catch (std::invalid_argument&) {
        throw_err("Invalid value");
    } catch (std::out_of_range&) {
        throw_err("Value out of range");
    }

    return new Constant(std::to_string(val), "int");
}

Value *Convert::ToFloat(std::vector<Value *> args) {
    auto* constant = dynamic_cast<Constant*>(args[0]);

    if (constant == nullptr) {
        throw_err("Not constant val");
    }

    float val;
    try {
        val = std::stof(constant->Value);
    } catch (std::invalid_argument&) {
        throw_err("Invalid value");
    } catch (std::out_of_range&) {
        throw_err("Value out of range");
    }

    return new Constant(std::to_string(val), "float");
}
