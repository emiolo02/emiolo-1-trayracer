#define _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW

#include <stdio.h>
#include "window.h"
#include "vec3.h"
#include "raytracer.h"
#include "sphere.h"
#include <chrono>
#include <iostream>

#define degtorad(angle) angle * MPI / 180

#define NO_GL 1
#if NO_GL
void GenerateObjects(int num, Raytracer& rt)
{
    Material mat = Material();
    mat.type = "Lambertian";
    mat.color = { 0.5,0.5,0.5 };
    mat.roughness = 0.3;
    Sphere* ground = new Sphere(1000, { 0,-1000, -1 }, mat);
    rt.AddObject(ground);

    for (int it = 0; it < num; it++)
    {
        {
            Material mat = Material();
            mat.type = "Lambertian";
            float r = RandomFloat();
            float g = RandomFloat();
            float b = RandomFloat();
            mat.color = { r,g,b };
            mat.roughness = RandomFloat();
            const float span = 10.0f;
            Sphere* ground = new Sphere(
                RandomFloat() * 0.7f + 0.2f,
                {
                    RandomFloatNTP() * span,
                    RandomFloat() * span + 0.2f,
                    RandomFloatNTP() * span
                },
                mat);
            rt.AddObject(ground);
        } {
            Material mat = Material();
            mat.type = "Conductor";
            float r = RandomFloat();
            float g = RandomFloat();
            float b = RandomFloat();
            mat.color = { r,g,b };
            mat.roughness = RandomFloat();
            const float span = 30.0f;
            Sphere* ground = new Sphere(
                RandomFloat() * 0.7f + 0.2f,
                {
                    RandomFloatNTP() * span,
                    RandomFloat() * span + 0.2f,
                    RandomFloatNTP() * span
                },
                mat);
            rt.AddObject(ground);
        } {
            Material mat = Material();
            mat.type = "Dielectric";
            float r = RandomFloat();
            float g = RandomFloat();
            float b = RandomFloat();
            mat.color = { r,g,b };
            mat.roughness = RandomFloat();
            mat.refractionIndex = 1.65;
            const float span = 25.0f;
            Sphere* ground = new Sphere(
                RandomFloat() * 0.7f + 0.2f,
                {
                    RandomFloatNTP() * span,
                    RandomFloat() * span + 0.2f,
                    RandomFloatNTP() * span
                },
                mat);
            rt.AddObject(ground);
        }
    }
}
int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(491);

    int width;
    int height;
    int raysPerPixel;
    int maxBounces;
    std::cout << "Image width?" << std::endl;
    std::cin >> width; 
    std::cout << "Image height?" << std::endl;
    std::cin >> height;
    std::cout << "Rays per pixel?" << std::endl;
    std::cin >> raysPerPixel;
    std::cout << "Max bounces?" << std::endl;
    std::cin >> maxBounces;

    std::vector<Color> frameBuffer;
    frameBuffer.resize(width * height);
    
    Raytracer rt = Raytracer(width, height, frameBuffer, raysPerPixel, maxBounces);
    GenerateObjects(12, rt);

    auto start = std::chrono::steady_clock::now();
    rt.Raytrace();
    auto end = std::chrono::steady_clock::now();
    int frameTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << rt.numOfRays << " rays in " << (float)frameTime / 1000 << " ms";;
    return 0;
}
#else
int main()
{
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(491);
    Display::Window wnd;
    
    wnd.SetTitle("TrayRacer");
    
    if (!wnd.Open())
        return 1;

    std::vector<Color> framebuffer;

    const unsigned w = 720*16/9;
    const unsigned h = 720;
    wnd.SetSize(w, h);
    framebuffer.resize(w * h);
    
    int raysPerPixel = 1;
    int maxBounces = 5;

    Raytracer rt = Raytracer(w, h, framebuffer, raysPerPixel, maxBounces);

    // Create some objects
    Material mat = Material();
    mat.type = "Lambertian";
    mat.color = { 0.5,0.5,0.5 };
    mat.roughness = 0.3;
    Sphere* ground = new Sphere(1000, { 0,-1000, -1 }, mat);
    rt.AddObject(ground);

    for (int it = 0; it < 12; it++)
    {
        {
            Material mat = Material();
            mat.type = "Lambertian";
            float r = RandomFloat();
            float g = RandomFloat();
            float b = RandomFloat();
            mat.color = { r,g,b };
            mat.roughness = RandomFloat();
            const float span = 10.0f;
            Sphere* ground = new Sphere(
                RandomFloat() * 0.7f + 0.2f,
                {
                    RandomFloatNTP() * span,
                    RandomFloat() * span + 0.2f,
                    RandomFloatNTP() * span
                },
                mat);
            rt.AddObject(ground);
        } {
            Material mat = Material();
            mat.type = "Conductor";
            float r = RandomFloat();
            float g = RandomFloat();
            float b = RandomFloat();
            mat.color = { r,g,b };
            mat.roughness = RandomFloat();
            const float span = 30.0f;
            Sphere* ground = new Sphere(
                RandomFloat() * 0.7f + 0.2f,
                {
                    RandomFloatNTP() * span,
                    RandomFloat() * span + 0.2f,
                    RandomFloatNTP() * span
                },
                mat);
            rt.AddObject(ground);
        } {
            Material mat = Material();
            mat.type = "Dielectric";
            float r = RandomFloat();
            float g = RandomFloat();
            float b = RandomFloat();
            mat.color = { r,g,b };
            mat.roughness = RandomFloat();
            mat.refractionIndex = 1.65;
            const float span = 25.0f;
            Sphere* ground = new Sphere(
                RandomFloat() * 0.7f + 0.2f,
                {
                    RandomFloatNTP() * span,
                    RandomFloat() * span + 0.2f,
                    RandomFloatNTP() * span
                },
                mat);
            rt.AddObject(ground);
        }
    }
    
    bool exit = false;

    // camera
    bool resetFramebuffer = false;
    vec3 camPos = { 0,1.0f,10.0f };
    vec3 moveDir = { 0,0,0 };

    wnd.SetKeyPressFunction([&exit, &moveDir, &resetFramebuffer](int key, int scancode, int action, int mods)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            exit = true;
            break;
        case GLFW_KEY_W:
            moveDir.z -= 1.0f;
            resetFramebuffer |= true;
            break;
        case GLFW_KEY_S:
            moveDir.z += 1.0f;
            resetFramebuffer |= true;
            break;
        case GLFW_KEY_A:
            moveDir.x -= 1.0f;
            resetFramebuffer |= true;
            break;
        case GLFW_KEY_D:
            moveDir.x += 1.0f;
            resetFramebuffer |= true;
            break;
        case GLFW_KEY_SPACE:
            moveDir.y += 1.0f;
            resetFramebuffer |= true;
            break;
        case GLFW_KEY_LEFT_CONTROL:
            moveDir.y -= 1.0f;
            resetFramebuffer |= true;
            break;
        default:
            break;
        }
    });

    float pitch = 0;
    float yaw = 0;
    float oldx = 0;
    float oldy = 0;

    wnd.SetMouseMoveFunction([&pitch, &yaw, &oldx, &oldy, &resetFramebuffer](double x, double y)
    {
        x *= -0.1;
        y *= -0.1;
        yaw = x - oldx;
        pitch = y - oldy;
        resetFramebuffer |= true;
        oldx = x;
        oldy = y;
    });

    float rotx = 0;
    float roty = 0;

    // number of accumulated frames
    int frameIndex = 0;

    std::vector<Color> framebufferCopy;
    framebufferCopy.resize(w * h);

    // rendering loop
    while (wnd.IsOpen() && !exit)
    {
        resetFramebuffer = false;
        moveDir = {0,0,0};
        pitch = 0;
        yaw = 0;

        // poll input
        wnd.Update();

        rotx -= pitch;
        roty -= yaw;

        moveDir = normalize(moveDir);

        mat4 xMat = (rotationx(rotx));
        mat4 yMat = (rotationy(roty));
        mat4 cameraTransform = multiply(yMat, xMat);

        camPos = camPos + transform(moveDir * 0.2f, cameraTransform);
        
        cameraTransform.m30 = camPos.x;
        cameraTransform.m31 = camPos.y;
        cameraTransform.m32 = camPos.z;

        rt.SetViewMatrix(cameraTransform);
        
        if (resetFramebuffer)
        {
            rt.Clear();
            frameIndex = 0;
        }
        auto start = std::chrono::steady_clock::now();
        rt.Raytrace();
        auto end = std::chrono::steady_clock::now();
        int frameTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        system("cls");
        std::cout << (float)frameTime/1000 << " ms\n";
        std::cout << 1000000.0f / frameTime << " FPS\n";
        frameIndex++;

        // Get the average distribution of all samples
        {
            size_t p = 0;
            for (Color const& pixel : framebuffer)
            {
                framebufferCopy[p] = pixel;
                framebufferCopy[p].r /= frameIndex;
                framebufferCopy[p].g /= frameIndex;
                framebufferCopy[p].b /= frameIndex;
                p++;
            }
        }

        glClearColor(0, 0, 0, 1.0);
        glClear( GL_COLOR_BUFFER_BIT );

        wnd.Blit((float*)&framebufferCopy[0], w, h);
        wnd.SwapBuffers();
    }

    if (wnd.IsOpen())
        wnd.Close();

    
    return 0;
} 
#endif