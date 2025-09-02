#ifndef STORYSYSTEM_H
#define STORYSYSTEM_H
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <functional>
#include "Person_Skill_Class/Person.h"
// 提前声明Player类
class Player;
// 剧情节点类
struct Choice {

    std::string text;
    int nextNode;
    std::function<void(const Player&)> choice_handel;
    Choice(std::string _t,int _n):text(std::move(_t)),nextNode(_n),choice_handel(nullptr){}
};

class StoryNode {
private:
    int id;
    std::string text;
    std::vector<Choice*> choices; // 选项文本和下一个节点ID
    std::function<bool(const Person&)> condition; // 触发条件
    bool isTerminal; // 是否为结束节点

public:
    StoryNode(int id, const std::string& text, bool isTerminal = false)
        : id(id), text(text), isTerminal(isTerminal) {}

    // 添加选项
    void addChoice(const std::string& choiceText, int nextNodeId) {
        choices.push_back(new Choice(choiceText,nextNodeId));
    }
    void SetHandleChoiceEvent(int choice,std::function<void(const Person&)> Handel) {
            choices[choice]->choice_handel=Handel;
    }
    // 设置触发条件
    void setCondition(std::function<bool(const Person&)> cond) {
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
    bool checkCondition(const Person& player) const {
        return condition ? condition(player) : true;
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

    // 触发地图剧情检查
    bool checkMapTrigger(int x, int y, const Player& player) {
        // 这里可以根据坐标和玩家状态检查是否触发剧情
        // 示例：当玩家到达(10,10)且等级大于5时触发剧情节点1
        if (x == 10 && y == 10 && player.GetLevel() > 5) {
            startStory(1);
            return true;
        }
        return false;
    }

    // 触发人物剧情检查
    bool checkNPCTrigger(int npcId, const Person& player) {
        // 这里可以根据NPC ID和玩家状态检查是否触发剧情
        // 示例：当与NPC 1对话且拥有特定物品时触发剧情节点2
        if (npcId == 1 /* && 玩家拥有特定物品 */) {
            startStory(2);
            return true;
        }
        return false;
    }

};

#endif // STORYSYSTEM_H