#include "StorySystem.h"
#include <iostream>
#include <map>
#include "../../Fight/Fight.h"
#include "../../draw/MainShell.h"
#include "../../Role/Person_Skill_Class/Person.h"
#include "../../Role/Person_Skill_Class/monster.h"
#include "../../map/Map.h"
#include "../../map/map_data/map.h"

// 声明外部全局变量
extern std::map<int, Person> boss;
extern std::map<int, Person> monsters;
extern Player player;


// 定义全局剧情系统
StorySystem g_storySystem;

// 重置游戏数据函数
extern void InitMapSystem();
void ResetGameData() {
    // 重置怪物和Boss数据
    std::string monsterFilePath = "../Role/Person_Skill_Class/monster.json";
    std::string bossFilePath = "../Role/Person_Skill_Class/boss.json";
    std::string eventPath = "..\\map\\map_data\\event.json";
    monsters = loadEventMonstersMap(eventPath, monsterFilePath);
    boss = loadBossFromJson(bossFilePath);
    
    // 重新初始化地图系统
    InitMapSystem();
}

// 初始化剧情系统
void InitStorySystem() {


    StoryNode node0(0, "【场景：帝都 - 魔法学院顶楼】\n"
        "站在帝国魔法学院的顶楼，俯瞰着繁华的帝都。你，罗汀·奥德瑞恩，是帝国的首席魔法师，拥有令人艳羡的天赋和财富。然而，你胸口的黑色结晶纹路正在隐隐作痛，那是你强行激活禁忌水晶后留下的\"诅咒\"，你的力量正在不可逆转地流失。\n"
        "艾利克斯（担忧地）：罗汀！你的状态越来越差了。我查阅了所有古籍，只有传说中的\"始源之种\"才能稳定你的魔力。它位于迷雾森林深处的精灵遗迹中。你必须立刻动身！");
    node0.addChoice("1.前往迷雾森林 - 精灵遗迹", 1);
    node0.addChoice("2.蛐蛐诅咒，不足为惧，不接受建议", 100); // 指向新节点100
    g_storySystem.addNode(node0);



    // 新增节点100：拒绝建议的结局
    StoryNode node100(100, "你发现自己的生命力正缓缓流逝，不见好转，生命消散在了家里的床上\n恭喜您达成成就【头铁的一生】");
    node100.addChoice("1.重新开始", 0); // 回到初始节点
    node100.GetChoice(1)->Set_choice_handel(ResetGameData);
    g_storySystem.addNode(node100);


    // 节点1: 洞穴探险
    StoryNode node1(1, "【场景：迷雾森林 - 精灵遗迹入口】\n "
        "你来到迷雾森林的入口，雾气缭绕，危机四伏。一位精灵游侠拦住了你的去路。\n"
        "【精灵游侠莉娜】（警惕地）：站住！人类。精灵圣地不是人类配进入的地方！2");
    node1.addChoice("1. 让开！帝国首席魔法师的事，轮不到你一个小小游侠管。", 4);  // 注意：这里原本指向节点4，但我们需要先处理支线
    node1.addChoice("2. 你选择陈述你的糟糕境遇，并拿出了早些年得到的精灵族的族徽。我需要进入遗迹寻找‘始源之种’来救治我的伤势，希望能得到你的帮助", 5);
    g_storySystem.addNode(node1);



    // 节点2: 接受帮助
    StoryNode node2(2, "莉娜：虽然不知道你是怎么得到它的，但拥有它，足以证明你得到过精灵圣地的认可。拿着它，你可以安全进入遗迹深处。");
    node2.addChoice("1. 感谢莉娜，前往遗迹深处", 6);
    g_storySystem.addNode(node2);



    // 节点3: 拒绝帮助的结局
    StoryNode node3(3, "【结局：傲慢的代价】\n你的傲慢激怒了莉娜，她对你施加了永久的诅咒。你永远无法进入精灵遗迹，也无法找到‘始源之种’。最终，你因魔力失控而痛苦地死去...\n\n【游戏结束】");
    g_storySystem.addNode(node3);


    // 新增：节点4（傲慢选择的后续）


    StoryNode node4(4, "莉娜（眉头紧锁，眼神冰冷）：傲慢的人类！果然，我不该跟你多费口舌。");
    node4.addChoice("1. 选择离开，随后自己悄悄闯进森林", 40);
    node4.addChoice("2. （沉默片刻）突然动手袭击对方", 41);
    node4.addChoice("3. （反思）“……你说得对。我为我的言行道歉。我不该把我的急躁发泄在你身上。请告诉我，我能做些什么来弥补？", 42);
    g_storySystem.addNode(node4);




    // 节点40：继续傲慢的结局

    StoryNode node40(40, "你因为你的傲慢迷失在了森林的大雾中，最终彻底在世间不见了踪影。\n"
        "恭喜您达成成就：【寻宝未始身先死】");
    // 结束或循环
    node40.addChoice("1. 重新开始", 0);
    node40.GetChoice(1)->Set_choice_handel(ResetGameData);
    g_storySystem.addNode(node40);



    // 节点41：攻击的结局

    StoryNode node41(41, "你尝试攻击对方，但却高估了自己如今的实力。\n"
        "你的诅咒解除，但伴随着的也是你生命的消散。");
    // 结束或循环
    node41.addChoice("1. 重新开始", 0);
    node41.GetChoice(1)->Set_choice_handel(ResetGameData);
    g_storySystem.addNode(node41);

    // 节点42：选择反思，触发支线任务

    StoryNode node42(42, "莉娜（表情稍缓，但仍很严肃）：“弥补？行动比言语更有力。在森林的东边，一群贪婪的哥布林抓走了几只林妖的幼崽，它们的生命很脆弱。如果你真的想证明自己，就去救它们吧。记住，不是为了获得我的感谢，而是为了那些无辜的生命。”\n"
        "【触发支线任务：“破碎的信任”】\n"
        "任务内容：前往哥布林巢穴，解救精灵幼仔");
    node42.addChoice("1. 前往哥布林巢穴", 43);
    node42.GetChoice(1)->Set_choice_handel([&]() {
        player.SetChapter(2);
    });
    g_storySystem.addNode(node42);




    // 节点43：哥布林巢穴入口

    StoryNode node43(43, "【场景：哥布林巢穴】\n"
        "阴暗潮湿，绿光闪烁，哥布林巢穴危机四伏。");
    node43.addChoice("1. 瞧不起哥布林，直接冲进去", 44);
    node43.addChoice("2. 小心为上，选择谨慎进入巢穴", 45);
    g_storySystem.addNode(node43);




    // 节点44：直接冲进去

    StoryNode node44(44, "！！你不小心触发了哥布林布置的陷阱，受到了伤害。");
    // 这里可以扣血，然后继续
    // 假设继续到节点45（谨慎进入的后续）
    node44.addChoice("1. 继续前进", 45);
    g_storySystem.addNode(node44);




    // 节点45：巢穴深处

    StoryNode node45(45, "你顺利进入了哥布林巢穴的深处，看到被绑在一起的精灵幼仔时，哥布林们气势汹汹地围了上来。");
    node45.addChoice("1. 进入战斗", 46);
    //《《这里需要跳转至战斗界面进行战斗》》！！！！！
    node45.addChoice("2. 看到面前气势汹汹的哥布林，你转头想跑", 47);
    g_storySystem.addNode(node45);





    // 节点46：战斗（假设战斗胜利）
    StoryNode node46(46, "你击败了哥布林，成功解救了精灵幼仔。");
    // 任务完成，跳转至莉娜的信任修复（即节点5的内容）
    node46.addChoice("1. 返回莉娜处", 5);  // 注意：这里直接跳转到节点5（信任修复后的对话）
    g_storySystem.addNode(node46);


    // 节点47：逃跑的结局
    StoryNode node47(47, "！！你在跑出巢穴的途中触发了哥布林的陷阱，逃脱不及时，被哥布林包围。\n"
        "恭喜您达成成就【不要让战斗停下来啊】");
    // 结束或循环
    node47.addChoice("1. 重新开始", 0);
    g_storySystem.addNode(node47);


    // 注意：节点5是原本的选择2（友善选项）的后续，现在也作为支线任务完成后的跳转目标
    StoryNode node5(5, "莉娜对你的态度缓和。\n莉娜（点头）：“好吧，我信你。但遗迹深处很危险，我不能陪你进去。这是进入圣地的信物，拿好。”");
    node5.addChoice("1. 前往遗迹深处", 6);
    g_storySystem.addNode(node5);

    StoryNode node6(6,
        "你在遗迹深处找到了一块巨大的水晶基座，但上面空空如也。不知何时，一位年长的精灵守护者走了过来。你认出了曾经一起冒险过的他，并讲述了自己的事情\n"
        "\n【精灵守护者】\n"
        "守护者（叹息）： 你还是这么看重力量\"始源之种\"早已破碎，被分散到了大陆的四个角落。你需要集齐它们的力量，才能重塑它。\""
        "据我所知，\"始源之种\"被分为四块碎片，分别位于矮人王国的熔火之心、兽人部落的遗忘高地、人类帝国的帝都、天空之城的圣山之巅，抓紧出发吧");
    node6.addChoice("1.前往最近的【矮人王国 - 熔火之心】", 7);
    node6.GetChoice(1)->Set_choice_handel([&]() {
        player.SetChapter(3);
    });
    g_storySystem.addNode(node6);



    StoryNode node7(7, "【场景：矮人王国 - 熔火之心】\n"\

        "你来到矮人王国的锻炼圣地——熔火之心，这里热浪滚滚。矮人国王正为矿洞塌方而烦恼。你缓步上前，说明自己的来意\n"
        "【矮人国王】: \"该死的！矿洞又塌了，我的工匠们都被困在里面了！更糟糕的是，通往矿洞深处的'熔火钥匙'被熔岩巨兽看守着，没有它就无法打开加固通道的机关！你要是能帮我拿到钥匙并救出他们，我就把'大地碎片'给你！\"");
    node7.addChoice("1.接受任务，前往矿洞深处", 8);
    g_storySystem.addNode(node7);


    
    // 在选择进入矿洞深处时触发Boss战斗
    node7.GetChoice(1)->Set_choice_handel([&]() {
        // 查找熔岩巨兽怪物数据
        auto it = boss.find(1); // 熔岩巨兽的ID为1
        if (it != boss.end()) {
            int fightResult = Fight(player, it->second, true);
            // 只有战斗胜利才推进到节点8
            if (fightResult == 1) {
				player.ChangeEXP(600); // 战斗胜利后获得经验
                g_storySystem.startStory(8);
            }
            // 如果逃跑成功，返回到节点7
            else if (fightResult == 2) {
                g_storySystem.startStory(7);
            }
        }
    });

    StoryNode node8(8, "你勇敢地深入矿洞，眼前出现了一只巨大的熔岩巨兽，它的身体由流动的岩浆构成，散发着炽热的气息。\n"
        "经过一场激烈的战斗，你成功击败了熔岩巨兽，获得了'熔火钥匙'。\n"
        "你使用钥匙打开了加固通道的机关，成功救出了被困的工匠们。\n"
        "矮人国王感激地将'大地碎片'交给你：\"这是你应得的报酬，勇敢的人类！熔岩巨兽已经困扰我们很久了，你为我们解决了一个大麻烦！\"");
    node8.addChoice("1.带着大地碎片前往兽人部落", 9);
    node8.addChoice("2.与救出的工匠们交谈", 80);
    node8.GetChoice(1)->Set_choice_handel([&]() {
        player.SetChapter(4);
    });
    g_storySystem.addNode(node8);

    StoryNode node80(80, "工匠们感激地对你说：\"谢谢你救了我们！如果没有你，我们可能就永远被困在这里了。\"\n"
        "其中一位年长的工匠叹叹道：\"听说你要前往兽人部落？那边最近也不太平，你可要小心啊。\"");
    node80.addChoice("1.告别工匠，继续冒险", 9);
    g_storySystem.addNode(node80);

    // 原有的节点9保持不变
    StoryNode node9(9, "【场景：兽人部落 - 遗忘高地】\n"
        "你来到兽人部落的遗忘高地，这里充满了原始的狂野气息。兽人战士们个个肌肉虬结，眼神中透露着对入侵者的警惕，但肚子却咕咕作响。");
    node9.addChoice("1.饥饿的兽人不足为惧，直接抢", 90);  // 改为指向新节点90
    node9.addChoice("2.与兽人们做交易，用食物换取生命碎片", 91);  // 改为指向新节点91
    node9.addChoice("3.与酋长交流，询问情况", 10);  // 保持指向节点10
    g_storySystem.addNode(node9);

    // 新增节点90：战斗支线
    node9.GetChoice(1)->Set_choice_handel([&]() {
        // 查找兽人酋长怪物数据
        auto it = boss.find(8); // 兽人酋长的ID为8
        if (it != boss.end()) {
            int fightResult = Fight(player, it->second, true);
            // 只有战斗胜利才推进到节点90
            if (fightResult == 1) {
				player.ChangeEXP(1400); // 战斗胜利后获得经验
                g_storySystem.gotoStory(90);
            }
            // 如果逃跑成功，返回到节点9
            else if (fightResult == 2) {
                g_storySystem.gotoStory(9);
            }
        }
        });
    StoryNode node90(90, "你与兽人酋长展开战斗\n"
        "你取得了战斗的胜利，自己通过恢复的力量得到了生命碎片，但似乎也失去了什么东西。\n"
        "恭喜您达成成就【再次黑暗的心】");

    node90.addChoice("1.继续前往【人类帝国 - 帝都】", 12);
        // 添加战斗触发代码

    g_storySystem.addNode(node90);



    // 新增节点91：交易支线
    StoryNode node91(91, "【任务内容】：交付一定数量的食物\n"
        "恭喜您通过交易的方式获得'生命碎片'，获得成就【兽人永不为奴？】");
    node91.addChoice("1.继续前往【人类帝国 - 帝都】", 12);
    g_storySystem.addNode(node91);

    // 修改节点10
    StoryNode node10(10, "【兽人酋长】\n"
        "酋长（低吼）：我们的猎场被怪物占据了，大家都要饿死了！如果你能夺回猎场，我就把'生命碎片'给你。\n"
        "【任务：夺回猎场】");
    node10.addChoice("1.接受任务，前往猎场", 92);  // 改为指向新节点92

    // 添加战斗触发代码
    node10.GetChoice(1)->Set_choice_handel([&]() {
        // 查找猎场怪物数据
        // 猎场怪物的ID为5
        auto it = boss.find(5);

        if (it != boss.end()) {
            int fightResult = Fight(player, it->second, false);
            // 只有战斗胜利才推进到节点92
            if (fightResult == 1) {
				player.ChangeEXP(1400); // 战斗胜利后获得经验
                g_storySystem.gotoStory(92);
            }
            // 如果逃跑成功，返回到节点10
            else if (fightResult == 2) {
                g_storySystem.gotoStory(10);
            }
        }
    });

    g_storySystem.addNode(node10);

    // 新增节点92：猎场战斗
    StoryNode node92(92, "你成功击败了占据猎场的怪物，夺回了兽人的猎场。");
    node92.addChoice("1.返回向酋长复命", 11);
    g_storySystem.addNode(node92);

    std::cout << "InitStorySystem: 完成创建node10节点" << std::endl;
    std::cout << "InitStorySystem: 开始创建node11节点" << std::endl;
    // 修改节点11
    StoryNode node11(11, "酋长（敬佩地）：兽人向来尊敬强者，我的朋友，这是你需要的'生命碎片'。\n"
        "酋长将一块散发着柔和绿光的晶体递到你手中，那光芒仿佛有生命般在掌心轻轻脉动。\n"
        "你接过碎片，感受到一股温暖的力量顺着指尖蔓延至全身。这力量并不猛烈，却如涓涓细流，滋养着你疲惫的身躯。\n"
        "【恭喜您获得道具生命碎片】\n"
        "酋长（咧嘴一笑，露出两颗略微发亮的獠牙）：兽人从不亏待真正的勇士。你的勇气和智慧已经赢得了我们的尊重。这片森林是我们的家园，也是你的。朋友，无论何时需要，这里永远有你的位置。\n"
        "恭喜您达成成就【兽人以鱼不如兽人以渔】");
    node11.addChoice("1.即刻前往【人类帝国 - 帝都】", 12);
    node11.addChoice("2.我也许需要休整一下", 93);  // 改为指向新节点93
    node11.GetChoice(1)->Set_choice_handel([&]() {
        player.SetChapter(5);
    });
    g_storySystem.addNode(node11);

    // 新增节点93：休整支线
    StoryNode node93(93, "你在兽人部落稍作休整，感受着生命碎片带来的温暖力量。");
    node93.addChoice("1.休整完毕，前往【人类帝国 - 帝都】", 12);
    g_storySystem.addNode(node93);

    // 后续节点保持不变（节点12及之后的节点）
    StoryNode node12(12, "【场景：人类帝国 - 帝都】\n"
        "你回到了帝都，这个曾经熟悉的地方。你准备进城，却遭到了卫兵的盘查。\n"
        "\"站住！出示你的通行令牌。\"为首的卫兵队长拦住了你的去路，眼神锐利地打量着你。");
    node12.addChoice("1.我是帝国的首席魔法师（亮出自己的令牌），这是我的令牌。", 13);
    node12.addChoice("2.\"我是帝国的首席魔法师，\"你试图用曾经的威严震慑对方，\"奉陛下密令返回，让开！\"", 120);
    g_storySystem.addNode(node12);

    // 新增节点120：小支线选择
    StoryNode node120(120, "\"我是帝国的首席魔法师，\"你试图用曾经的威严震慑对方，\"奉陛下密令返回，让开！\"");
    node120.addChoice("1.身为首席魔法师，你决定用实力展现自己的身份。", 121);
    node120.addChoice("2.你收敛了些，在门口徘徊了一会，发现自己身上还有曾经的首席魔法师令牌。你选择试试。", 13);
    g_storySystem.addNode(node120);

    // 新增节点121：战斗结局
    StoryNode node121(121, "你随后便对卫兵展开了攻击\n"
        "但是寡不敌众，你被团团士兵包围，被后来支援的高阶战士击败。或许，在帝都蛮横不再是你的特权了\n"
        "恭喜您达成成就【吃一堑，长一堑】");
    node121.addChoice("1.重新开始", 0);
    node121.GetChoice(1)->Set_choice_handel(ResetGameData);
    g_storySystem.addNode(node121);

    // 修改节点13
    StoryNode node13(13, "【守门卫兵】（惊恐的）首席…首席大人恕罪！属下有眼不识泰山，请大人恕罪！\n"
        "繁华如织，车水马龙，魔法光芒照亮夜空，商贾云集，人声鼎沸，这位远来的客人，您似乎对眼前的光景并不惊讶\n"
        "你循声看去，说话的是一名金发飘逸，蓝眸灵动，身着华丽丝绒长袍，腰间挂着鲁特琴，举手投足间有着贵族气质与吟游诗人般衣着的青年男子。\n"
        "【丹德里恩】\"也许您曾经到过帝都，但帝都这些年可大变样了，那么，我是说，您是否需要一名导游呢，我的朋友。\"");
    node13.addChoice("1.（支付货币或道具，聘请他）", 130);
    node13.addChoice("2.不理会他，转身就走", 14);
    g_storySystem.addNode(node13);

    // 新增节点130：帝都生活支线
    StoryNode node130(130, "您解锁了地点【皇家宝库】、【旅馆】等地点的信息\n"
        "**【皇家宝库】通过一路上的打听消息，你得知，火焰碎片如今似乎就在宝库里\n"
        "**【旅馆】这只是一家普通的旅馆，普通到曾经的你不会正眼相看\n"
        "**【提示：你可以随时按S键打开通用商店进行交易】");
    node130.addChoice("1.前往【皇家宝库】", 15);
    node130.addChoice("2.前往【旅馆】", 132);
    g_storySystem.addNode(node130);

    // 新增节点132：旅馆
    StoryNode node132(132, "【场景：旅馆】\n"
        "你跟着丹德里恩来到了一家不起眼的小旅馆，推开门，迎面而来的是廉价麦芽酒的淡香味与不知多少人在相互交谈的喧哗声。");
    node132.addChoice("1.我只是来休息的，办理入住，即刻休息", 133);
    node132.addChoice("2.坐下来，看能不能打探到一些有用的消息", 134);
    g_storySystem.addNode(node132);

    // 新增节点133：休息
    StoryNode node133(133, "你在旅馆休息，恢复了体力和状态。");
    node133.addChoice("1.前往【皇家宝库】", 15);
    g_storySystem.addNode(node133);

    // 新增节点134：打探消息
    StoryNode node134(134, "你刚坐下来，就有人来到了你和丹德里恩的桌子前\n"
        "服务员】：（不客气道）丹德里恩，你赊的账再不还，老板可就要把你赶出去了。\n"
        "服务员似乎还没有说完，丹德里恩就打断了他\n"
        "【丹德里恩】：（站起来赔个笑）怎么会不还呢，这是酒钱，先上酒，要好点的\n"
        "服务员忿忿了一声，放下酒就离开了。\n"
        "你刚开始喝酒，就听到了背后传来的议论的声音\n"
        "\"听说了吗，帝都上任了新的魔法师\"\n"
        "\"真的假的，不过还真是，自打几年前帝都魔法学院传出来爆炸声后，就再没见过之前那个家伙了\"\n"
        "\"可不是，不过也好，据说新上任的这个脾气好多了，前一阵子还救了几个人呢\"\n"
        "\"哼哼，我看不一定，之前那个刚上任的时候不也是，装的有模有样，后边还不是走到哪嚣张到哪，都一个鸟样\"\n"
        "你隐约听出了他们谈论的话题，对新的首席魔法师这个消息感到惊讶");
    node134.addChoice("1.同时也为他们口中的曾经的那个自己的所作所为感到羞愧", 135);
    node134.addChoice("2.同时也为他们议论自己的行为感到不满", 136);
    g_storySystem.addNode(node134);

    // 新增节点135：感到羞愧
    StoryNode node135(135, "你决定前去休息，反思自己的过去。");
    node135.addChoice("1.前往【皇家宝库】", 15);
    g_storySystem.addNode(node135);

    // 新增节点136：感到不满
    StoryNode node136(136, "你对他们议论自己的行为感到不满。");
    node136.addChoice("1.选择休息", 133);
    node136.addChoice("2.直接战斗", 137);
    g_storySystem.addNode(node136);

    // 新增节点137：战斗结果
    StoryNode node137(137, "你心生怒火，大打出手，但是你忘记了这里是帝都，这里最不缺的就是对触碰法律者的处罚");//《《这里需要减少金币》》
    node137.addChoice("1.前往【皇家宝库】", 15);
    g_storySystem.addNode(node137);

    // 修改节点14
    StoryNode node14(14, "望着偌大的帝都，你竟一时慌了神，不知前往哪里");
    node14.addChoice("1.前往【皇家金库】", 15);
    node14.addChoice("2.前往【帝都魔法学院】", 140);
    g_storySystem.addNode(node14);

    // 新增节点140：魔法学院支线
    StoryNode node140(140, "你到了记忆中学院的位置，但是这里已经不是记忆中的样子了。你悻悻的离开");
    node140.addChoice("1.前往【皇家金库】", 15);

    // 修改节点15
    g_storySystem.addNode(node140);
    StoryNode node15(15, "【场景：皇家宝库】\n"
        "你来到了一座高耸入云的建筑前，这里守卫森严。你走上前去，在威严的守卫注视下，出示了你的首席魔法师令牌。守卫疑惑了一瞬，但随后为你打开了通往宝库深处的道路。\n"
        "宝库深处灼热逼人，一只巨大的炽焰巨龙盘踞在火焰碎片旁边，它的鳞片闪烁着赤金色光芒，眼神中充满了敌意。");
    node15.addChoice("1.准备战斗", 152); // 先进入战斗准备节点
    // 设置战斗处理
    node15.GetChoice(1)->Set_choice_handel([&]() {
        // 查找炽焰巨龙怪物数据
        auto it = boss.find(2); // 炽焰巨龙的ID为2
        if (it != boss.end()) {
            // 调用Fight函数，设置isBoss为true
            int fightResult = Fight(player, it->second, true);
            // 只有战斗胜利才推进到节点152
            if (fightResult == 1) {
				player.ChangeEXP(3000); // 战斗胜利后获得经验
                g_storySystem.gotoStory(152);
            }
            // 如果逃跑成功，返回到节点15
            else if (fightResult == 2) {
                g_storySystem.gotoStory(15);
            }
        }
        });
    g_storySystem.addNode(node15);

    // 新增节点152：战斗结果节点
    StoryNode node152(152, "经过一场艰苦的战斗，你成功击败了炽焰巨龙，获得了'火焰碎片'。\n"
        "恭喜您获得了道具\"火焰碎片\"");
    node152.addChoice("1.前往旅馆", 150);
   // node152.addChoice("2.前往帝都商店", 151);
    node152.addChoice("2.前往【天空之城-圣山之巅】", 16);
    node152.GetChoice(2)->Set_choice_handel([&]() {
        player.SetChapter(6);
        });
    g_storySystem.addNode(node152);

    // 新增节点150：旅馆选项（需要检测是否开启帝都生活）
    StoryNode node150(150, "你尚未开启帝都生活支线，无法前往旅馆");
    node150.addChoice("1.前往【天空之城-圣山之巅】", 16);
    g_storySystem.addNode(node150);

    ////新增节点151：商店选项（需要检测是否开启帝都生活）
    //StoryNode node151(151, "你尚未开启帝都生活支线，无法前往商店");
    //node151.addChoice("1.前往【天空之城-圣山之巅】", 16);
    //g_storySystem.addNode(node151);

    // 后续节点保持不变（节点16及之后的节点）
    StoryNode node16(16, "【场景：天空之城 - 圣山之巅】\n"
        "你来到天空之城，在圣山之巅，你看到了曾经被你击败的翼人族族长。\n"
        "【翼人族族长】\n"
        "族长（平静地）： \"我一直在等你。我知道'风之碎片'在哪里。它不在我们这里，而在守护它的'天空守护者'体内。击败它，你就能得到碎片。\"\n"
        "【挑战：天空守护者】");
    node16.addChoice("1.挑战天空守护者", 160); // 先进入战斗节点
    // 添加战斗触发代码
    node16.GetChoice(1)->Set_choice_handel([&]() {
        // 查找天空守护者怪物数据
        auto it = boss.find(3); // 天空守护者的ID为3
        if (it != boss.end()) {
            // 调用Fight函数，设置isBoss为true
            int fightResult = Fight(player, it->second, true);
            // 只有战斗胜利才推进到节点160
            if (fightResult == 1) {
				player.ChangeEXP(8000); // 战斗胜利后获得经验
                g_storySystem.startStory(160);
            }
            // 如果逃跑成功，返回到节点16
            else if (fightResult == 2) {
                g_storySystem.startStory(16);
            }
        }
    });
    g_storySystem.addNode(node16);

    // 新增战斗节点160
    StoryNode node160(160, "您成功击败了天空守护者，但感觉眼前闪过一个黑影\n"
        "【翼人族族长】得到了”风之碎片”\n"
        "【翼人族族长】：\n"
        "你在战斗中似乎没有以前那么游刃有余了（举起风之碎片），如果你想要这东西，那就自己来拿（一阵意义不明的鼓点）");
    node160.addChoice("1.进入战斗", 17); // 进入与族长的战斗
    // 添加战斗触发代码
    node160.GetChoice(1)->Set_choice_handel([&]() {
        // 查找翼人族族长怪物数据
        auto it = boss.find(9);
        if (it != boss.end()) {
            int fightResult = Fight(player, it->second, true);
            // 只有战斗胜利才推进到节点17
            if (fightResult == 1) {
				player.ChangeEXP(10000); // 战斗胜利后获得经验
                g_storySystem.startStory(17);
            }
            // 如果逃跑成功，返回到节点160
            else if (fightResult == 2) {
                g_storySystem.startStory(160);
            }
        }
    });
    g_storySystem.addNode(node160);

    // 修改节点17为战斗胜利后的结果
    StoryNode node17(17,  "恭喜您获得了道具”风之碎片”\n"
        "你即刻启程，朝着精灵遗迹出发");
    node17.addChoice("1.一路前进！", 18);
    g_storySystem.addNode(node17);
    // 节点18保持不变
   // 修改节点18
    StoryNode node18(18, "【场景：精灵遗迹 - 中央大厅】\n"
        "你集齐了四块碎片，回到精灵遗迹。将它们放入水晶基座，光芒大盛，\"始源之种\"被重塑。\n"
        "你感受到一股温暖的力量涌入体内，诅咒被解除了。但你的魔力并没有回到巅峰，而是以一种更稳定、更可控的形式存在。\n"
        "【精灵守护者】\n"
        "守护者（微笑）： \"朋友，你找到了。但这并非结束。在这场旅途中，你一定收获了很多。那么，在这之后，你的安排是什么呢？。\"");
    node18.addChoice("1. \"我要回到帝国，重新夺回我的一切！\"", 20);
    node18.addChoice("2. 也许是去兽人部落那帮忙狩猎，也许去矮人那当个工匠，当然，也许留在这。我不确定，也许我应该继续我的旅途，什么时候走不动了，就随处安顿下来吧。我想我不再想要当首席魔法师了。", 21);
    g_storySystem.addNode(node18);

    // 修改节点20，添加战斗环节和结局
    StoryNode node20(20, 
        "\"你终究是选择了自己的老路，你想要回到帝都，重新拥抱权力！\"\n"
        "在回帝都的路上，你却意外的遭遇了埋伏");
    node20.addChoice("开始战斗", 200); // 先进入战斗节点
    node20.GetChoice(1)->Set_choice_handel([&]() {
        auto it = boss.find(4);
        if (it != boss.end()) {
            Fight(player, it->second, true);
        }
        });
    g_storySystem.addNode(node20);

    // 新增节点200：战斗胜利后的结局
    StoryNode node200(200, 
        "你在那场天昏地暗的战斗中取得了胜利。你回到了帝都，虽然力量恢复，但你的傲慢本性未改，最终众叛亲离，在孤独和权力的追逐中迷失了自己。\n"
        "恭喜您完成了结局【我还是我】\n"
        "感谢您的游玩！");
    node200.addChoice("1.重新开始", 0);
    node200.GetChoice(1)->Set_choice_handel(ResetGameData);
    g_storySystem.addNode(node200);
    // 修改节点21
    StoryNode node21(21, "你没有做出选择，但你做出了最好的选择\n"
        "恭喜您完成了结局【没有选择的余生】\n"
        "感谢您的游玩！");
    node21.addChoice("1.重新开始", 0);
    node21.GetChoice(1)->Set_choice_handel(ResetGameData);
    g_storySystem.addNode(node21);
}

// 处理剧情选择事件
void handleChoiceEvent(int nodeId, int choiceId) {

}

// 在main函数中调用此函数来初始化剧情系统
// 可以在游戏开始时或需要的时候触发特定剧情
void StartGameStory() {
    g_storySystem.startStory(0);
}