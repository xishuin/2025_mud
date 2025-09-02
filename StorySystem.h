#ifndef STORYSYSTEM_H
#define STORYSYSTEM_H
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <functional>
#include "Person_Skill_Class/Person.h"
#include "Task.h"
// 提前声明Player类
class Player;
// 剧情节点类
struct Choice {
    std::string text;
    int nextNode;
    Task choice_handel;
    Condition Selection_Detection_Handler;
    Choice(std::string _t,int _n):text(std::move(_t)),nextNode(_n),choice_handel([]()->void{return;}),Selection_Detection_Handler([]()->bool{return true;}){}
    void Set_Selection_Detection_Handler(Condition _cond) {
            Selection_Detection_Handler=_cond;
    }
    void Set_choice_handel(Task _t) {
        choice_handel=_t;
    }
};

class StoryNode {
private:
    int id;
    std::string text;
    std::vector<Choice*> choices; // 选项文本和下一个节点ID
    Condition condition; // 触发条件
    bool isTerminal; // 是否为结束节点

public:
    StoryNode(int id, const std::string& text, bool isTerminal = false)
        : id(id), text(text), isTerminal(isTerminal) {}

    // 添加选项
    void addChoice(const std::string& choiceText, int nextNodeId) {
        choices.push_back(new Choice(choiceText,nextNodeId));
    }
    Choice* GetChoice(int id) {
        return choices[id-1];
    }
    // 设置触发条件
    void setCondition(Condition cond) {
        condition = cond;
    }

    // 获取节点ID
    int getId() const { return id; }

    // 获取节点文本
    std::string getText() const { return text; }

    // 获取选项列表
    const std::vector<Choice*>& getChoices() const {
        return choices;
    }

    // 检查是否满足触发条件
    bool checkCondition() const {
        return condition ? condition() : true;
    }

    // 是否为结束节点
    bool isEnd() const { return isTerminal; }
};

// 剧情系统类
class StorySystem {
private:
    std::vector<StoryNode> nodes;
    int currentNodeId;
    bool storyActive;
    int selectedChoice;

public:
    StorySystem() : currentNodeId(0), storyActive(false), selectedChoice(-1) {}

    // 添加剧情节点
    void addNode(const StoryNode& node) {
        nodes.push_back(node);
    }

    // 开始剧情
    void startStory(int startNodeId) {
        currentNodeId = startNodeId;
        storyActive = true;
        selectedChoice = -1;
    }

    // 选择选项
    void chooseOption(int optionIndex) {
        if (!storyActive) return;

        const auto& currentNode = getCurrentNode();
        if (optionIndex >= 0 && optionIndex < currentNode.getChoices().size()) {
            currentNodeId = currentNode.getChoices()[optionIndex]->nextNode;
            selectedChoice = optionIndex;
        }
    }
    // 获取当前节点
    const StoryNode& getCurrentNode() const {
        for (const auto& node : nodes) {
            if (node.getId() == currentNodeId) {
                return node;
            }
        }
        // 如果找不到节点，返回第一个节点
        return nodes.empty() ? StoryNode(-1, "") : nodes[0];
    }
    // 剧情是否活跃
    bool isActive() const { return storyActive; }
    // 结束剧情
    void endStory() {
        storyActive = false;
    }
    // 获取当前选择
    int getSelectedChoice() const { return selectedChoice; }
};

#endif // STORYSYSTEM_H