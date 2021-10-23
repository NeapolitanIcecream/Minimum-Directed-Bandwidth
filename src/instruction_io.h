#ifndef SRC_INSTRUCTIONIO_H
#define SRC_INSTRUCTIONIO_H
#include <vector>
#include <string>
#include <functional>
#define IO_DEBUG (true)
class InstructionManager {
public:
    InstructionManager() = default;
    InstructionManager(const std::vector<std::string> &instructSet) : instructions(instructSet) {}
    void ForEachInstruction(std::function<void(const std::string&)> &visitor) {
        for (auto &instruct : instructions) {
            visitor(instruct);
        }
    }
    size_t GetSize() const {
        return instructions.size();
    }

protected:
    std::vector<std::string> instructions;
};
class InputInstructionManager : public InstructionManager {
public:
    InputInstructionManager(std::vector<std::string> &instructSet) : InstructionManager(instructSet) {}
    InputInstructionManager(const std::string &fileName) {
        ResolveFile(fileName);
    }
private:
    void ResolveFile(const std::string &fileName);
};

class OutputInstructionManager : public InstructionManager {
public:
    OutputInstructionManager(std::vector<std::string> &instructSet) : InstructionManager(instructSet) {}
    void DumpToFile(const std::string &fileName) const;
};
#endif // SRC_INSTRUCTIONIO_H
