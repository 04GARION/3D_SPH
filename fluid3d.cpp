// fluid3d.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include "global.hpp"
#include "ParticleSystem.hpp"
#include "Renderer.hpp"

int main()//Note!
{
    fluid::ParticleSystem system;
    system.SetContainerSize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.6f, 0.6f, 0.6f));
    system.AddFluidBlock(glm::vec3(0.05f, 0.35f, 0.25f), glm::vec3(0.15f, 0.15f, 0.3f), glm::vec3(0.0f, -1.0f, 0.0f), 0.01 * 0.8);
    system.AddFluidBlock(glm::vec3(0.35f, 0.05f, 0.25f), glm::vec3(0.15f, 0.15f, 0.3f), glm::vec3(0.0f, -1.0f, 0.0f), 0.01 * 0.8);
    system.SortBlock();

    fluid::Renderer renderer;
    renderer.Init();
    renderer.UploadUniforms(system);

    while (1)//Note!
    {
        for (int i = 0; i < params::substep; i++)
        {
            system.SortBlock();
            renderer.UploadParticleInfo(system);
            renderer.Solve();
            renderer.DumpParticleInfo(system);
        }
        renderer.Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
